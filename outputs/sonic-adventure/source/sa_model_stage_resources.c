/* Original USA stage resource/control entries; names inferred from original behavior.
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
SA_STAGE_EXPORT int sa_stage_resource_files_8c09ce90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c09ce90;
L8c09ce90: *stop=0x8c09ce90u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09ce92; /* mov.l r14,@-r15 */
L8c09ce92: *stop=0x8c09ce92u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c09ce94; /* mov.l r13,@-r15 */
L8c09ce94: *stop=0x8c09ce94u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c09ce96; /* mov.l r12,@-r15 */
L8c09ce96: *stop=0x8c09ce96u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c09ce98; /* mov.l r11,@-r15 */
L8c09ce98: *stop=0x8c09ce98u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c09ce9a; /* mov.l r10,@-r15 */
L8c09ce9a: *stop=0x8c09ce9au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c09ce9c; /* mov.l r9,@-r15 */
L8c09ce9c: *stop=0x8c09ce9cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c09ce9e; /* mov.l r8,@-r15 */
L8c09ce9e: *stop=0x8c09ce9eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c09cea0; /* sts.l PR,@-r15 */
L8c09cea0: *stop=0x8c09cea0u;r[1]=rd(m,0x8c09ced8u,4);goto L8c09cea2; /* mov.l 0x8c09ced8,r1 */
L8c09cea2: target=r[1];*pr=0x8c09cea6u;*stop=0x8c09cea4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cea6; /* jsr @r1 */
L8c09cea6: *stop=0x8c09cea6u;r[0]=r[0]&65535u;goto L8c09cea8; /* extu.w r0,r0 */
L8c09cea8: *stop=0x8c09cea8u;r[4]=r[0];goto L8c09ceaa; /* mov r0,r4 */
L8c09ceaa: *stop=0x8c09ceaau;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c09ceac; /* cmp/pz r4 */
L8c09ceac: if(!(*sr&1u))goto L8c09cee0;goto L8c09ceae; /* bf 0x8c09cee0 */
L8c09ceae: *stop=0x8c09ceb0u;r[4]>>=8;goto L8c09cee6; /* bra 0x8c09cee6 */
L8c09cee0: *stop=0x8c09cee0u;r[4]=~r[4];goto L8c09cee2; /* not r4,r4 */
L8c09cee2: *stop=0x8c09cee2u;r[4]>>=8;goto L8c09cee4; /* shlr8 r4 */
L8c09cee4: *stop=0x8c09cee4u;r[4]=~r[4];goto L8c09cee6; /* not r4,r4 */
L8c09cee6: *stop=0x8c09cee6u;r[12]=rd(m,0x8c09cf1cu,4);goto L8c09cee8; /* mov.l 0x8c09cf1c,r12 */
L8c09cee8: *stop=0x8c09cee8u;r[0]=r[4]&65535u;goto L8c09ceea; /* extu.w r4,r0 */
L8c09ceea: *stop=0x8c09ceeau;r[13]=rd(m,0x8c09cf20u,4);goto L8c09ceec; /* mov.l 0x8c09cf20,r13 */
L8c09ceec: *stop=0x8c09ceecu;r[14]=rd(m,0x8c09cf24u,4);goto L8c09ceee; /* mov.l 0x8c09cf24,r14 */
L8c09ceee: *stop=0x8c09ceeeu;r[11]=rd(m,0x8c09cf18u,4);goto L8c09cef0; /* mov.l 0x8c09cf18,r11 */
L8c09cef0: *stop=0x8c09cef0u;r[8]=rd(m,0x8c09cf0cu,4);goto L8c09cef2; /* mov.l 0x8c09cf0c,r8 */
L8c09cef2: *stop=0x8c09cef2u;r[9]=rd(m,0x8c09cf10u,4);goto L8c09cef4; /* mov.l 0x8c09cf10,r9 */
L8c09cef4: *stop=0x8c09cef4u;r[10]=rd(m,0x8c09cf14u,4);goto L8c09cef6; /* mov.l 0x8c09cf14,r10 */
L8c09cef6: *stop=0x8c09cef6u;r[1]=43u;goto L8c09cef8; /* mov #0x2b,r1 */
L8c09cef8: *stop=0x8c09cef8u;*sr=(*sr&~1u)|(r[0]>=r[1]);goto L8c09cefa; /* cmp/hs r1,r0 */
L8c09cefa: if(!(*sr&1u))goto L8c09cf00;goto L8c09cefc; /* bf 0x8c09cf00 */
L8c09cefc: *stop=0x8c09cefeu;goto L8c09d926; /* bra 0x8c09d926 */
L8c09cf00: *stop=0x8c09cf00u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c09cf02; /* shll r0 */
L8c09cf02: *stop=0x8c09cf02u;r[1]=r[0];goto L8c09cf04; /* mov r0,r1 */
L8c09cf04: *stop=0x8c09cf04u;r[0]=0x8c09cf28u;goto L8c09cf06; /* mova 0x8c09cf28,r0 */
L8c09cf06: *stop=0x8c09cf06u;if(!valid_ram(r[0]+r[1],2u))return -3;r[0]=sx(m,r[0]+r[1],2);goto L8c09cf08; /* mov.w @(r0,r1),r0 */
L8c09cf08: target=0x8c09cf0cu+r[0];*stop=0x8c09cf0au;switch(target){case 0x8c09cf7eu:goto L8c09cf7e;case 0x8c09cf94u:goto L8c09cf94;case 0x8c09cfbeu:goto L8c09cfbe;case 0x8c09cfe8u:goto L8c09cfe8;case 0x8c09d012u:goto L8c09d012;case 0x8c09d03cu:goto L8c09d03c;case 0x8c09d0e0u:goto L8c09d0e0;case 0x8c09d10au:goto L8c09d10a;case 0x8c09d134u:goto L8c09d134;case 0x8c09d16eu:goto L8c09d16e;case 0x8c09d1a8u:goto L8c09d1a8;case 0x8c09d280u:goto L8c09d280;case 0x8c09d2bau:goto L8c09d2ba;case 0x8c09d2ccu:goto L8c09d2cc;case 0x8c09d2deu:goto L8c09d2de;case 0x8c09d2f0u:goto L8c09d2f0;case 0x8c09d308u:goto L8c09d308;case 0x8c09d330u:goto L8c09d330;case 0x8c09d3c0u:goto L8c09d3c0;case 0x8c09d3d2u:goto L8c09d3d2;case 0x8c09d3e4u:goto L8c09d3e4;case 0x8c09d3f6u:goto L8c09d3f6;case 0x8c09d408u:goto L8c09d408;case 0x8c09d41au:goto L8c09d41a;case 0x8c09d4e0u:goto L8c09d4e0;case 0x8c09d542u:goto L8c09d542;case 0x8c09d5a4u:goto L8c09d5a4;case 0x8c09d5eau:goto L8c09d5ea;case 0x8c09d6e0u:goto L8c09d6e0;case 0x8c09d73au:goto L8c09d73a;case 0x8c09d754u:goto L8c09d754;case 0x8c09d76eu:goto L8c09d76e;case 0x8c09d7b0u:goto L8c09d7b0;case 0x8c09d880u:goto L8c09d880;case 0x8c09d8b0u:goto L8c09d8b0;case 0x8c09d8f0u:goto L8c09d8f0;case 0x8c09d926u:goto L8c09d926;default:*stop=0x8c09cf08u;return -3;} /* braf r0 */
L8c09cf7e: *stop=0x8c09cf7eu;r[4]=rd(m,0x8c09d080u,4);goto L8c09cf80; /* mov.l 0x8c09d080,r4 */
L8c09cf80: target=0x8c09da44u;*pr=0x8c09cf84u;*stop=0x8c09cf82u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cf84; /* bsr 0x8c09da44 */
L8c09cf84: *stop=0x8c09cf84u;r[4]=rd(m,0x8c09d084u,4);goto L8c09cf86; /* mov.l 0x8c09d084,r4 */
L8c09cf86: target=0x8c09cce4u;*pr=0x8c09cf8au;*stop=0x8c09cf88u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cf8a; /* bsr 0x8c09cce4 */
L8c09cf8a: *stop=0x8c09cf8au;r[4]=rd(m,0x8c09d088u,4);goto L8c09cf8c; /* mov.l 0x8c09d088,r4 */
L8c09cf8c: target=0x8c09cce4u;*pr=0x8c09cf90u;*stop=0x8c09cf8eu;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cf90; /* bsr 0x8c09cce4 */
L8c09cf90: *stop=0x8c09cf92u;goto L8c09d2c6; /* bra 0x8c09d2c6 */
L8c09cf94: *stop=0x8c09cf94u;r[4]=rd(m,0x8c09d08cu,4);goto L8c09cf96; /* mov.l 0x8c09d08c,r4 */
L8c09cf96: target=0x8c09da44u;*pr=0x8c09cf9au;*stop=0x8c09cf98u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cf9a; /* bsr 0x8c09da44 */
L8c09cf9a: *stop=0x8c09cf9au;r[4]=rd(m,0x8c09d090u,4);goto L8c09cf9c; /* mov.l 0x8c09d090,r4 */
L8c09cf9c: target=0x8c09cce4u;*pr=0x8c09cfa0u;*stop=0x8c09cf9eu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfa0; /* bsr 0x8c09cce4 */
L8c09cfa0: *stop=0x8c09cfa0u;r[4]=rd(m,0x8c09d094u,4);goto L8c09cfa2; /* mov.l 0x8c09d094,r4 */
L8c09cfa2: target=0x8c09cce4u;*pr=0x8c09cfa6u;*stop=0x8c09cfa4u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfa6; /* bsr 0x8c09cce4 */
L8c09cfa6: *stop=0x8c09cfa6u;r[4]=rd(m,0x8c09d098u,4);goto L8c09cfa8; /* mov.l 0x8c09d098,r4 */
L8c09cfa8: target=0x8c09cce4u;*pr=0x8c09cfacu;*stop=0x8c09cfaau;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfac; /* bsr 0x8c09cce4 */
L8c09cfac: *stop=0x8c09cfacu;r[4]=rd(m,0x8c09d090u,4);goto L8c09cfae; /* mov.l 0x8c09d090,r4 */
L8c09cfae: target=0x8c09cd6eu;*pr=0x8c09cfb2u;*stop=0x8c09cfb0u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfb2; /* bsr 0x8c09cd6e */
L8c09cfb2: *stop=0x8c09cfb2u;r[4]=rd(m,0x8c09d094u,4);goto L8c09cfb4; /* mov.l 0x8c09d094,r4 */
L8c09cfb4: target=0x8c09cd6eu;*pr=0x8c09cfb8u;*stop=0x8c09cfb6u;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfb8; /* bsr 0x8c09cd6e */
L8c09cfb8: *stop=0x8c09cfb8u;r[4]=rd(m,0x8c09d098u,4);goto L8c09cfba; /* mov.l 0x8c09d098,r4 */
L8c09cfba: *stop=0x8c09cfbcu;r[5]=r[8];goto L8c09d914; /* bra 0x8c09d914 */
L8c09cfbe: *stop=0x8c09cfbeu;r[4]=rd(m,0x8c09d09cu,4);goto L8c09cfc0; /* mov.l 0x8c09d09c,r4 */
L8c09cfc0: target=0x8c09da44u;*pr=0x8c09cfc4u;*stop=0x8c09cfc2u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfc4; /* bsr 0x8c09da44 */
L8c09cfc4: *stop=0x8c09cfc4u;r[4]=rd(m,0x8c09d0a0u,4);goto L8c09cfc6; /* mov.l 0x8c09d0a0,r4 */
L8c09cfc6: target=0x8c09cce4u;*pr=0x8c09cfcau;*stop=0x8c09cfc8u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfca; /* bsr 0x8c09cce4 */
L8c09cfca: *stop=0x8c09cfcau;r[4]=rd(m,0x8c09d0a4u,4);goto L8c09cfcc; /* mov.l 0x8c09d0a4,r4 */
L8c09cfcc: target=0x8c09cce4u;*pr=0x8c09cfd0u;*stop=0x8c09cfceu;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfd0; /* bsr 0x8c09cce4 */
L8c09cfd0: *stop=0x8c09cfd0u;r[4]=rd(m,0x8c09d0a8u,4);goto L8c09cfd2; /* mov.l 0x8c09d0a8,r4 */
L8c09cfd2: target=0x8c09cce4u;*pr=0x8c09cfd6u;*stop=0x8c09cfd4u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfd6; /* bsr 0x8c09cce4 */
L8c09cfd6: *stop=0x8c09cfd6u;r[4]=rd(m,0x8c09d0a0u,4);goto L8c09cfd8; /* mov.l 0x8c09d0a0,r4 */
L8c09cfd8: target=0x8c09cd6eu;*pr=0x8c09cfdcu;*stop=0x8c09cfdau;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfdc; /* bsr 0x8c09cd6e */
L8c09cfdc: *stop=0x8c09cfdcu;r[4]=rd(m,0x8c09d0a4u,4);goto L8c09cfde; /* mov.l 0x8c09d0a4,r4 */
L8c09cfde: target=0x8c09cd6eu;*pr=0x8c09cfe2u;*stop=0x8c09cfe0u;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfe2; /* bsr 0x8c09cd6e */
L8c09cfe2: *stop=0x8c09cfe2u;r[4]=rd(m,0x8c09d0a8u,4);goto L8c09cfe4; /* mov.l 0x8c09d0a8,r4 */
L8c09cfe4: *stop=0x8c09cfe6u;r[5]=r[8];goto L8c09d914; /* bra 0x8c09d914 */
L8c09cfe8: *stop=0x8c09cfe8u;r[4]=rd(m,0x8c09d0acu,4);goto L8c09cfea; /* mov.l 0x8c09d0ac,r4 */
L8c09cfea: target=0x8c09da44u;*pr=0x8c09cfeeu;*stop=0x8c09cfecu;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cfee; /* bsr 0x8c09da44 */
L8c09cfee: *stop=0x8c09cfeeu;r[4]=rd(m,0x8c09d0b0u,4);goto L8c09cff0; /* mov.l 0x8c09d0b0,r4 */
L8c09cff0: target=0x8c09cce4u;*pr=0x8c09cff4u;*stop=0x8c09cff2u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cff4; /* bsr 0x8c09cce4 */
L8c09cff4: *stop=0x8c09cff4u;r[4]=rd(m,0x8c09d0b4u,4);goto L8c09cff6; /* mov.l 0x8c09d0b4,r4 */
L8c09cff6: target=0x8c09cce4u;*pr=0x8c09cffau;*stop=0x8c09cff8u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cffa; /* bsr 0x8c09cce4 */
L8c09cffa: *stop=0x8c09cffau;r[4]=rd(m,0x8c09d0b8u,4);goto L8c09cffc; /* mov.l 0x8c09d0b8,r4 */
L8c09cffc: target=0x8c09cce4u;*pr=0x8c09d000u;*stop=0x8c09cffeu;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d000; /* bsr 0x8c09cce4 */
L8c09d000: *stop=0x8c09d000u;r[4]=rd(m,0x8c09d0b0u,4);goto L8c09d002; /* mov.l 0x8c09d0b0,r4 */
L8c09d002: target=0x8c09cd6eu;*pr=0x8c09d006u;*stop=0x8c09d004u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d006; /* bsr 0x8c09cd6e */
L8c09d006: *stop=0x8c09d006u;r[4]=rd(m,0x8c09d0b4u,4);goto L8c09d008; /* mov.l 0x8c09d0b4,r4 */
L8c09d008: target=0x8c09cd6eu;*pr=0x8c09d00cu;*stop=0x8c09d00au;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d00c; /* bsr 0x8c09cd6e */
L8c09d00c: *stop=0x8c09d00cu;r[4]=rd(m,0x8c09d0b8u,4);goto L8c09d00e; /* mov.l 0x8c09d0b8,r4 */
L8c09d00e: *stop=0x8c09d010u;r[5]=r[8];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d012: *stop=0x8c09d012u;r[4]=rd(m,0x8c09d0bcu,4);goto L8c09d014; /* mov.l 0x8c09d0bc,r4 */
L8c09d014: target=0x8c09da44u;*pr=0x8c09d018u;*stop=0x8c09d016u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d018; /* bsr 0x8c09da44 */
L8c09d018: *stop=0x8c09d018u;r[4]=rd(m,0x8c09d0c0u,4);goto L8c09d01a; /* mov.l 0x8c09d0c0,r4 */
L8c09d01a: target=0x8c09cce4u;*pr=0x8c09d01eu;*stop=0x8c09d01cu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d01e; /* bsr 0x8c09cce4 */
L8c09d01e: *stop=0x8c09d01eu;r[4]=rd(m,0x8c09d0c4u,4);goto L8c09d020; /* mov.l 0x8c09d0c4,r4 */
L8c09d020: target=0x8c09cce4u;*pr=0x8c09d024u;*stop=0x8c09d022u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d024; /* bsr 0x8c09cce4 */
L8c09d024: *stop=0x8c09d024u;r[4]=rd(m,0x8c09d0c8u,4);goto L8c09d026; /* mov.l 0x8c09d0c8,r4 */
L8c09d026: target=0x8c09cce4u;*pr=0x8c09d02au;*stop=0x8c09d028u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d02a; /* bsr 0x8c09cce4 */
L8c09d02a: *stop=0x8c09d02au;r[4]=rd(m,0x8c09d0c0u,4);goto L8c09d02c; /* mov.l 0x8c09d0c0,r4 */
L8c09d02c: target=0x8c09cd6eu;*pr=0x8c09d030u;*stop=0x8c09d02eu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d030; /* bsr 0x8c09cd6e */
L8c09d030: *stop=0x8c09d030u;r[4]=rd(m,0x8c09d0c4u,4);goto L8c09d032; /* mov.l 0x8c09d0c4,r4 */
L8c09d032: target=0x8c09cd6eu;*pr=0x8c09d036u;*stop=0x8c09d034u;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d036; /* bsr 0x8c09cd6e */
L8c09d036: *stop=0x8c09d036u;r[4]=rd(m,0x8c09d0c8u,4);goto L8c09d038; /* mov.l 0x8c09d0c8,r4 */
L8c09d038: *stop=0x8c09d03au;r[5]=r[8];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d03c: *stop=0x8c09d03cu;r[4]=rd(m,0x8c09d0ccu,4);goto L8c09d03e; /* mov.l 0x8c09d0cc,r4 */
L8c09d03e: target=0x8c09da44u;*pr=0x8c09d042u;*stop=0x8c09d040u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d042; /* bsr 0x8c09da44 */
L8c09d042: *stop=0x8c09d042u;r[4]=rd(m,0x8c09d0d0u,4);goto L8c09d044; /* mov.l 0x8c09d0d0,r4 */
L8c09d044: target=0x8c09cce4u;*pr=0x8c09d048u;*stop=0x8c09d046u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d048; /* bsr 0x8c09cce4 */
L8c09d048: *stop=0x8c09d048u;r[4]=rd(m,0x8c09d0d4u,4);goto L8c09d04a; /* mov.l 0x8c09d0d4,r4 */
L8c09d04a: target=0x8c09cce4u;*pr=0x8c09d04eu;*stop=0x8c09d04cu;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d04e; /* bsr 0x8c09cce4 */
L8c09d04e: *stop=0x8c09d04eu;r[4]=rd(m,0x8c09d0d8u,4);goto L8c09d050; /* mov.l 0x8c09d0d8,r4 */
L8c09d050: target=0x8c09cce4u;*pr=0x8c09d054u;*stop=0x8c09d052u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d054; /* bsr 0x8c09cce4 */
L8c09d054: *stop=0x8c09d054u;r[4]=rd(m,0x8c09d0d0u,4);goto L8c09d056; /* mov.l 0x8c09d0d0,r4 */
L8c09d056: target=0x8c09cd6eu;*pr=0x8c09d05au;*stop=0x8c09d058u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d05a; /* bsr 0x8c09cd6e */
L8c09d05a: *stop=0x8c09d05au;r[4]=rd(m,0x8c09d0d4u,4);goto L8c09d05c; /* mov.l 0x8c09d0d4,r4 */
L8c09d05c: target=0x8c09cd6eu;*pr=0x8c09d060u;*stop=0x8c09d05eu;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d060; /* bsr 0x8c09cd6e */
L8c09d060: *stop=0x8c09d060u;r[4]=rd(m,0x8c09d0d8u,4);goto L8c09d062; /* mov.l 0x8c09d0d8,r4 */
L8c09d062: *stop=0x8c09d064u;r[5]=r[8];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d0e0: *stop=0x8c09d0e0u;r[4]=rd(m,0x8c09d200u,4);goto L8c09d0e2; /* mov.l 0x8c09d200,r4 */
L8c09d0e2: target=0x8c09da44u;*pr=0x8c09d0e6u;*stop=0x8c09d0e4u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d0e6; /* bsr 0x8c09da44 */
L8c09d0e6: *stop=0x8c09d0e6u;r[4]=rd(m,0x8c09d204u,4);goto L8c09d0e8; /* mov.l 0x8c09d204,r4 */
L8c09d0e8: target=0x8c09cce4u;*pr=0x8c09d0ecu;*stop=0x8c09d0eau;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d0ec; /* bsr 0x8c09cce4 */
L8c09d0ec: *stop=0x8c09d0ecu;r[4]=rd(m,0x8c09d208u,4);goto L8c09d0ee; /* mov.l 0x8c09d208,r4 */
L8c09d0ee: target=0x8c09cce4u;*pr=0x8c09d0f2u;*stop=0x8c09d0f0u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d0f2; /* bsr 0x8c09cce4 */
L8c09d0f2: *stop=0x8c09d0f2u;r[4]=rd(m,0x8c09d20cu,4);goto L8c09d0f4; /* mov.l 0x8c09d20c,r4 */
L8c09d0f4: target=0x8c09cce4u;*pr=0x8c09d0f8u;*stop=0x8c09d0f6u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d0f8; /* bsr 0x8c09cce4 */
L8c09d0f8: *stop=0x8c09d0f8u;r[4]=rd(m,0x8c09d204u,4);goto L8c09d0fa; /* mov.l 0x8c09d204,r4 */
L8c09d0fa: target=0x8c09cd6eu;*pr=0x8c09d0feu;*stop=0x8c09d0fcu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d0fe; /* bsr 0x8c09cd6e */
L8c09d0fe: *stop=0x8c09d0feu;r[4]=rd(m,0x8c09d208u,4);goto L8c09d100; /* mov.l 0x8c09d208,r4 */
L8c09d100: target=0x8c09cd6eu;*pr=0x8c09d104u;*stop=0x8c09d102u;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d104; /* bsr 0x8c09cd6e */
L8c09d104: *stop=0x8c09d104u;r[4]=rd(m,0x8c09d20cu,4);goto L8c09d106; /* mov.l 0x8c09d20c,r4 */
L8c09d106: *stop=0x8c09d108u;r[5]=r[8];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d10a: *stop=0x8c09d10au;r[4]=rd(m,0x8c09d210u,4);goto L8c09d10c; /* mov.l 0x8c09d210,r4 */
L8c09d10c: target=0x8c09da44u;*pr=0x8c09d110u;*stop=0x8c09d10eu;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d110; /* bsr 0x8c09da44 */
L8c09d110: *stop=0x8c09d110u;r[4]=rd(m,0x8c09d214u,4);goto L8c09d112; /* mov.l 0x8c09d214,r4 */
L8c09d112: target=0x8c09cce4u;*pr=0x8c09d116u;*stop=0x8c09d114u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d116; /* bsr 0x8c09cce4 */
L8c09d116: *stop=0x8c09d116u;r[4]=rd(m,0x8c09d218u,4);goto L8c09d118; /* mov.l 0x8c09d218,r4 */
L8c09d118: target=0x8c09cce4u;*pr=0x8c09d11cu;*stop=0x8c09d11au;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d11c; /* bsr 0x8c09cce4 */
L8c09d11c: *stop=0x8c09d11cu;r[4]=rd(m,0x8c09d21cu,4);goto L8c09d11e; /* mov.l 0x8c09d21c,r4 */
L8c09d11e: target=0x8c09cce4u;*pr=0x8c09d122u;*stop=0x8c09d120u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d122; /* bsr 0x8c09cce4 */
L8c09d122: *stop=0x8c09d122u;r[4]=rd(m,0x8c09d214u,4);goto L8c09d124; /* mov.l 0x8c09d214,r4 */
L8c09d124: target=0x8c09cd6eu;*pr=0x8c09d128u;*stop=0x8c09d126u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d128; /* bsr 0x8c09cd6e */
L8c09d128: *stop=0x8c09d128u;r[4]=rd(m,0x8c09d218u,4);goto L8c09d12a; /* mov.l 0x8c09d218,r4 */
L8c09d12a: target=0x8c09cd6eu;*pr=0x8c09d12eu;*stop=0x8c09d12cu;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d12e; /* bsr 0x8c09cd6e */
L8c09d12e: *stop=0x8c09d12eu;r[4]=rd(m,0x8c09d21cu,4);goto L8c09d130; /* mov.l 0x8c09d21c,r4 */
L8c09d130: *stop=0x8c09d132u;r[5]=r[8];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d134: *stop=0x8c09d134u;r[4]=rd(m,0x8c09d220u,4);goto L8c09d136; /* mov.l 0x8c09d220,r4 */
L8c09d136: target=0x8c09da44u;*pr=0x8c09d13au;*stop=0x8c09d138u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d13a; /* bsr 0x8c09da44 */
L8c09d13a: *stop=0x8c09d13au;r[4]=rd(m,0x8c09d224u,4);goto L8c09d13c; /* mov.l 0x8c09d224,r4 */
L8c09d13c: target=0x8c09cce4u;*pr=0x8c09d140u;*stop=0x8c09d13eu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d140; /* bsr 0x8c09cce4 */
L8c09d140: *stop=0x8c09d140u;r[4]=rd(m,0x8c09d228u,4);goto L8c09d142; /* mov.l 0x8c09d228,r4 */
L8c09d142: target=0x8c09cce4u;*pr=0x8c09d146u;*stop=0x8c09d144u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d146; /* bsr 0x8c09cce4 */
L8c09d146: *stop=0x8c09d146u;r[4]=rd(m,0x8c09d22cu,4);goto L8c09d148; /* mov.l 0x8c09d22c,r4 */
L8c09d148: target=0x8c09cce4u;*pr=0x8c09d14cu;*stop=0x8c09d14au;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d14c; /* bsr 0x8c09cce4 */
L8c09d14c: *stop=0x8c09d14cu;r[4]=rd(m,0x8c09d234u,4);goto L8c09d14e; /* mov.l 0x8c09d234,r4 */
L8c09d14e: *stop=0x8c09d14eu;r[5]=rd(m,0x8c09d230u,4);goto L8c09d150; /* mov.l 0x8c09d230,r5 */
L8c09d150: target=0x8c09cce4u;*pr=0x8c09d154u;*stop=0x8c09d152u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d154; /* bsr 0x8c09cce4 */
L8c09d154: *stop=0x8c09d154u;r[4]=rd(m,0x8c09d224u,4);goto L8c09d156; /* mov.l 0x8c09d224,r4 */
L8c09d156: target=0x8c09cd6eu;*pr=0x8c09d15au;*stop=0x8c09d158u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d15a; /* bsr 0x8c09cd6e */
L8c09d15a: *stop=0x8c09d15au;r[4]=rd(m,0x8c09d228u,4);goto L8c09d15c; /* mov.l 0x8c09d228,r4 */
L8c09d15c: target=0x8c09cd6eu;*pr=0x8c09d160u;*stop=0x8c09d15eu;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d160; /* bsr 0x8c09cd6e */
L8c09d160: *stop=0x8c09d160u;r[4]=rd(m,0x8c09d22cu,4);goto L8c09d162; /* mov.l 0x8c09d22c,r4 */
L8c09d162: target=0x8c09cd6eu;*pr=0x8c09d166u;*stop=0x8c09d164u;r[5]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d166; /* bsr 0x8c09cd6e */
L8c09d166: *stop=0x8c09d166u;r[4]=rd(m,0x8c09d234u,4);goto L8c09d168; /* mov.l 0x8c09d234,r4 */
L8c09d168: *stop=0x8c09d168u;r[5]=rd(m,0x8c09d238u,4);goto L8c09d16a; /* mov.l 0x8c09d238,r5 */
L8c09d16a: *stop=0x8c09d16cu;goto L8c09d914; /* bra 0x8c09d914 */
L8c09d16e: *stop=0x8c09d16eu;r[4]=rd(m,0x8c09d23cu,4);goto L8c09d170; /* mov.l 0x8c09d23c,r4 */
L8c09d170: target=0x8c09da44u;*pr=0x8c09d174u;*stop=0x8c09d172u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d174; /* bsr 0x8c09da44 */
L8c09d174: *stop=0x8c09d174u;r[4]=rd(m,0x8c09d240u,4);goto L8c09d176; /* mov.l 0x8c09d240,r4 */
L8c09d176: target=0x8c09cce4u;*pr=0x8c09d17au;*stop=0x8c09d178u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d17a; /* bsr 0x8c09cce4 */
L8c09d17a: *stop=0x8c09d17au;r[4]=rd(m,0x8c09d244u,4);goto L8c09d17c; /* mov.l 0x8c09d244,r4 */
L8c09d17c: target=0x8c09cce4u;*pr=0x8c09d180u;*stop=0x8c09d17eu;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d180; /* bsr 0x8c09cce4 */
L8c09d180: *stop=0x8c09d180u;r[4]=rd(m,0x8c09d248u,4);goto L8c09d182; /* mov.l 0x8c09d248,r4 */
L8c09d182: target=0x8c09cce4u;*pr=0x8c09d186u;*stop=0x8c09d184u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d186; /* bsr 0x8c09cce4 */
L8c09d186: *stop=0x8c09d186u;r[4]=rd(m,0x8c09d24cu,4);goto L8c09d188; /* mov.l 0x8c09d24c,r4 */
L8c09d188: *stop=0x8c09d188u;r[5]=rd(m,0x8c09d230u,4);goto L8c09d18a; /* mov.l 0x8c09d230,r5 */
L8c09d18a: target=0x8c09cce4u;*pr=0x8c09d18eu;*stop=0x8c09d18cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d18e; /* bsr 0x8c09cce4 */
L8c09d18e: *stop=0x8c09d18eu;r[4]=rd(m,0x8c09d240u,4);goto L8c09d190; /* mov.l 0x8c09d240,r4 */
L8c09d190: target=0x8c09cd6eu;*pr=0x8c09d194u;*stop=0x8c09d192u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d194; /* bsr 0x8c09cd6e */
L8c09d194: *stop=0x8c09d194u;r[4]=rd(m,0x8c09d244u,4);goto L8c09d196; /* mov.l 0x8c09d244,r4 */
L8c09d196: target=0x8c09cd6eu;*pr=0x8c09d19au;*stop=0x8c09d198u;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d19a; /* bsr 0x8c09cd6e */
L8c09d19a: *stop=0x8c09d19au;r[4]=rd(m,0x8c09d248u,4);goto L8c09d19c; /* mov.l 0x8c09d248,r4 */
L8c09d19c: target=0x8c09cd6eu;*pr=0x8c09d1a0u;*stop=0x8c09d19eu;r[5]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d1a0; /* bsr 0x8c09cd6e */
L8c09d1a0: *stop=0x8c09d1a0u;r[4]=rd(m,0x8c09d24cu,4);goto L8c09d1a2; /* mov.l 0x8c09d24c,r4 */
L8c09d1a2: *stop=0x8c09d1a2u;r[5]=rd(m,0x8c09d238u,4);goto L8c09d1a4; /* mov.l 0x8c09d238,r5 */
L8c09d1a4: *stop=0x8c09d1a6u;goto L8c09d914; /* bra 0x8c09d914 */
L8c09d1a8: *stop=0x8c09d1a8u;r[4]=rd(m,0x8c09d250u,4);goto L8c09d1aa; /* mov.l 0x8c09d250,r4 */
L8c09d1aa: target=0x8c09da44u;*pr=0x8c09d1aeu;*stop=0x8c09d1acu;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d1ae; /* bsr 0x8c09da44 */
L8c09d1ae: *stop=0x8c09d1aeu;r[4]=rd(m,0x8c09d254u,4);goto L8c09d1b0; /* mov.l 0x8c09d254,r4 */
L8c09d1b0: target=0x8c09cce4u;*pr=0x8c09d1b4u;*stop=0x8c09d1b2u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d1b4; /* bsr 0x8c09cce4 */
L8c09d1b4: *stop=0x8c09d1b4u;r[4]=rd(m,0x8c09d258u,4);goto L8c09d1b6; /* mov.l 0x8c09d258,r4 */
L8c09d1b6: target=0x8c09cce4u;*pr=0x8c09d1bau;*stop=0x8c09d1b8u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d1ba; /* bsr 0x8c09cce4 */
L8c09d1ba: *stop=0x8c09d1bau;r[4]=rd(m,0x8c09d25cu,4);goto L8c09d1bc; /* mov.l 0x8c09d25c,r4 */
L8c09d1bc: target=0x8c09cce4u;*pr=0x8c09d1c0u;*stop=0x8c09d1beu;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d1c0; /* bsr 0x8c09cce4 */
L8c09d1c0: *stop=0x8c09d1c0u;r[4]=rd(m,0x8c09d260u,4);goto L8c09d1c2; /* mov.l 0x8c09d260,r4 */
L8c09d1c2: *stop=0x8c09d1c2u;r[5]=rd(m,0x8c09d230u,4);goto L8c09d1c4; /* mov.l 0x8c09d230,r5 */
L8c09d1c4: target=0x8c09cce4u;*pr=0x8c09d1c8u;*stop=0x8c09d1c6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d1c8; /* bsr 0x8c09cce4 */
L8c09d1c8: *stop=0x8c09d1c8u;r[4]=rd(m,0x8c09d254u,4);goto L8c09d1ca; /* mov.l 0x8c09d254,r4 */
L8c09d1ca: target=0x8c09cd6eu;*pr=0x8c09d1ceu;*stop=0x8c09d1ccu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d1ce; /* bsr 0x8c09cd6e */
L8c09d1ce: *stop=0x8c09d1ceu;r[4]=rd(m,0x8c09d258u,4);goto L8c09d1d0; /* mov.l 0x8c09d258,r4 */
L8c09d1d0: target=0x8c09cd6eu;*pr=0x8c09d1d4u;*stop=0x8c09d1d2u;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d1d4; /* bsr 0x8c09cd6e */
L8c09d1d4: *stop=0x8c09d1d4u;r[4]=rd(m,0x8c09d25cu,4);goto L8c09d1d6; /* mov.l 0x8c09d25c,r4 */
L8c09d1d6: target=0x8c09cd6eu;*pr=0x8c09d1dau;*stop=0x8c09d1d8u;r[5]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d1da; /* bsr 0x8c09cd6e */
L8c09d1da: *stop=0x8c09d1dau;r[4]=rd(m,0x8c09d260u,4);goto L8c09d1dc; /* mov.l 0x8c09d260,r4 */
L8c09d1dc: *stop=0x8c09d1dcu;r[5]=rd(m,0x8c09d238u,4);goto L8c09d1de; /* mov.l 0x8c09d238,r5 */
L8c09d1de: *stop=0x8c09d1e0u;goto L8c09d914; /* bra 0x8c09d914 */
L8c09d280: *stop=0x8c09d280u;r[4]=rd(m,0x8c09d360u,4);goto L8c09d282; /* mov.l 0x8c09d360,r4 */
L8c09d282: target=0x8c09da44u;*pr=0x8c09d286u;*stop=0x8c09d284u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d286; /* bsr 0x8c09da44 */
L8c09d286: *stop=0x8c09d286u;r[4]=rd(m,0x8c09d364u,4);goto L8c09d288; /* mov.l 0x8c09d364,r4 */
L8c09d288: target=0x8c09cce4u;*pr=0x8c09d28cu;*stop=0x8c09d28au;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d28c; /* bsr 0x8c09cce4 */
L8c09d28c: *stop=0x8c09d28cu;r[4]=rd(m,0x8c09d368u,4);goto L8c09d28e; /* mov.l 0x8c09d368,r4 */
L8c09d28e: target=0x8c09cce4u;*pr=0x8c09d292u;*stop=0x8c09d290u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d292; /* bsr 0x8c09cce4 */
L8c09d292: *stop=0x8c09d292u;r[4]=rd(m,0x8c09d36cu,4);goto L8c09d294; /* mov.l 0x8c09d36c,r4 */
L8c09d294: target=0x8c09cce4u;*pr=0x8c09d298u;*stop=0x8c09d296u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d298; /* bsr 0x8c09cce4 */
L8c09d298: *stop=0x8c09d298u;r[4]=rd(m,0x8c09d374u,4);goto L8c09d29a; /* mov.l 0x8c09d374,r4 */
L8c09d29a: *stop=0x8c09d29au;r[5]=rd(m,0x8c09d370u,4);goto L8c09d29c; /* mov.l 0x8c09d370,r5 */
L8c09d29c: target=0x8c09cce4u;*pr=0x8c09d2a0u;*stop=0x8c09d29eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2a0; /* bsr 0x8c09cce4 */
L8c09d2a0: *stop=0x8c09d2a0u;r[4]=rd(m,0x8c09d364u,4);goto L8c09d2a2; /* mov.l 0x8c09d364,r4 */
L8c09d2a2: target=0x8c09cd6eu;*pr=0x8c09d2a6u;*stop=0x8c09d2a4u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2a6; /* bsr 0x8c09cd6e */
L8c09d2a6: *stop=0x8c09d2a6u;r[4]=rd(m,0x8c09d368u,4);goto L8c09d2a8; /* mov.l 0x8c09d368,r4 */
L8c09d2a8: target=0x8c09cd6eu;*pr=0x8c09d2acu;*stop=0x8c09d2aau;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2ac; /* bsr 0x8c09cd6e */
L8c09d2ac: *stop=0x8c09d2acu;r[4]=rd(m,0x8c09d36cu,4);goto L8c09d2ae; /* mov.l 0x8c09d36c,r4 */
L8c09d2ae: target=0x8c09cd6eu;*pr=0x8c09d2b2u;*stop=0x8c09d2b0u;r[5]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2b2; /* bsr 0x8c09cd6e */
L8c09d2b2: *stop=0x8c09d2b2u;r[4]=rd(m,0x8c09d374u,4);goto L8c09d2b4; /* mov.l 0x8c09d374,r4 */
L8c09d2b4: *stop=0x8c09d2b4u;r[5]=rd(m,0x8c09d378u,4);goto L8c09d2b6; /* mov.l 0x8c09d378,r5 */
L8c09d2b6: *stop=0x8c09d2b8u;goto L8c09d914; /* bra 0x8c09d914 */
L8c09d2ba: *stop=0x8c09d2bau;r[4]=rd(m,0x8c09d37cu,4);goto L8c09d2bc; /* mov.l 0x8c09d37c,r4 */
L8c09d2bc: target=0x8c09da44u;*pr=0x8c09d2c0u;*stop=0x8c09d2beu;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2c0; /* bsr 0x8c09da44 */
L8c09d2c0: *stop=0x8c09d2c0u;r[4]=rd(m,0x8c09d380u,4);goto L8c09d2c2; /* mov.l 0x8c09d380,r4 */
L8c09d2c2: target=0x8c09cce4u;*pr=0x8c09d2c6u;*stop=0x8c09d2c4u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2c6; /* bsr 0x8c09cce4 */
L8c09d2c6: *stop=0x8c09d2c6u;r[4]=rd(m,0x8c09d384u,4);goto L8c09d2c8; /* mov.l 0x8c09d384,r4 */
L8c09d2c8: *stop=0x8c09d2cau;r[5]=r[14];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d2cc: *stop=0x8c09d2ccu;r[4]=rd(m,0x8c09d388u,4);goto L8c09d2ce; /* mov.l 0x8c09d388,r4 */
L8c09d2ce: target=0x8c09da44u;*pr=0x8c09d2d2u;*stop=0x8c09d2d0u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2d2; /* bsr 0x8c09da44 */
L8c09d2d2: *stop=0x8c09d2d2u;r[4]=rd(m,0x8c09d38cu,4);goto L8c09d2d4; /* mov.l 0x8c09d38c,r4 */
L8c09d2d4: target=0x8c09cce4u;*pr=0x8c09d2d8u;*stop=0x8c09d2d6u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2d8; /* bsr 0x8c09cce4 */
L8c09d2d8: *stop=0x8c09d2d8u;r[4]=rd(m,0x8c09d384u,4);goto L8c09d2da; /* mov.l 0x8c09d384,r4 */
L8c09d2da: *stop=0x8c09d2dcu;r[5]=r[14];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d2de: *stop=0x8c09d2deu;r[4]=rd(m,0x8c09d390u,4);goto L8c09d2e0; /* mov.l 0x8c09d390,r4 */
L8c09d2e0: target=0x8c09da44u;*pr=0x8c09d2e4u;*stop=0x8c09d2e2u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2e4; /* bsr 0x8c09da44 */
L8c09d2e4: *stop=0x8c09d2e4u;r[4]=rd(m,0x8c09d394u,4);goto L8c09d2e6; /* mov.l 0x8c09d394,r4 */
L8c09d2e6: target=0x8c09cce4u;*pr=0x8c09d2eau;*stop=0x8c09d2e8u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2ea; /* bsr 0x8c09cce4 */
L8c09d2ea: *stop=0x8c09d2eau;r[4]=rd(m,0x8c09d384u,4);goto L8c09d2ec; /* mov.l 0x8c09d384,r4 */
L8c09d2ec: *stop=0x8c09d2eeu;r[5]=r[14];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d2f0: *stop=0x8c09d2f0u;r[4]=rd(m,0x8c09d398u,4);goto L8c09d2f2; /* mov.l 0x8c09d398,r4 */
L8c09d2f2: target=0x8c09da44u;*pr=0x8c09d2f6u;*stop=0x8c09d2f4u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2f6; /* bsr 0x8c09da44 */
L8c09d2f6: *stop=0x8c09d2f6u;r[4]=rd(m,0x8c09d39cu,4);goto L8c09d2f8; /* mov.l 0x8c09d39c,r4 */
L8c09d2f8: target=0x8c09cce4u;*pr=0x8c09d2fcu;*stop=0x8c09d2fau;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d2fc; /* bsr 0x8c09cce4 */
L8c09d2fc: *stop=0x8c09d2fcu;r[4]=rd(m,0x8c09d3a0u,4);goto L8c09d2fe; /* mov.l 0x8c09d3a0,r4 */
L8c09d2fe: target=0x8c09cce4u;*pr=0x8c09d302u;*stop=0x8c09d300u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d302; /* bsr 0x8c09cce4 */
L8c09d302: *stop=0x8c09d302u;r[4]=rd(m,0x8c09d384u,4);goto L8c09d304; /* mov.l 0x8c09d384,r4 */
L8c09d304: *stop=0x8c09d306u;r[5]=r[14];goto L8c09d8de; /* bra 0x8c09d8de */
L8c09d308: *stop=0x8c09d308u;r[4]=rd(m,0x8c09d3a4u,4);goto L8c09d30a; /* mov.l 0x8c09d3a4,r4 */
L8c09d30a: target=0x8c09da44u;*pr=0x8c09d30eu;*stop=0x8c09d30cu;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d30e; /* bsr 0x8c09da44 */
L8c09d30e: *stop=0x8c09d30eu;r[4]=rd(m,0x8c09d3a8u,4);goto L8c09d310; /* mov.l 0x8c09d3a8,r4 */
L8c09d310: target=0x8c09cce4u;*pr=0x8c09d314u;*stop=0x8c09d312u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d314; /* bsr 0x8c09cce4 */
L8c09d314: *stop=0x8c09d314u;r[4]=rd(m,0x8c09d3a8u,4);goto L8c09d316; /* mov.l 0x8c09d3a8,r4 */
L8c09d316: target=0x8c09cd6eu;*pr=0x8c09d31au;*stop=0x8c09d318u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d31a; /* bsr 0x8c09cd6e */
L8c09d31a: *stop=0x8c09d31au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09d31c; /* lds.l @r15+,PR */
L8c09d31c: *stop=0x8c09d31cu;r[4]=rd(m,0x8c09d3b0u,4);goto L8c09d31e; /* mov.l 0x8c09d3b0,r4 */
L8c09d31e: *stop=0x8c09d31eu;r[5]=rd(m,0x8c09d3acu,4);goto L8c09d320; /* mov.l 0x8c09d3ac,r5 */
L8c09d320: *stop=0x8c09d320u;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c09d322; /* mov.l @r15+,r8 */
L8c09d322: *stop=0x8c09d322u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c09d324; /* mov.l @r15+,r9 */
L8c09d324: *stop=0x8c09d324u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c09d326; /* mov.l @r15+,r10 */
L8c09d326: *stop=0x8c09d326u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c09d328; /* mov.l @r15+,r11 */
L8c09d328: *stop=0x8c09d328u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c09d32a; /* mov.l @r15+,r12 */
L8c09d32a: *stop=0x8c09d32au;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09d32c; /* mov.l @r15+,r13 */
L8c09d32c: *stop=0x8c09d32eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0x8c09da44u,m,f,sr,r,pr); /* bra 0x8c09da44 */
L8c09d330: *stop=0x8c09d330u;r[4]=rd(m,0x8c09d3b4u,4);goto L8c09d332; /* mov.l 0x8c09d3b4,r4 */
L8c09d332: target=0x8c09da44u;*pr=0x8c09d336u;*stop=0x8c09d334u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d336; /* bsr 0x8c09da44 */
L8c09d336: *stop=0x8c09d336u;r[4]=rd(m,0x8c09d3b8u,4);goto L8c09d338; /* mov.l 0x8c09d3b8,r4 */
L8c09d338: target=0x8c09cce4u;*pr=0x8c09d33cu;*stop=0x8c09d33au;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d33c; /* bsr 0x8c09cce4 */
L8c09d33c: *stop=0x8c09d33cu;r[4]=rd(m,0x8c09d384u,4);goto L8c09d33e; /* mov.l 0x8c09d384,r4 */
L8c09d33e: *stop=0x8c09d340u;r[5]=r[14];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d3c0: *stop=0x8c09d3c0u;r[4]=rd(m,0x8c09d480u,4);goto L8c09d3c2; /* mov.l 0x8c09d480,r4 */
L8c09d3c2: target=0x8c09da44u;*pr=0x8c09d3c6u;*stop=0x8c09d3c4u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d3c6; /* bsr 0x8c09da44 */
L8c09d3c6: *stop=0x8c09d3c6u;r[4]=rd(m,0x8c09d484u,4);goto L8c09d3c8; /* mov.l 0x8c09d484,r4 */
L8c09d3c8: target=0x8c09cce4u;*pr=0x8c09d3ccu;*stop=0x8c09d3cau;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d3cc; /* bsr 0x8c09cce4 */
L8c09d3cc: *stop=0x8c09d3ccu;r[4]=rd(m,0x8c09d488u,4);goto L8c09d3ce; /* mov.l 0x8c09d488,r4 */
L8c09d3ce: *stop=0x8c09d3d0u;r[5]=r[14];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d3d2: *stop=0x8c09d3d2u;r[4]=rd(m,0x8c09d48cu,4);goto L8c09d3d4; /* mov.l 0x8c09d48c,r4 */
L8c09d3d4: target=0x8c09da44u;*pr=0x8c09d3d8u;*stop=0x8c09d3d6u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d3d8; /* bsr 0x8c09da44 */
L8c09d3d8: *stop=0x8c09d3d8u;r[4]=rd(m,0x8c09d490u,4);goto L8c09d3da; /* mov.l 0x8c09d490,r4 */
L8c09d3da: target=0x8c09cce4u;*pr=0x8c09d3deu;*stop=0x8c09d3dcu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d3de; /* bsr 0x8c09cce4 */
L8c09d3de: *stop=0x8c09d3deu;r[4]=rd(m,0x8c09d488u,4);goto L8c09d3e0; /* mov.l 0x8c09d488,r4 */
L8c09d3e0: *stop=0x8c09d3e2u;r[5]=r[14];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d3e4: *stop=0x8c09d3e4u;r[4]=rd(m,0x8c09d494u,4);goto L8c09d3e6; /* mov.l 0x8c09d494,r4 */
L8c09d3e6: target=0x8c09da44u;*pr=0x8c09d3eau;*stop=0x8c09d3e8u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d3ea; /* bsr 0x8c09da44 */
L8c09d3ea: *stop=0x8c09d3eau;r[4]=rd(m,0x8c09d498u,4);goto L8c09d3ec; /* mov.l 0x8c09d498,r4 */
L8c09d3ec: target=0x8c09cce4u;*pr=0x8c09d3f0u;*stop=0x8c09d3eeu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d3f0; /* bsr 0x8c09cce4 */
L8c09d3f0: *stop=0x8c09d3f0u;r[4]=rd(m,0x8c09d488u,4);goto L8c09d3f2; /* mov.l 0x8c09d488,r4 */
L8c09d3f2: *stop=0x8c09d3f4u;r[5]=r[14];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d3f6: *stop=0x8c09d3f6u;r[4]=rd(m,0x8c09d49cu,4);goto L8c09d3f8; /* mov.l 0x8c09d49c,r4 */
L8c09d3f8: target=0x8c09da44u;*pr=0x8c09d3fcu;*stop=0x8c09d3fau;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d3fc; /* bsr 0x8c09da44 */
L8c09d3fc: *stop=0x8c09d3fcu;r[4]=rd(m,0x8c09d4a0u,4);goto L8c09d3fe; /* mov.l 0x8c09d4a0,r4 */
L8c09d3fe: target=0x8c09cce4u;*pr=0x8c09d402u;*stop=0x8c09d400u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d402; /* bsr 0x8c09cce4 */
L8c09d402: *stop=0x8c09d402u;r[4]=rd(m,0x8c09d488u,4);goto L8c09d404; /* mov.l 0x8c09d488,r4 */
L8c09d404: *stop=0x8c09d406u;r[5]=r[14];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d408: *stop=0x8c09d408u;r[4]=rd(m,0x8c09d4a4u,4);goto L8c09d40a; /* mov.l 0x8c09d4a4,r4 */
L8c09d40a: target=0x8c09da44u;*pr=0x8c09d40eu;*stop=0x8c09d40cu;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d40e; /* bsr 0x8c09da44 */
L8c09d40e: *stop=0x8c09d40eu;r[4]=rd(m,0x8c09d4a8u,4);goto L8c09d410; /* mov.l 0x8c09d4a8,r4 */
L8c09d410: target=0x8c09cce4u;*pr=0x8c09d414u;*stop=0x8c09d412u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d414; /* bsr 0x8c09cce4 */
L8c09d414: *stop=0x8c09d414u;r[4]=rd(m,0x8c09d488u,4);goto L8c09d416; /* mov.l 0x8c09d488,r4 */
L8c09d416: *stop=0x8c09d418u;r[5]=r[14];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d41a: *stop=0x8c09d41au;r[4]=rd(m,0x8c09d4acu,4);goto L8c09d41c; /* mov.l 0x8c09d4ac,r4 */
L8c09d41c: target=0x8c09da44u;*pr=0x8c09d420u;*stop=0x8c09d41eu;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d420; /* bsr 0x8c09da44 */
L8c09d420: *stop=0x8c09d420u;r[4]=rd(m,0x8c09d4b0u,4);goto L8c09d422; /* mov.l 0x8c09d4b0,r4 */
L8c09d422: target=0x8c09cce4u;*pr=0x8c09d426u;*stop=0x8c09d424u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d426; /* bsr 0x8c09cce4 */
L8c09d426: *stop=0x8c09d426u;r[4]=rd(m,0x8c09d4b4u,4);goto L8c09d428; /* mov.l 0x8c09d4b4,r4 */
L8c09d428: target=0x8c09cce4u;*pr=0x8c09d42cu;*stop=0x8c09d42au;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d42c; /* bsr 0x8c09cce4 */
L8c09d42c: *stop=0x8c09d42cu;r[4]=rd(m,0x8c09d4b8u,4);goto L8c09d42e; /* mov.l 0x8c09d4b8,r4 */
L8c09d42e: target=0x8c09cce4u;*pr=0x8c09d432u;*stop=0x8c09d430u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d432; /* bsr 0x8c09cce4 */
L8c09d432: *stop=0x8c09d432u;r[4]=rd(m,0x8c09d4c0u,4);goto L8c09d434; /* mov.l 0x8c09d4c0,r4 */
L8c09d434: *stop=0x8c09d434u;r[5]=rd(m,0x8c09d4bcu,4);goto L8c09d436; /* mov.l 0x8c09d4bc,r5 */
L8c09d436: target=0x8c09cce4u;*pr=0x8c09d43au;*stop=0x8c09d438u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d43a; /* bsr 0x8c09cce4 */
L8c09d43a: *stop=0x8c09d43au;r[4]=rd(m,0x8c09d4c8u,4);goto L8c09d43c; /* mov.l 0x8c09d4c8,r4 */
L8c09d43c: *stop=0x8c09d43cu;r[5]=rd(m,0x8c09d4c4u,4);goto L8c09d43e; /* mov.l 0x8c09d4c4,r5 */
L8c09d43e: target=0x8c09cce4u;*pr=0x8c09d442u;*stop=0x8c09d440u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d442; /* bsr 0x8c09cce4 */
L8c09d442: *stop=0x8c09d442u;r[4]=rd(m,0x8c09d4d0u,4);goto L8c09d444; /* mov.l 0x8c09d4d0,r4 */
L8c09d444: *stop=0x8c09d444u;r[5]=rd(m,0x8c09d4ccu,4);goto L8c09d446; /* mov.l 0x8c09d4cc,r5 */
L8c09d446: target=0x8c09cce4u;*pr=0x8c09d44au;*stop=0x8c09d448u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d44a; /* bsr 0x8c09cce4 */
L8c09d44a: *stop=0x8c09d44au;r[4]=rd(m,0x8c09d4b0u,4);goto L8c09d44c; /* mov.l 0x8c09d4b0,r4 */
L8c09d44c: target=0x8c09cd6eu;*pr=0x8c09d450u;*stop=0x8c09d44eu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d450; /* bsr 0x8c09cd6e */
L8c09d450: *stop=0x8c09d450u;r[4]=rd(m,0x8c09d4b4u,4);goto L8c09d452; /* mov.l 0x8c09d4b4,r4 */
L8c09d452: target=0x8c09cd6eu;*pr=0x8c09d456u;*stop=0x8c09d454u;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d456; /* bsr 0x8c09cd6e */
L8c09d456: *stop=0x8c09d456u;r[4]=rd(m,0x8c09d4b8u,4);goto L8c09d458; /* mov.l 0x8c09d4b8,r4 */
L8c09d458: target=0x8c09cd6eu;*pr=0x8c09d45cu;*stop=0x8c09d45au;r[5]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d45c; /* bsr 0x8c09cd6e */
L8c09d45c: *stop=0x8c09d45cu;r[4]=rd(m,0x8c09d4c0u,4);goto L8c09d45e; /* mov.l 0x8c09d4c0,r4 */
L8c09d45e: *stop=0x8c09d45eu;r[5]=rd(m,0x8c09d4d4u,4);goto L8c09d460; /* mov.l 0x8c09d4d4,r5 */
L8c09d460: target=0x8c09cd6eu;*pr=0x8c09d464u;*stop=0x8c09d462u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d464; /* bsr 0x8c09cd6e */
L8c09d464: *stop=0x8c09d464u;r[4]=rd(m,0x8c09d4c8u,4);goto L8c09d466; /* mov.l 0x8c09d4c8,r4 */
L8c09d466: *stop=0x8c09d466u;r[5]=rd(m,0x8c09d4d8u,4);goto L8c09d468; /* mov.l 0x8c09d4d8,r5 */
L8c09d468: target=0x8c09cd6eu;*pr=0x8c09d46cu;*stop=0x8c09d46au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d46c; /* bsr 0x8c09cd6e */
L8c09d46c: *stop=0x8c09d46cu;r[4]=rd(m,0x8c09d4d0u,4);goto L8c09d46e; /* mov.l 0x8c09d4d0,r4 */
L8c09d46e: *stop=0x8c09d46eu;r[5]=rd(m,0x8c09d4dcu,4);goto L8c09d470; /* mov.l 0x8c09d4dc,r5 */
L8c09d470: *stop=0x8c09d472u;goto L8c09d914; /* bra 0x8c09d914 */
L8c09d4e0: *stop=0x8c09d4e0u;r[4]=rd(m,0x8c09d640u,4);goto L8c09d4e2; /* mov.l 0x8c09d640,r4 */
L8c09d4e2: target=0x8c09da44u;*pr=0x8c09d4e6u;*stop=0x8c09d4e4u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d4e6; /* bsr 0x8c09da44 */
L8c09d4e6: *stop=0x8c09d4e6u;r[4]=rd(m,0x8c09d648u,4);goto L8c09d4e8; /* mov.l 0x8c09d648,r4 */
L8c09d4e8: *stop=0x8c09d4e8u;r[5]=rd(m,0x8c09d644u,4);goto L8c09d4ea; /* mov.l 0x8c09d644,r5 */
L8c09d4ea: target=0x8c09da44u;*pr=0x8c09d4eeu;*stop=0x8c09d4ecu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d4ee; /* bsr 0x8c09da44 */
L8c09d4ee: *stop=0x8c09d4eeu;r[4]=rd(m,0x8c09d64cu,4);goto L8c09d4f0; /* mov.l 0x8c09d64c,r4 */
L8c09d4f0: target=0x8c09cce4u;*pr=0x8c09d4f4u;*stop=0x8c09d4f2u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d4f4; /* bsr 0x8c09cce4 */
L8c09d4f4: *stop=0x8c09d4f4u;r[4]=rd(m,0x8c09d650u,4);goto L8c09d4f6; /* mov.l 0x8c09d650,r4 */
L8c09d4f6: target=0x8c09cce4u;*pr=0x8c09d4fau;*stop=0x8c09d4f8u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d4fa; /* bsr 0x8c09cce4 */
L8c09d4fa: *stop=0x8c09d4fau;r[4]=rd(m,0x8c09d654u,4);goto L8c09d4fc; /* mov.l 0x8c09d654,r4 */
L8c09d4fc: target=0x8c09cce4u;*pr=0x8c09d500u;*stop=0x8c09d4feu;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d500; /* bsr 0x8c09cce4 */
L8c09d500: *stop=0x8c09d500u;r[4]=rd(m,0x8c09d65cu,4);goto L8c09d502; /* mov.l 0x8c09d65c,r4 */
L8c09d502: *stop=0x8c09d502u;r[5]=rd(m,0x8c09d658u,4);goto L8c09d504; /* mov.l 0x8c09d658,r5 */
L8c09d504: target=0x8c09cce4u;*pr=0x8c09d508u;*stop=0x8c09d506u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d508; /* bsr 0x8c09cce4 */
L8c09d508: *stop=0x8c09d508u;r[4]=rd(m,0x8c09d664u,4);goto L8c09d50a; /* mov.l 0x8c09d664,r4 */
L8c09d50a: *stop=0x8c09d50au;r[5]=rd(m,0x8c09d660u,4);goto L8c09d50c; /* mov.l 0x8c09d660,r5 */
L8c09d50c: target=0x8c09cce4u;*pr=0x8c09d510u;*stop=0x8c09d50eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d510; /* bsr 0x8c09cce4 */
L8c09d510: *stop=0x8c09d510u;r[4]=rd(m,0x8c09d66cu,4);goto L8c09d512; /* mov.l 0x8c09d66c,r4 */
L8c09d512: *stop=0x8c09d512u;r[5]=rd(m,0x8c09d668u,4);goto L8c09d514; /* mov.l 0x8c09d668,r5 */
L8c09d514: target=0x8c09cce4u;*pr=0x8c09d518u;*stop=0x8c09d516u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d518; /* bsr 0x8c09cce4 */
L8c09d518: *stop=0x8c09d518u;r[4]=rd(m,0x8c09d64cu,4);goto L8c09d51a; /* mov.l 0x8c09d64c,r4 */
L8c09d51a: target=0x8c09cd6eu;*pr=0x8c09d51eu;*stop=0x8c09d51cu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d51e; /* bsr 0x8c09cd6e */
L8c09d51e: *stop=0x8c09d51eu;r[4]=rd(m,0x8c09d650u,4);goto L8c09d520; /* mov.l 0x8c09d650,r4 */
L8c09d520: target=0x8c09cd6eu;*pr=0x8c09d524u;*stop=0x8c09d522u;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d524; /* bsr 0x8c09cd6e */
L8c09d524: *stop=0x8c09d524u;r[4]=rd(m,0x8c09d654u,4);goto L8c09d526; /* mov.l 0x8c09d654,r4 */
L8c09d526: target=0x8c09cd6eu;*pr=0x8c09d52au;*stop=0x8c09d528u;r[5]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d52a; /* bsr 0x8c09cd6e */
L8c09d52a: *stop=0x8c09d52au;r[4]=rd(m,0x8c09d65cu,4);goto L8c09d52c; /* mov.l 0x8c09d65c,r4 */
L8c09d52c: *stop=0x8c09d52cu;r[5]=rd(m,0x8c09d670u,4);goto L8c09d52e; /* mov.l 0x8c09d670,r5 */
L8c09d52e: target=0x8c09cd6eu;*pr=0x8c09d532u;*stop=0x8c09d530u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d532; /* bsr 0x8c09cd6e */
L8c09d532: *stop=0x8c09d532u;r[4]=rd(m,0x8c09d664u,4);goto L8c09d534; /* mov.l 0x8c09d664,r4 */
L8c09d534: *stop=0x8c09d534u;r[5]=rd(m,0x8c09d674u,4);goto L8c09d536; /* mov.l 0x8c09d674,r5 */
L8c09d536: target=0x8c09cd6eu;*pr=0x8c09d53au;*stop=0x8c09d538u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d53a; /* bsr 0x8c09cd6e */
L8c09d53a: *stop=0x8c09d53au;r[4]=rd(m,0x8c09d66cu,4);goto L8c09d53c; /* mov.l 0x8c09d66c,r4 */
L8c09d53c: *stop=0x8c09d53cu;r[5]=rd(m,0x8c09d678u,4);goto L8c09d53e; /* mov.l 0x8c09d678,r5 */
L8c09d53e: *stop=0x8c09d540u;goto L8c09d8de; /* bra 0x8c09d8de */
L8c09d542: *stop=0x8c09d542u;r[4]=rd(m,0x8c09d640u,4);goto L8c09d544; /* mov.l 0x8c09d640,r4 */
L8c09d544: target=0x8c09da44u;*pr=0x8c09d548u;*stop=0x8c09d546u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d548; /* bsr 0x8c09da44 */
L8c09d548: *stop=0x8c09d548u;r[4]=rd(m,0x8c09d67cu,4);goto L8c09d54a; /* mov.l 0x8c09d67c,r4 */
L8c09d54a: *stop=0x8c09d54au;r[5]=rd(m,0x8c09d644u,4);goto L8c09d54c; /* mov.l 0x8c09d644,r5 */
L8c09d54c: target=0x8c09da44u;*pr=0x8c09d550u;*stop=0x8c09d54eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d550; /* bsr 0x8c09da44 */
L8c09d550: *stop=0x8c09d550u;r[4]=rd(m,0x8c09d680u,4);goto L8c09d552; /* mov.l 0x8c09d680,r4 */
L8c09d552: target=0x8c09cce4u;*pr=0x8c09d556u;*stop=0x8c09d554u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d556; /* bsr 0x8c09cce4 */
L8c09d556: *stop=0x8c09d556u;r[4]=rd(m,0x8c09d684u,4);goto L8c09d558; /* mov.l 0x8c09d684,r4 */
L8c09d558: target=0x8c09cce4u;*pr=0x8c09d55cu;*stop=0x8c09d55au;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d55c; /* bsr 0x8c09cce4 */
L8c09d55c: *stop=0x8c09d55cu;r[4]=rd(m,0x8c09d688u,4);goto L8c09d55e; /* mov.l 0x8c09d688,r4 */
L8c09d55e: target=0x8c09cce4u;*pr=0x8c09d562u;*stop=0x8c09d560u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d562; /* bsr 0x8c09cce4 */
L8c09d562: *stop=0x8c09d562u;r[4]=rd(m,0x8c09d68cu,4);goto L8c09d564; /* mov.l 0x8c09d68c,r4 */
L8c09d564: *stop=0x8c09d564u;r[5]=rd(m,0x8c09d658u,4);goto L8c09d566; /* mov.l 0x8c09d658,r5 */
L8c09d566: target=0x8c09cce4u;*pr=0x8c09d56au;*stop=0x8c09d568u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d56a; /* bsr 0x8c09cce4 */
L8c09d56a: *stop=0x8c09d56au;r[4]=rd(m,0x8c09d690u,4);goto L8c09d56c; /* mov.l 0x8c09d690,r4 */
L8c09d56c: *stop=0x8c09d56cu;r[5]=rd(m,0x8c09d660u,4);goto L8c09d56e; /* mov.l 0x8c09d660,r5 */
L8c09d56e: target=0x8c09cce4u;*pr=0x8c09d572u;*stop=0x8c09d570u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d572; /* bsr 0x8c09cce4 */
L8c09d572: *stop=0x8c09d572u;r[4]=rd(m,0x8c09d694u,4);goto L8c09d574; /* mov.l 0x8c09d694,r4 */
L8c09d574: *stop=0x8c09d574u;r[5]=rd(m,0x8c09d668u,4);goto L8c09d576; /* mov.l 0x8c09d668,r5 */
L8c09d576: target=0x8c09cce4u;*pr=0x8c09d57au;*stop=0x8c09d578u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d57a; /* bsr 0x8c09cce4 */
L8c09d57a: *stop=0x8c09d57au;r[4]=rd(m,0x8c09d680u,4);goto L8c09d57c; /* mov.l 0x8c09d680,r4 */
L8c09d57c: target=0x8c09cd6eu;*pr=0x8c09d580u;*stop=0x8c09d57eu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d580; /* bsr 0x8c09cd6e */
L8c09d580: *stop=0x8c09d580u;r[4]=rd(m,0x8c09d684u,4);goto L8c09d582; /* mov.l 0x8c09d684,r4 */
L8c09d582: target=0x8c09cd6eu;*pr=0x8c09d586u;*stop=0x8c09d584u;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d586; /* bsr 0x8c09cd6e */
L8c09d586: *stop=0x8c09d586u;r[4]=rd(m,0x8c09d688u,4);goto L8c09d588; /* mov.l 0x8c09d688,r4 */
L8c09d588: target=0x8c09cd6eu;*pr=0x8c09d58cu;*stop=0x8c09d58au;r[5]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d58c; /* bsr 0x8c09cd6e */
L8c09d58c: *stop=0x8c09d58cu;r[4]=rd(m,0x8c09d68cu,4);goto L8c09d58e; /* mov.l 0x8c09d68c,r4 */
L8c09d58e: *stop=0x8c09d58eu;r[5]=rd(m,0x8c09d670u,4);goto L8c09d590; /* mov.l 0x8c09d670,r5 */
L8c09d590: target=0x8c09cd6eu;*pr=0x8c09d594u;*stop=0x8c09d592u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d594; /* bsr 0x8c09cd6e */
L8c09d594: *stop=0x8c09d594u;r[4]=rd(m,0x8c09d690u,4);goto L8c09d596; /* mov.l 0x8c09d690,r4 */
L8c09d596: *stop=0x8c09d596u;r[5]=rd(m,0x8c09d674u,4);goto L8c09d598; /* mov.l 0x8c09d674,r5 */
L8c09d598: target=0x8c09cd6eu;*pr=0x8c09d59cu;*stop=0x8c09d59au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d59c; /* bsr 0x8c09cd6e */
L8c09d59c: *stop=0x8c09d59cu;r[4]=rd(m,0x8c09d694u,4);goto L8c09d59e; /* mov.l 0x8c09d694,r4 */
L8c09d59e: *stop=0x8c09d59eu;r[5]=rd(m,0x8c09d678u,4);goto L8c09d5a0; /* mov.l 0x8c09d678,r5 */
L8c09d5a0: *stop=0x8c09d5a2u;goto L8c09d914; /* bra 0x8c09d914 */
L8c09d5a4: *stop=0x8c09d5a4u;r[4]=rd(m,0x8c09d698u,4);goto L8c09d5a6; /* mov.l 0x8c09d698,r4 */
L8c09d5a6: target=0x8c09da44u;*pr=0x8c09d5aau;*stop=0x8c09d5a8u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5aa; /* bsr 0x8c09da44 */
L8c09d5aa: *stop=0x8c09d5aau;r[4]=rd(m,0x8c09d69cu,4);goto L8c09d5ac; /* mov.l 0x8c09d69c,r4 */
L8c09d5ac: target=0x8c09cce4u;*pr=0x8c09d5b0u;*stop=0x8c09d5aeu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5b0; /* bsr 0x8c09cce4 */
L8c09d5b0: *stop=0x8c09d5b0u;r[4]=rd(m,0x8c09d6a0u,4);goto L8c09d5b2; /* mov.l 0x8c09d6a0,r4 */
L8c09d5b2: target=0x8c09cce4u;*pr=0x8c09d5b6u;*stop=0x8c09d5b4u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5b6; /* bsr 0x8c09cce4 */
L8c09d5b6: *stop=0x8c09d5b6u;r[4]=rd(m,0x8c09d6a4u,4);goto L8c09d5b8; /* mov.l 0x8c09d6a4,r4 */
L8c09d5b8: target=0x8c09cce4u;*pr=0x8c09d5bcu;*stop=0x8c09d5bau;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5bc; /* bsr 0x8c09cce4 */
L8c09d5bc: *stop=0x8c09d5bcu;r[4]=rd(m,0x8c09d6a8u,4);goto L8c09d5be; /* mov.l 0x8c09d6a8,r4 */
L8c09d5be: *stop=0x8c09d5beu;r[5]=rd(m,0x8c09d658u,4);goto L8c09d5c0; /* mov.l 0x8c09d658,r5 */
L8c09d5c0: target=0x8c09cce4u;*pr=0x8c09d5c4u;*stop=0x8c09d5c2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5c4; /* bsr 0x8c09cce4 */
L8c09d5c4: *stop=0x8c09d5c4u;r[4]=rd(m,0x8c09d69cu,4);goto L8c09d5c6; /* mov.l 0x8c09d69c,r4 */
L8c09d5c6: target=0x8c09cd6eu;*pr=0x8c09d5cau;*stop=0x8c09d5c8u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5ca; /* bsr 0x8c09cd6e */
L8c09d5ca: *stop=0x8c09d5cau;r[4]=rd(m,0x8c09d6a0u,4);goto L8c09d5cc; /* mov.l 0x8c09d6a0,r4 */
L8c09d5cc: target=0x8c09cd6eu;*pr=0x8c09d5d0u;*stop=0x8c09d5ceu;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5d0; /* bsr 0x8c09cd6e */
L8c09d5d0: *stop=0x8c09d5d0u;r[4]=rd(m,0x8c09d6a4u,4);goto L8c09d5d2; /* mov.l 0x8c09d6a4,r4 */
L8c09d5d2: target=0x8c09cd6eu;*pr=0x8c09d5d6u;*stop=0x8c09d5d4u;r[5]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5d6; /* bsr 0x8c09cd6e */
L8c09d5d6: *stop=0x8c09d5d6u;r[4]=rd(m,0x8c09d6a8u,4);goto L8c09d5d8; /* mov.l 0x8c09d6a8,r4 */
L8c09d5d8: *stop=0x8c09d5d8u;r[5]=rd(m,0x8c09d670u,4);goto L8c09d5da; /* mov.l 0x8c09d670,r5 */
L8c09d5da: target=0x8c09cd6eu;*pr=0x8c09d5deu;*stop=0x8c09d5dcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5de; /* bsr 0x8c09cd6e */
L8c09d5de: *stop=0x8c09d5deu;r[4]=rd(m,0x8c09d6acu,4);goto L8c09d5e0; /* mov.l 0x8c09d6ac,r4 */
L8c09d5e0: *stop=0x8c09d5e0u;r[5]=rd(m,0x8c09d678u,4);goto L8c09d5e2; /* mov.l 0x8c09d678,r5 */
L8c09d5e2: target=0x8c09cc80u;*pr=0x8c09d5e6u;*stop=0x8c09d5e4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5e6; /* bsr 0x8c09cc80 */
L8c09d5e6: *stop=0x8c09d5e8u;goto L8c09d92a; /* bra 0x8c09d92a */
L8c09d5ea: *stop=0x8c09d5eau;r[4]=rd(m,0x8c09d6b0u,4);goto L8c09d5ec; /* mov.l 0x8c09d6b0,r4 */
L8c09d5ec: target=0x8c09da44u;*pr=0x8c09d5f0u;*stop=0x8c09d5eeu;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5f0; /* bsr 0x8c09da44 */
L8c09d5f0: *stop=0x8c09d5f0u;r[4]=rd(m,0x8c09d6b4u,4);goto L8c09d5f2; /* mov.l 0x8c09d6b4,r4 */
L8c09d5f2: target=0x8c09cce4u;*pr=0x8c09d5f6u;*stop=0x8c09d5f4u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5f6; /* bsr 0x8c09cce4 */
L8c09d5f6: *stop=0x8c09d5f6u;r[4]=rd(m,0x8c09d6b8u,4);goto L8c09d5f8; /* mov.l 0x8c09d6b8,r4 */
L8c09d5f8: target=0x8c09cce4u;*pr=0x8c09d5fcu;*stop=0x8c09d5fau;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d5fc; /* bsr 0x8c09cce4 */
L8c09d5fc: *stop=0x8c09d5fcu;r[4]=rd(m,0x8c09d6bcu,4);goto L8c09d5fe; /* mov.l 0x8c09d6bc,r4 */
L8c09d5fe: target=0x8c09cce4u;*pr=0x8c09d602u;*stop=0x8c09d600u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d602; /* bsr 0x8c09cce4 */
L8c09d602: *stop=0x8c09d602u;r[4]=rd(m,0x8c09d6b4u,4);goto L8c09d604; /* mov.l 0x8c09d6b4,r4 */
L8c09d604: target=0x8c09cd6eu;*pr=0x8c09d608u;*stop=0x8c09d606u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d608; /* bsr 0x8c09cd6e */
L8c09d608: *stop=0x8c09d608u;r[4]=rd(m,0x8c09d6b8u,4);goto L8c09d60a; /* mov.l 0x8c09d6b8,r4 */
L8c09d60a: target=0x8c09cd6eu;*pr=0x8c09d60eu;*stop=0x8c09d60cu;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d60e; /* bsr 0x8c09cd6e */
L8c09d60e: *stop=0x8c09d60eu;r[4]=rd(m,0x8c09d6bcu,4);goto L8c09d610; /* mov.l 0x8c09d6bc,r4 */
L8c09d610: target=0x8c09cd6eu;*pr=0x8c09d614u;*stop=0x8c09d612u;r[5]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d614; /* bsr 0x8c09cd6e */
L8c09d614: *stop=0x8c09d614u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09d616; /* lds.l @r15+,PR */
L8c09d616: *stop=0x8c09d616u;r[4]=rd(m,0x8c09d6c4u,4);goto L8c09d618; /* mov.l 0x8c09d6c4,r4 */
L8c09d618: *stop=0x8c09d618u;r[5]=rd(m,0x8c09d6c0u,4);goto L8c09d61a; /* mov.l 0x8c09d6c0,r5 */
L8c09d61a: *stop=0x8c09d61au;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c09d61c; /* mov.l @r15+,r8 */
L8c09d61c: *stop=0x8c09d61cu;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c09d61e; /* mov.l @r15+,r9 */
L8c09d61e: *stop=0x8c09d61eu;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c09d620; /* mov.l @r15+,r10 */
L8c09d620: *stop=0x8c09d620u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c09d622; /* mov.l @r15+,r11 */
L8c09d622: *stop=0x8c09d622u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c09d624; /* mov.l @r15+,r12 */
L8c09d624: *stop=0x8c09d624u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09d626; /* mov.l @r15+,r13 */
L8c09d626: *stop=0x8c09d628u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0x8c09da44u,m,f,sr,r,pr); /* bra 0x8c09da44 */
L8c09d6e0: *stop=0x8c09d6e0u;r[4]=rd(m,0x8c09d7e0u,4);goto L8c09d6e2; /* mov.l 0x8c09d7e0,r4 */
L8c09d6e2: target=0x8c09da44u;*pr=0x8c09d6e6u;*stop=0x8c09d6e4u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d6e6; /* bsr 0x8c09da44 */
L8c09d6e6: *stop=0x8c09d6e6u;r[4]=rd(m,0x8c09d7e4u,4);goto L8c09d6e8; /* mov.l 0x8c09d7e4,r4 */
L8c09d6e8: target=0x8c09cce4u;*pr=0x8c09d6ecu;*stop=0x8c09d6eau;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d6ec; /* bsr 0x8c09cce4 */
L8c09d6ec: *stop=0x8c09d6ecu;r[4]=rd(m,0x8c09d7e8u,4);goto L8c09d6ee; /* mov.l 0x8c09d7e8,r4 */
L8c09d6ee: target=0x8c09cce4u;*pr=0x8c09d6f2u;*stop=0x8c09d6f0u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d6f2; /* bsr 0x8c09cce4 */
L8c09d6f2: *stop=0x8c09d6f2u;r[4]=rd(m,0x8c09d7ecu,4);goto L8c09d6f4; /* mov.l 0x8c09d7ec,r4 */
L8c09d6f4: target=0x8c09cce4u;*pr=0x8c09d6f8u;*stop=0x8c09d6f6u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d6f8; /* bsr 0x8c09cce4 */
L8c09d6f8: *stop=0x8c09d6f8u;r[4]=rd(m,0x8c09d7f4u,4);goto L8c09d6fa; /* mov.l 0x8c09d7f4,r4 */
L8c09d6fa: *stop=0x8c09d6fau;r[5]=rd(m,0x8c09d7f0u,4);goto L8c09d6fc; /* mov.l 0x8c09d7f0,r5 */
L8c09d6fc: target=0x8c09cce4u;*pr=0x8c09d700u;*stop=0x8c09d6feu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d700; /* bsr 0x8c09cce4 */
L8c09d700: *stop=0x8c09d700u;r[4]=rd(m,0x8c09d7fcu,4);goto L8c09d702; /* mov.l 0x8c09d7fc,r4 */
L8c09d702: *stop=0x8c09d702u;r[5]=rd(m,0x8c09d7f8u,4);goto L8c09d704; /* mov.l 0x8c09d7f8,r5 */
L8c09d704: target=0x8c09cce4u;*pr=0x8c09d708u;*stop=0x8c09d706u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d708; /* bsr 0x8c09cce4 */
L8c09d708: *stop=0x8c09d708u;r[4]=rd(m,0x8c09d804u,4);goto L8c09d70a; /* mov.l 0x8c09d804,r4 */
L8c09d70a: *stop=0x8c09d70au;r[5]=rd(m,0x8c09d800u,4);goto L8c09d70c; /* mov.l 0x8c09d800,r5 */
L8c09d70c: target=0x8c09cce4u;*pr=0x8c09d710u;*stop=0x8c09d70eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d710; /* bsr 0x8c09cce4 */
L8c09d710: *stop=0x8c09d710u;r[4]=rd(m,0x8c09d7e4u,4);goto L8c09d712; /* mov.l 0x8c09d7e4,r4 */
L8c09d712: target=0x8c09cd6eu;*pr=0x8c09d716u;*stop=0x8c09d714u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d716; /* bsr 0x8c09cd6e */
L8c09d716: *stop=0x8c09d716u;r[4]=rd(m,0x8c09d7e8u,4);goto L8c09d718; /* mov.l 0x8c09d7e8,r4 */
L8c09d718: target=0x8c09cd6eu;*pr=0x8c09d71cu;*stop=0x8c09d71au;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d71c; /* bsr 0x8c09cd6e */
L8c09d71c: *stop=0x8c09d71cu;r[4]=rd(m,0x8c09d7ecu,4);goto L8c09d71e; /* mov.l 0x8c09d7ec,r4 */
L8c09d71e: target=0x8c09cd6eu;*pr=0x8c09d722u;*stop=0x8c09d720u;r[5]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d722; /* bsr 0x8c09cd6e */
L8c09d722: *stop=0x8c09d722u;r[4]=rd(m,0x8c09d7f4u,4);goto L8c09d724; /* mov.l 0x8c09d7f4,r4 */
L8c09d724: *stop=0x8c09d724u;r[5]=rd(m,0x8c09d808u,4);goto L8c09d726; /* mov.l 0x8c09d808,r5 */
L8c09d726: target=0x8c09cd6eu;*pr=0x8c09d72au;*stop=0x8c09d728u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d72a; /* bsr 0x8c09cd6e */
L8c09d72a: *stop=0x8c09d72au;r[4]=rd(m,0x8c09d7fcu,4);goto L8c09d72c; /* mov.l 0x8c09d7fc,r4 */
L8c09d72c: *stop=0x8c09d72cu;r[5]=rd(m,0x8c09d80cu,4);goto L8c09d72e; /* mov.l 0x8c09d80c,r5 */
L8c09d72e: target=0x8c09cd6eu;*pr=0x8c09d732u;*stop=0x8c09d730u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d732; /* bsr 0x8c09cd6e */
L8c09d732: *stop=0x8c09d732u;r[4]=rd(m,0x8c09d804u,4);goto L8c09d734; /* mov.l 0x8c09d804,r4 */
L8c09d734: *stop=0x8c09d734u;r[5]=rd(m,0x8c09d810u,4);goto L8c09d736; /* mov.l 0x8c09d810,r5 */
L8c09d736: *stop=0x8c09d738u;goto L8c09d8de; /* bra 0x8c09d8de */
L8c09d73a: *stop=0x8c09d73au;r[4]=rd(m,0x8c09d814u,4);goto L8c09d73c; /* mov.l 0x8c09d814,r4 */
L8c09d73c: target=0x8c09da44u;*pr=0x8c09d740u;*stop=0x8c09d73eu;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d740; /* bsr 0x8c09da44 */
L8c09d740: *stop=0x8c09d740u;r[4]=rd(m,0x8c09d81cu,4);goto L8c09d742; /* mov.l 0x8c09d81c,r4 */
L8c09d742: *stop=0x8c09d742u;r[5]=rd(m,0x8c09d818u,4);goto L8c09d744; /* mov.l 0x8c09d818,r5 */
L8c09d744: target=0x8c09da44u;*pr=0x8c09d748u;*stop=0x8c09d746u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d748; /* bsr 0x8c09da44 */
L8c09d748: *stop=0x8c09d748u;r[4]=rd(m,0x8c09d820u,4);goto L8c09d74a; /* mov.l 0x8c09d820,r4 */
L8c09d74a: target=0x8c09cce4u;*pr=0x8c09d74eu;*stop=0x8c09d74cu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d74e; /* bsr 0x8c09cce4 */
L8c09d74e: *stop=0x8c09d74eu;r[4]=rd(m,0x8c09d820u,4);goto L8c09d750; /* mov.l 0x8c09d820,r4 */
L8c09d750: *stop=0x8c09d752u;r[5]=r[14];goto L8c09d914; /* bra 0x8c09d914 */
L8c09d754: *stop=0x8c09d754u;r[4]=rd(m,0x8c09d814u,4);goto L8c09d756; /* mov.l 0x8c09d814,r4 */
L8c09d756: target=0x8c09da44u;*pr=0x8c09d75au;*stop=0x8c09d758u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d75a; /* bsr 0x8c09da44 */
L8c09d75a: *stop=0x8c09d75au;r[4]=rd(m,0x8c09d824u,4);goto L8c09d75c; /* mov.l 0x8c09d824,r4 */
L8c09d75c: *stop=0x8c09d75cu;r[5]=rd(m,0x8c09d818u,4);goto L8c09d75e; /* mov.l 0x8c09d818,r5 */
L8c09d75e: target=0x8c09da44u;*pr=0x8c09d762u;*stop=0x8c09d760u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d762; /* bsr 0x8c09da44 */
L8c09d762: *stop=0x8c09d762u;r[4]=rd(m,0x8c09d828u,4);goto L8c09d764; /* mov.l 0x8c09d828,r4 */
L8c09d764: target=0x8c09cce4u;*pr=0x8c09d768u;*stop=0x8c09d766u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d768; /* bsr 0x8c09cce4 */
L8c09d768: *stop=0x8c09d768u;r[4]=rd(m,0x8c09d828u,4);goto L8c09d76a; /* mov.l 0x8c09d828,r4 */
L8c09d76a: *stop=0x8c09d76cu;r[5]=r[14];goto L8c09d8de; /* bra 0x8c09d8de */
L8c09d76e: *stop=0x8c09d76eu;r[4]=rd(m,0x8c09d82cu,4);goto L8c09d770; /* mov.l 0x8c09d82c,r4 */
L8c09d770: target=0x8c09da44u;*pr=0x8c09d774u;*stop=0x8c09d772u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d774; /* bsr 0x8c09da44 */
L8c09d774: *stop=0x8c09d774u;r[3]=rd(m,0x8c09d830u,4);goto L8c09d776; /* mov.l 0x8c09d830,r3 */
L8c09d776: target=r[3];*pr=0x8c09d77au;*stop=0x8c09d778u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d77a; /* jsr @r3 */
L8c09d77a: *stop=0x8c09d77au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09d77c; /* tst r0,r0 */
L8c09d77c: if((*sr&1u))goto L8c09d788;goto L8c09d77e; /* bt 0x8c09d788 */
L8c09d77e: *stop=0x8c09d77eu;r[3]=rd(m,0x8c09d830u,4);goto L8c09d780; /* mov.l 0x8c09d830,r3 */
L8c09d780: target=r[3];*pr=0x8c09d784u;*stop=0x8c09d782u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d784; /* jsr @r3 */
L8c09d784: *stop=0x8c09d784u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c09d786; /* cmp/eq #0x2,r0 */
L8c09d786: if(!(*sr&1u))goto L8c09d798;goto L8c09d788; /* bf 0x8c09d798 */
L8c09d788: *stop=0x8c09d788u;r[4]=rd(m,0x8c09d838u,4);goto L8c09d78a; /* mov.l 0x8c09d838,r4 */
L8c09d78a: *stop=0x8c09d78au;r[5]=rd(m,0x8c09d834u,4);goto L8c09d78c; /* mov.l 0x8c09d834,r5 */
L8c09d78c: target=0x8c09da44u;*pr=0x8c09d790u;*stop=0x8c09d78eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d790; /* bsr 0x8c09da44 */
L8c09d790: *stop=0x8c09d790u;r[4]=rd(m,0x8c09d840u,4);goto L8c09d792; /* mov.l 0x8c09d840,r4 */
L8c09d792: *stop=0x8c09d792u;r[5]=rd(m,0x8c09d83cu,4);goto L8c09d794; /* mov.l 0x8c09d83c,r5 */
L8c09d794: target=0x8c09da44u;*pr=0x8c09d798u;*stop=0x8c09d796u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d798; /* bsr 0x8c09da44 */
L8c09d798: *stop=0x8c09d798u;r[4]=rd(m,0x8c09d844u,4);goto L8c09d79a; /* mov.l 0x8c09d844,r4 */
L8c09d79a: target=0x8c09cce4u;*pr=0x8c09d79eu;*stop=0x8c09d79cu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d79e; /* bsr 0x8c09cce4 */
L8c09d79e: *stop=0x8c09d79eu;r[4]=rd(m,0x8c09d848u,4);goto L8c09d7a0; /* mov.l 0x8c09d848,r4 */
L8c09d7a0: target=0x8c09cce4u;*pr=0x8c09d7a4u;*stop=0x8c09d7a2u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d7a4; /* bsr 0x8c09cce4 */
L8c09d7a4: *stop=0x8c09d7a4u;r[4]=rd(m,0x8c09d844u,4);goto L8c09d7a6; /* mov.l 0x8c09d844,r4 */
L8c09d7a6: target=0x8c09cd6eu;*pr=0x8c09d7aau;*stop=0x8c09d7a8u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d7aa; /* bsr 0x8c09cd6e */
L8c09d7aa: *stop=0x8c09d7aau;r[4]=rd(m,0x8c09d848u,4);goto L8c09d7ac; /* mov.l 0x8c09d848,r4 */
L8c09d7ac: *stop=0x8c09d7aeu;r[5]=r[10];goto L8c09d8de; /* bra 0x8c09d8de */
L8c09d7b0: *stop=0x8c09d7b0u;r[4]=rd(m,0x8c09d84cu,4);goto L8c09d7b2; /* mov.l 0x8c09d84c,r4 */
L8c09d7b2: target=0x8c09da44u;*pr=0x8c09d7b6u;*stop=0x8c09d7b4u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d7b6; /* bsr 0x8c09da44 */
L8c09d7b6: *stop=0x8c09d7b6u;r[2]=rd(m,0x8c09d850u,4);goto L8c09d7b8; /* mov.l 0x8c09d850,r2 */
L8c09d7b8: *stop=0x8c09d7b8u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c09d7ba; /* mov.l @r2,r3 */
L8c09d7ba: *stop=0x8c09d7bau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09d7bc; /* tst r3,r3 */
L8c09d7bc: if(!(*sr&1u))goto L8c09d7c4;goto L8c09d7be; /* bf 0x8c09d7c4 */
L8c09d7be: *stop=0x8c09d7beu;r[4]=rd(m,0x8c09d854u,4);goto L8c09d7c0; /* mov.l 0x8c09d854,r4 */
L8c09d7c0: *stop=0x8c09d7c2u;r[5]=r[11];goto L8c09d7c8; /* bra 0x8c09d7c8 */
L8c09d7c4: *stop=0x8c09d7c4u;r[4]=rd(m,0x8c09d858u,4);goto L8c09d7c6; /* mov.l 0x8c09d858,r4 */
L8c09d7c6: *stop=0x8c09d7c6u;r[5]=r[11];goto L8c09d7c8; /* mov r11,r5 */
L8c09d7c8: target=0x8c09da44u;*pr=0x8c09d7ccu;*stop=0x8c09d7cau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d7cc; /* bsr 0x8c09da44 */
L8c09d7cc: *stop=0x8c09d7ccu;r[4]=rd(m,0x8c09d85cu,4);goto L8c09d7ce; /* mov.l 0x8c09d85c,r4 */
L8c09d7ce: *stop=0x8c09d7ceu;r[5]=rd(m,0x8c09d818u,4);goto L8c09d7d0; /* mov.l 0x8c09d818,r5 */
L8c09d7d0: target=0x8c09da44u;*pr=0x8c09d7d4u;*stop=0x8c09d7d2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d7d4; /* bsr 0x8c09da44 */
L8c09d7d4: *stop=0x8c09d7d4u;r[4]=rd(m,0x8c09d860u,4);goto L8c09d7d6; /* mov.l 0x8c09d860,r4 */
L8c09d7d6: target=0x8c09cce4u;*pr=0x8c09d7dau;*stop=0x8c09d7d8u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d7da; /* bsr 0x8c09cce4 */
L8c09d7da: *stop=0x8c09d7dau;r[4]=rd(m,0x8c09d860u,4);goto L8c09d7dc; /* mov.l 0x8c09d860,r4 */
L8c09d7dc: *stop=0x8c09d7deu;r[5]=r[14];goto L8c09d8de; /* bra 0x8c09d8de */
L8c09d880: *stop=0x8c09d880u;r[4]=rd(m,0x8c09d940u,4);goto L8c09d882; /* mov.l 0x8c09d940,r4 */
L8c09d882: target=0x8c09da44u;*pr=0x8c09d886u;*stop=0x8c09d884u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d886; /* bsr 0x8c09da44 */
L8c09d886: *stop=0x8c09d886u;r[2]=rd(m,0x8c09d944u,4);goto L8c09d888; /* mov.l 0x8c09d944,r2 */
L8c09d888: *stop=0x8c09d888u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c09d88a; /* mov.l @r2,r3 */
L8c09d88a: *stop=0x8c09d88au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09d88c; /* tst r3,r3 */
L8c09d88c: if(!(*sr&1u))goto L8c09d894;goto L8c09d88e; /* bf 0x8c09d894 */
L8c09d88e: *stop=0x8c09d88eu;r[4]=rd(m,0x8c09d948u,4);goto L8c09d890; /* mov.l 0x8c09d948,r4 */
L8c09d890: *stop=0x8c09d892u;r[5]=r[11];goto L8c09d898; /* bra 0x8c09d898 */
L8c09d894: *stop=0x8c09d894u;r[4]=rd(m,0x8c09d94cu,4);goto L8c09d896; /* mov.l 0x8c09d94c,r4 */
L8c09d896: *stop=0x8c09d896u;r[5]=r[11];goto L8c09d898; /* mov r11,r5 */
L8c09d898: target=0x8c09da44u;*pr=0x8c09d89cu;*stop=0x8c09d89au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d89c; /* bsr 0x8c09da44 */
L8c09d89c: *stop=0x8c09d89cu;r[4]=rd(m,0x8c09d954u,4);goto L8c09d89e; /* mov.l 0x8c09d954,r4 */
L8c09d89e: *stop=0x8c09d89eu;r[5]=rd(m,0x8c09d950u,4);goto L8c09d8a0; /* mov.l 0x8c09d950,r5 */
L8c09d8a0: target=0x8c09da44u;*pr=0x8c09d8a4u;*stop=0x8c09d8a2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d8a4; /* bsr 0x8c09da44 */
L8c09d8a4: *stop=0x8c09d8a4u;r[4]=rd(m,0x8c09d958u,4);goto L8c09d8a6; /* mov.l 0x8c09d958,r4 */
L8c09d8a6: target=0x8c09cce4u;*pr=0x8c09d8aau;*stop=0x8c09d8a8u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d8aa; /* bsr 0x8c09cce4 */
L8c09d8aa: *stop=0x8c09d8aau;r[4]=rd(m,0x8c09d958u,4);goto L8c09d8ac; /* mov.l 0x8c09d958,r4 */
L8c09d8ac: *stop=0x8c09d8aeu;r[5]=r[14];goto L8c09d8de; /* bra 0x8c09d8de */
L8c09d8b0: *stop=0x8c09d8b0u;r[4]=rd(m,0x8c09d940u,4);goto L8c09d8b2; /* mov.l 0x8c09d940,r4 */
L8c09d8b2: target=0x8c09da44u;*pr=0x8c09d8b6u;*stop=0x8c09d8b4u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d8b6; /* bsr 0x8c09da44 */
L8c09d8b6: *stop=0x8c09d8b6u;r[2]=rd(m,0x8c09d944u,4);goto L8c09d8b8; /* mov.l 0x8c09d944,r2 */
L8c09d8b8: *stop=0x8c09d8b8u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c09d8ba; /* mov.l @r2,r3 */
L8c09d8ba: *stop=0x8c09d8bau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09d8bc; /* tst r3,r3 */
L8c09d8bc: if(!(*sr&1u))goto L8c09d8c4;goto L8c09d8be; /* bf 0x8c09d8c4 */
L8c09d8be: *stop=0x8c09d8beu;r[4]=rd(m,0x8c09d948u,4);goto L8c09d8c0; /* mov.l 0x8c09d948,r4 */
L8c09d8c0: *stop=0x8c09d8c2u;r[5]=r[11];goto L8c09d8c8; /* bra 0x8c09d8c8 */
L8c09d8c4: *stop=0x8c09d8c4u;r[4]=rd(m,0x8c09d94cu,4);goto L8c09d8c6; /* mov.l 0x8c09d94c,r4 */
L8c09d8c6: *stop=0x8c09d8c6u;r[5]=r[11];goto L8c09d8c8; /* mov r11,r5 */
L8c09d8c8: target=0x8c09da44u;*pr=0x8c09d8ccu;*stop=0x8c09d8cau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d8cc; /* bsr 0x8c09da44 */
L8c09d8cc: *stop=0x8c09d8ccu;r[4]=rd(m,0x8c09d95cu,4);goto L8c09d8ce; /* mov.l 0x8c09d95c,r4 */
L8c09d8ce: *stop=0x8c09d8ceu;r[5]=rd(m,0x8c09d950u,4);goto L8c09d8d0; /* mov.l 0x8c09d950,r5 */
L8c09d8d0: target=0x8c09da44u;*pr=0x8c09d8d4u;*stop=0x8c09d8d2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d8d4; /* bsr 0x8c09da44 */
L8c09d8d4: *stop=0x8c09d8d4u;r[4]=rd(m,0x8c09d960u,4);goto L8c09d8d6; /* mov.l 0x8c09d960,r4 */
L8c09d8d6: target=0x8c09cce4u;*pr=0x8c09d8dau;*stop=0x8c09d8d8u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d8da; /* bsr 0x8c09cce4 */
L8c09d8da: *stop=0x8c09d8dau;r[4]=rd(m,0x8c09d960u,4);goto L8c09d8dc; /* mov.l 0x8c09d960,r4 */
L8c09d8dc: *stop=0x8c09d8dcu;r[5]=r[14];goto L8c09d8de; /* mov r14,r5 */
L8c09d8de: *stop=0x8c09d8deu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09d8e0; /* lds.l @r15+,PR */
L8c09d8e0: *stop=0x8c09d8e0u;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c09d8e2; /* mov.l @r15+,r8 */
L8c09d8e2: *stop=0x8c09d8e2u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c09d8e4; /* mov.l @r15+,r9 */
L8c09d8e4: *stop=0x8c09d8e4u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c09d8e6; /* mov.l @r15+,r10 */
L8c09d8e6: *stop=0x8c09d8e6u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c09d8e8; /* mov.l @r15+,r11 */
L8c09d8e8: *stop=0x8c09d8e8u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c09d8ea; /* mov.l @r15+,r12 */
L8c09d8ea: *stop=0x8c09d8eau;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09d8ec; /* mov.l @r15+,r13 */
L8c09d8ec: *stop=0x8c09d8eeu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0x8c09cd6eu,m,f,sr,r,pr); /* bra 0x8c09cd6e */
L8c09d8f0: *stop=0x8c09d8f0u;r[4]=rd(m,0x8c09d940u,4);goto L8c09d8f2; /* mov.l 0x8c09d940,r4 */
L8c09d8f2: target=0x8c09da44u;*pr=0x8c09d8f6u;*stop=0x8c09d8f4u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d8f6; /* bsr 0x8c09da44 */
L8c09d8f6: *stop=0x8c09d8f6u;r[4]=rd(m,0x8c09d964u,4);goto L8c09d8f8; /* mov.l 0x8c09d964,r4 */
L8c09d8f8: *stop=0x8c09d8f8u;r[5]=rd(m,0x8c09d950u,4);goto L8c09d8fa; /* mov.l 0x8c09d950,r5 */
L8c09d8fa: target=0x8c09da44u;*pr=0x8c09d8feu;*stop=0x8c09d8fcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d8fe; /* bsr 0x8c09da44 */
L8c09d8fe: *stop=0x8c09d8feu;r[4]=rd(m,0x8c09d968u,4);goto L8c09d900; /* mov.l 0x8c09d968,r4 */
L8c09d900: target=0x8c09cce4u;*pr=0x8c09d904u;*stop=0x8c09d902u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d904; /* bsr 0x8c09cce4 */
L8c09d904: *stop=0x8c09d904u;r[4]=rd(m,0x8c09d96cu,4);goto L8c09d906; /* mov.l 0x8c09d96c,r4 */
L8c09d906: target=0x8c09cce4u;*pr=0x8c09d90au;*stop=0x8c09d908u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d90a; /* bsr 0x8c09cce4 */
L8c09d90a: *stop=0x8c09d90au;r[4]=rd(m,0x8c09d968u,4);goto L8c09d90c; /* mov.l 0x8c09d968,r4 */
L8c09d90c: target=0x8c09cd6eu;*pr=0x8c09d910u;*stop=0x8c09d90eu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d910; /* bsr 0x8c09cd6e */
L8c09d910: *stop=0x8c09d910u;r[4]=rd(m,0x8c09d96cu,4);goto L8c09d912; /* mov.l 0x8c09d96c,r4 */
L8c09d912: *stop=0x8c09d912u;r[5]=r[10];goto L8c09d914; /* mov r10,r5 */
L8c09d914: *stop=0x8c09d914u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09d916; /* lds.l @r15+,PR */
L8c09d916: *stop=0x8c09d916u;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c09d918; /* mov.l @r15+,r8 */
L8c09d918: *stop=0x8c09d918u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c09d91a; /* mov.l @r15+,r9 */
L8c09d91a: *stop=0x8c09d91au;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c09d91c; /* mov.l @r15+,r10 */
L8c09d91c: *stop=0x8c09d91cu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c09d91e; /* mov.l @r15+,r11 */
L8c09d91e: *stop=0x8c09d91eu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c09d920; /* mov.l @r15+,r12 */
L8c09d920: *stop=0x8c09d920u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09d922; /* mov.l @r15+,r13 */
L8c09d922: *stop=0x8c09d924u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0x8c09cd6eu,m,f,sr,r,pr); /* bra 0x8c09cd6e */
L8c09d926: *stop=0x8c09d928u;goto L8c09d926; /* bra 0x8c09d926 */
L8c09d92a: *stop=0x8c09d92au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09d92c; /* lds.l @r15+,PR */
L8c09d92c: *stop=0x8c09d92cu;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c09d92e; /* mov.l @r15+,r8 */
L8c09d92e: *stop=0x8c09d92eu;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c09d930; /* mov.l @r15+,r9 */
L8c09d930: *stop=0x8c09d930u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c09d932; /* mov.l @r15+,r10 */
L8c09d932: *stop=0x8c09d932u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c09d934; /* mov.l @r15+,r11 */
L8c09d934: *stop=0x8c09d934u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c09d936; /* mov.l @r15+,r12 */
L8c09d936: *stop=0x8c09d936u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09d938; /* mov.l @r15+,r13 */
L8c09d938: *stop=0x8c09d938u;if(*pr!=entry_pr)return -3;*stop=0x8c09d93au;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_resource_bounds_8c052ba0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c052ba0;
L8c052ba0: *stop=0x8c052ba0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c052ba2; /* mov.l r14,@-r15 */
L8c052ba2: *stop=0x8c052ba2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c052ba4; /* mov.l r13,@-r15 */
L8c052ba4: *stop=0x8c052ba4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c052ba6; /* mov.l r12,@-r15 */
L8c052ba6: *stop=0x8c052ba6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c052ba8; /* mov.l r11,@-r15 */
L8c052ba8: *stop=0x8c052ba8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c052baa; /* mov.l r10,@-r15 */
L8c052baa: *stop=0x8c052baau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c052bac; /* mov.l r9,@-r15 */
L8c052bac: *stop=0x8c052bacu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c052bae; /* mov.l r8,@-r15 */
L8c052bae: *stop=0x8c052baeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c052bb0; /* sts.l PR,@-r15 */
L8c052bb0: *stop=0x8c052bb0u;r[15]+=4294967264u;goto L8c052bb2; /* add #-0x20,r15 */
L8c052bb2: *stop=0x8c052bb2u;r[1]=rd(m,0x8c052da0u,4);goto L8c052bb4; /* mov.l 0x8c052da0,r1 */
L8c052bb4: target=r[1];*pr=0x8c052bb8u;*stop=0x8c052bb6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052bb8; /* jsr @r1 */
L8c052bb8: *stop=0x8c052bb8u;r[3]=rd(m,0x8c052da4u,4);goto L8c052bba; /* mov.l 0x8c052da4,r3 */
L8c052bba: *stop=0x8c052bbau;r[4]=r[0]&65535u;goto L8c052bbc; /* extu.w r0,r4 */
L8c052bbc: *stop=0x8c052bbcu;r[2]=4294967288u;goto L8c052bbe; /* mov #-0x8,r2 */
L8c052bbe: *stop=0x8c052bbeu;r[1]=rd(m,0x8c052dacu,4);goto L8c052bc0; /* mov.l 0x8c052dac,r1 */
L8c052bc0: *stop=0x8c052bc0u;r[4]&=r[3];goto L8c052bc2; /* and r3,r4 */
L8c052bc2: *stop=0x8c052bc2u;r[10]=rd(m,0x8c052db0u,4);goto L8c052bc4; /* mov.l 0x8c052db0,r10 */
L8c052bc4: *stop=0x8c052bc4u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[4]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[4]=n?(uint32_t)((int32_t)r[4]>>n):((r[4]&0x80000000u)?0xffffffffu:0u);}}goto L8c052bc6; /* shad r2,r4 */
L8c052bc6: *stop=0x8c052bc6u;r[2]=rd(m,0x8c052da8u,4);goto L8c052bc8; /* mov.l 0x8c052da8,r2 */
L8c052bc8: *stop=0x8c052bc8u;r[4]=r[4]&65535u;goto L8c052bca; /* extu.w r4,r4 */
L8c052bca: *stop=0x8c052bcau;r[4]<<=2;goto L8c052bcc; /* shll2 r4 */
L8c052bcc: *stop=0x8c052bccu;r[4]<<=2;goto L8c052bce; /* shll2 r4 */
L8c052bce: *stop=0x8c052bceu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c052bd0; /* shll r4 */
L8c052bd0: *stop=0x8c052bd0u;r[2]+=r[4];goto L8c052bd2; /* add r4,r2 */
L8c052bd2: *stop=0x8c052bd2u;r[9]=0u;goto L8c052bd4; /* mov #0x0,r9 */
L8c052bd4: *stop=0x8c052bd4u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[9],4);goto L8c052bd6; /* mov.l r9,@(0x8,r15) */
L8c052bd6: *stop=0x8c052bd6u;if(!valid_ram(r[15]+16u,4u))return -3;wr(m,r[15]+0x10u,r[2],4);goto L8c052bd8; /* mov.l r2,@(0x10,r15) */
L8c052bd8: *stop=0x8c052bd8u;if(!valid_ram(r[15]+12u,4u))return -3;wr(m,r[15]+0xcu,r[1],4);goto L8c052bda; /* mov.l r1,@(0xc,r15) */
L8c052bda: *stop=0x8c052bdau;if(!valid_ram(r[15]+16u,4u))return -3;r[3]=rd(m,r[15]+0x10u,4);goto L8c052bdc; /* mov.l @(0x10,r15),r3 */
L8c052bdc: *stop=0x8c052bdcu;r[3]+=4u;goto L8c052bde; /* add #0x4,r3 */
L8c052bde: *stop=0x8c052bdeu;if(!valid_ram(r[15]+16u,4u))return -3;wr(m,r[15]+0x10u,r[3],4);goto L8c052be0; /* mov.l r3,@(0x10,r15) */
L8c052be0: *stop=0x8c052be0u;r[3]+=4294967292u;goto L8c052be2; /* add #-0x4,r3 */
L8c052be2: *stop=0x8c052be2u;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c052be4; /* mov.l @r3,r2 */
L8c052be4: *stop=0x8c052be4u;r[4]=r[2];goto L8c052be6; /* mov r2,r4 */
L8c052be6: *stop=0x8c052be6u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c052be8; /* tst r4,r4 */
L8c052be8: take=!(*sr&1u);*stop=0x8c052beau;if(!valid_ram(r[10],4u))return -3;wr(m,r[10],r[2],4);if(take)goto L8c052bf0;goto L8c052bec; /* bf/s 0x8c052bf0 */
L8c052bec: *stop=0x8c052beeu;goto L8c052d12; /* bra 0x8c052d12 */
L8c052bf0: *stop=0x8c052bf0u;if(!valid_ram(r[4],2u))return -3;r[4]=sx(m,r[4],2);goto L8c052bf2; /* mov.w @r4,r4 */
L8c052bf2: *stop=0x8c052bf2u;r[11]=r[9];goto L8c052bf4; /* mov r9,r11 */
L8c052bf4: *stop=0x8c052bf4u;if(!valid_ram(r[15],2u))return -3;wr(m,r[15],r[9],2);goto L8c052bf6; /* mov.w r9,@r15 */
L8c052bf6: *stop=0x8c052bf6u;r[3]=r[4];goto L8c052bf8; /* mov r4,r3 */
L8c052bf8: *stop=0x8c052bf8u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c052bfa; /* cmp/pl r3 */
L8c052bfa: take=(*sr&1u);*stop=0x8c052bfcu;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[4],4);if(take)goto L8c052c02;goto L8c052bfe; /* bt/s 0x8c052c02 */
L8c052bfe: *stop=0x8c052c00u;goto L8c052d12; /* bra 0x8c052d12 */
L8c052c02: *stop=0x8c052c02u;if(!valid_ram(r[10],4u))return -3;r[14]=rd(m,r[10],4);goto L8c052c04; /* mov.l @r10,r14 */
L8c052c04: *stop=0x8c052c04u;if(!valid_ram(r[14]+12u,4u))return -3;r[14]=rd(m,r[14]+0xcu,4);goto L8c052c06; /* mov.l @(0xc,r14),r14 */
L8c052c06: *stop=0x8c052c06u;r[14]+=r[11];goto L8c052c08; /* add r11,r14 */
L8c052c08: *stop=0x8c052c08u;if(!valid_ram(r[14]+24u,4u))return -3;r[12]=rd(m,r[14]+0x18u,4);goto L8c052c0a; /* mov.l @(0x18,r14),r12 */
L8c052c0a: *stop=0x8c052c0au;if(!valid_ram(r[12]+4u,4u))return -3;r[13]=rd(m,r[12]+0x4u,4);goto L8c052c0c; /* mov.l @(0x4,r12),r13 */
L8c052c0c: *stop=0x8c052c0cu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c052c0e; /* tst r13,r13 */
L8c052c0e: if((*sr&1u))goto L8c052cfe;goto L8c052c10; /* bt 0x8c052cfe */
L8c052c10: *stop=0x8c052c10u;r[0]=12u;goto L8c052c12; /* mov #0xc,r0 */
L8c052c12: *stop=0x8c052c12u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c052c14; /* fldi0 fr3 */
L8c052c14: *stop=0x8c052c14u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c052c16; /* fmov.s @(r0,r14),fr2 */
L8c052c16: *stop=0x8c052c16u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c052c18; /* fcmp/eq fr3,fr2 */
L8c052c18: if((*sr&1u))goto L8c052c2c;goto L8c052c1a; /* bt 0x8c052c2c */
L8c052c1a: *stop=0x8c052c1au;r[0]=16u;goto L8c052c1c; /* mov #0x10,r0 */
L8c052c1c: *stop=0x8c052c1cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c052c1e; /* fmov.s @(r0,r14),fr2 */
L8c052c1e: *stop=0x8c052c1eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c052c20; /* fcmp/eq fr3,fr2 */
L8c052c20: if(!(*sr&1u))goto L8c052c2c;goto L8c052c22; /* bf 0x8c052c2c */
L8c052c22: *stop=0x8c052c22u;r[0]=20u;goto L8c052c24; /* mov #0x14,r0 */
L8c052c24: *stop=0x8c052c24u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c052c26; /* fldi0 fr3 */
L8c052c26: *stop=0x8c052c26u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c052c28; /* fmov.s @(r0,r14),fr2 */
L8c052c28: *stop=0x8c052c28u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c052c2a; /* fcmp/eq fr3,fr2 */
L8c052c2a: if((*sr&1u))goto L8c052cfe;goto L8c052c2c; /* bt 0x8c052cfe */
L8c052c2c: *stop=0x8c052c2cu;r[3]=rd(m,0x8c052db4u,4);goto L8c052c2e; /* mov.l 0x8c052db4,r3 */
L8c052c2e: target=r[3];*pr=0x8c052c32u;*stop=0x8c052c30u;if(!valid_ram(r[15]+12u,4u))return -3;r[4]=rd(m,r[15]+0xcu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052c32; /* jsr @r3 */
L8c052c32: *stop=0x8c052c32u;r[8]=r[12];goto L8c052c34; /* mov r12,r8 */
L8c052c34: *stop=0x8c052c34u;r[0]=8u;goto L8c052c36; /* mov #0x8,r0 */
L8c052c36: *stop=0x8c052c36u;r[8]+=8u;goto L8c052c38; /* add #0x8,r8 */
L8c052c38: *stop=0x8c052c38u;r[3]=rd(m,0x8c052db8u,4);goto L8c052c3a; /* mov.l 0x8c052db8,r3 */
L8c052c3a: *stop=0x8c052c3au;if(!valid_ram(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[8]);goto L8c052c3c; /* fmov.s @(r0,r8),fr6 */
L8c052c3c: *stop=0x8c052c3cu;r[0]=4u;goto L8c052c3e; /* mov #0x4,r0 */
L8c052c3e: *stop=0x8c052c3eu;if(!valid_ram(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[8]);goto L8c052c40; /* fmov.s @r8,fr4 */
L8c052c40: *stop=0x8c052c40u;if(!valid_ram(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[8]);goto L8c052c42; /* fmov.s @(r0,r8),fr5 */
L8c052c42: target=r[3];*pr=0x8c052c46u;*stop=0x8c052c44u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052c46; /* jsr @r3 */
L8c052c46: *stop=0x8c052c46u;if(!valid_ram(r[12]+20u,4u))return -3;r[8]=rd(m,r[12]+0x14u,4);goto L8c052c48; /* mov.l @(0x14,r12),r8 */
L8c052c48: *stop=0x8c052c48u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c052c4a; /* tst r8,r8 */
L8c052c4a: if((*sr&1u))goto L8c052c54;goto L8c052c4c; /* bt 0x8c052c54 */
L8c052c4c: *stop=0x8c052c4cu;r[3]=rd(m,0x8c052dbcu,4);goto L8c052c4e; /* mov.l 0x8c052dbc,r3 */
L8c052c4e: *stop=0x8c052c4eu;r[5]=r[8];goto L8c052c50; /* mov r8,r5 */
L8c052c50: target=r[3];*pr=0x8c052c54u;*stop=0x8c052c52u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052c54; /* jsr @r3 */
L8c052c54: *stop=0x8c052c54u;if(!valid_ram(r[12]+24u,4u))return -3;r[8]=rd(m,r[12]+0x18u,4);goto L8c052c56; /* mov.l @(0x18,r12),r8 */
L8c052c56: *stop=0x8c052c56u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c052c58; /* tst r8,r8 */
L8c052c58: if((*sr&1u))goto L8c052c62;goto L8c052c5a; /* bt 0x8c052c62 */
L8c052c5a: *stop=0x8c052c5au;r[2]=rd(m,0x8c052dc0u,4);goto L8c052c5c; /* mov.l 0x8c052dc0,r2 */
L8c052c5c: *stop=0x8c052c5cu;r[5]=r[8];goto L8c052c5e; /* mov r8,r5 */
L8c052c5e: target=r[2];*pr=0x8c052c62u;*stop=0x8c052c60u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052c62; /* jsr @r2 */
L8c052c62: *stop=0x8c052c62u;r[0]=12u;goto L8c052c64; /* mov #0xc,r0 */
L8c052c64: *stop=0x8c052c64u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c052c66; /* fldi0 fr3 */
L8c052c66: *stop=0x8c052c66u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c052c68; /* fmov.s @(r0,r14),fr2 */
L8c052c68: *stop=0x8c052c68u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c052c6a; /* fcmp/eq fr3,fr2 */
L8c052c6a: if(!(*sr&1u))goto L8c052c9c;goto L8c052c6c; /* bf 0x8c052c9c */
L8c052c6c: *stop=0x8c052c6cu;r[0]=16u;goto L8c052c6e; /* mov #0x10,r0 */
L8c052c6e: *stop=0x8c052c6eu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c052c70; /* fmov.s @(r0,r14),fr2 */
L8c052c70: *stop=0x8c052c70u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c052c72; /* fcmp/eq fr3,fr2 */
L8c052c72: if(!(*sr&1u))goto L8c052c9c;goto L8c052c74; /* bf 0x8c052c9c */
L8c052c74: *stop=0x8c052c74u;r[0]=20u;goto L8c052c76; /* mov #0x14,r0 */
L8c052c76: *stop=0x8c052c76u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c052c78; /* fldi0 fr3 */
L8c052c78: *stop=0x8c052c78u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c052c7a; /* fmov.s @(r0,r14),fr2 */
L8c052c7a: *stop=0x8c052c7au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c052c7c; /* fcmp/eq fr3,fr2 */
L8c052c7c: if(!(*sr&1u))goto L8c052c9c;goto L8c052c7e; /* bf 0x8c052c9c */
L8c052c7e: *stop=0x8c052c7eu;r[0]=24u;goto L8c052c80; /* mov #0x18,r0 */
L8c052c80: *stop=0x8c052c80u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c052c82; /* fmov.s @(r0,r13),fr3 */
L8c052c82: *stop=0x8c052c82u;r[0]=20u;goto L8c052c84; /* mov #0x14,r0 */
L8c052c84: *stop=0x8c052c84u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c052c86; /* fmov fr3,@(r0,r15) */
L8c052c86: *stop=0x8c052c86u;r[0]=28u;goto L8c052c88; /* mov #0x1c,r0 */
L8c052c88: *stop=0x8c052c88u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c052c8a; /* fmov.s @(r0,r13),fr3 */
L8c052c8a: *stop=0x8c052c8au;r[0]=24u;goto L8c052c8c; /* mov #0x18,r0 */
L8c052c8c: *stop=0x8c052c8cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c052c8e; /* fmov fr3,@(r0,r15) */
L8c052c8e: *stop=0x8c052c8eu;r[0]=32u;goto L8c052c90; /* mov #0x20,r0 */
L8c052c90: *stop=0x8c052c90u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c052c92; /* fmov.s @(r0,r13),fr3 */
L8c052c92: *stop=0x8c052c92u;r[0]=28u;goto L8c052c94; /* mov #0x1c,r0 */
L8c052c94: *stop=0x8c052c94u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c052c96; /* fmov fr3,@(r0,r15) */
L8c052c96: *stop=0x8c052c96u;r[0]=36u;goto L8c052c98; /* mov #0x24,r0 */
L8c052c98: *stop=0x8c052c9au;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[13]);goto L8c052cd0; /* bra 0x8c052cd0 */
L8c052c9c: *stop=0x8c052c9cu;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[14]);goto L8c052c9e; /* fmov.s @r14,fr3 */
L8c052c9e: *stop=0x8c052c9eu;r[0]=20u;goto L8c052ca0; /* mov #0x14,r0 */
L8c052ca0: *stop=0x8c052ca0u;r[3]=rd(m,0x8c052dc4u,4);goto L8c052ca2; /* mov.l 0x8c052dc4,r3 */
L8c052ca2: *stop=0x8c052ca2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c052ca4; /* fmov fr3,@(r0,r15) */
L8c052ca4: *stop=0x8c052ca4u;r[0]=4u;goto L8c052ca6; /* mov #0x4,r0 */
L8c052ca6: *stop=0x8c052ca6u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c052ca8; /* fmov.s @(r0,r14),fr3 */
L8c052ca8: *stop=0x8c052ca8u;r[0]=24u;goto L8c052caa; /* mov #0x18,r0 */
L8c052caa: *stop=0x8c052caau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c052cac; /* fmov fr3,@(r0,r15) */
L8c052cac: *stop=0x8c052cacu;r[0]=8u;goto L8c052cae; /* mov #0x8,r0 */
L8c052cae: *stop=0x8c052caeu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c052cb0; /* fmov.s @(r0,r14),fr3 */
L8c052cb0: *stop=0x8c052cb0u;r[0]=28u;goto L8c052cb2; /* mov #0x1c,r0 */
L8c052cb2: *stop=0x8c052cb2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c052cb4; /* fmov fr3,@(r0,r15) */
L8c052cb4: *stop=0x8c052cb4u;r[0]=12u;goto L8c052cb6; /* mov #0xc,r0 */
L8c052cb6: *stop=0x8c052cb6u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c052cb8; /* fmov.s @(r0,r14),fr4 */
L8c052cb8: *stop=0x8c052cb8u;r[0]=16u;goto L8c052cba; /* mov #0x10,r0 */
L8c052cba: *stop=0x8c052cbau;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto L8c052cbc; /* fmov.s @(r0,r14),fr5 */
L8c052cbc: *stop=0x8c052cbcu;r[0]=20u;goto L8c052cbe; /* mov #0x14,r0 */
L8c052cbe: *stop=0x8c052cbeu;fcopy(f,3u,4u);goto L8c052cc0; /* fmov fr4,fr3 */
L8c052cc0: *stop=0x8c052cc0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c052cc2; /* fmul fr3,fr4 */
L8c052cc2: *stop=0x8c052cc2u;fcopy(f,0u,5u);goto L8c052cc4; /* fmov fr5,fr0 */
L8c052cc4: *stop=0x8c052cc4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[5],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c052cc6; /* fmac fr0,fr5,fr4 */
L8c052cc6: *stop=0x8c052cc6u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto L8c052cc8; /* fmov.s @(r0,r14),fr5 */
L8c052cc8: *stop=0x8c052cc8u;fcopy(f,0u,5u);goto L8c052cca; /* fmov fr5,fr0 */
L8c052cca: target=r[3];*pr=0x8c052cceu;*stop=0x8c052cccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[5],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052cce; /* jsr @r3 */
L8c052cce: *stop=0x8c052cceu;fcopy(f,5u,0u);goto L8c052cd0; /* fmov fr0,fr5 */
L8c052cd0: *stop=0x8c052cd0u;r[6]=r[15];goto L8c052cd2; /* mov r15,r6 */
L8c052cd2: *stop=0x8c052cd2u;r[5]=r[15];goto L8c052cd4; /* mov r15,r5 */
L8c052cd4: *stop=0x8c052cd4u;r[0]=12u;goto L8c052cd6; /* mov #0xc,r0 */
L8c052cd6: *stop=0x8c052cd6u;r[3]=rd(m,0x8c052dc8u,4);goto L8c052cd8; /* mov.l 0x8c052dc8,r3 */
L8c052cd8: *stop=0x8c052cd8u;r[5]+=20u;goto L8c052cda; /* add #0x14,r5 */
L8c052cda: *stop=0x8c052cdau;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[14]);goto L8c052cdc; /* fmov fr5,@(r0,r14) */
L8c052cdc: *stop=0x8c052cdcu;r[6]+=20u;goto L8c052cde; /* add #0x14,r6 */
L8c052cde: target=r[3];*pr=0x8c052ce2u;*stop=0x8c052ce0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052ce2; /* jsr @r3 */
L8c052ce2: *stop=0x8c052ce2u;r[2]=rd(m,0x8c052dccu,4);goto L8c052ce4; /* mov.l 0x8c052dcc,r2 */
L8c052ce4: target=r[2];*pr=0x8c052ce8u;*stop=0x8c052ce6u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052ce8; /* jsr @r2 */
L8c052ce8: *stop=0x8c052ce8u;r[0]=20u;goto L8c052cea; /* mov #0x14,r0 */
L8c052cea: *stop=0x8c052ceau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c052cec; /* fmov.s @(r0,r15),fr3 */
L8c052cec: *stop=0x8c052cecu;r[0]=24u;goto L8c052cee; /* mov #0x18,r0 */
L8c052cee: *stop=0x8c052ceeu;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[14]);goto L8c052cf0; /* fmov.s fr3,@r14 */
L8c052cf0: *stop=0x8c052cf0u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c052cf2; /* fmov.s @(r0,r15),fr3 */
L8c052cf2: *stop=0x8c052cf2u;r[0]=4u;goto L8c052cf4; /* mov #0x4,r0 */
L8c052cf4: *stop=0x8c052cf4u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto L8c052cf6; /* fmov fr3,@(r0,r14) */
L8c052cf6: *stop=0x8c052cf6u;r[0]=28u;goto L8c052cf8; /* mov #0x1c,r0 */
L8c052cf8: *stop=0x8c052cf8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c052cfa; /* fmov.s @(r0,r15),fr3 */
L8c052cfa: *stop=0x8c052cfau;r[0]=8u;goto L8c052cfc; /* mov #0x8,r0 */
L8c052cfc: *stop=0x8c052cfcu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto L8c052cfe; /* fmov fr3,@(r0,r14) */
L8c052cfe: *stop=0x8c052cfeu;if(!valid_ram(r[15],2u))return -3;r[3]=sx(m,r[15],2);goto L8c052d00; /* mov.w @r15,r3 */
L8c052d00: *stop=0x8c052d00u;r[3]+=1u;goto L8c052d02; /* add #0x1,r3 */
L8c052d02: *stop=0x8c052d02u;if(!valid_ram(r[15],2u))return -3;wr(m,r[15],r[3],2);goto L8c052d04; /* mov.w r3,@r15 */
L8c052d04: *stop=0x8c052d04u;r[3]=(uint32_t)(int32_t)(int16_t)r[3];goto L8c052d06; /* exts.w r3,r3 */
L8c052d06: *stop=0x8c052d06u;if(!valid_ram(r[15]+4u,4u))return -3;r[2]=rd(m,r[15]+0x4u,4);goto L8c052d08; /* mov.l @(0x4,r15),r2 */
L8c052d08: *stop=0x8c052d08u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c052d0a; /* cmp/ge r2,r3 */
L8c052d0a: take=(*sr&1u);*stop=0x8c052d0cu;r[11]+=36u;if(take)goto L8c052d12;goto L8c052d0e; /* bt/s 0x8c052d12 */
L8c052d0e: *stop=0x8c052d10u;goto L8c052c02; /* bra 0x8c052c02 */
L8c052d12: *stop=0x8c052d12u;if(!valid_ram(r[15]+8u,4u))return -3;r[1]=rd(m,r[15]+0x8u,4);goto L8c052d14; /* mov.l @(0x8,r15),r1 */
L8c052d14: *stop=0x8c052d14u;r[3]=8u;goto L8c052d16; /* mov #0x8,r3 */
L8c052d16: *stop=0x8c052d16u;r[1]+=1u;goto L8c052d18; /* add #0x1,r1 */
L8c052d18: *stop=0x8c052d18u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[3]);goto L8c052d1a; /* cmp/ge r3,r1 */
L8c052d1a: take=(*sr&1u);*stop=0x8c052d1cu;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[1],4);if(take)goto L8c052d22;goto L8c052d1e; /* bt/s 0x8c052d22 */
L8c052d1e: *stop=0x8c052d20u;goto L8c052bda; /* bra 0x8c052bda */
L8c052d22: *stop=0x8c052d22u;r[15]+=32u;goto L8c052d24; /* add #0x20,r15 */
L8c052d24: *stop=0x8c052d24u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c052d26; /* lds.l @r15+,PR */
L8c052d26: *stop=0x8c052d26u;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c052d28; /* mov.l @r15+,r8 */
L8c052d28: *stop=0x8c052d28u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c052d2a; /* mov.l @r15+,r9 */
L8c052d2a: *stop=0x8c052d2au;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c052d2c; /* mov.l @r15+,r10 */
L8c052d2c: *stop=0x8c052d2cu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c052d2e; /* mov.l @r15+,r11 */
L8c052d2e: *stop=0x8c052d2eu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c052d30; /* mov.l @r15+,r12 */
L8c052d30: *stop=0x8c052d30u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c052d32; /* mov.l @r15+,r13 */
L8c052d32: *stop=0x8c052d32u;if(*pr!=entry_pr)return -3;*stop=0x8c052d34u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_resource_localized_file_8c09cce4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09cce4;
L8c09cce4: *stop=0x8c09cce4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09cce6; /* mov.l r14,@-r15 */
L8c09cce6: *stop=0x8c09cce6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c09cce8; /* mov.l r13,@-r15 */
L8c09cce8: *stop=0x8c09cce8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c09ccea; /* mov.l r12,@-r15 */
L8c09ccea: *stop=0x8c09cceau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c09ccec; /* sts.l PR,@-r15 */
L8c09ccec: *stop=0x8c09ccecu;r[15]+=4294967248u;goto L8c09ccee; /* add #-0x30,r15 */
L8c09ccee: *stop=0x8c09cceeu;r[2]=rd(m,0x8c09ce1cu,4);goto L8c09ccf0; /* mov.l 0x8c09ce1c,r2 */
L8c09ccf0: *stop=0x8c09ccf0u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[4],4);goto L8c09ccf2; /* mov.l r4,@(0x8,r15) */
L8c09ccf2: *stop=0x8c09ccf2u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[5],4);goto L8c09ccf4; /* mov.l r5,@(0x4,r15) */
L8c09ccf4: *stop=0x8c09ccf4u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c09ccf6; /* mov.l @r2,r3 */
L8c09ccf6: *stop=0x8c09ccf6u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09ccf8; /* tst r3,r3 */
L8c09ccf8: if((*sr&1u))goto L8c09cd00;goto L8c09ccfa; /* bt 0x8c09cd00 */
L8c09ccfa: *stop=0x8c09ccfau;r[3]=rd(m,0x8c09ce20u,4);goto L8c09ccfc; /* mov.l 0x8c09ce20,r3 */
L8c09ccfc: *stop=0x8c09ccfeu;if(!valid_ram(r[3],4u))return -3;r[12]=rd(m,r[3],4);goto L8c09cd0e; /* bra 0x8c09cd0e */
L8c09cd00: *stop=0x8c09cd00u;r[3]=rd(m,0x8c09ce24u,4);goto L8c09cd02; /* mov.l 0x8c09ce24,r3 */
L8c09cd02: target=r[3];*pr=0x8c09cd06u;*stop=0x8c09cd04u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd06; /* jsr @r3 */
L8c09cd06: *stop=0x8c09cd06u;r[12]=r[0];goto L8c09cd08; /* mov r0,r12 */
L8c09cd08: *stop=0x8c09cd08u;r[0]=rd(m,0x8c09ce28u,4);goto L8c09cd0a; /* mov.l 0x8c09ce28,r0 */
L8c09cd0a: *stop=0x8c09cd0au;r[12]<<=2;goto L8c09cd0c; /* shll2 r12 */
L8c09cd0c: *stop=0x8c09cd0cu;if(!valid_ram(r[0]+r[12],4u))return -3;r[12]=rd(m,r[12]+r[0],4);goto L8c09cd0e; /* mov.l @(r0,r12),r12 */
L8c09cd0e: *stop=0x8c09cd0eu;r[5]=rd(m,0x8c09ce2cu,4);goto L8c09cd10; /* mov.l 0x8c09ce2c,r5 */
L8c09cd10: *stop=0x8c09cd10u;r[14]=r[15];goto L8c09cd12; /* mov r15,r14 */
L8c09cd12: *stop=0x8c09cd12u;r[3]=rd(m,0x8c09ce30u,4);goto L8c09cd14; /* mov.l 0x8c09ce30,r3 */
L8c09cd14: *stop=0x8c09cd14u;r[14]+=16u;goto L8c09cd16; /* add #0x10,r14 */
L8c09cd16: target=r[3];*pr=0x8c09cd1au;*stop=0x8c09cd18u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd1a; /* jsr @r3 */
L8c09cd1a: *stop=0x8c09cd1au;if(!valid_ram(r[15]+8u,4u))return -3;r[5]=rd(m,r[15]+0x8u,4);goto L8c09cd1c; /* mov.l @(0x8,r15),r5 */
L8c09cd1c: *stop=0x8c09cd1cu;r[13]=rd(m,0x8c09ce34u,4);goto L8c09cd1e; /* mov.l 0x8c09ce34,r13 */
L8c09cd1e: target=r[13];*pr=0x8c09cd22u;*stop=0x8c09cd20u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd22; /* jsr @r13 */
L8c09cd22: *stop=0x8c09cd22u;r[5]=r[12];goto L8c09cd24; /* mov r12,r5 */
L8c09cd24: target=r[13];*pr=0x8c09cd28u;*stop=0x8c09cd26u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd28; /* jsr @r13 */
L8c09cd28: *stop=0x8c09cd28u;r[5]=rd(m,0x8c09ce38u,4);goto L8c09cd2a; /* mov.l 0x8c09ce38,r5 */
L8c09cd2a: target=r[13];*pr=0x8c09cd2eu;*stop=0x8c09cd2cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd2e; /* jsr @r13 */
L8c09cd2e: *stop=0x8c09cd2eu;if(!valid_ram(r[15]+4u,4u))return -3;r[5]=rd(m,r[15]+0x4u,4);goto L8c09cd30; /* mov.l @(0x4,r15),r5 */
L8c09cd30: *stop=0x8c09cd30u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[14],4);goto L8c09cd32; /* mov.l r14,@r15 */
L8c09cd32: target=0x8c09cc80u;*pr=0x8c09cd36u;*stop=0x8c09cd34u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd36; /* bsr 0x8c09cc80 */
L8c09cd36: *stop=0x8c09cd36u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c09cd38; /* cmp/eq #-0x1,r0 */
L8c09cd38: take=!(*sr&1u);*stop=0x8c09cd3au;r[4]=r[0];if(take)goto L8c09cd62;goto L8c09cd3c; /* bf/s 0x8c09cd62 */
L8c09cd3c: *stop=0x8c09cd3cu;r[2]=rd(m,0x8c09ce28u,4);goto L8c09cd3e; /* mov.l 0x8c09ce28,r2 */
L8c09cd3e: *stop=0x8c09cd3eu;r[5]=rd(m,0x8c09ce2cu,4);goto L8c09cd40; /* mov.l 0x8c09ce2c,r5 */
L8c09cd40: *stop=0x8c09cd40u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c09cd42; /* mov.l @r2,r3 */
L8c09cd42: *stop=0x8c09cd42u;if(!valid_ram(r[15]+12u,4u))return -3;wr(m,r[15]+0xcu,r[3],4);goto L8c09cd44; /* mov.l r3,@(0xc,r15) */
L8c09cd44: *stop=0x8c09cd44u;r[3]=rd(m,0x8c09ce30u,4);goto L8c09cd46; /* mov.l 0x8c09ce30,r3 */
L8c09cd46: target=r[3];*pr=0x8c09cd4au;*stop=0x8c09cd48u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd4a; /* jsr @r3 */
L8c09cd4a: *stop=0x8c09cd4au;if(!valid_ram(r[15]+8u,4u))return -3;r[5]=rd(m,r[15]+0x8u,4);goto L8c09cd4c; /* mov.l @(0x8,r15),r5 */
L8c09cd4c: target=r[13];*pr=0x8c09cd50u;*stop=0x8c09cd4eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd50; /* jsr @r13 */
L8c09cd50: *stop=0x8c09cd50u;if(!valid_ram(r[15]+12u,4u))return -3;r[5]=rd(m,r[15]+0xcu,4);goto L8c09cd52; /* mov.l @(0xc,r15),r5 */
L8c09cd52: target=r[13];*pr=0x8c09cd56u;*stop=0x8c09cd54u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd56; /* jsr @r13 */
L8c09cd56: *stop=0x8c09cd56u;r[5]=rd(m,0x8c09ce38u,4);goto L8c09cd58; /* mov.l 0x8c09ce38,r5 */
L8c09cd58: target=r[13];*pr=0x8c09cd5cu;*stop=0x8c09cd5au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd5c; /* jsr @r13 */
L8c09cd5c: *stop=0x8c09cd5cu;if(!valid_ram(r[15]+4u,4u))return -3;r[5]=rd(m,r[15]+0x4u,4);goto L8c09cd5e; /* mov.l @(0x4,r15),r5 */
L8c09cd5e: target=0x8c09cc80u;*pr=0x8c09cd62u;*stop=0x8c09cd60u;if(!valid_ram(r[15],4u))return -3;r[4]=rd(m,r[15],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd62; /* bsr 0x8c09cc80 */
L8c09cd62: *stop=0x8c09cd62u;r[15]+=48u;goto L8c09cd64; /* add #0x30,r15 */
L8c09cd64: *stop=0x8c09cd64u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09cd66; /* lds.l @r15+,PR */
L8c09cd66: *stop=0x8c09cd66u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c09cd68; /* mov.l @r15+,r12 */
L8c09cd68: *stop=0x8c09cd68u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09cd6a; /* mov.l @r15+,r13 */
L8c09cd6a: *stop=0x8c09cd6au;if(*pr!=entry_pr)return -3;*stop=0x8c09cd6cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_resource_regional_file_8c09cd6e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09cd6e;
L8c09cd6e: *stop=0x8c09cd6eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09cd70; /* mov.l r14,@-r15 */
L8c09cd70: *stop=0x8c09cd70u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c09cd72; /* mov.l r13,@-r15 */
L8c09cd72: *stop=0x8c09cd72u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c09cd74; /* sts.l PR,@-r15 */
L8c09cd74: *stop=0x8c09cd74u;r[15]+=4294967248u;goto L8c09cd76; /* add #-0x30,r15 */
L8c09cd76: *stop=0x8c09cd76u;r[3]=rd(m,0x8c09ce24u,4);goto L8c09cd78; /* mov.l 0x8c09ce24,r3 */
L8c09cd78: *stop=0x8c09cd78u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[4],4);goto L8c09cd7a; /* mov.l r4,@(0x8,r15) */
L8c09cd7a: target=r[3];*pr=0x8c09cd7eu;*stop=0x8c09cd7cu;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[5],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd7e; /* jsr @r3 */
L8c09cd7e: *stop=0x8c09cd7eu;r[1]=rd(m,0x8c09ce28u,4);goto L8c09cd80; /* mov.l 0x8c09ce28,r1 */
L8c09cd80: *stop=0x8c09cd80u;r[0]<<=2;goto L8c09cd82; /* shll2 r0 */
L8c09cd82: *stop=0x8c09cd82u;r[5]=rd(m,0x8c09ce3cu,4);goto L8c09cd84; /* mov.l 0x8c09ce3c,r5 */
L8c09cd84: *stop=0x8c09cd84u;r[14]=r[15];goto L8c09cd86; /* mov r15,r14 */
L8c09cd86: *stop=0x8c09cd86u;if(!valid_ram(r[0]+r[1],4u))return -3;r[2]=rd(m,r[1]+r[0],4);goto L8c09cd88; /* mov.l @(r0,r1),r2 */
L8c09cd88: *stop=0x8c09cd88u;r[14]+=16u;goto L8c09cd8a; /* add #0x10,r14 */
L8c09cd8a: *stop=0x8c09cd8au;r[3]=rd(m,0x8c09ce30u,4);goto L8c09cd8c; /* mov.l 0x8c09ce30,r3 */
L8c09cd8c: *stop=0x8c09cd8cu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);goto L8c09cd8e; /* mov.l r2,@r15 */
L8c09cd8e: target=r[3];*pr=0x8c09cd92u;*stop=0x8c09cd90u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd92; /* jsr @r3 */
L8c09cd92: *stop=0x8c09cd92u;if(!valid_ram(r[15]+8u,4u))return -3;r[5]=rd(m,r[15]+0x8u,4);goto L8c09cd94; /* mov.l @(0x8,r15),r5 */
L8c09cd94: *stop=0x8c09cd94u;r[13]=rd(m,0x8c09ce34u,4);goto L8c09cd96; /* mov.l 0x8c09ce34,r13 */
L8c09cd96: target=r[13];*pr=0x8c09cd9au;*stop=0x8c09cd98u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cd9a; /* jsr @r13 */
L8c09cd9a: *stop=0x8c09cd9au;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c09cd9c; /* mov.l @r15,r5 */
L8c09cd9c: target=r[13];*pr=0x8c09cda0u;*stop=0x8c09cd9eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cda0; /* jsr @r13 */
L8c09cda0: *stop=0x8c09cda0u;r[5]=rd(m,0x8c09ce38u,4);goto L8c09cda2; /* mov.l 0x8c09ce38,r5 */
L8c09cda2: target=r[13];*pr=0x8c09cda6u;*stop=0x8c09cda4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cda6; /* jsr @r13 */
L8c09cda6: *stop=0x8c09cda6u;if(!valid_ram(r[15]+4u,4u))return -3;r[5]=rd(m,r[15]+0x4u,4);goto L8c09cda8; /* mov.l @(0x4,r15),r5 */
L8c09cda8: *stop=0x8c09cda8u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[14],4);goto L8c09cdaa; /* mov.l r14,@r15 */
L8c09cdaa: target=0x8c09cc80u;*pr=0x8c09cdaeu;*stop=0x8c09cdacu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cdae; /* bsr 0x8c09cc80 */
L8c09cdae: *stop=0x8c09cdaeu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c09cdb0; /* cmp/eq #-0x1,r0 */
L8c09cdb0: take=!(*sr&1u);*stop=0x8c09cdb2u;r[4]=r[0];if(take)goto L8c09cdda;goto L8c09cdb4; /* bf/s 0x8c09cdda */
L8c09cdb4: *stop=0x8c09cdb4u;r[2]=rd(m,0x8c09ce28u,4);goto L8c09cdb6; /* mov.l 0x8c09ce28,r2 */
L8c09cdb6: *stop=0x8c09cdb6u;r[5]=rd(m,0x8c09ce3cu,4);goto L8c09cdb8; /* mov.l 0x8c09ce3c,r5 */
L8c09cdb8: *stop=0x8c09cdb8u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c09cdba; /* mov.l @r2,r3 */
L8c09cdba: *stop=0x8c09cdbau;if(!valid_ram(r[15]+12u,4u))return -3;wr(m,r[15]+0xcu,r[3],4);goto L8c09cdbc; /* mov.l r3,@(0xc,r15) */
L8c09cdbc: *stop=0x8c09cdbcu;r[3]=rd(m,0x8c09ce30u,4);goto L8c09cdbe; /* mov.l 0x8c09ce30,r3 */
L8c09cdbe: target=r[3];*pr=0x8c09cdc2u;*stop=0x8c09cdc0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cdc2; /* jsr @r3 */
L8c09cdc2: *stop=0x8c09cdc2u;if(!valid_ram(r[15]+8u,4u))return -3;r[5]=rd(m,r[15]+0x8u,4);goto L8c09cdc4; /* mov.l @(0x8,r15),r5 */
L8c09cdc4: target=r[13];*pr=0x8c09cdc8u;*stop=0x8c09cdc6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cdc8; /* jsr @r13 */
L8c09cdc8: *stop=0x8c09cdc8u;if(!valid_ram(r[15]+12u,4u))return -3;r[5]=rd(m,r[15]+0xcu,4);goto L8c09cdca; /* mov.l @(0xc,r15),r5 */
L8c09cdca: target=r[13];*pr=0x8c09cdceu;*stop=0x8c09cdccu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cdce; /* jsr @r13 */
L8c09cdce: *stop=0x8c09cdceu;r[5]=rd(m,0x8c09ce38u,4);goto L8c09cdd0; /* mov.l 0x8c09ce38,r5 */
L8c09cdd0: target=r[13];*pr=0x8c09cdd4u;*stop=0x8c09cdd2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cdd4; /* jsr @r13 */
L8c09cdd4: *stop=0x8c09cdd4u;if(!valid_ram(r[15]+4u,4u))return -3;r[5]=rd(m,r[15]+0x4u,4);goto L8c09cdd6; /* mov.l @(0x4,r15),r5 */
L8c09cdd6: target=0x8c09cc80u;*pr=0x8c09cddau;*stop=0x8c09cdd8u;if(!valid_ram(r[15],4u))return -3;r[4]=rd(m,r[15],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cdda; /* bsr 0x8c09cc80 */
L8c09cdda: *stop=0x8c09cddau;r[15]+=48u;goto L8c09cddc; /* add #0x30,r15 */
L8c09cddc: *stop=0x8c09cddcu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09cdde; /* lds.l @r15+,PR */
L8c09cdde: *stop=0x8c09cddeu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09cde0; /* mov.l @r15+,r13 */
L8c09cde0: *stop=0x8c09cde0u;if(*pr!=entry_pr)return -3;*stop=0x8c09cde2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_resource_prs_file_8c09da44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c09da44;
L8c09da44: *stop=0x8c09da44u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09da46; /* mov.l r14,@-r15 */
L8c09da46: *stop=0x8c09da46u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c09da48; /* sts.l PR,@-r15 */
L8c09da48: *stop=0x8c09da48u;r[15]+=4294967292u;goto L8c09da4a; /* add #-0x4,r15 */
L8c09da4a: *stop=0x8c09da4au;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto L8c09da4c; /* mov.l r5,@r15 */
L8c09da4c: *stop=0x8c09da4cu;r[5]=rd(m,0x8c09db40u,4);goto L8c09da4e; /* mov.l 0x8c09db40,r5 */
L8c09da4e: target=0x8c09cc80u;*pr=0x8c09da52u;*stop=0x8c09da50u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09da52; /* bsr 0x8c09cc80 */
L8c09da52: *stop=0x8c09da52u;r[14]=r[0];goto L8c09da54; /* mov r0,r14 */
L8c09da54: *stop=0x8c09da54u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c09da56; /* cmp/pz r14 */
L8c09da56: if(!(*sr&1u))goto L8c09da60;goto L8c09da58; /* bf 0x8c09da60 */
L8c09da58: *stop=0x8c09da58u;r[3]=rd(m,0x8c09db44u,4);goto L8c09da5a; /* mov.l 0x8c09db44,r3 */
L8c09da5a: *stop=0x8c09da5au;r[4]=rd(m,0x8c09db40u,4);goto L8c09da5c; /* mov.l 0x8c09db40,r4 */
L8c09da5c: target=r[3];*pr=0x8c09da60u;*stop=0x8c09da5eu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09da60; /* jsr @r3 */
L8c09da60: *stop=0x8c09da60u;r[2]=rd(m,0x8c09db4cu,4);goto L8c09da62; /* mov.l 0x8c09db4c,r2 */
L8c09da62: *stop=0x8c09da62u;r[3]=rd(m,0x8c09db48u,4);goto L8c09da64; /* mov.l 0x8c09db48,r3 */
L8c09da64: *stop=0x8c09da64u;r[2]|=r[3];goto L8c09da66; /* or r3,r2 */
L8c09da66: target=r[2];*pr=0x8c09da6au;*stop=0x8c09da68u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09da6a; /* jsr @r2 */
L8c09da6a: *stop=0x8c09da6au;r[0]=r[14];goto L8c09da6c; /* mov r14,r0 */
L8c09da6c: *stop=0x8c09da6cu;r[15]+=4u;goto L8c09da6e; /* add #0x4,r15 */
L8c09da6e: *stop=0x8c09da6eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09da70; /* lds.l @r15+,PR */
L8c09da70: *stop=0x8c09da70u;if(*pr!=entry_pr)return -3;*stop=0x8c09da72u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_resource_load_8c04e490(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c04e490;
L8c04e490: *stop=0x8c04e490u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c04e492; /* mov.l r14,@-r15 */
L8c04e492: *stop=0x8c04e492u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c04e494; /* mov.l r13,@-r15 */
L8c04e494: *stop=0x8c04e494u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c04e496; /* mov.l r12,@-r15 */
L8c04e496: *stop=0x8c04e496u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c04e498; /* sts.l PR,@-r15 */
L8c04e498: *stop=0x8c04e498u;r[15]+=4294967292u;goto L8c04e49a; /* add #-0x4,r15 */
L8c04e49a: *stop=0x8c04e49au;r[14]=rd(m,0x8c04e5c4u,4);goto L8c04e49c; /* mov.l 0x8c04e5c4,r14 */
L8c04e49c: *stop=0x8c04e49cu;r[2]=rd(m,0x8c04e5c8u,4);goto L8c04e49e; /* mov.l 0x8c04e5c8,r2 */
L8c04e49e: *stop=0x8c04e49eu;if(!valid_ram(r[14],2u))return -3;r[3]=sx(m,r[14],2);goto L8c04e4a0; /* mov.w @r14,r3 */
L8c04e4a0: *stop=0x8c04e4a0u;r[13]=rd(m,0x8c04e5ccu,4);goto L8c04e4a2; /* mov.l 0x8c04e5cc,r13 */
L8c04e4a2: *stop=0x8c04e4a2u;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[3],2);goto L8c04e4a4; /* mov.w r3,@r2 */
L8c04e4a4: *stop=0x8c04e4a4u;r[1]=rd(m,0x8c04e5d0u,4);goto L8c04e4a6; /* mov.l 0x8c04e5d0,r1 */
L8c04e4a6: *stop=0x8c04e4a6u;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c04e4a8; /* mov.w @r13,r3 */
L8c04e4a8: *stop=0x8c04e4a8u;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[3],2);goto L8c04e4aa; /* mov.w r3,@r1 */
L8c04e4aa: *stop=0x8c04e4aau;r[3]=rd(m,0x8c04e5d4u,4);goto L8c04e4ac; /* mov.l 0x8c04e5d4,r3 */
L8c04e4ac: target=r[3];*pr=0x8c04e4b0u;*stop=0x8c04e4aeu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e4b0; /* jsr @r3 */
L8c04e4b0: *stop=0x8c04e4b0u;r[0]=0x8c04e5d8u;goto L8c04e4b2; /* mova 0x8c04e5d8,r0 */
L8c04e4b2: *stop=0x8c04e4b2u;r[4]=rd(m,0x8c04e5e0u,4);goto L8c04e4b4; /* mov.l 0x8c04e5e0,r4 */
L8c04e4b4: *stop=0x8c04e4b4u;r[3]=rd(m,0x8c04e5e4u,4);goto L8c04e4b6; /* mov.l 0x8c04e5e4,r3 */
L8c04e4b6: *stop=0x8c04e4b6u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto L8c04e4b8; /* fmov.s @r0,fr5 */
L8c04e4b8: *stop=0x8c04e4b8u;r[0]=0x8c04e5dcu;goto L8c04e4ba; /* mova 0x8c04e5dc,r0 */
L8c04e4ba: target=r[3];*pr=0x8c04e4beu;*stop=0x8c04e4bcu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e4be; /* jsr @r3 */
L8c04e4be: *stop=0x8c04e4beu;r[2]=rd(m,0x8c04e5e8u,4);goto L8c04e4c0; /* mov.l 0x8c04e5e8,r2 */
L8c04e4c0: *stop=0x8c04e4c0u;r[4]=rd(m,0x8c04e5e0u,4);goto L8c04e4c2; /* mov.l 0x8c04e5e0,r4 */
L8c04e4c2: target=r[2];*pr=0x8c04e4c6u;*stop=0x8c04e4c4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e4c6; /* jsr @r2 */
L8c04e4c6: *stop=0x8c04e4c6u;r[3]=rd(m,0x8c04e5ecu,4);goto L8c04e4c8; /* mov.l 0x8c04e5ec,r3 */
L8c04e4c8: target=r[3];*pr=0x8c04e4ccu;*stop=0x8c04e4cau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e4cc; /* jsr @r3 */
L8c04e4cc: *stop=0x8c04e4ccu;r[2]=rd(m,0x8c04e5f0u,4);goto L8c04e4ce; /* mov.l 0x8c04e5f0,r2 */
L8c04e4ce: target=r[2];*pr=0x8c04e4d2u;*stop=0x8c04e4d0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e4d2; /* jsr @r2 */
L8c04e4d2: *stop=0x8c04e4d2u;r[3]=rd(m,0x8c04e5f4u,4);goto L8c04e4d4; /* mov.l 0x8c04e5f4,r3 */
L8c04e4d4: target=r[3];*pr=0x8c04e4d8u;*stop=0x8c04e4d6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e4d8; /* jsr @r3 */
L8c04e4d8: *stop=0x8c04e4d8u;if(!valid_ram(r[14],2u))return -3;r[2]=sx(m,r[14],2);goto L8c04e4da; /* mov.w @r14,r2 */
L8c04e4da: *stop=0x8c04e4dau;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c04e4dc; /* mov.w @r13,r3 */
L8c04e4dc: *stop=0x8c04e4dcu;r[2]<<=8;goto L8c04e4de; /* shll8 r2 */
L8c04e4de: *stop=0x8c04e4deu;r[12]=rd(m,0x8c04e5fcu,4);goto L8c04e4e0; /* mov.l 0x8c04e5fc,r12 */
L8c04e4e0: *stop=0x8c04e4e0u;r[2]|=r[3];goto L8c04e4e2; /* or r3,r2 */
L8c04e4e2: *stop=0x8c04e4e2u;r[3]=rd(m,0x8c04e5f8u,4);goto L8c04e4e4; /* mov.l 0x8c04e5f8,r3 */
L8c04e4e4: *stop=0x8c04e4e4u;if(!valid_ram(r[15],2u))return -3;wr(m,r[15],r[2],2);goto L8c04e4e6; /* mov.w r2,@r15 */
L8c04e4e6: *stop=0x8c04e4e6u;r[2]=4294967288u;goto L8c04e4e8; /* mov #-0x8,r2 */
L8c04e4e8: *stop=0x8c04e4e8u;if(!valid_ram(r[15],2u))return -3;r[4]=sx(m,r[15],2);goto L8c04e4ea; /* mov.w @r15,r4 */
L8c04e4ea: *stop=0x8c04e4eau;r[4]=r[4]&65535u;goto L8c04e4ec; /* extu.w r4,r4 */
L8c04e4ec: *stop=0x8c04e4ecu;r[4]&=r[3];goto L8c04e4ee; /* and r3,r4 */
L8c04e4ee: *stop=0x8c04e4eeu;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[4]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[4]=n?(uint32_t)((int32_t)r[4]>>n):((r[4]&0x80000000u)?0xffffffffu:0u);}}goto L8c04e4f0; /* shad r2,r4 */
L8c04e4f0: *stop=0x8c04e4f0u;r[0]=r[4]&65535u;goto L8c04e4f2; /* extu.w r4,r0 */
L8c04e4f2: *stop=0x8c04e4f2u;*sr=(*sr&~1u)|(r[0]==0x00000024u);goto L8c04e4f4; /* cmp/eq #0x24,r0 */
L8c04e4f4: if((*sr&1u))goto L8c04e4fe;goto L8c04e4f6; /* bt 0x8c04e4fe */
L8c04e4f6: *stop=0x8c04e4f6u;*sr=(*sr&~1u)|(r[0]==0x00000025u);goto L8c04e4f8; /* cmp/eq #0x25,r0 */
L8c04e4f8: if((*sr&1u))goto L8c04e4fe;goto L8c04e4fa; /* bt 0x8c04e4fe */
L8c04e4fa: *stop=0x8c04e4fcu;goto L8c04e508; /* bra 0x8c04e508 */
L8c04e4fe: *stop=0x8c04e4feu;r[1]=rd(m,0x8c04e600u,4);goto L8c04e500; /* mov.l 0x8c04e600,r1 */
L8c04e500: target=r[1];*pr=0x8c04e504u;*stop=0x8c04e502u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e504; /* jsr @r1 */
L8c04e504: *stop=0x8c04e506u;r[4]=2u;goto L8c04e524; /* bra 0x8c04e524 */
L8c04e508: *stop=0x8c04e508u;r[1]=rd(m,0x8c04e600u,4);goto L8c04e50a; /* mov.l 0x8c04e600,r1 */
L8c04e50a: target=r[1];*pr=0x8c04e50eu;*stop=0x8c04e50cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e50e; /* jsr @r1 */
L8c04e50e: *stop=0x8c04e50eu;r[3]=rd(m,0x8c04e604u,4);goto L8c04e510; /* mov.l 0x8c04e604,r3 */
L8c04e510: target=r[3];*pr=0x8c04e514u;*stop=0x8c04e512u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e514; /* jsr @r3 */
L8c04e514: *stop=0x8c04e514u;r[2]=rd(m,0x8c04e608u,4);goto L8c04e516; /* mov.l 0x8c04e608,r2 */
L8c04e516: target=r[2];*pr=0x8c04e51au;*stop=0x8c04e518u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e51a; /* jsr @r2 */
L8c04e51a: *stop=0x8c04e51au;r[2]=rd(m,0x8c04e60cu,4);goto L8c04e51c; /* mov.l 0x8c04e60c,r2 */
L8c04e51c: *stop=0x8c04e51cu;if(!valid_ram(r[12],2u))return -3;r[4]=sx(m,r[12],2);goto L8c04e51e; /* mov.w @r12,r4 */
L8c04e51e: target=r[2];*pr=0x8c04e522u;*stop=0x8c04e520u;r[4]=r[4]&65535u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e522; /* jsr @r2 */
L8c04e522: *stop=0x8c04e522u;r[4]=0u;goto L8c04e524; /* mov #0x0,r4 */
L8c04e524: *stop=0x8c04e524u;r[3]=rd(m,0x8c04e610u,4);goto L8c04e526; /* mov.l 0x8c04e610,r3 */
L8c04e526: target=r[3];*pr=0x8c04e52au;*stop=0x8c04e528u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e52a; /* jsr @r3 */
L8c04e52a: *stop=0x8c04e52au;r[3]=rd(m,0x8c04e614u,4);goto L8c04e52c; /* mov.l 0x8c04e614,r3 */
L8c04e52c: target=r[3];*pr=0x8c04e530u;*stop=0x8c04e52eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e530; /* jsr @r3 */
L8c04e530: *stop=0x8c04e530u;if(!valid_ram(r[14],2u))return -3;r[4]=sx(m,r[14],2);goto L8c04e532; /* mov.w @r14,r4 */
L8c04e532: *stop=0x8c04e532u;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c04e534; /* mov.w @r13,r3 */
L8c04e534: *stop=0x8c04e534u;r[2]=rd(m,0x8c04e618u,4);goto L8c04e536; /* mov.l 0x8c04e618,r2 */
L8c04e536: *stop=0x8c04e536u;r[4]<<=8;goto L8c04e538; /* shll8 r4 */
L8c04e538: target=r[2];*pr=0x8c04e53cu;*stop=0x8c04e53au;r[4]|=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e53c; /* jsr @r2 */
L8c04e53c: *stop=0x8c04e53cu;r[3]=rd(m,0x8c04e61cu,4);goto L8c04e53e; /* mov.l 0x8c04e61c,r3 */
L8c04e53e: *stop=0x8c04e53eu;if(!valid_ram(r[13],2u))return -3;r[5]=sx(m,r[13],2);goto L8c04e540; /* mov.w @r13,r5 */
L8c04e540: target=r[3];*pr=0x8c04e544u;*stop=0x8c04e542u;if(!valid_ram(r[14],2u))return -3;r[4]=sx(m,r[14],2);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e544; /* jsr @r3 */
L8c04e544: *stop=0x8c04e544u;r[4]=rd(m,0x8c04e620u,4);goto L8c04e546; /* mov.l 0x8c04e620,r4 */
L8c04e546: *stop=0x8c04e546u;r[5]=8u;goto L8c04e548; /* mov #0x8,r5 */
L8c04e548: *stop=0x8c04e548u;r[2]=rd(m,0x8c04e624u,4);goto L8c04e54a; /* mov.l 0x8c04e624,r2 */
L8c04e54a: *stop=0x8c04e54au;r[5]<<=8;goto L8c04e54c; /* shll8 r5 */
L8c04e54c: target=r[2];*pr=0x8c04e550u;*stop=0x8c04e54eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e550; /* jsr @r2 */
L8c04e550: target=0x8c04e3aau;*pr=0x8c04e554u;*stop=0x8c04e552u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e554; /* bsr 0x8c04e3aa */
L8c04e554: *stop=0x8c04e554u;r[1]=rd(m,0x8c04e62cu,4);goto L8c04e556; /* mov.l 0x8c04e62c,r1 */
L8c04e556: *stop=0x8c04e556u;r[2]=0u;goto L8c04e558; /* mov #0x0,r2 */
L8c04e558: *stop=0x8c04e558u;r[3]=rd(m,0x8c04e628u,4);goto L8c04e55a; /* mov.l 0x8c04e628,r3 */
L8c04e55a: target=r[1];*pr=0x8c04e55eu;*stop=0x8c04e55cu;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[2],2);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e55e; /* jsr @r1 */
L8c04e55e: *stop=0x8c04e55eu;r[3]=rd(m,0x8c04e630u,4);goto L8c04e560; /* mov.l 0x8c04e630,r3 */
L8c04e560: target=r[3];*pr=0x8c04e564u;*stop=0x8c04e562u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e564; /* jsr @r3 */
L8c04e564: *stop=0x8c04e564u;if(!valid_ram(r[12],2u))return -3;r[0]=sx(m,r[12],2);goto L8c04e566; /* mov.w @r12,r0 */
L8c04e566: *stop=0x8c04e566u;r[0]=r[0]&65535u;goto L8c04e568; /* extu.w r0,r0 */
L8c04e568: *stop=0x8c04e568u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c04e56a; /* cmp/eq #0x3,r0 */
L8c04e56a: if(!(*sr&1u))goto L8c04e576;goto L8c04e56c; /* bf 0x8c04e576 */
L8c04e56c: *stop=0x8c04e56cu;r[2]=rd(m,0x8c04e634u,4);goto L8c04e56e; /* mov.l 0x8c04e634,r2 */
L8c04e56e: target=r[2];*pr=0x8c04e572u;*stop=0x8c04e570u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e572; /* jsr @r2 */
L8c04e572: *stop=0x8c04e574u;goto L8c04e580; /* bra 0x8c04e580 */
L8c04e576: *stop=0x8c04e576u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c04e578; /* cmp/eq #0x7,r0 */
L8c04e578: if(!(*sr&1u))goto L8c04e580;goto L8c04e57a; /* bf 0x8c04e580 */
L8c04e57a: *stop=0x8c04e57au;r[2]=rd(m,0x8c04e638u,4);goto L8c04e57c; /* mov.l 0x8c04e638,r2 */
L8c04e57c: target=r[2];*pr=0x8c04e580u;*stop=0x8c04e57eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e580; /* jsr @r2 */
L8c04e580: *stop=0x8c04e580u;r[3]=rd(m,0x8c04e63cu,4);goto L8c04e582; /* mov.l 0x8c04e63c,r3 */
L8c04e582: target=r[3];*pr=0x8c04e586u;*stop=0x8c04e584u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e586; /* jsr @r3 */
L8c04e586: *stop=0x8c04e586u;r[2]=rd(m,0x8c04e640u,4);goto L8c04e588; /* mov.l 0x8c04e640,r2 */
L8c04e588: target=r[2];*pr=0x8c04e58cu;*stop=0x8c04e58au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e58c; /* jsr @r2 */
L8c04e58c: *stop=0x8c04e58cu;r[6]=rd(m,0x8c04e644u,4);goto L8c04e58e; /* mov.l 0x8c04e644,r6 */
L8c04e58e: *stop=0x8c04e58eu;r[4]=2u;goto L8c04e590; /* mov #0x2,r4 */
L8c04e590: *stop=0x8c04e590u;r[5]=8u;goto L8c04e592; /* mov #0x8,r5 */
L8c04e592: *stop=0x8c04e592u;r[15]+=4u;goto L8c04e594; /* add #0x4,r15 */
L8c04e594: *stop=0x8c04e594u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c04e596; /* lds.l @r15+,PR */
L8c04e596: *stop=0x8c04e596u;r[3]=rd(m,0x8c04e648u,4);goto L8c04e598; /* mov.l 0x8c04e648,r3 */
L8c04e598: *stop=0x8c04e598u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c04e59a; /* mov.l @r15+,r12 */
L8c04e59a: *stop=0x8c04e59au;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c04e59c; /* mov.l @r15+,r13 */
L8c04e59c: target=r[3];*stop=0x8c04e59eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_stage_resource_string_copy_8c10d3dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d3dc;
L8c10d3dc: *stop=0x8c10d3dcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c10d3de; /* sts.l PR,@-r15 */
L8c10d3de: *stop=0x8c10d3deu;r[3]=rd(m,0x8c10d3ecu,4);goto L8c10d3e0; /* mov.l 0x8c10d3ec,r3 */
L8c10d3e0: *stop=0x8c10d3e0u;r[1]=r[5];goto L8c10d3e2; /* mov r5,r1 */
L8c10d3e2: target=r[3];*pr=0x8c10d3e6u;*stop=0x8c10d3e4u;r[0]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d3e6; /* jsr @r3 */
L8c10d3e6: *stop=0x8c10d3e6u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c10d3e8; /* lds.l @r15+,PR */
L8c10d3e8: *stop=0x8c10d3e8u;if(*pr!=entry_pr)return -3;*stop=0x8c10d3eau;return 0; /* rts */
}
