/* Draw predicate and four-float state wrapper/copy; names inferred.
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
SA_STAGE_EXPORT int sa_draw_predicate_8c038d00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c038d00;
L8c038d00: *stop=0x8c038d00u;r[0]=rd(m,0x8c038d0cu,4);goto L8c038d02; /* mov.l 0x8c038d0c,r0 */
L8c038d02: *stop=0x8c038d02u;if(!valid_ram(r[0],4u))return -3;r[0]=rd(m,r[0],4);goto L8c038d04; /* mov.l @r0,r0 */
L8c038d04: *stop=0x8c038d04u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c038d06; /* tst r0,r0 */
L8c038d06: if((*sr&1u))goto L8c038d10;goto L8c038d08; /* bt 0x8c038d10 */
L8c038d08: *stop=0x8c038d08u;if(*pr!=entry_pr)return -3;*stop=0x8c038d0au;r[0]=0u;return 0; /* rts */
L8c038d10: *stop=0x8c038d10u;fcopy(f,0u,7u);goto L8c038d12; /* fmov fr7,fr0 */
L8c038d12: *stop=0x8c038d12u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c038d14; /* fldi1 fr7 */
L8c038d14: *stop=0x8c038d14u;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c038d16; /* ftrv xmtrx,fv4 */
L8c038d16: *stop=0x8c038d16u;r[1]=rd(m,0x8c038d84u,4);goto L8c038d18; /* mov.l 0x8c038d84,r1 */
L8c038d18: *stop=0x8c038d18u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[1]);goto L8c038d1a; /* fmov.s @r1,fr7 */
L8c038d1a: *stop=0x8c038d1au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[0],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c038d1c; /* fsub fr0,fr7 */
L8c038d1c: *stop=0x8c038d1cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][7],&f->fpscr,sr,1);if(status)return status;goto L8c038d1e; /* fcmp/gt fr7,fr6 */
L8c038d1e: if(!(*sr&1u))goto L8c038d9c;goto L8c038d20; /* bf 0x8c038d9c */
L8c038d20: *stop=0x8c038d20u;r[1]=rd(m,0x8c038d88u,4);goto L8c038d22; /* mov.l 0x8c038d88,r1 */
L8c038d22: *stop=0x8c038d22u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[1]);goto L8c038d24; /* fmov.s @r1,fr7 */
L8c038d24: *stop=0x8c038d24u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[0],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c038d26; /* fadd fr0,fr7 */
L8c038d26: *stop=0x8c038d26u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][7],&f->fpscr,sr,1);if(status)return status;goto L8c038d28; /* fcmp/gt fr7,fr6 */
L8c038d28: if((*sr&1u))goto L8c038d9c;goto L8c038d2a; /* bt 0x8c038d9c */
L8c038d2a: *stop=0x8c038d2au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c038d2c; /* fldi1 fr1 */
L8c038d2c: *stop=0x8c038d2cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[0],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c038d2e; /* fadd fr0,fr6 */
L8c038d2e: *stop=0x8c038d2eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c038d30; /* fdiv fr6,fr1 */
L8c038d30: *stop=0x8c038d30u;r[1]=rd(m,0x8c038d8cu,4);goto L8c038d32; /* mov.l 0x8c038d8c,r1 */
L8c038d32: *stop=0x8c038d32u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[1]);goto L8c038d34; /* fmov.s @r1,fr7 */
L8c038d34: *stop=0x8c038d34u;r[2]=rd(m,0x8c038d90u,4);goto L8c038d36; /* mov.l 0x8c038d90,r2 */
L8c038d36: *stop=0x8c038d36u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[2]);goto L8c038d38; /* fmov.s @r2,fr2 */
L8c038d38: *stop=0x8c038d38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c038d3a; /* fmul fr7,fr2 */
L8c038d3a: *stop=0x8c038d3au;r[3]=rd(m,0x8c038d94u,4);goto L8c038d3c; /* mov.l 0x8c038d94,r3 */
L8c038d3c: *stop=0x8c038d3cu;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[3]);r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038d3e; /* fmov.s @r3+,fr8 */
L8c038d3e: *stop=0x8c038d3eu;r[1]+=12u;goto L8c038d40; /* add #0xc,r1 */
L8c038d40: *stop=0x8c038d40u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038d42; /* fmov.s @r1+,fr7 */
L8c038d42: *stop=0x8c038d42u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c038d44; /* fmul fr1,fr2 */
L8c038d44: *stop=0x8c038d44u;fcopy(f,11u,4u);goto L8c038d46; /* fmov fr4,fr11 */
L8c038d46: *stop=0x8c038d46u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[0],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c038d48; /* fadd fr0,fr4 */
L8c038d48: *stop=0x8c038d48u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c038d4a; /* fmul fr2,fr4 */
L8c038d4a: *stop=0x8c038d4au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[7],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c038d4c; /* fadd fr7,fr4 */
L8c038d4c: *stop=0x8c038d4cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][8],&f->fpscr,sr,1);if(status)return status;goto L8c038d4e; /* fcmp/gt fr8,fr4 */
L8c038d4e: if(!(*sr&1u))goto L8c038d9c;goto L8c038d50; /* bf 0x8c038d9c */
L8c038d50: *stop=0x8c038d50u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[3]);r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038d52; /* fmov.s @r3+,fr9 */
L8c038d52: *stop=0x8c038d52u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[3]);r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038d54; /* fmov.s @r3+,fr8 */
L8c038d54: *stop=0x8c038d54u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[0],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c038d56; /* fsub fr0,fr11 */
L8c038d56: *stop=0x8c038d56u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[2],&f->fpscr,&fr[11]);if(status)return status;}goto L8c038d58; /* fmul fr2,fr11 */
L8c038d58: *stop=0x8c038d58u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[7],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c038d5a; /* fadd fr7,fr11 */
L8c038d5a: *stop=0x8c038d5au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][11],f->bank[(f->fpscr>>21)&1u][8],&f->fpscr,sr,1);if(status)return status;goto L8c038d5c; /* fcmp/gt fr8,fr11 */
L8c038d5c: if((*sr&1u))goto L8c038d9c;goto L8c038d5e; /* bt 0x8c038d9c */
L8c038d5e: *stop=0x8c038d5eu;r[2]=rd(m,0x8c038d98u,4);goto L8c038d60; /* mov.l 0x8c038d98,r2 */
L8c038d60: *stop=0x8c038d60u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[2]);goto L8c038d62; /* fmov.s @r2,fr3 */
L8c038d62: *stop=0x8c038d62u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c038d64; /* fmul fr7,fr3 */
L8c038d64: *stop=0x8c038d64u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038d66; /* fmov.s @r1+,fr7 */
L8c038d66: *stop=0x8c038d66u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[1],&f->fpscr,&fr[3]);if(status)return status;}goto L8c038d68; /* fmul fr1,fr3 */
L8c038d68: *stop=0x8c038d68u;fcopy(f,11u,5u);goto L8c038d6a; /* fmov fr5,fr11 */
L8c038d6a: *stop=0x8c038d6au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[0],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c038d6c; /* fadd fr0,fr5 */
L8c038d6c: *stop=0x8c038d6cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c038d6e; /* fmul fr3,fr5 */
L8c038d6e: *stop=0x8c038d6eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[7],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c038d70; /* fadd fr7,fr5 */
L8c038d70: *stop=0x8c038d70u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][9],&f->fpscr,sr,1);if(status)return status;goto L8c038d72; /* fcmp/gt fr9,fr5 */
L8c038d72: if(!(*sr&1u))goto L8c038d9c;goto L8c038d74; /* bf 0x8c038d9c */
L8c038d74: *stop=0x8c038d74u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[3]);r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038d76; /* fmov.s @r3+,fr9 */
L8c038d76: *stop=0x8c038d76u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[0],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c038d78; /* fsub fr0,fr11 */
L8c038d78: *stop=0x8c038d78u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[2],&f->fpscr,&fr[11]);if(status)return status;}goto L8c038d7a; /* fmul fr2,fr11 */
L8c038d7a: *stop=0x8c038d7au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[7],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c038d7c; /* fadd fr7,fr11 */
L8c038d7c: *stop=0x8c038d7cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][11],f->bank[(f->fpscr>>21)&1u][8],&f->fpscr,sr,1);if(status)return status;goto L8c038d7e; /* fcmp/gt fr8,fr11 */
L8c038d7e: if((*sr&1u))goto L8c038d9c;goto L8c038d80; /* bt 0x8c038d9c */
L8c038d80: *stop=0x8c038d80u;if(*pr!=entry_pr)return -3;*stop=0x8c038d82u;r[0]=1u;return 0; /* rts */
L8c038d9c: *stop=0x8c038d9cu;if(*pr!=entry_pr)return -3;*stop=0x8c038d9eu;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_draw_float_state_wrapper_8c043a20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c043a20;
L8c043a20: *stop=0x8c043a20u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c043a22; /* sts.l PR,@-r15 */
L8c043a22: *stop=0x8c043a22u;r[15]+=4294967280u;goto L8c043a24; /* add #-0x10,r15 */
L8c043a24: *stop=0x8c043a24u;r[0]=4u;goto L8c043a26; /* mov #0x4,r0 */
L8c043a26: *stop=0x8c043a26u;r[3]=rd(m,0x8c043be0u,4);goto L8c043a28; /* mov.l 0x8c043be0,r3 */
L8c043a28: *stop=0x8c043a28u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[15]);goto L8c043a2a; /* fmov.s fr4,@r15 */
L8c043a2a: *stop=0x8c043a2au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[15]);goto L8c043a2c; /* fmov fr5,@(r0,r15) */
L8c043a2c: *stop=0x8c043a2cu;r[0]=8u;goto L8c043a2e; /* mov #0x8,r0 */
L8c043a2e: *stop=0x8c043a2eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[15]);goto L8c043a30; /* fmov fr6,@(r0,r15) */
L8c043a30: *stop=0x8c043a30u;r[0]=12u;goto L8c043a32; /* mov #0xc,r0 */
L8c043a32: *stop=0x8c043a32u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,7u,r[0]+r[15]);goto L8c043a34; /* fmov fr7,@(r0,r15) */
L8c043a34: target=r[3];*pr=0x8c043a38u;*stop=0x8c043a36u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c043a38; /* jsr @r3 */
L8c043a38: *stop=0x8c043a38u;r[15]+=16u;goto L8c043a3a; /* add #0x10,r15 */
L8c043a3a: *stop=0x8c043a3au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c043a3c; /* lds.l @r15+,PR */
L8c043a3c: *stop=0x8c043a3cu;if(*pr!=entry_pr)return -3;*stop=0x8c043a3eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_draw_float_state_copy_8c60edf0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60edf0;
L8c60edf0: *stop=0x8c60edf0u;r[0]=4u;goto L8c60edf2; /* mov #0x4,r0 */
L8c60edf2: *stop=0x8c60edf2u;r[5]=rd(m,0x8c60ee40u,4);goto L8c60edf4; /* mov.l 0x8c60ee40,r5 */
L8c60edf4: *stop=0x8c60edf4u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto L8c60edf6; /* fmov.s @r4,fr3 */
L8c60edf6: *stop=0x8c60edf6u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[5]);goto L8c60edf8; /* fmov.s fr3,@r5 */
L8c60edf8: *stop=0x8c60edf8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c60edfa; /* fmov.s @(r0,r4),fr2 */
L8c60edfa: *stop=0x8c60edfau;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[5]);goto L8c60edfc; /* fmov fr2,@(r0,r5) */
L8c60edfc: *stop=0x8c60edfcu;r[0]=8u;goto L8c60edfe; /* mov #0x8,r0 */
L8c60edfe: *stop=0x8c60edfeu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c60ee00; /* fmov.s @(r0,r4),fr3 */
L8c60ee00: *stop=0x8c60ee00u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto L8c60ee02; /* fmov fr3,@(r0,r5) */
L8c60ee02: *stop=0x8c60ee02u;r[0]=12u;goto L8c60ee04; /* mov #0xc,r0 */
L8c60ee04: *stop=0x8c60ee04u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c60ee06; /* fmov.s @(r0,r4),fr3 */
L8c60ee06: *stop=0x8c60ee06u;if(*pr!=entry_pr)return -3;*stop=0x8c60ee08u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);return 0; /* rts */
}
