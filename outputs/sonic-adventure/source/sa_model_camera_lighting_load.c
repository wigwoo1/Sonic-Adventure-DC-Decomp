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
SA_STAGE_EXPORT int sa_camera_lighting_8c0141e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0141e0;
L8c0141e0: *stop=0x8c0141e0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0141e2; /* mov.l r14,@-r15 */
L8c0141e2: *stop=0x8c0141e2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c0141e4; /* sts.l PR,@-r15 */
L8c0141e4: *stop=0x8c0141e4u;r[15]+=4294967288u;goto L8c0141e6; /* add #-0x8,r15 */
L8c0141e6: *stop=0x8c0141e6u;r[3]=rd(m,0x8c014380u,4);goto L8c0141e8; /* mov.l 0x8c014380,r3 */
L8c0141e8: *stop=0x8c0141e8u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[5],4);goto L8c0141ea; /* mov.l r5,@(0x4,r15) */
L8c0141ea: target=r[3];*pr=0x8c0141eeu;*stop=0x8c0141ecu;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0141ee; /* jsr @r3 */
L8c0141ee: *stop=0x8c0141eeu;r[14]=r[0];goto L8c0141f0; /* mov r0,r14 */
L8c0141f0: *stop=0x8c0141f0u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0141f2; /* tst r14,r14 */
L8c0141f2: if(!(*sr&1u))goto L8c0141fe;goto L8c0141f4; /* bf 0x8c0141fe */
L8c0141f4: *stop=0x8c0141f4u;r[0]=0u;goto L8c0141f6; /* mov #0x0,r0 */
L8c0141f6: *stop=0x8c0141f6u;r[15]+=8u;goto L8c0141f8; /* add #0x8,r15 */
L8c0141f8: *stop=0x8c0141f8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c0141fa; /* lds.l @r15+,PR */
L8c0141fa: *stop=0x8c0141fau;if(*pr!=entry_pr)return -3;*stop=0x8c0141fcu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c0141fe: *stop=0x8c0141feu;r[3]=rd(m,0x8c014384u,4);goto L8c014200; /* mov.l 0x8c014384,r3 */
L8c014200: *stop=0x8c014200u;r[5]=r[15];goto L8c014202; /* mov r15,r5 */
L8c014202: target=r[3];*pr=0x8c014206u;*stop=0x8c014204u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c014206; /* jsr @r3 */
L8c014206: *stop=0x8c014206u;if(!valid_ram(r[15],4u))return -3;r[1]=rd(m,r[15],4);goto L8c014208; /* mov.l @r15,r1 */
L8c014208: *stop=0x8c014208u;r[5]=8u;goto L8c01420a; /* mov #0x8,r5 */
L8c01420a: *stop=0x8c01420au;r[2]=rd(m,0x8c014388u,4);goto L8c01420c; /* mov.l 0x8c014388,r2 */
L8c01420c: *stop=0x8c01420cu;r[5]<<=8;goto L8c01420e; /* shll8 r5 */
L8c01420e: target=r[2];*pr=0x8c014212u;*stop=0x8c014210u;r[0]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c014212; /* jsr @r2 */
L8c014212: *stop=0x8c014212u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c014214; /* tst r0,r0 */
L8c014214: if((*sr&1u))goto L8c01421a;goto L8c014216; /* bt 0x8c01421a */
L8c014216: *stop=0x8c014218u;r[4]=1u;goto L8c01421c; /* bra 0x8c01421c */
L8c01421a: *stop=0x8c01421au;r[4]=0u;goto L8c01421c; /* mov #0x0,r4 */
L8c01421c: *stop=0x8c01421cu;r[3]=rd(m,0x8c01438cu,4);goto L8c01421e; /* mov.l 0x8c01438c,r3 */
L8c01421e: *stop=0x8c01421eu;if(!valid_ram(r[15],4u))return -3;r[1]=rd(m,r[15],4);goto L8c014220; /* mov.l @r15,r1 */
L8c014220: target=r[3];*pr=0x8c014224u;*stop=0x8c014222u;r[0]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c014224; /* jsr @r3 */
L8c014224: *stop=0x8c014224u;r[3]=rd(m,0x8c014390u,4);goto L8c014226; /* mov.l 0x8c014390,r3 */
L8c014226: *stop=0x8c014226u;r[0]+=r[4];goto L8c014228; /* add r4,r0 */
L8c014228: *stop=0x8c014228u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[0],4);goto L8c01422a; /* mov.l r0,@r15 */
L8c01422a: *stop=0x8c01422au;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c01422c; /* mov.l @r15,r5 */
L8c01422c: *stop=0x8c01422cu;if(!valid_ram(r[15]+4u,4u))return -3;r[6]=rd(m,r[15]+0x4u,4);goto L8c01422e; /* mov.l @(0x4,r15),r6 */
L8c01422e: target=r[3];*pr=0x8c014232u;*stop=0x8c014230u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c014232; /* jsr @r3 */
L8c014232: *stop=0x8c014232u;r[2]=rd(m,0x8c014394u,4);goto L8c014234; /* mov.l 0x8c014394,r2 */
L8c014234: target=r[2];*pr=0x8c014238u;*stop=0x8c014236u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c014238; /* jsr @r2 */
L8c014238: *stop=0x8c014238u;r[0]=1u;goto L8c01423a; /* mov #0x1,r0 */
L8c01423a: *stop=0x8c01423au;r[15]+=8u;goto L8c01423c; /* add #0x8,r15 */
L8c01423c: *stop=0x8c01423cu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01423e; /* lds.l @r15+,PR */
L8c01423e: *stop=0x8c01423eu;if(*pr!=entry_pr)return -3;*stop=0x8c014240u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
