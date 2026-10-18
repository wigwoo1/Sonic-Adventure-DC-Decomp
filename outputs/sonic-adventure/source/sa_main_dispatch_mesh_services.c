/* Mesh filter and material/header services; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
#include "sh4_divide.h"
#include "sh4_fmac.h"
#include "sh4_vector_numeric.h"
#include "sh4_multiply_extended.h"
#include "sh4_float_i32.h"
#include "sh4_compare_bits.h"
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,uint32_t a,uint32_t value,unsigned n){
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,a,4);f->bank[bank][reg+1]=rd(m,a+4u,4);
 }else f->bank[bank][reg]=rd(m,a,4);
}
static void fstore(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,a,f->bank[bank][reg],4);wr(m,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,a,f->bank[bank][reg],4);
}
static void fcopy(struct sa_fpu_banks *f,unsigned n,unsigned s){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){unsigned nb=bank^(n&1u),sb=bank^(s&1u);n&=~1u;s&=~1u;
  uint32_t a=f->bank[sb][s],b=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=b;
 }else f->bank[bank][n]=f->bank[bank][s];
}
SA_STAGE_EXPORT int sa_mesh_filter_8c611444(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c611444;
L8c611444: r[15]-=4;wr(m,r[15],r[14],4);goto L8c611446; /* mov.l r14,@-r15 */
L8c611446: r[15]-=4;wr(m,r[15],r[13],4);goto L8c611448; /* mov.l r13,@-r15 */
L8c611448: r[15]-=4;wr(m,r[15],r[12],4);goto L8c61144a; /* mov.l r12,@-r15 */
L8c61144a: r[15]-=4;wr(m,r[15],r[11],4);goto L8c61144c; /* mov.l r11,@-r15 */
L8c61144c: r[15]-=4;wr(m,r[15],r[10],4);goto L8c61144e; /* mov.l r10,@-r15 */
L8c61144e: r[15]-=4;wr(m,r[15],r[9],4);goto L8c611450; /* mov.l r9,@-r15 */
L8c611450: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto L8c611452; /* fmov.s fr14,@-r15 */
L8c611452: r[15]-=4;wr(m,r[15],*pr,4);goto L8c611454; /* sts.l PR,@-r15 */
L8c611454: r[15]+=4294967232u;goto L8c611456; /* add #-0x40,r15 */
L8c611456: r[3]=rd(m,0x8c6114b0u,4);goto L8c611458; /* mov.l 0x8c6114b0,r3 */
L8c611458: r[2]=0u;goto L8c61145a; /* mov #0x0,r2 */
L8c61145a: r[1]=rd(m,0x8c6114b4u,4);goto L8c61145c; /* mov.l 0x8c6114b4,r1 */
L8c61145c: r[14]=r[4];goto L8c61145e; /* mov r4,r14 */
L8c61145e: r[11]=rd(m,0x8c6114c0u,4);goto L8c611460; /* mov.l 0x8c6114c0,r11 */
L8c611460: r[12]=r[4];goto L8c611462; /* mov r4,r12 */
L8c611462: r[0]=rd(m,0x8c6114bcu,4);goto L8c611464; /* mov.l 0x8c6114bc,r0 */
L8c611464: r[10]=rd(m,0x8c6114b8u,4);goto L8c611466; /* mov.l 0x8c6114b8,r10 */
L8c611466: r[9]=rd(m,0x8c6114d4u,4);goto L8c611468; /* mov.l 0x8c6114d4,r9 */
L8c611468: wr(m,r[15],r[4],4);goto L8c61146a; /* mov.l r4,@r15 */
L8c61146a: r[13]=rd(m,r[3],4);goto L8c61146c; /* mov.l @r3,r13 */
L8c61146c: r[3]=rd(m,0x8c6114c4u,4);goto L8c61146e; /* mov.l 0x8c6114c4,r3 */
L8c61146e: r[14]=rd(m,r[14]+0x4u,4);goto L8c611470; /* mov.l @(0x4,r14),r14 */
L8c611470: r[12]=rd(m,r[12],4);goto L8c611472; /* mov.l @r12,r12 */
L8c611472: wr(m,r[1],r[2],4);goto L8c611474; /* mov.l r2,@r1 */
L8c611474: r[2]=rd(m,r[0],4);goto L8c611476; /* mov.l @r0,r2 */
L8c611476: r[1]=rd(m,0x8c6114ccu,4);goto L8c611478; /* mov.l 0x8c6114cc,r1 */
L8c611478: wr(m,r[10],r[2],4);goto L8c61147a; /* mov.l r2,@r10 */
L8c61147a: fload(m,f,3u,r[3]);goto L8c61147c; /* fmov.s @r3,fr3 */
L8c61147c: fload(m,f,2u,r[11]);goto L8c61147e; /* fmov.s @r11,fr2 */
L8c61147e: r[2]=rd(m,0x8c6114c8u,4);goto L8c611480; /* mov.l 0x8c6114c8,r2 */
L8c611480: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611482; /* fmul fr3,fr2 */
L8c611482: r[3]=rd(m,0x8c6114d0u,4);goto L8c611484; /* mov.l 0x8c6114d0,r3 */
L8c611484: fstore(m,f,2u,r[2]);goto L8c611486; /* fmov.s fr2,@r2 */
L8c611486: fload(m,f,2u,r[11]);goto L8c611488; /* fmov.s @r11,fr2 */
L8c611488: fload(m,f,3u,r[1]);goto L8c61148a; /* fmov.s @r1,fr3 */
L8c61148a: r[2]=sx(m,0x8c6114aeu,2);goto L8c61148c; /* mov.w 0x8c6114ae,r2 */
L8c61148c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61148e; /* fmul fr3,fr2 */
L8c61148e: fstore(m,f,2u,r[3]);goto L8c611490; /* fmov.s fr2,@r3 */
L8c611490: r[3]=rd(m,0x8c6114d8u,4);goto L8c611492; /* mov.l 0x8c6114d8,r3 */
L8c611492: r[0]=rd(m,r[3],4);goto L8c611494; /* mov.l @r3,r0 */
L8c611494: *sr=(*sr&~1u)|!(r[2]&r[0]);goto L8c611496; /* tst r2,r0 */
L8c611496: if(!(*sr&1u))goto L8c61149e;goto L8c611498; /* bf 0x8c61149e */
L8c611498: r[1]=rd(m,0x8c6114dcu,4);goto L8c61149a; /* mov.l 0x8c6114dc,r1 */
L8c61149a: target=r[1];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r1 */
L8c61149e: r[5]=rd(m,r[15],4);goto L8c6114a0; /* mov.l @r15,r5 */
L8c6114a0: r[4]=r[15];goto L8c6114a2; /* mov r15,r4 */
L8c6114a2: r[4]+=4u;goto L8c6114a4; /* add #0x4,r4 */
L8c6114a4: r[5]+=24u;goto L8c6114a6; /* add #0x18,r5 */
L8c6114a6: goto L8c6114e0; /* bra 0x8c6114e0 */
L8c6114e0: fload(m,f,0u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6114e2; /* fmov.s @r5+,fr0 */
L8c6114e2: fload(m,f,1u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6114e4; /* fmov.s @r5+,fr1 */
L8c6114e4: fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6114e6; /* fmov.s @r5+,fr2 */
L8c6114e6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6114e8; /* fldi1 fr3 */
L8c6114e8: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6114ea; /* ftrv xmtrx,fv0 */
L8c6114ea: r[4]+=12u;goto L8c6114ec; /* add #0xc,r4 */
L8c6114ec: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[4]);goto L8c6114ee; /* fmov.s fr2,@-r4 */
L8c6114ee: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[4]);goto L8c6114f0; /* fmov.s fr1,@-r4 */
L8c6114f0: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[4]);goto L8c6114f2; /* fmov.s fr0,@-r4 */
L8c6114f2: r[2]=rd(m,r[15],4);goto L8c6114f4; /* mov.l @r15,r2 */
L8c6114f4: r[0]=36u;goto L8c6114f6; /* mov #0x24,r0 */
L8c6114f6: r[3]=rd(m,0x8c6115bcu,4);goto L8c6114f8; /* mov.l 0x8c6115bc,r3 */
L8c6114f8: if(((r[0]+r[2])&0x1f000000u)!=0x0c000000u)return -3;fload(m,f,10u,r[0]+r[2]);goto L8c6114fa; /* fmov.s @(r0,r2),fr10 */
L8c6114fa: r[0]=12u;goto L8c6114fc; /* mov #0xc,r0 */
L8c6114fc: fload(m,f,4u,r[0]+r[15]);goto L8c6114fe; /* fmov.s @(r0,r15),fr4 */
L8c6114fe: fload(m,f,2u,r[3]);goto L8c611500; /* fmov.s @r3,fr2 */
L8c611500: fcopy(f,3u,4u);goto L8c611502; /* fmov fr4,fr3 */
L8c611502: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c611504; /* fadd fr10,fr3 */
L8c611504: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c611506; /* fcmp/gt fr2,fr3 */
L8c611506: if((*sr&1u))goto L8c61150e;goto L8c611508; /* bt 0x8c61150e */
L8c611508: r[2]=rd(m,0x8c6115c0u,4);goto L8c61150a; /* mov.l 0x8c6115c0,r2 */
L8c61150a: target=r[2];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r2 */
L8c61150e: fcopy(f,3u,4u);goto L8c611510; /* fmov fr4,fr3 */
L8c611510: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611512; /* fsub fr10,fr3 */
L8c611512: r[3]=rd(m,0x8c6115c4u,4);goto L8c611514; /* mov.l 0x8c6115c4,r3 */
L8c611514: fload(m,f,2u,r[3]);goto L8c611516; /* fmov.s @r3,fr2 */
L8c611516: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c611518; /* fcmp/gt fr2,fr3 */
L8c611518: if(!(*sr&1u))goto L8c611520;goto L8c61151a; /* bf 0x8c611520 */
L8c61151a: r[2]=rd(m,0x8c6115c0u,4);goto L8c61151c; /* mov.l 0x8c6115c0,r2 */
L8c61151c: target=r[2];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r2 */
L8c611520: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[10],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c611522; /* fadd fr10,fr4 */
L8c611522: r[0]=4u;goto L8c611524; /* mov #0x4,r0 */
L8c611524: fload(m,f,9u,r[0]+r[15]);goto L8c611526; /* fmov.s @(r0,r15),fr9 */
L8c611526: r[0]=12u;goto L8c611528; /* mov #0xc,r0 */
L8c611528: r[3]=rd(m,0x8c6115c8u,4);goto L8c61152a; /* mov.l 0x8c6115c8,r3 */
L8c61152a: fload(m,f,7u,r[0]+r[11]);goto L8c61152c; /* fmov.s @(r0,r11),fr7 */
L8c61152c: r[0]=8u;goto L8c61152e; /* mov #0x8,r0 */
L8c61152e: fcopy(f,3u,4u);goto L8c611530; /* fmov fr4,fr3 */
L8c611530: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c611532; /* fldi1 fr4 */
L8c611532: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c611534; /* fdiv fr3,fr4 */
L8c611534: fcopy(f,3u,9u);goto L8c611536; /* fmov fr9,fr3 */
L8c611536: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611538; /* fsub fr10,fr3 */
L8c611538: fload(m,f,8u,r[3]);goto L8c61153a; /* fmov.s @r3,fr8 */
L8c61153a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[10],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c61153c; /* fadd fr10,fr9 */
L8c61153c: fload(m,f,14u,r[0]+r[15]);goto L8c61153e; /* fmov.s @(r0,r15),fr14 */
L8c61153e: r[0]=16u;goto L8c611540; /* mov #0x10,r0 */
L8c611540: fcopy(f,2u,8u);goto L8c611542; /* fmov fr8,fr2 */
L8c611542: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611544; /* fmul fr3,fr2 */
L8c611544: r[2]=rd(m,0x8c6115ccu,4);goto L8c611546; /* mov.l 0x8c6115cc,r2 */
L8c611546: fcopy(f,3u,14u);goto L8c611548; /* fmov fr14,fr3 */
L8c611548: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61154a; /* fsub fr10,fr3 */
L8c61154a: fcopy(f,5u,7u);goto L8c61154c; /* fmov fr7,fr5 */
L8c61154c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[9],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61154e; /* fmul fr9,fr8 */
L8c61154e: fload(m,f,1u,r[2]);goto L8c611550; /* fmov.s @r2,fr1 */
L8c611550: fload(m,f,11u,r[0]+r[11]);goto L8c611552; /* fmov.s @(r0,r11),fr11 */
L8c611552: fcopy(f,6u,11u);goto L8c611554; /* fmov fr11,fr6 */
L8c611554: fcopy(f,0u,4u);goto L8c611556; /* fmov fr4,fr0 */
L8c611556: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[5],&f->fpscr,&fr[5]);if(status)return status;}goto L8c611558; /* fmac fr0,fr2,fr5 */
L8c611558: fcopy(f,2u,1u);goto L8c61155a; /* fmov fr1,fr2 */
L8c61155a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61155c; /* fmul fr3,fr2 */
L8c61155c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[8],&fr[7],&f->fpscr,&fr[7]);if(status)return status;}goto L8c61155e; /* fmac fr0,fr8,fr7 */
L8c61155e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[6],&f->fpscr,&fr[6]);if(status)return status;}goto L8c611560; /* fmac fr0,fr2,fr6 */
L8c611560: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[10],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c611562; /* fadd fr10,fr14 */
L8c611562: r[5]=r[9];goto L8c611564; /* mov r9,r5 */
L8c611564: fload(m,f,3u,r[5]);goto L8c611566; /* fmov.s @r5,fr3 */
L8c611566: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][7],&f->fpscr,sr,1);if(status)return status;goto L8c611568; /* fcmp/gt fr7,fr3 */
L8c611568: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[14],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61156a; /* fmul fr14,fr1 */
L8c61156a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[11],&f->fpscr,&fr[11]);if(status)return status;}goto L8c61156c; /* fmac fr0,fr1,fr11 */
L8c61156c: fcopy(f,4u,11u);goto L8c61156e; /* fmov fr11,fr4 */
L8c61156e: take=!(*sr&1u);r[4]=r[9];if(take)goto L8c611578;goto L8c611572; /* bf/s 0x8c611578 */
L8c611572: r[1]=rd(m,0x8c6115c0u,4);goto L8c611574; /* mov.l 0x8c6115c0,r1 */
L8c611574: target=r[1];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r1 */
L8c611578: r[0]=8u;goto L8c61157a; /* mov #0x8,r0 */
L8c61157a: fload(m,f,3u,r[0]+r[4]);goto L8c61157c; /* fmov.s @(r0,r4),fr3 */
L8c61157c: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61157e; /* fcmp/gt fr3,fr5 */
L8c61157e: if(!(*sr&1u))goto L8c611586;goto L8c611580; /* bf 0x8c611586 */
L8c611580: r[1]=rd(m,0x8c6115c0u,4);goto L8c611582; /* mov.l 0x8c6115c0,r1 */
L8c611582: target=r[1];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r1 */
L8c611586: r[0]=4u;goto L8c611588; /* mov #0x4,r0 */
L8c611588: fload(m,f,3u,r[0]+r[5]);goto L8c61158a; /* fmov.s @(r0,r5),fr3 */
L8c61158a: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c61158c; /* fcmp/gt fr4,fr3 */
L8c61158c: if(!(*sr&1u))goto L8c611594;goto L8c61158e; /* bf 0x8c611594 */
L8c61158e: r[1]=rd(m,0x8c6115c0u,4);goto L8c611590; /* mov.l 0x8c6115c0,r1 */
L8c611590: target=r[1];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r1 */
L8c611594: r[0]=12u;goto L8c611596; /* mov #0xc,r0 */
L8c611596: fload(m,f,3u,r[0]+r[4]);goto L8c611598; /* fmov.s @(r0,r4),fr3 */
L8c611598: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61159a; /* fcmp/gt fr3,fr6 */
L8c61159a: if(!(*sr&1u))goto L8c6115a2;goto L8c61159c; /* bf 0x8c6115a2 */
L8c61159c: r[1]=rd(m,0x8c6115c0u,4);goto L8c61159e; /* mov.l 0x8c6115c0,r1 */
L8c61159e: target=r[1];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r1 */
L8c6115a2: r[2]=rd(m,r[15],4);goto L8c6115a4; /* mov.l @r15,r2 */
L8c6115a4: r[4]=rd(m,0x8c6115d0u,4);goto L8c6115a6; /* mov.l 0x8c6115d0,r4 */
L8c6115a6: if(((r[2]+8u)&0x1f000000u)!=0x0c000000u)return -3;r[3]=rd(m,r[2]+0x8u,4);goto L8c6115a8; /* mov.l @(0x8,r2),r3 */
L8c6115a8: r[1]=rd(m,r[4],4);goto L8c6115aa; /* mov.l @r4,r1 */
L8c6115aa: r[2]=rd(m,0x8c6115d4u,4);goto L8c6115ac; /* mov.l 0x8c6115d4,r2 */
L8c6115ac: r[1]+=r[3];goto L8c6115ae; /* add r3,r1 */
L8c6115ae: r[5]=rd(m,0x8c6115d8u,4);goto L8c6115b0; /* mov.l 0x8c6115d8,r5 */
L8c6115b0: wr(m,r[4],r[1],4);goto L8c6115b2; /* mov.l r1,@r4 */
L8c6115b2: r[4]=rd(m,0x8c6115dcu,4);goto L8c6115b4; /* mov.l 0x8c6115dc,r4 */
L8c6115b4: fload(m,f,4u,r[2]);goto L8c6115b6; /* fmov.s @r2,fr4 */
L8c6115b6: goto L8c6115e0; /* bra 0x8c6115e0 */
L8c6115e0: fcopy(f,11u,4u);goto L8c6115e2; /* fmov fr4,fr11 */
L8c6115e2: fload(m,f,0u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6115e4; /* fmov.s @r5+,fr0 */
L8c6115e4: fload(m,f,1u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6115e6; /* fmov.s @r5+,fr1 */
L8c6115e6: fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6115e8; /* fmov.s @r5+,fr2 */
L8c6115e8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6115ea; /* fldi0 fr3 */
L8c6115ea: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6115ec; /* fschg */
L8c6115ec: fcopy(f,4u,1u);goto L8c6115ee; /* fmov fr1,fr4 */
L8c6115ee: fcopy(f,6u,3u);goto L8c6115f0; /* fmov fr3,fr6 */
L8c6115f0: status=sa_sh4_vector_numeric(0xf4edu,f,*fpul);if(status)return status;goto L8c6115f2; /* fipr fv0,fv4 */
L8c6115f2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6115f4; /* fschg */
L8c6115f4: fcopy(f,8u,7u);goto L8c6115f6; /* fmov fr7,fr8 */
L8c6115f6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6115f8; /* fschg */
L8c6115f8: fcopy(f,4u,5u);goto L8c6115fa; /* fmov fr5,fr4 */
L8c6115fa: fcopy(f,6u,7u);goto L8c6115fc; /* fmov fr7,fr6 */
L8c6115fc: status=sa_sh4_vector_numeric(0xf4edu,f,*fpul);if(status)return status;goto L8c6115fe; /* fipr fv0,fv4 */
L8c6115fe: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c611600; /* fschg */
L8c611600: fcopy(f,9u,7u);goto L8c611602; /* fmov fr7,fr9 */
L8c611602: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c611604; /* fschg */
L8c611604: fcopy(f,4u,9u);goto L8c611606; /* fmov fr9,fr4 */
L8c611606: fcopy(f,6u,11u);goto L8c611608; /* fmov fr11,fr6 */
L8c611608: status=sa_sh4_vector_numeric(0xf4edu,f,*fpul);if(status)return status;goto L8c61160a; /* fipr fv0,fv4 */
L8c61160a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61160c; /* fschg */
L8c61160c: fcopy(f,10u,7u);goto L8c61160e; /* fmov fr7,fr10 */
L8c61160e: fcopy(f,0u,8u);goto L8c611610; /* fmov fr8,fr0 */
L8c611610: fcopy(f,1u,9u);goto L8c611612; /* fmov fr9,fr1 */
L8c611612: fcopy(f,2u,10u);goto L8c611614; /* fmov fr10,fr2 */
L8c611614: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c611616; /* fipr fv8,fv0 */
L8c611616: r[4]+=12u;goto L8c611618; /* add #0xc,r4 */
L8c611618: status=sa_sh4_vector_numeric(0xf37du,f,*fpul);if(status)return status;goto L8c61161a; /* fsrra fr3 */
L8c61161a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[11],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61161c; /* fmul fr11,fr3 */
L8c61161c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c61161e; /* fmul fr3,fr10 */
L8c61161e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c611620; /* fmul fr3,fr9 */
L8c611620: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c611622; /* fmul fr3,fr8 */
L8c611622: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[4]);goto L8c611624; /* fmov.s fr10,@-r4 */
L8c611624: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[4]);goto L8c611626; /* fmov.s fr9,@-r4 */
L8c611626: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[4]);goto L8c611628; /* fmov.s fr8,@-r4 */
L8c611628: r[2]=rd(m,r[15],4);goto L8c61162a; /* mov.l @r15,r2 */
L8c61162a: r[0]=40u;goto L8c61162c; /* mov #0x28,r0 */
L8c61162c: r[1]=rd(m,0x8c6116f0u,4);goto L8c61162e; /* mov.l 0x8c6116f0,r1 */
L8c61162e: if(((r[2]+8u)&0x1f000000u)!=0x0c000000u)return -3;r[3]=rd(m,r[2]+0x8u,4);goto L8c611630; /* mov.l @(0x8,r2),r3 */
L8c611630: r[2]=rd(m,0x8c6116ecu,4);goto L8c611632; /* mov.l 0x8c6116ec,r2 */
L8c611632: wr(m,r[15]+0x10u,r[3],4);goto L8c611634; /* mov.l r3,@(0x10,r15) */
L8c611634: r[3]=rd(m,0x8c6116e8u,4);goto L8c611636; /* mov.l 0x8c6116e8,r3 */
L8c611636: wr(m,r[15]+0x14u,r[13],4);goto L8c611638; /* mov.l r13,@(0x14,r15) */
L8c611638: wr(m,r[15]+0x18u,r[12],4);goto L8c61163a; /* mov.l r12,@(0x18,r15) */
L8c61163a: fload(m,f,3u,r[3]);goto L8c61163c; /* fmov.s @r3,fr3 */
L8c61163c: fstore(m,f,3u,r[0]+r[15]);goto L8c61163e; /* fmov fr3,@(r0,r15) */
L8c61163e: r[0]=44u;goto L8c611640; /* mov #0x2c,r0 */
L8c611640: fload(m,f,3u,r[2]);goto L8c611642; /* fmov.s @r2,fr3 */
L8c611642: fstore(m,f,3u,r[0]+r[15]);goto L8c611644; /* fmov fr3,@(r0,r15) */
L8c611644: r[0]=rd(m,r[1],4);goto L8c611646; /* mov.l @r1,r0 */
L8c611646: *sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c611648; /* tst #0x2,r0 */
L8c611648: if((*sr&1u))goto L8c61166c;goto L8c61164a; /* bt 0x8c61166c */
L8c61164a: *sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c61164c; /* tst r14,r14 */
L8c61164c: if((*sr&1u))goto L8c61165e;goto L8c61164e; /* bt 0x8c61165e */
L8c61164e: r[3]=rd(m,0x8c6116f4u,4);goto L8c611650; /* mov.l 0x8c6116f4,r3 */
L8c611650: r[4]=r[15];goto L8c611652; /* mov r15,r4 */
L8c611652: wr(m,r[15]+0x1cu,r[14],4);goto L8c611654; /* mov.l r14,@(0x1c,r15) */
L8c611654: r[5]=rd(m,r[10],4);goto L8c611656; /* mov.l @r10,r5 */
L8c611656: target=r[3];*pr=0x8c61165au;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61165a; /* jsr @r3 */
L8c61165a: goto L8c6116d2; /* bra 0x8c6116d2 */
L8c61165e: r[3]=rd(m,0x8c6116f8u,4);goto L8c611660; /* mov.l 0x8c6116f8,r3 */
L8c611660: r[4]=r[15];goto L8c611662; /* mov r15,r4 */
L8c611662: r[5]=rd(m,r[10],4);goto L8c611664; /* mov.l @r10,r5 */
L8c611664: target=r[3];*pr=0x8c611668u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c611668; /* jsr @r3 */
L8c611668: goto L8c6116d2; /* bra 0x8c6116d2 */
L8c61166c: r[4]=r[9];goto L8c61166e; /* mov r9,r4 */
L8c61166e: r[0]=48u;goto L8c611670; /* mov #0x30,r0 */
L8c611670: r[5]=r[4];goto L8c611672; /* mov r4,r5 */
L8c611672: r[5]+=8u;goto L8c611674; /* add #0x8,r5 */
L8c611674: r[3]=4294967295u;goto L8c611676; /* mov #-0x1,r3 */
L8c611676: wr(m,r[15]+0x20u,r[3],4);goto L8c611678; /* mov.l r3,@(0x20,r15) */
L8c611678: r[3]=0u;goto L8c61167a; /* mov #0x0,r3 */
L8c61167a: *sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c61167c; /* tst r14,r14 */
L8c61167c: wr(m,r[15]+0x24u,r[3],4);goto L8c61167e; /* mov.l r3,@(0x24,r15) */
L8c61167e: fload(m,f,3u,r[4]);goto L8c611680; /* fmov.s @r4,fr3 */
L8c611680: fstore(m,f,3u,r[0]+r[15]);goto L8c611682; /* fmov fr3,@(r0,r15) */
L8c611682: r[0]=52u;goto L8c611684; /* mov #0x34,r0 */
L8c611684: fload(m,f,3u,r[5]);goto L8c611686; /* fmov.s @r5,fr3 */
L8c611686: fstore(m,f,3u,r[0]+r[15]);goto L8c611688; /* fmov fr3,@(r0,r15) */
L8c611688: r[0]=4u;goto L8c61168a; /* mov #0x4,r0 */
L8c61168a: fload(m,f,3u,r[0]+r[4]);goto L8c61168c; /* fmov.s @(r0,r4),fr3 */
L8c61168c: r[0]=56u;goto L8c61168e; /* mov #0x38,r0 */
L8c61168e: fstore(m,f,3u,r[0]+r[15]);goto L8c611690; /* fmov fr3,@(r0,r15) */
L8c611690: r[0]=4u;goto L8c611692; /* mov #0x4,r0 */
L8c611692: fload(m,f,3u,r[0]+r[5]);goto L8c611694; /* fmov.s @(r0,r5),fr3 */
L8c611694: r[0]=60u;goto L8c611696; /* mov #0x3c,r0 */
L8c611696: take=(*sr&1u);fstore(m,f,3u,r[0]+r[15]);if(take)goto L8c6116aa;goto L8c61169a; /* bt/s 0x8c6116aa */
L8c61169a: r[3]=rd(m,0x8c6116fcu,4);goto L8c61169c; /* mov.l 0x8c6116fc,r3 */
L8c61169c: r[4]=r[15];goto L8c61169e; /* mov r15,r4 */
L8c61169e: wr(m,r[15]+0x1cu,r[14],4);goto L8c6116a0; /* mov.l r14,@(0x1c,r15) */
L8c6116a0: r[5]=rd(m,r[10],4);goto L8c6116a2; /* mov.l @r10,r5 */
L8c6116a2: target=r[3];*pr=0x8c6116a6u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6116a6; /* jsr @r3 */
L8c6116a6: goto L8c6116b4; /* bra 0x8c6116b4 */
L8c6116aa: r[3]=rd(m,0x8c611700u,4);goto L8c6116ac; /* mov.l 0x8c611700,r3 */
L8c6116ac: r[4]=r[15];goto L8c6116ae; /* mov r15,r4 */
L8c6116ae: r[5]=rd(m,r[10],4);goto L8c6116b0; /* mov.l @r10,r5 */
L8c6116b0: target=r[3];*pr=0x8c6116b4u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6116b4; /* jsr @r3 */
L8c6116b4: r[4]=rd(m,r[15]+0x20u,4);goto L8c6116b6; /* mov.l @(0x20,r15),r4 */
L8c6116b6: r[3]=60u;goto L8c6116b8; /* mov #0x3c,r3 */
L8c6116b8: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6116ba; /* tst r4,r3 */
L8c6116ba: if(!(*sr&1u))goto L8c6116c8;goto L8c6116bc; /* bf 0x8c6116c8 */
L8c6116bc: r[3]=2u;goto L8c6116be; /* mov #0x2,r3 */
L8c6116be: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6116c0; /* tst r4,r3 */
L8c6116c0: if(!(*sr&1u))goto L8c6116c8;goto L8c6116c2; /* bf 0x8c6116c8 */
L8c6116c2: r[3]=1u;goto L8c6116c4; /* mov #0x1,r3 */
L8c6116c4: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c6116c6; /* tst r3,r4 */
L8c6116c6: if((*sr&1u))goto L8c6116cc;goto L8c6116c8; /* bt 0x8c6116cc */
L8c6116c8: r[0]=4294967295u;goto L8c6116d4; /* bra 0x8c6116d4 */
L8c6116cc: r[2]=rd(m,0x8c611704u,4);goto L8c6116ce; /* mov.l 0x8c611704,r2 */
L8c6116ce: r[3]=rd(m,r[15]+0x24u,4);goto L8c6116d0; /* mov.l @(0x24,r15),r3 */
L8c6116d0: wr(m,r[2],r[3],4);goto L8c6116d2; /* mov.l r3,@r2 */
L8c6116d2: r[0]=0u;goto L8c6116d4; /* mov #0x0,r0 */
L8c6116d4: r[15]+=64u;goto L8c6116d6; /* add #0x40,r15 */
L8c6116d6: *pr=rd(m,r[15],4);r[15]+=4;goto L8c6116d8; /* lds.l @r15+,PR */
L8c6116d8: fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6116da; /* fmov.s @r15+,fr14 */
L8c6116da: r[9]=rd(m,r[15],4);r[15]+=4;goto L8c6116dc; /* mov.l @r15+,r9 */
L8c6116dc: r[10]=rd(m,r[15],4);r[15]+=4;goto L8c6116de; /* mov.l @r15+,r10 */
L8c6116de: r[11]=rd(m,r[15],4);r[15]+=4;goto L8c6116e0; /* mov.l @r15+,r11 */
L8c6116e0: r[12]=rd(m,r[15],4);r[15]+=4;goto L8c6116e2; /* mov.l @r15+,r12 */
L8c6116e2: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c6116e4; /* mov.l @r15+,r13 */
L8c6116e4: if(*pr!=entry_pr)return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_flags_8c606c56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c606c56;
L8c606c56: r[6]=rd(m,0x8c606ca8u,4);goto L8c606c58; /* mov.l 0x8c606ca8,r6 */
L8c606c58: r[2]=rd(m,0x8c606cb8u,4);goto L8c606c5a; /* mov.l 0x8c606cb8,r2 */
L8c606c5a: r[5]=rd(m,r[6]+0x8u,4);goto L8c606c5c; /* mov.l @(0x8,r6),r5 */
L8c606c5c: r[3]=rd(m,0x8c606cb4u,4);goto L8c606c5e; /* mov.l 0x8c606cb4,r3 */
L8c606c5e: r[4]&=r[2];goto L8c606c60; /* and r2,r4 */
L8c606c60: r[5]&=r[3];goto L8c606c62; /* and r3,r5 */
L8c606c62: r[5]|=r[4];goto L8c606c64; /* or r4,r5 */
L8c606c64: wr(m,r[6]+0x8u,r[5],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_shader_state_8c606a08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c606a08;
L8c606a08: r[15]-=4;wr(m,r[15],r[14],4);goto L8c606a0a; /* mov.l r14,@-r15 */
L8c606a0a: r[14]=rd(m,0x8c606b20u,4);goto L8c606a0c; /* mov.l 0x8c606b20,r14 */
L8c606a0c: r[3]=rd(m,0x8c606b24u,4);goto L8c606a0e; /* mov.l 0x8c606b24,r3 */
L8c606a0e: r[1]=rd(m,0x8c606b2cu,4);goto L8c606a10; /* mov.l 0x8c606b2c,r1 */
L8c606a10: r[2]=rd(m,0x8c606b28u,4);goto L8c606a12; /* mov.l 0x8c606b28,r2 */
L8c606a12: r[1]&=r[4];goto L8c606a14; /* and r4,r1 */
L8c606a14: r[15]-=4;wr(m,r[15],r[13],4);goto L8c606a16; /* mov.l r13,@-r15 */
L8c606a16: r[5]=rd(m,r[14],4);goto L8c606a18; /* mov.l @r14,r5 */
L8c606a18: r[6]=rd(m,r[14]+0x8u,4);goto L8c606a1a; /* mov.l @(0x8,r14),r6 */
L8c606a1a: r[5]&=r[3];goto L8c606a1c; /* and r3,r5 */
L8c606a1c: r[3]=rd(m,0x8c606b30u,4);goto L8c606a1e; /* mov.l 0x8c606b30,r3 */
L8c606a1e: r[5]|=r[1];goto L8c606a20; /* or r1,r5 */
L8c606a20: r[1]=rd(m,0x8c606b34u,4);goto L8c606a22; /* mov.l 0x8c606b34,r1 */
L8c606a22: r[3]&=r[4];goto L8c606a24; /* and r4,r3 */
L8c606a24: r[6]&=r[2];goto L8c606a26; /* and r2,r6 */
L8c606a26: r[13]=rd(m,r[1],4);goto L8c606a28; /* mov.l @r1,r13 */
L8c606a28: r[6]|=r[3];goto L8c606a2a; /* or r3,r6 */
L8c606a2a: r[3]=sx(m,0x8c606b14u,2);goto L8c606a2c; /* mov.w 0x8c606b14,r3 */
L8c606a2c: *sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c606a2e; /* tst r13,r3 */
L8c606a2e: take=!(*sr&1u);r[7]=rd(m,r[14]+0x4u,4);if(take)goto L8c606a3e;goto L8c606a32; /* bf/s 0x8c606a3e */
L8c606a32: r[0]=sx(m,0x8c606b16u,2);goto L8c606a34; /* mov.w 0x8c606b16,r0 */
L8c606a34: *sr=(*sr&~1u)|!(r[0]&r[13]);goto L8c606a36; /* tst r0,r13 */
L8c606a36: if((*sr&1u))goto L8c606a44;goto L8c606a38; /* bt 0x8c606a44 */
L8c606a38: r[2]=rd(m,0x8c606b38u,4);goto L8c606a3a; /* mov.l 0x8c606b38,r2 */
L8c606a3a: *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c606a3c; /* tst r4,r2 */
L8c606a3c: if(!(*sr&1u))goto L8c606a44;goto L8c606a3e; /* bf 0x8c606a44 */
L8c606a3e: r[1]=sx(m,0x8c606b18u,2);goto L8c606a40; /* mov.w 0x8c606b18,r1 */
L8c606a40: r[5]|=r[1];goto L8c606a48; /* bra 0x8c606a48 */
L8c606a44: r[2]=sx(m,0x8c606b1au,2);goto L8c606a46; /* mov.w 0x8c606b1a,r2 */
L8c606a46: r[5]&=r[2];goto L8c606a48; /* and r2,r5 */
L8c606a48: r[2]=rd(m,0x8c606b3cu,4);goto L8c606a4a; /* mov.l 0x8c606b3c,r2 */
L8c606a4a: wr(m,r[14],r[5],4);goto L8c606a4c; /* mov.l r5,@r14 */
L8c606a4c: wr(m,r[14]+0x4u,r[7],4);goto L8c606a4e; /* mov.l r7,@(0x4,r14) */
L8c606a4e: wr(m,r[14]+0x8u,r[6],4);goto L8c606a50; /* mov.l r6,@(0x8,r14) */
L8c606a50: r[3]=rd(m,r[2],4);goto L8c606a52; /* mov.l @r2,r3 */
L8c606a52: wr(m,r[3],r[4],4);goto L8c606a54; /* mov.l r4,@r3 */
L8c606a54: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c606a56; /* mov.l @r15+,r13 */
L8c606a56: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_texture_state_8c606a88(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c606a88;
L8c606a88: r[2]=rd(m,0x8c606b40u,4);goto L8c606a8a; /* mov.l 0x8c606b40,r2 */
L8c606a8a: r[1]=r[4];goto L8c606a8c; /* mov r4,r1 */
L8c606a8c: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c606a8e; /* shll r4 */
L8c606a8e: r[5]=rd(m,0x8c606b44u,4);goto L8c606a90; /* mov.l 0x8c606b44,r5 */
L8c606a90: r[3]=rd(m,r[2],4);goto L8c606a92; /* mov.l @r2,r3 */
L8c606a92: r[4]+=r[1];goto L8c606a94; /* add r1,r4 */
L8c606a94: r[4]<<=2;goto L8c606a96; /* shll2 r4 */
L8c606a96: r[1]=rd(m,r[5],4);goto L8c606a98; /* mov.l @r5,r1 */
L8c606a98: r[3]=rd(m,r[3],4);goto L8c606a9a; /* mov.l @r3,r3 */
L8c606a9a: r[4]+=r[3];goto L8c606a9c; /* add r3,r4 */
L8c606a9c: r[4]=rd(m,r[4]+0x8u,4);goto L8c606a9e; /* mov.l @(0x8,r4),r4 */
L8c606a9e: r[3]=rd(m,r[4],4);goto L8c606aa0; /* mov.l @r4,r3 */
L8c606aa0: *sr=(*sr&~1u)|(r[1]==r[3]);goto L8c606aa2; /* cmp/eq r1,r3 */
L8c606aa2: if(!(*sr&1u))goto L8c606aae;goto L8c606aa4; /* bf 0x8c606aae */
L8c606aa4: r[1]=rd(m,0x8c606b48u,4);goto L8c606aa6; /* mov.l 0x8c606b48,r1 */
L8c606aa6: r[3]=rd(m,r[4]+0x4u,4);goto L8c606aa8; /* mov.l @(0x4,r4),r3 */
L8c606aa8: r[0]=rd(m,r[1],4);goto L8c606aaa; /* mov.l @r1,r0 */
L8c606aaa: *sr=(*sr&~1u)|(r[3]==r[0]);goto L8c606aac; /* cmp/eq r3,r0 */
L8c606aac: if((*sr&1u))goto L8c606ada;goto L8c606aae; /* bt 0x8c606ada */
L8c606aae: r[2]=rd(m,r[4],4);goto L8c606ab0; /* mov.l @r4,r2 */
L8c606ab0: r[1]=rd(m,0x8c606b48u,4);goto L8c606ab2; /* mov.l 0x8c606b48,r1 */
L8c606ab2: r[6]=rd(m,0x8c606b20u,4);goto L8c606ab4; /* mov.l 0x8c606b20,r6 */
L8c606ab4: wr(m,r[5],r[2],4);goto L8c606ab6; /* mov.l r2,@r5 */
L8c606ab6: r[3]=rd(m,r[4]+0x4u,4);goto L8c606ab8; /* mov.l @(0x4,r4),r3 */
L8c606ab8: r[2]=rd(m,0x8c606b4cu,4);goto L8c606aba; /* mov.l 0x8c606b4c,r2 */
L8c606aba: wr(m,r[1],r[3],4);goto L8c606abc; /* mov.l r3,@r1 */
L8c606abc: r[3]=4294967232u;goto L8c606abe; /* mov #-0x40,r3 */
L8c606abe: r[0]=rd(m,r[4]+0x8u,4);goto L8c606ac0; /* mov.l @(0x8,r4),r0 */
L8c606ac0: r[5]=rd(m,r[6]+0x8u,4);goto L8c606ac2; /* mov.l @(0x8,r6),r5 */
L8c606ac2: r[1]=rd(m,0x8c606b50u,4);goto L8c606ac4; /* mov.l 0x8c606b50,r1 */
L8c606ac4: r[0]&=0x3fu;goto L8c606ac6; /* and #0x3f,r0 */
L8c606ac6: r[5]&=r[3];goto L8c606ac8; /* and r3,r5 */
L8c606ac8: r[5]|=r[0];goto L8c606aca; /* or r0,r5 */
L8c606aca: wr(m,r[6]+0x8u,r[5],4);goto L8c606acc; /* mov.l r5,@(0x8,r6) */
L8c606acc: r[4]=rd(m,r[4]+0xcu,4);goto L8c606ace; /* mov.l @(0xc,r4),r4 */
L8c606ace: r[2]&=r[4];goto L8c606ad0; /* and r4,r2 */
L8c606ad0: *sr=(*sr&~1u)|(r[2]>r[1]);goto L8c606ad2; /* cmp/hi r1,r2 */
L8c606ad2: if(!(*sr&1u))goto L8c606ad8;goto L8c606ad4; /* bf 0x8c606ad8 */
L8c606ad4: r[0]=4294967295u;return 0; /* rts */
L8c606ad8: wr(m,r[6]+0xcu,r[4],4);goto L8c606ada; /* mov.l r4,@(0xc,r6) */
L8c606ada: r[0]=1u;goto L8c606adc; /* mov #0x1,r0 */
L8c606adc: return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_command_finish_8c6069a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6069a4;
L8c6069a4: r[2]=rd(m,0x8c6069d4u,4);goto L8c6069a6; /* mov.l 0x8c6069d4,r2 */
L8c6069a6: r[1]=rd(m,0x8c6069ccu,4);goto L8c6069a8; /* mov.l 0x8c6069cc,r1 */
L8c6069a8: r[3]=rd(m,r[2],4);goto L8c6069aa; /* mov.l @r2,r3 */
L8c6069aa: r[2]=rd(m,0x8c6069c0u,4);goto L8c6069ac; /* mov.l 0x8c6069c0,r2 */
L8c6069ac: r[4]&=r[1];goto L8c6069ae; /* and r1,r4 */
L8c6069ae: r[0]=rd(m,r[2],4);goto L8c6069b0; /* mov.l @r2,r0 */
L8c6069b0: r[4]|=r[0];goto L8c6069b2; /* or r0,r4 */
L8c6069b2: wr(m,r[3],r[4],4);return 0; /* rts */
}
