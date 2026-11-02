/* Original camera follow-up and shared vector services; names inferred.
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
SA_STAGE_EXPORT int sa_camera_draw_8c01bd9e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c01bd9e;
L8c01bd9e: *stop=0x8c01bd9eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01bda0; /* mov.l r14,@-r15 */
L8c01bda0: *stop=0x8c01bda0u;r[0]=4294967295u;goto L8c01bda2; /* mov #-0x1,r0 */
L8c01bda2: *stop=0x8c01bda2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c01bda4; /* mov.l r13,@-r15 */
L8c01bda4: *stop=0x8c01bda4u;r[0]<<=8;goto L8c01bda6; /* shll8 r0 */
L8c01bda6: *stop=0x8c01bda6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c01bda8; /* mov.l r12,@-r15 */
L8c01bda8: *stop=0x8c01bda8u;r[0]+=12u;goto L8c01bdaa; /* add #0xc,r0 */
L8c01bdaa: *stop=0x8c01bdaau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c01bdac; /* mov.l r11,@-r15 */
L8c01bdac: *stop=0x8c01bdacu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c01bdae; /* mov.l r10,@-r15 */
L8c01bdae: *stop=0x8c01bdaeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c01bdb0; /* mov.l r9,@-r15 */
L8c01bdb0: *stop=0x8c01bdb0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c01bdb2; /* mov.l r8,@-r15 */
L8c01bdb2: *stop=0x8c01bdb2u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c01bdb4; /* fmov.s fr15,@-r15 */
L8c01bdb4: *stop=0x8c01bdb4u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto L8c01bdb6; /* fmov.s fr14,@-r15 */
L8c01bdb6: *stop=0x8c01bdb6u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[15]);goto L8c01bdb8; /* fmov.s fr13,@-r15 */
L8c01bdb8: *stop=0x8c01bdb8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01bdba; /* sts.l PR,@-r15 */
L8c01bdba: *stop=0x8c01bdbau;r[15]+=r[0];goto L8c01bdbc; /* add r0,r15 */
L8c01bdbc: *stop=0x8c01bdbcu;r[1]=rd(m,0x8c01bef4u,4);goto L8c01bdbe; /* mov.l 0x8c01bef4,r1 */
L8c01bdbe: *stop=0x8c01bdbeu;r[2]=rd(m,0x8c01bef0u,4);goto L8c01bdc0; /* mov.l 0x8c01bef0,r2 */
L8c01bdc0: *stop=0x8c01bdc0u;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c01bdc2; /* mov.l @r1,r3 */
L8c01bdc2: *stop=0x8c01bdc2u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c01bdc4; /* tst r3,r3 */
L8c01bdc4: take=(*sr&1u);*stop=0x8c01bdc6u;if(!valid_ram(r[2],4u))return -3;r[14]=rd(m,r[2],4);if(take)goto L8c01bdcc;goto L8c01bdc8; /* bt/s 0x8c01bdcc */
L8c01bdc8: *stop=0x8c01bdcau;goto L8c01c270; /* bra 0x8c01c270 */
L8c01bdcc: *stop=0x8c01bdccu;r[1]=rd(m,0x8c01bef8u,4);goto L8c01bdce; /* mov.l 0x8c01bef8,r1 */
L8c01bdce: *stop=0x8c01bdceu;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c01bdd0; /* mov.l @r1,r3 */
L8c01bdd0: *stop=0x8c01bdd0u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c01bdd2; /* mov.l r3,@r15 */
L8c01bdd2: *stop=0x8c01bdd2u;r[3]=rd(m,0x8c01befcu,4);goto L8c01bdd4; /* mov.l 0x8c01befc,r3 */
L8c01bdd4: target=r[3];*pr=0x8c01bdd8u;*stop=0x8c01bdd6u;r[10]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01bdd8; /* jsr @r3 */
L8c01bdd8: *stop=0x8c01bdd8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c01bdda; /* tst r0,r0 */
L8c01bdda: if(!(*sr&1u))goto L8c01bde0;goto L8c01bddc; /* bf 0x8c01bde0 */
L8c01bddc: *stop=0x8c01bddcu;r[3]=rd(m,0x8c01bef8u,4);goto L8c01bdde; /* mov.l 0x8c01bef8,r3 */
L8c01bdde: *stop=0x8c01bddeu;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[10],4);goto L8c01bde0; /* mov.l r10,@r3 */
L8c01bde0: *stop=0x8c01bde0u;if(!valid_ram(r[15],4u))return -3;r[0]=rd(m,r[15],4);goto L8c01bde2; /* mov.l @r15,r0 */
L8c01bde2: *stop=0x8c01bde2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c01bde4; /* cmp/eq #0x1,r0 */
L8c01bde4: if(!(*sr&1u))goto L8c01bdea;goto L8c01bde6; /* bf 0x8c01bdea */
L8c01bde6: *stop=0x8c01bde8u;goto L8c01c270; /* bra 0x8c01c270 */
L8c01bdea: *stop=0x8c01bdeau;r[0]=4294967172u;goto L8c01bdec; /* mov #-0x7c,r0 */
L8c01bdec: *stop=0x8c01bdecu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c01bdee; /* fldi0 fr15 */
L8c01bdee: *stop=0x8c01bdeeu;r[0]=r[0]&255u;goto L8c01bdf0; /* extu.b r0,r0 */
L8c01bdf0: *stop=0x8c01bdf0u;r[4]=rd(m,0x8c01bf04u,4);goto L8c01bdf2; /* mov.l 0x8c01bf04,r4 */
L8c01bdf2: *stop=0x8c01bdf2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[15]);goto L8c01bdf4; /* fmov fr15,@(r0,r15) */
L8c01bdf4: *stop=0x8c01bdf4u;r[0]=4294967176u;goto L8c01bdf6; /* mov #-0x78,r0 */
L8c01bdf6: *stop=0x8c01bdf6u;r[0]=r[0]&255u;goto L8c01bdf8; /* extu.b r0,r0 */
L8c01bdf8: *stop=0x8c01bdf8u;r[3]=rd(m,0x8c01bf08u,4);goto L8c01bdfa; /* mov.l 0x8c01bf08,r3 */
L8c01bdfa: *stop=0x8c01bdfau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[15]);goto L8c01bdfc; /* fmov fr15,@(r0,r15) */
L8c01bdfc: *stop=0x8c01bdfcu;r[0]=0x8c01bf00u;goto L8c01bdfe; /* mova 0x8c01bf00,r0 */
L8c01bdfe: *stop=0x8c01bdfeu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c01be00; /* fmov.s @r0,fr3 */
L8c01be00: *stop=0x8c01be00u;r[0]=4294967180u;goto L8c01be02; /* mov #-0x74,r0 */
L8c01be02: *stop=0x8c01be02u;r[0]=r[0]&255u;goto L8c01be04; /* extu.b r0,r0 */
L8c01be04: target=r[3];*pr=0x8c01be08u;*stop=0x8c01be06u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01be08; /* jsr @r3 */
L8c01be08: *stop=0x8c01be08u;r[2]=rd(m,0x8c01bf0cu,4);goto L8c01be0a; /* mov.l 0x8c01bf0c,r2 */
L8c01be0a: *stop=0x8c01be0au;r[5]=r[14];goto L8c01be0c; /* mov r14,r5 */
L8c01be0c: *stop=0x8c01be0cu;r[5]+=32u;goto L8c01be0e; /* add #0x20,r5 */
L8c01be0e: target=r[2];*pr=0x8c01be12u;*stop=0x8c01be10u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01be12; /* jsr @r2 */
L8c01be12: *stop=0x8c01be12u;if(!valid_ram(r[14]+24u,4u))return -3;r[3]=rd(m,r[14]+0x18u,4);goto L8c01be14; /* mov.l @(0x18,r14),r3 */
L8c01be14: *stop=0x8c01be14u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c01be16; /* tst r3,r3 */
L8c01be16: take=(*sr&1u);*stop=0x8c01be18u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);if(take)goto L8c01be22;goto L8c01be1a; /* bt/s 0x8c01be22 */
L8c01be1a: *stop=0x8c01be1au;r[3]=rd(m,0x8c01bf10u,4);goto L8c01be1c; /* mov.l 0x8c01bf10,r3 */
L8c01be1c: *stop=0x8c01be1cu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c01be1e; /* mov.l @r15,r5 */
L8c01be1e: target=r[3];*pr=0x8c01be22u;*stop=0x8c01be20u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01be22; /* jsr @r3 */
L8c01be22: *stop=0x8c01be22u;if(!valid_ram(r[14]+20u,4u))return -3;r[3]=rd(m,r[14]+0x14u,4);goto L8c01be24; /* mov.l @(0x14,r14),r3 */
L8c01be24: *stop=0x8c01be24u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c01be26; /* tst r3,r3 */
L8c01be26: take=(*sr&1u);*stop=0x8c01be28u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);if(take)goto L8c01be32;goto L8c01be2a; /* bt/s 0x8c01be32 */
L8c01be2a: *stop=0x8c01be2au;r[3]=rd(m,0x8c01bf14u,4);goto L8c01be2c; /* mov.l 0x8c01bf14,r3 */
L8c01be2c: *stop=0x8c01be2cu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c01be2e; /* mov.l @r15,r5 */
L8c01be2e: target=r[3];*pr=0x8c01be32u;*stop=0x8c01be30u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01be32; /* jsr @r3 */
L8c01be32: *stop=0x8c01be32u;r[5]=4294967172u;goto L8c01be34; /* mov #-0x7c,r5 */
L8c01be34: *stop=0x8c01be34u;r[3]=rd(m,0x8c01bee4u,4);goto L8c01be36; /* mov.l 0x8c01bee4,r3 */
L8c01be36: *stop=0x8c01be36u;r[6]=4294967172u;goto L8c01be38; /* mov #-0x7c,r6 */
L8c01be38: *stop=0x8c01be38u;r[5]=r[5]&255u;goto L8c01be3a; /* extu.b r5,r5 */
L8c01be3a: *stop=0x8c01be3au;r[6]=r[6]&255u;goto L8c01be3c; /* extu.b r6,r6 */
L8c01be3c: *stop=0x8c01be3cu;r[6]+=r[15];goto L8c01be3e; /* add r15,r6 */
L8c01be3e: *stop=0x8c01be3eu;r[5]+=r[15];goto L8c01be40; /* add r15,r5 */
L8c01be40: target=r[3];*pr=0x8c01be44u;*stop=0x8c01be42u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01be44; /* jsr @r3 */
L8c01be44: *stop=0x8c01be44u;r[0]=4294967172u;goto L8c01be46; /* mov #-0x7c,r0 */
L8c01be46: *stop=0x8c01be46u;r[6]=r[15];goto L8c01be48; /* mov r15,r6 */
L8c01be48: *stop=0x8c01be48u;r[0]=r[0]&255u;goto L8c01be4a; /* extu.b r0,r0 */
L8c01be4a: *stop=0x8c01be4au;r[5]=r[15];goto L8c01be4c; /* mov r15,r5 */
L8c01be4c: *stop=0x8c01be4cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01be4e; /* fmov.s @(r0,r15),fr3 */
L8c01be4e: *stop=0x8c01be4eu;r[0]=100u;goto L8c01be50; /* mov #0x64,r0 */
L8c01be50: *stop=0x8c01be50u;r[3]=rd(m,0x8c01bf18u,4);goto L8c01be52; /* mov.l 0x8c01bf18,r3 */
L8c01be52: *stop=0x8c01be52u;r[5]+=112u;goto L8c01be54; /* add #0x70,r5 */
L8c01be54: *stop=0x8c01be54u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01be56; /* fmov fr3,@(r0,r15) */
L8c01be56: *stop=0x8c01be56u;r[0]=4294967176u;goto L8c01be58; /* mov #-0x78,r0 */
L8c01be58: *stop=0x8c01be58u;r[0]=r[0]&255u;goto L8c01be5a; /* extu.b r0,r0 */
L8c01be5a: *stop=0x8c01be5au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01be5c; /* fmov.s @(r0,r15),fr3 */
L8c01be5c: *stop=0x8c01be5cu;r[0]=104u;goto L8c01be5e; /* mov #0x68,r0 */
L8c01be5e: *stop=0x8c01be5eu;r[6]+=112u;goto L8c01be60; /* add #0x70,r6 */
L8c01be60: *stop=0x8c01be60u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01be62; /* fmov fr3,@(r0,r15) */
L8c01be62: *stop=0x8c01be62u;r[0]=4294967180u;goto L8c01be64; /* mov #-0x74,r0 */
L8c01be64: *stop=0x8c01be64u;r[0]=r[0]&255u;goto L8c01be66; /* extu.b r0,r0 */
L8c01be66: *stop=0x8c01be66u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01be68; /* fmov.s @(r0,r15),fr3 */
L8c01be68: *stop=0x8c01be68u;r[0]=108u;goto L8c01be6a; /* mov #0x6c,r0 */
L8c01be6a: *stop=0x8c01be6au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01be6c; /* fmov fr3,@(r0,r15) */
L8c01be6c: *stop=0x8c01be6cu;r[0]=112u;goto L8c01be6e; /* mov #0x70,r0 */
L8c01be6e: *stop=0x8c01be6eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[15]);goto L8c01be70; /* fmov fr15,@(r0,r15) */
L8c01be70: *stop=0x8c01be70u;r[0]=116u;goto L8c01be72; /* mov #0x74,r0 */
L8c01be72: *stop=0x8c01be72u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c01be74; /* fldi1 fr3 */
L8c01be74: *stop=0x8c01be74u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[15]);goto L8c01be76; /* fmov fr15,@(r0,r15) */
L8c01be76: *stop=0x8c01be76u;r[0]=120u;goto L8c01be78; /* mov #0x78,r0 */
L8c01be78: *stop=0x8c01be78u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c01be7a; /* fneg fr3 */
L8c01be7a: *stop=0x8c01be7au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01be7c; /* fmov fr3,@(r0,r15) */
L8c01be7c: target=r[3];*pr=0x8c01be80u;*stop=0x8c01be7eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01be80; /* jsr @r3 */
L8c01be80: *stop=0x8c01be80u;r[2]=rd(m,0x8c01bf1cu,4);goto L8c01be82; /* mov.l 0x8c01bf1c,r2 */
L8c01be82: target=r[2];*pr=0x8c01be86u;*stop=0x8c01be84u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01be86; /* jsr @r2 */
L8c01be86: *stop=0x8c01be86u;r[3]=4294967172u;goto L8c01be88; /* mov #-0x7c,r3 */
L8c01be88: *stop=0x8c01be88u;r[3]=r[3]&255u;goto L8c01be8a; /* extu.b r3,r3 */
L8c01be8a: *stop=0x8c01be8au;r[2]=4294967208u;goto L8c01be8c; /* mov #-0x58,r2 */
L8c01be8c: *stop=0x8c01be8cu;r[3]+=r[15];goto L8c01be8e; /* add r15,r3 */
L8c01be8e: *stop=0x8c01be8eu;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto L8c01be90; /* mov.l @(0x4,r3),r0 */
L8c01be90: *stop=0x8c01be90u;r[2]=r[2]&255u;goto L8c01be92; /* extu.b r2,r2 */
L8c01be92: *stop=0x8c01be92u;r[2]+=r[15];goto L8c01be94; /* add r15,r2 */
L8c01be94: *stop=0x8c01be94u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c01be96; /* mov.l @r3,r1 */
L8c01be96: *stop=0x8c01be96u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto L8c01be98; /* mov.l r1,@r2 */
L8c01be98: *stop=0x8c01be98u;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto L8c01be9a; /* mov.l r0,@(0x4,r2) */
L8c01be9a: *stop=0x8c01be9au;r[0]=0x8c01bf24u;goto L8c01be9c; /* mova 0x8c01bf24,r0 */
L8c01be9c: *stop=0x8c01be9cu;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto L8c01be9e; /* mov.l @(0x8,r3),r1 */
L8c01be9e: *stop=0x8c01be9eu;r[3]=rd(m,0x8c01bf20u,4);goto L8c01bea0; /* mov.l 0x8c01bf20,r3 */
L8c01bea0: *stop=0x8c01bea0u;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto L8c01bea2; /* mov.l r1,@(0x8,r2) */
L8c01bea2: *stop=0x8c01bea2u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c01bea4; /* mov.l r3,@r15 */
L8c01bea4: *stop=0x8c01bea4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]);goto L8c01bea6; /* fmov.s @r0,fr7 */
L8c01bea6: *stop=0x8c01bea6u;r[0]=4294967180u;goto L8c01bea8; /* mov #-0x74,r0 */
L8c01bea8: *stop=0x8c01bea8u;r[0]=r[0]&255u;goto L8c01beaa; /* extu.b r0,r0 */
L8c01beaa: *stop=0x8c01beaau;r[3]=rd(m,0x8c01bf28u,4);goto L8c01beac; /* mov.l 0x8c01bf28,r3 */
L8c01beac: *stop=0x8c01beacu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[15]);goto L8c01beae; /* fmov.s @(r0,r15),fr6 */
L8c01beae: *stop=0x8c01beaeu;r[0]=4294967176u;goto L8c01beb0; /* mov #-0x78,r0 */
L8c01beb0: *stop=0x8c01beb0u;r[0]=r[0]&255u;goto L8c01beb2; /* extu.b r0,r0 */
L8c01beb2: *stop=0x8c01beb2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[15]);goto L8c01beb4; /* fmov.s @(r0,r15),fr5 */
L8c01beb4: *stop=0x8c01beb4u;r[0]=4294967172u;goto L8c01beb6; /* mov #-0x7c,r0 */
L8c01beb6: *stop=0x8c01beb6u;r[0]=r[0]&255u;goto L8c01beb8; /* extu.b r0,r0 */
L8c01beb8: target=r[3];*pr=0x8c01bebcu;*stop=0x8c01bebau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[15]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01bebc; /* jsr @r3 */
L8c01bebc: *stop=0x8c01bebcu;r[0]=0x8c01bf2cu;goto L8c01bebe; /* mova 0x8c01bf2c,r0 */
L8c01bebe: *stop=0x8c01bebeu;r[8]=rd(m,0x8c01bf34u,4);goto L8c01bec0; /* mov.l 0x8c01bf34,r8 */
L8c01bec0: *stop=0x8c01bec0u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[0]);goto L8c01bec2; /* fmov.s @r0,fr13 */
L8c01bec2: *stop=0x8c01bec2u;r[0]=4294967200u;goto L8c01bec4; /* mov #-0x60,r0 */
L8c01bec4: *stop=0x8c01bec4u;r[0]=r[0]&255u;goto L8c01bec6; /* extu.b r0,r0 */
L8c01bec6: *stop=0x8c01bec6u;r[9]=r[10];goto L8c01bec8; /* mov r10,r9 */
L8c01bec8: *stop=0x8c01bec8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,13u,r[0]+r[15]);goto L8c01beca; /* fmov fr13,@(r0,r15) */
L8c01beca: *stop=0x8c01becau;r[0]=0x8c01bf30u;goto L8c01becc; /* mova 0x8c01bf30,r0 */
L8c01becc: *stop=0x8c01beccu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]);goto L8c01bece; /* fmov.s @r0,fr14 */
L8c01bece: *stop=0x8c01beceu;r[0]=4294967188u;goto L8c01bed0; /* mov #-0x6c,r0 */
L8c01bed0: *stop=0x8c01bed0u;r[0]=r[0]&255u;goto L8c01bed2; /* extu.b r0,r0 */
L8c01bed2: *stop=0x8c01bed2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,14u,r[0]+r[15]);goto L8c01bed4; /* fmov fr14,@(r0,r15) */
L8c01bed4: *stop=0x8c01bed4u;if(!valid_ram(r[15],4u))return -3;r[4]=rd(m,r[15],4);goto L8c01bed6; /* mov.l @r15,r4 */
L8c01bed6: *stop=0x8c01bed6u;r[11]=r[4];goto L8c01bed8; /* mov r4,r11 */
L8c01bed8: *stop=0x8c01bed8u;r[12]=r[4];goto L8c01beda; /* mov r4,r12 */
L8c01beda: *stop=0x8c01bedcu;r[13]=r[4];goto L8c01c02a; /* bra 0x8c01c02a */
L8c01bf40: *stop=0x8c01bf40u;if(!valid_ram(r[13]+8u,4u))return -3;r[2]=rd(m,r[13]+0x8u,4);goto L8c01bf42; /* mov.l @(0x8,r13),r2 */
L8c01bf42: *stop=0x8c01bf42u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c01bf44; /* tst r2,r2 */
L8c01bf44: take=(*sr&1u);*stop=0x8c01bf46u;if(!valid_ram(r[13]+4u,4u))return -3;r[14]=rd(m,r[13]+0x4u,4);if(take)goto L8c01bf9e;goto L8c01bf48; /* bt/s 0x8c01bf9e */
L8c01bf48: *stop=0x8c01bf48u;r[0]=4294967172u;goto L8c01bf4a; /* mov #-0x7c,r0 */
L8c01bf4a: *stop=0x8c01bf4au;r[3]=rd(m,0x8c01c120u,4);goto L8c01bf4c; /* mov.l 0x8c01c120,r3 */
L8c01bf4c: *stop=0x8c01bf4cu;r[0]=r[0]&255u;goto L8c01bf4e; /* extu.b r0,r0 */
L8c01bf4e: *stop=0x8c01bf4eu;r[4]=r[15];goto L8c01bf50; /* mov r15,r4 */
L8c01bf50: *stop=0x8c01bf50u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01bf52; /* fmov.s @(r0,r15),fr3 */
L8c01bf52: *stop=0x8c01bf52u;r[0]=8u;goto L8c01bf54; /* mov #0x8,r0 */
L8c01bf54: *stop=0x8c01bf54u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c01bf56; /* fmov.s @(r0,r14),fr2 */
L8c01bf56: *stop=0x8c01bf56u;r[0]=40u;goto L8c01bf58; /* mov #0x28,r0 */
L8c01bf58: *stop=0x8c01bf58u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c01bf5a; /* fsub fr3,fr2 */
L8c01bf5a: *stop=0x8c01bf5au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto L8c01bf5c; /* fmov fr2,@(r0,r15) */
L8c01bf5c: *stop=0x8c01bf5cu;r[0]=44u;goto L8c01bf5e; /* mov #0x2c,r0 */
L8c01bf5e: *stop=0x8c01bf5eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[15]);goto L8c01bf60; /* fmov fr15,@(r0,r15) */
L8c01bf60: *stop=0x8c01bf60u;r[0]=4294967180u;goto L8c01bf62; /* mov #-0x74,r0 */
L8c01bf62: *stop=0x8c01bf62u;r[0]=r[0]&255u;goto L8c01bf64; /* extu.b r0,r0 */
L8c01bf64: *stop=0x8c01bf64u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01bf66; /* fmov.s @(r0,r15),fr3 */
L8c01bf66: *stop=0x8c01bf66u;r[0]=16u;goto L8c01bf68; /* mov #0x10,r0 */
L8c01bf68: *stop=0x8c01bf68u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c01bf6a; /* fmov.s @(r0,r14),fr2 */
L8c01bf6a: *stop=0x8c01bf6au;r[0]=48u;goto L8c01bf6c; /* mov #0x30,r0 */
L8c01bf6c: *stop=0x8c01bf6cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c01bf6e; /* fsub fr3,fr2 */
L8c01bf6e: *stop=0x8c01bf6eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto L8c01bf70; /* fmov fr2,@(r0,r15) */
L8c01bf70: target=r[3];*pr=0x8c01bf74u;*stop=0x8c01bf72u;r[4]+=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01bf74; /* jsr @r3 */
L8c01bf74: *stop=0x8c01bf74u;r[4]=r[14];goto L8c01bf76; /* mov r14,r4 */
L8c01bf76: *stop=0x8c01bf76u;r[0]=4u;goto L8c01bf78; /* mov #0x4,r0 */
L8c01bf78: *stop=0x8c01bf78u;r[4]+=32u;goto L8c01bf7a; /* add #0x20,r4 */
L8c01bf7a: *stop=0x8c01bf7au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c01bf7c; /* fmov.s @(r0,r4),fr3 */
L8c01bf7c: *stop=0x8c01bf7cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);goto L8c01bf7e; /* fmov.s @r4,fr4 */
L8c01bf7e: *stop=0x8c01bf7eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c01bf80; /* fcmp/gt fr4,fr3 */
L8c01bf80: take=!(*sr&1u);*stop=0x8c01bf82u;fcopy(f,5u,0u);if(take)goto L8c01bf88;goto L8c01bf84; /* bf/s 0x8c01bf88 */
L8c01bf84: *stop=0x8c01bf84u;r[0]=36u;goto L8c01bf86; /* mov #0x24,r0 */
L8c01bf86: *stop=0x8c01bf86u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c01bf88; /* fmov.s @(r0,r14),fr4 */
L8c01bf88: *stop=0x8c01bf88u;r[0]=40u;goto L8c01bf8a; /* mov #0x28,r0 */
L8c01bf8a: *stop=0x8c01bf8au;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c01bf8c; /* fmov.s @(r0,r14),fr3 */
L8c01bf8c: *stop=0x8c01bf8cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c01bf8e; /* fcmp/gt fr4,fr3 */
L8c01bf8e: if(!(*sr&1u))goto L8c01bf92;goto L8c01bf90; /* bf 0x8c01bf92 */
L8c01bf90: *stop=0x8c01bf90u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c01bf92; /* fmov.s @(r0,r14),fr4 */
L8c01bf92: *stop=0x8c01bf92u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c01bf94; /* mov.l @(0x4,r14),r3 */
L8c01bf94: *stop=0x8c01bf94u;r[0]=36u;goto L8c01bf96; /* mov #0x24,r0 */
L8c01bf96: *stop=0x8c01bf96u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[3]);goto L8c01bf98; /* fmov.s @(r0,r3),fr3 */
L8c01bf98: *stop=0x8c01bf98u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c01bf9a; /* fmul fr3,fr4 */
L8c01bf9a: *stop=0x8c01bf9au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c01bf9c; /* fcmp/gt fr4,fr5 */
L8c01bf9c: if((*sr&1u))goto L8c01c022;goto L8c01bf9e; /* bt 0x8c01c022 */
L8c01bf9e: *stop=0x8c01bf9eu;if(!valid_ram(r[11],4u))return -3;r[3]=rd(m,r[11],4);goto L8c01bfa0; /* mov.l @r11,r3 */
L8c01bfa0: *stop=0x8c01bfa0u;*sr=(*sr&~1u)|!(r[8]&r[3]);goto L8c01bfa2; /* tst r8,r3 */
L8c01bfa2: if((*sr&1u))goto L8c01c022;goto L8c01bfa4; /* bt 0x8c01c022 */
L8c01bfa4: *stop=0x8c01bfa4u;r[3]=rd(m,0x8c01c124u,4);goto L8c01bfa6; /* mov.l 0x8c01c124,r3 */
L8c01bfa6: *stop=0x8c01bfa6u;r[4]=4294967208u;goto L8c01bfa8; /* mov #-0x58,r4 */
L8c01bfa8: *stop=0x8c01bfa8u;if(!valid_ram(r[12]+4u,4u))return -3;r[5]=rd(m,r[12]+0x4u,4);goto L8c01bfaa; /* mov.l @(0x4,r12),r5 */
L8c01bfaa: *stop=0x8c01bfaau;r[4]=r[4]&255u;goto L8c01bfac; /* extu.b r4,r4 */
L8c01bfac: target=r[3];*pr=0x8c01bfb0u;*stop=0x8c01bfaeu;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01bfb0; /* jsr @r3 */
L8c01bfb0: *stop=0x8c01bfb0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c01bfb2; /* tst r0,r0 */
L8c01bfb2: if((*sr&1u))goto L8c01c022;goto L8c01bfb4; /* bt 0x8c01c022 */
L8c01bfb4: *stop=0x8c01bfb4u;r[0]=4294967252u;goto L8c01bfb6; /* mov #-0x2c,r0 */
L8c01bfb6: *stop=0x8c01bfb6u;r[0]=r[0]&255u;goto L8c01bfb8; /* extu.b r0,r0 */
L8c01bfb8: *stop=0x8c01bfb8u;if(!valid_ram(r[0]+r[15],4u))return -3;r[0]=rd(m,r[15]+r[0],4);goto L8c01bfba; /* mov.l @(r0,r15),r0 */
L8c01bfba: *stop=0x8c01bfbau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c01bfbc; /* cmp/eq #0x1,r0 */
L8c01bfbc: if(!(*sr&1u))goto L8c01bfec;goto L8c01bfbe; /* bf 0x8c01bfec */
L8c01bfbe: *stop=0x8c01bfbeu;r[0]=4294967268u;goto L8c01bfc0; /* mov #-0x1c,r0 */
L8c01bfc0: *stop=0x8c01bfc0u;r[0]=r[0]&255u;goto L8c01bfc2; /* extu.b r0,r0 */
L8c01bfc2: *stop=0x8c01bfc2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01bfc4; /* fmov.s @(r0,r15),fr3 */
L8c01bfc4: *stop=0x8c01bfc4u;r[0]=4294967212u;goto L8c01bfc6; /* mov #-0x54,r0 */
L8c01bfc6: *stop=0x8c01bfc6u;r[0]=r[0]&255u;goto L8c01bfc8; /* extu.b r0,r0 */
L8c01bfc8: *stop=0x8c01bfc8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01bfca; /* fmov.s @(r0,r15),fr2 */
L8c01bfca: *stop=0x8c01bfcau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c01bfcc; /* fcmp/gt fr2,fr3 */
L8c01bfcc: if(!(*sr&1u))goto L8c01bfec;goto L8c01bfce; /* bf 0x8c01bfec */
L8c01bfce: *stop=0x8c01bfceu;r[0]=4294967200u;goto L8c01bfd0; /* mov #-0x60,r0 */
L8c01bfd0: *stop=0x8c01bfd0u;r[0]=r[0]&255u;goto L8c01bfd2; /* extu.b r0,r0 */
L8c01bfd2: *stop=0x8c01bfd2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01bfd4; /* fmov.s @(r0,r15),fr3 */
L8c01bfd4: *stop=0x8c01bfd4u;r[0]=4294967268u;goto L8c01bfd6; /* mov #-0x1c,r0 */
L8c01bfd6: *stop=0x8c01bfd6u;r[0]=r[0]&255u;goto L8c01bfd8; /* extu.b r0,r0 */
L8c01bfd8: *stop=0x8c01bfd8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01bfda; /* fmov.s @(r0,r15),fr2 */
L8c01bfda: *stop=0x8c01bfdau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c01bfdc; /* fcmp/gt fr2,fr3 */
L8c01bfdc: if(!(*sr&1u))goto L8c01bfec;goto L8c01bfde; /* bf 0x8c01bfec */
L8c01bfde: *stop=0x8c01bfdeu;r[0]=4294967268u;goto L8c01bfe0; /* mov #-0x1c,r0 */
L8c01bfe0: *stop=0x8c01bfe0u;r[0]=r[0]&255u;goto L8c01bfe2; /* extu.b r0,r0 */
L8c01bfe2: *stop=0x8c01bfe2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01bfe4; /* fmov.s @(r0,r15),fr3 */
L8c01bfe4: *stop=0x8c01bfe4u;r[0]=4294967200u;goto L8c01bfe6; /* mov #-0x60,r0 */
L8c01bfe6: *stop=0x8c01bfe6u;r[0]=r[0]&255u;goto L8c01bfe8; /* extu.b r0,r0 */
L8c01bfe8: *stop=0x8c01bfeau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01c022; /* bra 0x8c01c022 */
L8c01bfec: *stop=0x8c01bfecu;r[0]=4294967220u;goto L8c01bfee; /* mov #-0x4c,r0 */
L8c01bfee: *stop=0x8c01bfeeu;r[0]=r[0]&255u;goto L8c01bff0; /* extu.b r0,r0 */
L8c01bff0: *stop=0x8c01bff0u;if(!valid_ram(r[0]+r[15],4u))return -3;r[0]=rd(m,r[15]+r[0],4);goto L8c01bff2; /* mov.l @(r0,r15),r0 */
L8c01bff2: *stop=0x8c01bff2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c01bff4; /* cmp/eq #0x1,r0 */
L8c01bff4: if(!(*sr&1u))goto L8c01c022;goto L8c01bff6; /* bf 0x8c01c022 */
L8c01bff6: *stop=0x8c01bff6u;r[0]=4294967236u;goto L8c01bff8; /* mov #-0x3c,r0 */
L8c01bff8: *stop=0x8c01bff8u;r[0]=r[0]&255u;goto L8c01bffa; /* extu.b r0,r0 */
L8c01bffa: *stop=0x8c01bffau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01bffc; /* fmov.s @(r0,r15),fr3 */
L8c01bffc: *stop=0x8c01bffcu;r[0]=4294967212u;goto L8c01bffe; /* mov #-0x54,r0 */
L8c01bffe: *stop=0x8c01bffeu;r[0]=r[0]&255u;goto L8c01c000; /* extu.b r0,r0 */
L8c01c000: *stop=0x8c01c000u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01c002; /* fmov.s @(r0,r15),fr2 */
L8c01c002: *stop=0x8c01c002u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c01c004; /* fcmp/gt fr3,fr2 */
L8c01c004: if(!(*sr&1u))goto L8c01c022;goto L8c01c006; /* bf 0x8c01c022 */
L8c01c006: *stop=0x8c01c006u;r[0]=4294967188u;goto L8c01c008; /* mov #-0x6c,r0 */
L8c01c008: *stop=0x8c01c008u;r[0]=r[0]&255u;goto L8c01c00a; /* extu.b r0,r0 */
L8c01c00a: *stop=0x8c01c00au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01c00c; /* fmov.s @(r0,r15),fr3 */
L8c01c00c: *stop=0x8c01c00cu;r[0]=4294967236u;goto L8c01c00e; /* mov #-0x3c,r0 */
L8c01c00e: *stop=0x8c01c00eu;r[0]=r[0]&255u;goto L8c01c010; /* extu.b r0,r0 */
L8c01c010: *stop=0x8c01c010u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01c012; /* fmov.s @(r0,r15),fr2 */
L8c01c012: *stop=0x8c01c012u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c01c014; /* fcmp/gt fr2,fr3 */
L8c01c014: if(!(*sr&1u))goto L8c01c022;goto L8c01c016; /* bf 0x8c01c022 */
L8c01c016: *stop=0x8c01c016u;r[0]=4294967236u;goto L8c01c018; /* mov #-0x3c,r0 */
L8c01c018: *stop=0x8c01c018u;r[0]=r[0]&255u;goto L8c01c01a; /* extu.b r0,r0 */
L8c01c01a: *stop=0x8c01c01au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01c01c; /* fmov.s @(r0,r15),fr3 */
L8c01c01c: *stop=0x8c01c01cu;r[0]=4294967188u;goto L8c01c01e; /* mov #-0x6c,r0 */
L8c01c01e: *stop=0x8c01c01eu;r[0]=r[0]&255u;goto L8c01c020; /* extu.b r0,r0 */
L8c01c020: *stop=0x8c01c020u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01c022; /* fmov fr3,@(r0,r15) */
L8c01c022: *stop=0x8c01c022u;r[11]+=12u;goto L8c01c024; /* add #0xc,r11 */
L8c01c024: *stop=0x8c01c024u;r[13]+=12u;goto L8c01c026; /* add #0xc,r13 */
L8c01c026: *stop=0x8c01c026u;r[9]+=1u;goto L8c01c028; /* add #0x1,r9 */
L8c01c028: *stop=0x8c01c028u;r[12]+=12u;goto L8c01c02a; /* add #0xc,r12 */
L8c01c02a: *stop=0x8c01c02au;r[1]=rd(m,0x8c01c128u,4);goto L8c01c02c; /* mov.l 0x8c01c128,r1 */
L8c01c02c: *stop=0x8c01c02cu;r[3]=r[9]&65535u;goto L8c01c02e; /* extu.w r9,r3 */
L8c01c02e: *stop=0x8c01c02eu;if(!valid_ram(r[1],2u))return -3;r[2]=sx(m,r[1],2);goto L8c01c030; /* mov.w @r1,r2 */
L8c01c030: *stop=0x8c01c030u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c01c032; /* cmp/ge r2,r3 */
L8c01c032: if(!(*sr&1u))goto L8c01bf40;goto L8c01c034; /* bf 0x8c01bf40 */
L8c01c034: *stop=0x8c01c034u;r[0]=4294967200u;goto L8c01c036; /* mov #-0x60,r0 */
L8c01c036: *stop=0x8c01c036u;r[0]=r[0]&255u;goto L8c01c038; /* extu.b r0,r0 */
L8c01c038: *stop=0x8c01c038u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01c03a; /* fmov.s @(r0,r15),fr3 */
L8c01c03a: *stop=0x8c01c03au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][13],&f->fpscr,sr,0);if(status)return status;goto L8c01c03c; /* fcmp/eq fr13,fr3 */
L8c01c03c: if(!(*sr&1u))goto L8c01c050;goto L8c01c03e; /* bf 0x8c01c050 */
L8c01c03e: *stop=0x8c01c03eu;r[0]=0x8c01c12cu;goto L8c01c040; /* mova 0x8c01c12c,r0 */
L8c01c040: *stop=0x8c01c040u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c01c042; /* fmov.s @r0,fr3 */
L8c01c042: *stop=0x8c01c042u;r[0]=4294967188u;goto L8c01c044; /* mov #-0x6c,r0 */
L8c01c044: *stop=0x8c01c044u;r[0]=r[0]&255u;goto L8c01c046; /* extu.b r0,r0 */
L8c01c046: *stop=0x8c01c046u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01c048; /* fmov.s @(r0,r15),fr2 */
L8c01c048: *stop=0x8c01c048u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c01c04a; /* fcmp/eq fr3,fr2 */
L8c01c04a: if(!(*sr&1u))goto L8c01c050;goto L8c01c04c; /* bf 0x8c01c050 */
L8c01c04c: *stop=0x8c01c04eu;goto L8c01c270; /* bra 0x8c01c270 */
L8c01c050: *stop=0x8c01c050u;r[0]=4294967172u;goto L8c01c052; /* mov #-0x7c,r0 */
L8c01c052: *stop=0x8c01c052u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c01c054; /* fldi1 fr4 */
L8c01c054: *stop=0x8c01c054u;r[0]=r[0]&255u;goto L8c01c056; /* extu.b r0,r0 */
L8c01c056: *stop=0x8c01c056u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01c058; /* fmov.s @(r0,r15),fr3 */
L8c01c058: *stop=0x8c01c058u;r[0]=76u;goto L8c01c05a; /* mov #0x4c,r0 */
L8c01c05a: *stop=0x8c01c05au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01c05c; /* fmov fr3,@(r0,r15) */
L8c01c05c: *stop=0x8c01c05cu;r[0]=4294967200u;goto L8c01c05e; /* mov #-0x60,r0 */
L8c01c05e: *stop=0x8c01c05eu;r[0]=r[0]&255u;goto L8c01c060; /* extu.b r0,r0 */
L8c01c060: *stop=0x8c01c060u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01c062; /* fmov.s @(r0,r15),fr3 */
L8c01c062: *stop=0x8c01c062u;r[0]=80u;goto L8c01c064; /* mov #0x50,r0 */
L8c01c064: *stop=0x8c01c064u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01c066; /* fmov fr3,@(r0,r15) */
L8c01c066: *stop=0x8c01c066u;r[0]=4294967180u;goto L8c01c068; /* mov #-0x74,r0 */
L8c01c068: *stop=0x8c01c068u;r[0]=r[0]&255u;goto L8c01c06a; /* extu.b r0,r0 */
L8c01c06a: *stop=0x8c01c06au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01c06c; /* fmov.s @(r0,r15),fr3 */
L8c01c06c: *stop=0x8c01c06cu;r[0]=84u;goto L8c01c06e; /* mov #0x54,r0 */
L8c01c06e: *stop=0x8c01c06eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01c070; /* fmov fr3,@(r0,r15) */
L8c01c070: *stop=0x8c01c070u;r[0]=88u;goto L8c01c072; /* mov #0x58,r0 */
L8c01c072: *stop=0x8c01c072u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[15]);goto L8c01c074; /* fmov fr15,@(r0,r15) */
L8c01c074: *stop=0x8c01c074u;r[0]=92u;goto L8c01c076; /* mov #0x5c,r0 */
L8c01c076: *stop=0x8c01c076u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[15]);goto L8c01c078; /* fmov fr4,@(r0,r15) */
L8c01c078: *stop=0x8c01c078u;r[0]=96u;goto L8c01c07a; /* mov #0x60,r0 */
L8c01c07a: *stop=0x8c01c07au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[15]);goto L8c01c07c; /* fmov fr15,@(r0,r15) */
L8c01c07c: *stop=0x8c01c07cu;r[0]=4294967172u;goto L8c01c07e; /* mov #-0x7c,r0 */
L8c01c07e: *stop=0x8c01c07eu;r[0]=r[0]&255u;goto L8c01c080; /* extu.b r0,r0 */
L8c01c080: *stop=0x8c01c080u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01c082; /* fmov.s @(r0,r15),fr3 */
L8c01c082: *stop=0x8c01c082u;r[0]=52u;goto L8c01c084; /* mov #0x34,r0 */
L8c01c084: *stop=0x8c01c084u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01c086; /* fmov fr3,@(r0,r15) */
L8c01c086: *stop=0x8c01c086u;r[0]=4294967200u;goto L8c01c088; /* mov #-0x60,r0 */
L8c01c088: *stop=0x8c01c088u;r[0]=r[0]&255u;goto L8c01c08a; /* extu.b r0,r0 */
L8c01c08a: *stop=0x8c01c08au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01c08c; /* fmov.s @(r0,r15),fr3 */
L8c01c08c: *stop=0x8c01c08cu;r[0]=56u;goto L8c01c08e; /* mov #0x38,r0 */
L8c01c08e: *stop=0x8c01c08eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01c090; /* fmov fr3,@(r0,r15) */
L8c01c090: *stop=0x8c01c090u;r[0]=4294967180u;goto L8c01c092; /* mov #-0x74,r0 */
L8c01c092: *stop=0x8c01c092u;r[6]=r[15];goto L8c01c094; /* mov r15,r6 */
L8c01c094: *stop=0x8c01c094u;r[0]=r[0]&255u;goto L8c01c096; /* extu.b r0,r0 */
L8c01c096: *stop=0x8c01c096u;r[5]=r[15];goto L8c01c098; /* mov r15,r5 */
L8c01c098: *stop=0x8c01c098u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01c09a; /* fmov.s @(r0,r15),fr3 */
L8c01c09a: *stop=0x8c01c09au;r[0]=60u;goto L8c01c09c; /* mov #0x3c,r0 */
L8c01c09c: *stop=0x8c01c09cu;r[3]=rd(m,0x8c01c130u,4);goto L8c01c09e; /* mov.l 0x8c01c130,r3 */
L8c01c09e: *stop=0x8c01c09eu;r[4]=r[15];goto L8c01c0a0; /* mov r15,r4 */
L8c01c0a0: *stop=0x8c01c0a0u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c01c0a2; /* fmov fr3,@(r0,r15) */
L8c01c0a2: *stop=0x8c01c0a2u;r[0]=64u;goto L8c01c0a4; /* mov #0x40,r0 */
L8c01c0a4: *stop=0x8c01c0a4u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[15]);goto L8c01c0a6; /* fmov fr15,@(r0,r15) */
L8c01c0a6: *stop=0x8c01c0a6u;r[0]=68u;goto L8c01c0a8; /* mov #0x44,r0 */
L8c01c0a8: *stop=0x8c01c0a8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[15]);goto L8c01c0aa; /* fmov fr4,@(r0,r15) */
L8c01c0aa: *stop=0x8c01c0aau;r[0]=72u;goto L8c01c0ac; /* mov #0x48,r0 */
L8c01c0ac: *stop=0x8c01c0acu;r[5]+=76u;goto L8c01c0ae; /* add #0x4c,r5 */
L8c01c0ae: *stop=0x8c01c0aeu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[15]);goto L8c01c0b0; /* fmov fr15,@(r0,r15) */
L8c01c0b0: *stop=0x8c01c0b0u;r[6]+=28u;goto L8c01c0b2; /* add #0x1c,r6 */
L8c01c0b2: target=r[3];*pr=0x8c01c0b6u;*stop=0x8c01c0b4u;r[4]+=100u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c0b6; /* jsr @r3 */
L8c01c0b6: *stop=0x8c01c0b6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c01c0b8; /* fldi0 fr3 */
L8c01c0b8: *stop=0x8c01c0b8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c01c0ba; /* fcmp/eq fr3,fr0 */
L8c01c0ba: if(!(*sr&1u))goto L8c01c0dc;goto L8c01c0bc; /* bf 0x8c01c0dc */
L8c01c0bc: *stop=0x8c01c0bcu;r[0]=28u;goto L8c01c0be; /* mov #0x1c,r0 */
L8c01c0be: *stop=0x8c01c0beu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01c0c0; /* fmov.s @(r0,r15),fr2 */
L8c01c0c0: *stop=0x8c01c0c0u;r[0]=4294967196u;goto L8c01c0c2; /* mov #-0x64,r0 */
L8c01c0c2: *stop=0x8c01c0c2u;r[0]=r[0]&255u;goto L8c01c0c4; /* extu.b r0,r0 */
L8c01c0c4: *stop=0x8c01c0c4u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto L8c01c0c6; /* fmov fr2,@(r0,r15) */
L8c01c0c6: *stop=0x8c01c0c6u;r[0]=32u;goto L8c01c0c8; /* mov #0x20,r0 */
L8c01c0c8: *stop=0x8c01c0c8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01c0ca; /* fmov.s @(r0,r15),fr2 */
L8c01c0ca: *stop=0x8c01c0cau;r[0]=4294967200u;goto L8c01c0cc; /* mov #-0x60,r0 */
L8c01c0cc: *stop=0x8c01c0ccu;r[0]=r[0]&255u;goto L8c01c0ce; /* extu.b r0,r0 */
L8c01c0ce: *stop=0x8c01c0ceu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto L8c01c0d0; /* fmov fr2,@(r0,r15) */
L8c01c0d0: *stop=0x8c01c0d0u;r[0]=36u;goto L8c01c0d2; /* mov #0x24,r0 */
L8c01c0d2: *stop=0x8c01c0d2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01c0d4; /* fmov.s @(r0,r15),fr2 */
L8c01c0d4: *stop=0x8c01c0d4u;r[0]=4294967204u;goto L8c01c0d6; /* mov #-0x5c,r0 */
L8c01c0d6: *stop=0x8c01c0d6u;r[0]=r[0]&255u;goto L8c01c0d8; /* extu.b r0,r0 */
L8c01c0d8: *stop=0x8c01c0dau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto L8c01c0e2; /* bra 0x8c01c0e2 */
L8c01c0dc: *stop=0x8c01c0dcu;r[0]=4294967200u;goto L8c01c0de; /* mov #-0x60,r0 */
L8c01c0de: *stop=0x8c01c0deu;r[0]=r[0]&255u;goto L8c01c0e0; /* extu.b r0,r0 */
L8c01c0e0: *stop=0x8c01c0e0u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,14u,r[0]+r[15]);goto L8c01c0e2; /* fmov fr14,@(r0,r15) */
L8c01c0e2: *stop=0x8c01c0e2u;r[5]=r[15];goto L8c01c0e4; /* mov r15,r5 */
L8c01c0e4: *stop=0x8c01c0e4u;r[3]=rd(m,0x8c01c130u,4);goto L8c01c0e6; /* mov.l 0x8c01c130,r3 */
L8c01c0e6: *stop=0x8c01c0e6u;r[6]=r[15];goto L8c01c0e8; /* mov r15,r6 */
L8c01c0e8: *stop=0x8c01c0e8u;r[4]=r[15];goto L8c01c0ea; /* mov r15,r4 */
L8c01c0ea: *stop=0x8c01c0eau;r[6]+=28u;goto L8c01c0ec; /* add #0x1c,r6 */
L8c01c0ec: *stop=0x8c01c0ecu;r[5]+=52u;goto L8c01c0ee; /* add #0x34,r5 */
L8c01c0ee: target=r[3];*pr=0x8c01c0f2u;*stop=0x8c01c0f0u;r[4]+=100u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c0f2; /* jsr @r3 */
L8c01c0f2: *stop=0x8c01c0f2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c01c0f4; /* fldi0 fr3 */
L8c01c0f4: *stop=0x8c01c0f4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c01c0f6; /* fcmp/eq fr3,fr0 */
L8c01c0f6: take=!(*sr&1u);*stop=0x8c01c0f8u;r[4]=0u;if(take)goto L8c01c140;goto L8c01c0fa; /* bf/s 0x8c01c140 */
L8c01c0fa: *stop=0x8c01c0fau;r[0]=28u;goto L8c01c0fc; /* mov #0x1c,r0 */
L8c01c0fc: *stop=0x8c01c0fcu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01c0fe; /* fmov.s @(r0,r15),fr2 */
L8c01c0fe: *stop=0x8c01c0feu;r[0]=4294967184u;goto L8c01c100; /* mov #-0x70,r0 */
L8c01c100: *stop=0x8c01c100u;r[0]=r[0]&255u;goto L8c01c102; /* extu.b r0,r0 */
L8c01c102: *stop=0x8c01c102u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto L8c01c104; /* fmov fr2,@(r0,r15) */
L8c01c104: *stop=0x8c01c104u;r[0]=32u;goto L8c01c106; /* mov #0x20,r0 */
L8c01c106: *stop=0x8c01c106u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01c108; /* fmov.s @(r0,r15),fr2 */
L8c01c108: *stop=0x8c01c108u;r[0]=4294967188u;goto L8c01c10a; /* mov #-0x6c,r0 */
L8c01c10a: *stop=0x8c01c10au;r[0]=r[0]&255u;goto L8c01c10c; /* extu.b r0,r0 */
L8c01c10c: *stop=0x8c01c10cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto L8c01c10e; /* fmov fr2,@(r0,r15) */
L8c01c10e: *stop=0x8c01c10eu;r[0]=36u;goto L8c01c110; /* mov #0x24,r0 */
L8c01c110: *stop=0x8c01c110u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto L8c01c112; /* fmov.s @(r0,r15),fr2 */
L8c01c112: *stop=0x8c01c112u;r[0]=4294967192u;goto L8c01c114; /* mov #-0x68,r0 */
L8c01c114: *stop=0x8c01c114u;r[0]=r[0]&255u;goto L8c01c116; /* extu.b r0,r0 */
L8c01c116: *stop=0x8c01c118u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto L8c01c146; /* bra 0x8c01c146 */
L8c01c140: *stop=0x8c01c140u;r[0]=4294967200u;goto L8c01c142; /* mov #-0x60,r0 */
L8c01c142: *stop=0x8c01c142u;r[0]=r[0]&255u;goto L8c01c144; /* extu.b r0,r0 */
L8c01c144: *stop=0x8c01c144u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,14u,r[0]+r[15]);goto L8c01c146; /* fmov fr14,@(r0,r15) */
L8c01c146: *stop=0x8c01c146u;r[3]=rd(m,0x8c01c300u,4);goto L8c01c148; /* mov.l 0x8c01c300,r3 */
L8c01c148: target=r[3];*pr=0x8c01c14cu;*stop=0x8c01c14au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c14c; /* jsr @r3 */
L8c01c14c: *stop=0x8c01c14cu;r[5]=4294967196u;goto L8c01c14e; /* mov #-0x64,r5 */
L8c01c14e: *stop=0x8c01c14eu;r[2]=rd(m,0x8c01c304u,4);goto L8c01c150; /* mov.l 0x8c01c304,r2 */
L8c01c150: *stop=0x8c01c150u;r[5]=r[5]&255u;goto L8c01c152; /* extu.b r5,r5 */
L8c01c152: *stop=0x8c01c152u;r[6]=r[15];goto L8c01c154; /* mov r15,r6 */
L8c01c154: *stop=0x8c01c154u;r[6]+=124u;goto L8c01c156; /* add #0x7c,r6 */
L8c01c156: *stop=0x8c01c156u;r[5]+=r[15];goto L8c01c158; /* add r15,r5 */
L8c01c158: target=r[2];*pr=0x8c01c15cu;*stop=0x8c01c15au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c15c; /* jsr @r2 */
L8c01c15c: *stop=0x8c01c15cu;r[3]=rd(m,0x8c01c308u,4);goto L8c01c15e; /* mov.l 0x8c01c308,r3 */
L8c01c15e: target=r[3];*pr=0x8c01c162u;*stop=0x8c01c160u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c162; /* jsr @r3 */
L8c01c162: *stop=0x8c01c162u;r[0]=4294967168u;goto L8c01c164; /* mov #-0x80,r0 */
L8c01c164: *stop=0x8c01c164u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c01c166; /* fldi0 fr3 */
L8c01c166: *stop=0x8c01c166u;r[0]=r[0]&255u;goto L8c01c168; /* extu.b r0,r0 */
L8c01c168: *stop=0x8c01c168u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[15]);goto L8c01c16a; /* fmov.s @(r0,r15),fr4 */
L8c01c16a: *stop=0x8c01c16au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c01c16c; /* fcmp/gt fr4,fr3 */
L8c01c16c: if((*sr&1u))goto L8c01c176;goto L8c01c16e; /* bt 0x8c01c176 */
L8c01c16e: *stop=0x8c01c16eu;r[0]=0x8c01c30cu;goto L8c01c170; /* mova 0x8c01c30c,r0 */
L8c01c170: *stop=0x8c01c170u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c01c172; /* fmov.s @r0,fr2 */
L8c01c172: *stop=0x8c01c172u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c01c174; /* fcmp/gt fr2,fr4 */
L8c01c174: if(!(*sr&1u))goto L8c01c192;goto L8c01c176; /* bf 0x8c01c192 */
L8c01c176: *stop=0x8c01c176u;r[2]=rd(m,0x8c01c300u,4);goto L8c01c178; /* mov.l 0x8c01c300,r2 */
L8c01c178: target=r[2];*pr=0x8c01c17cu;*stop=0x8c01c17au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c17c; /* jsr @r2 */
L8c01c17c: *stop=0x8c01c17cu;r[5]=4294967184u;goto L8c01c17e; /* mov #-0x70,r5 */
L8c01c17e: *stop=0x8c01c17eu;r[3]=rd(m,0x8c01c304u,4);goto L8c01c180; /* mov.l 0x8c01c304,r3 */
L8c01c180: *stop=0x8c01c180u;r[5]=r[5]&255u;goto L8c01c182; /* extu.b r5,r5 */
L8c01c182: *stop=0x8c01c182u;r[6]=r[15];goto L8c01c184; /* mov r15,r6 */
L8c01c184: *stop=0x8c01c184u;r[6]+=124u;goto L8c01c186; /* add #0x7c,r6 */
L8c01c186: *stop=0x8c01c186u;r[5]+=r[15];goto L8c01c188; /* add r15,r5 */
L8c01c188: target=r[3];*pr=0x8c01c18cu;*stop=0x8c01c18au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c18c; /* jsr @r3 */
L8c01c18c: *stop=0x8c01c18cu;r[2]=rd(m,0x8c01c308u,4);goto L8c01c18e; /* mov.l 0x8c01c308,r2 */
L8c01c18e: target=r[2];*pr=0x8c01c192u;*stop=0x8c01c190u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c192; /* jsr @r2 */
L8c01c192: *stop=0x8c01c192u;r[3]=r[15];goto L8c01c194; /* mov r15,r3 */
L8c01c194: *stop=0x8c01c194u;r[3]+=20u;goto L8c01c196; /* add #0x14,r3 */
L8c01c196: *stop=0x8c01c196u;if(!valid_ram(r[15]+36u,4u))return -3;wr(m,r[15]+0x24u,r[3],4);goto L8c01c198; /* mov.l r3,@(0x24,r15) */
L8c01c198: *stop=0x8c01c198u;r[3]=2u;goto L8c01c19a; /* mov #0x2,r3 */
L8c01c19a: *stop=0x8c01c19au;r[0]=0x8c01c310u;goto L8c01c19c; /* mova 0x8c01c310,r0 */
L8c01c19c: *stop=0x8c01c19cu;r[4]=rd(m,0x8c01c318u,4);goto L8c01c19e; /* mov.l 0x8c01c318,r4 */
L8c01c19e: *stop=0x8c01c19eu;r[2]=r[15];goto L8c01c1a0; /* mov r15,r2 */
L8c01c1a0: *stop=0x8c01c1a0u;r[2]+=12u;goto L8c01c1a2; /* add #0xc,r2 */
L8c01c1a2: *stop=0x8c01c1a2u;if(!valid_ram(r[15]+40u,4u))return -3;wr(m,r[15]+0x28u,r[2],4);goto L8c01c1a4; /* mov.l r2,@(0x28,r15) */
L8c01c1a4: *stop=0x8c01c1a4u;r[6]=64u;goto L8c01c1a6; /* mov #0x40,r6 */
L8c01c1a6: *stop=0x8c01c1a6u;if(!valid_ram(r[15]+44u,4u))return -3;wr(m,r[15]+0x2cu,r[10],4);goto L8c01c1a8; /* mov.l r10,@(0x2c,r15) */
L8c01c1a8: *stop=0x8c01c1a8u;if(!valid_ram(r[15]+48u,4u))return -3;wr(m,r[15]+0x30u,r[3],4);goto L8c01c1aa; /* mov.l r3,@(0x30,r15) */
L8c01c1aa: *stop=0x8c01c1aau;if(!valid_ram(r[15]+36u,4u))return -3;r[2]=rd(m,r[15]+0x24u,4);goto L8c01c1ac; /* mov.l @(0x24,r15),r2 */
L8c01c1ac: *stop=0x8c01c1acu;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[2]);goto L8c01c1ae; /* fmov.s fr15,@r2 */
L8c01c1ae: *stop=0x8c01c1aeu;if(!valid_ram(r[15]+36u,4u))return -3;r[3]=rd(m,r[15]+0x24u,4);goto L8c01c1b0; /* mov.l @(0x24,r15),r3 */
L8c01c1b0: *stop=0x8c01c1b0u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]);goto L8c01c1b2; /* fmov.s @r0,fr14 */
L8c01c1b2: *stop=0x8c01c1b2u;r[0]=0x8c01c314u;goto L8c01c1b4; /* mova 0x8c01c314,r0 */
L8c01c1b4: *stop=0x8c01c1b4u;r[3]+=8u;goto L8c01c1b6; /* add #0x8,r3 */
L8c01c1b6: *stop=0x8c01c1b6u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,14u,r[3]);goto L8c01c1b8; /* fmov.s fr14,@r3 */
L8c01c1b8: *stop=0x8c01c1b8u;if(!valid_ram(r[15]+36u,4u))return -3;r[3]=rd(m,r[15]+0x24u,4);goto L8c01c1ba; /* mov.l @(0x24,r15),r3 */
L8c01c1ba: *stop=0x8c01c1bau;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto L8c01c1bc; /* fmov.s @r0,fr4 */
L8c01c1bc: *stop=0x8c01c1bcu;r[0]=4u;goto L8c01c1be; /* mov #0x4,r0 */
L8c01c1be: *stop=0x8c01c1beu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[3]);goto L8c01c1c0; /* fmov fr4,@(r0,r3) */
L8c01c1c0: *stop=0x8c01c1c0u;r[0]=4u;goto L8c01c1c2; /* mov #0x4,r0 */
L8c01c1c2: *stop=0x8c01c1c2u;if(!valid_ram(r[15]+36u,4u))return -3;r[3]=rd(m,r[15]+0x24u,4);goto L8c01c1c4; /* mov.l @(0x24,r15),r3 */
L8c01c1c4: *stop=0x8c01c1c4u;r[3]+=8u;goto L8c01c1c6; /* add #0x8,r3 */
L8c01c1c6: *stop=0x8c01c1c6u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[3]);goto L8c01c1c8; /* fmov fr4,@(r0,r3) */
L8c01c1c8: *stop=0x8c01c1c8u;if(!valid_ram(r[15]+40u,4u))return -3;r[3]=rd(m,r[15]+0x28u,4);goto L8c01c1ca; /* mov.l @(0x28,r15),r3 */
L8c01c1ca: *stop=0x8c01c1cau;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[4],4);goto L8c01c1cc; /* mov.l r4,@r3 */
L8c01c1cc: *stop=0x8c01c1ccu;if(!valid_ram(r[15]+40u,4u))return -3;r[3]=rd(m,r[15]+0x28u,4);goto L8c01c1ce; /* mov.l @(0x28,r15),r3 */
L8c01c1ce: *stop=0x8c01c1ceu;r[3]+=4u;goto L8c01c1d0; /* add #0x4,r3 */
L8c01c1d0: *stop=0x8c01c1d0u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[4],4);goto L8c01c1d2; /* mov.l r4,@r3 */
L8c01c1d2: *stop=0x8c01c1d2u;r[0]=0x8c01c31cu;goto L8c01c1d4; /* mova 0x8c01c31c,r0 */
L8c01c1d4: *stop=0x8c01c1d4u;r[3]=rd(m,0x8c01c320u,4);goto L8c01c1d6; /* mov.l 0x8c01c320,r3 */
L8c01c1d6: *stop=0x8c01c1d6u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto L8c01c1d8; /* fmov.s @r0,fr4 */
L8c01c1d8: *stop=0x8c01c1d8u;r[4]=r[15];goto L8c01c1da; /* mov r15,r4 */
L8c01c1da: *stop=0x8c01c1dau;r[5]=2u;goto L8c01c1dc; /* mov #0x2,r5 */
L8c01c1dc: target=r[3];*pr=0x8c01c1e0u;*stop=0x8c01c1deu;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c1e0; /* jsr @r3 */
L8c01c1e0: *stop=0x8c01c1e0u;r[0]=4294967168u;goto L8c01c1e2; /* mov #-0x80,r0 */
L8c01c1e2: *stop=0x8c01c1e2u;r[0]=r[0]&255u;goto L8c01c1e4; /* extu.b r0,r0 */
L8c01c1e4: *stop=0x8c01c1e4u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c01c1e6; /* fmov.s @(r0,r15),fr3 */
L8c01c1e6: *stop=0x8c01c1e6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c01c1e8; /* ftrc fr3,FPUL */
L8c01c1e8: *stop=0x8c01c1e8u;r[5]=*fpul;goto L8c01c1ea; /* sts FPUL,r5 */
L8c01c1ea: *stop=0x8c01c1eau;r[4]=(uint32_t)(int32_t)(int16_t)r[5];goto L8c01c1ec; /* exts.w r5,r4 */
L8c01c1ec: *stop=0x8c01c1ecu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c01c1ee; /* cmp/pz r4 */
L8c01c1ee: if(!(*sr&1u))goto L8c01c1f8;goto L8c01c1f0; /* bf 0x8c01c1f8 */
L8c01c1f0: *stop=0x8c01c1f0u;r[2]=120u;goto L8c01c1f2; /* mov #0x78,r2 */
L8c01c1f2: *stop=0x8c01c1f2u;r[2]<<=2;goto L8c01c1f4; /* shll2 r2 */
L8c01c1f4: *stop=0x8c01c1f4u;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]);goto L8c01c1f6; /* cmp/gt r2,r4 */
L8c01c1f6: if(!(*sr&1u))goto L8c01c1fa;goto L8c01c1f8; /* bf 0x8c01c1fa */
L8c01c1f8: *stop=0x8c01c1f8u;r[5]=r[10];goto L8c01c1fa; /* mov r10,r5 */
L8c01c1fa: *stop=0x8c01c1fau;r[3]=rd(m,0x8c01c324u,4);goto L8c01c1fc; /* mov.l 0x8c01c324,r3 */
L8c01c1fc: *stop=0x8c01c1fcu;r[4]=r[15];goto L8c01c1fe; /* mov r15,r4 */
L8c01c1fe: *stop=0x8c01c1feu;r[4]+=4u;goto L8c01c200; /* add #0x4,r4 */
L8c01c200: *stop=0x8c01c200u;r[0]=4u;goto L8c01c202; /* mov #0x4,r0 */
L8c01c202: *stop=0x8c01c202u;r[6]=r[4];goto L8c01c204; /* mov r4,r6 */
L8c01c204: *stop=0x8c01c204u;if(!valid_ram(r[15]+36u,4u))return -3;wr(m,r[15]+0x24u,r[4],4);goto L8c01c206; /* mov.l r4,@(0x24,r15) */
L8c01c206: *stop=0x8c01c206u;if(!valid_ram(r[15]+40u,4u))return -3;wr(m,r[15]+0x28u,r[3],4);goto L8c01c208; /* mov.l r3,@(0x28,r15) */
L8c01c208: *stop=0x8c01c208u;r[3]=4u;goto L8c01c20a; /* mov #0x4,r3 */
L8c01c20a: *stop=0x8c01c20au;if(!valid_ram(r[15]+44u,4u))return -3;wr(m,r[15]+0x2cu,r[10],4);goto L8c01c20c; /* mov.l r10,@(0x2c,r15) */
L8c01c20c: *stop=0x8c01c20cu;if(!valid_ram(r[15]+48u,4u))return -3;wr(m,r[15]+0x30u,r[3],4);goto L8c01c20e; /* mov.l r3,@(0x30,r15) */
L8c01c20e: *stop=0x8c01c20eu;r[3]=(uint32_t)(int32_t)(int16_t)r[5];goto L8c01c210; /* exts.w r5,r3 */
L8c01c210: *stop=0x8c01c210u;*fpul=r[3];goto L8c01c212; /* lds r3,FPUL */
L8c01c212: *stop=0x8c01c212u;r[5]=r[6];goto L8c01c214; /* mov r6,r5 */
L8c01c214: *stop=0x8c01c214u;r[5]+=8u;goto L8c01c216; /* add #0x8,r5 */
L8c01c216: *stop=0x8c01c216u;r[3]=rd(m,0x8c01c328u,4);goto L8c01c218; /* mov.l 0x8c01c328,r3 */
L8c01c218: *stop=0x8c01c218u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[6]);goto L8c01c21a; /* fmov.s fr15,@r6 */
L8c01c21a: *stop=0x8c01c21au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][4]);if(status)return status;goto L8c01c21c; /* float FPUL,fr4 */
L8c01c21c: *stop=0x8c01c21cu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[6]);goto L8c01c21e; /* fmov fr4,@(r0,r6) */
L8c01c21e: *stop=0x8c01c21eu;r[0]=0x8c01c30cu;goto L8c01c220; /* mova 0x8c01c30c,r0 */
L8c01c220: *stop=0x8c01c220u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[5]);goto L8c01c222; /* fmov.s fr15,@r5 */
L8c01c222: *stop=0x8c01c222u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto L8c01c224; /* fmov.s @r0,fr5 */
L8c01c224: *stop=0x8c01c224u;r[0]=4u;goto L8c01c226; /* mov #0x4,r0 */
L8c01c226: *stop=0x8c01c226u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[5]);goto L8c01c228; /* fmov fr5,@(r0,r5) */
L8c01c228: *stop=0x8c01c228u;r[5]=r[6];goto L8c01c22a; /* mov r6,r5 */
L8c01c22a: *stop=0x8c01c22au;r[5]+=16u;goto L8c01c22c; /* add #0x10,r5 */
L8c01c22c: *stop=0x8c01c22cu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,14u,r[5]);goto L8c01c22e; /* fmov.s fr14,@r5 */
L8c01c22e: *stop=0x8c01c22eu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[5]);goto L8c01c230; /* fmov fr5,@(r0,r5) */
L8c01c230: *stop=0x8c01c230u;r[5]=r[6];goto L8c01c232; /* mov r6,r5 */
L8c01c232: *stop=0x8c01c232u;r[5]+=24u;goto L8c01c234; /* add #0x18,r5 */
L8c01c234: *stop=0x8c01c234u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,14u,r[5]);goto L8c01c236; /* fmov.s fr14,@r5 */
L8c01c236: *stop=0x8c01c236u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[5]);goto L8c01c238; /* fmov fr4,@(r0,r5) */
L8c01c238: *stop=0x8c01c238u;r[5]=8u;goto L8c01c23a; /* mov #0x8,r5 */
L8c01c23a: target=r[3];*pr=0x8c01c23eu;*stop=0x8c01c23cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c23e; /* jsr @r3 */
L8c01c23e: *stop=0x8c01c23eu;r[2]=rd(m,0x8c01c328u,4);goto L8c01c240; /* mov.l 0x8c01c328,r2 */
L8c01c240: *stop=0x8c01c240u;r[5]=10u;goto L8c01c242; /* mov #0xa,r5 */
L8c01c242: target=r[2];*pr=0x8c01c246u;*stop=0x8c01c244u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c246; /* jsr @r2 */
L8c01c246: *stop=0x8c01c246u;r[3]=rd(m,0x8c01c32cu,4);goto L8c01c248; /* mov.l 0x8c01c32c,r3 */
L8c01c248: *stop=0x8c01c248u;fcopy(f,5u,15u);goto L8c01c24a; /* fmov fr15,fr5 */
L8c01c24a: *stop=0x8c01c24au;fcopy(f,7u,15u);goto L8c01c24c; /* fmov fr15,fr7 */
L8c01c24c: *stop=0x8c01c24cu;fcopy(f,6u,15u);goto L8c01c24e; /* fmov fr15,fr6 */
L8c01c24e: target=r[3];*pr=0x8c01c252u;*stop=0x8c01c250u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c252; /* jsr @r3 */
L8c01c252: *stop=0x8c01c252u;r[2]=rd(m,0x8c01c330u,4);goto L8c01c254; /* mov.l 0x8c01c330,r2 */
L8c01c254: *stop=0x8c01c254u;r[4]=r[15];goto L8c01c256; /* mov r15,r4 */
L8c01c256: *stop=0x8c01c256u;r[5]=4u;goto L8c01c258; /* mov #0x4,r5 */
L8c01c258: *stop=0x8c01c258u;fcopy(f,4u,15u);goto L8c01c25a; /* fmov fr15,fr4 */
L8c01c25a: *stop=0x8c01c25au;r[6]=96u;goto L8c01c25c; /* mov #0x60,r6 */
L8c01c25c: target=r[2];*pr=0x8c01c260u;*stop=0x8c01c25eu;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c260; /* jsr @r2 */
L8c01c260: *stop=0x8c01c260u;r[0]=0x8c01c334u;goto L8c01c262; /* mova 0x8c01c334,r0 */
L8c01c262: *stop=0x8c01c262u;r[3]=rd(m,0x8c01c330u,4);goto L8c01c264; /* mov.l 0x8c01c330,r3 */
L8c01c264: *stop=0x8c01c264u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto L8c01c266; /* fmov.s @r0,fr4 */
L8c01c266: *stop=0x8c01c266u;r[6]=96u;goto L8c01c268; /* mov #0x60,r6 */
L8c01c268: *stop=0x8c01c268u;r[4]=r[15];goto L8c01c26a; /* mov r15,r4 */
L8c01c26a: *stop=0x8c01c26au;r[5]=4u;goto L8c01c26c; /* mov #0x4,r5 */
L8c01c26c: target=r[3];*pr=0x8c01c270u;*stop=0x8c01c26eu;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c270; /* jsr @r3 */
L8c01c270: *stop=0x8c01c270u;r[1]=4294967284u;goto L8c01c272; /* mov #-0xc,r1 */
L8c01c272: *stop=0x8c01c272u;r[1]=r[1]&255u;goto L8c01c274; /* extu.b r1,r1 */
L8c01c274: *stop=0x8c01c274u;r[15]+=r[1];goto L8c01c276; /* add r1,r15 */
L8c01c276: *stop=0x8c01c276u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01c278; /* lds.l @r15+,PR */
L8c01c278: *stop=0x8c01c278u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01c27a; /* fmov.s @r15+,fr13 */
L8c01c27a: *stop=0x8c01c27au;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01c27c; /* fmov.s @r15+,fr14 */
L8c01c27c: *stop=0x8c01c27cu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01c27e; /* fmov.s @r15+,fr15 */
L8c01c27e: *stop=0x8c01c27eu;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c01c280; /* mov.l @r15+,r8 */
L8c01c280: *stop=0x8c01c280u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c01c282; /* mov.l @r15+,r9 */
L8c01c282: *stop=0x8c01c282u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c01c284; /* mov.l @r15+,r10 */
L8c01c284: *stop=0x8c01c284u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c01c286; /* mov.l @r15+,r11 */
L8c01c286: *stop=0x8c01c286u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c01c288; /* mov.l @r15+,r12 */
L8c01c288: *stop=0x8c01c288u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01c28a; /* mov.l @r15+,r13 */
L8c01c28a: *stop=0x8c01c28au;if(*pr!=entry_pr)return -3;*stop=0x8c01c28cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
