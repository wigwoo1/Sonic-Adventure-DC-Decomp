#include "sa_draw_motion_stop.h"
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
L8c611444: sa_motion_publish_stop(ctx,0x8c611444u); r[15]-=4;wr(m,r[15],r[14],4);goto L8c611446; /* mov.l r14,@-r15 */
L8c611446: sa_motion_publish_stop(ctx,0x8c611446u); r[15]-=4;wr(m,r[15],r[13],4);goto L8c611448; /* mov.l r13,@-r15 */
L8c611448: sa_motion_publish_stop(ctx,0x8c611448u); r[15]-=4;wr(m,r[15],r[12],4);goto L8c61144a; /* mov.l r12,@-r15 */
L8c61144a: sa_motion_publish_stop(ctx,0x8c61144au); r[15]-=4;wr(m,r[15],r[11],4);goto L8c61144c; /* mov.l r11,@-r15 */
L8c61144c: sa_motion_publish_stop(ctx,0x8c61144cu); r[15]-=4;wr(m,r[15],r[10],4);goto L8c61144e; /* mov.l r10,@-r15 */
L8c61144e: sa_motion_publish_stop(ctx,0x8c61144eu); r[15]-=4;wr(m,r[15],r[9],4);goto L8c611450; /* mov.l r9,@-r15 */
L8c611450: sa_motion_publish_stop(ctx,0x8c611450u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto L8c611452; /* fmov.s fr14,@-r15 */
L8c611452: sa_motion_publish_stop(ctx,0x8c611452u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c611454; /* sts.l PR,@-r15 */
L8c611454: sa_motion_publish_stop(ctx,0x8c611454u); r[15]+=4294967232u;goto L8c611456; /* add #-0x40,r15 */
L8c611456: sa_motion_publish_stop(ctx,0x8c611456u); r[3]=rd(m,0x8c6114b0u,4);goto L8c611458; /* mov.l 0x8c6114b0,r3 */
L8c611458: sa_motion_publish_stop(ctx,0x8c611458u); r[2]=0u;goto L8c61145a; /* mov #0x0,r2 */
L8c61145a: sa_motion_publish_stop(ctx,0x8c61145au); r[1]=rd(m,0x8c6114b4u,4);goto L8c61145c; /* mov.l 0x8c6114b4,r1 */
L8c61145c: sa_motion_publish_stop(ctx,0x8c61145cu); r[14]=r[4];goto L8c61145e; /* mov r4,r14 */
L8c61145e: sa_motion_publish_stop(ctx,0x8c61145eu); r[11]=rd(m,0x8c6114c0u,4);goto L8c611460; /* mov.l 0x8c6114c0,r11 */
L8c611460: sa_motion_publish_stop(ctx,0x8c611460u); r[12]=r[4];goto L8c611462; /* mov r4,r12 */
L8c611462: sa_motion_publish_stop(ctx,0x8c611462u); r[0]=rd(m,0x8c6114bcu,4);goto L8c611464; /* mov.l 0x8c6114bc,r0 */
L8c611464: sa_motion_publish_stop(ctx,0x8c611464u); r[10]=rd(m,0x8c6114b8u,4);goto L8c611466; /* mov.l 0x8c6114b8,r10 */
L8c611466: sa_motion_publish_stop(ctx,0x8c611466u); r[9]=rd(m,0x8c6114d4u,4);goto L8c611468; /* mov.l 0x8c6114d4,r9 */
L8c611468: sa_motion_publish_stop(ctx,0x8c611468u); wr(m,r[15],r[4],4);goto L8c61146a; /* mov.l r4,@r15 */
L8c61146a: sa_motion_publish_stop(ctx,0x8c61146au); r[13]=rd(m,r[3],4);goto L8c61146c; /* mov.l @r3,r13 */
L8c61146c: sa_motion_publish_stop(ctx,0x8c61146cu); r[3]=rd(m,0x8c6114c4u,4);goto L8c61146e; /* mov.l 0x8c6114c4,r3 */
L8c61146e: sa_motion_publish_stop(ctx,0x8c61146eu); r[14]=rd(m,r[14]+0x4u,4);goto L8c611470; /* mov.l @(0x4,r14),r14 */
L8c611470: sa_motion_publish_stop(ctx,0x8c611470u); r[12]=rd(m,r[12],4);goto L8c611472; /* mov.l @r12,r12 */
L8c611472: sa_motion_publish_stop(ctx,0x8c611472u); wr(m,r[1],r[2],4);goto L8c611474; /* mov.l r2,@r1 */
L8c611474: sa_motion_publish_stop(ctx,0x8c611474u); r[2]=rd(m,r[0],4);goto L8c611476; /* mov.l @r0,r2 */
L8c611476: sa_motion_publish_stop(ctx,0x8c611476u); r[1]=rd(m,0x8c6114ccu,4);goto L8c611478; /* mov.l 0x8c6114cc,r1 */
L8c611478: sa_motion_publish_stop(ctx,0x8c611478u); wr(m,r[10],r[2],4);goto L8c61147a; /* mov.l r2,@r10 */
L8c61147a: sa_motion_publish_stop(ctx,0x8c61147au); fload(m,f,3u,r[3]);goto L8c61147c; /* fmov.s @r3,fr3 */
L8c61147c: sa_motion_publish_stop(ctx,0x8c61147cu); fload(m,f,2u,r[11]);goto L8c61147e; /* fmov.s @r11,fr2 */
L8c61147e: sa_motion_publish_stop(ctx,0x8c61147eu); r[2]=rd(m,0x8c6114c8u,4);goto L8c611480; /* mov.l 0x8c6114c8,r2 */
L8c611480: sa_motion_publish_stop(ctx,0x8c611480u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611482; /* fmul fr3,fr2 */
L8c611482: sa_motion_publish_stop(ctx,0x8c611482u); r[3]=rd(m,0x8c6114d0u,4);goto L8c611484; /* mov.l 0x8c6114d0,r3 */
L8c611484: sa_motion_publish_stop(ctx,0x8c611484u); fstore(m,f,2u,r[2]);goto L8c611486; /* fmov.s fr2,@r2 */
L8c611486: sa_motion_publish_stop(ctx,0x8c611486u); fload(m,f,2u,r[11]);goto L8c611488; /* fmov.s @r11,fr2 */
L8c611488: sa_motion_publish_stop(ctx,0x8c611488u); fload(m,f,3u,r[1]);goto L8c61148a; /* fmov.s @r1,fr3 */
L8c61148a: sa_motion_publish_stop(ctx,0x8c61148au); r[2]=sx(m,0x8c6114aeu,2);goto L8c61148c; /* mov.w 0x8c6114ae,r2 */
L8c61148c: sa_motion_publish_stop(ctx,0x8c61148cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61148e; /* fmul fr3,fr2 */
L8c61148e: sa_motion_publish_stop(ctx,0x8c61148eu); fstore(m,f,2u,r[3]);goto L8c611490; /* fmov.s fr2,@r3 */
L8c611490: sa_motion_publish_stop(ctx,0x8c611490u); r[3]=rd(m,0x8c6114d8u,4);goto L8c611492; /* mov.l 0x8c6114d8,r3 */
L8c611492: sa_motion_publish_stop(ctx,0x8c611492u); r[0]=rd(m,r[3],4);goto L8c611494; /* mov.l @r3,r0 */
L8c611494: sa_motion_publish_stop(ctx,0x8c611494u); *sr=(*sr&~1u)|!(r[2]&r[0]);goto L8c611496; /* tst r2,r0 */
L8c611496: sa_motion_publish_stop(ctx,0x8c611496u); if(!(*sr&1u))goto L8c61149e;goto L8c611498; /* bf 0x8c61149e */
L8c611498: sa_motion_publish_stop(ctx,0x8c611498u); r[1]=rd(m,0x8c6114dcu,4);goto L8c61149a; /* mov.l 0x8c6114dc,r1 */
L8c61149a: sa_motion_publish_stop(ctx,0x8c61149cu); target=r[1];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r1 */
L8c61149e: sa_motion_publish_stop(ctx,0x8c61149eu); r[5]=rd(m,r[15],4);goto L8c6114a0; /* mov.l @r15,r5 */
L8c6114a0: sa_motion_publish_stop(ctx,0x8c6114a0u); r[4]=r[15];goto L8c6114a2; /* mov r15,r4 */
L8c6114a2: sa_motion_publish_stop(ctx,0x8c6114a2u); r[4]+=4u;goto L8c6114a4; /* add #0x4,r4 */
L8c6114a4: sa_motion_publish_stop(ctx,0x8c6114a4u); r[5]+=24u;goto L8c6114a6; /* add #0x18,r5 */
L8c6114a6: sa_motion_publish_stop(ctx,0x8c6114a8u); goto L8c6114e0; /* bra 0x8c6114e0 */
L8c6114e0: sa_motion_publish_stop(ctx,0x8c6114e0u); fload(m,f,0u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6114e2; /* fmov.s @r5+,fr0 */
L8c6114e2: sa_motion_publish_stop(ctx,0x8c6114e2u); fload(m,f,1u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6114e4; /* fmov.s @r5+,fr1 */
L8c6114e4: sa_motion_publish_stop(ctx,0x8c6114e4u); fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6114e6; /* fmov.s @r5+,fr2 */
L8c6114e6: sa_motion_publish_stop(ctx,0x8c6114e6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6114e8; /* fldi1 fr3 */
L8c6114e8: sa_motion_publish_stop(ctx,0x8c6114e8u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6114ea; /* ftrv xmtrx,fv0 */
L8c6114ea: sa_motion_publish_stop(ctx,0x8c6114eau); r[4]+=12u;goto L8c6114ec; /* add #0xc,r4 */
L8c6114ec: sa_motion_publish_stop(ctx,0x8c6114ecu); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[4]);goto L8c6114ee; /* fmov.s fr2,@-r4 */
L8c6114ee: sa_motion_publish_stop(ctx,0x8c6114eeu); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[4]);goto L8c6114f0; /* fmov.s fr1,@-r4 */
L8c6114f0: sa_motion_publish_stop(ctx,0x8c6114f0u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[4]);goto L8c6114f2; /* fmov.s fr0,@-r4 */
L8c6114f2: sa_motion_publish_stop(ctx,0x8c6114f2u); r[2]=rd(m,r[15],4);goto L8c6114f4; /* mov.l @r15,r2 */
L8c6114f4: sa_motion_publish_stop(ctx,0x8c6114f4u); r[0]=36u;goto L8c6114f6; /* mov #0x24,r0 */
L8c6114f6: sa_motion_publish_stop(ctx,0x8c6114f6u); r[3]=rd(m,0x8c6115bcu,4);goto L8c6114f8; /* mov.l 0x8c6115bc,r3 */
L8c6114f8: sa_motion_publish_stop(ctx,0x8c6114f8u); if(((r[0]+r[2])&0x1f000000u)!=0x0c000000u)return -3;fload(m,f,10u,r[0]+r[2]);goto L8c6114fa; /* fmov.s @(r0,r2),fr10 */
L8c6114fa: sa_motion_publish_stop(ctx,0x8c6114fau); r[0]=12u;goto L8c6114fc; /* mov #0xc,r0 */
L8c6114fc: sa_motion_publish_stop(ctx,0x8c6114fcu); fload(m,f,4u,r[0]+r[15]);goto L8c6114fe; /* fmov.s @(r0,r15),fr4 */
L8c6114fe: sa_motion_publish_stop(ctx,0x8c6114feu); fload(m,f,2u,r[3]);goto L8c611500; /* fmov.s @r3,fr2 */
L8c611500: sa_motion_publish_stop(ctx,0x8c611500u); fcopy(f,3u,4u);goto L8c611502; /* fmov fr4,fr3 */
L8c611502: sa_motion_publish_stop(ctx,0x8c611502u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c611504; /* fadd fr10,fr3 */
L8c611504: sa_motion_publish_stop(ctx,0x8c611504u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c611506; /* fcmp/gt fr2,fr3 */
L8c611506: sa_motion_publish_stop(ctx,0x8c611506u); if((*sr&1u))goto L8c61150e;goto L8c611508; /* bt 0x8c61150e */
L8c611508: sa_motion_publish_stop(ctx,0x8c611508u); r[2]=rd(m,0x8c6115c0u,4);goto L8c61150a; /* mov.l 0x8c6115c0,r2 */
L8c61150a: sa_motion_publish_stop(ctx,0x8c61150cu); target=r[2];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r2 */
L8c61150e: sa_motion_publish_stop(ctx,0x8c61150eu); fcopy(f,3u,4u);goto L8c611510; /* fmov fr4,fr3 */
L8c611510: sa_motion_publish_stop(ctx,0x8c611510u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611512; /* fsub fr10,fr3 */
L8c611512: sa_motion_publish_stop(ctx,0x8c611512u); r[3]=rd(m,0x8c6115c4u,4);goto L8c611514; /* mov.l 0x8c6115c4,r3 */
L8c611514: sa_motion_publish_stop(ctx,0x8c611514u); fload(m,f,2u,r[3]);goto L8c611516; /* fmov.s @r3,fr2 */
L8c611516: sa_motion_publish_stop(ctx,0x8c611516u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c611518; /* fcmp/gt fr2,fr3 */
L8c611518: sa_motion_publish_stop(ctx,0x8c611518u); if(!(*sr&1u))goto L8c611520;goto L8c61151a; /* bf 0x8c611520 */
L8c61151a: sa_motion_publish_stop(ctx,0x8c61151au); r[2]=rd(m,0x8c6115c0u,4);goto L8c61151c; /* mov.l 0x8c6115c0,r2 */
L8c61151c: sa_motion_publish_stop(ctx,0x8c61151eu); target=r[2];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r2 */
L8c611520: sa_motion_publish_stop(ctx,0x8c611520u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[10],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c611522; /* fadd fr10,fr4 */
L8c611522: sa_motion_publish_stop(ctx,0x8c611522u); r[0]=4u;goto L8c611524; /* mov #0x4,r0 */
L8c611524: sa_motion_publish_stop(ctx,0x8c611524u); fload(m,f,9u,r[0]+r[15]);goto L8c611526; /* fmov.s @(r0,r15),fr9 */
L8c611526: sa_motion_publish_stop(ctx,0x8c611526u); r[0]=12u;goto L8c611528; /* mov #0xc,r0 */
L8c611528: sa_motion_publish_stop(ctx,0x8c611528u); r[3]=rd(m,0x8c6115c8u,4);goto L8c61152a; /* mov.l 0x8c6115c8,r3 */
L8c61152a: sa_motion_publish_stop(ctx,0x8c61152au); fload(m,f,7u,r[0]+r[11]);goto L8c61152c; /* fmov.s @(r0,r11),fr7 */
L8c61152c: sa_motion_publish_stop(ctx,0x8c61152cu); r[0]=8u;goto L8c61152e; /* mov #0x8,r0 */
L8c61152e: sa_motion_publish_stop(ctx,0x8c61152eu); fcopy(f,3u,4u);goto L8c611530; /* fmov fr4,fr3 */
L8c611530: sa_motion_publish_stop(ctx,0x8c611530u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c611532; /* fldi1 fr4 */
L8c611532: sa_motion_publish_stop(ctx,0x8c611532u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c611534; /* fdiv fr3,fr4 */
L8c611534: sa_motion_publish_stop(ctx,0x8c611534u); fcopy(f,3u,9u);goto L8c611536; /* fmov fr9,fr3 */
L8c611536: sa_motion_publish_stop(ctx,0x8c611536u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611538; /* fsub fr10,fr3 */
L8c611538: sa_motion_publish_stop(ctx,0x8c611538u); fload(m,f,8u,r[3]);goto L8c61153a; /* fmov.s @r3,fr8 */
L8c61153a: sa_motion_publish_stop(ctx,0x8c61153au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[10],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c61153c; /* fadd fr10,fr9 */
L8c61153c: sa_motion_publish_stop(ctx,0x8c61153cu); fload(m,f,14u,r[0]+r[15]);goto L8c61153e; /* fmov.s @(r0,r15),fr14 */
L8c61153e: sa_motion_publish_stop(ctx,0x8c61153eu); r[0]=16u;goto L8c611540; /* mov #0x10,r0 */
L8c611540: sa_motion_publish_stop(ctx,0x8c611540u); fcopy(f,2u,8u);goto L8c611542; /* fmov fr8,fr2 */
L8c611542: sa_motion_publish_stop(ctx,0x8c611542u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611544; /* fmul fr3,fr2 */
L8c611544: sa_motion_publish_stop(ctx,0x8c611544u); r[2]=rd(m,0x8c6115ccu,4);goto L8c611546; /* mov.l 0x8c6115cc,r2 */
L8c611546: sa_motion_publish_stop(ctx,0x8c611546u); fcopy(f,3u,14u);goto L8c611548; /* fmov fr14,fr3 */
L8c611548: sa_motion_publish_stop(ctx,0x8c611548u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61154a; /* fsub fr10,fr3 */
L8c61154a: sa_motion_publish_stop(ctx,0x8c61154au); fcopy(f,5u,7u);goto L8c61154c; /* fmov fr7,fr5 */
L8c61154c: sa_motion_publish_stop(ctx,0x8c61154cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[9],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61154e; /* fmul fr9,fr8 */
L8c61154e: sa_motion_publish_stop(ctx,0x8c61154eu); fload(m,f,1u,r[2]);goto L8c611550; /* fmov.s @r2,fr1 */
L8c611550: sa_motion_publish_stop(ctx,0x8c611550u); fload(m,f,11u,r[0]+r[11]);goto L8c611552; /* fmov.s @(r0,r11),fr11 */
L8c611552: sa_motion_publish_stop(ctx,0x8c611552u); fcopy(f,6u,11u);goto L8c611554; /* fmov fr11,fr6 */
L8c611554: sa_motion_publish_stop(ctx,0x8c611554u); fcopy(f,0u,4u);goto L8c611556; /* fmov fr4,fr0 */
L8c611556: sa_motion_publish_stop(ctx,0x8c611556u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[5],&f->fpscr,&fr[5]);if(status)return status;}goto L8c611558; /* fmac fr0,fr2,fr5 */
L8c611558: sa_motion_publish_stop(ctx,0x8c611558u); fcopy(f,2u,1u);goto L8c61155a; /* fmov fr1,fr2 */
L8c61155a: sa_motion_publish_stop(ctx,0x8c61155au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61155c; /* fmul fr3,fr2 */
L8c61155c: sa_motion_publish_stop(ctx,0x8c61155cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[8],&fr[7],&f->fpscr,&fr[7]);if(status)return status;}goto L8c61155e; /* fmac fr0,fr8,fr7 */
L8c61155e: sa_motion_publish_stop(ctx,0x8c61155eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[6],&f->fpscr,&fr[6]);if(status)return status;}goto L8c611560; /* fmac fr0,fr2,fr6 */
L8c611560: sa_motion_publish_stop(ctx,0x8c611560u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[10],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c611562; /* fadd fr10,fr14 */
L8c611562: sa_motion_publish_stop(ctx,0x8c611562u); r[5]=r[9];goto L8c611564; /* mov r9,r5 */
L8c611564: sa_motion_publish_stop(ctx,0x8c611564u); fload(m,f,3u,r[5]);goto L8c611566; /* fmov.s @r5,fr3 */
L8c611566: sa_motion_publish_stop(ctx,0x8c611566u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][7],&f->fpscr,sr,1);if(status)return status;goto L8c611568; /* fcmp/gt fr7,fr3 */
L8c611568: sa_motion_publish_stop(ctx,0x8c611568u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[14],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61156a; /* fmul fr14,fr1 */
L8c61156a: sa_motion_publish_stop(ctx,0x8c61156au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[11],&f->fpscr,&fr[11]);if(status)return status;}goto L8c61156c; /* fmac fr0,fr1,fr11 */
L8c61156c: sa_motion_publish_stop(ctx,0x8c61156cu); fcopy(f,4u,11u);goto L8c61156e; /* fmov fr11,fr4 */
L8c61156e: sa_motion_publish_stop(ctx,0x8c611570u); take=!(*sr&1u);r[4]=r[9];if(take)goto L8c611578;goto L8c611572; /* bf/s 0x8c611578 */
L8c611572: sa_motion_publish_stop(ctx,0x8c611572u); r[1]=rd(m,0x8c6115c0u,4);goto L8c611574; /* mov.l 0x8c6115c0,r1 */
L8c611574: sa_motion_publish_stop(ctx,0x8c611576u); target=r[1];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r1 */
L8c611578: sa_motion_publish_stop(ctx,0x8c611578u); r[0]=8u;goto L8c61157a; /* mov #0x8,r0 */
L8c61157a: sa_motion_publish_stop(ctx,0x8c61157au); fload(m,f,3u,r[0]+r[4]);goto L8c61157c; /* fmov.s @(r0,r4),fr3 */
L8c61157c: sa_motion_publish_stop(ctx,0x8c61157cu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61157e; /* fcmp/gt fr3,fr5 */
L8c61157e: sa_motion_publish_stop(ctx,0x8c61157eu); if(!(*sr&1u))goto L8c611586;goto L8c611580; /* bf 0x8c611586 */
L8c611580: sa_motion_publish_stop(ctx,0x8c611580u); r[1]=rd(m,0x8c6115c0u,4);goto L8c611582; /* mov.l 0x8c6115c0,r1 */
L8c611582: sa_motion_publish_stop(ctx,0x8c611584u); target=r[1];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r1 */
L8c611586: sa_motion_publish_stop(ctx,0x8c611586u); r[0]=4u;goto L8c611588; /* mov #0x4,r0 */
L8c611588: sa_motion_publish_stop(ctx,0x8c611588u); fload(m,f,3u,r[0]+r[5]);goto L8c61158a; /* fmov.s @(r0,r5),fr3 */
L8c61158a: sa_motion_publish_stop(ctx,0x8c61158au); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c61158c; /* fcmp/gt fr4,fr3 */
L8c61158c: sa_motion_publish_stop(ctx,0x8c61158cu); if(!(*sr&1u))goto L8c611594;goto L8c61158e; /* bf 0x8c611594 */
L8c61158e: sa_motion_publish_stop(ctx,0x8c61158eu); r[1]=rd(m,0x8c6115c0u,4);goto L8c611590; /* mov.l 0x8c6115c0,r1 */
L8c611590: sa_motion_publish_stop(ctx,0x8c611592u); target=r[1];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r1 */
L8c611594: sa_motion_publish_stop(ctx,0x8c611594u); r[0]=12u;goto L8c611596; /* mov #0xc,r0 */
L8c611596: sa_motion_publish_stop(ctx,0x8c611596u); fload(m,f,3u,r[0]+r[4]);goto L8c611598; /* fmov.s @(r0,r4),fr3 */
L8c611598: sa_motion_publish_stop(ctx,0x8c611598u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61159a; /* fcmp/gt fr3,fr6 */
L8c61159a: sa_motion_publish_stop(ctx,0x8c61159au); if(!(*sr&1u))goto L8c6115a2;goto L8c61159c; /* bf 0x8c6115a2 */
L8c61159c: sa_motion_publish_stop(ctx,0x8c61159cu); r[1]=rd(m,0x8c6115c0u,4);goto L8c61159e; /* mov.l 0x8c6115c0,r1 */
L8c61159e: sa_motion_publish_stop(ctx,0x8c6115a0u); target=r[1];switch(target){case 0x8c6115a2u:goto L8c6115a2;case 0x8c6116c8u:goto L8c6116c8;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r1 */
L8c6115a2: sa_motion_publish_stop(ctx,0x8c6115a2u); r[2]=rd(m,r[15],4);goto L8c6115a4; /* mov.l @r15,r2 */
L8c6115a4: sa_motion_publish_stop(ctx,0x8c6115a4u); r[4]=rd(m,0x8c6115d0u,4);goto L8c6115a6; /* mov.l 0x8c6115d0,r4 */
L8c6115a6: sa_motion_publish_stop(ctx,0x8c6115a6u); if(((r[2]+8u)&0x1f000000u)!=0x0c000000u)return -3;r[3]=rd(m,r[2]+0x8u,4);goto L8c6115a8; /* mov.l @(0x8,r2),r3 */
L8c6115a8: sa_motion_publish_stop(ctx,0x8c6115a8u); r[1]=rd(m,r[4],4);goto L8c6115aa; /* mov.l @r4,r1 */
L8c6115aa: sa_motion_publish_stop(ctx,0x8c6115aau); r[2]=rd(m,0x8c6115d4u,4);goto L8c6115ac; /* mov.l 0x8c6115d4,r2 */
L8c6115ac: sa_motion_publish_stop(ctx,0x8c6115acu); r[1]+=r[3];goto L8c6115ae; /* add r3,r1 */
L8c6115ae: sa_motion_publish_stop(ctx,0x8c6115aeu); r[5]=rd(m,0x8c6115d8u,4);goto L8c6115b0; /* mov.l 0x8c6115d8,r5 */
L8c6115b0: sa_motion_publish_stop(ctx,0x8c6115b0u); wr(m,r[4],r[1],4);goto L8c6115b2; /* mov.l r1,@r4 */
L8c6115b2: sa_motion_publish_stop(ctx,0x8c6115b2u); r[4]=rd(m,0x8c6115dcu,4);goto L8c6115b4; /* mov.l 0x8c6115dc,r4 */
L8c6115b4: sa_motion_publish_stop(ctx,0x8c6115b4u); fload(m,f,4u,r[2]);goto L8c6115b6; /* fmov.s @r2,fr4 */
L8c6115b6: sa_motion_publish_stop(ctx,0x8c6115b8u); goto L8c6115e0; /* bra 0x8c6115e0 */
L8c6115e0: sa_motion_publish_stop(ctx,0x8c6115e0u); fcopy(f,11u,4u);goto L8c6115e2; /* fmov fr4,fr11 */
L8c6115e2: sa_motion_publish_stop(ctx,0x8c6115e2u); fload(m,f,0u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6115e4; /* fmov.s @r5+,fr0 */
L8c6115e4: sa_motion_publish_stop(ctx,0x8c6115e4u); fload(m,f,1u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6115e6; /* fmov.s @r5+,fr1 */
L8c6115e6: sa_motion_publish_stop(ctx,0x8c6115e6u); fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6115e8; /* fmov.s @r5+,fr2 */
L8c6115e8: sa_motion_publish_stop(ctx,0x8c6115e8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6115ea; /* fldi0 fr3 */
L8c6115ea: sa_motion_publish_stop(ctx,0x8c6115eau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6115ec; /* fschg */
L8c6115ec: sa_motion_publish_stop(ctx,0x8c6115ecu); fcopy(f,4u,1u);goto L8c6115ee; /* fmov fr1,fr4 */
L8c6115ee: sa_motion_publish_stop(ctx,0x8c6115eeu); fcopy(f,6u,3u);goto L8c6115f0; /* fmov fr3,fr6 */
L8c6115f0: sa_motion_publish_stop(ctx,0x8c6115f0u); status=sa_sh4_vector_numeric(0xf4edu,f,*fpul);if(status)return status;goto L8c6115f2; /* fipr fv0,fv4 */
L8c6115f2: sa_motion_publish_stop(ctx,0x8c6115f2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6115f4; /* fschg */
L8c6115f4: sa_motion_publish_stop(ctx,0x8c6115f4u); fcopy(f,8u,7u);goto L8c6115f6; /* fmov fr7,fr8 */
L8c6115f6: sa_motion_publish_stop(ctx,0x8c6115f6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6115f8; /* fschg */
L8c6115f8: sa_motion_publish_stop(ctx,0x8c6115f8u); fcopy(f,4u,5u);goto L8c6115fa; /* fmov fr5,fr4 */
L8c6115fa: sa_motion_publish_stop(ctx,0x8c6115fau); fcopy(f,6u,7u);goto L8c6115fc; /* fmov fr7,fr6 */
L8c6115fc: sa_motion_publish_stop(ctx,0x8c6115fcu); status=sa_sh4_vector_numeric(0xf4edu,f,*fpul);if(status)return status;goto L8c6115fe; /* fipr fv0,fv4 */
L8c6115fe: sa_motion_publish_stop(ctx,0x8c6115feu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c611600; /* fschg */
L8c611600: sa_motion_publish_stop(ctx,0x8c611600u); fcopy(f,9u,7u);goto L8c611602; /* fmov fr7,fr9 */
L8c611602: sa_motion_publish_stop(ctx,0x8c611602u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c611604; /* fschg */
L8c611604: sa_motion_publish_stop(ctx,0x8c611604u); fcopy(f,4u,9u);goto L8c611606; /* fmov fr9,fr4 */
L8c611606: sa_motion_publish_stop(ctx,0x8c611606u); fcopy(f,6u,11u);goto L8c611608; /* fmov fr11,fr6 */
L8c611608: sa_motion_publish_stop(ctx,0x8c611608u); status=sa_sh4_vector_numeric(0xf4edu,f,*fpul);if(status)return status;goto L8c61160a; /* fipr fv0,fv4 */
L8c61160a: sa_motion_publish_stop(ctx,0x8c61160au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61160c; /* fschg */
L8c61160c: sa_motion_publish_stop(ctx,0x8c61160cu); fcopy(f,10u,7u);goto L8c61160e; /* fmov fr7,fr10 */
L8c61160e: sa_motion_publish_stop(ctx,0x8c61160eu); fcopy(f,0u,8u);goto L8c611610; /* fmov fr8,fr0 */
L8c611610: sa_motion_publish_stop(ctx,0x8c611610u); fcopy(f,1u,9u);goto L8c611612; /* fmov fr9,fr1 */
L8c611612: sa_motion_publish_stop(ctx,0x8c611612u); fcopy(f,2u,10u);goto L8c611614; /* fmov fr10,fr2 */
L8c611614: sa_motion_publish_stop(ctx,0x8c611614u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c611616; /* fipr fv8,fv0 */
L8c611616: sa_motion_publish_stop(ctx,0x8c611616u); r[4]+=12u;goto L8c611618; /* add #0xc,r4 */
L8c611618: sa_motion_publish_stop(ctx,0x8c611618u); status=sa_sh4_vector_numeric(0xf37du,f,*fpul);if(status)return status;goto L8c61161a; /* fsrra fr3 */
L8c61161a: sa_motion_publish_stop(ctx,0x8c61161au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[11],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61161c; /* fmul fr11,fr3 */
L8c61161c: sa_motion_publish_stop(ctx,0x8c61161cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c61161e; /* fmul fr3,fr10 */
L8c61161e: sa_motion_publish_stop(ctx,0x8c61161eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c611620; /* fmul fr3,fr9 */
L8c611620: sa_motion_publish_stop(ctx,0x8c611620u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c611622; /* fmul fr3,fr8 */
L8c611622: sa_motion_publish_stop(ctx,0x8c611622u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[4]);goto L8c611624; /* fmov.s fr10,@-r4 */
L8c611624: sa_motion_publish_stop(ctx,0x8c611624u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[4]);goto L8c611626; /* fmov.s fr9,@-r4 */
L8c611626: sa_motion_publish_stop(ctx,0x8c611626u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[4]);goto L8c611628; /* fmov.s fr8,@-r4 */
L8c611628: sa_motion_publish_stop(ctx,0x8c611628u); r[2]=rd(m,r[15],4);goto L8c61162a; /* mov.l @r15,r2 */
L8c61162a: sa_motion_publish_stop(ctx,0x8c61162au); r[0]=40u;goto L8c61162c; /* mov #0x28,r0 */
L8c61162c: sa_motion_publish_stop(ctx,0x8c61162cu); r[1]=rd(m,0x8c6116f0u,4);goto L8c61162e; /* mov.l 0x8c6116f0,r1 */
L8c61162e: sa_motion_publish_stop(ctx,0x8c61162eu); if(((r[2]+8u)&0x1f000000u)!=0x0c000000u)return -3;r[3]=rd(m,r[2]+0x8u,4);goto L8c611630; /* mov.l @(0x8,r2),r3 */
L8c611630: sa_motion_publish_stop(ctx,0x8c611630u); r[2]=rd(m,0x8c6116ecu,4);goto L8c611632; /* mov.l 0x8c6116ec,r2 */
L8c611632: sa_motion_publish_stop(ctx,0x8c611632u); wr(m,r[15]+0x10u,r[3],4);goto L8c611634; /* mov.l r3,@(0x10,r15) */
L8c611634: sa_motion_publish_stop(ctx,0x8c611634u); r[3]=rd(m,0x8c6116e8u,4);goto L8c611636; /* mov.l 0x8c6116e8,r3 */
L8c611636: sa_motion_publish_stop(ctx,0x8c611636u); wr(m,r[15]+0x14u,r[13],4);goto L8c611638; /* mov.l r13,@(0x14,r15) */
L8c611638: sa_motion_publish_stop(ctx,0x8c611638u); wr(m,r[15]+0x18u,r[12],4);goto L8c61163a; /* mov.l r12,@(0x18,r15) */
L8c61163a: sa_motion_publish_stop(ctx,0x8c61163au); fload(m,f,3u,r[3]);goto L8c61163c; /* fmov.s @r3,fr3 */
L8c61163c: sa_motion_publish_stop(ctx,0x8c61163cu); fstore(m,f,3u,r[0]+r[15]);goto L8c61163e; /* fmov fr3,@(r0,r15) */
L8c61163e: sa_motion_publish_stop(ctx,0x8c61163eu); r[0]=44u;goto L8c611640; /* mov #0x2c,r0 */
L8c611640: sa_motion_publish_stop(ctx,0x8c611640u); fload(m,f,3u,r[2]);goto L8c611642; /* fmov.s @r2,fr3 */
L8c611642: sa_motion_publish_stop(ctx,0x8c611642u); fstore(m,f,3u,r[0]+r[15]);goto L8c611644; /* fmov fr3,@(r0,r15) */
L8c611644: sa_motion_publish_stop(ctx,0x8c611644u); r[0]=rd(m,r[1],4);goto L8c611646; /* mov.l @r1,r0 */
L8c611646: sa_motion_publish_stop(ctx,0x8c611646u); *sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c611648; /* tst #0x2,r0 */
L8c611648: sa_motion_publish_stop(ctx,0x8c611648u); if((*sr&1u))goto L8c61166c;goto L8c61164a; /* bt 0x8c61166c */
L8c61164a: sa_motion_publish_stop(ctx,0x8c61164au); *sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c61164c; /* tst r14,r14 */
L8c61164c: sa_motion_publish_stop(ctx,0x8c61164cu); if((*sr&1u))goto L8c61165e;goto L8c61164e; /* bt 0x8c61165e */
L8c61164e: sa_motion_publish_stop(ctx,0x8c61164eu); r[3]=rd(m,0x8c6116f4u,4);goto L8c611650; /* mov.l 0x8c6116f4,r3 */
L8c611650: sa_motion_publish_stop(ctx,0x8c611650u); r[4]=r[15];goto L8c611652; /* mov r15,r4 */
L8c611652: sa_motion_publish_stop(ctx,0x8c611652u); wr(m,r[15]+0x1cu,r[14],4);goto L8c611654; /* mov.l r14,@(0x1c,r15) */
L8c611654: sa_motion_publish_stop(ctx,0x8c611654u); r[5]=rd(m,r[10],4);goto L8c611656; /* mov.l @r10,r5 */
L8c611656: sa_motion_publish_stop(ctx,0x8c611658u); target=r[3];*pr=0x8c61165au;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61165a; /* jsr @r3 */
L8c61165a: sa_motion_publish_stop(ctx,0x8c61165cu); goto L8c6116d2; /* bra 0x8c6116d2 */
L8c61165e: sa_motion_publish_stop(ctx,0x8c61165eu); r[3]=rd(m,0x8c6116f8u,4);goto L8c611660; /* mov.l 0x8c6116f8,r3 */
L8c611660: sa_motion_publish_stop(ctx,0x8c611660u); r[4]=r[15];goto L8c611662; /* mov r15,r4 */
L8c611662: sa_motion_publish_stop(ctx,0x8c611662u); r[5]=rd(m,r[10],4);goto L8c611664; /* mov.l @r10,r5 */
L8c611664: sa_motion_publish_stop(ctx,0x8c611666u); target=r[3];*pr=0x8c611668u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c611668; /* jsr @r3 */
L8c611668: sa_motion_publish_stop(ctx,0x8c61166au); goto L8c6116d2; /* bra 0x8c6116d2 */
L8c61166c: sa_motion_publish_stop(ctx,0x8c61166cu); r[4]=r[9];goto L8c61166e; /* mov r9,r4 */
L8c61166e: sa_motion_publish_stop(ctx,0x8c61166eu); r[0]=48u;goto L8c611670; /* mov #0x30,r0 */
L8c611670: sa_motion_publish_stop(ctx,0x8c611670u); r[5]=r[4];goto L8c611672; /* mov r4,r5 */
L8c611672: sa_motion_publish_stop(ctx,0x8c611672u); r[5]+=8u;goto L8c611674; /* add #0x8,r5 */
L8c611674: sa_motion_publish_stop(ctx,0x8c611674u); r[3]=4294967295u;goto L8c611676; /* mov #-0x1,r3 */
L8c611676: sa_motion_publish_stop(ctx,0x8c611676u); wr(m,r[15]+0x20u,r[3],4);goto L8c611678; /* mov.l r3,@(0x20,r15) */
L8c611678: sa_motion_publish_stop(ctx,0x8c611678u); r[3]=0u;goto L8c61167a; /* mov #0x0,r3 */
L8c61167a: sa_motion_publish_stop(ctx,0x8c61167au); *sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c61167c; /* tst r14,r14 */
L8c61167c: sa_motion_publish_stop(ctx,0x8c61167cu); wr(m,r[15]+0x24u,r[3],4);goto L8c61167e; /* mov.l r3,@(0x24,r15) */
L8c61167e: sa_motion_publish_stop(ctx,0x8c61167eu); fload(m,f,3u,r[4]);goto L8c611680; /* fmov.s @r4,fr3 */
L8c611680: sa_motion_publish_stop(ctx,0x8c611680u); fstore(m,f,3u,r[0]+r[15]);goto L8c611682; /* fmov fr3,@(r0,r15) */
L8c611682: sa_motion_publish_stop(ctx,0x8c611682u); r[0]=52u;goto L8c611684; /* mov #0x34,r0 */
L8c611684: sa_motion_publish_stop(ctx,0x8c611684u); fload(m,f,3u,r[5]);goto L8c611686; /* fmov.s @r5,fr3 */
L8c611686: sa_motion_publish_stop(ctx,0x8c611686u); fstore(m,f,3u,r[0]+r[15]);goto L8c611688; /* fmov fr3,@(r0,r15) */
L8c611688: sa_motion_publish_stop(ctx,0x8c611688u); r[0]=4u;goto L8c61168a; /* mov #0x4,r0 */
L8c61168a: sa_motion_publish_stop(ctx,0x8c61168au); fload(m,f,3u,r[0]+r[4]);goto L8c61168c; /* fmov.s @(r0,r4),fr3 */
L8c61168c: sa_motion_publish_stop(ctx,0x8c61168cu); r[0]=56u;goto L8c61168e; /* mov #0x38,r0 */
L8c61168e: sa_motion_publish_stop(ctx,0x8c61168eu); fstore(m,f,3u,r[0]+r[15]);goto L8c611690; /* fmov fr3,@(r0,r15) */
L8c611690: sa_motion_publish_stop(ctx,0x8c611690u); r[0]=4u;goto L8c611692; /* mov #0x4,r0 */
L8c611692: sa_motion_publish_stop(ctx,0x8c611692u); fload(m,f,3u,r[0]+r[5]);goto L8c611694; /* fmov.s @(r0,r5),fr3 */
L8c611694: sa_motion_publish_stop(ctx,0x8c611694u); r[0]=60u;goto L8c611696; /* mov #0x3c,r0 */
L8c611696: sa_motion_publish_stop(ctx,0x8c611698u); take=(*sr&1u);fstore(m,f,3u,r[0]+r[15]);if(take)goto L8c6116aa;goto L8c61169a; /* bt/s 0x8c6116aa */
L8c61169a: sa_motion_publish_stop(ctx,0x8c61169au); r[3]=rd(m,0x8c6116fcu,4);goto L8c61169c; /* mov.l 0x8c6116fc,r3 */
L8c61169c: sa_motion_publish_stop(ctx,0x8c61169cu); r[4]=r[15];goto L8c61169e; /* mov r15,r4 */
L8c61169e: sa_motion_publish_stop(ctx,0x8c61169eu); wr(m,r[15]+0x1cu,r[14],4);goto L8c6116a0; /* mov.l r14,@(0x1c,r15) */
L8c6116a0: sa_motion_publish_stop(ctx,0x8c6116a0u); r[5]=rd(m,r[10],4);goto L8c6116a2; /* mov.l @r10,r5 */
L8c6116a2: sa_motion_publish_stop(ctx,0x8c6116a4u); target=r[3];*pr=0x8c6116a6u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6116a6; /* jsr @r3 */
L8c6116a6: sa_motion_publish_stop(ctx,0x8c6116a8u); goto L8c6116b4; /* bra 0x8c6116b4 */
L8c6116aa: sa_motion_publish_stop(ctx,0x8c6116aau); r[3]=rd(m,0x8c611700u,4);goto L8c6116ac; /* mov.l 0x8c611700,r3 */
L8c6116ac: sa_motion_publish_stop(ctx,0x8c6116acu); r[4]=r[15];goto L8c6116ae; /* mov r15,r4 */
L8c6116ae: sa_motion_publish_stop(ctx,0x8c6116aeu); r[5]=rd(m,r[10],4);goto L8c6116b0; /* mov.l @r10,r5 */
L8c6116b0: sa_motion_publish_stop(ctx,0x8c6116b2u); target=r[3];*pr=0x8c6116b4u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6116b4; /* jsr @r3 */
L8c6116b4: sa_motion_publish_stop(ctx,0x8c6116b4u); r[4]=rd(m,r[15]+0x20u,4);goto L8c6116b6; /* mov.l @(0x20,r15),r4 */
L8c6116b6: sa_motion_publish_stop(ctx,0x8c6116b6u); r[3]=60u;goto L8c6116b8; /* mov #0x3c,r3 */
L8c6116b8: sa_motion_publish_stop(ctx,0x8c6116b8u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6116ba; /* tst r4,r3 */
L8c6116ba: sa_motion_publish_stop(ctx,0x8c6116bau); if(!(*sr&1u))goto L8c6116c8;goto L8c6116bc; /* bf 0x8c6116c8 */
L8c6116bc: sa_motion_publish_stop(ctx,0x8c6116bcu); r[3]=2u;goto L8c6116be; /* mov #0x2,r3 */
L8c6116be: sa_motion_publish_stop(ctx,0x8c6116beu); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6116c0; /* tst r4,r3 */
L8c6116c0: sa_motion_publish_stop(ctx,0x8c6116c0u); if(!(*sr&1u))goto L8c6116c8;goto L8c6116c2; /* bf 0x8c6116c8 */
L8c6116c2: sa_motion_publish_stop(ctx,0x8c6116c2u); r[3]=1u;goto L8c6116c4; /* mov #0x1,r3 */
L8c6116c4: sa_motion_publish_stop(ctx,0x8c6116c4u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c6116c6; /* tst r3,r4 */
L8c6116c6: sa_motion_publish_stop(ctx,0x8c6116c6u); if((*sr&1u))goto L8c6116cc;goto L8c6116c8; /* bt 0x8c6116cc */
L8c6116c8: sa_motion_publish_stop(ctx,0x8c6116cau); r[0]=4294967295u;goto L8c6116d4; /* bra 0x8c6116d4 */
L8c6116cc: sa_motion_publish_stop(ctx,0x8c6116ccu); r[2]=rd(m,0x8c611704u,4);goto L8c6116ce; /* mov.l 0x8c611704,r2 */
L8c6116ce: sa_motion_publish_stop(ctx,0x8c6116ceu); r[3]=rd(m,r[15]+0x24u,4);goto L8c6116d0; /* mov.l @(0x24,r15),r3 */
L8c6116d0: sa_motion_publish_stop(ctx,0x8c6116d0u); wr(m,r[2],r[3],4);goto L8c6116d2; /* mov.l r3,@r2 */
L8c6116d2: sa_motion_publish_stop(ctx,0x8c6116d2u); r[0]=0u;goto L8c6116d4; /* mov #0x0,r0 */
L8c6116d4: sa_motion_publish_stop(ctx,0x8c6116d4u); r[15]+=64u;goto L8c6116d6; /* add #0x40,r15 */
L8c6116d6: sa_motion_publish_stop(ctx,0x8c6116d6u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c6116d8; /* lds.l @r15+,PR */
L8c6116d8: sa_motion_publish_stop(ctx,0x8c6116d8u); fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6116da; /* fmov.s @r15+,fr14 */
L8c6116da: sa_motion_publish_stop(ctx,0x8c6116dau); r[9]=rd(m,r[15],4);r[15]+=4;goto L8c6116dc; /* mov.l @r15+,r9 */
L8c6116dc: sa_motion_publish_stop(ctx,0x8c6116dcu); r[10]=rd(m,r[15],4);r[15]+=4;goto L8c6116de; /* mov.l @r15+,r10 */
L8c6116de: sa_motion_publish_stop(ctx,0x8c6116deu); r[11]=rd(m,r[15],4);r[15]+=4;goto L8c6116e0; /* mov.l @r15+,r11 */
L8c6116e0: sa_motion_publish_stop(ctx,0x8c6116e0u); r[12]=rd(m,r[15],4);r[15]+=4;goto L8c6116e2; /* mov.l @r15+,r12 */
L8c6116e2: sa_motion_publish_stop(ctx,0x8c6116e2u); r[13]=rd(m,r[15],4);r[15]+=4;goto L8c6116e4; /* mov.l @r15+,r13 */
L8c6116e4: sa_motion_publish_stop(ctx,0x8c6116e6u); if(*pr!=entry_pr)return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_flags_8c606c56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c606c56;
L8c606c56: sa_motion_publish_stop(ctx,0x8c606c56u); r[6]=rd(m,0x8c606ca8u,4);goto L8c606c58; /* mov.l 0x8c606ca8,r6 */
L8c606c58: sa_motion_publish_stop(ctx,0x8c606c58u); r[2]=rd(m,0x8c606cb8u,4);goto L8c606c5a; /* mov.l 0x8c606cb8,r2 */
L8c606c5a: sa_motion_publish_stop(ctx,0x8c606c5au); r[5]=rd(m,r[6]+0x8u,4);goto L8c606c5c; /* mov.l @(0x8,r6),r5 */
L8c606c5c: sa_motion_publish_stop(ctx,0x8c606c5cu); r[3]=rd(m,0x8c606cb4u,4);goto L8c606c5e; /* mov.l 0x8c606cb4,r3 */
L8c606c5e: sa_motion_publish_stop(ctx,0x8c606c5eu); r[4]&=r[2];goto L8c606c60; /* and r2,r4 */
L8c606c60: sa_motion_publish_stop(ctx,0x8c606c60u); r[5]&=r[3];goto L8c606c62; /* and r3,r5 */
L8c606c62: sa_motion_publish_stop(ctx,0x8c606c62u); r[5]|=r[4];goto L8c606c64; /* or r4,r5 */
L8c606c64: sa_motion_publish_stop(ctx,0x8c606c66u); wr(m,r[6]+0x8u,r[5],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_shader_state_8c606a08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c606a08;
L8c606a08: sa_motion_publish_stop(ctx,0x8c606a08u); r[15]-=4;wr(m,r[15],r[14],4);goto L8c606a0a; /* mov.l r14,@-r15 */
L8c606a0a: sa_motion_publish_stop(ctx,0x8c606a0au); r[14]=rd(m,0x8c606b20u,4);goto L8c606a0c; /* mov.l 0x8c606b20,r14 */
L8c606a0c: sa_motion_publish_stop(ctx,0x8c606a0cu); r[3]=rd(m,0x8c606b24u,4);goto L8c606a0e; /* mov.l 0x8c606b24,r3 */
L8c606a0e: sa_motion_publish_stop(ctx,0x8c606a0eu); r[1]=rd(m,0x8c606b2cu,4);goto L8c606a10; /* mov.l 0x8c606b2c,r1 */
L8c606a10: sa_motion_publish_stop(ctx,0x8c606a10u); r[2]=rd(m,0x8c606b28u,4);goto L8c606a12; /* mov.l 0x8c606b28,r2 */
L8c606a12: sa_motion_publish_stop(ctx,0x8c606a12u); r[1]&=r[4];goto L8c606a14; /* and r4,r1 */
L8c606a14: sa_motion_publish_stop(ctx,0x8c606a14u); r[15]-=4;wr(m,r[15],r[13],4);goto L8c606a16; /* mov.l r13,@-r15 */
L8c606a16: sa_motion_publish_stop(ctx,0x8c606a16u); r[5]=rd(m,r[14],4);goto L8c606a18; /* mov.l @r14,r5 */
L8c606a18: sa_motion_publish_stop(ctx,0x8c606a18u); r[6]=rd(m,r[14]+0x8u,4);goto L8c606a1a; /* mov.l @(0x8,r14),r6 */
L8c606a1a: sa_motion_publish_stop(ctx,0x8c606a1au); r[5]&=r[3];goto L8c606a1c; /* and r3,r5 */
L8c606a1c: sa_motion_publish_stop(ctx,0x8c606a1cu); r[3]=rd(m,0x8c606b30u,4);goto L8c606a1e; /* mov.l 0x8c606b30,r3 */
L8c606a1e: sa_motion_publish_stop(ctx,0x8c606a1eu); r[5]|=r[1];goto L8c606a20; /* or r1,r5 */
L8c606a20: sa_motion_publish_stop(ctx,0x8c606a20u); r[1]=rd(m,0x8c606b34u,4);goto L8c606a22; /* mov.l 0x8c606b34,r1 */
L8c606a22: sa_motion_publish_stop(ctx,0x8c606a22u); r[3]&=r[4];goto L8c606a24; /* and r4,r3 */
L8c606a24: sa_motion_publish_stop(ctx,0x8c606a24u); r[6]&=r[2];goto L8c606a26; /* and r2,r6 */
L8c606a26: sa_motion_publish_stop(ctx,0x8c606a26u); r[13]=rd(m,r[1],4);goto L8c606a28; /* mov.l @r1,r13 */
L8c606a28: sa_motion_publish_stop(ctx,0x8c606a28u); r[6]|=r[3];goto L8c606a2a; /* or r3,r6 */
L8c606a2a: sa_motion_publish_stop(ctx,0x8c606a2au); r[3]=sx(m,0x8c606b14u,2);goto L8c606a2c; /* mov.w 0x8c606b14,r3 */
L8c606a2c: sa_motion_publish_stop(ctx,0x8c606a2cu); *sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c606a2e; /* tst r13,r3 */
L8c606a2e: sa_motion_publish_stop(ctx,0x8c606a30u); take=!(*sr&1u);r[7]=rd(m,r[14]+0x4u,4);if(take)goto L8c606a3e;goto L8c606a32; /* bf/s 0x8c606a3e */
L8c606a32: sa_motion_publish_stop(ctx,0x8c606a32u); r[0]=sx(m,0x8c606b16u,2);goto L8c606a34; /* mov.w 0x8c606b16,r0 */
L8c606a34: sa_motion_publish_stop(ctx,0x8c606a34u); *sr=(*sr&~1u)|!(r[0]&r[13]);goto L8c606a36; /* tst r0,r13 */
L8c606a36: sa_motion_publish_stop(ctx,0x8c606a36u); if((*sr&1u))goto L8c606a44;goto L8c606a38; /* bt 0x8c606a44 */
L8c606a38: sa_motion_publish_stop(ctx,0x8c606a38u); r[2]=rd(m,0x8c606b38u,4);goto L8c606a3a; /* mov.l 0x8c606b38,r2 */
L8c606a3a: sa_motion_publish_stop(ctx,0x8c606a3au); *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c606a3c; /* tst r4,r2 */
L8c606a3c: sa_motion_publish_stop(ctx,0x8c606a3cu); if(!(*sr&1u))goto L8c606a44;goto L8c606a3e; /* bf 0x8c606a44 */
L8c606a3e: sa_motion_publish_stop(ctx,0x8c606a3eu); r[1]=sx(m,0x8c606b18u,2);goto L8c606a40; /* mov.w 0x8c606b18,r1 */
L8c606a40: sa_motion_publish_stop(ctx,0x8c606a42u); r[5]|=r[1];goto L8c606a48; /* bra 0x8c606a48 */
L8c606a44: sa_motion_publish_stop(ctx,0x8c606a44u); r[2]=sx(m,0x8c606b1au,2);goto L8c606a46; /* mov.w 0x8c606b1a,r2 */
L8c606a46: sa_motion_publish_stop(ctx,0x8c606a46u); r[5]&=r[2];goto L8c606a48; /* and r2,r5 */
L8c606a48: sa_motion_publish_stop(ctx,0x8c606a48u); r[2]=rd(m,0x8c606b3cu,4);goto L8c606a4a; /* mov.l 0x8c606b3c,r2 */
L8c606a4a: sa_motion_publish_stop(ctx,0x8c606a4au); wr(m,r[14],r[5],4);goto L8c606a4c; /* mov.l r5,@r14 */
L8c606a4c: sa_motion_publish_stop(ctx,0x8c606a4cu); wr(m,r[14]+0x4u,r[7],4);goto L8c606a4e; /* mov.l r7,@(0x4,r14) */
L8c606a4e: sa_motion_publish_stop(ctx,0x8c606a4eu); wr(m,r[14]+0x8u,r[6],4);goto L8c606a50; /* mov.l r6,@(0x8,r14) */
L8c606a50: sa_motion_publish_stop(ctx,0x8c606a50u); r[3]=rd(m,r[2],4);goto L8c606a52; /* mov.l @r2,r3 */
L8c606a52: sa_motion_publish_stop(ctx,0x8c606a52u); wr(m,r[3],r[4],4);goto L8c606a54; /* mov.l r4,@r3 */
L8c606a54: sa_motion_publish_stop(ctx,0x8c606a54u); r[13]=rd(m,r[15],4);r[15]+=4;goto L8c606a56; /* mov.l @r15+,r13 */
L8c606a56: sa_motion_publish_stop(ctx,0x8c606a58u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_texture_state_8c606a88(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c606a88;
L8c606a88: sa_motion_publish_stop(ctx,0x8c606a88u); r[2]=rd(m,0x8c606b40u,4);goto L8c606a8a; /* mov.l 0x8c606b40,r2 */
L8c606a8a: sa_motion_publish_stop(ctx,0x8c606a8au); r[1]=r[4];goto L8c606a8c; /* mov r4,r1 */
L8c606a8c: sa_motion_publish_stop(ctx,0x8c606a8cu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c606a8e; /* shll r4 */
L8c606a8e: sa_motion_publish_stop(ctx,0x8c606a8eu); r[5]=rd(m,0x8c606b44u,4);goto L8c606a90; /* mov.l 0x8c606b44,r5 */
L8c606a90: sa_motion_publish_stop(ctx,0x8c606a90u); r[3]=rd(m,r[2],4);goto L8c606a92; /* mov.l @r2,r3 */
L8c606a92: sa_motion_publish_stop(ctx,0x8c606a92u); r[4]+=r[1];goto L8c606a94; /* add r1,r4 */
L8c606a94: sa_motion_publish_stop(ctx,0x8c606a94u); r[4]<<=2;goto L8c606a96; /* shll2 r4 */
L8c606a96: sa_motion_publish_stop(ctx,0x8c606a96u); r[1]=rd(m,r[5],4);goto L8c606a98; /* mov.l @r5,r1 */
L8c606a98: sa_motion_publish_stop(ctx,0x8c606a98u); r[3]=rd(m,r[3],4);goto L8c606a9a; /* mov.l @r3,r3 */
L8c606a9a: sa_motion_publish_stop(ctx,0x8c606a9au); r[4]+=r[3];goto L8c606a9c; /* add r3,r4 */
L8c606a9c: sa_motion_publish_stop(ctx,0x8c606a9cu); r[4]=rd(m,r[4]+0x8u,4);goto L8c606a9e; /* mov.l @(0x8,r4),r4 */
L8c606a9e: sa_motion_publish_stop(ctx,0x8c606a9eu); r[3]=rd(m,r[4],4);goto L8c606aa0; /* mov.l @r4,r3 */
L8c606aa0: sa_motion_publish_stop(ctx,0x8c606aa0u); *sr=(*sr&~1u)|(r[1]==r[3]);goto L8c606aa2; /* cmp/eq r1,r3 */
L8c606aa2: sa_motion_publish_stop(ctx,0x8c606aa2u); if(!(*sr&1u))goto L8c606aae;goto L8c606aa4; /* bf 0x8c606aae */
L8c606aa4: sa_motion_publish_stop(ctx,0x8c606aa4u); r[1]=rd(m,0x8c606b48u,4);goto L8c606aa6; /* mov.l 0x8c606b48,r1 */
L8c606aa6: sa_motion_publish_stop(ctx,0x8c606aa6u); r[3]=rd(m,r[4]+0x4u,4);goto L8c606aa8; /* mov.l @(0x4,r4),r3 */
L8c606aa8: sa_motion_publish_stop(ctx,0x8c606aa8u); r[0]=rd(m,r[1],4);goto L8c606aaa; /* mov.l @r1,r0 */
L8c606aaa: sa_motion_publish_stop(ctx,0x8c606aaau); *sr=(*sr&~1u)|(r[3]==r[0]);goto L8c606aac; /* cmp/eq r3,r0 */
L8c606aac: sa_motion_publish_stop(ctx,0x8c606aacu); if((*sr&1u))goto L8c606ada;goto L8c606aae; /* bt 0x8c606ada */
L8c606aae: sa_motion_publish_stop(ctx,0x8c606aaeu); r[2]=rd(m,r[4],4);goto L8c606ab0; /* mov.l @r4,r2 */
L8c606ab0: sa_motion_publish_stop(ctx,0x8c606ab0u); r[1]=rd(m,0x8c606b48u,4);goto L8c606ab2; /* mov.l 0x8c606b48,r1 */
L8c606ab2: sa_motion_publish_stop(ctx,0x8c606ab2u); r[6]=rd(m,0x8c606b20u,4);goto L8c606ab4; /* mov.l 0x8c606b20,r6 */
L8c606ab4: sa_motion_publish_stop(ctx,0x8c606ab4u); wr(m,r[5],r[2],4);goto L8c606ab6; /* mov.l r2,@r5 */
L8c606ab6: sa_motion_publish_stop(ctx,0x8c606ab6u); r[3]=rd(m,r[4]+0x4u,4);goto L8c606ab8; /* mov.l @(0x4,r4),r3 */
L8c606ab8: sa_motion_publish_stop(ctx,0x8c606ab8u); r[2]=rd(m,0x8c606b4cu,4);goto L8c606aba; /* mov.l 0x8c606b4c,r2 */
L8c606aba: sa_motion_publish_stop(ctx,0x8c606abau); wr(m,r[1],r[3],4);goto L8c606abc; /* mov.l r3,@r1 */
L8c606abc: sa_motion_publish_stop(ctx,0x8c606abcu); r[3]=4294967232u;goto L8c606abe; /* mov #-0x40,r3 */
L8c606abe: sa_motion_publish_stop(ctx,0x8c606abeu); r[0]=rd(m,r[4]+0x8u,4);goto L8c606ac0; /* mov.l @(0x8,r4),r0 */
L8c606ac0: sa_motion_publish_stop(ctx,0x8c606ac0u); r[5]=rd(m,r[6]+0x8u,4);goto L8c606ac2; /* mov.l @(0x8,r6),r5 */
L8c606ac2: sa_motion_publish_stop(ctx,0x8c606ac2u); r[1]=rd(m,0x8c606b50u,4);goto L8c606ac4; /* mov.l 0x8c606b50,r1 */
L8c606ac4: sa_motion_publish_stop(ctx,0x8c606ac4u); r[0]&=0x3fu;goto L8c606ac6; /* and #0x3f,r0 */
L8c606ac6: sa_motion_publish_stop(ctx,0x8c606ac6u); r[5]&=r[3];goto L8c606ac8; /* and r3,r5 */
L8c606ac8: sa_motion_publish_stop(ctx,0x8c606ac8u); r[5]|=r[0];goto L8c606aca; /* or r0,r5 */
L8c606aca: sa_motion_publish_stop(ctx,0x8c606acau); wr(m,r[6]+0x8u,r[5],4);goto L8c606acc; /* mov.l r5,@(0x8,r6) */
L8c606acc: sa_motion_publish_stop(ctx,0x8c606accu); r[4]=rd(m,r[4]+0xcu,4);goto L8c606ace; /* mov.l @(0xc,r4),r4 */
L8c606ace: sa_motion_publish_stop(ctx,0x8c606aceu); r[2]&=r[4];goto L8c606ad0; /* and r4,r2 */
L8c606ad0: sa_motion_publish_stop(ctx,0x8c606ad0u); *sr=(*sr&~1u)|(r[2]>r[1]);goto L8c606ad2; /* cmp/hi r1,r2 */
L8c606ad2: sa_motion_publish_stop(ctx,0x8c606ad2u); if(!(*sr&1u))goto L8c606ad8;goto L8c606ad4; /* bf 0x8c606ad8 */
L8c606ad4: sa_motion_publish_stop(ctx,0x8c606ad6u); r[0]=4294967295u;return 0; /* rts */
L8c606ad8: sa_motion_publish_stop(ctx,0x8c606ad8u); wr(m,r[6]+0xcu,r[4],4);goto L8c606ada; /* mov.l r4,@(0xc,r6) */
L8c606ada: sa_motion_publish_stop(ctx,0x8c606adau); r[0]=1u;goto L8c606adc; /* mov #0x1,r0 */
L8c606adc: sa_motion_publish_stop(ctx,0x8c606adeu); return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_command_finish_8c6069a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6069a4;
L8c6069a4: sa_motion_publish_stop(ctx,0x8c6069a4u); r[2]=rd(m,0x8c6069d4u,4);goto L8c6069a6; /* mov.l 0x8c6069d4,r2 */
L8c6069a6: sa_motion_publish_stop(ctx,0x8c6069a6u); r[1]=rd(m,0x8c6069ccu,4);goto L8c6069a8; /* mov.l 0x8c6069cc,r1 */
L8c6069a8: sa_motion_publish_stop(ctx,0x8c6069a8u); r[3]=rd(m,r[2],4);goto L8c6069aa; /* mov.l @r2,r3 */
L8c6069aa: sa_motion_publish_stop(ctx,0x8c6069aau); r[2]=rd(m,0x8c6069c0u,4);goto L8c6069ac; /* mov.l 0x8c6069c0,r2 */
L8c6069ac: sa_motion_publish_stop(ctx,0x8c6069acu); r[4]&=r[1];goto L8c6069ae; /* and r1,r4 */
L8c6069ae: sa_motion_publish_stop(ctx,0x8c6069aeu); r[0]=rd(m,r[2],4);goto L8c6069b0; /* mov.l @r2,r0 */
L8c6069b0: sa_motion_publish_stop(ctx,0x8c6069b0u); r[4]|=r[0];goto L8c6069b2; /* or r0,r4 */
L8c6069b2: sa_motion_publish_stop(ctx,0x8c6069b4u); wr(m,r[3],r[4],4);return 0; /* rts */
}
