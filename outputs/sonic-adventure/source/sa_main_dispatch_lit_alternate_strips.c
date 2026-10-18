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
L8c6136c0: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6136c2; /* mov.l r14,@-r15 */
L8c6136c2: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6136c4; /* mov.l r13,@-r15 */
L8c6136c4: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6136c6; /* mov.l r12,@-r15 */
L8c6136c6: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6136c8; /* mov.l r11,@-r15 */
L8c6136c8: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6136ca; /* mov.l r10,@-r15 */
L8c6136ca: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6136cc; /* mov.l r9,@-r15 */
L8c6136cc: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6136ce; /* fmov.s fr15,@-r15 */
L8c6136ce: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6136d0; /* fmov.s fr14,@-r15 */
L8c6136d0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6136d2; /* fmov.s fr13,@-r15 */
L8c6136d2: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6136d4; /* sts.l PR,@-r15 */
L8c6136d4: r[15]+=4294967280u;goto L8c6136d6; /* add #-0x10,r15 */
L8c6136d6: r[3]=rd(m,bus,&io_status,0x8c613718u,4);if(io_status)return io_status;goto L8c6136d8; /* mov.l 0x8c613718,r3 */
L8c6136d8: r[9]=rd(m,bus,&io_status,0x8c613714u,4);if(io_status)return io_status;goto L8c6136da; /* mov.l 0x8c613714,r9 */
L8c6136da: target=r[3];*pr=0x8c6136deu;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6136de; /* jsr @r3 */
L8c6136de: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6136e0; /* mov.l @(0x4,r15),r3 */
L8c6136e0: r[2]=0u;goto L8c6136e2; /* mov #0x0,r2 */
L8c6136e2: r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6136e4; /* mov.l @(0x4,r15),r14 */
L8c6136e4: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6136e6; /* mov.w @(0x2,r3),r0 */
L8c6136e6: r[3]=rd(m,bus,&io_status,0x8c61371cu,4);if(io_status)return io_status;goto L8c6136e8; /* mov.l 0x8c61371c,r3 */
L8c6136e8: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6136ea; /* mov.l @(0x4,r14),r14 */
L8c6136ea: r[0]=r[0]&65535u;goto L8c6136ec; /* extu.w r0,r0 */
L8c6136ec: wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6136ee; /* mov.l r0,@(0xc,r15) */
L8c6136ee: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6136f0; /* mov.l @r3,r13 */
L8c6136f0: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6136f2; /* mov.l r2,@(0x8,r15) */
L8c6136f2: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6136f4; /* mov.l r2,@(0x4,r15) */
L8c6136f4: goto L8c613720; /* bra 0x8c613720 */
L8c613720: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613722; /* mov.w @r14+,r3 */
L8c613722: r[4]=rd(m,bus,&io_status,0x8c61380cu,4);if(io_status)return io_status;goto L8c613724; /* mov.l 0x8c61380c,r4 */
L8c613724: r[3]=r[3]&65535u;goto L8c613726; /* extu.w r3,r3 */
L8c613726: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613728; /* fldi1 fr4 */
L8c613728: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c61372a; /* tst r4,r3 */
L8c61372a: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61372c; /* mov.l r3,@r15 */
L8c61372c: take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c61373e;goto L8c613730; /* bt/s 0x8c61373e */
L8c613730: r[4]=sx(m,bus,&io_status,0x8c613808u,2);if(io_status)return io_status;goto L8c613732; /* mov.w 0x8c613808,r4 */
L8c613732: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613734; /* mov.l @r15,r3 */
L8c613734: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613736; /* fldi1 fr4 */
L8c613736: r[3]&=r[4];goto L8c613738; /* and r4,r3 */
L8c613738: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c61373a; /* fneg fr4 */
L8c61373a: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61373c; /* mov.l r3,@r15 */
L8c61373c: fcopy(f,15u,4u);goto L8c61373e; /* fmov fr4,fr15 */
L8c61373e: r[0]=16u;goto L8c613740; /* mov #0x10,r0 */
L8c613740: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613742; /* mov.l @r15,r3 */
L8c613742: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613744; /* mov.l @(0x4,r15),r2 */
L8c613744: r[10]=r[8];goto L8c613746; /* mov r8,r10 */
L8c613746: r[2]+=r[3];goto L8c613748; /* add r3,r2 */
L8c613748: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61374a; /* mov.l r2,@(0x4,r15) */
L8c61374a: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61374c; /* mov.l @r15,r1 */
L8c61374c: r[1]+=4294967294u;goto L8c61374e; /* add #-0x2,r1 */
L8c61374e: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c613750; /* mov.l r1,@r15 */
L8c613750: r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613752; /* mov.w @r14+,r4 */
L8c613752: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613754; /* mov.w @r14+,r11 */
L8c613754: r[4]=r[4]&65535u;goto L8c613756; /* extu.w r4,r4 */
L8c613756: r[4]<<=2;goto L8c613758; /* shll2 r4 */
L8c613758: r[11]=r[11]&65535u;goto L8c61375a; /* extu.w r11,r11 */
L8c61375a: r[11]<<=2;goto L8c61375c; /* shll2 r11 */
L8c61375c: r[4]<<=2;goto L8c61375e; /* shll2 r4 */
L8c61375e: r[11]<<=2;goto L8c613760; /* shll2 r11 */
L8c613760: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c613762; /* shll r4 */
L8c613762: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c613764; /* shll r11 */
L8c613764: r[11]+=r[13];goto L8c613766; /* add r13,r11 */
L8c613766: r[4]+=r[13];goto L8c613768; /* add r13,r4 */
L8c613768: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c61376a; /* fmov.s @(r0,r11),fr14 */
L8c61376a: fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c61376c; /* fmov.s @(r0,r4),fr4 */
L8c61376c: r[0]=20u;goto L8c61376e; /* mov #0x14,r0 */
L8c61376e: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c613770; /* fmov.s @(r0,r11),fr13 */
L8c613770: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c613772; /* fmov.s @(r0,r4),fr5 */
L8c613772: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613774; /* mov.w @r14+,r12 */
L8c613774: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c613776; /* mov.w @r14,r3 */
L8c613776: r[12]=r[12]&65535u;goto L8c613778; /* extu.w r12,r12 */
L8c613778: r[12]<<=2;goto L8c61377a; /* shll2 r12 */
L8c61377a: r[3]=r[3]&65535u;goto L8c61377c; /* extu.w r3,r3 */
L8c61377c: r[3]<<=2;goto L8c61377e; /* shll2 r3 */
L8c61377e: r[12]<<=2;goto L8c613780; /* shll2 r12 */
L8c613780: r[3]<<=2;goto L8c613782; /* shll2 r3 */
L8c613782: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c613784; /* shll r12 */
L8c613784: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c613786; /* shll r3 */
L8c613786: r[3]+=r[13];goto L8c613788; /* add r13,r3 */
L8c613788: r[12]+=r[13];goto L8c61378a; /* add r13,r12 */
L8c61378a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c61378c; /* pref @r3 */
L8c61378c: r[0]=20u;goto L8c61378e; /* mov #0x14,r0 */
L8c61378e: fcopy(f,3u,14u);goto L8c613790; /* fmov fr14,fr3 */
L8c613790: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c613792; /* fmov.s @(r0,r12),fr2 */
L8c613792: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613794; /* fsub fr4,fr3 */
L8c613794: r[0]=16u;goto L8c613796; /* mov #0x10,r0 */
L8c613796: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613798; /* cmp/hs r8,r10 */
L8c613798: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61379a; /* fsub fr13,fr2 */
L8c61379a: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c61379c; /* fmov.s @(r0,r12),fr1 */
L8c61379c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61379e; /* fsub fr14,fr1 */
L8c61379e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6137a0; /* fmul fr2,fr3 */
L8c6137a0: fcopy(f,2u,13u);goto L8c6137a2; /* fmov fr13,fr2 */
L8c6137a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6137a4; /* fsub fr5,fr2 */
L8c6137a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6137a6; /* fmul fr1,fr2 */
L8c6137a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6137a8; /* fsub fr2,fr3 */
L8c6137a8: fcopy(f,4u,3u);goto L8c6137aa; /* fmov fr3,fr4 */
L8c6137aa: take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c613810;goto L8c6137ae; /* bt/s 0x8c613810 */
L8c6137ae: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6137b0; /* fldi0 fr3 */
L8c6137b0: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6137b2; /* fcmp/gt fr4,fr3 */
L8c6137b2: if((*sr&1u))goto L8c613856;goto L8c6137b4; /* bt 0x8c613856 */
L8c6137b4: r[4]=r[11];goto L8c6137b6; /* mov r11,r4 */
L8c6137b6: r[5]=r[9];goto L8c6137b8; /* mov r9,r5 */
L8c6137b8: r[8]+=16u;goto L8c6137ba; /* add #0x10,r8 */
L8c6137ba: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6137bc; /* fschg */
L8c6137bc: fcopy(f,0u,1u);goto L8c6137be; /* fmov fr1,fr0 */
L8c6137be: fcopy(f,2u,3u);goto L8c6137c0; /* fmov fr3,fr2 */
L8c6137c0: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6137c2; /* fmov.s @r4+,fr4 */
L8c6137c2: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6137c4; /* fmov.s @r4+,fr6 */
L8c6137c4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6137c6; /* fldi0 fr3 */
L8c6137c6: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c6137c8; /* fipr fv4,fv0 */
L8c6137c8: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6137ca; /* fmov.s @r4+,fr8 */
L8c6137ca: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6137cc; /* fmov.s @r4+,fr10 */
L8c6137cc: fcopy(f,0u,9u);goto L8c6137ce; /* fmov fr9,fr0 */
L8c6137ce: fcopy(f,4u,5u);goto L8c6137d0; /* fmov fr5,fr4 */
L8c6137d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6137d2; /* fsub fr3,fr0 */
L8c6137d2: fcopy(f,6u,7u);goto L8c6137d4; /* fmov fr7,fr6 */
L8c6137d4: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6137d6; /* fschg */
L8c6137d6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c6137d8; /* fmov.s fr10,@-r8 */
L8c6137d8: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6137da; /* fmov.s fr0,@-r4 */
L8c6137da: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c6137dc; /* ocbwb @r4 */
L8c6137dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6137de; /* fmul fr0,fr7 */
L8c6137de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c6137e0; /* fmov.s fr9,@-r8 */
L8c6137e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6137e2; /* fmul fr0,fr6 */
L8c6137e2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c6137e4; /* fmov.s fr8,@-r8 */
L8c6137e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6137e6; /* fmul fr0,fr5 */
L8c6137e6: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6137e8; /* mov.l r5,@-r8 */
L8c6137e8: r[8]+=32u;goto L8c6137ea; /* add #0x20,r8 */
L8c6137ea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c6137ec; /* fmov.s fr7,@-r8 */
L8c6137ec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6137ee; /* fmov.s fr6,@-r8 */
L8c6137ee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6137f0; /* fmov.s fr5,@-r8 */
L8c6137f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6137f2; /* fmov.s fr4,@-r8 */
L8c6137f2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6137f4; /* pref @r8 */
L8c6137f4: r[8]+=16u;goto L8c6137f6; /* add #0x10,r8 */
L8c6137f6: r[3]=r[8];goto L8c6137f8; /* mov r8,r3 */
L8c6137f8: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6137fa; /* mov.l @(0x8,r15),r2 */
L8c6137fa: r[3]-=r[10];goto L8c6137fc; /* sub r10,r3 */
L8c6137fc: r[3]+=32u;goto L8c6137fe; /* add #0x20,r3 */
L8c6137fe: r[2]+=r[3];goto L8c613800; /* add r3,r2 */
L8c613800: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613802; /* mov.l r2,@(0x8,r15) */
L8c613802: r[10]=r[8];goto L8c613898; /* bra 0x8c613898 */
L8c613810: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613812; /* fldi0 fr3 */
L8c613812: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613814; /* fcmp/gt fr4,fr3 */
L8c613814: if(!(*sr&1u))goto L8c613898;goto L8c613816; /* bf 0x8c613898 */
L8c613816: r[5]=rd(m,bus,&io_status,0x8c613984u,4);if(io_status)return io_status;goto L8c613818; /* mov.l 0x8c613984,r5 */
L8c613818: r[8]+=16u;goto L8c61381a; /* add #0x10,r8 */
L8c61381a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61381c; /* fschg */
L8c61381c: fcopy(f,0u,1u);goto L8c61381e; /* fmov fr1,fr0 */
L8c61381e: fcopy(f,2u,3u);goto L8c613820; /* fmov fr3,fr2 */
L8c613820: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613822; /* fmov.s @r4+,fr4 */
L8c613822: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613824; /* fmov.s @r4+,fr6 */
L8c613824: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613826; /* fldi0 fr3 */
L8c613826: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c613828; /* fipr fv4,fv0 */
L8c613828: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61382a; /* fmov.s @r4+,fr8 */
L8c61382a: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61382c; /* fmov.s @r4+,fr10 */
L8c61382c: fcopy(f,0u,9u);goto L8c61382e; /* fmov fr9,fr0 */
L8c61382e: fcopy(f,4u,5u);goto L8c613830; /* fmov fr5,fr4 */
L8c613830: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c613832; /* fsub fr3,fr0 */
L8c613832: fcopy(f,6u,7u);goto L8c613834; /* fmov fr7,fr6 */
L8c613834: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613836; /* fschg */
L8c613836: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c613838; /* fmov.s fr10,@-r8 */
L8c613838: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61383a; /* fmov.s fr0,@-r4 */
L8c61383a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c61383c; /* ocbwb @r4 */
L8c61383c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c61383e; /* fmul fr0,fr7 */
L8c61383e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c613840; /* fmov.s fr9,@-r8 */
L8c613840: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c613842; /* fmul fr0,fr6 */
L8c613842: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c613844; /* fmov.s fr8,@-r8 */
L8c613844: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613846; /* fmul fr0,fr5 */
L8c613846: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613848; /* mov.l r5,@-r8 */
L8c613848: r[8]+=32u;goto L8c61384a; /* add #0x20,r8 */
L8c61384a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61384c; /* fmov.s fr7,@-r8 */
L8c61384c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c61384e; /* fmov.s fr6,@-r8 */
L8c61384e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613850; /* fmov.s fr5,@-r8 */
L8c613850: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613852; /* fmov.s fr4,@-r8 */
L8c613852: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613854; /* pref @r8 */
L8c613854: r[8]+=16u;goto L8c613856; /* add #0x10,r8 */
L8c613856: r[5]=rd(m,bus,&io_status,0x8c613984u,4);if(io_status)return io_status;goto L8c613858; /* mov.l 0x8c613984,r5 */
L8c613858: r[4]=r[11];goto L8c61385a; /* mov r11,r4 */
L8c61385a: r[8]+=16u;goto L8c61385c; /* add #0x10,r8 */
L8c61385c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61385e; /* fschg */
L8c61385e: fcopy(f,0u,1u);goto L8c613860; /* fmov fr1,fr0 */
L8c613860: fcopy(f,2u,3u);goto L8c613862; /* fmov fr3,fr2 */
L8c613862: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613864; /* fmov.s @r4+,fr4 */
L8c613864: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613866; /* fmov.s @r4+,fr6 */
L8c613866: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613868; /* fldi0 fr3 */
L8c613868: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c61386a; /* fipr fv4,fv0 */
L8c61386a: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61386c; /* fmov.s @r4+,fr8 */
L8c61386c: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61386e; /* fmov.s @r4+,fr10 */
L8c61386e: fcopy(f,0u,9u);goto L8c613870; /* fmov fr9,fr0 */
L8c613870: fcopy(f,4u,5u);goto L8c613872; /* fmov fr5,fr4 */
L8c613872: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c613874; /* fsub fr3,fr0 */
L8c613874: fcopy(f,6u,7u);goto L8c613876; /* fmov fr7,fr6 */
L8c613876: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613878; /* fschg */
L8c613878: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c61387a; /* fmov.s fr10,@-r8 */
L8c61387a: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61387c; /* fmov.s fr0,@-r4 */
L8c61387c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c61387e; /* ocbwb @r4 */
L8c61387e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c613880; /* fmul fr0,fr7 */
L8c613880: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c613882; /* fmov.s fr9,@-r8 */
L8c613882: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c613884; /* fmul fr0,fr6 */
L8c613884: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c613886; /* fmov.s fr8,@-r8 */
L8c613886: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613888; /* fmul fr0,fr5 */
L8c613888: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61388a; /* mov.l r5,@-r8 */
L8c61388a: r[8]+=32u;goto L8c61388c; /* add #0x20,r8 */
L8c61388c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61388e; /* fmov.s fr7,@-r8 */
L8c61388e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613890; /* fmov.s fr6,@-r8 */
L8c613890: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613892; /* fmov.s fr5,@-r8 */
L8c613892: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613894; /* fmov.s fr4,@-r8 */
L8c613894: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613896; /* pref @r8 */
L8c613896: r[8]+=16u;goto L8c613898; /* add #0x10,r8 */
L8c613898: fcopy(f,4u,14u);goto L8c61389a; /* fmov fr14,fr4 */
L8c61389a: r[0]=16u;goto L8c61389c; /* mov #0x10,r0 */
L8c61389c: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61389e; /* mov.l @r15,r3 */
L8c61389e: r[4]=r[11];goto L8c6138a0; /* mov r11,r4 */
L8c6138a0: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c6138a2; /* fmov.s @(r0,r12),fr14 */
L8c6138a2: r[0]=20u;goto L8c6138a4; /* mov #0x14,r0 */
L8c6138a4: fcopy(f,5u,13u);goto L8c6138a6; /* fmov fr13,fr5 */
L8c6138a6: r[3]+=4294967295u;goto L8c6138a8; /* add #-0x1,r3 */
L8c6138a8: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c6138aa; /* fmov.s @(r0,r12),fr13 */
L8c6138aa: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6138ac; /* tst r3,r3 */
L8c6138ac: r[11]=r[12];goto L8c6138ae; /* mov r12,r11 */
L8c6138ae: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c6138b0; /* fneg fr15 */
L8c6138b0: take=(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c6138b8;goto L8c6138b4; /* bt/s 0x8c6138b8 */
L8c6138b4: goto L8c613772; /* bra 0x8c613772 */
L8c6138b8: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c6138ba; /* cmp/hs r8,r10 */
L8c6138ba: if((*sr&1u))goto L8c61390a;goto L8c6138bc; /* bt 0x8c61390a */
L8c6138bc: r[4]=r[12];goto L8c6138be; /* mov r12,r4 */
L8c6138be: r[5]=r[9];goto L8c6138c0; /* mov r9,r5 */
L8c6138c0: r[8]+=16u;goto L8c6138c2; /* add #0x10,r8 */
L8c6138c2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6138c4; /* fschg */
L8c6138c4: fcopy(f,0u,1u);goto L8c6138c6; /* fmov fr1,fr0 */
L8c6138c6: fcopy(f,2u,3u);goto L8c6138c8; /* fmov fr3,fr2 */
L8c6138c8: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6138ca; /* fmov.s @r4+,fr4 */
L8c6138ca: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6138cc; /* fmov.s @r4+,fr6 */
L8c6138cc: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6138ce; /* fldi0 fr3 */
L8c6138ce: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c6138d0; /* fipr fv4,fv0 */
L8c6138d0: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6138d2; /* fmov.s @r4+,fr8 */
L8c6138d2: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6138d4; /* fmov.s @r4+,fr10 */
L8c6138d4: fcopy(f,0u,9u);goto L8c6138d6; /* fmov fr9,fr0 */
L8c6138d6: fcopy(f,4u,5u);goto L8c6138d8; /* fmov fr5,fr4 */
L8c6138d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6138da; /* fsub fr3,fr0 */
L8c6138da: fcopy(f,6u,7u);goto L8c6138dc; /* fmov fr7,fr6 */
L8c6138dc: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6138de; /* fschg */
L8c6138de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c6138e0; /* fmov.s fr10,@-r8 */
L8c6138e0: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6138e2; /* fmov.s fr0,@-r4 */
L8c6138e2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c6138e4; /* ocbwb @r4 */
L8c6138e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6138e6; /* fmul fr0,fr7 */
L8c6138e6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c6138e8; /* fmov.s fr9,@-r8 */
L8c6138e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6138ea; /* fmul fr0,fr6 */
L8c6138ea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c6138ec; /* fmov.s fr8,@-r8 */
L8c6138ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6138ee; /* fmul fr0,fr5 */
L8c6138ee: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6138f0; /* mov.l r5,@-r8 */
L8c6138f0: r[8]+=32u;goto L8c6138f2; /* add #0x20,r8 */
L8c6138f2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c6138f4; /* fmov.s fr7,@-r8 */
L8c6138f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6138f6; /* fmov.s fr6,@-r8 */
L8c6138f6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6138f8; /* fmov.s fr5,@-r8 */
L8c6138f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6138fa; /* fmov.s fr4,@-r8 */
L8c6138fa: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6138fc; /* pref @r8 */
L8c6138fc: r[8]+=16u;goto L8c6138fe; /* add #0x10,r8 */
L8c6138fe: r[3]=r[8];goto L8c613900; /* mov r8,r3 */
L8c613900: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613902; /* mov.l @(0x8,r15),r2 */
L8c613902: r[3]-=r[10];goto L8c613904; /* sub r10,r3 */
L8c613904: r[3]+=32u;goto L8c613906; /* add #0x20,r3 */
L8c613906: r[2]+=r[3];goto L8c613908; /* add r3,r2 */
L8c613908: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c61390a; /* mov.l r2,@(0x8,r15) */
L8c61390a: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61390c; /* mov.l @(0xc,r15),r3 */
L8c61390c: r[3]+=4294967295u;goto L8c61390e; /* add #-0x1,r3 */
L8c61390e: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613910; /* tst r3,r3 */
L8c613910: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c613918;goto L8c613914; /* bt/s 0x8c613918 */
L8c613914: goto L8c613720; /* bra 0x8c613720 */
L8c613918: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61391a; /* mov.l @(0x8,r15),r3 */
L8c61391a: r[4]=rd(m,bus,&io_status,0x8c613988u,4);if(io_status)return io_status;goto L8c61391c; /* mov.l 0x8c613988,r4 */
L8c61391c: r[3]>>=2;goto L8c61391e; /* shlr2 r3 */
L8c61391e: r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c613920; /* mov.l @r4,r1 */
L8c613920: r[3]>>=2;goto L8c613922; /* shlr2 r3 */
L8c613922: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c613924; /* shlr r3 */
L8c613924: r[1]+=r[3];goto L8c613926; /* add r3,r1 */
L8c613926: wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c613928; /* mov.l r1,@r4 */
L8c613928: r[4]=rd(m,bus,&io_status,0x8c61398cu,4);if(io_status)return io_status;goto L8c61392a; /* mov.l 0x8c61398c,r4 */
L8c61392a: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61392c; /* mov.l @(0x4,r15),r3 */
L8c61392c: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61392e; /* mov.l @r4,r2 */
L8c61392e: r[2]+=r[3];goto L8c613930; /* add r3,r2 */
L8c613930: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c613932; /* mov.l r2,@r4 */
L8c613932: r[15]+=16u;goto L8c613934; /* add #0x10,r15 */
L8c613934: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613936; /* lds.l @r15+,PR */
L8c613936: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613938; /* fmov.s @r15+,fr13 */
L8c613938: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61393a; /* fmov.s @r15+,fr14 */
L8c61393a: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61393c; /* fmov.s @r15+,fr15 */
L8c61393c: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61393e; /* mov.l @r15+,r9 */
L8c61393e: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613940; /* mov.l @r15+,r10 */
L8c613940: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613942; /* mov.l @r15+,r11 */
L8c613942: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613944; /* mov.l @r15+,r12 */
L8c613944: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613946; /* mov.l @r15+,r13 */
L8c613946: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_lit_alternate_strip_clipped_8c614bb0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c614bb0;
L8c614bb0: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614bb2; /* mov.l r14,@-r15 */
L8c614bb2: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c614bb4; /* mov.l r13,@-r15 */
L8c614bb4: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c614bb6; /* mov.l r12,@-r15 */
L8c614bb6: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c614bb8; /* mov.l r11,@-r15 */
L8c614bb8: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c614bba; /* mov.l r10,@-r15 */
L8c614bba: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c614bbc; /* mov.l r9,@-r15 */
L8c614bbc: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c614bbe; /* fmov.s fr15,@-r15 */
L8c614bbe: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c614bc0; /* fmov.s fr14,@-r15 */
L8c614bc0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c614bc2; /* fmov.s fr13,@-r15 */
L8c614bc2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c614bc4; /* fmov.s fr12,@-r15 */
L8c614bc4: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c614bc6; /* sts.l PR,@-r15 */
L8c614bc6: r[15]+=4294967220u;goto L8c614bc8; /* add #-0x4c,r15 */
L8c614bc8: r[3]=rd(m,bus,&io_status,0x8c614bf4u,4);if(io_status)return io_status;goto L8c614bca; /* mov.l 0x8c614bf4,r3 */
L8c614bca: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c614bcc; /* fldi1 fr12 */
L8c614bcc: target=r[3];*pr=0x8c614bd0u;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c614bd0; /* jsr @r3 */
L8c614bd0: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614bd2; /* mov.l @(0xc,r15),r3 */
L8c614bd2: r[2]=0u;goto L8c614bd4; /* mov #0x0,r2 */
L8c614bd4: r[14]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614bd6; /* mov.l @(0xc,r15),r14 */
L8c614bd6: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c614bd8; /* mov.w @(0x2,r3),r0 */
L8c614bd8: r[3]=rd(m,bus,&io_status,0x8c614bf8u,4);if(io_status)return io_status;goto L8c614bda; /* mov.l 0x8c614bf8,r3 */
L8c614bda: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c614bdc; /* mov.l @(0x4,r14),r14 */
L8c614bdc: r[0]=r[0]&65535u;goto L8c614bde; /* extu.w r0,r0 */
L8c614bde: wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c614be0; /* mov.l r0,@(0x18,r15) */
L8c614be0: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c614be2; /* mov.l @r3,r13 */
L8c614be2: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c614be4; /* mov.l r2,@(0xc,r15) */
L8c614be4: goto L8c614bfc; /* bra 0x8c614bfc */
L8c614bfc: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614bfe; /* mov.w @r14+,r3 */
L8c614bfe: r[4]=rd(m,bus,&io_status,0x8c614d18u,4);if(io_status)return io_status;goto L8c614c00; /* mov.l 0x8c614d18,r4 */
L8c614c00: r[3]=r[3]&65535u;goto L8c614c02; /* extu.w r3,r3 */
L8c614c02: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c614c04; /* tst r4,r3 */
L8c614c04: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c614c06; /* mov.l r3,@(0x8,r15) */
L8c614c06: take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c614c18;goto L8c614c0a; /* bt/s 0x8c614c18 */
L8c614c0a: r[4]=sx(m,bus,&io_status,0x8c614d14u,2);if(io_status)return io_status;goto L8c614c0c; /* mov.w 0x8c614d14,r4 */
L8c614c0c: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614c0e; /* mov.l @(0x8,r15),r3 */
L8c614c0e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614c10; /* fldi1 fr4 */
L8c614c10: r[3]&=r[4];goto L8c614c12; /* and r4,r3 */
L8c614c12: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c614c14; /* fneg fr4 */
L8c614c14: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c614c16; /* mov.l r3,@(0x8,r15) */
L8c614c16: fcopy(f,15u,4u);goto L8c614c18; /* fmov fr4,fr15 */
L8c614c18: r[0]=16u;goto L8c614c1a; /* mov #0x10,r0 */
L8c614c1a: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614c1c; /* mov.l @(0x8,r15),r3 */
L8c614c1c: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614c1e; /* mov.l @(0xc,r15),r2 */
L8c614c1e: r[9]=r[8];goto L8c614c20; /* mov r8,r9 */
L8c614c20: r[2]+=r[3];goto L8c614c22; /* add r3,r2 */
L8c614c22: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c614c24; /* mov.l r2,@(0xc,r15) */
L8c614c24: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614c26; /* mov.l @(0x8,r15),r1 */
L8c614c26: r[1]+=4294967294u;goto L8c614c28; /* add #-0x2,r1 */
L8c614c28: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c614c2a; /* mov.l r1,@(0x8,r15) */
L8c614c2a: r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614c2c; /* mov.w @r14+,r10 */
L8c614c2c: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614c2e; /* mov.w @r14+,r11 */
L8c614c2e: r[10]=r[10]&65535u;goto L8c614c30; /* extu.w r10,r10 */
L8c614c30: r[10]<<=2;goto L8c614c32; /* shll2 r10 */
L8c614c32: r[11]=r[11]&65535u;goto L8c614c34; /* extu.w r11,r11 */
L8c614c34: r[11]<<=2;goto L8c614c36; /* shll2 r11 */
L8c614c36: r[10]<<=2;goto L8c614c38; /* shll2 r10 */
L8c614c38: r[11]<<=2;goto L8c614c3a; /* shll2 r11 */
L8c614c3a: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c614c3c; /* shll r10 */
L8c614c3c: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c614c3e; /* shll r11 */
L8c614c3e: r[11]+=r[13];goto L8c614c40; /* add r13,r11 */
L8c614c40: r[10]+=r[13];goto L8c614c42; /* add r13,r10 */
L8c614c42: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c614c44; /* fmov.s @(r0,r11),fr13 */
L8c614c44: fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c614c46; /* fmov.s @(r0,r10),fr6 */
L8c614c46: r[0]=20u;goto L8c614c48; /* mov #0x14,r0 */
L8c614c48: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c614c4a; /* fmov.s @(r0,r11),fr14 */
L8c614c4a: fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c614c4c; /* fmov.s @(r0,r10),fr5 */
L8c614c4c: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614c4e; /* mov.w @r14+,r12 */
L8c614c4e: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c614c50; /* mov.w @r14,r3 */
L8c614c50: r[12]=r[12]&65535u;goto L8c614c52; /* extu.w r12,r12 */
L8c614c52: r[12]<<=2;goto L8c614c54; /* shll2 r12 */
L8c614c54: r[3]=r[3]&65535u;goto L8c614c56; /* extu.w r3,r3 */
L8c614c56: r[3]<<=2;goto L8c614c58; /* shll2 r3 */
L8c614c58: r[12]<<=2;goto L8c614c5a; /* shll2 r12 */
L8c614c5a: r[3]<<=2;goto L8c614c5c; /* shll2 r3 */
L8c614c5c: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c614c5e; /* shll r12 */
L8c614c5e: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c614c60; /* shll r3 */
L8c614c60: r[3]+=r[13];goto L8c614c62; /* add r13,r3 */
L8c614c62: r[12]+=r[13];goto L8c614c64; /* add r13,r12 */
L8c614c64: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c614c66; /* pref @r3 */
L8c614c66: r[2]=0u;goto L8c614c68; /* mov #0x0,r2 */
L8c614c68: wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c614c6a; /* mov.l r2,@(0x14,r15) */
L8c614c6a: r[4]=63u;goto L8c614c6c; /* mov #0x3f,r4 */
L8c614c6c: r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c614c6e; /* mov.l @(0xc,r10),r2 */
L8c614c6e: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c614c70; /* mov.l @(0xc,r11),r3 */
L8c614c70: r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c614c72; /* mov.l @(0xc,r12),r1 */
L8c614c72: r[2]&=r[3];goto L8c614c74; /* and r3,r2 */
L8c614c74: r[2]&=r[1];goto L8c614c76; /* and r1,r2 */
L8c614c76: r[4]&=r[2];goto L8c614c78; /* and r2,r4 */
L8c614c78: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c614c7a; /* tst r4,r4 */
L8c614c7a: take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c614c82;goto L8c614c7e; /* bt/s 0x8c614c82 */
L8c614c7e: goto L8c614ddc; /* bra 0x8c614ddc */
L8c614c82: r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c614c84; /* mov.l @(0xc,r10),r4 */
L8c614c84: r[5]=1u;goto L8c614c86; /* mov #0x1,r5 */
L8c614c86: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c614c88; /* mov.l @(0xc,r11),r3 */
L8c614c88: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c614c8a; /* mov.l @(0xc,r12),r2 */
L8c614c8a: r[4]|=r[3];goto L8c614c8c; /* or r3,r4 */
L8c614c8c: r[4]|=r[2];goto L8c614c8e; /* or r2,r4 */
L8c614c8e: r[4]&=r[5];goto L8c614c90; /* and r5,r4 */
L8c614c90: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c614c92; /* tst r4,r4 */
L8c614c92: if(!(*sr&1u))goto L8c614c98;goto L8c614c94; /* bf 0x8c614c98 */
L8c614c94: goto L8c614dbe; /* bra 0x8c614dbe */
L8c614c98: r[7]=r[15];goto L8c614c9a; /* mov r15,r7 */
L8c614c9a: r[7]+=64u;goto L8c614c9c; /* add #0x40,r7 */
L8c614c9c: wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c614c9e; /* mov.l r10,@r7 */
L8c614c9e: wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c614ca0; /* mov.l r11,@(0x4,r7) */
L8c614ca0: wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c614ca2; /* mov.l r12,@(0x8,r7) */
L8c614ca2: r[6]=0u;goto L8c614d50; /* bra 0x8c614d50 */
L8c614ca6: r[2]=r[6];goto L8c614ca8; /* mov r6,r2 */
L8c614ca8: r[1]=r[6];goto L8c614caa; /* mov r6,r1 */
L8c614caa: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c614cac; /* shll r2 */
L8c614cac: r[3]=r[6];goto L8c614cae; /* mov r6,r3 */
L8c614cae: r[3]<<=2;goto L8c614cb0; /* shll2 r3 */
L8c614cb0: r[2]+=r[1];goto L8c614cb2; /* add r1,r2 */
L8c614cb2: r[2]<<=2;goto L8c614cb4; /* shll2 r2 */
L8c614cb4: r[3]+=r[7];goto L8c614cb6; /* add r7,r3 */
L8c614cb6: r[2]+=r[4];goto L8c614cb8; /* add r4,r2 */
L8c614cb8: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c614cba; /* mov.l r3,@(0x4,r15) */
L8c614cba: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c614cbc; /* mov.l r2,@r15 */
L8c614cbc: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c614cbe; /* mov.l @r3,r1 */
L8c614cbe: r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c614cc0; /* mov.l @(0xc,r1),r2 */
L8c614cc0: *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c614cc2; /* tst r5,r2 */
L8c614cc2: take=!(*sr&1u);r[6]+=1u;if(take)goto L8c614d28;goto L8c614cc6; /* bf/s 0x8c614d28 */
L8c614cc6: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614cc8; /* mov.l @(0x4,r15),r1 */
L8c614cc8: r[0]=24u;goto L8c614cca; /* mov #0x18,r0 */
L8c614cca: fcopy(f,4u,12u);goto L8c614ccc; /* fmov fr12,fr4 */
L8c614ccc: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c614cce; /* mov.l @r1,r3 */
L8c614cce: r[4]=rd(m,bus,&io_status,0x8c614d1cu,4);if(io_status)return io_status;goto L8c614cd0; /* mov.l 0x8c614d1c,r4 */
L8c614cd0: wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c614cd2; /* mov.l r3,@(0x10,r15) */
L8c614cd2: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c614cd4; /* fmov.s @(r0,r3),fr3 */
L8c614cd4: r[0]=12u;goto L8c614cd6; /* mov #0xc,r0 */
L8c614cd6: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c614cd8; /* mov.l @(0x10,r15),r2 */
L8c614cd8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c614cda; /* fdiv fr3,fr4 */
L8c614cda: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c614cdc; /* fmov.s @(r0,r4),fr3 */
L8c614cdc: r[0]=16u;goto L8c614cde; /* mov #0x10,r0 */
L8c614cde: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614ce0; /* mov.l @r15,r3 */
L8c614ce0: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c614ce2; /* fmov.s @(r0,r2),fr2 */
L8c614ce2: r[0]=16u;goto L8c614ce4; /* mov #0x10,r0 */
L8c614ce4: r[2]=rd(m,bus,&io_status,0x8c614d20u,4);if(io_status)return io_status;goto L8c614ce6; /* mov.l 0x8c614d20,r2 */
L8c614ce6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614ce8; /* fsub fr3,fr2 */
L8c614ce8: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c614cea; /* fmov.s @r2,fr3 */
L8c614cea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614cec; /* fmul fr4,fr2 */
L8c614cec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614cee; /* fdiv fr3,fr2 */
L8c614cee: fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c614cf0; /* fmov.s fr2,@r3 */
L8c614cf0: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614cf2; /* mov.l @(0x4,r15),r1 */
L8c614cf2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c614cf4; /* fmov.s @(r0,r4),fr3 */
L8c614cf4: r[0]=20u;goto L8c614cf6; /* mov #0x14,r0 */
L8c614cf6: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c614cf8; /* mov.l @r1,r1 */
L8c614cf8: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614cfa; /* mov.l @r15,r3 */
L8c614cfa: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c614cfc; /* fmov.s @(r0,r1),fr2 */
L8c614cfc: r[1]=rd(m,bus,&io_status,0x8c614d24u,4);if(io_status)return io_status;goto L8c614cfe; /* mov.l 0x8c614d24,r1 */
L8c614cfe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614d00; /* fsub fr3,fr2 */
L8c614d00: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c614d02; /* fmov.s @r1,fr3 */
L8c614d02: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614d04; /* fmul fr4,fr2 */
L8c614d04: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614d06; /* fdiv fr3,fr2 */
L8c614d06: r[0]=4u;goto L8c614d08; /* mov #0x4,r0 */
L8c614d08: fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d0a; /* fmov fr2,@(r0,r3) */
L8c614d0a: r[0]=8u;goto L8c614d0c; /* mov #0x8,r0 */
L8c614d0c: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614d0e; /* mov.l @r15,r3 */
L8c614d0e: fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d10; /* fmov fr4,@(r0,r3) */
L8c614d10: goto L8c614d50; /* bra 0x8c614d50 */
L8c614d28: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614d2a; /* mov.l @(0x4,r15),r3 */
L8c614d2a: r[0]=16u;goto L8c614d2c; /* mov #0x10,r0 */
L8c614d2c: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614d2e; /* mov.l @r15,r2 */
L8c614d2e: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c614d30; /* mov.l @r3,r1 */
L8c614d30: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c614d32; /* fmov.s @(r0,r1),fr3 */
L8c614d32: r[0]=20u;goto L8c614d34; /* mov #0x14,r0 */
L8c614d34: fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c614d36; /* fmov.s fr3,@r2 */
L8c614d36: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614d38; /* mov.l @(0x4,r15),r2 */
L8c614d38: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614d3a; /* mov.l @r15,r3 */
L8c614d3a: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c614d3c; /* mov.l @r2,r1 */
L8c614d3c: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c614d3e; /* fmov.s @(r0,r1),fr3 */
L8c614d3e: r[0]=4u;goto L8c614d40; /* mov #0x4,r0 */
L8c614d40: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d42; /* fmov fr3,@(r0,r3) */
L8c614d42: r[0]=24u;goto L8c614d44; /* mov #0x18,r0 */
L8c614d44: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614d46; /* mov.l @(0x4,r15),r2 */
L8c614d46: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614d48; /* mov.l @r15,r3 */
L8c614d48: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c614d4a; /* mov.l @r2,r1 */
L8c614d4a: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c614d4c; /* fmov.s @(r0,r1),fr3 */
L8c614d4c: r[0]=8u;goto L8c614d4e; /* mov #0x8,r0 */
L8c614d4e: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d50; /* fmov fr3,@(r0,r3) */
L8c614d50: r[3]=3u;goto L8c614d52; /* mov #0x3,r3 */
L8c614d52: r[4]=r[15];goto L8c614d54; /* mov r15,r4 */
L8c614d54: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c614d56; /* cmp/ge r3,r6 */
L8c614d56: take=!(*sr&1u);r[4]+=28u;if(take)goto L8c614ca6;goto L8c614d5a; /* bf/s 0x8c614ca6 */
L8c614d5a: r[6]=r[4];goto L8c614d5c; /* mov r4,r6 */
L8c614d5c: r[0]=8u;goto L8c614d5e; /* mov #0x8,r0 */
L8c614d5e: r[3]=r[4];goto L8c614d60; /* mov r4,r3 */
L8c614d60: fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c614d62; /* fmov.s @(r0,r6),fr6 */
L8c614d62: r[3]+=12u;goto L8c614d64; /* add #0xc,r3 */
L8c614d64: r[7]=r[4];goto L8c614d66; /* mov r4,r7 */
L8c614d66: r[0]=4u;goto L8c614d68; /* mov #0x4,r0 */
L8c614d68: fcopy(f,0u,6u);goto L8c614d6a; /* fmov fr6,fr0 */
L8c614d6a: r[7]+=24u;goto L8c614d6c; /* add #0x18,r7 */
L8c614d6c: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c614d6e; /* mov.l r3,@r15 */
L8c614d6e: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c614d70; /* fmov.s @(r0,r6),fr4 */
L8c614d70: fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d72; /* fmov.s @(r0,r3),fr5 */
L8c614d72: r[0]=8u;goto L8c614d74; /* mov #0x8,r0 */
L8c614d74: fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c614d76; /* fmov.s @(r0,r3),fr9 */
L8c614d76: r[0]=4u;goto L8c614d78; /* mov #0x4,r0 */
L8c614d78: fcopy(f,3u,4u);goto L8c614d7a; /* fmov fr4,fr3 */
L8c614d7a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c614d7c; /* fmul fr5,fr0 */
L8c614d7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614d7e; /* fmul fr9,fr3 */
L8c614d7e: fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c614d80; /* fmov.s @r6,fr8 */
L8c614d80: fload(m,bus,&io_status,f,7u,r[3]);if(io_status)return io_status;goto L8c614d82; /* fmov.s @r3,fr7 */
L8c614d82: fcopy(f,2u,8u);goto L8c614d84; /* fmov fr8,fr2 */
L8c614d84: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614d86; /* fmul fr9,fr2 */
L8c614d86: fcopy(f,1u,4u);goto L8c614d88; /* fmov fr4,fr1 */
L8c614d88: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614d8a; /* fsub fr3,fr0 */
L8c614d8a: fcopy(f,3u,6u);goto L8c614d8c; /* fmov fr6,fr3 */
L8c614d8c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614d8e; /* fmul fr7,fr3 */
L8c614d8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614d90; /* fsub fr3,fr2 */
L8c614d90: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c614d92; /* fmov.s @(r0,r7),fr3 */
L8c614d92: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614d94; /* fmul fr2,fr3 */
L8c614d94: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c614d96; /* fmov.s @r7,fr2 */
L8c614d96: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614d98; /* fmac fr0,fr2,fr3 */
L8c614d98: fcopy(f,2u,3u);goto L8c614d9a; /* fmov fr3,fr2 */
L8c614d9a: fcopy(f,3u,8u);goto L8c614d9c; /* fmov fr8,fr3 */
L8c614d9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614d9e; /* fmul fr5,fr3 */
L8c614d9e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c614da0; /* fmul fr7,fr1 */
L8c614da0: r[0]=8u;goto L8c614da2; /* mov #0x8,r0 */
L8c614da2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c614da4; /* fsub fr3,fr1 */
L8c614da4: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c614da6; /* fmov.s @(r0,r7),fr3 */
L8c614da6: fcopy(f,0u,1u);goto L8c614da8; /* fmov fr1,fr0 */
L8c614da8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614daa; /* fmac fr0,fr3,fr2 */
L8c614daa: fcopy(f,3u,15u);goto L8c614dac; /* fmov fr15,fr3 */
L8c614dac: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c614dae; /* fneg fr3 */
L8c614dae: fcopy(f,4u,2u);goto L8c614db0; /* fmov fr2,fr4 */
L8c614db0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c614db2; /* fmul fr3,fr4 */
L8c614db2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c614db4; /* fldi0 fr2 */
L8c614db4: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c614db6; /* fcmp/gt fr2,fr4 */
L8c614db6: if((*sr&1u))goto L8c614ddc;goto L8c614db8; /* bt 0x8c614ddc */
L8c614db8: wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c614dba; /* mov.l r5,@(0x14,r15) */
L8c614dba: fcopy(f,4u,12u);goto L8c614ddc; /* bra 0x8c614ddc */
L8c614dbe: r[0]=20u;goto L8c614dc0; /* mov #0x14,r0 */
L8c614dc0: fcopy(f,3u,13u);goto L8c614dc2; /* fmov fr13,fr3 */
L8c614dc2: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c614dc4; /* fmov.s @(r0,r12),fr2 */
L8c614dc4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614dc6; /* fsub fr6,fr3 */
L8c614dc6: r[0]=16u;goto L8c614dc8; /* mov #0x10,r0 */
L8c614dc8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614dca; /* fsub fr14,fr2 */
L8c614dca: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c614dcc; /* fmov.s @(r0,r12),fr1 */
L8c614dcc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c614dce; /* fsub fr13,fr1 */
L8c614dce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614dd0; /* fmul fr2,fr3 */
L8c614dd0: fcopy(f,2u,14u);goto L8c614dd2; /* fmov fr14,fr2 */
L8c614dd2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614dd4; /* fsub fr5,fr2 */
L8c614dd4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614dd6; /* fmul fr1,fr2 */
L8c614dd6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614dd8; /* fsub fr2,fr3 */
L8c614dd8: fcopy(f,4u,3u);goto L8c614dda; /* fmov fr3,fr4 */
L8c614dda: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c614ddc; /* fmul fr15,fr4 */
L8c614ddc: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c614dde; /* cmp/hs r8,r9 */
L8c614dde: if((*sr&1u))goto L8c614e2c;goto L8c614de0; /* bt 0x8c614e2c */
L8c614de0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614de2; /* fldi0 fr3 */
L8c614de2: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614de4; /* fcmp/gt fr4,fr3 */
L8c614de4: if((*sr&1u))goto L8c614e74;goto L8c614de6; /* bt 0x8c614e74 */
L8c614de6: r[5]=rd(m,bus,&io_status,0x8c614fa8u,4);if(io_status)return io_status;goto L8c614de8; /* mov.l 0x8c614fa8,r5 */
L8c614de8: r[4]=r[11];goto L8c614dea; /* mov r11,r4 */
L8c614dea: r[8]+=16u;goto L8c614dec; /* add #0x10,r8 */
L8c614dec: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614dee; /* fschg */
L8c614dee: fcopy(f,0u,1u);goto L8c614df0; /* fmov fr1,fr0 */
L8c614df0: fcopy(f,2u,3u);goto L8c614df2; /* fmov fr3,fr2 */
L8c614df2: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614df4; /* fmov.s @r4+,fr4 */
L8c614df4: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614df6; /* fmov.s @r4+,fr6 */
L8c614df6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614df8; /* fldi0 fr3 */
L8c614df8: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c614dfa; /* fipr fv4,fv0 */
L8c614dfa: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614dfc; /* fmov.s @r4+,fr8 */
L8c614dfc: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614dfe; /* fmov.s @r4+,fr10 */
L8c614dfe: fcopy(f,0u,9u);goto L8c614e00; /* fmov fr9,fr0 */
L8c614e00: fcopy(f,4u,5u);goto L8c614e02; /* fmov fr5,fr4 */
L8c614e02: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614e04; /* fsub fr3,fr0 */
L8c614e04: fcopy(f,6u,7u);goto L8c614e06; /* fmov fr7,fr6 */
L8c614e06: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614e08; /* fschg */
L8c614e08: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c614e0a; /* fmov.s fr10,@-r8 */
L8c614e0a: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c614e0c; /* fmov.s fr0,@-r4 */
L8c614e0c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c614e0e; /* ocbwb @r4 */
L8c614e0e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c614e10; /* fmul fr0,fr7 */
L8c614e10: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c614e12; /* fmov.s fr9,@-r8 */
L8c614e12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c614e14; /* fmul fr0,fr6 */
L8c614e14: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c614e16; /* fmov.s fr8,@-r8 */
L8c614e16: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c614e18; /* fmul fr0,fr5 */
L8c614e18: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614e1a; /* mov.l r5,@-r8 */
L8c614e1a: r[8]+=32u;goto L8c614e1c; /* add #0x20,r8 */
L8c614e1c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c614e1e; /* fmov.s fr7,@-r8 */
L8c614e1e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614e20; /* fmov.s fr6,@-r8 */
L8c614e20: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614e22; /* fmov.s fr5,@-r8 */
L8c614e22: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614e24; /* fmov.s fr4,@-r8 */
L8c614e24: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614e26; /* pref @r8 */
L8c614e26: r[8]+=16u;goto L8c614e28; /* add #0x10,r8 */
L8c614e28: r[9]=r[8];goto L8c614eb6; /* bra 0x8c614eb6 */
L8c614e2c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614e2e; /* fldi0 fr3 */
L8c614e2e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614e30; /* fcmp/gt fr4,fr3 */
L8c614e30: if(!(*sr&1u))goto L8c614eb6;goto L8c614e32; /* bf 0x8c614eb6 */
L8c614e32: r[5]=rd(m,bus,&io_status,0x8c614facu,4);if(io_status)return io_status;goto L8c614e34; /* mov.l 0x8c614fac,r5 */
L8c614e34: r[4]=r[10];goto L8c614e36; /* mov r10,r4 */
L8c614e36: r[8]+=16u;goto L8c614e38; /* add #0x10,r8 */
L8c614e38: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614e3a; /* fschg */
L8c614e3a: fcopy(f,0u,1u);goto L8c614e3c; /* fmov fr1,fr0 */
L8c614e3c: fcopy(f,2u,3u);goto L8c614e3e; /* fmov fr3,fr2 */
L8c614e3e: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e40; /* fmov.s @r4+,fr4 */
L8c614e40: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e42; /* fmov.s @r4+,fr6 */
L8c614e42: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614e44; /* fldi0 fr3 */
L8c614e44: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c614e46; /* fipr fv4,fv0 */
L8c614e46: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e48; /* fmov.s @r4+,fr8 */
L8c614e48: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e4a; /* fmov.s @r4+,fr10 */
L8c614e4a: fcopy(f,0u,9u);goto L8c614e4c; /* fmov fr9,fr0 */
L8c614e4c: fcopy(f,4u,5u);goto L8c614e4e; /* fmov fr5,fr4 */
L8c614e4e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614e50; /* fsub fr3,fr0 */
L8c614e50: fcopy(f,6u,7u);goto L8c614e52; /* fmov fr7,fr6 */
L8c614e52: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614e54; /* fschg */
L8c614e54: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c614e56; /* fmov.s fr10,@-r8 */
L8c614e56: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c614e58; /* fmov.s fr0,@-r4 */
L8c614e58: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c614e5a; /* ocbwb @r4 */
L8c614e5a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c614e5c; /* fmul fr0,fr7 */
L8c614e5c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c614e5e; /* fmov.s fr9,@-r8 */
L8c614e5e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c614e60; /* fmul fr0,fr6 */
L8c614e60: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c614e62; /* fmov.s fr8,@-r8 */
L8c614e62: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c614e64; /* fmul fr0,fr5 */
L8c614e64: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614e66; /* mov.l r5,@-r8 */
L8c614e66: r[8]+=32u;goto L8c614e68; /* add #0x20,r8 */
L8c614e68: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c614e6a; /* fmov.s fr7,@-r8 */
L8c614e6a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614e6c; /* fmov.s fr6,@-r8 */
L8c614e6c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614e6e; /* fmov.s fr5,@-r8 */
L8c614e6e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614e70; /* fmov.s fr4,@-r8 */
L8c614e70: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614e72; /* pref @r8 */
L8c614e72: r[8]+=16u;goto L8c614e74; /* add #0x10,r8 */
L8c614e74: r[5]=rd(m,bus,&io_status,0x8c614facu,4);if(io_status)return io_status;goto L8c614e76; /* mov.l 0x8c614fac,r5 */
L8c614e76: r[4]=r[11];goto L8c614e78; /* mov r11,r4 */
L8c614e78: r[8]+=16u;goto L8c614e7a; /* add #0x10,r8 */
L8c614e7a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614e7c; /* fschg */
L8c614e7c: fcopy(f,0u,1u);goto L8c614e7e; /* fmov fr1,fr0 */
L8c614e7e: fcopy(f,2u,3u);goto L8c614e80; /* fmov fr3,fr2 */
L8c614e80: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e82; /* fmov.s @r4+,fr4 */
L8c614e82: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e84; /* fmov.s @r4+,fr6 */
L8c614e84: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614e86; /* fldi0 fr3 */
L8c614e86: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c614e88; /* fipr fv4,fv0 */
L8c614e88: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e8a; /* fmov.s @r4+,fr8 */
L8c614e8a: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614e8c; /* fmov.s @r4+,fr10 */
L8c614e8c: fcopy(f,0u,9u);goto L8c614e8e; /* fmov fr9,fr0 */
L8c614e8e: fcopy(f,4u,5u);goto L8c614e90; /* fmov fr5,fr4 */
L8c614e90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614e92; /* fsub fr3,fr0 */
L8c614e92: fcopy(f,6u,7u);goto L8c614e94; /* fmov fr7,fr6 */
L8c614e94: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614e96; /* fschg */
L8c614e96: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c614e98; /* fmov.s fr10,@-r8 */
L8c614e98: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c614e9a; /* fmov.s fr0,@-r4 */
L8c614e9a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c614e9c; /* ocbwb @r4 */
L8c614e9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c614e9e; /* fmul fr0,fr7 */
L8c614e9e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c614ea0; /* fmov.s fr9,@-r8 */
L8c614ea0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c614ea2; /* fmul fr0,fr6 */
L8c614ea2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c614ea4; /* fmov.s fr8,@-r8 */
L8c614ea4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c614ea6; /* fmul fr0,fr5 */
L8c614ea6: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614ea8; /* mov.l r5,@-r8 */
L8c614ea8: r[8]+=32u;goto L8c614eaa; /* add #0x20,r8 */
L8c614eaa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c614eac; /* fmov.s fr7,@-r8 */
L8c614eac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614eae; /* fmov.s fr6,@-r8 */
L8c614eae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614eb0; /* fmov.s fr5,@-r8 */
L8c614eb0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614eb2; /* fmov.s fr4,@-r8 */
L8c614eb2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614eb4; /* pref @r8 */
L8c614eb4: r[8]+=16u;goto L8c614eb6; /* add #0x10,r8 */
L8c614eb6: r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c614eb8; /* mov.l @(0x14,r15),r2 */
L8c614eb8: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c614eba; /* tst r2,r2 */
L8c614eba: if((*sr&1u))goto L8c614ece;goto L8c614ebc; /* bt 0x8c614ece */
L8c614ebc: r[4]=rd(m,bus,&io_status,0x8c614fb0u,4);if(io_status)return io_status;goto L8c614ebe; /* mov.l 0x8c614fb0,r4 */
L8c614ebe: r[3]=0u;goto L8c614ec0; /* mov #0x0,r3 */
L8c614ec0: r[2]=rd(m,bus,&io_status,0x8c614fb4u,4);if(io_status)return io_status;goto L8c614ec2; /* mov.l 0x8c614fb4,r2 */
L8c614ec2: r[5]=r[11];goto L8c614ec4; /* mov r11,r5 */
L8c614ec4: wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c614ec6; /* mov.l r3,@r4 */
L8c614ec6: r[6]=r[12];goto L8c614ec8; /* mov r12,r6 */
L8c614ec8: target=r[2];*pr=0x8c614eccu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c614ecc; /* jsr @r2 */
L8c614ecc: r[9]=r[8];goto L8c614ece; /* mov r8,r9 */
L8c614ece: fcopy(f,6u,13u);goto L8c614ed0; /* fmov fr13,fr6 */
L8c614ed0: r[0]=16u;goto L8c614ed2; /* mov #0x10,r0 */
L8c614ed2: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614ed4; /* mov.l @(0x8,r15),r3 */
L8c614ed4: r[10]=r[11];goto L8c614ed6; /* mov r11,r10 */
L8c614ed6: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c614ed8; /* fmov.s @(r0,r12),fr13 */
L8c614ed8: r[0]=20u;goto L8c614eda; /* mov #0x14,r0 */
L8c614eda: fcopy(f,5u,14u);goto L8c614edc; /* fmov fr14,fr5 */
L8c614edc: r[3]+=4294967295u;goto L8c614ede; /* add #-0x1,r3 */
L8c614ede: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c614ee0; /* fmov.s @(r0,r12),fr14 */
L8c614ee0: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614ee2; /* tst r3,r3 */
L8c614ee2: r[11]=r[12];goto L8c614ee4; /* mov r12,r11 */
L8c614ee4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c614ee6; /* fneg fr15 */
L8c614ee6: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c614eee;goto L8c614eea; /* bt/s 0x8c614eee */
L8c614eea: goto L8c614c4c; /* bra 0x8c614c4c */
L8c614eee: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c614ef0; /* cmp/hs r8,r9 */
L8c614ef0: if((*sr&1u))goto L8c614f34;goto L8c614ef2; /* bt 0x8c614f34 */
L8c614ef2: r[5]=rd(m,bus,&io_status,0x8c614fa8u,4);if(io_status)return io_status;goto L8c614ef4; /* mov.l 0x8c614fa8,r5 */
L8c614ef4: r[4]=r[12];goto L8c614ef6; /* mov r12,r4 */
L8c614ef6: r[8]+=16u;goto L8c614ef8; /* add #0x10,r8 */
L8c614ef8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614efa; /* fschg */
L8c614efa: fcopy(f,0u,1u);goto L8c614efc; /* fmov fr1,fr0 */
L8c614efc: fcopy(f,2u,3u);goto L8c614efe; /* fmov fr3,fr2 */
L8c614efe: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f00; /* fmov.s @r4+,fr4 */
L8c614f00: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f02; /* fmov.s @r4+,fr6 */
L8c614f02: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614f04; /* fldi0 fr3 */
L8c614f04: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c614f06; /* fipr fv4,fv0 */
L8c614f06: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f08; /* fmov.s @r4+,fr8 */
L8c614f08: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f0a; /* fmov.s @r4+,fr10 */
L8c614f0a: fcopy(f,0u,9u);goto L8c614f0c; /* fmov fr9,fr0 */
L8c614f0c: fcopy(f,4u,5u);goto L8c614f0e; /* fmov fr5,fr4 */
L8c614f0e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614f10; /* fsub fr3,fr0 */
L8c614f10: fcopy(f,6u,7u);goto L8c614f12; /* fmov fr7,fr6 */
L8c614f12: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614f14; /* fschg */
L8c614f14: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c614f16; /* fmov.s fr10,@-r8 */
L8c614f16: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c614f18; /* fmov.s fr0,@-r4 */
L8c614f18: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c614f1a; /* ocbwb @r4 */
L8c614f1a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c614f1c; /* fmul fr0,fr7 */
L8c614f1c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c614f1e; /* fmov.s fr9,@-r8 */
L8c614f1e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c614f20; /* fmul fr0,fr6 */
L8c614f20: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c614f22; /* fmov.s fr8,@-r8 */
L8c614f22: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c614f24; /* fmul fr0,fr5 */
L8c614f24: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614f26; /* mov.l r5,@-r8 */
L8c614f26: r[8]+=32u;goto L8c614f28; /* add #0x20,r8 */
L8c614f28: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c614f2a; /* fmov.s fr7,@-r8 */
L8c614f2a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614f2c; /* fmov.s fr6,@-r8 */
L8c614f2c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614f2e; /* fmov.s fr5,@-r8 */
L8c614f2e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614f30; /* fmov.s fr4,@-r8 */
L8c614f30: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614f32; /* pref @r8 */
L8c614f32: r[8]+=16u;goto L8c614f34; /* add #0x10,r8 */
L8c614f34: r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c614f36; /* mov.l @(0x18,r15),r3 */
L8c614f36: r[3]+=4294967295u;goto L8c614f38; /* add #-0x1,r3 */
L8c614f38: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614f3a; /* tst r3,r3 */
L8c614f3a: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c614f42;goto L8c614f3e; /* bt/s 0x8c614f42 */
L8c614f3e: goto L8c614bfc; /* bra 0x8c614bfc */
L8c614f42: r[4]=rd(m,bus,&io_status,0x8c614fb8u,4);if(io_status)return io_status;goto L8c614f44; /* mov.l 0x8c614fb8,r4 */
L8c614f44: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614f46; /* mov.l @(0xc,r15),r3 */
L8c614f46: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614f48; /* mov.l @r4,r2 */
L8c614f48: r[2]+=r[3];goto L8c614f4a; /* add r3,r2 */
L8c614f4a: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c614f4c; /* mov.l r2,@r4 */
L8c614f4c: r[4]=rd(m,bus,&io_status,0x8c614fbcu,4);if(io_status)return io_status;goto L8c614f4e; /* mov.l 0x8c614fbc,r4 */
L8c614f4e: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614f50; /* mov.l @(0xc,r15),r3 */
L8c614f50: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614f52; /* mov.l @r4,r2 */
L8c614f52: r[2]+=r[3];goto L8c614f54; /* add r3,r2 */
L8c614f54: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c614f56; /* mov.l r2,@r4 */
L8c614f56: r[15]+=76u;goto L8c614f58; /* add #0x4c,r15 */
L8c614f58: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f5a; /* lds.l @r15+,PR */
L8c614f5a: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f5c; /* fmov.s @r15+,fr12 */
L8c614f5c: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f5e; /* fmov.s @r15+,fr13 */
L8c614f5e: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f60; /* fmov.s @r15+,fr14 */
L8c614f60: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614f62; /* fmov.s @r15+,fr15 */
L8c614f62: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f64; /* mov.l @r15+,r9 */
L8c614f64: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f66; /* mov.l @r15+,r10 */
L8c614f66: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f68; /* mov.l @r15+,r11 */
L8c614f68: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f6a; /* mov.l @r15+,r12 */
L8c614f6a: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614f6c; /* mov.l @r15+,r13 */
L8c614f6c: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
