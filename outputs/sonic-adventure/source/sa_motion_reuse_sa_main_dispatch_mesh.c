#include "sa_draw_motion_stop.h"
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
L8c6209a8: sa_motion_publish_stop(ctx,0x8c6209a8u); r[15]-=4;wr(m,r[15],r[14],4);goto L8c6209aa; /* mov.l r14,@-r15 */
L8c6209aa: sa_motion_publish_stop(ctx,0x8c6209aau); r[15]-=4;wr(m,r[15],r[13],4);goto L8c6209ac; /* mov.l r13,@-r15 */
L8c6209ac: sa_motion_publish_stop(ctx,0x8c6209acu); r[15]-=4;wr(m,r[15],r[12],4);goto L8c6209ae; /* mov.l r12,@-r15 */
L8c6209ae: sa_motion_publish_stop(ctx,0x8c6209aeu); r[15]-=4;wr(m,r[15],r[11],4);goto L8c6209b0; /* mov.l r11,@-r15 */
L8c6209b0: sa_motion_publish_stop(ctx,0x8c6209b0u); r[15]-=4;wr(m,r[15],r[10],4);goto L8c6209b2; /* mov.l r10,@-r15 */
L8c6209b2: sa_motion_publish_stop(ctx,0x8c6209b2u); r[15]-=4;wr(m,r[15],r[9],4);goto L8c6209b4; /* mov.l r9,@-r15 */
L8c6209b4: sa_motion_publish_stop(ctx,0x8c6209b4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c6209b6; /* fmov.s fr15,@-r15 */
L8c6209b6: sa_motion_publish_stop(ctx,0x8c6209b6u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c6209b8; /* sts.l PR,@-r15 */
L8c6209b8: sa_motion_publish_stop(ctx,0x8c6209b8u); r[15]+=4294967284u;goto L8c6209ba; /* add #-0xc,r15 */
L8c6209ba: sa_motion_publish_stop(ctx,0x8c6209bau); r[1]=rd(m,0x8c620a20u,4);goto L8c6209bc; /* mov.l 0x8c620a20,r1 */
L8c6209bc: sa_motion_publish_stop(ctx,0x8c6209bcu); r[9]=r[4];goto L8c6209be; /* mov r4,r9 */
L8c6209be: sa_motion_publish_stop(ctx,0x8c6209beu); r[2]=rd(m,r[9]+0x8u,4);goto L8c6209c0; /* mov.l @(0x8,r9),r2 */
L8c6209c0: sa_motion_publish_stop(ctx,0x8c6209c0u); r[3]=rd(m,r[1],4);goto L8c6209c2; /* mov.l @r1,r3 */
L8c6209c2: sa_motion_publish_stop(ctx,0x8c6209c2u); r[14]=rd(m,0x8c620a1cu,4);goto L8c6209c4; /* mov.l 0x8c620a1c,r14 */
L8c6209c4: sa_motion_publish_stop(ctx,0x8c6209c4u); *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]);goto L8c6209c6; /* cmp/gt r3,r2 */
L8c6209c6: sa_motion_publish_stop(ctx,0x8c6209c6u); if(!(*sr&1u))goto L8c6209ce;goto L8c6209c8; /* bf 0x8c6209ce */
L8c6209c8: sa_motion_publish_stop(ctx,0x8c6209c8u); r[0]=rd(m,0x8c620a24u,4);goto L8c6209ca; /* mov.l 0x8c620a24,r0 */
L8c6209ca: sa_motion_publish_stop(ctx,0x8c6209ccu); target=r[0];switch(target){case 0x8c620a30u:goto L8c620a30;case 0x8c620efau:goto L8c620efa;case 0x8c620f0au:goto L8c620f0a;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r0 */
L8c6209ce: sa_motion_publish_stop(ctx,0x8c6209ceu); r[3]=rd(m,0x8c620a28u,4);goto L8c6209d0; /* mov.l 0x8c620a28,r3 */
L8c6209d0: sa_motion_publish_stop(ctx,0x8c6209d0u); r[12]=rd(m,r[9]+0xcu,4);goto L8c6209d2; /* mov.l @(0xc,r9),r12 */
L8c6209d2: sa_motion_publish_stop(ctx,0x8c6209d4u); target=r[3];*pr=0x8c6209d6u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6209d6; /* jsr @r3 */
L8c6209d6: sa_motion_publish_stop(ctx,0x8c6209d6u); *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6209d8; /* tst r0,r0 */
L8c6209d8: sa_motion_publish_stop(ctx,0x8c6209d8u); if((*sr&1u))goto L8c6209e0;goto L8c6209da; /* bt 0x8c6209e0 */
L8c6209da: sa_motion_publish_stop(ctx,0x8c6209dau); r[2]=rd(m,0x8c620a24u,4);goto L8c6209dc; /* mov.l 0x8c620a24,r2 */
L8c6209dc: sa_motion_publish_stop(ctx,0x8c6209deu); target=r[2];switch(target){case 0x8c620a30u:goto L8c620a30;case 0x8c620efau:goto L8c620efa;case 0x8c620f0au:goto L8c620f0a;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r2 */
L8c6209e0: sa_motion_publish_stop(ctx,0x8c6209e0u); r[2]=rd(m,0x8c620a2cu,4);goto L8c6209e2; /* mov.l 0x8c620a2c,r2 */
L8c6209e2: sa_motion_publish_stop(ctx,0x8c6209e2u); r[3]=0u;goto L8c6209e4; /* mov #0x0,r3 */
L8c6209e4: sa_motion_publish_stop(ctx,0x8c6209e6u); target=r[2];wr(m,r[15],r[3],4);switch(target){case 0x8c620a30u:goto L8c620a30;case 0x8c620efau:goto L8c620efa;case 0x8c620f0au:goto L8c620f0a;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r2 */
L8c620a30: sa_motion_publish_stop(ctx,0x8c620a30u); r[0]=sx(m,r[12],2);goto L8c620a32; /* mov.w @r12,r0 */
L8c620a32: sa_motion_publish_stop(ctx,0x8c620a32u); r[4]=sx(m,0x8c620b62u,2);goto L8c620a34; /* mov.w 0x8c620b62,r4 */
L8c620a34: sa_motion_publish_stop(ctx,0x8c620a34u); r[0]&=r[4];goto L8c620a36; /* and r4,r0 */
L8c620a36: sa_motion_publish_stop(ctx,0x8c620a36u); wr(m,r[15]+0x4u,r[0],2);goto L8c620a38; /* mov.w r0,@(0x4,r15) */
L8c620a38: sa_motion_publish_stop(ctx,0x8c620a38u); r[3]=rd(m,r[15],4);goto L8c620a3a; /* mov.l @r15,r3 */
L8c620a3a: sa_motion_publish_stop(ctx,0x8c620a3au); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c620a3c; /* tst r3,r3 */
L8c620a3c: sa_motion_publish_stop(ctx,0x8c620a3cu); if((*sr&1u))goto L8c620a4c;goto L8c620a3e; /* bt 0x8c620a4c */
L8c620a3e: sa_motion_publish_stop(ctx,0x8c620a3eu); r[0]=sx(m,r[15]+0x4u,2);goto L8c620a40; /* mov.w @(0x4,r15),r0 */
L8c620a40: sa_motion_publish_stop(ctx,0x8c620a40u); r[3]=r[11]&65535u;goto L8c620a42; /* extu.w r11,r3 */
L8c620a42: sa_motion_publish_stop(ctx,0x8c620a42u); r[0]=r[0]&65535u;goto L8c620a44; /* extu.w r0,r0 */
L8c620a44: sa_motion_publish_stop(ctx,0x8c620a44u); *sr=(*sr&~1u)|(r[3]==r[0]);goto L8c620a46; /* cmp/eq r3,r0 */
L8c620a46: sa_motion_publish_stop(ctx,0x8c620a46u); if(!(*sr&1u))goto L8c620a4c;goto L8c620a48; /* bf 0x8c620a4c */
L8c620a48: sa_motion_publish_stop(ctx,0x8c620a4au); goto L8c620cac; /* bra 0x8c620cac */
L8c620a4c: sa_motion_publish_stop(ctx,0x8c620a4cu); r[0]=sx(m,r[15]+0x4u,2);goto L8c620a4e; /* mov.w @(0x4,r15),r0 */
L8c620a4e: sa_motion_publish_stop(ctx,0x8c620a4eu); r[4]=4u;goto L8c620a50; /* mov #0x4,r4 */
L8c620a50: sa_motion_publish_stop(ctx,0x8c620a50u); r[13]=rd(m,r[9]+0x10u,4);goto L8c620a52; /* mov.l @(0x10,r9),r13 */
L8c620a52: sa_motion_publish_stop(ctx,0x8c620a52u); r[0]=r[0]&65535u;goto L8c620a54; /* extu.w r0,r0 */
L8c620a54: sa_motion_publish_stop(ctx,0x8c620a54u); r[10]=rd(m,0x8c620b6cu,4);goto L8c620a56; /* mov.l 0x8c620b6c,r10 */
L8c620a56: sa_motion_publish_stop(ctx,0x8c620a56u); r[3]=r[0];goto L8c620a58; /* mov r0,r3 */
L8c620a58: sa_motion_publish_stop(ctx,0x8c620a58u); r[0]<<=2;goto L8c620a5a; /* shll2 r0 */
L8c620a5a: sa_motion_publish_stop(ctx,0x8c620a5au); r[0]+=r[3];goto L8c620a5c; /* add r3,r0 */
L8c620a5c: sa_motion_publish_stop(ctx,0x8c620a5cu); r[11]=rd(m,0x8c620b68u,4);goto L8c620a5e; /* mov.l 0x8c620b68,r11 */
L8c620a5e: sa_motion_publish_stop(ctx,0x8c620a5eu); r[0]<<=2;goto L8c620a60; /* shll2 r0 */
L8c620a60: sa_motion_publish_stop(ctx,0x8c620a60u); r[13]+=r[0];goto L8c620a62; /* add r0,r13 */
L8c620a62: sa_motion_publish_stop(ctx,0x8c620a62u); r[2]=rd(m,r[13]+0x10u,4);goto L8c620a64; /* mov.l @(0x10,r13),r2 */
L8c620a64: sa_motion_publish_stop(ctx,0x8c620a64u); wr(m,r[11],r[2],4);goto L8c620a66; /* mov.l r2,@r11 */
L8c620a66: sa_motion_publish_stop(ctx,0x8c620a66u); r[3]=rd(m,r[10],4);goto L8c620a68; /* mov.l @r10,r3 */
L8c620a68: sa_motion_publish_stop(ctx,0x8c620a68u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c620a6a; /* tst r4,r3 */
L8c620a6a: sa_motion_publish_stop(ctx,0x8c620a6au); if((*sr&1u))goto L8c620a7e;goto L8c620a6c; /* bt 0x8c620a7e */
L8c620a6c: sa_motion_publish_stop(ctx,0x8c620a6cu); r[4]=rd(m,0x8c620b70u,4);goto L8c620a6e; /* mov.l 0x8c620b70,r4 */
L8c620a6e: sa_motion_publish_stop(ctx,0x8c620a6eu); r[2]=rd(m,r[11],4);goto L8c620a70; /* mov.l @r11,r2 */
L8c620a70: sa_motion_publish_stop(ctx,0x8c620a70u); r[3]=rd(m,r[4],4);goto L8c620a72; /* mov.l @r4,r3 */
L8c620a72: sa_motion_publish_stop(ctx,0x8c620a72u); r[4]=rd(m,0x8c620b74u,4);goto L8c620a74; /* mov.l 0x8c620b74,r4 */
L8c620a74: sa_motion_publish_stop(ctx,0x8c620a74u); r[2]&=r[3];goto L8c620a76; /* and r3,r2 */
L8c620a76: sa_motion_publish_stop(ctx,0x8c620a76u); wr(m,r[11],r[2],4);goto L8c620a78; /* mov.l r2,@r11 */
L8c620a78: sa_motion_publish_stop(ctx,0x8c620a78u); r[3]=rd(m,r[4],4);goto L8c620a7a; /* mov.l @r4,r3 */
L8c620a7a: sa_motion_publish_stop(ctx,0x8c620a7au); r[2]|=r[3];goto L8c620a7c; /* or r3,r2 */
L8c620a7c: sa_motion_publish_stop(ctx,0x8c620a7cu); wr(m,r[11],r[2],4);goto L8c620a7e; /* mov.l r2,@r11 */
L8c620a7e: sa_motion_publish_stop(ctx,0x8c620a7eu); r[2]=rd(m,r[11],4);goto L8c620a80; /* mov.l @r11,r2 */
L8c620a80: sa_motion_publish_stop(ctx,0x8c620a80u); r[0]=0x8c620b78u;goto L8c620a82; /* mova 0x8c620b78,r0 */
L8c620a82: sa_motion_publish_stop(ctx,0x8c620a82u); r[3]=rd(m,0x8c620b7cu,4);goto L8c620a84; /* mov.l 0x8c620b7c,r3 */
L8c620a84: sa_motion_publish_stop(ctx,0x8c620a84u); *sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c620a86; /* tst r3,r2 */
L8c620a86: sa_motion_publish_stop(ctx,0x8c620a88u); take=(*sr&1u);fload(m,f,15u,r[0]);if(take)goto L8c620af8;goto L8c620a8a; /* bt/s 0x8c620af8 */
L8c620a8a: sa_motion_publish_stop(ctx,0x8c620a8au); r[1]=rd(m,0x8c620b84u,4);goto L8c620a8c; /* mov.l 0x8c620b84,r1 */
L8c620a8c: sa_motion_publish_stop(ctx,0x8c620a8cu); r[4]=rd(m,r[13]+0xcu,4);goto L8c620a8e; /* mov.l @(0xc,r13),r4 */
L8c620a8e: sa_motion_publish_stop(ctx,0x8c620a8eu); r[2]=rd(m,0x8c620b80u,4);goto L8c620a90; /* mov.l 0x8c620b80,r2 */
L8c620a90: sa_motion_publish_stop(ctx,0x8c620a92u); target=r[1];*pr=0x8c620a94u;r[4]&=r[2];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620a94; /* jsr @r1 */
L8c620a94: sa_motion_publish_stop(ctx,0x8c620a94u); r[3]=rd(m,r[10],4);goto L8c620a96; /* mov.l @r10,r3 */
L8c620a96: sa_motion_publish_stop(ctx,0x8c620a96u); r[4]=sx(m,0x8c620b64u,2);goto L8c620a98; /* mov.w 0x8c620b64,r4 */
L8c620a98: sa_motion_publish_stop(ctx,0x8c620a98u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c620a9a; /* tst r4,r3 */
L8c620a9a: sa_motion_publish_stop(ctx,0x8c620a9au); if((*sr&1u))goto L8c620abc;goto L8c620a9c; /* bt 0x8c620abc */
L8c620a9c: sa_motion_publish_stop(ctx,0x8c620a9cu); r[0]=sx(m,r[13]+0x3u,1);goto L8c620a9e; /* mov.b @(0x3,r13),r0 */
L8c620a9e: sa_motion_publish_stop(ctx,0x8c620a9eu); fcopy(f,4u,15u);goto L8c620aa0; /* fmov fr15,fr4 */
L8c620aa0: sa_motion_publish_stop(ctx,0x8c620aa0u); r[3]=r[0]&255u;goto L8c620aa2; /* extu.b r0,r3 */
L8c620aa2: sa_motion_publish_stop(ctx,0x8c620aa2u); *fpul=r[3];goto L8c620aa4; /* lds r3,FPUL */
L8c620aa4: sa_motion_publish_stop(ctx,0x8c620aa4u); r[0]=16u;goto L8c620aa6; /* mov #0x10,r0 */
L8c620aa6: sa_motion_publish_stop(ctx,0x8c620aa6u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620aa8; /* float FPUL,fr3 */
L8c620aa8: sa_motion_publish_stop(ctx,0x8c620aa8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620aaa; /* fmul fr4,fr3 */
L8c620aaa: sa_motion_publish_stop(ctx,0x8c620aaau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c620aac; /* fldi1 fr4 */
L8c620aac: sa_motion_publish_stop(ctx,0x8c620aacu); fstore(m,f,3u,r[0]+r[14]);goto L8c620aae; /* fmov fr3,@(r0,r14) */
L8c620aae: sa_motion_publish_stop(ctx,0x8c620aaeu); r[0]=20u;goto L8c620ab0; /* mov #0x14,r0 */
L8c620ab0: sa_motion_publish_stop(ctx,0x8c620ab0u); fstore(m,f,4u,r[0]+r[14]);goto L8c620ab2; /* fmov fr4,@(r0,r14) */
L8c620ab2: sa_motion_publish_stop(ctx,0x8c620ab2u); r[0]=24u;goto L8c620ab4; /* mov #0x18,r0 */
L8c620ab4: sa_motion_publish_stop(ctx,0x8c620ab4u); fstore(m,f,4u,r[0]+r[14]);goto L8c620ab6; /* fmov fr4,@(r0,r14) */
L8c620ab6: sa_motion_publish_stop(ctx,0x8c620ab6u); r[0]=28u;goto L8c620ab8; /* mov #0x1c,r0 */
L8c620ab8: sa_motion_publish_stop(ctx,0x8c620abau); fstore(m,f,4u,r[0]+r[14]);goto L8c620b32; /* bra 0x8c620b32 */
L8c620abc: sa_motion_publish_stop(ctx,0x8c620abcu); r[0]=sx(m,r[13]+0x2u,1);goto L8c620abe; /* mov.b @(0x2,r13),r0 */
L8c620abe: sa_motion_publish_stop(ctx,0x8c620abeu); fcopy(f,4u,15u);goto L8c620ac0; /* fmov fr15,fr4 */
L8c620ac0: sa_motion_publish_stop(ctx,0x8c620ac0u); r[3]=r[0]&255u;goto L8c620ac2; /* extu.b r0,r3 */
L8c620ac2: sa_motion_publish_stop(ctx,0x8c620ac2u); *fpul=r[3];goto L8c620ac4; /* lds r3,FPUL */
L8c620ac4: sa_motion_publish_stop(ctx,0x8c620ac4u); r[0]=20u;goto L8c620ac6; /* mov #0x14,r0 */
L8c620ac6: sa_motion_publish_stop(ctx,0x8c620ac6u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620ac8; /* float FPUL,fr3 */
L8c620ac8: sa_motion_publish_stop(ctx,0x8c620ac8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620aca; /* fmul fr4,fr3 */
L8c620aca: sa_motion_publish_stop(ctx,0x8c620acau); fstore(m,f,3u,r[0]+r[14]);goto L8c620acc; /* fmov fr3,@(r0,r14) */
L8c620acc: sa_motion_publish_stop(ctx,0x8c620accu); r[0]=sx(m,r[13]+0x1u,1);goto L8c620ace; /* mov.b @(0x1,r13),r0 */
L8c620ace: sa_motion_publish_stop(ctx,0x8c620aceu); r[3]=r[0]&255u;goto L8c620ad0; /* extu.b r0,r3 */
L8c620ad0: sa_motion_publish_stop(ctx,0x8c620ad0u); *fpul=r[3];goto L8c620ad2; /* lds r3,FPUL */
L8c620ad2: sa_motion_publish_stop(ctx,0x8c620ad2u); r[0]=24u;goto L8c620ad4; /* mov #0x18,r0 */
L8c620ad4: sa_motion_publish_stop(ctx,0x8c620ad4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620ad6; /* float FPUL,fr3 */
L8c620ad6: sa_motion_publish_stop(ctx,0x8c620ad6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620ad8; /* fmul fr4,fr3 */
L8c620ad8: sa_motion_publish_stop(ctx,0x8c620ad8u); fstore(m,f,3u,r[0]+r[14]);goto L8c620ada; /* fmov fr3,@(r0,r14) */
L8c620ada: sa_motion_publish_stop(ctx,0x8c620adau); r[0]=28u;goto L8c620adc; /* mov #0x1c,r0 */
L8c620adc: sa_motion_publish_stop(ctx,0x8c620adcu); r[3]=sx(m,r[13],1);goto L8c620ade; /* mov.b @r13,r3 */
L8c620ade: sa_motion_publish_stop(ctx,0x8c620adeu); r[3]=r[3]&255u;goto L8c620ae0; /* extu.b r3,r3 */
L8c620ae0: sa_motion_publish_stop(ctx,0x8c620ae0u); *fpul=r[3];goto L8c620ae2; /* lds r3,FPUL */
L8c620ae2: sa_motion_publish_stop(ctx,0x8c620ae2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620ae4; /* float FPUL,fr3 */
L8c620ae4: sa_motion_publish_stop(ctx,0x8c620ae4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620ae6; /* fmul fr4,fr3 */
L8c620ae6: sa_motion_publish_stop(ctx,0x8c620ae6u); fstore(m,f,3u,r[0]+r[14]);goto L8c620ae8; /* fmov fr3,@(r0,r14) */
L8c620ae8: sa_motion_publish_stop(ctx,0x8c620ae8u); r[0]=sx(m,r[13]+0x3u,1);goto L8c620aea; /* mov.b @(0x3,r13),r0 */
L8c620aea: sa_motion_publish_stop(ctx,0x8c620aeau); r[3]=r[0]&255u;goto L8c620aec; /* extu.b r0,r3 */
L8c620aec: sa_motion_publish_stop(ctx,0x8c620aecu); *fpul=r[3];goto L8c620aee; /* lds r3,FPUL */
L8c620aee: sa_motion_publish_stop(ctx,0x8c620aeeu); r[0]=16u;goto L8c620af0; /* mov #0x10,r0 */
L8c620af0: sa_motion_publish_stop(ctx,0x8c620af0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620af2; /* float FPUL,fr3 */
L8c620af2: sa_motion_publish_stop(ctx,0x8c620af2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620af4; /* fmul fr4,fr3 */
L8c620af4: sa_motion_publish_stop(ctx,0x8c620af6u); fstore(m,f,3u,r[0]+r[14]);goto L8c620b32; /* bra 0x8c620b32 */
L8c620af8: sa_motion_publish_stop(ctx,0x8c620af8u); r[0]=sx(m,r[13]+0x2u,1);goto L8c620afa; /* mov.b @(0x2,r13),r0 */
L8c620afa: sa_motion_publish_stop(ctx,0x8c620afau); fcopy(f,4u,15u);goto L8c620afc; /* fmov fr15,fr4 */
L8c620afc: sa_motion_publish_stop(ctx,0x8c620afcu); r[2]=r[0]&255u;goto L8c620afe; /* extu.b r0,r2 */
L8c620afe: sa_motion_publish_stop(ctx,0x8c620afeu); *fpul=r[2];goto L8c620b00; /* lds r2,FPUL */
L8c620b00: sa_motion_publish_stop(ctx,0x8c620b00u); r[0]=20u;goto L8c620b02; /* mov #0x14,r0 */
L8c620b02: sa_motion_publish_stop(ctx,0x8c620b02u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620b04; /* float FPUL,fr3 */
L8c620b04: sa_motion_publish_stop(ctx,0x8c620b04u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620b06; /* fmul fr4,fr3 */
L8c620b06: sa_motion_publish_stop(ctx,0x8c620b06u); fstore(m,f,3u,r[0]+r[14]);goto L8c620b08; /* fmov fr3,@(r0,r14) */
L8c620b08: sa_motion_publish_stop(ctx,0x8c620b08u); r[0]=sx(m,r[13]+0x1u,1);goto L8c620b0a; /* mov.b @(0x1,r13),r0 */
L8c620b0a: sa_motion_publish_stop(ctx,0x8c620b0au); r[2]=r[0]&255u;goto L8c620b0c; /* extu.b r0,r2 */
L8c620b0c: sa_motion_publish_stop(ctx,0x8c620b0cu); *fpul=r[2];goto L8c620b0e; /* lds r2,FPUL */
L8c620b0e: sa_motion_publish_stop(ctx,0x8c620b0eu); r[0]=24u;goto L8c620b10; /* mov #0x18,r0 */
L8c620b10: sa_motion_publish_stop(ctx,0x8c620b10u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620b12; /* float FPUL,fr3 */
L8c620b12: sa_motion_publish_stop(ctx,0x8c620b12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620b14; /* fmul fr4,fr3 */
L8c620b14: sa_motion_publish_stop(ctx,0x8c620b14u); fstore(m,f,3u,r[0]+r[14]);goto L8c620b16; /* fmov fr3,@(r0,r14) */
L8c620b16: sa_motion_publish_stop(ctx,0x8c620b16u); r[0]=28u;goto L8c620b18; /* mov #0x1c,r0 */
L8c620b18: sa_motion_publish_stop(ctx,0x8c620b18u); r[2]=sx(m,r[13],1);goto L8c620b1a; /* mov.b @r13,r2 */
L8c620b1a: sa_motion_publish_stop(ctx,0x8c620b1au); r[2]=r[2]&255u;goto L8c620b1c; /* extu.b r2,r2 */
L8c620b1c: sa_motion_publish_stop(ctx,0x8c620b1cu); *fpul=r[2];goto L8c620b1e; /* lds r2,FPUL */
L8c620b1e: sa_motion_publish_stop(ctx,0x8c620b1eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620b20; /* float FPUL,fr3 */
L8c620b20: sa_motion_publish_stop(ctx,0x8c620b20u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620b22; /* fmul fr4,fr3 */
L8c620b22: sa_motion_publish_stop(ctx,0x8c620b22u); fstore(m,f,3u,r[0]+r[14]);goto L8c620b24; /* fmov fr3,@(r0,r14) */
L8c620b24: sa_motion_publish_stop(ctx,0x8c620b24u); r[0]=sx(m,r[13]+0x3u,1);goto L8c620b26; /* mov.b @(0x3,r13),r0 */
L8c620b26: sa_motion_publish_stop(ctx,0x8c620b26u); r[2]=r[0]&255u;goto L8c620b28; /* extu.b r0,r2 */
L8c620b28: sa_motion_publish_stop(ctx,0x8c620b28u); *fpul=r[2];goto L8c620b2a; /* lds r2,FPUL */
L8c620b2a: sa_motion_publish_stop(ctx,0x8c620b2au); r[0]=16u;goto L8c620b2c; /* mov #0x10,r0 */
L8c620b2c: sa_motion_publish_stop(ctx,0x8c620b2cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620b2e; /* float FPUL,fr3 */
L8c620b2e: sa_motion_publish_stop(ctx,0x8c620b2eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620b30; /* fmul fr4,fr3 */
L8c620b30: sa_motion_publish_stop(ctx,0x8c620b30u); fstore(m,f,3u,r[0]+r[14]);goto L8c620b32; /* fmov fr3,@(r0,r14) */
L8c620b32: sa_motion_publish_stop(ctx,0x8c620b32u); r[5]=rd(m,r[10],4);goto L8c620b34; /* mov.l @r10,r5 */
L8c620b34: sa_motion_publish_stop(ctx,0x8c620b34u); r[6]=16u;goto L8c620b36; /* mov #0x10,r6 */
L8c620b36: sa_motion_publish_stop(ctx,0x8c620b36u); r[4]=rd(m,0x8c620b88u,4);goto L8c620b38; /* mov.l 0x8c620b88,r4 */
L8c620b38: sa_motion_publish_stop(ctx,0x8c620b38u); r[3]=r[5];goto L8c620b3a; /* mov r5,r3 */
L8c620b3a: sa_motion_publish_stop(ctx,0x8c620b3au); *sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c620b3c; /* tst r6,r3 */
L8c620b3c: sa_motion_publish_stop(ctx,0x8c620b3eu); take=(*sr&1u);fload(m,f,4u,r[4]);if(take)goto L8c620b8c;goto L8c620b40; /* bt/s 0x8c620b8c */
L8c620b40: sa_motion_publish_stop(ctx,0x8c620b40u); r[0]=16u;goto L8c620b42; /* mov #0x10,r0 */
L8c620b42: sa_motion_publish_stop(ctx,0x8c620b42u); fstore(m,f,4u,r[0]+r[14]);goto L8c620b44; /* fmov fr4,@(r0,r14) */
L8c620b44: sa_motion_publish_stop(ctx,0x8c620b44u); r[0]=4u;goto L8c620b46; /* mov #0x4,r0 */
L8c620b46: sa_motion_publish_stop(ctx,0x8c620b46u); fload(m,f,3u,r[0]+r[4]);goto L8c620b48; /* fmov.s @(r0,r4),fr3 */
L8c620b48: sa_motion_publish_stop(ctx,0x8c620b48u); r[0]=20u;goto L8c620b4a; /* mov #0x14,r0 */
L8c620b4a: sa_motion_publish_stop(ctx,0x8c620b4au); fstore(m,f,3u,r[0]+r[14]);goto L8c620b4c; /* fmov fr3,@(r0,r14) */
L8c620b4c: sa_motion_publish_stop(ctx,0x8c620b4cu); r[0]=8u;goto L8c620b4e; /* mov #0x8,r0 */
L8c620b4e: sa_motion_publish_stop(ctx,0x8c620b4eu); fload(m,f,3u,r[0]+r[4]);goto L8c620b50; /* fmov.s @(r0,r4),fr3 */
L8c620b50: sa_motion_publish_stop(ctx,0x8c620b50u); r[0]=24u;goto L8c620b52; /* mov #0x18,r0 */
L8c620b52: sa_motion_publish_stop(ctx,0x8c620b52u); fstore(m,f,3u,r[0]+r[14]);goto L8c620b54; /* fmov fr3,@(r0,r14) */
L8c620b54: sa_motion_publish_stop(ctx,0x8c620b54u); r[0]=12u;goto L8c620b56; /* mov #0xc,r0 */
L8c620b56: sa_motion_publish_stop(ctx,0x8c620b56u); fload(m,f,3u,r[0]+r[4]);goto L8c620b58; /* fmov.s @(r0,r4),fr3 */
L8c620b58: sa_motion_publish_stop(ctx,0x8c620b58u); r[0]=28u;goto L8c620b5a; /* mov #0x1c,r0 */
L8c620b5a: sa_motion_publish_stop(ctx,0x8c620b5cu); fstore(m,f,3u,r[0]+r[14]);goto L8c620bc4; /* bra 0x8c620bc4 */
L8c620b8c: sa_motion_publish_stop(ctx,0x8c620b8cu); r[6]=32u;goto L8c620b8e; /* mov #0x20,r6 */
L8c620b8e: sa_motion_publish_stop(ctx,0x8c620b8eu); *sr=(*sr&~1u)|!(r[6]&r[5]);goto L8c620b90; /* tst r6,r5 */
L8c620b90: sa_motion_publish_stop(ctx,0x8c620b90u); if((*sr&1u))goto L8c620bc4;goto L8c620b92; /* bt 0x8c620bc4 */
L8c620b92: sa_motion_publish_stop(ctx,0x8c620b92u); r[0]=16u;goto L8c620b94; /* mov #0x10,r0 */
L8c620b94: sa_motion_publish_stop(ctx,0x8c620b94u); fload(m,f,3u,r[0]+r[14]);goto L8c620b96; /* fmov.s @(r0,r14),fr3 */
L8c620b96: sa_motion_publish_stop(ctx,0x8c620b96u); r[1]=4u;goto L8c620b98; /* mov #0x4,r1 */
L8c620b98: sa_motion_publish_stop(ctx,0x8c620b98u); r[1]+=r[4];goto L8c620b9a; /* add r4,r1 */
L8c620b9a: sa_motion_publish_stop(ctx,0x8c620b9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c620b9c; /* fadd fr4,fr3 */
L8c620b9c: sa_motion_publish_stop(ctx,0x8c620b9cu); fstore(m,f,3u,r[0]+r[14]);goto L8c620b9e; /* fmov fr3,@(r0,r14) */
L8c620b9e: sa_motion_publish_stop(ctx,0x8c620b9eu); r[0]=20u;goto L8c620ba0; /* mov #0x14,r0 */
L8c620ba0: sa_motion_publish_stop(ctx,0x8c620ba0u); fload(m,f,2u,r[0]+r[14]);goto L8c620ba2; /* fmov.s @(r0,r14),fr2 */
L8c620ba2: sa_motion_publish_stop(ctx,0x8c620ba2u); fload(m,f,3u,r[1]);goto L8c620ba4; /* fmov.s @r1,fr3 */
L8c620ba4: sa_motion_publish_stop(ctx,0x8c620ba4u); r[1]=8u;goto L8c620ba6; /* mov #0x8,r1 */
L8c620ba6: sa_motion_publish_stop(ctx,0x8c620ba6u); r[1]+=r[4];goto L8c620ba8; /* add r4,r1 */
L8c620ba8: sa_motion_publish_stop(ctx,0x8c620ba8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c620baa; /* fadd fr3,fr2 */
L8c620baa: sa_motion_publish_stop(ctx,0x8c620baau); fstore(m,f,2u,r[0]+r[14]);goto L8c620bac; /* fmov fr2,@(r0,r14) */
L8c620bac: sa_motion_publish_stop(ctx,0x8c620bacu); r[0]=24u;goto L8c620bae; /* mov #0x18,r0 */
L8c620bae: sa_motion_publish_stop(ctx,0x8c620baeu); fload(m,f,2u,r[0]+r[14]);goto L8c620bb0; /* fmov.s @(r0,r14),fr2 */
L8c620bb0: sa_motion_publish_stop(ctx,0x8c620bb0u); fload(m,f,3u,r[1]);goto L8c620bb2; /* fmov.s @r1,fr3 */
L8c620bb2: sa_motion_publish_stop(ctx,0x8c620bb2u); r[1]=12u;goto L8c620bb4; /* mov #0xc,r1 */
L8c620bb4: sa_motion_publish_stop(ctx,0x8c620bb4u); r[1]+=r[4];goto L8c620bb6; /* add r4,r1 */
L8c620bb6: sa_motion_publish_stop(ctx,0x8c620bb6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c620bb8; /* fadd fr3,fr2 */
L8c620bb8: sa_motion_publish_stop(ctx,0x8c620bb8u); fstore(m,f,2u,r[0]+r[14]);goto L8c620bba; /* fmov fr2,@(r0,r14) */
L8c620bba: sa_motion_publish_stop(ctx,0x8c620bbau); r[0]=28u;goto L8c620bbc; /* mov #0x1c,r0 */
L8c620bbc: sa_motion_publish_stop(ctx,0x8c620bbcu); fload(m,f,2u,r[0]+r[14]);goto L8c620bbe; /* fmov.s @(r0,r14),fr2 */
L8c620bbe: sa_motion_publish_stop(ctx,0x8c620bbeu); fload(m,f,3u,r[1]);goto L8c620bc0; /* fmov.s @r1,fr3 */
L8c620bc0: sa_motion_publish_stop(ctx,0x8c620bc0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c620bc2; /* fadd fr3,fr2 */
L8c620bc2: sa_motion_publish_stop(ctx,0x8c620bc2u); fstore(m,f,2u,r[0]+r[14]);goto L8c620bc4; /* fmov fr2,@(r0,r14) */
L8c620bc4: sa_motion_publish_stop(ctx,0x8c620bc4u); r[0]=sx(m,r[13]+0x6u,1);goto L8c620bc6; /* mov.b @(0x6,r13),r0 */
L8c620bc6: sa_motion_publish_stop(ctx,0x8c620bc6u); fcopy(f,4u,15u);goto L8c620bc8; /* fmov fr15,fr4 */
L8c620bc8: sa_motion_publish_stop(ctx,0x8c620bc8u); r[3]=r[0]&255u;goto L8c620bca; /* extu.b r0,r3 */
L8c620bca: sa_motion_publish_stop(ctx,0x8c620bcau); r[4]=rd(m,0x8c620cb0u,4);goto L8c620bcc; /* mov.l 0x8c620cb0,r4 */
L8c620bcc: sa_motion_publish_stop(ctx,0x8c620bccu); *fpul=r[3];goto L8c620bce; /* lds r3,FPUL */
L8c620bce: sa_motion_publish_stop(ctx,0x8c620bceu); r[0]=36u;goto L8c620bd0; /* mov #0x24,r0 */
L8c620bd0: sa_motion_publish_stop(ctx,0x8c620bd0u); r[10]=rd(m,0x8c620cb4u,4);goto L8c620bd2; /* mov.l 0x8c620cb4,r10 */
L8c620bd2: sa_motion_publish_stop(ctx,0x8c620bd2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620bd4; /* float FPUL,fr3 */
L8c620bd4: sa_motion_publish_stop(ctx,0x8c620bd4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620bd6; /* fmul fr4,fr3 */
L8c620bd6: sa_motion_publish_stop(ctx,0x8c620bd6u); fstore(m,f,3u,r[0]+r[14]);goto L8c620bd8; /* fmov fr3,@(r0,r14) */
L8c620bd8: sa_motion_publish_stop(ctx,0x8c620bd8u); r[0]=sx(m,r[13]+0x5u,1);goto L8c620bda; /* mov.b @(0x5,r13),r0 */
L8c620bda: sa_motion_publish_stop(ctx,0x8c620bdau); r[3]=r[0]&255u;goto L8c620bdc; /* extu.b r0,r3 */
L8c620bdc: sa_motion_publish_stop(ctx,0x8c620bdcu); *fpul=r[3];goto L8c620bde; /* lds r3,FPUL */
L8c620bde: sa_motion_publish_stop(ctx,0x8c620bdeu); r[0]=40u;goto L8c620be0; /* mov #0x28,r0 */
L8c620be0: sa_motion_publish_stop(ctx,0x8c620be0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620be2; /* float FPUL,fr3 */
L8c620be2: sa_motion_publish_stop(ctx,0x8c620be2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620be4; /* fmul fr4,fr3 */
L8c620be4: sa_motion_publish_stop(ctx,0x8c620be4u); fstore(m,f,3u,r[0]+r[14]);goto L8c620be6; /* fmov fr3,@(r0,r14) */
L8c620be6: sa_motion_publish_stop(ctx,0x8c620be6u); r[0]=sx(m,r[13]+0x4u,1);goto L8c620be8; /* mov.b @(0x4,r13),r0 */
L8c620be8: sa_motion_publish_stop(ctx,0x8c620be8u); r[3]=r[0]&255u;goto L8c620bea; /* extu.b r0,r3 */
L8c620bea: sa_motion_publish_stop(ctx,0x8c620beau); *fpul=r[3];goto L8c620bec; /* lds r3,FPUL */
L8c620bec: sa_motion_publish_stop(ctx,0x8c620becu); r[0]=44u;goto L8c620bee; /* mov #0x2c,r0 */
L8c620bee: sa_motion_publish_stop(ctx,0x8c620beeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c620bf0; /* float FPUL,fr3 */
L8c620bf0: sa_motion_publish_stop(ctx,0x8c620bf0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620bf2; /* fmul fr4,fr3 */
L8c620bf2: sa_motion_publish_stop(ctx,0x8c620bf2u); fstore(m,f,3u,r[0]+r[14]);goto L8c620bf4; /* fmov fr3,@(r0,r14) */
L8c620bf4: sa_motion_publish_stop(ctx,0x8c620bf4u); r[0]=8u;goto L8c620bf6; /* mov #0x8,r0 */
L8c620bf6: sa_motion_publish_stop(ctx,0x8c620bf6u); fload(m,f,3u,r[0]+r[13]);goto L8c620bf8; /* fmov.s @(r0,r13),fr3 */
L8c620bf8: sa_motion_publish_stop(ctx,0x8c620bf8u); r[0]=48u;goto L8c620bfa; /* mov #0x30,r0 */
L8c620bfa: sa_motion_publish_stop(ctx,0x8c620bfau); fstore(m,f,3u,r[0]+r[14]);goto L8c620bfc; /* fmov fr3,@(r0,r14) */
L8c620bfc: sa_motion_publish_stop(ctx,0x8c620bfcu); r[3]=rd(m,r[11],4);goto L8c620bfe; /* mov.l @r11,r3 */
L8c620bfe: sa_motion_publish_stop(ctx,0x8c620bfeu); r[3]&=r[4];goto L8c620c00; /* and r4,r3 */
L8c620c00: sa_motion_publish_stop(ctx,0x8c620c00u); wr(m,r[15]+0x8u,r[3],4);goto L8c620c02; /* mov.l r3,@(0x8,r15) */
L8c620c02: sa_motion_publish_stop(ctx,0x8c620c04u); target=r[10];*pr=0x8c620c06u;r[4]=rd(m,r[11],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c06; /* jsr @r10 */
L8c620c06: sa_motion_publish_stop(ctx,0x8c620c06u); r[5]=rd(m,r[11],4);goto L8c620c08; /* mov.l @r11,r5 */
L8c620c08: sa_motion_publish_stop(ctx,0x8c620c08u); r[3]=rd(m,0x8c620cc8u,4);goto L8c620c0a; /* mov.l 0x8c620cc8,r3 */
L8c620c0a: sa_motion_publish_stop(ctx,0x8c620c0au); r[2]=r[5];goto L8c620c0c; /* mov r5,r2 */
L8c620c0c: sa_motion_publish_stop(ctx,0x8c620c0cu); r[10]=rd(m,0x8c620cc4u,4);goto L8c620c0e; /* mov.l 0x8c620cc4,r10 */
L8c620c0e: sa_motion_publish_stop(ctx,0x8c620c0eu); r[7]=rd(m,0x8c620cb8u,4);goto L8c620c10; /* mov.l 0x8c620cb8,r7 */
L8c620c10: sa_motion_publish_stop(ctx,0x8c620c10u); *sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c620c12; /* tst r3,r2 */
L8c620c12: sa_motion_publish_stop(ctx,0x8c620c12u); r[6]=rd(m,0x8c620cbcu,4);goto L8c620c14; /* mov.l 0x8c620cbc,r6 */
L8c620c14: sa_motion_publish_stop(ctx,0x8c620c14u); r[4]=rd(m,0x8c620cc0u,4);goto L8c620c16; /* mov.l 0x8c620cc0,r4 */
L8c620c16: sa_motion_publish_stop(ctx,0x8c620c16u); if((*sr&1u))goto L8c620c62;goto L8c620c18; /* bt 0x8c620c62 */
L8c620c18: sa_motion_publish_stop(ctx,0x8c620c18u); *sr=(*sr&~1u)|!(r[5]&r[6]);goto L8c620c1a; /* tst r5,r6 */
L8c620c1a: sa_motion_publish_stop(ctx,0x8c620c1au); if((*sr&1u))goto L8c620c24;goto L8c620c1c; /* bt 0x8c620c24 */
L8c620c1c: sa_motion_publish_stop(ctx,0x8c620c1cu); *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c1e; /* tst r4,r5 */
L8c620c1e: sa_motion_publish_stop(ctx,0x8c620c1eu); if(!(*sr&1u))goto L8c620c2c;goto L8c620c20; /* bf 0x8c620c2c */
L8c620c20: sa_motion_publish_stop(ctx,0x8c620c22u); goto L8c620c36; /* bra 0x8c620c36 */
L8c620c24: sa_motion_publish_stop(ctx,0x8c620c24u); *sr=(*sr&~1u)|!(r[5]&r[7]);goto L8c620c26; /* tst r5,r7 */
L8c620c26: sa_motion_publish_stop(ctx,0x8c620c26u); if((*sr&1u))goto L8c620c40;goto L8c620c28; /* bt 0x8c620c40 */
L8c620c28: sa_motion_publish_stop(ctx,0x8c620c28u); *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c2a; /* tst r4,r5 */
L8c620c2a: sa_motion_publish_stop(ctx,0x8c620c2au); if((*sr&1u))goto L8c620c36;goto L8c620c2c; /* bt 0x8c620c36 */
L8c620c2c: sa_motion_publish_stop(ctx,0x8c620c2cu); r[11]=rd(m,0x8c620cccu,4);goto L8c620c2e; /* mov.l 0x8c620ccc,r11 */
L8c620c2e: sa_motion_publish_stop(ctx,0x8c620c30u); target=r[10];*pr=0x8c620c32u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c32; /* jsr @r10 */
L8c620c32: sa_motion_publish_stop(ctx,0x8c620c34u); goto L8c620c4a; /* bra 0x8c620c4a */
L8c620c36: sa_motion_publish_stop(ctx,0x8c620c36u); r[11]=rd(m,0x8c620cd0u,4);goto L8c620c38; /* mov.l 0x8c620cd0,r11 */
L8c620c38: sa_motion_publish_stop(ctx,0x8c620c3au); target=r[10];*pr=0x8c620c3cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c3c; /* jsr @r10 */
L8c620c3c: sa_motion_publish_stop(ctx,0x8c620c3eu); goto L8c620c56; /* bra 0x8c620c56 */
L8c620c40: sa_motion_publish_stop(ctx,0x8c620c40u); *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c42; /* tst r4,r5 */
L8c620c42: sa_motion_publish_stop(ctx,0x8c620c42u); if((*sr&1u))goto L8c620c50;goto L8c620c44; /* bt 0x8c620c50 */
L8c620c44: sa_motion_publish_stop(ctx,0x8c620c44u); r[11]=rd(m,0x8c620cd4u,4);goto L8c620c46; /* mov.l 0x8c620cd4,r11 */
L8c620c46: sa_motion_publish_stop(ctx,0x8c620c48u); target=r[10];*pr=0x8c620c4au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c4a; /* jsr @r10 */
L8c620c4a: sa_motion_publish_stop(ctx,0x8c620c4au); r[11]=rd(m,0x8c620cd8u,4);goto L8c620c4c; /* mov.l 0x8c620cd8,r11 */
L8c620c4c: sa_motion_publish_stop(ctx,0x8c620c4eu); goto L8c620c58; /* bra 0x8c620c58 */
L8c620c50: sa_motion_publish_stop(ctx,0x8c620c50u); r[11]=rd(m,0x8c620cdcu,4);goto L8c620c52; /* mov.l 0x8c620cdc,r11 */
L8c620c52: sa_motion_publish_stop(ctx,0x8c620c54u); target=r[10];*pr=0x8c620c56u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c56; /* jsr @r10 */
L8c620c56: sa_motion_publish_stop(ctx,0x8c620c56u); r[11]=rd(m,0x8c620ce0u,4);goto L8c620c58; /* mov.l 0x8c620ce0,r11 */
L8c620c58: sa_motion_publish_stop(ctx,0x8c620c58u); r[4]=r[14];goto L8c620c5a; /* mov r14,r4 */
L8c620c5a: sa_motion_publish_stop(ctx,0x8c620c5cu); target=r[11];*pr=0x8c620c5eu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c5e; /* jsr @r11 */
L8c620c5e: sa_motion_publish_stop(ctx,0x8c620c60u); r[8]=r[0];goto L8c620ca8; /* bra 0x8c620ca8 */
L8c620c62: sa_motion_publish_stop(ctx,0x8c620c62u); *sr=(*sr&~1u)|!(r[5]&r[6]);goto L8c620c64; /* tst r5,r6 */
L8c620c64: sa_motion_publish_stop(ctx,0x8c620c64u); if((*sr&1u))goto L8c620c6e;goto L8c620c66; /* bt 0x8c620c6e */
L8c620c66: sa_motion_publish_stop(ctx,0x8c620c66u); *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c68; /* tst r4,r5 */
L8c620c68: sa_motion_publish_stop(ctx,0x8c620c68u); if(!(*sr&1u))goto L8c620c76;goto L8c620c6a; /* bf 0x8c620c76 */
L8c620c6a: sa_motion_publish_stop(ctx,0x8c620c6cu); goto L8c620c80; /* bra 0x8c620c80 */
L8c620c6e: sa_motion_publish_stop(ctx,0x8c620c6eu); *sr=(*sr&~1u)|!(r[5]&r[7]);goto L8c620c70; /* tst r5,r7 */
L8c620c70: sa_motion_publish_stop(ctx,0x8c620c70u); if((*sr&1u))goto L8c620c8a;goto L8c620c72; /* bt 0x8c620c8a */
L8c620c72: sa_motion_publish_stop(ctx,0x8c620c72u); *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c74; /* tst r4,r5 */
L8c620c74: sa_motion_publish_stop(ctx,0x8c620c74u); if((*sr&1u))goto L8c620c80;goto L8c620c76; /* bt 0x8c620c80 */
L8c620c76: sa_motion_publish_stop(ctx,0x8c620c76u); r[11]=rd(m,0x8c620ce4u,4);goto L8c620c78; /* mov.l 0x8c620ce4,r11 */
L8c620c78: sa_motion_publish_stop(ctx,0x8c620c7au); target=r[10];*pr=0x8c620c7cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c7c; /* jsr @r10 */
L8c620c7c: sa_motion_publish_stop(ctx,0x8c620c7eu); goto L8c620c94; /* bra 0x8c620c94 */
L8c620c80: sa_motion_publish_stop(ctx,0x8c620c80u); r[11]=rd(m,0x8c620ce8u,4);goto L8c620c82; /* mov.l 0x8c620ce8,r11 */
L8c620c82: sa_motion_publish_stop(ctx,0x8c620c84u); target=r[10];*pr=0x8c620c86u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c86; /* jsr @r10 */
L8c620c86: sa_motion_publish_stop(ctx,0x8c620c88u); goto L8c620ca0; /* bra 0x8c620ca0 */
L8c620c8a: sa_motion_publish_stop(ctx,0x8c620c8au); *sr=(*sr&~1u)|!(r[4]&r[5]);goto L8c620c8c; /* tst r4,r5 */
L8c620c8c: sa_motion_publish_stop(ctx,0x8c620c8cu); if((*sr&1u))goto L8c620c9a;goto L8c620c8e; /* bt 0x8c620c9a */
L8c620c8e: sa_motion_publish_stop(ctx,0x8c620c8eu); r[11]=rd(m,0x8c620cecu,4);goto L8c620c90; /* mov.l 0x8c620cec,r11 */
L8c620c90: sa_motion_publish_stop(ctx,0x8c620c92u); target=r[10];*pr=0x8c620c94u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620c94; /* jsr @r10 */
L8c620c94: sa_motion_publish_stop(ctx,0x8c620c94u); r[11]=rd(m,0x8c620cf0u,4);goto L8c620c96; /* mov.l 0x8c620cf0,r11 */
L8c620c96: sa_motion_publish_stop(ctx,0x8c620c98u); goto L8c620ca2; /* bra 0x8c620ca2 */
L8c620c9a: sa_motion_publish_stop(ctx,0x8c620c9au); r[11]=rd(m,0x8c620cf4u,4);goto L8c620c9c; /* mov.l 0x8c620cf4,r11 */
L8c620c9c: sa_motion_publish_stop(ctx,0x8c620c9eu); target=r[10];*pr=0x8c620ca0u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620ca0; /* jsr @r10 */
L8c620ca0: sa_motion_publish_stop(ctx,0x8c620ca0u); r[11]=rd(m,0x8c620cf8u,4);goto L8c620ca2; /* mov.l 0x8c620cf8,r11 */
L8c620ca2: sa_motion_publish_stop(ctx,0x8c620ca4u); target=r[11];*pr=0x8c620ca6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620ca6; /* jsr @r11 */
L8c620ca6: sa_motion_publish_stop(ctx,0x8c620ca6u); r[8]=r[0];goto L8c620ca8; /* mov r0,r8 */
L8c620ca8: sa_motion_publish_stop(ctx,0x8c620ca8u); r[0]=sx(m,r[15]+0x4u,2);goto L8c620caa; /* mov.w @(0x4,r15),r0 */
L8c620caa: sa_motion_publish_stop(ctx,0x8c620caau); r[11]=r[0];goto L8c620cac; /* mov r0,r11 */
L8c620cac: sa_motion_publish_stop(ctx,0x8c620caeu); goto L8c620cfc; /* bra 0x8c620cfc */
L8c620cfc: sa_motion_publish_stop(ctx,0x8c620cfcu); r[4]=rd(m,0x8c620e74u,4);goto L8c620cfe; /* mov.l 0x8c620e74,r4 */
L8c620cfe: sa_motion_publish_stop(ctx,0x8c620cfeu); r[2]=rd(m,r[4],4);goto L8c620d00; /* mov.l @r4,r2 */
L8c620d00: sa_motion_publish_stop(ctx,0x8c620d00u); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c620d02; /* tst r2,r2 */
L8c620d02: sa_motion_publish_stop(ctx,0x8c620d02u); if(!(*sr&1u))goto L8c620dbc;goto L8c620d04; /* bf 0x8c620dbc */
L8c620d04: sa_motion_publish_stop(ctx,0x8c620d04u); r[0]=rd(m,r[15]+0x8u,4);goto L8c620d06; /* mov.l @(0x8,r15),r0 */
L8c620d06: sa_motion_publish_stop(ctx,0x8c620d06u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c620d08; /* cmp/eq #0x0,r0 */
L8c620d08: sa_motion_publish_stop(ctx,0x8c620d08u); if((*sr&1u))goto L8c620d50;goto L8c620d0a; /* bt 0x8c620d50 */
L8c620d0a: sa_motion_publish_stop(ctx,0x8c620d0au); r[1]=rd(m,0x8c620e78u,4);goto L8c620d0c; /* mov.l 0x8c620e78,r1 */
L8c620d0c: sa_motion_publish_stop(ctx,0x8c620d0cu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d0e; /* cmp/eq r1,r0 */
L8c620d0e: sa_motion_publish_stop(ctx,0x8c620d0eu); if((*sr&1u))goto L8c620d5c;goto L8c620d10; /* bt 0x8c620d5c */
L8c620d10: sa_motion_publish_stop(ctx,0x8c620d10u); r[1]=rd(m,0x8c620e7cu,4);goto L8c620d12; /* mov.l 0x8c620e7c,r1 */
L8c620d12: sa_motion_publish_stop(ctx,0x8c620d12u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d14; /* cmp/eq r1,r0 */
L8c620d14: sa_motion_publish_stop(ctx,0x8c620d14u); if((*sr&1u))goto L8c620d68;goto L8c620d16; /* bt 0x8c620d68 */
L8c620d16: sa_motion_publish_stop(ctx,0x8c620d16u); r[1]=rd(m,0x8c620e80u,4);goto L8c620d18; /* mov.l 0x8c620e80,r1 */
L8c620d18: sa_motion_publish_stop(ctx,0x8c620d18u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d1a; /* cmp/eq r1,r0 */
L8c620d1a: sa_motion_publish_stop(ctx,0x8c620d1au); if((*sr&1u))goto L8c620d74;goto L8c620d1c; /* bt 0x8c620d74 */
L8c620d1c: sa_motion_publish_stop(ctx,0x8c620d1cu); r[1]=rd(m,0x8c620e84u,4);goto L8c620d1e; /* mov.l 0x8c620e84,r1 */
L8c620d1e: sa_motion_publish_stop(ctx,0x8c620d1eu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d20; /* cmp/eq r1,r0 */
L8c620d20: sa_motion_publish_stop(ctx,0x8c620d20u); if((*sr&1u))goto L8c620d80;goto L8c620d22; /* bt 0x8c620d80 */
L8c620d22: sa_motion_publish_stop(ctx,0x8c620d22u); r[1]=rd(m,0x8c620e88u,4);goto L8c620d24; /* mov.l 0x8c620e88,r1 */
L8c620d24: sa_motion_publish_stop(ctx,0x8c620d24u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d26; /* cmp/eq r1,r0 */
L8c620d26: sa_motion_publish_stop(ctx,0x8c620d26u); if((*sr&1u))goto L8c620d8c;goto L8c620d28; /* bt 0x8c620d8c */
L8c620d28: sa_motion_publish_stop(ctx,0x8c620d28u); r[1]=rd(m,0x8c620e8cu,4);goto L8c620d2a; /* mov.l 0x8c620e8c,r1 */
L8c620d2a: sa_motion_publish_stop(ctx,0x8c620d2au); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d2c; /* cmp/eq r1,r0 */
L8c620d2c: sa_motion_publish_stop(ctx,0x8c620d2cu); if((*sr&1u))goto L8c620d8c;goto L8c620d2e; /* bt 0x8c620d8c */
L8c620d2e: sa_motion_publish_stop(ctx,0x8c620d2eu); r[1]=rd(m,0x8c620e90u,4);goto L8c620d30; /* mov.l 0x8c620e90,r1 */
L8c620d30: sa_motion_publish_stop(ctx,0x8c620d30u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d32; /* cmp/eq r1,r0 */
L8c620d32: sa_motion_publish_stop(ctx,0x8c620d32u); if((*sr&1u))goto L8c620d98;goto L8c620d34; /* bt 0x8c620d98 */
L8c620d34: sa_motion_publish_stop(ctx,0x8c620d34u); r[1]=rd(m,0x8c620e94u,4);goto L8c620d36; /* mov.l 0x8c620e94,r1 */
L8c620d36: sa_motion_publish_stop(ctx,0x8c620d36u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d38; /* cmp/eq r1,r0 */
L8c620d38: sa_motion_publish_stop(ctx,0x8c620d38u); if((*sr&1u))goto L8c620da4;goto L8c620d3a; /* bt 0x8c620da4 */
L8c620d3a: sa_motion_publish_stop(ctx,0x8c620d3au); r[1]=rd(m,0x8c620e98u,4);goto L8c620d3c; /* mov.l 0x8c620e98,r1 */
L8c620d3c: sa_motion_publish_stop(ctx,0x8c620d3cu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d3e; /* cmp/eq r1,r0 */
L8c620d3e: sa_motion_publish_stop(ctx,0x8c620d3eu); if((*sr&1u))goto L8c620da4;goto L8c620d40; /* bt 0x8c620da4 */
L8c620d40: sa_motion_publish_stop(ctx,0x8c620d40u); r[1]=rd(m,0x8c620e9cu,4);goto L8c620d42; /* mov.l 0x8c620e9c,r1 */
L8c620d42: sa_motion_publish_stop(ctx,0x8c620d42u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d44; /* cmp/eq r1,r0 */
L8c620d44: sa_motion_publish_stop(ctx,0x8c620d44u); if((*sr&1u))goto L8c620db0;goto L8c620d46; /* bt 0x8c620db0 */
L8c620d46: sa_motion_publish_stop(ctx,0x8c620d46u); r[1]=rd(m,0x8c620ea0u,4);goto L8c620d48; /* mov.l 0x8c620ea0,r1 */
L8c620d48: sa_motion_publish_stop(ctx,0x8c620d48u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620d4a; /* cmp/eq r1,r0 */
L8c620d4a: sa_motion_publish_stop(ctx,0x8c620d4au); if((*sr&1u))goto L8c620db0;goto L8c620d4c; /* bt 0x8c620db0 */
L8c620d4c: sa_motion_publish_stop(ctx,0x8c620d4eu); goto L8c620e70; /* bra 0x8c620e70 */
L8c620d50: sa_motion_publish_stop(ctx,0x8c620d50u); r[3]=rd(m,0x8c620ea4u,4);goto L8c620d52; /* mov.l 0x8c620ea4,r3 */
L8c620d52: sa_motion_publish_stop(ctx,0x8c620d52u); r[5]=r[13];goto L8c620d54; /* mov r13,r5 */
L8c620d54: sa_motion_publish_stop(ctx,0x8c620d56u); target=r[3];*pr=0x8c620d58u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d58; /* jsr @r3 */
L8c620d58: sa_motion_publish_stop(ctx,0x8c620d5au); goto L8c620e70; /* bra 0x8c620e70 */
L8c620d5c: sa_motion_publish_stop(ctx,0x8c620d5cu); r[3]=rd(m,0x8c620ea8u,4);goto L8c620d5e; /* mov.l 0x8c620ea8,r3 */
L8c620d5e: sa_motion_publish_stop(ctx,0x8c620d5eu); r[5]=r[13];goto L8c620d60; /* mov r13,r5 */
L8c620d60: sa_motion_publish_stop(ctx,0x8c620d62u); target=r[3];*pr=0x8c620d64u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d64; /* jsr @r3 */
L8c620d64: sa_motion_publish_stop(ctx,0x8c620d66u); goto L8c620e70; /* bra 0x8c620e70 */
L8c620d68: sa_motion_publish_stop(ctx,0x8c620d68u); r[3]=rd(m,0x8c620eacu,4);goto L8c620d6a; /* mov.l 0x8c620eac,r3 */
L8c620d6a: sa_motion_publish_stop(ctx,0x8c620d6au); r[5]=r[13];goto L8c620d6c; /* mov r13,r5 */
L8c620d6c: sa_motion_publish_stop(ctx,0x8c620d6eu); target=r[3];*pr=0x8c620d70u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d70; /* jsr @r3 */
L8c620d70: sa_motion_publish_stop(ctx,0x8c620d72u); goto L8c620e70; /* bra 0x8c620e70 */
L8c620d74: sa_motion_publish_stop(ctx,0x8c620d74u); r[3]=rd(m,0x8c620eb0u,4);goto L8c620d76; /* mov.l 0x8c620eb0,r3 */
L8c620d76: sa_motion_publish_stop(ctx,0x8c620d76u); r[5]=r[13];goto L8c620d78; /* mov r13,r5 */
L8c620d78: sa_motion_publish_stop(ctx,0x8c620d7au); target=r[3];*pr=0x8c620d7cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d7c; /* jsr @r3 */
L8c620d7c: sa_motion_publish_stop(ctx,0x8c620d7eu); goto L8c620e70; /* bra 0x8c620e70 */
L8c620d80: sa_motion_publish_stop(ctx,0x8c620d80u); r[3]=rd(m,0x8c620eb4u,4);goto L8c620d82; /* mov.l 0x8c620eb4,r3 */
L8c620d82: sa_motion_publish_stop(ctx,0x8c620d82u); r[5]=r[13];goto L8c620d84; /* mov r13,r5 */
L8c620d84: sa_motion_publish_stop(ctx,0x8c620d86u); target=r[3];*pr=0x8c620d88u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d88; /* jsr @r3 */
L8c620d88: sa_motion_publish_stop(ctx,0x8c620d8au); goto L8c620e70; /* bra 0x8c620e70 */
L8c620d8c: sa_motion_publish_stop(ctx,0x8c620d8cu); r[3]=rd(m,0x8c620eb8u,4);goto L8c620d8e; /* mov.l 0x8c620eb8,r3 */
L8c620d8e: sa_motion_publish_stop(ctx,0x8c620d8eu); r[5]=r[13];goto L8c620d90; /* mov r13,r5 */
L8c620d90: sa_motion_publish_stop(ctx,0x8c620d92u); target=r[3];*pr=0x8c620d94u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620d94; /* jsr @r3 */
L8c620d94: sa_motion_publish_stop(ctx,0x8c620d96u); goto L8c620e70; /* bra 0x8c620e70 */
L8c620d98: sa_motion_publish_stop(ctx,0x8c620d98u); r[3]=rd(m,0x8c620ebcu,4);goto L8c620d9a; /* mov.l 0x8c620ebc,r3 */
L8c620d9a: sa_motion_publish_stop(ctx,0x8c620d9au); r[5]=r[13];goto L8c620d9c; /* mov r13,r5 */
L8c620d9c: sa_motion_publish_stop(ctx,0x8c620d9eu); target=r[3];*pr=0x8c620da0u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620da0; /* jsr @r3 */
L8c620da0: sa_motion_publish_stop(ctx,0x8c620da2u); goto L8c620e70; /* bra 0x8c620e70 */
L8c620da4: sa_motion_publish_stop(ctx,0x8c620da4u); r[3]=rd(m,0x8c620ec0u,4);goto L8c620da6; /* mov.l 0x8c620ec0,r3 */
L8c620da6: sa_motion_publish_stop(ctx,0x8c620da6u); r[5]=r[13];goto L8c620da8; /* mov r13,r5 */
L8c620da8: sa_motion_publish_stop(ctx,0x8c620daau); target=r[3];*pr=0x8c620dacu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620dac; /* jsr @r3 */
L8c620dac: sa_motion_publish_stop(ctx,0x8c620daeu); goto L8c620e70; /* bra 0x8c620e70 */
L8c620db0: sa_motion_publish_stop(ctx,0x8c620db0u); r[3]=rd(m,0x8c620ec4u,4);goto L8c620db2; /* mov.l 0x8c620ec4,r3 */
L8c620db2: sa_motion_publish_stop(ctx,0x8c620db2u); r[5]=r[13];goto L8c620db4; /* mov r13,r5 */
L8c620db4: sa_motion_publish_stop(ctx,0x8c620db6u); target=r[3];*pr=0x8c620db8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620db8; /* jsr @r3 */
L8c620db8: sa_motion_publish_stop(ctx,0x8c620dbau); goto L8c620e70; /* bra 0x8c620e70 */
L8c620dbc: sa_motion_publish_stop(ctx,0x8c620dbcu); r[0]=rd(m,r[15]+0x8u,4);goto L8c620dbe; /* mov.l @(0x8,r15),r0 */
L8c620dbe: sa_motion_publish_stop(ctx,0x8c620dbeu); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c620dc0; /* cmp/eq #0x0,r0 */
L8c620dc0: sa_motion_publish_stop(ctx,0x8c620dc0u); if((*sr&1u))goto L8c620e08;goto L8c620dc2; /* bt 0x8c620e08 */
L8c620dc2: sa_motion_publish_stop(ctx,0x8c620dc2u); r[1]=rd(m,0x8c620e78u,4);goto L8c620dc4; /* mov.l 0x8c620e78,r1 */
L8c620dc4: sa_motion_publish_stop(ctx,0x8c620dc4u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dc6; /* cmp/eq r1,r0 */
L8c620dc6: sa_motion_publish_stop(ctx,0x8c620dc6u); if((*sr&1u))goto L8c620e14;goto L8c620dc8; /* bt 0x8c620e14 */
L8c620dc8: sa_motion_publish_stop(ctx,0x8c620dc8u); r[1]=rd(m,0x8c620e7cu,4);goto L8c620dca; /* mov.l 0x8c620e7c,r1 */
L8c620dca: sa_motion_publish_stop(ctx,0x8c620dcau); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dcc; /* cmp/eq r1,r0 */
L8c620dcc: sa_motion_publish_stop(ctx,0x8c620dccu); if((*sr&1u))goto L8c620e20;goto L8c620dce; /* bt 0x8c620e20 */
L8c620dce: sa_motion_publish_stop(ctx,0x8c620dceu); r[1]=rd(m,0x8c620e80u,4);goto L8c620dd0; /* mov.l 0x8c620e80,r1 */
L8c620dd0: sa_motion_publish_stop(ctx,0x8c620dd0u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dd2; /* cmp/eq r1,r0 */
L8c620dd2: sa_motion_publish_stop(ctx,0x8c620dd2u); if((*sr&1u))goto L8c620e2c;goto L8c620dd4; /* bt 0x8c620e2c */
L8c620dd4: sa_motion_publish_stop(ctx,0x8c620dd4u); r[1]=rd(m,0x8c620e84u,4);goto L8c620dd6; /* mov.l 0x8c620e84,r1 */
L8c620dd6: sa_motion_publish_stop(ctx,0x8c620dd6u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dd8; /* cmp/eq r1,r0 */
L8c620dd8: sa_motion_publish_stop(ctx,0x8c620dd8u); if((*sr&1u))goto L8c620e38;goto L8c620dda; /* bt 0x8c620e38 */
L8c620dda: sa_motion_publish_stop(ctx,0x8c620ddau); r[1]=rd(m,0x8c620e88u,4);goto L8c620ddc; /* mov.l 0x8c620e88,r1 */
L8c620ddc: sa_motion_publish_stop(ctx,0x8c620ddcu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dde; /* cmp/eq r1,r0 */
L8c620dde: sa_motion_publish_stop(ctx,0x8c620ddeu); if((*sr&1u))goto L8c620e44;goto L8c620de0; /* bt 0x8c620e44 */
L8c620de0: sa_motion_publish_stop(ctx,0x8c620de0u); r[1]=rd(m,0x8c620e8cu,4);goto L8c620de2; /* mov.l 0x8c620e8c,r1 */
L8c620de2: sa_motion_publish_stop(ctx,0x8c620de2u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620de4; /* cmp/eq r1,r0 */
L8c620de4: sa_motion_publish_stop(ctx,0x8c620de4u); if((*sr&1u))goto L8c620e44;goto L8c620de6; /* bt 0x8c620e44 */
L8c620de6: sa_motion_publish_stop(ctx,0x8c620de6u); r[1]=rd(m,0x8c620e90u,4);goto L8c620de8; /* mov.l 0x8c620e90,r1 */
L8c620de8: sa_motion_publish_stop(ctx,0x8c620de8u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dea; /* cmp/eq r1,r0 */
L8c620dea: sa_motion_publish_stop(ctx,0x8c620deau); if((*sr&1u))goto L8c620e50;goto L8c620dec; /* bt 0x8c620e50 */
L8c620dec: sa_motion_publish_stop(ctx,0x8c620decu); r[1]=rd(m,0x8c620e94u,4);goto L8c620dee; /* mov.l 0x8c620e94,r1 */
L8c620dee: sa_motion_publish_stop(ctx,0x8c620deeu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620df0; /* cmp/eq r1,r0 */
L8c620df0: sa_motion_publish_stop(ctx,0x8c620df0u); if((*sr&1u))goto L8c620e5c;goto L8c620df2; /* bt 0x8c620e5c */
L8c620df2: sa_motion_publish_stop(ctx,0x8c620df2u); r[1]=rd(m,0x8c620e98u,4);goto L8c620df4; /* mov.l 0x8c620e98,r1 */
L8c620df4: sa_motion_publish_stop(ctx,0x8c620df4u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620df6; /* cmp/eq r1,r0 */
L8c620df6: sa_motion_publish_stop(ctx,0x8c620df6u); if((*sr&1u))goto L8c620e5c;goto L8c620df8; /* bt 0x8c620e5c */
L8c620df8: sa_motion_publish_stop(ctx,0x8c620df8u); r[1]=rd(m,0x8c620e9cu,4);goto L8c620dfa; /* mov.l 0x8c620e9c,r1 */
L8c620dfa: sa_motion_publish_stop(ctx,0x8c620dfau); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620dfc; /* cmp/eq r1,r0 */
L8c620dfc: sa_motion_publish_stop(ctx,0x8c620dfcu); if((*sr&1u))goto L8c620e68;goto L8c620dfe; /* bt 0x8c620e68 */
L8c620dfe: sa_motion_publish_stop(ctx,0x8c620dfeu); r[1]=rd(m,0x8c620ea0u,4);goto L8c620e00; /* mov.l 0x8c620ea0,r1 */
L8c620e00: sa_motion_publish_stop(ctx,0x8c620e00u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620e02; /* cmp/eq r1,r0 */
L8c620e02: sa_motion_publish_stop(ctx,0x8c620e02u); if((*sr&1u))goto L8c620e68;goto L8c620e04; /* bt 0x8c620e68 */
L8c620e04: sa_motion_publish_stop(ctx,0x8c620e06u); goto L8c620e70; /* bra 0x8c620e70 */
L8c620e08: sa_motion_publish_stop(ctx,0x8c620e08u); r[3]=rd(m,0x8c620ec8u,4);goto L8c620e0a; /* mov.l 0x8c620ec8,r3 */
L8c620e0a: sa_motion_publish_stop(ctx,0x8c620e0au); r[5]=r[13];goto L8c620e0c; /* mov r13,r5 */
L8c620e0c: sa_motion_publish_stop(ctx,0x8c620e0eu); target=r[3];*pr=0x8c620e10u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e10; /* jsr @r3 */
L8c620e10: sa_motion_publish_stop(ctx,0x8c620e12u); goto L8c620e70; /* bra 0x8c620e70 */
L8c620e14: sa_motion_publish_stop(ctx,0x8c620e14u); r[3]=rd(m,0x8c620eccu,4);goto L8c620e16; /* mov.l 0x8c620ecc,r3 */
L8c620e16: sa_motion_publish_stop(ctx,0x8c620e16u); r[5]=r[13];goto L8c620e18; /* mov r13,r5 */
L8c620e18: sa_motion_publish_stop(ctx,0x8c620e1au); target=r[3];*pr=0x8c620e1cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e1c; /* jsr @r3 */
L8c620e1c: sa_motion_publish_stop(ctx,0x8c620e1eu); goto L8c620e70; /* bra 0x8c620e70 */
L8c620e20: sa_motion_publish_stop(ctx,0x8c620e20u); r[3]=rd(m,0x8c620ed0u,4);goto L8c620e22; /* mov.l 0x8c620ed0,r3 */
L8c620e22: sa_motion_publish_stop(ctx,0x8c620e22u); r[5]=r[13];goto L8c620e24; /* mov r13,r5 */
L8c620e24: sa_motion_publish_stop(ctx,0x8c620e26u); target=r[3];*pr=0x8c620e28u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e28; /* jsr @r3 */
L8c620e28: sa_motion_publish_stop(ctx,0x8c620e2au); goto L8c620e70; /* bra 0x8c620e70 */
L8c620e2c: sa_motion_publish_stop(ctx,0x8c620e2cu); r[3]=rd(m,0x8c620ed4u,4);goto L8c620e2e; /* mov.l 0x8c620ed4,r3 */
L8c620e2e: sa_motion_publish_stop(ctx,0x8c620e2eu); r[5]=r[13];goto L8c620e30; /* mov r13,r5 */
L8c620e30: sa_motion_publish_stop(ctx,0x8c620e32u); target=r[3];*pr=0x8c620e34u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e34; /* jsr @r3 */
L8c620e34: sa_motion_publish_stop(ctx,0x8c620e36u); goto L8c620e70; /* bra 0x8c620e70 */
L8c620e38: sa_motion_publish_stop(ctx,0x8c620e38u); r[3]=rd(m,0x8c620ed8u,4);goto L8c620e3a; /* mov.l 0x8c620ed8,r3 */
L8c620e3a: sa_motion_publish_stop(ctx,0x8c620e3au); r[5]=r[13];goto L8c620e3c; /* mov r13,r5 */
L8c620e3c: sa_motion_publish_stop(ctx,0x8c620e3eu); target=r[3];*pr=0x8c620e40u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e40; /* jsr @r3 */
L8c620e40: sa_motion_publish_stop(ctx,0x8c620e42u); goto L8c620e70; /* bra 0x8c620e70 */
L8c620e44: sa_motion_publish_stop(ctx,0x8c620e44u); r[3]=rd(m,0x8c620edcu,4);goto L8c620e46; /* mov.l 0x8c620edc,r3 */
L8c620e46: sa_motion_publish_stop(ctx,0x8c620e46u); r[5]=r[13];goto L8c620e48; /* mov r13,r5 */
L8c620e48: sa_motion_publish_stop(ctx,0x8c620e4au); target=r[3];*pr=0x8c620e4cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e4c; /* jsr @r3 */
L8c620e4c: sa_motion_publish_stop(ctx,0x8c620e4eu); goto L8c620e70; /* bra 0x8c620e70 */
L8c620e50: sa_motion_publish_stop(ctx,0x8c620e50u); r[3]=rd(m,0x8c620ee0u,4);goto L8c620e52; /* mov.l 0x8c620ee0,r3 */
L8c620e52: sa_motion_publish_stop(ctx,0x8c620e52u); r[5]=r[13];goto L8c620e54; /* mov r13,r5 */
L8c620e54: sa_motion_publish_stop(ctx,0x8c620e56u); target=r[3];*pr=0x8c620e58u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e58; /* jsr @r3 */
L8c620e58: sa_motion_publish_stop(ctx,0x8c620e5au); goto L8c620e70; /* bra 0x8c620e70 */
L8c620e5c: sa_motion_publish_stop(ctx,0x8c620e5cu); r[3]=rd(m,0x8c620ee4u,4);goto L8c620e5e; /* mov.l 0x8c620ee4,r3 */
L8c620e5e: sa_motion_publish_stop(ctx,0x8c620e5eu); r[5]=r[13];goto L8c620e60; /* mov r13,r5 */
L8c620e60: sa_motion_publish_stop(ctx,0x8c620e62u); target=r[3];*pr=0x8c620e64u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e64; /* jsr @r3 */
L8c620e64: sa_motion_publish_stop(ctx,0x8c620e66u); goto L8c620e70; /* bra 0x8c620e70 */
L8c620e68: sa_motion_publish_stop(ctx,0x8c620e68u); r[3]=rd(m,0x8c620ee8u,4);goto L8c620e6a; /* mov.l 0x8c620ee8,r3 */
L8c620e6a: sa_motion_publish_stop(ctx,0x8c620e6au); r[5]=r[13];goto L8c620e6c; /* mov r13,r5 */
L8c620e6c: sa_motion_publish_stop(ctx,0x8c620e6eu); target=r[3];*pr=0x8c620e70u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620e70; /* jsr @r3 */
L8c620e70: sa_motion_publish_stop(ctx,0x8c620e72u); goto L8c620eec; /* bra 0x8c620eec */
L8c620eec: sa_motion_publish_stop(ctx,0x8c620eecu); r[10]=rd(m,0x8c620f24u,4);goto L8c620eee; /* mov.l 0x8c620f24,r10 */
L8c620eee: sa_motion_publish_stop(ctx,0x8c620ef0u); target=r[10];*pr=0x8c620ef2u;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620ef2; /* jsr @r10 */
L8c620ef2: sa_motion_publish_stop(ctx,0x8c620ef2u); r[2]=rd(m,r[15],4);goto L8c620ef4; /* mov.l @r15,r2 */
L8c620ef4: sa_motion_publish_stop(ctx,0x8c620ef4u); r[12]+=24u;goto L8c620ef6; /* add #0x18,r12 */
L8c620ef6: sa_motion_publish_stop(ctx,0x8c620ef6u); r[2]+=1u;goto L8c620ef8; /* add #0x1,r2 */
L8c620ef8: sa_motion_publish_stop(ctx,0x8c620ef8u); wr(m,r[15],r[2],4);goto L8c620efa; /* mov.l r2,@r15 */
L8c620efa: sa_motion_publish_stop(ctx,0x8c620efau); r[0]=sx(m,r[9]+0x14u,2);goto L8c620efc; /* mov.w @(0x14,r9),r0 */
L8c620efc: sa_motion_publish_stop(ctx,0x8c620efcu); r[3]=rd(m,r[15],4);goto L8c620efe; /* mov.l @r15,r3 */
L8c620efe: sa_motion_publish_stop(ctx,0x8c620efeu); r[0]=r[0]&65535u;goto L8c620f00; /* extu.w r0,r0 */
L8c620f00: sa_motion_publish_stop(ctx,0x8c620f00u); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c620f02; /* cmp/ge r0,r3 */
L8c620f02: sa_motion_publish_stop(ctx,0x8c620f02u); if((*sr&1u))goto L8c620f0a;goto L8c620f04; /* bt 0x8c620f0a */
L8c620f04: sa_motion_publish_stop(ctx,0x8c620f04u); r[2]=rd(m,0x8c620f28u,4);goto L8c620f06; /* mov.l 0x8c620f28,r2 */
L8c620f06: sa_motion_publish_stop(ctx,0x8c620f08u); target=r[2];switch(target){case 0x8c620a30u:goto L8c620a30;case 0x8c620efau:goto L8c620efa;case 0x8c620f0au:goto L8c620f0a;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r2 */
L8c620f0a: sa_motion_publish_stop(ctx,0x8c620f0au); r[15]+=12u;goto L8c620f0c; /* add #0xc,r15 */
L8c620f0c: sa_motion_publish_stop(ctx,0x8c620f0cu); *pr=rd(m,r[15],4);r[15]+=4;goto L8c620f0e; /* lds.l @r15+,PR */
L8c620f0e: sa_motion_publish_stop(ctx,0x8c620f0eu); fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620f10; /* fmov.s @r15+,fr15 */
L8c620f10: sa_motion_publish_stop(ctx,0x8c620f10u); r[9]=rd(m,r[15],4);r[15]+=4;goto L8c620f12; /* mov.l @r15+,r9 */
L8c620f12: sa_motion_publish_stop(ctx,0x8c620f12u); r[10]=rd(m,r[15],4);r[15]+=4;goto L8c620f14; /* mov.l @r15+,r10 */
L8c620f14: sa_motion_publish_stop(ctx,0x8c620f14u); r[11]=rd(m,r[15],4);r[15]+=4;goto L8c620f16; /* mov.l @r15+,r11 */
L8c620f16: sa_motion_publish_stop(ctx,0x8c620f16u); r[12]=rd(m,r[15],4);r[15]+=4;goto L8c620f18; /* mov.l @r15+,r12 */
L8c620f18: sa_motion_publish_stop(ctx,0x8c620f18u); r[13]=rd(m,r[15],4);r[15]+=4;goto L8c620f1a; /* mov.l @r15+,r13 */
L8c620f1a: sa_motion_publish_stop(ctx,0x8c620f1cu); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_mesh_filter_wrapper_8c62159a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c62159a;
L8c62159a: sa_motion_publish_stop(ctx,0x8c62159au); r[15]-=4;wr(m,r[15],*pr,4);goto L8c62159c; /* sts.l PR,@-r15 */
L8c62159c: sa_motion_publish_stop(ctx,0x8c62159cu); r[15]+=4294967288u;goto L8c62159e; /* add #-0x8,r15 */
L8c62159e: sa_motion_publish_stop(ctx,0x8c62159eu); wr(m,r[15]+0x4u,r[4],4);goto L8c6215a0; /* mov.l r4,@(0x4,r15) */
L8c6215a0: sa_motion_publish_stop(ctx,0x8c6215a0u); r[4]=r[15];goto L8c6215a2; /* mov r15,r4 */
L8c6215a2: sa_motion_publish_stop(ctx,0x8c6215a2u); wr(m,r[4],r[8],4);goto L8c6215a4; /* mov.l r8,@r4 */
L8c6215a4: sa_motion_publish_stop(ctx,0x8c6215a4u); r[3]=rd(m,0x8c621628u,4);goto L8c6215a6; /* mov.l 0x8c621628,r3 */
L8c6215a6: sa_motion_publish_stop(ctx,0x8c6215a8u); target=r[3];*pr=0x8c6215aau;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6215aa; /* jsr @r3 */
L8c6215aa: sa_motion_publish_stop(ctx,0x8c6215aau); r[4]=r[15];goto L8c6215ac; /* mov r15,r4 */
L8c6215ac: sa_motion_publish_stop(ctx,0x8c6215acu); wr(m,r[15]+0x4u,r[0],4);goto L8c6215ae; /* mov.l r0,@(0x4,r15) */
L8c6215ae: sa_motion_publish_stop(ctx,0x8c6215aeu); r[8]=rd(m,r[4],4);goto L8c6215b0; /* mov.l @r4,r8 */
L8c6215b0: sa_motion_publish_stop(ctx,0x8c6215b0u); r[0]=rd(m,r[15]+0x4u,4);goto L8c6215b2; /* mov.l @(0x4,r15),r0 */
L8c6215b2: sa_motion_publish_stop(ctx,0x8c6215b2u); r[15]+=8u;goto L8c6215b4; /* add #0x8,r15 */
L8c6215b4: sa_motion_publish_stop(ctx,0x8c6215b4u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c6215b6; /* lds.l @r15+,PR */
L8c6215b6: sa_motion_publish_stop(ctx,0x8c6215b8u); return 0; /* rts */
}
