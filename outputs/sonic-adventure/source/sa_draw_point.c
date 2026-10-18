/* Original point transform with explicit/implicit matrix; name inferred.
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
SA_STAGE_EXPORT int sa_draw_point_transform_8c638ecc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c638ecc;
L8c638ecc: *stop=0x8c638eccu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c638ece; /* tst r4,r4 */
L8c638ece: *stop=0x8c638eceu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c638ed0; /* fmov.s @r5+,fr0 */
L8c638ed0: *stop=0x8c638ed0u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c638ed2; /* fmov.s @r5+,fr1 */
L8c638ed2: *stop=0x8c638ed2u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c638ed4; /* fmov.s @r5+,fr2 */
L8c638ed4: take=!(*sr&1u);*stop=0x8c638ed6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;if(take)goto L8c638ee4;goto L8c638ed8; /* bf/s 0x8c638ee4 */
L8c638ed8: *stop=0x8c638ed8u;r[6]+=12u;goto L8c638eda; /* add #0xc,r6 */
L8c638eda: *stop=0x8c638edau;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c638edc; /* ftrv xmtrx,fv0 */
L8c638edc: *stop=0x8c638edcu;if(!valid_ram(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[6]);goto L8c638ede; /* fmov.s fr2,@-r6 */
L8c638ede: *stop=0x8c638edeu;if(!valid_ram(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[6]);goto L8c638ee0; /* fmov.s fr1,@-r6 */
L8c638ee0: *stop=0x8c638ee0u;if(*pr!=entry_pr)return -3;*stop=0x8c638ee2u;if(!valid_ram(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[6]);return 0; /* rts */
L8c638ee4: *stop=0x8c638ee4u;r[0]=16u;goto L8c638ee6; /* mov #0x10,r0 */
L8c638ee6: *stop=0x8c638ee6u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);goto L8c638ee8; /* fmov.s @r4,fr4 */
L8c638ee8: *stop=0x8c638ee8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[4]);goto L8c638eea; /* fmov.s @(r0,r4),fr5 */
L8c638eea: *stop=0x8c638eeau;r[0]=32u;goto L8c638eec; /* mov #0x20,r0 */
L8c638eec: *stop=0x8c638eecu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[4]);goto L8c638eee; /* fmov.s @(r0,r4),fr6 */
L8c638eee: *stop=0x8c638eeeu;r[0]=48u;goto L8c638ef0; /* mov #0x30,r0 */
L8c638ef0: *stop=0x8c638ef0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]+r[4]);goto L8c638ef2; /* fmov.s @(r0,r4),fr7 */
L8c638ef2: *stop=0x8c638ef2u;status=sa_sh4_vector_numeric(0xf4edu,f,*fpul);if(status)return status;goto L8c638ef4; /* fipr fv0,fv4 */
L8c638ef4: *stop=0x8c638ef4u;r[0]=4u;goto L8c638ef6; /* mov #0x4,r0 */
L8c638ef6: *stop=0x8c638ef6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[4]);goto L8c638ef8; /* fmov.s @(r0,r4),fr8 */
L8c638ef8: *stop=0x8c638ef8u;r[0]=20u;goto L8c638efa; /* mov #0x14,r0 */
L8c638efa: *stop=0x8c638efau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[0]+r[4]);goto L8c638efc; /* fmov.s @(r0,r4),fr9 */
L8c638efc: *stop=0x8c638efcu;r[0]=36u;goto L8c638efe; /* mov #0x24,r0 */
L8c638efe: *stop=0x8c638efeu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,10u,r[0]+r[4]);goto L8c638f00; /* fmov.s @(r0,r4),fr10 */
L8c638f00: *stop=0x8c638f00u;r[0]=52u;goto L8c638f02; /* mov #0x34,r0 */
L8c638f02: *stop=0x8c638f02u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,11u,r[0]+r[4]);goto L8c638f04; /* fmov.s @(r0,r4),fr11 */
L8c638f04: *stop=0x8c638f04u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,7u,r[6]);goto L8c638f06; /* fmov.s fr7,@r6 */
L8c638f06: *stop=0x8c638f06u;status=sa_sh4_vector_numeric(0xf8edu,f,*fpul);if(status)return status;goto L8c638f08; /* fipr fv0,fv8 */
L8c638f08: *stop=0x8c638f08u;r[6]+=4u;goto L8c638f0a; /* add #0x4,r6 */
L8c638f0a: *stop=0x8c638f0au;r[0]=8u;goto L8c638f0c; /* mov #0x8,r0 */
L8c638f0c: *stop=0x8c638f0cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[4]);goto L8c638f0e; /* fmov.s @(r0,r4),fr4 */
L8c638f0e: *stop=0x8c638f0eu;r[0]=24u;goto L8c638f10; /* mov #0x18,r0 */
L8c638f10: *stop=0x8c638f10u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[4]);goto L8c638f12; /* fmov.s @(r0,r4),fr5 */
L8c638f12: *stop=0x8c638f12u;r[0]=40u;goto L8c638f14; /* mov #0x28,r0 */
L8c638f14: *stop=0x8c638f14u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[4]);goto L8c638f16; /* fmov.s @(r0,r4),fr6 */
L8c638f16: *stop=0x8c638f16u;r[0]=56u;goto L8c638f18; /* mov #0x38,r0 */
L8c638f18: *stop=0x8c638f18u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]+r[4]);goto L8c638f1a; /* fmov.s @(r0,r4),fr7 */
L8c638f1a: *stop=0x8c638f1au;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,11u,r[6]);goto L8c638f1c; /* fmov.s fr11,@r6 */
L8c638f1c: *stop=0x8c638f1cu;status=sa_sh4_vector_numeric(0xf4edu,f,*fpul);if(status)return status;goto L8c638f1e; /* fipr fv0,fv4 */
L8c638f1e: *stop=0x8c638f1eu;r[6]+=4u;goto L8c638f20; /* add #0x4,r6 */
L8c638f20: *stop=0x8c638f20u;if(*pr!=entry_pr)return -3;*stop=0x8c638f22u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,7u,r[6]);return 0; /* rts */
}
