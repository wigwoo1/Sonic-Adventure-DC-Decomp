#include "sa_draw_motion_stop.h"
/* Lit alternate strip emitters with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_lit_alternate_strip_8c6136c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6136c0;
L8c6136c0: sa_motion_publish_stop(ctx,0x8c6136c0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6136c2; /* mov.l r14,@-r15 */
L8c6136c2: sa_motion_publish_stop(ctx,0x8c6136c2u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6136c4; /* mov.l r13,@-r15 */
L8c6136c4: sa_motion_publish_stop(ctx,0x8c6136c4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6136c6; /* mov.l r12,@-r15 */
L8c6136c6: sa_motion_publish_stop(ctx,0x8c6136c6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6136c8; /* mov.l r11,@-r15 */
L8c6136c8: sa_motion_publish_stop(ctx,0x8c6136c8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6136ca; /* mov.l r10,@-r15 */
L8c6136ca: sa_motion_publish_stop(ctx,0x8c6136cau); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6136cc; /* mov.l r9,@-r15 */
L8c6136cc: sa_motion_publish_stop(ctx,0x8c6136ccu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6136ce; /* fmov.s fr15,@-r15 */
L8c6136ce: sa_motion_publish_stop(ctx,0x8c6136ceu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6136d0; /* fmov.s fr14,@-r15 */
L8c6136d0: sa_motion_publish_stop(ctx,0x8c6136d0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6136d2; /* fmov.s fr13,@-r15 */
L8c6136d2: sa_motion_publish_stop(ctx,0x8c6136d2u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6136d4; /* sts.l PR,@-r15 */
L8c6136d4: sa_motion_publish_stop(ctx,0x8c6136d4u); r[15]+=4294967280u;goto L8c6136d6; /* add #-0x10,r15 */
L8c6136d6: sa_motion_publish_stop(ctx,0x8c6136d6u); r[3]=rd(m,bus,&io_status,0x8c613718u,4);if(io_status)return io_status;goto L8c6136d8; /* mov.l 0x8c613718,r3 */
L8c6136d8: sa_motion_publish_stop(ctx,0x8c6136d8u); r[9]=rd(m,bus,&io_status,0x8c613714u,4);if(io_status)return io_status;goto L8c6136da; /* mov.l 0x8c613714,r9 */
L8c6136da: sa_motion_publish_stop(ctx,0x8c6136dcu); target=r[3];*pr=0x8c6136deu;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6136de; /* jsr @r3 */
L8c6136de: sa_motion_publish_stop(ctx,0x8c6136deu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6136e0; /* mov.l @(0x4,r15),r3 */
L8c6136e0: sa_motion_publish_stop(ctx,0x8c6136e0u); r[2]=0u;goto L8c6136e2; /* mov #0x0,r2 */
L8c6136e2: sa_motion_publish_stop(ctx,0x8c6136e2u); r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6136e4; /* mov.l @(0x4,r15),r14 */
L8c6136e4: sa_motion_publish_stop(ctx,0x8c6136e4u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6136e6; /* mov.w @(0x2,r3),r0 */
L8c6136e6: sa_motion_publish_stop(ctx,0x8c6136e6u); r[3]=rd(m,bus,&io_status,0x8c61371cu,4);if(io_status)return io_status;goto L8c6136e8; /* mov.l 0x8c61371c,r3 */
L8c6136e8: sa_motion_publish_stop(ctx,0x8c6136e8u); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6136ea; /* mov.l @(0x4,r14),r14 */
L8c6136ea: sa_motion_publish_stop(ctx,0x8c6136eau); r[0]=r[0]&65535u;goto L8c6136ec; /* extu.w r0,r0 */
L8c6136ec: sa_motion_publish_stop(ctx,0x8c6136ecu); wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6136ee; /* mov.l r0,@(0xc,r15) */
L8c6136ee: sa_motion_publish_stop(ctx,0x8c6136eeu); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6136f0; /* mov.l @r3,r13 */
L8c6136f0: sa_motion_publish_stop(ctx,0x8c6136f0u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6136f2; /* mov.l r2,@(0x8,r15) */
L8c6136f2: sa_motion_publish_stop(ctx,0x8c6136f2u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6136f4; /* mov.l r2,@(0x4,r15) */
L8c6136f4: sa_motion_publish_stop(ctx,0x8c6136f6u); goto L8c613720; /* bra 0x8c613720 */
L8c613720: sa_motion_publish_stop(ctx,0x8c613720u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613722; /* mov.w @r14+,r3 */
L8c613722: sa_motion_publish_stop(ctx,0x8c613722u); r[4]=rd(m,bus,&io_status,0x8c61380cu,4);if(io_status)return io_status;goto L8c613724; /* mov.l 0x8c61380c,r4 */
L8c613724: sa_motion_publish_stop(ctx,0x8c613724u); r[3]=r[3]&65535u;goto L8c613726; /* extu.w r3,r3 */
L8c613726: sa_motion_publish_stop(ctx,0x8c613726u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613728; /* fldi1 fr4 */
L8c613728: sa_motion_publish_stop(ctx,0x8c613728u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c61372a; /* tst r4,r3 */
L8c61372a: sa_motion_publish_stop(ctx,0x8c61372au); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61372c; /* mov.l r3,@r15 */
L8c61372c: sa_motion_publish_stop(ctx,0x8c61372eu); take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c61373e;goto L8c613730; /* bt/s 0x8c61373e */
L8c613730: sa_motion_publish_stop(ctx,0x8c613730u); r[4]=sx(m,bus,&io_status,0x8c613808u,2);if(io_status)return io_status;goto L8c613732; /* mov.w 0x8c613808,r4 */
L8c613732: sa_motion_publish_stop(ctx,0x8c613732u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613734; /* mov.l @r15,r3 */
L8c613734: sa_motion_publish_stop(ctx,0x8c613734u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613736; /* fldi1 fr4 */
L8c613736: sa_motion_publish_stop(ctx,0x8c613736u); r[3]&=r[4];goto L8c613738; /* and r4,r3 */
L8c613738: sa_motion_publish_stop(ctx,0x8c613738u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c61373a; /* fneg fr4 */
L8c61373a: sa_motion_publish_stop(ctx,0x8c61373au); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61373c; /* mov.l r3,@r15 */
L8c61373c: sa_motion_publish_stop(ctx,0x8c61373cu); fcopy(f,15u,4u);goto L8c61373e; /* fmov fr4,fr15 */
L8c61373e: sa_motion_publish_stop(ctx,0x8c61373eu); r[0]=16u;goto L8c613740; /* mov #0x10,r0 */
L8c613740: sa_motion_publish_stop(ctx,0x8c613740u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613742; /* mov.l @r15,r3 */
L8c613742: sa_motion_publish_stop(ctx,0x8c613742u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613744; /* mov.l @(0x4,r15),r2 */
L8c613744: sa_motion_publish_stop(ctx,0x8c613744u); r[10]=r[8];goto L8c613746; /* mov r8,r10 */
L8c613746: sa_motion_publish_stop(ctx,0x8c613746u); r[2]+=r[3];goto L8c613748; /* add r3,r2 */
L8c613748: sa_motion_publish_stop(ctx,0x8c613748u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61374a; /* mov.l r2,@(0x4,r15) */
L8c61374a: sa_motion_publish_stop(ctx,0x8c61374au); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61374c; /* mov.l @r15,r1 */
L8c61374c: sa_motion_publish_stop(ctx,0x8c61374cu); r[1]+=4294967294u;goto L8c61374e; /* add #-0x2,r1 */
L8c61374e: sa_motion_publish_stop(ctx,0x8c61374eu); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c613750; /* mov.l r1,@r15 */
L8c613750: sa_motion_publish_stop(ctx,0x8c613750u); r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613752; /* mov.w @r14+,r4 */
L8c613752: sa_motion_publish_stop(ctx,0x8c613752u); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613754; /* mov.w @r14+,r11 */
L8c613754: sa_motion_publish_stop(ctx,0x8c613754u); r[4]=r[4]&65535u;goto L8c613756; /* extu.w r4,r4 */
L8c613756: sa_motion_publish_stop(ctx,0x8c613756u); r[4]<<=2;goto L8c613758; /* shll2 r4 */
L8c613758: sa_motion_publish_stop(ctx,0x8c613758u); r[11]=r[11]&65535u;goto L8c61375a; /* extu.w r11,r11 */
L8c61375a: sa_motion_publish_stop(ctx,0x8c61375au); r[11]<<=2;goto L8c61375c; /* shll2 r11 */
L8c61375c: sa_motion_publish_stop(ctx,0x8c61375cu); r[4]<<=2;goto L8c61375e; /* shll2 r4 */
L8c61375e: sa_motion_publish_stop(ctx,0x8c61375eu); r[11]<<=2;goto L8c613760; /* shll2 r11 */
L8c613760: sa_motion_publish_stop(ctx,0x8c613760u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c613762; /* shll r4 */
L8c613762: sa_motion_publish_stop(ctx,0x8c613762u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c613764; /* shll r11 */
L8c613764: sa_motion_publish_stop(ctx,0x8c613764u); r[11]+=r[13];goto L8c613766; /* add r13,r11 */
L8c613766: sa_motion_publish_stop(ctx,0x8c613766u); r[4]+=r[13];goto L8c613768; /* add r13,r4 */
L8c613768: sa_motion_publish_stop(ctx,0x8c613768u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c61376a; /* fmov.s @(r0,r11),fr14 */
L8c61376a: sa_motion_publish_stop(ctx,0x8c61376au); fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c61376c; /* fmov.s @(r0,r4),fr4 */
L8c61376c: sa_motion_publish_stop(ctx,0x8c61376cu); r[0]=20u;goto L8c61376e; /* mov #0x14,r0 */
L8c61376e: sa_motion_publish_stop(ctx,0x8c61376eu); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c613770; /* fmov.s @(r0,r11),fr13 */
L8c613770: sa_motion_publish_stop(ctx,0x8c613770u); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c613772; /* fmov.s @(r0,r4),fr5 */
L8c613772: sa_motion_publish_stop(ctx,0x8c613772u); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613774; /* mov.w @r14+,r12 */
L8c613774: sa_motion_publish_stop(ctx,0x8c613774u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c613776; /* mov.w @r14,r3 */
L8c613776: sa_motion_publish_stop(ctx,0x8c613776u); r[12]=r[12]&65535u;goto L8c613778; /* extu.w r12,r12 */
L8c613778: sa_motion_publish_stop(ctx,0x8c613778u); r[12]<<=2;goto L8c61377a; /* shll2 r12 */
L8c61377a: sa_motion_publish_stop(ctx,0x8c61377au); r[3]=r[3]&65535u;goto L8c61377c; /* extu.w r3,r3 */
L8c61377c: sa_motion_publish_stop(ctx,0x8c61377cu); r[3]<<=2;goto L8c61377e; /* shll2 r3 */
L8c61377e: sa_motion_publish_stop(ctx,0x8c61377eu); r[12]<<=2;goto L8c613780; /* shll2 r12 */
L8c613780: sa_motion_publish_stop(ctx,0x8c613780u); r[3]<<=2;goto L8c613782; /* shll2 r3 */
L8c613782: sa_motion_publish_stop(ctx,0x8c613782u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c613784; /* shll r12 */
L8c613784: sa_motion_publish_stop(ctx,0x8c613784u); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c613786; /* shll r3 */
L8c613786: sa_motion_publish_stop(ctx,0x8c613786u); r[3]+=r[13];goto L8c613788; /* add r13,r3 */
L8c613788: sa_motion_publish_stop(ctx,0x8c613788u); r[12]+=r[13];goto L8c61378a; /* add r13,r12 */
L8c61378a: sa_motion_publish_stop(ctx,0x8c61378au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c61378c; /* pref @r3 */
L8c61378c: sa_motion_publish_stop(ctx,0x8c61378cu); r[0]=20u;goto L8c61378e; /* mov #0x14,r0 */
L8c61378e: sa_motion_publish_stop(ctx,0x8c61378eu); fcopy(f,3u,14u);goto L8c613790; /* fmov fr14,fr3 */
L8c613790: sa_motion_publish_stop(ctx,0x8c613790u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c613792; /* fmov.s @(r0,r12),fr2 */
L8c613792: sa_motion_publish_stop(ctx,0x8c613792u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613794; /* fsub fr4,fr3 */
L8c613794: sa_motion_publish_stop(ctx,0x8c613794u); r[0]=16u;goto L8c613796; /* mov #0x10,r0 */
L8c613796: sa_motion_publish_stop(ctx,0x8c613796u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613798; /* cmp/hs r8,r10 */
L8c613798: sa_motion_publish_stop(ctx,0x8c613798u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61379a; /* fsub fr13,fr2 */
L8c61379a: sa_motion_publish_stop(ctx,0x8c61379au); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c61379c; /* fmov.s @(r0,r12),fr1 */
L8c61379c: sa_motion_publish_stop(ctx,0x8c61379cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61379e; /* fsub fr14,fr1 */
L8c61379e: sa_motion_publish_stop(ctx,0x8c61379eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6137a0; /* fmul fr2,fr3 */
L8c6137a0: sa_motion_publish_stop(ctx,0x8c6137a0u); fcopy(f,2u,13u);goto L8c6137a2; /* fmov fr13,fr2 */
L8c6137a2: sa_motion_publish_stop(ctx,0x8c6137a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6137a4; /* fsub fr5,fr2 */
L8c6137a4: sa_motion_publish_stop(ctx,0x8c6137a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6137a6; /* fmul fr1,fr2 */
L8c6137a6: sa_motion_publish_stop(ctx,0x8c6137a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6137a8; /* fsub fr2,fr3 */
L8c6137a8: sa_motion_publish_stop(ctx,0x8c6137a8u); fcopy(f,4u,3u);goto L8c6137aa; /* fmov fr3,fr4 */
L8c6137aa: sa_motion_publish_stop(ctx,0x8c6137acu); take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c613810;goto L8c6137ae; /* bt/s 0x8c613810 */
L8c6137ae: sa_motion_publish_stop(ctx,0x8c6137aeu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6137b0; /* fldi0 fr3 */
L8c6137b0: sa_motion_publish_stop(ctx,0x8c6137b0u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6137b2; /* fcmp/gt fr4,fr3 */
L8c6137b2: sa_motion_publish_stop(ctx,0x8c6137b2u); if((*sr&1u))goto L8c613856;goto L8c6137b4; /* bt 0x8c613856 */
L8c6137b4: sa_motion_publish_stop(ctx,0x8c6137b4u); r[4]=r[11];goto L8c6137b6; /* mov r11,r4 */
L8c6137b6: sa_motion_publish_stop(ctx,0x8c6137b6u); r[5]=r[9];goto L8c6137b8; /* mov r9,r5 */
L8c6137b8: sa_motion_publish_stop(ctx,0x8c6137b8u); r[8]+=16u;goto L8c6137ba; /* add #0x10,r8 */
L8c6137ba: sa_motion_publish_stop(ctx,0x8c6137bau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6137bc; /* fschg */
L8c6137bc: sa_motion_publish_stop(ctx,0x8c6137bcu); fcopy(f,0u,1u);goto L8c6137be; /* fmov fr1,fr0 */
L8c6137be: sa_motion_publish_stop(ctx,0x8c6137beu); fcopy(f,2u,3u);goto L8c6137c0; /* fmov fr3,fr2 */
L8c6137c0: sa_motion_publish_stop(ctx,0x8c6137c0u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6137c2; /* fmov.s @r4+,fr4 */
L8c6137c2: sa_motion_publish_stop(ctx,0x8c6137c2u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6137c4; /* fmov.s @r4+,fr6 */
L8c6137c4: sa_motion_publish_stop(ctx,0x8c6137c4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6137c6; /* fldi0 fr3 */
L8c6137c6: sa_motion_publish_stop(ctx,0x8c6137c6u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c6137c8; /* fipr fv4,fv0 */
L8c6137c8: sa_motion_publish_stop(ctx,0x8c6137c8u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6137ca; /* fmov.s @r4+,fr8 */
L8c6137ca: sa_motion_publish_stop(ctx,0x8c6137cau); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6137cc; /* fmov.s @r4+,fr10 */
L8c6137cc: sa_motion_publish_stop(ctx,0x8c6137ccu); fcopy(f,0u,9u);goto L8c6137ce; /* fmov fr9,fr0 */
L8c6137ce: sa_motion_publish_stop(ctx,0x8c6137ceu); fcopy(f,4u,5u);goto L8c6137d0; /* fmov fr5,fr4 */
L8c6137d0: sa_motion_publish_stop(ctx,0x8c6137d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6137d2; /* fsub fr3,fr0 */
L8c6137d2: sa_motion_publish_stop(ctx,0x8c6137d2u); fcopy(f,6u,7u);goto L8c6137d4; /* fmov fr7,fr6 */
L8c6137d4: sa_motion_publish_stop(ctx,0x8c6137d4u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6137d6; /* fschg */
L8c6137d6: sa_motion_publish_stop(ctx,0x8c6137d6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c6137d8; /* fmov.s fr10,@-r8 */
L8c6137d8: sa_motion_publish_stop(ctx,0x8c6137d8u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6137da; /* fmov.s fr0,@-r4 */
L8c6137da: sa_motion_publish_stop(ctx,0x8c6137dau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c6137dc; /* ocbwb @r4 */
L8c6137dc: sa_motion_publish_stop(ctx,0x8c6137dcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6137de; /* fmul fr0,fr7 */
L8c6137de: sa_motion_publish_stop(ctx,0x8c6137deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c6137e0; /* fmov.s fr9,@-r8 */
L8c6137e0: sa_motion_publish_stop(ctx,0x8c6137e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6137e2; /* fmul fr0,fr6 */
L8c6137e2: sa_motion_publish_stop(ctx,0x8c6137e2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c6137e4; /* fmov.s fr8,@-r8 */
L8c6137e4: sa_motion_publish_stop(ctx,0x8c6137e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6137e6; /* fmul fr0,fr5 */
L8c6137e6: sa_motion_publish_stop(ctx,0x8c6137e6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6137e8; /* mov.l r5,@-r8 */
L8c6137e8: sa_motion_publish_stop(ctx,0x8c6137e8u); r[8]+=32u;goto L8c6137ea; /* add #0x20,r8 */
L8c6137ea: sa_motion_publish_stop(ctx,0x8c6137eau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c6137ec; /* fmov.s fr7,@-r8 */
L8c6137ec: sa_motion_publish_stop(ctx,0x8c6137ecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6137ee; /* fmov.s fr6,@-r8 */
L8c6137ee: sa_motion_publish_stop(ctx,0x8c6137eeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6137f0; /* fmov.s fr5,@-r8 */
L8c6137f0: sa_motion_publish_stop(ctx,0x8c6137f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6137f2; /* fmov.s fr4,@-r8 */
L8c6137f2: sa_motion_publish_stop(ctx,0x8c6137f2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6137f4; /* pref @r8 */
L8c6137f4: sa_motion_publish_stop(ctx,0x8c6137f4u); r[8]+=16u;goto L8c6137f6; /* add #0x10,r8 */
L8c6137f6: sa_motion_publish_stop(ctx,0x8c6137f6u); r[3]=r[8];goto L8c6137f8; /* mov r8,r3 */
L8c6137f8: sa_motion_publish_stop(ctx,0x8c6137f8u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6137fa; /* mov.l @(0x8,r15),r2 */
L8c6137fa: sa_motion_publish_stop(ctx,0x8c6137fau); r[3]-=r[10];goto L8c6137fc; /* sub r10,r3 */
L8c6137fc: sa_motion_publish_stop(ctx,0x8c6137fcu); r[3]+=32u;goto L8c6137fe; /* add #0x20,r3 */
L8c6137fe: sa_motion_publish_stop(ctx,0x8c6137feu); r[2]+=r[3];goto L8c613800; /* add r3,r2 */
L8c613800: sa_motion_publish_stop(ctx,0x8c613800u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613802; /* mov.l r2,@(0x8,r15) */
L8c613802: sa_motion_publish_stop(ctx,0x8c613804u); r[10]=r[8];goto L8c613898; /* bra 0x8c613898 */
L8c613810: sa_motion_publish_stop(ctx,0x8c613810u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613812; /* fldi0 fr3 */
L8c613812: sa_motion_publish_stop(ctx,0x8c613812u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613814; /* fcmp/gt fr4,fr3 */
L8c613814: sa_motion_publish_stop(ctx,0x8c613814u); if(!(*sr&1u))goto L8c613898;goto L8c613816; /* bf 0x8c613898 */
L8c613816: sa_motion_publish_stop(ctx,0x8c613816u); r[5]=rd(m,bus,&io_status,0x8c613984u,4);if(io_status)return io_status;goto L8c613818; /* mov.l 0x8c613984,r5 */
L8c613818: sa_motion_publish_stop(ctx,0x8c613818u); r[8]+=16u;goto L8c61381a; /* add #0x10,r8 */
L8c61381a: sa_motion_publish_stop(ctx,0x8c61381au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61381c; /* fschg */
L8c61381c: sa_motion_publish_stop(ctx,0x8c61381cu); fcopy(f,0u,1u);goto L8c61381e; /* fmov fr1,fr0 */
L8c61381e: sa_motion_publish_stop(ctx,0x8c61381eu); fcopy(f,2u,3u);goto L8c613820; /* fmov fr3,fr2 */
L8c613820: sa_motion_publish_stop(ctx,0x8c613820u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613822; /* fmov.s @r4+,fr4 */
L8c613822: sa_motion_publish_stop(ctx,0x8c613822u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613824; /* fmov.s @r4+,fr6 */
L8c613824: sa_motion_publish_stop(ctx,0x8c613824u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613826; /* fldi0 fr3 */
L8c613826: sa_motion_publish_stop(ctx,0x8c613826u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c613828; /* fipr fv4,fv0 */
L8c613828: sa_motion_publish_stop(ctx,0x8c613828u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61382a; /* fmov.s @r4+,fr8 */
L8c61382a: sa_motion_publish_stop(ctx,0x8c61382au); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61382c; /* fmov.s @r4+,fr10 */
L8c61382c: sa_motion_publish_stop(ctx,0x8c61382cu); fcopy(f,0u,9u);goto L8c61382e; /* fmov fr9,fr0 */
L8c61382e: sa_motion_publish_stop(ctx,0x8c61382eu); fcopy(f,4u,5u);goto L8c613830; /* fmov fr5,fr4 */
L8c613830: sa_motion_publish_stop(ctx,0x8c613830u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c613832; /* fsub fr3,fr0 */
L8c613832: sa_motion_publish_stop(ctx,0x8c613832u); fcopy(f,6u,7u);goto L8c613834; /* fmov fr7,fr6 */
L8c613834: sa_motion_publish_stop(ctx,0x8c613834u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613836; /* fschg */
L8c613836: sa_motion_publish_stop(ctx,0x8c613836u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c613838; /* fmov.s fr10,@-r8 */
L8c613838: sa_motion_publish_stop(ctx,0x8c613838u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61383a; /* fmov.s fr0,@-r4 */
L8c61383a: sa_motion_publish_stop(ctx,0x8c61383au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c61383c; /* ocbwb @r4 */
L8c61383c: sa_motion_publish_stop(ctx,0x8c61383cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c61383e; /* fmul fr0,fr7 */
L8c61383e: sa_motion_publish_stop(ctx,0x8c61383eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c613840; /* fmov.s fr9,@-r8 */
L8c613840: sa_motion_publish_stop(ctx,0x8c613840u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c613842; /* fmul fr0,fr6 */
L8c613842: sa_motion_publish_stop(ctx,0x8c613842u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c613844; /* fmov.s fr8,@-r8 */
L8c613844: sa_motion_publish_stop(ctx,0x8c613844u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613846; /* fmul fr0,fr5 */
L8c613846: sa_motion_publish_stop(ctx,0x8c613846u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613848; /* mov.l r5,@-r8 */
L8c613848: sa_motion_publish_stop(ctx,0x8c613848u); r[8]+=32u;goto L8c61384a; /* add #0x20,r8 */
L8c61384a: sa_motion_publish_stop(ctx,0x8c61384au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61384c; /* fmov.s fr7,@-r8 */
L8c61384c: sa_motion_publish_stop(ctx,0x8c61384cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c61384e; /* fmov.s fr6,@-r8 */
L8c61384e: sa_motion_publish_stop(ctx,0x8c61384eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613850; /* fmov.s fr5,@-r8 */
L8c613850: sa_motion_publish_stop(ctx,0x8c613850u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613852; /* fmov.s fr4,@-r8 */
L8c613852: sa_motion_publish_stop(ctx,0x8c613852u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613854; /* pref @r8 */
L8c613854: sa_motion_publish_stop(ctx,0x8c613854u); r[8]+=16u;goto L8c613856; /* add #0x10,r8 */
L8c613856: sa_motion_publish_stop(ctx,0x8c613856u); r[5]=rd(m,bus,&io_status,0x8c613984u,4);if(io_status)return io_status;goto L8c613858; /* mov.l 0x8c613984,r5 */
L8c613858: sa_motion_publish_stop(ctx,0x8c613858u); r[4]=r[11];goto L8c61385a; /* mov r11,r4 */
L8c61385a: sa_motion_publish_stop(ctx,0x8c61385au); r[8]+=16u;goto L8c61385c; /* add #0x10,r8 */
L8c61385c: sa_motion_publish_stop(ctx,0x8c61385cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61385e; /* fschg */
L8c61385e: sa_motion_publish_stop(ctx,0x8c61385eu); fcopy(f,0u,1u);goto L8c613860; /* fmov fr1,fr0 */
L8c613860: sa_motion_publish_stop(ctx,0x8c613860u); fcopy(f,2u,3u);goto L8c613862; /* fmov fr3,fr2 */
L8c613862: sa_motion_publish_stop(ctx,0x8c613862u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613864; /* fmov.s @r4+,fr4 */
L8c613864: sa_motion_publish_stop(ctx,0x8c613864u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613866; /* fmov.s @r4+,fr6 */
L8c613866: sa_motion_publish_stop(ctx,0x8c613866u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613868; /* fldi0 fr3 */
L8c613868: sa_motion_publish_stop(ctx,0x8c613868u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c61386a; /* fipr fv4,fv0 */
L8c61386a: sa_motion_publish_stop(ctx,0x8c61386au); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61386c; /* fmov.s @r4+,fr8 */
L8c61386c: sa_motion_publish_stop(ctx,0x8c61386cu); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61386e; /* fmov.s @r4+,fr10 */
L8c61386e: sa_motion_publish_stop(ctx,0x8c61386eu); fcopy(f,0u,9u);goto L8c613870; /* fmov fr9,fr0 */
L8c613870: sa_motion_publish_stop(ctx,0x8c613870u); fcopy(f,4u,5u);goto L8c613872; /* fmov fr5,fr4 */
L8c613872: sa_motion_publish_stop(ctx,0x8c613872u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c613874; /* fsub fr3,fr0 */
L8c613874: sa_motion_publish_stop(ctx,0x8c613874u); fcopy(f,6u,7u);goto L8c613876; /* fmov fr7,fr6 */
L8c613876: sa_motion_publish_stop(ctx,0x8c613876u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613878; /* fschg */
L8c613878: sa_motion_publish_stop(ctx,0x8c613878u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c61387a; /* fmov.s fr10,@-r8 */
L8c61387a: sa_motion_publish_stop(ctx,0x8c61387au); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61387c; /* fmov.s fr0,@-r4 */
L8c61387c: sa_motion_publish_stop(ctx,0x8c61387cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c61387e; /* ocbwb @r4 */
L8c61387e: sa_motion_publish_stop(ctx,0x8c61387eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c613880; /* fmul fr0,fr7 */
L8c613880: sa_motion_publish_stop(ctx,0x8c613880u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c613882; /* fmov.s fr9,@-r8 */
L8c613882: sa_motion_publish_stop(ctx,0x8c613882u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c613884; /* fmul fr0,fr6 */
L8c613884: sa_motion_publish_stop(ctx,0x8c613884u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c613886; /* fmov.s fr8,@-r8 */
L8c613886: sa_motion_publish_stop(ctx,0x8c613886u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613888; /* fmul fr0,fr5 */
L8c613888: sa_motion_publish_stop(ctx,0x8c613888u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61388a; /* mov.l r5,@-r8 */
L8c61388a: sa_motion_publish_stop(ctx,0x8c61388au); r[8]+=32u;goto L8c61388c; /* add #0x20,r8 */
L8c61388c: sa_motion_publish_stop(ctx,0x8c61388cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61388e; /* fmov.s fr7,@-r8 */
L8c61388e: sa_motion_publish_stop(ctx,0x8c61388eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613890; /* fmov.s fr6,@-r8 */
L8c613890: sa_motion_publish_stop(ctx,0x8c613890u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613892; /* fmov.s fr5,@-r8 */
L8c613892: sa_motion_publish_stop(ctx,0x8c613892u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613894; /* fmov.s fr4,@-r8 */
L8c613894: sa_motion_publish_stop(ctx,0x8c613894u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613896; /* pref @r8 */
L8c613896: sa_motion_publish_stop(ctx,0x8c613896u); r[8]+=16u;goto L8c613898; /* add #0x10,r8 */
L8c613898: sa_motion_publish_stop(ctx,0x8c613898u); fcopy(f,4u,14u);goto L8c61389a; /* fmov fr14,fr4 */
L8c61389a: sa_motion_publish_stop(ctx,0x8c61389au); r[0]=16u;goto L8c61389c; /* mov #0x10,r0 */
L8c61389c: sa_motion_publish_stop(ctx,0x8c61389cu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61389e; /* mov.l @r15,r3 */
L8c61389e: sa_motion_publish_stop(ctx,0x8c61389eu); r[4]=r[11];goto L8c6138a0; /* mov r11,r4 */
L8c6138a0: sa_motion_publish_stop(ctx,0x8c6138a0u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c6138a2; /* fmov.s @(r0,r12),fr14 */
L8c6138a2: sa_motion_publish_stop(ctx,0x8c6138a2u); r[0]=20u;goto L8c6138a4; /* mov #0x14,r0 */
L8c6138a4: sa_motion_publish_stop(ctx,0x8c6138a4u); fcopy(f,5u,13u);goto L8c6138a6; /* fmov fr13,fr5 */
L8c6138a6: sa_motion_publish_stop(ctx,0x8c6138a6u); r[3]+=4294967295u;goto L8c6138a8; /* add #-0x1,r3 */
L8c6138a8: sa_motion_publish_stop(ctx,0x8c6138a8u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c6138aa; /* fmov.s @(r0,r12),fr13 */
L8c6138aa: sa_motion_publish_stop(ctx,0x8c6138aau); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6138ac; /* tst r3,r3 */
L8c6138ac: sa_motion_publish_stop(ctx,0x8c6138acu); r[11]=r[12];goto L8c6138ae; /* mov r12,r11 */
L8c6138ae: sa_motion_publish_stop(ctx,0x8c6138aeu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c6138b0; /* fneg fr15 */
L8c6138b0: sa_motion_publish_stop(ctx,0x8c6138b2u); take=(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c6138b8;goto L8c6138b4; /* bt/s 0x8c6138b8 */
L8c6138b4: sa_motion_publish_stop(ctx,0x8c6138b6u); goto L8c613772; /* bra 0x8c613772 */
L8c6138b8: sa_motion_publish_stop(ctx,0x8c6138b8u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c6138ba; /* cmp/hs r8,r10 */
L8c6138ba: sa_motion_publish_stop(ctx,0x8c6138bau); if((*sr&1u))goto L8c61390a;goto L8c6138bc; /* bt 0x8c61390a */
L8c6138bc: sa_motion_publish_stop(ctx,0x8c6138bcu); r[4]=r[12];goto L8c6138be; /* mov r12,r4 */
L8c6138be: sa_motion_publish_stop(ctx,0x8c6138beu); r[5]=r[9];goto L8c6138c0; /* mov r9,r5 */
L8c6138c0: sa_motion_publish_stop(ctx,0x8c6138c0u); r[8]+=16u;goto L8c6138c2; /* add #0x10,r8 */
L8c6138c2: sa_motion_publish_stop(ctx,0x8c6138c2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6138c4; /* fschg */
L8c6138c4: sa_motion_publish_stop(ctx,0x8c6138c4u); fcopy(f,0u,1u);goto L8c6138c6; /* fmov fr1,fr0 */
L8c6138c6: sa_motion_publish_stop(ctx,0x8c6138c6u); fcopy(f,2u,3u);goto L8c6138c8; /* fmov fr3,fr2 */
L8c6138c8: sa_motion_publish_stop(ctx,0x8c6138c8u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6138ca; /* fmov.s @r4+,fr4 */
L8c6138ca: sa_motion_publish_stop(ctx,0x8c6138cau); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6138cc; /* fmov.s @r4+,fr6 */
L8c6138cc: sa_motion_publish_stop(ctx,0x8c6138ccu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6138ce; /* fldi0 fr3 */
L8c6138ce: sa_motion_publish_stop(ctx,0x8c6138ceu); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c6138d0; /* fipr fv4,fv0 */
L8c6138d0: sa_motion_publish_stop(ctx,0x8c6138d0u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6138d2; /* fmov.s @r4+,fr8 */
L8c6138d2: sa_motion_publish_stop(ctx,0x8c6138d2u); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6138d4; /* fmov.s @r4+,fr10 */
L8c6138d4: sa_motion_publish_stop(ctx,0x8c6138d4u); fcopy(f,0u,9u);goto L8c6138d6; /* fmov fr9,fr0 */
L8c6138d6: sa_motion_publish_stop(ctx,0x8c6138d6u); fcopy(f,4u,5u);goto L8c6138d8; /* fmov fr5,fr4 */
L8c6138d8: sa_motion_publish_stop(ctx,0x8c6138d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6138da; /* fsub fr3,fr0 */
L8c6138da: sa_motion_publish_stop(ctx,0x8c6138dau); fcopy(f,6u,7u);goto L8c6138dc; /* fmov fr7,fr6 */
L8c6138dc: sa_motion_publish_stop(ctx,0x8c6138dcu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6138de; /* fschg */
L8c6138de: sa_motion_publish_stop(ctx,0x8c6138deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c6138e0; /* fmov.s fr10,@-r8 */
L8c6138e0: sa_motion_publish_stop(ctx,0x8c6138e0u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6138e2; /* fmov.s fr0,@-r4 */
L8c6138e2: sa_motion_publish_stop(ctx,0x8c6138e2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c6138e4; /* ocbwb @r4 */
L8c6138e4: sa_motion_publish_stop(ctx,0x8c6138e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6138e6; /* fmul fr0,fr7 */
L8c6138e6: sa_motion_publish_stop(ctx,0x8c6138e6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c6138e8; /* fmov.s fr9,@-r8 */
L8c6138e8: sa_motion_publish_stop(ctx,0x8c6138e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6138ea; /* fmul fr0,fr6 */
L8c6138ea: sa_motion_publish_stop(ctx,0x8c6138eau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c6138ec; /* fmov.s fr8,@-r8 */
L8c6138ec: sa_motion_publish_stop(ctx,0x8c6138ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6138ee; /* fmul fr0,fr5 */
L8c6138ee: sa_motion_publish_stop(ctx,0x8c6138eeu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6138f0; /* mov.l r5,@-r8 */
L8c6138f0: sa_motion_publish_stop(ctx,0x8c6138f0u); r[8]+=32u;goto L8c6138f2; /* add #0x20,r8 */
L8c6138f2: sa_motion_publish_stop(ctx,0x8c6138f2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c6138f4; /* fmov.s fr7,@-r8 */
L8c6138f4: sa_motion_publish_stop(ctx,0x8c6138f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6138f6; /* fmov.s fr6,@-r8 */
L8c6138f6: sa_motion_publish_stop(ctx,0x8c6138f6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6138f8; /* fmov.s fr5,@-r8 */
L8c6138f8: sa_motion_publish_stop(ctx,0x8c6138f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6138fa; /* fmov.s fr4,@-r8 */
L8c6138fa: sa_motion_publish_stop(ctx,0x8c6138fau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6138fc; /* pref @r8 */
L8c6138fc: sa_motion_publish_stop(ctx,0x8c6138fcu); r[8]+=16u;goto L8c6138fe; /* add #0x10,r8 */
L8c6138fe: sa_motion_publish_stop(ctx,0x8c6138feu); r[3]=r[8];goto L8c613900; /* mov r8,r3 */
L8c613900: sa_motion_publish_stop(ctx,0x8c613900u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613902; /* mov.l @(0x8,r15),r2 */
L8c613902: sa_motion_publish_stop(ctx,0x8c613902u); r[3]-=r[10];goto L8c613904; /* sub r10,r3 */
L8c613904: sa_motion_publish_stop(ctx,0x8c613904u); r[3]+=32u;goto L8c613906; /* add #0x20,r3 */
L8c613906: sa_motion_publish_stop(ctx,0x8c613906u); r[2]+=r[3];goto L8c613908; /* add r3,r2 */
L8c613908: sa_motion_publish_stop(ctx,0x8c613908u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c61390a; /* mov.l r2,@(0x8,r15) */
L8c61390a: sa_motion_publish_stop(ctx,0x8c61390au); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61390c; /* mov.l @(0xc,r15),r3 */
L8c61390c: sa_motion_publish_stop(ctx,0x8c61390cu); r[3]+=4294967295u;goto L8c61390e; /* add #-0x1,r3 */
L8c61390e: sa_motion_publish_stop(ctx,0x8c61390eu); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613910; /* tst r3,r3 */
L8c613910: sa_motion_publish_stop(ctx,0x8c613912u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c613918;goto L8c613914; /* bt/s 0x8c613918 */
L8c613914: sa_motion_publish_stop(ctx,0x8c613916u); goto L8c613720; /* bra 0x8c613720 */
L8c613918: sa_motion_publish_stop(ctx,0x8c613918u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61391a; /* mov.l @(0x8,r15),r3 */
L8c61391a: sa_motion_publish_stop(ctx,0x8c61391au); r[4]=rd(m,bus,&io_status,0x8c613988u,4);if(io_status)return io_status;goto L8c61391c; /* mov.l 0x8c613988,r4 */
L8c61391c: sa_motion_publish_stop(ctx,0x8c61391cu); r[3]>>=2;goto L8c61391e; /* shlr2 r3 */
L8c61391e: sa_motion_publish_stop(ctx,0x8c61391eu); r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c613920; /* mov.l @r4,r1 */
L8c613920: sa_motion_publish_stop(ctx,0x8c613920u); r[3]>>=2;goto L8c613922; /* shlr2 r3 */
L8c613922: sa_motion_publish_stop(ctx,0x8c613922u); *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c613924; /* shlr r3 */
L8c613924: sa_motion_publish_stop(ctx,0x8c613924u); r[1]+=r[3];goto L8c613926; /* add r3,r1 */
L8c613926: sa_motion_publish_stop(ctx,0x8c613926u); wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c613928; /* mov.l r1,@r4 */
L8c613928: sa_motion_publish_stop(ctx,0x8c613928u); r[4]=rd(m,bus,&io_status,0x8c61398cu,4);if(io_status)return io_status;goto L8c61392a; /* mov.l 0x8c61398c,r4 */
L8c61392a: sa_motion_publish_stop(ctx,0x8c61392au); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61392c; /* mov.l @(0x4,r15),r3 */
L8c61392c: sa_motion_publish_stop(ctx,0x8c61392cu); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61392e; /* mov.l @r4,r2 */
L8c61392e: sa_motion_publish_stop(ctx,0x8c61392eu); r[2]+=r[3];goto L8c613930; /* add r3,r2 */
L8c613930: sa_motion_publish_stop(ctx,0x8c613930u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c613932; /* mov.l r2,@r4 */
L8c613932: sa_motion_publish_stop(ctx,0x8c613932u); r[15]+=16u;goto L8c613934; /* add #0x10,r15 */
L8c613934: sa_motion_publish_stop(ctx,0x8c613934u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613936; /* lds.l @r15+,PR */
L8c613936: sa_motion_publish_stop(ctx,0x8c613936u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613938; /* fmov.s @r15+,fr13 */
L8c613938: sa_motion_publish_stop(ctx,0x8c613938u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61393a; /* fmov.s @r15+,fr14 */
L8c61393a: sa_motion_publish_stop(ctx,0x8c61393au); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61393c; /* fmov.s @r15+,fr15 */
L8c61393c: sa_motion_publish_stop(ctx,0x8c61393cu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61393e; /* mov.l @r15+,r9 */
L8c61393e: sa_motion_publish_stop(ctx,0x8c61393eu); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613940; /* mov.l @r15+,r10 */
L8c613940: sa_motion_publish_stop(ctx,0x8c613940u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613942; /* mov.l @r15+,r11 */
L8c613942: sa_motion_publish_stop(ctx,0x8c613942u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613944; /* mov.l @r15+,r12 */
L8c613944: sa_motion_publish_stop(ctx,0x8c613944u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613946; /* mov.l @r15+,r13 */
L8c613946: sa_motion_publish_stop(ctx,0x8c613948u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_lit_alternate_strip_clipped_8c614bb0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c614bb0;
L8c614bb0: sa_motion_publish_stop(ctx,0x8c614bb0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614bb2; /* mov.l r14,@-r15 */
L8c614bb2: sa_motion_publish_stop(ctx,0x8c614bb2u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c614bb4; /* mov.l r13,@-r15 */
L8c614bb4: sa_motion_publish_stop(ctx,0x8c614bb4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c614bb6; /* mov.l r12,@-r15 */
L8c614bb6: sa_motion_publish_stop(ctx,0x8c614bb6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c614bb8; /* mov.l r11,@-r15 */
L8c614bb8: sa_motion_publish_stop(ctx,0x8c614bb8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c614bba; /* mov.l r10,@-r15 */
L8c614bba: sa_motion_publish_stop(ctx,0x8c614bbau); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c614bbc; /* mov.l r9,@-r15 */
L8c614bbc: sa_motion_publish_stop(ctx,0x8c614bbcu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c614bbe; /* fmov.s fr15,@-r15 */
L8c614bbe: sa_motion_publish_stop(ctx,0x8c614bbeu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c614bc0; /* fmov.s fr14,@-r15 */
L8c614bc0: sa_motion_publish_stop(ctx,0x8c614bc0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c614bc2; /* fmov.s fr13,@-r15 */
L8c614bc2: sa_motion_publish_stop(ctx,0x8c614bc2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c614bc4; /* fmov.s fr12,@-r15 */
L8c614bc4: sa_motion_publish_stop(ctx,0x8c614bc4u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c614bc6; /* sts.l PR,@-r15 */
L8c614bc6: sa_motion_publish_stop(ctx,0x8c614bc6u); r[15]+=4294967220u;goto L8c614bc8; /* add #-0x4c,r15 */
L8c614bc8: sa_motion_publish_stop(ctx,0x8c614bc8u); r[3]=rd(m,bus,&io_status,0x8c614bf4u,4);if(io_status)return io_status;goto L8c614bca; /* mov.l 0x8c614bf4,r3 */
L8c614bca: sa_motion_publish_stop(ctx,0x8c614bcau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c614bcc; /* fldi1 fr12 */
L8c614bcc: sa_motion_publish_stop(ctx,0x8c614bceu); target=r[3];*pr=0x8c614bd0u;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c614bd0; /* jsr @r3 */
L8c614bd0: sa_motion_publish_stop(ctx,0x8c614bd0u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614bd2; /* mov.l @(0xc,r15),r3 */
L8c614bd2: sa_motion_publish_stop(ctx,0x8c614bd2u); r[2]=0u;goto L8c614bd4; /* mov #0x0,r2 */
L8c614bd4: sa_motion_publish_stop(ctx,0x8c614bd4u); r[14]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614bd6; /* mov.l @(0xc,r15),r14 */
L8c614bd6: sa_motion_publish_stop(ctx,0x8c614bd6u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c614bd8; /* mov.w @(0x2,r3),r0 */
L8c614bd8: sa_motion_publish_stop(ctx,0x8c614bd8u); r[3]=rd(m,bus,&io_status,0x8c614bf8u,4);if(io_status)return io_status;goto L8c614bda; /* mov.l 0x8c614bf8,r3 */
L8c614bda: sa_motion_publish_stop(ctx,0x8c614bdau); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c614bdc; /* mov.l @(0x4,r14),r14 */
L8c614bdc: sa_motion_publish_stop(ctx,0x8c614bdcu); r[0]=r[0]&65535u;goto L8c614bde; /* extu.w r0,r0 */
L8c614bde: sa_motion_publish_stop(ctx,0x8c614bdeu); wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c614be0; /* mov.l r0,@(0x18,r15) */
L8c614be0: sa_motion_publish_stop(ctx,0x8c614be0u); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c614be2; /* mov.l @r3,r13 */
L8c614be2: sa_motion_publish_stop(ctx,0x8c614be2u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c614be4; /* mov.l r2,@(0xc,r15) */
L8c614be4: sa_motion_publish_stop(ctx,0x8c614be6u); goto L8c614bfc; /* bra 0x8c614bfc */
L8c614bfc: sa_motion_publish_stop(ctx,0x8c614bfcu); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614bfe; /* mov.w @r14+,r3 */
L8c614bfe: sa_motion_publish_stop(ctx,0x8c614bfeu); r[4]=rd(m,bus,&io_status,0x8c614d18u,4);if(io_status)return io_status;goto L8c614c00; /* mov.l 0x8c614d18,r4 */
L8c614c00: sa_motion_publish_stop(ctx,0x8c614c00u); r[3]=r[3]&65535u;goto L8c614c02; /* extu.w r3,r3 */
L8c614c02: sa_motion_publish_stop(ctx,0x8c614c02u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c614c04; /* tst r4,r3 */
L8c614c04: sa_motion_publish_stop(ctx,0x8c614c04u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c614c06; /* mov.l r3,@(0x8,r15) */
L8c614c06: sa_motion_publish_stop(ctx,0x8c614c08u); take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c614c18;goto L8c614c0a; /* bt/s 0x8c614c18 */
L8c614c0a: sa_motion_publish_stop(ctx,0x8c614c0au); r[4]=sx(m,bus,&io_status,0x8c614d14u,2);if(io_status)return io_status;goto L8c614c0c; /* mov.w 0x8c614d14,r4 */
L8c614c0c: sa_motion_publish_stop(ctx,0x8c614c0cu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614c0e; /* mov.l @(0x8,r15),r3 */
L8c614c0e: sa_motion_publish_stop(ctx,0x8c614c0eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614c10; /* fldi1 fr4 */
L8c614c10: sa_motion_publish_stop(ctx,0x8c614c10u); r[3]&=r[4];goto L8c614c12; /* and r4,r3 */
L8c614c12: sa_motion_publish_stop(ctx,0x8c614c12u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c614c14; /* fneg fr4 */
L8c614c14: sa_motion_publish_stop(ctx,0x8c614c14u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c614c16; /* mov.l r3,@(0x8,r15) */
L8c614c16: sa_motion_publish_stop(ctx,0x8c614c16u); fcopy(f,15u,4u);goto L8c614c18; /* fmov fr4,fr15 */
L8c614c18: sa_motion_publish_stop(ctx,0x8c614c18u); r[0]=16u;goto L8c614c1a; /* mov #0x10,r0 */
L8c614c1a: sa_motion_publish_stop(ctx,0x8c614c1au); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614c1c; /* mov.l @(0x8,r15),r3 */
L8c614c1c: sa_motion_publish_stop(ctx,0x8c614c1cu); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614c1e; /* mov.l @(0xc,r15),r2 */
L8c614c1e: sa_motion_publish_stop(ctx,0x8c614c1eu); r[9]=r[8];goto L8c614c20; /* mov r8,r9 */
L8c614c20: sa_motion_publish_stop(ctx,0x8c614c20u); r[2]+=r[3];goto L8c614c22; /* add r3,r2 */
L8c614c22: sa_motion_publish_stop(ctx,0x8c614c22u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c614c24; /* mov.l r2,@(0xc,r15) */
L8c614c24: sa_motion_publish_stop(ctx,0x8c614c24u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614c26; /* mov.l @(0x8,r15),r1 */
L8c614c26: sa_motion_publish_stop(ctx,0x8c614c26u); r[1]+=4294967294u;goto L8c614c28; /* add #-0x2,r1 */
L8c614c28: sa_motion_publish_stop(ctx,0x8c614c28u); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c614c2a; /* mov.l r1,@(0x8,r15) */
L8c614c2a: sa_motion_publish_stop(ctx,0x8c614c2au); r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614c2c; /* mov.w @r14+,r10 */
L8c614c2c: sa_motion_publish_stop(ctx,0x8c614c2cu); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614c2e; /* mov.w @r14+,r11 */
L8c614c2e: sa_motion_publish_stop(ctx,0x8c614c2eu); r[10]=r[10]&65535u;goto L8c614c30; /* extu.w r10,r10 */
L8c614c30: sa_motion_publish_stop(ctx,0x8c614c30u); r[10]<<=2;goto L8c614c32; /* shll2 r10 */
L8c614c32: sa_motion_publish_stop(ctx,0x8c614c32u); r[11]=r[11]&65535u;goto L8c614c34; /* extu.w r11,r11 */
L8c614c34: sa_motion_publish_stop(ctx,0x8c614c34u); r[11]<<=2;goto L8c614c36; /* shll2 r11 */
L8c614c36: sa_motion_publish_stop(ctx,0x8c614c36u); r[10]<<=2;goto L8c614c38; /* shll2 r10 */
L8c614c38: sa_motion_publish_stop(ctx,0x8c614c38u); r[11]<<=2;goto L8c614c3a; /* shll2 r11 */
L8c614c3a: sa_motion_publish_stop(ctx,0x8c614c3au); *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c614c3c; /* shll r10 */
L8c614c3c: sa_motion_publish_stop(ctx,0x8c614c3cu); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c614c3e; /* shll r11 */
L8c614c3e: sa_motion_publish_stop(ctx,0x8c614c3eu); r[11]+=r[13];goto L8c614c40; /* add r13,r11 */
L8c614c40: sa_motion_publish_stop(ctx,0x8c614c40u); r[10]+=r[13];goto L8c614c42; /* add r13,r10 */
L8c614c42: sa_motion_publish_stop(ctx,0x8c614c42u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c614c44; /* fmov.s @(r0,r11),fr13 */
L8c614c44: sa_motion_publish_stop(ctx,0x8c614c44u); fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c614c46; /* fmov.s @(r0,r10),fr6 */
L8c614c46: sa_motion_publish_stop(ctx,0x8c614c46u); r[0]=20u;goto L8c614c48; /* mov #0x14,r0 */
L8c614c48: sa_motion_publish_stop(ctx,0x8c614c48u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c614c4a; /* fmov.s @(r0,r11),fr14 */
L8c614c4a: sa_motion_publish_stop(ctx,0x8c614c4au); fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c614c4c; /* fmov.s @(r0,r10),fr5 */
L8c614c4c: sa_motion_publish_stop(ctx,0x8c614c4cu); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614c4e; /* mov.w @r14+,r12 */
L8c614c4e: sa_motion_publish_stop(ctx,0x8c614c4eu); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c614c50; /* mov.w @r14,r3 */
L8c614c50: sa_motion_publish_stop(ctx,0x8c614c50u); r[12]=r[12]&65535u;goto L8c614c52; /* extu.w r12,r12 */
L8c614c52: sa_motion_publish_stop(ctx,0x8c614c52u); r[12]<<=2;goto L8c614c54; /* shll2 r12 */
L8c614c54: sa_motion_publish_stop(ctx,0x8c614c54u); r[3]=r[3]&65535u;goto L8c614c56; /* extu.w r3,r3 */
L8c614c56: sa_motion_publish_stop(ctx,0x8c614c56u); r[3]<<=2;goto L8c614c58; /* shll2 r3 */
L8c614c58: sa_motion_publish_stop(ctx,0x8c614c58u); r[12]<<=2;goto L8c614c5a; /* shll2 r12 */
L8c614c5a: sa_motion_publish_stop(ctx,0x8c614c5au); r[3]<<=2;goto L8c614c5c; /* shll2 r3 */
L8c614c5c: sa_motion_publish_stop(ctx,0x8c614c5cu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c614c5e; /* shll r12 */
L8c614c5e: sa_motion_publish_stop(ctx,0x8c614c5eu); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c614c60; /* shll r3 */
L8c614c60: sa_motion_publish_stop(ctx,0x8c614c60u); r[3]+=r[13];goto L8c614c62; /* add r13,r3 */
L8c614c62: sa_motion_publish_stop(ctx,0x8c614c62u); r[12]+=r[13];goto L8c614c64; /* add r13,r12 */
L8c614c64: sa_motion_publish_stop(ctx,0x8c614c64u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c614c66; /* pref @r3 */
L8c614c66: sa_motion_publish_stop(ctx,0x8c614c66u); r[2]=0u;goto L8c614c68; /* mov #0x0,r2 */
L8c614c68: sa_motion_publish_stop(ctx,0x8c614c68u); wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c614c6a; /* mov.l r2,@(0x14,r15) */
L8c614c6a: sa_motion_publish_stop(ctx,0x8c614c6au); r[4]=63u;goto L8c614c6c; /* mov #0x3f,r4 */
L8c614c6c: sa_motion_publish_stop(ctx,0x8c614c6cu); r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c614c6e; /* mov.l @(0xc,r10),r2 */
L8c614c6e: sa_motion_publish_stop(ctx,0x8c614c6eu); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c614c70; /* mov.l @(0xc,r11),r3 */
L8c614c70: sa_motion_publish_stop(ctx,0x8c614c70u); r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c614c72; /* mov.l @(0xc,r12),r1 */
L8c614c72: sa_motion_publish_stop(ctx,0x8c614c72u); r[2]&=r[3];goto L8c614c74; /* and r3,r2 */
L8c614c74: sa_motion_publish_stop(ctx,0x8c614c74u); r[2]&=r[1];goto L8c614c76; /* and r1,r2 */
L8c614c76: sa_motion_publish_stop(ctx,0x8c614c76u); r[4]&=r[2];goto L8c614c78; /* and r2,r4 */
L8c614c78: sa_motion_publish_stop(ctx,0x8c614c78u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c614c7a; /* tst r4,r4 */
L8c614c7a: sa_motion_publish_stop(ctx,0x8c614c7cu); take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c614c82;goto L8c614c7e; /* bt/s 0x8c614c82 */
L8c614c7e: sa_motion_publish_stop(ctx,0x8c614c80u); goto L8c614ddc; /* bra 0x8c614ddc */
L8c614c82: sa_motion_publish_stop(ctx,0x8c614c82u); r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c614c84; /* mov.l @(0xc,r10),r4 */
L8c614c84: sa_motion_publish_stop(ctx,0x8c614c84u); r[5]=1u;goto L8c614c86; /* mov #0x1,r5 */
L8c614c86: sa_motion_publish_stop(ctx,0x8c614c86u); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c614c88; /* mov.l @(0xc,r11),r3 */
L8c614c88: sa_motion_publish_stop(ctx,0x8c614c88u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c614c8a; /* mov.l @(0xc,r12),r2 */
L8c614c8a: sa_motion_publish_stop(ctx,0x8c614c8au); r[4]|=r[3];goto L8c614c8c; /* or r3,r4 */
L8c614c8c: sa_motion_publish_stop(ctx,0x8c614c8cu); r[4]|=r[2];goto L8c614c8e; /* or r2,r4 */
L8c614c8e: sa_motion_publish_stop(ctx,0x8c614c8eu); r[4]&=r[5];goto L8c614c90; /* and r5,r4 */
L8c614c90: sa_motion_publish_stop(ctx,0x8c614c90u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c614c92; /* tst r4,r4 */
L8c614c92: sa_motion_publish_stop(ctx,0x8c614c92u); if(!(*sr&1u))goto L8c614c98;goto L8c614c94; /* bf 0x8c614c98 */
L8c614c94: sa_motion_publish_stop(ctx,0x8c614c96u); goto L8c614dbe; /* bra 0x8c614dbe */
L8c614c98: sa_motion_publish_stop(ctx,0x8c614c98u); r[7]=r[15];goto L8c614c9a; /* mov r15,r7 */
L8c614c9a: sa_motion_publish_stop(ctx,0x8c614c9au); r[7]+=64u;goto L8c614c9c; /* add #0x40,r7 */
L8c614c9c: sa_motion_publish_stop(ctx,0x8c614c9cu); wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c614c9e; /* mov.l r10,@r7 */
L8c614c9e: sa_motion_publish_stop(ctx,0x8c614c9eu); wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c614ca0; /* mov.l r11,@(0x4,r7) */
L8c614ca0: sa_motion_publish_stop(ctx,0x8c614ca0u); wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c614ca2; /* mov.l r12,@(0x8,r7) */
L8c614ca2: sa_motion_publish_stop(ctx,0x8c614ca4u); r[6]=0u;goto L8c614d50; /* bra 0x8c614d50 */
L8c614ca6: sa_motion_publish_stop(ctx,0x8c614ca6u); r[2]=r[6];goto L8c614ca8; /* mov r6,r2 */
L8c614ca8: sa_motion_publish_stop(ctx,0x8c614ca8u); r[1]=r[6];goto L8c614caa; /* mov r6,r1 */
L8c614caa: sa_motion_publish_stop(ctx,0x8c614caau); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c614cac; /* shll r2 */
L8c614cac: sa_motion_publish_stop(ctx,0x8c614cacu); r[3]=r[6];goto L8c614cae; /* mov r6,r3 */
L8c614cae: sa_motion_publish_stop(ctx,0x8c614caeu); r[3]<<=2;goto L8c614cb0; /* shll2 r3 */
L8c614cb0: sa_motion_publish_stop(ctx,0x8c614cb0u); r[2]+=r[1];goto L8c614cb2; /* add r1,r2 */
L8c614cb2: sa_motion_publish_stop(ctx,0x8c614cb2u); r[2]<<=2;goto L8c614cb4; /* shll2 r2 */
L8c614cb4: sa_motion_publish_stop(ctx,0x8c614cb4u); r[3]+=r[7];goto L8c614cb6; /* add r7,r3 */
L8c614cb6: sa_motion_publish_stop(ctx,0x8c614cb6u); r[2]+=r[4];goto L8c614cb8; /* add r4,r2 */
L8c614cb8: sa_motion_publish_stop(ctx,0x8c614cb8u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c614cba; /* mov.l r3,@(0x4,r15) */
L8c614cba: sa_motion_publish_stop(ctx,0x8c614cbau); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c614cbc; /* mov.l r2,@r15 */
L8c614cbc: sa_motion_publish_stop(ctx,0x8c614cbcu); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c614cbe; /* mov.l @r3,r1 */
L8c614cbe: sa_motion_publish_stop(ctx,0x8c614cbeu); r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c614cc0; /* mov.l @(0xc,r1),r2 */
L8c614cc0: sa_motion_publish_stop(ctx,0x8c614cc0u); *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c614cc2; /* tst r5,r2 */
L8c614cc2: sa_motion_publish_stop(ctx,0x8c614cc4u); take=!(*sr&1u);r[6]+=1u;if(take)goto L8c614d28;goto L8c614cc6; /* bf/s 0x8c614d28 */
L8c614cc6: sa_motion_publish_stop(ctx,0x8c614cc6u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614cc8; /* mov.l @(0x4,r15),r1 */
L8c614cc8: sa_motion_publish_stop(ctx,0x8c614cc8u); r[0]=24u;goto L8c614cca; /* mov #0x18,r0 */
L8c614cca: sa_motion_publish_stop(ctx,0x8c614ccau); fcopy(f,4u,12u);goto L8c614ccc; /* fmov fr12,fr4 */
L8c614ccc: sa_motion_publish_stop(ctx,0x8c614cccu); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c614cce; /* mov.l @r1,r3 */
L8c614cce: sa_motion_publish_stop(ctx,0x8c614cceu); r[4]=rd(m,bus,&io_status,0x8c614d1cu,4);if(io_status)return io_status;goto L8c614cd0; /* mov.l 0x8c614d1c,r4 */
L8c614cd0: sa_motion_publish_stop(ctx,0x8c614cd0u); wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c614cd2; /* mov.l r3,@(0x10,r15) */
L8c614cd2: sa_motion_publish_stop(ctx,0x8c614cd2u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c614cd4; /* fmov.s @(r0,r3),fr3 */
L8c614cd4: sa_motion_publish_stop(ctx,0x8c614cd4u); r[0]=12u;goto L8c614cd6; /* mov #0xc,r0 */
L8c614cd6: sa_motion_publish_stop(ctx,0x8c614cd6u); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c614cd8; /* mov.l @(0x10,r15),r2 */
L8c614cd8: sa_motion_publish_stop(ctx,0x8c614cd8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c614cda; /* fdiv fr3,fr4 */
L8c614cda: sa_motion_publish_stop(ctx,0x8c614cdau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c614cdc; /* fmov.s @(r0,r4),fr3 */
L8c614cdc: sa_motion_publish_stop(ctx,0x8c614cdcu); r[0]=16u;goto L8c614cde; /* mov #0x10,r0 */
L8c614cde: sa_motion_publish_stop(ctx,0x8c614cdeu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614ce0; /* mov.l @r15,r3 */
L8c614ce0: sa_motion_publish_stop(ctx,0x8c614ce0u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c614ce2; /* fmov.s @(r0,r2),fr2 */
L8c614ce2: sa_motion_publish_stop(ctx,0x8c614ce2u); r[0]=16u;goto L8c614ce4; /* mov #0x10,r0 */
L8c614ce4: sa_motion_publish_stop(ctx,0x8c614ce4u); r[2]=rd(m,bus,&io_status,0x8c614d20u,4);if(io_status)return io_status;goto L8c614ce6; /* mov.l 0x8c614d20,r2 */
L8c614ce6: sa_motion_publish_stop(ctx,0x8c614ce6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614ce8; /* fsub fr3,fr2 */
L8c614ce8: sa_motion_publish_stop(ctx,0x8c614ce8u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c614cea; /* fmov.s @r2,fr3 */
L8c614cea: sa_motion_publish_stop(ctx,0x8c614ceau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614cec; /* fmul fr4,fr2 */
L8c614cec: sa_motion_publish_stop(ctx,0x8c614cecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614cee; /* fdiv fr3,fr2 */
L8c614cee: sa_motion_publish_stop(ctx,0x8c614ceeu); fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c614cf0; /* fmov.s fr2,@r3 */
L8c614cf0: sa_motion_publish_stop(ctx,0x8c614cf0u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614cf2; /* mov.l @(0x4,r15),r1 */
L8c614cf2: sa_motion_publish_stop(ctx,0x8c614cf2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c614cf4; /* fmov.s @(r0,r4),fr3 */
L8c614cf4: sa_motion_publish_stop(ctx,0x8c614cf4u); r[0]=20u;goto L8c614cf6; /* mov #0x14,r0 */
L8c614cf6: sa_motion_publish_stop(ctx,0x8c614cf6u); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c614cf8; /* mov.l @r1,r1 */
L8c614cf8: sa_motion_publish_stop(ctx,0x8c614cf8u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614cfa; /* mov.l @r15,r3 */
L8c614cfa: sa_motion_publish_stop(ctx,0x8c614cfau); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c614cfc; /* fmov.s @(r0,r1),fr2 */
L8c614cfc: sa_motion_publish_stop(ctx,0x8c614cfcu); r[1]=rd(m,bus,&io_status,0x8c614d24u,4);if(io_status)return io_status;goto L8c614cfe; /* mov.l 0x8c614d24,r1 */
L8c614cfe: sa_motion_publish_stop(ctx,0x8c614cfeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614d00; /* fsub fr3,fr2 */
L8c614d00: sa_motion_publish_stop(ctx,0x8c614d00u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c614d02; /* fmov.s @r1,fr3 */
L8c614d02: sa_motion_publish_stop(ctx,0x8c614d02u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614d04; /* fmul fr4,fr2 */
L8c614d04: sa_motion_publish_stop(ctx,0x8c614d04u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614d06; /* fdiv fr3,fr2 */
L8c614d06: sa_motion_publish_stop(ctx,0x8c614d06u); r[0]=4u;goto L8c614d08; /* mov #0x4,r0 */
L8c614d08: sa_motion_publish_stop(ctx,0x8c614d08u); fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d0a; /* fmov fr2,@(r0,r3) */
L8c614d0a: sa_motion_publish_stop(ctx,0x8c614d0au); r[0]=8u;goto L8c614d0c; /* mov #0x8,r0 */
L8c614d0c: sa_motion_publish_stop(ctx,0x8c614d0cu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614d0e; /* mov.l @r15,r3 */
L8c614d0e: sa_motion_publish_stop(ctx,0x8c614d0eu); fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d10; /* fmov fr4,@(r0,r3) */
L8c614d10: sa_motion_publish_stop(ctx,0x8c614d12u); goto L8c614d50; /* bra 0x8c614d50 */
L8c614d28: sa_motion_publish_stop(ctx,0x8c614d28u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614d2a; /* mov.l @(0x4,r15),r3 */
L8c614d2a: sa_motion_publish_stop(ctx,0x8c614d2au); r[0]=16u;goto L8c614d2c; /* mov #0x10,r0 */
L8c614d2c: sa_motion_publish_stop(ctx,0x8c614d2cu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614d2e; /* mov.l @r15,r2 */
L8c614d2e: sa_motion_publish_stop(ctx,0x8c614d2eu); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c614d30; /* mov.l @r3,r1 */
L8c614d30: sa_motion_publish_stop(ctx,0x8c614d30u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c614d32; /* fmov.s @(r0,r1),fr3 */
L8c614d32: sa_motion_publish_stop(ctx,0x8c614d32u); r[0]=20u;goto L8c614d34; /* mov #0x14,r0 */
L8c614d34: sa_motion_publish_stop(ctx,0x8c614d34u); fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c614d36; /* fmov.s fr3,@r2 */
L8c614d36: sa_motion_publish_stop(ctx,0x8c614d36u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614d38; /* mov.l @(0x4,r15),r2 */
L8c614d38: sa_motion_publish_stop(ctx,0x8c614d38u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614d3a; /* mov.l @r15,r3 */
L8c614d3a: sa_motion_publish_stop(ctx,0x8c614d3au); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c614d3c; /* mov.l @r2,r1 */
L8c614d3c: sa_motion_publish_stop(ctx,0x8c614d3cu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c614d3e; /* fmov.s @(r0,r1),fr3 */
L8c614d3e: sa_motion_publish_stop(ctx,0x8c614d3eu); r[0]=4u;goto L8c614d40; /* mov #0x4,r0 */
L8c614d40: sa_motion_publish_stop(ctx,0x8c614d40u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d42; /* fmov fr3,@(r0,r3) */
L8c614d42: sa_motion_publish_stop(ctx,0x8c614d42u); r[0]=24u;goto L8c614d44; /* mov #0x18,r0 */
L8c614d44: sa_motion_publish_stop(ctx,0x8c614d44u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614d46; /* mov.l @(0x4,r15),r2 */
L8c614d46: sa_motion_publish_stop(ctx,0x8c614d46u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614d48; /* mov.l @r15,r3 */
L8c614d48: sa_motion_publish_stop(ctx,0x8c614d48u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c614d4a; /* mov.l @r2,r1 */
L8c614d4a: sa_motion_publish_stop(ctx,0x8c614d4au); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c614d4c; /* fmov.s @(r0,r1),fr3 */
L8c614d4c: sa_motion_publish_stop(ctx,0x8c614d4cu); r[0]=8u;goto L8c614d4e; /* mov #0x8,r0 */
L8c614d4e: sa_motion_publish_stop(ctx,0x8c614d4eu); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d50; /* fmov fr3,@(r0,r3) */
L8c614d50: sa_motion_publish_stop(ctx,0x8c614d50u); r[3]=3u;goto L8c614d52; /* mov #0x3,r3 */
L8c614d52: sa_motion_publish_stop(ctx,0x8c614d52u); r[4]=r[15];goto L8c614d54; /* mov r15,r4 */
L8c614d54: sa_motion_publish_stop(ctx,0x8c614d54u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c614d56; /* cmp/ge r3,r6 */
L8c614d56: sa_motion_publish_stop(ctx,0x8c614d58u); take=!(*sr&1u);r[4]+=28u;if(take)goto L8c614ca6;goto L8c614d5a; /* bf/s 0x8c614ca6 */
L8c614d5a: sa_motion_publish_stop(ctx,0x8c614d5au); r[6]=r[4];goto L8c614d5c; /* mov r4,r6 */
L8c614d5c: sa_motion_publish_stop(ctx,0x8c614d5cu); r[0]=8u;goto L8c614d5e; /* mov #0x8,r0 */
L8c614d5e: sa_motion_publish_stop(ctx,0x8c614d5eu); r[3]=r[4];goto L8c614d60; /* mov r4,r3 */
L8c614d60: sa_motion_publish_stop(ctx,0x8c614d60u); fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c614d62; /* fmov.s @(r0,r6),fr6 */
L8c614d62: sa_motion_publish_stop(ctx,0x8c614d62u); r[3]+=12u;goto L8c614d64; /* add #0xc,r3 */
L8c614d64: sa_motion_publish_stop(ctx,0x8c614d64u); r[7]=r[4];goto L8c614d66; /* mov r4,r7 */
L8c614d66: sa_motion_publish_stop(ctx,0x8c614d66u); r[0]=4u;goto L8c614d68; /* mov #0x4,r0 */
L8c614d68: sa_motion_publish_stop(ctx,0x8c614d68u); fcopy(f,0u,6u);goto L8c614d6a; /* fmov fr6,fr0 */
L8c614d6a: sa_motion_publish_stop(ctx,0x8c614d6au); r[7]+=24u;goto L8c614d6c; /* add #0x18,r7 */
L8c614d6c: sa_motion_publish_stop(ctx,0x8c614d6cu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c614d6e; /* mov.l r3,@r15 */
L8c614d6e: sa_motion_publish_stop(ctx,0x8c614d6eu); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c614d70; /* fmov.s @(r0,r6),fr4 */
L8c614d70: sa_motion_publish_stop(ctx,0x8c614d70u); fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d72; /* fmov.s @(r0,r3),fr5 */
L8c614d72: sa_motion_publish_stop(ctx,0x8c614d72u); r[0]=8u;goto L8c614d74; /* mov #0x8,r0 */
L8c614d74: sa_motion_publish_stop(ctx,0x8c614d74u); fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d76; /* fmov.s @(r0,r3),fr9 */
L8c614d76: sa_motion_publish_stop(ctx,0x8c614d76u); r[0]=4u;goto L8c614d78; /* mov #0x4,r0 */
L8c614d78: sa_motion_publish_stop(ctx,0x8c614d78u); fcopy(f,3u,4u);goto L8c614d7a; /* fmov fr4,fr3 */
L8c614d7a: sa_motion_publish_stop(ctx,0x8c614d7au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c614d7c; /* fmul fr5,fr0 */
L8c614d7c: sa_motion_publish_stop(ctx,0x8c614d7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614d7e; /* fmul fr9,fr3 */
L8c614d7e: sa_motion_publish_stop(ctx,0x8c614d7eu); fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c614d80; /* fmov.s @r6,fr8 */
L8c614d80: sa_motion_publish_stop(ctx,0x8c614d80u); fload(m,bus,&io_status,f,7u,r[3]);if(io_status)return io_status;goto L8c614d82; /* fmov.s @r3,fr7 */
L8c614d82: sa_motion_publish_stop(ctx,0x8c614d82u); fcopy(f,2u,8u);goto L8c614d84; /* fmov fr8,fr2 */
L8c614d84: sa_motion_publish_stop(ctx,0x8c614d84u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614d86; /* fmul fr9,fr2 */
L8c614d86: sa_motion_publish_stop(ctx,0x8c614d86u); fcopy(f,1u,4u);goto L8c614d88; /* fmov fr4,fr1 */
L8c614d88: sa_motion_publish_stop(ctx,0x8c614d88u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614d8a; /* fsub fr3,fr0 */
L8c614d8a: sa_motion_publish_stop(ctx,0x8c614d8au); fcopy(f,3u,6u);goto L8c614d8c; /* fmov fr6,fr3 */
L8c614d8c: sa_motion_publish_stop(ctx,0x8c614d8cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614d8e; /* fmul fr7,fr3 */
L8c614d8e: sa_motion_publish_stop(ctx,0x8c614d8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614d90; /* fsub fr3,fr2 */
L8c614d90: sa_motion_publish_stop(ctx,0x8c614d90u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c614d92; /* fmov.s @(r0,r7),fr3 */
L8c614d92: sa_motion_publish_stop(ctx,0x8c614d92u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614d94; /* fmul fr2,fr3 */
L8c614d94: sa_motion_publish_stop(ctx,0x8c614d94u); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c614d96; /* fmov.s @r7,fr2 */
L8c614d96: sa_motion_publish_stop(ctx,0x8c614d96u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614d98; /* fmac fr0,fr2,fr3 */
L8c614d98: sa_motion_publish_stop(ctx,0x8c614d98u); fcopy(f,2u,3u);goto L8c614d9a; /* fmov fr3,fr2 */
L8c614d9a: sa_motion_publish_stop(ctx,0x8c614d9au); fcopy(f,3u,8u);goto L8c614d9c; /* fmov fr8,fr3 */
L8c614d9c: sa_motion_publish_stop(ctx,0x8c614d9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614d9e; /* fmul fr5,fr3 */
L8c614d9e: sa_motion_publish_stop(ctx,0x8c614d9eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c614da0; /* fmul fr7,fr1 */
L8c614da0: sa_motion_publish_stop(ctx,0x8c614da0u); r[0]=8u;goto L8c614da2; /* mov #0x8,r0 */
L8c614da2: sa_motion_publish_stop(ctx,0x8c614da2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c614da4; /* fsub fr3,fr1 */
L8c614da4: sa_motion_publish_stop(ctx,0x8c614da4u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c614da6; /* fmov.s @(r0,r7),fr3 */
L8c614da6: sa_motion_publish_stop(ctx,0x8c614da6u); fcopy(f,0u,1u);goto L8c614da8; /* fmov fr1,fr0 */
L8c614da8: sa_motion_publish_stop(ctx,0x8c614da8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614daa; /* fmac fr0,fr3,fr2 */
L8c614daa: sa_motion_publish_stop(ctx,0x8c614daau); fcopy(f,3u,15u);goto L8c614dac; /* fmov fr15,fr3 */
L8c614dac: sa_motion_publish_stop(ctx,0x8c614dacu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c614dae; /* fneg fr3 */
L8c614dae: sa_motion_publish_stop(ctx,0x8c614daeu); fcopy(f,4u,2u);goto L8c614db0; /* fmov fr2,fr4 */
L8c614db0: sa_motion_publish_stop(ctx,0x8c614db0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c614db2; /* fmul fr3,fr4 */
L8c614db2: sa_motion_publish_stop(ctx,0x8c614db2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c614db4; /* fldi0 fr2 */
L8c614db4: sa_motion_publish_stop(ctx,0x8c614db4u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c614db6; /* fcmp/gt fr2,fr4 */
L8c614db6: sa_motion_publish_stop(ctx,0x8c614db6u); if((*sr&1u))goto L8c614ddc;goto L8c614db8; /* bt 0x8c614ddc */
L8c614db8: sa_motion_publish_stop(ctx,0x8c614db8u); wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c614dba; /* mov.l r5,@(0x14,r15) */
L8c614dba: sa_motion_publish_stop(ctx,0x8c614dbcu); fcopy(f,4u,12u);goto L8c614ddc; /* bra 0x8c614ddc */
L8c614dbe: sa_motion_publish_stop(ctx,0x8c614dbeu); r[0]=20u;goto L8c614dc0; /* mov #0x14,r0 */
L8c614dc0: sa_motion_publish_stop(ctx,0x8c614dc0u); fcopy(f,3u,13u);goto L8c614dc2; /* fmov fr13,fr3 */
L8c614dc2: sa_motion_publish_stop(ctx,0x8c614dc2u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c614dc4; /* fmov.s @(r0,r12),fr2 */
L8c614dc4: sa_motion_publish_stop(ctx,0x8c614dc4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614dc6; /* fsub fr6,fr3 */
L8c614dc6: sa_motion_publish_stop(ctx,0x8c614dc6u); r[0]=16u;goto L8c614dc8; /* mov #0x10,r0 */
L8c614dc8: sa_motion_publish_stop(ctx,0x8c614dc8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614dca; /* fsub fr14,fr2 */
L8c614dca: sa_motion_publish_stop(ctx,0x8c614dcau); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c614dcc; /* fmov.s @(r0,r12),fr1 */
L8c614dcc: sa_motion_publish_stop(ctx,0x8c614dccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c614dce; /* fsub fr13,fr1 */
L8c614dce: sa_motion_publish_stop(ctx,0x8c614dceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614dd0; /* fmul fr2,fr3 */
L8c614dd0: sa_motion_publish_stop(ctx,0x8c614dd0u); fcopy(f,2u,14u);goto L8c614dd2; /* fmov fr14,fr2 */
L8c614dd2: sa_motion_publish_stop(ctx,0x8c614dd2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614dd4; /* fsub fr5,fr2 */
L8c614dd4: sa_motion_publish_stop(ctx,0x8c614dd4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614dd6; /* fmul fr1,fr2 */
L8c614dd6: sa_motion_publish_stop(ctx,0x8c614dd6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614dd8; /* fsub fr2,fr3 */
L8c614dd8: sa_motion_publish_stop(ctx,0x8c614dd8u); fcopy(f,4u,3u);goto L8c614dda; /* fmov fr3,fr4 */
L8c614dda: sa_motion_publish_stop(ctx,0x8c614ddau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c614ddc; /* fmul fr15,fr4 */
L8c614ddc: sa_motion_publish_stop(ctx,0x8c614ddcu); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c614dde; /* cmp/hs r8,r9 */
L8c614dde: sa_motion_publish_stop(ctx,0x8c614ddeu); if((*sr&1u))goto L8c614e2c;goto L8c614de0; /* bt 0x8c614e2c */
L8c614de0: sa_motion_publish_stop(ctx,0x8c614de0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614de2; /* fldi0 fr3 */
L8c614de2: sa_motion_publish_stop(ctx,0x8c614de2u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614de4; /* fcmp/gt fr4,fr3 */
L8c614de4: sa_motion_publish_stop(ctx,0x8c614de4u); if((*sr&1u))goto L8c614e74;goto L8c614de6; /* bt 0x8c614e74 */
L8c614de6: sa_motion_publish_stop(ctx,0x8c614de6u); r[5]=rd(m,bus,&io_status,0x8c614fa8u,4);if(io_status)return io_status;goto L8c614de8; /* mov.l 0x8c614fa8,r5 */
L8c614de8: sa_motion_publish_stop(ctx,0x8c614de8u); r[4]=r[11];goto L8c614dea; /* mov r11,r4 */
L8c614dea: sa_motion_publish_stop(ctx,0x8c614deau); r[8]+=16u;goto L8c614dec; /* add #0x10,r8 */
L8c614dec: sa_motion_publish_stop(ctx,0x8c614decu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614dee; /* fschg */
L8c614dee: sa_motion_publish_stop(ctx,0x8c614deeu); fcopy(f,0u,1u);goto L8c614df0; /* fmov fr1,fr0 */
L8c614df0: sa_motion_publish_stop(ctx,0x8c614df0u); fcopy(f,2u,3u);goto L8c614df2; /* fmov fr3,fr2 */
L8c614df2: sa_motion_publish_stop(ctx,0x8c614df2u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614df4; /* fmov.s @r4+,fr4 */
L8c614df4: sa_motion_publish_stop(ctx,0x8c614df4u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614df6; /* fmov.s @r4+,fr6 */
L8c614df6: sa_motion_publish_stop(ctx,0x8c614df6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614df8; /* fldi0 fr3 */
L8c614df8: sa_motion_publish_stop(ctx,0x8c614df8u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c614dfa; /* fipr fv4,fv0 */
L8c614dfa: sa_motion_publish_stop(ctx,0x8c614dfau); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614dfc; /* fmov.s @r4+,fr8 */
L8c614dfc: sa_motion_publish_stop(ctx,0x8c614dfcu); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614dfe; /* fmov.s @r4+,fr10 */
L8c614dfe: sa_motion_publish_stop(ctx,0x8c614dfeu); fcopy(f,0u,9u);goto L8c614e00; /* fmov fr9,fr0 */
L8c614e00: sa_motion_publish_stop(ctx,0x8c614e00u); fcopy(f,4u,5u);goto L8c614e02; /* fmov fr5,fr4 */
L8c614e02: sa_motion_publish_stop(ctx,0x8c614e02u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614e04; /* fsub fr3,fr0 */
L8c614e04: sa_motion_publish_stop(ctx,0x8c614e04u); fcopy(f,6u,7u);goto L8c614e06; /* fmov fr7,fr6 */
L8c614e06: sa_motion_publish_stop(ctx,0x8c614e06u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614e08; /* fschg */
L8c614e08: sa_motion_publish_stop(ctx,0x8c614e08u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c614e0a; /* fmov.s fr10,@-r8 */
L8c614e0a: sa_motion_publish_stop(ctx,0x8c614e0au); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c614e0c; /* fmov.s fr0,@-r4 */
L8c614e0c: sa_motion_publish_stop(ctx,0x8c614e0cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c614e0e; /* ocbwb @r4 */
L8c614e0e: sa_motion_publish_stop(ctx,0x8c614e0eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c614e10; /* fmul fr0,fr7 */
L8c614e10: sa_motion_publish_stop(ctx,0x8c614e10u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c614e12; /* fmov.s fr9,@-r8 */
L8c614e12: sa_motion_publish_stop(ctx,0x8c614e12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c614e14; /* fmul fr0,fr6 */
L8c614e14: sa_motion_publish_stop(ctx,0x8c614e14u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c614e16; /* fmov.s fr8,@-r8 */
L8c614e16: sa_motion_publish_stop(ctx,0x8c614e16u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c614e18; /* fmul fr0,fr5 */
L8c614e18: sa_motion_publish_stop(ctx,0x8c614e18u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614e1a; /* mov.l r5,@-r8 */
L8c614e1a: sa_motion_publish_stop(ctx,0x8c614e1au); r[8]+=32u;goto L8c614e1c; /* add #0x20,r8 */
L8c614e1c: sa_motion_publish_stop(ctx,0x8c614e1cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c614e1e; /* fmov.s fr7,@-r8 */
L8c614e1e: sa_motion_publish_stop(ctx,0x8c614e1eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614e20; /* fmov.s fr6,@-r8 */
L8c614e20: sa_motion_publish_stop(ctx,0x8c614e20u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614e22; /* fmov.s fr5,@-r8 */
L8c614e22: sa_motion_publish_stop(ctx,0x8c614e22u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614e24; /* fmov.s fr4,@-r8 */
L8c614e24: sa_motion_publish_stop(ctx,0x8c614e24u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614e26; /* pref @r8 */
L8c614e26: sa_motion_publish_stop(ctx,0x8c614e26u); r[8]+=16u;goto L8c614e28; /* add #0x10,r8 */
L8c614e28: sa_motion_publish_stop(ctx,0x8c614e2au); r[9]=r[8];goto L8c614eb6; /* bra 0x8c614eb6 */
L8c614e2c: sa_motion_publish_stop(ctx,0x8c614e2cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614e2e; /* fldi0 fr3 */
L8c614e2e: sa_motion_publish_stop(ctx,0x8c614e2eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614e30; /* fcmp/gt fr4,fr3 */
L8c614e30: sa_motion_publish_stop(ctx,0x8c614e30u); if(!(*sr&1u))goto L8c614eb6;goto L8c614e32; /* bf 0x8c614eb6 */
L8c614e32: sa_motion_publish_stop(ctx,0x8c614e32u); r[5]=rd(m,bus,&io_status,0x8c614facu,4);if(io_status)return io_status;goto L8c614e34; /* mov.l 0x8c614fac,r5 */
L8c614e34: sa_motion_publish_stop(ctx,0x8c614e34u); r[4]=r[10];goto L8c614e36; /* mov r10,r4 */
L8c614e36: sa_motion_publish_stop(ctx,0x8c614e36u); r[8]+=16u;goto L8c614e38; /* add #0x10,r8 */
L8c614e38: sa_motion_publish_stop(ctx,0x8c614e38u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614e3a; /* fschg */
L8c614e3a: sa_motion_publish_stop(ctx,0x8c614e3au); fcopy(f,0u,1u);goto L8c614e3c; /* fmov fr1,fr0 */
L8c614e3c: sa_motion_publish_stop(ctx,0x8c614e3cu); fcopy(f,2u,3u);goto L8c614e3e; /* fmov fr3,fr2 */
L8c614e3e: sa_motion_publish_stop(ctx,0x8c614e3eu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e40; /* fmov.s @r4+,fr4 */
L8c614e40: sa_motion_publish_stop(ctx,0x8c614e40u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e42; /* fmov.s @r4+,fr6 */
L8c614e42: sa_motion_publish_stop(ctx,0x8c614e42u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614e44; /* fldi0 fr3 */
L8c614e44: sa_motion_publish_stop(ctx,0x8c614e44u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c614e46; /* fipr fv4,fv0 */
L8c614e46: sa_motion_publish_stop(ctx,0x8c614e46u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e48; /* fmov.s @r4+,fr8 */
L8c614e48: sa_motion_publish_stop(ctx,0x8c614e48u); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e4a; /* fmov.s @r4+,fr10 */
L8c614e4a: sa_motion_publish_stop(ctx,0x8c614e4au); fcopy(f,0u,9u);goto L8c614e4c; /* fmov fr9,fr0 */
L8c614e4c: sa_motion_publish_stop(ctx,0x8c614e4cu); fcopy(f,4u,5u);goto L8c614e4e; /* fmov fr5,fr4 */
L8c614e4e: sa_motion_publish_stop(ctx,0x8c614e4eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614e50; /* fsub fr3,fr0 */
L8c614e50: sa_motion_publish_stop(ctx,0x8c614e50u); fcopy(f,6u,7u);goto L8c614e52; /* fmov fr7,fr6 */
L8c614e52: sa_motion_publish_stop(ctx,0x8c614e52u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614e54; /* fschg */
L8c614e54: sa_motion_publish_stop(ctx,0x8c614e54u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c614e56; /* fmov.s fr10,@-r8 */
L8c614e56: sa_motion_publish_stop(ctx,0x8c614e56u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c614e58; /* fmov.s fr0,@-r4 */
L8c614e58: sa_motion_publish_stop(ctx,0x8c614e58u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c614e5a; /* ocbwb @r4 */
L8c614e5a: sa_motion_publish_stop(ctx,0x8c614e5au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c614e5c; /* fmul fr0,fr7 */
L8c614e5c: sa_motion_publish_stop(ctx,0x8c614e5cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c614e5e; /* fmov.s fr9,@-r8 */
L8c614e5e: sa_motion_publish_stop(ctx,0x8c614e5eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c614e60; /* fmul fr0,fr6 */
L8c614e60: sa_motion_publish_stop(ctx,0x8c614e60u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c614e62; /* fmov.s fr8,@-r8 */
L8c614e62: sa_motion_publish_stop(ctx,0x8c614e62u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c614e64; /* fmul fr0,fr5 */
L8c614e64: sa_motion_publish_stop(ctx,0x8c614e64u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614e66; /* mov.l r5,@-r8 */
L8c614e66: sa_motion_publish_stop(ctx,0x8c614e66u); r[8]+=32u;goto L8c614e68; /* add #0x20,r8 */
L8c614e68: sa_motion_publish_stop(ctx,0x8c614e68u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c614e6a; /* fmov.s fr7,@-r8 */
L8c614e6a: sa_motion_publish_stop(ctx,0x8c614e6au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614e6c; /* fmov.s fr6,@-r8 */
L8c614e6c: sa_motion_publish_stop(ctx,0x8c614e6cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614e6e; /* fmov.s fr5,@-r8 */
L8c614e6e: sa_motion_publish_stop(ctx,0x8c614e6eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614e70; /* fmov.s fr4,@-r8 */
L8c614e70: sa_motion_publish_stop(ctx,0x8c614e70u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614e72; /* pref @r8 */
L8c614e72: sa_motion_publish_stop(ctx,0x8c614e72u); r[8]+=16u;goto L8c614e74; /* add #0x10,r8 */
L8c614e74: sa_motion_publish_stop(ctx,0x8c614e74u); r[5]=rd(m,bus,&io_status,0x8c614facu,4);if(io_status)return io_status;goto L8c614e76; /* mov.l 0x8c614fac,r5 */
L8c614e76: sa_motion_publish_stop(ctx,0x8c614e76u); r[4]=r[11];goto L8c614e78; /* mov r11,r4 */
L8c614e78: sa_motion_publish_stop(ctx,0x8c614e78u); r[8]+=16u;goto L8c614e7a; /* add #0x10,r8 */
L8c614e7a: sa_motion_publish_stop(ctx,0x8c614e7au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614e7c; /* fschg */
L8c614e7c: sa_motion_publish_stop(ctx,0x8c614e7cu); fcopy(f,0u,1u);goto L8c614e7e; /* fmov fr1,fr0 */
L8c614e7e: sa_motion_publish_stop(ctx,0x8c614e7eu); fcopy(f,2u,3u);goto L8c614e80; /* fmov fr3,fr2 */
L8c614e80: sa_motion_publish_stop(ctx,0x8c614e80u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e82; /* fmov.s @r4+,fr4 */
L8c614e82: sa_motion_publish_stop(ctx,0x8c614e82u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e84; /* fmov.s @r4+,fr6 */
L8c614e84: sa_motion_publish_stop(ctx,0x8c614e84u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614e86; /* fldi0 fr3 */
L8c614e86: sa_motion_publish_stop(ctx,0x8c614e86u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c614e88; /* fipr fv4,fv0 */
L8c614e88: sa_motion_publish_stop(ctx,0x8c614e88u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e8a; /* fmov.s @r4+,fr8 */
L8c614e8a: sa_motion_publish_stop(ctx,0x8c614e8au); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e8c; /* fmov.s @r4+,fr10 */
L8c614e8c: sa_motion_publish_stop(ctx,0x8c614e8cu); fcopy(f,0u,9u);goto L8c614e8e; /* fmov fr9,fr0 */
L8c614e8e: sa_motion_publish_stop(ctx,0x8c614e8eu); fcopy(f,4u,5u);goto L8c614e90; /* fmov fr5,fr4 */
L8c614e90: sa_motion_publish_stop(ctx,0x8c614e90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614e92; /* fsub fr3,fr0 */
L8c614e92: sa_motion_publish_stop(ctx,0x8c614e92u); fcopy(f,6u,7u);goto L8c614e94; /* fmov fr7,fr6 */
L8c614e94: sa_motion_publish_stop(ctx,0x8c614e94u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614e96; /* fschg */
L8c614e96: sa_motion_publish_stop(ctx,0x8c614e96u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c614e98; /* fmov.s fr10,@-r8 */
L8c614e98: sa_motion_publish_stop(ctx,0x8c614e98u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c614e9a; /* fmov.s fr0,@-r4 */
L8c614e9a: sa_motion_publish_stop(ctx,0x8c614e9au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c614e9c; /* ocbwb @r4 */
L8c614e9c: sa_motion_publish_stop(ctx,0x8c614e9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c614e9e; /* fmul fr0,fr7 */
L8c614e9e: sa_motion_publish_stop(ctx,0x8c614e9eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c614ea0; /* fmov.s fr9,@-r8 */
L8c614ea0: sa_motion_publish_stop(ctx,0x8c614ea0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c614ea2; /* fmul fr0,fr6 */
L8c614ea2: sa_motion_publish_stop(ctx,0x8c614ea2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c614ea4; /* fmov.s fr8,@-r8 */
L8c614ea4: sa_motion_publish_stop(ctx,0x8c614ea4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c614ea6; /* fmul fr0,fr5 */
L8c614ea6: sa_motion_publish_stop(ctx,0x8c614ea6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614ea8; /* mov.l r5,@-r8 */
L8c614ea8: sa_motion_publish_stop(ctx,0x8c614ea8u); r[8]+=32u;goto L8c614eaa; /* add #0x20,r8 */
L8c614eaa: sa_motion_publish_stop(ctx,0x8c614eaau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c614eac; /* fmov.s fr7,@-r8 */
L8c614eac: sa_motion_publish_stop(ctx,0x8c614eacu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614eae; /* fmov.s fr6,@-r8 */
L8c614eae: sa_motion_publish_stop(ctx,0x8c614eaeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614eb0; /* fmov.s fr5,@-r8 */
L8c614eb0: sa_motion_publish_stop(ctx,0x8c614eb0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614eb2; /* fmov.s fr4,@-r8 */
L8c614eb2: sa_motion_publish_stop(ctx,0x8c614eb2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614eb4; /* pref @r8 */
L8c614eb4: sa_motion_publish_stop(ctx,0x8c614eb4u); r[8]+=16u;goto L8c614eb6; /* add #0x10,r8 */
L8c614eb6: sa_motion_publish_stop(ctx,0x8c614eb6u); r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c614eb8; /* mov.l @(0x14,r15),r2 */
L8c614eb8: sa_motion_publish_stop(ctx,0x8c614eb8u); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c614eba; /* tst r2,r2 */
L8c614eba: sa_motion_publish_stop(ctx,0x8c614ebau); if((*sr&1u))goto L8c614ece;goto L8c614ebc; /* bt 0x8c614ece */
L8c614ebc: sa_motion_publish_stop(ctx,0x8c614ebcu); r[4]=rd(m,bus,&io_status,0x8c614fb0u,4);if(io_status)return io_status;goto L8c614ebe; /* mov.l 0x8c614fb0,r4 */
L8c614ebe: sa_motion_publish_stop(ctx,0x8c614ebeu); r[3]=0u;goto L8c614ec0; /* mov #0x0,r3 */
L8c614ec0: sa_motion_publish_stop(ctx,0x8c614ec0u); r[2]=rd(m,bus,&io_status,0x8c614fb4u,4);if(io_status)return io_status;goto L8c614ec2; /* mov.l 0x8c614fb4,r2 */
L8c614ec2: sa_motion_publish_stop(ctx,0x8c614ec2u); r[5]=r[11];goto L8c614ec4; /* mov r11,r5 */
L8c614ec4: sa_motion_publish_stop(ctx,0x8c614ec4u); wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c614ec6; /* mov.l r3,@r4 */
L8c614ec6: sa_motion_publish_stop(ctx,0x8c614ec6u); r[6]=r[12];goto L8c614ec8; /* mov r12,r6 */
L8c614ec8: sa_motion_publish_stop(ctx,0x8c614ecau); target=r[2];*pr=0x8c614eccu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c614ecc; /* jsr @r2 */
L8c614ecc: sa_motion_publish_stop(ctx,0x8c614eccu); r[9]=r[8];goto L8c614ece; /* mov r8,r9 */
L8c614ece: sa_motion_publish_stop(ctx,0x8c614eceu); fcopy(f,6u,13u);goto L8c614ed0; /* fmov fr13,fr6 */
L8c614ed0: sa_motion_publish_stop(ctx,0x8c614ed0u); r[0]=16u;goto L8c614ed2; /* mov #0x10,r0 */
L8c614ed2: sa_motion_publish_stop(ctx,0x8c614ed2u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614ed4; /* mov.l @(0x8,r15),r3 */
L8c614ed4: sa_motion_publish_stop(ctx,0x8c614ed4u); r[10]=r[11];goto L8c614ed6; /* mov r11,r10 */
L8c614ed6: sa_motion_publish_stop(ctx,0x8c614ed6u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c614ed8; /* fmov.s @(r0,r12),fr13 */
L8c614ed8: sa_motion_publish_stop(ctx,0x8c614ed8u); r[0]=20u;goto L8c614eda; /* mov #0x14,r0 */
L8c614eda: sa_motion_publish_stop(ctx,0x8c614edau); fcopy(f,5u,14u);goto L8c614edc; /* fmov fr14,fr5 */
L8c614edc: sa_motion_publish_stop(ctx,0x8c614edcu); r[3]+=4294967295u;goto L8c614ede; /* add #-0x1,r3 */
L8c614ede: sa_motion_publish_stop(ctx,0x8c614edeu); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c614ee0; /* fmov.s @(r0,r12),fr14 */
L8c614ee0: sa_motion_publish_stop(ctx,0x8c614ee0u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614ee2; /* tst r3,r3 */
L8c614ee2: sa_motion_publish_stop(ctx,0x8c614ee2u); r[11]=r[12];goto L8c614ee4; /* mov r12,r11 */
L8c614ee4: sa_motion_publish_stop(ctx,0x8c614ee4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c614ee6; /* fneg fr15 */
L8c614ee6: sa_motion_publish_stop(ctx,0x8c614ee8u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c614eee;goto L8c614eea; /* bt/s 0x8c614eee */
L8c614eea: sa_motion_publish_stop(ctx,0x8c614eecu); goto L8c614c4c; /* bra 0x8c614c4c */
L8c614eee: sa_motion_publish_stop(ctx,0x8c614eeeu); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c614ef0; /* cmp/hs r8,r9 */
L8c614ef0: sa_motion_publish_stop(ctx,0x8c614ef0u); if((*sr&1u))goto L8c614f34;goto L8c614ef2; /* bt 0x8c614f34 */
L8c614ef2: sa_motion_publish_stop(ctx,0x8c614ef2u); r[5]=rd(m,bus,&io_status,0x8c614fa8u,4);if(io_status)return io_status;goto L8c614ef4; /* mov.l 0x8c614fa8,r5 */
L8c614ef4: sa_motion_publish_stop(ctx,0x8c614ef4u); r[4]=r[12];goto L8c614ef6; /* mov r12,r4 */
L8c614ef6: sa_motion_publish_stop(ctx,0x8c614ef6u); r[8]+=16u;goto L8c614ef8; /* add #0x10,r8 */
L8c614ef8: sa_motion_publish_stop(ctx,0x8c614ef8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614efa; /* fschg */
L8c614efa: sa_motion_publish_stop(ctx,0x8c614efau); fcopy(f,0u,1u);goto L8c614efc; /* fmov fr1,fr0 */
L8c614efc: sa_motion_publish_stop(ctx,0x8c614efcu); fcopy(f,2u,3u);goto L8c614efe; /* fmov fr3,fr2 */
L8c614efe: sa_motion_publish_stop(ctx,0x8c614efeu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f00; /* fmov.s @r4+,fr4 */
L8c614f00: sa_motion_publish_stop(ctx,0x8c614f00u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f02; /* fmov.s @r4+,fr6 */
L8c614f02: sa_motion_publish_stop(ctx,0x8c614f02u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614f04; /* fldi0 fr3 */
L8c614f04: sa_motion_publish_stop(ctx,0x8c614f04u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c614f06; /* fipr fv4,fv0 */
L8c614f06: sa_motion_publish_stop(ctx,0x8c614f06u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f08; /* fmov.s @r4+,fr8 */
L8c614f08: sa_motion_publish_stop(ctx,0x8c614f08u); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f0a; /* fmov.s @r4+,fr10 */
L8c614f0a: sa_motion_publish_stop(ctx,0x8c614f0au); fcopy(f,0u,9u);goto L8c614f0c; /* fmov fr9,fr0 */
L8c614f0c: sa_motion_publish_stop(ctx,0x8c614f0cu); fcopy(f,4u,5u);goto L8c614f0e; /* fmov fr5,fr4 */
L8c614f0e: sa_motion_publish_stop(ctx,0x8c614f0eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614f10; /* fsub fr3,fr0 */
L8c614f10: sa_motion_publish_stop(ctx,0x8c614f10u); fcopy(f,6u,7u);goto L8c614f12; /* fmov fr7,fr6 */
L8c614f12: sa_motion_publish_stop(ctx,0x8c614f12u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614f14; /* fschg */
L8c614f14: sa_motion_publish_stop(ctx,0x8c614f14u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c614f16; /* fmov.s fr10,@-r8 */
L8c614f16: sa_motion_publish_stop(ctx,0x8c614f16u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c614f18; /* fmov.s fr0,@-r4 */
L8c614f18: sa_motion_publish_stop(ctx,0x8c614f18u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c614f1a; /* ocbwb @r4 */
L8c614f1a: sa_motion_publish_stop(ctx,0x8c614f1au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c614f1c; /* fmul fr0,fr7 */
L8c614f1c: sa_motion_publish_stop(ctx,0x8c614f1cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c614f1e; /* fmov.s fr9,@-r8 */
L8c614f1e: sa_motion_publish_stop(ctx,0x8c614f1eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c614f20; /* fmul fr0,fr6 */
L8c614f20: sa_motion_publish_stop(ctx,0x8c614f20u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c614f22; /* fmov.s fr8,@-r8 */
L8c614f22: sa_motion_publish_stop(ctx,0x8c614f22u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c614f24; /* fmul fr0,fr5 */
L8c614f24: sa_motion_publish_stop(ctx,0x8c614f24u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614f26; /* mov.l r5,@-r8 */
L8c614f26: sa_motion_publish_stop(ctx,0x8c614f26u); r[8]+=32u;goto L8c614f28; /* add #0x20,r8 */
L8c614f28: sa_motion_publish_stop(ctx,0x8c614f28u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c614f2a; /* fmov.s fr7,@-r8 */
L8c614f2a: sa_motion_publish_stop(ctx,0x8c614f2au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614f2c; /* fmov.s fr6,@-r8 */
L8c614f2c: sa_motion_publish_stop(ctx,0x8c614f2cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614f2e; /* fmov.s fr5,@-r8 */
L8c614f2e: sa_motion_publish_stop(ctx,0x8c614f2eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614f30; /* fmov.s fr4,@-r8 */
L8c614f30: sa_motion_publish_stop(ctx,0x8c614f30u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614f32; /* pref @r8 */
L8c614f32: sa_motion_publish_stop(ctx,0x8c614f32u); r[8]+=16u;goto L8c614f34; /* add #0x10,r8 */
L8c614f34: sa_motion_publish_stop(ctx,0x8c614f34u); r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c614f36; /* mov.l @(0x18,r15),r3 */
L8c614f36: sa_motion_publish_stop(ctx,0x8c614f36u); r[3]+=4294967295u;goto L8c614f38; /* add #-0x1,r3 */
L8c614f38: sa_motion_publish_stop(ctx,0x8c614f38u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614f3a; /* tst r3,r3 */
L8c614f3a: sa_motion_publish_stop(ctx,0x8c614f3cu); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c614f42;goto L8c614f3e; /* bt/s 0x8c614f42 */
L8c614f3e: sa_motion_publish_stop(ctx,0x8c614f40u); goto L8c614bfc; /* bra 0x8c614bfc */
L8c614f42: sa_motion_publish_stop(ctx,0x8c614f42u); r[4]=rd(m,bus,&io_status,0x8c614fb8u,4);if(io_status)return io_status;goto L8c614f44; /* mov.l 0x8c614fb8,r4 */
L8c614f44: sa_motion_publish_stop(ctx,0x8c614f44u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614f46; /* mov.l @(0xc,r15),r3 */
L8c614f46: sa_motion_publish_stop(ctx,0x8c614f46u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614f48; /* mov.l @r4,r2 */
L8c614f48: sa_motion_publish_stop(ctx,0x8c614f48u); r[2]+=r[3];goto L8c614f4a; /* add r3,r2 */
L8c614f4a: sa_motion_publish_stop(ctx,0x8c614f4au); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c614f4c; /* mov.l r2,@r4 */
L8c614f4c: sa_motion_publish_stop(ctx,0x8c614f4cu); r[4]=rd(m,bus,&io_status,0x8c614fbcu,4);if(io_status)return io_status;goto L8c614f4e; /* mov.l 0x8c614fbc,r4 */
L8c614f4e: sa_motion_publish_stop(ctx,0x8c614f4eu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614f50; /* mov.l @(0xc,r15),r3 */
L8c614f50: sa_motion_publish_stop(ctx,0x8c614f50u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614f52; /* mov.l @r4,r2 */
L8c614f52: sa_motion_publish_stop(ctx,0x8c614f52u); r[2]+=r[3];goto L8c614f54; /* add r3,r2 */
L8c614f54: sa_motion_publish_stop(ctx,0x8c614f54u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c614f56; /* mov.l r2,@r4 */
L8c614f56: sa_motion_publish_stop(ctx,0x8c614f56u); r[15]+=76u;goto L8c614f58; /* add #0x4c,r15 */
L8c614f58: sa_motion_publish_stop(ctx,0x8c614f58u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f5a; /* lds.l @r15+,PR */
L8c614f5a: sa_motion_publish_stop(ctx,0x8c614f5au); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f5c; /* fmov.s @r15+,fr12 */
L8c614f5c: sa_motion_publish_stop(ctx,0x8c614f5cu); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f5e; /* fmov.s @r15+,fr13 */
L8c614f5e: sa_motion_publish_stop(ctx,0x8c614f5eu); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f60; /* fmov.s @r15+,fr14 */
L8c614f60: sa_motion_publish_stop(ctx,0x8c614f60u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f62; /* fmov.s @r15+,fr15 */
L8c614f62: sa_motion_publish_stop(ctx,0x8c614f62u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f64; /* mov.l @r15+,r9 */
L8c614f64: sa_motion_publish_stop(ctx,0x8c614f64u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f66; /* mov.l @r15+,r10 */
L8c614f66: sa_motion_publish_stop(ctx,0x8c614f66u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f68; /* mov.l @r15+,r11 */
L8c614f68: sa_motion_publish_stop(ctx,0x8c614f68u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f6a; /* mov.l @r15+,r12 */
L8c614f6a: sa_motion_publish_stop(ctx,0x8c614f6au); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f6c; /* mov.l @r15+,r13 */
L8c614f6c: sa_motion_publish_stop(ctx,0x8c614f6eu); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
