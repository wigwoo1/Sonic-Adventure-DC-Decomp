/* Original camera allocation, descriptor and stored lifecycle callbacks; names inferred.
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
SA_STAGE_EXPORT int sa_model_camera_8c01951e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01951e;
L8c01951e: *stop=0x8c01951eu;r[4]=rd(m,0x8c019664u,4);goto L8c019520; /* mov.l 0x8c019664,r4 */
L8c019520: *stop=0x8c019520u;r[5]=0u;goto L8c019522; /* mov #0x0,r5 */
L8c019522: *stop=0x8c019522u;r[0]=r[5];goto L8c019524; /* mov r5,r0 */
L8c019524: *stop=0x8c019524u;r[2]=rd(m,0x8c019668u,4);goto L8c019526; /* mov.l 0x8c019668,r2 */
L8c019526: *stop=0x8c019526u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019528; /* mov.l @r4,r3 */
L8c019528: *stop=0x8c019528u;r[6]=rd(m,0x8c01966cu,4);goto L8c01952a; /* mov.l 0x8c01966c,r6 */
L8c01952a: *stop=0x8c01952au;if(!valid_ram(r[3]+8u,1u))return -3;wr(m,r[3]+0x8u,r[0],1);goto L8c01952c; /* mov.b r0,@(0x8,r3) */
L8c01952c: *stop=0x8c01952cu;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c01952e; /* mov.l @r4,r3 */
L8c01952e: *stop=0x8c01952eu;if(!valid_ram(r[3]+4u,2u))return -3;wr(m,r[3]+0x4u,r[0],2);goto L8c019530; /* mov.w r0,@(0x4,r3) */
L8c019530: *stop=0x8c019530u;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto L8c019532; /* mov.l @r2,r0 */
L8c019532: *stop=0x8c019532u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019534; /* mov.l @r4,r3 */
L8c019534: *stop=0x8c019534u;if(!valid_ram(r[0],1u))return -3;r[0]=sx(m,r[0],1);goto L8c019536; /* mov.b @r0,r0 */
L8c019536: *stop=0x8c019536u;if(!valid_ram(r[3]+6u,1u))return -3;wr(m,r[3]+0x6u,r[0],1);goto L8c019538; /* mov.b r0,@(0x6,r3) */
L8c019538: *stop=0x8c019538u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c01953a; /* mov.l @r4,r3 */
L8c01953a: *stop=0x8c01953au;r[1]=r[3];goto L8c01953c; /* mov r3,r1 */
L8c01953c: *stop=0x8c01953cu;if(!valid_ram(r[1]+6u,1u))return -3;r[0]=sx(m,r[1]+0x6u,1);goto L8c01953e; /* mov.b @(0x6,r1),r0 */
L8c01953e: *stop=0x8c01953eu;if(!valid_ram(r[6],4u))return -3;r[1]=rd(m,r[6],4);goto L8c019540; /* mov.l @r6,r1 */
L8c019540: *stop=0x8c019540u;r[0]<<=2;goto L8c019542; /* shll2 r0 */
L8c019542: *stop=0x8c019542u;r[0]<<=2;goto L8c019544; /* shll2 r0 */
L8c019544: *stop=0x8c019544u;if(!valid_ram(r[0]+r[1],4u))return -3;r[1]=rd(m,r[1]+r[0],4);goto L8c019546; /* mov.l @(r0,r1),r1 */
L8c019546: *stop=0x8c019546u;r[0]=r[5];goto L8c019548; /* mov r5,r0 */
L8c019548: *stop=0x8c019548u;if(!valid_ram(r[3]+12u,4u))return -3;wr(m,r[3]+0xcu,r[1],4);goto L8c01954a; /* mov.l r1,@(0xc,r3) */
L8c01954a: *stop=0x8c01954au;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c01954c; /* mov.l @r4,r3 */
L8c01954c: *stop=0x8c01954cu;if(!valid_ram(r[3]+9u,1u))return -3;wr(m,r[3]+0x9u,r[0],1);goto L8c01954e; /* mov.b r0,@(0x9,r3) */
L8c01954e: *stop=0x8c01954eu;r[0]=rd(m,0x8c019670u,4);goto L8c019550; /* mov.l 0x8c019670,r0 */
L8c019550: *stop=0x8c019550u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019552; /* mov.l @r4,r3 */
L8c019552: *stop=0x8c019552u;if(!valid_ram(r[0],4u))return -3;r[1]=rd(m,r[0],4);goto L8c019554; /* mov.l @r0,r1 */
L8c019554: *stop=0x8c019554u;if(!valid_ram(r[1]+4u,4u))return -3;r[1]=rd(m,r[1]+0x4u,4);goto L8c019556; /* mov.l @(0x4,r1),r1 */
L8c019556: *stop=0x8c019556u;if(!valid_ram(r[3]+16u,4u))return -3;wr(m,r[3]+0x10u,r[1],4);goto L8c019558; /* mov.l r1,@(0x10,r3) */
L8c019558: *stop=0x8c019558u;r[1]=rd(m,0x8c019674u,4);goto L8c01955a; /* mov.l 0x8c019674,r1 */
L8c01955a: *stop=0x8c01955au;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c01955c; /* mov.l @r1,r3 */
L8c01955c: *stop=0x8c01955cu;if(!valid_ram(r[3]+20u,4u))return -3;wr(m,r[3]+0x14u,r[5],4);goto L8c01955e; /* mov.l r5,@(0x14,r3) */
L8c01955e: *stop=0x8c01955eu;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019560; /* mov.l @r4,r3 */
L8c019560: *stop=0x8c019560u;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c019562; /* mov.l @r6,r2 */
L8c019562: *stop=0x8c019562u;r[0]=r[3];goto L8c019564; /* mov r3,r0 */
L8c019564: *stop=0x8c019564u;if(!valid_ram(r[0]+6u,1u))return -3;r[0]=sx(m,r[0]+0x6u,1);goto L8c019566; /* mov.b @(0x6,r0),r0 */
L8c019566: *stop=0x8c019566u;r[0]<<=2;goto L8c019568; /* shll2 r0 */
L8c019568: *stop=0x8c019568u;r[0]<<=2;goto L8c01956a; /* shll2 r0 */
L8c01956a: *stop=0x8c01956au;r[0]+=r[2];goto L8c01956c; /* add r2,r0 */
L8c01956c: *stop=0x8c01956cu;if(!valid_ram(r[0]+5u,1u))return -3;r[0]=sx(m,r[0]+0x5u,1);goto L8c01956e; /* mov.b @(0x5,r0),r0 */
L8c01956e: *stop=0x8c01956eu;r[2]=1u;goto L8c019570; /* mov #0x1,r2 */
L8c019570: *stop=0x8c019570u;if(!valid_ram(r[3]+7u,1u))return -3;wr(m,r[3]+0x7u,r[0],1);goto L8c019572; /* mov.b r0,@(0x7,r3) */
L8c019572: *stop=0x8c019572u;r[0]=2u;goto L8c019574; /* mov #0x2,r0 */
L8c019574: *stop=0x8c019574u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019576; /* mov.l @r4,r3 */
L8c019576: *stop=0x8c019576u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto L8c019578; /* mov.l r2,@r3 */
L8c019578: *stop=0x8c019578u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c01957a; /* mov.l @r4,r3 */
L8c01957a: *stop=0x8c01957au;if(*pr!=entry_pr)return -3;*stop=0x8c01957cu;if(!valid_ram(r[3]+11u,1u))return -3;wr(m,r[3]+0xbu,r[0],1);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_model_camera_8c01957e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01957e;
L8c01957e: *stop=0x8c01957eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c019580; /* mov.l r14,@-r15 */
L8c019580: *stop=0x8c019580u;r[5]=0u;goto L8c019582; /* mov #0x0,r5 */
L8c019582: *stop=0x8c019582u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c019584; /* mov.l r13,@-r15 */
L8c019584: *stop=0x8c019584u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c019586; /* sts.l PR,@-r15 */
L8c019586: *stop=0x8c019586u;r[3]=rd(m,0x8c01967cu,4);goto L8c019588; /* mov.l 0x8c01967c,r3 */
L8c019588: *stop=0x8c019588u;r[6]=rd(m,0x8c019678u,4);goto L8c01958a; /* mov.l 0x8c019678,r6 */
L8c01958a: target=r[3];*pr=0x8c01958eu;*stop=0x8c01958cu;r[4]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01958e; /* jsr @r3 */
L8c01958e: *stop=0x8c01958eu;r[2]=rd(m,0x8c019684u,4);goto L8c019590; /* mov.l 0x8c019684,r2 */
L8c019590: *stop=0x8c019590u;r[14]=rd(m,0x8c019680u,4);goto L8c019592; /* mov.l 0x8c019680,r14 */
L8c019592: target=r[2];*pr=0x8c019596u;*stop=0x8c019594u;r[13]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019596; /* jsr @r2 */
L8c019596: *stop=0x8c019596u;r[5]=rd(m,0x8c019688u,4);goto L8c019598; /* mov.l 0x8c019688,r5 */
L8c019598: *stop=0x8c019598u;r[4]=rd(m,0x8c01968cu,4);goto L8c01959a; /* mov.l 0x8c01968c,r4 */
L8c01959a: *stop=0x8c01959au;if(!valid_ram(r[14],2u))return -3;wr(m,r[14],r[0],2);goto L8c01959c; /* mov.w r0,@r14 */
L8c01959c: *stop=0x8c01959cu;if(!valid_ram(r[14],1u))return -3;r[3]=sx(m,r[14],1);goto L8c01959e; /* mov.b @r14,r3 */
L8c01959e: *stop=0x8c01959eu;r[0]=rd(m,0x8c01966cu,4);goto L8c0195a0; /* mov.l 0x8c01966c,r0 */
L8c0195a0: *stop=0x8c0195a0u;r[3]=r[3]&255u;goto L8c0195a2; /* extu.b r3,r3 */
L8c0195a2: *stop=0x8c0195a2u;if(!valid_ram(r[5],2u))return -3;wr(m,r[5],r[3],2);goto L8c0195a4; /* mov.w r3,@r5 */
L8c0195a4: *stop=0x8c0195a4u;r[3]=4294967288u;goto L8c0195a6; /* mov #-0x8,r3 */
L8c0195a6: *stop=0x8c0195a6u;if(!valid_ram(r[14],2u))return -3;r[2]=sx(m,r[14],2);goto L8c0195a8; /* mov.w @r14,r2 */
L8c0195a8: *stop=0x8c0195a8u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c0195aa; /* shad r3,r2 */
L8c0195aa: *stop=0x8c0195aau;r[3]=rd(m,0x8c019690u,4);goto L8c0195ac; /* mov.l 0x8c019690,r3 */
L8c0195ac: *stop=0x8c0195acu;if(!valid_ram(r[14],2u))return -3;wr(m,r[14],r[2],2);goto L8c0195ae; /* mov.w r2,@r14 */
L8c0195ae: *stop=0x8c0195aeu;if(!valid_ram(r[4],2u))return -3;r[2]=sx(m,r[4],2);goto L8c0195b0; /* mov.w @r4,r2 */
L8c0195b0: *stop=0x8c0195b0u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[2],2);goto L8c0195b2; /* mov.w r2,@r3 */
L8c0195b2: *stop=0x8c0195b2u;if(!valid_ram(r[4]+4u,4u))return -3;r[1]=rd(m,r[4]+0x4u,4);goto L8c0195b4; /* mov.l @(0x4,r4),r1 */
L8c0195b4: *stop=0x8c0195b4u;r[2]=rd(m,0x8c019694u,4);goto L8c0195b6; /* mov.l 0x8c019694,r2 */
L8c0195b6: *stop=0x8c0195b6u;if(!valid_ram(r[0],4u))return -3;wr(m,r[0],r[1],4);goto L8c0195b8; /* mov.l r1,@r0 */
L8c0195b8: *stop=0x8c0195b8u;if(!valid_ram(r[4]+2u,2u))return -3;r[0]=sx(m,r[4]+0x2u,2);goto L8c0195ba; /* mov.w @(0x2,r4),r0 */
L8c0195ba: *stop=0x8c0195bau;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[0],2);goto L8c0195bc; /* mov.w r0,@r2 */
L8c0195bc: *stop=0x8c0195bcu;if(!valid_ram(r[4]+8u,4u))return -3;r[1]=rd(m,r[4]+0x8u,4);goto L8c0195be; /* mov.l @(0x8,r4),r1 */
L8c0195be: *stop=0x8c0195beu;r[0]=rd(m,0x8c019670u,4);goto L8c0195c0; /* mov.l 0x8c019670,r0 */
L8c0195c0: *stop=0x8c0195c0u;if(!valid_ram(r[0],4u))return -3;wr(m,r[0],r[1],4);goto L8c0195c2; /* mov.l r1,@r0 */
L8c0195c2: *stop=0x8c0195c2u;if(!valid_ram(r[14],2u))return -3;r[4]=sx(m,r[14],2);goto L8c0195c4; /* mov.w @r14,r4 */
L8c0195c4: *stop=0x8c0195c4u;if(!valid_ram(r[5],2u))return -3;r[3]=sx(m,r[5],2);goto L8c0195c6; /* mov.w @r5,r3 */
L8c0195c6: *stop=0x8c0195c6u;r[4]<<=2;goto L8c0195c8; /* shll2 r4 */
L8c0195c8: *stop=0x8c0195c8u;r[1]=rd(m,0x8c019698u,4);goto L8c0195ca; /* mov.l 0x8c019698,r1 */
L8c0195ca: *stop=0x8c0195cau;r[4]<<=2;goto L8c0195cc; /* shll2 r4 */
L8c0195cc: *stop=0x8c0195ccu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c0195ce; /* shll r4 */
L8c0195ce: *stop=0x8c0195ceu;r[4]+=r[1];goto L8c0195d0; /* add r1,r4 */
L8c0195d0: *stop=0x8c0195d0u;r[3]<<=2;goto L8c0195d2; /* shll2 r3 */
L8c0195d2: *stop=0x8c0195d2u;r[4]+=r[3];goto L8c0195d4; /* add r3,r4 */
L8c0195d4: *stop=0x8c0195d4u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c0195d6; /* mov.l @r4,r3 */
L8c0195d6: *stop=0x8c0195d6u;r[2]=rd(m,0x8c01969cu,4);goto L8c0195d8; /* mov.l 0x8c01969c,r2 */
L8c0195d8: *stop=0x8c0195d8u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c0195da; /* mov.l r3,@r2 */
L8c0195da: *stop=0x8c0195dau;if(!valid_ram(r[4],4u))return -3;r[0]=rd(m,r[4],4);goto L8c0195dc; /* mov.l @r4,r0 */
L8c0195dc: *stop=0x8c0195dcu;r[3]=rd(m,0x8c019668u,4);goto L8c0195de; /* mov.l 0x8c019668,r3 */
L8c0195de: *stop=0x8c0195deu;r[0]+=64u;goto L8c0195e0; /* add #0x40,r0 */
L8c0195e0: target=0x8c01951eu;*pr=0x8c0195e4u;*stop=0x8c0195e2u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[0],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0195e4; /* bsr 0x8c01951e */
L8c0195e4: *stop=0x8c0195e4u;r[1]=rd(m,0x8c0196a0u,4);goto L8c0195e6; /* mov.l 0x8c0196a0,r1 */
L8c0195e6: *stop=0x8c0195e6u;r[2]=0u;goto L8c0195e8; /* mov #0x0,r2 */
L8c0195e8: *stop=0x8c0195e8u;r[3]=rd(m,0x8c0196a4u,4);goto L8c0195ea; /* mov.l 0x8c0196a4,r3 */
L8c0195ea: *stop=0x8c0195eau;r[4]=4294967295u;goto L8c0195ec; /* mov #-0x1,r4 */
L8c0195ec: *stop=0x8c0195ecu;if(!valid_ram(r[13]+24u,4u))return -3;wr(m,r[13]+0x18u,r[1],4);goto L8c0195ee; /* mov.l r1,@(0x18,r13) */
L8c0195ee: *stop=0x8c0195eeu;r[1]=rd(m,0x8c0196a8u,4);goto L8c0195f0; /* mov.l 0x8c0196a8,r1 */
L8c0195f0: *stop=0x8c0195f0u;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[3],4);goto L8c0195f2; /* mov.l r3,@(0x14,r13) */
L8c0195f2: *stop=0x8c0195f2u;r[3]=rd(m,0x8c0196acu,4);goto L8c0195f4; /* mov.l 0x8c0196ac,r3 */
L8c0195f4: *stop=0x8c0195f4u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[2],4);goto L8c0195f6; /* mov.l r2,@r1 */
L8c0195f6: *stop=0x8c0195f6u;r[1]=50u;goto L8c0195f8; /* mov #0x32,r1 */
L8c0195f8: *stop=0x8c0195f8u;r[1]<<=8;goto L8c0195fa; /* shll8 r1 */
L8c0195fa: *stop=0x8c0195fau;r[0]=rd(m,0x8c0196b4u,4);goto L8c0195fc; /* mov.l 0x8c0196b4,r0 */
L8c0195fc: *stop=0x8c0195fcu;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[4],4);goto L8c0195fe; /* mov.l r4,@r3 */
L8c0195fe: *stop=0x8c0195feu;r[1]+=4294967239u;goto L8c019600; /* add #-0x39,r1 */
L8c019600: *stop=0x8c019600u;r[2]=rd(m,0x8c0196b0u,4);goto L8c019602; /* mov.l 0x8c0196b0,r2 */
L8c019602: *stop=0x8c019602u;r[3]=rd(m,0x8c0196b8u,4);goto L8c019604; /* mov.l 0x8c0196b8,r3 */
L8c019604: *stop=0x8c019604u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[4],4);goto L8c019606; /* mov.l r4,@r2 */
L8c019606: *stop=0x8c019606u;if(!valid_ram(r[0],4u))return -3;wr(m,r[0],r[1],4);goto L8c019608; /* mov.l r1,@r0 */
L8c019608: target=r[3];*pr=0x8c01960cu;*stop=0x8c01960au;r[4]=r[1];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01960c; /* jsr @r3 */
L8c01960c: *stop=0x8c01960cu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01960e; /* lds.l @r15+,PR */
L8c01960e: *stop=0x8c01960eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c019610; /* mov.l @r15+,r13 */
L8c019610: *stop=0x8c019610u;if(*pr!=entry_pr)return -3;*stop=0x8c019612u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_model_camera_8c0197ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0197ba;
L8c0197ba: *stop=0x8c0197bau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0197bc; /* mov.l r14,@-r15 */
L8c0197bc: *stop=0x8c0197bcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0197be; /* mov.l r13,@-r15 */
L8c0197be: *stop=0x8c0197beu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0197c0; /* mov.l r12,@-r15 */
L8c0197c0: *stop=0x8c0197c0u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c0197c2; /* fmov.s fr15,@-r15 */
L8c0197c2: *stop=0x8c0197c2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c0197c4; /* sts.l PR,@-r15 */
L8c0197c4: *stop=0x8c0197c4u;r[15]+=4294967292u;goto L8c0197c6; /* add #-0x4,r15 */
L8c0197c6: *stop=0x8c0197c6u;r[14]=r[4];goto L8c0197c8; /* mov r4,r14 */
L8c0197c8: *stop=0x8c0197c8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c0197ca; /* fldi0 fr15 */
L8c0197ca: *stop=0x8c0197cau;r[3]=rd(m,0x8c01985cu,4);goto L8c0197cc; /* mov.l 0x8c01985c,r3 */
L8c0197cc: *stop=0x8c0197ccu;fcopy(f,5u,15u);goto L8c0197ce; /* fmov fr15,fr5 */
L8c0197ce: *stop=0x8c0197ceu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[4],4);goto L8c0197d0; /* mov.l r4,@r15 */
L8c0197d0: *stop=0x8c0197d0u;if(!valid_ram(r[14]+32u,4u))return -3;r[14]=rd(m,r[14]+0x20u,4);goto L8c0197d2; /* mov.l @(0x20,r14),r14 */
L8c0197d2: *stop=0x8c0197d2u;fcopy(f,7u,15u);goto L8c0197d4; /* fmov fr15,fr7 */
L8c0197d4: *stop=0x8c0197d4u;fcopy(f,6u,15u);goto L8c0197d6; /* fmov fr15,fr6 */
L8c0197d6: target=r[3];*pr=0x8c0197dau;*stop=0x8c0197d8u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0197da; /* jsr @r3 */
L8c0197da: *stop=0x8c0197dau;if(!valid_ram(r[14],1u))return -3;r[0]=sx(m,r[14],1);goto L8c0197dc; /* mov.b @r14,r0 */
L8c0197dc: *stop=0x8c0197dcu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c0197de; /* cmp/eq #0x0,r0 */
L8c0197de: if((*sr&1u))goto L8c0197ec;goto L8c0197e0; /* bt 0x8c0197ec */
L8c0197e0: *stop=0x8c0197e0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0197e2; /* cmp/eq #0x1,r0 */
L8c0197e2: if((*sr&1u))goto L8c019880;goto L8c0197e4; /* bt 0x8c019880 */
L8c0197e4: *stop=0x8c0197e4u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c0197e6; /* cmp/eq #0x2,r0 */
L8c0197e6: if((*sr&1u))goto L8c0198b2;goto L8c0197e8; /* bt 0x8c0198b2 */
L8c0197e8: *stop=0x8c0197eau;goto L8c019940; /* bra 0x8c019940 */
L8c0197ec: *stop=0x8c0197ecu;r[2]=rd(m,0x8c019840u,4);goto L8c0197ee; /* mov.l 0x8c019840,r2 */
L8c0197ee: *stop=0x8c0197eeu;r[4]=1u;goto L8c0197f0; /* mov #0x1,r4 */
L8c0197f0: *stop=0x8c0197f0u;r[3]=rd(m,0x8c019860u,4);goto L8c0197f2; /* mov.l 0x8c019860,r3 */
L8c0197f2: *stop=0x8c0197f2u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[14],4);goto L8c0197f4; /* mov.l r14,@r2 */
L8c0197f4: *stop=0x8c0197f4u;if(!valid_ram(r[14],1u))return -3;wr(m,r[14],r[4],1);goto L8c0197f6; /* mov.b r4,@r14 */
L8c0197f6: *stop=0x8c0197f8u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[4],4);goto L8c019940; /* bra 0x8c019940 */
L8c019880: *stop=0x8c019880u;r[2]=rd(m,0x8c019a20u,4);goto L8c019882; /* mov.l 0x8c019a20,r2 */
L8c019882: *stop=0x8c019882u;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto L8c019884; /* mov.l @r2,r0 */
L8c019884: *stop=0x8c019884u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c019886; /* cmp/eq #-0x1,r0 */
L8c019886: take=(*sr&1u);*stop=0x8c019888u;r[13]=r[0];if(take)goto L8c019892;goto L8c01988a; /* bt/s 0x8c019892 */
L8c01988a: *stop=0x8c01988au;r[1]=rd(m,0x8c019a24u,4);goto L8c01988c; /* mov.l 0x8c019a24,r1 */
L8c01988c: *stop=0x8c01988cu;r[5]=0u;goto L8c01988e; /* mov #0x0,r5 */
L8c01988e: target=r[1];*pr=0x8c019892u;*stop=0x8c019890u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019892; /* jsr @r1 */
L8c019892: target=0x8c019400u;*pr=0x8c019896u;*stop=0x8c019894u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019896; /* bsr 0x8c019400 */
L8c019896: target=0x8c019f4au;*pr=0x8c01989au;*stop=0x8c019898u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01989a; /* bsr 0x8c019f4a */
L8c01989a: target=0x8c01a100u;*pr=0x8c01989eu;*stop=0x8c01989cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01989e; /* bsr 0x8c01a100 */
L8c01989e: *stop=0x8c01989eu;r[2]=2u;goto L8c0198a0; /* mov #0x2,r2 */
L8c0198a0: *stop=0x8c0198a0u;if(!valid_ram(r[14],1u))return -3;wr(m,r[14],r[2],1);goto L8c0198a2; /* mov.b r2,@r14 */
L8c0198a2: *stop=0x8c0198a2u;r[2]=rd(m,0x8c019a28u,4);goto L8c0198a4; /* mov.l 0x8c019a28,r2 */
L8c0198a4: *stop=0x8c0198a4u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto L8c0198a6; /* mov.l @r15,r3 */
L8c0198a6: *stop=0x8c0198a6u;if(!valid_ram(r[3]+20u,4u))return -3;wr(m,r[3]+0x14u,r[2],4);goto L8c0198a8; /* mov.l r2,@(0x14,r3) */
L8c0198a8: *stop=0x8c0198a8u;r[3]=rd(m,0x8c019a2cu,4);goto L8c0198aa; /* mov.l 0x8c019a2c,r3 */
L8c0198aa: target=r[3];*pr=0x8c0198aeu;*stop=0x8c0198acu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0198ae; /* jsr @r3 */
L8c0198ae: *stop=0x8c0198b0u;goto L8c019940; /* bra 0x8c019940 */
L8c0198b2: *stop=0x8c0198b2u;r[4]=rd(m,0x8c019a30u,4);goto L8c0198b4; /* mov.l 0x8c019a30,r4 */
L8c0198b4: *stop=0x8c0198b4u;r[12]=rd(m,0x8c019a34u,4);goto L8c0198b6; /* mov.l 0x8c019a34,r12 */
L8c0198b6: *stop=0x8c0198b6u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c0198b8; /* mov.l @r4,r3 */
L8c0198b8: *stop=0x8c0198b8u;r[3]+=1u;goto L8c0198ba; /* add #0x1,r3 */
L8c0198ba: *stop=0x8c0198bau;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[3],4);goto L8c0198bc; /* mov.l r3,@r4 */
L8c0198bc: *stop=0x8c0198bcu;if(!valid_ram(r[12],4u))return -3;r[0]=rd(m,r[12],4);goto L8c0198be; /* mov.l @r12,r0 */
L8c0198be: *stop=0x8c0198beu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c0198c0; /* cmp/eq #0x2,r0 */
L8c0198c0: take=!(*sr&1u);*stop=0x8c0198c2u;r[13]=0u;if(take)goto L8c0198ca;goto L8c0198c4; /* bf/s 0x8c0198ca */
L8c0198c4: *stop=0x8c0198c4u;r[3]=rd(m,0x8c019a38u,4);goto L8c0198c6; /* mov.l 0x8c019a38,r3 */
L8c0198c6: target=r[3];*pr=0x8c0198cau;*stop=0x8c0198c8u;if(!valid_ram(r[12],4u))return -3;wr(m,r[12],r[13],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0198ca; /* jsr @r3 */
L8c0198ca: *stop=0x8c0198cau;r[2]=rd(m,0x8c019a3cu,4);goto L8c0198cc; /* mov.l 0x8c019a3c,r2 */
L8c0198cc: target=r[2];*pr=0x8c0198d0u;*stop=0x8c0198ceu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0198d0; /* jsr @r2 */
L8c0198d0: *stop=0x8c0198d0u;r[3]=rd(m,0x8c019a40u,4);goto L8c0198d2; /* mov.l 0x8c019a40,r3 */
L8c0198d2: target=r[3];*pr=0x8c0198d6u;*stop=0x8c0198d4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0198d6; /* jsr @r3 */
L8c0198d6: target=0x8c01995eu;*pr=0x8c0198dau;*stop=0x8c0198d8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0198da; /* bsr 0x8c01995e */
L8c0198da: *stop=0x8c0198dau;r[5]=rd(m,0x8c019a44u,4);goto L8c0198dc; /* mov.l 0x8c019a44,r5 */
L8c0198dc: *stop=0x8c0198dcu;if(!valid_ram(r[5],4u))return -3;r[4]=rd(m,r[5],4);goto L8c0198de; /* mov.l @r5,r4 */
L8c0198de: *stop=0x8c0198deu;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c0198e0; /* mov.l @r4,r3 */
L8c0198e0: *stop=0x8c0198e0u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0198e2; /* tst r3,r3 */
L8c0198e2: if(!(*sr&1u))goto L8c0198ec;goto L8c0198e4; /* bf 0x8c0198ec */
L8c0198e4: *stop=0x8c0198e4u;if(!valid_ram(r[4]+20u,4u))return -3;r[3]=rd(m,r[4]+0x14u,4);goto L8c0198e6; /* mov.l @(0x14,r4),r3 */
L8c0198e6: *stop=0x8c0198e6u;r[3]+=1u;goto L8c0198e8; /* add #0x1,r3 */
L8c0198e8: *stop=0x8c0198eau;if(!valid_ram(r[4]+20u,4u))return -3;wr(m,r[4]+0x14u,r[3],4);goto L8c019910; /* bra 0x8c019910 */
L8c0198ec: *stop=0x8c0198ecu;if(!valid_ram(r[4]+20u,4u))return -3;wr(m,r[4]+0x14u,r[13],4);goto L8c0198ee; /* mov.l r13,@(0x14,r4) */
L8c0198ee: *stop=0x8c0198eeu;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c0198f0; /* mov.l @r5,r3 */
L8c0198f0: *stop=0x8c0198f0u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[13],4);goto L8c0198f2; /* mov.l r13,@r3 */
L8c0198f2: *stop=0x8c0198f2u;if(!valid_ram(r[5],4u))return -3;r[4]=rd(m,r[5],4);goto L8c0198f4; /* mov.l @r5,r4 */
L8c0198f4: *stop=0x8c0198f4u;if(!valid_ram(r[4]+9u,1u))return -3;r[0]=sx(m,r[4]+0x9u,1);goto L8c0198f6; /* mov.b @(0x9,r4),r0 */
L8c0198f6: *stop=0x8c0198f6u;r[4]=r[0];goto L8c0198f8; /* mov r0,r4 */
L8c0198f8: *stop=0x8c0198f8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c0198fa; /* tst r4,r4 */
L8c0198fa: if(!(*sr&1u))goto L8c019910;goto L8c0198fc; /* bf 0x8c019910 */
L8c0198fc: *stop=0x8c0198fcu;r[2]=rd(m,0x8c019a48u,4);goto L8c0198fe; /* mov.l 0x8c019a48,r2 */
L8c0198fe: *stop=0x8c0198feu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c019900; /* mov.l @r2,r3 */
L8c019900: *stop=0x8c019900u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c019902; /* tst r3,r3 */
L8c019902: if((*sr&1u))goto L8c019910;goto L8c019904; /* bt 0x8c019910 */
L8c019904: *stop=0x8c019904u;r[3]=rd(m,0x8c019a4cu,4);goto L8c019906; /* mov.l 0x8c019a4c,r3 */
L8c019906: target=r[3];*pr=0x8c01990au;*stop=0x8c019908u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01990a; /* jsr @r3 */
L8c01990a: *stop=0x8c01990au;r[4]=r[0];goto L8c01990c; /* mov r0,r4 */
L8c01990c: *stop=0x8c01990cu;r[0]=30u;goto L8c01990e; /* mov #0x1e,r0 */
L8c01990e: *stop=0x8c01990eu;if(!valid_ram(r[4]+10u,2u))return -3;wr(m,r[4]+0xau,r[0],2);goto L8c019910; /* mov.w r0,@(0xa,r4) */
L8c019910: *stop=0x8c019910u;r[3]=rd(m,0x8c019a48u,4);goto L8c019912; /* mov.l 0x8c019a48,r3 */
L8c019912: *stop=0x8c019912u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[13],4);goto L8c019914; /* mov.l r13,@r3 */
L8c019914: target=0x8c019f4au;*pr=0x8c019918u;*stop=0x8c019916u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019918; /* bsr 0x8c019f4a */
L8c019918: target=0x8c01a100u;*pr=0x8c01991cu;*stop=0x8c01991au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01991c; /* bsr 0x8c01a100 */
L8c01991c: *stop=0x8c01991cu;r[2]=rd(m,0x8c019a50u,4);goto L8c01991e; /* mov.l 0x8c019a50,r2 */
L8c01991e: target=r[2];*pr=0x8c019922u;*stop=0x8c019920u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019922; /* jsr @r2 */
L8c019922: *stop=0x8c019922u;r[3]=rd(m,0x8c019a54u,4);goto L8c019924; /* mov.l 0x8c019a54,r3 */
L8c019924: *stop=0x8c019924u;r[2]=rd(m,0x8c019a58u,4);goto L8c019926; /* mov.l 0x8c019a58,r2 */
L8c019926: *stop=0x8c019926u;if(!valid_ram(r[3],4u))return -3;r[5]=rd(m,r[3],4);goto L8c019928; /* mov.l @r3,r5 */
L8c019928: target=r[2];*pr=0x8c01992cu;*stop=0x8c01992au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01992c; /* jsr @r2 */
L8c01992c: *stop=0x8c01992cu;r[3]=rd(m,0x8c019a5cu,4);goto L8c01992e; /* mov.l 0x8c019a5c,r3 */
L8c01992e: target=r[3];*pr=0x8c019932u;*stop=0x8c019930u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019932; /* jsr @r3 */
L8c019932: target=0x8c01a5aeu;*pr=0x8c019936u;*stop=0x8c019934u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019936; /* bsr 0x8c01a5ae */
L8c019936: *stop=0x8c019936u;if(!valid_ram(r[12],4u))return -3;r[0]=rd(m,r[12],4);goto L8c019938; /* mov.l @r12,r0 */
L8c019938: *stop=0x8c019938u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c01993a; /* cmp/eq #0x1,r0 */
L8c01993a: if(!(*sr&1u))goto L8c019940;goto L8c01993c; /* bf 0x8c019940 */
L8c01993c: *stop=0x8c01993cu;r[2]=2u;goto L8c01993e; /* mov #0x2,r2 */
L8c01993e: *stop=0x8c01993eu;if(!valid_ram(r[12],4u))return -3;wr(m,r[12],r[2],4);goto L8c019940; /* mov.l r2,@r12 */
L8c019940: *stop=0x8c019940u;r[4]=rd(m,0x8c019a60u,4);goto L8c019942; /* mov.l 0x8c019a60,r4 */
L8c019942: *stop=0x8c019942u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019944; /* mov.l @r4,r3 */
L8c019944: *stop=0x8c019944u;r[3]+=1u;goto L8c019946; /* add #0x1,r3 */
L8c019946: *stop=0x8c019946u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[3],4);goto L8c019948; /* mov.l r3,@r4 */
L8c019948: *stop=0x8c019948u;r[4]=rd(m,0x8c019a64u,4);goto L8c01994a; /* mov.l 0x8c019a64,r4 */
L8c01994a: *stop=0x8c01994au;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c01994c; /* mov.l @r4,r2 */
L8c01994c: *stop=0x8c01994cu;r[2]+=1u;goto L8c01994e; /* add #0x1,r2 */
L8c01994e: *stop=0x8c01994eu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[2],4);goto L8c019950; /* mov.l r2,@r4 */
L8c019950: *stop=0x8c019950u;r[15]+=4u;goto L8c019952; /* add #0x4,r15 */
L8c019952: *stop=0x8c019952u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019954; /* lds.l @r15+,PR */
L8c019954: *stop=0x8c019954u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c019956; /* fmov.s @r15+,fr15 */
L8c019956: *stop=0x8c019956u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c019958; /* mov.l @r15+,r12 */
L8c019958: *stop=0x8c019958u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01995a; /* mov.l @r15+,r13 */
L8c01995a: *stop=0x8c01995au;if(*pr!=entry_pr)return -3;*stop=0x8c01995cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_model_camera_8c019760(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c019760;
L8c019760: *stop=0x8c019760u;r[2]=rd(m,0x8c019840u,4);goto L8c019762; /* mov.l 0x8c019840,r2 */
L8c019762: *stop=0x8c019762u;r[3]=0u;goto L8c019764; /* mov #0x0,r3 */
L8c019764: *stop=0x8c019764u;if(*pr!=entry_pr)return -3;*stop=0x8c019766u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_model_camera_8c019768(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c019768;
L8c019768: *stop=0x8c01976au;if(!valid_ram(r[4]+32u,4u))return -3;r[4]=rd(m,r[4]+0x20u,4);if(!call)return -2;return call(ctx,0x8c01a100u,m,f,sr,r,pr); /* bra 0x8c01a100 */
}

SA_STAGE_EXPORT int sa_model_camera_8c09878c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09878c;
L8c09878c: *stop=0x8c09878cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09878e; /* mov.l r14,@-r15 */
L8c09878e: *stop=0x8c09878eu;r[0]=r[4];goto L8c098790; /* mov r4,r0 */
L8c098790: *stop=0x8c098790u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c098792; /* mov.l r13,@-r15 */
L8c098792: *stop=0x8c098792u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c098794; /* cmp/eq #0x8,r0 */
L8c098794: *stop=0x8c098794u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c098796; /* mov.l r12,@-r15 */
L8c098796: *stop=0x8c098796u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c098798; /* mov.l r11,@-r15 */
L8c098798: *stop=0x8c098798u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c09879a; /* sts.l PR,@-r15 */
L8c09879a: if(!(*sr&1u))goto L8c09879e;goto L8c09879c; /* bf 0x8c09879e */
L8c09879c: *stop=0x8c09879cu;r[4]=7u;goto L8c09879e; /* mov #0x7,r4 */
L8c09879e: *stop=0x8c09879eu;r[0]=rd(m,0x8c0987e0u,4);goto L8c0987a0; /* mov.l 0x8c0987e0,r0 */
L8c0987a0: *stop=0x8c0987a0u;r[4]<<=2;goto L8c0987a2; /* shll2 r4 */
L8c0987a2: *stop=0x8c0987a2u;if(!valid_ram(r[0]+r[4],4u))return -3;r[4]=rd(m,r[4]+r[0],4);goto L8c0987a4; /* mov.l @(r0,r4),r4 */
L8c0987a4: *stop=0x8c0987a4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c0987a6; /* tst r4,r4 */
L8c0987a6: if((*sr&1u))goto L8c0987c2;goto L8c0987a8; /* bt 0x8c0987c2 */
L8c0987a8: *stop=0x8c0987a8u;r[13]=rd(m,0x8c0987ecu,4);goto L8c0987aa; /* mov.l 0x8c0987ec,r13 */
L8c0987aa: *stop=0x8c0987aau;r[11]=0u;goto L8c0987ac; /* mov #0x0,r11 */
L8c0987ac: *stop=0x8c0987acu;if(!valid_ram(r[4]+16u,4u))return -3;r[14]=rd(m,r[4]+0x10u,4);goto L8c0987ae; /* mov.l @(0x10,r4),r14 */
L8c0987ae: *stop=0x8c0987aeu;if(!valid_ram(r[4],4u))return -3;r[12]=rd(m,r[4],4);goto L8c0987b0; /* mov.l @r4,r12 */
L8c0987b0: *stop=0x8c0987b0u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0987b2; /* tst r14,r14 */
L8c0987b2: take=(*sr&1u);*stop=0x8c0987b4u;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[14],4);if(take)goto L8c0987ba;goto L8c0987b6; /* bt/s 0x8c0987ba */
L8c0987b6: target=r[14];*pr=0x8c0987bau;*stop=0x8c0987b8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0987ba; /* jsr @r14 */
L8c0987ba: *stop=0x8c0987bau;r[4]=r[12];goto L8c0987bc; /* mov r12,r4 */
L8c0987bc: *stop=0x8c0987bcu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c0987be; /* tst r4,r4 */
L8c0987be: take=!(*sr&1u);*stop=0x8c0987c0u;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[11],4);if(take)goto L8c0987ac;goto L8c0987c2; /* bf/s 0x8c0987ac */
L8c0987c2: *stop=0x8c0987c2u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c0987c4; /* lds.l @r15+,PR */
L8c0987c4: *stop=0x8c0987c4u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c0987c6; /* mov.l @r15+,r11 */
L8c0987c6: *stop=0x8c0987c6u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0987c8; /* mov.l @r15+,r12 */
L8c0987c8: *stop=0x8c0987c8u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0987ca; /* mov.l @r15+,r13 */
L8c0987ca: *stop=0x8c0987cau;if(*pr!=entry_pr)return -3;*stop=0x8c0987ccu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
