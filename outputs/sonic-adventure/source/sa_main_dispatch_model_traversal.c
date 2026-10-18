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
L8c62113c: r[15]-=4;wr(m,r[15],r[14],4);goto L8c62113e; /* mov.l r14,@-r15 */
L8c62113e: r[14]=r[4];goto L8c621140; /* mov r4,r14 */
L8c621140: r[15]-=4;wr(m,r[15],r[13],4);goto L8c621142; /* mov.l r13,@-r15 */
L8c621142: r[15]-=4;wr(m,r[15],r[12],4);goto L8c621144; /* mov.l r12,@-r15 */
L8c621144: r[15]-=4;wr(m,r[15],r[11],4);goto L8c621146; /* mov.l r11,@-r15 */
L8c621146: r[15]-=4;wr(m,r[15],r[10],4);goto L8c621148; /* mov.l r10,@-r15 */
L8c621148: r[15]-=4;wr(m,r[15],r[9],4);goto L8c62114a; /* mov.l r9,@-r15 */
L8c62114a: r[15]-=4;wr(m,r[15],*pr,4);goto L8c62114c; /* sts.l PR,@-r15 */
L8c62114c: r[0]=rd(m,r[14],4);goto L8c62114e; /* mov.l @r14,r0 */
L8c62114e: r[4]=7u;goto L8c621150; /* mov #0x7,r4 */
L8c621150: r[13]=rd(m,0x8c62124cu,4);goto L8c621152; /* mov.l 0x8c62124c,r13 */
L8c621152: r[0]&=r[4];goto L8c621154; /* and r4,r0 */
L8c621154: r[9]=rd(m,0x8c621240u,4);goto L8c621156; /* mov.l 0x8c621240,r9 */
L8c621156: r[10]=rd(m,0x8c621244u,4);goto L8c621158; /* mov.l 0x8c621244,r10 */
L8c621158: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c62115a; /* cmp/eq #0x0,r0 */
L8c62115a: r[11]=rd(m,0x8c621248u,4);goto L8c62115c; /* mov.l 0x8c621248,r11 */
L8c62115c: take=(*sr&1u);r[12]=16u;if(take)goto L8c621198;goto L8c621160; /* bt/s 0x8c621198 */
L8c621160: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c621162; /* cmp/eq #0x1,r0 */
L8c621162: if((*sr&1u))goto L8c62125c;goto L8c621164; /* bt 0x8c62125c */
L8c621164: *sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c621166; /* cmp/eq #0x2,r0 */
L8c621166: if(!(*sr&1u))goto L8c62116c;goto L8c621168; /* bf 0x8c62116c */
L8c621168: goto L8c6212e4; /* bra 0x8c6212e4 */
L8c62116c: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c62116e; /* cmp/eq #0x3,r0 */
L8c62116e: if(!(*sr&1u))goto L8c621174;goto L8c621170; /* bf 0x8c621174 */
L8c621170: goto L8c621356; /* bra 0x8c621356 */
L8c621174: *sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c621176; /* cmp/eq #0x4,r0 */
L8c621176: if(!(*sr&1u))goto L8c62117c;goto L8c621178; /* bf 0x8c62117c */
L8c621178: goto L8c6213b4; /* bra 0x8c6213b4 */
L8c62117c: *sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c62117e; /* cmp/eq #0x5,r0 */
L8c62117e: if(!(*sr&1u))goto L8c621184;goto L8c621180; /* bf 0x8c621184 */
L8c621180: goto L8c62143a; /* bra 0x8c62143a */
L8c621184: *sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c621186; /* cmp/eq #0x6,r0 */
L8c621186: if(!(*sr&1u))goto L8c62118c;goto L8c621188; /* bf 0x8c62118c */
L8c621188: goto L8c6214b8; /* bra 0x8c6214b8 */
L8c62118c: *sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c62118e; /* cmp/eq #0x7,r0 */
L8c62118e: if(!(*sr&1u))goto L8c621194;goto L8c621190; /* bf 0x8c621194 */
L8c621190: goto L8c62151a; /* bra 0x8c62151a */
L8c621194: goto L8c62155a; /* bra 0x8c62155a */
L8c621198: target=r[10];*pr=0x8c62119cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62119c; /* jsr @r10 */
L8c62119c: r[10]=r[14];goto L8c62119e; /* mov r14,r10 */
L8c62119e: r[0]=8u;goto L8c6211a0; /* mov #0x8,r0 */
L8c6211a0: r[10]+=8u;goto L8c6211a2; /* add #0x8,r10 */
L8c6211a2: r[3]=rd(m,0x8c621250u,4);goto L8c6211a4; /* mov.l 0x8c621250,r3 */
L8c6211a4: fload(m,f,6u,r[0]+r[10]);goto L8c6211a6; /* fmov.s @(r0,r10),fr6 */
L8c6211a6: r[0]=4u;goto L8c6211a8; /* mov #0x4,r0 */
L8c6211a8: fload(m,f,4u,r[10]);goto L8c6211aa; /* fmov.s @r10,fr4 */
L8c6211aa: fload(m,f,5u,r[0]+r[10]);goto L8c6211ac; /* fmov.s @(r0,r10),fr5 */
L8c6211ac: target=r[3];*pr=0x8c6211b0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6211b0; /* jsr @r3 */
L8c6211b0: r[0]=rd(m,r[14],4);goto L8c6211b2; /* mov.l @r14,r0 */
L8c6211b2: *sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c6211b4; /* tst #0x20,r0 */
L8c6211b4: if((*sr&1u))goto L8c6211ca;goto L8c6211b6; /* bt 0x8c6211ca */
L8c6211b6: r[10]=r[14];goto L8c6211b8; /* mov r14,r10 */
L8c6211b8: r[10]+=20u;goto L8c6211ba; /* add #0x14,r10 */
L8c6211ba: r[5]=rd(m,r[10],4);goto L8c6211bc; /* mov.l @r10,r5 */
L8c6211bc: r[3]=rd(m,0x8c621254u,4);goto L8c6211be; /* mov.l 0x8c621254,r3 */
L8c6211be: r[7]=rd(m,r[10]+0x8u,4);goto L8c6211c0; /* mov.l @(0x8,r10),r7 */
L8c6211c0: r[6]=rd(m,r[10]+0x4u,4);goto L8c6211c2; /* mov.l @(0x4,r10),r6 */
L8c6211c2: target=r[3];*pr=0x8c6211c6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6211c6; /* jsr @r3 */
L8c6211c6: goto L8c6211da; /* bra 0x8c6211da */
L8c6211ca: r[10]=r[14];goto L8c6211cc; /* mov r14,r10 */
L8c6211cc: r[10]+=20u;goto L8c6211ce; /* add #0x14,r10 */
L8c6211ce: r[5]=rd(m,r[10],4);goto L8c6211d0; /* mov.l @r10,r5 */
L8c6211d0: r[3]=rd(m,0x8c621258u,4);goto L8c6211d2; /* mov.l 0x8c621258,r3 */
L8c6211d2: r[7]=rd(m,r[10]+0x8u,4);goto L8c6211d4; /* mov.l @(0x8,r10),r7 */
L8c6211d4: r[6]=rd(m,r[10]+0x4u,4);goto L8c6211d6; /* mov.l @(0x4,r10),r6 */
L8c6211d6: target=r[3];*pr=0x8c6211dau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6211da; /* jsr @r3 */
L8c6211da: r[10]=r[14];goto L8c6211dc; /* mov r14,r10 */
L8c6211dc: r[0]=8u;goto L8c6211de; /* mov #0x8,r0 */
L8c6211de: r[10]+=32u;goto L8c6211e0; /* add #0x20,r10 */
L8c6211e0: fload(m,f,6u,r[0]+r[10]);goto L8c6211e2; /* fmov.s @(r0,r10),fr6 */
L8c6211e2: r[0]=4u;goto L8c6211e4; /* mov #0x4,r0 */
L8c6211e4: fload(m,f,4u,r[10]);goto L8c6211e6; /* fmov.s @r10,fr4 */
L8c6211e6: fload(m,f,5u,r[0]+r[10]);goto L8c6211e8; /* fmov.s @(r0,r10),fr5 */
L8c6211e8: target=r[9];*pr=0x8c6211ecu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6211ec; /* jsr @r9 */
L8c6211ec: r[3]=rd(m,r[11],4);goto L8c6211ee; /* mov.l @r11,r3 */
L8c6211ee: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6211f0; /* frchg */
L8c6211f0: r[2]=r[3];goto L8c6211f2; /* mov r3,r2 */
L8c6211f2: r[2]+=64u;goto L8c6211f4; /* add #0x40,r2 */
L8c6211f4: wr(m,r[13],r[3],4);goto L8c6211f6; /* mov.l r3,@r13 */
L8c6211f6: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c6211f8; /* fmov.s fr15,@-r2 */
L8c6211f8: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c6211fa; /* fmov.s fr14,@-r2 */
L8c6211fa: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c6211fc; /* fmov.s fr13,@-r2 */
L8c6211fc: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c6211fe; /* fmov.s fr12,@-r2 */
L8c6211fe: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c621200; /* fmov.s fr11,@-r2 */
L8c621200: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c621202; /* fmov.s fr10,@-r2 */
L8c621202: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c621204; /* fmov.s fr9,@-r2 */
L8c621204: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c621206; /* fmov.s fr8,@-r2 */
L8c621206: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c621208; /* fmov.s fr7,@-r2 */
L8c621208: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c62120a; /* fmov.s fr6,@-r2 */
L8c62120a: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c62120c; /* fmov.s fr5,@-r2 */
L8c62120c: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c62120e; /* fmov.s fr4,@-r2 */
L8c62120e: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c621210; /* fmov.s fr3,@-r2 */
L8c621210: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c621212; /* fmov.s fr2,@-r2 */
L8c621212: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c621214; /* fmov.s fr1,@-r2 */
L8c621214: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c621216; /* fmov.s fr0,@-r2 */
L8c621216: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621218; /* frchg */
L8c621218: r[0]=rd(m,r[14],4);goto L8c62121a; /* mov.l @r14,r0 */
L8c62121a: *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c62121c; /* tst #0x8,r0 */
L8c62121c: if(!(*sr&1u))goto L8c621222;goto L8c62121e; /* bf 0x8c621222 */
L8c62121e: target=0x8c6215bau;*pr=0x8c621222u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621222; /* bsr 0x8c6215ba */
L8c621222: r[3]=rd(m,r[14],4);goto L8c621224; /* mov.l @r14,r3 */
L8c621224: *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c621226; /* tst r3,r12 */
L8c621226: if((*sr&1u))goto L8c62122c;goto L8c621228; /* bt 0x8c62122c */
L8c621228: goto L8c621510; /* bra 0x8c621510 */
L8c62122c: target=0x8c6215fau;*pr=0x8c621230u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621230; /* bsr 0x8c6215fa */
L8c621230: goto L8c621510; /* bra 0x8c621510 */
L8c62125c: target=r[10];*pr=0x8c621260u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621260; /* jsr @r10 */
L8c621260: r[0]=rd(m,r[14],4);goto L8c621262; /* mov.l @r14,r0 */
L8c621262: *sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c621264; /* tst #0x20,r0 */
L8c621264: if((*sr&1u))goto L8c62127a;goto L8c621266; /* bt 0x8c62127a */
L8c621266: r[10]=r[14];goto L8c621268; /* mov r14,r10 */
L8c621268: r[10]+=20u;goto L8c62126a; /* add #0x14,r10 */
L8c62126a: r[5]=rd(m,r[10],4);goto L8c62126c; /* mov.l @r10,r5 */
L8c62126c: r[2]=rd(m,0x8c6214acu,4);goto L8c62126e; /* mov.l 0x8c6214ac,r2 */
L8c62126e: r[7]=rd(m,r[10]+0x8u,4);goto L8c621270; /* mov.l @(0x8,r10),r7 */
L8c621270: r[6]=rd(m,r[10]+0x4u,4);goto L8c621272; /* mov.l @(0x4,r10),r6 */
L8c621272: target=r[2];*pr=0x8c621276u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621276; /* jsr @r2 */
L8c621276: goto L8c62128a; /* bra 0x8c62128a */
L8c62127a: r[10]=r[14];goto L8c62127c; /* mov r14,r10 */
L8c62127c: r[10]+=20u;goto L8c62127e; /* add #0x14,r10 */
L8c62127e: r[5]=rd(m,r[10],4);goto L8c621280; /* mov.l @r10,r5 */
L8c621280: r[2]=rd(m,0x8c6214b0u,4);goto L8c621282; /* mov.l 0x8c6214b0,r2 */
L8c621282: r[7]=rd(m,r[10]+0x8u,4);goto L8c621284; /* mov.l @(0x8,r10),r7 */
L8c621284: r[6]=rd(m,r[10]+0x4u,4);goto L8c621286; /* mov.l @(0x4,r10),r6 */
L8c621286: target=r[2];*pr=0x8c62128au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62128a; /* jsr @r2 */
L8c62128a: r[10]=r[14];goto L8c62128c; /* mov r14,r10 */
L8c62128c: r[0]=8u;goto L8c62128e; /* mov #0x8,r0 */
L8c62128e: r[10]+=32u;goto L8c621290; /* add #0x20,r10 */
L8c621290: fload(m,f,6u,r[0]+r[10]);goto L8c621292; /* fmov.s @(r0,r10),fr6 */
L8c621292: r[0]=4u;goto L8c621294; /* mov #0x4,r0 */
L8c621294: fload(m,f,4u,r[10]);goto L8c621296; /* fmov.s @r10,fr4 */
L8c621296: fload(m,f,5u,r[0]+r[10]);goto L8c621298; /* fmov.s @(r0,r10),fr5 */
L8c621298: target=r[9];*pr=0x8c62129cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62129c; /* jsr @r9 */
L8c62129c: r[3]=rd(m,r[11],4);goto L8c62129e; /* mov.l @r11,r3 */
L8c62129e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6212a0; /* frchg */
L8c6212a0: r[2]=r[3];goto L8c6212a2; /* mov r3,r2 */
L8c6212a2: r[2]+=64u;goto L8c6212a4; /* add #0x40,r2 */
L8c6212a4: wr(m,r[13],r[3],4);goto L8c6212a6; /* mov.l r3,@r13 */
L8c6212a6: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c6212a8; /* fmov.s fr15,@-r2 */
L8c6212a8: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c6212aa; /* fmov.s fr14,@-r2 */
L8c6212aa: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c6212ac; /* fmov.s fr13,@-r2 */
L8c6212ac: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c6212ae; /* fmov.s fr12,@-r2 */
L8c6212ae: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c6212b0; /* fmov.s fr11,@-r2 */
L8c6212b0: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c6212b2; /* fmov.s fr10,@-r2 */
L8c6212b2: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c6212b4; /* fmov.s fr9,@-r2 */
L8c6212b4: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c6212b6; /* fmov.s fr8,@-r2 */
L8c6212b6: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c6212b8; /* fmov.s fr7,@-r2 */
L8c6212b8: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c6212ba; /* fmov.s fr6,@-r2 */
L8c6212ba: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c6212bc; /* fmov.s fr5,@-r2 */
L8c6212bc: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c6212be; /* fmov.s fr4,@-r2 */
L8c6212be: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c6212c0; /* fmov.s fr3,@-r2 */
L8c6212c0: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c6212c2; /* fmov.s fr2,@-r2 */
L8c6212c2: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c6212c4; /* fmov.s fr1,@-r2 */
L8c6212c4: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c6212c6; /* fmov.s fr0,@-r2 */
L8c6212c6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6212c8; /* frchg */
L8c6212c8: r[0]=rd(m,r[14],4);goto L8c6212ca; /* mov.l @r14,r0 */
L8c6212ca: *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c6212cc; /* tst #0x8,r0 */
L8c6212cc: if(!(*sr&1u))goto L8c6212d2;goto L8c6212ce; /* bf 0x8c6212d2 */
L8c6212ce: target=0x8c6215bau;*pr=0x8c6212d2u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6212d2; /* bsr 0x8c6215ba */
L8c6212d2: r[3]=rd(m,r[14],4);goto L8c6212d4; /* mov.l @r14,r3 */
L8c6212d4: *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c6212d6; /* tst r3,r12 */
L8c6212d6: if((*sr&1u))goto L8c6212dc;goto L8c6212d8; /* bt 0x8c6212dc */
L8c6212d8: goto L8c621510; /* bra 0x8c621510 */
L8c6212dc: target=0x8c6215fau;*pr=0x8c6212e0u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6212e0; /* bsr 0x8c6215fa */
L8c6212e0: goto L8c621510; /* bra 0x8c621510 */
L8c6212e4: target=r[10];*pr=0x8c6212e8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6212e8; /* jsr @r10 */
L8c6212e8: r[10]=r[14];goto L8c6212ea; /* mov r14,r10 */
L8c6212ea: r[0]=8u;goto L8c6212ec; /* mov #0x8,r0 */
L8c6212ec: r[10]+=8u;goto L8c6212ee; /* add #0x8,r10 */
L8c6212ee: r[3]=rd(m,0x8c6214b4u,4);goto L8c6212f0; /* mov.l 0x8c6214b4,r3 */
L8c6212f0: fload(m,f,6u,r[0]+r[10]);goto L8c6212f2; /* fmov.s @(r0,r10),fr6 */
L8c6212f2: r[0]=4u;goto L8c6212f4; /* mov #0x4,r0 */
L8c6212f4: fload(m,f,4u,r[10]);goto L8c6212f6; /* fmov.s @r10,fr4 */
L8c6212f6: fload(m,f,5u,r[0]+r[10]);goto L8c6212f8; /* fmov.s @(r0,r10),fr5 */
L8c6212f8: target=r[3];*pr=0x8c6212fcu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6212fc; /* jsr @r3 */
L8c6212fc: r[10]=r[14];goto L8c6212fe; /* mov r14,r10 */
L8c6212fe: r[0]=8u;goto L8c621300; /* mov #0x8,r0 */
L8c621300: r[10]+=32u;goto L8c621302; /* add #0x20,r10 */
L8c621302: fload(m,f,6u,r[0]+r[10]);goto L8c621304; /* fmov.s @(r0,r10),fr6 */
L8c621304: r[0]=4u;goto L8c621306; /* mov #0x4,r0 */
L8c621306: fload(m,f,4u,r[10]);goto L8c621308; /* fmov.s @r10,fr4 */
L8c621308: fload(m,f,5u,r[0]+r[10]);goto L8c62130a; /* fmov.s @(r0,r10),fr5 */
L8c62130a: target=r[9];*pr=0x8c62130eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62130e; /* jsr @r9 */
L8c62130e: r[3]=rd(m,r[11],4);goto L8c621310; /* mov.l @r11,r3 */
L8c621310: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621312; /* frchg */
L8c621312: r[2]=r[3];goto L8c621314; /* mov r3,r2 */
L8c621314: r[2]+=64u;goto L8c621316; /* add #0x40,r2 */
L8c621316: wr(m,r[13],r[3],4);goto L8c621318; /* mov.l r3,@r13 */
L8c621318: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c62131a; /* fmov.s fr15,@-r2 */
L8c62131a: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c62131c; /* fmov.s fr14,@-r2 */
L8c62131c: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c62131e; /* fmov.s fr13,@-r2 */
L8c62131e: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c621320; /* fmov.s fr12,@-r2 */
L8c621320: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c621322; /* fmov.s fr11,@-r2 */
L8c621322: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c621324; /* fmov.s fr10,@-r2 */
L8c621324: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c621326; /* fmov.s fr9,@-r2 */
L8c621326: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c621328; /* fmov.s fr8,@-r2 */
L8c621328: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c62132a; /* fmov.s fr7,@-r2 */
L8c62132a: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c62132c; /* fmov.s fr6,@-r2 */
L8c62132c: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c62132e; /* fmov.s fr5,@-r2 */
L8c62132e: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c621330; /* fmov.s fr4,@-r2 */
L8c621330: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c621332; /* fmov.s fr3,@-r2 */
L8c621332: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c621334; /* fmov.s fr2,@-r2 */
L8c621334: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c621336; /* fmov.s fr1,@-r2 */
L8c621336: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c621338; /* fmov.s fr0,@-r2 */
L8c621338: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c62133a; /* frchg */
L8c62133a: r[0]=rd(m,r[14],4);goto L8c62133c; /* mov.l @r14,r0 */
L8c62133c: *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c62133e; /* tst #0x8,r0 */
L8c62133e: if(!(*sr&1u))goto L8c621344;goto L8c621340; /* bf 0x8c621344 */
L8c621340: target=0x8c6215bau;*pr=0x8c621344u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621344; /* bsr 0x8c6215ba */
L8c621344: r[3]=rd(m,r[14],4);goto L8c621346; /* mov.l @r14,r3 */
L8c621346: *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c621348; /* tst r3,r12 */
L8c621348: if((*sr&1u))goto L8c62134e;goto L8c62134a; /* bt 0x8c62134e */
L8c62134a: goto L8c621510; /* bra 0x8c621510 */
L8c62134e: target=0x8c6215fau;*pr=0x8c621352u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621352; /* bsr 0x8c6215fa */
L8c621352: goto L8c621510; /* bra 0x8c621510 */
L8c621356: target=r[10];*pr=0x8c62135au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62135a; /* jsr @r10 */
L8c62135a: r[10]=r[14];goto L8c62135c; /* mov r14,r10 */
L8c62135c: r[0]=8u;goto L8c62135e; /* mov #0x8,r0 */
L8c62135e: r[10]+=32u;goto L8c621360; /* add #0x20,r10 */
L8c621360: fload(m,f,6u,r[0]+r[10]);goto L8c621362; /* fmov.s @(r0,r10),fr6 */
L8c621362: r[0]=4u;goto L8c621364; /* mov #0x4,r0 */
L8c621364: fload(m,f,4u,r[10]);goto L8c621366; /* fmov.s @r10,fr4 */
L8c621366: fload(m,f,5u,r[0]+r[10]);goto L8c621368; /* fmov.s @(r0,r10),fr5 */
L8c621368: target=r[9];*pr=0x8c62136cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62136c; /* jsr @r9 */
L8c62136c: r[3]=rd(m,r[11],4);goto L8c62136e; /* mov.l @r11,r3 */
L8c62136e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621370; /* frchg */
L8c621370: r[2]=r[3];goto L8c621372; /* mov r3,r2 */
L8c621372: r[2]+=64u;goto L8c621374; /* add #0x40,r2 */
L8c621374: wr(m,r[13],r[3],4);goto L8c621376; /* mov.l r3,@r13 */
L8c621376: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c621378; /* fmov.s fr15,@-r2 */
L8c621378: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c62137a; /* fmov.s fr14,@-r2 */
L8c62137a: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c62137c; /* fmov.s fr13,@-r2 */
L8c62137c: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c62137e; /* fmov.s fr12,@-r2 */
L8c62137e: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c621380; /* fmov.s fr11,@-r2 */
L8c621380: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c621382; /* fmov.s fr10,@-r2 */
L8c621382: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c621384; /* fmov.s fr9,@-r2 */
L8c621384: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c621386; /* fmov.s fr8,@-r2 */
L8c621386: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c621388; /* fmov.s fr7,@-r2 */
L8c621388: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c62138a; /* fmov.s fr6,@-r2 */
L8c62138a: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c62138c; /* fmov.s fr5,@-r2 */
L8c62138c: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c62138e; /* fmov.s fr4,@-r2 */
L8c62138e: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c621390; /* fmov.s fr3,@-r2 */
L8c621390: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c621392; /* fmov.s fr2,@-r2 */
L8c621392: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c621394; /* fmov.s fr1,@-r2 */
L8c621394: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c621396; /* fmov.s fr0,@-r2 */
L8c621396: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621398; /* frchg */
L8c621398: r[0]=rd(m,r[14],4);goto L8c62139a; /* mov.l @r14,r0 */
L8c62139a: *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c62139c; /* tst #0x8,r0 */
L8c62139c: if(!(*sr&1u))goto L8c6213a2;goto L8c62139e; /* bf 0x8c6213a2 */
L8c62139e: target=0x8c6215bau;*pr=0x8c6213a2u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213a2; /* bsr 0x8c6215ba */
L8c6213a2: r[3]=rd(m,r[14],4);goto L8c6213a4; /* mov.l @r14,r3 */
L8c6213a4: *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c6213a6; /* tst r3,r12 */
L8c6213a6: if((*sr&1u))goto L8c6213ac;goto L8c6213a8; /* bt 0x8c6213ac */
L8c6213a8: goto L8c621510; /* bra 0x8c621510 */
L8c6213ac: target=0x8c6215fau;*pr=0x8c6213b0u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213b0; /* bsr 0x8c6215fa */
L8c6213b0: goto L8c621510; /* bra 0x8c621510 */
L8c6213b4: target=r[10];*pr=0x8c6213b8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213b8; /* jsr @r10 */
L8c6213b8: r[10]=r[14];goto L8c6213ba; /* mov r14,r10 */
L8c6213ba: r[0]=8u;goto L8c6213bc; /* mov #0x8,r0 */
L8c6213bc: r[10]+=8u;goto L8c6213be; /* add #0x8,r10 */
L8c6213be: r[3]=rd(m,0x8c6214b4u,4);goto L8c6213c0; /* mov.l 0x8c6214b4,r3 */
L8c6213c0: fload(m,f,6u,r[0]+r[10]);goto L8c6213c2; /* fmov.s @(r0,r10),fr6 */
L8c6213c2: r[0]=4u;goto L8c6213c4; /* mov #0x4,r0 */
L8c6213c4: fload(m,f,4u,r[10]);goto L8c6213c6; /* fmov.s @r10,fr4 */
L8c6213c6: fload(m,f,5u,r[0]+r[10]);goto L8c6213c8; /* fmov.s @(r0,r10),fr5 */
L8c6213c8: target=r[3];*pr=0x8c6213ccu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213cc; /* jsr @r3 */
L8c6213cc: r[0]=rd(m,r[14],4);goto L8c6213ce; /* mov.l @r14,r0 */
L8c6213ce: *sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c6213d0; /* tst #0x20,r0 */
L8c6213d0: if((*sr&1u))goto L8c6213e6;goto L8c6213d2; /* bt 0x8c6213e6 */
L8c6213d2: r[10]=r[14];goto L8c6213d4; /* mov r14,r10 */
L8c6213d4: r[10]+=20u;goto L8c6213d6; /* add #0x14,r10 */
L8c6213d6: r[5]=rd(m,r[10],4);goto L8c6213d8; /* mov.l @r10,r5 */
L8c6213d8: r[3]=rd(m,0x8c6214acu,4);goto L8c6213da; /* mov.l 0x8c6214ac,r3 */
L8c6213da: r[7]=rd(m,r[10]+0x8u,4);goto L8c6213dc; /* mov.l @(0x8,r10),r7 */
L8c6213dc: r[6]=rd(m,r[10]+0x4u,4);goto L8c6213de; /* mov.l @(0x4,r10),r6 */
L8c6213de: target=r[3];*pr=0x8c6213e2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213e2; /* jsr @r3 */
L8c6213e2: goto L8c6213f6; /* bra 0x8c6213f6 */
L8c6213e6: r[10]=r[14];goto L8c6213e8; /* mov r14,r10 */
L8c6213e8: r[10]+=20u;goto L8c6213ea; /* add #0x14,r10 */
L8c6213ea: r[5]=rd(m,r[10],4);goto L8c6213ec; /* mov.l @r10,r5 */
L8c6213ec: r[3]=rd(m,0x8c6214b0u,4);goto L8c6213ee; /* mov.l 0x8c6214b0,r3 */
L8c6213ee: r[7]=rd(m,r[10]+0x8u,4);goto L8c6213f0; /* mov.l @(0x8,r10),r7 */
L8c6213f0: r[6]=rd(m,r[10]+0x4u,4);goto L8c6213f2; /* mov.l @(0x4,r10),r6 */
L8c6213f2: target=r[3];*pr=0x8c6213f6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6213f6; /* jsr @r3 */
L8c6213f6: r[2]=rd(m,r[11],4);goto L8c6213f8; /* mov.l @r11,r2 */
L8c6213f8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6213fa; /* frchg */
L8c6213fa: r[3]=r[2];goto L8c6213fc; /* mov r2,r3 */
L8c6213fc: r[3]+=64u;goto L8c6213fe; /* add #0x40,r3 */
L8c6213fe: wr(m,r[13],r[2],4);goto L8c621400; /* mov.l r2,@r13 */
L8c621400: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[3]);goto L8c621402; /* fmov.s fr15,@-r3 */
L8c621402: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[3]);goto L8c621404; /* fmov.s fr14,@-r3 */
L8c621404: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[3]);goto L8c621406; /* fmov.s fr13,@-r3 */
L8c621406: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[3]);goto L8c621408; /* fmov.s fr12,@-r3 */
L8c621408: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[3]);goto L8c62140a; /* fmov.s fr11,@-r3 */
L8c62140a: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[3]);goto L8c62140c; /* fmov.s fr10,@-r3 */
L8c62140c: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c62140e; /* fmov.s fr9,@-r3 */
L8c62140e: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[3]);goto L8c621410; /* fmov.s fr8,@-r3 */
L8c621410: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[3]);goto L8c621412; /* fmov.s fr7,@-r3 */
L8c621412: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[3]);goto L8c621414; /* fmov.s fr6,@-r3 */
L8c621414: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[3]);goto L8c621416; /* fmov.s fr5,@-r3 */
L8c621416: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[3]);goto L8c621418; /* fmov.s fr4,@-r3 */
L8c621418: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[3]);goto L8c62141a; /* fmov.s fr3,@-r3 */
L8c62141a: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[3]);goto L8c62141c; /* fmov.s fr2,@-r3 */
L8c62141c: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[3]);goto L8c62141e; /* fmov.s fr1,@-r3 */
L8c62141e: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[3]);goto L8c621420; /* fmov.s fr0,@-r3 */
L8c621420: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621422; /* frchg */
L8c621422: r[0]=rd(m,r[14],4);goto L8c621424; /* mov.l @r14,r0 */
L8c621424: *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c621426; /* tst #0x8,r0 */
L8c621426: if(!(*sr&1u))goto L8c62142c;goto L8c621428; /* bf 0x8c62142c */
L8c621428: target=0x8c6215bau;*pr=0x8c62142cu;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62142c; /* bsr 0x8c6215ba */
L8c62142c: r[3]=rd(m,r[14],4);goto L8c62142e; /* mov.l @r14,r3 */
L8c62142e: *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c621430; /* tst r3,r12 */
L8c621430: if(!(*sr&1u))goto L8c621510;goto L8c621432; /* bf 0x8c621510 */
L8c621432: target=0x8c6215fau;*pr=0x8c621436u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621436; /* bsr 0x8c6215fa */
L8c621436: goto L8c621510; /* bra 0x8c621510 */
L8c62143a: target=r[10];*pr=0x8c62143eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62143e; /* jsr @r10 */
L8c62143e: r[0]=rd(m,r[14],4);goto L8c621440; /* mov.l @r14,r0 */
L8c621440: *sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c621442; /* tst #0x20,r0 */
L8c621442: if((*sr&1u))goto L8c621458;goto L8c621444; /* bt 0x8c621458 */
L8c621444: r[10]=r[14];goto L8c621446; /* mov r14,r10 */
L8c621446: r[10]+=20u;goto L8c621448; /* add #0x14,r10 */
L8c621448: r[5]=rd(m,r[10],4);goto L8c62144a; /* mov.l @r10,r5 */
L8c62144a: r[2]=rd(m,0x8c6214acu,4);goto L8c62144c; /* mov.l 0x8c6214ac,r2 */
L8c62144c: r[7]=rd(m,r[10]+0x8u,4);goto L8c62144e; /* mov.l @(0x8,r10),r7 */
L8c62144e: r[6]=rd(m,r[10]+0x4u,4);goto L8c621450; /* mov.l @(0x4,r10),r6 */
L8c621450: target=r[2];*pr=0x8c621454u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621454; /* jsr @r2 */
L8c621454: goto L8c621468; /* bra 0x8c621468 */
L8c621458: r[10]=r[14];goto L8c62145a; /* mov r14,r10 */
L8c62145a: r[10]+=20u;goto L8c62145c; /* add #0x14,r10 */
L8c62145c: r[5]=rd(m,r[10],4);goto L8c62145e; /* mov.l @r10,r5 */
L8c62145e: r[2]=rd(m,0x8c6214b0u,4);goto L8c621460; /* mov.l 0x8c6214b0,r2 */
L8c621460: r[7]=rd(m,r[10]+0x8u,4);goto L8c621462; /* mov.l @(0x8,r10),r7 */
L8c621462: r[6]=rd(m,r[10]+0x4u,4);goto L8c621464; /* mov.l @(0x4,r10),r6 */
L8c621464: target=r[2];*pr=0x8c621468u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621468; /* jsr @r2 */
L8c621468: r[3]=rd(m,r[11],4);goto L8c62146a; /* mov.l @r11,r3 */
L8c62146a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c62146c; /* frchg */
L8c62146c: r[2]=r[3];goto L8c62146e; /* mov r3,r2 */
L8c62146e: r[2]+=64u;goto L8c621470; /* add #0x40,r2 */
L8c621470: wr(m,r[13],r[3],4);goto L8c621472; /* mov.l r3,@r13 */
L8c621472: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c621474; /* fmov.s fr15,@-r2 */
L8c621474: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c621476; /* fmov.s fr14,@-r2 */
L8c621476: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c621478; /* fmov.s fr13,@-r2 */
L8c621478: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c62147a; /* fmov.s fr12,@-r2 */
L8c62147a: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c62147c; /* fmov.s fr11,@-r2 */
L8c62147c: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c62147e; /* fmov.s fr10,@-r2 */
L8c62147e: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c621480; /* fmov.s fr9,@-r2 */
L8c621480: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c621482; /* fmov.s fr8,@-r2 */
L8c621482: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c621484; /* fmov.s fr7,@-r2 */
L8c621484: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c621486; /* fmov.s fr6,@-r2 */
L8c621486: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c621488; /* fmov.s fr5,@-r2 */
L8c621488: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c62148a; /* fmov.s fr4,@-r2 */
L8c62148a: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c62148c; /* fmov.s fr3,@-r2 */
L8c62148c: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c62148e; /* fmov.s fr2,@-r2 */
L8c62148e: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c621490; /* fmov.s fr1,@-r2 */
L8c621490: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c621492; /* fmov.s fr0,@-r2 */
L8c621492: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621494; /* frchg */
L8c621494: r[0]=rd(m,r[14],4);goto L8c621496; /* mov.l @r14,r0 */
L8c621496: *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c621498; /* tst #0x8,r0 */
L8c621498: if(!(*sr&1u))goto L8c62149e;goto L8c62149a; /* bf 0x8c62149e */
L8c62149a: target=0x8c6215bau;*pr=0x8c62149eu;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62149e; /* bsr 0x8c6215ba */
L8c62149e: r[3]=rd(m,r[14],4);goto L8c6214a0; /* mov.l @r14,r3 */
L8c6214a0: *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c6214a2; /* tst r3,r12 */
L8c6214a2: if(!(*sr&1u))goto L8c621510;goto L8c6214a4; /* bf 0x8c621510 */
L8c6214a4: target=0x8c6215fau;*pr=0x8c6214a8u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6214a8; /* bsr 0x8c6215fa */
L8c6214a8: goto L8c621510; /* bra 0x8c621510 */
L8c6214b8: target=r[10];*pr=0x8c6214bcu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6214bc; /* jsr @r10 */
L8c6214bc: r[10]=r[14];goto L8c6214be; /* mov r14,r10 */
L8c6214be: r[0]=8u;goto L8c6214c0; /* mov #0x8,r0 */
L8c6214c0: r[10]+=8u;goto L8c6214c2; /* add #0x8,r10 */
L8c6214c2: r[3]=rd(m,0x8c621614u,4);goto L8c6214c4; /* mov.l 0x8c621614,r3 */
L8c6214c4: fload(m,f,6u,r[0]+r[10]);goto L8c6214c6; /* fmov.s @(r0,r10),fr6 */
L8c6214c6: r[0]=4u;goto L8c6214c8; /* mov #0x4,r0 */
L8c6214c8: fload(m,f,4u,r[10]);goto L8c6214ca; /* fmov.s @r10,fr4 */
L8c6214ca: fload(m,f,5u,r[0]+r[10]);goto L8c6214cc; /* fmov.s @(r0,r10),fr5 */
L8c6214cc: target=r[3];*pr=0x8c6214d0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6214d0; /* jsr @r3 */
L8c6214d0: r[2]=rd(m,r[11],4);goto L8c6214d2; /* mov.l @r11,r2 */
L8c6214d2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6214d4; /* frchg */
L8c6214d4: r[3]=r[2];goto L8c6214d6; /* mov r2,r3 */
L8c6214d6: r[3]+=64u;goto L8c6214d8; /* add #0x40,r3 */
L8c6214d8: wr(m,r[13],r[2],4);goto L8c6214da; /* mov.l r2,@r13 */
L8c6214da: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[3]);goto L8c6214dc; /* fmov.s fr15,@-r3 */
L8c6214dc: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[3]);goto L8c6214de; /* fmov.s fr14,@-r3 */
L8c6214de: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[3]);goto L8c6214e0; /* fmov.s fr13,@-r3 */
L8c6214e0: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[3]);goto L8c6214e2; /* fmov.s fr12,@-r3 */
L8c6214e2: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[3]);goto L8c6214e4; /* fmov.s fr11,@-r3 */
L8c6214e4: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[3]);goto L8c6214e6; /* fmov.s fr10,@-r3 */
L8c6214e6: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c6214e8; /* fmov.s fr9,@-r3 */
L8c6214e8: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[3]);goto L8c6214ea; /* fmov.s fr8,@-r3 */
L8c6214ea: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[3]);goto L8c6214ec; /* fmov.s fr7,@-r3 */
L8c6214ec: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[3]);goto L8c6214ee; /* fmov.s fr6,@-r3 */
L8c6214ee: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[3]);goto L8c6214f0; /* fmov.s fr5,@-r3 */
L8c6214f0: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[3]);goto L8c6214f2; /* fmov.s fr4,@-r3 */
L8c6214f2: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[3]);goto L8c6214f4; /* fmov.s fr3,@-r3 */
L8c6214f4: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[3]);goto L8c6214f6; /* fmov.s fr2,@-r3 */
L8c6214f6: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[3]);goto L8c6214f8; /* fmov.s fr1,@-r3 */
L8c6214f8: r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[3]);goto L8c6214fa; /* fmov.s fr0,@-r3 */
L8c6214fa: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6214fc; /* frchg */
L8c6214fc: r[0]=rd(m,r[14],4);goto L8c6214fe; /* mov.l @r14,r0 */
L8c6214fe: *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c621500; /* tst #0x8,r0 */
L8c621500: if(!(*sr&1u))goto L8c621506;goto L8c621502; /* bf 0x8c621506 */
L8c621502: target=0x8c6215bau;*pr=0x8c621506u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621506; /* bsr 0x8c6215ba */
L8c621506: r[3]=rd(m,r[14],4);goto L8c621508; /* mov.l @r14,r3 */
L8c621508: *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c62150a; /* tst r3,r12 */
L8c62150a: if(!(*sr&1u))goto L8c621510;goto L8c62150c; /* bf 0x8c621510 */
L8c62150c: target=0x8c6215fau;*pr=0x8c621510u;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621510; /* bsr 0x8c6215fa */
L8c621510: r[13]=rd(m,0x8c621618u,4);goto L8c621512; /* mov.l 0x8c621618,r13 */
L8c621512: target=r[13];*pr=0x8c621516u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621516; /* jsr @r13 */
L8c621516: goto L8c62155a; /* bra 0x8c62155a */
L8c62151a: r[3]=rd(m,r[11],4);goto L8c62151c; /* mov.l @r11,r3 */
L8c62151c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c62151e; /* frchg */
L8c62151e: r[2]=r[3];goto L8c621520; /* mov r3,r2 */
L8c621520: r[2]+=64u;goto L8c621522; /* add #0x40,r2 */
L8c621522: wr(m,r[13],r[3],4);goto L8c621524; /* mov.l r3,@r13 */
L8c621524: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[2]);goto L8c621526; /* fmov.s fr15,@-r2 */
L8c621526: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[2]);goto L8c621528; /* fmov.s fr14,@-r2 */
L8c621528: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[2]);goto L8c62152a; /* fmov.s fr13,@-r2 */
L8c62152a: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[2]);goto L8c62152c; /* fmov.s fr12,@-r2 */
L8c62152c: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[2]);goto L8c62152e; /* fmov.s fr11,@-r2 */
L8c62152e: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[2]);goto L8c621530; /* fmov.s fr10,@-r2 */
L8c621530: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[2]);goto L8c621532; /* fmov.s fr9,@-r2 */
L8c621532: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[2]);goto L8c621534; /* fmov.s fr8,@-r2 */
L8c621534: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[2]);goto L8c621536; /* fmov.s fr7,@-r2 */
L8c621536: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c621538; /* fmov.s fr6,@-r2 */
L8c621538: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c62153a; /* fmov.s fr5,@-r2 */
L8c62153a: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);goto L8c62153c; /* fmov.s fr4,@-r2 */
L8c62153c: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[2]);goto L8c62153e; /* fmov.s fr3,@-r2 */
L8c62153e: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[2]);goto L8c621540; /* fmov.s fr2,@-r2 */
L8c621540: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[2]);goto L8c621542; /* fmov.s fr1,@-r2 */
L8c621542: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[2]);goto L8c621544; /* fmov.s fr0,@-r2 */
L8c621544: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c621546; /* frchg */
L8c621546: r[0]=rd(m,r[14],4);goto L8c621548; /* mov.l @r14,r0 */
L8c621548: *sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c62154a; /* tst #0x8,r0 */
L8c62154a: if(!(*sr&1u))goto L8c621550;goto L8c62154c; /* bf 0x8c621550 */
L8c62154c: target=0x8c6215bau;*pr=0x8c621550u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621550; /* bsr 0x8c6215ba */
L8c621550: r[3]=rd(m,r[14],4);goto L8c621552; /* mov.l @r14,r3 */
L8c621552: *sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c621554; /* tst r3,r12 */
L8c621554: if(!(*sr&1u))goto L8c62155a;goto L8c621556; /* bf 0x8c62155a */
L8c621556: target=0x8c6215fau;*pr=0x8c62155au;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62155a; /* bsr 0x8c6215fa */
L8c62155a: r[14]=rd(m,r[14]+0x30u,4);goto L8c62155c; /* mov.l @(0x30,r14),r14 */
L8c62155c: *sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c62155e; /* tst r14,r14 */
L8c62155e: if((*sr&1u))goto L8c621564;goto L8c621560; /* bt 0x8c621564 */
L8c621560: goto L8c62114c; /* bra 0x8c62114c */
L8c621564: *pr=rd(m,r[15],4);r[15]+=4;goto L8c621566; /* lds.l @r15+,PR */
L8c621566: r[9]=rd(m,r[15],4);r[15]+=4;goto L8c621568; /* mov.l @r15+,r9 */
L8c621568: r[10]=rd(m,r[15],4);r[15]+=4;goto L8c62156a; /* mov.l @r15+,r10 */
L8c62156a: r[11]=rd(m,r[15],4);r[15]+=4;goto L8c62156c; /* mov.l @r15+,r11 */
L8c62156c: r[12]=rd(m,r[15],4);r[15]+=4;goto L8c62156e; /* mov.l @r15+,r12 */
L8c62156e: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c621570; /* mov.l @r15+,r13 */
L8c621570: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_model_mesh_wrapper_8c6215ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6215ba;
L8c6215ba: r[15]-=4;wr(m,r[15],*pr,4);goto L8c6215bc; /* sts.l PR,@-r15 */
L8c6215bc: r[15]+=4294967288u;goto L8c6215be; /* add #-0x8,r15 */
L8c6215be: wr(m,r[15]+0x4u,r[4],4);goto L8c6215c0; /* mov.l r4,@(0x4,r15) */
L8c6215c0: r[4]=r[15];goto L8c6215c2; /* mov r15,r4 */
L8c6215c2: wr(m,r[4],r[8],4);goto L8c6215c4; /* mov.l r8,@r4 */
L8c6215c4: r[3]=rd(m,0x8c62162cu,4);goto L8c6215c6; /* mov.l 0x8c62162c,r3 */
L8c6215c6: target=r[3];*pr=0x8c6215cau;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6215ca; /* jsr @r3 */
L8c6215ca: r[4]=r[15];goto L8c6215cc; /* mov r15,r4 */
L8c6215cc: r[8]=rd(m,r[4],4);goto L8c6215ce; /* mov.l @r4,r8 */
L8c6215ce: r[15]+=8u;goto L8c6215d0; /* add #0x8,r15 */
L8c6215d0: *pr=rd(m,r[15],4);r[15]+=4;goto L8c6215d2; /* lds.l @r15+,PR */
L8c6215d2: return 0; /* rts */
}
