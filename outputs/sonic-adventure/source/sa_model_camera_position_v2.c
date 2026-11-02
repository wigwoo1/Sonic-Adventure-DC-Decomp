/* Original camera positioning, preset and shared-tail services; names inferred.
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
SA_STAGE_EXPORT int sa_camera_position_8c019400(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c019400;
L8c019400: *stop=0x8c019400u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c019402; /* mov.l r14,@-r15 */
L8c019402: *stop=0x8c019402u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c019404; /* mov.l r13,@-r15 */
L8c019404: *stop=0x8c019404u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c019406; /* mov.l r12,@-r15 */
L8c019406: *stop=0x8c019406u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c019408; /* sts.l PR,@-r15 */
L8c019408: *stop=0x8c019408u;r[15]+=4294967272u;goto L8c01940a; /* add #-0x18,r15 */
L8c01940a: *stop=0x8c01940au;r[13]=rd(m,0x8c019624u,4);goto L8c01940c; /* mov.l 0x8c019624,r13 */
L8c01940c: *stop=0x8c01940cu;r[0]=32u;goto L8c01940e; /* mov #0x20,r0 */
L8c01940e: *stop=0x8c01940eu;r[14]=rd(m,0x8c019620u,4);goto L8c019410; /* mov.l 0x8c019620,r14 */
L8c019410: *stop=0x8c019410u;r[12]=0u;goto L8c019412; /* mov #0x0,r12 */
L8c019412: *stop=0x8c019412u;if(!valid_ram(r[13],4u))return -3;r[2]=rd(m,r[13],4);goto L8c019414; /* mov.l @r13,r2 */
L8c019414: *stop=0x8c019414u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c019416; /* mov.l @r14,r3 */
L8c019416: *stop=0x8c019416u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[2]);goto L8c019418; /* fmov.s @(r0,r2),fr3 */
L8c019418: *stop=0x8c019418u;r[0]=24u;goto L8c01941a; /* mov #0x18,r0 */
L8c01941a: *stop=0x8c01941au;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[3]);goto L8c01941c; /* fmov fr3,@(r0,r3) */
L8c01941c: *stop=0x8c01941cu;r[0]=0x8c019628u;goto L8c01941e; /* mova 0x8c019628,r0 */
L8c01941e: *stop=0x8c01941eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c019420; /* fmov.s @r0,fr3 */
L8c019420: *stop=0x8c019420u;r[0]=36u;goto L8c019422; /* mov #0x24,r0 */
L8c019422: *stop=0x8c019422u;if(!valid_ram(r[13],4u))return -3;r[2]=rd(m,r[13],4);goto L8c019424; /* mov.l @r13,r2 */
L8c019424: *stop=0x8c019424u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c019426; /* mov.l @r14,r3 */
L8c019426: *stop=0x8c019426u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[2]);goto L8c019428; /* fmov.s @(r0,r2),fr2 */
L8c019428: *stop=0x8c019428u;r[0]=28u;goto L8c01942a; /* mov #0x1c,r0 */
L8c01942a: *stop=0x8c01942au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c01942c; /* fadd fr3,fr2 */
L8c01942c: *stop=0x8c01942cu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[3]);goto L8c01942e; /* fmov fr2,@(r0,r3) */
L8c01942e: *stop=0x8c01942eu;r[0]=40u;goto L8c019430; /* mov #0x28,r0 */
L8c019430: *stop=0x8c019430u;if(!valid_ram(r[13],4u))return -3;r[2]=rd(m,r[13],4);goto L8c019432; /* mov.l @r13,r2 */
L8c019432: *stop=0x8c019432u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c019434; /* mov.l @r14,r3 */
L8c019434: *stop=0x8c019434u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[2]);goto L8c019436; /* fmov.s @(r0,r2),fr2 */
L8c019436: *stop=0x8c019436u;r[0]=32u;goto L8c019438; /* mov #0x20,r0 */
L8c019438: *stop=0x8c019438u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[3]);goto L8c01943a; /* fmov fr2,@(r0,r3) */
L8c01943a: *stop=0x8c01943au;r[0]=0x8c01962cu;goto L8c01943c; /* mova 0x8c01962c,r0 */
L8c01943c: *stop=0x8c01943cu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01943e; /* mov.l @r14,r3 */
L8c01943e: *stop=0x8c01943eu;if(!valid_ram(r[3]+36u,4u))return -3;wr(m,r[3]+0x24u,r[12],4);goto L8c019440; /* mov.l r12,@(0x24,r3) */
L8c019440: *stop=0x8c019440u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c019442; /* mov.l @r14,r2 */
L8c019442: *stop=0x8c019442u;if(!valid_ram(r[2]+40u,4u))return -3;wr(m,r[2]+0x28u,r[12],4);goto L8c019444; /* mov.l r12,@(0x28,r2) */
L8c019444: *stop=0x8c019444u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c019446; /* mov.l @r14,r3 */
L8c019446: *stop=0x8c019446u;if(!valid_ram(r[3]+44u,4u))return -3;wr(m,r[3]+0x2cu,r[12],4);goto L8c019448; /* mov.l r12,@(0x2c,r3) */
L8c019448: *stop=0x8c019448u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c01944a; /* mov.l @r14,r2 */
L8c01944a: *stop=0x8c01944au;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c01944c; /* fmov.s @r0,fr2 */
L8c01944c: *stop=0x8c01944cu;r[0]=8u;goto L8c01944e; /* mov #0x8,r0 */
L8c01944e: *stop=0x8c01944eu;r[3]=rd(m,0x8c019630u,4);goto L8c019450; /* mov.l 0x8c019630,r3 */
L8c019450: *stop=0x8c019450u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[2]);goto L8c019452; /* fmov fr2,@(r0,r2) */
L8c019452: *stop=0x8c019452u;r[2]=r[15];goto L8c019454; /* mov r15,r2 */
L8c019454: *stop=0x8c019454u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c019456; /* mov.l @r3,r1 */
L8c019456: *stop=0x8c019456u;r[2]+=12u;goto L8c019458; /* add #0xc,r2 */
L8c019458: *stop=0x8c019458u;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto L8c01945a; /* mov.l @(0x4,r3),r0 */
L8c01945a: *stop=0x8c01945au;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto L8c01945c; /* mov.l r1,@r2 */
L8c01945c: *stop=0x8c01945cu;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto L8c01945e; /* mov.l r0,@(0x4,r2) */
L8c01945e: *stop=0x8c01945eu;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto L8c019460; /* mov.l @(0x8,r3),r1 */
L8c019460: *stop=0x8c019460u;r[4]=rd(m,0x8c019634u,4);goto L8c019462; /* mov.l 0x8c019634,r4 */
L8c019462: *stop=0x8c019462u;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto L8c019464; /* mov.l r1,@(0x8,r2) */
L8c019464: *stop=0x8c019464u;r[2]=rd(m,0x8c019638u,4);goto L8c019466; /* mov.l 0x8c019638,r2 */
L8c019466: target=r[2];*pr=0x8c01946au;*stop=0x8c019468u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01946a; /* jsr @r2 */
L8c01946a: *stop=0x8c01946au;if(!valid_ram(r[13],4u))return -3;r[3]=rd(m,r[13],4);goto L8c01946c; /* mov.l @r13,r3 */
L8c01946c: *stop=0x8c01946cu;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[3],4);goto L8c01946e; /* mov.l r3,@(0x8,r15) */
L8c01946e: *stop=0x8c01946eu;if(!valid_ram(r[3]+28u,4u))return -3;r[2]=rd(m,r[3]+0x1cu,4);goto L8c019470; /* mov.l @(0x1c,r3),r2 */
L8c019470: *stop=0x8c019470u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c019472; /* tst r2,r2 */
L8c019472: take=(*sr&1u);*stop=0x8c019474u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);if(take)goto L8c01947e;goto L8c019476; /* bt/s 0x8c01947e */
L8c019476: *stop=0x8c019476u;r[3]=rd(m,0x8c01963cu,4);goto L8c019478; /* mov.l 0x8c01963c,r3 */
L8c019478: *stop=0x8c019478u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c01947a; /* mov.l @r15,r5 */
L8c01947a: target=r[3];*pr=0x8c01947eu;*stop=0x8c01947cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01947e; /* jsr @r3 */
L8c01947e: *stop=0x8c01947eu;if(!valid_ram(r[13],4u))return -3;r[3]=rd(m,r[13],4);goto L8c019480; /* mov.l @r13,r3 */
L8c019480: *stop=0x8c019480u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[3],4);goto L8c019482; /* mov.l r3,@(0x4,r15) */
L8c019482: *stop=0x8c019482u;if(!valid_ram(r[3]+20u,4u))return -3;r[2]=rd(m,r[3]+0x14u,4);goto L8c019484; /* mov.l @(0x14,r3),r2 */
L8c019484: *stop=0x8c019484u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c019486; /* tst r2,r2 */
L8c019486: take=(*sr&1u);*stop=0x8c019488u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);if(take)goto L8c019492;goto L8c01948a; /* bt/s 0x8c019492 */
L8c01948a: *stop=0x8c01948au;r[3]=rd(m,0x8c019640u,4);goto L8c01948c; /* mov.l 0x8c019640,r3 */
L8c01948c: *stop=0x8c01948cu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c01948e; /* mov.l @r15,r5 */
L8c01948e: target=r[3];*pr=0x8c019492u;*stop=0x8c019490u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019492; /* jsr @r3 */
L8c019492: *stop=0x8c019492u;if(!valid_ram(r[13],4u))return -3;r[3]=rd(m,r[13],4);goto L8c019494; /* mov.l @r13,r3 */
L8c019494: *stop=0x8c019494u;if(!valid_ram(r[3]+24u,4u))return -3;r[2]=rd(m,r[3]+0x18u,4);goto L8c019496; /* mov.l @(0x18,r3),r2 */
L8c019496: *stop=0x8c019496u;r[3]=64u;goto L8c019498; /* mov #0x40,r3 */
L8c019498: *stop=0x8c019498u;r[3]<<=8;goto L8c01949a; /* shll8 r3 */
L8c01949a: *stop=0x8c01949au;r[2]=0u-r[2];goto L8c01949c; /* neg r2,r2 */
L8c01949c: *stop=0x8c01949cu;r[2]+=r[3];goto L8c01949e; /* add r3,r2 */
L8c01949e: *stop=0x8c01949eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0194a0; /* tst r2,r2 */
L8c0194a0: take=(*sr&1u);*stop=0x8c0194a2u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);if(take)goto L8c0194ac;goto L8c0194a4; /* bt/s 0x8c0194ac */
L8c0194a4: *stop=0x8c0194a4u;r[1]=rd(m,0x8c019644u,4);goto L8c0194a6; /* mov.l 0x8c019644,r1 */
L8c0194a6: *stop=0x8c0194a6u;r[5]=r[2];goto L8c0194a8; /* mov r2,r5 */
L8c0194a8: target=r[1];*pr=0x8c0194acu;*stop=0x8c0194aau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0194ac; /* jsr @r1 */
L8c0194ac: *stop=0x8c0194acu;r[5]=r[15];goto L8c0194ae; /* mov r15,r5 */
L8c0194ae: *stop=0x8c0194aeu;r[2]=rd(m,0x8c019648u,4);goto L8c0194b0; /* mov.l 0x8c019648,r2 */
L8c0194b0: *stop=0x8c0194b0u;r[6]=r[15];goto L8c0194b2; /* mov r15,r6 */
L8c0194b2: *stop=0x8c0194b2u;r[5]+=12u;goto L8c0194b4; /* add #0xc,r5 */
L8c0194b4: *stop=0x8c0194b4u;r[6]+=12u;goto L8c0194b6; /* add #0xc,r6 */
L8c0194b6: target=r[2];*pr=0x8c0194bau;*stop=0x8c0194b8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0194ba; /* jsr @r2 */
L8c0194ba: *stop=0x8c0194bau;r[3]=rd(m,0x8c01964cu,4);goto L8c0194bc; /* mov.l 0x8c01964c,r3 */
L8c0194bc: target=r[3];*pr=0x8c0194c0u;*stop=0x8c0194beu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0194c0; /* jsr @r3 */
L8c0194c0: *stop=0x8c0194c0u;r[0]=0x8c019650u;goto L8c0194c2; /* mova 0x8c019650,r0 */
L8c0194c2: *stop=0x8c0194c2u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c0194c4; /* mov.l @r14,r3 */
L8c0194c4: *stop=0x8c0194c4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto L8c0194c6; /* fmov.s @r0,fr4 */
L8c0194c6: *stop=0x8c0194c6u;r[0]=12u;goto L8c0194c8; /* mov #0xc,r0 */
L8c0194c8: *stop=0x8c0194c8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c0194ca; /* fmov.s @(r0,r15),fr3 */
L8c0194ca: *stop=0x8c0194cau;r[0]=24u;goto L8c0194cc; /* mov #0x18,r0 */
L8c0194cc: *stop=0x8c0194ccu;r[2]=r[3];goto L8c0194ce; /* mov r3,r2 */
L8c0194ce: *stop=0x8c0194ceu;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[2]);goto L8c0194d0; /* fmov.s @(r0,r2),fr2 */
L8c0194d0: *stop=0x8c0194d0u;r[0]=12u;goto L8c0194d2; /* mov #0xc,r0 */
L8c0194d2: *stop=0x8c0194d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0194d4; /* fadd fr3,fr2 */
L8c0194d4: *stop=0x8c0194d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0194d6; /* fadd fr4,fr2 */
L8c0194d6: *stop=0x8c0194d6u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[3]);goto L8c0194d8; /* fmov fr2,@(r0,r3) */
L8c0194d8: *stop=0x8c0194d8u;r[0]=16u;goto L8c0194da; /* mov #0x10,r0 */
L8c0194da: *stop=0x8c0194dau;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c0194dc; /* mov.l @r14,r3 */
L8c0194dc: *stop=0x8c0194dcu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c0194de; /* fmov.s @(r0,r15),fr3 */
L8c0194de: *stop=0x8c0194deu;r[0]=28u;goto L8c0194e0; /* mov #0x1c,r0 */
L8c0194e0: *stop=0x8c0194e0u;r[2]=r[3];goto L8c0194e2; /* mov r3,r2 */
L8c0194e2: *stop=0x8c0194e2u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[2]);goto L8c0194e4; /* fmov.s @(r0,r2),fr2 */
L8c0194e4: *stop=0x8c0194e4u;r[0]=16u;goto L8c0194e6; /* mov #0x10,r0 */
L8c0194e6: *stop=0x8c0194e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0194e8; /* fadd fr3,fr2 */
L8c0194e8: *stop=0x8c0194e8u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[3]);goto L8c0194ea; /* fmov fr2,@(r0,r3) */
L8c0194ea: *stop=0x8c0194eau;r[0]=20u;goto L8c0194ec; /* mov #0x14,r0 */
L8c0194ec: *stop=0x8c0194ecu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c0194ee; /* mov.l @r14,r3 */
L8c0194ee: *stop=0x8c0194eeu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c0194f0; /* fmov.s @(r0,r15),fr3 */
L8c0194f0: *stop=0x8c0194f0u;r[0]=32u;goto L8c0194f2; /* mov #0x20,r0 */
L8c0194f2: *stop=0x8c0194f2u;r[2]=r[3];goto L8c0194f4; /* mov r3,r2 */
L8c0194f4: *stop=0x8c0194f4u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[2]);goto L8c0194f6; /* fmov.s @(r0,r2),fr2 */
L8c0194f6: *stop=0x8c0194f6u;r[0]=20u;goto L8c0194f8; /* mov #0x14,r0 */
L8c0194f8: *stop=0x8c0194f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0194fa; /* fadd fr3,fr2 */
L8c0194fa: *stop=0x8c0194fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0194fc; /* fadd fr4,fr2 */
L8c0194fc: *stop=0x8c0194fcu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[3]);goto L8c0194fe; /* fmov fr2,@(r0,r3) */
L8c0194fe: *stop=0x8c0194feu;r[3]=rd(m,0x8c019654u,4);goto L8c019500; /* mov.l 0x8c019654,r3 */
L8c019500: *stop=0x8c019500u;r[2]=rd(m,0x8c019658u,4);goto L8c019502; /* mov.l 0x8c019658,r2 */
L8c019502: target=r[2];*pr=0x8c019506u;*stop=0x8c019504u;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019506; /* jsr @r2 */
L8c019506: *stop=0x8c019506u;r[0]=rd(m,0x8c019660u,4);goto L8c019508; /* mov.l 0x8c019660,r0 */
L8c019508: *stop=0x8c019508u;r[1]=0u;goto L8c01950a; /* mov #0x0,r1 */
L8c01950a: *stop=0x8c01950au;r[2]=rd(m,0x8c01965cu,4);goto L8c01950c; /* mov.l 0x8c01965c,r2 */
L8c01950c: *stop=0x8c01950cu;r[3]=r[1];goto L8c01950e; /* mov r1,r3 */
L8c01950e: *stop=0x8c01950eu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto L8c019510; /* mov.l r1,@r2 */
L8c019510: *stop=0x8c019510u;if(!valid_ram(r[0],4u))return -3;wr(m,r[0],r[1],4);goto L8c019512; /* mov.l r1,@r0 */
L8c019512: *stop=0x8c019512u;r[15]+=24u;goto L8c019514; /* add #0x18,r15 */
L8c019514: *stop=0x8c019514u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019516; /* lds.l @r15+,PR */
L8c019516: *stop=0x8c019516u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c019518; /* mov.l @r15+,r12 */
L8c019518: *stop=0x8c019518u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01951a; /* mov.l @r15+,r13 */
L8c01951a: *stop=0x8c01951au;if(*pr!=entry_pr)return -3;*stop=0x8c01951cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_position_8c019f4a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c019f4a;
L8c019f4a: *stop=0x8c019f4au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c019f4c; /* mov.l r14,@-r15 */
L8c019f4c: *stop=0x8c019f4cu;r[14]=rd(m,0x8c01a0c0u,4);goto L8c019f4e; /* mov.l 0x8c01a0c0,r14 */
L8c019f4e: *stop=0x8c019f4eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c019f50; /* mov.l r13,@-r15 */
L8c019f50: *stop=0x8c019f50u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c019f52; /* mov.l r12,@-r15 */
L8c019f52: *stop=0x8c019f52u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c019f54; /* sts.l PR,@-r15 */
L8c019f54: *stop=0x8c019f54u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c019f56; /* mov.l @r14,r3 */
L8c019f56: *stop=0x8c019f56u;if(!valid_ram(r[3]+12u,4u))return -3;r[2]=rd(m,r[3]+0xcu,4);goto L8c019f58; /* mov.l @(0xc,r3),r2 */
L8c019f58: *stop=0x8c019f58u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c019f5a; /* tst r2,r2 */
L8c019f5a: take=!(*sr&1u);*stop=0x8c019f5cu;r[13]=r[4];if(take)goto L8c019f62;goto L8c019f5e; /* bf/s 0x8c019f62 */
L8c019f5e: *stop=0x8c019f60u;goto L8c01a098; /* bra 0x8c01a098 */
L8c019f62: *stop=0x8c019f62u;if(!valid_ram(r[13],4u))return -3;r[1]=rd(m,r[13],4);goto L8c019f64; /* mov.l @r13,r1 */
L8c019f64: *stop=0x8c019f64u;r[2]=rd(m,0x8c01a0c4u,4);goto L8c019f66; /* mov.l 0x8c01a0c4,r2 */
L8c019f66: *stop=0x8c019f66u;if(!valid_ram(r[13]+4u,4u))return -3;r[0]=rd(m,r[13]+0x4u,4);goto L8c019f68; /* mov.l @(0x4,r13),r0 */
L8c019f68: *stop=0x8c019f68u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto L8c019f6a; /* mov.l r1,@r2 */
L8c019f6a: *stop=0x8c019f6au;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto L8c019f6c; /* mov.l r0,@(0x4,r2) */
L8c019f6c: *stop=0x8c019f6cu;if(!valid_ram(r[13]+8u,4u))return -3;r[1]=rd(m,r[13]+0x8u,4);goto L8c019f6e; /* mov.l @(0x8,r13),r1 */
L8c019f6e: *stop=0x8c019f6eu;if(!valid_ram(r[13]+12u,4u))return -3;r[0]=rd(m,r[13]+0xcu,4);goto L8c019f70; /* mov.l @(0xc,r13),r0 */
L8c019f70: *stop=0x8c019f70u;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto L8c019f72; /* mov.l r1,@(0x8,r2) */
L8c019f72: *stop=0x8c019f72u;if(!valid_ram(r[2]+12u,4u))return -3;wr(m,r[2]+0xcu,r[0],4);goto L8c019f74; /* mov.l r0,@(0xc,r2) */
L8c019f74: *stop=0x8c019f74u;if(!valid_ram(r[13]+16u,4u))return -3;r[1]=rd(m,r[13]+0x10u,4);goto L8c019f76; /* mov.l @(0x10,r13),r1 */
L8c019f76: *stop=0x8c019f76u;if(!valid_ram(r[13]+20u,4u))return -3;r[0]=rd(m,r[13]+0x14u,4);goto L8c019f78; /* mov.l @(0x14,r13),r0 */
L8c019f78: *stop=0x8c019f78u;if(!valid_ram(r[2]+16u,4u))return -3;wr(m,r[2]+0x10u,r[1],4);goto L8c019f7a; /* mov.l r1,@(0x10,r2) */
L8c019f7a: *stop=0x8c019f7au;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[0],4);goto L8c019f7c; /* mov.l r0,@(0x14,r2) */
L8c019f7c: *stop=0x8c019f7cu;if(!valid_ram(r[13]+24u,4u))return -3;r[1]=rd(m,r[13]+0x18u,4);goto L8c019f7e; /* mov.l @(0x18,r13),r1 */
L8c019f7e: *stop=0x8c019f7eu;if(!valid_ram(r[13]+28u,4u))return -3;r[0]=rd(m,r[13]+0x1cu,4);goto L8c019f80; /* mov.l @(0x1c,r13),r0 */
L8c019f80: *stop=0x8c019f80u;if(!valid_ram(r[2]+24u,4u))return -3;wr(m,r[2]+0x18u,r[1],4);goto L8c019f82; /* mov.l r1,@(0x18,r2) */
L8c019f82: *stop=0x8c019f82u;if(!valid_ram(r[2]+28u,4u))return -3;wr(m,r[2]+0x1cu,r[0],4);goto L8c019f84; /* mov.l r0,@(0x1c,r2) */
L8c019f84: *stop=0x8c019f84u;if(!valid_ram(r[13]+32u,4u))return -3;r[1]=rd(m,r[13]+0x20u,4);goto L8c019f86; /* mov.l @(0x20,r13),r1 */
L8c019f86: *stop=0x8c019f86u;if(!valid_ram(r[13]+36u,4u))return -3;r[0]=rd(m,r[13]+0x24u,4);goto L8c019f88; /* mov.l @(0x24,r13),r0 */
L8c019f88: *stop=0x8c019f88u;if(!valid_ram(r[2]+32u,4u))return -3;wr(m,r[2]+0x20u,r[1],4);goto L8c019f8a; /* mov.l r1,@(0x20,r2) */
L8c019f8a: *stop=0x8c019f8au;if(!valid_ram(r[2]+36u,4u))return -3;wr(m,r[2]+0x24u,r[0],4);goto L8c019f8c; /* mov.l r0,@(0x24,r2) */
L8c019f8c: *stop=0x8c019f8cu;if(!valid_ram(r[13]+40u,4u))return -3;r[1]=rd(m,r[13]+0x28u,4);goto L8c019f8e; /* mov.l @(0x28,r13),r1 */
L8c019f8e: *stop=0x8c019f8eu;if(!valid_ram(r[13]+44u,4u))return -3;r[0]=rd(m,r[13]+0x2cu,4);goto L8c019f90; /* mov.l @(0x2c,r13),r0 */
L8c019f90: *stop=0x8c019f90u;if(!valid_ram(r[2]+40u,4u))return -3;wr(m,r[2]+0x28u,r[1],4);goto L8c019f92; /* mov.l r1,@(0x28,r2) */
L8c019f92: *stop=0x8c019f92u;if(!valid_ram(r[2]+44u,4u))return -3;wr(m,r[2]+0x2cu,r[0],4);goto L8c019f94; /* mov.l r0,@(0x2c,r2) */
L8c019f94: *stop=0x8c019f94u;if(!valid_ram(r[13]+48u,4u))return -3;r[1]=rd(m,r[13]+0x30u,4);goto L8c019f96; /* mov.l @(0x30,r13),r1 */
L8c019f96: *stop=0x8c019f96u;if(!valid_ram(r[13]+52u,4u))return -3;r[0]=rd(m,r[13]+0x34u,4);goto L8c019f98; /* mov.l @(0x34,r13),r0 */
L8c019f98: *stop=0x8c019f98u;if(!valid_ram(r[2]+48u,4u))return -3;wr(m,r[2]+0x30u,r[1],4);goto L8c019f9a; /* mov.l r1,@(0x30,r2) */
L8c019f9a: *stop=0x8c019f9au;if(!valid_ram(r[2]+52u,4u))return -3;wr(m,r[2]+0x34u,r[0],4);goto L8c019f9c; /* mov.l r0,@(0x34,r2) */
L8c019f9c: *stop=0x8c019f9cu;if(!valid_ram(r[13]+56u,4u))return -3;r[1]=rd(m,r[13]+0x38u,4);goto L8c019f9e; /* mov.l @(0x38,r13),r1 */
L8c019f9e: *stop=0x8c019f9eu;if(!valid_ram(r[13]+60u,4u))return -3;r[0]=rd(m,r[13]+0x3cu,4);goto L8c019fa0; /* mov.l @(0x3c,r13),r0 */
L8c019fa0: *stop=0x8c019fa0u;if(!valid_ram(r[2]+56u,4u))return -3;wr(m,r[2]+0x38u,r[1],4);goto L8c019fa2; /* mov.l r1,@(0x38,r2) */
L8c019fa2: *stop=0x8c019fa2u;r[4]=rd(m,0x8c01a0c8u,4);goto L8c019fa4; /* mov.l 0x8c01a0c8,r4 */
L8c019fa4: *stop=0x8c019fa4u;if(!valid_ram(r[2]+60u,4u))return -3;wr(m,r[2]+0x3cu,r[0],4);goto L8c019fa6; /* mov.l r0,@(0x3c,r2) */
L8c019fa6: *stop=0x8c019fa6u;r[0]=32u;goto L8c019fa8; /* mov #0x20,r0 */
L8c019fa8: *stop=0x8c019fa8u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c019faa; /* fmov.s @(r0,r13),fr3 */
L8c019faa: *stop=0x8c019faau;r[0]=12u;goto L8c019fac; /* mov #0xc,r0 */
L8c019fac: *stop=0x8c019facu;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019fae; /* mov.l @r4,r3 */
L8c019fae: *stop=0x8c019faeu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[3]);goto L8c019fb0; /* fmov fr3,@(r0,r3) */
L8c019fb0: *stop=0x8c019fb0u;r[0]=36u;goto L8c019fb2; /* mov #0x24,r0 */
L8c019fb2: *stop=0x8c019fb2u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c019fb4; /* fmov.s @(r0,r13),fr3 */
L8c019fb4: *stop=0x8c019fb4u;r[0]=16u;goto L8c019fb6; /* mov #0x10,r0 */
L8c019fb6: *stop=0x8c019fb6u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019fb8; /* mov.l @r4,r3 */
L8c019fb8: *stop=0x8c019fb8u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[3]);goto L8c019fba; /* fmov fr3,@(r0,r3) */
L8c019fba: *stop=0x8c019fbau;r[0]=40u;goto L8c019fbc; /* mov #0x28,r0 */
L8c019fbc: *stop=0x8c019fbcu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c019fbe; /* fmov.s @(r0,r13),fr3 */
L8c019fbe: *stop=0x8c019fbeu;r[0]=20u;goto L8c019fc0; /* mov #0x14,r0 */
L8c019fc0: *stop=0x8c019fc0u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019fc2; /* mov.l @r4,r3 */
L8c019fc2: *stop=0x8c019fc2u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[3]);goto L8c019fc4; /* fmov fr3,@(r0,r3) */
L8c019fc4: *stop=0x8c019fc4u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);goto L8c019fc6; /* mov.l @r14,r4 */
L8c019fc6: *stop=0x8c019fc6u;r[0]=r[4];goto L8c019fc8; /* mov r4,r0 */
L8c019fc8: *stop=0x8c019fc8u;if(!valid_ram(r[0]+8u,1u))return -3;r[0]=sx(m,r[0]+0x8u,1);goto L8c019fca; /* mov.b @(0x8,r0),r0 */
L8c019fca: *stop=0x8c019fcau;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c019fcc; /* cmp/eq #0x5,r0 */
L8c019fcc: if(!(*sr&1u))goto L8c019fe4;goto L8c019fce; /* bf 0x8c019fe4 */
L8c019fce: *stop=0x8c019fceu;r[1]=rd(m,0x8c01a0d0u,4);goto L8c019fd0; /* mov.l 0x8c01a0d0,r1 */
L8c019fd0: *stop=0x8c019fd0u;r[4]=rd(m,0x8c01a0ccu,4);goto L8c019fd2; /* mov.l 0x8c01a0cc,r4 */
L8c019fd2: target=r[1];*pr=0x8c019fd6u;*stop=0x8c019fd4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019fd6; /* jsr @r1 */
L8c019fd6: *stop=0x8c019fd6u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c019fd8; /* mov.l @r14,r3 */
L8c019fd8: *stop=0x8c019fd8u;r[4]=rd(m,0x8c01a0ccu,4);goto L8c019fda; /* mov.l 0x8c01a0cc,r4 */
L8c019fda: *stop=0x8c019fdau;if(!valid_ram(r[3]+12u,4u))return -3;r[2]=rd(m,r[3]+0xcu,4);goto L8c019fdc; /* mov.l @(0xc,r3),r2 */
L8c019fdc: target=r[2];*pr=0x8c019fe0u;*stop=0x8c019fdeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019fe0; /* jsr @r2 */
L8c019fe0: *stop=0x8c019fe2u;goto L8c01a040; /* bra 0x8c01a040 */
L8c019fe4: *stop=0x8c019fe4u;if(!valid_ram(r[4]+11u,1u))return -3;r[0]=sx(m,r[4]+0xbu,1);goto L8c019fe6; /* mov.b @(0xb,r4),r0 */
L8c019fe6: *stop=0x8c019fe6u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c019fe8; /* tst #0x2,r0 */
L8c019fe8: if((*sr&1u))goto L8c01a024;goto L8c019fea; /* bt 0x8c01a024 */
L8c019fea: *stop=0x8c019feau;r[1]=rd(m,0x8c01a0d4u,4);goto L8c019fec; /* mov.l 0x8c01a0d4,r1 */
L8c019fec: *stop=0x8c019fecu;r[4]=rd(m,0x8c01a0ccu,4);goto L8c019fee; /* mov.l 0x8c01a0cc,r4 */
L8c019fee: *stop=0x8c019feeu;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c019ff0; /* mov.l @r1,r3 */
L8c019ff0: *stop=0x8c019ff0u;if(!valid_ram(r[1]+4u,4u))return -3;r[0]=rd(m,r[1]+0x4u,4);goto L8c019ff2; /* mov.l @(0x4,r1),r0 */
L8c019ff2: *stop=0x8c019ff2u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[3],4);goto L8c019ff4; /* mov.l r3,@r4 */
L8c019ff4: *stop=0x8c019ff4u;if(!valid_ram(r[4]+4u,4u))return -3;wr(m,r[4]+0x4u,r[0],4);goto L8c019ff6; /* mov.l r0,@(0x4,r4) */
L8c019ff6: *stop=0x8c019ff6u;if(!valid_ram(r[1]+8u,4u))return -3;r[3]=rd(m,r[1]+0x8u,4);goto L8c019ff8; /* mov.l @(0x8,r1),r3 */
L8c019ff8: *stop=0x8c019ff8u;if(!valid_ram(r[1]+12u,4u))return -3;r[0]=rd(m,r[1]+0xcu,4);goto L8c019ffa; /* mov.l @(0xc,r1),r0 */
L8c019ffa: *stop=0x8c019ffau;if(!valid_ram(r[4]+8u,4u))return -3;wr(m,r[4]+0x8u,r[3],4);goto L8c019ffc; /* mov.l r3,@(0x8,r4) */
L8c019ffc: *stop=0x8c019ffcu;if(!valid_ram(r[4]+12u,4u))return -3;wr(m,r[4]+0xcu,r[0],4);goto L8c019ffe; /* mov.l r0,@(0xc,r4) */
L8c019ffe: *stop=0x8c019ffeu;if(!valid_ram(r[1]+16u,4u))return -3;r[3]=rd(m,r[1]+0x10u,4);goto L8c01a000; /* mov.l @(0x10,r1),r3 */
L8c01a000: *stop=0x8c01a000u;if(!valid_ram(r[1]+20u,4u))return -3;r[0]=rd(m,r[1]+0x14u,4);goto L8c01a002; /* mov.l @(0x14,r1),r0 */
L8c01a002: *stop=0x8c01a002u;if(!valid_ram(r[4]+16u,4u))return -3;wr(m,r[4]+0x10u,r[3],4);goto L8c01a004; /* mov.l r3,@(0x10,r4) */
L8c01a004: *stop=0x8c01a004u;if(!valid_ram(r[4]+20u,4u))return -3;wr(m,r[4]+0x14u,r[0],4);goto L8c01a006; /* mov.l r0,@(0x14,r4) */
L8c01a006: *stop=0x8c01a006u;if(!valid_ram(r[1]+24u,4u))return -3;r[3]=rd(m,r[1]+0x18u,4);goto L8c01a008; /* mov.l @(0x18,r1),r3 */
L8c01a008: *stop=0x8c01a008u;if(!valid_ram(r[1]+28u,4u))return -3;r[0]=rd(m,r[1]+0x1cu,4);goto L8c01a00a; /* mov.l @(0x1c,r1),r0 */
L8c01a00a: *stop=0x8c01a00au;if(!valid_ram(r[4]+24u,4u))return -3;wr(m,r[4]+0x18u,r[3],4);goto L8c01a00c; /* mov.l r3,@(0x18,r4) */
L8c01a00c: *stop=0x8c01a00cu;if(!valid_ram(r[4]+28u,4u))return -3;wr(m,r[4]+0x1cu,r[0],4);goto L8c01a00e; /* mov.l r0,@(0x1c,r4) */
L8c01a00e: *stop=0x8c01a00eu;if(!valid_ram(r[1]+32u,4u))return -3;r[3]=rd(m,r[1]+0x20u,4);goto L8c01a010; /* mov.l @(0x20,r1),r3 */
L8c01a010: *stop=0x8c01a010u;if(!valid_ram(r[4]+32u,4u))return -3;wr(m,r[4]+0x20u,r[3],4);goto L8c01a012; /* mov.l r3,@(0x20,r4) */
L8c01a012: *stop=0x8c01a012u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a014; /* mov.l @r14,r3 */
L8c01a014: *stop=0x8c01a014u;if(!valid_ram(r[3]+20u,4u))return -3;r[0]=rd(m,r[3]+0x14u,4);goto L8c01a016; /* mov.l @(0x14,r3),r0 */
L8c01a016: *stop=0x8c01a016u;if(!valid_ram(r[4]+32u,4u))return -3;wr(m,r[4]+0x20u,r[0],4);goto L8c01a018; /* mov.l r0,@(0x20,r4) */
L8c01a018: *stop=0x8c01a018u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a01a; /* mov.l @r14,r3 */
L8c01a01a: *stop=0x8c01a01au;if(!valid_ram(r[3]+12u,4u))return -3;r[0]=rd(m,r[3]+0xcu,4);goto L8c01a01c; /* mov.l @(0xc,r3),r0 */
L8c01a01c: target=r[0];*pr=0x8c01a020u;*stop=0x8c01a01eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a020; /* jsr @r0 */
L8c01a020: *stop=0x8c01a022u;goto L8c01a040; /* bra 0x8c01a040 */
L8c01a024: *stop=0x8c01a024u;if(!valid_ram(r[4]+4u,2u))return -3;r[0]=sx(m,r[4]+0x4u,2);goto L8c01a026; /* mov.w @(0x4,r4),r0 */
L8c01a026: *stop=0x8c01a026u;r[3]=rd(m,0x8c01a0d8u,4);goto L8c01a028; /* mov.l 0x8c01a0d8,r3 */
L8c01a028: *stop=0x8c01a028u;r[0]<<=2;goto L8c01a02a; /* shll2 r0 */
L8c01a02a: *stop=0x8c01a02au;r[1]=rd(m,0x8c01a0dcu,4);goto L8c01a02c; /* mov.l 0x8c01a0dc,r1 */
L8c01a02c: *stop=0x8c01a02cu;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);goto L8c01a02e; /* mov.l @r3,r4 */
L8c01a02e: *stop=0x8c01a02eu;r[0]<<=2;goto L8c01a030; /* shll2 r0 */
L8c01a030: *stop=0x8c01a030u;r[0]<<=2;goto L8c01a032; /* shll2 r0 */
L8c01a032: target=r[1];*pr=0x8c01a036u;*stop=0x8c01a034u;r[4]+=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a036; /* jsr @r1 */
L8c01a036: *stop=0x8c01a036u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a038; /* mov.l @r14,r3 */
L8c01a038: *stop=0x8c01a038u;r[4]=rd(m,0x8c01a0ccu,4);goto L8c01a03a; /* mov.l 0x8c01a0cc,r4 */
L8c01a03a: *stop=0x8c01a03au;if(!valid_ram(r[3]+12u,4u))return -3;r[2]=rd(m,r[3]+0xcu,4);goto L8c01a03c; /* mov.l @(0xc,r3),r2 */
L8c01a03c: target=r[2];*pr=0x8c01a040u;*stop=0x8c01a03eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a040; /* jsr @r2 */
L8c01a040: *stop=0x8c01a040u;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c01a042; /* mov.l @r14,r0 */
L8c01a042: *stop=0x8c01a042u;if(!valid_ram(r[0]+7u,1u))return -3;r[0]=sx(m,r[0]+0x7u,1);goto L8c01a044; /* mov.b @(0x7,r0),r0 */
L8c01a044: *stop=0x8c01a044u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c01a046; /* cmp/eq #0x3,r0 */
L8c01a046: if((*sr&1u))goto L8c01a054;goto L8c01a048; /* bt 0x8c01a054 */
L8c01a048: *stop=0x8c01a048u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c01a04a; /* cmp/eq #0x2,r0 */
L8c01a04a: if((*sr&1u))goto L8c01a05c;goto L8c01a04c; /* bt 0x8c01a05c */
L8c01a04c: *stop=0x8c01a04cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c01a04e; /* cmp/eq #0x1,r0 */
L8c01a04e: if((*sr&1u))goto L8c01a064;goto L8c01a050; /* bt 0x8c01a064 */
L8c01a050: *stop=0x8c01a052u;goto L8c01a068; /* bra 0x8c01a068 */
L8c01a054: target=0x8c01a60cu;*pr=0x8c01a058u;*stop=0x8c01a056u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a058; /* bsr 0x8c01a60c */
L8c01a058: *stop=0x8c01a05au;goto L8c01a068; /* bra 0x8c01a068 */
L8c01a05c: target=0x8c01a680u;*pr=0x8c01a060u;*stop=0x8c01a05eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a060; /* bsr 0x8c01a680 */
L8c01a060: *stop=0x8c01a062u;goto L8c01a068; /* bra 0x8c01a068 */
L8c01a064: target=0x8c01a718u;*pr=0x8c01a068u;*stop=0x8c01a066u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a068; /* bsr 0x8c01a718 */
L8c01a068: *stop=0x8c01a068u;if(!valid_ram(r[14],4u))return -3;r[12]=rd(m,r[14],4);goto L8c01a06a; /* mov.l @r14,r12 */
L8c01a06a: *stop=0x8c01a06au;if(!valid_ram(r[12]+20u,4u))return -3;r[3]=rd(m,r[12]+0x14u,4);goto L8c01a06c; /* mov.l @(0x14,r12),r3 */
L8c01a06c: *stop=0x8c01a06cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c01a06e; /* tst r3,r3 */
L8c01a06e: if(!(*sr&1u))goto L8c01a076;goto L8c01a070; /* bf 0x8c01a076 */
L8c01a070: *stop=0x8c01a070u;if(!valid_ram(r[12]+11u,1u))return -3;r[0]=sx(m,r[12]+0xbu,1);goto L8c01a072; /* mov.b @(0xb,r12),r0 */
L8c01a072: *stop=0x8c01a072u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c01a074; /* tst #0x1,r0 */
L8c01a074: if(!(*sr&1u))goto L8c01a098;goto L8c01a076; /* bf 0x8c01a098 */
L8c01a076: *stop=0x8c01a076u;r[1]=rd(m,0x8c01a0e0u,4);goto L8c01a078; /* mov.l 0x8c01a0e0,r1 */
L8c01a078: *stop=0x8c01a078u;r[2]=60u;goto L8c01a07a; /* mov #0x3c,r2 */
L8c01a07a: *stop=0x8c01a07au;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c01a07c; /* mov.l @r1,r3 */
L8c01a07c: *stop=0x8c01a07cu;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c01a07e; /* cmp/hi r2,r3 */
L8c01a07e: if(!(*sr&1u))goto L8c01a098;goto L8c01a080; /* bf 0x8c01a098 */
L8c01a080: *stop=0x8c01a080u;r[0]=rd(m,0x8c01a0e8u,4);goto L8c01a082; /* mov.l 0x8c01a0e8,r0 */
L8c01a082: *stop=0x8c01a082u;r[1]=rd(m,0x8c01a0e4u,4);goto L8c01a084; /* mov.l 0x8c01a0e4,r1 */
L8c01a084: *stop=0x8c01a084u;if(!valid_ram(r[0],4u))return -3;r[5]=rd(m,r[0],4);goto L8c01a086; /* mov.l @r0,r5 */
L8c01a086: *stop=0x8c01a086u;if(!valid_ram(r[12]+16u,4u))return -3;r[3]=rd(m,r[12]+0x10u,4);goto L8c01a088; /* mov.l @(0x10,r12),r3 */
L8c01a088: *stop=0x8c01a088u;if(!valid_ram(r[1],4u))return -3;r[6]=rd(m,r[1],4);goto L8c01a08a; /* mov.l @r1,r6 */
L8c01a08a: target=r[3];*pr=0x8c01a08eu;*stop=0x8c01a08cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a08e; /* jsr @r3 */
L8c01a08e: *stop=0x8c01a08eu;r[2]=rd(m,0x8c01a0e4u,4);goto L8c01a090; /* mov.l 0x8c01a0e4,r2 */
L8c01a090: *stop=0x8c01a090u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c01a092; /* mov.l @r2,r3 */
L8c01a092: *stop=0x8c01a092u;if(!valid_ram(r[3]+20u,4u))return -3;r[1]=rd(m,r[3]+0x14u,4);goto L8c01a094; /* mov.l @(0x14,r3),r1 */
L8c01a094: *stop=0x8c01a094u;r[1]+=1u;goto L8c01a096; /* add #0x1,r1 */
L8c01a096: *stop=0x8c01a096u;if(!valid_ram(r[3]+20u,4u))return -3;wr(m,r[3]+0x14u,r[1],4);goto L8c01a098; /* mov.l r1,@(0x14,r3) */
L8c01a098: *stop=0x8c01a098u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01a09a; /* lds.l @r15+,PR */
L8c01a09a: *stop=0x8c01a09au;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c01a09c; /* mov.l @r15+,r12 */
L8c01a09c: *stop=0x8c01a09cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01a09e; /* mov.l @r15+,r13 */
L8c01a09e: *stop=0x8c01a09eu;if(*pr!=entry_pr)return -3;*stop=0x8c01a0a0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_position_8c01a100(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c01a100;
L8c01a100: *stop=0x8c01a100u;r[2]=rd(m,0x8c01a200u,4);goto L8c01a102; /* mov.l 0x8c01a200,r2 */
L8c01a102: *stop=0x8c01a102u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01a104; /* mov.l r14,@-r15 */
L8c01a104: *stop=0x8c01a104u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c01a106; /* mov.l r13,@-r15 */
L8c01a106: *stop=0x8c01a106u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01a108; /* sts.l PR,@-r15 */
L8c01a108: *stop=0x8c01a108u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c01a10a; /* mov.l @r2,r3 */
L8c01a10a: *stop=0x8c01a10au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c01a10c; /* tst r3,r3 */
L8c01a10c: take=(*sr&1u);*stop=0x8c01a10eu;r[13]=r[4];if(take)goto L8c01a140;goto L8c01a110; /* bt/s 0x8c01a140 */
L8c01a110: *stop=0x8c01a110u;r[3]=rd(m,0x8c01a204u,4);goto L8c01a112; /* mov.l 0x8c01a204,r3 */
L8c01a112: *stop=0x8c01a112u;r[2]=rd(m,0x8c01a20cu,4);goto L8c01a114; /* mov.l 0x8c01a20c,r2 */
L8c01a114: *stop=0x8c01a114u;r[1]=rd(m,0x8c01a208u,4);goto L8c01a116; /* mov.l 0x8c01a208,r1 */
L8c01a116: *stop=0x8c01a116u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[3]);goto L8c01a118; /* fmov.s @r3,fr4 */
L8c01a118: target=r[2];*pr=0x8c01a11cu;*stop=0x8c01a11au;if(!valid_ram(r[1],4u))return -3;r[4]=rd(m,r[1],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a11c; /* jsr @r2 */
L8c01a11c: *stop=0x8c01a11cu;r[3]=rd(m,0x8c01a210u,4);goto L8c01a11e; /* mov.l 0x8c01a210,r3 */
L8c01a11e: target=r[3];*pr=0x8c01a122u;*stop=0x8c01a120u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a122; /* jsr @r3 */
L8c01a122: *stop=0x8c01a122u;r[0]=r[0]&65535u;goto L8c01a124; /* extu.w r0,r0 */
L8c01a124: *stop=0x8c01a124u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c01a126; /* cmp/pz r0 */
L8c01a126: if(!(*sr&1u))goto L8c01a12c;goto L8c01a128; /* bf 0x8c01a12c */
L8c01a128: *stop=0x8c01a12au;r[0]>>=8;goto L8c01a132; /* bra 0x8c01a132 */
L8c01a12c: *stop=0x8c01a12cu;r[0]=~r[0];goto L8c01a12e; /* not r0,r0 */
L8c01a12e: *stop=0x8c01a12eu;r[0]>>=8;goto L8c01a130; /* shlr8 r0 */
L8c01a130: *stop=0x8c01a130u;r[0]=~r[0];goto L8c01a132; /* not r0,r0 */
L8c01a132: *stop=0x8c01a132u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c01a134; /* cmp/eq #0x20,r0 */
L8c01a134: if(!(*sr&1u))goto L8c01a1a2;goto L8c01a136; /* bf 0x8c01a1a2 */
L8c01a136: *stop=0x8c01a136u;r[1]=rd(m,0x8c01a214u,4);goto L8c01a138; /* mov.l 0x8c01a214,r1 */
L8c01a138: *stop=0x8c01a138u;r[2]=0u;goto L8c01a13a; /* mov #0x0,r2 */
L8c01a13a: *stop=0x8c01a13au;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c01a13c; /* mov.l @r1,r3 */
L8c01a13c: *stop=0x8c01a13eu;if(!valid_ram(r[3]+24u,4u))return -3;wr(m,r[3]+0x18u,r[2],4);goto L8c01a1a2; /* bra 0x8c01a1a2 */
L8c01a140: *stop=0x8c01a140u;r[3]=rd(m,0x8c01a21cu,4);goto L8c01a142; /* mov.l 0x8c01a21c,r3 */
L8c01a142: *stop=0x8c01a142u;r[4]=rd(m,0x8c01a218u,4);goto L8c01a144; /* mov.l 0x8c01a218,r4 */
L8c01a144: target=r[3];*pr=0x8c01a148u;*stop=0x8c01a146u;if(!valid_ram(r[13]+28u,4u))return -3;r[5]=rd(m,r[13]+0x1cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a148; /* jsr @r3 */
L8c01a148: *stop=0x8c01a148u;r[2]=rd(m,0x8c01a220u,4);goto L8c01a14a; /* mov.l 0x8c01a220,r2 */
L8c01a14a: *stop=0x8c01a14au;r[4]=rd(m,0x8c01a218u,4);goto L8c01a14c; /* mov.l 0x8c01a218,r4 */
L8c01a14c: target=r[2];*pr=0x8c01a150u;*stop=0x8c01a14eu;if(!valid_ram(r[13]+20u,4u))return -3;r[5]=rd(m,r[13]+0x14u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a150; /* jsr @r2 */
L8c01a150: *stop=0x8c01a150u;r[3]=rd(m,0x8c01a224u,4);goto L8c01a152; /* mov.l 0x8c01a224,r3 */
L8c01a152: *stop=0x8c01a152u;r[4]=rd(m,0x8c01a218u,4);goto L8c01a154; /* mov.l 0x8c01a218,r4 */
L8c01a154: target=r[3];*pr=0x8c01a158u;*stop=0x8c01a156u;if(!valid_ram(r[13]+24u,4u))return -3;r[5]=rd(m,r[13]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a158; /* jsr @r3 */
L8c01a158: *stop=0x8c01a158u;r[14]=rd(m,0x8c01a228u,4);goto L8c01a15a; /* mov.l 0x8c01a228,r14 */
L8c01a15a: *stop=0x8c01a15au;r[0]=4294967196u;goto L8c01a15c; /* mov #-0x64,r0 */
L8c01a15c: *stop=0x8c01a15cu;r[0]=r[0]&255u;goto L8c01a15e; /* extu.b r0,r0 */
L8c01a15e: *stop=0x8c01a15eu;r[4]=rd(m,0x8c01a218u,4);goto L8c01a160; /* mov.l 0x8c01a218,r4 */
L8c01a160: *stop=0x8c01a160u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c01a162; /* mov.l @r14,r2 */
L8c01a162: *stop=0x8c01a162u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[2]);goto L8c01a164; /* fmov.s @(r0,r2),fr3 */
L8c01a164: *stop=0x8c01a164u;r[0]=40u;goto L8c01a166; /* mov #0x28,r0 */
L8c01a166: *stop=0x8c01a166u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[13]);goto L8c01a168; /* fmov.s @(r0,r13),fr6 */
L8c01a168: *stop=0x8c01a168u;r[0]+=112u;goto L8c01a16a; /* add #0x70,r0 */
L8c01a16a: *stop=0x8c01a16au;r[3]=r[2];goto L8c01a16c; /* mov r2,r3 */
L8c01a16c: *stop=0x8c01a16cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c01a16e; /* fadd fr3,fr6 */
L8c01a16e: *stop=0x8c01a16eu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[3]);goto L8c01a170; /* fmov.s @(r0,r3),fr3 */
L8c01a170: *stop=0x8c01a170u;r[0]=36u;goto L8c01a172; /* mov #0x24,r0 */
L8c01a172: *stop=0x8c01a172u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[13]);goto L8c01a174; /* fmov.s @(r0,r13),fr5 */
L8c01a174: *stop=0x8c01a174u;r[0]+=112u;goto L8c01a176; /* add #0x70,r0 */
L8c01a176: *stop=0x8c01a176u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c01a178; /* fadd fr3,fr5 */
L8c01a178: *stop=0x8c01a178u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[3]);goto L8c01a17a; /* fmov.s @(r0,r3),fr3 */
L8c01a17a: *stop=0x8c01a17au;r[0]=32u;goto L8c01a17c; /* mov #0x20,r0 */
L8c01a17c: *stop=0x8c01a17cu;r[3]=rd(m,0x8c01a22cu,4);goto L8c01a17e; /* mov.l 0x8c01a22c,r3 */
L8c01a17e: *stop=0x8c01a17eu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto L8c01a180; /* fmov.s @(r0,r13),fr4 */
L8c01a180: target=r[3];*pr=0x8c01a184u;*stop=0x8c01a182u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a184; /* jsr @r3 */
L8c01a184: *stop=0x8c01a184u;r[2]=rd(m,0x8c01a230u,4);goto L8c01a186; /* mov.l 0x8c01a230,r2 */
L8c01a186: *stop=0x8c01a186u;r[4]=rd(m,0x8c01a218u,4);goto L8c01a188; /* mov.l 0x8c01a218,r4 */
L8c01a188: target=r[2];*pr=0x8c01a18cu;*stop=0x8c01a18au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a18c; /* jsr @r2 */
L8c01a18c: *stop=0x8c01a18cu;r[0]=4294967188u;goto L8c01a18e; /* mov #-0x6c,r0 */
L8c01a18e: *stop=0x8c01a18eu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a190; /* mov.l @r14,r3 */
L8c01a190: *stop=0x8c01a190u;r[0]=r[0]&255u;goto L8c01a192; /* extu.b r0,r0 */
L8c01a192: *stop=0x8c01a192u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c01a194; /* fldi0 fr4 */
L8c01a194: *stop=0x8c01a194u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[3]);goto L8c01a196; /* fmov fr4,@(r0,r3) */
L8c01a196: *stop=0x8c01a196u;r[0]+=4u;goto L8c01a198; /* add #0x4,r0 */
L8c01a198: *stop=0x8c01a198u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a19a; /* mov.l @r14,r3 */
L8c01a19a: *stop=0x8c01a19au;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[3]);goto L8c01a19c; /* fmov fr4,@(r0,r3) */
L8c01a19c: *stop=0x8c01a19cu;r[0]+=4u;goto L8c01a19e; /* add #0x4,r0 */
L8c01a19e: *stop=0x8c01a19eu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a1a0; /* mov.l @r14,r3 */
L8c01a1a0: *stop=0x8c01a1a0u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[3]);goto L8c01a1a2; /* fmov fr4,@(r0,r3) */
L8c01a1a2: *stop=0x8c01a1a2u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01a1a4; /* lds.l @r15+,PR */
L8c01a1a4: *stop=0x8c01a1a4u;r[3]=rd(m,0x8c01a234u,4);goto L8c01a1a6; /* mov.l 0x8c01a234,r3 */
L8c01a1a6: *stop=0x8c01a1a6u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01a1a8; /* mov.l @r15+,r13 */
L8c01a1a8: target=r[3];*stop=0x8c01a1aau;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_camera_position_8c0130f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0130f8;
L8c0130f8: *stop=0x8c0130f8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0130fa; /* mov.l r14,@-r15 */
L8c0130fa: *stop=0x8c0130fau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c0130fc; /* sts.l PR,@-r15 */
L8c0130fc: *stop=0x8c0130fcu;r[15]+=4294967284u;goto L8c0130fe; /* add #-0xc,r15 */
L8c0130fe: *stop=0x8c0130feu;r[3]=rd(m,0x8c013210u,4);goto L8c013100; /* mov.l 0x8c013210,r3 */
L8c013100: target=r[3];*pr=0x8c013104u;*stop=0x8c013102u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c013104; /* jsr @r3 */
L8c013104: *stop=0x8c013104u;r[2]=rd(m,0x8c013214u,4);goto L8c013106; /* mov.l 0x8c013214,r2 */
L8c013106: target=r[2];*pr=0x8c01310au;*stop=0x8c013108u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01310a; /* jsr @r2 */
L8c01310a: *stop=0x8c01310au;r[2]=rd(m,0x8c013218u,4);goto L8c01310c; /* mov.l 0x8c013218,r2 */
L8c01310c: *stop=0x8c01310cu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c01310e; /* mov.l @r2,r3 */
L8c01310e: *stop=0x8c01310eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c013110; /* tst r3,r3 */
L8c013110: if((*sr&1u))goto L8c01311a;goto L8c013112; /* bt 0x8c01311a */
L8c013112: *stop=0x8c013112u;r[3]=rd(m,0x8c01321cu,4);goto L8c013114; /* mov.l 0x8c01321c,r3 */
L8c013114: *stop=0x8c013114u;if(!valid_ram(r[2],4u))return -3;r[5]=rd(m,r[2],4);goto L8c013116; /* mov.l @r2,r5 */
L8c013116: target=r[3];*pr=0x8c01311au;*stop=0x8c013118u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01311a; /* jsr @r3 */
L8c01311a: *stop=0x8c01311au;r[2]=rd(m,0x8c013220u,4);goto L8c01311c; /* mov.l 0x8c013220,r2 */
L8c01311c: *stop=0x8c01311cu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c01311e; /* mov.l @r2,r3 */
L8c01311e: *stop=0x8c01311eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c013120; /* tst r3,r3 */
L8c013120: if((*sr&1u))goto L8c01312a;goto L8c013122; /* bt 0x8c01312a */
L8c013122: *stop=0x8c013122u;r[3]=rd(m,0x8c013224u,4);goto L8c013124; /* mov.l 0x8c013224,r3 */
L8c013124: *stop=0x8c013124u;if(!valid_ram(r[2],4u))return -3;r[5]=rd(m,r[2],4);goto L8c013126; /* mov.l @r2,r5 */
L8c013126: target=r[3];*pr=0x8c01312au;*stop=0x8c013128u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01312a; /* jsr @r3 */
L8c01312a: *stop=0x8c01312au;r[3]=rd(m,0x8c013230u,4);goto L8c01312c; /* mov.l 0x8c013230,r3 */
L8c01312c: *stop=0x8c01312cu;r[5]=rd(m,0x8c01322cu,4);goto L8c01312e; /* mov.l 0x8c01322c,r5 */
L8c01312e: *stop=0x8c01312eu;r[6]=rd(m,0x8c013228u,4);goto L8c013130; /* mov.l 0x8c013228,r6 */
L8c013130: target=r[3];*pr=0x8c013134u;*stop=0x8c013132u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c013134; /* jsr @r3 */
L8c013134: *stop=0x8c013134u;r[2]=rd(m,0x8c013234u,4);goto L8c013136; /* mov.l 0x8c013234,r2 */
L8c013136: target=r[2];*pr=0x8c01313au;*stop=0x8c013138u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01313a; /* jsr @r2 */
L8c01313a: *stop=0x8c01313au;r[3]=rd(m,0x8c013210u,4);goto L8c01313c; /* mov.l 0x8c013210,r3 */
L8c01313c: target=r[3];*pr=0x8c013140u;*stop=0x8c01313eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c013140; /* jsr @r3 */
L8c013140: *stop=0x8c013140u;r[2]=rd(m,0x8c013230u,4);goto L8c013142; /* mov.l 0x8c013230,r2 */
L8c013142: *stop=0x8c013142u;r[6]=r[15];goto L8c013144; /* mov r15,r6 */
L8c013144: *stop=0x8c013144u;r[5]=rd(m,0x8c013228u,4);goto L8c013146; /* mov.l 0x8c013228,r5 */
L8c013146: target=r[2];*pr=0x8c01314au;*stop=0x8c013148u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01314a; /* jsr @r2 */
L8c01314a: *stop=0x8c01314au;r[3]=rd(m,0x8c013234u,4);goto L8c01314c; /* mov.l 0x8c013234,r3 */
L8c01314c: target=r[3];*pr=0x8c013150u;*stop=0x8c01314eu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c013150; /* jsr @r3 */
L8c013150: *stop=0x8c013150u;r[0]=8u;goto L8c013152; /* mov #0x8,r0 */
L8c013152: *stop=0x8c013152u;r[3]=rd(m,0x8c013238u,4);goto L8c013154; /* mov.l 0x8c013238,r3 */
L8c013154: *stop=0x8c013154u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[15]);goto L8c013156; /* fmov.s @(r0,r15),fr6 */
L8c013156: *stop=0x8c013156u;r[0]=4u;goto L8c013158; /* mov #0x4,r0 */
L8c013158: *stop=0x8c013158u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[15]);goto L8c01315a; /* fmov.s @(r0,r15),fr5 */
L8c01315a: target=r[3];*pr=0x8c01315eu;*stop=0x8c01315cu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[15]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01315e; /* jsr @r3 */
L8c01315e: *stop=0x8c01315eu;r[0]=8u;goto L8c013160; /* mov #0x8,r0 */
L8c013160: *stop=0x8c013160u;r[3]=rd(m,0x8c01323cu,4);goto L8c013162; /* mov.l 0x8c01323c,r3 */
L8c013162: *stop=0x8c013162u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[15]);goto L8c013164; /* fmov.s @(r0,r15),fr6 */
L8c013164: *stop=0x8c013164u;r[0]=4u;goto L8c013166; /* mov #0x4,r0 */
L8c013166: *stop=0x8c013166u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[15]);goto L8c013168; /* fmov.s @(r0,r15),fr5 */
L8c013168: target=r[3];*pr=0x8c01316cu;*stop=0x8c01316au;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[15]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01316c; /* jsr @r3 */
L8c01316c: *stop=0x8c01316cu;r[3]=rd(m,0x8c01320cu,4);goto L8c01316e; /* mov.l 0x8c01320c,r3 */
L8c01316e: *stop=0x8c01316eu;r[0]=0x8c013240u;goto L8c013170; /* mova 0x8c013240,r0 */
L8c013170: *stop=0x8c013170u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c013172; /* fmov.s @r0,fr3 */
L8c013172: *stop=0x8c013172u;r[0]=0x8c013244u;goto L8c013174; /* mova 0x8c013244,r0 */
L8c013174: *stop=0x8c013174u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[3]);goto L8c013176; /* fmov.s @r3,fr2 */
L8c013176: *stop=0x8c013176u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto L8c013178; /* fmov.s @r0,fr1 */
L8c013178: *stop=0x8c013178u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c01317a; /* fadd fr3,fr2 */
L8c01317a: *stop=0x8c01317au;r[2]=rd(m,0x8c013248u,4);goto L8c01317c; /* mov.l 0x8c013248,r2 */
L8c01317c: *stop=0x8c01317cu;r[3]=rd(m,0x8c013208u,4);goto L8c01317e; /* mov.l 0x8c013208,r3 */
L8c01317e: *stop=0x8c01317eu;fcopy(f,5u,2u);goto L8c013180; /* fmov fr2,fr5 */
L8c013180: *stop=0x8c013180u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[1],&f->fpscr,&fr[5]);if(status)return status;}goto L8c013182; /* fmul fr1,fr5 */
L8c013182: target=r[2];*pr=0x8c013186u;*stop=0x8c013184u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[3]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c013186; /* jsr @r2 */
L8c013186: *stop=0x8c013186u;r[3]=rd(m,0x8c01324cu,4);goto L8c013188; /* mov.l 0x8c01324c,r3 */
L8c013188: *stop=0x8c013188u;r[1]=rd(m,0x8c013200u,4);goto L8c01318a; /* mov.l 0x8c013200,r1 */
L8c01318a: *stop=0x8c01318au;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[3]);goto L8c01318c; /* fmov.s @r3,fr5 */
L8c01318c: *stop=0x8c01318cu;r[3]=rd(m,0x8c013254u,4);goto L8c01318e; /* mov.l 0x8c013254,r3 */
L8c01318e: *stop=0x8c01318eu;r[2]=rd(m,0x8c013250u,4);goto L8c013190; /* mov.l 0x8c013250,r2 */
L8c013190: *stop=0x8c013190u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[1]);goto L8c013192; /* fmov.s @r1,fr6 */
L8c013192: target=r[3];*pr=0x8c013196u;*stop=0x8c013194u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[2]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c013196; /* jsr @r3 */
L8c013196: *stop=0x8c013196u;r[3]=rd(m,0x8c013208u,4);goto L8c013198; /* mov.l 0x8c013208,r3 */
L8c013198: *stop=0x8c013198u;r[1]=rd(m,0x8c01320cu,4);goto L8c01319a; /* mov.l 0x8c01320c,r1 */
L8c01319a: *stop=0x8c01319au;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[3]);goto L8c01319c; /* fmov.s @r3,fr5 */
L8c01319c: *stop=0x8c01319cu;r[4]=rd(m,0x8c013258u,4);goto L8c01319e; /* mov.l 0x8c013258,r4 */
L8c01319e: *stop=0x8c01319eu;r[3]=rd(m,0x8c01325cu,4);goto L8c0131a0; /* mov.l 0x8c01325c,r3 */
L8c0131a0: *stop=0x8c0131a0u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[1]);goto L8c0131a2; /* fmov.s @r1,fr6 */
L8c0131a2: *stop=0x8c0131a2u;r[2]=rd(m,0x8c013204u,4);goto L8c0131a4; /* mov.l 0x8c013204,r2 */
L8c0131a4: target=r[3];*pr=0x8c0131a8u;*stop=0x8c0131a6u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[2]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0131a8; /* jsr @r3 */
L8c0131a8: *stop=0x8c0131a8u;r[3]=rd(m,0x8c01324cu,4);goto L8c0131aa; /* mov.l 0x8c01324c,r3 */
L8c0131aa: *stop=0x8c0131aau;r[2]=rd(m,0x8c013200u,4);goto L8c0131ac; /* mov.l 0x8c013200,r2 */
L8c0131ac: *stop=0x8c0131acu;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[3]);goto L8c0131ae; /* fmov.s @r3,fr5 */
L8c0131ae: *stop=0x8c0131aeu;r[4]=rd(m,0x8c013258u,4);goto L8c0131b0; /* mov.l 0x8c013258,r4 */
L8c0131b0: *stop=0x8c0131b0u;r[3]=rd(m,0x8c013260u,4);goto L8c0131b2; /* mov.l 0x8c013260,r3 */
L8c0131b2: *stop=0x8c0131b2u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[2]);goto L8c0131b4; /* fmov.s @r2,fr6 */
L8c0131b4: *stop=0x8c0131b4u;r[1]=rd(m,0x8c013250u,4);goto L8c0131b6; /* mov.l 0x8c013250,r1 */
L8c0131b6: target=r[3];*pr=0x8c0131bau;*stop=0x8c0131b8u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[1]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0131ba; /* jsr @r3 */
L8c0131ba: *stop=0x8c0131bau;r[5]=11u;goto L8c0131bc; /* mov #0xb,r5 */
L8c0131bc: *stop=0x8c0131bcu;r[4]=rd(m,0x8c013258u,4);goto L8c0131be; /* mov.l 0x8c013258,r4 */
L8c0131be: *stop=0x8c0131beu;r[6]=50u;goto L8c0131c0; /* mov #0x32,r6 */
L8c0131c0: *stop=0x8c0131c0u;r[2]=rd(m,0x8c013264u,4);goto L8c0131c2; /* mov.l 0x8c013264,r2 */
L8c0131c2: *stop=0x8c0131c2u;r[5]<<=8;goto L8c0131c4; /* shll8 r5 */
L8c0131c4: *stop=0x8c0131c4u;r[6]<<=8;goto L8c0131c6; /* shll8 r6 */
L8c0131c6: *stop=0x8c0131c6u;r[6]+=4294967239u;goto L8c0131c8; /* add #-0x39,r6 */
L8c0131c8: *stop=0x8c0131c8u;r[5]+=4294967210u;goto L8c0131ca; /* add #-0x56,r5 */
L8c0131ca: target=r[2];*pr=0x8c0131ceu;*stop=0x8c0131ccu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0131ce; /* jsr @r2 */
L8c0131ce: *stop=0x8c0131ceu;r[14]=rd(m,0x8c013228u,4);goto L8c0131d0; /* mov.l 0x8c013228,r14 */
L8c0131d0: *stop=0x8c0131d0u;r[0]=8u;goto L8c0131d2; /* mov #0x8,r0 */
L8c0131d2: *stop=0x8c0131d2u;r[3]=rd(m,0x8c013268u,4);goto L8c0131d4; /* mov.l 0x8c013268,r3 */
L8c0131d4: *stop=0x8c0131d4u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[14]);goto L8c0131d6; /* fmov.s @(r0,r14),fr6 */
L8c0131d6: *stop=0x8c0131d6u;r[0]=4u;goto L8c0131d8; /* mov #0x4,r0 */
L8c0131d8: *stop=0x8c0131d8u;r[4]=rd(m,0x8c013258u,4);goto L8c0131da; /* mov.l 0x8c013258,r4 */
L8c0131da: *stop=0x8c0131dau;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto L8c0131dc; /* fmov.s @(r0,r14),fr5 */
L8c0131dc: target=r[3];*pr=0x8c0131e0u;*stop=0x8c0131deu;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[14]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0131e0; /* jsr @r3 */
L8c0131e0: *stop=0x8c0131e0u;r[2]=rd(m,0x8c01326cu,4);goto L8c0131e2; /* mov.l 0x8c01326c,r2 */
L8c0131e2: target=r[2];*pr=0x8c0131e6u;*stop=0x8c0131e4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0131e6; /* jsr @r2 */
L8c0131e6: *stop=0x8c0131e6u;r[15]+=12u;goto L8c0131e8; /* add #0xc,r15 */
L8c0131e8: *stop=0x8c0131e8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c0131ea; /* lds.l @r15+,PR */
L8c0131ea: *stop=0x8c0131eau;if(*pr!=entry_pr)return -3;*stop=0x8c0131ecu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_position_8c01a680(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01a680;
L8c01a680: *stop=0x8c01a680u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01a682; /* mov.l r14,@-r15 */
L8c01a682: *stop=0x8c01a682u;r[0]=12u;goto L8c01a684; /* mov #0xc,r0 */
L8c01a684: *stop=0x8c01a684u;r[14]=rd(m,0x8c01a8a0u,4);goto L8c01a686; /* mov.l 0x8c01a8a0,r14 */
L8c01a686: *stop=0x8c01a686u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c01a688; /* mov.l r13,@-r15 */
L8c01a688: *stop=0x8c01a688u;r[13]=r[4];goto L8c01a68a; /* mov r4,r13 */
L8c01a68a: *stop=0x8c01a68au;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c01a68c; /* fmov.s fr15,@-r15 */
L8c01a68c: *stop=0x8c01a68cu;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto L8c01a68e; /* fmov.s fr14,@-r15 */
L8c01a68e: *stop=0x8c01a68eu;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[15]);goto L8c01a690; /* fmov.s fr13,@-r15 */
L8c01a690: *stop=0x8c01a690u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01a692; /* sts.l PR,@-r15 */
L8c01a692: *stop=0x8c01a692u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a694; /* mov.l @r14,r3 */
L8c01a694: *stop=0x8c01a694u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[3]);goto L8c01a696; /* fmov.s @(r0,r3),fr3 */
L8c01a696: *stop=0x8c01a696u;r[0]=32u;goto L8c01a698; /* mov #0x20,r0 */
L8c01a698: *stop=0x8c01a698u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[13]);goto L8c01a69a; /* fmov fr3,@(r0,r13) */
L8c01a69a: *stop=0x8c01a69au;r[0]=16u;goto L8c01a69c; /* mov #0x10,r0 */
L8c01a69c: *stop=0x8c01a69cu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a69e; /* mov.l @r14,r3 */
L8c01a69e: *stop=0x8c01a69eu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[3]);goto L8c01a6a0; /* fmov.s @(r0,r3),fr3 */
L8c01a6a0: *stop=0x8c01a6a0u;r[0]=36u;goto L8c01a6a2; /* mov #0x24,r0 */
L8c01a6a2: *stop=0x8c01a6a2u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[13]);goto L8c01a6a4; /* fmov fr3,@(r0,r13) */
L8c01a6a4: *stop=0x8c01a6a4u;r[0]=20u;goto L8c01a6a6; /* mov #0x14,r0 */
L8c01a6a6: *stop=0x8c01a6a6u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a6a8; /* mov.l @r14,r3 */
L8c01a6a8: *stop=0x8c01a6a8u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[3]);goto L8c01a6aa; /* fmov.s @(r0,r3),fr3 */
L8c01a6aa: *stop=0x8c01a6aau;r[0]=40u;goto L8c01a6ac; /* mov #0x28,r0 */
L8c01a6ac: *stop=0x8c01a6acu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[13]);goto L8c01a6ae; /* fmov fr3,@(r0,r13) */
L8c01a6ae: *stop=0x8c01a6aeu;r[0]=24u;goto L8c01a6b0; /* mov #0x18,r0 */
L8c01a6b0: *stop=0x8c01a6b0u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a6b2; /* mov.l @r14,r3 */
L8c01a6b2: *stop=0x8c01a6b2u;r[2]=r[3];goto L8c01a6b4; /* mov r3,r2 */
L8c01a6b4: *stop=0x8c01a6b4u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[2]);goto L8c01a6b6; /* fmov.s @(r0,r2),fr3 */
L8c01a6b6: *stop=0x8c01a6b6u;r[0]=12u;goto L8c01a6b8; /* mov #0xc,r0 */
L8c01a6b8: *stop=0x8c01a6b8u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]+r[3]);goto L8c01a6ba; /* fmov.s @(r0,r3),fr15 */
L8c01a6ba: *stop=0x8c01a6bau;r[0]=32u;goto L8c01a6bc; /* mov #0x20,r0 */
L8c01a6bc: *stop=0x8c01a6bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c01a6be; /* fsub fr3,fr15 */
L8c01a6be: *stop=0x8c01a6beu;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[2]);goto L8c01a6c0; /* fmov.s @(r0,r2),fr3 */
L8c01a6c0: *stop=0x8c01a6c0u;r[0]=20u;goto L8c01a6c2; /* mov #0x14,r0 */
L8c01a6c2: *stop=0x8c01a6c2u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]+r[3]);goto L8c01a6c4; /* fmov.s @(r0,r3),fr14 */
L8c01a6c4: *stop=0x8c01a6c4u;r[0]=0x8c01a8a4u;goto L8c01a6c6; /* mova 0x8c01a8a4,r0 */
L8c01a6c6: *stop=0x8c01a6c6u;r[3]=rd(m,0x8c01a8a8u,4);goto L8c01a6c8; /* mov.l 0x8c01a8a8,r3 */
L8c01a6c8: *stop=0x8c01a6c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],1);if(status)return status;}goto L8c01a6ca; /* fsub fr3,fr14 */
L8c01a6ca: *stop=0x8c01a6cau;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[0]);goto L8c01a6cc; /* fmov.s @r0,fr13 */
L8c01a6cc: *stop=0x8c01a6ccu;fcopy(f,5u,14u);goto L8c01a6ce; /* fmov fr14,fr5 */
L8c01a6ce: target=r[3];*pr=0x8c01a6d2u;*stop=0x8c01a6d0u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a6d2; /* jsr @r3 */
L8c01a6d2: *stop=0x8c01a6d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[13],&f->fpscr,&fr[0]);if(status)return status;}goto L8c01a6d4; /* fmul fr13,fr0 */
L8c01a6d4: *stop=0x8c01a6d4u;fcopy(f,3u,14u);goto L8c01a6d6; /* fmov fr14,fr3 */
L8c01a6d6: *stop=0x8c01a6d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c01a6d8; /* fmul fr14,fr3 */
L8c01a6d8: *stop=0x8c01a6d8u;r[3]=rd(m,0x8c01a8acu,4);goto L8c01a6da; /* mov.l 0x8c01a8ac,r3 */
L8c01a6da: *stop=0x8c01a6dau;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c01a6dc; /* ftrc fr0,FPUL */
L8c01a6dc: *stop=0x8c01a6dcu;fcopy(f,0u,15u);goto L8c01a6de; /* fmov fr15,fr0 */
L8c01a6de: *stop=0x8c01a6deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[15],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c01a6e0; /* fmac fr0,fr15,fr3 */
L8c01a6e0: *stop=0x8c01a6e0u;r[2]=*fpul;goto L8c01a6e2; /* sts FPUL,r2 */
L8c01a6e2: *stop=0x8c01a6e2u;if(!valid_ram(r[13]+24u,4u))return -3;wr(m,r[13]+0x18u,r[2],4);goto L8c01a6e4; /* mov.l r2,@(0x18,r13) */
L8c01a6e4: target=r[3];*pr=0x8c01a6e8u;*stop=0x8c01a6e6u;fcopy(f,4u,3u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a6e8; /* jsr @r3 */
L8c01a6e8: *stop=0x8c01a6e8u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c01a6ea; /* mov.l @r14,r2 */
L8c01a6ea: *stop=0x8c01a6eau;r[0]=16u;goto L8c01a6ec; /* mov #0x10,r0 */
L8c01a6ec: *stop=0x8c01a6ecu;fcopy(f,5u,0u);goto L8c01a6ee; /* fmov fr0,fr5 */
L8c01a6ee: *stop=0x8c01a6eeu;r[3]=r[2];goto L8c01a6f0; /* mov r2,r3 */
L8c01a6f0: *stop=0x8c01a6f0u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[3]);goto L8c01a6f2; /* fmov.s @(r0,r3),fr3 */
L8c01a6f2: *stop=0x8c01a6f2u;r[0]=28u;goto L8c01a6f4; /* mov #0x1c,r0 */
L8c01a6f4: *stop=0x8c01a6f4u;r[3]=rd(m,0x8c01a8a8u,4);goto L8c01a6f6; /* mov.l 0x8c01a8a8,r3 */
L8c01a6f6: *stop=0x8c01a6f6u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[2]);goto L8c01a6f8; /* fmov.s @(r0,r2),fr4 */
L8c01a6f8: target=r[3];*pr=0x8c01a6fcu;*stop=0x8c01a6fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a6fc; /* jsr @r3 */
L8c01a6fc: *stop=0x8c01a6fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[13],&f->fpscr,&fr[0]);if(status)return status;}goto L8c01a6fe; /* fmul fr13,fr0 */
L8c01a6fe: *stop=0x8c01a6feu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c01a700; /* ftrc fr0,FPUL */
L8c01a700: *stop=0x8c01a700u;r[2]=*fpul;goto L8c01a702; /* sts FPUL,r2 */
L8c01a702: *stop=0x8c01a702u;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[2],4);goto L8c01a704; /* mov.l r2,@(0x14,r13) */
L8c01a704: *stop=0x8c01a704u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a706; /* mov.l @r14,r3 */
L8c01a706: *stop=0x8c01a706u;if(!valid_ram(r[3]+44u,4u))return -3;r[2]=rd(m,r[3]+0x2cu,4);goto L8c01a708; /* mov.l @(0x2c,r3),r2 */
L8c01a708: *stop=0x8c01a708u;if(!valid_ram(r[13]+28u,4u))return -3;wr(m,r[13]+0x1cu,r[2],4);goto L8c01a70a; /* mov.l r2,@(0x1c,r13) */
L8c01a70a: *stop=0x8c01a70au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01a70c; /* lds.l @r15+,PR */
L8c01a70c: *stop=0x8c01a70cu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01a70e; /* fmov.s @r15+,fr13 */
L8c01a70e: *stop=0x8c01a70eu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01a710; /* fmov.s @r15+,fr14 */
L8c01a710: *stop=0x8c01a710u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01a712; /* fmov.s @r15+,fr15 */
L8c01a712: *stop=0x8c01a712u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01a714; /* mov.l @r15+,r13 */
L8c01a714: *stop=0x8c01a714u;if(*pr!=entry_pr)return -3;*stop=0x8c01a716u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_position_8c6404c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6404c8;
L8c6404c8: *stop=0x8c6404c8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6404ca; /* tst r4,r4 */
L8c6404ca: *stop=0x8c6404cau;r[0]=116u;goto L8c6404cc; /* mov #0x74,r0 */
L8c6404cc: if(!(*sr&1u))goto L8c6404d2;goto L8c6404ce; /* bf 0x8c6404d2 */
L8c6404ce: *stop=0x8c6404ceu;r[2]=rd(m,0x8c6404dcu,4);goto L8c6404d0; /* mov.l 0x8c6404dc,r2 */
L8c6404d0: *stop=0x8c6404d0u;if(!valid_ram(r[2],4u))return -3;r[4]=rd(m,r[2],4);goto L8c6404d2; /* mov.l @r2,r4 */
L8c6404d2: *stop=0x8c6404d2u;if(!valid_ram(r[0]+r[4],4u))return -3;r[3]=rd(m,r[4]+r[0],4);goto L8c6404d4; /* mov.l @(r0,r4),r3 */
L8c6404d4: *stop=0x8c6404d4u;r[3]+=r[5];goto L8c6404d6; /* add r5,r3 */
L8c6404d6: *stop=0x8c6404d6u;r[0]=88u;goto L8c6404d8; /* mov #0x58,r0 */
L8c6404d8: *stop=0x8c6404d8u;if(*pr!=entry_pr)return -3;*stop=0x8c6404dau;if(!valid_ram(r[0]+r[4],4u))return -3;wr(m,r[4]+r[0],r[3],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_position_8c64041c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64041c;
L8c64041c: *stop=0x8c64041cu;*fpul=r[5];goto L8c64041e; /* lds r5,FPUL */
L8c64041e: *stop=0x8c64041eu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c640420; /* tst r4,r4 */
L8c640420: take=!(*sr&1u);*stop=0x8c640422u;status=sa_sh4_vector_numeric(0xf0fdu,f,*fpul);if(status)return status;if(take)goto L8c640428;goto L8c640424; /* bf/s 0x8c640428 */
L8c640424: *stop=0x8c640424u;r[2]=rd(m,0x8c64044cu,4);goto L8c640426; /* mov.l 0x8c64044c,r2 */
L8c640426: *stop=0x8c640426u;if(!valid_ram(r[2],4u))return -3;r[4]=rd(m,r[2],4);goto L8c640428; /* mov.l @r2,r4 */
L8c640428: *stop=0x8c640428u;r[4]+=104u;goto L8c64042a; /* add #0x68,r4 */
L8c64042a: *stop=0x8c64042au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64042c; /* fmov.s @r4+,fr4 */
L8c64042c: *stop=0x8c64042cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64042e; /* fmov.s @r4+,fr2 */
L8c64042e: *stop=0x8c64042eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640430; /* fmov.s @r4+,fr3 */
L8c640430: *stop=0x8c640430u;r[4]+=4294967256u;goto L8c640432; /* add #-0x28,r4 */
L8c640432: *stop=0x8c640432u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[4]);goto L8c640434; /* fmov.s fr4,@r4 */
L8c640434: *stop=0x8c640434u;fcopy(f,4u,3u);goto L8c640436; /* fmov fr3,fr4 */
L8c640436: *stop=0x8c640436u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[1],&f->fpscr,&fr[4]);if(status)return status;}goto L8c640438; /* fmul fr1,fr4 */
L8c640438: *stop=0x8c640438u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c64043a; /* fmac fr0,fr2,fr4 */
L8c64043a: *stop=0x8c64043au;r[4]+=8u;goto L8c64043c; /* add #0x8,r4 */
L8c64043c: *stop=0x8c64043cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[4]);goto L8c64043e; /* fmov.s fr4,@r4 */
L8c64043e: *stop=0x8c64043eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640440; /* fmul fr1,fr2 */
L8c640440: *stop=0x8c640440u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto L8c640442; /* fneg fr0 */
L8c640442: *stop=0x8c640442u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640444; /* fmac fr0,fr3,fr2 */
L8c640444: *stop=0x8c640444u;r[4]+=4294967292u;goto L8c640446; /* add #-0x4,r4 */
L8c640446: *stop=0x8c640446u;if(*pr!=entry_pr)return -3;*stop=0x8c640448u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_position_8c64048c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64048c;
L8c64048c: *stop=0x8c64048cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64048e; /* tst r4,r4 */
L8c64048e: *stop=0x8c64048eu;*fpul=r[5];goto L8c640490; /* lds r5,FPUL */
L8c640490: *stop=0x8c640490u;r[0]=rd(m,0x8c6404b8u,4);goto L8c640492; /* mov.l 0x8c6404b8,r0 */
L8c640492: take=!(*sr&1u);*stop=0x8c640494u;status=sa_sh4_vector_numeric(0xf0fdu,f,*fpul);if(status)return status;if(take)goto L8c64049a;goto L8c640496; /* bf/s 0x8c64049a */
L8c640496: *stop=0x8c640496u;r[2]=rd(m,0x8c6404bcu,4);goto L8c640498; /* mov.l 0x8c6404bc,r2 */
L8c640498: *stop=0x8c640498u;if(!valid_ram(r[2],4u))return -3;r[4]=rd(m,r[2],4);goto L8c64049a; /* mov.l @r2,r4 */
L8c64049a: *stop=0x8c64049au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c64049c; /* fmov.s @(r0,r4),fr2 */
L8c64049c: *stop=0x8c64049cu;r[0]=rd(m,0x8c6404c0u,4);goto L8c64049e; /* mov.l 0x8c6404c0,r0 */
L8c64049e: *stop=0x8c64049eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c6404a0; /* fmov.s @(r0,r4),fr3 */
L8c6404a0: *stop=0x8c6404a0u;fcopy(f,4u,2u);goto L8c6404a2; /* fmov fr2,fr4 */
L8c6404a2: *stop=0x8c6404a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[1],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6404a4; /* fmul fr1,fr4 */
L8c6404a4: *stop=0x8c6404a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6404a6; /* fmac fr0,fr3,fr4 */
L8c6404a6: *stop=0x8c6404a6u;r[0]=rd(m,0x8c6404b8u,4);goto L8c6404a8; /* mov.l 0x8c6404b8,r0 */
L8c6404a8: *stop=0x8c6404a8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto L8c6404aa; /* fmov fr4,@(r0,r4) */
L8c6404aa: *stop=0x8c6404aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[1],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6404ac; /* fmul fr1,fr3 */
L8c6404ac: *stop=0x8c6404acu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto L8c6404ae; /* fneg fr0 */
L8c6404ae: *stop=0x8c6404aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6404b0; /* fmac fr0,fr2,fr3 */
L8c6404b0: *stop=0x8c6404b0u;r[0]=rd(m,0x8c6404c0u,4);goto L8c6404b2; /* mov.l 0x8c6404c0,r0 */
L8c6404b2: *stop=0x8c6404b2u;if(*pr!=entry_pr)return -3;*stop=0x8c6404b4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_position_8c64065c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64065c;
L8c64065c: *stop=0x8c64065cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64065e; /* tst r4,r4 */
L8c64065e: if(!(*sr&1u))goto L8c640664;goto L8c640660; /* bf 0x8c640664 */
L8c640660: *stop=0x8c640660u;r[2]=rd(m,0x8c64067cu,4);goto L8c640662; /* mov.l 0x8c64067c,r2 */
L8c640662: *stop=0x8c640662u;if(!valid_ram(r[2],4u))return -3;r[4]=rd(m,r[2],4);goto L8c640664; /* mov.l @r2,r4 */
L8c640664: *stop=0x8c640664u;r[4]+=92u;goto L8c640666; /* add #0x5c,r4 */
L8c640666: *stop=0x8c640666u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640668; /* fmov.s @r4+,fr0 */
L8c640668: *stop=0x8c640668u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64066a; /* fmov.s @r4+,fr1 */
L8c64066a: *stop=0x8c64066au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[0],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c64066c; /* fadd fr0,fr4 */
L8c64066c: *stop=0x8c64066cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64066e; /* fmov.s @r4+,fr2 */
L8c64066e: *stop=0x8c64066eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[1],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c640670; /* fadd fr1,fr5 */
L8c640670: *stop=0x8c640670u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[2],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c640672; /* fadd fr2,fr6 */
L8c640672: *stop=0x8c640672u;r[4]+=4294967268u;goto L8c640674; /* add #-0x1c,r4 */
L8c640674: *stop=0x8c640674u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[4]);goto L8c640676; /* fmov.s fr6,@-r4 */
L8c640676: *stop=0x8c640676u;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[4]);goto L8c640678; /* fmov.s fr5,@-r4 */
L8c640678: *stop=0x8c640678u;if(*pr!=entry_pr)return -3;*stop=0x8c64067au;if(!valid_ram(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_position_8c60957e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60957e;
L8c60957e: *stop=0x8c60957eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c609580; /* mov.l r14,@-r15 */
L8c609580: *stop=0x8c609580u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c609582; /* mov.l r13,@-r15 */
L8c609582: *stop=0x8c609582u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c609584; /* mov.l r12,@-r15 */
L8c609584: *stop=0x8c609584u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c609586; /* sts.l PR,@-r15 */
L8c609586: *stop=0x8c609586u;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c609588; /* mov.l @r4,r5 */
L8c609588: *stop=0x8c609588u;if(!valid_ram(r[4]+4u,4u))return -3;r[4]=rd(m,r[4]+0x4u,4);goto L8c60958a; /* mov.l @(0x4,r4),r4 */
L8c60958a: *stop=0x8c60958au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c60958c; /* tst r4,r4 */
L8c60958c: if(!(*sr&1u))goto L8c609592;goto L8c60958e; /* bf 0x8c609592 */
L8c60958e: *stop=0x8c609590u;goto L8c609718; /* bra 0x8c609718 */
L8c609592: *stop=0x8c609592u;if(!valid_ram(r[4]+4u,4u))return -3;r[2]=rd(m,r[4]+0x4u,4);goto L8c609594; /* mov.l @(0x4,r4),r2 */
L8c609594: *stop=0x8c609594u;*fpul=r[2];goto L8c609596; /* lds r2,FPUL */
L8c609596: *stop=0x8c609596u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c609598; /* cmp/pz r2 */
L8c609598: take=(*sr&1u);*stop=0x8c60959au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c6095a4;goto L8c60959c; /* bt/s 0x8c6095a4 */
L8c60959c: *stop=0x8c60959cu;r[3]=rd(m,0x8c60968cu,4);goto L8c60959e; /* mov.l 0x8c60968c,r3 */
L8c60959e: *stop=0x8c60959eu;*fpul=r[3];goto L8c6095a0; /* lds r3,FPUL */
L8c6095a0: *stop=0x8c6095a0u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c6095a2; /* fsts FPUL,fr2 */
L8c6095a2: *stop=0x8c6095a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c6095a4; /* fadd fr2,fr3 */
L8c6095a4: *stop=0x8c6095a4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6095a6; /* fcmp/gt fr4,fr3 */
L8c6095a6: if((*sr&1u))goto L8c6095ac;goto L8c6095a8; /* bt 0x8c6095ac */
L8c6095a8: *stop=0x8c6095aau;goto L8c609718; /* bra 0x8c609718 */
L8c6095ac: *stop=0x8c6095acu;if(!valid_ram(r[4]+8u,2u))return -3;r[0]=sx(m,r[4]+0x8u,2);goto L8c6095ae; /* mov.w @(0x8,r4),r0 */
L8c6095ae: *stop=0x8c6095aeu;r[13]=rd(m,0x8c609690u,4);goto L8c6095b0; /* mov.l 0x8c609690,r13 */
L8c6095b0: *stop=0x8c6095b0u;r[3]=rd(m,0x8c609698u,4);goto L8c6095b2; /* mov.l 0x8c609698,r3 */
L8c6095b2: *stop=0x8c6095b2u;if(!valid_ram(r[13],2u))return -3;wr(m,r[13],r[0],2);goto L8c6095b4; /* mov.w r0,@r13 */
L8c6095b4: *stop=0x8c6095b4u;if(!valid_ram(r[4]+10u,2u))return -3;r[0]=sx(m,r[4]+0xau,2);goto L8c6095b6; /* mov.w @(0xa,r4),r0 */
L8c6095b6: *stop=0x8c6095b6u;r[7]=rd(m,0x8c609694u,4);goto L8c6095b8; /* mov.l 0x8c609694,r7 */
L8c6095b8: *stop=0x8c6095b8u;r[0]&=r[3];goto L8c6095ba; /* and r3,r0 */
L8c6095ba: *stop=0x8c6095bau;r[14]=rd(m,0x8c6096a0u,4);goto L8c6095bc; /* mov.l 0x8c6096a0,r14 */
L8c6095bc: *stop=0x8c6095bcu;if(!valid_ram(r[7],2u))return -3;wr(m,r[7],r[0],2);goto L8c6095be; /* mov.w r0,@r7 */
L8c6095be: *stop=0x8c6095beu;if(!valid_ram(r[4]+10u,2u))return -3;r[0]=sx(m,r[4]+0xau,2);goto L8c6095c0; /* mov.w @(0xa,r4),r0 */
L8c6095c0: *stop=0x8c6095c0u;r[6]=rd(m,0x8c60969cu,4);goto L8c6095c2; /* mov.l 0x8c60969c,r6 */
L8c6095c2: *stop=0x8c6095c2u;r[0]=r[0]&65535u;goto L8c6095c4; /* extu.w r0,r0 */
L8c6095c4: *stop=0x8c6095c4u;r[3]=rd(m,0x8c6096a4u,4);goto L8c6095c6; /* mov.l 0x8c6096a4,r3 */
L8c6095c6: *stop=0x8c6095c6u;r[0]&=0xc0u;goto L8c6095c8; /* and #0xc0,r0 */
L8c6095c8: *stop=0x8c6095c8u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c6095ca; /* cmp/pl r0 */
L8c6095ca: *stop=0x8c6095cau;r[0]=*sr&1u;goto L8c6095cc; /* movt r0 */
L8c6095cc: *stop=0x8c6095ccu;if(!valid_ram(r[6],2u))return -3;wr(m,r[6],r[0],2);goto L8c6095ce; /* mov.w r0,@r6 */
L8c6095ce: *stop=0x8c6095ceu;r[0]=12u;goto L8c6095d0; /* mov #0xc,r0 */
L8c6095d0: *stop=0x8c6095d0u;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c6095d2; /* mov.l @r4,r2 */
L8c6095d2: *stop=0x8c6095d2u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[2],4);goto L8c6095d4; /* mov.l r2,@r14 */
L8c6095d4: *stop=0x8c6095d4u;r[2]=0u;goto L8c6095d6; /* mov #0x0,r2 */
L8c6095d6: *stop=0x8c6095d6u;if(!valid_ram(r[4]+4u,4u))return -3;r[1]=rd(m,r[4]+0x4u,4);goto L8c6095d8; /* mov.l @(0x4,r4),r1 */
L8c6095d8: *stop=0x8c6095d8u;if(!valid_ram(r[14]+16u,4u))return -3;wr(m,r[14]+0x10u,r[1],4);goto L8c6095da; /* mov.l r1,@(0x10,r14) */
L8c6095da: *stop=0x8c6095dau;r[1]=r[2];goto L8c6095dc; /* mov r2,r1 */
L8c6095dc: *stop=0x8c6095dcu;r[1]=rd(m,0x8c609684u,4);goto L8c6095de; /* mov.l 0x8c609684,r1 */
L8c6095de: *stop=0x8c6095deu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[14]);goto L8c6095e0; /* fmov fr4,@(r0,r14) */
L8c6095e0: *stop=0x8c6095e0u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[2],4);goto L8c6095e2; /* mov.l r2,@(0x14,r14) */
L8c6095e2: *stop=0x8c6095e2u;if(!valid_ram(r[14]+24u,4u))return -3;wr(m,r[14]+0x18u,r[2],4);goto L8c6095e4; /* mov.l r2,@(0x18,r14) */
L8c6095e4: *stop=0x8c6095e4u;r[2]=r[5];goto L8c6095e6; /* mov r5,r2 */
L8c6095e6: target=r[3];*pr=0x8c6095eau;*stop=0x8c6095e8u;r[0]=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6095ea; /* jsr @r3 */
L8c6095ea: *stop=0x8c6095eau;if(!valid_ram(r[6],2u))return -3;r[2]=sx(m,r[6],2);goto L8c6095ec; /* mov.w @r6,r2 */
L8c6095ec: *stop=0x8c6095ecu;r[0]=rd(m,0x8c6096a8u,4);goto L8c6095ee; /* mov.l 0x8c6096a8,r0 */
L8c6095ee: *stop=0x8c6095eeu;r[2]=r[2]&65535u;goto L8c6095f0; /* extu.w r2,r2 */
L8c6095f0: *stop=0x8c6095f0u;r[2]<<=2;goto L8c6095f2; /* shll2 r2 */
L8c6095f2: *stop=0x8c6095f2u;if(!valid_ram(r[0]+r[2],4u))return -3;r[3]=rd(m,r[2]+r[0],4);goto L8c6095f4; /* mov.l @(r0,r2),r3 */
L8c6095f4: *stop=0x8c6095f4u;r[2]=rd(m,0x8c6096acu,4);goto L8c6095f6; /* mov.l 0x8c6096ac,r2 */
L8c6095f6: *stop=0x8c6095f6u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c6095f8; /* mov.l r3,@r2 */
L8c6095f8: *stop=0x8c6095f8u;if(!valid_ram(r[7],2u))return -3;r[0]=sx(m,r[7],2);goto L8c6095fa; /* mov.w @r7,r0 */
L8c6095fa: *stop=0x8c6095fau;r[0]=r[0]&65535u;goto L8c6095fc; /* extu.w r0,r0 */
L8c6095fc: *stop=0x8c6095fcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6095fe; /* cmp/eq #0x1,r0 */
L8c6095fe: take=(*sr&1u);*stop=0x8c609600u;if(!valid_ram(r[14],4u))return -3;r[12]=rd(m,r[14],4);if(take)goto L8c609616;goto L8c609602; /* bt/s 0x8c609616 */
L8c609602: *stop=0x8c609602u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c609604; /* cmp/eq #0x2,r0 */
L8c609604: if((*sr&1u))goto L8c609620;goto L8c609606; /* bt 0x8c609620 */
L8c609606: *stop=0x8c609606u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c609608; /* cmp/eq #0x3,r0 */
L8c609608: if((*sr&1u))goto L8c60962a;goto L8c60960a; /* bt 0x8c60962a */
L8c60960a: *stop=0x8c60960au;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c60960c; /* cmp/eq #0x4,r0 */
L8c60960c: if((*sr&1u))goto L8c609634;goto L8c60960e; /* bt 0x8c609634 */
L8c60960e: *stop=0x8c60960eu;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c609610; /* cmp/eq #0x5,r0 */
L8c609610: if((*sr&1u))goto L8c60963e;goto L8c609612; /* bt 0x8c60963e */
L8c609612: *stop=0x8c609614u;goto L8c609718; /* bra 0x8c609718 */
L8c609616: *stop=0x8c609616u;r[2]=r[12];goto L8c609618; /* mov r12,r2 */
L8c609618: *stop=0x8c609618u;r[2]+=4u;goto L8c60961a; /* add #0x4,r2 */
L8c60961a: *stop=0x8c60961au;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[12],4);goto L8c60961c; /* mov.l r12,@(0x4,r14) */
L8c60961c: *stop=0x8c60961eu;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[2],4);goto L8c609648; /* bra 0x8c609648 */
L8c609620: *stop=0x8c609620u;r[1]=r[12];goto L8c609622; /* mov r12,r1 */
L8c609622: *stop=0x8c609622u;r[3]=r[1];goto L8c609624; /* mov r1,r3 */
L8c609624: *stop=0x8c609624u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[12],4);goto L8c609626; /* mov.l r12,@(0x4,r14) */
L8c609626: *stop=0x8c609628u;r[3]+=8u;goto L8c609646; /* bra 0x8c609646 */
L8c60962a: *stop=0x8c60962au;r[1]=r[12];goto L8c60962c; /* mov r12,r1 */
L8c60962c: *stop=0x8c60962cu;r[3]=r[1];goto L8c60962e; /* mov r1,r3 */
L8c60962e: *stop=0x8c60962eu;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[12],4);goto L8c609630; /* mov.l r12,@(0x4,r14) */
L8c609630: *stop=0x8c609632u;r[3]+=12u;goto L8c609646; /* bra 0x8c609646 */
L8c609634: *stop=0x8c609634u;r[1]=r[12];goto L8c609636; /* mov r12,r1 */
L8c609636: *stop=0x8c609636u;r[3]=r[1];goto L8c609638; /* mov r1,r3 */
L8c609638: *stop=0x8c609638u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[12],4);goto L8c60963a; /* mov.l r12,@(0x4,r14) */
L8c60963a: *stop=0x8c60963cu;r[3]+=16u;goto L8c609646; /* bra 0x8c609646 */
L8c60963e: *stop=0x8c60963eu;r[1]=r[12];goto L8c609640; /* mov r12,r1 */
L8c609640: *stop=0x8c609640u;r[3]=r[1];goto L8c609642; /* mov r1,r3 */
L8c609642: *stop=0x8c609642u;r[3]+=20u;goto L8c609644; /* add #0x14,r3 */
L8c609644: *stop=0x8c609644u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[12],4);goto L8c609646; /* mov.l r12,@(0x4,r14) */
L8c609646: *stop=0x8c609646u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[3],4);goto L8c609648; /* mov.l r3,@(0x8,r14) */
L8c609648: *stop=0x8c609648u;if(!valid_ram(r[13],2u))return -3;r[0]=sx(m,r[13],2);goto L8c60964a; /* mov.w @r13,r0 */
L8c60964a: *stop=0x8c60964au;r[0]=r[0]&65535u;goto L8c60964c; /* extu.w r0,r0 */
L8c60964c: *stop=0x8c60964cu;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c60964e; /* tst #0x1,r0 */
L8c60964e: if((*sr&1u))goto L8c609654;goto L8c609650; /* bt 0x8c609654 */
L8c609650: target=0x8c609264u;*pr=0x8c609654u;*stop=0x8c609652u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c609654; /* bsr 0x8c609264 */
L8c609654: *stop=0x8c609654u;if(!valid_ram(r[13],2u))return -3;r[0]=sx(m,r[13],2);goto L8c609656; /* mov.w @r13,r0 */
L8c609656: *stop=0x8c609656u;r[0]=r[0]&65535u;goto L8c609658; /* extu.w r0,r0 */
L8c609658: *stop=0x8c609658u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c60965a; /* tst #0x2,r0 */
L8c60965a: if((*sr&1u))goto L8c609662;goto L8c60965c; /* bt 0x8c609662 */
L8c60965c: *stop=0x8c60965cu;if(!valid_ram(r[14]+20u,4u))return -3;r[2]=rd(m,r[14]+0x14u,4);goto L8c60965e; /* mov.l @(0x14,r14),r2 */
L8c60965e: *stop=0x8c60965eu;r[2]+=1u;goto L8c609660; /* add #0x1,r2 */
L8c609660: *stop=0x8c609660u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[2],4);goto L8c609662; /* mov.l r2,@(0x14,r14) */
L8c609662: *stop=0x8c609662u;if(!valid_ram(r[13],2u))return -3;r[0]=sx(m,r[13],2);goto L8c609664; /* mov.w @r13,r0 */
L8c609664: *stop=0x8c609664u;r[0]=r[0]&65535u;goto L8c609666; /* extu.w r0,r0 */
L8c609666: *stop=0x8c609666u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c609668; /* tst #0x4,r0 */
L8c609668: if((*sr&1u))goto L8c609670;goto L8c60966a; /* bt 0x8c609670 */
L8c60966a: *stop=0x8c60966au;if(!valid_ram(r[14]+20u,4u))return -3;r[2]=rd(m,r[14]+0x14u,4);goto L8c60966c; /* mov.l @(0x14,r14),r2 */
L8c60966c: *stop=0x8c60966cu;r[2]+=1u;goto L8c60966e; /* add #0x1,r2 */
L8c60966e: *stop=0x8c60966eu;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[2],4);goto L8c609670; /* mov.l r2,@(0x14,r14) */
L8c609670: *stop=0x8c609670u;if(!valid_ram(r[13],2u))return -3;r[4]=sx(m,r[13],2);goto L8c609672; /* mov.w @r13,r4 */
L8c609672: *stop=0x8c609672u;r[3]=64u;goto L8c609674; /* mov #0x40,r3 */
L8c609674: *stop=0x8c609674u;r[4]=r[4]&65535u;goto L8c609676; /* extu.w r4,r4 */
L8c609676: *stop=0x8c609676u;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c609678; /* tst r4,r3 */
L8c609678: if((*sr&1u))goto L8c6096b0;goto L8c60967a; /* bt 0x8c6096b0 */
L8c60967a: target=0x8c6093d8u;*pr=0x8c60967eu;*stop=0x8c60967cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60967e; /* bsr 0x8c6093d8 */
L8c60967e: *stop=0x8c609680u;goto L8c6096ba; /* bra 0x8c6096ba */
L8c6096b0: *stop=0x8c6096b0u;r[1]=8u;goto L8c6096b2; /* mov #0x8,r1 */
L8c6096b2: *stop=0x8c6096b2u;*sr=(*sr&~1u)|!(r[1]&r[4]);goto L8c6096b4; /* tst r1,r4 */
L8c6096b4: if((*sr&1u))goto L8c6096ba;goto L8c6096b6; /* bt 0x8c6096ba */
L8c6096b6: target=0x8c609326u;*pr=0x8c6096bau;*stop=0x8c6096b8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6096ba; /* bsr 0x8c609326 */
L8c6096ba: *stop=0x8c6096bau;if(!valid_ram(r[13],2u))return -3;r[0]=sx(m,r[13],2);goto L8c6096bc; /* mov.w @r13,r0 */
L8c6096bc: *stop=0x8c6096bcu;r[0]=r[0]&65535u;goto L8c6096be; /* extu.w r0,r0 */
L8c6096be: *stop=0x8c6096beu;*sr=(*sr&~1u)|!(r[0]&0x80u);goto L8c6096c0; /* tst #0x80,r0 */
L8c6096c0: if((*sr&1u))goto L8c6096c6;goto L8c6096c2; /* bt 0x8c6096c6 */
L8c6096c2: target=0x8c60945eu;*pr=0x8c6096c6u;*stop=0x8c6096c4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6096c6; /* bsr 0x8c60945e */
L8c6096c6: *stop=0x8c6096c6u;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c6096c8; /* mov.w @r13,r3 */
L8c6096c8: *stop=0x8c6096c8u;r[2]=sx(m,0x8c609722u,2);goto L8c6096ca; /* mov.w 0x8c609722,r2 */
L8c6096ca: *stop=0x8c6096cau;r[3]=r[3]&65535u;goto L8c6096cc; /* extu.w r3,r3 */
L8c6096cc: *stop=0x8c6096ccu;*sr=(*sr&~1u)|!(r[2]&r[3]);goto L8c6096ce; /* tst r2,r3 */
L8c6096ce: if((*sr&1u))goto L8c6096d4;goto L8c6096d0; /* bt 0x8c6096d4 */
L8c6096d0: target=0x8c6094e0u;*pr=0x8c6096d4u;*stop=0x8c6096d2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6096d4; /* bsr 0x8c6094e0 */
L8c6096d4: *stop=0x8c6096d4u;if(!valid_ram(r[13],2u))return -3;r[1]=sx(m,r[13],2);goto L8c6096d6; /* mov.w @r13,r1 */
L8c6096d6: *stop=0x8c6096d6u;r[3]=sx(m,0x8c609724u,2);goto L8c6096d8; /* mov.w 0x8c609724,r3 */
L8c6096d8: *stop=0x8c6096d8u;r[1]=r[1]&65535u;goto L8c6096da; /* extu.w r1,r1 */
L8c6096da: *stop=0x8c6096dau;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c6096dc; /* tst r3,r1 */
L8c6096dc: if((*sr&1u))goto L8c6096e4;goto L8c6096de; /* bt 0x8c6096e4 */
L8c6096de: *stop=0x8c6096deu;if(!valid_ram(r[14]+20u,4u))return -3;r[0]=rd(m,r[14]+0x14u,4);goto L8c6096e0; /* mov.l @(0x14,r14),r0 */
L8c6096e0: *stop=0x8c6096e0u;r[0]+=1u;goto L8c6096e2; /* add #0x1,r0 */
L8c6096e2: *stop=0x8c6096e2u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[0],4);goto L8c6096e4; /* mov.l r0,@(0x14,r14) */
L8c6096e4: *stop=0x8c6096e4u;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c6096e6; /* mov.w @r13,r3 */
L8c6096e6: *stop=0x8c6096e6u;r[2]=sx(m,0x8c609726u,2);goto L8c6096e8; /* mov.w 0x8c609726,r2 */
L8c6096e8: *stop=0x8c6096e8u;r[3]=r[3]&65535u;goto L8c6096ea; /* extu.w r3,r3 */
L8c6096ea: *stop=0x8c6096eau;*sr=(*sr&~1u)|!(r[2]&r[3]);goto L8c6096ec; /* tst r2,r3 */
L8c6096ec: if((*sr&1u))goto L8c6096f4;goto L8c6096ee; /* bt 0x8c6096f4 */
L8c6096ee: *stop=0x8c6096eeu;if(!valid_ram(r[14]+20u,4u))return -3;r[0]=rd(m,r[14]+0x14u,4);goto L8c6096f0; /* mov.l @(0x14,r14),r0 */
L8c6096f0: *stop=0x8c6096f0u;r[0]+=1u;goto L8c6096f2; /* add #0x1,r0 */
L8c6096f2: *stop=0x8c6096f2u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[0],4);goto L8c6096f4; /* mov.l r0,@(0x14,r14) */
L8c6096f4: *stop=0x8c6096f4u;if(!valid_ram(r[13],2u))return -3;r[4]=sx(m,r[13],2);goto L8c6096f6; /* mov.w @r13,r4 */
L8c6096f6: *stop=0x8c6096f6u;r[3]=sx(m,0x8c609728u,2);goto L8c6096f8; /* mov.w 0x8c609728,r3 */
L8c6096f8: *stop=0x8c6096f8u;r[4]=r[4]&65535u;goto L8c6096fa; /* extu.w r4,r4 */
L8c6096fa: *stop=0x8c6096fau;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6096fc; /* tst r4,r3 */
L8c6096fc: if(!(*sr&1u))goto L8c609704;goto L8c6096fe; /* bf 0x8c609704 */
L8c6096fe: *stop=0x8c6096feu;r[1]=sx(m,0x8c60972au,2);goto L8c609700; /* mov.w 0x8c60972a,r1 */
L8c609700: *stop=0x8c609700u;*sr=(*sr&~1u)|!(r[1]&r[4]);goto L8c609702; /* tst r1,r4 */
L8c609702: if((*sr&1u))goto L8c60970a;goto L8c609704; /* bt 0x8c60970a */
L8c609704: *stop=0x8c609704u;if(!valid_ram(r[14]+20u,4u))return -3;r[2]=rd(m,r[14]+0x14u,4);goto L8c609706; /* mov.l @(0x14,r14),r2 */
L8c609706: *stop=0x8c609706u;r[2]+=1u;goto L8c609708; /* add #0x1,r2 */
L8c609708: *stop=0x8c609708u;if(!valid_ram(r[14]+20u,4u))return -3;wr(m,r[14]+0x14u,r[2],4);goto L8c60970a; /* mov.l r2,@(0x14,r14) */
L8c60970a: *stop=0x8c60970au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60970c; /* lds.l @r15+,PR */
L8c60970c: *stop=0x8c60970cu;r[3]=rd(m,0x8c609730u,4);goto L8c60970e; /* mov.l 0x8c609730,r3 */
L8c60970e: *stop=0x8c60970eu;r[4]=rd(m,0x8c60972cu,4);goto L8c609710; /* mov.l 0x8c60972c,r4 */
L8c609710: *stop=0x8c609710u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c609712; /* mov.l @r15+,r12 */
L8c609712: *stop=0x8c609712u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c609714; /* mov.l @r15+,r13 */
L8c609714: target=r[3];*stop=0x8c609716u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c609718: *stop=0x8c609718u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60971a; /* lds.l @r15+,PR */
L8c60971a: *stop=0x8c60971au;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60971c; /* mov.l @r15+,r12 */
L8c60971c: *stop=0x8c60971cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60971e; /* mov.l @r15+,r13 */
L8c60971e: *stop=0x8c60971eu;if(*pr!=entry_pr)return -3;*stop=0x8c609720u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
