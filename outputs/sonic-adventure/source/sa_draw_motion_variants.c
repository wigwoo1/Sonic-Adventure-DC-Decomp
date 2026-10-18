/* Motion/drawing selectors, hierarchy traversal and callbacks; names inferred.
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
SA_STAGE_EXPORT int sa_motion_variant_8c03fe1a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c03fe1a;
L8c03fe1a: *stop=0x8c03fe1au;r[7]=0u;goto L8c03fe1c; /* mov #0x0,r7 */
L8c03fe1c: *stop=0x8c03fe1cu;r[5]=rd(m,0x8c040038u,4);goto L8c03fe1e; /* mov.l 0x8c040038,r5 */
L8c03fe1e: *stop=0x8c03fe1eu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c03fe20; /* tst r4,r4 */
L8c03fe20: take=(*sr&1u);*stop=0x8c03fe22u;r[6]=r[7];if(take)goto L8c03fe46;goto L8c03fe24; /* bt/s 0x8c03fe46 */
L8c03fe24: *stop=0x8c03fe24u;if(!valid_ram(r[4]+4u,4u))return -3;r[3]=rd(m,r[4]+0x4u,4);goto L8c03fe26; /* mov.l @(0x4,r4),r3 */
L8c03fe26: *stop=0x8c03fe26u;*fpul=r[3];goto L8c03fe28; /* lds r3,FPUL */
L8c03fe28: *stop=0x8c03fe28u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c03fe2a; /* float FPUL,fr3 */
L8c03fe2a: *stop=0x8c03fe2au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c03fe2c; /* fcmp/gt fr4,fr3 */
L8c03fe2c: if((*sr&1u))goto L8c03fe32;goto L8c03fe2e; /* bt 0x8c03fe32 */
L8c03fe2e: *stop=0x8c03fe2eu;if(*pr!=entry_pr)return -3;*stop=0x8c03fe30u;r[0]=1u;return 0; /* rts */
L8c03fe32: *stop=0x8c03fe32u;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c03fe34; /* mov.l @r4,r2 */
L8c03fe34: *stop=0x8c03fe34u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c03fe36; /* mov.l r2,@r5 */
L8c03fe36: *stop=0x8c03fe36u;if(!valid_ram(r[4]+4u,4u))return -3;r[3]=rd(m,r[4]+0x4u,4);goto L8c03fe38; /* mov.l @(0x4,r4),r3 */
L8c03fe38: *stop=0x8c03fe38u;if(!valid_ram(r[5]+16u,4u))return -3;wr(m,r[5]+0x10u,r[3],4);goto L8c03fe3a; /* mov.l r3,@(0x10,r5) */
L8c03fe3a: *stop=0x8c03fe3au;r[3]=rd(m,0x8c04003cu,4);goto L8c03fe3c; /* mov.l 0x8c04003c,r3 */
L8c03fe3c: *stop=0x8c03fe3cu;if(!valid_ram(r[4]+10u,2u))return -3;r[0]=sx(m,r[4]+0xau,2);goto L8c03fe3e; /* mov.w @(0xa,r4),r0 */
L8c03fe3e: *stop=0x8c03fe3eu;r[6]=r[0];goto L8c03fe40; /* mov r0,r6 */
L8c03fe40: *stop=0x8c03fe40u;if(!valid_ram(r[4]+8u,2u))return -3;r[0]=sx(m,r[4]+0x8u,2);goto L8c03fe42; /* mov.w @(0x8,r4),r0 */
L8c03fe42: *stop=0x8c03fe42u;r[6]&=r[3];goto L8c03fe44; /* and r3,r6 */
L8c03fe44: *stop=0x8c03fe44u;r[7]=r[0];goto L8c03fe46; /* mov r0,r7 */
L8c03fe46: *stop=0x8c03fe46u;r[0]=12u;goto L8c03fe48; /* mov #0xc,r0 */
L8c03fe48: *stop=0x8c03fe48u;r[3]=rd(m,0x8c040040u,4);goto L8c03fe4a; /* mov.l 0x8c040040,r3 */
L8c03fe4a: *stop=0x8c03fe4au;r[4]=0u;goto L8c03fe4c; /* mov #0x0,r4 */
L8c03fe4c: *stop=0x8c03fe4cu;r[2]=rd(m,0x8c040044u,4);goto L8c03fe4e; /* mov.l 0x8c040044,r2 */
L8c03fe4e: *stop=0x8c03fe4eu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[5]);goto L8c03fe50; /* fmov fr4,@(r0,r5) */
L8c03fe50: *stop=0x8c03fe50u;if(!valid_ram(r[5]+20u,4u))return -3;wr(m,r[5]+0x14u,r[4],4);goto L8c03fe52; /* mov.l r4,@(0x14,r5) */
L8c03fe52: *stop=0x8c03fe52u;if(!valid_ram(r[5]+24u,4u))return -3;wr(m,r[5]+0x18u,r[4],4);goto L8c03fe54; /* mov.l r4,@(0x18,r5) */
L8c03fe54: *stop=0x8c03fe54u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[6],2);goto L8c03fe56; /* mov.w r6,@r3 */
L8c03fe56: *stop=0x8c03fe56u;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[7],2);goto L8c03fe58; /* mov.w r7,@r2 */
L8c03fe58: *stop=0x8c03fe58u;r[0]=r[4];goto L8c03fe5a; /* mov r4,r0 */
L8c03fe5a: *stop=0x8c03fe5au;goto L8c03fe5c; /* nop */
L8c03fe5c: *stop=0x8c03fe5cu;if(*pr!=entry_pr)return -3;*stop=0x8c03fe5eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c03fe60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c03fe60;
L8c03fe60: *stop=0x8c03fe60u;r[3]=rd(m,0x8c040044u,4);goto L8c03fe62; /* mov.l 0x8c040044,r3 */
L8c03fe62: *stop=0x8c03fe62u;if(!valid_ram(r[3],2u))return -3;r[7]=sx(m,r[3],2);goto L8c03fe64; /* mov.w @r3,r7 */
L8c03fe64: *stop=0x8c03fe64u;r[1]=rd(m,0x8c040040u,4);goto L8c03fe66; /* mov.l 0x8c040040,r1 */
L8c03fe66: *stop=0x8c03fe66u;r[5]=r[7]&65535u;goto L8c03fe68; /* extu.w r7,r5 */
L8c03fe68: *stop=0x8c03fe68u;r[0]=rd(m,0x8c040048u,4);goto L8c03fe6a; /* mov.l 0x8c040048,r0 */
L8c03fe6a: *stop=0x8c03fe6au;if(!valid_ram(r[1],2u))return -3;r[2]=sx(m,r[1],2);goto L8c03fe6c; /* mov.w @r1,r2 */
L8c03fe6c: *stop=0x8c03fe6cu;r[6]=rd(m,0x8c04004cu,4);goto L8c03fe6e; /* mov.l 0x8c04004c,r6 */
L8c03fe6e: *stop=0x8c03fe6eu;r[1]=1u;goto L8c03fe70; /* mov #0x1,r1 */
L8c03fe70: *stop=0x8c03fe70u;r[2]=r[2]&65535u;goto L8c03fe72; /* extu.w r2,r2 */
L8c03fe72: *stop=0x8c03fe72u;r[2]<<=2;goto L8c03fe74; /* shll2 r2 */
L8c03fe74: *stop=0x8c03fe74u;if(!valid_ram(r[0]+r[2],4u))return -3;r[3]=rd(m,r[2]+r[0],4);goto L8c03fe76; /* mov.l @(r0,r2),r3 */
L8c03fe76: *stop=0x8c03fe76u;*sr=(*sr&~1u)|!(r[5]&r[1]);goto L8c03fe78; /* tst r5,r1 */
L8c03fe78: *stop=0x8c03fe78u;r[2]=rd(m,0x8c040050u,4);goto L8c03fe7a; /* mov.l 0x8c040050,r2 */
L8c03fe7a: *stop=0x8c03fe7au;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c03fe7c; /* mov.l r3,@r2 */
L8c03fe7c: if((*sr&1u))goto L8c03fe80;goto L8c03fe7e; /* bt 0x8c03fe80 */
L8c03fe7e: *stop=0x8c03fe7eu;r[6]=rd(m,0x8c040054u,4);goto L8c03fe80; /* mov.l 0x8c040054,r6 */
L8c03fe80: *stop=0x8c03fe80u;r[2]=rd(m,0x8c040058u,4);goto L8c03fe82; /* mov.l 0x8c040058,r2 */
L8c03fe82: *stop=0x8c03fe82u;r[3]=2u;goto L8c03fe84; /* mov #0x2,r3 */
L8c03fe84: *stop=0x8c03fe84u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[6],4);goto L8c03fe86; /* mov.l r6,@r2 */
L8c03fe86: *stop=0x8c03fe86u;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c03fe88; /* tst r5,r3 */
L8c03fe88: *stop=0x8c03fe88u;if(!valid_ram(r[4],4u))return -3;r[0]=rd(m,r[4],4);goto L8c03fe8a; /* mov.l @r4,r0 */
L8c03fe8a: *stop=0x8c03fe8au;r[0]&=0x20u;goto L8c03fe8c; /* and #0x20,r0 */
L8c03fe8c: take=(*sr&1u);*stop=0x8c03fe8eu;r[6]=r[0];if(take)goto L8c03fe9c;goto L8c03fe90; /* bt/s 0x8c03fe9c */
L8c03fe90: *stop=0x8c03fe90u;r[4]=rd(m,0x8c04005cu,4);goto L8c03fe92; /* mov.l 0x8c04005c,r4 */
L8c03fe92: *stop=0x8c03fe92u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c03fe94; /* tst r6,r6 */
L8c03fe94: if((*sr&1u))goto L8c03fea4;goto L8c03fe96; /* bt 0x8c03fea4 */
L8c03fe96: *stop=0x8c03fe96u;r[4]=rd(m,0x8c040060u,4);goto L8c03fe98; /* mov.l 0x8c040060,r4 */
L8c03fe98: *stop=0x8c03fe9au;goto L8c03fea4; /* bra 0x8c03fea4 */
L8c03fe9c: *stop=0x8c03fe9cu;r[4]=rd(m,0x8c040064u,4);goto L8c03fe9e; /* mov.l 0x8c040064,r4 */
L8c03fe9e: *stop=0x8c03fe9eu;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c03fea0; /* tst r6,r6 */
L8c03fea0: if((*sr&1u))goto L8c03fea4;goto L8c03fea2; /* bt 0x8c03fea4 */
L8c03fea2: *stop=0x8c03fea2u;r[4]=rd(m,0x8c040068u,4);goto L8c03fea4; /* mov.l 0x8c040068,r4 */
L8c03fea4: *stop=0x8c03fea4u;r[2]=rd(m,0x8c04006cu,4);goto L8c03fea6; /* mov.l 0x8c04006c,r2 */
L8c03fea6: *stop=0x8c03fea6u;r[3]=4u;goto L8c03fea8; /* mov #0x4,r3 */
L8c03fea8: *stop=0x8c03fea8u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[4],4);goto L8c03feaa; /* mov.l r4,@r2 */
L8c03feaa: *stop=0x8c03feaau;*sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c03feac; /* tst r3,r5 */
L8c03feac: *stop=0x8c03feacu;r[4]=rd(m,0x8c040070u,4);goto L8c03feae; /* mov.l 0x8c040070,r4 */
L8c03feae: if((*sr&1u))goto L8c03feb2;goto L8c03feb0; /* bt 0x8c03feb2 */
L8c03feb0: *stop=0x8c03feb0u;r[4]=rd(m,0x8c040074u,4);goto L8c03feb2; /* mov.l 0x8c040074,r4 */
L8c03feb2: *stop=0x8c03feb2u;r[3]=rd(m,0x8c040078u,4);goto L8c03feb4; /* mov.l 0x8c040078,r3 */
L8c03feb4: *stop=0x8c03feb4u;if(*pr!=entry_pr)return -3;*stop=0x8c03feb6u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[4],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040784(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c040784;
L8c040784: *stop=0x8c040784u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040786; /* mov.l r14,@-r15 */
L8c040786: *stop=0x8c040786u;r[14]=r[4];goto L8c040788; /* mov r4,r14 */
L8c040788: *stop=0x8c040788u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c04078a; /* mov.l r13,@-r15 */
L8c04078a: *stop=0x8c04078au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c04078c; /* mov.l r12,@-r15 */
L8c04078c: *stop=0x8c04078cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c04078e; /* mov.l r11,@-r15 */
L8c04078e: *stop=0x8c04078eu;r[11]=32u;goto L8c040790; /* mov #0x20,r11 */
L8c040790: *stop=0x8c040790u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c040792; /* mov.l r10,@-r15 */
L8c040792: *stop=0x8c040792u;r[10]=64u;goto L8c040794; /* mov #0x40,r10 */
L8c040794: *stop=0x8c040794u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c040796; /* mov.l r9,@-r15 */
L8c040796: *stop=0x8c040796u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c040798; /* mov.l r8,@-r15 */
L8c040798: *stop=0x8c040798u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c04079a; /* sts.l PR,@-r15 */
L8c04079a: *stop=0x8c04079au;r[8]=rd(m,0x8c04085cu,4);goto L8c04079c; /* mov.l 0x8c04085c,r8 */
L8c04079c: *stop=0x8c04079cu;r[9]=rd(m,0x8c040860u,4);goto L8c04079e; /* mov.l 0x8c040860,r9 */
L8c04079e: *stop=0x8c04079eu;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c0407a0; /* mov.l @r14,r13 */
L8c0407a0: target=r[9];*pr=0x8c0407a4u;*stop=0x8c0407a2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0407a4; /* jsr @r9 */
L8c0407a4: *stop=0x8c0407a4u;r[3]=r[13];goto L8c0407a6; /* mov r13,r3 */
L8c0407a6: *stop=0x8c0407a6u;*sr=(*sr&~1u)|!(r[10]&r[3]);goto L8c0407a8; /* tst r10,r3 */
L8c0407a8: if(!(*sr&1u))goto L8c0407b6;goto L8c0407aa; /* bf 0x8c0407b6 */
L8c0407aa: *stop=0x8c0407aau;r[2]=rd(m,0x8c040864u,4);goto L8c0407ac; /* mov.l 0x8c040864,r2 */
L8c0407ac: *stop=0x8c0407acu;if(!valid_ram(r[2],4u))return -3;r[1]=rd(m,r[2],4);goto L8c0407ae; /* mov.l @r2,r1 */
L8c0407ae: target=r[1];*pr=0x8c0407b2u;*stop=0x8c0407b0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0407b2; /* jsr @r1 */
L8c0407b2: *stop=0x8c0407b4u;goto L8c0407ea; /* bra 0x8c0407ea */
L8c0407b6: *stop=0x8c0407b6u;r[4]=r[14];goto L8c0407b8; /* mov r14,r4 */
L8c0407b8: *stop=0x8c0407b8u;r[3]=rd(m,0x8c040868u,4);goto L8c0407ba; /* mov.l 0x8c040868,r3 */
L8c0407ba: *stop=0x8c0407bau;r[4]+=8u;goto L8c0407bc; /* add #0x8,r4 */
L8c0407bc: *stop=0x8c0407bcu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0407be; /* fmov.s @r4+,fr4 */
L8c0407be: *stop=0x8c0407beu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0407c0; /* fmov.s @r4+,fr5 */
L8c0407c0: *stop=0x8c0407c0u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c0407c2; /* fmov.s @r4,fr6 */
L8c0407c2: target=r[3];*pr=0x8c0407c6u;*stop=0x8c0407c4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0407c6; /* jsr @r3 */
L8c0407c6: *stop=0x8c0407c6u;r[2]=r[13];goto L8c0407c8; /* mov r13,r2 */
L8c0407c8: *stop=0x8c0407c8u;*sr=(*sr&~1u)|!(r[11]&r[2]);goto L8c0407ca; /* tst r11,r2 */
L8c0407ca: take=(*sr&1u);*stop=0x8c0407ccu;r[12]=r[8];if(take)goto L8c0407d0;goto L8c0407ce; /* bt/s 0x8c0407d0 */
L8c0407ce: *stop=0x8c0407ceu;r[12]=rd(m,0x8c04086cu,4);goto L8c0407d0; /* mov.l 0x8c04086c,r12 */
L8c0407d0: *stop=0x8c0407d0u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c0407d2; /* mov.l @r15,r5 */
L8c0407d2: *stop=0x8c0407d2u;if(!valid_ram(r[15]+4u,4u))return -3;r[6]=rd(m,r[15]+0x4u,4);goto L8c0407d4; /* mov.l @(0x4,r15),r6 */
L8c0407d4: *stop=0x8c0407d4u;if(!valid_ram(r[15]+8u,4u))return -3;r[7]=rd(m,r[15]+0x8u,4);goto L8c0407d6; /* mov.l @(0x8,r15),r7 */
L8c0407d6: target=r[12];*pr=0x8c0407dau;*stop=0x8c0407d8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0407da; /* jsr @r12 */
L8c0407da: *stop=0x8c0407dau;r[4]=r[14];goto L8c0407dc; /* mov r14,r4 */
L8c0407dc: *stop=0x8c0407dcu;r[3]=rd(m,0x8c040870u,4);goto L8c0407de; /* mov.l 0x8c040870,r3 */
L8c0407de: *stop=0x8c0407deu;r[4]+=32u;goto L8c0407e0; /* add #0x20,r4 */
L8c0407e0: *stop=0x8c0407e0u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0407e2; /* fmov.s @r4+,fr4 */
L8c0407e2: *stop=0x8c0407e2u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0407e4; /* fmov.s @r4+,fr5 */
L8c0407e4: *stop=0x8c0407e4u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c0407e6; /* fmov.s @r4,fr6 */
L8c0407e6: target=r[3];*pr=0x8c0407eau;*stop=0x8c0407e8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0407ea; /* jsr @r3 */
L8c0407ea: *stop=0x8c0407eau;r[2]=rd(m,0x8c040874u,4);goto L8c0407ec; /* mov.l 0x8c040874,r2 */
L8c0407ec: *stop=0x8c0407ecu;if(!valid_ram(r[2],4u))return -3;r[12]=rd(m,r[2],4);goto L8c0407ee; /* mov.l @r2,r12 */
L8c0407ee: *stop=0x8c0407eeu;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c0407f0; /* tst r12,r12 */
L8c0407f0: if((*sr&1u))goto L8c0407f6;goto L8c0407f2; /* bt 0x8c0407f6 */
L8c0407f2: target=r[12];*pr=0x8c0407f6u;*stop=0x8c0407f4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0407f6; /* jsr @r12 */
L8c0407f6: *stop=0x8c0407f6u;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c0407f8; /* mov.l @(0x4,r14),r2 */
L8c0407f8: *stop=0x8c0407f8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0407fa; /* tst r2,r2 */
L8c0407fa: if((*sr&1u))goto L8c040808;goto L8c0407fc; /* bt 0x8c040808 */
L8c0407fc: *stop=0x8c0407fcu;r[3]=8u;goto L8c0407fe; /* mov #0x8,r3 */
L8c0407fe: *stop=0x8c0407feu;*sr=(*sr&~1u)|!(r[3]&r[13]);goto L8c040800; /* tst r3,r13 */
L8c040800: if(!(*sr&1u))goto L8c040808;goto L8c040802; /* bf 0x8c040808 */
L8c040802: *stop=0x8c040802u;r[3]=rd(m,0x8c040854u,4);goto L8c040804; /* mov.l 0x8c040854,r3 */
L8c040804: target=r[3];*pr=0x8c040808u;*stop=0x8c040806u;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040808; /* jsr @r3 */
L8c040808: *stop=0x8c040808u;if(!valid_ram(r[14]+44u,4u))return -3;r[2]=rd(m,r[14]+0x2cu,4);goto L8c04080a; /* mov.l @(0x2c,r14),r2 */
L8c04080a: *stop=0x8c04080au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c04080c; /* tst r2,r2 */
L8c04080c: if((*sr&1u))goto L8c040812;goto L8c04080e; /* bt 0x8c040812 */
L8c04080e: target=0x8c040784u;*pr=0x8c040812u;*stop=0x8c040810u;if(!valid_ram(r[14]+44u,4u))return -3;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040812; /* bsr 0x8c040784 */
L8c040812: *stop=0x8c040812u;r[2]=rd(m,0x8c040878u,4);goto L8c040814; /* mov.l 0x8c040878,r2 */
L8c040814: target=r[2];*pr=0x8c040818u;*stop=0x8c040816u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040818; /* jsr @r2 */
L8c040818: *stop=0x8c040818u;if(!valid_ram(r[14]+48u,4u))return -3;r[14]=rd(m,r[14]+0x30u,4);goto L8c04081a; /* mov.l @(0x30,r14),r14 */
L8c04081a: *stop=0x8c04081au;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c04081c; /* tst r14,r14 */
L8c04081c: if(!(*sr&1u))goto L8c04079e;goto L8c04081e; /* bf 0x8c04079e */
L8c04081e: *stop=0x8c04081eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c040820; /* lds.l @r15+,PR */
L8c040820: *stop=0x8c040820u;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c040822; /* mov.l @r15+,r8 */
L8c040822: *stop=0x8c040822u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c040824; /* mov.l @r15+,r9 */
L8c040824: *stop=0x8c040824u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c040826; /* mov.l @r15+,r10 */
L8c040826: *stop=0x8c040826u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c040828; /* mov.l @r15+,r11 */
L8c040828: *stop=0x8c040828u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c04082a; /* mov.l @r15+,r12 */
L8c04082a: *stop=0x8c04082au;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c04082c; /* mov.l @r15+,r13 */
L8c04082c: *stop=0x8c04082cu;if(*pr!=entry_pr)return -3;*stop=0x8c04082eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040b90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c040b90;
L8c040b90: *stop=0x8c040b90u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c040b92; /* mov.l r12,@-r15 */
L8c040b92: *stop=0x8c040b92u;r[7]=28u;goto L8c040b94; /* mov #0x1c,r7 */
L8c040b94: *stop=0x8c040b94u;r[3]=rd(m,0x8c040d08u,4);goto L8c040b96; /* mov.l 0x8c040d08,r3 */
L8c040b96: *stop=0x8c040b96u;r[6]=r[4];goto L8c040b98; /* mov r4,r6 */
L8c040b98: *stop=0x8c040b98u;*macl=(uint32_t)((int32_t)(int16_t)r[7]*(int32_t)(int16_t)r[5]);goto L8c040b9a; /* muls.w r7,r5 */
L8c040b9a: *stop=0x8c040b9au;r[12]=r[5];goto L8c040b9c; /* mov r5,r12 */
L8c040b9c: *stop=0x8c040b9cu;r[7]=*macl;goto L8c040b9e; /* sts MACL,r7 */
L8c040b9e: *stop=0x8c040b9eu;r[7]=(uint32_t)(int32_t)(int8_t)r[7];goto L8c040ba0; /* exts.b r7,r7 */
L8c040ba0: *stop=0x8c040ba0u;r[7]+=r[3];goto L8c040ba2; /* add r3,r7 */
L8c040ba2: *stop=0x8c040ba2u;r[12]<<=2;goto L8c040ba4; /* shll2 r12 */
L8c040ba4: *stop=0x8c040ba4u;r[6]+=r[12];goto L8c040ba6; /* add r12,r6 */
L8c040ba6: *stop=0x8c040ba6u;r[3]=0u;goto L8c040ba8; /* mov #0x0,r3 */
L8c040ba8: *stop=0x8c040ba8u;if(!valid_ram(r[6],4u))return -3;r[6]=rd(m,r[6],4);goto L8c040baa; /* mov.l @r6,r6 */
L8c040baa: *stop=0x8c040baau;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c040bac; /* tst r6,r6 */
L8c040bac: take=(*sr&1u);*stop=0x8c040baeu;r[1]=r[3];if(take)goto L8c040be8;goto L8c040bb0; /* bt/s 0x8c040be8 */
L8c040bb0: *stop=0x8c040bb0u;if(!valid_ram(r[6]+4u,4u))return -3;r[2]=rd(m,r[6]+0x4u,4);goto L8c040bb2; /* mov.l @(0x4,r6),r2 */
L8c040bb2: *stop=0x8c040bb2u;r[1]=r[4];goto L8c040bb4; /* mov r4,r1 */
L8c040bb4: *stop=0x8c040bb4u;r[1]+=8u;goto L8c040bb6; /* add #0x8,r1 */
L8c040bb6: *stop=0x8c040bb6u;r[1]+=r[12];goto L8c040bb8; /* add r12,r1 */
L8c040bb8: *stop=0x8c040bb8u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto L8c040bba; /* fmov.s @r1,fr2 */
L8c040bba: *stop=0x8c040bbau;*fpul=r[2];goto L8c040bbc; /* lds r2,FPUL */
L8c040bbc: *stop=0x8c040bbcu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c040bbe; /* float FPUL,fr3 */
L8c040bbe: *stop=0x8c040bbeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c040bc0; /* fcmp/gt fr2,fr3 */
L8c040bc0: if((*sr&1u))goto L8c040bc8;goto L8c040bc2; /* bt 0x8c040bc8 */
L8c040bc2: *stop=0x8c040bc2u;r[0]=1u;goto L8c040bc4; /* mov #0x1,r0 */
L8c040bc4: *stop=0x8c040bc4u;if(*pr!=entry_pr)return -3;*stop=0x8c040bc6u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c040bc8: *stop=0x8c040bc8u;r[3]=r[4];goto L8c040bca; /* mov r4,r3 */
L8c040bca: *stop=0x8c040bcau;r[3]+=8u;goto L8c040bcc; /* add #0x8,r3 */
L8c040bcc: *stop=0x8c040bccu;r[12]+=r[3];goto L8c040bce; /* add r3,r12 */
L8c040bce: *stop=0x8c040bceu;r[0]=12u;goto L8c040bd0; /* mov #0xc,r0 */
L8c040bd0: *stop=0x8c040bd0u;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[12]);goto L8c040bd2; /* fmov.s @r12,fr3 */
L8c040bd2: *stop=0x8c040bd2u;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[7]);goto L8c040bd4; /* fmov fr3,@(r0,r7) */
L8c040bd4: *stop=0x8c040bd4u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c040bd6; /* mov.l @r6,r3 */
L8c040bd6: *stop=0x8c040bd6u;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[3],4);goto L8c040bd8; /* mov.l r3,@r7 */
L8c040bd8: *stop=0x8c040bd8u;if(!valid_ram(r[6]+4u,4u))return -3;r[2]=rd(m,r[6]+0x4u,4);goto L8c040bda; /* mov.l @(0x4,r6),r2 */
L8c040bda: *stop=0x8c040bdau;if(!valid_ram(r[7]+16u,4u))return -3;wr(m,r[7]+0x10u,r[2],4);goto L8c040bdc; /* mov.l r2,@(0x10,r7) */
L8c040bdc: *stop=0x8c040bdcu;r[3]=rd(m,0x8c040d0cu,4);goto L8c040bde; /* mov.l 0x8c040d0c,r3 */
L8c040bde: *stop=0x8c040bdeu;if(!valid_ram(r[6]+10u,2u))return -3;r[0]=sx(m,r[6]+0xau,2);goto L8c040be0; /* mov.w @(0xa,r6),r0 */
L8c040be0: *stop=0x8c040be0u;r[1]=r[0];goto L8c040be2; /* mov r0,r1 */
L8c040be2: *stop=0x8c040be2u;r[1]&=r[3];goto L8c040be4; /* and r3,r1 */
L8c040be4: *stop=0x8c040be4u;if(!valid_ram(r[6]+8u,2u))return -3;r[0]=sx(m,r[6]+0x8u,2);goto L8c040be6; /* mov.w @(0x8,r6),r0 */
L8c040be6: *stop=0x8c040be6u;r[3]=r[0];goto L8c040be8; /* mov r0,r3 */
L8c040be8: *stop=0x8c040be8u;r[0]=rd(m,0x8c040d10u,4);goto L8c040bea; /* mov.l 0x8c040d10,r0 */
L8c040bea: *stop=0x8c040beau;r[4]=0u;goto L8c040bec; /* mov #0x0,r4 */
L8c040bec: *stop=0x8c040becu;if(!valid_ram(r[7]+20u,4u))return -3;wr(m,r[7]+0x14u,r[4],4);goto L8c040bee; /* mov.l r4,@(0x14,r7) */
L8c040bee: *stop=0x8c040beeu;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c040bf0; /* shll r5 */
L8c040bf0: *stop=0x8c040bf0u;if(!valid_ram(r[7]+24u,4u))return -3;wr(m,r[7]+0x18u,r[4],4);goto L8c040bf2; /* mov.l r4,@(0x18,r7) */
L8c040bf2: *stop=0x8c040bf2u;r[6]=r[5];goto L8c040bf4; /* mov r5,r6 */
L8c040bf4: *stop=0x8c040bf4u;if(!valid_ram(r[0]+r[5],2u))return -3;wr(m,r[0]+r[5],r[1],2);goto L8c040bf6; /* mov.w r1,@(r0,r5) */
L8c040bf6: *stop=0x8c040bf6u;r[0]=rd(m,0x8c040d14u,4);goto L8c040bf8; /* mov.l 0x8c040d14,r0 */
L8c040bf8: *stop=0x8c040bf8u;if(!valid_ram(r[0]+r[6],2u))return -3;wr(m,r[0]+r[6],r[3],2);goto L8c040bfa; /* mov.w r3,@(r0,r6) */
L8c040bfa: *stop=0x8c040bfau;r[0]=r[4];goto L8c040bfc; /* mov r4,r0 */
L8c040bfc: *stop=0x8c040bfcu;goto L8c040bfe; /* nop */
L8c040bfe: *stop=0x8c040bfeu;if(*pr!=entry_pr)return -3;*stop=0x8c040c00u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040c02(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040c02;
L8c040c02: *stop=0x8c040c02u;r[6]=r[5];goto L8c040c04; /* mov r5,r6 */
L8c040c04: *stop=0x8c040c04u;r[1]=rd(m,0x8c040d18u,4);goto L8c040c06; /* mov.l 0x8c040d18,r1 */
L8c040c06: *stop=0x8c040c06u;r[7]=r[5];goto L8c040c08; /* mov r5,r7 */
L8c040c08: *stop=0x8c040c08u;r[0]=rd(m,0x8c040d10u,4);goto L8c040c0a; /* mov.l 0x8c040d10,r0 */
L8c040c0a: *stop=0x8c040c0au;r[6]<<=2;goto L8c040c0c; /* shll2 r6 */
L8c040c0c: *stop=0x8c040c0cu;r[5]=rd(m,0x8c040d1cu,4);goto L8c040c0e; /* mov.l 0x8c040d1c,r5 */
L8c040c0e: *stop=0x8c040c0eu;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c040c10; /* shll r7 */
L8c040c10: *stop=0x8c040c10u;if(!valid_ram(r[0]+r[7],2u))return -3;r[3]=sx(m,r[0]+r[7],2);goto L8c040c12; /* mov.w @(r0,r7),r3 */
L8c040c12: *stop=0x8c040c12u;r[1]+=r[6];goto L8c040c14; /* add r6,r1 */
L8c040c14: *stop=0x8c040c14u;r[0]=rd(m,0x8c040d20u,4);goto L8c040c16; /* mov.l 0x8c040d20,r0 */
L8c040c16: *stop=0x8c040c16u;r[3]=r[3]&65535u;goto L8c040c18; /* extu.w r3,r3 */
L8c040c18: *stop=0x8c040c18u;r[3]<<=2;goto L8c040c1a; /* shll2 r3 */
L8c040c1a: *stop=0x8c040c1au;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c040c1c; /* mov.l @(r0,r3),r2 */
L8c040c1c: *stop=0x8c040c1cu;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[2],4);goto L8c040c1e; /* mov.l r2,@r1 */
L8c040c1e: *stop=0x8c040c1eu;if(!valid_ram(r[4],4u))return -3;r[0]=rd(m,r[4],4);goto L8c040c20; /* mov.l @r4,r0 */
L8c040c20: *stop=0x8c040c20u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c040c22; /* tst #0x20,r0 */
L8c040c22: if((*sr&1u))goto L8c040c26;goto L8c040c24; /* bt 0x8c040c26 */
L8c040c24: *stop=0x8c040c24u;r[5]=rd(m,0x8c040d24u,4);goto L8c040c26; /* mov.l 0x8c040d24,r5 */
L8c040c26: *stop=0x8c040c26u;r[3]=1u;goto L8c040c28; /* mov #0x1,r3 */
L8c040c28: *stop=0x8c040c28u;r[2]=rd(m,0x8c040d28u,4);goto L8c040c2a; /* mov.l 0x8c040d28,r2 */
L8c040c2a: *stop=0x8c040c2au;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[5],4);goto L8c040c2c; /* mov.l r5,@r2 */
L8c040c2c: *stop=0x8c040c2cu;r[0]=rd(m,0x8c040d14u,4);goto L8c040c2e; /* mov.l 0x8c040d14,r0 */
L8c040c2e: *stop=0x8c040c2eu;if(!valid_ram(r[0]+r[7],2u))return -3;r[4]=sx(m,r[0]+r[7],2);goto L8c040c30; /* mov.w @(r0,r7),r4 */
L8c040c30: *stop=0x8c040c30u;r[5]=rd(m,0x8c040d2cu,4);goto L8c040c32; /* mov.l 0x8c040d2c,r5 */
L8c040c32: *stop=0x8c040c32u;r[4]=r[4]&65535u;goto L8c040c34; /* extu.w r4,r4 */
L8c040c34: *stop=0x8c040c34u;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c040c36; /* tst r4,r3 */
L8c040c36: if((*sr&1u))goto L8c040c3a;goto L8c040c38; /* bt 0x8c040c3a */
L8c040c38: *stop=0x8c040c38u;r[5]=rd(m,0x8c040d30u,4);goto L8c040c3a; /* mov.l 0x8c040d30,r5 */
L8c040c3a: *stop=0x8c040c3au;r[3]=2u;goto L8c040c3c; /* mov #0x2,r3 */
L8c040c3c: *stop=0x8c040c3cu;r[0]=rd(m,0x8c040d34u,4);goto L8c040c3e; /* mov.l 0x8c040d34,r0 */
L8c040c3e: *stop=0x8c040c3eu;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c040c40; /* tst r4,r3 */
L8c040c40: *stop=0x8c040c40u;if(!valid_ram(r[0]+r[6],4u))return -3;wr(m,r[6]+r[0],r[5],4);goto L8c040c42; /* mov.l r5,@(r0,r6) */
L8c040c42: *stop=0x8c040c42u;r[5]=rd(m,0x8c040d38u,4);goto L8c040c44; /* mov.l 0x8c040d38,r5 */
L8c040c44: if((*sr&1u))goto L8c040c48;goto L8c040c46; /* bt 0x8c040c48 */
L8c040c46: *stop=0x8c040c46u;r[5]=rd(m,0x8c040d3cu,4);goto L8c040c48; /* mov.l 0x8c040d3c,r5 */
L8c040c48: *stop=0x8c040c48u;r[0]=rd(m,0x8c040d40u,4);goto L8c040c4a; /* mov.l 0x8c040d40,r0 */
L8c040c4a: *stop=0x8c040c4au;r[3]=4u;goto L8c040c4c; /* mov #0x4,r3 */
L8c040c4c: *stop=0x8c040c4cu;if(!valid_ram(r[0]+r[6],4u))return -3;wr(m,r[6]+r[0],r[5],4);goto L8c040c4e; /* mov.l r5,@(r0,r6) */
L8c040c4e: *stop=0x8c040c4eu;*sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c040c50; /* tst r3,r4 */
L8c040c50: *stop=0x8c040c50u;r[5]=rd(m,0x8c040d44u,4);goto L8c040c52; /* mov.l 0x8c040d44,r5 */
L8c040c52: if((*sr&1u))goto L8c040c56;goto L8c040c54; /* bt 0x8c040c56 */
L8c040c54: *stop=0x8c040c54u;r[5]=rd(m,0x8c040d48u,4);goto L8c040c56; /* mov.l 0x8c040d48,r5 */
L8c040c56: *stop=0x8c040c56u;r[0]=rd(m,0x8c040d4cu,4);goto L8c040c58; /* mov.l 0x8c040d4c,r0 */
L8c040c58: *stop=0x8c040c58u;if(*pr!=entry_pr)return -3;*stop=0x8c040c5au;if(!valid_ram(r[0]+r[6],4u))return -3;wr(m,r[6]+r[0],r[5],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c041a2e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c041a2e;
L8c041a2e: *stop=0x8c041a2eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c041a30; /* mov.l r14,@-r15 */
L8c041a30: *stop=0x8c041a30u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c041a32; /* mov.l r13,@-r15 */
L8c041a32: *stop=0x8c041a32u;r[14]=r[4];goto L8c041a34; /* mov r4,r14 */
L8c041a34: *stop=0x8c041a34u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c041a36; /* mov.l r12,@-r15 */
L8c041a36: *stop=0x8c041a36u;r[13]=0u;goto L8c041a38; /* mov #0x0,r13 */
L8c041a38: *stop=0x8c041a38u;r[4]=rd(m,0x8c041bb0u,4);goto L8c041a3a; /* mov.l 0x8c041bb0,r4 */
L8c041a3a: *stop=0x8c041a3au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c041a3c; /* mov.l r11,@-r15 */
L8c041a3c: *stop=0x8c041a3cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c041a3e; /* mov.l r10,@-r15 */
L8c041a3e: *stop=0x8c041a3eu;r[3]=r[4];goto L8c041a40; /* mov r4,r3 */
L8c041a40: *stop=0x8c041a40u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c041a42; /* mov.l r9,@-r15 */
L8c041a42: *stop=0x8c041a42u;r[4]+=4u;goto L8c041a44; /* add #0x4,r4 */
L8c041a44: *stop=0x8c041a44u;r[10]=rd(m,0x8c041bb4u,4);goto L8c041a46; /* mov.l 0x8c041bb4,r10 */
L8c041a46: *stop=0x8c041a46u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c041a48; /* mov.l r8,@-r15 */
L8c041a48: *stop=0x8c041a48u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c041a4a; /* sts.l PR,@-r15 */
L8c041a4a: *stop=0x8c041a4au;r[8]=64u;goto L8c041a4c; /* mov #0x40,r8 */
L8c041a4c: *stop=0x8c041a4cu;r[15]+=4294967288u;goto L8c041a4e; /* add #-0x8,r15 */
L8c041a4e: *stop=0x8c041a4eu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c041a50; /* mov.l r3,@r15 */
L8c041a50: *stop=0x8c041a50u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[4],4);goto L8c041a52; /* mov.l r4,@(0x4,r15) */
L8c041a52: *stop=0x8c041a52u;if(!valid_ram(r[14],4u))return -3;r[12]=rd(m,r[14],4);goto L8c041a54; /* mov.l @r14,r12 */
L8c041a54: *stop=0x8c041a54u;r[3]=rd(m,0x8c041bb8u,4);goto L8c041a56; /* mov.l 0x8c041bb8,r3 */
L8c041a56: target=r[3];*pr=0x8c041a5au;*stop=0x8c041a58u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041a5a; /* jsr @r3 */
L8c041a5a: *stop=0x8c041a5au;r[2]=r[12];goto L8c041a5c; /* mov r12,r2 */
L8c041a5c: *stop=0x8c041a5cu;*sr=(*sr&~1u)|!(r[8]&r[2]);goto L8c041a5e; /* tst r8,r2 */
L8c041a5e: if(!(*sr&1u))goto L8c041a8a;goto L8c041a60; /* bf 0x8c041a8a */
L8c041a60: *stop=0x8c041a60u;r[1]=rd(m,0x8c041bbcu,4);goto L8c041a62; /* mov.l 0x8c041bbc,r1 */
L8c041a62: *stop=0x8c041a62u;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[13],2);goto L8c041a64; /* mov.w r13,@r1 */
L8c041a64: *stop=0x8c041a64u;r[3]=rd(m,0x8c041bc0u,4);goto L8c041a66; /* mov.l 0x8c041bc0,r3 */
L8c041a66: *stop=0x8c041a66u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[13],2);goto L8c041a68; /* mov.w r13,@r3 */
L8c041a68: *stop=0x8c041a68u;r[2]=rd(m,0x8c041bc4u,4);goto L8c041a6a; /* mov.l 0x8c041bc4,r2 */
L8c041a6a: *stop=0x8c041a6au;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[13],2);goto L8c041a6c; /* mov.w r13,@r2 */
L8c041a6c: *stop=0x8c041a6cu;if(!valid_ram(r[10],2u))return -3;wr(m,r[10],r[13],2);goto L8c041a6e; /* mov.w r13,@r10 */
L8c041a6e: *stop=0x8c041a6eu;if(!valid_ram(r[15],4u))return -3;r[1]=rd(m,r[15],4);goto L8c041a70; /* mov.l @r15,r1 */
L8c041a70: *stop=0x8c041a70u;if(!valid_ram(r[1],4u))return -3;r[1]=rd(m,r[1],4);goto L8c041a72; /* mov.l @r1,r1 */
L8c041a72: target=r[1];*pr=0x8c041a76u;*stop=0x8c041a74u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041a76; /* jsr @r1 */
L8c041a76: *stop=0x8c041a76u;r[3]=1u;goto L8c041a78; /* mov #0x1,r3 */
L8c041a78: *stop=0x8c041a78u;if(!valid_ram(r[10],2u))return -3;wr(m,r[10],r[3],2);goto L8c041a7a; /* mov.w r3,@r10 */
L8c041a7a: *stop=0x8c041a7au;if(!valid_ram(r[15]+4u,4u))return -3;r[2]=rd(m,r[15]+0x4u,4);goto L8c041a7c; /* mov.l @(0x4,r15),r2 */
L8c041a7c: *stop=0x8c041a7cu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c041a7e; /* mov.l @r2,r3 */
L8c041a7e: target=r[3];*pr=0x8c041a82u;*stop=0x8c041a80u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041a82; /* jsr @r3 */
L8c041a82: target=0x8c0417c8u;*pr=0x8c041a86u;*stop=0x8c041a84u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041a86; /* bsr 0x8c0417c8 */
L8c041a86: *stop=0x8c041a88u;goto L8c041ac6; /* bra 0x8c041ac6 */
L8c041a8a: *stop=0x8c041a8au;r[4]=r[14];goto L8c041a8c; /* mov r14,r4 */
L8c041a8c: *stop=0x8c041a8cu;r[3]=rd(m,0x8c041b88u,4);goto L8c041a8e; /* mov.l 0x8c041b88,r3 */
L8c041a8e: *stop=0x8c041a8eu;r[4]+=8u;goto L8c041a90; /* add #0x8,r4 */
L8c041a90: *stop=0x8c041a90u;r[2]=rd(m,0x8c041bc8u,4);goto L8c041a92; /* mov.l 0x8c041bc8,r2 */
L8c041a92: *stop=0x8c041a92u;if(!valid_ram(r[2],4u))return -3;r[11]=rd(m,r[2],4);goto L8c041a94; /* mov.l @r2,r11 */
L8c041a94: *stop=0x8c041a94u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c041a96; /* fmov.s @r4+,fr4 */
L8c041a96: *stop=0x8c041a96u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c041a98; /* fmov.s @r4+,fr5 */
L8c041a98: *stop=0x8c041a98u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c041a9a; /* fmov.s @r4,fr6 */
L8c041a9a: target=r[3];*pr=0x8c041a9eu;*stop=0x8c041a9cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041a9e; /* jsr @r3 */
L8c041a9e: *stop=0x8c041a9eu;r[2]=32u;goto L8c041aa0; /* mov #0x20,r2 */
L8c041aa0: *stop=0x8c041aa0u;r[9]=rd(m,0x8c041bccu,4);goto L8c041aa2; /* mov.l 0x8c041bcc,r9 */
L8c041aa2: *stop=0x8c041aa2u;*sr=(*sr&~1u)|!(r[12]&r[2]);goto L8c041aa4; /* tst r12,r2 */
L8c041aa4: take=(*sr&1u);*stop=0x8c041aa6u;r[4]=r[14];if(take)goto L8c041aaa;goto L8c041aa8; /* bt/s 0x8c041aaa */
L8c041aa8: *stop=0x8c041aa8u;r[9]=rd(m,0x8c041bd0u,4);goto L8c041aaa; /* mov.l 0x8c041bd0,r9 */
L8c041aaa: *stop=0x8c041aaau;r[4]+=20u;goto L8c041aac; /* add #0x14,r4 */
L8c041aac: *stop=0x8c041aacu;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c041aae; /* mov.l @r4,r5 */
L8c041aae: *stop=0x8c041aaeu;if(!valid_ram(r[4]+4u,4u))return -3;r[6]=rd(m,r[4]+0x4u,4);goto L8c041ab0; /* mov.l @(0x4,r4),r6 */
L8c041ab0: *stop=0x8c041ab0u;if(!valid_ram(r[4]+8u,4u))return -3;r[7]=rd(m,r[4]+0x8u,4);goto L8c041ab2; /* mov.l @(0x8,r4),r7 */
L8c041ab2: target=r[9];*pr=0x8c041ab6u;*stop=0x8c041ab4u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041ab6; /* jsr @r9 */
L8c041ab6: *stop=0x8c041ab6u;r[4]=r[14];goto L8c041ab8; /* mov r14,r4 */
L8c041ab8: *stop=0x8c041ab8u;r[3]=rd(m,0x8c041b90u,4);goto L8c041aba; /* mov.l 0x8c041b90,r3 */
L8c041aba: *stop=0x8c041abau;r[4]+=32u;goto L8c041abc; /* add #0x20,r4 */
L8c041abc: *stop=0x8c041abcu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c041abe; /* fmov.s @r4+,fr4 */
L8c041abe: *stop=0x8c041abeu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c041ac0; /* fmov.s @r4+,fr5 */
L8c041ac0: *stop=0x8c041ac0u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c041ac2; /* fmov.s @r4,fr6 */
L8c041ac2: target=r[3];*pr=0x8c041ac6u;*stop=0x8c041ac4u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041ac6; /* jsr @r3 */
L8c041ac6: *stop=0x8c041ac6u;r[2]=rd(m,0x8c041bd4u,4);goto L8c041ac8; /* mov.l 0x8c041bd4,r2 */
L8c041ac8: *stop=0x8c041ac8u;if(!valid_ram(r[2],4u))return -3;r[11]=rd(m,r[2],4);goto L8c041aca; /* mov.l @r2,r11 */
L8c041aca: *stop=0x8c041acau;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c041acc; /* tst r11,r11 */
L8c041acc: if((*sr&1u))goto L8c041ad2;goto L8c041ace; /* bt 0x8c041ad2 */
L8c041ace: target=r[11];*pr=0x8c041ad2u;*stop=0x8c041ad0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041ad2; /* jsr @r11 */
L8c041ad2: *stop=0x8c041ad2u;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c041ad4; /* mov.l @(0x4,r14),r2 */
L8c041ad4: *stop=0x8c041ad4u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c041ad6; /* tst r2,r2 */
L8c041ad6: if((*sr&1u))goto L8c041ae4;goto L8c041ad8; /* bt 0x8c041ae4 */
L8c041ad8: *stop=0x8c041ad8u;r[3]=8u;goto L8c041ada; /* mov #0x8,r3 */
L8c041ada: *stop=0x8c041adau;*sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c041adc; /* tst r3,r12 */
L8c041adc: if(!(*sr&1u))goto L8c041ae4;goto L8c041ade; /* bf 0x8c041ae4 */
L8c041ade: *stop=0x8c041adeu;r[3]=rd(m,0x8c041bacu,4);goto L8c041ae0; /* mov.l 0x8c041bac,r3 */
L8c041ae0: target=r[3];*pr=0x8c041ae4u;*stop=0x8c041ae2u;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041ae4; /* jsr @r3 */
L8c041ae4: *stop=0x8c041ae4u;if(!valid_ram(r[14]+44u,4u))return -3;r[2]=rd(m,r[14]+0x2cu,4);goto L8c041ae6; /* mov.l @(0x2c,r14),r2 */
L8c041ae6: *stop=0x8c041ae6u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c041ae8; /* tst r2,r2 */
L8c041ae8: if((*sr&1u))goto L8c041aee;goto L8c041aea; /* bt 0x8c041aee */
L8c041aea: target=0x8c041a2eu;*pr=0x8c041aeeu;*stop=0x8c041aecu;if(!valid_ram(r[14]+44u,4u))return -3;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041aee; /* bsr 0x8c041a2e */
L8c041aee: *stop=0x8c041aeeu;r[2]=rd(m,0x8c041bd8u,4);goto L8c041af0; /* mov.l 0x8c041bd8,r2 */
L8c041af0: target=r[2];*pr=0x8c041af4u;*stop=0x8c041af2u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041af4; /* jsr @r2 */
L8c041af4: *stop=0x8c041af4u;if(!valid_ram(r[14]+48u,4u))return -3;r[14]=rd(m,r[14]+0x30u,4);goto L8c041af6; /* mov.l @(0x30,r14),r14 */
L8c041af6: *stop=0x8c041af6u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c041af8; /* tst r14,r14 */
L8c041af8: if(!(*sr&1u))goto L8c041a52;goto L8c041afa; /* bf 0x8c041a52 */
L8c041afa: *stop=0x8c041afau;r[15]+=8u;goto L8c041afc; /* add #0x8,r15 */
L8c041afc: *stop=0x8c041afcu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c041afe; /* lds.l @r15+,PR */
L8c041afe: *stop=0x8c041afeu;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c041b00; /* mov.l @r15+,r8 */
L8c041b00: *stop=0x8c041b00u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c041b02; /* mov.l @r15+,r9 */
L8c041b02: *stop=0x8c041b02u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c041b04; /* mov.l @r15+,r10 */
L8c041b04: *stop=0x8c041b04u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c041b06; /* mov.l @r15+,r11 */
L8c041b06: *stop=0x8c041b06u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c041b08; /* mov.l @r15+,r12 */
L8c041b08: *stop=0x8c041b08u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c041b0a; /* mov.l @r15+,r13 */
L8c041b0a: *stop=0x8c041b0au;if(*pr!=entry_pr)return -3;*stop=0x8c041b0cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c5fdbec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5fdbec;
L8c5fdbec: *stop=0x8c5fdbecu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c5fdbee; /* mov.l r14,@-r15 */
L8c5fdbee: *stop=0x8c5fdbeeu;r[14]=r[4];goto L8c5fdbf0; /* mov r4,r14 */
L8c5fdbf0: *stop=0x8c5fdbf0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c5fdbf2; /* mov.l r13,@-r15 */
L8c5fdbf2: *stop=0x8c5fdbf2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c5fdbf4; /* sts.l PR,@-r15 */
L8c5fdbf4: *stop=0x8c5fdbf4u;r[3]=rd(m,0x8c5fddd0u,4);goto L8c5fdbf6; /* mov.l 0x8c5fddd0,r3 */
L8c5fdbf6: target=r[3];*pr=0x8c5fdbfau;*stop=0x8c5fdbf8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdbfa; /* jsr @r3 */
L8c5fdbfa: *stop=0x8c5fdbfau;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c5fdbfc; /* mov.l @r14,r0 */
L8c5fdbfc: *stop=0x8c5fdbfcu;*sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c5fdbfe; /* tst #0x40,r0 */
L8c5fdbfe: if(!(*sr&1u))goto L8c5fdc32;goto L8c5fdc00; /* bf 0x8c5fdc32 */
L8c5fdc00: *stop=0x8c5fdc00u;r[2]=rd(m,0x8c5fddd8u,4);goto L8c5fdc02; /* mov.l 0x8c5fddd8,r2 */
L8c5fdc02: *stop=0x8c5fdc02u;r[4]=0u;goto L8c5fdc04; /* mov #0x0,r4 */
L8c5fdc04: *stop=0x8c5fdc04u;r[3]=rd(m,0x8c5fddd4u,4);goto L8c5fdc06; /* mov.l 0x8c5fddd4,r3 */
L8c5fdc06: *stop=0x8c5fdc06u;r[1]=rd(m,0x8c5fdddcu,4);goto L8c5fdc08; /* mov.l 0x8c5fdddc,r1 */
L8c5fdc08: *stop=0x8c5fdc08u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[4],2);goto L8c5fdc0a; /* mov.w r4,@r3 */
L8c5fdc0a: *stop=0x8c5fdc0au;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[4],2);goto L8c5fdc0c; /* mov.w r4,@r2 */
L8c5fdc0c: *stop=0x8c5fdc0cu;r[2]=rd(m,0x8c5fdde4u,4);goto L8c5fdc0e; /* mov.l 0x8c5fdde4,r2 */
L8c5fdc0e: *stop=0x8c5fdc0eu;r[3]=rd(m,0x8c5fdde0u,4);goto L8c5fdc10; /* mov.l 0x8c5fdde0,r3 */
L8c5fdc10: *stop=0x8c5fdc10u;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[4],2);goto L8c5fdc12; /* mov.w r4,@r1 */
L8c5fdc12: *stop=0x8c5fdc12u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[4],2);goto L8c5fdc14; /* mov.w r4,@r3 */
L8c5fdc14: *stop=0x8c5fdc14u;if(!valid_ram(r[2],4u))return -3;r[1]=rd(m,r[2],4);goto L8c5fdc16; /* mov.l @r2,r1 */
L8c5fdc16: target=r[1];*pr=0x8c5fdc1au;*stop=0x8c5fdc18u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdc1a; /* jsr @r1 */
L8c5fdc1a: *stop=0x8c5fdc1au;r[2]=rd(m,0x8c5fdde0u,4);goto L8c5fdc1c; /* mov.l 0x8c5fdde0,r2 */
L8c5fdc1c: *stop=0x8c5fdc1cu;r[3]=1u;goto L8c5fdc1e; /* mov #0x1,r3 */
L8c5fdc1e: *stop=0x8c5fdc1eu;r[1]=rd(m,0x8c5fdde8u,4);goto L8c5fdc20; /* mov.l 0x8c5fdde8,r1 */
L8c5fdc20: *stop=0x8c5fdc20u;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[3],2);goto L8c5fdc22; /* mov.w r3,@r2 */
L8c5fdc22: *stop=0x8c5fdc22u;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c5fdc24; /* mov.l @r1,r3 */
L8c5fdc24: target=r[3];*pr=0x8c5fdc28u;*stop=0x8c5fdc26u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdc28; /* jsr @r3 */
L8c5fdc28: *stop=0x8c5fdc28u;r[2]=rd(m,0x8c5fddecu,4);goto L8c5fdc2a; /* mov.l 0x8c5fddec,r2 */
L8c5fdc2a: target=r[2];*pr=0x8c5fdc2eu;*stop=0x8c5fdc2cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdc2e; /* jsr @r2 */
L8c5fdc2e: *stop=0x8c5fdc30u;goto L8c5fdc84; /* bra 0x8c5fdc84 */
L8c5fdc32: *stop=0x8c5fdc32u;r[13]=r[14];goto L8c5fdc34; /* mov r14,r13 */
L8c5fdc34: *stop=0x8c5fdc34u;r[0]=8u;goto L8c5fdc36; /* mov #0x8,r0 */
L8c5fdc36: *stop=0x8c5fdc36u;r[13]+=8u;goto L8c5fdc38; /* add #0x8,r13 */
L8c5fdc38: *stop=0x8c5fdc38u;r[3]=rd(m,0x8c5fddf0u,4);goto L8c5fdc3a; /* mov.l 0x8c5fddf0,r3 */
L8c5fdc3a: *stop=0x8c5fdc3au;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[13]);goto L8c5fdc3c; /* fmov.s @(r0,r13),fr6 */
L8c5fdc3c: *stop=0x8c5fdc3cu;r[0]=4u;goto L8c5fdc3e; /* mov #0x4,r0 */
L8c5fdc3e: *stop=0x8c5fdc3eu;if(!valid_ram(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[13]);goto L8c5fdc40; /* fmov.s @r13,fr4 */
L8c5fdc40: *stop=0x8c5fdc40u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[13]);goto L8c5fdc42; /* fmov.s @(r0,r13),fr5 */
L8c5fdc42: target=r[3];*pr=0x8c5fdc46u;*stop=0x8c5fdc44u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdc46; /* jsr @r3 */
L8c5fdc46: *stop=0x8c5fdc46u;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c5fdc48; /* mov.l @r14,r0 */
L8c5fdc48: *stop=0x8c5fdc48u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c5fdc4a; /* tst #0x20,r0 */
L8c5fdc4a: if((*sr&1u))goto L8c5fdc60;goto L8c5fdc4c; /* bt 0x8c5fdc60 */
L8c5fdc4c: *stop=0x8c5fdc4cu;r[13]=r[14];goto L8c5fdc4e; /* mov r14,r13 */
L8c5fdc4e: *stop=0x8c5fdc4eu;r[13]+=20u;goto L8c5fdc50; /* add #0x14,r13 */
L8c5fdc50: *stop=0x8c5fdc50u;if(!valid_ram(r[13],4u))return -3;r[5]=rd(m,r[13],4);goto L8c5fdc52; /* mov.l @r13,r5 */
L8c5fdc52: *stop=0x8c5fdc52u;r[3]=rd(m,0x8c5fddf4u,4);goto L8c5fdc54; /* mov.l 0x8c5fddf4,r3 */
L8c5fdc54: *stop=0x8c5fdc54u;if(!valid_ram(r[13]+8u,4u))return -3;r[7]=rd(m,r[13]+0x8u,4);goto L8c5fdc56; /* mov.l @(0x8,r13),r7 */
L8c5fdc56: *stop=0x8c5fdc56u;if(!valid_ram(r[13]+4u,4u))return -3;r[6]=rd(m,r[13]+0x4u,4);goto L8c5fdc58; /* mov.l @(0x4,r13),r6 */
L8c5fdc58: target=r[3];*pr=0x8c5fdc5cu;*stop=0x8c5fdc5au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdc5c; /* jsr @r3 */
L8c5fdc5c: *stop=0x8c5fdc5eu;goto L8c5fdc70; /* bra 0x8c5fdc70 */
L8c5fdc60: *stop=0x8c5fdc60u;r[13]=r[14];goto L8c5fdc62; /* mov r14,r13 */
L8c5fdc62: *stop=0x8c5fdc62u;r[13]+=20u;goto L8c5fdc64; /* add #0x14,r13 */
L8c5fdc64: *stop=0x8c5fdc64u;if(!valid_ram(r[13],4u))return -3;r[5]=rd(m,r[13],4);goto L8c5fdc66; /* mov.l @r13,r5 */
L8c5fdc66: *stop=0x8c5fdc66u;r[3]=rd(m,0x8c5fddf8u,4);goto L8c5fdc68; /* mov.l 0x8c5fddf8,r3 */
L8c5fdc68: *stop=0x8c5fdc68u;if(!valid_ram(r[13]+8u,4u))return -3;r[7]=rd(m,r[13]+0x8u,4);goto L8c5fdc6a; /* mov.l @(0x8,r13),r7 */
L8c5fdc6a: *stop=0x8c5fdc6au;if(!valid_ram(r[13]+4u,4u))return -3;r[6]=rd(m,r[13]+0x4u,4);goto L8c5fdc6c; /* mov.l @(0x4,r13),r6 */
L8c5fdc6c: target=r[3];*pr=0x8c5fdc70u;*stop=0x8c5fdc6eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdc70; /* jsr @r3 */
L8c5fdc70: *stop=0x8c5fdc70u;r[13]=r[14];goto L8c5fdc72; /* mov r14,r13 */
L8c5fdc72: *stop=0x8c5fdc72u;r[0]=8u;goto L8c5fdc74; /* mov #0x8,r0 */
L8c5fdc74: *stop=0x8c5fdc74u;r[13]+=32u;goto L8c5fdc76; /* add #0x20,r13 */
L8c5fdc76: *stop=0x8c5fdc76u;r[3]=rd(m,0x8c5fddfcu,4);goto L8c5fdc78; /* mov.l 0x8c5fddfc,r3 */
L8c5fdc78: *stop=0x8c5fdc78u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[13]);goto L8c5fdc7a; /* fmov.s @(r0,r13),fr6 */
L8c5fdc7a: *stop=0x8c5fdc7au;r[0]=4u;goto L8c5fdc7c; /* mov #0x4,r0 */
L8c5fdc7c: *stop=0x8c5fdc7cu;if(!valid_ram(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[13]);goto L8c5fdc7e; /* fmov.s @r13,fr4 */
L8c5fdc7e: *stop=0x8c5fdc7eu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[13]);goto L8c5fdc80; /* fmov.s @(r0,r13),fr5 */
L8c5fdc80: target=r[3];*pr=0x8c5fdc84u;*stop=0x8c5fdc82u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdc84; /* jsr @r3 */
L8c5fdc84: *stop=0x8c5fdc84u;r[2]=rd(m,0x8c5fde00u,4);goto L8c5fdc86; /* mov.l 0x8c5fde00,r2 */
L8c5fdc86: *stop=0x8c5fdc86u;if(!valid_ram(r[2],4u))return -3;r[13]=rd(m,r[2],4);goto L8c5fdc88; /* mov.l @r2,r13 */
L8c5fdc88: *stop=0x8c5fdc88u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c5fdc8a; /* tst r13,r13 */
L8c5fdc8a: if((*sr&1u))goto L8c5fdc90;goto L8c5fdc8c; /* bt 0x8c5fdc90 */
L8c5fdc8c: target=r[13];*pr=0x8c5fdc90u;*stop=0x8c5fdc8eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdc90; /* jsr @r13 */
L8c5fdc90: *stop=0x8c5fdc90u;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c5fdc92; /* mov.l @(0x4,r14),r2 */
L8c5fdc92: *stop=0x8c5fdc92u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c5fdc94; /* tst r2,r2 */
L8c5fdc94: if((*sr&1u))goto L8c5fdca2;goto L8c5fdc96; /* bt 0x8c5fdca2 */
L8c5fdc96: *stop=0x8c5fdc96u;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c5fdc98; /* mov.l @r14,r0 */
L8c5fdc98: *stop=0x8c5fdc98u;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c5fdc9a; /* tst #0x8,r0 */
L8c5fdc9a: if(!(*sr&1u))goto L8c5fdca2;goto L8c5fdc9c; /* bf 0x8c5fdca2 */
L8c5fdc9c: *stop=0x8c5fdc9cu;r[2]=rd(m,0x8c5fde04u,4);goto L8c5fdc9e; /* mov.l 0x8c5fde04,r2 */
L8c5fdc9e: target=r[2];*pr=0x8c5fdca2u;*stop=0x8c5fdca0u;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdca2; /* jsr @r2 */
L8c5fdca2: *stop=0x8c5fdca2u;if(!valid_ram(r[14]+44u,4u))return -3;r[3]=rd(m,r[14]+0x2cu,4);goto L8c5fdca4; /* mov.l @(0x2c,r14),r3 */
L8c5fdca4: *stop=0x8c5fdca4u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c5fdca6; /* tst r3,r3 */
L8c5fdca6: if((*sr&1u))goto L8c5fdcac;goto L8c5fdca8; /* bt 0x8c5fdcac */
L8c5fdca8: target=0x8c5fdbecu;*pr=0x8c5fdcacu;*stop=0x8c5fdcaau;if(!valid_ram(r[14]+44u,4u))return -3;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdcac; /* bsr 0x8c5fdbec */
L8c5fdcac: *stop=0x8c5fdcacu;r[2]=rd(m,0x8c5fde08u,4);goto L8c5fdcae; /* mov.l 0x8c5fde08,r2 */
L8c5fdcae: target=r[2];*pr=0x8c5fdcb2u;*stop=0x8c5fdcb0u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fdcb2; /* jsr @r2 */
L8c5fdcb2: *stop=0x8c5fdcb2u;if(!valid_ram(r[14]+48u,4u))return -3;r[3]=rd(m,r[14]+0x30u,4);goto L8c5fdcb4; /* mov.l @(0x30,r14),r3 */
L8c5fdcb4: *stop=0x8c5fdcb4u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c5fdcb6; /* tst r3,r3 */
L8c5fdcb6: if((*sr&1u))goto L8c5fdcbc;goto L8c5fdcb8; /* bt 0x8c5fdcbc */
L8c5fdcb8: *stop=0x8c5fdcbau;if(!valid_ram(r[14]+48u,4u))return -3;r[14]=rd(m,r[14]+0x30u,4);goto L8c5fdbf4; /* bra 0x8c5fdbf4 */
L8c5fdcbc: *stop=0x8c5fdcbcu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c5fdcbe; /* lds.l @r15+,PR */
L8c5fdcbe: *stop=0x8c5fdcbeu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c5fdcc0; /* mov.l @r15+,r13 */
L8c5fdcc0: *stop=0x8c5fdcc0u;if(*pr!=entry_pr)return -3;*stop=0x8c5fdcc2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0409c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0409c0;
L8c0409c0: *stop=0x8c0409c0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c0409c2; /* sts.l PR,@-r15 */
L8c0409c2: *stop=0x8c0409c2u;r[15]+=4294967288u;goto L8c0409c4; /* add #-0x8,r15 */
L8c0409c4: *stop=0x8c0409c4u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[4],4);goto L8c0409c6; /* mov.l r4,@r15 */
L8c0409c6: *stop=0x8c0409c6u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[5],4);goto L8c0409c8; /* mov.l r5,@(0x4,r15) */
L8c0409c8: target=0x8c03fe1au;*pr=0x8c0409ccu;*stop=0x8c0409cau;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0409cc; /* bsr 0x8c03fe1a */
L8c0409cc: *stop=0x8c0409ccu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c0409ce; /* cmp/eq #0x0,r0 */
L8c0409ce: if(!(*sr&1u))goto L8c0409e6;goto L8c0409d0; /* bf 0x8c0409e6 */
L8c0409d0: target=0x8c03fe60u;*pr=0x8c0409d4u;*stop=0x8c0409d2u;if(!valid_ram(r[15],4u))return -3;r[4]=rd(m,r[15],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0409d4; /* bsr 0x8c03fe60 */
L8c0409d4: *stop=0x8c0409d4u;r[2]=rd(m,0x8c040a44u,4);goto L8c0409d6; /* mov.l 0x8c040a44,r2 */
L8c0409d6: target=r[2];*pr=0x8c0409dau;*stop=0x8c0409d8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0409da; /* jsr @r2 */
L8c0409da: target=0x8c040784u;*pr=0x8c0409deu;*stop=0x8c0409dcu;if(!valid_ram(r[15],4u))return -3;r[4]=rd(m,r[15],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0409de; /* bsr 0x8c040784 */
L8c0409de: *stop=0x8c0409deu;r[15]+=8u;goto L8c0409e0; /* add #0x8,r15 */
L8c0409e0: *stop=0x8c0409e0u;r[2]=rd(m,0x8c040a48u,4);goto L8c0409e2; /* mov.l 0x8c040a48,r2 */
L8c0409e2: target=r[2];*stop=0x8c0409e4u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c0409e6: *stop=0x8c0409e6u;r[15]+=8u;goto L8c0409e8; /* add #0x8,r15 */
L8c0409e8: *stop=0x8c0409e8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c0409ea; /* lds.l @r15+,PR */
L8c0409ea: *stop=0x8c0409eau;if(*pr!=entry_pr)return -3;*stop=0x8c0409ecu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c041c92(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c041c92;
L8c041c92: *stop=0x8c041c92u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c041c94; /* fldi1 fr3 */
L8c041c94: *stop=0x8c041c94u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c041c96; /* mov.l r14,@-r15 */
L8c041c96: *stop=0x8c041c96u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c041c98; /* fcmp/gt fr3,fr4 */
L8c041c98: *stop=0x8c041c98u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c041c9a; /* mov.l r13,@-r15 */
L8c041c9a: *stop=0x8c041c9au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c041c9c; /* sts.l PR,@-r15 */
L8c041c9c: if((*sr&1u))goto L8c041d60;goto L8c041c9e; /* bt 0x8c041d60 */
L8c041c9e: *stop=0x8c041c9eu;r[2]=0u;goto L8c041ca0; /* mov #0x0,r2 */
L8c041ca0: *stop=0x8c041ca0u;r[3]=rd(m,0x8c041d30u,4);goto L8c041ca2; /* mov.l 0x8c041d30,r3 */
L8c041ca2: *stop=0x8c041ca2u;r[5]=r[2];goto L8c041ca4; /* mov r2,r5 */
L8c041ca4: *stop=0x8c041ca4u;if(!valid_ram(r[4]+4u,4u))return -3;r[13]=rd(m,r[4]+0x4u,4);goto L8c041ca6; /* mov.l @(0x4,r4),r13 */
L8c041ca6: *stop=0x8c041ca6u;if(!valid_ram(r[4],4u))return -3;r[14]=rd(m,r[4],4);goto L8c041ca8; /* mov.l @r4,r14 */
L8c041ca8: *stop=0x8c041ca8u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[2],2);goto L8c041caa; /* mov.w r2,@r3 */
L8c041caa: *stop=0x8c041caau;r[1]=rd(m,0x8c041d34u,4);goto L8c041cac; /* mov.l 0x8c041d34,r1 */
L8c041cac: *stop=0x8c041cacu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[1]);goto L8c041cae; /* fmov.s fr4,@r1 */
L8c041cae: *stop=0x8c041caeu;r[2]=rd(m,0x8c041d38u,4);goto L8c041cb0; /* mov.l 0x8c041d38,r2 */
L8c041cb0: target=r[2];*pr=0x8c041cb4u;*stop=0x8c041cb2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041cb4; /* jsr @r2 */
L8c041cb4: *stop=0x8c041cb4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c041cb6; /* tst r0,r0 */
L8c041cb6: if(!(*sr&1u))goto L8c041d60;goto L8c041cb8; /* bf 0x8c041d60 */
L8c041cb8: *stop=0x8c041cb8u;r[2]=rd(m,0x8c041d38u,4);goto L8c041cba; /* mov.l 0x8c041d38,r2 */
L8c041cba: *stop=0x8c041cbau;r[5]=1u;goto L8c041cbc; /* mov #0x1,r5 */
L8c041cbc: target=r[2];*pr=0x8c041cc0u;*stop=0x8c041cbeu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041cc0; /* jsr @r2 */
L8c041cc0: *stop=0x8c041cc0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c041cc2; /* tst r0,r0 */
L8c041cc2: if(!(*sr&1u))goto L8c041d60;goto L8c041cc4; /* bf 0x8c041d60 */
L8c041cc4: *stop=0x8c041cc4u;r[2]=rd(m,0x8c041d3cu,4);goto L8c041cc6; /* mov.l 0x8c041d3c,r2 */
L8c041cc6: *stop=0x8c041cc6u;r[5]=0u;goto L8c041cc8; /* mov #0x0,r5 */
L8c041cc8: target=r[2];*pr=0x8c041cccu;*stop=0x8c041ccau;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041ccc; /* jsr @r2 */
L8c041ccc: *stop=0x8c041cccu;r[3]=rd(m,0x8c041d3cu,4);goto L8c041cce; /* mov.l 0x8c041d3c,r3 */
L8c041cce: *stop=0x8c041cceu;r[5]=1u;goto L8c041cd0; /* mov #0x1,r5 */
L8c041cd0: target=r[3];*pr=0x8c041cd4u;*stop=0x8c041cd2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041cd4; /* jsr @r3 */
L8c041cd4: *stop=0x8c041cd4u;r[2]=rd(m,0x8c041d40u,4);goto L8c041cd6; /* mov.l 0x8c041d40,r2 */
L8c041cd6: target=r[2];*pr=0x8c041cdau;*stop=0x8c041cd8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041cda; /* jsr @r2 */
L8c041cda: target=0x8c041a2eu;*pr=0x8c041cdeu;*stop=0x8c041cdcu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041cde; /* bsr 0x8c041a2e */
L8c041cde: *stop=0x8c041cdeu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c041ce0; /* lds.l @r15+,PR */
L8c041ce0: *stop=0x8c041ce0u;r[2]=rd(m,0x8c041d44u,4);goto L8c041ce2; /* mov.l 0x8c041d44,r2 */
L8c041ce2: *stop=0x8c041ce2u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c041ce4; /* mov.l @r15+,r13 */
L8c041ce4: target=r[2];*stop=0x8c041ce6u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c041d60: *stop=0x8c041d60u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c041d62; /* lds.l @r15+,PR */
L8c041d62: *stop=0x8c041d62u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c041d64; /* mov.l @r15+,r13 */
L8c041d64: *stop=0x8c041d64u;if(*pr!=entry_pr)return -3;*stop=0x8c041d66u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c5fde20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c5fde20;
L8c5fde20: *stop=0x8c5fde20u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c5fde22; /* mov.l r14,@-r15 */
L8c5fde22: *stop=0x8c5fde22u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c5fde24; /* mov.l r13,@-r15 */
L8c5fde24: *stop=0x8c5fde24u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c5fde26; /* mov.l r12,@-r15 */
L8c5fde26: *stop=0x8c5fde26u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c5fde28; /* mov.l r11,@-r15 */
L8c5fde28: *stop=0x8c5fde28u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c5fde2a; /* sts.l PR,@-r15 */
L8c5fde2a: *stop=0x8c5fde2au;r[15]+=4294967292u;goto L8c5fde2c; /* add #-0x4,r15 */
L8c5fde2c: *stop=0x8c5fde2cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c5fde2e; /* fldi1 fr3 */
L8c5fde2e: *stop=0x8c5fde2eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c5fde30; /* fcmp/gt fr3,fr4 */
L8c5fde30: *stop=0x8c5fde30u;if(!valid_ram(r[4]+4u,4u))return -3;r[5]=rd(m,r[4]+0x4u,4);goto L8c5fde32; /* mov.l @(0x4,r4),r5 */
L8c5fde32: take=!(*sr&1u);*stop=0x8c5fde34u;if(!valid_ram(r[4],4u))return -3;r[11]=rd(m,r[4],4);if(take)goto L8c5fde3a;goto L8c5fde36; /* bf/s 0x8c5fde3a */
L8c5fde36: *stop=0x8c5fde38u;goto L8c5fe0c4; /* bra 0x8c5fe0c4 */
L8c5fde3a: *stop=0x8c5fde3au;r[3]=rd(m,0x8c5fe040u,4);goto L8c5fde3c; /* mov.l 0x8c5fe040,r3 */
L8c5fde3c: *stop=0x8c5fde3cu;r[6]=0u;goto L8c5fde3e; /* mov #0x0,r6 */
L8c5fde3e: *stop=0x8c5fde3eu;r[2]=rd(m,0x8c5fe03cu,4);goto L8c5fde40; /* mov.l 0x8c5fe03c,r2 */
L8c5fde40: *stop=0x8c5fde40u;r[12]=rd(m,0x8c5fe048u,4);goto L8c5fde42; /* mov.l 0x8c5fe048,r12 */
L8c5fde42: *stop=0x8c5fde42u;r[13]=rd(m,0x8c5fe04cu,4);goto L8c5fde44; /* mov.l 0x8c5fe04c,r13 */
L8c5fde44: *stop=0x8c5fde44u;r[4]=rd(m,0x8c5fe050u,4);goto L8c5fde46; /* mov.l 0x8c5fe050,r4 */
L8c5fde46: *stop=0x8c5fde46u;r[7]=rd(m,0x8c5fe044u,4);goto L8c5fde48; /* mov.l 0x8c5fe044,r7 */
L8c5fde48: *stop=0x8c5fde48u;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[6],2);goto L8c5fde4a; /* mov.w r6,@r2 */
L8c5fde4a: *stop=0x8c5fde4au;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[3]);goto L8c5fde4c; /* fmov.s fr4,@r3 */
L8c5fde4c: *stop=0x8c5fde4cu;if(!valid_ram(r[5],4u))return -3;r[1]=rd(m,r[5],4);goto L8c5fde4e; /* mov.l @r5,r1 */
L8c5fde4e: *stop=0x8c5fde4eu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c5fde50; /* tst r1,r1 */
L8c5fde50: take=(*sr&1u);*stop=0x8c5fde52u;r[14]=r[12];if(take)goto L8c5fdeaa;goto L8c5fde54; /* bt/s 0x8c5fdeaa */
L8c5fde54: *stop=0x8c5fde54u;if(!valid_ram(r[5],4u))return -3;r[1]=rd(m,r[5],4);goto L8c5fde56; /* mov.l @r5,r1 */
L8c5fde56: *stop=0x8c5fde56u;if(!valid_ram(r[1]+4u,4u))return -3;r[0]=rd(m,r[1]+0x4u,4);goto L8c5fde58; /* mov.l @(0x4,r1),r0 */
L8c5fde58: *stop=0x8c5fde58u;*fpul=r[0];goto L8c5fde5a; /* lds r0,FPUL */
L8c5fde5a: *stop=0x8c5fde5au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c5fde5c; /* cmp/pz r0 */
L8c5fde5c: take=(*sr&1u);*stop=0x8c5fde5eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c5fde68;goto L8c5fde60; /* bt/s 0x8c5fde68 */
L8c5fde60: *stop=0x8c5fde60u;r[2]=rd(m,0x8c5fe054u,4);goto L8c5fde62; /* mov.l 0x8c5fe054,r2 */
L8c5fde62: *stop=0x8c5fde62u;*fpul=r[2];goto L8c5fde64; /* lds r2,FPUL */
L8c5fde64: *stop=0x8c5fde64u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c5fde66; /* fsts FPUL,fr2 */
L8c5fde66: *stop=0x8c5fde66u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c5fde68; /* fadd fr2,fr3 */
L8c5fde68: *stop=0x8c5fde68u;r[0]=8u;goto L8c5fde6a; /* mov #0x8,r0 */
L8c5fde6a: *stop=0x8c5fde6au;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[5]);goto L8c5fde6c; /* fmov.s @(r0,r5),fr1 */
L8c5fde6c: *stop=0x8c5fde6cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c5fde6e; /* fcmp/gt fr1,fr3 */
L8c5fde6e: if((*sr&1u))goto L8c5fde74;goto L8c5fde70; /* bt 0x8c5fde74 */
L8c5fde70: *stop=0x8c5fde72u;goto L8c5fe0c4; /* bra 0x8c5fe0c4 */
L8c5fde74: *stop=0x8c5fde74u;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c5fde76; /* mov.l @r5,r3 */
L8c5fde76: *stop=0x8c5fde76u;r[2]=rd(m,0x8c5fe058u,4);goto L8c5fde78; /* mov.l 0x8c5fe058,r2 */
L8c5fde78: *stop=0x8c5fde78u;if(!valid_ram(r[3]+10u,2u))return -3;r[0]=sx(m,r[3]+0xau,2);goto L8c5fde7a; /* mov.w @(0xa,r3),r0 */
L8c5fde7a: *stop=0x8c5fde7au;r[0]&=r[2];goto L8c5fde7c; /* and r2,r0 */
L8c5fde7c: *stop=0x8c5fde7cu;if(!valid_ram(r[14],2u))return -3;wr(m,r[14],r[0],2);goto L8c5fde7e; /* mov.w r0,@r14 */
L8c5fde7e: *stop=0x8c5fde7eu;r[14]=r[7];goto L8c5fde80; /* mov r7,r14 */
L8c5fde80: *stop=0x8c5fde80u;if(!valid_ram(r[5],4u))return -3;r[0]=rd(m,r[5],4);goto L8c5fde82; /* mov.l @r5,r0 */
L8c5fde82: *stop=0x8c5fde82u;if(!valid_ram(r[0]+10u,2u))return -3;r[0]=sx(m,r[0]+0xau,2);goto L8c5fde84; /* mov.w @(0xa,r0),r0 */
L8c5fde84: *stop=0x8c5fde84u;r[0]=r[0]&65535u;goto L8c5fde86; /* extu.w r0,r0 */
L8c5fde86: *stop=0x8c5fde86u;r[0]&=0xc0u;goto L8c5fde88; /* and #0xc0,r0 */
L8c5fde88: *stop=0x8c5fde88u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c5fde8a; /* cmp/pl r0 */
L8c5fde8a: *stop=0x8c5fde8au;r[0]=*sr&1u;goto L8c5fde8c; /* movt r0 */
L8c5fde8c: *stop=0x8c5fde8cu;if(!valid_ram(r[13],2u))return -3;wr(m,r[13],r[0],2);goto L8c5fde8e; /* mov.w r0,@r13 */
L8c5fde8e: *stop=0x8c5fde8eu;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c5fde90; /* mov.l @r5,r3 */
L8c5fde90: *stop=0x8c5fde90u;if(!valid_ram(r[3]+8u,2u))return -3;r[0]=sx(m,r[3]+0x8u,2);goto L8c5fde92; /* mov.w @(0x8,r3),r0 */
L8c5fde92: *stop=0x8c5fde92u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[0],2);goto L8c5fde94; /* mov.w r0,@r4 */
L8c5fde94: *stop=0x8c5fde94u;r[0]=8u;goto L8c5fde96; /* mov #0x8,r0 */
L8c5fde96: *stop=0x8c5fde96u;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c5fde98; /* mov.l @r5,r3 */
L8c5fde98: *stop=0x8c5fde98u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c5fde9a; /* mov.l @r3,r1 */
L8c5fde9a: *stop=0x8c5fde9au;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[1],4);goto L8c5fde9c; /* mov.l r1,@r14 */
L8c5fde9c: *stop=0x8c5fde9cu;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c5fde9e; /* mov.l @r5,r3 */
L8c5fde9e: *stop=0x8c5fde9eu;if(!valid_ram(r[3]+4u,4u))return -3;r[1]=rd(m,r[3]+0x4u,4);goto L8c5fdea0; /* mov.l @(0x4,r3),r1 */
L8c5fdea0: *stop=0x8c5fdea0u;if(!valid_ram(r[14]+16u,4u))return -3;wr(m,r[14]+0x10u,r[1],4);goto L8c5fdea2; /* mov.l r1,@(0x10,r14) */
L8c5fdea2: *stop=0x8c5fdea2u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto L8c5fdea4; /* fmov.s @(r0,r5),fr3 */
L8c5fdea4: *stop=0x8c5fdea4u;r[0]=12u;goto L8c5fdea6; /* mov #0xc,r0 */
L8c5fdea6: *stop=0x8c5fdea8u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto L8c5fdeae; /* bra 0x8c5fdeae */
L8c5fdeaa: *stop=0x8c5fdeaau;if(!valid_ram(r[14],2u))return -3;wr(m,r[14],r[6],2);goto L8c5fdeac; /* mov.w r6,@r14 */
L8c5fdeac: *stop=0x8c5fdeacu;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[6],2);goto L8c5fdeae; /* mov.w r6,@r4 */
L8c5fdeae: *stop=0x8c5fdeaeu;r[3]=0u;goto L8c5fdeb0; /* mov #0x0,r3 */
L8c5fdeb0: *stop=0x8c5fdeb0u;r[2]=r[7];goto L8c5fdeb2; /* mov r7,r2 */
L8c5fdeb2: *stop=0x8c5fdeb2u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);goto L8c5fdeb4; /* mov.l r2,@r15 */
L8c5fdeb4: *stop=0x8c5fdeb4u;r[14]=r[12];goto L8c5fdeb6; /* mov r12,r14 */
L8c5fdeb6: *stop=0x8c5fdeb6u;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[3],4);goto L8c5fdeb8; /* mov.l r3,@(0x14,r2) */
L8c5fdeb8: *stop=0x8c5fdeb8u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c5fdeba; /* mov.l @r15,r2 */
L8c5fdeba: *stop=0x8c5fdebau;if(!valid_ram(r[2]+24u,4u))return -3;wr(m,r[2]+0x18u,r[6],4);goto L8c5fdebc; /* mov.l r6,@(0x18,r2) */
L8c5fdebc: *stop=0x8c5fdebcu;if(!valid_ram(r[5]+4u,4u))return -3;r[3]=rd(m,r[5]+0x4u,4);goto L8c5fdebe; /* mov.l @(0x4,r5),r3 */
L8c5fdebe: *stop=0x8c5fdebeu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c5fdec0; /* tst r3,r3 */
L8c5fdec0: take=(*sr&1u);*stop=0x8c5fdec2u;r[14]+=2u;if(take)goto L8c5fdf1a;goto L8c5fdec4; /* bt/s 0x8c5fdf1a */
L8c5fdec4: *stop=0x8c5fdec4u;if(!valid_ram(r[5]+4u,4u))return -3;r[3]=rd(m,r[5]+0x4u,4);goto L8c5fdec6; /* mov.l @(0x4,r5),r3 */
L8c5fdec6: *stop=0x8c5fdec6u;if(!valid_ram(r[3]+4u,4u))return -3;r[2]=rd(m,r[3]+0x4u,4);goto L8c5fdec8; /* mov.l @(0x4,r3),r2 */
L8c5fdec8: *stop=0x8c5fdec8u;*fpul=r[2];goto L8c5fdeca; /* lds r2,FPUL */
L8c5fdeca: *stop=0x8c5fdecau;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c5fdecc; /* cmp/pz r2 */
L8c5fdecc: take=(*sr&1u);*stop=0x8c5fdeceu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c5fded8;goto L8c5fded0; /* bt/s 0x8c5fded8 */
L8c5fded0: *stop=0x8c5fded0u;r[1]=rd(m,0x8c5fe054u,4);goto L8c5fded2; /* mov.l 0x8c5fe054,r1 */
L8c5fded2: *stop=0x8c5fded2u;*fpul=r[1];goto L8c5fded4; /* lds r1,FPUL */
L8c5fded4: *stop=0x8c5fded4u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c5fded6; /* fsts FPUL,fr2 */
L8c5fded6: *stop=0x8c5fded6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c5fded8; /* fadd fr2,fr3 */
L8c5fded8: *stop=0x8c5fded8u;r[0]=12u;goto L8c5fdeda; /* mov #0xc,r0 */
L8c5fdeda: *stop=0x8c5fdedau;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[5]);goto L8c5fdedc; /* fmov.s @(r0,r5),fr1 */
L8c5fdedc: *stop=0x8c5fdedcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c5fdede; /* fcmp/gt fr1,fr3 */
L8c5fdede: if((*sr&1u))goto L8c5fdee4;goto L8c5fdee0; /* bt 0x8c5fdee4 */
L8c5fdee0: *stop=0x8c5fdee2u;goto L8c5fe0c4; /* bra 0x8c5fe0c4 */
L8c5fdee4: *stop=0x8c5fdee4u;if(!valid_ram(r[5]+4u,4u))return -3;r[2]=rd(m,r[5]+0x4u,4);goto L8c5fdee6; /* mov.l @(0x4,r5),r2 */
L8c5fdee6: *stop=0x8c5fdee6u;r[3]=rd(m,0x8c5fe058u,4);goto L8c5fdee8; /* mov.l 0x8c5fe058,r3 */
L8c5fdee8: *stop=0x8c5fdee8u;if(!valid_ram(r[2]+10u,2u))return -3;r[0]=sx(m,r[2]+0xau,2);goto L8c5fdeea; /* mov.w @(0xa,r2),r0 */
L8c5fdeea: *stop=0x8c5fdeeau;r[0]&=r[3];goto L8c5fdeec; /* and r3,r0 */
L8c5fdeec: *stop=0x8c5fdeecu;if(!valid_ram(r[14],2u))return -3;wr(m,r[14],r[0],2);goto L8c5fdeee; /* mov.w r0,@r14 */
L8c5fdeee: *stop=0x8c5fdeeeu;r[14]=r[7];goto L8c5fdef0; /* mov r7,r14 */
L8c5fdef0: *stop=0x8c5fdef0u;if(!valid_ram(r[5]+4u,4u))return -3;r[0]=rd(m,r[5]+0x4u,4);goto L8c5fdef2; /* mov.l @(0x4,r5),r0 */
L8c5fdef2: *stop=0x8c5fdef2u;r[14]+=28u;goto L8c5fdef4; /* add #0x1c,r14 */
L8c5fdef4: *stop=0x8c5fdef4u;if(!valid_ram(r[0]+10u,2u))return -3;r[0]=sx(m,r[0]+0xau,2);goto L8c5fdef6; /* mov.w @(0xa,r0),r0 */
L8c5fdef6: *stop=0x8c5fdef6u;r[0]=r[0]&65535u;goto L8c5fdef8; /* extu.w r0,r0 */
L8c5fdef8: *stop=0x8c5fdef8u;r[0]&=0xc0u;goto L8c5fdefa; /* and #0xc0,r0 */
L8c5fdefa: *stop=0x8c5fdefau;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c5fdefc; /* cmp/pl r0 */
L8c5fdefc: *stop=0x8c5fdefcu;r[0]=*sr&1u;goto L8c5fdefe; /* movt r0 */
L8c5fdefe: *stop=0x8c5fdefeu;if(!valid_ram(r[13],2u))return -3;wr(m,r[13],r[0],2);goto L8c5fdf00; /* mov.w r0,@r13 */
L8c5fdf00: *stop=0x8c5fdf00u;if(!valid_ram(r[5]+4u,4u))return -3;r[0]=rd(m,r[5]+0x4u,4);goto L8c5fdf02; /* mov.l @(0x4,r5),r0 */
L8c5fdf02: *stop=0x8c5fdf02u;if(!valid_ram(r[0]+8u,2u))return -3;r[0]=sx(m,r[0]+0x8u,2);goto L8c5fdf04; /* mov.w @(0x8,r0),r0 */
L8c5fdf04: *stop=0x8c5fdf04u;if(!valid_ram(r[4]+2u,2u))return -3;wr(m,r[4]+0x2u,r[0],2);goto L8c5fdf06; /* mov.w r0,@(0x2,r4) */
L8c5fdf06: *stop=0x8c5fdf06u;r[0]=12u;goto L8c5fdf08; /* mov #0xc,r0 */
L8c5fdf08: *stop=0x8c5fdf08u;if(!valid_ram(r[5]+4u,4u))return -3;r[2]=rd(m,r[5]+0x4u,4);goto L8c5fdf0a; /* mov.l @(0x4,r5),r2 */
L8c5fdf0a: *stop=0x8c5fdf0au;if(!valid_ram(r[2],4u))return -3;r[1]=rd(m,r[2],4);goto L8c5fdf0c; /* mov.l @r2,r1 */
L8c5fdf0c: *stop=0x8c5fdf0cu;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[1],4);goto L8c5fdf0e; /* mov.l r1,@r14 */
L8c5fdf0e: *stop=0x8c5fdf0eu;if(!valid_ram(r[5]+4u,4u))return -3;r[2]=rd(m,r[5]+0x4u,4);goto L8c5fdf10; /* mov.l @(0x4,r5),r2 */
L8c5fdf10: *stop=0x8c5fdf10u;if(!valid_ram(r[2]+4u,4u))return -3;r[1]=rd(m,r[2]+0x4u,4);goto L8c5fdf12; /* mov.l @(0x4,r2),r1 */
L8c5fdf12: *stop=0x8c5fdf12u;if(!valid_ram(r[14]+16u,4u))return -3;wr(m,r[14]+0x10u,r[1],4);goto L8c5fdf14; /* mov.l r1,@(0x10,r14) */
L8c5fdf14: *stop=0x8c5fdf14u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto L8c5fdf16; /* fmov.s @(r0,r5),fr3 */
L8c5fdf16: *stop=0x8c5fdf18u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto L8c5fdf20; /* bra 0x8c5fdf20 */
L8c5fdf1a: *stop=0x8c5fdf1au;r[0]=r[6];goto L8c5fdf1c; /* mov r6,r0 */
L8c5fdf1c: *stop=0x8c5fdf1cu;if(!valid_ram(r[14],2u))return -3;wr(m,r[14],r[6],2);goto L8c5fdf1e; /* mov.w r6,@r14 */
L8c5fdf1e: *stop=0x8c5fdf1eu;if(!valid_ram(r[4]+2u,2u))return -3;wr(m,r[4]+0x2u,r[0],2);goto L8c5fdf20; /* mov.w r0,@(0x2,r4) */
L8c5fdf20: *stop=0x8c5fdf20u;r[0]=rd(m,0x8c5fe060u,4);goto L8c5fdf22; /* mov.l 0x8c5fe060,r0 */
L8c5fdf22: *stop=0x8c5fdf22u;r[5]=r[7];goto L8c5fdf24; /* mov r7,r5 */
L8c5fdf24: *stop=0x8c5fdf24u;r[3]=rd(m,0x8c5fe05cu,4);goto L8c5fdf26; /* mov.l 0x8c5fe05c,r3 */
L8c5fdf26: *stop=0x8c5fdf26u;r[5]+=28u;goto L8c5fdf28; /* add #0x1c,r5 */
L8c5fdf28: *stop=0x8c5fdf28u;r[7]=rd(m,0x8c5fe064u,4);goto L8c5fdf2a; /* mov.l 0x8c5fe064,r7 */
L8c5fdf2a: *stop=0x8c5fdf2au;if(!valid_ram(r[5]+20u,4u))return -3;wr(m,r[5]+0x14u,r[6],4);goto L8c5fdf2c; /* mov.l r6,@(0x14,r5) */
L8c5fdf2c: *stop=0x8c5fdf2cu;if(!valid_ram(r[5]+24u,4u))return -3;wr(m,r[5]+0x18u,r[6],4);goto L8c5fdf2e; /* mov.l r6,@(0x18,r5) */
L8c5fdf2e: *stop=0x8c5fdf2eu;if(!valid_ram(r[12],2u))return -3;r[2]=sx(m,r[12],2);goto L8c5fdf30; /* mov.w @r12,r2 */
L8c5fdf30: *stop=0x8c5fdf30u;r[2]=r[2]&65535u;goto L8c5fdf32; /* extu.w r2,r2 */
L8c5fdf32: *stop=0x8c5fdf32u;r[2]<<=2;goto L8c5fdf34; /* shll2 r2 */
L8c5fdf34: *stop=0x8c5fdf34u;if(!valid_ram(r[0]+r[2],4u))return -3;r[1]=rd(m,r[2]+r[0],4);goto L8c5fdf36; /* mov.l @(r0,r2),r1 */
L8c5fdf36: *stop=0x8c5fdf36u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[1],4);goto L8c5fdf38; /* mov.l r1,@r3 */
L8c5fdf38: *stop=0x8c5fdf38u;if(!valid_ram(r[11],4u))return -3;r[0]=rd(m,r[11],4);goto L8c5fdf3a; /* mov.l @r11,r0 */
L8c5fdf3a: *stop=0x8c5fdf3au;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c5fdf3c; /* tst #0x20,r0 */
L8c5fdf3c: take=(*sr&1u);*stop=0x8c5fdf3eu;r[14]=1u;if(take)goto L8c5fdf44;goto L8c5fdf40; /* bt/s 0x8c5fdf44 */
L8c5fdf40: *stop=0x8c5fdf42u;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[14],4);goto L8c5fdf46; /* bra 0x8c5fdf46 */
L8c5fdf44: *stop=0x8c5fdf44u;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[6],4);goto L8c5fdf46; /* mov.l r6,@r7 */
L8c5fdf46: *stop=0x8c5fdf46u;if(!valid_ram(r[4],2u))return -3;r[3]=sx(m,r[4],2);goto L8c5fdf48; /* mov.w @r4,r3 */
L8c5fdf48: *stop=0x8c5fdf48u;r[5]=rd(m,0x8c5fe068u,4);goto L8c5fdf4a; /* mov.l 0x8c5fe068,r5 */
L8c5fdf4a: *stop=0x8c5fdf4au;r[3]=r[3]&65535u;goto L8c5fdf4c; /* extu.w r3,r3 */
L8c5fdf4c: *stop=0x8c5fdf4cu;*sr=(*sr&~1u)|!(r[14]&r[3]);goto L8c5fdf4e; /* tst r14,r3 */
L8c5fdf4e: if((*sr&1u))goto L8c5fdf56;goto L8c5fdf50; /* bt 0x8c5fdf56 */
L8c5fdf50: *stop=0x8c5fdf50u;r[1]=rd(m,0x8c5fe06cu,4);goto L8c5fdf52; /* mov.l 0x8c5fe06c,r1 */
L8c5fdf52: *stop=0x8c5fdf54u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c5fdf5a; /* bra 0x8c5fdf5a */
L8c5fdf56: *stop=0x8c5fdf56u;r[2]=rd(m,0x8c5fe070u,4);goto L8c5fdf58; /* mov.l 0x8c5fe070,r2 */
L8c5fdf58: *stop=0x8c5fdf58u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c5fdf5a; /* mov.l r2,@r5 */
L8c5fdf5a: *stop=0x8c5fdf5au;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c5fdf5c; /* mov.w @r4,r0 */
L8c5fdf5c: *stop=0x8c5fdf5cu;r[5]=rd(m,0x8c5fe074u,4);goto L8c5fdf5e; /* mov.l 0x8c5fe074,r5 */
L8c5fdf5e: *stop=0x8c5fdf5eu;r[0]=r[0]&65535u;goto L8c5fdf60; /* extu.w r0,r0 */
L8c5fdf60: *stop=0x8c5fdf60u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c5fdf62; /* tst #0x2,r0 */
L8c5fdf62: if((*sr&1u))goto L8c5fdf6a;goto L8c5fdf64; /* bt 0x8c5fdf6a */
L8c5fdf64: *stop=0x8c5fdf64u;r[2]=rd(m,0x8c5fe078u,4);goto L8c5fdf66; /* mov.l 0x8c5fe078,r2 */
L8c5fdf66: *stop=0x8c5fdf68u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c5fdf6e; /* bra 0x8c5fdf6e */
L8c5fdf6a: *stop=0x8c5fdf6au;r[1]=rd(m,0x8c5fe07cu,4);goto L8c5fdf6c; /* mov.l 0x8c5fe07c,r1 */
L8c5fdf6c: *stop=0x8c5fdf6cu;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c5fdf6e; /* mov.l r1,@r5 */
L8c5fdf6e: *stop=0x8c5fdf6eu;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c5fdf70; /* mov.w @r4,r0 */
L8c5fdf70: *stop=0x8c5fdf70u;r[5]=rd(m,0x8c5fe080u,4);goto L8c5fdf72; /* mov.l 0x8c5fe080,r5 */
L8c5fdf72: *stop=0x8c5fdf72u;r[0]=r[0]&65535u;goto L8c5fdf74; /* extu.w r0,r0 */
L8c5fdf74: *stop=0x8c5fdf74u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c5fdf76; /* tst #0x4,r0 */
L8c5fdf76: if((*sr&1u))goto L8c5fdf7e;goto L8c5fdf78; /* bt 0x8c5fdf7e */
L8c5fdf78: *stop=0x8c5fdf78u;r[2]=rd(m,0x8c5fe084u,4);goto L8c5fdf7a; /* mov.l 0x8c5fe084,r2 */
L8c5fdf7a: *stop=0x8c5fdf7cu;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c5fdf82; /* bra 0x8c5fdf82 */
L8c5fdf7e: *stop=0x8c5fdf7eu;r[1]=rd(m,0x8c5fe088u,4);goto L8c5fdf80; /* mov.l 0x8c5fe088,r1 */
L8c5fdf80: *stop=0x8c5fdf80u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c5fdf82; /* mov.l r1,@r5 */
L8c5fdf82: *stop=0x8c5fdf82u;if(!valid_ram(r[13],2u))return -3;r[5]=sx(m,r[13],2);goto L8c5fdf84; /* mov.w @r13,r5 */
L8c5fdf84: *stop=0x8c5fdf84u;r[0]=rd(m,0x8c5fe08cu,4);goto L8c5fdf86; /* mov.l 0x8c5fe08c,r0 */
L8c5fdf86: *stop=0x8c5fdf86u;r[5]=r[5]&65535u;goto L8c5fdf88; /* extu.w r5,r5 */
L8c5fdf88: *stop=0x8c5fdf88u;r[2]=rd(m,0x8c5fe090u,4);goto L8c5fdf8a; /* mov.l 0x8c5fe090,r2 */
L8c5fdf8a: *stop=0x8c5fdf8au;r[5]<<=2;goto L8c5fdf8c; /* shll2 r5 */
L8c5fdf8c: *stop=0x8c5fdf8cu;r[1]=rd(m,0x8c5fe098u,4);goto L8c5fdf8e; /* mov.l 0x8c5fe098,r1 */
L8c5fdf8e: *stop=0x8c5fdf8eu;if(!valid_ram(r[0]+r[5],4u))return -3;r[3]=rd(m,r[5]+r[0],4);goto L8c5fdf90; /* mov.l @(r0,r5),r3 */
L8c5fdf90: *stop=0x8c5fdf90u;r[0]=rd(m,0x8c5fe094u,4);goto L8c5fdf92; /* mov.l 0x8c5fe094,r0 */
L8c5fdf92: *stop=0x8c5fdf92u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c5fdf94; /* mov.l r3,@r2 */
L8c5fdf94: *stop=0x8c5fdf94u;if(!valid_ram(r[0]+r[5],4u))return -3;r[2]=rd(m,r[5]+r[0],4);goto L8c5fdf96; /* mov.l @(r0,r5),r2 */
L8c5fdf96: *stop=0x8c5fdf96u;r[0]=rd(m,0x8c5fe09cu,4);goto L8c5fdf98; /* mov.l 0x8c5fe09c,r0 */
L8c5fdf98: *stop=0x8c5fdf98u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[2],4);goto L8c5fdf9a; /* mov.l r2,@r1 */
L8c5fdf9a: *stop=0x8c5fdf9au;r[1]=rd(m,0x8c5fe0a0u,4);goto L8c5fdf9c; /* mov.l 0x8c5fe0a0,r1 */
L8c5fdf9c: *stop=0x8c5fdf9cu;if(!valid_ram(r[0]+r[5],4u))return -3;r[3]=rd(m,r[5]+r[0],4);goto L8c5fdf9e; /* mov.l @(r0,r5),r3 */
L8c5fdf9e: *stop=0x8c5fdf9eu;r[0]=rd(m,0x8c5fe0a4u,4);goto L8c5fdfa0; /* mov.l 0x8c5fe0a4,r0 */
L8c5fdfa0: *stop=0x8c5fdfa0u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[3],4);goto L8c5fdfa2; /* mov.l r3,@r1 */
L8c5fdfa2: *stop=0x8c5fdfa2u;r[1]=rd(m,0x8c5fe0a8u,4);goto L8c5fdfa4; /* mov.l 0x8c5fe0a8,r1 */
L8c5fdfa4: *stop=0x8c5fdfa4u;if(!valid_ram(r[0]+r[5],4u))return -3;r[2]=rd(m,r[5]+r[0],4);goto L8c5fdfa6; /* mov.l @(r0,r5),r2 */
L8c5fdfa6: *stop=0x8c5fdfa6u;r[3]=rd(m,0x8c5fe0acu,4);goto L8c5fdfa8; /* mov.l 0x8c5fe0ac,r3 */
L8c5fdfa8: *stop=0x8c5fdfa8u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[2],4);goto L8c5fdfaa; /* mov.l r2,@r1 */
L8c5fdfaa: *stop=0x8c5fdfaau;if(!valid_ram(r[12]+2u,2u))return -3;r[0]=sx(m,r[12]+0x2u,2);goto L8c5fdfac; /* mov.w @(0x2,r12),r0 */
L8c5fdfac: *stop=0x8c5fdfacu;r[1]=rd(m,0x8c5fe060u,4);goto L8c5fdfae; /* mov.l 0x8c5fe060,r1 */
L8c5fdfae: *stop=0x8c5fdfaeu;r[0]=r[0]&65535u;goto L8c5fdfb0; /* extu.w r0,r0 */
L8c5fdfb0: *stop=0x8c5fdfb0u;r[0]<<=2;goto L8c5fdfb2; /* shll2 r0 */
L8c5fdfb2: *stop=0x8c5fdfb2u;if(!valid_ram(r[0]+r[1],4u))return -3;r[1]=rd(m,r[1]+r[0],4);goto L8c5fdfb4; /* mov.l @(r0,r1),r1 */
L8c5fdfb4: *stop=0x8c5fdfb4u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[1],4);goto L8c5fdfb6; /* mov.l r1,@r3 */
L8c5fdfb6: *stop=0x8c5fdfb6u;if(!valid_ram(r[11],4u))return -3;r[0]=rd(m,r[11],4);goto L8c5fdfb8; /* mov.l @r11,r0 */
L8c5fdfb8: *stop=0x8c5fdfb8u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c5fdfba; /* tst #0x20,r0 */
L8c5fdfba: if((*sr&1u))goto L8c5fdfc0;goto L8c5fdfbc; /* bt 0x8c5fdfc0 */
L8c5fdfbc: *stop=0x8c5fdfbeu;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[14],4);goto L8c5fdfc2; /* bra 0x8c5fdfc2 */
L8c5fdfc0: *stop=0x8c5fdfc0u;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[6],4);goto L8c5fdfc2; /* mov.l r6,@r7 */
L8c5fdfc2: *stop=0x8c5fdfc2u;if(!valid_ram(r[4]+2u,2u))return -3;r[0]=sx(m,r[4]+0x2u,2);goto L8c5fdfc4; /* mov.w @(0x2,r4),r0 */
L8c5fdfc4: *stop=0x8c5fdfc4u;r[5]=rd(m,0x8c5fe0b0u,4);goto L8c5fdfc6; /* mov.l 0x8c5fe0b0,r5 */
L8c5fdfc6: *stop=0x8c5fdfc6u;r[0]=r[0]&65535u;goto L8c5fdfc8; /* extu.w r0,r0 */
L8c5fdfc8: *stop=0x8c5fdfc8u;*sr=(*sr&~1u)|!(r[14]&r[0]);goto L8c5fdfca; /* tst r14,r0 */
L8c5fdfca: if((*sr&1u))goto L8c5fdfd2;goto L8c5fdfcc; /* bt 0x8c5fdfd2 */
L8c5fdfcc: *stop=0x8c5fdfccu;r[2]=rd(m,0x8c5fe06cu,4);goto L8c5fdfce; /* mov.l 0x8c5fe06c,r2 */
L8c5fdfce: *stop=0x8c5fdfd0u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c5fdfd6; /* bra 0x8c5fdfd6 */
L8c5fdfd2: *stop=0x8c5fdfd2u;r[1]=rd(m,0x8c5fe070u,4);goto L8c5fdfd4; /* mov.l 0x8c5fe070,r1 */
L8c5fdfd4: *stop=0x8c5fdfd4u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c5fdfd6; /* mov.l r1,@r5 */
L8c5fdfd6: *stop=0x8c5fdfd6u;if(!valid_ram(r[4]+2u,2u))return -3;r[0]=sx(m,r[4]+0x2u,2);goto L8c5fdfd8; /* mov.w @(0x2,r4),r0 */
L8c5fdfd8: *stop=0x8c5fdfd8u;r[5]=rd(m,0x8c5fe0b4u,4);goto L8c5fdfda; /* mov.l 0x8c5fe0b4,r5 */
L8c5fdfda: *stop=0x8c5fdfdau;r[0]=r[0]&65535u;goto L8c5fdfdc; /* extu.w r0,r0 */
L8c5fdfdc: *stop=0x8c5fdfdcu;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c5fdfde; /* tst #0x2,r0 */
L8c5fdfde: if((*sr&1u))goto L8c5fdfe6;goto L8c5fdfe0; /* bt 0x8c5fdfe6 */
L8c5fdfe0: *stop=0x8c5fdfe0u;r[2]=rd(m,0x8c5fe078u,4);goto L8c5fdfe2; /* mov.l 0x8c5fe078,r2 */
L8c5fdfe2: *stop=0x8c5fdfe4u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c5fdfea; /* bra 0x8c5fdfea */
L8c5fdfe6: *stop=0x8c5fdfe6u;r[1]=rd(m,0x8c5fe07cu,4);goto L8c5fdfe8; /* mov.l 0x8c5fe07c,r1 */
L8c5fdfe8: *stop=0x8c5fdfe8u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c5fdfea; /* mov.l r1,@r5 */
L8c5fdfea: *stop=0x8c5fdfeau;if(!valid_ram(r[4]+2u,2u))return -3;r[0]=sx(m,r[4]+0x2u,2);goto L8c5fdfec; /* mov.w @(0x2,r4),r0 */
L8c5fdfec: *stop=0x8c5fdfecu;r[5]=rd(m,0x8c5fe0b8u,4);goto L8c5fdfee; /* mov.l 0x8c5fe0b8,r5 */
L8c5fdfee: *stop=0x8c5fdfeeu;r[0]=r[0]&65535u;goto L8c5fdff0; /* extu.w r0,r0 */
L8c5fdff0: *stop=0x8c5fdff0u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c5fdff2; /* tst #0x4,r0 */
L8c5fdff2: if((*sr&1u))goto L8c5fdffa;goto L8c5fdff4; /* bt 0x8c5fdffa */
L8c5fdff4: *stop=0x8c5fdff4u;r[2]=rd(m,0x8c5fe084u,4);goto L8c5fdff6; /* mov.l 0x8c5fe084,r2 */
L8c5fdff6: *stop=0x8c5fdff8u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c5fdffe; /* bra 0x8c5fdffe */
L8c5fdffa: *stop=0x8c5fdffau;r[1]=rd(m,0x8c5fe088u,4);goto L8c5fdffc; /* mov.l 0x8c5fe088,r1 */
L8c5fdffc: *stop=0x8c5fdffcu;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c5fdffe; /* mov.l r1,@r5 */
L8c5fdffe: *stop=0x8c5fdffeu;if(!valid_ram(r[13],2u))return -3;r[4]=sx(m,r[13],2);goto L8c5fe000; /* mov.w @r13,r4 */
L8c5fe000: *stop=0x8c5fe000u;r[0]=rd(m,0x8c5fe08cu,4);goto L8c5fe002; /* mov.l 0x8c5fe08c,r0 */
L8c5fe002: *stop=0x8c5fe002u;r[4]=r[4]&65535u;goto L8c5fe004; /* extu.w r4,r4 */
L8c5fe004: *stop=0x8c5fe004u;r[2]=rd(m,0x8c5fe090u,4);goto L8c5fe006; /* mov.l 0x8c5fe090,r2 */
L8c5fe006: *stop=0x8c5fe006u;r[4]<<=2;goto L8c5fe008; /* shll2 r4 */
L8c5fe008: *stop=0x8c5fe008u;r[1]=rd(m,0x8c5fe098u,4);goto L8c5fe00a; /* mov.l 0x8c5fe098,r1 */
L8c5fe00a: *stop=0x8c5fe00au;if(!valid_ram(r[0]+r[4],4u))return -3;r[3]=rd(m,r[4]+r[0],4);goto L8c5fe00c; /* mov.l @(r0,r4),r3 */
L8c5fe00c: *stop=0x8c5fe00cu;r[0]=rd(m,0x8c5fe094u,4);goto L8c5fe00e; /* mov.l 0x8c5fe094,r0 */
L8c5fe00e: *stop=0x8c5fe00eu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c5fe010; /* mov.l r3,@r2 */
L8c5fe010: *stop=0x8c5fe010u;if(!valid_ram(r[0]+r[4],4u))return -3;r[2]=rd(m,r[4]+r[0],4);goto L8c5fe012; /* mov.l @(r0,r4),r2 */
L8c5fe012: *stop=0x8c5fe012u;r[0]=rd(m,0x8c5fe09cu,4);goto L8c5fe014; /* mov.l 0x8c5fe09c,r0 */
L8c5fe014: *stop=0x8c5fe014u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[2],4);goto L8c5fe016; /* mov.l r2,@r1 */
L8c5fe016: *stop=0x8c5fe016u;r[1]=rd(m,0x8c5fe0a0u,4);goto L8c5fe018; /* mov.l 0x8c5fe0a0,r1 */
L8c5fe018: *stop=0x8c5fe018u;if(!valid_ram(r[0]+r[4],4u))return -3;r[3]=rd(m,r[4]+r[0],4);goto L8c5fe01a; /* mov.l @(r0,r4),r3 */
L8c5fe01a: *stop=0x8c5fe01au;r[0]=rd(m,0x8c5fe0a4u,4);goto L8c5fe01c; /* mov.l 0x8c5fe0a4,r0 */
L8c5fe01c: *stop=0x8c5fe01cu;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[3],4);goto L8c5fe01e; /* mov.l r3,@r1 */
L8c5fe01e: *stop=0x8c5fe01eu;r[3]=rd(m,0x8c5fe0bcu,4);goto L8c5fe020; /* mov.l 0x8c5fe0bc,r3 */
L8c5fe020: *stop=0x8c5fe020u;r[1]=rd(m,0x8c5fe0a8u,4);goto L8c5fe022; /* mov.l 0x8c5fe0a8,r1 */
L8c5fe022: *stop=0x8c5fe022u;if(!valid_ram(r[0]+r[4],4u))return -3;r[2]=rd(m,r[4]+r[0],4);goto L8c5fe024; /* mov.l @(r0,r4),r2 */
L8c5fe024: target=r[3];*pr=0x8c5fe028u;*stop=0x8c5fe026u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[2],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fe028; /* jsr @r3 */
L8c5fe028: target=0x8c5fdbecu;*pr=0x8c5fe02cu;*stop=0x8c5fe02au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fe02c; /* bsr 0x8c5fdbec */
L8c5fe02c: *stop=0x8c5fe02cu;r[15]+=4u;goto L8c5fe02e; /* add #0x4,r15 */
L8c5fe02e: *stop=0x8c5fe02eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c5fe030; /* lds.l @r15+,PR */
L8c5fe030: *stop=0x8c5fe030u;r[3]=rd(m,0x8c5fe0c0u,4);goto L8c5fe032; /* mov.l 0x8c5fe0c0,r3 */
L8c5fe032: *stop=0x8c5fe032u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c5fe034; /* mov.l @r15+,r11 */
L8c5fe034: *stop=0x8c5fe034u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c5fe036; /* mov.l @r15+,r12 */
L8c5fe036: *stop=0x8c5fe036u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c5fe038; /* mov.l @r15+,r13 */
L8c5fe038: target=r[3];*stop=0x8c5fe03au;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c5fe0c4: *stop=0x8c5fe0c4u;r[15]+=4u;goto L8c5fe0c6; /* add #0x4,r15 */
L8c5fe0c6: *stop=0x8c5fe0c6u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c5fe0c8; /* lds.l @r15+,PR */
L8c5fe0c8: *stop=0x8c5fe0c8u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c5fe0ca; /* mov.l @r15+,r11 */
L8c5fe0ca: *stop=0x8c5fe0cau;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c5fe0cc; /* mov.l @r15+,r12 */
L8c5fe0cc: *stop=0x8c5fe0ccu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c5fe0ce; /* mov.l @r15+,r13 */
L8c5fe0ce: *stop=0x8c5fe0ceu;if(*pr!=entry_pr)return -3;*stop=0x8c5fe0d0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c03700c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c03700c;
L8c03700c: *stop=0x8c03700cu;r[0]=rd(m,0x8c037018u,4);goto L8c03700e; /* mov.l 0x8c037018,r0 */
L8c03700e: *stop=0x8c03700eu;if(!valid_ram(r[0],4u))return -3;r[0]=rd(m,r[0],4);goto L8c037010; /* mov.l @r0,r0 */
L8c037010: *stop=0x8c037010u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c037012; /* tst r0,r0 */
L8c037012: if((*sr&1u))goto L8c03701c;goto L8c037014; /* bt 0x8c03701c */
L8c037014: *stop=0x8c037014u;if(*pr!=entry_pr)return -3;*stop=0x8c037016u;return 0; /* rts */
L8c03701c: *stop=0x8c03701cu;r[7]=*pr;goto L8c03701e; /* sts PR,r7 */
L8c03701e: target=0x8c03718cu;*pr=0x8c037022u;*stop=0x8c037020u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037022; /* bsr 0x8c03718c */
L8c037022: *stop=0x8c037022u;*pr=r[7];goto L8c037024; /* lds r7,PR */
L8c037024: if(!(*sr&1u))goto L8c03702a;goto L8c037026; /* bf 0x8c03702a */
L8c037026: *stop=0x8c037026u;if(*pr!=entry_pr)return -3;*stop=0x8c037028u;return 0; /* rts */
L8c03702a: *stop=0x8c03702au;r[0]=rd(m,0x8c037088u,4);goto L8c03702c; /* mov.l 0x8c037088,r0 */
L8c03702c: *stop=0x8c03702cu;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4u;wr(m,r[0],*pr,4);goto L8c03702e; /* sts.l PR,@-r0 */
L8c03702e: *stop=0x8c03702eu;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[0]);goto L8c037030; /* fmov.s fr15,@-r0 */
L8c037030: *stop=0x8c037030u;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[0]);goto L8c037032; /* fmov.s fr14,@-r0 */
L8c037032: *stop=0x8c037032u;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[0]);goto L8c037034; /* fmov.s fr13,@-r0 */
L8c037034: *stop=0x8c037034u;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[0]);goto L8c037036; /* fmov.s fr12,@-r0 */
L8c037036: *stop=0x8c037036u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[14],4);goto L8c037038; /* mov.l r14,@-r0 */
L8c037038: *stop=0x8c037038u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[13],4);goto L8c03703a; /* mov.l r13,@-r0 */
L8c03703a: *stop=0x8c03703au;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[12],4);goto L8c03703c; /* mov.l r12,@-r0 */
L8c03703c: *stop=0x8c03703cu;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[11],4);goto L8c03703e; /* mov.l r11,@-r0 */
L8c03703e: *stop=0x8c03703eu;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[10],4);goto L8c037040; /* mov.l r10,@-r0 */
L8c037040: *stop=0x8c037040u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[9],4);goto L8c037042; /* mov.l r9,@-r0 */
L8c037042: *stop=0x8c037042u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[8],4);goto L8c037044; /* mov.l r8,@-r0 */
L8c037044: target=0x8c037294u;*pr=0x8c037048u;*stop=0x8c037046u;r[13]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037048; /* bsr 0x8c037294 */
L8c037048: *stop=0x8c037048u;if(!valid_ram(r[4]+8u,4u))return -3;r[0]=rd(m,r[4]+0x8u,4);goto L8c03704a; /* mov.l @(0x8,r4),r0 */
L8c03704a: *stop=0x8c03704au;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[0]);goto L8c03704c; /* cmp/ge r0,r13 */
L8c03704c: if((*sr&1u))goto L8c03706a;goto L8c03704e; /* bt 0x8c03706a */
L8c03704e: target=0x8c037350u;*pr=0x8c037052u;*stop=0x8c037050u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037052; /* bsr 0x8c037350 */
L8c037052: *stop=0x8c037052u;r[14]=r[4];goto L8c037054; /* mov r4,r14 */
L8c037054: *stop=0x8c037054u;r[0]=rd(m,0x8c03708cu,4);goto L8c037056; /* mov.l 0x8c03708c,r0 */
L8c037056: target=r[0];*pr=0x8c03705au;*stop=0x8c037058u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03705a; /* jsr @r0 */
L8c03705a: *stop=0x8c03705au;r[4]=r[14];goto L8c03705c; /* mov r14,r4 */
L8c03705c: target=0x8c0376d0u;*pr=0x8c037060u;*stop=0x8c03705eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037060; /* bsr 0x8c0376d0 */
L8c037060: target=0x8c036ffcu;*pr=0x8c037064u;*stop=0x8c037062u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037064; /* bsr 0x8c036ffc */
L8c037064: *stop=0x8c037064u;r[0]=rd(m,0x8c037090u,4);goto L8c037066; /* mov.l 0x8c037090,r0 */
L8c037066: target=r[0];*pr=0x8c03706au;*stop=0x8c037068u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03706a; /* jsr @r0 */
L8c03706a: *stop=0x8c03706au;r[0]=rd(m,0x8c037094u,4);goto L8c03706c; /* mov.l 0x8c037094,r0 */
L8c03706c: *stop=0x8c03706cu;if(!valid_ram(r[0],4u))return -3;r[8]=rd(m,r[0],4);r[0]+=4;goto L8c03706e; /* mov.l @r0+,r8 */
L8c03706e: *stop=0x8c03706eu;if(!valid_ram(r[0],4u))return -3;r[9]=rd(m,r[0],4);r[0]+=4;goto L8c037070; /* mov.l @r0+,r9 */
L8c037070: *stop=0x8c037070u;if(!valid_ram(r[0],4u))return -3;r[10]=rd(m,r[0],4);r[0]+=4;goto L8c037072; /* mov.l @r0+,r10 */
L8c037072: *stop=0x8c037072u;if(!valid_ram(r[0],4u))return -3;r[11]=rd(m,r[0],4);r[0]+=4;goto L8c037074; /* mov.l @r0+,r11 */
L8c037074: *stop=0x8c037074u;if(!valid_ram(r[0],4u))return -3;r[12]=rd(m,r[0],4);r[0]+=4;goto L8c037076; /* mov.l @r0+,r12 */
L8c037076: *stop=0x8c037076u;if(!valid_ram(r[0],4u))return -3;r[13]=rd(m,r[0],4);r[0]+=4;goto L8c037078; /* mov.l @r0+,r13 */
L8c037078: *stop=0x8c037078u;if(!valid_ram(r[0],4u))return -3;r[14]=rd(m,r[0],4);r[0]+=4;goto L8c03707a; /* mov.l @r0+,r14 */
L8c03707a: *stop=0x8c03707au;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03707c; /* fmov.s @r0+,fr12 */
L8c03707c: *stop=0x8c03707cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03707e; /* fmov.s @r0+,fr13 */
L8c03707e: *stop=0x8c03707eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037080; /* fmov.s @r0+,fr14 */
L8c037080: *stop=0x8c037080u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037082; /* fmov.s @r0+,fr15 */
L8c037082: *stop=0x8c037082u;if(!valid_ram(r[0],4u))return -3;*pr=rd(m,r[0],4);r[0]+=4u;goto L8c037084; /* lds.l @r0+,PR */
L8c037084: *stop=0x8c037084u;if(*pr!=entry_pr)return -3;*stop=0x8c037086u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c037098(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c037098;
L8c037098: *stop=0x8c037098u;r[0]=rd(m,0x8c0370a4u,4);goto L8c03709a; /* mov.l 0x8c0370a4,r0 */
L8c03709a: *stop=0x8c03709au;if(!valid_ram(r[0],4u))return -3;r[0]=rd(m,r[0],4);goto L8c03709c; /* mov.l @r0,r0 */
L8c03709c: *stop=0x8c03709cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c03709e; /* tst r0,r0 */
L8c03709e: if((*sr&1u))goto L8c0370a8;goto L8c0370a0; /* bt 0x8c0370a8 */
L8c0370a0: *stop=0x8c0370a0u;if(*pr!=entry_pr)return -3;*stop=0x8c0370a2u;return 0; /* rts */
L8c0370a8: *stop=0x8c0370a8u;r[7]=*pr;goto L8c0370aa; /* sts PR,r7 */
L8c0370aa: target=0x8c03718cu;*pr=0x8c0370aeu;*stop=0x8c0370acu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0370ae; /* bsr 0x8c03718c */
L8c0370ae: *stop=0x8c0370aeu;*pr=r[7];goto L8c0370b0; /* lds r7,PR */
L8c0370b0: if(!(*sr&1u))goto L8c0370b6;goto L8c0370b2; /* bf 0x8c0370b6 */
L8c0370b2: *stop=0x8c0370b2u;if(*pr!=entry_pr)return -3;*stop=0x8c0370b4u;return 0; /* rts */
L8c0370b6: *stop=0x8c0370b6u;r[0]=rd(m,0x8c037100u,4);goto L8c0370b8; /* mov.l 0x8c037100,r0 */
L8c0370b8: *stop=0x8c0370b8u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4u;wr(m,r[0],*pr,4);goto L8c0370ba; /* sts.l PR,@-r0 */
L8c0370ba: *stop=0x8c0370bau;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[0]);goto L8c0370bc; /* fmov.s fr15,@-r0 */
L8c0370bc: *stop=0x8c0370bcu;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[0]);goto L8c0370be; /* fmov.s fr14,@-r0 */
L8c0370be: *stop=0x8c0370beu;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[0]);goto L8c0370c0; /* fmov.s fr13,@-r0 */
L8c0370c0: *stop=0x8c0370c0u;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[0]);goto L8c0370c2; /* fmov.s fr12,@-r0 */
L8c0370c2: *stop=0x8c0370c2u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[14],4);goto L8c0370c4; /* mov.l r14,@-r0 */
L8c0370c4: *stop=0x8c0370c4u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[13],4);goto L8c0370c6; /* mov.l r13,@-r0 */
L8c0370c6: *stop=0x8c0370c6u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[12],4);goto L8c0370c8; /* mov.l r12,@-r0 */
L8c0370c8: *stop=0x8c0370c8u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[11],4);goto L8c0370ca; /* mov.l r11,@-r0 */
L8c0370ca: *stop=0x8c0370cau;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[10],4);goto L8c0370cc; /* mov.l r10,@-r0 */
L8c0370cc: *stop=0x8c0370ccu;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[9],4);goto L8c0370ce; /* mov.l r9,@-r0 */
L8c0370ce: *stop=0x8c0370ceu;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[8],4);goto L8c0370d0; /* mov.l r8,@-r0 */
L8c0370d0: target=0x8c037294u;*pr=0x8c0370d4u;*stop=0x8c0370d2u;r[13]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0370d4; /* bsr 0x8c037294 */
L8c0370d4: *stop=0x8c0370d4u;if(!valid_ram(r[4]+8u,4u))return -3;r[0]=rd(m,r[4]+0x8u,4);goto L8c0370d6; /* mov.l @(0x8,r4),r0 */
L8c0370d6: *stop=0x8c0370d6u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[0]);goto L8c0370d8; /* cmp/ge r0,r13 */
L8c0370d8: if((*sr&1u))goto L8c0370e2;goto L8c0370da; /* bt 0x8c0370e2 */
L8c0370da: target=0x8c037350u;*pr=0x8c0370deu;*stop=0x8c0370dcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0370de; /* bsr 0x8c037350 */
L8c0370de: target=0x8c0376d0u;*pr=0x8c0370e2u;*stop=0x8c0370e0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0370e2; /* bsr 0x8c0376d0 */
L8c0370e2: *stop=0x8c0370e2u;r[0]=rd(m,0x8c037104u,4);goto L8c0370e4; /* mov.l 0x8c037104,r0 */
L8c0370e4: *stop=0x8c0370e4u;if(!valid_ram(r[0],4u))return -3;r[8]=rd(m,r[0],4);r[0]+=4;goto L8c0370e6; /* mov.l @r0+,r8 */
L8c0370e6: *stop=0x8c0370e6u;if(!valid_ram(r[0],4u))return -3;r[9]=rd(m,r[0],4);r[0]+=4;goto L8c0370e8; /* mov.l @r0+,r9 */
L8c0370e8: *stop=0x8c0370e8u;if(!valid_ram(r[0],4u))return -3;r[10]=rd(m,r[0],4);r[0]+=4;goto L8c0370ea; /* mov.l @r0+,r10 */
L8c0370ea: *stop=0x8c0370eau;if(!valid_ram(r[0],4u))return -3;r[11]=rd(m,r[0],4);r[0]+=4;goto L8c0370ec; /* mov.l @r0+,r11 */
L8c0370ec: *stop=0x8c0370ecu;if(!valid_ram(r[0],4u))return -3;r[12]=rd(m,r[0],4);r[0]+=4;goto L8c0370ee; /* mov.l @r0+,r12 */
L8c0370ee: *stop=0x8c0370eeu;if(!valid_ram(r[0],4u))return -3;r[13]=rd(m,r[0],4);r[0]+=4;goto L8c0370f0; /* mov.l @r0+,r13 */
L8c0370f0: *stop=0x8c0370f0u;if(!valid_ram(r[0],4u))return -3;r[14]=rd(m,r[0],4);r[0]+=4;goto L8c0370f2; /* mov.l @r0+,r14 */
L8c0370f2: *stop=0x8c0370f2u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0370f4; /* fmov.s @r0+,fr12 */
L8c0370f4: *stop=0x8c0370f4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0370f6; /* fmov.s @r0+,fr13 */
L8c0370f6: *stop=0x8c0370f6u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0370f8; /* fmov.s @r0+,fr14 */
L8c0370f8: *stop=0x8c0370f8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0370fa; /* fmov.s @r0+,fr15 */
L8c0370fa: *stop=0x8c0370fau;if(!valid_ram(r[0],4u))return -3;*pr=rd(m,r[0],4);r[0]+=4u;goto L8c0370fc; /* lds.l @r0+,PR */
L8c0370fc: *stop=0x8c0370feu;if(!call)return -2;return call(ctx,0x8c036ffcu,m,f,sr,r,pr); /* bra 0x8c036ffc */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0400a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0400a0;
L8c0400a0: *stop=0x8c0400a0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0400a2; /* mov.l r14,@-r15 */
L8c0400a2: *stop=0x8c0400a2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0400a4; /* mov.l r13,@-r15 */
L8c0400a4: *stop=0x8c0400a4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0400a6; /* mov.l r12,@-r15 */
L8c0400a6: *stop=0x8c0400a6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c0400a8; /* mov.l r11,@-r15 */
L8c0400a8: *stop=0x8c0400a8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c0400aa; /* sts.l PR,@-r15 */
L8c0400aa: *stop=0x8c0400aau;r[15]+=4294967280u;goto L8c0400ac; /* add #-0x10,r15 */
L8c0400ac: *stop=0x8c0400acu;r[14]=rd(m,0x8c040384u,4);goto L8c0400ae; /* mov.l 0x8c040384,r14 */
L8c0400ae: *stop=0x8c0400aeu;if(!valid_ram(r[14]+20u,4u))return -3;r[13]=rd(m,r[14]+0x14u,4);goto L8c0400b0; /* mov.l @(0x14,r14),r13 */
L8c0400b0: *stop=0x8c0400b0u;if(!valid_ram(r[14]+4u,4u))return -3;r[0]=rd(m,r[14]+0x4u,4);goto L8c0400b2; /* mov.l @(0x4,r14),r0 */
L8c0400b2: *stop=0x8c0400b2u;r[5]=r[13];goto L8c0400b4; /* mov r13,r5 */
L8c0400b4: *stop=0x8c0400b4u;r[5]<<=2;goto L8c0400b6; /* shll2 r5 */
L8c0400b6: *stop=0x8c0400b6u;if(!valid_ram(r[0]+r[5],4u))return -3;r[12]=rd(m,r[5]+r[0],4);goto L8c0400b8; /* mov.l @(r0,r5),r12 */
L8c0400b8: *stop=0x8c0400b8u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c0400ba; /* tst r12,r12 */
L8c0400ba: take=(*sr&1u);*stop=0x8c0400bcu;r[4]+=8u;if(take)goto L8c0400da;goto L8c0400be; /* bt/s 0x8c0400da */
L8c0400be: *stop=0x8c0400beu;r[6]=r[15];goto L8c0400c0; /* mov r15,r6 */
L8c0400c0: *stop=0x8c0400c0u;if(!valid_ram(r[14]+8u,4u))return -3;r[0]=rd(m,r[14]+0x8u,4);goto L8c0400c2; /* mov.l @(0x8,r14),r0 */
L8c0400c2: *stop=0x8c0400c2u;if(!valid_ram(r[0]+r[5],4u))return -3;r[11]=rd(m,r[5]+r[0],4);goto L8c0400c4; /* mov.l @(r0,r5),r11 */
L8c0400c4: *stop=0x8c0400c4u;r[5]=r[11];goto L8c0400c6; /* mov r11,r5 */
L8c0400c6: target=0x8c03feb8u;*pr=0x8c0400cau;*stop=0x8c0400c8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0400ca; /* bsr 0x8c03feb8 */
L8c0400ca: *stop=0x8c0400cau;r[7]=r[15];goto L8c0400cc; /* mov r15,r7 */
L8c0400cc: *stop=0x8c0400ccu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c0400ce; /* mov.l @r15,r5 */
L8c0400ce: *stop=0x8c0400ceu;r[6]=r[11];goto L8c0400d0; /* mov r11,r6 */
L8c0400d0: *stop=0x8c0400d0u;r[7]+=4u;goto L8c0400d2; /* add #0x4,r7 */
L8c0400d2: target=0x8c03ff2cu;*pr=0x8c0400d6u;*stop=0x8c0400d4u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0400d6; /* bsr 0x8c03ff2c */
L8c0400d6: *stop=0x8c0400d6u;r[4]=r[15];goto L8c0400d8; /* mov r15,r4 */
L8c0400d8: *stop=0x8c0400d8u;r[4]+=4u;goto L8c0400da; /* add #0x4,r4 */
L8c0400da: *stop=0x8c0400dau;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0400dc; /* fmov.s @r4+,fr4 */
L8c0400dc: *stop=0x8c0400dcu;r[3]=rd(m,0x8c040388u,4);goto L8c0400de; /* mov.l 0x8c040388,r3 */
L8c0400de: *stop=0x8c0400deu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0400e0; /* fmov.s @r4+,fr5 */
L8c0400e0: *stop=0x8c0400e0u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c0400e2; /* fmov.s @r4,fr6 */
L8c0400e2: target=r[3];*pr=0x8c0400e6u;*stop=0x8c0400e4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0400e6; /* jsr @r3 */
L8c0400e6: *stop=0x8c0400e6u;r[13]+=1u;goto L8c0400e8; /* add #0x1,r13 */
L8c0400e8: *stop=0x8c0400e8u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[13],4);goto L8c0400ea; /* mov.l r13,@(0x14,r14) */
L8c0400ea: *stop=0x8c0400eau;r[15]+=16u;goto L8c0400ec; /* add #0x10,r15 */
L8c0400ec: *stop=0x8c0400ecu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c0400ee; /* lds.l @r15+,PR */
L8c0400ee: *stop=0x8c0400eeu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c0400f0; /* mov.l @r15+,r11 */
L8c0400f0: *stop=0x8c0400f0u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0400f2; /* mov.l @r15+,r12 */
L8c0400f2: *stop=0x8c0400f2u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0400f4; /* mov.l @r15+,r13 */
L8c0400f4: *stop=0x8c0400f4u;if(*pr!=entry_pr)return -3;*stop=0x8c0400f6u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0400f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0400f8;
L8c0400f8: *stop=0x8c0400f8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0400fa; /* mov.l r14,@-r15 */
L8c0400fa: *stop=0x8c0400fau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0400fc; /* mov.l r13,@-r15 */
L8c0400fc: *stop=0x8c0400fcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0400fe; /* mov.l r12,@-r15 */
L8c0400fe: *stop=0x8c0400feu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c040100; /* mov.l r11,@-r15 */
L8c040100: *stop=0x8c040100u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c040102; /* sts.l PR,@-r15 */
L8c040102: *stop=0x8c040102u;r[15]+=4294967280u;goto L8c040104; /* add #-0x10,r15 */
L8c040104: *stop=0x8c040104u;r[14]=rd(m,0x8c040384u,4);goto L8c040106; /* mov.l 0x8c040384,r14 */
L8c040106: *stop=0x8c040106u;if(!valid_ram(r[14]+20u,4u))return -3;r[13]=rd(m,r[14]+0x14u,4);goto L8c040108; /* mov.l @(0x14,r14),r13 */
L8c040108: *stop=0x8c040108u;if(!valid_ram(r[14]+4u,4u))return -3;r[0]=rd(m,r[14]+0x4u,4);goto L8c04010a; /* mov.l @(0x4,r14),r0 */
L8c04010a: *stop=0x8c04010au;r[5]=r[13];goto L8c04010c; /* mov r13,r5 */
L8c04010c: *stop=0x8c04010cu;r[5]<<=2;goto L8c04010e; /* shll2 r5 */
L8c04010e: *stop=0x8c04010eu;if(!valid_ram(r[0]+r[5],4u))return -3;r[12]=rd(m,r[5]+r[0],4);goto L8c040110; /* mov.l @(r0,r5),r12 */
L8c040110: *stop=0x8c040110u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c040112; /* tst r12,r12 */
L8c040112: take=(*sr&1u);*stop=0x8c040114u;r[4]+=32u;if(take)goto L8c040132;goto L8c040116; /* bt/s 0x8c040132 */
L8c040116: *stop=0x8c040116u;r[6]=r[15];goto L8c040118; /* mov r15,r6 */
L8c040118: *stop=0x8c040118u;if(!valid_ram(r[14]+8u,4u))return -3;r[0]=rd(m,r[14]+0x8u,4);goto L8c04011a; /* mov.l @(0x8,r14),r0 */
L8c04011a: *stop=0x8c04011au;if(!valid_ram(r[0]+r[5],4u))return -3;r[11]=rd(m,r[5]+r[0],4);goto L8c04011c; /* mov.l @(r0,r5),r11 */
L8c04011c: *stop=0x8c04011cu;r[5]=r[11];goto L8c04011e; /* mov r11,r5 */
L8c04011e: target=0x8c03feb8u;*pr=0x8c040122u;*stop=0x8c040120u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040122; /* bsr 0x8c03feb8 */
L8c040122: *stop=0x8c040122u;r[7]=r[15];goto L8c040124; /* mov r15,r7 */
L8c040124: *stop=0x8c040124u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c040126; /* mov.l @r15,r5 */
L8c040126: *stop=0x8c040126u;r[6]=r[11];goto L8c040128; /* mov r11,r6 */
L8c040128: *stop=0x8c040128u;r[7]+=4u;goto L8c04012a; /* add #0x4,r7 */
L8c04012a: target=0x8c03ff2cu;*pr=0x8c04012eu;*stop=0x8c04012cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04012e; /* bsr 0x8c03ff2c */
L8c04012e: *stop=0x8c04012eu;r[4]=r[15];goto L8c040130; /* mov r15,r4 */
L8c040130: *stop=0x8c040130u;r[4]+=4u;goto L8c040132; /* add #0x4,r4 */
L8c040132: *stop=0x8c040132u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040134; /* fmov.s @r4+,fr4 */
L8c040134: *stop=0x8c040134u;r[3]=rd(m,0x8c04038cu,4);goto L8c040136; /* mov.l 0x8c04038c,r3 */
L8c040136: *stop=0x8c040136u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040138; /* fmov.s @r4+,fr5 */
L8c040138: *stop=0x8c040138u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c04013a; /* fmov.s @r4,fr6 */
L8c04013a: target=r[3];*pr=0x8c04013eu;*stop=0x8c04013cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04013e; /* jsr @r3 */
L8c04013e: *stop=0x8c04013eu;r[13]+=1u;goto L8c040140; /* add #0x1,r13 */
L8c040140: *stop=0x8c040140u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[13],4);goto L8c040142; /* mov.l r13,@(0x14,r14) */
L8c040142: *stop=0x8c040142u;r[15]+=16u;goto L8c040144; /* add #0x10,r15 */
L8c040144: *stop=0x8c040144u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c040146; /* lds.l @r15+,PR */
L8c040146: *stop=0x8c040146u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c040148; /* mov.l @r15+,r11 */
L8c040148: *stop=0x8c040148u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c04014a; /* mov.l @r15+,r12 */
L8c04014a: *stop=0x8c04014au;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c04014c; /* mov.l @r15+,r13 */
L8c04014c: *stop=0x8c04014cu;if(*pr!=entry_pr)return -3;*stop=0x8c04014eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040150(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c040150;
L8c040150: *stop=0x8c040150u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040152; /* mov.l r14,@-r15 */
L8c040152: *stop=0x8c040152u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c040154; /* mov.l r13,@-r15 */
L8c040154: *stop=0x8c040154u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c040156; /* mov.l r12,@-r15 */
L8c040156: *stop=0x8c040156u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c040158; /* mov.l r11,@-r15 */
L8c040158: *stop=0x8c040158u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c04015a; /* sts.l PR,@-r15 */
L8c04015a: *stop=0x8c04015au;r[15]+=4294967280u;goto L8c04015c; /* add #-0x10,r15 */
L8c04015c: *stop=0x8c04015cu;r[14]=rd(m,0x8c040384u,4);goto L8c04015e; /* mov.l 0x8c040384,r14 */
L8c04015e: *stop=0x8c04015eu;if(!valid_ram(r[14]+20u,4u))return -3;r[13]=rd(m,r[14]+0x14u,4);goto L8c040160; /* mov.l @(0x14,r14),r13 */
L8c040160: *stop=0x8c040160u;if(!valid_ram(r[14]+4u,4u))return -3;r[0]=rd(m,r[14]+0x4u,4);goto L8c040162; /* mov.l @(0x4,r14),r0 */
L8c040162: *stop=0x8c040162u;r[5]=r[13];goto L8c040164; /* mov r13,r5 */
L8c040164: *stop=0x8c040164u;r[5]<<=2;goto L8c040166; /* shll2 r5 */
L8c040166: *stop=0x8c040166u;if(!valid_ram(r[0]+r[5],4u))return -3;r[12]=rd(m,r[5]+r[0],4);goto L8c040168; /* mov.l @(r0,r5),r12 */
L8c040168: *stop=0x8c040168u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c04016a; /* tst r12,r12 */
L8c04016a: take=(*sr&1u);*stop=0x8c04016cu;r[4]+=20u;if(take)goto L8c04018a;goto L8c04016e; /* bt/s 0x8c04018a */
L8c04016e: *stop=0x8c04016eu;r[6]=r[15];goto L8c040170; /* mov r15,r6 */
L8c040170: *stop=0x8c040170u;if(!valid_ram(r[14]+8u,4u))return -3;r[0]=rd(m,r[14]+0x8u,4);goto L8c040172; /* mov.l @(0x8,r14),r0 */
L8c040172: *stop=0x8c040172u;if(!valid_ram(r[0]+r[5],4u))return -3;r[11]=rd(m,r[5]+r[0],4);goto L8c040174; /* mov.l @(r0,r5),r11 */
L8c040174: *stop=0x8c040174u;r[5]=r[11];goto L8c040176; /* mov r11,r5 */
L8c040176: target=0x8c03feb8u;*pr=0x8c04017au;*stop=0x8c040178u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04017a; /* bsr 0x8c03feb8 */
L8c04017a: *stop=0x8c04017au;r[7]=r[15];goto L8c04017c; /* mov r15,r7 */
L8c04017c: *stop=0x8c04017cu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c04017e; /* mov.l @r15,r5 */
L8c04017e: *stop=0x8c04017eu;r[6]=r[11];goto L8c040180; /* mov r11,r6 */
L8c040180: *stop=0x8c040180u;r[7]+=4u;goto L8c040182; /* add #0x4,r7 */
L8c040182: target=0x8c03ff90u;*pr=0x8c040186u;*stop=0x8c040184u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040186; /* bsr 0x8c03ff90 */
L8c040186: *stop=0x8c040186u;r[4]=r[15];goto L8c040188; /* mov r15,r4 */
L8c040188: *stop=0x8c040188u;r[4]+=4u;goto L8c04018a; /* add #0x4,r4 */
L8c04018a: *stop=0x8c04018au;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c04018c; /* mov.l @r4,r5 */
L8c04018c: *stop=0x8c04018cu;if(!valid_ram(r[4]+4u,4u))return -3;r[6]=rd(m,r[4]+0x4u,4);goto L8c04018e; /* mov.l @(0x4,r4),r6 */
L8c04018e: *stop=0x8c04018eu;if(!valid_ram(r[4]+8u,4u))return -3;r[7]=rd(m,r[4]+0x8u,4);goto L8c040190; /* mov.l @(0x8,r4),r7 */
L8c040190: *stop=0x8c040190u;r[2]=rd(m,0x8c040390u,4);goto L8c040192; /* mov.l 0x8c040390,r2 */
L8c040192: target=r[2];*pr=0x8c040196u;*stop=0x8c040194u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040196; /* jsr @r2 */
L8c040196: *stop=0x8c040196u;r[13]+=1u;goto L8c040198; /* add #0x1,r13 */
L8c040198: *stop=0x8c040198u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[13],4);goto L8c04019a; /* mov.l r13,@(0x14,r14) */
L8c04019a: *stop=0x8c04019au;r[15]+=16u;goto L8c04019c; /* add #0x10,r15 */
L8c04019c: *stop=0x8c04019cu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c04019e; /* lds.l @r15+,PR */
L8c04019e: *stop=0x8c04019eu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c0401a0; /* mov.l @r15+,r11 */
L8c0401a0: *stop=0x8c0401a0u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0401a2; /* mov.l @r15+,r12 */
L8c0401a2: *stop=0x8c0401a2u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0401a4; /* mov.l @r15+,r13 */
L8c0401a4: *stop=0x8c0401a4u;if(*pr!=entry_pr)return -3;*stop=0x8c0401a6u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0401a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0401a8;
L8c0401a8: *stop=0x8c0401a8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0401aa; /* mov.l r14,@-r15 */
L8c0401aa: *stop=0x8c0401aau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0401ac; /* mov.l r13,@-r15 */
L8c0401ac: *stop=0x8c0401acu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0401ae; /* mov.l r12,@-r15 */
L8c0401ae: *stop=0x8c0401aeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c0401b0; /* mov.l r11,@-r15 */
L8c0401b0: *stop=0x8c0401b0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c0401b2; /* sts.l PR,@-r15 */
L8c0401b2: *stop=0x8c0401b2u;r[15]+=4294967280u;goto L8c0401b4; /* add #-0x10,r15 */
L8c0401b4: *stop=0x8c0401b4u;r[14]=rd(m,0x8c040384u,4);goto L8c0401b6; /* mov.l 0x8c040384,r14 */
L8c0401b6: *stop=0x8c0401b6u;if(!valid_ram(r[14]+20u,4u))return -3;r[13]=rd(m,r[14]+0x14u,4);goto L8c0401b8; /* mov.l @(0x14,r14),r13 */
L8c0401b8: *stop=0x8c0401b8u;if(!valid_ram(r[14]+4u,4u))return -3;r[0]=rd(m,r[14]+0x4u,4);goto L8c0401ba; /* mov.l @(0x4,r14),r0 */
L8c0401ba: *stop=0x8c0401bau;r[5]=r[13];goto L8c0401bc; /* mov r13,r5 */
L8c0401bc: *stop=0x8c0401bcu;r[5]<<=2;goto L8c0401be; /* shll2 r5 */
L8c0401be: *stop=0x8c0401beu;if(!valid_ram(r[0]+r[5],4u))return -3;r[12]=rd(m,r[5]+r[0],4);goto L8c0401c0; /* mov.l @(r0,r5),r12 */
L8c0401c0: *stop=0x8c0401c0u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c0401c2; /* tst r12,r12 */
L8c0401c2: take=(*sr&1u);*stop=0x8c0401c4u;r[4]+=20u;if(take)goto L8c0401e2;goto L8c0401c6; /* bt/s 0x8c0401e2 */
L8c0401c6: *stop=0x8c0401c6u;r[6]=r[15];goto L8c0401c8; /* mov r15,r6 */
L8c0401c8: *stop=0x8c0401c8u;if(!valid_ram(r[14]+8u,4u))return -3;r[0]=rd(m,r[14]+0x8u,4);goto L8c0401ca; /* mov.l @(0x8,r14),r0 */
L8c0401ca: *stop=0x8c0401cau;if(!valid_ram(r[0]+r[5],4u))return -3;r[11]=rd(m,r[5]+r[0],4);goto L8c0401cc; /* mov.l @(r0,r5),r11 */
L8c0401cc: *stop=0x8c0401ccu;r[5]=r[11];goto L8c0401ce; /* mov r11,r5 */
L8c0401ce: target=0x8c03feb8u;*pr=0x8c0401d2u;*stop=0x8c0401d0u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0401d2; /* bsr 0x8c03feb8 */
L8c0401d2: *stop=0x8c0401d2u;r[7]=r[15];goto L8c0401d4; /* mov r15,r7 */
L8c0401d4: *stop=0x8c0401d4u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c0401d6; /* mov.l @r15,r5 */
L8c0401d6: *stop=0x8c0401d6u;r[6]=r[11];goto L8c0401d8; /* mov r11,r6 */
L8c0401d8: *stop=0x8c0401d8u;r[7]+=4u;goto L8c0401da; /* add #0x4,r7 */
L8c0401da: target=0x8c03ff90u;*pr=0x8c0401deu;*stop=0x8c0401dcu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0401de; /* bsr 0x8c03ff90 */
L8c0401de: *stop=0x8c0401deu;r[4]=r[15];goto L8c0401e0; /* mov r15,r4 */
L8c0401e0: *stop=0x8c0401e0u;r[4]+=4u;goto L8c0401e2; /* add #0x4,r4 */
L8c0401e2: *stop=0x8c0401e2u;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c0401e4; /* mov.l @r4,r5 */
L8c0401e4: *stop=0x8c0401e4u;if(!valid_ram(r[4]+4u,4u))return -3;r[6]=rd(m,r[4]+0x4u,4);goto L8c0401e6; /* mov.l @(0x4,r4),r6 */
L8c0401e6: *stop=0x8c0401e6u;if(!valid_ram(r[4]+8u,4u))return -3;r[7]=rd(m,r[4]+0x8u,4);goto L8c0401e8; /* mov.l @(0x8,r4),r7 */
L8c0401e8: *stop=0x8c0401e8u;r[2]=rd(m,0x8c040394u,4);goto L8c0401ea; /* mov.l 0x8c040394,r2 */
L8c0401ea: target=r[2];*pr=0x8c0401eeu;*stop=0x8c0401ecu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0401ee; /* jsr @r2 */
L8c0401ee: *stop=0x8c0401eeu;r[13]+=1u;goto L8c0401f0; /* add #0x1,r13 */
L8c0401f0: *stop=0x8c0401f0u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[13],4);goto L8c0401f2; /* mov.l r13,@(0x14,r14) */
L8c0401f2: *stop=0x8c0401f2u;r[15]+=16u;goto L8c0401f4; /* add #0x10,r15 */
L8c0401f4: *stop=0x8c0401f4u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c0401f6; /* lds.l @r15+,PR */
L8c0401f6: *stop=0x8c0401f6u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c0401f8; /* mov.l @r15+,r11 */
L8c0401f8: *stop=0x8c0401f8u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0401fa; /* mov.l @r15+,r12 */
L8c0401fa: *stop=0x8c0401fau;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0401fc; /* mov.l @r15+,r13 */
L8c0401fc: *stop=0x8c0401fcu;if(*pr!=entry_pr)return -3;*stop=0x8c0401feu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c04057a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c04057a;
L8c04057a: *stop=0x8c04057au;r[4]+=8u;goto L8c04057c; /* add #0x8,r4 */
L8c04057c: *stop=0x8c04057cu;r[3]=rd(m,0x8c040674u,4);goto L8c04057e; /* mov.l 0x8c040674,r3 */
L8c04057e: *stop=0x8c04057eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040580; /* fmov.s @r4+,fr4 */
L8c040580: *stop=0x8c040580u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040582; /* fmov.s @r4+,fr5 */
L8c040582: *stop=0x8c040582u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c040584; /* fmov.s @r4,fr6 */
L8c040584: target=r[3];*stop=0x8c040586u;r[4]=0u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040588(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040588;
L8c040588: *stop=0x8c040588u;r[3]=rd(m,0x8c040678u,4);goto L8c04058a; /* mov.l 0x8c040678,r3 */
L8c04058a: *stop=0x8c04058au;r[4]+=20u;goto L8c04058c; /* add #0x14,r4 */
L8c04058c: *stop=0x8c04058cu;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c04058e; /* mov.l @r4,r5 */
L8c04058e: *stop=0x8c04058eu;if(!valid_ram(r[4]+4u,4u))return -3;r[6]=rd(m,r[4]+0x4u,4);goto L8c040590; /* mov.l @(0x4,r4),r6 */
L8c040590: *stop=0x8c040590u;if(!valid_ram(r[4]+8u,4u))return -3;r[7]=rd(m,r[4]+0x8u,4);goto L8c040592; /* mov.l @(0x8,r4),r7 */
L8c040592: target=r[3];*stop=0x8c040594u;r[4]=0u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040596(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040596;
L8c040596: *stop=0x8c040596u;r[4]+=20u;goto L8c040598; /* add #0x14,r4 */
L8c040598: *stop=0x8c040598u;r[3]=rd(m,0x8c04067cu,4);goto L8c04059a; /* mov.l 0x8c04067c,r3 */
L8c04059a: *stop=0x8c04059au;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c04059c; /* mov.l @r4,r5 */
L8c04059c: *stop=0x8c04059cu;if(!valid_ram(r[4]+4u,4u))return -3;r[6]=rd(m,r[4]+0x4u,4);goto L8c04059e; /* mov.l @(0x4,r4),r6 */
L8c04059e: *stop=0x8c04059eu;if(!valid_ram(r[4]+8u,4u))return -3;r[7]=rd(m,r[4]+0x8u,4);goto L8c0405a0; /* mov.l @(0x8,r4),r7 */
L8c0405a0: target=r[3];*stop=0x8c0405a2u;r[4]=0u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0405a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0405a4;
L8c0405a4: *stop=0x8c0405a4u;r[3]=rd(m,0x8c040680u,4);goto L8c0405a6; /* mov.l 0x8c040680,r3 */
L8c0405a6: *stop=0x8c0405a6u;r[4]+=32u;goto L8c0405a8; /* add #0x20,r4 */
L8c0405a8: *stop=0x8c0405a8u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0405aa; /* fmov.s @r4+,fr4 */
L8c0405aa: *stop=0x8c0405aau;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0405ac; /* fmov.s @r4+,fr5 */
L8c0405ac: *stop=0x8c0405acu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c0405ae; /* fmov.s @r4,fr6 */
L8c0405ae: target=r[3];*stop=0x8c0405b0u;r[4]=0u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0405b2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0405b2;
L8c0405b2: *stop=0x8c0405b2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0405b4; /* mov.l r14,@-r15 */
L8c0405b4: *stop=0x8c0405b4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c0405b6; /* sts.l PR,@-r15 */
L8c0405b6: *stop=0x8c0405b6u;r[2]=rd(m,0x8c040684u,4);goto L8c0405b8; /* mov.l 0x8c040684,r2 */
L8c0405b8: *stop=0x8c0405b8u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c0405ba; /* mov.l @r2,r3 */
L8c0405ba: target=r[3];*pr=0x8c0405beu;*stop=0x8c0405bcu;r[14]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0405be; /* jsr @r3 */
L8c0405be: *stop=0x8c0405beu;r[2]=rd(m,0x8c040688u,4);goto L8c0405c0; /* mov.l 0x8c040688,r2 */
L8c0405c0: *stop=0x8c0405c0u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c0405c2; /* mov.l @r2,r3 */
L8c0405c2: target=r[3];*pr=0x8c0405c6u;*stop=0x8c0405c4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0405c6; /* jsr @r3 */
L8c0405c6: *stop=0x8c0405c6u;r[4]=r[14];goto L8c0405c8; /* mov r14,r4 */
L8c0405c8: *stop=0x8c0405c8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c0405ca; /* lds.l @r15+,PR */
L8c0405ca: *stop=0x8c0405cau;r[2]=rd(m,0x8c04068cu,4);goto L8c0405cc; /* mov.l 0x8c04068c,r2 */
L8c0405cc: *stop=0x8c0405ccu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c0405ce; /* mov.l @r2,r3 */
L8c0405ce: target=r[3];*stop=0x8c0405d0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0405d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0405d2;
L8c0405d2: *stop=0x8c0405d2u;r[3]=0u;goto L8c0405d4; /* mov #0x0,r3 */
L8c0405d4: *stop=0x8c0405d4u;r[1]=rd(m,0x8c040684u,4);goto L8c0405d6; /* mov.l 0x8c040684,r1 */
L8c0405d6: *stop=0x8c0405d6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0405d8; /* mov.l r14,@-r15 */
L8c0405d8: *stop=0x8c0405d8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0405da; /* mov.l r13,@-r15 */
L8c0405da: *stop=0x8c0405dau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0405dc; /* mov.l r12,@-r15 */
L8c0405dc: *stop=0x8c0405dcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c0405de; /* sts.l PR,@-r15 */
L8c0405de: *stop=0x8c0405deu;r[12]=r[4];goto L8c0405e0; /* mov r4,r12 */
L8c0405e0: *stop=0x8c0405e0u;r[14]=rd(m,0x8c040690u,4);goto L8c0405e2; /* mov.l 0x8c040690,r14 */
L8c0405e2: *stop=0x8c0405e2u;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c0405e4; /* mov.l @r14,r13 */
L8c0405e4: *stop=0x8c0405e4u;r[2]=r[13];goto L8c0405e6; /* mov r13,r2 */
L8c0405e6: *stop=0x8c0405e6u;r[2]+=4u;goto L8c0405e8; /* add #0x4,r2 */
L8c0405e8: *stop=0x8c0405e8u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c0405ea; /* mov.l r13,@(0x4,r14) */
L8c0405ea: *stop=0x8c0405eau;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[2],4);goto L8c0405ec; /* mov.l r2,@(0x8,r14) */
L8c0405ec: *stop=0x8c0405ecu;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[3],4);goto L8c0405ee; /* mov.l r3,@(0x14,r14) */
L8c0405ee: *stop=0x8c0405eeu;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c0405f0; /* mov.l @r1,r2 */
L8c0405f0: target=r[2];*pr=0x8c0405f4u;*stop=0x8c0405f2u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0405f4; /* jsr @r2 */
L8c0405f4: *stop=0x8c0405f4u;r[2]=rd(m,0x8c040688u,4);goto L8c0405f6; /* mov.l 0x8c040688,r2 */
L8c0405f6: *stop=0x8c0405f6u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c0405f8; /* mov.l @r2,r3 */
L8c0405f8: target=r[3];*pr=0x8c0405fcu;*stop=0x8c0405fau;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0405fc; /* jsr @r3 */
L8c0405fc: *stop=0x8c0405fcu;r[2]=rd(m,0x8c04068cu,4);goto L8c0405fe; /* mov.l 0x8c04068c,r2 */
L8c0405fe: *stop=0x8c0405feu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c040600; /* mov.l @r2,r3 */
L8c040600: target=r[3];*pr=0x8c040604u;*stop=0x8c040602u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040604; /* jsr @r3 */
L8c040604: *stop=0x8c040604u;r[13]+=8u;goto L8c040606; /* add #0x8,r13 */
L8c040606: *stop=0x8c040606u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c040608; /* mov.l r13,@r14 */
L8c040608: *stop=0x8c040608u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c04060a; /* lds.l @r15+,PR */
L8c04060a: *stop=0x8c04060au;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c04060c; /* mov.l @r15+,r12 */
L8c04060c: *stop=0x8c04060cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c04060e; /* mov.l @r15+,r13 */
L8c04060e: *stop=0x8c04060eu;if(*pr!=entry_pr)return -3;*stop=0x8c040610u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040612(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040612;
L8c040612: *stop=0x8c040612u;r[3]=0u;goto L8c040614; /* mov #0x0,r3 */
L8c040614: *stop=0x8c040614u;r[1]=rd(m,0x8c040684u,4);goto L8c040616; /* mov.l 0x8c040684,r1 */
L8c040616: *stop=0x8c040616u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040618; /* mov.l r14,@-r15 */
L8c040618: *stop=0x8c040618u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c04061a; /* mov.l r13,@-r15 */
L8c04061a: *stop=0x8c04061au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c04061c; /* mov.l r12,@-r15 */
L8c04061c: *stop=0x8c04061cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c04061e; /* sts.l PR,@-r15 */
L8c04061e: *stop=0x8c04061eu;r[12]=r[4];goto L8c040620; /* mov r4,r12 */
L8c040620: *stop=0x8c040620u;r[14]=rd(m,0x8c040690u,4);goto L8c040622; /* mov.l 0x8c040690,r14 */
L8c040622: *stop=0x8c040622u;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c040624; /* mov.l @r14,r13 */
L8c040624: *stop=0x8c040624u;r[2]=r[13];goto L8c040626; /* mov r13,r2 */
L8c040626: *stop=0x8c040626u;r[2]+=8u;goto L8c040628; /* add #0x8,r2 */
L8c040628: *stop=0x8c040628u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c04062a; /* mov.l r13,@(0x4,r14) */
L8c04062a: *stop=0x8c04062au;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[2],4);goto L8c04062c; /* mov.l r2,@(0x8,r14) */
L8c04062c: *stop=0x8c04062cu;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[3],4);goto L8c04062e; /* mov.l r3,@(0x14,r14) */
L8c04062e: *stop=0x8c04062eu;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c040630; /* mov.l @r1,r2 */
L8c040630: target=r[2];*pr=0x8c040634u;*stop=0x8c040632u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040634; /* jsr @r2 */
L8c040634: *stop=0x8c040634u;r[2]=rd(m,0x8c040688u,4);goto L8c040636; /* mov.l 0x8c040688,r2 */
L8c040636: *stop=0x8c040636u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c040638; /* mov.l @r2,r3 */
L8c040638: target=r[3];*pr=0x8c04063cu;*stop=0x8c04063au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04063c; /* jsr @r3 */
L8c04063c: *stop=0x8c04063cu;r[2]=rd(m,0x8c04068cu,4);goto L8c04063e; /* mov.l 0x8c04068c,r2 */
L8c04063e: *stop=0x8c04063eu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c040640; /* mov.l @r2,r3 */
L8c040640: target=r[3];*pr=0x8c040644u;*stop=0x8c040642u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040644; /* jsr @r3 */
L8c040644: *stop=0x8c040644u;r[13]+=16u;goto L8c040646; /* add #0x10,r13 */
L8c040646: *stop=0x8c040646u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c040648; /* mov.l r13,@r14 */
L8c040648: *stop=0x8c040648u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c04064a; /* lds.l @r15+,PR */
L8c04064a: *stop=0x8c04064au;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c04064c; /* mov.l @r15+,r12 */
L8c04064c: *stop=0x8c04064cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c04064e; /* mov.l @r15+,r13 */
L8c04064e: *stop=0x8c04064eu;if(*pr!=entry_pr)return -3;*stop=0x8c040650u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0406a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0406a0;
L8c0406a0: *stop=0x8c0406a0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0406a2; /* mov.l r14,@-r15 */
L8c0406a2: *stop=0x8c0406a2u;r[3]=0u;goto L8c0406a4; /* mov #0x0,r3 */
L8c0406a4: *stop=0x8c0406a4u;r[1]=rd(m,0x8c040844u,4);goto L8c0406a6; /* mov.l 0x8c040844,r1 */
L8c0406a6: *stop=0x8c0406a6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0406a8; /* mov.l r13,@-r15 */
L8c0406a8: *stop=0x8c0406a8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0406aa; /* mov.l r12,@-r15 */
L8c0406aa: *stop=0x8c0406aau;r[12]=r[4];goto L8c0406ac; /* mov r4,r12 */
L8c0406ac: *stop=0x8c0406acu;r[14]=rd(m,0x8c040848u,4);goto L8c0406ae; /* mov.l 0x8c040848,r14 */
L8c0406ae: *stop=0x8c0406aeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c0406b0; /* sts.l PR,@-r15 */
L8c0406b0: *stop=0x8c0406b0u;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c0406b2; /* mov.l @r14,r13 */
L8c0406b2: *stop=0x8c0406b2u;r[2]=r[13];goto L8c0406b4; /* mov r13,r2 */
L8c0406b4: *stop=0x8c0406b4u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c0406b6; /* mov.l r13,@(0x4,r14) */
L8c0406b6: *stop=0x8c0406b6u;r[2]+=12u;goto L8c0406b8; /* add #0xc,r2 */
L8c0406b8: *stop=0x8c0406b8u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[2],4);goto L8c0406ba; /* mov.l r2,@(0x8,r14) */
L8c0406ba: *stop=0x8c0406bau;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[3],4);goto L8c0406bc; /* mov.l r3,@(0x14,r14) */
L8c0406bc: *stop=0x8c0406bcu;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c0406be; /* mov.l @r1,r2 */
L8c0406be: target=r[2];*pr=0x8c0406c2u;*stop=0x8c0406c0u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0406c2; /* jsr @r2 */
L8c0406c2: *stop=0x8c0406c2u;r[2]=rd(m,0x8c04084cu,4);goto L8c0406c4; /* mov.l 0x8c04084c,r2 */
L8c0406c4: *stop=0x8c0406c4u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c0406c6; /* mov.l @r2,r3 */
L8c0406c6: target=r[3];*pr=0x8c0406cau;*stop=0x8c0406c8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0406ca; /* jsr @r3 */
L8c0406ca: *stop=0x8c0406cau;r[2]=rd(m,0x8c040850u,4);goto L8c0406cc; /* mov.l 0x8c040850,r2 */
L8c0406cc: *stop=0x8c0406ccu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c0406ce; /* mov.l @r2,r3 */
L8c0406ce: target=r[3];*pr=0x8c0406d2u;*stop=0x8c0406d0u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0406d2; /* jsr @r3 */
L8c0406d2: *stop=0x8c0406d2u;r[13]+=24u;goto L8c0406d4; /* add #0x18,r13 */
L8c0406d4: *stop=0x8c0406d4u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c0406d6; /* mov.l r13,@r14 */
L8c0406d6: *stop=0x8c0406d6u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c0406d8; /* lds.l @r15+,PR */
L8c0406d8: *stop=0x8c0406d8u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0406da; /* mov.l @r15+,r12 */
L8c0406da: *stop=0x8c0406dau;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0406dc; /* mov.l @r15+,r13 */
L8c0406dc: *stop=0x8c0406dcu;if(*pr!=entry_pr)return -3;*stop=0x8c0406deu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0406e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0406e0;
L8c0406e0: *stop=0x8c0406e0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0406e2; /* mov.l r14,@-r15 */
L8c0406e2: *stop=0x8c0406e2u;r[3]=0u;goto L8c0406e4; /* mov #0x0,r3 */
L8c0406e4: *stop=0x8c0406e4u;r[1]=rd(m,0x8c040844u,4);goto L8c0406e6; /* mov.l 0x8c040844,r1 */
L8c0406e6: *stop=0x8c0406e6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0406e8; /* mov.l r13,@-r15 */
L8c0406e8: *stop=0x8c0406e8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0406ea; /* mov.l r12,@-r15 */
L8c0406ea: *stop=0x8c0406eau;r[12]=r[4];goto L8c0406ec; /* mov r4,r12 */
L8c0406ec: *stop=0x8c0406ecu;r[14]=rd(m,0x8c040848u,4);goto L8c0406ee; /* mov.l 0x8c040848,r14 */
L8c0406ee: *stop=0x8c0406eeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c0406f0; /* sts.l PR,@-r15 */
L8c0406f0: *stop=0x8c0406f0u;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c0406f2; /* mov.l @r14,r13 */
L8c0406f2: *stop=0x8c0406f2u;r[2]=r[13];goto L8c0406f4; /* mov r13,r2 */
L8c0406f4: *stop=0x8c0406f4u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c0406f6; /* mov.l r13,@(0x4,r14) */
L8c0406f6: *stop=0x8c0406f6u;r[2]+=16u;goto L8c0406f8; /* add #0x10,r2 */
L8c0406f8: *stop=0x8c0406f8u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[2],4);goto L8c0406fa; /* mov.l r2,@(0x8,r14) */
L8c0406fa: *stop=0x8c0406fau;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[3],4);goto L8c0406fc; /* mov.l r3,@(0x14,r14) */
L8c0406fc: *stop=0x8c0406fcu;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c0406fe; /* mov.l @r1,r2 */
L8c0406fe: target=r[2];*pr=0x8c040702u;*stop=0x8c040700u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040702; /* jsr @r2 */
L8c040702: *stop=0x8c040702u;r[2]=rd(m,0x8c04084cu,4);goto L8c040704; /* mov.l 0x8c04084c,r2 */
L8c040704: *stop=0x8c040704u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c040706; /* mov.l @r2,r3 */
L8c040706: target=r[3];*pr=0x8c04070au;*stop=0x8c040708u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04070a; /* jsr @r3 */
L8c04070a: *stop=0x8c04070au;r[2]=rd(m,0x8c040850u,4);goto L8c04070c; /* mov.l 0x8c040850,r2 */
L8c04070c: *stop=0x8c04070cu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c04070e; /* mov.l @r2,r3 */
L8c04070e: target=r[3];*pr=0x8c040712u;*stop=0x8c040710u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040712; /* jsr @r3 */
L8c040712: *stop=0x8c040712u;r[13]+=32u;goto L8c040714; /* add #0x20,r13 */
L8c040714: *stop=0x8c040714u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c040716; /* mov.l r13,@r14 */
L8c040716: *stop=0x8c040716u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c040718; /* lds.l @r15+,PR */
L8c040718: *stop=0x8c040718u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c04071a; /* mov.l @r15+,r12 */
L8c04071a: *stop=0x8c04071au;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c04071c; /* mov.l @r15+,r13 */
L8c04071c: *stop=0x8c04071cu;if(*pr!=entry_pr)return -3;*stop=0x8c04071eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040720(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040720;
L8c040720: *stop=0x8c040720u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040722; /* mov.l r14,@-r15 */
L8c040722: *stop=0x8c040722u;r[14]=r[4];goto L8c040724; /* mov r4,r14 */
L8c040724: *stop=0x8c040724u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c040726; /* mov.l @(0x4,r14),r3 */
L8c040726: *stop=0x8c040726u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c040728; /* tst r3,r3 */
L8c040728: if((*sr&1u))goto L8c040738;goto L8c04072a; /* bt 0x8c040738 */
L8c04072a: *stop=0x8c04072au;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c04072c; /* mov.l @r14,r0 */
L8c04072c: *stop=0x8c04072cu;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c04072e; /* tst #0x8,r0 */
L8c04072e: if(!(*sr&1u))goto L8c040738;goto L8c040730; /* bf 0x8c040738 */
L8c040730: *stop=0x8c040730u;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);goto L8c040732; /* mov.l @(0x4,r14),r4 */
L8c040732: *stop=0x8c040732u;r[2]=rd(m,0x8c040854u,4);goto L8c040734; /* mov.l 0x8c040854,r2 */
L8c040734: target=r[2];*stop=0x8c040736u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c040738: *stop=0x8c040738u;if(*pr!=entry_pr)return -3;*stop=0x8c04073au;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c04073c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c04073c;
L8c04073c: *stop=0x8c04073cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c04073e; /* mov.l r14,@-r15 */
L8c04073e: *stop=0x8c04073eu;r[3]=0u;goto L8c040740; /* mov #0x0,r3 */
L8c040740: *stop=0x8c040740u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c040742; /* mov.l r13,@-r15 */
L8c040742: *stop=0x8c040742u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c040744; /* sts.l PR,@-r15 */
L8c040744: *stop=0x8c040744u;r[14]=rd(m,0x8c040858u,4);goto L8c040746; /* mov.l 0x8c040858,r14 */
L8c040746: *stop=0x8c040746u;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c040748; /* mov.l @r14,r13 */
L8c040748: *stop=0x8c040748u;r[2]=r[13];goto L8c04074a; /* mov r13,r2 */
L8c04074a: *stop=0x8c04074au;r[2]+=4u;goto L8c04074c; /* add #0x4,r2 */
L8c04074c: *stop=0x8c04074cu;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c04074e; /* mov.l r13,@(0x4,r14) */
L8c04074e: *stop=0x8c04074eu;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[2],4);goto L8c040750; /* mov.l r2,@(0x8,r14) */
L8c040750: target=0x8c040498u;*pr=0x8c040754u;*stop=0x8c040752u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[3],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040754; /* bsr 0x8c040498 */
L8c040754: *stop=0x8c040754u;r[13]+=8u;goto L8c040756; /* add #0x8,r13 */
L8c040756: *stop=0x8c040756u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c040758; /* mov.l r13,@r14 */
L8c040758: *stop=0x8c040758u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c04075a; /* lds.l @r15+,PR */
L8c04075a: *stop=0x8c04075au;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c04075c; /* mov.l @r15+,r13 */
L8c04075c: *stop=0x8c04075cu;if(*pr!=entry_pr)return -3;*stop=0x8c04075eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040760(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040760;
L8c040760: *stop=0x8c040760u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040762; /* mov.l r14,@-r15 */
L8c040762: *stop=0x8c040762u;r[3]=0u;goto L8c040764; /* mov #0x0,r3 */
L8c040764: *stop=0x8c040764u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c040766; /* mov.l r13,@-r15 */
L8c040766: *stop=0x8c040766u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c040768; /* sts.l PR,@-r15 */
L8c040768: *stop=0x8c040768u;r[14]=rd(m,0x8c040858u,4);goto L8c04076a; /* mov.l 0x8c040858,r14 */
L8c04076a: *stop=0x8c04076au;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c04076c; /* mov.l @r14,r13 */
L8c04076c: *stop=0x8c04076cu;r[2]=r[13];goto L8c04076e; /* mov r13,r2 */
L8c04076e: *stop=0x8c04076eu;r[2]+=8u;goto L8c040770; /* add #0x8,r2 */
L8c040770: *stop=0x8c040770u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c040772; /* mov.l r13,@(0x4,r14) */
L8c040772: *stop=0x8c040772u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[2],4);goto L8c040774; /* mov.l r2,@(0x8,r14) */
L8c040774: target=0x8c040498u;*pr=0x8c040778u;*stop=0x8c040776u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[3],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040778; /* bsr 0x8c040498 */
L8c040778: *stop=0x8c040778u;r[13]+=16u;goto L8c04077a; /* add #0x10,r13 */
L8c04077a: *stop=0x8c04077au;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c04077c; /* mov.l r13,@r14 */
L8c04077c: *stop=0x8c04077cu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c04077e; /* lds.l @r15+,PR */
L8c04077e: *stop=0x8c04077eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c040780; /* mov.l @r15+,r13 */
L8c040780: *stop=0x8c040780u;if(*pr!=entry_pr)return -3;*stop=0x8c040782u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040df0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040df0;
L8c040df0: *stop=0x8c040df0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040df2; /* mov.l r14,@-r15 */
L8c040df2: *stop=0x8c040df2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c040df4; /* mov.l r13,@-r15 */
L8c040df4: *stop=0x8c040df4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c040df6; /* mov.l r12,@-r15 */
L8c040df6: *stop=0x8c040df6u;r[13]=28u;goto L8c040df8; /* mov #0x1c,r13 */
L8c040df8: *stop=0x8c040df8u;r[3]=rd(m,0x8c04100cu,4);goto L8c040dfa; /* mov.l 0x8c04100c,r3 */
L8c040dfa: *stop=0x8c040dfau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c040dfc; /* mov.l r11,@-r15 */
L8c040dfc: *stop=0x8c040dfcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c040dfe; /* mov.l r10,@-r15 */
L8c040dfe: *stop=0x8c040dfeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c040e00; /* sts.l PR,@-r15 */
L8c040e00: *stop=0x8c040e00u;r[2]=rd(m,0x8c041004u,4);goto L8c040e02; /* mov.l 0x8c041004,r2 */
L8c040e02: *stop=0x8c040e02u;r[15]+=4294967292u;goto L8c040e04; /* add #-0x4,r15 */
L8c040e04: *stop=0x8c040e04u;r[0]=rd(m,0x8c041010u,4);goto L8c040e06; /* mov.l 0x8c041010,r0 */
L8c040e06: *stop=0x8c040e06u;if(!valid_ram(r[2],2u))return -3;r[5]=sx(m,r[2],2);goto L8c040e08; /* mov.w @r2,r5 */
L8c040e08: *stop=0x8c040e08u;*macl=(uint32_t)((int32_t)(int16_t)r[13]*(int32_t)(int16_t)r[5]);goto L8c040e0a; /* muls.w r13,r5 */
L8c040e0a: *stop=0x8c040e0au;r[14]=r[5]&65535u;goto L8c040e0c; /* extu.w r5,r14 */
L8c040e0c: *stop=0x8c040e0cu;r[13]=*macl;goto L8c040e0e; /* sts MACL,r13 */
L8c040e0e: *stop=0x8c040e0eu;r[13]=(uint32_t)(int32_t)(int8_t)r[13];goto L8c040e10; /* exts.b r13,r13 */
L8c040e10: *stop=0x8c040e10u;r[13]+=r[3];goto L8c040e12; /* add r3,r13 */
L8c040e12: *stop=0x8c040e12u;r[1]=r[14];goto L8c040e14; /* mov r14,r1 */
L8c040e14: *stop=0x8c040e14u;if(!valid_ram(r[13]+20u,4u))return -3;r[12]=rd(m,r[13]+0x14u,4);goto L8c040e16; /* mov.l @(0x14,r13),r12 */
L8c040e16: *stop=0x8c040e16u;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c040e18; /* shll r14 */
L8c040e18: *stop=0x8c040e18u;r[14]+=r[1];goto L8c040e1a; /* add r1,r14 */
L8c040e1a: *stop=0x8c040e1au;r[14]<<=2;goto L8c040e1c; /* shll2 r14 */
L8c040e1c: *stop=0x8c040e1cu;r[14]+=r[0];goto L8c040e1e; /* add r0,r14 */
L8c040e1e: *stop=0x8c040e1eu;r[5]=r[12];goto L8c040e20; /* mov r12,r5 */
L8c040e20: *stop=0x8c040e20u;if(!valid_ram(r[13]+4u,4u))return -3;r[0]=rd(m,r[13]+0x4u,4);goto L8c040e22; /* mov.l @(0x4,r13),r0 */
L8c040e22: *stop=0x8c040e22u;r[5]<<=2;goto L8c040e24; /* shll2 r5 */
L8c040e24: *stop=0x8c040e24u;if(!valid_ram(r[0]+r[5],4u))return -3;r[11]=rd(m,r[5]+r[0],4);goto L8c040e26; /* mov.l @(r0,r5),r11 */
L8c040e26: *stop=0x8c040e26u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c040e28; /* tst r11,r11 */
L8c040e28: if((*sr&1u))goto L8c040e50;goto L8c040e2a; /* bt 0x8c040e50 */
L8c040e2a: *stop=0x8c040e2au;r[6]=r[15];goto L8c040e2c; /* mov r15,r6 */
L8c040e2c: *stop=0x8c040e2cu;if(!valid_ram(r[13]+8u,4u))return -3;r[0]=rd(m,r[13]+0x8u,4);goto L8c040e2e; /* mov.l @(0x8,r13),r0 */
L8c040e2e: *stop=0x8c040e2eu;if(!valid_ram(r[0]+r[5],4u))return -3;r[10]=rd(m,r[5]+r[0],4);goto L8c040e30; /* mov.l @(r0,r5),r10 */
L8c040e30: *stop=0x8c040e30u;r[5]=r[10];goto L8c040e32; /* mov r10,r5 */
L8c040e32: target=0x8c040a60u;*pr=0x8c040e36u;*stop=0x8c040e34u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040e36; /* bsr 0x8c040a60 */
L8c040e36: *stop=0x8c040e36u;r[7]=r[14];goto L8c040e38; /* mov r14,r7 */
L8c040e38: *stop=0x8c040e38u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c040e3a; /* mov.l @r15,r5 */
L8c040e3a: *stop=0x8c040e3au;r[6]=r[10];goto L8c040e3c; /* mov r10,r6 */
L8c040e3c: target=0x8c040c5cu;*pr=0x8c040e40u;*stop=0x8c040e3eu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040e40; /* bsr 0x8c040c5c */
L8c040e40: *stop=0x8c040e40u;r[4]=rd(m,0x8c041014u,4);goto L8c040e42; /* mov.l 0x8c041014,r4 */
L8c040e42: *stop=0x8c040e42u;r[3]=rd(m,0x8c041004u,4);goto L8c040e44; /* mov.l 0x8c041004,r3 */
L8c040e44: *stop=0x8c040e44u;if(!valid_ram(r[4],2u))return -3;r[1]=sx(m,r[4],2);goto L8c040e46; /* mov.w @r4,r1 */
L8c040e46: *stop=0x8c040e46u;if(!valid_ram(r[3],2u))return -3;r[2]=sx(m,r[3],2);goto L8c040e48; /* mov.w @r3,r2 */
L8c040e48: *stop=0x8c040e48u;r[2]+=1u;goto L8c040e4a; /* add #0x1,r2 */
L8c040e4a: *stop=0x8c040e4au;r[1]|=r[2];goto L8c040e4c; /* or r2,r1 */
L8c040e4c: *stop=0x8c040e4eu;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[1],2);goto L8c040e60; /* bra 0x8c040e60 */
L8c040e50: *stop=0x8c040e50u;r[4]+=8u;goto L8c040e52; /* add #0x8,r4 */
L8c040e52: *stop=0x8c040e52u;r[14]+=12u;goto L8c040e54; /* add #0xc,r14 */
L8c040e54: *stop=0x8c040e54u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040e56; /* fmov.s @r4+,fr6 */
L8c040e56: *stop=0x8c040e56u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040e58; /* fmov.s @r4+,fr5 */
L8c040e58: *stop=0x8c040e58u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);goto L8c040e5a; /* fmov.s @r4,fr4 */
L8c040e5a: *stop=0x8c040e5au;if(!valid_ram(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[14]);goto L8c040e5c; /* fmov.s fr4,@-r14 */
L8c040e5c: *stop=0x8c040e5cu;if(!valid_ram(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[14]);goto L8c040e5e; /* fmov.s fr5,@-r14 */
L8c040e5e: *stop=0x8c040e5eu;if(!valid_ram(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[14]);goto L8c040e60; /* fmov.s fr6,@-r14 */
L8c040e60: *stop=0x8c040e60u;r[12]+=1u;goto L8c040e62; /* add #0x1,r12 */
L8c040e62: *stop=0x8c040e62u;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[12],4);goto L8c040e64; /* mov.l r12,@(0x14,r13) */
L8c040e64: *stop=0x8c040e64u;r[15]+=4u;goto L8c040e66; /* add #0x4,r15 */
L8c040e66: *stop=0x8c040e66u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c040e68; /* lds.l @r15+,PR */
L8c040e68: *stop=0x8c040e68u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c040e6a; /* mov.l @r15+,r10 */
L8c040e6a: *stop=0x8c040e6au;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c040e6c; /* mov.l @r15+,r11 */
L8c040e6c: *stop=0x8c040e6cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c040e6e; /* mov.l @r15+,r12 */
L8c040e6e: *stop=0x8c040e6eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c040e70; /* mov.l @r15+,r13 */
L8c040e70: *stop=0x8c040e70u;if(*pr!=entry_pr)return -3;*stop=0x8c040e72u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040e74(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040e74;
L8c040e74: *stop=0x8c040e74u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040e76; /* mov.l r14,@-r15 */
L8c040e76: *stop=0x8c040e76u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c040e78; /* mov.l r13,@-r15 */
L8c040e78: *stop=0x8c040e78u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c040e7a; /* mov.l r12,@-r15 */
L8c040e7a: *stop=0x8c040e7au;r[13]=28u;goto L8c040e7c; /* mov #0x1c,r13 */
L8c040e7c: *stop=0x8c040e7cu;r[2]=rd(m,0x8c04100cu,4);goto L8c040e7e; /* mov.l 0x8c04100c,r2 */
L8c040e7e: *stop=0x8c040e7eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c040e80; /* mov.l r11,@-r15 */
L8c040e80: *stop=0x8c040e80u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c040e82; /* mov.l r10,@-r15 */
L8c040e82: *stop=0x8c040e82u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c040e84; /* sts.l PR,@-r15 */
L8c040e84: *stop=0x8c040e84u;r[3]=rd(m,0x8c041004u,4);goto L8c040e86; /* mov.l 0x8c041004,r3 */
L8c040e86: *stop=0x8c040e86u;r[15]+=4294967292u;goto L8c040e88; /* add #-0x4,r15 */
L8c040e88: *stop=0x8c040e88u;r[0]=rd(m,0x8c041018u,4);goto L8c040e8a; /* mov.l 0x8c041018,r0 */
L8c040e8a: *stop=0x8c040e8au;if(!valid_ram(r[3],2u))return -3;r[5]=sx(m,r[3],2);goto L8c040e8c; /* mov.w @r3,r5 */
L8c040e8c: *stop=0x8c040e8cu;*macl=(uint32_t)((int32_t)(int16_t)r[13]*(int32_t)(int16_t)r[5]);goto L8c040e8e; /* muls.w r13,r5 */
L8c040e8e: *stop=0x8c040e8eu;r[14]=r[5]&65535u;goto L8c040e90; /* extu.w r5,r14 */
L8c040e90: *stop=0x8c040e90u;r[13]=*macl;goto L8c040e92; /* sts MACL,r13 */
L8c040e92: *stop=0x8c040e92u;r[13]=(uint32_t)(int32_t)(int8_t)r[13];goto L8c040e94; /* exts.b r13,r13 */
L8c040e94: *stop=0x8c040e94u;r[13]+=r[2];goto L8c040e96; /* add r2,r13 */
L8c040e96: *stop=0x8c040e96u;r[1]=r[14];goto L8c040e98; /* mov r14,r1 */
L8c040e98: *stop=0x8c040e98u;if(!valid_ram(r[13]+20u,4u))return -3;r[12]=rd(m,r[13]+0x14u,4);goto L8c040e9a; /* mov.l @(0x14,r13),r12 */
L8c040e9a: *stop=0x8c040e9au;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c040e9c; /* shll r14 */
L8c040e9c: *stop=0x8c040e9cu;r[14]+=r[1];goto L8c040e9e; /* add r1,r14 */
L8c040e9e: *stop=0x8c040e9eu;r[14]<<=2;goto L8c040ea0; /* shll2 r14 */
L8c040ea0: *stop=0x8c040ea0u;r[14]+=r[0];goto L8c040ea2; /* add r0,r14 */
L8c040ea2: *stop=0x8c040ea2u;r[5]=r[12];goto L8c040ea4; /* mov r12,r5 */
L8c040ea4: *stop=0x8c040ea4u;if(!valid_ram(r[13]+4u,4u))return -3;r[0]=rd(m,r[13]+0x4u,4);goto L8c040ea6; /* mov.l @(0x4,r13),r0 */
L8c040ea6: *stop=0x8c040ea6u;r[5]<<=2;goto L8c040ea8; /* shll2 r5 */
L8c040ea8: *stop=0x8c040ea8u;if(!valid_ram(r[0]+r[5],4u))return -3;r[11]=rd(m,r[5]+r[0],4);goto L8c040eaa; /* mov.l @(r0,r5),r11 */
L8c040eaa: *stop=0x8c040eaau;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c040eac; /* tst r11,r11 */
L8c040eac: if((*sr&1u))goto L8c040ed4;goto L8c040eae; /* bt 0x8c040ed4 */
L8c040eae: *stop=0x8c040eaeu;r[6]=r[15];goto L8c040eb0; /* mov r15,r6 */
L8c040eb0: *stop=0x8c040eb0u;if(!valid_ram(r[13]+8u,4u))return -3;r[0]=rd(m,r[13]+0x8u,4);goto L8c040eb2; /* mov.l @(0x8,r13),r0 */
L8c040eb2: *stop=0x8c040eb2u;if(!valid_ram(r[0]+r[5],4u))return -3;r[10]=rd(m,r[5]+r[0],4);goto L8c040eb4; /* mov.l @(r0,r5),r10 */
L8c040eb4: *stop=0x8c040eb4u;r[5]=r[10];goto L8c040eb6; /* mov r10,r5 */
L8c040eb6: target=0x8c040a60u;*pr=0x8c040ebau;*stop=0x8c040eb8u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040eba; /* bsr 0x8c040a60 */
L8c040eba: *stop=0x8c040ebau;r[7]=r[14];goto L8c040ebc; /* mov r14,r7 */
L8c040ebc: *stop=0x8c040ebcu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c040ebe; /* mov.l @r15,r5 */
L8c040ebe: *stop=0x8c040ebeu;r[6]=r[10];goto L8c040ec0; /* mov r10,r6 */
L8c040ec0: target=0x8c040c5cu;*pr=0x8c040ec4u;*stop=0x8c040ec2u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040ec4; /* bsr 0x8c040c5c */
L8c040ec4: *stop=0x8c040ec4u;r[4]=rd(m,0x8c04101cu,4);goto L8c040ec6; /* mov.l 0x8c04101c,r4 */
L8c040ec6: *stop=0x8c040ec6u;r[3]=rd(m,0x8c041004u,4);goto L8c040ec8; /* mov.l 0x8c041004,r3 */
L8c040ec8: *stop=0x8c040ec8u;if(!valid_ram(r[4],2u))return -3;r[1]=sx(m,r[4],2);goto L8c040eca; /* mov.w @r4,r1 */
L8c040eca: *stop=0x8c040ecau;if(!valid_ram(r[3],2u))return -3;r[2]=sx(m,r[3],2);goto L8c040ecc; /* mov.w @r3,r2 */
L8c040ecc: *stop=0x8c040eccu;r[2]+=1u;goto L8c040ece; /* add #0x1,r2 */
L8c040ece: *stop=0x8c040eceu;r[1]|=r[2];goto L8c040ed0; /* or r2,r1 */
L8c040ed0: *stop=0x8c040ed2u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[1],2);goto L8c040ee4; /* bra 0x8c040ee4 */
L8c040ed4: *stop=0x8c040ed4u;r[4]+=32u;goto L8c040ed6; /* add #0x20,r4 */
L8c040ed6: *stop=0x8c040ed6u;r[14]+=12u;goto L8c040ed8; /* add #0xc,r14 */
L8c040ed8: *stop=0x8c040ed8u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040eda; /* fmov.s @r4+,fr6 */
L8c040eda: *stop=0x8c040edau;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040edc; /* fmov.s @r4+,fr5 */
L8c040edc: *stop=0x8c040edcu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);goto L8c040ede; /* fmov.s @r4,fr4 */
L8c040ede: *stop=0x8c040edeu;if(!valid_ram(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[14]);goto L8c040ee0; /* fmov.s fr4,@-r14 */
L8c040ee0: *stop=0x8c040ee0u;if(!valid_ram(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[14]);goto L8c040ee2; /* fmov.s fr5,@-r14 */
L8c040ee2: *stop=0x8c040ee2u;if(!valid_ram(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[14]);goto L8c040ee4; /* fmov.s fr6,@-r14 */
L8c040ee4: *stop=0x8c040ee4u;r[12]+=1u;goto L8c040ee6; /* add #0x1,r12 */
L8c040ee6: *stop=0x8c040ee6u;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[12],4);goto L8c040ee8; /* mov.l r12,@(0x14,r13) */
L8c040ee8: *stop=0x8c040ee8u;r[15]+=4u;goto L8c040eea; /* add #0x4,r15 */
L8c040eea: *stop=0x8c040eeau;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c040eec; /* lds.l @r15+,PR */
L8c040eec: *stop=0x8c040eecu;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c040eee; /* mov.l @r15+,r10 */
L8c040eee: *stop=0x8c040eeeu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c040ef0; /* mov.l @r15+,r11 */
L8c040ef0: *stop=0x8c040ef0u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c040ef2; /* mov.l @r15+,r12 */
L8c040ef2: *stop=0x8c040ef2u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c040ef4; /* mov.l @r15+,r13 */
L8c040ef4: *stop=0x8c040ef4u;if(*pr!=entry_pr)return -3;*stop=0x8c040ef6u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c040ef8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040ef8;
L8c040ef8: *stop=0x8c040ef8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040efa; /* mov.l r14,@-r15 */
L8c040efa: *stop=0x8c040efau;r[14]=28u;goto L8c040efc; /* mov #0x1c,r14 */
L8c040efc: *stop=0x8c040efcu;r[2]=rd(m,0x8c04100cu,4);goto L8c040efe; /* mov.l 0x8c04100c,r2 */
L8c040efe: *stop=0x8c040efeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c040f00; /* mov.l r13,@-r15 */
L8c040f00: *stop=0x8c040f00u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c040f02; /* mov.l r12,@-r15 */
L8c040f02: *stop=0x8c040f02u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c040f04; /* mov.l r11,@-r15 */
L8c040f04: *stop=0x8c040f04u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c040f06; /* mov.l r10,@-r15 */
L8c040f06: *stop=0x8c040f06u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c040f08; /* sts.l PR,@-r15 */
L8c040f08: *stop=0x8c040f08u;r[3]=rd(m,0x8c041004u,4);goto L8c040f0a; /* mov.l 0x8c041004,r3 */
L8c040f0a: *stop=0x8c040f0au;r[15]+=4294967292u;goto L8c040f0c; /* add #-0x4,r15 */
L8c040f0c: *stop=0x8c040f0cu;r[0]=rd(m,0x8c041020u,4);goto L8c040f0e; /* mov.l 0x8c041020,r0 */
L8c040f0e: *stop=0x8c040f0eu;if(!valid_ram(r[3],2u))return -3;r[5]=sx(m,r[3],2);goto L8c040f10; /* mov.w @r3,r5 */
L8c040f10: *stop=0x8c040f10u;*macl=(uint32_t)((int32_t)(int16_t)r[14]*(int32_t)(int16_t)r[5]);goto L8c040f12; /* muls.w r14,r5 */
L8c040f12: *stop=0x8c040f12u;r[13]=r[5]&65535u;goto L8c040f14; /* extu.w r5,r13 */
L8c040f14: *stop=0x8c040f14u;r[14]=*macl;goto L8c040f16; /* sts MACL,r14 */
L8c040f16: *stop=0x8c040f16u;r[14]=(uint32_t)(int32_t)(int8_t)r[14];goto L8c040f18; /* exts.b r14,r14 */
L8c040f18: *stop=0x8c040f18u;r[14]+=r[2];goto L8c040f1a; /* add r2,r14 */
L8c040f1a: *stop=0x8c040f1au;r[1]=r[13];goto L8c040f1c; /* mov r13,r1 */
L8c040f1c: *stop=0x8c040f1cu;if(!valid_ram(r[14]+20u,4u))return -3;r[10]=rd(m,r[14]+0x14u,4);goto L8c040f1e; /* mov.l @(0x14,r14),r10 */
L8c040f1e: *stop=0x8c040f1eu;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c040f20; /* shll r13 */
L8c040f20: *stop=0x8c040f20u;r[13]+=r[1];goto L8c040f22; /* add r1,r13 */
L8c040f22: *stop=0x8c040f22u;r[13]<<=2;goto L8c040f24; /* shll2 r13 */
L8c040f24: *stop=0x8c040f24u;r[13]+=r[0];goto L8c040f26; /* add r0,r13 */
L8c040f26: *stop=0x8c040f26u;r[5]=r[10];goto L8c040f28; /* mov r10,r5 */
L8c040f28: *stop=0x8c040f28u;if(!valid_ram(r[14]+4u,4u))return -3;r[0]=rd(m,r[14]+0x4u,4);goto L8c040f2a; /* mov.l @(0x4,r14),r0 */
L8c040f2a: *stop=0x8c040f2au;r[5]<<=2;goto L8c040f2c; /* shll2 r5 */
L8c040f2c: *stop=0x8c040f2cu;if(!valid_ram(r[0]+r[5],4u))return -3;r[12]=rd(m,r[5]+r[0],4);goto L8c040f2e; /* mov.l @(r0,r5),r12 */
L8c040f2e: *stop=0x8c040f2eu;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c040f30; /* tst r12,r12 */
L8c040f30: if((*sr&1u))goto L8c040f58;goto L8c040f32; /* bt 0x8c040f58 */
L8c040f32: *stop=0x8c040f32u;r[6]=r[15];goto L8c040f34; /* mov r15,r6 */
L8c040f34: *stop=0x8c040f34u;if(!valid_ram(r[14]+8u,4u))return -3;r[0]=rd(m,r[14]+0x8u,4);goto L8c040f36; /* mov.l @(0x8,r14),r0 */
L8c040f36: *stop=0x8c040f36u;if(!valid_ram(r[0]+r[5],4u))return -3;r[11]=rd(m,r[5]+r[0],4);goto L8c040f38; /* mov.l @(r0,r5),r11 */
L8c040f38: *stop=0x8c040f38u;r[5]=r[11];goto L8c040f3a; /* mov r11,r5 */
L8c040f3a: target=0x8c040a60u;*pr=0x8c040f3eu;*stop=0x8c040f3cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040f3e; /* bsr 0x8c040a60 */
L8c040f3e: *stop=0x8c040f3eu;r[7]=r[13];goto L8c040f40; /* mov r13,r7 */
L8c040f40: *stop=0x8c040f40u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c040f42; /* mov.l @r15,r5 */
L8c040f42: *stop=0x8c040f42u;r[6]=r[11];goto L8c040f44; /* mov r11,r6 */
L8c040f44: target=0x8c040d60u;*pr=0x8c040f48u;*stop=0x8c040f46u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040f48; /* bsr 0x8c040d60 */
L8c040f48: *stop=0x8c040f48u;r[4]=rd(m,0x8c041024u,4);goto L8c040f4a; /* mov.l 0x8c041024,r4 */
L8c040f4a: *stop=0x8c040f4au;r[3]=rd(m,0x8c041004u,4);goto L8c040f4c; /* mov.l 0x8c041004,r3 */
L8c040f4c: *stop=0x8c040f4cu;if(!valid_ram(r[4],2u))return -3;r[1]=sx(m,r[4],2);goto L8c040f4e; /* mov.w @r4,r1 */
L8c040f4e: *stop=0x8c040f4eu;if(!valid_ram(r[3],2u))return -3;r[2]=sx(m,r[3],2);goto L8c040f50; /* mov.w @r3,r2 */
L8c040f50: *stop=0x8c040f50u;r[2]+=1u;goto L8c040f52; /* add #0x1,r2 */
L8c040f52: *stop=0x8c040f52u;r[1]|=r[2];goto L8c040f54; /* or r2,r1 */
L8c040f54: *stop=0x8c040f56u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[1],2);goto L8c040f66; /* bra 0x8c040f66 */
L8c040f58: *stop=0x8c040f58u;r[4]+=20u;goto L8c040f5a; /* add #0x14,r4 */
L8c040f5a: *stop=0x8c040f5au;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c040f5c; /* mov.l @r4,r3 */
L8c040f5c: *stop=0x8c040f5cu;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[3],4);goto L8c040f5e; /* mov.l r3,@r13 */
L8c040f5e: *stop=0x8c040f5eu;if(!valid_ram(r[4]+4u,4u))return -3;r[1]=rd(m,r[4]+0x4u,4);goto L8c040f60; /* mov.l @(0x4,r4),r1 */
L8c040f60: *stop=0x8c040f60u;if(!valid_ram(r[13]+4u,4u))return -3;wr(m,r[13]+0x4u,r[1],4);goto L8c040f62; /* mov.l r1,@(0x4,r13) */
L8c040f62: *stop=0x8c040f62u;if(!valid_ram(r[4]+8u,4u))return -3;r[3]=rd(m,r[4]+0x8u,4);goto L8c040f64; /* mov.l @(0x8,r4),r3 */
L8c040f64: *stop=0x8c040f64u;if(!valid_ram(r[13]+8u,4u))return -3;wr(m,r[13]+0x8u,r[3],4);goto L8c040f66; /* mov.l r3,@(0x8,r13) */
L8c040f66: *stop=0x8c040f66u;r[10]+=1u;goto L8c040f68; /* add #0x1,r10 */
L8c040f68: *stop=0x8c040f68u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[10],4);goto L8c040f6a; /* mov.l r10,@(0x14,r14) */
L8c040f6a: *stop=0x8c040f6au;r[15]+=4u;goto L8c040f6c; /* add #0x4,r15 */
L8c040f6c: *stop=0x8c040f6cu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c040f6e; /* lds.l @r15+,PR */
L8c040f6e: *stop=0x8c040f6eu;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c040f70; /* mov.l @r15+,r10 */
L8c040f70: *stop=0x8c040f70u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c040f72; /* mov.l @r15+,r11 */
L8c040f72: *stop=0x8c040f72u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c040f74; /* mov.l @r15+,r12 */
L8c040f74: *stop=0x8c040f74u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c040f76; /* mov.l @r15+,r13 */
L8c040f76: *stop=0x8c040f76u;if(*pr!=entry_pr)return -3;*stop=0x8c040f78u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0414a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0414a6;
L8c0414a6: *stop=0x8c0414a6u;r[5]=r[4];goto L8c0414a8; /* mov r4,r5 */
L8c0414a8: *stop=0x8c0414a8u;r[3]=rd(m,0x8c0415c4u,4);goto L8c0414aa; /* mov.l 0x8c0415c4,r3 */
L8c0414aa: *stop=0x8c0414aau;r[5]+=8u;goto L8c0414ac; /* add #0x8,r5 */
L8c0414ac: *stop=0x8c0414acu;r[0]=rd(m,0x8c0415dcu,4);goto L8c0414ae; /* mov.l 0x8c0415dc,r0 */
L8c0414ae: *stop=0x8c0414aeu;if(!valid_ram(r[3],2u))return -3;r[4]=sx(m,r[3],2);goto L8c0414b0; /* mov.w @r3,r4 */
L8c0414b0: *stop=0x8c0414b0u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0414b2; /* fmov.s @r5+,fr6 */
L8c0414b2: *stop=0x8c0414b2u;r[4]=r[4]&65535u;goto L8c0414b4; /* extu.w r4,r4 */
L8c0414b4: *stop=0x8c0414b4u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0414b6; /* fmov.s @r5+,fr5 */
L8c0414b6: *stop=0x8c0414b6u;r[2]=r[4];goto L8c0414b8; /* mov r4,r2 */
L8c0414b8: *stop=0x8c0414b8u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[5]);goto L8c0414ba; /* fmov.s @r5,fr4 */
L8c0414ba: *stop=0x8c0414bau;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c0414bc; /* shll r4 */
L8c0414bc: *stop=0x8c0414bcu;r[4]+=r[2];goto L8c0414be; /* add r2,r4 */
L8c0414be: *stop=0x8c0414beu;r[4]<<=2;goto L8c0414c0; /* shll2 r4 */
L8c0414c0: *stop=0x8c0414c0u;r[4]+=r[0];goto L8c0414c2; /* add r0,r4 */
L8c0414c2: *stop=0x8c0414c2u;r[4]+=12u;goto L8c0414c4; /* add #0xc,r4 */
L8c0414c4: *stop=0x8c0414c4u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[4]);goto L8c0414c6; /* fmov.s fr4,@-r4 */
L8c0414c6: *stop=0x8c0414c6u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[4]);goto L8c0414c8; /* fmov.s fr5,@-r4 */
L8c0414c8: *stop=0x8c0414c8u;if(*pr!=entry_pr)return -3;*stop=0x8c0414cau;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0414cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0414cc;
L8c0414cc: *stop=0x8c0414ccu;r[3]=rd(m,0x8c0415c4u,4);goto L8c0414ce; /* mov.l 0x8c0415c4,r3 */
L8c0414ce: *stop=0x8c0414ceu;r[5]=r[4];goto L8c0414d0; /* mov r4,r5 */
L8c0414d0: *stop=0x8c0414d0u;r[1]=rd(m,0x8c0415e0u,4);goto L8c0414d2; /* mov.l 0x8c0415e0,r1 */
L8c0414d2: *stop=0x8c0414d2u;r[5]+=20u;goto L8c0414d4; /* add #0x14,r5 */
L8c0414d4: *stop=0x8c0414d4u;if(!valid_ram(r[3],2u))return -3;r[4]=sx(m,r[3],2);goto L8c0414d6; /* mov.w @r3,r4 */
L8c0414d6: *stop=0x8c0414d6u;r[4]=r[4]&65535u;goto L8c0414d8; /* extu.w r4,r4 */
L8c0414d8: *stop=0x8c0414d8u;if(!valid_ram(r[5],4u))return -3;r[6]=rd(m,r[5],4);goto L8c0414da; /* mov.l @r5,r6 */
L8c0414da: *stop=0x8c0414dau;r[2]=r[4];goto L8c0414dc; /* mov r4,r2 */
L8c0414dc: *stop=0x8c0414dcu;if(!valid_ram(r[5]+4u,4u))return -3;r[7]=rd(m,r[5]+0x4u,4);goto L8c0414de; /* mov.l @(0x4,r5),r7 */
L8c0414de: *stop=0x8c0414deu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c0414e0; /* shll r4 */
L8c0414e0: *stop=0x8c0414e0u;if(!valid_ram(r[5]+8u,4u))return -3;r[5]=rd(m,r[5]+0x8u,4);goto L8c0414e2; /* mov.l @(0x8,r5),r5 */
L8c0414e2: *stop=0x8c0414e2u;r[4]+=r[2];goto L8c0414e4; /* add r2,r4 */
L8c0414e4: *stop=0x8c0414e4u;r[4]<<=2;goto L8c0414e6; /* shll2 r4 */
L8c0414e6: *stop=0x8c0414e6u;r[4]+=r[1];goto L8c0414e8; /* add r1,r4 */
L8c0414e8: *stop=0x8c0414e8u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[6],4);goto L8c0414ea; /* mov.l r6,@r4 */
L8c0414ea: *stop=0x8c0414eau;if(!valid_ram(r[4]+4u,4u))return -3;wr(m,r[4]+0x4u,r[7],4);goto L8c0414ec; /* mov.l r7,@(0x4,r4) */
L8c0414ec: *stop=0x8c0414ecu;if(*pr!=entry_pr)return -3;*stop=0x8c0414eeu;if(!valid_ram(r[4]+8u,4u))return -3;wr(m,r[4]+0x8u,r[5],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0414f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0414f0;
L8c0414f0: *stop=0x8c0414f0u;r[3]=rd(m,0x8c0415c4u,4);goto L8c0414f2; /* mov.l 0x8c0415c4,r3 */
L8c0414f2: *stop=0x8c0414f2u;r[5]=r[4];goto L8c0414f4; /* mov r4,r5 */
L8c0414f4: *stop=0x8c0414f4u;r[0]=rd(m,0x8c0415e4u,4);goto L8c0414f6; /* mov.l 0x8c0415e4,r0 */
L8c0414f6: *stop=0x8c0414f6u;r[5]+=32u;goto L8c0414f8; /* add #0x20,r5 */
L8c0414f8: *stop=0x8c0414f8u;if(!valid_ram(r[3],2u))return -3;r[4]=sx(m,r[3],2);goto L8c0414fa; /* mov.w @r3,r4 */
L8c0414fa: *stop=0x8c0414fau;r[4]=r[4]&65535u;goto L8c0414fc; /* extu.w r4,r4 */
L8c0414fc: *stop=0x8c0414fcu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0414fe; /* fmov.s @r5+,fr6 */
L8c0414fe: *stop=0x8c0414feu;r[2]=r[4];goto L8c041500; /* mov r4,r2 */
L8c041500: *stop=0x8c041500u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c041502; /* fmov.s @r5+,fr5 */
L8c041502: *stop=0x8c041502u;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c041504; /* shll r4 */
L8c041504: *stop=0x8c041504u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[5]);goto L8c041506; /* fmov.s @r5,fr4 */
L8c041506: *stop=0x8c041506u;r[4]+=r[2];goto L8c041508; /* add r2,r4 */
L8c041508: *stop=0x8c041508u;r[4]<<=2;goto L8c04150a; /* shll2 r4 */
L8c04150a: *stop=0x8c04150au;r[4]+=r[0];goto L8c04150c; /* add r0,r4 */
L8c04150c: *stop=0x8c04150cu;r[4]+=12u;goto L8c04150e; /* add #0xc,r4 */
L8c04150e: *stop=0x8c04150eu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[4]);goto L8c041510; /* fmov.s fr4,@-r4 */
L8c041510: *stop=0x8c041510u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[4]);goto L8c041512; /* fmov.s fr5,@-r4 */
L8c041512: *stop=0x8c041512u;if(*pr!=entry_pr)return -3;*stop=0x8c041514u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c041516(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c041516;
L8c041516: *stop=0x8c041516u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c041518; /* mov.l r14,@-r15 */
L8c041518: *stop=0x8c041518u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c04151a; /* mov.l r13,@-r15 */
L8c04151a: *stop=0x8c04151au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c04151c; /* sts.l PR,@-r15 */
L8c04151c: *stop=0x8c04151cu;r[13]=rd(m,0x8c0415c4u,4);goto L8c04151e; /* mov.l 0x8c0415c4,r13 */
L8c04151e: *stop=0x8c04151eu;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c041520; /* mov.w @r13,r3 */
L8c041520: *stop=0x8c041520u;r[0]=rd(m,0x8c0415e8u,4);goto L8c041522; /* mov.l 0x8c0415e8,r0 */
L8c041522: *stop=0x8c041522u;r[3]=r[3]&65535u;goto L8c041524; /* extu.w r3,r3 */
L8c041524: *stop=0x8c041524u;r[3]<<=2;goto L8c041526; /* shll2 r3 */
L8c041526: *stop=0x8c041526u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c041528; /* mov.l @(r0,r3),r2 */
L8c041528: target=r[2];*pr=0x8c04152cu;*stop=0x8c04152au;r[14]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04152c; /* jsr @r2 */
L8c04152c: *stop=0x8c04152cu;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c04152e; /* mov.w @r13,r3 */
L8c04152e: *stop=0x8c04152eu;r[3]=r[3]&65535u;goto L8c041530; /* extu.w r3,r3 */
L8c041530: *stop=0x8c041530u;r[0]=rd(m,0x8c0415ecu,4);goto L8c041532; /* mov.l 0x8c0415ec,r0 */
L8c041532: *stop=0x8c041532u;r[3]<<=2;goto L8c041534; /* shll2 r3 */
L8c041534: *stop=0x8c041534u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c041536; /* mov.l @(r0,r3),r2 */
L8c041536: target=r[2];*pr=0x8c04153au;*stop=0x8c041538u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04153a; /* jsr @r2 */
L8c04153a: *stop=0x8c04153au;r[4]=r[14];goto L8c04153c; /* mov r14,r4 */
L8c04153c: *stop=0x8c04153cu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c04153e; /* lds.l @r15+,PR */
L8c04153e: *stop=0x8c04153eu;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c041540; /* mov.w @r13,r3 */
L8c041540: *stop=0x8c041540u;r[0]=rd(m,0x8c0415f0u,4);goto L8c041542; /* mov.l 0x8c0415f0,r0 */
L8c041542: *stop=0x8c041542u;r[3]=r[3]&65535u;goto L8c041544; /* extu.w r3,r3 */
L8c041544: *stop=0x8c041544u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c041546; /* mov.l @r15+,r13 */
L8c041546: *stop=0x8c041546u;r[3]<<=2;goto L8c041548; /* shll2 r3 */
L8c041548: *stop=0x8c041548u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c04154a; /* mov.l @(r0,r3),r2 */
L8c04154a: target=r[2];*stop=0x8c04154cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_motion_variant_8c04154e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c04154e;
L8c04154e: *stop=0x8c04154eu;r[3]=28u;goto L8c041550; /* mov #0x1c,r3 */
L8c041550: *stop=0x8c041550u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c041552; /* mov.l r14,@-r15 */
L8c041552: *stop=0x8c041552u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c041554; /* mov.l r13,@-r15 */
L8c041554: *stop=0x8c041554u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c041556; /* mov.l r12,@-r15 */
L8c041556: *stop=0x8c041556u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c041558; /* mov.l r11,@-r15 */
L8c041558: *stop=0x8c041558u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c04155a; /* sts.l PR,@-r15 */
L8c04155a: *stop=0x8c04155au;r[11]=r[4];goto L8c04155c; /* mov r4,r11 */
L8c04155c: *stop=0x8c04155cu;r[12]=rd(m,0x8c0415c4u,4);goto L8c04155e; /* mov.l 0x8c0415c4,r12 */
L8c04155e: *stop=0x8c04155eu;if(!valid_ram(r[12],1u))return -3;r[2]=sx(m,r[12],1);goto L8c041560; /* mov.b @r12,r2 */
L8c041560: *stop=0x8c041560u;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[2]);goto L8c041562; /* muls.w r3,r2 */
L8c041562: *stop=0x8c041562u;r[14]=*macl;goto L8c041564; /* sts MACL,r14 */
L8c041564: *stop=0x8c041564u;r[2]=rd(m,0x8c0415f4u,4);goto L8c041566; /* mov.l 0x8c0415f4,r2 */
L8c041566: *stop=0x8c041566u;r[14]=(uint32_t)(int32_t)(int8_t)r[14];goto L8c041568; /* exts.b r14,r14 */
L8c041568: *stop=0x8c041568u;r[14]+=r[2];goto L8c04156a; /* add r2,r14 */
L8c04156a: *stop=0x8c04156au;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c04156c; /* mov.l @r14,r13 */
L8c04156c: *stop=0x8c04156cu;r[0]=r[13];goto L8c04156e; /* mov r13,r0 */
L8c04156e: *stop=0x8c04156eu;goto L8c041570; /* nop */
L8c041570: *stop=0x8c041570u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c041572; /* mov.l r13,@(0x4,r14) */
L8c041572: *stop=0x8c041572u;r[0]+=4u;goto L8c041574; /* add #0x4,r0 */
L8c041574: *stop=0x8c041574u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[0],4);goto L8c041576; /* mov.l r0,@(0x8,r14) */
L8c041576: *stop=0x8c041576u;r[1]=0u;goto L8c041578; /* mov #0x0,r1 */
L8c041578: *stop=0x8c041578u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[1],4);goto L8c04157a; /* mov.l r1,@(0x14,r14) */
L8c04157a: *stop=0x8c04157au;if(!valid_ram(r[12],2u))return -3;r[0]=sx(m,r[12],2);goto L8c04157c; /* mov.w @r12,r0 */
L8c04157c: *stop=0x8c04157cu;r[1]=rd(m,0x8c0415e8u,4);goto L8c04157e; /* mov.l 0x8c0415e8,r1 */
L8c04157e: *stop=0x8c04157eu;r[0]=r[0]&65535u;goto L8c041580; /* extu.w r0,r0 */
L8c041580: *stop=0x8c041580u;r[0]<<=2;goto L8c041582; /* shll2 r0 */
L8c041582: *stop=0x8c041582u;if(!valid_ram(r[0]+r[1],4u))return -3;r[1]=rd(m,r[1]+r[0],4);goto L8c041584; /* mov.l @(r0,r1),r1 */
L8c041584: target=r[1];*pr=0x8c041588u;*stop=0x8c041586u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041588; /* jsr @r1 */
L8c041588: *stop=0x8c041588u;if(!valid_ram(r[12],2u))return -3;r[3]=sx(m,r[12],2);goto L8c04158a; /* mov.w @r12,r3 */
L8c04158a: *stop=0x8c04158au;r[3]=r[3]&65535u;goto L8c04158c; /* extu.w r3,r3 */
L8c04158c: *stop=0x8c04158cu;r[0]=rd(m,0x8c0415ecu,4);goto L8c04158e; /* mov.l 0x8c0415ec,r0 */
L8c04158e: *stop=0x8c04158eu;r[3]<<=2;goto L8c041590; /* shll2 r3 */
L8c041590: *stop=0x8c041590u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c041592; /* mov.l @(r0,r3),r2 */
L8c041592: target=r[2];*pr=0x8c041596u;*stop=0x8c041594u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041596; /* jsr @r2 */
L8c041596: *stop=0x8c041596u;if(!valid_ram(r[12],2u))return -3;r[3]=sx(m,r[12],2);goto L8c041598; /* mov.w @r12,r3 */
L8c041598: *stop=0x8c041598u;r[0]=rd(m,0x8c0415f0u,4);goto L8c04159a; /* mov.l 0x8c0415f0,r0 */
L8c04159a: *stop=0x8c04159au;r[3]=r[3]&65535u;goto L8c04159c; /* extu.w r3,r3 */
L8c04159c: *stop=0x8c04159cu;r[3]<<=2;goto L8c04159e; /* shll2 r3 */
L8c04159e: *stop=0x8c04159eu;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c0415a0; /* mov.l @(r0,r3),r2 */
L8c0415a0: target=r[2];*pr=0x8c0415a4u;*stop=0x8c0415a2u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0415a4; /* jsr @r2 */
L8c0415a4: *stop=0x8c0415a4u;r[13]+=8u;goto L8c0415a6; /* add #0x8,r13 */
L8c0415a6: *stop=0x8c0415a6u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c0415a8; /* mov.l r13,@r14 */
L8c0415a8: *stop=0x8c0415a8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c0415aa; /* lds.l @r15+,PR */
L8c0415aa: *stop=0x8c0415aau;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c0415ac; /* mov.l @r15+,r11 */
L8c0415ac: *stop=0x8c0415acu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0415ae; /* mov.l @r15+,r12 */
L8c0415ae: *stop=0x8c0415aeu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0415b0; /* mov.l @r15+,r13 */
L8c0415b0: *stop=0x8c0415b0u;if(*pr!=entry_pr)return -3;*stop=0x8c0415b2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c041600(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c041600;
L8c041600: *stop=0x8c041600u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c041602; /* mov.l r14,@-r15 */
L8c041602: *stop=0x8c041602u;r[3]=28u;goto L8c041604; /* mov #0x1c,r3 */
L8c041604: *stop=0x8c041604u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c041606; /* mov.l r13,@-r15 */
L8c041606: *stop=0x8c041606u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c041608; /* mov.l r12,@-r15 */
L8c041608: *stop=0x8c041608u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c04160a; /* mov.l r11,@-r15 */
L8c04160a: *stop=0x8c04160au;r[11]=r[4];goto L8c04160c; /* mov r4,r11 */
L8c04160c: *stop=0x8c04160cu;r[12]=rd(m,0x8c0418e4u,4);goto L8c04160e; /* mov.l 0x8c0418e4,r12 */
L8c04160e: *stop=0x8c04160eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c041610; /* sts.l PR,@-r15 */
L8c041610: *stop=0x8c041610u;if(!valid_ram(r[12],1u))return -3;r[2]=sx(m,r[12],1);goto L8c041612; /* mov.b @r12,r2 */
L8c041612: *stop=0x8c041612u;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[2]);goto L8c041614; /* muls.w r3,r2 */
L8c041614: *stop=0x8c041614u;r[2]=rd(m,0x8c0418e8u,4);goto L8c041616; /* mov.l 0x8c0418e8,r2 */
L8c041616: *stop=0x8c041616u;r[14]=*macl;goto L8c041618; /* sts MACL,r14 */
L8c041618: *stop=0x8c041618u;r[14]=(uint32_t)(int32_t)(int8_t)r[14];goto L8c04161a; /* exts.b r14,r14 */
L8c04161a: *stop=0x8c04161au;r[14]+=r[2];goto L8c04161c; /* add r2,r14 */
L8c04161c: *stop=0x8c04161cu;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c04161e; /* mov.l @r14,r13 */
L8c04161e: *stop=0x8c04161eu;r[0]=r[13];goto L8c041620; /* mov r13,r0 */
L8c041620: *stop=0x8c041620u;goto L8c041622; /* nop */
L8c041622: *stop=0x8c041622u;r[0]+=8u;goto L8c041624; /* add #0x8,r0 */
L8c041624: *stop=0x8c041624u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c041626; /* mov.l r13,@(0x4,r14) */
L8c041626: *stop=0x8c041626u;r[1]=0u;goto L8c041628; /* mov #0x0,r1 */
L8c041628: *stop=0x8c041628u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[0],4);goto L8c04162a; /* mov.l r0,@(0x8,r14) */
L8c04162a: *stop=0x8c04162au;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[1],4);goto L8c04162c; /* mov.l r1,@(0x14,r14) */
L8c04162c: *stop=0x8c04162cu;if(!valid_ram(r[12],2u))return -3;r[0]=sx(m,r[12],2);goto L8c04162e; /* mov.w @r12,r0 */
L8c04162e: *stop=0x8c04162eu;r[0]=r[0]&65535u;goto L8c041630; /* extu.w r0,r0 */
L8c041630: *stop=0x8c041630u;r[1]=rd(m,0x8c0418ecu,4);goto L8c041632; /* mov.l 0x8c0418ec,r1 */
L8c041632: *stop=0x8c041632u;r[0]<<=2;goto L8c041634; /* shll2 r0 */
L8c041634: *stop=0x8c041634u;if(!valid_ram(r[0]+r[1],4u))return -3;r[1]=rd(m,r[1]+r[0],4);goto L8c041636; /* mov.l @(r0,r1),r1 */
L8c041636: target=r[1];*pr=0x8c04163au;*stop=0x8c041638u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04163a; /* jsr @r1 */
L8c04163a: *stop=0x8c04163au;if(!valid_ram(r[12],2u))return -3;r[3]=sx(m,r[12],2);goto L8c04163c; /* mov.w @r12,r3 */
L8c04163c: *stop=0x8c04163cu;r[0]=rd(m,0x8c0418f0u,4);goto L8c04163e; /* mov.l 0x8c0418f0,r0 */
L8c04163e: *stop=0x8c04163eu;r[3]=r[3]&65535u;goto L8c041640; /* extu.w r3,r3 */
L8c041640: *stop=0x8c041640u;r[3]<<=2;goto L8c041642; /* shll2 r3 */
L8c041642: *stop=0x8c041642u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c041644; /* mov.l @(r0,r3),r2 */
L8c041644: target=r[2];*pr=0x8c041648u;*stop=0x8c041646u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041648; /* jsr @r2 */
L8c041648: *stop=0x8c041648u;if(!valid_ram(r[12],2u))return -3;r[3]=sx(m,r[12],2);goto L8c04164a; /* mov.w @r12,r3 */
L8c04164a: *stop=0x8c04164au;r[3]=r[3]&65535u;goto L8c04164c; /* extu.w r3,r3 */
L8c04164c: *stop=0x8c04164cu;r[0]=rd(m,0x8c0418f4u,4);goto L8c04164e; /* mov.l 0x8c0418f4,r0 */
L8c04164e: *stop=0x8c04164eu;r[3]<<=2;goto L8c041650; /* shll2 r3 */
L8c041650: *stop=0x8c041650u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c041652; /* mov.l @(r0,r3),r2 */
L8c041652: target=r[2];*pr=0x8c041656u;*stop=0x8c041654u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041656; /* jsr @r2 */
L8c041656: *stop=0x8c041656u;r[13]+=16u;goto L8c041658; /* add #0x10,r13 */
L8c041658: *stop=0x8c041658u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c04165a; /* mov.l r13,@r14 */
L8c04165a: *stop=0x8c04165au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c04165c; /* lds.l @r15+,PR */
L8c04165c: *stop=0x8c04165cu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c04165e; /* mov.l @r15+,r11 */
L8c04165e: *stop=0x8c04165eu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c041660; /* mov.l @r15+,r12 */
L8c041660: *stop=0x8c041660u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c041662; /* mov.l @r15+,r13 */
L8c041662: *stop=0x8c041662u;if(*pr!=entry_pr)return -3;*stop=0x8c041664u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c041666(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c041666;
L8c041666: *stop=0x8c041666u;r[3]=28u;goto L8c041668; /* mov #0x1c,r3 */
L8c041668: *stop=0x8c041668u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c04166a; /* mov.l r14,@-r15 */
L8c04166a: *stop=0x8c04166au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c04166c; /* mov.l r13,@-r15 */
L8c04166c: *stop=0x8c04166cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c04166e; /* mov.l r12,@-r15 */
L8c04166e: *stop=0x8c04166eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c041670; /* mov.l r11,@-r15 */
L8c041670: *stop=0x8c041670u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c041672; /* sts.l PR,@-r15 */
L8c041672: *stop=0x8c041672u;r[11]=r[4];goto L8c041674; /* mov r4,r11 */
L8c041674: *stop=0x8c041674u;r[12]=rd(m,0x8c0418e4u,4);goto L8c041676; /* mov.l 0x8c0418e4,r12 */
L8c041676: *stop=0x8c041676u;if(!valid_ram(r[12],1u))return -3;r[2]=sx(m,r[12],1);goto L8c041678; /* mov.b @r12,r2 */
L8c041678: *stop=0x8c041678u;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[2]);goto L8c04167a; /* muls.w r3,r2 */
L8c04167a: *stop=0x8c04167au;r[14]=*macl;goto L8c04167c; /* sts MACL,r14 */
L8c04167c: *stop=0x8c04167cu;r[2]=rd(m,0x8c0418e8u,4);goto L8c04167e; /* mov.l 0x8c0418e8,r2 */
L8c04167e: *stop=0x8c04167eu;r[14]=(uint32_t)(int32_t)(int8_t)r[14];goto L8c041680; /* exts.b r14,r14 */
L8c041680: *stop=0x8c041680u;r[14]+=r[2];goto L8c041682; /* add r2,r14 */
L8c041682: *stop=0x8c041682u;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c041684; /* mov.l @r14,r13 */
L8c041684: *stop=0x8c041684u;r[0]=r[13];goto L8c041686; /* mov r13,r0 */
L8c041686: *stop=0x8c041686u;goto L8c041688; /* nop */
L8c041688: *stop=0x8c041688u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c04168a; /* mov.l r13,@(0x4,r14) */
L8c04168a: *stop=0x8c04168au;r[0]+=12u;goto L8c04168c; /* add #0xc,r0 */
L8c04168c: *stop=0x8c04168cu;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[0],4);goto L8c04168e; /* mov.l r0,@(0x8,r14) */
L8c04168e: *stop=0x8c04168eu;r[1]=0u;goto L8c041690; /* mov #0x0,r1 */
L8c041690: *stop=0x8c041690u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[1],4);goto L8c041692; /* mov.l r1,@(0x14,r14) */
L8c041692: *stop=0x8c041692u;if(!valid_ram(r[12],2u))return -3;r[0]=sx(m,r[12],2);goto L8c041694; /* mov.w @r12,r0 */
L8c041694: *stop=0x8c041694u;r[1]=rd(m,0x8c0418ecu,4);goto L8c041696; /* mov.l 0x8c0418ec,r1 */
L8c041696: *stop=0x8c041696u;r[0]=r[0]&65535u;goto L8c041698; /* extu.w r0,r0 */
L8c041698: *stop=0x8c041698u;r[0]<<=2;goto L8c04169a; /* shll2 r0 */
L8c04169a: *stop=0x8c04169au;if(!valid_ram(r[0]+r[1],4u))return -3;r[1]=rd(m,r[1]+r[0],4);goto L8c04169c; /* mov.l @(r0,r1),r1 */
L8c04169c: target=r[1];*pr=0x8c0416a0u;*stop=0x8c04169eu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0416a0; /* jsr @r1 */
L8c0416a0: *stop=0x8c0416a0u;if(!valid_ram(r[12],2u))return -3;r[3]=sx(m,r[12],2);goto L8c0416a2; /* mov.w @r12,r3 */
L8c0416a2: *stop=0x8c0416a2u;r[3]=r[3]&65535u;goto L8c0416a4; /* extu.w r3,r3 */
L8c0416a4: *stop=0x8c0416a4u;r[0]=rd(m,0x8c0418f0u,4);goto L8c0416a6; /* mov.l 0x8c0418f0,r0 */
L8c0416a6: *stop=0x8c0416a6u;r[3]<<=2;goto L8c0416a8; /* shll2 r3 */
L8c0416a8: *stop=0x8c0416a8u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c0416aa; /* mov.l @(r0,r3),r2 */
L8c0416aa: target=r[2];*pr=0x8c0416aeu;*stop=0x8c0416acu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0416ae; /* jsr @r2 */
L8c0416ae: *stop=0x8c0416aeu;if(!valid_ram(r[12],2u))return -3;r[3]=sx(m,r[12],2);goto L8c0416b0; /* mov.w @r12,r3 */
L8c0416b0: *stop=0x8c0416b0u;r[0]=rd(m,0x8c0418f4u,4);goto L8c0416b2; /* mov.l 0x8c0418f4,r0 */
L8c0416b2: *stop=0x8c0416b2u;r[3]=r[3]&65535u;goto L8c0416b4; /* extu.w r3,r3 */
L8c0416b4: *stop=0x8c0416b4u;r[3]<<=2;goto L8c0416b6; /* shll2 r3 */
L8c0416b6: *stop=0x8c0416b6u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c0416b8; /* mov.l @(r0,r3),r2 */
L8c0416b8: target=r[2];*pr=0x8c0416bcu;*stop=0x8c0416bau;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0416bc; /* jsr @r2 */
L8c0416bc: *stop=0x8c0416bcu;r[13]+=24u;goto L8c0416be; /* add #0x18,r13 */
L8c0416be: *stop=0x8c0416beu;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c0416c0; /* mov.l r13,@r14 */
L8c0416c0: *stop=0x8c0416c0u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c0416c2; /* lds.l @r15+,PR */
L8c0416c2: *stop=0x8c0416c2u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c0416c4; /* mov.l @r15+,r11 */
L8c0416c4: *stop=0x8c0416c4u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0416c6; /* mov.l @r15+,r12 */
L8c0416c6: *stop=0x8c0416c6u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0416c8; /* mov.l @r15+,r13 */
L8c0416c8: *stop=0x8c0416c8u;if(*pr!=entry_pr)return -3;*stop=0x8c0416cau;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0416cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0416cc;
L8c0416cc: *stop=0x8c0416ccu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0416ce; /* mov.l r14,@-r15 */
L8c0416ce: *stop=0x8c0416ceu;r[3]=28u;goto L8c0416d0; /* mov #0x1c,r3 */
L8c0416d0: *stop=0x8c0416d0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0416d2; /* mov.l r13,@-r15 */
L8c0416d2: *stop=0x8c0416d2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0416d4; /* mov.l r12,@-r15 */
L8c0416d4: *stop=0x8c0416d4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c0416d6; /* mov.l r11,@-r15 */
L8c0416d6: *stop=0x8c0416d6u;r[11]=r[4];goto L8c0416d8; /* mov r4,r11 */
L8c0416d8: *stop=0x8c0416d8u;r[12]=rd(m,0x8c0418e4u,4);goto L8c0416da; /* mov.l 0x8c0418e4,r12 */
L8c0416da: *stop=0x8c0416dau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c0416dc; /* sts.l PR,@-r15 */
L8c0416dc: *stop=0x8c0416dcu;if(!valid_ram(r[12],1u))return -3;r[2]=sx(m,r[12],1);goto L8c0416de; /* mov.b @r12,r2 */
L8c0416de: *stop=0x8c0416deu;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[2]);goto L8c0416e0; /* muls.w r3,r2 */
L8c0416e0: *stop=0x8c0416e0u;r[2]=rd(m,0x8c0418e8u,4);goto L8c0416e2; /* mov.l 0x8c0418e8,r2 */
L8c0416e2: *stop=0x8c0416e2u;r[14]=*macl;goto L8c0416e4; /* sts MACL,r14 */
L8c0416e4: *stop=0x8c0416e4u;r[14]=(uint32_t)(int32_t)(int8_t)r[14];goto L8c0416e6; /* exts.b r14,r14 */
L8c0416e6: *stop=0x8c0416e6u;r[14]+=r[2];goto L8c0416e8; /* add r2,r14 */
L8c0416e8: *stop=0x8c0416e8u;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c0416ea; /* mov.l @r14,r13 */
L8c0416ea: *stop=0x8c0416eau;r[0]=r[13];goto L8c0416ec; /* mov r13,r0 */
L8c0416ec: *stop=0x8c0416ecu;goto L8c0416ee; /* nop */
L8c0416ee: *stop=0x8c0416eeu;r[0]+=16u;goto L8c0416f0; /* add #0x10,r0 */
L8c0416f0: *stop=0x8c0416f0u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c0416f2; /* mov.l r13,@(0x4,r14) */
L8c0416f2: *stop=0x8c0416f2u;r[1]=0u;goto L8c0416f4; /* mov #0x0,r1 */
L8c0416f4: *stop=0x8c0416f4u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[0],4);goto L8c0416f6; /* mov.l r0,@(0x8,r14) */
L8c0416f6: *stop=0x8c0416f6u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[1],4);goto L8c0416f8; /* mov.l r1,@(0x14,r14) */
L8c0416f8: *stop=0x8c0416f8u;if(!valid_ram(r[12],2u))return -3;r[0]=sx(m,r[12],2);goto L8c0416fa; /* mov.w @r12,r0 */
L8c0416fa: *stop=0x8c0416fau;r[0]=r[0]&65535u;goto L8c0416fc; /* extu.w r0,r0 */
L8c0416fc: *stop=0x8c0416fcu;r[1]=rd(m,0x8c0418ecu,4);goto L8c0416fe; /* mov.l 0x8c0418ec,r1 */
L8c0416fe: *stop=0x8c0416feu;r[0]<<=2;goto L8c041700; /* shll2 r0 */
L8c041700: *stop=0x8c041700u;if(!valid_ram(r[0]+r[1],4u))return -3;r[1]=rd(m,r[1]+r[0],4);goto L8c041702; /* mov.l @(r0,r1),r1 */
L8c041702: target=r[1];*pr=0x8c041706u;*stop=0x8c041704u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041706; /* jsr @r1 */
L8c041706: *stop=0x8c041706u;if(!valid_ram(r[12],2u))return -3;r[3]=sx(m,r[12],2);goto L8c041708; /* mov.w @r12,r3 */
L8c041708: *stop=0x8c041708u;r[0]=rd(m,0x8c0418f0u,4);goto L8c04170a; /* mov.l 0x8c0418f0,r0 */
L8c04170a: *stop=0x8c04170au;r[3]=r[3]&65535u;goto L8c04170c; /* extu.w r3,r3 */
L8c04170c: *stop=0x8c04170cu;r[3]<<=2;goto L8c04170e; /* shll2 r3 */
L8c04170e: *stop=0x8c04170eu;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c041710; /* mov.l @(r0,r3),r2 */
L8c041710: target=r[2];*pr=0x8c041714u;*stop=0x8c041712u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041714; /* jsr @r2 */
L8c041714: *stop=0x8c041714u;if(!valid_ram(r[12],2u))return -3;r[3]=sx(m,r[12],2);goto L8c041716; /* mov.w @r12,r3 */
L8c041716: *stop=0x8c041716u;r[3]=r[3]&65535u;goto L8c041718; /* extu.w r3,r3 */
L8c041718: *stop=0x8c041718u;r[0]=rd(m,0x8c0418f4u,4);goto L8c04171a; /* mov.l 0x8c0418f4,r0 */
L8c04171a: *stop=0x8c04171au;r[3]<<=2;goto L8c04171c; /* shll2 r3 */
L8c04171c: *stop=0x8c04171cu;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c04171e; /* mov.l @(r0,r3),r2 */
L8c04171e: target=r[2];*pr=0x8c041722u;*stop=0x8c041720u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041722; /* jsr @r2 */
L8c041722: *stop=0x8c041722u;r[13]+=32u;goto L8c041724; /* add #0x20,r13 */
L8c041724: *stop=0x8c041724u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c041726; /* mov.l r13,@r14 */
L8c041726: *stop=0x8c041726u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c041728; /* lds.l @r15+,PR */
L8c041728: *stop=0x8c041728u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c04172a; /* mov.l @r15+,r11 */
L8c04172a: *stop=0x8c04172au;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c04172c; /* mov.l @r15+,r12 */
L8c04172c: *stop=0x8c04172cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c04172e; /* mov.l @r15+,r13 */
L8c04172e: *stop=0x8c04172eu;if(*pr!=entry_pr)return -3;*stop=0x8c041730u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c041732(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c041732;
L8c041732: *stop=0x8c041732u;if(!valid_ram(r[4]+4u,4u))return -3;r[5]=rd(m,r[4]+0x4u,4);goto L8c041734; /* mov.l @(0x4,r4),r5 */
L8c041734: *stop=0x8c041734u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c041736; /* tst r5,r5 */
L8c041736: if((*sr&1u))goto L8c041758;goto L8c041738; /* bt 0x8c041758 */
L8c041738: *stop=0x8c041738u;if(!valid_ram(r[4],4u))return -3;r[0]=rd(m,r[4],4);goto L8c04173a; /* mov.l @r4,r0 */
L8c04173a: *stop=0x8c04173au;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c04173c; /* tst #0x8,r0 */
L8c04173c: if(!(*sr&1u))goto L8c041758;goto L8c04173e; /* bf 0x8c041758 */
L8c04173e: *stop=0x8c04173eu;r[4]=rd(m,0x8c0418e4u,4);goto L8c041740; /* mov.l 0x8c0418e4,r4 */
L8c041740: *stop=0x8c041740u;r[0]=rd(m,0x8c0418f8u,4);goto L8c041742; /* mov.l 0x8c0418f8,r0 */
L8c041742: *stop=0x8c041742u;if(!valid_ram(r[4],2u))return -3;r[3]=sx(m,r[4],2);goto L8c041744; /* mov.w @r4,r3 */
L8c041744: *stop=0x8c041744u;if(!valid_ram(r[5],4u))return -3;r[2]=rd(m,r[5],4);goto L8c041746; /* mov.l @r5,r2 */
L8c041746: *stop=0x8c041746u;r[3]=r[3]&65535u;goto L8c041748; /* extu.w r3,r3 */
L8c041748: *stop=0x8c041748u;r[3]<<=2;goto L8c04174a; /* shll2 r3 */
L8c04174a: *stop=0x8c04174au;if(!valid_ram(r[0]+r[3],4u))return -3;wr(m,r[3]+r[0],r[2],4);goto L8c04174c; /* mov.l r2,@(r0,r3) */
L8c04174c: *stop=0x8c04174cu;if(!valid_ram(r[4],2u))return -3;r[3]=sx(m,r[4],2);goto L8c04174e; /* mov.w @r4,r3 */
L8c04174e: *stop=0x8c04174eu;r[3]=r[3]&65535u;goto L8c041750; /* extu.w r3,r3 */
L8c041750: *stop=0x8c041750u;r[0]=rd(m,0x8c0418fcu,4);goto L8c041752; /* mov.l 0x8c0418fc,r0 */
L8c041752: *stop=0x8c041752u;r[3]<<=2;goto L8c041754; /* shll2 r3 */
L8c041754: *stop=0x8c041754u;if(!valid_ram(r[5]+4u,4u))return -3;r[2]=rd(m,r[5]+0x4u,4);goto L8c041756; /* mov.l @(0x4,r5),r2 */
L8c041756: *stop=0x8c041756u;if(!valid_ram(r[0]+r[3],4u))return -3;wr(m,r[3]+r[0],r[2],4);goto L8c041758; /* mov.l r2,@(r0,r3) */
L8c041758: *stop=0x8c041758u;if(*pr!=entry_pr)return -3;*stop=0x8c04175au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c04175c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c04175c;
L8c04175c: *stop=0x8c04175cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c04175e; /* mov.l r14,@-r15 */
L8c04175e: *stop=0x8c04175eu;r[3]=28u;goto L8c041760; /* mov #0x1c,r3 */
L8c041760: *stop=0x8c041760u;r[1]=rd(m,0x8c0418e4u,4);goto L8c041762; /* mov.l 0x8c0418e4,r1 */
L8c041762: *stop=0x8c041762u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c041764; /* mov.l r13,@-r15 */
L8c041764: *stop=0x8c041764u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c041766; /* sts.l PR,@-r15 */
L8c041766: *stop=0x8c041766u;if(!valid_ram(r[1],1u))return -3;r[2]=sx(m,r[1],1);goto L8c041768; /* mov.b @r1,r2 */
L8c041768: *stop=0x8c041768u;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[2]);goto L8c04176a; /* muls.w r3,r2 */
L8c04176a: *stop=0x8c04176au;r[14]=*macl;goto L8c04176c; /* sts MACL,r14 */
L8c04176c: *stop=0x8c04176cu;r[2]=rd(m,0x8c041900u,4);goto L8c04176e; /* mov.l 0x8c041900,r2 */
L8c04176e: *stop=0x8c04176eu;r[14]=(uint32_t)(int32_t)(int8_t)r[14];goto L8c041770; /* exts.b r14,r14 */
L8c041770: *stop=0x8c041770u;r[14]+=r[2];goto L8c041772; /* add r2,r14 */
L8c041772: *stop=0x8c041772u;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c041774; /* mov.l @r14,r13 */
L8c041774: *stop=0x8c041774u;r[0]=r[13];goto L8c041776; /* mov r13,r0 */
L8c041776: *stop=0x8c041776u;goto L8c041778; /* nop */
L8c041778: *stop=0x8c041778u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c04177a; /* mov.l r13,@(0x4,r14) */
L8c04177a: *stop=0x8c04177au;r[0]+=4u;goto L8c04177c; /* add #0x4,r0 */
L8c04177c: *stop=0x8c04177cu;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[0],4);goto L8c04177e; /* mov.l r0,@(0x8,r14) */
L8c04177e: *stop=0x8c04177eu;r[1]=0u;goto L8c041780; /* mov #0x0,r1 */
L8c041780: *stop=0x8c041780u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[1],4);goto L8c041782; /* mov.l r1,@(0x14,r14) */
L8c041782: target=0x8c041378u;*pr=0x8c041786u;*stop=0x8c041784u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041786; /* bsr 0x8c041378 */
L8c041786: *stop=0x8c041786u;r[13]+=8u;goto L8c041788; /* add #0x8,r13 */
L8c041788: *stop=0x8c041788u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c04178a; /* mov.l r13,@r14 */
L8c04178a: *stop=0x8c04178au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c04178c; /* lds.l @r15+,PR */
L8c04178c: *stop=0x8c04178cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c04178e; /* mov.l @r15+,r13 */
L8c04178e: *stop=0x8c04178eu;if(*pr!=entry_pr)return -3;*stop=0x8c041790u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c041792(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c041792;
L8c041792: *stop=0x8c041792u;r[3]=28u;goto L8c041794; /* mov #0x1c,r3 */
L8c041794: *stop=0x8c041794u;r[1]=rd(m,0x8c0418e4u,4);goto L8c041796; /* mov.l 0x8c0418e4,r1 */
L8c041796: *stop=0x8c041796u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c041798; /* mov.l r14,@-r15 */
L8c041798: *stop=0x8c041798u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c04179a; /* mov.l r13,@-r15 */
L8c04179a: *stop=0x8c04179au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c04179c; /* sts.l PR,@-r15 */
L8c04179c: *stop=0x8c04179cu;if(!valid_ram(r[1],1u))return -3;r[2]=sx(m,r[1],1);goto L8c04179e; /* mov.b @r1,r2 */
L8c04179e: *stop=0x8c04179eu;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[2]);goto L8c0417a0; /* muls.w r3,r2 */
L8c0417a0: *stop=0x8c0417a0u;r[2]=rd(m,0x8c041900u,4);goto L8c0417a2; /* mov.l 0x8c041900,r2 */
L8c0417a2: *stop=0x8c0417a2u;r[14]=*macl;goto L8c0417a4; /* sts MACL,r14 */
L8c0417a4: *stop=0x8c0417a4u;r[14]=(uint32_t)(int32_t)(int8_t)r[14];goto L8c0417a6; /* exts.b r14,r14 */
L8c0417a6: *stop=0x8c0417a6u;r[14]+=r[2];goto L8c0417a8; /* add r2,r14 */
L8c0417a8: *stop=0x8c0417a8u;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);goto L8c0417aa; /* mov.l @r14,r13 */
L8c0417aa: *stop=0x8c0417aau;r[0]=r[13];goto L8c0417ac; /* mov r13,r0 */
L8c0417ac: *stop=0x8c0417acu;goto L8c0417ae; /* nop */
L8c0417ae: *stop=0x8c0417aeu;r[0]+=8u;goto L8c0417b0; /* add #0x8,r0 */
L8c0417b0: *stop=0x8c0417b0u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[13],4);goto L8c0417b2; /* mov.l r13,@(0x4,r14) */
L8c0417b2: *stop=0x8c0417b2u;r[1]=0u;goto L8c0417b4; /* mov #0x0,r1 */
L8c0417b4: *stop=0x8c0417b4u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[0],4);goto L8c0417b6; /* mov.l r0,@(0x8,r14) */
L8c0417b6: *stop=0x8c0417b6u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[1],4);goto L8c0417b8; /* mov.l r1,@(0x14,r14) */
L8c0417b8: target=0x8c041378u;*pr=0x8c0417bcu;*stop=0x8c0417bau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0417bc; /* bsr 0x8c041378 */
L8c0417bc: *stop=0x8c0417bcu;r[13]+=16u;goto L8c0417be; /* add #0x10,r13 */
L8c0417be: *stop=0x8c0417beu;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[13],4);goto L8c0417c0; /* mov.l r13,@r14 */
L8c0417c0: *stop=0x8c0417c0u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c0417c2; /* lds.l @r15+,PR */
L8c0417c2: *stop=0x8c0417c2u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0417c4; /* mov.l @r15+,r13 */
L8c0417c4: *stop=0x8c0417c4u;if(*pr!=entry_pr)return -3;*stop=0x8c0417c6u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_variant_8c0417c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0417c8;
L8c0417c8: *stop=0x8c0417c8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0417ca; /* mov.l r14,@-r15 */
L8c0417ca: *stop=0x8c0417cau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][9]=0x3f800000u;goto L8c0417cc; /* fldi1 fr9 */
L8c0417cc: *stop=0x8c0417ccu;r[7]=rd(m,0x8c041904u,4);goto L8c0417ce; /* mov.l 0x8c041904,r7 */
L8c0417ce: *stop=0x8c0417ceu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0417d0; /* mov.l r12,@-r15 */
L8c0417d0: *stop=0x8c0417d0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c0417d2; /* sts.l PR,@-r15 */
L8c0417d2: *stop=0x8c0417d2u;r[15]+=4294967260u;goto L8c0417d4; /* add #-0x24,r15 */
L8c0417d4: *stop=0x8c0417d4u;r[12]=rd(m,0x8c041908u,4);goto L8c0417d6; /* mov.l 0x8c041908,r12 */
L8c0417d6: *stop=0x8c0417d6u;r[2]=rd(m,0x8c04190cu,4);goto L8c0417d8; /* mov.l 0x8c04190c,r2 */
L8c0417d8: *stop=0x8c0417d8u;if(!valid_ram(r[2],2u))return -3;r[3]=sx(m,r[2],2);goto L8c0417da; /* mov.w @r2,r3 */
L8c0417da: *stop=0x8c0417dau;r[3]=r[3]&65535u;goto L8c0417dc; /* extu.w r3,r3 */
L8c0417dc: *stop=0x8c0417dcu;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c0417de; /* cmp/pl r3 */
L8c0417de: take=!(*sr&1u);*stop=0x8c0417e0u;r[4]=r[7];if(take)goto L8c04180c;goto L8c0417e2; /* bf/s 0x8c04180c */
L8c0417e2: *stop=0x8c0417e2u;fcopy(f,8u,9u);goto L8c0417e4; /* fmov fr9,fr8 */
L8c0417e4: *stop=0x8c0417e4u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0417e6; /* fmov.s @r4+,fr5 */
L8c0417e6: *stop=0x8c0417e6u;r[5]=r[7];goto L8c0417e8; /* mov r7,r5 */
L8c0417e8: *stop=0x8c0417e8u;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[12]);goto L8c0417ea; /* fmov.s @r12,fr4 */
L8c0417ea: *stop=0x8c0417eau;r[5]+=12u;goto L8c0417ec; /* add #0xc,r5 */
L8c0417ec: *stop=0x8c0417ecu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0417ee; /* fmov.s @r5+,fr6 */
L8c0417ee: *stop=0x8c0417eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[4],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c0417f0; /* fsub fr4,fr8 */
L8c0417f0: *stop=0x8c0417f0u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0417f2; /* fmov.s @r5+,fr7 */
L8c0417f2: *stop=0x8c0417f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[8],&f->fpscr,&fr[5]);if(status)return status;}goto L8c0417f4; /* fmul fr8,fr5 */
L8c0417f4: *stop=0x8c0417f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[4],&f->fpscr,&fr[6]);if(status)return status;}goto L8c0417f6; /* fmul fr4,fr6 */
L8c0417f6: *stop=0x8c0417f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[6],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c0417f8; /* fadd fr6,fr5 */
L8c0417f8: *stop=0x8c0417f8u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0417fa; /* fmov.s @r4+,fr6 */
L8c0417fa: *stop=0x8c0417fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[8],&f->fpscr,&fr[6]);if(status)return status;}goto L8c0417fc; /* fmul fr8,fr6 */
L8c0417fc: *stop=0x8c0417fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[4],&f->fpscr,&fr[7]);if(status)return status;}goto L8c0417fe; /* fmul fr4,fr7 */
L8c0417fe: *stop=0x8c0417feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c041800; /* fadd fr7,fr6 */
L8c041800: *stop=0x8c041800u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[4]);goto L8c041802; /* fmov.s @r4,fr7 */
L8c041802: *stop=0x8c041802u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[8],&f->fpscr,&fr[7]);if(status)return status;}goto L8c041804; /* fmul fr8,fr7 */
L8c041804: *stop=0x8c041804u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[5]);goto L8c041806; /* fmov.s @r5,fr8 */
L8c041806: *stop=0x8c041806u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[4],&f->fpscr,&fr[8]);if(status)return status;}goto L8c041808; /* fmul fr4,fr8 */
L8c041808: *stop=0x8c04180au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[8],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c041812; /* bra 0x8c041812 */
L8c04180c: *stop=0x8c04180cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c04180e; /* fmov.s @r4+,fr5 */
L8c04180e: *stop=0x8c04180eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c041810; /* fmov.s @r4+,fr6 */
L8c041810: *stop=0x8c041810u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[4]);goto L8c041812; /* fmov.s @r4,fr7 */
L8c041812: *stop=0x8c041812u;r[4]=r[15];goto L8c041814; /* mov r15,r4 */
L8c041814: *stop=0x8c041814u;r[2]=rd(m,0x8c041910u,4);goto L8c041816; /* mov.l 0x8c041910,r2 */
L8c041816: *stop=0x8c041816u;r[4]+=36u;goto L8c041818; /* add #0x24,r4 */
L8c041818: *stop=0x8c041818u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[4]);goto L8c04181a; /* fmov.s fr7,@-r4 */
L8c04181a: *stop=0x8c04181au;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[4]);goto L8c04181c; /* fmov.s fr6,@-r4 */
L8c04181c: *stop=0x8c04181cu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[4]);goto L8c04181e; /* fmov.s fr5,@-r4 */
L8c04181e: *stop=0x8c04181eu;r[4]=rd(m,0x8c041914u,4);goto L8c041820; /* mov.l 0x8c041914,r4 */
L8c041820: *stop=0x8c041820u;if(!valid_ram(r[2],2u))return -3;r[3]=sx(m,r[2],2);goto L8c041822; /* mov.w @r2,r3 */
L8c041822: *stop=0x8c041822u;r[3]=r[3]&65535u;goto L8c041824; /* extu.w r3,r3 */
L8c041824: *stop=0x8c041824u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c041826; /* cmp/pl r3 */
L8c041826: if(!(*sr&1u))goto L8c041888;goto L8c041828; /* bf 0x8c041888 */
L8c041828: *stop=0x8c041828u;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[12]);goto L8c04182a; /* fmov.s @r12,fr4 */
L8c04182a: *stop=0x8c04182au;fcopy(f,5u,9u);goto L8c04182c; /* fmov fr9,fr5 */
L8c04182c: *stop=0x8c04182cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[4],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c04182e; /* fsub fr4,fr5 */
L8c04182e: *stop=0x8c04182eu;r[5]=r[4];goto L8c041830; /* mov r4,r5 */
L8c041830: *stop=0x8c041830u;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c041832; /* mov.l @r5,r3 */
L8c041832: *stop=0x8c041832u;r[4]+=12u;goto L8c041834; /* add #0xc,r4 */
L8c041834: *stop=0x8c041834u;*fpul=r[3];goto L8c041836; /* lds r3,FPUL */
L8c041836: *stop=0x8c041836u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c041838; /* float FPUL,fr3 */
L8c041838: *stop=0x8c041838u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c04183a; /* mov.l @r4,r3 */
L8c04183a: *stop=0x8c04183au;fcopy(f,2u,3u);goto L8c04183c; /* fmov fr3,fr2 */
L8c04183c: *stop=0x8c04183cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c04183e; /* fmul fr5,fr2 */
L8c04183e: *stop=0x8c04183eu;*fpul=r[3];goto L8c041840; /* lds r3,FPUL */
L8c041840: *stop=0x8c041840u;if(!valid_ram(r[5]+4u,4u))return -3;r[3]=rd(m,r[5]+0x4u,4);goto L8c041842; /* mov.l @(0x4,r5),r3 */
L8c041842: *stop=0x8c041842u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c041844; /* float FPUL,fr3 */
L8c041844: *stop=0x8c041844u;fcopy(f,1u,3u);goto L8c041846; /* fmov fr3,fr1 */
L8c041846: *stop=0x8c041846u;fcopy(f,3u,2u);goto L8c041848; /* fmov fr2,fr3 */
L8c041848: *stop=0x8c041848u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c04184a; /* fmul fr4,fr1 */
L8c04184a: *stop=0x8c04184au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c04184c; /* fadd fr1,fr3 */
L8c04184c: *stop=0x8c04184cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c04184e; /* ftrc fr3,FPUL */
L8c04184e: *stop=0x8c04184eu;r[1]=*fpul;goto L8c041850; /* sts FPUL,r1 */
L8c041850: *stop=0x8c041850u;*fpul=r[3];goto L8c041852; /* lds r3,FPUL */
L8c041852: *stop=0x8c041852u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c041854; /* float FPUL,fr3 */
L8c041854: *stop=0x8c041854u;if(!valid_ram(r[4]+4u,4u))return -3;r[3]=rd(m,r[4]+0x4u,4);goto L8c041856; /* mov.l @(0x4,r4),r3 */
L8c041856: *stop=0x8c041856u;fcopy(f,2u,3u);goto L8c041858; /* fmov fr3,fr2 */
L8c041858: *stop=0x8c041858u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c04185a; /* fmul fr5,fr2 */
L8c04185a: *stop=0x8c04185au;*fpul=r[3];goto L8c04185c; /* lds r3,FPUL */
L8c04185c: *stop=0x8c04185cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c04185e; /* float FPUL,fr3 */
L8c04185e: *stop=0x8c04185eu;fcopy(f,1u,3u);goto L8c041860; /* fmov fr3,fr1 */
L8c041860: *stop=0x8c041860u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c041862; /* fmul fr4,fr1 */
L8c041862: *stop=0x8c041862u;fcopy(f,3u,2u);goto L8c041864; /* fmov fr2,fr3 */
L8c041864: *stop=0x8c041864u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c041866; /* fadd fr1,fr3 */
L8c041866: *stop=0x8c041866u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c041868; /* ftrc fr3,FPUL */
L8c041868: *stop=0x8c041868u;if(!valid_ram(r[5]+8u,4u))return -3;r[3]=rd(m,r[5]+0x8u,4);goto L8c04186a; /* mov.l @(0x8,r5),r3 */
L8c04186a: *stop=0x8c04186au;r[0]=*fpul;goto L8c04186c; /* sts FPUL,r0 */
L8c04186c: *stop=0x8c04186cu;*fpul=r[3];goto L8c04186e; /* lds r3,FPUL */
L8c04186e: *stop=0x8c04186eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c041870; /* float FPUL,fr3 */
L8c041870: *stop=0x8c041870u;if(!valid_ram(r[4]+8u,4u))return -3;r[3]=rd(m,r[4]+0x8u,4);goto L8c041872; /* mov.l @(0x8,r4),r3 */
L8c041872: *stop=0x8c041872u;fcopy(f,2u,3u);goto L8c041874; /* fmov fr3,fr2 */
L8c041874: *stop=0x8c041874u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c041876; /* fmul fr5,fr2 */
L8c041876: *stop=0x8c041876u;*fpul=r[3];goto L8c041878; /* lds r3,FPUL */
L8c041878: *stop=0x8c041878u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c04187a; /* float FPUL,fr3 */
L8c04187a: *stop=0x8c04187au;fcopy(f,1u,3u);goto L8c04187c; /* fmov fr3,fr1 */
L8c04187c: *stop=0x8c04187cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c04187e; /* fmul fr4,fr1 */
L8c04187e: *stop=0x8c04187eu;fcopy(f,3u,2u);goto L8c041880; /* fmov fr2,fr3 */
L8c041880: *stop=0x8c041880u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c041882; /* fadd fr1,fr3 */
L8c041882: *stop=0x8c041882u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c041884; /* ftrc fr3,FPUL */
L8c041884: *stop=0x8c041886u;r[6]=*fpul;goto L8c04188e; /* bra 0x8c04188e */
L8c041888: *stop=0x8c041888u;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);r[4]+=4;goto L8c04188a; /* mov.l @r4+,r1 */
L8c04188a: *stop=0x8c04188au;if(!valid_ram(r[4],4u))return -3;r[0]=rd(m,r[4],4);r[4]+=4;goto L8c04188c; /* mov.l @r4+,r0 */
L8c04188c: *stop=0x8c04188cu;if(!valid_ram(r[4],4u))return -3;r[6]=rd(m,r[4],4);goto L8c04188e; /* mov.l @r4,r6 */
L8c04188e: *stop=0x8c04188eu;r[14]=r[15];goto L8c041890; /* mov r15,r14 */
L8c041890: *stop=0x8c041890u;r[2]=rd(m,0x8c041918u,4);goto L8c041892; /* mov.l 0x8c041918,r2 */
L8c041892: *stop=0x8c041892u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[1],4);goto L8c041894; /* mov.l r1,@r14 */
L8c041894: *stop=0x8c041894u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[0],4);goto L8c041896; /* mov.l r0,@(0x4,r14) */
L8c041896: *stop=0x8c041896u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[6],4);goto L8c041898; /* mov.l r6,@(0x8,r14) */
L8c041898: *stop=0x8c041898u;if(!valid_ram(r[2],2u))return -3;r[3]=sx(m,r[2],2);goto L8c04189a; /* mov.w @r2,r3 */
L8c04189a: *stop=0x8c04189au;r[3]=r[3]&65535u;goto L8c04189c; /* extu.w r3,r3 */
L8c04189c: *stop=0x8c04189cu;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c04189e; /* cmp/pl r3 */
L8c04189e: if(!(*sr&1u))goto L8c041920;goto L8c0418a0; /* bf 0x8c041920 */
L8c0418a0: *stop=0x8c0418a0u;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[12]);goto L8c0418a2; /* fmov.s @r12,fr5 */
L8c0418a2: *stop=0x8c0418a2u;fcopy(f,8u,9u);goto L8c0418a4; /* fmov fr9,fr8 */
L8c0418a4: *stop=0x8c0418a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[5],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c0418a6; /* fsub fr5,fr8 */
L8c0418a6: *stop=0x8c0418a6u;r[5]=r[7];goto L8c0418a8; /* mov r7,r5 */
L8c0418a8: *stop=0x8c0418a8u;r[4]=r[7];goto L8c0418aa; /* mov r7,r4 */
L8c0418aa: *stop=0x8c0418aau;r[5]+=12u;goto L8c0418ac; /* add #0xc,r5 */
L8c0418ac: *stop=0x8c0418acu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0418ae; /* fmov.s @r4+,fr4 */
L8c0418ae: *stop=0x8c0418aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[8],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0418b0; /* fmul fr8,fr4 */
L8c0418b0: *stop=0x8c0418b0u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0418b2; /* fmov.s @r5+,fr6 */
L8c0418b2: *stop=0x8c0418b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[5],&f->fpscr,&fr[6]);if(status)return status;}goto L8c0418b4; /* fmul fr5,fr6 */
L8c0418b4: *stop=0x8c0418b4u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0418b6; /* fmov.s @r5+,fr7 */
L8c0418b6: *stop=0x8c0418b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[6],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0418b8; /* fadd fr6,fr4 */
L8c0418b8: *stop=0x8c0418b8u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0418ba; /* fmov.s @r4+,fr6 */
L8c0418ba: *stop=0x8c0418bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[8],&f->fpscr,&fr[6]);if(status)return status;}goto L8c0418bc; /* fmul fr8,fr6 */
L8c0418bc: *stop=0x8c0418bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c0418be; /* fmul fr5,fr7 */
L8c0418be: *stop=0x8c0418beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c0418c0; /* fadd fr7,fr6 */
L8c0418c0: *stop=0x8c0418c0u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[4]);goto L8c0418c2; /* fmov.s @r4,fr7 */
L8c0418c2: *stop=0x8c0418c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[8],&f->fpscr,&fr[7]);if(status)return status;}goto L8c0418c4; /* fmul fr8,fr7 */
L8c0418c4: *stop=0x8c0418c4u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[5]);goto L8c0418c6; /* fmov.s @r5,fr8 */
L8c0418c6: *stop=0x8c0418c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c0418c8; /* fmul fr5,fr8 */
L8c0418c8: *stop=0x8c0418cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[8],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c041928; /* bra 0x8c041928 */
L8c041920: *stop=0x8c041920u;r[4]=rd(m,0x8c041b84u,4);goto L8c041922; /* mov.l 0x8c041b84,r4 */
L8c041922: *stop=0x8c041922u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c041924; /* fmov.s @r4+,fr4 */
L8c041924: *stop=0x8c041924u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c041926; /* fmov.s @r4+,fr6 */
L8c041926: *stop=0x8c041926u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[4]);goto L8c041928; /* fmov.s @r4,fr7 */
L8c041928: *stop=0x8c041928u;r[3]=rd(m,0x8c041b88u,4);goto L8c04192a; /* mov.l 0x8c041b88,r3 */
L8c04192a: *stop=0x8c04192au;r[4]=r[15];goto L8c04192c; /* mov r15,r4 */
L8c04192c: *stop=0x8c04192cu;r[4]+=24u;goto L8c04192e; /* add #0x18,r4 */
L8c04192e: *stop=0x8c04192eu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[4]);goto L8c041930; /* fmov.s fr7,@-r4 */
L8c041930: *stop=0x8c041930u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[4]);goto L8c041932; /* fmov.s fr6,@-r4 */
L8c041932: *stop=0x8c041932u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[4]);goto L8c041934; /* fmov.s fr4,@-r4 */
L8c041934: *stop=0x8c041934u;r[4]=r[15];goto L8c041936; /* mov r15,r4 */
L8c041936: *stop=0x8c041936u;r[4]+=24u;goto L8c041938; /* add #0x18,r4 */
L8c041938: *stop=0x8c041938u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c04193a; /* fmov.s @r4+,fr4 */
L8c04193a: *stop=0x8c04193au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c04193c; /* fmov.s @r4+,fr5 */
L8c04193c: *stop=0x8c04193cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c04193e; /* fmov.s @r4,fr6 */
L8c04193e: target=r[3];*pr=0x8c041942u;*stop=0x8c041940u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041942; /* jsr @r3 */
L8c041942: *stop=0x8c041942u;r[4]=r[14];goto L8c041944; /* mov r14,r4 */
L8c041944: *stop=0x8c041944u;r[3]=rd(m,0x8c041b8cu,4);goto L8c041946; /* mov.l 0x8c041b8c,r3 */
L8c041946: *stop=0x8c041946u;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c041948; /* mov.l @r4,r5 */
L8c041948: *stop=0x8c041948u;if(!valid_ram(r[4]+4u,4u))return -3;r[6]=rd(m,r[4]+0x4u,4);goto L8c04194a; /* mov.l @(0x4,r4),r6 */
L8c04194a: *stop=0x8c04194au;if(!valid_ram(r[4]+8u,4u))return -3;r[7]=rd(m,r[4]+0x8u,4);goto L8c04194c; /* mov.l @(0x8,r4),r7 */
L8c04194c: *stop=0x8c04194cu;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c04194e; /* mov.l @r3,r2 */
L8c04194e: target=r[2];*pr=0x8c041952u;*stop=0x8c041950u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041952; /* jsr @r2 */
L8c041952: *stop=0x8c041952u;r[4]=r[15];goto L8c041954; /* mov r15,r4 */
L8c041954: *stop=0x8c041954u;r[3]=rd(m,0x8c041b90u,4);goto L8c041956; /* mov.l 0x8c041b90,r3 */
L8c041956: *stop=0x8c041956u;r[4]+=12u;goto L8c041958; /* add #0xc,r4 */
L8c041958: *stop=0x8c041958u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c04195a; /* fmov.s @r4+,fr4 */
L8c04195a: *stop=0x8c04195au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c04195c; /* fmov.s @r4+,fr5 */
L8c04195c: *stop=0x8c04195cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c04195e; /* fmov.s @r4,fr6 */
L8c04195e: target=r[3];*pr=0x8c041962u;*stop=0x8c041960u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c041962; /* jsr @r3 */
L8c041962: *stop=0x8c041962u;r[15]+=36u;goto L8c041964; /* add #0x24,r15 */
L8c041964: *stop=0x8c041964u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c041966; /* lds.l @r15+,PR */
L8c041966: *stop=0x8c041966u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c041968; /* mov.l @r15+,r12 */
L8c041968: *stop=0x8c041968u;if(*pr!=entry_pr)return -3;*stop=0x8c04196au;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
