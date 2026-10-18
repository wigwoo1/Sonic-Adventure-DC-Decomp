/* AL_MAIN object draw candidate: complete static body, partial integration proof.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
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
SA_STAGE_EXPORT int sa_overlay_object_draw_candidate_0c9058a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc9058a0;
Lc9058a0: *stop=0xc9058a0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc9058a2; /* mov.l r14,@-r15 */
Lc9058a2: *stop=0xc9058a2u;r[0]=4294967295u;goto Lc9058a4; /* mov #-0x1,r0 */
Lc9058a4: *stop=0xc9058a4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto Lc9058a6; /* mov.l r13,@-r15 */
Lc9058a6: *stop=0xc9058a6u;r[0]<<=8;goto Lc9058a8; /* shll8 r0 */
Lc9058a8: *stop=0xc9058a8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto Lc9058aa; /* mov.l r12,@-r15 */
Lc9058aa: *stop=0xc9058aau;r[0]+=112u;goto Lc9058ac; /* add #0x70,r0 */
Lc9058ac: *stop=0xc9058acu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto Lc9058ae; /* mov.l r11,@-r15 */
Lc9058ae: *stop=0xc9058aeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto Lc9058b0; /* mov.l r10,@-r15 */
Lc9058b0: *stop=0xc9058b0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto Lc9058b2; /* mov.l r9,@-r15 */
Lc9058b2: *stop=0xc9058b2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto Lc9058b4; /* mov.l r8,@-r15 */
Lc9058b4: *stop=0xc9058b4u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto Lc9058b6; /* fmov.s fr15,@-r15 */
Lc9058b6: *stop=0xc9058b6u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto Lc9058b8; /* fmov.s fr14,@-r15 */
Lc9058b8: *stop=0xc9058b8u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[15]);goto Lc9058ba; /* fmov.s fr13,@-r15 */
Lc9058ba: *stop=0xc9058bau;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[15]);goto Lc9058bc; /* fmov.s fr12,@-r15 */
Lc9058bc: *stop=0xc9058bcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc9058be; /* sts.l PR,@-r15 */
Lc9058be: *stop=0xc9058beu;r[15]+=r[0];goto Lc9058c0; /* add r0,r15 */
Lc9058c0: *stop=0xc9058c0u;r[0]=4294967256u;goto Lc9058c2; /* mov #-0x28,r0 */
Lc9058c2: *stop=0xc9058c2u;r[14]=r[5];goto Lc9058c4; /* mov r5,r14 */
Lc9058c4: *stop=0xc9058c4u;r[0]=r[0]&255u;goto Lc9058c6; /* extu.b r0,r0 */
Lc9058c6: *stop=0xc9058c6u;r[12]=r[4];goto Lc9058c8; /* mov r4,r12 */
Lc9058c8: *stop=0xc9058c8u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[14]);goto Lc9058ca; /* fmov.s @(r0,r14),fr4 */
Lc9058ca: *stop=0xc9058cau;r[0]=44u;goto Lc9058cc; /* mov #0x2c,r0 */
Lc9058cc: *stop=0xc9058ccu;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[12]);goto Lc9058ce; /* fmov.s @(r0,r12),fr3 */
Lc9058ce: *stop=0xc9058ceu;r[13]=70u;goto Lc9058d0; /* mov #0x46,r13 */
Lc9058d0: *stop=0xc9058d0u;fcopy(f,14u,4u);goto Lc9058d2; /* fmov fr4,fr14 */
Lc9058d2: *stop=0xc9058d2u;r[13]<<=2;goto Lc9058d4; /* shll2 r13 */
Lc9058d4: *stop=0xc9058d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[14],&fr[3],&f->fpscr,&fr[14]);if(status)return status;}goto Lc9058d6; /* fmul fr3,fr14 */
Lc9058d6: *stop=0xc9058d6u;r[0]=48u;goto Lc9058d8; /* mov #0x30,r0 */
Lc9058d8: *stop=0xc9058d8u;r[13]+=r[14];goto Lc9058da; /* add r14,r13 */
Lc9058da: *stop=0xc9058dau;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[12]);goto Lc9058dc; /* fmov.s @(r0,r12),fr3 */
Lc9058dc: *stop=0xc9058dcu;if(!valid_ram(r[13]+6u,2u))return -3;r[0]=sx(m,r[13]+0x6u,2);goto Lc9058de; /* mov.w @(0x6,r13),r0 */
Lc9058de: *stop=0xc9058deu;if(!valid_ram(r[13]+32u,4u))return -3;r[4]=rd(m,r[13]+0x20u,4);goto Lc9058e0; /* mov.l @(0x20,r13),r4 */
Lc9058e0: *stop=0xc9058e0u;r[3]=r[0];goto Lc9058e2; /* mov r0,r3 */
Lc9058e2: *stop=0xc9058e2u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc9058e4; /* shll r0 */
Lc9058e4: *stop=0xc9058e4u;r[0]+=r[3];goto Lc9058e6; /* add r3,r0 */
Lc9058e6: *stop=0xc9058e6u;r[11]=r[4];goto Lc9058e8; /* mov r4,r11 */
Lc9058e8: *stop=0xc9058e8u;r[0]<<=2;goto Lc9058ea; /* shll2 r0 */
Lc9058ea: *stop=0xc9058eau;fcopy(f,13u,4u);goto Lc9058ec; /* fmov fr4,fr13 */
Lc9058ec: *stop=0xc9058ecu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc9058ee; /* shll r0 */
Lc9058ee: *stop=0xc9058eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[3],&f->fpscr,&fr[13]);if(status)return status;}goto Lc9058f0; /* fmul fr3,fr13 */
Lc9058f0: *stop=0xc9058f0u;r[11]+=r[0];goto Lc9058f2; /* add r0,r11 */
Lc9058f2: *stop=0xc9058f2u;if(!valid_ram(r[13]+8u,2u))return -3;r[0]=sx(m,r[13]+0x8u,2);goto Lc9058f4; /* mov.w @(0x8,r13),r0 */
Lc9058f4: *stop=0xc9058f4u;r[9]=r[4];goto Lc9058f6; /* mov r4,r9 */
Lc9058f6: *stop=0xc9058f6u;r[3]=r[0];goto Lc9058f8; /* mov r0,r3 */
Lc9058f8: *stop=0xc9058f8u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc9058fa; /* shll r0 */
Lc9058fa: *stop=0xc9058fau;r[0]+=r[3];goto Lc9058fc; /* add r3,r0 */
Lc9058fc: *stop=0xc9058fcu;r[0]<<=2;goto Lc9058fe; /* shll2 r0 */
Lc9058fe: *stop=0xc9058feu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc905900; /* shll r0 */
Lc905900: *stop=0xc905900u;r[9]+=r[0];goto Lc905902; /* add r0,r9 */
Lc905902: target=0x0c905618u;*pr=0x0c905906u;*stop=0xc905904u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905906; /* bsr 0x0c905618 */
Lc905906: *stop=0xc905906u;r[0]=0x0c905a80u;goto Lc905908; /* mova 0x0c905a80,r0 */
Lc905908: *stop=0xc905908u;r[3]=rd(m,0x0c905a84u,4);goto Lc90590a; /* mov.l 0x0c905a84,r3 */
Lc90590a: *stop=0xc90590au;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]);goto Lc90590c; /* fmov.s @r0,fr7 */
Lc90590c: *stop=0xc90590cu;r[0]=40u;goto Lc90590e; /* mov #0x28,r0 */
Lc90590e: *stop=0xc90590eu;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[12]);goto Lc905910; /* fmov.s @(r0,r12),fr6 */
Lc905910: *stop=0xc905910u;r[0]=36u;goto Lc905912; /* mov #0x24,r0 */
Lc905912: *stop=0xc905912u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[12]);goto Lc905914; /* fmov.s @(r0,r12),fr5 */
Lc905914: *stop=0xc905914u;r[0]=32u;goto Lc905916; /* mov #0x20,r0 */
Lc905916: target=r[3];*pr=0x0c90591au;*stop=0xc905918u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[12]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90591a; /* jsr @r3 */
Lc90591a: *stop=0xc90591au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc90591c; /* tst r0,r0 */
Lc90591c: if(!(*sr&1u))goto Lc905922;goto Lc90591e; /* bf 0x0c905922 */
Lc90591e: *stop=0xc905920u;goto Lc90606e; /* bra 0x0c90606e */
Lc905922: *stop=0xc905922u;r[3]=rd(m,0x0c905a88u,4);goto Lc905924; /* mov.l 0x0c905a88,r3 */
Lc905924: *stop=0xc905924u;r[4]=2u;goto Lc905926; /* mov #0x2,r4 */
Lc905926: *stop=0xc905926u;r[4]<<=8;goto Lc905928; /* shll8 r4 */
Lc905928: target=r[3];*pr=0x0c90592cu;*stop=0xc90592au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90592c; /* jsr @r3 */
Lc90592c: *stop=0xc90592cu;r[0]=96u;goto Lc90592e; /* mov #0x60,r0 */
Lc90592e: *stop=0xc90592eu;r[10]=rd(m,0x0c905a8cu,4);goto Lc905930; /* mov.l 0x0c905a8c,r10 */
Lc905930: *stop=0xc905930u;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905932; /* mov.l @(r0,r14),r0 */
Lc905932: *stop=0xc905932u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto Lc905934; /* fldi1 fr12 */
Lc905934: *stop=0xc905934u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto Lc905936; /* tst #0x10,r0 */
Lc905936: take=!(*sr&1u);*stop=0xc905938u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;if(take)goto Lc90593e;goto Lc90593a; /* bf/s 0x0c90593e */
Lc90593a: *stop=0xc90593cu;goto Lc905fd2; /* bra 0x0c905fd2 */
Lc90593e: *stop=0xc90593eu;r[0]=8u;goto Lc905940; /* mov #0x8,r0 */
Lc905940: *stop=0xc905940u;r[3]=rd(m,0x0c905a90u,4);goto Lc905942; /* mov.l 0x0c905a90,r3 */
Lc905942: *stop=0xc905942u;r[0]<<=8;goto Lc905944; /* shll8 r0 */
Lc905944: *stop=0xc905944u;r[0]+=4294967236u;goto Lc905946; /* add #-0x3c,r0 */
Lc905946: target=r[3];*pr=0x0c90594au;*stop=0xc905948u;if(!valid_ram(r[0]+r[14],4u))return -3;r[4]=rd(m,r[14]+r[0],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90594a; /* jsr @r3 */
Lc90594a: *stop=0xc90594au;fcopy(f,5u,15u);goto Lc90594c; /* fmov fr15,fr5 */
Lc90594c: *stop=0xc90594cu;fcopy(f,6u,15u);goto Lc90594e; /* fmov fr15,fr6 */
Lc90594e: *stop=0xc90594eu;fcopy(f,7u,15u);goto Lc905950; /* fmov fr15,fr7 */
Lc905950: target=r[10];*pr=0x0c905954u;*stop=0xc905952u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905954; /* jsr @r10 */
Lc905954: *stop=0xc905954u;r[2]=rd(m,0x0c905a94u,4);goto Lc905956; /* mov.l 0x0c905a94,r2 */
Lc905956: target=r[2];*pr=0x0c90595au;*stop=0xc905958u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90595a; /* jsr @r2 */
Lc90595a: *stop=0xc90595au;r[3]=rd(m,0x0c905a98u,4);goto Lc90595c; /* mov.l 0x0c905a98,r3 */
Lc90595c: *stop=0xc90595cu;r[5]=r[12];goto Lc90595e; /* mov r12,r5 */
Lc90595e: *stop=0xc90595eu;r[5]+=32u;goto Lc905960; /* add #0x20,r5 */
Lc905960: target=r[3];*pr=0x0c905964u;*stop=0xc905962u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905964; /* jsr @r3 */
Lc905964: *stop=0xc905964u;if(!valid_ram(r[12]+28u,4u))return -3;r[2]=rd(m,r[12]+0x1cu,4);goto Lc905966; /* mov.l @(0x1c,r12),r2 */
Lc905966: *stop=0xc905966u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc905968; /* tst r2,r2 */
Lc905968: take=(*sr&1u);*stop=0xc90596au;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);if(take)goto Lc905974;goto Lc90596c; /* bt/s 0x0c905974 */
Lc90596c: *stop=0xc90596cu;r[3]=rd(m,0x0c905a9cu,4);goto Lc90596e; /* mov.l 0x0c905a9c,r3 */
Lc90596e: *stop=0xc90596eu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto Lc905970; /* mov.l @r15,r5 */
Lc905970: target=r[3];*pr=0x0c905974u;*stop=0xc905972u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905974; /* jsr @r3 */
Lc905974: *stop=0xc905974u;if(!valid_ram(r[12]+20u,4u))return -3;r[3]=rd(m,r[12]+0x14u,4);goto Lc905976; /* mov.l @(0x14,r12),r3 */
Lc905976: *stop=0xc905976u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc905978; /* tst r3,r3 */
Lc905978: take=(*sr&1u);*stop=0xc90597au;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);if(take)goto Lc905984;goto Lc90597c; /* bt/s 0x0c905984 */
Lc90597c: *stop=0xc90597cu;r[3]=rd(m,0x0c905aa0u,4);goto Lc90597e; /* mov.l 0x0c905aa0,r3 */
Lc90597e: *stop=0xc90597eu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto Lc905980; /* mov.l @r15,r5 */
Lc905980: target=r[3];*pr=0x0c905984u;*stop=0xc905982u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905984; /* jsr @r3 */
Lc905984: *stop=0xc905984u;if(!valid_ram(r[12]+24u,4u))return -3;r[3]=rd(m,r[12]+0x18u,4);goto Lc905986; /* mov.l @(0x18,r12),r3 */
Lc905986: *stop=0xc905986u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc905988; /* tst r3,r3 */
Lc905988: take=(*sr&1u);*stop=0xc90598au;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);if(take)goto Lc905994;goto Lc90598c; /* bt/s 0x0c905994 */
Lc90598c: *stop=0xc90598cu;r[3]=rd(m,0x0c905aa4u,4);goto Lc90598e; /* mov.l 0x0c905aa4,r3 */
Lc90598e: *stop=0xc90598eu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto Lc905990; /* mov.l @r15,r5 */
Lc905990: target=r[3];*pr=0x0c905994u;*stop=0xc905992u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905994; /* jsr @r3 */
Lc905994: *stop=0xc905994u;r[0]=0x0c905aa8u;goto Lc905996; /* mova 0x0c905aa8,r0 */
Lc905996: *stop=0xc905996u;r[3]=rd(m,0x0c905aacu,4);goto Lc905998; /* mov.l 0x0c905aac,r3 */
Lc905998: *stop=0xc905998u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc90599a; /* fmov.s @r0,fr5 */
Lc90599a: *stop=0xc90599au;fcopy(f,6u,15u);goto Lc90599c; /* fmov fr15,fr6 */
Lc90599c: *stop=0xc90599cu;fcopy(f,4u,15u);goto Lc90599e; /* fmov fr15,fr4 */
Lc90599e: target=r[3];*pr=0x0c9059a2u;*stop=0xc9059a0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9059a2; /* jsr @r3 */
Lc9059a2: *stop=0xc9059a2u;r[0]=4294967168u;goto Lc9059a4; /* mov #-0x80,r0 */
Lc9059a4: *stop=0xc9059a4u;r[0]=r[0]&255u;goto Lc9059a6; /* extu.b r0,r0 */
Lc9059a6: *stop=0xc9059a6u;if(!valid_ram(r[0]+r[14],4u))return -3;r[2]=rd(m,r[14]+r[0],4);goto Lc9059a8; /* mov.l @(r0,r14),r2 */
Lc9059a8: *stop=0xc9059a8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc9059aa; /* tst r2,r2 */
Lc9059aa: take=(*sr&1u);*stop=0xc9059acu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);if(take)goto Lc9059b6;goto Lc9059ae; /* bt/s 0x0c9059b6 */
Lc9059ae: *stop=0xc9059aeu;r[3]=rd(m,0x0c905aa0u,4);goto Lc9059b0; /* mov.l 0x0c905aa0,r3 */
Lc9059b0: *stop=0xc9059b0u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto Lc9059b2; /* mov.l @r15,r5 */
Lc9059b2: target=r[3];*pr=0x0c9059b6u;*stop=0xc9059b4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9059b6; /* jsr @r3 */
Lc9059b6: *stop=0xc9059b6u;r[0]=124u;goto Lc9059b8; /* mov #0x7c,r0 */
Lc9059b8: *stop=0xc9059b8u;if(!valid_ram(r[0]+r[14],4u))return -3;r[3]=rd(m,r[14]+r[0],4);goto Lc9059ba; /* mov.l @(r0,r14),r3 */
Lc9059ba: *stop=0xc9059bau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc9059bc; /* tst r3,r3 */
Lc9059bc: take=(*sr&1u);*stop=0xc9059beu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);if(take)goto Lc9059c8;goto Lc9059c0; /* bt/s 0x0c9059c8 */
Lc9059c0: *stop=0xc9059c0u;r[3]=rd(m,0x0c905a9cu,4);goto Lc9059c2; /* mov.l 0x0c905a9c,r3 */
Lc9059c2: *stop=0xc9059c2u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto Lc9059c4; /* mov.l @r15,r5 */
Lc9059c4: target=r[3];*pr=0x0c9059c8u;*stop=0xc9059c6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9059c8; /* jsr @r3 */
Lc9059c8: *stop=0xc9059c8u;r[0]=0x0c905ab0u;goto Lc9059ca; /* mova 0x0c905ab0,r0 */
Lc9059ca: *stop=0xc9059cau;r[3]=rd(m,0x0c905aacu,4);goto Lc9059cc; /* mov.l 0x0c905aac,r3 */
Lc9059cc: *stop=0xc9059ccu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc9059ce; /* fmov.s @r0,fr5 */
Lc9059ce: *stop=0xc9059ceu;fcopy(f,6u,15u);goto Lc9059d0; /* fmov fr15,fr6 */
Lc9059d0: *stop=0xc9059d0u;fcopy(f,4u,15u);goto Lc9059d2; /* fmov fr15,fr4 */
Lc9059d2: target=r[3];*pr=0x0c9059d6u;*stop=0xc9059d4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9059d6; /* jsr @r3 */
Lc9059d6: *stop=0xc9059d6u;r[2]=rd(m,0x0c905ab4u,4);goto Lc9059d8; /* mov.l 0x0c905ab4,r2 */
Lc9059d8: *stop=0xc9059d8u;fcopy(f,4u,14u);goto Lc9059da; /* fmov fr14,fr4 */
Lc9059da: *stop=0xc9059dau;fcopy(f,6u,14u);goto Lc9059dc; /* fmov fr14,fr6 */
Lc9059dc: *stop=0xc9059dcu;fcopy(f,5u,13u);goto Lc9059de; /* fmov fr13,fr5 */
Lc9059de: target=r[2];*pr=0x0c9059e2u;*stop=0xc9059e0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9059e2; /* jsr @r2 */
Lc9059e2: *stop=0xc9059e2u;r[3]=rd(m,0x0c905aa4u,4);goto Lc9059e4; /* mov.l 0x0c905aa4,r3 */
Lc9059e4: *stop=0xc9059e4u;r[5]=64u;goto Lc9059e6; /* mov #0x40,r5 */
Lc9059e6: *stop=0xc9059e6u;r[5]<<=8;goto Lc9059e8; /* shll8 r5 */
Lc9059e8: target=r[3];*pr=0x0c9059ecu;*stop=0xc9059eau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9059ec; /* jsr @r3 */
Lc9059ec: *stop=0xc9059ecu;r[0]=0x0c905abcu;goto Lc9059ee; /* mova 0x0c905abc,r0 */
Lc9059ee: *stop=0xc9059eeu;r[2]=rd(m,0x0c905ac4u,4);goto Lc9059f0; /* mov.l 0x0c905ac4,r2 */
Lc9059f0: *stop=0xc9059f0u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[0]);goto Lc9059f2; /* fmov.s @r0,fr13 */
Lc9059f2: *stop=0xc9059f2u;r[0]=0x0c905ac0u;goto Lc9059f4; /* mova 0x0c905ac0,r0 */
Lc9059f4: *stop=0xc9059f4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]);goto Lc9059f6; /* fmov.s @r0,fr14 */
Lc9059f6: *stop=0xc9059f6u;r[4]=1u;goto Lc9059f8; /* mov #0x1,r4 */
Lc9059f8: *stop=0xc9059f8u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto Lc9059fa; /* mov.l @r2,r3 */
Lc9059fa: *stop=0xc9059fau;r[4]<<=8;goto Lc9059fc; /* shll8 r4 */
Lc9059fc: *stop=0xc9059fcu;r[8]=rd(m,0x0c905ab8u,4);goto Lc9059fe; /* mov.l 0x0c905ab8,r8 */
Lc9059fe: *stop=0xc9059feu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto Lc905a00; /* mov.l r3,@r15 */
Lc905a00: *stop=0xc905a00u;if(!valid_ram(r[13]+2u,2u))return -3;r[0]=sx(m,r[13]+0x2u,2);goto Lc905a02; /* mov.w @(0x2,r13),r0 */
Lc905a02: *stop=0xc905a02u;r[0]=r[0]&65535u;goto Lc905a04; /* extu.w r0,r0 */
Lc905a04: *stop=0xc905a04u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto Lc905a06; /* tst #0x1,r0 */
Lc905a06: if(!(*sr&1u))goto Lc905a0c;goto Lc905a08; /* bf 0x0c905a0c */
Lc905a08: *stop=0xc905a0au;goto Lc905db8; /* bra 0x0c905db8 */
Lc905a0c: *stop=0xc905a0cu;r[0]=96u;goto Lc905a0e; /* mov #0x60,r0 */
Lc905a0e: *stop=0xc905a0eu;if(!valid_ram(r[0]+r[14],4u))return -3;r[1]=rd(m,r[14]+r[0],4);goto Lc905a10; /* mov.l @(r0,r14),r1 */
Lc905a10: *stop=0xc905a10u;*sr=(*sr&~1u)|!(r[1]&r[4]);goto Lc905a12; /* tst r1,r4 */
Lc905a12: if((*sr&1u))goto Lc905a18;goto Lc905a14; /* bt 0x0c905a18 */
Lc905a14: *stop=0xc905a16u;fcopy(f,4u,15u);goto Lc905a26; /* bra 0x0c905a26 */
Lc905a18: *stop=0xc905a18u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto Lc905a1a; /* mov.l @r15,r5 */
Lc905a1a: *stop=0xc905a1au;r[4]=r[12];goto Lc905a1c; /* mov r12,r4 */
Lc905a1c: *stop=0xc905a1cu;r[3]=rd(m,0x0c905ac8u,4);goto Lc905a1e; /* mov.l 0x0c905ac8,r3 */
Lc905a1e: *stop=0xc905a1eu;r[5]+=32u;goto Lc905a20; /* add #0x20,r5 */
Lc905a20: target=r[3];*pr=0x0c905a24u;*stop=0xc905a22u;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905a24; /* jsr @r3 */
Lc905a24: *stop=0xc905a24u;fcopy(f,4u,0u);goto Lc905a26; /* fmov fr0,fr4 */
Lc905a26: *stop=0xc905a26u;r[0]=4294967276u;goto Lc905a28; /* mov #-0x14,r0 */
Lc905a28: *stop=0xc905a28u;r[0]=r[0]&255u;goto Lc905a2a; /* extu.b r0,r0 */
Lc905a2a: *stop=0xc905a2au;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905a2c; /* mov.l @(r0,r14),r0 */
Lc905a2c: *stop=0xc905a2cu;r[1]=54u;goto Lc905a2e; /* mov #0x36,r1 */
Lc905a2e: *stop=0xc905a2eu;if(!valid_ram(r[0]+r[1],2u))return -3;r[0]=sx(m,r[0]+r[1],2);goto Lc905a30; /* mov.w @(r0,r1),r0 */
Lc905a30: *stop=0xc905a30u;r[0]=r[0]&65535u;goto Lc905a32; /* extu.w r0,r0 */
Lc905a32: *stop=0xc905a32u;*sr=(*sr&~1u)|!(r[0]&0x40u);goto Lc905a34; /* tst #0x40,r0 */
Lc905a34: if((*sr&1u))goto Lc905ae0;goto Lc905a36; /* bt 0x0c905ae0 */
Lc905a36: *stop=0xc905a36u;r[0]=4294967168u;goto Lc905a38; /* mov #-0x80,r0 */
Lc905a38: *stop=0xc905a38u;if(!valid_ram(r[11],4u))return -3;r[2]=rd(m,r[11],4);goto Lc905a3a; /* mov.l @r11,r2 */
Lc905a3a: *stop=0xc905a3au;r[0]=r[0]&255u;goto Lc905a3c; /* extu.b r0,r0 */
Lc905a3c: *stop=0xc905a3cu;r[4]=r[15];goto Lc905a3e; /* mov r15,r4 */
Lc905a3e: *stop=0xc905a3eu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc905a40; /* mov.l r2,@(r0,r15) */
Lc905a40: *stop=0xc905a40u;r[0]=4294967172u;goto Lc905a42; /* mov #-0x7c,r0 */
Lc905a42: *stop=0xc905a42u;if(!valid_ram(r[9],4u))return -3;r[3]=rd(m,r[9],4);goto Lc905a44; /* mov.l @r9,r3 */
Lc905a44: *stop=0xc905a44u;r[0]=r[0]&255u;goto Lc905a46; /* extu.b r0,r0 */
Lc905a46: *stop=0xc905a46u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc905a48; /* mov.l r3,@(r0,r15) */
Lc905a48: *stop=0xc905a48u;r[0]=12u;goto Lc905a4a; /* mov #0xc,r0 */
Lc905a4a: *stop=0xc905a4au;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto Lc905a4c; /* fmov.s @(r0,r13),fr3 */
Lc905a4c: *stop=0xc905a4cu;r[0]=4294967176u;goto Lc905a4e; /* mov #-0x78,r0 */
Lc905a4e: *stop=0xc905a4eu;r[0]=r[0]&255u;goto Lc905a50; /* extu.b r0,r0 */
Lc905a50: *stop=0xc905a50u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc905a52; /* fmov fr3,@(r0,r15) */
Lc905a52: *stop=0xc905a52u;r[0]=20u;goto Lc905a54; /* mov #0x14,r0 */
Lc905a54: *stop=0xc905a54u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto Lc905a56; /* fmov.s @(r0,r13),fr3 */
Lc905a56: *stop=0xc905a56u;r[0]=4294967180u;goto Lc905a58; /* mov #-0x74,r0 */
Lc905a58: *stop=0xc905a58u;r[3]=4294967168u;goto Lc905a5a; /* mov #-0x80,r3 */
Lc905a5a: *stop=0xc905a5au;r[0]=r[0]&255u;goto Lc905a5c; /* extu.b r0,r0 */
Lc905a5c: *stop=0xc905a5cu;r[3]=r[3]&255u;goto Lc905a5e; /* extu.b r3,r3 */
Lc905a5e: *stop=0xc905a5eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc905a60; /* fmov fr3,@(r0,r15) */
Lc905a60: *stop=0xc905a60u;r[0]=124u;goto Lc905a62; /* mov #0x7c,r0 */
Lc905a62: *stop=0xc905a62u;r[3]+=r[15];goto Lc905a64; /* add r15,r3 */
Lc905a64: *stop=0xc905a64u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc905a66; /* mov.l r3,@(r0,r15) */
Lc905a66: *stop=0xc905a66u;r[0]=2u;goto Lc905a68; /* mov #0x2,r0 */
Lc905a68: *stop=0xc905a68u;r[0]<<=8;goto Lc905a6a; /* shll8 r0 */
Lc905a6a: *stop=0xc905a6au;r[0]+=72u;goto Lc905a6c; /* add #0x48,r0 */
Lc905a6c: *stop=0xc905a6cu;if(!valid_ram(r[0]+r[14],4u))return -3;r[2]=rd(m,r[14]+r[0],4);goto Lc905a6e; /* mov.l @(r0,r14),r2 */
Lc905a6e: *stop=0xc905a6eu;r[0]=120u;goto Lc905a70; /* mov #0x78,r0 */
Lc905a70: *stop=0xc905a70u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc905a72; /* mov.l r2,@(r0,r15) */
Lc905a72: *stop=0xc905a72u;r[0]=24u;goto Lc905a74; /* mov #0x18,r0 */
Lc905a74: *stop=0xc905a74u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905a76; /* fmov.s @(r0,r13),fr4 */
Lc905a76: *stop=0xc905a76u;r[3]=rd(m,0x0c905accu,4);goto Lc905a78; /* mov.l 0x0c905acc,r3 */
Lc905a78: target=r[3];*pr=0x0c905a7cu;*stop=0xc905a7au;r[4]+=120u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905a7c; /* jsr @r3 */
Lc905a7c: *stop=0xc905a7eu;goto Lc905ca8; /* bra 0x0c905ca8 */
Lc905ae0: *stop=0xc905ae0u;r[0]=4294967276u;goto Lc905ae2; /* mov #-0x14,r0 */
Lc905ae2: *stop=0xc905ae2u;r[0]=r[0]&255u;goto Lc905ae4; /* extu.b r0,r0 */
Lc905ae4: *stop=0xc905ae4u;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905ae6; /* mov.l @(r0,r14),r0 */
Lc905ae6: *stop=0xc905ae6u;r[2]=54u;goto Lc905ae8; /* mov #0x36,r2 */
Lc905ae8: *stop=0xc905ae8u;if(!valid_ram(r[0]+r[2],2u))return -3;r[0]=sx(m,r[0]+r[2],2);goto Lc905aea; /* mov.w @(r0,r2),r0 */
Lc905aea: *stop=0xc905aeau;r[0]=r[0]&65535u;goto Lc905aec; /* extu.w r0,r0 */
Lc905aec: *stop=0xc905aecu;*sr=(*sr&~1u)|!(r[0]&0xcu);goto Lc905aee; /* tst #0xc,r0 */
Lc905aee: if((*sr&1u))goto Lc905b5a;goto Lc905af0; /* bt 0x0c905b5a */
Lc905af0: *stop=0xc905af0u;r[0]=4294967168u;goto Lc905af2; /* mov #-0x80,r0 */
Lc905af2: *stop=0xc905af2u;if(!valid_ram(r[11],4u))return -3;r[3]=rd(m,r[11],4);goto Lc905af4; /* mov.l @r11,r3 */
Lc905af4: *stop=0xc905af4u;r[0]=r[0]&255u;goto Lc905af6; /* extu.b r0,r0 */
Lc905af6: *stop=0xc905af6u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc905af8; /* mov.l r3,@(r0,r15) */
Lc905af8: *stop=0xc905af8u;r[0]=4294967172u;goto Lc905afa; /* mov #-0x7c,r0 */
Lc905afa: *stop=0xc905afau;if(!valid_ram(r[9],4u))return -3;r[3]=rd(m,r[9],4);goto Lc905afc; /* mov.l @r9,r3 */
Lc905afc: *stop=0xc905afcu;r[0]=r[0]&255u;goto Lc905afe; /* extu.b r0,r0 */
Lc905afe: *stop=0xc905afeu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc905b00; /* mov.l r3,@(r0,r15) */
Lc905b00: *stop=0xc905b00u;r[0]=12u;goto Lc905b02; /* mov #0xc,r0 */
Lc905b02: *stop=0xc905b02u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto Lc905b04; /* fmov.s @(r0,r13),fr3 */
Lc905b04: *stop=0xc905b04u;r[0]=4294967176u;goto Lc905b06; /* mov #-0x78,r0 */
Lc905b06: *stop=0xc905b06u;r[0]=r[0]&255u;goto Lc905b08; /* extu.b r0,r0 */
Lc905b08: *stop=0xc905b08u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc905b0a; /* fmov fr3,@(r0,r15) */
Lc905b0a: *stop=0xc905b0au;r[0]=20u;goto Lc905b0c; /* mov #0x14,r0 */
Lc905b0c: *stop=0xc905b0cu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto Lc905b0e; /* fmov.s @(r0,r13),fr3 */
Lc905b0e: *stop=0xc905b0eu;r[0]=4294967180u;goto Lc905b10; /* mov #-0x74,r0 */
Lc905b10: *stop=0xc905b10u;r[3]=4294967168u;goto Lc905b12; /* mov #-0x80,r3 */
Lc905b12: *stop=0xc905b12u;r[0]=r[0]&255u;goto Lc905b14; /* extu.b r0,r0 */
Lc905b14: *stop=0xc905b14u;r[3]=r[3]&255u;goto Lc905b16; /* extu.b r3,r3 */
Lc905b16: *stop=0xc905b16u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc905b18; /* fmov fr3,@(r0,r15) */
Lc905b18: *stop=0xc905b18u;r[0]=124u;goto Lc905b1a; /* mov #0x7c,r0 */
Lc905b1a: *stop=0xc905b1au;r[3]+=r[15];goto Lc905b1c; /* add r15,r3 */
Lc905b1c: *stop=0xc905b1cu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc905b1e; /* mov.l r3,@(r0,r15) */
Lc905b1e: *stop=0xc905b1eu;r[0]=2u;goto Lc905b20; /* mov #0x2,r0 */
Lc905b20: *stop=0xc905b20u;r[0]<<=8;goto Lc905b22; /* shll8 r0 */
Lc905b22: *stop=0xc905b22u;r[0]+=72u;goto Lc905b24; /* add #0x48,r0 */
Lc905b24: *stop=0xc905b24u;if(!valid_ram(r[0]+r[14],4u))return -3;r[3]=rd(m,r[14]+r[0],4);goto Lc905b26; /* mov.l @(r0,r14),r3 */
Lc905b26: *stop=0xc905b26u;r[0]=120u;goto Lc905b28; /* mov #0x78,r0 */
Lc905b28: *stop=0xc905b28u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc905b2a; /* mov.l r3,@(r0,r15) */
Lc905b2a: *stop=0xc905b2au;r[0]=4294967276u;goto Lc905b2c; /* mov #-0x14,r0 */
Lc905b2c: *stop=0xc905b2cu;r[0]=r[0]&255u;goto Lc905b2e; /* extu.b r0,r0 */
Lc905b2e: *stop=0xc905b2eu;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905b30; /* mov.l @(r0,r14),r0 */
Lc905b30: *stop=0xc905b30u;r[1]=54u;goto Lc905b32; /* mov #0x36,r1 */
Lc905b32: *stop=0xc905b32u;if(!valid_ram(r[0]+r[1],2u))return -3;r[0]=sx(m,r[0]+r[1],2);goto Lc905b34; /* mov.w @(r0,r1),r0 */
Lc905b34: *stop=0xc905b34u;r[0]=r[0]&65535u;goto Lc905b36; /* extu.w r0,r0 */
Lc905b36: *stop=0xc905b36u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto Lc905b38; /* tst #0x2,r0 */
Lc905b38: if((*sr&1u))goto Lc905b4a;goto Lc905b3a; /* bt 0x0c905b4a */
Lc905b3a: *stop=0xc905b3au;r[0]=24u;goto Lc905b3c; /* mov #0x18,r0 */
Lc905b3c: *stop=0xc905b3cu;r[3]=rd(m,0x0c905c40u,4);goto Lc905b3e; /* mov.l 0x0c905c40,r3 */
Lc905b3e: *stop=0xc905b3eu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905b40; /* fmov.s @(r0,r13),fr4 */
Lc905b40: *stop=0xc905b40u;r[4]=r[15];goto Lc905b42; /* mov r15,r4 */
Lc905b42: target=r[3];*pr=0x0c905b46u;*stop=0xc905b44u;r[4]+=120u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905b46; /* jsr @r3 */
Lc905b46: *stop=0xc905b48u;goto Lc905ca8; /* bra 0x0c905ca8 */
Lc905b4a: *stop=0xc905b4au;r[0]=24u;goto Lc905b4c; /* mov #0x18,r0 */
Lc905b4c: *stop=0xc905b4cu;r[3]=rd(m,0x0c905c44u,4);goto Lc905b4e; /* mov.l 0x0c905c44,r3 */
Lc905b4e: *stop=0xc905b4eu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905b50; /* fmov.s @(r0,r13),fr4 */
Lc905b50: *stop=0xc905b50u;r[4]=r[15];goto Lc905b52; /* mov r15,r4 */
Lc905b52: target=r[3];*pr=0x0c905b56u;*stop=0xc905b54u;r[4]+=120u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905b56; /* jsr @r3 */
Lc905b56: *stop=0xc905b58u;goto Lc905ca8; /* bra 0x0c905ca8 */
Lc905b5a: *stop=0xc905b5au;r[0]=0x0c905c48u;goto Lc905b5c; /* mova 0x0c905c48,r0 */
Lc905b5c: *stop=0xc905b5cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc905b5e; /* fmov.s @r0,fr3 */
Lc905b5e: *stop=0xc905b5eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc905b60; /* fcmp/gt fr3,fr4 */
Lc905b60: if(!(*sr&1u))goto Lc905bbc;goto Lc905b62; /* bf 0x0c905bbc */
Lc905b62: *stop=0xc905b62u;r[0]=9u;goto Lc905b64; /* mov #0x9,r0 */
Lc905b64: *stop=0xc905b64u;fcopy(f,1u,13u);goto Lc905b66; /* fmov fr13,fr1 */
Lc905b66: *stop=0xc905b66u;r[0]<<=8;goto Lc905b68; /* shll8 r0 */
Lc905b68: *stop=0xc905b68u;r[0]+=4294967238u;goto Lc905b6a; /* add #-0x3a,r0 */
Lc905b6a: *stop=0xc905b6au;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905b6c; /* mov.b @(r0,r14),r3 */
Lc905b6c: *stop=0xc905b6cu;r[0]=20u;goto Lc905b6e; /* mov #0x14,r0 */
Lc905b6e: *stop=0xc905b6eu;r[3]=r[3]&255u;goto Lc905b70; /* extu.b r3,r3 */
Lc905b70: *stop=0xc905b70u;*fpul=r[3];goto Lc905b72; /* lds r3,FPUL */
Lc905b72: *stop=0xc905b72u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc905b74; /* float FPUL,fr2 */
Lc905b74: *stop=0xc905b74u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[13],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905b76; /* fmul fr13,fr2 */
Lc905b76: *stop=0xc905b76u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto Lc905b78; /* fmov fr2,@(r0,r15) */
Lc905b78: *stop=0xc905b78u;r[0]=9u;goto Lc905b7a; /* mov #0x9,r0 */
Lc905b7a: *stop=0xc905b7au;r[0]<<=8;goto Lc905b7c; /* shll8 r0 */
Lc905b7c: *stop=0xc905b7cu;r[0]+=4294967237u;goto Lc905b7e; /* add #-0x3b,r0 */
Lc905b7e: *stop=0xc905b7eu;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905b80; /* mov.b @(r0,r14),r3 */
Lc905b80: *stop=0xc905b80u;r[0]=16u;goto Lc905b82; /* mov #0x10,r0 */
Lc905b82: *stop=0xc905b82u;r[3]=r[3]&255u;goto Lc905b84; /* extu.b r3,r3 */
Lc905b84: *stop=0xc905b84u;*fpul=r[3];goto Lc905b86; /* lds r3,FPUL */
Lc905b86: *stop=0xc905b86u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc905b88; /* float FPUL,fr2 */
Lc905b88: *stop=0xc905b88u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[13],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905b8a; /* fmul fr13,fr2 */
Lc905b8a: *stop=0xc905b8au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto Lc905b8c; /* fmov fr2,@(r0,r15) */
Lc905b8c: *stop=0xc905b8cu;r[0]=9u;goto Lc905b8e; /* mov #0x9,r0 */
Lc905b8e: *stop=0xc905b8eu;r[0]<<=8;goto Lc905b90; /* shll8 r0 */
Lc905b90: *stop=0xc905b90u;r[0]+=4294967236u;goto Lc905b92; /* add #-0x3c,r0 */
Lc905b92: *stop=0xc905b92u;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905b94; /* mov.b @(r0,r14),r3 */
Lc905b94: *stop=0xc905b94u;r[3]=r[3]&255u;goto Lc905b96; /* extu.b r3,r3 */
Lc905b96: *stop=0xc905b96u;*fpul=r[3];goto Lc905b98; /* lds r3,FPUL */
Lc905b98: *stop=0xc905b98u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc905b9a; /* float FPUL,fr2 */
Lc905b9a: *stop=0xc905b9au;fcopy(f,13u,2u);goto Lc905b9c; /* fmov fr2,fr13 */
Lc905b9c: *stop=0xc905b9cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[1],&f->fpscr,&fr[13]);if(status)return status;}goto Lc905b9e; /* fmul fr1,fr13 */
Lc905b9e: *stop=0xc905b9eu;fcopy(f,7u,13u);goto Lc905ba0; /* fmov fr13,fr7 */
Lc905ba0: *stop=0xc905ba0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[14],&f->fpscr,&fr[7],0);if(status)return status;}goto Lc905ba2; /* fadd fr14,fr7 */
Lc905ba2: *stop=0xc905ba2u;r[0]=16u;goto Lc905ba4; /* mov #0x10,r0 */
Lc905ba4: *stop=0xc905ba4u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[15]);goto Lc905ba6; /* fmov.s @(r0,r15),fr6 */
Lc905ba6: *stop=0xc905ba6u;r[0]=20u;goto Lc905ba8; /* mov #0x14,r0 */
Lc905ba8: *stop=0xc905ba8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[15]);goto Lc905baa; /* fmov.s @(r0,r15),fr5 */
Lc905baa: *stop=0xc905baau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[14],&f->fpscr,&fr[6],0);if(status)return status;}goto Lc905bac; /* fadd fr14,fr6 */
Lc905bac: *stop=0xc905bacu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[14],&f->fpscr,&fr[5],0);if(status)return status;}goto Lc905bae; /* fadd fr14,fr5 */
Lc905bae: target=r[10];*pr=0x0c905bb2u;*stop=0xc905bb0u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905bb2; /* jsr @r10 */
Lc905bb2: *stop=0xc905bb2u;r[0]=12u;goto Lc905bb4; /* mov #0xc,r0 */
Lc905bb4: *stop=0xc905bb4u;r[4]=rd(m,0x0c905c4cu,4);goto Lc905bb6; /* mov.l 0x0c905c4c,r4 */
Lc905bb6: *stop=0xc905bb6u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905bb8; /* fmov.s @(r0,r13),fr4 */
Lc905bb8: *stop=0xc905bbau;if(!valid_ram(r[11],4u))return -3;r[5]=rd(m,r[11],4);goto Lc905c1c; /* bra 0x0c905c1c */
Lc905bbc: *stop=0xc905bbcu;r[0]=0x0c905c50u;goto Lc905bbe; /* mova 0x0c905c50,r0 */
Lc905bbe: *stop=0xc905bbeu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto Lc905bc0; /* fmov.s @r0,fr2 */
Lc905bc0: *stop=0xc905bc0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto Lc905bc2; /* fcmp/gt fr2,fr4 */
Lc905bc2: if(!(*sr&1u))goto Lc905c60;goto Lc905bc4; /* bf 0x0c905c60 */
Lc905bc4: *stop=0xc905bc4u;r[0]=9u;goto Lc905bc6; /* mov #0x9,r0 */
Lc905bc6: *stop=0xc905bc6u;r[0]<<=8;goto Lc905bc8; /* shll8 r0 */
Lc905bc8: *stop=0xc905bc8u;r[0]+=4294967238u;goto Lc905bca; /* add #-0x3a,r0 */
Lc905bca: *stop=0xc905bcau;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905bcc; /* mov.b @(r0,r14),r3 */
Lc905bcc: *stop=0xc905bccu;r[0]=28u;goto Lc905bce; /* mov #0x1c,r0 */
Lc905bce: *stop=0xc905bceu;r[3]=r[3]&255u;goto Lc905bd0; /* extu.b r3,r3 */
Lc905bd0: *stop=0xc905bd0u;*fpul=r[3];goto Lc905bd2; /* lds r3,FPUL */
Lc905bd2: *stop=0xc905bd2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto Lc905bd4; /* float FPUL,fr1 */
Lc905bd4: *stop=0xc905bd4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[13],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905bd6; /* fmul fr13,fr1 */
Lc905bd6: *stop=0xc905bd6u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[15]);goto Lc905bd8; /* fmov fr1,@(r0,r15) */
Lc905bd8: *stop=0xc905bd8u;r[0]=9u;goto Lc905bda; /* mov #0x9,r0 */
Lc905bda: *stop=0xc905bdau;r[0]<<=8;goto Lc905bdc; /* shll8 r0 */
Lc905bdc: *stop=0xc905bdcu;fcopy(f,1u,13u);goto Lc905bde; /* fmov fr13,fr1 */
Lc905bde: *stop=0xc905bdeu;r[0]+=4294967237u;goto Lc905be0; /* add #-0x3b,r0 */
Lc905be0: *stop=0xc905be0u;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905be2; /* mov.b @(r0,r14),r3 */
Lc905be2: *stop=0xc905be2u;r[0]=24u;goto Lc905be4; /* mov #0x18,r0 */
Lc905be4: *stop=0xc905be4u;r[3]=r[3]&255u;goto Lc905be6; /* extu.b r3,r3 */
Lc905be6: *stop=0xc905be6u;*fpul=r[3];goto Lc905be8; /* lds r3,FPUL */
Lc905be8: *stop=0xc905be8u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc905bea; /* float FPUL,fr3 */
Lc905bea: *stop=0xc905beau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[13],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905bec; /* fmul fr13,fr3 */
Lc905bec: *stop=0xc905becu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc905bee; /* fmov fr3,@(r0,r15) */
Lc905bee: *stop=0xc905beeu;r[0]=9u;goto Lc905bf0; /* mov #0x9,r0 */
Lc905bf0: *stop=0xc905bf0u;r[0]<<=8;goto Lc905bf2; /* shll8 r0 */
Lc905bf2: *stop=0xc905bf2u;r[0]+=4294967236u;goto Lc905bf4; /* add #-0x3c,r0 */
Lc905bf4: *stop=0xc905bf4u;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905bf6; /* mov.b @(r0,r14),r3 */
Lc905bf6: *stop=0xc905bf6u;r[3]=r[3]&255u;goto Lc905bf8; /* extu.b r3,r3 */
Lc905bf8: *stop=0xc905bf8u;*fpul=r[3];goto Lc905bfa; /* lds r3,FPUL */
Lc905bfa: *stop=0xc905bfau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc905bfc; /* float FPUL,fr3 */
Lc905bfc: *stop=0xc905bfcu;fcopy(f,13u,3u);goto Lc905bfe; /* fmov fr3,fr13 */
Lc905bfe: *stop=0xc905bfeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[1],&f->fpscr,&fr[13]);if(status)return status;}goto Lc905c00; /* fmul fr1,fr13 */
Lc905c00: *stop=0xc905c00u;fcopy(f,7u,13u);goto Lc905c02; /* fmov fr13,fr7 */
Lc905c02: *stop=0xc905c02u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[14],&f->fpscr,&fr[7],0);if(status)return status;}goto Lc905c04; /* fadd fr14,fr7 */
Lc905c04: *stop=0xc905c04u;r[0]=24u;goto Lc905c06; /* mov #0x18,r0 */
Lc905c06: *stop=0xc905c06u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[15]);goto Lc905c08; /* fmov.s @(r0,r15),fr6 */
Lc905c08: *stop=0xc905c08u;r[0]=28u;goto Lc905c0a; /* mov #0x1c,r0 */
Lc905c0a: *stop=0xc905c0au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[15]);goto Lc905c0c; /* fmov.s @(r0,r15),fr5 */
Lc905c0c: *stop=0xc905c0cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[14],&f->fpscr,&fr[6],0);if(status)return status;}goto Lc905c0e; /* fadd fr14,fr6 */
Lc905c0e: *stop=0xc905c0eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[14],&f->fpscr,&fr[5],0);if(status)return status;}goto Lc905c10; /* fadd fr14,fr5 */
Lc905c10: target=r[10];*pr=0x0c905c14u;*stop=0xc905c12u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905c14; /* jsr @r10 */
Lc905c14: *stop=0xc905c14u;r[0]=12u;goto Lc905c16; /* mov #0xc,r0 */
Lc905c16: *stop=0xc905c16u;r[4]=rd(m,0x0c905c54u,4);goto Lc905c18; /* mov.l 0x0c905c54,r4 */
Lc905c18: *stop=0xc905c18u;if(!valid_ram(r[11],4u))return -3;r[5]=rd(m,r[11],4);goto Lc905c1a; /* mov.l @r11,r5 */
Lc905c1a: *stop=0xc905c1au;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905c1c; /* fmov.s @(r0,r13),fr4 */
Lc905c1c: target=r[8];*pr=0x0c905c20u;*stop=0xc905c1eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905c20; /* jsr @r8 */
Lc905c20: *stop=0xc905c20u;fcopy(f,5u,15u);goto Lc905c22; /* fmov fr15,fr5 */
Lc905c22: *stop=0xc905c22u;fcopy(f,6u,15u);goto Lc905c24; /* fmov fr15,fr6 */
Lc905c24: *stop=0xc905c24u;fcopy(f,7u,15u);goto Lc905c26; /* fmov fr15,fr7 */
Lc905c26: target=r[10];*pr=0x0c905c2au;*stop=0xc905c28u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905c2a; /* jsr @r10 */
Lc905c2a: *stop=0xc905c2cu;goto Lc905ca8; /* bra 0x0c905ca8 */
Lc905c60: *stop=0xc905c60u;r[3]=4294967168u;goto Lc905c62; /* mov #-0x80,r3 */
Lc905c62: *stop=0xc905c62u;if(!valid_ram(r[11],4u))return -3;r[0]=rd(m,r[11],4);goto Lc905c64; /* mov.l @r11,r0 */
Lc905c64: *stop=0xc905c64u;r[3]=r[3]&255u;goto Lc905c66; /* extu.b r3,r3 */
Lc905c66: *stop=0xc905c66u;r[3]+=r[15];goto Lc905c68; /* add r15,r3 */
Lc905c68: *stop=0xc905c68u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[0],4);goto Lc905c6a; /* mov.l r0,@r3 */
Lc905c6a: *stop=0xc905c6au;r[0]=4294967172u;goto Lc905c6c; /* mov #-0x7c,r0 */
Lc905c6c: *stop=0xc905c6cu;if(!valid_ram(r[9],4u))return -3;r[3]=rd(m,r[9],4);goto Lc905c6e; /* mov.l @r9,r3 */
Lc905c6e: *stop=0xc905c6eu;r[0]=r[0]&255u;goto Lc905c70; /* extu.b r0,r0 */
Lc905c70: *stop=0xc905c70u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc905c72; /* mov.l r3,@(r0,r15) */
Lc905c72: *stop=0xc905c72u;r[0]=12u;goto Lc905c74; /* mov #0xc,r0 */
Lc905c74: *stop=0xc905c74u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[13]);goto Lc905c76; /* fmov.s @(r0,r13),fr1 */
Lc905c76: *stop=0xc905c76u;r[0]=4294967176u;goto Lc905c78; /* mov #-0x78,r0 */
Lc905c78: *stop=0xc905c78u;r[0]=r[0]&255u;goto Lc905c7a; /* extu.b r0,r0 */
Lc905c7a: *stop=0xc905c7au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[15]);goto Lc905c7c; /* fmov fr1,@(r0,r15) */
Lc905c7c: *stop=0xc905c7cu;r[0]=20u;goto Lc905c7e; /* mov #0x14,r0 */
Lc905c7e: *stop=0xc905c7eu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[13]);goto Lc905c80; /* fmov.s @(r0,r13),fr1 */
Lc905c80: *stop=0xc905c80u;r[0]=4294967180u;goto Lc905c82; /* mov #-0x74,r0 */
Lc905c82: *stop=0xc905c82u;r[3]=4294967168u;goto Lc905c84; /* mov #-0x80,r3 */
Lc905c84: *stop=0xc905c84u;r[0]=r[0]&255u;goto Lc905c86; /* extu.b r0,r0 */
Lc905c86: *stop=0xc905c86u;r[3]=r[3]&255u;goto Lc905c88; /* extu.b r3,r3 */
Lc905c88: *stop=0xc905c88u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[15]);goto Lc905c8a; /* fmov fr1,@(r0,r15) */
Lc905c8a: *stop=0xc905c8au;r[0]=124u;goto Lc905c8c; /* mov #0x7c,r0 */
Lc905c8c: *stop=0xc905c8cu;r[3]+=r[15];goto Lc905c8e; /* add r15,r3 */
Lc905c8e: *stop=0xc905c8eu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc905c90; /* mov.l r3,@(r0,r15) */
Lc905c90: *stop=0xc905c90u;r[0]=2u;goto Lc905c92; /* mov #0x2,r0 */
Lc905c92: *stop=0xc905c92u;r[0]<<=8;goto Lc905c94; /* shll8 r0 */
Lc905c94: *stop=0xc905c94u;r[0]+=72u;goto Lc905c96; /* add #0x48,r0 */
Lc905c96: *stop=0xc905c96u;if(!valid_ram(r[0]+r[14],4u))return -3;r[3]=rd(m,r[14]+r[0],4);goto Lc905c98; /* mov.l @(r0,r14),r3 */
Lc905c98: *stop=0xc905c98u;r[0]=120u;goto Lc905c9a; /* mov #0x78,r0 */
Lc905c9a: *stop=0xc905c9au;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc905c9c; /* mov.l r3,@(r0,r15) */
Lc905c9c: *stop=0xc905c9cu;r[0]=24u;goto Lc905c9e; /* mov #0x18,r0 */
Lc905c9e: *stop=0xc905c9eu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905ca0; /* fmov.s @(r0,r13),fr4 */
Lc905ca0: *stop=0xc905ca0u;r[3]=rd(m,0x0c905e60u,4);goto Lc905ca2; /* mov.l 0x0c905e60,r3 */
Lc905ca2: *stop=0xc905ca2u;r[4]=r[15];goto Lc905ca4; /* mov r15,r4 */
Lc905ca4: target=r[3];*pr=0x0c905ca8u;*stop=0xc905ca6u;r[4]+=120u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905ca8; /* jsr @r3 */
Lc905ca8: *stop=0xc905ca8u;r[0]=96u;goto Lc905caa; /* mov #0x60,r0 */
Lc905caa: *stop=0xc905caau;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905cac; /* mov.l @(r0,r14),r0 */
Lc905cac: *stop=0xc905cacu;*sr=(*sr&~1u)|!(r[0]&0x4u);goto Lc905cae; /* tst #0x4,r0 */
Lc905cae: if(!(*sr&1u))goto Lc905d7a;goto Lc905cb0; /* bf 0x0c905d7a */
Lc905cb0: *stop=0xc905cb0u;r[3]=rd(m,0x0c905e64u,4);goto Lc905cb2; /* mov.l 0x0c905e64,r3 */
Lc905cb2: *stop=0xc905cb2u;r[2]=4294967172u;goto Lc905cb4; /* mov #-0x7c,r2 */
Lc905cb4: *stop=0xc905cb4u;r[2]=r[2]&255u;goto Lc905cb6; /* extu.b r2,r2 */
Lc905cb6: *stop=0xc905cb6u;fcopy(f,13u,12u);goto Lc905cb8; /* fmov fr12,fr13 */
Lc905cb8: *stop=0xc905cb8u;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto Lc905cba; /* mov.l @(0x4,r3),r0 */
Lc905cba: *stop=0xc905cbau;r[2]+=r[15];goto Lc905cbc; /* add r15,r2 */
Lc905cbc: *stop=0xc905cbcu;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto Lc905cbe; /* mov.l @r3,r1 */
Lc905cbe: *stop=0xc905cbeu;r[5]=r[15];goto Lc905cc0; /* mov r15,r5 */
Lc905cc0: *stop=0xc905cc0u;r[7]=r[15];goto Lc905cc2; /* mov r15,r7 */
Lc905cc2: *stop=0xc905cc2u;r[5]+=124u;goto Lc905cc4; /* add #0x7c,r5 */
Lc905cc4: *stop=0xc905cc4u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc905cc6; /* mov.l r1,@r2 */
Lc905cc6: *stop=0xc905cc6u;r[7]+=60u;goto Lc905cc8; /* add #0x3c,r7 */
Lc905cc8: *stop=0xc905cc8u;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto Lc905cca; /* mov.l r0,@(0x4,r2) */
Lc905cca: *stop=0xc905ccau;r[0]=24u;goto Lc905ccc; /* mov #0x18,r0 */
Lc905ccc: *stop=0xc905cccu;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto Lc905cce; /* mov.l @(0x8,r3),r1 */
Lc905cce: *stop=0xc905cceu;r[6]=32u;goto Lc905cd0; /* mov #0x20,r6 */
Lc905cd0: *stop=0xc905cd0u;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto Lc905cd2; /* mov.l r1,@(0x8,r2) */
Lc905cd2: *stop=0xc905cd2u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]+r[13]);goto Lc905cd4; /* fmov.s @(r0,r13),fr14 */
Lc905cd4: *stop=0xc905cd4u;r[0]=2u;goto Lc905cd6; /* mov #0x2,r0 */
Lc905cd6: *stop=0xc905cd6u;r[0]<<=8;goto Lc905cd8; /* shll8 r0 */
Lc905cd8: *stop=0xc905cd8u;r[0]+=72u;goto Lc905cda; /* add #0x48,r0 */
Lc905cda: *stop=0xc905cdau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[14],&f->fpscr,&fr[13],1);if(status)return status;}goto Lc905cdc; /* fsub fr14,fr13 */
Lc905cdc: *stop=0xc905cdcu;if(!valid_ram(r[0]+r[14],4u))return -3;r[2]=rd(m,r[14]+r[0],4);goto Lc905cde; /* mov.l @(r0,r14),r2 */
Lc905cde: *stop=0xc905cdeu;r[0]=124u;goto Lc905ce0; /* mov #0x7c,r0 */
Lc905ce0: *stop=0xc905ce0u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc905ce2; /* mov.l r2,@(r0,r15) */
Lc905ce2: *stop=0xc905ce2u;r[0]=4294967168u;goto Lc905ce4; /* mov #-0x80,r0 */
Lc905ce4: *stop=0xc905ce4u;if(!valid_ram(r[11],4u))return -3;r[1]=rd(m,r[11],4);goto Lc905ce6; /* mov.l @r11,r1 */
Lc905ce6: *stop=0xc905ce6u;r[0]=r[0]&255u;goto Lc905ce8; /* extu.b r0,r0 */
Lc905ce8: *stop=0xc905ce8u;r[2]=rd(m,0x0c905e68u,4);goto Lc905cea; /* mov.l 0x0c905e68,r2 */
Lc905cea: *stop=0xc905ceau;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[1],4);goto Lc905cec; /* mov.l r1,@(r0,r15) */
Lc905cec: *stop=0xc905cecu;r[0]=12u;goto Lc905cee; /* mov #0xc,r0 */
Lc905cee: *stop=0xc905ceeu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905cf0; /* fmov.s @(r0,r13),fr4 */
Lc905cf0: target=r[2];*pr=0x0c905cf4u;*stop=0xc905cf2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905cf4; /* jsr @r2 */
Lc905cf4: *stop=0xc905cf4u;r[5]=4294967172u;goto Lc905cf6; /* mov #-0x7c,r5 */
Lc905cf6: *stop=0xc905cf6u;r[3]=rd(m,0x0c905e6cu,4);goto Lc905cf8; /* mov.l 0x0c905e6c,r3 */
Lc905cf8: *stop=0xc905cf8u;r[5]=r[5]&255u;goto Lc905cfa; /* extu.b r5,r5 */
Lc905cfa: *stop=0xc905cfau;r[6]=r[15];goto Lc905cfc; /* mov r15,r6 */
Lc905cfc: *stop=0xc905cfcu;r[4]=r[15];goto Lc905cfe; /* mov r15,r4 */
Lc905cfe: *stop=0xc905cfeu;r[6]+=48u;goto Lc905d00; /* add #0x30,r6 */
Lc905d00: *stop=0xc905d00u;r[5]+=r[15];goto Lc905d02; /* add r15,r5 */
Lc905d02: target=r[3];*pr=0x0c905d06u;*stop=0xc905d04u;r[4]+=60u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905d06; /* jsr @r3 */
Lc905d06: *stop=0xc905d06u;r[0]=4294967168u;goto Lc905d08; /* mov #-0x80,r0 */
Lc905d08: *stop=0xc905d08u;if(!valid_ram(r[9],4u))return -3;r[2]=rd(m,r[9],4);goto Lc905d0a; /* mov.l @r9,r2 */
Lc905d0a: *stop=0xc905d0au;r[0]=r[0]&255u;goto Lc905d0c; /* extu.b r0,r0 */
Lc905d0c: *stop=0xc905d0cu;r[5]=r[15];goto Lc905d0e; /* mov r15,r5 */
Lc905d0e: *stop=0xc905d0eu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc905d10; /* mov.l r2,@(r0,r15) */
Lc905d10: *stop=0xc905d10u;r[0]=20u;goto Lc905d12; /* mov #0x14,r0 */
Lc905d12: *stop=0xc905d12u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905d14; /* fmov.s @(r0,r13),fr4 */
Lc905d14: *stop=0xc905d14u;r[7]=r[15];goto Lc905d16; /* mov r15,r7 */
Lc905d16: *stop=0xc905d16u;r[3]=rd(m,0x0c905e68u,4);goto Lc905d18; /* mov.l 0x0c905e68,r3 */
Lc905d18: *stop=0xc905d18u;r[5]+=124u;goto Lc905d1a; /* add #0x7c,r5 */
Lc905d1a: *stop=0xc905d1au;r[7]+=60u;goto Lc905d1c; /* add #0x3c,r7 */
Lc905d1c: *stop=0xc905d1cu;r[6]=32u;goto Lc905d1e; /* mov #0x20,r6 */
Lc905d1e: target=r[3];*pr=0x0c905d22u;*stop=0xc905d20u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905d22; /* jsr @r3 */
Lc905d22: *stop=0xc905d22u;r[5]=4294967172u;goto Lc905d24; /* mov #-0x7c,r5 */
Lc905d24: *stop=0xc905d24u;r[2]=rd(m,0x0c905e6cu,4);goto Lc905d26; /* mov.l 0x0c905e6c,r2 */
Lc905d26: *stop=0xc905d26u;r[5]=r[5]&255u;goto Lc905d28; /* extu.b r5,r5 */
Lc905d28: *stop=0xc905d28u;r[6]=r[15];goto Lc905d2a; /* mov r15,r6 */
Lc905d2a: *stop=0xc905d2au;r[4]=r[15];goto Lc905d2c; /* mov r15,r4 */
Lc905d2c: *stop=0xc905d2cu;r[6]+=36u;goto Lc905d2e; /* add #0x24,r6 */
Lc905d2e: *stop=0xc905d2eu;r[5]+=r[15];goto Lc905d30; /* add r15,r5 */
Lc905d30: target=r[2];*pr=0x0c905d34u;*stop=0xc905d32u;r[4]+=60u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905d34; /* jsr @r2 */
Lc905d34: *stop=0xc905d34u;r[0]=36u;goto Lc905d36; /* mov #0x24,r0 */
Lc905d36: *stop=0xc905d36u;fcopy(f,0u,13u);goto Lc905d38; /* fmov fr13,fr0 */
Lc905d38: *stop=0xc905d38u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc905d3a; /* fmov.s @(r0,r15),fr3 */
Lc905d3a: *stop=0xc905d3au;r[0]=48u;goto Lc905d3c; /* mov #0x30,r0 */
Lc905d3c: *stop=0xc905d3cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto Lc905d3e; /* fmov.s @(r0,r15),fr2 */
Lc905d3e: *stop=0xc905d3eu;r[0]=8u;goto Lc905d40; /* mov #0x8,r0 */
Lc905d40: *stop=0xc905d40u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905d42; /* fmul fr14,fr3 */
Lc905d42: *stop=0xc905d42u;r[0]<<=8;goto Lc905d44; /* shll8 r0 */
Lc905d44: *stop=0xc905d44u;r[0]+=28u;goto Lc905d46; /* add #0x1c,r0 */
Lc905d46: *stop=0xc905d46u;r[5]=r[14];goto Lc905d48; /* mov r14,r5 */
Lc905d48: *stop=0xc905d48u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905d4a; /* fmac fr0,fr2,fr3 */
Lc905d4a: *stop=0xc905d4au;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto Lc905d4c; /* fmov fr3,@(r0,r14) */
Lc905d4c: *stop=0xc905d4cu;r[0]=40u;goto Lc905d4e; /* mov #0x28,r0 */
Lc905d4e: *stop=0xc905d4eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc905d50; /* fmov.s @(r0,r15),fr3 */
Lc905d50: *stop=0xc905d50u;r[0]=52u;goto Lc905d52; /* mov #0x34,r0 */
Lc905d52: *stop=0xc905d52u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto Lc905d54; /* fmov.s @(r0,r15),fr2 */
Lc905d54: *stop=0xc905d54u;r[0]=8u;goto Lc905d56; /* mov #0x8,r0 */
Lc905d56: *stop=0xc905d56u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905d58; /* fmul fr14,fr3 */
Lc905d58: *stop=0xc905d58u;r[0]<<=8;goto Lc905d5a; /* shll8 r0 */
Lc905d5a: *stop=0xc905d5au;r[0]+=32u;goto Lc905d5c; /* add #0x20,r0 */
Lc905d5c: *stop=0xc905d5cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905d5e; /* fmac fr0,fr2,fr3 */
Lc905d5e: *stop=0xc905d5eu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto Lc905d60; /* fmov fr3,@(r0,r14) */
Lc905d60: *stop=0xc905d60u;r[0]=44u;goto Lc905d62; /* mov #0x2c,r0 */
Lc905d62: *stop=0xc905d62u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc905d64; /* fmov.s @(r0,r15),fr3 */
Lc905d64: *stop=0xc905d64u;r[0]=56u;goto Lc905d66; /* mov #0x38,r0 */
Lc905d66: *stop=0xc905d66u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto Lc905d68; /* fmov.s @(r0,r15),fr2 */
Lc905d68: *stop=0xc905d68u;r[0]=8u;goto Lc905d6a; /* mov #0x8,r0 */
Lc905d6a: *stop=0xc905d6au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905d6c; /* fmul fr14,fr3 */
Lc905d6c: *stop=0xc905d6cu;r[0]<<=8;goto Lc905d6e; /* shll8 r0 */
Lc905d6e: *stop=0xc905d6eu;r[0]+=36u;goto Lc905d70; /* add #0x24,r0 */
Lc905d70: *stop=0xc905d70u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905d72; /* fmac fr0,fr2,fr3 */
Lc905d72: *stop=0xc905d72u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto Lc905d74; /* fmov fr3,@(r0,r14) */
Lc905d74: *stop=0xc905d74u;r[3]=rd(m,0x0c905e70u,4);goto Lc905d76; /* mov.l 0x0c905e70,r3 */
Lc905d76: target=r[3];*pr=0x0c905d7au;*stop=0xc905d78u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905d7a; /* jsr @r3 */
Lc905d7a: *stop=0xc905d7au;r[1]=20u;goto Lc905d7c; /* mov #0x14,r1 */
Lc905d7c: *stop=0xc905d7cu;r[1]+=r[9];goto Lc905d7e; /* add r9,r1 */
Lc905d7e: *stop=0xc905d7eu;r[0]=24u;goto Lc905d80; /* mov #0x18,r0 */
Lc905d80: *stop=0xc905d80u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto Lc905d82; /* fmov.s @r1,fr3 */
Lc905d82: *stop=0xc905d82u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[13]);goto Lc905d84; /* fmov.s @(r0,r13),fr2 */
Lc905d84: *stop=0xc905d84u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc905d86; /* fadd fr3,fr2 */
Lc905d86: *stop=0xc905d86u;fcopy(f,3u,2u);goto Lc905d88; /* fmov fr2,fr3 */
Lc905d88: *stop=0xc905d88u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,sr,1);if(status)return status;goto Lc905d8a; /* fcmp/gt fr12,fr3 */
Lc905d8a: take=(*sr&1u);*stop=0xc905d8cu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[13]);if(take)goto Lc905d92;goto Lc905d8e; /* bt/s 0x0c905d92 */
Lc905d8e: *stop=0xc905d90u;goto Lc905fcc; /* bra 0x0c905fcc */
Lc905d92: *stop=0xc905d92u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,15u,r[0]+r[13]);goto Lc905d94; /* fmov fr15,@(r0,r13) */
Lc905d94: *stop=0xc905d94u;r[0]=20u;goto Lc905d96; /* mov #0x14,r0 */
Lc905d96: *stop=0xc905d96u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[13]);goto Lc905d98; /* fmov.s @(r0,r13),fr3 */
Lc905d98: *stop=0xc905d98u;r[0]=12u;goto Lc905d9a; /* mov #0xc,r0 */
Lc905d9a: *stop=0xc905d9au;r[3]=rd(m,0x0c905e74u,4);goto Lc905d9c; /* mov.l 0x0c905e74,r3 */
Lc905d9c: *stop=0xc905d9cu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[13]);goto Lc905d9e; /* fmov fr3,@(r0,r13) */
Lc905d9e: *stop=0xc905d9eu;if(!valid_ram(r[13]+8u,2u))return -3;r[0]=sx(m,r[13]+0x8u,2);goto Lc905da0; /* mov.w @(0x8,r13),r0 */
Lc905da0: *stop=0xc905da0u;if(!valid_ram(r[13]+6u,2u))return -3;wr(m,r[13]+0x6u,r[0],2);goto Lc905da2; /* mov.w r0,@(0x6,r13) */
Lc905da2: *stop=0xc905da2u;if(!valid_ram(r[9]+4u,2u))return -3;r[0]=sx(m,r[9]+0x4u,2);goto Lc905da4; /* mov.w @(0x4,r9),r0 */
Lc905da4: *stop=0xc905da4u;if(!valid_ram(r[13],2u))return -3;wr(m,r[13],r[0],2);goto Lc905da6; /* mov.w r0,@r13 */
Lc905da6: *stop=0xc905da6u;if(!valid_ram(r[13]+2u,2u))return -3;r[0]=sx(m,r[13]+0x2u,2);goto Lc905da8; /* mov.w @(0x2,r13),r0 */
Lc905da8: *stop=0xc905da8u;r[0]&=r[3];goto Lc905daa; /* and r3,r0 */
Lc905daa: *stop=0xc905daau;if(!valid_ram(r[13]+2u,2u))return -3;wr(m,r[13]+0x2u,r[0],2);goto Lc905dac; /* mov.w r0,@(0x2,r13) */
Lc905dac: *stop=0xc905dacu;if(!valid_ram(r[13]+2u,2u))return -3;r[0]=sx(m,r[13]+0x2u,2);goto Lc905dae; /* mov.w @(0x2,r13),r0 */
Lc905dae: *stop=0xc905daeu;r[0]|=0x8u;goto Lc905db0; /* or #0x8,r0 */
Lc905db0: *stop=0xc905db0u;if(!valid_ram(r[13]+2u,2u))return -3;wr(m,r[13]+0x2u,r[0],2);goto Lc905db2; /* mov.w r0,@(0x2,r13) */
Lc905db2: *stop=0xc905db2u;r[0]=28u;goto Lc905db4; /* mov #0x1c,r0 */
Lc905db4: *stop=0xc905db6u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,12u,r[0]+r[13]);goto Lc905fcc; /* bra 0x0c905fcc */
Lc905db8: *stop=0xc905db8u;r[0]=96u;goto Lc905dba; /* mov #0x60,r0 */
Lc905dba: *stop=0xc905dbau;if(!valid_ram(r[0]+r[14],4u))return -3;r[1]=rd(m,r[14]+r[0],4);goto Lc905dbc; /* mov.l @(r0,r14),r1 */
Lc905dbc: *stop=0xc905dbcu;*sr=(*sr&~1u)|!(r[1]&r[4]);goto Lc905dbe; /* tst r1,r4 */
Lc905dbe: if((*sr&1u))goto Lc905dc4;goto Lc905dc0; /* bt 0x0c905dc4 */
Lc905dc0: *stop=0xc905dc2u;fcopy(f,4u,15u);goto Lc905dd2; /* bra 0x0c905dd2 */
Lc905dc4: *stop=0xc905dc4u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto Lc905dc6; /* mov.l @r15,r5 */
Lc905dc6: *stop=0xc905dc6u;r[4]=r[12];goto Lc905dc8; /* mov r12,r4 */
Lc905dc8: *stop=0xc905dc8u;r[3]=rd(m,0x0c905e78u,4);goto Lc905dca; /* mov.l 0x0c905e78,r3 */
Lc905dca: *stop=0xc905dcau;r[5]+=32u;goto Lc905dcc; /* add #0x20,r5 */
Lc905dcc: target=r[3];*pr=0x0c905dd0u;*stop=0xc905dceu;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905dd0; /* jsr @r3 */
Lc905dd0: *stop=0xc905dd0u;fcopy(f,4u,0u);goto Lc905dd2; /* fmov fr0,fr4 */
Lc905dd2: *stop=0xc905dd2u;r[0]=4294967276u;goto Lc905dd4; /* mov #-0x14,r0 */
Lc905dd4: *stop=0xc905dd4u;r[0]=r[0]&255u;goto Lc905dd6; /* extu.b r0,r0 */
Lc905dd6: *stop=0xc905dd6u;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905dd8; /* mov.l @(r0,r14),r0 */
Lc905dd8: *stop=0xc905dd8u;r[1]=54u;goto Lc905dda; /* mov #0x36,r1 */
Lc905dda: *stop=0xc905ddau;if(!valid_ram(r[0]+r[1],2u))return -3;r[0]=sx(m,r[0]+r[1],2);goto Lc905ddc; /* mov.w @(r0,r1),r0 */
Lc905ddc: *stop=0xc905ddcu;r[0]=r[0]&65535u;goto Lc905dde; /* extu.w r0,r0 */
Lc905dde: *stop=0xc905ddeu;*sr=(*sr&~1u)|!(r[0]&0x40u);goto Lc905de0; /* tst #0x40,r0 */
Lc905de0: if((*sr&1u))goto Lc905df8;goto Lc905de2; /* bt 0x0c905df8 */
Lc905de2: *stop=0xc905de2u;r[0]=12u;goto Lc905de4; /* mov #0xc,r0 */
Lc905de4: *stop=0xc905de4u;r[3]=rd(m,0x0c905e7cu,4);goto Lc905de6; /* mov.l 0x0c905e7c,r3 */
Lc905de6: *stop=0xc905de6u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905de8; /* fmov.s @(r0,r13),fr4 */
Lc905de8: *stop=0xc905de8u;r[0]=2u;goto Lc905dea; /* mov #0x2,r0 */
Lc905dea: *stop=0xc905deau;r[0]<<=8;goto Lc905dec; /* shll8 r0 */
Lc905dec: *stop=0xc905decu;if(!valid_ram(r[11],4u))return -3;r[5]=rd(m,r[11],4);goto Lc905dee; /* mov.l @r11,r5 */
Lc905dee: *stop=0xc905deeu;r[0]+=72u;goto Lc905df0; /* add #0x48,r0 */
Lc905df0: target=r[3];*pr=0x0c905df4u;*stop=0xc905df2u;if(!valid_ram(r[0]+r[14],4u))return -3;r[4]=rd(m,r[14]+r[0],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905df4; /* jsr @r3 */
Lc905df4: *stop=0xc905df6u;goto Lc905f64; /* bra 0x0c905f64 */
Lc905df8: *stop=0xc905df8u;r[0]=4294967276u;goto Lc905dfa; /* mov #-0x14,r0 */
Lc905dfa: *stop=0xc905dfau;r[0]=r[0]&255u;goto Lc905dfc; /* extu.b r0,r0 */
Lc905dfc: *stop=0xc905dfcu;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905dfe; /* mov.l @(r0,r14),r0 */
Lc905dfe: *stop=0xc905dfeu;r[2]=54u;goto Lc905e00; /* mov #0x36,r2 */
Lc905e00: *stop=0xc905e00u;if(!valid_ram(r[0]+r[2],2u))return -3;r[0]=sx(m,r[0]+r[2],2);goto Lc905e02; /* mov.w @(r0,r2),r0 */
Lc905e02: *stop=0xc905e02u;r[0]=r[0]&65535u;goto Lc905e04; /* extu.w r0,r0 */
Lc905e04: *stop=0xc905e04u;*sr=(*sr&~1u)|!(r[0]&0xcu);goto Lc905e06; /* tst #0xc,r0 */
Lc905e06: if((*sr&1u))goto Lc905e80;goto Lc905e08; /* bt 0x0c905e80 */
Lc905e08: *stop=0xc905e08u;r[0]=4294967276u;goto Lc905e0a; /* mov #-0x14,r0 */
Lc905e0a: *stop=0xc905e0au;r[0]=r[0]&255u;goto Lc905e0c; /* extu.b r0,r0 */
Lc905e0c: *stop=0xc905e0cu;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905e0e; /* mov.l @(r0,r14),r0 */
Lc905e0e: *stop=0xc905e0eu;r[1]=54u;goto Lc905e10; /* mov #0x36,r1 */
Lc905e10: *stop=0xc905e10u;if(!valid_ram(r[0]+r[1],2u))return -3;r[0]=sx(m,r[0]+r[1],2);goto Lc905e12; /* mov.w @(r0,r1),r0 */
Lc905e12: *stop=0xc905e12u;r[0]=r[0]&65535u;goto Lc905e14; /* extu.w r0,r0 */
Lc905e14: *stop=0xc905e14u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto Lc905e16; /* tst #0x2,r0 */
Lc905e16: if((*sr&1u))goto Lc905e2c;goto Lc905e18; /* bt 0x0c905e2c */
Lc905e18: *stop=0xc905e18u;r[0]=12u;goto Lc905e1a; /* mov #0xc,r0 */
Lc905e1a: *stop=0xc905e1au;if(!valid_ram(r[11],4u))return -3;r[5]=rd(m,r[11],4);goto Lc905e1c; /* mov.l @r11,r5 */
Lc905e1c: *stop=0xc905e1cu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905e1e; /* fmov.s @(r0,r13),fr4 */
Lc905e1e: *stop=0xc905e1eu;r[0]=2u;goto Lc905e20; /* mov #0x2,r0 */
Lc905e20: *stop=0xc905e20u;r[0]<<=8;goto Lc905e22; /* shll8 r0 */
Lc905e22: *stop=0xc905e22u;r[0]+=72u;goto Lc905e24; /* add #0x48,r0 */
Lc905e24: target=r[8];*pr=0x0c905e28u;*stop=0xc905e26u;if(!valid_ram(r[0]+r[14],4u))return -3;r[4]=rd(m,r[14]+r[0],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905e28; /* jsr @r8 */
Lc905e28: *stop=0xc905e2au;goto Lc905f64; /* bra 0x0c905f64 */
Lc905e2c: *stop=0xc905e2cu;r[0]=12u;goto Lc905e2e; /* mov #0xc,r0 */
Lc905e2e: *stop=0xc905e2eu;r[3]=rd(m,0x0c905e7cu,4);goto Lc905e30; /* mov.l 0x0c905e7c,r3 */
Lc905e30: *stop=0xc905e30u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905e32; /* fmov.s @(r0,r13),fr4 */
Lc905e32: *stop=0xc905e32u;r[0]=2u;goto Lc905e34; /* mov #0x2,r0 */
Lc905e34: *stop=0xc905e34u;r[0]<<=8;goto Lc905e36; /* shll8 r0 */
Lc905e36: *stop=0xc905e36u;if(!valid_ram(r[11],4u))return -3;r[5]=rd(m,r[11],4);goto Lc905e38; /* mov.l @r11,r5 */
Lc905e38: *stop=0xc905e38u;r[0]+=72u;goto Lc905e3a; /* add #0x48,r0 */
Lc905e3a: target=r[3];*pr=0x0c905e3eu;*stop=0xc905e3cu;if(!valid_ram(r[0]+r[14],4u))return -3;r[4]=rd(m,r[14]+r[0],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905e3e; /* jsr @r3 */
Lc905e3e: *stop=0xc905e40u;goto Lc905f64; /* bra 0x0c905f64 */
Lc905e80: *stop=0xc905e80u;r[0]=0x0c9060a0u;goto Lc905e82; /* mova 0x0c9060a0,r0 */
Lc905e82: *stop=0xc905e82u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc905e84; /* fmov.s @r0,fr3 */
Lc905e84: *stop=0xc905e84u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc905e86; /* fcmp/gt fr3,fr4 */
Lc905e86: if(!(*sr&1u))goto Lc905ee2;goto Lc905e88; /* bf 0x0c905ee2 */
Lc905e88: *stop=0xc905e88u;r[0]=9u;goto Lc905e8a; /* mov #0x9,r0 */
Lc905e8a: *stop=0xc905e8au;fcopy(f,1u,13u);goto Lc905e8c; /* fmov fr13,fr1 */
Lc905e8c: *stop=0xc905e8cu;r[0]<<=8;goto Lc905e8e; /* shll8 r0 */
Lc905e8e: *stop=0xc905e8eu;r[0]+=4294967238u;goto Lc905e90; /* add #-0x3a,r0 */
Lc905e90: *stop=0xc905e90u;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905e92; /* mov.b @(r0,r14),r3 */
Lc905e92: *stop=0xc905e92u;r[0]=4u;goto Lc905e94; /* mov #0x4,r0 */
Lc905e94: *stop=0xc905e94u;r[3]=r[3]&255u;goto Lc905e96; /* extu.b r3,r3 */
Lc905e96: *stop=0xc905e96u;*fpul=r[3];goto Lc905e98; /* lds r3,FPUL */
Lc905e98: *stop=0xc905e98u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc905e9a; /* float FPUL,fr2 */
Lc905e9a: *stop=0xc905e9au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[13],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905e9c; /* fmul fr13,fr2 */
Lc905e9c: *stop=0xc905e9cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto Lc905e9e; /* fmov fr2,@(r0,r15) */
Lc905e9e: *stop=0xc905e9eu;r[0]=9u;goto Lc905ea0; /* mov #0x9,r0 */
Lc905ea0: *stop=0xc905ea0u;r[0]<<=8;goto Lc905ea2; /* shll8 r0 */
Lc905ea2: *stop=0xc905ea2u;r[0]+=4294967237u;goto Lc905ea4; /* add #-0x3b,r0 */
Lc905ea4: *stop=0xc905ea4u;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905ea6; /* mov.b @(r0,r14),r3 */
Lc905ea6: *stop=0xc905ea6u;r[0]=8u;goto Lc905ea8; /* mov #0x8,r0 */
Lc905ea8: *stop=0xc905ea8u;r[3]=r[3]&255u;goto Lc905eaa; /* extu.b r3,r3 */
Lc905eaa: *stop=0xc905eaau;*fpul=r[3];goto Lc905eac; /* lds r3,FPUL */
Lc905eac: *stop=0xc905eacu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc905eae; /* float FPUL,fr2 */
Lc905eae: *stop=0xc905eaeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[13],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905eb0; /* fmul fr13,fr2 */
Lc905eb0: *stop=0xc905eb0u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto Lc905eb2; /* fmov fr2,@(r0,r15) */
Lc905eb2: *stop=0xc905eb2u;r[0]=9u;goto Lc905eb4; /* mov #0x9,r0 */
Lc905eb4: *stop=0xc905eb4u;r[0]<<=8;goto Lc905eb6; /* shll8 r0 */
Lc905eb6: *stop=0xc905eb6u;r[0]+=4294967236u;goto Lc905eb8; /* add #-0x3c,r0 */
Lc905eb8: *stop=0xc905eb8u;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905eba; /* mov.b @(r0,r14),r3 */
Lc905eba: *stop=0xc905ebau;r[3]=r[3]&255u;goto Lc905ebc; /* extu.b r3,r3 */
Lc905ebc: *stop=0xc905ebcu;*fpul=r[3];goto Lc905ebe; /* lds r3,FPUL */
Lc905ebe: *stop=0xc905ebeu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc905ec0; /* float FPUL,fr2 */
Lc905ec0: *stop=0xc905ec0u;fcopy(f,13u,2u);goto Lc905ec2; /* fmov fr2,fr13 */
Lc905ec2: *stop=0xc905ec2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[1],&f->fpscr,&fr[13]);if(status)return status;}goto Lc905ec4; /* fmul fr1,fr13 */
Lc905ec4: *stop=0xc905ec4u;fcopy(f,7u,13u);goto Lc905ec6; /* fmov fr13,fr7 */
Lc905ec6: *stop=0xc905ec6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[14],&f->fpscr,&fr[7],0);if(status)return status;}goto Lc905ec8; /* fadd fr14,fr7 */
Lc905ec8: *stop=0xc905ec8u;r[0]=8u;goto Lc905eca; /* mov #0x8,r0 */
Lc905eca: *stop=0xc905ecau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[15]);goto Lc905ecc; /* fmov.s @(r0,r15),fr6 */
Lc905ecc: *stop=0xc905eccu;r[0]=4u;goto Lc905ece; /* mov #0x4,r0 */
Lc905ece: *stop=0xc905eceu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[15]);goto Lc905ed0; /* fmov.s @(r0,r15),fr5 */
Lc905ed0: *stop=0xc905ed0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[14],&f->fpscr,&fr[6],0);if(status)return status;}goto Lc905ed2; /* fadd fr14,fr6 */
Lc905ed2: *stop=0xc905ed2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[14],&f->fpscr,&fr[5],0);if(status)return status;}goto Lc905ed4; /* fadd fr14,fr5 */
Lc905ed4: target=r[10];*pr=0x0c905ed8u;*stop=0xc905ed6u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905ed8; /* jsr @r10 */
Lc905ed8: *stop=0xc905ed8u;r[0]=12u;goto Lc905eda; /* mov #0xc,r0 */
Lc905eda: *stop=0xc905edau;r[4]=rd(m,0x0c9060a4u,4);goto Lc905edc; /* mov.l 0x0c9060a4,r4 */
Lc905edc: *stop=0xc905edcu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905ede; /* fmov.s @(r0,r13),fr4 */
Lc905ede: *stop=0xc905ee0u;if(!valid_ram(r[11],4u))return -3;r[5]=rd(m,r[11],4);goto Lc905f42; /* bra 0x0c905f42 */
Lc905ee2: *stop=0xc905ee2u;r[0]=0x0c9060a8u;goto Lc905ee4; /* mova 0x0c9060a8,r0 */
Lc905ee4: *stop=0xc905ee4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto Lc905ee6; /* fmov.s @r0,fr2 */
Lc905ee6: *stop=0xc905ee6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto Lc905ee8; /* fcmp/gt fr2,fr4 */
Lc905ee8: if(!(*sr&1u))goto Lc905f54;goto Lc905eea; /* bf 0x0c905f54 */
Lc905eea: *stop=0xc905eeau;r[0]=9u;goto Lc905eec; /* mov #0x9,r0 */
Lc905eec: *stop=0xc905eecu;r[0]<<=8;goto Lc905eee; /* shll8 r0 */
Lc905eee: *stop=0xc905eeeu;r[0]+=4294967238u;goto Lc905ef0; /* add #-0x3a,r0 */
Lc905ef0: *stop=0xc905ef0u;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905ef2; /* mov.b @(r0,r14),r3 */
Lc905ef2: *stop=0xc905ef2u;r[0]=12u;goto Lc905ef4; /* mov #0xc,r0 */
Lc905ef4: *stop=0xc905ef4u;r[3]=r[3]&255u;goto Lc905ef6; /* extu.b r3,r3 */
Lc905ef6: *stop=0xc905ef6u;*fpul=r[3];goto Lc905ef8; /* lds r3,FPUL */
Lc905ef8: *stop=0xc905ef8u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto Lc905efa; /* float FPUL,fr1 */
Lc905efa: *stop=0xc905efau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[13],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905efc; /* fmul fr13,fr1 */
Lc905efc: *stop=0xc905efcu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[15]);goto Lc905efe; /* fmov fr1,@(r0,r15) */
Lc905efe: *stop=0xc905efeu;r[0]=9u;goto Lc905f00; /* mov #0x9,r0 */
Lc905f00: *stop=0xc905f00u;r[0]<<=8;goto Lc905f02; /* shll8 r0 */
Lc905f02: *stop=0xc905f02u;fcopy(f,1u,13u);goto Lc905f04; /* fmov fr13,fr1 */
Lc905f04: *stop=0xc905f04u;r[0]+=4294967237u;goto Lc905f06; /* add #-0x3b,r0 */
Lc905f06: *stop=0xc905f06u;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905f08; /* mov.b @(r0,r14),r3 */
Lc905f08: *stop=0xc905f08u;r[0]=32u;goto Lc905f0a; /* mov #0x20,r0 */
Lc905f0a: *stop=0xc905f0au;r[3]=r[3]&255u;goto Lc905f0c; /* extu.b r3,r3 */
Lc905f0c: *stop=0xc905f0cu;*fpul=r[3];goto Lc905f0e; /* lds r3,FPUL */
Lc905f0e: *stop=0xc905f0eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc905f10; /* float FPUL,fr3 */
Lc905f10: *stop=0xc905f10u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[13],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905f12; /* fmul fr13,fr3 */
Lc905f12: *stop=0xc905f12u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc905f14; /* fmov fr3,@(r0,r15) */
Lc905f14: *stop=0xc905f14u;r[0]=9u;goto Lc905f16; /* mov #0x9,r0 */
Lc905f16: *stop=0xc905f16u;r[0]<<=8;goto Lc905f18; /* shll8 r0 */
Lc905f18: *stop=0xc905f18u;r[0]+=4294967236u;goto Lc905f1a; /* add #-0x3c,r0 */
Lc905f1a: *stop=0xc905f1au;if(!valid_ram(r[0]+r[14],1u))return -3;r[3]=sx(m,r[14]+r[0],1);goto Lc905f1c; /* mov.b @(r0,r14),r3 */
Lc905f1c: *stop=0xc905f1cu;r[3]=r[3]&255u;goto Lc905f1e; /* extu.b r3,r3 */
Lc905f1e: *stop=0xc905f1eu;*fpul=r[3];goto Lc905f20; /* lds r3,FPUL */
Lc905f20: *stop=0xc905f20u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc905f22; /* float FPUL,fr3 */
Lc905f22: *stop=0xc905f22u;fcopy(f,13u,3u);goto Lc905f24; /* fmov fr3,fr13 */
Lc905f24: *stop=0xc905f24u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[1],&f->fpscr,&fr[13]);if(status)return status;}goto Lc905f26; /* fmul fr1,fr13 */
Lc905f26: *stop=0xc905f26u;fcopy(f,7u,13u);goto Lc905f28; /* fmov fr13,fr7 */
Lc905f28: *stop=0xc905f28u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[14],&f->fpscr,&fr[7],0);if(status)return status;}goto Lc905f2a; /* fadd fr14,fr7 */
Lc905f2a: *stop=0xc905f2au;r[0]=32u;goto Lc905f2c; /* mov #0x20,r0 */
Lc905f2c: *stop=0xc905f2cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[15]);goto Lc905f2e; /* fmov.s @(r0,r15),fr6 */
Lc905f2e: *stop=0xc905f2eu;r[0]=12u;goto Lc905f30; /* mov #0xc,r0 */
Lc905f30: *stop=0xc905f30u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[15]);goto Lc905f32; /* fmov.s @(r0,r15),fr5 */
Lc905f32: *stop=0xc905f32u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[14],&f->fpscr,&fr[6],0);if(status)return status;}goto Lc905f34; /* fadd fr14,fr6 */
Lc905f34: *stop=0xc905f34u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[14],&f->fpscr,&fr[5],0);if(status)return status;}goto Lc905f36; /* fadd fr14,fr5 */
Lc905f36: target=r[10];*pr=0x0c905f3au;*stop=0xc905f38u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905f3a; /* jsr @r10 */
Lc905f3a: *stop=0xc905f3au;r[0]=12u;goto Lc905f3c; /* mov #0xc,r0 */
Lc905f3c: *stop=0xc905f3cu;r[4]=rd(m,0x0c9060acu,4);goto Lc905f3e; /* mov.l 0x0c9060ac,r4 */
Lc905f3e: *stop=0xc905f3eu;if(!valid_ram(r[11],4u))return -3;r[5]=rd(m,r[11],4);goto Lc905f40; /* mov.l @r11,r5 */
Lc905f40: *stop=0xc905f40u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905f42; /* fmov.s @(r0,r13),fr4 */
Lc905f42: target=r[8];*pr=0x0c905f46u;*stop=0xc905f44u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905f46; /* jsr @r8 */
Lc905f46: *stop=0xc905f46u;fcopy(f,5u,15u);goto Lc905f48; /* fmov fr15,fr5 */
Lc905f48: *stop=0xc905f48u;fcopy(f,6u,15u);goto Lc905f4a; /* fmov fr15,fr6 */
Lc905f4a: *stop=0xc905f4au;fcopy(f,7u,15u);goto Lc905f4c; /* fmov fr15,fr7 */
Lc905f4c: target=r[10];*pr=0x0c905f50u;*stop=0xc905f4eu;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905f50; /* jsr @r10 */
Lc905f50: *stop=0xc905f52u;goto Lc905f64; /* bra 0x0c905f64 */
Lc905f54: *stop=0xc905f54u;r[0]=12u;goto Lc905f56; /* mov #0xc,r0 */
Lc905f56: *stop=0xc905f56u;if(!valid_ram(r[11],4u))return -3;r[5]=rd(m,r[11],4);goto Lc905f58; /* mov.l @r11,r5 */
Lc905f58: *stop=0xc905f58u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905f5a; /* fmov.s @(r0,r13),fr4 */
Lc905f5a: *stop=0xc905f5au;r[0]=2u;goto Lc905f5c; /* mov #0x2,r0 */
Lc905f5c: *stop=0xc905f5cu;r[0]<<=8;goto Lc905f5e; /* shll8 r0 */
Lc905f5e: *stop=0xc905f5eu;r[0]+=72u;goto Lc905f60; /* add #0x48,r0 */
Lc905f60: target=r[8];*pr=0x0c905f64u;*stop=0xc905f62u;if(!valid_ram(r[0]+r[14],4u))return -3;r[4]=rd(m,r[14]+r[0],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905f64; /* jsr @r8 */
Lc905f64: *stop=0xc905f64u;r[0]=96u;goto Lc905f66; /* mov #0x60,r0 */
Lc905f66: *stop=0xc905f66u;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905f68; /* mov.l @(r0,r14),r0 */
Lc905f68: *stop=0xc905f68u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto Lc905f6a; /* tst #0x4,r0 */
Lc905f6a: if(!(*sr&1u))goto Lc905fcc;goto Lc905f6c; /* bf 0x0c905fcc */
Lc905f6c: *stop=0xc905f6cu;r[2]=rd(m,0x0c9060b0u,4);goto Lc905f6e; /* mov.l 0x0c9060b0,r2 */
Lc905f6e: *stop=0xc905f6eu;r[3]=4294967172u;goto Lc905f70; /* mov #-0x7c,r3 */
Lc905f70: *stop=0xc905f70u;r[3]=r[3]&255u;goto Lc905f72; /* extu.b r3,r3 */
Lc905f72: *stop=0xc905f72u;r[5]=r[15];goto Lc905f74; /* mov r15,r5 */
Lc905f74: *stop=0xc905f74u;if(!valid_ram(r[2],4u))return -3;r[1]=rd(m,r[2],4);goto Lc905f76; /* mov.l @r2,r1 */
Lc905f76: *stop=0xc905f76u;r[3]+=r[15];goto Lc905f78; /* add r15,r3 */
Lc905f78: *stop=0xc905f78u;if(!valid_ram(r[2]+4u,4u))return -3;r[0]=rd(m,r[2]+0x4u,4);goto Lc905f7a; /* mov.l @(0x4,r2),r0 */
Lc905f7a: *stop=0xc905f7au;r[7]=r[15];goto Lc905f7c; /* mov r15,r7 */
Lc905f7c: *stop=0xc905f7cu;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[1],4);goto Lc905f7e; /* mov.l r1,@r3 */
Lc905f7e: *stop=0xc905f7eu;r[5]+=124u;goto Lc905f80; /* add #0x7c,r5 */
Lc905f80: *stop=0xc905f80u;if(!valid_ram(r[3]+4u,4u))return -3;wr(m,r[3]+0x4u,r[0],4);goto Lc905f82; /* mov.l r0,@(0x4,r3) */
Lc905f82: *stop=0xc905f82u;r[0]=2u;goto Lc905f84; /* mov #0x2,r0 */
Lc905f84: *stop=0xc905f84u;if(!valid_ram(r[2]+8u,4u))return -3;r[1]=rd(m,r[2]+0x8u,4);goto Lc905f86; /* mov.l @(0x8,r2),r1 */
Lc905f86: *stop=0xc905f86u;r[0]<<=8;goto Lc905f88; /* shll8 r0 */
Lc905f88: *stop=0xc905f88u;r[0]+=72u;goto Lc905f8a; /* add #0x48,r0 */
Lc905f8a: *stop=0xc905f8au;if(!valid_ram(r[3]+8u,4u))return -3;wr(m,r[3]+0x8u,r[1],4);goto Lc905f8c; /* mov.l r1,@(0x8,r3) */
Lc905f8c: *stop=0xc905f8cu;r[6]=32u;goto Lc905f8e; /* mov #0x20,r6 */
Lc905f8e: *stop=0xc905f8eu;if(!valid_ram(r[0]+r[14],4u))return -3;r[3]=rd(m,r[14]+r[0],4);goto Lc905f90; /* mov.l @(r0,r14),r3 */
Lc905f90: *stop=0xc905f90u;r[0]=124u;goto Lc905f92; /* mov #0x7c,r0 */
Lc905f92: *stop=0xc905f92u;r[7]+=60u;goto Lc905f94; /* add #0x3c,r7 */
Lc905f94: *stop=0xc905f94u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[3],4);goto Lc905f96; /* mov.l r3,@(r0,r15) */
Lc905f96: *stop=0xc905f96u;r[0]=4294967168u;goto Lc905f98; /* mov #-0x80,r0 */
Lc905f98: *stop=0xc905f98u;if(!valid_ram(r[11],4u))return -3;r[1]=rd(m,r[11],4);goto Lc905f9a; /* mov.l @r11,r1 */
Lc905f9a: *stop=0xc905f9au;r[0]=r[0]&255u;goto Lc905f9c; /* extu.b r0,r0 */
Lc905f9c: *stop=0xc905f9cu;r[3]=rd(m,0x0c9060b4u,4);goto Lc905f9e; /* mov.l 0x0c9060b4,r3 */
Lc905f9e: *stop=0xc905f9eu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[1],4);goto Lc905fa0; /* mov.l r1,@(r0,r15) */
Lc905fa0: *stop=0xc905fa0u;r[0]=12u;goto Lc905fa2; /* mov #0xc,r0 */
Lc905fa2: *stop=0xc905fa2u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[13]);goto Lc905fa4; /* fmov.s @(r0,r13),fr4 */
Lc905fa4: target=r[3];*pr=0x0c905fa8u;*stop=0xc905fa6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905fa8; /* jsr @r3 */
Lc905fa8: *stop=0xc905fa8u;r[6]=8u;goto Lc905faa; /* mov #0x8,r6 */
Lc905faa: *stop=0xc905faau;r[2]=rd(m,0x0c9060b8u,4);goto Lc905fac; /* mov.l 0x0c9060b8,r2 */
Lc905fac: *stop=0xc905facu;r[5]=4294967172u;goto Lc905fae; /* mov #-0x7c,r5 */
Lc905fae: *stop=0xc905faeu;r[4]=r[15];goto Lc905fb0; /* mov r15,r4 */
Lc905fb0: *stop=0xc905fb0u;r[6]<<=8;goto Lc905fb2; /* shll8 r6 */
Lc905fb2: *stop=0xc905fb2u;r[5]=r[5]&255u;goto Lc905fb4; /* extu.b r5,r5 */
Lc905fb4: *stop=0xc905fb4u;r[6]+=28u;goto Lc905fb6; /* add #0x1c,r6 */
Lc905fb6: *stop=0xc905fb6u;r[6]+=r[14];goto Lc905fb8; /* add r14,r6 */
Lc905fb8: *stop=0xc905fb8u;r[5]+=r[15];goto Lc905fba; /* add r15,r5 */
Lc905fba: target=r[2];*pr=0x0c905fbeu;*stop=0xc905fbcu;r[4]+=60u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905fbe; /* jsr @r2 */
Lc905fbe: *stop=0xc905fbeu;r[3]=rd(m,0x0c9060bcu,4);goto Lc905fc0; /* mov.l 0x0c9060bc,r3 */
Lc905fc0: *stop=0xc905fc0u;r[5]=r[14];goto Lc905fc2; /* mov r14,r5 */
Lc905fc2: target=r[3];*pr=0x0c905fc6u;*stop=0xc905fc4u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905fc6; /* jsr @r3 */
Lc905fc6: *stop=0xc905fc6u;r[0]=96u;goto Lc905fc8; /* mov #0x60,r0 */
Lc905fc8: *stop=0xc905fc8u;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905fca; /* mov.l @(r0,r14),r0 */
Lc905fca: *stop=0xc905fcau;*sr=(*sr&~1u)|!(r[0]&0x1u);goto Lc905fcc; /* tst #0x1,r0 */
Lc905fcc: *stop=0xc905fccu;r[3]=rd(m,0x0c9060c0u,4);goto Lc905fce; /* mov.l 0x0c9060c0,r3 */
Lc905fce: target=r[3];*pr=0x0c905fd2u;*stop=0xc905fd0u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905fd2; /* jsr @r3 */
Lc905fd2: *stop=0xc905fd2u;r[2]=rd(m,0x0c9060c4u,4);goto Lc905fd4; /* mov.l 0x0c9060c4,r2 */
Lc905fd4: target=r[2];*pr=0x0c905fd8u;*stop=0xc905fd6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905fd8; /* jsr @r2 */
Lc905fd8: *stop=0xc905fd8u;r[3]=rd(m,0x0c9060ccu,4);goto Lc905fda; /* mov.l 0x0c9060cc,r3 */
Lc905fda: *stop=0xc905fdau;r[5]=rd(m,0x0c9060c8u,4);goto Lc905fdc; /* mov.l 0x0c9060c8,r5 */
Lc905fdc: target=r[3];*pr=0x0c905fe0u;*stop=0xc905fdeu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc905fe0; /* jsr @r3 */
Lc905fe0: *stop=0xc905fe0u;r[0]=96u;goto Lc905fe2; /* mov #0x60,r0 */
Lc905fe2: *stop=0xc905fe2u;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc905fe4; /* mov.l @(r0,r14),r0 */
Lc905fe4: *stop=0xc905fe4u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto Lc905fe6; /* tst #0x4,r0 */
Lc905fe6: if((*sr&1u))goto Lc90604c;goto Lc905fe8; /* bt 0x0c90604c */
Lc905fe8: *stop=0xc905fe8u;r[0]=0x0c9060d0u;goto Lc905fea; /* mova 0x0c9060d0,r0 */
Lc905fea: *stop=0xc905feau;r[4]=rd(m,0x0c9060d4u,4);goto Lc905fec; /* mov.l 0x0c9060d4,r4 */
Lc905fec: *stop=0xc905fecu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc905fee; /* fmov.s @r0,fr3 */
Lc905fee: *stop=0xc905feeu;r[0]=108u;goto Lc905ff0; /* mov #0x6c,r0 */
Lc905ff0: *stop=0xc905ff0u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]+r[14]);goto Lc905ff2; /* fmov.s @(r0,r14),fr14 */
Lc905ff2: *stop=0xc905ff2u;r[0]=112u;goto Lc905ff4; /* mov #0x70,r0 */
Lc905ff4: *stop=0xc905ff4u;r[3]=rd(m,0x0c9060d8u,4);goto Lc905ff6; /* mov.l 0x0c9060d8,r3 */
Lc905ff6: *stop=0xc905ff6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[14],&fr[3],&f->fpscr,&fr[14]);if(status)return status;}goto Lc905ff8; /* fmul fr3,fr14 */
Lc905ff8: *stop=0xc905ff8u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[0]+r[14]);goto Lc905ffa; /* fmov.s @(r0,r14),fr13 */
Lc905ffa: *stop=0xc905ffau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto Lc905ffc; /* fldi1 fr2 */
Lc905ffc: *stop=0xc905ffcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc905ffe; /* fneg fr2 */
Lc905ffe: target=r[3];*pr=0x0c906002u;*stop=0xc906000u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[2],&f->fpscr,&fr[13],0);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc906002; /* jsr @r3 */
Lc906002: *stop=0xc906002u;r[2]=rd(m,0x0c9060dcu,4);goto Lc906004; /* mov.l 0x0c9060dc,r2 */
Lc906004: target=r[2];*pr=0x0c906008u;*stop=0xc906006u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc906008; /* jsr @r2 */
Lc906008: *stop=0xc906008u;fcopy(f,5u,12u);goto Lc90600a; /* fmov fr12,fr5 */
Lc90600a: *stop=0xc90600au;fcopy(f,6u,12u);goto Lc90600c; /* fmov fr12,fr6 */
Lc90600c: *stop=0xc90600cu;fcopy(f,7u,12u);goto Lc90600e; /* fmov fr12,fr7 */
Lc90600e: target=r[10];*pr=0x0c906012u;*stop=0xc906010u;fcopy(f,4u,13u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc906012; /* jsr @r10 */
Lc906012: *stop=0xc906012u;r[3]=rd(m,0x0c9060e0u,4);goto Lc906014; /* mov.l 0x0c9060e0,r3 */
Lc906014: *stop=0xc906014u;r[5]=r[12];goto Lc906016; /* mov r12,r5 */
Lc906016: *stop=0xc906016u;r[5]+=32u;goto Lc906018; /* add #0x20,r5 */
Lc906018: target=r[3];*pr=0x0c90601cu;*stop=0xc90601au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90601c; /* jsr @r3 */
Lc90601c: *stop=0xc90601cu;if(!valid_ram(r[12]+24u,4u))return -3;r[2]=rd(m,r[12]+0x18u,4);goto Lc90601e; /* mov.l @(0x18,r12),r2 */
Lc90601e: *stop=0xc90601eu;r[3]=64u;goto Lc906020; /* mov #0x40,r3 */
Lc906020: *stop=0xc906020u;r[3]<<=8;goto Lc906022; /* shll8 r3 */
Lc906022: *stop=0xc906022u;r[2]+=r[3];goto Lc906024; /* add r3,r2 */
Lc906024: *stop=0xc906024u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc906026; /* tst r2,r2 */
Lc906026: take=(*sr&1u);*stop=0xc906028u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);if(take)goto Lc906032;goto Lc90602a; /* bt/s 0x0c906032 */
Lc90602a: *stop=0xc90602au;r[1]=rd(m,0x0c9060e4u,4);goto Lc90602c; /* mov.l 0x0c9060e4,r1 */
Lc90602c: *stop=0xc90602cu;r[5]=r[2];goto Lc90602e; /* mov r2,r5 */
Lc90602e: target=r[1];*pr=0x0c906032u;*stop=0xc906030u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc906032; /* jsr @r1 */
Lc906032: *stop=0xc906032u;r[2]=rd(m,0x0c9060e8u,4);goto Lc906034; /* mov.l 0x0c9060e8,r2 */
Lc906034: *stop=0xc906034u;fcopy(f,4u,14u);goto Lc906036; /* fmov fr14,fr4 */
Lc906036: *stop=0xc906036u;fcopy(f,6u,14u);goto Lc906038; /* fmov fr14,fr6 */
Lc906038: *stop=0xc906038u;fcopy(f,5u,14u);goto Lc90603a; /* fmov fr14,fr5 */
Lc90603a: target=r[2];*pr=0x0c90603eu;*stop=0xc90603cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90603e; /* jsr @r2 */
Lc90603e: *stop=0xc90603eu;r[3]=rd(m,0x0c9060f0u,4);goto Lc906040; /* mov.l 0x0c9060f0,r3 */
Lc906040: *stop=0xc906040u;r[4]=rd(m,0x0c9060ecu,4);goto Lc906042; /* mov.l 0x0c9060ec,r4 */
Lc906042: target=r[3];*pr=0x0c906046u;*stop=0xc906044u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc906046; /* jsr @r3 */
Lc906046: *stop=0xc906046u;r[2]=rd(m,0x0c9060c0u,4);goto Lc906048; /* mov.l 0x0c9060c0,r2 */
Lc906048: target=r[2];*pr=0x0c90604cu;*stop=0xc90604au;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90604c; /* jsr @r2 */
Lc90604c: *stop=0xc90604cu;r[3]=rd(m,0x0c9060f4u,4);goto Lc90604e; /* mov.l 0x0c9060f4,r3 */
Lc90604e: target=r[3];*pr=0x0c906052u;*stop=0xc906050u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc906052; /* jsr @r3 */
Lc906052: *stop=0xc906052u;r[2]=rd(m,0x0c9060f8u,4);goto Lc906054; /* mov.l 0x0c9060f8,r2 */
Lc906054: *stop=0xc906054u;r[5]=r[14];goto Lc906056; /* mov r14,r5 */
Lc906056: target=r[2];*pr=0x0c90605au;*stop=0xc906058u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90605a; /* jsr @r2 */
Lc90605a: *stop=0xc90605au;r[3]=rd(m,0x0c9060fcu,4);goto Lc90605c; /* mov.l 0x0c9060fc,r3 */
Lc90605c: *stop=0xc90605cu;r[4]=2u;goto Lc90605e; /* mov #0x2,r4 */
Lc90605e: *stop=0xc90605eu;r[4]<<=8;goto Lc906060; /* shll8 r4 */
Lc906060: target=r[3];*pr=0x0c906064u;*stop=0xc906062u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc906064; /* jsr @r3 */
Lc906064: *stop=0xc906064u;fcopy(f,5u,15u);goto Lc906066; /* fmov fr15,fr5 */
Lc906066: *stop=0xc906066u;fcopy(f,6u,15u);goto Lc906068; /* fmov fr15,fr6 */
Lc906068: *stop=0xc906068u;fcopy(f,7u,15u);goto Lc90606a; /* fmov fr15,fr7 */
Lc90606a: target=r[10];*pr=0x0c90606eu;*stop=0xc90606cu;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90606e; /* jsr @r10 */
Lc90606e: *stop=0xc90606eu;r[1]=4294967184u;goto Lc906070; /* mov #-0x70,r1 */
Lc906070: *stop=0xc906070u;r[1]=r[1]&255u;goto Lc906072; /* extu.b r1,r1 */
Lc906072: *stop=0xc906072u;r[15]+=r[1];goto Lc906074; /* add r1,r15 */
Lc906074: *stop=0xc906074u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc906076; /* lds.l @r15+,PR */
Lc906076: *stop=0xc906076u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc906078; /* fmov.s @r15+,fr12 */
Lc906078: *stop=0xc906078u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc90607a; /* fmov.s @r15+,fr13 */
Lc90607a: *stop=0xc90607au;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc90607c; /* fmov.s @r15+,fr14 */
Lc90607c: *stop=0xc90607cu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc90607e; /* fmov.s @r15+,fr15 */
Lc90607e: *stop=0xc90607eu;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto Lc906080; /* mov.l @r15+,r8 */
Lc906080: *stop=0xc906080u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto Lc906082; /* mov.l @r15+,r9 */
Lc906082: *stop=0xc906082u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto Lc906084; /* mov.l @r15+,r10 */
Lc906084: *stop=0xc906084u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto Lc906086; /* mov.l @r15+,r11 */
Lc906086: *stop=0xc906086u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto Lc906088; /* mov.l @r15+,r12 */
Lc906088: *stop=0xc906088u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto Lc90608a; /* mov.l @r15+,r13 */
Lc90608a: *stop=0xc90608au;if(*pr!=entry_pr)return -3;*stop=0xc90608cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
