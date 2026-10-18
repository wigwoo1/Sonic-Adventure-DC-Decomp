/* Motion format routing, keyframe samplers and interpolators; names inferred.
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
SA_STAGE_EXPORT int sa_motion_format_one_8c60d054(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60d054;
L8c60d054: *stop=0x8c60d054u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60d056; /* mov.l r14,@-r15 */
L8c60d056: *stop=0x8c60d056u;r[3]=0u;goto L8c60d058; /* mov #0x0,r3 */
L8c60d058: *stop=0x8c60d058u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60d05a; /* mov.l r13,@-r15 */
L8c60d05a: *stop=0x8c60d05au;r[13]=rd(m,0x8c60d13cu,4);goto L8c60d05c; /* mov.l 0x8c60d13c,r13 */
L8c60d05c: *stop=0x8c60d05cu;r[1]=rd(m,0x8c60d130u,4);goto L8c60d05e; /* mov.l 0x8c60d130,r1 */
L8c60d05e: *stop=0x8c60d05eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60d060; /* mov.l r12,@-r15 */
L8c60d060: *stop=0x8c60d060u;r[12]=r[4];goto L8c60d062; /* mov r4,r12 */
L8c60d062: *stop=0x8c60d062u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60d064; /* sts.l PR,@-r15 */
L8c60d064: *stop=0x8c60d064u;if(!valid_ram(r[13],4u))return -3;r[14]=rd(m,r[13],4);goto L8c60d066; /* mov.l @r13,r14 */
L8c60d066: *stop=0x8c60d066u;r[2]=r[14];goto L8c60d068; /* mov r14,r2 */
L8c60d068: *stop=0x8c60d068u;r[2]+=4u;goto L8c60d06a; /* add #0x4,r2 */
L8c60d06a: *stop=0x8c60d06au;if(!valid_ram(r[13]+4u,4u))return -3;wr(m,r[13]+0x4u,r[14],4);goto L8c60d06c; /* mov.l r14,@(0x4,r13) */
L8c60d06c: *stop=0x8c60d06cu;if(!valid_ram(r[13]+8u,4u))return -3;wr(m,r[13]+0x8u,r[2],4);goto L8c60d06e; /* mov.l r2,@(0x8,r13) */
L8c60d06e: *stop=0x8c60d06eu;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[3],4);goto L8c60d070; /* mov.l r3,@(0x14,r13) */
L8c60d070: *stop=0x8c60d070u;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c60d072; /* mov.l @r1,r2 */
L8c60d072: target=r[2];*pr=0x8c60d076u;*stop=0x8c60d074u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d076; /* jsr @r2 */
L8c60d076: *stop=0x8c60d076u;r[2]=rd(m,0x8c60d134u,4);goto L8c60d078; /* mov.l 0x8c60d134,r2 */
L8c60d078: *stop=0x8c60d078u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d07a; /* mov.l @r2,r3 */
L8c60d07a: target=r[3];*pr=0x8c60d07eu;*stop=0x8c60d07cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d07e; /* jsr @r3 */
L8c60d07e: *stop=0x8c60d07eu;r[2]=rd(m,0x8c60d138u,4);goto L8c60d080; /* mov.l 0x8c60d138,r2 */
L8c60d080: *stop=0x8c60d080u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d082; /* mov.l @r2,r3 */
L8c60d082: target=r[3];*pr=0x8c60d086u;*stop=0x8c60d084u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d086; /* jsr @r3 */
L8c60d086: *stop=0x8c60d086u;r[14]+=8u;goto L8c60d088; /* add #0x8,r14 */
L8c60d088: *stop=0x8c60d088u;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[14],4);goto L8c60d08a; /* mov.l r14,@r13 */
L8c60d08a: *stop=0x8c60d08au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60d08c; /* lds.l @r15+,PR */
L8c60d08c: *stop=0x8c60d08cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60d08e; /* mov.l @r15+,r12 */
L8c60d08e: *stop=0x8c60d08eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60d090; /* mov.l @r15+,r13 */
L8c60d090: *stop=0x8c60d090u;if(*pr!=entry_pr)return -3;*stop=0x8c60d092u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_format_two_8c60d094(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60d094;
L8c60d094: *stop=0x8c60d094u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60d096; /* mov.l r14,@-r15 */
L8c60d096: *stop=0x8c60d096u;r[3]=0u;goto L8c60d098; /* mov #0x0,r3 */
L8c60d098: *stop=0x8c60d098u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60d09a; /* mov.l r13,@-r15 */
L8c60d09a: *stop=0x8c60d09au;r[13]=rd(m,0x8c60d13cu,4);goto L8c60d09c; /* mov.l 0x8c60d13c,r13 */
L8c60d09c: *stop=0x8c60d09cu;r[1]=rd(m,0x8c60d130u,4);goto L8c60d09e; /* mov.l 0x8c60d130,r1 */
L8c60d09e: *stop=0x8c60d09eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60d0a0; /* mov.l r12,@-r15 */
L8c60d0a0: *stop=0x8c60d0a0u;r[12]=r[4];goto L8c60d0a2; /* mov r4,r12 */
L8c60d0a2: *stop=0x8c60d0a2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60d0a4; /* sts.l PR,@-r15 */
L8c60d0a4: *stop=0x8c60d0a4u;if(!valid_ram(r[13],4u))return -3;r[14]=rd(m,r[13],4);goto L8c60d0a6; /* mov.l @r13,r14 */
L8c60d0a6: *stop=0x8c60d0a6u;r[2]=r[14];goto L8c60d0a8; /* mov r14,r2 */
L8c60d0a8: *stop=0x8c60d0a8u;r[2]+=8u;goto L8c60d0aa; /* add #0x8,r2 */
L8c60d0aa: *stop=0x8c60d0aau;if(!valid_ram(r[13]+4u,4u))return -3;wr(m,r[13]+0x4u,r[14],4);goto L8c60d0ac; /* mov.l r14,@(0x4,r13) */
L8c60d0ac: *stop=0x8c60d0acu;if(!valid_ram(r[13]+8u,4u))return -3;wr(m,r[13]+0x8u,r[2],4);goto L8c60d0ae; /* mov.l r2,@(0x8,r13) */
L8c60d0ae: *stop=0x8c60d0aeu;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[3],4);goto L8c60d0b0; /* mov.l r3,@(0x14,r13) */
L8c60d0b0: *stop=0x8c60d0b0u;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c60d0b2; /* mov.l @r1,r2 */
L8c60d0b2: target=r[2];*pr=0x8c60d0b6u;*stop=0x8c60d0b4u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d0b6; /* jsr @r2 */
L8c60d0b6: *stop=0x8c60d0b6u;r[2]=rd(m,0x8c60d134u,4);goto L8c60d0b8; /* mov.l 0x8c60d134,r2 */
L8c60d0b8: *stop=0x8c60d0b8u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d0ba; /* mov.l @r2,r3 */
L8c60d0ba: target=r[3];*pr=0x8c60d0beu;*stop=0x8c60d0bcu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d0be; /* jsr @r3 */
L8c60d0be: *stop=0x8c60d0beu;r[2]=rd(m,0x8c60d138u,4);goto L8c60d0c0; /* mov.l 0x8c60d138,r2 */
L8c60d0c0: *stop=0x8c60d0c0u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d0c2; /* mov.l @r2,r3 */
L8c60d0c2: target=r[3];*pr=0x8c60d0c6u;*stop=0x8c60d0c4u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d0c6; /* jsr @r3 */
L8c60d0c6: *stop=0x8c60d0c6u;r[14]+=16u;goto L8c60d0c8; /* add #0x10,r14 */
L8c60d0c8: *stop=0x8c60d0c8u;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[14],4);goto L8c60d0ca; /* mov.l r14,@r13 */
L8c60d0ca: *stop=0x8c60d0cau;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60d0cc; /* lds.l @r15+,PR */
L8c60d0cc: *stop=0x8c60d0ccu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60d0ce; /* mov.l @r15+,r12 */
L8c60d0ce: *stop=0x8c60d0ceu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60d0d0; /* mov.l @r15+,r13 */
L8c60d0d0: *stop=0x8c60d0d0u;if(*pr!=entry_pr)return -3;*stop=0x8c60d0d2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_format_three_8c60d0d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60d0d4;
L8c60d0d4: *stop=0x8c60d0d4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60d0d6; /* mov.l r14,@-r15 */
L8c60d0d6: *stop=0x8c60d0d6u;r[3]=0u;goto L8c60d0d8; /* mov #0x0,r3 */
L8c60d0d8: *stop=0x8c60d0d8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60d0da; /* mov.l r13,@-r15 */
L8c60d0da: *stop=0x8c60d0dau;r[13]=rd(m,0x8c60d13cu,4);goto L8c60d0dc; /* mov.l 0x8c60d13c,r13 */
L8c60d0dc: *stop=0x8c60d0dcu;r[1]=rd(m,0x8c60d130u,4);goto L8c60d0de; /* mov.l 0x8c60d130,r1 */
L8c60d0de: *stop=0x8c60d0deu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60d0e0; /* mov.l r12,@-r15 */
L8c60d0e0: *stop=0x8c60d0e0u;r[12]=r[4];goto L8c60d0e2; /* mov r4,r12 */
L8c60d0e2: *stop=0x8c60d0e2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60d0e4; /* sts.l PR,@-r15 */
L8c60d0e4: *stop=0x8c60d0e4u;if(!valid_ram(r[13],4u))return -3;r[14]=rd(m,r[13],4);goto L8c60d0e6; /* mov.l @r13,r14 */
L8c60d0e6: *stop=0x8c60d0e6u;r[2]=r[14];goto L8c60d0e8; /* mov r14,r2 */
L8c60d0e8: *stop=0x8c60d0e8u;r[2]+=12u;goto L8c60d0ea; /* add #0xc,r2 */
L8c60d0ea: *stop=0x8c60d0eau;if(!valid_ram(r[13]+4u,4u))return -3;wr(m,r[13]+0x4u,r[14],4);goto L8c60d0ec; /* mov.l r14,@(0x4,r13) */
L8c60d0ec: *stop=0x8c60d0ecu;if(!valid_ram(r[13]+8u,4u))return -3;wr(m,r[13]+0x8u,r[2],4);goto L8c60d0ee; /* mov.l r2,@(0x8,r13) */
L8c60d0ee: *stop=0x8c60d0eeu;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[3],4);goto L8c60d0f0; /* mov.l r3,@(0x14,r13) */
L8c60d0f0: *stop=0x8c60d0f0u;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c60d0f2; /* mov.l @r1,r2 */
L8c60d0f2: target=r[2];*pr=0x8c60d0f6u;*stop=0x8c60d0f4u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d0f6; /* jsr @r2 */
L8c60d0f6: *stop=0x8c60d0f6u;r[2]=rd(m,0x8c60d134u,4);goto L8c60d0f8; /* mov.l 0x8c60d134,r2 */
L8c60d0f8: *stop=0x8c60d0f8u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d0fa; /* mov.l @r2,r3 */
L8c60d0fa: target=r[3];*pr=0x8c60d0feu;*stop=0x8c60d0fcu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d0fe; /* jsr @r3 */
L8c60d0fe: *stop=0x8c60d0feu;r[2]=rd(m,0x8c60d138u,4);goto L8c60d100; /* mov.l 0x8c60d138,r2 */
L8c60d100: *stop=0x8c60d100u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d102; /* mov.l @r2,r3 */
L8c60d102: target=r[3];*pr=0x8c60d106u;*stop=0x8c60d104u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d106; /* jsr @r3 */
L8c60d106: *stop=0x8c60d106u;r[14]+=24u;goto L8c60d108; /* add #0x18,r14 */
L8c60d108: *stop=0x8c60d108u;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[14],4);goto L8c60d10a; /* mov.l r14,@r13 */
L8c60d10a: *stop=0x8c60d10au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60d10c; /* lds.l @r15+,PR */
L8c60d10c: *stop=0x8c60d10cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60d10e; /* mov.l @r15+,r12 */
L8c60d10e: *stop=0x8c60d10eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60d110; /* mov.l @r15+,r13 */
L8c60d110: *stop=0x8c60d110u;if(*pr!=entry_pr)return -3;*stop=0x8c60d112u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_format_four_8c60d140(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60d140;
L8c60d140: *stop=0x8c60d140u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60d142; /* mov.l r14,@-r15 */
L8c60d142: *stop=0x8c60d142u;r[3]=0u;goto L8c60d144; /* mov #0x0,r3 */
L8c60d144: *stop=0x8c60d144u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60d146; /* mov.l r13,@-r15 */
L8c60d146: *stop=0x8c60d146u;r[13]=rd(m,0x8c60d250u,4);goto L8c60d148; /* mov.l 0x8c60d250,r13 */
L8c60d148: *stop=0x8c60d148u;r[1]=rd(m,0x8c60d254u,4);goto L8c60d14a; /* mov.l 0x8c60d254,r1 */
L8c60d14a: *stop=0x8c60d14au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60d14c; /* mov.l r12,@-r15 */
L8c60d14c: *stop=0x8c60d14cu;r[12]=r[4];goto L8c60d14e; /* mov r4,r12 */
L8c60d14e: *stop=0x8c60d14eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60d150; /* sts.l PR,@-r15 */
L8c60d150: *stop=0x8c60d150u;if(!valid_ram(r[13],4u))return -3;r[14]=rd(m,r[13],4);goto L8c60d152; /* mov.l @r13,r14 */
L8c60d152: *stop=0x8c60d152u;r[2]=r[14];goto L8c60d154; /* mov r14,r2 */
L8c60d154: *stop=0x8c60d154u;r[2]+=16u;goto L8c60d156; /* add #0x10,r2 */
L8c60d156: *stop=0x8c60d156u;if(!valid_ram(r[13]+4u,4u))return -3;wr(m,r[13]+0x4u,r[14],4);goto L8c60d158; /* mov.l r14,@(0x4,r13) */
L8c60d158: *stop=0x8c60d158u;if(!valid_ram(r[13]+8u,4u))return -3;wr(m,r[13]+0x8u,r[2],4);goto L8c60d15a; /* mov.l r2,@(0x8,r13) */
L8c60d15a: *stop=0x8c60d15au;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[3],4);goto L8c60d15c; /* mov.l r3,@(0x14,r13) */
L8c60d15c: *stop=0x8c60d15cu;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c60d15e; /* mov.l @r1,r2 */
L8c60d15e: target=r[2];*pr=0x8c60d162u;*stop=0x8c60d160u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d162; /* jsr @r2 */
L8c60d162: *stop=0x8c60d162u;r[2]=rd(m,0x8c60d258u,4);goto L8c60d164; /* mov.l 0x8c60d258,r2 */
L8c60d164: *stop=0x8c60d164u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d166; /* mov.l @r2,r3 */
L8c60d166: target=r[3];*pr=0x8c60d16au;*stop=0x8c60d168u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d16a; /* jsr @r3 */
L8c60d16a: *stop=0x8c60d16au;r[2]=rd(m,0x8c60d25cu,4);goto L8c60d16c; /* mov.l 0x8c60d25c,r2 */
L8c60d16c: *stop=0x8c60d16cu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d16e; /* mov.l @r2,r3 */
L8c60d16e: target=r[3];*pr=0x8c60d172u;*stop=0x8c60d170u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d172; /* jsr @r3 */
L8c60d172: *stop=0x8c60d172u;r[14]+=32u;goto L8c60d174; /* add #0x20,r14 */
L8c60d174: *stop=0x8c60d174u;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[14],4);goto L8c60d176; /* mov.l r14,@r13 */
L8c60d176: *stop=0x8c60d176u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60d178; /* lds.l @r15+,PR */
L8c60d178: *stop=0x8c60d178u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60d17a; /* mov.l @r15+,r12 */
L8c60d17a: *stop=0x8c60d17au;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60d17c; /* mov.l @r15+,r13 */
L8c60d17c: *stop=0x8c60d17cu;if(*pr!=entry_pr)return -3;*stop=0x8c60d17eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_format_mesh_8c60d180(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60d180;
L8c60d180: *stop=0x8c60d180u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60d182; /* mov.l r14,@-r15 */
L8c60d182: *stop=0x8c60d182u;r[14]=r[4];goto L8c60d184; /* mov r4,r14 */
L8c60d184: *stop=0x8c60d184u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c60d186; /* mov.l @(0x4,r14),r3 */
L8c60d186: *stop=0x8c60d186u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c60d188; /* tst r3,r3 */
L8c60d188: if((*sr&1u))goto L8c60d19a;goto L8c60d18a; /* bt 0x8c60d19a */
L8c60d18a: *stop=0x8c60d18au;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c60d18c; /* mov.l @r14,r0 */
L8c60d18c: *stop=0x8c60d18cu;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c60d18e; /* tst #0x8,r0 */
L8c60d18e: if(!(*sr&1u))goto L8c60d19a;goto L8c60d190; /* bf 0x8c60d19a */
L8c60d190: *stop=0x8c60d190u;r[3]=rd(m,0x8c60d260u,4);goto L8c60d192; /* mov.l 0x8c60d260,r3 */
L8c60d192: *stop=0x8c60d192u;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);goto L8c60d194; /* mov.l @(0x4,r14),r4 */
L8c60d194: *stop=0x8c60d194u;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c60d196; /* mov.l @r3,r2 */
L8c60d196: target=r[2];*stop=0x8c60d198u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c60d19a: *stop=0x8c60d19au;if(*pr!=entry_pr)return -3;*stop=0x8c60d19cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_format_six_seven_8c60d19e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60d19e;
L8c60d19e: *stop=0x8c60d19eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60d1a0; /* mov.l r14,@-r15 */
L8c60d1a0: *stop=0x8c60d1a0u;r[3]=0u;goto L8c60d1a2; /* mov #0x0,r3 */
L8c60d1a2: *stop=0x8c60d1a2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60d1a4; /* mov.l r13,@-r15 */
L8c60d1a4: *stop=0x8c60d1a4u;r[13]=rd(m,0x8c60d264u,4);goto L8c60d1a6; /* mov.l 0x8c60d264,r13 */
L8c60d1a6: *stop=0x8c60d1a6u;r[1]=rd(m,0x8c60d268u,4);goto L8c60d1a8; /* mov.l 0x8c60d268,r1 */
L8c60d1a8: *stop=0x8c60d1a8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60d1aa; /* sts.l PR,@-r15 */
L8c60d1aa: *stop=0x8c60d1aau;if(!valid_ram(r[13],4u))return -3;r[14]=rd(m,r[13],4);goto L8c60d1ac; /* mov.l @r13,r14 */
L8c60d1ac: *stop=0x8c60d1acu;r[2]=r[14];goto L8c60d1ae; /* mov r14,r2 */
L8c60d1ae: *stop=0x8c60d1aeu;r[2]+=4u;goto L8c60d1b0; /* add #0x4,r2 */
L8c60d1b0: *stop=0x8c60d1b0u;if(!valid_ram(r[13]+4u,4u))return -3;wr(m,r[13]+0x4u,r[14],4);goto L8c60d1b2; /* mov.l r14,@(0x4,r13) */
L8c60d1b2: *stop=0x8c60d1b2u;if(!valid_ram(r[13]+8u,4u))return -3;wr(m,r[13]+0x8u,r[2],4);goto L8c60d1b4; /* mov.l r2,@(0x8,r13) */
L8c60d1b4: *stop=0x8c60d1b4u;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[3],4);goto L8c60d1b6; /* mov.l r3,@(0x14,r13) */
L8c60d1b6: *stop=0x8c60d1b6u;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c60d1b8; /* mov.l @r1,r2 */
L8c60d1b8: target=r[2];*pr=0x8c60d1bcu;*stop=0x8c60d1bau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d1bc; /* jsr @r2 */
L8c60d1bc: *stop=0x8c60d1bcu;r[14]+=8u;goto L8c60d1be; /* add #0x8,r14 */
L8c60d1be: *stop=0x8c60d1beu;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[14],4);goto L8c60d1c0; /* mov.l r14,@r13 */
L8c60d1c0: *stop=0x8c60d1c0u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60d1c2; /* lds.l @r15+,PR */
L8c60d1c2: *stop=0x8c60d1c2u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60d1c4; /* mov.l @r15+,r13 */
L8c60d1c4: *stop=0x8c60d1c4u;if(*pr!=entry_pr)return -3;*stop=0x8c60d1c6u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_format_eight_8c60d1c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60d1c8;
L8c60d1c8: *stop=0x8c60d1c8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60d1ca; /* mov.l r14,@-r15 */
L8c60d1ca: *stop=0x8c60d1cau;r[3]=0u;goto L8c60d1cc; /* mov #0x0,r3 */
L8c60d1cc: *stop=0x8c60d1ccu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60d1ce; /* mov.l r13,@-r15 */
L8c60d1ce: *stop=0x8c60d1ceu;r[13]=rd(m,0x8c60d264u,4);goto L8c60d1d0; /* mov.l 0x8c60d264,r13 */
L8c60d1d0: *stop=0x8c60d1d0u;r[1]=rd(m,0x8c60d268u,4);goto L8c60d1d2; /* mov.l 0x8c60d268,r1 */
L8c60d1d2: *stop=0x8c60d1d2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60d1d4; /* sts.l PR,@-r15 */
L8c60d1d4: *stop=0x8c60d1d4u;if(!valid_ram(r[13],4u))return -3;r[14]=rd(m,r[13],4);goto L8c60d1d6; /* mov.l @r13,r14 */
L8c60d1d6: *stop=0x8c60d1d6u;r[2]=r[14];goto L8c60d1d8; /* mov r14,r2 */
L8c60d1d8: *stop=0x8c60d1d8u;r[2]+=8u;goto L8c60d1da; /* add #0x8,r2 */
L8c60d1da: *stop=0x8c60d1dau;if(!valid_ram(r[13]+4u,4u))return -3;wr(m,r[13]+0x4u,r[14],4);goto L8c60d1dc; /* mov.l r14,@(0x4,r13) */
L8c60d1dc: *stop=0x8c60d1dcu;if(!valid_ram(r[13]+8u,4u))return -3;wr(m,r[13]+0x8u,r[2],4);goto L8c60d1de; /* mov.l r2,@(0x8,r13) */
L8c60d1de: *stop=0x8c60d1deu;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[3],4);goto L8c60d1e0; /* mov.l r3,@(0x14,r13) */
L8c60d1e0: *stop=0x8c60d1e0u;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c60d1e2; /* mov.l @r1,r2 */
L8c60d1e2: target=r[2];*pr=0x8c60d1e6u;*stop=0x8c60d1e4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d1e6; /* jsr @r2 */
L8c60d1e6: *stop=0x8c60d1e6u;r[14]+=16u;goto L8c60d1e8; /* add #0x10,r14 */
L8c60d1e8: *stop=0x8c60d1e8u;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[14],4);goto L8c60d1ea; /* mov.l r14,@r13 */
L8c60d1ea: *stop=0x8c60d1eau;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60d1ec; /* lds.l @r15+,PR */
L8c60d1ec: *stop=0x8c60d1ecu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60d1ee; /* mov.l @r15+,r13 */
L8c60d1ee: *stop=0x8c60d1eeu;if(*pr!=entry_pr)return -3;*stop=0x8c60d1f0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sample_position_8c60c322(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60c322;
L8c60c322: *stop=0x8c60c322u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60c324; /* mov.l r14,@-r15 */
L8c60c324: *stop=0x8c60c324u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60c326; /* mov.l r13,@-r15 */
L8c60c326: *stop=0x8c60c326u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60c328; /* sts.l PR,@-r15 */
L8c60c328: *stop=0x8c60c328u;r[15]+=4294967280u;goto L8c60c32a; /* add #-0x10,r15 */
L8c60c32a: *stop=0x8c60c32au;r[13]=rd(m,0x8c60c470u,4);goto L8c60c32c; /* mov.l 0x8c60c470,r13 */
L8c60c32c: *stop=0x8c60c32cu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[4],4);goto L8c60c32e; /* mov.l r4,@r15 */
L8c60c32e: *stop=0x8c60c32eu;if(!valid_ram(r[13]+20u,4u))return -3;r[5]=rd(m,r[13]+0x14u,4);goto L8c60c330; /* mov.l @(0x14,r13),r5 */
L8c60c330: *stop=0x8c60c330u;if(!valid_ram(r[13]+4u,4u))return -3;r[0]=rd(m,r[13]+0x4u,4);goto L8c60c332; /* mov.l @(0x4,r13),r0 */
L8c60c332: *stop=0x8c60c332u;r[5]<<=2;goto L8c60c334; /* shll2 r5 */
L8c60c334: *stop=0x8c60c334u;if(!valid_ram(r[0]+r[5],4u))return -3;r[4]=rd(m,r[5]+r[0],4);goto L8c60c336; /* mov.l @(r0,r5),r4 */
L8c60c336: *stop=0x8c60c336u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c60c338; /* tst r4,r4 */
L8c60c338: if((*sr&1u))goto L8c60c394;goto L8c60c33a; /* bt 0x8c60c394 */
L8c60c33a: *stop=0x8c60c33au;if(!valid_ram(r[13]+8u,4u))return -3;r[0]=rd(m,r[13]+0x8u,4);goto L8c60c33c; /* mov.l @(0x8,r13),r0 */
L8c60c33c: *stop=0x8c60c33cu;if(!valid_ram(r[0]+r[5],4u))return -3;r[6]=rd(m,r[5]+r[0],4);goto L8c60c33e; /* mov.l @(r0,r5),r6 */
L8c60c33e: *stop=0x8c60c33eu;r[0]=12u;goto L8c60c340; /* mov #0xc,r0 */
L8c60c340: *stop=0x8c60c340u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto L8c60c342; /* fmov.s @(r0,r13),fr4 */
L8c60c342: *stop=0x8c60c342u;r[0]=0x8c60c474u;goto L8c60c344; /* mova 0x8c60c474,r0 */
L8c60c344: *stop=0x8c60c344u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c60c346; /* fmov.s @r0,fr3 */
L8c60c346: *stop=0x8c60c346u;r[5]=0u;goto L8c60c348; /* mov #0x0,r5 */
L8c60c348: *stop=0x8c60c348u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c60c34a; /* fcmp/gt fr3,fr4 */
L8c60c34a: take=!(*sr&1u);*stop=0x8c60c34cu;r[14]=r[6];if(take)goto L8c60c358;goto L8c60c34e; /* bf/s 0x8c60c358 */
L8c60c34e: *stop=0x8c60c34eu;r[0]=0x8c60c478u;goto L8c60c350; /* mova 0x8c60c478,r0 */
L8c60c350: *stop=0x8c60c350u;fcopy(f,3u,4u);goto L8c60c352; /* fmov fr4,fr3 */
L8c60c352: *stop=0x8c60c352u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c60c354; /* fmov.s @r0,fr2 */
L8c60c354: *stop=0x8c60c356u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60c35a; /* bra 0x8c60c35a */
L8c60c358: *stop=0x8c60c358u;fcopy(f,3u,4u);goto L8c60c35a; /* fmov fr4,fr3 */
L8c60c35a: *stop=0x8c60c35au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c60c35c; /* ftrc fr3,FPUL */
L8c60c35c: *stop=0x8c60c35cu;r[1]=*fpul;goto L8c60c35e; /* sts FPUL,r1 */
L8c60c35e: *stop=0x8c60c360u;r[0]=1u;goto L8c60c37c; /* bra 0x8c60c37c */
L8c60c362: *stop=0x8c60c362u;r[7]=r[5];goto L8c60c364; /* mov r5,r7 */
L8c60c364: *stop=0x8c60c364u;r[7]+=r[14];goto L8c60c366; /* add r14,r7 */
L8c60c366: *stop=0x8c60c366u;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c60c368; /* shlr r7 */
L8c60c368: *stop=0x8c60c368u;r[2]=r[7];goto L8c60c36a; /* mov r7,r2 */
L8c60c36a: *stop=0x8c60c36au;r[2]<<=2;goto L8c60c36c; /* shll2 r2 */
L8c60c36c: *stop=0x8c60c36cu;r[2]<<=2;goto L8c60c36e; /* shll2 r2 */
L8c60c36e: *stop=0x8c60c36eu;r[2]+=r[4];goto L8c60c370; /* add r4,r2 */
L8c60c370: *stop=0x8c60c370u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60c372; /* mov.l @r2,r3 */
L8c60c372: *stop=0x8c60c372u;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c60c374; /* cmp/hs r3,r1 */
L8c60c374: if(!(*sr&1u))goto L8c60c37a;goto L8c60c376; /* bf 0x8c60c37a */
L8c60c376: *stop=0x8c60c378u;r[5]=r[7];goto L8c60c37c; /* bra 0x8c60c37c */
L8c60c37a: *stop=0x8c60c37au;r[14]=r[7];goto L8c60c37c; /* mov r7,r14 */
L8c60c37c: *stop=0x8c60c37cu;r[2]=r[14];goto L8c60c37e; /* mov r14,r2 */
L8c60c37e: *stop=0x8c60c37eu;r[2]-=r[5];goto L8c60c380; /* sub r5,r2 */
L8c60c380: *stop=0x8c60c380u;*sr=(*sr&~1u)|(r[2]>r[0]);goto L8c60c382; /* cmp/hi r0,r2 */
L8c60c382: if((*sr&1u))goto L8c60c362;goto L8c60c384; /* bt 0x8c60c362 */
L8c60c384: *stop=0x8c60c384u;r[3]=rd(m,0x8c60c47cu,4);goto L8c60c386; /* mov.l 0x8c60c47c,r3 */
L8c60c386: *stop=0x8c60c386u;r[14]=r[15];goto L8c60c388; /* mov r15,r14 */
L8c60c388: *stop=0x8c60c388u;r[14]+=4u;goto L8c60c38a; /* add #0x4,r14 */
L8c60c38a: *stop=0x8c60c38au;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c60c38c; /* mov.l @r3,r2 */
L8c60c38c: target=r[2];*pr=0x8c60c390u;*stop=0x8c60c38eu;r[7]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c390; /* jsr @r2 */
L8c60c390: *stop=0x8c60c392u;goto L8c60c398; /* bra 0x8c60c398 */
L8c60c394: *stop=0x8c60c394u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);goto L8c60c396; /* mov.l @r15,r14 */
L8c60c396: *stop=0x8c60c396u;r[14]+=8u;goto L8c60c398; /* add #0x8,r14 */
L8c60c398: *stop=0x8c60c398u;r[0]=8u;goto L8c60c39a; /* mov #0x8,r0 */
L8c60c39a: *stop=0x8c60c39au;r[3]=rd(m,0x8c60c480u,4);goto L8c60c39c; /* mov.l 0x8c60c480,r3 */
L8c60c39c: *stop=0x8c60c39cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[14]);goto L8c60c39e; /* fmov.s @(r0,r14),fr6 */
L8c60c39e: *stop=0x8c60c39eu;r[0]=4u;goto L8c60c3a0; /* mov #0x4,r0 */
L8c60c3a0: *stop=0x8c60c3a0u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[14]);goto L8c60c3a2; /* fmov.s @r14,fr4 */
L8c60c3a2: *stop=0x8c60c3a2u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto L8c60c3a4; /* fmov.s @(r0,r14),fr5 */
L8c60c3a4: target=r[3];*pr=0x8c60c3a8u;*stop=0x8c60c3a6u;if(!valid_ram(r[13]+24u,4u))return -3;r[4]=rd(m,r[13]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c3a8; /* jsr @r3 */
L8c60c3a8: *stop=0x8c60c3a8u;if(!valid_ram(r[13]+20u,4u))return -3;r[2]=rd(m,r[13]+0x14u,4);goto L8c60c3aa; /* mov.l @(0x14,r13),r2 */
L8c60c3aa: *stop=0x8c60c3aau;r[2]+=1u;goto L8c60c3ac; /* add #0x1,r2 */
L8c60c3ac: *stop=0x8c60c3acu;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[2],4);goto L8c60c3ae; /* mov.l r2,@(0x14,r13) */
L8c60c3ae: *stop=0x8c60c3aeu;r[15]+=16u;goto L8c60c3b0; /* add #0x10,r15 */
L8c60c3b0: *stop=0x8c60c3b0u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60c3b2; /* lds.l @r15+,PR */
L8c60c3b2: *stop=0x8c60c3b2u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60c3b4; /* mov.l @r15+,r13 */
L8c60c3b4: *stop=0x8c60c3b4u;if(*pr!=entry_pr)return -3;*stop=0x8c60c3b6u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sample_scale_8c60c3b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60c3b8;
L8c60c3b8: *stop=0x8c60c3b8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60c3ba; /* mov.l r14,@-r15 */
L8c60c3ba: *stop=0x8c60c3bau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60c3bc; /* mov.l r13,@-r15 */
L8c60c3bc: *stop=0x8c60c3bcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60c3be; /* sts.l PR,@-r15 */
L8c60c3be: *stop=0x8c60c3beu;r[15]+=4294967280u;goto L8c60c3c0; /* add #-0x10,r15 */
L8c60c3c0: *stop=0x8c60c3c0u;r[13]=rd(m,0x8c60c470u,4);goto L8c60c3c2; /* mov.l 0x8c60c470,r13 */
L8c60c3c2: *stop=0x8c60c3c2u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[4],4);goto L8c60c3c4; /* mov.l r4,@r15 */
L8c60c3c4: *stop=0x8c60c3c4u;if(!valid_ram(r[13]+20u,4u))return -3;r[5]=rd(m,r[13]+0x14u,4);goto L8c60c3c6; /* mov.l @(0x14,r13),r5 */
L8c60c3c6: *stop=0x8c60c3c6u;if(!valid_ram(r[13]+4u,4u))return -3;r[0]=rd(m,r[13]+0x4u,4);goto L8c60c3c8; /* mov.l @(0x4,r13),r0 */
L8c60c3c8: *stop=0x8c60c3c8u;r[5]<<=2;goto L8c60c3ca; /* shll2 r5 */
L8c60c3ca: *stop=0x8c60c3cau;if(!valid_ram(r[0]+r[5],4u))return -3;r[4]=rd(m,r[5]+r[0],4);goto L8c60c3cc; /* mov.l @(r0,r5),r4 */
L8c60c3cc: *stop=0x8c60c3ccu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c60c3ce; /* tst r4,r4 */
L8c60c3ce: if((*sr&1u))goto L8c60c42a;goto L8c60c3d0; /* bt 0x8c60c42a */
L8c60c3d0: *stop=0x8c60c3d0u;if(!valid_ram(r[13]+8u,4u))return -3;r[0]=rd(m,r[13]+0x8u,4);goto L8c60c3d2; /* mov.l @(0x8,r13),r0 */
L8c60c3d2: *stop=0x8c60c3d2u;if(!valid_ram(r[0]+r[5],4u))return -3;r[6]=rd(m,r[5]+r[0],4);goto L8c60c3d4; /* mov.l @(r0,r5),r6 */
L8c60c3d4: *stop=0x8c60c3d4u;r[0]=12u;goto L8c60c3d6; /* mov #0xc,r0 */
L8c60c3d6: *stop=0x8c60c3d6u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto L8c60c3d8; /* fmov.s @(r0,r13),fr4 */
L8c60c3d8: *stop=0x8c60c3d8u;r[0]=0x8c60c474u;goto L8c60c3da; /* mova 0x8c60c474,r0 */
L8c60c3da: *stop=0x8c60c3dau;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c60c3dc; /* fmov.s @r0,fr3 */
L8c60c3dc: *stop=0x8c60c3dcu;r[5]=0u;goto L8c60c3de; /* mov #0x0,r5 */
L8c60c3de: *stop=0x8c60c3deu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c60c3e0; /* fcmp/gt fr3,fr4 */
L8c60c3e0: take=!(*sr&1u);*stop=0x8c60c3e2u;r[14]=r[6];if(take)goto L8c60c3ee;goto L8c60c3e4; /* bf/s 0x8c60c3ee */
L8c60c3e4: *stop=0x8c60c3e4u;r[0]=0x8c60c478u;goto L8c60c3e6; /* mova 0x8c60c478,r0 */
L8c60c3e6: *stop=0x8c60c3e6u;fcopy(f,3u,4u);goto L8c60c3e8; /* fmov fr4,fr3 */
L8c60c3e8: *stop=0x8c60c3e8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c60c3ea; /* fmov.s @r0,fr2 */
L8c60c3ea: *stop=0x8c60c3ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60c3f0; /* bra 0x8c60c3f0 */
L8c60c3ee: *stop=0x8c60c3eeu;fcopy(f,3u,4u);goto L8c60c3f0; /* fmov fr4,fr3 */
L8c60c3f0: *stop=0x8c60c3f0u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c60c3f2; /* ftrc fr3,FPUL */
L8c60c3f2: *stop=0x8c60c3f2u;r[1]=*fpul;goto L8c60c3f4; /* sts FPUL,r1 */
L8c60c3f4: *stop=0x8c60c3f6u;r[0]=1u;goto L8c60c412; /* bra 0x8c60c412 */
L8c60c3f8: *stop=0x8c60c3f8u;r[7]=r[5];goto L8c60c3fa; /* mov r5,r7 */
L8c60c3fa: *stop=0x8c60c3fau;r[7]+=r[14];goto L8c60c3fc; /* add r14,r7 */
L8c60c3fc: *stop=0x8c60c3fcu;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c60c3fe; /* shlr r7 */
L8c60c3fe: *stop=0x8c60c3feu;r[2]=r[7];goto L8c60c400; /* mov r7,r2 */
L8c60c400: *stop=0x8c60c400u;r[2]<<=2;goto L8c60c402; /* shll2 r2 */
L8c60c402: *stop=0x8c60c402u;r[2]<<=2;goto L8c60c404; /* shll2 r2 */
L8c60c404: *stop=0x8c60c404u;r[2]+=r[4];goto L8c60c406; /* add r4,r2 */
L8c60c406: *stop=0x8c60c406u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60c408; /* mov.l @r2,r3 */
L8c60c408: *stop=0x8c60c408u;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c60c40a; /* cmp/hs r3,r1 */
L8c60c40a: if(!(*sr&1u))goto L8c60c410;goto L8c60c40c; /* bf 0x8c60c410 */
L8c60c40c: *stop=0x8c60c40eu;r[5]=r[7];goto L8c60c412; /* bra 0x8c60c412 */
L8c60c410: *stop=0x8c60c410u;r[14]=r[7];goto L8c60c412; /* mov r7,r14 */
L8c60c412: *stop=0x8c60c412u;r[2]=r[14];goto L8c60c414; /* mov r14,r2 */
L8c60c414: *stop=0x8c60c414u;r[2]-=r[5];goto L8c60c416; /* sub r5,r2 */
L8c60c416: *stop=0x8c60c416u;*sr=(*sr&~1u)|(r[2]>r[0]);goto L8c60c418; /* cmp/hi r0,r2 */
L8c60c418: if((*sr&1u))goto L8c60c3f8;goto L8c60c41a; /* bt 0x8c60c3f8 */
L8c60c41a: *stop=0x8c60c41au;r[3]=rd(m,0x8c60c47cu,4);goto L8c60c41c; /* mov.l 0x8c60c47c,r3 */
L8c60c41c: *stop=0x8c60c41cu;r[14]=r[15];goto L8c60c41e; /* mov r15,r14 */
L8c60c41e: *stop=0x8c60c41eu;r[14]+=4u;goto L8c60c420; /* add #0x4,r14 */
L8c60c420: *stop=0x8c60c420u;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c60c422; /* mov.l @r3,r2 */
L8c60c422: target=r[2];*pr=0x8c60c426u;*stop=0x8c60c424u;r[7]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c426; /* jsr @r2 */
L8c60c426: *stop=0x8c60c428u;goto L8c60c42e; /* bra 0x8c60c42e */
L8c60c42a: *stop=0x8c60c42au;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);goto L8c60c42c; /* mov.l @r15,r14 */
L8c60c42c: *stop=0x8c60c42cu;r[14]+=32u;goto L8c60c42e; /* add #0x20,r14 */
L8c60c42e: *stop=0x8c60c42eu;r[0]=8u;goto L8c60c430; /* mov #0x8,r0 */
L8c60c430: *stop=0x8c60c430u;r[3]=rd(m,0x8c60c484u,4);goto L8c60c432; /* mov.l 0x8c60c484,r3 */
L8c60c432: *stop=0x8c60c432u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[14]);goto L8c60c434; /* fmov.s @(r0,r14),fr6 */
L8c60c434: *stop=0x8c60c434u;r[0]=4u;goto L8c60c436; /* mov #0x4,r0 */
L8c60c436: *stop=0x8c60c436u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[14]);goto L8c60c438; /* fmov.s @r14,fr4 */
L8c60c438: *stop=0x8c60c438u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto L8c60c43a; /* fmov.s @(r0,r14),fr5 */
L8c60c43a: target=r[3];*pr=0x8c60c43eu;*stop=0x8c60c43cu;if(!valid_ram(r[13]+24u,4u))return -3;r[4]=rd(m,r[13]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c43e; /* jsr @r3 */
L8c60c43e: *stop=0x8c60c43eu;if(!valid_ram(r[13]+20u,4u))return -3;r[2]=rd(m,r[13]+0x14u,4);goto L8c60c440; /* mov.l @(0x14,r13),r2 */
L8c60c440: *stop=0x8c60c440u;r[2]+=1u;goto L8c60c442; /* add #0x1,r2 */
L8c60c442: *stop=0x8c60c442u;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[2],4);goto L8c60c444; /* mov.l r2,@(0x14,r13) */
L8c60c444: *stop=0x8c60c444u;r[15]+=16u;goto L8c60c446; /* add #0x10,r15 */
L8c60c446: *stop=0x8c60c446u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60c448; /* lds.l @r15+,PR */
L8c60c448: *stop=0x8c60c448u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60c44a; /* mov.l @r15+,r13 */
L8c60c44a: *stop=0x8c60c44au;if(*pr!=entry_pr)return -3;*stop=0x8c60c44cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sample_rotation_8c60c488(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60c488;
L8c60c488: *stop=0x8c60c488u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60c48a; /* mov.l r14,@-r15 */
L8c60c48a: *stop=0x8c60c48au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60c48c; /* mov.l r13,@-r15 */
L8c60c48c: *stop=0x8c60c48cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60c48e; /* sts.l PR,@-r15 */
L8c60c48e: *stop=0x8c60c48eu;r[15]+=4294967280u;goto L8c60c490; /* add #-0x10,r15 */
L8c60c490: *stop=0x8c60c490u;r[13]=rd(m,0x8c60c714u,4);goto L8c60c492; /* mov.l 0x8c60c714,r13 */
L8c60c492: *stop=0x8c60c492u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[4],4);goto L8c60c494; /* mov.l r4,@r15 */
L8c60c494: *stop=0x8c60c494u;if(!valid_ram(r[13]+20u,4u))return -3;r[5]=rd(m,r[13]+0x14u,4);goto L8c60c496; /* mov.l @(0x14,r13),r5 */
L8c60c496: *stop=0x8c60c496u;if(!valid_ram(r[13]+4u,4u))return -3;r[0]=rd(m,r[13]+0x4u,4);goto L8c60c498; /* mov.l @(0x4,r13),r0 */
L8c60c498: *stop=0x8c60c498u;r[5]<<=2;goto L8c60c49a; /* shll2 r5 */
L8c60c49a: *stop=0x8c60c49au;if(!valid_ram(r[0]+r[5],4u))return -3;r[4]=rd(m,r[5]+r[0],4);goto L8c60c49c; /* mov.l @(r0,r5),r4 */
L8c60c49c: *stop=0x8c60c49cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c60c49e; /* tst r4,r4 */
L8c60c49e: if((*sr&1u))goto L8c60c506;goto L8c60c4a0; /* bt 0x8c60c506 */
L8c60c4a0: *stop=0x8c60c4a0u;if(!valid_ram(r[13]+8u,4u))return -3;r[0]=rd(m,r[13]+0x8u,4);goto L8c60c4a2; /* mov.l @(0x8,r13),r0 */
L8c60c4a2: *stop=0x8c60c4a2u;if(!valid_ram(r[0]+r[5],4u))return -3;r[6]=rd(m,r[5]+r[0],4);goto L8c60c4a4; /* mov.l @(r0,r5),r6 */
L8c60c4a4: *stop=0x8c60c4a4u;r[0]=12u;goto L8c60c4a6; /* mov #0xc,r0 */
L8c60c4a6: *stop=0x8c60c4a6u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto L8c60c4a8; /* fmov.s @(r0,r13),fr4 */
L8c60c4a8: *stop=0x8c60c4a8u;r[0]=0x8c60c718u;goto L8c60c4aa; /* mova 0x8c60c718,r0 */
L8c60c4aa: *stop=0x8c60c4aau;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c60c4ac; /* fmov.s @r0,fr3 */
L8c60c4ac: *stop=0x8c60c4acu;r[5]=0u;goto L8c60c4ae; /* mov #0x0,r5 */
L8c60c4ae: *stop=0x8c60c4aeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c60c4b0; /* fcmp/gt fr3,fr4 */
L8c60c4b0: take=!(*sr&1u);*stop=0x8c60c4b2u;r[14]=r[6];if(take)goto L8c60c4be;goto L8c60c4b4; /* bf/s 0x8c60c4be */
L8c60c4b4: *stop=0x8c60c4b4u;r[0]=0x8c60c71cu;goto L8c60c4b6; /* mova 0x8c60c71c,r0 */
L8c60c4b6: *stop=0x8c60c4b6u;fcopy(f,3u,4u);goto L8c60c4b8; /* fmov fr4,fr3 */
L8c60c4b8: *stop=0x8c60c4b8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c60c4ba; /* fmov.s @r0,fr2 */
L8c60c4ba: *stop=0x8c60c4bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60c4c0; /* bra 0x8c60c4c0 */
L8c60c4be: *stop=0x8c60c4beu;fcopy(f,3u,4u);goto L8c60c4c0; /* fmov fr4,fr3 */
L8c60c4c0: *stop=0x8c60c4c0u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c60c4c2; /* ftrc fr3,FPUL */
L8c60c4c2: *stop=0x8c60c4c2u;r[1]=*fpul;goto L8c60c4c4; /* sts FPUL,r1 */
L8c60c4c4: *stop=0x8c60c4c6u;r[0]=1u;goto L8c60c4e2; /* bra 0x8c60c4e2 */
L8c60c4c8: *stop=0x8c60c4c8u;r[7]=r[5];goto L8c60c4ca; /* mov r5,r7 */
L8c60c4ca: *stop=0x8c60c4cau;r[7]+=r[14];goto L8c60c4cc; /* add r14,r7 */
L8c60c4cc: *stop=0x8c60c4ccu;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c60c4ce; /* shlr r7 */
L8c60c4ce: *stop=0x8c60c4ceu;r[2]=r[7];goto L8c60c4d0; /* mov r7,r2 */
L8c60c4d0: *stop=0x8c60c4d0u;r[2]<<=2;goto L8c60c4d2; /* shll2 r2 */
L8c60c4d2: *stop=0x8c60c4d2u;r[2]<<=2;goto L8c60c4d4; /* shll2 r2 */
L8c60c4d4: *stop=0x8c60c4d4u;r[2]+=r[4];goto L8c60c4d6; /* add r4,r2 */
L8c60c4d6: *stop=0x8c60c4d6u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60c4d8; /* mov.l @r2,r3 */
L8c60c4d8: *stop=0x8c60c4d8u;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c60c4da; /* cmp/hs r3,r1 */
L8c60c4da: if(!(*sr&1u))goto L8c60c4e0;goto L8c60c4dc; /* bf 0x8c60c4e0 */
L8c60c4dc: *stop=0x8c60c4deu;r[5]=r[7];goto L8c60c4e2; /* bra 0x8c60c4e2 */
L8c60c4e0: *stop=0x8c60c4e0u;r[14]=r[7];goto L8c60c4e2; /* mov r7,r14 */
L8c60c4e2: *stop=0x8c60c4e2u;r[2]=r[14];goto L8c60c4e4; /* mov r14,r2 */
L8c60c4e4: *stop=0x8c60c4e4u;r[2]-=r[5];goto L8c60c4e6; /* sub r5,r2 */
L8c60c4e6: *stop=0x8c60c4e6u;*sr=(*sr&~1u)|(r[2]>r[0]);goto L8c60c4e8; /* cmp/hi r0,r2 */
L8c60c4e8: if((*sr&1u))goto L8c60c4c8;goto L8c60c4ea; /* bt 0x8c60c4c8 */
L8c60c4ea: *stop=0x8c60c4eau;r[3]=rd(m,0x8c60c720u,4);goto L8c60c4ec; /* mov.l 0x8c60c720,r3 */
L8c60c4ec: *stop=0x8c60c4ecu;r[14]=r[15];goto L8c60c4ee; /* mov r15,r14 */
L8c60c4ee: *stop=0x8c60c4eeu;r[14]+=4u;goto L8c60c4f0; /* add #0x4,r14 */
L8c60c4f0: *stop=0x8c60c4f0u;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c60c4f2; /* mov.l @r3,r2 */
L8c60c4f2: target=r[2];*pr=0x8c60c4f6u;*stop=0x8c60c4f4u;r[7]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c4f6; /* jsr @r2 */
L8c60c4f6: *stop=0x8c60c4f6u;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c60c4f8; /* mov.l @r14,r5 */
L8c60c4f8: *stop=0x8c60c4f8u;r[2]=rd(m,0x8c60c724u,4);goto L8c60c4fa; /* mov.l 0x8c60c724,r2 */
L8c60c4fa: *stop=0x8c60c4fau;if(!valid_ram(r[14]+8u,4u))return -3;r[7]=rd(m,r[14]+0x8u,4);goto L8c60c4fc; /* mov.l @(0x8,r14),r7 */
L8c60c4fc: *stop=0x8c60c4fcu;if(!valid_ram(r[14]+4u,4u))return -3;r[6]=rd(m,r[14]+0x4u,4);goto L8c60c4fe; /* mov.l @(0x4,r14),r6 */
L8c60c4fe: target=r[2];*pr=0x8c60c502u;*stop=0x8c60c500u;if(!valid_ram(r[13]+24u,4u))return -3;r[4]=rd(m,r[13]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c502; /* jsr @r2 */
L8c60c502: *stop=0x8c60c504u;goto L8c60c516; /* bra 0x8c60c516 */
L8c60c506: *stop=0x8c60c506u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);goto L8c60c508; /* mov.l @r15,r14 */
L8c60c508: *stop=0x8c60c508u;r[3]=rd(m,0x8c60c724u,4);goto L8c60c50a; /* mov.l 0x8c60c724,r3 */
L8c60c50a: *stop=0x8c60c50au;r[14]+=20u;goto L8c60c50c; /* add #0x14,r14 */
L8c60c50c: *stop=0x8c60c50cu;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c60c50e; /* mov.l @r14,r5 */
L8c60c50e: *stop=0x8c60c50eu;if(!valid_ram(r[14]+8u,4u))return -3;r[7]=rd(m,r[14]+0x8u,4);goto L8c60c510; /* mov.l @(0x8,r14),r7 */
L8c60c510: *stop=0x8c60c510u;if(!valid_ram(r[14]+4u,4u))return -3;r[6]=rd(m,r[14]+0x4u,4);goto L8c60c512; /* mov.l @(0x4,r14),r6 */
L8c60c512: target=r[3];*pr=0x8c60c516u;*stop=0x8c60c514u;if(!valid_ram(r[13]+24u,4u))return -3;r[4]=rd(m,r[13]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c516; /* jsr @r3 */
L8c60c516: *stop=0x8c60c516u;if(!valid_ram(r[13]+20u,4u))return -3;r[2]=rd(m,r[13]+0x14u,4);goto L8c60c518; /* mov.l @(0x14,r13),r2 */
L8c60c518: *stop=0x8c60c518u;r[2]+=1u;goto L8c60c51a; /* add #0x1,r2 */
L8c60c51a: *stop=0x8c60c51au;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[2],4);goto L8c60c51c; /* mov.l r2,@(0x14,r13) */
L8c60c51c: *stop=0x8c60c51cu;r[15]+=16u;goto L8c60c51e; /* add #0x10,r15 */
L8c60c51e: *stop=0x8c60c51eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60c520; /* lds.l @r15+,PR */
L8c60c520: *stop=0x8c60c520u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60c522; /* mov.l @r15+,r13 */
L8c60c522: *stop=0x8c60c522u;if(*pr!=entry_pr)return -3;*stop=0x8c60c524u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sample_rotation_yxz_8c60c526(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60c526;
L8c60c526: *stop=0x8c60c526u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60c528; /* mov.l r14,@-r15 */
L8c60c528: *stop=0x8c60c528u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60c52a; /* mov.l r13,@-r15 */
L8c60c52a: *stop=0x8c60c52au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60c52c; /* sts.l PR,@-r15 */
L8c60c52c: *stop=0x8c60c52cu;r[15]+=4294967280u;goto L8c60c52e; /* add #-0x10,r15 */
L8c60c52e: *stop=0x8c60c52eu;r[13]=rd(m,0x8c60c714u,4);goto L8c60c530; /* mov.l 0x8c60c714,r13 */
L8c60c530: *stop=0x8c60c530u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[4],4);goto L8c60c532; /* mov.l r4,@r15 */
L8c60c532: *stop=0x8c60c532u;if(!valid_ram(r[13]+20u,4u))return -3;r[5]=rd(m,r[13]+0x14u,4);goto L8c60c534; /* mov.l @(0x14,r13),r5 */
L8c60c534: *stop=0x8c60c534u;if(!valid_ram(r[13]+4u,4u))return -3;r[0]=rd(m,r[13]+0x4u,4);goto L8c60c536; /* mov.l @(0x4,r13),r0 */
L8c60c536: *stop=0x8c60c536u;r[5]<<=2;goto L8c60c538; /* shll2 r5 */
L8c60c538: *stop=0x8c60c538u;if(!valid_ram(r[0]+r[5],4u))return -3;r[4]=rd(m,r[5]+r[0],4);goto L8c60c53a; /* mov.l @(r0,r5),r4 */
L8c60c53a: *stop=0x8c60c53au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c60c53c; /* tst r4,r4 */
L8c60c53c: if((*sr&1u))goto L8c60c5a4;goto L8c60c53e; /* bt 0x8c60c5a4 */
L8c60c53e: *stop=0x8c60c53eu;if(!valid_ram(r[13]+8u,4u))return -3;r[0]=rd(m,r[13]+0x8u,4);goto L8c60c540; /* mov.l @(0x8,r13),r0 */
L8c60c540: *stop=0x8c60c540u;if(!valid_ram(r[0]+r[5],4u))return -3;r[6]=rd(m,r[5]+r[0],4);goto L8c60c542; /* mov.l @(r0,r5),r6 */
L8c60c542: *stop=0x8c60c542u;r[0]=12u;goto L8c60c544; /* mov #0xc,r0 */
L8c60c544: *stop=0x8c60c544u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto L8c60c546; /* fmov.s @(r0,r13),fr4 */
L8c60c546: *stop=0x8c60c546u;r[0]=0x8c60c718u;goto L8c60c548; /* mova 0x8c60c718,r0 */
L8c60c548: *stop=0x8c60c548u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c60c54a; /* fmov.s @r0,fr3 */
L8c60c54a: *stop=0x8c60c54au;r[5]=0u;goto L8c60c54c; /* mov #0x0,r5 */
L8c60c54c: *stop=0x8c60c54cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c60c54e; /* fcmp/gt fr3,fr4 */
L8c60c54e: take=!(*sr&1u);*stop=0x8c60c550u;r[14]=r[6];if(take)goto L8c60c55c;goto L8c60c552; /* bf/s 0x8c60c55c */
L8c60c552: *stop=0x8c60c552u;r[0]=0x8c60c71cu;goto L8c60c554; /* mova 0x8c60c71c,r0 */
L8c60c554: *stop=0x8c60c554u;fcopy(f,3u,4u);goto L8c60c556; /* fmov fr4,fr3 */
L8c60c556: *stop=0x8c60c556u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c60c558; /* fmov.s @r0,fr2 */
L8c60c558: *stop=0x8c60c55au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60c55e; /* bra 0x8c60c55e */
L8c60c55c: *stop=0x8c60c55cu;fcopy(f,3u,4u);goto L8c60c55e; /* fmov fr4,fr3 */
L8c60c55e: *stop=0x8c60c55eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c60c560; /* ftrc fr3,FPUL */
L8c60c560: *stop=0x8c60c560u;r[1]=*fpul;goto L8c60c562; /* sts FPUL,r1 */
L8c60c562: *stop=0x8c60c564u;r[0]=1u;goto L8c60c580; /* bra 0x8c60c580 */
L8c60c566: *stop=0x8c60c566u;r[7]=r[5];goto L8c60c568; /* mov r5,r7 */
L8c60c568: *stop=0x8c60c568u;r[7]+=r[14];goto L8c60c56a; /* add r14,r7 */
L8c60c56a: *stop=0x8c60c56au;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c60c56c; /* shlr r7 */
L8c60c56c: *stop=0x8c60c56cu;r[2]=r[7];goto L8c60c56e; /* mov r7,r2 */
L8c60c56e: *stop=0x8c60c56eu;r[2]<<=2;goto L8c60c570; /* shll2 r2 */
L8c60c570: *stop=0x8c60c570u;r[2]<<=2;goto L8c60c572; /* shll2 r2 */
L8c60c572: *stop=0x8c60c572u;r[2]+=r[4];goto L8c60c574; /* add r4,r2 */
L8c60c574: *stop=0x8c60c574u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60c576; /* mov.l @r2,r3 */
L8c60c576: *stop=0x8c60c576u;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c60c578; /* cmp/hs r3,r1 */
L8c60c578: if(!(*sr&1u))goto L8c60c57e;goto L8c60c57a; /* bf 0x8c60c57e */
L8c60c57a: *stop=0x8c60c57cu;r[5]=r[7];goto L8c60c580; /* bra 0x8c60c580 */
L8c60c57e: *stop=0x8c60c57eu;r[14]=r[7];goto L8c60c580; /* mov r7,r14 */
L8c60c580: *stop=0x8c60c580u;r[2]=r[14];goto L8c60c582; /* mov r14,r2 */
L8c60c582: *stop=0x8c60c582u;r[2]-=r[5];goto L8c60c584; /* sub r5,r2 */
L8c60c584: *stop=0x8c60c584u;*sr=(*sr&~1u)|(r[2]>r[0]);goto L8c60c586; /* cmp/hi r0,r2 */
L8c60c586: if((*sr&1u))goto L8c60c566;goto L8c60c588; /* bt 0x8c60c566 */
L8c60c588: *stop=0x8c60c588u;r[3]=rd(m,0x8c60c720u,4);goto L8c60c58a; /* mov.l 0x8c60c720,r3 */
L8c60c58a: *stop=0x8c60c58au;r[14]=r[15];goto L8c60c58c; /* mov r15,r14 */
L8c60c58c: *stop=0x8c60c58cu;r[14]+=4u;goto L8c60c58e; /* add #0x4,r14 */
L8c60c58e: *stop=0x8c60c58eu;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c60c590; /* mov.l @r3,r2 */
L8c60c590: target=r[2];*pr=0x8c60c594u;*stop=0x8c60c592u;r[7]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c594; /* jsr @r2 */
L8c60c594: *stop=0x8c60c594u;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c60c596; /* mov.l @r14,r5 */
L8c60c596: *stop=0x8c60c596u;r[2]=rd(m,0x8c60c728u,4);goto L8c60c598; /* mov.l 0x8c60c728,r2 */
L8c60c598: *stop=0x8c60c598u;if(!valid_ram(r[14]+8u,4u))return -3;r[7]=rd(m,r[14]+0x8u,4);goto L8c60c59a; /* mov.l @(0x8,r14),r7 */
L8c60c59a: *stop=0x8c60c59au;if(!valid_ram(r[14]+4u,4u))return -3;r[6]=rd(m,r[14]+0x4u,4);goto L8c60c59c; /* mov.l @(0x4,r14),r6 */
L8c60c59c: target=r[2];*pr=0x8c60c5a0u;*stop=0x8c60c59eu;if(!valid_ram(r[13]+24u,4u))return -3;r[4]=rd(m,r[13]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c5a0; /* jsr @r2 */
L8c60c5a0: *stop=0x8c60c5a2u;goto L8c60c5b4; /* bra 0x8c60c5b4 */
L8c60c5a4: *stop=0x8c60c5a4u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);goto L8c60c5a6; /* mov.l @r15,r14 */
L8c60c5a6: *stop=0x8c60c5a6u;r[3]=rd(m,0x8c60c728u,4);goto L8c60c5a8; /* mov.l 0x8c60c728,r3 */
L8c60c5a8: *stop=0x8c60c5a8u;r[14]+=20u;goto L8c60c5aa; /* add #0x14,r14 */
L8c60c5aa: *stop=0x8c60c5aau;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c60c5ac; /* mov.l @r14,r5 */
L8c60c5ac: *stop=0x8c60c5acu;if(!valid_ram(r[14]+8u,4u))return -3;r[7]=rd(m,r[14]+0x8u,4);goto L8c60c5ae; /* mov.l @(0x8,r14),r7 */
L8c60c5ae: *stop=0x8c60c5aeu;if(!valid_ram(r[14]+4u,4u))return -3;r[6]=rd(m,r[14]+0x4u,4);goto L8c60c5b0; /* mov.l @(0x4,r14),r6 */
L8c60c5b0: target=r[3];*pr=0x8c60c5b4u;*stop=0x8c60c5b2u;if(!valid_ram(r[13]+24u,4u))return -3;r[4]=rd(m,r[13]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c5b4; /* jsr @r3 */
L8c60c5b4: *stop=0x8c60c5b4u;if(!valid_ram(r[13]+20u,4u))return -3;r[2]=rd(m,r[13]+0x14u,4);goto L8c60c5b6; /* mov.l @(0x14,r13),r2 */
L8c60c5b6: *stop=0x8c60c5b6u;r[2]+=1u;goto L8c60c5b8; /* add #0x1,r2 */
L8c60c5b8: *stop=0x8c60c5b8u;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[2],4);goto L8c60c5ba; /* mov.l r2,@(0x14,r13) */
L8c60c5ba: *stop=0x8c60c5bau;r[15]+=16u;goto L8c60c5bc; /* add #0x10,r15 */
L8c60c5bc: *stop=0x8c60c5bcu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60c5be; /* lds.l @r15+,PR */
L8c60c5be: *stop=0x8c60c5beu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60c5c0; /* mov.l @r15+,r13 */
L8c60c5c0: *stop=0x8c60c5c0u;if(*pr!=entry_pr)return -3;*stop=0x8c60c5c2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_linear_vector_8c60beb0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60beb0;
L8c60beb0: *stop=0x8c60beb0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60beb2; /* mov.l r14,@-r15 */
L8c60beb2: *stop=0x8c60beb2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60beb4; /* mov.l r13,@-r15 */
L8c60beb4: *stop=0x8c60beb4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60beb6; /* sts.l PR,@-r15 */
L8c60beb6: *stop=0x8c60beb6u;r[15]+=4294967288u;goto L8c60beb8; /* add #-0x8,r15 */
L8c60beb8: *stop=0x8c60beb8u;r[2]=rd(m,0x8c60c174u,4);goto L8c60beba; /* mov.l 0x8c60c174,r2 */
L8c60beba: *stop=0x8c60bebau;r[13]=r[15];goto L8c60bebc; /* mov r15,r13 */
L8c60bebc: *stop=0x8c60bebcu;r[3]=rd(m,0x8c60c178u,4);goto L8c60bebe; /* mov.l 0x8c60c178,r3 */
L8c60bebe: *stop=0x8c60bebeu;r[1]=r[15];goto L8c60bec0; /* mov r15,r1 */
L8c60bec0: target=r[3];*pr=0x8c60bec4u;*stop=0x8c60bec2u;r[0]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60bec4; /* jsr @r3 */
L8c60bec4: *stop=0x8c60bec4u;r[14]=r[5];goto L8c60bec6; /* mov r5,r14 */
L8c60bec6: *stop=0x8c60bec6u;r[14]<<=2;goto L8c60bec8; /* shll2 r14 */
L8c60bec8: *stop=0x8c60bec8u;r[1]=r[5];goto L8c60beca; /* mov r5,r1 */
L8c60beca: *stop=0x8c60becau;r[3]=rd(m,0x8c60c17cu,4);goto L8c60becc; /* mov.l 0x8c60c17c,r3 */
L8c60becc: *stop=0x8c60beccu;r[14]<<=2;goto L8c60bece; /* shll2 r14 */
L8c60bece: *stop=0x8c60beceu;r[1]+=1u;goto L8c60bed0; /* add #0x1,r1 */
L8c60bed0: *stop=0x8c60bed0u;r[14]+=r[4];goto L8c60bed2; /* add r4,r14 */
L8c60bed2: target=r[3];*pr=0x8c60bed6u;*stop=0x8c60bed4u;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60bed6; /* jsr @r3 */
L8c60bed6: *stop=0x8c60bed6u;r[0]<<=2;goto L8c60bed8; /* shll2 r0 */
L8c60bed8: *stop=0x8c60bed8u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c60beda; /* mov.l @r14,r3 */
L8c60beda: *stop=0x8c60bedau;r[0]<<=2;goto L8c60bedc; /* shll2 r0 */
L8c60bedc: *stop=0x8c60bedcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c60bede; /* fldi0 fr2 */
L8c60bede: *stop=0x8c60bedeu;r[4]+=r[0];goto L8c60bee0; /* add r0,r4 */
L8c60bee0: *stop=0x8c60bee0u;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);goto L8c60bee2; /* mov.l @r4,r1 */
L8c60bee2: *stop=0x8c60bee2u;r[1]-=r[3];goto L8c60bee4; /* sub r3,r1 */
L8c60bee4: *stop=0x8c60bee4u;*fpul=r[1];goto L8c60bee6; /* lds r1,FPUL */
L8c60bee6: *stop=0x8c60bee6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60bee8; /* float FPUL,fr3 */
L8c60bee8: *stop=0x8c60bee8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c60beea; /* fcmp/gt fr3,fr2 */
L8c60beea: take=!(*sr&1u);*stop=0x8c60beecu;fcopy(f,4u,3u);if(take)goto L8c60bef8;goto L8c60beee; /* bf/s 0x8c60bef8 */
L8c60beee: *stop=0x8c60beeeu;r[0]=0x8c60c180u;goto L8c60bef0; /* mova 0x8c60c180,r0 */
L8c60bef0: *stop=0x8c60bef0u;fcopy(f,3u,4u);goto L8c60bef2; /* fmov fr4,fr3 */
L8c60bef2: *stop=0x8c60bef2u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto L8c60bef4; /* fmov.s @r0,fr1 */
L8c60bef4: *stop=0x8c60bef6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60befa; /* bra 0x8c60befa */
L8c60bef8: *stop=0x8c60bef8u;fcopy(f,3u,4u);goto L8c60befa; /* fmov fr4,fr3 */
L8c60befa: *stop=0x8c60befau;r[0]=4u;goto L8c60befc; /* mov #0x4,r0 */
L8c60befc: *stop=0x8c60befcu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[13]);goto L8c60befe; /* fmov fr3,@(r0,r13) */
L8c60befe: *stop=0x8c60befeu;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c60bf00; /* mov.l @r14,r5 */
L8c60bf00: *stop=0x8c60bf00u;*fpul=r[5];goto L8c60bf02; /* lds r5,FPUL */
L8c60bf02: *stop=0x8c60bf02u;*sr=(*sr&~1u)|((int32_t)r[5]>=0);goto L8c60bf04; /* cmp/pz r5 */
L8c60bf04: take=(*sr&1u);*stop=0x8c60bf06u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c60bf10;goto L8c60bf08; /* bt/s 0x8c60bf10 */
L8c60bf08: *stop=0x8c60bf08u;r[3]=rd(m,0x8c60c180u,4);goto L8c60bf0a; /* mov.l 0x8c60c180,r3 */
L8c60bf0a: *stop=0x8c60bf0au;*fpul=r[3];goto L8c60bf0c; /* lds r3,FPUL */
L8c60bf0c: *stop=0x8c60bf0cu;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c60bf0e; /* fsts FPUL,fr2 */
L8c60bf0e: *stop=0x8c60bf0eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60bf10; /* fadd fr2,fr3 */
L8c60bf10: *stop=0x8c60bf10u;r[2]=rd(m,0x8c60c184u,4);goto L8c60bf12; /* mov.l 0x8c60c184,r2 */
L8c60bf12: *stop=0x8c60bf12u;if(!valid_ram(r[4],4u))return -3;r[0]=rd(m,r[4],4);goto L8c60bf14; /* mov.l @r4,r0 */
L8c60bf14: *stop=0x8c60bf14u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[2]);goto L8c60bf16; /* fmov.s @r2,fr1 */
L8c60bf16: *stop=0x8c60bf16u;r[0]-=r[5];goto L8c60bf18; /* sub r5,r0 */
L8c60bf18: *stop=0x8c60bf18u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c60bf1a; /* fldi1 fr5 */
L8c60bf1a: *stop=0x8c60bf1au;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c60bf1c; /* cmp/pl r0 */
L8c60bf1c: *stop=0x8c60bf1cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c60bf1e; /* fsub fr3,fr1 */
L8c60bf1e: *stop=0x8c60bf1eu;r[0]=*sr&1u;goto L8c60bf20; /* movt r0 */
L8c60bf20: *stop=0x8c60bf20u;r[0]<<=2;goto L8c60bf22; /* shll2 r0 */
L8c60bf22: *stop=0x8c60bf22u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c60bf24; /* fmov.s @(r0,r13),fr3 */
L8c60bf24: *stop=0x8c60bf24u;r[0]=4u;goto L8c60bf26; /* mov #0x4,r0 */
L8c60bf26: *stop=0x8c60bf26u;fcopy(f,4u,1u);goto L8c60bf28; /* fmov fr1,fr4 */
L8c60bf28: *stop=0x8c60bf28u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c60bf2a; /* fdiv fr3,fr4 */
L8c60bf2a: *stop=0x8c60bf2au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c60bf2c; /* fmov.s @(r0,r4),fr3 */
L8c60bf2c: *stop=0x8c60bf2cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c60bf2e; /* fmov.s @(r0,r14),fr2 */
L8c60bf2e: *stop=0x8c60bf2eu;r[0]=8u;goto L8c60bf30; /* mov #0x8,r0 */
L8c60bf30: *stop=0x8c60bf30u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[4],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c60bf32; /* fsub fr4,fr5 */
L8c60bf32: *stop=0x8c60bf32u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60bf34; /* fmul fr4,fr3 */
L8c60bf34: *stop=0x8c60bf34u;fcopy(f,0u,5u);goto L8c60bf36; /* fmov fr5,fr0 */
L8c60bf36: *stop=0x8c60bf36u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60bf38; /* fmac fr0,fr2,fr3 */
L8c60bf38: *stop=0x8c60bf38u;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[7]);goto L8c60bf3a; /* fmov.s fr3,@r7 */
L8c60bf3a: *stop=0x8c60bf3au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c60bf3c; /* fmov.s @(r0,r4),fr3 */
L8c60bf3c: *stop=0x8c60bf3cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c60bf3e; /* fmov.s @(r0,r14),fr2 */
L8c60bf3e: *stop=0x8c60bf3eu;r[0]=4u;goto L8c60bf40; /* mov #0x4,r0 */
L8c60bf40: *stop=0x8c60bf40u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60bf42; /* fmul fr4,fr3 */
L8c60bf42: *stop=0x8c60bf42u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60bf44; /* fmac fr0,fr2,fr3 */
L8c60bf44: *stop=0x8c60bf44u;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[7]);goto L8c60bf46; /* fmov fr3,@(r0,r7) */
L8c60bf46: *stop=0x8c60bf46u;r[0]=12u;goto L8c60bf48; /* mov #0xc,r0 */
L8c60bf48: *stop=0x8c60bf48u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c60bf4a; /* fmov.s @(r0,r4),fr3 */
L8c60bf4a: *stop=0x8c60bf4au;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c60bf4c; /* fmov.s @(r0,r14),fr2 */
L8c60bf4c: *stop=0x8c60bf4cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60bf4e; /* fmul fr4,fr3 */
L8c60bf4e: *stop=0x8c60bf4eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60bf50; /* fmac fr0,fr2,fr3 */
L8c60bf50: *stop=0x8c60bf50u;r[0]=8u;goto L8c60bf52; /* mov #0x8,r0 */
L8c60bf52: *stop=0x8c60bf52u;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[7]);goto L8c60bf54; /* fmov fr3,@(r0,r7) */
L8c60bf54: *stop=0x8c60bf54u;r[15]+=8u;goto L8c60bf56; /* add #0x8,r15 */
L8c60bf56: *stop=0x8c60bf56u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60bf58; /* lds.l @r15+,PR */
L8c60bf58: *stop=0x8c60bf58u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60bf5a; /* mov.l @r15+,r13 */
L8c60bf5a: *stop=0x8c60bf5au;if(*pr!=entry_pr)return -3;*stop=0x8c60bf5cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_linear_angles_8c60bf5e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60bf5e;
L8c60bf5e: *stop=0x8c60bf5eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60bf60; /* mov.l r14,@-r15 */
L8c60bf60: *stop=0x8c60bf60u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60bf62; /* mov.l r13,@-r15 */
L8c60bf62: *stop=0x8c60bf62u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60bf64; /* sts.l PR,@-r15 */
L8c60bf64: *stop=0x8c60bf64u;r[15]+=4294967288u;goto L8c60bf66; /* add #-0x8,r15 */
L8c60bf66: *stop=0x8c60bf66u;r[2]=rd(m,0x8c60c188u,4);goto L8c60bf68; /* mov.l 0x8c60c188,r2 */
L8c60bf68: *stop=0x8c60bf68u;r[13]=r[15];goto L8c60bf6a; /* mov r15,r13 */
L8c60bf6a: *stop=0x8c60bf6au;r[3]=rd(m,0x8c60c178u,4);goto L8c60bf6c; /* mov.l 0x8c60c178,r3 */
L8c60bf6c: *stop=0x8c60bf6cu;r[1]=r[15];goto L8c60bf6e; /* mov r15,r1 */
L8c60bf6e: target=r[3];*pr=0x8c60bf72u;*stop=0x8c60bf70u;r[0]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60bf72; /* jsr @r3 */
L8c60bf72: *stop=0x8c60bf72u;r[14]=r[5];goto L8c60bf74; /* mov r5,r14 */
L8c60bf74: *stop=0x8c60bf74u;r[14]<<=2;goto L8c60bf76; /* shll2 r14 */
L8c60bf76: *stop=0x8c60bf76u;r[1]=r[5];goto L8c60bf78; /* mov r5,r1 */
L8c60bf78: *stop=0x8c60bf78u;r[3]=rd(m,0x8c60c17cu,4);goto L8c60bf7a; /* mov.l 0x8c60c17c,r3 */
L8c60bf7a: *stop=0x8c60bf7au;r[14]<<=2;goto L8c60bf7c; /* shll2 r14 */
L8c60bf7c: *stop=0x8c60bf7cu;r[1]+=1u;goto L8c60bf7e; /* add #0x1,r1 */
L8c60bf7e: *stop=0x8c60bf7eu;r[14]+=r[4];goto L8c60bf80; /* add r4,r14 */
L8c60bf80: target=r[3];*pr=0x8c60bf84u;*stop=0x8c60bf82u;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60bf84; /* jsr @r3 */
L8c60bf84: *stop=0x8c60bf84u;r[0]<<=2;goto L8c60bf86; /* shll2 r0 */
L8c60bf86: *stop=0x8c60bf86u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c60bf88; /* mov.l @r14,r3 */
L8c60bf88: *stop=0x8c60bf88u;r[0]<<=2;goto L8c60bf8a; /* shll2 r0 */
L8c60bf8a: *stop=0x8c60bf8au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c60bf8c; /* fldi0 fr2 */
L8c60bf8c: *stop=0x8c60bf8cu;r[4]+=r[0];goto L8c60bf8e; /* add r0,r4 */
L8c60bf8e: *stop=0x8c60bf8eu;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);goto L8c60bf90; /* mov.l @r4,r1 */
L8c60bf90: *stop=0x8c60bf90u;r[1]-=r[3];goto L8c60bf92; /* sub r3,r1 */
L8c60bf92: *stop=0x8c60bf92u;*fpul=r[1];goto L8c60bf94; /* lds r1,FPUL */
L8c60bf94: *stop=0x8c60bf94u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60bf96; /* float FPUL,fr3 */
L8c60bf96: *stop=0x8c60bf96u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c60bf98; /* fcmp/gt fr3,fr2 */
L8c60bf98: take=!(*sr&1u);*stop=0x8c60bf9au;fcopy(f,4u,3u);if(take)goto L8c60bfa6;goto L8c60bf9c; /* bf/s 0x8c60bfa6 */
L8c60bf9c: *stop=0x8c60bf9cu;r[0]=0x8c60c180u;goto L8c60bf9e; /* mova 0x8c60c180,r0 */
L8c60bf9e: *stop=0x8c60bf9eu;fcopy(f,3u,4u);goto L8c60bfa0; /* fmov fr4,fr3 */
L8c60bfa0: *stop=0x8c60bfa0u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto L8c60bfa2; /* fmov.s @r0,fr1 */
L8c60bfa2: *stop=0x8c60bfa4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60bfa8; /* bra 0x8c60bfa8 */
L8c60bfa6: *stop=0x8c60bfa6u;fcopy(f,3u,4u);goto L8c60bfa8; /* fmov fr4,fr3 */
L8c60bfa8: *stop=0x8c60bfa8u;r[0]=4u;goto L8c60bfaa; /* mov #0x4,r0 */
L8c60bfaa: *stop=0x8c60bfaau;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[13]);goto L8c60bfac; /* fmov fr3,@(r0,r13) */
L8c60bfac: *stop=0x8c60bfacu;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c60bfae; /* mov.l @r14,r5 */
L8c60bfae: *stop=0x8c60bfaeu;*fpul=r[5];goto L8c60bfb0; /* lds r5,FPUL */
L8c60bfb0: *stop=0x8c60bfb0u;*sr=(*sr&~1u)|((int32_t)r[5]>=0);goto L8c60bfb2; /* cmp/pz r5 */
L8c60bfb2: take=(*sr&1u);*stop=0x8c60bfb4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c60bfbe;goto L8c60bfb6; /* bt/s 0x8c60bfbe */
L8c60bfb6: *stop=0x8c60bfb6u;r[3]=rd(m,0x8c60c180u,4);goto L8c60bfb8; /* mov.l 0x8c60c180,r3 */
L8c60bfb8: *stop=0x8c60bfb8u;*fpul=r[3];goto L8c60bfba; /* lds r3,FPUL */
L8c60bfba: *stop=0x8c60bfbau;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c60bfbc; /* fsts FPUL,fr2 */
L8c60bfbc: *stop=0x8c60bfbcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60bfbe; /* fadd fr2,fr3 */
L8c60bfbe: *stop=0x8c60bfbeu;r[2]=rd(m,0x8c60c184u,4);goto L8c60bfc0; /* mov.l 0x8c60c184,r2 */
L8c60bfc0: *stop=0x8c60bfc0u;if(!valid_ram(r[4],4u))return -3;r[0]=rd(m,r[4],4);goto L8c60bfc2; /* mov.l @r4,r0 */
L8c60bfc2: *stop=0x8c60bfc2u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[2]);goto L8c60bfc4; /* fmov.s @r2,fr1 */
L8c60bfc4: *stop=0x8c60bfc4u;r[0]-=r[5];goto L8c60bfc6; /* sub r5,r0 */
L8c60bfc6: *stop=0x8c60bfc6u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c60bfc8; /* mov.l @(0x4,r14),r3 */
L8c60bfc8: *stop=0x8c60bfc8u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c60bfca; /* cmp/pl r0 */
L8c60bfca: *stop=0x8c60bfcau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c60bfcc; /* fsub fr3,fr1 */
L8c60bfcc: *stop=0x8c60bfccu;r[0]=*sr&1u;goto L8c60bfce; /* movt r0 */
L8c60bfce: *stop=0x8c60bfceu;*fpul=r[3];goto L8c60bfd0; /* lds r3,FPUL */
L8c60bfd0: *stop=0x8c60bfd0u;r[0]<<=2;goto L8c60bfd2; /* shll2 r0 */
L8c60bfd2: *stop=0x8c60bfd2u;if(!valid_ram(r[4]+4u,4u))return -3;r[1]=rd(m,r[4]+0x4u,4);goto L8c60bfd4; /* mov.l @(0x4,r4),r1 */
L8c60bfd4: *stop=0x8c60bfd4u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c60bfd6; /* fmov.s @(r0,r13),fr3 */
L8c60bfd6: *stop=0x8c60bfd6u;fcopy(f,4u,1u);goto L8c60bfd8; /* fmov fr1,fr4 */
L8c60bfd8: *stop=0x8c60bfd8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c60bfda; /* fdiv fr3,fr4 */
L8c60bfda: *stop=0x8c60bfdau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c60bfdc; /* fldi1 fr5 */
L8c60bfdc: *stop=0x8c60bfdcu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60bfde; /* float FPUL,fr3 */
L8c60bfde: *stop=0x8c60bfdeu;*fpul=r[1];goto L8c60bfe0; /* lds r1,FPUL */
L8c60bfe0: *stop=0x8c60bfe0u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c60bfe2; /* float FPUL,fr2 */
L8c60bfe2: *stop=0x8c60bfe2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[4],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c60bfe4; /* fsub fr4,fr5 */
L8c60bfe4: *stop=0x8c60bfe4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60bfe6; /* fmul fr4,fr2 */
L8c60bfe6: *stop=0x8c60bfe6u;fcopy(f,0u,5u);goto L8c60bfe8; /* fmov fr5,fr0 */
L8c60bfe8: *stop=0x8c60bfe8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60bfea; /* fmac fr0,fr3,fr2 */
L8c60bfea: *stop=0x8c60bfeau;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c60bfec; /* ftrc fr2,FPUL */
L8c60bfec: *stop=0x8c60bfecu;r[3]=*fpul;goto L8c60bfee; /* sts FPUL,r3 */
L8c60bfee: *stop=0x8c60bfeeu;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[3],4);goto L8c60bff0; /* mov.l r3,@r7 */
L8c60bff0: *stop=0x8c60bff0u;if(!valid_ram(r[14]+8u,4u))return -3;r[1]=rd(m,r[14]+0x8u,4);goto L8c60bff2; /* mov.l @(0x8,r14),r1 */
L8c60bff2: *stop=0x8c60bff2u;if(!valid_ram(r[4]+8u,4u))return -3;r[3]=rd(m,r[4]+0x8u,4);goto L8c60bff4; /* mov.l @(0x8,r4),r3 */
L8c60bff4: *stop=0x8c60bff4u;*fpul=r[1];goto L8c60bff6; /* lds r1,FPUL */
L8c60bff6: *stop=0x8c60bff6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60bff8; /* float FPUL,fr3 */
L8c60bff8: *stop=0x8c60bff8u;*fpul=r[3];goto L8c60bffa; /* lds r3,FPUL */
L8c60bffa: *stop=0x8c60bffau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c60bffc; /* float FPUL,fr2 */
L8c60bffc: *stop=0x8c60bffcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60bffe; /* fmul fr4,fr2 */
L8c60bffe: *stop=0x8c60bffeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60c000; /* fmac fr0,fr3,fr2 */
L8c60c000: *stop=0x8c60c000u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c60c002; /* ftrc fr2,FPUL */
L8c60c002: *stop=0x8c60c002u;r[3]=*fpul;goto L8c60c004; /* sts FPUL,r3 */
L8c60c004: *stop=0x8c60c004u;if(!valid_ram(r[7]+4u,4u))return -3;wr(m,r[7]+0x4u,r[3],4);goto L8c60c006; /* mov.l r3,@(0x4,r7) */
L8c60c006: *stop=0x8c60c006u;if(!valid_ram(r[14]+12u,4u))return -3;r[1]=rd(m,r[14]+0xcu,4);goto L8c60c008; /* mov.l @(0xc,r14),r1 */
L8c60c008: *stop=0x8c60c008u;if(!valid_ram(r[4]+12u,4u))return -3;r[3]=rd(m,r[4]+0xcu,4);goto L8c60c00a; /* mov.l @(0xc,r4),r3 */
L8c60c00a: *stop=0x8c60c00au;*fpul=r[1];goto L8c60c00c; /* lds r1,FPUL */
L8c60c00c: *stop=0x8c60c00cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60c00e; /* float FPUL,fr3 */
L8c60c00e: *stop=0x8c60c00eu;*fpul=r[3];goto L8c60c010; /* lds r3,FPUL */
L8c60c010: *stop=0x8c60c010u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c60c012; /* float FPUL,fr2 */
L8c60c012: *stop=0x8c60c012u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60c014; /* fmul fr4,fr2 */
L8c60c014: *stop=0x8c60c014u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60c016; /* fmac fr0,fr3,fr2 */
L8c60c016: *stop=0x8c60c016u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c60c018; /* ftrc fr2,FPUL */
L8c60c018: *stop=0x8c60c018u;r[3]=*fpul;goto L8c60c01a; /* sts FPUL,r3 */
L8c60c01a: *stop=0x8c60c01au;if(!valid_ram(r[7]+8u,4u))return -3;wr(m,r[7]+0x8u,r[3],4);goto L8c60c01c; /* mov.l r3,@(0x8,r7) */
L8c60c01c: *stop=0x8c60c01cu;r[15]+=8u;goto L8c60c01e; /* add #0x8,r15 */
L8c60c01e: *stop=0x8c60c01eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60c020; /* lds.l @r15+,PR */
L8c60c020: *stop=0x8c60c020u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60c022; /* mov.l @r15+,r13 */
L8c60c022: *stop=0x8c60c022u;if(*pr!=entry_pr)return -3;*stop=0x8c60c024u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_cubic_vector_8c60c026(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60c026;
L8c60c026: *stop=0x8c60c026u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60c028; /* mov.l r14,@-r15 */
L8c60c028: *stop=0x8c60c028u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60c02a; /* mov.l r13,@-r15 */
L8c60c02a: *stop=0x8c60c02au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60c02c; /* mov.l r12,@-r15 */
L8c60c02c: *stop=0x8c60c02cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c60c02e; /* mov.l r11,@-r15 */
L8c60c02e: *stop=0x8c60c02eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60c030; /* sts.l PR,@-r15 */
L8c60c030: *stop=0x8c60c030u;r[15]+=4294967288u;goto L8c60c032; /* add #-0x8,r15 */
L8c60c032: *stop=0x8c60c032u;r[2]=rd(m,0x8c60c18cu,4);goto L8c60c034; /* mov.l 0x8c60c18c,r2 */
L8c60c034: *stop=0x8c60c034u;r[11]=r[15];goto L8c60c036; /* mov r15,r11 */
L8c60c036: *stop=0x8c60c036u;r[3]=rd(m,0x8c60c178u,4);goto L8c60c038; /* mov.l 0x8c60c178,r3 */
L8c60c038: *stop=0x8c60c038u;r[1]=r[15];goto L8c60c03a; /* mov r15,r1 */
L8c60c03a: target=r[3];*pr=0x8c60c03eu;*stop=0x8c60c03cu;r[0]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c03e; /* jsr @r3 */
L8c60c03e: *stop=0x8c60c03eu;r[1]=r[5];goto L8c60c040; /* mov r5,r1 */
L8c60c040: *stop=0x8c60c040u;r[3]=rd(m,0x8c60c17cu,4);goto L8c60c042; /* mov.l 0x8c60c17c,r3 */
L8c60c042: *stop=0x8c60c042u;r[1]+=r[6];goto L8c60c044; /* add r6,r1 */
L8c60c044: *stop=0x8c60c044u;r[1]+=4294967295u;goto L8c60c046; /* add #-0x1,r1 */
L8c60c046: target=r[3];*pr=0x8c60c04au;*stop=0x8c60c048u;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c04a; /* jsr @r3 */
L8c60c04a: *stop=0x8c60c04au;r[13]=r[5];goto L8c60c04c; /* mov r5,r13 */
L8c60c04c: *stop=0x8c60c04cu;r[12]=r[0];goto L8c60c04e; /* mov r0,r12 */
L8c60c04e: *stop=0x8c60c04eu;r[13]<<=2;goto L8c60c050; /* shll2 r13 */
L8c60c050: *stop=0x8c60c050u;r[1]=r[5];goto L8c60c052; /* mov r5,r1 */
L8c60c052: *stop=0x8c60c052u;r[12]<<=2;goto L8c60c054; /* shll2 r12 */
L8c60c054: *stop=0x8c60c054u;r[3]=rd(m,0x8c60c17cu,4);goto L8c60c056; /* mov.l 0x8c60c17c,r3 */
L8c60c056: *stop=0x8c60c056u;r[13]<<=2;goto L8c60c058; /* shll2 r13 */
L8c60c058: *stop=0x8c60c058u;r[12]<<=2;goto L8c60c05a; /* shll2 r12 */
L8c60c05a: *stop=0x8c60c05au;r[12]+=r[4];goto L8c60c05c; /* add r4,r12 */
L8c60c05c: *stop=0x8c60c05cu;r[1]+=1u;goto L8c60c05e; /* add #0x1,r1 */
L8c60c05e: *stop=0x8c60c05eu;r[13]+=r[4];goto L8c60c060; /* add r4,r13 */
L8c60c060: target=r[3];*pr=0x8c60c064u;*stop=0x8c60c062u;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c064; /* jsr @r3 */
L8c60c064: *stop=0x8c60c064u;r[14]=r[0];goto L8c60c066; /* mov r0,r14 */
L8c60c066: *stop=0x8c60c066u;r[14]<<=2;goto L8c60c068; /* shll2 r14 */
L8c60c068: *stop=0x8c60c068u;r[1]=r[5];goto L8c60c06a; /* mov r5,r1 */
L8c60c06a: *stop=0x8c60c06au;r[3]=rd(m,0x8c60c17cu,4);goto L8c60c06c; /* mov.l 0x8c60c17c,r3 */
L8c60c06c: *stop=0x8c60c06cu;r[14]<<=2;goto L8c60c06e; /* shll2 r14 */
L8c60c06e: *stop=0x8c60c06eu;r[1]+=2u;goto L8c60c070; /* add #0x2,r1 */
L8c60c070: *stop=0x8c60c070u;r[14]+=r[4];goto L8c60c072; /* add r4,r14 */
L8c60c072: target=r[3];*pr=0x8c60c076u;*stop=0x8c60c074u;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c076; /* jsr @r3 */
L8c60c076: *stop=0x8c60c076u;if(!valid_ram(r[13],4u))return -3;r[3]=rd(m,r[13],4);goto L8c60c078; /* mov.l @r13,r3 */
L8c60c078: *stop=0x8c60c078u;r[0]<<=2;goto L8c60c07a; /* shll2 r0 */
L8c60c07a: *stop=0x8c60c07au;if(!valid_ram(r[14],4u))return -3;r[1]=rd(m,r[14],4);goto L8c60c07c; /* mov.l @r14,r1 */
L8c60c07c: *stop=0x8c60c07cu;r[0]<<=2;goto L8c60c07e; /* shll2 r0 */
L8c60c07e: *stop=0x8c60c07eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c60c080; /* fldi0 fr2 */
L8c60c080: *stop=0x8c60c080u;r[4]+=r[0];goto L8c60c082; /* add r0,r4 */
L8c60c082: *stop=0x8c60c082u;r[1]-=r[3];goto L8c60c084; /* sub r3,r1 */
L8c60c084: *stop=0x8c60c084u;*fpul=r[1];goto L8c60c086; /* lds r1,FPUL */
L8c60c086: *stop=0x8c60c086u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60c088; /* float FPUL,fr3 */
L8c60c088: *stop=0x8c60c088u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c60c08a; /* fcmp/gt fr3,fr2 */
L8c60c08a: take=!(*sr&1u);*stop=0x8c60c08cu;fcopy(f,4u,3u);if(take)goto L8c60c098;goto L8c60c08e; /* bf/s 0x8c60c098 */
L8c60c08e: *stop=0x8c60c08eu;r[0]=0x8c60c180u;goto L8c60c090; /* mova 0x8c60c180,r0 */
L8c60c090: *stop=0x8c60c090u;fcopy(f,3u,4u);goto L8c60c092; /* fmov fr4,fr3 */
L8c60c092: *stop=0x8c60c092u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto L8c60c094; /* fmov.s @r0,fr1 */
L8c60c094: *stop=0x8c60c096u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60c09a; /* bra 0x8c60c09a */
L8c60c098: *stop=0x8c60c098u;fcopy(f,3u,4u);goto L8c60c09a; /* fmov fr4,fr3 */
L8c60c09a: *stop=0x8c60c09au;r[0]=4u;goto L8c60c09c; /* mov #0x4,r0 */
L8c60c09c: *stop=0x8c60c09cu;if(!valid_ram(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[11]);goto L8c60c09e; /* fmov fr3,@(r0,r11) */
L8c60c09e: *stop=0x8c60c09eu;if(!valid_ram(r[13],4u))return -3;r[5]=rd(m,r[13],4);goto L8c60c0a0; /* mov.l @r13,r5 */
L8c60c0a0: *stop=0x8c60c0a0u;*fpul=r[5];goto L8c60c0a2; /* lds r5,FPUL */
L8c60c0a2: *stop=0x8c60c0a2u;*sr=(*sr&~1u)|((int32_t)r[5]>=0);goto L8c60c0a4; /* cmp/pz r5 */
L8c60c0a4: take=(*sr&1u);*stop=0x8c60c0a6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c60c0b0;goto L8c60c0a8; /* bt/s 0x8c60c0b0 */
L8c60c0a8: *stop=0x8c60c0a8u;r[3]=rd(m,0x8c60c180u,4);goto L8c60c0aa; /* mov.l 0x8c60c180,r3 */
L8c60c0aa: *stop=0x8c60c0aau;*fpul=r[3];goto L8c60c0ac; /* lds r3,FPUL */
L8c60c0ac: *stop=0x8c60c0acu;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c60c0ae; /* fsts FPUL,fr2 */
L8c60c0ae: *stop=0x8c60c0aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60c0b0; /* fadd fr2,fr3 */
L8c60c0b0: *stop=0x8c60c0b0u;r[2]=rd(m,0x8c60c184u,4);goto L8c60c0b2; /* mov.l 0x8c60c184,r2 */
L8c60c0b2: *stop=0x8c60c0b2u;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c60c0b4; /* mov.l @r14,r0 */
L8c60c0b4: *stop=0x8c60c0b4u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[2]);goto L8c60c0b6; /* fmov.s @r2,fr1 */
L8c60c0b6: *stop=0x8c60c0b6u;r[0]-=r[5];goto L8c60c0b8; /* sub r5,r0 */
L8c60c0b8: *stop=0x8c60c0b8u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c60c0ba; /* cmp/pl r0 */
L8c60c0ba: *stop=0x8c60c0bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c60c0bc; /* fsub fr3,fr1 */
L8c60c0bc: *stop=0x8c60c0bcu;r[0]=*sr&1u;goto L8c60c0be; /* movt r0 */
L8c60c0be: *stop=0x8c60c0beu;r[0]<<=2;goto L8c60c0c0; /* shll2 r0 */
L8c60c0c0: *stop=0x8c60c0c0u;if(!valid_ram(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[11]);goto L8c60c0c2; /* fmov.s @(r0,r11),fr3 */
L8c60c0c2: *stop=0x8c60c0c2u;r[0]=0x8c60c190u;goto L8c60c0c4; /* mova 0x8c60c190,r0 */
L8c60c0c4: *stop=0x8c60c0c4u;fcopy(f,4u,1u);goto L8c60c0c6; /* fmov fr1,fr4 */
L8c60c0c6: *stop=0x8c60c0c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c60c0c8; /* fdiv fr3,fr4 */
L8c60c0c8: *stop=0x8c60c0c8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c60c0ca; /* fldi1 fr3 */
L8c60c0ca: *stop=0x8c60c0cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60c0cc; /* fadd fr3,fr3 */
L8c60c0cc: *stop=0x8c60c0ccu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,10u,r[0]);goto L8c60c0ce; /* fmov.s @r0,fr10 */
L8c60c0ce: *stop=0x8c60c0ceu;r[0]=0x8c60c194u;goto L8c60c0d0; /* mova 0x8c60c194,r0 */
L8c60c0d0: *stop=0x8c60c0d0u;fcopy(f,5u,4u);goto L8c60c0d2; /* fmov fr4,fr5 */
L8c60c0d2: *stop=0x8c60c0d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[4],&f->fpscr,&fr[5]);if(status)return status;}goto L8c60c0d4; /* fmul fr4,fr5 */
L8c60c0d4: *stop=0x8c60c0d4u;fcopy(f,7u,4u);goto L8c60c0d6; /* fmov fr4,fr7 */
L8c60c0d6: *stop=0x8c60c0d6u;fcopy(f,8u,5u);goto L8c60c0d8; /* fmov fr5,fr8 */
L8c60c0d8: *stop=0x8c60c0d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c60c0da; /* fmul fr3,fr8 */
L8c60c0da: *stop=0x8c60c0dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c60c0dc; /* fmul fr5,fr7 */
L8c60c0dc: *stop=0x8c60c0dcu;fcopy(f,2u,8u);goto L8c60c0de; /* fmov fr8,fr2 */
L8c60c0de: *stop=0x8c60c0deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60c0e0; /* fsub fr4,fr2 */
L8c60c0e0: *stop=0x8c60c0e0u;fcopy(f,11u,7u);goto L8c60c0e2; /* fmov fr7,fr11 */
L8c60c0e2: *stop=0x8c60c0e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[7],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60c0e4; /* fsub fr7,fr2 */
L8c60c0e4: *stop=0x8c60c0e4u;fcopy(f,9u,2u);goto L8c60c0e6; /* fmov fr2,fr9 */
L8c60c0e6: *stop=0x8c60c0e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[10],&f->fpscr,&fr[9]);if(status)return status;}goto L8c60c0e8; /* fmul fr10,fr9 */
L8c60c0e8: *stop=0x8c60c0e8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c60c0ea; /* fmov.s @r0,fr2 */
L8c60c0ea: *stop=0x8c60c0eau;r[0]=0x8c60c198u;goto L8c60c0ec; /* mova 0x8c60c198,r0 */
L8c60c0ec: *stop=0x8c60c0ecu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto L8c60c0ee; /* fmov.s @r0,fr1 */
L8c60c0ee: *stop=0x8c60c0eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[2],&f->fpscr,&fr[11]);if(status)return status;}goto L8c60c0f0; /* fmul fr2,fr11 */
L8c60c0f0: *stop=0x8c60c0f0u;fcopy(f,0u,5u);goto L8c60c0f2; /* fmov fr5,fr0 */
L8c60c0f2: *stop=0x8c60c0f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[1],&f->fpscr,&fr[0]);if(status)return status;}goto L8c60c0f4; /* fmul fr1,fr0 */
L8c60c0f4: *stop=0x8c60c0f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c60c0f6; /* fmul fr10,fr4 */
L8c60c0f6: *stop=0x8c60c0f6u;fcopy(f,3u,11u);goto L8c60c0f8; /* fmov fr11,fr3 */
L8c60c0f8: *stop=0x8c60c0f8u;r[0]=4u;goto L8c60c0fa; /* mov #0x4,r0 */
L8c60c0fa: *stop=0x8c60c0fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[5],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c60c0fc; /* fsub fr5,fr7 */
L8c60c0fc: *stop=0x8c60c0fcu;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[12]);goto L8c60c0fe; /* fmov.s @(r0,r12),fr2 */
L8c60c0fe: *stop=0x8c60c0feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[0],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c60c100; /* fsub fr0,fr3 */
L8c60c100: *stop=0x8c60c100u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;goto L8c60c102; /* fldi1 fr0 */
L8c60c102: *stop=0x8c60c102u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[11],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c60c104; /* fsub fr11,fr4 */
L8c60c104: *stop=0x8c60c104u;fcopy(f,6u,3u);goto L8c60c106; /* fmov fr3,fr6 */
L8c60c106: *stop=0x8c60c106u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[0],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c60c108; /* fadd fr0,fr6 */
L8c60c108: *stop=0x8c60c108u;fcopy(f,3u,8u);goto L8c60c10a; /* fmov fr8,fr3 */
L8c60c10a: *stop=0x8c60c10au;fcopy(f,8u,4u);goto L8c60c10c; /* fmov fr4,fr8 */
L8c60c10c: *stop=0x8c60c10cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c60c10e; /* fadd fr3,fr8 */
L8c60c10e: *stop=0x8c60c10eu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[13]);goto L8c60c110; /* fmov.s @(r0,r13),fr0 */
L8c60c110: *stop=0x8c60c110u;fcopy(f,3u,6u);goto L8c60c112; /* fmov fr6,fr3 */
L8c60c112: *stop=0x8c60c112u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c114; /* fmul fr0,fr3 */
L8c60c114: *stop=0x8c60c114u;fcopy(f,0u,9u);goto L8c60c116; /* fmov fr9,fr0 */
L8c60c116: *stop=0x8c60c116u;fcopy(f,4u,7u);goto L8c60c118; /* fmov fr7,fr4 */
L8c60c118: *stop=0x8c60c118u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c60c11a; /* fmul fr10,fr4 */
L8c60c11a: *stop=0x8c60c11au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c11c; /* fmac fr0,fr2,fr3 */
L8c60c11c: *stop=0x8c60c11cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c60c11e; /* fmov.s @(r0,r14),fr2 */
L8c60c11e: *stop=0x8c60c11eu;fcopy(f,0u,8u);goto L8c60c120; /* fmov fr8,fr0 */
L8c60c120: *stop=0x8c60c120u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c122; /* fmac fr0,fr2,fr3 */
L8c60c122: *stop=0x8c60c122u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c60c124; /* fmov.s @(r0,r4),fr2 */
L8c60c124: *stop=0x8c60c124u;fcopy(f,0u,4u);goto L8c60c126; /* fmov fr4,fr0 */
L8c60c126: *stop=0x8c60c126u;r[0]=8u;goto L8c60c128; /* mov #0x8,r0 */
L8c60c128: *stop=0x8c60c128u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c12a; /* fmac fr0,fr2,fr3 */
L8c60c12a: *stop=0x8c60c12au;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[7]);goto L8c60c12c; /* fmov.s fr3,@r7 */
L8c60c12c: *stop=0x8c60c12cu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[13]);goto L8c60c12e; /* fmov.s @(r0,r13),fr0 */
L8c60c12e: *stop=0x8c60c12eu;fcopy(f,3u,6u);goto L8c60c130; /* fmov fr6,fr3 */
L8c60c130: *stop=0x8c60c130u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c132; /* fmul fr0,fr3 */
L8c60c132: *stop=0x8c60c132u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[12]);goto L8c60c134; /* fmov.s @(r0,r12),fr2 */
L8c60c134: *stop=0x8c60c134u;fcopy(f,0u,9u);goto L8c60c136; /* fmov fr9,fr0 */
L8c60c136: *stop=0x8c60c136u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c138; /* fmac fr0,fr2,fr3 */
L8c60c138: *stop=0x8c60c138u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c60c13a; /* fmov.s @(r0,r14),fr2 */
L8c60c13a: *stop=0x8c60c13au;fcopy(f,0u,8u);goto L8c60c13c; /* fmov fr8,fr0 */
L8c60c13c: *stop=0x8c60c13cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c13e; /* fmac fr0,fr2,fr3 */
L8c60c13e: *stop=0x8c60c13eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c60c140; /* fmov.s @(r0,r4),fr2 */
L8c60c140: *stop=0x8c60c140u;fcopy(f,0u,4u);goto L8c60c142; /* fmov fr4,fr0 */
L8c60c142: *stop=0x8c60c142u;r[0]=4u;goto L8c60c144; /* mov #0x4,r0 */
L8c60c144: *stop=0x8c60c144u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c146; /* fmac fr0,fr2,fr3 */
L8c60c146: *stop=0x8c60c146u;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[7]);goto L8c60c148; /* fmov fr3,@(r0,r7) */
L8c60c148: *stop=0x8c60c148u;r[0]=12u;goto L8c60c14a; /* mov #0xc,r0 */
L8c60c14a: *stop=0x8c60c14au;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[13]);goto L8c60c14c; /* fmov.s @(r0,r13),fr0 */
L8c60c14c: *stop=0x8c60c14cu;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[12]);goto L8c60c14e; /* fmov.s @(r0,r12),fr3 */
L8c60c14e: *stop=0x8c60c14eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c60c150; /* fmul fr0,fr6 */
L8c60c150: *stop=0x8c60c150u;fcopy(f,0u,9u);goto L8c60c152; /* fmov fr9,fr0 */
L8c60c152: *stop=0x8c60c152u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c60c154; /* fmov.s @(r0,r14),fr2 */
L8c60c154: *stop=0x8c60c154u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[6],&f->fpscr,&fr[6]);if(status)return status;}goto L8c60c156; /* fmac fr0,fr3,fr6 */
L8c60c156: *stop=0x8c60c156u;fcopy(f,0u,8u);goto L8c60c158; /* fmov fr8,fr0 */
L8c60c158: *stop=0x8c60c158u;fcopy(f,3u,6u);goto L8c60c15a; /* fmov fr6,fr3 */
L8c60c15a: *stop=0x8c60c15au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c15c; /* fmac fr0,fr2,fr3 */
L8c60c15c: *stop=0x8c60c15cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c60c15e; /* fmov.s @(r0,r4),fr2 */
L8c60c15e: *stop=0x8c60c15eu;r[0]=8u;goto L8c60c160; /* mov #0x8,r0 */
L8c60c160: *stop=0x8c60c160u;fcopy(f,0u,4u);goto L8c60c162; /* fmov fr4,fr0 */
L8c60c162: *stop=0x8c60c162u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c164; /* fmac fr0,fr2,fr3 */
L8c60c164: *stop=0x8c60c164u;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[7]);goto L8c60c166; /* fmov fr3,@(r0,r7) */
L8c60c166: *stop=0x8c60c166u;r[15]+=8u;goto L8c60c168; /* add #0x8,r15 */
L8c60c168: *stop=0x8c60c168u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60c16a; /* lds.l @r15+,PR */
L8c60c16a: *stop=0x8c60c16au;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c60c16c; /* mov.l @r15+,r11 */
L8c60c16c: *stop=0x8c60c16cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60c16e; /* mov.l @r15+,r12 */
L8c60c16e: *stop=0x8c60c16eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60c170; /* mov.l @r15+,r13 */
L8c60c170: *stop=0x8c60c170u;if(*pr!=entry_pr)return -3;*stop=0x8c60c172u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_cubic_angles_8c60c19c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60c19c;
L8c60c19c: *stop=0x8c60c19cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60c19e; /* mov.l r14,@-r15 */
L8c60c19e: *stop=0x8c60c19eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60c1a0; /* mov.l r13,@-r15 */
L8c60c1a0: *stop=0x8c60c1a0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60c1a2; /* mov.l r12,@-r15 */
L8c60c1a2: *stop=0x8c60c1a2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c60c1a4; /* mov.l r11,@-r15 */
L8c60c1a4: *stop=0x8c60c1a4u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[15]);goto L8c60c1a6; /* fmov.s fr12,@-r15 */
L8c60c1a6: *stop=0x8c60c1a6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60c1a8; /* sts.l PR,@-r15 */
L8c60c1a8: *stop=0x8c60c1a8u;r[15]+=4294967288u;goto L8c60c1aa; /* add #-0x8,r15 */
L8c60c1aa: *stop=0x8c60c1aau;r[2]=rd(m,0x8c60c450u,4);goto L8c60c1ac; /* mov.l 0x8c60c450,r2 */
L8c60c1ac: *stop=0x8c60c1acu;r[11]=r[15];goto L8c60c1ae; /* mov r15,r11 */
L8c60c1ae: *stop=0x8c60c1aeu;r[3]=rd(m,0x8c60c454u,4);goto L8c60c1b0; /* mov.l 0x8c60c454,r3 */
L8c60c1b0: *stop=0x8c60c1b0u;r[1]=r[15];goto L8c60c1b2; /* mov r15,r1 */
L8c60c1b2: target=r[3];*pr=0x8c60c1b6u;*stop=0x8c60c1b4u;r[0]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c1b6; /* jsr @r3 */
L8c60c1b6: *stop=0x8c60c1b6u;r[1]=r[5];goto L8c60c1b8; /* mov r5,r1 */
L8c60c1b8: *stop=0x8c60c1b8u;r[3]=rd(m,0x8c60c458u,4);goto L8c60c1ba; /* mov.l 0x8c60c458,r3 */
L8c60c1ba: *stop=0x8c60c1bau;r[1]+=r[6];goto L8c60c1bc; /* add r6,r1 */
L8c60c1bc: *stop=0x8c60c1bcu;r[1]+=4294967295u;goto L8c60c1be; /* add #-0x1,r1 */
L8c60c1be: target=r[3];*pr=0x8c60c1c2u;*stop=0x8c60c1c0u;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c1c2; /* jsr @r3 */
L8c60c1c2: *stop=0x8c60c1c2u;r[13]=r[5];goto L8c60c1c4; /* mov r5,r13 */
L8c60c1c4: *stop=0x8c60c1c4u;r[12]=r[0];goto L8c60c1c6; /* mov r0,r12 */
L8c60c1c6: *stop=0x8c60c1c6u;r[13]<<=2;goto L8c60c1c8; /* shll2 r13 */
L8c60c1c8: *stop=0x8c60c1c8u;r[1]=r[5];goto L8c60c1ca; /* mov r5,r1 */
L8c60c1ca: *stop=0x8c60c1cau;r[12]<<=2;goto L8c60c1cc; /* shll2 r12 */
L8c60c1cc: *stop=0x8c60c1ccu;r[3]=rd(m,0x8c60c458u,4);goto L8c60c1ce; /* mov.l 0x8c60c458,r3 */
L8c60c1ce: *stop=0x8c60c1ceu;r[13]<<=2;goto L8c60c1d0; /* shll2 r13 */
L8c60c1d0: *stop=0x8c60c1d0u;r[12]<<=2;goto L8c60c1d2; /* shll2 r12 */
L8c60c1d2: *stop=0x8c60c1d2u;r[12]+=r[4];goto L8c60c1d4; /* add r4,r12 */
L8c60c1d4: *stop=0x8c60c1d4u;r[1]+=1u;goto L8c60c1d6; /* add #0x1,r1 */
L8c60c1d6: *stop=0x8c60c1d6u;r[13]+=r[4];goto L8c60c1d8; /* add r4,r13 */
L8c60c1d8: target=r[3];*pr=0x8c60c1dcu;*stop=0x8c60c1dau;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c1dc; /* jsr @r3 */
L8c60c1dc: *stop=0x8c60c1dcu;r[14]=r[0];goto L8c60c1de; /* mov r0,r14 */
L8c60c1de: *stop=0x8c60c1deu;r[14]<<=2;goto L8c60c1e0; /* shll2 r14 */
L8c60c1e0: *stop=0x8c60c1e0u;r[1]=r[5];goto L8c60c1e2; /* mov r5,r1 */
L8c60c1e2: *stop=0x8c60c1e2u;r[3]=rd(m,0x8c60c458u,4);goto L8c60c1e4; /* mov.l 0x8c60c458,r3 */
L8c60c1e4: *stop=0x8c60c1e4u;r[14]<<=2;goto L8c60c1e6; /* shll2 r14 */
L8c60c1e6: *stop=0x8c60c1e6u;r[1]+=2u;goto L8c60c1e8; /* add #0x2,r1 */
L8c60c1e8: *stop=0x8c60c1e8u;r[14]+=r[4];goto L8c60c1ea; /* add r4,r14 */
L8c60c1ea: target=r[3];*pr=0x8c60c1eeu;*stop=0x8c60c1ecu;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60c1ee; /* jsr @r3 */
L8c60c1ee: *stop=0x8c60c1eeu;if(!valid_ram(r[13],4u))return -3;r[3]=rd(m,r[13],4);goto L8c60c1f0; /* mov.l @r13,r3 */
L8c60c1f0: *stop=0x8c60c1f0u;r[0]<<=2;goto L8c60c1f2; /* shll2 r0 */
L8c60c1f2: *stop=0x8c60c1f2u;if(!valid_ram(r[14],4u))return -3;r[1]=rd(m,r[14],4);goto L8c60c1f4; /* mov.l @r14,r1 */
L8c60c1f4: *stop=0x8c60c1f4u;r[0]<<=2;goto L8c60c1f6; /* shll2 r0 */
L8c60c1f6: *stop=0x8c60c1f6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c60c1f8; /* fldi0 fr2 */
L8c60c1f8: *stop=0x8c60c1f8u;r[4]+=r[0];goto L8c60c1fa; /* add r0,r4 */
L8c60c1fa: *stop=0x8c60c1fau;r[1]-=r[3];goto L8c60c1fc; /* sub r3,r1 */
L8c60c1fc: *stop=0x8c60c1fcu;*fpul=r[1];goto L8c60c1fe; /* lds r1,FPUL */
L8c60c1fe: *stop=0x8c60c1feu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60c200; /* float FPUL,fr3 */
L8c60c200: *stop=0x8c60c200u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c60c202; /* fcmp/gt fr3,fr2 */
L8c60c202: take=!(*sr&1u);*stop=0x8c60c204u;fcopy(f,4u,3u);if(take)goto L8c60c210;goto L8c60c206; /* bf/s 0x8c60c210 */
L8c60c206: *stop=0x8c60c206u;r[0]=0x8c60c45cu;goto L8c60c208; /* mova 0x8c60c45c,r0 */
L8c60c208: *stop=0x8c60c208u;fcopy(f,3u,4u);goto L8c60c20a; /* fmov fr4,fr3 */
L8c60c20a: *stop=0x8c60c20au;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto L8c60c20c; /* fmov.s @r0,fr1 */
L8c60c20c: *stop=0x8c60c20eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60c212; /* bra 0x8c60c212 */
L8c60c210: *stop=0x8c60c210u;fcopy(f,3u,4u);goto L8c60c212; /* fmov fr4,fr3 */
L8c60c212: *stop=0x8c60c212u;r[0]=4u;goto L8c60c214; /* mov #0x4,r0 */
L8c60c214: *stop=0x8c60c214u;if(!valid_ram(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[11]);goto L8c60c216; /* fmov fr3,@(r0,r11) */
L8c60c216: *stop=0x8c60c216u;if(!valid_ram(r[13],4u))return -3;r[5]=rd(m,r[13],4);goto L8c60c218; /* mov.l @r13,r5 */
L8c60c218: *stop=0x8c60c218u;*fpul=r[5];goto L8c60c21a; /* lds r5,FPUL */
L8c60c21a: *stop=0x8c60c21au;*sr=(*sr&~1u)|((int32_t)r[5]>=0);goto L8c60c21c; /* cmp/pz r5 */
L8c60c21c: take=(*sr&1u);*stop=0x8c60c21eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c60c228;goto L8c60c220; /* bt/s 0x8c60c228 */
L8c60c220: *stop=0x8c60c220u;r[3]=rd(m,0x8c60c45cu,4);goto L8c60c222; /* mov.l 0x8c60c45c,r3 */
L8c60c222: *stop=0x8c60c222u;*fpul=r[3];goto L8c60c224; /* lds r3,FPUL */
L8c60c224: *stop=0x8c60c224u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c60c226; /* fsts FPUL,fr2 */
L8c60c226: *stop=0x8c60c226u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60c228; /* fadd fr2,fr3 */
L8c60c228: *stop=0x8c60c228u;r[2]=rd(m,0x8c60c460u,4);goto L8c60c22a; /* mov.l 0x8c60c460,r2 */
L8c60c22a: *stop=0x8c60c22au;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c60c22c; /* mov.l @r14,r0 */
L8c60c22c: *stop=0x8c60c22cu;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[2]);goto L8c60c22e; /* fmov.s @r2,fr1 */
L8c60c22e: *stop=0x8c60c22eu;r[0]-=r[5];goto L8c60c230; /* sub r5,r0 */
L8c60c230: *stop=0x8c60c230u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c60c232; /* cmp/pl r0 */
L8c60c232: *stop=0x8c60c232u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c60c234; /* fsub fr3,fr1 */
L8c60c234: *stop=0x8c60c234u;r[0]=*sr&1u;goto L8c60c236; /* movt r0 */
L8c60c236: *stop=0x8c60c236u;r[0]<<=2;goto L8c60c238; /* shll2 r0 */
L8c60c238: *stop=0x8c60c238u;if(!valid_ram(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[11]);goto L8c60c23a; /* fmov.s @(r0,r11),fr3 */
L8c60c23a: *stop=0x8c60c23au;r[0]=0x8c60c464u;goto L8c60c23c; /* mova 0x8c60c464,r0 */
L8c60c23c: *stop=0x8c60c23cu;fcopy(f,4u,1u);goto L8c60c23e; /* fmov fr1,fr4 */
L8c60c23e: *stop=0x8c60c23eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c60c240; /* fdiv fr3,fr4 */
L8c60c240: *stop=0x8c60c240u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c60c242; /* fldi1 fr3 */
L8c60c242: *stop=0x8c60c242u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60c244; /* fadd fr3,fr3 */
L8c60c244: *stop=0x8c60c244u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,10u,r[0]);goto L8c60c246; /* fmov.s @r0,fr10 */
L8c60c246: *stop=0x8c60c246u;r[0]=0x8c60c468u;goto L8c60c248; /* mova 0x8c60c468,r0 */
L8c60c248: *stop=0x8c60c248u;fcopy(f,5u,4u);goto L8c60c24a; /* fmov fr4,fr5 */
L8c60c24a: *stop=0x8c60c24au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[4],&f->fpscr,&fr[5]);if(status)return status;}goto L8c60c24c; /* fmul fr4,fr5 */
L8c60c24c: *stop=0x8c60c24cu;fcopy(f,7u,4u);goto L8c60c24e; /* fmov fr4,fr7 */
L8c60c24e: *stop=0x8c60c24eu;fcopy(f,8u,5u);goto L8c60c250; /* fmov fr5,fr8 */
L8c60c250: *stop=0x8c60c250u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c60c252; /* fmul fr3,fr8 */
L8c60c252: *stop=0x8c60c252u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c60c254; /* fmul fr5,fr7 */
L8c60c254: *stop=0x8c60c254u;fcopy(f,2u,8u);goto L8c60c256; /* fmov fr8,fr2 */
L8c60c256: *stop=0x8c60c256u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60c258; /* fsub fr4,fr2 */
L8c60c258: *stop=0x8c60c258u;fcopy(f,11u,7u);goto L8c60c25a; /* fmov fr7,fr11 */
L8c60c25a: *stop=0x8c60c25au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[7],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60c25c; /* fsub fr7,fr2 */
L8c60c25c: *stop=0x8c60c25cu;fcopy(f,9u,2u);goto L8c60c25e; /* fmov fr2,fr9 */
L8c60c25e: *stop=0x8c60c25eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[10],&f->fpscr,&fr[9]);if(status)return status;}goto L8c60c260; /* fmul fr10,fr9 */
L8c60c260: *stop=0x8c60c260u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c60c262; /* fmov.s @r0,fr2 */
L8c60c262: *stop=0x8c60c262u;r[0]=0x8c60c46cu;goto L8c60c264; /* mova 0x8c60c46c,r0 */
L8c60c264: *stop=0x8c60c264u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto L8c60c266; /* fmov.s @r0,fr1 */
L8c60c266: *stop=0x8c60c266u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[2],&f->fpscr,&fr[11]);if(status)return status;}goto L8c60c268; /* fmul fr2,fr11 */
L8c60c268: *stop=0x8c60c268u;fcopy(f,0u,5u);goto L8c60c26a; /* fmov fr5,fr0 */
L8c60c26a: *stop=0x8c60c26au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[1],&f->fpscr,&fr[0]);if(status)return status;}goto L8c60c26c; /* fmul fr1,fr0 */
L8c60c26c: *stop=0x8c60c26cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c60c26e; /* fmul fr10,fr4 */
L8c60c26e: *stop=0x8c60c26eu;fcopy(f,3u,11u);goto L8c60c270; /* fmov fr11,fr3 */
L8c60c270: *stop=0x8c60c270u;if(!valid_ram(r[4]+4u,4u))return -3;r[3]=rd(m,r[4]+0x4u,4);goto L8c60c272; /* mov.l @(0x4,r4),r3 */
L8c60c272: *stop=0x8c60c272u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[5],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c60c274; /* fsub fr5,fr7 */
L8c60c274: *stop=0x8c60c274u;if(!valid_ram(r[14]+4u,4u))return -3;r[1]=rd(m,r[14]+0x4u,4);goto L8c60c276; /* mov.l @(0x4,r14),r1 */
L8c60c276: *stop=0x8c60c276u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[0],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c60c278; /* fsub fr0,fr3 */
L8c60c278: *stop=0x8c60c278u;*fpul=r[3];goto L8c60c27a; /* lds r3,FPUL */
L8c60c27a: *stop=0x8c60c27au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[11],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c60c27c; /* fsub fr11,fr4 */
L8c60c27c: *stop=0x8c60c27cu;if(!valid_ram(r[12]+4u,4u))return -3;r[3]=rd(m,r[12]+0x4u,4);goto L8c60c27e; /* mov.l @(0x4,r12),r3 */
L8c60c27e: *stop=0x8c60c27eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;goto L8c60c280; /* fldi1 fr0 */
L8c60c280: *stop=0x8c60c280u;fcopy(f,6u,3u);goto L8c60c282; /* fmov fr3,fr6 */
L8c60c282: *stop=0x8c60c282u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[0],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c60c284; /* fadd fr0,fr6 */
L8c60c284: *stop=0x8c60c284u;fcopy(f,3u,8u);goto L8c60c286; /* fmov fr8,fr3 */
L8c60c286: *stop=0x8c60c286u;fcopy(f,8u,4u);goto L8c60c288; /* fmov fr4,fr8 */
L8c60c288: *stop=0x8c60c288u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c60c28a; /* fadd fr3,fr8 */
L8c60c28a: *stop=0x8c60c28au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60c28c; /* float FPUL,fr3 */
L8c60c28c: *stop=0x8c60c28cu;*fpul=r[1];goto L8c60c28e; /* lds r1,FPUL */
L8c60c28e: *stop=0x8c60c28eu;if(!valid_ram(r[13]+4u,4u))return -3;r[1]=rd(m,r[13]+0x4u,4);goto L8c60c290; /* mov.l @(0x4,r13),r1 */
L8c60c290: *stop=0x8c60c290u;fcopy(f,12u,6u);goto L8c60c292; /* fmov fr6,fr12 */
L8c60c292: *stop=0x8c60c292u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c60c294; /* float FPUL,fr2 */
L8c60c294: *stop=0x8c60c294u;*fpul=r[3];goto L8c60c296; /* lds r3,FPUL */
L8c60c296: *stop=0x8c60c296u;fcopy(f,4u,7u);goto L8c60c298; /* fmov fr7,fr4 */
L8c60c298: *stop=0x8c60c298u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c60c29a; /* fmul fr10,fr4 */
L8c60c29a: *stop=0x8c60c29au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c60c29c; /* float FPUL,fr1 */
L8c60c29c: *stop=0x8c60c29cu;*fpul=r[1];goto L8c60c29e; /* lds r1,FPUL */
L8c60c29e: *stop=0x8c60c29eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][0]);if(status)return status;goto L8c60c2a0; /* float FPUL,fr0 */
L8c60c2a0: *stop=0x8c60c2a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[0],&f->fpscr,&fr[12]);if(status)return status;}goto L8c60c2a2; /* fmul fr0,fr12 */
L8c60c2a2: *stop=0x8c60c2a2u;fcopy(f,0u,9u);goto L8c60c2a4; /* fmov fr9,fr0 */
L8c60c2a4: *stop=0x8c60c2a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c60c2a6; /* fmac fr0,fr1,fr12 */
L8c60c2a6: *stop=0x8c60c2a6u;fcopy(f,0u,8u);goto L8c60c2a8; /* fmov fr8,fr0 */
L8c60c2a8: *stop=0x8c60c2a8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c60c2aa; /* fmac fr0,fr2,fr12 */
L8c60c2aa: *stop=0x8c60c2aau;fcopy(f,0u,4u);goto L8c60c2ac; /* fmov fr4,fr0 */
L8c60c2ac: *stop=0x8c60c2acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c60c2ae; /* fmac fr0,fr3,fr12 */
L8c60c2ae: *stop=0x8c60c2aeu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,fpul);if(status)return status;goto L8c60c2b0; /* ftrc fr12,FPUL */
L8c60c2b0: *stop=0x8c60c2b0u;fcopy(f,12u,6u);goto L8c60c2b2; /* fmov fr6,fr12 */
L8c60c2b2: *stop=0x8c60c2b2u;r[3]=*fpul;goto L8c60c2b4; /* sts FPUL,r3 */
L8c60c2b4: *stop=0x8c60c2b4u;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[3],4);goto L8c60c2b6; /* mov.l r3,@r7 */
L8c60c2b6: *stop=0x8c60c2b6u;if(!valid_ram(r[4]+8u,4u))return -3;r[1]=rd(m,r[4]+0x8u,4);goto L8c60c2b8; /* mov.l @(0x8,r4),r1 */
L8c60c2b8: *stop=0x8c60c2b8u;if(!valid_ram(r[14]+8u,4u))return -3;r[3]=rd(m,r[14]+0x8u,4);goto L8c60c2ba; /* mov.l @(0x8,r14),r3 */
L8c60c2ba: *stop=0x8c60c2bau;*fpul=r[1];goto L8c60c2bc; /* lds r1,FPUL */
L8c60c2bc: *stop=0x8c60c2bcu;if(!valid_ram(r[12]+8u,4u))return -3;r[1]=rd(m,r[12]+0x8u,4);goto L8c60c2be; /* mov.l @(0x8,r12),r1 */
L8c60c2be: *stop=0x8c60c2beu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60c2c0; /* float FPUL,fr3 */
L8c60c2c0: *stop=0x8c60c2c0u;*fpul=r[3];goto L8c60c2c2; /* lds r3,FPUL */
L8c60c2c2: *stop=0x8c60c2c2u;if(!valid_ram(r[13]+8u,4u))return -3;r[3]=rd(m,r[13]+0x8u,4);goto L8c60c2c4; /* mov.l @(0x8,r13),r3 */
L8c60c2c4: *stop=0x8c60c2c4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c60c2c6; /* float FPUL,fr2 */
L8c60c2c6: *stop=0x8c60c2c6u;*fpul=r[1];goto L8c60c2c8; /* lds r1,FPUL */
L8c60c2c8: *stop=0x8c60c2c8u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c60c2ca; /* float FPUL,fr1 */
L8c60c2ca: *stop=0x8c60c2cau;*fpul=r[3];goto L8c60c2cc; /* lds r3,FPUL */
L8c60c2cc: *stop=0x8c60c2ccu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][0]);if(status)return status;goto L8c60c2ce; /* float FPUL,fr0 */
L8c60c2ce: *stop=0x8c60c2ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[0],&f->fpscr,&fr[12]);if(status)return status;}goto L8c60c2d0; /* fmul fr0,fr12 */
L8c60c2d0: *stop=0x8c60c2d0u;fcopy(f,0u,9u);goto L8c60c2d2; /* fmov fr9,fr0 */
L8c60c2d2: *stop=0x8c60c2d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c60c2d4; /* fmac fr0,fr1,fr12 */
L8c60c2d4: *stop=0x8c60c2d4u;fcopy(f,0u,8u);goto L8c60c2d6; /* fmov fr8,fr0 */
L8c60c2d6: *stop=0x8c60c2d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c60c2d8; /* fmac fr0,fr2,fr12 */
L8c60c2d8: *stop=0x8c60c2d8u;fcopy(f,0u,4u);goto L8c60c2da; /* fmov fr4,fr0 */
L8c60c2da: *stop=0x8c60c2dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c60c2dc; /* fmac fr0,fr3,fr12 */
L8c60c2dc: *stop=0x8c60c2dcu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,fpul);if(status)return status;goto L8c60c2de; /* ftrc fr12,FPUL */
L8c60c2de: *stop=0x8c60c2deu;r[3]=*fpul;goto L8c60c2e0; /* sts FPUL,r3 */
L8c60c2e0: *stop=0x8c60c2e0u;if(!valid_ram(r[7]+4u,4u))return -3;wr(m,r[7]+0x4u,r[3],4);goto L8c60c2e2; /* mov.l r3,@(0x4,r7) */
L8c60c2e2: *stop=0x8c60c2e2u;if(!valid_ram(r[4]+12u,4u))return -3;r[1]=rd(m,r[4]+0xcu,4);goto L8c60c2e4; /* mov.l @(0xc,r4),r1 */
L8c60c2e4: *stop=0x8c60c2e4u;*fpul=r[1];goto L8c60c2e6; /* lds r1,FPUL */
L8c60c2e6: *stop=0x8c60c2e6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60c2e8; /* float FPUL,fr3 */
L8c60c2e8: *stop=0x8c60c2e8u;if(!valid_ram(r[14]+12u,4u))return -3;r[3]=rd(m,r[14]+0xcu,4);goto L8c60c2ea; /* mov.l @(0xc,r14),r3 */
L8c60c2ea: *stop=0x8c60c2eau;if(!valid_ram(r[12]+12u,4u))return -3;r[1]=rd(m,r[12]+0xcu,4);goto L8c60c2ec; /* mov.l @(0xc,r12),r1 */
L8c60c2ec: *stop=0x8c60c2ecu;*fpul=r[3];goto L8c60c2ee; /* lds r3,FPUL */
L8c60c2ee: *stop=0x8c60c2eeu;if(!valid_ram(r[13]+12u,4u))return -3;r[3]=rd(m,r[13]+0xcu,4);goto L8c60c2f0; /* mov.l @(0xc,r13),r3 */
L8c60c2f0: *stop=0x8c60c2f0u;fcopy(f,12u,3u);goto L8c60c2f2; /* fmov fr3,fr12 */
L8c60c2f2: *stop=0x8c60c2f2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c60c2f4; /* float FPUL,fr2 */
L8c60c2f4: *stop=0x8c60c2f4u;*fpul=r[1];goto L8c60c2f6; /* lds r1,FPUL */
L8c60c2f6: *stop=0x8c60c2f6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c60c2f8; /* float FPUL,fr1 */
L8c60c2f8: *stop=0x8c60c2f8u;*fpul=r[3];goto L8c60c2fa; /* lds r3,FPUL */
L8c60c2fa: *stop=0x8c60c2fau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][0]);if(status)return status;goto L8c60c2fc; /* float FPUL,fr0 */
L8c60c2fc: *stop=0x8c60c2fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c60c2fe; /* fmul fr0,fr6 */
L8c60c2fe: *stop=0x8c60c2feu;fcopy(f,0u,9u);goto L8c60c300; /* fmov fr9,fr0 */
L8c60c300: *stop=0x8c60c300u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[6],&f->fpscr,&fr[6]);if(status)return status;}goto L8c60c302; /* fmac fr0,fr1,fr6 */
L8c60c302: *stop=0x8c60c302u;fcopy(f,0u,8u);goto L8c60c304; /* fmov fr8,fr0 */
L8c60c304: *stop=0x8c60c304u;fcopy(f,3u,6u);goto L8c60c306; /* fmov fr6,fr3 */
L8c60c306: *stop=0x8c60c306u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c308; /* fmac fr0,fr2,fr3 */
L8c60c308: *stop=0x8c60c308u;fcopy(f,0u,4u);goto L8c60c30a; /* fmov fr4,fr0 */
L8c60c30a: *stop=0x8c60c30au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60c30c; /* fmac fr0,fr12,fr3 */
L8c60c30c: *stop=0x8c60c30cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c60c30e; /* ftrc fr3,FPUL */
L8c60c30e: *stop=0x8c60c30eu;r[3]=*fpul;goto L8c60c310; /* sts FPUL,r3 */
L8c60c310: *stop=0x8c60c310u;if(!valid_ram(r[7]+8u,4u))return -3;wr(m,r[7]+0x8u,r[3],4);goto L8c60c312; /* mov.l r3,@(0x8,r7) */
L8c60c312: *stop=0x8c60c312u;r[15]+=8u;goto L8c60c314; /* add #0x8,r15 */
L8c60c314: *stop=0x8c60c314u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60c316; /* lds.l @r15+,PR */
L8c60c316: *stop=0x8c60c316u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c60c318; /* fmov.s @r15+,fr12 */
L8c60c318: *stop=0x8c60c318u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c60c31a; /* mov.l @r15+,r11 */
L8c60c31a: *stop=0x8c60c31au;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60c31c; /* mov.l @r15+,r12 */
L8c60c31c: *stop=0x8c60c31cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60c31e; /* mov.l @r15+,r13 */
L8c60c31e: *stop=0x8c60c31eu;if(*pr!=entry_pr)return -3;*stop=0x8c60c320u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
