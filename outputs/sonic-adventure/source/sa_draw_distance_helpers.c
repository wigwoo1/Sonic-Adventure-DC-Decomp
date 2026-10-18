/* Distance helper and signed-square-root tail; names inferred.
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
int sa_sh4_sqrt(uint32_t,uint32_t*,uint32_t*);
SA_STAGE_EXPORT int sa_draw_distance_8c640128(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c640128;
L8c640128: *stop=0x8c640128u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c64012a; /* mov.l r14,@-r15 */
L8c64012a: *stop=0x8c64012au;r[0]=4u;goto L8c64012c; /* mov #0x4,r0 */
L8c64012c: *stop=0x8c64012cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c64012e; /* mov.l r13,@-r15 */
L8c64012e: *stop=0x8c64012eu;r[13]=r[5];goto L8c640130; /* mov r5,r13 */
L8c640130: *stop=0x8c640130u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c640132; /* fmov.s fr15,@-r15 */
L8c640132: *stop=0x8c640132u;r[14]=r[4];goto L8c640134; /* mov r4,r14 */
L8c640134: *stop=0x8c640134u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto L8c640136; /* fmov.s fr14,@-r15 */
L8c640136: *stop=0x8c640136u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[15]);goto L8c640138; /* fmov.s fr13,@-r15 */
L8c640138: *stop=0x8c640138u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]+r[14]);goto L8c64013a; /* fmov.s @(r0,r14),fr14 */
L8c64013a: *stop=0x8c64013au;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[13]);goto L8c64013c; /* fmov.s @(r0,r13),fr0 */
L8c64013c: *stop=0x8c64013cu;r[0]=8u;goto L8c64013e; /* mov #0x8,r0 */
L8c64013e: *stop=0x8c64013eu;if(!valid_ram(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[13]);goto L8c640140; /* fmov.s @r13,fr3 */
L8c640140: *stop=0x8c640140u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[0],&f->fpscr,&fr[14],1);if(status)return status;}goto L8c640142; /* fsub fr0,fr14 */
L8c640142: *stop=0x8c640142u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[14]);goto L8c640144; /* fmov.s @r14,fr15 */
L8c640144: *stop=0x8c640144u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[0]+r[14]);goto L8c640146; /* fmov.s @(r0,r14),fr13 */
L8c640146: *stop=0x8c640146u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c640148; /* fsub fr3,fr15 */
L8c640148: *stop=0x8c640148u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[13]);goto L8c64014a; /* fmov.s @(r0,r13),fr2 */
L8c64014a: *stop=0x8c64014au;r[3]=rd(m,0x8c640168u,4);goto L8c64014c; /* mov.l 0x8c640168,r3 */
L8c64014c: *stop=0x8c64014cu;fcopy(f,3u,14u);goto L8c64014e; /* fmov fr14,fr3 */
L8c64014e: *stop=0x8c64014eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c640150; /* fmul fr14,fr3 */
L8c640150: *stop=0x8c640150u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[2],&f->fpscr,&fr[13],1);if(status)return status;}goto L8c640152; /* fsub fr2,fr13 */
L8c640152: *stop=0x8c640152u;fcopy(f,0u,15u);goto L8c640154; /* fmov fr15,fr0 */
L8c640154: *stop=0x8c640154u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[15],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c640156; /* fmac fr0,fr15,fr3 */
L8c640156: *stop=0x8c640156u;fcopy(f,0u,13u);goto L8c640158; /* fmov fr13,fr0 */
L8c640158: *stop=0x8c640158u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[13],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64015a; /* fmac fr0,fr13,fr3 */
L8c64015a: *stop=0x8c64015au;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64015c; /* fmov.s @r15+,fr13 */
L8c64015c: *stop=0x8c64015cu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64015e; /* fmov.s @r15+,fr14 */
L8c64015e: *stop=0x8c64015eu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640160; /* fmov.s @r15+,fr15 */
L8c640160: *stop=0x8c640160u;fcopy(f,4u,3u);goto L8c640162; /* fmov fr3,fr4 */
L8c640162: *stop=0x8c640162u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c640164; /* mov.l @r15+,r13 */
L8c640164: target=r[3];*stop=0x8c640166u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_draw_signed_sqrt_8c63a9c4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c63a9c4;
L8c63a9c4: *stop=0x8c63a9c4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c63a9c6; /* fldi0 fr0 */
L8c63a9c6: *stop=0x8c63a9c6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c63a9c8; /* fcmp/gt fr4,fr0 */
L8c63a9c8: take=!(*sr&1u);*stop=0x8c63a9cau;fcopy(f,0u,4u);if(take)goto L8c63a9d4;goto L8c63a9cc; /* bf/s 0x8c63a9d4 */
L8c63a9cc: *stop=0x8c63a9ccu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]&=0x7fffffffu;goto L8c63a9ce; /* fabs fr0 */
L8c63a9ce: *stop=0x8c63a9ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[0],&f->fpscr,&fr[0]);if(status)return status;}goto L8c63a9d0; /* fsqrt fr0 */
L8c63a9d0: *stop=0x8c63a9d0u;if(*pr!=entry_pr)return -3;*stop=0x8c63a9d2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;return 0; /* rts */
L8c63a9d4: *stop=0x8c63a9d4u;if(*pr!=entry_pr)return -3;*stop=0x8c63a9d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[0],&f->fpscr,&fr[0]);if(status)return status;}return 0; /* rts */
}
