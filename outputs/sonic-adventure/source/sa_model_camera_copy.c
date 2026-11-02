/* Original computed record copy, all seventeen internal table entries; names inferred.
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
SA_STAGE_EXPORT int sa_camera_record_copy_8c10cd44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cd44;
L8c10cd44: *stop=0x8c10cd44u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[3],4);goto L8c10cd46; /* mov.l r3,@-r15 */
L8c10cd46: *stop=0x8c10cd46u;r[3]=rd(m,0x8c10cd50u,4);goto L8c10cd48; /* mov.l 0x8c10cd50,r3 */
L8c10cd48: *stop=0x8c10cd48u;if(!valid_ram(r[0]+r[3],4u))return -3;r[3]=rd(m,r[3]+r[0],4);goto L8c10cd4a; /* mov.l @(r0,r3),r3 */
L8c10cd4a: *stop=0x8c10cd4au;r[0]+=4294967292u;goto L8c10cd4c; /* add #-0x4,r0 */
L8c10cd4c: target=r[3];*stop=0x8c10cd4eu;if(!valid_ram(r[0]+r[2],4u))return -3;r[3]=rd(m,r[2]+r[0],4);switch(target){case 0x8c10cd56u:goto L8c10cd56;case 0x8c10cd5au:goto L8c10cd5a;case 0x8c10cd5eu:goto L8c10cd5e;case 0x8c10cd62u:goto L8c10cd62;case 0x8c10cd66u:goto L8c10cd66;case 0x8c10cd6au:goto L8c10cd6a;case 0x8c10cd6eu:goto L8c10cd6e;case 0x8c10cd72u:goto L8c10cd72;case 0x8c10cd76u:goto L8c10cd76;case 0x8c10cd7au:goto L8c10cd7a;case 0x8c10cd7eu:goto L8c10cd7e;case 0x8c10cd82u:goto L8c10cd82;case 0x8c10cd86u:goto L8c10cd86;case 0x8c10cd8au:goto L8c10cd8a;case 0x8c10cd8eu:goto L8c10cd8e;case 0x8c10cd92u:goto L8c10cd92;case 0x8c10cd94u:goto L8c10cd94;default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);} /* jmp @r3 */
L8c10cd56: *stop=0x8c10cd56u;if(!valid_ram(r[2]+56u,4u))return -3;r[3]=rd(m,r[2]+0x38u,4);goto L8c10cd58; /* mov.l @(0x38,r2),r3 */
L8c10cd58: *stop=0x8c10cd58u;if(!valid_ram(r[1]+60u,4u))return -3;wr(m,r[1]+0x3cu,r[0],4);goto L8c10cd5a; /* mov.l r0,@(0x3c,r1) */
L8c10cd5a: *stop=0x8c10cd5au;if(!valid_ram(r[2]+52u,4u))return -3;r[0]=rd(m,r[2]+0x34u,4);goto L8c10cd5c; /* mov.l @(0x34,r2),r0 */
L8c10cd5c: *stop=0x8c10cd5cu;if(!valid_ram(r[1]+56u,4u))return -3;wr(m,r[1]+0x38u,r[3],4);goto L8c10cd5e; /* mov.l r3,@(0x38,r1) */
L8c10cd5e: *stop=0x8c10cd5eu;if(!valid_ram(r[2]+48u,4u))return -3;r[3]=rd(m,r[2]+0x30u,4);goto L8c10cd60; /* mov.l @(0x30,r2),r3 */
L8c10cd60: *stop=0x8c10cd60u;if(!valid_ram(r[1]+52u,4u))return -3;wr(m,r[1]+0x34u,r[0],4);goto L8c10cd62; /* mov.l r0,@(0x34,r1) */
L8c10cd62: *stop=0x8c10cd62u;if(!valid_ram(r[2]+44u,4u))return -3;r[0]=rd(m,r[2]+0x2cu,4);goto L8c10cd64; /* mov.l @(0x2c,r2),r0 */
L8c10cd64: *stop=0x8c10cd64u;if(!valid_ram(r[1]+48u,4u))return -3;wr(m,r[1]+0x30u,r[3],4);goto L8c10cd66; /* mov.l r3,@(0x30,r1) */
L8c10cd66: *stop=0x8c10cd66u;if(!valid_ram(r[2]+40u,4u))return -3;r[3]=rd(m,r[2]+0x28u,4);goto L8c10cd68; /* mov.l @(0x28,r2),r3 */
L8c10cd68: *stop=0x8c10cd68u;if(!valid_ram(r[1]+44u,4u))return -3;wr(m,r[1]+0x2cu,r[0],4);goto L8c10cd6a; /* mov.l r0,@(0x2c,r1) */
L8c10cd6a: *stop=0x8c10cd6au;if(!valid_ram(r[2]+36u,4u))return -3;r[0]=rd(m,r[2]+0x24u,4);goto L8c10cd6c; /* mov.l @(0x24,r2),r0 */
L8c10cd6c: *stop=0x8c10cd6cu;if(!valid_ram(r[1]+40u,4u))return -3;wr(m,r[1]+0x28u,r[3],4);goto L8c10cd6e; /* mov.l r3,@(0x28,r1) */
L8c10cd6e: *stop=0x8c10cd6eu;if(!valid_ram(r[2]+32u,4u))return -3;r[3]=rd(m,r[2]+0x20u,4);goto L8c10cd70; /* mov.l @(0x20,r2),r3 */
L8c10cd70: *stop=0x8c10cd70u;if(!valid_ram(r[1]+36u,4u))return -3;wr(m,r[1]+0x24u,r[0],4);goto L8c10cd72; /* mov.l r0,@(0x24,r1) */
L8c10cd72: *stop=0x8c10cd72u;if(!valid_ram(r[2]+28u,4u))return -3;r[0]=rd(m,r[2]+0x1cu,4);goto L8c10cd74; /* mov.l @(0x1c,r2),r0 */
L8c10cd74: *stop=0x8c10cd74u;if(!valid_ram(r[1]+32u,4u))return -3;wr(m,r[1]+0x20u,r[3],4);goto L8c10cd76; /* mov.l r3,@(0x20,r1) */
L8c10cd76: *stop=0x8c10cd76u;if(!valid_ram(r[2]+24u,4u))return -3;r[3]=rd(m,r[2]+0x18u,4);goto L8c10cd78; /* mov.l @(0x18,r2),r3 */
L8c10cd78: *stop=0x8c10cd78u;if(!valid_ram(r[1]+28u,4u))return -3;wr(m,r[1]+0x1cu,r[0],4);goto L8c10cd7a; /* mov.l r0,@(0x1c,r1) */
L8c10cd7a: *stop=0x8c10cd7au;if(!valid_ram(r[2]+20u,4u))return -3;r[0]=rd(m,r[2]+0x14u,4);goto L8c10cd7c; /* mov.l @(0x14,r2),r0 */
L8c10cd7c: *stop=0x8c10cd7cu;if(!valid_ram(r[1]+24u,4u))return -3;wr(m,r[1]+0x18u,r[3],4);goto L8c10cd7e; /* mov.l r3,@(0x18,r1) */
L8c10cd7e: *stop=0x8c10cd7eu;if(!valid_ram(r[2]+16u,4u))return -3;r[3]=rd(m,r[2]+0x10u,4);goto L8c10cd80; /* mov.l @(0x10,r2),r3 */
L8c10cd80: *stop=0x8c10cd80u;if(!valid_ram(r[1]+20u,4u))return -3;wr(m,r[1]+0x14u,r[0],4);goto L8c10cd82; /* mov.l r0,@(0x14,r1) */
L8c10cd82: *stop=0x8c10cd82u;if(!valid_ram(r[2]+12u,4u))return -3;r[0]=rd(m,r[2]+0xcu,4);goto L8c10cd84; /* mov.l @(0xc,r2),r0 */
L8c10cd84: *stop=0x8c10cd84u;if(!valid_ram(r[1]+16u,4u))return -3;wr(m,r[1]+0x10u,r[3],4);goto L8c10cd86; /* mov.l r3,@(0x10,r1) */
L8c10cd86: *stop=0x8c10cd86u;if(!valid_ram(r[2]+8u,4u))return -3;r[3]=rd(m,r[2]+0x8u,4);goto L8c10cd88; /* mov.l @(0x8,r2),r3 */
L8c10cd88: *stop=0x8c10cd88u;if(!valid_ram(r[1]+12u,4u))return -3;wr(m,r[1]+0xcu,r[0],4);goto L8c10cd8a; /* mov.l r0,@(0xc,r1) */
L8c10cd8a: *stop=0x8c10cd8au;if(!valid_ram(r[2]+4u,4u))return -3;r[0]=rd(m,r[2]+0x4u,4);goto L8c10cd8c; /* mov.l @(0x4,r2),r0 */
L8c10cd8c: *stop=0x8c10cd8cu;if(!valid_ram(r[1]+8u,4u))return -3;wr(m,r[1]+0x8u,r[3],4);goto L8c10cd8e; /* mov.l r3,@(0x8,r1) */
L8c10cd8e: *stop=0x8c10cd8eu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c10cd90; /* mov.l @r2,r3 */
L8c10cd90: *stop=0x8c10cd90u;if(!valid_ram(r[1]+4u,4u))return -3;wr(m,r[1]+0x4u,r[0],4);goto L8c10cd92; /* mov.l r0,@(0x4,r1) */
L8c10cd92: *stop=0x8c10cd92u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[3],4);goto L8c10cd94; /* mov.l r3,@r1 */
L8c10cd94: *stop=0x8c10cd94u;if(*pr!=entry_pr)return -3;*stop=0x8c10cd96u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
