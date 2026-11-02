/* Original source-record initialization and scalar/vector dependencies; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
#include "sh4_vector_numeric.h"
#include "sh4_fmac.h"
#include "sh4_divide.h"
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
static int valid_ram(uint32_t a,unsigned n){
 return (a&0x1f000000u)==0x0c000000u && (a&0xffffffu)<=0x1000000u-n;
}

int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
SA_STAGE_EXPORT int sa_model_view_producer_8c63aa04(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)m;(void)sr;(void)wr;(void)fload;(void)fstore;(void)stop;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63aa04;
L8c63aa04: *stop=0x8c63aa04u;r[1]=17u;goto L8c63aa06; /* mov #0x11,r1 */
L8c63aa06: *stop=0x8c63aa06u;*sr=(*sr&~1u)|(r[4]>=r[1]);goto L8c63aa08; /* cmp/hs r1,r4 */
L8c63aa08: if((*sr&1u))goto L8c63aa5a;goto L8c63aa0a; /* bt 0x8c63aa5a */
L8c63aa0a: *stop=0x8c63aa0au;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c63aa0c; /* shll r4 */
L8c63aa0c: *stop=0x8c63aa0cu;r[0]=0x8c63aa14u;goto L8c63aa0e; /* mova 0x8c63aa14,r0 */
L8c63aa0e: *stop=0x8c63aa0eu;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto L8c63aa10; /* mov.w @(r0,r4),r0 */
L8c63aa10: target=0x8c63aa14u+r[0];*stop=0x8c63aa12u;switch(target){case 0x8c63aa36u:goto L8c63aa36;case 0x8c63aa3au:goto L8c63aa3a;case 0x8c63aa40u:goto L8c63aa40;case 0x8c63aa48u:goto L8c63aa48;case 0x8c63aa4au:goto L8c63aa4a;case 0x8c63aa4cu:goto L8c63aa4c;case 0x8c63aa4eu:goto L8c63aa4e;case 0x8c63aa50u:goto L8c63aa50;case 0x8c63aa52u:goto L8c63aa52;case 0x8c63aa5au:goto L8c63aa5a;case 0x8c63aa5cu:goto L8c63aa5c;case 0x8c63aa5eu:goto L8c63aa5e;case 0x8c63aa60u:goto L8c63aa60;case 0x8c63aa62u:goto L8c63aa62;case 0x8c63aa64u:goto L8c63aa64;case 0x8c63aa66u:goto L8c63aa66;case 0x8c63aa68u:goto L8c63aa68;default:*stop=0x8c63aa10u;return -3;} /* braf r0 */
L8c63aa36: *stop=0x8c63aa36u;if(*pr!=entry_pr)return -3;*stop=0x8c63aa38u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;return 0; /* rts */
L8c63aa3a: *stop=0x8c63aa3au;fcopy(f,0u,4u);goto L8c63aa3c; /* fmov fr4,fr0 */
L8c63aa3c: *stop=0x8c63aa3cu;if(*pr!=entry_pr)return -3;*stop=0x8c63aa3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[0],&f->fpscr,&fr[0]);if(status)return status;}return 0; /* rts */
L8c63aa40: *stop=0x8c63aa40u;fcopy(f,0u,4u);goto L8c63aa42; /* fmov fr4,fr0 */
L8c63aa42: *stop=0x8c63aa42u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa44; /* fsqrt fr4 */
L8c63aa44: *stop=0x8c63aa44u;if(*pr!=entry_pr)return -3;*stop=0x8c63aa46u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}return 0; /* rts */
L8c63aa48: *stop=0x8c63aa48u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa4a; /* fmul fr4,fr4 */
L8c63aa4a: *stop=0x8c63aa4au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa4c; /* fmul fr4,fr4 */
L8c63aa4c: *stop=0x8c63aa4cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa4e; /* fmul fr4,fr4 */
L8c63aa4e: *stop=0x8c63aa4eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa50; /* fmul fr4,fr4 */
L8c63aa50: *stop=0x8c63aa50u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa52; /* fmul fr4,fr4 */
L8c63aa52: *stop=0x8c63aa52u;fcopy(f,0u,4u);goto L8c63aa54; /* fmov fr4,fr0 */
L8c63aa54: *stop=0x8c63aa54u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c63aa56; /* fmul fr4,fr0 */
L8c63aa56: *stop=0x8c63aa56u;if(*pr!=entry_pr)return -3;*stop=0x8c63aa58u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}return 0; /* rts */
L8c63aa5a: *stop=0x8c63aa5au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa5c; /* fmul fr4,fr4 */
L8c63aa5c: *stop=0x8c63aa5cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa5e; /* fmul fr4,fr4 */
L8c63aa5e: *stop=0x8c63aa5eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa60; /* fmul fr4,fr4 */
L8c63aa60: *stop=0x8c63aa60u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa62; /* fmul fr4,fr4 */
L8c63aa62: *stop=0x8c63aa62u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa64; /* fmul fr4,fr4 */
L8c63aa64: *stop=0x8c63aa64u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa66; /* fmul fr4,fr4 */
L8c63aa66: *stop=0x8c63aa66u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63aa68; /* fmul fr4,fr4 */
L8c63aa68: *stop=0x8c63aa68u;fcopy(f,0u,4u);goto L8c63aa6a; /* fmov fr4,fr0 */
L8c63aa6a: *stop=0x8c63aa6au;if(*pr!=entry_pr)return -3;*stop=0x8c63aa6cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_model_view_producer_8c63aa74(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)m;(void)sr;(void)wr;(void)fload;(void)fstore;(void)stop;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63aa74;
L8c63aa74: *stop=0x8c63aa74u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63aa76; /* fmov.s @r5+,fr3 */
L8c63aa76: *stop=0x8c63aa76u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63aa78; /* fmul fr4,fr3 */
L8c63aa78: *stop=0x8c63aa78u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63aa7a; /* fmov.s @r5+,fr2 */
L8c63aa7a: *stop=0x8c63aa7au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63aa7c; /* fmul fr4,fr2 */
L8c63aa7c: *stop=0x8c63aa7cu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63aa7e; /* fmov.s @r5+,fr1 */
L8c63aa7e: *stop=0x8c63aa7eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63aa80; /* fmul fr4,fr1 */
L8c63aa80: *stop=0x8c63aa80u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63aa82; /* fmov.s @r5+,fr0 */
L8c63aa82: *stop=0x8c63aa82u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c63aa84; /* fmul fr4,fr0 */
L8c63aa84: *stop=0x8c63aa84u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[4]);goto L8c63aa86; /* fmov.s fr3,@r4 */
L8c63aa86: *stop=0x8c63aa86u;r[4]+=4u;goto L8c63aa88; /* add #0x4,r4 */
L8c63aa88: *stop=0x8c63aa88u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[4]);goto L8c63aa8a; /* fmov.s fr2,@r4 */
L8c63aa8a: *stop=0x8c63aa8au;r[4]+=4u;goto L8c63aa8c; /* add #0x4,r4 */
L8c63aa8c: *stop=0x8c63aa8cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[4]);goto L8c63aa8e; /* fmov.s fr1,@r4 */
L8c63aa8e: *stop=0x8c63aa8eu;r[4]+=4u;goto L8c63aa90; /* add #0x4,r4 */
L8c63aa90: *stop=0x8c63aa90u;if(*pr!=entry_pr)return -3;*stop=0x8c63aa92u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_model_view_producer_8c63aa94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)m;(void)sr;(void)wr;(void)fload;(void)fstore;(void)stop;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c63aa94;
L8c63aa94: *stop=0x8c63aa94u;r[0]=sx(m,0x8c63aaa6u,2);goto L8c63aa96; /* mov.w 0x8c63aaa6,r0 */
L8c63aa96: *stop=0x8c63aa96u;r[3]=rd(m,0x8c63aaa8u,4);goto L8c63aa98; /* mov.l 0x8c63aaa8,r3 */
L8c63aa98: *stop=0x8c63aa98u;r[1]=32u;goto L8c63aa9a; /* mov #0x20,r1 */
L8c63aa9a: *stop=0x8c63aa9au;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c63aa9c; /* mov.l @(r0,r3),r2 */
L8c63aa9c: *stop=0x8c63aa9cu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[1]);goto L8c63aa9e; /* cmp/ge r1,r2 */
L8c63aa9e: take=!(*sr&1u);*stop=0x8c63aaa0u;r[1]=r[2];if(take)goto L8c63aaac;goto L8c63aaa2; /* bf/s 0x8c63aaac */
L8c63aaa2: *stop=0x8c63aaa2u;if(*pr!=entry_pr)return -3;*stop=0x8c63aaa4u;return 0; /* rts */
L8c63aaac: *stop=0x8c63aaacu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c63aaae; /* mov.l r14,@-r15 */
L8c63aaae: *stop=0x8c63aaaeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c63aab0; /* sts.l PR,@-r15 */
L8c63aab0: *stop=0x8c63aab0u;r[14]=r[4];goto L8c63aab2; /* mov r4,r14 */
L8c63aab2: *stop=0x8c63aab2u;r[1]<<=2;goto L8c63aab4; /* shll2 r1 */
L8c63aab4: *stop=0x8c63aab4u;r[1]+=r[3];goto L8c63aab6; /* add r3,r1 */
L8c63aab6: *stop=0x8c63aab6u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[4],4);goto L8c63aab8; /* mov.l r4,@r1 */
L8c63aab8: *stop=0x8c63aab8u;r[2]+=1u;goto L8c63aaba; /* add #0x1,r2 */
L8c63aaba: *stop=0x8c63aabau;if(!valid_ram(r[0]+r[3],4u))return -3;wr(m,r[3]+r[0],r[2],4);goto L8c63aabc; /* mov.l r2,@(r0,r3) */
L8c63aabc: *stop=0x8c63aabcu;r[7]=sx(m,0x8c63ab98u,2);goto L8c63aabe; /* mov.w 0x8c63ab98,r7 */
L8c63aabe: *stop=0x8c63aabeu;r[6]=sx(m,0x8c63ab9au,2);goto L8c63aac0; /* mov.w 0x8c63ab9a,r6 */
L8c63aac0: *stop=0x8c63aac0u;r[7]+=r[4];goto L8c63aac2; /* add r4,r7 */
L8c63aac2: *stop=0x8c63aac2u;r[6]+=r[4];goto L8c63aac4; /* add r4,r6 */
L8c63aac4: *stop=0x8c63aac4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]=0x3f800000u;goto L8c63aac6; /* fldi1 fr11 */
L8c63aac6: *stop=0x8c63aac6u;r[1]=1u;goto L8c63aac8; /* mov #0x1,r1 */
L8c63aac8: *stop=0x8c63aac8u;fcopy(f,10u,11u);goto L8c63aaca; /* fmov fr11,fr10 */
L8c63aaca: *stop=0x8c63aacau;r[3]=88u;goto L8c63aacc; /* mov #0x58,r3 */
L8c63aacc: *stop=0x8c63aaccu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][9]=0;goto L8c63aace; /* fldi0 fr9 */
L8c63aace: *stop=0x8c63aaceu;r[3]+=r[4];goto L8c63aad0; /* add r4,r3 */
L8c63aad0: *stop=0x8c63aad0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]^=0x80000000u;goto L8c63aad2; /* fneg fr10 */
L8c63aad2: *stop=0x8c63aad2u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[1],4);goto L8c63aad4; /* mov.l r1,@r3 */
L8c63aad4: *stop=0x8c63aad4u;r[3]+=4294967292u;goto L8c63aad6; /* add #-0x4,r3 */
L8c63aad6: *stop=0x8c63aad6u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,10u,r[3]);goto L8c63aad8; /* fmov.s fr10,@r3 */
L8c63aad8: *stop=0x8c63aad8u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63aada; /* fmov.s fr9,@-r3 */
L8c63aada: *stop=0x8c63aadau;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63aadc; /* fmov.s fr9,@-r3 */
L8c63aadc: *stop=0x8c63aadcu;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63aade; /* fmov.s fr9,@-r3 */
L8c63aade: *stop=0x8c63aadeu;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63aae0; /* fmov.s fr9,@-r3 */
L8c63aae0: *stop=0x8c63aae0u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63aae2; /* fmov.s fr9,@-r3 */
L8c63aae2: *stop=0x8c63aae2u;r[2]=sx(m,0x8c63ab9cu,2);goto L8c63aae4; /* mov.w 0x8c63ab9c,r2 */
L8c63aae4: *stop=0x8c63aae4u;r[1]=sx(m,0x8c63ab9eu,2);goto L8c63aae6; /* mov.w 0x8c63ab9e,r1 */
L8c63aae6: *stop=0x8c63aae6u;r[0]=0x8c63aba4u;goto L8c63aae8; /* mova 0x8c63aba4,r0 */
L8c63aae8: *stop=0x8c63aae8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c63aaea; /* fmov.s @r0,fr3 */
L8c63aaea: *stop=0x8c63aaeau;r[0]=0x8c63aba8u;goto L8c63aaec; /* mova 0x8c63aba8,r0 */
L8c63aaec: *stop=0x8c63aaecu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c63aaee; /* fmov.s @r0,fr2 */
L8c63aaee: *stop=0x8c63aaeeu;r[3]=48u;goto L8c63aaf0; /* mov #0x30,r3 */
L8c63aaf0: *stop=0x8c63aaf0u;r[3]+=r[7];goto L8c63aaf2; /* add r7,r3 */
L8c63aaf2: *stop=0x8c63aaf2u;r[0]=0x8c63abacu;goto L8c63aaf4; /* mova 0x8c63abac,r0 */
L8c63aaf4: *stop=0x8c63aaf4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto L8c63aaf6; /* fmov.s @r0,fr1 */
L8c63aaf6: *stop=0x8c63aaf6u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,11u,r[3]);goto L8c63aaf8; /* fmov.s fr11,@r3 */
L8c63aaf8: *stop=0x8c63aaf8u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[3]);goto L8c63aafa; /* fmov.s fr11,@-r3 */
L8c63aafa: *stop=0x8c63aafau;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[3]);goto L8c63aafc; /* fmov.s fr11,@-r3 */
L8c63aafc: *stop=0x8c63aafcu;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63aafe; /* fmov.s fr9,@-r3 */
L8c63aafe: *stop=0x8c63aafeu;if(!valid_ram(r[3]-4u,4u))return -3;r[3]-=4;wr(m,r[3],r[2],4);goto L8c63ab00; /* mov.l r2,@-r3 */
L8c63ab00: *stop=0x8c63ab00u;if(!valid_ram(r[3]-4u,4u))return -3;r[3]-=4;wr(m,r[3],r[1],4);goto L8c63ab02; /* mov.l r1,@-r3 */
L8c63ab02: *stop=0x8c63ab02u;r[3]+=4294967292u;goto L8c63ab04; /* add #-0x4,r3 */
L8c63ab04: *stop=0x8c63ab04u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[3]);goto L8c63ab06; /* fmov.s fr3,@-r3 */
L8c63ab06: *stop=0x8c63ab06u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[3]);goto L8c63ab08; /* fmov.s fr11,@-r3 */
L8c63ab08: *stop=0x8c63ab08u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[3]);goto L8c63ab0a; /* fmov.s fr2,@-r3 */
L8c63ab0a: *stop=0x8c63ab0au;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[3]);goto L8c63ab0c; /* fmov.s fr11,@-r3 */
L8c63ab0c: *stop=0x8c63ab0cu;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[3]);goto L8c63ab0e; /* fmov.s fr1,@-r3 */
L8c63ab0e: *stop=0x8c63ab0eu;if(!valid_ram(r[3]-4u,4u))return -3;r[3]-=4;wr(m,r[3],r[5],4);goto L8c63ab10; /* mov.l r5,@-r3 */
L8c63ab10: *stop=0x8c63ab10u;r[3]=68u;goto L8c63ab12; /* mov #0x44,r3 */
L8c63ab12: *stop=0x8c63ab12u;r[3]+=r[6];goto L8c63ab14; /* add r6,r3 */
L8c63ab14: *stop=0x8c63ab14u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,10u,r[3]);goto L8c63ab16; /* fmov.s fr10,@r3 */
L8c63ab16: *stop=0x8c63ab16u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63ab18; /* fmov.s fr9,@-r3 */
L8c63ab18: *stop=0x8c63ab18u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63ab1a; /* fmov.s fr9,@-r3 */
L8c63ab1a: *stop=0x8c63ab1au;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63ab1c; /* fmov.s fr9,@-r3 */
L8c63ab1c: *stop=0x8c63ab1cu;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63ab1e; /* fmov.s fr9,@-r3 */
L8c63ab1e: *stop=0x8c63ab1eu;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[3]);goto L8c63ab20; /* fmov.s fr9,@-r3 */
L8c63ab20: *stop=0x8c63ab20u;r[0]=16u;goto L8c63ab22; /* mov #0x10,r0 */
L8c63ab22: *stop=0x8c63ab22u;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[7]);goto L8c63ab24; /* fmov.s @(r0,r7),fr4 */
L8c63ab24: *stop=0x8c63ab24u;r[0]=20u;goto L8c63ab26; /* mov #0x14,r0 */
L8c63ab26: *stop=0x8c63ab26u;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[7]);goto L8c63ab28; /* fmov.s @(r0,r7),fr3 */
L8c63ab28: *stop=0x8c63ab28u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63ab2a; /* fmul fr4,fr4 */
L8c63ab2a: *stop=0x8c63ab2au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63ab2c; /* fmul fr3,fr3 */
L8c63ab2c: *stop=0x8c63ab2cu;r[0]=8u;goto L8c63ab2e; /* mov #0x8,r0 */
L8c63ab2e: *stop=0x8c63ab2eu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[6]);goto L8c63ab30; /* fmov fr4,@(r0,r6) */
L8c63ab30: *stop=0x8c63ab30u;r[0]=12u;goto L8c63ab32; /* mov #0xc,r0 */
L8c63ab32: *stop=0x8c63ab32u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c63ab34; /* fmov fr3,@(r0,r6) */
L8c63ab34: *stop=0x8c63ab34u;*fpul=r[1];goto L8c63ab36; /* lds r1,FPUL */
L8c63ab36: *stop=0x8c63ab36u;status=sa_sh4_vector_numeric(0xf0fdu,f,*fpul);if(status)return status;goto L8c63ab38; /* fsca FPUL,dr0 */
L8c63ab38: *stop=0x8c63ab38u;*fpul=r[2];goto L8c63ab3a; /* lds r2,FPUL */
L8c63ab3a: *stop=0x8c63ab3au;status=sa_sh4_vector_numeric(0xf2fdu,f,*fpul);if(status)return status;goto L8c63ab3c; /* fsca FPUL,dr2 */
L8c63ab3c: *stop=0x8c63ab3cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63ab3e; /* fmul fr1,fr1 */
L8c63ab3e: *stop=0x8c63ab3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63ab40; /* fmul fr3,fr3 */
L8c63ab40: *stop=0x8c63ab40u;r[0]=16u;goto L8c63ab42; /* mov #0x10,r0 */
L8c63ab42: *stop=0x8c63ab42u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[6]);goto L8c63ab44; /* fmov fr1,@(r0,r6) */
L8c63ab44: *stop=0x8c63ab44u;r[0]=20u;goto L8c63ab46; /* mov #0x14,r0 */
L8c63ab46: *stop=0x8c63ab46u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c63ab48; /* fmov fr3,@(r0,r6) */
L8c63ab48: *stop=0x8c63ab48u;r[0]=16u;goto L8c63ab4a; /* mov #0x10,r0 */
L8c63ab4a: *stop=0x8c63ab4au;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[7]);goto L8c63ab4c; /* fmov.s @(r0,r7),fr4 */
L8c63ab4c: *stop=0x8c63ab4cu;r[0]=20u;goto L8c63ab4e; /* mov #0x14,r0 */
L8c63ab4e: *stop=0x8c63ab4eu;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[7]);goto L8c63ab50; /* fmov.s @(r0,r7),fr3 */
L8c63ab50: *stop=0x8c63ab50u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c63ab52; /* fsub fr3,fr4 */
L8c63ab52: *stop=0x8c63ab52u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c63ab54; /* fneg fr4 */
L8c63ab54: *stop=0x8c63ab54u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c63ab56; /* fldi1 fr2 */
L8c63ab56: *stop=0x8c63ab56u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63ab58; /* fdiv fr4,fr2 */
L8c63ab58: *stop=0x8c63ab58u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63ab5a; /* fmul fr2,fr3 */
L8c63ab5a: *stop=0x8c63ab5au;r[0]=24u;goto L8c63ab5c; /* mov #0x18,r0 */
L8c63ab5c: *stop=0x8c63ab5cu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[6]);goto L8c63ab5e; /* fmov fr2,@(r0,r6) */
L8c63ab5e: *stop=0x8c63ab5eu;r[0]=28u;goto L8c63ab60; /* mov #0x1c,r0 */
L8c63ab60: *stop=0x8c63ab60u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c63ab62; /* fmov fr3,@(r0,r6) */
L8c63ab62: *stop=0x8c63ab62u;r[0]=44u;goto L8c63ab64; /* mov #0x2c,r0 */
L8c63ab64: *stop=0x8c63ab64u;if(!valid_ram(r[0]+r[6],2u))return -3;wr(m,r[0]+r[6],r[2],2);goto L8c63ab66; /* mov.w r2,@(r0,r6) */
L8c63ab66: *stop=0x8c63ab66u;r[0]+=2u;goto L8c63ab68; /* add #0x2,r0 */
L8c63ab68: *stop=0x8c63ab68u;if(!valid_ram(r[0]+r[6],2u))return -3;wr(m,r[0]+r[6],r[1],2);goto L8c63ab6a; /* mov.w r1,@(r0,r6) */
L8c63ab6a: *stop=0x8c63ab6au;r[3]=rd(m,0x8c63abb0u,4);goto L8c63ab6c; /* mov.l 0x8c63abb0,r3 */
L8c63ab6c: *stop=0x8c63ab6cu;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[3]);goto L8c63ab6e; /* fmov.s @r3,fr4 */
L8c63ab6e: *stop=0x8c63ab6eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,fpul);if(status)return status;goto L8c63ab70; /* ftrc fr4,FPUL */
L8c63ab70: *stop=0x8c63ab70u;r[0]=40u;goto L8c63ab72; /* mov #0x28,r0 */
L8c63ab72: *stop=0x8c63ab72u;r[1]=*fpul;goto L8c63ab74; /* sts FPUL,r1 */
L8c63ab74: *stop=0x8c63ab74u;if(!valid_ram(r[0]+r[6],4u))return -3;wr(m,r[6]+r[0],r[1],4);goto L8c63ab76; /* mov.l r1,@(r0,r6) */
L8c63ab76: *stop=0x8c63ab76u;r[0]=36u;goto L8c63ab78; /* mov #0x24,r0 */
L8c63ab78: *stop=0x8c63ab78u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,11u,r[0]+r[6]);goto L8c63ab7a; /* fmov fr11,@(r0,r6) */
L8c63ab7a: *stop=0x8c63ab7au;r[2]=sx(m,0x8c63aba0u,2);goto L8c63ab7c; /* mov.w 0x8c63aba0,r2 */
L8c63ab7c: *stop=0x8c63ab7cu;*sr=(*sr&~1u)|(r[2]==r[5]);goto L8c63ab7e; /* cmp/eq r2,r5 */
L8c63ab7e: if((*sr&1u))goto L8c63ab86;goto L8c63ab80; /* bt 0x8c63ab86 */
L8c63ab80: *stop=0x8c63ab80u;r[1]=sx(m,0x8c63aba2u,2);goto L8c63ab82; /* mov.w 0x8c63aba2,r1 */
L8c63ab82: *stop=0x8c63ab82u;*sr=(*sr&~1u)|(r[1]==r[5]);goto L8c63ab84; /* cmp/eq r1,r5 */
L8c63ab84: if(!(*sr&1u))goto L8c63abb4;goto L8c63ab86; /* bf 0x8c63abb4 */
L8c63ab86: *stop=0x8c63ab86u;r[0]=20u;goto L8c63ab88; /* mov #0x14,r0 */
L8c63ab88: *stop=0x8c63ab88u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto L8c63ab8a; /* fmov.s @(r0,r6),fr3 */
L8c63ab8a: *stop=0x8c63ab8au;r[0]+=4294967292u;goto L8c63ab8c; /* add #-0x4,r0 */
L8c63ab8c: *stop=0x8c63ab8cu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[6]);goto L8c63ab8e; /* fmov.s @(r0,r6),fr2 */
L8c63ab8e: *stop=0x8c63ab8eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c63ab90; /* fsub fr3,fr2 */
L8c63ab90: *stop=0x8c63ab90u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63ab92; /* fmul fr2,fr2 */
L8c63ab92: *stop=0x8c63ab92u;status=sa_sh4_vector_numeric(0xf27du,f,*fpul);if(status)return status;goto L8c63ab94; /* fsrra fr2 */
L8c63ab94: *stop=0x8c63ab96u;r[0]+=16u;goto L8c63abc6; /* bra 0x8c63abc6 */
L8c63abb4: *stop=0x8c63abb4u;r[0]=40u;goto L8c63abb6; /* mov #0x28,r0 */
L8c63abb6: *stop=0x8c63abb6u;if(!valid_ram(r[0]+r[6],4u))return -3;r[4]=rd(m,r[6]+r[0],4);goto L8c63abb8; /* mov.l @(r0,r6),r4 */
L8c63abb8: *stop=0x8c63abb8u;r[0]+=4294967272u;goto L8c63abba; /* add #-0x18,r0 */
L8c63abba: target=0x8c63aa04u;*pr=0x8c63abbeu;*stop=0x8c63abbcu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[6]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63abbe; /* bsr 0x8c63aa04 */
L8c63abbe: *stop=0x8c63abbeu;fcopy(f,2u,0u);goto L8c63abc0; /* fmov fr0,fr2 */
L8c63abc0: *stop=0x8c63abc0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63abc2; /* fmul fr2,fr2 */
L8c63abc2: *stop=0x8c63abc2u;status=sa_sh4_vector_numeric(0xf27du,f,*fpul);if(status)return status;goto L8c63abc4; /* fsrra fr2 */
L8c63abc4: *stop=0x8c63abc4u;r[0]=32u;goto L8c63abc6; /* mov #0x20,r0 */
L8c63abc6: *stop=0x8c63abc6u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[6]);goto L8c63abc8; /* fmov fr2,@(r0,r6) */
L8c63abc8: *stop=0x8c63abc8u;r[0]=36u;goto L8c63abca; /* mov #0x24,r0 */
L8c63abca: *stop=0x8c63abcau;r[4]=84u;goto L8c63abcc; /* mov #0x54,r4 */
L8c63abcc: *stop=0x8c63abccu;r[5]=36u;goto L8c63abce; /* mov #0x24,r5 */
L8c63abce: *stop=0x8c63abceu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[6]);goto L8c63abd0; /* fmov.s @(r0,r6),fr4 */
L8c63abd0: *stop=0x8c63abd0u;r[5]+=r[7];goto L8c63abd2; /* add r7,r5 */
L8c63abd2: target=0x8c63aa74u;*pr=0x8c63abd6u;*stop=0x8c63abd4u;r[4]+=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63abd6; /* bsr 0x8c63aa74 */
L8c63abd6: *stop=0x8c63abd6u;r[0]=4u;goto L8c63abd8; /* mov #0x4,r0 */
L8c63abd8: *stop=0x8c63abd8u;r[5]=84u;goto L8c63abda; /* mov #0x54,r5 */
L8c63abda: *stop=0x8c63abdau;r[4]=16u;goto L8c63abdc; /* mov #0x10,r4 */
L8c63abdc: *stop=0x8c63abdcu;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[7]);goto L8c63abde; /* fmov.s @(r0,r7),fr4 */
L8c63abde: *stop=0x8c63abdeu;r[5]+=r[6];goto L8c63abe0; /* add r6,r5 */
L8c63abe0: target=0x8c63aa74u;*pr=0x8c63abe4u;*stop=0x8c63abe2u;r[4]+=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63abe4; /* bsr 0x8c63aa74 */
L8c63abe4: *stop=0x8c63abe4u;r[0]=8u;goto L8c63abe6; /* mov #0x8,r0 */
L8c63abe6: *stop=0x8c63abe6u;r[5]=84u;goto L8c63abe8; /* mov #0x54,r5 */
L8c63abe8: *stop=0x8c63abe8u;r[4]=32u;goto L8c63abea; /* mov #0x20,r4 */
L8c63abea: *stop=0x8c63abeau;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[7]);goto L8c63abec; /* fmov.s @(r0,r7),fr4 */
L8c63abec: *stop=0x8c63abecu;r[5]+=r[6];goto L8c63abee; /* add r6,r5 */
L8c63abee: target=0x8c63aa74u;*pr=0x8c63abf2u;*stop=0x8c63abf0u;r[4]+=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63abf2; /* bsr 0x8c63aa74 */
L8c63abf2: *stop=0x8c63abf2u;r[0]=12u;goto L8c63abf4; /* mov #0xc,r0 */
L8c63abf4: *stop=0x8c63abf4u;r[5]=84u;goto L8c63abf6; /* mov #0x54,r5 */
L8c63abf6: *stop=0x8c63abf6u;r[4]=48u;goto L8c63abf8; /* mov #0x30,r4 */
L8c63abf8: *stop=0x8c63abf8u;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[7]);goto L8c63abfa; /* fmov.s @(r0,r7),fr4 */
L8c63abfa: *stop=0x8c63abfau;r[5]+=r[6];goto L8c63abfc; /* add r6,r5 */
L8c63abfc: target=0x8c63aa74u;*pr=0x8c63ac00u;*stop=0x8c63abfeu;r[4]+=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63ac00; /* bsr 0x8c63aa74 */
L8c63ac00: *stop=0x8c63ac00u;r[2]=rd(m,0x8c63ac0cu,4);goto L8c63ac02; /* mov.l 0x8c63ac0c,r2 */
L8c63ac02: *stop=0x8c63ac02u;r[4]=r[14];goto L8c63ac04; /* mov r14,r4 */
L8c63ac04: *stop=0x8c63ac04u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c63ac06; /* lds.l @r15+,PR */
L8c63ac06: *stop=0x8c63ac06u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;goto L8c63ac08; /* mov.l @r15+,r14 */
L8c63ac08: target=r[2];*stop=0x8c63ac0au;r[4]+=0u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}
