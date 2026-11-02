/* Complete register-input matrix scale and translation; names inferred.
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
SA_STAGE_EXPORT int sa_camera_position_tail_8c01b3e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01b3e0;
L8c01b3e0: *stop=0x8c01b3e0u;r[5]=rd(m,0x8c01b4e0u,4);goto L8c01b3e2; /* mov.l 0x8c01b4e0,r5 */
L8c01b3e2: *stop=0x8c01b3e2u;r[0]=r[4];goto L8c01b3e4; /* mov r4,r0 */
L8c01b3e4: *stop=0x8c01b3e4u;r[1]=rd(m,0x8c01b4e4u,4);goto L8c01b3e6; /* mov.l 0x8c01b4e4,r1 */
L8c01b3e6: *stop=0x8c01b3e6u;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c01b3e8; /* mov.l @r5,r3 */
L8c01b3e8: *stop=0x8c01b3e8u;if(!valid_ram(r[3]+9u,1u))return -3;wr(m,r[3]+0x9u,r[0],1);goto L8c01b3ea; /* mov.b r0,@(0x9,r3) */
L8c01b3ea: *stop=0x8c01b3eau;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c01b3ec; /* mov.l @r5,r3 */
L8c01b3ec: *stop=0x8c01b3ecu;r[2]=r[3];goto L8c01b3ee; /* mov r3,r2 */
L8c01b3ee: *stop=0x8c01b3eeu;if(!valid_ram(r[2]+9u,1u))return -3;r[0]=sx(m,r[2]+0x9u,1);goto L8c01b3f0; /* mov.b @(0x9,r2),r0 */
L8c01b3f0: *stop=0x8c01b3f0u;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c01b3f2; /* mov.l @r1,r2 */
L8c01b3f2: *stop=0x8c01b3f2u;r[0]<<=2;goto L8c01b3f4; /* shll2 r0 */
L8c01b3f4: *stop=0x8c01b3f4u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c01b3f6; /* shll r0 */
L8c01b3f6: *stop=0x8c01b3f6u;r[0]+=r[2];goto L8c01b3f8; /* add r2,r0 */
L8c01b3f8: *stop=0x8c01b3f8u;if(!valid_ram(r[0]+4u,4u))return -3;r[2]=rd(m,r[0]+0x4u,4);goto L8c01b3fa; /* mov.l @(0x4,r0),r2 */
L8c01b3fa: *stop=0x8c01b3fau;r[0]=rd(m,0x8c01b4e8u,4);goto L8c01b3fc; /* mov.l 0x8c01b4e8,r0 */
L8c01b3fc: *stop=0x8c01b3fcu;if(!valid_ram(r[3]+16u,4u))return -3;wr(m,r[3]+0x10u,r[2],4);goto L8c01b3fe; /* mov.l r2,@(0x10,r3) */
L8c01b3fe: *stop=0x8c01b3feu;r[2]=0u;goto L8c01b400; /* mov #0x0,r2 */
L8c01b400: *stop=0x8c01b400u;if(!valid_ram(r[0],4u))return -3;r[3]=rd(m,r[0],4);goto L8c01b402; /* mov.l @r0,r3 */
L8c01b402: *stop=0x8c01b402u;if(*pr!=entry_pr)return -3;*stop=0x8c01b404u;if(!valid_ram(r[3]+20u,4u))return -3;wr(m,r[3]+0x14u,r[2],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_position_tail_8c01b158(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01b158;
L8c01b158: *stop=0x8c01b158u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01b15a; /* mov.l r14,@-r15 */
L8c01b15a: *stop=0x8c01b15au;r[14]=rd(m,0x8c01b200u,4);goto L8c01b15c; /* mov.l 0x8c01b200,r14 */
L8c01b15c: *stop=0x8c01b15cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01b15e; /* sts.l PR,@-r15 */
L8c01b15e: *stop=0x8c01b15eu;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);goto L8c01b160; /* mov.l @r14,r4 */
L8c01b160: *stop=0x8c01b160u;if(!valid_ram(r[4]+8u,1u))return -3;r[0]=sx(m,r[4]+0x8u,1);goto L8c01b162; /* mov.b @(0x8,r4),r0 */
L8c01b162: *stop=0x8c01b162u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c01b164; /* cmp/eq #0x4,r0 */
L8c01b164: if(!(*sr&1u))goto L8c01b1f4;goto L8c01b166; /* bf 0x8c01b1f4 */
L8c01b166: *stop=0x8c01b166u;r[0]=56u;goto L8c01b168; /* mov #0x38,r0 */
L8c01b168: *stop=0x8c01b168u;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto L8c01b16a; /* mov.w @(r0,r4),r0 */
L8c01b16a: *stop=0x8c01b16au;r[1]=64u;goto L8c01b16c; /* mov #0x40,r1 */
L8c01b16c: *stop=0x8c01b16cu;r[2]=120u;goto L8c01b16e; /* mov #0x78,r2 */
L8c01b16e: *stop=0x8c01b16eu;if(!valid_ram(r[4]+4u,2u))return -3;wr(m,r[4]+0x4u,r[0],2);goto L8c01b170; /* mov.w r0,@(0x4,r4) */
L8c01b170: *stop=0x8c01b170u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b172; /* mov.l @r14,r3 */
L8c01b172: *stop=0x8c01b172u;r[0]=r[3];goto L8c01b174; /* mov r3,r0 */
L8c01b174: *stop=0x8c01b174u;if(!valid_ram(r[0]+r[1],1u))return -3;r[0]=sx(m,r[1]+r[0],1);goto L8c01b176; /* mov.b @(r0,r1),r0 */
L8c01b176: *stop=0x8c01b176u;r[1]=84u;goto L8c01b178; /* mov #0x54,r1 */
L8c01b178: *stop=0x8c01b178u;if(!valid_ram(r[3]+11u,1u))return -3;wr(m,r[3]+0xbu,r[0],1);goto L8c01b17a; /* mov.b r0,@(0xb,r3) */
L8c01b17a: *stop=0x8c01b17au;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b17c; /* mov.l @r14,r3 */
L8c01b17c: *stop=0x8c01b17cu;r[0]=r[3];goto L8c01b17e; /* mov r3,r0 */
L8c01b17e: *stop=0x8c01b17eu;if(!valid_ram(r[0]+r[2],1u))return -3;r[0]=sx(m,r[2]+r[0],1);goto L8c01b180; /* mov.b @(r0,r2),r0 */
L8c01b180: *stop=0x8c01b180u;r[2]=126u;goto L8c01b182; /* mov #0x7e,r2 */
L8c01b182: *stop=0x8c01b182u;if(!valid_ram(r[3]+6u,1u))return -3;wr(m,r[3]+0x6u,r[0],1);goto L8c01b184; /* mov.b r0,@(0x6,r3) */
L8c01b184: *stop=0x8c01b184u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b186; /* mov.l @r14,r3 */
L8c01b186: *stop=0x8c01b186u;r[0]=r[3];goto L8c01b188; /* mov r3,r0 */
L8c01b188: *stop=0x8c01b188u;if(!valid_ram(r[0]+r[1],4u))return -3;r[0]=rd(m,r[1]+r[0],4);goto L8c01b18a; /* mov.l @(r0,r1),r0 */
L8c01b18a: *stop=0x8c01b18au;r[1]=108u;goto L8c01b18c; /* mov #0x6c,r1 */
L8c01b18c: *stop=0x8c01b18cu;if(!valid_ram(r[3]+12u,4u))return -3;wr(m,r[3]+0xcu,r[0],4);goto L8c01b18e; /* mov.l r0,@(0xc,r3) */
L8c01b18e: *stop=0x8c01b18eu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b190; /* mov.l @r14,r3 */
L8c01b190: *stop=0x8c01b190u;r[0]=r[3];goto L8c01b192; /* mov r3,r0 */
L8c01b192: *stop=0x8c01b192u;if(!valid_ram(r[0]+r[2],1u))return -3;r[0]=sx(m,r[2]+r[0],1);goto L8c01b194; /* mov.b @(r0,r2),r0 */
L8c01b194: *stop=0x8c01b194u;if(!valid_ram(r[3]+9u,1u))return -3;wr(m,r[3]+0x9u,r[0],1);goto L8c01b196; /* mov.b r0,@(0x9,r3) */
L8c01b196: *stop=0x8c01b196u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b198; /* mov.l @r14,r3 */
L8c01b198: *stop=0x8c01b198u;r[0]=r[3];goto L8c01b19a; /* mov r3,r0 */
L8c01b19a: *stop=0x8c01b19au;if(!valid_ram(r[0]+r[1],4u))return -3;r[0]=rd(m,r[1]+r[0],4);goto L8c01b19c; /* mov.l @(r0,r1),r0 */
L8c01b19c: *stop=0x8c01b19cu;if(!valid_ram(r[3]+16u,4u))return -3;wr(m,r[3]+0x10u,r[0],4);goto L8c01b19e; /* mov.l r0,@(0x10,r3) */
L8c01b19e: *stop=0x8c01b19eu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b1a0; /* mov.l @r14,r3 */
L8c01b1a0: *stop=0x8c01b1a0u;r[0]=r[3];goto L8c01b1a2; /* mov r3,r0 */
L8c01b1a2: *stop=0x8c01b1a2u;if(!valid_ram(r[0]+40u,4u))return -3;r[0]=rd(m,r[0]+0x28u,4);goto L8c01b1a4; /* mov.l @(0x28,r0),r0 */
L8c01b1a4: *stop=0x8c01b1a4u;if(!valid_ram(r[3]+8u,1u))return -3;wr(m,r[3]+0x8u,r[0],1);goto L8c01b1a6; /* mov.b r0,@(0x8,r3) */
L8c01b1a6: *stop=0x8c01b1a6u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b1a8; /* mov.l @r14,r3 */
L8c01b1a8: *stop=0x8c01b1a8u;r[4]=0u;goto L8c01b1aa; /* mov #0x0,r4 */
L8c01b1aa: *stop=0x8c01b1aau;r[1]=rd(m,0x8c01b20cu,4);goto L8c01b1ac; /* mov.l 0x8c01b20c,r1 */
L8c01b1ac: *stop=0x8c01b1acu;r[5]=6u;goto L8c01b1ae; /* mov #0x6,r5 */
L8c01b1ae: *stop=0x8c01b1aeu;r[0]=r[3];goto L8c01b1b0; /* mov r3,r0 */
L8c01b1b0: *stop=0x8c01b1b0u;if(!valid_ram(r[0]+6u,1u))return -3;r[0]=sx(m,r[0]+0x6u,1);goto L8c01b1b2; /* mov.b @(0x6,r0),r0 */
L8c01b1b2: *stop=0x8c01b1b2u;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c01b1b4; /* mov.l @r1,r2 */
L8c01b1b4: *stop=0x8c01b1b4u;r[0]<<=2;goto L8c01b1b6; /* shll2 r0 */
L8c01b1b6: *stop=0x8c01b1b6u;r[6]=rd(m,0x8c01b214u,4);goto L8c01b1b8; /* mov.l 0x8c01b214,r6 */
L8c01b1b8: *stop=0x8c01b1b8u;r[0]<<=2;goto L8c01b1ba; /* shll2 r0 */
L8c01b1ba: *stop=0x8c01b1bau;r[0]+=r[2];goto L8c01b1bc; /* add r2,r0 */
L8c01b1bc: *stop=0x8c01b1bcu;if(!valid_ram(r[0]+5u,1u))return -3;r[0]=sx(m,r[0]+0x5u,1);goto L8c01b1be; /* mov.b @(0x5,r0),r0 */
L8c01b1be: *stop=0x8c01b1beu;r[2]=1u;goto L8c01b1c0; /* mov #0x1,r2 */
L8c01b1c0: *stop=0x8c01b1c0u;if(!valid_ram(r[3]+7u,1u))return -3;wr(m,r[3]+0x7u,r[0],1);goto L8c01b1c2; /* mov.b r0,@(0x7,r3) */
L8c01b1c2: *stop=0x8c01b1c2u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b1c4; /* mov.l @r14,r3 */
L8c01b1c4: *stop=0x8c01b1c4u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto L8c01b1c6; /* mov.l r2,@r3 */
L8c01b1c6: *stop=0x8c01b1c6u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b1c8; /* mov.l @r14,r3 */
L8c01b1c8: *stop=0x8c01b1c8u;if(!valid_ram(r[3]+20u,4u))return -3;wr(m,r[3]+0x14u,r[4],4);goto L8c01b1ca; /* mov.l r4,@(0x14,r3) */
L8c01b1ca: *stop=0x8c01b1cau;r[3]=rd(m,0x8c01b210u,4);goto L8c01b1cc; /* mov.l 0x8c01b210,r3 */
L8c01b1cc: *stop=0x8c01b1ccu;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c01b1ce; /* mov.l @r3,r2 */
L8c01b1ce: *stop=0x8c01b1ceu;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[4],4);goto L8c01b1d0; /* mov.l r4,@(0x14,r2) */
L8c01b1d0: *stop=0x8c01b1d0u;r[2]=rd(m,0x8c01b218u,4);goto L8c01b1d2; /* mov.l 0x8c01b218,r2 */
L8c01b1d2: target=r[2];*pr=0x8c01b1d6u;*stop=0x8c01b1d4u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01b1d6; /* jsr @r2 */
L8c01b1d6: *stop=0x8c01b1d6u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b1d8; /* mov.l @r14,r3 */
L8c01b1d8: *stop=0x8c01b1d8u;if(!valid_ram(r[3]+8u,1u))return -3;r[0]=sx(m,r[3]+0x8u,1);goto L8c01b1da; /* mov.b @(0x8,r3),r0 */
L8c01b1da: *stop=0x8c01b1dau;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c01b1dc; /* tst r0,r0 */
L8c01b1dc: if(!(*sr&1u))goto L8c01b1f4;goto L8c01b1de; /* bf 0x8c01b1f4 */
L8c01b1de: *stop=0x8c01b1deu;r[2]=rd(m,0x8c01b21cu,4);goto L8c01b1e0; /* mov.l 0x8c01b21c,r2 */
L8c01b1e0: *stop=0x8c01b1e0u;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto L8c01b1e2; /* mov.l @r2,r0 */
L8c01b1e2: *stop=0x8c01b1e2u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c01b1e4; /* cmp/eq #-0x1,r0 */
L8c01b1e4: if((*sr&1u))goto L8c01b1f4;goto L8c01b1e6; /* bt 0x8c01b1f4 */
L8c01b1e6: *stop=0x8c01b1e6u;r[3]=rd(m,0x8c01b21cu,4);goto L8c01b1e8; /* mov.l 0x8c01b21c,r3 */
L8c01b1e8: *stop=0x8c01b1e8u;r[1]=rd(m,0x8c01b220u,4);goto L8c01b1ea; /* mov.l 0x8c01b220,r1 */
L8c01b1ea: *stop=0x8c01b1eau;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01b1ec; /* lds.l @r15+,PR */
L8c01b1ec: *stop=0x8c01b1ecu;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);goto L8c01b1ee; /* mov.l @r3,r4 */
L8c01b1ee: *stop=0x8c01b1eeu;if(!valid_ram(r[1],4u))return -3;r[5]=rd(m,r[1],4);goto L8c01b1f0; /* mov.l @r1,r5 */
L8c01b1f0: *stop=0x8c01b1f2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0x8c01aae0u,m,f,sr,r,pr); /* bra 0x8c01aae0 */
L8c01b1f4: *stop=0x8c01b1f4u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01b1f6; /* lds.l @r15+,PR */
L8c01b1f6: *stop=0x8c01b1f6u;if(*pr!=entry_pr)return -3;*stop=0x8c01b1f8u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_position_tail_8c01b040(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01b040;
L8c01b040: *stop=0x8c01b040u;r[6]=rd(m,0x8c01b200u,4);goto L8c01b042; /* mov.l 0x8c01b200,r6 */
L8c01b042: *stop=0x8c01b042u;r[3]=4u;goto L8c01b044; /* mov #0x4,r3 */
L8c01b044: *stop=0x8c01b044u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01b046; /* mov.l r14,@-r15 */
L8c01b046: *stop=0x8c01b046u;if(!valid_ram(r[6],4u))return -3;r[7]=rd(m,r[6],4);goto L8c01b048; /* mov.l @r6,r7 */
L8c01b048: *stop=0x8c01b048u;if(!valid_ram(r[7]+8u,1u))return -3;r[0]=sx(m,r[7]+0x8u,1);goto L8c01b04a; /* mov.b @(0x8,r7),r0 */
L8c01b04a: *stop=0x8c01b04au;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[3]);goto L8c01b04c; /* cmp/gt r3,r0 */
L8c01b04c: if(!(*sr&1u))goto L8c01b052;goto L8c01b04e; /* bf 0x8c01b052 */
L8c01b04e: *stop=0x8c01b050u;goto L8c01b154; /* bra 0x8c01b154 */
L8c01b052: *stop=0x8c01b052u;if(!valid_ram(r[7]+8u,1u))return -3;r[0]=sx(m,r[7]+0x8u,1);goto L8c01b054; /* mov.b @(0x8,r7),r0 */
L8c01b054: *stop=0x8c01b054u;r[14]=rd(m,0x8c01b204u,4);goto L8c01b056; /* mov.l 0x8c01b204,r14 */
L8c01b056: *stop=0x8c01b056u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c01b058; /* cmp/eq #0x4,r0 */
L8c01b058: if((*sr&1u))goto L8c01b0d4;goto L8c01b05a; /* bt 0x8c01b0d4 */
L8c01b05a: *stop=0x8c01b05au;if(!valid_ram(r[7]+8u,1u))return -3;r[0]=sx(m,r[7]+0x8u,1);goto L8c01b05c; /* mov.b @(0x8,r7),r0 */
L8c01b05c: *stop=0x8c01b05cu;if(!valid_ram(r[7]+40u,4u))return -3;wr(m,r[7]+0x28u,r[0],4);goto L8c01b05e; /* mov.l r0,@(0x28,r7) */
L8c01b05e: *stop=0x8c01b05eu;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c01b060; /* mov.l @r6,r2 */
L8c01b060: *stop=0x8c01b060u;r[1]=r[2];goto L8c01b062; /* mov r2,r1 */
L8c01b062: *stop=0x8c01b062u;if(!valid_ram(r[1]+4u,2u))return -3;r[0]=sx(m,r[1]+0x4u,2);goto L8c01b064; /* mov.w @(0x4,r1),r0 */
L8c01b064: *stop=0x8c01b064u;r[1]=56u;goto L8c01b066; /* mov #0x38,r1 */
L8c01b066: *stop=0x8c01b066u;r[1]+=r[2];goto L8c01b068; /* add r2,r1 */
L8c01b068: *stop=0x8c01b068u;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[0],2);goto L8c01b06a; /* mov.w r0,@r1 */
L8c01b06a: *stop=0x8c01b06au;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c01b06c; /* mov.l @r6,r2 */
L8c01b06c: *stop=0x8c01b06cu;if(!valid_ram(r[6],4u))return -3;r[1]=rd(m,r[6],4);goto L8c01b06e; /* mov.l @r6,r1 */
L8c01b06e: *stop=0x8c01b06eu;if(!valid_ram(r[2]+11u,1u))return -3;r[0]=sx(m,r[2]+0xbu,1);goto L8c01b070; /* mov.b @(0xb,r2),r0 */
L8c01b070: *stop=0x8c01b070u;r[1]+=60u;goto L8c01b072; /* add #0x3c,r1 */
L8c01b072: *stop=0x8c01b072u;r[3]+=r[1];goto L8c01b074; /* add r1,r3 */
L8c01b074: *stop=0x8c01b074u;if(!valid_ram(r[3],1u))return -3;wr(m,r[3],r[0],1);goto L8c01b076; /* mov.b r0,@r3 */
L8c01b076: *stop=0x8c01b076u;r[1]=120u;goto L8c01b078; /* mov #0x78,r1 */
L8c01b078: *stop=0x8c01b078u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b07a; /* mov.l @r6,r3 */
L8c01b07a: *stop=0x8c01b07au;r[1]+=r[3];goto L8c01b07c; /* add r3,r1 */
L8c01b07c: *stop=0x8c01b07cu;r[2]=r[3];goto L8c01b07e; /* mov r3,r2 */
L8c01b07e: *stop=0x8c01b07eu;if(!valid_ram(r[2]+6u,1u))return -3;r[0]=sx(m,r[2]+0x6u,1);goto L8c01b080; /* mov.b @(0x6,r2),r0 */
L8c01b080: *stop=0x8c01b080u;if(!valid_ram(r[1],1u))return -3;wr(m,r[1],r[0],1);goto L8c01b082; /* mov.b r0,@r1 */
L8c01b082: *stop=0x8c01b082u;r[0]=84u;goto L8c01b084; /* mov #0x54,r0 */
L8c01b084: *stop=0x8c01b084u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b086; /* mov.l @r6,r3 */
L8c01b086: *stop=0x8c01b086u;r[2]=r[3];goto L8c01b088; /* mov r3,r2 */
L8c01b088: *stop=0x8c01b088u;if(!valid_ram(r[2]+12u,4u))return -3;r[1]=rd(m,r[2]+0xcu,4);goto L8c01b08a; /* mov.l @(0xc,r2),r1 */
L8c01b08a: *stop=0x8c01b08au;if(!valid_ram(r[0]+r[3],4u))return -3;wr(m,r[3]+r[0],r[1],4);goto L8c01b08c; /* mov.l r1,@(r0,r3) */
L8c01b08c: *stop=0x8c01b08cu;if(!valid_ram(r[6],4u))return -3;r[0]=rd(m,r[6],4);goto L8c01b08e; /* mov.l @r6,r0 */
L8c01b08e: *stop=0x8c01b08eu;if(!valid_ram(r[0]+4u,2u))return -3;r[0]=sx(m,r[0]+0x4u,2);goto L8c01b090; /* mov.w @(0x4,r0),r0 */
L8c01b090: *stop=0x8c01b090u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c01b092; /* cmp/eq #-0x1,r0 */
L8c01b092: if((*sr&1u))goto L8c01b0b0;goto L8c01b094; /* bt 0x8c01b0b0 */
L8c01b094: *stop=0x8c01b094u;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c01b096; /* mov.l @r6,r2 */
L8c01b096: *stop=0x8c01b096u;r[1]=rd(m,0x8c01b208u,4);goto L8c01b098; /* mov.l 0x8c01b208,r1 */
L8c01b098: *stop=0x8c01b098u;r[3]=r[2];goto L8c01b09a; /* mov r2,r3 */
L8c01b09a: *stop=0x8c01b09au;if(!valid_ram(r[3]+4u,2u))return -3;r[0]=sx(m,r[3]+0x4u,2);goto L8c01b09c; /* mov.w @(0x4,r3),r0 */
L8c01b09c: *stop=0x8c01b09cu;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c01b09e; /* mov.l @r1,r3 */
L8c01b09e: *stop=0x8c01b09eu;r[0]<<=2;goto L8c01b0a0; /* shll2 r0 */
L8c01b0a0: *stop=0x8c01b0a0u;r[0]<<=2;goto L8c01b0a2; /* shll2 r0 */
L8c01b0a2: *stop=0x8c01b0a2u;r[0]<<=2;goto L8c01b0a4; /* shll2 r0 */
L8c01b0a4: *stop=0x8c01b0a4u;r[0]+=r[3];goto L8c01b0a6; /* add r3,r0 */
L8c01b0a6: *stop=0x8c01b0a6u;r[3]=126u;goto L8c01b0a8; /* mov #0x7e,r3 */
L8c01b0a8: *stop=0x8c01b0a8u;if(!valid_ram(r[0]+2u,1u))return -3;r[0]=sx(m,r[0]+0x2u,1);goto L8c01b0aa; /* mov.b @(0x2,r0),r0 */
L8c01b0aa: *stop=0x8c01b0aau;r[3]+=r[2];goto L8c01b0ac; /* add r2,r3 */
L8c01b0ac: *stop=0x8c01b0aeu;if(!valid_ram(r[3],1u))return -3;wr(m,r[3],r[0],1);goto L8c01b0b8; /* bra 0x8c01b0b8 */
L8c01b0b0: *stop=0x8c01b0b0u;if(!valid_ram(r[6],4u))return -3;r[1]=rd(m,r[6],4);goto L8c01b0b2; /* mov.l @r6,r1 */
L8c01b0b2: *stop=0x8c01b0b2u;r[0]=126u;goto L8c01b0b4; /* mov #0x7e,r0 */
L8c01b0b4: *stop=0x8c01b0b4u;r[3]=21u;goto L8c01b0b6; /* mov #0x15,r3 */
L8c01b0b6: *stop=0x8c01b0b6u;if(!valid_ram(r[0]+r[1],1u))return -3;wr(m,r[1]+r[0],r[3],1);goto L8c01b0b8; /* mov.b r3,@(r0,r1) */
L8c01b0b8: *stop=0x8c01b0b8u;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c01b0ba; /* mov.l @r6,r2 */
L8c01b0ba: *stop=0x8c01b0bau;r[0]=126u;goto L8c01b0bc; /* mov #0x7e,r0 */
L8c01b0bc: *stop=0x8c01b0bcu;r[3]=r[2];goto L8c01b0be; /* mov r2,r3 */
L8c01b0be: *stop=0x8c01b0beu;if(!valid_ram(r[0]+r[3],1u))return -3;r[1]=sx(m,r[3]+r[0],1);goto L8c01b0c0; /* mov.b @(r0,r3),r1 */
L8c01b0c0: *stop=0x8c01b0c0u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b0c2; /* mov.l @r14,r3 */
L8c01b0c2: *stop=0x8c01b0c2u;r[0]=108u;goto L8c01b0c4; /* mov #0x6c,r0 */
L8c01b0c4: *stop=0x8c01b0c4u;r[1]<<=2;goto L8c01b0c6; /* shll2 r1 */
L8c01b0c6: *stop=0x8c01b0c6u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c01b0c8; /* shll r1 */
L8c01b0c8: *stop=0x8c01b0c8u;r[1]+=r[3];goto L8c01b0ca; /* add r3,r1 */
L8c01b0ca: *stop=0x8c01b0cau;if(!valid_ram(r[1]+4u,4u))return -3;r[3]=rd(m,r[1]+0x4u,4);goto L8c01b0cc; /* mov.l @(0x4,r1),r3 */
L8c01b0cc: *stop=0x8c01b0ccu;if(!valid_ram(r[0]+r[2],4u))return -3;wr(m,r[2]+r[0],r[3],4);goto L8c01b0ce; /* mov.l r3,@(r0,r2) */
L8c01b0ce: *stop=0x8c01b0ceu;r[0]=4u;goto L8c01b0d0; /* mov #0x4,r0 */
L8c01b0d0: *stop=0x8c01b0d0u;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c01b0d2; /* mov.l @r6,r2 */
L8c01b0d2: *stop=0x8c01b0d2u;if(!valid_ram(r[2]+8u,1u))return -3;wr(m,r[2]+0x8u,r[0],1);goto L8c01b0d4; /* mov.b r0,@(0x8,r2) */
L8c01b0d4: *stop=0x8c01b0d4u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b0d6; /* mov.l @r6,r3 */
L8c01b0d6: *stop=0x8c01b0d6u;r[0]=4294967295u;goto L8c01b0d8; /* mov #-0x1,r0 */
L8c01b0d8: *stop=0x8c01b0d8u;r[7]=rd(m,0x8c01b20cu,4);goto L8c01b0da; /* mov.l 0x8c01b20c,r7 */
L8c01b0da: *stop=0x8c01b0dau;if(!valid_ram(r[3]+4u,2u))return -3;wr(m,r[3]+0x4u,r[0],2);goto L8c01b0dc; /* mov.w r0,@(0x4,r3) */
L8c01b0dc: *stop=0x8c01b0dcu;r[0]=r[4];goto L8c01b0de; /* mov r4,r0 */
L8c01b0de: *stop=0x8c01b0deu;r[4]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c01b0e0; /* exts.w r4,r4 */
L8c01b0e0: *stop=0x8c01b0e0u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b0e2; /* mov.l @r6,r3 */
L8c01b0e2: *stop=0x8c01b0e2u;r[4]<<=2;goto L8c01b0e4; /* shll2 r4 */
L8c01b0e4: *stop=0x8c01b0e4u;r[4]<<=2;goto L8c01b0e6; /* shll2 r4 */
L8c01b0e6: *stop=0x8c01b0e6u;if(!valid_ram(r[3]+6u,1u))return -3;wr(m,r[3]+0x6u,r[0],1);goto L8c01b0e8; /* mov.b r0,@(0x6,r3) */
L8c01b0e8: *stop=0x8c01b0e8u;if(!valid_ram(r[7],4u))return -3;r[2]=rd(m,r[7],4);goto L8c01b0ea; /* mov.l @r7,r2 */
L8c01b0ea: *stop=0x8c01b0eau;r[0]=r[5];goto L8c01b0ec; /* mov r5,r0 */
L8c01b0ec: *stop=0x8c01b0ecu;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b0ee; /* mov.l @r6,r3 */
L8c01b0ee: *stop=0x8c01b0eeu;r[5]=r[5]&255u;goto L8c01b0f0; /* extu.b r5,r5 */
L8c01b0f0: *stop=0x8c01b0f0u;r[4]+=r[2];goto L8c01b0f2; /* add r2,r4 */
L8c01b0f2: *stop=0x8c01b0f2u;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);goto L8c01b0f4; /* mov.l @r4,r1 */
L8c01b0f4: *stop=0x8c01b0f4u;r[5]<<=2;goto L8c01b0f6; /* shll2 r5 */
L8c01b0f6: *stop=0x8c01b0f6u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c01b0f8; /* shll r5 */
L8c01b0f8: *stop=0x8c01b0f8u;if(!valid_ram(r[3]+12u,4u))return -3;wr(m,r[3]+0xcu,r[1],4);goto L8c01b0fa; /* mov.l r1,@(0xc,r3) */
L8c01b0fa: *stop=0x8c01b0fau;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b0fc; /* mov.l @r6,r3 */
L8c01b0fc: *stop=0x8c01b0fcu;if(!valid_ram(r[3]+9u,1u))return -3;wr(m,r[3]+0x9u,r[0],1);goto L8c01b0fe; /* mov.b r0,@(0x9,r3) */
L8c01b0fe: *stop=0x8c01b0feu;r[0]=2u;goto L8c01b100; /* mov #0x2,r0 */
L8c01b100: *stop=0x8c01b100u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c01b102; /* mov.l @r14,r2 */
L8c01b102: *stop=0x8c01b102u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b104; /* mov.l @r6,r3 */
L8c01b104: *stop=0x8c01b104u;r[5]+=r[2];goto L8c01b106; /* add r2,r5 */
L8c01b106: *stop=0x8c01b106u;r[2]=rd(m,0x8c01b210u,4);goto L8c01b108; /* mov.l 0x8c01b210,r2 */
L8c01b108: *stop=0x8c01b108u;if(!valid_ram(r[5]+4u,4u))return -3;r[1]=rd(m,r[5]+0x4u,4);goto L8c01b10a; /* mov.l @(0x4,r5),r1 */
L8c01b10a: *stop=0x8c01b10au;if(!valid_ram(r[3]+16u,4u))return -3;wr(m,r[3]+0x10u,r[1],4);goto L8c01b10c; /* mov.l r1,@(0x10,r3) */
L8c01b10c: *stop=0x8c01b10cu;r[1]=0u;goto L8c01b10e; /* mov #0x0,r1 */
L8c01b10e: *stop=0x8c01b10eu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c01b110; /* mov.l @r2,r3 */
L8c01b110: *stop=0x8c01b110u;if(!valid_ram(r[3]+20u,4u))return -3;wr(m,r[3]+0x14u,r[1],4);goto L8c01b112; /* mov.l r1,@(0x14,r3) */
L8c01b112: *stop=0x8c01b112u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b114; /* mov.l @r6,r3 */
L8c01b114: *stop=0x8c01b114u;if(!valid_ram(r[3]+11u,1u))return -3;wr(m,r[3]+0xbu,r[0],1);goto L8c01b116; /* mov.b r0,@(0xb,r3) */
L8c01b116: *stop=0x8c01b116u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b118; /* mov.l @r6,r3 */
L8c01b118: *stop=0x8c01b118u;if(!valid_ram(r[3]+9u,1u))return -3;r[0]=sx(m,r[3]+0x9u,1);goto L8c01b11a; /* mov.b @(0x9,r3),r0 */
L8c01b11a: *stop=0x8c01b11au;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01b11c; /* mov.l @r14,r3 */
L8c01b11c: *stop=0x8c01b11cu;r[0]<<=2;goto L8c01b11e; /* shll2 r0 */
L8c01b11e: *stop=0x8c01b11eu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c01b120; /* shll r0 */
L8c01b120: *stop=0x8c01b120u;r[0]+=r[3];goto L8c01b122; /* add r3,r0 */
L8c01b122: *stop=0x8c01b122u;if(!valid_ram(r[0],4u))return -3;r[1]=rd(m,r[0],4);goto L8c01b124; /* mov.l @r0,r1 */
L8c01b124: *stop=0x8c01b124u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c01b126; /* tst r1,r1 */
L8c01b126: if((*sr&1u))goto L8c01b130;goto L8c01b128; /* bt 0x8c01b130 */
L8c01b128: *stop=0x8c01b128u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b12a; /* mov.l @r6,r3 */
L8c01b12a: *stop=0x8c01b12au;if(!valid_ram(r[3]+11u,1u))return -3;r[0]=sx(m,r[3]+0xbu,1);goto L8c01b12c; /* mov.b @(0xb,r3),r0 */
L8c01b12c: *stop=0x8c01b12cu;r[0]|=0x1u;goto L8c01b12e; /* or #0x1,r0 */
L8c01b12e: *stop=0x8c01b12eu;if(!valid_ram(r[3]+11u,1u))return -3;wr(m,r[3]+0xbu,r[0],1);goto L8c01b130; /* mov.b r0,@(0xb,r3) */
L8c01b130: *stop=0x8c01b130u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b132; /* mov.l @r6,r3 */
L8c01b132: *stop=0x8c01b132u;r[4]=8u;goto L8c01b134; /* mov #0x8,r4 */
L8c01b134: *stop=0x8c01b134u;if(!valid_ram(r[7],4u))return -3;r[2]=rd(m,r[7],4);goto L8c01b136; /* mov.l @r7,r2 */
L8c01b136: *stop=0x8c01b136u;r[5]=6u;goto L8c01b138; /* mov #0x6,r5 */
L8c01b138: *stop=0x8c01b138u;r[0]=r[3];goto L8c01b13a; /* mov r3,r0 */
L8c01b13a: *stop=0x8c01b13au;if(!valid_ram(r[0]+6u,1u))return -3;r[0]=sx(m,r[0]+0x6u,1);goto L8c01b13c; /* mov.b @(0x6,r0),r0 */
L8c01b13c: *stop=0x8c01b13cu;r[0]<<=2;goto L8c01b13e; /* shll2 r0 */
L8c01b13e: *stop=0x8c01b13eu;r[0]<<=2;goto L8c01b140; /* shll2 r0 */
L8c01b140: *stop=0x8c01b140u;r[0]+=r[2];goto L8c01b142; /* add r2,r0 */
L8c01b142: *stop=0x8c01b142u;if(!valid_ram(r[0]+5u,1u))return -3;r[0]=sx(m,r[0]+0x5u,1);goto L8c01b144; /* mov.b @(0x5,r0),r0 */
L8c01b144: *stop=0x8c01b144u;r[2]=1u;goto L8c01b146; /* mov #0x1,r2 */
L8c01b146: *stop=0x8c01b146u;if(!valid_ram(r[3]+7u,1u))return -3;wr(m,r[3]+0x7u,r[0],1);goto L8c01b148; /* mov.b r0,@(0x7,r3) */
L8c01b148: *stop=0x8c01b148u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c01b14a; /* mov.l @r6,r3 */
L8c01b14a: *stop=0x8c01b14au;r[6]=rd(m,0x8c01b214u,4);goto L8c01b14c; /* mov.l 0x8c01b214,r6 */
L8c01b14c: *stop=0x8c01b14cu;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto L8c01b14e; /* mov.l r2,@r3 */
L8c01b14e: *stop=0x8c01b14eu;r[3]=rd(m,0x8c01b218u,4);goto L8c01b150; /* mov.l 0x8c01b218,r3 */
L8c01b150: target=r[3];*stop=0x8c01b152u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c01b154: *stop=0x8c01b154u;if(*pr!=entry_pr)return -3;*stop=0x8c01b156u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
