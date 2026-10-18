/* Object-effect profiles, dispatcher and rendering services; names inferred.
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
SA_STAGE_EXPORT int sa_object_effect_dispatch_0c909d80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc909d80;
Lc909d80: *stop=0xc909d80u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc909d82; /* mov.l r14,@-r15 */
Lc909d82: *stop=0xc909d82u;r[14]=r[5];goto Lc909d84; /* mov r5,r14 */
Lc909d84: *stop=0xc909d84u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto Lc909d86; /* mov.l r13,@-r15 */
Lc909d86: *stop=0xc909d86u;r[13]=8u;goto Lc909d88; /* mov #0x8,r13 */
Lc909d88: *stop=0xc909d88u;r[13]<<=8;goto Lc909d8a; /* shll8 r13 */
Lc909d8a: *stop=0xc909d8au;r[13]+=16u;goto Lc909d8c; /* add #0x10,r13 */
Lc909d8c: *stop=0xc909d8cu;r[13]+=r[14];goto Lc909d8e; /* add r14,r13 */
Lc909d8e: *stop=0xc909d8eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc909d90; /* sts.l PR,@-r15 */
Lc909d90: *stop=0xc909d90u;if(!valid_ram(r[13]+2u,2u))return -3;r[0]=sx(m,r[13]+0x2u,2);goto Lc909d92; /* mov.w @(0x2,r13),r0 */
Lc909d92: *stop=0xc909d92u;r[0]=r[0]&65535u;goto Lc909d94; /* extu.w r0,r0 */
Lc909d94: *stop=0xc909d94u;r[1]=10u;goto Lc909d96; /* mov #0xa,r1 */
Lc909d96: *stop=0xc909d96u;*sr=(*sr&~1u)|(r[0]>=r[1]);goto Lc909d98; /* cmp/hs r1,r0 */
Lc909d98: if((*sr&1u))goto Lc909de8;goto Lc909d9a; /* bt 0x0c909de8 */
Lc909d9a: *stop=0xc909d9au;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc909d9c; /* shll r0 */
Lc909d9c: *stop=0xc909d9cu;r[1]=r[0];goto Lc909d9e; /* mov r0,r1 */
Lc909d9e: *stop=0xc909d9eu;r[0]=0x0c909da8u;goto Lc909da0; /* mova 0x0c909da8,r0 */
Lc909da0: *stop=0xc909da0u;if(!valid_ram(r[0]+r[1],2u))return -3;r[0]=sx(m,r[0]+r[1],2);goto Lc909da2; /* mov.w @(r0,r1),r0 */
Lc909da2: target=0xc909da6u+r[0];*stop=0xc909da4u;switch(target){case 0xc909dbcu:goto Lc909dbc;case 0xc909dc4u:goto Lc909dc4;case 0xc909dccu:goto Lc909dcc;case 0xc909dd4u:goto Lc909dd4;case 0xc909ddcu:goto Lc909ddc;case 0xc909de4u:goto Lc909de4;case 0xc909de8u:goto Lc909de8;default:*stop=0xc909da2u;return -3;} /* braf r0 */
Lc909dbc: target=0x0c908f3eu;*pr=0x0c909dc0u;*stop=0xc909dbeu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc909dc0; /* bsr 0x0c908f3e */
Lc909dc0: *stop=0xc909dc2u;goto Lc909de8; /* bra 0x0c909de8 */
Lc909dc4: target=0x0c909054u;*pr=0x0c909dc8u;*stop=0xc909dc6u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc909dc8; /* bsr 0x0c909054 */
Lc909dc8: *stop=0xc909dcau;goto Lc909de8; /* bra 0x0c909de8 */
Lc909dcc: target=0x0c9092dau;*pr=0x0c909dd0u;*stop=0xc909dceu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc909dd0; /* bsr 0x0c9092da */
Lc909dd0: *stop=0xc909dd2u;goto Lc909de8; /* bra 0x0c909de8 */
Lc909dd4: target=0x0c909560u;*pr=0x0c909dd8u;*stop=0xc909dd6u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc909dd8; /* bsr 0x0c909560 */
Lc909dd8: *stop=0xc909ddau;goto Lc909de8; /* bra 0x0c909de8 */
Lc909ddc: target=0x0c9097e4u;*pr=0x0c909de0u;*stop=0xc909ddeu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc909de0; /* bsr 0x0c9097e4 */
Lc909de0: *stop=0xc909de2u;goto Lc909de8; /* bra 0x0c909de8 */
Lc909de4: target=0x0c909aa0u;*pr=0x0c909de8u;*stop=0xc909de6u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc909de8; /* bsr 0x0c909aa0 */
Lc909de8: *stop=0xc909de8u;r[0]=4294967276u;goto Lc909dea; /* mov #-0x14,r0 */
Lc909dea: *stop=0xc909deau;r[0]=r[0]&255u;goto Lc909dec; /* extu.b r0,r0 */
Lc909dec: *stop=0xc909decu;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc909dee; /* mov.l @(r0,r14),r0 */
Lc909dee: *stop=0xc909deeu;if(!valid_ram(r[0],1u))return -3;r[0]=sx(m,r[0],1);goto Lc909df0; /* mov.b @r0,r0 */
Lc909df0: *stop=0xc909df0u;r[0]=r[0]&255u;goto Lc909df2; /* extu.b r0,r0 */
Lc909df2: *stop=0xc909df2u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto Lc909df4; /* cmp/eq #0x6,r0 */
Lc909df4: if((*sr&1u))goto Lc909e02;goto Lc909df6; /* bt 0x0c909e02 */
Lc909df6: *stop=0xc909df6u;r[2]=rd(m,0x0c909e80u,4);goto Lc909df8; /* mov.l 0x0c909e80,r2 */
Lc909df8: *stop=0xc909df8u;r[5]=r[13];goto Lc909dfa; /* mov r13,r5 */
Lc909dfa: target=r[2];*pr=0x0c909dfeu;*stop=0xc909dfcu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc909dfe; /* jsr @r2 */
Lc909dfe: *stop=0xc909e00u;goto Lc909e0a; /* bra 0x0c909e0a */
Lc909e02: *stop=0xc909e02u;r[2]=rd(m,0x0c909e84u,4);goto Lc909e04; /* mov.l 0x0c909e84,r2 */
Lc909e04: *stop=0xc909e04u;r[5]=r[13];goto Lc909e06; /* mov r13,r5 */
Lc909e06: target=r[2];*pr=0x0c909e0au;*stop=0xc909e08u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc909e0a; /* jsr @r2 */
Lc909e0a: *stop=0xc909e0au;r[0]=52u;goto Lc909e0c; /* mov #0x34,r0 */
Lc909e0c: *stop=0xc909e0cu;if(!valid_ram(r[0]+r[13],2u))return -3;r[3]=sx(m,r[0]+r[13],2);goto Lc909e0e; /* mov.w @(r0,r13),r3 */
Lc909e0e: *stop=0xc909e0eu;r[3]+=1u;goto Lc909e10; /* add #0x1,r3 */
Lc909e10: *stop=0xc909e10u;if(!valid_ram(r[0]+r[13],2u))return -3;wr(m,r[0]+r[13],r[3],2);goto Lc909e12; /* mov.w r3,@(r0,r13) */
Lc909e12: *stop=0xc909e12u;r[0]=116u;goto Lc909e14; /* mov #0x74,r0 */
Lc909e14: *stop=0xc909e14u;if(!valid_ram(r[0]+r[13],2u))return -3;r[2]=sx(m,r[0]+r[13],2);goto Lc909e16; /* mov.w @(r0,r13),r2 */
Lc909e16: *stop=0xc909e16u;r[2]+=1u;goto Lc909e18; /* add #0x1,r2 */
Lc909e18: *stop=0xc909e18u;if(!valid_ram(r[0]+r[13],2u))return -3;wr(m,r[0]+r[13],r[2],2);goto Lc909e1a; /* mov.w r2,@(r0,r13) */
Lc909e1a: *stop=0xc909e1au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc909e1c; /* lds.l @r15+,PR */
Lc909e1c: *stop=0xc909e1cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto Lc909e1e; /* mov.l @r15+,r13 */
Lc909e1e: *stop=0xc909e1eu;if(*pr!=entry_pr)return -3;*stop=0xc909e20u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_profile_zero_0c908f3e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc908f3e;
Lc908f3e: *stop=0xc908f3eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc908f40; /* mov.l r14,@-r15 */
Lc908f40: *stop=0xc908f40u;r[14]=8u;goto Lc908f42; /* mov #0x8,r14 */
Lc908f42: *stop=0xc908f42u;r[14]<<=8;goto Lc908f44; /* shll8 r14 */
Lc908f44: *stop=0xc908f44u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto Lc908f46; /* fmov.s fr15,@-r15 */
Lc908f46: *stop=0xc908f46u;r[14]+=16u;goto Lc908f48; /* add #0x10,r14 */
Lc908f48: *stop=0xc908f48u;r[14]+=r[5];goto Lc908f4a; /* add r5,r14 */
Lc908f4a: *stop=0xc908f4au;r[0]=120u;goto Lc908f4c; /* mov #0x78,r0 */
Lc908f4c: *stop=0xc908f4cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc908f4e; /* sts.l PR,@-r15 */
Lc908f4e: *stop=0xc908f4eu;if(!valid_ram(r[0]+r[14],2u))return -3;r[0]=sx(m,r[0]+r[14],2);goto Lc908f50; /* mov.w @(r0,r14),r0 */
Lc908f50: *stop=0xc908f50u;r[0]=r[0]&65535u;goto Lc908f52; /* extu.w r0,r0 */
Lc908f52: *stop=0xc908f52u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc908f54; /* cmp/eq #0x0,r0 */
Lc908f54: take=(*sr&1u);*stop=0xc908f56u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0x3f800000u;if(take)goto Lc908f60;goto Lc908f58; /* bt/s 0x0c908f60 */
Lc908f58: *stop=0xc908f58u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc908f5a; /* cmp/eq #0x1,r0 */
Lc908f5a: if((*sr&1u))goto Lc909020;goto Lc908f5c; /* bt 0x0c909020 */
Lc908f5c: *stop=0xc908f5eu;goto Lc909042; /* bra 0x0c909042 */
Lc908f60: *stop=0xc908f60u;r[0]=54u;goto Lc908f62; /* mov #0x36,r0 */
Lc908f62: *stop=0xc908f62u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto Lc908f64; /* fldi0 fr4 */
Lc908f64: *stop=0xc908f64u;r[4]=0u;goto Lc908f66; /* mov #0x0,r4 */
Lc908f66: *stop=0xc908f66u;if(!valid_ram(r[0]+r[14],2u))return -3;wr(m,r[0]+r[14],r[4],2);goto Lc908f68; /* mov.w r4,@(r0,r14) */
Lc908f68: *stop=0xc908f68u;r[0]=68u;goto Lc908f6a; /* mov #0x44,r0 */
Lc908f6a: *stop=0xc908f6au;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[14]);goto Lc908f6c; /* fmov fr4,@(r0,r14) */
Lc908f6c: *stop=0xc908f6cu;r[0]=0x0c909008u;goto Lc908f6e; /* mova 0x0c909008,r0 */
Lc908f6e: *stop=0xc908f6eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc908f70; /* fmov.s @r0,fr3 */
Lc908f70: *stop=0xc908f70u;r[0]=72u;goto Lc908f72; /* mov #0x48,r0 */
Lc908f72: *stop=0xc908f72u;r[3]=1u;goto Lc908f74; /* mov #0x1,r3 */
Lc908f74: *stop=0xc908f74u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto Lc908f76; /* fmov fr3,@(r0,r14) */
Lc908f76: *stop=0xc908f76u;r[0]=76u;goto Lc908f78; /* mov #0x4c,r0 */
Lc908f78: *stop=0xc908f78u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[14]);goto Lc908f7a; /* fmov fr4,@(r0,r14) */
Lc908f7a: *stop=0xc908f7au;r[0]=0x0c90900cu;goto Lc908f7c; /* mova 0x0c90900c,r0 */
Lc908f7c: *stop=0xc908f7cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc908f7e; /* fmov.s @r0,fr5 */
Lc908f7e: *stop=0xc908f7eu;r[0]=92u;goto Lc908f80; /* mov #0x5c,r0 */
Lc908f80: *stop=0xc908f80u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[14]);goto Lc908f82; /* fmov fr5,@(r0,r14) */
Lc908f82: *stop=0xc908f82u;r[0]=96u;goto Lc908f84; /* mov #0x60,r0 */
Lc908f84: *stop=0xc908f84u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[14]);goto Lc908f86; /* fmov fr5,@(r0,r14) */
Lc908f86: *stop=0xc908f86u;r[0]=118u;goto Lc908f88; /* mov #0x76,r0 */
Lc908f88: *stop=0xc908f88u;if(!valid_ram(r[0]+r[14],2u))return -3;wr(m,r[0]+r[14],r[3],2);goto Lc908f8a; /* mov.w r3,@(r0,r14) */
Lc908f8a: *stop=0xc908f8au;r[0]+=14u;goto Lc908f8c; /* add #0xe,r0 */
Lc908f8c: *stop=0xc908f8cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[14]);goto Lc908f8e; /* fmov fr4,@(r0,r14) */
Lc908f8e: *stop=0xc908f8eu;r[0]+=4u;goto Lc908f90; /* add #0x4,r0 */
Lc908f90: *stop=0xc908f90u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[14]);goto Lc908f92; /* fmov fr4,@(r0,r14) */
Lc908f92: *stop=0xc908f92u;r[0]+=4u;goto Lc908f94; /* add #0x4,r0 */
Lc908f94: *stop=0xc908f94u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[14]);goto Lc908f96; /* fmov fr4,@(r0,r14) */
Lc908f96: *stop=0xc908f96u;r[0]+=16u;goto Lc908f98; /* add #0x10,r0 */
Lc908f98: *stop=0xc908f98u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[14]);goto Lc908f9a; /* fmov fr15,@(r0,r14) */
Lc908f9a: *stop=0xc908f9au;r[0]+=4u;goto Lc908f9c; /* add #0x4,r0 */
Lc908f9c: *stop=0xc908f9cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[14]);goto Lc908f9e; /* fmov fr15,@(r0,r14) */
Lc908f9e: *stop=0xc908f9eu;if(!valid_ram(r[14]+48u,4u))return -3;wr(m,r[14]+0x30u,r[4],4);goto Lc908fa0; /* mov.l r4,@(0x30,r14) */
Lc908fa0: *stop=0xc908fa0u;r[0]=120u;goto Lc908fa2; /* mov #0x78,r0 */
Lc908fa2: *stop=0xc908fa2u;if(!valid_ram(r[0]+r[14],2u))return -3;r[3]=sx(m,r[0]+r[14],2);goto Lc908fa4; /* mov.w @(r0,r14),r3 */
Lc908fa4: *stop=0xc908fa4u;r[3]+=1u;goto Lc908fa6; /* add #0x1,r3 */
Lc908fa6: *stop=0xc908fa8u;if(!valid_ram(r[0]+r[14],2u))return -3;wr(m,r[0]+r[14],r[3],2);goto Lc909042; /* bra 0x0c909042 */
Lc909020: *stop=0xc909020u;r[3]=rd(m,0x0c9091a0u,4);goto Lc909022; /* mov.l 0x0c9091a0,r3 */
Lc909022: target=r[3];*pr=0x0c909026u;*stop=0xc909024u;if(!valid_ram(r[14]+48u,4u))return -3;r[4]=rd(m,r[14]+0x30u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc909026; /* jsr @r3 */
Lc909026: *stop=0xc909026u;r[0]=0x0c9091a4u;goto Lc909028; /* mova 0x0c9091a4,r0 */
Lc909028: *stop=0xc909028u;fcopy(f,3u,0u);goto Lc90902a; /* fmov fr0,fr3 */
Lc90902a: *stop=0xc90902au;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]);goto Lc90902c; /* fmov.s @r0,fr0 */
Lc90902c: *stop=0xc90902cu;r[0]=4294967196u;goto Lc90902e; /* mov #-0x64,r0 */
Lc90902e: *stop=0xc90902eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto Lc909030; /* fldi1 fr2 */
Lc909030: *stop=0xc909030u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[2],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc909032; /* fadd fr2,fr2 */
Lc909032: *stop=0xc909032u;r[0]=r[0]&255u;goto Lc909034; /* extu.b r0,r0 */
Lc909034: *stop=0xc909034u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[15],&f->fpscr,&fr[15]);if(status)return status;}goto Lc909036; /* fmac fr0,fr3,fr15 */
Lc909036: *stop=0xc909036u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[14]);goto Lc909038; /* fmov fr15,@(r0,r14) */
Lc909038: *stop=0xc909038u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto Lc90903a; /* fmov.s @(r0,r14),fr3 */
Lc90903a: *stop=0xc90903au;r[0]+=4u;goto Lc90903c; /* add #0x4,r0 */
Lc90903c: *stop=0xc90903cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto Lc90903e; /* fneg fr3 */
Lc90903e: *stop=0xc90903eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc909040; /* fadd fr2,fr3 */
Lc909040: *stop=0xc909040u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto Lc909042; /* fmov fr3,@(r0,r14) */
Lc909042: *stop=0xc909042u;if(!valid_ram(r[14]+48u,4u))return -3;r[2]=rd(m,r[14]+0x30u,4);goto Lc909044; /* mov.l @(0x30,r14),r2 */
Lc909044: *stop=0xc909044u;r[3]=12u;goto Lc909046; /* mov #0xc,r3 */
Lc909046: *stop=0xc909046u;r[3]<<=8;goto Lc909048; /* shll8 r3 */
Lc909048: *stop=0xc909048u;r[2]+=r[3];goto Lc90904a; /* add r3,r2 */
Lc90904a: *stop=0xc90904au;if(!valid_ram(r[14]+48u,4u))return -3;wr(m,r[14]+0x30u,r[2],4);goto Lc90904c; /* mov.l r2,@(0x30,r14) */
Lc90904c: *stop=0xc90904cu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc90904e; /* lds.l @r15+,PR */
Lc90904e: *stop=0xc90904eu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc909050; /* fmov.s @r15+,fr15 */
Lc909050: *stop=0xc909050u;if(*pr!=entry_pr)return -3;*stop=0xc909052u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_profile_one_two_three_0c909054(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc909054;
Lc909054: *stop=0xc909054u;r[0]=0x0c9091a8u;goto Lc909056; /* mova 0x0c9091a8,r0 */
Lc909056: *stop=0xc909056u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]);goto Lc909058; /* fmov.s @r0,fr7 */
Lc909058: *stop=0xc909058u;r[0]=0x0c9091acu;goto Lc90905a; /* mova 0x0c9091ac,r0 */
Lc90905a: *stop=0xc90905au;r[4]=8u;goto Lc90905c; /* mov #0x8,r4 */
Lc90905c: *stop=0xc90905cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]);goto Lc90905e; /* fmov.s @r0,fr6 */
Lc90905e: *stop=0xc90905eu;r[0]=0x0c9091b0u;goto Lc909060; /* mova 0x0c9091b0,r0 */
Lc909060: *stop=0xc909060u;r[4]<<=8;goto Lc909062; /* shll8 r4 */
Lc909062: *stop=0xc909062u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc909064; /* fmov.s @r0,fr5 */
Lc909064: *stop=0xc909064u;r[4]+=16u;goto Lc909066; /* add #0x10,r4 */
Lc909066: *stop=0xc909066u;r[0]=56u;goto Lc909068; /* mov #0x38,r0 */
Lc909068: *stop=0xc909068u;r[4]+=r[5];goto Lc90906a; /* add r5,r4 */
Lc90906a: *stop=0xc90906au;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto Lc90906c; /* mov.w @(r0,r4),r0 */
Lc90906c: *stop=0xc90906cu;r[0]=r[0]&65535u;goto Lc90906e; /* extu.w r0,r0 */
Lc90906e: *stop=0xc90906eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc909070; /* cmp/eq #0x0,r0 */
Lc909070: take=(*sr&1u);*stop=0xc909072u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;if(take)goto Lc909088;goto Lc909074; /* bt/s 0x0c909088 */
Lc909074: *stop=0xc909074u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc909076; /* cmp/eq #0x1,r0 */
Lc909076: if((*sr&1u))goto Lc9090c0;goto Lc909078; /* bt 0x0c9090c0 */
Lc909078: *stop=0xc909078u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto Lc90907a; /* cmp/eq #0x2,r0 */
Lc90907a: if((*sr&1u))goto Lc9090da;goto Lc90907c; /* bt 0x0c9090da */
Lc90907c: *stop=0xc90907cu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto Lc90907e; /* cmp/eq #0x3,r0 */
Lc90907e: if(!(*sr&1u))goto Lc909084;goto Lc909080; /* bf 0x0c909084 */
Lc909080: *stop=0xc909082u;goto Lc9091e0; /* bra 0x0c9091e0 */
Lc909084: *stop=0xc909086u;goto Lc909226; /* bra 0x0c909226 */
Lc909088: *stop=0xc909088u;r[0]=54u;goto Lc90908a; /* mov #0x36,r0 */
Lc90908a: *stop=0xc90908au;r[3]=2u;goto Lc90908c; /* mov #0x2,r3 */
Lc90908c: *stop=0xc90908cu;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc90908e; /* mov.w r3,@(r0,r4) */
Lc90908e: *stop=0xc90908eu;r[0]=68u;goto Lc909090; /* mov #0x44,r0 */
Lc909090: *stop=0xc909090u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909092; /* fmov fr4,@(r0,r4) */
Lc909092: *stop=0xc909092u;r[0]=72u;goto Lc909094; /* mov #0x48,r0 */
Lc909094: *stop=0xc909094u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909096; /* fmov fr4,@(r0,r4) */
Lc909096: *stop=0xc909096u;r[0]=0x0c9091b4u;goto Lc909098; /* mova 0x0c9091b4,r0 */
Lc909098: *stop=0xc909098u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc90909a; /* fmov.s @r0,fr3 */
Lc90909a: *stop=0xc90909au;r[0]=76u;goto Lc90909c; /* mov #0x4c,r0 */
Lc90909c: *stop=0xc90909cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc90909e; /* fmov fr3,@(r0,r4) */
Lc90909e: *stop=0xc90909eu;r[0]=84u;goto Lc9090a0; /* mov #0x54,r0 */
Lc9090a0: *stop=0xc9090a0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9090a2; /* fmov fr4,@(r0,r4) */
Lc9090a2: *stop=0xc9090a2u;r[0]=92u;goto Lc9090a4; /* mov #0x5c,r0 */
Lc9090a4: *stop=0xc9090a4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,7u,r[0]+r[4]);goto Lc9090a6; /* fmov fr7,@(r0,r4) */
Lc9090a6: *stop=0xc9090a6u;r[0]=0x0c9091b8u;goto Lc9090a8; /* mova 0x0c9091b8,r0 */
Lc9090a8: *stop=0xc9090a8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc9090aa; /* fmov.s @r0,fr3 */
Lc9090aa: *stop=0xc9090aau;r[0]=96u;goto Lc9090ac; /* mov #0x60,r0 */
Lc9090ac: *stop=0xc9090acu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc9090ae; /* fmov fr3,@(r0,r4) */
Lc9090ae: *stop=0xc9090aeu;r[0]=104u;goto Lc9090b0; /* mov #0x68,r0 */
Lc9090b0: *stop=0xc9090b0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9090b2; /* fmov fr4,@(r0,r4) */
Lc9090b2: *stop=0xc9090b2u;r[0]=108u;goto Lc9090b4; /* mov #0x6c,r0 */
Lc9090b4: *stop=0xc9090b4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9090b6; /* fmov fr4,@(r0,r4) */
Lc9090b6: *stop=0xc9090b6u;r[0]=56u;goto Lc9090b8; /* mov #0x38,r0 */
Lc9090b8: *stop=0xc9090b8u;if(!valid_ram(r[0]+r[4],2u))return -3;r[3]=sx(m,r[0]+r[4],2);goto Lc9090ba; /* mov.w @(r0,r4),r3 */
Lc9090ba: *stop=0xc9090bau;r[3]+=1u;goto Lc9090bc; /* add #0x1,r3 */
Lc9090bc: *stop=0xc9090beu;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc909226; /* bra 0x0c909226 */
Lc9090c0: *stop=0xc9090c0u;r[0]=52u;goto Lc9090c2; /* mov #0x34,r0 */
Lc9090c2: *stop=0xc9090c2u;if(!valid_ram(r[0]+r[4],2u))return -3;r[1]=sx(m,r[0]+r[4],2);goto Lc9090c4; /* mov.w @(r0,r4),r1 */
Lc9090c4: *stop=0xc9090c4u;r[3]=10u;goto Lc9090c6; /* mov #0xa,r3 */
Lc9090c6: *stop=0xc9090c6u;r[1]=r[1]&65535u;goto Lc9090c8; /* extu.w r1,r1 */
Lc9090c8: *stop=0xc9090c8u;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[3]);goto Lc9090ca; /* cmp/gt r3,r1 */
Lc9090ca: if((*sr&1u))goto Lc9090d0;goto Lc9090cc; /* bt 0x0c9090d0 */
Lc9090cc: *stop=0xc9090ceu;goto Lc909226; /* bra 0x0c909226 */
Lc9090d0: *stop=0xc9090d0u;r[0]=56u;goto Lc9090d2; /* mov #0x38,r0 */
Lc9090d2: *stop=0xc9090d2u;if(!valid_ram(r[0]+r[4],2u))return -3;r[1]=sx(m,r[0]+r[4],2);goto Lc9090d4; /* mov.w @(r0,r4),r1 */
Lc9090d4: *stop=0xc9090d4u;r[1]+=1u;goto Lc9090d6; /* add #0x1,r1 */
Lc9090d6: *stop=0xc9090d8u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[1],2);goto Lc909226; /* bra 0x0c909226 */
Lc9090da: *stop=0xc9090dau;r[0]=0x0c9091bcu;goto Lc9090dc; /* mova 0x0c9091bc,r0 */
Lc9090dc: *stop=0xc9090dcu;r[1]=rd(m,0x0c9091c0u,4);goto Lc9090de; /* mov.l 0x0c9091c0,r1 */
Lc9090de: *stop=0xc9090deu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[0]);goto Lc9090e0; /* fmov.s @r0,fr9 */
Lc9090e0: *stop=0xc9090e0u;r[0]=92u;goto Lc9090e2; /* mov #0x5c,r0 */
Lc9090e2: *stop=0xc9090e2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto Lc9090e4; /* fmov.s @(r0,r4),fr8 */
Lc9090e4: *stop=0xc9090e4u;r[0]=104u;goto Lc9090e6; /* mov #0x68,r0 */
Lc9090e6: *stop=0xc9090e6u;*fpul=r[1];goto Lc9090e8; /* lds r1,FPUL */
Lc9090e8: *stop=0xc9090e8u;r[1]=r[0];goto Lc9090ea; /* mov r0,r1 */
Lc9090ea: *stop=0xc9090eau;r[1]+=r[4];goto Lc9090ec; /* add r4,r1 */
Lc9090ec: *stop=0xc9090ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[9],&f->fpscr,&fr[8],0);if(status)return status;}goto Lc9090ee; /* fadd fr9,fr8 */
Lc9090ee: *stop=0xc9090eeu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc9090f0; /* fmov.s @r1,fr2 */
Lc9090f0: *stop=0xc9090f0u;r[1]=104u;goto Lc9090f2; /* mov #0x68,r1 */
Lc9090f2: *stop=0xc9090f2u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc9090f4; /* fsts FPUL,fr3 */
Lc9090f4: *stop=0xc9090f4u;r[1]+=r[4];goto Lc9090f6; /* add r4,r1 */
Lc9090f6: *stop=0xc9090f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9090f8; /* fmul fr3,fr2 */
Lc9090f8: *stop=0xc9090f8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9090fa; /* fmov.s @(r0,r4),fr1 */
Lc9090fa: *stop=0xc9090fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[6],&f->fpscr,&fr[8]);if(status)return status;}goto Lc9090fc; /* fmul fr6,fr8 */
Lc9090fc: *stop=0xc9090fcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc9090fe; /* fneg fr2 */
Lc9090fe: *stop=0xc9090feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[8],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc909100; /* fsub fr8,fr2 */
Lc909100: *stop=0xc909100u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc909102; /* fadd fr2,fr1 */
Lc909102: *stop=0xc909102u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc909104; /* fmov fr1,@(r0,r4) */
Lc909104: *stop=0xc909104u;r[0]=92u;goto Lc909106; /* mov #0x5c,r0 */
Lc909106: *stop=0xc909106u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc909108; /* fmov.s @(r0,r4),fr1 */
Lc909108: *stop=0xc909108u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc90910a; /* fmov.s @r1,fr2 */
Lc90910a: *stop=0xc90910au;r[1]=rd(m,0x0c9091c4u,4);goto Lc90910c; /* mov.l 0x0c9091c4,r1 */
Lc90910c: *stop=0xc90910cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc90910e; /* fadd fr2,fr1 */
Lc90910e: *stop=0xc90910eu;*fpul=r[1];goto Lc909110; /* lds r1,FPUL */
Lc909110: *stop=0xc909110u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc909112; /* fmov fr1,@(r0,r4) */
Lc909112: *stop=0xc909112u;r[0]=96u;goto Lc909114; /* mov #0x60,r0 */
Lc909114: *stop=0xc909114u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto Lc909116; /* fmov.s @(r0,r4),fr8 */
Lc909116: *stop=0xc909116u;r[0]=108u;goto Lc909118; /* mov #0x6c,r0 */
Lc909118: *stop=0xc909118u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[9],&f->fpscr,&fr[8],0);if(status)return status;}goto Lc90911a; /* fadd fr9,fr8 */
Lc90911a: *stop=0xc90911au;r[1]=r[0];goto Lc90911c; /* mov r0,r1 */
Lc90911c: *stop=0xc90911cu;r[1]+=r[4];goto Lc90911e; /* add r4,r1 */
Lc90911e: *stop=0xc90911eu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc909120; /* fmov.s @r1,fr1 */
Lc909120: *stop=0xc909120u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[6],&f->fpscr,&fr[8]);if(status)return status;}goto Lc909122; /* fmul fr6,fr8 */
Lc909122: *stop=0xc909122u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto Lc909124; /* fsts FPUL,fr2 */
Lc909124: *stop=0xc909124u;r[1]=108u;goto Lc909126; /* mov #0x6c,r1 */
Lc909126: *stop=0xc909126u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc909128; /* fmul fr2,fr1 */
Lc909128: *stop=0xc909128u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc90912a; /* fmov.s @(r0,r4),fr0 */
Lc90912a: *stop=0xc90912au;r[1]+=r[4];goto Lc90912c; /* add r4,r1 */
Lc90912c: *stop=0xc90912cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc90912e; /* fneg fr1 */
Lc90912e: *stop=0xc90912eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[8],&f->fpscr,&fr[1],1);if(status)return status;}goto Lc909130; /* fsub fr8,fr1 */
Lc909130: *stop=0xc909130u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909132; /* fadd fr1,fr0 */
Lc909132: *stop=0xc909132u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909134; /* fmov fr0,@(r0,r4) */
Lc909134: *stop=0xc909134u;r[0]=96u;goto Lc909136; /* mov #0x60,r0 */
Lc909136: *stop=0xc909136u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909138; /* fmov.s @(r0,r4),fr0 */
Lc909138: *stop=0xc909138u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc90913a; /* fmov.s @r1,fr1 */
Lc90913a: *stop=0xc90913au;r[1]=rd(m,0x0c9091ccu,4);goto Lc90913c; /* mov.l 0x0c9091cc,r1 */
Lc90913c: *stop=0xc90913cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc90913e; /* fadd fr1,fr0 */
Lc90913e: *stop=0xc90913eu;*fpul=r[1];goto Lc909140; /* lds r1,FPUL */
Lc909140: *stop=0xc909140u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909142; /* fmov fr0,@(r0,r4) */
Lc909142: *stop=0xc909142u;r[0]=0x0c9091c8u;goto Lc909144; /* mova 0x0c9091c8,r0 */
Lc909144: *stop=0xc909144u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto Lc909146; /* fmov.s @r0,fr1 */
Lc909146: *stop=0xc909146u;r[0]=72u;goto Lc909148; /* mov #0x48,r0 */
Lc909148: *stop=0xc909148u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto Lc90914a; /* fmov.s @(r0,r4),fr8 */
Lc90914a: *stop=0xc90914au;r[0]=84u;goto Lc90914c; /* mov #0x54,r0 */
Lc90914c: *stop=0xc90914cu;r[1]=r[0];goto Lc90914e; /* mov r0,r1 */
Lc90914e: *stop=0xc90914eu;r[1]+=r[4];goto Lc909150; /* add r4,r1 */
Lc909150: *stop=0xc909150u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[1]);goto Lc909152; /* fmov.s @r1,fr0 */
Lc909152: *stop=0xc909152u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[1],&f->fpscr,&fr[8],0);if(status)return status;}goto Lc909154; /* fadd fr1,fr8 */
Lc909154: *stop=0xc909154u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc909156; /* fsts FPUL,fr3 */
Lc909156: *stop=0xc909156u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto Lc909158; /* fmul fr3,fr0 */
Lc909158: *stop=0xc909158u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto Lc90915a; /* fneg fr0 */
Lc90915a: *stop=0xc90915au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto Lc90915c; /* fmul fr5,fr8 */
Lc90915c: *stop=0xc90915cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc90915e; /* fmov.s @(r0,r4),fr2 */
Lc90915e: *stop=0xc90915eu;r[1]=84u;goto Lc909160; /* mov #0x54,r1 */
Lc909160: *stop=0xc909160u;r[1]+=r[4];goto Lc909162; /* add r4,r1 */
Lc909162: *stop=0xc909162u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[8],&f->fpscr,&fr[0],1);if(status)return status;}goto Lc909164; /* fsub fr8,fr0 */
Lc909164: *stop=0xc909164u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[0],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc909166; /* fadd fr0,fr2 */
Lc909166: *stop=0xc909166u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc909168; /* fmov fr2,@(r0,r4) */
Lc909168: *stop=0xc909168u;r[0]=72u;goto Lc90916a; /* mov #0x48,r0 */
Lc90916a: *stop=0xc90916au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc90916c; /* fmov.s @(r0,r4),fr0 */
Lc90916c: *stop=0xc90916cu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc90916e; /* fmov.s @r1,fr2 */
Lc90916e: *stop=0xc90916eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[2],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909170; /* fadd fr2,fr0 */
Lc909170: *stop=0xc909170u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909172; /* fmov fr0,@(r0,r4) */
Lc909172: *stop=0xc909172u;if(!valid_ram(r[4]+4u,2u))return -3;r[0]=sx(m,r[4]+0x4u,2);goto Lc909174; /* mov.w @(0x4,r4),r0 */
Lc909174: *stop=0xc909174u;r[0]+=4294967295u;goto Lc909176; /* add #-0x1,r0 */
Lc909176: *stop=0xc909176u;if(!valid_ram(r[4]+4u,2u))return -3;wr(m,r[4]+0x4u,r[0],2);goto Lc909178; /* mov.w r0,@(0x4,r4) */
Lc909178: *stop=0xc909178u;r[0]+=1u;goto Lc90917a; /* add #0x1,r0 */
Lc90917a: *stop=0xc90917au;r[0]=r[0]&65535u;goto Lc90917c; /* extu.w r0,r0 */
Lc90917c: *stop=0xc90917cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc90917e; /* tst r0,r0 */
Lc90917e: if(!(*sr&1u))goto Lc909226;goto Lc909180; /* bf 0x0c909226 */
Lc909180: *stop=0xc909180u;r[0]=56u;goto Lc909182; /* mov #0x38,r0 */
Lc909182: *stop=0xc909182u;if(!valid_ram(r[0]+r[4],2u))return -3;r[2]=sx(m,r[0]+r[4],2);goto Lc909184; /* mov.w @(r0,r4),r2 */
Lc909184: *stop=0xc909184u;r[2]+=1u;goto Lc909186; /* add #0x1,r2 */
Lc909186: *stop=0xc909188u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[2],2);goto Lc909226; /* bra 0x0c909226 */
Lc9091e0: *stop=0xc9091e0u;r[0]=92u;goto Lc9091e2; /* mov #0x5c,r0 */
Lc9091e2: *stop=0xc9091e2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc9091e4; /* fldi0 fr2 */
Lc9091e4: *stop=0xc9091e4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc9091e6; /* fmov.s @(r0,r4),fr3 */
Lc9091e6: *stop=0xc9091e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc9091e8; /* fsub fr5,fr3 */
Lc9091e8: *stop=0xc9091e8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc9091ea; /* fcmp/gt fr3,fr2 */
Lc9091ea: take=!(*sr&1u);*stop=0xc9091ecu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc9091f0;goto Lc9091ee; /* bf/s 0x0c9091f0 */
Lc9091ee: *stop=0xc9091eeu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9091f0; /* fmov fr4,@(r0,r4) */
Lc9091f0: *stop=0xc9091f0u;r[0]=96u;goto Lc9091f2; /* mov #0x60,r0 */
Lc9091f2: *stop=0xc9091f2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc9091f4; /* fldi0 fr2 */
Lc9091f4: *stop=0xc9091f4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc9091f6; /* fmov.s @(r0,r4),fr3 */
Lc9091f6: *stop=0xc9091f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc9091f8; /* fsub fr5,fr3 */
Lc9091f8: *stop=0xc9091f8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc9091fa; /* fcmp/gt fr3,fr2 */
Lc9091fa: take=!(*sr&1u);*stop=0xc9091fcu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc909200;goto Lc9091fe; /* bf/s 0x0c909200 */
Lc9091fe: *stop=0xc9091feu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909200; /* fmov fr4,@(r0,r4) */
Lc909200: *stop=0xc909200u;r[0]=72u;goto Lc909202; /* mov #0x48,r0 */
Lc909202: *stop=0xc909202u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc909204; /* fldi0 fr2 */
Lc909204: *stop=0xc909204u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc909206; /* fmov.s @(r0,r4),fr3 */
Lc909206: *stop=0xc909206u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc909208; /* fsub fr5,fr3 */
Lc909208: *stop=0xc909208u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc90920a; /* fcmp/gt fr3,fr2 */
Lc90920a: take=!(*sr&1u);*stop=0xc90920cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc909226;goto Lc90920e; /* bf/s 0x0c909226 */
Lc90920e: *stop=0xc90920eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909210; /* fmov fr4,@(r0,r4) */
Lc909210: *stop=0xc909210u;r[0]=8u;goto Lc909212; /* mov #0x8,r0 */
Lc909212: *stop=0xc909212u;r[0]<<=8;goto Lc909214; /* shll8 r0 */
Lc909214: *stop=0xc909214u;r[0]+=18u;goto Lc909216; /* add #0x12,r0 */
Lc909216: *stop=0xc909216u;r[6]=0u;goto Lc909218; /* mov #0x0,r6 */
Lc909218: *stop=0xc909218u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc90921a; /* mov.w r6,@(r0,r5) */
Lc90921a: *stop=0xc90921au;r[0]+=54u;goto Lc90921c; /* add #0x36,r0 */
Lc90921c: *stop=0xc90921cu;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc90921e; /* mov.w r6,@(r0,r5) */
Lc90921e: *stop=0xc90921eu;r[0]+=64u;goto Lc909220; /* add #0x40,r0 */
Lc909220: *stop=0xc909220u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909222; /* mov.w r6,@(r0,r5) */
Lc909222: *stop=0xc909222u;r[0]+=4294967180u;goto Lc909224; /* add #-0x74,r0 */
Lc909224: *stop=0xc909224u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909226; /* mov.w r6,@(r0,r5) */
Lc909226: *stop=0xc909226u;r[0]=120u;goto Lc909228; /* mov #0x78,r0 */
Lc909228: *stop=0xc909228u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto Lc90922a; /* fldi1 fr5 */
Lc90922a: *stop=0xc90922au;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto Lc90922c; /* mov.w @(r0,r4),r0 */
Lc90922c: *stop=0xc90922cu;r[0]=r[0]&65535u;goto Lc90922e; /* extu.w r0,r0 */
Lc90922e: *stop=0xc90922eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc909230; /* cmp/eq #0x0,r0 */
Lc909230: take=(*sr&1u);*stop=0xc909232u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[5],&f->fpscr,&fr[5],0);if(status)return status;}if(take)goto Lc909240;goto Lc909234; /* bt/s 0x0c909240 */
Lc909234: *stop=0xc909234u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc909236; /* cmp/eq #0x1,r0 */
Lc909236: if((*sr&1u))goto Lc909266;goto Lc909238; /* bt 0x0c909266 */
Lc909238: *stop=0xc909238u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto Lc90923a; /* cmp/eq #0x2,r0 */
Lc90923a: if((*sr&1u))goto Lc909292;goto Lc90923c; /* bt 0x0c909292 */
Lc90923c: *stop=0xc90923cu;if(*pr!=entry_pr)return -3;*stop=0xc90923eu;return 0; /* rts */
Lc909240: *stop=0xc909240u;r[0]=118u;goto Lc909242; /* mov #0x76,r0 */
Lc909242: *stop=0xc909242u;r[3]=1u;goto Lc909244; /* mov #0x1,r3 */
Lc909244: *stop=0xc909244u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc909246; /* mov.w r3,@(r0,r4) */
Lc909246: *stop=0xc909246u;r[0]+=14u;goto Lc909248; /* add #0xe,r0 */
Lc909248: *stop=0xc909248u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc90924a; /* fmov fr4,@(r0,r4) */
Lc90924a: *stop=0xc90924au;r[0]+=4u;goto Lc90924c; /* add #0x4,r0 */
Lc90924c: *stop=0xc90924cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc90924e; /* fmov fr4,@(r0,r4) */
Lc90924e: *stop=0xc90924eu;r[0]+=4u;goto Lc909250; /* add #0x4,r0 */
Lc909250: *stop=0xc909250u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909252; /* fmov fr4,@(r0,r4) */
Lc909252: *stop=0xc909252u;r[0]+=16u;goto Lc909254; /* add #0x10,r0 */
Lc909254: *stop=0xc909254u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto Lc909256; /* fldi1 fr4 */
Lc909256: *stop=0xc909256u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909258; /* fmov fr4,@(r0,r4) */
Lc909258: *stop=0xc909258u;r[0]+=4u;goto Lc90925a; /* add #0x4,r0 */
Lc90925a: *stop=0xc90925au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc90925c; /* fmov fr4,@(r0,r4) */
Lc90925c: *stop=0xc90925cu;r[0]=120u;goto Lc90925e; /* mov #0x78,r0 */
Lc90925e: *stop=0xc90925eu;if(!valid_ram(r[0]+r[4],2u))return -3;r[3]=sx(m,r[0]+r[4],2);goto Lc909260; /* mov.w @(r0,r4),r3 */
Lc909260: *stop=0xc909260u;r[3]+=1u;goto Lc909262; /* add #0x1,r3 */
Lc909262: *stop=0xc909262u;if(*pr!=entry_pr)return -3;*stop=0xc909264u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);return 0; /* rts */
Lc909266: *stop=0xc909266u;r[1]=rd(m,0x0c909360u,4);goto Lc909268; /* mov.l 0x0c909360,r1 */
Lc909268: *stop=0xc909268u;r[0]=4294967196u;goto Lc90926a; /* mov #-0x64,r0 */
Lc90926a: *stop=0xc90926au;r[0]=r[0]&255u;goto Lc90926c; /* extu.b r0,r0 */
Lc90926c: *stop=0xc90926cu;*fpul=r[1];goto Lc90926e; /* lds r1,FPUL */
Lc90926e: *stop=0xc90926eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc909270; /* fmov.s @(r0,r4),fr2 */
Lc909270: *stop=0xc909270u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc909272; /* fsts FPUL,fr3 */
Lc909272: *stop=0xc909272u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc909274; /* fsub fr3,fr2 */
Lc909274: *stop=0xc909274u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc909276; /* fmov fr2,@(r0,r4) */
Lc909276: *stop=0xc909276u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc909278; /* fmov.s @(r0,r4),fr2 */
Lc909278: *stop=0xc909278u;r[0]+=4u;goto Lc90927a; /* add #0x4,r0 */
Lc90927a: *stop=0xc90927au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc90927c; /* fneg fr2 */
Lc90927c: *stop=0xc90927cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc90927e; /* fadd fr5,fr2 */
Lc90927e: *stop=0xc90927eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc909280; /* fmov fr2,@(r0,r4) */
Lc909280: *stop=0xc909280u;r[0]+=4294967292u;goto Lc909282; /* add #-0x4,r0 */
Lc909282: *stop=0xc909282u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc909284; /* fmov.s @(r0,r4),fr2 */
Lc909284: *stop=0xc909284u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto Lc909286; /* fcmp/gt fr2,fr7 */
Lc909286: if(!(*sr&1u))goto Lc9092d6;goto Lc909288; /* bf 0x0c9092d6 */
Lc909288: *stop=0xc909288u;r[0]=120u;goto Lc90928a; /* mov #0x78,r0 */
Lc90928a: *stop=0xc90928au;if(!valid_ram(r[0]+r[4],2u))return -3;r[2]=sx(m,r[0]+r[4],2);goto Lc90928c; /* mov.w @(r0,r4),r2 */
Lc90928c: *stop=0xc90928cu;r[2]+=1u;goto Lc90928e; /* add #0x1,r2 */
Lc90928e: *stop=0xc90928eu;if(*pr!=entry_pr)return -3;*stop=0xc909290u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[2],2);return 0; /* rts */
Lc909292: *stop=0xc909292u;r[1]=rd(m,0x0c909364u,4);goto Lc909294; /* mov.l 0x0c909364,r1 */
Lc909294: *stop=0xc909294u;r[0]=4294967196u;goto Lc909296; /* mov #-0x64,r0 */
Lc909296: *stop=0xc909296u;r[0]=r[0]&255u;goto Lc909298; /* extu.b r0,r0 */
Lc909298: *stop=0xc909298u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto Lc90929a; /* fldi1 fr3 */
Lc90929a: *stop=0xc90929au;*fpul=r[1];goto Lc90929c; /* lds r1,FPUL */
Lc90929c: *stop=0xc90929cu;r[1]=4294967208u;goto Lc90929e; /* mov #-0x58,r1 */
Lc90929e: *stop=0xc90929eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[4]);goto Lc9092a0; /* fmov.s @(r0,r4),fr4 */
Lc9092a0: *stop=0xc9092a0u;r[1]=r[1]&255u;goto Lc9092a2; /* extu.b r1,r1 */
Lc9092a2: *stop=0xc9092a2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto Lc9092a4; /* fneg fr3 */
Lc9092a4: *stop=0xc9092a4u;r[1]+=r[4];goto Lc9092a6; /* add r4,r1 */
Lc9092a6: *stop=0xc9092a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto Lc9092a8; /* fadd fr3,fr4 */
Lc9092a8: *stop=0xc9092a8u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc9092aa; /* fmov.s @r1,fr1 */
Lc9092aa: *stop=0xc9092aau;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto Lc9092ac; /* fsts FPUL,fr2 */
Lc9092ac: *stop=0xc9092acu;r[0]+=12u;goto Lc9092ae; /* add #0xc,r0 */
Lc9092ae: *stop=0xc9092aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc9092b0; /* fmul fr2,fr1 */
Lc9092b0: *stop=0xc9092b0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc9092b2; /* fmov.s @(r0,r4),fr0 */
Lc9092b2: *stop=0xc9092b2u;r[1]=4294967208u;goto Lc9092b4; /* mov #-0x58,r1 */
Lc9092b4: *stop=0xc9092b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[6],&f->fpscr,&fr[4]);if(status)return status;}goto Lc9092b6; /* fmul fr6,fr4 */
Lc9092b6: *stop=0xc9092b6u;r[1]=r[1]&255u;goto Lc9092b8; /* extu.b r1,r1 */
Lc9092b8: *stop=0xc9092b8u;r[1]+=r[4];goto Lc9092ba; /* add r4,r1 */
Lc9092ba: *stop=0xc9092bau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc9092bc; /* fneg fr1 */
Lc9092bc: *stop=0xc9092bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],1);if(status)return status;}goto Lc9092be; /* fsub fr4,fr1 */
Lc9092be: *stop=0xc9092beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc9092c0; /* fadd fr1,fr0 */
Lc9092c0: *stop=0xc9092c0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc9092c2; /* fmov fr0,@(r0,r4) */
Lc9092c2: *stop=0xc9092c2u;r[0]+=4294967284u;goto Lc9092c4; /* add #-0xc,r0 */
Lc9092c4: *stop=0xc9092c4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc9092c6; /* fmov.s @(r0,r4),fr0 */
Lc9092c6: *stop=0xc9092c6u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc9092c8; /* fmov.s @r1,fr1 */
Lc9092c8: *stop=0xc9092c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc9092ca; /* fadd fr1,fr0 */
Lc9092ca: *stop=0xc9092cau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc9092cc; /* fmov fr0,@(r0,r4) */
Lc9092cc: *stop=0xc9092ccu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9092ce; /* fmov.s @(r0,r4),fr1 */
Lc9092ce: *stop=0xc9092ceu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc9092d0; /* fneg fr1 */
Lc9092d0: *stop=0xc9092d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[5],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc9092d2; /* fadd fr5,fr1 */
Lc9092d2: *stop=0xc9092d2u;r[0]+=4u;goto Lc9092d4; /* add #0x4,r0 */
Lc9092d4: *stop=0xc9092d4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc9092d6; /* fmov fr1,@(r0,r4) */
Lc9092d6: *stop=0xc9092d6u;if(*pr!=entry_pr)return -3;*stop=0xc9092d8u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_profile_four_0c9092da(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc9092da;
Lc9092da: *stop=0xc9092dau;r[0]=0x0c909368u;goto Lc9092dc; /* mova 0x0c909368,r0 */
Lc9092dc: *stop=0xc9092dcu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]);goto Lc9092de; /* fmov.s @r0,fr7 */
Lc9092de: *stop=0xc9092deu;r[0]=0x0c90936cu;goto Lc9092e0; /* mova 0x0c90936c,r0 */
Lc9092e0: *stop=0xc9092e0u;r[4]=8u;goto Lc9092e2; /* mov #0x8,r4 */
Lc9092e2: *stop=0xc9092e2u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]);goto Lc9092e4; /* fmov.s @r0,fr6 */
Lc9092e4: *stop=0xc9092e4u;r[0]=0x0c909370u;goto Lc9092e6; /* mova 0x0c909370,r0 */
Lc9092e6: *stop=0xc9092e6u;r[4]<<=8;goto Lc9092e8; /* shll8 r4 */
Lc9092e8: *stop=0xc9092e8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc9092ea; /* fmov.s @r0,fr5 */
Lc9092ea: *stop=0xc9092eau;r[4]+=16u;goto Lc9092ec; /* add #0x10,r4 */
Lc9092ec: *stop=0xc9092ecu;r[0]=56u;goto Lc9092ee; /* mov #0x38,r0 */
Lc9092ee: *stop=0xc9092eeu;r[4]+=r[5];goto Lc9092f0; /* add r5,r4 */
Lc9092f0: *stop=0xc9092f0u;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto Lc9092f2; /* mov.w @(r0,r4),r0 */
Lc9092f2: *stop=0xc9092f2u;r[0]=r[0]&65535u;goto Lc9092f4; /* extu.w r0,r0 */
Lc9092f4: *stop=0xc9092f4u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc9092f6; /* cmp/eq #0x0,r0 */
Lc9092f6: take=(*sr&1u);*stop=0xc9092f8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;if(take)goto Lc90930e;goto Lc9092fa; /* bt/s 0x0c90930e */
Lc9092fa: *stop=0xc9092fau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc9092fc; /* cmp/eq #0x1,r0 */
Lc9092fc: if((*sr&1u))goto Lc909346;goto Lc9092fe; /* bt 0x0c909346 */
Lc9092fe: *stop=0xc9092feu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto Lc909300; /* cmp/eq #0x2,r0 */
Lc909300: if((*sr&1u))goto Lc909380;goto Lc909302; /* bt 0x0c909380 */
Lc909302: *stop=0xc909302u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto Lc909304; /* cmp/eq #0x3,r0 */
Lc909304: if(!(*sr&1u))goto Lc90930a;goto Lc909306; /* bf 0x0c90930a */
Lc909306: *stop=0xc909308u;goto Lc90942e; /* bra 0x0c90942e */
Lc90930a: *stop=0xc90930cu;goto Lc90947c; /* bra 0x0c90947c */
Lc90930e: *stop=0xc90930eu;r[0]=54u;goto Lc909310; /* mov #0x36,r0 */
Lc909310: *stop=0xc909310u;r[3]=3u;goto Lc909312; /* mov #0x3,r3 */
Lc909312: *stop=0xc909312u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc909314; /* mov.w r3,@(r0,r4) */
Lc909314: *stop=0xc909314u;r[0]=68u;goto Lc909316; /* mov #0x44,r0 */
Lc909316: *stop=0xc909316u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909318; /* fmov fr4,@(r0,r4) */
Lc909318: *stop=0xc909318u;r[0]=72u;goto Lc90931a; /* mov #0x48,r0 */
Lc90931a: *stop=0xc90931au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc90931c; /* fmov fr4,@(r0,r4) */
Lc90931c: *stop=0xc90931cu;r[0]=0x0c909374u;goto Lc90931e; /* mova 0x0c909374,r0 */
Lc90931e: *stop=0xc90931eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc909320; /* fmov.s @r0,fr3 */
Lc909320: *stop=0xc909320u;r[0]=76u;goto Lc909322; /* mov #0x4c,r0 */
Lc909322: *stop=0xc909322u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc909324; /* fmov fr3,@(r0,r4) */
Lc909324: *stop=0xc909324u;r[0]=84u;goto Lc909326; /* mov #0x54,r0 */
Lc909326: *stop=0xc909326u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909328; /* fmov fr4,@(r0,r4) */
Lc909328: *stop=0xc909328u;r[0]=92u;goto Lc90932a; /* mov #0x5c,r0 */
Lc90932a: *stop=0xc90932au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[4]);goto Lc90932c; /* fmov fr6,@(r0,r4) */
Lc90932c: *stop=0xc90932cu;r[0]=0x0c909378u;goto Lc90932e; /* mova 0x0c909378,r0 */
Lc90932e: *stop=0xc90932eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc909330; /* fmov.s @r0,fr3 */
Lc909330: *stop=0xc909330u;r[0]=96u;goto Lc909332; /* mov #0x60,r0 */
Lc909332: *stop=0xc909332u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc909334; /* fmov fr3,@(r0,r4) */
Lc909334: *stop=0xc909334u;r[0]=104u;goto Lc909336; /* mov #0x68,r0 */
Lc909336: *stop=0xc909336u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909338; /* fmov fr4,@(r0,r4) */
Lc909338: *stop=0xc909338u;r[0]=108u;goto Lc90933a; /* mov #0x6c,r0 */
Lc90933a: *stop=0xc90933au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc90933c; /* fmov fr4,@(r0,r4) */
Lc90933c: *stop=0xc90933cu;r[0]=56u;goto Lc90933e; /* mov #0x38,r0 */
Lc90933e: *stop=0xc90933eu;if(!valid_ram(r[0]+r[4],2u))return -3;r[3]=sx(m,r[0]+r[4],2);goto Lc909340; /* mov.w @(r0,r4),r3 */
Lc909340: *stop=0xc909340u;r[3]+=1u;goto Lc909342; /* add #0x1,r3 */
Lc909342: *stop=0xc909344u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc90947c; /* bra 0x0c90947c */
Lc909346: *stop=0xc909346u;r[0]=52u;goto Lc909348; /* mov #0x34,r0 */
Lc909348: *stop=0xc909348u;if(!valid_ram(r[0]+r[4],2u))return -3;r[1]=sx(m,r[0]+r[4],2);goto Lc90934a; /* mov.w @(r0,r4),r1 */
Lc90934a: *stop=0xc90934au;r[3]=10u;goto Lc90934c; /* mov #0xa,r3 */
Lc90934c: *stop=0xc90934cu;r[1]=r[1]&65535u;goto Lc90934e; /* extu.w r1,r1 */
Lc90934e: *stop=0xc90934eu;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[3]);goto Lc909350; /* cmp/gt r3,r1 */
Lc909350: if((*sr&1u))goto Lc909356;goto Lc909352; /* bt 0x0c909356 */
Lc909352: *stop=0xc909354u;goto Lc90947c; /* bra 0x0c90947c */
Lc909356: *stop=0xc909356u;r[0]=56u;goto Lc909358; /* mov #0x38,r0 */
Lc909358: *stop=0xc909358u;if(!valid_ram(r[0]+r[4],2u))return -3;r[1]=sx(m,r[0]+r[4],2);goto Lc90935a; /* mov.w @(r0,r4),r1 */
Lc90935a: *stop=0xc90935au;r[1]+=1u;goto Lc90935c; /* add #0x1,r1 */
Lc90935c: *stop=0xc90935eu;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[1],2);goto Lc90947c; /* bra 0x0c90947c */
Lc909380: *stop=0xc909380u;r[0]=0x0c909540u;goto Lc909382; /* mova 0x0c909540,r0 */
Lc909382: *stop=0xc909382u;r[1]=rd(m,0x0c909548u,4);goto Lc909384; /* mov.l 0x0c909548,r1 */
Lc909384: *stop=0xc909384u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,10u,r[0]);goto Lc909386; /* fmov.s @r0,fr10 */
Lc909386: *stop=0xc909386u;r[0]=92u;goto Lc909388; /* mov #0x5c,r0 */
Lc909388: *stop=0xc909388u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[0]+r[4]);goto Lc90938a; /* fmov.s @(r0,r4),fr9 */
Lc90938a: *stop=0xc90938au;r[0]=0x0c909544u;goto Lc90938c; /* mova 0x0c909544,r0 */
Lc90938c: *stop=0xc90938cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]);goto Lc90938e; /* fmov.s @r0,fr8 */
Lc90938e: *stop=0xc90938eu;r[0]=104u;goto Lc909390; /* mov #0x68,r0 */
Lc909390: *stop=0xc909390u;*fpul=r[1];goto Lc909392; /* lds r1,FPUL */
Lc909392: *stop=0xc909392u;r[1]=r[0];goto Lc909394; /* mov r0,r1 */
Lc909394: *stop=0xc909394u;r[1]+=r[4];goto Lc909396; /* add r4,r1 */
Lc909396: *stop=0xc909396u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[10],&f->fpscr,&fr[9],0);if(status)return status;}goto Lc909398; /* fadd fr10,fr9 */
Lc909398: *stop=0xc909398u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc90939a; /* fmov.s @r1,fr2 */
Lc90939a: *stop=0xc90939au;r[1]=104u;goto Lc90939c; /* mov #0x68,r1 */
Lc90939c: *stop=0xc90939cu;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc90939e; /* fsts FPUL,fr3 */
Lc90939e: *stop=0xc90939eu;r[1]+=r[4];goto Lc9093a0; /* add r4,r1 */
Lc9093a0: *stop=0xc9093a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9093a2; /* fmul fr3,fr2 */
Lc9093a2: *stop=0xc9093a2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9093a4; /* fmov.s @(r0,r4),fr1 */
Lc9093a4: *stop=0xc9093a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[8],&f->fpscr,&fr[9]);if(status)return status;}goto Lc9093a6; /* fmul fr8,fr9 */
Lc9093a6: *stop=0xc9093a6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc9093a8; /* fneg fr2 */
Lc9093a8: *stop=0xc9093a8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[9],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc9093aa; /* fsub fr9,fr2 */
Lc9093aa: *stop=0xc9093aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc9093ac; /* fadd fr2,fr1 */
Lc9093ac: *stop=0xc9093acu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc9093ae; /* fmov fr1,@(r0,r4) */
Lc9093ae: *stop=0xc9093aeu;r[0]=92u;goto Lc9093b0; /* mov #0x5c,r0 */
Lc9093b0: *stop=0xc9093b0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9093b2; /* fmov.s @(r0,r4),fr1 */
Lc9093b2: *stop=0xc9093b2u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc9093b4; /* fmov.s @r1,fr2 */
Lc9093b4: *stop=0xc9093b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc9093b6; /* fadd fr2,fr1 */
Lc9093b6: *stop=0xc9093b6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc9093b8; /* fmov fr1,@(r0,r4) */
Lc9093b8: *stop=0xc9093b8u;r[0]=96u;goto Lc9093ba; /* mov #0x60,r0 */
Lc9093ba: *stop=0xc9093bau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[0]+r[4]);goto Lc9093bc; /* fmov.s @(r0,r4),fr9 */
Lc9093bc: *stop=0xc9093bcu;r[0]=108u;goto Lc9093be; /* mov #0x6c,r0 */
Lc9093be: *stop=0xc9093beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[10],&f->fpscr,&fr[9],0);if(status)return status;}goto Lc9093c0; /* fadd fr10,fr9 */
Lc9093c0: *stop=0xc9093c0u;r[1]=r[0];goto Lc9093c2; /* mov r0,r1 */
Lc9093c2: *stop=0xc9093c2u;r[1]+=r[4];goto Lc9093c4; /* add r4,r1 */
Lc9093c4: *stop=0xc9093c4u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc9093c6; /* fmov.s @r1,fr2 */
Lc9093c6: *stop=0xc9093c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[5],&f->fpscr,&fr[9]);if(status)return status;}goto Lc9093c8; /* fmul fr5,fr9 */
Lc9093c8: *stop=0xc9093c8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9093ca; /* fmov.s @(r0,r4),fr1 */
Lc9093ca: *stop=0xc9093cau;r[1]=108u;goto Lc9093cc; /* mov #0x6c,r1 */
Lc9093cc: *stop=0xc9093ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9093ce; /* fmul fr8,fr2 */
Lc9093ce: *stop=0xc9093ceu;r[1]+=r[4];goto Lc9093d0; /* add r4,r1 */
Lc9093d0: *stop=0xc9093d0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc9093d2; /* fneg fr2 */
Lc9093d2: *stop=0xc9093d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[9],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc9093d4; /* fsub fr9,fr2 */
Lc9093d4: *stop=0xc9093d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc9093d6; /* fadd fr2,fr1 */
Lc9093d6: *stop=0xc9093d6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc9093d8; /* fmov fr1,@(r0,r4) */
Lc9093d8: *stop=0xc9093d8u;r[0]=96u;goto Lc9093da; /* mov #0x60,r0 */
Lc9093da: *stop=0xc9093dau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9093dc; /* fmov.s @(r0,r4),fr1 */
Lc9093dc: *stop=0xc9093dcu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc9093de; /* fmov.s @r1,fr2 */
Lc9093de: *stop=0xc9093deu;r[1]=rd(m,0x0c909550u,4);goto Lc9093e0; /* mov.l 0x0c909550,r1 */
Lc9093e0: *stop=0xc9093e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc9093e2; /* fadd fr2,fr1 */
Lc9093e2: *stop=0xc9093e2u;*fpul=r[1];goto Lc9093e4; /* lds r1,FPUL */
Lc9093e4: *stop=0xc9093e4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc9093e6; /* fmov fr1,@(r0,r4) */
Lc9093e6: *stop=0xc9093e6u;r[0]=0x0c90954cu;goto Lc9093e8; /* mova 0x0c90954c,r0 */
Lc9093e8: *stop=0xc9093e8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto Lc9093ea; /* fmov.s @r0,fr2 */
Lc9093ea: *stop=0xc9093eau;r[0]=72u;goto Lc9093ec; /* mov #0x48,r0 */
Lc9093ec: *stop=0xc9093ecu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto Lc9093ee; /* fmov.s @(r0,r4),fr8 */
Lc9093ee: *stop=0xc9093eeu;r[0]=84u;goto Lc9093f0; /* mov #0x54,r0 */
Lc9093f0: *stop=0xc9093f0u;r[1]=r[0];goto Lc9093f2; /* mov r0,r1 */
Lc9093f2: *stop=0xc9093f2u;r[1]+=r[4];goto Lc9093f4; /* add r4,r1 */
Lc9093f4: *stop=0xc9093f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[2],&f->fpscr,&fr[8],0);if(status)return status;}goto Lc9093f6; /* fadd fr2,fr8 */
Lc9093f6: *stop=0xc9093f6u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[1]);goto Lc9093f8; /* fmov.s @r1,fr0 */
Lc9093f8: *stop=0xc9093f8u;f->bank[(f->fpscr>>21)&1u][1]=*fpul;goto Lc9093fa; /* fsts FPUL,fr1 */
Lc9093fa: *stop=0xc9093fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[1],&f->fpscr,&fr[0]);if(status)return status;}goto Lc9093fc; /* fmul fr1,fr0 */
Lc9093fc: *stop=0xc9093fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[7],&f->fpscr,&fr[8]);if(status)return status;}goto Lc9093fe; /* fmul fr7,fr8 */
Lc9093fe: *stop=0xc9093feu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto Lc909400; /* fneg fr0 */
Lc909400: *stop=0xc909400u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[8],&f->fpscr,&fr[0],1);if(status)return status;}goto Lc909402; /* fsub fr8,fr0 */
Lc909402: *stop=0xc909402u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc909404; /* fmov.s @(r0,r4),fr3 */
Lc909404: *stop=0xc909404u;r[1]=84u;goto Lc909406; /* mov #0x54,r1 */
Lc909406: *stop=0xc909406u;r[1]+=r[4];goto Lc909408; /* add r4,r1 */
Lc909408: *stop=0xc909408u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[0],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc90940a; /* fadd fr0,fr3 */
Lc90940a: *stop=0xc90940au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc90940c; /* fmov fr3,@(r0,r4) */
Lc90940c: *stop=0xc90940cu;r[0]=72u;goto Lc90940e; /* mov #0x48,r0 */
Lc90940e: *stop=0xc90940eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909410; /* fmov.s @(r0,r4),fr0 */
Lc909410: *stop=0xc909410u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto Lc909412; /* fmov.s @r1,fr3 */
Lc909412: *stop=0xc909412u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909414; /* fadd fr3,fr0 */
Lc909414: *stop=0xc909414u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909416; /* fmov fr0,@(r0,r4) */
Lc909416: *stop=0xc909416u;if(!valid_ram(r[4]+4u,2u))return -3;r[0]=sx(m,r[4]+0x4u,2);goto Lc909418; /* mov.w @(0x4,r4),r0 */
Lc909418: *stop=0xc909418u;r[0]+=4294967295u;goto Lc90941a; /* add #-0x1,r0 */
Lc90941a: *stop=0xc90941au;if(!valid_ram(r[4]+4u,2u))return -3;wr(m,r[4]+0x4u,r[0],2);goto Lc90941c; /* mov.w r0,@(0x4,r4) */
Lc90941c: *stop=0xc90941cu;r[0]+=1u;goto Lc90941e; /* add #0x1,r0 */
Lc90941e: *stop=0xc90941eu;r[0]=r[0]&65535u;goto Lc909420; /* extu.w r0,r0 */
Lc909420: *stop=0xc909420u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc909422; /* tst r0,r0 */
Lc909422: if(!(*sr&1u))goto Lc90947c;goto Lc909424; /* bf 0x0c90947c */
Lc909424: *stop=0xc909424u;r[0]=56u;goto Lc909426; /* mov #0x38,r0 */
Lc909426: *stop=0xc909426u;if(!valid_ram(r[0]+r[4],2u))return -3;r[2]=sx(m,r[0]+r[4],2);goto Lc909428; /* mov.w @(r0,r4),r2 */
Lc909428: *stop=0xc909428u;r[2]+=1u;goto Lc90942a; /* add #0x1,r2 */
Lc90942a: *stop=0xc90942cu;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[2],2);goto Lc90947c; /* bra 0x0c90947c */
Lc90942e: *stop=0xc90942eu;r[1]=rd(m,0x0c909554u,4);goto Lc909430; /* mov.l 0x0c909554,r1 */
Lc909430: *stop=0xc909430u;r[0]=92u;goto Lc909432; /* mov #0x5c,r0 */
Lc909432: *stop=0xc909432u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc909434; /* fmov.s @(r0,r4),fr2 */
Lc909434: *stop=0xc909434u;*fpul=r[1];goto Lc909436; /* lds r1,FPUL */
Lc909436: *stop=0xc909436u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc909438; /* fsts FPUL,fr3 */
Lc909438: *stop=0xc909438u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc90943a; /* fsub fr3,fr2 */
Lc90943a: *stop=0xc90943au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc90943c; /* fmov fr2,@(r0,r4) */
Lc90943c: *stop=0xc90943cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc90943e; /* fmov.s @(r0,r4),fr1 */
Lc90943e: *stop=0xc90943eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc909440; /* fldi0 fr2 */
Lc909440: *stop=0xc909440u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto Lc909442; /* fcmp/gt fr1,fr2 */
Lc909442: if(!(*sr&1u))goto Lc909446;goto Lc909444; /* bf 0x0c909446 */
Lc909444: *stop=0xc909444u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909446; /* fmov fr4,@(r0,r4) */
Lc909446: *stop=0xc909446u;r[0]=96u;goto Lc909448; /* mov #0x60,r0 */
Lc909448: *stop=0xc909448u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc90944a; /* fldi0 fr2 */
Lc90944a: *stop=0xc90944au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc90944c; /* fmov.s @(r0,r4),fr3 */
Lc90944c: *stop=0xc90944cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc90944e; /* fsub fr7,fr3 */
Lc90944e: *stop=0xc90944eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc909450; /* fcmp/gt fr3,fr2 */
Lc909450: take=!(*sr&1u);*stop=0xc909452u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc909456;goto Lc909454; /* bf/s 0x0c909456 */
Lc909454: *stop=0xc909454u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909456; /* fmov fr4,@(r0,r4) */
Lc909456: *stop=0xc909456u;r[0]=72u;goto Lc909458; /* mov #0x48,r0 */
Lc909458: *stop=0xc909458u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc90945a; /* fldi0 fr2 */
Lc90945a: *stop=0xc90945au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc90945c; /* fmov.s @(r0,r4),fr3 */
Lc90945c: *stop=0xc90945cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc90945e; /* fsub fr7,fr3 */
Lc90945e: *stop=0xc90945eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc909460; /* fcmp/gt fr3,fr2 */
Lc909460: take=!(*sr&1u);*stop=0xc909462u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc90947c;goto Lc909464; /* bf/s 0x0c90947c */
Lc909464: *stop=0xc909464u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909466; /* fmov fr4,@(r0,r4) */
Lc909466: *stop=0xc909466u;r[0]=8u;goto Lc909468; /* mov #0x8,r0 */
Lc909468: *stop=0xc909468u;r[0]<<=8;goto Lc90946a; /* shll8 r0 */
Lc90946a: *stop=0xc90946au;r[0]+=18u;goto Lc90946c; /* add #0x12,r0 */
Lc90946c: *stop=0xc90946cu;r[6]=0u;goto Lc90946e; /* mov #0x0,r6 */
Lc90946e: *stop=0xc90946eu;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909470; /* mov.w r6,@(r0,r5) */
Lc909470: *stop=0xc909470u;r[0]+=54u;goto Lc909472; /* add #0x36,r0 */
Lc909472: *stop=0xc909472u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909474; /* mov.w r6,@(r0,r5) */
Lc909474: *stop=0xc909474u;r[0]+=64u;goto Lc909476; /* add #0x40,r0 */
Lc909476: *stop=0xc909476u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909478; /* mov.w r6,@(r0,r5) */
Lc909478: *stop=0xc909478u;r[0]+=4294967180u;goto Lc90947a; /* add #-0x74,r0 */
Lc90947a: *stop=0xc90947au;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc90947c; /* mov.w r6,@(r0,r5) */
Lc90947c: *stop=0xc90947cu;r[0]=120u;goto Lc90947e; /* mov #0x78,r0 */
Lc90947e: *stop=0xc90947eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto Lc909480; /* fldi1 fr7 */
Lc909480: *stop=0xc909480u;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto Lc909482; /* mov.w @(r0,r4),r0 */
Lc909482: *stop=0xc909482u;r[0]=r[0]&65535u;goto Lc909484; /* extu.w r0,r0 */
Lc909484: *stop=0xc909484u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc909486; /* cmp/eq #0x0,r0 */
Lc909486: take=(*sr&1u);*stop=0xc909488u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[7],&f->fpscr,&fr[7],0);if(status)return status;}if(take)goto Lc909496;goto Lc90948a; /* bt/s 0x0c909496 */
Lc90948a: *stop=0xc90948au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc90948c; /* cmp/eq #0x1,r0 */
Lc90948c: if((*sr&1u))goto Lc9094bc;goto Lc90948e; /* bt 0x0c9094bc */
Lc90948e: *stop=0xc90948eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto Lc909490; /* cmp/eq #0x2,r0 */
Lc909490: if((*sr&1u))goto Lc9094e8;goto Lc909492; /* bt 0x0c9094e8 */
Lc909492: *stop=0xc909492u;if(*pr!=entry_pr)return -3;*stop=0xc909494u;return 0; /* rts */
Lc909496: *stop=0xc909496u;r[0]=118u;goto Lc909498; /* mov #0x76,r0 */
Lc909498: *stop=0xc909498u;r[3]=1u;goto Lc90949a; /* mov #0x1,r3 */
Lc90949a: *stop=0xc90949au;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc90949c; /* mov.w r3,@(r0,r4) */
Lc90949c: *stop=0xc90949cu;r[0]+=14u;goto Lc90949e; /* add #0xe,r0 */
Lc90949e: *stop=0xc90949eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9094a0; /* fmov fr4,@(r0,r4) */
Lc9094a0: *stop=0xc9094a0u;r[0]+=4u;goto Lc9094a2; /* add #0x4,r0 */
Lc9094a2: *stop=0xc9094a2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9094a4; /* fmov fr4,@(r0,r4) */
Lc9094a4: *stop=0xc9094a4u;r[0]+=4u;goto Lc9094a6; /* add #0x4,r0 */
Lc9094a6: *stop=0xc9094a6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9094a8; /* fmov fr4,@(r0,r4) */
Lc9094a8: *stop=0xc9094a8u;r[0]+=16u;goto Lc9094aa; /* add #0x10,r0 */
Lc9094aa: *stop=0xc9094aau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto Lc9094ac; /* fldi1 fr4 */
Lc9094ac: *stop=0xc9094acu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9094ae; /* fmov fr4,@(r0,r4) */
Lc9094ae: *stop=0xc9094aeu;r[0]+=4u;goto Lc9094b0; /* add #0x4,r0 */
Lc9094b0: *stop=0xc9094b0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9094b2; /* fmov fr4,@(r0,r4) */
Lc9094b2: *stop=0xc9094b2u;r[0]=120u;goto Lc9094b4; /* mov #0x78,r0 */
Lc9094b4: *stop=0xc9094b4u;if(!valid_ram(r[0]+r[4],2u))return -3;r[3]=sx(m,r[0]+r[4],2);goto Lc9094b6; /* mov.w @(r0,r4),r3 */
Lc9094b6: *stop=0xc9094b6u;r[3]+=1u;goto Lc9094b8; /* add #0x1,r3 */
Lc9094b8: *stop=0xc9094b8u;if(*pr!=entry_pr)return -3;*stop=0xc9094bau;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);return 0; /* rts */
Lc9094bc: *stop=0xc9094bcu;r[1]=rd(m,0x0c909558u,4);goto Lc9094be; /* mov.l 0x0c909558,r1 */
Lc9094be: *stop=0xc9094beu;r[0]=4294967196u;goto Lc9094c0; /* mov #-0x64,r0 */
Lc9094c0: *stop=0xc9094c0u;r[0]=r[0]&255u;goto Lc9094c2; /* extu.b r0,r0 */
Lc9094c2: *stop=0xc9094c2u;*fpul=r[1];goto Lc9094c4; /* lds r1,FPUL */
Lc9094c4: *stop=0xc9094c4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc9094c6; /* fmov.s @(r0,r4),fr2 */
Lc9094c6: *stop=0xc9094c6u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc9094c8; /* fsts FPUL,fr3 */
Lc9094c8: *stop=0xc9094c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc9094ca; /* fsub fr3,fr2 */
Lc9094ca: *stop=0xc9094cau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc9094cc; /* fmov fr2,@(r0,r4) */
Lc9094cc: *stop=0xc9094ccu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc9094ce; /* fmov.s @(r0,r4),fr2 */
Lc9094ce: *stop=0xc9094ceu;r[0]+=4u;goto Lc9094d0; /* add #0x4,r0 */
Lc9094d0: *stop=0xc9094d0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc9094d2; /* fneg fr2 */
Lc9094d2: *stop=0xc9094d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[7],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc9094d4; /* fadd fr7,fr2 */
Lc9094d4: *stop=0xc9094d4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc9094d6; /* fmov fr2,@(r0,r4) */
Lc9094d6: *stop=0xc9094d6u;r[0]+=4294967292u;goto Lc9094d8; /* add #-0x4,r0 */
Lc9094d8: *stop=0xc9094d8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc9094da; /* fmov.s @(r0,r4),fr2 */
Lc9094da: *stop=0xc9094dau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto Lc9094dc; /* fcmp/gt fr2,fr6 */
Lc9094dc: if(!(*sr&1u))goto Lc90952c;goto Lc9094de; /* bf 0x0c90952c */
Lc9094de: *stop=0xc9094deu;r[0]=120u;goto Lc9094e0; /* mov #0x78,r0 */
Lc9094e0: *stop=0xc9094e0u;if(!valid_ram(r[0]+r[4],2u))return -3;r[2]=sx(m,r[0]+r[4],2);goto Lc9094e2; /* mov.w @(r0,r4),r2 */
Lc9094e2: *stop=0xc9094e2u;r[2]+=1u;goto Lc9094e4; /* add #0x1,r2 */
Lc9094e4: *stop=0xc9094e4u;if(*pr!=entry_pr)return -3;*stop=0xc9094e6u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[2],2);return 0; /* rts */
Lc9094e8: *stop=0xc9094e8u;r[1]=rd(m,0x0c90955cu,4);goto Lc9094ea; /* mov.l 0x0c90955c,r1 */
Lc9094ea: *stop=0xc9094eau;r[0]=4294967196u;goto Lc9094ec; /* mov #-0x64,r0 */
Lc9094ec: *stop=0xc9094ecu;r[0]=r[0]&255u;goto Lc9094ee; /* extu.b r0,r0 */
Lc9094ee: *stop=0xc9094eeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto Lc9094f0; /* fldi1 fr3 */
Lc9094f0: *stop=0xc9094f0u;*fpul=r[1];goto Lc9094f2; /* lds r1,FPUL */
Lc9094f2: *stop=0xc9094f2u;r[1]=4294967208u;goto Lc9094f4; /* mov #-0x58,r1 */
Lc9094f4: *stop=0xc9094f4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[4]);goto Lc9094f6; /* fmov.s @(r0,r4),fr4 */
Lc9094f6: *stop=0xc9094f6u;r[1]=r[1]&255u;goto Lc9094f8; /* extu.b r1,r1 */
Lc9094f8: *stop=0xc9094f8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto Lc9094fa; /* fneg fr3 */
Lc9094fa: *stop=0xc9094fau;r[1]+=r[4];goto Lc9094fc; /* add r4,r1 */
Lc9094fc: *stop=0xc9094fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto Lc9094fe; /* fadd fr3,fr4 */
Lc9094fe: *stop=0xc9094feu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc909500; /* fmov.s @r1,fr1 */
Lc909500: *stop=0xc909500u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto Lc909502; /* fsts FPUL,fr2 */
Lc909502: *stop=0xc909502u;r[0]+=12u;goto Lc909504; /* add #0xc,r0 */
Lc909504: *stop=0xc909504u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc909506; /* fmul fr2,fr1 */
Lc909506: *stop=0xc909506u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909508; /* fmov.s @(r0,r4),fr0 */
Lc909508: *stop=0xc909508u;r[1]=4294967208u;goto Lc90950a; /* mov #-0x58,r1 */
Lc90950a: *stop=0xc90950au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[5],&f->fpscr,&fr[4]);if(status)return status;}goto Lc90950c; /* fmul fr5,fr4 */
Lc90950c: *stop=0xc90950cu;r[1]=r[1]&255u;goto Lc90950e; /* extu.b r1,r1 */
Lc90950e: *stop=0xc90950eu;r[1]+=r[4];goto Lc909510; /* add r4,r1 */
Lc909510: *stop=0xc909510u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc909512; /* fneg fr1 */
Lc909512: *stop=0xc909512u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],1);if(status)return status;}goto Lc909514; /* fsub fr4,fr1 */
Lc909514: *stop=0xc909514u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909516; /* fadd fr1,fr0 */
Lc909516: *stop=0xc909516u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909518; /* fmov fr0,@(r0,r4) */
Lc909518: *stop=0xc909518u;r[0]+=4294967284u;goto Lc90951a; /* add #-0xc,r0 */
Lc90951a: *stop=0xc90951au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc90951c; /* fmov.s @(r0,r4),fr0 */
Lc90951c: *stop=0xc90951cu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc90951e; /* fmov.s @r1,fr1 */
Lc90951e: *stop=0xc90951eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909520; /* fadd fr1,fr0 */
Lc909520: *stop=0xc909520u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909522; /* fmov fr0,@(r0,r4) */
Lc909522: *stop=0xc909522u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc909524; /* fmov.s @(r0,r4),fr1 */
Lc909524: *stop=0xc909524u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc909526; /* fneg fr1 */
Lc909526: *stop=0xc909526u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[7],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc909528; /* fadd fr7,fr1 */
Lc909528: *stop=0xc909528u;r[0]+=4u;goto Lc90952a; /* add #0x4,r0 */
Lc90952a: *stop=0xc90952au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc90952c; /* fmov fr1,@(r0,r4) */
Lc90952c: *stop=0xc90952cu;if(*pr!=entry_pr)return -3;*stop=0xc90952eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_profile_five_0c909560(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc909560;
Lc909560: *stop=0xc909560u;r[0]=0x0c909700u;goto Lc909562; /* mova 0x0c909700,r0 */
Lc909562: *stop=0xc909562u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]);goto Lc909564; /* fmov.s @r0,fr8 */
Lc909564: *stop=0xc909564u;r[0]=0x0c909704u;goto Lc909566; /* mova 0x0c909704,r0 */
Lc909566: *stop=0xc909566u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]);goto Lc909568; /* fmov.s @r0,fr7 */
Lc909568: *stop=0xc909568u;r[0]=0x0c909708u;goto Lc90956a; /* mova 0x0c909708,r0 */
Lc90956a: *stop=0xc90956au;r[4]=8u;goto Lc90956c; /* mov #0x8,r4 */
Lc90956c: *stop=0xc90956cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]);goto Lc90956e; /* fmov.s @r0,fr6 */
Lc90956e: *stop=0xc90956eu;r[0]=0x0c90970cu;goto Lc909570; /* mova 0x0c90970c,r0 */
Lc909570: *stop=0xc909570u;r[4]<<=8;goto Lc909572; /* shll8 r4 */
Lc909572: *stop=0xc909572u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc909574; /* fmov.s @r0,fr5 */
Lc909574: *stop=0xc909574u;r[4]+=16u;goto Lc909576; /* add #0x10,r4 */
Lc909576: *stop=0xc909576u;r[0]=56u;goto Lc909578; /* mov #0x38,r0 */
Lc909578: *stop=0xc909578u;r[4]+=r[5];goto Lc90957a; /* add r5,r4 */
Lc90957a: *stop=0xc90957au;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto Lc90957c; /* mov.w @(r0,r4),r0 */
Lc90957c: *stop=0xc90957cu;r[0]=r[0]&65535u;goto Lc90957e; /* extu.w r0,r0 */
Lc90957e: *stop=0xc90957eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc909580; /* cmp/eq #0x0,r0 */
Lc909580: take=(*sr&1u);*stop=0xc909582u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;if(take)goto Lc909594;goto Lc909584; /* bt/s 0x0c909594 */
Lc909584: *stop=0xc909584u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc909586; /* cmp/eq #0x1,r0 */
Lc909586: if((*sr&1u))goto Lc9095d0;goto Lc909588; /* bt 0x0c9095d0 */
Lc909588: *stop=0xc909588u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto Lc90958a; /* cmp/eq #0x2,r0 */
Lc90958a: if((*sr&1u))goto Lc9095e6;goto Lc90958c; /* bt 0x0c9095e6 */
Lc90958c: *stop=0xc90958cu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto Lc90958e; /* cmp/eq #0x3,r0 */
Lc90958e: if((*sr&1u))goto Lc909684;goto Lc909590; /* bt 0x0c909684 */
Lc909590: *stop=0xc909592u;goto Lc9096d2; /* bra 0x0c9096d2 */
Lc909594: *stop=0xc909594u;r[0]=54u;goto Lc909596; /* mov #0x36,r0 */
Lc909596: *stop=0xc909596u;r[3]=4u;goto Lc909598; /* mov #0x4,r3 */
Lc909598: *stop=0xc909598u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc90959a; /* mov.w r3,@(r0,r4) */
Lc90959a: *stop=0xc90959au;r[0]=0x0c909710u;goto Lc90959c; /* mova 0x0c909710,r0 */
Lc90959c: *stop=0xc90959cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc90959e; /* fmov.s @r0,fr3 */
Lc90959e: *stop=0xc90959eu;r[0]=68u;goto Lc9095a0; /* mov #0x44,r0 */
Lc9095a0: *stop=0xc9095a0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc9095a2; /* fmov fr3,@(r0,r4) */
Lc9095a2: *stop=0xc9095a2u;r[0]=72u;goto Lc9095a4; /* mov #0x48,r0 */
Lc9095a4: *stop=0xc9095a4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9095a6; /* fmov fr4,@(r0,r4) */
Lc9095a6: *stop=0xc9095a6u;r[0]=0x0c909714u;goto Lc9095a8; /* mova 0x0c909714,r0 */
Lc9095a8: *stop=0xc9095a8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc9095aa; /* fmov.s @r0,fr3 */
Lc9095aa: *stop=0xc9095aau;r[0]=76u;goto Lc9095ac; /* mov #0x4c,r0 */
Lc9095ac: *stop=0xc9095acu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc9095ae; /* fmov fr3,@(r0,r4) */
Lc9095ae: *stop=0xc9095aeu;r[0]=84u;goto Lc9095b0; /* mov #0x54,r0 */
Lc9095b0: *stop=0xc9095b0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9095b2; /* fmov fr4,@(r0,r4) */
Lc9095b2: *stop=0xc9095b2u;r[0]=92u;goto Lc9095b4; /* mov #0x5c,r0 */
Lc9095b4: *stop=0xc9095b4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[4]);goto Lc9095b6; /* fmov fr5,@(r0,r4) */
Lc9095b6: *stop=0xc9095b6u;r[0]=0x0c909718u;goto Lc9095b8; /* mova 0x0c909718,r0 */
Lc9095b8: *stop=0xc9095b8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc9095ba; /* fmov.s @r0,fr3 */
Lc9095ba: *stop=0xc9095bau;r[0]=96u;goto Lc9095bc; /* mov #0x60,r0 */
Lc9095bc: *stop=0xc9095bcu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc9095be; /* fmov fr3,@(r0,r4) */
Lc9095be: *stop=0xc9095beu;r[0]=104u;goto Lc9095c0; /* mov #0x68,r0 */
Lc9095c0: *stop=0xc9095c0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9095c2; /* fmov fr4,@(r0,r4) */
Lc9095c2: *stop=0xc9095c2u;r[0]=108u;goto Lc9095c4; /* mov #0x6c,r0 */
Lc9095c4: *stop=0xc9095c4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9095c6; /* fmov fr4,@(r0,r4) */
Lc9095c6: *stop=0xc9095c6u;r[0]=56u;goto Lc9095c8; /* mov #0x38,r0 */
Lc9095c8: *stop=0xc9095c8u;if(!valid_ram(r[0]+r[4],2u))return -3;r[3]=sx(m,r[0]+r[4],2);goto Lc9095ca; /* mov.w @(r0,r4),r3 */
Lc9095ca: *stop=0xc9095cau;r[3]+=1u;goto Lc9095cc; /* add #0x1,r3 */
Lc9095cc: *stop=0xc9095ceu;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc9096d2; /* bra 0x0c9096d2 */
Lc9095d0: *stop=0xc9095d0u;r[0]=52u;goto Lc9095d2; /* mov #0x34,r0 */
Lc9095d2: *stop=0xc9095d2u;if(!valid_ram(r[0]+r[4],2u))return -3;r[1]=sx(m,r[0]+r[4],2);goto Lc9095d4; /* mov.w @(r0,r4),r1 */
Lc9095d4: *stop=0xc9095d4u;r[3]=10u;goto Lc9095d6; /* mov #0xa,r3 */
Lc9095d6: *stop=0xc9095d6u;r[1]=r[1]&65535u;goto Lc9095d8; /* extu.w r1,r1 */
Lc9095d8: *stop=0xc9095d8u;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[3]);goto Lc9095da; /* cmp/gt r3,r1 */
Lc9095da: if(!(*sr&1u))goto Lc9096d2;goto Lc9095dc; /* bf 0x0c9096d2 */
Lc9095dc: *stop=0xc9095dcu;r[0]=56u;goto Lc9095de; /* mov #0x38,r0 */
Lc9095de: *stop=0xc9095deu;if(!valid_ram(r[0]+r[4],2u))return -3;r[1]=sx(m,r[0]+r[4],2);goto Lc9095e0; /* mov.w @(r0,r4),r1 */
Lc9095e0: *stop=0xc9095e0u;r[1]+=1u;goto Lc9095e2; /* add #0x1,r1 */
Lc9095e2: *stop=0xc9095e4u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[1],2);goto Lc9096d2; /* bra 0x0c9096d2 */
Lc9095e6: *stop=0xc9095e6u;r[0]=0x0c90971cu;goto Lc9095e8; /* mova 0x0c90971c,r0 */
Lc9095e8: *stop=0xc9095e8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,10u,r[0]);goto Lc9095ea; /* fmov.s @r0,fr10 */
Lc9095ea: *stop=0xc9095eau;r[0]=92u;goto Lc9095ec; /* mov #0x5c,r0 */
Lc9095ec: *stop=0xc9095ecu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[0]+r[4]);goto Lc9095ee; /* fmov.s @(r0,r4),fr9 */
Lc9095ee: *stop=0xc9095eeu;r[0]=104u;goto Lc9095f0; /* mov #0x68,r0 */
Lc9095f0: *stop=0xc9095f0u;r[1]=r[0];goto Lc9095f2; /* mov r0,r1 */
Lc9095f2: *stop=0xc9095f2u;r[1]+=r[4];goto Lc9095f4; /* add r4,r1 */
Lc9095f4: *stop=0xc9095f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[10],&f->fpscr,&fr[9],0);if(status)return status;}goto Lc9095f6; /* fadd fr10,fr9 */
Lc9095f6: *stop=0xc9095f6u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto Lc9095f8; /* fmov.s @r1,fr3 */
Lc9095f8: *stop=0xc9095f8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc9095fa; /* fmov.s @(r0,r4),fr2 */
Lc9095fa: *stop=0xc9095fau;r[1]=104u;goto Lc9095fc; /* mov #0x68,r1 */
Lc9095fc: *stop=0xc9095fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9095fe; /* fmul fr6,fr3 */
Lc9095fe: *stop=0xc9095feu;r[1]+=r[4];goto Lc909600; /* add r4,r1 */
Lc909600: *stop=0xc909600u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[7],&f->fpscr,&fr[9]);if(status)return status;}goto Lc909602; /* fmul fr7,fr9 */
Lc909602: *stop=0xc909602u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto Lc909604; /* fneg fr3 */
Lc909604: *stop=0xc909604u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[9],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc909606; /* fsub fr9,fr3 */
Lc909606: *stop=0xc909606u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc909608; /* fadd fr3,fr2 */
Lc909608: *stop=0xc909608u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc90960a; /* fmov fr2,@(r0,r4) */
Lc90960a: *stop=0xc90960au;r[0]=92u;goto Lc90960c; /* mov #0x5c,r0 */
Lc90960c: *stop=0xc90960cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc90960e; /* fmov.s @(r0,r4),fr2 */
Lc90960e: *stop=0xc90960eu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto Lc909610; /* fmov.s @r1,fr3 */
Lc909610: *stop=0xc909610u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc909612; /* fadd fr3,fr2 */
Lc909612: *stop=0xc909612u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc909614; /* fmov fr2,@(r0,r4) */
Lc909614: *stop=0xc909614u;r[0]=96u;goto Lc909616; /* mov #0x60,r0 */
Lc909616: *stop=0xc909616u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[0]+r[4]);goto Lc909618; /* fmov.s @(r0,r4),fr9 */
Lc909618: *stop=0xc909618u;r[0]=108u;goto Lc90961a; /* mov #0x6c,r0 */
Lc90961a: *stop=0xc90961au;r[1]=r[0];goto Lc90961c; /* mov r0,r1 */
Lc90961c: *stop=0xc90961cu;r[1]+=r[4];goto Lc90961e; /* add r4,r1 */
Lc90961e: *stop=0xc90961eu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto Lc909620; /* fmov.s @r1,fr3 */
Lc909620: *stop=0xc909620u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[10],&f->fpscr,&fr[9],0);if(status)return status;}goto Lc909622; /* fadd fr10,fr9 */
Lc909622: *stop=0xc909622u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto Lc909624; /* fmul fr6,fr3 */
Lc909624: *stop=0xc909624u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto Lc909626; /* fneg fr3 */
Lc909626: *stop=0xc909626u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[7],&f->fpscr,&fr[9]);if(status)return status;}goto Lc909628; /* fmul fr7,fr9 */
Lc909628: *stop=0xc909628u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc90962a; /* fmov.s @(r0,r4),fr2 */
Lc90962a: *stop=0xc90962au;r[1]=108u;goto Lc90962c; /* mov #0x6c,r1 */
Lc90962c: *stop=0xc90962cu;r[1]+=r[4];goto Lc90962e; /* add r4,r1 */
Lc90962e: *stop=0xc90962eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[9],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc909630; /* fsub fr9,fr3 */
Lc909630: *stop=0xc909630u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc909632; /* fadd fr3,fr2 */
Lc909632: *stop=0xc909632u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc909634; /* fmov fr2,@(r0,r4) */
Lc909634: *stop=0xc909634u;r[0]=96u;goto Lc909636; /* mov #0x60,r0 */
Lc909636: *stop=0xc909636u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc909638; /* fmov.s @(r0,r4),fr2 */
Lc909638: *stop=0xc909638u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto Lc90963a; /* fmov.s @r1,fr3 */
Lc90963a: *stop=0xc90963au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc90963c; /* fadd fr3,fr2 */
Lc90963c: *stop=0xc90963cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc90963e; /* fmov fr2,@(r0,r4) */
Lc90963e: *stop=0xc90963eu;r[0]=0x0c909720u;goto Lc909640; /* mova 0x0c909720,r0 */
Lc909640: *stop=0xc909640u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc909642; /* fmov.s @r0,fr3 */
Lc909642: *stop=0xc909642u;r[0]=72u;goto Lc909644; /* mov #0x48,r0 */
Lc909644: *stop=0xc909644u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[0]+r[4]);goto Lc909646; /* fmov.s @(r0,r4),fr9 */
Lc909646: *stop=0xc909646u;r[0]=84u;goto Lc909648; /* mov #0x54,r0 */
Lc909648: *stop=0xc909648u;r[1]=r[0];goto Lc90964a; /* mov r0,r1 */
Lc90964a: *stop=0xc90964au;r[1]+=r[4];goto Lc90964c; /* add r4,r1 */
Lc90964c: *stop=0xc90964cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[3],&f->fpscr,&fr[9],0);if(status)return status;}goto Lc90964e; /* fadd fr3,fr9 */
Lc90964e: *stop=0xc90964eu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc909650; /* fmov.s @r1,fr2 */
Lc909650: *stop=0xc909650u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc909652; /* fmov.s @(r0,r4),fr1 */
Lc909652: *stop=0xc909652u;r[1]=84u;goto Lc909654; /* mov #0x54,r1 */
Lc909654: *stop=0xc909654u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto Lc909656; /* fmul fr5,fr2 */
Lc909656: *stop=0xc909656u;r[1]+=r[4];goto Lc909658; /* add r4,r1 */
Lc909658: *stop=0xc909658u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[8],&f->fpscr,&fr[9]);if(status)return status;}goto Lc90965a; /* fmul fr8,fr9 */
Lc90965a: *stop=0xc90965au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc90965c; /* fneg fr2 */
Lc90965c: *stop=0xc90965cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[9],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc90965e; /* fsub fr9,fr2 */
Lc90965e: *stop=0xc90965eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc909660; /* fadd fr2,fr1 */
Lc909660: *stop=0xc909660u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc909662; /* fmov fr1,@(r0,r4) */
Lc909662: *stop=0xc909662u;r[0]=72u;goto Lc909664; /* mov #0x48,r0 */
Lc909664: *stop=0xc909664u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc909666; /* fmov.s @r1,fr2 */
Lc909666: *stop=0xc909666u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc909668; /* fmov.s @(r0,r4),fr1 */
Lc909668: *stop=0xc909668u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc90966a; /* fadd fr2,fr1 */
Lc90966a: *stop=0xc90966au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc90966c; /* fmov fr1,@(r0,r4) */
Lc90966c: *stop=0xc90966cu;if(!valid_ram(r[4]+4u,2u))return -3;r[0]=sx(m,r[4]+0x4u,2);goto Lc90966e; /* mov.w @(0x4,r4),r0 */
Lc90966e: *stop=0xc90966eu;r[0]+=4294967295u;goto Lc909670; /* add #-0x1,r0 */
Lc909670: *stop=0xc909670u;if(!valid_ram(r[4]+4u,2u))return -3;wr(m,r[4]+0x4u,r[0],2);goto Lc909672; /* mov.w r0,@(0x4,r4) */
Lc909672: *stop=0xc909672u;r[0]+=1u;goto Lc909674; /* add #0x1,r0 */
Lc909674: *stop=0xc909674u;r[0]=r[0]&65535u;goto Lc909676; /* extu.w r0,r0 */
Lc909676: *stop=0xc909676u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc909678; /* tst r0,r0 */
Lc909678: if(!(*sr&1u))goto Lc9096d2;goto Lc90967a; /* bf 0x0c9096d2 */
Lc90967a: *stop=0xc90967au;r[0]=56u;goto Lc90967c; /* mov #0x38,r0 */
Lc90967c: *stop=0xc90967cu;if(!valid_ram(r[0]+r[4],2u))return -3;r[2]=sx(m,r[0]+r[4],2);goto Lc90967e; /* mov.w @(r0,r4),r2 */
Lc90967e: *stop=0xc90967eu;r[2]+=1u;goto Lc909680; /* add #0x1,r2 */
Lc909680: *stop=0xc909682u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[2],2);goto Lc9096d2; /* bra 0x0c9096d2 */
Lc909684: *stop=0xc909684u;r[0]=92u;goto Lc909686; /* mov #0x5c,r0 */
Lc909686: *stop=0xc909686u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc909688; /* fldi0 fr2 */
Lc909688: *stop=0xc909688u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc90968a; /* fmov.s @(r0,r4),fr3 */
Lc90968a: *stop=0xc90968au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[8],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc90968c; /* fsub fr8,fr3 */
Lc90968c: *stop=0xc90968cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc90968e; /* fcmp/gt fr3,fr2 */
Lc90968e: take=!(*sr&1u);*stop=0xc909690u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc909694;goto Lc909692; /* bf/s 0x0c909694 */
Lc909692: *stop=0xc909692u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909694; /* fmov fr4,@(r0,r4) */
Lc909694: *stop=0xc909694u;r[0]=96u;goto Lc909696; /* mov #0x60,r0 */
Lc909696: *stop=0xc909696u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc909698; /* fldi0 fr2 */
Lc909698: *stop=0xc909698u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc90969a; /* fmov.s @(r0,r4),fr3 */
Lc90969a: *stop=0xc90969au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[8],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc90969c; /* fsub fr8,fr3 */
Lc90969c: *stop=0xc90969cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc90969e; /* fcmp/gt fr3,fr2 */
Lc90969e: take=!(*sr&1u);*stop=0xc9096a0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc9096a4;goto Lc9096a2; /* bf/s 0x0c9096a4 */
Lc9096a2: *stop=0xc9096a2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9096a4; /* fmov fr4,@(r0,r4) */
Lc9096a4: *stop=0xc9096a4u;r[1]=rd(m,0x0c909724u,4);goto Lc9096a6; /* mov.l 0x0c909724,r1 */
Lc9096a6: *stop=0xc9096a6u;r[0]=72u;goto Lc9096a8; /* mov #0x48,r0 */
Lc9096a8: *stop=0xc9096a8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc9096aa; /* fmov.s @(r0,r4),fr2 */
Lc9096aa: *stop=0xc9096aau;*fpul=r[1];goto Lc9096ac; /* lds r1,FPUL */
Lc9096ac: *stop=0xc9096acu;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc9096ae; /* fsts FPUL,fr3 */
Lc9096ae: *stop=0xc9096aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc9096b0; /* fsub fr3,fr2 */
Lc9096b0: *stop=0xc9096b0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc9096b2; /* fmov fr2,@(r0,r4) */
Lc9096b2: *stop=0xc9096b2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9096b4; /* fmov.s @(r0,r4),fr1 */
Lc9096b4: *stop=0xc9096b4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc9096b6; /* fldi0 fr2 */
Lc9096b6: *stop=0xc9096b6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto Lc9096b8; /* fcmp/gt fr1,fr2 */
Lc9096b8: if(!(*sr&1u))goto Lc9096d2;goto Lc9096ba; /* bf 0x0c9096d2 */
Lc9096ba: *stop=0xc9096bau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9096bc; /* fmov fr4,@(r0,r4) */
Lc9096bc: *stop=0xc9096bcu;r[0]=8u;goto Lc9096be; /* mov #0x8,r0 */
Lc9096be: *stop=0xc9096beu;r[0]<<=8;goto Lc9096c0; /* shll8 r0 */
Lc9096c0: *stop=0xc9096c0u;r[0]+=18u;goto Lc9096c2; /* add #0x12,r0 */
Lc9096c2: *stop=0xc9096c2u;r[6]=0u;goto Lc9096c4; /* mov #0x0,r6 */
Lc9096c4: *stop=0xc9096c4u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc9096c6; /* mov.w r6,@(r0,r5) */
Lc9096c6: *stop=0xc9096c6u;r[0]+=54u;goto Lc9096c8; /* add #0x36,r0 */
Lc9096c8: *stop=0xc9096c8u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc9096ca; /* mov.w r6,@(r0,r5) */
Lc9096ca: *stop=0xc9096cau;r[0]+=64u;goto Lc9096cc; /* add #0x40,r0 */
Lc9096cc: *stop=0xc9096ccu;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc9096ce; /* mov.w r6,@(r0,r5) */
Lc9096ce: *stop=0xc9096ceu;r[0]+=4294967180u;goto Lc9096d0; /* add #-0x74,r0 */
Lc9096d0: *stop=0xc9096d0u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc9096d2; /* mov.w r6,@(r0,r5) */
Lc9096d2: *stop=0xc9096d2u;r[0]=120u;goto Lc9096d4; /* mov #0x78,r0 */
Lc9096d4: *stop=0xc9096d4u;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto Lc9096d6; /* mov.w @(r0,r4),r0 */
Lc9096d6: *stop=0xc9096d6u;r[0]=r[0]&65535u;goto Lc9096d8; /* extu.w r0,r0 */
Lc9096d8: *stop=0xc9096d8u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc9096da; /* cmp/eq #0x0,r0 */
Lc9096da: if((*sr&1u))goto Lc909740;goto Lc9096dc; /* bt 0x0c909740 */
Lc9096dc: *stop=0xc9096dcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc9096de; /* cmp/eq #0x1,r0 */
Lc9096de: if((*sr&1u))goto Lc909766;goto Lc9096e0; /* bt 0x0c909766 */
Lc9096e0: *stop=0xc9096e0u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto Lc9096e2; /* cmp/eq #0x2,r0 */
Lc9096e2: if((*sr&1u))goto Lc909796;goto Lc9096e4; /* bt 0x0c909796 */
Lc9096e4: *stop=0xc9096e4u;if(*pr!=entry_pr)return -3;*stop=0xc9096e6u;return 0; /* rts */
Lc909740: *stop=0xc909740u;r[0]=118u;goto Lc909742; /* mov #0x76,r0 */
Lc909742: *stop=0xc909742u;r[3]=1u;goto Lc909744; /* mov #0x1,r3 */
Lc909744: *stop=0xc909744u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc909746; /* mov.w r3,@(r0,r4) */
Lc909746: *stop=0xc909746u;r[0]+=14u;goto Lc909748; /* add #0xe,r0 */
Lc909748: *stop=0xc909748u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc90974a; /* fmov fr4,@(r0,r4) */
Lc90974a: *stop=0xc90974au;r[0]+=4u;goto Lc90974c; /* add #0x4,r0 */
Lc90974c: *stop=0xc90974cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc90974e; /* fmov fr4,@(r0,r4) */
Lc90974e: *stop=0xc90974eu;r[0]+=4u;goto Lc909750; /* add #0x4,r0 */
Lc909750: *stop=0xc909750u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909752; /* fmov fr4,@(r0,r4) */
Lc909752: *stop=0xc909752u;r[0]+=16u;goto Lc909754; /* add #0x10,r0 */
Lc909754: *stop=0xc909754u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto Lc909756; /* fldi1 fr4 */
Lc909756: *stop=0xc909756u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909758; /* fmov fr4,@(r0,r4) */
Lc909758: *stop=0xc909758u;r[0]+=4u;goto Lc90975a; /* add #0x4,r0 */
Lc90975a: *stop=0xc90975au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc90975c; /* fmov fr4,@(r0,r4) */
Lc90975c: *stop=0xc90975cu;r[0]=120u;goto Lc90975e; /* mov #0x78,r0 */
Lc90975e: *stop=0xc90975eu;if(!valid_ram(r[0]+r[4],2u))return -3;r[3]=sx(m,r[0]+r[4],2);goto Lc909760; /* mov.w @(r0,r4),r3 */
Lc909760: *stop=0xc909760u;r[3]+=1u;goto Lc909762; /* add #0x1,r3 */
Lc909762: *stop=0xc909762u;if(*pr!=entry_pr)return -3;*stop=0xc909764u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);return 0; /* rts */
Lc909766: *stop=0xc909766u;r[1]=rd(m,0x0c909880u,4);goto Lc909768; /* mov.l 0x0c909880,r1 */
Lc909768: *stop=0xc909768u;r[0]=4294967196u;goto Lc90976a; /* mov #-0x64,r0 */
Lc90976a: *stop=0xc90976au;r[0]=r[0]&255u;goto Lc90976c; /* extu.b r0,r0 */
Lc90976c: *stop=0xc90976cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto Lc90976e; /* fldi1 fr1 */
Lc90976e: *stop=0xc90976eu;*fpul=r[1];goto Lc909770; /* lds r1,FPUL */
Lc909770: *stop=0xc909770u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[1],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc909772; /* fadd fr1,fr1 */
Lc909772: *stop=0xc909772u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc909774; /* fmov.s @(r0,r4),fr2 */
Lc909774: *stop=0xc909774u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc909776; /* fsts FPUL,fr3 */
Lc909776: *stop=0xc909776u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc909778; /* fsub fr3,fr2 */
Lc909778: *stop=0xc909778u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc90977a; /* fmov fr2,@(r0,r4) */
Lc90977a: *stop=0xc90977au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc90977c; /* fmov.s @(r0,r4),fr2 */
Lc90977c: *stop=0xc90977cu;r[0]+=4u;goto Lc90977e; /* add #0x4,r0 */
Lc90977e: *stop=0xc90977eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc909780; /* fneg fr2 */
Lc909780: *stop=0xc909780u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc909782; /* fadd fr1,fr2 */
Lc909782: *stop=0xc909782u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc909784; /* fmov fr2,@(r0,r4) */
Lc909784: *stop=0xc909784u;r[0]+=4294967292u;goto Lc909786; /* add #-0x4,r0 */
Lc909786: *stop=0xc909786u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc909788; /* fmov.s @(r0,r4),fr2 */
Lc909788: *stop=0xc909788u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto Lc90978a; /* fcmp/gt fr2,fr5 */
Lc90978a: if(!(*sr&1u))goto Lc9097e0;goto Lc90978c; /* bf 0x0c9097e0 */
Lc90978c: *stop=0xc90978cu;r[0]=120u;goto Lc90978e; /* mov #0x78,r0 */
Lc90978e: *stop=0xc90978eu;if(!valid_ram(r[0]+r[4],2u))return -3;r[2]=sx(m,r[0]+r[4],2);goto Lc909790; /* mov.w @(r0,r4),r2 */
Lc909790: *stop=0xc909790u;r[2]+=1u;goto Lc909792; /* add #0x1,r2 */
Lc909792: *stop=0xc909792u;if(*pr!=entry_pr)return -3;*stop=0xc909794u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[2],2);return 0; /* rts */
Lc909796: *stop=0xc909796u;r[0]=0x0c909884u;goto Lc909798; /* mova 0x0c909884,r0 */
Lc909798: *stop=0xc909798u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc90979a; /* fmov.s @r0,fr3 */
Lc90979a: *stop=0xc90979au;r[0]=4294967196u;goto Lc90979c; /* mov #-0x64,r0 */
Lc90979c: *stop=0xc90979cu;r[0]=r[0]&255u;goto Lc90979e; /* extu.b r0,r0 */
Lc90979e: *stop=0xc90979eu;r[1]=4294967208u;goto Lc9097a0; /* mov #-0x58,r1 */
Lc9097a0: *stop=0xc9097a0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[4]);goto Lc9097a2; /* fmov.s @(r0,r4),fr4 */
Lc9097a2: *stop=0xc9097a2u;r[1]=r[1]&255u;goto Lc9097a4; /* extu.b r1,r1 */
Lc9097a4: *stop=0xc9097a4u;r[1]+=r[4];goto Lc9097a6; /* add r4,r1 */
Lc9097a6: *stop=0xc9097a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto Lc9097a8; /* fadd fr3,fr4 */
Lc9097a8: *stop=0xc9097a8u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc9097aa; /* fmov.s @r1,fr2 */
Lc9097aa: *stop=0xc9097aau;r[0]+=12u;goto Lc9097ac; /* add #0xc,r0 */
Lc9097ac: *stop=0xc9097acu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9097ae; /* fmov.s @(r0,r4),fr1 */
Lc9097ae: *stop=0xc9097aeu;r[1]=4294967208u;goto Lc9097b0; /* mov #-0x58,r1 */
Lc9097b0: *stop=0xc9097b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9097b2; /* fmul fr6,fr2 */
Lc9097b2: *stop=0xc9097b2u;r[1]=r[1]&255u;goto Lc9097b4; /* extu.b r1,r1 */
Lc9097b4: *stop=0xc9097b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[7],&f->fpscr,&fr[4]);if(status)return status;}goto Lc9097b6; /* fmul fr7,fr4 */
Lc9097b6: *stop=0xc9097b6u;r[1]+=r[4];goto Lc9097b8; /* add r4,r1 */
Lc9097b8: *stop=0xc9097b8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc9097ba; /* fneg fr2 */
Lc9097ba: *stop=0xc9097bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc9097bc; /* fsub fr4,fr2 */
Lc9097bc: *stop=0xc9097bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc9097be; /* fadd fr2,fr1 */
Lc9097be: *stop=0xc9097beu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc9097c0; /* fmov fr1,@(r0,r4) */
Lc9097c0: *stop=0xc9097c0u;r[0]+=4294967284u;goto Lc9097c2; /* add #-0xc,r0 */
Lc9097c2: *stop=0xc9097c2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9097c4; /* fmov.s @(r0,r4),fr1 */
Lc9097c4: *stop=0xc9097c4u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc9097c6; /* fmov.s @r1,fr2 */
Lc9097c6: *stop=0xc9097c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc9097c8; /* fadd fr2,fr1 */
Lc9097c8: *stop=0xc9097c8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc9097ca; /* fmov fr1,@(r0,r4) */
Lc9097ca: *stop=0xc9097cau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc9097cc; /* fmov.s @(r0,r4),fr2 */
Lc9097cc: *stop=0xc9097ccu;r[0]=0x0c909888u;goto Lc9097ce; /* mova 0x0c909888,r0 */
Lc9097ce: *stop=0xc9097ceu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto Lc9097d0; /* fmov.s @r0,fr1 */
Lc9097d0: *stop=0xc9097d0u;r[0]=0x0c90988cu;goto Lc9097d2; /* mova 0x0c90988c,r0 */
Lc9097d2: *stop=0xc9097d2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc9097d4; /* fneg fr2 */
Lc9097d4: *stop=0xc9097d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc9097d6; /* fadd fr1,fr2 */
Lc9097d6: *stop=0xc9097d6u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]);goto Lc9097d8; /* fmov.s @r0,fr0 */
Lc9097d8: *stop=0xc9097d8u;r[0]=4294967200u;goto Lc9097da; /* mov #-0x60,r0 */
Lc9097da: *stop=0xc9097dau;r[0]=r[0]&255u;goto Lc9097dc; /* extu.b r0,r0 */
Lc9097dc: *stop=0xc9097dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9097de; /* fmul fr0,fr2 */
Lc9097de: *stop=0xc9097deu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc9097e0; /* fmov fr2,@(r0,r4) */
Lc9097e0: *stop=0xc9097e0u;if(*pr!=entry_pr)return -3;*stop=0xc9097e2u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_profile_six_0c9097e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc9097e4;
Lc9097e4: *stop=0xc9097e4u;r[0]=0x0c909890u;goto Lc9097e6; /* mova 0x0c909890,r0 */
Lc9097e6: *stop=0xc9097e6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto Lc9097e8; /* fldi1 fr7 */
Lc9097e8: *stop=0xc9097e8u;r[4]=8u;goto Lc9097ea; /* mov #0x8,r4 */
Lc9097ea: *stop=0xc9097eau;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]);goto Lc9097ec; /* fmov.s @r0,fr6 */
Lc9097ec: *stop=0xc9097ecu;r[4]<<=8;goto Lc9097ee; /* shll8 r4 */
Lc9097ee: *stop=0xc9097eeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]^=0x80000000u;goto Lc9097f0; /* fneg fr7 */
Lc9097f0: *stop=0xc9097f0u;r[0]=0x0c909894u;goto Lc9097f2; /* mova 0x0c909894,r0 */
Lc9097f2: *stop=0xc9097f2u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc9097f4; /* fmov.s @r0,fr5 */
Lc9097f4: *stop=0xc9097f4u;r[4]+=16u;goto Lc9097f6; /* add #0x10,r4 */
Lc9097f6: *stop=0xc9097f6u;r[0]=56u;goto Lc9097f8; /* mov #0x38,r0 */
Lc9097f8: *stop=0xc9097f8u;r[4]+=r[5];goto Lc9097fa; /* add r5,r4 */
Lc9097fa: *stop=0xc9097fau;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto Lc9097fc; /* mov.w @(r0,r4),r0 */
Lc9097fc: *stop=0xc9097fcu;r[0]=r[0]&65535u;goto Lc9097fe; /* extu.w r0,r0 */
Lc9097fe: *stop=0xc9097feu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc909800; /* cmp/eq #0x0,r0 */
Lc909800: take=(*sr&1u);*stop=0xc909802u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;if(take)goto Lc909818;goto Lc909804; /* bt/s 0x0c909818 */
Lc909804: *stop=0xc909804u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc909806; /* cmp/eq #0x1,r0 */
Lc909806: if((*sr&1u))goto Lc90984c;goto Lc909808; /* bt 0x0c90984c */
Lc909808: *stop=0xc909808u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto Lc90980a; /* cmp/eq #0x2,r0 */
Lc90980a: if((*sr&1u))goto Lc9098a0;goto Lc90980c; /* bt 0x0c9098a0 */
Lc90980c: *stop=0xc90980cu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto Lc90980e; /* cmp/eq #0x3,r0 */
Lc90980e: if(!(*sr&1u))goto Lc909814;goto Lc909810; /* bf 0x0c909814 */
Lc909810: *stop=0xc909812u;goto Lc909956; /* bra 0x0c909956 */
Lc909814: *stop=0xc909816u;goto Lc90998c; /* bra 0x0c90998c */
Lc909818: *stop=0xc909818u;r[0]=54u;goto Lc90981a; /* mov #0x36,r0 */
Lc90981a: *stop=0xc90981au;r[3]=5u;goto Lc90981c; /* mov #0x5,r3 */
Lc90981c: *stop=0xc90981cu;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc90981e; /* mov.w r3,@(r0,r4) */
Lc90981e: *stop=0xc90981eu;r[0]=68u;goto Lc909820; /* mov #0x44,r0 */
Lc909820: *stop=0xc909820u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909822; /* fmov fr4,@(r0,r4) */
Lc909822: *stop=0xc909822u;r[0]=72u;goto Lc909824; /* mov #0x48,r0 */
Lc909824: *stop=0xc909824u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909826; /* fmov fr4,@(r0,r4) */
Lc909826: *stop=0xc909826u;r[0]=0x0c909898u;goto Lc909828; /* mova 0x0c909898,r0 */
Lc909828: *stop=0xc909828u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc90982a; /* fmov.s @r0,fr3 */
Lc90982a: *stop=0xc90982au;r[0]=76u;goto Lc90982c; /* mov #0x4c,r0 */
Lc90982c: *stop=0xc90982cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc90982e; /* fmov fr3,@(r0,r4) */
Lc90982e: *stop=0xc90982eu;r[0]=84u;goto Lc909830; /* mov #0x54,r0 */
Lc909830: *stop=0xc909830u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909832; /* fmov fr4,@(r0,r4) */
Lc909832: *stop=0xc909832u;r[0]=92u;goto Lc909834; /* mov #0x5c,r0 */
Lc909834: *stop=0xc909834u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909836; /* fmov fr4,@(r0,r4) */
Lc909836: *stop=0xc909836u;r[0]=96u;goto Lc909838; /* mov #0x60,r0 */
Lc909838: *stop=0xc909838u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc90983a; /* fmov fr4,@(r0,r4) */
Lc90983a: *stop=0xc90983au;r[0]=104u;goto Lc90983c; /* mov #0x68,r0 */
Lc90983c: *stop=0xc90983cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc90983e; /* fmov fr4,@(r0,r4) */
Lc90983e: *stop=0xc90983eu;r[0]=108u;goto Lc909840; /* mov #0x6c,r0 */
Lc909840: *stop=0xc909840u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909842; /* fmov fr4,@(r0,r4) */
Lc909842: *stop=0xc909842u;r[0]=56u;goto Lc909844; /* mov #0x38,r0 */
Lc909844: *stop=0xc909844u;if(!valid_ram(r[0]+r[4],2u))return -3;r[3]=sx(m,r[0]+r[4],2);goto Lc909846; /* mov.w @(r0,r4),r3 */
Lc909846: *stop=0xc909846u;r[3]+=1u;goto Lc909848; /* add #0x1,r3 */
Lc909848: *stop=0xc90984au;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc90998c; /* bra 0x0c90998c */
Lc90984c: *stop=0xc90984cu;r[0]=52u;goto Lc90984e; /* mov #0x34,r0 */
Lc90984e: *stop=0xc90984eu;if(!valid_ram(r[0]+r[4],2u))return -3;r[1]=sx(m,r[0]+r[4],2);goto Lc909850; /* mov.w @(r0,r4),r1 */
Lc909850: *stop=0xc909850u;r[3]=10u;goto Lc909852; /* mov #0xa,r3 */
Lc909852: *stop=0xc909852u;r[1]=r[1]&65535u;goto Lc909854; /* extu.w r1,r1 */
Lc909854: *stop=0xc909854u;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[3]);goto Lc909856; /* cmp/gt r3,r1 */
Lc909856: if((*sr&1u))goto Lc90985c;goto Lc909858; /* bt 0x0c90985c */
Lc909858: *stop=0xc90985au;goto Lc90998c; /* bra 0x0c90998c */
Lc90985c: *stop=0xc90985cu;r[0]=56u;goto Lc90985e; /* mov #0x38,r0 */
Lc90985e: *stop=0xc90985eu;if(!valid_ram(r[0]+r[4],2u))return -3;r[1]=sx(m,r[0]+r[4],2);goto Lc909860; /* mov.w @(r0,r4),r1 */
Lc909860: *stop=0xc909860u;r[1]+=1u;goto Lc909862; /* add #0x1,r1 */
Lc909862: *stop=0xc909864u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[1],2);goto Lc90998c; /* bra 0x0c90998c */
Lc9098a0: *stop=0xc9098a0u;r[0]=0x0c909a60u;goto Lc9098a2; /* mova 0x0c909a60,r0 */
Lc9098a2: *stop=0xc9098a2u;r[1]=rd(m,0x0c909a64u,4);goto Lc9098a4; /* mov.l 0x0c909a64,r1 */
Lc9098a4: *stop=0xc9098a4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc9098a6; /* fmov.s @r0,fr3 */
Lc9098a6: *stop=0xc9098a6u;r[0]=92u;goto Lc9098a8; /* mov #0x5c,r0 */
Lc9098a8: *stop=0xc9098a8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto Lc9098aa; /* fmov.s @(r0,r4),fr8 */
Lc9098aa: *stop=0xc9098aau;r[0]=104u;goto Lc9098ac; /* mov #0x68,r0 */
Lc9098ac: *stop=0xc9098acu;*fpul=r[1];goto Lc9098ae; /* lds r1,FPUL */
Lc9098ae: *stop=0xc9098aeu;r[1]=r[0];goto Lc9098b0; /* mov r0,r1 */
Lc9098b0: *stop=0xc9098b0u;r[1]+=r[4];goto Lc9098b2; /* add r4,r1 */
Lc9098b2: *stop=0xc9098b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto Lc9098b4; /* fadd fr3,fr8 */
Lc9098b4: *stop=0xc9098b4u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc9098b6; /* fmov.s @r1,fr1 */
Lc9098b6: *stop=0xc9098b6u;r[1]=rd(m,0x0c909a68u,4);goto Lc9098b8; /* mov.l 0x0c909a68,r1 */
Lc9098b8: *stop=0xc9098b8u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto Lc9098ba; /* fsts FPUL,fr2 */
Lc9098ba: *stop=0xc9098bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc9098bc; /* fmul fr2,fr1 */
Lc9098bc: *stop=0xc9098bcu;*fpul=r[1];goto Lc9098be; /* lds r1,FPUL */
Lc9098be: *stop=0xc9098beu;r[1]=104u;goto Lc9098c0; /* mov #0x68,r1 */
Lc9098c0: *stop=0xc9098c0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc9098c2; /* fmov.s @(r0,r4),fr3 */
Lc9098c2: *stop=0xc9098c2u;r[1]+=r[4];goto Lc9098c4; /* add r4,r1 */
Lc9098c4: *stop=0xc9098c4u;f->bank[(f->fpscr>>21)&1u][0]=*fpul;goto Lc9098c6; /* fsts FPUL,fr0 */
Lc9098c6: *stop=0xc9098c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[0],&f->fpscr,&fr[8]);if(status)return status;}goto Lc9098c8; /* fmul fr0,fr8 */
Lc9098c8: *stop=0xc9098c8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc9098ca; /* fneg fr1 */
Lc9098ca: *stop=0xc9098cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[8],&f->fpscr,&fr[1],1);if(status)return status;}goto Lc9098cc; /* fsub fr8,fr1 */
Lc9098cc: *stop=0xc9098ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc9098ce; /* fadd fr1,fr3 */
Lc9098ce: *stop=0xc9098ceu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc9098d0; /* fmov fr3,@(r0,r4) */
Lc9098d0: *stop=0xc9098d0u;r[0]=92u;goto Lc9098d2; /* mov #0x5c,r0 */
Lc9098d2: *stop=0xc9098d2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9098d4; /* fmov.s @(r0,r4),fr1 */
Lc9098d4: *stop=0xc9098d4u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto Lc9098d6; /* fmov.s @r1,fr3 */
Lc9098d6: *stop=0xc9098d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc9098d8; /* fadd fr3,fr1 */
Lc9098d8: *stop=0xc9098d8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc9098da; /* fmov fr1,@(r0,r4) */
Lc9098da: *stop=0xc9098dau;r[0]=0x0c909a6cu;goto Lc9098dc; /* mova 0x0c909a6c,r0 */
Lc9098dc: *stop=0xc9098dcu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc9098de; /* fmov.s @r0,fr3 */
Lc9098de: *stop=0xc9098deu;r[0]=96u;goto Lc9098e0; /* mov #0x60,r0 */
Lc9098e0: *stop=0xc9098e0u;r[1]=rd(m,0x0c909a70u,4);goto Lc9098e2; /* mov.l 0x0c909a70,r1 */
Lc9098e2: *stop=0xc9098e2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto Lc9098e4; /* fmov.s @(r0,r4),fr8 */
Lc9098e4: *stop=0xc9098e4u;r[0]=108u;goto Lc9098e6; /* mov #0x6c,r0 */
Lc9098e6: *stop=0xc9098e6u;*fpul=r[1];goto Lc9098e8; /* lds r1,FPUL */
Lc9098e8: *stop=0xc9098e8u;r[1]=r[0];goto Lc9098ea; /* mov r0,r1 */
Lc9098ea: *stop=0xc9098eau;r[1]+=r[4];goto Lc9098ec; /* add r4,r1 */
Lc9098ec: *stop=0xc9098ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto Lc9098ee; /* fadd fr3,fr8 */
Lc9098ee: *stop=0xc9098eeu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc9098f0; /* fmov.s @r1,fr1 */
Lc9098f0: *stop=0xc9098f0u;r[1]=108u;goto Lc9098f2; /* mov #0x6c,r1 */
Lc9098f2: *stop=0xc9098f2u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto Lc9098f4; /* fsts FPUL,fr2 */
Lc9098f4: *stop=0xc9098f4u;r[1]+=r[4];goto Lc9098f6; /* add r4,r1 */
Lc9098f6: *stop=0xc9098f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc9098f8; /* fmul fr2,fr1 */
Lc9098f8: *stop=0xc9098f8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc9098fa; /* fmov.s @(r0,r4),fr0 */
Lc9098fa: *stop=0xc9098fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto Lc9098fc; /* fmul fr5,fr8 */
Lc9098fc: *stop=0xc9098fcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc9098fe; /* fneg fr1 */
Lc9098fe: *stop=0xc9098feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[8],&f->fpscr,&fr[1],1);if(status)return status;}goto Lc909900; /* fsub fr8,fr1 */
Lc909900: *stop=0xc909900u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909902; /* fadd fr1,fr0 */
Lc909902: *stop=0xc909902u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909904; /* fmov fr0,@(r0,r4) */
Lc909904: *stop=0xc909904u;r[0]=96u;goto Lc909906; /* mov #0x60,r0 */
Lc909906: *stop=0xc909906u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909908; /* fmov.s @(r0,r4),fr0 */
Lc909908: *stop=0xc909908u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc90990a; /* fmov.s @r1,fr1 */
Lc90990a: *stop=0xc90990au;r[1]=rd(m,0x0c909a74u,4);goto Lc90990c; /* mov.l 0x0c909a74,r1 */
Lc90990c: *stop=0xc90990cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc90990e; /* fadd fr1,fr0 */
Lc90990e: *stop=0xc90990eu;*fpul=r[1];goto Lc909910; /* lds r1,FPUL */
Lc909910: *stop=0xc909910u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909912; /* fmov fr0,@(r0,r4) */
Lc909912: *stop=0xc909912u;r[0]=72u;goto Lc909914; /* mov #0x48,r0 */
Lc909914: *stop=0xc909914u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto Lc909916; /* fmov.s @(r0,r4),fr8 */
Lc909916: *stop=0xc909916u;r[0]=84u;goto Lc909918; /* mov #0x54,r0 */
Lc909918: *stop=0xc909918u;r[1]=r[0];goto Lc90991a; /* mov r0,r1 */
Lc90991a: *stop=0xc90991au;r[1]+=r[4];goto Lc90991c; /* add r4,r1 */
Lc90991c: *stop=0xc90991cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[7],&f->fpscr,&fr[8],0);if(status)return status;}goto Lc90991e; /* fadd fr7,fr8 */
Lc90991e: *stop=0xc90991eu;f->bank[(f->fpscr>>21)&1u][1]=*fpul;goto Lc909920; /* fsts FPUL,fr1 */
Lc909920: *stop=0xc909920u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[1]);goto Lc909922; /* fmov.s @r1,fr0 */
Lc909922: *stop=0xc909922u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[6],&f->fpscr,&fr[8]);if(status)return status;}goto Lc909924; /* fmul fr6,fr8 */
Lc909924: *stop=0xc909924u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc909926; /* fmov.s @(r0,r4),fr3 */
Lc909926: *stop=0xc909926u;r[1]=84u;goto Lc909928; /* mov #0x54,r1 */
Lc909928: *stop=0xc909928u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[1],&f->fpscr,&fr[0]);if(status)return status;}goto Lc90992a; /* fmul fr1,fr0 */
Lc90992a: *stop=0xc90992au;r[1]+=r[4];goto Lc90992c; /* add r4,r1 */
Lc90992c: *stop=0xc90992cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto Lc90992e; /* fneg fr0 */
Lc90992e: *stop=0xc90992eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[8],&f->fpscr,&fr[0],1);if(status)return status;}goto Lc909930; /* fsub fr8,fr0 */
Lc909930: *stop=0xc909930u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[0],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc909932; /* fadd fr0,fr3 */
Lc909932: *stop=0xc909932u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc909934; /* fmov fr3,@(r0,r4) */
Lc909934: *stop=0xc909934u;r[0]=72u;goto Lc909936; /* mov #0x48,r0 */
Lc909936: *stop=0xc909936u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909938; /* fmov.s @(r0,r4),fr0 */
Lc909938: *stop=0xc909938u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto Lc90993a; /* fmov.s @r1,fr3 */
Lc90993a: *stop=0xc90993au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc90993c; /* fadd fr3,fr0 */
Lc90993c: *stop=0xc90993cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc90993e; /* fmov fr0,@(r0,r4) */
Lc90993e: *stop=0xc90993eu;if(!valid_ram(r[4]+4u,2u))return -3;r[0]=sx(m,r[4]+0x4u,2);goto Lc909940; /* mov.w @(0x4,r4),r0 */
Lc909940: *stop=0xc909940u;r[0]+=4294967295u;goto Lc909942; /* add #-0x1,r0 */
Lc909942: *stop=0xc909942u;if(!valid_ram(r[4]+4u,2u))return -3;wr(m,r[4]+0x4u,r[0],2);goto Lc909944; /* mov.w r0,@(0x4,r4) */
Lc909944: *stop=0xc909944u;r[0]+=1u;goto Lc909946; /* add #0x1,r0 */
Lc909946: *stop=0xc909946u;r[0]=r[0]&65535u;goto Lc909948; /* extu.w r0,r0 */
Lc909948: *stop=0xc909948u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc90994a; /* tst r0,r0 */
Lc90994a: if(!(*sr&1u))goto Lc90998c;goto Lc90994c; /* bf 0x0c90998c */
Lc90994c: *stop=0xc90994cu;r[0]=56u;goto Lc90994e; /* mov #0x38,r0 */
Lc90994e: *stop=0xc90994eu;if(!valid_ram(r[0]+r[4],2u))return -3;r[2]=sx(m,r[0]+r[4],2);goto Lc909950; /* mov.w @(r0,r4),r2 */
Lc909950: *stop=0xc909950u;r[2]+=1u;goto Lc909952; /* add #0x1,r2 */
Lc909952: *stop=0xc909954u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[2],2);goto Lc90998c; /* bra 0x0c90998c */
Lc909956: *stop=0xc909956u;r[0]=92u;goto Lc909958; /* mov #0x5c,r0 */
Lc909958: *stop=0xc909958u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc90995a; /* fldi0 fr2 */
Lc90995a: *stop=0xc90995au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc90995c; /* fmov.s @(r0,r4),fr3 */
Lc90995c: *stop=0xc90995cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc90995e; /* fsub fr6,fr3 */
Lc90995e: *stop=0xc90995eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc909960; /* fcmp/gt fr3,fr2 */
Lc909960: take=!(*sr&1u);*stop=0xc909962u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc909966;goto Lc909964; /* bf/s 0x0c909966 */
Lc909964: *stop=0xc909964u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909966; /* fmov fr4,@(r0,r4) */
Lc909966: *stop=0xc909966u;r[0]=96u;goto Lc909968; /* mov #0x60,r0 */
Lc909968: *stop=0xc909968u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc90996a; /* fldi0 fr2 */
Lc90996a: *stop=0xc90996au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc90996c; /* fmov.s @(r0,r4),fr3 */
Lc90996c: *stop=0xc90996cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc90996e; /* fsub fr6,fr3 */
Lc90996e: *stop=0xc90996eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc909970; /* fcmp/gt fr3,fr2 */
Lc909970: take=!(*sr&1u);*stop=0xc909972u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc90998c;goto Lc909974; /* bf/s 0x0c90998c */
Lc909974: *stop=0xc909974u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909976; /* fmov fr4,@(r0,r4) */
Lc909976: *stop=0xc909976u;r[0]=8u;goto Lc909978; /* mov #0x8,r0 */
Lc909978: *stop=0xc909978u;r[0]<<=8;goto Lc90997a; /* shll8 r0 */
Lc90997a: *stop=0xc90997au;r[0]+=18u;goto Lc90997c; /* add #0x12,r0 */
Lc90997c: *stop=0xc90997cu;r[6]=0u;goto Lc90997e; /* mov #0x0,r6 */
Lc90997e: *stop=0xc90997eu;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909980; /* mov.w r6,@(r0,r5) */
Lc909980: *stop=0xc909980u;r[0]+=54u;goto Lc909982; /* add #0x36,r0 */
Lc909982: *stop=0xc909982u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909984; /* mov.w r6,@(r0,r5) */
Lc909984: *stop=0xc909984u;r[0]+=64u;goto Lc909986; /* add #0x40,r0 */
Lc909986: *stop=0xc909986u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909988; /* mov.w r6,@(r0,r5) */
Lc909988: *stop=0xc909988u;r[0]+=4294967180u;goto Lc90998a; /* add #-0x74,r0 */
Lc90998a: *stop=0xc90998au;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc90998c; /* mov.w r6,@(r0,r5) */
Lc90998c: *stop=0xc90998cu;r[0]=120u;goto Lc90998e; /* mov #0x78,r0 */
Lc90998e: *stop=0xc90998eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto Lc909990; /* fldi1 fr6 */
Lc909990: *stop=0xc909990u;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto Lc909992; /* mov.w @(r0,r4),r0 */
Lc909992: *stop=0xc909992u;r[0]=r[0]&65535u;goto Lc909994; /* extu.w r0,r0 */
Lc909994: *stop=0xc909994u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc909996; /* cmp/eq #0x0,r0 */
Lc909996: take=(*sr&1u);*stop=0xc909998u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[6],&f->fpscr,&fr[6],0);if(status)return status;}if(take)goto Lc9099a6;goto Lc90999a; /* bt/s 0x0c9099a6 */
Lc90999a: *stop=0xc90999au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc90999c; /* cmp/eq #0x1,r0 */
Lc90999c: if((*sr&1u))goto Lc9099cc;goto Lc90999e; /* bt 0x0c9099cc */
Lc90999e: *stop=0xc90999eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto Lc9099a0; /* cmp/eq #0x2,r0 */
Lc9099a0: if((*sr&1u))goto Lc9099fe;goto Lc9099a2; /* bt 0x0c9099fe */
Lc9099a2: *stop=0xc9099a2u;if(*pr!=entry_pr)return -3;*stop=0xc9099a4u;return 0; /* rts */
Lc9099a6: *stop=0xc9099a6u;r[0]=118u;goto Lc9099a8; /* mov #0x76,r0 */
Lc9099a8: *stop=0xc9099a8u;r[3]=1u;goto Lc9099aa; /* mov #0x1,r3 */
Lc9099aa: *stop=0xc9099aau;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc9099ac; /* mov.w r3,@(r0,r4) */
Lc9099ac: *stop=0xc9099acu;r[0]+=14u;goto Lc9099ae; /* add #0xe,r0 */
Lc9099ae: *stop=0xc9099aeu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9099b0; /* fmov fr4,@(r0,r4) */
Lc9099b0: *stop=0xc9099b0u;r[0]+=4u;goto Lc9099b2; /* add #0x4,r0 */
Lc9099b2: *stop=0xc9099b2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9099b4; /* fmov fr4,@(r0,r4) */
Lc9099b4: *stop=0xc9099b4u;r[0]+=4u;goto Lc9099b6; /* add #0x4,r0 */
Lc9099b6: *stop=0xc9099b6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9099b8; /* fmov fr4,@(r0,r4) */
Lc9099b8: *stop=0xc9099b8u;r[0]+=16u;goto Lc9099ba; /* add #0x10,r0 */
Lc9099ba: *stop=0xc9099bau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto Lc9099bc; /* fldi1 fr4 */
Lc9099bc: *stop=0xc9099bcu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9099be; /* fmov fr4,@(r0,r4) */
Lc9099be: *stop=0xc9099beu;r[0]+=4u;goto Lc9099c0; /* add #0x4,r0 */
Lc9099c0: *stop=0xc9099c0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc9099c2; /* fmov fr4,@(r0,r4) */
Lc9099c2: *stop=0xc9099c2u;r[0]=120u;goto Lc9099c4; /* mov #0x78,r0 */
Lc9099c4: *stop=0xc9099c4u;if(!valid_ram(r[0]+r[4],2u))return -3;r[3]=sx(m,r[0]+r[4],2);goto Lc9099c6; /* mov.w @(r0,r4),r3 */
Lc9099c6: *stop=0xc9099c6u;r[3]+=1u;goto Lc9099c8; /* add #0x1,r3 */
Lc9099c8: *stop=0xc9099c8u;if(*pr!=entry_pr)return -3;*stop=0xc9099cau;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);return 0; /* rts */
Lc9099cc: *stop=0xc9099ccu;r[1]=rd(m,0x0c909a78u,4);goto Lc9099ce; /* mov.l 0x0c909a78,r1 */
Lc9099ce: *stop=0xc9099ceu;r[0]=4294967196u;goto Lc9099d0; /* mov #-0x64,r0 */
Lc9099d0: *stop=0xc9099d0u;r[0]=r[0]&255u;goto Lc9099d2; /* extu.b r0,r0 */
Lc9099d2: *stop=0xc9099d2u;*fpul=r[1];goto Lc9099d4; /* lds r1,FPUL */
Lc9099d4: *stop=0xc9099d4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc9099d6; /* fmov.s @(r0,r4),fr2 */
Lc9099d6: *stop=0xc9099d6u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc9099d8; /* fsts FPUL,fr3 */
Lc9099d8: *stop=0xc9099d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc9099da; /* fsub fr3,fr2 */
Lc9099da: *stop=0xc9099dau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc9099dc; /* fmov fr2,@(r0,r4) */
Lc9099dc: *stop=0xc9099dcu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc9099de; /* fmov.s @(r0,r4),fr2 */
Lc9099de: *stop=0xc9099deu;r[0]+=4u;goto Lc9099e0; /* add #0x4,r0 */
Lc9099e0: *stop=0xc9099e0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc9099e2; /* fneg fr2 */
Lc9099e2: *stop=0xc9099e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc9099e4; /* fadd fr6,fr2 */
Lc9099e4: *stop=0xc9099e4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc9099e6; /* fmov fr2,@(r0,r4) */
Lc9099e6: *stop=0xc9099e6u;r[0]=0x0c909a7cu;goto Lc9099e8; /* mova 0x0c909a7c,r0 */
Lc9099e8: *stop=0xc9099e8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto Lc9099ea; /* fmov.s @r0,fr2 */
Lc9099ea: *stop=0xc9099eau;r[0]=4294967196u;goto Lc9099ec; /* mov #-0x64,r0 */
Lc9099ec: *stop=0xc9099ecu;r[0]=r[0]&255u;goto Lc9099ee; /* extu.b r0,r0 */
Lc9099ee: *stop=0xc9099eeu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc9099f0; /* fmov.s @(r0,r4),fr1 */
Lc9099f0: *stop=0xc9099f0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto Lc9099f2; /* fcmp/gt fr1,fr2 */
Lc9099f2: if(!(*sr&1u))goto Lc909a3e;goto Lc9099f4; /* bf 0x0c909a3e */
Lc9099f4: *stop=0xc9099f4u;r[0]=120u;goto Lc9099f6; /* mov #0x78,r0 */
Lc9099f6: *stop=0xc9099f6u;if(!valid_ram(r[0]+r[4],2u))return -3;r[2]=sx(m,r[0]+r[4],2);goto Lc9099f8; /* mov.w @(r0,r4),r2 */
Lc9099f8: *stop=0xc9099f8u;r[2]+=1u;goto Lc9099fa; /* add #0x1,r2 */
Lc9099fa: *stop=0xc9099fau;if(*pr!=entry_pr)return -3;*stop=0xc9099fcu;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[2],2);return 0; /* rts */
Lc9099fe: *stop=0xc9099feu;r[1]=rd(m,0x0c909a80u,4);goto Lc909a00; /* mov.l 0x0c909a80,r1 */
Lc909a00: *stop=0xc909a00u;r[0]=4294967196u;goto Lc909a02; /* mov #-0x64,r0 */
Lc909a02: *stop=0xc909a02u;r[0]=r[0]&255u;goto Lc909a04; /* extu.b r0,r0 */
Lc909a04: *stop=0xc909a04u;*fpul=r[1];goto Lc909a06; /* lds r1,FPUL */
Lc909a06: *stop=0xc909a06u;r[1]=4294967208u;goto Lc909a08; /* mov #-0x58,r1 */
Lc909a08: *stop=0xc909a08u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[4]);goto Lc909a0a; /* fmov.s @(r0,r4),fr4 */
Lc909a0a: *stop=0xc909a0au;r[1]=r[1]&255u;goto Lc909a0c; /* extu.b r1,r1 */
Lc909a0c: *stop=0xc909a0cu;r[1]+=r[4];goto Lc909a0e; /* add r4,r1 */
Lc909a0e: *stop=0xc909a0eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[7],&f->fpscr,&fr[4],0);if(status)return status;}goto Lc909a10; /* fadd fr7,fr4 */
Lc909a10: *stop=0xc909a10u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc909a12; /* fmov.s @r1,fr2 */
Lc909a12: *stop=0xc909a12u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc909a14; /* fsts FPUL,fr3 */
Lc909a14: *stop=0xc909a14u;r[0]+=12u;goto Lc909a16; /* add #0xc,r0 */
Lc909a16: *stop=0xc909a16u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto Lc909a18; /* fmul fr3,fr2 */
Lc909a18: *stop=0xc909a18u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc909a1a; /* fmov.s @(r0,r4),fr1 */
Lc909a1a: *stop=0xc909a1au;r[1]=4294967208u;goto Lc909a1c; /* mov #-0x58,r1 */
Lc909a1c: *stop=0xc909a1cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[5],&f->fpscr,&fr[4]);if(status)return status;}goto Lc909a1e; /* fmul fr5,fr4 */
Lc909a1e: *stop=0xc909a1eu;r[1]=r[1]&255u;goto Lc909a20; /* extu.b r1,r1 */
Lc909a20: *stop=0xc909a20u;r[1]+=r[4];goto Lc909a22; /* add r4,r1 */
Lc909a22: *stop=0xc909a22u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc909a24; /* fneg fr2 */
Lc909a24: *stop=0xc909a24u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc909a26; /* fsub fr4,fr2 */
Lc909a26: *stop=0xc909a26u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc909a28; /* fadd fr2,fr1 */
Lc909a28: *stop=0xc909a28u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc909a2a; /* fmov fr1,@(r0,r4) */
Lc909a2a: *stop=0xc909a2au;r[0]+=4294967284u;goto Lc909a2c; /* add #-0xc,r0 */
Lc909a2c: *stop=0xc909a2cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc909a2e; /* fmov.s @(r0,r4),fr1 */
Lc909a2e: *stop=0xc909a2eu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc909a30; /* fmov.s @r1,fr2 */
Lc909a30: *stop=0xc909a30u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc909a32; /* fadd fr2,fr1 */
Lc909a32: *stop=0xc909a32u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc909a34; /* fmov fr1,@(r0,r4) */
Lc909a34: *stop=0xc909a34u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc909a36; /* fmov.s @(r0,r4),fr2 */
Lc909a36: *stop=0xc909a36u;r[0]+=4u;goto Lc909a38; /* add #0x4,r0 */
Lc909a38: *stop=0xc909a38u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc909a3a; /* fneg fr2 */
Lc909a3a: *stop=0xc909a3au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc909a3c; /* fadd fr6,fr2 */
Lc909a3c: *stop=0xc909a3cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc909a3e; /* fmov fr2,@(r0,r4) */
Lc909a3e: *stop=0xc909a3eu;if(*pr!=entry_pr)return -3;*stop=0xc909a40u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_profile_seven_0c909aa0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc909aa0;
Lc909aa0: *stop=0xc909aa0u;r[0]=0x0c909c60u;goto Lc909aa2; /* mova 0x0c909c60,r0 */
Lc909aa2: *stop=0xc909aa2u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]);goto Lc909aa4; /* fmov.s @r0,fr6 */
Lc909aa4: *stop=0xc909aa4u;r[4]=8u;goto Lc909aa6; /* mov #0x8,r4 */
Lc909aa6: *stop=0xc909aa6u;r[0]=0x0c909c64u;goto Lc909aa8; /* mova 0x0c909c64,r0 */
Lc909aa8: *stop=0xc909aa8u;r[4]<<=8;goto Lc909aaa; /* shll8 r4 */
Lc909aaa: *stop=0xc909aaau;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc909aac; /* fmov.s @r0,fr5 */
Lc909aac: *stop=0xc909aacu;r[4]+=16u;goto Lc909aae; /* add #0x10,r4 */
Lc909aae: *stop=0xc909aaeu;r[0]=56u;goto Lc909ab0; /* mov #0x38,r0 */
Lc909ab0: *stop=0xc909ab0u;r[4]+=r[5];goto Lc909ab2; /* add r5,r4 */
Lc909ab2: *stop=0xc909ab2u;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto Lc909ab4; /* mov.w @(r0,r4),r0 */
Lc909ab4: *stop=0xc909ab4u;r[0]=r[0]&65535u;goto Lc909ab6; /* extu.w r0,r0 */
Lc909ab6: *stop=0xc909ab6u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc909ab8; /* cmp/eq #0x0,r0 */
Lc909ab8: take=(*sr&1u);*stop=0xc909abau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;if(take)goto Lc909ad0;goto Lc909abc; /* bt/s 0x0c909ad0 */
Lc909abc: *stop=0xc909abcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc909abe; /* cmp/eq #0x1,r0 */
Lc909abe: if((*sr&1u))goto Lc909b04;goto Lc909ac0; /* bt 0x0c909b04 */
Lc909ac0: *stop=0xc909ac0u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto Lc909ac2; /* cmp/eq #0x2,r0 */
Lc909ac2: if((*sr&1u))goto Lc909b1e;goto Lc909ac4; /* bt 0x0c909b1e */
Lc909ac4: *stop=0xc909ac4u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto Lc909ac6; /* cmp/eq #0x3,r0 */
Lc909ac6: if(!(*sr&1u))goto Lc909acc;goto Lc909ac8; /* bf 0x0c909acc */
Lc909ac8: *stop=0xc909acau;goto Lc909c02; /* bra 0x0c909c02 */
Lc909acc: *stop=0xc909aceu;goto Lc909c38; /* bra 0x0c909c38 */
Lc909ad0: *stop=0xc909ad0u;r[0]=54u;goto Lc909ad2; /* mov #0x36,r0 */
Lc909ad2: *stop=0xc909ad2u;r[3]=6u;goto Lc909ad4; /* mov #0x6,r3 */
Lc909ad4: *stop=0xc909ad4u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc909ad6; /* mov.w r3,@(r0,r4) */
Lc909ad6: *stop=0xc909ad6u;r[0]=68u;goto Lc909ad8; /* mov #0x44,r0 */
Lc909ad8: *stop=0xc909ad8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909ada; /* fmov fr4,@(r0,r4) */
Lc909ada: *stop=0xc909adau;r[0]=72u;goto Lc909adc; /* mov #0x48,r0 */
Lc909adc: *stop=0xc909adcu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909ade; /* fmov fr4,@(r0,r4) */
Lc909ade: *stop=0xc909adeu;r[0]=0x0c909c68u;goto Lc909ae0; /* mova 0x0c909c68,r0 */
Lc909ae0: *stop=0xc909ae0u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc909ae2; /* fmov.s @r0,fr3 */
Lc909ae2: *stop=0xc909ae2u;r[0]=76u;goto Lc909ae4; /* mov #0x4c,r0 */
Lc909ae4: *stop=0xc909ae4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc909ae6; /* fmov fr3,@(r0,r4) */
Lc909ae6: *stop=0xc909ae6u;r[0]=84u;goto Lc909ae8; /* mov #0x54,r0 */
Lc909ae8: *stop=0xc909ae8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909aea; /* fmov fr4,@(r0,r4) */
Lc909aea: *stop=0xc909aeau;r[0]=92u;goto Lc909aec; /* mov #0x5c,r0 */
Lc909aec: *stop=0xc909aecu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909aee; /* fmov fr4,@(r0,r4) */
Lc909aee: *stop=0xc909aeeu;r[0]=96u;goto Lc909af0; /* mov #0x60,r0 */
Lc909af0: *stop=0xc909af0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909af2; /* fmov fr4,@(r0,r4) */
Lc909af2: *stop=0xc909af2u;r[0]=104u;goto Lc909af4; /* mov #0x68,r0 */
Lc909af4: *stop=0xc909af4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909af6; /* fmov fr4,@(r0,r4) */
Lc909af6: *stop=0xc909af6u;r[0]=108u;goto Lc909af8; /* mov #0x6c,r0 */
Lc909af8: *stop=0xc909af8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909afa; /* fmov fr4,@(r0,r4) */
Lc909afa: *stop=0xc909afau;r[0]=56u;goto Lc909afc; /* mov #0x38,r0 */
Lc909afc: *stop=0xc909afcu;if(!valid_ram(r[0]+r[4],2u))return -3;r[3]=sx(m,r[0]+r[4],2);goto Lc909afe; /* mov.w @(r0,r4),r3 */
Lc909afe: *stop=0xc909afeu;r[3]+=1u;goto Lc909b00; /* add #0x1,r3 */
Lc909b00: *stop=0xc909b02u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc909c38; /* bra 0x0c909c38 */
Lc909b04: *stop=0xc909b04u;r[0]=52u;goto Lc909b06; /* mov #0x34,r0 */
Lc909b06: *stop=0xc909b06u;if(!valid_ram(r[0]+r[4],2u))return -3;r[1]=sx(m,r[0]+r[4],2);goto Lc909b08; /* mov.w @(r0,r4),r1 */
Lc909b08: *stop=0xc909b08u;r[3]=10u;goto Lc909b0a; /* mov #0xa,r3 */
Lc909b0a: *stop=0xc909b0au;r[1]=r[1]&65535u;goto Lc909b0c; /* extu.w r1,r1 */
Lc909b0c: *stop=0xc909b0cu;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[3]);goto Lc909b0e; /* cmp/gt r3,r1 */
Lc909b0e: if((*sr&1u))goto Lc909b14;goto Lc909b10; /* bt 0x0c909b14 */
Lc909b10: *stop=0xc909b12u;goto Lc909c38; /* bra 0x0c909c38 */
Lc909b14: *stop=0xc909b14u;r[0]=56u;goto Lc909b16; /* mov #0x38,r0 */
Lc909b16: *stop=0xc909b16u;if(!valid_ram(r[0]+r[4],2u))return -3;r[1]=sx(m,r[0]+r[4],2);goto Lc909b18; /* mov.w @(r0,r4),r1 */
Lc909b18: *stop=0xc909b18u;r[1]+=1u;goto Lc909b1a; /* add #0x1,r1 */
Lc909b1a: *stop=0xc909b1cu;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[1],2);goto Lc909c38; /* bra 0x0c909c38 */
Lc909b1e: *stop=0xc909b1eu;r[0]=0x0c909c6cu;goto Lc909b20; /* mova 0x0c909c6c,r0 */
Lc909b20: *stop=0xc909b20u;r[1]=rd(m,0x0c909c70u,4);goto Lc909b22; /* mov.l 0x0c909c70,r1 */
Lc909b22: *stop=0xc909b22u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]);goto Lc909b24; /* fmov.s @r0,fr7 */
Lc909b24: *stop=0xc909b24u;r[0]=92u;goto Lc909b26; /* mov #0x5c,r0 */
Lc909b26: *stop=0xc909b26u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto Lc909b28; /* fmov.s @(r0,r4),fr8 */
Lc909b28: *stop=0xc909b28u;r[0]=104u;goto Lc909b2a; /* mov #0x68,r0 */
Lc909b2a: *stop=0xc909b2au;*fpul=r[1];goto Lc909b2c; /* lds r1,FPUL */
Lc909b2c: *stop=0xc909b2cu;r[1]=r[0];goto Lc909b2e; /* mov r0,r1 */
Lc909b2e: *stop=0xc909b2eu;r[1]+=r[4];goto Lc909b30; /* add r4,r1 */
Lc909b30: *stop=0xc909b30u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[7],&f->fpscr,&fr[8],0);if(status)return status;}goto Lc909b32; /* fadd fr7,fr8 */
Lc909b32: *stop=0xc909b32u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc909b34; /* fmov.s @r1,fr2 */
Lc909b34: *stop=0xc909b34u;r[1]=rd(m,0x0c909c74u,4);goto Lc909b36; /* mov.l 0x0c909c74,r1 */
Lc909b36: *stop=0xc909b36u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc909b38; /* fsts FPUL,fr3 */
Lc909b38: *stop=0xc909b38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto Lc909b3a; /* fmul fr3,fr2 */
Lc909b3a: *stop=0xc909b3au;*fpul=r[1];goto Lc909b3c; /* lds r1,FPUL */
Lc909b3c: *stop=0xc909b3cu;r[1]=104u;goto Lc909b3e; /* mov #0x68,r1 */
Lc909b3e: *stop=0xc909b3eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909b40; /* fmov.s @(r0,r4),fr0 */
Lc909b40: *stop=0xc909b40u;r[1]+=r[4];goto Lc909b42; /* add r4,r1 */
Lc909b42: *stop=0xc909b42u;f->bank[(f->fpscr>>21)&1u][1]=*fpul;goto Lc909b44; /* fsts FPUL,fr1 */
Lc909b44: *stop=0xc909b44u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[1],&f->fpscr,&fr[8]);if(status)return status;}goto Lc909b46; /* fmul fr1,fr8 */
Lc909b46: *stop=0xc909b46u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc909b48; /* fneg fr2 */
Lc909b48: *stop=0xc909b48u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[8],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc909b4a; /* fsub fr8,fr2 */
Lc909b4a: *stop=0xc909b4au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[2],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909b4c; /* fadd fr2,fr0 */
Lc909b4c: *stop=0xc909b4cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909b4e; /* fmov fr0,@(r0,r4) */
Lc909b4e: *stop=0xc909b4eu;r[0]=92u;goto Lc909b50; /* mov #0x5c,r0 */
Lc909b50: *stop=0xc909b50u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909b52; /* fmov.s @(r0,r4),fr0 */
Lc909b52: *stop=0xc909b52u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc909b54; /* fmov.s @r1,fr2 */
Lc909b54: *stop=0xc909b54u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[2],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909b56; /* fadd fr2,fr0 */
Lc909b56: *stop=0xc909b56u;fcopy(f,2u,7u);goto Lc909b58; /* fmov fr7,fr2 */
Lc909b58: *stop=0xc909b58u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909b5a; /* fmov fr0,@(r0,r4) */
Lc909b5a: *stop=0xc909b5au;r[0]=96u;goto Lc909b5c; /* mov #0x60,r0 */
Lc909b5c: *stop=0xc909b5cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]+r[4]);goto Lc909b5e; /* fmov.s @(r0,r4),fr7 */
Lc909b5e: *stop=0xc909b5eu;r[1]=rd(m,0x0c909c78u,4);goto Lc909b60; /* mov.l 0x0c909c78,r1 */
Lc909b60: *stop=0xc909b60u;r[0]=108u;goto Lc909b62; /* mov #0x6c,r0 */
Lc909b62: *stop=0xc909b62u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[2],&f->fpscr,&fr[7],0);if(status)return status;}goto Lc909b64; /* fadd fr2,fr7 */
Lc909b64: *stop=0xc909b64u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc909b66; /* fmov.s @(r0,r4),fr3 */
Lc909b66: *stop=0xc909b66u;*fpul=r[1];goto Lc909b68; /* lds r1,FPUL */
Lc909b68: *stop=0xc909b68u;r[1]=r[0];goto Lc909b6a; /* mov r0,r1 */
Lc909b6a: *stop=0xc909b6au;r[1]+=r[4];goto Lc909b6c; /* add r4,r1 */
Lc909b6c: *stop=0xc909b6cu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[1]);goto Lc909b6e; /* fmov.s @r1,fr0 */
Lc909b6e: *stop=0xc909b6eu;r[1]=108u;goto Lc909b70; /* mov #0x6c,r1 */
Lc909b70: *stop=0xc909b70u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto Lc909b72; /* fsts FPUL,fr2 */
Lc909b72: *stop=0xc909b72u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[6],&f->fpscr,&fr[7]);if(status)return status;}goto Lc909b74; /* fmul fr6,fr7 */
Lc909b74: *stop=0xc909b74u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[2],&f->fpscr,&fr[0]);if(status)return status;}goto Lc909b76; /* fmul fr2,fr0 */
Lc909b76: *stop=0xc909b76u;r[1]+=r[4];goto Lc909b78; /* add r4,r1 */
Lc909b78: *stop=0xc909b78u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto Lc909b7a; /* fneg fr0 */
Lc909b7a: *stop=0xc909b7au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[7],&f->fpscr,&fr[0],1);if(status)return status;}goto Lc909b7c; /* fsub fr7,fr0 */
Lc909b7c: *stop=0xc909b7cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[0],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc909b7e; /* fadd fr0,fr3 */
Lc909b7e: *stop=0xc909b7eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc909b80; /* fmov fr3,@(r0,r4) */
Lc909b80: *stop=0xc909b80u;r[0]=96u;goto Lc909b82; /* mov #0x60,r0 */
Lc909b82: *stop=0xc909b82u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909b84; /* fmov.s @(r0,r4),fr0 */
Lc909b84: *stop=0xc909b84u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto Lc909b86; /* fmov.s @r1,fr3 */
Lc909b86: *stop=0xc909b86u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909b88; /* fadd fr3,fr0 */
Lc909b88: *stop=0xc909b88u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909b8a; /* fmov fr0,@(r0,r4) */
Lc909b8a: *stop=0xc909b8au;r[0]=0x0c909c7cu;goto Lc909b8c; /* mova 0x0c909c7c,r0 */
Lc909b8c: *stop=0xc909b8cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc909b8e; /* fmov.s @r0,fr3 */
Lc909b8e: *stop=0xc909b8eu;r[0]=72u;goto Lc909b90; /* mov #0x48,r0 */
Lc909b90: *stop=0xc909b90u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto Lc909b92; /* fmov.s @(r0,r4),fr8 */
Lc909b92: *stop=0xc909b92u;r[0]=0x0c909c80u;goto Lc909b94; /* mova 0x0c909c80,r0 */
Lc909b94: *stop=0xc909b94u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]);goto Lc909b96; /* fmov.s @r0,fr7 */
Lc909b96: *stop=0xc909b96u;r[0]=84u;goto Lc909b98; /* mov #0x54,r0 */
Lc909b98: *stop=0xc909b98u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto Lc909b9a; /* fadd fr3,fr8 */
Lc909b9a: *stop=0xc909b9au;r[1]=r[0];goto Lc909b9c; /* mov r0,r1 */
Lc909b9c: *stop=0xc909b9cu;r[1]+=r[4];goto Lc909b9e; /* add r4,r1 */
Lc909b9e: *stop=0xc909b9eu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc909ba0; /* fmov.s @r1,fr1 */
Lc909ba0: *stop=0xc909ba0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto Lc909ba2; /* fmul fr5,fr8 */
Lc909ba2: *stop=0xc909ba2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909ba4; /* fmov.s @(r0,r4),fr0 */
Lc909ba4: *stop=0xc909ba4u;r[1]=84u;goto Lc909ba6; /* mov #0x54,r1 */
Lc909ba6: *stop=0xc909ba6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto Lc909ba8; /* fmul fr7,fr1 */
Lc909ba8: *stop=0xc909ba8u;r[1]+=r[4];goto Lc909baa; /* add r4,r1 */
Lc909baa: *stop=0xc909baau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc909bac; /* fneg fr1 */
Lc909bac: *stop=0xc909bacu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[8],&f->fpscr,&fr[1],1);if(status)return status;}goto Lc909bae; /* fsub fr8,fr1 */
Lc909bae: *stop=0xc909baeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909bb0; /* fadd fr1,fr0 */
Lc909bb0: *stop=0xc909bb0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909bb2; /* fmov fr0,@(r0,r4) */
Lc909bb2: *stop=0xc909bb2u;r[0]=72u;goto Lc909bb4; /* mov #0x48,r0 */
Lc909bb4: *stop=0xc909bb4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909bb6; /* fmov.s @(r0,r4),fr0 */
Lc909bb6: *stop=0xc909bb6u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc909bb8; /* fmov.s @r1,fr1 */
Lc909bb8: *stop=0xc909bb8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909bba; /* fadd fr1,fr0 */
Lc909bba: *stop=0xc909bbau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909bbc; /* fmov fr0,@(r0,r4) */
Lc909bbc: *stop=0xc909bbcu;r[0]=0x0c909c84u;goto Lc909bbe; /* mova 0x0c909c84,r0 */
Lc909bbe: *stop=0xc909bbeu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto Lc909bc0; /* fmov.s @r0,fr1 */
Lc909bc0: *stop=0xc909bc0u;r[0]=68u;goto Lc909bc2; /* mov #0x44,r0 */
Lc909bc2: *stop=0xc909bc2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto Lc909bc4; /* fmov.s @(r0,r4),fr8 */
Lc909bc4: *stop=0xc909bc4u;r[0]=84u;goto Lc909bc6; /* mov #0x54,r0 */
Lc909bc6: *stop=0xc909bc6u;r[1]=r[0];goto Lc909bc8; /* mov r0,r1 */
Lc909bc8: *stop=0xc909bc8u;r[1]+=r[4];goto Lc909bca; /* add r4,r1 */
Lc909bca: *stop=0xc909bcau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[1],&f->fpscr,&fr[8],0);if(status)return status;}goto Lc909bcc; /* fadd fr1,fr8 */
Lc909bcc: *stop=0xc909bccu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc909bce; /* fmov.s @r1,fr2 */
Lc909bce: *stop=0xc909bceu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909bd0; /* fmov.s @(r0,r4),fr0 */
Lc909bd0: *stop=0xc909bd0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto Lc909bd2; /* fmul fr7,fr2 */
Lc909bd2: *stop=0xc909bd2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto Lc909bd4; /* fmul fr5,fr8 */
Lc909bd4: *stop=0xc909bd4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc909bd6; /* fneg fr2 */
Lc909bd6: *stop=0xc909bd6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[8],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc909bd8; /* fsub fr8,fr2 */
Lc909bd8: *stop=0xc909bd8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[2],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909bda; /* fadd fr2,fr0 */
Lc909bda: *stop=0xc909bdau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909bdc; /* fmov fr0,@(r0,r4) */
Lc909bdc: *stop=0xc909bdcu;r[0]=72u;goto Lc909bde; /* mov #0x48,r0 */
Lc909bde: *stop=0xc909bdeu;r[1]=84u;goto Lc909be0; /* mov #0x54,r1 */
Lc909be0: *stop=0xc909be0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909be2; /* fmov.s @(r0,r4),fr0 */
Lc909be2: *stop=0xc909be2u;r[1]+=r[4];goto Lc909be4; /* add r4,r1 */
Lc909be4: *stop=0xc909be4u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto Lc909be6; /* fmov.s @r1,fr2 */
Lc909be6: *stop=0xc909be6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[2],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909be8; /* fadd fr2,fr0 */
Lc909be8: *stop=0xc909be8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909bea; /* fmov fr0,@(r0,r4) */
Lc909bea: *stop=0xc909beau;if(!valid_ram(r[4]+4u,2u))return -3;r[0]=sx(m,r[4]+0x4u,2);goto Lc909bec; /* mov.w @(0x4,r4),r0 */
Lc909bec: *stop=0xc909becu;r[0]+=4294967295u;goto Lc909bee; /* add #-0x1,r0 */
Lc909bee: *stop=0xc909beeu;if(!valid_ram(r[4]+4u,2u))return -3;wr(m,r[4]+0x4u,r[0],2);goto Lc909bf0; /* mov.w r0,@(0x4,r4) */
Lc909bf0: *stop=0xc909bf0u;r[0]+=1u;goto Lc909bf2; /* add #0x1,r0 */
Lc909bf2: *stop=0xc909bf2u;r[0]=r[0]&65535u;goto Lc909bf4; /* extu.w r0,r0 */
Lc909bf4: *stop=0xc909bf4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc909bf6; /* tst r0,r0 */
Lc909bf6: if(!(*sr&1u))goto Lc909c38;goto Lc909bf8; /* bf 0x0c909c38 */
Lc909bf8: *stop=0xc909bf8u;r[0]=56u;goto Lc909bfa; /* mov #0x38,r0 */
Lc909bfa: *stop=0xc909bfau;if(!valid_ram(r[0]+r[4],2u))return -3;r[2]=sx(m,r[0]+r[4],2);goto Lc909bfc; /* mov.w @(r0,r4),r2 */
Lc909bfc: *stop=0xc909bfcu;r[2]+=1u;goto Lc909bfe; /* add #0x1,r2 */
Lc909bfe: *stop=0xc909c00u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[2],2);goto Lc909c38; /* bra 0x0c909c38 */
Lc909c02: *stop=0xc909c02u;r[0]=92u;goto Lc909c04; /* mov #0x5c,r0 */
Lc909c04: *stop=0xc909c04u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc909c06; /* fldi0 fr2 */
Lc909c06: *stop=0xc909c06u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc909c08; /* fmov.s @(r0,r4),fr3 */
Lc909c08: *stop=0xc909c08u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc909c0a; /* fsub fr5,fr3 */
Lc909c0a: *stop=0xc909c0au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc909c0c; /* fcmp/gt fr3,fr2 */
Lc909c0c: take=!(*sr&1u);*stop=0xc909c0eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc909c12;goto Lc909c10; /* bf/s 0x0c909c12 */
Lc909c10: *stop=0xc909c10u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909c12; /* fmov fr4,@(r0,r4) */
Lc909c12: *stop=0xc909c12u;r[0]=96u;goto Lc909c14; /* mov #0x60,r0 */
Lc909c14: *stop=0xc909c14u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc909c16; /* fldi0 fr2 */
Lc909c16: *stop=0xc909c16u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc909c18; /* fmov.s @(r0,r4),fr3 */
Lc909c18: *stop=0xc909c18u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc909c1a; /* fsub fr5,fr3 */
Lc909c1a: *stop=0xc909c1au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc909c1c; /* fcmp/gt fr3,fr2 */
Lc909c1c: take=!(*sr&1u);*stop=0xc909c1eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);if(take)goto Lc909c38;goto Lc909c20; /* bf/s 0x0c909c38 */
Lc909c20: *stop=0xc909c20u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909c22; /* fmov fr4,@(r0,r4) */
Lc909c22: *stop=0xc909c22u;r[0]=8u;goto Lc909c24; /* mov #0x8,r0 */
Lc909c24: *stop=0xc909c24u;r[0]<<=8;goto Lc909c26; /* shll8 r0 */
Lc909c26: *stop=0xc909c26u;r[0]+=18u;goto Lc909c28; /* add #0x12,r0 */
Lc909c28: *stop=0xc909c28u;r[6]=0u;goto Lc909c2a; /* mov #0x0,r6 */
Lc909c2a: *stop=0xc909c2au;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909c2c; /* mov.w r6,@(r0,r5) */
Lc909c2c: *stop=0xc909c2cu;r[0]+=54u;goto Lc909c2e; /* add #0x36,r0 */
Lc909c2e: *stop=0xc909c2eu;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909c30; /* mov.w r6,@(r0,r5) */
Lc909c30: *stop=0xc909c30u;r[0]+=64u;goto Lc909c32; /* add #0x40,r0 */
Lc909c32: *stop=0xc909c32u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909c34; /* mov.w r6,@(r0,r5) */
Lc909c34: *stop=0xc909c34u;r[0]+=4294967180u;goto Lc909c36; /* add #-0x74,r0 */
Lc909c36: *stop=0xc909c36u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[6],2);goto Lc909c38; /* mov.w r6,@(r0,r5) */
Lc909c38: *stop=0xc909c38u;r[0]=120u;goto Lc909c3a; /* mov #0x78,r0 */
Lc909c3a: *stop=0xc909c3au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto Lc909c3c; /* fldi1 fr5 */
Lc909c3c: *stop=0xc909c3cu;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto Lc909c3e; /* mov.w @(r0,r4),r0 */
Lc909c3e: *stop=0xc909c3eu;r[0]=r[0]&65535u;goto Lc909c40; /* extu.w r0,r0 */
Lc909c40: *stop=0xc909c40u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto Lc909c42; /* cmp/eq #0x0,r0 */
Lc909c42: take=(*sr&1u);*stop=0xc909c44u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[5],&f->fpscr,&fr[5],0);if(status)return status;}if(take)goto Lc909ca0;goto Lc909c46; /* bt/s 0x0c909ca0 */
Lc909c46: *stop=0xc909c46u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lc909c48; /* cmp/eq #0x1,r0 */
Lc909c48: if((*sr&1u))goto Lc909cc6;goto Lc909c4a; /* bt 0x0c909cc6 */
Lc909c4a: *stop=0xc909c4au;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto Lc909c4c; /* cmp/eq #0x2,r0 */
Lc909c4c: if((*sr&1u))goto Lc909cf8;goto Lc909c4e; /* bt 0x0c909cf8 */
Lc909c4e: *stop=0xc909c4eu;if(*pr!=entry_pr)return -3;*stop=0xc909c50u;return 0; /* rts */
Lc909ca0: *stop=0xc909ca0u;r[0]=118u;goto Lc909ca2; /* mov #0x76,r0 */
Lc909ca2: *stop=0xc909ca2u;r[3]=1u;goto Lc909ca4; /* mov #0x1,r3 */
Lc909ca4: *stop=0xc909ca4u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);goto Lc909ca6; /* mov.w r3,@(r0,r4) */
Lc909ca6: *stop=0xc909ca6u;r[0]+=14u;goto Lc909ca8; /* add #0xe,r0 */
Lc909ca8: *stop=0xc909ca8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909caa; /* fmov fr4,@(r0,r4) */
Lc909caa: *stop=0xc909caau;r[0]+=4u;goto Lc909cac; /* add #0x4,r0 */
Lc909cac: *stop=0xc909cacu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909cae; /* fmov fr4,@(r0,r4) */
Lc909cae: *stop=0xc909caeu;r[0]+=4u;goto Lc909cb0; /* add #0x4,r0 */
Lc909cb0: *stop=0xc909cb0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909cb2; /* fmov fr4,@(r0,r4) */
Lc909cb2: *stop=0xc909cb2u;r[0]+=16u;goto Lc909cb4; /* add #0x10,r0 */
Lc909cb4: *stop=0xc909cb4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto Lc909cb6; /* fldi1 fr4 */
Lc909cb6: *stop=0xc909cb6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909cb8; /* fmov fr4,@(r0,r4) */
Lc909cb8: *stop=0xc909cb8u;r[0]+=4u;goto Lc909cba; /* add #0x4,r0 */
Lc909cba: *stop=0xc909cbau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto Lc909cbc; /* fmov fr4,@(r0,r4) */
Lc909cbc: *stop=0xc909cbcu;r[0]=120u;goto Lc909cbe; /* mov #0x78,r0 */
Lc909cbe: *stop=0xc909cbeu;if(!valid_ram(r[0]+r[4],2u))return -3;r[3]=sx(m,r[0]+r[4],2);goto Lc909cc0; /* mov.w @(r0,r4),r3 */
Lc909cc0: *stop=0xc909cc0u;r[3]+=1u;goto Lc909cc2; /* add #0x1,r3 */
Lc909cc2: *stop=0xc909cc2u;if(*pr!=entry_pr)return -3;*stop=0xc909cc4u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[3],2);return 0; /* rts */
Lc909cc6: *stop=0xc909cc6u;r[1]=rd(m,0x0c909d60u,4);goto Lc909cc8; /* mov.l 0x0c909d60,r1 */
Lc909cc8: *stop=0xc909cc8u;r[0]=4294967196u;goto Lc909cca; /* mov #-0x64,r0 */
Lc909cca: *stop=0xc909ccau;r[0]=r[0]&255u;goto Lc909ccc; /* extu.b r0,r0 */
Lc909ccc: *stop=0xc909cccu;*fpul=r[1];goto Lc909cce; /* lds r1,FPUL */
Lc909cce: *stop=0xc909cceu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc909cd0; /* fmov.s @(r0,r4),fr2 */
Lc909cd0: *stop=0xc909cd0u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc909cd2; /* fsts FPUL,fr3 */
Lc909cd2: *stop=0xc909cd2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc909cd4; /* fsub fr3,fr2 */
Lc909cd4: *stop=0xc909cd4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc909cd6; /* fmov fr2,@(r0,r4) */
Lc909cd6: *stop=0xc909cd6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto Lc909cd8; /* fmov.s @(r0,r4),fr2 */
Lc909cd8: *stop=0xc909cd8u;r[0]+=4u;goto Lc909cda; /* add #0x4,r0 */
Lc909cda: *stop=0xc909cdau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc909cdc; /* fneg fr2 */
Lc909cdc: *stop=0xc909cdcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc909cde; /* fadd fr5,fr2 */
Lc909cde: *stop=0xc909cdeu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto Lc909ce0; /* fmov fr2,@(r0,r4) */
Lc909ce0: *stop=0xc909ce0u;r[0]=0x0c909d64u;goto Lc909ce2; /* mova 0x0c909d64,r0 */
Lc909ce2: *stop=0xc909ce2u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto Lc909ce4; /* fmov.s @r0,fr2 */
Lc909ce4: *stop=0xc909ce4u;r[0]=4294967196u;goto Lc909ce6; /* mov #-0x64,r0 */
Lc909ce6: *stop=0xc909ce6u;r[0]=r[0]&255u;goto Lc909ce8; /* extu.b r0,r0 */
Lc909ce8: *stop=0xc909ce8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc909cea; /* fmov.s @(r0,r4),fr1 */
Lc909cea: *stop=0xc909ceau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto Lc909cec; /* fcmp/gt fr1,fr2 */
Lc909cec: if(!(*sr&1u))goto Lc909d3c;goto Lc909cee; /* bf 0x0c909d3c */
Lc909cee: *stop=0xc909ceeu;r[0]=120u;goto Lc909cf0; /* mov #0x78,r0 */
Lc909cf0: *stop=0xc909cf0u;if(!valid_ram(r[0]+r[4],2u))return -3;r[2]=sx(m,r[0]+r[4],2);goto Lc909cf2; /* mov.w @(r0,r4),r2 */
Lc909cf2: *stop=0xc909cf2u;r[2]+=1u;goto Lc909cf4; /* add #0x1,r2 */
Lc909cf4: *stop=0xc909cf4u;if(*pr!=entry_pr)return -3;*stop=0xc909cf6u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[2],2);return 0; /* rts */
Lc909cf8: *stop=0xc909cf8u;r[1]=rd(m,0x0c909d68u,4);goto Lc909cfa; /* mov.l 0x0c909d68,r1 */
Lc909cfa: *stop=0xc909cfau;r[0]=4294967196u;goto Lc909cfc; /* mov #-0x64,r0 */
Lc909cfc: *stop=0xc909cfcu;r[0]=r[0]&255u;goto Lc909cfe; /* extu.b r0,r0 */
Lc909cfe: *stop=0xc909cfeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto Lc909d00; /* fldi1 fr3 */
Lc909d00: *stop=0xc909d00u;*fpul=r[1];goto Lc909d02; /* lds r1,FPUL */
Lc909d02: *stop=0xc909d02u;r[1]=4294967208u;goto Lc909d04; /* mov #-0x58,r1 */
Lc909d04: *stop=0xc909d04u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[4]);goto Lc909d06; /* fmov.s @(r0,r4),fr4 */
Lc909d06: *stop=0xc909d06u;r[1]=r[1]&255u;goto Lc909d08; /* extu.b r1,r1 */
Lc909d08: *stop=0xc909d08u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto Lc909d0a; /* fneg fr3 */
Lc909d0a: *stop=0xc909d0au;r[1]+=r[4];goto Lc909d0c; /* add r4,r1 */
Lc909d0c: *stop=0xc909d0cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto Lc909d0e; /* fadd fr3,fr4 */
Lc909d0e: *stop=0xc909d0eu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc909d10; /* fmov.s @r1,fr1 */
Lc909d10: *stop=0xc909d10u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto Lc909d12; /* fsts FPUL,fr2 */
Lc909d12: *stop=0xc909d12u;r[0]+=12u;goto Lc909d14; /* add #0xc,r0 */
Lc909d14: *stop=0xc909d14u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc909d16; /* fmul fr2,fr1 */
Lc909d16: *stop=0xc909d16u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909d18; /* fmov.s @(r0,r4),fr0 */
Lc909d18: *stop=0xc909d18u;r[1]=4294967208u;goto Lc909d1a; /* mov #-0x58,r1 */
Lc909d1a: *stop=0xc909d1au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[6],&f->fpscr,&fr[4]);if(status)return status;}goto Lc909d1c; /* fmul fr6,fr4 */
Lc909d1c: *stop=0xc909d1cu;r[1]=r[1]&255u;goto Lc909d1e; /* extu.b r1,r1 */
Lc909d1e: *stop=0xc909d1eu;r[1]+=r[4];goto Lc909d20; /* add r4,r1 */
Lc909d20: *stop=0xc909d20u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc909d22; /* fneg fr1 */
Lc909d22: *stop=0xc909d22u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],1);if(status)return status;}goto Lc909d24; /* fsub fr4,fr1 */
Lc909d24: *stop=0xc909d24u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909d26; /* fadd fr1,fr0 */
Lc909d26: *stop=0xc909d26u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909d28; /* fmov fr0,@(r0,r4) */
Lc909d28: *stop=0xc909d28u;r[0]+=4294967284u;goto Lc909d2a; /* add #-0xc,r0 */
Lc909d2a: *stop=0xc909d2au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto Lc909d2c; /* fmov.s @(r0,r4),fr0 */
Lc909d2c: *stop=0xc909d2cu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc909d2e; /* fmov.s @r1,fr1 */
Lc909d2e: *stop=0xc909d2eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto Lc909d30; /* fadd fr1,fr0 */
Lc909d30: *stop=0xc909d30u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[0]+r[4]);goto Lc909d32; /* fmov fr0,@(r0,r4) */
Lc909d32: *stop=0xc909d32u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc909d34; /* fmov.s @(r0,r4),fr1 */
Lc909d34: *stop=0xc909d34u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc909d36; /* fneg fr1 */
Lc909d36: *stop=0xc909d36u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[5],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc909d38; /* fadd fr5,fr1 */
Lc909d38: *stop=0xc909d38u;r[0]+=4u;goto Lc909d3a; /* add #0x4,r0 */
Lc909d3a: *stop=0xc909d3au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc909d3c; /* fmov fr1,@(r0,r4) */
Lc909d3c: *stop=0xc909d3cu;if(*pr!=entry_pr)return -3;*stop=0xc909d3eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_emit_regular_0c908b00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc908b00;
Lc908b00: *stop=0xc908b00u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc908b02; /* mov.l r14,@-r15 */
Lc908b02: *stop=0xc908b02u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto Lc908b04; /* mov.l r13,@-r15 */
Lc908b04: *stop=0xc908b04u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto Lc908b06; /* mov.l r12,@-r15 */
Lc908b06: *stop=0xc908b06u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto Lc908b08; /* fmov.s fr15,@-r15 */
Lc908b08: *stop=0xc908b08u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc908b0a; /* sts.l PR,@-r15 */
Lc908b0a: *stop=0xc908b0au;r[15]+=4294967196u;goto Lc908b0c; /* add #-0x64,r15 */
Lc908b0c: *stop=0xc908b0cu;r[0]=4294967276u;goto Lc908b0e; /* mov #-0x14,r0 */
Lc908b0e: *stop=0xc908b0eu;r[14]=r[5];goto Lc908b10; /* mov r5,r14 */
Lc908b10: *stop=0xc908b10u;r[0]=r[0]&255u;goto Lc908b12; /* extu.b r0,r0 */
Lc908b12: *stop=0xc908b12u;if(!valid_ram(r[0]+r[4],4u))return -3;r[6]=rd(m,r[4]+r[0],4);goto Lc908b14; /* mov.l @(r0,r4),r6 */
Lc908b14: *stop=0xc908b14u;r[0]+=4294967276u;goto Lc908b16; /* add #-0x14,r0 */
Lc908b16: *stop=0xc908b16u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]+r[4]);goto Lc908b18; /* fmov.s @(r0,r4),fr15 */
Lc908b18: *stop=0xc908b18u;r[0]+=4294967262u;goto Lc908b1a; /* add #-0x22,r0 */
Lc908b1a: *stop=0xc908b1au;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc908b1c; /* mov.b @(r0,r14),r3 */
Lc908b1c: *stop=0xc908b1cu;r[4]=r[15];goto Lc908b1e; /* mov r15,r4 */
Lc908b1e: *stop=0xc908b1eu;if(!valid_ram(r[6],1u))return -3;r[6]=sx(m,r[6],1);goto Lc908b20; /* mov.b @r6,r6 */
Lc908b20: *stop=0xc908b20u;r[3]=r[3]&255u;goto Lc908b22; /* extu.b r3,r3 */
Lc908b22: *stop=0xc908b22u;*fpul=r[3];goto Lc908b24; /* lds r3,FPUL */
Lc908b24: *stop=0xc908b24u;r[5]=r[6]&255u;goto Lc908b26; /* extu.b r6,r5 */
Lc908b26: *stop=0xc908b26u;r[3]=rd(m,0x0c908d40u,4);goto Lc908b28; /* mov.l 0x0c908d40,r3 */
Lc908b28: *stop=0xc908b28u;r[5]<<=2;goto Lc908b2a; /* shll2 r5 */
Lc908b2a: *stop=0xc908b2au;r[5]+=r[3];goto Lc908b2c; /* add r3,r5 */
Lc908b2c: *stop=0xc908b2cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc908b2e; /* float FPUL,fr3 */
Lc908b2e: *stop=0xc908b2eu;if(!valid_ram(r[5]+2u,1u))return -3;r[0]=sx(m,r[5]+0x2u,1);goto Lc908b30; /* mov.b @(0x2,r5),r0 */
Lc908b30: *stop=0xc908b30u;r[2]=r[0]&255u;goto Lc908b32; /* extu.b r0,r2 */
Lc908b32: *stop=0xc908b32u;*fpul=r[2];goto Lc908b34; /* lds r2,FPUL */
Lc908b34: *stop=0xc908b34u;r[0]=0x0c908d44u;goto Lc908b36; /* mova 0x0c908d44,r0 */
Lc908b36: *stop=0xc908b36u;fcopy(f,5u,3u);goto Lc908b38; /* fmov fr3,fr5 */
Lc908b38: *stop=0xc908b38u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto Lc908b3a; /* fmov.s @r0,fr4 */
Lc908b3a: *stop=0xc908b3au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc908b3c; /* float FPUL,fr2 */
Lc908b3c: *stop=0xc908b3cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto Lc908b3e; /* fmul fr2,fr5 */
Lc908b3e: *stop=0xc908b3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[4],&f->fpscr,&fr[5]);if(status)return status;}goto Lc908b40; /* fmul fr4,fr5 */
Lc908b40: *stop=0xc908b40u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,fpul);if(status)return status;goto Lc908b42; /* ftrc fr5,FPUL */
Lc908b42: *stop=0xc908b42u;r[0]=*fpul;goto Lc908b44; /* sts FPUL,r0 */
Lc908b44: *stop=0xc908b44u;if(!valid_ram(r[4]+2u,1u))return -3;wr(m,r[4]+0x2u,r[0],1);goto Lc908b46; /* mov.b r0,@(0x2,r4) */
Lc908b46: *stop=0xc908b46u;r[0]=4294967221u;goto Lc908b48; /* mov #-0x4b,r0 */
Lc908b48: *stop=0xc908b48u;r[0]=r[0]&255u;goto Lc908b4a; /* extu.b r0,r0 */
Lc908b4a: *stop=0xc908b4au;if(!valid_ram(r[0]+r[14],1u))return -3;r[2]=sx(m,r[14]+r[0],1);goto Lc908b4c; /* mov.b @(r0,r14),r2 */
Lc908b4c: *stop=0xc908b4cu;r[2]=r[2]&255u;goto Lc908b4e; /* extu.b r2,r2 */
Lc908b4e: *stop=0xc908b4eu;if(!valid_ram(r[5]+1u,1u))return -3;r[0]=sx(m,r[5]+0x1u,1);goto Lc908b50; /* mov.b @(0x1,r5),r0 */
Lc908b50: *stop=0xc908b50u;*fpul=r[2];goto Lc908b52; /* lds r2,FPUL */
Lc908b52: *stop=0xc908b52u;r[2]=r[0]&255u;goto Lc908b54; /* extu.b r0,r2 */
Lc908b54: *stop=0xc908b54u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc908b56; /* float FPUL,fr3 */
Lc908b56: *stop=0xc908b56u;*fpul=r[2];goto Lc908b58; /* lds r2,FPUL */
Lc908b58: *stop=0xc908b58u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc908b5a; /* float FPUL,fr2 */
Lc908b5a: *stop=0xc908b5au;fcopy(f,5u,3u);goto Lc908b5c; /* fmov fr3,fr5 */
Lc908b5c: *stop=0xc908b5cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto Lc908b5e; /* fmul fr2,fr5 */
Lc908b5e: *stop=0xc908b5eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[4],&f->fpscr,&fr[5]);if(status)return status;}goto Lc908b60; /* fmul fr4,fr5 */
Lc908b60: *stop=0xc908b60u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,fpul);if(status)return status;goto Lc908b62; /* ftrc fr5,FPUL */
Lc908b62: *stop=0xc908b62u;r[0]=*fpul;goto Lc908b64; /* sts FPUL,r0 */
Lc908b64: *stop=0xc908b64u;if(!valid_ram(r[4]+1u,1u))return -3;wr(m,r[4]+0x1u,r[0],1);goto Lc908b66; /* mov.b r0,@(0x1,r4) */
Lc908b66: *stop=0xc908b66u;r[0]=4294967220u;goto Lc908b68; /* mov #-0x4c,r0 */
Lc908b68: *stop=0xc908b68u;r[0]=r[0]&255u;goto Lc908b6a; /* extu.b r0,r0 */
Lc908b6a: *stop=0xc908b6au;if(!valid_ram(r[0]+r[14],1u))return -3;r[2]=sx(m,r[14]+r[0],1);goto Lc908b6c; /* mov.b @(r0,r14),r2 */
Lc908b6c: *stop=0xc908b6cu;r[2]=r[2]&255u;goto Lc908b6e; /* extu.b r2,r2 */
Lc908b6e: *stop=0xc908b6eu;*fpul=r[2];goto Lc908b70; /* lds r2,FPUL */
Lc908b70: *stop=0xc908b70u;if(!valid_ram(r[5],1u))return -3;r[2]=sx(m,r[5],1);goto Lc908b72; /* mov.b @r5,r2 */
Lc908b72: *stop=0xc908b72u;r[5]=4294967295u;goto Lc908b74; /* mov #-0x1,r5 */
Lc908b74: *stop=0xc908b74u;r[5]=r[5]&255u;goto Lc908b76; /* extu.b r5,r5 */
Lc908b76: *stop=0xc908b76u;r[2]=r[2]&255u;goto Lc908b78; /* extu.b r2,r2 */
Lc908b78: *stop=0xc908b78u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc908b7a; /* float FPUL,fr3 */
Lc908b7a: *stop=0xc908b7au;*fpul=r[2];goto Lc908b7c; /* lds r2,FPUL */
Lc908b7c: *stop=0xc908b7cu;r[0]=r[5];goto Lc908b7e; /* mov r5,r0 */
Lc908b7e: *stop=0xc908b7eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc908b80; /* float FPUL,fr2 */
Lc908b80: *stop=0xc908b80u;fcopy(f,5u,3u);goto Lc908b82; /* fmov fr3,fr5 */
Lc908b82: *stop=0xc908b82u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto Lc908b84; /* fmul fr2,fr5 */
Lc908b84: *stop=0xc908b84u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[4],&f->fpscr,&fr[5]);if(status)return status;}goto Lc908b86; /* fmul fr4,fr5 */
Lc908b86: *stop=0xc908b86u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,fpul);if(status)return status;goto Lc908b88; /* ftrc fr5,FPUL */
Lc908b88: *stop=0xc908b88u;r[2]=*fpul;goto Lc908b8a; /* sts FPUL,r2 */
Lc908b8a: *stop=0xc908b8au;if(!valid_ram(r[4],1u))return -3;wr(m,r[4],r[2],1);goto Lc908b8c; /* mov.b r2,@r4 */
Lc908b8c: *stop=0xc908b8cu;r[2]=r[15];goto Lc908b8e; /* mov r15,r2 */
Lc908b8e: *stop=0xc908b8eu;if(!valid_ram(r[4]+3u,1u))return -3;wr(m,r[4]+0x3u,r[0],1);goto Lc908b90; /* mov.b r0,@(0x3,r4) */
Lc908b90: *stop=0xc908b90u;r[0]=84u;goto Lc908b92; /* mov #0x54,r0 */
Lc908b92: *stop=0xc908b92u;r[1]=r[15];goto Lc908b94; /* mov r15,r1 */
Lc908b94: *stop=0xc908b94u;r[2]+=36u;goto Lc908b96; /* add #0x24,r2 */
Lc908b96: *stop=0xc908b96u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc908b98; /* mov.l r2,@(r0,r15) */
Lc908b98: *stop=0xc908b98u;r[0]=88u;goto Lc908b9a; /* mov #0x58,r0 */
Lc908b9a: *stop=0xc908b9au;r[1]+=20u;goto Lc908b9c; /* add #0x14,r1 */
Lc908b9c: *stop=0xc908b9cu;r[2]=r[15];goto Lc908b9e; /* mov r15,r2 */
Lc908b9e: *stop=0xc908b9eu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[1],4);goto Lc908ba0; /* mov.l r1,@(r0,r15) */
Lc908ba0: *stop=0xc908ba0u;r[0]=92u;goto Lc908ba2; /* mov #0x5c,r0 */
Lc908ba2: *stop=0xc908ba2u;r[2]+=4u;goto Lc908ba4; /* add #0x4,r2 */
Lc908ba4: *stop=0xc908ba4u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc908ba6; /* mov.l r2,@(r0,r15) */
Lc908ba6: *stop=0xc908ba6u;r[0]=96u;goto Lc908ba8; /* mov #0x60,r0 */
Lc908ba8: *stop=0xc908ba8u;r[1]=4u;goto Lc908baa; /* mov #0x4,r1 */
Lc908baa: *stop=0xc908baau;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[1],4);goto Lc908bac; /* mov.l r1,@(r0,r15) */
Lc908bac: *stop=0xc908bacu;r[0]=0x0c908d48u;goto Lc908bae; /* mova 0x0c908d48,r0 */
Lc908bae: *stop=0xc908baeu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]);goto Lc908bb0; /* fmov.s @r0,fr6 */
Lc908bb0: *stop=0xc908bb0u;r[0]=84u;goto Lc908bb2; /* mov #0x54,r0 */
Lc908bb2: *stop=0xc908bb2u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908bb4; /* mov.l @(r0,r15),r2 */
Lc908bb4: *stop=0xc908bb4u;r[0]=0x0c908d4cu;goto Lc908bb6; /* mova 0x0c908d4c,r0 */
Lc908bb6: *stop=0xc908bb6u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[2]);goto Lc908bb8; /* fmov.s fr6,@r2 */
Lc908bb8: *stop=0xc908bb8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc908bba; /* fmov.s @r0,fr5 */
Lc908bba: *stop=0xc908bbau;r[0]=84u;goto Lc908bbc; /* mov #0x54,r0 */
Lc908bbc: *stop=0xc908bbcu;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908bbe; /* mov.l @(r0,r15),r2 */
Lc908bbe: *stop=0xc908bbeu;r[0]=r[1];goto Lc908bc0; /* mov r1,r0 */
Lc908bc0: *stop=0xc908bc0u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[2]);goto Lc908bc2; /* fmov fr5,@(r0,r2) */
Lc908bc2: *stop=0xc908bc2u;r[0]=0x0c908d50u;goto Lc908bc4; /* mova 0x0c908d50,r0 */
Lc908bc4: *stop=0xc908bc4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto Lc908bc6; /* fmov.s @r0,fr4 */
Lc908bc6: *stop=0xc908bc6u;r[0]=84u;goto Lc908bc8; /* mov #0x54,r0 */
Lc908bc8: *stop=0xc908bc8u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908bca; /* mov.l @(r0,r15),r2 */
Lc908bca: *stop=0xc908bcau;r[0]=8u;goto Lc908bcc; /* mov #0x8,r0 */
Lc908bcc: *stop=0xc908bccu;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[2]);goto Lc908bce; /* fmov fr4,@(r0,r2) */
Lc908bce: *stop=0xc908bceu;r[0]=92u;goto Lc908bd0; /* mov #0x5c,r0 */
Lc908bd0: *stop=0xc908bd0u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908bd2; /* mov.l @(r0,r15),r2 */
Lc908bd2: *stop=0xc908bd2u;r[6]=0u;goto Lc908bd4; /* mov #0x0,r6 */
Lc908bd4: *stop=0xc908bd4u;r[0]=92u;goto Lc908bd6; /* mov #0x5c,r0 */
Lc908bd6: *stop=0xc908bd6u;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[6],2);goto Lc908bd8; /* mov.w r6,@r2 */
Lc908bd8: *stop=0xc908bd8u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908bda; /* mov.l @(r0,r15),r2 */
Lc908bda: *stop=0xc908bdau;r[0]=r[6];goto Lc908bdc; /* mov r6,r0 */
Lc908bdc: *stop=0xc908bdcu;if(!valid_ram(r[2]+2u,2u))return -3;wr(m,r[2]+0x2u,r[0],2);goto Lc908bde; /* mov.w r0,@(0x2,r2) */
Lc908bde: *stop=0xc908bdeu;r[0]=88u;goto Lc908be0; /* mov #0x58,r0 */
Lc908be0: *stop=0xc908be0u;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);goto Lc908be2; /* mov.l @r4,r1 */
Lc908be2: *stop=0xc908be2u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908be4; /* mov.l @(r0,r15),r2 */
Lc908be4: *stop=0xc908be4u;r[0]=84u;goto Lc908be6; /* mov #0x54,r0 */
Lc908be6: *stop=0xc908be6u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc908be8; /* mov.l r1,@r2 */
Lc908be8: *stop=0xc908be8u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908bea; /* mov.l @(r0,r15),r2 */
Lc908bea: *stop=0xc908beau;r[0]=84u;goto Lc908bec; /* mov #0x54,r0 */
Lc908bec: *stop=0xc908becu;r[2]+=12u;goto Lc908bee; /* add #0xc,r2 */
Lc908bee: *stop=0xc908beeu;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[2]);goto Lc908bf0; /* fmov.s fr6,@r2 */
Lc908bf0: *stop=0xc908bf0u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908bf2; /* mov.l @(r0,r15),r2 */
Lc908bf2: *stop=0xc908bf2u;r[0]=4u;goto Lc908bf4; /* mov #0x4,r0 */
Lc908bf4: *stop=0xc908bf4u;r[2]+=12u;goto Lc908bf6; /* add #0xc,r2 */
Lc908bf6: *stop=0xc908bf6u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[2]);goto Lc908bf8; /* fmov fr6,@(r0,r2) */
Lc908bf8: *stop=0xc908bf8u;r[0]=84u;goto Lc908bfa; /* mov #0x54,r0 */
Lc908bfa: *stop=0xc908bfau;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908bfc; /* mov.l @(r0,r15),r2 */
Lc908bfc: *stop=0xc908bfcu;r[0]=8u;goto Lc908bfe; /* mov #0x8,r0 */
Lc908bfe: *stop=0xc908bfeu;r[2]+=12u;goto Lc908c00; /* add #0xc,r2 */
Lc908c00: *stop=0xc908c00u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[2]);goto Lc908c02; /* fmov fr4,@(r0,r2) */
Lc908c02: *stop=0xc908c02u;r[0]=92u;goto Lc908c04; /* mov #0x5c,r0 */
Lc908c04: *stop=0xc908c04u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c06; /* mov.l @(r0,r15),r2 */
Lc908c06: *stop=0xc908c06u;r[0]=92u;goto Lc908c08; /* mov #0x5c,r0 */
Lc908c08: *stop=0xc908c08u;r[2]+=4u;goto Lc908c0a; /* add #0x4,r2 */
Lc908c0a: *stop=0xc908c0au;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[6],2);goto Lc908c0c; /* mov.w r6,@r2 */
Lc908c0c: *stop=0xc908c0cu;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c0e; /* mov.l @(r0,r15),r2 */
Lc908c0e: *stop=0xc908c0eu;r[0]=r[5];goto Lc908c10; /* mov r5,r0 */
Lc908c10: *stop=0xc908c10u;r[2]+=4u;goto Lc908c12; /* add #0x4,r2 */
Lc908c12: *stop=0xc908c12u;if(!valid_ram(r[2]+2u,2u))return -3;wr(m,r[2]+0x2u,r[0],2);goto Lc908c14; /* mov.w r0,@(0x2,r2) */
Lc908c14: *stop=0xc908c14u;r[0]=88u;goto Lc908c16; /* mov #0x58,r0 */
Lc908c16: *stop=0xc908c16u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c18; /* mov.l @(r0,r15),r2 */
Lc908c18: *stop=0xc908c18u;r[0]=84u;goto Lc908c1a; /* mov #0x54,r0 */
Lc908c1a: *stop=0xc908c1au;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);goto Lc908c1c; /* mov.l @r4,r1 */
Lc908c1c: *stop=0xc908c1cu;r[2]+=4u;goto Lc908c1e; /* add #0x4,r2 */
Lc908c1e: *stop=0xc908c1eu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc908c20; /* mov.l r1,@r2 */
Lc908c20: *stop=0xc908c20u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c22; /* mov.l @(r0,r15),r2 */
Lc908c22: *stop=0xc908c22u;r[0]=84u;goto Lc908c24; /* mov #0x54,r0 */
Lc908c24: *stop=0xc908c24u;r[2]+=24u;goto Lc908c26; /* add #0x18,r2 */
Lc908c26: *stop=0xc908c26u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[2]);goto Lc908c28; /* fmov.s fr5,@r2 */
Lc908c28: *stop=0xc908c28u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c2a; /* mov.l @(r0,r15),r2 */
Lc908c2a: *stop=0xc908c2au;r[0]=4u;goto Lc908c2c; /* mov #0x4,r0 */
Lc908c2c: *stop=0xc908c2cu;r[2]+=24u;goto Lc908c2e; /* add #0x18,r2 */
Lc908c2e: *stop=0xc908c2eu;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[2]);goto Lc908c30; /* fmov fr6,@(r0,r2) */
Lc908c30: *stop=0xc908c30u;r[0]=84u;goto Lc908c32; /* mov #0x54,r0 */
Lc908c32: *stop=0xc908c32u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c34; /* mov.l @(r0,r15),r2 */
Lc908c34: *stop=0xc908c34u;r[0]=8u;goto Lc908c36; /* mov #0x8,r0 */
Lc908c36: *stop=0xc908c36u;r[2]+=24u;goto Lc908c38; /* add #0x18,r2 */
Lc908c38: *stop=0xc908c38u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[2]);goto Lc908c3a; /* fmov fr4,@(r0,r2) */
Lc908c3a: *stop=0xc908c3au;r[0]=92u;goto Lc908c3c; /* mov #0x5c,r0 */
Lc908c3c: *stop=0xc908c3cu;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c3e; /* mov.l @(r0,r15),r2 */
Lc908c3e: *stop=0xc908c3eu;r[0]=92u;goto Lc908c40; /* mov #0x5c,r0 */
Lc908c40: *stop=0xc908c40u;r[2]+=8u;goto Lc908c42; /* add #0x8,r2 */
Lc908c42: *stop=0xc908c42u;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[5],2);goto Lc908c44; /* mov.w r5,@r2 */
Lc908c44: *stop=0xc908c44u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c46; /* mov.l @(r0,r15),r2 */
Lc908c46: *stop=0xc908c46u;r[0]=r[5];goto Lc908c48; /* mov r5,r0 */
Lc908c48: *stop=0xc908c48u;r[2]+=8u;goto Lc908c4a; /* add #0x8,r2 */
Lc908c4a: *stop=0xc908c4au;if(!valid_ram(r[2]+2u,2u))return -3;wr(m,r[2]+0x2u,r[0],2);goto Lc908c4c; /* mov.w r0,@(0x2,r2) */
Lc908c4c: *stop=0xc908c4cu;r[0]=88u;goto Lc908c4e; /* mov #0x58,r0 */
Lc908c4e: *stop=0xc908c4eu;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);goto Lc908c50; /* mov.l @r4,r1 */
Lc908c50: *stop=0xc908c50u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c52; /* mov.l @(r0,r15),r2 */
Lc908c52: *stop=0xc908c52u;r[0]=84u;goto Lc908c54; /* mov #0x54,r0 */
Lc908c54: *stop=0xc908c54u;r[2]+=8u;goto Lc908c56; /* add #0x8,r2 */
Lc908c56: *stop=0xc908c56u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc908c58; /* mov.l r1,@r2 */
Lc908c58: *stop=0xc908c58u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c5a; /* mov.l @(r0,r15),r2 */
Lc908c5a: *stop=0xc908c5au;r[0]=84u;goto Lc908c5c; /* mov #0x54,r0 */
Lc908c5c: *stop=0xc908c5cu;r[2]+=36u;goto Lc908c5e; /* add #0x24,r2 */
Lc908c5e: *stop=0xc908c5eu;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[2]);goto Lc908c60; /* fmov.s fr5,@r2 */
Lc908c60: *stop=0xc908c60u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c62; /* mov.l @(r0,r15),r2 */
Lc908c62: *stop=0xc908c62u;r[0]=4u;goto Lc908c64; /* mov #0x4,r0 */
Lc908c64: *stop=0xc908c64u;r[2]+=36u;goto Lc908c66; /* add #0x24,r2 */
Lc908c66: *stop=0xc908c66u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[2]);goto Lc908c68; /* fmov fr5,@(r0,r2) */
Lc908c68: *stop=0xc908c68u;r[0]=84u;goto Lc908c6a; /* mov #0x54,r0 */
Lc908c6a: *stop=0xc908c6au;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c6c; /* mov.l @(r0,r15),r2 */
Lc908c6c: *stop=0xc908c6cu;r[0]=8u;goto Lc908c6e; /* mov #0x8,r0 */
Lc908c6e: *stop=0xc908c6eu;r[2]+=36u;goto Lc908c70; /* add #0x24,r2 */
Lc908c70: *stop=0xc908c70u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[2]);goto Lc908c72; /* fmov fr4,@(r0,r2) */
Lc908c72: *stop=0xc908c72u;r[0]=92u;goto Lc908c74; /* mov #0x5c,r0 */
Lc908c74: *stop=0xc908c74u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c76; /* mov.l @(r0,r15),r2 */
Lc908c76: *stop=0xc908c76u;r[0]=92u;goto Lc908c78; /* mov #0x5c,r0 */
Lc908c78: *stop=0xc908c78u;r[2]+=12u;goto Lc908c7a; /* add #0xc,r2 */
Lc908c7a: *stop=0xc908c7au;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[5],2);goto Lc908c7c; /* mov.w r5,@r2 */
Lc908c7c: *stop=0xc908c7cu;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c7e; /* mov.l @(r0,r15),r2 */
Lc908c7e: *stop=0xc908c7eu;r[0]=r[6];goto Lc908c80; /* mov r6,r0 */
Lc908c80: *stop=0xc908c80u;r[2]+=12u;goto Lc908c82; /* add #0xc,r2 */
Lc908c82: *stop=0xc908c82u;if(!valid_ram(r[2]+2u,2u))return -3;wr(m,r[2]+0x2u,r[0],2);goto Lc908c84; /* mov.w r0,@(0x2,r2) */
Lc908c84: *stop=0xc908c84u;r[0]=88u;goto Lc908c86; /* mov #0x58,r0 */
Lc908c86: *stop=0xc908c86u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc908c88; /* mov.l @(r0,r15),r2 */
Lc908c88: *stop=0xc908c88u;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);goto Lc908c8a; /* mov.l @r4,r1 */
Lc908c8a: *stop=0xc908c8au;r[2]+=12u;goto Lc908c8c; /* add #0xc,r2 */
Lc908c8c: *stop=0xc908c8cu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc908c8e; /* mov.l r1,@r2 */
Lc908c8e: *stop=0xc908c8eu;r[2]=rd(m,0x0c908d58u,4);goto Lc908c90; /* mov.l 0x0c908d58,r2 */
Lc908c90: *stop=0xc908c90u;r[4]=rd(m,0x0c908d54u,4);goto Lc908c92; /* mov.l 0x0c908d54,r4 */
Lc908c92: target=r[2];*pr=0x0c908c96u;*stop=0xc908c94u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908c96; /* jsr @r2 */
Lc908c96: *stop=0xc908c96u;r[3]=rd(m,0x0c908d60u,4);goto Lc908c98; /* mov.l 0x0c908d60,r3 */
Lc908c98: *stop=0xc908c98u;r[4]=rd(m,0x0c908d5cu,4);goto Lc908c9a; /* mov.l 0x0c908d5c,r4 */
Lc908c9a: target=r[3];*pr=0x0c908c9eu;*stop=0xc908c9cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908c9e; /* jsr @r3 */
Lc908c9e: *stop=0xc908c9eu;r[5]=r[14];goto Lc908ca0; /* mov r14,r5 */
Lc908ca0: *stop=0xc908ca0u;r[12]=rd(m,0x0c908d64u,4);goto Lc908ca2; /* mov.l 0x0c908d64,r12 */
Lc908ca2: *stop=0xc908ca2u;r[5]+=12u;goto Lc908ca4; /* add #0xc,r5 */
Lc908ca4: target=r[12];*pr=0x0c908ca8u;*stop=0xc908ca6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908ca8; /* jsr @r12 */
Lc908ca8: *stop=0xc908ca8u;fcopy(f,5u,15u);goto Lc908caa; /* fmov fr15,fr5 */
Lc908caa: *stop=0xc908caau;r[13]=rd(m,0x0c908d68u,4);goto Lc908cac; /* mov.l 0x0c908d68,r13 */
Lc908cac: *stop=0xc908cacu;fcopy(f,4u,15u);goto Lc908cae; /* fmov fr15,fr4 */
Lc908cae: *stop=0xc908caeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto Lc908cb0; /* fneg fr5 */
Lc908cb0: *stop=0xc908cb0u;fcopy(f,6u,15u);goto Lc908cb2; /* fmov fr15,fr6 */
Lc908cb2: target=r[13];*pr=0x0c908cb6u;*stop=0xc908cb4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908cb6; /* jsr @r13 */
Lc908cb6: *stop=0xc908cb6u;r[2]=rd(m,0x0c908d60u,4);goto Lc908cb8; /* mov.l 0x0c908d60,r2 */
Lc908cb8: target=r[2];*pr=0x0c908cbcu;*stop=0xc908cbau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908cbc; /* jsr @r2 */
Lc908cbc: *stop=0xc908cbcu;r[5]=4294967172u;goto Lc908cbe; /* mov #-0x7c,r5 */
Lc908cbe: *stop=0xc908cbeu;r[5]=r[5]&255u;goto Lc908cc0; /* extu.b r5,r5 */
Lc908cc0: *stop=0xc908cc0u;r[5]+=r[14];goto Lc908cc2; /* add r14,r5 */
Lc908cc2: target=r[12];*pr=0x0c908cc6u;*stop=0xc908cc4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908cc6; /* jsr @r12 */
Lc908cc6: *stop=0xc908cc6u;r[0]=118u;goto Lc908cc8; /* mov #0x76,r0 */
Lc908cc8: *stop=0xc908cc8u;r[3]=rd(m,0x0c908d6cu,4);goto Lc908cca; /* mov.l 0x0c908d6c,r3 */
Lc908cca: *stop=0xc908ccau;if(!valid_ram(r[0]+r[14],2u))return -3;r[4]=sx(m,r[0]+r[14],2);goto Lc908ccc; /* mov.w @(r0,r14),r4 */
Lc908ccc: target=r[3];*pr=0x0c908cd0u;*stop=0xc908cceu;r[4]=r[4]&65535u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908cd0; /* jsr @r3 */
Lc908cd0: *stop=0xc908cd0u;r[0]=4294967200u;goto Lc908cd2; /* mov #-0x60,r0 */
Lc908cd2: *stop=0xc908cd2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0x3f800000u;goto Lc908cd4; /* fldi1 fr15 */
Lc908cd4: *stop=0xc908cd4u;r[0]=r[0]&255u;goto Lc908cd6; /* extu.b r0,r0 */
Lc908cd6: *stop=0xc908cd6u;fcopy(f,6u,15u);goto Lc908cd8; /* fmov fr15,fr6 */
Lc908cd8: *stop=0xc908cd8u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto Lc908cda; /* fmov.s @(r0,r14),fr5 */
Lc908cda: *stop=0xc908cdau;r[0]+=4294967292u;goto Lc908cdc; /* add #-0x4,r0 */
Lc908cdc: *stop=0xc908cdcu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto Lc908cde; /* fmov.s @(r0,r14),fr4 */
Lc908cde: target=r[13];*pr=0x0c908ce2u;*stop=0xc908ce0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908ce2; /* jsr @r13 */
Lc908ce2: *stop=0xc908ce2u;r[3]=rd(m,0x0c908d74u,4);goto Lc908ce4; /* mov.l 0x0c908d74,r3 */
Lc908ce4: *stop=0xc908ce4u;r[4]=r[15];goto Lc908ce6; /* mov r15,r4 */
Lc908ce6: *stop=0xc908ce6u;r[6]=rd(m,0x0c908d70u,4);goto Lc908ce8; /* mov.l 0x0c908d70,r6 */
Lc908ce8: *stop=0xc908ce8u;r[5]=4u;goto Lc908cea; /* mov #0x4,r5 */
Lc908cea: target=r[3];*pr=0x0c908ceeu;*stop=0xc908cecu;r[4]+=84u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908cee; /* jsr @r3 */
Lc908cee: *stop=0xc908ceeu;r[2]=rd(m,0x0c908d78u,4);goto Lc908cf0; /* mov.l 0x0c908d78,r2 */
Lc908cf0: target=r[2];*pr=0x0c908cf4u;*stop=0xc908cf2u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908cf4; /* jsr @r2 */
Lc908cf4: *stop=0xc908cf4u;r[5]=r[14];goto Lc908cf6; /* mov r14,r5 */
Lc908cf6: *stop=0xc908cf6u;r[5]+=68u;goto Lc908cf8; /* add #0x44,r5 */
Lc908cf8: target=r[12];*pr=0x0c908cfcu;*stop=0xc908cfau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908cfc; /* jsr @r12 */
Lc908cfc: *stop=0xc908cfcu;r[0]=54u;goto Lc908cfe; /* mov #0x36,r0 */
Lc908cfe: *stop=0xc908cfeu;r[3]=rd(m,0x0c908d6cu,4);goto Lc908d00; /* mov.l 0x0c908d6c,r3 */
Lc908d00: *stop=0xc908d00u;if(!valid_ram(r[0]+r[14],2u))return -3;r[4]=sx(m,r[0]+r[14],2);goto Lc908d02; /* mov.w @(r0,r14),r4 */
Lc908d02: target=r[3];*pr=0x0c908d06u;*stop=0xc908d04u;r[4]=r[4]&65535u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908d06; /* jsr @r3 */
Lc908d06: *stop=0xc908d06u;r[0]=96u;goto Lc908d08; /* mov #0x60,r0 */
Lc908d08: *stop=0xc908d08u;fcopy(f,6u,15u);goto Lc908d0a; /* fmov fr15,fr6 */
Lc908d0a: *stop=0xc908d0au;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto Lc908d0c; /* fmov.s @(r0,r14),fr5 */
Lc908d0c: *stop=0xc908d0cu;r[0]=92u;goto Lc908d0e; /* mov #0x5c,r0 */
Lc908d0e: *stop=0xc908d0eu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto Lc908d10; /* fmov.s @(r0,r14),fr4 */
Lc908d10: target=r[13];*pr=0x0c908d14u;*stop=0xc908d12u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908d14; /* jsr @r13 */
Lc908d14: *stop=0xc908d14u;r[3]=rd(m,0x0c908d74u,4);goto Lc908d16; /* mov.l 0x0c908d74,r3 */
Lc908d16: *stop=0xc908d16u;r[4]=r[15];goto Lc908d18; /* mov r15,r4 */
Lc908d18: *stop=0xc908d18u;r[6]=rd(m,0x0c908d70u,4);goto Lc908d1a; /* mov.l 0x0c908d70,r6 */
Lc908d1a: *stop=0xc908d1au;r[5]=4u;goto Lc908d1c; /* mov #0x4,r5 */
Lc908d1c: target=r[3];*pr=0x0c908d20u;*stop=0xc908d1eu;r[4]+=84u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908d20; /* jsr @r3 */
Lc908d20: *stop=0xc908d20u;r[2]=rd(m,0x0c908d78u,4);goto Lc908d22; /* mov.l 0x0c908d78,r2 */
Lc908d22: target=r[2];*pr=0x0c908d26u;*stop=0xc908d24u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908d26; /* jsr @r2 */
Lc908d26: *stop=0xc908d26u;r[15]+=100u;goto Lc908d28; /* add #0x64,r15 */
Lc908d28: *stop=0xc908d28u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc908d2a; /* lds.l @r15+,PR */
Lc908d2a: *stop=0xc908d2au;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc908d2c; /* fmov.s @r15+,fr15 */
Lc908d2c: *stop=0xc908d2cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto Lc908d2e; /* mov.l @r15+,r12 */
Lc908d2e: *stop=0xc908d2eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto Lc908d30; /* mov.l @r15+,r13 */
Lc908d30: *stop=0xc908d30u;if(*pr!=entry_pr)return -3;*stop=0xc908d32u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_emit_special_0c908d80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc908d80;
Lc908d80: *stop=0xc908d80u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc908d82; /* mov.l r14,@-r15 */
Lc908d82: *stop=0xc908d82u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto Lc908d84; /* fmov.s fr15,@-r15 */
Lc908d84: *stop=0xc908d84u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc908d86; /* sts.l PR,@-r15 */
Lc908d86: *stop=0xc908d86u;r[15]+=4294967192u;goto Lc908d88; /* add #-0x68,r15 */
Lc908d88: *stop=0xc908d88u;r[0]=4294967256u;goto Lc908d8a; /* mov #-0x28,r0 */
Lc908d8a: *stop=0xc908d8au;r[3]=rd(m,0x0c908fc0u,4);goto Lc908d8c; /* mov.l 0x0c908fc0,r3 */
Lc908d8c: *stop=0xc908d8cu;r[0]=r[0]&255u;goto Lc908d8e; /* extu.b r0,r0 */
Lc908d8e: *stop=0xc908d8eu;r[2]=r[15];goto Lc908d90; /* mov r15,r2 */
Lc908d90: *stop=0xc908d90u;r[2]+=40u;goto Lc908d92; /* add #0x28,r2 */
Lc908d92: *stop=0xc908d92u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto Lc908d94; /* fldi1 fr3 */
Lc908d94: *stop=0xc908d94u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc908d96; /* fadd fr3,fr3 */
Lc908d96: *stop=0xc908d96u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto Lc908d98; /* mov.l r5,@r15 */
Lc908d98: *stop=0xc908d98u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]+r[4]);goto Lc908d9a; /* fmov.s @(r0,r4),fr15 */
Lc908d9a: *stop=0xc908d9au;r[4]=r[15];goto Lc908d9c; /* mov r15,r4 */
Lc908d9c: *stop=0xc908d9cu;r[4]+=4u;goto Lc908d9e; /* add #0x4,r4 */
Lc908d9e: *stop=0xc908d9eu;r[0]=88u;goto Lc908da0; /* mov #0x58,r0 */
Lc908da0: *stop=0xc908da0u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[3],4);goto Lc908da2; /* mov.l r3,@r4 */
Lc908da2: *stop=0xc908da2u;r[3]=r[15];goto Lc908da4; /* mov r15,r3 */
Lc908da4: *stop=0xc908da4u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc908da6; /* mov.l r2,@(r0,r15) */
Lc908da6: *stop=0xc908da6u;r[0]=92u;goto Lc908da8; /* mov #0x5c,r0 */
Lc908da8: *stop=0xc908da8u;r[2]=r[15];goto Lc908daa; /* mov r15,r2 */
Lc908daa: *stop=0xc908daau;r[3]+=24u;goto Lc908dac; /* add #0x18,r3 */
Lc908dac: *stop=0xc908dacu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc908dae; /* mov.l r3,@(r0,r15) */
Lc908dae: *stop=0xc908daeu;r[2]+=8u;goto Lc908db0; /* add #0x8,r2 */
Lc908db0: *stop=0xc908db0u;r[0]=96u;goto Lc908db2; /* mov #0x60,r0 */
Lc908db2: *stop=0xc908db2u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc908db4; /* mov.l r2,@(r0,r15) */
Lc908db4: *stop=0xc908db4u;r[3]=4u;goto Lc908db6; /* mov #0x4,r3 */
Lc908db6: *stop=0xc908db6u;r[0]=100u;goto Lc908db8; /* mov #0x64,r0 */
Lc908db8: *stop=0xc908db8u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc908dba; /* mov.l r3,@(r0,r15) */
Lc908dba: *stop=0xc908dbau;r[0]=0x0c908fc4u;goto Lc908dbc; /* mova 0x0c908fc4,r0 */
Lc908dbc: *stop=0xc908dbcu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc908dbe; /* fmov.s @r0,fr5 */
Lc908dbe: *stop=0xc908dbeu;r[0]=88u;goto Lc908dc0; /* mov #0x58,r0 */
Lc908dc0: *stop=0xc908dc0u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908dc2; /* mov.l @(r0,r15),r3 */
Lc908dc2: *stop=0xc908dc2u;r[0]=0x0c908fc8u;goto Lc908dc4; /* mova 0x0c908fc8,r0 */
Lc908dc4: *stop=0xc908dc4u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[3]);goto Lc908dc6; /* fmov.s fr5,@r3 */
Lc908dc6: *stop=0xc908dc6u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto Lc908dc8; /* fmov.s @r0,fr4 */
Lc908dc8: *stop=0xc908dc8u;r[0]=88u;goto Lc908dca; /* mov #0x58,r0 */
Lc908dca: *stop=0xc908dcau;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908dcc; /* mov.l @(r0,r15),r3 */
Lc908dcc: *stop=0xc908dccu;r[0]=4u;goto Lc908dce; /* mov #0x4,r0 */
Lc908dce: *stop=0xc908dceu;r[6]=0u;goto Lc908dd0; /* mov #0x0,r6 */
Lc908dd0: *stop=0xc908dd0u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[3]);goto Lc908dd2; /* fmov fr4,@(r0,r3) */
Lc908dd2: *stop=0xc908dd2u;r[0]=0x0c908fccu;goto Lc908dd4; /* mova 0x0c908fcc,r0 */
Lc908dd4: *stop=0xc908dd4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]);goto Lc908dd6; /* fmov.s @r0,fr6 */
Lc908dd6: *stop=0xc908dd6u;r[0]=88u;goto Lc908dd8; /* mov #0x58,r0 */
Lc908dd8: *stop=0xc908dd8u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908dda; /* mov.l @(r0,r15),r3 */
Lc908dda: *stop=0xc908ddau;r[0]=8u;goto Lc908ddc; /* mov #0x8,r0 */
Lc908ddc: *stop=0xc908ddcu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[3]);goto Lc908dde; /* fmov fr6,@(r0,r3) */
Lc908dde: *stop=0xc908ddeu;r[0]=96u;goto Lc908de0; /* mov #0x60,r0 */
Lc908de0: *stop=0xc908de0u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908de2; /* mov.l @(r0,r15),r3 */
Lc908de2: *stop=0xc908de2u;r[0]=96u;goto Lc908de4; /* mov #0x60,r0 */
Lc908de4: *stop=0xc908de4u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[6],2);goto Lc908de6; /* mov.w r6,@r3 */
Lc908de6: *stop=0xc908de6u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908de8; /* mov.l @(r0,r15),r3 */
Lc908de8: *stop=0xc908de8u;r[0]=r[6];goto Lc908dea; /* mov r6,r0 */
Lc908dea: *stop=0xc908deau;if(!valid_ram(r[3]+2u,2u))return -3;wr(m,r[3]+0x2u,r[0],2);goto Lc908dec; /* mov.w r0,@(0x2,r3) */
Lc908dec: *stop=0xc908decu;r[0]=92u;goto Lc908dee; /* mov #0x5c,r0 */
Lc908dee: *stop=0xc908deeu;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto Lc908df0; /* mov.l @r4,r2 */
Lc908df0: *stop=0xc908df0u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908df2; /* mov.l @(r0,r15),r3 */
Lc908df2: *stop=0xc908df2u;r[0]=88u;goto Lc908df4; /* mov #0x58,r0 */
Lc908df4: *stop=0xc908df4u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto Lc908df6; /* mov.l r2,@r3 */
Lc908df6: *stop=0xc908df6u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908df8; /* mov.l @(r0,r15),r3 */
Lc908df8: *stop=0xc908df8u;r[0]=88u;goto Lc908dfa; /* mov #0x58,r0 */
Lc908dfa: *stop=0xc908dfau;r[3]+=12u;goto Lc908dfc; /* add #0xc,r3 */
Lc908dfc: *stop=0xc908dfcu;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[3]);goto Lc908dfe; /* fmov.s fr5,@r3 */
Lc908dfe: *stop=0xc908dfeu;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e00; /* mov.l @(r0,r15),r3 */
Lc908e00: *stop=0xc908e00u;r[0]=4u;goto Lc908e02; /* mov #0x4,r0 */
Lc908e02: *stop=0xc908e02u;r[3]+=12u;goto Lc908e04; /* add #0xc,r3 */
Lc908e04: *stop=0xc908e04u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[3]);goto Lc908e06; /* fmov fr5,@(r0,r3) */
Lc908e06: *stop=0xc908e06u;r[0]=88u;goto Lc908e08; /* mov #0x58,r0 */
Lc908e08: *stop=0xc908e08u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e0a; /* mov.l @(r0,r15),r3 */
Lc908e0a: *stop=0xc908e0au;r[0]=8u;goto Lc908e0c; /* mov #0x8,r0 */
Lc908e0c: *stop=0xc908e0cu;r[5]=4294967295u;goto Lc908e0e; /* mov #-0x1,r5 */
Lc908e0e: *stop=0xc908e0eu;r[3]+=12u;goto Lc908e10; /* add #0xc,r3 */
Lc908e10: *stop=0xc908e10u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[3]);goto Lc908e12; /* fmov fr6,@(r0,r3) */
Lc908e12: *stop=0xc908e12u;r[0]=96u;goto Lc908e14; /* mov #0x60,r0 */
Lc908e14: *stop=0xc908e14u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e16; /* mov.l @(r0,r15),r3 */
Lc908e16: *stop=0xc908e16u;r[0]=96u;goto Lc908e18; /* mov #0x60,r0 */
Lc908e18: *stop=0xc908e18u;r[5]=r[5]&255u;goto Lc908e1a; /* extu.b r5,r5 */
Lc908e1a: *stop=0xc908e1au;r[3]+=4u;goto Lc908e1c; /* add #0x4,r3 */
Lc908e1c: *stop=0xc908e1cu;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[6],2);goto Lc908e1e; /* mov.w r6,@r3 */
Lc908e1e: *stop=0xc908e1eu;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e20; /* mov.l @(r0,r15),r3 */
Lc908e20: *stop=0xc908e20u;r[0]=r[5];goto Lc908e22; /* mov r5,r0 */
Lc908e22: *stop=0xc908e22u;r[3]+=4u;goto Lc908e24; /* add #0x4,r3 */
Lc908e24: *stop=0xc908e24u;if(!valid_ram(r[3]+2u,2u))return -3;wr(m,r[3]+0x2u,r[0],2);goto Lc908e26; /* mov.w r0,@(0x2,r3) */
Lc908e26: *stop=0xc908e26u;r[0]=92u;goto Lc908e28; /* mov #0x5c,r0 */
Lc908e28: *stop=0xc908e28u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e2a; /* mov.l @(r0,r15),r3 */
Lc908e2a: *stop=0xc908e2au;r[0]=88u;goto Lc908e2c; /* mov #0x58,r0 */
Lc908e2c: *stop=0xc908e2cu;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto Lc908e2e; /* mov.l @r4,r2 */
Lc908e2e: *stop=0xc908e2eu;r[3]+=4u;goto Lc908e30; /* add #0x4,r3 */
Lc908e30: *stop=0xc908e30u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto Lc908e32; /* mov.l r2,@r3 */
Lc908e32: *stop=0xc908e32u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e34; /* mov.l @(r0,r15),r3 */
Lc908e34: *stop=0xc908e34u;r[0]=88u;goto Lc908e36; /* mov #0x58,r0 */
Lc908e36: *stop=0xc908e36u;r[3]+=24u;goto Lc908e38; /* add #0x18,r3 */
Lc908e38: *stop=0xc908e38u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[3]);goto Lc908e3a; /* fmov.s fr4,@r3 */
Lc908e3a: *stop=0xc908e3au;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e3c; /* mov.l @(r0,r15),r3 */
Lc908e3c: *stop=0xc908e3cu;r[0]=4u;goto Lc908e3e; /* mov #0x4,r0 */
Lc908e3e: *stop=0xc908e3eu;r[3]+=24u;goto Lc908e40; /* add #0x18,r3 */
Lc908e40: *stop=0xc908e40u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[3]);goto Lc908e42; /* fmov fr5,@(r0,r3) */
Lc908e42: *stop=0xc908e42u;r[0]=88u;goto Lc908e44; /* mov #0x58,r0 */
Lc908e44: *stop=0xc908e44u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e46; /* mov.l @(r0,r15),r3 */
Lc908e46: *stop=0xc908e46u;r[3]+=24u;goto Lc908e48; /* add #0x18,r3 */
Lc908e48: *stop=0xc908e48u;r[0]=8u;goto Lc908e4a; /* mov #0x8,r0 */
Lc908e4a: *stop=0xc908e4au;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[3]);goto Lc908e4c; /* fmov fr6,@(r0,r3) */
Lc908e4c: *stop=0xc908e4cu;r[0]=96u;goto Lc908e4e; /* mov #0x60,r0 */
Lc908e4e: *stop=0xc908e4eu;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e50; /* mov.l @(r0,r15),r3 */
Lc908e50: *stop=0xc908e50u;r[0]=96u;goto Lc908e52; /* mov #0x60,r0 */
Lc908e52: *stop=0xc908e52u;r[3]+=8u;goto Lc908e54; /* add #0x8,r3 */
Lc908e54: *stop=0xc908e54u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[5],2);goto Lc908e56; /* mov.w r5,@r3 */
Lc908e56: *stop=0xc908e56u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e58; /* mov.l @(r0,r15),r3 */
Lc908e58: *stop=0xc908e58u;r[0]=r[5];goto Lc908e5a; /* mov r5,r0 */
Lc908e5a: *stop=0xc908e5au;r[3]+=8u;goto Lc908e5c; /* add #0x8,r3 */
Lc908e5c: *stop=0xc908e5cu;if(!valid_ram(r[3]+2u,2u))return -3;wr(m,r[3]+0x2u,r[0],2);goto Lc908e5e; /* mov.w r0,@(0x2,r3) */
Lc908e5e: *stop=0xc908e5eu;r[0]=92u;goto Lc908e60; /* mov #0x5c,r0 */
Lc908e60: *stop=0xc908e60u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e62; /* mov.l @(r0,r15),r3 */
Lc908e62: *stop=0xc908e62u;r[0]=88u;goto Lc908e64; /* mov #0x58,r0 */
Lc908e64: *stop=0xc908e64u;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto Lc908e66; /* mov.l @r4,r2 */
Lc908e66: *stop=0xc908e66u;r[3]+=8u;goto Lc908e68; /* add #0x8,r3 */
Lc908e68: *stop=0xc908e68u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto Lc908e6a; /* mov.l r2,@r3 */
Lc908e6a: *stop=0xc908e6au;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e6c; /* mov.l @(r0,r15),r3 */
Lc908e6c: *stop=0xc908e6cu;r[0]=88u;goto Lc908e6e; /* mov #0x58,r0 */
Lc908e6e: *stop=0xc908e6eu;r[3]+=36u;goto Lc908e70; /* add #0x24,r3 */
Lc908e70: *stop=0xc908e70u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[3]);goto Lc908e72; /* fmov.s fr4,@r3 */
Lc908e72: *stop=0xc908e72u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e74; /* mov.l @(r0,r15),r3 */
Lc908e74: *stop=0xc908e74u;r[0]=4u;goto Lc908e76; /* mov #0x4,r0 */
Lc908e76: *stop=0xc908e76u;r[3]+=36u;goto Lc908e78; /* add #0x24,r3 */
Lc908e78: *stop=0xc908e78u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[3]);goto Lc908e7a; /* fmov fr4,@(r0,r3) */
Lc908e7a: *stop=0xc908e7au;r[0]=88u;goto Lc908e7c; /* mov #0x58,r0 */
Lc908e7c: *stop=0xc908e7cu;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e7e; /* mov.l @(r0,r15),r3 */
Lc908e7e: *stop=0xc908e7eu;r[0]=8u;goto Lc908e80; /* mov #0x8,r0 */
Lc908e80: *stop=0xc908e80u;r[3]+=36u;goto Lc908e82; /* add #0x24,r3 */
Lc908e82: *stop=0xc908e82u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[3]);goto Lc908e84; /* fmov fr6,@(r0,r3) */
Lc908e84: *stop=0xc908e84u;r[0]=96u;goto Lc908e86; /* mov #0x60,r0 */
Lc908e86: *stop=0xc908e86u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e88; /* mov.l @(r0,r15),r3 */
Lc908e88: *stop=0xc908e88u;r[3]+=12u;goto Lc908e8a; /* add #0xc,r3 */
Lc908e8a: *stop=0xc908e8au;r[0]=96u;goto Lc908e8c; /* mov #0x60,r0 */
Lc908e8c: *stop=0xc908e8cu;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[5],2);goto Lc908e8e; /* mov.w r5,@r3 */
Lc908e8e: *stop=0xc908e8eu;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e90; /* mov.l @(r0,r15),r3 */
Lc908e90: *stop=0xc908e90u;r[0]=r[6];goto Lc908e92; /* mov r6,r0 */
Lc908e92: *stop=0xc908e92u;r[3]+=12u;goto Lc908e94; /* add #0xc,r3 */
Lc908e94: *stop=0xc908e94u;if(!valid_ram(r[3]+2u,2u))return -3;wr(m,r[3]+0x2u,r[0],2);goto Lc908e96; /* mov.w r0,@(0x2,r3) */
Lc908e96: *stop=0xc908e96u;r[0]=92u;goto Lc908e98; /* mov #0x5c,r0 */
Lc908e98: *stop=0xc908e98u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc908e9a; /* mov.l @(r0,r15),r3 */
Lc908e9a: *stop=0xc908e9au;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto Lc908e9c; /* mov.l @r4,r2 */
Lc908e9c: *stop=0xc908e9cu;r[3]+=12u;goto Lc908e9e; /* add #0xc,r3 */
Lc908e9e: *stop=0xc908e9eu;r[4]=rd(m,0x0c908fd0u,4);goto Lc908ea0; /* mov.l 0x0c908fd0,r4 */
Lc908ea0: *stop=0xc908ea0u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto Lc908ea2; /* mov.l r2,@r3 */
Lc908ea2: *stop=0xc908ea2u;r[3]=rd(m,0x0c908fd4u,4);goto Lc908ea4; /* mov.l 0x0c908fd4,r3 */
Lc908ea4: target=r[3];*pr=0x0c908ea8u;*stop=0xc908ea6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[3],&f->fpscr,&fr[15]);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908ea8; /* jsr @r3 */
Lc908ea8: *stop=0xc908ea8u;r[2]=rd(m,0x0c908fdcu,4);goto Lc908eaa; /* mov.l 0x0c908fdc,r2 */
Lc908eaa: *stop=0xc908eaau;r[4]=rd(m,0x0c908fd8u,4);goto Lc908eac; /* mov.l 0x0c908fd8,r4 */
Lc908eac: target=r[2];*pr=0x0c908eb0u;*stop=0xc908eaeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908eb0; /* jsr @r2 */
Lc908eb0: *stop=0xc908eb0u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto Lc908eb2; /* mov.l @r15,r5 */
Lc908eb2: *stop=0xc908eb2u;r[3]=rd(m,0x0c908fe0u,4);goto Lc908eb4; /* mov.l 0x0c908fe0,r3 */
Lc908eb4: *stop=0xc908eb4u;r[5]+=12u;goto Lc908eb6; /* add #0xc,r5 */
Lc908eb6: target=r[3];*pr=0x0c908ebau;*stop=0xc908eb8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908eba; /* jsr @r3 */
Lc908eba: *stop=0xc908ebau;fcopy(f,5u,15u);goto Lc908ebc; /* fmov fr15,fr5 */
Lc908ebc: *stop=0xc908ebcu;r[2]=rd(m,0x0c908fe4u,4);goto Lc908ebe; /* mov.l 0x0c908fe4,r2 */
Lc908ebe: *stop=0xc908ebeu;fcopy(f,4u,15u);goto Lc908ec0; /* fmov fr15,fr4 */
Lc908ec0: *stop=0xc908ec0u;fcopy(f,6u,15u);goto Lc908ec2; /* fmov fr15,fr6 */
Lc908ec2: *stop=0xc908ec2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto Lc908ec4; /* fneg fr5 */
Lc908ec4: target=r[2];*pr=0x0c908ec8u;*stop=0xc908ec6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908ec8; /* jsr @r2 */
Lc908ec8: *stop=0xc908ec8u;r[3]=rd(m,0x0c908fecu,4);goto Lc908eca; /* mov.l 0x0c908fec,r3 */
Lc908eca: *stop=0xc908ecau;r[0]=0x0c908fe8u;goto Lc908ecc; /* mova 0x0c908fe8,r0 */
Lc908ecc: target=r[3];*pr=0x0c908ed0u;*stop=0xc908eceu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908ed0; /* jsr @r3 */
Lc908ed0: *stop=0xc908ed0u;*fpul=r[0];goto Lc908ed2; /* lds r0,FPUL */
Lc908ed2: *stop=0xc908ed2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc908ed4; /* float FPUL,fr3 */
Lc908ed4: *stop=0xc908ed4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto Lc908ed6; /* fmul fr15,fr3 */
Lc908ed6: *stop=0xc908ed6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto Lc908ed8; /* ftrc fr3,FPUL */
Lc908ed8: *stop=0xc908ed8u;r[3]=*fpul;goto Lc908eda; /* sts FPUL,r3 */
Lc908eda: *stop=0xc908edau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc908edc; /* tst r3,r3 */
Lc908edc: if((*sr&1u))goto Lc908ef4;goto Lc908ede; /* bt 0x0c908ef4 */
Lc908ede: *stop=0xc908edeu;r[3]=rd(m,0x0c908fecu,4);goto Lc908ee0; /* mov.l 0x0c908fec,r3 */
Lc908ee0: target=r[3];*pr=0x0c908ee4u;*stop=0xc908ee2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908ee4; /* jsr @r3 */
Lc908ee4: *stop=0xc908ee4u;*fpul=r[0];goto Lc908ee6; /* lds r0,FPUL */
Lc908ee6: *stop=0xc908ee6u;r[3]=rd(m,0x0c908ff0u,4);goto Lc908ee8; /* mov.l 0x0c908ff0,r3 */
Lc908ee8: *stop=0xc908ee8u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc908eea; /* float FPUL,fr3 */
Lc908eea: *stop=0xc908eeau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto Lc908eec; /* fmul fr15,fr3 */
Lc908eec: *stop=0xc908eecu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto Lc908eee; /* ftrc fr3,FPUL */
Lc908eee: *stop=0xc908eeeu;r[5]=*fpul;goto Lc908ef0; /* sts FPUL,r5 */
Lc908ef0: target=r[3];*pr=0x0c908ef4u;*stop=0xc908ef2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908ef4; /* jsr @r3 */
Lc908ef4: *stop=0xc908ef4u;r[3]=4294967172u;goto Lc908ef6; /* mov #-0x7c,r3 */
Lc908ef6: *stop=0xc908ef6u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto Lc908ef8; /* mov.l @r15,r5 */
Lc908ef8: *stop=0xc908ef8u;r[2]=rd(m,0x0c908fe0u,4);goto Lc908efa; /* mov.l 0x0c908fe0,r2 */
Lc908efa: *stop=0xc908efau;r[3]=r[3]&255u;goto Lc908efc; /* extu.b r3,r3 */
Lc908efc: *stop=0xc908efcu;r[5]+=r[3];goto Lc908efe; /* add r3,r5 */
Lc908efe: target=r[2];*pr=0x0c908f02u;*stop=0xc908f00u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908f02; /* jsr @r2 */
Lc908f02: *stop=0xc908f02u;r[14]=rd(m,0x0c908ff4u,4);goto Lc908f04; /* mov.l 0x0c908ff4,r14 */
Lc908f04: *stop=0xc908f04u;r[5]=8u;goto Lc908f06; /* mov #0x8,r5 */
Lc908f06: target=r[14];*pr=0x0c908f0au;*stop=0xc908f08u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908f0a; /* jsr @r14 */
Lc908f0a: *stop=0xc908f0au;r[5]=10u;goto Lc908f0c; /* mov #0xa,r5 */
Lc908f0c: target=r[14];*pr=0x0c908f10u;*stop=0xc908f0eu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908f10; /* jsr @r14 */
Lc908f10: *stop=0xc908f10u;r[3]=rd(m,0x0c908ff8u,4);goto Lc908f12; /* mov.l 0x0c908ff8,r3 */
Lc908f12: target=r[3];*pr=0x0c908f16u;*stop=0xc908f14u;r[4]=11u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908f16; /* jsr @r3 */
Lc908f16: *stop=0xc908f16u;r[2]=rd(m,0x0c909000u,4);goto Lc908f18; /* mov.l 0x0c909000,r2 */
Lc908f18: *stop=0xc908f18u;r[4]=r[15];goto Lc908f1a; /* mov r15,r4 */
Lc908f1a: *stop=0xc908f1au;r[6]=rd(m,0x0c908ffcu,4);goto Lc908f1c; /* mov.l 0x0c908ffc,r6 */
Lc908f1c: *stop=0xc908f1cu;r[5]=4u;goto Lc908f1e; /* mov #0x4,r5 */
Lc908f1e: target=r[2];*pr=0x0c908f22u;*stop=0xc908f20u;r[4]+=88u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908f22; /* jsr @r2 */
Lc908f22: *stop=0xc908f22u;r[5]=8u;goto Lc908f24; /* mov #0x8,r5 */
Lc908f24: target=r[14];*pr=0x0c908f28u;*stop=0xc908f26u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908f28; /* jsr @r14 */
Lc908f28: *stop=0xc908f28u;r[5]=6u;goto Lc908f2a; /* mov #0x6,r5 */
Lc908f2a: target=r[14];*pr=0x0c908f2eu;*stop=0xc908f2cu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908f2e; /* jsr @r14 */
Lc908f2e: *stop=0xc908f2eu;r[3]=rd(m,0x0c909004u,4);goto Lc908f30; /* mov.l 0x0c909004,r3 */
Lc908f30: target=r[3];*pr=0x0c908f34u;*stop=0xc908f32u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908f34; /* jsr @r3 */
Lc908f34: *stop=0xc908f34u;r[15]+=104u;goto Lc908f36; /* add #0x68,r15 */
Lc908f36: *stop=0xc908f36u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc908f38; /* lds.l @r15+,PR */
Lc908f38: *stop=0xc908f38u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc908f3a; /* fmov.s @r15+,fr15 */
Lc908f3a: *stop=0xc908f3au;if(*pr!=entry_pr)return -3;*stop=0xc908f3cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_material_mode_8c605628(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c605628;
L8c605628: *stop=0x8c605628u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60562a; /* mov.l r14,@-r15 */
L8c60562a: *stop=0x8c60562au;r[0]=r[5];goto L8c60562c; /* mov r5,r0 */
L8c60562c: *stop=0x8c60562cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60562e; /* sts.l PR,@-r15 */
L8c60562e: *stop=0x8c60562eu;r[1]=12u;goto L8c605630; /* mov #0xc,r1 */
L8c605630: *stop=0x8c605630u;*sr=(*sr&~1u)|(r[0]>=r[1]);goto L8c605632; /* cmp/hs r1,r0 */
L8c605632: if((*sr&1u))goto L8c605686;goto L8c605634; /* bt 0x8c605686 */
L8c605634: *stop=0x8c605634u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c605636; /* shll r0 */
L8c605636: *stop=0x8c605636u;r[1]=r[0];goto L8c605638; /* mov r0,r1 */
L8c605638: *stop=0x8c605638u;r[0]=0x8c605640u;goto L8c60563a; /* mova 0x8c605640,r0 */
L8c60563a: *stop=0x8c60563au;if(!valid_ram(r[0]+r[1],2u))return -3;r[0]=sx(m,r[0]+r[1],2);goto L8c60563c; /* mov.w @(r0,r1),r0 */
L8c60563c: target=0x8c605640u+r[0];*stop=0x8c60563eu;switch(target){case 0x8c605658u:goto L8c605658;case 0x8c60565cu:goto L8c60565c;case 0x8c605660u:goto L8c605660;case 0x8c605664u:goto L8c605664;case 0x8c605668u:goto L8c605668;case 0x8c60566cu:goto L8c60566c;case 0x8c605670u:goto L8c605670;case 0x8c605674u:goto L8c605674;case 0x8c605678u:goto L8c605678;case 0x8c60567cu:goto L8c60567c;case 0x8c605680u:goto L8c605680;case 0x8c605684u:goto L8c605684;default:*stop=0x8c60563cu;return -3;} /* braf r0 */
L8c605658: *stop=0x8c60565au;r[5]=0u;goto L8c605686; /* bra 0x8c605686 */
L8c60565c: *stop=0x8c60565eu;r[5]=1u;goto L8c605686; /* bra 0x8c605686 */
L8c605660: *stop=0x8c605662u;r[5]=2u;goto L8c605686; /* bra 0x8c605686 */
L8c605664: *stop=0x8c605666u;r[5]=3u;goto L8c605686; /* bra 0x8c605686 */
L8c605668: *stop=0x8c60566au;r[5]=4u;goto L8c605686; /* bra 0x8c605686 */
L8c60566c: *stop=0x8c60566eu;r[5]=5u;goto L8c605686; /* bra 0x8c605686 */
L8c605670: *stop=0x8c605672u;r[5]=6u;goto L8c605686; /* bra 0x8c605686 */
L8c605674: *stop=0x8c605676u;r[5]=7u;goto L8c605686; /* bra 0x8c605686 */
L8c605678: *stop=0x8c60567au;r[5]=8u;goto L8c605686; /* bra 0x8c605686 */
L8c60567c: *stop=0x8c60567eu;r[5]=9u;goto L8c605686; /* bra 0x8c605686 */
L8c605680: *stop=0x8c605682u;r[5]=10u;goto L8c605686; /* bra 0x8c605686 */
L8c605684: *stop=0x8c605684u;r[5]=11u;goto L8c605686; /* mov #0xb,r5 */
L8c605686: *stop=0x8c605686u;r[14]=rd(m,0x8c605730u,4);goto L8c605688; /* mov.l 0x8c605730,r14 */
L8c605688: *stop=0x8c605688u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c60568a; /* tst r4,r4 */
L8c60568a: take=!(*sr&1u);*stop=0x8c60568cu;r[0]=r[4];if(take)goto L8c605698;goto L8c60568e; /* bf/s 0x8c605698 */
L8c60568e: *stop=0x8c60568eu;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c605690; /* mov.l @r14,r2 */
L8c605690: *stop=0x8c605690u;r[3]=64u;goto L8c605692; /* mov #0x40,r3 */
L8c605692: *stop=0x8c605692u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c605694; /* mov.l r3,@r2 */
L8c605694: *stop=0x8c605694u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c605696; /* mov.l @r14,r2 */
L8c605696: *stop=0x8c605696u;if(!valid_ram(r[2]+44u,4u))return -3;wr(m,r[2]+0x2cu,r[5],4);goto L8c605698; /* mov.l r5,@(0x2c,r2) */
L8c605698: *stop=0x8c605698u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c60569a; /* cmp/eq #0x1,r0 */
L8c60569a: if(!(*sr&1u))goto L8c6056a6;goto L8c60569c; /* bf 0x8c6056a6 */
L8c60569c: *stop=0x8c60569cu;r[3]=sx(m,0x8c60572au,2);goto L8c60569e; /* mov.w 0x8c60572a,r3 */
L8c60569e: *stop=0x8c60569eu;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c6056a0; /* mov.l @r14,r2 */
L8c6056a0: *stop=0x8c6056a0u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c6056a2; /* mov.l r3,@r2 */
L8c6056a2: *stop=0x8c6056a2u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c6056a4; /* mov.l @r14,r2 */
L8c6056a4: *stop=0x8c6056a4u;if(!valid_ram(r[2]+48u,4u))return -3;wr(m,r[2]+0x30u,r[5],4);goto L8c6056a6; /* mov.l r5,@(0x30,r2) */
L8c6056a6: *stop=0x8c6056a6u;r[3]=rd(m,0x8c605734u,4);goto L8c6056a8; /* mov.l 0x8c605734,r3 */
L8c6056a8: target=r[3];*pr=0x8c6056acu;*stop=0x8c6056aau;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6056ac; /* jsr @r3 */
L8c6056ac: *stop=0x8c6056acu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c6056ae; /* lds.l @r15+,PR */
L8c6056ae: *stop=0x8c6056aeu;r[2]=rd(m,0x8c605738u,4);goto L8c6056b0; /* mov.l 0x8c605738,r2 */
L8c6056b0: *stop=0x8c6056b0u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);goto L8c6056b2; /* mov.l @r14,r4 */
L8c6056b2: target=r[2];*stop=0x8c6056b4u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_object_effect_texture_select_8c608d14(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c608d14;
L8c608d14: *stop=0x8c608d14u;r[2]=rd(m,0x8c608e3cu,4);goto L8c608d16; /* mov.l 0x8c608e3c,r2 */
L8c608d16: *stop=0x8c608d16u;r[1]=r[4];goto L8c608d18; /* mov r4,r1 */
L8c608d18: *stop=0x8c608d18u;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c608d1a; /* shll r4 */
L8c608d1a: *stop=0x8c608d1au;r[5]=rd(m,0x8c608e40u,4);goto L8c608d1c; /* mov.l 0x8c608e40,r5 */
L8c608d1c: *stop=0x8c608d1cu;r[4]+=r[1];goto L8c608d1e; /* add r1,r4 */
L8c608d1e: *stop=0x8c608d1eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c608d20; /* mov.l r14,@-r15 */
L8c608d20: *stop=0x8c608d20u;r[4]<<=2;goto L8c608d22; /* shll2 r4 */
L8c608d22: *stop=0x8c608d22u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c608d24; /* sts.l PR,@-r15 */
L8c608d24: *stop=0x8c608d24u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c608d26; /* mov.l @r2,r3 */
L8c608d26: *stop=0x8c608d26u;if(!valid_ram(r[5],4u))return -3;r[1]=rd(m,r[5],4);goto L8c608d28; /* mov.l @r5,r1 */
L8c608d28: *stop=0x8c608d28u;if(!valid_ram(r[3],4u))return -3;r[3]=rd(m,r[3],4);goto L8c608d2a; /* mov.l @r3,r3 */
L8c608d2a: *stop=0x8c608d2au;r[4]+=r[3];goto L8c608d2c; /* add r3,r4 */
L8c608d2c: *stop=0x8c608d2cu;if(!valid_ram(r[4]+8u,4u))return -3;r[4]=rd(m,r[4]+0x8u,4);goto L8c608d2e; /* mov.l @(0x8,r4),r4 */
L8c608d2e: *stop=0x8c608d2eu;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c608d30; /* mov.l @r4,r3 */
L8c608d30: *stop=0x8c608d30u;*sr=(*sr&~1u)|(r[1]==r[3]);goto L8c608d32; /* cmp/eq r1,r3 */
L8c608d32: if(!(*sr&1u))goto L8c608d3e;goto L8c608d34; /* bf 0x8c608d3e */
L8c608d34: *stop=0x8c608d34u;r[1]=rd(m,0x8c608e44u,4);goto L8c608d36; /* mov.l 0x8c608e44,r1 */
L8c608d36: *stop=0x8c608d36u;if(!valid_ram(r[4]+4u,4u))return -3;r[3]=rd(m,r[4]+0x4u,4);goto L8c608d38; /* mov.l @(0x4,r4),r3 */
L8c608d38: *stop=0x8c608d38u;if(!valid_ram(r[1],4u))return -3;r[0]=rd(m,r[1],4);goto L8c608d3a; /* mov.l @r1,r0 */
L8c608d3a: *stop=0x8c608d3au;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c608d3c; /* cmp/eq r3,r0 */
L8c608d3c: if((*sr&1u))goto L8c608d80;goto L8c608d3e; /* bt 0x8c608d80 */
L8c608d3e: *stop=0x8c608d3eu;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c608d40; /* mov.l @r4,r2 */
L8c608d40: *stop=0x8c608d40u;r[14]=rd(m,0x8c608e48u,4);goto L8c608d42; /* mov.l 0x8c608e48,r14 */
L8c608d42: *stop=0x8c608d42u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c608d44; /* mov.l r2,@r5 */
L8c608d44: *stop=0x8c608d44u;r[0]=sx(m,0x8c608e36u,2);goto L8c608d46; /* mov.w 0x8c608e36,r0 */
L8c608d46: *stop=0x8c608d46u;if(!valid_ram(r[4]+4u,4u))return -3;r[3]=rd(m,r[4]+0x4u,4);goto L8c608d48; /* mov.l @(0x4,r4),r3 */
L8c608d48: *stop=0x8c608d48u;r[1]=rd(m,0x8c608e44u,4);goto L8c608d4a; /* mov.l 0x8c608e44,r1 */
L8c608d4a: *stop=0x8c608d4au;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[3],4);goto L8c608d4c; /* mov.l r3,@r1 */
L8c608d4c: *stop=0x8c608d4cu;r[3]=4294967232u;goto L8c608d4e; /* mov #-0x40,r3 */
L8c608d4e: *stop=0x8c608d4eu;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c608d50; /* mov.l @r14,r5 */
L8c608d50: *stop=0x8c608d50u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c608d52; /* mov.l @r14,r2 */
L8c608d52: *stop=0x8c608d52u;if(!valid_ram(r[0]+r[5],4u))return -3;r[5]=rd(m,r[5]+r[0],4);goto L8c608d54; /* mov.l @(r0,r5),r5 */
L8c608d54: *stop=0x8c608d54u;if(!valid_ram(r[4]+8u,4u))return -3;r[0]=rd(m,r[4]+0x8u,4);goto L8c608d56; /* mov.l @(0x8,r4),r0 */
L8c608d56: *stop=0x8c608d56u;r[5]&=r[3];goto L8c608d58; /* and r3,r5 */
L8c608d58: *stop=0x8c608d58u;r[3]=rd(m,0x8c608e50u,4);goto L8c608d5a; /* mov.l 0x8c608e50,r3 */
L8c608d5a: *stop=0x8c608d5au;r[0]&=0x3fu;goto L8c608d5c; /* and #0x3f,r0 */
L8c608d5c: *stop=0x8c608d5cu;r[5]|=r[0];goto L8c608d5e; /* or r0,r5 */
L8c608d5e: *stop=0x8c608d5eu;r[0]=sx(m,0x8c608e36u,2);goto L8c608d60; /* mov.w 0x8c608e36,r0 */
L8c608d60: *stop=0x8c608d60u;if(!valid_ram(r[0]+r[2],4u))return -3;wr(m,r[2]+r[0],r[5],4);goto L8c608d62; /* mov.l r5,@(r0,r2) */
L8c608d62: *stop=0x8c608d62u;r[2]=rd(m,0x8c608e4cu,4);goto L8c608d64; /* mov.l 0x8c608e4c,r2 */
L8c608d64: *stop=0x8c608d64u;if(!valid_ram(r[4]+12u,4u))return -3;r[4]=rd(m,r[4]+0xcu,4);goto L8c608d66; /* mov.l @(0xc,r4),r4 */
L8c608d66: *stop=0x8c608d66u;r[2]&=r[4];goto L8c608d68; /* and r4,r2 */
L8c608d68: *stop=0x8c608d68u;*sr=(*sr&~1u)|(r[2]>r[3]);goto L8c608d6a; /* cmp/hi r3,r2 */
L8c608d6a: if(!(*sr&1u))goto L8c608d74;goto L8c608d6c; /* bf 0x8c608d74 */
L8c608d6c: *stop=0x8c608d6cu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c608d6e; /* lds.l @r15+,PR */
L8c608d6e: *stop=0x8c608d6eu;r[0]=4294967295u;goto L8c608d70; /* mov #-0x1,r0 */
L8c608d70: *stop=0x8c608d70u;if(*pr!=entry_pr)return -3;*stop=0x8c608d72u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c608d74: *stop=0x8c608d74u;r[0]=sx(m,0x8c608e38u,2);goto L8c608d76; /* mov.w 0x8c608e38,r0 */
L8c608d76: *stop=0x8c608d76u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c608d78; /* mov.l @r14,r3 */
L8c608d78: *stop=0x8c608d78u;if(!valid_ram(r[0]+r[3],4u))return -3;wr(m,r[3]+r[0],r[4],4);goto L8c608d7a; /* mov.l r4,@(r0,r3) */
L8c608d7a: *stop=0x8c608d7au;r[3]=rd(m,0x8c608e54u,4);goto L8c608d7c; /* mov.l 0x8c608e54,r3 */
L8c608d7c: target=r[3];*pr=0x8c608d80u;*stop=0x8c608d7eu;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608d80; /* jsr @r3 */
L8c608d80: *stop=0x8c608d80u;r[0]=1u;goto L8c608d82; /* mov #0x1,r0 */
L8c608d82: *stop=0x8c608d82u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c608d84; /* lds.l @r15+,PR */
L8c608d84: *stop=0x8c608d84u;if(*pr!=entry_pr)return -3;*stop=0x8c608d86u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_polygon_array_8c63d414(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c63d414;
L8c63d414: *stop=0x8c63d414u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c63d416; /* mov.l r14,@-r15 */
L8c63d416: *stop=0x8c63d416u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c63d418; /* mov.l r13,@-r15 */
L8c63d418: *stop=0x8c63d418u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c63d41a; /* mov.l r12,@-r15 */
L8c63d41a: *stop=0x8c63d41au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c63d41c; /* mov.l r11,@-r15 */
L8c63d41c: *stop=0x8c63d41cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c63d41e; /* mov.l r10,@-r15 */
L8c63d41e: *stop=0x8c63d41eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c63d420; /* mov.l r9,@-r15 */
L8c63d420: *stop=0x8c63d420u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c63d422; /* mov.l r8,@-r15 */
L8c63d422: *stop=0x8c63d422u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c63d424; /* sts.l PR,@-r15 */
L8c63d424: *stop=0x8c63d424u;r[0]=sx(m,0x8c63d4f0u,2);goto L8c63d426; /* mov.w 0x8c63d4f0,r0 */
L8c63d426: *stop=0x8c63d426u;r[15]+=r[0];goto L8c63d428; /* add r0,r15 */
L8c63d428: *stop=0x8c63d428u;r[3]=sx(m,0x8c63d4f2u,2);goto L8c63d42a; /* mov.w 0x8c63d4f2,r3 */
L8c63d42a: *stop=0x8c63d42au;r[14]=r[4];goto L8c63d42c; /* mov r4,r14 */
L8c63d42c: *stop=0x8c63d42cu;r[0]=sx(m,0x8c63d4f4u,2);goto L8c63d42e; /* mov.w 0x8c63d4f4,r0 */
L8c63d42e: *stop=0x8c63d42eu;r[12]=r[5];goto L8c63d430; /* mov r5,r12 */
L8c63d430: *stop=0x8c63d430u;r[3]+=r[15];goto L8c63d432; /* add r15,r3 */
L8c63d432: *stop=0x8c63d432u;r[2]=sx(m,0x8c63d4f6u,2);goto L8c63d434; /* mov.w 0x8c63d4f6,r2 */
L8c63d434: *stop=0x8c63d434u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto L8c63d436; /* mov.l r3,@(r0,r15) */
L8c63d436: *stop=0x8c63d436u;r[11]=r[6];goto L8c63d438; /* mov r6,r11 */
L8c63d438: *stop=0x8c63d438u;r[0]=sx(m,0x8c63d4f8u,2);goto L8c63d43a; /* mov.w 0x8c63d4f8,r0 */
L8c63d43a: *stop=0x8c63d43au;r[2]+=r[15];goto L8c63d43c; /* add r15,r2 */
L8c63d43c: *stop=0x8c63d43cu;r[3]=sx(m,0x8c63d4fau,2);goto L8c63d43e; /* mov.w 0x8c63d4fa,r3 */
L8c63d43e: *stop=0x8c63d43eu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto L8c63d440; /* mov.l r2,@(r0,r15) */
L8c63d440: *stop=0x8c63d440u;r[0]=sx(m,0x8c63d4fcu,2);goto L8c63d442; /* mov.w 0x8c63d4fc,r0 */
L8c63d442: *stop=0x8c63d442u;r[3]+=r[15];goto L8c63d444; /* add r15,r3 */
L8c63d444: *stop=0x8c63d444u;r[2]=sx(m,0x8c63d4feu,2);goto L8c63d446; /* mov.w 0x8c63d4fe,r2 */
L8c63d446: *stop=0x8c63d446u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto L8c63d448; /* mov.l r3,@(r0,r15) */
L8c63d448: *stop=0x8c63d448u;r[0]=sx(m,0x8c63d500u,2);goto L8c63d44a; /* mov.w 0x8c63d500,r0 */
L8c63d44a: *stop=0x8c63d44au;r[2]+=r[15];goto L8c63d44c; /* add r15,r2 */
L8c63d44c: *stop=0x8c63d44cu;r[3]=sx(m,0x8c63d502u,2);goto L8c63d44e; /* mov.w 0x8c63d502,r3 */
L8c63d44e: *stop=0x8c63d44eu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto L8c63d450; /* mov.l r2,@(r0,r15) */
L8c63d450: *stop=0x8c63d450u;r[0]=sx(m,0x8c63d504u,2);goto L8c63d452; /* mov.w 0x8c63d504,r0 */
L8c63d452: *stop=0x8c63d452u;r[3]+=r[15];goto L8c63d454; /* add r15,r3 */
L8c63d454: *stop=0x8c63d454u;r[2]=sx(m,0x8c63d506u,2);goto L8c63d456; /* mov.w 0x8c63d506,r2 */
L8c63d456: *stop=0x8c63d456u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto L8c63d458; /* mov.l r3,@(r0,r15) */
L8c63d458: *stop=0x8c63d458u;r[4]=sx(m,0x8c63d50au,2);goto L8c63d45a; /* mov.w 0x8c63d50a,r4 */
L8c63d45a: *stop=0x8c63d45au;r[2]+=r[15];goto L8c63d45c; /* add r15,r2 */
L8c63d45c: *stop=0x8c63d45cu;r[3]=sx(m,0x8c63d50cu,2);goto L8c63d45e; /* mov.w 0x8c63d50c,r3 */
L8c63d45e: *stop=0x8c63d45eu;r[0]=sx(m,0x8c63d508u,2);goto L8c63d460; /* mov.w 0x8c63d508,r0 */
L8c63d460: *stop=0x8c63d460u;r[4]+=r[15];goto L8c63d462; /* add r15,r4 */
L8c63d462: *stop=0x8c63d462u;r[3]+=r[15];goto L8c63d464; /* add r15,r3 */
L8c63d464: *stop=0x8c63d464u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto L8c63d466; /* mov.l r2,@(r0,r15) */
L8c63d466: *stop=0x8c63d466u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[3],4);goto L8c63d468; /* mov.l r3,@r4 */
L8c63d468: *stop=0x8c63d468u;r[2]=sx(m,0x8c63d50eu,2);goto L8c63d46a; /* mov.w 0x8c63d50e,r2 */
L8c63d46a: *stop=0x8c63d46au;r[3]=r[15];goto L8c63d46c; /* mov r15,r3 */
L8c63d46c: *stop=0x8c63d46cu;r[10]=0u;goto L8c63d46e; /* mov #0x0,r10 */
L8c63d46e: *stop=0x8c63d46eu;r[8]=rd(m,0x8c63d510u,4);goto L8c63d470; /* mov.l 0x8c63d510,r8 */
L8c63d470: *stop=0x8c63d470u;r[3]+=4u;goto L8c63d472; /* add #0x4,r3 */
L8c63d472: *stop=0x8c63d472u;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c63d474; /* cmp/pl r12 */
L8c63d474: *stop=0x8c63d474u;r[2]+=r[15];goto L8c63d476; /* add r15,r2 */
L8c63d476: *stop=0x8c63d476u;r[9]=r[10];goto L8c63d478; /* mov r10,r9 */
L8c63d478: *stop=0x8c63d478u;if(!valid_ram(r[4]+4u,4u))return -3;wr(m,r[4]+0x4u,r[2],4);goto L8c63d47a; /* mov.l r2,@(0x4,r4) */
L8c63d47a: *stop=0x8c63d47au;if(!valid_ram(r[4]+8u,4u))return -3;wr(m,r[4]+0x8u,r[3],4);goto L8c63d47c; /* mov.l r3,@(0x8,r4) */
L8c63d47c: take=!(*sr&1u);*stop=0x8c63d47eu;r[13]=r[10];if(take)goto L8c63d496;goto L8c63d480; /* bf/s 0x8c63d496 */
L8c63d480: *stop=0x8c63d480u;r[0]=sx(m,0x8c63d4f4u,2);goto L8c63d482; /* mov.w 0x8c63d4f4,r0 */
L8c63d482: *stop=0x8c63d482u;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c63d484; /* mov.l @r14,r5 */
L8c63d484: *stop=0x8c63d484u;if(!valid_ram(r[0]+r[15],4u))return -3;r[6]=rd(m,r[15]+r[0],4);goto L8c63d486; /* mov.l @(r0,r15),r6 */
L8c63d486: *stop=0x8c63d486u;r[5]+=r[13];goto L8c63d488; /* add r13,r5 */
L8c63d488: *stop=0x8c63d488u;r[6]+=r[13];goto L8c63d48a; /* add r13,r6 */
L8c63d48a: target=r[8];*pr=0x8c63d48eu;*stop=0x8c63d48cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d48e; /* jsr @r8 */
L8c63d48e: *stop=0x8c63d48eu;r[9]+=1u;goto L8c63d490; /* add #0x1,r9 */
L8c63d490: *stop=0x8c63d490u;*sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[12]);goto L8c63d492; /* cmp/ge r12,r9 */
L8c63d492: take=!(*sr&1u);*stop=0x8c63d494u;r[13]+=12u;if(take)goto L8c63d480;goto L8c63d496; /* bf/s 0x8c63d480 */
L8c63d496: *stop=0x8c63d496u;r[2]=rd(m,0x8c63d514u,4);goto L8c63d498; /* mov.l 0x8c63d514,r2 */
L8c63d498: *stop=0x8c63d498u;r[5]=r[12];goto L8c63d49a; /* mov r12,r5 */
L8c63d49a: *stop=0x8c63d49au;r[4]=sx(m,0x8c63d4f4u,2);goto L8c63d49c; /* mov.w 0x8c63d4f4,r4 */
L8c63d49c: target=r[2];*pr=0x8c63d4a0u;*stop=0x8c63d49eu;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d4a0; /* jsr @r2 */
L8c63d4a0: *stop=0x8c63d4a0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c63d4a2; /* tst r0,r0 */
L8c63d4a2: if(!(*sr&1u))goto L8c63d4a8;goto L8c63d4a4; /* bf 0x8c63d4a8 */
L8c63d4a4: *stop=0x8c63d4a6u;goto L8c63d644; /* bra 0x8c63d644 */
L8c63d4a8: *stop=0x8c63d4a8u;r[13]=rd(m,0x8c63d518u,4);goto L8c63d4aa; /* mov.l 0x8c63d518,r13 */
L8c63d4aa: *stop=0x8c63d4aau;r[4]=r[10];goto L8c63d4ac; /* mov r10,r4 */
L8c63d4ac: *stop=0x8c63d4acu;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c63d4ae; /* cmp/pl r12 */
L8c63d4ae: *stop=0x8c63d4aeu;r[7]=r[10];goto L8c63d4b0; /* mov r10,r7 */
L8c63d4b0: *stop=0x8c63d4b0u;r[6]=r[10];goto L8c63d4b2; /* mov r10,r6 */
L8c63d4b2: *stop=0x8c63d4b2u;r[5]=r[10];goto L8c63d4b4; /* mov r10,r5 */
L8c63d4b4: take=!(*sr&1u);*stop=0x8c63d4b6u;r[13]&=r[11];if(take)goto L8c63d536;goto L8c63d4b8; /* bf/s 0x8c63d536 */
L8c63d4b8: *stop=0x8c63d4b8u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c63d4ba; /* tst r13,r13 */
L8c63d4ba: if((*sr&1u))goto L8c63d51c;goto L8c63d4bc; /* bt 0x8c63d51c */
L8c63d4bc: *stop=0x8c63d4bcu;r[0]=sx(m,0x8c63d4fcu,2);goto L8c63d4be; /* mov.w 0x8c63d4fc,r0 */
L8c63d4be: *stop=0x8c63d4beu;if(!valid_ram(r[14]+8u,4u))return -3;r[3]=rd(m,r[14]+0x8u,4);goto L8c63d4c0; /* mov.l @(0x8,r14),r3 */
L8c63d4c0: *stop=0x8c63d4c0u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto L8c63d4c2; /* mov.l @(r0,r15),r2 */
L8c63d4c2: *stop=0x8c63d4c2u;r[0]=sx(m,0x8c63d4fcu,2);goto L8c63d4c4; /* mov.w 0x8c63d4fc,r0 */
L8c63d4c4: *stop=0x8c63d4c4u;r[3]+=r[4];goto L8c63d4c6; /* add r4,r3 */
L8c63d4c6: *stop=0x8c63d4c6u;if(!valid_ram(r[3],2u))return -3;r[1]=sx(m,r[3],2);goto L8c63d4c8; /* mov.w @r3,r1 */
L8c63d4c8: *stop=0x8c63d4c8u;r[2]+=r[4];goto L8c63d4ca; /* add r4,r2 */
L8c63d4ca: *stop=0x8c63d4cau;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[1],2);goto L8c63d4cc; /* mov.w r1,@r2 */
L8c63d4cc: *stop=0x8c63d4ccu;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto L8c63d4ce; /* mov.l @(r0,r15),r3 */
L8c63d4ce: *stop=0x8c63d4ceu;if(!valid_ram(r[14]+8u,4u))return -3;r[0]=rd(m,r[14]+0x8u,4);goto L8c63d4d0; /* mov.l @(0x8,r14),r0 */
L8c63d4d0: *stop=0x8c63d4d0u;r[3]+=r[4];goto L8c63d4d2; /* add r4,r3 */
L8c63d4d2: *stop=0x8c63d4d2u;r[0]+=r[4];goto L8c63d4d4; /* add r4,r0 */
L8c63d4d4: *stop=0x8c63d4d4u;if(!valid_ram(r[0]+2u,2u))return -3;r[0]=sx(m,r[0]+0x2u,2);goto L8c63d4d6; /* mov.w @(0x2,r0),r0 */
L8c63d4d6: *stop=0x8c63d4d6u;if(!valid_ram(r[3]+2u,2u))return -3;wr(m,r[3]+0x2u,r[0],2);goto L8c63d4d8; /* mov.w r0,@(0x2,r3) */
L8c63d4d8: *stop=0x8c63d4d8u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c63d4da; /* mov.l @(0x4,r14),r3 */
L8c63d4da: *stop=0x8c63d4dau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c63d4dc; /* tst r3,r3 */
L8c63d4dc: if((*sr&1u))goto L8c63d52a;goto L8c63d4de; /* bt 0x8c63d52a */
L8c63d4de: *stop=0x8c63d4deu;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c63d4e0; /* mov.l @(0x4,r14),r2 */
L8c63d4e0: *stop=0x8c63d4e0u;r[0]=sx(m,0x8c63d4f8u,2);goto L8c63d4e2; /* mov.w 0x8c63d4f8,r0 */
L8c63d4e2: *stop=0x8c63d4e2u;r[2]+=r[5];goto L8c63d4e4; /* add r5,r2 */
L8c63d4e4: *stop=0x8c63d4e4u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto L8c63d4e6; /* mov.l @(r0,r15),r3 */
L8c63d4e6: *stop=0x8c63d4e6u;if(!valid_ram(r[2],4u))return -3;r[1]=rd(m,r[2],4);goto L8c63d4e8; /* mov.l @r2,r1 */
L8c63d4e8: *stop=0x8c63d4e8u;r[3]+=r[5];goto L8c63d4ea; /* add r5,r3 */
L8c63d4ea: *stop=0x8c63d4eau;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[1],4);goto L8c63d4ec; /* mov.l r1,@r3 */
L8c63d4ec: *stop=0x8c63d4eeu;goto L8c63d52a; /* bra 0x8c63d52a */
L8c63d51c: *stop=0x8c63d51cu;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c63d51e; /* mov.l @(0x4,r14),r3 */
L8c63d51e: *stop=0x8c63d51eu;r[0]=sx(m,0x8c63d5ecu,2);goto L8c63d520; /* mov.w 0x8c63d5ec,r0 */
L8c63d520: *stop=0x8c63d520u;r[3]+=r[6];goto L8c63d522; /* add r6,r3 */
L8c63d522: *stop=0x8c63d522u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto L8c63d524; /* mov.l @(r0,r15),r2 */
L8c63d524: *stop=0x8c63d524u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c63d526; /* mov.l @r3,r1 */
L8c63d526: *stop=0x8c63d526u;r[2]+=r[6];goto L8c63d528; /* add r6,r2 */
L8c63d528: *stop=0x8c63d528u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto L8c63d52a; /* mov.l r1,@r2 */
L8c63d52a: *stop=0x8c63d52au;r[7]+=1u;goto L8c63d52c; /* add #0x1,r7 */
L8c63d52c: *stop=0x8c63d52cu;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[12]);goto L8c63d52e; /* cmp/ge r12,r7 */
L8c63d52e: *stop=0x8c63d52eu;r[5]+=4u;goto L8c63d530; /* add #0x4,r5 */
L8c63d530: *stop=0x8c63d530u;r[6]+=4u;goto L8c63d532; /* add #0x4,r6 */
L8c63d532: take=!(*sr&1u);*stop=0x8c63d534u;r[4]+=4u;if(take)goto L8c63d4b8;goto L8c63d536; /* bf/s 0x8c63d4b8 */
L8c63d536: *stop=0x8c63d536u;r[6]=sx(m,0x8c63d5eeu,2);goto L8c63d538; /* mov.w 0x8c63d5ee,r6 */
L8c63d538: *stop=0x8c63d538u;r[5]=r[12];goto L8c63d53a; /* mov r12,r5 */
L8c63d53a: *stop=0x8c63d53au;r[4]=sx(m,0x8c63d5f0u,2);goto L8c63d53c; /* mov.w 0x8c63d5f0,r4 */
L8c63d53c: *stop=0x8c63d53cu;r[7]=r[11];goto L8c63d53e; /* mov r11,r7 */
L8c63d53e: *stop=0x8c63d53eu;r[2]=rd(m,0x8c63d5f4u,4);goto L8c63d540; /* mov.l 0x8c63d5f4,r2 */
L8c63d540: *stop=0x8c63d540u;r[6]+=r[15];goto L8c63d542; /* add r15,r6 */
L8c63d542: target=r[2];*pr=0x8c63d546u;*stop=0x8c63d544u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d546; /* jsr @r2 */
L8c63d546: *stop=0x8c63d546u;r[3]=3u;goto L8c63d548; /* mov #0x3,r3 */
L8c63d548: *stop=0x8c63d548u;r[14]=r[0];goto L8c63d54a; /* mov r0,r14 */
L8c63d54a: *stop=0x8c63d54au;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[3]);goto L8c63d54c; /* cmp/ge r3,r14 */
L8c63d54c: if(!(*sr&1u))goto L8c63d644;goto L8c63d54e; /* bf 0x8c63d644 */
L8c63d54e: *stop=0x8c63d54eu;r[1]=rd(m,0x8c63d5f8u,4);goto L8c63d550; /* mov.l 0x8c63d5f8,r1 */
L8c63d550: target=r[1];*pr=0x8c63d554u;*stop=0x8c63d552u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d554; /* jsr @r1 */
L8c63d554: *stop=0x8c63d554u;r[12]=rd(m,0x8c63d5fcu,4);goto L8c63d556; /* mov.l 0x8c63d5fc,r12 */
L8c63d556: *stop=0x8c63d556u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c63d558; /* tst r13,r13 */
L8c63d558: if((*sr&1u))goto L8c63d584;goto L8c63d55a; /* bt 0x8c63d584 */
L8c63d55a: *stop=0x8c63d55au;r[2]=64u;goto L8c63d55c; /* mov #0x40,r2 */
L8c63d55c: *stop=0x8c63d55cu;*sr=(*sr&~1u)|!(r[11]&r[2]);goto L8c63d55e; /* tst r11,r2 */
L8c63d55e: if((*sr&1u))goto L8c63d570;goto L8c63d560; /* bt 0x8c63d570 */
L8c63d560: *stop=0x8c63d560u;r[4]=rd(m,0x8c63d600u,4);goto L8c63d562; /* mov.l 0x8c63d600,r4 */
L8c63d562: target=r[12];*pr=0x8c63d566u;*stop=0x8c63d564u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d566; /* jsr @r12 */
L8c63d566: *stop=0x8c63d566u;r[2]=rd(m,0x8c63d604u,4);goto L8c63d568; /* mov.l 0x8c63d604,r2 */
L8c63d568: target=r[2];*pr=0x8c63d56cu;*stop=0x8c63d56au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d56c; /* jsr @r2 */
L8c63d56c: *stop=0x8c63d56eu;goto L8c63d580; /* bra 0x8c63d580 */
L8c63d570: *stop=0x8c63d570u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c63d572; /* tst r13,r13 */
L8c63d572: if((*sr&1u))goto L8c63d584;goto L8c63d574; /* bt 0x8c63d584 */
L8c63d574: *stop=0x8c63d574u;r[4]=rd(m,0x8c63d608u,4);goto L8c63d576; /* mov.l 0x8c63d608,r4 */
L8c63d576: target=r[12];*pr=0x8c63d57au;*stop=0x8c63d578u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d57a; /* jsr @r12 */
L8c63d57a: *stop=0x8c63d57au;r[3]=rd(m,0x8c63d60cu,4);goto L8c63d57c; /* mov.l 0x8c63d60c,r3 */
L8c63d57c: target=r[3];*pr=0x8c63d580u;*stop=0x8c63d57eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d580; /* jsr @r3 */
L8c63d580: *stop=0x8c63d582u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[0],4);goto L8c63d5a8; /* bra 0x8c63d5a8 */
L8c63d584: *stop=0x8c63d584u;r[2]=64u;goto L8c63d586; /* mov #0x40,r2 */
L8c63d586: *stop=0x8c63d586u;*sr=(*sr&~1u)|!(r[11]&r[2]);goto L8c63d588; /* tst r11,r2 */
L8c63d588: if((*sr&1u))goto L8c63d59a;goto L8c63d58a; /* bt 0x8c63d59a */
L8c63d58a: *stop=0x8c63d58au;r[4]=rd(m,0x8c63d610u,4);goto L8c63d58c; /* mov.l 0x8c63d610,r4 */
L8c63d58c: target=r[12];*pr=0x8c63d590u;*stop=0x8c63d58eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d590; /* jsr @r12 */
L8c63d590: *stop=0x8c63d590u;r[2]=rd(m,0x8c63d614u,4);goto L8c63d592; /* mov.l 0x8c63d614,r2 */
L8c63d592: target=r[2];*pr=0x8c63d596u;*stop=0x8c63d594u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d596; /* jsr @r2 */
L8c63d596: *stop=0x8c63d598u;goto L8c63d5a6; /* bra 0x8c63d5a6 */
L8c63d59a: *stop=0x8c63d59au;r[4]=rd(m,0x8c63d618u,4);goto L8c63d59c; /* mov.l 0x8c63d618,r4 */
L8c63d59c: target=r[12];*pr=0x8c63d5a0u;*stop=0x8c63d59eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d5a0; /* jsr @r12 */
L8c63d5a0: *stop=0x8c63d5a0u;r[2]=rd(m,0x8c63d61cu,4);goto L8c63d5a2; /* mov.l 0x8c63d61c,r2 */
L8c63d5a2: target=r[2];*pr=0x8c63d5a6u;*stop=0x8c63d5a4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d5a6; /* jsr @r2 */
L8c63d5a6: *stop=0x8c63d5a6u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[0],4);goto L8c63d5a8; /* mov.l r0,@r15 */
L8c63d5a8: *stop=0x8c63d5a8u;r[4]=32u;goto L8c63d5aa; /* mov #0x20,r4 */
L8c63d5aa: *stop=0x8c63d5aau;r[4]&=r[11];goto L8c63d5ac; /* and r11,r4 */
L8c63d5ac: *stop=0x8c63d5acu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c63d5ae; /* tst r4,r4 */
L8c63d5ae: if((*sr&1u))goto L8c63d5d8;goto L8c63d5b0; /* bt 0x8c63d5d8 */
L8c63d5b0: *stop=0x8c63d5b0u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c63d5b2; /* tst r13,r13 */
L8c63d5b2: if((*sr&1u))goto L8c63d5c4;goto L8c63d5b4; /* bt 0x8c63d5c4 */
L8c63d5b4: *stop=0x8c63d5b4u;r[5]=sx(m,0x8c63d5eeu,2);goto L8c63d5b6; /* mov.w 0x8c63d5ee,r5 */
L8c63d5b6: *stop=0x8c63d5b6u;r[6]=r[15];goto L8c63d5b8; /* mov r15,r6 */
L8c63d5b8: *stop=0x8c63d5b8u;r[3]=rd(m,0x8c63d620u,4);goto L8c63d5ba; /* mov.l 0x8c63d620,r3 */
L8c63d5ba: *stop=0x8c63d5bau;r[5]+=r[15];goto L8c63d5bc; /* add r15,r5 */
L8c63d5bc: target=r[3];*pr=0x8c63d5c0u;*stop=0x8c63d5beu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d5c0; /* jsr @r3 */
L8c63d5c0: *stop=0x8c63d5c2u;goto L8c63d638; /* bra 0x8c63d638 */
L8c63d5c4: *stop=0x8c63d5c4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c63d5c6; /* tst r4,r4 */
L8c63d5c6: if((*sr&1u))goto L8c63d5d8;goto L8c63d5c8; /* bt 0x8c63d5d8 */
L8c63d5c8: *stop=0x8c63d5c8u;r[5]=sx(m,0x8c63d5eeu,2);goto L8c63d5ca; /* mov.w 0x8c63d5ee,r5 */
L8c63d5ca: *stop=0x8c63d5cau;r[6]=r[15];goto L8c63d5cc; /* mov r15,r6 */
L8c63d5cc: *stop=0x8c63d5ccu;r[2]=rd(m,0x8c63d624u,4);goto L8c63d5ce; /* mov.l 0x8c63d624,r2 */
L8c63d5ce: *stop=0x8c63d5ceu;r[5]+=r[15];goto L8c63d5d0; /* add r15,r5 */
L8c63d5d0: target=r[2];*pr=0x8c63d5d4u;*stop=0x8c63d5d2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d5d4; /* jsr @r2 */
L8c63d5d4: *stop=0x8c63d5d6u;goto L8c63d638; /* bra 0x8c63d638 */
L8c63d5d8: *stop=0x8c63d5d8u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c63d5da; /* tst r13,r13 */
L8c63d5da: if((*sr&1u))goto L8c63d62c;goto L8c63d5dc; /* bt 0x8c63d62c */
L8c63d5dc: *stop=0x8c63d5dcu;r[5]=sx(m,0x8c63d5eeu,2);goto L8c63d5de; /* mov.w 0x8c63d5ee,r5 */
L8c63d5de: *stop=0x8c63d5deu;r[6]=r[15];goto L8c63d5e0; /* mov r15,r6 */
L8c63d5e0: *stop=0x8c63d5e0u;r[3]=rd(m,0x8c63d628u,4);goto L8c63d5e2; /* mov.l 0x8c63d628,r3 */
L8c63d5e2: *stop=0x8c63d5e2u;r[5]+=r[15];goto L8c63d5e4; /* add r15,r5 */
L8c63d5e4: target=r[3];*pr=0x8c63d5e8u;*stop=0x8c63d5e6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d5e8; /* jsr @r3 */
L8c63d5e8: *stop=0x8c63d5eau;goto L8c63d638; /* bra 0x8c63d638 */
L8c63d62c: *stop=0x8c63d62cu;r[5]=sx(m,0x8c63d65au,2);goto L8c63d62e; /* mov.w 0x8c63d65a,r5 */
L8c63d62e: *stop=0x8c63d62eu;r[6]=r[15];goto L8c63d630; /* mov r15,r6 */
L8c63d630: *stop=0x8c63d630u;r[3]=rd(m,0x8c63d660u,4);goto L8c63d632; /* mov.l 0x8c63d660,r3 */
L8c63d632: *stop=0x8c63d632u;r[5]+=r[15];goto L8c63d634; /* add r15,r5 */
L8c63d634: target=r[3];*pr=0x8c63d638u;*stop=0x8c63d636u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d638; /* jsr @r3 */
L8c63d638: *stop=0x8c63d638u;r[3]=rd(m,0x8c63d664u,4);goto L8c63d63a; /* mov.l 0x8c63d664,r3 */
L8c63d63a: target=r[3];*pr=0x8c63d63eu;*stop=0x8c63d63cu;if(!valid_ram(r[15],4u))return -3;r[4]=rd(m,r[15],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d63e; /* jsr @r3 */
L8c63d63e: *stop=0x8c63d63eu;r[2]=rd(m,0x8c63d668u,4);goto L8c63d640; /* mov.l 0x8c63d668,r2 */
L8c63d640: target=r[2];*pr=0x8c63d644u;*stop=0x8c63d642u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d644; /* jsr @r2 */
L8c63d644: *stop=0x8c63d644u;r[1]=sx(m,0x8c63d65cu,2);goto L8c63d646; /* mov.w 0x8c63d65c,r1 */
L8c63d646: *stop=0x8c63d646u;r[15]+=r[1];goto L8c63d648; /* add r1,r15 */
L8c63d648: *stop=0x8c63d648u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c63d64a; /* lds.l @r15+,PR */
L8c63d64a: *stop=0x8c63d64au;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c63d64c; /* mov.l @r15+,r8 */
L8c63d64c: *stop=0x8c63d64cu;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c63d64e; /* mov.l @r15+,r9 */
L8c63d64e: *stop=0x8c63d64eu;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c63d650; /* mov.l @r15+,r10 */
L8c63d650: *stop=0x8c63d650u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c63d652; /* mov.l @r15+,r11 */
L8c63d652: *stop=0x8c63d652u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c63d654; /* mov.l @r15+,r12 */
L8c63d654: *stop=0x8c63d654u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c63d656; /* mov.l @r15+,r13 */
L8c63d656: *stop=0x8c63d656u;if(*pr!=entry_pr)return -3;*stop=0x8c63d658u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_object_effect_phase_8c63a9b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63a9b8;
L8c63a9b8: *stop=0x8c63a9b8u;r[4]=r[4]&65535u;goto L8c63a9ba; /* extu.w r4,r4 */
L8c63a9ba: *stop=0x8c63a9bau;*fpul=r[4];goto L8c63a9bc; /* lds r4,FPUL */
L8c63a9bc: *stop=0x8c63a9bcu;if(*pr!=entry_pr)return -3;*stop=0x8c63a9beu;status=sa_sh4_vector_numeric(0xf0fdu,f,*fpul);if(status)return status;return 0; /* rts */
}
