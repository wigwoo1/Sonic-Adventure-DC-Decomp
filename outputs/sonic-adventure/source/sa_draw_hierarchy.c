/* Hierarchy matrix preparation/generation/copy/retrieval; names inferred.
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

/* Production default retains the original unsupported-layout infinite loop.
 * Focused research builds stop only after its first BRA/NOP cycle. */
#ifndef SA_BOUND_RETAIL_STALLS
#define SA_BOUND_RETAIL_STALLS 0
#endif
#if SA_BOUND_RETAIL_STALLS
#define SA_HIERARCHY_RETAIL_STALL(stop) do{*(stop)=0x8c05809cu;return -4;}while(0)
#else
#define SA_HIERARCHY_RETAIL_STALL(stop) goto L8c05809c
#endif
SA_STAGE_EXPORT int sa_draw_hierarchy_prepare_8c0583a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0583a0;
L8c0583a0: *stop=0x8c0583a0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0583a2; /* mov.l r14,@-r15 */
L8c0583a2: *stop=0x8c0583a2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c0583a4; /* sts.l PR,@-r15 */
L8c0583a4: *stop=0x8c0583a4u;r[15]+=4294967260u;goto L8c0583a6; /* add #-0x24,r15 */
L8c0583a6: *stop=0x8c0583a6u;r[2]=rd(m,0x8c058480u,4);goto L8c0583a8; /* mov.l 0x8c058480,r2 */
L8c0583a8: *stop=0x8c0583a8u;r[3]=0u;goto L8c0583aa; /* mov #0x0,r3 */
L8c0583aa: *stop=0x8c0583aau;r[14]=r[5];goto L8c0583ac; /* mov r5,r14 */
L8c0583ac: *stop=0x8c0583acu;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[4],4);goto L8c0583ae; /* mov.l r4,@(0x4,r15) */
L8c0583ae: *stop=0x8c0583aeu;r[4]=4294967168u;goto L8c0583b0; /* mov #-0x80,r4 */
L8c0583b0: *stop=0x8c0583b0u;r[5]=rd(m,0x8c058484u,4);goto L8c0583b2; /* mov.l 0x8c058484,r5 */
L8c0583b2: *stop=0x8c0583b2u;r[4]=r[4]&255u;goto L8c0583b4; /* extu.b r4,r4 */
L8c0583b4: *stop=0x8c0583b4u;r[0]=12u;goto L8c0583b6; /* mov #0xc,r0 */
L8c0583b6: *stop=0x8c0583b6u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[15]);goto L8c0583b8; /* fmov fr4,@(r0,r15) */
L8c0583b8: *stop=0x8c0583b8u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[6],4);goto L8c0583ba; /* mov.l r6,@(0x8,r15) */
L8c0583ba: *stop=0x8c0583bau;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[7],4);goto L8c0583bc; /* mov.l r7,@r15 */
L8c0583bc: *stop=0x8c0583bcu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c0583be; /* mov.l r3,@r2 */
L8c0583be: *stop=0x8c0583beu;if(!valid_ram(r[15]+8u,4u))return -3;r[3]=rd(m,r[15]+0x8u,4);goto L8c0583c0; /* mov.l @(0x8,r15),r3 */
L8c0583c0: *stop=0x8c0583c0u;r[3]+=1u;goto L8c0583c2; /* add #0x1,r3 */
L8c0583c2: *stop=0x8c0583c2u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[3],4);goto L8c0583c4; /* mov.l r3,@r5 */
L8c0583c4: *stop=0x8c0583c4u;*sr=(*sr&~1u)|(r[3]>r[4]);goto L8c0583c6; /* cmp/hi r4,r3 */
L8c0583c6: if(!(*sr&1u))goto L8c0583ca;goto L8c0583c8; /* bf 0x8c0583ca */
L8c0583c8: *stop=0x8c0583c8u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[4],4);goto L8c0583ca; /* mov.l r4,@r5 */
L8c0583ca: *stop=0x8c0583cau;r[2]=rd(m,0x8c058488u,4);goto L8c0583cc; /* mov.l 0x8c058488,r2 */
L8c0583cc: target=r[2];*pr=0x8c0583d0u;*stop=0x8c0583ceu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0583d0; /* jsr @r2 */
L8c0583d0: *stop=0x8c0583d0u;if(!valid_ram(r[15]+4u,4u))return -3;r[3]=rd(m,r[15]+0x4u,4);goto L8c0583d2; /* mov.l @(0x4,r15),r3 */
L8c0583d2: *stop=0x8c0583d2u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0583d4; /* tst r3,r3 */
L8c0583d4: if((*sr&1u))goto L8c0583de;goto L8c0583d6; /* bt 0x8c0583de */
L8c0583d6: *stop=0x8c0583d6u;r[3]=rd(m,0x8c05848cu,4);goto L8c0583d8; /* mov.l 0x8c05848c,r3 */
L8c0583d8: *stop=0x8c0583d8u;if(!valid_ram(r[15]+4u,4u))return -3;r[5]=rd(m,r[15]+0x4u,4);goto L8c0583da; /* mov.l @(0x4,r15),r5 */
L8c0583da: target=r[3];*pr=0x8c0583deu;*stop=0x8c0583dcu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0583de; /* jsr @r3 */
L8c0583de: *stop=0x8c0583deu;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c0583e0; /* mov.l @(0x4,r14),r2 */
L8c0583e0: *stop=0x8c0583e0u;r[5]=r[15];goto L8c0583e2; /* mov r15,r5 */
L8c0583e2: *stop=0x8c0583e2u;r[5]+=16u;goto L8c0583e4; /* add #0x10,r5 */
L8c0583e4: *stop=0x8c0583e4u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c0583e6; /* mov.l @r2,r3 */
L8c0583e6: *stop=0x8c0583e6u;r[2]=rd(m,0x8c058490u,4);goto L8c0583e8; /* mov.l 0x8c058490,r2 */
L8c0583e8: *stop=0x8c0583e8u;if(!valid_ram(r[15]+16u,4u))return -3;wr(m,r[15]+0x10u,r[3],4);goto L8c0583ea; /* mov.l r3,@(0x10,r15) */
L8c0583ea: *stop=0x8c0583eau;if(!valid_ram(r[15]+20u,4u))return -3;wr(m,r[15]+0x14u,r[2],4);goto L8c0583ec; /* mov.l r2,@(0x14,r15) */
L8c0583ec: *stop=0x8c0583ecu;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c0583ee; /* mov.l @(0x4,r14),r3 */
L8c0583ee: *stop=0x8c0583eeu;if(!valid_ram(r[3]+8u,2u))return -3;r[0]=sx(m,r[3]+0x8u,2);goto L8c0583f0; /* mov.w @(0x8,r3),r0 */
L8c0583f0: *stop=0x8c0583f0u;r[0]=r[0]&65535u;goto L8c0583f2; /* extu.w r0,r0 */
L8c0583f2: *stop=0x8c0583f2u;if(!valid_ram(r[15]+24u,4u))return -3;wr(m,r[15]+0x18u,r[0],4);goto L8c0583f4; /* mov.l r0,@(0x18,r15) */
L8c0583f4: *stop=0x8c0583f4u;r[0]=12u;goto L8c0583f6; /* mov #0xc,r0 */
L8c0583f6: *stop=0x8c0583f6u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c0583f8; /* mov.l @(0x4,r14),r3 */
L8c0583f8: *stop=0x8c0583f8u;if(!valid_ram(r[3]+4u,4u))return -3;r[2]=rd(m,r[3]+0x4u,4);goto L8c0583fa; /* mov.l @(0x4,r3),r2 */
L8c0583fa: *stop=0x8c0583fau;if(!valid_ram(r[15]+28u,4u))return -3;wr(m,r[15]+0x1cu,r[2],4);goto L8c0583fc; /* mov.l r2,@(0x1c,r15) */
L8c0583fc: *stop=0x8c0583fcu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c0583fe; /* fmov.s @(r0,r15),fr3 */
L8c0583fe: *stop=0x8c0583feu;r[0]=32u;goto L8c058400; /* mov #0x20,r0 */
L8c058400: *stop=0x8c058400u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c058402; /* fmov fr3,@(r0,r15) */
L8c058402: target=0x8c057bc0u;*pr=0x8c058406u;*stop=0x8c058404u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c058406; /* bsr 0x8c057bc0 */
L8c058406: *stop=0x8c058406u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c058408; /* mov.l @r15,r2 */
L8c058408: *stop=0x8c058408u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c05840a; /* tst r2,r2 */
L8c05840a: if((*sr&1u))goto L8c05841e;goto L8c05840c; /* bt 0x8c05841e */
L8c05840c: *stop=0x8c05840cu;if(!valid_ram(r[15]+8u,4u))return -3;r[5]=rd(m,r[15]+0x8u,4);goto L8c05840e; /* mov.l @(0x8,r15),r5 */
L8c05840e: *stop=0x8c05840eu;r[3]=rd(m,0x8c058490u,4);goto L8c058410; /* mov.l 0x8c058490,r3 */
L8c058410: *stop=0x8c058410u;r[5]<<=2;goto L8c058412; /* shll2 r5 */
L8c058412: *stop=0x8c058412u;r[2]=rd(m,0x8c05848cu,4);goto L8c058414; /* mov.l 0x8c05848c,r2 */
L8c058414: *stop=0x8c058414u;r[5]<<=2;goto L8c058416; /* shll2 r5 */
L8c058416: *stop=0x8c058416u;r[5]<<=2;goto L8c058418; /* shll2 r5 */
L8c058418: *stop=0x8c058418u;r[5]+=r[3];goto L8c05841a; /* add r3,r5 */
L8c05841a: target=r[2];*pr=0x8c05841eu;*stop=0x8c05841cu;if(!valid_ram(r[15],4u))return -3;r[4]=rd(m,r[15],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05841e; /* jsr @r2 */
L8c05841e: *stop=0x8c05841eu;r[3]=rd(m,0x8c058494u,4);goto L8c058420; /* mov.l 0x8c058494,r3 */
L8c058420: target=r[3];*pr=0x8c058424u;*stop=0x8c058422u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c058424; /* jsr @r3 */
L8c058424: *stop=0x8c058424u;r[0]=1u;goto L8c058426; /* mov #0x1,r0 */
L8c058426: *stop=0x8c058426u;r[15]+=36u;goto L8c058428; /* add #0x24,r15 */
L8c058428: *stop=0x8c058428u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c05842a; /* lds.l @r15+,PR */
L8c05842a: *stop=0x8c05842au;if(*pr!=entry_pr)return -3;*stop=0x8c05842cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_draw_hierarchy_generate_8c057bc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c057bc0;
L8c057bc0: *stop=0x8c057bc0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c057bc2; /* mov.l r14,@-r15 */
L8c057bc2: *stop=0x8c057bc2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c057bc4; /* mov.l r13,@-r15 */
L8c057bc4: *stop=0x8c057bc4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c057bc6; /* mov.l r12,@-r15 */
L8c057bc6: *stop=0x8c057bc6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c057bc8; /* mov.l r11,@-r15 */
L8c057bc8: *stop=0x8c057bc8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c057bca; /* mov.l r10,@-r15 */
L8c057bca: *stop=0x8c057bcau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c057bcc; /* mov.l r9,@-r15 */
L8c057bcc: *stop=0x8c057bccu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c057bce; /* mov.l r8,@-r15 */
L8c057bce: *stop=0x8c057bceu;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c057bd0; /* fmov.s fr15,@-r15 */
L8c057bd0: *stop=0x8c057bd0u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto L8c057bd2; /* fmov.s fr14,@-r15 */
L8c057bd2: *stop=0x8c057bd2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c057bd4; /* sts.l PR,@-r15 */
L8c057bd4: *stop=0x8c057bd4u;r[15]+=4294967272u;goto L8c057bd6; /* add #-0x18,r15 */
L8c057bd6: *stop=0x8c057bd6u;r[13]=r[4];goto L8c057bd8; /* mov r4,r13 */
L8c057bd8: *stop=0x8c057bd8u;r[4]=rd(m,0x8c057ca4u,4);goto L8c057bda; /* mov.l 0x8c057ca4,r4 */
L8c057bda: *stop=0x8c057bdau;r[14]=r[5];goto L8c057bdc; /* mov r5,r14 */
L8c057bdc: *stop=0x8c057bdcu;r[5]=rd(m,0x8c057ca0u,4);goto L8c057bde; /* mov.l 0x8c057ca0,r5 */
L8c057bde: *stop=0x8c057bdeu;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c057be0; /* mov.l @r4,r3 */
L8c057be0: *stop=0x8c057be0u;r[3]+=1u;goto L8c057be2; /* add #0x1,r3 */
L8c057be2: *stop=0x8c057be2u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[3],4);goto L8c057be4; /* mov.l r3,@r4 */
L8c057be4: *stop=0x8c057be4u;if(!valid_ram(r[5],4u))return -3;r[2]=rd(m,r[5],4);goto L8c057be6; /* mov.l @r5,r2 */
L8c057be6: *stop=0x8c057be6u;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c057be8; /* cmp/hi r2,r3 */
L8c057be8: if(!(*sr&1u))goto L8c057bf2;goto L8c057bea; /* bf 0x8c057bf2 */
L8c057bea: *stop=0x8c057beau;if(!valid_ram(r[5],4u))return -3;r[1]=rd(m,r[5],4);goto L8c057bec; /* mov.l @r5,r1 */
L8c057bec: *stop=0x8c057becu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c057bee; /* mov.l r1,@r4 */
L8c057bee: *stop=0x8c057bf0u;r[0]=0u;goto L8c0580c6; /* bra 0x8c0580c6 */
L8c057bf2: *stop=0x8c057bf2u;if(!valid_ram(r[14]+8u,4u))return -3;r[4]=rd(m,r[14]+0x8u,4);goto L8c057bf4; /* mov.l @(0x8,r14),r4 */
L8c057bf4: *stop=0x8c057bf4u;r[5]=1u;goto L8c057bf6; /* mov #0x1,r5 */
L8c057bf6: *stop=0x8c057bf6u;r[9]=0u;goto L8c057bf8; /* mov #0x0,r9 */
L8c057bf8: *stop=0x8c057bf8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c057bfa; /* tst r4,r4 */
L8c057bfa: take=(*sr&1u);*stop=0x8c057bfcu;r[6]=5u;if(take)goto L8c057c0e;goto L8c057bfe; /* bt/s 0x8c057c0e */
L8c057bfe: *stop=0x8c057bfeu;r[2]=~r[5];goto L8c057c00; /* not r5,r2 */
L8c057c00: *stop=0x8c057c00u;r[9]+=1u;goto L8c057c02; /* add #0x1,r9 */
L8c057c02: *stop=0x8c057c02u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c057c04; /* shll r5 */
L8c057c04: *stop=0x8c057c04u;*sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[6]);goto L8c057c06; /* cmp/ge r6,r9 */
L8c057c06: take=(*sr&1u);*stop=0x8c057c08u;r[4]&=r[2];if(take)goto L8c057c0e;goto L8c057c0a; /* bt/s 0x8c057c0e */
L8c057c0a: *stop=0x8c057c0au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c057c0c; /* tst r4,r4 */
L8c057c0c: if(!(*sr&1u))goto L8c057bfe;goto L8c057c0e; /* bf 0x8c057bfe */
L8c057c0e: *stop=0x8c057c0eu;r[3]=rd(m,0x8c057ca8u,4);goto L8c057c10; /* mov.l 0x8c057ca8,r3 */
L8c057c10: target=r[3];*pr=0x8c057c14u;*stop=0x8c057c12u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057c14; /* jsr @r3 */
L8c057c14: *stop=0x8c057c14u;r[0]=0x8c057cacu;goto L8c057c16; /* mova 0x8c057cac,r0 */
L8c057c16: *stop=0x8c057c16u;r[12]=rd(m,0x8c057cb4u,4);goto L8c057c18; /* mov.l 0x8c057cb4,r12 */
L8c057c18: *stop=0x8c057c18u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]);goto L8c057c1a; /* fmov.s @r0,fr14 */
L8c057c1a: *stop=0x8c057c1au;r[0]=r[9];goto L8c057c1c; /* mov r9,r0 */
L8c057c1c: *stop=0x8c057c1cu;r[11]=rd(m,0x8c057cb0u,4);goto L8c057c1e; /* mov.l 0x8c057cb0,r11 */
L8c057c1e: *stop=0x8c057c1eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c057c20; /* cmp/eq #0x2,r0 */
L8c057c20: take=(*sr&1u);*stop=0x8c057c22u;r[10]=2u;if(take)goto L8c057c30;goto L8c057c24; /* bt/s 0x8c057c30 */
L8c057c24: *stop=0x8c057c24u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c057c26; /* cmp/eq #0x3,r0 */
L8c057c26: if(!(*sr&1u))goto L8c057c2c;goto L8c057c28; /* bf 0x8c057c2c */
L8c057c28: *stop=0x8c057c2au;goto L8c057e1c; /* bra 0x8c057e1c */
L8c057c2c: *stop=0x8c057c2eu;goto L8c05809c; /* bra 0x8c05809c */
L8c057c30: *stop=0x8c057c30u;if(!valid_ram(r[14],4u))return -3;r[8]=rd(m,r[14],4);goto L8c057c32; /* mov.l @r14,r8 */
L8c057c32: *stop=0x8c057c34u;r[9]=0u;goto L8c057e00; /* bra 0x8c057e00 */
L8c057c36: *stop=0x8c057c36u;r[7]=r[9];goto L8c057c38; /* mov r9,r7 */
L8c057c38: *stop=0x8c057c38u;r[2]=r[8];goto L8c057c3a; /* mov r8,r2 */
L8c057c3a: *stop=0x8c057c3au;r[7]<<=2;goto L8c057c3c; /* shll2 r7 */
L8c057c3c: *stop=0x8c057c3cu;r[2]+=r[7];goto L8c057c3e; /* add r7,r2 */
L8c057c3e: *stop=0x8c057c3eu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c057c40; /* mov.l @r2,r3 */
L8c057c40: *stop=0x8c057c40u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c057c42; /* tst r3,r3 */
L8c057c42: if(!(*sr&1u))goto L8c057c80;goto L8c057c44; /* bf 0x8c057c80 */
L8c057c44: *stop=0x8c057c44u;r[0]=r[9];goto L8c057c46; /* mov r9,r0 */
L8c057c46: *stop=0x8c057c46u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c057c48; /* cmp/eq #0x0,r0 */
L8c057c48: if((*sr&1u))goto L8c057c52;goto L8c057c4a; /* bt 0x8c057c52 */
L8c057c4a: *stop=0x8c057c4au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c057c4c; /* cmp/eq #0x1,r0 */
L8c057c4c: if((*sr&1u))goto L8c057c60;goto L8c057c4e; /* bt 0x8c057c60 */
L8c057c4e: *stop=0x8c057c50u;goto L8c057dfe; /* bra 0x8c057dfe */
L8c057c52: *stop=0x8c057c52u;r[3]=rd(m,0x8c057cb8u,4);goto L8c057c54; /* mov.l 0x8c057cb8,r3 */
L8c057c54: *stop=0x8c057c54u;r[5]=r[13];goto L8c057c56; /* mov r13,r5 */
L8c057c56: *stop=0x8c057c56u;r[5]+=8u;goto L8c057c58; /* add #0x8,r5 */
L8c057c58: target=r[3];*pr=0x8c057c5cu;*stop=0x8c057c5au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057c5c; /* jsr @r3 */
L8c057c5c: *stop=0x8c057c5eu;goto L8c057dfe; /* bra 0x8c057dfe */
L8c057c60: *stop=0x8c057c60u;if(!valid_ram(r[13],4u))return -3;r[3]=rd(m,r[13],4);goto L8c057c62; /* mov.l @r13,r3 */
L8c057c62: *stop=0x8c057c62u;*sr=(*sr&~1u)|!(r[10]&r[3]);goto L8c057c64; /* tst r10,r3 */
L8c057c64: if((*sr&1u))goto L8c057c6a;goto L8c057c66; /* bt 0x8c057c6a */
L8c057c66: *stop=0x8c057c68u;goto L8c057dfe; /* bra 0x8c057dfe */
L8c057c6a: *stop=0x8c057c6au;r[7]=r[13];goto L8c057c6c; /* mov r13,r7 */
L8c057c6c: *stop=0x8c057c6cu;r[7]+=20u;goto L8c057c6e; /* add #0x14,r7 */
L8c057c6e: *stop=0x8c057c6eu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[7],4);goto L8c057c70; /* mov.l r7,@r15 */
L8c057c70: *stop=0x8c057c70u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto L8c057c72; /* mov.l @r15,r3 */
L8c057c72: *stop=0x8c057c72u;if(!valid_ram(r[7]+8u,4u))return -3;r[7]=rd(m,r[7]+0x8u,4);goto L8c057c74; /* mov.l @(0x8,r7),r7 */
L8c057c74: *stop=0x8c057c74u;if(!valid_ram(r[3],4u))return -3;r[5]=rd(m,r[3],4);goto L8c057c76; /* mov.l @r3,r5 */
L8c057c76: *stop=0x8c057c76u;if(!valid_ram(r[3]+4u,4u))return -3;r[6]=rd(m,r[3]+0x4u,4);goto L8c057c78; /* mov.l @(0x4,r3),r6 */
L8c057c78: target=r[11];*pr=0x8c057c7cu;*stop=0x8c057c7au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057c7c; /* jsr @r11 */
L8c057c7c: *stop=0x8c057c7eu;goto L8c057dfe; /* bra 0x8c057dfe */
L8c057c80: *stop=0x8c057c80u;r[0]=16u;goto L8c057c82; /* mov #0x10,r0 */
L8c057c82: *stop=0x8c057c82u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c057c84; /* fmov.s @(r0,r14),fr4 */
L8c057c84: *stop=0x8c057c84u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,1);if(status)return status;goto L8c057c86; /* fcmp/gt fr14,fr4 */
L8c057c86: if(!(*sr&1u))goto L8c057cc0;goto L8c057c88; /* bf 0x8c057cc0 */
L8c057c88: *stop=0x8c057c88u;r[0]=0x8c057cbcu;goto L8c057c8a; /* mova 0x8c057cbc,r0 */
L8c057c8a: *stop=0x8c057c8au;fcopy(f,2u,4u);goto L8c057c8c; /* fmov fr4,fr2 */
L8c057c8c: *stop=0x8c057c8cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c057c8e; /* fmov.s @r0,fr3 */
L8c057c8e: *stop=0x8c057c8eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c057c90; /* fadd fr3,fr2 */
L8c057c90: *stop=0x8c057c92u;fcopy(f,3u,2u);goto L8c057cc2; /* bra 0x8c057cc2 */
L8c057cc0: *stop=0x8c057cc0u;fcopy(f,3u,4u);goto L8c057cc2; /* fmov fr4,fr3 */
L8c057cc2: *stop=0x8c057cc2u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c057cc4; /* ftrc fr3,FPUL */
L8c057cc4: *stop=0x8c057cc4u;r[4]=r[8];goto L8c057cc6; /* mov r8,r4 */
L8c057cc6: *stop=0x8c057cc6u;r[4]+=r[7];goto L8c057cc8; /* add r7,r4 */
L8c057cc8: *stop=0x8c057cc8u;if(!valid_ram(r[4],4u))return -3;r[4]=rd(m,r[4],4);goto L8c057cca; /* mov.l @r4,r4 */
L8c057cca: *stop=0x8c057ccau;r[6]=*fpul;goto L8c057ccc; /* sts FPUL,r6 */
L8c057ccc: *stop=0x8c057cccu;r[5]=r[4];goto L8c057cce; /* mov r4,r5 */
L8c057cce: *stop=0x8c057cceu;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c057cd0; /* mov.l @r4,r3 */
L8c057cd0: *stop=0x8c057cd0u;*sr=(*sr&~1u)|(r[6]>r[3]);goto L8c057cd2; /* cmp/hi r3,r6 */
L8c057cd2: take=!(*sr&1u);*stop=0x8c057cd4u;r[5]+=16u;if(take)goto L8c057cec;goto L8c057cd6; /* bf/s 0x8c057cec */
L8c057cd6: *stop=0x8c057cd6u;if(!valid_ram(r[5],4u))return -3;r[1]=rd(m,r[5],4);goto L8c057cd8; /* mov.l @r5,r1 */
L8c057cd8: *stop=0x8c057cd8u;*sr=(*sr&~1u)|(r[6]>=r[1]);goto L8c057cda; /* cmp/hs r1,r6 */
L8c057cda: if(!(*sr&1u))goto L8c057cec;goto L8c057cdc; /* bf 0x8c057cec */
L8c057cdc: *stop=0x8c057cdcu;r[4]+=16u;goto L8c057cde; /* add #0x10,r4 */
L8c057cde: *stop=0x8c057cdeu;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c057ce0; /* mov.l @r4,r2 */
L8c057ce0: *stop=0x8c057ce0u;*sr=(*sr&~1u)|(r[6]>r[2]);goto L8c057ce2; /* cmp/hi r2,r6 */
L8c057ce2: take=!(*sr&1u);*stop=0x8c057ce4u;r[5]+=16u;if(take)goto L8c057cec;goto L8c057ce6; /* bf/s 0x8c057cec */
L8c057ce6: *stop=0x8c057ce6u;if(!valid_ram(r[5],4u))return -3;r[1]=rd(m,r[5],4);goto L8c057ce8; /* mov.l @r5,r1 */
L8c057ce8: *stop=0x8c057ce8u;*sr=(*sr&~1u)|(r[6]>=r[1]);goto L8c057cea; /* cmp/hs r1,r6 */
L8c057cea: if((*sr&1u))goto L8c057cdc;goto L8c057cec; /* bt 0x8c057cdc */
L8c057cec: *stop=0x8c057cecu;if(!valid_ram(r[14]+12u,4u))return -3;r[2]=rd(m,r[14]+0xcu,4);goto L8c057cee; /* mov.l @(0xc,r14),r2 */
L8c057cee: *stop=0x8c057ceeu;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c057cf0; /* mov.l @r4,r3 */
L8c057cf0: *stop=0x8c057cf0u;r[2]+=4294967295u;goto L8c057cf2; /* add #-0x1,r2 */
L8c057cf2: *stop=0x8c057cf2u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c057cf4; /* cmp/eq r2,r3 */
L8c057cf4: if(!(*sr&1u))goto L8c057d16;goto L8c057cf6; /* bf 0x8c057d16 */
L8c057cf6: *stop=0x8c057cf6u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c057cf8; /* mov.l @r4,r3 */
L8c057cf8: *stop=0x8c057cf8u;r[5]=r[8];goto L8c057cfa; /* mov r8,r5 */
L8c057cfa: *stop=0x8c057cfau;r[5]+=r[7];goto L8c057cfc; /* add r7,r5 */
L8c057cfc: *stop=0x8c057cfcu;*fpul=r[3];goto L8c057cfe; /* lds r3,FPUL */
L8c057cfe: *stop=0x8c057cfeu;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c057d00; /* cmp/pz r3 */
L8c057d00: *stop=0x8c057d00u;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c057d02; /* mov.l @r5,r5 */
L8c057d02: take=(*sr&1u);*stop=0x8c057d04u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c057d0e;goto L8c057d06; /* bt/s 0x8c057d0e */
L8c057d06: *stop=0x8c057d06u;r[2]=rd(m,0x8c057e60u,4);goto L8c057d08; /* mov.l 0x8c057e60,r2 */
L8c057d08: *stop=0x8c057d08u;*fpul=r[2];goto L8c057d0a; /* lds r2,FPUL */
L8c057d0a: *stop=0x8c057d0au;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c057d0c; /* fsts FPUL,fr2 */
L8c057d0c: *stop=0x8c057d0cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c057d0e; /* fadd fr2,fr3 */
L8c057d0e: *stop=0x8c057d0eu;r[0]=16u;goto L8c057d10; /* mov #0x10,r0 */
L8c057d10: *stop=0x8c057d10u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]+r[14]);goto L8c057d12; /* fmov.s @(r0,r14),fr15 */
L8c057d12: *stop=0x8c057d14u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c057d46; /* bra 0x8c057d46 */
L8c057d16: *stop=0x8c057d16u;if(!valid_ram(r[4],4u))return -3;r[6]=rd(m,r[4],4);goto L8c057d18; /* mov.l @r4,r6 */
L8c057d18: *stop=0x8c057d18u;*fpul=r[6];goto L8c057d1a; /* lds r6,FPUL */
L8c057d1a: *stop=0x8c057d1au;*sr=(*sr&~1u)|((int32_t)r[6]>=0);goto L8c057d1c; /* cmp/pz r6 */
L8c057d1c: take=(*sr&1u);*stop=0x8c057d1eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c057d28;goto L8c057d20; /* bt/s 0x8c057d28 */
L8c057d20: *stop=0x8c057d20u;r[2]=rd(m,0x8c057e60u,4);goto L8c057d22; /* mov.l 0x8c057e60,r2 */
L8c057d22: *stop=0x8c057d22u;*fpul=r[2];goto L8c057d24; /* lds r2,FPUL */
L8c057d24: *stop=0x8c057d24u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c057d26; /* fsts FPUL,fr2 */
L8c057d26: *stop=0x8c057d26u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c057d28; /* fadd fr2,fr3 */
L8c057d28: *stop=0x8c057d28u;r[0]=16u;goto L8c057d2a; /* mov #0x10,r0 */
L8c057d2a: *stop=0x8c057d2au;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c057d2c; /* mov.l @r5,r3 */
L8c057d2c: *stop=0x8c057d2cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[14]);goto L8c057d2e; /* fmov.s @(r0,r14),fr1 */
L8c057d2e: *stop=0x8c057d2eu;r[3]-=r[6];goto L8c057d30; /* sub r6,r3 */
L8c057d30: *stop=0x8c057d30u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c057d32; /* fsub fr3,fr1 */
L8c057d32: *stop=0x8c057d32u;*fpul=r[3];goto L8c057d34; /* lds r3,FPUL */
L8c057d34: *stop=0x8c057d34u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c057d36; /* cmp/pz r3 */
L8c057d36: take=(*sr&1u);*stop=0x8c057d38u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c057d42;goto L8c057d3a; /* bt/s 0x8c057d42 */
L8c057d3a: *stop=0x8c057d3au;r[2]=rd(m,0x8c057e60u,4);goto L8c057d3c; /* mov.l 0x8c057e60,r2 */
L8c057d3c: *stop=0x8c057d3cu;*fpul=r[2];goto L8c057d3e; /* lds r2,FPUL */
L8c057d3e: *stop=0x8c057d3eu;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c057d40; /* fsts FPUL,fr2 */
L8c057d40: *stop=0x8c057d40u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c057d42; /* fadd fr2,fr3 */
L8c057d42: *stop=0x8c057d42u;fcopy(f,15u,1u);goto L8c057d44; /* fmov fr1,fr15 */
L8c057d44: *stop=0x8c057d44u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[15],&fr[3],&f->fpscr,&fr[15]);if(status)return status;}goto L8c057d46; /* fdiv fr3,fr15 */
L8c057d46: *stop=0x8c057d46u;r[0]=r[9];goto L8c057d48; /* mov r9,r0 */
L8c057d48: *stop=0x8c057d48u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c057d4a; /* cmp/eq #0x0,r0 */
L8c057d4a: if((*sr&1u))goto L8c057d54;goto L8c057d4c; /* bt 0x8c057d54 */
L8c057d4c: *stop=0x8c057d4cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c057d4e; /* cmp/eq #0x1,r0 */
L8c057d4e: if((*sr&1u))goto L8c057d8c;goto L8c057d50; /* bt 0x8c057d8c */
L8c057d50: *stop=0x8c057d52u;goto L8c057dfe; /* bra 0x8c057dfe */
L8c057d54: *stop=0x8c057d54u;r[6]=r[4];goto L8c057d56; /* mov r4,r6 */
L8c057d56: *stop=0x8c057d56u;r[4]=r[5];goto L8c057d58; /* mov r5,r4 */
L8c057d58: *stop=0x8c057d58u;r[4]+=4u;goto L8c057d5a; /* add #0x4,r4 */
L8c057d5a: *stop=0x8c057d5au;fcopy(f,0u,15u);goto L8c057d5c; /* fmov fr15,fr0 */
L8c057d5c: *stop=0x8c057d5cu;r[6]+=4u;goto L8c057d5e; /* add #0x4,r6 */
L8c057d5e: *stop=0x8c057d5eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c057d60; /* fmov.s @r4+,fr3 */
L8c057d60: *stop=0x8c057d60u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[6]);r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c057d62; /* fmov.s @r6+,fr4 */
L8c057d62: *stop=0x8c057d62u;r[3]=rd(m,0x8c057e64u,4);goto L8c057d64; /* mov.l 0x8c057e64,r3 */
L8c057d64: *stop=0x8c057d64u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c057d66; /* fsub fr4,fr3 */
L8c057d66: *stop=0x8c057d66u;fcopy(f,2u,4u);goto L8c057d68; /* fmov fr4,fr2 */
L8c057d68: *stop=0x8c057d68u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[6]);r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c057d6a; /* fmov.s @r6+,fr5 */
L8c057d6a: *stop=0x8c057d6au;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[6]);goto L8c057d6c; /* fmov.s @r6,fr6 */
L8c057d6c: *stop=0x8c057d6cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c057d6e; /* fmac fr0,fr3,fr2 */
L8c057d6e: *stop=0x8c057d6eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c057d70; /* fmov.s @r4+,fr3 */
L8c057d70: *stop=0x8c057d70u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c057d72; /* fsub fr5,fr3 */
L8c057d72: *stop=0x8c057d72u;fcopy(f,4u,2u);goto L8c057d74; /* fmov fr2,fr4 */
L8c057d74: *stop=0x8c057d74u;fcopy(f,2u,5u);goto L8c057d76; /* fmov fr5,fr2 */
L8c057d76: *stop=0x8c057d76u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c057d78; /* fmac fr0,fr3,fr2 */
L8c057d78: *stop=0x8c057d78u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto L8c057d7a; /* fmov.s @r4,fr3 */
L8c057d7a: *stop=0x8c057d7au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c057d7c; /* fsub fr6,fr3 */
L8c057d7c: *stop=0x8c057d7cu;fcopy(f,5u,2u);goto L8c057d7e; /* fmov fr2,fr5 */
L8c057d7e: *stop=0x8c057d7eu;fcopy(f,2u,6u);goto L8c057d80; /* fmov fr6,fr2 */
L8c057d80: *stop=0x8c057d80u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c057d82; /* fmac fr0,fr3,fr2 */
L8c057d82: *stop=0x8c057d82u;fcopy(f,6u,2u);goto L8c057d84; /* fmov fr2,fr6 */
L8c057d84: target=r[3];*pr=0x8c057d88u;*stop=0x8c057d86u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057d88; /* jsr @r3 */
L8c057d88: *stop=0x8c057d8au;goto L8c057dfe; /* bra 0x8c057dfe */
L8c057d8c: *stop=0x8c057d8cu;r[3]=r[5];goto L8c057d8e; /* mov r5,r3 */
L8c057d8e: *stop=0x8c057d8eu;r[3]+=4u;goto L8c057d90; /* add #0x4,r3 */
L8c057d90: *stop=0x8c057d90u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c057d92; /* mov.l r3,@r15 */
L8c057d92: *stop=0x8c057d92u;r[4]+=4u;goto L8c057d94; /* add #0x4,r4 */
L8c057d94: *stop=0x8c057d94u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);r[4]+=4;goto L8c057d96; /* mov.l @r4+,r3 */
L8c057d96: *stop=0x8c057d96u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[3],4);goto L8c057d98; /* mov.l r3,@(0x8,r15) */
L8c057d98: *stop=0x8c057d98u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);r[4]+=4;goto L8c057d9a; /* mov.l @r4+,r3 */
L8c057d9a: *stop=0x8c057d9au;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[3],4);goto L8c057d9c; /* mov.l r3,@(0x4,r15) */
L8c057d9c: *stop=0x8c057d9cu;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c057d9e; /* mov.l @r4,r2 */
L8c057d9e: *stop=0x8c057d9eu;if(!valid_ram(r[15]+12u,4u))return -3;wr(m,r[15]+0xcu,r[2],4);goto L8c057da0; /* mov.l r2,@(0xc,r15) */
L8c057da0: *stop=0x8c057da0u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c057da2; /* mov.l @r15,r5 */
L8c057da2: *stop=0x8c057da2u;r[5]+=4u;goto L8c057da4; /* add #0x4,r5 */
L8c057da4: *stop=0x8c057da4u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto L8c057da6; /* mov.l r5,@r15 */
L8c057da6: *stop=0x8c057da6u;r[5]+=4294967292u;goto L8c057da8; /* add #-0x4,r5 */
L8c057da8: *stop=0x8c057da8u;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c057daa; /* mov.l @r5,r5 */
L8c057daa: target=r[12];*pr=0x8c057daeu;*stop=0x8c057dacu;if(!valid_ram(r[15]+8u,4u))return -3;r[4]=rd(m,r[15]+0x8u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057dae; /* jsr @r12 */
L8c057dae: *stop=0x8c057daeu;if(!valid_ram(r[15]+16u,4u))return -3;wr(m,r[15]+0x10u,r[0],4);goto L8c057db0; /* mov.l r0,@(0x10,r15) */
L8c057db0: *stop=0x8c057db0u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c057db2; /* mov.l @r15,r5 */
L8c057db2: *stop=0x8c057db2u;r[5]+=4u;goto L8c057db4; /* add #0x4,r5 */
L8c057db4: *stop=0x8c057db4u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto L8c057db6; /* mov.l r5,@r15 */
L8c057db6: *stop=0x8c057db6u;r[5]+=4294967292u;goto L8c057db8; /* add #-0x4,r5 */
L8c057db8: *stop=0x8c057db8u;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c057dba; /* mov.l @r5,r5 */
L8c057dba: target=r[12];*pr=0x8c057dbeu;*stop=0x8c057dbcu;if(!valid_ram(r[15]+4u,4u))return -3;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057dbe; /* jsr @r12 */
L8c057dbe: *stop=0x8c057dbeu;if(!valid_ram(r[15]+20u,4u))return -3;wr(m,r[15]+0x14u,r[0],4);goto L8c057dc0; /* mov.l r0,@(0x14,r15) */
L8c057dc0: *stop=0x8c057dc0u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c057dc2; /* mov.l @r15,r5 */
L8c057dc2: *stop=0x8c057dc2u;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c057dc4; /* mov.l @r5,r5 */
L8c057dc4: target=r[12];*pr=0x8c057dc8u;*stop=0x8c057dc6u;if(!valid_ram(r[15]+12u,4u))return -3;r[4]=rd(m,r[15]+0xcu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057dc8; /* jsr @r12 */
L8c057dc8: *stop=0x8c057dc8u;if(!valid_ram(r[15]+16u,4u))return -3;r[3]=rd(m,r[15]+0x10u,4);goto L8c057dca; /* mov.l @(0x10,r15),r3 */
L8c057dca: *stop=0x8c057dcau;r[4]=r[0];goto L8c057dcc; /* mov r0,r4 */
L8c057dcc: *stop=0x8c057dccu;if(!valid_ram(r[15]+20u,4u))return -3;r[2]=rd(m,r[15]+0x14u,4);goto L8c057dce; /* mov.l @(0x14,r15),r2 */
L8c057dce: *stop=0x8c057dceu;*fpul=r[3];goto L8c057dd0; /* lds r3,FPUL */
L8c057dd0: *stop=0x8c057dd0u;if(!valid_ram(r[15]+12u,4u))return -3;r[1]=rd(m,r[15]+0xcu,4);goto L8c057dd2; /* mov.l @(0xc,r15),r1 */
L8c057dd2: *stop=0x8c057dd2u;if(!valid_ram(r[15]+8u,4u))return -3;r[3]=rd(m,r[15]+0x8u,4);goto L8c057dd4; /* mov.l @(0x8,r15),r3 */
L8c057dd4: *stop=0x8c057dd4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c057dd6; /* float FPUL,fr3 */
L8c057dd6: *stop=0x8c057dd6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c057dd8; /* fmul fr15,fr3 */
L8c057dd8: *stop=0x8c057dd8u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c057dda; /* ftrc fr3,FPUL */
L8c057dda: *stop=0x8c057ddau;r[5]=*fpul;goto L8c057ddc; /* sts FPUL,r5 */
L8c057ddc: *stop=0x8c057ddcu;*fpul=r[2];goto L8c057dde; /* lds r2,FPUL */
L8c057dde: *stop=0x8c057ddeu;if(!valid_ram(r[15]+4u,4u))return -3;r[2]=rd(m,r[15]+0x4u,4);goto L8c057de0; /* mov.l @(0x4,r15),r2 */
L8c057de0: *stop=0x8c057de0u;r[5]+=r[3];goto L8c057de2; /* add r3,r5 */
L8c057de2: *stop=0x8c057de2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c057de4; /* float FPUL,fr3 */
L8c057de4: *stop=0x8c057de4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c057de6; /* fmul fr15,fr3 */
L8c057de6: *stop=0x8c057de6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c057de8; /* ftrc fr3,FPUL */
L8c057de8: *stop=0x8c057de8u;r[6]=*fpul;goto L8c057dea; /* sts FPUL,r6 */
L8c057dea: *stop=0x8c057deau;*fpul=r[4];goto L8c057dec; /* lds r4,FPUL */
L8c057dec: *stop=0x8c057decu;r[6]+=r[2];goto L8c057dee; /* add r2,r6 */
L8c057dee: *stop=0x8c057deeu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c057df0; /* float FPUL,fr3 */
L8c057df0: *stop=0x8c057df0u;fcopy(f,2u,3u);goto L8c057df2; /* fmov fr3,fr2 */
L8c057df2: *stop=0x8c057df2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c057df4; /* fmul fr15,fr2 */
L8c057df4: *stop=0x8c057df4u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c057df6; /* ftrc fr2,FPUL */
L8c057df6: *stop=0x8c057df6u;r[7]=*fpul;goto L8c057df8; /* sts FPUL,r7 */
L8c057df8: *stop=0x8c057df8u;r[7]+=r[1];goto L8c057dfa; /* add r1,r7 */
L8c057dfa: target=r[11];*pr=0x8c057dfeu;*stop=0x8c057dfcu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057dfe; /* jsr @r11 */
L8c057dfe: *stop=0x8c057dfeu;r[9]+=1u;goto L8c057e00; /* add #0x1,r9 */
L8c057e00: *stop=0x8c057e00u;*sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[10]);goto L8c057e02; /* cmp/ge r10,r9 */
L8c057e02: if((*sr&1u))goto L8c057e08;goto L8c057e04; /* bt 0x8c057e08 */
L8c057e04: *stop=0x8c057e06u;goto L8c057c36; /* bra 0x8c057c36 */
L8c057e08: *stop=0x8c057e08u;if(!valid_ram(r[13],4u))return -3;r[0]=rd(m,r[13],4);goto L8c057e0a; /* mov.l @r13,r0 */
L8c057e0a: *stop=0x8c057e0au;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c057e0c; /* tst #0x4,r0 */
L8c057e0c: if(!(*sr&1u))goto L8c057e18;goto L8c057e0e; /* bf 0x8c057e18 */
L8c057e0e: *stop=0x8c057e0eu;r[2]=rd(m,0x8c057e68u,4);goto L8c057e10; /* mov.l 0x8c057e68,r2 */
L8c057e10: *stop=0x8c057e10u;r[5]=r[13];goto L8c057e12; /* mov r13,r5 */
L8c057e12: *stop=0x8c057e12u;r[5]+=32u;goto L8c057e14; /* add #0x20,r5 */
L8c057e14: target=r[2];*pr=0x8c057e18u;*stop=0x8c057e16u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057e18; /* jsr @r2 */
L8c057e18: *stop=0x8c057e1au;r[8]+=16u;goto L8c058098; /* bra 0x8c058098 */
L8c057e1c: *stop=0x8c057e1cu;if(!valid_ram(r[14],4u))return -3;r[8]=rd(m,r[14],4);goto L8c057e1e; /* mov.l @r14,r8 */
L8c057e1e: *stop=0x8c057e20u;r[9]=0u;goto L8c05808c; /* bra 0x8c05808c */
L8c057e22: *stop=0x8c057e22u;r[7]=r[9];goto L8c057e24; /* mov r9,r7 */
L8c057e24: *stop=0x8c057e24u;r[3]=r[8];goto L8c057e26; /* mov r8,r3 */
L8c057e26: *stop=0x8c057e26u;r[7]<<=2;goto L8c057e28; /* shll2 r7 */
L8c057e28: *stop=0x8c057e28u;r[3]+=r[7];goto L8c057e2a; /* add r7,r3 */
L8c057e2a: *stop=0x8c057e2au;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c057e2c; /* mov.l @r3,r2 */
L8c057e2c: *stop=0x8c057e2cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c057e2e; /* tst r2,r2 */
L8c057e2e: if(!(*sr&1u))goto L8c057ed0;goto L8c057e30; /* bf 0x8c057ed0 */
L8c057e30: *stop=0x8c057e30u;r[0]=r[9];goto L8c057e32; /* mov r9,r0 */
L8c057e32: *stop=0x8c057e32u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c057e34; /* cmp/eq #0x0,r0 */
L8c057e34: if((*sr&1u))goto L8c057e80;goto L8c057e36; /* bt 0x8c057e80 */
L8c057e36: *stop=0x8c057e36u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c057e38; /* cmp/eq #0x1,r0 */
L8c057e38: if((*sr&1u))goto L8c057e98;goto L8c057e3a; /* bt 0x8c057e98 */
L8c057e3a: *stop=0x8c057e3au;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c057e3c; /* cmp/eq #0x2,r0 */
L8c057e3c: if((*sr&1u))goto L8c057eb8;goto L8c057e3e; /* bt 0x8c057eb8 */
L8c057e3e: *stop=0x8c057e40u;goto L8c05808a; /* bra 0x8c05808a */
L8c057e80: *stop=0x8c057e80u;if(!valid_ram(r[13],4u))return -3;r[0]=rd(m,r[13],4);goto L8c057e82; /* mov.l @r13,r0 */
L8c057e82: *stop=0x8c057e82u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c057e84; /* tst #0x1,r0 */
L8c057e84: if((*sr&1u))goto L8c057e8a;goto L8c057e86; /* bt 0x8c057e8a */
L8c057e86: *stop=0x8c057e88u;goto L8c05808a; /* bra 0x8c05808a */
L8c057e8a: *stop=0x8c057e8au;r[2]=rd(m,0x8c057fc0u,4);goto L8c057e8c; /* mov.l 0x8c057fc0,r2 */
L8c057e8c: *stop=0x8c057e8cu;r[5]=r[13];goto L8c057e8e; /* mov r13,r5 */
L8c057e8e: *stop=0x8c057e8eu;r[5]+=8u;goto L8c057e90; /* add #0x8,r5 */
L8c057e90: target=r[2];*pr=0x8c057e94u;*stop=0x8c057e92u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057e94; /* jsr @r2 */
L8c057e94: *stop=0x8c057e96u;goto L8c05808a; /* bra 0x8c05808a */
L8c057e98: *stop=0x8c057e98u;if(!valid_ram(r[13],4u))return -3;r[2]=rd(m,r[13],4);goto L8c057e9a; /* mov.l @r13,r2 */
L8c057e9a: *stop=0x8c057e9au;*sr=(*sr&~1u)|!(r[10]&r[2]);goto L8c057e9c; /* tst r10,r2 */
L8c057e9c: if((*sr&1u))goto L8c057ea2;goto L8c057e9e; /* bt 0x8c057ea2 */
L8c057e9e: *stop=0x8c057ea0u;goto L8c05808a; /* bra 0x8c05808a */
L8c057ea2: *stop=0x8c057ea2u;r[7]=r[13];goto L8c057ea4; /* mov r13,r7 */
L8c057ea4: *stop=0x8c057ea4u;r[7]+=20u;goto L8c057ea6; /* add #0x14,r7 */
L8c057ea6: *stop=0x8c057ea6u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[7],4);goto L8c057ea8; /* mov.l r7,@r15 */
L8c057ea8: *stop=0x8c057ea8u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto L8c057eaa; /* mov.l @r15,r3 */
L8c057eaa: *stop=0x8c057eaau;if(!valid_ram(r[7]+8u,4u))return -3;r[7]=rd(m,r[7]+0x8u,4);goto L8c057eac; /* mov.l @(0x8,r7),r7 */
L8c057eac: *stop=0x8c057eacu;if(!valid_ram(r[3],4u))return -3;r[5]=rd(m,r[3],4);goto L8c057eae; /* mov.l @r3,r5 */
L8c057eae: *stop=0x8c057eaeu;if(!valid_ram(r[3]+4u,4u))return -3;r[6]=rd(m,r[3]+0x4u,4);goto L8c057eb0; /* mov.l @(0x4,r3),r6 */
L8c057eb0: target=r[11];*pr=0x8c057eb4u;*stop=0x8c057eb2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057eb4; /* jsr @r11 */
L8c057eb4: *stop=0x8c057eb6u;goto L8c05808a; /* bra 0x8c05808a */
L8c057eb8: *stop=0x8c057eb8u;if(!valid_ram(r[13],4u))return -3;r[0]=rd(m,r[13],4);goto L8c057eba; /* mov.l @r13,r0 */
L8c057eba: *stop=0x8c057ebau;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c057ebc; /* tst #0x4,r0 */
L8c057ebc: if((*sr&1u))goto L8c057ec2;goto L8c057ebe; /* bt 0x8c057ec2 */
L8c057ebe: *stop=0x8c057ec0u;goto L8c05808a; /* bra 0x8c05808a */
L8c057ec2: *stop=0x8c057ec2u;r[3]=rd(m,0x8c057fc4u,4);goto L8c057ec4; /* mov.l 0x8c057fc4,r3 */
L8c057ec4: *stop=0x8c057ec4u;r[5]=r[13];goto L8c057ec6; /* mov r13,r5 */
L8c057ec6: *stop=0x8c057ec6u;r[5]+=32u;goto L8c057ec8; /* add #0x20,r5 */
L8c057ec8: target=r[3];*pr=0x8c057eccu;*stop=0x8c057ecau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057ecc; /* jsr @r3 */
L8c057ecc: *stop=0x8c057eceu;goto L8c05808a; /* bra 0x8c05808a */
L8c057ed0: *stop=0x8c057ed0u;r[0]=16u;goto L8c057ed2; /* mov #0x10,r0 */
L8c057ed2: *stop=0x8c057ed2u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c057ed4; /* fmov.s @(r0,r14),fr4 */
L8c057ed4: *stop=0x8c057ed4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,1);if(status)return status;goto L8c057ed6; /* fcmp/gt fr14,fr4 */
L8c057ed6: if(!(*sr&1u))goto L8c057ee4;goto L8c057ed8; /* bf 0x8c057ee4 */
L8c057ed8: *stop=0x8c057ed8u;r[0]=0x8c057fc8u;goto L8c057eda; /* mova 0x8c057fc8,r0 */
L8c057eda: *stop=0x8c057edau;fcopy(f,2u,4u);goto L8c057edc; /* fmov fr4,fr2 */
L8c057edc: *stop=0x8c057edcu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c057ede; /* fmov.s @r0,fr3 */
L8c057ede: *stop=0x8c057edeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c057ee0; /* fadd fr3,fr2 */
L8c057ee0: *stop=0x8c057ee2u;fcopy(f,3u,2u);goto L8c057ee6; /* bra 0x8c057ee6 */
L8c057ee4: *stop=0x8c057ee4u;fcopy(f,3u,4u);goto L8c057ee6; /* fmov fr4,fr3 */
L8c057ee6: *stop=0x8c057ee6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c057ee8; /* ftrc fr3,FPUL */
L8c057ee8: *stop=0x8c057ee8u;r[4]=r[8];goto L8c057eea; /* mov r8,r4 */
L8c057eea: *stop=0x8c057eeau;r[4]+=r[7];goto L8c057eec; /* add r7,r4 */
L8c057eec: *stop=0x8c057eecu;if(!valid_ram(r[4],4u))return -3;r[4]=rd(m,r[4],4);goto L8c057eee; /* mov.l @r4,r4 */
L8c057eee: *stop=0x8c057eeeu;r[6]=*fpul;goto L8c057ef0; /* sts FPUL,r6 */
L8c057ef0: *stop=0x8c057ef0u;r[5]=r[4];goto L8c057ef2; /* mov r4,r5 */
L8c057ef2: *stop=0x8c057ef2u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c057ef4; /* mov.l @r4,r3 */
L8c057ef4: *stop=0x8c057ef4u;*sr=(*sr&~1u)|(r[6]>r[3]);goto L8c057ef6; /* cmp/hi r3,r6 */
L8c057ef6: take=!(*sr&1u);*stop=0x8c057ef8u;r[5]+=16u;if(take)goto L8c057f10;goto L8c057efa; /* bf/s 0x8c057f10 */
L8c057efa: *stop=0x8c057efau;if(!valid_ram(r[5],4u))return -3;r[1]=rd(m,r[5],4);goto L8c057efc; /* mov.l @r5,r1 */
L8c057efc: *stop=0x8c057efcu;*sr=(*sr&~1u)|(r[6]>=r[1]);goto L8c057efe; /* cmp/hs r1,r6 */
L8c057efe: if(!(*sr&1u))goto L8c057f10;goto L8c057f00; /* bf 0x8c057f10 */
L8c057f00: *stop=0x8c057f00u;r[4]+=16u;goto L8c057f02; /* add #0x10,r4 */
L8c057f02: *stop=0x8c057f02u;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c057f04; /* mov.l @r4,r2 */
L8c057f04: *stop=0x8c057f04u;*sr=(*sr&~1u)|(r[6]>r[2]);goto L8c057f06; /* cmp/hi r2,r6 */
L8c057f06: take=!(*sr&1u);*stop=0x8c057f08u;r[5]+=16u;if(take)goto L8c057f10;goto L8c057f0a; /* bf/s 0x8c057f10 */
L8c057f0a: *stop=0x8c057f0au;if(!valid_ram(r[5],4u))return -3;r[1]=rd(m,r[5],4);goto L8c057f0c; /* mov.l @r5,r1 */
L8c057f0c: *stop=0x8c057f0cu;*sr=(*sr&~1u)|(r[6]>=r[1]);goto L8c057f0e; /* cmp/hs r1,r6 */
L8c057f0e: if((*sr&1u))goto L8c057f00;goto L8c057f10; /* bt 0x8c057f00 */
L8c057f10: *stop=0x8c057f10u;if(!valid_ram(r[14]+12u,4u))return -3;r[2]=rd(m,r[14]+0xcu,4);goto L8c057f12; /* mov.l @(0xc,r14),r2 */
L8c057f12: *stop=0x8c057f12u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c057f14; /* mov.l @r4,r3 */
L8c057f14: *stop=0x8c057f14u;r[2]+=4294967295u;goto L8c057f16; /* add #-0x1,r2 */
L8c057f16: *stop=0x8c057f16u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c057f18; /* cmp/eq r2,r3 */
L8c057f18: if(!(*sr&1u))goto L8c057f3a;goto L8c057f1a; /* bf 0x8c057f3a */
L8c057f1a: *stop=0x8c057f1au;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c057f1c; /* mov.l @r4,r3 */
L8c057f1c: *stop=0x8c057f1cu;r[5]=r[8];goto L8c057f1e; /* mov r8,r5 */
L8c057f1e: *stop=0x8c057f1eu;r[5]+=r[7];goto L8c057f20; /* add r7,r5 */
L8c057f20: *stop=0x8c057f20u;*fpul=r[3];goto L8c057f22; /* lds r3,FPUL */
L8c057f22: *stop=0x8c057f22u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c057f24; /* cmp/pz r3 */
L8c057f24: *stop=0x8c057f24u;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c057f26; /* mov.l @r5,r5 */
L8c057f26: take=(*sr&1u);*stop=0x8c057f28u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c057f32;goto L8c057f2a; /* bt/s 0x8c057f32 */
L8c057f2a: *stop=0x8c057f2au;r[2]=rd(m,0x8c057fccu,4);goto L8c057f2c; /* mov.l 0x8c057fcc,r2 */
L8c057f2c: *stop=0x8c057f2cu;*fpul=r[2];goto L8c057f2e; /* lds r2,FPUL */
L8c057f2e: *stop=0x8c057f2eu;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c057f30; /* fsts FPUL,fr2 */
L8c057f30: *stop=0x8c057f30u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c057f32; /* fadd fr2,fr3 */
L8c057f32: *stop=0x8c057f32u;r[0]=16u;goto L8c057f34; /* mov #0x10,r0 */
L8c057f34: *stop=0x8c057f34u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]+r[14]);goto L8c057f36; /* fmov.s @(r0,r14),fr15 */
L8c057f36: *stop=0x8c057f38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c057f6a; /* bra 0x8c057f6a */
L8c057f3a: *stop=0x8c057f3au;if(!valid_ram(r[4],4u))return -3;r[6]=rd(m,r[4],4);goto L8c057f3c; /* mov.l @r4,r6 */
L8c057f3c: *stop=0x8c057f3cu;*fpul=r[6];goto L8c057f3e; /* lds r6,FPUL */
L8c057f3e: *stop=0x8c057f3eu;*sr=(*sr&~1u)|((int32_t)r[6]>=0);goto L8c057f40; /* cmp/pz r6 */
L8c057f40: take=(*sr&1u);*stop=0x8c057f42u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c057f4c;goto L8c057f44; /* bt/s 0x8c057f4c */
L8c057f44: *stop=0x8c057f44u;r[2]=rd(m,0x8c057fccu,4);goto L8c057f46; /* mov.l 0x8c057fcc,r2 */
L8c057f46: *stop=0x8c057f46u;*fpul=r[2];goto L8c057f48; /* lds r2,FPUL */
L8c057f48: *stop=0x8c057f48u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c057f4a; /* fsts FPUL,fr2 */
L8c057f4a: *stop=0x8c057f4au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c057f4c; /* fadd fr2,fr3 */
L8c057f4c: *stop=0x8c057f4cu;r[0]=16u;goto L8c057f4e; /* mov #0x10,r0 */
L8c057f4e: *stop=0x8c057f4eu;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c057f50; /* mov.l @r5,r3 */
L8c057f50: *stop=0x8c057f50u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[14]);goto L8c057f52; /* fmov.s @(r0,r14),fr1 */
L8c057f52: *stop=0x8c057f52u;r[3]-=r[6];goto L8c057f54; /* sub r6,r3 */
L8c057f54: *stop=0x8c057f54u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c057f56; /* fsub fr3,fr1 */
L8c057f56: *stop=0x8c057f56u;*fpul=r[3];goto L8c057f58; /* lds r3,FPUL */
L8c057f58: *stop=0x8c057f58u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c057f5a; /* cmp/pz r3 */
L8c057f5a: take=(*sr&1u);*stop=0x8c057f5cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c057f66;goto L8c057f5e; /* bt/s 0x8c057f66 */
L8c057f5e: *stop=0x8c057f5eu;r[2]=rd(m,0x8c057fccu,4);goto L8c057f60; /* mov.l 0x8c057fcc,r2 */
L8c057f60: *stop=0x8c057f60u;*fpul=r[2];goto L8c057f62; /* lds r2,FPUL */
L8c057f62: *stop=0x8c057f62u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c057f64; /* fsts FPUL,fr2 */
L8c057f64: *stop=0x8c057f64u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c057f66; /* fadd fr2,fr3 */
L8c057f66: *stop=0x8c057f66u;fcopy(f,15u,1u);goto L8c057f68; /* fmov fr1,fr15 */
L8c057f68: *stop=0x8c057f68u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[15],&fr[3],&f->fpscr,&fr[15]);if(status)return status;}goto L8c057f6a; /* fdiv fr3,fr15 */
L8c057f6a: *stop=0x8c057f6au;r[0]=r[9];goto L8c057f6c; /* mov r9,r0 */
L8c057f6c: *stop=0x8c057f6cu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c057f6e; /* cmp/eq #0x0,r0 */
L8c057f6e: if((*sr&1u))goto L8c057f80;goto L8c057f70; /* bt 0x8c057f80 */
L8c057f70: *stop=0x8c057f70u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c057f72; /* cmp/eq #0x1,r0 */
L8c057f72: if((*sr&1u))goto L8c057fe0;goto L8c057f74; /* bt 0x8c057fe0 */
L8c057f74: *stop=0x8c057f74u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c057f76; /* cmp/eq #0x2,r0 */
L8c057f76: if(!(*sr&1u))goto L8c057f7c;goto L8c057f78; /* bf 0x8c057f7c */
L8c057f78: *stop=0x8c057f7au;goto L8c058056; /* bra 0x8c058056 */
L8c057f7c: *stop=0x8c057f7eu;goto L8c05808a; /* bra 0x8c05808a */
L8c057f80: *stop=0x8c057f80u;r[6]=r[4];goto L8c057f82; /* mov r4,r6 */
L8c057f82: *stop=0x8c057f82u;r[4]=r[5];goto L8c057f84; /* mov r5,r4 */
L8c057f84: *stop=0x8c057f84u;r[4]+=4u;goto L8c057f86; /* add #0x4,r4 */
L8c057f86: *stop=0x8c057f86u;fcopy(f,0u,15u);goto L8c057f88; /* fmov fr15,fr0 */
L8c057f88: *stop=0x8c057f88u;r[6]+=4u;goto L8c057f8a; /* add #0x4,r6 */
L8c057f8a: *stop=0x8c057f8au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c057f8c; /* fmov.s @r4+,fr3 */
L8c057f8c: *stop=0x8c057f8cu;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[6]);r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c057f8e; /* fmov.s @r6+,fr4 */
L8c057f8e: *stop=0x8c057f8eu;r[3]=rd(m,0x8c057fd0u,4);goto L8c057f90; /* mov.l 0x8c057fd0,r3 */
L8c057f90: *stop=0x8c057f90u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c057f92; /* fsub fr4,fr3 */
L8c057f92: *stop=0x8c057f92u;fcopy(f,2u,4u);goto L8c057f94; /* fmov fr4,fr2 */
L8c057f94: *stop=0x8c057f94u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[6]);r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c057f96; /* fmov.s @r6+,fr5 */
L8c057f96: *stop=0x8c057f96u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[6]);goto L8c057f98; /* fmov.s @r6,fr6 */
L8c057f98: *stop=0x8c057f98u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c057f9a; /* fmac fr0,fr3,fr2 */
L8c057f9a: *stop=0x8c057f9au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c057f9c; /* fmov.s @r4+,fr3 */
L8c057f9c: *stop=0x8c057f9cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c057f9e; /* fsub fr5,fr3 */
L8c057f9e: *stop=0x8c057f9eu;fcopy(f,4u,2u);goto L8c057fa0; /* fmov fr2,fr4 */
L8c057fa0: *stop=0x8c057fa0u;fcopy(f,2u,5u);goto L8c057fa2; /* fmov fr5,fr2 */
L8c057fa2: *stop=0x8c057fa2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c057fa4; /* fmac fr0,fr3,fr2 */
L8c057fa4: *stop=0x8c057fa4u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto L8c057fa6; /* fmov.s @r4,fr3 */
L8c057fa6: *stop=0x8c057fa6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c057fa8; /* fsub fr6,fr3 */
L8c057fa8: *stop=0x8c057fa8u;fcopy(f,5u,2u);goto L8c057faa; /* fmov fr2,fr5 */
L8c057faa: *stop=0x8c057faau;fcopy(f,2u,6u);goto L8c057fac; /* fmov fr6,fr2 */
L8c057fac: *stop=0x8c057facu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c057fae; /* fmac fr0,fr3,fr2 */
L8c057fae: *stop=0x8c057faeu;fcopy(f,6u,2u);goto L8c057fb0; /* fmov fr2,fr6 */
L8c057fb0: target=r[3];*pr=0x8c057fb4u;*stop=0x8c057fb2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c057fb4; /* jsr @r3 */
L8c057fb4: *stop=0x8c057fb6u;goto L8c05808a; /* bra 0x8c05808a */
L8c057fe0: *stop=0x8c057fe0u;r[3]=r[5];goto L8c057fe2; /* mov r5,r3 */
L8c057fe2: *stop=0x8c057fe2u;r[3]+=4u;goto L8c057fe4; /* add #0x4,r3 */
L8c057fe4: *stop=0x8c057fe4u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c057fe6; /* mov.l r3,@r15 */
L8c057fe6: *stop=0x8c057fe6u;r[4]+=4u;goto L8c057fe8; /* add #0x4,r4 */
L8c057fe8: *stop=0x8c057fe8u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);r[4]+=4;goto L8c057fea; /* mov.l @r4+,r3 */
L8c057fea: *stop=0x8c057feau;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[3],4);goto L8c057fec; /* mov.l r3,@(0x8,r15) */
L8c057fec: *stop=0x8c057fecu;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);r[4]+=4;goto L8c057fee; /* mov.l @r4+,r3 */
L8c057fee: *stop=0x8c057feeu;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[3],4);goto L8c057ff0; /* mov.l r3,@(0x4,r15) */
L8c057ff0: *stop=0x8c057ff0u;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c057ff2; /* mov.l @r4,r2 */
L8c057ff2: *stop=0x8c057ff2u;if(!valid_ram(r[15]+12u,4u))return -3;wr(m,r[15]+0xcu,r[2],4);goto L8c057ff4; /* mov.l r2,@(0xc,r15) */
L8c057ff4: *stop=0x8c057ff4u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c057ff6; /* mov.l @r15,r5 */
L8c057ff6: *stop=0x8c057ff6u;r[5]+=4u;goto L8c057ff8; /* add #0x4,r5 */
L8c057ff8: *stop=0x8c057ff8u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto L8c057ffa; /* mov.l r5,@r15 */
L8c057ffa: *stop=0x8c057ffau;r[5]+=4294967292u;goto L8c057ffc; /* add #-0x4,r5 */
L8c057ffc: *stop=0x8c057ffcu;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c057ffe; /* mov.l @r5,r5 */
L8c057ffe: target=r[12];*pr=0x8c058002u;*stop=0x8c058000u;if(!valid_ram(r[15]+8u,4u))return -3;r[4]=rd(m,r[15]+0x8u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c058002; /* jsr @r12 */
L8c058002: *stop=0x8c058002u;if(!valid_ram(r[15]+20u,4u))return -3;wr(m,r[15]+0x14u,r[0],4);goto L8c058004; /* mov.l r0,@(0x14,r15) */
L8c058004: *stop=0x8c058004u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c058006; /* mov.l @r15,r5 */
L8c058006: *stop=0x8c058006u;r[5]+=4u;goto L8c058008; /* add #0x4,r5 */
L8c058008: *stop=0x8c058008u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto L8c05800a; /* mov.l r5,@r15 */
L8c05800a: *stop=0x8c05800au;r[5]+=4294967292u;goto L8c05800c; /* add #-0x4,r5 */
L8c05800c: *stop=0x8c05800cu;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c05800e; /* mov.l @r5,r5 */
L8c05800e: target=r[12];*pr=0x8c058012u;*stop=0x8c058010u;if(!valid_ram(r[15]+4u,4u))return -3;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c058012; /* jsr @r12 */
L8c058012: *stop=0x8c058012u;if(!valid_ram(r[15]+16u,4u))return -3;wr(m,r[15]+0x10u,r[0],4);goto L8c058014; /* mov.l r0,@(0x10,r15) */
L8c058014: *stop=0x8c058014u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c058016; /* mov.l @r15,r5 */
L8c058016: *stop=0x8c058016u;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c058018; /* mov.l @r5,r5 */
L8c058018: target=r[12];*pr=0x8c05801cu;*stop=0x8c05801au;if(!valid_ram(r[15]+12u,4u))return -3;r[4]=rd(m,r[15]+0xcu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05801c; /* jsr @r12 */
L8c05801c: *stop=0x8c05801cu;if(!valid_ram(r[15]+20u,4u))return -3;r[3]=rd(m,r[15]+0x14u,4);goto L8c05801e; /* mov.l @(0x14,r15),r3 */
L8c05801e: *stop=0x8c05801eu;r[4]=r[0];goto L8c058020; /* mov r0,r4 */
L8c058020: *stop=0x8c058020u;if(!valid_ram(r[15]+16u,4u))return -3;r[2]=rd(m,r[15]+0x10u,4);goto L8c058022; /* mov.l @(0x10,r15),r2 */
L8c058022: *stop=0x8c058022u;*fpul=r[3];goto L8c058024; /* lds r3,FPUL */
L8c058024: *stop=0x8c058024u;if(!valid_ram(r[15]+12u,4u))return -3;r[1]=rd(m,r[15]+0xcu,4);goto L8c058026; /* mov.l @(0xc,r15),r1 */
L8c058026: *stop=0x8c058026u;if(!valid_ram(r[15]+8u,4u))return -3;r[3]=rd(m,r[15]+0x8u,4);goto L8c058028; /* mov.l @(0x8,r15),r3 */
L8c058028: *stop=0x8c058028u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c05802a; /* float FPUL,fr3 */
L8c05802a: *stop=0x8c05802au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c05802c; /* fmul fr15,fr3 */
L8c05802c: *stop=0x8c05802cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c05802e; /* ftrc fr3,FPUL */
L8c05802e: *stop=0x8c05802eu;r[5]=*fpul;goto L8c058030; /* sts FPUL,r5 */
L8c058030: *stop=0x8c058030u;*fpul=r[2];goto L8c058032; /* lds r2,FPUL */
L8c058032: *stop=0x8c058032u;if(!valid_ram(r[15]+4u,4u))return -3;r[2]=rd(m,r[15]+0x4u,4);goto L8c058034; /* mov.l @(0x4,r15),r2 */
L8c058034: *stop=0x8c058034u;r[5]+=r[3];goto L8c058036; /* add r3,r5 */
L8c058036: *stop=0x8c058036u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c058038; /* float FPUL,fr3 */
L8c058038: *stop=0x8c058038u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c05803a; /* fmul fr15,fr3 */
L8c05803a: *stop=0x8c05803au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c05803c; /* ftrc fr3,FPUL */
L8c05803c: *stop=0x8c05803cu;r[6]=*fpul;goto L8c05803e; /* sts FPUL,r6 */
L8c05803e: *stop=0x8c05803eu;*fpul=r[4];goto L8c058040; /* lds r4,FPUL */
L8c058040: *stop=0x8c058040u;r[6]+=r[2];goto L8c058042; /* add r2,r6 */
L8c058042: *stop=0x8c058042u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c058044; /* float FPUL,fr3 */
L8c058044: *stop=0x8c058044u;fcopy(f,2u,3u);goto L8c058046; /* fmov fr3,fr2 */
L8c058046: *stop=0x8c058046u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c058048; /* fmul fr15,fr2 */
L8c058048: *stop=0x8c058048u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c05804a; /* ftrc fr2,FPUL */
L8c05804a: *stop=0x8c05804au;r[7]=*fpul;goto L8c05804c; /* sts FPUL,r7 */
L8c05804c: *stop=0x8c05804cu;r[7]+=r[1];goto L8c05804e; /* add r1,r7 */
L8c05804e: target=r[11];*pr=0x8c058052u;*stop=0x8c058050u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c058052; /* jsr @r11 */
L8c058052: *stop=0x8c058054u;goto L8c05808a; /* bra 0x8c05808a */
L8c058056: *stop=0x8c058056u;r[6]=r[4];goto L8c058058; /* mov r4,r6 */
L8c058058: *stop=0x8c058058u;r[4]=r[5];goto L8c05805a; /* mov r5,r4 */
L8c05805a: *stop=0x8c05805au;r[4]+=4u;goto L8c05805c; /* add #0x4,r4 */
L8c05805c: *stop=0x8c05805cu;fcopy(f,0u,15u);goto L8c05805e; /* fmov fr15,fr0 */
L8c05805e: *stop=0x8c05805eu;r[6]+=4u;goto L8c058060; /* add #0x4,r6 */
L8c058060: *stop=0x8c058060u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c058062; /* fmov.s @r4+,fr3 */
L8c058062: *stop=0x8c058062u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[6]);r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c058064; /* fmov.s @r6+,fr4 */
L8c058064: *stop=0x8c058064u;r[3]=rd(m,0x8c0580e0u,4);goto L8c058066; /* mov.l 0x8c0580e0,r3 */
L8c058066: *stop=0x8c058066u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c058068; /* fsub fr4,fr3 */
L8c058068: *stop=0x8c058068u;fcopy(f,2u,4u);goto L8c05806a; /* fmov fr4,fr2 */
L8c05806a: *stop=0x8c05806au;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[6]);r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c05806c; /* fmov.s @r6+,fr5 */
L8c05806c: *stop=0x8c05806cu;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[6]);goto L8c05806e; /* fmov.s @r6,fr6 */
L8c05806e: *stop=0x8c05806eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c058070; /* fmac fr0,fr3,fr2 */
L8c058070: *stop=0x8c058070u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c058072; /* fmov.s @r4+,fr3 */
L8c058072: *stop=0x8c058072u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c058074; /* fsub fr5,fr3 */
L8c058074: *stop=0x8c058074u;fcopy(f,4u,2u);goto L8c058076; /* fmov fr2,fr4 */
L8c058076: *stop=0x8c058076u;fcopy(f,2u,5u);goto L8c058078; /* fmov fr5,fr2 */
L8c058078: *stop=0x8c058078u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c05807a; /* fmac fr0,fr3,fr2 */
L8c05807a: *stop=0x8c05807au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto L8c05807c; /* fmov.s @r4,fr3 */
L8c05807c: *stop=0x8c05807cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c05807e; /* fsub fr6,fr3 */
L8c05807e: *stop=0x8c05807eu;fcopy(f,5u,2u);goto L8c058080; /* fmov fr2,fr5 */
L8c058080: *stop=0x8c058080u;fcopy(f,2u,6u);goto L8c058082; /* fmov fr6,fr2 */
L8c058082: *stop=0x8c058082u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c058084; /* fmac fr0,fr3,fr2 */
L8c058084: *stop=0x8c058084u;fcopy(f,6u,2u);goto L8c058086; /* fmov fr2,fr6 */
L8c058086: target=r[3];*pr=0x8c05808au;*stop=0x8c058088u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05808a; /* jsr @r3 */
L8c05808a: *stop=0x8c05808au;r[9]+=1u;goto L8c05808c; /* add #0x1,r9 */
L8c05808c: *stop=0x8c05808cu;r[3]=3u;goto L8c05808e; /* mov #0x3,r3 */
L8c05808e: *stop=0x8c05808eu;*sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[3]);goto L8c058090; /* cmp/ge r3,r9 */
L8c058090: if((*sr&1u))goto L8c058096;goto L8c058092; /* bt 0x8c058096 */
L8c058092: *stop=0x8c058094u;goto L8c057e22; /* bra 0x8c057e22 */
L8c058096: *stop=0x8c058096u;r[8]+=24u;goto L8c058098; /* add #0x18,r8 */
L8c058098: *stop=0x8c05809au;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[8],4);goto L8c0580a0; /* bra 0x8c0580a0 */
L8c05809c: *stop=0x8c05809eu;SA_HIERARCHY_RETAIL_STALL(stop); /* bra 0x8c05809c */
L8c0580a0: *stop=0x8c0580a0u;r[2]=rd(m,0x8c0580e4u,4);goto L8c0580a2; /* mov.l 0x8c0580e4,r2 */
L8c0580a2: target=r[2];*pr=0x8c0580a6u;*stop=0x8c0580a4u;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0580a6; /* jsr @r2 */
L8c0580a6: *stop=0x8c0580a6u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c0580a8; /* mov.l @(0x4,r14),r3 */
L8c0580a8: *stop=0x8c0580a8u;r[3]+=64u;goto L8c0580aa; /* add #0x40,r3 */
L8c0580aa: *stop=0x8c0580aau;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[3],4);goto L8c0580ac; /* mov.l r3,@(0x4,r14) */
L8c0580ac: *stop=0x8c0580acu;if(!valid_ram(r[13]+44u,4u))return -3;r[12]=rd(m,r[13]+0x2cu,4);goto L8c0580ae; /* mov.l @(0x2c,r13),r12 */
L8c0580ae: *stop=0x8c0580aeu;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c0580b0; /* tst r12,r12 */
L8c0580b0: if((*sr&1u))goto L8c0580be;goto L8c0580b2; /* bt 0x8c0580be */
L8c0580b2: *stop=0x8c0580b2u;r[5]=r[14];goto L8c0580b4; /* mov r14,r5 */
L8c0580b4: target=0x8c057bc0u;*pr=0x8c0580b8u;*stop=0x8c0580b6u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0580b8; /* bsr 0x8c057bc0 */
L8c0580b8: *stop=0x8c0580b8u;r[12]=r[0];goto L8c0580ba; /* mov r0,r12 */
L8c0580ba: *stop=0x8c0580bau;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c0580bc; /* tst r12,r12 */
L8c0580bc: if(!(*sr&1u))goto L8c0580b2;goto L8c0580be; /* bf 0x8c0580b2 */
L8c0580be: *stop=0x8c0580beu;r[3]=rd(m,0x8c0580e8u,4);goto L8c0580c0; /* mov.l 0x8c0580e8,r3 */
L8c0580c0: target=r[3];*pr=0x8c0580c4u;*stop=0x8c0580c2u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0580c4; /* jsr @r3 */
L8c0580c4: *stop=0x8c0580c4u;if(!valid_ram(r[13]+48u,4u))return -3;r[0]=rd(m,r[13]+0x30u,4);goto L8c0580c6; /* mov.l @(0x30,r13),r0 */
L8c0580c6: *stop=0x8c0580c6u;r[15]+=24u;goto L8c0580c8; /* add #0x18,r15 */
L8c0580c8: *stop=0x8c0580c8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c0580ca; /* lds.l @r15+,PR */
L8c0580ca: *stop=0x8c0580cau;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0580cc; /* fmov.s @r15+,fr14 */
L8c0580cc: *stop=0x8c0580ccu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0580ce; /* fmov.s @r15+,fr15 */
L8c0580ce: *stop=0x8c0580ceu;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c0580d0; /* mov.l @r15+,r8 */
L8c0580d0: *stop=0x8c0580d0u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c0580d2; /* mov.l @r15+,r9 */
L8c0580d2: *stop=0x8c0580d2u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c0580d4; /* mov.l @r15+,r10 */
L8c0580d4: *stop=0x8c0580d4u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c0580d6; /* mov.l @r15+,r11 */
L8c0580d6: *stop=0x8c0580d6u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0580d8; /* mov.l @r15+,r12 */
L8c0580d8: *stop=0x8c0580d8u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0580da; /* mov.l @r15+,r13 */
L8c0580da: *stop=0x8c0580dau;if(*pr!=entry_pr)return -3;*stop=0x8c0580dcu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_draw_hierarchy_copy_8c63a770(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63a770;
L8c63a770: *stop=0x8c63a770u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c63a772; /* tst r4,r4 */
L8c63a772: if(!(*sr&1u))goto L8c63a79a;goto L8c63a774; /* bf 0x8c63a79a */
L8c63a774: *stop=0x8c63a774u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c63a776; /* frchg */
L8c63a776: *stop=0x8c63a776u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a778; /* fmov.s @r5+,fr0 */
L8c63a778: *stop=0x8c63a778u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a77a; /* fmov.s @r5+,fr1 */
L8c63a77a: *stop=0x8c63a77au;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a77c; /* fmov.s @r5+,fr2 */
L8c63a77c: *stop=0x8c63a77cu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a77e; /* fmov.s @r5+,fr3 */
L8c63a77e: *stop=0x8c63a77eu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a780; /* fmov.s @r5+,fr4 */
L8c63a780: *stop=0x8c63a780u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a782; /* fmov.s @r5+,fr5 */
L8c63a782: *stop=0x8c63a782u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a784; /* fmov.s @r5+,fr6 */
L8c63a784: *stop=0x8c63a784u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a786; /* fmov.s @r5+,fr7 */
L8c63a786: *stop=0x8c63a786u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a788; /* fmov.s @r5+,fr8 */
L8c63a788: *stop=0x8c63a788u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a78a; /* fmov.s @r5+,fr9 */
L8c63a78a: *stop=0x8c63a78au;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,10u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a78c; /* fmov.s @r5+,fr10 */
L8c63a78c: *stop=0x8c63a78cu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,11u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a78e; /* fmov.s @r5+,fr11 */
L8c63a78e: *stop=0x8c63a78eu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a790; /* fmov.s @r5+,fr12 */
L8c63a790: *stop=0x8c63a790u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a792; /* fmov.s @r5+,fr13 */
L8c63a792: *stop=0x8c63a792u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a794; /* fmov.s @r5+,fr14 */
L8c63a794: *stop=0x8c63a794u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a796; /* fmov.s @r5+,fr15 */
L8c63a796: *stop=0x8c63a796u;if(*pr!=entry_pr)return -3;*stop=0x8c63a798u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;return 0; /* rts */
L8c63a79a: *stop=0x8c63a79au;r[4]+=32u;goto L8c63a79c; /* add #0x20,r4 */
L8c63a79c: *stop=0x8c63a79cu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a79e; /* fmov.s @r5+,fr0 */
L8c63a79e: *stop=0x8c63a79eu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7a0; /* fmov.s @r5+,fr1 */
L8c63a7a0: *stop=0x8c63a7a0u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7a2; /* fmov.s @r5+,fr2 */
L8c63a7a2: *stop=0x8c63a7a2u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7a4; /* fmov.s @r5+,fr3 */
L8c63a7a4: *stop=0x8c63a7a4u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7a6; /* fmov.s @r5+,fr4 */
L8c63a7a6: *stop=0x8c63a7a6u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7a8; /* fmov.s @r5+,fr5 */
L8c63a7a8: *stop=0x8c63a7a8u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7aa; /* fmov.s @r5+,fr6 */
L8c63a7aa: *stop=0x8c63a7aau;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7ac; /* fmov.s @r5+,fr7 */
L8c63a7ac: *stop=0x8c63a7acu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[4]);goto L8c63a7ae; /* fmov.s fr7,@-r4 */
L8c63a7ae: *stop=0x8c63a7aeu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[4]);goto L8c63a7b0; /* fmov.s fr6,@-r4 */
L8c63a7b0: *stop=0x8c63a7b0u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[4]);goto L8c63a7b2; /* fmov.s fr5,@-r4 */
L8c63a7b2: *stop=0x8c63a7b2u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[4]);goto L8c63a7b4; /* fmov.s fr4,@-r4 */
L8c63a7b4: *stop=0x8c63a7b4u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[4]);goto L8c63a7b6; /* fmov.s fr3,@-r4 */
L8c63a7b6: *stop=0x8c63a7b6u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[4]);goto L8c63a7b8; /* fmov.s fr2,@-r4 */
L8c63a7b8: *stop=0x8c63a7b8u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[4]);goto L8c63a7ba; /* fmov.s fr1,@-r4 */
L8c63a7ba: *stop=0x8c63a7bau;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[4]);goto L8c63a7bc; /* fmov.s fr0,@-r4 */
L8c63a7bc: *stop=0x8c63a7bcu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7be; /* fmov.s @r5+,fr0 */
L8c63a7be: *stop=0x8c63a7beu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7c0; /* fmov.s @r5+,fr1 */
L8c63a7c0: *stop=0x8c63a7c0u;r[4]+=64u;goto L8c63a7c2; /* add #0x40,r4 */
L8c63a7c2: *stop=0x8c63a7c2u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7c4; /* fmov.s @r5+,fr2 */
L8c63a7c4: *stop=0x8c63a7c4u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7c6; /* fmov.s @r5+,fr3 */
L8c63a7c6: *stop=0x8c63a7c6u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7c8; /* fmov.s @r5+,fr4 */
L8c63a7c8: *stop=0x8c63a7c8u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7ca; /* fmov.s @r5+,fr5 */
L8c63a7ca: *stop=0x8c63a7cau;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7cc; /* fmov.s @r5+,fr6 */
L8c63a7cc: *stop=0x8c63a7ccu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a7ce; /* fmov.s @r5+,fr7 */
L8c63a7ce: *stop=0x8c63a7ceu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[4]);goto L8c63a7d0; /* fmov.s fr7,@-r4 */
L8c63a7d0: *stop=0x8c63a7d0u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[4]);goto L8c63a7d2; /* fmov.s fr6,@-r4 */
L8c63a7d2: *stop=0x8c63a7d2u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[4]);goto L8c63a7d4; /* fmov.s fr5,@-r4 */
L8c63a7d4: *stop=0x8c63a7d4u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[4]);goto L8c63a7d6; /* fmov.s fr4,@-r4 */
L8c63a7d6: *stop=0x8c63a7d6u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[4]);goto L8c63a7d8; /* fmov.s fr3,@-r4 */
L8c63a7d8: *stop=0x8c63a7d8u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[4]);goto L8c63a7da; /* fmov.s fr2,@-r4 */
L8c63a7da: *stop=0x8c63a7dau;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[4]);goto L8c63a7dc; /* fmov.s fr1,@-r4 */
L8c63a7dc: *stop=0x8c63a7dcu;if(*pr!=entry_pr)return -3;*stop=0x8c63a7deu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_draw_hierarchy_get_8c058434(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c058434;
L8c058434: *stop=0x8c058434u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c058436; /* sts.l PR,@-r15 */
L8c058436: *stop=0x8c058436u;r[15]+=4294967288u;goto L8c058438; /* add #-0x8,r15 */
L8c058438: *stop=0x8c058438u;r[1]=rd(m,0x8c058480u,4);goto L8c05843a; /* mov.l 0x8c058480,r1 */
L8c05843a: *stop=0x8c05843au;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[4],4);goto L8c05843c; /* mov.l r4,@r15 */
L8c05843c: *stop=0x8c05843cu;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[5],4);goto L8c05843e; /* mov.l r5,@(0x4,r15) */
L8c05843e: *stop=0x8c05843eu;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c058440; /* mov.l @r1,r3 */
L8c058440: *stop=0x8c058440u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c058442; /* mov.l @r15,r2 */
L8c058442: *stop=0x8c058442u;*sr=(*sr&~1u)|(r[2]>=r[3]);goto L8c058444; /* cmp/hs r3,r2 */
L8c058444: if(!(*sr&1u))goto L8c05844e;goto L8c058446; /* bf 0x8c05844e */
L8c058446: *stop=0x8c058446u;r[15]+=8u;goto L8c058448; /* add #0x8,r15 */
L8c058448: *stop=0x8c058448u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c05844a; /* lds.l @r15+,PR */
L8c05844a: *stop=0x8c05844au;if(*pr!=entry_pr)return -3;*stop=0x8c05844cu;r[0]=0u;return 0; /* rts */
L8c05844e: *stop=0x8c05844eu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c058450; /* mov.l @r15,r5 */
L8c058450: *stop=0x8c058450u;r[3]=rd(m,0x8c058490u,4);goto L8c058452; /* mov.l 0x8c058490,r3 */
L8c058452: *stop=0x8c058452u;r[5]<<=2;goto L8c058454; /* shll2 r5 */
L8c058454: *stop=0x8c058454u;r[2]=rd(m,0x8c05848cu,4);goto L8c058456; /* mov.l 0x8c05848c,r2 */
L8c058456: *stop=0x8c058456u;r[5]<<=2;goto L8c058458; /* shll2 r5 */
L8c058458: *stop=0x8c058458u;r[5]<<=2;goto L8c05845a; /* shll2 r5 */
L8c05845a: *stop=0x8c05845au;r[5]+=r[3];goto L8c05845c; /* add r3,r5 */
L8c05845c: target=r[2];*pr=0x8c058460u;*stop=0x8c05845eu;if(!valid_ram(r[15]+4u,4u))return -3;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c058460; /* jsr @r2 */
L8c058460: *stop=0x8c058460u;r[0]=1u;goto L8c058462; /* mov #0x1,r0 */
L8c058462: *stop=0x8c058462u;r[15]+=8u;goto L8c058464; /* add #0x8,r15 */
L8c058464: *stop=0x8c058464u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c058466; /* lds.l @r15+,PR */
L8c058466: *stop=0x8c058466u;if(*pr!=entry_pr)return -3;*stop=0x8c058468u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_draw_hierarchy_scale_8c63a5ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c63a5ec;
L8c63a5ec: *stop=0x8c63a5ecu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c63a5ee; /* tst r4,r4 */
L8c63a5ee: *stop=0x8c63a5eeu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a5f0; /* fmov.s @r5+,fr4 */
L8c63a5f0: *stop=0x8c63a5f0u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a5f2; /* fmov.s @r5+,fr5 */
L8c63a5f2: take=!(*sr&1u);*stop=0x8c63a5f4u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;if(take)goto L8c63a638;goto L8c63a5f6; /* bf/s 0x8c63a638 */
L8c63a5f6: *stop=0x8c63a5f6u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a5f8; /* fschg */
L8c63a5f8: *stop=0x8c63a5f8u;fcopy(f,0u,1u);goto L8c63a5fa; /* fmov fr1,fr0 */
L8c63a5fa: *stop=0x8c63a5fau;fcopy(f,2u,3u);goto L8c63a5fc; /* fmov fr3,fr2 */
L8c63a5fc: *stop=0x8c63a5fcu;fcopy(f,8u,5u);goto L8c63a5fe; /* fmov fr5,fr8 */
L8c63a5fe: *stop=0x8c63a5feu;fcopy(f,10u,7u);goto L8c63a600; /* fmov fr7,fr10 */
L8c63a600: *stop=0x8c63a600u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a602; /* fschg */
L8c63a602: *stop=0x8c63a602u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c63a604; /* fmul fr4,fr0 */
L8c63a604: *stop=0x8c63a604u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a606; /* fmul fr4,fr1 */
L8c63a606: *stop=0x8c63a606u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a608; /* fmul fr4,fr2 */
L8c63a608: *stop=0x8c63a608u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a60a; /* fmul fr4,fr3 */
L8c63a60a: *stop=0x8c63a60au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c63a60c; /* fmul fr5,fr8 */
L8c63a60c: *stop=0x8c63a60cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[5],&f->fpscr,&fr[9]);if(status)return status;}goto L8c63a60e; /* fmul fr5,fr9 */
L8c63a60e: *stop=0x8c63a60eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[5],&f->fpscr,&fr[10]);if(status)return status;}goto L8c63a610; /* fmul fr5,fr10 */
L8c63a610: *stop=0x8c63a610u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c63a612; /* fmul fr5,fr11 */
L8c63a612: *stop=0x8c63a612u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a614; /* fschg */
L8c63a614: *stop=0x8c63a614u;fcopy(f,1u,0u);goto L8c63a616; /* fmov fr0,fr1 */
L8c63a616: *stop=0x8c63a616u;fcopy(f,3u,2u);goto L8c63a618; /* fmov fr2,fr3 */
L8c63a618: *stop=0x8c63a618u;fcopy(f,5u,8u);goto L8c63a61a; /* fmov fr8,fr5 */
L8c63a61a: *stop=0x8c63a61au;fcopy(f,7u,10u);goto L8c63a61c; /* fmov fr10,fr7 */
L8c63a61c: *stop=0x8c63a61cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a61e; /* fschg */
L8c63a61e: *stop=0x8c63a61eu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a620; /* fschg */
L8c63a620: *stop=0x8c63a620u;fcopy(f,0u,9u);goto L8c63a622; /* fmov fr9,fr0 */
L8c63a622: *stop=0x8c63a622u;fcopy(f,2u,11u);goto L8c63a624; /* fmov fr11,fr2 */
L8c63a624: *stop=0x8c63a624u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a626; /* fschg */
L8c63a626: *stop=0x8c63a626u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c63a628; /* fmul fr6,fr0 */
L8c63a628: *stop=0x8c63a628u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a62a; /* fmul fr6,fr1 */
L8c63a62a: *stop=0x8c63a62au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a62c; /* fmul fr6,fr2 */
L8c63a62c: *stop=0x8c63a62cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a62e; /* fmul fr6,fr3 */
L8c63a62e: *stop=0x8c63a62eu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a630; /* fschg */
L8c63a630: *stop=0x8c63a630u;fcopy(f,9u,0u);goto L8c63a632; /* fmov fr0,fr9 */
L8c63a632: *stop=0x8c63a632u;fcopy(f,11u,2u);goto L8c63a634; /* fmov fr2,fr11 */
L8c63a634: *stop=0x8c63a634u;if(*pr!=entry_pr)return -3;*stop=0x8c63a636u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;return 0; /* rts */
L8c63a638: *stop=0x8c63a638u;r[5]=r[4];goto L8c63a63a; /* mov r4,r5 */
L8c63a63a: *stop=0x8c63a63au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a63c; /* fmov.s @r4+,fr3 */
L8c63a63c: *stop=0x8c63a63cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a63e; /* fmul fr4,fr3 */
L8c63a63e: *stop=0x8c63a63eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a640; /* fmov.s @r4+,fr2 */
L8c63a640: *stop=0x8c63a640u;r[0]=4u;goto L8c63a642; /* mov #0x4,r0 */
L8c63a642: *stop=0x8c63a642u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a644; /* fmul fr4,fr2 */
L8c63a644: *stop=0x8c63a644u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[5]);goto L8c63a646; /* fmov.s fr3,@r5 */
L8c63a646: *stop=0x8c63a646u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a648; /* fmov.s @r4+,fr1 */
L8c63a648: *stop=0x8c63a648u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[5]);goto L8c63a64a; /* fmov fr2,@(r0,r5) */
L8c63a64a: *stop=0x8c63a64au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a64c; /* fmul fr4,fr1 */
L8c63a64c: *stop=0x8c63a64cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a64e; /* fmov.s @r4+,fr3 */
L8c63a64e: *stop=0x8c63a64eu;r[0]=8u;goto L8c63a650; /* mov #0x8,r0 */
L8c63a650: *stop=0x8c63a650u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a652; /* fmul fr4,fr3 */
L8c63a652: *stop=0x8c63a652u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[5]);goto L8c63a654; /* fmov fr1,@(r0,r5) */
L8c63a654: *stop=0x8c63a654u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a656; /* fmov.s @r4+,fr2 */
L8c63a656: *stop=0x8c63a656u;r[0]=12u;goto L8c63a658; /* mov #0xc,r0 */
L8c63a658: *stop=0x8c63a658u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a65a; /* fmul fr5,fr2 */
L8c63a65a: *stop=0x8c63a65au;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto L8c63a65c; /* fmov fr3,@(r0,r5) */
L8c63a65c: *stop=0x8c63a65cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a65e; /* fmov.s @r4+,fr1 */
L8c63a65e: *stop=0x8c63a65eu;r[0]=16u;goto L8c63a660; /* mov #0x10,r0 */
L8c63a660: *stop=0x8c63a660u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a662; /* fmul fr5,fr1 */
L8c63a662: *stop=0x8c63a662u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[5]);goto L8c63a664; /* fmov fr2,@(r0,r5) */
L8c63a664: *stop=0x8c63a664u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a666; /* fmov.s @r4+,fr3 */
L8c63a666: *stop=0x8c63a666u;r[0]=20u;goto L8c63a668; /* mov #0x14,r0 */
L8c63a668: *stop=0x8c63a668u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a66a; /* fmul fr5,fr3 */
L8c63a66a: *stop=0x8c63a66au;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[5]);goto L8c63a66c; /* fmov fr1,@(r0,r5) */
L8c63a66c: *stop=0x8c63a66cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a66e; /* fmov.s @r4+,fr2 */
L8c63a66e: *stop=0x8c63a66eu;r[0]=24u;goto L8c63a670; /* mov #0x18,r0 */
L8c63a670: *stop=0x8c63a670u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a672; /* fmul fr5,fr2 */
L8c63a672: *stop=0x8c63a672u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto L8c63a674; /* fmov fr3,@(r0,r5) */
L8c63a674: *stop=0x8c63a674u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a676; /* fmov.s @r4+,fr1 */
L8c63a676: *stop=0x8c63a676u;r[0]=28u;goto L8c63a678; /* mov #0x1c,r0 */
L8c63a678: *stop=0x8c63a678u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a67a; /* fmul fr6,fr1 */
L8c63a67a: *stop=0x8c63a67au;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[5]);goto L8c63a67c; /* fmov fr2,@(r0,r5) */
L8c63a67c: *stop=0x8c63a67cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a67e; /* fmov.s @r4+,fr3 */
L8c63a67e: *stop=0x8c63a67eu;r[0]=32u;goto L8c63a680; /* mov #0x20,r0 */
L8c63a680: *stop=0x8c63a680u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a682; /* fmul fr6,fr3 */
L8c63a682: *stop=0x8c63a682u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[5]);goto L8c63a684; /* fmov fr1,@(r0,r5) */
L8c63a684: *stop=0x8c63a684u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a686; /* fmov.s @r4+,fr2 */
L8c63a686: *stop=0x8c63a686u;r[0]=36u;goto L8c63a688; /* mov #0x24,r0 */
L8c63a688: *stop=0x8c63a688u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a68a; /* fmul fr6,fr2 */
L8c63a68a: *stop=0x8c63a68au;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto L8c63a68c; /* fmov fr3,@(r0,r5) */
L8c63a68c: *stop=0x8c63a68cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[4]);goto L8c63a68e; /* fmov.s @r4,fr1 */
L8c63a68e: *stop=0x8c63a68eu;r[0]=40u;goto L8c63a690; /* mov #0x28,r0 */
L8c63a690: *stop=0x8c63a690u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a692; /* fmul fr6,fr1 */
L8c63a692: *stop=0x8c63a692u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[5]);goto L8c63a694; /* fmov fr2,@(r0,r5) */
L8c63a694: *stop=0x8c63a694u;if(*pr!=entry_pr)return -3;*stop=0x8c63a696u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_draw_hierarchy_bank_store_8c638f94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c638f94;
L8c638f94: *stop=0x8c638f94u;r[4]+=64u;goto L8c638f96; /* add #0x40,r4 */
L8c638f96: *stop=0x8c638f96u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c638f98; /* frchg */
L8c638f98: *stop=0x8c638f98u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[4]);goto L8c638f9a; /* fmov.s fr15,@-r4 */
L8c638f9a: *stop=0x8c638f9au;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[4]);goto L8c638f9c; /* fmov.s fr14,@-r4 */
L8c638f9c: *stop=0x8c638f9cu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[4]);goto L8c638f9e; /* fmov.s fr13,@-r4 */
L8c638f9e: *stop=0x8c638f9eu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[4]);goto L8c638fa0; /* fmov.s fr12,@-r4 */
L8c638fa0: *stop=0x8c638fa0u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,11u,r[4]);goto L8c638fa2; /* fmov.s fr11,@-r4 */
L8c638fa2: *stop=0x8c638fa2u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,10u,r[4]);goto L8c638fa4; /* fmov.s fr10,@-r4 */
L8c638fa4: *stop=0x8c638fa4u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,9u,r[4]);goto L8c638fa6; /* fmov.s fr9,@-r4 */
L8c638fa6: *stop=0x8c638fa6u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[4]);goto L8c638fa8; /* fmov.s fr8,@-r4 */
L8c638fa8: *stop=0x8c638fa8u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[4]);goto L8c638faa; /* fmov.s fr7,@-r4 */
L8c638faa: *stop=0x8c638faau;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[4]);goto L8c638fac; /* fmov.s fr6,@-r4 */
L8c638fac: *stop=0x8c638facu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[4]);goto L8c638fae; /* fmov.s fr5,@-r4 */
L8c638fae: *stop=0x8c638faeu;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[4]);goto L8c638fb0; /* fmov.s fr4,@-r4 */
L8c638fb0: *stop=0x8c638fb0u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[4]);goto L8c638fb2; /* fmov.s fr3,@-r4 */
L8c638fb2: *stop=0x8c638fb2u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[4]);goto L8c638fb4; /* fmov.s fr2,@-r4 */
L8c638fb4: *stop=0x8c638fb4u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[4]);goto L8c638fb6; /* fmov.s fr1,@-r4 */
L8c638fb6: *stop=0x8c638fb6u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[4]);goto L8c638fb8; /* fmov.s fr0,@-r4 */
L8c638fb8: *stop=0x8c638fb8u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c638fba; /* frchg */
L8c638fba: *stop=0x8c638fbau;goto L8c638fbc; /* nop */
L8c638fbc: *stop=0x8c638fbcu;if(*pr!=entry_pr)return -3;*stop=0x8c638fbeu;return 0; /* rts */
}
