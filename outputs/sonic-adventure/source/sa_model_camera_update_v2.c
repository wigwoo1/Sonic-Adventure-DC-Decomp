/* Reached original camera preset and shared render setters; names inferred.
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
SA_STAGE_EXPORT int sa_camera_update_8c01e6a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c01e6a6;
L8c01e6a6: *stop=0x8c01e6a6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01e6a8; /* mov.l r14,@-r15 */
L8c01e6a8: *stop=0x8c01e6a8u;r[0]=32u;goto L8c01e6aa; /* mov #0x20,r0 */
L8c01e6aa: *stop=0x8c01e6aau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c01e6ac; /* mov.l r13,@-r15 */
L8c01e6ac: *stop=0x8c01e6acu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c01e6ae; /* mov.l r12,@-r15 */
L8c01e6ae: *stop=0x8c01e6aeu;r[12]=rd(m,0x8c01e7c8u,4);goto L8c01e6b0; /* mov.l 0x8c01e7c8,r12 */
L8c01e6b0: *stop=0x8c01e6b0u;r[14]=rd(m,0x8c01e7c4u,4);goto L8c01e6b2; /* mov.l 0x8c01e7c4,r14 */
L8c01e6b2: *stop=0x8c01e6b2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c01e6b4; /* mov.l r8,@-r15 */
L8c01e6b4: *stop=0x8c01e6b4u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[15]);goto L8c01e6b6; /* fmov.s fr12,@-r15 */
L8c01e6b6: *stop=0x8c01e6b6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01e6b8; /* sts.l PR,@-r15 */
L8c01e6b8: *stop=0x8c01e6b8u;if(!valid_ram(r[12],4u))return -3;r[2]=rd(m,r[12],4);goto L8c01e6ba; /* mov.l @r12,r2 */
L8c01e6ba: *stop=0x8c01e6bau;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01e6bc; /* mov.l @r14,r3 */
L8c01e6bc: *stop=0x8c01e6bcu;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[2]);goto L8c01e6be; /* fmov.s @(r0,r2),fr3 */
L8c01e6be: *stop=0x8c01e6beu;r[0]=24u;goto L8c01e6c0; /* mov #0x18,r0 */
L8c01e6c0: *stop=0x8c01e6c0u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[3]);goto L8c01e6c2; /* fmov fr3,@(r0,r3) */
L8c01e6c2: *stop=0x8c01e6c2u;r[0]=36u;goto L8c01e6c4; /* mov #0x24,r0 */
L8c01e6c4: *stop=0x8c01e6c4u;if(!valid_ram(r[12],4u))return -3;r[2]=rd(m,r[12],4);goto L8c01e6c6; /* mov.l @r12,r2 */
L8c01e6c6: *stop=0x8c01e6c6u;if(!valid_ram(r[14],4u))return -3;r[8]=rd(m,r[14],4);goto L8c01e6c8; /* mov.l @r14,r8 */
L8c01e6c8: *stop=0x8c01e6c8u;r[3]=rd(m,0x8c01e7ccu,4);goto L8c01e6ca; /* mov.l 0x8c01e7cc,r3 */
L8c01e6ca: *stop=0x8c01e6cau;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]+r[2]);goto L8c01e6cc; /* fmov.s @(r0,r2),fr12 */
L8c01e6cc: *stop=0x8c01e6ccu;r[8]+=28u;goto L8c01e6ce; /* add #0x1c,r8 */
L8c01e6ce: target=r[3];*pr=0x8c01e6d2u;*stop=0x8c01e6d0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01e6d2; /* jsr @r3 */
L8c01e6d2: *stop=0x8c01e6d2u;r[1]=70u;goto L8c01e6d4; /* mov #0x46,r1 */
L8c01e6d4: *stop=0x8c01e6d4u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c01e6d6; /* mov.l @r14,r2 */
L8c01e6d6: *stop=0x8c01e6d6u;r[1]<<=2;goto L8c01e6d8; /* shll2 r1 */
L8c01e6d8: *stop=0x8c01e6d8u;r[4]=rd(m,0x8c01e7d4u,4);goto L8c01e6da; /* mov.l 0x8c01e7d4,r4 */
L8c01e6da: *stop=0x8c01e6dau;if(!valid_ram(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[1]);goto L8c01e6dc; /* fmov.s @(r0,r1),fr0 */
L8c01e6dc: *stop=0x8c01e6dcu;r[0]=0x8c01e7d0u;goto L8c01e6de; /* mova 0x8c01e7d0,r0 */
L8c01e6de: *stop=0x8c01e6deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[0],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c01e6e0; /* fadd fr0,fr12 */
L8c01e6e0: *stop=0x8c01e6e0u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]);goto L8c01e6e2; /* fmov.s @r0,fr0 */
L8c01e6e2: *stop=0x8c01e6e2u;r[0]=8u;goto L8c01e6e4; /* mov #0x8,r0 */
L8c01e6e4: *stop=0x8c01e6e4u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[2]);goto L8c01e6e6; /* fmov.s @(r0,r2),fr3 */
L8c01e6e6: *stop=0x8c01e6e6u;r[0]=40u;goto L8c01e6e8; /* mov #0x28,r0 */
L8c01e6e8: *stop=0x8c01e6e8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c01e6ea; /* fmac fr0,fr3,fr12 */
L8c01e6ea: *stop=0x8c01e6eau;if(!valid_ram(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,12u,r[8]);goto L8c01e6ec; /* fmov.s fr12,@r8 */
L8c01e6ec: *stop=0x8c01e6ecu;if(!valid_ram(r[12],4u))return -3;r[2]=rd(m,r[12],4);goto L8c01e6ee; /* mov.l @r12,r2 */
L8c01e6ee: *stop=0x8c01e6eeu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01e6f0; /* mov.l @r14,r3 */
L8c01e6f0: *stop=0x8c01e6f0u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[2]);goto L8c01e6f2; /* fmov.s @(r0,r2),fr3 */
L8c01e6f2: *stop=0x8c01e6f2u;r[0]=32u;goto L8c01e6f4; /* mov #0x20,r0 */
L8c01e6f4: *stop=0x8c01e6f4u;r[2]=4294967288u;goto L8c01e6f6; /* mov #-0x8,r2 */
L8c01e6f6: *stop=0x8c01e6f6u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[3]);goto L8c01e6f8; /* fmov fr3,@(r0,r3) */
L8c01e6f8: *stop=0x8c01e6f8u;r[0]=8u;goto L8c01e6fa; /* mov #0x8,r0 */
L8c01e6fa: *stop=0x8c01e6fau;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto L8c01e6fc; /* fmov.s @r4,fr3 */
L8c01e6fc: *stop=0x8c01e6fcu;r[2]<<=8;goto L8c01e6fe; /* shll8 r2 */
L8c01e6fe: *stop=0x8c01e6feu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01e700; /* mov.l @r14,r3 */
L8c01e700: *stop=0x8c01e700u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[3]);goto L8c01e702; /* fmov fr3,@(r0,r3) */
L8c01e702: *stop=0x8c01e702u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01e704; /* mov.l @r14,r3 */
L8c01e704: *stop=0x8c01e704u;if(!valid_ram(r[3]+36u,4u))return -3;wr(m,r[3]+0x24u,r[2],4);goto L8c01e706; /* mov.l r2,@(0x24,r3) */
L8c01e706: *stop=0x8c01e706u;r[2]=0u;goto L8c01e708; /* mov #0x0,r2 */
L8c01e708: *stop=0x8c01e708u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01e70a; /* mov.l @r14,r3 */
L8c01e70a: *stop=0x8c01e70au;if(!valid_ram(r[3]+44u,4u))return -3;wr(m,r[3]+0x2cu,r[2],4);goto L8c01e70c; /* mov.l r2,@(0x2c,r3) */
L8c01e70c: *stop=0x8c01e70cu;if(!valid_ram(r[12],4u))return -3;r[5]=rd(m,r[12],4);goto L8c01e70e; /* mov.l @r12,r5 */
L8c01e70e: *stop=0x8c01e70eu;if(!valid_ram(r[5]+9u,1u))return -3;r[0]=sx(m,r[5]+0x9u,1);goto L8c01e710; /* mov.b @(0x9,r5),r0 */
L8c01e710: *stop=0x8c01e710u;r[0]=r[0]&255u;goto L8c01e712; /* extu.b r0,r0 */
L8c01e712: *stop=0x8c01e712u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c01e714; /* cmp/eq #0x3,r0 */
L8c01e714: take=!(*sr&1u);*stop=0x8c01e716u;if(!valid_ram(r[14],4u))return -3;r[13]=rd(m,r[14],4);if(take)goto L8c01e73c;goto L8c01e718; /* bf/s 0x8c01e73c */
L8c01e718: *stop=0x8c01e718u;if(!valid_ram(r[5],1u))return -3;r[0]=sx(m,r[5],1);goto L8c01e71a; /* mov.b @r5,r0 */
L8c01e71a: *stop=0x8c01e71au;*sr=(*sr&~1u)|(r[0]==0x0000000du);goto L8c01e71c; /* cmp/eq #0xd,r0 */
L8c01e71c: if(!(*sr&1u))goto L8c01e73c;goto L8c01e71e; /* bf 0x8c01e73c */
L8c01e71e: *stop=0x8c01e71eu;r[5]=rd(m,0x8c01e7c8u,4);goto L8c01e720; /* mov.l 0x8c01e7c8,r5 */
L8c01e720: *stop=0x8c01e720u;r[3]=rd(m,0x8c01e7d8u,4);goto L8c01e722; /* mov.l 0x8c01e7d8,r3 */
L8c01e722: *stop=0x8c01e722u;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c01e724; /* mov.l @r5,r5 */
L8c01e724: *stop=0x8c01e724u;r[2]=rd(m,0x8c01e7dcu,4);goto L8c01e726; /* mov.l 0x8c01e7dc,r2 */
L8c01e726: *stop=0x8c01e726u;if(!valid_ram(r[5]+24u,4u))return -3;r[5]=rd(m,r[5]+0x18u,4);goto L8c01e728; /* mov.l @(0x18,r5),r5 */
L8c01e728: *stop=0x8c01e728u;r[5]=0u-r[5];goto L8c01e72a; /* neg r5,r5 */
L8c01e72a: *stop=0x8c01e72au;r[5]+=r[3];goto L8c01e72c; /* add r3,r5 */
L8c01e72c: target=r[2];*pr=0x8c01e730u;*stop=0x8c01e72eu;if(!valid_ram(r[13]+40u,4u))return -3;r[4]=rd(m,r[13]+0x28u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01e730; /* jsr @r2 */
L8c01e730: *stop=0x8c01e730u;if(!valid_ram(r[13]+40u,4u))return -3;r[2]=rd(m,r[13]+0x28u,4);goto L8c01e732; /* mov.l @(0x28,r13),r2 */
L8c01e732: *stop=0x8c01e732u;*sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]&0x80000000u);goto L8c01e734; /* shar r0 */
L8c01e734: *stop=0x8c01e734u;*sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]&0x80000000u);goto L8c01e736; /* shar r0 */
L8c01e736: *stop=0x8c01e736u;r[2]+=r[0];goto L8c01e738; /* add r0,r2 */
L8c01e738: *stop=0x8c01e73au;if(!valid_ram(r[13]+40u,4u))return -3;wr(m,r[13]+0x28u,r[2],4);goto L8c01e796; /* bra 0x8c01e796 */
L8c01e73c: *stop=0x8c01e73cu;r[5]=rd(m,0x8c01e7e8u,4);goto L8c01e73e; /* mov.l 0x8c01e7e8,r5 */
L8c01e73e: *stop=0x8c01e73eu;r[12]=2u;goto L8c01e740; /* mov #0x2,r12 */
L8c01e740: *stop=0x8c01e740u;r[6]=rd(m,0x8c01e7e4u,4);goto L8c01e742; /* mov.l 0x8c01e7e4,r6 */
L8c01e742: *stop=0x8c01e742u;r[12]<<=8;goto L8c01e744; /* shll8 r12 */
L8c01e744: *stop=0x8c01e744u;if(!valid_ram(r[5]+8u,4u))return -3;r[2]=rd(m,r[5]+0x8u,4);goto L8c01e746; /* mov.l @(0x8,r5),r2 */
L8c01e746: *stop=0x8c01e746u;r[7]=rd(m,0x8c01e7e0u,4);goto L8c01e748; /* mov.l 0x8c01e7e0,r7 */
L8c01e748: *stop=0x8c01e748u;*sr=(*sr&~1u)|!(r[12]&r[2]);goto L8c01e74a; /* tst r12,r2 */
L8c01e74a: if((*sr&1u))goto L8c01e76c;goto L8c01e74c; /* bt 0x8c01e76c */
L8c01e74c: *stop=0x8c01e74cu;if(!valid_ram(r[5]+8u,4u))return -3;r[3]=rd(m,r[5]+0x8u,4);goto L8c01e74e; /* mov.l @(0x8,r5),r3 */
L8c01e74e: *stop=0x8c01e74eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c01e750; /* fldi1 fr4 */
L8c01e750: *stop=0x8c01e750u;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c01e752; /* tst r3,r6 */
L8c01e752: take=(*sr&1u);*stop=0x8c01e754u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[4],&f->fpscr,&fr[4],0);if(status)return status;}if(take)goto L8c01e75e;goto L8c01e756; /* bt/s 0x8c01e75e */
L8c01e756: *stop=0x8c01e756u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto L8c01e758; /* fmov.s @r4,fr3 */
L8c01e758: *stop=0x8c01e758u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c01e75a; /* fadd fr4,fr3 */
L8c01e75a: *stop=0x8c01e75cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[4]);goto L8c01e796; /* bra 0x8c01e796 */
L8c01e75e: *stop=0x8c01e75eu;if(!valid_ram(r[5]+8u,4u))return -3;r[2]=rd(m,r[5]+0x8u,4);goto L8c01e760; /* mov.l @(0x8,r5),r2 */
L8c01e760: *stop=0x8c01e760u;*sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c01e762; /* tst r2,r7 */
L8c01e762: if((*sr&1u))goto L8c01e796;goto L8c01e764; /* bt 0x8c01e796 */
L8c01e764: *stop=0x8c01e764u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto L8c01e766; /* fmov.s @r4,fr3 */
L8c01e766: *stop=0x8c01e766u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c01e768; /* fsub fr4,fr3 */
L8c01e768: *stop=0x8c01e76au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[4]);goto L8c01e796; /* bra 0x8c01e796 */
L8c01e76c: *stop=0x8c01e76cu;*sr=(*sr&~1u)|!(r[2]&r[6]);goto L8c01e76e; /* tst r2,r6 */
L8c01e76e: if((*sr&1u))goto L8c01e782;goto L8c01e770; /* bt 0x8c01e782 */
L8c01e770: *stop=0x8c01e770u;if(!valid_ram(r[13]+6u,2u))return -3;r[0]=sx(m,r[13]+0x6u,2);goto L8c01e772; /* mov.w @(0x6,r13),r0 */
L8c01e772: *stop=0x8c01e772u;r[0]|=0x1u;goto L8c01e774; /* or #0x1,r0 */
L8c01e774: *stop=0x8c01e774u;if(!valid_ram(r[13]+6u,2u))return -3;wr(m,r[13]+0x6u,r[0],2);goto L8c01e776; /* mov.w r0,@(0x6,r13) */
L8c01e776: *stop=0x8c01e776u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01e778; /* mov.l @r14,r3 */
L8c01e778: *stop=0x8c01e778u;if(!valid_ram(r[3]+40u,4u))return -3;r[2]=rd(m,r[3]+0x28u,4);goto L8c01e77a; /* mov.l @(0x28,r3),r2 */
L8c01e77a: *stop=0x8c01e77au;r[2]-=r[12];goto L8c01e77c; /* sub r12,r2 */
L8c01e77c: *stop=0x8c01e77cu;if(!valid_ram(r[3]+40u,4u))return -3;wr(m,r[3]+0x28u,r[2],4);goto L8c01e77e; /* mov.l r2,@(0x28,r3) */
L8c01e77e: *stop=0x8c01e780u;goto L8c01e796; /* bra 0x8c01e796 */
L8c01e782: *stop=0x8c01e782u;if(!valid_ram(r[5]+8u,4u))return -3;r[1]=rd(m,r[5]+0x8u,4);goto L8c01e784; /* mov.l @(0x8,r5),r1 */
L8c01e784: *stop=0x8c01e784u;*sr=(*sr&~1u)|!(r[1]&r[7]);goto L8c01e786; /* tst r1,r7 */
L8c01e786: if((*sr&1u))goto L8c01e796;goto L8c01e788; /* bt 0x8c01e796 */
L8c01e788: *stop=0x8c01e788u;if(!valid_ram(r[13]+6u,2u))return -3;r[0]=sx(m,r[13]+0x6u,2);goto L8c01e78a; /* mov.w @(0x6,r13),r0 */
L8c01e78a: *stop=0x8c01e78au;r[0]|=0x1u;goto L8c01e78c; /* or #0x1,r0 */
L8c01e78c: *stop=0x8c01e78cu;if(!valid_ram(r[13]+6u,2u))return -3;wr(m,r[13]+0x6u,r[0],2);goto L8c01e78e; /* mov.w r0,@(0x6,r13) */
L8c01e78e: *stop=0x8c01e78eu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01e790; /* mov.l @r14,r3 */
L8c01e790: *stop=0x8c01e790u;if(!valid_ram(r[3]+40u,4u))return -3;r[2]=rd(m,r[3]+0x28u,4);goto L8c01e792; /* mov.l @(0x28,r3),r2 */
L8c01e792: *stop=0x8c01e792u;r[2]+=r[12];goto L8c01e794; /* add r12,r2 */
L8c01e794: *stop=0x8c01e794u;if(!valid_ram(r[3]+40u,4u))return -3;wr(m,r[3]+0x28u,r[2],4);goto L8c01e796; /* mov.l r2,@(0x28,r3) */
L8c01e796: *stop=0x8c01e796u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01e798; /* lds.l @r15+,PR */
L8c01e798: *stop=0x8c01e798u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01e79a; /* fmov.s @r15+,fr12 */
L8c01e79a: *stop=0x8c01e79au;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c01e79c; /* mov.l @r15+,r8 */
L8c01e79c: *stop=0x8c01e79cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c01e79e; /* mov.l @r15+,r12 */
L8c01e79e: *stop=0x8c01e79eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01e7a0; /* mov.l @r15+,r13 */
L8c01e7a0: *stop=0x8c01e7a0u;if(*pr!=entry_pr)return -3;*stop=0x8c01e7a2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_update_8c038bb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c038bb8;
L8c038bb8: *stop=0x8c038bb8u;r[1]=rd(m,0x8c038bccu,4);goto L8c038bba; /* mov.l 0x8c038bcc,r1 */
L8c038bba: *stop=0x8c038bbau;r[2]=rd(m,0x8c038bd0u,4);goto L8c038bbc; /* mov.l 0x8c038bd0,r2 */
L8c038bbc: *stop=0x8c038bbcu;if(!valid_ram(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[1]);goto L8c038bbe; /* fmov.s fr6,@-r1 */
L8c038bbe: *stop=0x8c038bbeu;if(!valid_ram(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[1]);goto L8c038bc0; /* fmov.s fr5,@-r1 */
L8c038bc0: *stop=0x8c038bc0u;if(!valid_ram(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[1]);goto L8c038bc2; /* fmov.s fr4,@-r1 */
L8c038bc2: *stop=0x8c038bc2u;if(!valid_ram(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[2]);goto L8c038bc4; /* fmov.s fr6,@-r2 */
L8c038bc4: *stop=0x8c038bc4u;if(!valid_ram(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,5u,r[2]);goto L8c038bc6; /* fmov.s fr5,@-r2 */
L8c038bc6: *stop=0x8c038bc6u;if(*pr!=entry_pr)return -3;*stop=0x8c038bc8u;if(!valid_ram(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,4u,r[2]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_update_8c610cf8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c610cf8;
L8c610cf8: *stop=0x8c610cf8u;r[5]=rd(m,0x8c610d68u,4);goto L8c610cfa; /* mov.l 0x8c610d68,r5 */
L8c610cfa: *stop=0x8c610cfau;r[0]=4u;goto L8c610cfc; /* mov #0x4,r0 */
L8c610cfc: *stop=0x8c610cfcu;r[3]=rd(m,0x8c610d6cu,4);goto L8c610cfe; /* mov.l 0x8c610d6c,r3 */
L8c610cfe: *stop=0x8c610cfeu;r[4]=rd(m,0x8c610d64u,4);goto L8c610d00; /* mov.l 0x8c610d64,r4 */
L8c610d00: *stop=0x8c610d00u;r[2]=rd(m,0x8c610d70u,4);goto L8c610d02; /* mov.l 0x8c610d70,r2 */
L8c610d02: *stop=0x8c610d02u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[4]);goto L8c610d04; /* fmov.s fr4,@r4 */
L8c610d04: *stop=0x8c610d04u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[4]);goto L8c610d06; /* fmov fr5,@(r0,r4) */
L8c610d06: *stop=0x8c610d06u;r[0]=8u;goto L8c610d08; /* mov #0x8,r0 */
L8c610d08: *stop=0x8c610d08u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[4]);goto L8c610d0a; /* fmov fr6,@(r0,r4) */
L8c610d0a: *stop=0x8c610d0au;r[0]=36u;goto L8c610d0c; /* mov #0x24,r0 */
L8c610d0c: *stop=0x8c610d0cu;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[3]);goto L8c610d0e; /* fmov.s @r3,fr3 */
L8c610d0e: *stop=0x8c610d0eu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[5]);goto L8c610d10; /* fmov.s @r5,fr2 */
L8c610d10: *stop=0x8c610d10u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c610d12; /* fmul fr3,fr2 */
L8c610d12: *stop=0x8c610d12u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto L8c610d14; /* fmov fr2,@(r0,r4) */
L8c610d14: *stop=0x8c610d14u;r[0]=40u;goto L8c610d16; /* mov #0x28,r0 */
L8c610d16: *stop=0x8c610d16u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[5]);goto L8c610d18; /* fmov.s @r5,fr2 */
L8c610d18: *stop=0x8c610d18u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[2]);goto L8c610d1a; /* fmov.s @r2,fr3 */
L8c610d1a: *stop=0x8c610d1au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c610d1c; /* fmul fr3,fr2 */
L8c610d1c: *stop=0x8c610d1cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto L8c610d1e; /* fmov fr2,@(r0,r4) */
L8c610d1e: *stop=0x8c610d1eu;r[0]=12u;goto L8c610d20; /* mov #0xc,r0 */
L8c610d20: *stop=0x8c610d20u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto L8c610d22; /* fmov.s @(r0,r5),fr3 */
L8c610d22: *stop=0x8c610d22u;r[0]=44u;goto L8c610d24; /* mov #0x2c,r0 */
L8c610d24: *stop=0x8c610d24u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c610d26; /* fmov fr3,@(r0,r4) */
L8c610d26: *stop=0x8c610d26u;r[0]=16u;goto L8c610d28; /* mov #0x10,r0 */
L8c610d28: *stop=0x8c610d28u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto L8c610d2a; /* fmov.s @(r0,r5),fr3 */
L8c610d2a: *stop=0x8c610d2au;r[0]=48u;goto L8c610d2c; /* mov #0x30,r0 */
L8c610d2c: *stop=0x8c610d2cu;if(*pr!=entry_pr)return -3;*stop=0x8c610d2eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_update_8c610d30(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c610d30;
L8c610d30: *stop=0x8c610d30u;r[4]=rd(m,0x8c610d64u,4);goto L8c610d32; /* mov.l 0x8c610d64,r4 */
L8c610d32: *stop=0x8c610d32u;r[0]=12u;goto L8c610d34; /* mov #0xc,r0 */
L8c610d34: *stop=0x8c610d34u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c610d36; /* fneg fr4 */
L8c610d36: *stop=0x8c610d36u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto L8c610d38; /* fmov fr4,@(r0,r4) */
L8c610d38: *stop=0x8c610d38u;r[0]=32u;goto L8c610d3a; /* mov #0x20,r0 */
L8c610d3a: *stop=0x8c610d3au;if(*pr!=entry_pr)return -3;*stop=0x8c610d3cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_update_8c610d3e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c610d3e;
L8c610d3e: *stop=0x8c610d3eu;r[4]=rd(m,0x8c610d74u,4);goto L8c610d40; /* mov.l 0x8c610d74,r4 */
L8c610d40: *stop=0x8c610d40u;r[0]=4u;goto L8c610d42; /* mov #0x4,r0 */
L8c610d42: *stop=0x8c610d42u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto L8c610d44; /* fmov fr4,@(r0,r4) */
L8c610d44: *stop=0x8c610d44u;r[0]=8u;goto L8c610d46; /* mov #0x8,r0 */
L8c610d46: *stop=0x8c610d46u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[4]);goto L8c610d48; /* fmov fr5,@(r0,r4) */
L8c610d48: *stop=0x8c610d48u;r[0]=12u;goto L8c610d4a; /* mov #0xc,r0 */
L8c610d4a: *stop=0x8c610d4au;if(*pr!=entry_pr)return -3;*stop=0x8c610d4cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_update_8c63aee8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63aee8;
L8c63aee8: *stop=0x8c63aee8u;r[0]=sx(m,0x8c63af36u,2);goto L8c63aeea; /* mov.w 0x8c63af36,r0 */
L8c63aeea: *stop=0x8c63aeeau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c63aeec; /* mov.l r14,@-r15 */
L8c63aeec: *stop=0x8c63aeecu;r[14]=r[4];goto L8c63aeee; /* mov r4,r14 */
L8c63aeee: *stop=0x8c63aeeeu;r[5]=sx(m,0x8c63af3au,2);goto L8c63aef0; /* mov.w 0x8c63af3a,r5 */
L8c63aef0: *stop=0x8c63aef0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c63aef2; /* sts.l PR,@-r15 */
L8c63aef2: *stop=0x8c63aef2u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[14]);goto L8c63aef4; /* fmov fr4,@(r0,r14) */
L8c63aef4: *stop=0x8c63aef4u;r[0]+=4294967292u;goto L8c63aef6; /* add #-0x4,r0 */
L8c63aef6: *stop=0x8c63aef6u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[14]);goto L8c63aef8; /* fmov fr5,@(r0,r14) */
L8c63aef8: *stop=0x8c63aef8u;r[0]+=4294967292u;goto L8c63aefa; /* add #-0x4,r0 */
L8c63aefa: *stop=0x8c63aefau;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[14]);goto L8c63aefc; /* fmov fr6,@(r0,r14) */
L8c63aefc: *stop=0x8c63aefcu;r[5]+=r[14];goto L8c63aefe; /* add r14,r5 */
L8c63aefe: *stop=0x8c63aefeu;r[0]=sx(m,0x8c63af38u,2);goto L8c63af00; /* mov.w 0x8c63af38,r0 */
L8c63af00: *stop=0x8c63af00u;r[4]=sx(m,0x8c63af3cu,2);goto L8c63af02; /* mov.w 0x8c63af3c,r4 */
L8c63af02: *stop=0x8c63af02u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c63af04; /* fmov.s @(r0,r14),fr4 */
L8c63af04: target=0x8c63aec8u;*pr=0x8c63af08u;*stop=0x8c63af06u;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63af08; /* bsr 0x8c63aec8 */
L8c63af08: *stop=0x8c63af08u;r[0]=sx(m,0x8c63af36u,2);goto L8c63af0a; /* mov.w 0x8c63af36,r0 */
L8c63af0a: *stop=0x8c63af0au;r[5]=sx(m,0x8c63af3cu,2);goto L8c63af0c; /* mov.w 0x8c63af3c,r5 */
L8c63af0c: *stop=0x8c63af0cu;r[4]=sx(m,0x8c63af3eu,2);goto L8c63af0e; /* mov.w 0x8c63af3e,r4 */
L8c63af0e: *stop=0x8c63af0eu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c63af10; /* fmov.s @(r0,r14),fr4 */
L8c63af10: *stop=0x8c63af10u;r[5]+=r[14];goto L8c63af12; /* add r14,r5 */
L8c63af12: target=0x8c63aec8u;*pr=0x8c63af16u;*stop=0x8c63af14u;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63af16; /* bsr 0x8c63aec8 */
L8c63af16: *stop=0x8c63af16u;r[0]=sx(m,0x8c63af40u,2);goto L8c63af18; /* mov.w 0x8c63af40,r0 */
L8c63af18: *stop=0x8c63af18u;r[5]=sx(m,0x8c63af3cu,2);goto L8c63af1a; /* mov.w 0x8c63af3c,r5 */
L8c63af1a: *stop=0x8c63af1au;r[4]=sx(m,0x8c63af42u,2);goto L8c63af1c; /* mov.w 0x8c63af42,r4 */
L8c63af1c: *stop=0x8c63af1cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c63af1e; /* fmov.s @(r0,r14),fr4 */
L8c63af1e: *stop=0x8c63af1eu;r[5]+=r[14];goto L8c63af20; /* add r14,r5 */
L8c63af20: target=0x8c63aec8u;*pr=0x8c63af24u;*stop=0x8c63af22u;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63af24; /* bsr 0x8c63aec8 */
L8c63af24: *stop=0x8c63af24u;r[0]=sx(m,0x8c63af44u,2);goto L8c63af26; /* mov.w 0x8c63af44,r0 */
L8c63af26: *stop=0x8c63af26u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c63af28; /* lds.l @r15+,PR */
L8c63af28: *stop=0x8c63af28u;r[4]=sx(m,0x8c63af46u,2);goto L8c63af2a; /* mov.w 0x8c63af46,r4 */
L8c63af2a: *stop=0x8c63af2au;r[5]=sx(m,0x8c63af3cu,2);goto L8c63af2c; /* mov.w 0x8c63af3c,r5 */
L8c63af2c: *stop=0x8c63af2cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c63af2e; /* fmov.s @(r0,r14),fr4 */
L8c63af2e: *stop=0x8c63af2eu;r[4]+=r[14];goto L8c63af30; /* add r14,r4 */
L8c63af30: *stop=0x8c63af30u;r[5]+=r[14];goto L8c63af32; /* add r14,r5 */
L8c63af32: *stop=0x8c63af34u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0x8c63aec8u,m,f,sr,r,pr); /* bra 0x8c63aec8 */
}

SA_STAGE_EXPORT int sa_camera_update_8c63ae2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63ae2c;
L8c63ae2c: *stop=0x8c63ae2cu;r[0]=sx(m,0x8c63ae7au,2);goto L8c63ae2e; /* mov.w 0x8c63ae7a,r0 */
L8c63ae2e: *stop=0x8c63ae2eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c63ae30; /* mov.l r14,@-r15 */
L8c63ae30: *stop=0x8c63ae30u;r[14]=r[4];goto L8c63ae32; /* mov r4,r14 */
L8c63ae32: *stop=0x8c63ae32u;r[5]=sx(m,0x8c63ae7eu,2);goto L8c63ae34; /* mov.w 0x8c63ae7e,r5 */
L8c63ae34: *stop=0x8c63ae34u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c63ae36; /* sts.l PR,@-r15 */
L8c63ae36: *stop=0x8c63ae36u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[14]);goto L8c63ae38; /* fmov fr4,@(r0,r14) */
L8c63ae38: *stop=0x8c63ae38u;r[0]+=4u;goto L8c63ae3a; /* add #0x4,r0 */
L8c63ae3a: *stop=0x8c63ae3au;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[14]);goto L8c63ae3c; /* fmov fr5,@(r0,r14) */
L8c63ae3c: *stop=0x8c63ae3cu;r[0]+=4u;goto L8c63ae3e; /* add #0x4,r0 */
L8c63ae3e: *stop=0x8c63ae3eu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[14]);goto L8c63ae40; /* fmov fr6,@(r0,r14) */
L8c63ae40: *stop=0x8c63ae40u;r[5]+=r[14];goto L8c63ae42; /* add r14,r5 */
L8c63ae42: *stop=0x8c63ae42u;r[0]=sx(m,0x8c63ae7cu,2);goto L8c63ae44; /* mov.w 0x8c63ae7c,r0 */
L8c63ae44: *stop=0x8c63ae44u;r[4]=sx(m,0x8c63ae80u,2);goto L8c63ae46; /* mov.w 0x8c63ae80,r4 */
L8c63ae46: *stop=0x8c63ae46u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c63ae48; /* fmov.s @(r0,r14),fr4 */
L8c63ae48: target=0x8c63ae0cu;*pr=0x8c63ae4cu;*stop=0x8c63ae4au;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63ae4c; /* bsr 0x8c63ae0c */
L8c63ae4c: *stop=0x8c63ae4cu;r[0]=sx(m,0x8c63ae82u,2);goto L8c63ae4e; /* mov.w 0x8c63ae82,r0 */
L8c63ae4e: *stop=0x8c63ae4eu;r[5]=sx(m,0x8c63ae80u,2);goto L8c63ae50; /* mov.w 0x8c63ae80,r5 */
L8c63ae50: *stop=0x8c63ae50u;r[4]=sx(m,0x8c63ae84u,2);goto L8c63ae52; /* mov.w 0x8c63ae84,r4 */
L8c63ae52: *stop=0x8c63ae52u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c63ae54; /* fmov.s @(r0,r14),fr4 */
L8c63ae54: *stop=0x8c63ae54u;r[5]+=r[14];goto L8c63ae56; /* add r14,r5 */
L8c63ae56: target=0x8c63ae0cu;*pr=0x8c63ae5au;*stop=0x8c63ae58u;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63ae5a; /* bsr 0x8c63ae0c */
L8c63ae5a: *stop=0x8c63ae5au;r[0]=sx(m,0x8c63ae86u,2);goto L8c63ae5c; /* mov.w 0x8c63ae86,r0 */
L8c63ae5c: *stop=0x8c63ae5cu;r[5]=sx(m,0x8c63ae80u,2);goto L8c63ae5e; /* mov.w 0x8c63ae80,r5 */
L8c63ae5e: *stop=0x8c63ae5eu;r[4]=sx(m,0x8c63ae88u,2);goto L8c63ae60; /* mov.w 0x8c63ae88,r4 */
L8c63ae60: *stop=0x8c63ae60u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c63ae62; /* fmov.s @(r0,r14),fr4 */
L8c63ae62: *stop=0x8c63ae62u;r[5]+=r[14];goto L8c63ae64; /* add r14,r5 */
L8c63ae64: target=0x8c63ae0cu;*pr=0x8c63ae68u;*stop=0x8c63ae66u;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63ae68; /* bsr 0x8c63ae0c */
L8c63ae68: *stop=0x8c63ae68u;r[0]=sx(m,0x8c63ae8au,2);goto L8c63ae6a; /* mov.w 0x8c63ae8a,r0 */
L8c63ae6a: *stop=0x8c63ae6au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c63ae6c; /* lds.l @r15+,PR */
L8c63ae6c: *stop=0x8c63ae6cu;r[4]=sx(m,0x8c63ae8cu,2);goto L8c63ae6e; /* mov.w 0x8c63ae8c,r4 */
L8c63ae6e: *stop=0x8c63ae6eu;r[5]=sx(m,0x8c63ae80u,2);goto L8c63ae70; /* mov.w 0x8c63ae80,r5 */
L8c63ae70: *stop=0x8c63ae70u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto L8c63ae72; /* fmov.s @(r0,r14),fr4 */
L8c63ae72: *stop=0x8c63ae72u;r[4]+=r[14];goto L8c63ae74; /* add r14,r4 */
L8c63ae74: *stop=0x8c63ae74u;r[5]+=r[14];goto L8c63ae76; /* add r14,r5 */
L8c63ae76: *stop=0x8c63ae78u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0x8c63ae0cu,m,f,sr,r,pr); /* bra 0x8c63ae0c */
}

SA_STAGE_EXPORT int sa_camera_update_8c63adc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63adc0;
L8c63adc0: *stop=0x8c63adc0u;r[0]=sx(m,0x8c63adf8u,2);goto L8c63adc2; /* mov.w 0x8c63adf8,r0 */
L8c63adc2: *stop=0x8c63adc2u;if(!valid_ram(r[0]+r[4],4u))return -3;wr(m,r[4]+r[0],r[5],4);goto L8c63adc4; /* mov.l r5,@(r0,r4) */
L8c63adc4: *stop=0x8c63adc4u;r[0]=sx(m,0x8c63adfau,2);goto L8c63adc6; /* mov.w 0x8c63adfa,r0 */
L8c63adc6: *stop=0x8c63adc6u;if(!valid_ram(r[0]+r[4],4u))return -3;wr(m,r[4]+r[0],r[6],4);goto L8c63adc8; /* mov.l r6,@(r0,r4) */
L8c63adc8: *stop=0x8c63adc8u;*fpul=r[5];goto L8c63adca; /* lds r5,FPUL */
L8c63adca: *stop=0x8c63adcau;status=sa_sh4_vector_numeric(0xf0fdu,f,*fpul);if(status)return status;goto L8c63adcc; /* fsca FPUL,dr0 */
L8c63adcc: *stop=0x8c63adccu;r[0]=sx(m,0x8c63adfcu,2);goto L8c63adce; /* mov.w 0x8c63adfc,r0 */
L8c63adce: *stop=0x8c63adceu;*fpul=r[6];goto L8c63add0; /* lds r6,FPUL */
L8c63add0: *stop=0x8c63add0u;status=sa_sh4_vector_numeric(0xf2fdu,f,*fpul);if(status)return status;goto L8c63add2; /* fsca FPUL,dr2 */
L8c63add2: *stop=0x8c63add2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63add4; /* fmul fr1,fr1 */
L8c63add4: *stop=0x8c63add4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63add6; /* fmul fr3,fr3 */
L8c63add6: *stop=0x8c63add6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto L8c63add8; /* fmov fr1,@(r0,r4) */
L8c63add8: *stop=0x8c63add8u;r[0]=sx(m,0x8c63adfeu,2);goto L8c63adda; /* mov.w 0x8c63adfe,r0 */
L8c63adda: *stop=0x8c63addau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c63addc; /* fmov fr3,@(r0,r4) */
L8c63addc: *stop=0x8c63addcu;r[0]=sx(m,0x8c63adf8u,2);goto L8c63adde; /* mov.w 0x8c63adf8,r0 */
L8c63adde: *stop=0x8c63addeu;if(!valid_ram(r[0]+r[4],4u))return -3;r[3]=rd(m,r[4]+r[0],4);goto L8c63ade0; /* mov.l @(r0,r4),r3 */
L8c63ade0: *stop=0x8c63ade0u;r[2]=rd(m,0x8c63ae04u,4);goto L8c63ade2; /* mov.l 0x8c63ae04,r2 */
L8c63ade2: *stop=0x8c63ade2u;r[0]=sx(m,0x8c63adfau,2);goto L8c63ade4; /* mov.w 0x8c63adfa,r0 */
L8c63ade4: *stop=0x8c63ade4u;r[3]+=r[2];goto L8c63ade6; /* add r2,r3 */
L8c63ade6: *stop=0x8c63ade6u;if(!valid_ram(r[0]+r[4],4u))return -3;r[5]=rd(m,r[4]+r[0],4);goto L8c63ade8; /* mov.l @(r0,r4),r5 */
L8c63ade8: *stop=0x8c63ade8u;r[3]=r[3]&65535u;goto L8c63adea; /* extu.w r3,r3 */
L8c63adea: *stop=0x8c63adeau;r[5]+=r[2];goto L8c63adec; /* add r2,r5 */
L8c63adec: *stop=0x8c63adecu;r[3]<<=16;goto L8c63adee; /* shll16 r3 */
L8c63adee: *stop=0x8c63adeeu;r[5]=r[5]&65535u;goto L8c63adf0; /* extu.w r5,r5 */
L8c63adf0: *stop=0x8c63adf0u;r[0]=sx(m,0x8c63ae00u,2);goto L8c63adf2; /* mov.w 0x8c63ae00,r0 */
L8c63adf2: *stop=0x8c63adf2u;r[3]|=r[5];goto L8c63adf4; /* or r5,r3 */
L8c63adf4: *stop=0x8c63adf4u;if(*pr!=entry_pr)return -3;*stop=0x8c63adf6u;if(!valid_ram(r[0]+r[4],4u))return -3;wr(m,r[4]+r[0],r[3],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_update_8c63ae90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63ae90;
L8c63ae90: *stop=0x8c63ae90u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c63ae92; /* mov.l r14,@-r15 */
L8c63ae92: *stop=0x8c63ae92u;r[0]=76u;goto L8c63ae94; /* mov #0x4c,r0 */
L8c63ae94: *stop=0x8c63ae94u;r[14]=r[4];goto L8c63ae96; /* mov r4,r14 */
L8c63ae96: *stop=0x8c63ae96u;r[3]=rd(m,0x8c63aec0u,4);goto L8c63ae98; /* mov.l 0x8c63aec0,r3 */
L8c63ae98: *stop=0x8c63ae98u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c63ae9a; /* sts.l PR,@-r15 */
L8c63ae9a: *stop=0x8c63ae9au;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[14]);goto L8c63ae9c; /* fmov fr4,@(r0,r14) */
L8c63ae9c: *stop=0x8c63ae9cu;r[0]=80u;goto L8c63ae9e; /* mov #0x50,r0 */
L8c63ae9e: *stop=0x8c63ae9eu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[14]);goto L8c63aea0; /* fmov fr5,@(r0,r14) */
L8c63aea0: *stop=0x8c63aea0u;r[0]=84u;goto L8c63aea2; /* mov #0x54,r0 */
L8c63aea2: *stop=0x8c63aea2u;r[4]=r[14];goto L8c63aea4; /* mov r14,r4 */
L8c63aea4: *stop=0x8c63aea4u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,6u,r[0]+r[14]);goto L8c63aea6; /* fmov fr6,@(r0,r14) */
L8c63aea6: target=r[3];*pr=0x8c63aeaau;*stop=0x8c63aea8u;r[4]+=76u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63aeaa; /* jsr @r3 */
L8c63aeaa: *stop=0x8c63aeaau;r[1]=sx(m,0x8c63aebeu,2);goto L8c63aeac; /* mov.w 0x8c63aebe,r1 */
L8c63aeac: *stop=0x8c63aeacu;r[2]=r[14];goto L8c63aeae; /* mov r14,r2 */
L8c63aeae: *stop=0x8c63aeaeu;r[3]=rd(m,0x8c63aec4u,4);goto L8c63aeb0; /* mov.l 0x8c63aec4,r3 */
L8c63aeb0: *stop=0x8c63aeb0u;r[2]+=76u;goto L8c63aeb2; /* add #0x4c,r2 */
L8c63aeb2: *stop=0x8c63aeb2u;r[1]+=r[14];goto L8c63aeb4; /* add r14,r1 */
L8c63aeb4: target=r[3];*pr=0x8c63aeb8u;*stop=0x8c63aeb6u;r[0]=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63aeb8; /* jsr @r3 */
L8c63aeb8: *stop=0x8c63aeb8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c63aeba; /* lds.l @r15+,PR */
L8c63aeba: *stop=0x8c63aebau;if(*pr!=entry_pr)return -3;*stop=0x8c63aebcu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_update_8c038c24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c038c24;
L8c038c24: *stop=0x8c038c24u;r[2]=rd(m,0x8c038c64u,4);goto L8c038c26; /* mov.l 0x8c038c64,r2 */
L8c038c26: *stop=0x8c038c26u;r[1]=rd(m,0x8c038c68u,4);goto L8c038c28; /* mov.l 0x8c038c68,r1 */
L8c038c28: *stop=0x8c038c28u;if(!valid_ram(r[1],4u))return -3;r[0]=rd(m,r[1],4);goto L8c038c2a; /* mov.l @r1,r0 */
L8c038c2a: *stop=0x8c038c2au;r[7]=0u;goto L8c038c2c; /* mov #0x0,r7 */
L8c038c2c: *stop=0x8c038c2cu;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[7],4);goto L8c038c2e; /* mov.l r7,@r1 */
L8c038c2e: *stop=0x8c038c2eu;if(!valid_ram(*gbr+0x8cu,4u))return -3;wr(m,*gbr+0x8cu,r[0],4);goto L8c038c30; /* mov.l r0,@(0x8c,GBR) */
L8c038c30: *stop=0x8c038c30u;r[7]=r[0];goto L8c038c32; /* mov r0,r7 */
L8c038c32: *stop=0x8c038c32u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c038c34; /* tst r7,r7 */
L8c038c34: if((*sr&1u))goto L8c038c60;goto L8c038c36; /* bt 0x8c038c60 */
L8c038c36: *stop=0x8c038c36u;r[0]=16u;goto L8c038c38; /* mov #0x10,r0 */
L8c038c38: *stop=0x8c038c38u;*sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[0]);goto L8c038c3a; /* cmp/gt r0,r7 */
L8c038c3a: if(!(*sr&1u))goto L8c038c3e;goto L8c038c3c; /* bf 0x8c038c3e */
L8c038c3c: *stop=0x8c038c3cu;r[7]=r[0];goto L8c038c3e; /* mov r0,r7 */
L8c038c3e: *stop=0x8c038c3eu;r[3]=rd(m,0x8c038c6cu,4);goto L8c038c40; /* mov.l 0x8c038c6c,r3 */
L8c038c40: *stop=0x8c038c40u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038c42; /* fmov.s @r2+,fr0 */
L8c038c42: *stop=0x8c038c42u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[2]);r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038c44; /* fmov.s @r2+,fr1 */
L8c038c44: *stop=0x8c038c44u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[2]);r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038c46; /* fmov.s @r2+,fr2 */
L8c038c46: *stop=0x8c038c46u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c038c48; /* fldi1 fr3 */
L8c038c48: *stop=0x8c038c48u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c038c4a; /* ftrv xmtrx,fv0 */
L8c038c4a: *stop=0x8c038c4au;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[2]);r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038c4c; /* fmov.s @r2+,fr3 */
L8c038c4c: *stop=0x8c038c4cu;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);r[2]+=4;goto L8c038c4e; /* mov.l @r2+,r0 */
L8c038c4e: *stop=0x8c038c4eu;r[2]+=4u;goto L8c038c50; /* add #0x4,r2 */
L8c038c50: *stop=0x8c038c50u;if(!valid_ram(r[3]-4u,4u))return -3;r[3]-=4;wr(m,r[3],r[0],4);goto L8c038c52; /* mov.l r0,@-r3 */
L8c038c52: *stop=0x8c038c52u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,3u,r[3]);goto L8c038c54; /* fmov.s fr3,@-r3 */
L8c038c54: *stop=0x8c038c54u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,2u,r[3]);goto L8c038c56; /* fmov.s fr2,@-r3 */
L8c038c56: *stop=0x8c038c56u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,1u,r[3]);goto L8c038c58; /* fmov.s fr1,@-r3 */
L8c038c58: *stop=0x8c038c58u;if(!valid_ram(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,0u,r[3]);goto L8c038c5a; /* fmov.s fr0,@-r3 */
L8c038c5a: *stop=0x8c038c5au;r[3]+=4294967292u;goto L8c038c5c; /* add #-0x4,r3 */
L8c038c5c: *stop=0x8c038c5cu;r[7]--;*sr=(*sr&~1u)|(!r[7]);goto L8c038c5e; /* dt r7 */
L8c038c5e: if(!(*sr&1u))goto L8c038c40;goto L8c038c60; /* bf 0x8c038c40 */
L8c038c60: *stop=0x8c038c60u;if(*pr!=entry_pr)return -3;*stop=0x8c038c62u;return 0; /* rts */
}
