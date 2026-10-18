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
L8c61cf9c: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61cf9e; /* mov.l r14,@-r15 */
L8c61cf9e: r[0]=0x8c61d058u;goto L8c61cfa0; /* mova 0x8c61d058,r0 */
L8c61cfa0: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61cfa2; /* mov.l r13,@-r15 */
L8c61cfa2: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61cfa4; /* mov.l r12,@-r15 */
L8c61cfa4: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61cfa6; /* mov.l r11,@-r15 */
L8c61cfa6: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61cfa8; /* mov.l r10,@-r15 */
L8c61cfa8: r[3]=rd(m,bus,&io_status,0x8c61d060u,4);if(io_status)return io_status;goto L8c61cfaa; /* mov.l 0x8c61d060,r3 */
L8c61cfaa: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61cfac; /* mov.l r9,@-r15 */
L8c61cfac: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61cfae; /* sts.l PR,@-r15 */
L8c61cfae: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c61cfb0; /* fmov.s @r0,fr6 */
L8c61cfb0: r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61cfb2; /* mov.w @r4,r0 */
L8c61cfb2: r[12]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c61cfb4; /* mov.l @(0x4,r4),r12 */
L8c61cfb4: r[0]=r[0]&65535u;goto L8c61cfb6; /* extu.w r0,r0 */
L8c61cfb6: r[13]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c61cfb8; /* mov.l @(0x14,r4),r13 */
L8c61cfb8: r[0]&=r[3];goto L8c61cfba; /* and r3,r0 */
L8c61cfba: r[10]=rd(m,bus,&io_status,0x8c61d05cu,4);if(io_status)return io_status;goto L8c61cfbc; /* mov.l 0x8c61d05c,r10 */
L8c61cfbc: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61cfbe; /* cmp/eq #0x0,r0 */
L8c61cfbe: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c61cfc0; /* fldi1 fr7 */
L8c61cfc0: take=(*sr&1u);r[11]=0u;if(take)goto L8c61cfe6;goto L8c61cfc4; /* bt/s 0x8c61cfe6 */
L8c61cfc4: r[1]=sx(m,bus,&io_status,0x8c61d054u,2);if(io_status)return io_status;goto L8c61cfc6; /* mov.w 0x8c61d054,r1 */
L8c61cfc6: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61cfc8; /* cmp/eq r1,r0 */
L8c61cfc8: if(!(*sr&1u))goto L8c61cfce;goto L8c61cfca; /* bf 0x8c61cfce */
L8c61cfca: goto L8c61d122; /* bra 0x8c61d122 */
L8c61cfce: r[1]=rd(m,bus,&io_status,0x8c61d064u,4);if(io_status)return io_status;goto L8c61cfd0; /* mov.l 0x8c61d064,r1 */
L8c61cfd0: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61cfd2; /* cmp/eq r1,r0 */
L8c61cfd2: if(!(*sr&1u))goto L8c61cfd8;goto L8c61cfd4; /* bf 0x8c61cfd8 */
L8c61cfd4: goto L8c61d282; /* bra 0x8c61d282 */
L8c61cfd8: r[1]=rd(m,bus,&io_status,0x8c61d060u,4);if(io_status)return io_status;goto L8c61cfda; /* mov.l 0x8c61d060,r1 */
L8c61cfda: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61cfdc; /* cmp/eq r1,r0 */
L8c61cfdc: if(!(*sr&1u))goto L8c61cfe2;goto L8c61cfde; /* bf 0x8c61cfe2 */
L8c61cfde: goto L8c61d3cc; /* bra 0x8c61d3cc */
L8c61cfe2: goto L8c61d3f4; /* bra 0x8c61d3f4 */
L8c61cfe6: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61cfe8; /* mov.w @(0x2,r4),r0 */
L8c61cfe8: r[1]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c61cfea; /* mov.l @r10,r1 */
L8c61cfea: r[0]=r[0]&65535u;goto L8c61cfec; /* extu.w r0,r0 */
L8c61cfec: r[2]=r[0];goto L8c61cfee; /* mov r0,r2 */
L8c61cfee: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61cff0; /* shll r0 */
L8c61cff0: r[0]+=r[2];goto L8c61cff2; /* add r2,r0 */
L8c61cff2: r[1]+=r[0];goto L8c61cff4; /* add r0,r1 */
L8c61cff4: wr(m,bus,&io_status,r[10],r[1],4);if(io_status)return io_status;goto L8c61cff6; /* mov.l r1,@r10 */
L8c61cff6: r[14]=r[11];goto L8c61d112; /* bra 0x8c61d112 */
L8c61cffa: r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61cffc; /* mov.w @r12+,r5 */
L8c61cffc: r[0]=16u;goto L8c61cffe; /* mov #0x10,r0 */
L8c61cffe: r[3]=rd(m,bus,&io_status,0x8c61d068u,4);if(io_status)return io_status;goto L8c61d000; /* mov.l 0x8c61d068,r3 */
L8c61d000: r[6]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d002; /* mov.w @r12+,r6 */
L8c61d002: r[5]=r[5]&65535u;goto L8c61d004; /* extu.w r5,r5 */
L8c61d004: r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61d006; /* mov.l @r3,r11 */
L8c61d006: r[5]<<=2;goto L8c61d008; /* shll2 r5 */
L8c61d008: r[6]=r[6]&65535u;goto L8c61d00a; /* extu.w r6,r6 */
L8c61d00a: r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d00c; /* mov.w @r12+,r7 */
L8c61d00c: r[6]<<=2;goto L8c61d00e; /* shll2 r6 */
L8c61d00e: r[6]<<=2;goto L8c61d010; /* shll2 r6 */
L8c61d010: r[5]<<=2;goto L8c61d012; /* shll2 r5 */
L8c61d012: r[7]=r[7]&65535u;goto L8c61d014; /* extu.w r7,r7 */
L8c61d014: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d016; /* shll r5 */
L8c61d016: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61d018; /* shll r6 */
L8c61d018: r[6]+=r[11];goto L8c61d01a; /* add r11,r6 */
L8c61d01a: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d01c; /* fmov.s @(r0,r6),fr4 */
L8c61d01c: r[5]+=r[11];goto L8c61d01e; /* add r11,r5 */
L8c61d01e: r[7]<<=2;goto L8c61d020; /* shll2 r7 */
L8c61d020: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d022; /* fmov.s @(r0,r5),fr3 */
L8c61d022: r[7]<<=2;goto L8c61d024; /* shll2 r7 */
L8c61d024: fcopy(f,2u,4u);goto L8c61d026; /* fmov fr4,fr2 */
L8c61d026: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61d028; /* shll r7 */
L8c61d028: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d02a; /* fsub fr3,fr2 */
L8c61d02a: r[0]=20u;goto L8c61d02c; /* mov #0x14,r0 */
L8c61d02c: r[7]+=r[11];goto L8c61d02e; /* add r11,r7 */
L8c61d02e: fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d030; /* fmov.s @(r0,r6),fr5 */
L8c61d030: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d032; /* fmov.s @(r0,r7),fr3 */
L8c61d032: fcopy(f,1u,5u);goto L8c61d034; /* fmov fr5,fr1 */
L8c61d034: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d036; /* fsub fr5,fr3 */
L8c61d036: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d038; /* fmul fr3,fr2 */
L8c61d038: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d03a; /* fmov.s @(r0,r5),fr3 */
L8c61d03a: r[0]=16u;goto L8c61d03c; /* mov #0x10,r0 */
L8c61d03c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d03e; /* fsub fr3,fr1 */
L8c61d03e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d040; /* fmov.s @(r0,r7),fr3 */
L8c61d040: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d042; /* fsub fr4,fr3 */
L8c61d042: fcopy(f,4u,2u);goto L8c61d044; /* fmov fr2,fr4 */
L8c61d044: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d046; /* fmul fr3,fr1 */
L8c61d046: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d048; /* fldi0 fr3 */
L8c61d048: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d04a; /* fsub fr1,fr4 */
L8c61d04a: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d04c; /* fcmp/gt fr3,fr4 */
L8c61d04c: take=!(*sr&1u);r[14]+=1u;if(take)goto L8c61d06c;goto L8c61d050; /* bf/s 0x8c61d06c */
L8c61d050: r[13]+=12u;goto L8c61d112; /* bra 0x8c61d112 */
L8c61d06c: r[8]+=28u;goto L8c61d06e; /* add #0x1c,r8 */
L8c61d06e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d070; /* fmov.s fr7,@-r8 */
L8c61d070: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d072; /* mov.w @(0x2,r13),r0 */
L8c61d072: r[3]=r[0];goto L8c61d074; /* mov r0,r3 */
L8c61d074: *fpul=r[3];goto L8c61d076; /* lds r3,FPUL */
L8c61d076: r[0]=24u;goto L8c61d078; /* mov #0x18,r0 */
L8c61d078: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d07a; /* float FPUL,fr3 */
L8c61d07a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d07c; /* fmul fr6,fr3 */
L8c61d07c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d07e; /* fmov.s fr3,@-r8 */
L8c61d07e: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d080; /* mov.w @r13,r3 */
L8c61d080: r[13]+=4u;goto L8c61d082; /* add #0x4,r13 */
L8c61d082: *fpul=r[3];goto L8c61d084; /* lds r3,FPUL */
L8c61d084: r[3]=rd(m,bus,&io_status,0x8c61d2f0u,4);if(io_status)return io_status;goto L8c61d086; /* mov.l 0x8c61d2f0,r3 */
L8c61d086: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d088; /* float FPUL,fr3 */
L8c61d088: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d08a; /* fmul fr6,fr3 */
L8c61d08a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d08c; /* fmov.s fr3,@-r8 */
L8c61d08c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d08e; /* fmov.s @(r0,r5),fr3 */
L8c61d08e: r[0]=20u;goto L8c61d090; /* mov #0x14,r0 */
L8c61d090: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d092; /* fmov.s fr3,@-r8 */
L8c61d092: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d094; /* fmov.s @(r0,r5),fr3 */
L8c61d094: r[0]=16u;goto L8c61d096; /* mov #0x10,r0 */
L8c61d096: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d098; /* fmov.s fr3,@-r8 */
L8c61d098: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d09a; /* fmov.s @(r0,r5),fr3 */
L8c61d09a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d09c; /* fmov.s fr3,@-r8 */
L8c61d09c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d09e; /* mov.l r3,@-r8 */
L8c61d09e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d0a0; /* pref @r8 */
L8c61d0a0: r[8]+=60u;goto L8c61d0a2; /* add #0x3c,r8 */
L8c61d0a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d0a4; /* fmov.s fr7,@-r8 */
L8c61d0a4: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d0a6; /* mov.w @(0x2,r13),r0 */
L8c61d0a6: r[3]=r[0];goto L8c61d0a8; /* mov r0,r3 */
L8c61d0a8: *fpul=r[3];goto L8c61d0aa; /* lds r3,FPUL */
L8c61d0aa: r[0]=24u;goto L8c61d0ac; /* mov #0x18,r0 */
L8c61d0ac: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d0ae; /* float FPUL,fr3 */
L8c61d0ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d0b0; /* fmul fr6,fr3 */
L8c61d0b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0b2; /* fmov.s fr3,@-r8 */
L8c61d0b2: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d0b4; /* mov.w @r13,r3 */
L8c61d0b4: r[13]+=4u;goto L8c61d0b6; /* add #0x4,r13 */
L8c61d0b6: *fpul=r[3];goto L8c61d0b8; /* lds r3,FPUL */
L8c61d0b8: r[3]=rd(m,bus,&io_status,0x8c61d2f0u,4);if(io_status)return io_status;goto L8c61d0ba; /* mov.l 0x8c61d2f0,r3 */
L8c61d0ba: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d0bc; /* float FPUL,fr3 */
L8c61d0bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d0be; /* fmul fr6,fr3 */
L8c61d0be: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0c0; /* fmov.s fr3,@-r8 */
L8c61d0c0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d0c2; /* fmov.s @(r0,r6),fr3 */
L8c61d0c2: r[0]=20u;goto L8c61d0c4; /* mov #0x14,r0 */
L8c61d0c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0c6; /* fmov.s fr3,@-r8 */
L8c61d0c6: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d0c8; /* fmov.s @(r0,r6),fr3 */
L8c61d0c8: r[0]=16u;goto L8c61d0ca; /* mov #0x10,r0 */
L8c61d0ca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0cc; /* fmov.s fr3,@-r8 */
L8c61d0cc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d0ce; /* fmov.s @(r0,r6),fr3 */
L8c61d0ce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0d0; /* fmov.s fr3,@-r8 */
L8c61d0d0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d0d2; /* mov.l r3,@-r8 */
L8c61d0d2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d0d4; /* pref @r8 */
L8c61d0d4: r[8]+=60u;goto L8c61d0d6; /* add #0x3c,r8 */
L8c61d0d6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d0d8; /* fmov.s fr7,@-r8 */
L8c61d0d8: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d0da; /* mov.w @(0x2,r13),r0 */
L8c61d0da: r[3]=r[0];goto L8c61d0dc; /* mov r0,r3 */
L8c61d0dc: *fpul=r[3];goto L8c61d0de; /* lds r3,FPUL */
L8c61d0de: r[0]=24u;goto L8c61d0e0; /* mov #0x18,r0 */
L8c61d0e0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d0e2; /* float FPUL,fr3 */
L8c61d0e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d0e4; /* fmul fr6,fr3 */
L8c61d0e4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0e6; /* fmov.s fr3,@-r8 */
L8c61d0e6: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d0e8; /* mov.w @r13,r3 */
L8c61d0e8: r[13]+=4u;goto L8c61d0ea; /* add #0x4,r13 */
L8c61d0ea: *fpul=r[3];goto L8c61d0ec; /* lds r3,FPUL */
L8c61d0ec: r[3]=rd(m,bus,&io_status,0x8c61d2f4u,4);if(io_status)return io_status;goto L8c61d0ee; /* mov.l 0x8c61d2f4,r3 */
L8c61d0ee: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d0f0; /* float FPUL,fr3 */
L8c61d0f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d0f2; /* fmul fr6,fr3 */
L8c61d0f2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0f4; /* fmov.s fr3,@-r8 */
L8c61d0f4: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d0f6; /* fmov.s @(r0,r7),fr3 */
L8c61d0f6: r[0]=20u;goto L8c61d0f8; /* mov #0x14,r0 */
L8c61d0f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d0fa; /* fmov.s fr3,@-r8 */
L8c61d0fa: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d0fc; /* fmov.s @(r0,r7),fr3 */
L8c61d0fc: r[0]=16u;goto L8c61d0fe; /* mov #0x10,r0 */
L8c61d0fe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d100; /* fmov.s fr3,@-r8 */
L8c61d100: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d102; /* fmov.s @(r0,r7),fr3 */
L8c61d102: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d104; /* fmov.s fr3,@-r8 */
L8c61d104: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d106; /* mov.l r3,@-r8 */
L8c61d106: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d108; /* pref @r8 */
L8c61d108: r[2]=rd(m,bus,&io_status,0x8c61d2f8u,4);if(io_status)return io_status;goto L8c61d10a; /* mov.l 0x8c61d2f8,r2 */
L8c61d10a: r[8]+=32u;goto L8c61d10c; /* add #0x20,r8 */
L8c61d10c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d10e; /* mov.l @r2,r3 */
L8c61d10e: r[3]+=3u;goto L8c61d110; /* add #0x3,r3 */
L8c61d110: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61d112; /* mov.l r3,@r2 */
L8c61d112: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61d114; /* mov.w @(0x2,r4),r0 */
L8c61d114: r[0]=r[0]&65535u;goto L8c61d116; /* extu.w r0,r0 */
L8c61d116: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c61d118; /* cmp/ge r0,r14 */
L8c61d118: if((*sr&1u))goto L8c61d11e;goto L8c61d11a; /* bt 0x8c61d11e */
L8c61d11a: goto L8c61cffa; /* bra 0x8c61cffa */
L8c61d11e: goto L8c61d3f4; /* bra 0x8c61d3f4 */
L8c61d122: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61d124; /* mov.w @(0x2,r4),r0 */
L8c61d124: r[2]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c61d126; /* mov.l @r10,r2 */
L8c61d126: r[0]=r[0]&65535u;goto L8c61d128; /* extu.w r0,r0 */
L8c61d128: r[0]<<=2;goto L8c61d12a; /* shll2 r0 */
L8c61d12a: r[2]+=r[0];goto L8c61d12c; /* add r0,r2 */
L8c61d12c: wr(m,bus,&io_status,r[10],r[2],4);if(io_status)return io_status;goto L8c61d12e; /* mov.l r2,@r10 */
L8c61d12e: r[10]=r[11];goto L8c61d272; /* bra 0x8c61d272 */
L8c61d132: r[6]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d134; /* mov.w @r12+,r6 */
L8c61d134: r[0]=16u;goto L8c61d136; /* mov #0x10,r0 */
L8c61d136: r[3]=rd(m,bus,&io_status,0x8c61d2fcu,4);if(io_status)return io_status;goto L8c61d138; /* mov.l 0x8c61d2fc,r3 */
L8c61d138: r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d13a; /* mov.w @r12+,r7 */
L8c61d13a: r[6]=r[6]&65535u;goto L8c61d13c; /* extu.w r6,r6 */
L8c61d13c: r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61d13e; /* mov.l @r3,r11 */
L8c61d13e: r[6]<<=2;goto L8c61d140; /* shll2 r6 */
L8c61d140: r[7]=r[7]&65535u;goto L8c61d142; /* extu.w r7,r7 */
L8c61d142: r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d144; /* mov.w @r12+,r5 */
L8c61d144: r[7]<<=2;goto L8c61d146; /* shll2 r7 */
L8c61d146: r[7]<<=2;goto L8c61d148; /* shll2 r7 */
L8c61d148: r[14]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d14a; /* mov.w @r12+,r14 */
L8c61d14a: r[6]<<=2;goto L8c61d14c; /* shll2 r6 */
L8c61d14c: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61d14e; /* shll r6 */
L8c61d14e: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61d150; /* shll r7 */
L8c61d150: r[7]+=r[11];goto L8c61d152; /* add r11,r7 */
L8c61d152: r[6]+=r[11];goto L8c61d154; /* add r11,r6 */
L8c61d154: fload(m,bus,&io_status,f,4u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d156; /* fmov.s @(r0,r7),fr4 */
L8c61d156: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d158; /* fmov.s @(r0,r6),fr3 */
L8c61d158: r[5]=r[5]&65535u;goto L8c61d15a; /* extu.w r5,r5 */
L8c61d15a: fcopy(f,2u,4u);goto L8c61d15c; /* fmov fr4,fr2 */
L8c61d15c: r[5]<<=2;goto L8c61d15e; /* shll2 r5 */
L8c61d15e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d160; /* fsub fr3,fr2 */
L8c61d160: r[14]=r[14]&65535u;goto L8c61d162; /* extu.w r14,r14 */
L8c61d162: r[5]<<=2;goto L8c61d164; /* shll2 r5 */
L8c61d164: r[14]<<=2;goto L8c61d166; /* shll2 r14 */
L8c61d166: r[0]=20u;goto L8c61d168; /* mov #0x14,r0 */
L8c61d168: fload(m,bus,&io_status,f,5u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d16a; /* fmov.s @(r0,r7),fr5 */
L8c61d16a: r[14]<<=2;goto L8c61d16c; /* shll2 r14 */
L8c61d16c: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d16e; /* shll r5 */
L8c61d16e: r[5]+=r[11];goto L8c61d170; /* add r11,r5 */
L8c61d170: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61d172; /* shll r14 */
L8c61d172: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d174; /* fmov.s @(r0,r5),fr3 */
L8c61d174: fcopy(f,1u,5u);goto L8c61d176; /* fmov fr5,fr1 */
L8c61d176: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d178; /* fsub fr5,fr3 */
L8c61d178: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d17a; /* fmul fr3,fr2 */
L8c61d17a: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d17c; /* fmov.s @(r0,r6),fr3 */
L8c61d17c: r[0]=16u;goto L8c61d17e; /* mov #0x10,r0 */
L8c61d17e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d180; /* fsub fr3,fr1 */
L8c61d180: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d182; /* fmov.s @(r0,r5),fr3 */
L8c61d182: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d184; /* fsub fr4,fr3 */
L8c61d184: fcopy(f,4u,2u);goto L8c61d186; /* fmov fr2,fr4 */
L8c61d186: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d188; /* fmul fr3,fr1 */
L8c61d188: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d18a; /* fldi0 fr3 */
L8c61d18a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d18c; /* fsub fr1,fr4 */
L8c61d18c: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d18e; /* fcmp/gt fr3,fr4 */
L8c61d18e: take=!(*sr&1u);r[14]+=r[11];if(take)goto L8c61d196;goto L8c61d192; /* bf/s 0x8c61d196 */
L8c61d192: r[13]+=16u;goto L8c61d270; /* bra 0x8c61d270 */
L8c61d196: r[8]+=28u;goto L8c61d198; /* add #0x1c,r8 */
L8c61d198: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d19a; /* fmov.s fr7,@-r8 */
L8c61d19a: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d19c; /* mov.w @(0x2,r13),r0 */
L8c61d19c: r[3]=r[0];goto L8c61d19e; /* mov r0,r3 */
L8c61d19e: *fpul=r[3];goto L8c61d1a0; /* lds r3,FPUL */
L8c61d1a0: r[0]=24u;goto L8c61d1a2; /* mov #0x18,r0 */
L8c61d1a2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d1a4; /* float FPUL,fr3 */
L8c61d1a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d1a6; /* fmul fr6,fr3 */
L8c61d1a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1a8; /* fmov.s fr3,@-r8 */
L8c61d1a8: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d1aa; /* mov.w @r13,r3 */
L8c61d1aa: r[13]+=4u;goto L8c61d1ac; /* add #0x4,r13 */
L8c61d1ac: *fpul=r[3];goto L8c61d1ae; /* lds r3,FPUL */
L8c61d1ae: r[3]=rd(m,bus,&io_status,0x8c61d2f0u,4);if(io_status)return io_status;goto L8c61d1b0; /* mov.l 0x8c61d2f0,r3 */
L8c61d1b0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d1b2; /* float FPUL,fr3 */
L8c61d1b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d1b4; /* fmul fr6,fr3 */
L8c61d1b4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1b6; /* fmov.s fr3,@-r8 */
L8c61d1b6: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d1b8; /* fmov.s @(r0,r6),fr3 */
L8c61d1b8: r[0]=20u;goto L8c61d1ba; /* mov #0x14,r0 */
L8c61d1ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1bc; /* fmov.s fr3,@-r8 */
L8c61d1bc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d1be; /* fmov.s @(r0,r6),fr3 */
L8c61d1be: r[0]=16u;goto L8c61d1c0; /* mov #0x10,r0 */
L8c61d1c0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1c2; /* fmov.s fr3,@-r8 */
L8c61d1c2: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d1c4; /* fmov.s @(r0,r6),fr3 */
L8c61d1c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1c6; /* fmov.s fr3,@-r8 */
L8c61d1c6: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d1c8; /* mov.l r3,@-r8 */
L8c61d1c8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d1ca; /* pref @r8 */
L8c61d1ca: r[8]+=60u;goto L8c61d1cc; /* add #0x3c,r8 */
L8c61d1cc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d1ce; /* fmov.s fr7,@-r8 */
L8c61d1ce: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d1d0; /* mov.w @(0x2,r13),r0 */
L8c61d1d0: r[3]=r[0];goto L8c61d1d2; /* mov r0,r3 */
L8c61d1d2: *fpul=r[3];goto L8c61d1d4; /* lds r3,FPUL */
L8c61d1d4: r[0]=24u;goto L8c61d1d6; /* mov #0x18,r0 */
L8c61d1d6: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d1d8; /* float FPUL,fr3 */
L8c61d1d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d1da; /* fmul fr6,fr3 */
L8c61d1da: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1dc; /* fmov.s fr3,@-r8 */
L8c61d1dc: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d1de; /* mov.w @r13,r3 */
L8c61d1de: r[13]+=4u;goto L8c61d1e0; /* add #0x4,r13 */
L8c61d1e0: *fpul=r[3];goto L8c61d1e2; /* lds r3,FPUL */
L8c61d1e2: r[3]=rd(m,bus,&io_status,0x8c61d2f0u,4);if(io_status)return io_status;goto L8c61d1e4; /* mov.l 0x8c61d2f0,r3 */
L8c61d1e4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d1e6; /* float FPUL,fr3 */
L8c61d1e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d1e8; /* fmul fr6,fr3 */
L8c61d1e8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1ea; /* fmov.s fr3,@-r8 */
L8c61d1ea: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d1ec; /* fmov.s @(r0,r7),fr3 */
L8c61d1ec: r[0]=20u;goto L8c61d1ee; /* mov #0x14,r0 */
L8c61d1ee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1f0; /* fmov.s fr3,@-r8 */
L8c61d1f0: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d1f2; /* fmov.s @(r0,r7),fr3 */
L8c61d1f2: r[0]=16u;goto L8c61d1f4; /* mov #0x10,r0 */
L8c61d1f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1f6; /* fmov.s fr3,@-r8 */
L8c61d1f6: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d1f8; /* fmov.s @(r0,r7),fr3 */
L8c61d1f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d1fa; /* fmov.s fr3,@-r8 */
L8c61d1fa: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d1fc; /* mov.l r3,@-r8 */
L8c61d1fc: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d1fe; /* pref @r8 */
L8c61d1fe: r[8]+=60u;goto L8c61d200; /* add #0x3c,r8 */
L8c61d200: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d202; /* fmov.s fr7,@-r8 */
L8c61d202: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d204; /* mov.w @(0x2,r13),r0 */
L8c61d204: r[3]=r[0];goto L8c61d206; /* mov r0,r3 */
L8c61d206: *fpul=r[3];goto L8c61d208; /* lds r3,FPUL */
L8c61d208: r[0]=24u;goto L8c61d20a; /* mov #0x18,r0 */
L8c61d20a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d20c; /* float FPUL,fr3 */
L8c61d20c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d20e; /* fmul fr6,fr3 */
L8c61d20e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d210; /* fmov.s fr3,@-r8 */
L8c61d210: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d212; /* mov.w @r13,r3 */
L8c61d212: r[13]+=4u;goto L8c61d214; /* add #0x4,r13 */
L8c61d214: *fpul=r[3];goto L8c61d216; /* lds r3,FPUL */
L8c61d216: r[3]=rd(m,bus,&io_status,0x8c61d2f0u,4);if(io_status)return io_status;goto L8c61d218; /* mov.l 0x8c61d2f0,r3 */
L8c61d218: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d21a; /* float FPUL,fr3 */
L8c61d21a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d21c; /* fmul fr6,fr3 */
L8c61d21c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d21e; /* fmov.s fr3,@-r8 */
L8c61d21e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d220; /* fmov.s @(r0,r5),fr3 */
L8c61d220: r[0]=20u;goto L8c61d222; /* mov #0x14,r0 */
L8c61d222: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d224; /* fmov.s fr3,@-r8 */
L8c61d224: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d226; /* fmov.s @(r0,r5),fr3 */
L8c61d226: r[0]=16u;goto L8c61d228; /* mov #0x10,r0 */
L8c61d228: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d22a; /* fmov.s fr3,@-r8 */
L8c61d22a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d22c; /* fmov.s @(r0,r5),fr3 */
L8c61d22c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d22e; /* fmov.s fr3,@-r8 */
L8c61d22e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d230; /* mov.l r3,@-r8 */
L8c61d230: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d232; /* pref @r8 */
L8c61d232: r[8]+=60u;goto L8c61d234; /* add #0x3c,r8 */
L8c61d234: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d236; /* fmov.s fr7,@-r8 */
L8c61d236: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d238; /* mov.w @(0x2,r13),r0 */
L8c61d238: r[3]=r[0];goto L8c61d23a; /* mov r0,r3 */
L8c61d23a: *fpul=r[3];goto L8c61d23c; /* lds r3,FPUL */
L8c61d23c: r[0]=24u;goto L8c61d23e; /* mov #0x18,r0 */
L8c61d23e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d240; /* float FPUL,fr3 */
L8c61d240: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d242; /* fmul fr6,fr3 */
L8c61d242: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d244; /* fmov.s fr3,@-r8 */
L8c61d244: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d246; /* mov.w @r13,r3 */
L8c61d246: r[13]+=4u;goto L8c61d248; /* add #0x4,r13 */
L8c61d248: *fpul=r[3];goto L8c61d24a; /* lds r3,FPUL */
L8c61d24a: r[3]=rd(m,bus,&io_status,0x8c61d2f4u,4);if(io_status)return io_status;goto L8c61d24c; /* mov.l 0x8c61d2f4,r3 */
L8c61d24c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d24e; /* float FPUL,fr3 */
L8c61d24e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d250; /* fmul fr6,fr3 */
L8c61d250: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d252; /* fmov.s fr3,@-r8 */
L8c61d252: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d254; /* fmov.s @(r0,r14),fr3 */
L8c61d254: r[0]=20u;goto L8c61d256; /* mov #0x14,r0 */
L8c61d256: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d258; /* fmov.s fr3,@-r8 */
L8c61d258: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d25a; /* fmov.s @(r0,r14),fr3 */
L8c61d25a: r[0]=16u;goto L8c61d25c; /* mov #0x10,r0 */
L8c61d25c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d25e; /* fmov.s fr3,@-r8 */
L8c61d25e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d260; /* fmov.s @(r0,r14),fr3 */
L8c61d260: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d262; /* fmov.s fr3,@-r8 */
L8c61d262: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d264; /* mov.l r3,@-r8 */
L8c61d264: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d266; /* pref @r8 */
L8c61d266: r[2]=rd(m,bus,&io_status,0x8c61d2f8u,4);if(io_status)return io_status;goto L8c61d268; /* mov.l 0x8c61d2f8,r2 */
L8c61d268: r[8]+=32u;goto L8c61d26a; /* add #0x20,r8 */
L8c61d26a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d26c; /* mov.l @r2,r3 */
L8c61d26c: r[3]+=4u;goto L8c61d26e; /* add #0x4,r3 */
L8c61d26e: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61d270; /* mov.l r3,@r2 */
L8c61d270: r[10]+=1u;goto L8c61d272; /* add #0x1,r10 */
L8c61d272: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61d274; /* mov.w @(0x2,r4),r0 */
L8c61d274: r[0]=r[0]&65535u;goto L8c61d276; /* extu.w r0,r0 */
L8c61d276: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61d278; /* cmp/ge r0,r10 */
L8c61d278: if((*sr&1u))goto L8c61d27e;goto L8c61d27a; /* bt 0x8c61d27e */
L8c61d27a: goto L8c61d132; /* bra 0x8c61d132 */
L8c61d27e: goto L8c61d3f4; /* bra 0x8c61d3f4 */
L8c61d282: r[9]=r[11];goto L8c61d3a6; /* bra 0x8c61d3a6 */
L8c61d286: r[14]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d288; /* mov.w @r12+,r14 */
L8c61d288: r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c61d28a; /* mov.l @r10,r3 */
L8c61d28a: r[14]=r[14]&65535u;goto L8c61d28c; /* extu.w r14,r14 */
L8c61d28c: r[2]=rd(m,bus,&io_status,0x8c61d2fcu,4);if(io_status)return io_status;goto L8c61d28e; /* mov.l 0x8c61d2fc,r2 */
L8c61d28e: r[3]+=r[14];goto L8c61d290; /* add r14,r3 */
L8c61d290: wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c61d292; /* mov.l r3,@r10 */
L8c61d292: r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c61d294; /* mov.w @(0x2,r12),r0 */
L8c61d294: r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;goto L8c61d296; /* mov.w @r12,r7 */
L8c61d296: r[5]=r[0]&65535u;goto L8c61d298; /* extu.w r0,r5 */
L8c61d298: r[0]=sx(m,bus,&io_status,r[12]+0x4u,2);if(io_status)return io_status;goto L8c61d29a; /* mov.w @(0x4,r12),r0 */
L8c61d29a: r[5]<<=2;goto L8c61d29c; /* shll2 r5 */
L8c61d29c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d29e; /* mov.l @r2,r3 */
L8c61d29e: r[7]=r[7]&65535u;goto L8c61d2a0; /* extu.w r7,r7 */
L8c61d2a0: r[7]<<=2;goto L8c61d2a2; /* shll2 r7 */
L8c61d2a2: r[5]<<=2;goto L8c61d2a4; /* shll2 r5 */
L8c61d2a4: r[6]=r[0]&65535u;goto L8c61d2a6; /* extu.w r0,r6 */
L8c61d2a6: r[7]<<=2;goto L8c61d2a8; /* shll2 r7 */
L8c61d2a8: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61d2aa; /* shll r7 */
L8c61d2aa: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d2ac; /* shll r5 */
L8c61d2ac: r[0]=16u;goto L8c61d2ae; /* mov #0x10,r0 */
L8c61d2ae: r[5]+=r[3];goto L8c61d2b0; /* add r3,r5 */
L8c61d2b0: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d2b2; /* fmov.s @(r0,r5),fr5 */
L8c61d2b2: r[7]+=r[3];goto L8c61d2b4; /* add r3,r7 */
L8c61d2b4: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d2b6; /* fmov.s @(r0,r7),fr3 */
L8c61d2b6: r[6]<<=2;goto L8c61d2b8; /* shll2 r6 */
L8c61d2b8: fcopy(f,2u,5u);goto L8c61d2ba; /* fmov fr5,fr2 */
L8c61d2ba: r[6]<<=2;goto L8c61d2bc; /* shll2 r6 */
L8c61d2bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d2be; /* fsub fr3,fr2 */
L8c61d2be: r[0]=20u;goto L8c61d2c0; /* mov #0x14,r0 */
L8c61d2c0: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61d2c2; /* shll r6 */
L8c61d2c2: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d2c4; /* fmov.s @(r0,r5),fr4 */
L8c61d2c4: r[6]+=r[3];goto L8c61d2c6; /* add r3,r6 */
L8c61d2c6: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d2c8; /* fmov.s @(r0,r6),fr3 */
L8c61d2c8: fcopy(f,1u,4u);goto L8c61d2ca; /* fmov fr4,fr1 */
L8c61d2ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d2cc; /* fsub fr4,fr3 */
L8c61d2cc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d2ce; /* fmul fr3,fr2 */
L8c61d2ce: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61d2d0; /* fmov.s @(r0,r7),fr3 */
L8c61d2d0: r[0]=16u;goto L8c61d2d2; /* mov #0x10,r0 */
L8c61d2d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d2d4; /* fsub fr3,fr1 */
L8c61d2d4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d2d6; /* fmov.s @(r0,r6),fr3 */
L8c61d2d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d2d8; /* fsub fr5,fr3 */
L8c61d2d8: fcopy(f,4u,2u);goto L8c61d2da; /* fmov fr2,fr4 */
L8c61d2da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d2dc; /* fmul fr3,fr1 */
L8c61d2dc: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d2de; /* fldi0 fr3 */
L8c61d2de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d2e0; /* fsub fr1,fr4 */
L8c61d2e0: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d2e2; /* fcmp/gt fr3,fr4 */
L8c61d2e2: if(!(*sr&1u))goto L8c61d300;goto L8c61d2e4; /* bf 0x8c61d300 */
L8c61d2e4: r[1]=r[14];goto L8c61d2e6; /* mov r14,r1 */
L8c61d2e6: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c61d2e8; /* shll r1 */
L8c61d2e8: r[14]<<=2;goto L8c61d2ea; /* shll2 r14 */
L8c61d2ea: r[12]+=r[1];goto L8c61d2ec; /* add r1,r12 */
L8c61d2ec: r[13]+=r[14];goto L8c61d3a4; /* bra 0x8c61d3a4 */
L8c61d300: r[2]=rd(m,bus,&io_status,0x8c61d3bcu,4);if(io_status)return io_status;goto L8c61d302; /* mov.l 0x8c61d3bc,r2 */
L8c61d302: r[7]=r[14];goto L8c61d304; /* mov r14,r7 */
L8c61d304: r[7]+=4294967295u;goto L8c61d306; /* add #-0x1,r7 */
L8c61d306: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d308; /* mov.l @r2,r3 */
L8c61d308: *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c61d30a; /* cmp/pl r7 */
L8c61d30a: r[3]+=r[14];goto L8c61d30c; /* add r14,r3 */
L8c61d30c: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61d30e; /* mov.l r3,@r2 */
L8c61d30e: take=!(*sr&1u);r[6]=r[11];if(take)goto L8c61d35e;goto L8c61d312; /* bf/s 0x8c61d35e */
L8c61d312: r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d314; /* mov.w @r12+,r5 */
L8c61d314: r[8]+=28u;goto L8c61d316; /* add #0x1c,r8 */
L8c61d316: r[2]=rd(m,bus,&io_status,0x8c61d3c0u,4);if(io_status)return io_status;goto L8c61d318; /* mov.l 0x8c61d3c0,r2 */
L8c61d318: r[5]=r[5]&65535u;goto L8c61d31a; /* extu.w r5,r5 */
L8c61d31a: r[5]<<=2;goto L8c61d31c; /* shll2 r5 */
L8c61d31c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d31e; /* mov.l @r2,r3 */
L8c61d31e: r[5]<<=2;goto L8c61d320; /* shll2 r5 */
L8c61d320: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d322; /* fmov.s fr7,@-r8 */
L8c61d322: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d324; /* mov.w @(0x2,r13),r0 */
L8c61d324: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d326; /* shll r5 */
L8c61d326: r[5]+=r[3];goto L8c61d328; /* add r3,r5 */
L8c61d328: r[3]=r[0];goto L8c61d32a; /* mov r0,r3 */
L8c61d32a: *fpul=r[3];goto L8c61d32c; /* lds r3,FPUL */
L8c61d32c: r[0]=24u;goto L8c61d32e; /* mov #0x18,r0 */
L8c61d32e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d330; /* float FPUL,fr3 */
L8c61d330: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d332; /* fmul fr6,fr3 */
L8c61d332: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d334; /* fmov.s fr3,@-r8 */
L8c61d334: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d336; /* mov.w @r13,r3 */
L8c61d336: r[13]+=4u;goto L8c61d338; /* add #0x4,r13 */
L8c61d338: *fpul=r[3];goto L8c61d33a; /* lds r3,FPUL */
L8c61d33a: r[3]=rd(m,bus,&io_status,0x8c61d3c4u,4);if(io_status)return io_status;goto L8c61d33c; /* mov.l 0x8c61d3c4,r3 */
L8c61d33c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d33e; /* float FPUL,fr3 */
L8c61d33e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d340; /* fmul fr6,fr3 */
L8c61d340: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d342; /* fmov.s fr3,@-r8 */
L8c61d342: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d344; /* fmov.s @(r0,r5),fr3 */
L8c61d344: r[0]=20u;goto L8c61d346; /* mov #0x14,r0 */
L8c61d346: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d348; /* fmov.s fr3,@-r8 */
L8c61d348: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d34a; /* fmov.s @(r0,r5),fr3 */
L8c61d34a: r[0]=16u;goto L8c61d34c; /* mov #0x10,r0 */
L8c61d34c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d34e; /* fmov.s fr3,@-r8 */
L8c61d34e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d350; /* fmov.s @(r0,r5),fr3 */
L8c61d350: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d352; /* fmov.s fr3,@-r8 */
L8c61d352: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d354; /* mov.l r3,@-r8 */
L8c61d354: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d356; /* pref @r8 */
L8c61d356: r[6]+=1u;goto L8c61d358; /* add #0x1,r6 */
L8c61d358: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c61d35a; /* cmp/ge r7,r6 */
L8c61d35a: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61d312;goto L8c61d35e; /* bf/s 0x8c61d312 */
L8c61d35e: r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61d360; /* mov.w @r12+,r5 */
L8c61d360: r[8]+=28u;goto L8c61d362; /* add #0x1c,r8 */
L8c61d362: r[2]=rd(m,bus,&io_status,0x8c61d3c0u,4);if(io_status)return io_status;goto L8c61d364; /* mov.l 0x8c61d3c0,r2 */
L8c61d364: r[5]=r[5]&65535u;goto L8c61d366; /* extu.w r5,r5 */
L8c61d366: r[5]<<=2;goto L8c61d368; /* shll2 r5 */
L8c61d368: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d36a; /* mov.l @r2,r3 */
L8c61d36a: r[5]<<=2;goto L8c61d36c; /* shll2 r5 */
L8c61d36c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61d36e; /* fmov.s fr7,@-r8 */
L8c61d36e: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61d370; /* mov.w @(0x2,r13),r0 */
L8c61d370: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d372; /* shll r5 */
L8c61d372: r[5]+=r[3];goto L8c61d374; /* add r3,r5 */
L8c61d374: r[3]=r[0];goto L8c61d376; /* mov r0,r3 */
L8c61d376: *fpul=r[3];goto L8c61d378; /* lds r3,FPUL */
L8c61d378: r[0]=24u;goto L8c61d37a; /* mov #0x18,r0 */
L8c61d37a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d37c; /* float FPUL,fr3 */
L8c61d37c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d37e; /* fmul fr6,fr3 */
L8c61d37e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d380; /* fmov.s fr3,@-r8 */
L8c61d380: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61d382; /* mov.w @r13,r3 */
L8c61d382: r[13]+=4u;goto L8c61d384; /* add #0x4,r13 */
L8c61d384: *fpul=r[3];goto L8c61d386; /* lds r3,FPUL */
L8c61d386: r[3]=rd(m,bus,&io_status,0x8c61d3c8u,4);if(io_status)return io_status;goto L8c61d388; /* mov.l 0x8c61d3c8,r3 */
L8c61d388: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d38a; /* float FPUL,fr3 */
L8c61d38a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d38c; /* fmul fr6,fr3 */
L8c61d38c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d38e; /* fmov.s fr3,@-r8 */
L8c61d38e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d390; /* fmov.s @(r0,r5),fr3 */
L8c61d390: r[0]=20u;goto L8c61d392; /* mov #0x14,r0 */
L8c61d392: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d394; /* fmov.s fr3,@-r8 */
L8c61d394: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d396; /* fmov.s @(r0,r5),fr3 */
L8c61d396: r[0]=16u;goto L8c61d398; /* mov #0x10,r0 */
L8c61d398: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d39a; /* fmov.s fr3,@-r8 */
L8c61d39a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d39c; /* fmov.s @(r0,r5),fr3 */
L8c61d39c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d39e; /* fmov.s fr3,@-r8 */
L8c61d39e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d3a0; /* mov.l r3,@-r8 */
L8c61d3a0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d3a2; /* pref @r8 */
L8c61d3a2: r[8]+=32u;goto L8c61d3a4; /* add #0x20,r8 */
L8c61d3a4: r[9]+=1u;goto L8c61d3a6; /* add #0x1,r9 */
L8c61d3a6: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61d3a8; /* mov.w @(0x2,r4),r0 */
L8c61d3a8: r[0]=r[0]&65535u;goto L8c61d3aa; /* extu.w r0,r0 */
L8c61d3aa: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61d3ac; /* cmp/ge r0,r9 */
L8c61d3ac: if((*sr&1u))goto L8c61d3b2;goto L8c61d3ae; /* bt 0x8c61d3b2 */
L8c61d3ae: goto L8c61d286; /* bra 0x8c61d286 */
L8c61d3b2: goto L8c61d3f4; /* bra 0x8c61d3f4 */
L8c61d3cc: r[1]=rd(m,bus,&io_status,0x8c61d3d4u,4);if(io_status)return io_status;goto L8c61d3ce; /* mov.l 0x8c61d3d4,r1 */
L8c61d3ce: wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c61d3d0; /* mov.l r4,@r1 */
L8c61d3d0: goto L8c61d3d8; /* bra 0x8c61d3d8 */
L8c61d3d8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61d3da; /* frchg */
L8c61d3da: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61d3dc; /* fmov.s fr0,@-r15 */
L8c61d3dc: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61d3de; /* fmov.s fr1,@-r15 */
L8c61d3de: r[0]=0x8c61d404u;goto L8c61d3e0; /* mova 0x8c61d404,r0 */
L8c61d3e0: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;goto L8c61d3e2; /* fmov.s @r0,fr0 */
L8c61d3e2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61d3e4; /* frchg */
L8c61d3e4: r[2]=rd(m,bus,&io_status,0x8c61d4fcu,4);if(io_status)return io_status;goto L8c61d3e6; /* mov.l 0x8c61d4fc,r2 */
L8c61d3e6: r[3]=rd(m,bus,&io_status,0x8c61d4f8u,4);if(io_status)return io_status;goto L8c61d3e8; /* mov.l 0x8c61d4f8,r3 */
L8c61d3e8: target=r[2];*pr=0x8c61d3ecu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61d3ec; /* jsr @r2 */
L8c61d3ec: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61d3ee; /* frchg */
L8c61d3ee: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61d3f0; /* fmov.s @r15+,fr1 */
L8c61d3f0: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61d3f2; /* fmov.s @r15+,fr0 */
L8c61d3f2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61d3f4; /* frchg */
L8c61d3f4: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d3f6; /* lds.l @r15+,PR */
L8c61d3f6: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d3f8; /* mov.l @r15+,r9 */
L8c61d3f8: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d3fa; /* mov.l @r15+,r10 */
L8c61d3fa: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d3fc; /* mov.l @r15+,r11 */
L8c61d3fc: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d3fe; /* mov.l @r15+,r12 */
L8c61d3fe: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61d400; /* mov.l @r15+,r13 */
L8c61d400: if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
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
L8c61d408: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61d40a; /* mov.l r14,@-r15 */
L8c61d40a: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61d40c; /* mov.l r13,@-r15 */
L8c61d40c: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61d40e; /* mov.l r12,@-r15 */
L8c61d40e: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61d410; /* mov.l r11,@-r15 */
L8c61d410: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61d412; /* mov.l r10,@-r15 */
L8c61d412: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61d414; /* mov.l r9,@-r15 */
L8c61d414: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61d416; /* fmov.s fr15,@-r15 */
L8c61d416: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61d418; /* fmov.s fr14,@-r15 */
L8c61d418: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61d41a; /* sts.l PR,@-r15 */
L8c61d41a: r[15]+=4294967272u;goto L8c61d41c; /* add #-0x18,r15 */
L8c61d41c: r[0]=0x8c61d500u;goto L8c61d41e; /* mova 0x8c61d500,r0 */
L8c61d41e: r[3]=rd(m,bus,&io_status,0x8c61d504u,4);if(io_status)return io_status;goto L8c61d420; /* mov.l 0x8c61d504,r3 */
L8c61d420: fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c61d422; /* fmov.s @r0,fr15 */
L8c61d422: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0x3f800000u;goto L8c61d424; /* fldi1 fr14 */
L8c61d424: wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c61d426; /* mov.l r4,@(0x8,r15) */
L8c61d426: r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61d428; /* mov.w @r4,r0 */
L8c61d428: r[11]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c61d42a; /* mov.l @(0x14,r4),r11 */
L8c61d42a: r[0]=r[0]&65535u;goto L8c61d42c; /* extu.w r0,r0 */
L8c61d42c: r[0]&=r[3];goto L8c61d42e; /* and r3,r0 */
L8c61d42e: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61d430; /* cmp/eq #0x0,r0 */
L8c61d430: take=(*sr&1u);r[10]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61d456;goto L8c61d434; /* bt/s 0x8c61d456 */
L8c61d434: r[1]=sx(m,bus,&io_status,0x8c61d4f6u,2);if(io_status)return io_status;goto L8c61d436; /* mov.w 0x8c61d4f6,r1 */
L8c61d436: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61d438; /* cmp/eq r1,r0 */
L8c61d438: if(!(*sr&1u))goto L8c61d43e;goto L8c61d43a; /* bf 0x8c61d43e */
L8c61d43a: goto L8c61d636; /* bra 0x8c61d636 */
L8c61d43e: r[1]=rd(m,bus,&io_status,0x8c61d508u,4);if(io_status)return io_status;goto L8c61d440; /* mov.l 0x8c61d508,r1 */
L8c61d440: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61d442; /* cmp/eq r1,r0 */
L8c61d442: if(!(*sr&1u))goto L8c61d448;goto L8c61d444; /* bf 0x8c61d448 */
L8c61d444: goto L8c61d8c0; /* bra 0x8c61d8c0 */
L8c61d448: r[1]=rd(m,bus,&io_status,0x8c61d504u,4);if(io_status)return io_status;goto L8c61d44a; /* mov.l 0x8c61d504,r1 */
L8c61d44a: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61d44c; /* cmp/eq r1,r0 */
L8c61d44c: if(!(*sr&1u))goto L8c61d452;goto L8c61d44e; /* bf 0x8c61d452 */
L8c61d44e: goto L8c61db4c; /* bra 0x8c61db4c */
L8c61d452: goto L8c61db7c; /* bra 0x8c61db7c */
L8c61d456: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61d458; /* mov.l @(0x8,r15),r2 */
L8c61d458: r[12]=0u;goto L8c61d45a; /* mov #0x0,r12 */
L8c61d45a: r[1]=rd(m,bus,&io_status,0x8c61d50cu,4);if(io_status)return io_status;goto L8c61d45c; /* mov.l 0x8c61d50c,r1 */
L8c61d45c: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61d45e; /* mov.w @(0x2,r2),r0 */
L8c61d45e: r[13]=rd(m,bus,&io_status,0x8c61d510u,4);if(io_status)return io_status;goto L8c61d460; /* mov.l 0x8c61d510,r13 */
L8c61d460: r[0]=r[0]&65535u;goto L8c61d462; /* extu.w r0,r0 */
L8c61d462: r[14]=rd(m,bus,&io_status,0x8c61d514u,4);if(io_status)return io_status;goto L8c61d464; /* mov.l 0x8c61d514,r14 */
L8c61d464: r[2]=r[0];goto L8c61d466; /* mov r0,r2 */
L8c61d466: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61d468; /* shll r0 */
L8c61d468: r[0]+=r[2];goto L8c61d46a; /* add r2,r0 */
L8c61d46a: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61d46c; /* mov.l @r1,r2 */
L8c61d46c: r[9]=r[13];goto L8c61d46e; /* mov r13,r9 */
L8c61d46e: r[2]+=r[0];goto L8c61d470; /* add r0,r2 */
L8c61d470: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61d472; /* mov.l r2,@r1 */
L8c61d472: r[9]+=4u;goto L8c61d624; /* bra 0x8c61d624 */
L8c61d476: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d478; /* mov.w @r10+,r4 */
L8c61d478: r[2]=rd(m,bus,&io_status,0x8c61d518u,4);if(io_status)return io_status;goto L8c61d47a; /* mov.l 0x8c61d518,r2 */
L8c61d47a: r[5]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d47c; /* mov.w @r10+,r5 */
L8c61d47c: r[4]=r[4]&65535u;goto L8c61d47e; /* extu.w r4,r4 */
L8c61d47e: r[4]<<=2;goto L8c61d480; /* shll2 r4 */
L8c61d480: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d482; /* mov.l @r2,r3 */
L8c61d482: r[6]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d484; /* mov.w @r10+,r6 */
L8c61d484: r[5]=r[5]&65535u;goto L8c61d486; /* extu.w r5,r5 */
L8c61d486: r[5]<<=2;goto L8c61d488; /* shll2 r5 */
L8c61d488: r[6]=r[6]&65535u;goto L8c61d48a; /* extu.w r6,r6 */
L8c61d48a: r[6]<<=2;goto L8c61d48c; /* shll2 r6 */
L8c61d48c: r[4]<<=2;goto L8c61d48e; /* shll2 r4 */
L8c61d48e: r[6]<<=2;goto L8c61d490; /* shll2 r6 */
L8c61d490: r[5]<<=2;goto L8c61d492; /* shll2 r5 */
L8c61d492: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61d494; /* shll r4 */
L8c61d494: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d496; /* shll r5 */
L8c61d496: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61d498; /* shll r6 */
L8c61d498: r[6]+=r[3];goto L8c61d49a; /* add r3,r6 */
L8c61d49a: r[4]+=r[3];goto L8c61d49c; /* add r3,r4 */
L8c61d49c: r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61d49e; /* mov.l @(0xc,r6),r1 */
L8c61d49e: r[5]+=r[3];goto L8c61d4a0; /* add r3,r5 */
L8c61d4a0: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61d4a2; /* mov.l @(0xc,r4),r7 */
L8c61d4a2: r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61d4a4; /* mov.l @(0xc,r5),r3 */
L8c61d4a4: r[7]&=r[3];goto L8c61d4a6; /* and r3,r7 */
L8c61d4a6: r[3]=63u;goto L8c61d4a8; /* mov #0x3f,r3 */
L8c61d4a8: r[7]&=r[1];goto L8c61d4aa; /* and r1,r7 */
L8c61d4aa: *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c61d4ac; /* tst r3,r7 */
L8c61d4ac: if(!(*sr&1u))goto L8c61d4ee;goto L8c61d4ae; /* bf 0x8c61d4ee */
L8c61d4ae: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61d4b0; /* mov.l @(0xc,r4),r7 */
L8c61d4b0: r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61d4b2; /* mov.l @(0xc,r5),r3 */
L8c61d4b2: r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61d4b4; /* mov.l @(0xc,r6),r2 */
L8c61d4b4: r[7]|=r[3];goto L8c61d4b6; /* or r3,r7 */
L8c61d4b6: r[3]=1u;goto L8c61d4b8; /* mov #0x1,r3 */
L8c61d4b8: r[7]|=r[2];goto L8c61d4ba; /* or r2,r7 */
L8c61d4ba: *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c61d4bc; /* tst r3,r7 */
L8c61d4bc: if((*sr&1u))goto L8c61d4c2;goto L8c61d4be; /* bt 0x8c61d4c2 */
L8c61d4be: goto L8c61d5c4; /* bra 0x8c61d5c4 */
L8c61d4c2: r[0]=16u;goto L8c61d4c4; /* mov #0x10,r0 */
L8c61d4c4: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d4c6; /* fmov.s @(r0,r5),fr4 */
L8c61d4c6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d4c8; /* fmov.s @(r0,r4),fr3 */
L8c61d4c8: r[0]=20u;goto L8c61d4ca; /* mov #0x14,r0 */
L8c61d4ca: fcopy(f,2u,4u);goto L8c61d4cc; /* fmov fr4,fr2 */
L8c61d4cc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d4ce; /* fsub fr3,fr2 */
L8c61d4ce: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d4d0; /* fmov.s @(r0,r6),fr3 */
L8c61d4d0: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d4d2; /* fmov.s @(r0,r5),fr5 */
L8c61d4d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d4d4; /* fsub fr5,fr3 */
L8c61d4d4: fcopy(f,1u,5u);goto L8c61d4d6; /* fmov fr5,fr1 */
L8c61d4d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d4d8; /* fmul fr3,fr2 */
L8c61d4d8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d4da; /* fmov.s @(r0,r4),fr3 */
L8c61d4da: r[0]=16u;goto L8c61d4dc; /* mov #0x10,r0 */
L8c61d4dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d4de; /* fsub fr3,fr1 */
L8c61d4de: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d4e0; /* fmov.s @(r0,r6),fr3 */
L8c61d4e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d4e2; /* fsub fr4,fr3 */
L8c61d4e2: fcopy(f,4u,2u);goto L8c61d4e4; /* fmov fr2,fr4 */
L8c61d4e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d4e6; /* fmul fr3,fr1 */
L8c61d4e6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d4e8; /* fldi0 fr3 */
L8c61d4e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d4ea; /* fsub fr1,fr4 */
L8c61d4ea: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d4ec; /* fcmp/gt fr3,fr4 */
L8c61d4ec: if(!(*sr&1u))goto L8c61d51c;goto L8c61d4ee; /* bf 0x8c61d51c */
L8c61d4ee: r[11]+=12u;goto L8c61d622; /* bra 0x8c61d622 */
L8c61d51c: r[8]+=28u;goto L8c61d51e; /* add #0x1c,r8 */
L8c61d51e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d520; /* fmov.s fr14,@-r8 */
L8c61d520: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d522; /* mov.w @(0x2,r11),r0 */
L8c61d522: r[3]=r[0];goto L8c61d524; /* mov r0,r3 */
L8c61d524: *fpul=r[3];goto L8c61d526; /* lds r3,FPUL */
L8c61d526: r[0]=24u;goto L8c61d528; /* mov #0x18,r0 */
L8c61d528: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d52a; /* float FPUL,fr3 */
L8c61d52a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d52c; /* fmul fr15,fr3 */
L8c61d52c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d52e; /* fmov.s fr3,@-r8 */
L8c61d52e: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d530; /* mov.w @r11,r3 */
L8c61d530: r[11]+=4u;goto L8c61d532; /* add #0x4,r11 */
L8c61d532: *fpul=r[3];goto L8c61d534; /* lds r3,FPUL */
L8c61d534: r[3]=rd(m,bus,&io_status,0x8c61d7c8u,4);if(io_status)return io_status;goto L8c61d536; /* mov.l 0x8c61d7c8,r3 */
L8c61d536: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d538; /* float FPUL,fr3 */
L8c61d538: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d53a; /* fmul fr15,fr3 */
L8c61d53a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d53c; /* fmov.s fr3,@-r8 */
L8c61d53c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d53e; /* fmov.s @(r0,r4),fr3 */
L8c61d53e: r[0]=20u;goto L8c61d540; /* mov #0x14,r0 */
L8c61d540: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d542; /* fmov.s fr3,@-r8 */
L8c61d542: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d544; /* fmov.s @(r0,r4),fr3 */
L8c61d544: r[0]=16u;goto L8c61d546; /* mov #0x10,r0 */
L8c61d546: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d548; /* fmov.s fr3,@-r8 */
L8c61d548: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d54a; /* fmov.s @(r0,r4),fr3 */
L8c61d54a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d54c; /* fmov.s fr3,@-r8 */
L8c61d54c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d54e; /* mov.l r3,@-r8 */
L8c61d54e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d550; /* pref @r8 */
L8c61d550: r[8]+=60u;goto L8c61d552; /* add #0x3c,r8 */
L8c61d552: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d554; /* fmov.s fr14,@-r8 */
L8c61d554: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d556; /* mov.w @(0x2,r11),r0 */
L8c61d556: r[3]=r[0];goto L8c61d558; /* mov r0,r3 */
L8c61d558: *fpul=r[3];goto L8c61d55a; /* lds r3,FPUL */
L8c61d55a: r[0]=24u;goto L8c61d55c; /* mov #0x18,r0 */
L8c61d55c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d55e; /* float FPUL,fr3 */
L8c61d55e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d560; /* fmul fr15,fr3 */
L8c61d560: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d562; /* fmov.s fr3,@-r8 */
L8c61d562: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d564; /* mov.w @r11,r3 */
L8c61d564: r[11]+=4u;goto L8c61d566; /* add #0x4,r11 */
L8c61d566: *fpul=r[3];goto L8c61d568; /* lds r3,FPUL */
L8c61d568: r[3]=rd(m,bus,&io_status,0x8c61d7c8u,4);if(io_status)return io_status;goto L8c61d56a; /* mov.l 0x8c61d7c8,r3 */
L8c61d56a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d56c; /* float FPUL,fr3 */
L8c61d56c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d56e; /* fmul fr15,fr3 */
L8c61d56e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d570; /* fmov.s fr3,@-r8 */
L8c61d570: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d572; /* fmov.s @(r0,r5),fr3 */
L8c61d572: r[0]=20u;goto L8c61d574; /* mov #0x14,r0 */
L8c61d574: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d576; /* fmov.s fr3,@-r8 */
L8c61d576: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d578; /* fmov.s @(r0,r5),fr3 */
L8c61d578: r[0]=16u;goto L8c61d57a; /* mov #0x10,r0 */
L8c61d57a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d57c; /* fmov.s fr3,@-r8 */
L8c61d57c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d57e; /* fmov.s @(r0,r5),fr3 */
L8c61d57e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d580; /* fmov.s fr3,@-r8 */
L8c61d580: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d582; /* mov.l r3,@-r8 */
L8c61d582: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d584; /* pref @r8 */
L8c61d584: r[8]+=60u;goto L8c61d586; /* add #0x3c,r8 */
L8c61d586: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d588; /* fmov.s fr14,@-r8 */
L8c61d588: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d58a; /* mov.w @(0x2,r11),r0 */
L8c61d58a: r[3]=r[0];goto L8c61d58c; /* mov r0,r3 */
L8c61d58c: *fpul=r[3];goto L8c61d58e; /* lds r3,FPUL */
L8c61d58e: r[0]=24u;goto L8c61d590; /* mov #0x18,r0 */
L8c61d590: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d592; /* float FPUL,fr3 */
L8c61d592: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d594; /* fmul fr15,fr3 */
L8c61d594: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d596; /* fmov.s fr3,@-r8 */
L8c61d596: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d598; /* mov.w @r11,r3 */
L8c61d598: r[11]+=4u;goto L8c61d59a; /* add #0x4,r11 */
L8c61d59a: *fpul=r[3];goto L8c61d59c; /* lds r3,FPUL */
L8c61d59c: r[3]=rd(m,bus,&io_status,0x8c61d7ccu,4);if(io_status)return io_status;goto L8c61d59e; /* mov.l 0x8c61d7cc,r3 */
L8c61d59e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d5a0; /* float FPUL,fr3 */
L8c61d5a0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d5a2; /* fmul fr15,fr3 */
L8c61d5a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d5a4; /* fmov.s fr3,@-r8 */
L8c61d5a4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d5a6; /* fmov.s @(r0,r6),fr3 */
L8c61d5a6: r[0]=20u;goto L8c61d5a8; /* mov #0x14,r0 */
L8c61d5a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d5aa; /* fmov.s fr3,@-r8 */
L8c61d5aa: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d5ac; /* fmov.s @(r0,r6),fr3 */
L8c61d5ac: r[0]=16u;goto L8c61d5ae; /* mov #0x10,r0 */
L8c61d5ae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d5b0; /* fmov.s fr3,@-r8 */
L8c61d5b0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d5b2; /* fmov.s @(r0,r6),fr3 */
L8c61d5b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d5b4; /* fmov.s fr3,@-r8 */
L8c61d5b4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d5b6; /* mov.l r3,@-r8 */
L8c61d5b6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d5b8; /* pref @r8 */
L8c61d5b8: r[2]=rd(m,bus,&io_status,0x8c61d7d0u,4);if(io_status)return io_status;goto L8c61d5ba; /* mov.l 0x8c61d7d0,r2 */
L8c61d5ba: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d5bc; /* mov.l @r2,r3 */
L8c61d5bc: r[3]+=3u;goto L8c61d5be; /* add #0x3,r3 */
L8c61d5be: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61d5c0; /* mov.l r3,@r2 */
L8c61d5c0: r[8]+=32u;goto L8c61d622; /* bra 0x8c61d622 */
L8c61d5c4: r[1]=rd(m,bus,&io_status,0x8c61d7d4u,4);if(io_status)return io_status;goto L8c61d5c6; /* mov.l 0x8c61d7d4,r1 */
L8c61d5c6: wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c61d5c8; /* mov.l r3,@r1 */
L8c61d5c8: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d5ca; /* mov.w @r11,r3 */
L8c61d5ca: *fpul=r[3];goto L8c61d5cc; /* lds r3,FPUL */
L8c61d5cc: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d5ce; /* float FPUL,fr3 */
L8c61d5ce: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d5d0; /* ftrc fr3,FPUL */
L8c61d5d0: r[2]=*fpul;goto L8c61d5d2; /* sts FPUL,r2 */
L8c61d5d2: wr(m,bus,&io_status,r[13],r[2],2);if(io_status)return io_status;goto L8c61d5d4; /* mov.w r2,@r13 */
L8c61d5d4: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d5d6; /* mov.w @(0x2,r11),r0 */
L8c61d5d6: r[11]+=4u;goto L8c61d5d8; /* add #0x4,r11 */
L8c61d5d8: r[3]=r[0];goto L8c61d5da; /* mov r0,r3 */
L8c61d5da: *fpul=r[3];goto L8c61d5dc; /* lds r3,FPUL */
L8c61d5dc: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d5de; /* float FPUL,fr3 */
L8c61d5de: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d5e0; /* ftrc fr3,FPUL */
L8c61d5e0: r[0]=*fpul;goto L8c61d5e2; /* sts FPUL,r0 */
L8c61d5e2: wr(m,bus,&io_status,r[13]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d5e4; /* mov.w r0,@(0x2,r13) */
L8c61d5e4: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d5e6; /* mov.w @r11,r3 */
L8c61d5e6: *fpul=r[3];goto L8c61d5e8; /* lds r3,FPUL */
L8c61d5e8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d5ea; /* float FPUL,fr3 */
L8c61d5ea: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d5ec; /* ftrc fr3,FPUL */
L8c61d5ec: r[2]=*fpul;goto L8c61d5ee; /* sts FPUL,r2 */
L8c61d5ee: wr(m,bus,&io_status,r[9],r[2],2);if(io_status)return io_status;goto L8c61d5f0; /* mov.w r2,@r9 */
L8c61d5f0: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d5f2; /* mov.w @(0x2,r11),r0 */
L8c61d5f2: r[11]+=4u;goto L8c61d5f4; /* add #0x4,r11 */
L8c61d5f4: r[3]=r[0];goto L8c61d5f6; /* mov r0,r3 */
L8c61d5f6: *fpul=r[3];goto L8c61d5f8; /* lds r3,FPUL */
L8c61d5f8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d5fa; /* float FPUL,fr3 */
L8c61d5fa: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d5fc; /* ftrc fr3,FPUL */
L8c61d5fc: r[0]=*fpul;goto L8c61d5fe; /* sts FPUL,r0 */
L8c61d5fe: wr(m,bus,&io_status,r[9]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d600; /* mov.w r0,@(0x2,r9) */
L8c61d600: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d602; /* mov.w @r11,r3 */
L8c61d602: *fpul=r[3];goto L8c61d604; /* lds r3,FPUL */
L8c61d604: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d606; /* float FPUL,fr3 */
L8c61d606: r[7]=rd(m,bus,&io_status,0x8c61d7d8u,4);if(io_status)return io_status;goto L8c61d608; /* mov.l 0x8c61d7d8,r7 */
L8c61d608: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d60a; /* ftrc fr3,FPUL */
L8c61d60a: r[2]=*fpul;goto L8c61d60c; /* sts FPUL,r2 */
L8c61d60c: wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c61d60e; /* mov.w r2,@r14 */
L8c61d60e: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d610; /* mov.w @(0x2,r11),r0 */
L8c61d610: r[3]=r[0];goto L8c61d612; /* mov r0,r3 */
L8c61d612: *fpul=r[3];goto L8c61d614; /* lds r3,FPUL */
L8c61d614: r[3]=rd(m,bus,&io_status,0x8c61d7dcu,4);if(io_status)return io_status;goto L8c61d616; /* mov.l 0x8c61d7dc,r3 */
L8c61d616: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d618; /* float FPUL,fr3 */
L8c61d618: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d61a; /* ftrc fr3,FPUL */
L8c61d61a: r[0]=*fpul;goto L8c61d61c; /* sts FPUL,r0 */
L8c61d61c: wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d61e; /* mov.w r0,@(0x2,r14) */
L8c61d61e: target=r[3];*pr=0x8c61d622u;r[11]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61d622; /* jsr @r3 */
L8c61d622: r[12]+=1u;goto L8c61d624; /* add #0x1,r12 */
L8c61d624: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61d626; /* mov.l @(0x8,r15),r2 */
L8c61d626: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61d628; /* mov.w @(0x2,r2),r0 */
L8c61d628: r[0]=r[0]&65535u;goto L8c61d62a; /* extu.w r0,r0 */
L8c61d62a: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[0]);goto L8c61d62c; /* cmp/ge r0,r12 */
L8c61d62c: if((*sr&1u))goto L8c61d632;goto L8c61d62e; /* bt 0x8c61d632 */
L8c61d62e: goto L8c61d476; /* bra 0x8c61d476 */
L8c61d632: goto L8c61db7c; /* bra 0x8c61db7c */
L8c61d636: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61d638; /* mov.l @(0x8,r15),r1 */
L8c61d638: r[2]=rd(m,bus,&io_status,0x8c61d7e0u,4);if(io_status)return io_status;goto L8c61d63a; /* mov.l 0x8c61d7e0,r2 */
L8c61d63a: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61d63c; /* mov.w @(0x2,r1),r0 */
L8c61d63c: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d63e; /* mov.l @r2,r1 */
L8c61d63e: r[0]=r[0]&65535u;goto L8c61d640; /* extu.w r0,r0 */
L8c61d640: r[9]=rd(m,bus,&io_status,0x8c61d7d8u,4);if(io_status)return io_status;goto L8c61d642; /* mov.l 0x8c61d7d8,r9 */
L8c61d642: r[0]<<=2;goto L8c61d644; /* shll2 r0 */
L8c61d644: r[3]=rd(m,bus,&io_status,0x8c61d7e4u,4);if(io_status)return io_status;goto L8c61d646; /* mov.l 0x8c61d7e4,r3 */
L8c61d646: r[1]+=r[0];goto L8c61d648; /* add r0,r1 */
L8c61d648: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61d64a; /* mov.l r1,@r2 */
L8c61d64a: r[2]=0u;goto L8c61d64c; /* mov #0x0,r2 */
L8c61d64c: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c61d64e; /* mov.l r2,@(0xc,r15) */
L8c61d64e: r[2]=r[9];goto L8c61d650; /* mov r9,r2 */
L8c61d650: r[1]=rd(m,bus,&io_status,0x8c61d7e8u,4);if(io_status)return io_status;goto L8c61d652; /* mov.l 0x8c61d7e8,r1 */
L8c61d652: r[2]+=4u;goto L8c61d654; /* add #0x4,r2 */
L8c61d654: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61d656; /* mov.l r2,@r15 */
L8c61d656: wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c61d658; /* mov.l r3,@(0x10,r15) */
L8c61d658: wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c61d8ac; /* bra 0x8c61d8ac */
L8c61d65c: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d65e; /* mov.w @r10+,r4 */
L8c61d65e: r[2]=rd(m,bus,&io_status,0x8c61d7ecu,4);if(io_status)return io_status;goto L8c61d660; /* mov.l 0x8c61d7ec,r2 */
L8c61d660: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d662; /* mov.w @r10+,r14 */
L8c61d662: r[4]=r[4]&65535u;goto L8c61d664; /* extu.w r4,r4 */
L8c61d664: r[4]<<=2;goto L8c61d666; /* shll2 r4 */
L8c61d666: r[6]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d668; /* mov.l @r2,r6 */
L8c61d668: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d66a; /* mov.w @r10+,r13 */
L8c61d66a: r[14]=r[14]&65535u;goto L8c61d66c; /* extu.w r14,r14 */
L8c61d66c: r[14]<<=2;goto L8c61d66e; /* shll2 r14 */
L8c61d66e: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d670; /* mov.w @r10+,r12 */
L8c61d670: r[13]=r[13]&65535u;goto L8c61d672; /* extu.w r13,r13 */
L8c61d672: r[14]<<=2;goto L8c61d674; /* shll2 r14 */
L8c61d674: r[4]<<=2;goto L8c61d676; /* shll2 r4 */
L8c61d676: r[12]=r[12]&65535u;goto L8c61d678; /* extu.w r12,r12 */
L8c61d678: r[13]<<=2;goto L8c61d67a; /* shll2 r13 */
L8c61d67a: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61d67c; /* shll r4 */
L8c61d67c: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61d67e; /* shll r14 */
L8c61d67e: r[4]+=r[6];goto L8c61d680; /* add r6,r4 */
L8c61d680: r[12]<<=2;goto L8c61d682; /* shll2 r12 */
L8c61d682: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61d684; /* mov.l @(0xc,r4),r5 */
L8c61d684: r[14]+=r[6];goto L8c61d686; /* add r6,r14 */
L8c61d686: r[13]<<=2;goto L8c61d688; /* shll2 r13 */
L8c61d688: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61d68a; /* mov.l @(0xc,r14),r3 */
L8c61d68a: r[12]<<=2;goto L8c61d68c; /* shll2 r12 */
L8c61d68c: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61d68e; /* shll r13 */
L8c61d68e: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61d690; /* shll r12 */
L8c61d690: r[13]+=r[6];goto L8c61d692; /* add r6,r13 */
L8c61d692: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61d694; /* mov.l @(0xc,r13),r1 */
L8c61d694: r[5]&=r[3];goto L8c61d696; /* and r3,r5 */
L8c61d696: r[12]+=r[6];goto L8c61d698; /* add r6,r12 */
L8c61d698: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61d69a; /* mov.l @(0xc,r12),r3 */
L8c61d69a: r[5]&=r[1];goto L8c61d69c; /* and r1,r5 */
L8c61d69c: r[1]=63u;goto L8c61d69e; /* mov #0x3f,r1 */
L8c61d69e: r[5]&=r[3];goto L8c61d6a0; /* and r3,r5 */
L8c61d6a0: *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c61d6a2; /* tst r1,r5 */
L8c61d6a2: if(!(*sr&1u))goto L8c61d6e8;goto L8c61d6a4; /* bf 0x8c61d6e8 */
L8c61d6a4: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61d6a6; /* mov.l @(0xc,r4),r5 */
L8c61d6a6: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61d6a8; /* mov.l @(0xc,r14),r3 */
L8c61d6a8: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61d6aa; /* mov.l @(0xc,r13),r2 */
L8c61d6aa: r[5]|=r[3];goto L8c61d6ac; /* or r3,r5 */
L8c61d6ac: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61d6ae; /* mov.l @(0xc,r12),r3 */
L8c61d6ae: r[5]|=r[2];goto L8c61d6b0; /* or r2,r5 */
L8c61d6b0: r[2]=1u;goto L8c61d6b2; /* mov #0x1,r2 */
L8c61d6b2: r[5]|=r[3];goto L8c61d6b4; /* or r3,r5 */
L8c61d6b4: *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c61d6b6; /* tst r2,r5 */
L8c61d6b6: if((*sr&1u))goto L8c61d6bc;goto L8c61d6b8; /* bt 0x8c61d6bc */
L8c61d6b8: goto L8c61d7f0; /* bra 0x8c61d7f0 */
L8c61d6bc: r[0]=16u;goto L8c61d6be; /* mov #0x10,r0 */
L8c61d6be: fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d6c0; /* fmov.s @(r0,r14),fr5 */
L8c61d6c0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d6c2; /* fmov.s @(r0,r4),fr3 */
L8c61d6c2: r[0]=20u;goto L8c61d6c4; /* mov #0x14,r0 */
L8c61d6c4: fcopy(f,2u,5u);goto L8c61d6c6; /* fmov fr5,fr2 */
L8c61d6c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d6c8; /* fsub fr3,fr2 */
L8c61d6c8: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61d6ca; /* fmov.s @(r0,r13),fr3 */
L8c61d6ca: fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d6cc; /* fmov.s @(r0,r14),fr4 */
L8c61d6cc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d6ce; /* fsub fr4,fr3 */
L8c61d6ce: fcopy(f,1u,4u);goto L8c61d6d0; /* fmov fr4,fr1 */
L8c61d6d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d6d2; /* fmul fr3,fr2 */
L8c61d6d2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d6d4; /* fmov.s @(r0,r4),fr3 */
L8c61d6d4: r[0]=16u;goto L8c61d6d6; /* mov #0x10,r0 */
L8c61d6d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d6d8; /* fsub fr3,fr1 */
L8c61d6d8: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61d6da; /* fmov.s @(r0,r13),fr3 */
L8c61d6da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d6dc; /* fsub fr5,fr3 */
L8c61d6dc: fcopy(f,4u,2u);goto L8c61d6de; /* fmov fr2,fr4 */
L8c61d6de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d6e0; /* fmul fr3,fr1 */
L8c61d6e0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d6e2; /* fldi0 fr3 */
L8c61d6e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d6e4; /* fsub fr1,fr4 */
L8c61d6e4: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d6e6; /* fcmp/gt fr3,fr4 */
L8c61d6e6: if(!(*sr&1u))goto L8c61d6ec;goto L8c61d6e8; /* bf 0x8c61d6ec */
L8c61d6e8: r[11]+=16u;goto L8c61d8a6; /* bra 0x8c61d8a6 */
L8c61d6ec: r[8]+=28u;goto L8c61d6ee; /* add #0x1c,r8 */
L8c61d6ee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d6f0; /* fmov.s fr14,@-r8 */
L8c61d6f0: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d6f2; /* mov.w @(0x2,r11),r0 */
L8c61d6f2: r[3]=r[0];goto L8c61d6f4; /* mov r0,r3 */
L8c61d6f4: *fpul=r[3];goto L8c61d6f6; /* lds r3,FPUL */
L8c61d6f6: r[0]=24u;goto L8c61d6f8; /* mov #0x18,r0 */
L8c61d6f8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d6fa; /* float FPUL,fr3 */
L8c61d6fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d6fc; /* fmul fr15,fr3 */
L8c61d6fc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d6fe; /* fmov.s fr3,@-r8 */
L8c61d6fe: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d700; /* mov.w @r11,r3 */
L8c61d700: r[11]+=4u;goto L8c61d702; /* add #0x4,r11 */
L8c61d702: *fpul=r[3];goto L8c61d704; /* lds r3,FPUL */
L8c61d704: r[3]=rd(m,bus,&io_status,0x8c61d7c8u,4);if(io_status)return io_status;goto L8c61d706; /* mov.l 0x8c61d7c8,r3 */
L8c61d706: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d708; /* float FPUL,fr3 */
L8c61d708: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d70a; /* fmul fr15,fr3 */
L8c61d70a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d70c; /* fmov.s fr3,@-r8 */
L8c61d70c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d70e; /* fmov.s @(r0,r4),fr3 */
L8c61d70e: r[0]=20u;goto L8c61d710; /* mov #0x14,r0 */
L8c61d710: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d712; /* fmov.s fr3,@-r8 */
L8c61d712: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d714; /* fmov.s @(r0,r4),fr3 */
L8c61d714: r[0]=16u;goto L8c61d716; /* mov #0x10,r0 */
L8c61d716: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d718; /* fmov.s fr3,@-r8 */
L8c61d718: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d71a; /* fmov.s @(r0,r4),fr3 */
L8c61d71a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d71c; /* fmov.s fr3,@-r8 */
L8c61d71c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d71e; /* mov.l r3,@-r8 */
L8c61d71e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d720; /* pref @r8 */
L8c61d720: r[8]+=60u;goto L8c61d722; /* add #0x3c,r8 */
L8c61d722: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d724; /* fmov.s fr14,@-r8 */
L8c61d724: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d726; /* mov.w @(0x2,r11),r0 */
L8c61d726: r[3]=r[0];goto L8c61d728; /* mov r0,r3 */
L8c61d728: *fpul=r[3];goto L8c61d72a; /* lds r3,FPUL */
L8c61d72a: r[0]=24u;goto L8c61d72c; /* mov #0x18,r0 */
L8c61d72c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d72e; /* float FPUL,fr3 */
L8c61d72e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d730; /* fmul fr15,fr3 */
L8c61d730: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d732; /* fmov.s fr3,@-r8 */
L8c61d732: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d734; /* mov.w @r11,r3 */
L8c61d734: r[11]+=4u;goto L8c61d736; /* add #0x4,r11 */
L8c61d736: *fpul=r[3];goto L8c61d738; /* lds r3,FPUL */
L8c61d738: r[3]=rd(m,bus,&io_status,0x8c61d7c8u,4);if(io_status)return io_status;goto L8c61d73a; /* mov.l 0x8c61d7c8,r3 */
L8c61d73a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d73c; /* float FPUL,fr3 */
L8c61d73c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d73e; /* fmul fr15,fr3 */
L8c61d73e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d740; /* fmov.s fr3,@-r8 */
L8c61d740: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d742; /* fmov.s @(r0,r14),fr3 */
L8c61d742: r[0]=20u;goto L8c61d744; /* mov #0x14,r0 */
L8c61d744: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d746; /* fmov.s fr3,@-r8 */
L8c61d746: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d748; /* fmov.s @(r0,r14),fr3 */
L8c61d748: r[0]=16u;goto L8c61d74a; /* mov #0x10,r0 */
L8c61d74a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d74c; /* fmov.s fr3,@-r8 */
L8c61d74c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61d74e; /* fmov.s @(r0,r14),fr3 */
L8c61d74e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d750; /* fmov.s fr3,@-r8 */
L8c61d750: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d752; /* mov.l r3,@-r8 */
L8c61d752: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d754; /* pref @r8 */
L8c61d754: r[8]+=60u;goto L8c61d756; /* add #0x3c,r8 */
L8c61d756: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d758; /* fmov.s fr14,@-r8 */
L8c61d758: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d75a; /* mov.w @(0x2,r11),r0 */
L8c61d75a: r[3]=r[0];goto L8c61d75c; /* mov r0,r3 */
L8c61d75c: *fpul=r[3];goto L8c61d75e; /* lds r3,FPUL */
L8c61d75e: r[0]=24u;goto L8c61d760; /* mov #0x18,r0 */
L8c61d760: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d762; /* float FPUL,fr3 */
L8c61d762: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d764; /* fmul fr15,fr3 */
L8c61d764: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d766; /* fmov.s fr3,@-r8 */
L8c61d766: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d768; /* mov.w @r11,r3 */
L8c61d768: r[11]+=4u;goto L8c61d76a; /* add #0x4,r11 */
L8c61d76a: *fpul=r[3];goto L8c61d76c; /* lds r3,FPUL */
L8c61d76c: r[3]=rd(m,bus,&io_status,0x8c61d7c8u,4);if(io_status)return io_status;goto L8c61d76e; /* mov.l 0x8c61d7c8,r3 */
L8c61d76e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d770; /* float FPUL,fr3 */
L8c61d770: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d772; /* fmul fr15,fr3 */
L8c61d772: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d774; /* fmov.s fr3,@-r8 */
L8c61d774: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61d776; /* fmov.s @(r0,r13),fr3 */
L8c61d776: r[0]=20u;goto L8c61d778; /* mov #0x14,r0 */
L8c61d778: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d77a; /* fmov.s fr3,@-r8 */
L8c61d77a: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61d77c; /* fmov.s @(r0,r13),fr3 */
L8c61d77c: r[0]=16u;goto L8c61d77e; /* mov #0x10,r0 */
L8c61d77e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d780; /* fmov.s fr3,@-r8 */
L8c61d780: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61d782; /* fmov.s @(r0,r13),fr3 */
L8c61d782: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d784; /* fmov.s fr3,@-r8 */
L8c61d784: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d786; /* mov.l r3,@-r8 */
L8c61d786: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d788; /* pref @r8 */
L8c61d788: r[8]+=60u;goto L8c61d78a; /* add #0x3c,r8 */
L8c61d78a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d78c; /* fmov.s fr14,@-r8 */
L8c61d78c: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d78e; /* mov.w @(0x2,r11),r0 */
L8c61d78e: r[3]=r[0];goto L8c61d790; /* mov r0,r3 */
L8c61d790: *fpul=r[3];goto L8c61d792; /* lds r3,FPUL */
L8c61d792: r[0]=24u;goto L8c61d794; /* mov #0x18,r0 */
L8c61d794: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d796; /* float FPUL,fr3 */
L8c61d796: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d798; /* fmul fr15,fr3 */
L8c61d798: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d79a; /* fmov.s fr3,@-r8 */
L8c61d79a: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d79c; /* mov.w @r11,r3 */
L8c61d79c: r[11]+=4u;goto L8c61d79e; /* add #0x4,r11 */
L8c61d79e: *fpul=r[3];goto L8c61d7a0; /* lds r3,FPUL */
L8c61d7a0: r[3]=rd(m,bus,&io_status,0x8c61d7ccu,4);if(io_status)return io_status;goto L8c61d7a2; /* mov.l 0x8c61d7cc,r3 */
L8c61d7a2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d7a4; /* float FPUL,fr3 */
L8c61d7a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d7a6; /* fmul fr15,fr3 */
L8c61d7a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d7a8; /* fmov.s fr3,@-r8 */
L8c61d7a8: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61d7aa; /* fmov.s @(r0,r12),fr3 */
L8c61d7aa: r[0]=20u;goto L8c61d7ac; /* mov #0x14,r0 */
L8c61d7ac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d7ae; /* fmov.s fr3,@-r8 */
L8c61d7ae: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61d7b0; /* fmov.s @(r0,r12),fr3 */
L8c61d7b0: r[0]=16u;goto L8c61d7b2; /* mov #0x10,r0 */
L8c61d7b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d7b4; /* fmov.s fr3,@-r8 */
L8c61d7b4: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61d7b6; /* fmov.s @(r0,r12),fr3 */
L8c61d7b6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d7b8; /* fmov.s fr3,@-r8 */
L8c61d7b8: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d7ba; /* mov.l r3,@-r8 */
L8c61d7ba: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d7bc; /* pref @r8 */
L8c61d7bc: r[2]=rd(m,bus,&io_status,0x8c61d7d0u,4);if(io_status)return io_status;goto L8c61d7be; /* mov.l 0x8c61d7d0,r2 */
L8c61d7be: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d7c0; /* mov.l @r2,r3 */
L8c61d7c0: r[3]+=4u;goto L8c61d7c2; /* add #0x4,r3 */
L8c61d7c2: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61d7c4; /* mov.l r3,@r2 */
L8c61d7c4: r[8]+=32u;goto L8c61d8a6; /* bra 0x8c61d8a6 */
L8c61d7f0: r[1]=rd(m,bus,&io_status,0x8c61da2cu,4);if(io_status)return io_status;goto L8c61d7f2; /* mov.l 0x8c61da2c,r1 */
L8c61d7f2: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61d7f4; /* mov.l r2,@r1 */
L8c61d7f4: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d7f6; /* mov.w @r11,r3 */
L8c61d7f6: *fpul=r[3];goto L8c61d7f8; /* lds r3,FPUL */
L8c61d7f8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d7fa; /* float FPUL,fr3 */
L8c61d7fa: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d7fc; /* ftrc fr3,FPUL */
L8c61d7fc: r[2]=*fpul;goto L8c61d7fe; /* sts FPUL,r2 */
L8c61d7fe: wr(m,bus,&io_status,r[9],r[2],2);if(io_status)return io_status;goto L8c61d800; /* mov.w r2,@r9 */
L8c61d800: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d802; /* mov.w @(0x2,r11),r0 */
L8c61d802: r[11]+=4u;goto L8c61d804; /* add #0x4,r11 */
L8c61d804: r[3]=r[0];goto L8c61d806; /* mov r0,r3 */
L8c61d806: *fpul=r[3];goto L8c61d808; /* lds r3,FPUL */
L8c61d808: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d80a; /* float FPUL,fr3 */
L8c61d80a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d80c; /* ftrc fr3,FPUL */
L8c61d80c: r[0]=*fpul;goto L8c61d80e; /* sts FPUL,r0 */
L8c61d80e: wr(m,bus,&io_status,r[9]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d810; /* mov.w r0,@(0x2,r9) */
L8c61d810: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d812; /* mov.w @r11,r2 */
L8c61d812: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61d814; /* mov.l @r15,r3 */
L8c61d814: *fpul=r[2];goto L8c61d816; /* lds r2,FPUL */
L8c61d816: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d818; /* float FPUL,fr3 */
L8c61d818: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d81a; /* ftrc fr3,FPUL */
L8c61d81a: r[1]=*fpul;goto L8c61d81c; /* sts FPUL,r1 */
L8c61d81c: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61d81e; /* mov.w r1,@r3 */
L8c61d81e: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d820; /* mov.w @(0x2,r11),r0 */
L8c61d820: r[11]+=4u;goto L8c61d822; /* add #0x4,r11 */
L8c61d822: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61d824; /* mov.l @r15,r3 */
L8c61d824: r[2]=r[0];goto L8c61d826; /* mov r0,r2 */
L8c61d826: *fpul=r[2];goto L8c61d828; /* lds r2,FPUL */
L8c61d828: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d82a; /* float FPUL,fr3 */
L8c61d82a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d82c; /* ftrc fr3,FPUL */
L8c61d82c: r[0]=*fpul;goto L8c61d82e; /* sts FPUL,r0 */
L8c61d82e: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d830; /* mov.w r0,@(0x2,r3) */
L8c61d830: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d832; /* mov.w @r11,r2 */
L8c61d832: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61d834; /* mov.l @(0x10,r15),r3 */
L8c61d834: *fpul=r[2];goto L8c61d836; /* lds r2,FPUL */
L8c61d836: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d838; /* float FPUL,fr3 */
L8c61d838: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d83a; /* ftrc fr3,FPUL */
L8c61d83a: r[1]=*fpul;goto L8c61d83c; /* sts FPUL,r1 */
L8c61d83c: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61d83e; /* mov.w r1,@r3 */
L8c61d83e: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d840; /* mov.w @(0x2,r11),r0 */
L8c61d840: r[11]+=4u;goto L8c61d842; /* add #0x4,r11 */
L8c61d842: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61d844; /* mov.l @(0x10,r15),r3 */
L8c61d844: r[2]=r[0];goto L8c61d846; /* mov r0,r2 */
L8c61d846: *fpul=r[2];goto L8c61d848; /* lds r2,FPUL */
L8c61d848: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d84a; /* float FPUL,fr3 */
L8c61d84a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d84c; /* ftrc fr3,FPUL */
L8c61d84c: r[0]=*fpul;goto L8c61d84e; /* sts FPUL,r0 */
L8c61d84e: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d850; /* mov.w r0,@(0x2,r3) */
L8c61d850: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d852; /* mov.w @r11,r2 */
L8c61d852: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61d854; /* mov.l @(0x4,r15),r3 */
L8c61d854: *fpul=r[2];goto L8c61d856; /* lds r2,FPUL */
L8c61d856: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d858; /* float FPUL,fr3 */
L8c61d858: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d85a; /* ftrc fr3,FPUL */
L8c61d85a: r[1]=*fpul;goto L8c61d85c; /* sts FPUL,r1 */
L8c61d85c: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61d85e; /* mov.w r1,@r3 */
L8c61d85e: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d860; /* mov.w @(0x2,r11),r0 */
L8c61d860: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61d862; /* mov.l @(0x4,r15),r3 */
L8c61d862: r[2]=r[0];goto L8c61d864; /* mov r0,r2 */
L8c61d864: *fpul=r[2];goto L8c61d866; /* lds r2,FPUL */
L8c61d866: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d868; /* float FPUL,fr3 */
L8c61d868: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61d86a; /* ftrc fr3,FPUL */
L8c61d86a: r[0]=*fpul;goto L8c61d86c; /* sts FPUL,r0 */
L8c61d86c: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61d86e; /* mov.w r0,@(0x2,r3) */
L8c61d86e: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61d870; /* mov.l @(0xc,r14),r3 */
L8c61d870: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61d872; /* mov.l @(0xc,r4),r5 */
L8c61d872: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61d874; /* mov.l @(0xc,r13),r2 */
L8c61d874: r[5]&=r[3];goto L8c61d876; /* and r3,r5 */
L8c61d876: r[3]=1u;goto L8c61d878; /* mov #0x1,r3 */
L8c61d878: r[5]&=r[2];goto L8c61d87a; /* and r2,r5 */
L8c61d87a: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61d87c; /* tst r3,r5 */
L8c61d87c: take=!(*sr&1u);r[11]+=4u;if(take)goto L8c61d88a;goto L8c61d880; /* bf/s 0x8c61d88a */
L8c61d880: r[1]=rd(m,bus,&io_status,0x8c61da30u,4);if(io_status)return io_status;goto L8c61d882; /* mov.l 0x8c61da30,r1 */
L8c61d882: r[6]=r[13];goto L8c61d884; /* mov r13,r6 */
L8c61d884: r[7]=r[9];goto L8c61d886; /* mov r9,r7 */
L8c61d886: target=r[1];*pr=0x8c61d88au;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61d88a; /* jsr @r1 */
L8c61d88a: r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61d88c; /* mov.l @(0xc,r14),r4 */
L8c61d88c: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61d88e; /* mov.l @(0xc,r13),r3 */
L8c61d88e: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61d890; /* mov.l @(0xc,r12),r2 */
L8c61d890: r[4]&=r[3];goto L8c61d892; /* and r3,r4 */
L8c61d892: r[3]=1u;goto L8c61d894; /* mov #0x1,r3 */
L8c61d894: r[4]&=r[2];goto L8c61d896; /* and r2,r4 */
L8c61d896: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61d898; /* tst r3,r4 */
L8c61d898: if(!(*sr&1u))goto L8c61d8a6;goto L8c61d89a; /* bf 0x8c61d8a6 */
L8c61d89a: r[2]=rd(m,bus,&io_status,0x8c61da30u,4);if(io_status)return io_status;goto L8c61d89c; /* mov.l 0x8c61da30,r2 */
L8c61d89c: r[5]=r[13];goto L8c61d89e; /* mov r13,r5 */
L8c61d89e: r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61d8a0; /* mov.l @r15,r7 */
L8c61d8a0: r[6]=r[12];goto L8c61d8a2; /* mov r12,r6 */
L8c61d8a2: target=r[2];*pr=0x8c61d8a6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61d8a6; /* jsr @r2 */
L8c61d8a6: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61d8a8; /* mov.l @(0xc,r15),r3 */
L8c61d8a8: r[3]+=1u;goto L8c61d8aa; /* add #0x1,r3 */
L8c61d8aa: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c61d8ac; /* mov.l r3,@(0xc,r15) */
L8c61d8ac: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61d8ae; /* mov.l @(0x8,r15),r2 */
L8c61d8ae: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61d8b0; /* mov.l @(0xc,r15),r3 */
L8c61d8b0: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61d8b2; /* mov.w @(0x2,r2),r0 */
L8c61d8b2: r[0]=r[0]&65535u;goto L8c61d8b4; /* extu.w r0,r0 */
L8c61d8b4: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61d8b6; /* cmp/ge r0,r3 */
L8c61d8b6: if((*sr&1u))goto L8c61d8bc;goto L8c61d8b8; /* bt 0x8c61d8bc */
L8c61d8b8: goto L8c61d65c; /* bra 0x8c61d65c */
L8c61d8bc: goto L8c61db7c; /* bra 0x8c61db7c */
L8c61d8c0: r[4]=rd(m,bus,&io_status,0x8c61da34u,4);if(io_status)return io_status;goto L8c61d8c2; /* mov.l 0x8c61da34,r4 */
L8c61d8c2: r[2]=0u;goto L8c61d8c4; /* mov #0x0,r2 */
L8c61d8c4: wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c61d8c6; /* mov.l r2,@(0x10,r15) */
L8c61d8c6: r[2]=r[4];goto L8c61d8c8; /* mov r4,r2 */
L8c61d8c8: r[1]=r[4];goto L8c61d8ca; /* mov r4,r1 */
L8c61d8ca: r[2]+=8u;goto L8c61d8cc; /* add #0x8,r2 */
L8c61d8cc: r[4]+=4u;goto L8c61d8ce; /* add #0x4,r4 */
L8c61d8ce: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61d8d0; /* mov.l r2,@(0x4,r15) */
L8c61d8d0: wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c61d8d2; /* mov.l r1,@(0xc,r15) */
L8c61d8d2: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61db22; /* bra 0x8c61db22 */
L8c61d8d6: r[3]=rd(m,bus,&io_status,0x8c61da38u,4);if(io_status)return io_status;goto L8c61d8d8; /* mov.l 0x8c61da38,r3 */
L8c61d8d8: r[5]=0u;goto L8c61d8da; /* mov #0x0,r5 */
L8c61d8da: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d8dc; /* mov.w @r10+,r13 */
L8c61d8dc: r[6]=0u;goto L8c61d8de; /* mov #0x0,r6 */
L8c61d8de: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61d8e0; /* mov.l @r3,r2 */
L8c61d8e0: r[7]=r[5];goto L8c61d8e2; /* mov r5,r7 */
L8c61d8e2: r[13]=r[13]&65535u;goto L8c61d8e4; /* extu.w r13,r13 */
L8c61d8e4: r[2]+=r[13];goto L8c61d8e6; /* add r13,r2 */
L8c61d8e6: *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c61d8e8; /* cmp/pl r13 */
L8c61d8e8: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61d8ea; /* mov.l r2,@r3 */
L8c61d8ea: r[4]=4294967295u;goto L8c61d8ec; /* mov #-0x1,r4 */
L8c61d8ec: take=!(*sr&1u);r[6]+=r[10];if(take)goto L8c61d90e;goto L8c61d8f0; /* bf/s 0x8c61d90e */
L8c61d8f0: r[14]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c61d8f2; /* mov.w @r6+,r14 */
L8c61d8f2: r[7]+=1u;goto L8c61d8f4; /* add #0x1,r7 */
L8c61d8f4: r[2]=rd(m,bus,&io_status,0x8c61da3cu,4);if(io_status)return io_status;goto L8c61d8f6; /* mov.l 0x8c61da3c,r2 */
L8c61d8f6: r[14]=r[14]&65535u;goto L8c61d8f8; /* extu.w r14,r14 */
L8c61d8f8: r[14]<<=2;goto L8c61d8fa; /* shll2 r14 */
L8c61d8fa: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d8fc; /* mov.l @r2,r3 */
L8c61d8fc: r[14]<<=2;goto L8c61d8fe; /* shll2 r14 */
L8c61d8fe: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61d900; /* shll r14 */
L8c61d900: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[13]);goto L8c61d902; /* cmp/ge r13,r7 */
L8c61d902: r[14]+=r[3];goto L8c61d904; /* add r3,r14 */
L8c61d904: wr(m,bus,&io_status,r[15]+0x14u,r[14],4);if(io_status)return io_status;goto L8c61d906; /* mov.l r14,@(0x14,r15) */
L8c61d906: r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61d908; /* mov.l @(0xc,r14),r14 */
L8c61d908: r[4]&=r[14];goto L8c61d90a; /* and r14,r4 */
L8c61d90a: take=!(*sr&1u);r[5]|=r[14];if(take)goto L8c61d8f0;goto L8c61d90e; /* bf/s 0x8c61d8f0 */
L8c61d90e: r[2]=63u;goto L8c61d910; /* mov #0x3f,r2 */
L8c61d910: r[14]=r[13];goto L8c61d912; /* mov r13,r14 */
L8c61d912: r[7]=r[13];goto L8c61d914; /* mov r13,r7 */
L8c61d914: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61d916; /* shll r14 */
L8c61d916: *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61d918; /* tst r2,r4 */
L8c61d918: take=(*sr&1u);r[7]<<=2;if(take)goto L8c61d922;goto L8c61d91c; /* bt/s 0x8c61d922 */
L8c61d91c: r[11]+=r[7];goto L8c61d91e; /* add r7,r11 */
L8c61d91e: r[10]+=r[14];goto L8c61db1c; /* bra 0x8c61db1c */
L8c61d922: r[3]=1u;goto L8c61d924; /* mov #0x1,r3 */
L8c61d924: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61d926; /* tst r3,r5 */
L8c61d926: if((*sr&1u))goto L8c61d92c;goto L8c61d928; /* bt 0x8c61d92c */
L8c61d928: goto L8c61da4c; /* bra 0x8c61da4c */
L8c61d92c: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61d92e; /* mov.w @(0x2,r10),r0 */
L8c61d92e: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61d930; /* mov.w @r10,r4 */
L8c61d930: r[2]=rd(m,bus,&io_status,0x8c61da3cu,4);if(io_status)return io_status;goto L8c61d932; /* mov.l 0x8c61da3c,r2 */
L8c61d932: r[5]=r[0]&65535u;goto L8c61d934; /* extu.w r0,r5 */
L8c61d934: r[5]<<=2;goto L8c61d936; /* shll2 r5 */
L8c61d936: r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61d938; /* mov.w @(0x4,r10),r0 */
L8c61d938: r[4]=r[4]&65535u;goto L8c61d93a; /* extu.w r4,r4 */
L8c61d93a: r[12]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d93c; /* mov.l @r2,r12 */
L8c61d93c: r[4]<<=2;goto L8c61d93e; /* shll2 r4 */
L8c61d93e: r[5]<<=2;goto L8c61d940; /* shll2 r5 */
L8c61d940: r[6]=r[0]&65535u;goto L8c61d942; /* extu.w r0,r6 */
L8c61d942: r[4]<<=2;goto L8c61d944; /* shll2 r4 */
L8c61d944: r[0]=16u;goto L8c61d946; /* mov #0x10,r0 */
L8c61d946: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61d948; /* shll r4 */
L8c61d948: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61d94a; /* shll r5 */
L8c61d94a: r[5]+=r[12];goto L8c61d94c; /* add r12,r5 */
L8c61d94c: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d94e; /* fmov.s @(r0,r5),fr5 */
L8c61d94e: r[6]<<=2;goto L8c61d950; /* shll2 r6 */
L8c61d950: r[4]+=r[12];goto L8c61d952; /* add r12,r4 */
L8c61d952: r[6]<<=2;goto L8c61d954; /* shll2 r6 */
L8c61d954: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d956; /* fmov.s @(r0,r4),fr3 */
L8c61d956: fcopy(f,2u,5u);goto L8c61d958; /* fmov fr5,fr2 */
L8c61d958: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61d95a; /* shll r6 */
L8c61d95a: r[0]=20u;goto L8c61d95c; /* mov #0x14,r0 */
L8c61d95c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61d95e; /* fsub fr3,fr2 */
L8c61d95e: r[6]+=r[12];goto L8c61d960; /* add r12,r6 */
L8c61d960: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61d962; /* fmov.s @(r0,r5),fr4 */
L8c61d962: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d964; /* fmov.s @(r0,r6),fr3 */
L8c61d964: fcopy(f,1u,4u);goto L8c61d966; /* fmov fr4,fr1 */
L8c61d966: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d968; /* fsub fr4,fr3 */
L8c61d968: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61d96a; /* fmul fr3,fr2 */
L8c61d96a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d96c; /* fmov.s @(r0,r4),fr3 */
L8c61d96c: r[0]=16u;goto L8c61d96e; /* mov #0x10,r0 */
L8c61d96e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61d970; /* fsub fr3,fr1 */
L8c61d970: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61d972; /* fmov.s @(r0,r6),fr3 */
L8c61d972: fcopy(f,4u,2u);goto L8c61d974; /* fmov fr2,fr4 */
L8c61d974: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61d976; /* fsub fr5,fr3 */
L8c61d976: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61d978; /* fmul fr3,fr1 */
L8c61d978: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61d97a; /* fldi0 fr3 */
L8c61d97a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61d97c; /* fsub fr1,fr4 */
L8c61d97c: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61d97e; /* fcmp/gt fr3,fr4 */
L8c61d97e: if(!(*sr&1u))goto L8c61d986;goto L8c61d980; /* bf 0x8c61d986 */
L8c61d980: r[10]+=r[14];goto L8c61d982; /* add r14,r10 */
L8c61d982: r[11]+=r[7];goto L8c61db1c; /* bra 0x8c61db1c */
L8c61d986: r[3]=rd(m,bus,&io_status,0x8c61da40u,4);if(io_status)return io_status;goto L8c61d988; /* mov.l 0x8c61da40,r3 */
L8c61d988: r[6]=r[13];goto L8c61d98a; /* mov r13,r6 */
L8c61d98a: r[6]+=4294967295u;goto L8c61d98c; /* add #-0x1,r6 */
L8c61d98c: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61d98e; /* mov.l @r3,r2 */
L8c61d98e: *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c61d990; /* cmp/pl r6 */
L8c61d990: r[2]+=r[13];goto L8c61d992; /* add r13,r2 */
L8c61d992: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61d994; /* mov.l r2,@r3 */
L8c61d994: take=!(*sr&1u);r[5]=0u;if(take)goto L8c61d9e4;goto L8c61d998; /* bf/s 0x8c61d9e4 */
L8c61d998: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d99a; /* mov.w @r10+,r4 */
L8c61d99a: r[8]+=28u;goto L8c61d99c; /* add #0x1c,r8 */
L8c61d99c: r[2]=rd(m,bus,&io_status,0x8c61da3cu,4);if(io_status)return io_status;goto L8c61d99e; /* mov.l 0x8c61da3c,r2 */
L8c61d99e: r[4]=r[4]&65535u;goto L8c61d9a0; /* extu.w r4,r4 */
L8c61d9a0: r[4]<<=2;goto L8c61d9a2; /* shll2 r4 */
L8c61d9a2: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d9a4; /* mov.l @r2,r3 */
L8c61d9a4: r[4]<<=2;goto L8c61d9a6; /* shll2 r4 */
L8c61d9a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d9a8; /* fmov.s fr14,@-r8 */
L8c61d9a8: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d9aa; /* mov.w @(0x2,r11),r0 */
L8c61d9aa: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61d9ac; /* shll r4 */
L8c61d9ac: r[4]+=r[3];goto L8c61d9ae; /* add r3,r4 */
L8c61d9ae: r[3]=r[0];goto L8c61d9b0; /* mov r0,r3 */
L8c61d9b0: *fpul=r[3];goto L8c61d9b2; /* lds r3,FPUL */
L8c61d9b2: r[0]=24u;goto L8c61d9b4; /* mov #0x18,r0 */
L8c61d9b4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d9b6; /* float FPUL,fr3 */
L8c61d9b6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d9b8; /* fmul fr15,fr3 */
L8c61d9b8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d9ba; /* fmov.s fr3,@-r8 */
L8c61d9ba: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61d9bc; /* mov.w @r11,r3 */
L8c61d9bc: r[11]+=4u;goto L8c61d9be; /* add #0x4,r11 */
L8c61d9be: *fpul=r[3];goto L8c61d9c0; /* lds r3,FPUL */
L8c61d9c0: r[3]=rd(m,bus,&io_status,0x8c61da44u,4);if(io_status)return io_status;goto L8c61d9c2; /* mov.l 0x8c61da44,r3 */
L8c61d9c2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61d9c4; /* float FPUL,fr3 */
L8c61d9c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61d9c6; /* fmul fr15,fr3 */
L8c61d9c6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d9c8; /* fmov.s fr3,@-r8 */
L8c61d9c8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d9ca; /* fmov.s @(r0,r4),fr3 */
L8c61d9ca: r[0]=20u;goto L8c61d9cc; /* mov #0x14,r0 */
L8c61d9cc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d9ce; /* fmov.s fr3,@-r8 */
L8c61d9ce: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d9d0; /* fmov.s @(r0,r4),fr3 */
L8c61d9d0: r[0]=16u;goto L8c61d9d2; /* mov #0x10,r0 */
L8c61d9d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d9d4; /* fmov.s fr3,@-r8 */
L8c61d9d4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61d9d6; /* fmov.s @(r0,r4),fr3 */
L8c61d9d6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61d9d8; /* fmov.s fr3,@-r8 */
L8c61d9d8: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61d9da; /* mov.l r3,@-r8 */
L8c61d9da: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61d9dc; /* pref @r8 */
L8c61d9dc: r[5]+=1u;goto L8c61d9de; /* add #0x1,r5 */
L8c61d9de: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61d9e0; /* cmp/ge r6,r5 */
L8c61d9e0: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61d998;goto L8c61d9e4; /* bf/s 0x8c61d998 */
L8c61d9e4: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61d9e6; /* mov.w @r10+,r4 */
L8c61d9e6: r[8]+=28u;goto L8c61d9e8; /* add #0x1c,r8 */
L8c61d9e8: r[2]=rd(m,bus,&io_status,0x8c61da3cu,4);if(io_status)return io_status;goto L8c61d9ea; /* mov.l 0x8c61da3c,r2 */
L8c61d9ea: r[4]=r[4]&65535u;goto L8c61d9ec; /* extu.w r4,r4 */
L8c61d9ec: r[4]<<=2;goto L8c61d9ee; /* shll2 r4 */
L8c61d9ee: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61d9f0; /* mov.l @r2,r3 */
L8c61d9f0: r[4]<<=2;goto L8c61d9f2; /* shll2 r4 */
L8c61d9f2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61d9f4; /* fmov.s fr14,@-r8 */
L8c61d9f4: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61d9f6; /* mov.w @(0x2,r11),r0 */
L8c61d9f6: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61d9f8; /* shll r4 */
L8c61d9f8: r[4]+=r[3];goto L8c61d9fa; /* add r3,r4 */
L8c61d9fa: r[3]=r[0];goto L8c61d9fc; /* mov r0,r3 */
L8c61d9fc: *fpul=r[3];goto L8c61d9fe; /* lds r3,FPUL */
L8c61d9fe: r[0]=24u;goto L8c61da00; /* mov #0x18,r0 */
L8c61da00: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61da02; /* float FPUL,fr3 */
L8c61da02: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61da04; /* fmul fr15,fr3 */
L8c61da04: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61da06; /* fmov.s fr3,@-r8 */
L8c61da06: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61da08; /* mov.w @r11,r3 */
L8c61da08: r[11]+=4u;goto L8c61da0a; /* add #0x4,r11 */
L8c61da0a: *fpul=r[3];goto L8c61da0c; /* lds r3,FPUL */
L8c61da0c: r[3]=rd(m,bus,&io_status,0x8c61da48u,4);if(io_status)return io_status;goto L8c61da0e; /* mov.l 0x8c61da48,r3 */
L8c61da0e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61da10; /* float FPUL,fr3 */
L8c61da10: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61da12; /* fmul fr15,fr3 */
L8c61da12: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61da14; /* fmov.s fr3,@-r8 */
L8c61da14: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61da16; /* fmov.s @(r0,r4),fr3 */
L8c61da16: r[0]=20u;goto L8c61da18; /* mov #0x14,r0 */
L8c61da18: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61da1a; /* fmov.s fr3,@-r8 */
L8c61da1a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61da1c; /* fmov.s @(r0,r4),fr3 */
L8c61da1c: r[0]=16u;goto L8c61da1e; /* mov #0x10,r0 */
L8c61da1e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61da20; /* fmov.s fr3,@-r8 */
L8c61da20: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61da22; /* fmov.s @(r0,r4),fr3 */
L8c61da22: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61da24; /* fmov.s fr3,@-r8 */
L8c61da24: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61da26; /* mov.l r3,@-r8 */
L8c61da26: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61da28; /* pref @r8 */
L8c61da28: r[8]+=32u;goto L8c61db1c; /* bra 0x8c61db1c */
L8c61da4c: r[1]=rd(m,bus,&io_status,0x8c61db3cu,4);if(io_status)return io_status;goto L8c61da4e; /* mov.l 0x8c61db3c,r1 */
L8c61da4e: r[2]=0u;goto L8c61da50; /* mov #0x0,r2 */
L8c61da50: r[0]=rd(m,bus,&io_status,0x8c61db40u,4);if(io_status)return io_status;goto L8c61da52; /* mov.l 0x8c61db40,r0 */
L8c61da52: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61da54; /* mov.l r2,@r1 */
L8c61da54: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61da56; /* mov.w @r10+,r4 */
L8c61da56: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61da58; /* mov.l @r0,r2 */
L8c61da58: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61da5a; /* mov.w @r10+,r14 */
L8c61da5a: r[4]=r[4]&65535u;goto L8c61da5c; /* extu.w r4,r4 */
L8c61da5c: r[4]<<=2;goto L8c61da5e; /* shll2 r4 */
L8c61da5e: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61da60; /* mov.l @(0xc,r15),r3 */
L8c61da60: r[14]=r[14]&65535u;goto L8c61da62; /* extu.w r14,r14 */
L8c61da62: r[14]<<=2;goto L8c61da64; /* shll2 r14 */
L8c61da64: r[14]<<=2;goto L8c61da66; /* shll2 r14 */
L8c61da66: r[4]<<=2;goto L8c61da68; /* shll2 r4 */
L8c61da68: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61da6a; /* shll r4 */
L8c61da6a: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61da6c; /* shll r14 */
L8c61da6c: r[14]+=r[2];goto L8c61da6e; /* add r2,r14 */
L8c61da6e: r[4]+=r[2];goto L8c61da70; /* add r2,r4 */
L8c61da70: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61da72; /* mov.w @r11,r2 */
L8c61da72: *fpul=r[2];goto L8c61da74; /* lds r2,FPUL */
L8c61da74: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61da76; /* float FPUL,fr3 */
L8c61da76: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61da78; /* ftrc fr3,FPUL */
L8c61da78: r[1]=*fpul;goto L8c61da7a; /* sts FPUL,r1 */
L8c61da7a: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61da7c; /* mov.w r1,@r3 */
L8c61da7c: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61da7e; /* mov.w @(0x2,r11),r0 */
L8c61da7e: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61da80; /* mov.l @(0xc,r15),r3 */
L8c61da80: r[2]=r[0];goto L8c61da82; /* mov r0,r2 */
L8c61da82: *fpul=r[2];goto L8c61da84; /* lds r2,FPUL */
L8c61da84: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61da86; /* float FPUL,fr3 */
L8c61da86: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61da88; /* ftrc fr3,FPUL */
L8c61da88: r[0]=*fpul;goto L8c61da8a; /* sts FPUL,r0 */
L8c61da8a: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61da8c; /* mov.w r0,@(0x2,r3) */
L8c61da8c: r[11]+=4u;goto L8c61da8e; /* add #0x4,r11 */
L8c61da8e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61da90; /* mov.l @r15,r3 */
L8c61da90: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61da92; /* mov.w @r11,r2 */
L8c61da92: r[9]=r[13];goto L8c61da94; /* mov r13,r9 */
L8c61da94: r[9]+=4294967294u;goto L8c61da96; /* add #-0x2,r9 */
L8c61da96: *fpul=r[2];goto L8c61da98; /* lds r2,FPUL */
L8c61da98: *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c61da9a; /* cmp/pl r9 */
L8c61da9a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61da9c; /* float FPUL,fr3 */
L8c61da9c: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61da9e; /* ftrc fr3,FPUL */
L8c61da9e: r[1]=*fpul;goto L8c61daa0; /* sts FPUL,r1 */
L8c61daa0: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61daa2; /* mov.w r1,@r3 */
L8c61daa2: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61daa4; /* mov.w @(0x2,r11),r0 */
L8c61daa4: r[11]+=4u;goto L8c61daa6; /* add #0x4,r11 */
L8c61daa6: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61daa8; /* mov.l @r15,r3 */
L8c61daa8: r[2]=r[0];goto L8c61daaa; /* mov r0,r2 */
L8c61daaa: *fpul=r[2];goto L8c61daac; /* lds r2,FPUL */
L8c61daac: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61daae; /* float FPUL,fr3 */
L8c61daae: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61dab0; /* ftrc fr3,FPUL */
L8c61dab0: r[0]=*fpul;goto L8c61dab2; /* sts FPUL,r0 */
L8c61dab2: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61dab4; /* mov.w r0,@(0x2,r3) */
L8c61dab4: take=!(*sr&1u);r[12]=0u;if(take)goto L8c61db1c;goto L8c61dab8; /* bf/s 0x8c61db1c */
L8c61dab8: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61daba; /* mov.w @r10+,r13 */
L8c61daba: r[2]=rd(m,bus,&io_status,0x8c61db40u,4);if(io_status)return io_status;goto L8c61dabc; /* mov.l 0x8c61db40,r2 */
L8c61dabc: r[13]=r[13]&65535u;goto L8c61dabe; /* extu.w r13,r13 */
L8c61dabe: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61dac0; /* mov.l @(0x4,r15),r1 */
L8c61dac0: r[13]<<=2;goto L8c61dac2; /* shll2 r13 */
L8c61dac2: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dac4; /* mov.l @r2,r3 */
L8c61dac4: r[13]<<=2;goto L8c61dac6; /* shll2 r13 */
L8c61dac6: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61dac8; /* shll r13 */
L8c61dac8: r[13]+=r[3];goto L8c61daca; /* add r3,r13 */
L8c61daca: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61dacc; /* mov.w @r11,r3 */
L8c61dacc: *fpul=r[3];goto L8c61dace; /* lds r3,FPUL */
L8c61dace: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61dad0; /* float FPUL,fr3 */
L8c61dad0: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61dad2; /* ftrc fr3,FPUL */
L8c61dad2: r[0]=*fpul;goto L8c61dad4; /* sts FPUL,r0 */
L8c61dad4: wr(m,bus,&io_status,r[1],r[0],2);if(io_status)return io_status;goto L8c61dad6; /* mov.w r0,@r1 */
L8c61dad6: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61dad8; /* mov.w @(0x2,r11),r0 */
L8c61dad8: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61dada; /* mov.l @(0x4,r15),r3 */
L8c61dada: r[1]=r[0];goto L8c61dadc; /* mov r0,r1 */
L8c61dadc: *fpul=r[1];goto L8c61dade; /* lds r1,FPUL */
L8c61dade: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61dae0; /* float FPUL,fr3 */
L8c61dae0: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61dae2; /* ftrc fr3,FPUL */
L8c61dae2: r[0]=*fpul;goto L8c61dae4; /* sts FPUL,r0 */
L8c61dae4: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61dae6; /* mov.w r0,@(0x2,r3) */
L8c61dae6: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61dae8; /* mov.l @(0xc,r4),r5 */
L8c61dae8: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61daea; /* mov.l @(0xc,r14),r3 */
L8c61daea: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61daec; /* mov.l @(0xc,r13),r1 */
L8c61daec: r[5]&=r[3];goto L8c61daee; /* and r3,r5 */
L8c61daee: r[3]=1u;goto L8c61daf0; /* mov #0x1,r3 */
L8c61daf0: r[5]&=r[1];goto L8c61daf2; /* and r1,r5 */
L8c61daf2: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61daf4; /* tst r3,r5 */
L8c61daf4: take=!(*sr&1u);r[11]+=4u;if(take)goto L8c61db02;goto L8c61daf8; /* bf/s 0x8c61db02 */
L8c61daf8: r[1]=rd(m,bus,&io_status,0x8c61db48u,4);if(io_status)return io_status;goto L8c61dafa; /* mov.l 0x8c61db48,r1 */
L8c61dafa: r[6]=r[13];goto L8c61dafc; /* mov r13,r6 */
L8c61dafc: r[7]=rd(m,bus,&io_status,0x8c61db44u,4);if(io_status)return io_status;goto L8c61dafe; /* mov.l 0x8c61db44,r7 */
L8c61dafe: target=r[1];*pr=0x8c61db02u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61db02; /* jsr @r1 */
L8c61db02: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61db04; /* mov.l @r15,r2 */
L8c61db04: r[12]+=1u;goto L8c61db06; /* add #0x1,r12 */
L8c61db06: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61db08; /* mov.l @(0xc,r15),r3 */
L8c61db08: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[9]);goto L8c61db0a; /* cmp/ge r9,r12 */
L8c61db0a: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61db0c; /* mov.l @r2,r1 */
L8c61db0c: r[4]=r[14];goto L8c61db0e; /* mov r14,r4 */
L8c61db0e: wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c61db10; /* mov.l r1,@r3 */
L8c61db10: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61db12; /* mov.l @(0x4,r15),r3 */
L8c61db12: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61db14; /* mov.l @r15,r0 */
L8c61db14: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61db16; /* mov.l @r3,r2 */
L8c61db16: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61db18; /* mov.l r2,@r0 */
L8c61db18: take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61dab8;goto L8c61db1c; /* bf/s 0x8c61dab8 */
L8c61db1c: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61db1e; /* mov.l @(0x10,r15),r2 */
L8c61db1e: r[2]+=1u;goto L8c61db20; /* add #0x1,r2 */
L8c61db20: wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c61db22; /* mov.l r2,@(0x10,r15) */
L8c61db22: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61db24; /* mov.l @(0x8,r15),r3 */
L8c61db24: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61db26; /* mov.w @(0x2,r3),r0 */
L8c61db26: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61db28; /* mov.l @(0x10,r15),r3 */
L8c61db28: r[0]=r[0]&65535u;goto L8c61db2a; /* extu.w r0,r0 */
L8c61db2a: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61db2c; /* cmp/ge r0,r3 */
L8c61db2c: if((*sr&1u))goto L8c61db32;goto L8c61db2e; /* bt 0x8c61db32 */
L8c61db2e: goto L8c61d8d6; /* bra 0x8c61d8d6 */
L8c61db32: goto L8c61db7c; /* bra 0x8c61db7c */
L8c61db4c: r[1]=rd(m,bus,&io_status,0x8c61db5cu,4);if(io_status)return io_status;goto L8c61db4e; /* mov.l 0x8c61db5c,r1 */
L8c61db4e: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61db50; /* mov.l @(0x8,r15),r2 */
L8c61db50: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61db52; /* mov.l r2,@r1 */
L8c61db52: goto L8c61db60; /* bra 0x8c61db60 */
L8c61db60: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61db62; /* frchg */
L8c61db62: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61db64; /* fmov.s fr0,@-r15 */
L8c61db64: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61db66; /* fmov.s fr1,@-r15 */
L8c61db66: r[0]=0x8c61db94u;goto L8c61db68; /* mova 0x8c61db94,r0 */
L8c61db68: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;goto L8c61db6a; /* fmov.s @r0,fr0 */
L8c61db6a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61db6c; /* frchg */
L8c61db6c: r[2]=rd(m,bus,&io_status,0x8c61dc08u,4);if(io_status)return io_status;goto L8c61db6e; /* mov.l 0x8c61dc08,r2 */
L8c61db6e: r[3]=rd(m,bus,&io_status,0x8c61dc04u,4);if(io_status)return io_status;goto L8c61db70; /* mov.l 0x8c61dc04,r3 */
L8c61db70: target=r[2];*pr=0x8c61db74u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61db74; /* jsr @r2 */
L8c61db74: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61db76; /* frchg */
L8c61db76: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61db78; /* fmov.s @r15+,fr1 */
L8c61db78: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61db7a; /* fmov.s @r15+,fr0 */
L8c61db7a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61db7c; /* frchg */
L8c61db7c: r[15]+=24u;goto L8c61db7e; /* add #0x18,r15 */
L8c61db7e: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db80; /* lds.l @r15+,PR */
L8c61db80: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61db82; /* fmov.s @r15+,fr14 */
L8c61db82: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61db84; /* fmov.s @r15+,fr15 */
L8c61db84: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db86; /* mov.l @r15+,r9 */
L8c61db86: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db88; /* mov.l @r15+,r10 */
L8c61db88: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db8a; /* mov.l @r15+,r11 */
L8c61db8a: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db8c; /* mov.l @r15+,r12 */
L8c61db8c: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61db8e; /* mov.l @r15+,r13 */
L8c61db8e: if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
