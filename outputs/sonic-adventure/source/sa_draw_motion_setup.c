/* Motion setup, recursive dispatch and base callbacks; names inferred.
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
SA_STAGE_EXPORT int sa_motion_setup_8c60e5e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60e5e4;
L8c60e5e4: *stop=0x8c60e5e4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60e5e6; /* mov.l r14,@-r15 */
L8c60e5e6: *stop=0x8c60e5e6u;r[14]=r[4];goto L8c60e5e8; /* mov r4,r14 */
L8c60e5e8: *stop=0x8c60e5e8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60e5ea; /* mov.l r13,@-r15 */
L8c60e5ea: *stop=0x8c60e5eau;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c60e5ec; /* tst r5,r5 */
L8c60e5ec: *stop=0x8c60e5ecu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60e5ee; /* mov.l r12,@-r15 */
L8c60e5ee: *stop=0x8c60e5eeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60e5f0; /* sts.l PR,@-r15 */
L8c60e5f0: *stop=0x8c60e5f0u;r[6]=rd(m,0x8c60e778u,4);goto L8c60e5f2; /* mov.l 0x8c60e778,r6 */
L8c60e5f2: *stop=0x8c60e5f2u;r[4]=rd(m,0x8c60e77cu,4);goto L8c60e5f4; /* mov.l 0x8c60e77c,r4 */
L8c60e5f4: *stop=0x8c60e5f4u;r[12]=rd(m,0x8c60e770u,4);goto L8c60e5f6; /* mov.l 0x8c60e770,r12 */
L8c60e5f6: *stop=0x8c60e5f6u;r[13]=rd(m,0x8c60e774u,4);goto L8c60e5f8; /* mov.l 0x8c60e774,r13 */
L8c60e5f8: take=(*sr&1u);*stop=0x8c60e5fau;r[7]=0u;if(take)goto L8c60e634;goto L8c60e5fc; /* bt/s 0x8c60e634 */
L8c60e5fc: *stop=0x8c60e5fcu;if(!valid_ram(r[5]+4u,4u))return -3;r[2]=rd(m,r[5]+0x4u,4);goto L8c60e5fe; /* mov.l @(0x4,r5),r2 */
L8c60e5fe: *stop=0x8c60e5feu;*fpul=r[2];goto L8c60e600; /* lds r2,FPUL */
L8c60e600: *stop=0x8c60e600u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c60e602; /* cmp/pz r2 */
L8c60e602: take=(*sr&1u);*stop=0x8c60e604u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c60e60e;goto L8c60e606; /* bt/s 0x8c60e60e */
L8c60e606: *stop=0x8c60e606u;r[3]=rd(m,0x8c60e780u,4);goto L8c60e608; /* mov.l 0x8c60e780,r3 */
L8c60e608: *stop=0x8c60e608u;*fpul=r[3];goto L8c60e60a; /* lds r3,FPUL */
L8c60e60a: *stop=0x8c60e60au;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c60e60c; /* fsts FPUL,fr2 */
L8c60e60c: *stop=0x8c60e60cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60e60e; /* fadd fr2,fr3 */
L8c60e60e: *stop=0x8c60e60eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c60e610; /* fcmp/gt fr4,fr3 */
L8c60e610: if(!(*sr&1u))goto L8c60e6d0;goto L8c60e612; /* bf 0x8c60e6d0 */
L8c60e612: *stop=0x8c60e612u;if(!valid_ram(r[5]+10u,2u))return -3;r[0]=sx(m,r[5]+0xau,2);goto L8c60e614; /* mov.w @(0xa,r5),r0 */
L8c60e614: *stop=0x8c60e614u;r[3]=rd(m,0x8c60e784u,4);goto L8c60e616; /* mov.l 0x8c60e784,r3 */
L8c60e616: *stop=0x8c60e616u;r[0]&=r[3];goto L8c60e618; /* and r3,r0 */
L8c60e618: *stop=0x8c60e618u;if(!valid_ram(r[12],2u))return -3;wr(m,r[12],r[0],2);goto L8c60e61a; /* mov.w r0,@r12 */
L8c60e61a: *stop=0x8c60e61au;if(!valid_ram(r[5]+10u,2u))return -3;r[0]=sx(m,r[5]+0xau,2);goto L8c60e61c; /* mov.w @(0xa,r5),r0 */
L8c60e61c: *stop=0x8c60e61cu;r[0]=r[0]&65535u;goto L8c60e61e; /* extu.w r0,r0 */
L8c60e61e: *stop=0x8c60e61eu;r[0]&=0xc0u;goto L8c60e620; /* and #0xc0,r0 */
L8c60e620: *stop=0x8c60e620u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c60e622; /* cmp/pl r0 */
L8c60e622: *stop=0x8c60e622u;r[0]=*sr&1u;goto L8c60e624; /* movt r0 */
L8c60e624: *stop=0x8c60e624u;if(!valid_ram(r[13],2u))return -3;wr(m,r[13],r[0],2);goto L8c60e626; /* mov.w r0,@r13 */
L8c60e626: *stop=0x8c60e626u;if(!valid_ram(r[5]+8u,2u))return -3;r[0]=sx(m,r[5]+0x8u,2);goto L8c60e628; /* mov.w @(0x8,r5),r0 */
L8c60e628: *stop=0x8c60e628u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[0],2);goto L8c60e62a; /* mov.w r0,@r4 */
L8c60e62a: *stop=0x8c60e62au;if(!valid_ram(r[5],4u))return -3;r[2]=rd(m,r[5],4);goto L8c60e62c; /* mov.l @r5,r2 */
L8c60e62c: *stop=0x8c60e62cu;if(!valid_ram(r[6],4u))return -3;wr(m,r[6],r[2],4);goto L8c60e62e; /* mov.l r2,@r6 */
L8c60e62e: *stop=0x8c60e62eu;if(!valid_ram(r[5]+4u,4u))return -3;r[1]=rd(m,r[5]+0x4u,4);goto L8c60e630; /* mov.l @(0x4,r5),r1 */
L8c60e630: *stop=0x8c60e632u;if(!valid_ram(r[6]+16u,4u))return -3;wr(m,r[6]+0x10u,r[1],4);goto L8c60e63a; /* bra 0x8c60e63a */
L8c60e634: *stop=0x8c60e634u;if(!valid_ram(r[12],2u))return -3;wr(m,r[12],r[7],2);goto L8c60e636; /* mov.w r7,@r12 */
L8c60e636: *stop=0x8c60e636u;if(!valid_ram(r[13],2u))return -3;wr(m,r[13],r[7],2);goto L8c60e638; /* mov.w r7,@r13 */
L8c60e638: *stop=0x8c60e638u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[7],2);goto L8c60e63a; /* mov.w r7,@r4 */
L8c60e63a: *stop=0x8c60e63au;r[0]=12u;goto L8c60e63c; /* mov #0xc,r0 */
L8c60e63c: *stop=0x8c60e63cu;r[1]=rd(m,0x8c60e78cu,4);goto L8c60e63e; /* mov.l 0x8c60e78c,r1 */
L8c60e63e: *stop=0x8c60e63eu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[6]);goto L8c60e640; /* fmov fr4,@(r0,r6) */
L8c60e640: *stop=0x8c60e640u;r[0]=rd(m,0x8c60e788u,4);goto L8c60e642; /* mov.l 0x8c60e788,r0 */
L8c60e642: *stop=0x8c60e642u;r[5]=rd(m,0x8c60e790u,4);goto L8c60e644; /* mov.l 0x8c60e790,r5 */
L8c60e644: *stop=0x8c60e644u;if(!valid_ram(r[6]+20u,4u))return -3;wr(m,r[6]+0x14u,r[7],4);goto L8c60e646; /* mov.l r7,@(0x14,r6) */
L8c60e646: *stop=0x8c60e646u;if(!valid_ram(r[6]+24u,4u))return -3;wr(m,r[6]+0x18u,r[7],4);goto L8c60e648; /* mov.l r7,@(0x18,r6) */
L8c60e648: *stop=0x8c60e648u;if(!valid_ram(r[12],2u))return -3;r[3]=sx(m,r[12],2);goto L8c60e64a; /* mov.w @r12,r3 */
L8c60e64a: *stop=0x8c60e64au;r[3]=r[3]&65535u;goto L8c60e64c; /* extu.w r3,r3 */
L8c60e64c: *stop=0x8c60e64cu;r[3]<<=2;goto L8c60e64e; /* shll2 r3 */
L8c60e64e: *stop=0x8c60e64eu;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c60e650; /* mov.l @(r0,r3),r2 */
L8c60e650: *stop=0x8c60e650u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[2],4);goto L8c60e652; /* mov.l r2,@r1 */
L8c60e652: *stop=0x8c60e652u;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60e654; /* mov.w @r4,r0 */
L8c60e654: *stop=0x8c60e654u;r[0]=r[0]&65535u;goto L8c60e656; /* extu.w r0,r0 */
L8c60e656: *stop=0x8c60e656u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c60e658; /* tst #0x1,r0 */
L8c60e658: take=(*sr&1u);*stop=0x8c60e65au;r[6]=32u;if(take)goto L8c60e662;goto L8c60e65c; /* bt/s 0x8c60e662 */
L8c60e65c: *stop=0x8c60e65cu;r[1]=rd(m,0x8c60e794u,4);goto L8c60e65e; /* mov.l 0x8c60e794,r1 */
L8c60e65e: *stop=0x8c60e660u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c60e666; /* bra 0x8c60e666 */
L8c60e662: *stop=0x8c60e662u;r[0]=rd(m,0x8c60e734u,4);goto L8c60e664; /* mov.l 0x8c60e734,r0 */
L8c60e664: *stop=0x8c60e664u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[0],4);goto L8c60e666; /* mov.l r0,@r5 */
L8c60e666: *stop=0x8c60e666u;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60e668; /* mov.w @r4,r0 */
L8c60e668: *stop=0x8c60e668u;r[5]=rd(m,0x8c60e738u,4);goto L8c60e66a; /* mov.l 0x8c60e738,r5 */
L8c60e66a: *stop=0x8c60e66au;r[0]=r[0]&65535u;goto L8c60e66c; /* extu.w r0,r0 */
L8c60e66c: *stop=0x8c60e66cu;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c60e66e; /* tst #0x2,r0 */
L8c60e66e: if((*sr&1u))goto L8c60e682;goto L8c60e670; /* bt 0x8c60e682 */
L8c60e670: *stop=0x8c60e670u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c60e672; /* mov.l @r14,r2 */
L8c60e672: *stop=0x8c60e672u;*sr=(*sr&~1u)|!(r[2]&r[6]);goto L8c60e674; /* tst r2,r6 */
L8c60e674: if((*sr&1u))goto L8c60e67c;goto L8c60e676; /* bt 0x8c60e67c */
L8c60e676: *stop=0x8c60e676u;r[1]=rd(m,0x8c60e73cu,4);goto L8c60e678; /* mov.l 0x8c60e73c,r1 */
L8c60e678: *stop=0x8c60e67au;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c60e692; /* bra 0x8c60e692 */
L8c60e67c: *stop=0x8c60e67cu;r[2]=rd(m,0x8c60e740u,4);goto L8c60e67e; /* mov.l 0x8c60e740,r2 */
L8c60e67e: *stop=0x8c60e680u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c60e692; /* bra 0x8c60e692 */
L8c60e682: *stop=0x8c60e682u;if(!valid_ram(r[14],4u))return -3;r[1]=rd(m,r[14],4);goto L8c60e684; /* mov.l @r14,r1 */
L8c60e684: *stop=0x8c60e684u;*sr=(*sr&~1u)|!(r[1]&r[6]);goto L8c60e686; /* tst r1,r6 */
L8c60e686: if((*sr&1u))goto L8c60e68e;goto L8c60e688; /* bt 0x8c60e68e */
L8c60e688: *stop=0x8c60e688u;r[2]=rd(m,0x8c60e744u,4);goto L8c60e68a; /* mov.l 0x8c60e744,r2 */
L8c60e68a: *stop=0x8c60e68cu;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c60e692; /* bra 0x8c60e692 */
L8c60e68e: *stop=0x8c60e68eu;r[1]=rd(m,0x8c60e748u,4);goto L8c60e690; /* mov.l 0x8c60e748,r1 */
L8c60e690: *stop=0x8c60e690u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c60e692; /* mov.l r1,@r5 */
L8c60e692: *stop=0x8c60e692u;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60e694; /* mov.w @r4,r0 */
L8c60e694: *stop=0x8c60e694u;r[5]=rd(m,0x8c60e74cu,4);goto L8c60e696; /* mov.l 0x8c60e74c,r5 */
L8c60e696: *stop=0x8c60e696u;r[0]=r[0]&65535u;goto L8c60e698; /* extu.w r0,r0 */
L8c60e698: *stop=0x8c60e698u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c60e69a; /* tst #0x4,r0 */
L8c60e69a: if((*sr&1u))goto L8c60e6a2;goto L8c60e69c; /* bt 0x8c60e6a2 */
L8c60e69c: *stop=0x8c60e69cu;r[2]=rd(m,0x8c60e750u,4);goto L8c60e69e; /* mov.l 0x8c60e750,r2 */
L8c60e69e: *stop=0x8c60e6a0u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[2],4);goto L8c60e6a6; /* bra 0x8c60e6a6 */
L8c60e6a2: *stop=0x8c60e6a2u;r[1]=rd(m,0x8c60e754u,4);goto L8c60e6a4; /* mov.l 0x8c60e754,r1 */
L8c60e6a4: *stop=0x8c60e6a4u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c60e6a6; /* mov.l r1,@r5 */
L8c60e6a6: *stop=0x8c60e6a6u;if(!valid_ram(r[13],2u))return -3;r[4]=sx(m,r[13],2);goto L8c60e6a8; /* mov.w @r13,r4 */
L8c60e6a8: *stop=0x8c60e6a8u;r[0]=rd(m,0x8c60e758u,4);goto L8c60e6aa; /* mov.l 0x8c60e758,r0 */
L8c60e6aa: *stop=0x8c60e6aau;r[4]=r[4]&65535u;goto L8c60e6ac; /* extu.w r4,r4 */
L8c60e6ac: *stop=0x8c60e6acu;r[2]=rd(m,0x8c60e75cu,4);goto L8c60e6ae; /* mov.l 0x8c60e75c,r2 */
L8c60e6ae: *stop=0x8c60e6aeu;r[4]<<=2;goto L8c60e6b0; /* shll2 r4 */
L8c60e6b0: *stop=0x8c60e6b0u;r[1]=rd(m,0x8c60e764u,4);goto L8c60e6b2; /* mov.l 0x8c60e764,r1 */
L8c60e6b2: *stop=0x8c60e6b2u;if(!valid_ram(r[0]+r[4],4u))return -3;r[3]=rd(m,r[4]+r[0],4);goto L8c60e6b4; /* mov.l @(r0,r4),r3 */
L8c60e6b4: *stop=0x8c60e6b4u;r[0]=rd(m,0x8c60e760u,4);goto L8c60e6b6; /* mov.l 0x8c60e760,r0 */
L8c60e6b6: *stop=0x8c60e6b6u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c60e6b8; /* mov.l r3,@r2 */
L8c60e6b8: *stop=0x8c60e6b8u;r[3]=rd(m,0x8c60e768u,4);goto L8c60e6ba; /* mov.l 0x8c60e768,r3 */
L8c60e6ba: *stop=0x8c60e6bau;if(!valid_ram(r[0]+r[4],4u))return -3;r[2]=rd(m,r[4]+r[0],4);goto L8c60e6bc; /* mov.l @(r0,r4),r2 */
L8c60e6bc: target=r[3];*pr=0x8c60e6c0u;*stop=0x8c60e6beu;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[2],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e6c0; /* jsr @r3 */
L8c60e6c0: target=0x8c60e31cu;*pr=0x8c60e6c4u;*stop=0x8c60e6c2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e6c4; /* bsr 0x8c60e31c */
L8c60e6c4: *stop=0x8c60e6c4u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60e6c6; /* lds.l @r15+,PR */
L8c60e6c6: *stop=0x8c60e6c6u;r[3]=rd(m,0x8c60e76cu,4);goto L8c60e6c8; /* mov.l 0x8c60e76c,r3 */
L8c60e6c8: *stop=0x8c60e6c8u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60e6ca; /* mov.l @r15+,r12 */
L8c60e6ca: *stop=0x8c60e6cau;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60e6cc; /* mov.l @r15+,r13 */
L8c60e6cc: target=r[3];*stop=0x8c60e6ceu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c60e6d0: *stop=0x8c60e6d0u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60e6d2; /* lds.l @r15+,PR */
L8c60e6d2: *stop=0x8c60e6d2u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60e6d4; /* mov.l @r15+,r12 */
L8c60e6d4: *stop=0x8c60e6d4u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60e6d6; /* mov.l @r15+,r13 */
L8c60e6d6: *stop=0x8c60e6d6u;if(*pr!=entry_pr)return -3;*stop=0x8c60e6d8u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_dispatch_8c60e31c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60e31c;
L8c60e31c: *stop=0x8c60e31cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60e31e; /* mov.l r14,@-r15 */
L8c60e31e: *stop=0x8c60e31eu;r[14]=r[4];goto L8c60e320; /* mov r4,r14 */
L8c60e320: *stop=0x8c60e320u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60e322; /* mov.l r13,@-r15 */
L8c60e322: *stop=0x8c60e322u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60e324; /* mov.l r12,@-r15 */
L8c60e324: *stop=0x8c60e324u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60e326; /* sts.l PR,@-r15 */
L8c60e326: *stop=0x8c60e326u;r[3]=rd(m,0x8c60e520u,4);goto L8c60e328; /* mov.l 0x8c60e520,r3 */
L8c60e328: target=r[3];*pr=0x8c60e32cu;*stop=0x8c60e32au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e32c; /* jsr @r3 */
L8c60e32c: *stop=0x8c60e32cu;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c60e32e; /* mov.l @r14,r0 */
L8c60e32e: *stop=0x8c60e32eu;*sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c60e330; /* tst #0x40,r0 */
L8c60e330: if(!(*sr&1u))goto L8c60e33e;goto L8c60e332; /* bf 0x8c60e33e */
L8c60e332: *stop=0x8c60e332u;r[2]=rd(m,0x8c60e524u,4);goto L8c60e334; /* mov.l 0x8c60e524,r2 */
L8c60e334: *stop=0x8c60e334u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60e336; /* mov.l @r2,r3 */
L8c60e336: target=r[3];*pr=0x8c60e33au;*stop=0x8c60e338u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e33a; /* jsr @r3 */
L8c60e33a: *stop=0x8c60e33cu;goto L8c60e390; /* bra 0x8c60e390 */
L8c60e33e: *stop=0x8c60e33eu;r[12]=r[14];goto L8c60e340; /* mov r14,r12 */
L8c60e340: *stop=0x8c60e340u;r[0]=8u;goto L8c60e342; /* mov #0x8,r0 */
L8c60e342: *stop=0x8c60e342u;r[12]+=8u;goto L8c60e344; /* add #0x8,r12 */
L8c60e344: *stop=0x8c60e344u;r[3]=rd(m,0x8c60e52cu,4);goto L8c60e346; /* mov.l 0x8c60e52c,r3 */
L8c60e346: *stop=0x8c60e346u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[12]);goto L8c60e348; /* fmov.s @(r0,r12),fr6 */
L8c60e348: *stop=0x8c60e348u;r[0]=4u;goto L8c60e34a; /* mov #0x4,r0 */
L8c60e34a: *stop=0x8c60e34au;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[12]);goto L8c60e34c; /* fmov.s @r12,fr4 */
L8c60e34c: *stop=0x8c60e34cu;r[13]=rd(m,0x8c60e528u,4);goto L8c60e34e; /* mov.l 0x8c60e528,r13 */
L8c60e34e: *stop=0x8c60e34eu;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[12]);goto L8c60e350; /* fmov.s @(r0,r12),fr5 */
L8c60e350: target=r[3];*pr=0x8c60e354u;*stop=0x8c60e352u;if(!valid_ram(r[13]+24u,4u))return -3;r[4]=rd(m,r[13]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e354; /* jsr @r3 */
L8c60e354: *stop=0x8c60e354u;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c60e356; /* mov.l @r14,r0 */
L8c60e356: *stop=0x8c60e356u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c60e358; /* tst #0x20,r0 */
L8c60e358: take=(*sr&1u);*stop=0x8c60e35au;if(!valid_ram(r[13]+24u,4u))return -3;r[4]=rd(m,r[13]+0x18u,4);if(take)goto L8c60e36e;goto L8c60e35c; /* bt/s 0x8c60e36e */
L8c60e35c: *stop=0x8c60e35cu;r[12]=r[14];goto L8c60e35e; /* mov r14,r12 */
L8c60e35e: *stop=0x8c60e35eu;r[12]+=20u;goto L8c60e360; /* add #0x14,r12 */
L8c60e360: *stop=0x8c60e360u;r[3]=rd(m,0x8c60e530u,4);goto L8c60e362; /* mov.l 0x8c60e530,r3 */
L8c60e362: *stop=0x8c60e362u;if(!valid_ram(r[12]+4u,4u))return -3;r[6]=rd(m,r[12]+0x4u,4);goto L8c60e364; /* mov.l @(0x4,r12),r6 */
L8c60e364: *stop=0x8c60e364u;if(!valid_ram(r[12]+8u,4u))return -3;r[7]=rd(m,r[12]+0x8u,4);goto L8c60e366; /* mov.l @(0x8,r12),r7 */
L8c60e366: target=r[3];*pr=0x8c60e36au;*stop=0x8c60e368u;if(!valid_ram(r[12],4u))return -3;r[5]=rd(m,r[12],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e36a; /* jsr @r3 */
L8c60e36a: *stop=0x8c60e36cu;goto L8c60e37c; /* bra 0x8c60e37c */
L8c60e36e: *stop=0x8c60e36eu;r[12]=r[14];goto L8c60e370; /* mov r14,r12 */
L8c60e370: *stop=0x8c60e370u;r[12]+=20u;goto L8c60e372; /* add #0x14,r12 */
L8c60e372: *stop=0x8c60e372u;r[3]=rd(m,0x8c60e534u,4);goto L8c60e374; /* mov.l 0x8c60e534,r3 */
L8c60e374: *stop=0x8c60e374u;if(!valid_ram(r[12]+4u,4u))return -3;r[6]=rd(m,r[12]+0x4u,4);goto L8c60e376; /* mov.l @(0x4,r12),r6 */
L8c60e376: *stop=0x8c60e376u;if(!valid_ram(r[12]+8u,4u))return -3;r[7]=rd(m,r[12]+0x8u,4);goto L8c60e378; /* mov.l @(0x8,r12),r7 */
L8c60e378: target=r[3];*pr=0x8c60e37cu;*stop=0x8c60e37au;if(!valid_ram(r[12],4u))return -3;r[5]=rd(m,r[12],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e37c; /* jsr @r3 */
L8c60e37c: *stop=0x8c60e37cu;r[12]=r[14];goto L8c60e37e; /* mov r14,r12 */
L8c60e37e: *stop=0x8c60e37eu;r[0]=8u;goto L8c60e380; /* mov #0x8,r0 */
L8c60e380: *stop=0x8c60e380u;r[12]+=32u;goto L8c60e382; /* add #0x20,r12 */
L8c60e382: *stop=0x8c60e382u;r[3]=rd(m,0x8c60e538u,4);goto L8c60e384; /* mov.l 0x8c60e538,r3 */
L8c60e384: *stop=0x8c60e384u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[12]);goto L8c60e386; /* fmov.s @(r0,r12),fr6 */
L8c60e386: *stop=0x8c60e386u;r[0]=4u;goto L8c60e388; /* mov #0x4,r0 */
L8c60e388: *stop=0x8c60e388u;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[12]);goto L8c60e38a; /* fmov.s @r12,fr4 */
L8c60e38a: *stop=0x8c60e38au;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[12]);goto L8c60e38c; /* fmov.s @(r0,r12),fr5 */
L8c60e38c: target=r[3];*pr=0x8c60e390u;*stop=0x8c60e38eu;if(!valid_ram(r[13]+24u,4u))return -3;r[4]=rd(m,r[13]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e390; /* jsr @r3 */
L8c60e390: *stop=0x8c60e390u;r[2]=rd(m,0x8c60e53cu,4);goto L8c60e392; /* mov.l 0x8c60e53c,r2 */
L8c60e392: *stop=0x8c60e392u;if(!valid_ram(r[2],4u))return -3;r[13]=rd(m,r[2],4);goto L8c60e394; /* mov.l @r2,r13 */
L8c60e394: *stop=0x8c60e394u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c60e396; /* tst r13,r13 */
L8c60e396: if((*sr&1u))goto L8c60e39c;goto L8c60e398; /* bt 0x8c60e39c */
L8c60e398: target=r[13];*pr=0x8c60e39cu;*stop=0x8c60e39au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e39c; /* jsr @r13 */
L8c60e39c: *stop=0x8c60e39cu;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c60e39e; /* mov.l @(0x4,r14),r2 */
L8c60e39e: *stop=0x8c60e39eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c60e3a0; /* tst r2,r2 */
L8c60e3a0: if((*sr&1u))goto L8c60e3ae;goto L8c60e3a2; /* bt 0x8c60e3ae */
L8c60e3a2: *stop=0x8c60e3a2u;if(!valid_ram(r[14],4u))return -3;r[0]=rd(m,r[14],4);goto L8c60e3a4; /* mov.l @r14,r0 */
L8c60e3a4: *stop=0x8c60e3a4u;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c60e3a6; /* tst #0x8,r0 */
L8c60e3a6: if(!(*sr&1u))goto L8c60e3ae;goto L8c60e3a8; /* bf 0x8c60e3ae */
L8c60e3a8: *stop=0x8c60e3a8u;r[2]=rd(m,0x8c60e540u,4);goto L8c60e3aa; /* mov.l 0x8c60e540,r2 */
L8c60e3aa: target=r[2];*pr=0x8c60e3aeu;*stop=0x8c60e3acu;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e3ae; /* jsr @r2 */
L8c60e3ae: *stop=0x8c60e3aeu;if(!valid_ram(r[14]+44u,4u))return -3;r[3]=rd(m,r[14]+0x2cu,4);goto L8c60e3b0; /* mov.l @(0x2c,r14),r3 */
L8c60e3b0: *stop=0x8c60e3b0u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c60e3b2; /* tst r3,r3 */
L8c60e3b2: if((*sr&1u))goto L8c60e3b8;goto L8c60e3b4; /* bt 0x8c60e3b8 */
L8c60e3b4: target=0x8c60e31cu;*pr=0x8c60e3b8u;*stop=0x8c60e3b6u;if(!valid_ram(r[14]+44u,4u))return -3;r[4]=rd(m,r[14]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e3b8; /* bsr 0x8c60e31c */
L8c60e3b8: *stop=0x8c60e3b8u;r[2]=rd(m,0x8c60e544u,4);goto L8c60e3ba; /* mov.l 0x8c60e544,r2 */
L8c60e3ba: target=r[2];*pr=0x8c60e3beu;*stop=0x8c60e3bcu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60e3be; /* jsr @r2 */
L8c60e3be: *stop=0x8c60e3beu;if(!valid_ram(r[14]+48u,4u))return -3;r[3]=rd(m,r[14]+0x30u,4);goto L8c60e3c0; /* mov.l @(0x30,r14),r3 */
L8c60e3c0: *stop=0x8c60e3c0u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c60e3c2; /* tst r3,r3 */
L8c60e3c2: if((*sr&1u))goto L8c60e3c8;goto L8c60e3c4; /* bt 0x8c60e3c8 */
L8c60e3c4: *stop=0x8c60e3c6u;if(!valid_ram(r[14]+48u,4u))return -3;r[14]=rd(m,r[14]+0x30u,4);goto L8c60e326; /* bra 0x8c60e326 */
L8c60e3c8: *stop=0x8c60e3c8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60e3ca; /* lds.l @r15+,PR */
L8c60e3ca: *stop=0x8c60e3cau;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60e3cc; /* mov.l @r15+,r12 */
L8c60e3cc: *stop=0x8c60e3ccu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60e3ce; /* mov.l @r15+,r13 */
L8c60e3ce: *stop=0x8c60e3ceu;if(*pr!=entry_pr)return -3;*stop=0x8c60e3d0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_base_position_8c60cfc4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60cfc4;
L8c60cfc4: *stop=0x8c60cfc4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60cfc6; /* mov.l r14,@-r15 */
L8c60cfc6: *stop=0x8c60cfc6u;r[14]=r[4];goto L8c60cfc8; /* mov r4,r14 */
L8c60cfc8: *stop=0x8c60cfc8u;r[0]=8u;goto L8c60cfca; /* mov #0x8,r0 */
L8c60cfca: *stop=0x8c60cfcau;r[3]=rd(m,0x8c60d11cu,4);goto L8c60cfcc; /* mov.l 0x8c60d11c,r3 */
L8c60cfcc: *stop=0x8c60cfccu;r[14]+=8u;goto L8c60cfce; /* add #0x8,r14 */
L8c60cfce: *stop=0x8c60cfceu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[4],4);goto L8c60cfd0; /* mov.l r4,@-r15 */
L8c60cfd0: *stop=0x8c60cfd0u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[14]);goto L8c60cfd2; /* fmov.s @(r0,r14),fr6 */
L8c60cfd2: *stop=0x8c60cfd2u;r[0]=4u;goto L8c60cfd4; /* mov #0x4,r0 */
L8c60cfd4: *stop=0x8c60cfd4u;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);goto L8c60cfd6; /* mov.l @r3,r4 */
L8c60cfd6: *stop=0x8c60cfd6u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto L8c60cfd8; /* fmov.s @(r0,r14),fr5 */
L8c60cfd8: *stop=0x8c60cfd8u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[14]);goto L8c60cfda; /* fmov.s @r14,fr4 */
L8c60cfda: *stop=0x8c60cfdau;r[15]+=4u;goto L8c60cfdc; /* add #0x4,r15 */
L8c60cfdc: *stop=0x8c60cfdcu;r[2]=rd(m,0x8c60d120u,4);goto L8c60cfde; /* mov.l 0x8c60d120,r2 */
L8c60cfde: target=r[2];*stop=0x8c60cfe0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_motion_base_rotation_8c60cfe2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60cfe2;
L8c60cfe2: *stop=0x8c60cfe2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60cfe4; /* mov.l r14,@-r15 */
L8c60cfe4: *stop=0x8c60cfe4u;r[14]=r[4];goto L8c60cfe6; /* mov r4,r14 */
L8c60cfe6: *stop=0x8c60cfe6u;r[3]=rd(m,0x8c60d11cu,4);goto L8c60cfe8; /* mov.l 0x8c60d11c,r3 */
L8c60cfe8: *stop=0x8c60cfe8u;r[14]+=20u;goto L8c60cfea; /* add #0x14,r14 */
L8c60cfea: *stop=0x8c60cfeau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[4],4);goto L8c60cfec; /* mov.l r4,@-r15 */
L8c60cfec: *stop=0x8c60cfecu;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);goto L8c60cfee; /* mov.l @r3,r4 */
L8c60cfee: *stop=0x8c60cfeeu;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c60cff0; /* mov.l @r14,r5 */
L8c60cff0: *stop=0x8c60cff0u;if(!valid_ram(r[14]+8u,4u))return -3;r[7]=rd(m,r[14]+0x8u,4);goto L8c60cff2; /* mov.l @(0x8,r14),r7 */
L8c60cff2: *stop=0x8c60cff2u;if(!valid_ram(r[14]+4u,4u))return -3;r[6]=rd(m,r[14]+0x4u,4);goto L8c60cff4; /* mov.l @(0x4,r14),r6 */
L8c60cff4: *stop=0x8c60cff4u;r[15]+=4u;goto L8c60cff6; /* add #0x4,r15 */
L8c60cff6: *stop=0x8c60cff6u;r[2]=rd(m,0x8c60d124u,4);goto L8c60cff8; /* mov.l 0x8c60d124,r2 */
L8c60cff8: target=r[2];*stop=0x8c60cffau;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_motion_base_rotation_yxz_8c60cffc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60cffc;
L8c60cffc: *stop=0x8c60cffcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60cffe; /* mov.l r14,@-r15 */
L8c60cffe: *stop=0x8c60cffeu;r[14]=r[4];goto L8c60d000; /* mov r4,r14 */
L8c60d000: *stop=0x8c60d000u;r[3]=rd(m,0x8c60d11cu,4);goto L8c60d002; /* mov.l 0x8c60d11c,r3 */
L8c60d002: *stop=0x8c60d002u;r[14]+=20u;goto L8c60d004; /* add #0x14,r14 */
L8c60d004: *stop=0x8c60d004u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[4],4);goto L8c60d006; /* mov.l r4,@-r15 */
L8c60d006: *stop=0x8c60d006u;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);goto L8c60d008; /* mov.l @r3,r4 */
L8c60d008: *stop=0x8c60d008u;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c60d00a; /* mov.l @r14,r5 */
L8c60d00a: *stop=0x8c60d00au;if(!valid_ram(r[14]+8u,4u))return -3;r[7]=rd(m,r[14]+0x8u,4);goto L8c60d00c; /* mov.l @(0x8,r14),r7 */
L8c60d00c: *stop=0x8c60d00cu;if(!valid_ram(r[14]+4u,4u))return -3;r[6]=rd(m,r[14]+0x4u,4);goto L8c60d00e; /* mov.l @(0x4,r14),r6 */
L8c60d00e: *stop=0x8c60d00eu;r[15]+=4u;goto L8c60d010; /* add #0x4,r15 */
L8c60d010: *stop=0x8c60d010u;r[2]=rd(m,0x8c60d128u,4);goto L8c60d012; /* mov.l 0x8c60d128,r2 */
L8c60d012: target=r[2];*stop=0x8c60d014u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_motion_base_scale_8c60d016(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60d016;
L8c60d016: *stop=0x8c60d016u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60d018; /* mov.l r14,@-r15 */
L8c60d018: *stop=0x8c60d018u;r[14]=r[4];goto L8c60d01a; /* mov r4,r14 */
L8c60d01a: *stop=0x8c60d01au;r[0]=8u;goto L8c60d01c; /* mov #0x8,r0 */
L8c60d01c: *stop=0x8c60d01cu;r[3]=rd(m,0x8c60d11cu,4);goto L8c60d01e; /* mov.l 0x8c60d11c,r3 */
L8c60d01e: *stop=0x8c60d01eu;r[14]+=32u;goto L8c60d020; /* add #0x20,r14 */
L8c60d020: *stop=0x8c60d020u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[4],4);goto L8c60d022; /* mov.l r4,@-r15 */
L8c60d022: *stop=0x8c60d022u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[14]);goto L8c60d024; /* fmov.s @(r0,r14),fr6 */
L8c60d024: *stop=0x8c60d024u;r[0]=4u;goto L8c60d026; /* mov #0x4,r0 */
L8c60d026: *stop=0x8c60d026u;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);goto L8c60d028; /* mov.l @r3,r4 */
L8c60d028: *stop=0x8c60d028u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto L8c60d02a; /* fmov.s @(r0,r14),fr5 */
L8c60d02a: *stop=0x8c60d02au;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[14]);goto L8c60d02c; /* fmov.s @r14,fr4 */
L8c60d02c: *stop=0x8c60d02cu;r[15]+=4u;goto L8c60d02e; /* add #0x4,r15 */
L8c60d02e: *stop=0x8c60d02eu;r[2]=rd(m,0x8c60d12cu,4);goto L8c60d030; /* mov.l 0x8c60d12c,r2 */
L8c60d030: target=r[2];*stop=0x8c60d032u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_motion_format_zero_8c60d034(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60d034;
L8c60d034: *stop=0x8c60d034u;r[2]=rd(m,0x8c60d130u,4);goto L8c60d036; /* mov.l 0x8c60d130,r2 */
L8c60d036: *stop=0x8c60d036u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60d038; /* mov.l r14,@-r15 */
L8c60d038: *stop=0x8c60d038u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60d03a; /* sts.l PR,@-r15 */
L8c60d03a: *stop=0x8c60d03au;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d03c; /* mov.l @r2,r3 */
L8c60d03c: target=r[3];*pr=0x8c60d040u;*stop=0x8c60d03eu;r[14]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d040; /* jsr @r3 */
L8c60d040: *stop=0x8c60d040u;r[2]=rd(m,0x8c60d134u,4);goto L8c60d042; /* mov.l 0x8c60d134,r2 */
L8c60d042: *stop=0x8c60d042u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d044; /* mov.l @r2,r3 */
L8c60d044: target=r[3];*pr=0x8c60d048u;*stop=0x8c60d046u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60d048; /* jsr @r3 */
L8c60d048: *stop=0x8c60d048u;r[2]=rd(m,0x8c60d138u,4);goto L8c60d04a; /* mov.l 0x8c60d138,r2 */
L8c60d04a: *stop=0x8c60d04au;r[4]=r[14];goto L8c60d04c; /* mov r14,r4 */
L8c60d04c: *stop=0x8c60d04cu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c60d04e; /* lds.l @r15+,PR */
L8c60d04e: *stop=0x8c60d04eu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60d050; /* mov.l @r2,r3 */
L8c60d050: target=r[3];*stop=0x8c60d052u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}
