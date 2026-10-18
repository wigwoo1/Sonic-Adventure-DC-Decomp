#include "sa_draw_motion_stop.h"
/* UV/constant-factor primitive family with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_uv_constant_primitive_emit_8c61cf9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61cf9c;
L8c61cf9c: sa_motion_publish_stop(ctx,0x8c61cf9cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61cf9e; /* mov.l r14,@-r15 */
L8c61cf9e: sa_motion_publish_stop(ctx,0x8c61cf9eu); r[0]=0x8c61d058u;goto L8c61cfa0; /* mova 0x8c61d058,r0 */
L8c61cfa0: sa_motion_publish_stop(ctx,0x8c61cfa0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61cfa2; /* mov.l r13,@-r15 */
L8c61cfa2: sa_motion_publish_stop(ctx,0x8c61cfa2u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61cfa4; /* mov.l r12,@-r15 */
L8c61cfa4: sa_motion_publish_stop(ctx,0x8c61cfa4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61cfa6; /* mov.l r11,@-r15 */
L8c61cfa6: sa_motion_publish_stop(ctx,0x8c61cfa6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61cfa8; /* mov.l r10,@-r15 */
L8c61cfa8: sa_motion_publish_stop(ctx,0x8c61cfa8u); r[3]=rd(m,bus,&io_status,0x8c61d060u,4);if(io_status)return io_status;goto L8c61cfaa; /* mov.l 0x8c61d060,r3 */
L8c61cfaa: sa_motion_publish_stop(ctx,0x8c61cfaau); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61cfac; /* mov.l r9,@-r15 */
L8c61cfac: sa_motion_publish_stop(ctx,0x8c61cfacu); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61cfae; /* sts.l PR,@-r15 */
L8c61cfae: sa_motion_publish_stop(ctx,0x8c61cfaeu); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c61cfb0; /* fmov.s @r0,fr6 */
L8c61cfb0: sa_motion_publish_stop(ctx,0x8c61cfb0u); r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61cfb2; /* mov.w @r4,r0 */
L8c61cfb2: sa_motion_publish_stop(ctx,0x8c61cfb2u); r[12]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c61cfb4; /* mov.l @(0x4,r4),r12 */
L8c61cfb4: sa_motion_publish_stop(ctx,0x8c61cfb4u); r[0]=r[0]&65535u;goto L8c61cfb6; /* extu.w r0,r0 */
L8c61cfb6: sa_motion_publish_stop(ctx,0x8c61cfb6u); r[13]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c61cfb8; /* mov.l @(0x14,r4),r13 */
L8c61cfb8: sa_motion_publish_stop(ctx,0x8c61cfb8u); r[0]&=r[3];goto L8c61cfba; /* and r3,r0 */
L8c61cfba: sa_motion_publish_stop(ctx,0x8c61cfbau); r[10]=rd(m,bus,&io_status,0x8c61d05cu,4);if(io_status)return io_status;goto L8c61cfbc; /* mov.l 0x8c61d05c,r10 */
L8c61cfbc: sa_motion_publish_stop(ctx,0x8c61cfbcu); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61cfbe; /* cmp/eq #0x0,r0 */
L8c61cfbe: sa_motion_publish_stop(ctx,0x8c61cfbeu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c61cfc0; /* fldi1 fr7 */
L8c61cfc0: sa_motion_publish_stop(ctx,0x8c61cfc2u); take=(*sr&1u);r[11]=0u;if(take)goto L8c61cfe6;goto L8c61cfc4; /* bt/s 0x8c61cfe6 */
L8c61cfc4: sa_motion_publish_stop(ctx,0x8c61cfc4u); r[1]=sx(m,bus,&io_status,0x8c61d054u,2);if(io_status)return io_status;goto L8c61cfc6; /* mov.w 0x8c61d054,r1 */
L8c61cfc6: sa_motion_publish_stop(ctx,0x8c61cfc6u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61cfc8; /* cmp/eq r1,r0 */
L8c61cfc8: sa_motion_publish_stop(ctx,0x8c61cfc8u); if(!(*sr&1u))goto L8c61cfce;goto L8c61cfca; /* bf 0x8c61cfce */
L8c61cfca: sa_motion_publish_stop(ctx,0x8c61cfccu); goto L8c61d122; /* bra 0x8c61d122 */
L8c61cfce: sa_motion_publish_stop(ctx,0x8c61cfceu); r[1]=rd(m,bus,&io_status,0x8c61d064u,4);if(io_status)return io_status;goto L8c61cfd0; /* mov.l 0x8c61d064,r1 */
L8c61cfd0: sa_motion_publish_stop(ctx,0x8c61cfd0u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61cfd2; /* cmp/eq r1,r0 */
L8c61cfd2: sa_motion_publish_stop(ctx,0x8c61cfd2u); if(!(*sr&1u))goto L8c61cfd8;goto L8c61cfd4; /* bf 0x8c61cfd8 */
L8c61cfd4: sa_motion_publish_stop(ctx,0x8c61cfd6u); goto L8c61d282; /* bra 0x8c61d282 */
L8c61cfd8: sa_motion_publish_stop(ctx,0x8c61cfd8u); r[1]=rd(m,bus,&io_status,0x8c61d060u,4);if(io_status)return io_status;goto L8c61cfda; /* mov.l 0x8c61d060,r1 */
L8c61cfda: sa_motion_publish_stop(ctx,0x8c61cfdau); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61cfdc; /* cmp/eq r1,r0 */
L8c61cfdc: sa_motion_publish_stop(ctx,0x8c61cfdcu); if(!(*sr&1u))goto L8c61cfe2;goto L8c61cfde; /* bf 0x8c61cfe2 */
L8c61cfde: sa_motion_publish_stop(ctx,0x8c61cfe0u); goto L8c61d3cc; /* bra 0x8c61d3cc */
L8c61cfe2: sa_motion_publish_stop(ctx,0x8c61cfe4u); goto L8c61d3f4; /* bra 0x8c61d3f4 */
L8c61cfe6: sa_motion_publish_stop(ctx,0x8c61cfe6u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61cfe8; /* mov.w @(0x2,r4),r0 */
L8c61cfe8: sa_motion_publish_stop(ctx,0x8c61cfe8u); r[1]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c61cfea; /* mov.l @r10,r1 */
L8c61cfea: sa_motion_publish_stop(ctx,0x8c61cfeau); r[0]=r[0]&65535u;goto L8c61cfec; /* extu.w r0,r0 */
L8c61cfec: sa_motion_publish_stop(ctx,0x8c61cfecu); r[2]=r[0];goto L8c61cfee; /* mov r0,r2 */
L8c61cfee: sa_motion_publish_stop(ctx,0x8c61cfeeu); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61cff0; /* shll r0 */
L8c61cff0: sa_motion_publish_stop(ctx,0x8c61cff0u); r[0]+=r[2];goto L8c61cff2; /* add r2,r0 */
L8c61cff2: sa_motion_publish_stop(ctx,0x8c61cff2u); r[1]+=r[0];goto L8c61cff4; /* add r0,r1 */
L8c61cff4: sa_motion_publish_stop(ctx,0x8c61cff4u); wr(m,bus,&io_status,r[10],r[1],4);if(io_status)return io_status;goto L8c61cff6; /* mov.l r1,@r10 */
L8c61cff6: sa_motion_publish_stop(ctx,0x8c61cff8u); r[14]=r[11];goto L8c61d112; /* bra 0x8c61d112 */
L8c61cffa: sa_motion_publish_stop(ctx,0x8c61cffau); r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61cffc; /* mov.w @r12+,r5 */
L8c61cffc: sa_motion_publish_stop(ctx,0x8c61cffcu); r[0]=16u;goto L8c61cffe; /* mov #0x10,r0 */
L8c61cffe: sa_motion_publish_stop(ctx,0x8c61cffeu); r[3]=rd(m,bus,&io_status,0x8c61d068u,4);if(io_status)return io_status;goto L8c61d000; /* mov.l 0x8c61d068,r3 */
L8c61d000: sa_motion_publish_stop(ctx,0x8c61d000u); r[6]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d002; /* mov.w @r12+,r6 */
L8c61d002: sa_motion_publish_stop(ctx,0x8c61d002u); r[5]=r[5]&65535u;goto L8c61d004; /* extu.w r5,r5 */
L8c61d004: sa_motion_publish_stop(ctx,0x8c61d004u); r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61d006; /* mov.l @r3,r11 */
L8c61d006: sa_motion_publish_stop(ctx,0x8c61d006u); r[5]<<=2;goto L8c61d008; /* shll2 r5 */
L8c61d008: sa_motion_publish_stop(ctx,0x8c61d008u); r[6]=r[6]&65535u;goto L8c61d00a; /* extu.w r6,r6 */
L8c61d00a: sa_motion_publish_stop(ctx,0x8c61d00au); r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d00c; /* mov.w @r12+,r7 */
L8c61d00c: sa_motion_publish_stop(ctx,0x8c61d00cu); r[6]<<=2;goto L8c61d00e; /* shll2 r6 */
L8c61d00e: sa_motion_publish_stop(ctx,0x8c61d00eu); r[6]<<=2;goto L8c61d010; /* shll2 r6 */
L8c61d010: sa_motion_publish_stop(ctx,0x8c61d010u); r[5]<<=2;goto L8c61d012; /* shll2 r5 */
L8c61d012: sa_motion_publish_stop(ctx,0x8c61d012u); r[7]=r[7]&65535u;goto L8c61d014; /* extu.w r7,r7 */
L8c61d014: sa_motion_publish_stop(ctx,0x8c61d014u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d016; /* shll r5 */
L8c61d016: sa_motion_publish_stop(ctx,0x8c61d016u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61d018; /* shll r6 */
L8c61d018: sa_motion_publish_stop(ctx,0x8c61d018u); r[6]+=r[11];goto L8c61d01a; /* add r11,r6 */
L8c61d01a: sa_motion_publish_stop(ctx,0x8c61d01au); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d01c; /* fmov.s @(r0,r6),fr4 */
L8c61d01c: sa_motion_publish_stop(ctx,0x8c61d01cu); r[5]+=r[11];goto L8c61d01e; /* add r11,r5 */
L8c61d01e: sa_motion_publish_stop(ctx,0x8c61d01eu); r[7]<<=2;goto L8c61d020; /* shll2 r7 */
L8c61d020: sa_motion_publish_stop(ctx,0x8c61d020u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d022; /* fmov.s @(r0,r5),fr3 */
L8c61d022: sa_motion_publish_stop(ctx,0x8c61d022u); r[7]<<=2;goto L8c61d024; /* shll2 r7 */
L8c61d024: sa_motion_publish_stop(ctx,0x8c61d024u); fcopy(f,2u,4u);goto L8c61d026; /* fmov fr4,fr2 */
L8c61d026: sa_motion_publish_stop(ctx,0x8c61d026u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61d028; /* shll r7 */
L8c61d028: sa_motion_publish_stop(ctx,0x8c61d028u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d02a; /* fsub fr3,fr2 */
L8c61d02a: sa_motion_publish_stop(ctx,0x8c61d02au); r[0]=20u;goto L8c61d02c; /* mov #0x14,r0 */
L8c61d02c: sa_motion_publish_stop(ctx,0x8c61d02cu); r[7]+=r[11];goto L8c61d02e; /* add r11,r7 */
L8c61d02e: sa_motion_publish_stop(ctx,0x8c61d02eu); fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d030; /* fmov.s @(r0,r6),fr5 */
L8c61d030: sa_motion_publish_stop(ctx,0x8c61d030u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d032; /* fmov.s @(r0,r7),fr3 */
L8c61d032: sa_motion_publish_stop(ctx,0x8c61d032u); fcopy(f,1u,5u);goto L8c61d034; /* fmov fr5,fr1 */
L8c61d034: sa_motion_publish_stop(ctx,0x8c61d034u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d036; /* fsub fr5,fr3 */
L8c61d036: sa_motion_publish_stop(ctx,0x8c61d036u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d038; /* fmul fr3,fr2 */
L8c61d038: sa_motion_publish_stop(ctx,0x8c61d038u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d03a; /* fmov.s @(r0,r5),fr3 */
L8c61d03a: sa_motion_publish_stop(ctx,0x8c61d03au); r[0]=16u;goto L8c61d03c; /* mov #0x10,r0 */
L8c61d03c: sa_motion_publish_stop(ctx,0x8c61d03cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d03e; /* fsub fr3,fr1 */
L8c61d03e: sa_motion_publish_stop(ctx,0x8c61d03eu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d040; /* fmov.s @(r0,r7),fr3 */
L8c61d040: sa_motion_publish_stop(ctx,0x8c61d040u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d042; /* fsub fr4,fr3 */
L8c61d042: sa_motion_publish_stop(ctx,0x8c61d042u); fcopy(f,4u,2u);goto L8c61d044; /* fmov fr2,fr4 */
L8c61d044: sa_motion_publish_stop(ctx,0x8c61d044u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d046; /* fmul fr3,fr1 */
L8c61d046: sa_motion_publish_stop(ctx,0x8c61d046u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d048; /* fldi0 fr3 */
L8c61d048: sa_motion_publish_stop(ctx,0x8c61d048u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d04a; /* fsub fr1,fr4 */
L8c61d04a: sa_motion_publish_stop(ctx,0x8c61d04au); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d04c; /* fcmp/gt fr3,fr4 */
L8c61d04c: sa_motion_publish_stop(ctx,0x8c61d04eu); take=!(*sr&1u);r[14]+=1u;if(take)goto L8c61d06c;goto L8c61d050; /* bf/s 0x8c61d06c */
L8c61d050: sa_motion_publish_stop(ctx,0x8c61d052u); r[13]+=12u;goto L8c61d112; /* bra 0x8c61d112 */
L8c61d06c: sa_motion_publish_stop(ctx,0x8c61d06cu); r[8]+=28u;goto L8c61d06e; /* add #0x1c,r8 */
L8c61d06e: sa_motion_publish_stop(ctx,0x8c61d06eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d070; /* fmov.s fr7,@-r8 */
L8c61d070: sa_motion_publish_stop(ctx,0x8c61d070u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d072; /* mov.w @(0x2,r13),r0 */
L8c61d072: sa_motion_publish_stop(ctx,0x8c61d072u); r[3]=r[0];goto L8c61d074; /* mov r0,r3 */
L8c61d074: sa_motion_publish_stop(ctx,0x8c61d074u); *fpul=r[3];goto L8c61d076; /* lds r3,FPUL */
L8c61d076: sa_motion_publish_stop(ctx,0x8c61d076u); r[0]=24u;goto L8c61d078; /* mov #0x18,r0 */
L8c61d078: sa_motion_publish_stop(ctx,0x8c61d078u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d07a; /* float FPUL,fr3 */
L8c61d07a: sa_motion_publish_stop(ctx,0x8c61d07au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d07c; /* fmul fr6,fr3 */
L8c61d07c: sa_motion_publish_stop(ctx,0x8c61d07cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d07e; /* fmov.s fr3,@-r8 */
L8c61d07e: sa_motion_publish_stop(ctx,0x8c61d07eu); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d080; /* mov.w @r13,r3 */
L8c61d080: sa_motion_publish_stop(ctx,0x8c61d080u); r[13]+=4u;goto L8c61d082; /* add #0x4,r13 */
L8c61d082: sa_motion_publish_stop(ctx,0x8c61d082u); *fpul=r[3];goto L8c61d084; /* lds r3,FPUL */
L8c61d084: sa_motion_publish_stop(ctx,0x8c61d084u); r[3]=rd(m,bus,&io_status,0x8c61d2f0u,4);if(io_status)return io_status;goto L8c61d086; /* mov.l 0x8c61d2f0,r3 */
L8c61d086: sa_motion_publish_stop(ctx,0x8c61d086u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d088; /* float FPUL,fr3 */
L8c61d088: sa_motion_publish_stop(ctx,0x8c61d088u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d08a; /* fmul fr6,fr3 */
L8c61d08a: sa_motion_publish_stop(ctx,0x8c61d08au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d08c; /* fmov.s fr3,@-r8 */
L8c61d08c: sa_motion_publish_stop(ctx,0x8c61d08cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d08e; /* fmov.s @(r0,r5),fr3 */
L8c61d08e: sa_motion_publish_stop(ctx,0x8c61d08eu); r[0]=20u;goto L8c61d090; /* mov #0x14,r0 */
L8c61d090: sa_motion_publish_stop(ctx,0x8c61d090u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d092; /* fmov.s fr3,@-r8 */
L8c61d092: sa_motion_publish_stop(ctx,0x8c61d092u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d094; /* fmov.s @(r0,r5),fr3 */
L8c61d094: sa_motion_publish_stop(ctx,0x8c61d094u); r[0]=16u;goto L8c61d096; /* mov #0x10,r0 */
L8c61d096: sa_motion_publish_stop(ctx,0x8c61d096u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d098; /* fmov.s fr3,@-r8 */
L8c61d098: sa_motion_publish_stop(ctx,0x8c61d098u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d09a; /* fmov.s @(r0,r5),fr3 */
L8c61d09a: sa_motion_publish_stop(ctx,0x8c61d09au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d09c; /* fmov.s fr3,@-r8 */
L8c61d09c: sa_motion_publish_stop(ctx,0x8c61d09cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d09e; /* mov.l r3,@-r8 */
L8c61d09e: sa_motion_publish_stop(ctx,0x8c61d09eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d0a0; /* pref @r8 */
L8c61d0a0: sa_motion_publish_stop(ctx,0x8c61d0a0u); r[8]+=60u;goto L8c61d0a2; /* add #0x3c,r8 */
L8c61d0a2: sa_motion_publish_stop(ctx,0x8c61d0a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d0a4; /* fmov.s fr7,@-r8 */
L8c61d0a4: sa_motion_publish_stop(ctx,0x8c61d0a4u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d0a6; /* mov.w @(0x2,r13),r0 */
L8c61d0a6: sa_motion_publish_stop(ctx,0x8c61d0a6u); r[3]=r[0];goto L8c61d0a8; /* mov r0,r3 */
L8c61d0a8: sa_motion_publish_stop(ctx,0x8c61d0a8u); *fpul=r[3];goto L8c61d0aa; /* lds r3,FPUL */
L8c61d0aa: sa_motion_publish_stop(ctx,0x8c61d0aau); r[0]=24u;goto L8c61d0ac; /* mov #0x18,r0 */
L8c61d0ac: sa_motion_publish_stop(ctx,0x8c61d0acu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d0ae; /* float FPUL,fr3 */
L8c61d0ae: sa_motion_publish_stop(ctx,0x8c61d0aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d0b0; /* fmul fr6,fr3 */
L8c61d0b0: sa_motion_publish_stop(ctx,0x8c61d0b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0b2; /* fmov.s fr3,@-r8 */
L8c61d0b2: sa_motion_publish_stop(ctx,0x8c61d0b2u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d0b4; /* mov.w @r13,r3 */
L8c61d0b4: sa_motion_publish_stop(ctx,0x8c61d0b4u); r[13]+=4u;goto L8c61d0b6; /* add #0x4,r13 */
L8c61d0b6: sa_motion_publish_stop(ctx,0x8c61d0b6u); *fpul=r[3];goto L8c61d0b8; /* lds r3,FPUL */
L8c61d0b8: sa_motion_publish_stop(ctx,0x8c61d0b8u); r[3]=rd(m,bus,&io_status,0x8c61d2f0u,4);if(io_status)return io_status;goto L8c61d0ba; /* mov.l 0x8c61d2f0,r3 */
L8c61d0ba: sa_motion_publish_stop(ctx,0x8c61d0bau); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d0bc; /* float FPUL,fr3 */
L8c61d0bc: sa_motion_publish_stop(ctx,0x8c61d0bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d0be; /* fmul fr6,fr3 */
L8c61d0be: sa_motion_publish_stop(ctx,0x8c61d0beu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0c0; /* fmov.s fr3,@-r8 */
L8c61d0c0: sa_motion_publish_stop(ctx,0x8c61d0c0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d0c2; /* fmov.s @(r0,r6),fr3 */
L8c61d0c2: sa_motion_publish_stop(ctx,0x8c61d0c2u); r[0]=20u;goto L8c61d0c4; /* mov #0x14,r0 */
L8c61d0c4: sa_motion_publish_stop(ctx,0x8c61d0c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0c6; /* fmov.s fr3,@-r8 */
L8c61d0c6: sa_motion_publish_stop(ctx,0x8c61d0c6u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d0c8; /* fmov.s @(r0,r6),fr3 */
L8c61d0c8: sa_motion_publish_stop(ctx,0x8c61d0c8u); r[0]=16u;goto L8c61d0ca; /* mov #0x10,r0 */
L8c61d0ca: sa_motion_publish_stop(ctx,0x8c61d0cau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0cc; /* fmov.s fr3,@-r8 */
L8c61d0cc: sa_motion_publish_stop(ctx,0x8c61d0ccu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d0ce; /* fmov.s @(r0,r6),fr3 */
L8c61d0ce: sa_motion_publish_stop(ctx,0x8c61d0ceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0d0; /* fmov.s fr3,@-r8 */
L8c61d0d0: sa_motion_publish_stop(ctx,0x8c61d0d0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d0d2; /* mov.l r3,@-r8 */
L8c61d0d2: sa_motion_publish_stop(ctx,0x8c61d0d2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d0d4; /* pref @r8 */
L8c61d0d4: sa_motion_publish_stop(ctx,0x8c61d0d4u); r[8]+=60u;goto L8c61d0d6; /* add #0x3c,r8 */
L8c61d0d6: sa_motion_publish_stop(ctx,0x8c61d0d6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d0d8; /* fmov.s fr7,@-r8 */
L8c61d0d8: sa_motion_publish_stop(ctx,0x8c61d0d8u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d0da; /* mov.w @(0x2,r13),r0 */
L8c61d0da: sa_motion_publish_stop(ctx,0x8c61d0dau); r[3]=r[0];goto L8c61d0dc; /* mov r0,r3 */
L8c61d0dc: sa_motion_publish_stop(ctx,0x8c61d0dcu); *fpul=r[3];goto L8c61d0de; /* lds r3,FPUL */
L8c61d0de: sa_motion_publish_stop(ctx,0x8c61d0deu); r[0]=24u;goto L8c61d0e0; /* mov #0x18,r0 */
L8c61d0e0: sa_motion_publish_stop(ctx,0x8c61d0e0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d0e2; /* float FPUL,fr3 */
L8c61d0e2: sa_motion_publish_stop(ctx,0x8c61d0e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d0e4; /* fmul fr6,fr3 */
L8c61d0e4: sa_motion_publish_stop(ctx,0x8c61d0e4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0e6; /* fmov.s fr3,@-r8 */
L8c61d0e6: sa_motion_publish_stop(ctx,0x8c61d0e6u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d0e8; /* mov.w @r13,r3 */
L8c61d0e8: sa_motion_publish_stop(ctx,0x8c61d0e8u); r[13]+=4u;goto L8c61d0ea; /* add #0x4,r13 */
L8c61d0ea: sa_motion_publish_stop(ctx,0x8c61d0eau); *fpul=r[3];goto L8c61d0ec; /* lds r3,FPUL */
L8c61d0ec: sa_motion_publish_stop(ctx,0x8c61d0ecu); r[3]=rd(m,bus,&io_status,0x8c61d2f4u,4);if(io_status)return io_status;goto L8c61d0ee; /* mov.l 0x8c61d2f4,r3 */
L8c61d0ee: sa_motion_publish_stop(ctx,0x8c61d0eeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d0f0; /* float FPUL,fr3 */
L8c61d0f0: sa_motion_publish_stop(ctx,0x8c61d0f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d0f2; /* fmul fr6,fr3 */
L8c61d0f2: sa_motion_publish_stop(ctx,0x8c61d0f2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0f4; /* fmov.s fr3,@-r8 */
L8c61d0f4: sa_motion_publish_stop(ctx,0x8c61d0f4u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d0f6; /* fmov.s @(r0,r7),fr3 */
L8c61d0f6: sa_motion_publish_stop(ctx,0x8c61d0f6u); r[0]=20u;goto L8c61d0f8; /* mov #0x14,r0 */
L8c61d0f8: sa_motion_publish_stop(ctx,0x8c61d0f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0fa; /* fmov.s fr3,@-r8 */
L8c61d0fa: sa_motion_publish_stop(ctx,0x8c61d0fau); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d0fc; /* fmov.s @(r0,r7),fr3 */
L8c61d0fc: sa_motion_publish_stop(ctx,0x8c61d0fcu); r[0]=16u;goto L8c61d0fe; /* mov #0x10,r0 */
L8c61d0fe: sa_motion_publish_stop(ctx,0x8c61d0feu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d100; /* fmov.s fr3,@-r8 */
L8c61d100: sa_motion_publish_stop(ctx,0x8c61d100u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d102; /* fmov.s @(r0,r7),fr3 */
L8c61d102: sa_motion_publish_stop(ctx,0x8c61d102u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d104; /* fmov.s fr3,@-r8 */
L8c61d104: sa_motion_publish_stop(ctx,0x8c61d104u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d106; /* mov.l r3,@-r8 */
L8c61d106: sa_motion_publish_stop(ctx,0x8c61d106u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d108; /* pref @r8 */
L8c61d108: sa_motion_publish_stop(ctx,0x8c61d108u); r[2]=rd(m,bus,&io_status,0x8c61d2f8u,4);if(io_status)return io_status;goto L8c61d10a; /* mov.l 0x8c61d2f8,r2 */
L8c61d10a: sa_motion_publish_stop(ctx,0x8c61d10au); r[8]+=32u;goto L8c61d10c; /* add #0x20,r8 */
L8c61d10c: sa_motion_publish_stop(ctx,0x8c61d10cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d10e; /* mov.l @r2,r3 */
L8c61d10e: sa_motion_publish_stop(ctx,0x8c61d10eu); r[3]+=3u;goto L8c61d110; /* add #0x3,r3 */
L8c61d110: sa_motion_publish_stop(ctx,0x8c61d110u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61d112; /* mov.l r3,@r2 */
L8c61d112: sa_motion_publish_stop(ctx,0x8c61d112u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61d114; /* mov.w @(0x2,r4),r0 */
L8c61d114: sa_motion_publish_stop(ctx,0x8c61d114u); r[0]=r[0]&65535u;goto L8c61d116; /* extu.w r0,r0 */
L8c61d116: sa_motion_publish_stop(ctx,0x8c61d116u); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c61d118; /* cmp/ge r0,r14 */
L8c61d118: sa_motion_publish_stop(ctx,0x8c61d118u); if((*sr&1u))goto L8c61d11e;goto L8c61d11a; /* bt 0x8c61d11e */
L8c61d11a: sa_motion_publish_stop(ctx,0x8c61d11cu); goto L8c61cffa; /* bra 0x8c61cffa */
L8c61d11e: sa_motion_publish_stop(ctx,0x8c61d120u); goto L8c61d3f4; /* bra 0x8c61d3f4 */
L8c61d122: sa_motion_publish_stop(ctx,0x8c61d122u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61d124; /* mov.w @(0x2,r4),r0 */
L8c61d124: sa_motion_publish_stop(ctx,0x8c61d124u); r[2]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c61d126; /* mov.l @r10,r2 */
L8c61d126: sa_motion_publish_stop(ctx,0x8c61d126u); r[0]=r[0]&65535u;goto L8c61d128; /* extu.w r0,r0 */
L8c61d128: sa_motion_publish_stop(ctx,0x8c61d128u); r[0]<<=2;goto L8c61d12a; /* shll2 r0 */
L8c61d12a: sa_motion_publish_stop(ctx,0x8c61d12au); r[2]+=r[0];goto L8c61d12c; /* add r0,r2 */
L8c61d12c: sa_motion_publish_stop(ctx,0x8c61d12cu); wr(m,bus,&io_status,r[10],r[2],4);if(io_status)return io_status;goto L8c61d12e; /* mov.l r2,@r10 */
L8c61d12e: sa_motion_publish_stop(ctx,0x8c61d130u); r[10]=r[11];goto L8c61d272; /* bra 0x8c61d272 */
L8c61d132: sa_motion_publish_stop(ctx,0x8c61d132u); r[6]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d134; /* mov.w @r12+,r6 */
L8c61d134: sa_motion_publish_stop(ctx,0x8c61d134u); r[0]=16u;goto L8c61d136; /* mov #0x10,r0 */
L8c61d136: sa_motion_publish_stop(ctx,0x8c61d136u); r[3]=rd(m,bus,&io_status,0x8c61d2fcu,4);if(io_status)return io_status;goto L8c61d138; /* mov.l 0x8c61d2fc,r3 */
L8c61d138: sa_motion_publish_stop(ctx,0x8c61d138u); r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d13a; /* mov.w @r12+,r7 */
L8c61d13a: sa_motion_publish_stop(ctx,0x8c61d13au); r[6]=r[6]&65535u;goto L8c61d13c; /* extu.w r6,r6 */
L8c61d13c: sa_motion_publish_stop(ctx,0x8c61d13cu); r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61d13e; /* mov.l @r3,r11 */
L8c61d13e: sa_motion_publish_stop(ctx,0x8c61d13eu); r[6]<<=2;goto L8c61d140; /* shll2 r6 */
L8c61d140: sa_motion_publish_stop(ctx,0x8c61d140u); r[7]=r[7]&65535u;goto L8c61d142; /* extu.w r7,r7 */
L8c61d142: sa_motion_publish_stop(ctx,0x8c61d142u); r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d144; /* mov.w @r12+,r5 */
L8c61d144: sa_motion_publish_stop(ctx,0x8c61d144u); r[7]<<=2;goto L8c61d146; /* shll2 r7 */
L8c61d146: sa_motion_publish_stop(ctx,0x8c61d146u); r[7]<<=2;goto L8c61d148; /* shll2 r7 */
L8c61d148: sa_motion_publish_stop(ctx,0x8c61d148u); r[14]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d14a; /* mov.w @r12+,r14 */
L8c61d14a: sa_motion_publish_stop(ctx,0x8c61d14au); r[6]<<=2;goto L8c61d14c; /* shll2 r6 */
L8c61d14c: sa_motion_publish_stop(ctx,0x8c61d14cu); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61d14e; /* shll r6 */
L8c61d14e: sa_motion_publish_stop(ctx,0x8c61d14eu); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61d150; /* shll r7 */
L8c61d150: sa_motion_publish_stop(ctx,0x8c61d150u); r[7]+=r[11];goto L8c61d152; /* add r11,r7 */
L8c61d152: sa_motion_publish_stop(ctx,0x8c61d152u); r[6]+=r[11];goto L8c61d154; /* add r11,r6 */
L8c61d154: sa_motion_publish_stop(ctx,0x8c61d154u); fload(m,bus,&io_status,f,4u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d156; /* fmov.s @(r0,r7),fr4 */
L8c61d156: sa_motion_publish_stop(ctx,0x8c61d156u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d158; /* fmov.s @(r0,r6),fr3 */
L8c61d158: sa_motion_publish_stop(ctx,0x8c61d158u); r[5]=r[5]&65535u;goto L8c61d15a; /* extu.w r5,r5 */
L8c61d15a: sa_motion_publish_stop(ctx,0x8c61d15au); fcopy(f,2u,4u);goto L8c61d15c; /* fmov fr4,fr2 */
L8c61d15c: sa_motion_publish_stop(ctx,0x8c61d15cu); r[5]<<=2;goto L8c61d15e; /* shll2 r5 */
L8c61d15e: sa_motion_publish_stop(ctx,0x8c61d15eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d160; /* fsub fr3,fr2 */
L8c61d160: sa_motion_publish_stop(ctx,0x8c61d160u); r[14]=r[14]&65535u;goto L8c61d162; /* extu.w r14,r14 */
L8c61d162: sa_motion_publish_stop(ctx,0x8c61d162u); r[5]<<=2;goto L8c61d164; /* shll2 r5 */
L8c61d164: sa_motion_publish_stop(ctx,0x8c61d164u); r[14]<<=2;goto L8c61d166; /* shll2 r14 */
L8c61d166: sa_motion_publish_stop(ctx,0x8c61d166u); r[0]=20u;goto L8c61d168; /* mov #0x14,r0 */
L8c61d168: sa_motion_publish_stop(ctx,0x8c61d168u); fload(m,bus,&io_status,f,5u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d16a; /* fmov.s @(r0,r7),fr5 */
L8c61d16a: sa_motion_publish_stop(ctx,0x8c61d16au); r[14]<<=2;goto L8c61d16c; /* shll2 r14 */
L8c61d16c: sa_motion_publish_stop(ctx,0x8c61d16cu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d16e; /* shll r5 */
L8c61d16e: sa_motion_publish_stop(ctx,0x8c61d16eu); r[5]+=r[11];goto L8c61d170; /* add r11,r5 */
L8c61d170: sa_motion_publish_stop(ctx,0x8c61d170u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61d172; /* shll r14 */
L8c61d172: sa_motion_publish_stop(ctx,0x8c61d172u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d174; /* fmov.s @(r0,r5),fr3 */
L8c61d174: sa_motion_publish_stop(ctx,0x8c61d174u); fcopy(f,1u,5u);goto L8c61d176; /* fmov fr5,fr1 */
L8c61d176: sa_motion_publish_stop(ctx,0x8c61d176u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d178; /* fsub fr5,fr3 */
L8c61d178: sa_motion_publish_stop(ctx,0x8c61d178u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d17a; /* fmul fr3,fr2 */
L8c61d17a: sa_motion_publish_stop(ctx,0x8c61d17au); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d17c; /* fmov.s @(r0,r6),fr3 */
L8c61d17c: sa_motion_publish_stop(ctx,0x8c61d17cu); r[0]=16u;goto L8c61d17e; /* mov #0x10,r0 */
L8c61d17e: sa_motion_publish_stop(ctx,0x8c61d17eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d180; /* fsub fr3,fr1 */
L8c61d180: sa_motion_publish_stop(ctx,0x8c61d180u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d182; /* fmov.s @(r0,r5),fr3 */
L8c61d182: sa_motion_publish_stop(ctx,0x8c61d182u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d184; /* fsub fr4,fr3 */
L8c61d184: sa_motion_publish_stop(ctx,0x8c61d184u); fcopy(f,4u,2u);goto L8c61d186; /* fmov fr2,fr4 */
L8c61d186: sa_motion_publish_stop(ctx,0x8c61d186u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d188; /* fmul fr3,fr1 */
L8c61d188: sa_motion_publish_stop(ctx,0x8c61d188u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d18a; /* fldi0 fr3 */
L8c61d18a: sa_motion_publish_stop(ctx,0x8c61d18au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d18c; /* fsub fr1,fr4 */
L8c61d18c: sa_motion_publish_stop(ctx,0x8c61d18cu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d18e; /* fcmp/gt fr3,fr4 */
L8c61d18e: sa_motion_publish_stop(ctx,0x8c61d190u); take=!(*sr&1u);r[14]+=r[11];if(take)goto L8c61d196;goto L8c61d192; /* bf/s 0x8c61d196 */
L8c61d192: sa_motion_publish_stop(ctx,0x8c61d194u); r[13]+=16u;goto L8c61d270; /* bra 0x8c61d270 */
L8c61d196: sa_motion_publish_stop(ctx,0x8c61d196u); r[8]+=28u;goto L8c61d198; /* add #0x1c,r8 */
L8c61d198: sa_motion_publish_stop(ctx,0x8c61d198u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d19a; /* fmov.s fr7,@-r8 */
L8c61d19a: sa_motion_publish_stop(ctx,0x8c61d19au); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d19c; /* mov.w @(0x2,r13),r0 */
L8c61d19c: sa_motion_publish_stop(ctx,0x8c61d19cu); r[3]=r[0];goto L8c61d19e; /* mov r0,r3 */
L8c61d19e: sa_motion_publish_stop(ctx,0x8c61d19eu); *fpul=r[3];goto L8c61d1a0; /* lds r3,FPUL */
L8c61d1a0: sa_motion_publish_stop(ctx,0x8c61d1a0u); r[0]=24u;goto L8c61d1a2; /* mov #0x18,r0 */
L8c61d1a2: sa_motion_publish_stop(ctx,0x8c61d1a2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d1a4; /* float FPUL,fr3 */
L8c61d1a4: sa_motion_publish_stop(ctx,0x8c61d1a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d1a6; /* fmul fr6,fr3 */
L8c61d1a6: sa_motion_publish_stop(ctx,0x8c61d1a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1a8; /* fmov.s fr3,@-r8 */
L8c61d1a8: sa_motion_publish_stop(ctx,0x8c61d1a8u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d1aa; /* mov.w @r13,r3 */
L8c61d1aa: sa_motion_publish_stop(ctx,0x8c61d1aau); r[13]+=4u;goto L8c61d1ac; /* add #0x4,r13 */
L8c61d1ac: sa_motion_publish_stop(ctx,0x8c61d1acu); *fpul=r[3];goto L8c61d1ae; /* lds r3,FPUL */
L8c61d1ae: sa_motion_publish_stop(ctx,0x8c61d1aeu); r[3]=rd(m,bus,&io_status,0x8c61d2f0u,4);if(io_status)return io_status;goto L8c61d1b0; /* mov.l 0x8c61d2f0,r3 */
L8c61d1b0: sa_motion_publish_stop(ctx,0x8c61d1b0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d1b2; /* float FPUL,fr3 */
L8c61d1b2: sa_motion_publish_stop(ctx,0x8c61d1b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d1b4; /* fmul fr6,fr3 */
L8c61d1b4: sa_motion_publish_stop(ctx,0x8c61d1b4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1b6; /* fmov.s fr3,@-r8 */
L8c61d1b6: sa_motion_publish_stop(ctx,0x8c61d1b6u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d1b8; /* fmov.s @(r0,r6),fr3 */
L8c61d1b8: sa_motion_publish_stop(ctx,0x8c61d1b8u); r[0]=20u;goto L8c61d1ba; /* mov #0x14,r0 */
L8c61d1ba: sa_motion_publish_stop(ctx,0x8c61d1bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1bc; /* fmov.s fr3,@-r8 */
L8c61d1bc: sa_motion_publish_stop(ctx,0x8c61d1bcu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d1be; /* fmov.s @(r0,r6),fr3 */
L8c61d1be: sa_motion_publish_stop(ctx,0x8c61d1beu); r[0]=16u;goto L8c61d1c0; /* mov #0x10,r0 */
L8c61d1c0: sa_motion_publish_stop(ctx,0x8c61d1c0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1c2; /* fmov.s fr3,@-r8 */
L8c61d1c2: sa_motion_publish_stop(ctx,0x8c61d1c2u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d1c4; /* fmov.s @(r0,r6),fr3 */
L8c61d1c4: sa_motion_publish_stop(ctx,0x8c61d1c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1c6; /* fmov.s fr3,@-r8 */
L8c61d1c6: sa_motion_publish_stop(ctx,0x8c61d1c6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d1c8; /* mov.l r3,@-r8 */
L8c61d1c8: sa_motion_publish_stop(ctx,0x8c61d1c8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d1ca; /* pref @r8 */
L8c61d1ca: sa_motion_publish_stop(ctx,0x8c61d1cau); r[8]+=60u;goto L8c61d1cc; /* add #0x3c,r8 */
L8c61d1cc: sa_motion_publish_stop(ctx,0x8c61d1ccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d1ce; /* fmov.s fr7,@-r8 */
L8c61d1ce: sa_motion_publish_stop(ctx,0x8c61d1ceu); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d1d0; /* mov.w @(0x2,r13),r0 */
L8c61d1d0: sa_motion_publish_stop(ctx,0x8c61d1d0u); r[3]=r[0];goto L8c61d1d2; /* mov r0,r3 */
L8c61d1d2: sa_motion_publish_stop(ctx,0x8c61d1d2u); *fpul=r[3];goto L8c61d1d4; /* lds r3,FPUL */
L8c61d1d4: sa_motion_publish_stop(ctx,0x8c61d1d4u); r[0]=24u;goto L8c61d1d6; /* mov #0x18,r0 */
L8c61d1d6: sa_motion_publish_stop(ctx,0x8c61d1d6u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d1d8; /* float FPUL,fr3 */
L8c61d1d8: sa_motion_publish_stop(ctx,0x8c61d1d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d1da; /* fmul fr6,fr3 */
L8c61d1da: sa_motion_publish_stop(ctx,0x8c61d1dau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1dc; /* fmov.s fr3,@-r8 */
L8c61d1dc: sa_motion_publish_stop(ctx,0x8c61d1dcu); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d1de; /* mov.w @r13,r3 */
L8c61d1de: sa_motion_publish_stop(ctx,0x8c61d1deu); r[13]+=4u;goto L8c61d1e0; /* add #0x4,r13 */
L8c61d1e0: sa_motion_publish_stop(ctx,0x8c61d1e0u); *fpul=r[3];goto L8c61d1e2; /* lds r3,FPUL */
L8c61d1e2: sa_motion_publish_stop(ctx,0x8c61d1e2u); r[3]=rd(m,bus,&io_status,0x8c61d2f0u,4);if(io_status)return io_status;goto L8c61d1e4; /* mov.l 0x8c61d2f0,r3 */
L8c61d1e4: sa_motion_publish_stop(ctx,0x8c61d1e4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d1e6; /* float FPUL,fr3 */
L8c61d1e6: sa_motion_publish_stop(ctx,0x8c61d1e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d1e8; /* fmul fr6,fr3 */
L8c61d1e8: sa_motion_publish_stop(ctx,0x8c61d1e8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1ea; /* fmov.s fr3,@-r8 */
L8c61d1ea: sa_motion_publish_stop(ctx,0x8c61d1eau); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d1ec; /* fmov.s @(r0,r7),fr3 */
L8c61d1ec: sa_motion_publish_stop(ctx,0x8c61d1ecu); r[0]=20u;goto L8c61d1ee; /* mov #0x14,r0 */
L8c61d1ee: sa_motion_publish_stop(ctx,0x8c61d1eeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1f0; /* fmov.s fr3,@-r8 */
L8c61d1f0: sa_motion_publish_stop(ctx,0x8c61d1f0u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d1f2; /* fmov.s @(r0,r7),fr3 */
L8c61d1f2: sa_motion_publish_stop(ctx,0x8c61d1f2u); r[0]=16u;goto L8c61d1f4; /* mov #0x10,r0 */
L8c61d1f4: sa_motion_publish_stop(ctx,0x8c61d1f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1f6; /* fmov.s fr3,@-r8 */
L8c61d1f6: sa_motion_publish_stop(ctx,0x8c61d1f6u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d1f8; /* fmov.s @(r0,r7),fr3 */
L8c61d1f8: sa_motion_publish_stop(ctx,0x8c61d1f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1fa; /* fmov.s fr3,@-r8 */
L8c61d1fa: sa_motion_publish_stop(ctx,0x8c61d1fau); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d1fc; /* mov.l r3,@-r8 */
L8c61d1fc: sa_motion_publish_stop(ctx,0x8c61d1fcu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d1fe; /* pref @r8 */
L8c61d1fe: sa_motion_publish_stop(ctx,0x8c61d1feu); r[8]+=60u;goto L8c61d200; /* add #0x3c,r8 */
L8c61d200: sa_motion_publish_stop(ctx,0x8c61d200u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d202; /* fmov.s fr7,@-r8 */
L8c61d202: sa_motion_publish_stop(ctx,0x8c61d202u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d204; /* mov.w @(0x2,r13),r0 */
L8c61d204: sa_motion_publish_stop(ctx,0x8c61d204u); r[3]=r[0];goto L8c61d206; /* mov r0,r3 */
L8c61d206: sa_motion_publish_stop(ctx,0x8c61d206u); *fpul=r[3];goto L8c61d208; /* lds r3,FPUL */
L8c61d208: sa_motion_publish_stop(ctx,0x8c61d208u); r[0]=24u;goto L8c61d20a; /* mov #0x18,r0 */
L8c61d20a: sa_motion_publish_stop(ctx,0x8c61d20au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d20c; /* float FPUL,fr3 */
L8c61d20c: sa_motion_publish_stop(ctx,0x8c61d20cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d20e; /* fmul fr6,fr3 */
L8c61d20e: sa_motion_publish_stop(ctx,0x8c61d20eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d210; /* fmov.s fr3,@-r8 */
L8c61d210: sa_motion_publish_stop(ctx,0x8c61d210u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d212; /* mov.w @r13,r3 */
L8c61d212: sa_motion_publish_stop(ctx,0x8c61d212u); r[13]+=4u;goto L8c61d214; /* add #0x4,r13 */
L8c61d214: sa_motion_publish_stop(ctx,0x8c61d214u); *fpul=r[3];goto L8c61d216; /* lds r3,FPUL */
L8c61d216: sa_motion_publish_stop(ctx,0x8c61d216u); r[3]=rd(m,bus,&io_status,0x8c61d2f0u,4);if(io_status)return io_status;goto L8c61d218; /* mov.l 0x8c61d2f0,r3 */
L8c61d218: sa_motion_publish_stop(ctx,0x8c61d218u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d21a; /* float FPUL,fr3 */
L8c61d21a: sa_motion_publish_stop(ctx,0x8c61d21au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d21c; /* fmul fr6,fr3 */
L8c61d21c: sa_motion_publish_stop(ctx,0x8c61d21cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d21e; /* fmov.s fr3,@-r8 */
L8c61d21e: sa_motion_publish_stop(ctx,0x8c61d21eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d220; /* fmov.s @(r0,r5),fr3 */
L8c61d220: sa_motion_publish_stop(ctx,0x8c61d220u); r[0]=20u;goto L8c61d222; /* mov #0x14,r0 */
L8c61d222: sa_motion_publish_stop(ctx,0x8c61d222u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d224; /* fmov.s fr3,@-r8 */
L8c61d224: sa_motion_publish_stop(ctx,0x8c61d224u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d226; /* fmov.s @(r0,r5),fr3 */
L8c61d226: sa_motion_publish_stop(ctx,0x8c61d226u); r[0]=16u;goto L8c61d228; /* mov #0x10,r0 */
L8c61d228: sa_motion_publish_stop(ctx,0x8c61d228u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d22a; /* fmov.s fr3,@-r8 */
L8c61d22a: sa_motion_publish_stop(ctx,0x8c61d22au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d22c; /* fmov.s @(r0,r5),fr3 */
L8c61d22c: sa_motion_publish_stop(ctx,0x8c61d22cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d22e; /* fmov.s fr3,@-r8 */
L8c61d22e: sa_motion_publish_stop(ctx,0x8c61d22eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d230; /* mov.l r3,@-r8 */
L8c61d230: sa_motion_publish_stop(ctx,0x8c61d230u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d232; /* pref @r8 */
L8c61d232: sa_motion_publish_stop(ctx,0x8c61d232u); r[8]+=60u;goto L8c61d234; /* add #0x3c,r8 */
L8c61d234: sa_motion_publish_stop(ctx,0x8c61d234u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d236; /* fmov.s fr7,@-r8 */
L8c61d236: sa_motion_publish_stop(ctx,0x8c61d236u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d238; /* mov.w @(0x2,r13),r0 */
L8c61d238: sa_motion_publish_stop(ctx,0x8c61d238u); r[3]=r[0];goto L8c61d23a; /* mov r0,r3 */
L8c61d23a: sa_motion_publish_stop(ctx,0x8c61d23au); *fpul=r[3];goto L8c61d23c; /* lds r3,FPUL */
L8c61d23c: sa_motion_publish_stop(ctx,0x8c61d23cu); r[0]=24u;goto L8c61d23e; /* mov #0x18,r0 */
L8c61d23e: sa_motion_publish_stop(ctx,0x8c61d23eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d240; /* float FPUL,fr3 */
L8c61d240: sa_motion_publish_stop(ctx,0x8c61d240u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d242; /* fmul fr6,fr3 */
L8c61d242: sa_motion_publish_stop(ctx,0x8c61d242u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d244; /* fmov.s fr3,@-r8 */
L8c61d244: sa_motion_publish_stop(ctx,0x8c61d244u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d246; /* mov.w @r13,r3 */
L8c61d246: sa_motion_publish_stop(ctx,0x8c61d246u); r[13]+=4u;goto L8c61d248; /* add #0x4,r13 */
L8c61d248: sa_motion_publish_stop(ctx,0x8c61d248u); *fpul=r[3];goto L8c61d24a; /* lds r3,FPUL */
L8c61d24a: sa_motion_publish_stop(ctx,0x8c61d24au); r[3]=rd(m,bus,&io_status,0x8c61d2f4u,4);if(io_status)return io_status;goto L8c61d24c; /* mov.l 0x8c61d2f4,r3 */
L8c61d24c: sa_motion_publish_stop(ctx,0x8c61d24cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d24e; /* float FPUL,fr3 */
L8c61d24e: sa_motion_publish_stop(ctx,0x8c61d24eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d250; /* fmul fr6,fr3 */
L8c61d250: sa_motion_publish_stop(ctx,0x8c61d250u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d252; /* fmov.s fr3,@-r8 */
L8c61d252: sa_motion_publish_stop(ctx,0x8c61d252u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d254; /* fmov.s @(r0,r14),fr3 */
L8c61d254: sa_motion_publish_stop(ctx,0x8c61d254u); r[0]=20u;goto L8c61d256; /* mov #0x14,r0 */
L8c61d256: sa_motion_publish_stop(ctx,0x8c61d256u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d258; /* fmov.s fr3,@-r8 */
L8c61d258: sa_motion_publish_stop(ctx,0x8c61d258u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d25a; /* fmov.s @(r0,r14),fr3 */
L8c61d25a: sa_motion_publish_stop(ctx,0x8c61d25au); r[0]=16u;goto L8c61d25c; /* mov #0x10,r0 */
L8c61d25c: sa_motion_publish_stop(ctx,0x8c61d25cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d25e; /* fmov.s fr3,@-r8 */
L8c61d25e: sa_motion_publish_stop(ctx,0x8c61d25eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d260; /* fmov.s @(r0,r14),fr3 */
L8c61d260: sa_motion_publish_stop(ctx,0x8c61d260u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d262; /* fmov.s fr3,@-r8 */
L8c61d262: sa_motion_publish_stop(ctx,0x8c61d262u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d264; /* mov.l r3,@-r8 */
L8c61d264: sa_motion_publish_stop(ctx,0x8c61d264u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d266; /* pref @r8 */
L8c61d266: sa_motion_publish_stop(ctx,0x8c61d266u); r[2]=rd(m,bus,&io_status,0x8c61d2f8u,4);if(io_status)return io_status;goto L8c61d268; /* mov.l 0x8c61d2f8,r2 */
L8c61d268: sa_motion_publish_stop(ctx,0x8c61d268u); r[8]+=32u;goto L8c61d26a; /* add #0x20,r8 */
L8c61d26a: sa_motion_publish_stop(ctx,0x8c61d26au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d26c; /* mov.l @r2,r3 */
L8c61d26c: sa_motion_publish_stop(ctx,0x8c61d26cu); r[3]+=4u;goto L8c61d26e; /* add #0x4,r3 */
L8c61d26e: sa_motion_publish_stop(ctx,0x8c61d26eu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61d270; /* mov.l r3,@r2 */
L8c61d270: sa_motion_publish_stop(ctx,0x8c61d270u); r[10]+=1u;goto L8c61d272; /* add #0x1,r10 */
L8c61d272: sa_motion_publish_stop(ctx,0x8c61d272u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61d274; /* mov.w @(0x2,r4),r0 */
L8c61d274: sa_motion_publish_stop(ctx,0x8c61d274u); r[0]=r[0]&65535u;goto L8c61d276; /* extu.w r0,r0 */
L8c61d276: sa_motion_publish_stop(ctx,0x8c61d276u); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61d278; /* cmp/ge r0,r10 */
L8c61d278: sa_motion_publish_stop(ctx,0x8c61d278u); if((*sr&1u))goto L8c61d27e;goto L8c61d27a; /* bt 0x8c61d27e */
L8c61d27a: sa_motion_publish_stop(ctx,0x8c61d27cu); goto L8c61d132; /* bra 0x8c61d132 */
L8c61d27e: sa_motion_publish_stop(ctx,0x8c61d280u); goto L8c61d3f4; /* bra 0x8c61d3f4 */
L8c61d282: sa_motion_publish_stop(ctx,0x8c61d284u); r[9]=r[11];goto L8c61d3a6; /* bra 0x8c61d3a6 */
L8c61d286: sa_motion_publish_stop(ctx,0x8c61d286u); r[14]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d288; /* mov.w @r12+,r14 */
L8c61d288: sa_motion_publish_stop(ctx,0x8c61d288u); r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c61d28a; /* mov.l @r10,r3 */
L8c61d28a: sa_motion_publish_stop(ctx,0x8c61d28au); r[14]=r[14]&65535u;goto L8c61d28c; /* extu.w r14,r14 */
L8c61d28c: sa_motion_publish_stop(ctx,0x8c61d28cu); r[2]=rd(m,bus,&io_status,0x8c61d2fcu,4);if(io_status)return io_status;goto L8c61d28e; /* mov.l 0x8c61d2fc,r2 */
L8c61d28e: sa_motion_publish_stop(ctx,0x8c61d28eu); r[3]+=r[14];goto L8c61d290; /* add r14,r3 */
L8c61d290: sa_motion_publish_stop(ctx,0x8c61d290u); wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c61d292; /* mov.l r3,@r10 */
L8c61d292: sa_motion_publish_stop(ctx,0x8c61d292u); r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c61d294; /* mov.w @(0x2,r12),r0 */
L8c61d294: sa_motion_publish_stop(ctx,0x8c61d294u); r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;goto L8c61d296; /* mov.w @r12,r7 */
L8c61d296: sa_motion_publish_stop(ctx,0x8c61d296u); r[5]=r[0]&65535u;goto L8c61d298; /* extu.w r0,r5 */
L8c61d298: sa_motion_publish_stop(ctx,0x8c61d298u); r[0]=sx(m,bus,&io_status,r[12]+0x4u,2);if(io_status)return io_status;goto L8c61d29a; /* mov.w @(0x4,r12),r0 */
L8c61d29a: sa_motion_publish_stop(ctx,0x8c61d29au); r[5]<<=2;goto L8c61d29c; /* shll2 r5 */
L8c61d29c: sa_motion_publish_stop(ctx,0x8c61d29cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d29e; /* mov.l @r2,r3 */
L8c61d29e: sa_motion_publish_stop(ctx,0x8c61d29eu); r[7]=r[7]&65535u;goto L8c61d2a0; /* extu.w r7,r7 */
L8c61d2a0: sa_motion_publish_stop(ctx,0x8c61d2a0u); r[7]<<=2;goto L8c61d2a2; /* shll2 r7 */
L8c61d2a2: sa_motion_publish_stop(ctx,0x8c61d2a2u); r[5]<<=2;goto L8c61d2a4; /* shll2 r5 */
L8c61d2a4: sa_motion_publish_stop(ctx,0x8c61d2a4u); r[6]=r[0]&65535u;goto L8c61d2a6; /* extu.w r0,r6 */
L8c61d2a6: sa_motion_publish_stop(ctx,0x8c61d2a6u); r[7]<<=2;goto L8c61d2a8; /* shll2 r7 */
L8c61d2a8: sa_motion_publish_stop(ctx,0x8c61d2a8u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61d2aa; /* shll r7 */
L8c61d2aa: sa_motion_publish_stop(ctx,0x8c61d2aau); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d2ac; /* shll r5 */
L8c61d2ac: sa_motion_publish_stop(ctx,0x8c61d2acu); r[0]=16u;goto L8c61d2ae; /* mov #0x10,r0 */
L8c61d2ae: sa_motion_publish_stop(ctx,0x8c61d2aeu); r[5]+=r[3];goto L8c61d2b0; /* add r3,r5 */
L8c61d2b0: sa_motion_publish_stop(ctx,0x8c61d2b0u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d2b2; /* fmov.s @(r0,r5),fr5 */
L8c61d2b2: sa_motion_publish_stop(ctx,0x8c61d2b2u); r[7]+=r[3];goto L8c61d2b4; /* add r3,r7 */
L8c61d2b4: sa_motion_publish_stop(ctx,0x8c61d2b4u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d2b6; /* fmov.s @(r0,r7),fr3 */
L8c61d2b6: sa_motion_publish_stop(ctx,0x8c61d2b6u); r[6]<<=2;goto L8c61d2b8; /* shll2 r6 */
L8c61d2b8: sa_motion_publish_stop(ctx,0x8c61d2b8u); fcopy(f,2u,5u);goto L8c61d2ba; /* fmov fr5,fr2 */
L8c61d2ba: sa_motion_publish_stop(ctx,0x8c61d2bau); r[6]<<=2;goto L8c61d2bc; /* shll2 r6 */
L8c61d2bc: sa_motion_publish_stop(ctx,0x8c61d2bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d2be; /* fsub fr3,fr2 */
L8c61d2be: sa_motion_publish_stop(ctx,0x8c61d2beu); r[0]=20u;goto L8c61d2c0; /* mov #0x14,r0 */
L8c61d2c0: sa_motion_publish_stop(ctx,0x8c61d2c0u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61d2c2; /* shll r6 */
L8c61d2c2: sa_motion_publish_stop(ctx,0x8c61d2c2u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d2c4; /* fmov.s @(r0,r5),fr4 */
L8c61d2c4: sa_motion_publish_stop(ctx,0x8c61d2c4u); r[6]+=r[3];goto L8c61d2c6; /* add r3,r6 */
L8c61d2c6: sa_motion_publish_stop(ctx,0x8c61d2c6u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d2c8; /* fmov.s @(r0,r6),fr3 */
L8c61d2c8: sa_motion_publish_stop(ctx,0x8c61d2c8u); fcopy(f,1u,4u);goto L8c61d2ca; /* fmov fr4,fr1 */
L8c61d2ca: sa_motion_publish_stop(ctx,0x8c61d2cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d2cc; /* fsub fr4,fr3 */
L8c61d2cc: sa_motion_publish_stop(ctx,0x8c61d2ccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d2ce; /* fmul fr3,fr2 */
L8c61d2ce: sa_motion_publish_stop(ctx,0x8c61d2ceu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d2d0; /* fmov.s @(r0,r7),fr3 */
L8c61d2d0: sa_motion_publish_stop(ctx,0x8c61d2d0u); r[0]=16u;goto L8c61d2d2; /* mov #0x10,r0 */
L8c61d2d2: sa_motion_publish_stop(ctx,0x8c61d2d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d2d4; /* fsub fr3,fr1 */
L8c61d2d4: sa_motion_publish_stop(ctx,0x8c61d2d4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d2d6; /* fmov.s @(r0,r6),fr3 */
L8c61d2d6: sa_motion_publish_stop(ctx,0x8c61d2d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d2d8; /* fsub fr5,fr3 */
L8c61d2d8: sa_motion_publish_stop(ctx,0x8c61d2d8u); fcopy(f,4u,2u);goto L8c61d2da; /* fmov fr2,fr4 */
L8c61d2da: sa_motion_publish_stop(ctx,0x8c61d2dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d2dc; /* fmul fr3,fr1 */
L8c61d2dc: sa_motion_publish_stop(ctx,0x8c61d2dcu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d2de; /* fldi0 fr3 */
L8c61d2de: sa_motion_publish_stop(ctx,0x8c61d2deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d2e0; /* fsub fr1,fr4 */
L8c61d2e0: sa_motion_publish_stop(ctx,0x8c61d2e0u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d2e2; /* fcmp/gt fr3,fr4 */
L8c61d2e2: sa_motion_publish_stop(ctx,0x8c61d2e2u); if(!(*sr&1u))goto L8c61d300;goto L8c61d2e4; /* bf 0x8c61d300 */
L8c61d2e4: sa_motion_publish_stop(ctx,0x8c61d2e4u); r[1]=r[14];goto L8c61d2e6; /* mov r14,r1 */
L8c61d2e6: sa_motion_publish_stop(ctx,0x8c61d2e6u); *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c61d2e8; /* shll r1 */
L8c61d2e8: sa_motion_publish_stop(ctx,0x8c61d2e8u); r[14]<<=2;goto L8c61d2ea; /* shll2 r14 */
L8c61d2ea: sa_motion_publish_stop(ctx,0x8c61d2eau); r[12]+=r[1];goto L8c61d2ec; /* add r1,r12 */
L8c61d2ec: sa_motion_publish_stop(ctx,0x8c61d2eeu); r[13]+=r[14];goto L8c61d3a4; /* bra 0x8c61d3a4 */
L8c61d300: sa_motion_publish_stop(ctx,0x8c61d300u); r[2]=rd(m,bus,&io_status,0x8c61d3bcu,4);if(io_status)return io_status;goto L8c61d302; /* mov.l 0x8c61d3bc,r2 */
L8c61d302: sa_motion_publish_stop(ctx,0x8c61d302u); r[7]=r[14];goto L8c61d304; /* mov r14,r7 */
L8c61d304: sa_motion_publish_stop(ctx,0x8c61d304u); r[7]+=4294967295u;goto L8c61d306; /* add #-0x1,r7 */
L8c61d306: sa_motion_publish_stop(ctx,0x8c61d306u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d308; /* mov.l @r2,r3 */
L8c61d308: sa_motion_publish_stop(ctx,0x8c61d308u); *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c61d30a; /* cmp/pl r7 */
L8c61d30a: sa_motion_publish_stop(ctx,0x8c61d30au); r[3]+=r[14];goto L8c61d30c; /* add r14,r3 */
L8c61d30c: sa_motion_publish_stop(ctx,0x8c61d30cu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61d30e; /* mov.l r3,@r2 */
L8c61d30e: sa_motion_publish_stop(ctx,0x8c61d310u); take=!(*sr&1u);r[6]=r[11];if(take)goto L8c61d35e;goto L8c61d312; /* bf/s 0x8c61d35e */
L8c61d312: sa_motion_publish_stop(ctx,0x8c61d312u); r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d314; /* mov.w @r12+,r5 */
L8c61d314: sa_motion_publish_stop(ctx,0x8c61d314u); r[8]+=28u;goto L8c61d316; /* add #0x1c,r8 */
L8c61d316: sa_motion_publish_stop(ctx,0x8c61d316u); r[2]=rd(m,bus,&io_status,0x8c61d3c0u,4);if(io_status)return io_status;goto L8c61d318; /* mov.l 0x8c61d3c0,r2 */
L8c61d318: sa_motion_publish_stop(ctx,0x8c61d318u); r[5]=r[5]&65535u;goto L8c61d31a; /* extu.w r5,r5 */
L8c61d31a: sa_motion_publish_stop(ctx,0x8c61d31au); r[5]<<=2;goto L8c61d31c; /* shll2 r5 */
L8c61d31c: sa_motion_publish_stop(ctx,0x8c61d31cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d31e; /* mov.l @r2,r3 */
L8c61d31e: sa_motion_publish_stop(ctx,0x8c61d31eu); r[5]<<=2;goto L8c61d320; /* shll2 r5 */
L8c61d320: sa_motion_publish_stop(ctx,0x8c61d320u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d322; /* fmov.s fr7,@-r8 */
L8c61d322: sa_motion_publish_stop(ctx,0x8c61d322u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d324; /* mov.w @(0x2,r13),r0 */
L8c61d324: sa_motion_publish_stop(ctx,0x8c61d324u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d326; /* shll r5 */
L8c61d326: sa_motion_publish_stop(ctx,0x8c61d326u); r[5]+=r[3];goto L8c61d328; /* add r3,r5 */
L8c61d328: sa_motion_publish_stop(ctx,0x8c61d328u); r[3]=r[0];goto L8c61d32a; /* mov r0,r3 */
L8c61d32a: sa_motion_publish_stop(ctx,0x8c61d32au); *fpul=r[3];goto L8c61d32c; /* lds r3,FPUL */
L8c61d32c: sa_motion_publish_stop(ctx,0x8c61d32cu); r[0]=24u;goto L8c61d32e; /* mov #0x18,r0 */
L8c61d32e: sa_motion_publish_stop(ctx,0x8c61d32eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d330; /* float FPUL,fr3 */
L8c61d330: sa_motion_publish_stop(ctx,0x8c61d330u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d332; /* fmul fr6,fr3 */
L8c61d332: sa_motion_publish_stop(ctx,0x8c61d332u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d334; /* fmov.s fr3,@-r8 */
L8c61d334: sa_motion_publish_stop(ctx,0x8c61d334u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d336; /* mov.w @r13,r3 */
L8c61d336: sa_motion_publish_stop(ctx,0x8c61d336u); r[13]+=4u;goto L8c61d338; /* add #0x4,r13 */
L8c61d338: sa_motion_publish_stop(ctx,0x8c61d338u); *fpul=r[3];goto L8c61d33a; /* lds r3,FPUL */
L8c61d33a: sa_motion_publish_stop(ctx,0x8c61d33au); r[3]=rd(m,bus,&io_status,0x8c61d3c4u,4);if(io_status)return io_status;goto L8c61d33c; /* mov.l 0x8c61d3c4,r3 */
L8c61d33c: sa_motion_publish_stop(ctx,0x8c61d33cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d33e; /* float FPUL,fr3 */
L8c61d33e: sa_motion_publish_stop(ctx,0x8c61d33eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d340; /* fmul fr6,fr3 */
L8c61d340: sa_motion_publish_stop(ctx,0x8c61d340u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d342; /* fmov.s fr3,@-r8 */
L8c61d342: sa_motion_publish_stop(ctx,0x8c61d342u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d344; /* fmov.s @(r0,r5),fr3 */
L8c61d344: sa_motion_publish_stop(ctx,0x8c61d344u); r[0]=20u;goto L8c61d346; /* mov #0x14,r0 */
L8c61d346: sa_motion_publish_stop(ctx,0x8c61d346u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d348; /* fmov.s fr3,@-r8 */
L8c61d348: sa_motion_publish_stop(ctx,0x8c61d348u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d34a; /* fmov.s @(r0,r5),fr3 */
L8c61d34a: sa_motion_publish_stop(ctx,0x8c61d34au); r[0]=16u;goto L8c61d34c; /* mov #0x10,r0 */
L8c61d34c: sa_motion_publish_stop(ctx,0x8c61d34cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d34e; /* fmov.s fr3,@-r8 */
L8c61d34e: sa_motion_publish_stop(ctx,0x8c61d34eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d350; /* fmov.s @(r0,r5),fr3 */
L8c61d350: sa_motion_publish_stop(ctx,0x8c61d350u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d352; /* fmov.s fr3,@-r8 */
L8c61d352: sa_motion_publish_stop(ctx,0x8c61d352u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d354; /* mov.l r3,@-r8 */
L8c61d354: sa_motion_publish_stop(ctx,0x8c61d354u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d356; /* pref @r8 */
L8c61d356: sa_motion_publish_stop(ctx,0x8c61d356u); r[6]+=1u;goto L8c61d358; /* add #0x1,r6 */
L8c61d358: sa_motion_publish_stop(ctx,0x8c61d358u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c61d35a; /* cmp/ge r7,r6 */
L8c61d35a: sa_motion_publish_stop(ctx,0x8c61d35cu); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61d312;goto L8c61d35e; /* bf/s 0x8c61d312 */
L8c61d35e: sa_motion_publish_stop(ctx,0x8c61d35eu); r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d360; /* mov.w @r12+,r5 */
L8c61d360: sa_motion_publish_stop(ctx,0x8c61d360u); r[8]+=28u;goto L8c61d362; /* add #0x1c,r8 */
L8c61d362: sa_motion_publish_stop(ctx,0x8c61d362u); r[2]=rd(m,bus,&io_status,0x8c61d3c0u,4);if(io_status)return io_status;goto L8c61d364; /* mov.l 0x8c61d3c0,r2 */
L8c61d364: sa_motion_publish_stop(ctx,0x8c61d364u); r[5]=r[5]&65535u;goto L8c61d366; /* extu.w r5,r5 */
L8c61d366: sa_motion_publish_stop(ctx,0x8c61d366u); r[5]<<=2;goto L8c61d368; /* shll2 r5 */
L8c61d368: sa_motion_publish_stop(ctx,0x8c61d368u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d36a; /* mov.l @r2,r3 */
L8c61d36a: sa_motion_publish_stop(ctx,0x8c61d36au); r[5]<<=2;goto L8c61d36c; /* shll2 r5 */
L8c61d36c: sa_motion_publish_stop(ctx,0x8c61d36cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d36e; /* fmov.s fr7,@-r8 */
L8c61d36e: sa_motion_publish_stop(ctx,0x8c61d36eu); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d370; /* mov.w @(0x2,r13),r0 */
L8c61d370: sa_motion_publish_stop(ctx,0x8c61d370u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d372; /* shll r5 */
L8c61d372: sa_motion_publish_stop(ctx,0x8c61d372u); r[5]+=r[3];goto L8c61d374; /* add r3,r5 */
L8c61d374: sa_motion_publish_stop(ctx,0x8c61d374u); r[3]=r[0];goto L8c61d376; /* mov r0,r3 */
L8c61d376: sa_motion_publish_stop(ctx,0x8c61d376u); *fpul=r[3];goto L8c61d378; /* lds r3,FPUL */
L8c61d378: sa_motion_publish_stop(ctx,0x8c61d378u); r[0]=24u;goto L8c61d37a; /* mov #0x18,r0 */
L8c61d37a: sa_motion_publish_stop(ctx,0x8c61d37au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d37c; /* float FPUL,fr3 */
L8c61d37c: sa_motion_publish_stop(ctx,0x8c61d37cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d37e; /* fmul fr6,fr3 */
L8c61d37e: sa_motion_publish_stop(ctx,0x8c61d37eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d380; /* fmov.s fr3,@-r8 */
L8c61d380: sa_motion_publish_stop(ctx,0x8c61d380u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d382; /* mov.w @r13,r3 */
L8c61d382: sa_motion_publish_stop(ctx,0x8c61d382u); r[13]+=4u;goto L8c61d384; /* add #0x4,r13 */
L8c61d384: sa_motion_publish_stop(ctx,0x8c61d384u); *fpul=r[3];goto L8c61d386; /* lds r3,FPUL */
L8c61d386: sa_motion_publish_stop(ctx,0x8c61d386u); r[3]=rd(m,bus,&io_status,0x8c61d3c8u,4);if(io_status)return io_status;goto L8c61d388; /* mov.l 0x8c61d3c8,r3 */
L8c61d388: sa_motion_publish_stop(ctx,0x8c61d388u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d38a; /* float FPUL,fr3 */
L8c61d38a: sa_motion_publish_stop(ctx,0x8c61d38au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d38c; /* fmul fr6,fr3 */
L8c61d38c: sa_motion_publish_stop(ctx,0x8c61d38cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d38e; /* fmov.s fr3,@-r8 */
L8c61d38e: sa_motion_publish_stop(ctx,0x8c61d38eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d390; /* fmov.s @(r0,r5),fr3 */
L8c61d390: sa_motion_publish_stop(ctx,0x8c61d390u); r[0]=20u;goto L8c61d392; /* mov #0x14,r0 */
L8c61d392: sa_motion_publish_stop(ctx,0x8c61d392u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d394; /* fmov.s fr3,@-r8 */
L8c61d394: sa_motion_publish_stop(ctx,0x8c61d394u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d396; /* fmov.s @(r0,r5),fr3 */
L8c61d396: sa_motion_publish_stop(ctx,0x8c61d396u); r[0]=16u;goto L8c61d398; /* mov #0x10,r0 */
L8c61d398: sa_motion_publish_stop(ctx,0x8c61d398u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d39a; /* fmov.s fr3,@-r8 */
L8c61d39a: sa_motion_publish_stop(ctx,0x8c61d39au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d39c; /* fmov.s @(r0,r5),fr3 */
L8c61d39c: sa_motion_publish_stop(ctx,0x8c61d39cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d39e; /* fmov.s fr3,@-r8 */
L8c61d39e: sa_motion_publish_stop(ctx,0x8c61d39eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d3a0; /* mov.l r3,@-r8 */
L8c61d3a0: sa_motion_publish_stop(ctx,0x8c61d3a0u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d3a2; /* pref @r8 */
L8c61d3a2: sa_motion_publish_stop(ctx,0x8c61d3a2u); r[8]+=32u;goto L8c61d3a4; /* add #0x20,r8 */
L8c61d3a4: sa_motion_publish_stop(ctx,0x8c61d3a4u); r[9]+=1u;goto L8c61d3a6; /* add #0x1,r9 */
L8c61d3a6: sa_motion_publish_stop(ctx,0x8c61d3a6u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61d3a8; /* mov.w @(0x2,r4),r0 */
L8c61d3a8: sa_motion_publish_stop(ctx,0x8c61d3a8u); r[0]=r[0]&65535u;goto L8c61d3aa; /* extu.w r0,r0 */
L8c61d3aa: sa_motion_publish_stop(ctx,0x8c61d3aau); *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61d3ac; /* cmp/ge r0,r9 */
L8c61d3ac: sa_motion_publish_stop(ctx,0x8c61d3acu); if((*sr&1u))goto L8c61d3b2;goto L8c61d3ae; /* bt 0x8c61d3b2 */
L8c61d3ae: sa_motion_publish_stop(ctx,0x8c61d3b0u); goto L8c61d286; /* bra 0x8c61d286 */
L8c61d3b2: sa_motion_publish_stop(ctx,0x8c61d3b4u); goto L8c61d3f4; /* bra 0x8c61d3f4 */
L8c61d3cc: sa_motion_publish_stop(ctx,0x8c61d3ccu); r[1]=rd(m,bus,&io_status,0x8c61d3d4u,4);if(io_status)return io_status;goto L8c61d3ce; /* mov.l 0x8c61d3d4,r1 */
L8c61d3ce: sa_motion_publish_stop(ctx,0x8c61d3ceu); wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c61d3d0; /* mov.l r4,@r1 */
L8c61d3d0: sa_motion_publish_stop(ctx,0x8c61d3d2u); goto L8c61d3d8; /* bra 0x8c61d3d8 */
L8c61d3d8: sa_motion_publish_stop(ctx,0x8c61d3d8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61d3da; /* frchg */
L8c61d3da: sa_motion_publish_stop(ctx,0x8c61d3dau); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61d3dc; /* fmov.s fr0,@-r15 */
L8c61d3dc: sa_motion_publish_stop(ctx,0x8c61d3dcu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61d3de; /* fmov.s fr1,@-r15 */
L8c61d3de: sa_motion_publish_stop(ctx,0x8c61d3deu); r[0]=0x8c61d404u;goto L8c61d3e0; /* mova 0x8c61d404,r0 */
L8c61d3e0: sa_motion_publish_stop(ctx,0x8c61d3e0u); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;goto L8c61d3e2; /* fmov.s @r0,fr0 */
L8c61d3e2: sa_motion_publish_stop(ctx,0x8c61d3e2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61d3e4; /* frchg */
L8c61d3e4: sa_motion_publish_stop(ctx,0x8c61d3e4u); r[2]=rd(m,bus,&io_status,0x8c61d4fcu,4);if(io_status)return io_status;goto L8c61d3e6; /* mov.l 0x8c61d4fc,r2 */
L8c61d3e6: sa_motion_publish_stop(ctx,0x8c61d3e6u); r[3]=rd(m,bus,&io_status,0x8c61d4f8u,4);if(io_status)return io_status;goto L8c61d3e8; /* mov.l 0x8c61d4f8,r3 */
L8c61d3e8: sa_motion_publish_stop(ctx,0x8c61d3eau); target=r[2];*pr=0x8c61d3ecu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61d3ec; /* jsr @r2 */
L8c61d3ec: sa_motion_publish_stop(ctx,0x8c61d3ecu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61d3ee; /* frchg */
L8c61d3ee: sa_motion_publish_stop(ctx,0x8c61d3eeu); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61d3f0; /* fmov.s @r15+,fr1 */
L8c61d3f0: sa_motion_publish_stop(ctx,0x8c61d3f0u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61d3f2; /* fmov.s @r15+,fr0 */
L8c61d3f2: sa_motion_publish_stop(ctx,0x8c61d3f2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61d3f4; /* frchg */
L8c61d3f4: sa_motion_publish_stop(ctx,0x8c61d3f4u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d3f6; /* lds.l @r15+,PR */
L8c61d3f6: sa_motion_publish_stop(ctx,0x8c61d3f6u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d3f8; /* mov.l @r15+,r9 */
L8c61d3f8: sa_motion_publish_stop(ctx,0x8c61d3f8u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d3fa; /* mov.l @r15+,r10 */
L8c61d3fa: sa_motion_publish_stop(ctx,0x8c61d3fau); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d3fc; /* mov.l @r15+,r11 */
L8c61d3fc: sa_motion_publish_stop(ctx,0x8c61d3fcu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d3fe; /* mov.l @r15+,r12 */
L8c61d3fe: sa_motion_publish_stop(ctx,0x8c61d3feu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d400; /* mov.l @r15+,r13 */
L8c61d400: sa_motion_publish_stop(ctx,0x8c61d402u); if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_uv_constant_primitive_clipped_8c61d408(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61d408;
L8c61d408: sa_motion_publish_stop(ctx,0x8c61d408u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61d40a; /* mov.l r14,@-r15 */
L8c61d40a: sa_motion_publish_stop(ctx,0x8c61d40au); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61d40c; /* mov.l r13,@-r15 */
L8c61d40c: sa_motion_publish_stop(ctx,0x8c61d40cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61d40e; /* mov.l r12,@-r15 */
L8c61d40e: sa_motion_publish_stop(ctx,0x8c61d40eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61d410; /* mov.l r11,@-r15 */
L8c61d410: sa_motion_publish_stop(ctx,0x8c61d410u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61d412; /* mov.l r10,@-r15 */
L8c61d412: sa_motion_publish_stop(ctx,0x8c61d412u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61d414; /* mov.l r9,@-r15 */
L8c61d414: sa_motion_publish_stop(ctx,0x8c61d414u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61d416; /* fmov.s fr15,@-r15 */
L8c61d416: sa_motion_publish_stop(ctx,0x8c61d416u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61d418; /* fmov.s fr14,@-r15 */
L8c61d418: sa_motion_publish_stop(ctx,0x8c61d418u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61d41a; /* sts.l PR,@-r15 */
L8c61d41a: sa_motion_publish_stop(ctx,0x8c61d41au); r[15]+=4294967272u;goto L8c61d41c; /* add #-0x18,r15 */
L8c61d41c: sa_motion_publish_stop(ctx,0x8c61d41cu); r[0]=0x8c61d500u;goto L8c61d41e; /* mova 0x8c61d500,r0 */
L8c61d41e: sa_motion_publish_stop(ctx,0x8c61d41eu); r[3]=rd(m,bus,&io_status,0x8c61d504u,4);if(io_status)return io_status;goto L8c61d420; /* mov.l 0x8c61d504,r3 */
L8c61d420: sa_motion_publish_stop(ctx,0x8c61d420u); fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c61d422; /* fmov.s @r0,fr15 */
L8c61d422: sa_motion_publish_stop(ctx,0x8c61d422u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0x3f800000u;goto L8c61d424; /* fldi1 fr14 */
L8c61d424: sa_motion_publish_stop(ctx,0x8c61d424u); wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c61d426; /* mov.l r4,@(0x8,r15) */
L8c61d426: sa_motion_publish_stop(ctx,0x8c61d426u); r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61d428; /* mov.w @r4,r0 */
L8c61d428: sa_motion_publish_stop(ctx,0x8c61d428u); r[11]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c61d42a; /* mov.l @(0x14,r4),r11 */
L8c61d42a: sa_motion_publish_stop(ctx,0x8c61d42au); r[0]=r[0]&65535u;goto L8c61d42c; /* extu.w r0,r0 */
L8c61d42c: sa_motion_publish_stop(ctx,0x8c61d42cu); r[0]&=r[3];goto L8c61d42e; /* and r3,r0 */
L8c61d42e: sa_motion_publish_stop(ctx,0x8c61d42eu); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61d430; /* cmp/eq #0x0,r0 */
L8c61d430: sa_motion_publish_stop(ctx,0x8c61d432u); take=(*sr&1u);r[10]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61d456;goto L8c61d434; /* bt/s 0x8c61d456 */
L8c61d434: sa_motion_publish_stop(ctx,0x8c61d434u); r[1]=sx(m,bus,&io_status,0x8c61d4f6u,2);if(io_status)return io_status;goto L8c61d436; /* mov.w 0x8c61d4f6,r1 */
L8c61d436: sa_motion_publish_stop(ctx,0x8c61d436u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61d438; /* cmp/eq r1,r0 */
L8c61d438: sa_motion_publish_stop(ctx,0x8c61d438u); if(!(*sr&1u))goto L8c61d43e;goto L8c61d43a; /* bf 0x8c61d43e */
L8c61d43a: sa_motion_publish_stop(ctx,0x8c61d43cu); goto L8c61d636; /* bra 0x8c61d636 */
L8c61d43e: sa_motion_publish_stop(ctx,0x8c61d43eu); r[1]=rd(m,bus,&io_status,0x8c61d508u,4);if(io_status)return io_status;goto L8c61d440; /* mov.l 0x8c61d508,r1 */
L8c61d440: sa_motion_publish_stop(ctx,0x8c61d440u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61d442; /* cmp/eq r1,r0 */
L8c61d442: sa_motion_publish_stop(ctx,0x8c61d442u); if(!(*sr&1u))goto L8c61d448;goto L8c61d444; /* bf 0x8c61d448 */
L8c61d444: sa_motion_publish_stop(ctx,0x8c61d446u); goto L8c61d8c0; /* bra 0x8c61d8c0 */
L8c61d448: sa_motion_publish_stop(ctx,0x8c61d448u); r[1]=rd(m,bus,&io_status,0x8c61d504u,4);if(io_status)return io_status;goto L8c61d44a; /* mov.l 0x8c61d504,r1 */
L8c61d44a: sa_motion_publish_stop(ctx,0x8c61d44au); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61d44c; /* cmp/eq r1,r0 */
L8c61d44c: sa_motion_publish_stop(ctx,0x8c61d44cu); if(!(*sr&1u))goto L8c61d452;goto L8c61d44e; /* bf 0x8c61d452 */
L8c61d44e: sa_motion_publish_stop(ctx,0x8c61d450u); goto L8c61db4c; /* bra 0x8c61db4c */
L8c61d452: sa_motion_publish_stop(ctx,0x8c61d454u); goto L8c61db7c; /* bra 0x8c61db7c */
L8c61d456: sa_motion_publish_stop(ctx,0x8c61d456u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61d458; /* mov.l @(0x8,r15),r2 */
L8c61d458: sa_motion_publish_stop(ctx,0x8c61d458u); r[12]=0u;goto L8c61d45a; /* mov #0x0,r12 */
L8c61d45a: sa_motion_publish_stop(ctx,0x8c61d45au); r[1]=rd(m,bus,&io_status,0x8c61d50cu,4);if(io_status)return io_status;goto L8c61d45c; /* mov.l 0x8c61d50c,r1 */
L8c61d45c: sa_motion_publish_stop(ctx,0x8c61d45cu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61d45e; /* mov.w @(0x2,r2),r0 */
L8c61d45e: sa_motion_publish_stop(ctx,0x8c61d45eu); r[13]=rd(m,bus,&io_status,0x8c61d510u,4);if(io_status)return io_status;goto L8c61d460; /* mov.l 0x8c61d510,r13 */
L8c61d460: sa_motion_publish_stop(ctx,0x8c61d460u); r[0]=r[0]&65535u;goto L8c61d462; /* extu.w r0,r0 */
L8c61d462: sa_motion_publish_stop(ctx,0x8c61d462u); r[14]=rd(m,bus,&io_status,0x8c61d514u,4);if(io_status)return io_status;goto L8c61d464; /* mov.l 0x8c61d514,r14 */
L8c61d464: sa_motion_publish_stop(ctx,0x8c61d464u); r[2]=r[0];goto L8c61d466; /* mov r0,r2 */
L8c61d466: sa_motion_publish_stop(ctx,0x8c61d466u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61d468; /* shll r0 */
L8c61d468: sa_motion_publish_stop(ctx,0x8c61d468u); r[0]+=r[2];goto L8c61d46a; /* add r2,r0 */
L8c61d46a: sa_motion_publish_stop(ctx,0x8c61d46au); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61d46c; /* mov.l @r1,r2 */
L8c61d46c: sa_motion_publish_stop(ctx,0x8c61d46cu); r[9]=r[13];goto L8c61d46e; /* mov r13,r9 */
L8c61d46e: sa_motion_publish_stop(ctx,0x8c61d46eu); r[2]+=r[0];goto L8c61d470; /* add r0,r2 */
L8c61d470: sa_motion_publish_stop(ctx,0x8c61d470u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61d472; /* mov.l r2,@r1 */
L8c61d472: sa_motion_publish_stop(ctx,0x8c61d474u); r[9]+=4u;goto L8c61d624; /* bra 0x8c61d624 */
L8c61d476: sa_motion_publish_stop(ctx,0x8c61d476u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d478; /* mov.w @r10+,r4 */
L8c61d478: sa_motion_publish_stop(ctx,0x8c61d478u); r[2]=rd(m,bus,&io_status,0x8c61d518u,4);if(io_status)return io_status;goto L8c61d47a; /* mov.l 0x8c61d518,r2 */
L8c61d47a: sa_motion_publish_stop(ctx,0x8c61d47au); r[5]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d47c; /* mov.w @r10+,r5 */
L8c61d47c: sa_motion_publish_stop(ctx,0x8c61d47cu); r[4]=r[4]&65535u;goto L8c61d47e; /* extu.w r4,r4 */
L8c61d47e: sa_motion_publish_stop(ctx,0x8c61d47eu); r[4]<<=2;goto L8c61d480; /* shll2 r4 */
L8c61d480: sa_motion_publish_stop(ctx,0x8c61d480u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d482; /* mov.l @r2,r3 */
L8c61d482: sa_motion_publish_stop(ctx,0x8c61d482u); r[6]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d484; /* mov.w @r10+,r6 */
L8c61d484: sa_motion_publish_stop(ctx,0x8c61d484u); r[5]=r[5]&65535u;goto L8c61d486; /* extu.w r5,r5 */
L8c61d486: sa_motion_publish_stop(ctx,0x8c61d486u); r[5]<<=2;goto L8c61d488; /* shll2 r5 */
L8c61d488: sa_motion_publish_stop(ctx,0x8c61d488u); r[6]=r[6]&65535u;goto L8c61d48a; /* extu.w r6,r6 */
L8c61d48a: sa_motion_publish_stop(ctx,0x8c61d48au); r[6]<<=2;goto L8c61d48c; /* shll2 r6 */
L8c61d48c: sa_motion_publish_stop(ctx,0x8c61d48cu); r[4]<<=2;goto L8c61d48e; /* shll2 r4 */
L8c61d48e: sa_motion_publish_stop(ctx,0x8c61d48eu); r[6]<<=2;goto L8c61d490; /* shll2 r6 */
L8c61d490: sa_motion_publish_stop(ctx,0x8c61d490u); r[5]<<=2;goto L8c61d492; /* shll2 r5 */
L8c61d492: sa_motion_publish_stop(ctx,0x8c61d492u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61d494; /* shll r4 */
L8c61d494: sa_motion_publish_stop(ctx,0x8c61d494u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d496; /* shll r5 */
L8c61d496: sa_motion_publish_stop(ctx,0x8c61d496u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61d498; /* shll r6 */
L8c61d498: sa_motion_publish_stop(ctx,0x8c61d498u); r[6]+=r[3];goto L8c61d49a; /* add r3,r6 */
L8c61d49a: sa_motion_publish_stop(ctx,0x8c61d49au); r[4]+=r[3];goto L8c61d49c; /* add r3,r4 */
L8c61d49c: sa_motion_publish_stop(ctx,0x8c61d49cu); r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61d49e; /* mov.l @(0xc,r6),r1 */
L8c61d49e: sa_motion_publish_stop(ctx,0x8c61d49eu); r[5]+=r[3];goto L8c61d4a0; /* add r3,r5 */
L8c61d4a0: sa_motion_publish_stop(ctx,0x8c61d4a0u); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61d4a2; /* mov.l @(0xc,r4),r7 */
L8c61d4a2: sa_motion_publish_stop(ctx,0x8c61d4a2u); r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61d4a4; /* mov.l @(0xc,r5),r3 */
L8c61d4a4: sa_motion_publish_stop(ctx,0x8c61d4a4u); r[7]&=r[3];goto L8c61d4a6; /* and r3,r7 */
L8c61d4a6: sa_motion_publish_stop(ctx,0x8c61d4a6u); r[3]=63u;goto L8c61d4a8; /* mov #0x3f,r3 */
L8c61d4a8: sa_motion_publish_stop(ctx,0x8c61d4a8u); r[7]&=r[1];goto L8c61d4aa; /* and r1,r7 */
L8c61d4aa: sa_motion_publish_stop(ctx,0x8c61d4aau); *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c61d4ac; /* tst r3,r7 */
L8c61d4ac: sa_motion_publish_stop(ctx,0x8c61d4acu); if(!(*sr&1u))goto L8c61d4ee;goto L8c61d4ae; /* bf 0x8c61d4ee */
L8c61d4ae: sa_motion_publish_stop(ctx,0x8c61d4aeu); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61d4b0; /* mov.l @(0xc,r4),r7 */
L8c61d4b0: sa_motion_publish_stop(ctx,0x8c61d4b0u); r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61d4b2; /* mov.l @(0xc,r5),r3 */
L8c61d4b2: sa_motion_publish_stop(ctx,0x8c61d4b2u); r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61d4b4; /* mov.l @(0xc,r6),r2 */
L8c61d4b4: sa_motion_publish_stop(ctx,0x8c61d4b4u); r[7]|=r[3];goto L8c61d4b6; /* or r3,r7 */
L8c61d4b6: sa_motion_publish_stop(ctx,0x8c61d4b6u); r[3]=1u;goto L8c61d4b8; /* mov #0x1,r3 */
L8c61d4b8: sa_motion_publish_stop(ctx,0x8c61d4b8u); r[7]|=r[2];goto L8c61d4ba; /* or r2,r7 */
L8c61d4ba: sa_motion_publish_stop(ctx,0x8c61d4bau); *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c61d4bc; /* tst r3,r7 */
L8c61d4bc: sa_motion_publish_stop(ctx,0x8c61d4bcu); if((*sr&1u))goto L8c61d4c2;goto L8c61d4be; /* bt 0x8c61d4c2 */
L8c61d4be: sa_motion_publish_stop(ctx,0x8c61d4c0u); goto L8c61d5c4; /* bra 0x8c61d5c4 */
L8c61d4c2: sa_motion_publish_stop(ctx,0x8c61d4c2u); r[0]=16u;goto L8c61d4c4; /* mov #0x10,r0 */
L8c61d4c4: sa_motion_publish_stop(ctx,0x8c61d4c4u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d4c6; /* fmov.s @(r0,r5),fr4 */
L8c61d4c6: sa_motion_publish_stop(ctx,0x8c61d4c6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d4c8; /* fmov.s @(r0,r4),fr3 */
L8c61d4c8: sa_motion_publish_stop(ctx,0x8c61d4c8u); r[0]=20u;goto L8c61d4ca; /* mov #0x14,r0 */
L8c61d4ca: sa_motion_publish_stop(ctx,0x8c61d4cau); fcopy(f,2u,4u);goto L8c61d4cc; /* fmov fr4,fr2 */
L8c61d4cc: sa_motion_publish_stop(ctx,0x8c61d4ccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d4ce; /* fsub fr3,fr2 */
L8c61d4ce: sa_motion_publish_stop(ctx,0x8c61d4ceu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d4d0; /* fmov.s @(r0,r6),fr3 */
L8c61d4d0: sa_motion_publish_stop(ctx,0x8c61d4d0u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d4d2; /* fmov.s @(r0,r5),fr5 */
L8c61d4d2: sa_motion_publish_stop(ctx,0x8c61d4d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d4d4; /* fsub fr5,fr3 */
L8c61d4d4: sa_motion_publish_stop(ctx,0x8c61d4d4u); fcopy(f,1u,5u);goto L8c61d4d6; /* fmov fr5,fr1 */
L8c61d4d6: sa_motion_publish_stop(ctx,0x8c61d4d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d4d8; /* fmul fr3,fr2 */
L8c61d4d8: sa_motion_publish_stop(ctx,0x8c61d4d8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d4da; /* fmov.s @(r0,r4),fr3 */
L8c61d4da: sa_motion_publish_stop(ctx,0x8c61d4dau); r[0]=16u;goto L8c61d4dc; /* mov #0x10,r0 */
L8c61d4dc: sa_motion_publish_stop(ctx,0x8c61d4dcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d4de; /* fsub fr3,fr1 */
L8c61d4de: sa_motion_publish_stop(ctx,0x8c61d4deu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d4e0; /* fmov.s @(r0,r6),fr3 */
L8c61d4e0: sa_motion_publish_stop(ctx,0x8c61d4e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d4e2; /* fsub fr4,fr3 */
L8c61d4e2: sa_motion_publish_stop(ctx,0x8c61d4e2u); fcopy(f,4u,2u);goto L8c61d4e4; /* fmov fr2,fr4 */
L8c61d4e4: sa_motion_publish_stop(ctx,0x8c61d4e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d4e6; /* fmul fr3,fr1 */
L8c61d4e6: sa_motion_publish_stop(ctx,0x8c61d4e6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d4e8; /* fldi0 fr3 */
L8c61d4e8: sa_motion_publish_stop(ctx,0x8c61d4e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d4ea; /* fsub fr1,fr4 */
L8c61d4ea: sa_motion_publish_stop(ctx,0x8c61d4eau); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d4ec; /* fcmp/gt fr3,fr4 */
L8c61d4ec: sa_motion_publish_stop(ctx,0x8c61d4ecu); if(!(*sr&1u))goto L8c61d51c;goto L8c61d4ee; /* bf 0x8c61d51c */
L8c61d4ee: sa_motion_publish_stop(ctx,0x8c61d4f0u); r[11]+=12u;goto L8c61d622; /* bra 0x8c61d622 */
L8c61d51c: sa_motion_publish_stop(ctx,0x8c61d51cu); r[8]+=28u;goto L8c61d51e; /* add #0x1c,r8 */
L8c61d51e: sa_motion_publish_stop(ctx,0x8c61d51eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d520; /* fmov.s fr14,@-r8 */
L8c61d520: sa_motion_publish_stop(ctx,0x8c61d520u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d522; /* mov.w @(0x2,r11),r0 */
L8c61d522: sa_motion_publish_stop(ctx,0x8c61d522u); r[3]=r[0];goto L8c61d524; /* mov r0,r3 */
L8c61d524: sa_motion_publish_stop(ctx,0x8c61d524u); *fpul=r[3];goto L8c61d526; /* lds r3,FPUL */
L8c61d526: sa_motion_publish_stop(ctx,0x8c61d526u); r[0]=24u;goto L8c61d528; /* mov #0x18,r0 */
L8c61d528: sa_motion_publish_stop(ctx,0x8c61d528u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d52a; /* float FPUL,fr3 */
L8c61d52a: sa_motion_publish_stop(ctx,0x8c61d52au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d52c; /* fmul fr15,fr3 */
L8c61d52c: sa_motion_publish_stop(ctx,0x8c61d52cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d52e; /* fmov.s fr3,@-r8 */
L8c61d52e: sa_motion_publish_stop(ctx,0x8c61d52eu); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d530; /* mov.w @r11,r3 */
L8c61d530: sa_motion_publish_stop(ctx,0x8c61d530u); r[11]+=4u;goto L8c61d532; /* add #0x4,r11 */
L8c61d532: sa_motion_publish_stop(ctx,0x8c61d532u); *fpul=r[3];goto L8c61d534; /* lds r3,FPUL */
L8c61d534: sa_motion_publish_stop(ctx,0x8c61d534u); r[3]=rd(m,bus,&io_status,0x8c61d7c8u,4);if(io_status)return io_status;goto L8c61d536; /* mov.l 0x8c61d7c8,r3 */
L8c61d536: sa_motion_publish_stop(ctx,0x8c61d536u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d538; /* float FPUL,fr3 */
L8c61d538: sa_motion_publish_stop(ctx,0x8c61d538u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d53a; /* fmul fr15,fr3 */
L8c61d53a: sa_motion_publish_stop(ctx,0x8c61d53au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d53c; /* fmov.s fr3,@-r8 */
L8c61d53c: sa_motion_publish_stop(ctx,0x8c61d53cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d53e; /* fmov.s @(r0,r4),fr3 */
L8c61d53e: sa_motion_publish_stop(ctx,0x8c61d53eu); r[0]=20u;goto L8c61d540; /* mov #0x14,r0 */
L8c61d540: sa_motion_publish_stop(ctx,0x8c61d540u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d542; /* fmov.s fr3,@-r8 */
L8c61d542: sa_motion_publish_stop(ctx,0x8c61d542u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d544; /* fmov.s @(r0,r4),fr3 */
L8c61d544: sa_motion_publish_stop(ctx,0x8c61d544u); r[0]=16u;goto L8c61d546; /* mov #0x10,r0 */
L8c61d546: sa_motion_publish_stop(ctx,0x8c61d546u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d548; /* fmov.s fr3,@-r8 */
L8c61d548: sa_motion_publish_stop(ctx,0x8c61d548u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d54a; /* fmov.s @(r0,r4),fr3 */
L8c61d54a: sa_motion_publish_stop(ctx,0x8c61d54au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d54c; /* fmov.s fr3,@-r8 */
L8c61d54c: sa_motion_publish_stop(ctx,0x8c61d54cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d54e; /* mov.l r3,@-r8 */
L8c61d54e: sa_motion_publish_stop(ctx,0x8c61d54eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d550; /* pref @r8 */
L8c61d550: sa_motion_publish_stop(ctx,0x8c61d550u); r[8]+=60u;goto L8c61d552; /* add #0x3c,r8 */
L8c61d552: sa_motion_publish_stop(ctx,0x8c61d552u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d554; /* fmov.s fr14,@-r8 */
L8c61d554: sa_motion_publish_stop(ctx,0x8c61d554u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d556; /* mov.w @(0x2,r11),r0 */
L8c61d556: sa_motion_publish_stop(ctx,0x8c61d556u); r[3]=r[0];goto L8c61d558; /* mov r0,r3 */
L8c61d558: sa_motion_publish_stop(ctx,0x8c61d558u); *fpul=r[3];goto L8c61d55a; /* lds r3,FPUL */
L8c61d55a: sa_motion_publish_stop(ctx,0x8c61d55au); r[0]=24u;goto L8c61d55c; /* mov #0x18,r0 */
L8c61d55c: sa_motion_publish_stop(ctx,0x8c61d55cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d55e; /* float FPUL,fr3 */
L8c61d55e: sa_motion_publish_stop(ctx,0x8c61d55eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d560; /* fmul fr15,fr3 */
L8c61d560: sa_motion_publish_stop(ctx,0x8c61d560u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d562; /* fmov.s fr3,@-r8 */
L8c61d562: sa_motion_publish_stop(ctx,0x8c61d562u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d564; /* mov.w @r11,r3 */
L8c61d564: sa_motion_publish_stop(ctx,0x8c61d564u); r[11]+=4u;goto L8c61d566; /* add #0x4,r11 */
L8c61d566: sa_motion_publish_stop(ctx,0x8c61d566u); *fpul=r[3];goto L8c61d568; /* lds r3,FPUL */
L8c61d568: sa_motion_publish_stop(ctx,0x8c61d568u); r[3]=rd(m,bus,&io_status,0x8c61d7c8u,4);if(io_status)return io_status;goto L8c61d56a; /* mov.l 0x8c61d7c8,r3 */
L8c61d56a: sa_motion_publish_stop(ctx,0x8c61d56au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d56c; /* float FPUL,fr3 */
L8c61d56c: sa_motion_publish_stop(ctx,0x8c61d56cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d56e; /* fmul fr15,fr3 */
L8c61d56e: sa_motion_publish_stop(ctx,0x8c61d56eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d570; /* fmov.s fr3,@-r8 */
L8c61d570: sa_motion_publish_stop(ctx,0x8c61d570u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d572; /* fmov.s @(r0,r5),fr3 */
L8c61d572: sa_motion_publish_stop(ctx,0x8c61d572u); r[0]=20u;goto L8c61d574; /* mov #0x14,r0 */
L8c61d574: sa_motion_publish_stop(ctx,0x8c61d574u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d576; /* fmov.s fr3,@-r8 */
L8c61d576: sa_motion_publish_stop(ctx,0x8c61d576u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d578; /* fmov.s @(r0,r5),fr3 */
L8c61d578: sa_motion_publish_stop(ctx,0x8c61d578u); r[0]=16u;goto L8c61d57a; /* mov #0x10,r0 */
L8c61d57a: sa_motion_publish_stop(ctx,0x8c61d57au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d57c; /* fmov.s fr3,@-r8 */
L8c61d57c: sa_motion_publish_stop(ctx,0x8c61d57cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d57e; /* fmov.s @(r0,r5),fr3 */
L8c61d57e: sa_motion_publish_stop(ctx,0x8c61d57eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d580; /* fmov.s fr3,@-r8 */
L8c61d580: sa_motion_publish_stop(ctx,0x8c61d580u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d582; /* mov.l r3,@-r8 */
L8c61d582: sa_motion_publish_stop(ctx,0x8c61d582u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d584; /* pref @r8 */
L8c61d584: sa_motion_publish_stop(ctx,0x8c61d584u); r[8]+=60u;goto L8c61d586; /* add #0x3c,r8 */
L8c61d586: sa_motion_publish_stop(ctx,0x8c61d586u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d588; /* fmov.s fr14,@-r8 */
L8c61d588: sa_motion_publish_stop(ctx,0x8c61d588u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d58a; /* mov.w @(0x2,r11),r0 */
L8c61d58a: sa_motion_publish_stop(ctx,0x8c61d58au); r[3]=r[0];goto L8c61d58c; /* mov r0,r3 */
L8c61d58c: sa_motion_publish_stop(ctx,0x8c61d58cu); *fpul=r[3];goto L8c61d58e; /* lds r3,FPUL */
L8c61d58e: sa_motion_publish_stop(ctx,0x8c61d58eu); r[0]=24u;goto L8c61d590; /* mov #0x18,r0 */
L8c61d590: sa_motion_publish_stop(ctx,0x8c61d590u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d592; /* float FPUL,fr3 */
L8c61d592: sa_motion_publish_stop(ctx,0x8c61d592u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d594; /* fmul fr15,fr3 */
L8c61d594: sa_motion_publish_stop(ctx,0x8c61d594u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d596; /* fmov.s fr3,@-r8 */
L8c61d596: sa_motion_publish_stop(ctx,0x8c61d596u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d598; /* mov.w @r11,r3 */
L8c61d598: sa_motion_publish_stop(ctx,0x8c61d598u); r[11]+=4u;goto L8c61d59a; /* add #0x4,r11 */
L8c61d59a: sa_motion_publish_stop(ctx,0x8c61d59au); *fpul=r[3];goto L8c61d59c; /* lds r3,FPUL */
L8c61d59c: sa_motion_publish_stop(ctx,0x8c61d59cu); r[3]=rd(m,bus,&io_status,0x8c61d7ccu,4);if(io_status)return io_status;goto L8c61d59e; /* mov.l 0x8c61d7cc,r3 */
L8c61d59e: sa_motion_publish_stop(ctx,0x8c61d59eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d5a0; /* float FPUL,fr3 */
L8c61d5a0: sa_motion_publish_stop(ctx,0x8c61d5a0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d5a2; /* fmul fr15,fr3 */
L8c61d5a2: sa_motion_publish_stop(ctx,0x8c61d5a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d5a4; /* fmov.s fr3,@-r8 */
L8c61d5a4: sa_motion_publish_stop(ctx,0x8c61d5a4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d5a6; /* fmov.s @(r0,r6),fr3 */
L8c61d5a6: sa_motion_publish_stop(ctx,0x8c61d5a6u); r[0]=20u;goto L8c61d5a8; /* mov #0x14,r0 */
L8c61d5a8: sa_motion_publish_stop(ctx,0x8c61d5a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d5aa; /* fmov.s fr3,@-r8 */
L8c61d5aa: sa_motion_publish_stop(ctx,0x8c61d5aau); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d5ac; /* fmov.s @(r0,r6),fr3 */
L8c61d5ac: sa_motion_publish_stop(ctx,0x8c61d5acu); r[0]=16u;goto L8c61d5ae; /* mov #0x10,r0 */
L8c61d5ae: sa_motion_publish_stop(ctx,0x8c61d5aeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d5b0; /* fmov.s fr3,@-r8 */
L8c61d5b0: sa_motion_publish_stop(ctx,0x8c61d5b0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d5b2; /* fmov.s @(r0,r6),fr3 */
L8c61d5b2: sa_motion_publish_stop(ctx,0x8c61d5b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d5b4; /* fmov.s fr3,@-r8 */
L8c61d5b4: sa_motion_publish_stop(ctx,0x8c61d5b4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d5b6; /* mov.l r3,@-r8 */
L8c61d5b6: sa_motion_publish_stop(ctx,0x8c61d5b6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d5b8; /* pref @r8 */
L8c61d5b8: sa_motion_publish_stop(ctx,0x8c61d5b8u); r[2]=rd(m,bus,&io_status,0x8c61d7d0u,4);if(io_status)return io_status;goto L8c61d5ba; /* mov.l 0x8c61d7d0,r2 */
L8c61d5ba: sa_motion_publish_stop(ctx,0x8c61d5bau); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d5bc; /* mov.l @r2,r3 */
L8c61d5bc: sa_motion_publish_stop(ctx,0x8c61d5bcu); r[3]+=3u;goto L8c61d5be; /* add #0x3,r3 */
L8c61d5be: sa_motion_publish_stop(ctx,0x8c61d5beu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61d5c0; /* mov.l r3,@r2 */
L8c61d5c0: sa_motion_publish_stop(ctx,0x8c61d5c2u); r[8]+=32u;goto L8c61d622; /* bra 0x8c61d622 */
L8c61d5c4: sa_motion_publish_stop(ctx,0x8c61d5c4u); r[1]=rd(m,bus,&io_status,0x8c61d7d4u,4);if(io_status)return io_status;goto L8c61d5c6; /* mov.l 0x8c61d7d4,r1 */
L8c61d5c6: sa_motion_publish_stop(ctx,0x8c61d5c6u); wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c61d5c8; /* mov.l r3,@r1 */
L8c61d5c8: sa_motion_publish_stop(ctx,0x8c61d5c8u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d5ca; /* mov.w @r11,r3 */
L8c61d5ca: sa_motion_publish_stop(ctx,0x8c61d5cau); *fpul=r[3];goto L8c61d5cc; /* lds r3,FPUL */
L8c61d5cc: sa_motion_publish_stop(ctx,0x8c61d5ccu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d5ce; /* float FPUL,fr3 */
L8c61d5ce: sa_motion_publish_stop(ctx,0x8c61d5ceu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d5d0; /* ftrc fr3,FPUL */
L8c61d5d0: sa_motion_publish_stop(ctx,0x8c61d5d0u); r[2]=*fpul;goto L8c61d5d2; /* sts FPUL,r2 */
L8c61d5d2: sa_motion_publish_stop(ctx,0x8c61d5d2u); wr(m,bus,&io_status,r[13],r[2],2);if(io_status)return io_status;goto L8c61d5d4; /* mov.w r2,@r13 */
L8c61d5d4: sa_motion_publish_stop(ctx,0x8c61d5d4u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d5d6; /* mov.w @(0x2,r11),r0 */
L8c61d5d6: sa_motion_publish_stop(ctx,0x8c61d5d6u); r[11]+=4u;goto L8c61d5d8; /* add #0x4,r11 */
L8c61d5d8: sa_motion_publish_stop(ctx,0x8c61d5d8u); r[3]=r[0];goto L8c61d5da; /* mov r0,r3 */
L8c61d5da: sa_motion_publish_stop(ctx,0x8c61d5dau); *fpul=r[3];goto L8c61d5dc; /* lds r3,FPUL */
L8c61d5dc: sa_motion_publish_stop(ctx,0x8c61d5dcu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d5de; /* float FPUL,fr3 */
L8c61d5de: sa_motion_publish_stop(ctx,0x8c61d5deu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d5e0; /* ftrc fr3,FPUL */
L8c61d5e0: sa_motion_publish_stop(ctx,0x8c61d5e0u); r[0]=*fpul;goto L8c61d5e2; /* sts FPUL,r0 */
L8c61d5e2: sa_motion_publish_stop(ctx,0x8c61d5e2u); wr(m,bus,&io_status,r[13]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d5e4; /* mov.w r0,@(0x2,r13) */
L8c61d5e4: sa_motion_publish_stop(ctx,0x8c61d5e4u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d5e6; /* mov.w @r11,r3 */
L8c61d5e6: sa_motion_publish_stop(ctx,0x8c61d5e6u); *fpul=r[3];goto L8c61d5e8; /* lds r3,FPUL */
L8c61d5e8: sa_motion_publish_stop(ctx,0x8c61d5e8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d5ea; /* float FPUL,fr3 */
L8c61d5ea: sa_motion_publish_stop(ctx,0x8c61d5eau); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d5ec; /* ftrc fr3,FPUL */
L8c61d5ec: sa_motion_publish_stop(ctx,0x8c61d5ecu); r[2]=*fpul;goto L8c61d5ee; /* sts FPUL,r2 */
L8c61d5ee: sa_motion_publish_stop(ctx,0x8c61d5eeu); wr(m,bus,&io_status,r[9],r[2],2);if(io_status)return io_status;goto L8c61d5f0; /* mov.w r2,@r9 */
L8c61d5f0: sa_motion_publish_stop(ctx,0x8c61d5f0u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d5f2; /* mov.w @(0x2,r11),r0 */
L8c61d5f2: sa_motion_publish_stop(ctx,0x8c61d5f2u); r[11]+=4u;goto L8c61d5f4; /* add #0x4,r11 */
L8c61d5f4: sa_motion_publish_stop(ctx,0x8c61d5f4u); r[3]=r[0];goto L8c61d5f6; /* mov r0,r3 */
L8c61d5f6: sa_motion_publish_stop(ctx,0x8c61d5f6u); *fpul=r[3];goto L8c61d5f8; /* lds r3,FPUL */
L8c61d5f8: sa_motion_publish_stop(ctx,0x8c61d5f8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d5fa; /* float FPUL,fr3 */
L8c61d5fa: sa_motion_publish_stop(ctx,0x8c61d5fau); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d5fc; /* ftrc fr3,FPUL */
L8c61d5fc: sa_motion_publish_stop(ctx,0x8c61d5fcu); r[0]=*fpul;goto L8c61d5fe; /* sts FPUL,r0 */
L8c61d5fe: sa_motion_publish_stop(ctx,0x8c61d5feu); wr(m,bus,&io_status,r[9]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d600; /* mov.w r0,@(0x2,r9) */
L8c61d600: sa_motion_publish_stop(ctx,0x8c61d600u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d602; /* mov.w @r11,r3 */
L8c61d602: sa_motion_publish_stop(ctx,0x8c61d602u); *fpul=r[3];goto L8c61d604; /* lds r3,FPUL */
L8c61d604: sa_motion_publish_stop(ctx,0x8c61d604u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d606; /* float FPUL,fr3 */
L8c61d606: sa_motion_publish_stop(ctx,0x8c61d606u); r[7]=rd(m,bus,&io_status,0x8c61d7d8u,4);if(io_status)return io_status;goto L8c61d608; /* mov.l 0x8c61d7d8,r7 */
L8c61d608: sa_motion_publish_stop(ctx,0x8c61d608u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d60a; /* ftrc fr3,FPUL */
L8c61d60a: sa_motion_publish_stop(ctx,0x8c61d60au); r[2]=*fpul;goto L8c61d60c; /* sts FPUL,r2 */
L8c61d60c: sa_motion_publish_stop(ctx,0x8c61d60cu); wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c61d60e; /* mov.w r2,@r14 */
L8c61d60e: sa_motion_publish_stop(ctx,0x8c61d60eu); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d610; /* mov.w @(0x2,r11),r0 */
L8c61d610: sa_motion_publish_stop(ctx,0x8c61d610u); r[3]=r[0];goto L8c61d612; /* mov r0,r3 */
L8c61d612: sa_motion_publish_stop(ctx,0x8c61d612u); *fpul=r[3];goto L8c61d614; /* lds r3,FPUL */
L8c61d614: sa_motion_publish_stop(ctx,0x8c61d614u); r[3]=rd(m,bus,&io_status,0x8c61d7dcu,4);if(io_status)return io_status;goto L8c61d616; /* mov.l 0x8c61d7dc,r3 */
L8c61d616: sa_motion_publish_stop(ctx,0x8c61d616u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d618; /* float FPUL,fr3 */
L8c61d618: sa_motion_publish_stop(ctx,0x8c61d618u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d61a; /* ftrc fr3,FPUL */
L8c61d61a: sa_motion_publish_stop(ctx,0x8c61d61au); r[0]=*fpul;goto L8c61d61c; /* sts FPUL,r0 */
L8c61d61c: sa_motion_publish_stop(ctx,0x8c61d61cu); wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d61e; /* mov.w r0,@(0x2,r14) */
L8c61d61e: sa_motion_publish_stop(ctx,0x8c61d620u); target=r[3];*pr=0x8c61d622u;r[11]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61d622; /* jsr @r3 */
L8c61d622: sa_motion_publish_stop(ctx,0x8c61d622u); r[12]+=1u;goto L8c61d624; /* add #0x1,r12 */
L8c61d624: sa_motion_publish_stop(ctx,0x8c61d624u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61d626; /* mov.l @(0x8,r15),r2 */
L8c61d626: sa_motion_publish_stop(ctx,0x8c61d626u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61d628; /* mov.w @(0x2,r2),r0 */
L8c61d628: sa_motion_publish_stop(ctx,0x8c61d628u); r[0]=r[0]&65535u;goto L8c61d62a; /* extu.w r0,r0 */
L8c61d62a: sa_motion_publish_stop(ctx,0x8c61d62au); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[0]);goto L8c61d62c; /* cmp/ge r0,r12 */
L8c61d62c: sa_motion_publish_stop(ctx,0x8c61d62cu); if((*sr&1u))goto L8c61d632;goto L8c61d62e; /* bt 0x8c61d632 */
L8c61d62e: sa_motion_publish_stop(ctx,0x8c61d630u); goto L8c61d476; /* bra 0x8c61d476 */
L8c61d632: sa_motion_publish_stop(ctx,0x8c61d634u); goto L8c61db7c; /* bra 0x8c61db7c */
L8c61d636: sa_motion_publish_stop(ctx,0x8c61d636u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61d638; /* mov.l @(0x8,r15),r1 */
L8c61d638: sa_motion_publish_stop(ctx,0x8c61d638u); r[2]=rd(m,bus,&io_status,0x8c61d7e0u,4);if(io_status)return io_status;goto L8c61d63a; /* mov.l 0x8c61d7e0,r2 */
L8c61d63a: sa_motion_publish_stop(ctx,0x8c61d63au); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61d63c; /* mov.w @(0x2,r1),r0 */
L8c61d63c: sa_motion_publish_stop(ctx,0x8c61d63cu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d63e; /* mov.l @r2,r1 */
L8c61d63e: sa_motion_publish_stop(ctx,0x8c61d63eu); r[0]=r[0]&65535u;goto L8c61d640; /* extu.w r0,r0 */
L8c61d640: sa_motion_publish_stop(ctx,0x8c61d640u); r[9]=rd(m,bus,&io_status,0x8c61d7d8u,4);if(io_status)return io_status;goto L8c61d642; /* mov.l 0x8c61d7d8,r9 */
L8c61d642: sa_motion_publish_stop(ctx,0x8c61d642u); r[0]<<=2;goto L8c61d644; /* shll2 r0 */
L8c61d644: sa_motion_publish_stop(ctx,0x8c61d644u); r[3]=rd(m,bus,&io_status,0x8c61d7e4u,4);if(io_status)return io_status;goto L8c61d646; /* mov.l 0x8c61d7e4,r3 */
L8c61d646: sa_motion_publish_stop(ctx,0x8c61d646u); r[1]+=r[0];goto L8c61d648; /* add r0,r1 */
L8c61d648: sa_motion_publish_stop(ctx,0x8c61d648u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61d64a; /* mov.l r1,@r2 */
L8c61d64a: sa_motion_publish_stop(ctx,0x8c61d64au); r[2]=0u;goto L8c61d64c; /* mov #0x0,r2 */
L8c61d64c: sa_motion_publish_stop(ctx,0x8c61d64cu); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c61d64e; /* mov.l r2,@(0xc,r15) */
L8c61d64e: sa_motion_publish_stop(ctx,0x8c61d64eu); r[2]=r[9];goto L8c61d650; /* mov r9,r2 */
L8c61d650: sa_motion_publish_stop(ctx,0x8c61d650u); r[1]=rd(m,bus,&io_status,0x8c61d7e8u,4);if(io_status)return io_status;goto L8c61d652; /* mov.l 0x8c61d7e8,r1 */
L8c61d652: sa_motion_publish_stop(ctx,0x8c61d652u); r[2]+=4u;goto L8c61d654; /* add #0x4,r2 */
L8c61d654: sa_motion_publish_stop(ctx,0x8c61d654u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61d656; /* mov.l r2,@r15 */
L8c61d656: sa_motion_publish_stop(ctx,0x8c61d656u); wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c61d658; /* mov.l r3,@(0x10,r15) */
L8c61d658: sa_motion_publish_stop(ctx,0x8c61d65au); wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c61d8ac; /* bra 0x8c61d8ac */
L8c61d65c: sa_motion_publish_stop(ctx,0x8c61d65cu); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d65e; /* mov.w @r10+,r4 */
L8c61d65e: sa_motion_publish_stop(ctx,0x8c61d65eu); r[2]=rd(m,bus,&io_status,0x8c61d7ecu,4);if(io_status)return io_status;goto L8c61d660; /* mov.l 0x8c61d7ec,r2 */
L8c61d660: sa_motion_publish_stop(ctx,0x8c61d660u); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d662; /* mov.w @r10+,r14 */
L8c61d662: sa_motion_publish_stop(ctx,0x8c61d662u); r[4]=r[4]&65535u;goto L8c61d664; /* extu.w r4,r4 */
L8c61d664: sa_motion_publish_stop(ctx,0x8c61d664u); r[4]<<=2;goto L8c61d666; /* shll2 r4 */
L8c61d666: sa_motion_publish_stop(ctx,0x8c61d666u); r[6]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d668; /* mov.l @r2,r6 */
L8c61d668: sa_motion_publish_stop(ctx,0x8c61d668u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d66a; /* mov.w @r10+,r13 */
L8c61d66a: sa_motion_publish_stop(ctx,0x8c61d66au); r[14]=r[14]&65535u;goto L8c61d66c; /* extu.w r14,r14 */
L8c61d66c: sa_motion_publish_stop(ctx,0x8c61d66cu); r[14]<<=2;goto L8c61d66e; /* shll2 r14 */
L8c61d66e: sa_motion_publish_stop(ctx,0x8c61d66eu); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d670; /* mov.w @r10+,r12 */
L8c61d670: sa_motion_publish_stop(ctx,0x8c61d670u); r[13]=r[13]&65535u;goto L8c61d672; /* extu.w r13,r13 */
L8c61d672: sa_motion_publish_stop(ctx,0x8c61d672u); r[14]<<=2;goto L8c61d674; /* shll2 r14 */
L8c61d674: sa_motion_publish_stop(ctx,0x8c61d674u); r[4]<<=2;goto L8c61d676; /* shll2 r4 */
L8c61d676: sa_motion_publish_stop(ctx,0x8c61d676u); r[12]=r[12]&65535u;goto L8c61d678; /* extu.w r12,r12 */
L8c61d678: sa_motion_publish_stop(ctx,0x8c61d678u); r[13]<<=2;goto L8c61d67a; /* shll2 r13 */
L8c61d67a: sa_motion_publish_stop(ctx,0x8c61d67au); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61d67c; /* shll r4 */
L8c61d67c: sa_motion_publish_stop(ctx,0x8c61d67cu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61d67e; /* shll r14 */
L8c61d67e: sa_motion_publish_stop(ctx,0x8c61d67eu); r[4]+=r[6];goto L8c61d680; /* add r6,r4 */
L8c61d680: sa_motion_publish_stop(ctx,0x8c61d680u); r[12]<<=2;goto L8c61d682; /* shll2 r12 */
L8c61d682: sa_motion_publish_stop(ctx,0x8c61d682u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61d684; /* mov.l @(0xc,r4),r5 */
L8c61d684: sa_motion_publish_stop(ctx,0x8c61d684u); r[14]+=r[6];goto L8c61d686; /* add r6,r14 */
L8c61d686: sa_motion_publish_stop(ctx,0x8c61d686u); r[13]<<=2;goto L8c61d688; /* shll2 r13 */
L8c61d688: sa_motion_publish_stop(ctx,0x8c61d688u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61d68a; /* mov.l @(0xc,r14),r3 */
L8c61d68a: sa_motion_publish_stop(ctx,0x8c61d68au); r[12]<<=2;goto L8c61d68c; /* shll2 r12 */
L8c61d68c: sa_motion_publish_stop(ctx,0x8c61d68cu); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61d68e; /* shll r13 */
L8c61d68e: sa_motion_publish_stop(ctx,0x8c61d68eu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61d690; /* shll r12 */
L8c61d690: sa_motion_publish_stop(ctx,0x8c61d690u); r[13]+=r[6];goto L8c61d692; /* add r6,r13 */
L8c61d692: sa_motion_publish_stop(ctx,0x8c61d692u); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61d694; /* mov.l @(0xc,r13),r1 */
L8c61d694: sa_motion_publish_stop(ctx,0x8c61d694u); r[5]&=r[3];goto L8c61d696; /* and r3,r5 */
L8c61d696: sa_motion_publish_stop(ctx,0x8c61d696u); r[12]+=r[6];goto L8c61d698; /* add r6,r12 */
L8c61d698: sa_motion_publish_stop(ctx,0x8c61d698u); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61d69a; /* mov.l @(0xc,r12),r3 */
L8c61d69a: sa_motion_publish_stop(ctx,0x8c61d69au); r[5]&=r[1];goto L8c61d69c; /* and r1,r5 */
L8c61d69c: sa_motion_publish_stop(ctx,0x8c61d69cu); r[1]=63u;goto L8c61d69e; /* mov #0x3f,r1 */
L8c61d69e: sa_motion_publish_stop(ctx,0x8c61d69eu); r[5]&=r[3];goto L8c61d6a0; /* and r3,r5 */
L8c61d6a0: sa_motion_publish_stop(ctx,0x8c61d6a0u); *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c61d6a2; /* tst r1,r5 */
L8c61d6a2: sa_motion_publish_stop(ctx,0x8c61d6a2u); if(!(*sr&1u))goto L8c61d6e8;goto L8c61d6a4; /* bf 0x8c61d6e8 */
L8c61d6a4: sa_motion_publish_stop(ctx,0x8c61d6a4u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61d6a6; /* mov.l @(0xc,r4),r5 */
L8c61d6a6: sa_motion_publish_stop(ctx,0x8c61d6a6u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61d6a8; /* mov.l @(0xc,r14),r3 */
L8c61d6a8: sa_motion_publish_stop(ctx,0x8c61d6a8u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61d6aa; /* mov.l @(0xc,r13),r2 */
L8c61d6aa: sa_motion_publish_stop(ctx,0x8c61d6aau); r[5]|=r[3];goto L8c61d6ac; /* or r3,r5 */
L8c61d6ac: sa_motion_publish_stop(ctx,0x8c61d6acu); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61d6ae; /* mov.l @(0xc,r12),r3 */
L8c61d6ae: sa_motion_publish_stop(ctx,0x8c61d6aeu); r[5]|=r[2];goto L8c61d6b0; /* or r2,r5 */
L8c61d6b0: sa_motion_publish_stop(ctx,0x8c61d6b0u); r[2]=1u;goto L8c61d6b2; /* mov #0x1,r2 */
L8c61d6b2: sa_motion_publish_stop(ctx,0x8c61d6b2u); r[5]|=r[3];goto L8c61d6b4; /* or r3,r5 */
L8c61d6b4: sa_motion_publish_stop(ctx,0x8c61d6b4u); *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c61d6b6; /* tst r2,r5 */
L8c61d6b6: sa_motion_publish_stop(ctx,0x8c61d6b6u); if((*sr&1u))goto L8c61d6bc;goto L8c61d6b8; /* bt 0x8c61d6bc */
L8c61d6b8: sa_motion_publish_stop(ctx,0x8c61d6bau); goto L8c61d7f0; /* bra 0x8c61d7f0 */
L8c61d6bc: sa_motion_publish_stop(ctx,0x8c61d6bcu); r[0]=16u;goto L8c61d6be; /* mov #0x10,r0 */
L8c61d6be: sa_motion_publish_stop(ctx,0x8c61d6beu); fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d6c0; /* fmov.s @(r0,r14),fr5 */
L8c61d6c0: sa_motion_publish_stop(ctx,0x8c61d6c0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d6c2; /* fmov.s @(r0,r4),fr3 */
L8c61d6c2: sa_motion_publish_stop(ctx,0x8c61d6c2u); r[0]=20u;goto L8c61d6c4; /* mov #0x14,r0 */
L8c61d6c4: sa_motion_publish_stop(ctx,0x8c61d6c4u); fcopy(f,2u,5u);goto L8c61d6c6; /* fmov fr5,fr2 */
L8c61d6c6: sa_motion_publish_stop(ctx,0x8c61d6c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d6c8; /* fsub fr3,fr2 */
L8c61d6c8: sa_motion_publish_stop(ctx,0x8c61d6c8u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61d6ca; /* fmov.s @(r0,r13),fr3 */
L8c61d6ca: sa_motion_publish_stop(ctx,0x8c61d6cau); fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d6cc; /* fmov.s @(r0,r14),fr4 */
L8c61d6cc: sa_motion_publish_stop(ctx,0x8c61d6ccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d6ce; /* fsub fr4,fr3 */
L8c61d6ce: sa_motion_publish_stop(ctx,0x8c61d6ceu); fcopy(f,1u,4u);goto L8c61d6d0; /* fmov fr4,fr1 */
L8c61d6d0: sa_motion_publish_stop(ctx,0x8c61d6d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d6d2; /* fmul fr3,fr2 */
L8c61d6d2: sa_motion_publish_stop(ctx,0x8c61d6d2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d6d4; /* fmov.s @(r0,r4),fr3 */
L8c61d6d4: sa_motion_publish_stop(ctx,0x8c61d6d4u); r[0]=16u;goto L8c61d6d6; /* mov #0x10,r0 */
L8c61d6d6: sa_motion_publish_stop(ctx,0x8c61d6d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d6d8; /* fsub fr3,fr1 */
L8c61d6d8: sa_motion_publish_stop(ctx,0x8c61d6d8u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61d6da; /* fmov.s @(r0,r13),fr3 */
L8c61d6da: sa_motion_publish_stop(ctx,0x8c61d6dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d6dc; /* fsub fr5,fr3 */
L8c61d6dc: sa_motion_publish_stop(ctx,0x8c61d6dcu); fcopy(f,4u,2u);goto L8c61d6de; /* fmov fr2,fr4 */
L8c61d6de: sa_motion_publish_stop(ctx,0x8c61d6deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d6e0; /* fmul fr3,fr1 */
L8c61d6e0: sa_motion_publish_stop(ctx,0x8c61d6e0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d6e2; /* fldi0 fr3 */
L8c61d6e2: sa_motion_publish_stop(ctx,0x8c61d6e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d6e4; /* fsub fr1,fr4 */
L8c61d6e4: sa_motion_publish_stop(ctx,0x8c61d6e4u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d6e6; /* fcmp/gt fr3,fr4 */
L8c61d6e6: sa_motion_publish_stop(ctx,0x8c61d6e6u); if(!(*sr&1u))goto L8c61d6ec;goto L8c61d6e8; /* bf 0x8c61d6ec */
L8c61d6e8: sa_motion_publish_stop(ctx,0x8c61d6eau); r[11]+=16u;goto L8c61d8a6; /* bra 0x8c61d8a6 */
L8c61d6ec: sa_motion_publish_stop(ctx,0x8c61d6ecu); r[8]+=28u;goto L8c61d6ee; /* add #0x1c,r8 */
L8c61d6ee: sa_motion_publish_stop(ctx,0x8c61d6eeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d6f0; /* fmov.s fr14,@-r8 */
L8c61d6f0: sa_motion_publish_stop(ctx,0x8c61d6f0u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d6f2; /* mov.w @(0x2,r11),r0 */
L8c61d6f2: sa_motion_publish_stop(ctx,0x8c61d6f2u); r[3]=r[0];goto L8c61d6f4; /* mov r0,r3 */
L8c61d6f4: sa_motion_publish_stop(ctx,0x8c61d6f4u); *fpul=r[3];goto L8c61d6f6; /* lds r3,FPUL */
L8c61d6f6: sa_motion_publish_stop(ctx,0x8c61d6f6u); r[0]=24u;goto L8c61d6f8; /* mov #0x18,r0 */
L8c61d6f8: sa_motion_publish_stop(ctx,0x8c61d6f8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d6fa; /* float FPUL,fr3 */
L8c61d6fa: sa_motion_publish_stop(ctx,0x8c61d6fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d6fc; /* fmul fr15,fr3 */
L8c61d6fc: sa_motion_publish_stop(ctx,0x8c61d6fcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d6fe; /* fmov.s fr3,@-r8 */
L8c61d6fe: sa_motion_publish_stop(ctx,0x8c61d6feu); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d700; /* mov.w @r11,r3 */
L8c61d700: sa_motion_publish_stop(ctx,0x8c61d700u); r[11]+=4u;goto L8c61d702; /* add #0x4,r11 */
L8c61d702: sa_motion_publish_stop(ctx,0x8c61d702u); *fpul=r[3];goto L8c61d704; /* lds r3,FPUL */
L8c61d704: sa_motion_publish_stop(ctx,0x8c61d704u); r[3]=rd(m,bus,&io_status,0x8c61d7c8u,4);if(io_status)return io_status;goto L8c61d706; /* mov.l 0x8c61d7c8,r3 */
L8c61d706: sa_motion_publish_stop(ctx,0x8c61d706u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d708; /* float FPUL,fr3 */
L8c61d708: sa_motion_publish_stop(ctx,0x8c61d708u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d70a; /* fmul fr15,fr3 */
L8c61d70a: sa_motion_publish_stop(ctx,0x8c61d70au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d70c; /* fmov.s fr3,@-r8 */
L8c61d70c: sa_motion_publish_stop(ctx,0x8c61d70cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d70e; /* fmov.s @(r0,r4),fr3 */
L8c61d70e: sa_motion_publish_stop(ctx,0x8c61d70eu); r[0]=20u;goto L8c61d710; /* mov #0x14,r0 */
L8c61d710: sa_motion_publish_stop(ctx,0x8c61d710u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d712; /* fmov.s fr3,@-r8 */
L8c61d712: sa_motion_publish_stop(ctx,0x8c61d712u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d714; /* fmov.s @(r0,r4),fr3 */
L8c61d714: sa_motion_publish_stop(ctx,0x8c61d714u); r[0]=16u;goto L8c61d716; /* mov #0x10,r0 */
L8c61d716: sa_motion_publish_stop(ctx,0x8c61d716u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d718; /* fmov.s fr3,@-r8 */
L8c61d718: sa_motion_publish_stop(ctx,0x8c61d718u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d71a; /* fmov.s @(r0,r4),fr3 */
L8c61d71a: sa_motion_publish_stop(ctx,0x8c61d71au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d71c; /* fmov.s fr3,@-r8 */
L8c61d71c: sa_motion_publish_stop(ctx,0x8c61d71cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d71e; /* mov.l r3,@-r8 */
L8c61d71e: sa_motion_publish_stop(ctx,0x8c61d71eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d720; /* pref @r8 */
L8c61d720: sa_motion_publish_stop(ctx,0x8c61d720u); r[8]+=60u;goto L8c61d722; /* add #0x3c,r8 */
L8c61d722: sa_motion_publish_stop(ctx,0x8c61d722u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d724; /* fmov.s fr14,@-r8 */
L8c61d724: sa_motion_publish_stop(ctx,0x8c61d724u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d726; /* mov.w @(0x2,r11),r0 */
L8c61d726: sa_motion_publish_stop(ctx,0x8c61d726u); r[3]=r[0];goto L8c61d728; /* mov r0,r3 */
L8c61d728: sa_motion_publish_stop(ctx,0x8c61d728u); *fpul=r[3];goto L8c61d72a; /* lds r3,FPUL */
L8c61d72a: sa_motion_publish_stop(ctx,0x8c61d72au); r[0]=24u;goto L8c61d72c; /* mov #0x18,r0 */
L8c61d72c: sa_motion_publish_stop(ctx,0x8c61d72cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d72e; /* float FPUL,fr3 */
L8c61d72e: sa_motion_publish_stop(ctx,0x8c61d72eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d730; /* fmul fr15,fr3 */
L8c61d730: sa_motion_publish_stop(ctx,0x8c61d730u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d732; /* fmov.s fr3,@-r8 */
L8c61d732: sa_motion_publish_stop(ctx,0x8c61d732u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d734; /* mov.w @r11,r3 */
L8c61d734: sa_motion_publish_stop(ctx,0x8c61d734u); r[11]+=4u;goto L8c61d736; /* add #0x4,r11 */
L8c61d736: sa_motion_publish_stop(ctx,0x8c61d736u); *fpul=r[3];goto L8c61d738; /* lds r3,FPUL */
L8c61d738: sa_motion_publish_stop(ctx,0x8c61d738u); r[3]=rd(m,bus,&io_status,0x8c61d7c8u,4);if(io_status)return io_status;goto L8c61d73a; /* mov.l 0x8c61d7c8,r3 */
L8c61d73a: sa_motion_publish_stop(ctx,0x8c61d73au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d73c; /* float FPUL,fr3 */
L8c61d73c: sa_motion_publish_stop(ctx,0x8c61d73cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d73e; /* fmul fr15,fr3 */
L8c61d73e: sa_motion_publish_stop(ctx,0x8c61d73eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d740; /* fmov.s fr3,@-r8 */
L8c61d740: sa_motion_publish_stop(ctx,0x8c61d740u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d742; /* fmov.s @(r0,r14),fr3 */
L8c61d742: sa_motion_publish_stop(ctx,0x8c61d742u); r[0]=20u;goto L8c61d744; /* mov #0x14,r0 */
L8c61d744: sa_motion_publish_stop(ctx,0x8c61d744u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d746; /* fmov.s fr3,@-r8 */
L8c61d746: sa_motion_publish_stop(ctx,0x8c61d746u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d748; /* fmov.s @(r0,r14),fr3 */
L8c61d748: sa_motion_publish_stop(ctx,0x8c61d748u); r[0]=16u;goto L8c61d74a; /* mov #0x10,r0 */
L8c61d74a: sa_motion_publish_stop(ctx,0x8c61d74au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d74c; /* fmov.s fr3,@-r8 */
L8c61d74c: sa_motion_publish_stop(ctx,0x8c61d74cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d74e; /* fmov.s @(r0,r14),fr3 */
L8c61d74e: sa_motion_publish_stop(ctx,0x8c61d74eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d750; /* fmov.s fr3,@-r8 */
L8c61d750: sa_motion_publish_stop(ctx,0x8c61d750u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d752; /* mov.l r3,@-r8 */
L8c61d752: sa_motion_publish_stop(ctx,0x8c61d752u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d754; /* pref @r8 */
L8c61d754: sa_motion_publish_stop(ctx,0x8c61d754u); r[8]+=60u;goto L8c61d756; /* add #0x3c,r8 */
L8c61d756: sa_motion_publish_stop(ctx,0x8c61d756u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d758; /* fmov.s fr14,@-r8 */
L8c61d758: sa_motion_publish_stop(ctx,0x8c61d758u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d75a; /* mov.w @(0x2,r11),r0 */
L8c61d75a: sa_motion_publish_stop(ctx,0x8c61d75au); r[3]=r[0];goto L8c61d75c; /* mov r0,r3 */
L8c61d75c: sa_motion_publish_stop(ctx,0x8c61d75cu); *fpul=r[3];goto L8c61d75e; /* lds r3,FPUL */
L8c61d75e: sa_motion_publish_stop(ctx,0x8c61d75eu); r[0]=24u;goto L8c61d760; /* mov #0x18,r0 */
L8c61d760: sa_motion_publish_stop(ctx,0x8c61d760u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d762; /* float FPUL,fr3 */
L8c61d762: sa_motion_publish_stop(ctx,0x8c61d762u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d764; /* fmul fr15,fr3 */
L8c61d764: sa_motion_publish_stop(ctx,0x8c61d764u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d766; /* fmov.s fr3,@-r8 */
L8c61d766: sa_motion_publish_stop(ctx,0x8c61d766u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d768; /* mov.w @r11,r3 */
L8c61d768: sa_motion_publish_stop(ctx,0x8c61d768u); r[11]+=4u;goto L8c61d76a; /* add #0x4,r11 */
L8c61d76a: sa_motion_publish_stop(ctx,0x8c61d76au); *fpul=r[3];goto L8c61d76c; /* lds r3,FPUL */
L8c61d76c: sa_motion_publish_stop(ctx,0x8c61d76cu); r[3]=rd(m,bus,&io_status,0x8c61d7c8u,4);if(io_status)return io_status;goto L8c61d76e; /* mov.l 0x8c61d7c8,r3 */
L8c61d76e: sa_motion_publish_stop(ctx,0x8c61d76eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d770; /* float FPUL,fr3 */
L8c61d770: sa_motion_publish_stop(ctx,0x8c61d770u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d772; /* fmul fr15,fr3 */
L8c61d772: sa_motion_publish_stop(ctx,0x8c61d772u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d774; /* fmov.s fr3,@-r8 */
L8c61d774: sa_motion_publish_stop(ctx,0x8c61d774u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61d776; /* fmov.s @(r0,r13),fr3 */
L8c61d776: sa_motion_publish_stop(ctx,0x8c61d776u); r[0]=20u;goto L8c61d778; /* mov #0x14,r0 */
L8c61d778: sa_motion_publish_stop(ctx,0x8c61d778u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d77a; /* fmov.s fr3,@-r8 */
L8c61d77a: sa_motion_publish_stop(ctx,0x8c61d77au); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61d77c; /* fmov.s @(r0,r13),fr3 */
L8c61d77c: sa_motion_publish_stop(ctx,0x8c61d77cu); r[0]=16u;goto L8c61d77e; /* mov #0x10,r0 */
L8c61d77e: sa_motion_publish_stop(ctx,0x8c61d77eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d780; /* fmov.s fr3,@-r8 */
L8c61d780: sa_motion_publish_stop(ctx,0x8c61d780u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61d782; /* fmov.s @(r0,r13),fr3 */
L8c61d782: sa_motion_publish_stop(ctx,0x8c61d782u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d784; /* fmov.s fr3,@-r8 */
L8c61d784: sa_motion_publish_stop(ctx,0x8c61d784u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d786; /* mov.l r3,@-r8 */
L8c61d786: sa_motion_publish_stop(ctx,0x8c61d786u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d788; /* pref @r8 */
L8c61d788: sa_motion_publish_stop(ctx,0x8c61d788u); r[8]+=60u;goto L8c61d78a; /* add #0x3c,r8 */
L8c61d78a: sa_motion_publish_stop(ctx,0x8c61d78au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d78c; /* fmov.s fr14,@-r8 */
L8c61d78c: sa_motion_publish_stop(ctx,0x8c61d78cu); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d78e; /* mov.w @(0x2,r11),r0 */
L8c61d78e: sa_motion_publish_stop(ctx,0x8c61d78eu); r[3]=r[0];goto L8c61d790; /* mov r0,r3 */
L8c61d790: sa_motion_publish_stop(ctx,0x8c61d790u); *fpul=r[3];goto L8c61d792; /* lds r3,FPUL */
L8c61d792: sa_motion_publish_stop(ctx,0x8c61d792u); r[0]=24u;goto L8c61d794; /* mov #0x18,r0 */
L8c61d794: sa_motion_publish_stop(ctx,0x8c61d794u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d796; /* float FPUL,fr3 */
L8c61d796: sa_motion_publish_stop(ctx,0x8c61d796u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d798; /* fmul fr15,fr3 */
L8c61d798: sa_motion_publish_stop(ctx,0x8c61d798u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d79a; /* fmov.s fr3,@-r8 */
L8c61d79a: sa_motion_publish_stop(ctx,0x8c61d79au); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d79c; /* mov.w @r11,r3 */
L8c61d79c: sa_motion_publish_stop(ctx,0x8c61d79cu); r[11]+=4u;goto L8c61d79e; /* add #0x4,r11 */
L8c61d79e: sa_motion_publish_stop(ctx,0x8c61d79eu); *fpul=r[3];goto L8c61d7a0; /* lds r3,FPUL */
L8c61d7a0: sa_motion_publish_stop(ctx,0x8c61d7a0u); r[3]=rd(m,bus,&io_status,0x8c61d7ccu,4);if(io_status)return io_status;goto L8c61d7a2; /* mov.l 0x8c61d7cc,r3 */
L8c61d7a2: sa_motion_publish_stop(ctx,0x8c61d7a2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d7a4; /* float FPUL,fr3 */
L8c61d7a4: sa_motion_publish_stop(ctx,0x8c61d7a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d7a6; /* fmul fr15,fr3 */
L8c61d7a6: sa_motion_publish_stop(ctx,0x8c61d7a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d7a8; /* fmov.s fr3,@-r8 */
L8c61d7a8: sa_motion_publish_stop(ctx,0x8c61d7a8u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61d7aa; /* fmov.s @(r0,r12),fr3 */
L8c61d7aa: sa_motion_publish_stop(ctx,0x8c61d7aau); r[0]=20u;goto L8c61d7ac; /* mov #0x14,r0 */
L8c61d7ac: sa_motion_publish_stop(ctx,0x8c61d7acu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d7ae; /* fmov.s fr3,@-r8 */
L8c61d7ae: sa_motion_publish_stop(ctx,0x8c61d7aeu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61d7b0; /* fmov.s @(r0,r12),fr3 */
L8c61d7b0: sa_motion_publish_stop(ctx,0x8c61d7b0u); r[0]=16u;goto L8c61d7b2; /* mov #0x10,r0 */
L8c61d7b2: sa_motion_publish_stop(ctx,0x8c61d7b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d7b4; /* fmov.s fr3,@-r8 */
L8c61d7b4: sa_motion_publish_stop(ctx,0x8c61d7b4u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61d7b6; /* fmov.s @(r0,r12),fr3 */
L8c61d7b6: sa_motion_publish_stop(ctx,0x8c61d7b6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d7b8; /* fmov.s fr3,@-r8 */
L8c61d7b8: sa_motion_publish_stop(ctx,0x8c61d7b8u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d7ba; /* mov.l r3,@-r8 */
L8c61d7ba: sa_motion_publish_stop(ctx,0x8c61d7bau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d7bc; /* pref @r8 */
L8c61d7bc: sa_motion_publish_stop(ctx,0x8c61d7bcu); r[2]=rd(m,bus,&io_status,0x8c61d7d0u,4);if(io_status)return io_status;goto L8c61d7be; /* mov.l 0x8c61d7d0,r2 */
L8c61d7be: sa_motion_publish_stop(ctx,0x8c61d7beu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d7c0; /* mov.l @r2,r3 */
L8c61d7c0: sa_motion_publish_stop(ctx,0x8c61d7c0u); r[3]+=4u;goto L8c61d7c2; /* add #0x4,r3 */
L8c61d7c2: sa_motion_publish_stop(ctx,0x8c61d7c2u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61d7c4; /* mov.l r3,@r2 */
L8c61d7c4: sa_motion_publish_stop(ctx,0x8c61d7c6u); r[8]+=32u;goto L8c61d8a6; /* bra 0x8c61d8a6 */
L8c61d7f0: sa_motion_publish_stop(ctx,0x8c61d7f0u); r[1]=rd(m,bus,&io_status,0x8c61da2cu,4);if(io_status)return io_status;goto L8c61d7f2; /* mov.l 0x8c61da2c,r1 */
L8c61d7f2: sa_motion_publish_stop(ctx,0x8c61d7f2u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61d7f4; /* mov.l r2,@r1 */
L8c61d7f4: sa_motion_publish_stop(ctx,0x8c61d7f4u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d7f6; /* mov.w @r11,r3 */
L8c61d7f6: sa_motion_publish_stop(ctx,0x8c61d7f6u); *fpul=r[3];goto L8c61d7f8; /* lds r3,FPUL */
L8c61d7f8: sa_motion_publish_stop(ctx,0x8c61d7f8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d7fa; /* float FPUL,fr3 */
L8c61d7fa: sa_motion_publish_stop(ctx,0x8c61d7fau); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d7fc; /* ftrc fr3,FPUL */
L8c61d7fc: sa_motion_publish_stop(ctx,0x8c61d7fcu); r[2]=*fpul;goto L8c61d7fe; /* sts FPUL,r2 */
L8c61d7fe: sa_motion_publish_stop(ctx,0x8c61d7feu); wr(m,bus,&io_status,r[9],r[2],2);if(io_status)return io_status;goto L8c61d800; /* mov.w r2,@r9 */
L8c61d800: sa_motion_publish_stop(ctx,0x8c61d800u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d802; /* mov.w @(0x2,r11),r0 */
L8c61d802: sa_motion_publish_stop(ctx,0x8c61d802u); r[11]+=4u;goto L8c61d804; /* add #0x4,r11 */
L8c61d804: sa_motion_publish_stop(ctx,0x8c61d804u); r[3]=r[0];goto L8c61d806; /* mov r0,r3 */
L8c61d806: sa_motion_publish_stop(ctx,0x8c61d806u); *fpul=r[3];goto L8c61d808; /* lds r3,FPUL */
L8c61d808: sa_motion_publish_stop(ctx,0x8c61d808u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d80a; /* float FPUL,fr3 */
L8c61d80a: sa_motion_publish_stop(ctx,0x8c61d80au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d80c; /* ftrc fr3,FPUL */
L8c61d80c: sa_motion_publish_stop(ctx,0x8c61d80cu); r[0]=*fpul;goto L8c61d80e; /* sts FPUL,r0 */
L8c61d80e: sa_motion_publish_stop(ctx,0x8c61d80eu); wr(m,bus,&io_status,r[9]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d810; /* mov.w r0,@(0x2,r9) */
L8c61d810: sa_motion_publish_stop(ctx,0x8c61d810u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d812; /* mov.w @r11,r2 */
L8c61d812: sa_motion_publish_stop(ctx,0x8c61d812u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61d814; /* mov.l @r15,r3 */
L8c61d814: sa_motion_publish_stop(ctx,0x8c61d814u); *fpul=r[2];goto L8c61d816; /* lds r2,FPUL */
L8c61d816: sa_motion_publish_stop(ctx,0x8c61d816u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d818; /* float FPUL,fr3 */
L8c61d818: sa_motion_publish_stop(ctx,0x8c61d818u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d81a; /* ftrc fr3,FPUL */
L8c61d81a: sa_motion_publish_stop(ctx,0x8c61d81au); r[1]=*fpul;goto L8c61d81c; /* sts FPUL,r1 */
L8c61d81c: sa_motion_publish_stop(ctx,0x8c61d81cu); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61d81e; /* mov.w r1,@r3 */
L8c61d81e: sa_motion_publish_stop(ctx,0x8c61d81eu); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d820; /* mov.w @(0x2,r11),r0 */
L8c61d820: sa_motion_publish_stop(ctx,0x8c61d820u); r[11]+=4u;goto L8c61d822; /* add #0x4,r11 */
L8c61d822: sa_motion_publish_stop(ctx,0x8c61d822u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61d824; /* mov.l @r15,r3 */
L8c61d824: sa_motion_publish_stop(ctx,0x8c61d824u); r[2]=r[0];goto L8c61d826; /* mov r0,r2 */
L8c61d826: sa_motion_publish_stop(ctx,0x8c61d826u); *fpul=r[2];goto L8c61d828; /* lds r2,FPUL */
L8c61d828: sa_motion_publish_stop(ctx,0x8c61d828u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d82a; /* float FPUL,fr3 */
L8c61d82a: sa_motion_publish_stop(ctx,0x8c61d82au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d82c; /* ftrc fr3,FPUL */
L8c61d82c: sa_motion_publish_stop(ctx,0x8c61d82cu); r[0]=*fpul;goto L8c61d82e; /* sts FPUL,r0 */
L8c61d82e: sa_motion_publish_stop(ctx,0x8c61d82eu); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d830; /* mov.w r0,@(0x2,r3) */
L8c61d830: sa_motion_publish_stop(ctx,0x8c61d830u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d832; /* mov.w @r11,r2 */
L8c61d832: sa_motion_publish_stop(ctx,0x8c61d832u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61d834; /* mov.l @(0x10,r15),r3 */
L8c61d834: sa_motion_publish_stop(ctx,0x8c61d834u); *fpul=r[2];goto L8c61d836; /* lds r2,FPUL */
L8c61d836: sa_motion_publish_stop(ctx,0x8c61d836u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d838; /* float FPUL,fr3 */
L8c61d838: sa_motion_publish_stop(ctx,0x8c61d838u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d83a; /* ftrc fr3,FPUL */
L8c61d83a: sa_motion_publish_stop(ctx,0x8c61d83au); r[1]=*fpul;goto L8c61d83c; /* sts FPUL,r1 */
L8c61d83c: sa_motion_publish_stop(ctx,0x8c61d83cu); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61d83e; /* mov.w r1,@r3 */
L8c61d83e: sa_motion_publish_stop(ctx,0x8c61d83eu); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d840; /* mov.w @(0x2,r11),r0 */
L8c61d840: sa_motion_publish_stop(ctx,0x8c61d840u); r[11]+=4u;goto L8c61d842; /* add #0x4,r11 */
L8c61d842: sa_motion_publish_stop(ctx,0x8c61d842u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61d844; /* mov.l @(0x10,r15),r3 */
L8c61d844: sa_motion_publish_stop(ctx,0x8c61d844u); r[2]=r[0];goto L8c61d846; /* mov r0,r2 */
L8c61d846: sa_motion_publish_stop(ctx,0x8c61d846u); *fpul=r[2];goto L8c61d848; /* lds r2,FPUL */
L8c61d848: sa_motion_publish_stop(ctx,0x8c61d848u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d84a; /* float FPUL,fr3 */
L8c61d84a: sa_motion_publish_stop(ctx,0x8c61d84au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d84c; /* ftrc fr3,FPUL */
L8c61d84c: sa_motion_publish_stop(ctx,0x8c61d84cu); r[0]=*fpul;goto L8c61d84e; /* sts FPUL,r0 */
L8c61d84e: sa_motion_publish_stop(ctx,0x8c61d84eu); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d850; /* mov.w r0,@(0x2,r3) */
L8c61d850: sa_motion_publish_stop(ctx,0x8c61d850u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d852; /* mov.w @r11,r2 */
L8c61d852: sa_motion_publish_stop(ctx,0x8c61d852u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61d854; /* mov.l @(0x4,r15),r3 */
L8c61d854: sa_motion_publish_stop(ctx,0x8c61d854u); *fpul=r[2];goto L8c61d856; /* lds r2,FPUL */
L8c61d856: sa_motion_publish_stop(ctx,0x8c61d856u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d858; /* float FPUL,fr3 */
L8c61d858: sa_motion_publish_stop(ctx,0x8c61d858u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d85a; /* ftrc fr3,FPUL */
L8c61d85a: sa_motion_publish_stop(ctx,0x8c61d85au); r[1]=*fpul;goto L8c61d85c; /* sts FPUL,r1 */
L8c61d85c: sa_motion_publish_stop(ctx,0x8c61d85cu); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61d85e; /* mov.w r1,@r3 */
L8c61d85e: sa_motion_publish_stop(ctx,0x8c61d85eu); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d860; /* mov.w @(0x2,r11),r0 */
L8c61d860: sa_motion_publish_stop(ctx,0x8c61d860u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61d862; /* mov.l @(0x4,r15),r3 */
L8c61d862: sa_motion_publish_stop(ctx,0x8c61d862u); r[2]=r[0];goto L8c61d864; /* mov r0,r2 */
L8c61d864: sa_motion_publish_stop(ctx,0x8c61d864u); *fpul=r[2];goto L8c61d866; /* lds r2,FPUL */
L8c61d866: sa_motion_publish_stop(ctx,0x8c61d866u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d868; /* float FPUL,fr3 */
L8c61d868: sa_motion_publish_stop(ctx,0x8c61d868u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d86a; /* ftrc fr3,FPUL */
L8c61d86a: sa_motion_publish_stop(ctx,0x8c61d86au); r[0]=*fpul;goto L8c61d86c; /* sts FPUL,r0 */
L8c61d86c: sa_motion_publish_stop(ctx,0x8c61d86cu); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d86e; /* mov.w r0,@(0x2,r3) */
L8c61d86e: sa_motion_publish_stop(ctx,0x8c61d86eu); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61d870; /* mov.l @(0xc,r14),r3 */
L8c61d870: sa_motion_publish_stop(ctx,0x8c61d870u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61d872; /* mov.l @(0xc,r4),r5 */
L8c61d872: sa_motion_publish_stop(ctx,0x8c61d872u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61d874; /* mov.l @(0xc,r13),r2 */
L8c61d874: sa_motion_publish_stop(ctx,0x8c61d874u); r[5]&=r[3];goto L8c61d876; /* and r3,r5 */
L8c61d876: sa_motion_publish_stop(ctx,0x8c61d876u); r[3]=1u;goto L8c61d878; /* mov #0x1,r3 */
L8c61d878: sa_motion_publish_stop(ctx,0x8c61d878u); r[5]&=r[2];goto L8c61d87a; /* and r2,r5 */
L8c61d87a: sa_motion_publish_stop(ctx,0x8c61d87au); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61d87c; /* tst r3,r5 */
L8c61d87c: sa_motion_publish_stop(ctx,0x8c61d87eu); take=!(*sr&1u);r[11]+=4u;if(take)goto L8c61d88a;goto L8c61d880; /* bf/s 0x8c61d88a */
L8c61d880: sa_motion_publish_stop(ctx,0x8c61d880u); r[1]=rd(m,bus,&io_status,0x8c61da30u,4);if(io_status)return io_status;goto L8c61d882; /* mov.l 0x8c61da30,r1 */
L8c61d882: sa_motion_publish_stop(ctx,0x8c61d882u); r[6]=r[13];goto L8c61d884; /* mov r13,r6 */
L8c61d884: sa_motion_publish_stop(ctx,0x8c61d884u); r[7]=r[9];goto L8c61d886; /* mov r9,r7 */
L8c61d886: sa_motion_publish_stop(ctx,0x8c61d888u); target=r[1];*pr=0x8c61d88au;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61d88a; /* jsr @r1 */
L8c61d88a: sa_motion_publish_stop(ctx,0x8c61d88au); r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61d88c; /* mov.l @(0xc,r14),r4 */
L8c61d88c: sa_motion_publish_stop(ctx,0x8c61d88cu); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61d88e; /* mov.l @(0xc,r13),r3 */
L8c61d88e: sa_motion_publish_stop(ctx,0x8c61d88eu); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61d890; /* mov.l @(0xc,r12),r2 */
L8c61d890: sa_motion_publish_stop(ctx,0x8c61d890u); r[4]&=r[3];goto L8c61d892; /* and r3,r4 */
L8c61d892: sa_motion_publish_stop(ctx,0x8c61d892u); r[3]=1u;goto L8c61d894; /* mov #0x1,r3 */
L8c61d894: sa_motion_publish_stop(ctx,0x8c61d894u); r[4]&=r[2];goto L8c61d896; /* and r2,r4 */
L8c61d896: sa_motion_publish_stop(ctx,0x8c61d896u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61d898; /* tst r3,r4 */
L8c61d898: sa_motion_publish_stop(ctx,0x8c61d898u); if(!(*sr&1u))goto L8c61d8a6;goto L8c61d89a; /* bf 0x8c61d8a6 */
L8c61d89a: sa_motion_publish_stop(ctx,0x8c61d89au); r[2]=rd(m,bus,&io_status,0x8c61da30u,4);if(io_status)return io_status;goto L8c61d89c; /* mov.l 0x8c61da30,r2 */
L8c61d89c: sa_motion_publish_stop(ctx,0x8c61d89cu); r[5]=r[13];goto L8c61d89e; /* mov r13,r5 */
L8c61d89e: sa_motion_publish_stop(ctx,0x8c61d89eu); r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61d8a0; /* mov.l @r15,r7 */
L8c61d8a0: sa_motion_publish_stop(ctx,0x8c61d8a0u); r[6]=r[12];goto L8c61d8a2; /* mov r12,r6 */
L8c61d8a2: sa_motion_publish_stop(ctx,0x8c61d8a4u); target=r[2];*pr=0x8c61d8a6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61d8a6; /* jsr @r2 */
L8c61d8a6: sa_motion_publish_stop(ctx,0x8c61d8a6u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61d8a8; /* mov.l @(0xc,r15),r3 */
L8c61d8a8: sa_motion_publish_stop(ctx,0x8c61d8a8u); r[3]+=1u;goto L8c61d8aa; /* add #0x1,r3 */
L8c61d8aa: sa_motion_publish_stop(ctx,0x8c61d8aau); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c61d8ac; /* mov.l r3,@(0xc,r15) */
L8c61d8ac: sa_motion_publish_stop(ctx,0x8c61d8acu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61d8ae; /* mov.l @(0x8,r15),r2 */
L8c61d8ae: sa_motion_publish_stop(ctx,0x8c61d8aeu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61d8b0; /* mov.l @(0xc,r15),r3 */
L8c61d8b0: sa_motion_publish_stop(ctx,0x8c61d8b0u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61d8b2; /* mov.w @(0x2,r2),r0 */
L8c61d8b2: sa_motion_publish_stop(ctx,0x8c61d8b2u); r[0]=r[0]&65535u;goto L8c61d8b4; /* extu.w r0,r0 */
L8c61d8b4: sa_motion_publish_stop(ctx,0x8c61d8b4u); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61d8b6; /* cmp/ge r0,r3 */
L8c61d8b6: sa_motion_publish_stop(ctx,0x8c61d8b6u); if((*sr&1u))goto L8c61d8bc;goto L8c61d8b8; /* bt 0x8c61d8bc */
L8c61d8b8: sa_motion_publish_stop(ctx,0x8c61d8bau); goto L8c61d65c; /* bra 0x8c61d65c */
L8c61d8bc: sa_motion_publish_stop(ctx,0x8c61d8beu); goto L8c61db7c; /* bra 0x8c61db7c */
L8c61d8c0: sa_motion_publish_stop(ctx,0x8c61d8c0u); r[4]=rd(m,bus,&io_status,0x8c61da34u,4);if(io_status)return io_status;goto L8c61d8c2; /* mov.l 0x8c61da34,r4 */
L8c61d8c2: sa_motion_publish_stop(ctx,0x8c61d8c2u); r[2]=0u;goto L8c61d8c4; /* mov #0x0,r2 */
L8c61d8c4: sa_motion_publish_stop(ctx,0x8c61d8c4u); wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c61d8c6; /* mov.l r2,@(0x10,r15) */
L8c61d8c6: sa_motion_publish_stop(ctx,0x8c61d8c6u); r[2]=r[4];goto L8c61d8c8; /* mov r4,r2 */
L8c61d8c8: sa_motion_publish_stop(ctx,0x8c61d8c8u); r[1]=r[4];goto L8c61d8ca; /* mov r4,r1 */
L8c61d8ca: sa_motion_publish_stop(ctx,0x8c61d8cau); r[2]+=8u;goto L8c61d8cc; /* add #0x8,r2 */
L8c61d8cc: sa_motion_publish_stop(ctx,0x8c61d8ccu); r[4]+=4u;goto L8c61d8ce; /* add #0x4,r4 */
L8c61d8ce: sa_motion_publish_stop(ctx,0x8c61d8ceu); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61d8d0; /* mov.l r2,@(0x4,r15) */
L8c61d8d0: sa_motion_publish_stop(ctx,0x8c61d8d0u); wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c61d8d2; /* mov.l r1,@(0xc,r15) */
L8c61d8d2: sa_motion_publish_stop(ctx,0x8c61d8d4u); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61db22; /* bra 0x8c61db22 */
L8c61d8d6: sa_motion_publish_stop(ctx,0x8c61d8d6u); r[3]=rd(m,bus,&io_status,0x8c61da38u,4);if(io_status)return io_status;goto L8c61d8d8; /* mov.l 0x8c61da38,r3 */
L8c61d8d8: sa_motion_publish_stop(ctx,0x8c61d8d8u); r[5]=0u;goto L8c61d8da; /* mov #0x0,r5 */
L8c61d8da: sa_motion_publish_stop(ctx,0x8c61d8dau); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d8dc; /* mov.w @r10+,r13 */
L8c61d8dc: sa_motion_publish_stop(ctx,0x8c61d8dcu); r[6]=0u;goto L8c61d8de; /* mov #0x0,r6 */
L8c61d8de: sa_motion_publish_stop(ctx,0x8c61d8deu); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61d8e0; /* mov.l @r3,r2 */
L8c61d8e0: sa_motion_publish_stop(ctx,0x8c61d8e0u); r[7]=r[5];goto L8c61d8e2; /* mov r5,r7 */
L8c61d8e2: sa_motion_publish_stop(ctx,0x8c61d8e2u); r[13]=r[13]&65535u;goto L8c61d8e4; /* extu.w r13,r13 */
L8c61d8e4: sa_motion_publish_stop(ctx,0x8c61d8e4u); r[2]+=r[13];goto L8c61d8e6; /* add r13,r2 */
L8c61d8e6: sa_motion_publish_stop(ctx,0x8c61d8e6u); *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c61d8e8; /* cmp/pl r13 */
L8c61d8e8: sa_motion_publish_stop(ctx,0x8c61d8e8u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61d8ea; /* mov.l r2,@r3 */
L8c61d8ea: sa_motion_publish_stop(ctx,0x8c61d8eau); r[4]=4294967295u;goto L8c61d8ec; /* mov #-0x1,r4 */
L8c61d8ec: sa_motion_publish_stop(ctx,0x8c61d8eeu); take=!(*sr&1u);r[6]+=r[10];if(take)goto L8c61d90e;goto L8c61d8f0; /* bf/s 0x8c61d90e */
L8c61d8f0: sa_motion_publish_stop(ctx,0x8c61d8f0u); r[14]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c61d8f2; /* mov.w @r6+,r14 */
L8c61d8f2: sa_motion_publish_stop(ctx,0x8c61d8f2u); r[7]+=1u;goto L8c61d8f4; /* add #0x1,r7 */
L8c61d8f4: sa_motion_publish_stop(ctx,0x8c61d8f4u); r[2]=rd(m,bus,&io_status,0x8c61da3cu,4);if(io_status)return io_status;goto L8c61d8f6; /* mov.l 0x8c61da3c,r2 */
L8c61d8f6: sa_motion_publish_stop(ctx,0x8c61d8f6u); r[14]=r[14]&65535u;goto L8c61d8f8; /* extu.w r14,r14 */
L8c61d8f8: sa_motion_publish_stop(ctx,0x8c61d8f8u); r[14]<<=2;goto L8c61d8fa; /* shll2 r14 */
L8c61d8fa: sa_motion_publish_stop(ctx,0x8c61d8fau); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d8fc; /* mov.l @r2,r3 */
L8c61d8fc: sa_motion_publish_stop(ctx,0x8c61d8fcu); r[14]<<=2;goto L8c61d8fe; /* shll2 r14 */
L8c61d8fe: sa_motion_publish_stop(ctx,0x8c61d8feu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61d900; /* shll r14 */
L8c61d900: sa_motion_publish_stop(ctx,0x8c61d900u); *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[13]);goto L8c61d902; /* cmp/ge r13,r7 */
L8c61d902: sa_motion_publish_stop(ctx,0x8c61d902u); r[14]+=r[3];goto L8c61d904; /* add r3,r14 */
L8c61d904: sa_motion_publish_stop(ctx,0x8c61d904u); wr(m,bus,&io_status,r[15]+0x14u,r[14],4);if(io_status)return io_status;goto L8c61d906; /* mov.l r14,@(0x14,r15) */
L8c61d906: sa_motion_publish_stop(ctx,0x8c61d906u); r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61d908; /* mov.l @(0xc,r14),r14 */
L8c61d908: sa_motion_publish_stop(ctx,0x8c61d908u); r[4]&=r[14];goto L8c61d90a; /* and r14,r4 */
L8c61d90a: sa_motion_publish_stop(ctx,0x8c61d90cu); take=!(*sr&1u);r[5]|=r[14];if(take)goto L8c61d8f0;goto L8c61d90e; /* bf/s 0x8c61d8f0 */
L8c61d90e: sa_motion_publish_stop(ctx,0x8c61d90eu); r[2]=63u;goto L8c61d910; /* mov #0x3f,r2 */
L8c61d910: sa_motion_publish_stop(ctx,0x8c61d910u); r[14]=r[13];goto L8c61d912; /* mov r13,r14 */
L8c61d912: sa_motion_publish_stop(ctx,0x8c61d912u); r[7]=r[13];goto L8c61d914; /* mov r13,r7 */
L8c61d914: sa_motion_publish_stop(ctx,0x8c61d914u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61d916; /* shll r14 */
L8c61d916: sa_motion_publish_stop(ctx,0x8c61d916u); *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61d918; /* tst r2,r4 */
L8c61d918: sa_motion_publish_stop(ctx,0x8c61d91au); take=(*sr&1u);r[7]<<=2;if(take)goto L8c61d922;goto L8c61d91c; /* bt/s 0x8c61d922 */
L8c61d91c: sa_motion_publish_stop(ctx,0x8c61d91cu); r[11]+=r[7];goto L8c61d91e; /* add r7,r11 */
L8c61d91e: sa_motion_publish_stop(ctx,0x8c61d920u); r[10]+=r[14];goto L8c61db1c; /* bra 0x8c61db1c */
L8c61d922: sa_motion_publish_stop(ctx,0x8c61d922u); r[3]=1u;goto L8c61d924; /* mov #0x1,r3 */
L8c61d924: sa_motion_publish_stop(ctx,0x8c61d924u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61d926; /* tst r3,r5 */
L8c61d926: sa_motion_publish_stop(ctx,0x8c61d926u); if((*sr&1u))goto L8c61d92c;goto L8c61d928; /* bt 0x8c61d92c */
L8c61d928: sa_motion_publish_stop(ctx,0x8c61d92au); goto L8c61da4c; /* bra 0x8c61da4c */
L8c61d92c: sa_motion_publish_stop(ctx,0x8c61d92cu); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61d92e; /* mov.w @(0x2,r10),r0 */
L8c61d92e: sa_motion_publish_stop(ctx,0x8c61d92eu); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61d930; /* mov.w @r10,r4 */
L8c61d930: sa_motion_publish_stop(ctx,0x8c61d930u); r[2]=rd(m,bus,&io_status,0x8c61da3cu,4);if(io_status)return io_status;goto L8c61d932; /* mov.l 0x8c61da3c,r2 */
L8c61d932: sa_motion_publish_stop(ctx,0x8c61d932u); r[5]=r[0]&65535u;goto L8c61d934; /* extu.w r0,r5 */
L8c61d934: sa_motion_publish_stop(ctx,0x8c61d934u); r[5]<<=2;goto L8c61d936; /* shll2 r5 */
L8c61d936: sa_motion_publish_stop(ctx,0x8c61d936u); r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61d938; /* mov.w @(0x4,r10),r0 */
L8c61d938: sa_motion_publish_stop(ctx,0x8c61d938u); r[4]=r[4]&65535u;goto L8c61d93a; /* extu.w r4,r4 */
L8c61d93a: sa_motion_publish_stop(ctx,0x8c61d93au); r[12]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d93c; /* mov.l @r2,r12 */
L8c61d93c: sa_motion_publish_stop(ctx,0x8c61d93cu); r[4]<<=2;goto L8c61d93e; /* shll2 r4 */
L8c61d93e: sa_motion_publish_stop(ctx,0x8c61d93eu); r[5]<<=2;goto L8c61d940; /* shll2 r5 */
L8c61d940: sa_motion_publish_stop(ctx,0x8c61d940u); r[6]=r[0]&65535u;goto L8c61d942; /* extu.w r0,r6 */
L8c61d942: sa_motion_publish_stop(ctx,0x8c61d942u); r[4]<<=2;goto L8c61d944; /* shll2 r4 */
L8c61d944: sa_motion_publish_stop(ctx,0x8c61d944u); r[0]=16u;goto L8c61d946; /* mov #0x10,r0 */
L8c61d946: sa_motion_publish_stop(ctx,0x8c61d946u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61d948; /* shll r4 */
L8c61d948: sa_motion_publish_stop(ctx,0x8c61d948u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d94a; /* shll r5 */
L8c61d94a: sa_motion_publish_stop(ctx,0x8c61d94au); r[5]+=r[12];goto L8c61d94c; /* add r12,r5 */
L8c61d94c: sa_motion_publish_stop(ctx,0x8c61d94cu); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d94e; /* fmov.s @(r0,r5),fr5 */
L8c61d94e: sa_motion_publish_stop(ctx,0x8c61d94eu); r[6]<<=2;goto L8c61d950; /* shll2 r6 */
L8c61d950: sa_motion_publish_stop(ctx,0x8c61d950u); r[4]+=r[12];goto L8c61d952; /* add r12,r4 */
L8c61d952: sa_motion_publish_stop(ctx,0x8c61d952u); r[6]<<=2;goto L8c61d954; /* shll2 r6 */
L8c61d954: sa_motion_publish_stop(ctx,0x8c61d954u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d956; /* fmov.s @(r0,r4),fr3 */
L8c61d956: sa_motion_publish_stop(ctx,0x8c61d956u); fcopy(f,2u,5u);goto L8c61d958; /* fmov fr5,fr2 */
L8c61d958: sa_motion_publish_stop(ctx,0x8c61d958u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61d95a; /* shll r6 */
L8c61d95a: sa_motion_publish_stop(ctx,0x8c61d95au); r[0]=20u;goto L8c61d95c; /* mov #0x14,r0 */
L8c61d95c: sa_motion_publish_stop(ctx,0x8c61d95cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d95e; /* fsub fr3,fr2 */
L8c61d95e: sa_motion_publish_stop(ctx,0x8c61d95eu); r[6]+=r[12];goto L8c61d960; /* add r12,r6 */
L8c61d960: sa_motion_publish_stop(ctx,0x8c61d960u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d962; /* fmov.s @(r0,r5),fr4 */
L8c61d962: sa_motion_publish_stop(ctx,0x8c61d962u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d964; /* fmov.s @(r0,r6),fr3 */
L8c61d964: sa_motion_publish_stop(ctx,0x8c61d964u); fcopy(f,1u,4u);goto L8c61d966; /* fmov fr4,fr1 */
L8c61d966: sa_motion_publish_stop(ctx,0x8c61d966u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d968; /* fsub fr4,fr3 */
L8c61d968: sa_motion_publish_stop(ctx,0x8c61d968u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d96a; /* fmul fr3,fr2 */
L8c61d96a: sa_motion_publish_stop(ctx,0x8c61d96au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d96c; /* fmov.s @(r0,r4),fr3 */
L8c61d96c: sa_motion_publish_stop(ctx,0x8c61d96cu); r[0]=16u;goto L8c61d96e; /* mov #0x10,r0 */
L8c61d96e: sa_motion_publish_stop(ctx,0x8c61d96eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d970; /* fsub fr3,fr1 */
L8c61d970: sa_motion_publish_stop(ctx,0x8c61d970u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d972; /* fmov.s @(r0,r6),fr3 */
L8c61d972: sa_motion_publish_stop(ctx,0x8c61d972u); fcopy(f,4u,2u);goto L8c61d974; /* fmov fr2,fr4 */
L8c61d974: sa_motion_publish_stop(ctx,0x8c61d974u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d976; /* fsub fr5,fr3 */
L8c61d976: sa_motion_publish_stop(ctx,0x8c61d976u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d978; /* fmul fr3,fr1 */
L8c61d978: sa_motion_publish_stop(ctx,0x8c61d978u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d97a; /* fldi0 fr3 */
L8c61d97a: sa_motion_publish_stop(ctx,0x8c61d97au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d97c; /* fsub fr1,fr4 */
L8c61d97c: sa_motion_publish_stop(ctx,0x8c61d97cu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d97e; /* fcmp/gt fr3,fr4 */
L8c61d97e: sa_motion_publish_stop(ctx,0x8c61d97eu); if(!(*sr&1u))goto L8c61d986;goto L8c61d980; /* bf 0x8c61d986 */
L8c61d980: sa_motion_publish_stop(ctx,0x8c61d980u); r[10]+=r[14];goto L8c61d982; /* add r14,r10 */
L8c61d982: sa_motion_publish_stop(ctx,0x8c61d984u); r[11]+=r[7];goto L8c61db1c; /* bra 0x8c61db1c */
L8c61d986: sa_motion_publish_stop(ctx,0x8c61d986u); r[3]=rd(m,bus,&io_status,0x8c61da40u,4);if(io_status)return io_status;goto L8c61d988; /* mov.l 0x8c61da40,r3 */
L8c61d988: sa_motion_publish_stop(ctx,0x8c61d988u); r[6]=r[13];goto L8c61d98a; /* mov r13,r6 */
L8c61d98a: sa_motion_publish_stop(ctx,0x8c61d98au); r[6]+=4294967295u;goto L8c61d98c; /* add #-0x1,r6 */
L8c61d98c: sa_motion_publish_stop(ctx,0x8c61d98cu); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61d98e; /* mov.l @r3,r2 */
L8c61d98e: sa_motion_publish_stop(ctx,0x8c61d98eu); *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c61d990; /* cmp/pl r6 */
L8c61d990: sa_motion_publish_stop(ctx,0x8c61d990u); r[2]+=r[13];goto L8c61d992; /* add r13,r2 */
L8c61d992: sa_motion_publish_stop(ctx,0x8c61d992u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61d994; /* mov.l r2,@r3 */
L8c61d994: sa_motion_publish_stop(ctx,0x8c61d996u); take=!(*sr&1u);r[5]=0u;if(take)goto L8c61d9e4;goto L8c61d998; /* bf/s 0x8c61d9e4 */
L8c61d998: sa_motion_publish_stop(ctx,0x8c61d998u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d99a; /* mov.w @r10+,r4 */
L8c61d99a: sa_motion_publish_stop(ctx,0x8c61d99au); r[8]+=28u;goto L8c61d99c; /* add #0x1c,r8 */
L8c61d99c: sa_motion_publish_stop(ctx,0x8c61d99cu); r[2]=rd(m,bus,&io_status,0x8c61da3cu,4);if(io_status)return io_status;goto L8c61d99e; /* mov.l 0x8c61da3c,r2 */
L8c61d99e: sa_motion_publish_stop(ctx,0x8c61d99eu); r[4]=r[4]&65535u;goto L8c61d9a0; /* extu.w r4,r4 */
L8c61d9a0: sa_motion_publish_stop(ctx,0x8c61d9a0u); r[4]<<=2;goto L8c61d9a2; /* shll2 r4 */
L8c61d9a2: sa_motion_publish_stop(ctx,0x8c61d9a2u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d9a4; /* mov.l @r2,r3 */
L8c61d9a4: sa_motion_publish_stop(ctx,0x8c61d9a4u); r[4]<<=2;goto L8c61d9a6; /* shll2 r4 */
L8c61d9a6: sa_motion_publish_stop(ctx,0x8c61d9a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d9a8; /* fmov.s fr14,@-r8 */
L8c61d9a8: sa_motion_publish_stop(ctx,0x8c61d9a8u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d9aa; /* mov.w @(0x2,r11),r0 */
L8c61d9aa: sa_motion_publish_stop(ctx,0x8c61d9aau); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61d9ac; /* shll r4 */
L8c61d9ac: sa_motion_publish_stop(ctx,0x8c61d9acu); r[4]+=r[3];goto L8c61d9ae; /* add r3,r4 */
L8c61d9ae: sa_motion_publish_stop(ctx,0x8c61d9aeu); r[3]=r[0];goto L8c61d9b0; /* mov r0,r3 */
L8c61d9b0: sa_motion_publish_stop(ctx,0x8c61d9b0u); *fpul=r[3];goto L8c61d9b2; /* lds r3,FPUL */
L8c61d9b2: sa_motion_publish_stop(ctx,0x8c61d9b2u); r[0]=24u;goto L8c61d9b4; /* mov #0x18,r0 */
L8c61d9b4: sa_motion_publish_stop(ctx,0x8c61d9b4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d9b6; /* float FPUL,fr3 */
L8c61d9b6: sa_motion_publish_stop(ctx,0x8c61d9b6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d9b8; /* fmul fr15,fr3 */
L8c61d9b8: sa_motion_publish_stop(ctx,0x8c61d9b8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d9ba; /* fmov.s fr3,@-r8 */
L8c61d9ba: sa_motion_publish_stop(ctx,0x8c61d9bau); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d9bc; /* mov.w @r11,r3 */
L8c61d9bc: sa_motion_publish_stop(ctx,0x8c61d9bcu); r[11]+=4u;goto L8c61d9be; /* add #0x4,r11 */
L8c61d9be: sa_motion_publish_stop(ctx,0x8c61d9beu); *fpul=r[3];goto L8c61d9c0; /* lds r3,FPUL */
L8c61d9c0: sa_motion_publish_stop(ctx,0x8c61d9c0u); r[3]=rd(m,bus,&io_status,0x8c61da44u,4);if(io_status)return io_status;goto L8c61d9c2; /* mov.l 0x8c61da44,r3 */
L8c61d9c2: sa_motion_publish_stop(ctx,0x8c61d9c2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d9c4; /* float FPUL,fr3 */
L8c61d9c4: sa_motion_publish_stop(ctx,0x8c61d9c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d9c6; /* fmul fr15,fr3 */
L8c61d9c6: sa_motion_publish_stop(ctx,0x8c61d9c6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d9c8; /* fmov.s fr3,@-r8 */
L8c61d9c8: sa_motion_publish_stop(ctx,0x8c61d9c8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d9ca; /* fmov.s @(r0,r4),fr3 */
L8c61d9ca: sa_motion_publish_stop(ctx,0x8c61d9cau); r[0]=20u;goto L8c61d9cc; /* mov #0x14,r0 */
L8c61d9cc: sa_motion_publish_stop(ctx,0x8c61d9ccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d9ce; /* fmov.s fr3,@-r8 */
L8c61d9ce: sa_motion_publish_stop(ctx,0x8c61d9ceu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d9d0; /* fmov.s @(r0,r4),fr3 */
L8c61d9d0: sa_motion_publish_stop(ctx,0x8c61d9d0u); r[0]=16u;goto L8c61d9d2; /* mov #0x10,r0 */
L8c61d9d2: sa_motion_publish_stop(ctx,0x8c61d9d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d9d4; /* fmov.s fr3,@-r8 */
L8c61d9d4: sa_motion_publish_stop(ctx,0x8c61d9d4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d9d6; /* fmov.s @(r0,r4),fr3 */
L8c61d9d6: sa_motion_publish_stop(ctx,0x8c61d9d6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d9d8; /* fmov.s fr3,@-r8 */
L8c61d9d8: sa_motion_publish_stop(ctx,0x8c61d9d8u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d9da; /* mov.l r3,@-r8 */
L8c61d9da: sa_motion_publish_stop(ctx,0x8c61d9dau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d9dc; /* pref @r8 */
L8c61d9dc: sa_motion_publish_stop(ctx,0x8c61d9dcu); r[5]+=1u;goto L8c61d9de; /* add #0x1,r5 */
L8c61d9de: sa_motion_publish_stop(ctx,0x8c61d9deu); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61d9e0; /* cmp/ge r6,r5 */
L8c61d9e0: sa_motion_publish_stop(ctx,0x8c61d9e2u); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61d998;goto L8c61d9e4; /* bf/s 0x8c61d998 */
L8c61d9e4: sa_motion_publish_stop(ctx,0x8c61d9e4u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d9e6; /* mov.w @r10+,r4 */
L8c61d9e6: sa_motion_publish_stop(ctx,0x8c61d9e6u); r[8]+=28u;goto L8c61d9e8; /* add #0x1c,r8 */
L8c61d9e8: sa_motion_publish_stop(ctx,0x8c61d9e8u); r[2]=rd(m,bus,&io_status,0x8c61da3cu,4);if(io_status)return io_status;goto L8c61d9ea; /* mov.l 0x8c61da3c,r2 */
L8c61d9ea: sa_motion_publish_stop(ctx,0x8c61d9eau); r[4]=r[4]&65535u;goto L8c61d9ec; /* extu.w r4,r4 */
L8c61d9ec: sa_motion_publish_stop(ctx,0x8c61d9ecu); r[4]<<=2;goto L8c61d9ee; /* shll2 r4 */
L8c61d9ee: sa_motion_publish_stop(ctx,0x8c61d9eeu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d9f0; /* mov.l @r2,r3 */
L8c61d9f0: sa_motion_publish_stop(ctx,0x8c61d9f0u); r[4]<<=2;goto L8c61d9f2; /* shll2 r4 */
L8c61d9f2: sa_motion_publish_stop(ctx,0x8c61d9f2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d9f4; /* fmov.s fr14,@-r8 */
L8c61d9f4: sa_motion_publish_stop(ctx,0x8c61d9f4u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d9f6; /* mov.w @(0x2,r11),r0 */
L8c61d9f6: sa_motion_publish_stop(ctx,0x8c61d9f6u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61d9f8; /* shll r4 */
L8c61d9f8: sa_motion_publish_stop(ctx,0x8c61d9f8u); r[4]+=r[3];goto L8c61d9fa; /* add r3,r4 */
L8c61d9fa: sa_motion_publish_stop(ctx,0x8c61d9fau); r[3]=r[0];goto L8c61d9fc; /* mov r0,r3 */
L8c61d9fc: sa_motion_publish_stop(ctx,0x8c61d9fcu); *fpul=r[3];goto L8c61d9fe; /* lds r3,FPUL */
L8c61d9fe: sa_motion_publish_stop(ctx,0x8c61d9feu); r[0]=24u;goto L8c61da00; /* mov #0x18,r0 */
L8c61da00: sa_motion_publish_stop(ctx,0x8c61da00u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61da02; /* float FPUL,fr3 */
L8c61da02: sa_motion_publish_stop(ctx,0x8c61da02u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61da04; /* fmul fr15,fr3 */
L8c61da04: sa_motion_publish_stop(ctx,0x8c61da04u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61da06; /* fmov.s fr3,@-r8 */
L8c61da06: sa_motion_publish_stop(ctx,0x8c61da06u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61da08; /* mov.w @r11,r3 */
L8c61da08: sa_motion_publish_stop(ctx,0x8c61da08u); r[11]+=4u;goto L8c61da0a; /* add #0x4,r11 */
L8c61da0a: sa_motion_publish_stop(ctx,0x8c61da0au); *fpul=r[3];goto L8c61da0c; /* lds r3,FPUL */
L8c61da0c: sa_motion_publish_stop(ctx,0x8c61da0cu); r[3]=rd(m,bus,&io_status,0x8c61da48u,4);if(io_status)return io_status;goto L8c61da0e; /* mov.l 0x8c61da48,r3 */
L8c61da0e: sa_motion_publish_stop(ctx,0x8c61da0eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61da10; /* float FPUL,fr3 */
L8c61da10: sa_motion_publish_stop(ctx,0x8c61da10u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61da12; /* fmul fr15,fr3 */
L8c61da12: sa_motion_publish_stop(ctx,0x8c61da12u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61da14; /* fmov.s fr3,@-r8 */
L8c61da14: sa_motion_publish_stop(ctx,0x8c61da14u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61da16; /* fmov.s @(r0,r4),fr3 */
L8c61da16: sa_motion_publish_stop(ctx,0x8c61da16u); r[0]=20u;goto L8c61da18; /* mov #0x14,r0 */
L8c61da18: sa_motion_publish_stop(ctx,0x8c61da18u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61da1a; /* fmov.s fr3,@-r8 */
L8c61da1a: sa_motion_publish_stop(ctx,0x8c61da1au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61da1c; /* fmov.s @(r0,r4),fr3 */
L8c61da1c: sa_motion_publish_stop(ctx,0x8c61da1cu); r[0]=16u;goto L8c61da1e; /* mov #0x10,r0 */
L8c61da1e: sa_motion_publish_stop(ctx,0x8c61da1eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61da20; /* fmov.s fr3,@-r8 */
L8c61da20: sa_motion_publish_stop(ctx,0x8c61da20u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61da22; /* fmov.s @(r0,r4),fr3 */
L8c61da22: sa_motion_publish_stop(ctx,0x8c61da22u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61da24; /* fmov.s fr3,@-r8 */
L8c61da24: sa_motion_publish_stop(ctx,0x8c61da24u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61da26; /* mov.l r3,@-r8 */
L8c61da26: sa_motion_publish_stop(ctx,0x8c61da26u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61da28; /* pref @r8 */
L8c61da28: sa_motion_publish_stop(ctx,0x8c61da2au); r[8]+=32u;goto L8c61db1c; /* bra 0x8c61db1c */
L8c61da4c: sa_motion_publish_stop(ctx,0x8c61da4cu); r[1]=rd(m,bus,&io_status,0x8c61db3cu,4);if(io_status)return io_status;goto L8c61da4e; /* mov.l 0x8c61db3c,r1 */
L8c61da4e: sa_motion_publish_stop(ctx,0x8c61da4eu); r[2]=0u;goto L8c61da50; /* mov #0x0,r2 */
L8c61da50: sa_motion_publish_stop(ctx,0x8c61da50u); r[0]=rd(m,bus,&io_status,0x8c61db40u,4);if(io_status)return io_status;goto L8c61da52; /* mov.l 0x8c61db40,r0 */
L8c61da52: sa_motion_publish_stop(ctx,0x8c61da52u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61da54; /* mov.l r2,@r1 */
L8c61da54: sa_motion_publish_stop(ctx,0x8c61da54u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61da56; /* mov.w @r10+,r4 */
L8c61da56: sa_motion_publish_stop(ctx,0x8c61da56u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61da58; /* mov.l @r0,r2 */
L8c61da58: sa_motion_publish_stop(ctx,0x8c61da58u); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61da5a; /* mov.w @r10+,r14 */
L8c61da5a: sa_motion_publish_stop(ctx,0x8c61da5au); r[4]=r[4]&65535u;goto L8c61da5c; /* extu.w r4,r4 */
L8c61da5c: sa_motion_publish_stop(ctx,0x8c61da5cu); r[4]<<=2;goto L8c61da5e; /* shll2 r4 */
L8c61da5e: sa_motion_publish_stop(ctx,0x8c61da5eu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61da60; /* mov.l @(0xc,r15),r3 */
L8c61da60: sa_motion_publish_stop(ctx,0x8c61da60u); r[14]=r[14]&65535u;goto L8c61da62; /* extu.w r14,r14 */
L8c61da62: sa_motion_publish_stop(ctx,0x8c61da62u); r[14]<<=2;goto L8c61da64; /* shll2 r14 */
L8c61da64: sa_motion_publish_stop(ctx,0x8c61da64u); r[14]<<=2;goto L8c61da66; /* shll2 r14 */
L8c61da66: sa_motion_publish_stop(ctx,0x8c61da66u); r[4]<<=2;goto L8c61da68; /* shll2 r4 */
L8c61da68: sa_motion_publish_stop(ctx,0x8c61da68u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61da6a; /* shll r4 */
L8c61da6a: sa_motion_publish_stop(ctx,0x8c61da6au); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61da6c; /* shll r14 */
L8c61da6c: sa_motion_publish_stop(ctx,0x8c61da6cu); r[14]+=r[2];goto L8c61da6e; /* add r2,r14 */
L8c61da6e: sa_motion_publish_stop(ctx,0x8c61da6eu); r[4]+=r[2];goto L8c61da70; /* add r2,r4 */
L8c61da70: sa_motion_publish_stop(ctx,0x8c61da70u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61da72; /* mov.w @r11,r2 */
L8c61da72: sa_motion_publish_stop(ctx,0x8c61da72u); *fpul=r[2];goto L8c61da74; /* lds r2,FPUL */
L8c61da74: sa_motion_publish_stop(ctx,0x8c61da74u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61da76; /* float FPUL,fr3 */
L8c61da76: sa_motion_publish_stop(ctx,0x8c61da76u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61da78; /* ftrc fr3,FPUL */
L8c61da78: sa_motion_publish_stop(ctx,0x8c61da78u); r[1]=*fpul;goto L8c61da7a; /* sts FPUL,r1 */
L8c61da7a: sa_motion_publish_stop(ctx,0x8c61da7au); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61da7c; /* mov.w r1,@r3 */
L8c61da7c: sa_motion_publish_stop(ctx,0x8c61da7cu); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61da7e; /* mov.w @(0x2,r11),r0 */
L8c61da7e: sa_motion_publish_stop(ctx,0x8c61da7eu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61da80; /* mov.l @(0xc,r15),r3 */
L8c61da80: sa_motion_publish_stop(ctx,0x8c61da80u); r[2]=r[0];goto L8c61da82; /* mov r0,r2 */
L8c61da82: sa_motion_publish_stop(ctx,0x8c61da82u); *fpul=r[2];goto L8c61da84; /* lds r2,FPUL */
L8c61da84: sa_motion_publish_stop(ctx,0x8c61da84u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61da86; /* float FPUL,fr3 */
L8c61da86: sa_motion_publish_stop(ctx,0x8c61da86u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61da88; /* ftrc fr3,FPUL */
L8c61da88: sa_motion_publish_stop(ctx,0x8c61da88u); r[0]=*fpul;goto L8c61da8a; /* sts FPUL,r0 */
L8c61da8a: sa_motion_publish_stop(ctx,0x8c61da8au); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61da8c; /* mov.w r0,@(0x2,r3) */
L8c61da8c: sa_motion_publish_stop(ctx,0x8c61da8cu); r[11]+=4u;goto L8c61da8e; /* add #0x4,r11 */
L8c61da8e: sa_motion_publish_stop(ctx,0x8c61da8eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61da90; /* mov.l @r15,r3 */
L8c61da90: sa_motion_publish_stop(ctx,0x8c61da90u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61da92; /* mov.w @r11,r2 */
L8c61da92: sa_motion_publish_stop(ctx,0x8c61da92u); r[9]=r[13];goto L8c61da94; /* mov r13,r9 */
L8c61da94: sa_motion_publish_stop(ctx,0x8c61da94u); r[9]+=4294967294u;goto L8c61da96; /* add #-0x2,r9 */
L8c61da96: sa_motion_publish_stop(ctx,0x8c61da96u); *fpul=r[2];goto L8c61da98; /* lds r2,FPUL */
L8c61da98: sa_motion_publish_stop(ctx,0x8c61da98u); *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c61da9a; /* cmp/pl r9 */
L8c61da9a: sa_motion_publish_stop(ctx,0x8c61da9au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61da9c; /* float FPUL,fr3 */
L8c61da9c: sa_motion_publish_stop(ctx,0x8c61da9cu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61da9e; /* ftrc fr3,FPUL */
L8c61da9e: sa_motion_publish_stop(ctx,0x8c61da9eu); r[1]=*fpul;goto L8c61daa0; /* sts FPUL,r1 */
L8c61daa0: sa_motion_publish_stop(ctx,0x8c61daa0u); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61daa2; /* mov.w r1,@r3 */
L8c61daa2: sa_motion_publish_stop(ctx,0x8c61daa2u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61daa4; /* mov.w @(0x2,r11),r0 */
L8c61daa4: sa_motion_publish_stop(ctx,0x8c61daa4u); r[11]+=4u;goto L8c61daa6; /* add #0x4,r11 */
L8c61daa6: sa_motion_publish_stop(ctx,0x8c61daa6u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61daa8; /* mov.l @r15,r3 */
L8c61daa8: sa_motion_publish_stop(ctx,0x8c61daa8u); r[2]=r[0];goto L8c61daaa; /* mov r0,r2 */
L8c61daaa: sa_motion_publish_stop(ctx,0x8c61daaau); *fpul=r[2];goto L8c61daac; /* lds r2,FPUL */
L8c61daac: sa_motion_publish_stop(ctx,0x8c61daacu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61daae; /* float FPUL,fr3 */
L8c61daae: sa_motion_publish_stop(ctx,0x8c61daaeu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61dab0; /* ftrc fr3,FPUL */
L8c61dab0: sa_motion_publish_stop(ctx,0x8c61dab0u); r[0]=*fpul;goto L8c61dab2; /* sts FPUL,r0 */
L8c61dab2: sa_motion_publish_stop(ctx,0x8c61dab2u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61dab4; /* mov.w r0,@(0x2,r3) */
L8c61dab4: sa_motion_publish_stop(ctx,0x8c61dab6u); take=!(*sr&1u);r[12]=0u;if(take)goto L8c61db1c;goto L8c61dab8; /* bf/s 0x8c61db1c */
L8c61dab8: sa_motion_publish_stop(ctx,0x8c61dab8u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61daba; /* mov.w @r10+,r13 */
L8c61daba: sa_motion_publish_stop(ctx,0x8c61dabau); r[2]=rd(m,bus,&io_status,0x8c61db40u,4);if(io_status)return io_status;goto L8c61dabc; /* mov.l 0x8c61db40,r2 */
L8c61dabc: sa_motion_publish_stop(ctx,0x8c61dabcu); r[13]=r[13]&65535u;goto L8c61dabe; /* extu.w r13,r13 */
L8c61dabe: sa_motion_publish_stop(ctx,0x8c61dabeu); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61dac0; /* mov.l @(0x4,r15),r1 */
L8c61dac0: sa_motion_publish_stop(ctx,0x8c61dac0u); r[13]<<=2;goto L8c61dac2; /* shll2 r13 */
L8c61dac2: sa_motion_publish_stop(ctx,0x8c61dac2u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dac4; /* mov.l @r2,r3 */
L8c61dac4: sa_motion_publish_stop(ctx,0x8c61dac4u); r[13]<<=2;goto L8c61dac6; /* shll2 r13 */
L8c61dac6: sa_motion_publish_stop(ctx,0x8c61dac6u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61dac8; /* shll r13 */
L8c61dac8: sa_motion_publish_stop(ctx,0x8c61dac8u); r[13]+=r[3];goto L8c61daca; /* add r3,r13 */
L8c61daca: sa_motion_publish_stop(ctx,0x8c61dacau); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61dacc; /* mov.w @r11,r3 */
L8c61dacc: sa_motion_publish_stop(ctx,0x8c61daccu); *fpul=r[3];goto L8c61dace; /* lds r3,FPUL */
L8c61dace: sa_motion_publish_stop(ctx,0x8c61daceu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61dad0; /* float FPUL,fr3 */
L8c61dad0: sa_motion_publish_stop(ctx,0x8c61dad0u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61dad2; /* ftrc fr3,FPUL */
L8c61dad2: sa_motion_publish_stop(ctx,0x8c61dad2u); r[0]=*fpul;goto L8c61dad4; /* sts FPUL,r0 */
L8c61dad4: sa_motion_publish_stop(ctx,0x8c61dad4u); wr(m,bus,&io_status,r[1],r[0],2);if(io_status)return io_status;goto L8c61dad6; /* mov.w r0,@r1 */
L8c61dad6: sa_motion_publish_stop(ctx,0x8c61dad6u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61dad8; /* mov.w @(0x2,r11),r0 */
L8c61dad8: sa_motion_publish_stop(ctx,0x8c61dad8u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61dada; /* mov.l @(0x4,r15),r3 */
L8c61dada: sa_motion_publish_stop(ctx,0x8c61dadau); r[1]=r[0];goto L8c61dadc; /* mov r0,r1 */
L8c61dadc: sa_motion_publish_stop(ctx,0x8c61dadcu); *fpul=r[1];goto L8c61dade; /* lds r1,FPUL */
L8c61dade: sa_motion_publish_stop(ctx,0x8c61dadeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61dae0; /* float FPUL,fr3 */
L8c61dae0: sa_motion_publish_stop(ctx,0x8c61dae0u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61dae2; /* ftrc fr3,FPUL */
L8c61dae2: sa_motion_publish_stop(ctx,0x8c61dae2u); r[0]=*fpul;goto L8c61dae4; /* sts FPUL,r0 */
L8c61dae4: sa_motion_publish_stop(ctx,0x8c61dae4u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61dae6; /* mov.w r0,@(0x2,r3) */
L8c61dae6: sa_motion_publish_stop(ctx,0x8c61dae6u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61dae8; /* mov.l @(0xc,r4),r5 */
L8c61dae8: sa_motion_publish_stop(ctx,0x8c61dae8u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61daea; /* mov.l @(0xc,r14),r3 */
L8c61daea: sa_motion_publish_stop(ctx,0x8c61daeau); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61daec; /* mov.l @(0xc,r13),r1 */
L8c61daec: sa_motion_publish_stop(ctx,0x8c61daecu); r[5]&=r[3];goto L8c61daee; /* and r3,r5 */
L8c61daee: sa_motion_publish_stop(ctx,0x8c61daeeu); r[3]=1u;goto L8c61daf0; /* mov #0x1,r3 */
L8c61daf0: sa_motion_publish_stop(ctx,0x8c61daf0u); r[5]&=r[1];goto L8c61daf2; /* and r1,r5 */
L8c61daf2: sa_motion_publish_stop(ctx,0x8c61daf2u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61daf4; /* tst r3,r5 */
L8c61daf4: sa_motion_publish_stop(ctx,0x8c61daf6u); take=!(*sr&1u);r[11]+=4u;if(take)goto L8c61db02;goto L8c61daf8; /* bf/s 0x8c61db02 */
L8c61daf8: sa_motion_publish_stop(ctx,0x8c61daf8u); r[1]=rd(m,bus,&io_status,0x8c61db48u,4);if(io_status)return io_status;goto L8c61dafa; /* mov.l 0x8c61db48,r1 */
L8c61dafa: sa_motion_publish_stop(ctx,0x8c61dafau); r[6]=r[13];goto L8c61dafc; /* mov r13,r6 */
L8c61dafc: sa_motion_publish_stop(ctx,0x8c61dafcu); r[7]=rd(m,bus,&io_status,0x8c61db44u,4);if(io_status)return io_status;goto L8c61dafe; /* mov.l 0x8c61db44,r7 */
L8c61dafe: sa_motion_publish_stop(ctx,0x8c61db00u); target=r[1];*pr=0x8c61db02u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61db02; /* jsr @r1 */
L8c61db02: sa_motion_publish_stop(ctx,0x8c61db02u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61db04; /* mov.l @r15,r2 */
L8c61db04: sa_motion_publish_stop(ctx,0x8c61db04u); r[12]+=1u;goto L8c61db06; /* add #0x1,r12 */
L8c61db06: sa_motion_publish_stop(ctx,0x8c61db06u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61db08; /* mov.l @(0xc,r15),r3 */
L8c61db08: sa_motion_publish_stop(ctx,0x8c61db08u); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[9]);goto L8c61db0a; /* cmp/ge r9,r12 */
L8c61db0a: sa_motion_publish_stop(ctx,0x8c61db0au); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61db0c; /* mov.l @r2,r1 */
L8c61db0c: sa_motion_publish_stop(ctx,0x8c61db0cu); r[4]=r[14];goto L8c61db0e; /* mov r14,r4 */
L8c61db0e: sa_motion_publish_stop(ctx,0x8c61db0eu); wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c61db10; /* mov.l r1,@r3 */
L8c61db10: sa_motion_publish_stop(ctx,0x8c61db10u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61db12; /* mov.l @(0x4,r15),r3 */
L8c61db12: sa_motion_publish_stop(ctx,0x8c61db12u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61db14; /* mov.l @r15,r0 */
L8c61db14: sa_motion_publish_stop(ctx,0x8c61db14u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61db16; /* mov.l @r3,r2 */
L8c61db16: sa_motion_publish_stop(ctx,0x8c61db16u); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61db18; /* mov.l r2,@r0 */
L8c61db18: sa_motion_publish_stop(ctx,0x8c61db1au); take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61dab8;goto L8c61db1c; /* bf/s 0x8c61dab8 */
L8c61db1c: sa_motion_publish_stop(ctx,0x8c61db1cu); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61db1e; /* mov.l @(0x10,r15),r2 */
L8c61db1e: sa_motion_publish_stop(ctx,0x8c61db1eu); r[2]+=1u;goto L8c61db20; /* add #0x1,r2 */
L8c61db20: sa_motion_publish_stop(ctx,0x8c61db20u); wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c61db22; /* mov.l r2,@(0x10,r15) */
L8c61db22: sa_motion_publish_stop(ctx,0x8c61db22u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61db24; /* mov.l @(0x8,r15),r3 */
L8c61db24: sa_motion_publish_stop(ctx,0x8c61db24u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61db26; /* mov.w @(0x2,r3),r0 */
L8c61db26: sa_motion_publish_stop(ctx,0x8c61db26u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61db28; /* mov.l @(0x10,r15),r3 */
L8c61db28: sa_motion_publish_stop(ctx,0x8c61db28u); r[0]=r[0]&65535u;goto L8c61db2a; /* extu.w r0,r0 */
L8c61db2a: sa_motion_publish_stop(ctx,0x8c61db2au); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61db2c; /* cmp/ge r0,r3 */
L8c61db2c: sa_motion_publish_stop(ctx,0x8c61db2cu); if((*sr&1u))goto L8c61db32;goto L8c61db2e; /* bt 0x8c61db32 */
L8c61db2e: sa_motion_publish_stop(ctx,0x8c61db30u); goto L8c61d8d6; /* bra 0x8c61d8d6 */
L8c61db32: sa_motion_publish_stop(ctx,0x8c61db34u); goto L8c61db7c; /* bra 0x8c61db7c */
L8c61db4c: sa_motion_publish_stop(ctx,0x8c61db4cu); r[1]=rd(m,bus,&io_status,0x8c61db5cu,4);if(io_status)return io_status;goto L8c61db4e; /* mov.l 0x8c61db5c,r1 */
L8c61db4e: sa_motion_publish_stop(ctx,0x8c61db4eu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61db50; /* mov.l @(0x8,r15),r2 */
L8c61db50: sa_motion_publish_stop(ctx,0x8c61db50u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61db52; /* mov.l r2,@r1 */
L8c61db52: sa_motion_publish_stop(ctx,0x8c61db54u); goto L8c61db60; /* bra 0x8c61db60 */
L8c61db60: sa_motion_publish_stop(ctx,0x8c61db60u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61db62; /* frchg */
L8c61db62: sa_motion_publish_stop(ctx,0x8c61db62u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61db64; /* fmov.s fr0,@-r15 */
L8c61db64: sa_motion_publish_stop(ctx,0x8c61db64u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61db66; /* fmov.s fr1,@-r15 */
L8c61db66: sa_motion_publish_stop(ctx,0x8c61db66u); r[0]=0x8c61db94u;goto L8c61db68; /* mova 0x8c61db94,r0 */
L8c61db68: sa_motion_publish_stop(ctx,0x8c61db68u); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;goto L8c61db6a; /* fmov.s @r0,fr0 */
L8c61db6a: sa_motion_publish_stop(ctx,0x8c61db6au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61db6c; /* frchg */
L8c61db6c: sa_motion_publish_stop(ctx,0x8c61db6cu); r[2]=rd(m,bus,&io_status,0x8c61dc08u,4);if(io_status)return io_status;goto L8c61db6e; /* mov.l 0x8c61dc08,r2 */
L8c61db6e: sa_motion_publish_stop(ctx,0x8c61db6eu); r[3]=rd(m,bus,&io_status,0x8c61dc04u,4);if(io_status)return io_status;goto L8c61db70; /* mov.l 0x8c61dc04,r3 */
L8c61db70: sa_motion_publish_stop(ctx,0x8c61db72u); target=r[2];*pr=0x8c61db74u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61db74; /* jsr @r2 */
L8c61db74: sa_motion_publish_stop(ctx,0x8c61db74u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61db76; /* frchg */
L8c61db76: sa_motion_publish_stop(ctx,0x8c61db76u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61db78; /* fmov.s @r15+,fr1 */
L8c61db78: sa_motion_publish_stop(ctx,0x8c61db78u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61db7a; /* fmov.s @r15+,fr0 */
L8c61db7a: sa_motion_publish_stop(ctx,0x8c61db7au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61db7c; /* frchg */
L8c61db7c: sa_motion_publish_stop(ctx,0x8c61db7cu); r[15]+=24u;goto L8c61db7e; /* add #0x18,r15 */
L8c61db7e: sa_motion_publish_stop(ctx,0x8c61db7eu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db80; /* lds.l @r15+,PR */
L8c61db80: sa_motion_publish_stop(ctx,0x8c61db80u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61db82; /* fmov.s @r15+,fr14 */
L8c61db82: sa_motion_publish_stop(ctx,0x8c61db82u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61db84; /* fmov.s @r15+,fr15 */
L8c61db84: sa_motion_publish_stop(ctx,0x8c61db84u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db86; /* mov.l @r15+,r9 */
L8c61db86: sa_motion_publish_stop(ctx,0x8c61db86u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db88; /* mov.l @r15+,r10 */
L8c61db88: sa_motion_publish_stop(ctx,0x8c61db88u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db8a; /* mov.l @r15+,r11 */
L8c61db8a: sa_motion_publish_stop(ctx,0x8c61db8au); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db8c; /* mov.l @r15+,r12 */
L8c61db8c: sa_motion_publish_stop(ctx,0x8c61db8cu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db8e; /* mov.l @r15+,r13 */
L8c61db8e: sa_motion_publish_stop(ctx,0x8c61db90u); if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
