#include "sa_draw_motion_stop.h"
/* Recursive model traversal and R8-preserving mesh wrapper; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
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
SA_STAGE_EXPORT int sa_model_traversal_8c62113c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c62113c;
L8c62113c: sa_motion_publish_stop(ctx,0x8c62113cu); r[15]-=4;wr(m,r[15],r[14],4);goto L8c62113e; /* mov.l r14,@-r15 */
L8c62113e: sa_motion_publish_stop(ctx,0x8c62113eu); r[14]=r[4];goto L8c621140; /* mov r4,r14 */
L8c621140: sa_motion_publish_stop(ctx,0x8c621140u); r[15]-=4;wr(m,r[15],r[13],4);goto L8c621142; /* mov.l r13,@-r15 */
L8c621142: sa_motion_publish_stop(ctx,0x8c621142u); r[15]-=4;wr(m,r[15],r[12],4);goto L8c621144; /* mov.l r12,@-r15 */
L8c621144: sa_motion_publish_stop(ctx,0x8c621144u); r[15]-=4;wr(m,r[15],r[11],4);goto L8c621146; /* mov.l r11,@-r15 */
L8c621146: sa_motion_publish_stop(ctx,0x8c621146u); r[15]-=4;wr(m,r[15],r[10],4);goto L8c621148; /* mov.l r10,@-r15 */
L8c621148: sa_motion_publish_stop(ctx,0x8c621148u); r[15]-=4;wr(m,r[15],r[9],4);goto L8c62114a; /* mov.l r9,@-r15 */
L8c62114a: sa_motion_publish_stop(ctx,0x8c62114au); r[15]-=4;wr(m,r[15],*pr,4);goto L8c62114c; /* sts.l PR,@-r15 */
L8c62114c: sa_motion_publish_stop(ctx,0x8c62114cu); r[0]=rd(m,r[14],4);goto L8c62114e; /* mov.l @r14,r0 */
L8c62114e: sa_motion_publish_stop(ctx,0x8c62114eu); r[4]=7u;goto L8c621150; /* mov #0x7,r4 */
L8c621150: sa_motion_publish_stop(ctx,0x8c621150u); r[13]=rd(m,0x8c62124cu,4);goto L8c621152; /* mov.l 0x8c62124c,r13 */
L8c621152: sa_motion_publish_stop(ctx,0x8c621152u); r[0]&=r[4];goto L8c621154; /* and r4,r0 */
L8c621154: sa_motion_publish_stop(ctx,0x8c621154u); r[9]=rd(m,0x8c621240u,4);goto L8c621156; /* mov.l 0x8c621240,r9 */
L8c621156: sa_motion_publish_stop(ctx,0x8c621156u); r[10]=rd(m,0x8c621244u,4);goto L8c621158; /* mov.l 0x8c621244,r10 */
L8c621158: sa_motion_publish_stop(ctx,0x8c621158u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c62115a; /* cmp/eq #0x0,r0 */
L8c62115a: sa_motion_publish_stop(ctx,0x8c62115au); r[11]=rd(m,0x8c621248u,4);goto L8c62115c; /* mov.l 0x8c621248,r11 */
L8c62115c: sa_motion_publish_stop(ctx,0x8c62115eu); take=(*sr&1u);r[12]=16u;if(take)goto L8c621198;goto L8c621160; /* bt/s 0x8c621198 */
L8c621160: sa_motion_publish_stop(ctx,0x8c621160u); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c621162; /* cmp/eq #0x1,r0 */
L8c621162: sa_motion_publish_stop(ctx,0x8c621162u); if((*sr&1u))goto L8c62125c;goto L8c621164; /* bt 0x8c62125c */
L8c621164: sa_motion_publish_stop(ctx,0x8c621164u); *sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c621166; /* cmp/eq #0x2,r0 */
L8c621166: sa_motion_publish_stop(ctx,0x8c621166u); if(!(*sr&1u))goto L8c62116c;goto L8c621168; /* bf 0x8c62116c */
L8c621168: sa_motion_publish_stop(ctx,0x8c62116au); goto L8c6212e4; /* bra 0x8c6212e4 */
L8c62116c: sa_motion_publish_stop(ctx,0x8c62116cu); *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c62116e; /* cmp/eq #0x3,r0 */
L8c62116e: sa_motion_publish_stop(ctx,0x8c62116eu); if(!(*sr&1u))goto L8c621174;goto L8c621170; /* bf 0x8c621174 */
L8c621170: sa_motion_publish_stop(ctx,0x8c621172u); goto L8c621356; /* bra 0x8c621356 */
L8c621174: sa_motion_publish_stop(ctx,0x8c621174u); *sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c621176; /* cmp/eq #0x4,r0 */
L8c621176: sa_motion_publish_stop(ctx,0x8c621176u); if(!(*sr&1u))goto L8c62117c;goto L8c621178; /* bf 0x8c62117c */
L8c621178: sa_motion_publish_stop(ctx,0x8c62117au); goto L8c6213b4; /* bra 0x8c6213b4 */
L8c62117c: sa_motion_publish_stop(ctx,0x8c62117cu); *sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c62117e; /* cmp/eq #0x5,r0 */
L8c62117e: sa_motion_publish_stop(ctx,0x8c62117eu); if(!(*sr&1u))goto L8c621184;goto L8c621180; /* bf 0x8c621184 */
L8c621180: sa_motion_publish_stop(ctx,0x8c621182u); goto L8c62143a; /* bra 0x8c62143a */
L8c621184: sa_motion_publish_stop(ctx,0x8c621184u); *sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c621186; /* cmp/eq #0x6,r0 */
L8c621186: sa_motion_publish_stop(ctx,0x8c621186u); if(!(*sr&1u))goto L8c62118c;goto L8c621188; /* bf 0x8c62118c */
L8c621188: sa_motion_publish_stop(ctx,0x8c62118au); goto L8c6214b8; /* bra 0x8c6214b8 */
L8c62118c: sa_motion_publish_stop(ctx,0x8c62118cu); *sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c62118e; /* cmp/eq #0x7,r0 */
L8c62118e: sa_motion_publish_stop(ctx,0x8c62118eu); if(!(*sr&1u))goto L8c621194;goto L8c621190; /* bf 0x8c621194 */
L8c621190: sa_motion_publish_stop(ctx,0x8c621192u); goto L8c62151a; /* bra 0x8c62151a */
L8c621194: sa_motion_publish_stop(ctx,0x8c621196u); goto L8c62155a; /* bra 0x8c62155a */
L8c621198: sa_motion_publish_stop(ctx,0x8c62119au); target=r[10];*pr=0x8c62119cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62119c; /* jsr @r10 */
L8c62119c: sa_motion_publish_stop(ctx,0x8c62119cu); r[10]=r[14];goto L8c62119e; /* mov r14,r10 */
L8c62119e: sa_motion_publish_stop(ctx,0x8c62119eu); r[0]=8u;goto L8c6211a0; /* mov #0x8,r0 */
L8c6211a0: sa_motion_publish_stop(ctx,0x8c6211a0u); r[10]+=8u;goto L8c6211a2; /* add #0x8,r10 */
L8c6211a2: sa_motion_publish_stop(ctx,0x8c6211a2u); r[3]=rd(m,0x8c621250u,4);goto L8c6211a4; /* mov.l 0x8c621250,r3 */
L8c6211a4: sa_motion_publish_stop(ctx,0x8c6211a4u); fload(m,f,6u,r[0]+r[10]);goto L8c6211a6; /* fmov.s @(r0,r10),fr6 */
L8c6211a6: sa_motion_publish_stop(ctx,0x8c6211a6u); r[0]=4u;goto L8c6211a8; /* mov #0x4,r0 */
L8c6211a8: sa_motion_publish_stop(ctx,0x8c6211a8u); fload(m,f,4u,r[10]);goto L8c6211aa; /* fmov.s @r10,fr4 */
L8c6211aa: sa_motion_publish_stop(ctx,0x8c6211aau); fload(m,f,5u,r[0]+r[10]);goto L8c6211ac; /* fmov.s @(r0,r10),fr5 */
L8c6211ac: sa_motion_publish_stop(ctx,0x8c6211aeu); target=r[3];*pr=0x8c6211b0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6211b0; /* jsr @r3 */
L8c6211b0: sa_motion_publish_stop(ctx,0x8c6211b0u); r[0]=rd(m,r[14],4);goto L8c6211b2; /* mov.l @r14,r0 */
L8c6211b2: sa_motion_publish_stop(ctx,0x8c6211b2u); *sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c6211b4; /* tst #0x20,r0 */
L8c6211b4: sa_motion_publish_stop(ctx,0x8c6211b4u); if((*sr&1u))goto L8c6211ca;goto L8c6211b6; /* bt 0x8c6211ca */
L8c6211b6: sa_motion_publish_stop(ctx,0x8c6211b6u); r[10]=r[14];goto L8c6211b8; /* mov r14,r10 */
L8c6211b8: sa_motion_publish_stop(ctx,0x8c6211b8u); r[10]+=20u;goto L8c6211ba; /* add #0x14,r10 */
L8c6211ba: sa_motion_publish_stop(ctx,0x8c6211bau); r[5]=rd(m,r[10],4);goto L8c6211bc; /* mov.l @r10,r5 */
L8c6211bc: sa_motion_publish_stop(ctx,0x8c6211bcu); r[3]=rd(m,0x8c621254u,4);goto L8c6211be; /* mov.l 0x8c621254,r3 */
L8c6211be: sa_motion_publish_stop(ctx,0x8c6211beu); r[7]=rd(m,r[10]+0x8u,4);goto L8c6211c0; /* mov.l @(0x8,r10),r7 */
L8c6211c0: sa_motion_publish_stop(ctx,0x8c6211c0u); r[6]=rd(m,r[10]+0x4u,4);goto L8c6211c2; /* mov.l @(0x4,r10),r6 */
L8c6211c2: sa_motion_publish_stop(ctx,0x8c6211c4u); target=r[3];*pr=0x8c6211c6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6211c6; /* jsr @r3 */
L8c6211c6: sa_motion_publish_stop(ctx,0x8c6211c8u); goto L8c6211da; /* bra 0x8c6211da */
L8c6211ca: sa_motion_publish_stop(ctx,0x8c6211cau); r[10]=r[14];goto L8c6211cc; /* mov r14,r10 */
L8c6211cc: sa_motion_publish_stop(ctx,0x8c6211ccu); r[10]+=20u;goto L8c6211ce; /* add #0x14,r10 */
L8c6211ce: sa_motion_publish_stop(ctx,0x8c6211ceu); r[5]=rd(m,r[10],4);goto L8c6211d0; /* mov.l @r10,r5 */
L8c6211d0: sa_motion_publish_stop(ctx,0x8c6211d0u); r[3]=rd(m,0x8c621258u,4);goto L8c6211d2; /* mov.l 0x8c621258,r3 */
L8c6211d2: sa_motion_publish_stop(ctx,0x8c6211d2u); r[7]=rd(m,r[10]+0x8u,4);goto L8c6211d4; /* mov.l @(0x8,r10),r7 */
L8c6211d4: sa_motion_publish_stop(ctx,0x8c6211d4u); r[6]=rd(m,r[10]+0x4u,4);goto L8c6211d6; /* mov.l @(0x4,r10),r6 */
L8c6211d6: sa_motion_publish_stop(ctx,0x8c6211d8u); target=r[3];*pr=0x8c6211dau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6211da; /* jsr @r3 */
L8c6211da: sa_motion_publish_stop(ctx,0x8c6211dau); r[10]=r[14];goto L8c6211dc; /* mov r14,r10 */
L8c6211dc: sa_motion_publish_stop(ctx,0x8c6211dcu); r[0]=8u;goto L8c6211de; /* mov #0x8,r0 */
L8c6211de: sa_motion_publish_stop(ctx,0x8c6211deu); r[10]+=32u;goto L8c6211e0; /* add #0x20,r10 */
L8c6211e0: sa_motion_publish_stop(ctx,0x8c6211e0u); fload(m,f,6u,r[0]+r[10]);goto L8c6211e2; /* fmov.s @(r0,r10),fr6 */
L8c6211e2: sa_motion_publish_stop(ctx,0x8c6211e2u); r[0]=4u;goto L8c6211e4; /* mov #0x4,r0 */
L8c6211e4: sa_motion_publish_stop(ctx,0x8c6211e4u); fload(m,f,4u,r[10]);goto L8c6211e6; /* fmov.s @r10,fr4 */
L8c6211e6: sa_motion_publish_stop(ctx,0x8c6211e6u); fload(m,f,5u,r[0]+r[10]);goto L8c6211e8; /* fmov.s @(r0,r10),fr5 */
L8c6211e8: sa_motion_publish_stop(ctx,0x8c6211eau); target=r[9];*pr=0x8c6211ecu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6211ec; /* jsr @r9 */
L8c6211ec: sa_motion_publish_stop(ctx,0x8c6211ecu); r[3]=rd(m,r[11],4);goto L8c6211ee; /* mov.l @r11,r3 */
L8c6211ee: sa_motion_publish_stop(ctx,0x8c6211eeu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6211f0; /* frchg */
L8c6211f0: sa_motion_publish_stop(ctx,0x8c6211f0u); r[2]=r[3];goto L8c6211f2; /* mov r3,r2 */
L8c6211f2: sa_motion_publish_stop(ctx,0x8c6211f2u); r[2]+=64u;goto L8c6211f4; /* add #0x40,r2 */
L8c6211f4: sa_motion_publish_stop(ctx,0x8c6211f4u); wr(m,r[13],r[3],4);goto L8c6211f6; /* mov.l r3,@r13 */
L8c6211f6: sa_motion_publish_stop(ctx,0x8c6211f6u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c6211f8; /* fmov.s fr15,@-r2 */
L8c6211f8: sa_motion_publish_stop(ctx,0x8c6211f8u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c6211fa; /* fmov.s fr14,@-r2 */
L8c6211fa: sa_motion_publish_stop(ctx,0x8c6211fau); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c6211fc; /* fmov.s fr13,@-r2 */
L8c6211fc: sa_motion_publish_stop(ctx,0x8c6211fcu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c6211fe; /* fmov.s fr12,@-r2 */
L8c6211fe: sa_motion_publish_stop(ctx,0x8c6211feu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c621200; /* fmov.s fr11,@-r2 */
L8c621200: sa_motion_publish_stop(ctx,0x8c621200u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c621202; /* fmov.s fr10,@-r2 */
L8c621202: sa_motion_publish_stop(ctx,0x8c621202u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c621204; /* fmov.s fr9,@-r2 */
L8c621204: sa_motion_publish_stop(ctx,0x8c621204u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c621206; /* fmov.s fr8,@-r2 */
L8c621206: sa_motion_publish_stop(ctx,0x8c621206u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c621208; /* fmov.s fr7,@-r2 */
L8c621208: sa_motion_publish_stop(ctx,0x8c621208u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c62120a; /* fmov.s fr6,@-r2 */
L8c62120a: sa_motion_publish_stop(ctx,0x8c62120au); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c62120c; /* fmov.s fr5,@-r2 */
L8c62120c: sa_motion_publish_stop(ctx,0x8c62120cu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c62120e; /* fmov.s fr4,@-r2 */
L8c62120e: sa_motion_publish_stop(ctx,0x8c62120eu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c621210; /* fmov.s fr3,@-r2 */
L8c621210: sa_motion_publish_stop(ctx,0x8c621210u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c621212; /* fmov.s fr2,@-r2 */
L8c621212: sa_motion_publish_stop(ctx,0x8c621212u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c621214; /* fmov.s fr1,@-r2 */
L8c621214: sa_motion_publish_stop(ctx,0x8c621214u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c621216; /* fmov.s fr0,@-r2 */
L8c621216: sa_motion_publish_stop(ctx,0x8c621216u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621218; /* frchg */
L8c621218: sa_motion_publish_stop(ctx,0x8c621218u); r[0]=rd(m,r[14],4);goto L8c62121a; /* mov.l @r14,r0 */
L8c62121a: sa_motion_publish_stop(ctx,0x8c62121au); *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c62121c; /* tst #0x8,r0 */
L8c62121c: sa_motion_publish_stop(ctx,0x8c62121cu); if(!(*sr&1u))goto L8c621222;goto L8c62121e; /* bf 0x8c621222 */
L8c62121e: sa_motion_publish_stop(ctx,0x8c621220u); target=0x8c6215bau;*pr=0x8c621222u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621222; /* bsr 0x8c6215ba */
L8c621222: sa_motion_publish_stop(ctx,0x8c621222u); r[3]=rd(m,r[14],4);goto L8c621224; /* mov.l @r14,r3 */
L8c621224: sa_motion_publish_stop(ctx,0x8c621224u); *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c621226; /* tst r3,r12 */
L8c621226: sa_motion_publish_stop(ctx,0x8c621226u); if((*sr&1u))goto L8c62122c;goto L8c621228; /* bt 0x8c62122c */
L8c621228: sa_motion_publish_stop(ctx,0x8c62122au); goto L8c621510; /* bra 0x8c621510 */
L8c62122c: sa_motion_publish_stop(ctx,0x8c62122eu); target=0x8c6215fau;*pr=0x8c621230u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621230; /* bsr 0x8c6215fa */
L8c621230: sa_motion_publish_stop(ctx,0x8c621232u); goto L8c621510; /* bra 0x8c621510 */
L8c62125c: sa_motion_publish_stop(ctx,0x8c62125eu); target=r[10];*pr=0x8c621260u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621260; /* jsr @r10 */
L8c621260: sa_motion_publish_stop(ctx,0x8c621260u); r[0]=rd(m,r[14],4);goto L8c621262; /* mov.l @r14,r0 */
L8c621262: sa_motion_publish_stop(ctx,0x8c621262u); *sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c621264; /* tst #0x20,r0 */
L8c621264: sa_motion_publish_stop(ctx,0x8c621264u); if((*sr&1u))goto L8c62127a;goto L8c621266; /* bt 0x8c62127a */
L8c621266: sa_motion_publish_stop(ctx,0x8c621266u); r[10]=r[14];goto L8c621268; /* mov r14,r10 */
L8c621268: sa_motion_publish_stop(ctx,0x8c621268u); r[10]+=20u;goto L8c62126a; /* add #0x14,r10 */
L8c62126a: sa_motion_publish_stop(ctx,0x8c62126au); r[5]=rd(m,r[10],4);goto L8c62126c; /* mov.l @r10,r5 */
L8c62126c: sa_motion_publish_stop(ctx,0x8c62126cu); r[2]=rd(m,0x8c6214acu,4);goto L8c62126e; /* mov.l 0x8c6214ac,r2 */
L8c62126e: sa_motion_publish_stop(ctx,0x8c62126eu); r[7]=rd(m,r[10]+0x8u,4);goto L8c621270; /* mov.l @(0x8,r10),r7 */
L8c621270: sa_motion_publish_stop(ctx,0x8c621270u); r[6]=rd(m,r[10]+0x4u,4);goto L8c621272; /* mov.l @(0x4,r10),r6 */
L8c621272: sa_motion_publish_stop(ctx,0x8c621274u); target=r[2];*pr=0x8c621276u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621276; /* jsr @r2 */
L8c621276: sa_motion_publish_stop(ctx,0x8c621278u); goto L8c62128a; /* bra 0x8c62128a */
L8c62127a: sa_motion_publish_stop(ctx,0x8c62127au); r[10]=r[14];goto L8c62127c; /* mov r14,r10 */
L8c62127c: sa_motion_publish_stop(ctx,0x8c62127cu); r[10]+=20u;goto L8c62127e; /* add #0x14,r10 */
L8c62127e: sa_motion_publish_stop(ctx,0x8c62127eu); r[5]=rd(m,r[10],4);goto L8c621280; /* mov.l @r10,r5 */
L8c621280: sa_motion_publish_stop(ctx,0x8c621280u); r[2]=rd(m,0x8c6214b0u,4);goto L8c621282; /* mov.l 0x8c6214b0,r2 */
L8c621282: sa_motion_publish_stop(ctx,0x8c621282u); r[7]=rd(m,r[10]+0x8u,4);goto L8c621284; /* mov.l @(0x8,r10),r7 */
L8c621284: sa_motion_publish_stop(ctx,0x8c621284u); r[6]=rd(m,r[10]+0x4u,4);goto L8c621286; /* mov.l @(0x4,r10),r6 */
L8c621286: sa_motion_publish_stop(ctx,0x8c621288u); target=r[2];*pr=0x8c62128au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62128a; /* jsr @r2 */
L8c62128a: sa_motion_publish_stop(ctx,0x8c62128au); r[10]=r[14];goto L8c62128c; /* mov r14,r10 */
L8c62128c: sa_motion_publish_stop(ctx,0x8c62128cu); r[0]=8u;goto L8c62128e; /* mov #0x8,r0 */
L8c62128e: sa_motion_publish_stop(ctx,0x8c62128eu); r[10]+=32u;goto L8c621290; /* add #0x20,r10 */
L8c621290: sa_motion_publish_stop(ctx,0x8c621290u); fload(m,f,6u,r[0]+r[10]);goto L8c621292; /* fmov.s @(r0,r10),fr6 */
L8c621292: sa_motion_publish_stop(ctx,0x8c621292u); r[0]=4u;goto L8c621294; /* mov #0x4,r0 */
L8c621294: sa_motion_publish_stop(ctx,0x8c621294u); fload(m,f,4u,r[10]);goto L8c621296; /* fmov.s @r10,fr4 */
L8c621296: sa_motion_publish_stop(ctx,0x8c621296u); fload(m,f,5u,r[0]+r[10]);goto L8c621298; /* fmov.s @(r0,r10),fr5 */
L8c621298: sa_motion_publish_stop(ctx,0x8c62129au); target=r[9];*pr=0x8c62129cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62129c; /* jsr @r9 */
L8c62129c: sa_motion_publish_stop(ctx,0x8c62129cu); r[3]=rd(m,r[11],4);goto L8c62129e; /* mov.l @r11,r3 */
L8c62129e: sa_motion_publish_stop(ctx,0x8c62129eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6212a0; /* frchg */
L8c6212a0: sa_motion_publish_stop(ctx,0x8c6212a0u); r[2]=r[3];goto L8c6212a2; /* mov r3,r2 */
L8c6212a2: sa_motion_publish_stop(ctx,0x8c6212a2u); r[2]+=64u;goto L8c6212a4; /* add #0x40,r2 */
L8c6212a4: sa_motion_publish_stop(ctx,0x8c6212a4u); wr(m,r[13],r[3],4);goto L8c6212a6; /* mov.l r3,@r13 */
L8c6212a6: sa_motion_publish_stop(ctx,0x8c6212a6u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c6212a8; /* fmov.s fr15,@-r2 */
L8c6212a8: sa_motion_publish_stop(ctx,0x8c6212a8u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c6212aa; /* fmov.s fr14,@-r2 */
L8c6212aa: sa_motion_publish_stop(ctx,0x8c6212aau); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c6212ac; /* fmov.s fr13,@-r2 */
L8c6212ac: sa_motion_publish_stop(ctx,0x8c6212acu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c6212ae; /* fmov.s fr12,@-r2 */
L8c6212ae: sa_motion_publish_stop(ctx,0x8c6212aeu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c6212b0; /* fmov.s fr11,@-r2 */
L8c6212b0: sa_motion_publish_stop(ctx,0x8c6212b0u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c6212b2; /* fmov.s fr10,@-r2 */
L8c6212b2: sa_motion_publish_stop(ctx,0x8c6212b2u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c6212b4; /* fmov.s fr9,@-r2 */
L8c6212b4: sa_motion_publish_stop(ctx,0x8c6212b4u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c6212b6; /* fmov.s fr8,@-r2 */
L8c6212b6: sa_motion_publish_stop(ctx,0x8c6212b6u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c6212b8; /* fmov.s fr7,@-r2 */
L8c6212b8: sa_motion_publish_stop(ctx,0x8c6212b8u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c6212ba; /* fmov.s fr6,@-r2 */
L8c6212ba: sa_motion_publish_stop(ctx,0x8c6212bau); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c6212bc; /* fmov.s fr5,@-r2 */
L8c6212bc: sa_motion_publish_stop(ctx,0x8c6212bcu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c6212be; /* fmov.s fr4,@-r2 */
L8c6212be: sa_motion_publish_stop(ctx,0x8c6212beu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c6212c0; /* fmov.s fr3,@-r2 */
L8c6212c0: sa_motion_publish_stop(ctx,0x8c6212c0u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c6212c2; /* fmov.s fr2,@-r2 */
L8c6212c2: sa_motion_publish_stop(ctx,0x8c6212c2u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c6212c4; /* fmov.s fr1,@-r2 */
L8c6212c4: sa_motion_publish_stop(ctx,0x8c6212c4u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c6212c6; /* fmov.s fr0,@-r2 */
L8c6212c6: sa_motion_publish_stop(ctx,0x8c6212c6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6212c8; /* frchg */
L8c6212c8: sa_motion_publish_stop(ctx,0x8c6212c8u); r[0]=rd(m,r[14],4);goto L8c6212ca; /* mov.l @r14,r0 */
L8c6212ca: sa_motion_publish_stop(ctx,0x8c6212cau); *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c6212cc; /* tst #0x8,r0 */
L8c6212cc: sa_motion_publish_stop(ctx,0x8c6212ccu); if(!(*sr&1u))goto L8c6212d2;goto L8c6212ce; /* bf 0x8c6212d2 */
L8c6212ce: sa_motion_publish_stop(ctx,0x8c6212d0u); target=0x8c6215bau;*pr=0x8c6212d2u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6212d2; /* bsr 0x8c6215ba */
L8c6212d2: sa_motion_publish_stop(ctx,0x8c6212d2u); r[3]=rd(m,r[14],4);goto L8c6212d4; /* mov.l @r14,r3 */
L8c6212d4: sa_motion_publish_stop(ctx,0x8c6212d4u); *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c6212d6; /* tst r3,r12 */
L8c6212d6: sa_motion_publish_stop(ctx,0x8c6212d6u); if((*sr&1u))goto L8c6212dc;goto L8c6212d8; /* bt 0x8c6212dc */
L8c6212d8: sa_motion_publish_stop(ctx,0x8c6212dau); goto L8c621510; /* bra 0x8c621510 */
L8c6212dc: sa_motion_publish_stop(ctx,0x8c6212deu); target=0x8c6215fau;*pr=0x8c6212e0u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6212e0; /* bsr 0x8c6215fa */
L8c6212e0: sa_motion_publish_stop(ctx,0x8c6212e2u); goto L8c621510; /* bra 0x8c621510 */
L8c6212e4: sa_motion_publish_stop(ctx,0x8c6212e6u); target=r[10];*pr=0x8c6212e8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6212e8; /* jsr @r10 */
L8c6212e8: sa_motion_publish_stop(ctx,0x8c6212e8u); r[10]=r[14];goto L8c6212ea; /* mov r14,r10 */
L8c6212ea: sa_motion_publish_stop(ctx,0x8c6212eau); r[0]=8u;goto L8c6212ec; /* mov #0x8,r0 */
L8c6212ec: sa_motion_publish_stop(ctx,0x8c6212ecu); r[10]+=8u;goto L8c6212ee; /* add #0x8,r10 */
L8c6212ee: sa_motion_publish_stop(ctx,0x8c6212eeu); r[3]=rd(m,0x8c6214b4u,4);goto L8c6212f0; /* mov.l 0x8c6214b4,r3 */
L8c6212f0: sa_motion_publish_stop(ctx,0x8c6212f0u); fload(m,f,6u,r[0]+r[10]);goto L8c6212f2; /* fmov.s @(r0,r10),fr6 */
L8c6212f2: sa_motion_publish_stop(ctx,0x8c6212f2u); r[0]=4u;goto L8c6212f4; /* mov #0x4,r0 */
L8c6212f4: sa_motion_publish_stop(ctx,0x8c6212f4u); fload(m,f,4u,r[10]);goto L8c6212f6; /* fmov.s @r10,fr4 */
L8c6212f6: sa_motion_publish_stop(ctx,0x8c6212f6u); fload(m,f,5u,r[0]+r[10]);goto L8c6212f8; /* fmov.s @(r0,r10),fr5 */
L8c6212f8: sa_motion_publish_stop(ctx,0x8c6212fau); target=r[3];*pr=0x8c6212fcu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6212fc; /* jsr @r3 */
L8c6212fc: sa_motion_publish_stop(ctx,0x8c6212fcu); r[10]=r[14];goto L8c6212fe; /* mov r14,r10 */
L8c6212fe: sa_motion_publish_stop(ctx,0x8c6212feu); r[0]=8u;goto L8c621300; /* mov #0x8,r0 */
L8c621300: sa_motion_publish_stop(ctx,0x8c621300u); r[10]+=32u;goto L8c621302; /* add #0x20,r10 */
L8c621302: sa_motion_publish_stop(ctx,0x8c621302u); fload(m,f,6u,r[0]+r[10]);goto L8c621304; /* fmov.s @(r0,r10),fr6 */
L8c621304: sa_motion_publish_stop(ctx,0x8c621304u); r[0]=4u;goto L8c621306; /* mov #0x4,r0 */
L8c621306: sa_motion_publish_stop(ctx,0x8c621306u); fload(m,f,4u,r[10]);goto L8c621308; /* fmov.s @r10,fr4 */
L8c621308: sa_motion_publish_stop(ctx,0x8c621308u); fload(m,f,5u,r[0]+r[10]);goto L8c62130a; /* fmov.s @(r0,r10),fr5 */
L8c62130a: sa_motion_publish_stop(ctx,0x8c62130cu); target=r[9];*pr=0x8c62130eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62130e; /* jsr @r9 */
L8c62130e: sa_motion_publish_stop(ctx,0x8c62130eu); r[3]=rd(m,r[11],4);goto L8c621310; /* mov.l @r11,r3 */
L8c621310: sa_motion_publish_stop(ctx,0x8c621310u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621312; /* frchg */
L8c621312: sa_motion_publish_stop(ctx,0x8c621312u); r[2]=r[3];goto L8c621314; /* mov r3,r2 */
L8c621314: sa_motion_publish_stop(ctx,0x8c621314u); r[2]+=64u;goto L8c621316; /* add #0x40,r2 */
L8c621316: sa_motion_publish_stop(ctx,0x8c621316u); wr(m,r[13],r[3],4);goto L8c621318; /* mov.l r3,@r13 */
L8c621318: sa_motion_publish_stop(ctx,0x8c621318u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c62131a; /* fmov.s fr15,@-r2 */
L8c62131a: sa_motion_publish_stop(ctx,0x8c62131au); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c62131c; /* fmov.s fr14,@-r2 */
L8c62131c: sa_motion_publish_stop(ctx,0x8c62131cu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c62131e; /* fmov.s fr13,@-r2 */
L8c62131e: sa_motion_publish_stop(ctx,0x8c62131eu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c621320; /* fmov.s fr12,@-r2 */
L8c621320: sa_motion_publish_stop(ctx,0x8c621320u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c621322; /* fmov.s fr11,@-r2 */
L8c621322: sa_motion_publish_stop(ctx,0x8c621322u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c621324; /* fmov.s fr10,@-r2 */
L8c621324: sa_motion_publish_stop(ctx,0x8c621324u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c621326; /* fmov.s fr9,@-r2 */
L8c621326: sa_motion_publish_stop(ctx,0x8c621326u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c621328; /* fmov.s fr8,@-r2 */
L8c621328: sa_motion_publish_stop(ctx,0x8c621328u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c62132a; /* fmov.s fr7,@-r2 */
L8c62132a: sa_motion_publish_stop(ctx,0x8c62132au); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c62132c; /* fmov.s fr6,@-r2 */
L8c62132c: sa_motion_publish_stop(ctx,0x8c62132cu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c62132e; /* fmov.s fr5,@-r2 */
L8c62132e: sa_motion_publish_stop(ctx,0x8c62132eu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c621330; /* fmov.s fr4,@-r2 */
L8c621330: sa_motion_publish_stop(ctx,0x8c621330u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c621332; /* fmov.s fr3,@-r2 */
L8c621332: sa_motion_publish_stop(ctx,0x8c621332u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c621334; /* fmov.s fr2,@-r2 */
L8c621334: sa_motion_publish_stop(ctx,0x8c621334u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c621336; /* fmov.s fr1,@-r2 */
L8c621336: sa_motion_publish_stop(ctx,0x8c621336u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c621338; /* fmov.s fr0,@-r2 */
L8c621338: sa_motion_publish_stop(ctx,0x8c621338u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c62133a; /* frchg */
L8c62133a: sa_motion_publish_stop(ctx,0x8c62133au); r[0]=rd(m,r[14],4);goto L8c62133c; /* mov.l @r14,r0 */
L8c62133c: sa_motion_publish_stop(ctx,0x8c62133cu); *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c62133e; /* tst #0x8,r0 */
L8c62133e: sa_motion_publish_stop(ctx,0x8c62133eu); if(!(*sr&1u))goto L8c621344;goto L8c621340; /* bf 0x8c621344 */
L8c621340: sa_motion_publish_stop(ctx,0x8c621342u); target=0x8c6215bau;*pr=0x8c621344u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621344; /* bsr 0x8c6215ba */
L8c621344: sa_motion_publish_stop(ctx,0x8c621344u); r[3]=rd(m,r[14],4);goto L8c621346; /* mov.l @r14,r3 */
L8c621346: sa_motion_publish_stop(ctx,0x8c621346u); *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c621348; /* tst r3,r12 */
L8c621348: sa_motion_publish_stop(ctx,0x8c621348u); if((*sr&1u))goto L8c62134e;goto L8c62134a; /* bt 0x8c62134e */
L8c62134a: sa_motion_publish_stop(ctx,0x8c62134cu); goto L8c621510; /* bra 0x8c621510 */
L8c62134e: sa_motion_publish_stop(ctx,0x8c621350u); target=0x8c6215fau;*pr=0x8c621352u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621352; /* bsr 0x8c6215fa */
L8c621352: sa_motion_publish_stop(ctx,0x8c621354u); goto L8c621510; /* bra 0x8c621510 */
L8c621356: sa_motion_publish_stop(ctx,0x8c621358u); target=r[10];*pr=0x8c62135au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62135a; /* jsr @r10 */
L8c62135a: sa_motion_publish_stop(ctx,0x8c62135au); r[10]=r[14];goto L8c62135c; /* mov r14,r10 */
L8c62135c: sa_motion_publish_stop(ctx,0x8c62135cu); r[0]=8u;goto L8c62135e; /* mov #0x8,r0 */
L8c62135e: sa_motion_publish_stop(ctx,0x8c62135eu); r[10]+=32u;goto L8c621360; /* add #0x20,r10 */
L8c621360: sa_motion_publish_stop(ctx,0x8c621360u); fload(m,f,6u,r[0]+r[10]);goto L8c621362; /* fmov.s @(r0,r10),fr6 */
L8c621362: sa_motion_publish_stop(ctx,0x8c621362u); r[0]=4u;goto L8c621364; /* mov #0x4,r0 */
L8c621364: sa_motion_publish_stop(ctx,0x8c621364u); fload(m,f,4u,r[10]);goto L8c621366; /* fmov.s @r10,fr4 */
L8c621366: sa_motion_publish_stop(ctx,0x8c621366u); fload(m,f,5u,r[0]+r[10]);goto L8c621368; /* fmov.s @(r0,r10),fr5 */
L8c621368: sa_motion_publish_stop(ctx,0x8c62136au); target=r[9];*pr=0x8c62136cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62136c; /* jsr @r9 */
L8c62136c: sa_motion_publish_stop(ctx,0x8c62136cu); r[3]=rd(m,r[11],4);goto L8c62136e; /* mov.l @r11,r3 */
L8c62136e: sa_motion_publish_stop(ctx,0x8c62136eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621370; /* frchg */
L8c621370: sa_motion_publish_stop(ctx,0x8c621370u); r[2]=r[3];goto L8c621372; /* mov r3,r2 */
L8c621372: sa_motion_publish_stop(ctx,0x8c621372u); r[2]+=64u;goto L8c621374; /* add #0x40,r2 */
L8c621374: sa_motion_publish_stop(ctx,0x8c621374u); wr(m,r[13],r[3],4);goto L8c621376; /* mov.l r3,@r13 */
L8c621376: sa_motion_publish_stop(ctx,0x8c621376u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c621378; /* fmov.s fr15,@-r2 */
L8c621378: sa_motion_publish_stop(ctx,0x8c621378u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c62137a; /* fmov.s fr14,@-r2 */
L8c62137a: sa_motion_publish_stop(ctx,0x8c62137au); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c62137c; /* fmov.s fr13,@-r2 */
L8c62137c: sa_motion_publish_stop(ctx,0x8c62137cu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c62137e; /* fmov.s fr12,@-r2 */
L8c62137e: sa_motion_publish_stop(ctx,0x8c62137eu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c621380; /* fmov.s fr11,@-r2 */
L8c621380: sa_motion_publish_stop(ctx,0x8c621380u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c621382; /* fmov.s fr10,@-r2 */
L8c621382: sa_motion_publish_stop(ctx,0x8c621382u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c621384; /* fmov.s fr9,@-r2 */
L8c621384: sa_motion_publish_stop(ctx,0x8c621384u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c621386; /* fmov.s fr8,@-r2 */
L8c621386: sa_motion_publish_stop(ctx,0x8c621386u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c621388; /* fmov.s fr7,@-r2 */
L8c621388: sa_motion_publish_stop(ctx,0x8c621388u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c62138a; /* fmov.s fr6,@-r2 */
L8c62138a: sa_motion_publish_stop(ctx,0x8c62138au); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c62138c; /* fmov.s fr5,@-r2 */
L8c62138c: sa_motion_publish_stop(ctx,0x8c62138cu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c62138e; /* fmov.s fr4,@-r2 */
L8c62138e: sa_motion_publish_stop(ctx,0x8c62138eu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c621390; /* fmov.s fr3,@-r2 */
L8c621390: sa_motion_publish_stop(ctx,0x8c621390u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c621392; /* fmov.s fr2,@-r2 */
L8c621392: sa_motion_publish_stop(ctx,0x8c621392u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c621394; /* fmov.s fr1,@-r2 */
L8c621394: sa_motion_publish_stop(ctx,0x8c621394u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c621396; /* fmov.s fr0,@-r2 */
L8c621396: sa_motion_publish_stop(ctx,0x8c621396u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621398; /* frchg */
L8c621398: sa_motion_publish_stop(ctx,0x8c621398u); r[0]=rd(m,r[14],4);goto L8c62139a; /* mov.l @r14,r0 */
L8c62139a: sa_motion_publish_stop(ctx,0x8c62139au); *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c62139c; /* tst #0x8,r0 */
L8c62139c: sa_motion_publish_stop(ctx,0x8c62139cu); if(!(*sr&1u))goto L8c6213a2;goto L8c62139e; /* bf 0x8c6213a2 */
L8c62139e: sa_motion_publish_stop(ctx,0x8c6213a0u); target=0x8c6215bau;*pr=0x8c6213a2u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213a2; /* bsr 0x8c6215ba */
L8c6213a2: sa_motion_publish_stop(ctx,0x8c6213a2u); r[3]=rd(m,r[14],4);goto L8c6213a4; /* mov.l @r14,r3 */
L8c6213a4: sa_motion_publish_stop(ctx,0x8c6213a4u); *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c6213a6; /* tst r3,r12 */
L8c6213a6: sa_motion_publish_stop(ctx,0x8c6213a6u); if((*sr&1u))goto L8c6213ac;goto L8c6213a8; /* bt 0x8c6213ac */
L8c6213a8: sa_motion_publish_stop(ctx,0x8c6213aau); goto L8c621510; /* bra 0x8c621510 */
L8c6213ac: sa_motion_publish_stop(ctx,0x8c6213aeu); target=0x8c6215fau;*pr=0x8c6213b0u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213b0; /* bsr 0x8c6215fa */
L8c6213b0: sa_motion_publish_stop(ctx,0x8c6213b2u); goto L8c621510; /* bra 0x8c621510 */
L8c6213b4: sa_motion_publish_stop(ctx,0x8c6213b6u); target=r[10];*pr=0x8c6213b8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213b8; /* jsr @r10 */
L8c6213b8: sa_motion_publish_stop(ctx,0x8c6213b8u); r[10]=r[14];goto L8c6213ba; /* mov r14,r10 */
L8c6213ba: sa_motion_publish_stop(ctx,0x8c6213bau); r[0]=8u;goto L8c6213bc; /* mov #0x8,r0 */
L8c6213bc: sa_motion_publish_stop(ctx,0x8c6213bcu); r[10]+=8u;goto L8c6213be; /* add #0x8,r10 */
L8c6213be: sa_motion_publish_stop(ctx,0x8c6213beu); r[3]=rd(m,0x8c6214b4u,4);goto L8c6213c0; /* mov.l 0x8c6214b4,r3 */
L8c6213c0: sa_motion_publish_stop(ctx,0x8c6213c0u); fload(m,f,6u,r[0]+r[10]);goto L8c6213c2; /* fmov.s @(r0,r10),fr6 */
L8c6213c2: sa_motion_publish_stop(ctx,0x8c6213c2u); r[0]=4u;goto L8c6213c4; /* mov #0x4,r0 */
L8c6213c4: sa_motion_publish_stop(ctx,0x8c6213c4u); fload(m,f,4u,r[10]);goto L8c6213c6; /* fmov.s @r10,fr4 */
L8c6213c6: sa_motion_publish_stop(ctx,0x8c6213c6u); fload(m,f,5u,r[0]+r[10]);goto L8c6213c8; /* fmov.s @(r0,r10),fr5 */
L8c6213c8: sa_motion_publish_stop(ctx,0x8c6213cau); target=r[3];*pr=0x8c6213ccu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213cc; /* jsr @r3 */
L8c6213cc: sa_motion_publish_stop(ctx,0x8c6213ccu); r[0]=rd(m,r[14],4);goto L8c6213ce; /* mov.l @r14,r0 */
L8c6213ce: sa_motion_publish_stop(ctx,0x8c6213ceu); *sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c6213d0; /* tst #0x20,r0 */
L8c6213d0: sa_motion_publish_stop(ctx,0x8c6213d0u); if((*sr&1u))goto L8c6213e6;goto L8c6213d2; /* bt 0x8c6213e6 */
L8c6213d2: sa_motion_publish_stop(ctx,0x8c6213d2u); r[10]=r[14];goto L8c6213d4; /* mov r14,r10 */
L8c6213d4: sa_motion_publish_stop(ctx,0x8c6213d4u); r[10]+=20u;goto L8c6213d6; /* add #0x14,r10 */
L8c6213d6: sa_motion_publish_stop(ctx,0x8c6213d6u); r[5]=rd(m,r[10],4);goto L8c6213d8; /* mov.l @r10,r5 */
L8c6213d8: sa_motion_publish_stop(ctx,0x8c6213d8u); r[3]=rd(m,0x8c6214acu,4);goto L8c6213da; /* mov.l 0x8c6214ac,r3 */
L8c6213da: sa_motion_publish_stop(ctx,0x8c6213dau); r[7]=rd(m,r[10]+0x8u,4);goto L8c6213dc; /* mov.l @(0x8,r10),r7 */
L8c6213dc: sa_motion_publish_stop(ctx,0x8c6213dcu); r[6]=rd(m,r[10]+0x4u,4);goto L8c6213de; /* mov.l @(0x4,r10),r6 */
L8c6213de: sa_motion_publish_stop(ctx,0x8c6213e0u); target=r[3];*pr=0x8c6213e2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213e2; /* jsr @r3 */
L8c6213e2: sa_motion_publish_stop(ctx,0x8c6213e4u); goto L8c6213f6; /* bra 0x8c6213f6 */
L8c6213e6: sa_motion_publish_stop(ctx,0x8c6213e6u); r[10]=r[14];goto L8c6213e8; /* mov r14,r10 */
L8c6213e8: sa_motion_publish_stop(ctx,0x8c6213e8u); r[10]+=20u;goto L8c6213ea; /* add #0x14,r10 */
L8c6213ea: sa_motion_publish_stop(ctx,0x8c6213eau); r[5]=rd(m,r[10],4);goto L8c6213ec; /* mov.l @r10,r5 */
L8c6213ec: sa_motion_publish_stop(ctx,0x8c6213ecu); r[3]=rd(m,0x8c6214b0u,4);goto L8c6213ee; /* mov.l 0x8c6214b0,r3 */
L8c6213ee: sa_motion_publish_stop(ctx,0x8c6213eeu); r[7]=rd(m,r[10]+0x8u,4);goto L8c6213f0; /* mov.l @(0x8,r10),r7 */
L8c6213f0: sa_motion_publish_stop(ctx,0x8c6213f0u); r[6]=rd(m,r[10]+0x4u,4);goto L8c6213f2; /* mov.l @(0x4,r10),r6 */
L8c6213f2: sa_motion_publish_stop(ctx,0x8c6213f4u); target=r[3];*pr=0x8c6213f6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213f6; /* jsr @r3 */
L8c6213f6: sa_motion_publish_stop(ctx,0x8c6213f6u); r[2]=rd(m,r[11],4);goto L8c6213f8; /* mov.l @r11,r2 */
L8c6213f8: sa_motion_publish_stop(ctx,0x8c6213f8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6213fa; /* frchg */
L8c6213fa: sa_motion_publish_stop(ctx,0x8c6213fau); r[3]=r[2];goto L8c6213fc; /* mov r2,r3 */
L8c6213fc: sa_motion_publish_stop(ctx,0x8c6213fcu); r[3]+=64u;goto L8c6213fe; /* add #0x40,r3 */
L8c6213fe: sa_motion_publish_stop(ctx,0x8c6213feu); wr(m,r[13],r[2],4);goto L8c621400; /* mov.l r2,@r13 */
L8c621400: sa_motion_publish_stop(ctx,0x8c621400u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[3]);goto L8c621402; /* fmov.s fr15,@-r3 */
L8c621402: sa_motion_publish_stop(ctx,0x8c621402u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[3]);goto L8c621404; /* fmov.s fr14,@-r3 */
L8c621404: sa_motion_publish_stop(ctx,0x8c621404u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[3]);goto L8c621406; /* fmov.s fr13,@-r3 */
L8c621406: sa_motion_publish_stop(ctx,0x8c621406u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[3]);goto L8c621408; /* fmov.s fr12,@-r3 */
L8c621408: sa_motion_publish_stop(ctx,0x8c621408u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[3]);goto L8c62140a; /* fmov.s fr11,@-r3 */
L8c62140a: sa_motion_publish_stop(ctx,0x8c62140au); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[3]);goto L8c62140c; /* fmov.s fr10,@-r3 */
L8c62140c: sa_motion_publish_stop(ctx,0x8c62140cu); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c62140e; /* fmov.s fr9,@-r3 */
L8c62140e: sa_motion_publish_stop(ctx,0x8c62140eu); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[3]);goto L8c621410; /* fmov.s fr8,@-r3 */
L8c621410: sa_motion_publish_stop(ctx,0x8c621410u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[3]);goto L8c621412; /* fmov.s fr7,@-r3 */
L8c621412: sa_motion_publish_stop(ctx,0x8c621412u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[3]);goto L8c621414; /* fmov.s fr6,@-r3 */
L8c621414: sa_motion_publish_stop(ctx,0x8c621414u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[3]);goto L8c621416; /* fmov.s fr5,@-r3 */
L8c621416: sa_motion_publish_stop(ctx,0x8c621416u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[3]);goto L8c621418; /* fmov.s fr4,@-r3 */
L8c621418: sa_motion_publish_stop(ctx,0x8c621418u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[3]);goto L8c62141a; /* fmov.s fr3,@-r3 */
L8c62141a: sa_motion_publish_stop(ctx,0x8c62141au); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[3]);goto L8c62141c; /* fmov.s fr2,@-r3 */
L8c62141c: sa_motion_publish_stop(ctx,0x8c62141cu); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[3]);goto L8c62141e; /* fmov.s fr1,@-r3 */
L8c62141e: sa_motion_publish_stop(ctx,0x8c62141eu); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[3]);goto L8c621420; /* fmov.s fr0,@-r3 */
L8c621420: sa_motion_publish_stop(ctx,0x8c621420u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621422; /* frchg */
L8c621422: sa_motion_publish_stop(ctx,0x8c621422u); r[0]=rd(m,r[14],4);goto L8c621424; /* mov.l @r14,r0 */
L8c621424: sa_motion_publish_stop(ctx,0x8c621424u); *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c621426; /* tst #0x8,r0 */
L8c621426: sa_motion_publish_stop(ctx,0x8c621426u); if(!(*sr&1u))goto L8c62142c;goto L8c621428; /* bf 0x8c62142c */
L8c621428: sa_motion_publish_stop(ctx,0x8c62142au); target=0x8c6215bau;*pr=0x8c62142cu;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62142c; /* bsr 0x8c6215ba */
L8c62142c: sa_motion_publish_stop(ctx,0x8c62142cu); r[3]=rd(m,r[14],4);goto L8c62142e; /* mov.l @r14,r3 */
L8c62142e: sa_motion_publish_stop(ctx,0x8c62142eu); *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c621430; /* tst r3,r12 */
L8c621430: sa_motion_publish_stop(ctx,0x8c621430u); if(!(*sr&1u))goto L8c621510;goto L8c621432; /* bf 0x8c621510 */
L8c621432: sa_motion_publish_stop(ctx,0x8c621434u); target=0x8c6215fau;*pr=0x8c621436u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621436; /* bsr 0x8c6215fa */
L8c621436: sa_motion_publish_stop(ctx,0x8c621438u); goto L8c621510; /* bra 0x8c621510 */
L8c62143a: sa_motion_publish_stop(ctx,0x8c62143cu); target=r[10];*pr=0x8c62143eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62143e; /* jsr @r10 */
L8c62143e: sa_motion_publish_stop(ctx,0x8c62143eu); r[0]=rd(m,r[14],4);goto L8c621440; /* mov.l @r14,r0 */
L8c621440: sa_motion_publish_stop(ctx,0x8c621440u); *sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c621442; /* tst #0x20,r0 */
L8c621442: sa_motion_publish_stop(ctx,0x8c621442u); if((*sr&1u))goto L8c621458;goto L8c621444; /* bt 0x8c621458 */
L8c621444: sa_motion_publish_stop(ctx,0x8c621444u); r[10]=r[14];goto L8c621446; /* mov r14,r10 */
L8c621446: sa_motion_publish_stop(ctx,0x8c621446u); r[10]+=20u;goto L8c621448; /* add #0x14,r10 */
L8c621448: sa_motion_publish_stop(ctx,0x8c621448u); r[5]=rd(m,r[10],4);goto L8c62144a; /* mov.l @r10,r5 */
L8c62144a: sa_motion_publish_stop(ctx,0x8c62144au); r[2]=rd(m,0x8c6214acu,4);goto L8c62144c; /* mov.l 0x8c6214ac,r2 */
L8c62144c: sa_motion_publish_stop(ctx,0x8c62144cu); r[7]=rd(m,r[10]+0x8u,4);goto L8c62144e; /* mov.l @(0x8,r10),r7 */
L8c62144e: sa_motion_publish_stop(ctx,0x8c62144eu); r[6]=rd(m,r[10]+0x4u,4);goto L8c621450; /* mov.l @(0x4,r10),r6 */
L8c621450: sa_motion_publish_stop(ctx,0x8c621452u); target=r[2];*pr=0x8c621454u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621454; /* jsr @r2 */
L8c621454: sa_motion_publish_stop(ctx,0x8c621456u); goto L8c621468; /* bra 0x8c621468 */
L8c621458: sa_motion_publish_stop(ctx,0x8c621458u); r[10]=r[14];goto L8c62145a; /* mov r14,r10 */
L8c62145a: sa_motion_publish_stop(ctx,0x8c62145au); r[10]+=20u;goto L8c62145c; /* add #0x14,r10 */
L8c62145c: sa_motion_publish_stop(ctx,0x8c62145cu); r[5]=rd(m,r[10],4);goto L8c62145e; /* mov.l @r10,r5 */
L8c62145e: sa_motion_publish_stop(ctx,0x8c62145eu); r[2]=rd(m,0x8c6214b0u,4);goto L8c621460; /* mov.l 0x8c6214b0,r2 */
L8c621460: sa_motion_publish_stop(ctx,0x8c621460u); r[7]=rd(m,r[10]+0x8u,4);goto L8c621462; /* mov.l @(0x8,r10),r7 */
L8c621462: sa_motion_publish_stop(ctx,0x8c621462u); r[6]=rd(m,r[10]+0x4u,4);goto L8c621464; /* mov.l @(0x4,r10),r6 */
L8c621464: sa_motion_publish_stop(ctx,0x8c621466u); target=r[2];*pr=0x8c621468u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621468; /* jsr @r2 */
L8c621468: sa_motion_publish_stop(ctx,0x8c621468u); r[3]=rd(m,r[11],4);goto L8c62146a; /* mov.l @r11,r3 */
L8c62146a: sa_motion_publish_stop(ctx,0x8c62146au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c62146c; /* frchg */
L8c62146c: sa_motion_publish_stop(ctx,0x8c62146cu); r[2]=r[3];goto L8c62146e; /* mov r3,r2 */
L8c62146e: sa_motion_publish_stop(ctx,0x8c62146eu); r[2]+=64u;goto L8c621470; /* add #0x40,r2 */
L8c621470: sa_motion_publish_stop(ctx,0x8c621470u); wr(m,r[13],r[3],4);goto L8c621472; /* mov.l r3,@r13 */
L8c621472: sa_motion_publish_stop(ctx,0x8c621472u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c621474; /* fmov.s fr15,@-r2 */
L8c621474: sa_motion_publish_stop(ctx,0x8c621474u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c621476; /* fmov.s fr14,@-r2 */
L8c621476: sa_motion_publish_stop(ctx,0x8c621476u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c621478; /* fmov.s fr13,@-r2 */
L8c621478: sa_motion_publish_stop(ctx,0x8c621478u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c62147a; /* fmov.s fr12,@-r2 */
L8c62147a: sa_motion_publish_stop(ctx,0x8c62147au); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c62147c; /* fmov.s fr11,@-r2 */
L8c62147c: sa_motion_publish_stop(ctx,0x8c62147cu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c62147e; /* fmov.s fr10,@-r2 */
L8c62147e: sa_motion_publish_stop(ctx,0x8c62147eu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c621480; /* fmov.s fr9,@-r2 */
L8c621480: sa_motion_publish_stop(ctx,0x8c621480u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c621482; /* fmov.s fr8,@-r2 */
L8c621482: sa_motion_publish_stop(ctx,0x8c621482u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c621484; /* fmov.s fr7,@-r2 */
L8c621484: sa_motion_publish_stop(ctx,0x8c621484u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c621486; /* fmov.s fr6,@-r2 */
L8c621486: sa_motion_publish_stop(ctx,0x8c621486u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c621488; /* fmov.s fr5,@-r2 */
L8c621488: sa_motion_publish_stop(ctx,0x8c621488u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c62148a; /* fmov.s fr4,@-r2 */
L8c62148a: sa_motion_publish_stop(ctx,0x8c62148au); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c62148c; /* fmov.s fr3,@-r2 */
L8c62148c: sa_motion_publish_stop(ctx,0x8c62148cu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c62148e; /* fmov.s fr2,@-r2 */
L8c62148e: sa_motion_publish_stop(ctx,0x8c62148eu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c621490; /* fmov.s fr1,@-r2 */
L8c621490: sa_motion_publish_stop(ctx,0x8c621490u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c621492; /* fmov.s fr0,@-r2 */
L8c621492: sa_motion_publish_stop(ctx,0x8c621492u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621494; /* frchg */
L8c621494: sa_motion_publish_stop(ctx,0x8c621494u); r[0]=rd(m,r[14],4);goto L8c621496; /* mov.l @r14,r0 */
L8c621496: sa_motion_publish_stop(ctx,0x8c621496u); *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c621498; /* tst #0x8,r0 */
L8c621498: sa_motion_publish_stop(ctx,0x8c621498u); if(!(*sr&1u))goto L8c62149e;goto L8c62149a; /* bf 0x8c62149e */
L8c62149a: sa_motion_publish_stop(ctx,0x8c62149cu); target=0x8c6215bau;*pr=0x8c62149eu;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62149e; /* bsr 0x8c6215ba */
L8c62149e: sa_motion_publish_stop(ctx,0x8c62149eu); r[3]=rd(m,r[14],4);goto L8c6214a0; /* mov.l @r14,r3 */
L8c6214a0: sa_motion_publish_stop(ctx,0x8c6214a0u); *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c6214a2; /* tst r3,r12 */
L8c6214a2: sa_motion_publish_stop(ctx,0x8c6214a2u); if(!(*sr&1u))goto L8c621510;goto L8c6214a4; /* bf 0x8c621510 */
L8c6214a4: sa_motion_publish_stop(ctx,0x8c6214a6u); target=0x8c6215fau;*pr=0x8c6214a8u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6214a8; /* bsr 0x8c6215fa */
L8c6214a8: sa_motion_publish_stop(ctx,0x8c6214aau); goto L8c621510; /* bra 0x8c621510 */
L8c6214b8: sa_motion_publish_stop(ctx,0x8c6214bau); target=r[10];*pr=0x8c6214bcu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6214bc; /* jsr @r10 */
L8c6214bc: sa_motion_publish_stop(ctx,0x8c6214bcu); r[10]=r[14];goto L8c6214be; /* mov r14,r10 */
L8c6214be: sa_motion_publish_stop(ctx,0x8c6214beu); r[0]=8u;goto L8c6214c0; /* mov #0x8,r0 */
L8c6214c0: sa_motion_publish_stop(ctx,0x8c6214c0u); r[10]+=8u;goto L8c6214c2; /* add #0x8,r10 */
L8c6214c2: sa_motion_publish_stop(ctx,0x8c6214c2u); r[3]=rd(m,0x8c621614u,4);goto L8c6214c4; /* mov.l 0x8c621614,r3 */
L8c6214c4: sa_motion_publish_stop(ctx,0x8c6214c4u); fload(m,f,6u,r[0]+r[10]);goto L8c6214c6; /* fmov.s @(r0,r10),fr6 */
L8c6214c6: sa_motion_publish_stop(ctx,0x8c6214c6u); r[0]=4u;goto L8c6214c8; /* mov #0x4,r0 */
L8c6214c8: sa_motion_publish_stop(ctx,0x8c6214c8u); fload(m,f,4u,r[10]);goto L8c6214ca; /* fmov.s @r10,fr4 */
L8c6214ca: sa_motion_publish_stop(ctx,0x8c6214cau); fload(m,f,5u,r[0]+r[10]);goto L8c6214cc; /* fmov.s @(r0,r10),fr5 */
L8c6214cc: sa_motion_publish_stop(ctx,0x8c6214ceu); target=r[3];*pr=0x8c6214d0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6214d0; /* jsr @r3 */
L8c6214d0: sa_motion_publish_stop(ctx,0x8c6214d0u); r[2]=rd(m,r[11],4);goto L8c6214d2; /* mov.l @r11,r2 */
L8c6214d2: sa_motion_publish_stop(ctx,0x8c6214d2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6214d4; /* frchg */
L8c6214d4: sa_motion_publish_stop(ctx,0x8c6214d4u); r[3]=r[2];goto L8c6214d6; /* mov r2,r3 */
L8c6214d6: sa_motion_publish_stop(ctx,0x8c6214d6u); r[3]+=64u;goto L8c6214d8; /* add #0x40,r3 */
L8c6214d8: sa_motion_publish_stop(ctx,0x8c6214d8u); wr(m,r[13],r[2],4);goto L8c6214da; /* mov.l r2,@r13 */
L8c6214da: sa_motion_publish_stop(ctx,0x8c6214dau); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[3]);goto L8c6214dc; /* fmov.s fr15,@-r3 */
L8c6214dc: sa_motion_publish_stop(ctx,0x8c6214dcu); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[3]);goto L8c6214de; /* fmov.s fr14,@-r3 */
L8c6214de: sa_motion_publish_stop(ctx,0x8c6214deu); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[3]);goto L8c6214e0; /* fmov.s fr13,@-r3 */
L8c6214e0: sa_motion_publish_stop(ctx,0x8c6214e0u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[3]);goto L8c6214e2; /* fmov.s fr12,@-r3 */
L8c6214e2: sa_motion_publish_stop(ctx,0x8c6214e2u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[3]);goto L8c6214e4; /* fmov.s fr11,@-r3 */
L8c6214e4: sa_motion_publish_stop(ctx,0x8c6214e4u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[3]);goto L8c6214e6; /* fmov.s fr10,@-r3 */
L8c6214e6: sa_motion_publish_stop(ctx,0x8c6214e6u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c6214e8; /* fmov.s fr9,@-r3 */
L8c6214e8: sa_motion_publish_stop(ctx,0x8c6214e8u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[3]);goto L8c6214ea; /* fmov.s fr8,@-r3 */
L8c6214ea: sa_motion_publish_stop(ctx,0x8c6214eau); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[3]);goto L8c6214ec; /* fmov.s fr7,@-r3 */
L8c6214ec: sa_motion_publish_stop(ctx,0x8c6214ecu); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[3]);goto L8c6214ee; /* fmov.s fr6,@-r3 */
L8c6214ee: sa_motion_publish_stop(ctx,0x8c6214eeu); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[3]);goto L8c6214f0; /* fmov.s fr5,@-r3 */
L8c6214f0: sa_motion_publish_stop(ctx,0x8c6214f0u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[3]);goto L8c6214f2; /* fmov.s fr4,@-r3 */
L8c6214f2: sa_motion_publish_stop(ctx,0x8c6214f2u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[3]);goto L8c6214f4; /* fmov.s fr3,@-r3 */
L8c6214f4: sa_motion_publish_stop(ctx,0x8c6214f4u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[3]);goto L8c6214f6; /* fmov.s fr2,@-r3 */
L8c6214f6: sa_motion_publish_stop(ctx,0x8c6214f6u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[3]);goto L8c6214f8; /* fmov.s fr1,@-r3 */
L8c6214f8: sa_motion_publish_stop(ctx,0x8c6214f8u); r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[3]);goto L8c6214fa; /* fmov.s fr0,@-r3 */
L8c6214fa: sa_motion_publish_stop(ctx,0x8c6214fau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6214fc; /* frchg */
L8c6214fc: sa_motion_publish_stop(ctx,0x8c6214fcu); r[0]=rd(m,r[14],4);goto L8c6214fe; /* mov.l @r14,r0 */
L8c6214fe: sa_motion_publish_stop(ctx,0x8c6214feu); *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c621500; /* tst #0x8,r0 */
L8c621500: sa_motion_publish_stop(ctx,0x8c621500u); if(!(*sr&1u))goto L8c621506;goto L8c621502; /* bf 0x8c621506 */
L8c621502: sa_motion_publish_stop(ctx,0x8c621504u); target=0x8c6215bau;*pr=0x8c621506u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621506; /* bsr 0x8c6215ba */
L8c621506: sa_motion_publish_stop(ctx,0x8c621506u); r[3]=rd(m,r[14],4);goto L8c621508; /* mov.l @r14,r3 */
L8c621508: sa_motion_publish_stop(ctx,0x8c621508u); *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c62150a; /* tst r3,r12 */
L8c62150a: sa_motion_publish_stop(ctx,0x8c62150au); if(!(*sr&1u))goto L8c621510;goto L8c62150c; /* bf 0x8c621510 */
L8c62150c: sa_motion_publish_stop(ctx,0x8c62150eu); target=0x8c6215fau;*pr=0x8c621510u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621510; /* bsr 0x8c6215fa */
L8c621510: sa_motion_publish_stop(ctx,0x8c621510u); r[13]=rd(m,0x8c621618u,4);goto L8c621512; /* mov.l 0x8c621618,r13 */
L8c621512: sa_motion_publish_stop(ctx,0x8c621514u); target=r[13];*pr=0x8c621516u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621516; /* jsr @r13 */
L8c621516: sa_motion_publish_stop(ctx,0x8c621518u); goto L8c62155a; /* bra 0x8c62155a */
L8c62151a: sa_motion_publish_stop(ctx,0x8c62151au); r[3]=rd(m,r[11],4);goto L8c62151c; /* mov.l @r11,r3 */
L8c62151c: sa_motion_publish_stop(ctx,0x8c62151cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c62151e; /* frchg */
L8c62151e: sa_motion_publish_stop(ctx,0x8c62151eu); r[2]=r[3];goto L8c621520; /* mov r3,r2 */
L8c621520: sa_motion_publish_stop(ctx,0x8c621520u); r[2]+=64u;goto L8c621522; /* add #0x40,r2 */
L8c621522: sa_motion_publish_stop(ctx,0x8c621522u); wr(m,r[13],r[3],4);goto L8c621524; /* mov.l r3,@r13 */
L8c621524: sa_motion_publish_stop(ctx,0x8c621524u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c621526; /* fmov.s fr15,@-r2 */
L8c621526: sa_motion_publish_stop(ctx,0x8c621526u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c621528; /* fmov.s fr14,@-r2 */
L8c621528: sa_motion_publish_stop(ctx,0x8c621528u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c62152a; /* fmov.s fr13,@-r2 */
L8c62152a: sa_motion_publish_stop(ctx,0x8c62152au); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c62152c; /* fmov.s fr12,@-r2 */
L8c62152c: sa_motion_publish_stop(ctx,0x8c62152cu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c62152e; /* fmov.s fr11,@-r2 */
L8c62152e: sa_motion_publish_stop(ctx,0x8c62152eu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c621530; /* fmov.s fr10,@-r2 */
L8c621530: sa_motion_publish_stop(ctx,0x8c621530u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c621532; /* fmov.s fr9,@-r2 */
L8c621532: sa_motion_publish_stop(ctx,0x8c621532u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c621534; /* fmov.s fr8,@-r2 */
L8c621534: sa_motion_publish_stop(ctx,0x8c621534u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c621536; /* fmov.s fr7,@-r2 */
L8c621536: sa_motion_publish_stop(ctx,0x8c621536u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c621538; /* fmov.s fr6,@-r2 */
L8c621538: sa_motion_publish_stop(ctx,0x8c621538u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c62153a; /* fmov.s fr5,@-r2 */
L8c62153a: sa_motion_publish_stop(ctx,0x8c62153au); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c62153c; /* fmov.s fr4,@-r2 */
L8c62153c: sa_motion_publish_stop(ctx,0x8c62153cu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c62153e; /* fmov.s fr3,@-r2 */
L8c62153e: sa_motion_publish_stop(ctx,0x8c62153eu); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c621540; /* fmov.s fr2,@-r2 */
L8c621540: sa_motion_publish_stop(ctx,0x8c621540u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c621542; /* fmov.s fr1,@-r2 */
L8c621542: sa_motion_publish_stop(ctx,0x8c621542u); r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c621544; /* fmov.s fr0,@-r2 */
L8c621544: sa_motion_publish_stop(ctx,0x8c621544u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621546; /* frchg */
L8c621546: sa_motion_publish_stop(ctx,0x8c621546u); r[0]=rd(m,r[14],4);goto L8c621548; /* mov.l @r14,r0 */
L8c621548: sa_motion_publish_stop(ctx,0x8c621548u); *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c62154a; /* tst #0x8,r0 */
L8c62154a: sa_motion_publish_stop(ctx,0x8c62154au); if(!(*sr&1u))goto L8c621550;goto L8c62154c; /* bf 0x8c621550 */
L8c62154c: sa_motion_publish_stop(ctx,0x8c62154eu); target=0x8c6215bau;*pr=0x8c621550u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621550; /* bsr 0x8c6215ba */
L8c621550: sa_motion_publish_stop(ctx,0x8c621550u); r[3]=rd(m,r[14],4);goto L8c621552; /* mov.l @r14,r3 */
L8c621552: sa_motion_publish_stop(ctx,0x8c621552u); *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c621554; /* tst r3,r12 */
L8c621554: sa_motion_publish_stop(ctx,0x8c621554u); if(!(*sr&1u))goto L8c62155a;goto L8c621556; /* bf 0x8c62155a */
L8c621556: sa_motion_publish_stop(ctx,0x8c621558u); target=0x8c6215fau;*pr=0x8c62155au;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62155a; /* bsr 0x8c6215fa */
L8c62155a: sa_motion_publish_stop(ctx,0x8c62155au); r[14]=rd(m,r[14]+0x30u,4);goto L8c62155c; /* mov.l @(0x30,r14),r14 */
L8c62155c: sa_motion_publish_stop(ctx,0x8c62155cu); *sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c62155e; /* tst r14,r14 */
L8c62155e: sa_motion_publish_stop(ctx,0x8c62155eu); if((*sr&1u))goto L8c621564;goto L8c621560; /* bt 0x8c621564 */
L8c621560: sa_motion_publish_stop(ctx,0x8c621562u); goto L8c62114c; /* bra 0x8c62114c */
L8c621564: sa_motion_publish_stop(ctx,0x8c621564u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c621566; /* lds.l @r15+,PR */
L8c621566: sa_motion_publish_stop(ctx,0x8c621566u); r[9]=rd(m,r[15],4);r[15]+=4;goto L8c621568; /* mov.l @r15+,r9 */
L8c621568: sa_motion_publish_stop(ctx,0x8c621568u); r[10]=rd(m,r[15],4);r[15]+=4;goto L8c62156a; /* mov.l @r15+,r10 */
L8c62156a: sa_motion_publish_stop(ctx,0x8c62156au); r[11]=rd(m,r[15],4);r[15]+=4;goto L8c62156c; /* mov.l @r15+,r11 */
L8c62156c: sa_motion_publish_stop(ctx,0x8c62156cu); r[12]=rd(m,r[15],4);r[15]+=4;goto L8c62156e; /* mov.l @r15+,r12 */
L8c62156e: sa_motion_publish_stop(ctx,0x8c62156eu); r[13]=rd(m,r[15],4);r[15]+=4;goto L8c621570; /* mov.l @r15+,r13 */
L8c621570: sa_motion_publish_stop(ctx,0x8c621572u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_model_mesh_wrapper_8c6215ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6215ba;
L8c6215ba: sa_motion_publish_stop(ctx,0x8c6215bau); r[15]-=4;wr(m,r[15],*pr,4);goto L8c6215bc; /* sts.l PR,@-r15 */
L8c6215bc: sa_motion_publish_stop(ctx,0x8c6215bcu); r[15]+=4294967288u;goto L8c6215be; /* add #-0x8,r15 */
L8c6215be: sa_motion_publish_stop(ctx,0x8c6215beu); wr(m,r[15]+0x4u,r[4],4);goto L8c6215c0; /* mov.l r4,@(0x4,r15) */
L8c6215c0: sa_motion_publish_stop(ctx,0x8c6215c0u); r[4]=r[15];goto L8c6215c2; /* mov r15,r4 */
L8c6215c2: sa_motion_publish_stop(ctx,0x8c6215c2u); wr(m,r[4],r[8],4);goto L8c6215c4; /* mov.l r8,@r4 */
L8c6215c4: sa_motion_publish_stop(ctx,0x8c6215c4u); r[3]=rd(m,0x8c62162cu,4);goto L8c6215c6; /* mov.l 0x8c62162c,r3 */
L8c6215c6: sa_motion_publish_stop(ctx,0x8c6215c8u); target=r[3];*pr=0x8c6215cau;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6215ca; /* jsr @r3 */
L8c6215ca: sa_motion_publish_stop(ctx,0x8c6215cau); r[4]=r[15];goto L8c6215cc; /* mov r15,r4 */
L8c6215cc: sa_motion_publish_stop(ctx,0x8c6215ccu); r[8]=rd(m,r[4],4);goto L8c6215ce; /* mov.l @r4,r8 */
L8c6215ce: sa_motion_publish_stop(ctx,0x8c6215ceu); r[15]+=8u;goto L8c6215d0; /* add #0x8,r15 */
L8c6215d0: sa_motion_publish_stop(ctx,0x8c6215d0u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c6215d2; /* lds.l @r15+,PR */
L8c6215d2: sa_motion_publish_stop(ctx,0x8c6215d4u); return 0; /* rts */
}
