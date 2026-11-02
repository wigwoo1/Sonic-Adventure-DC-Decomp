/* Original USA world/land producer entries; names are source-informed.
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
SA_STAGE_EXPORT int sa_world_land_step_8c0519c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0519c0;
L8c0519c0: *stop=0x8c0519c0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0519c2; /* mov.l r14,@-r15 */
L8c0519c2: *stop=0x8c0519c2u;r[14]=r[4];goto L8c0519c4; /* mov r4,r14 */
L8c0519c4: *stop=0x8c0519c4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0519c6; /* mov.l r13,@-r15 */
L8c0519c6: *stop=0x8c0519c6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c0519c8; /* sts.l PR,@-r15 */
L8c0519c8: *stop=0x8c0519c8u;if(!valid_ram(r[14]+32u,4u))return -3;r[13]=rd(m,r[14]+0x20u,4);goto L8c0519ca; /* mov.l @(0x20,r14),r13 */
L8c0519ca: *stop=0x8c0519cau;if(!valid_ram(r[13],1u))return -3;r[0]=sx(m,r[13],1);goto L8c0519cc; /* mov.b @r13,r0 */
L8c0519cc: *stop=0x8c0519ccu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c0519ce; /* cmp/eq #0x0,r0 */
L8c0519ce: if((*sr&1u))goto L8c0519dc;goto L8c0519d0; /* bt 0x8c0519dc */
L8c0519d0: *stop=0x8c0519d0u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c0519d2; /* cmp/eq #0x2,r0 */
L8c0519d2: if((*sr&1u))goto L8c0519f8;goto L8c0519d4; /* bt 0x8c0519f8 */
L8c0519d4: *stop=0x8c0519d4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0519d6; /* cmp/eq #0x1,r0 */
L8c0519d6: if((*sr&1u))goto L8c051a00;goto L8c0519d8; /* bt 0x8c051a00 */
L8c0519d8: *stop=0x8c0519dau;goto L8c051a0a; /* bra 0x8c051a0a */
L8c0519dc: target=0x8c051a16u;*pr=0x8c0519e0u;*stop=0x8c0519deu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0519e0; /* bsr 0x8c051a16 */
L8c0519e0: *stop=0x8c0519e0u;r[3]=rd(m,0x8c051b04u,4);goto L8c0519e2; /* mov.l 0x8c051b04,r3 */
L8c0519e2: *stop=0x8c0519e2u;r[2]=rd(m,0x8c051b00u,4);goto L8c0519e4; /* mov.l 0x8c051b00,r2 */
L8c0519e4: target=r[3];*pr=0x8c0519e8u;*stop=0x8c0519e6u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[13],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0519e8; /* jsr @r3 */
L8c0519e8: *stop=0x8c0519e8u;r[3]=rd(m,0x8c051b0cu,4);goto L8c0519ea; /* mov.l 0x8c051b0c,r3 */
L8c0519ea: *stop=0x8c0519eau;r[0]=0x8c051b10u;goto L8c0519ec; /* mova 0x8c051b10,r0 */
L8c0519ec: *stop=0x8c0519ecu;r[1]=rd(m,0x8c051b08u,4);goto L8c0519ee; /* mov.l 0x8c051b08,r1 */
L8c0519ee: *stop=0x8c0519eeu;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[1],4);goto L8c0519f0; /* mov.l r1,@(0x14,r14) */
L8c0519f0: *stop=0x8c0519f0u;if(!valid_ram(r[14]+24u,4u))return -3;wr(m,r[14]+0x18u,r[3],4);goto L8c0519f2; /* mov.l r3,@(0x18,r14) */
L8c0519f2: *stop=0x8c0519f2u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto L8c0519f4; /* fmov.s @r0,fr4 */
L8c0519f4: *stop=0x8c0519f4u;r[3]=rd(m,0x8c051b14u,4);goto L8c0519f6; /* mov.l 0x8c051b14,r3 */
L8c0519f6: *stop=0x8c0519f6u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[3]);goto L8c0519f8; /* fmov.s fr4,@r3 */
L8c0519f8: target=0x8c051cc0u;*pr=0x8c0519fcu;*stop=0x8c0519fau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0519fc; /* bsr 0x8c051cc0 */
L8c0519fc: *stop=0x8c0519fcu;r[1]=1u;goto L8c0519fe; /* mov #0x1,r1 */
L8c0519fe: *stop=0x8c0519feu;if(!valid_ram(r[13],1u))return -3;wr(m,r[13],r[1],1);goto L8c051a00; /* mov.b r1,@r13 */
L8c051a00: *stop=0x8c051a00u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c051a02; /* lds.l @r15+,PR */
L8c051a02: *stop=0x8c051a02u;r[4]=r[14];goto L8c051a04; /* mov r14,r4 */
L8c051a04: *stop=0x8c051a04u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c051a06; /* mov.l @r15+,r13 */
L8c051a06: *stop=0x8c051a08u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;goto L8c051d50; /* bra 0x8c051d50 */
L8c051a0a: *stop=0x8c051a0au;r[3]=0u;goto L8c051a0c; /* mov #0x0,r3 */
L8c051a0c: *stop=0x8c051a0cu;if(!valid_ram(r[13],1u))return -3;wr(m,r[13],r[3],1);goto L8c051a0e; /* mov.b r3,@r13 */
L8c051a0e: *stop=0x8c051a0eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c051a10; /* lds.l @r15+,PR */
L8c051a10: *stop=0x8c051a10u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c051a12; /* mov.l @r15+,r13 */
L8c051a12: *stop=0x8c051a12u;if(*pr!=entry_pr)return -3;*stop=0x8c051a14u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c051d50: *stop=0x8c051d50u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c051d52; /* mov.l r14,@-r15 */
L8c051d52: *stop=0x8c051d52u;r[14]=rd(m,0x8c051de8u,4);goto L8c051d54; /* mov.l 0x8c051de8,r14 */
L8c051d54: *stop=0x8c051d54u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c051d56; /* fmov.s fr15,@-r15 */
L8c051d56: *stop=0x8c051d56u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c051d58; /* sts.l PR,@-r15 */
L8c051d58: *stop=0x8c051d58u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[14]);goto L8c051d5a; /* fmov.s @r14,fr7 */
L8c051d5a: *stop=0x8c051d5au;r[3]=rd(m,0x8c051decu,4);goto L8c051d5c; /* mov.l 0x8c051dec,r3 */
L8c051d5c: *stop=0x8c051d5cu;fcopy(f,5u,7u);goto L8c051d5e; /* fmov fr7,fr5 */
L8c051d5e: *stop=0x8c051d5eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c051d60; /* fldi0 fr15 */
L8c051d60: *stop=0x8c051d60u;fcopy(f,6u,7u);goto L8c051d62; /* fmov fr7,fr6 */
L8c051d62: target=r[3];*pr=0x8c051d66u;*stop=0x8c051d64u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051d66; /* jsr @r3 */
L8c051d66: *stop=0x8c051d66u;r[2]=rd(m,0x8c051df0u,4);goto L8c051d68; /* mov.l 0x8c051df0,r2 */
L8c051d68: target=r[2];*pr=0x8c051d6cu;*stop=0x8c051d6au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051d6c; /* jsr @r2 */
L8c051d6c: *stop=0x8c051d6cu;r[3]=rd(m,0x8c051df4u,4);goto L8c051d6e; /* mov.l 0x8c051df4,r3 */
L8c051d6e: target=r[3];*pr=0x8c051d72u;*stop=0x8c051d70u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051d72; /* jsr @r3 */
L8c051d72: *stop=0x8c051d72u;r[2]=rd(m,0x8c051dd4u,4);goto L8c051d74; /* mov.l 0x8c051dd4,r2 */
L8c051d74: *stop=0x8c051d74u;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto L8c051d76; /* mov.l @r2,r0 */
L8c051d76: *stop=0x8c051d76u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c051d78; /* cmp/eq #0x1,r0 */
L8c051d78: if(!(*sr&1u))goto L8c051d9a;goto L8c051d7a; /* bf 0x8c051d9a */
L8c051d7a: *stop=0x8c051d7au;r[3]=rd(m,0x8c051df8u,4);goto L8c051d7c; /* mov.l 0x8c051df8,r3 */
L8c051d7c: *stop=0x8c051d7cu;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c051d7e; /* mov.l @r3,r1 */
L8c051d7e: *stop=0x8c051d7eu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c051d80; /* tst r1,r1 */
L8c051d80: if(!(*sr&1u))goto L8c051d96;goto L8c051d82; /* bf 0x8c051d96 */
L8c051d82: target=0x8c0520c8u;*pr=0x8c051d86u;*stop=0x8c051d84u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051d86; /* bsr 0x8c0520c8 */
L8c051d86: target=0x8c051e56u;*pr=0x8c051d8au;*stop=0x8c051d88u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051d8a; /* bsr 0x8c051e56 */
L8c051d8a: target=0x8c052048u;*pr=0x8c051d8eu;*stop=0x8c051d8cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051d8e; /* bsr 0x8c052048 */
L8c051d8e: target=0x8c051f64u;*pr=0x8c051d92u;*stop=0x8c051d90u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051d92; /* bsr 0x8c051f64 */
L8c051d92: *stop=0x8c051d94u;goto L8c051d9a; /* bra 0x8c051d9a */
L8c051d96: target=0x8c052048u;*pr=0x8c051d9au;*stop=0x8c051d98u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051d9a; /* bsr 0x8c052048 */
L8c051d9a: *stop=0x8c051d9au;r[3]=rd(m,0x8c051dfcu,4);goto L8c051d9c; /* mov.l 0x8c051dfc,r3 */
L8c051d9c: target=r[3];*pr=0x8c051da0u;*stop=0x8c051d9eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051da0; /* jsr @r3 */
L8c051da0: *stop=0x8c051da0u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c051da2; /* lds.l @r15+,PR */
L8c051da2: *stop=0x8c051da2u;fcopy(f,4u,15u);goto L8c051da4; /* fmov fr15,fr4 */
L8c051da4: *stop=0x8c051da4u;fcopy(f,5u,15u);goto L8c051da6; /* fmov fr15,fr5 */
L8c051da6: *stop=0x8c051da6u;fcopy(f,7u,15u);goto L8c051da8; /* fmov fr15,fr7 */
L8c051da8: *stop=0x8c051da8u;fcopy(f,6u,15u);goto L8c051daa; /* fmov fr15,fr6 */
L8c051daa: *stop=0x8c051daau;r[2]=rd(m,0x8c051decu,4);goto L8c051dac; /* mov.l 0x8c051dec,r2 */
L8c051dac: *stop=0x8c051dacu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c051dae; /* fmov.s @r15+,fr15 */
L8c051dae: target=r[2];*stop=0x8c051db0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_world_load_act_8c051a52(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c051a52;
L8c051a52: *stop=0x8c051a52u;r[0]=r[4];goto L8c051a54; /* mov r4,r0 */
L8c051a54: *stop=0x8c051a54u;r[0]<<=2;goto L8c051a56; /* shll2 r0 */
L8c051a56: *stop=0x8c051a56u;r[0]<<=2;goto L8c051a58; /* shll2 r0 */
L8c051a58: *stop=0x8c051a58u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c051a5a; /* mov.l r14,@-r15 */
L8c051a5a: *stop=0x8c051a5au;r[3]=rd(m,0x8c051b48u,4);goto L8c051a5c; /* mov.l 0x8c051b48,r3 */
L8c051a5c: *stop=0x8c051a5cu;r[14]=r[5];goto L8c051a5e; /* mov r5,r14 */
L8c051a5e: *stop=0x8c051a5eu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c051a60; /* shll r0 */
L8c051a60: *stop=0x8c051a60u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c051a62; /* mov.l r13,@-r15 */
L8c051a62: *stop=0x8c051a62u;r[3]+=r[0];goto L8c051a64; /* add r0,r3 */
L8c051a64: *stop=0x8c051a64u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c051a66; /* mov.l r12,@-r15 */
L8c051a66: *stop=0x8c051a66u;r[14]<<=2;goto L8c051a68; /* shll2 r14 */
L8c051a68: *stop=0x8c051a68u;r[14]+=r[3];goto L8c051a6a; /* add r3,r14 */
L8c051a6a: *stop=0x8c051a6au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c051a6c; /* sts.l PR,@-r15 */
L8c051a6c: *stop=0x8c051a6cu;if(!valid_ram(r[14],4u))return -3;r[14]=rd(m,r[14],4);goto L8c051a6e; /* mov.l @r14,r14 */
L8c051a6e: *stop=0x8c051a6eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c051a70; /* tst r14,r14 */
L8c051a70: if(!(*sr&1u))goto L8c051a76;goto L8c051a72; /* bf 0x8c051a76 */
L8c051a72: *stop=0x8c051a74u;goto L8c051ba4; /* bra 0x8c051ba4 */
L8c051a76: *stop=0x8c051a76u;if(!valid_ram(r[14]+4u,2u))return -3;r[0]=sx(m,r[14]+0x4u,2);goto L8c051a78; /* mov.w @(0x4,r14),r0 */
L8c051a78: *stop=0x8c051a78u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c051a7a; /* tst #0x10,r0 */
L8c051a7a: take=(*sr&1u);*stop=0x8c051a7cu;if(!valid_ram(r[14],2u))return -3;r[12]=sx(m,r[14],2);if(take)goto L8c051ad2;goto L8c051a7e; /* bt/s 0x8c051ad2 */
L8c051a7e: *stop=0x8c051a7eu;if(!valid_ram(r[14]+6u,2u))return -3;r[0]=sx(m,r[14]+0x6u,2);goto L8c051a80; /* mov.w @(0x6,r14),r0 */
L8c051a80: *stop=0x8c051a80u;*sr=(*sr&~1u)|!(r[0]&0xcu);goto L8c051a82; /* tst #0xc,r0 */
L8c051a82: if(!(*sr&1u))goto L8c051acc;goto L8c051a84; /* bf 0x8c051acc */
L8c051a84: *stop=0x8c051a84u;r[2]=rd(m,0x8c051b4cu,4);goto L8c051a86; /* mov.l 0x8c051b4c,r2 */
L8c051a86: target=r[2];*pr=0x8c051a8au;*stop=0x8c051a88u;if(!valid_ram(r[14]+28u,4u))return -3;r[4]=rd(m,r[14]+0x1cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051a8a; /* jsr @r2 */
L8c051a8a: *stop=0x8c051a8au;r[3]=rd(m,0x8c051b50u,4);goto L8c051a8c; /* mov.l 0x8c051b50,r3 */
L8c051a8c: *stop=0x8c051a8cu;r[13]=r[0];goto L8c051a8e; /* mov r0,r13 */
L8c051a8e: *stop=0x8c051a8eu;if(!valid_ram(r[14]+32u,4u))return -3;wr(m,r[14]+0x20u,r[0],4);goto L8c051a90; /* mov.l r0,@(0x20,r14) */
L8c051a90: target=r[3];*pr=0x8c051a94u;*stop=0x8c051a92u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051a94; /* jsr @r3 */
L8c051a94: *stop=0x8c051a94u;r[5]=r[12];goto L8c051a96; /* mov r12,r5 */
L8c051a96: *stop=0x8c051a96u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c051a98; /* cmp/pl r5 */
L8c051a98: take=!(*sr&1u);*stop=0x8c051a9au;if(!valid_ram(r[14]+12u,4u))return -3;r[4]=rd(m,r[14]+0xcu,4);if(take)goto L8c051aaa;goto L8c051a9c; /* bf/s 0x8c051aaa */
L8c051a9c: *stop=0x8c051a9cu;if(!valid_ram(r[4]+24u,4u))return -3;r[3]=rd(m,r[4]+0x18u,4);goto L8c051a9e; /* mov.l @(0x18,r4),r3 */
L8c051a9e: *stop=0x8c051a9eu;r[5]+=4294967295u;goto L8c051aa0; /* add #-0x1,r5 */
L8c051aa0: *stop=0x8c051aa0u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c051aa2; /* cmp/pl r5 */
L8c051aa2: *stop=0x8c051aa2u;r[3]+=r[13];goto L8c051aa4; /* add r13,r3 */
L8c051aa4: *stop=0x8c051aa4u;if(!valid_ram(r[4]+24u,4u))return -3;wr(m,r[4]+0x18u,r[3],4);goto L8c051aa6; /* mov.l r3,@(0x18,r4) */
L8c051aa6: take=(*sr&1u);*stop=0x8c051aa8u;r[4]+=36u;if(take)goto L8c051a9c;goto L8c051aaa; /* bt/s 0x8c051a9c */
L8c051aaa: *stop=0x8c051aaau;if(!valid_ram(r[14]+4u,2u))return -3;r[0]=sx(m,r[14]+0x4u,2);goto L8c051aac; /* mov.w @(0x4,r14),r0 */
L8c051aac: *stop=0x8c051aacu;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c051aae; /* tst #0x1,r0 */
L8c051aae: if((*sr&1u))goto L8c051acc;goto L8c051ab0; /* bt 0x8c051acc */
L8c051ab0: *stop=0x8c051ab0u;r[5]=r[12];goto L8c051ab2; /* mov r12,r5 */
L8c051ab2: *stop=0x8c051ab2u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c051ab4; /* cmp/pl r5 */
L8c051ab4: take=!(*sr&1u);*stop=0x8c051ab6u;if(!valid_ram(r[14]+16u,4u))return -3;r[4]=rd(m,r[14]+0x10u,4);if(take)goto L8c051acc;goto L8c051ab8; /* bf/s 0x8c051acc */
L8c051ab8: *stop=0x8c051ab8u;if(!valid_ram(r[4]+12u,4u))return -3;r[3]=rd(m,r[4]+0xcu,4);goto L8c051aba; /* mov.l @(0xc,r4),r3 */
L8c051aba: *stop=0x8c051abau;r[5]+=4294967295u;goto L8c051abc; /* add #-0x1,r5 */
L8c051abc: *stop=0x8c051abcu;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c051abe; /* cmp/pl r5 */
L8c051abe: *stop=0x8c051abeu;r[3]+=r[13];goto L8c051ac0; /* add r13,r3 */
L8c051ac0: *stop=0x8c051ac0u;if(!valid_ram(r[4]+12u,4u))return -3;wr(m,r[4]+0xcu,r[3],4);goto L8c051ac2; /* mov.l r3,@(0xc,r4) */
L8c051ac2: *stop=0x8c051ac2u;if(!valid_ram(r[4]+16u,4u))return -3;r[2]=rd(m,r[4]+0x10u,4);goto L8c051ac4; /* mov.l @(0x10,r4),r2 */
L8c051ac4: *stop=0x8c051ac4u;r[2]+=r[13];goto L8c051ac6; /* add r13,r2 */
L8c051ac6: *stop=0x8c051ac6u;if(!valid_ram(r[4]+16u,4u))return -3;wr(m,r[4]+0x10u,r[2],4);goto L8c051ac8; /* mov.l r2,@(0x10,r4) */
L8c051ac8: take=(*sr&1u);*stop=0x8c051acau;r[4]+=24u;if(take)goto L8c051ab8;goto L8c051acc; /* bt/s 0x8c051ab8 */
L8c051acc: *stop=0x8c051accu;if(!valid_ram(r[14]+6u,2u))return -3;r[0]=sx(m,r[14]+0x6u,2);goto L8c051ace; /* mov.w @(0x6,r14),r0 */
L8c051ace: *stop=0x8c051aceu;r[0]+=4u;goto L8c051ad0; /* add #0x4,r0 */
L8c051ad0: *stop=0x8c051ad0u;if(!valid_ram(r[14]+6u,2u))return -3;wr(m,r[14]+0x6u,r[0],2);goto L8c051ad2; /* mov.w r0,@(0x6,r14) */
L8c051ad2: *stop=0x8c051ad2u;if(!valid_ram(r[14]+4u,2u))return -3;r[0]=sx(m,r[14]+0x4u,2);goto L8c051ad4; /* mov.w @(0x4,r14),r0 */
L8c051ad4: *stop=0x8c051ad4u;r[12]=rd(m,0x8c051b54u,4);goto L8c051ad6; /* mov.l 0x8c051b54,r12 */
L8c051ad6: *stop=0x8c051ad6u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c051ad8; /* tst #0x2,r0 */
L8c051ad8: take=(*sr&1u);*stop=0x8c051adau;r[4]=3u;if(take)goto L8c051b60;goto L8c051adc; /* bt/s 0x8c051b60 */
L8c051adc: *stop=0x8c051adcu;if(!valid_ram(r[14]+6u,2u))return -3;r[0]=sx(m,r[14]+0x6u,2);goto L8c051ade; /* mov.w @(0x6,r14),r0 */
L8c051ade: *stop=0x8c051adeu;*sr=(*sr&~1u)|!(r[4]&r[0]);goto L8c051ae0; /* tst r4,r0 */
L8c051ae0: if(!(*sr&1u))goto L8c051ae6;goto L8c051ae2; /* bf 0x8c051ae6 */
L8c051ae2: target=r[12];*pr=0x8c051ae6u;*stop=0x8c051ae4u;if(!valid_ram(r[14]+24u,4u))return -3;r[4]=rd(m,r[14]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051ae6; /* jsr @r12 */
L8c051ae6: *stop=0x8c051ae6u;if(!valid_ram(r[14]+6u,2u))return -3;r[0]=sx(m,r[14]+0x6u,2);goto L8c051ae8; /* mov.w @(0x6,r14),r0 */
L8c051ae8: *stop=0x8c051ae8u;r[0]+=1u;goto L8c051aea; /* add #0x1,r0 */
L8c051aea: *stop=0x8c051aecu;if(!valid_ram(r[14]+6u,2u))return -3;wr(m,r[14]+0x6u,r[0],2);goto L8c051ba4; /* bra 0x8c051ba4 */
L8c051b60: *stop=0x8c051b60u;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c051b62; /* tst #0x8,r0 */
L8c051b62: if((*sr&1u))goto L8c051b80;goto L8c051b64; /* bt 0x8c051b80 */
L8c051b64: *stop=0x8c051b64u;if(!valid_ram(r[14]+20u,4u))return -3;r[2]=rd(m,r[14]+0x14u,4);goto L8c051b66; /* mov.l @(0x14,r14),r2 */
L8c051b66: *stop=0x8c051b66u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c051b68; /* tst r2,r2 */
L8c051b68: if((*sr&1u))goto L8c051ba4;goto L8c051b6a; /* bt 0x8c051ba4 */
L8c051b6a: *stop=0x8c051b6au;if(!valid_ram(r[14]+6u,2u))return -3;r[0]=sx(m,r[14]+0x6u,2);goto L8c051b6c; /* mov.w @(0x6,r14),r0 */
L8c051b6c: *stop=0x8c051b6cu;*sr=(*sr&~1u)|!(r[4]&r[0]);goto L8c051b6e; /* tst r4,r0 */
L8c051b6e: if(!(*sr&1u))goto L8c051b78;goto L8c051b70; /* bf 0x8c051b78 */
L8c051b70: *stop=0x8c051b70u;r[2]=rd(m,0x8c051ca0u,4);goto L8c051b72; /* mov.l 0x8c051ca0,r2 */
L8c051b72: *stop=0x8c051b72u;if(!valid_ram(r[14]+24u,4u))return -3;r[5]=rd(m,r[14]+0x18u,4);goto L8c051b74; /* mov.l @(0x18,r14),r5 */
L8c051b74: target=r[2];*pr=0x8c051b78u;*stop=0x8c051b76u;if(!valid_ram(r[14]+20u,4u))return -3;r[4]=rd(m,r[14]+0x14u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051b78; /* jsr @r2 */
L8c051b78: *stop=0x8c051b78u;if(!valid_ram(r[14]+6u,2u))return -3;r[0]=sx(m,r[14]+0x6u,2);goto L8c051b7a; /* mov.w @(0x6,r14),r0 */
L8c051b7a: *stop=0x8c051b7au;r[0]+=1u;goto L8c051b7c; /* add #0x1,r0 */
L8c051b7c: *stop=0x8c051b7eu;if(!valid_ram(r[14]+6u,2u))return -3;wr(m,r[14]+0x6u,r[0],2);goto L8c051ba4; /* bra 0x8c051ba4 */
L8c051b80: *stop=0x8c051b80u;if(!valid_ram(r[14]+4u,2u))return -3;r[0]=sx(m,r[14]+0x4u,2);goto L8c051b82; /* mov.w @(0x4,r14),r0 */
L8c051b82: *stop=0x8c051b82u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c051b84; /* tst #0x1,r0 */
L8c051b84: if((*sr&1u))goto L8c051ba4;goto L8c051b86; /* bt 0x8c051ba4 */
L8c051b86: *stop=0x8c051b86u;if(!valid_ram(r[14]+2u,2u))return -3;r[0]=sx(m,r[14]+0x2u,2);goto L8c051b88; /* mov.w @(0x2,r14),r0 */
L8c051b88: *stop=0x8c051b88u;if(!valid_ram(r[14]+16u,4u))return -3;r[13]=rd(m,r[14]+0x10u,4);goto L8c051b8a; /* mov.l @(0x10,r14),r13 */
L8c051b8a: *stop=0x8c051b8au;r[14]=r[0];goto L8c051b8c; /* mov r0,r14 */
L8c051b8c: *stop=0x8c051b8cu;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c051b8e; /* cmp/pl r14 */
L8c051b8e: take=!(*sr&1u);*stop=0x8c051b90u;r[4]=r[0];if(take)goto L8c051ba4;goto L8c051b92; /* bf/s 0x8c051ba4 */
L8c051b92: *stop=0x8c051b92u;if(!valid_ram(r[13]+20u,4u))return -3;r[2]=rd(m,r[13]+0x14u,4);goto L8c051b94; /* mov.l @(0x14,r13),r2 */
L8c051b94: *stop=0x8c051b94u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c051b96; /* tst r2,r2 */
L8c051b96: if((*sr&1u))goto L8c051b9c;goto L8c051b98; /* bt 0x8c051b9c */
L8c051b98: target=r[12];*pr=0x8c051b9cu;*stop=0x8c051b9au;if(!valid_ram(r[13]+20u,4u))return -3;r[4]=rd(m,r[13]+0x14u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051b9c; /* jsr @r12 */
L8c051b9c: *stop=0x8c051b9cu;r[14]+=4294967295u;goto L8c051b9e; /* add #-0x1,r14 */
L8c051b9e: *stop=0x8c051b9eu;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c051ba0; /* cmp/pl r14 */
L8c051ba0: take=(*sr&1u);*stop=0x8c051ba2u;r[13]+=24u;if(take)goto L8c051b92;goto L8c051ba4; /* bt/s 0x8c051b92 */
L8c051ba4: *stop=0x8c051ba4u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c051ba6; /* lds.l @r15+,PR */
L8c051ba6: *stop=0x8c051ba6u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c051ba8; /* mov.l @r15+,r12 */
L8c051ba8: *stop=0x8c051ba8u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c051baa; /* mov.l @r15+,r13 */
L8c051baa: *stop=0x8c051baau;if(*pr!=entry_pr)return -3;*stop=0x8c051bacu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_land_start_8c051cc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c051cc0;
L8c051cc0: *stop=0x8c051cc0u;r[3]=rd(m,0x8c051dc8u,4);goto L8c051cc2; /* mov.l 0x8c051dc8,r3 */
L8c051cc2: *stop=0x8c051cc2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c051cc4; /* mov.l r14,@-r15 */
L8c051cc4: *stop=0x8c051cc4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c051cc6; /* mov.l r13,@-r15 */
L8c051cc6: *stop=0x8c051cc6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c051cc8; /* sts.l PR,@-r15 */
L8c051cc8: *stop=0x8c051cc8u;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);goto L8c051cca; /* mov.l @r3,r4 */
L8c051cca: *stop=0x8c051ccau;r[13]=rd(m,0x8c051dc0u,4);goto L8c051ccc; /* mov.l 0x8c051dc0,r13 */
L8c051ccc: *stop=0x8c051cccu;r[14]=rd(m,0x8c051dc4u,4);goto L8c051cce; /* mov.l 0x8c051dc4,r14 */
L8c051cce: *stop=0x8c051cceu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c051cd0; /* tst r4,r4 */
L8c051cd0: if((*sr&1u))goto L8c051cee;goto L8c051cd2; /* bt 0x8c051cee */
L8c051cd2: *stop=0x8c051cd2u;if(!valid_ram(r[4],1u))return -3;r[1]=sx(m,r[4],1);goto L8c051cd4; /* mov.b @r4,r1 */
L8c051cd4: *stop=0x8c051cd4u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c051cd6; /* tst r1,r1 */
L8c051cd6: if(!(*sr&1u))goto L8c051cee;goto L8c051cd8; /* bf 0x8c051cee */
L8c051cd8: *stop=0x8c051cd8u;r[3]=rd(m,0x8c051dccu,4);goto L8c051cda; /* mov.l 0x8c051dcc,r3 */
L8c051cda: target=r[3];*pr=0x8c051cdeu;*stop=0x8c051cdcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051cde; /* jsr @r3 */
L8c051cde: *stop=0x8c051cdeu;r[3]=rd(m,0x8c051dd0u,4);goto L8c051ce0; /* mov.l 0x8c051dd0,r3 */
L8c051ce0: *stop=0x8c051ce0u;r[4]=r[0];goto L8c051ce2; /* mov r0,r4 */
L8c051ce2: *stop=0x8c051ce2u;r[2]=r[4]&65535u;goto L8c051ce4; /* extu.w r4,r2 */
L8c051ce4: *stop=0x8c051ce4u;r[1]=4294967288u;goto L8c051ce6; /* mov #-0x8,r1 */
L8c051ce6: *stop=0x8c051ce6u;r[2]&=r[3];goto L8c051ce8; /* and r3,r2 */
L8c051ce8: *stop=0x8c051ce8u;{uint32_t shift=r[1];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c051cea; /* shad r1,r2 */
L8c051cea: *stop=0x8c051ceau;if(!valid_ram(r[14],1u))return -3;wr(m,r[14],r[2],1);goto L8c051cec; /* mov.b r2,@r14 */
L8c051cec: *stop=0x8c051cecu;if(!valid_ram(r[13],1u))return -3;wr(m,r[13],r[4],1);goto L8c051cee; /* mov.b r4,@r13 */
L8c051cee: *stop=0x8c051ceeu;r[2]=rd(m,0x8c051de0u,4);goto L8c051cf0; /* mov.l 0x8c051de0,r2 */
L8c051cf0: *stop=0x8c051cf0u;r[4]=1u;goto L8c051cf2; /* mov #0x1,r4 */
L8c051cf2: *stop=0x8c051cf2u;r[5]=rd(m,0x8c051dd4u,4);goto L8c051cf4; /* mov.l 0x8c051dd4,r5 */
L8c051cf4: *stop=0x8c051cf4u;r[6]=rd(m,0x8c051dd8u,4);goto L8c051cf6; /* mov.l 0x8c051dd8,r6 */
L8c051cf6: *stop=0x8c051cf6u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[4],4);goto L8c051cf8; /* mov.l r4,@r5 */
L8c051cf8: *stop=0x8c051cf8u;if(!valid_ram(r[6],4u))return -3;wr(m,r[6],r[4],4);goto L8c051cfa; /* mov.l r4,@r6 */
L8c051cfa: *stop=0x8c051cfau;if(!valid_ram(r[14],1u))return -3;r[0]=sx(m,r[14],1);goto L8c051cfc; /* mov.b @r14,r0 */
L8c051cfc: *stop=0x8c051cfcu;if(!valid_ram(r[13],1u))return -3;r[3]=sx(m,r[13],1);goto L8c051cfe; /* mov.b @r13,r3 */
L8c051cfe: *stop=0x8c051cfeu;r[0]<<=2;goto L8c051d00; /* shll2 r0 */
L8c051d00: *stop=0x8c051d00u;r[4]=rd(m,0x8c051ddcu,4);goto L8c051d02; /* mov.l 0x8c051ddc,r4 */
L8c051d02: *stop=0x8c051d02u;r[0]<<=2;goto L8c051d04; /* shll2 r0 */
L8c051d04: *stop=0x8c051d04u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c051d06; /* shll r0 */
L8c051d06: *stop=0x8c051d06u;r[3]<<=2;goto L8c051d08; /* shll2 r3 */
L8c051d08: *stop=0x8c051d08u;r[2]+=r[0];goto L8c051d0a; /* add r0,r2 */
L8c051d0a: *stop=0x8c051d0au;r[3]+=r[2];goto L8c051d0c; /* add r2,r3 */
L8c051d0c: *stop=0x8c051d0cu;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c051d0e; /* mov.l @r3,r1 */
L8c051d0e: *stop=0x8c051d0eu;r[3]=r[1];goto L8c051d10; /* mov r1,r3 */
L8c051d10: *stop=0x8c051d10u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c051d12; /* tst r3,r3 */
L8c051d12: take=!(*sr&1u);*stop=0x8c051d14u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);if(take)goto L8c051d1e;goto L8c051d16; /* bf/s 0x8c051d1e */
L8c051d16: *stop=0x8c051d16u;r[4]=0u;goto L8c051d18; /* mov #0x0,r4 */
L8c051d18: *stop=0x8c051d18u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[4],4);goto L8c051d1a; /* mov.l r4,@r5 */
L8c051d1a: *stop=0x8c051d1cu;if(!valid_ram(r[6],4u))return -3;wr(m,r[6],r[4],4);goto L8c051d30; /* bra 0x8c051d30 */
L8c051d1e: *stop=0x8c051d1eu;r[3]=rd(m,0x8c051de4u,4);goto L8c051d20; /* mov.l 0x8c051de4,r3 */
L8c051d20: target=r[3];*pr=0x8c051d24u;*stop=0x8c051d22u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051d24; /* jsr @r3 */
L8c051d24: *stop=0x8c051d24u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c051d26; /* lds.l @r15+,PR */
L8c051d26: *stop=0x8c051d26u;if(!valid_ram(r[13],1u))return -3;r[5]=sx(m,r[13],1);goto L8c051d28; /* mov.b @r13,r5 */
L8c051d28: *stop=0x8c051d28u;if(!valid_ram(r[14],1u))return -3;r[4]=sx(m,r[14],1);goto L8c051d2a; /* mov.b @r14,r4 */
L8c051d2a: *stop=0x8c051d2au;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c051d2c; /* mov.l @r15+,r13 */
L8c051d2c: *stop=0x8c051d2eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0x8c051a52u,m,f,sr,r,pr); /* bra 0x8c051a52 */
L8c051d30: *stop=0x8c051d30u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c051d32; /* lds.l @r15+,PR */
L8c051d32: *stop=0x8c051d32u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c051d34; /* mov.l @r15+,r13 */
L8c051d34: *stop=0x8c051d34u;if(*pr!=entry_pr)return -3;*stop=0x8c051d36u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_register_object_8c0528ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0528ae;
L8c0528ae: *stop=0x8c0528aeu;r[7]=rd(m,0x8c0529e4u,4);goto L8c0528b0; /* mov.l 0x8c0529e4,r7 */
L8c0528b0: *stop=0x8c0528b0u;r[2]=1u;goto L8c0528b2; /* mov #0x1,r2 */
L8c0528b2: *stop=0x8c0528b2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0528b4; /* mov.l r14,@-r15 */
L8c0528b4: *stop=0x8c0528b4u;r[2]<<=8;goto L8c0528b6; /* shll8 r2 */
L8c0528b6: *stop=0x8c0528b6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0528b8; /* mov.l r13,@-r15 */
L8c0528b8: *stop=0x8c0528b8u;if(!valid_ram(r[7],2u))return -3;r[14]=sx(m,r[7],2);goto L8c0528ba; /* mov.w @r7,r14 */
L8c0528ba: *stop=0x8c0528bau;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[2]);goto L8c0528bc; /* cmp/ge r2,r14 */
L8c0528bc: if((*sr&1u))goto L8c05293a;goto L8c0528be; /* bt 0x8c05293a */
L8c0528be: *stop=0x8c0528beu;if(!valid_ram(r[6]+4u,4u))return -3;r[13]=rd(m,r[6]+0x4u,4);goto L8c0528c0; /* mov.l @(0x4,r6),r13 */
L8c0528c0: *stop=0x8c0528c0u;r[0]=24u;goto L8c0528c2; /* mov #0x18,r0 */
L8c0528c2: *stop=0x8c0528c2u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto L8c0528c4; /* fmov.s @(r0,r13),fr4 */
L8c0528c4: *stop=0x8c0528c4u;r[0]=28u;goto L8c0528c6; /* mov #0x1c,r0 */
L8c0528c6: *stop=0x8c0528c6u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[13]);goto L8c0528c8; /* fmov.s @(r0,r13),fr5 */
L8c0528c8: *stop=0x8c0528c8u;r[0]=32u;goto L8c0528ca; /* mov #0x20,r0 */
L8c0528ca: *stop=0x8c0528cau;fcopy(f,0u,4u);goto L8c0528cc; /* fmov fr4,fr0 */
L8c0528cc: *stop=0x8c0528ccu;fcopy(f,3u,5u);goto L8c0528ce; /* fmov fr5,fr3 */
L8c0528ce: *stop=0x8c0528ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0528d0; /* fmul fr5,fr3 */
L8c0528d0: *stop=0x8c0528d0u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[13]);goto L8c0528d2; /* fmov.s @(r0,r13),fr6 */
L8c0528d2: *stop=0x8c0528d2u;r[0]=0x8c0529e8u;goto L8c0528d4; /* mova 0x8c0529e8,r0 */
L8c0528d4: *stop=0x8c0528d4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c0528d6; /* fmov.s @r0,fr2 */
L8c0528d6: *stop=0x8c0528d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0528d8; /* fmac fr0,fr4,fr3 */
L8c0528d8: *stop=0x8c0528d8u;fcopy(f,0u,6u);goto L8c0528da; /* fmov fr6,fr0 */
L8c0528da: *stop=0x8c0528dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0528dc; /* fmac fr0,fr6,fr3 */
L8c0528dc: *stop=0x8c0528dcu;fcopy(f,4u,3u);goto L8c0528de; /* fmov fr3,fr4 */
L8c0528de: *stop=0x8c0528deu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c0528e0; /* fcmp/gt fr4,fr2 */
L8c0528e0: if((*sr&1u))goto L8c0528fa;goto L8c0528e2; /* bt 0x8c0528fa */
L8c0528e2: *stop=0x8c0528e2u;r[13]=r[6];goto L8c0528e4; /* mov r6,r13 */
L8c0528e4: *stop=0x8c0528e4u;r[13]+=20u;goto L8c0528e6; /* add #0x14,r13 */
L8c0528e6: *stop=0x8c0528e6u;if(!valid_ram(r[13],4u))return -3;r[1]=rd(m,r[13],4);goto L8c0528e8; /* mov.l @r13,r1 */
L8c0528e8: *stop=0x8c0528e8u;if(!valid_ram(r[13]+4u,4u))return -3;r[3]=rd(m,r[13]+0x4u,4);goto L8c0528ea; /* mov.l @(0x4,r13),r3 */
L8c0528ea: *stop=0x8c0528eau;if(!valid_ram(r[13]+8u,4u))return -3;r[0]=rd(m,r[13]+0x8u,4);goto L8c0528ec; /* mov.l @(0x8,r13),r0 */
L8c0528ec: *stop=0x8c0528ecu;r[1]|=r[3];goto L8c0528ee; /* or r3,r1 */
L8c0528ee: *stop=0x8c0528eeu;r[3]=rd(m,0x8c0529ecu,4);goto L8c0528f0; /* mov.l 0x8c0529ec,r3 */
L8c0528f0: *stop=0x8c0528f0u;r[1]|=r[0];goto L8c0528f2; /* or r0,r1 */
L8c0528f2: *stop=0x8c0528f2u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c0528f4; /* tst r3,r1 */
L8c0528f4: if((*sr&1u))goto L8c0528fa;goto L8c0528f6; /* bt 0x8c0528fa */
L8c0528f6: *stop=0x8c0528f6u;r[1]=rd(m,0x8c0529f0u,4);goto L8c0528f8; /* mov.l 0x8c0529f0,r1 */
L8c0528f8: *stop=0x8c0528f8u;r[4]|=r[1];goto L8c0528fa; /* or r1,r4 */
L8c0528fa: *stop=0x8c0528fau;r[14]=(uint32_t)(int32_t)(int16_t)r[14];goto L8c0528fc; /* exts.w r14,r14 */
L8c0528fc: *stop=0x8c0528fcu;r[13]=rd(m,0x8c0529f4u,4);goto L8c0528fe; /* mov.l 0x8c0529f4,r13 */
L8c0528fe: *stop=0x8c0528feu;r[3]=r[14];goto L8c052900; /* mov r14,r3 */
L8c052900: *stop=0x8c052900u;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c052902; /* shll r14 */
L8c052902: *stop=0x8c052902u;r[14]+=r[3];goto L8c052904; /* add r3,r14 */
L8c052904: *stop=0x8c052904u;r[14]<<=2;goto L8c052906; /* shll2 r14 */
L8c052906: *stop=0x8c052906u;r[14]+=r[13];goto L8c052908; /* add r13,r14 */
L8c052908: *stop=0x8c052908u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[4],4);goto L8c05290a; /* mov.l r4,@r14 */
L8c05290a: *stop=0x8c05290au;if(!valid_ram(r[7],2u))return -3;r[2]=sx(m,r[7],2);goto L8c05290c; /* mov.w @r7,r2 */
L8c05290c: *stop=0x8c05290cu;r[3]=r[2];goto L8c05290e; /* mov r2,r3 */
L8c05290e: *stop=0x8c05290eu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c052910; /* shll r2 */
L8c052910: *stop=0x8c052910u;r[2]+=r[3];goto L8c052912; /* add r3,r2 */
L8c052912: *stop=0x8c052912u;r[2]<<=2;goto L8c052914; /* shll2 r2 */
L8c052914: *stop=0x8c052914u;r[2]+=r[13];goto L8c052916; /* add r13,r2 */
L8c052916: *stop=0x8c052916u;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[6],4);goto L8c052918; /* mov.l r6,@(0x4,r2) */
L8c052918: *stop=0x8c052918u;if(!valid_ram(r[7],2u))return -3;r[2]=sx(m,r[7],2);goto L8c05291a; /* mov.w @r7,r2 */
L8c05291a: *stop=0x8c05291au;r[2]+=1u;goto L8c05291c; /* add #0x1,r2 */
L8c05291c: *stop=0x8c05291cu;if(!valid_ram(r[7],2u))return -3;wr(m,r[7],r[2],2);goto L8c05291e; /* mov.w r2,@r7 */
L8c05291e: *stop=0x8c05291eu;r[2]+=4294967295u;goto L8c052920; /* add #-0x1,r2 */
L8c052920: *stop=0x8c052920u;r[2]=(uint32_t)(int32_t)(int16_t)r[2];goto L8c052922; /* exts.w r2,r2 */
L8c052922: *stop=0x8c052922u;r[3]=r[2];goto L8c052924; /* mov r2,r3 */
L8c052924: *stop=0x8c052924u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c052926; /* shll r2 */
L8c052926: *stop=0x8c052926u;r[2]+=r[3];goto L8c052928; /* add r3,r2 */
L8c052928: *stop=0x8c052928u;r[2]<<=2;goto L8c05292a; /* shll2 r2 */
L8c05292a: *stop=0x8c05292au;r[13]+=r[2];goto L8c05292c; /* add r2,r13 */
L8c05292c: *stop=0x8c05292cu;r[2]=1u;goto L8c05292e; /* mov #0x1,r2 */
L8c05292e: *stop=0x8c05292eu;if(!valid_ram(r[13]+8u,4u))return -3;wr(m,r[13]+0x8u,r[5],4);goto L8c052930; /* mov.l r5,@(0x8,r13) */
L8c052930: *stop=0x8c052930u;if(!valid_ram(r[5]+32u,4u))return -3;r[3]=rd(m,r[5]+0x20u,4);goto L8c052932; /* mov.l @(0x20,r5),r3 */
L8c052932: *stop=0x8c052932u;r[2]<<=8;goto L8c052934; /* shll8 r2 */
L8c052934: *stop=0x8c052934u;if(!valid_ram(r[3]+4u,2u))return -3;r[0]=sx(m,r[3]+0x4u,2);goto L8c052936; /* mov.w @(0x4,r3),r0 */
L8c052936: *stop=0x8c052936u;r[0]|=r[2];goto L8c052938; /* or r2,r0 */
L8c052938: *stop=0x8c052938u;if(!valid_ram(r[3]+4u,2u))return -3;wr(m,r[3]+0x4u,r[0],2);goto L8c05293a; /* mov.w r0,@(0x4,r3) */
L8c05293a: *stop=0x8c05293au;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c05293c; /* mov.l @r15+,r13 */
L8c05293c: *stop=0x8c05293cu;if(*pr!=entry_pr)return -3;*stop=0x8c05293eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
