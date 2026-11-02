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
SA_STAGE_EXPORT int sa_camera_draw_update_8c01b500(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01b500;
L8c01b500: *stop=0x8c01b500u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01b502; /* sts.l PR,@-r15 */
L8c01b502: *stop=0x8c01b502u;r[15]+=4294967292u;goto L8c01b504; /* add #-0x4,r15 */
L8c01b504: *stop=0x8c01b504u;r[5]=rd(m,0x8c01b5a0u,4);goto L8c01b506; /* mov.l 0x8c01b5a0,r5 */
L8c01b506: *stop=0x8c01b506u;r[3]=rd(m,0x8c01b5a8u,4);goto L8c01b508; /* mov.l 0x8c01b5a8,r3 */
L8c01b508: *stop=0x8c01b508u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto L8c01b50a; /* mov.l r5,@r15 */
L8c01b50a: *stop=0x8c01b50au;r[4]=rd(m,0x8c01b5a4u,4);goto L8c01b50c; /* mov.l 0x8c01b5a4,r4 */
L8c01b50c: *stop=0x8c01b50cu;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c01b50e; /* mov.l @r5,r5 */
L8c01b50e: target=r[3];*pr=0x8c01b512u;*stop=0x8c01b510u;r[5]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01b512; /* jsr @r3 */
L8c01b512: *stop=0x8c01b512u;r[2]=rd(m,0x8c01b5acu,4);goto L8c01b514; /* mov.l 0x8c01b5ac,r2 */
L8c01b514: *stop=0x8c01b514u;r[4]=rd(m,0x8c01b5a4u,4);goto L8c01b516; /* mov.l 0x8c01b5a4,r4 */
L8c01b516: target=r[2];*pr=0x8c01b51au;*stop=0x8c01b518u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01b51a; /* jsr @r2 */
L8c01b51a: *stop=0x8c01b51au;r[0]=0x8c01b5b0u;goto L8c01b51c; /* mova 0x8c01b5b0,r0 */
L8c01b51c: *stop=0x8c01b51cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c01b51e; /* fmov.s @r0,fr3 */
L8c01b51e: *stop=0x8c01b51eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c01b520; /* fcmp/gt fr0,fr3 */
L8c01b520: if(!(*sr&1u))goto L8c01b534;goto L8c01b522; /* bf 0x8c01b534 */
L8c01b522: *stop=0x8c01b522u;if(!valid_ram(r[15],4u))return -3;r[0]=rd(m,r[15],4);goto L8c01b524; /* mov.l @r15,r0 */
L8c01b524: *stop=0x8c01b524u;if(!valid_ram(r[0],4u))return -3;r[0]=rd(m,r[0],4);goto L8c01b526; /* mov.l @r0,r0 */
L8c01b526: *stop=0x8c01b526u;if(!valid_ram(r[0]+4u,2u))return -3;r[0]=sx(m,r[0]+0x4u,2);goto L8c01b528; /* mov.w @(0x4,r0),r0 */
L8c01b528: *stop=0x8c01b528u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c01b52a; /* tst #0x1,r0 */
L8c01b52a: if((*sr&1u))goto L8c01b534;goto L8c01b52c; /* bt 0x8c01b534 */
L8c01b52c: *stop=0x8c01b52cu;r[3]=rd(m,0x8c01b5b4u,4);goto L8c01b52e; /* mov.l 0x8c01b5b4,r3 */
L8c01b52e: *stop=0x8c01b52eu;r[2]=1u;goto L8c01b530; /* mov #0x1,r2 */
L8c01b530: *stop=0x8c01b532u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto L8c01b53a; /* bra 0x8c01b53a */
L8c01b534: *stop=0x8c01b534u;r[1]=rd(m,0x8c01b5b4u,4);goto L8c01b536; /* mov.l 0x8c01b5b4,r1 */
L8c01b536: *stop=0x8c01b536u;r[0]=0u;goto L8c01b538; /* mov #0x0,r0 */
L8c01b538: *stop=0x8c01b538u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[0],4);goto L8c01b53a; /* mov.l r0,@r1 */
L8c01b53a: *stop=0x8c01b53au;r[3]=rd(m,0x8c01b5a0u,4);goto L8c01b53c; /* mov.l 0x8c01b5a0,r3 */
L8c01b53c: *stop=0x8c01b53cu;r[1]=rd(m,0x8c01b5a4u,4);goto L8c01b53e; /* mov.l 0x8c01b5a4,r1 */
L8c01b53e: *stop=0x8c01b53eu;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c01b540; /* mov.l @r3,r2 */
L8c01b540: *stop=0x8c01b540u;if(!valid_ram(r[2]+32u,4u))return -3;r[0]=rd(m,r[2]+0x20u,4);goto L8c01b542; /* mov.l @(0x20,r2),r0 */
L8c01b542: *stop=0x8c01b542u;if(!valid_ram(r[2]+36u,4u))return -3;r[3]=rd(m,r[2]+0x24u,4);goto L8c01b544; /* mov.l @(0x24,r2),r3 */
L8c01b544: *stop=0x8c01b544u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[0],4);goto L8c01b546; /* mov.l r0,@r1 */
L8c01b546: *stop=0x8c01b546u;if(!valid_ram(r[1]+4u,4u))return -3;wr(m,r[1]+0x4u,r[3],4);goto L8c01b548; /* mov.l r3,@(0x4,r1) */
L8c01b548: *stop=0x8c01b548u;if(!valid_ram(r[2]+40u,4u))return -3;r[0]=rd(m,r[2]+0x28u,4);goto L8c01b54a; /* mov.l @(0x28,r2),r0 */
L8c01b54a: *stop=0x8c01b54au;if(!valid_ram(r[1]+8u,4u))return -3;wr(m,r[1]+0x8u,r[0],4);goto L8c01b54c; /* mov.l r0,@(0x8,r1) */
L8c01b54c: *stop=0x8c01b54cu;r[15]+=4u;goto L8c01b54e; /* add #0x4,r15 */
L8c01b54e: *stop=0x8c01b54eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01b550; /* lds.l @r15+,PR */
L8c01b550: *stop=0x8c01b550u;if(*pr!=entry_pr)return -3;*stop=0x8c01b552u;return 0; /* rts */
}
