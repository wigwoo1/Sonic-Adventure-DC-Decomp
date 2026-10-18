/* AL_MAIN animation, appearance, accessories and material tails; names inferred.
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
SA_STAGE_EXPORT int sa_overlay_animation_start_0c90bd40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc90bd40;
Lc90bd40: *stop=0xc90bd40u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc90bd42; /* mov.l r14,@-r15 */
Lc90bd42: *stop=0xc90bd42u;r[14]=70u;goto Lc90bd44; /* mov #0x46,r14 */
Lc90bd44: *stop=0xc90bd44u;r[14]<<=2;goto Lc90bd46; /* shll2 r14 */
Lc90bd46: *stop=0xc90bd46u;r[3]=rd(m,0x0c90bf00u,4);goto Lc90bd48; /* mov.l 0x0c90bf00,r3 */
Lc90bd48: *stop=0xc90bd48u;r[0]=0u;goto Lc90bd4a; /* mov #0x0,r0 */
Lc90bd4a: *stop=0xc90bd4au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto Lc90bd4c; /* fldi1 fr3 */
Lc90bd4c: *stop=0xc90bd4cu;r[14]+=r[5];goto Lc90bd4e; /* add r5,r14 */
Lc90bd4e: *stop=0xc90bd4eu;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto Lc90bd50; /* fmov.s fr15,@-r15 */
Lc90bd50: *stop=0xc90bd50u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto Lc90bd52; /* fmov.s fr14,@-r15 */
Lc90bd52: *stop=0xc90bd52u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc90bd54; /* sts.l PR,@-r15 */
Lc90bd54: *stop=0xc90bd54u;if(!valid_ram(r[14]+6u,2u))return -3;wr(m,r[14]+0x6u,r[0],2);goto Lc90bd56; /* mov.w r0,@(0x6,r14) */
Lc90bd56: *stop=0xc90bd56u;if(!valid_ram(r[14]+8u,2u))return -3;wr(m,r[14]+0x8u,r[0],2);goto Lc90bd58; /* mov.w r0,@(0x8,r14) */
Lc90bd58: *stop=0xc90bd58u;if(!valid_ram(r[14]+10u,2u))return -3;wr(m,r[14]+0xau,r[0],2);goto Lc90bd5a; /* mov.w r0,@(0xa,r14) */
Lc90bd5a: *stop=0xc90bd5au;if(!valid_ram(r[14]+32u,4u))return -3;wr(m,r[14]+0x20u,r[3],4);goto Lc90bd5c; /* mov.l r3,@(0x20,r14) */
Lc90bd5c: *stop=0xc90bd5cu;if(!valid_ram(r[14]+6u,2u))return -3;r[0]=sx(m,r[14]+0x6u,2);goto Lc90bd5e; /* mov.w @(0x6,r14),r0 */
Lc90bd5e: *stop=0xc90bd5eu;if(!valid_ram(r[14]+32u,4u))return -3;r[2]=rd(m,r[14]+0x20u,4);goto Lc90bd60; /* mov.l @(0x20,r14),r2 */
Lc90bd60: *stop=0xc90bd60u;r[3]=r[0];goto Lc90bd62; /* mov r0,r3 */
Lc90bd62: *stop=0xc90bd62u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc90bd64; /* shll r0 */
Lc90bd64: *stop=0xc90bd64u;r[0]+=r[3];goto Lc90bd66; /* add r3,r0 */
Lc90bd66: *stop=0xc90bd66u;r[0]<<=2;goto Lc90bd68; /* shll2 r0 */
Lc90bd68: *stop=0xc90bd68u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc90bd6a; /* shll r0 */
Lc90bd6a: *stop=0xc90bd6au;r[0]+=r[2];goto Lc90bd6c; /* add r2,r0 */
Lc90bd6c: *stop=0xc90bd6cu;if(!valid_ram(r[0]+4u,2u))return -3;r[0]=sx(m,r[0]+0x4u,2);goto Lc90bd6e; /* mov.w @(0x4,r0),r0 */
Lc90bd6e: *stop=0xc90bd6eu;if(!valid_ram(r[14],2u))return -3;wr(m,r[14],r[0],2);goto Lc90bd70; /* mov.w r0,@r14 */
Lc90bd70: *stop=0xc90bd70u;r[0]=28u;goto Lc90bd72; /* mov #0x1c,r0 */
Lc90bd72: *stop=0xc90bd72u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto Lc90bd74; /* fmov fr3,@(r0,r14) */
Lc90bd74: *stop=0xc90bd74u;if(!valid_ram(r[14]+6u,2u))return -3;r[0]=sx(m,r[14]+0x6u,2);goto Lc90bd76; /* mov.w @(0x6,r14),r0 */
Lc90bd76: *stop=0xc90bd76u;r[3]=r[0];goto Lc90bd78; /* mov r0,r3 */
Lc90bd78: *stop=0xc90bd78u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc90bd7a; /* shll r0 */
Lc90bd7a: *stop=0xc90bd7au;r[0]+=r[3];goto Lc90bd7c; /* add r3,r0 */
Lc90bd7c: *stop=0xc90bd7cu;r[0]<<=2;goto Lc90bd7e; /* shll2 r0 */
Lc90bd7e: *stop=0xc90bd7eu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc90bd80; /* shll r0 */
Lc90bd80: *stop=0xc90bd80u;if(!valid_ram(r[14]+32u,4u))return -3;r[4]=rd(m,r[14]+0x20u,4);goto Lc90bd82; /* mov.l @(0x20,r14),r4 */
Lc90bd82: *stop=0xc90bd82u;r[3]=rd(m,0x0c90bf04u,4);goto Lc90bd84; /* mov.l 0x0c90bf04,r3 */
Lc90bd84: *stop=0xc90bd84u;r[4]+=r[0];goto Lc90bd86; /* add r0,r4 */
Lc90bd86: *stop=0xc90bd86u;r[0]=8u;goto Lc90bd88; /* mov #0x8,r0 */
Lc90bd88: *stop=0xc90bd88u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]+r[4]);goto Lc90bd8a; /* fmov.s @(r0,r4),fr15 */
Lc90bd8a: *stop=0xc90bd8au;r[0]=12u;goto Lc90bd8c; /* mov #0xc,r0 */
Lc90bd8c: *stop=0xc90bd8cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]+r[4]);goto Lc90bd8e; /* fmov.s @(r0,r4),fr14 */
Lc90bd8e: target=r[3];*pr=0x0c90bd92u;*stop=0xc90bd90u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[15],&f->fpscr,&fr[14],1);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90bd92; /* jsr @r3 */
Lc90bd92: *stop=0xc90bd92u;*fpul=r[0];goto Lc90bd94; /* lds r0,FPUL */
Lc90bd94: *stop=0xc90bd94u;r[0]=0x0c90bf08u;goto Lc90bd96; /* mova 0x0c90bf08,r0 */
Lc90bd96: *stop=0xc90bd96u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]);goto Lc90bd98; /* fmov.s @r0,fr0 */
Lc90bd98: *stop=0xc90bd98u;r[0]=12u;goto Lc90bd9a; /* mov #0xc,r0 */
Lc90bd9a: *stop=0xc90bd9au;r[3]=rd(m,0x0c90bf0cu,4);goto Lc90bd9c; /* mov.l 0x0c90bf0c,r3 */
Lc90bd9c: *stop=0xc90bd9cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc90bd9e; /* float FPUL,fr3 */
Lc90bd9e: *stop=0xc90bd9eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[14],&fr[3],&f->fpscr,&fr[14]);if(status)return status;}goto Lc90bda0; /* fmul fr3,fr14 */
Lc90bda0: *stop=0xc90bda0u;fcopy(f,3u,15u);goto Lc90bda2; /* fmov fr15,fr3 */
Lc90bda2: *stop=0xc90bda2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[14],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90bda4; /* fmac fr0,fr14,fr3 */
Lc90bda4: *stop=0xc90bda4u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[14]);goto Lc90bda6; /* fmov fr3,@(r0,r14) */
Lc90bda6: *stop=0xc90bda6u;if(!valid_ram(r[14]+2u,2u))return -3;r[0]=sx(m,r[14]+0x2u,2);goto Lc90bda8; /* mov.w @(0x2,r14),r0 */
Lc90bda8: *stop=0xc90bda8u;r[0]&=r[3];goto Lc90bdaa; /* and r3,r0 */
Lc90bdaa: *stop=0xc90bdaau;if(!valid_ram(r[14]+2u,2u))return -3;wr(m,r[14]+0x2u,r[0],2);goto Lc90bdac; /* mov.w r0,@(0x2,r14) */
Lc90bdac: *stop=0xc90bdacu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc90bdae; /* lds.l @r15+,PR */
Lc90bdae: *stop=0xc90bdaeu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc90bdb0; /* fmov.s @r15+,fr14 */
Lc90bdb0: *stop=0xc90bdb0u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc90bdb2; /* fmov.s @r15+,fr15 */
Lc90bdb2: *stop=0xc90bdb2u;if(*pr!=entry_pr)return -3;*stop=0xc90bdb4u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_appearance_select_0c90853e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc90853e;
Lc90853e: *stop=0xc90853eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc908540; /* mov.l r14,@-r15 */
Lc908540: *stop=0xc908540u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto Lc908542; /* mov.l r13,@-r15 */
Lc908542: *stop=0xc908542u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto Lc908544; /* mov.l r12,@-r15 */
Lc908544: *stop=0xc908544u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto Lc908546; /* mov.l r11,@-r15 */
Lc908546: *stop=0xc908546u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto Lc908548; /* mov.l r9,@-r15 */
Lc908548: *stop=0xc908548u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc90854a; /* sts.l PR,@-r15 */
Lc90854a: *stop=0xc90854au;r[15]+=4294967280u;goto Lc90854c; /* add #-0x10,r15 */
Lc90854c: *stop=0xc90854cu;r[0]=4294967276u;goto Lc90854e; /* mov #-0x14,r0 */
Lc90854e: *stop=0xc90854eu;r[13]=r[5];goto Lc908550; /* mov r5,r13 */
Lc908550: *stop=0xc908550u;r[0]=r[0]&255u;goto Lc908552; /* extu.b r0,r0 */
Lc908552: *stop=0xc908552u;if(!valid_ram(r[0]+r[13],4u))return -3;r[4]=rd(m,r[13]+r[0],4);goto Lc908554; /* mov.l @(r0,r13),r4 */
Lc908554: *stop=0xc908554u;r[0]=92u;goto Lc908556; /* mov #0x5c,r0 */
Lc908556: *stop=0xc908556u;r[14]=8u;goto Lc908558; /* mov #0x8,r14 */
Lc908558: *stop=0xc908558u;if(!valid_ram(r[0]+r[4],1u))return -3;r[3]=sx(m,r[4]+r[0],1);goto Lc90855a; /* mov.b @(r0,r4),r3 */
Lc90855a: *stop=0xc90855au;r[14]<<=8;goto Lc90855c; /* shll8 r14 */
Lc90855c: *stop=0xc90855cu;r[11]=4294967261u;goto Lc90855e; /* mov #-0x23,r11 */
Lc90855e: *stop=0xc90855eu;r[14]+=4294967240u;goto Lc908560; /* add #-0x38,r14 */
Lc908560: *stop=0xc908560u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[11]);goto Lc908562; /* cmp/ge r11,r3 */
Lc908562: *stop=0xc908562u;r[12]=35u;goto Lc908564; /* mov #0x23,r12 */
Lc908564: *stop=0xc908564u;r[14]+=r[13];goto Lc908566; /* add r13,r14 */
Lc908566: *stop=0xc908566u;r[9]=1u;goto Lc908568; /* mov #0x1,r9 */
Lc908568: *stop=0xc908568u;r[1]=0u;goto Lc90856a; /* mov #0x0,r1 */
Lc90856a: take=(*sr&1u);*stop=0xc90856cu;r[5]=2u;if(take)goto Lc908572;goto Lc90856e; /* bt/s 0x0c908572 */
Lc90856e: *stop=0xc908570u;r[6]=r[1];goto Lc90857e; /* bra 0x0c90857e */
Lc908572: *stop=0xc908572u;if(!valid_ram(r[0]+r[4],1u))return -3;r[3]=sx(m,r[4]+r[0],1);goto Lc908574; /* mov.b @(r0,r4),r3 */
Lc908574: *stop=0xc908574u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[12]);goto Lc908576; /* cmp/ge r12,r3 */
Lc908576: if((*sr&1u))goto Lc90857c;goto Lc908578; /* bt 0x0c90857c */
Lc908578: *stop=0xc90857au;r[6]=r[9];goto Lc90857e; /* bra 0x0c90857e */
Lc90857c: *stop=0xc90857cu;r[6]=r[5];goto Lc90857e; /* mov r5,r6 */
Lc90857e: *stop=0xc90857eu;r[0]=93u;goto Lc908580; /* mov #0x5d,r0 */
Lc908580: *stop=0xc908580u;if(!valid_ram(r[0]+r[4],1u))return -3;r[3]=sx(m,r[4]+r[0],1);goto Lc908582; /* mov.b @(r0,r4),r3 */
Lc908582: *stop=0xc908582u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[11]);goto Lc908584; /* cmp/ge r11,r3 */
Lc908584: if((*sr&1u))goto Lc90858a;goto Lc908586; /* bt 0x0c90858a */
Lc908586: *stop=0xc908588u;r[7]=r[1];goto Lc9085c2; /* bra 0x0c9085c2 */
Lc90858a: *stop=0xc90858au;if(!valid_ram(r[0]+r[4],1u))return -3;r[3]=sx(m,r[4]+r[0],1);goto Lc90858c; /* mov.b @(r0,r4),r3 */
Lc90858c: *stop=0xc90858cu;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[12]);goto Lc90858e; /* cmp/ge r12,r3 */
Lc90858e: if((*sr&1u))goto Lc9085c0;goto Lc908590; /* bt 0x0c9085c0 */
Lc908590: *stop=0xc908592u;r[7]=r[9];goto Lc9085c2; /* bra 0x0c9085c2 */
Lc9085c0: *stop=0xc9085c0u;r[7]=r[5];goto Lc9085c2; /* mov r5,r7 */
Lc9085c2: *stop=0xc9085c2u;r[0]=94u;goto Lc9085c4; /* mov #0x5e,r0 */
Lc9085c4: *stop=0xc9085c4u;if(!valid_ram(r[0]+r[4],1u))return -3;r[3]=sx(m,r[4]+r[0],1);goto Lc9085c6; /* mov.b @(r0,r4),r3 */
Lc9085c6: *stop=0xc9085c6u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[11]);goto Lc9085c8; /* cmp/ge r11,r3 */
Lc9085c8: if((*sr&1u))goto Lc9085ce;goto Lc9085ca; /* bt 0x0c9085ce */
Lc9085ca: *stop=0xc9085ccu;r[4]=r[1];goto Lc9085da; /* bra 0x0c9085da */
Lc9085ce: *stop=0xc9085ceu;if(!valid_ram(r[0]+r[4],1u))return -3;r[3]=sx(m,r[4]+r[0],1);goto Lc9085d0; /* mov.b @(r0,r4),r3 */
Lc9085d0: *stop=0xc9085d0u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[12]);goto Lc9085d2; /* cmp/ge r12,r3 */
Lc9085d2: if((*sr&1u))goto Lc9085d8;goto Lc9085d4; /* bt 0x0c9085d8 */
Lc9085d4: *stop=0xc9085d6u;r[4]=r[9];goto Lc9085da; /* bra 0x0c9085da */
Lc9085d8: *stop=0xc9085d8u;r[4]=r[5];goto Lc9085da; /* mov r5,r4 */
Lc9085da: *stop=0xc9085dau;r[0]=(uint32_t)(int32_t)(int8_t)r[7];goto Lc9085dc; /* exts.b r7,r0 */
Lc9085dc: *stop=0xc9085dcu;r[1]=rd(m,0x0c908744u,4);goto Lc9085de; /* mov.l 0x0c908744,r1 */
Lc9085de: *stop=0xc9085deu;r[3]=r[0];goto Lc9085e0; /* mov r0,r3 */
Lc9085e0: *stop=0xc9085e0u;r[0]<<=2;goto Lc9085e2; /* shll2 r0 */
Lc9085e2: *stop=0xc9085e2u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc9085e4; /* shll r0 */
Lc9085e4: *stop=0xc9085e4u;r[0]+=r[3];goto Lc9085e6; /* add r3,r0 */
Lc9085e6: *stop=0xc9085e6u;r[3]=(uint32_t)(int32_t)(int8_t)r[6];goto Lc9085e8; /* exts.b r6,r3 */
Lc9085e8: *stop=0xc9085e8u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[0],4);goto Lc9085ea; /* mov.l r0,@(0x8,r15) */
Lc9085ea: *stop=0xc9085eau;r[2]=r[3];goto Lc9085ec; /* mov r3,r2 */
Lc9085ec: *stop=0xc9085ecu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto Lc9085ee; /* shll r3 */
Lc9085ee: *stop=0xc9085eeu;r[3]+=r[2];goto Lc9085f0; /* add r2,r3 */
Lc9085f0: *stop=0xc9085f0u;r[2]=rd(m,0x0c908740u,4);goto Lc9085f2; /* mov.l 0x0c908740,r2 */
Lc9085f2: *stop=0xc9085f2u;r[5]=(uint32_t)(int32_t)(int8_t)r[4];goto Lc9085f4; /* exts.b r4,r5 */
Lc9085f4: *stop=0xc9085f4u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto Lc9085f6; /* mov.l r3,@r15 */
Lc9085f6: *stop=0xc9085f6u;r[0]+=r[2];goto Lc9085f8; /* add r2,r0 */
Lc9085f8: *stop=0xc9085f8u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[5],4);goto Lc9085fa; /* mov.l r5,@(0x4,r15) */
Lc9085fa: *stop=0xc9085fau;r[0]+=r[3];goto Lc9085fc; /* add r3,r0 */
Lc9085fc: *stop=0xc9085fcu;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto Lc9085fe; /* mov.l @r15,r3 */
Lc9085fe: *stop=0xc9085feu;if(!valid_ram(r[0]+r[5],1u))return -3;r[5]=sx(m,r[5]+r[0],1);goto Lc908600; /* mov.b @(r0,r5),r5 */
Lc908600: *stop=0xc908600u;if(!valid_ram(r[15]+8u,4u))return -3;r[0]=rd(m,r[15]+0x8u,4);goto Lc908602; /* mov.l @(0x8,r15),r0 */
Lc908602: *stop=0xc908602u;if(!valid_ram(r[15]+4u,4u))return -3;r[2]=rd(m,r[15]+0x4u,4);goto Lc908604; /* mov.l @(0x4,r15),r2 */
Lc908604: *stop=0xc908604u;r[5]=r[5]&255u;goto Lc908606; /* extu.b r5,r5 */
Lc908606: *stop=0xc908606u;r[0]+=r[1];goto Lc908608; /* add r1,r0 */
Lc908608: *stop=0xc908608u;r[0]+=r[3];goto Lc90860a; /* add r3,r0 */
Lc90860a: *stop=0xc90860au;if(!valid_ram(r[0]+r[2],1u))return -3;r[3]=sx(m,r[2]+r[0],1);goto Lc90860c; /* mov.b @(r0,r2),r3 */
Lc90860c: *stop=0xc90860cu;r[0]=(uint32_t)(int32_t)(int8_t)r[7];goto Lc90860e; /* exts.b r7,r0 */
Lc90860e: *stop=0xc90860eu;r[2]=r[0];goto Lc908610; /* mov r0,r2 */
Lc908610: *stop=0xc908610u;r[0]<<=2;goto Lc908612; /* shll2 r0 */
Lc908612: *stop=0xc908612u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc908614; /* shll r0 */
Lc908614: *stop=0xc908614u;r[3]=r[3]&255u;goto Lc908616; /* extu.b r3,r3 */
Lc908616: *stop=0xc908616u;r[0]+=r[2];goto Lc908618; /* add r2,r0 */
Lc908618: *stop=0xc908618u;if(!valid_ram(r[15]+12u,4u))return -3;wr(m,r[15]+0xcu,r[3],4);goto Lc90861a; /* mov.l r3,@(0xc,r15) */
Lc90861a: *stop=0xc90861au;r[6]=(uint32_t)(int32_t)(int8_t)r[6];goto Lc90861c; /* exts.b r6,r6 */
Lc90861c: *stop=0xc90861cu;r[2]=rd(m,0x0c90874cu,4);goto Lc90861e; /* mov.l 0x0c90874c,r2 */
Lc90861e: *stop=0xc90861eu;r[3]=r[6];goto Lc908620; /* mov r6,r3 */
Lc908620: *stop=0xc908620u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto Lc908622; /* shll r6 */
Lc908622: *stop=0xc908622u;r[6]+=r[3];goto Lc908624; /* add r3,r6 */
Lc908624: *stop=0xc908624u;r[3]=rd(m,0x0c908748u,4);goto Lc908626; /* mov.l 0x0c908748,r3 */
Lc908626: *stop=0xc908626u;r[0]<<=2;goto Lc908628; /* shll2 r0 */
Lc908628: *stop=0xc908628u;r[11]=(uint32_t)(int32_t)(int8_t)r[4];goto Lc90862a; /* exts.b r4,r11 */
Lc90862a: *stop=0xc90862au;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[0],4);goto Lc90862c; /* mov.l r0,@r15 */
Lc90862c: *stop=0xc90862cu;r[11]<<=2;goto Lc90862e; /* shll2 r11 */
Lc90862e: *stop=0xc90862eu;r[0]+=r[3];goto Lc908630; /* add r3,r0 */
Lc908630: *stop=0xc908630u;r[6]<<=2;goto Lc908632; /* shll2 r6 */
Lc908632: *stop=0xc908632u;r[0]+=r[6];goto Lc908634; /* add r6,r0 */
Lc908634: *stop=0xc908634u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[6],4);goto Lc908636; /* mov.l r6,@(0x8,r15) */
Lc908636: *stop=0xc908636u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[11],4);goto Lc908638; /* mov.l r11,@(0x4,r15) */
Lc908638: *stop=0xc908638u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);goto Lc90863a; /* mov.l @r15,r12 */
Lc90863a: *stop=0xc90863au;if(!valid_ram(r[15]+8u,4u))return -3;r[1]=rd(m,r[15]+0x8u,4);goto Lc90863c; /* mov.l @(0x8,r15),r1 */
Lc90863c: *stop=0xc90863cu;if(!valid_ram(r[0]+r[11],4u))return -3;r[11]=rd(m,r[11]+r[0],4);goto Lc90863e; /* mov.l @(r0,r11),r11 */
Lc90863e: *stop=0xc90863eu;r[12]+=r[2];goto Lc908640; /* add r2,r12 */
Lc908640: *stop=0xc908640u;if(!valid_ram(r[15]+4u,4u))return -3;r[0]=rd(m,r[15]+0x4u,4);goto Lc908642; /* mov.l @(0x4,r15),r0 */
Lc908642: *stop=0xc908642u;r[12]+=r[1];goto Lc908644; /* add r1,r12 */
Lc908644: *stop=0xc908644u;if(!valid_ram(r[0]+r[12],4u))return -3;r[12]=rd(m,r[12]+r[0],4);goto Lc908646; /* mov.l @(r0,r12),r12 */
Lc908646: *stop=0xc908646u;if(!valid_ram(r[14]+44u,4u))return -3;wr(m,r[14]+0x2cu,r[11],4);goto Lc908648; /* mov.l r11,@(0x2c,r14) */
Lc908648: *stop=0xc908648u;if(!valid_ram(r[14]+28u,4u))return -3;wr(m,r[14]+0x1cu,r[12],4);goto Lc90864a; /* mov.l r12,@(0x1c,r14) */
Lc90864a: *stop=0xc90864au;if(!valid_ram(r[14]+32u,4u))return -3;wr(m,r[14]+0x20u,r[12],4);goto Lc90864c; /* mov.l r12,@(0x20,r14) */
Lc90864c: target=0x0c9084e6u;*pr=0x0c908650u;*stop=0xc90864eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908650; /* bsr 0x0c9084e6 */
Lc908650: *stop=0xc908650u;if(!valid_ram(r[15]+12u,4u))return -3;r[5]=rd(m,r[15]+0xcu,4);goto Lc908652; /* mov.l @(0xc,r15),r5 */
Lc908652: target=0x0c9084fcu;*pr=0x0c908656u;*stop=0xc908654u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc908656; /* bsr 0x0c9084fc */
Lc908656: *stop=0xc908656u;r[5]=r[12];goto Lc908658; /* mov r12,r5 */
Lc908658: *stop=0xc908658u;r[6]=r[11];goto Lc90865a; /* mov r11,r6 */
Lc90865a: target=0x0c908530u;*pr=0x0c90865eu;*stop=0xc90865cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90865e; /* bsr 0x0c908530 */
Lc90865e: *stop=0xc90865eu;r[4]=4294967295u;goto Lc908660; /* mov #-0x1,r4 */
Lc908660: *stop=0xc908660u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[4],4);goto Lc908662; /* mov.l r4,@(0x8,r14) */
Lc908662: *stop=0xc908662u;if(!valid_ram(r[14]+12u,4u))return -3;wr(m,r[14]+0xcu,r[4],4);goto Lc908664; /* mov.l r4,@(0xc,r14) */
Lc908664: *stop=0xc908664u;if(!valid_ram(r[14]+16u,4u))return -3;wr(m,r[14]+0x10u,r[4],4);goto Lc908666; /* mov.l r4,@(0x10,r14) */
Lc908666: *stop=0xc908666u;r[15]+=16u;goto Lc908668; /* add #0x10,r15 */
Lc908668: *stop=0xc908668u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc90866a; /* lds.l @r15+,PR */
Lc90866a: *stop=0xc90866au;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto Lc90866c; /* mov.l @r15+,r9 */
Lc90866c: *stop=0xc90866cu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto Lc90866e; /* mov.l @r15+,r11 */
Lc90866e: *stop=0xc90866eu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto Lc908670; /* mov.l @r15+,r12 */
Lc908670: *stop=0xc908670u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto Lc908672; /* mov.l @r15+,r13 */
Lc908672: *stop=0xc908672u;if(*pr!=entry_pr)return -3;*stop=0xc908674u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_accessory_install_0c9048ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc9048ec;
Lc9048ec: *stop=0xc9048ecu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc9048ee; /* mov.l r14,@-r15 */
Lc9048ee: *stop=0xc9048eeu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto Lc9048f0; /* mov.l r13,@-r15 */
Lc9048f0: *stop=0xc9048f0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto Lc9048f2; /* mov.l r12,@-r15 */
Lc9048f2: *stop=0xc9048f2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto Lc9048f4; /* mov.l r11,@-r15 */
Lc9048f4: *stop=0xc9048f4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto Lc9048f6; /* mov.l r10,@-r15 */
Lc9048f6: *stop=0xc9048f6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto Lc9048f8; /* mov.l r9,@-r15 */
Lc9048f8: *stop=0xc9048f8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto Lc9048fa; /* mov.l r8,@-r15 */
Lc9048fa: *stop=0xc9048fau;r[15]+=4294967220u;goto Lc9048fc; /* add #-0x4c,r15 */
Lc9048fc: *stop=0xc9048fcu;r[3]=rd(m,0x0c904a28u,4);goto Lc9048fe; /* mov.l 0x0c904a28,r3 */
Lc9048fe: *stop=0xc9048feu;r[2]=r[15];goto Lc904900; /* mov r15,r2 */
Lc904900: *stop=0xc904900u;r[2]+=64u;goto Lc904902; /* add #0x40,r2 */
Lc904902: *stop=0xc904902u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto Lc904904; /* mov.l @r3,r1 */
Lc904904: *stop=0xc904904u;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto Lc904906; /* mov.l @(0x4,r3),r0 */
Lc904906: *stop=0xc904906u;r[3]+=8u;goto Lc904908; /* add #0x8,r3 */
Lc904908: *stop=0xc904908u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc90490a; /* mov.l r1,@r2 */
Lc90490a: *stop=0xc90490au;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto Lc90490c; /* mov.l r0,@(0x4,r2) */
Lc90490c: *stop=0xc90490cu;r[2]+=8u;goto Lc90490e; /* add #0x8,r2 */
Lc90490e: *stop=0xc90490eu;if(!valid_ram(r[3],2u))return -3;r[1]=sx(m,r[3],2);r[3]+=2;goto Lc904910; /* mov.w @r3+,r1 */
Lc904910: *stop=0xc904910u;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[1],2);goto Lc904912; /* mov.w r1,@r2 */
Lc904912: *stop=0xc904912u;r[1]=r[15];goto Lc904914; /* mov r15,r1 */
Lc904914: *stop=0xc904914u;r[2]=rd(m,0x0c904a2cu,4);goto Lc904916; /* mov.l 0x0c904a2c,r2 */
Lc904916: *stop=0xc904916u;r[1]+=44u;goto Lc904918; /* add #0x2c,r1 */
Lc904918: *stop=0xc904918u;r[3]+=4294967286u;goto Lc90491a; /* add #-0xa,r3 */
Lc90491a: *stop=0xc90491au;if(!valid_ram(r[2],2u))return -3;r[0]=sx(m,r[2],2);goto Lc90491c; /* mov.w @r2,r0 */
Lc90491c: *stop=0xc90491cu;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[0],2);goto Lc90491e; /* mov.w r0,@r1 */
Lc90491e: *stop=0xc90491eu;if(!valid_ram(r[2]+2u,2u))return -3;r[0]=sx(m,r[2]+0x2u,2);goto Lc904920; /* mov.w @(0x2,r2),r0 */
Lc904920: *stop=0xc904920u;if(!valid_ram(r[1]+2u,2u))return -3;wr(m,r[1]+0x2u,r[0],2);goto Lc904922; /* mov.w r0,@(0x2,r1) */
Lc904922: *stop=0xc904922u;if(!valid_ram(r[2]+4u,2u))return -3;r[0]=sx(m,r[2]+0x4u,2);goto Lc904924; /* mov.w @(0x4,r2),r0 */
Lc904924: *stop=0xc904924u;if(!valid_ram(r[1]+4u,2u))return -3;wr(m,r[1]+0x4u,r[0],2);goto Lc904926; /* mov.w r0,@(0x4,r1) */
Lc904926: *stop=0xc904926u;if(!valid_ram(r[2]+6u,2u))return -3;r[0]=sx(m,r[2]+0x6u,2);goto Lc904928; /* mov.w @(0x6,r2),r0 */
Lc904928: *stop=0xc904928u;if(!valid_ram(r[1]+6u,2u))return -3;wr(m,r[1]+0x6u,r[0],2);goto Lc90492a; /* mov.w r0,@(0x6,r1) */
Lc90492a: *stop=0xc90492au;if(!valid_ram(r[2]+8u,2u))return -3;r[0]=sx(m,r[2]+0x8u,2);goto Lc90492c; /* mov.w @(0x8,r2),r0 */
Lc90492c: *stop=0xc90492cu;if(!valid_ram(r[1]+8u,2u))return -3;wr(m,r[1]+0x8u,r[0],2);goto Lc90492e; /* mov.w r0,@(0x8,r1) */
Lc90492e: *stop=0xc90492eu;if(!valid_ram(r[2]+10u,2u))return -3;r[0]=sx(m,r[2]+0xau,2);goto Lc904930; /* mov.w @(0xa,r2),r0 */
Lc904930: *stop=0xc904930u;if(!valid_ram(r[1]+10u,2u))return -3;wr(m,r[1]+0xau,r[0],2);goto Lc904932; /* mov.w r0,@(0xa,r1) */
Lc904932: *stop=0xc904932u;if(!valid_ram(r[2]+12u,2u))return -3;r[0]=sx(m,r[2]+0xcu,2);goto Lc904934; /* mov.w @(0xc,r2),r0 */
Lc904934: *stop=0xc904934u;if(!valid_ram(r[1]+12u,2u))return -3;wr(m,r[1]+0xcu,r[0],2);goto Lc904936; /* mov.w r0,@(0xc,r1) */
Lc904936: *stop=0xc904936u;if(!valid_ram(r[2]+14u,2u))return -3;r[0]=sx(m,r[2]+0xeu,2);goto Lc904938; /* mov.w @(0xe,r2),r0 */
Lc904938: *stop=0xc904938u;if(!valid_ram(r[1]+14u,2u))return -3;wr(m,r[1]+0xeu,r[0],2);goto Lc90493a; /* mov.w r0,@(0xe,r1) */
Lc90493a: *stop=0xc90493au;if(!valid_ram(r[2]+16u,2u))return -3;r[0]=sx(m,r[2]+0x10u,2);goto Lc90493c; /* mov.w @(0x10,r2),r0 */
Lc90493c: *stop=0xc90493cu;if(!valid_ram(r[1]+16u,2u))return -3;wr(m,r[1]+0x10u,r[0],2);goto Lc90493e; /* mov.w r0,@(0x10,r1) */
Lc90493e: *stop=0xc90493eu;r[1]=r[15];goto Lc904940; /* mov r15,r1 */
Lc904940: *stop=0xc904940u;r[0]=rd(m,0x0c904a30u,4);goto Lc904942; /* mov.l 0x0c904a30,r0 */
Lc904942: *stop=0xc904942u;r[1]+=20u;goto Lc904944; /* add #0x14,r1 */
Lc904944: *stop=0xc904944u;if(!valid_ram(r[0],4u))return -3;r[3]=rd(m,r[0],4);goto Lc904946; /* mov.l @r0,r3 */
Lc904946: *stop=0xc904946u;if(!valid_ram(r[0]+4u,4u))return -3;r[2]=rd(m,r[0]+0x4u,4);goto Lc904948; /* mov.l @(0x4,r0),r2 */
Lc904948: *stop=0xc904948u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[3],4);goto Lc90494a; /* mov.l r3,@r1 */
Lc90494a: *stop=0xc90494au;if(!valid_ram(r[1]+4u,4u))return -3;wr(m,r[1]+0x4u,r[2],4);goto Lc90494c; /* mov.l r2,@(0x4,r1) */
Lc90494c: *stop=0xc90494cu;if(!valid_ram(r[0]+12u,4u))return -3;r[2]=rd(m,r[0]+0xcu,4);goto Lc90494e; /* mov.l @(0xc,r0),r2 */
Lc90494e: *stop=0xc90494eu;if(!valid_ram(r[0]+8u,4u))return -3;r[3]=rd(m,r[0]+0x8u,4);goto Lc904950; /* mov.l @(0x8,r0),r3 */
Lc904950: *stop=0xc904950u;if(!valid_ram(r[1]+8u,4u))return -3;wr(m,r[1]+0x8u,r[3],4);goto Lc904952; /* mov.l r3,@(0x8,r1) */
Lc904952: *stop=0xc904952u;if(!valid_ram(r[1]+12u,4u))return -3;wr(m,r[1]+0xcu,r[2],4);goto Lc904954; /* mov.l r2,@(0xc,r1) */
Lc904954: *stop=0xc904954u;if(!valid_ram(r[0]+20u,4u))return -3;r[2]=rd(m,r[0]+0x14u,4);goto Lc904956; /* mov.l @(0x14,r0),r2 */
Lc904956: *stop=0xc904956u;if(!valid_ram(r[0]+16u,4u))return -3;r[3]=rd(m,r[0]+0x10u,4);goto Lc904958; /* mov.l @(0x10,r0),r3 */
Lc904958: *stop=0xc904958u;r[0]=4294967276u;goto Lc90495a; /* mov #-0x14,r0 */
Lc90495a: *stop=0xc90495au;r[0]=r[0]&255u;goto Lc90495c; /* extu.b r0,r0 */
Lc90495c: *stop=0xc90495cu;if(!valid_ram(r[1]+16u,4u))return -3;wr(m,r[1]+0x10u,r[3],4);goto Lc90495e; /* mov.l r3,@(0x10,r1) */
Lc90495e: *stop=0xc90495eu;r[3]=2u;goto Lc904960; /* mov #0x2,r3 */
Lc904960: *stop=0xc904960u;r[3]<<=8;goto Lc904962; /* shll8 r3 */
Lc904962: *stop=0xc904962u;if(!valid_ram(r[1]+20u,4u))return -3;wr(m,r[1]+0x14u,r[2],4);goto Lc904964; /* mov.l r2,@(0x14,r1) */
Lc904964: *stop=0xc904964u;if(!valid_ram(r[0]+r[5],4u))return -3;r[12]=rd(m,r[5]+r[0],4);goto Lc904966; /* mov.l @(r0,r5),r12 */
Lc904966: *stop=0xc904966u;r[3]+=72u;goto Lc904968; /* add #0x48,r3 */
Lc904968: *stop=0xc904968u;r[3]+=r[5];goto Lc90496a; /* add r5,r3 */
Lc90496a: *stop=0xc90496au;if(!valid_ram(r[12],1u))return -3;r[4]=sx(m,r[12],1);goto Lc90496c; /* mov.b @r12,r4 */
Lc90496c: *stop=0xc90496cu;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[3],4);goto Lc90496e; /* mov.l r3,@(0x4,r15) */
Lc90496e: *stop=0xc90496eu;r[9]=r[4]&255u;goto Lc904970; /* extu.b r4,r9 */
Lc904970: *stop=0xc904970u;r[0]=r[9];goto Lc904972; /* mov r9,r0 */
Lc904972: *stop=0xc904972u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto Lc904974; /* cmp/eq #0x6,r0 */
Lc904974: if(!(*sr&1u))goto Lc90497a;goto Lc904976; /* bf 0x0c90497a */
Lc904976: *stop=0xc904978u;goto Lc904ad8; /* bra 0x0c904ad8 */
Lc90497a: *stop=0xc90497au;r[11]=rd(m,0x0c904a38u,4);goto Lc90497c; /* mov.l 0x0c904a38,r11 */
Lc90497c: *stop=0xc90497cu;r[4]=4294967295u;goto Lc90497e; /* mov #-0x1,r4 */
Lc90497e: *stop=0xc90497eu;r[10]=rd(m,0x0c904a34u,4);goto Lc904980; /* mov.l 0x0c904a34,r10 */
Lc904980: *stop=0xc904980u;r[4]=r[4]&255u;goto Lc904982; /* extu.b r4,r4 */
Lc904982: *stop=0xc904982u;r[6]=0u;goto Lc904984; /* mov #0x0,r6 */
Lc904984: *stop=0xc904986u;r[7]=r[6];goto Lc904a06; /* bra 0x0c904a06 */
Lc904988: *stop=0xc904988u;r[3]=r[15];goto Lc90498a; /* mov r15,r3 */
Lc90498a: *stop=0xc90498au;r[3]+=64u;goto Lc90498c; /* add #0x40,r3 */
Lc90498c: *stop=0xc90498cu;r[13]=r[15];goto Lc90498e; /* mov r15,r13 */
Lc90498e: *stop=0xc90498eu;r[3]+=r[7];goto Lc904990; /* add r7,r3 */
Lc904990: *stop=0xc904990u;r[13]+=20u;goto Lc904992; /* add #0x14,r13 */
Lc904992: *stop=0xc904992u;if(!valid_ram(r[3],2u))return -3;r[3]=sx(m,r[3],2);goto Lc904994; /* mov.w @r3,r3 */
Lc904994: *stop=0xc904994u;r[13]+=r[7];goto Lc904996; /* add r7,r13 */
Lc904996: *stop=0xc904996u;if(!valid_ram(r[15],2u))return -3;wr(m,r[15],r[3],2);goto Lc904998; /* mov.w r3,@r15 */
Lc904998: *stop=0xc904998u;r[3]=r[12];goto Lc90499a; /* mov r12,r3 */
Lc90499a: *stop=0xc90499au;if(!valid_ram(r[13],2u))return -3;r[13]=sx(m,r[13],2);goto Lc90499c; /* mov.w @r13,r13 */
Lc90499c: *stop=0xc90499cu;r[3]+=76u;goto Lc90499e; /* add #0x4c,r3 */
Lc90499e: *stop=0xc90499eu;r[5]=(uint32_t)(int32_t)(int16_t)r[13];goto Lc9049a0; /* exts.w r13,r5 */
Lc9049a0: *stop=0xc9049a0u;r[5]+=r[3];goto Lc9049a2; /* add r3,r5 */
Lc9049a2: *stop=0xc9049a2u;if(!valid_ram(r[5],1u))return -3;r[5]=sx(m,r[5],1);goto Lc9049a4; /* mov.b @r5,r5 */
Lc9049a4: *stop=0xc9049a4u;r[0]=(uint32_t)(int32_t)(int16_t)r[5];goto Lc9049a6; /* exts.w r5,r0 */
Lc9049a6: *stop=0xc9049a6u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc9049a8; /* cmp/eq #-0x1,r0 */
Lc9049a8: if((*sr&1u))goto Lc904a04;goto Lc9049aa; /* bt 0x0c904a04 */
Lc9049aa: *stop=0xc9049aau;r[8]=(uint32_t)(int32_t)(int16_t)r[5];goto Lc9049ac; /* exts.w r5,r8 */
Lc9049ac: *stop=0xc9049acu;r[3]=r[8];goto Lc9049ae; /* mov r8,r3 */
Lc9049ae: *stop=0xc9049aeu;*sr=(*sr&~1u)|(r[8]>>31);r[8]<<=1;goto Lc9049b0; /* shll r8 */
Lc9049b0: *stop=0xc9049b0u;r[8]+=r[3];goto Lc9049b2; /* add r3,r8 */
Lc9049b2: *stop=0xc9049b2u;r[14]=(uint32_t)(int32_t)(int16_t)r[13];goto Lc9049b4; /* exts.w r13,r14 */
Lc9049b4: *stop=0xc9049b4u;r[8]<<=2;goto Lc9049b6; /* shll2 r8 */
Lc9049b6: *stop=0xc9049b6u;r[14]<<=2;goto Lc9049b8; /* shll2 r14 */
Lc9049b8: *stop=0xc9049b8u;r[8]<<=2;goto Lc9049ba; /* shll2 r8 */
Lc9049ba: *stop=0xc9049bau;*sr=(*sr&~1u)|(r[8]>>31);r[8]<<=1;goto Lc9049bc; /* shll r8 */
Lc9049bc: *stop=0xc9049bcu;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto Lc9049be; /* shll r14 */
Lc9049be: *stop=0xc9049beu;*sr=(*sr&~1u)|!(r[9]&r[9]);goto Lc9049c0; /* tst r9,r9 */
Lc9049c0: if(!(*sr&1u))goto Lc9049c8;goto Lc9049c2; /* bf 0x0c9049c8 */
Lc9049c2: *stop=0xc9049c2u;r[5]=r[8];goto Lc9049c4; /* mov r8,r5 */
Lc9049c4: *stop=0xc9049c6u;r[5]+=r[10];goto Lc9049cc; /* bra 0x0c9049cc */
Lc9049c8: *stop=0xc9049c8u;r[5]=r[8];goto Lc9049ca; /* mov r8,r5 */
Lc9049ca: *stop=0xc9049cau;r[5]+=r[11];goto Lc9049cc; /* add r11,r5 */
Lc9049cc: *stop=0xc9049ccu;r[5]+=r[14];goto Lc9049ce; /* add r14,r5 */
Lc9049ce: *stop=0xc9049ceu;if(!valid_ram(r[5]+4u,4u))return -3;r[5]=rd(m,r[5]+0x4u,4);goto Lc9049d0; /* mov.l @(0x4,r5),r5 */
Lc9049d0: *stop=0xc9049d0u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto Lc9049d2; /* tst r5,r5 */
Lc9049d2: if((*sr&1u))goto Lc904a04;goto Lc9049d4; /* bt 0x0c904a04 */
Lc9049d4: *stop=0xc9049d4u;if(!valid_ram(r[15],2u))return -3;r[3]=sx(m,r[15],2);goto Lc9049d6; /* mov.w @r15,r3 */
Lc9049d6: *stop=0xc9049d6u;if(!valid_ram(r[15]+4u,4u))return -3;r[2]=rd(m,r[15]+0x4u,4);goto Lc9049d8; /* mov.l @(0x4,r15),r2 */
Lc9049d8: *stop=0xc9049d8u;r[3]<<=2;goto Lc9049da; /* shll2 r3 */
Lc9049da: *stop=0xc9049dau;r[2]+=4u;goto Lc9049dc; /* add #0x4,r2 */
Lc9049dc: *stop=0xc9049dcu;r[3]+=r[2];goto Lc9049de; /* add r2,r3 */
Lc9049de: *stop=0xc9049deu;if(!valid_ram(r[5]+4u,4u))return -3;r[2]=rd(m,r[5]+0x4u,4);goto Lc9049e0; /* mov.l @(0x4,r5),r2 */
Lc9049e0: *stop=0xc9049e0u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto Lc9049e2; /* mov.l @r3,r1 */
Lc9049e2: *stop=0xc9049e2u;if(!valid_ram(r[1]+4u,4u))return -3;wr(m,r[1]+0x4u,r[2],4);goto Lc9049e4; /* mov.l r2,@(0x4,r1) */
Lc9049e4: *stop=0xc9049e4u;if(!valid_ram(r[5]+4u,4u))return -3;r[14]=rd(m,r[5]+0x4u,4);goto Lc9049e6; /* mov.l @(0x4,r5),r14 */
Lc9049e6: *stop=0xc9049e6u;if(!valid_ram(r[14]+22u,2u))return -3;r[0]=sx(m,r[14]+0x16u,2);goto Lc9049e8; /* mov.w @(0x16,r14),r0 */
Lc9049e8: *stop=0xc9049e8u;if(!valid_ram(r[14]+16u,4u))return -3;r[5]=rd(m,r[14]+0x10u,4);goto Lc9049ea; /* mov.l @(0x10,r14),r5 */
Lc9049ea: *stop=0xc9049eau;r[8]=r[0];goto Lc9049ec; /* mov r0,r8 */
Lc9049ec: *stop=0xc9049ecu;r[13]=r[8]&65535u;goto Lc9049ee; /* extu.w r8,r13 */
Lc9049ee: *stop=0xc9049eeu;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto Lc9049f0; /* cmp/pl r13 */
Lc9049f0: take=!(*sr&1u);*stop=0xc9049f2u;r[14]=r[6];if(take)goto Lc904a04;goto Lc9049f4; /* bf/s 0x0c904a04 */
Lc9049f4: *stop=0xc9049f4u;r[14]+=1u;goto Lc9049f6; /* add #0x1,r14 */
Lc9049f6: *stop=0xc9049f6u;r[0]=r[4];goto Lc9049f8; /* mov r4,r0 */
Lc9049f8: *stop=0xc9049f8u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[13]);goto Lc9049fa; /* cmp/ge r13,r14 */
Lc9049fa: *stop=0xc9049fau;if(!valid_ram(r[5]+2u,1u))return -3;wr(m,r[5]+0x2u,r[0],1);goto Lc9049fc; /* mov.b r0,@(0x2,r5) */
Lc9049fc: *stop=0xc9049fcu;if(!valid_ram(r[5]+1u,1u))return -3;wr(m,r[5]+0x1u,r[0],1);goto Lc9049fe; /* mov.b r0,@(0x1,r5) */
Lc9049fe: *stop=0xc9049feu;if(!valid_ram(r[5],1u))return -3;wr(m,r[5],r[0],1);goto Lc904a00; /* mov.b r0,@r5 */
Lc904a00: take=!(*sr&1u);*stop=0xc904a02u;r[5]+=20u;if(take)goto Lc9049f4;goto Lc904a04; /* bf/s 0x0c9049f4 */
Lc904a04: *stop=0xc904a04u;r[7]+=2u;goto Lc904a06; /* add #0x2,r7 */
Lc904a06: *stop=0xc904a06u;r[2]=6u;goto Lc904a08; /* mov #0x6,r2 */
Lc904a08: *stop=0xc904a08u;*sr=(*sr&~1u)|(r[7]>=r[2]);goto Lc904a0a; /* cmp/hs r2,r7 */
Lc904a0a: if(!(*sr&1u))goto Lc904988;goto Lc904a0c; /* bf 0x0c904988 */
Lc904a0c: *stop=0xc904a0cu;r[3]=r[15];goto Lc904a0e; /* mov r15,r3 */
Lc904a0e: *stop=0xc904a0eu;r[5]=24u;goto Lc904a10; /* mov #0x18,r5 */
Lc904a10: *stop=0xc904a10u;r[3]+=26u;goto Lc904a12; /* add #0x1a,r3 */
Lc904a12: *stop=0xc904a12u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto Lc904a14; /* mov.l r5,@r15 */
Lc904a14: *stop=0xc904a14u;r[13]=r[6];goto Lc904a16; /* mov r6,r13 */
Lc904a16: *stop=0xc904a16u;r[8]=r[5];goto Lc904a18; /* mov r5,r8 */
Lc904a18: *stop=0xc904a1au;if(!valid_ram(r[15]+12u,4u))return -3;wr(m,r[15]+0xcu,r[3],4);goto Lc904ad2; /* bra 0x0c904ad2 */
Lc904a40: *stop=0xc904a40u;if(!valid_ram(r[15]+12u,4u))return -3;r[5]=rd(m,r[15]+0xcu,4);goto Lc904a42; /* mov.l @(0xc,r15),r5 */
Lc904a42: *stop=0xc904a42u;r[2]=r[12];goto Lc904a44; /* mov r12,r2 */
Lc904a44: *stop=0xc904a44u;r[2]+=76u;goto Lc904a46; /* add #0x4c,r2 */
Lc904a46: *stop=0xc904a46u;r[14]=r[13];goto Lc904a48; /* mov r13,r14 */
Lc904a48: *stop=0xc904a48u;r[5]+=2u;goto Lc904a4a; /* add #0x2,r5 */
Lc904a4a: *stop=0xc904a4au;r[3]=r[15];goto Lc904a4c; /* mov r15,r3 */
Lc904a4c: *stop=0xc904a4cu;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto Lc904a4e; /* shll r14 */
Lc904a4e: *stop=0xc904a4eu;r[3]+=44u;goto Lc904a50; /* add #0x2c,r3 */
Lc904a50: *stop=0xc904a50u;r[14]+=r[3];goto Lc904a52; /* add r3,r14 */
Lc904a52: *stop=0xc904a52u;if(!valid_ram(r[14],2u))return -3;r[14]=sx(m,r[14],2);goto Lc904a54; /* mov.w @r14,r14 */
Lc904a54: *stop=0xc904a54u;if(!valid_ram(r[15]+12u,4u))return -3;wr(m,r[15]+0xcu,r[5],4);goto Lc904a56; /* mov.l r5,@(0xc,r15) */
Lc904a56: *stop=0xc904a56u;r[5]+=4294967294u;goto Lc904a58; /* add #-0x2,r5 */
Lc904a58: *stop=0xc904a58u;if(!valid_ram(r[5],2u))return -3;r[5]=sx(m,r[5],2);goto Lc904a5a; /* mov.w @r5,r5 */
Lc904a5a: *stop=0xc904a5au;r[5]+=r[2];goto Lc904a5c; /* add r2,r5 */
Lc904a5c: *stop=0xc904a5cu;if(!valid_ram(r[5],1u))return -3;r[5]=sx(m,r[5],1);goto Lc904a5e; /* mov.b @r5,r5 */
Lc904a5e: *stop=0xc904a5eu;r[0]=(uint32_t)(int32_t)(int16_t)r[5];goto Lc904a60; /* exts.w r5,r0 */
Lc904a60: *stop=0xc904a60u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc904a62; /* cmp/eq #-0x1,r0 */
Lc904a62: if((*sr&1u))goto Lc904ac8;goto Lc904a64; /* bt 0x0c904ac8 */
Lc904a64: *stop=0xc904a64u;r[7]=(uint32_t)(int32_t)(int16_t)r[5];goto Lc904a66; /* exts.w r5,r7 */
Lc904a66: *stop=0xc904a66u;r[3]=r[7];goto Lc904a68; /* mov r7,r3 */
Lc904a68: *stop=0xc904a68u;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto Lc904a6a; /* shll r7 */
Lc904a6a: *stop=0xc904a6au;r[7]+=r[3];goto Lc904a6c; /* add r3,r7 */
Lc904a6c: *stop=0xc904a6cu;r[7]<<=2;goto Lc904a6e; /* shll2 r7 */
Lc904a6e: *stop=0xc904a6eu;r[7]<<=2;goto Lc904a70; /* shll2 r7 */
Lc904a70: *stop=0xc904a70u;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto Lc904a72; /* shll r7 */
Lc904a72: *stop=0xc904a72u;*sr=(*sr&~1u)|!(r[9]&r[9]);goto Lc904a74; /* tst r9,r9 */
Lc904a74: if(!(*sr&1u))goto Lc904a82;goto Lc904a76; /* bf 0x0c904a82 */
Lc904a76: *stop=0xc904a76u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto Lc904a78; /* mov.l @r15,r3 */
Lc904a78: *stop=0xc904a78u;r[5]=r[7];goto Lc904a7a; /* mov r7,r5 */
Lc904a7a: *stop=0xc904a7au;r[5]+=r[10];goto Lc904a7c; /* add r10,r5 */
Lc904a7c: *stop=0xc904a7cu;r[5]+=r[3];goto Lc904a7e; /* add r3,r5 */
Lc904a7e: *stop=0xc904a80u;goto Lc904a88; /* bra 0x0c904a88 */
Lc904a82: *stop=0xc904a82u;r[5]=r[7];goto Lc904a84; /* mov r7,r5 */
Lc904a84: *stop=0xc904a84u;r[5]+=r[11];goto Lc904a86; /* add r11,r5 */
Lc904a86: *stop=0xc904a86u;r[5]+=r[8];goto Lc904a88; /* add r8,r5 */
Lc904a88: *stop=0xc904a88u;if(!valid_ram(r[5]+4u,4u))return -3;r[5]=rd(m,r[5]+0x4u,4);goto Lc904a8a; /* mov.l @(0x4,r5),r5 */
Lc904a8a: *stop=0xc904a8au;*sr=(*sr&~1u)|!(r[5]&r[5]);goto Lc904a8c; /* tst r5,r5 */
Lc904a8c: if((*sr&1u))goto Lc904ac8;goto Lc904a8e; /* bt 0x0c904ac8 */
Lc904a8e: *stop=0xc904a8eu;if(!valid_ram(r[15]+4u,4u))return -3;r[0]=rd(m,r[15]+0x4u,4);goto Lc904a90; /* mov.l @(0x4,r15),r0 */
Lc904a90: *stop=0xc904a90u;r[14]=(uint32_t)(int32_t)(int16_t)r[14];goto Lc904a92; /* exts.w r14,r14 */
Lc904a92: *stop=0xc904a92u;r[14]<<=2;goto Lc904a94; /* shll2 r14 */
Lc904a94: *stop=0xc904a94u;r[3]=r[0];goto Lc904a96; /* mov r0,r3 */
Lc904a96: *stop=0xc904a96u;r[3]+=4u;goto Lc904a98; /* add #0x4,r3 */
Lc904a98: *stop=0xc904a98u;r[14]+=r[3];goto Lc904a9a; /* add r3,r14 */
Lc904a9a: *stop=0xc904a9au;if(!valid_ram(r[5]+44u,4u))return -3;r[3]=rd(m,r[5]+0x2cu,4);goto Lc904a9c; /* mov.l @(0x2c,r5),r3 */
Lc904a9c: *stop=0xc904a9cu;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto Lc904a9e; /* mov.l @r14,r2 */
Lc904a9e: *stop=0xc904a9eu;if(!valid_ram(r[2]+44u,4u))return -3;wr(m,r[2]+0x2cu,r[3],4);goto Lc904aa0; /* mov.l r3,@(0x2c,r2) */
Lc904aa0: *stop=0xc904aa0u;if(!valid_ram(r[5]+44u,4u))return -3;r[7]=rd(m,r[5]+0x2cu,4);goto Lc904aa2; /* mov.l @(0x2c,r5),r7 */
Lc904aa2: *stop=0xc904aa2u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[7],4);goto Lc904aa4; /* mov.l r7,@(0x8,r15) */
Lc904aa4: *stop=0xc904aa4u;if(!valid_ram(r[7]+4u,4u))return -3;r[7]=rd(m,r[7]+0x4u,4);goto Lc904aa6; /* mov.l @(0x4,r7),r7 */
Lc904aa6: *stop=0xc904aa6u;if(!valid_ram(r[7]+22u,2u))return -3;r[0]=sx(m,r[7]+0x16u,2);goto Lc904aa8; /* mov.w @(0x16,r7),r0 */
Lc904aa8: *stop=0xc904aa8u;if(!valid_ram(r[15]+16u,2u))return -3;wr(m,r[15]+0x10u,r[0],2);goto Lc904aaa; /* mov.w r0,@(0x10,r15) */
Lc904aaa: *stop=0xc904aaau;if(!valid_ram(r[15]+16u,2u))return -3;r[0]=sx(m,r[15]+0x10u,2);goto Lc904aac; /* mov.w @(0x10,r15),r0 */
Lc904aac: *stop=0xc904aacu;if(!valid_ram(r[7]+16u,4u))return -3;r[5]=rd(m,r[7]+0x10u,4);goto Lc904aae; /* mov.l @(0x10,r7),r5 */
Lc904aae: *stop=0xc904aaeu;r[14]=r[0];goto Lc904ab0; /* mov r0,r14 */
Lc904ab0: *stop=0xc904ab0u;r[14]=r[14]&65535u;goto Lc904ab2; /* extu.w r14,r14 */
Lc904ab2: *stop=0xc904ab2u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto Lc904ab4; /* cmp/pl r14 */
Lc904ab4: take=!(*sr&1u);*stop=0xc904ab6u;r[7]=r[6];if(take)goto Lc904ac8;goto Lc904ab8; /* bf/s 0x0c904ac8 */
Lc904ab8: *stop=0xc904ab8u;r[7]+=1u;goto Lc904aba; /* add #0x1,r7 */
Lc904aba: *stop=0xc904abau;r[0]=r[4];goto Lc904abc; /* mov r4,r0 */
Lc904abc: *stop=0xc904abcu;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[14]);goto Lc904abe; /* cmp/ge r14,r7 */
Lc904abe: *stop=0xc904abeu;if(!valid_ram(r[5]+2u,1u))return -3;wr(m,r[5]+0x2u,r[0],1);goto Lc904ac0; /* mov.b r0,@(0x2,r5) */
Lc904ac0: *stop=0xc904ac0u;if(!valid_ram(r[5]+1u,1u))return -3;wr(m,r[5]+0x1u,r[0],1);goto Lc904ac2; /* mov.b r0,@(0x1,r5) */
Lc904ac2: *stop=0xc904ac2u;if(!valid_ram(r[5],1u))return -3;wr(m,r[5],r[0],1);goto Lc904ac4; /* mov.b r0,@r5 */
Lc904ac4: take=!(*sr&1u);*stop=0xc904ac6u;r[5]+=20u;if(take)goto Lc904ab8;goto Lc904ac8; /* bf/s 0x0c904ab8 */
Lc904ac8: *stop=0xc904ac8u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto Lc904aca; /* mov.l @r15,r2 */
Lc904aca: *stop=0xc904acau;r[8]+=8u;goto Lc904acc; /* add #0x8,r8 */
Lc904acc: *stop=0xc904accu;r[13]+=1u;goto Lc904ace; /* add #0x1,r13 */
Lc904ace: *stop=0xc904aceu;r[2]+=8u;goto Lc904ad0; /* add #0x8,r2 */
Lc904ad0: *stop=0xc904ad0u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);goto Lc904ad2; /* mov.l r2,@r15 */
Lc904ad2: *stop=0xc904ad2u;r[3]=9u;goto Lc904ad4; /* mov #0x9,r3 */
Lc904ad4: *stop=0xc904ad4u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[3]);goto Lc904ad6; /* cmp/ge r3,r13 */
Lc904ad6: if(!(*sr&1u))goto Lc904a40;goto Lc904ad8; /* bf 0x0c904a40 */
Lc904ad8: *stop=0xc904ad8u;r[15]+=76u;goto Lc904ada; /* add #0x4c,r15 */
Lc904ada: *stop=0xc904adau;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto Lc904adc; /* mov.l @r15+,r8 */
Lc904adc: *stop=0xc904adcu;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto Lc904ade; /* mov.l @r15+,r9 */
Lc904ade: *stop=0xc904adeu;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto Lc904ae0; /* mov.l @r15+,r10 */
Lc904ae0: *stop=0xc904ae0u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto Lc904ae2; /* mov.l @r15+,r11 */
Lc904ae2: *stop=0xc904ae2u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto Lc904ae4; /* mov.l @r15+,r12 */
Lc904ae4: *stop=0xc904ae4u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto Lc904ae6; /* mov.l @r15+,r13 */
Lc904ae6: *stop=0xc904ae6u;if(*pr!=entry_pr)return -3;*stop=0xc904ae8u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_tail_palette_0c90fb82(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc90fb82;
Lc90fb82: *stop=0xc90fb82u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc90fb84; /* mov.l r14,@-r15 */
Lc90fb84: *stop=0xc90fb84u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto Lc90fb86; /* mov.l r13,@-r15 */
Lc90fb86: *stop=0xc90fb86u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto Lc90fb88; /* mov.l r12,@-r15 */
Lc90fb88: *stop=0xc90fb88u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto Lc90fb8a; /* mov.l r10,@-r15 */
Lc90fb8a: *stop=0xc90fb8au;r[15]+=4294967232u;goto Lc90fb8c; /* add #-0x40,r15 */
Lc90fb8c: *stop=0xc90fb8cu;r[3]=rd(m,0x0c90fc7cu,4);goto Lc90fb8e; /* mov.l 0x0c90fc7c,r3 */
Lc90fb8e: *stop=0xc90fb8eu;r[2]=r[15];goto Lc90fb90; /* mov r15,r2 */
Lc90fb90: *stop=0xc90fb90u;r[2]+=8u;goto Lc90fb92; /* add #0x8,r2 */
Lc90fb92: *stop=0xc90fb92u;r[4]=r[15];goto Lc90fb94; /* mov r15,r4 */
Lc90fb94: *stop=0xc90fb94u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto Lc90fb96; /* mov.l @r3,r1 */
Lc90fb96: *stop=0xc90fb96u;r[4]+=8u;goto Lc90fb98; /* add #0x8,r4 */
Lc90fb98: *stop=0xc90fb98u;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto Lc90fb9a; /* mov.l @(0x4,r3),r0 */
Lc90fb9a: *stop=0xc90fb9au;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc90fb9c; /* mov.l r1,@r2 */
Lc90fb9c: *stop=0xc90fb9cu;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto Lc90fb9e; /* mov.l r0,@(0x4,r2) */
Lc90fb9e: *stop=0xc90fb9eu;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto Lc90fba0; /* mov.l @(0x8,r3),r1 */
Lc90fba0: *stop=0xc90fba0u;if(!valid_ram(r[3]+12u,4u))return -3;r[0]=rd(m,r[3]+0xcu,4);goto Lc90fba2; /* mov.l @(0xc,r3),r0 */
Lc90fba2: *stop=0xc90fba2u;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto Lc90fba4; /* mov.l r1,@(0x8,r2) */
Lc90fba4: *stop=0xc90fba4u;if(!valid_ram(r[2]+12u,4u))return -3;wr(m,r[2]+0xcu,r[0],4);goto Lc90fba6; /* mov.l r0,@(0xc,r2) */
Lc90fba6: *stop=0xc90fba6u;if(!valid_ram(r[3]+16u,4u))return -3;r[1]=rd(m,r[3]+0x10u,4);goto Lc90fba8; /* mov.l @(0x10,r3),r1 */
Lc90fba8: *stop=0xc90fba8u;if(!valid_ram(r[3]+20u,4u))return -3;r[0]=rd(m,r[3]+0x14u,4);goto Lc90fbaa; /* mov.l @(0x14,r3),r0 */
Lc90fbaa: *stop=0xc90fbaau;if(!valid_ram(r[2]+16u,4u))return -3;wr(m,r[2]+0x10u,r[1],4);goto Lc90fbac; /* mov.l r1,@(0x10,r2) */
Lc90fbac: *stop=0xc90fbacu;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[0],4);goto Lc90fbae; /* mov.l r0,@(0x14,r2) */
Lc90fbae: *stop=0xc90fbaeu;if(!valid_ram(r[3]+24u,4u))return -3;r[1]=rd(m,r[3]+0x18u,4);goto Lc90fbb0; /* mov.l @(0x18,r3),r1 */
Lc90fbb0: *stop=0xc90fbb0u;if(!valid_ram(r[3]+28u,4u))return -3;r[0]=rd(m,r[3]+0x1cu,4);goto Lc90fbb2; /* mov.l @(0x1c,r3),r0 */
Lc90fbb2: *stop=0xc90fbb2u;if(!valid_ram(r[2]+24u,4u))return -3;wr(m,r[2]+0x18u,r[1],4);goto Lc90fbb4; /* mov.l r1,@(0x18,r2) */
Lc90fbb4: *stop=0xc90fbb4u;if(!valid_ram(r[2]+28u,4u))return -3;wr(m,r[2]+0x1cu,r[0],4);goto Lc90fbb6; /* mov.l r0,@(0x1c,r2) */
Lc90fbb6: *stop=0xc90fbb6u;if(!valid_ram(r[3]+32u,4u))return -3;r[1]=rd(m,r[3]+0x20u,4);goto Lc90fbb8; /* mov.l @(0x20,r3),r1 */
Lc90fbb8: *stop=0xc90fbb8u;if(!valid_ram(r[3]+36u,4u))return -3;r[0]=rd(m,r[3]+0x24u,4);goto Lc90fbba; /* mov.l @(0x24,r3),r0 */
Lc90fbba: *stop=0xc90fbbau;if(!valid_ram(r[2]+32u,4u))return -3;wr(m,r[2]+0x20u,r[1],4);goto Lc90fbbc; /* mov.l r1,@(0x20,r2) */
Lc90fbbc: *stop=0xc90fbbcu;if(!valid_ram(r[2]+36u,4u))return -3;wr(m,r[2]+0x24u,r[0],4);goto Lc90fbbe; /* mov.l r0,@(0x24,r2) */
Lc90fbbe: *stop=0xc90fbbeu;if(!valid_ram(r[3]+40u,4u))return -3;r[1]=rd(m,r[3]+0x28u,4);goto Lc90fbc0; /* mov.l @(0x28,r3),r1 */
Lc90fbc0: *stop=0xc90fbc0u;if(!valid_ram(r[3]+44u,4u))return -3;r[0]=rd(m,r[3]+0x2cu,4);goto Lc90fbc2; /* mov.l @(0x2c,r3),r0 */
Lc90fbc2: *stop=0xc90fbc2u;if(!valid_ram(r[2]+40u,4u))return -3;wr(m,r[2]+0x28u,r[1],4);goto Lc90fbc4; /* mov.l r1,@(0x28,r2) */
Lc90fbc4: *stop=0xc90fbc4u;if(!valid_ram(r[2]+44u,4u))return -3;wr(m,r[2]+0x2cu,r[0],4);goto Lc90fbc6; /* mov.l r0,@(0x2c,r2) */
Lc90fbc6: *stop=0xc90fbc6u;if(!valid_ram(r[3]+48u,4u))return -3;r[1]=rd(m,r[3]+0x30u,4);goto Lc90fbc8; /* mov.l @(0x30,r3),r1 */
Lc90fbc8: *stop=0xc90fbc8u;if(!valid_ram(r[3]+52u,4u))return -3;r[0]=rd(m,r[3]+0x34u,4);goto Lc90fbca; /* mov.l @(0x34,r3),r0 */
Lc90fbca: *stop=0xc90fbcau;if(!valid_ram(r[2]+48u,4u))return -3;wr(m,r[2]+0x30u,r[1],4);goto Lc90fbcc; /* mov.l r1,@(0x30,r2) */
Lc90fbcc: *stop=0xc90fbccu;if(!valid_ram(r[2]+52u,4u))return -3;wr(m,r[2]+0x34u,r[0],4);goto Lc90fbce; /* mov.l r0,@(0x34,r2) */
Lc90fbce: *stop=0xc90fbceu;r[1]=r[15];goto Lc90fbd0; /* mov r15,r1 */
Lc90fbd0: *stop=0xc90fbd0u;r[2]=rd(m,0x0c90fc80u,4);goto Lc90fbd2; /* mov.l 0x0c90fc80,r2 */
Lc90fbd2: *stop=0xc90fbd2u;r[6]=r[4];goto Lc90fbd4; /* mov r4,r6 */
Lc90fbd4: *stop=0xc90fbd4u;r[10]=2u;goto Lc90fbd6; /* mov #0x2,r10 */
Lc90fbd6: *stop=0xc90fbd6u;r[7]=r[15];goto Lc90fbd8; /* mov r15,r7 */
Lc90fbd8: *stop=0xc90fbd8u;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto Lc90fbda; /* mov.l @r2,r0 */
Lc90fbda: *stop=0xc90fbdau;r[10]<<=8;goto Lc90fbdc; /* shll8 r10 */
Lc90fbdc: *stop=0xc90fbdcu;if(!valid_ram(r[2]+4u,4u))return -3;r[3]=rd(m,r[2]+0x4u,4);goto Lc90fbde; /* mov.l @(0x4,r2),r3 */
Lc90fbde: *stop=0xc90fbdeu;r[10]+=72u;goto Lc90fbe0; /* add #0x48,r10 */
Lc90fbe0: *stop=0xc90fbe0u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[0],4);goto Lc90fbe2; /* mov.l r0,@r1 */
Lc90fbe2: *stop=0xc90fbe2u;r[0]=4294967276u;goto Lc90fbe4; /* mov #-0x14,r0 */
Lc90fbe4: *stop=0xc90fbe4u;r[0]=r[0]&255u;goto Lc90fbe6; /* extu.b r0,r0 */
Lc90fbe6: *stop=0xc90fbe6u;if(!valid_ram(r[1]+4u,4u))return -3;wr(m,r[1]+0x4u,r[3],4);goto Lc90fbe8; /* mov.l r3,@(0x4,r1) */
Lc90fbe8: *stop=0xc90fbe8u;if(!valid_ram(r[0]+r[5],4u))return -3;r[4]=rd(m,r[5]+r[0],4);goto Lc90fbea; /* mov.l @(r0,r5),r4 */
Lc90fbea: *stop=0xc90fbeau;r[0]=2u;goto Lc90fbec; /* mov #0x2,r0 */
Lc90fbec: *stop=0xc90fbecu;r[0]<<=8;goto Lc90fbee; /* shll8 r0 */
Lc90fbee: *stop=0xc90fbeeu;r[0]+=4294967186u;goto Lc90fbf0; /* add #-0x6e,r0 */
Lc90fbf0: *stop=0xc90fbf0u;if(!valid_ram(r[0]+r[4],1u))return -3;r[4]=sx(m,r[4]+r[0],1);goto Lc90fbf2; /* mov.b @(r0,r4),r4 */
Lc90fbf2: *stop=0xc90fbf2u;r[1]=3u;goto Lc90fbf4; /* mov #0x3,r1 */
Lc90fbf4: *stop=0xc90fbf4u;r[10]+=r[5];goto Lc90fbf6; /* add r5,r10 */
Lc90fbf6: *stop=0xc90fbf6u;r[3]=r[4]&255u;goto Lc90fbf8; /* extu.b r4,r3 */
Lc90fbf8: *stop=0xc90fbf8u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[1]);goto Lc90fbfa; /* cmp/gt r1,r3 */
Lc90fbfa: take=!(*sr&1u);*stop=0xc90fbfcu;r[14]=0u;if(take)goto Lc90fc00;goto Lc90fbfe; /* bf/s 0x0c90fc00 */
Lc90fbfe: *stop=0xc90fbfeu;r[4]=r[14];goto Lc90fc00; /* mov r14,r4 */
Lc90fc00: *stop=0xc90fc00u;if(!valid_ram(r[6],4u))return -3;r[0]=rd(m,r[6],4);goto Lc90fc02; /* mov.l @r6,r0 */
Lc90fc02: *stop=0xc90fc02u;r[13]=r[4]&255u;goto Lc90fc04; /* extu.b r4,r13 */
Lc90fc04: *stop=0xc90fc04u;r[12]=rd(m,0x0c90fc84u,4);goto Lc90fc06; /* mov.l 0x0c90fc84,r12 */
Lc90fc06: *stop=0xc90fc06u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto Lc90fc08; /* shll r13 */
Lc90fc08: *stop=0xc90fc08u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc90fc0a; /* cmp/eq #-0x1,r0 */
Lc90fc0a: take=(*sr&1u);*stop=0xc90fc0cu;r[13]+=r[7];if(take)goto Lc90fc48;goto Lc90fc0e; /* bt/s 0x0c90fc48 */
Lc90fc0e: *stop=0xc90fc0eu;if(!valid_ram(r[6],4u))return -3;r[5]=rd(m,r[6],4);goto Lc90fc10; /* mov.l @r6,r5 */
Lc90fc10: *stop=0xc90fc10u;r[3]=r[10];goto Lc90fc12; /* mov r10,r3 */
Lc90fc12: *stop=0xc90fc12u;r[3]+=4u;goto Lc90fc14; /* add #0x4,r3 */
Lc90fc14: *stop=0xc90fc14u;r[5]<<=2;goto Lc90fc16; /* shll2 r5 */
Lc90fc16: *stop=0xc90fc16u;r[5]+=r[3];goto Lc90fc18; /* add r3,r5 */
Lc90fc18: *stop=0xc90fc18u;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto Lc90fc1a; /* mov.l @r5,r5 */
Lc90fc1a: *stop=0xc90fc1au;if(!valid_ram(r[5]+4u,4u))return -3;r[4]=rd(m,r[5]+0x4u,4);goto Lc90fc1c; /* mov.l @(0x4,r5),r4 */
Lc90fc1c: *stop=0xc90fc1cu;r[5]=r[14];goto Lc90fc1e; /* mov r14,r5 */
Lc90fc1e: *stop=0xc90fc1eu;if(!valid_ram(r[4]+22u,2u))return -3;r[0]=sx(m,r[4]+0x16u,2);goto Lc90fc20; /* mov.w @(0x16,r4),r0 */
Lc90fc20: *stop=0xc90fc20u;if(!valid_ram(r[4]+16u,4u))return -3;r[1]=rd(m,r[4]+0x10u,4);goto Lc90fc22; /* mov.l @(0x10,r4),r1 */
Lc90fc22: *stop=0xc90fc22u;r[4]=0u;goto Lc90fc24; /* mov #0x0,r4 */
Lc90fc24: *stop=0xc90fc24u;r[7]=r[0]&65535u;goto Lc90fc26; /* extu.w r0,r7 */
Lc90fc26: *stop=0xc90fc26u;*sr=(*sr&~1u)|((int32_t)r[7]>0);goto Lc90fc28; /* cmp/pl r7 */
Lc90fc28: take=!(*sr&1u);*stop=0xc90fc2au;r[4]+=r[1];if(take)goto Lc90fc40;goto Lc90fc2c; /* bf/s 0x0c90fc40 */
Lc90fc2c: *stop=0xc90fc2cu;if(!valid_ram(r[13],2u))return -3;r[2]=sx(m,r[13],2);goto Lc90fc2e; /* mov.w @r13,r2 */
Lc90fc2e: *stop=0xc90fc2eu;r[5]+=1u;goto Lc90fc30; /* add #0x1,r5 */
Lc90fc30: *stop=0xc90fc30u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[7]);goto Lc90fc32; /* cmp/ge r7,r5 */
Lc90fc32: *stop=0xc90fc32u;r[2]=r[2]&65535u;goto Lc90fc34; /* extu.w r2,r2 */
Lc90fc34: *stop=0xc90fc34u;if(!valid_ram(r[4]+12u,4u))return -3;wr(m,r[4]+0xcu,r[2],4);goto Lc90fc36; /* mov.l r2,@(0xc,r4) */
Lc90fc36: *stop=0xc90fc36u;if(!valid_ram(r[4]+16u,4u))return -3;r[3]=rd(m,r[4]+0x10u,4);goto Lc90fc38; /* mov.l @(0x10,r4),r3 */
Lc90fc38: *stop=0xc90fc38u;r[3]|=r[12];goto Lc90fc3a; /* or r12,r3 */
Lc90fc3a: *stop=0xc90fc3au;if(!valid_ram(r[4]+16u,4u))return -3;wr(m,r[4]+0x10u,r[3],4);goto Lc90fc3c; /* mov.l r3,@(0x10,r4) */
Lc90fc3c: take=!(*sr&1u);*stop=0xc90fc3eu;r[4]+=20u;if(take)goto Lc90fc2c;goto Lc90fc40; /* bf/s 0x0c90fc2c */
Lc90fc40: *stop=0xc90fc40u;r[6]+=4u;goto Lc90fc42; /* add #0x4,r6 */
Lc90fc42: *stop=0xc90fc42u;if(!valid_ram(r[6],4u))return -3;r[0]=rd(m,r[6],4);goto Lc90fc44; /* mov.l @r6,r0 */
Lc90fc44: *stop=0xc90fc44u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc90fc46; /* cmp/eq #-0x1,r0 */
Lc90fc46: if(!(*sr&1u))goto Lc90fc0e;goto Lc90fc48; /* bf 0x0c90fc0e */
Lc90fc48: *stop=0xc90fc48u;r[15]+=64u;goto Lc90fc4a; /* add #0x40,r15 */
Lc90fc4a: *stop=0xc90fc4au;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto Lc90fc4c; /* mov.l @r15+,r10 */
Lc90fc4c: *stop=0xc90fc4cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto Lc90fc4e; /* mov.l @r15+,r12 */
Lc90fc4e: *stop=0xc90fc4eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto Lc90fc50; /* mov.l @r15+,r13 */
Lc90fc50: *stop=0xc90fc50u;if(*pr!=entry_pr)return -3;*stop=0xc90fc52u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_tail_material_158_0c90fca0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc90fca0;
Lc90fca0: *stop=0xc90fca0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc90fca2; /* mov.l r14,@-r15 */
Lc90fca2: *stop=0xc90fca2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto Lc90fca4; /* mov.l r13,@-r15 */
Lc90fca4: *stop=0xc90fca4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto Lc90fca6; /* mov.l r11,@-r15 */
Lc90fca6: *stop=0xc90fca6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto Lc90fca8; /* mov.l r10,@-r15 */
Lc90fca8: *stop=0xc90fca8u;r[15]+=4294967240u;goto Lc90fcaa; /* add #-0x38,r15 */
Lc90fcaa: *stop=0xc90fcaau;r[3]=rd(m,0x0c90fec0u,4);goto Lc90fcac; /* mov.l 0x0c90fec0,r3 */
Lc90fcac: *stop=0xc90fcacu;r[2]=r[15];goto Lc90fcae; /* mov r15,r2 */
Lc90fcae: *stop=0xc90fcaeu;r[10]=2u;goto Lc90fcb0; /* mov #0x2,r10 */
Lc90fcb0: *stop=0xc90fcb0u;r[4]=r[15];goto Lc90fcb2; /* mov r15,r4 */
Lc90fcb2: *stop=0xc90fcb2u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto Lc90fcb4; /* mov.l @r3,r1 */
Lc90fcb4: *stop=0xc90fcb4u;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto Lc90fcb6; /* mov.l @(0x4,r3),r0 */
Lc90fcb6: *stop=0xc90fcb6u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc90fcb8; /* mov.l r1,@r2 */
Lc90fcb8: *stop=0xc90fcb8u;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto Lc90fcba; /* mov.l r0,@(0x4,r2) */
Lc90fcba: *stop=0xc90fcbau;if(!valid_ram(r[3]+12u,4u))return -3;r[0]=rd(m,r[3]+0xcu,4);goto Lc90fcbc; /* mov.l @(0xc,r3),r0 */
Lc90fcbc: *stop=0xc90fcbcu;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto Lc90fcbe; /* mov.l @(0x8,r3),r1 */
Lc90fcbe: *stop=0xc90fcbeu;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto Lc90fcc0; /* mov.l r1,@(0x8,r2) */
Lc90fcc0: *stop=0xc90fcc0u;if(!valid_ram(r[2]+12u,4u))return -3;wr(m,r[2]+0xcu,r[0],4);goto Lc90fcc2; /* mov.l r0,@(0xc,r2) */
Lc90fcc2: *stop=0xc90fcc2u;if(!valid_ram(r[3]+20u,4u))return -3;r[0]=rd(m,r[3]+0x14u,4);goto Lc90fcc4; /* mov.l @(0x14,r3),r0 */
Lc90fcc4: *stop=0xc90fcc4u;if(!valid_ram(r[3]+16u,4u))return -3;r[1]=rd(m,r[3]+0x10u,4);goto Lc90fcc6; /* mov.l @(0x10,r3),r1 */
Lc90fcc6: *stop=0xc90fcc6u;if(!valid_ram(r[2]+16u,4u))return -3;wr(m,r[2]+0x10u,r[1],4);goto Lc90fcc8; /* mov.l r1,@(0x10,r2) */
Lc90fcc8: *stop=0xc90fcc8u;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[0],4);goto Lc90fcca; /* mov.l r0,@(0x14,r2) */
Lc90fcca: *stop=0xc90fccau;if(!valid_ram(r[3]+28u,4u))return -3;r[0]=rd(m,r[3]+0x1cu,4);goto Lc90fccc; /* mov.l @(0x1c,r3),r0 */
Lc90fccc: *stop=0xc90fcccu;if(!valid_ram(r[3]+24u,4u))return -3;r[1]=rd(m,r[3]+0x18u,4);goto Lc90fcce; /* mov.l @(0x18,r3),r1 */
Lc90fcce: *stop=0xc90fcceu;if(!valid_ram(r[2]+24u,4u))return -3;wr(m,r[2]+0x18u,r[1],4);goto Lc90fcd0; /* mov.l r1,@(0x18,r2) */
Lc90fcd0: *stop=0xc90fcd0u;if(!valid_ram(r[2]+28u,4u))return -3;wr(m,r[2]+0x1cu,r[0],4);goto Lc90fcd2; /* mov.l r0,@(0x1c,r2) */
Lc90fcd2: *stop=0xc90fcd2u;if(!valid_ram(r[3]+36u,4u))return -3;r[0]=rd(m,r[3]+0x24u,4);goto Lc90fcd4; /* mov.l @(0x24,r3),r0 */
Lc90fcd4: *stop=0xc90fcd4u;if(!valid_ram(r[3]+32u,4u))return -3;r[1]=rd(m,r[3]+0x20u,4);goto Lc90fcd6; /* mov.l @(0x20,r3),r1 */
Lc90fcd6: *stop=0xc90fcd6u;if(!valid_ram(r[2]+32u,4u))return -3;wr(m,r[2]+0x20u,r[1],4);goto Lc90fcd8; /* mov.l r1,@(0x20,r2) */
Lc90fcd8: *stop=0xc90fcd8u;if(!valid_ram(r[2]+36u,4u))return -3;wr(m,r[2]+0x24u,r[0],4);goto Lc90fcda; /* mov.l r0,@(0x24,r2) */
Lc90fcda: *stop=0xc90fcdau;if(!valid_ram(r[3]+44u,4u))return -3;r[0]=rd(m,r[3]+0x2cu,4);goto Lc90fcdc; /* mov.l @(0x2c,r3),r0 */
Lc90fcdc: *stop=0xc90fcdcu;if(!valid_ram(r[3]+40u,4u))return -3;r[1]=rd(m,r[3]+0x28u,4);goto Lc90fcde; /* mov.l @(0x28,r3),r1 */
Lc90fcde: *stop=0xc90fcdeu;if(!valid_ram(r[2]+40u,4u))return -3;wr(m,r[2]+0x28u,r[1],4);goto Lc90fce0; /* mov.l r1,@(0x28,r2) */
Lc90fce0: *stop=0xc90fce0u;if(!valid_ram(r[2]+44u,4u))return -3;wr(m,r[2]+0x2cu,r[0],4);goto Lc90fce2; /* mov.l r0,@(0x2c,r2) */
Lc90fce2: *stop=0xc90fce2u;if(!valid_ram(r[3]+52u,4u))return -3;r[0]=rd(m,r[3]+0x34u,4);goto Lc90fce4; /* mov.l @(0x34,r3),r0 */
Lc90fce4: *stop=0xc90fce4u;if(!valid_ram(r[3]+48u,4u))return -3;r[1]=rd(m,r[3]+0x30u,4);goto Lc90fce6; /* mov.l @(0x30,r3),r1 */
Lc90fce6: *stop=0xc90fce6u;if(!valid_ram(r[2]+48u,4u))return -3;wr(m,r[2]+0x30u,r[1],4);goto Lc90fce8; /* mov.l r1,@(0x30,r2) */
Lc90fce8: *stop=0xc90fce8u;if(!valid_ram(r[2]+52u,4u))return -3;wr(m,r[2]+0x34u,r[0],4);goto Lc90fcea; /* mov.l r0,@(0x34,r2) */
Lc90fcea: *stop=0xc90fceau;r[10]<<=8;goto Lc90fcec; /* shll8 r10 */
Lc90fcec: *stop=0xc90fcecu;r[14]=rd(m,0x0c90fec4u,4);goto Lc90fcee; /* mov.l 0x0c90fec4,r14 */
Lc90fcee: *stop=0xc90fceeu;r[10]+=72u;goto Lc90fcf0; /* add #0x48,r10 */
Lc90fcf0: *stop=0xc90fcf0u;r[10]+=r[5];goto Lc90fcf2; /* add r5,r10 */
Lc90fcf2: *stop=0xc90fcf2u;r[5]=r[4];goto Lc90fcf4; /* mov r4,r5 */
Lc90fcf4: *stop=0xc90fcf4u;if(!valid_ram(r[5],4u))return -3;r[0]=rd(m,r[5],4);goto Lc90fcf6; /* mov.l @r5,r0 */
Lc90fcf6: *stop=0xc90fcf6u;r[13]=4294967198u;goto Lc90fcf8; /* mov #-0x62,r13 */
Lc90fcf8: *stop=0xc90fcf8u;r[13]=r[13]&255u;goto Lc90fcfa; /* extu.b r13,r13 */
Lc90fcfa: *stop=0xc90fcfau;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc90fcfc; /* cmp/eq #-0x1,r0 */
Lc90fcfc: take=(*sr&1u);*stop=0xc90fcfeu;r[11]=0u;if(take)goto Lc90fd36;goto Lc90fd00; /* bt/s 0x0c90fd36 */
Lc90fd00: *stop=0xc90fd00u;if(!valid_ram(r[5],4u))return -3;r[6]=rd(m,r[5],4);goto Lc90fd02; /* mov.l @r5,r6 */
Lc90fd02: *stop=0xc90fd02u;r[3]=r[10];goto Lc90fd04; /* mov r10,r3 */
Lc90fd04: *stop=0xc90fd04u;r[3]+=4u;goto Lc90fd06; /* add #0x4,r3 */
Lc90fd06: *stop=0xc90fd06u;r[6]<<=2;goto Lc90fd08; /* shll2 r6 */
Lc90fd08: *stop=0xc90fd08u;r[6]+=r[3];goto Lc90fd0a; /* add r3,r6 */
Lc90fd0a: *stop=0xc90fd0au;if(!valid_ram(r[6],4u))return -3;r[6]=rd(m,r[6],4);goto Lc90fd0c; /* mov.l @r6,r6 */
Lc90fd0c: *stop=0xc90fd0cu;if(!valid_ram(r[6]+4u,4u))return -3;r[4]=rd(m,r[6]+0x4u,4);goto Lc90fd0e; /* mov.l @(0x4,r6),r4 */
Lc90fd0e: *stop=0xc90fd0eu;r[6]=r[11];goto Lc90fd10; /* mov r11,r6 */
Lc90fd10: *stop=0xc90fd10u;if(!valid_ram(r[4]+22u,2u))return -3;r[0]=sx(m,r[4]+0x16u,2);goto Lc90fd12; /* mov.w @(0x16,r4),r0 */
Lc90fd12: *stop=0xc90fd12u;if(!valid_ram(r[4]+16u,4u))return -3;r[1]=rd(m,r[4]+0x10u,4);goto Lc90fd14; /* mov.l @(0x10,r4),r1 */
Lc90fd14: *stop=0xc90fd14u;r[4]=0u;goto Lc90fd16; /* mov #0x0,r4 */
Lc90fd16: *stop=0xc90fd16u;r[7]=r[0]&65535u;goto Lc90fd18; /* extu.w r0,r7 */
Lc90fd18: *stop=0xc90fd18u;*sr=(*sr&~1u)|((int32_t)r[7]>0);goto Lc90fd1a; /* cmp/pl r7 */
Lc90fd1a: take=!(*sr&1u);*stop=0xc90fd1cu;r[4]+=r[1];if(take)goto Lc90fd2e;goto Lc90fd1e; /* bf/s 0x0c90fd2e */
Lc90fd1e: *stop=0xc90fd1eu;r[6]+=1u;goto Lc90fd20; /* add #0x1,r6 */
Lc90fd20: *stop=0xc90fd20u;if(!valid_ram(r[4]+12u,4u))return -3;wr(m,r[4]+0xcu,r[13],4);goto Lc90fd22; /* mov.l r13,@(0xc,r4) */
Lc90fd22: *stop=0xc90fd22u;if(!valid_ram(r[4]+16u,4u))return -3;r[2]=rd(m,r[4]+0x10u,4);goto Lc90fd24; /* mov.l @(0x10,r4),r2 */
Lc90fd24: *stop=0xc90fd24u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto Lc90fd26; /* cmp/ge r7,r6 */
Lc90fd26: *stop=0xc90fd26u;r[2]|=r[14];goto Lc90fd28; /* or r14,r2 */
Lc90fd28: *stop=0xc90fd28u;if(!valid_ram(r[4]+16u,4u))return -3;wr(m,r[4]+0x10u,r[2],4);goto Lc90fd2a; /* mov.l r2,@(0x10,r4) */
Lc90fd2a: take=!(*sr&1u);*stop=0xc90fd2cu;r[4]+=20u;if(take)goto Lc90fd1e;goto Lc90fd2e; /* bf/s 0x0c90fd1e */
Lc90fd2e: *stop=0xc90fd2eu;r[5]+=4u;goto Lc90fd30; /* add #0x4,r5 */
Lc90fd30: *stop=0xc90fd30u;if(!valid_ram(r[5],4u))return -3;r[0]=rd(m,r[5],4);goto Lc90fd32; /* mov.l @r5,r0 */
Lc90fd32: *stop=0xc90fd32u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc90fd34; /* cmp/eq #-0x1,r0 */
Lc90fd34: if(!(*sr&1u))goto Lc90fd00;goto Lc90fd36; /* bf 0x0c90fd00 */
Lc90fd36: *stop=0xc90fd36u;r[15]+=56u;goto Lc90fd38; /* add #0x38,r15 */
Lc90fd38: *stop=0xc90fd38u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto Lc90fd3a; /* mov.l @r15+,r10 */
Lc90fd3a: *stop=0xc90fd3au;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto Lc90fd3c; /* mov.l @r15+,r11 */
Lc90fd3c: *stop=0xc90fd3cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto Lc90fd3e; /* mov.l @r15+,r13 */
Lc90fd3e: *stop=0xc90fd3eu;if(*pr!=entry_pr)return -3;*stop=0xc90fd40u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_tail_material_161_0c90fd42(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc90fd42;
Lc90fd42: *stop=0xc90fd42u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc90fd44; /* mov.l r14,@-r15 */
Lc90fd44: *stop=0xc90fd44u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto Lc90fd46; /* mov.l r13,@-r15 */
Lc90fd46: *stop=0xc90fd46u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto Lc90fd48; /* mov.l r11,@-r15 */
Lc90fd48: *stop=0xc90fd48u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto Lc90fd4a; /* mov.l r10,@-r15 */
Lc90fd4a: *stop=0xc90fd4au;r[15]+=4294967240u;goto Lc90fd4c; /* add #-0x38,r15 */
Lc90fd4c: *stop=0xc90fd4cu;r[3]=rd(m,0x0c90fec8u,4);goto Lc90fd4e; /* mov.l 0x0c90fec8,r3 */
Lc90fd4e: *stop=0xc90fd4eu;r[2]=r[15];goto Lc90fd50; /* mov r15,r2 */
Lc90fd50: *stop=0xc90fd50u;r[10]=2u;goto Lc90fd52; /* mov #0x2,r10 */
Lc90fd52: *stop=0xc90fd52u;r[4]=r[15];goto Lc90fd54; /* mov r15,r4 */
Lc90fd54: *stop=0xc90fd54u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto Lc90fd56; /* mov.l @r3,r1 */
Lc90fd56: *stop=0xc90fd56u;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto Lc90fd58; /* mov.l @(0x4,r3),r0 */
Lc90fd58: *stop=0xc90fd58u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc90fd5a; /* mov.l r1,@r2 */
Lc90fd5a: *stop=0xc90fd5au;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto Lc90fd5c; /* mov.l r0,@(0x4,r2) */
Lc90fd5c: *stop=0xc90fd5cu;if(!valid_ram(r[3]+12u,4u))return -3;r[0]=rd(m,r[3]+0xcu,4);goto Lc90fd5e; /* mov.l @(0xc,r3),r0 */
Lc90fd5e: *stop=0xc90fd5eu;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto Lc90fd60; /* mov.l @(0x8,r3),r1 */
Lc90fd60: *stop=0xc90fd60u;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto Lc90fd62; /* mov.l r1,@(0x8,r2) */
Lc90fd62: *stop=0xc90fd62u;if(!valid_ram(r[2]+12u,4u))return -3;wr(m,r[2]+0xcu,r[0],4);goto Lc90fd64; /* mov.l r0,@(0xc,r2) */
Lc90fd64: *stop=0xc90fd64u;if(!valid_ram(r[3]+20u,4u))return -3;r[0]=rd(m,r[3]+0x14u,4);goto Lc90fd66; /* mov.l @(0x14,r3),r0 */
Lc90fd66: *stop=0xc90fd66u;if(!valid_ram(r[3]+16u,4u))return -3;r[1]=rd(m,r[3]+0x10u,4);goto Lc90fd68; /* mov.l @(0x10,r3),r1 */
Lc90fd68: *stop=0xc90fd68u;if(!valid_ram(r[2]+16u,4u))return -3;wr(m,r[2]+0x10u,r[1],4);goto Lc90fd6a; /* mov.l r1,@(0x10,r2) */
Lc90fd6a: *stop=0xc90fd6au;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[0],4);goto Lc90fd6c; /* mov.l r0,@(0x14,r2) */
Lc90fd6c: *stop=0xc90fd6cu;if(!valid_ram(r[3]+28u,4u))return -3;r[0]=rd(m,r[3]+0x1cu,4);goto Lc90fd6e; /* mov.l @(0x1c,r3),r0 */
Lc90fd6e: *stop=0xc90fd6eu;if(!valid_ram(r[3]+24u,4u))return -3;r[1]=rd(m,r[3]+0x18u,4);goto Lc90fd70; /* mov.l @(0x18,r3),r1 */
Lc90fd70: *stop=0xc90fd70u;if(!valid_ram(r[2]+24u,4u))return -3;wr(m,r[2]+0x18u,r[1],4);goto Lc90fd72; /* mov.l r1,@(0x18,r2) */
Lc90fd72: *stop=0xc90fd72u;if(!valid_ram(r[2]+28u,4u))return -3;wr(m,r[2]+0x1cu,r[0],4);goto Lc90fd74; /* mov.l r0,@(0x1c,r2) */
Lc90fd74: *stop=0xc90fd74u;if(!valid_ram(r[3]+36u,4u))return -3;r[0]=rd(m,r[3]+0x24u,4);goto Lc90fd76; /* mov.l @(0x24,r3),r0 */
Lc90fd76: *stop=0xc90fd76u;if(!valid_ram(r[3]+32u,4u))return -3;r[1]=rd(m,r[3]+0x20u,4);goto Lc90fd78; /* mov.l @(0x20,r3),r1 */
Lc90fd78: *stop=0xc90fd78u;if(!valid_ram(r[2]+32u,4u))return -3;wr(m,r[2]+0x20u,r[1],4);goto Lc90fd7a; /* mov.l r1,@(0x20,r2) */
Lc90fd7a: *stop=0xc90fd7au;if(!valid_ram(r[2]+36u,4u))return -3;wr(m,r[2]+0x24u,r[0],4);goto Lc90fd7c; /* mov.l r0,@(0x24,r2) */
Lc90fd7c: *stop=0xc90fd7cu;if(!valid_ram(r[3]+44u,4u))return -3;r[0]=rd(m,r[3]+0x2cu,4);goto Lc90fd7e; /* mov.l @(0x2c,r3),r0 */
Lc90fd7e: *stop=0xc90fd7eu;if(!valid_ram(r[3]+40u,4u))return -3;r[1]=rd(m,r[3]+0x28u,4);goto Lc90fd80; /* mov.l @(0x28,r3),r1 */
Lc90fd80: *stop=0xc90fd80u;if(!valid_ram(r[2]+40u,4u))return -3;wr(m,r[2]+0x28u,r[1],4);goto Lc90fd82; /* mov.l r1,@(0x28,r2) */
Lc90fd82: *stop=0xc90fd82u;if(!valid_ram(r[2]+44u,4u))return -3;wr(m,r[2]+0x2cu,r[0],4);goto Lc90fd84; /* mov.l r0,@(0x2c,r2) */
Lc90fd84: *stop=0xc90fd84u;if(!valid_ram(r[3]+52u,4u))return -3;r[0]=rd(m,r[3]+0x34u,4);goto Lc90fd86; /* mov.l @(0x34,r3),r0 */
Lc90fd86: *stop=0xc90fd86u;if(!valid_ram(r[3]+48u,4u))return -3;r[1]=rd(m,r[3]+0x30u,4);goto Lc90fd88; /* mov.l @(0x30,r3),r1 */
Lc90fd88: *stop=0xc90fd88u;if(!valid_ram(r[2]+48u,4u))return -3;wr(m,r[2]+0x30u,r[1],4);goto Lc90fd8a; /* mov.l r1,@(0x30,r2) */
Lc90fd8a: *stop=0xc90fd8au;if(!valid_ram(r[2]+52u,4u))return -3;wr(m,r[2]+0x34u,r[0],4);goto Lc90fd8c; /* mov.l r0,@(0x34,r2) */
Lc90fd8c: *stop=0xc90fd8cu;r[10]<<=8;goto Lc90fd8e; /* shll8 r10 */
Lc90fd8e: *stop=0xc90fd8eu;r[14]=rd(m,0x0c90fec4u,4);goto Lc90fd90; /* mov.l 0x0c90fec4,r14 */
Lc90fd90: *stop=0xc90fd90u;r[10]+=72u;goto Lc90fd92; /* add #0x48,r10 */
Lc90fd92: *stop=0xc90fd92u;r[10]+=r[5];goto Lc90fd94; /* add r5,r10 */
Lc90fd94: *stop=0xc90fd94u;r[5]=r[4];goto Lc90fd96; /* mov r4,r5 */
Lc90fd96: *stop=0xc90fd96u;if(!valid_ram(r[5],4u))return -3;r[0]=rd(m,r[5],4);goto Lc90fd98; /* mov.l @r5,r0 */
Lc90fd98: *stop=0xc90fd98u;r[13]=4294967201u;goto Lc90fd9a; /* mov #-0x5f,r13 */
Lc90fd9a: *stop=0xc90fd9au;r[13]=r[13]&255u;goto Lc90fd9c; /* extu.b r13,r13 */
Lc90fd9c: *stop=0xc90fd9cu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc90fd9e; /* cmp/eq #-0x1,r0 */
Lc90fd9e: take=(*sr&1u);*stop=0xc90fda0u;r[11]=0u;if(take)goto Lc90fdd8;goto Lc90fda2; /* bt/s 0x0c90fdd8 */
Lc90fda2: *stop=0xc90fda2u;if(!valid_ram(r[5],4u))return -3;r[6]=rd(m,r[5],4);goto Lc90fda4; /* mov.l @r5,r6 */
Lc90fda4: *stop=0xc90fda4u;r[3]=r[10];goto Lc90fda6; /* mov r10,r3 */
Lc90fda6: *stop=0xc90fda6u;r[3]+=4u;goto Lc90fda8; /* add #0x4,r3 */
Lc90fda8: *stop=0xc90fda8u;r[6]<<=2;goto Lc90fdaa; /* shll2 r6 */
Lc90fdaa: *stop=0xc90fdaau;r[6]+=r[3];goto Lc90fdac; /* add r3,r6 */
Lc90fdac: *stop=0xc90fdacu;if(!valid_ram(r[6],4u))return -3;r[6]=rd(m,r[6],4);goto Lc90fdae; /* mov.l @r6,r6 */
Lc90fdae: *stop=0xc90fdaeu;if(!valid_ram(r[6]+4u,4u))return -3;r[4]=rd(m,r[6]+0x4u,4);goto Lc90fdb0; /* mov.l @(0x4,r6),r4 */
Lc90fdb0: *stop=0xc90fdb0u;r[6]=r[11];goto Lc90fdb2; /* mov r11,r6 */
Lc90fdb2: *stop=0xc90fdb2u;if(!valid_ram(r[4]+22u,2u))return -3;r[0]=sx(m,r[4]+0x16u,2);goto Lc90fdb4; /* mov.w @(0x16,r4),r0 */
Lc90fdb4: *stop=0xc90fdb4u;if(!valid_ram(r[4]+16u,4u))return -3;r[1]=rd(m,r[4]+0x10u,4);goto Lc90fdb6; /* mov.l @(0x10,r4),r1 */
Lc90fdb6: *stop=0xc90fdb6u;r[4]=0u;goto Lc90fdb8; /* mov #0x0,r4 */
Lc90fdb8: *stop=0xc90fdb8u;r[7]=r[0]&65535u;goto Lc90fdba; /* extu.w r0,r7 */
Lc90fdba: *stop=0xc90fdbau;*sr=(*sr&~1u)|((int32_t)r[7]>0);goto Lc90fdbc; /* cmp/pl r7 */
Lc90fdbc: take=!(*sr&1u);*stop=0xc90fdbeu;r[4]+=r[1];if(take)goto Lc90fdd0;goto Lc90fdc0; /* bf/s 0x0c90fdd0 */
Lc90fdc0: *stop=0xc90fdc0u;r[6]+=1u;goto Lc90fdc2; /* add #0x1,r6 */
Lc90fdc2: *stop=0xc90fdc2u;if(!valid_ram(r[4]+12u,4u))return -3;wr(m,r[4]+0xcu,r[13],4);goto Lc90fdc4; /* mov.l r13,@(0xc,r4) */
Lc90fdc4: *stop=0xc90fdc4u;if(!valid_ram(r[4]+16u,4u))return -3;r[2]=rd(m,r[4]+0x10u,4);goto Lc90fdc6; /* mov.l @(0x10,r4),r2 */
Lc90fdc6: *stop=0xc90fdc6u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto Lc90fdc8; /* cmp/ge r7,r6 */
Lc90fdc8: *stop=0xc90fdc8u;r[2]|=r[14];goto Lc90fdca; /* or r14,r2 */
Lc90fdca: *stop=0xc90fdcau;if(!valid_ram(r[4]+16u,4u))return -3;wr(m,r[4]+0x10u,r[2],4);goto Lc90fdcc; /* mov.l r2,@(0x10,r4) */
Lc90fdcc: take=!(*sr&1u);*stop=0xc90fdceu;r[4]+=20u;if(take)goto Lc90fdc0;goto Lc90fdd0; /* bf/s 0x0c90fdc0 */
Lc90fdd0: *stop=0xc90fdd0u;r[5]+=4u;goto Lc90fdd2; /* add #0x4,r5 */
Lc90fdd2: *stop=0xc90fdd2u;if(!valid_ram(r[5],4u))return -3;r[0]=rd(m,r[5],4);goto Lc90fdd4; /* mov.l @r5,r0 */
Lc90fdd4: *stop=0xc90fdd4u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc90fdd6; /* cmp/eq #-0x1,r0 */
Lc90fdd6: if(!(*sr&1u))goto Lc90fda2;goto Lc90fdd8; /* bf 0x0c90fda2 */
Lc90fdd8: *stop=0xc90fdd8u;r[15]+=56u;goto Lc90fdda; /* add #0x38,r15 */
Lc90fdda: *stop=0xc90fddau;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto Lc90fddc; /* mov.l @r15+,r10 */
Lc90fddc: *stop=0xc90fddcu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto Lc90fdde; /* mov.l @r15+,r11 */
Lc90fdde: *stop=0xc90fddeu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto Lc90fde0; /* mov.l @r15+,r13 */
Lc90fde0: *stop=0xc90fde0u;if(*pr!=entry_pr)return -3;*stop=0xc90fde2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_appearance_mode_0c9084e6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc9084e6;
Lc9084e6: *stop=0xc9084e6u;r[0]=rd(m,0x0c9085a0u,4);goto Lc9084e8; /* mov.l 0x0c9085a0,r0 */
Lc9084e8: *stop=0xc9084e8u;r[3]=8u;goto Lc9084ea; /* mov #0x8,r3 */
Lc9084ea: *stop=0xc9084eau;r[3]<<=8;goto Lc9084ec; /* shll8 r3 */
Lc9084ec: *stop=0xc9084ecu;if(!valid_ram(r[0]+r[5],1u))return -3;r[2]=sx(m,r[5]+r[0],1);goto Lc9084ee; /* mov.b @(r0,r5),r2 */
Lc9084ee: *stop=0xc9084eeu;r[3]+=4294967240u;goto Lc9084f0; /* add #-0x38,r3 */
Lc9084f0: *stop=0xc9084f0u;r[1]=60u;goto Lc9084f2; /* mov #0x3c,r1 */
Lc9084f2: *stop=0xc9084f2u;r[4]+=r[3];goto Lc9084f4; /* add r3,r4 */
Lc9084f4: *stop=0xc9084f4u;r[1]+=r[4];goto Lc9084f6; /* add r4,r1 */
Lc9084f6: *stop=0xc9084f6u;r[2]=r[2]&255u;goto Lc9084f8; /* extu.b r2,r2 */
Lc9084f8: *stop=0xc9084f8u;if(*pr!=entry_pr)return -3;*stop=0xc9084fau;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[2],2);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_appearance_pair_0c9084fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc9084fc;
Lc9084fc: *stop=0xc9084fcu;r[6]=r[5];goto Lc9084fe; /* mov r5,r6 */
Lc9084fe: *stop=0xc9084feu;r[2]=rd(m,0x0c9085a4u,4);goto Lc908500; /* mov.l 0x0c9085a4,r2 */
Lc908500: *stop=0xc908500u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto Lc908502; /* shll r6 */
Lc908502: *stop=0xc908502u;r[6]+=r[2];goto Lc908504; /* add r2,r6 */
Lc908504: *stop=0xc908504u;r[3]=8u;goto Lc908506; /* mov #0x8,r3 */
Lc908506: *stop=0xc908506u;if(!valid_ram(r[6],1u))return -3;r[1]=sx(m,r[6],1);goto Lc908508; /* mov.b @r6,r1 */
Lc908508: *stop=0xc908508u;r[3]<<=8;goto Lc90850a; /* shll8 r3 */
Lc90850a: *stop=0xc90850au;r[3]+=4294967240u;goto Lc90850c; /* add #-0x38,r3 */
Lc90850c: *stop=0xc90850cu;r[0]=64u;goto Lc90850e; /* mov #0x40,r0 */
Lc90850e: *stop=0xc90850eu;r[1]=r[1]&255u;goto Lc908510; /* extu.b r1,r1 */
Lc908510: *stop=0xc908510u;r[4]+=r[3];goto Lc908512; /* add r3,r4 */
Lc908512: *stop=0xc908512u;if(!valid_ram(r[0]+r[4],2u))return -3;wr(m,r[0]+r[4],r[1],2);goto Lc908514; /* mov.w r1,@(r0,r4) */
Lc908514: *stop=0xc908514u;r[1]=66u;goto Lc908516; /* mov #0x42,r1 */
Lc908516: *stop=0xc908516u;if(!valid_ram(r[6]+1u,1u))return -3;r[0]=sx(m,r[6]+0x1u,1);goto Lc908518; /* mov.b @(0x1,r6),r0 */
Lc908518: *stop=0xc908518u;r[1]+=r[4];goto Lc90851a; /* add r4,r1 */
Lc90851a: *stop=0xc90851au;r[0]=r[0]&255u;goto Lc90851c; /* extu.b r0,r0 */
Lc90851c: *stop=0xc90851cu;if(*pr!=entry_pr)return -3;*stop=0xc90851eu;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[0],2);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_appearance_links_0c908530(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc908530;
Lc908530: *stop=0xc908530u;r[3]=8u;goto Lc908532; /* mov #0x8,r3 */
Lc908532: *stop=0xc908532u;r[3]<<=8;goto Lc908534; /* shll8 r3 */
Lc908534: *stop=0xc908534u;r[3]+=4294967240u;goto Lc908536; /* add #-0x38,r3 */
Lc908536: *stop=0xc908536u;r[4]+=r[3];goto Lc908538; /* add r3,r4 */
Lc908538: *stop=0xc908538u;if(!valid_ram(r[4]+40u,4u))return -3;wr(m,r[4]+0x28u,r[5],4);goto Lc90853a; /* mov.l r5,@(0x28,r4) */
Lc90853a: *stop=0xc90853au;if(*pr!=entry_pr)return -3;*stop=0xc90853cu;if(!valid_ram(r[4]+56u,4u))return -3;wr(m,r[4]+0x38u,r[6],4);return 0; /* rts */
}
