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
L8c614580: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614582; /* mov.l r14,@-r15 */
L8c614582: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c614584; /* mov.l r13,@-r15 */
L8c614584: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c614586; /* mov.l r12,@-r15 */
L8c614586: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c614588; /* mov.l r11,@-r15 */
L8c614588: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61458a; /* mov.l r10,@-r15 */
L8c61458a: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61458c; /* mov.l r9,@-r15 */
L8c61458c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61458e; /* fmov.s fr15,@-r15 */
L8c61458e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c614590; /* fmov.s fr14,@-r15 */
L8c614590: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c614592; /* fmov.s fr13,@-r15 */
L8c614592: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c614594; /* sts.l PR,@-r15 */
L8c614594: r[15]+=4294967280u;goto L8c614596; /* add #-0x10,r15 */
L8c614596: r[3]=rd(m,bus,&io_status,0x8c6145c8u,4);if(io_status)return io_status;goto L8c614598; /* mov.l 0x8c6145c8,r3 */
L8c614598: r[9]=rd(m,bus,&io_status,0x8c6145c4u,4);if(io_status)return io_status;goto L8c61459a; /* mov.l 0x8c6145c4,r9 */
L8c61459a: target=r[3];*pr=0x8c61459eu;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61459e; /* jsr @r3 */
L8c61459e: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6145a0; /* mov.l @(0x4,r15),r3 */
L8c6145a0: r[2]=0u;goto L8c6145a2; /* mov #0x0,r2 */
L8c6145a2: r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6145a4; /* mov.l @(0x4,r15),r14 */
L8c6145a4: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6145a6; /* mov.w @(0x2,r3),r0 */
L8c6145a6: r[3]=rd(m,bus,&io_status,0x8c6145ccu,4);if(io_status)return io_status;goto L8c6145a8; /* mov.l 0x8c6145cc,r3 */
L8c6145a8: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6145aa; /* mov.l @(0x4,r14),r14 */
L8c6145aa: r[0]=r[0]&65535u;goto L8c6145ac; /* extu.w r0,r0 */
L8c6145ac: wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6145ae; /* mov.l r0,@(0xc,r15) */
L8c6145ae: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6145b0; /* mov.l @r3,r13 */
L8c6145b0: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6145b2; /* mov.l r2,@(0x8,r15) */
L8c6145b2: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6145b4; /* mov.l r2,@(0x4,r15) */
L8c6145b4: goto L8c6145d0; /* bra 0x8c6145d0 */
L8c6145d0: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6145d2; /* mov.w @r14+,r3 */
L8c6145d2: r[4]=rd(m,bus,&io_status,0x8c6146a4u,4);if(io_status)return io_status;goto L8c6145d4; /* mov.l 0x8c6146a4,r4 */
L8c6145d4: r[3]=r[3]&65535u;goto L8c6145d6; /* extu.w r3,r3 */
L8c6145d6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6145d8; /* fldi1 fr4 */
L8c6145d8: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6145da; /* tst r4,r3 */
L8c6145da: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6145dc; /* mov.l r3,@r15 */
L8c6145dc: take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c6145ee;goto L8c6145e0; /* bt/s 0x8c6145ee */
L8c6145e0: r[4]=sx(m,bus,&io_status,0x8c6146a0u,2);if(io_status)return io_status;goto L8c6145e2; /* mov.w 0x8c6146a0,r4 */
L8c6145e2: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6145e4; /* mov.l @r15,r3 */
L8c6145e4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6145e6; /* fldi1 fr4 */
L8c6145e6: r[3]&=r[4];goto L8c6145e8; /* and r4,r3 */
L8c6145e8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6145ea; /* fneg fr4 */
L8c6145ea: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6145ec; /* mov.l r3,@r15 */
L8c6145ec: fcopy(f,15u,4u);goto L8c6145ee; /* fmov fr4,fr15 */
L8c6145ee: r[0]=16u;goto L8c6145f0; /* mov #0x10,r0 */
L8c6145f0: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6145f2; /* mov.l @r15,r3 */
L8c6145f2: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6145f4; /* mov.l @(0x4,r15),r2 */
L8c6145f4: r[10]=r[8];goto L8c6145f6; /* mov r8,r10 */
L8c6145f6: r[2]+=r[3];goto L8c6145f8; /* add r3,r2 */
L8c6145f8: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6145fa; /* mov.l r2,@(0x4,r15) */
L8c6145fa: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6145fc; /* mov.l @r15,r1 */
L8c6145fc: r[1]+=4294967294u;goto L8c6145fe; /* add #-0x2,r1 */
L8c6145fe: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c614600; /* mov.l r1,@r15 */
L8c614600: r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614602; /* mov.w @r14+,r4 */
L8c614602: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614604; /* mov.w @r14+,r11 */
L8c614604: r[4]=r[4]&65535u;goto L8c614606; /* extu.w r4,r4 */
L8c614606: r[4]<<=2;goto L8c614608; /* shll2 r4 */
L8c614608: r[11]=r[11]&65535u;goto L8c61460a; /* extu.w r11,r11 */
L8c61460a: r[11]<<=2;goto L8c61460c; /* shll2 r11 */
L8c61460c: r[4]<<=2;goto L8c61460e; /* shll2 r4 */
L8c61460e: r[11]<<=2;goto L8c614610; /* shll2 r11 */
L8c614610: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c614612; /* shll r4 */
L8c614612: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c614614; /* shll r11 */
L8c614614: r[11]+=r[13];goto L8c614616; /* add r13,r11 */
L8c614616: r[4]+=r[13];goto L8c614618; /* add r13,r4 */
L8c614618: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c61461a; /* fmov.s @(r0,r11),fr14 */
L8c61461a: fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c61461c; /* fmov.s @(r0,r4),fr4 */
L8c61461c: r[0]=20u;goto L8c61461e; /* mov #0x14,r0 */
L8c61461e: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c614620; /* fmov.s @(r0,r11),fr13 */
L8c614620: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c614622; /* fmov.s @(r0,r4),fr5 */
L8c614622: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614624; /* mov.w @r14+,r12 */
L8c614624: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c614626; /* mov.w @r14,r3 */
L8c614626: r[12]=r[12]&65535u;goto L8c614628; /* extu.w r12,r12 */
L8c614628: r[12]<<=2;goto L8c61462a; /* shll2 r12 */
L8c61462a: r[3]=r[3]&65535u;goto L8c61462c; /* extu.w r3,r3 */
L8c61462c: r[3]<<=2;goto L8c61462e; /* shll2 r3 */
L8c61462e: r[12]<<=2;goto L8c614630; /* shll2 r12 */
L8c614630: r[3]<<=2;goto L8c614632; /* shll2 r3 */
L8c614632: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c614634; /* shll r12 */
L8c614634: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c614636; /* shll r3 */
L8c614636: r[3]+=r[13];goto L8c614638; /* add r13,r3 */
L8c614638: r[12]+=r[13];goto L8c61463a; /* add r13,r12 */
L8c61463a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c61463c; /* pref @r3 */
L8c61463c: r[0]=20u;goto L8c61463e; /* mov #0x14,r0 */
L8c61463e: fcopy(f,3u,14u);goto L8c614640; /* fmov fr14,fr3 */
L8c614640: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c614642; /* fmov.s @(r0,r12),fr2 */
L8c614642: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614644; /* fsub fr4,fr3 */
L8c614644: r[0]=16u;goto L8c614646; /* mov #0x10,r0 */
L8c614646: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614648; /* cmp/hs r8,r10 */
L8c614648: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61464a; /* fsub fr13,fr2 */
L8c61464a: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c61464c; /* fmov.s @(r0,r12),fr1 */
L8c61464c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61464e; /* fsub fr14,fr1 */
L8c61464e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614650; /* fmul fr2,fr3 */
L8c614650: fcopy(f,2u,13u);goto L8c614652; /* fmov fr13,fr2 */
L8c614652: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614654; /* fsub fr5,fr2 */
L8c614654: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614656; /* fmul fr1,fr2 */
L8c614656: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614658; /* fsub fr2,fr3 */
L8c614658: fcopy(f,4u,3u);goto L8c61465a; /* fmov fr3,fr4 */
L8c61465a: take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c6146a8;goto L8c61465e; /* bt/s 0x8c6146a8 */
L8c61465e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614660; /* fldi0 fr3 */
L8c614660: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614662; /* fcmp/gt fr4,fr3 */
L8c614662: if((*sr&1u))goto L8c6146d6;goto L8c614664; /* bt 0x8c6146d6 */
L8c614664: r[4]=r[11];goto L8c614666; /* mov r11,r4 */
L8c614666: r[5]=r[9];goto L8c614668; /* mov r9,r5 */
L8c614668: r[8]+=16u;goto L8c61466a; /* add #0x10,r8 */
L8c61466a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61466c; /* fschg */
L8c61466c: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61466e; /* fmov.s @r4+,fr0 */
L8c61466e: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614670; /* fmov.s @r4+,fr2 */
L8c614670: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c614672; /* fldi1 fr3 */
L8c614672: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c614674; /* ftrv xmtrx,fv0 */
L8c614674: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614676; /* fmov.s @r4+,fr4 */
L8c614676: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614678; /* fmov.s @r4,fr6 */
L8c614678: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61467a; /* fschg */
L8c61467a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c61467c; /* fmov.s fr6,@-r8 */
L8c61467c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61467e; /* fmov.s fr5,@-r8 */
L8c61467e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614680; /* fmov.s fr4,@-r8 */
L8c614680: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614682; /* mov.l r5,@-r8 */
L8c614682: r[8]+=28u;goto L8c614684; /* add #0x1c,r8 */
L8c614684: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614686; /* fmov.s fr2,@-r8 */
L8c614686: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c614688; /* fmov.s fr1,@-r8 */
L8c614688: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c61468a; /* fmov.s fr0,@-r8 */
L8c61468a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61468c; /* pref @r8 */
L8c61468c: r[8]+=16u;goto L8c61468e; /* add #0x10,r8 */
L8c61468e: r[3]=r[8];goto L8c614690; /* mov r8,r3 */
L8c614690: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614692; /* mov.l @(0x8,r15),r2 */
L8c614692: r[3]-=r[10];goto L8c614694; /* sub r10,r3 */
L8c614694: r[3]+=32u;goto L8c614696; /* add #0x20,r3 */
L8c614696: r[2]+=r[3];goto L8c614698; /* add r3,r2 */
L8c614698: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c61469a; /* mov.l r2,@(0x8,r15) */
L8c61469a: r[10]=r[8];goto L8c614700; /* bra 0x8c614700 */
L8c6146a8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6146aa; /* fldi0 fr3 */
L8c6146aa: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6146ac; /* fcmp/gt fr4,fr3 */
L8c6146ac: if(!(*sr&1u))goto L8c614700;goto L8c6146ae; /* bf 0x8c614700 */
L8c6146ae: r[5]=rd(m,bus,&io_status,0x8c6147d0u,4);if(io_status)return io_status;goto L8c6146b0; /* mov.l 0x8c6147d0,r5 */
L8c6146b0: r[8]+=16u;goto L8c6146b2; /* add #0x10,r8 */
L8c6146b2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6146b4; /* fschg */
L8c6146b4: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146b6; /* fmov.s @r4+,fr0 */
L8c6146b6: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146b8; /* fmov.s @r4+,fr2 */
L8c6146b8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6146ba; /* fldi1 fr3 */
L8c6146ba: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6146bc; /* ftrv xmtrx,fv0 */
L8c6146bc: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146be; /* fmov.s @r4+,fr4 */
L8c6146be: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6146c0; /* fmov.s @r4,fr6 */
L8c6146c0: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6146c2; /* fschg */
L8c6146c2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6146c4; /* fmov.s fr6,@-r8 */
L8c6146c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6146c6; /* fmov.s fr5,@-r8 */
L8c6146c6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6146c8; /* fmov.s fr4,@-r8 */
L8c6146c8: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6146ca; /* mov.l r5,@-r8 */
L8c6146ca: r[8]+=28u;goto L8c6146cc; /* add #0x1c,r8 */
L8c6146cc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6146ce; /* fmov.s fr2,@-r8 */
L8c6146ce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6146d0; /* fmov.s fr1,@-r8 */
L8c6146d0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6146d2; /* fmov.s fr0,@-r8 */
L8c6146d2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6146d4; /* pref @r8 */
L8c6146d4: r[8]+=16u;goto L8c6146d6; /* add #0x10,r8 */
L8c6146d6: r[5]=rd(m,bus,&io_status,0x8c6147d0u,4);if(io_status)return io_status;goto L8c6146d8; /* mov.l 0x8c6147d0,r5 */
L8c6146d8: r[4]=r[11];goto L8c6146da; /* mov r11,r4 */
L8c6146da: r[8]+=16u;goto L8c6146dc; /* add #0x10,r8 */
L8c6146dc: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6146de; /* fschg */
L8c6146de: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146e0; /* fmov.s @r4+,fr0 */
L8c6146e0: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146e2; /* fmov.s @r4+,fr2 */
L8c6146e2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6146e4; /* fldi1 fr3 */
L8c6146e4: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6146e6; /* ftrv xmtrx,fv0 */
L8c6146e6: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146e8; /* fmov.s @r4+,fr4 */
L8c6146e8: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6146ea; /* fmov.s @r4,fr6 */
L8c6146ea: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6146ec; /* fschg */
L8c6146ec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6146ee; /* fmov.s fr6,@-r8 */
L8c6146ee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6146f0; /* fmov.s fr5,@-r8 */
L8c6146f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6146f2; /* fmov.s fr4,@-r8 */
L8c6146f2: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6146f4; /* mov.l r5,@-r8 */
L8c6146f4: r[8]+=28u;goto L8c6146f6; /* add #0x1c,r8 */
L8c6146f6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6146f8; /* fmov.s fr2,@-r8 */
L8c6146f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6146fa; /* fmov.s fr1,@-r8 */
L8c6146fa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6146fc; /* fmov.s fr0,@-r8 */
L8c6146fc: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6146fe; /* pref @r8 */
L8c6146fe: r[8]+=16u;goto L8c614700; /* add #0x10,r8 */
L8c614700: fcopy(f,4u,14u);goto L8c614702; /* fmov fr14,fr4 */
L8c614702: r[0]=16u;goto L8c614704; /* mov #0x10,r0 */
L8c614704: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614706; /* mov.l @r15,r3 */
L8c614706: r[4]=r[11];goto L8c614708; /* mov r11,r4 */
L8c614708: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c61470a; /* fmov.s @(r0,r12),fr14 */
L8c61470a: r[0]=20u;goto L8c61470c; /* mov #0x14,r0 */
L8c61470c: fcopy(f,5u,13u);goto L8c61470e; /* fmov fr13,fr5 */
L8c61470e: r[3]+=4294967295u;goto L8c614710; /* add #-0x1,r3 */
L8c614710: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c614712; /* fmov.s @(r0,r12),fr13 */
L8c614712: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614714; /* tst r3,r3 */
L8c614714: r[11]=r[12];goto L8c614716; /* mov r12,r11 */
L8c614716: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c614718; /* fneg fr15 */
L8c614718: take=!(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c614622;goto L8c61471c; /* bf/s 0x8c614622 */
L8c61471c: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c61471e; /* cmp/hs r8,r10 */
L8c61471e: if((*sr&1u))goto L8c614756;goto L8c614720; /* bt 0x8c614756 */
L8c614720: r[4]=r[12];goto L8c614722; /* mov r12,r4 */
L8c614722: r[5]=r[9];goto L8c614724; /* mov r9,r5 */
L8c614724: r[8]+=16u;goto L8c614726; /* add #0x10,r8 */
L8c614726: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614728; /* fschg */
L8c614728: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61472a; /* fmov.s @r4+,fr0 */
L8c61472a: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61472c; /* fmov.s @r4+,fr2 */
L8c61472c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61472e; /* fldi1 fr3 */
L8c61472e: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c614730; /* ftrv xmtrx,fv0 */
L8c614730: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614732; /* fmov.s @r4+,fr4 */
L8c614732: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614734; /* fmov.s @r4,fr6 */
L8c614734: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614736; /* fschg */
L8c614736: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614738; /* fmov.s fr6,@-r8 */
L8c614738: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61473a; /* fmov.s fr5,@-r8 */
L8c61473a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61473c; /* fmov.s fr4,@-r8 */
L8c61473c: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61473e; /* mov.l r5,@-r8 */
L8c61473e: r[8]+=28u;goto L8c614740; /* add #0x1c,r8 */
L8c614740: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614742; /* fmov.s fr2,@-r8 */
L8c614742: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c614744; /* fmov.s fr1,@-r8 */
L8c614744: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614746; /* fmov.s fr0,@-r8 */
L8c614746: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614748; /* pref @r8 */
L8c614748: r[8]+=16u;goto L8c61474a; /* add #0x10,r8 */
L8c61474a: r[3]=r[8];goto L8c61474c; /* mov r8,r3 */
L8c61474c: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61474e; /* mov.l @(0x8,r15),r2 */
L8c61474e: r[3]-=r[10];goto L8c614750; /* sub r10,r3 */
L8c614750: r[3]+=32u;goto L8c614752; /* add #0x20,r3 */
L8c614752: r[2]+=r[3];goto L8c614754; /* add r3,r2 */
L8c614754: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614756; /* mov.l r2,@(0x8,r15) */
L8c614756: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614758; /* mov.l @(0xc,r15),r3 */
L8c614758: r[3]+=4294967295u;goto L8c61475a; /* add #-0x1,r3 */
L8c61475a: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61475c; /* tst r3,r3 */
L8c61475c: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c614764;goto L8c614760; /* bt/s 0x8c614764 */
L8c614760: goto L8c6145d0; /* bra 0x8c6145d0 */
L8c614764: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614766; /* mov.l @(0x8,r15),r3 */
L8c614766: r[4]=rd(m,bus,&io_status,0x8c6147d4u,4);if(io_status)return io_status;goto L8c614768; /* mov.l 0x8c6147d4,r4 */
L8c614768: r[3]>>=2;goto L8c61476a; /* shlr2 r3 */
L8c61476a: r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61476c; /* mov.l @r4,r1 */
L8c61476c: r[3]>>=2;goto L8c61476e; /* shlr2 r3 */
L8c61476e: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c614770; /* shlr r3 */
L8c614770: r[1]+=r[3];goto L8c614772; /* add r3,r1 */
L8c614772: wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c614774; /* mov.l r1,@r4 */
L8c614774: r[4]=rd(m,bus,&io_status,0x8c6147d8u,4);if(io_status)return io_status;goto L8c614776; /* mov.l 0x8c6147d8,r4 */
L8c614776: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614778; /* mov.l @(0x4,r15),r3 */
L8c614778: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61477a; /* mov.l @r4,r2 */
L8c61477a: r[2]+=r[3];goto L8c61477c; /* add r3,r2 */
L8c61477c: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61477e; /* mov.l r2,@r4 */
L8c61477e: r[15]+=16u;goto L8c614780; /* add #0x10,r15 */
L8c614780: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614782; /* lds.l @r15+,PR */
L8c614782: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614784; /* fmov.s @r15+,fr13 */
L8c614784: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614786; /* fmov.s @r15+,fr14 */
L8c614786: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614788; /* fmov.s @r15+,fr15 */
L8c614788: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61478a; /* mov.l @r15+,r9 */
L8c61478a: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61478c; /* mov.l @r15+,r10 */
L8c61478c: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61478e; /* mov.l @r15+,r11 */
L8c61478e: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614790; /* mov.l @r15+,r12 */
L8c614790: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614792; /* mov.l @r15+,r13 */
L8c614792: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_face_alternate_strip_clipped_8c616474(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c616474;
L8c616474: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c616476; /* mov.l r14,@-r15 */
L8c616476: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c616478; /* mov.l r13,@-r15 */
L8c616478: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61647a; /* mov.l r12,@-r15 */
L8c61647a: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61647c; /* mov.l r11,@-r15 */
L8c61647c: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61647e; /* mov.l r10,@-r15 */
L8c61647e: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c616480; /* mov.l r9,@-r15 */
L8c616480: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c616482; /* fmov.s fr15,@-r15 */
L8c616482: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c616484; /* fmov.s fr14,@-r15 */
L8c616484: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c616486; /* fmov.s fr13,@-r15 */
L8c616486: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c616488; /* fmov.s fr12,@-r15 */
L8c616488: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61648a; /* sts.l PR,@-r15 */
L8c61648a: r[15]+=4294967220u;goto L8c61648c; /* add #-0x4c,r15 */
L8c61648c: r[3]=rd(m,bus,&io_status,0x8c6164c8u,4);if(io_status)return io_status;goto L8c61648e; /* mov.l 0x8c6164c8,r3 */
L8c61648e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c616490; /* fldi1 fr12 */
L8c616490: target=r[3];*pr=0x8c616494u;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616494; /* jsr @r3 */
L8c616494: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616496; /* mov.l @(0x8,r15),r3 */
L8c616496: r[2]=0u;goto L8c616498; /* mov #0x0,r2 */
L8c616498: r[14]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61649a; /* mov.l @(0x8,r15),r14 */
L8c61649a: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61649c; /* mov.w @(0x2,r3),r0 */
L8c61649c: r[3]=rd(m,bus,&io_status,0x8c6164ccu,4);if(io_status)return io_status;goto L8c61649e; /* mov.l 0x8c6164cc,r3 */
L8c61649e: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6164a0; /* mov.l @(0x4,r14),r14 */
L8c6164a0: r[0]=r[0]&65535u;goto L8c6164a2; /* extu.w r0,r0 */
L8c6164a2: wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c6164a4; /* mov.l r0,@(0x18,r15) */
L8c6164a4: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6164a6; /* mov.l @r3,r13 */
L8c6164a6: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6164a8; /* mov.l r2,@(0x8,r15) */
L8c6164a8: goto L8c6164d0; /* bra 0x8c6164d0 */
L8c6164d0: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6164d2; /* mov.w @r14+,r3 */
L8c6164d2: r[4]=rd(m,bus,&io_status,0x8c6165ecu,4);if(io_status)return io_status;goto L8c6164d4; /* mov.l 0x8c6165ec,r4 */
L8c6164d4: r[3]=r[3]&65535u;goto L8c6164d6; /* extu.w r3,r3 */
L8c6164d6: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6164d8; /* tst r4,r3 */
L8c6164d8: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6164da; /* mov.l r3,@(0xc,r15) */
L8c6164da: take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c6164ec;goto L8c6164de; /* bt/s 0x8c6164ec */
L8c6164de: r[4]=sx(m,bus,&io_status,0x8c6165e8u,2);if(io_status)return io_status;goto L8c6164e0; /* mov.w 0x8c6165e8,r4 */
L8c6164e0: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6164e2; /* mov.l @(0xc,r15),r3 */
L8c6164e2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6164e4; /* fldi1 fr4 */
L8c6164e4: r[3]&=r[4];goto L8c6164e6; /* and r4,r3 */
L8c6164e6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6164e8; /* fneg fr4 */
L8c6164e8: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6164ea; /* mov.l r3,@(0xc,r15) */
L8c6164ea: fcopy(f,15u,4u);goto L8c6164ec; /* fmov fr4,fr15 */
L8c6164ec: r[0]=16u;goto L8c6164ee; /* mov #0x10,r0 */
L8c6164ee: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6164f0; /* mov.l @(0xc,r15),r3 */
L8c6164f0: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6164f2; /* mov.l @(0x8,r15),r2 */
L8c6164f2: r[9]=r[8];goto L8c6164f4; /* mov r8,r9 */
L8c6164f4: r[2]+=r[3];goto L8c6164f6; /* add r3,r2 */
L8c6164f6: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6164f8; /* mov.l r2,@(0x8,r15) */
L8c6164f8: r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6164fa; /* mov.l @(0xc,r15),r1 */
L8c6164fa: r[1]+=4294967294u;goto L8c6164fc; /* add #-0x2,r1 */
L8c6164fc: wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c6164fe; /* mov.l r1,@(0xc,r15) */
L8c6164fe: r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616500; /* mov.w @r14+,r10 */
L8c616500: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616502; /* mov.w @r14+,r11 */
L8c616502: r[10]=r[10]&65535u;goto L8c616504; /* extu.w r10,r10 */
L8c616504: r[10]<<=2;goto L8c616506; /* shll2 r10 */
L8c616506: r[11]=r[11]&65535u;goto L8c616508; /* extu.w r11,r11 */
L8c616508: r[11]<<=2;goto L8c61650a; /* shll2 r11 */
L8c61650a: r[10]<<=2;goto L8c61650c; /* shll2 r10 */
L8c61650c: r[11]<<=2;goto L8c61650e; /* shll2 r11 */
L8c61650e: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c616510; /* shll r10 */
L8c616510: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c616512; /* shll r11 */
L8c616512: r[11]+=r[13];goto L8c616514; /* add r13,r11 */
L8c616514: r[10]+=r[13];goto L8c616516; /* add r13,r10 */
L8c616516: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c616518; /* fmov.s @(r0,r11),fr13 */
L8c616518: fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c61651a; /* fmov.s @(r0,r10),fr6 */
L8c61651a: r[0]=20u;goto L8c61651c; /* mov #0x14,r0 */
L8c61651c: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c61651e; /* fmov.s @(r0,r11),fr14 */
L8c61651e: fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c616520; /* fmov.s @(r0,r10),fr5 */
L8c616520: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616522; /* mov.w @r14+,r12 */
L8c616522: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c616524; /* mov.w @r14,r3 */
L8c616524: r[12]=r[12]&65535u;goto L8c616526; /* extu.w r12,r12 */
L8c616526: r[12]<<=2;goto L8c616528; /* shll2 r12 */
L8c616528: r[3]=r[3]&65535u;goto L8c61652a; /* extu.w r3,r3 */
L8c61652a: r[3]<<=2;goto L8c61652c; /* shll2 r3 */
L8c61652c: r[12]<<=2;goto L8c61652e; /* shll2 r12 */
L8c61652e: r[3]<<=2;goto L8c616530; /* shll2 r3 */
L8c616530: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c616532; /* shll r12 */
L8c616532: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c616534; /* shll r3 */
L8c616534: r[3]+=r[13];goto L8c616536; /* add r13,r3 */
L8c616536: r[12]+=r[13];goto L8c616538; /* add r13,r12 */
L8c616538: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c61653a; /* pref @r3 */
L8c61653a: r[2]=0u;goto L8c61653c; /* mov #0x0,r2 */
L8c61653c: wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c61653e; /* mov.l r2,@(0x14,r15) */
L8c61653e: r[4]=63u;goto L8c616540; /* mov #0x3f,r4 */
L8c616540: r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616542; /* mov.l @(0xc,r10),r2 */
L8c616542: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616544; /* mov.l @(0xc,r11),r3 */
L8c616544: r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c616546; /* mov.l @(0xc,r12),r1 */
L8c616546: r[2]&=r[3];goto L8c616548; /* and r3,r2 */
L8c616548: r[2]&=r[1];goto L8c61654a; /* and r1,r2 */
L8c61654a: r[4]&=r[2];goto L8c61654c; /* and r2,r4 */
L8c61654c: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c61654e; /* tst r4,r4 */
L8c61654e: take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c616556;goto L8c616552; /* bt/s 0x8c616556 */
L8c616552: goto L8c6166b0; /* bra 0x8c6166b0 */
L8c616556: r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616558; /* mov.l @(0xc,r10),r4 */
L8c616558: r[5]=1u;goto L8c61655a; /* mov #0x1,r5 */
L8c61655a: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61655c; /* mov.l @(0xc,r11),r3 */
L8c61655c: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61655e; /* mov.l @(0xc,r12),r2 */
L8c61655e: r[4]|=r[3];goto L8c616560; /* or r3,r4 */
L8c616560: r[4]|=r[2];goto L8c616562; /* or r2,r4 */
L8c616562: r[4]&=r[5];goto L8c616564; /* and r5,r4 */
L8c616564: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c616566; /* tst r4,r4 */
L8c616566: if(!(*sr&1u))goto L8c61656c;goto L8c616568; /* bf 0x8c61656c */
L8c616568: goto L8c616692; /* bra 0x8c616692 */
L8c61656c: r[7]=r[15];goto L8c61656e; /* mov r15,r7 */
L8c61656e: r[7]+=64u;goto L8c616570; /* add #0x40,r7 */
L8c616570: wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c616572; /* mov.l r10,@r7 */
L8c616572: wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c616574; /* mov.l r11,@(0x4,r7) */
L8c616574: wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c616576; /* mov.l r12,@(0x8,r7) */
L8c616576: r[6]=0u;goto L8c616624; /* bra 0x8c616624 */
L8c61657a: r[2]=r[6];goto L8c61657c; /* mov r6,r2 */
L8c61657c: r[1]=r[6];goto L8c61657e; /* mov r6,r1 */
L8c61657e: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c616580; /* shll r2 */
L8c616580: r[3]=r[6];goto L8c616582; /* mov r6,r3 */
L8c616582: r[3]<<=2;goto L8c616584; /* shll2 r3 */
L8c616584: r[2]+=r[1];goto L8c616586; /* add r1,r2 */
L8c616586: r[2]<<=2;goto L8c616588; /* shll2 r2 */
L8c616588: r[3]+=r[7];goto L8c61658a; /* add r7,r3 */
L8c61658a: r[2]+=r[4];goto L8c61658c; /* add r4,r2 */
L8c61658c: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61658e; /* mov.l r3,@(0x4,r15) */
L8c61658e: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c616590; /* mov.l r2,@r15 */
L8c616590: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616592; /* mov.l @r3,r1 */
L8c616592: r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c616594; /* mov.l @(0xc,r1),r2 */
L8c616594: *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c616596; /* tst r5,r2 */
L8c616596: take=!(*sr&1u);r[6]+=1u;if(take)goto L8c6165fc;goto L8c61659a; /* bf/s 0x8c6165fc */
L8c61659a: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61659c; /* mov.l @(0x4,r15),r1 */
L8c61659c: r[0]=24u;goto L8c61659e; /* mov #0x18,r0 */
L8c61659e: fcopy(f,4u,12u);goto L8c6165a0; /* fmov fr12,fr4 */
L8c6165a0: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6165a2; /* mov.l @r1,r3 */
L8c6165a2: r[4]=rd(m,bus,&io_status,0x8c6165f0u,4);if(io_status)return io_status;goto L8c6165a4; /* mov.l 0x8c6165f0,r4 */
L8c6165a4: wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6165a6; /* mov.l r3,@(0x10,r15) */
L8c6165a6: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6165a8; /* fmov.s @(r0,r3),fr3 */
L8c6165a8: r[0]=12u;goto L8c6165aa; /* mov #0xc,r0 */
L8c6165aa: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6165ac; /* mov.l @(0x10,r15),r2 */
L8c6165ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6165ae; /* fdiv fr3,fr4 */
L8c6165ae: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6165b0; /* fmov.s @(r0,r4),fr3 */
L8c6165b0: r[0]=16u;goto L8c6165b2; /* mov #0x10,r0 */
L8c6165b2: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6165b4; /* mov.l @r15,r3 */
L8c6165b4: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6165b6; /* fmov.s @(r0,r2),fr2 */
L8c6165b6: r[0]=16u;goto L8c6165b8; /* mov #0x10,r0 */
L8c6165b8: r[2]=rd(m,bus,&io_status,0x8c6165f4u,4);if(io_status)return io_status;goto L8c6165ba; /* mov.l 0x8c6165f4,r2 */
L8c6165ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6165bc; /* fsub fr3,fr2 */
L8c6165bc: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6165be; /* fmov.s @r2,fr3 */
L8c6165be: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6165c0; /* fmul fr4,fr2 */
L8c6165c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6165c2; /* fdiv fr3,fr2 */
L8c6165c2: fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6165c4; /* fmov.s fr2,@r3 */
L8c6165c4: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6165c6; /* mov.l @(0x4,r15),r1 */
L8c6165c6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6165c8; /* fmov.s @(r0,r4),fr3 */
L8c6165c8: r[0]=20u;goto L8c6165ca; /* mov #0x14,r0 */
L8c6165ca: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6165cc; /* mov.l @r1,r1 */
L8c6165cc: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6165ce; /* mov.l @r15,r3 */
L8c6165ce: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c6165d0; /* fmov.s @(r0,r1),fr2 */
L8c6165d0: r[1]=rd(m,bus,&io_status,0x8c6165f8u,4);if(io_status)return io_status;goto L8c6165d2; /* mov.l 0x8c6165f8,r1 */
L8c6165d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6165d4; /* fsub fr3,fr2 */
L8c6165d4: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6165d6; /* fmov.s @r1,fr3 */
L8c6165d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6165d8; /* fmul fr4,fr2 */
L8c6165d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6165da; /* fdiv fr3,fr2 */
L8c6165da: r[0]=4u;goto L8c6165dc; /* mov #0x4,r0 */
L8c6165dc: fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c6165de; /* fmov fr2,@(r0,r3) */
L8c6165de: r[0]=8u;goto L8c6165e0; /* mov #0x8,r0 */
L8c6165e0: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6165e2; /* mov.l @r15,r3 */
L8c6165e2: fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c6165e4; /* fmov fr4,@(r0,r3) */
L8c6165e4: goto L8c616624; /* bra 0x8c616624 */
L8c6165fc: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6165fe; /* mov.l @(0x4,r15),r3 */
L8c6165fe: r[0]=16u;goto L8c616600; /* mov #0x10,r0 */
L8c616600: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616602; /* mov.l @r15,r2 */
L8c616602: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616604; /* mov.l @r3,r1 */
L8c616604: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616606; /* fmov.s @(r0,r1),fr3 */
L8c616606: r[0]=20u;goto L8c616608; /* mov #0x14,r0 */
L8c616608: fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61660a; /* fmov.s fr3,@r2 */
L8c61660a: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61660c; /* mov.l @(0x4,r15),r2 */
L8c61660c: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61660e; /* mov.l @r15,r3 */
L8c61660e: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c616610; /* mov.l @r2,r1 */
L8c616610: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616612; /* fmov.s @(r0,r1),fr3 */
L8c616612: r[0]=4u;goto L8c616614; /* mov #0x4,r0 */
L8c616614: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616616; /* fmov fr3,@(r0,r3) */
L8c616616: r[0]=24u;goto L8c616618; /* mov #0x18,r0 */
L8c616618: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61661a; /* mov.l @(0x4,r15),r2 */
L8c61661a: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61661c; /* mov.l @r15,r3 */
L8c61661c: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61661e; /* mov.l @r2,r1 */
L8c61661e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616620; /* fmov.s @(r0,r1),fr3 */
L8c616620: r[0]=8u;goto L8c616622; /* mov #0x8,r0 */
L8c616622: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616624; /* fmov fr3,@(r0,r3) */
L8c616624: r[3]=3u;goto L8c616626; /* mov #0x3,r3 */
L8c616626: r[4]=r[15];goto L8c616628; /* mov r15,r4 */
L8c616628: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c61662a; /* cmp/ge r3,r6 */
L8c61662a: take=!(*sr&1u);r[4]+=28u;if(take)goto L8c61657a;goto L8c61662e; /* bf/s 0x8c61657a */
L8c61662e: r[7]=r[4];goto L8c616630; /* mov r4,r7 */
L8c616630: r[0]=8u;goto L8c616632; /* mov #0x8,r0 */
L8c616632: r[3]=r[4];goto L8c616634; /* mov r4,r3 */
L8c616634: fload(m,bus,&io_status,f,6u,r[0]+r[7]);if(io_status)return io_status;goto L8c616636; /* fmov.s @(r0,r7),fr6 */
L8c616636: r[3]+=12u;goto L8c616638; /* add #0xc,r3 */
L8c616638: r[6]=r[4];goto L8c61663a; /* mov r4,r6 */
L8c61663a: r[0]=4u;goto L8c61663c; /* mov #0x4,r0 */
L8c61663c: fcopy(f,0u,6u);goto L8c61663e; /* fmov fr6,fr0 */
L8c61663e: r[6]+=24u;goto L8c616640; /* add #0x18,r6 */
L8c616640: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c616642; /* mov.l r3,@r15 */
L8c616642: fload(m,bus,&io_status,f,5u,r[0]+r[7]);if(io_status)return io_status;goto L8c616644; /* fmov.s @(r0,r7),fr5 */
L8c616644: fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c616646; /* fmov.s @(r0,r3),fr7 */
L8c616646: r[0]=8u;goto L8c616648; /* mov #0x8,r0 */
L8c616648: fload(m,bus,&io_status,f,8u,r[0]+r[3]);if(io_status)return io_status;goto L8c61664a; /* fmov.s @(r0,r3),fr8 */
L8c61664a: r[0]=4u;goto L8c61664c; /* mov #0x4,r0 */
L8c61664c: fcopy(f,3u,5u);goto L8c61664e; /* fmov fr5,fr3 */
L8c61664e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[7],&f->fpscr,&fr[0]);if(status)return status;}goto L8c616650; /* fmul fr7,fr0 */
L8c616650: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616652; /* fmul fr8,fr3 */
L8c616652: fload(m,bus,&io_status,f,4u,r[7]);if(io_status)return io_status;goto L8c616654; /* fmov.s @r7,fr4 */
L8c616654: fload(m,bus,&io_status,f,9u,r[3]);if(io_status)return io_status;goto L8c616656; /* fmov.s @r3,fr9 */
L8c616656: fcopy(f,2u,4u);goto L8c616658; /* fmov fr4,fr2 */
L8c616658: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61665a; /* fmul fr8,fr2 */
L8c61665a: fcopy(f,1u,5u);goto L8c61665c; /* fmov fr5,fr1 */
L8c61665c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c61665e; /* fsub fr3,fr0 */
L8c61665e: fcopy(f,3u,6u);goto L8c616660; /* fmov fr6,fr3 */
L8c616660: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616662; /* fmul fr9,fr3 */
L8c616662: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616664; /* fsub fr3,fr2 */
L8c616664: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c616666; /* fmov.s @(r0,r6),fr3 */
L8c616666: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616668; /* fmul fr2,fr3 */
L8c616668: fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c61666a; /* fmov.s @r6,fr2 */
L8c61666a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61666c; /* fmac fr0,fr2,fr3 */
L8c61666c: fcopy(f,2u,3u);goto L8c61666e; /* fmov fr3,fr2 */
L8c61666e: fcopy(f,3u,4u);goto L8c616670; /* fmov fr4,fr3 */
L8c616670: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616672; /* fmul fr7,fr3 */
L8c616672: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616674; /* fmul fr9,fr1 */
L8c616674: r[0]=8u;goto L8c616676; /* mov #0x8,r0 */
L8c616676: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616678; /* fsub fr3,fr1 */
L8c616678: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61667a; /* fmov.s @(r0,r6),fr3 */
L8c61667a: fcopy(f,0u,1u);goto L8c61667c; /* fmov fr1,fr0 */
L8c61667c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61667e; /* fmac fr0,fr3,fr2 */
L8c61667e: fcopy(f,3u,15u);goto L8c616680; /* fmov fr15,fr3 */
L8c616680: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c616682; /* fneg fr3 */
L8c616682: fcopy(f,4u,2u);goto L8c616684; /* fmov fr2,fr4 */
L8c616684: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616686; /* fmul fr3,fr4 */
L8c616686: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c616688; /* fldi0 fr2 */
L8c616688: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c61668a; /* fcmp/gt fr2,fr4 */
L8c61668a: if((*sr&1u))goto L8c6166b0;goto L8c61668c; /* bt 0x8c6166b0 */
L8c61668c: wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c61668e; /* mov.l r5,@(0x14,r15) */
L8c61668e: fcopy(f,4u,12u);goto L8c6166b0; /* bra 0x8c6166b0 */
L8c616692: r[0]=20u;goto L8c616694; /* mov #0x14,r0 */
L8c616694: fcopy(f,3u,13u);goto L8c616696; /* fmov fr13,fr3 */
L8c616696: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c616698; /* fmov.s @(r0,r12),fr2 */
L8c616698: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61669a; /* fsub fr6,fr3 */
L8c61669a: r[0]=16u;goto L8c61669c; /* mov #0x10,r0 */
L8c61669c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61669e; /* fsub fr14,fr2 */
L8c61669e: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c6166a0; /* fmov.s @(r0,r12),fr1 */
L8c6166a0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6166a2; /* fsub fr13,fr1 */
L8c6166a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6166a4; /* fmul fr2,fr3 */
L8c6166a4: fcopy(f,2u,14u);goto L8c6166a6; /* fmov fr14,fr2 */
L8c6166a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6166a8; /* fsub fr5,fr2 */
L8c6166a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6166aa; /* fmul fr1,fr2 */
L8c6166aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6166ac; /* fsub fr2,fr3 */
L8c6166ac: fcopy(f,4u,3u);goto L8c6166ae; /* fmov fr3,fr4 */
L8c6166ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6166b0; /* fmul fr15,fr4 */
L8c6166b0: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6166b2; /* cmp/hs r8,r9 */
L8c6166b2: if((*sr&1u))goto L8c6166e8;goto L8c6166b4; /* bt 0x8c6166e8 */
L8c6166b4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6166b6; /* fldi0 fr3 */
L8c6166b6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6166b8; /* fcmp/gt fr4,fr3 */
L8c6166b8: if((*sr&1u))goto L8c616718;goto L8c6166ba; /* bt 0x8c616718 */
L8c6166ba: r[5]=rd(m,bus,&io_status,0x8c61687cu,4);if(io_status)return io_status;goto L8c6166bc; /* mov.l 0x8c61687c,r5 */
L8c6166bc: r[4]=r[11];goto L8c6166be; /* mov r11,r4 */
L8c6166be: r[8]+=16u;goto L8c6166c0; /* add #0x10,r8 */
L8c6166c0: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6166c2; /* fschg */
L8c6166c2: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6166c4; /* fmov.s @r4+,fr0 */
L8c6166c4: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6166c6; /* fmov.s @r4+,fr2 */
L8c6166c6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6166c8; /* fldi1 fr3 */
L8c6166c8: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6166ca; /* ftrv xmtrx,fv0 */
L8c6166ca: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6166cc; /* fmov.s @r4+,fr4 */
L8c6166cc: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6166ce; /* fmov.s @r4,fr6 */
L8c6166ce: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6166d0; /* fschg */
L8c6166d0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6166d2; /* fmov.s fr6,@-r8 */
L8c6166d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6166d4; /* fmov.s fr5,@-r8 */
L8c6166d4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6166d6; /* fmov.s fr4,@-r8 */
L8c6166d6: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6166d8; /* mov.l r5,@-r8 */
L8c6166d8: r[8]+=28u;goto L8c6166da; /* add #0x1c,r8 */
L8c6166da: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6166dc; /* fmov.s fr2,@-r8 */
L8c6166dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6166de; /* fmov.s fr1,@-r8 */
L8c6166de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6166e0; /* fmov.s fr0,@-r8 */
L8c6166e0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6166e2; /* pref @r8 */
L8c6166e2: r[8]+=16u;goto L8c6166e4; /* add #0x10,r8 */
L8c6166e4: r[9]=r[8];goto L8c616742; /* bra 0x8c616742 */
L8c6166e8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6166ea; /* fldi0 fr3 */
L8c6166ea: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6166ec; /* fcmp/gt fr4,fr3 */
L8c6166ec: if(!(*sr&1u))goto L8c616742;goto L8c6166ee; /* bf 0x8c616742 */
L8c6166ee: r[5]=rd(m,bus,&io_status,0x8c616880u,4);if(io_status)return io_status;goto L8c6166f0; /* mov.l 0x8c616880,r5 */
L8c6166f0: r[4]=r[10];goto L8c6166f2; /* mov r10,r4 */
L8c6166f2: r[8]+=16u;goto L8c6166f4; /* add #0x10,r8 */
L8c6166f4: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6166f6; /* fschg */
L8c6166f6: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6166f8; /* fmov.s @r4+,fr0 */
L8c6166f8: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6166fa; /* fmov.s @r4+,fr2 */
L8c6166fa: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6166fc; /* fldi1 fr3 */
L8c6166fc: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6166fe; /* ftrv xmtrx,fv0 */
L8c6166fe: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616700; /* fmov.s @r4+,fr4 */
L8c616700: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616702; /* fmov.s @r4,fr6 */
L8c616702: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616704; /* fschg */
L8c616704: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616706; /* fmov.s fr6,@-r8 */
L8c616706: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616708; /* fmov.s fr5,@-r8 */
L8c616708: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61670a; /* fmov.s fr4,@-r8 */
L8c61670a: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61670c; /* mov.l r5,@-r8 */
L8c61670c: r[8]+=28u;goto L8c61670e; /* add #0x1c,r8 */
L8c61670e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616710; /* fmov.s fr2,@-r8 */
L8c616710: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616712; /* fmov.s fr1,@-r8 */
L8c616712: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616714; /* fmov.s fr0,@-r8 */
L8c616714: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616716; /* pref @r8 */
L8c616716: r[8]+=16u;goto L8c616718; /* add #0x10,r8 */
L8c616718: r[5]=rd(m,bus,&io_status,0x8c616880u,4);if(io_status)return io_status;goto L8c61671a; /* mov.l 0x8c616880,r5 */
L8c61671a: r[4]=r[11];goto L8c61671c; /* mov r11,r4 */
L8c61671c: r[8]+=16u;goto L8c61671e; /* add #0x10,r8 */
L8c61671e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616720; /* fschg */
L8c616720: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616722; /* fmov.s @r4+,fr0 */
L8c616722: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616724; /* fmov.s @r4+,fr2 */
L8c616724: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616726; /* fldi1 fr3 */
L8c616726: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c616728; /* ftrv xmtrx,fv0 */
L8c616728: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61672a; /* fmov.s @r4+,fr4 */
L8c61672a: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61672c; /* fmov.s @r4,fr6 */
L8c61672c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61672e; /* fschg */
L8c61672e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616730; /* fmov.s fr6,@-r8 */
L8c616730: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616732; /* fmov.s fr5,@-r8 */
L8c616732: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616734; /* fmov.s fr4,@-r8 */
L8c616734: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616736; /* mov.l r5,@-r8 */
L8c616736: r[8]+=28u;goto L8c616738; /* add #0x1c,r8 */
L8c616738: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61673a; /* fmov.s fr2,@-r8 */
L8c61673a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61673c; /* fmov.s fr1,@-r8 */
L8c61673c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c61673e; /* fmov.s fr0,@-r8 */
L8c61673e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616740; /* pref @r8 */
L8c616740: r[8]+=16u;goto L8c616742; /* add #0x10,r8 */
L8c616742: r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c616744; /* mov.l @(0x14,r15),r2 */
L8c616744: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c616746; /* tst r2,r2 */
L8c616746: if((*sr&1u))goto L8c6167ba;goto L8c616748; /* bt 0x8c6167ba */
L8c616748: r[0]=64u;goto L8c61674a; /* mov #0x40,r0 */
L8c61674a: r[4]=rd(m,bus,&io_status,0x8c616884u,4);if(io_status)return io_status;goto L8c61674c; /* mov.l 0x8c616884,r4 */
L8c61674c: r[3]=0u;goto L8c61674e; /* mov #0x0,r3 */
L8c61674e: r[9]=rd(m,bus,&io_status,0x8c616888u,4);if(io_status)return io_status;goto L8c616750; /* mov.l 0x8c616888,r9 */
L8c616750: wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c616752; /* mov.l r3,@r4 */
L8c616752: r[4]=r[15];goto L8c616754; /* mov r15,r4 */
L8c616754: fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c616756; /* fmov.s @r11,fr3 */
L8c616756: fload(m,bus,&io_status,f,2u,r[10]);if(io_status)return io_status;goto L8c616758; /* fmov.s @r10,fr2 */
L8c616758: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61675a; /* fadd fr3,fr2 */
L8c61675a: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61675c; /* fmov.s @r12,fr3 */
L8c61675c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61675e; /* fadd fr3,fr2 */
L8c61675e: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c616760; /* fmov fr2,@(r0,r15) */
L8c616760: r[0]=4u;goto L8c616762; /* mov #0x4,r0 */
L8c616762: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c616764; /* fmov.s @(r0,r11),fr3 */
L8c616764: fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c616766; /* fmov.s @(r0,r10),fr2 */
L8c616766: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c616768; /* fadd fr3,fr2 */
L8c616768: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61676a; /* fmov.s @(r0,r12),fr3 */
L8c61676a: r[0]=68u;goto L8c61676c; /* mov #0x44,r0 */
L8c61676c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61676e; /* fadd fr3,fr2 */
L8c61676e: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c616770; /* fmov fr2,@(r0,r15) */
L8c616770: r[0]=8u;goto L8c616772; /* mov #0x8,r0 */
L8c616772: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c616774; /* fmov.s @(r0,r11),fr3 */
L8c616774: fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c616776; /* fmov.s @(r0,r10),fr2 */
L8c616776: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c616778; /* fadd fr3,fr2 */
L8c616778: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61677a; /* fmov.s @(r0,r12),fr3 */
L8c61677a: r[0]=72u;goto L8c61677c; /* mov #0x48,r0 */
L8c61677c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61677e; /* fadd fr3,fr2 */
L8c61677e: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c616780; /* fmov fr2,@(r0,r15) */
L8c616780: target=r[9];*pr=0x8c616784u;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616784; /* jsr @r9 */
L8c616784: r[4]=rd(m,bus,&io_status,0x8c616890u,4);if(io_status)return io_status;goto L8c616786; /* mov.l 0x8c616890,r4 */
L8c616786: r[0]=64u;goto L8c616788; /* mov #0x40,r0 */
L8c616788: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61678a; /* fmov.s @(r0,r15),fr3 */
L8c61678a: r[0]=68u;goto L8c61678c; /* mov #0x44,r0 */
L8c61678c: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c61678e; /* fmov.s @r4,fr2 */
L8c61678e: r[6]=r[12];goto L8c616790; /* mov r12,r6 */
L8c616790: r[5]=rd(m,bus,&io_status,0x8c61688cu,4);if(io_status)return io_status;goto L8c616792; /* mov.l 0x8c61688c,r5 */
L8c616792: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616794; /* fmul fr3,fr2 */
L8c616794: r[3]=rd(m,bus,&io_status,0x8c616894u,4);if(io_status)return io_status;goto L8c616796; /* mov.l 0x8c616894,r3 */
L8c616796: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c616798; /* fmov.s @r5,fr3 */
L8c616798: r[5]=r[11];goto L8c61679a; /* mov r11,r5 */
L8c61679a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61679c; /* fsub fr2,fr3 */
L8c61679c: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61679e; /* fmov.s @(r0,r15),fr2 */
L8c61679e: r[0]=4u;goto L8c6167a0; /* mov #0x4,r0 */
L8c6167a0: fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c6167a2; /* fmov.s @(r0,r4),fr1 */
L8c6167a2: r[0]=72u;goto L8c6167a4; /* mov #0x48,r0 */
L8c6167a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6167a6; /* fmul fr2,fr1 */
L8c6167a6: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c6167a8; /* fmov.s @(r0,r15),fr2 */
L8c6167a8: r[0]=8u;goto L8c6167aa; /* mov #0x8,r0 */
L8c6167aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6167ac; /* fsub fr1,fr3 */
L8c6167ac: fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c6167ae; /* fmov.s @(r0,r4),fr1 */
L8c6167ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6167b0; /* fmul fr2,fr1 */
L8c6167b0: fcopy(f,4u,3u);goto L8c6167b2; /* fmov fr3,fr4 */
L8c6167b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6167b4; /* fsub fr1,fr4 */
L8c6167b4: target=r[3];*pr=0x8c6167b8u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6167b8; /* jsr @r3 */
L8c6167b8: r[9]=r[8];goto L8c6167ba; /* mov r8,r9 */
L8c6167ba: fcopy(f,6u,13u);goto L8c6167bc; /* fmov fr13,fr6 */
L8c6167bc: r[0]=16u;goto L8c6167be; /* mov #0x10,r0 */
L8c6167be: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6167c0; /* mov.l @(0xc,r15),r3 */
L8c6167c0: r[10]=r[11];goto L8c6167c2; /* mov r11,r10 */
L8c6167c2: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c6167c4; /* fmov.s @(r0,r12),fr13 */
L8c6167c4: r[0]=20u;goto L8c6167c6; /* mov #0x14,r0 */
L8c6167c6: fcopy(f,5u,14u);goto L8c6167c8; /* fmov fr14,fr5 */
L8c6167c8: r[3]+=4294967295u;goto L8c6167ca; /* add #-0x1,r3 */
L8c6167ca: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c6167cc; /* fmov.s @(r0,r12),fr14 */
L8c6167cc: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6167ce; /* tst r3,r3 */
L8c6167ce: r[11]=r[12];goto L8c6167d0; /* mov r12,r11 */
L8c6167d0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c6167d2; /* fneg fr15 */
L8c6167d2: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c6167da;goto L8c6167d6; /* bt/s 0x8c6167da */
L8c6167d6: goto L8c616520; /* bra 0x8c616520 */
L8c6167da: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6167dc; /* cmp/hs r8,r9 */
L8c6167dc: if((*sr&1u))goto L8c616808;goto L8c6167de; /* bt 0x8c616808 */
L8c6167de: r[5]=rd(m,bus,&io_status,0x8c61687cu,4);if(io_status)return io_status;goto L8c6167e0; /* mov.l 0x8c61687c,r5 */
L8c6167e0: r[4]=r[12];goto L8c6167e2; /* mov r12,r4 */
L8c6167e2: r[8]+=16u;goto L8c6167e4; /* add #0x10,r8 */
L8c6167e4: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6167e6; /* fschg */
L8c6167e6: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6167e8; /* fmov.s @r4+,fr0 */
L8c6167e8: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6167ea; /* fmov.s @r4+,fr2 */
L8c6167ea: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6167ec; /* fldi1 fr3 */
L8c6167ec: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6167ee; /* ftrv xmtrx,fv0 */
L8c6167ee: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6167f0; /* fmov.s @r4+,fr4 */
L8c6167f0: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6167f2; /* fmov.s @r4,fr6 */
L8c6167f2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6167f4; /* fschg */
L8c6167f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6167f6; /* fmov.s fr6,@-r8 */
L8c6167f6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6167f8; /* fmov.s fr5,@-r8 */
L8c6167f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6167fa; /* fmov.s fr4,@-r8 */
L8c6167fa: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6167fc; /* mov.l r5,@-r8 */
L8c6167fc: r[8]+=28u;goto L8c6167fe; /* add #0x1c,r8 */
L8c6167fe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616800; /* fmov.s fr2,@-r8 */
L8c616800: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616802; /* fmov.s fr1,@-r8 */
L8c616802: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616804; /* fmov.s fr0,@-r8 */
L8c616804: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616806; /* pref @r8 */
L8c616806: r[8]+=16u;goto L8c616808; /* add #0x10,r8 */
L8c616808: r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c61680a; /* mov.l @(0x18,r15),r3 */
L8c61680a: r[3]+=4294967295u;goto L8c61680c; /* add #-0x1,r3 */
L8c61680c: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61680e; /* tst r3,r3 */
L8c61680e: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c616816;goto L8c616812; /* bt/s 0x8c616816 */
L8c616812: goto L8c6164d0; /* bra 0x8c6164d0 */
L8c616816: r[4]=rd(m,bus,&io_status,0x8c616898u,4);if(io_status)return io_status;goto L8c616818; /* mov.l 0x8c616898,r4 */
L8c616818: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61681a; /* mov.l @(0x8,r15),r3 */
L8c61681a: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61681c; /* mov.l @r4,r2 */
L8c61681c: r[2]+=r[3];goto L8c61681e; /* add r3,r2 */
L8c61681e: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616820; /* mov.l r2,@r4 */
L8c616820: r[4]=rd(m,bus,&io_status,0x8c61689cu,4);if(io_status)return io_status;goto L8c616822; /* mov.l 0x8c61689c,r4 */
L8c616822: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616824; /* mov.l @(0x8,r15),r3 */
L8c616824: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616826; /* mov.l @r4,r2 */
L8c616826: r[2]+=r[3];goto L8c616828; /* add r3,r2 */
L8c616828: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61682a; /* mov.l r2,@r4 */
L8c61682a: r[15]+=76u;goto L8c61682c; /* add #0x4c,r15 */
L8c61682c: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61682e; /* lds.l @r15+,PR */
L8c61682e: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616830; /* fmov.s @r15+,fr12 */
L8c616830: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616832; /* fmov.s @r15+,fr13 */
L8c616832: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616834; /* fmov.s @r15+,fr14 */
L8c616834: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616836; /* fmov.s @r15+,fr15 */
L8c616836: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616838; /* mov.l @r15+,r9 */
L8c616838: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61683a; /* mov.l @r15+,r10 */
L8c61683a: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61683c; /* mov.l @r15+,r11 */
L8c61683c: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61683e; /* mov.l @r15+,r12 */
L8c61683e: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616840; /* mov.l @r15+,r13 */
L8c616840: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
