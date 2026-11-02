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
SA_STAGE_EXPORT int sa_camera_state2_8c01b554(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01b554;
L8c01b554: *stop=0x8c01b554u;r[5]=rd(m,0x8c01b5bcu,4);goto L8c01b556; /* mov.l 0x8c01b5bc,r5 */
L8c01b556: *stop=0x8c01b556u;r[0]=32u;goto L8c01b558; /* mov #0x20,r0 */
L8c01b558: *stop=0x8c01b558u;r[4]=rd(m,0x8c01b5c0u,4);goto L8c01b55a; /* mov.l 0x8c01b5c0,r4 */
L8c01b55a: *stop=0x8c01b55au;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c01b55c; /* mov.l @r5,r3 */
L8c01b55c: *stop=0x8c01b55cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto L8c01b55e; /* fmov.s @r4,fr3 */
L8c01b55e: *stop=0x8c01b55eu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[3]);goto L8c01b560; /* fmov.s @(r0,r3),fr2 */
L8c01b560: *stop=0x8c01b560u;r[0]=4u;goto L8c01b562; /* mov #0x4,r0 */
L8c01b562: *stop=0x8c01b562u;r[6]=rd(m,0x8c01b5b8u,4);goto L8c01b564; /* mov.l 0x8c01b5b8,r6 */
L8c01b564: *stop=0x8c01b564u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c01b566; /* fsub fr3,fr2 */
L8c01b566: *stop=0x8c01b566u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[6]);goto L8c01b568; /* fmov.s fr2,@r6 */
L8c01b568: *stop=0x8c01b568u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c01b56a; /* fmov.s @(r0,r4),fr3 */
L8c01b56a: *stop=0x8c01b56au;r[0]=36u;goto L8c01b56c; /* mov #0x24,r0 */
L8c01b56c: *stop=0x8c01b56cu;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c01b56e; /* mov.l @r5,r3 */
L8c01b56e: *stop=0x8c01b56eu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[3]);goto L8c01b570; /* fmov.s @(r0,r3),fr2 */
L8c01b570: *stop=0x8c01b570u;r[0]=4u;goto L8c01b572; /* mov #0x4,r0 */
L8c01b572: *stop=0x8c01b572u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c01b574; /* fsub fr3,fr2 */
L8c01b574: *stop=0x8c01b574u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[6]);goto L8c01b576; /* fmov fr2,@(r0,r6) */
L8c01b576: *stop=0x8c01b576u;r[0]=8u;goto L8c01b578; /* mov #0x8,r0 */
L8c01b578: *stop=0x8c01b578u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c01b57a; /* fmov.s @(r0,r4),fr3 */
L8c01b57a: *stop=0x8c01b57au;r[0]=40u;goto L8c01b57c; /* mov #0x28,r0 */
L8c01b57c: *stop=0x8c01b57cu;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c01b57e; /* mov.l @r5,r3 */
L8c01b57e: *stop=0x8c01b57eu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[3]);goto L8c01b580; /* fmov.s @(r0,r3),fr2 */
L8c01b580: *stop=0x8c01b580u;r[0]=8u;goto L8c01b582; /* mov #0x8,r0 */
L8c01b582: *stop=0x8c01b582u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c01b584; /* fsub fr3,fr2 */
L8c01b584: *stop=0x8c01b584u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[6]);goto L8c01b586; /* fmov fr2,@(r0,r6) */
L8c01b586: *stop=0x8c01b586u;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c01b588; /* mov.l @r5,r3 */
L8c01b588: *stop=0x8c01b588u;if(!valid_ram(r[3]+32u,4u))return -3;r[2]=rd(m,r[3]+0x20u,4);goto L8c01b58a; /* mov.l @(0x20,r3),r2 */
L8c01b58a: *stop=0x8c01b58au;if(!valid_ram(r[3]+36u,4u))return -3;r[1]=rd(m,r[3]+0x24u,4);goto L8c01b58c; /* mov.l @(0x24,r3),r1 */
L8c01b58c: *stop=0x8c01b58cu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[2],4);goto L8c01b58e; /* mov.l r2,@r4 */
L8c01b58e: *stop=0x8c01b58eu;if(!valid_ram(r[4]+4u,4u))return -3;wr(m,r[4]+0x4u,r[1],4);goto L8c01b590; /* mov.l r1,@(0x4,r4) */
L8c01b590: *stop=0x8c01b590u;if(!valid_ram(r[3]+40u,4u))return -3;r[2]=rd(m,r[3]+0x28u,4);goto L8c01b592; /* mov.l @(0x28,r3),r2 */
L8c01b592: *stop=0x8c01b592u;if(*pr!=entry_pr)return -3;*stop=0x8c01b594u;if(!valid_ram(r[4]+8u,4u))return -3;wr(m,r[4]+0x8u,r[2],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_state2_8c01995e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c01995e;
L8c01995e: *stop=0x8c01995eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c019960; /* mov.l r14,@-r15 */
L8c019960: *stop=0x8c019960u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c019962; /* mov.l r13,@-r15 */
L8c019962: *stop=0x8c019962u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c019964; /* mov.l r12,@-r15 */
L8c019964: *stop=0x8c019964u;r[1]=rd(m,0x8c019a68u,4);goto L8c019966; /* mov.l 0x8c019a68,r1 */
L8c019966: *stop=0x8c019966u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c019968; /* mov.l r11,@-r15 */
L8c019968: *stop=0x8c019968u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c01996a; /* mov.l r10,@-r15 */
L8c01996a: *stop=0x8c01996au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01996c; /* sts.l PR,@-r15 */
L8c01996c: *stop=0x8c01996cu;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c01996e; /* mov.l @r1,r3 */
L8c01996e: *stop=0x8c01996eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c019970; /* tst r3,r3 */
L8c019970: if(!(*sr&1u))goto L8c019976;goto L8c019972; /* bf 0x8c019976 */
L8c019972: *stop=0x8c019974u;goto L8c019ace; /* bra 0x8c019ace */
L8c019976: target=0x8c019adcu;*pr=0x8c01997au;*stop=0x8c019978u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01997a; /* bsr 0x8c019adc */
L8c01997a: *stop=0x8c01997au;r[12]=rd(m,0x8c019a44u,4);goto L8c01997c; /* mov.l 0x8c019a44,r12 */
L8c01997c: *stop=0x8c01997cu;if(!valid_ram(r[12],4u))return -3;r[0]=rd(m,r[12],4);goto L8c01997e; /* mov.l @r12,r0 */
L8c01997e: *stop=0x8c01997eu;if(!valid_ram(r[0]+6u,1u))return -3;r[0]=sx(m,r[0]+0x6u,1);goto L8c019980; /* mov.b @(0x6,r0),r0 */
L8c019980: *stop=0x8c019980u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c019982; /* cmp/eq #0x40,r0 */
L8c019982: if(!(*sr&1u))goto L8c019988;goto L8c019984; /* bf 0x8c019988 */
L8c019984: *stop=0x8c019986u;goto L8c019ace; /* bra 0x8c019ace */
L8c019988: *stop=0x8c019988u;r[2]=rd(m,0x8c019a6cu,4);goto L8c01998a; /* mov.l 0x8c019a6c,r2 */
L8c01998a: target=r[2];*pr=0x8c01998eu;*stop=0x8c01998cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01998e; /* jsr @r2 */
L8c01998e: *stop=0x8c01998eu;r[4]=r[0];goto L8c019990; /* mov r0,r4 */
L8c019990: *stop=0x8c019990u;r[5]=r[4]&65535u;goto L8c019992; /* extu.w r4,r5 */
L8c019992: *stop=0x8c019992u;r[3]=4294967288u;goto L8c019994; /* mov #-0x8,r3 */
L8c019994: *stop=0x8c019994u;r[0]=r[5];goto L8c019996; /* mov r5,r0 */
L8c019996: *stop=0x8c019996u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c019998; /* shad r3,r0 */
L8c019998: *stop=0x8c019998u;r[3]=rd(m,0x8c019a70u,4);goto L8c01999a; /* mov.l 0x8c019a70,r3 */
L8c01999a: *stop=0x8c01999au;r[0]<<=2;goto L8c01999c; /* shll2 r0 */
L8c01999c: *stop=0x8c01999cu;r[0]<<=2;goto L8c01999e; /* shll2 r0 */
L8c01999e: *stop=0x8c01999eu;r[2]=r[5]&255u;goto L8c0199a0; /* extu.b r5,r2 */
L8c0199a0: *stop=0x8c0199a0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0199a2; /* shll r0 */
L8c0199a2: *stop=0x8c0199a2u;r[3]+=r[0];goto L8c0199a4; /* add r0,r3 */
L8c0199a4: *stop=0x8c0199a4u;r[2]<<=2;goto L8c0199a6; /* shll2 r2 */
L8c0199a6: *stop=0x8c0199a6u;r[2]+=r[3];goto L8c0199a8; /* add r3,r2 */
L8c0199a8: *stop=0x8c0199a8u;if(!valid_ram(r[2],4u))return -3;r[1]=rd(m,r[2],4);goto L8c0199aa; /* mov.l @r2,r1 */
L8c0199aa: *stop=0x8c0199aau;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c0199ac; /* tst r1,r1 */
L8c0199ac: if(!(*sr&1u))goto L8c0199b2;goto L8c0199ae; /* bf 0x8c0199b2 */
L8c0199ae: *stop=0x8c0199b0u;goto L8c019ace; /* bra 0x8c019ace */
L8c0199b2: *stop=0x8c0199b2u;r[1]=rd(m,0x8c019a74u,4);goto L8c0199b4; /* mov.l 0x8c019a74,r1 */
L8c0199b4: *stop=0x8c0199b4u;r[2]=rd(m,0x8c019a78u,4);goto L8c0199b6; /* mov.l 0x8c019a78,r2 */
L8c0199b6: *stop=0x8c0199b6u;if(!valid_ram(r[1],4u))return -3;r[4]=rd(m,r[1],4);goto L8c0199b8; /* mov.l @r1,r4 */
L8c0199b8: *stop=0x8c0199b8u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c0199ba; /* mov.l @r2,r3 */
L8c0199ba: *stop=0x8c0199bau;if(!valid_ram(r[4],4u))return -3;r[4]=rd(m,r[4],4);goto L8c0199bc; /* mov.l @r4,r4 */
L8c0199bc: *stop=0x8c0199bcu;if(!valid_ram(r[12],4u))return -3;r[10]=rd(m,r[12],4);goto L8c0199be; /* mov.l @r12,r10 */
L8c0199be: *stop=0x8c0199beu;r[14]=r[4];goto L8c0199c0; /* mov r4,r14 */
L8c0199c0: *stop=0x8c0199c0u;r[14]+=4294967295u;goto L8c0199c2; /* add #-0x1,r14 */
L8c0199c2: *stop=0x8c0199c2u;r[14]<<=2;goto L8c0199c4; /* shll2 r14 */
L8c0199c4: *stop=0x8c0199c4u;r[13]=r[4];goto L8c0199c6; /* mov r4,r13 */
L8c0199c6: *stop=0x8c0199c6u;r[14]<<=2;goto L8c0199c8; /* shll2 r14 */
L8c0199c8: *stop=0x8c0199c8u;r[11]=rd(m,0x8c019a7cu,4);goto L8c0199ca; /* mov.l 0x8c019a7c,r11 */
L8c0199ca: *stop=0x8c0199cau;r[14]<<=2;goto L8c0199cc; /* shll2 r14 */
L8c0199cc: *stop=0x8c0199ccu;if(!valid_ram(r[10]+8u,1u))return -3;r[0]=sx(m,r[10]+0x8u,1);goto L8c0199ce; /* mov.b @(0x8,r10),r0 */
L8c0199ce: *stop=0x8c0199ceu;r[13]+=4294967295u;goto L8c0199d0; /* add #-0x1,r13 */
L8c0199d0: *stop=0x8c0199d0u;r[14]+=r[3];goto L8c0199d2; /* add r3,r14 */
L8c0199d2: *stop=0x8c0199d2u;r[3]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c0199d4; /* exts.w r13,r3 */
L8c0199d4: *stop=0x8c0199d4u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c0199d6; /* cmp/pl r3 */
L8c0199d6: take=(*sr&1u);*stop=0x8c0199d8u;r[10]=r[0];if(take)goto L8c0199de;goto L8c0199da; /* bt/s 0x8c0199de */
L8c0199da: *stop=0x8c0199dcu;goto L8c019ace; /* bra 0x8c019ace */
L8c0199de: *stop=0x8c0199deu;if(!valid_ram(r[14],1u))return -3;r[3]=sx(m,r[14],1);goto L8c0199e0; /* mov.b @r14,r3 */
L8c0199e0: *stop=0x8c0199e0u;if(!valid_ram(r[11],4u))return -3;r[2]=rd(m,r[11],4);goto L8c0199e2; /* mov.l @r11,r2 */
L8c0199e2: *stop=0x8c0199e2u;r[3]<<=2;goto L8c0199e4; /* shll2 r3 */
L8c0199e4: *stop=0x8c0199e4u;r[3]<<=2;goto L8c0199e6; /* shll2 r3 */
L8c0199e6: *stop=0x8c0199e6u;r[3]+=r[2];goto L8c0199e8; /* add r2,r3 */
L8c0199e8: *stop=0x8c0199e8u;if(!valid_ram(r[3]+4u,1u))return -3;r[0]=sx(m,r[3]+0x4u,1);goto L8c0199ea; /* mov.b @(0x4,r3),r0 */
L8c0199ea: *stop=0x8c0199eau;if(!valid_ram(r[12],4u))return -3;r[3]=rd(m,r[12],4);goto L8c0199ec; /* mov.l @r12,r3 */
L8c0199ec: *stop=0x8c0199ecu;r[2]=r[0];goto L8c0199ee; /* mov r0,r2 */
L8c0199ee: *stop=0x8c0199eeu;if(!valid_ram(r[3]+8u,1u))return -3;r[0]=sx(m,r[3]+0x8u,1);goto L8c0199f0; /* mov.b @(0x8,r3),r0 */
L8c0199f0: *stop=0x8c0199f0u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[0]);goto L8c0199f2; /* cmp/ge r0,r2 */
L8c0199f2: if((*sr&1u))goto L8c0199f8;goto L8c0199f4; /* bt 0x8c0199f8 */
L8c0199f4: *stop=0x8c0199f6u;goto L8c019ac4; /* bra 0x8c019ac4 */
L8c0199f8: *stop=0x8c0199f8u;if(!valid_ram(r[12],4u))return -3;r[0]=rd(m,r[12],4);goto L8c0199fa; /* mov.l @r12,r0 */
L8c0199fa: *stop=0x8c0199fau;if(!valid_ram(r[0]+8u,1u))return -3;r[0]=sx(m,r[0]+0x8u,1);goto L8c0199fc; /* mov.b @(0x8,r0),r0 */
L8c0199fc: *stop=0x8c0199fcu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c0199fe; /* cmp/eq #0x0,r0 */
L8c0199fe: take=(*sr&1u);*stop=0x8c019a00u;if(!valid_ram(r[11],4u))return -3;r[4]=rd(m,r[11],4);if(take)goto L8c019a80;goto L8c019a02; /* bt/s 0x8c019a80 */
L8c019a02: *stop=0x8c019a02u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c019a04; /* cmp/eq #0x1,r0 */
L8c019a04: if((*sr&1u))goto L8c019a98;goto L8c019a06; /* bt 0x8c019a98 */
L8c019a06: *stop=0x8c019a06u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c019a08; /* cmp/eq #0x3,r0 */
L8c019a08: if((*sr&1u))goto L8c019a98;goto L8c019a0a; /* bt 0x8c019a98 */
L8c019a0a: *stop=0x8c019a0au;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c019a0c; /* cmp/eq #0x2,r0 */
L8c019a0c: if((*sr&1u))goto L8c019ab0;goto L8c019a0e; /* bt 0x8c019ab0 */
L8c019a0e: *stop=0x8c019a10u;goto L8c019ab6; /* bra 0x8c019ab6 */
L8c019a80: *stop=0x8c019a80u;if(!valid_ram(r[14],1u))return -3;r[0]=sx(m,r[14],1);goto L8c019a82; /* mov.b @r14,r0 */
L8c019a82: *stop=0x8c019a82u;r[0]<<=2;goto L8c019a84; /* shll2 r0 */
L8c019a84: *stop=0x8c019a84u;r[0]<<=2;goto L8c019a86; /* shll2 r0 */
L8c019a86: *stop=0x8c019a86u;r[0]+=r[4];goto L8c019a88; /* add r4,r0 */
L8c019a88: *stop=0x8c019a88u;if(!valid_ram(r[0]+4u,1u))return -3;r[0]=sx(m,r[0]+0x4u,1);goto L8c019a8a; /* mov.b @(0x4,r0),r0 */
L8c019a8a: *stop=0x8c019a8au;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c019a8c; /* cmp/eq #0x4,r0 */
L8c019a8c: if((*sr&1u))goto L8c019ab6;goto L8c019a8e; /* bt 0x8c019ab6 */
L8c019a8e: *stop=0x8c019a8eu;r[5]=r[14];goto L8c019a90; /* mov r14,r5 */
L8c019a90: target=0x8c019be6u;*pr=0x8c019a94u;*stop=0x8c019a92u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019a94; /* bsr 0x8c019be6 */
L8c019a94: *stop=0x8c019a96u;goto L8c019ab6; /* bra 0x8c019ab6 */
L8c019a98: *stop=0x8c019a98u;if(!valid_ram(r[14],1u))return -3;r[0]=sx(m,r[14],1);goto L8c019a9a; /* mov.b @r14,r0 */
L8c019a9a: *stop=0x8c019a9au;r[0]<<=2;goto L8c019a9c; /* shll2 r0 */
L8c019a9c: *stop=0x8c019a9cu;r[0]<<=2;goto L8c019a9e; /* shll2 r0 */
L8c019a9e: *stop=0x8c019a9eu;r[0]+=r[4];goto L8c019aa0; /* add r4,r0 */
L8c019aa0: *stop=0x8c019aa0u;if(!valid_ram(r[0]+4u,1u))return -3;r[0]=sx(m,r[0]+0x4u,1);goto L8c019aa2; /* mov.b @(0x4,r0),r0 */
L8c019aa2: *stop=0x8c019aa2u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c019aa4; /* cmp/eq #0x4,r0 */
L8c019aa4: if((*sr&1u))goto L8c019ab6;goto L8c019aa6; /* bt 0x8c019ab6 */
L8c019aa6: *stop=0x8c019aa6u;r[5]=r[14];goto L8c019aa8; /* mov r14,r5 */
L8c019aa8: target=0x8c019c50u;*pr=0x8c019aacu;*stop=0x8c019aaau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019aac; /* bsr 0x8c019c50 */
L8c019aac: *stop=0x8c019aaeu;goto L8c019ab6; /* bra 0x8c019ab6 */
L8c019ab0: *stop=0x8c019ab0u;r[5]=r[14];goto L8c019ab2; /* mov r14,r5 */
L8c019ab2: target=0x8c019dc4u;*pr=0x8c019ab6u;*stop=0x8c019ab4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019ab6; /* bsr 0x8c019dc4 */
L8c019ab6: *stop=0x8c019ab6u;if(!valid_ram(r[12],4u))return -3;r[2]=rd(m,r[12],4);goto L8c019ab8; /* mov.l @r12,r2 */
L8c019ab8: *stop=0x8c019ab8u;r[3]=(uint32_t)(int32_t)(int8_t)r[10];goto L8c019aba; /* exts.b r10,r3 */
L8c019aba: *stop=0x8c019abau;if(!valid_ram(r[2]+8u,1u))return -3;r[0]=sx(m,r[2]+0x8u,1);goto L8c019abc; /* mov.b @(0x8,r2),r0 */
L8c019abc: *stop=0x8c019abcu;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]);goto L8c019abe; /* cmp/ge r3,r0 */
L8c019abe: if((*sr&1u))goto L8c019ac4;goto L8c019ac0; /* bt 0x8c019ac4 */
L8c019ac0: *stop=0x8c019ac2u;goto L8c0199b2; /* bra 0x8c0199b2 */
L8c019ac4: *stop=0x8c019ac4u;r[13]+=4294967295u;goto L8c019ac6; /* add #-0x1,r13 */
L8c019ac6: *stop=0x8c019ac6u;r[2]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c019ac8; /* exts.w r13,r2 */
L8c019ac8: *stop=0x8c019ac8u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c019aca; /* cmp/pl r2 */
L8c019aca: take=(*sr&1u);*stop=0x8c019accu;r[14]+=4294967232u;if(take)goto L8c0199de;goto L8c019ace; /* bt/s 0x8c0199de */
L8c019ace: *stop=0x8c019aceu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019ad0; /* lds.l @r15+,PR */
L8c019ad0: *stop=0x8c019ad0u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c019ad2; /* mov.l @r15+,r10 */
L8c019ad2: *stop=0x8c019ad2u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c019ad4; /* mov.l @r15+,r11 */
L8c019ad4: *stop=0x8c019ad4u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c019ad6; /* mov.l @r15+,r12 */
L8c019ad6: *stop=0x8c019ad6u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c019ad8; /* mov.l @r15+,r13 */
L8c019ad8: *stop=0x8c019ad8u;if(*pr!=entry_pr)return -3;*stop=0x8c019adau;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_state2_8c01b436(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01b436;
L8c01b436: *stop=0x8c01b436u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01b438; /* mov.l r14,@-r15 */
L8c01b438: *stop=0x8c01b438u;r[0]=32u;goto L8c01b43a; /* mov #0x20,r0 */
L8c01b43a: *stop=0x8c01b43au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c01b43c; /* mov.l r13,@-r15 */
L8c01b43c: *stop=0x8c01b43cu;r[13]=r[5];goto L8c01b43e; /* mov r5,r13 */
L8c01b43e: *stop=0x8c01b43eu;r[14]=r[4];goto L8c01b440; /* mov r4,r14 */
L8c01b440: *stop=0x8c01b440u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c01b442; /* fmov.s fr15,@-r15 */
L8c01b442: *stop=0x8c01b442u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01b444; /* sts.l PR,@-r15 */
L8c01b444: *stop=0x8c01b444u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c01b446; /* fmov.s @(r0,r13),fr3 */
L8c01b446: *stop=0x8c01b446u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c01b448; /* fmov.s @(r0,r14),fr4 */
L8c01b448: *stop=0x8c01b448u;r[0]=36u;goto L8c01b44a; /* mov #0x24,r0 */
L8c01b44a: *stop=0x8c01b44au;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto L8c01b44c; /* fmov.s @(r0,r14),fr5 */
L8c01b44c: *stop=0x8c01b44cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c01b44e; /* fsub fr3,fr4 */
L8c01b44e: *stop=0x8c01b44eu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c01b450; /* fmov.s @(r0,r13),fr3 */
L8c01b450: *stop=0x8c01b450u;r[0]=40u;goto L8c01b452; /* mov #0x28,r0 */
L8c01b452: *stop=0x8c01b452u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c01b454; /* fsub fr3,fr5 */
L8c01b454: *stop=0x8c01b454u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto L8c01b456; /* fmov.s @(r0,r13),fr3 */
L8c01b456: *stop=0x8c01b456u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[14]);goto L8c01b458; /* fmov.s @(r0,r14),fr6 */
L8c01b458: *stop=0x8c01b458u;r[0]=0x8c01b4f0u;goto L8c01b45a; /* mova 0x8c01b4f0,r0 */
L8c01b45a: *stop=0x8c01b45au;fcopy(f,0u,4u);goto L8c01b45c; /* fmov fr4,fr0 */
L8c01b45c: *stop=0x8c01b45cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c01b45e; /* fsub fr3,fr6 */
L8c01b45e: *stop=0x8c01b45eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c01b460; /* fmov.s @r0,fr2 */
L8c01b460: *stop=0x8c01b460u;fcopy(f,3u,5u);goto L8c01b462; /* fmov fr5,fr3 */
L8c01b462: *stop=0x8c01b462u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c01b464; /* fmul fr5,fr3 */
L8c01b464: *stop=0x8c01b464u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c01b466; /* fmac fr0,fr4,fr3 */
L8c01b466: *stop=0x8c01b466u;fcopy(f,0u,6u);goto L8c01b468; /* fmov fr6,fr0 */
L8c01b468: *stop=0x8c01b468u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c01b46a; /* fmac fr0,fr6,fr3 */
L8c01b46a: *stop=0x8c01b46au;fcopy(f,15u,3u);goto L8c01b46c; /* fmov fr3,fr15 */
L8c01b46c: *stop=0x8c01b46cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][15],&f->fpscr,sr,1);if(status)return status;goto L8c01b46e; /* fcmp/gt fr15,fr2 */
L8c01b46e: if(!(*sr&1u))goto L8c01b4c8;goto L8c01b470; /* bf 0x8c01b4c8 */
L8c01b470: *stop=0x8c01b470u;r[2]=rd(m,0x8c01b4f4u,4);goto L8c01b472; /* mov.l 0x8c01b4f4,r2 */
L8c01b472: *stop=0x8c01b472u;if(!valid_ram(r[13]+20u,4u))return -3;r[5]=rd(m,r[13]+0x14u,4);goto L8c01b474; /* mov.l @(0x14,r13),r5 */
L8c01b474: target=r[2];*pr=0x8c01b478u;*stop=0x8c01b476u;if(!valid_ram(r[14]+20u,4u))return -3;r[4]=rd(m,r[14]+0x14u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01b478; /* jsr @r2 */
L8c01b478: *stop=0x8c01b478u;r[3]=91u;goto L8c01b47a; /* mov #0x5b,r3 */
L8c01b47a: *stop=0x8c01b47au;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]);goto L8c01b47c; /* cmp/ge r3,r0 */
L8c01b47c: if((*sr&1u))goto L8c01b4c8;goto L8c01b47e; /* bt 0x8c01b4c8 */
L8c01b47e: *stop=0x8c01b47eu;r[1]=rd(m,0x8c01b4f4u,4);goto L8c01b480; /* mov.l 0x8c01b4f4,r1 */
L8c01b480: *stop=0x8c01b480u;if(!valid_ram(r[13]+24u,4u))return -3;r[5]=rd(m,r[13]+0x18u,4);goto L8c01b482; /* mov.l @(0x18,r13),r5 */
L8c01b482: target=r[1];*pr=0x8c01b486u;*stop=0x8c01b484u;if(!valid_ram(r[14]+24u,4u))return -3;r[4]=rd(m,r[14]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01b486; /* jsr @r1 */
L8c01b486: *stop=0x8c01b486u;r[3]=91u;goto L8c01b488; /* mov #0x5b,r3 */
L8c01b488: *stop=0x8c01b488u;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]);goto L8c01b48a; /* cmp/ge r3,r0 */
L8c01b48a: if((*sr&1u))goto L8c01b4c8;goto L8c01b48c; /* bt 0x8c01b4c8 */
L8c01b48c: *stop=0x8c01b48cu;r[1]=rd(m,0x8c01b4f4u,4);goto L8c01b48e; /* mov.l 0x8c01b4f4,r1 */
L8c01b48e: *stop=0x8c01b48eu;if(!valid_ram(r[13]+28u,4u))return -3;r[5]=rd(m,r[13]+0x1cu,4);goto L8c01b490; /* mov.l @(0x1c,r13),r5 */
L8c01b490: target=r[1];*pr=0x8c01b494u;*stop=0x8c01b492u;if(!valid_ram(r[14]+28u,4u))return -3;r[4]=rd(m,r[14]+0x1cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01b494; /* jsr @r1 */
L8c01b494: *stop=0x8c01b494u;r[3]=91u;goto L8c01b496; /* mov #0x5b,r3 */
L8c01b496: *stop=0x8c01b496u;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]);goto L8c01b498; /* cmp/ge r3,r0 */
L8c01b498: if((*sr&1u))goto L8c01b4c8;goto L8c01b49a; /* bt 0x8c01b4c8 */
L8c01b49a: *stop=0x8c01b49au;r[2]=rd(m,0x8c01b4e0u,4);goto L8c01b49c; /* mov.l 0x8c01b4e0,r2 */
L8c01b49c: *stop=0x8c01b49cu;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto L8c01b49e; /* mov.l @r2,r0 */
L8c01b49e: *stop=0x8c01b49eu;if(!valid_ram(r[0]+9u,1u))return -3;r[0]=sx(m,r[0]+0x9u,1);goto L8c01b4a0; /* mov.b @(0x9,r0),r0 */
L8c01b4a0: *stop=0x8c01b4a0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c01b4a2; /* cmp/eq #0x1,r0 */
L8c01b4a2: if((*sr&1u))goto L8c01b4b8;goto L8c01b4a4; /* bt 0x8c01b4b8 */
L8c01b4a4: *stop=0x8c01b4a4u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c01b4a6; /* cmp/eq #0x0,r0 */
L8c01b4a6: if((*sr&1u))goto L8c01b4bc;goto L8c01b4a8; /* bt 0x8c01b4bc */
L8c01b4a8: *stop=0x8c01b4a8u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c01b4aa; /* cmp/eq #0x2,r0 */
L8c01b4aa: if((*sr&1u))goto L8c01b4bc;goto L8c01b4ac; /* bt 0x8c01b4bc */
L8c01b4ac: *stop=0x8c01b4acu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c01b4ae; /* cmp/eq #0x3,r0 */
L8c01b4ae: if((*sr&1u))goto L8c01b4bc;goto L8c01b4b0; /* bt 0x8c01b4bc */
L8c01b4b0: *stop=0x8c01b4b0u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c01b4b2; /* cmp/eq #0x4,r0 */
L8c01b4b2: if((*sr&1u))goto L8c01b4bc;goto L8c01b4b4; /* bt 0x8c01b4bc */
L8c01b4b4: *stop=0x8c01b4b6u;goto L8c01b4c8; /* bra 0x8c01b4c8 */
L8c01b4b8: *stop=0x8c01b4bau;r[4]=3u;goto L8c01b4be; /* bra 0x8c01b4be */
L8c01b4bc: *stop=0x8c01b4bcu;r[4]=0u;goto L8c01b4be; /* mov #0x0,r4 */
L8c01b4be: *stop=0x8c01b4beu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01b4c0; /* lds.l @r15+,PR */
L8c01b4c0: *stop=0x8c01b4c0u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01b4c2; /* fmov.s @r15+,fr15 */
L8c01b4c2: *stop=0x8c01b4c2u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01b4c4; /* mov.l @r15+,r13 */
L8c01b4c4: *stop=0x8c01b4c6u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0x8c01b3e0u,m,f,sr,r,pr); /* bra 0x8c01b3e0 */
L8c01b4c8: *stop=0x8c01b4c8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01b4ca; /* lds.l @r15+,PR */
L8c01b4ca: *stop=0x8c01b4cau;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01b4cc; /* fmov.s @r15+,fr15 */
L8c01b4cc: *stop=0x8c01b4ccu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01b4ce; /* mov.l @r15+,r13 */
L8c01b4ce: *stop=0x8c01b4ceu;if(*pr!=entry_pr)return -3;*stop=0x8c01b4d0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_state2_8c01c2d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01c2d8;
L8c01c2d8: *stop=0x8c01c2d8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01c2da; /* mov.l r14,@-r15 */
L8c01c2da: *stop=0x8c01c2dau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c01c2dc; /* mov.l r13,@-r15 */
L8c01c2dc: *stop=0x8c01c2dcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c01c2de; /* mov.l r12,@-r15 */
L8c01c2de: *stop=0x8c01c2deu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c01c2e0; /* mov.l r11,@-r15 */
L8c01c2e0: *stop=0x8c01c2e0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c01c2e2; /* mov.l r8,@-r15 */
L8c01c2e2: *stop=0x8c01c2e2u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[15]);goto L8c01c2e4; /* fmov.s fr12,@-r15 */
L8c01c2e4: *stop=0x8c01c2e4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01c2e6; /* sts.l PR,@-r15 */
L8c01c2e6: *stop=0x8c01c2e6u;r[15]+=4294967284u;goto L8c01c2e8; /* add #-0xc,r15 */
L8c01c2e8: *stop=0x8c01c2e8u;r[14]=rd(m,0x8c01c348u,4);goto L8c01c2ea; /* mov.l 0x8c01c348,r14 */
L8c01c2ea: *stop=0x8c01c2eau;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);goto L8c01c2ec; /* mov.l @r14,r5 */
L8c01c2ec: *stop=0x8c01c2ecu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c01c2ee; /* tst r5,r5 */
L8c01c2ee: if(!(*sr&1u))goto L8c01c360;goto L8c01c2f0; /* bf 0x8c01c360 */
L8c01c2f0: *stop=0x8c01c2f2u;r[4]=49u;goto L8c01c364; /* bra 0x8c01c364 */
L8c01c360: *stop=0x8c01c360u;r[4]=r[5];goto L8c01c362; /* mov r5,r4 */
L8c01c362: *stop=0x8c01c362u;r[4]+=4294967295u;goto L8c01c364; /* add #-0x1,r4 */
L8c01c364: *stop=0x8c01c364u;r[12]=r[4];goto L8c01c366; /* mov r4,r12 */
L8c01c366: *stop=0x8c01c366u;r[2]=r[4];goto L8c01c368; /* mov r4,r2 */
L8c01c368: *stop=0x8c01c368u;*sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c01c36a; /* shll r12 */
L8c01c36a: *stop=0x8c01c36au;r[11]=rd(m,0x8c01c464u,4);goto L8c01c36c; /* mov.l 0x8c01c464,r11 */
L8c01c36c: *stop=0x8c01c36cu;r[12]+=r[2];goto L8c01c36e; /* add r2,r12 */
L8c01c36e: *stop=0x8c01c36eu;r[13]=rd(m,0x8c01c460u,4);goto L8c01c370; /* mov.l 0x8c01c460,r13 */
L8c01c370: *stop=0x8c01c370u;r[12]<<=2;goto L8c01c372; /* shll2 r12 */
L8c01c372: *stop=0x8c01c372u;if(!valid_ram(r[11],4u))return -3;r[3]=rd(m,r[11],4);goto L8c01c374; /* mov.l @r11,r3 */
L8c01c374: *stop=0x8c01c374u;r[0]=32u;goto L8c01c376; /* mov #0x20,r0 */
L8c01c376: *stop=0x8c01c376u;r[12]+=r[13];goto L8c01c378; /* add r13,r12 */
L8c01c378: *stop=0x8c01c378u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[3]);goto L8c01c37a; /* fmov.s @(r0,r3),fr2 */
L8c01c37a: *stop=0x8c01c37au;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[12]);goto L8c01c37c; /* fmov.s @r12,fr3 */
L8c01c37c: *stop=0x8c01c37cu;r[0]=4u;goto L8c01c37e; /* mov #0x4,r0 */
L8c01c37e: *stop=0x8c01c37eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c01c380; /* fsub fr3,fr2 */
L8c01c380: *stop=0x8c01c380u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[15]);goto L8c01c382; /* fmov.s fr2,@r15 */
L8c01c382: *stop=0x8c01c382u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[12]);goto L8c01c384; /* fmov.s @(r0,r12),fr3 */
L8c01c384: *stop=0x8c01c384u;r[0]=36u;goto L8c01c386; /* mov #0x24,r0 */
L8c01c386: *stop=0x8c01c386u;if(!valid_ram(r[11],4u))return -3;r[3]=rd(m,r[11],4);goto L8c01c388; /* mov.l @r11,r3 */
L8c01c388: *stop=0x8c01c388u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[3]);goto L8c01c38a; /* fmov.s @(r0,r3),fr2 */
L8c01c38a: *stop=0x8c01c38au;r[3]=rd(m,0x8c01c468u,4);goto L8c01c38c; /* mov.l 0x8c01c468,r3 */
L8c01c38c: *stop=0x8c01c38cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c01c38e; /* fsub fr3,fr2 */
L8c01c38e: *stop=0x8c01c38eu;fcopy(f,12u,2u);goto L8c01c390; /* fmov fr2,fr12 */
L8c01c390: target=r[3];*pr=0x8c01c394u;*stop=0x8c01c392u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c394; /* jsr @r3 */
L8c01c394: *stop=0x8c01c394u;r[1]=70u;goto L8c01c396; /* mov #0x46,r1 */
L8c01c396: *stop=0x8c01c396u;r[1]<<=2;goto L8c01c398; /* shll2 r1 */
L8c01c398: *stop=0x8c01c398u;if(!valid_ram(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[1]);goto L8c01c39a; /* fmov.s @(r0,r1),fr3 */
L8c01c39a: *stop=0x8c01c39au;r[0]=4u;goto L8c01c39c; /* mov #0x4,r0 */
L8c01c39c: *stop=0x8c01c39cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[3],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c01c39e; /* fadd fr3,fr12 */
L8c01c39e: *stop=0x8c01c39eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,12u,r[0]+r[15]);goto L8c01c3a0; /* fmov fr12,@(r0,r15) */
L8c01c3a0: *stop=0x8c01c3a0u;r[0]=8u;goto L8c01c3a2; /* mov #0x8,r0 */
L8c01c3a2: *stop=0x8c01c3a2u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[12]);goto L8c01c3a4; /* fmov.s @(r0,r12),fr3 */
L8c01c3a4: *stop=0x8c01c3a4u;r[0]=40u;goto L8c01c3a6; /* mov #0x28,r0 */
L8c01c3a6: *stop=0x8c01c3a6u;if(!valid_ram(r[11],4u))return -3;r[3]=rd(m,r[11],4);goto L8c01c3a8; /* mov.l @r11,r3 */
L8c01c3a8: *stop=0x8c01c3a8u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[3]);goto L8c01c3aa; /* fmov.s @(r0,r3),fr2 */
L8c01c3aa: *stop=0x8c01c3aau;r[0]=8u;goto L8c01c3ac; /* mov #0x8,r0 */
L8c01c3ac: *stop=0x8c01c3acu;r[3]=rd(m,0x8c01c46cu,4);goto L8c01c3ae; /* mov.l 0x8c01c46c,r3 */
L8c01c3ae: *stop=0x8c01c3aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c01c3b0; /* fsub fr3,fr2 */
L8c01c3b0: *stop=0x8c01c3b0u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto L8c01c3b2; /* fmov fr2,@(r0,r15) */
L8c01c3b2: target=r[3];*pr=0x8c01c3b6u;*stop=0x8c01c3b4u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c3b6; /* jsr @r3 */
L8c01c3b6: *stop=0x8c01c3b6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c01c3b8; /* fldi1 fr3 */
L8c01c3b8: *stop=0x8c01c3b8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c01c3ba; /* fcmp/gt fr3,fr0 */
L8c01c3ba: if(!(*sr&1u))goto L8c01c420;goto L8c01c3bc; /* bf 0x8c01c420 */
L8c01c3bc: *stop=0x8c01c3bcu;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c01c3be; /* mov.l @r14,r2 */
L8c01c3be: *stop=0x8c01c3beu;r[0]=32u;goto L8c01c3c0; /* mov #0x20,r0 */
L8c01c3c0: *stop=0x8c01c3c0u;r[12]=rd(m,0x8c01c464u,4);goto L8c01c3c2; /* mov.l 0x8c01c464,r12 */
L8c01c3c2: *stop=0x8c01c3c2u;r[3]=r[2];goto L8c01c3c4; /* mov r2,r3 */
L8c01c3c4: *stop=0x8c01c3c4u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c01c3c6; /* shll r2 */
L8c01c3c6: *stop=0x8c01c3c6u;if(!valid_ram(r[12],4u))return -3;r[1]=rd(m,r[12],4);goto L8c01c3c8; /* mov.l @r12,r1 */
L8c01c3c8: *stop=0x8c01c3c8u;r[2]+=r[3];goto L8c01c3ca; /* add r3,r2 */
L8c01c3ca: *stop=0x8c01c3cau;r[2]<<=2;goto L8c01c3cc; /* shll2 r2 */
L8c01c3cc: *stop=0x8c01c3ccu;if(!valid_ram(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[1]);goto L8c01c3ce; /* fmov.s @(r0,r1),fr2 */
L8c01c3ce: *stop=0x8c01c3ceu;r[2]+=r[13];goto L8c01c3d0; /* add r13,r2 */
L8c01c3d0: *stop=0x8c01c3d0u;r[0]=36u;goto L8c01c3d2; /* mov #0x24,r0 */
L8c01c3d2: *stop=0x8c01c3d2u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[2]);goto L8c01c3d4; /* fmov.s fr2,@r2 */
L8c01c3d4: *stop=0x8c01c3d4u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01c3d6; /* mov.l @r14,r3 */
L8c01c3d6: *stop=0x8c01c3d6u;if(!valid_ram(r[12],4u))return -3;r[1]=rd(m,r[12],4);goto L8c01c3d8; /* mov.l @r12,r1 */
L8c01c3d8: *stop=0x8c01c3d8u;r[2]=r[3];goto L8c01c3da; /* mov r3,r2 */
L8c01c3da: *stop=0x8c01c3dau;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c01c3dc; /* shll r3 */
L8c01c3dc: *stop=0x8c01c3dcu;r[3]+=r[2];goto L8c01c3de; /* add r2,r3 */
L8c01c3de: *stop=0x8c01c3deu;if(!valid_ram(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]+r[1]);goto L8c01c3e0; /* fmov.s @(r0,r1),fr12 */
L8c01c3e0: *stop=0x8c01c3e0u;r[3]<<=2;goto L8c01c3e2; /* shll2 r3 */
L8c01c3e2: *stop=0x8c01c3e2u;r[3]+=r[13];goto L8c01c3e4; /* add r13,r3 */
L8c01c3e4: *stop=0x8c01c3e4u;r[8]=r[3];goto L8c01c3e6; /* mov r3,r8 */
L8c01c3e6: *stop=0x8c01c3e6u;r[3]=rd(m,0x8c01c468u,4);goto L8c01c3e8; /* mov.l 0x8c01c468,r3 */
L8c01c3e8: *stop=0x8c01c3e8u;r[8]+=4u;goto L8c01c3ea; /* add #0x4,r8 */
L8c01c3ea: target=r[3];*pr=0x8c01c3eeu;*stop=0x8c01c3ecu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01c3ee; /* jsr @r3 */
L8c01c3ee: *stop=0x8c01c3eeu;r[2]=70u;goto L8c01c3f0; /* mov #0x46,r2 */
L8c01c3f0: *stop=0x8c01c3f0u;r[2]<<=2;goto L8c01c3f2; /* shll2 r2 */
L8c01c3f2: *stop=0x8c01c3f2u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[2]);goto L8c01c3f4; /* fmov.s @(r0,r2),fr3 */
L8c01c3f4: *stop=0x8c01c3f4u;r[0]=40u;goto L8c01c3f6; /* mov #0x28,r0 */
L8c01c3f6: *stop=0x8c01c3f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[3],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c01c3f8; /* fadd fr3,fr12 */
L8c01c3f8: *stop=0x8c01c3f8u;if(!valid_ram(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,12u,r[8]);goto L8c01c3fa; /* fmov.s fr12,@r8 */
L8c01c3fa: *stop=0x8c01c3fau;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01c3fc; /* mov.l @r14,r3 */
L8c01c3fc: *stop=0x8c01c3fcu;if(!valid_ram(r[12],4u))return -3;r[1]=rd(m,r[12],4);goto L8c01c3fe; /* mov.l @r12,r1 */
L8c01c3fe: *stop=0x8c01c3feu;r[2]=r[3];goto L8c01c400; /* mov r3,r2 */
L8c01c400: *stop=0x8c01c400u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c01c402; /* shll r3 */
L8c01c402: *stop=0x8c01c402u;r[3]+=r[2];goto L8c01c404; /* add r2,r3 */
L8c01c404: *stop=0x8c01c404u;if(!valid_ram(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[1]);goto L8c01c406; /* fmov.s @(r0,r1),fr3 */
L8c01c406: *stop=0x8c01c406u;r[3]<<=2;goto L8c01c408; /* shll2 r3 */
L8c01c408: *stop=0x8c01c408u;r[0]=8u;goto L8c01c40a; /* mov #0x8,r0 */
L8c01c40a: *stop=0x8c01c40au;r[3]+=r[13];goto L8c01c40c; /* add r13,r3 */
L8c01c40c: *stop=0x8c01c40cu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[3]);goto L8c01c40e; /* fmov fr3,@(r0,r3) */
L8c01c40e: *stop=0x8c01c40eu;r[2]=50u;goto L8c01c410; /* mov #0x32,r2 */
L8c01c410: *stop=0x8c01c410u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01c412; /* mov.l @r14,r3 */
L8c01c412: *stop=0x8c01c412u;r[3]+=1u;goto L8c01c414; /* add #0x1,r3 */
L8c01c414: *stop=0x8c01c414u;r[1]=r[3];goto L8c01c416; /* mov r3,r1 */
L8c01c416: *stop=0x8c01c416u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[2]);goto L8c01c418; /* cmp/ge r2,r1 */
L8c01c418: *stop=0x8c01c418u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[3],4);goto L8c01c41a; /* mov.l r3,@r14 */
L8c01c41a: if(!(*sr&1u))goto L8c01c420;goto L8c01c41c; /* bf 0x8c01c420 */
L8c01c41c: *stop=0x8c01c41cu;r[0]=0u;goto L8c01c41e; /* mov #0x0,r0 */
L8c01c41e: *stop=0x8c01c41eu;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[0],4);goto L8c01c420; /* mov.l r0,@r14 */
L8c01c420: *stop=0x8c01c420u;r[15]+=12u;goto L8c01c422; /* add #0xc,r15 */
L8c01c422: *stop=0x8c01c422u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01c424; /* lds.l @r15+,PR */
L8c01c424: *stop=0x8c01c424u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01c426; /* fmov.s @r15+,fr12 */
L8c01c426: *stop=0x8c01c426u;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c01c428; /* mov.l @r15+,r8 */
L8c01c428: *stop=0x8c01c428u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c01c42a; /* mov.l @r15+,r11 */
L8c01c42a: *stop=0x8c01c42au;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c01c42c; /* mov.l @r15+,r12 */
L8c01c42c: *stop=0x8c01c42cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01c42e; /* mov.l @r15+,r13 */
L8c01c42e: *stop=0x8c01c42eu;if(*pr!=entry_pr)return -3;*stop=0x8c01c430u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_state2_8c01a5ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01a5ae;
L8c01a5ae: *stop=0x8c01a5aeu;r[4]=rd(m,0x8c01a664u,4);goto L8c01a5b0; /* mov.l 0x8c01a664,r4 */
L8c01a5b0: *stop=0x8c01a5b0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01a5b2; /* mov.l r14,@-r15 */
L8c01a5b2: *stop=0x8c01a5b2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01a5b4; /* sts.l PR,@-r15 */
L8c01a5b4: *stop=0x8c01a5b4u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c01a5b6; /* mov.l @r4,r3 */
L8c01a5b6: *stop=0x8c01a5b6u;if(!valid_ram(r[3]+8u,1u))return -3;r[0]=sx(m,r[3]+0x8u,1);goto L8c01a5b8; /* mov.b @(0x8,r3),r0 */
L8c01a5b8: *stop=0x8c01a5b8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c01a5ba; /* tst r0,r0 */
L8c01a5ba: if(!(*sr&1u))goto L8c01a606;goto L8c01a5bc; /* bf 0x8c01a606 */
L8c01a5bc: *stop=0x8c01a5bcu;r[0]=rd(m,0x8c01a66cu,4);goto L8c01a5be; /* mov.l 0x8c01a66c,r0 */
L8c01a5be: *stop=0x8c01a5beu;r[2]=rd(m,0x8c01a668u,4);goto L8c01a5c0; /* mov.l 0x8c01a668,r2 */
L8c01a5c0: *stop=0x8c01a5c0u;if(!valid_ram(r[0],4u))return -3;r[3]=rd(m,r[0],4);goto L8c01a5c2; /* mov.l @r0,r3 */
L8c01a5c2: *stop=0x8c01a5c2u;if(!valid_ram(r[2]+16u,4u))return -3;r[1]=rd(m,r[2]+0x10u,4);goto L8c01a5c4; /* mov.l @(0x10,r2),r1 */
L8c01a5c4: *stop=0x8c01a5c4u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c01a5c6; /* tst r3,r1 */
L8c01a5c6: if((*sr&1u))goto L8c01a606;goto L8c01a5c8; /* bt 0x8c01a606 */
L8c01a5c8: *stop=0x8c01a5c8u;r[6]=rd(m,0x8c01a674u,4);goto L8c01a5ca; /* mov.l 0x8c01a674,r6 */
L8c01a5ca: *stop=0x8c01a5cau;r[5]=0u;goto L8c01a5cc; /* mov #0x0,r5 */
L8c01a5cc: *stop=0x8c01a5ccu;r[7]=rd(m,0x8c01a670u,4);goto L8c01a5ce; /* mov.l 0x8c01a670,r7 */
L8c01a5ce: *stop=0x8c01a5ceu;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c01a5d0; /* mov.l @r4,r2 */
L8c01a5d0: *stop=0x8c01a5d0u;if(!valid_ram(r[2]+6u,1u))return -3;r[0]=sx(m,r[2]+0x6u,1);goto L8c01a5d2; /* mov.b @(0x6,r2),r0 */
L8c01a5d2: *stop=0x8c01a5d2u;r[0]+=1u;goto L8c01a5d4; /* add #0x1,r0 */
L8c01a5d4: *stop=0x8c01a5d4u;if(!valid_ram(r[2]+6u,1u))return -3;wr(m,r[2]+0x6u,r[0],1);goto L8c01a5d6; /* mov.b r0,@(0x6,r2) */
L8c01a5d6: *stop=0x8c01a5d6u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c01a5d8; /* mov.l @r4,r3 */
L8c01a5d8: *stop=0x8c01a5d8u;if(!valid_ram(r[3]+6u,1u))return -3;r[0]=sx(m,r[3]+0x6u,1);goto L8c01a5da; /* mov.b @(0x6,r3),r0 */
L8c01a5da: *stop=0x8c01a5dau;if(!valid_ram(r[6],2u))return -3;r[3]=sx(m,r[6],2);goto L8c01a5dc; /* mov.w @r6,r3 */
L8c01a5dc: *stop=0x8c01a5dcu;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]);goto L8c01a5de; /* cmp/ge r3,r0 */
L8c01a5de: if(!(*sr&1u))goto L8c01a5e6;goto L8c01a5e0; /* bf 0x8c01a5e6 */
L8c01a5e0: *stop=0x8c01a5e0u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c01a5e2; /* mov.l @r4,r3 */
L8c01a5e2: *stop=0x8c01a5e2u;r[0]=r[5];goto L8c01a5e4; /* mov r5,r0 */
L8c01a5e4: *stop=0x8c01a5e4u;if(!valid_ram(r[3]+6u,1u))return -3;wr(m,r[3]+0x6u,r[0],1);goto L8c01a5e6; /* mov.b r0,@(0x6,r3) */
L8c01a5e6: *stop=0x8c01a5e6u;if(!valid_ram(r[4],4u))return -3;r[14]=rd(m,r[4],4);goto L8c01a5e8; /* mov.l @r4,r14 */
L8c01a5e8: *stop=0x8c01a5e8u;if(!valid_ram(r[7],4u))return -3;r[3]=rd(m,r[7],4);goto L8c01a5ea; /* mov.l @r7,r3 */
L8c01a5ea: *stop=0x8c01a5eau;if(!valid_ram(r[14]+6u,1u))return -3;r[0]=sx(m,r[14]+0x6u,1);goto L8c01a5ec; /* mov.b @(0x6,r14),r0 */
L8c01a5ec: *stop=0x8c01a5ecu;r[0]<<=2;goto L8c01a5ee; /* shll2 r0 */
L8c01a5ee: *stop=0x8c01a5eeu;r[0]<<=2;goto L8c01a5f0; /* shll2 r0 */
L8c01a5f0: *stop=0x8c01a5f0u;r[0]+=r[3];goto L8c01a5f2; /* add r3,r0 */
L8c01a5f2: *stop=0x8c01a5f2u;if(!valid_ram(r[0]+8u,4u))return -3;r[0]=rd(m,r[0]+0x8u,4);goto L8c01a5f4; /* mov.l @(0x8,r0),r0 */
L8c01a5f4: *stop=0x8c01a5f4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c01a5f6; /* cmp/eq #0x1,r0 */
L8c01a5f6: if(!(*sr&1u))goto L8c01a5ce;goto L8c01a5f8; /* bf 0x8c01a5ce */
L8c01a5f8: *stop=0x8c01a5f8u;if(!valid_ram(r[14]+8u,1u))return -3;r[0]=sx(m,r[14]+0x8u,1);goto L8c01a5fa; /* mov.b @(0x8,r14),r0 */
L8c01a5fa: *stop=0x8c01a5fau;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c01a5fc; /* tst r0,r0 */
L8c01a5fc: if(!(*sr&1u))goto L8c01a5ce;goto L8c01a5fe; /* bf 0x8c01a5ce */
L8c01a5fe: *stop=0x8c01a5feu;if(!valid_ram(r[14]+6u,1u))return -3;r[0]=sx(m,r[14]+0x6u,1);goto L8c01a600; /* mov.b @(0x6,r14),r0 */
L8c01a600: *stop=0x8c01a600u;r[5]=0u;goto L8c01a602; /* mov #0x0,r5 */
L8c01a602: target=0x8c01aae0u;*pr=0x8c01a606u;*stop=0x8c01a604u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a606; /* bsr 0x8c01aae0 */
L8c01a606: *stop=0x8c01a606u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01a608; /* lds.l @r15+,PR */
L8c01a608: *stop=0x8c01a608u;if(*pr!=entry_pr)return -3;*stop=0x8c01a60au;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_state2_8c01ae60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c01ae60;
L8c01ae60: *stop=0x8c01ae60u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01ae62; /* mov.l r14,@-r15 */
L8c01ae62: *stop=0x8c01ae62u;r[14]=rd(m,0x8c01b000u,4);goto L8c01ae64; /* mov.l 0x8c01b000,r14 */
L8c01ae64: *stop=0x8c01ae64u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c01ae66; /* mov.l r13,@-r15 */
L8c01ae66: *stop=0x8c01ae66u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01ae68; /* sts.l PR,@-r15 */
L8c01ae68: *stop=0x8c01ae68u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);goto L8c01ae6a; /* mov.l @r14,r4 */
L8c01ae6a: *stop=0x8c01ae6au;if(!valid_ram(r[4]+8u,1u))return -3;r[0]=sx(m,r[4]+0x8u,1);goto L8c01ae6c; /* mov.b @(0x8,r4),r0 */
L8c01ae6c: *stop=0x8c01ae6cu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c01ae6e; /* cmp/eq #0x2,r0 */
L8c01ae6e: take=!(*sr&1u);*stop=0x8c01ae70u;r[13]=0u;if(take)goto L8c01aefa;goto L8c01ae72; /* bf/s 0x8c01aefa */
L8c01ae72: *stop=0x8c01ae72u;r[0]=52u;goto L8c01ae74; /* mov #0x34,r0 */
L8c01ae74: *stop=0x8c01ae74u;if(!valid_ram(r[0]+r[4],2u))return -3;r[0]=sx(m,r[0]+r[4],2);goto L8c01ae76; /* mov.w @(r0,r4),r0 */
L8c01ae76: *stop=0x8c01ae76u;r[1]=62u;goto L8c01ae78; /* mov #0x3e,r1 */
L8c01ae78: *stop=0x8c01ae78u;r[2]=118u;goto L8c01ae7a; /* mov #0x76,r2 */
L8c01ae7a: *stop=0x8c01ae7au;if(!valid_ram(r[4]+4u,2u))return -3;wr(m,r[4]+0x4u,r[0],2);goto L8c01ae7c; /* mov.w r0,@(0x4,r4) */
L8c01ae7c: *stop=0x8c01ae7cu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01ae7e; /* mov.l @r14,r3 */
L8c01ae7e: *stop=0x8c01ae7eu;r[0]=r[3];goto L8c01ae80; /* mov r3,r0 */
L8c01ae80: *stop=0x8c01ae80u;if(!valid_ram(r[0]+r[1],1u))return -3;r[0]=sx(m,r[1]+r[0],1);goto L8c01ae82; /* mov.b @(r0,r1),r0 */
L8c01ae82: *stop=0x8c01ae82u;r[1]=76u;goto L8c01ae84; /* mov #0x4c,r1 */
L8c01ae84: *stop=0x8c01ae84u;if(!valid_ram(r[3]+11u,1u))return -3;wr(m,r[3]+0xbu,r[0],1);goto L8c01ae86; /* mov.b r0,@(0xb,r3) */
L8c01ae86: *stop=0x8c01ae86u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01ae88; /* mov.l @r14,r3 */
L8c01ae88: *stop=0x8c01ae88u;r[0]=r[3];goto L8c01ae8a; /* mov r3,r0 */
L8c01ae8a: *stop=0x8c01ae8au;if(!valid_ram(r[0]+r[2],1u))return -3;r[0]=sx(m,r[2]+r[0],1);goto L8c01ae8c; /* mov.b @(r0,r2),r0 */
L8c01ae8c: *stop=0x8c01ae8cu;r[2]=124u;goto L8c01ae8e; /* mov #0x7c,r2 */
L8c01ae8e: *stop=0x8c01ae8eu;if(!valid_ram(r[3]+6u,1u))return -3;wr(m,r[3]+0x6u,r[0],1);goto L8c01ae90; /* mov.b r0,@(0x6,r3) */
L8c01ae90: *stop=0x8c01ae90u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01ae92; /* mov.l @r14,r3 */
L8c01ae92: *stop=0x8c01ae92u;r[0]=r[3];goto L8c01ae94; /* mov r3,r0 */
L8c01ae94: *stop=0x8c01ae94u;if(!valid_ram(r[0]+r[1],4u))return -3;r[0]=rd(m,r[1]+r[0],4);goto L8c01ae96; /* mov.l @(r0,r1),r0 */
L8c01ae96: *stop=0x8c01ae96u;r[1]=100u;goto L8c01ae98; /* mov #0x64,r1 */
L8c01ae98: *stop=0x8c01ae98u;if(!valid_ram(r[3]+12u,4u))return -3;wr(m,r[3]+0xcu,r[0],4);goto L8c01ae9a; /* mov.l r0,@(0xc,r3) */
L8c01ae9a: *stop=0x8c01ae9au;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01ae9c; /* mov.l @r14,r3 */
L8c01ae9c: *stop=0x8c01ae9cu;r[0]=r[3];goto L8c01ae9e; /* mov r3,r0 */
L8c01ae9e: *stop=0x8c01ae9eu;if(!valid_ram(r[0]+r[2],1u))return -3;r[0]=sx(m,r[2]+r[0],1);goto L8c01aea0; /* mov.b @(r0,r2),r0 */
L8c01aea0: *stop=0x8c01aea0u;if(!valid_ram(r[3]+9u,1u))return -3;wr(m,r[3]+0x9u,r[0],1);goto L8c01aea2; /* mov.b r0,@(0x9,r3) */
L8c01aea2: *stop=0x8c01aea2u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01aea4; /* mov.l @r14,r3 */
L8c01aea4: *stop=0x8c01aea4u;r[0]=r[3];goto L8c01aea6; /* mov r3,r0 */
L8c01aea6: *stop=0x8c01aea6u;if(!valid_ram(r[0]+r[1],4u))return -3;r[0]=rd(m,r[1]+r[0],4);goto L8c01aea8; /* mov.l @(r0,r1),r0 */
L8c01aea8: *stop=0x8c01aea8u;if(!valid_ram(r[3]+16u,4u))return -3;wr(m,r[3]+0x10u,r[0],4);goto L8c01aeaa; /* mov.l r0,@(0x10,r3) */
L8c01aeaa: *stop=0x8c01aeaau;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01aeac; /* mov.l @r14,r3 */
L8c01aeac: *stop=0x8c01aeacu;r[0]=r[3];goto L8c01aeae; /* mov r3,r0 */
L8c01aeae: *stop=0x8c01aeaeu;if(!valid_ram(r[0]+32u,4u))return -3;r[0]=rd(m,r[0]+0x20u,4);goto L8c01aeb0; /* mov.l @(0x20,r0),r0 */
L8c01aeb0: *stop=0x8c01aeb0u;if(!valid_ram(r[3]+8u,1u))return -3;wr(m,r[3]+0x8u,r[0],1);goto L8c01aeb2; /* mov.b r0,@(0x8,r3) */
L8c01aeb2: *stop=0x8c01aeb2u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01aeb4; /* mov.l @r14,r3 */
L8c01aeb4: *stop=0x8c01aeb4u;r[5]=6u;goto L8c01aeb6; /* mov #0x6,r5 */
L8c01aeb6: *stop=0x8c01aeb6u;r[1]=rd(m,0x8c01b004u,4);goto L8c01aeb8; /* mov.l 0x8c01b004,r1 */
L8c01aeb8: *stop=0x8c01aeb8u;r[0]=r[3];goto L8c01aeba; /* mov r3,r0 */
L8c01aeba: *stop=0x8c01aebau;if(!valid_ram(r[0]+6u,1u))return -3;r[0]=sx(m,r[0]+0x6u,1);goto L8c01aebc; /* mov.b @(0x6,r0),r0 */
L8c01aebc: *stop=0x8c01aebcu;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c01aebe; /* mov.l @r1,r2 */
L8c01aebe: *stop=0x8c01aebeu;r[0]<<=2;goto L8c01aec0; /* shll2 r0 */
L8c01aec0: *stop=0x8c01aec0u;r[6]=rd(m,0x8c01b00cu,4);goto L8c01aec2; /* mov.l 0x8c01b00c,r6 */
L8c01aec2: *stop=0x8c01aec2u;r[0]<<=2;goto L8c01aec4; /* shll2 r0 */
L8c01aec4: *stop=0x8c01aec4u;r[0]+=r[2];goto L8c01aec6; /* add r2,r0 */
L8c01aec6: *stop=0x8c01aec6u;if(!valid_ram(r[0]+5u,1u))return -3;r[0]=sx(m,r[0]+0x5u,1);goto L8c01aec8; /* mov.b @(0x5,r0),r0 */
L8c01aec8: *stop=0x8c01aec8u;r[2]=1u;goto L8c01aeca; /* mov #0x1,r2 */
L8c01aeca: *stop=0x8c01aecau;if(!valid_ram(r[3]+7u,1u))return -3;wr(m,r[3]+0x7u,r[0],1);goto L8c01aecc; /* mov.b r0,@(0x7,r3) */
L8c01aecc: *stop=0x8c01aeccu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01aece; /* mov.l @r14,r3 */
L8c01aece: *stop=0x8c01aeceu;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto L8c01aed0; /* mov.l r2,@r3 */
L8c01aed0: *stop=0x8c01aed0u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01aed2; /* mov.l @r14,r3 */
L8c01aed2: *stop=0x8c01aed2u;if(!valid_ram(r[3]+20u,4u))return -3;wr(m,r[3]+0x14u,r[13],4);goto L8c01aed4; /* mov.l r13,@(0x14,r3) */
L8c01aed4: *stop=0x8c01aed4u;r[3]=rd(m,0x8c01b008u,4);goto L8c01aed6; /* mov.l 0x8c01b008,r3 */
L8c01aed6: *stop=0x8c01aed6u;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c01aed8; /* mov.l @r3,r2 */
L8c01aed8: *stop=0x8c01aed8u;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[13],4);goto L8c01aeda; /* mov.l r13,@(0x14,r2) */
L8c01aeda: *stop=0x8c01aedau;r[2]=rd(m,0x8c01b010u,4);goto L8c01aedc; /* mov.l 0x8c01b010,r2 */
L8c01aedc: target=r[2];*pr=0x8c01aee0u;*stop=0x8c01aedeu;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01aee0; /* jsr @r2 */
L8c01aee0: *stop=0x8c01aee0u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01aee2; /* mov.l @r14,r3 */
L8c01aee2: *stop=0x8c01aee2u;if(!valid_ram(r[3]+8u,1u))return -3;r[0]=sx(m,r[3]+0x8u,1);goto L8c01aee4; /* mov.b @(0x8,r3),r0 */
L8c01aee4: *stop=0x8c01aee4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c01aee6; /* tst r0,r0 */
L8c01aee6: if(!(*sr&1u))goto L8c01aefa;goto L8c01aee8; /* bf 0x8c01aefa */
L8c01aee8: *stop=0x8c01aee8u;r[2]=rd(m,0x8c01b014u,4);goto L8c01aeea; /* mov.l 0x8c01b014,r2 */
L8c01aeea: *stop=0x8c01aeeau;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto L8c01aeec; /* mov.l @r2,r0 */
L8c01aeec: *stop=0x8c01aeecu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c01aeee; /* cmp/eq #-0x1,r0 */
L8c01aeee: if((*sr&1u))goto L8c01aefa;goto L8c01aef0; /* bt 0x8c01aefa */
L8c01aef0: *stop=0x8c01aef0u;r[1]=rd(m,0x8c01b018u,4);goto L8c01aef2; /* mov.l 0x8c01b018,r1 */
L8c01aef2: *stop=0x8c01aef2u;r[3]=rd(m,0x8c01b014u,4);goto L8c01aef4; /* mov.l 0x8c01b014,r3 */
L8c01aef4: *stop=0x8c01aef4u;if(!valid_ram(r[1],4u))return -3;r[5]=rd(m,r[1],4);goto L8c01aef6; /* mov.l @r1,r5 */
L8c01aef6: target=0x8c01aae0u;*pr=0x8c01aefau;*stop=0x8c01aef8u;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01aefa; /* bsr 0x8c01aae0 */
L8c01aefa: *stop=0x8c01aefau;r[3]=rd(m,0x8c01b01cu,4);goto L8c01aefc; /* mov.l 0x8c01b01c,r3 */
L8c01aefc: *stop=0x8c01aefcu;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[13],4);goto L8c01aefe; /* mov.l r13,@r3 */
L8c01aefe: *stop=0x8c01aefeu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01af00; /* lds.l @r15+,PR */
L8c01af00: *stop=0x8c01af00u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01af02; /* mov.l @r15+,r13 */
L8c01af02: *stop=0x8c01af02u;if(*pr!=entry_pr)return -3;*stop=0x8c01af04u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_state2_8c019adc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c019adc;
L8c019adc: *stop=0x8c019adcu;r[5]=rd(m,0x8c019bc0u,4);goto L8c019ade; /* mov.l 0x8c019bc0,r5 */
L8c019ade: *stop=0x8c019adeu;r[6]=6u;goto L8c019ae0; /* mov #0x6,r6 */
L8c019ae0: *stop=0x8c019ae0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c019ae2; /* mov.l r14,@-r15 */
L8c019ae2: *stop=0x8c019ae2u;r[6]<<=8;goto L8c019ae4; /* shll8 r6 */
L8c019ae4: *stop=0x8c019ae4u;r[14]=rd(m,0x8c019bc4u,4);goto L8c019ae6; /* mov.l 0x8c019bc4,r14 */
L8c019ae6: *stop=0x8c019ae6u;r[6]+=6u;goto L8c019ae8; /* add #0x6,r6 */
L8c019ae8: *stop=0x8c019ae8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c019aea; /* sts.l PR,@-r15 */
L8c019aea: *stop=0x8c019aeau;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c019aec; /* mov.l @r5,r3 */
L8c019aec: *stop=0x8c019aecu;r[2]=rd(m,0x8c019bc8u,4);goto L8c019aee; /* mov.l 0x8c019bc8,r2 */
L8c019aee: *stop=0x8c019aeeu;r[3]+=1u;goto L8c019af0; /* add #0x1,r3 */
L8c019af0: *stop=0x8c019af0u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[3],4);goto L8c019af2; /* mov.l r3,@r5 */
L8c019af2: *stop=0x8c019af2u;if(!valid_ram(r[14]+8u,4u))return -3;r[0]=rd(m,r[14]+0x8u,4);goto L8c019af4; /* mov.l @(0x8,r14),r0 */
L8c019af4: *stop=0x8c019af4u;*sr=(*sr&~1u)|!(r[0]&0xf0u);goto L8c019af6; /* tst #0xf0,r0 */
L8c019af6: take=!(*sr&1u);*stop=0x8c019af8u;if(!valid_ram(r[2],4u))return -3;r[4]=rd(m,r[2],4);if(take)goto L8c019b36;goto L8c019afa; /* bf/s 0x8c019b36 */
L8c019afa: *stop=0x8c019afau;if(!valid_ram(r[4]+6u,1u))return -3;r[0]=sx(m,r[4]+0x6u,1);goto L8c019afc; /* mov.b @(0x6,r4),r0 */
L8c019afc: *stop=0x8c019afcu;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c019afe; /* cmp/eq #0x40,r0 */
L8c019afe: if((*sr&1u))goto L8c019b04;goto L8c019b00; /* bt 0x8c019b04 */
L8c019b00: *stop=0x8c019b02u;goto L8c019be0; /* bra 0x8c019be0 */
L8c019b04: *stop=0x8c019b04u;if(!valid_ram(r[14]+8u,4u))return -3;r[1]=rd(m,r[14]+0x8u,4);goto L8c019b06; /* mov.l @(0x8,r14),r1 */
L8c019b06: *stop=0x8c019b06u;*sr=(*sr&~1u)|!(r[1]&r[6]);goto L8c019b08; /* tst r1,r6 */
L8c019b08: if(!(*sr&1u))goto L8c019b26;goto L8c019b0a; /* bf 0x8c019b26 */
L8c019b0a: *stop=0x8c019b0au;if(!valid_ram(r[14]+28u,2u))return -3;r[0]=sx(m,r[14]+0x1cu,2);goto L8c019b0c; /* mov.w @(0x1c,r14),r0 */
L8c019b0c: *stop=0x8c019b0cu;r[0]<<=8;goto L8c019b0e; /* shll8 r0 */
L8c019b0e: *stop=0x8c019b0eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c019b10; /* tst r0,r0 */
L8c019b10: if(!(*sr&1u))goto L8c019b26;goto L8c019b12; /* bf 0x8c019b26 */
L8c019b12: *stop=0x8c019b12u;if(!valid_ram(r[14]+30u,2u))return -3;r[0]=sx(m,r[14]+0x1eu,2);goto L8c019b14; /* mov.w @(0x1e,r14),r0 */
L8c019b14: *stop=0x8c019b14u;r[0]<<=8;goto L8c019b16; /* shll8 r0 */
L8c019b16: *stop=0x8c019b16u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c019b18; /* tst r0,r0 */
L8c019b18: if(!(*sr&1u))goto L8c019b26;goto L8c019b1a; /* bf 0x8c019b26 */
L8c019b1a: *stop=0x8c019b1au;r[3]=rd(m,0x8c019bccu,4);goto L8c019b1c; /* mov.l 0x8c019bcc,r3 */
L8c019b1c: *stop=0x8c019b1cu;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c019b1e; /* mov.l @r3,r1 */
L8c019b1e: *stop=0x8c019b1eu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c019b20; /* tst r1,r1 */
L8c019b20: if((*sr&1u))goto L8c019b26;goto L8c019b22; /* bt 0x8c019b26 */
L8c019b22: *stop=0x8c019b24u;goto L8c019be0; /* bra 0x8c019be0 */
L8c019b26: *stop=0x8c019b26u;r[2]=rd(m,0x8c019bd0u,4);goto L8c019b28; /* mov.l 0x8c019bd0,r2 */
L8c019b28: target=r[2];*pr=0x8c019b2cu;*stop=0x8c019b2au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019b2c; /* jsr @r2 */
L8c019b2c: *stop=0x8c019b2cu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019b2e; /* lds.l @r15+,PR */
L8c019b2e: *stop=0x8c019b2eu;r[3]=rd(m,0x8c019bd4u,4);goto L8c019b30; /* mov.l 0x8c019bd4,r3 */
L8c019b30: *stop=0x8c019b30u;r[4]=5u;goto L8c019b32; /* mov #0x5,r4 */
L8c019b32: target=r[3];*stop=0x8c019b34u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c019b36: *stop=0x8c019b36u;if(!valid_ram(r[4]+6u,1u))return -3;r[0]=sx(m,r[4]+0x6u,1);goto L8c019b38; /* mov.b @(0x6,r4),r0 */
L8c019b38: *stop=0x8c019b38u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c019b3a; /* cmp/eq #0x40,r0 */
L8c019b3a: if((*sr&1u))goto L8c019b7e;goto L8c019b3c; /* bt 0x8c019b7e */
L8c019b3c: *stop=0x8c019b3cu;if(!valid_ram(r[4]+8u,1u))return -3;r[0]=sx(m,r[4]+0x8u,1);goto L8c019b3e; /* mov.b @(0x8,r4),r0 */
L8c019b3e: *stop=0x8c019b3eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c019b40; /* cmp/eq #0x2,r0 */
L8c019b40: if((*sr&1u))goto L8c019b7e;goto L8c019b42; /* bt 0x8c019b7e */
L8c019b42: *stop=0x8c019b42u;r[3]=rd(m,0x8c019bccu,4);goto L8c019b44; /* mov.l 0x8c019bcc,r3 */
L8c019b44: *stop=0x8c019b44u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c019b46; /* mov.l @r3,r1 */
L8c019b46: *stop=0x8c019b46u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c019b48; /* tst r1,r1 */
L8c019b48: if((*sr&1u))goto L8c019b7e;goto L8c019b4a; /* bt 0x8c019b7e */
L8c019b4a: *stop=0x8c019b4au;if(!valid_ram(r[4]+6u,1u))return -3;r[0]=sx(m,r[4]+0x6u,1);goto L8c019b4c; /* mov.b @(0x6,r4),r0 */
L8c019b4c: *stop=0x8c019b4cu;*sr=(*sr&~1u)|(r[0]==0x00000017u);goto L8c019b4e; /* cmp/eq #0x17,r0 */
L8c019b4e: if((*sr&1u))goto L8c019be0;goto L8c019b50; /* bt 0x8c019be0 */
L8c019b50: *stop=0x8c019b50u;*sr=(*sr&~1u)|(r[0]==0x00000018u);goto L8c019b52; /* cmp/eq #0x18,r0 */
L8c019b52: if((*sr&1u))goto L8c019be0;goto L8c019b54; /* bt 0x8c019be0 */
L8c019b54: *stop=0x8c019b54u;*sr=(*sr&~1u)|(r[0]==0x00000019u);goto L8c019b56; /* cmp/eq #0x19,r0 */
L8c019b56: if((*sr&1u))goto L8c019be0;goto L8c019b58; /* bt 0x8c019be0 */
L8c019b58: *stop=0x8c019b58u;*sr=(*sr&~1u)|(r[0]==0x0000001cu);goto L8c019b5a; /* cmp/eq #0x1c,r0 */
L8c019b5a: if((*sr&1u))goto L8c019be0;goto L8c019b5c; /* bt 0x8c019be0 */
L8c019b5c: *stop=0x8c019b5cu;*sr=(*sr&~1u)|(r[0]==0x0000001du);goto L8c019b5e; /* cmp/eq #0x1d,r0 */
L8c019b5e: if((*sr&1u))goto L8c019be0;goto L8c019b60; /* bt 0x8c019be0 */
L8c019b60: *stop=0x8c019b60u;*sr=(*sr&~1u)|(r[0]==0x0000001eu);goto L8c019b62; /* cmp/eq #0x1e,r0 */
L8c019b62: if((*sr&1u))goto L8c019be0;goto L8c019b64; /* bt 0x8c019be0 */
L8c019b64: *stop=0x8c019b64u;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c019b66; /* mov.l @r5,r3 */
L8c019b66: *stop=0x8c019b66u;r[2]=30u;goto L8c019b68; /* mov #0x1e,r2 */
L8c019b68: *stop=0x8c019b68u;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c019b6a; /* cmp/hi r2,r3 */
L8c019b6a: if(!(*sr&1u))goto L8c019be0;goto L8c019b6c; /* bf 0x8c019be0 */
L8c019b6c: *stop=0x8c019b6cu;if(!valid_ram(r[4]+8u,1u))return -3;r[0]=sx(m,r[4]+0x8u,1);goto L8c019b6e; /* mov.b @(0x8,r4),r0 */
L8c019b6e: *stop=0x8c019b6eu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c019b70; /* cmp/eq #0x4,r0 */
L8c019b70: if((*sr&1u))goto L8c019be0;goto L8c019b72; /* bt 0x8c019be0 */
L8c019b72: *stop=0x8c019b72u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019b74; /* lds.l @r15+,PR */
L8c019b74: *stop=0x8c019b74u;r[2]=rd(m,0x8c019bd8u,4);goto L8c019b76; /* mov.l 0x8c019bd8,r2 */
L8c019b76: *stop=0x8c019b76u;r[5]=5u;goto L8c019b78; /* mov #0x5,r5 */
L8c019b78: *stop=0x8c019b78u;r[4]=64u;goto L8c019b7a; /* mov #0x40,r4 */
L8c019b7a: target=r[2];*stop=0x8c019b7cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c019b7e: *stop=0x8c019b7eu;if(!valid_ram(r[4]+6u,1u))return -3;r[0]=sx(m,r[4]+0x6u,1);goto L8c019b80; /* mov.b @(0x6,r4),r0 */
L8c019b80: *stop=0x8c019b80u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c019b82; /* cmp/eq #0x40,r0 */
L8c019b82: if(!(*sr&1u))goto L8c019be0;goto L8c019b84; /* bf 0x8c019be0 */
L8c019b84: *stop=0x8c019b84u;if(!valid_ram(r[14]+8u,4u))return -3;r[2]=rd(m,r[14]+0x8u,4);goto L8c019b86; /* mov.l @(0x8,r14),r2 */
L8c019b86: *stop=0x8c019b86u;*sr=(*sr&~1u)|!(r[2]&r[6]);goto L8c019b88; /* tst r2,r6 */
L8c019b88: if(!(*sr&1u))goto L8c019ba2;goto L8c019b8a; /* bf 0x8c019ba2 */
L8c019b8a: *stop=0x8c019b8au;if(!valid_ram(r[14]+28u,2u))return -3;r[0]=sx(m,r[14]+0x1cu,2);goto L8c019b8c; /* mov.w @(0x1c,r14),r0 */
L8c019b8c: *stop=0x8c019b8cu;r[0]<<=8;goto L8c019b8e; /* shll8 r0 */
L8c019b8e: *stop=0x8c019b8eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c019b90; /* tst r0,r0 */
L8c019b90: if(!(*sr&1u))goto L8c019ba2;goto L8c019b92; /* bf 0x8c019ba2 */
L8c019b92: *stop=0x8c019b92u;if(!valid_ram(r[14]+30u,2u))return -3;r[0]=sx(m,r[14]+0x1eu,2);goto L8c019b94; /* mov.w @(0x1e,r14),r0 */
L8c019b94: *stop=0x8c019b94u;r[0]<<=8;goto L8c019b96; /* shll8 r0 */
L8c019b96: *stop=0x8c019b96u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c019b98; /* tst r0,r0 */
L8c019b98: if(!(*sr&1u))goto L8c019ba2;goto L8c019b9a; /* bf 0x8c019ba2 */
L8c019b9a: *stop=0x8c019b9au;r[3]=rd(m,0x8c019bccu,4);goto L8c019b9c; /* mov.l 0x8c019bcc,r3 */
L8c019b9c: *stop=0x8c019b9cu;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c019b9e; /* mov.l @r3,r2 */
L8c019b9e: *stop=0x8c019b9eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c019ba0; /* tst r2,r2 */
L8c019ba0: if(!(*sr&1u))goto L8c019be0;goto L8c019ba2; /* bf 0x8c019be0 */
L8c019ba2: *stop=0x8c019ba2u;r[2]=rd(m,0x8c019bd0u,4);goto L8c019ba4; /* mov.l 0x8c019bd0,r2 */
L8c019ba4: target=r[2];*pr=0x8c019ba8u;*stop=0x8c019ba6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019ba8; /* jsr @r2 */
L8c019ba8: *stop=0x8c019ba8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019baa; /* lds.l @r15+,PR */
L8c019baa: *stop=0x8c019baau;r[3]=rd(m,0x8c019bd4u,4);goto L8c019bac; /* mov.l 0x8c019bd4,r3 */
L8c019bac: *stop=0x8c019bacu;r[4]=5u;goto L8c019bae; /* mov #0x5,r4 */
L8c019bae: target=r[3];*stop=0x8c019bb0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c019be0: *stop=0x8c019be0u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019be2; /* lds.l @r15+,PR */
L8c019be2: *stop=0x8c019be2u;if(*pr!=entry_pr)return -3;*stop=0x8c019be4u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_state2_8c019be6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c019be6;
L8c019be6: *stop=0x8c019be6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c019be8; /* mov.l r14,@-r15 */
L8c019be8: *stop=0x8c019be8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c019bea; /* sts.l PR,@-r15 */
L8c019bea: *stop=0x8c019beau;r[15]+=4294967292u;goto L8c019bec; /* add #-0x4,r15 */
L8c019bec: *stop=0x8c019becu;r[2]=rd(m,0x8c019ca0u,4);goto L8c019bee; /* mov.l 0x8c019ca0,r2 */
L8c019bee: *stop=0x8c019beeu;r[14]=r[5];goto L8c019bf0; /* mov r5,r14 */
L8c019bf0: *stop=0x8c019bf0u;if(!valid_ram(r[15],2u))return -3;wr(m,r[15],r[4],2);goto L8c019bf2; /* mov.w r4,@r15 */
L8c019bf2: *stop=0x8c019bf2u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c019bf4; /* mov.l @r2,r3 */
L8c019bf4: *stop=0x8c019bf4u;if(!valid_ram(r[3]+4u,2u))return -3;r[0]=sx(m,r[3]+0x4u,2);goto L8c019bf6; /* mov.w @(0x4,r3),r0 */
L8c019bf6: *stop=0x8c019bf6u;if(!valid_ram(r[15],2u))return -3;r[3]=sx(m,r[15],2);goto L8c019bf8; /* mov.w @r15,r3 */
L8c019bf8: *stop=0x8c019bf8u;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c019bfa; /* cmp/eq r3,r0 */
L8c019bfa: take=(*sr&1u);*stop=0x8c019bfcu;r[4]=0u;if(take)goto L8c019c48;goto L8c019bfe; /* bt/s 0x8c019c48 */
L8c019bfe: *stop=0x8c019bfeu;r[1]=rd(m,0x8c019ca4u,4);goto L8c019c00; /* mov.l 0x8c019ca4,r1 */
L8c019c00: *stop=0x8c019c00u;if(!valid_ram(r[1],4u))return -3;r[3]=rd(m,r[1],4);goto L8c019c02; /* mov.l @r1,r3 */
L8c019c02: *stop=0x8c019c02u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c019c04; /* tst r3,r3 */
L8c019c04: if((*sr&1u))goto L8c019c48;goto L8c019c06; /* bt 0x8c019c48 */
L8c019c06: *stop=0x8c019c06u;if(!valid_ram(r[14]+3u,1u))return -3;r[0]=sx(m,r[14]+0x3u,1);goto L8c019c08; /* mov.b @(0x3,r14),r0 */
L8c019c08: *stop=0x8c019c08u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c019c0a; /* cmp/eq #0x0,r0 */
L8c019c0a: if((*sr&1u))goto L8c019c18;goto L8c019c0c; /* bt 0x8c019c18 */
L8c019c0c: *stop=0x8c019c0cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c019c0e; /* cmp/eq #0x1,r0 */
L8c019c0e: if((*sr&1u))goto L8c019c20;goto L8c019c10; /* bt 0x8c019c20 */
L8c019c10: *stop=0x8c019c10u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c019c12; /* cmp/eq #0x2,r0 */
L8c019c12: if((*sr&1u))goto L8c019c28;goto L8c019c14; /* bt 0x8c019c28 */
L8c019c14: *stop=0x8c019c16u;goto L8c019c2e; /* bra 0x8c019c2e */
L8c019c18: target=0x8c01a1acu;*pr=0x8c019c1cu;*stop=0x8c019c1au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019c1c; /* bsr 0x8c01a1ac */
L8c019c1c: *stop=0x8c019c1eu;goto L8c019c2c; /* bra 0x8c019c2c */
L8c019c20: target=0x8c01a240u;*pr=0x8c019c24u;*stop=0x8c019c22u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019c24; /* bsr 0x8c01a240 */
L8c019c24: *stop=0x8c019c26u;goto L8c019c2c; /* bra 0x8c019c2c */
L8c019c28: target=0x8c01a480u;*pr=0x8c019c2cu;*stop=0x8c019c2au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019c2c; /* bsr 0x8c01a480 */
L8c019c2c: *stop=0x8c019c2cu;r[4]=r[0];goto L8c019c2e; /* mov r0,r4 */
L8c019c2e: *stop=0x8c019c2eu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c019c30; /* tst r4,r4 */
L8c019c30: if((*sr&1u))goto L8c019c48;goto L8c019c32; /* bt 0x8c019c48 */
L8c019c32: *stop=0x8c019c32u;if(!valid_ram(r[15],2u))return -3;r[5]=sx(m,r[15],2);goto L8c019c34; /* mov.w @r15,r5 */
L8c019c34: target=0x8c01a7d6u;*pr=0x8c019c38u;*stop=0x8c019c36u;if(!valid_ram(r[14],1u))return -3;r[4]=sx(m,r[14],1);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019c38; /* bsr 0x8c01a7d6 */
L8c019c38: *stop=0x8c019c38u;r[6]=rd(m,0x8c019ca8u,4);goto L8c019c3a; /* mov.l 0x8c019ca8,r6 */
L8c019c3a: *stop=0x8c019c3au;r[4]=8u;goto L8c019c3c; /* mov #0x8,r4 */
L8c019c3c: *stop=0x8c019c3cu;r[5]=6u;goto L8c019c3e; /* mov #0x6,r5 */
L8c019c3e: *stop=0x8c019c3eu;r[15]+=4u;goto L8c019c40; /* add #0x4,r15 */
L8c019c40: *stop=0x8c019c40u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019c42; /* lds.l @r15+,PR */
L8c019c42: *stop=0x8c019c42u;r[2]=rd(m,0x8c019cacu,4);goto L8c019c44; /* mov.l 0x8c019cac,r2 */
L8c019c44: target=r[2];*stop=0x8c019c46u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c019c48: *stop=0x8c019c48u;r[15]+=4u;goto L8c019c4a; /* add #0x4,r15 */
L8c019c4a: *stop=0x8c019c4au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019c4c; /* lds.l @r15+,PR */
L8c019c4c: *stop=0x8c019c4cu;if(*pr!=entry_pr)return -3;*stop=0x8c019c4eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_state2_8c019c50(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c019c50;
L8c019c50: *stop=0x8c019c50u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c019c52; /* mov.l r14,@-r15 */
L8c019c52: *stop=0x8c019c52u;r[14]=r[5];goto L8c019c54; /* mov r5,r14 */
L8c019c54: *stop=0x8c019c54u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c019c56; /* mov.l r13,@-r15 */
L8c019c56: *stop=0x8c019c56u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c019c58; /* mov.l r12,@-r15 */
L8c019c58: *stop=0x8c019c58u;r[3]=rd(m,0x8c019ca0u,4);goto L8c019c5a; /* mov.l 0x8c019ca0,r3 */
L8c019c5a: *stop=0x8c019c5au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c019c5c; /* mov.l r11,@-r15 */
L8c019c5c: *stop=0x8c019c5cu;r[11]=r[4];goto L8c019c5e; /* mov r4,r11 */
L8c019c5e: *stop=0x8c019c5eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c019c60; /* sts.l PR,@-r15 */
L8c019c60: *stop=0x8c019c60u;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);goto L8c019c62; /* mov.l @r3,r4 */
L8c019c62: *stop=0x8c019c62u;r[2]=(uint32_t)(int32_t)(int16_t)r[11];goto L8c019c64; /* exts.w r11,r2 */
L8c019c64: *stop=0x8c019c64u;r[12]=rd(m,0x8c019cacu,4);goto L8c019c66; /* mov.l 0x8c019cac,r12 */
L8c019c66: *stop=0x8c019c66u;if(!valid_ram(r[4]+4u,2u))return -3;r[0]=sx(m,r[4]+0x4u,2);goto L8c019c68; /* mov.w @(0x4,r4),r0 */
L8c019c68: *stop=0x8c019c68u;r[5]=rd(m,0x8c019ca4u,4);goto L8c019c6a; /* mov.l 0x8c019ca4,r5 */
L8c019c6a: *stop=0x8c019c6au;*sr=(*sr&~1u)|(r[2]==r[0]);goto L8c019c6c; /* cmp/eq r2,r0 */
L8c019c6c: take=!(*sr&1u);*stop=0x8c019c6eu;r[13]=0u;if(take)goto L8c019cda;goto L8c019c70; /* bf/s 0x8c019cda */
L8c019c70: *stop=0x8c019c70u;if(!valid_ram(r[5],4u))return -3;r[2]=rd(m,r[5],4);goto L8c019c72; /* mov.l @r5,r2 */
L8c019c72: *stop=0x8c019c72u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c019c74; /* tst r2,r2 */
L8c019c74: if(!(*sr&1u))goto L8c019c7a;goto L8c019c76; /* bf 0x8c019c7a */
L8c019c76: *stop=0x8c019c78u;goto L8c019db8; /* bra 0x8c019db8 */
L8c019c7a: *stop=0x8c019c7au;if(!valid_ram(r[14]+3u,1u))return -3;r[0]=sx(m,r[14]+0x3u,1);goto L8c019c7c; /* mov.b @(0x3,r14),r0 */
L8c019c7c: *stop=0x8c019c7cu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c019c7e; /* cmp/eq #0x0,r0 */
L8c019c7e: if((*sr&1u))goto L8c019c88;goto L8c019c80; /* bt 0x8c019c88 */
L8c019c80: *stop=0x8c019c80u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c019c82; /* cmp/eq #0x2,r0 */
L8c019c82: if((*sr&1u))goto L8c019c90;goto L8c019c84; /* bt 0x8c019c90 */
L8c019c84: *stop=0x8c019c86u;goto L8c019cc0; /* bra 0x8c019cc0 */
L8c019c88: target=0x8c01a1acu;*pr=0x8c019c8cu;*stop=0x8c019c8au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019c8c; /* bsr 0x8c01a1ac */
L8c019c8c: *stop=0x8c019c8eu;goto L8c019c94; /* bra 0x8c019c94 */
L8c019c90: target=0x8c01a480u;*pr=0x8c019c94u;*stop=0x8c019c92u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019c94; /* bsr 0x8c01a480 */
L8c019c94: *stop=0x8c019c96u;r[4]=r[0];goto L8c019cc2; /* bra 0x8c019cc2 */
L8c019cc0: *stop=0x8c019cc0u;r[4]=r[13];goto L8c019cc2; /* mov r13,r4 */
L8c019cc2: *stop=0x8c019cc2u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c019cc4; /* tst r4,r4 */
L8c019cc4: if((*sr&1u))goto L8c019cca;goto L8c019cc6; /* bt 0x8c019cca */
L8c019cc6: *stop=0x8c019cc8u;goto L8c019db8; /* bra 0x8c019db8 */
L8c019cca: target=0x8c01a9b8u;*pr=0x8c019cceu;*stop=0x8c019cccu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019cce; /* bsr 0x8c01a9b8 */
L8c019cce: *stop=0x8c019cceu;r[6]=rd(m,0x8c019d80u,4);goto L8c019cd0; /* mov.l 0x8c019d80,r6 */
L8c019cd0: *stop=0x8c019cd0u;r[5]=6u;goto L8c019cd2; /* mov #0x6,r5 */
L8c019cd2: target=r[12];*pr=0x8c019cd6u;*stop=0x8c019cd4u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019cd6; /* jsr @r12 */
L8c019cd6: *stop=0x8c019cd8u;goto L8c019db8; /* bra 0x8c019db8 */
L8c019cda: *stop=0x8c019cdau;if(!valid_ram(r[14],1u))return -3;r[1]=sx(m,r[14],1);goto L8c019cdc; /* mov.b @r14,r1 */
L8c019cdc: *stop=0x8c019cdcu;r[2]=rd(m,0x8c019d84u,4);goto L8c019cde; /* mov.l 0x8c019d84,r2 */
L8c019cde: *stop=0x8c019cdeu;r[1]<<=2;goto L8c019ce0; /* shll2 r1 */
L8c019ce0: *stop=0x8c019ce0u;if(!valid_ram(r[2],4u))return -3;r[6]=rd(m,r[2],4);goto L8c019ce2; /* mov.l @r2,r6 */
L8c019ce2: *stop=0x8c019ce2u;r[1]<<=2;goto L8c019ce4; /* shll2 r1 */
L8c019ce4: *stop=0x8c019ce4u;r[1]+=r[6];goto L8c019ce6; /* add r6,r1 */
L8c019ce6: *stop=0x8c019ce6u;if(!valid_ram(r[1]+4u,1u))return -3;r[0]=sx(m,r[1]+0x4u,1);goto L8c019ce8; /* mov.b @(0x4,r1),r0 */
L8c019ce8: *stop=0x8c019ce8u;r[1]=r[0];goto L8c019cea; /* mov r0,r1 */
L8c019cea: *stop=0x8c019ceau;if(!valid_ram(r[4]+8u,1u))return -3;r[0]=sx(m,r[4]+0x8u,1);goto L8c019cec; /* mov.b @(0x8,r4),r0 */
L8c019cec: *stop=0x8c019cecu;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[0]);goto L8c019cee; /* cmp/gt r0,r1 */
L8c019cee: if(!(*sr&1u))goto L8c019d26;goto L8c019cf0; /* bf 0x8c019d26 */
L8c019cf0: *stop=0x8c019cf0u;if(!valid_ram(r[14]+3u,1u))return -3;r[0]=sx(m,r[14]+0x3u,1);goto L8c019cf2; /* mov.b @(0x3,r14),r0 */
L8c019cf2: *stop=0x8c019cf2u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c019cf4; /* cmp/eq #0x0,r0 */
L8c019cf4: if((*sr&1u))goto L8c019cfe;goto L8c019cf6; /* bt 0x8c019cfe */
L8c019cf6: *stop=0x8c019cf6u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c019cf8; /* cmp/eq #0x2,r0 */
L8c019cf8: if((*sr&1u))goto L8c019d06;goto L8c019cfa; /* bt 0x8c019d06 */
L8c019cfa: *stop=0x8c019cfcu;goto L8c019d0e; /* bra 0x8c019d0e */
L8c019cfe: target=0x8c01a1acu;*pr=0x8c019d02u;*stop=0x8c019d00u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019d02; /* bsr 0x8c01a1ac */
L8c019d02: *stop=0x8c019d04u;goto L8c019d0a; /* bra 0x8c019d0a */
L8c019d06: target=0x8c01a480u;*pr=0x8c019d0au;*stop=0x8c019d08u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019d0a; /* bsr 0x8c01a480 */
L8c019d0a: *stop=0x8c019d0cu;r[4]=r[0];goto L8c019d10; /* bra 0x8c019d10 */
L8c019d0e: *stop=0x8c019d0eu;r[4]=r[13];goto L8c019d10; /* mov r13,r4 */
L8c019d10: *stop=0x8c019d10u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c019d12; /* tst r4,r4 */
L8c019d12: if((*sr&1u))goto L8c019db8;goto L8c019d14; /* bt 0x8c019db8 */
L8c019d14: *stop=0x8c019d14u;r[5]=r[11];goto L8c019d16; /* mov r11,r5 */
L8c019d16: target=0x8c01a7d6u;*pr=0x8c019d1au;*stop=0x8c019d18u;if(!valid_ram(r[14],1u))return -3;r[4]=sx(m,r[14],1);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019d1a; /* bsr 0x8c01a7d6 */
L8c019d1a: *stop=0x8c019d1au;r[6]=rd(m,0x8c019d80u,4);goto L8c019d1c; /* mov.l 0x8c019d80,r6 */
L8c019d1c: *stop=0x8c019d1cu;r[5]=6u;goto L8c019d1e; /* mov #0x6,r5 */
L8c019d1e: target=r[12];*pr=0x8c019d22u;*stop=0x8c019d20u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019d22; /* jsr @r12 */
L8c019d22: *stop=0x8c019d24u;goto L8c019db8; /* bra 0x8c019db8 */
L8c019d26: *stop=0x8c019d26u;if(!valid_ram(r[14],1u))return -3;r[1]=sx(m,r[14],1);goto L8c019d28; /* mov.b @r14,r1 */
L8c019d28: *stop=0x8c019d28u;r[1]<<=2;goto L8c019d2a; /* shll2 r1 */
L8c019d2a: *stop=0x8c019d2au;r[1]<<=2;goto L8c019d2c; /* shll2 r1 */
L8c019d2c: *stop=0x8c019d2cu;r[6]+=r[1];goto L8c019d2e; /* add r1,r6 */
L8c019d2e: *stop=0x8c019d2eu;if(!valid_ram(r[6]+4u,1u))return -3;r[0]=sx(m,r[6]+0x4u,1);goto L8c019d30; /* mov.b @(0x4,r6),r0 */
L8c019d30: *stop=0x8c019d30u;r[1]=r[0];goto L8c019d32; /* mov r0,r1 */
L8c019d32: *stop=0x8c019d32u;if(!valid_ram(r[4]+8u,1u))return -3;r[0]=sx(m,r[4]+0x8u,1);goto L8c019d34; /* mov.b @(0x8,r4),r0 */
L8c019d34: *stop=0x8c019d34u;*sr=(*sr&~1u)|(r[0]==r[1]);goto L8c019d36; /* cmp/eq r0,r1 */
L8c019d36: if(!(*sr&1u))goto L8c019db8;goto L8c019d38; /* bf 0x8c019db8 */
L8c019d38: *stop=0x8c019d38u;if(!valid_ram(r[5],4u))return -3;r[1]=rd(m,r[5],4);goto L8c019d3a; /* mov.l @r5,r1 */
L8c019d3a: *stop=0x8c019d3au;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c019d3c; /* tst r1,r1 */
L8c019d3c: if((*sr&1u))goto L8c019db8;goto L8c019d3e; /* bt 0x8c019db8 */
L8c019d3e: *stop=0x8c019d3eu;if(!valid_ram(r[4]+4u,2u))return -3;r[0]=sx(m,r[4]+0x4u,2);goto L8c019d40; /* mov.w @(0x4,r4),r0 */
L8c019d40: *stop=0x8c019d40u;r[3]=rd(m,0x8c019d88u,4);goto L8c019d42; /* mov.l 0x8c019d88,r3 */
L8c019d42: *stop=0x8c019d42u;r[0]<<=2;goto L8c019d44; /* shll2 r0 */
L8c019d44: *stop=0x8c019d44u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c019d46; /* mov.l @r3,r1 */
L8c019d46: *stop=0x8c019d46u;r[0]<<=2;goto L8c019d48; /* shll2 r0 */
L8c019d48: *stop=0x8c019d48u;r[0]<<=2;goto L8c019d4a; /* shll2 r0 */
L8c019d4a: *stop=0x8c019d4au;r[0]+=r[1];goto L8c019d4c; /* add r1,r0 */
L8c019d4c: *stop=0x8c019d4cu;if(!valid_ram(r[0]+1u,1u))return -3;r[0]=sx(m,r[0]+0x1u,1);goto L8c019d4e; /* mov.b @(0x1,r0),r0 */
L8c019d4e: *stop=0x8c019d4eu;r[2]=r[0];goto L8c019d50; /* mov r0,r2 */
L8c019d50: *stop=0x8c019d50u;if(!valid_ram(r[14]+1u,1u))return -3;r[0]=sx(m,r[14]+0x1u,1);goto L8c019d52; /* mov.b @(0x1,r14),r0 */
L8c019d52: *stop=0x8c019d52u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[0]);goto L8c019d54; /* cmp/ge r0,r2 */
L8c019d54: if((*sr&1u))goto L8c019db8;goto L8c019d56; /* bt 0x8c019db8 */
L8c019d56: *stop=0x8c019d56u;if(!valid_ram(r[14]+3u,1u))return -3;r[0]=sx(m,r[14]+0x3u,1);goto L8c019d58; /* mov.b @(0x3,r14),r0 */
L8c019d58: *stop=0x8c019d58u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c019d5a; /* cmp/eq #0x0,r0 */
L8c019d5a: if((*sr&1u))goto L8c019d64;goto L8c019d5c; /* bt 0x8c019d64 */
L8c019d5c: *stop=0x8c019d5cu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c019d5e; /* cmp/eq #0x2,r0 */
L8c019d5e: if((*sr&1u))goto L8c019d6c;goto L8c019d60; /* bt 0x8c019d6c */
L8c019d60: *stop=0x8c019d62u;goto L8c019da0; /* bra 0x8c019da0 */
L8c019d64: target=0x8c01a1acu;*pr=0x8c019d68u;*stop=0x8c019d66u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019d68; /* bsr 0x8c01a1ac */
L8c019d68: *stop=0x8c019d6au;goto L8c019d70; /* bra 0x8c019d70 */
L8c019d6c: target=0x8c01a480u;*pr=0x8c019d70u;*stop=0x8c019d6eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019d70; /* bsr 0x8c01a480 */
L8c019d70: *stop=0x8c019d72u;r[4]=r[0];goto L8c019da2; /* bra 0x8c019da2 */
L8c019da0: *stop=0x8c019da0u;r[4]=r[13];goto L8c019da2; /* mov r13,r4 */
L8c019da2: *stop=0x8c019da2u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c019da4; /* tst r4,r4 */
L8c019da4: if((*sr&1u))goto L8c019db8;goto L8c019da6; /* bt 0x8c019db8 */
L8c019da6: target=0x8c01a9b8u;*pr=0x8c019daau;*stop=0x8c019da8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019daa; /* bsr 0x8c01a9b8 */
L8c019daa: *stop=0x8c019daau;r[5]=r[11];goto L8c019dac; /* mov r11,r5 */
L8c019dac: target=0x8c01a7d6u;*pr=0x8c019db0u;*stop=0x8c019daeu;if(!valid_ram(r[14],1u))return -3;r[4]=sx(m,r[14],1);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019db0; /* bsr 0x8c01a7d6 */
L8c019db0: *stop=0x8c019db0u;r[6]=rd(m,0x8c019f20u,4);goto L8c019db2; /* mov.l 0x8c019f20,r6 */
L8c019db2: *stop=0x8c019db2u;r[5]=6u;goto L8c019db4; /* mov #0x6,r5 */
L8c019db4: target=r[12];*pr=0x8c019db8u;*stop=0x8c019db6u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019db8; /* jsr @r12 */
L8c019db8: *stop=0x8c019db8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019dba; /* lds.l @r15+,PR */
L8c019dba: *stop=0x8c019dbau;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c019dbc; /* mov.l @r15+,r11 */
L8c019dbc: *stop=0x8c019dbcu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c019dbe; /* mov.l @r15+,r12 */
L8c019dbe: *stop=0x8c019dbeu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c019dc0; /* mov.l @r15+,r13 */
L8c019dc0: *stop=0x8c019dc0u;if(*pr!=entry_pr)return -3;*stop=0x8c019dc2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_state2_8c019dc4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c019dc4;
L8c019dc4: *stop=0x8c019dc4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c019dc6; /* mov.l r14,@-r15 */
L8c019dc6: *stop=0x8c019dc6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c019dc8; /* mov.l r13,@-r15 */
L8c019dc8: *stop=0x8c019dc8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c019dca; /* sts.l PR,@-r15 */
L8c019dca: *stop=0x8c019dcau;r[15]+=4294967292u;goto L8c019dcc; /* add #-0x4,r15 */
L8c019dcc: *stop=0x8c019dccu;r[14]=r[5];goto L8c019dce; /* mov r5,r14 */
L8c019dce: *stop=0x8c019dceu;r[2]=rd(m,0x8c019f24u,4);goto L8c019dd0; /* mov.l 0x8c019f24,r2 */
L8c019dd0: *stop=0x8c019dd0u;if(!valid_ram(r[15],2u))return -3;wr(m,r[15],r[4],2);goto L8c019dd2; /* mov.w r4,@r15 */
L8c019dd2: *stop=0x8c019dd2u;r[13]=0u;goto L8c019dd4; /* mov #0x0,r13 */
L8c019dd4: *stop=0x8c019dd4u;if(!valid_ram(r[14],1u))return -3;r[0]=sx(m,r[14],1);goto L8c019dd6; /* mov.b @r14,r0 */
L8c019dd6: *stop=0x8c019dd6u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c019dd8; /* mov.l @r2,r3 */
L8c019dd8: *stop=0x8c019dd8u;r[0]<<=2;goto L8c019dda; /* shll2 r0 */
L8c019dda: *stop=0x8c019ddau;r[0]<<=2;goto L8c019ddc; /* shll2 r0 */
L8c019ddc: *stop=0x8c019ddcu;r[0]+=r[3];goto L8c019dde; /* add r3,r0 */
L8c019dde: *stop=0x8c019ddeu;if(!valid_ram(r[0]+4u,1u))return -3;r[0]=sx(m,r[0]+0x4u,1);goto L8c019de0; /* mov.b @(0x4,r0),r0 */
L8c019de0: *stop=0x8c019de0u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c019de2; /* cmp/eq #0x4,r0 */
L8c019de2: take=!(*sr&1u);*stop=0x8c019de4u;r[4]=r[13];if(take)goto L8c019ea4;goto L8c019de6; /* bf/s 0x8c019ea4 */
L8c019de6: *stop=0x8c019de6u;if(!valid_ram(r[14]+3u,1u))return -3;r[0]=sx(m,r[14]+0x3u,1);goto L8c019de8; /* mov.b @(0x3,r14),r0 */
L8c019de8: *stop=0x8c019de8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c019dea; /* cmp/eq #0x1,r0 */
L8c019dea: if(!(*sr&1u))goto L8c019df2;goto L8c019dec; /* bf 0x8c019df2 */
L8c019dec: target=0x8c01a240u;*pr=0x8c019df0u;*stop=0x8c019deeu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019df0; /* bsr 0x8c01a240 */
L8c019df0: *stop=0x8c019df0u;r[4]=r[0];goto L8c019df2; /* mov r0,r4 */
L8c019df2: *stop=0x8c019df2u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c019df4; /* tst r4,r4 */
L8c019df4: if(!(*sr&1u))goto L8c019dfa;goto L8c019df6; /* bf 0x8c019dfa */
L8c019df6: *stop=0x8c019df8u;goto L8c019f40; /* bra 0x8c019f40 */
L8c019dfa: *stop=0x8c019dfau;r[4]=rd(m,0x8c019f28u,4);goto L8c019dfc; /* mov.l 0x8c019f28,r4 */
L8c019dfc: *stop=0x8c019dfcu;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c019dfe; /* mov.l @r4,r2 */
L8c019dfe: *stop=0x8c019dfeu;if(!valid_ram(r[2]+32u,4u))return -3;r[3]=rd(m,r[2]+0x20u,4);goto L8c019e00; /* mov.l @(0x20,r2),r3 */
L8c019e00: *stop=0x8c019e00u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c019e02; /* tst r3,r3 */
L8c019e02: take=(*sr&1u);*stop=0x8c019e04u;r[5]=2u;if(take)goto L8c019e24;goto L8c019e06; /* bt/s 0x8c019e24 */
L8c019e06: *stop=0x8c019e06u;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);goto L8c019e08; /* mov.l @r4,r2 */
L8c019e08: *stop=0x8c019e08u;r[3]=r[5];goto L8c019e0a; /* mov r5,r3 */
L8c019e0a: *stop=0x8c019e0au;r[3]<<=2;goto L8c019e0c; /* shll2 r3 */
L8c019e0c: *stop=0x8c019e0cu;if(!valid_ram(r[4],4u))return -3;r[0]=rd(m,r[4],4);goto L8c019e0e; /* mov.l @r4,r0 */
L8c019e0e: *stop=0x8c019e0eu;r[2]+=24u;goto L8c019e10; /* add #0x18,r2 */
L8c019e10: *stop=0x8c019e10u;r[3]+=r[2];goto L8c019e12; /* add r2,r3 */
L8c019e12: *stop=0x8c019e12u;r[2]=r[0];goto L8c019e14; /* mov r0,r2 */
L8c019e14: *stop=0x8c019e14u;if(!valid_ram(r[3],4u))return -3;r[5]=rd(m,r[3],4);goto L8c019e16; /* mov.l @r3,r5 */
L8c019e16: *stop=0x8c019e16u;r[2]+=24u;goto L8c019e18; /* add #0x18,r2 */
L8c019e18: *stop=0x8c019e18u;r[3]=r[5];goto L8c019e1a; /* mov r5,r3 */
L8c019e1a: *stop=0x8c019e1au;r[3]<<=2;goto L8c019e1c; /* shll2 r3 */
L8c019e1c: *stop=0x8c019e1cu;r[3]+=r[2];goto L8c019e1e; /* add r2,r3 */
L8c019e1e: *stop=0x8c019e1eu;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c019e20; /* mov.l @r3,r1 */
L8c019e20: *stop=0x8c019e20u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c019e22; /* tst r1,r1 */
L8c019e22: if(!(*sr&1u))goto L8c019e06;goto L8c019e24; /* bf 0x8c019e06 */
L8c019e24: *stop=0x8c019e24u;if(!valid_ram(r[15],2u))return -3;r[13]=sx(m,r[15],2);goto L8c019e26; /* mov.w @r15,r13 */
L8c019e26: *stop=0x8c019e26u;r[6]=rd(m,0x8c019f2cu,4);goto L8c019e28; /* mov.l 0x8c019f2c,r6 */
L8c019e28: *stop=0x8c019e28u;r[13]<<=2;goto L8c019e2a; /* shll2 r13 */
L8c019e2a: *stop=0x8c019e2au;if(!valid_ram(r[6],4u))return -3;r[0]=rd(m,r[6],4);goto L8c019e2c; /* mov.l @r6,r0 */
L8c019e2c: *stop=0x8c019e2cu;r[13]<<=2;goto L8c019e2e; /* shll2 r13 */
L8c019e2e: *stop=0x8c019e2eu;r[13]<<=2;goto L8c019e30; /* shll2 r13 */
L8c019e30: *stop=0x8c019e30u;if(!valid_ram(r[0]+r[13],1u))return -3;r[0]=sx(m,r[13]+r[0],1);goto L8c019e32; /* mov.b @(r0,r13),r0 */
L8c019e32: *stop=0x8c019e32u;*sr=(*sr&~1u)|(r[0]==0x0000003du);goto L8c019e34; /* cmp/eq #0x3d,r0 */
L8c019e34: if((*sr&1u))goto L8c019e46;goto L8c019e36; /* bt 0x8c019e46 */
L8c019e36: *stop=0x8c019e36u;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c019e38; /* mov.l @r6,r2 */
L8c019e38: *stop=0x8c019e38u;if(!valid_ram(r[4],4u))return -3;r[0]=rd(m,r[4],4);goto L8c019e3a; /* mov.l @r4,r0 */
L8c019e3a: *stop=0x8c019e3au;r[2]+=r[13];goto L8c019e3c; /* add r13,r2 */
L8c019e3c: *stop=0x8c019e3cu;r[3]=r[0];goto L8c019e3e; /* mov r0,r3 */
L8c019e3e: *stop=0x8c019e3eu;r[3]+=116u;goto L8c019e40; /* add #0x74,r3 */
L8c019e40: *stop=0x8c019e40u;if(!valid_ram(r[2],1u))return -3;r[1]=sx(m,r[2],1);goto L8c019e42; /* mov.b @r2,r1 */
L8c019e42: *stop=0x8c019e42u;r[3]+=r[5];goto L8c019e44; /* add r5,r3 */
L8c019e44: *stop=0x8c019e44u;if(!valid_ram(r[3],1u))return -3;wr(m,r[3],r[1],1);goto L8c019e46; /* mov.b r1,@r3 */
L8c019e46: *stop=0x8c019e46u;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c019e48; /* mov.l @r6,r2 */
L8c019e48: *stop=0x8c019e48u;r[7]=r[5];goto L8c019e4a; /* mov r5,r7 */
L8c019e4a: *stop=0x8c019e4au;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019e4c; /* mov.l @r4,r3 */
L8c019e4c: *stop=0x8c019e4cu;r[2]+=r[13];goto L8c019e4e; /* add r13,r2 */
L8c019e4e: *stop=0x8c019e4eu;if(!valid_ram(r[2]+2u,1u))return -3;r[0]=sx(m,r[2]+0x2u,1);goto L8c019e50; /* mov.b @(0x2,r2),r0 */
L8c019e50: *stop=0x8c019e50u;r[3]+=122u;goto L8c019e52; /* add #0x7a,r3 */
L8c019e52: *stop=0x8c019e52u;r[3]+=r[5];goto L8c019e54; /* add r5,r3 */
L8c019e54: *stop=0x8c019e54u;if(!valid_ram(r[3],1u))return -3;wr(m,r[3],r[0],1);goto L8c019e56; /* mov.b r0,@r3 */
L8c019e56: *stop=0x8c019e56u;if(!valid_ram(r[6],4u))return -3;r[0]=rd(m,r[6],4);goto L8c019e58; /* mov.l @r6,r0 */
L8c019e58: *stop=0x8c019e58u;if(!valid_ram(r[0]+r[13],1u))return -3;r[0]=sx(m,r[13]+r[0],1);goto L8c019e5a; /* mov.b @(r0,r13),r0 */
L8c019e5a: *stop=0x8c019e5au;*sr=(*sr&~1u)|(r[0]==0x0000003du);goto L8c019e5c; /* cmp/eq #0x3d,r0 */
L8c019e5c: take=(*sr&1u);*stop=0x8c019e5eu;r[7]<<=2;if(take)goto L8c019e76;goto L8c019e60; /* bt/s 0x8c019e76 */
L8c019e60: *stop=0x8c019e60u;if(!valid_ram(r[14],1u))return -3;r[2]=sx(m,r[14],1);goto L8c019e62; /* mov.b @r14,r2 */
L8c019e62: *stop=0x8c019e62u;r[0]=rd(m,0x8c019f24u,4);goto L8c019e64; /* mov.l 0x8c019f24,r0 */
L8c019e64: *stop=0x8c019e64u;r[2]<<=2;goto L8c019e66; /* shll2 r2 */
L8c019e66: *stop=0x8c019e66u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019e68; /* mov.l @r4,r3 */
L8c019e68: *stop=0x8c019e68u;if(!valid_ram(r[0],4u))return -3;r[1]=rd(m,r[0],4);goto L8c019e6a; /* mov.l @r0,r1 */
L8c019e6a: *stop=0x8c019e6au;r[2]<<=2;goto L8c019e6c; /* shll2 r2 */
L8c019e6c: *stop=0x8c019e6cu;r[3]+=68u;goto L8c019e6e; /* add #0x44,r3 */
L8c019e6e: *stop=0x8c019e6eu;r[2]+=r[1];goto L8c019e70; /* add r1,r2 */
L8c019e70: *stop=0x8c019e70u;if(!valid_ram(r[2],4u))return -3;r[2]=rd(m,r[2],4);goto L8c019e72; /* mov.l @r2,r2 */
L8c019e72: *stop=0x8c019e72u;r[3]+=r[7];goto L8c019e74; /* add r7,r3 */
L8c019e74: *stop=0x8c019e74u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto L8c019e76; /* mov.l r2,@r3 */
L8c019e76: *stop=0x8c019e76u;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c019e78; /* mov.l @r6,r2 */
L8c019e78: *stop=0x8c019e78u;r[5]=6u;goto L8c019e7a; /* mov #0x6,r5 */
L8c019e7a: *stop=0x8c019e7au;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c019e7c; /* mov.l @r4,r3 */
L8c019e7c: *stop=0x8c019e7cu;r[4]=8u;goto L8c019e7e; /* mov #0x8,r4 */
L8c019e7e: *stop=0x8c019e7eu;r[2]+=r[13];goto L8c019e80; /* add r13,r2 */
L8c019e80: *stop=0x8c019e80u;r[6]=rd(m,0x8c019f20u,4);goto L8c019e82; /* mov.l 0x8c019f20,r6 */
L8c019e82: *stop=0x8c019e82u;if(!valid_ram(r[2]+2u,1u))return -3;r[0]=sx(m,r[2]+0x2u,1);goto L8c019e84; /* mov.b @(0x2,r2),r0 */
L8c019e84: *stop=0x8c019e84u;r[3]+=92u;goto L8c019e86; /* add #0x5c,r3 */
L8c019e86: *stop=0x8c019e86u;r[2]=rd(m,0x8c019f30u,4);goto L8c019e88; /* mov.l 0x8c019f30,r2 */
L8c019e88: *stop=0x8c019e88u;r[7]+=r[3];goto L8c019e8a; /* add r3,r7 */
L8c019e8a: *stop=0x8c019e8au;r[0]=r[0]&255u;goto L8c019e8c; /* extu.b r0,r0 */
L8c019e8c: *stop=0x8c019e8cu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c019e8e; /* mov.l @r2,r3 */
L8c019e8e: *stop=0x8c019e8eu;r[0]<<=2;goto L8c019e90; /* shll2 r0 */
L8c019e90: *stop=0x8c019e90u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c019e92; /* shll r0 */
L8c019e92: *stop=0x8c019e92u;r[0]+=r[3];goto L8c019e94; /* add r3,r0 */
L8c019e94: *stop=0x8c019e94u;if(!valid_ram(r[0]+4u,4u))return -3;r[1]=rd(m,r[0]+0x4u,4);goto L8c019e96; /* mov.l @(0x4,r0),r1 */
L8c019e96: *stop=0x8c019e96u;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[1],4);goto L8c019e98; /* mov.l r1,@r7 */
L8c019e98: *stop=0x8c019e98u;r[15]+=4u;goto L8c019e9a; /* add #0x4,r15 */
L8c019e9a: *stop=0x8c019e9au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019e9c; /* lds.l @r15+,PR */
L8c019e9c: *stop=0x8c019e9cu;r[3]=rd(m,0x8c019f34u,4);goto L8c019e9e; /* mov.l 0x8c019f34,r3 */
L8c019e9e: *stop=0x8c019e9eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c019ea0; /* mov.l @r15+,r13 */
L8c019ea0: target=r[3];*stop=0x8c019ea2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c019ea4: *stop=0x8c019ea4u;r[3]=rd(m,0x8c019f38u,4);goto L8c019ea6; /* mov.l 0x8c019f38,r3 */
L8c019ea6: *stop=0x8c019ea6u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c019ea8; /* mov.l @r3,r1 */
L8c019ea8: *stop=0x8c019ea8u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c019eaa; /* tst r1,r1 */
L8c019eaa: if((*sr&1u))goto L8c019f40;goto L8c019eac; /* bt 0x8c019f40 */
L8c019eac: *stop=0x8c019eacu;r[0]=rd(m,0x8c019f28u,4);goto L8c019eae; /* mov.l 0x8c019f28,r0 */
L8c019eae: *stop=0x8c019eaeu;if(!valid_ram(r[0],4u))return -3;r[1]=rd(m,r[0],4);goto L8c019eb0; /* mov.l @r0,r1 */
L8c019eb0: *stop=0x8c019eb0u;if(!valid_ram(r[1]+4u,2u))return -3;r[0]=sx(m,r[1]+0x4u,2);goto L8c019eb2; /* mov.w @(0x4,r1),r0 */
L8c019eb2: *stop=0x8c019eb2u;r[1]=rd(m,0x8c019f2cu,4);goto L8c019eb4; /* mov.l 0x8c019f2c,r1 */
L8c019eb4: *stop=0x8c019eb4u;r[0]<<=2;goto L8c019eb6; /* shll2 r0 */
L8c019eb6: *stop=0x8c019eb6u;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c019eb8; /* mov.l @r1,r2 */
L8c019eb8: *stop=0x8c019eb8u;r[0]<<=2;goto L8c019eba; /* shll2 r0 */
L8c019eba: *stop=0x8c019ebau;r[0]<<=2;goto L8c019ebc; /* shll2 r0 */
L8c019ebc: *stop=0x8c019ebcu;r[0]+=r[2];goto L8c019ebe; /* add r2,r0 */
L8c019ebe: *stop=0x8c019ebeu;if(!valid_ram(r[0]+1u,1u))return -3;r[0]=sx(m,r[0]+0x1u,1);goto L8c019ec0; /* mov.b @(0x1,r0),r0 */
L8c019ec0: *stop=0x8c019ec0u;r[3]=r[0];goto L8c019ec2; /* mov r0,r3 */
L8c019ec2: *stop=0x8c019ec2u;if(!valid_ram(r[14]+1u,1u))return -3;r[0]=sx(m,r[14]+0x1u,1);goto L8c019ec4; /* mov.b @(0x1,r14),r0 */
L8c019ec4: *stop=0x8c019ec4u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c019ec6; /* cmp/ge r0,r3 */
L8c019ec6: if((*sr&1u))goto L8c019f40;goto L8c019ec8; /* bt 0x8c019f40 */
L8c019ec8: *stop=0x8c019ec8u;if(!valid_ram(r[14]+3u,1u))return -3;r[0]=sx(m,r[14]+0x3u,1);goto L8c019eca; /* mov.b @(0x3,r14),r0 */
L8c019eca: *stop=0x8c019ecau;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c019ecc; /* cmp/eq #0x0,r0 */
L8c019ecc: if((*sr&1u))goto L8c019ed6;goto L8c019ece; /* bt 0x8c019ed6 */
L8c019ece: *stop=0x8c019eceu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c019ed0; /* cmp/eq #0x2,r0 */
L8c019ed0: if((*sr&1u))goto L8c019ede;goto L8c019ed2; /* bt 0x8c019ede */
L8c019ed2: *stop=0x8c019ed4u;goto L8c019ee6; /* bra 0x8c019ee6 */
L8c019ed6: target=0x8c01a1acu;*pr=0x8c019edau;*stop=0x8c019ed8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019eda; /* bsr 0x8c01a1ac */
L8c019eda: *stop=0x8c019edcu;goto L8c019ee2; /* bra 0x8c019ee2 */
L8c019ede: target=0x8c01a480u;*pr=0x8c019ee2u;*stop=0x8c019ee0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019ee2; /* bsr 0x8c01a480 */
L8c019ee2: *stop=0x8c019ee4u;r[4]=r[0];goto L8c019ee8; /* bra 0x8c019ee8 */
L8c019ee6: *stop=0x8c019ee6u;r[4]=r[13];goto L8c019ee8; /* mov r13,r4 */
L8c019ee8: *stop=0x8c019ee8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c019eea; /* tst r4,r4 */
L8c019eea: if((*sr&1u))goto L8c019f40;goto L8c019eec; /* bt 0x8c019f40 */
L8c019eec: *stop=0x8c019eecu;r[2]=rd(m,0x8c019f3cu,4);goto L8c019eee; /* mov.l 0x8c019f3c,r2 */
L8c019eee: target=r[2];*pr=0x8c019ef2u;*stop=0x8c019ef0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019ef2; /* jsr @r2 */
L8c019ef2: *stop=0x8c019ef2u;if(!valid_ram(r[15],2u))return -3;r[5]=sx(m,r[15],2);goto L8c019ef4; /* mov.w @r15,r5 */
L8c019ef4: target=0x8c01a7d6u;*pr=0x8c019ef8u;*stop=0x8c019ef6u;if(!valid_ram(r[14],1u))return -3;r[4]=sx(m,r[14],1);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c019ef8; /* bsr 0x8c01a7d6 */
L8c019ef8: *stop=0x8c019ef8u;r[6]=rd(m,0x8c019f20u,4);goto L8c019efa; /* mov.l 0x8c019f20,r6 */
L8c019efa: *stop=0x8c019efau;r[4]=8u;goto L8c019efc; /* mov #0x8,r4 */
L8c019efc: *stop=0x8c019efcu;r[5]=6u;goto L8c019efe; /* mov #0x6,r5 */
L8c019efe: *stop=0x8c019efeu;r[15]+=4u;goto L8c019f00; /* add #0x4,r15 */
L8c019f00: *stop=0x8c019f00u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019f02; /* lds.l @r15+,PR */
L8c019f02: *stop=0x8c019f02u;r[2]=rd(m,0x8c019f34u,4);goto L8c019f04; /* mov.l 0x8c019f34,r2 */
L8c019f04: *stop=0x8c019f04u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c019f06; /* mov.l @r15+,r13 */
L8c019f06: target=r[2];*stop=0x8c019f08u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c019f40: *stop=0x8c019f40u;r[15]+=4u;goto L8c019f42; /* add #0x4,r15 */
L8c019f42: *stop=0x8c019f42u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c019f44; /* lds.l @r15+,PR */
L8c019f44: *stop=0x8c019f44u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c019f46; /* mov.l @r15+,r13 */
L8c019f46: *stop=0x8c019f46u;if(*pr!=entry_pr)return -3;*stop=0x8c019f48u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
