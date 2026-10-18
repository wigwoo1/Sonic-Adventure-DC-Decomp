/* Mesh material loop and filter-call wrapper; names inferred.
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
SA_STAGE_EXPORT int sa_mesh_material_loop_8c6209a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6209a8;
L8c6209a8: r[15]-=4;wr(m,r[15],r[14],4);goto L8c6209aa; /* mov.l r14,@-r15 */
L8c6209aa: r[15]-=4;wr(m,r[15],r[13],4);goto L8c6209ac; /* mov.l r13,@-r15 */
L8c6209ac: r[15]-=4;wr(m,r[15],r[12],4);goto L8c6209ae; /* mov.l r12,@-r15 */
L8c6209ae: r[15]-=4;wr(m,r[15],r[11],4);goto L8c6209b0; /* mov.l r11,@-r15 */
L8c6209b0: r[15]-=4;wr(m,r[15],r[10],4);goto L8c6209b2; /* mov.l r10,@-r15 */
L8c6209b2: r[15]-=4;wr(m,r[15],r[9],4);goto L8c6209b4; /* mov.l r9,@-r15 */
L8c6209b4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c6209b6; /* fmov.s fr15,@-r15 */
L8c6209b6: r[15]-=4;wr(m,r[15],*pr,4);goto L8c6209b8; /* sts.l PR,@-r15 */
L8c6209b8: r[15]+=4294967284u;goto L8c6209ba; /* add #-0xc,r15 */
L8c6209ba: r[1]=rd(m,0x8c620a20u,4);goto L8c6209bc; /* mov.l 0x8c620a20,r1 */
L8c6209bc: r[9]=r[4];goto L8c6209be; /* mov r4,r9 */
L8c6209be: r[2]=rd(m,r[9]+0x8u,4);goto L8c6209c0; /* mov.l @(0x8,r9),r2 */
L8c6209c0: r[3]=rd(m,r[1],4);goto L8c6209c2; /* mov.l @r1,r3 */
L8c6209c2: r[14]=rd(m,0x8c620a1cu,4);goto L8c6209c4; /* mov.l 0x8c620a1c,r14 */
L8c6209c4: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]);goto L8c6209c6; /* cmp/gt r3,r2 */
L8c6209c6: if(!(*sr&1u))goto L8c6209ce;goto L8c6209c8; /* bf 0x8c6209ce */
L8c6209c8: r[0]=rd(m,0x8c620a24u,4);goto L8c6209ca; /* mov.l 0x8c620a24,r0 */
L8c6209ca: target=r[0];switch(target){case 0x8c620a30u:goto L8c620a30;case 0x8c620efau:goto L8c620efa;case 0x8c620f0au:goto L8c620f0a;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r0 */
L8c6209ce: r[3]=rd(m,0x8c620a28u,4);goto L8c6209d0; /* mov.l 0x8c620a28,r3 */
L8c6209d0: r[12]=rd(m,r[9]+0xcu,4);goto L8c6209d2; /* mov.l @(0xc,r9),r12 */
L8c6209d2: target=r[3];*pr=0x8c6209d6u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6209d6; /* jsr @r3 */
L8c6209d6: *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6209d8; /* tst r0,r0 */
L8c6209d8: if((*sr&1u))goto L8c6209e0;goto L8c6209da; /* bt 0x8c6209e0 */
L8c6209da: r[2]=rd(m,0x8c620a24u,4);goto L8c6209dc; /* mov.l 0x8c620a24,r2 */
L8c6209dc: target=r[2];switch(target){case 0x8c620a30u:goto L8c620a30;case 0x8c620efau:goto L8c620efa;case 0x8c620f0au:goto L8c620f0a;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r2 */
L8c6209e0: r[2]=rd(m,0x8c620a2cu,4);goto L8c6209e2; /* mov.l 0x8c620a2c,r2 */
L8c6209e2: r[3]=0u;goto L8c6209e4; /* mov #0x0,r3 */
L8c6209e4: target=r[2];wr(m,r[15],r[3],4);switch(target){case 0x8c620a30u:goto L8c620a30;case 0x8c620efau:goto L8c620efa;case 0x8c620f0au:goto L8c620f0a;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r2 */
L8c620a30: r[0]=sx(m,r[12],2);goto L8c620a32; /* mov.w @r12,r0 */
L8c620a32: r[4]=sx(m,0x8c620b62u,2);goto L8c620a34; /* mov.w 0x8c620b62,r4 */
L8c620a34: r[0]&=r[4];goto L8c620a36; /* and r4,r0 */
L8c620a36: wr(m,r[15]+0x4u,r[0],2);goto L8c620a38; /* mov.w r0,@(0x4,r15) */
L8c620a38: r[3]=rd(m,r[15],4);goto L8c620a3a; /* mov.l @r15,r3 */
L8c620a3a: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c620a3c; /* tst r3,r3 */
L8c620a3c: if((*sr&1u))goto L8c620a4c;goto L8c620a3e; /* bt 0x8c620a4c */
L8c620a3e: r[0]=sx(m,r[15]+0x4u,2);goto L8c620a40; /* mov.w @(0x4,r15),r0 */
L8c620a40: r[3]=r[11]&65535u;goto L8c620a42; /* extu.w r11,r3 */
L8c620a42: r[0]=r[0]&65535u;goto L8c620a44; /* extu.w r0,r0 */
L8c620a44: *sr=(*sr&~1u)|(r[3]==r[0]);goto L8c620a46; /* cmp/eq r3,r0 */
L8c620a46: if(!(*sr&1u))goto L8c620a4c;goto L8c620a48; /* bf 0x8c620a4c */
L8c620a48: goto L8c620cac; /* bra 0x8c620cac */
L8c620a4c: r[0]=sx(m,r[15]+0x4u,2);goto L8c620a4e; /* mov.w @(0x4,r15),r0 */
L8c620a4e: r[4]=4u;goto L8c620a50; /* mov #0x4,r4 */
L8c620a50: r[13]=rd(m,r[9]+0x10u,4);goto L8c620a52; /* mov.l @(0x10,r9),r13 */
L8c620a52: r[0]=r[0]&65535u;goto L8c620a54; /* extu.w r0,r0 */
L8c620a54: r[10]=rd(m,0x8c620b6cu,4);goto L8c620a56; /* mov.l 0x8c620b6c,r10 */
L8c620a56: r[3]=r[0];goto L8c620a58; /* mov r0,r3 */
L8c620a58: r[0]<<=2;goto L8c620a5a; /* shll2 r0 */
L8c620a5a: r[0]+=r[3];goto L8c620a5c; /* add r3,r0 */
L8c620a5c: r[11]=rd(m,0x8c620b68u,4);goto L8c620a5e; /* mov.l 0x8c620b68,r11 */
L8c620a5e: r[0]<<=2;goto L8c620a60; /* shll2 r0 */
L8c620a60: r[13]+=r[0];goto L8c620a62; /* add r0,r13 */
L8c620a62: r[2]=rd(m,r[13]+0x10u,4);goto L8c620a64; /* mov.l @(0x10,r13),r2 */
L8c620a64: wr(m,r[11],r[2],4);goto L8c620a66; /* mov.l r2,@r11 */
L8c620a66: r[3]=rd(m,r[10],4);goto L8c620a68; /* mov.l @r10,r3 */
L8c620a68: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c620a6a; /* tst r4,r3 */
L8c620a6a: if((*sr&1u))goto L8c620a7e;goto L8c620a6c; /* bt 0x8c620a7e */
L8c620a6c: r[4]=rd(m,0x8c620b70u,4);goto L8c620a6e; /* mov.l 0x8c620b70,r4 */
L8c620a6e: r[2]=rd(m,r[11],4);goto L8c620a70; /* mov.l @r11,r2 */
L8c620a70: r[3]=rd(m,r[4],4);goto L8c620a72; /* mov.l @r4,r3 */
L8c620a72: r[4]=rd(m,0x8c620b74u,4);goto L8c620a74; /* mov.l 0x8c620b74,r4 */
L8c620a74: r[2]&=r[3];goto L8c620a76; /* and r3,r2 */
L8c620a76: wr(m,r[11],r[2],4);goto L8c620a78; /* mov.l r2,@r11 */
L8c620a78: r[3]=rd(m,r[4],4);goto L8c620a7a; /* mov.l @r4,r3 */
L8c620a7a: r[2]|=r[3];goto L8c620a7c; /* or r3,r2 */
L8c620a7c: wr(m,r[11],r[2],4);goto L8c620a7e; /* mov.l r2,@r11 */
L8c620a7e: r[2]=rd(m,r[11],4);goto L8c620a80; /* mov.l @r11,r2 */
L8c620a80: r[0]=0x8c620b78u;goto L8c620a82; /* mova 0x8c620b78,r0 */
L8c620a82: r[3]=rd(m,0x8c620b7cu,4);goto L8c620a84; /* mov.l 0x8c620b7c,r3 */
L8c620a84: *sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c620a86; /* tst r3,r2 */
L8c620a86: take=(*sr&1u);fload(m,f,15u,r[0]);if(take)goto L8c620af8;goto L8c620a8a; /* bt/s 0x8c620af8 */
L8c620a8a: r[1]=rd(m,0x8c620b84u,4);goto L8c620a8c; /* mov.l 0x8c620b84,r1 */
L8c620a8c: r[4]=rd(m,r[13]+0xcu,4);goto L8c620a8e; /* mov.l @(0xc,r13),r4 */
L8c620a8e: r[2]=rd(m,0x8c620b80u,4);goto L8c620a90; /* mov.l 0x8c620b80,r2 */
L8c620a90: target=r[1];*pr=0x8c620a94u;r[4]&=r[2];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620a94; /* jsr @r1 */
L8c620a94: r[3]=rd(m,r[10],4);goto L8c620a96; /* mov.l @r10,r3 */
L8c620a96: r[4]=sx(m,0x8c620b64u,2);goto L8c620a98; /* mov.w 0x8c620b64,r4 */
L8c620a98: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c620a9a; /* tst r4,r3 */
L8c620a9a: if((*sr&1u))goto L8c620abc;goto L8c620a9c; /* bt 0x8c620abc */
L8c620a9c: r[0]=sx(m,r[13]+0x3u,1);goto L8c620a9e; /* mov.b @(0x3,r13),r0 */
L8c620a9e: fcopy(f,4u,15u);goto L8c620aa0; /* fmov fr15,fr4 */
L8c620aa0: r[3]=r[0]&255u;goto L8c620aa2; /* extu.b r0,r3 */
L8c620aa2: *fpul=r[3];goto L8c620aa4; /* lds r3,FPUL */
L8c620aa4: r[0]=16u;goto L8c620aa6; /* mov #0x10,r0 */
L8c620aa6: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620aa8; /* float FPUL,fr3 */
L8c620aa8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620aaa; /* fmul fr4,fr3 */
L8c620aaa: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c620aac; /* fldi1 fr4 */
L8c620aac: fstore(m,f,3u,r[0]+r[14]);goto L8c620aae; /* fmov fr3,@(r0,r14) */
L8c620aae: r[0]=20u;goto L8c620ab0; /* mov #0x14,r0 */
L8c620ab0: fstore(m,f,4u,r[0]+r[14]);goto L8c620ab2; /* fmov fr4,@(r0,r14) */
L8c620ab2: r[0]=24u;goto L8c620ab4; /* mov #0x18,r0 */
L8c620ab4: fstore(m,f,4u,r[0]+r[14]);goto L8c620ab6; /* fmov fr4,@(r0,r14) */
L8c620ab6: r[0]=28u;goto L8c620ab8; /* mov #0x1c,r0 */
L8c620ab8: fstore(m,f,4u,r[0]+r[14]);goto L8c620b32; /* bra 0x8c620b32 */
L8c620abc: r[0]=sx(m,r[13]+0x2u,1);goto L8c620abe; /* mov.b @(0x2,r13),r0 */
L8c620abe: fcopy(f,4u,15u);goto L8c620ac0; /* fmov fr15,fr4 */
L8c620ac0: r[3]=r[0]&255u;goto L8c620ac2; /* extu.b r0,r3 */
L8c620ac2: *fpul=r[3];goto L8c620ac4; /* lds r3,FPUL */
L8c620ac4: r[0]=20u;goto L8c620ac6; /* mov #0x14,r0 */
L8c620ac6: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620ac8; /* float FPUL,fr3 */
L8c620ac8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620aca; /* fmul fr4,fr3 */
L8c620aca: fstore(m,f,3u,r[0]+r[14]);goto L8c620acc; /* fmov fr3,@(r0,r14) */
L8c620acc: r[0]=sx(m,r[13]+0x1u,1);goto L8c620ace; /* mov.b @(0x1,r13),r0 */
L8c620ace: r[3]=r[0]&255u;goto L8c620ad0; /* extu.b r0,r3 */
L8c620ad0: *fpul=r[3];goto L8c620ad2; /* lds r3,FPUL */
L8c620ad2: r[0]=24u;goto L8c620ad4; /* mov #0x18,r0 */
L8c620ad4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620ad6; /* float FPUL,fr3 */
L8c620ad6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620ad8; /* fmul fr4,fr3 */
L8c620ad8: fstore(m,f,3u,r[0]+r[14]);goto L8c620ada; /* fmov fr3,@(r0,r14) */
L8c620ada: r[0]=28u;goto L8c620adc; /* mov #0x1c,r0 */
L8c620adc: r[3]=sx(m,r[13],1);goto L8c620ade; /* mov.b @r13,r3 */
L8c620ade: r[3]=r[3]&255u;goto L8c620ae0; /* extu.b r3,r3 */
L8c620ae0: *fpul=r[3];goto L8c620ae2; /* lds r3,FPUL */
L8c620ae2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620ae4; /* float FPUL,fr3 */
L8c620ae4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620ae6; /* fmul fr4,fr3 */
L8c620ae6: fstore(m,f,3u,r[0]+r[14]);goto L8c620ae8; /* fmov fr3,@(r0,r14) */
L8c620ae8: r[0]=sx(m,r[13]+0x3u,1);goto L8c620aea; /* mov.b @(0x3,r13),r0 */
L8c620aea: r[3]=r[0]&255u;goto L8c620aec; /* extu.b r0,r3 */
L8c620aec: *fpul=r[3];goto L8c620aee; /* lds r3,FPUL */
L8c620aee: r[0]=16u;goto L8c620af0; /* mov #0x10,r0 */
L8c620af0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620af2; /* float FPUL,fr3 */
L8c620af2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620af4; /* fmul fr4,fr3 */
L8c620af4: fstore(m,f,3u,r[0]+r[14]);goto L8c620b32; /* bra 0x8c620b32 */
L8c620af8: r[0]=sx(m,r[13]+0x2u,1);goto L8c620afa; /* mov.b @(0x2,r13),r0 */
L8c620afa: fcopy(f,4u,15u);goto L8c620afc; /* fmov fr15,fr4 */
L8c620afc: r[2]=r[0]&255u;goto L8c620afe; /* extu.b r0,r2 */
L8c620afe: *fpul=r[2];goto L8c620b00; /* lds r2,FPUL */
L8c620b00: r[0]=20u;goto L8c620b02; /* mov #0x14,r0 */
L8c620b02: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620b04; /* float FPUL,fr3 */
L8c620b04: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620b06; /* fmul fr4,fr3 */
L8c620b06: fstore(m,f,3u,r[0]+r[14]);goto L8c620b08; /* fmov fr3,@(r0,r14) */
L8c620b08: r[0]=sx(m,r[13]+0x1u,1);goto L8c620b0a; /* mov.b @(0x1,r13),r0 */
L8c620b0a: r[2]=r[0]&255u;goto L8c620b0c; /* extu.b r0,r2 */
L8c620b0c: *fpul=r[2];goto L8c620b0e; /* lds r2,FPUL */
L8c620b0e: r[0]=24u;goto L8c620b10; /* mov #0x18,r0 */
L8c620b10: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620b12; /* float FPUL,fr3 */
L8c620b12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620b14; /* fmul fr4,fr3 */
L8c620b14: fstore(m,f,3u,r[0]+r[14]);goto L8c620b16; /* fmov fr3,@(r0,r14) */
L8c620b16: r[0]=28u;goto L8c620b18; /* mov #0x1c,r0 */
L8c620b18: r[2]=sx(m,r[13],1);goto L8c620b1a; /* mov.b @r13,r2 */
L8c620b1a: r[2]=r[2]&255u;goto L8c620b1c; /* extu.b r2,r2 */
L8c620b1c: *fpul=r[2];goto L8c620b1e; /* lds r2,FPUL */
L8c620b1e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620b20; /* float FPUL,fr3 */
L8c620b20: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620b22; /* fmul fr4,fr3 */
L8c620b22: fstore(m,f,3u,r[0]+r[14]);goto L8c620b24; /* fmov fr3,@(r0,r14) */
L8c620b24: r[0]=sx(m,r[13]+0x3u,1);goto L8c620b26; /* mov.b @(0x3,r13),r0 */
L8c620b26: r[2]=r[0]&255u;goto L8c620b28; /* extu.b r0,r2 */
L8c620b28: *fpul=r[2];goto L8c620b2a; /* lds r2,FPUL */
L8c620b2a: r[0]=16u;goto L8c620b2c; /* mov #0x10,r0 */
L8c620b2c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620b2e; /* float FPUL,fr3 */
L8c620b2e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620b30; /* fmul fr4,fr3 */
L8c620b30: fstore(m,f,3u,r[0]+r[14]);goto L8c620b32; /* fmov fr3,@(r0,r14) */
L8c620b32: r[5]=rd(m,r[10],4);goto L8c620b34; /* mov.l @r10,r5 */
L8c620b34: r[6]=16u;goto L8c620b36; /* mov #0x10,r6 */
L8c620b36: r[4]=rd(m,0x8c620b88u,4);goto L8c620b38; /* mov.l 0x8c620b88,r4 */
L8c620b38: r[3]=r[5];goto L8c620b3a; /* mov r5,r3 */
L8c620b3a: *sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c620b3c; /* tst r6,r3 */
L8c620b3c: take=(*sr&1u);fload(m,f,4u,r[4]);if(take)goto L8c620b8c;goto L8c620b40; /* bt/s 0x8c620b8c */
L8c620b40: r[0]=16u;goto L8c620b42; /* mov #0x10,r0 */
L8c620b42: fstore(m,f,4u,r[0]+r[14]);goto L8c620b44; /* fmov fr4,@(r0,r14) */
L8c620b44: r[0]=4u;goto L8c620b46; /* mov #0x4,r0 */
L8c620b46: fload(m,f,3u,r[0]+r[4]);goto L8c620b48; /* fmov.s @(r0,r4),fr3 */
L8c620b48: r[0]=20u;goto L8c620b4a; /* mov #0x14,r0 */
L8c620b4a: fstore(m,f,3u,r[0]+r[14]);goto L8c620b4c; /* fmov fr3,@(r0,r14) */
L8c620b4c: r[0]=8u;goto L8c620b4e; /* mov #0x8,r0 */
L8c620b4e: fload(m,f,3u,r[0]+r[4]);goto L8c620b50; /* fmov.s @(r0,r4),fr3 */
L8c620b50: r[0]=24u;goto L8c620b52; /* mov #0x18,r0 */
L8c620b52: fstore(m,f,3u,r[0]+r[14]);goto L8c620b54; /* fmov fr3,@(r0,r14) */
L8c620b54: r[0]=12u;goto L8c620b56; /* mov #0xc,r0 */
L8c620b56: fload(m,f,3u,r[0]+r[4]);goto L8c620b58; /* fmov.s @(r0,r4),fr3 */
L8c620b58: r[0]=28u;goto L8c620b5a; /* mov #0x1c,r0 */
L8c620b5a: fstore(m,f,3u,r[0]+r[14]);goto L8c620bc4; /* bra 0x8c620bc4 */
L8c620b8c: r[6]=32u;goto L8c620b8e; /* mov #0x20,r6 */
L8c620b8e: *sr=(*sr&~1u)|!(r[6]&r[5]);goto L8c620b90; /* tst r6,r5 */
L8c620b90: if((*sr&1u))goto L8c620bc4;goto L8c620b92; /* bt 0x8c620bc4 */
L8c620b92: r[0]=16u;goto L8c620b94; /* mov #0x10,r0 */
L8c620b94: fload(m,f,3u,r[0]+r[14]);goto L8c620b96; /* fmov.s @(r0,r14),fr3 */
L8c620b96: r[1]=4u;goto L8c620b98; /* mov #0x4,r1 */
L8c620b98: r[1]+=r[4];goto L8c620b9a; /* add r4,r1 */
L8c620b9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c620b9c; /* fadd fr4,fr3 */
L8c620b9c: fstore(m,f,3u,r[0]+r[14]);goto L8c620b9e; /* fmov fr3,@(r0,r14) */
L8c620b9e: r[0]=20u;goto L8c620ba0; /* mov #0x14,r0 */
L8c620ba0: fload(m,f,2u,r[0]+r[14]);goto L8c620ba2; /* fmov.s @(r0,r14),fr2 */
L8c620ba2: fload(m,f,3u,r[1]);goto L8c620ba4; /* fmov.s @r1,fr3 */
L8c620ba4: r[1]=8u;goto L8c620ba6; /* mov #0x8,r1 */
L8c620ba6: r[1]+=r[4];goto L8c620ba8; /* add r4,r1 */
L8c620ba8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c620baa; /* fadd fr3,fr2 */
L8c620baa: fstore(m,f,2u,r[0]+r[14]);goto L8c620bac; /* fmov fr2,@(r0,r14) */
L8c620bac: r[0]=24u;goto L8c620bae; /* mov #0x18,r0 */
L8c620bae: fload(m,f,2u,r[0]+r[14]);goto L8c620bb0; /* fmov.s @(r0,r14),fr2 */
L8c620bb0: fload(m,f,3u,r[1]);goto L8c620bb2; /* fmov.s @r1,fr3 */
L8c620bb2: r[1]=12u;goto L8c620bb4; /* mov #0xc,r1 */
L8c620bb4: r[1]+=r[4];goto L8c620bb6; /* add r4,r1 */
L8c620bb6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c620bb8; /* fadd fr3,fr2 */
L8c620bb8: fstore(m,f,2u,r[0]+r[14]);goto L8c620bba; /* fmov fr2,@(r0,r14) */
L8c620bba: r[0]=28u;goto L8c620bbc; /* mov #0x1c,r0 */
L8c620bbc: fload(m,f,2u,r[0]+r[14]);goto L8c620bbe; /* fmov.s @(r0,r14),fr2 */
L8c620bbe: fload(m,f,3u,r[1]);goto L8c620bc0; /* fmov.s @r1,fr3 */
L8c620bc0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c620bc2; /* fadd fr3,fr2 */
L8c620bc2: fstore(m,f,2u,r[0]+r[14]);goto L8c620bc4; /* fmov fr2,@(r0,r14) */
L8c620bc4: r[0]=sx(m,r[13]+0x6u,1);goto L8c620bc6; /* mov.b @(0x6,r13),r0 */
L8c620bc6: fcopy(f,4u,15u);goto L8c620bc8; /* fmov fr15,fr4 */
L8c620bc8: r[3]=r[0]&255u;goto L8c620bca; /* extu.b r0,r3 */
L8c620bca: r[4]=rd(m,0x8c620cb0u,4);goto L8c620bcc; /* mov.l 0x8c620cb0,r4 */
L8c620bcc: *fpul=r[3];goto L8c620bce; /* lds r3,FPUL */
L8c620bce: r[0]=36u;goto L8c620bd0; /* mov #0x24,r0 */
L8c620bd0: r[10]=rd(m,0x8c620cb4u,4);goto L8c620bd2; /* mov.l 0x8c620cb4,r10 */
L8c620bd2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620bd4; /* float FPUL,fr3 */
L8c620bd4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620bd6; /* fmul fr4,fr3 */
L8c620bd6: fstore(m,f,3u,r[0]+r[14]);goto L8c620bd8; /* fmov fr3,@(r0,r14) */
L8c620bd8: r[0]=sx(m,r[13]+0x5u,1);goto L8c620bda; /* mov.b @(0x5,r13),r0 */
L8c620bda: r[3]=r[0]&255u;goto L8c620bdc; /* extu.b r0,r3 */
L8c620bdc: *fpul=r[3];goto L8c620bde; /* lds r3,FPUL */
L8c620bde: r[0]=40u;goto L8c620be0; /* mov #0x28,r0 */
L8c620be0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620be2; /* float FPUL,fr3 */
L8c620be2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620be4; /* fmul fr4,fr3 */
L8c620be4: fstore(m,f,3u,r[0]+r[14]);goto L8c620be6; /* fmov fr3,@(r0,r14) */
L8c620be6: r[0]=sx(m,r[13]+0x4u,1);goto L8c620be8; /* mov.b @(0x4,r13),r0 */
L8c620be8: r[3]=r[0]&255u;goto L8c620bea; /* extu.b r0,r3 */
L8c620bea: *fpul=r[3];goto L8c620bec; /* lds r3,FPUL */
L8c620bec: r[0]=44u;goto L8c620bee; /* mov #0x2c,r0 */
L8c620bee: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620bf0; /* float FPUL,fr3 */
L8c620bf0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620bf2; /* fmul fr4,fr3 */
L8c620bf2: fstore(m,f,3u,r[0]+r[14]);goto L8c620bf4; /* fmov fr3,@(r0,r14) */
L8c620bf4: r[0]=8u;goto L8c620bf6; /* mov #0x8,r0 */
L8c620bf6: fload(m,f,3u,r[0]+r[13]);goto L8c620bf8; /* fmov.s @(r0,r13),fr3 */
L8c620bf8: r[0]=48u;goto L8c620bfa; /* mov #0x30,r0 */
L8c620bfa: fstore(m,f,3u,r[0]+r[14]);goto L8c620bfc; /* fmov fr3,@(r0,r14) */
L8c620bfc: r[3]=rd(m,r[11],4);goto L8c620bfe; /* mov.l @r11,r3 */
L8c620bfe: r[3]&=r[4];goto L8c620c00; /* and r4,r3 */
L8c620c00: wr(m,r[15]+0x8u,r[3],4);goto L8c620c02; /* mov.l r3,@(0x8,r15) */
L8c620c02: target=r[10];*pr=0x8c620c06u;r[4]=rd(m,r[11],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c06; /* jsr @r10 */
L8c620c06: r[5]=rd(m,r[11],4);goto L8c620c08; /* mov.l @r11,r5 */
L8c620c08: r[3]=rd(m,0x8c620cc8u,4);goto L8c620c0a; /* mov.l 0x8c620cc8,r3 */
L8c620c0a: r[2]=r[5];goto L8c620c0c; /* mov r5,r2 */
L8c620c0c: r[10]=rd(m,0x8c620cc4u,4);goto L8c620c0e; /* mov.l 0x8c620cc4,r10 */
L8c620c0e: r[7]=rd(m,0x8c620cb8u,4);goto L8c620c10; /* mov.l 0x8c620cb8,r7 */
L8c620c10: *sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c620c12; /* tst r3,r2 */
L8c620c12: r[6]=rd(m,0x8c620cbcu,4);goto L8c620c14; /* mov.l 0x8c620cbc,r6 */
L8c620c14: r[4]=rd(m,0x8c620cc0u,4);goto L8c620c16; /* mov.l 0x8c620cc0,r4 */
L8c620c16: if((*sr&1u))goto L8c620c62;goto L8c620c18; /* bt 0x8c620c62 */
L8c620c18: *sr=(*sr&~1u)|!(r[5]&r[6]);goto L8c620c1a; /* tst r5,r6 */
L8c620c1a: if((*sr&1u))goto L8c620c24;goto L8c620c1c; /* bt 0x8c620c24 */
L8c620c1c: *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c1e; /* tst r4,r5 */
L8c620c1e: if(!(*sr&1u))goto L8c620c2c;goto L8c620c20; /* bf 0x8c620c2c */
L8c620c20: goto L8c620c36; /* bra 0x8c620c36 */
L8c620c24: *sr=(*sr&~1u)|!(r[5]&r[7]);goto L8c620c26; /* tst r5,r7 */
L8c620c26: if((*sr&1u))goto L8c620c40;goto L8c620c28; /* bt 0x8c620c40 */
L8c620c28: *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c2a; /* tst r4,r5 */
L8c620c2a: if((*sr&1u))goto L8c620c36;goto L8c620c2c; /* bt 0x8c620c36 */
L8c620c2c: r[11]=rd(m,0x8c620cccu,4);goto L8c620c2e; /* mov.l 0x8c620ccc,r11 */
L8c620c2e: target=r[10];*pr=0x8c620c32u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c32; /* jsr @r10 */
L8c620c32: goto L8c620c4a; /* bra 0x8c620c4a */
L8c620c36: r[11]=rd(m,0x8c620cd0u,4);goto L8c620c38; /* mov.l 0x8c620cd0,r11 */
L8c620c38: target=r[10];*pr=0x8c620c3cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c3c; /* jsr @r10 */
L8c620c3c: goto L8c620c56; /* bra 0x8c620c56 */
L8c620c40: *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c42; /* tst r4,r5 */
L8c620c42: if((*sr&1u))goto L8c620c50;goto L8c620c44; /* bt 0x8c620c50 */
L8c620c44: r[11]=rd(m,0x8c620cd4u,4);goto L8c620c46; /* mov.l 0x8c620cd4,r11 */
L8c620c46: target=r[10];*pr=0x8c620c4au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c4a; /* jsr @r10 */
L8c620c4a: r[11]=rd(m,0x8c620cd8u,4);goto L8c620c4c; /* mov.l 0x8c620cd8,r11 */
L8c620c4c: goto L8c620c58; /* bra 0x8c620c58 */
L8c620c50: r[11]=rd(m,0x8c620cdcu,4);goto L8c620c52; /* mov.l 0x8c620cdc,r11 */
L8c620c52: target=r[10];*pr=0x8c620c56u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c56; /* jsr @r10 */
L8c620c56: r[11]=rd(m,0x8c620ce0u,4);goto L8c620c58; /* mov.l 0x8c620ce0,r11 */
L8c620c58: r[4]=r[14];goto L8c620c5a; /* mov r14,r4 */
L8c620c5a: target=r[11];*pr=0x8c620c5eu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c5e; /* jsr @r11 */
L8c620c5e: r[8]=r[0];goto L8c620ca8; /* bra 0x8c620ca8 */
L8c620c62: *sr=(*sr&~1u)|!(r[5]&r[6]);goto L8c620c64; /* tst r5,r6 */
L8c620c64: if((*sr&1u))goto L8c620c6e;goto L8c620c66; /* bt 0x8c620c6e */
L8c620c66: *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c68; /* tst r4,r5 */
L8c620c68: if(!(*sr&1u))goto L8c620c76;goto L8c620c6a; /* bf 0x8c620c76 */
L8c620c6a: goto L8c620c80; /* bra 0x8c620c80 */
L8c620c6e: *sr=(*sr&~1u)|!(r[5]&r[7]);goto L8c620c70; /* tst r5,r7 */
L8c620c70: if((*sr&1u))goto L8c620c8a;goto L8c620c72; /* bt 0x8c620c8a */
L8c620c72: *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c74; /* tst r4,r5 */
L8c620c74: if((*sr&1u))goto L8c620c80;goto L8c620c76; /* bt 0x8c620c80 */
L8c620c76: r[11]=rd(m,0x8c620ce4u,4);goto L8c620c78; /* mov.l 0x8c620ce4,r11 */
L8c620c78: target=r[10];*pr=0x8c620c7cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c7c; /* jsr @r10 */
L8c620c7c: goto L8c620c94; /* bra 0x8c620c94 */
L8c620c80: r[11]=rd(m,0x8c620ce8u,4);goto L8c620c82; /* mov.l 0x8c620ce8,r11 */
L8c620c82: target=r[10];*pr=0x8c620c86u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c86; /* jsr @r10 */
L8c620c86: goto L8c620ca0; /* bra 0x8c620ca0 */
L8c620c8a: *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c8c; /* tst r4,r5 */
L8c620c8c: if((*sr&1u))goto L8c620c9a;goto L8c620c8e; /* bt 0x8c620c9a */
L8c620c8e: r[11]=rd(m,0x8c620cecu,4);goto L8c620c90; /* mov.l 0x8c620cec,r11 */
L8c620c90: target=r[10];*pr=0x8c620c94u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c94; /* jsr @r10 */
L8c620c94: r[11]=rd(m,0x8c620cf0u,4);goto L8c620c96; /* mov.l 0x8c620cf0,r11 */
L8c620c96: goto L8c620ca2; /* bra 0x8c620ca2 */
L8c620c9a: r[11]=rd(m,0x8c620cf4u,4);goto L8c620c9c; /* mov.l 0x8c620cf4,r11 */
L8c620c9c: target=r[10];*pr=0x8c620ca0u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620ca0; /* jsr @r10 */
L8c620ca0: r[11]=rd(m,0x8c620cf8u,4);goto L8c620ca2; /* mov.l 0x8c620cf8,r11 */
L8c620ca2: target=r[11];*pr=0x8c620ca6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620ca6; /* jsr @r11 */
L8c620ca6: r[8]=r[0];goto L8c620ca8; /* mov r0,r8 */
L8c620ca8: r[0]=sx(m,r[15]+0x4u,2);goto L8c620caa; /* mov.w @(0x4,r15),r0 */
L8c620caa: r[11]=r[0];goto L8c620cac; /* mov r0,r11 */
L8c620cac: goto L8c620cfc; /* bra 0x8c620cfc */
L8c620cfc: r[4]=rd(m,0x8c620e74u,4);goto L8c620cfe; /* mov.l 0x8c620e74,r4 */
L8c620cfe: r[2]=rd(m,r[4],4);goto L8c620d00; /* mov.l @r4,r2 */
L8c620d00: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c620d02; /* tst r2,r2 */
L8c620d02: if(!(*sr&1u))goto L8c620dbc;goto L8c620d04; /* bf 0x8c620dbc */
L8c620d04: r[0]=rd(m,r[15]+0x8u,4);goto L8c620d06; /* mov.l @(0x8,r15),r0 */
L8c620d06: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c620d08; /* cmp/eq #0x0,r0 */
L8c620d08: if((*sr&1u))goto L8c620d50;goto L8c620d0a; /* bt 0x8c620d50 */
L8c620d0a: r[1]=rd(m,0x8c620e78u,4);goto L8c620d0c; /* mov.l 0x8c620e78,r1 */
L8c620d0c: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d0e; /* cmp/eq r1,r0 */
L8c620d0e: if((*sr&1u))goto L8c620d5c;goto L8c620d10; /* bt 0x8c620d5c */
L8c620d10: r[1]=rd(m,0x8c620e7cu,4);goto L8c620d12; /* mov.l 0x8c620e7c,r1 */
L8c620d12: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d14; /* cmp/eq r1,r0 */
L8c620d14: if((*sr&1u))goto L8c620d68;goto L8c620d16; /* bt 0x8c620d68 */
L8c620d16: r[1]=rd(m,0x8c620e80u,4);goto L8c620d18; /* mov.l 0x8c620e80,r1 */
L8c620d18: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d1a; /* cmp/eq r1,r0 */
L8c620d1a: if((*sr&1u))goto L8c620d74;goto L8c620d1c; /* bt 0x8c620d74 */
L8c620d1c: r[1]=rd(m,0x8c620e84u,4);goto L8c620d1e; /* mov.l 0x8c620e84,r1 */
L8c620d1e: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d20; /* cmp/eq r1,r0 */
L8c620d20: if((*sr&1u))goto L8c620d80;goto L8c620d22; /* bt 0x8c620d80 */
L8c620d22: r[1]=rd(m,0x8c620e88u,4);goto L8c620d24; /* mov.l 0x8c620e88,r1 */
L8c620d24: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d26; /* cmp/eq r1,r0 */
L8c620d26: if((*sr&1u))goto L8c620d8c;goto L8c620d28; /* bt 0x8c620d8c */
L8c620d28: r[1]=rd(m,0x8c620e8cu,4);goto L8c620d2a; /* mov.l 0x8c620e8c,r1 */
L8c620d2a: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d2c; /* cmp/eq r1,r0 */
L8c620d2c: if((*sr&1u))goto L8c620d8c;goto L8c620d2e; /* bt 0x8c620d8c */
L8c620d2e: r[1]=rd(m,0x8c620e90u,4);goto L8c620d30; /* mov.l 0x8c620e90,r1 */
L8c620d30: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d32; /* cmp/eq r1,r0 */
L8c620d32: if((*sr&1u))goto L8c620d98;goto L8c620d34; /* bt 0x8c620d98 */
L8c620d34: r[1]=rd(m,0x8c620e94u,4);goto L8c620d36; /* mov.l 0x8c620e94,r1 */
L8c620d36: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d38; /* cmp/eq r1,r0 */
L8c620d38: if((*sr&1u))goto L8c620da4;goto L8c620d3a; /* bt 0x8c620da4 */
L8c620d3a: r[1]=rd(m,0x8c620e98u,4);goto L8c620d3c; /* mov.l 0x8c620e98,r1 */
L8c620d3c: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d3e; /* cmp/eq r1,r0 */
L8c620d3e: if((*sr&1u))goto L8c620da4;goto L8c620d40; /* bt 0x8c620da4 */
L8c620d40: r[1]=rd(m,0x8c620e9cu,4);goto L8c620d42; /* mov.l 0x8c620e9c,r1 */
L8c620d42: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d44; /* cmp/eq r1,r0 */
L8c620d44: if((*sr&1u))goto L8c620db0;goto L8c620d46; /* bt 0x8c620db0 */
L8c620d46: r[1]=rd(m,0x8c620ea0u,4);goto L8c620d48; /* mov.l 0x8c620ea0,r1 */
L8c620d48: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d4a; /* cmp/eq r1,r0 */
L8c620d4a: if((*sr&1u))goto L8c620db0;goto L8c620d4c; /* bt 0x8c620db0 */
L8c620d4c: goto L8c620e70; /* bra 0x8c620e70 */
L8c620d50: r[3]=rd(m,0x8c620ea4u,4);goto L8c620d52; /* mov.l 0x8c620ea4,r3 */
L8c620d52: r[5]=r[13];goto L8c620d54; /* mov r13,r5 */
L8c620d54: target=r[3];*pr=0x8c620d58u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d58; /* jsr @r3 */
L8c620d58: goto L8c620e70; /* bra 0x8c620e70 */
L8c620d5c: r[3]=rd(m,0x8c620ea8u,4);goto L8c620d5e; /* mov.l 0x8c620ea8,r3 */
L8c620d5e: r[5]=r[13];goto L8c620d60; /* mov r13,r5 */
L8c620d60: target=r[3];*pr=0x8c620d64u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d64; /* jsr @r3 */
L8c620d64: goto L8c620e70; /* bra 0x8c620e70 */
L8c620d68: r[3]=rd(m,0x8c620eacu,4);goto L8c620d6a; /* mov.l 0x8c620eac,r3 */
L8c620d6a: r[5]=r[13];goto L8c620d6c; /* mov r13,r5 */
L8c620d6c: target=r[3];*pr=0x8c620d70u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d70; /* jsr @r3 */
L8c620d70: goto L8c620e70; /* bra 0x8c620e70 */
L8c620d74: r[3]=rd(m,0x8c620eb0u,4);goto L8c620d76; /* mov.l 0x8c620eb0,r3 */
L8c620d76: r[5]=r[13];goto L8c620d78; /* mov r13,r5 */
L8c620d78: target=r[3];*pr=0x8c620d7cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d7c; /* jsr @r3 */
L8c620d7c: goto L8c620e70; /* bra 0x8c620e70 */
L8c620d80: r[3]=rd(m,0x8c620eb4u,4);goto L8c620d82; /* mov.l 0x8c620eb4,r3 */
L8c620d82: r[5]=r[13];goto L8c620d84; /* mov r13,r5 */
L8c620d84: target=r[3];*pr=0x8c620d88u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d88; /* jsr @r3 */
L8c620d88: goto L8c620e70; /* bra 0x8c620e70 */
L8c620d8c: r[3]=rd(m,0x8c620eb8u,4);goto L8c620d8e; /* mov.l 0x8c620eb8,r3 */
L8c620d8e: r[5]=r[13];goto L8c620d90; /* mov r13,r5 */
L8c620d90: target=r[3];*pr=0x8c620d94u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d94; /* jsr @r3 */
L8c620d94: goto L8c620e70; /* bra 0x8c620e70 */
L8c620d98: r[3]=rd(m,0x8c620ebcu,4);goto L8c620d9a; /* mov.l 0x8c620ebc,r3 */
L8c620d9a: r[5]=r[13];goto L8c620d9c; /* mov r13,r5 */
L8c620d9c: target=r[3];*pr=0x8c620da0u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620da0; /* jsr @r3 */
L8c620da0: goto L8c620e70; /* bra 0x8c620e70 */
L8c620da4: r[3]=rd(m,0x8c620ec0u,4);goto L8c620da6; /* mov.l 0x8c620ec0,r3 */
L8c620da6: r[5]=r[13];goto L8c620da8; /* mov r13,r5 */
L8c620da8: target=r[3];*pr=0x8c620dacu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620dac; /* jsr @r3 */
L8c620dac: goto L8c620e70; /* bra 0x8c620e70 */
L8c620db0: r[3]=rd(m,0x8c620ec4u,4);goto L8c620db2; /* mov.l 0x8c620ec4,r3 */
L8c620db2: r[5]=r[13];goto L8c620db4; /* mov r13,r5 */
L8c620db4: target=r[3];*pr=0x8c620db8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620db8; /* jsr @r3 */
L8c620db8: goto L8c620e70; /* bra 0x8c620e70 */
L8c620dbc: r[0]=rd(m,r[15]+0x8u,4);goto L8c620dbe; /* mov.l @(0x8,r15),r0 */
L8c620dbe: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c620dc0; /* cmp/eq #0x0,r0 */
L8c620dc0: if((*sr&1u))goto L8c620e08;goto L8c620dc2; /* bt 0x8c620e08 */
L8c620dc2: r[1]=rd(m,0x8c620e78u,4);goto L8c620dc4; /* mov.l 0x8c620e78,r1 */
L8c620dc4: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dc6; /* cmp/eq r1,r0 */
L8c620dc6: if((*sr&1u))goto L8c620e14;goto L8c620dc8; /* bt 0x8c620e14 */
L8c620dc8: r[1]=rd(m,0x8c620e7cu,4);goto L8c620dca; /* mov.l 0x8c620e7c,r1 */
L8c620dca: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dcc; /* cmp/eq r1,r0 */
L8c620dcc: if((*sr&1u))goto L8c620e20;goto L8c620dce; /* bt 0x8c620e20 */
L8c620dce: r[1]=rd(m,0x8c620e80u,4);goto L8c620dd0; /* mov.l 0x8c620e80,r1 */
L8c620dd0: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dd2; /* cmp/eq r1,r0 */
L8c620dd2: if((*sr&1u))goto L8c620e2c;goto L8c620dd4; /* bt 0x8c620e2c */
L8c620dd4: r[1]=rd(m,0x8c620e84u,4);goto L8c620dd6; /* mov.l 0x8c620e84,r1 */
L8c620dd6: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dd8; /* cmp/eq r1,r0 */
L8c620dd8: if((*sr&1u))goto L8c620e38;goto L8c620dda; /* bt 0x8c620e38 */
L8c620dda: r[1]=rd(m,0x8c620e88u,4);goto L8c620ddc; /* mov.l 0x8c620e88,r1 */
L8c620ddc: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dde; /* cmp/eq r1,r0 */
L8c620dde: if((*sr&1u))goto L8c620e44;goto L8c620de0; /* bt 0x8c620e44 */
L8c620de0: r[1]=rd(m,0x8c620e8cu,4);goto L8c620de2; /* mov.l 0x8c620e8c,r1 */
L8c620de2: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620de4; /* cmp/eq r1,r0 */
L8c620de4: if((*sr&1u))goto L8c620e44;goto L8c620de6; /* bt 0x8c620e44 */
L8c620de6: r[1]=rd(m,0x8c620e90u,4);goto L8c620de8; /* mov.l 0x8c620e90,r1 */
L8c620de8: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dea; /* cmp/eq r1,r0 */
L8c620dea: if((*sr&1u))goto L8c620e50;goto L8c620dec; /* bt 0x8c620e50 */
L8c620dec: r[1]=rd(m,0x8c620e94u,4);goto L8c620dee; /* mov.l 0x8c620e94,r1 */
L8c620dee: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620df0; /* cmp/eq r1,r0 */
L8c620df0: if((*sr&1u))goto L8c620e5c;goto L8c620df2; /* bt 0x8c620e5c */
L8c620df2: r[1]=rd(m,0x8c620e98u,4);goto L8c620df4; /* mov.l 0x8c620e98,r1 */
L8c620df4: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620df6; /* cmp/eq r1,r0 */
L8c620df6: if((*sr&1u))goto L8c620e5c;goto L8c620df8; /* bt 0x8c620e5c */
L8c620df8: r[1]=rd(m,0x8c620e9cu,4);goto L8c620dfa; /* mov.l 0x8c620e9c,r1 */
L8c620dfa: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dfc; /* cmp/eq r1,r0 */
L8c620dfc: if((*sr&1u))goto L8c620e68;goto L8c620dfe; /* bt 0x8c620e68 */
L8c620dfe: r[1]=rd(m,0x8c620ea0u,4);goto L8c620e00; /* mov.l 0x8c620ea0,r1 */
L8c620e00: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620e02; /* cmp/eq r1,r0 */
L8c620e02: if((*sr&1u))goto L8c620e68;goto L8c620e04; /* bt 0x8c620e68 */
L8c620e04: goto L8c620e70; /* bra 0x8c620e70 */
L8c620e08: r[3]=rd(m,0x8c620ec8u,4);goto L8c620e0a; /* mov.l 0x8c620ec8,r3 */
L8c620e0a: r[5]=r[13];goto L8c620e0c; /* mov r13,r5 */
L8c620e0c: target=r[3];*pr=0x8c620e10u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e10; /* jsr @r3 */
L8c620e10: goto L8c620e70; /* bra 0x8c620e70 */
L8c620e14: r[3]=rd(m,0x8c620eccu,4);goto L8c620e16; /* mov.l 0x8c620ecc,r3 */
L8c620e16: r[5]=r[13];goto L8c620e18; /* mov r13,r5 */
L8c620e18: target=r[3];*pr=0x8c620e1cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e1c; /* jsr @r3 */
L8c620e1c: goto L8c620e70; /* bra 0x8c620e70 */
L8c620e20: r[3]=rd(m,0x8c620ed0u,4);goto L8c620e22; /* mov.l 0x8c620ed0,r3 */
L8c620e22: r[5]=r[13];goto L8c620e24; /* mov r13,r5 */
L8c620e24: target=r[3];*pr=0x8c620e28u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e28; /* jsr @r3 */
L8c620e28: goto L8c620e70; /* bra 0x8c620e70 */
L8c620e2c: r[3]=rd(m,0x8c620ed4u,4);goto L8c620e2e; /* mov.l 0x8c620ed4,r3 */
L8c620e2e: r[5]=r[13];goto L8c620e30; /* mov r13,r5 */
L8c620e30: target=r[3];*pr=0x8c620e34u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e34; /* jsr @r3 */
L8c620e34: goto L8c620e70; /* bra 0x8c620e70 */
L8c620e38: r[3]=rd(m,0x8c620ed8u,4);goto L8c620e3a; /* mov.l 0x8c620ed8,r3 */
L8c620e3a: r[5]=r[13];goto L8c620e3c; /* mov r13,r5 */
L8c620e3c: target=r[3];*pr=0x8c620e40u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e40; /* jsr @r3 */
L8c620e40: goto L8c620e70; /* bra 0x8c620e70 */
L8c620e44: r[3]=rd(m,0x8c620edcu,4);goto L8c620e46; /* mov.l 0x8c620edc,r3 */
L8c620e46: r[5]=r[13];goto L8c620e48; /* mov r13,r5 */
L8c620e48: target=r[3];*pr=0x8c620e4cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e4c; /* jsr @r3 */
L8c620e4c: goto L8c620e70; /* bra 0x8c620e70 */
L8c620e50: r[3]=rd(m,0x8c620ee0u,4);goto L8c620e52; /* mov.l 0x8c620ee0,r3 */
L8c620e52: r[5]=r[13];goto L8c620e54; /* mov r13,r5 */
L8c620e54: target=r[3];*pr=0x8c620e58u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e58; /* jsr @r3 */
L8c620e58: goto L8c620e70; /* bra 0x8c620e70 */
L8c620e5c: r[3]=rd(m,0x8c620ee4u,4);goto L8c620e5e; /* mov.l 0x8c620ee4,r3 */
L8c620e5e: r[5]=r[13];goto L8c620e60; /* mov r13,r5 */
L8c620e60: target=r[3];*pr=0x8c620e64u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e64; /* jsr @r3 */
L8c620e64: goto L8c620e70; /* bra 0x8c620e70 */
L8c620e68: r[3]=rd(m,0x8c620ee8u,4);goto L8c620e6a; /* mov.l 0x8c620ee8,r3 */
L8c620e6a: r[5]=r[13];goto L8c620e6c; /* mov r13,r5 */
L8c620e6c: target=r[3];*pr=0x8c620e70u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e70; /* jsr @r3 */
L8c620e70: goto L8c620eec; /* bra 0x8c620eec */
L8c620eec: r[10]=rd(m,0x8c620f24u,4);goto L8c620eee; /* mov.l 0x8c620f24,r10 */
L8c620eee: target=r[10];*pr=0x8c620ef2u;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620ef2; /* jsr @r10 */
L8c620ef2: r[2]=rd(m,r[15],4);goto L8c620ef4; /* mov.l @r15,r2 */
L8c620ef4: r[12]+=24u;goto L8c620ef6; /* add #0x18,r12 */
L8c620ef6: r[2]+=1u;goto L8c620ef8; /* add #0x1,r2 */
L8c620ef8: wr(m,r[15],r[2],4);goto L8c620efa; /* mov.l r2,@r15 */
L8c620efa: r[0]=sx(m,r[9]+0x14u,2);goto L8c620efc; /* mov.w @(0x14,r9),r0 */
L8c620efc: r[3]=rd(m,r[15],4);goto L8c620efe; /* mov.l @r15,r3 */
L8c620efe: r[0]=r[0]&65535u;goto L8c620f00; /* extu.w r0,r0 */
L8c620f00: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c620f02; /* cmp/ge r0,r3 */
L8c620f02: if((*sr&1u))goto L8c620f0a;goto L8c620f04; /* bt 0x8c620f0a */
L8c620f04: r[2]=rd(m,0x8c620f28u,4);goto L8c620f06; /* mov.l 0x8c620f28,r2 */
L8c620f06: target=r[2];switch(target){case 0x8c620a30u:goto L8c620a30;case 0x8c620efau:goto L8c620efa;case 0x8c620f0au:goto L8c620f0a;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r2 */
L8c620f0a: r[15]+=12u;goto L8c620f0c; /* add #0xc,r15 */
L8c620f0c: *pr=rd(m,r[15],4);r[15]+=4;goto L8c620f0e; /* lds.l @r15+,PR */
L8c620f0e: fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620f10; /* fmov.s @r15+,fr15 */
L8c620f10: r[9]=rd(m,r[15],4);r[15]+=4;goto L8c620f12; /* mov.l @r15+,r9 */
L8c620f12: r[10]=rd(m,r[15],4);r[15]+=4;goto L8c620f14; /* mov.l @r15+,r10 */
L8c620f14: r[11]=rd(m,r[15],4);r[15]+=4;goto L8c620f16; /* mov.l @r15+,r11 */
L8c620f16: r[12]=rd(m,r[15],4);r[15]+=4;goto L8c620f18; /* mov.l @r15+,r12 */
L8c620f18: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c620f1a; /* mov.l @r15+,r13 */
L8c620f1a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_mesh_filter_wrapper_8c62159a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c62159a;
L8c62159a: r[15]-=4;wr(m,r[15],*pr,4);goto L8c62159c; /* sts.l PR,@-r15 */
L8c62159c: r[15]+=4294967288u;goto L8c62159e; /* add #-0x8,r15 */
L8c62159e: wr(m,r[15]+0x4u,r[4],4);goto L8c6215a0; /* mov.l r4,@(0x4,r15) */
L8c6215a0: r[4]=r[15];goto L8c6215a2; /* mov r15,r4 */
L8c6215a2: wr(m,r[4],r[8],4);goto L8c6215a4; /* mov.l r8,@r4 */
L8c6215a4: r[3]=rd(m,0x8c621628u,4);goto L8c6215a6; /* mov.l 0x8c621628,r3 */
L8c6215a6: target=r[3];*pr=0x8c6215aau;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6215aa; /* jsr @r3 */
L8c6215aa: r[4]=r[15];goto L8c6215ac; /* mov r15,r4 */
L8c6215ac: wr(m,r[15]+0x4u,r[0],4);goto L8c6215ae; /* mov.l r0,@(0x4,r15) */
L8c6215ae: r[8]=rd(m,r[4],4);goto L8c6215b0; /* mov.l @r4,r8 */
L8c6215b0: r[0]=rd(m,r[15]+0x4u,4);goto L8c6215b2; /* mov.l @(0x4,r15),r0 */
L8c6215b2: r[15]+=8u;goto L8c6215b4; /* add #0x8,r15 */
L8c6215b4: *pr=rd(m,r[15],4);r[15]+=4;goto L8c6215b6; /* lds.l @r15+,PR */
L8c6215b6: return 0; /* rts */
}
