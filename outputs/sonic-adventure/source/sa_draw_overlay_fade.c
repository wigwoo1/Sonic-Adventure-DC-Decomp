/* Overlay fading labels, text layout and quads; names inferred.
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
SA_STAGE_EXPORT int sa_overlay_fade_label_0c90c384(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc90c384;
Lc90c384: *stop=0xc90c384u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc90c386; /* mov.l r14,@-r15 */
Lc90c386: *stop=0xc90c386u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto Lc90c388; /* fmov.s fr15,@-r15 */
Lc90c388: *stop=0xc90c388u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc90c38a; /* sts.l PR,@-r15 */
Lc90c38a: *stop=0xc90c38au;r[15]+=4294967280u;goto Lc90c38c; /* add #-0x10,r15 */
Lc90c38c: *stop=0xc90c38cu;r[0]=116u;goto Lc90c38e; /* mov #0x74,r0 */
Lc90c38e: *stop=0xc90c38eu;r[14]=r[5];goto Lc90c390; /* mov r5,r14 */
Lc90c390: *stop=0xc90c390u;if(!valid_ram(r[0]+r[14],4u))return -3;r[3]=rd(m,r[14]+r[0],4);goto Lc90c392; /* mov.l @(r0,r14),r3 */
Lc90c392: *stop=0xc90c392u;r[3]+=4294967266u;goto Lc90c394; /* add #-0x1e,r3 */
Lc90c394: *stop=0xc90c394u;if(!valid_ram(r[0]+r[14],4u))return -3;wr(m,r[14]+r[0],r[3],4);goto Lc90c396; /* mov.l r3,@(r0,r14) */
Lc90c396: *stop=0xc90c396u;r[0]=96u;goto Lc90c398; /* mov #0x60,r0 */
Lc90c398: *stop=0xc90c398u;if(!valid_ram(r[0]+r[14],4u))return -3;r[0]=rd(m,r[14]+r[0],4);goto Lc90c39a; /* mov.l @(r0,r14),r0 */
Lc90c39a: *stop=0xc90c39au;*sr=(*sr&~1u)|!(r[0]&0x2u);goto Lc90c39c; /* tst #0x2,r0 */
Lc90c39c: take=(*sr&1u);*stop=0xc90c39eu;r[6]=0u;if(take)goto Lc90c400;goto Lc90c3a0; /* bt/s 0x0c90c400 */
Lc90c3a0: *stop=0xc90c3a0u;r[0]=120u;goto Lc90c3a2; /* mov #0x78,r0 */
Lc90c3a2: *stop=0xc90c3a2u;if(!valid_ram(r[0]+r[14],2u))return -3;r[3]=sx(m,r[0]+r[14],2);goto Lc90c3a4; /* mov.w @(r0,r14),r3 */
Lc90c3a4: *stop=0xc90c3a4u;r[2]=4294967190u;goto Lc90c3a6; /* mov #-0x6a,r2 */
Lc90c3a6: *stop=0xc90c3a6u;r[2]=r[2]&255u;goto Lc90c3a8; /* extu.b r2,r2 */
Lc90c3a8: *stop=0xc90c3a8u;r[3]=r[3]&65535u;goto Lc90c3aa; /* extu.w r3,r3 */
Lc90c3aa: *stop=0xc90c3aau;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto Lc90c3ac; /* cmp/ge r2,r3 */
Lc90c3ac: if((*sr&1u))goto Lc90c404;goto Lc90c3ae; /* bt 0x0c90c404 */
Lc90c3ae: *stop=0xc90c3aeu;r[0]=116u;goto Lc90c3b0; /* mov #0x74,r0 */
Lc90c3b0: *stop=0xc90c3b0u;if(!valid_ram(r[0]+r[14],4u))return -3;r[1]=rd(m,r[14]+r[0],4);goto Lc90c3b2; /* mov.l @(r0,r14),r1 */
Lc90c3b2: *stop=0xc90c3b2u;r[1]+=70u;goto Lc90c3b4; /* add #0x46,r1 */
Lc90c3b4: *stop=0xc90c3b4u;if(!valid_ram(r[0]+r[14],4u))return -3;wr(m,r[14]+r[0],r[1],4);goto Lc90c3b6; /* mov.l r1,@(r0,r14) */
Lc90c3b6: *stop=0xc90c3b6u;r[0]=120u;goto Lc90c3b8; /* mov #0x78,r0 */
Lc90c3b8: *stop=0xc90c3b8u;if(!valid_ram(r[0]+r[14],2u))return -3;r[3]=sx(m,r[0]+r[14],2);goto Lc90c3ba; /* mov.w @(r0,r14),r3 */
Lc90c3ba: *stop=0xc90c3bau;r[3]+=1u;goto Lc90c3bc; /* add #0x1,r3 */
Lc90c3bc: *stop=0xc90c3beu;if(!valid_ram(r[0]+r[14],2u))return -3;wr(m,r[0]+r[14],r[3],2);goto Lc90c404; /* bra 0x0c90c404 */
Lc90c400: *stop=0xc90c400u;r[0]=120u;goto Lc90c402; /* mov #0x78,r0 */
Lc90c402: *stop=0xc90c402u;if(!valid_ram(r[0]+r[14],2u))return -3;wr(m,r[0]+r[14],r[6],2);goto Lc90c404; /* mov.w r6,@(r0,r14) */
Lc90c404: *stop=0xc90c404u;r[0]=116u;goto Lc90c406; /* mov #0x74,r0 */
Lc90c406: *stop=0xc90c406u;if(!valid_ram(r[0]+r[14],4u))return -3;r[3]=rd(m,r[14]+r[0],4);goto Lc90c408; /* mov.l @(r0,r14),r3 */
Lc90c408: *stop=0xc90c408u;r[5]=4294967295u;goto Lc90c40a; /* mov #-0x1,r5 */
Lc90c40a: *stop=0xc90c40au;r[5]=r[5]&255u;goto Lc90c40c; /* extu.b r5,r5 */
Lc90c40c: *stop=0xc90c40cu;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[5]);goto Lc90c40e; /* cmp/gt r5,r3 */
Lc90c40e: if(!(*sr&1u))goto Lc90c414;goto Lc90c410; /* bf 0x0c90c414 */
Lc90c410: *stop=0xc90c412u;if(!valid_ram(r[0]+r[14],4u))return -3;wr(m,r[14]+r[0],r[5],4);goto Lc90c41c; /* bra 0x0c90c41c */
Lc90c414: *stop=0xc90c414u;if(!valid_ram(r[0]+r[14],4u))return -3;r[2]=rd(m,r[14]+r[0],4);goto Lc90c416; /* mov.l @(r0,r14),r2 */
Lc90c416: *stop=0xc90c416u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto Lc90c418; /* cmp/pz r2 */
Lc90c418: if((*sr&1u))goto Lc90c41c;goto Lc90c41a; /* bt 0x0c90c41c */
Lc90c41a: *stop=0xc90c41au;if(!valid_ram(r[0]+r[14],4u))return -3;wr(m,r[14]+r[0],r[6],4);goto Lc90c41c; /* mov.l r6,@(r0,r14) */
Lc90c41c: *stop=0xc90c41cu;if(!valid_ram(r[0]+r[14],4u))return -3;r[2]=rd(m,r[14]+r[0],4);goto Lc90c41e; /* mov.l @(r0,r14),r2 */
Lc90c41e: *stop=0xc90c41eu;r[3]=10u;goto Lc90c420; /* mov #0xa,r3 */
Lc90c420: *stop=0xc90c420u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]);goto Lc90c422; /* cmp/gt r3,r2 */
Lc90c422: if(!(*sr&1u))goto Lc90c492;goto Lc90c424; /* bf 0x0c90c492 */
Lc90c424: *stop=0xc90c424u;r[0]=9u;goto Lc90c426; /* mov #0x9,r0 */
Lc90c426: *stop=0xc90c426u;r[2]=r[15];goto Lc90c428; /* mov r15,r2 */
Lc90c428: *stop=0xc90c428u;r[0]<<=8;goto Lc90c42a; /* shll8 r0 */
Lc90c42a: *stop=0xc90c42au;r[0]+=4294967238u;goto Lc90c42c; /* add #-0x3a,r0 */
Lc90c42c: *stop=0xc90c42cu;if(!valid_ram(r[0]+r[14],1u))return -3;r[0]=sx(m,r[14]+r[0],1);goto Lc90c42e; /* mov.b @(r0,r14),r0 */
Lc90c42e: *stop=0xc90c42eu;r[2]+=4u;goto Lc90c430; /* add #0x4,r2 */
Lc90c430: *stop=0xc90c430u;if(!valid_ram(r[15]+2u,1u))return -3;wr(m,r[15]+0x2u,r[0],1);goto Lc90c432; /* mov.b r0,@(0x2,r15) */
Lc90c432: *stop=0xc90c432u;r[0]=9u;goto Lc90c434; /* mov #0x9,r0 */
Lc90c434: *stop=0xc90c434u;r[0]<<=8;goto Lc90c436; /* shll8 r0 */
Lc90c436: *stop=0xc90c436u;r[0]+=4294967237u;goto Lc90c438; /* add #-0x3b,r0 */
Lc90c438: *stop=0xc90c438u;if(!valid_ram(r[0]+r[14],1u))return -3;r[0]=sx(m,r[14]+r[0],1);goto Lc90c43a; /* mov.b @(r0,r14),r0 */
Lc90c43a: *stop=0xc90c43au;if(!valid_ram(r[15]+1u,1u))return -3;wr(m,r[15]+0x1u,r[0],1);goto Lc90c43c; /* mov.b r0,@(0x1,r15) */
Lc90c43c: *stop=0xc90c43cu;r[0]=9u;goto Lc90c43e; /* mov #0x9,r0 */
Lc90c43e: *stop=0xc90c43eu;r[0]<<=8;goto Lc90c440; /* shll8 r0 */
Lc90c440: *stop=0xc90c440u;r[0]+=4294967236u;goto Lc90c442; /* add #-0x3c,r0 */
Lc90c442: *stop=0xc90c442u;if(!valid_ram(r[0]+r[14],1u))return -3;r[1]=sx(m,r[14]+r[0],1);goto Lc90c444; /* mov.b @(r0,r14),r1 */
Lc90c444: *stop=0xc90c444u;r[0]=116u;goto Lc90c446; /* mov #0x74,r0 */
Lc90c446: *stop=0xc90c446u;if(!valid_ram(r[15],1u))return -3;wr(m,r[15],r[1],1);goto Lc90c448; /* mov.b r1,@r15 */
Lc90c448: *stop=0xc90c448u;if(!valid_ram(r[0]+r[14],1u))return -3;r[0]=sx(m,r[14]+r[0],1);goto Lc90c44a; /* mov.b @(r0,r14),r0 */
Lc90c44a: *stop=0xc90c44au;if(!valid_ram(r[15]+3u,1u))return -3;wr(m,r[15]+0x3u,r[0],1);goto Lc90c44c; /* mov.b r0,@(0x3,r15) */
Lc90c44c: *stop=0xc90c44cu;r[0]=116u;goto Lc90c44e; /* mov #0x74,r0 */
Lc90c44e: *stop=0xc90c44eu;if(!valid_ram(r[0]+r[14],4u))return -3;r[1]=rd(m,r[14]+r[0],4);goto Lc90c450; /* mov.l @(r0,r14),r1 */
Lc90c450: *stop=0xc90c450u;r[0]+=100u;goto Lc90c452; /* add #0x64,r0 */
Lc90c452: *stop=0xc90c452u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto Lc90c454; /* fmov.s @(r0,r14),fr2 */
Lc90c454: *stop=0xc90c454u;r[0]=0x0c90c540u;goto Lc90c456; /* mova 0x0c90c540,r0 */
Lc90c456: *stop=0xc90c456u;*fpul=r[1];goto Lc90c458; /* lds r1,FPUL */
Lc90c458: *stop=0xc90c458u;if(!valid_ram(r[4]+32u,4u))return -3;r[1]=rd(m,r[4]+0x20u,4);goto Lc90c45a; /* mov.l @(0x20,r4),r1 */
Lc90c45a: *stop=0xc90c45au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc90c45c; /* float FPUL,fr3 */
Lc90c45c: *stop=0xc90c45cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90c45e; /* fmul fr2,fr3 */
Lc90c45e: *stop=0xc90c45eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto Lc90c460; /* fmov.s @r0,fr2 */
Lc90c460: *stop=0xc90c460u;fcopy(f,15u,3u);goto Lc90c462; /* fmov fr3,fr15 */
Lc90c462: *stop=0xc90c462u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[2],&f->fpscr,&fr[15]);if(status)return status;}goto Lc90c464; /* fmul fr2,fr15 */
Lc90c464: *stop=0xc90c464u;if(!valid_ram(r[4]+36u,4u))return -3;r[0]=rd(m,r[4]+0x24u,4);goto Lc90c466; /* mov.l @(0x24,r4),r0 */
Lc90c466: *stop=0xc90c466u;r[5]=r[15];goto Lc90c468; /* mov r15,r5 */
Lc90c468: *stop=0xc90c468u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc90c46a; /* mov.l r1,@r2 */
Lc90c46a: *stop=0xc90c46au;r[5]+=4u;goto Lc90c46c; /* add #0x4,r5 */
Lc90c46c: *stop=0xc90c46cu;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto Lc90c46e; /* mov.l r0,@(0x4,r2) */
Lc90c46e: *stop=0xc90c46eu;r[0]=0x0c90c544u;goto Lc90c470; /* mova 0x0c90c544,r0 */
Lc90c470: *stop=0xc90c470u;if(!valid_ram(r[4]+40u,4u))return -3;r[1]=rd(m,r[4]+0x28u,4);goto Lc90c472; /* mov.l @(0x28,r4),r1 */
Lc90c472: *stop=0xc90c472u;r[6]=r[15];goto Lc90c474; /* mov r15,r6 */
Lc90c474: *stop=0xc90c474u;fcopy(f,4u,15u);goto Lc90c476; /* fmov fr15,fr4 */
Lc90c476: *stop=0xc90c476u;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto Lc90c478; /* mov.l r1,@(0x8,r2) */
Lc90c478: *stop=0xc90c478u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc90c47a; /* fmov.s @r0,fr3 */
Lc90c47a: *stop=0xc90c47au;r[0]=0x0c90c548u;goto Lc90c47c; /* mova 0x0c90c548,r0 */
Lc90c47c: *stop=0xc90c47cu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]);goto Lc90c47e; /* fmov.s @r0,fr0 */
Lc90c47e: *stop=0xc90c47eu;r[0]=8u;goto Lc90c480; /* mov #0x8,r0 */
Lc90c480: *stop=0xc90c480u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto Lc90c482; /* fmov.s @(r0,r15),fr2 */
Lc90c482: *stop=0xc90c482u;r[0]=8u;goto Lc90c484; /* mov #0x8,r0 */
Lc90c484: *stop=0xc90c484u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[15],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90c486; /* fmac fr0,fr15,fr3 */
Lc90c486: *stop=0xc90c486u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc90c488; /* fadd fr3,fr2 */
Lc90c488: *stop=0xc90c488u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto Lc90c48a; /* fmov fr2,@(r0,r15) */
Lc90c48a: *stop=0xc90c48au;r[0]=4294967276u;goto Lc90c48c; /* mov #-0x14,r0 */
Lc90c48c: *stop=0xc90c48cu;r[0]=r[0]&255u;goto Lc90c48e; /* extu.b r0,r0 */
Lc90c48e: target=0x0c90c274u;*pr=0x0c90c492u;*stop=0xc90c490u;if(!valid_ram(r[0]+r[14],4u))return -3;r[4]=rd(m,r[14]+r[0],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c492; /* bsr 0x0c90c274 */
Lc90c492: *stop=0xc90c492u;r[15]+=16u;goto Lc90c494; /* add #0x10,r15 */
Lc90c494: *stop=0xc90c494u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc90c496; /* lds.l @r15+,PR */
Lc90c496: *stop=0xc90c496u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc90c498; /* fmov.s @r15+,fr15 */
Lc90c498: *stop=0xc90c498u;if(*pr!=entry_pr)return -3;*stop=0xc90c49au;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_fade_text_0c90c274(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc90c274;
Lc90c274: *stop=0xc90c274u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc90c276; /* mov.l r14,@-r15 */
Lc90c276: *stop=0xc90c276u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto Lc90c278; /* mov.l r13,@-r15 */
Lc90c278: *stop=0xc90c278u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto Lc90c27a; /* mov.l r12,@-r15 */
Lc90c27a: *stop=0xc90c27au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto Lc90c27c; /* mov.l r11,@-r15 */
Lc90c27c: *stop=0xc90c27cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto Lc90c27e; /* mov.l r10,@-r15 */
Lc90c27e: *stop=0xc90c27eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto Lc90c280; /* mov.l r9,@-r15 */
Lc90c280: *stop=0xc90c280u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto Lc90c282; /* mov.l r8,@-r15 */
Lc90c282: *stop=0xc90c282u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto Lc90c284; /* fmov.s fr15,@-r15 */
Lc90c284: *stop=0xc90c284u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto Lc90c286; /* fmov.s fr14,@-r15 */
Lc90c286: *stop=0xc90c286u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[15]);goto Lc90c288; /* fmov.s fr13,@-r15 */
Lc90c288: *stop=0xc90c288u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[15]);goto Lc90c28a; /* fmov.s fr12,@-r15 */
Lc90c28a: *stop=0xc90c28au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc90c28c; /* sts.l PR,@-r15 */
Lc90c28c: *stop=0xc90c28cu;r[15]+=4294967268u;goto Lc90c28e; /* add #-0x1c,r15 */
Lc90c28e: *stop=0xc90c28eu;r[3]=rd(m,0x0c90c3dcu,4);goto Lc90c290; /* mov.l 0x0c90c3dc,r3 */
Lc90c290: *stop=0xc90c290u;r[2]=r[15];goto Lc90c292; /* mov r15,r2 */
Lc90c292: *stop=0xc90c292u;r[2]+=16u;goto Lc90c294; /* add #0x10,r2 */
Lc90c294: *stop=0xc90c294u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto Lc90c296; /* mov.l r5,@r15 */
Lc90c296: *stop=0xc90c296u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto Lc90c298; /* mov.l @r3,r1 */
Lc90c298: *stop=0xc90c298u;r[5]=r[4];goto Lc90c29a; /* mov r4,r5 */
Lc90c29a: *stop=0xc90c29au;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto Lc90c29c; /* mov.l @(0x4,r3),r0 */
Lc90c29c: *stop=0xc90c29cu;r[5]+=4u;goto Lc90c29e; /* add #0x4,r5 */
Lc90c29e: *stop=0xc90c29eu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc90c2a0; /* mov.l r1,@r2 */
Lc90c2a0: *stop=0xc90c2a0u;r[8]=r[6];goto Lc90c2a2; /* mov r6,r8 */
Lc90c2a2: *stop=0xc90c2a2u;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto Lc90c2a4; /* mov.l r0,@(0x4,r2) */
Lc90c2a4: *stop=0xc90c2a4u;r[6]=0u;goto Lc90c2a6; /* mov #0x0,r6 */
Lc90c2a6: *stop=0xc90c2a6u;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto Lc90c2a8; /* mov.l @(0x8,r3),r1 */
Lc90c2a8: *stop=0xc90c2a8u;r[12]=r[6];goto Lc90c2aa; /* mov r6,r12 */
Lc90c2aa: *stop=0xc90c2aau;fcopy(f,15u,4u);goto Lc90c2ac; /* fmov fr4,fr15 */
Lc90c2ac: *stop=0xc90c2acu;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto Lc90c2ae; /* mov.l r1,@(0x8,r2) */
Lc90c2ae: *stop=0xc90c2aeu;if(!valid_ram(r[5],1u))return -3;r[2]=sx(m,r[5],1);goto Lc90c2b0; /* mov.b @r5,r2 */
Lc90c2b0: *stop=0xc90c2b0u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc90c2b2; /* tst r2,r2 */
Lc90c2b2: take=(*sr&1u);*stop=0xc90c2b4u;r[13]=r[6];if(take)goto Lc90c368;goto Lc90c2b6; /* bt/s 0x0c90c368 */
Lc90c2b6: *stop=0xc90c2b6u;r[0]=r[6];goto Lc90c2b8; /* mov r6,r0 */
Lc90c2b8: *stop=0xc90c2b8u;if(!valid_ram(r[5]+7u,1u))return -3;wr(m,r[5]+0x7u,r[0],1);goto Lc90c2ba; /* mov.b r0,@(0x7,r5) */
Lc90c2ba: *stop=0xc90c2bau;if(!valid_ram(r[5],1u))return -3;r[3]=sx(m,r[5],1);goto Lc90c2bc; /* mov.b @r5,r3 */
Lc90c2bc: *stop=0xc90c2bcu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc90c2be; /* tst r3,r3 */
Lc90c2be: if((*sr&1u))goto Lc90c2ca;goto Lc90c2c0; /* bt 0x0c90c2ca */
Lc90c2c0: *stop=0xc90c2c0u;r[5]+=1u;goto Lc90c2c2; /* add #0x1,r5 */
Lc90c2c2: *stop=0xc90c2c2u;if(!valid_ram(r[5],1u))return -3;r[3]=sx(m,r[5],1);goto Lc90c2c4; /* mov.b @r5,r3 */
Lc90c2c4: *stop=0xc90c2c4u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc90c2c6; /* tst r3,r3 */
Lc90c2c6: take=!(*sr&1u);*stop=0xc90c2c8u;r[12]+=1u;if(take)goto Lc90c2c0;goto Lc90c2ca; /* bf/s 0x0c90c2c0 */
Lc90c2ca: *stop=0xc90c2cau;r[10]=rd(m,0x0c90c3e0u,4);goto Lc90c2cc; /* mov.l 0x0c90c3e0,r10 */
Lc90c2cc: *stop=0xc90c2ccu;r[14]=r[4];goto Lc90c2ce; /* mov r4,r14 */
Lc90c2ce: *stop=0xc90c2ceu;r[14]+=4u;goto Lc90c2d0; /* add #0x4,r14 */
Lc90c2d0: target=r[10];*pr=0x0c90c2d4u;*stop=0xc90c2d2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c2d4; /* jsr @r10 */
Lc90c2d4: *stop=0xc90c2d4u;r[3]=rd(m,0x0c90c3e4u,4);goto Lc90c2d6; /* mov.l 0x0c90c3e4,r3 */
Lc90c2d6: *stop=0xc90c2d6u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto Lc90c2d8; /* mov.l @r15,r5 */
Lc90c2d8: target=r[3];*pr=0x0c90c2dcu;*stop=0xc90c2dau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c2dc; /* jsr @r3 */
Lc90c2dc: *stop=0xc90c2dcu;r[5]=r[15];goto Lc90c2de; /* mov r15,r5 */
Lc90c2de: *stop=0xc90c2deu;r[2]=rd(m,0x0c90c3e8u,4);goto Lc90c2e0; /* mov.l 0x0c90c3e8,r2 */
Lc90c2e0: *stop=0xc90c2e0u;r[6]=r[15];goto Lc90c2e2; /* mov r15,r6 */
Lc90c2e2: *stop=0xc90c2e2u;r[5]+=16u;goto Lc90c2e4; /* add #0x10,r5 */
Lc90c2e4: *stop=0xc90c2e4u;r[6]+=4u;goto Lc90c2e6; /* add #0x4,r6 */
Lc90c2e6: target=r[2];*pr=0x0c90c2eau;*stop=0xc90c2e8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c2ea; /* jsr @r2 */
Lc90c2ea: *stop=0xc90c2eau;r[11]=rd(m,0x0c90c3ecu,4);goto Lc90c2ec; /* mov.l 0x0c90c3ec,r11 */
Lc90c2ec: target=r[11];*pr=0x0c90c2f0u;*stop=0xc90c2eeu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c2f0; /* jsr @r11 */
Lc90c2f0: *stop=0xc90c2f0u;r[4]=rd(m,0x0c90c3f0u,4);goto Lc90c2f2; /* mov.l 0x0c90c3f0,r4 */
Lc90c2f2: target=r[10];*pr=0x0c90c2f6u;*stop=0xc90c2f4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c2f6; /* jsr @r10 */
Lc90c2f6: *stop=0xc90c2f6u;r[3]=rd(m,0x0c90c3e4u,4);goto Lc90c2f8; /* mov.l 0x0c90c3e4,r3 */
Lc90c2f8: *stop=0xc90c2f8u;r[5]=r[15];goto Lc90c2fa; /* mov r15,r5 */
Lc90c2fa: *stop=0xc90c2fau;r[5]+=4u;goto Lc90c2fc; /* add #0x4,r5 */
Lc90c2fc: target=r[3];*pr=0x0c90c300u;*stop=0xc90c2feu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c300; /* jsr @r3 */
Lc90c300: *stop=0xc90c300u;*fpul=r[12];goto Lc90c302; /* lds r12,FPUL */
Lc90c302: *stop=0xc90c302u;r[0]=0x0c90c3f4u;goto Lc90c304; /* mova 0x0c90c3f4,r0 */
Lc90c304: *stop=0xc90c304u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto Lc90c306; /* fldi1 fr2 */
Lc90c306: *stop=0xc90c306u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto Lc90c308; /* fneg fr2 */
Lc90c308: *stop=0xc90c308u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc90c30a; /* float FPUL,fr3 */
Lc90c30a: *stop=0xc90c30au;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto Lc90c30c; /* fmov.s @r0,fr1 */
Lc90c30c: *stop=0xc90c30cu;if(!valid_ram(r[14],1u))return -3;r[3]=sx(m,r[14],1);goto Lc90c30e; /* mov.b @r14,r3 */
Lc90c30e: *stop=0xc90c30eu;r[9]=rd(m,0x0c90c3f8u,4);goto Lc90c310; /* mov.l 0x0c90c3f8,r9 */
Lc90c310: *stop=0xc90c310u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc90c312; /* tst r3,r3 */
Lc90c312: *stop=0xc90c312u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc90c314; /* fadd fr2,fr3 */
Lc90c314: *stop=0xc90c314u;fcopy(f,12u,3u);goto Lc90c316; /* fmov fr3,fr12 */
Lc90c316: *stop=0xc90c316u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[1],&f->fpscr,&fr[12]);if(status)return status;}goto Lc90c318; /* fmul fr1,fr12 */
Lc90c318: take=(*sr&1u);*stop=0xc90c31au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0;if(take)goto Lc90c356;goto Lc90c31c; /* bt/s 0x0c90c356 */
Lc90c31c: *stop=0xc90c31cu;*fpul=r[13];goto Lc90c31e; /* lds r13,FPUL */
Lc90c31e: *stop=0xc90c31eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc90c320; /* float FPUL,fr3 */
Lc90c320: *stop=0xc90c320u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc90c322; /* fsub fr12,fr3 */
Lc90c322: *stop=0xc90c322u;fcopy(f,13u,3u);goto Lc90c324; /* fmov fr3,fr13 */
Lc90c324: *stop=0xc90c324u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[15],&f->fpscr,&fr[13]);if(status)return status;}goto Lc90c326; /* fmul fr15,fr13 */
Lc90c326: target=r[10];*pr=0x0c90c32au;*stop=0xc90c328u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c32a; /* jsr @r10 */
Lc90c32a: *stop=0xc90c32au;r[3]=rd(m,0x0c90c3fcu,4);goto Lc90c32c; /* mov.l 0x0c90c3fc,r3 */
Lc90c32c: *stop=0xc90c32cu;fcopy(f,4u,13u);goto Lc90c32e; /* fmov fr13,fr4 */
Lc90c32e: *stop=0xc90c32eu;fcopy(f,6u,14u);goto Lc90c330; /* fmov fr14,fr6 */
Lc90c330: *stop=0xc90c330u;fcopy(f,5u,14u);goto Lc90c332; /* fmov fr14,fr5 */
Lc90c332: target=r[3];*pr=0x0c90c336u;*stop=0xc90c334u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c336; /* jsr @r3 */
Lc90c336: *stop=0xc90c336u;fcopy(f,4u,15u);goto Lc90c338; /* fmov fr15,fr4 */
Lc90c338: *stop=0xc90c338u;fcopy(f,5u,15u);goto Lc90c33a; /* fmov fr15,fr5 */
Lc90c33a: *stop=0xc90c33au;fcopy(f,6u,15u);goto Lc90c33c; /* fmov fr15,fr6 */
Lc90c33c: target=r[9];*pr=0x0c90c340u;*stop=0xc90c33eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c340; /* jsr @r9 */
Lc90c340: *stop=0xc90c340u;r[5]=r[8];goto Lc90c342; /* mov r8,r5 */
Lc90c342: target=0x0c90bfc0u;*pr=0x0c90c346u;*stop=0xc90c344u;if(!valid_ram(r[14],1u))return -3;r[4]=sx(m,r[14],1);r[14]+=1;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c346; /* bsr 0x0c90bfc0 */
Lc90c346: target=r[11];*pr=0x0c90c34au;*stop=0xc90c348u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c34a; /* jsr @r11 */
Lc90c34a: *stop=0xc90c34au;r[13]+=1u;goto Lc90c34c; /* add #0x1,r13 */
Lc90c34c: *stop=0xc90c34cu;*sr=(*sr&~1u)|(r[12]==r[13]);goto Lc90c34e; /* cmp/eq r12,r13 */
Lc90c34e: if((*sr&1u))goto Lc90c356;goto Lc90c350; /* bt 0x0c90c356 */
Lc90c350: *stop=0xc90c350u;if(!valid_ram(r[14],1u))return -3;r[3]=sx(m,r[14],1);goto Lc90c352; /* mov.b @r14,r3 */
Lc90c352: *stop=0xc90c352u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc90c354; /* tst r3,r3 */
Lc90c354: if(!(*sr&1u))goto Lc90c31c;goto Lc90c356; /* bf 0x0c90c31c */
Lc90c356: *stop=0xc90c356u;fcopy(f,4u,15u);goto Lc90c358; /* fmov fr15,fr4 */
Lc90c358: *stop=0xc90c358u;fcopy(f,5u,15u);goto Lc90c35a; /* fmov fr15,fr5 */
Lc90c35a: *stop=0xc90c35au;fcopy(f,6u,15u);goto Lc90c35c; /* fmov fr15,fr6 */
Lc90c35c: target=r[9];*pr=0x0c90c360u;*stop=0xc90c35eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c360; /* jsr @r9 */
Lc90c360: target=0x0c90c160u;*pr=0x0c90c364u;*stop=0xc90c362u;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c364; /* bsr 0x0c90c160 */
Lc90c364: target=r[11];*pr=0x0c90c368u;*stop=0xc90c366u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c368; /* jsr @r11 */
Lc90c368: *stop=0xc90c368u;r[15]+=28u;goto Lc90c36a; /* add #0x1c,r15 */
Lc90c36a: *stop=0xc90c36au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc90c36c; /* lds.l @r15+,PR */
Lc90c36c: *stop=0xc90c36cu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc90c36e; /* fmov.s @r15+,fr12 */
Lc90c36e: *stop=0xc90c36eu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc90c370; /* fmov.s @r15+,fr13 */
Lc90c370: *stop=0xc90c370u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc90c372; /* fmov.s @r15+,fr14 */
Lc90c372: *stop=0xc90c372u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc90c374; /* fmov.s @r15+,fr15 */
Lc90c374: *stop=0xc90c374u;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto Lc90c376; /* mov.l @r15+,r8 */
Lc90c376: *stop=0xc90c376u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto Lc90c378; /* mov.l @r15+,r9 */
Lc90c378: *stop=0xc90c378u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto Lc90c37a; /* mov.l @r15+,r10 */
Lc90c37a: *stop=0xc90c37au;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto Lc90c37c; /* mov.l @r15+,r11 */
Lc90c37c: *stop=0xc90c37cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto Lc90c37e; /* mov.l @r15+,r12 */
Lc90c37e: *stop=0xc90c37eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto Lc90c380; /* mov.l @r15+,r13 */
Lc90c380: *stop=0xc90c380u;if(*pr!=entry_pr)return -3;*stop=0xc90c382u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_fade_glyph_0c90bfc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc90bfc0;
Lc90bfc0: *stop=0xc90bfc0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc90bfc2; /* sts.l PR,@-r15 */
Lc90bfc2: *stop=0xc90bfc2u;r[15]+=4294967200u;goto Lc90bfc4; /* add #-0x60,r15 */
Lc90bfc4: *stop=0xc90bfc4u;r[3]=rd(m,0x0c90c140u,4);goto Lc90bfc6; /* mov.l 0x0c90c140,r3 */
Lc90bfc6: *stop=0xc90bfc6u;r[2]=r[15];goto Lc90bfc8; /* mov r15,r2 */
Lc90bfc8: *stop=0xc90bfc8u;r[2]+=32u;goto Lc90bfca; /* add #0x20,r2 */
Lc90bfca: *stop=0xc90bfcau;r[6]=r[15];goto Lc90bfcc; /* mov r15,r6 */
Lc90bfcc: *stop=0xc90bfccu;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto Lc90bfce; /* mov.l @(0x4,r3),r0 */
Lc90bfce: *stop=0xc90bfceu;r[6]+=32u;goto Lc90bfd0; /* add #0x20,r6 */
Lc90bfd0: *stop=0xc90bfd0u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto Lc90bfd2; /* mov.l @r3,r1 */
Lc90bfd2: *stop=0xc90bfd2u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc90bfd4; /* mov.l r1,@r2 */
Lc90bfd4: *stop=0xc90bfd4u;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto Lc90bfd6; /* mov.l r0,@(0x4,r2) */
Lc90bfd6: *stop=0xc90bfd6u;if(!valid_ram(r[3]+12u,4u))return -3;r[0]=rd(m,r[3]+0xcu,4);goto Lc90bfd8; /* mov.l @(0xc,r3),r0 */
Lc90bfd8: *stop=0xc90bfd8u;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto Lc90bfda; /* mov.l @(0x8,r3),r1 */
Lc90bfda: *stop=0xc90bfdau;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto Lc90bfdc; /* mov.l r1,@(0x8,r2) */
Lc90bfdc: *stop=0xc90bfdcu;if(!valid_ram(r[2]+12u,4u))return -3;wr(m,r[2]+0xcu,r[0],4);goto Lc90bfde; /* mov.l r0,@(0xc,r2) */
Lc90bfde: *stop=0xc90bfdeu;if(!valid_ram(r[3]+20u,4u))return -3;r[0]=rd(m,r[3]+0x14u,4);goto Lc90bfe0; /* mov.l @(0x14,r3),r0 */
Lc90bfe0: *stop=0xc90bfe0u;if(!valid_ram(r[3]+16u,4u))return -3;r[1]=rd(m,r[3]+0x10u,4);goto Lc90bfe2; /* mov.l @(0x10,r3),r1 */
Lc90bfe2: *stop=0xc90bfe2u;if(!valid_ram(r[2]+16u,4u))return -3;wr(m,r[2]+0x10u,r[1],4);goto Lc90bfe4; /* mov.l r1,@(0x10,r2) */
Lc90bfe4: *stop=0xc90bfe4u;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[0],4);goto Lc90bfe6; /* mov.l r0,@(0x14,r2) */
Lc90bfe6: *stop=0xc90bfe6u;if(!valid_ram(r[3]+28u,4u))return -3;r[0]=rd(m,r[3]+0x1cu,4);goto Lc90bfe8; /* mov.l @(0x1c,r3),r0 */
Lc90bfe8: *stop=0xc90bfe8u;if(!valid_ram(r[3]+24u,4u))return -3;r[1]=rd(m,r[3]+0x18u,4);goto Lc90bfea; /* mov.l @(0x18,r3),r1 */
Lc90bfea: *stop=0xc90bfeau;if(!valid_ram(r[2]+24u,4u))return -3;wr(m,r[2]+0x18u,r[1],4);goto Lc90bfec; /* mov.l r1,@(0x18,r2) */
Lc90bfec: *stop=0xc90bfecu;if(!valid_ram(r[2]+28u,4u))return -3;wr(m,r[2]+0x1cu,r[0],4);goto Lc90bfee; /* mov.l r0,@(0x1c,r2) */
Lc90bfee: *stop=0xc90bfeeu;if(!valid_ram(r[3]+36u,4u))return -3;r[0]=rd(m,r[3]+0x24u,4);goto Lc90bff0; /* mov.l @(0x24,r3),r0 */
Lc90bff0: *stop=0xc90bff0u;if(!valid_ram(r[3]+32u,4u))return -3;r[1]=rd(m,r[3]+0x20u,4);goto Lc90bff2; /* mov.l @(0x20,r3),r1 */
Lc90bff2: *stop=0xc90bff2u;if(!valid_ram(r[2]+32u,4u))return -3;wr(m,r[2]+0x20u,r[1],4);goto Lc90bff4; /* mov.l r1,@(0x20,r2) */
Lc90bff4: *stop=0xc90bff4u;if(!valid_ram(r[2]+36u,4u))return -3;wr(m,r[2]+0x24u,r[0],4);goto Lc90bff6; /* mov.l r0,@(0x24,r2) */
Lc90bff6: *stop=0xc90bff6u;if(!valid_ram(r[3]+44u,4u))return -3;r[0]=rd(m,r[3]+0x2cu,4);goto Lc90bff8; /* mov.l @(0x2c,r3),r0 */
Lc90bff8: *stop=0xc90bff8u;if(!valid_ram(r[3]+40u,4u))return -3;r[1]=rd(m,r[3]+0x28u,4);goto Lc90bffa; /* mov.l @(0x28,r3),r1 */
Lc90bffa: *stop=0xc90bffau;if(!valid_ram(r[2]+40u,4u))return -3;wr(m,r[2]+0x28u,r[1],4);goto Lc90bffc; /* mov.l r1,@(0x28,r2) */
Lc90bffc: *stop=0xc90bffcu;if(!valid_ram(r[2]+44u,4u))return -3;wr(m,r[2]+0x2cu,r[0],4);goto Lc90bffe; /* mov.l r0,@(0x2c,r2) */
Lc90bffe: *stop=0xc90bffeu;r[0]=80u;goto Lc90c000; /* mov #0x50,r0 */
Lc90c000: *stop=0xc90c000u;r[2]=r[15];goto Lc90c002; /* mov r15,r2 */
Lc90c002: *stop=0xc90c002u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[6],4);goto Lc90c004; /* mov.l r6,@(r0,r15) */
Lc90c004: *stop=0xc90c004u;r[0]=84u;goto Lc90c006; /* mov #0x54,r0 */
Lc90c006: *stop=0xc90c006u;r[1]=r[15];goto Lc90c008; /* mov r15,r1 */
Lc90c008: *stop=0xc90c008u;r[2]+=16u;goto Lc90c00a; /* add #0x10,r2 */
Lc90c00a: *stop=0xc90c00au;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc90c00c; /* mov.l r2,@(r0,r15) */
Lc90c00c: *stop=0xc90c00cu;r[0]=88u;goto Lc90c00e; /* mov #0x58,r0 */
Lc90c00e: *stop=0xc90c00eu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[1],4);goto Lc90c010; /* mov.l r1,@(r0,r15) */
Lc90c010: *stop=0xc90c010u;r[0]=92u;goto Lc90c012; /* mov #0x5c,r0 */
Lc90c012: *stop=0xc90c012u;r[2]=4u;goto Lc90c014; /* mov #0x4,r2 */
Lc90c014: *stop=0xc90c014u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc90c016; /* mov.l r2,@(r0,r15) */
Lc90c016: *stop=0xc90c016u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto Lc90c018; /* tst r5,r5 */
Lc90c018: *stop=0xc90c018u;r[0]=84u;goto Lc90c01a; /* mov #0x54,r0 */
Lc90c01a: take=(*sr&1u);*stop=0xc90c01cu;if(!valid_ram(r[0]+r[15],4u))return -3;r[6]=rd(m,r[15]+r[0],4);if(take)goto Lc90c072;goto Lc90c01e; /* bt/s 0x0c90c072 */
Lc90c01e: *stop=0xc90c01eu;if(!valid_ram(r[5],1u))return -3;r[0]=sx(m,r[5],1);goto Lc90c020; /* mov.b @r5,r0 */
Lc90c020: *stop=0xc90c020u;if(!valid_ram(r[6],1u))return -3;wr(m,r[6],r[0],1);goto Lc90c022; /* mov.b r0,@r6 */
Lc90c022: *stop=0xc90c022u;if(!valid_ram(r[5]+1u,1u))return -3;r[0]=sx(m,r[5]+0x1u,1);goto Lc90c024; /* mov.b @(0x1,r5),r0 */
Lc90c024: *stop=0xc90c024u;if(!valid_ram(r[6]+1u,1u))return -3;wr(m,r[6]+0x1u,r[0],1);goto Lc90c026; /* mov.b r0,@(0x1,r6) */
Lc90c026: *stop=0xc90c026u;if(!valid_ram(r[5]+2u,1u))return -3;r[0]=sx(m,r[5]+0x2u,1);goto Lc90c028; /* mov.b @(0x2,r5),r0 */
Lc90c028: *stop=0xc90c028u;if(!valid_ram(r[6]+2u,1u))return -3;wr(m,r[6]+0x2u,r[0],1);goto Lc90c02a; /* mov.b r0,@(0x2,r6) */
Lc90c02a: *stop=0xc90c02au;if(!valid_ram(r[5]+3u,1u))return -3;r[0]=sx(m,r[5]+0x3u,1);goto Lc90c02c; /* mov.b @(0x3,r5),r0 */
Lc90c02c: *stop=0xc90c02cu;if(!valid_ram(r[6]+3u,1u))return -3;wr(m,r[6]+0x3u,r[0],1);goto Lc90c02e; /* mov.b r0,@(0x3,r6) */
Lc90c02e: *stop=0xc90c02eu;r[0]=84u;goto Lc90c030; /* mov #0x54,r0 */
Lc90c030: *stop=0xc90c030u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c032; /* mov.l @(r0,r15),r2 */
Lc90c032: *stop=0xc90c032u;if(!valid_ram(r[5],1u))return -3;r[0]=sx(m,r[5],1);goto Lc90c034; /* mov.b @r5,r0 */
Lc90c034: *stop=0xc90c034u;r[2]+=4u;goto Lc90c036; /* add #0x4,r2 */
Lc90c036: *stop=0xc90c036u;if(!valid_ram(r[2],1u))return -3;wr(m,r[2],r[0],1);goto Lc90c038; /* mov.b r0,@r2 */
Lc90c038: *stop=0xc90c038u;if(!valid_ram(r[5]+1u,1u))return -3;r[0]=sx(m,r[5]+0x1u,1);goto Lc90c03a; /* mov.b @(0x1,r5),r0 */
Lc90c03a: *stop=0xc90c03au;if(!valid_ram(r[2]+1u,1u))return -3;wr(m,r[2]+0x1u,r[0],1);goto Lc90c03c; /* mov.b r0,@(0x1,r2) */
Lc90c03c: *stop=0xc90c03cu;if(!valid_ram(r[5]+2u,1u))return -3;r[0]=sx(m,r[5]+0x2u,1);goto Lc90c03e; /* mov.b @(0x2,r5),r0 */
Lc90c03e: *stop=0xc90c03eu;if(!valid_ram(r[2]+2u,1u))return -3;wr(m,r[2]+0x2u,r[0],1);goto Lc90c040; /* mov.b r0,@(0x2,r2) */
Lc90c040: *stop=0xc90c040u;if(!valid_ram(r[5]+3u,1u))return -3;r[0]=sx(m,r[5]+0x3u,1);goto Lc90c042; /* mov.b @(0x3,r5),r0 */
Lc90c042: *stop=0xc90c042u;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto Lc90c044; /* mov.b r0,@(0x3,r2) */
Lc90c044: *stop=0xc90c044u;r[0]=84u;goto Lc90c046; /* mov #0x54,r0 */
Lc90c046: *stop=0xc90c046u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c048; /* mov.l @(r0,r15),r2 */
Lc90c048: *stop=0xc90c048u;if(!valid_ram(r[5],1u))return -3;r[0]=sx(m,r[5],1);goto Lc90c04a; /* mov.b @r5,r0 */
Lc90c04a: *stop=0xc90c04au;r[2]+=8u;goto Lc90c04c; /* add #0x8,r2 */
Lc90c04c: *stop=0xc90c04cu;if(!valid_ram(r[2],1u))return -3;wr(m,r[2],r[0],1);goto Lc90c04e; /* mov.b r0,@r2 */
Lc90c04e: *stop=0xc90c04eu;if(!valid_ram(r[5]+1u,1u))return -3;r[0]=sx(m,r[5]+0x1u,1);goto Lc90c050; /* mov.b @(0x1,r5),r0 */
Lc90c050: *stop=0xc90c050u;if(!valid_ram(r[2]+1u,1u))return -3;wr(m,r[2]+0x1u,r[0],1);goto Lc90c052; /* mov.b r0,@(0x1,r2) */
Lc90c052: *stop=0xc90c052u;if(!valid_ram(r[5]+2u,1u))return -3;r[0]=sx(m,r[5]+0x2u,1);goto Lc90c054; /* mov.b @(0x2,r5),r0 */
Lc90c054: *stop=0xc90c054u;if(!valid_ram(r[2]+2u,1u))return -3;wr(m,r[2]+0x2u,r[0],1);goto Lc90c056; /* mov.b r0,@(0x2,r2) */
Lc90c056: *stop=0xc90c056u;if(!valid_ram(r[5]+3u,1u))return -3;r[0]=sx(m,r[5]+0x3u,1);goto Lc90c058; /* mov.b @(0x3,r5),r0 */
Lc90c058: *stop=0xc90c058u;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto Lc90c05a; /* mov.b r0,@(0x3,r2) */
Lc90c05a: *stop=0xc90c05au;r[0]=84u;goto Lc90c05c; /* mov #0x54,r0 */
Lc90c05c: *stop=0xc90c05cu;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c05e; /* mov.l @(r0,r15),r2 */
Lc90c05e: *stop=0xc90c05eu;if(!valid_ram(r[5],1u))return -3;r[0]=sx(m,r[5],1);goto Lc90c060; /* mov.b @r5,r0 */
Lc90c060: *stop=0xc90c060u;r[2]+=12u;goto Lc90c062; /* add #0xc,r2 */
Lc90c062: *stop=0xc90c062u;if(!valid_ram(r[2],1u))return -3;wr(m,r[2],r[0],1);goto Lc90c064; /* mov.b r0,@r2 */
Lc90c064: *stop=0xc90c064u;if(!valid_ram(r[5]+1u,1u))return -3;r[0]=sx(m,r[5]+0x1u,1);goto Lc90c066; /* mov.b @(0x1,r5),r0 */
Lc90c066: *stop=0xc90c066u;if(!valid_ram(r[2]+1u,1u))return -3;wr(m,r[2]+0x1u,r[0],1);goto Lc90c068; /* mov.b r0,@(0x1,r2) */
Lc90c068: *stop=0xc90c068u;if(!valid_ram(r[5]+2u,1u))return -3;r[0]=sx(m,r[5]+0x2u,1);goto Lc90c06a; /* mov.b @(0x2,r5),r0 */
Lc90c06a: *stop=0xc90c06au;if(!valid_ram(r[2]+2u,1u))return -3;wr(m,r[2]+0x2u,r[0],1);goto Lc90c06c; /* mov.b r0,@(0x2,r2) */
Lc90c06c: *stop=0xc90c06cu;if(!valid_ram(r[5]+3u,1u))return -3;r[0]=sx(m,r[5]+0x3u,1);goto Lc90c06e; /* mov.b @(0x3,r5),r0 */
Lc90c06e: *stop=0xc90c070u;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto Lc90c08e; /* bra 0x0c90c08e */
Lc90c072: *stop=0xc90c072u;r[0]=84u;goto Lc90c074; /* mov #0x54,r0 */
Lc90c074: *stop=0xc90c074u;r[5]=4294967295u;goto Lc90c076; /* mov #-0x1,r5 */
Lc90c076: *stop=0xc90c076u;if(!valid_ram(r[6],4u))return -3;wr(m,r[6],r[5],4);goto Lc90c078; /* mov.l r5,@r6 */
Lc90c078: *stop=0xc90c078u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c07a; /* mov.l @(r0,r15),r2 */
Lc90c07a: *stop=0xc90c07au;r[0]=84u;goto Lc90c07c; /* mov #0x54,r0 */
Lc90c07c: *stop=0xc90c07cu;r[2]+=4u;goto Lc90c07e; /* add #0x4,r2 */
Lc90c07e: *stop=0xc90c07eu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[5],4);goto Lc90c080; /* mov.l r5,@r2 */
Lc90c080: *stop=0xc90c080u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c082; /* mov.l @(r0,r15),r2 */
Lc90c082: *stop=0xc90c082u;r[0]=84u;goto Lc90c084; /* mov #0x54,r0 */
Lc90c084: *stop=0xc90c084u;r[2]+=8u;goto Lc90c086; /* add #0x8,r2 */
Lc90c086: *stop=0xc90c086u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[5],4);goto Lc90c088; /* mov.l r5,@r2 */
Lc90c088: *stop=0xc90c088u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c08a; /* mov.l @(r0,r15),r2 */
Lc90c08a: *stop=0xc90c08au;r[2]+=12u;goto Lc90c08c; /* add #0xc,r2 */
Lc90c08c: *stop=0xc90c08cu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[5],4);goto Lc90c08e; /* mov.l r5,@r2 */
Lc90c08e: *stop=0xc90c08eu;r[3]=32u;goto Lc90c090; /* mov #0x20,r3 */
Lc90c090: *stop=0xc90c090u;r[5]=r[4]&255u;goto Lc90c092; /* extu.b r4,r5 */
Lc90c092: *stop=0xc90c092u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]);goto Lc90c094; /* cmp/ge r3,r5 */
Lc90c094: if(!(*sr&1u))goto Lc90c0a6;goto Lc90c096; /* bf 0x0c90c0a6 */
Lc90c096: *stop=0xc90c096u;r[1]=4294967231u;goto Lc90c098; /* mov #-0x41,r1 */
Lc90c098: *stop=0xc90c098u;r[1]=r[1]&255u;goto Lc90c09a; /* extu.b r1,r1 */
Lc90c09a: *stop=0xc90c09au;*sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[1]);goto Lc90c09c; /* cmp/gt r1,r5 */
Lc90c09c: if((*sr&1u))goto Lc90c0a6;goto Lc90c09e; /* bt 0x0c90c0a6 */
Lc90c09e: *stop=0xc90c09eu;r[5]=4294967264u;goto Lc90c0a0; /* mov #-0x20,r5 */
Lc90c0a0: *stop=0xc90c0a0u;r[5]=r[5]&255u;goto Lc90c0a2; /* extu.b r5,r5 */
Lc90c0a2: *stop=0xc90c0a4u;r[5]+=r[4];goto Lc90c0a8; /* bra 0x0c90c0a8 */
Lc90c0a6: *stop=0xc90c0a6u;r[5]=0u;goto Lc90c0a8; /* mov #0x0,r5 */
Lc90c0a8: *stop=0xc90c0a8u;r[6]=r[5]&255u;goto Lc90c0aa; /* extu.b r5,r6 */
Lc90c0aa: *stop=0xc90c0aau;r[0]=r[6];goto Lc90c0ac; /* mov r6,r0 */
Lc90c0ac: *stop=0xc90c0acu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto Lc90c0ae; /* cmp/pz r0 */
Lc90c0ae: if(!(*sr&1u))goto Lc90c0b4;goto Lc90c0b0; /* bf 0x0c90c0b4 */
Lc90c0b0: *stop=0xc90c0b2u;r[0]&=0xfu;goto Lc90c0be; /* bra 0x0c90c0be */
Lc90c0b4: *stop=0xc90c0b4u;r[0]=~r[0];goto Lc90c0b6; /* not r0,r0 */
Lc90c0b6: *stop=0xc90c0b6u;r[0]+=1u;goto Lc90c0b8; /* add #0x1,r0 */
Lc90c0b8: *stop=0xc90c0b8u;r[0]&=0xfu;goto Lc90c0ba; /* and #0xf,r0 */
Lc90c0ba: *stop=0xc90c0bau;r[0]=~r[0];goto Lc90c0bc; /* not r0,r0 */
Lc90c0bc: *stop=0xc90c0bcu;r[0]+=1u;goto Lc90c0be; /* add #0x1,r0 */
Lc90c0be: *stop=0xc90c0beu;r[3]=4294967292u;goto Lc90c0c0; /* mov #-0x4,r3 */
Lc90c0c0: *stop=0xc90c0c0u;r[4]=r[0];goto Lc90c0c2; /* mov r0,r4 */
Lc90c0c2: *stop=0xc90c0c2u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[6]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[6]=n?(uint32_t)((int32_t)r[6]>>n):((r[6]&0x80000000u)?0xffffffffu:0u);}}goto Lc90c0c4; /* shad r3,r6 */
Lc90c0c4: *stop=0xc90c0c4u;r[5]=r[6];goto Lc90c0c6; /* mov r6,r5 */
Lc90c0c6: *stop=0xc90c0c6u;r[0]=88u;goto Lc90c0c8; /* mov #0x58,r0 */
Lc90c0c8: *stop=0xc90c0c8u;r[5]<<=2;goto Lc90c0ca; /* shll2 r5 */
Lc90c0ca: *stop=0xc90c0cau;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c0cc; /* mov.l @(r0,r15),r2 */
Lc90c0cc: *stop=0xc90c0ccu;r[5]<<=2;goto Lc90c0ce; /* shll2 r5 */
Lc90c0ce: *stop=0xc90c0ceu;r[7]=r[5];goto Lc90c0d0; /* mov r5,r7 */
Lc90c0d0: *stop=0xc90c0d0u;r[0]=88u;goto Lc90c0d2; /* mov #0x58,r0 */
Lc90c0d2: *stop=0xc90c0d2u;r[7]+=15u;goto Lc90c0d4; /* add #0xf,r7 */
Lc90c0d4: *stop=0xc90c0d4u;r[4]<<=2;goto Lc90c0d6; /* shll2 r4 */
Lc90c0d6: *stop=0xc90c0d6u;r[4]<<=2;goto Lc90c0d8; /* shll2 r4 */
Lc90c0d8: *stop=0xc90c0d8u;r[6]=r[4];goto Lc90c0da; /* mov r4,r6 */
Lc90c0da: *stop=0xc90c0dau;r[6]+=15u;goto Lc90c0dc; /* add #0xf,r6 */
Lc90c0dc: *stop=0xc90c0dcu;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[4],2);goto Lc90c0de; /* mov.w r4,@r2 */
Lc90c0de: *stop=0xc90c0deu;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc90c0e0; /* mov.l @(r0,r15),r3 */
Lc90c0e0: *stop=0xc90c0e0u;r[0]=r[7];goto Lc90c0e2; /* mov r7,r0 */
Lc90c0e2: *stop=0xc90c0e2u;if(!valid_ram(r[3]+2u,2u))return -3;wr(m,r[3]+0x2u,r[0],2);goto Lc90c0e4; /* mov.w r0,@(0x2,r3) */
Lc90c0e4: *stop=0xc90c0e4u;r[0]=88u;goto Lc90c0e6; /* mov #0x58,r0 */
Lc90c0e6: *stop=0xc90c0e6u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc90c0e8; /* mov.l @(r0,r15),r3 */
Lc90c0e8: *stop=0xc90c0e8u;r[0]=88u;goto Lc90c0ea; /* mov #0x58,r0 */
Lc90c0ea: *stop=0xc90c0eau;r[3]+=4u;goto Lc90c0ec; /* add #0x4,r3 */
Lc90c0ec: *stop=0xc90c0ecu;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[4],2);goto Lc90c0ee; /* mov.w r4,@r3 */
Lc90c0ee: *stop=0xc90c0eeu;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc90c0f0; /* mov.l @(r0,r15),r3 */
Lc90c0f0: *stop=0xc90c0f0u;r[0]=r[5];goto Lc90c0f2; /* mov r5,r0 */
Lc90c0f2: *stop=0xc90c0f2u;r[3]+=4u;goto Lc90c0f4; /* add #0x4,r3 */
Lc90c0f4: *stop=0xc90c0f4u;if(!valid_ram(r[3]+2u,2u))return -3;wr(m,r[3]+0x2u,r[0],2);goto Lc90c0f6; /* mov.w r0,@(0x2,r3) */
Lc90c0f6: *stop=0xc90c0f6u;r[0]=88u;goto Lc90c0f8; /* mov #0x58,r0 */
Lc90c0f8: *stop=0xc90c0f8u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc90c0fa; /* mov.l @(r0,r15),r3 */
Lc90c0fa: *stop=0xc90c0fau;r[3]+=8u;goto Lc90c0fc; /* add #0x8,r3 */
Lc90c0fc: *stop=0xc90c0fcu;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[6],2);goto Lc90c0fe; /* mov.w r6,@r3 */
Lc90c0fe: *stop=0xc90c0feu;r[0]=88u;goto Lc90c100; /* mov #0x58,r0 */
Lc90c100: *stop=0xc90c100u;r[4]=rd(m,0x0c90c144u,4);goto Lc90c102; /* mov.l 0x0c90c144,r4 */
Lc90c102: *stop=0xc90c102u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc90c104; /* mov.l @(r0,r15),r3 */
Lc90c104: *stop=0xc90c104u;r[0]=r[5];goto Lc90c106; /* mov r5,r0 */
Lc90c106: *stop=0xc90c106u;r[3]+=8u;goto Lc90c108; /* add #0x8,r3 */
Lc90c108: *stop=0xc90c108u;if(!valid_ram(r[3]+2u,2u))return -3;wr(m,r[3]+0x2u,r[0],2);goto Lc90c10a; /* mov.w r0,@(0x2,r3) */
Lc90c10a: *stop=0xc90c10au;r[0]=88u;goto Lc90c10c; /* mov #0x58,r0 */
Lc90c10c: *stop=0xc90c10cu;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc90c10e; /* mov.l @(r0,r15),r3 */
Lc90c10e: *stop=0xc90c10eu;r[0]=88u;goto Lc90c110; /* mov #0x58,r0 */
Lc90c110: *stop=0xc90c110u;r[3]+=12u;goto Lc90c112; /* add #0xc,r3 */
Lc90c112: *stop=0xc90c112u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[6],2);goto Lc90c114; /* mov.w r6,@r3 */
Lc90c114: *stop=0xc90c114u;if(!valid_ram(r[0]+r[15],4u))return -3;r[3]=rd(m,r[15]+r[0],4);goto Lc90c116; /* mov.l @(r0,r15),r3 */
Lc90c116: *stop=0xc90c116u;r[0]=r[7];goto Lc90c118; /* mov r7,r0 */
Lc90c118: *stop=0xc90c118u;r[3]+=12u;goto Lc90c11a; /* add #0xc,r3 */
Lc90c11a: *stop=0xc90c11au;if(!valid_ram(r[3]+2u,2u))return -3;wr(m,r[3]+0x2u,r[0],2);goto Lc90c11c; /* mov.w r0,@(0x2,r3) */
Lc90c11c: *stop=0xc90c11cu;r[3]=rd(m,0x0c90c148u,4);goto Lc90c11e; /* mov.l 0x0c90c148,r3 */
Lc90c11e: target=r[3];*pr=0x0c90c122u;*stop=0xc90c120u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c122; /* jsr @r3 */
Lc90c122: *stop=0xc90c122u;r[2]=rd(m,0x0c90c14cu,4);goto Lc90c124; /* mov.l 0x0c90c14c,r2 */
Lc90c124: target=r[2];*pr=0x0c90c128u;*stop=0xc90c126u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c128; /* jsr @r2 */
Lc90c128: *stop=0xc90c128u;r[3]=rd(m,0x0c90c154u,4);goto Lc90c12a; /* mov.l 0x0c90c154,r3 */
Lc90c12a: *stop=0xc90c12au;r[4]=r[15];goto Lc90c12c; /* mov r15,r4 */
Lc90c12c: *stop=0xc90c12cu;r[6]=rd(m,0x0c90c150u,4);goto Lc90c12e; /* mov.l 0x0c90c150,r6 */
Lc90c12e: *stop=0xc90c12eu;r[5]=4u;goto Lc90c130; /* mov #0x4,r5 */
Lc90c130: target=r[3];*pr=0x0c90c134u;*stop=0xc90c132u;r[4]+=80u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c134; /* jsr @r3 */
Lc90c134: *stop=0xc90c134u;r[15]+=96u;goto Lc90c136; /* add #0x60,r15 */
Lc90c136: *stop=0xc90c136u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc90c138; /* lds.l @r15+,PR */
Lc90c138: *stop=0xc90c138u;if(*pr!=entry_pr)return -3;*stop=0xc90c13au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_fade_backdrop_0c90c160(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc90c160;
Lc90c160: *stop=0xc90c160u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc90c162; /* sts.l PR,@-r15 */
Lc90c162: *stop=0xc90c162u;r[15]+=4294967200u;goto Lc90c164; /* add #-0x60,r15 */
Lc90c164: *stop=0xc90c164u;r[3]=rd(m,0x0c90c3c0u,4);goto Lc90c166; /* mov.l 0x0c90c3c0,r3 */
Lc90c166: *stop=0xc90c166u;r[2]=r[15];goto Lc90c168; /* mov r15,r2 */
Lc90c168: *stop=0xc90c168u;r[2]+=32u;goto Lc90c16a; /* add #0x20,r2 */
Lc90c16a: *stop=0xc90c16au;r[5]=r[15];goto Lc90c16c; /* mov r15,r5 */
Lc90c16c: *stop=0xc90c16cu;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto Lc90c16e; /* mov.l @(0x4,r3),r0 */
Lc90c16e: *stop=0xc90c16eu;r[5]+=32u;goto Lc90c170; /* add #0x20,r5 */
Lc90c170: *stop=0xc90c170u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto Lc90c172; /* mov.l @r3,r1 */
Lc90c172: *stop=0xc90c172u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc90c174; /* mov.l r1,@r2 */
Lc90c174: *stop=0xc90c174u;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto Lc90c176; /* mov.l r0,@(0x4,r2) */
Lc90c176: *stop=0xc90c176u;if(!valid_ram(r[3]+12u,4u))return -3;r[0]=rd(m,r[3]+0xcu,4);goto Lc90c178; /* mov.l @(0xc,r3),r0 */
Lc90c178: *stop=0xc90c178u;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto Lc90c17a; /* mov.l @(0x8,r3),r1 */
Lc90c17a: *stop=0xc90c17au;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto Lc90c17c; /* mov.l r1,@(0x8,r2) */
Lc90c17c: *stop=0xc90c17cu;if(!valid_ram(r[2]+12u,4u))return -3;wr(m,r[2]+0xcu,r[0],4);goto Lc90c17e; /* mov.l r0,@(0xc,r2) */
Lc90c17e: *stop=0xc90c17eu;if(!valid_ram(r[3]+20u,4u))return -3;r[0]=rd(m,r[3]+0x14u,4);goto Lc90c180; /* mov.l @(0x14,r3),r0 */
Lc90c180: *stop=0xc90c180u;if(!valid_ram(r[3]+16u,4u))return -3;r[1]=rd(m,r[3]+0x10u,4);goto Lc90c182; /* mov.l @(0x10,r3),r1 */
Lc90c182: *stop=0xc90c182u;if(!valid_ram(r[2]+16u,4u))return -3;wr(m,r[2]+0x10u,r[1],4);goto Lc90c184; /* mov.l r1,@(0x10,r2) */
Lc90c184: *stop=0xc90c184u;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[0],4);goto Lc90c186; /* mov.l r0,@(0x14,r2) */
Lc90c186: *stop=0xc90c186u;if(!valid_ram(r[3]+28u,4u))return -3;r[0]=rd(m,r[3]+0x1cu,4);goto Lc90c188; /* mov.l @(0x1c,r3),r0 */
Lc90c188: *stop=0xc90c188u;if(!valid_ram(r[3]+24u,4u))return -3;r[1]=rd(m,r[3]+0x18u,4);goto Lc90c18a; /* mov.l @(0x18,r3),r1 */
Lc90c18a: *stop=0xc90c18au;if(!valid_ram(r[2]+24u,4u))return -3;wr(m,r[2]+0x18u,r[1],4);goto Lc90c18c; /* mov.l r1,@(0x18,r2) */
Lc90c18c: *stop=0xc90c18cu;if(!valid_ram(r[2]+28u,4u))return -3;wr(m,r[2]+0x1cu,r[0],4);goto Lc90c18e; /* mov.l r0,@(0x1c,r2) */
Lc90c18e: *stop=0xc90c18eu;if(!valid_ram(r[3]+36u,4u))return -3;r[0]=rd(m,r[3]+0x24u,4);goto Lc90c190; /* mov.l @(0x24,r3),r0 */
Lc90c190: *stop=0xc90c190u;if(!valid_ram(r[3]+32u,4u))return -3;r[1]=rd(m,r[3]+0x20u,4);goto Lc90c192; /* mov.l @(0x20,r3),r1 */
Lc90c192: *stop=0xc90c192u;if(!valid_ram(r[2]+32u,4u))return -3;wr(m,r[2]+0x20u,r[1],4);goto Lc90c194; /* mov.l r1,@(0x20,r2) */
Lc90c194: *stop=0xc90c194u;if(!valid_ram(r[2]+36u,4u))return -3;wr(m,r[2]+0x24u,r[0],4);goto Lc90c196; /* mov.l r0,@(0x24,r2) */
Lc90c196: *stop=0xc90c196u;if(!valid_ram(r[3]+44u,4u))return -3;r[0]=rd(m,r[3]+0x2cu,4);goto Lc90c198; /* mov.l @(0x2c,r3),r0 */
Lc90c198: *stop=0xc90c198u;if(!valid_ram(r[3]+40u,4u))return -3;r[1]=rd(m,r[3]+0x28u,4);goto Lc90c19a; /* mov.l @(0x28,r3),r1 */
Lc90c19a: *stop=0xc90c19au;if(!valid_ram(r[2]+40u,4u))return -3;wr(m,r[2]+0x28u,r[1],4);goto Lc90c19c; /* mov.l r1,@(0x28,r2) */
Lc90c19c: *stop=0xc90c19cu;if(!valid_ram(r[2]+44u,4u))return -3;wr(m,r[2]+0x2cu,r[0],4);goto Lc90c19e; /* mov.l r0,@(0x2c,r2) */
Lc90c19e: *stop=0xc90c19eu;r[0]=80u;goto Lc90c1a0; /* mov #0x50,r0 */
Lc90c1a0: *stop=0xc90c1a0u;r[2]=r[15];goto Lc90c1a2; /* mov r15,r2 */
Lc90c1a2: *stop=0xc90c1a2u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[5],4);goto Lc90c1a4; /* mov.l r5,@(r0,r15) */
Lc90c1a4: *stop=0xc90c1a4u;r[0]=84u;goto Lc90c1a6; /* mov #0x54,r0 */
Lc90c1a6: *stop=0xc90c1a6u;r[1]=r[15];goto Lc90c1a8; /* mov r15,r1 */
Lc90c1a8: *stop=0xc90c1a8u;r[2]+=16u;goto Lc90c1aa; /* add #0x10,r2 */
Lc90c1aa: *stop=0xc90c1aau;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc90c1ac; /* mov.l r2,@(r0,r15) */
Lc90c1ac: *stop=0xc90c1acu;r[0]=88u;goto Lc90c1ae; /* mov #0x58,r0 */
Lc90c1ae: *stop=0xc90c1aeu;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[1],4);goto Lc90c1b0; /* mov.l r1,@(r0,r15) */
Lc90c1b0: *stop=0xc90c1b0u;r[0]=92u;goto Lc90c1b2; /* mov #0x5c,r0 */
Lc90c1b2: *stop=0xc90c1b2u;r[2]=4u;goto Lc90c1b4; /* mov #0x4,r2 */
Lc90c1b4: *stop=0xc90c1b4u;if(!valid_ram(r[0]+r[15],4u))return -3;wr(m,r[15]+r[0],r[2],4);goto Lc90c1b6; /* mov.l r2,@(r0,r15) */
Lc90c1b6: *stop=0xc90c1b6u;r[0]=84u;goto Lc90c1b8; /* mov #0x54,r0 */
Lc90c1b8: *stop=0xc90c1b8u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c1ba; /* mov.l @(r0,r15),r2 */
Lc90c1ba: *stop=0xc90c1bau;r[5]=4294967295u;goto Lc90c1bc; /* mov #-0x1,r5 */
Lc90c1bc: *stop=0xc90c1bcu;r[0]=84u;goto Lc90c1be; /* mov #0x54,r0 */
Lc90c1be: *stop=0xc90c1beu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[5],4);goto Lc90c1c0; /* mov.l r5,@r2 */
Lc90c1c0: *stop=0xc90c1c0u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c1c2; /* mov.l @(r0,r15),r2 */
Lc90c1c2: *stop=0xc90c1c2u;r[0]=84u;goto Lc90c1c4; /* mov #0x54,r0 */
Lc90c1c4: *stop=0xc90c1c4u;r[2]+=4u;goto Lc90c1c6; /* add #0x4,r2 */
Lc90c1c6: *stop=0xc90c1c6u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[5],4);goto Lc90c1c8; /* mov.l r5,@r2 */
Lc90c1c8: *stop=0xc90c1c8u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c1ca; /* mov.l @(r0,r15),r2 */
Lc90c1ca: *stop=0xc90c1cau;r[0]=84u;goto Lc90c1cc; /* mov #0x54,r0 */
Lc90c1cc: *stop=0xc90c1ccu;r[2]+=8u;goto Lc90c1ce; /* add #0x8,r2 */
Lc90c1ce: *stop=0xc90c1ceu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[5],4);goto Lc90c1d0; /* mov.l r5,@r2 */
Lc90c1d0: *stop=0xc90c1d0u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c1d2; /* mov.l @(r0,r15),r2 */
Lc90c1d2: *stop=0xc90c1d2u;r[0]=84u;goto Lc90c1d4; /* mov #0x54,r0 */
Lc90c1d4: *stop=0xc90c1d4u;r[2]+=12u;goto Lc90c1d6; /* add #0xc,r2 */
Lc90c1d6: *stop=0xc90c1d6u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[5],4);goto Lc90c1d8; /* mov.l r5,@r2 */
Lc90c1d8: *stop=0xc90c1d8u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c1da; /* mov.l @(r0,r15),r2 */
Lc90c1da: *stop=0xc90c1dau;if(!valid_ram(r[4]+3u,1u))return -3;r[0]=sx(m,r[4]+0x3u,1);goto Lc90c1dc; /* mov.b @(0x3,r4),r0 */
Lc90c1dc: *stop=0xc90c1dcu;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto Lc90c1de; /* mov.b r0,@(0x3,r2) */
Lc90c1de: *stop=0xc90c1deu;r[0]=84u;goto Lc90c1e0; /* mov #0x54,r0 */
Lc90c1e0: *stop=0xc90c1e0u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c1e2; /* mov.l @(r0,r15),r2 */
Lc90c1e2: *stop=0xc90c1e2u;r[2]+=4u;goto Lc90c1e4; /* add #0x4,r2 */
Lc90c1e4: *stop=0xc90c1e4u;if(!valid_ram(r[4]+3u,1u))return -3;r[0]=sx(m,r[4]+0x3u,1);goto Lc90c1e6; /* mov.b @(0x3,r4),r0 */
Lc90c1e6: *stop=0xc90c1e6u;r[5]=4294967295u;goto Lc90c1e8; /* mov #-0x1,r5 */
Lc90c1e8: *stop=0xc90c1e8u;r[5]=r[5]&255u;goto Lc90c1ea; /* extu.b r5,r5 */
Lc90c1ea: *stop=0xc90c1eau;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto Lc90c1ec; /* mov.b r0,@(0x3,r2) */
Lc90c1ec: *stop=0xc90c1ecu;r[0]=84u;goto Lc90c1ee; /* mov #0x54,r0 */
Lc90c1ee: *stop=0xc90c1eeu;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c1f0; /* mov.l @(r0,r15),r2 */
Lc90c1f0: *stop=0xc90c1f0u;if(!valid_ram(r[4]+3u,1u))return -3;r[0]=sx(m,r[4]+0x3u,1);goto Lc90c1f2; /* mov.b @(0x3,r4),r0 */
Lc90c1f2: *stop=0xc90c1f2u;r[2]+=8u;goto Lc90c1f4; /* add #0x8,r2 */
Lc90c1f4: *stop=0xc90c1f4u;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto Lc90c1f6; /* mov.b r0,@(0x3,r2) */
Lc90c1f6: *stop=0xc90c1f6u;r[0]=84u;goto Lc90c1f8; /* mov #0x54,r0 */
Lc90c1f8: *stop=0xc90c1f8u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c1fa; /* mov.l @(r0,r15),r2 */
Lc90c1fa: *stop=0xc90c1fau;if(!valid_ram(r[4]+3u,1u))return -3;r[0]=sx(m,r[4]+0x3u,1);goto Lc90c1fc; /* mov.b @(0x3,r4),r0 */
Lc90c1fc: *stop=0xc90c1fcu;r[4]=0u;goto Lc90c1fe; /* mov #0x0,r4 */
Lc90c1fe: *stop=0xc90c1feu;r[2]+=12u;goto Lc90c200; /* add #0xc,r2 */
Lc90c200: *stop=0xc90c200u;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto Lc90c202; /* mov.b r0,@(0x3,r2) */
Lc90c202: *stop=0xc90c202u;r[0]=88u;goto Lc90c204; /* mov #0x58,r0 */
Lc90c204: *stop=0xc90c204u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c206; /* mov.l @(r0,r15),r2 */
Lc90c206: *stop=0xc90c206u;r[0]=88u;goto Lc90c208; /* mov #0x58,r0 */
Lc90c208: *stop=0xc90c208u;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[4],2);goto Lc90c20a; /* mov.w r4,@r2 */
Lc90c20a: *stop=0xc90c20au;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c20c; /* mov.l @(r0,r15),r2 */
Lc90c20c: *stop=0xc90c20cu;r[0]=r[5];goto Lc90c20e; /* mov r5,r0 */
Lc90c20e: *stop=0xc90c20eu;if(!valid_ram(r[2]+2u,2u))return -3;wr(m,r[2]+0x2u,r[0],2);goto Lc90c210; /* mov.w r0,@(0x2,r2) */
Lc90c210: *stop=0xc90c210u;r[0]=88u;goto Lc90c212; /* mov #0x58,r0 */
Lc90c212: *stop=0xc90c212u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c214; /* mov.l @(r0,r15),r2 */
Lc90c214: *stop=0xc90c214u;r[0]=88u;goto Lc90c216; /* mov #0x58,r0 */
Lc90c216: *stop=0xc90c216u;r[2]+=4u;goto Lc90c218; /* add #0x4,r2 */
Lc90c218: *stop=0xc90c218u;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[4],2);goto Lc90c21a; /* mov.w r4,@r2 */
Lc90c21a: *stop=0xc90c21au;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c21c; /* mov.l @(r0,r15),r2 */
Lc90c21c: *stop=0xc90c21cu;r[0]=r[4];goto Lc90c21e; /* mov r4,r0 */
Lc90c21e: *stop=0xc90c21eu;r[2]+=4u;goto Lc90c220; /* add #0x4,r2 */
Lc90c220: *stop=0xc90c220u;if(!valid_ram(r[2]+2u,2u))return -3;wr(m,r[2]+0x2u,r[0],2);goto Lc90c222; /* mov.w r0,@(0x2,r2) */
Lc90c222: *stop=0xc90c222u;r[0]=88u;goto Lc90c224; /* mov #0x58,r0 */
Lc90c224: *stop=0xc90c224u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c226; /* mov.l @(r0,r15),r2 */
Lc90c226: *stop=0xc90c226u;r[0]=88u;goto Lc90c228; /* mov #0x58,r0 */
Lc90c228: *stop=0xc90c228u;r[2]+=8u;goto Lc90c22a; /* add #0x8,r2 */
Lc90c22a: *stop=0xc90c22au;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[5],2);goto Lc90c22c; /* mov.w r5,@r2 */
Lc90c22c: *stop=0xc90c22cu;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c22e; /* mov.l @(r0,r15),r2 */
Lc90c22e: *stop=0xc90c22eu;r[0]=r[4];goto Lc90c230; /* mov r4,r0 */
Lc90c230: *stop=0xc90c230u;r[2]+=8u;goto Lc90c232; /* add #0x8,r2 */
Lc90c232: *stop=0xc90c232u;if(!valid_ram(r[2]+2u,2u))return -3;wr(m,r[2]+0x2u,r[0],2);goto Lc90c234; /* mov.w r0,@(0x2,r2) */
Lc90c234: *stop=0xc90c234u;r[0]=88u;goto Lc90c236; /* mov #0x58,r0 */
Lc90c236: *stop=0xc90c236u;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c238; /* mov.l @(r0,r15),r2 */
Lc90c238: *stop=0xc90c238u;r[0]=88u;goto Lc90c23a; /* mov #0x58,r0 */
Lc90c23a: *stop=0xc90c23au;r[2]+=12u;goto Lc90c23c; /* add #0xc,r2 */
Lc90c23c: *stop=0xc90c23cu;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[5],2);goto Lc90c23e; /* mov.w r5,@r2 */
Lc90c23e: *stop=0xc90c23eu;if(!valid_ram(r[0]+r[15],4u))return -3;r[2]=rd(m,r[15]+r[0],4);goto Lc90c240; /* mov.l @(r0,r15),r2 */
Lc90c240: *stop=0xc90c240u;r[0]=r[5];goto Lc90c242; /* mov r5,r0 */
Lc90c242: *stop=0xc90c242u;r[2]+=12u;goto Lc90c244; /* add #0xc,r2 */
Lc90c244: *stop=0xc90c244u;if(!valid_ram(r[2]+2u,2u))return -3;wr(m,r[2]+0x2u,r[0],2);goto Lc90c246; /* mov.w r0,@(0x2,r2) */
Lc90c246: *stop=0xc90c246u;r[2]=rd(m,0x0c90c3c4u,4);goto Lc90c248; /* mov.l 0x0c90c3c4,r2 */
Lc90c248: target=r[2];*pr=0x0c90c24cu;*stop=0xc90c24au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c24c; /* jsr @r2 */
Lc90c24c: *stop=0xc90c24cu;r[3]=rd(m,0x0c90c3ccu,4);goto Lc90c24e; /* mov.l 0x0c90c3cc,r3 */
Lc90c24e: *stop=0xc90c24eu;r[4]=rd(m,0x0c90c3c8u,4);goto Lc90c250; /* mov.l 0x0c90c3c8,r4 */
Lc90c250: target=r[3];*pr=0x0c90c254u;*stop=0xc90c252u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c254; /* jsr @r3 */
Lc90c254: *stop=0xc90c254u;r[2]=rd(m,0x0c90c3d0u,4);goto Lc90c256; /* mov.l 0x0c90c3d0,r2 */
Lc90c256: target=r[2];*pr=0x0c90c25au;*stop=0xc90c258u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c25a; /* jsr @r2 */
Lc90c25a: *stop=0xc90c25au;r[3]=rd(m,0x0c90c3d8u,4);goto Lc90c25c; /* mov.l 0x0c90c3d8,r3 */
Lc90c25c: *stop=0xc90c25cu;r[4]=r[15];goto Lc90c25e; /* mov r15,r4 */
Lc90c25e: *stop=0xc90c25eu;r[6]=rd(m,0x0c90c3d4u,4);goto Lc90c260; /* mov.l 0x0c90c3d4,r6 */
Lc90c260: *stop=0xc90c260u;r[5]=4u;goto Lc90c262; /* mov #0x4,r5 */
Lc90c262: target=r[3];*pr=0x0c90c266u;*stop=0xc90c264u;r[4]+=80u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c266; /* jsr @r3 */
Lc90c266: *stop=0xc90c266u;r[2]=rd(m,0x0c90c3c4u,4);goto Lc90c268; /* mov.l 0x0c90c3c4,r2 */
Lc90c268: target=r[2];*pr=0x0c90c26cu;*stop=0xc90c26au;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90c26c; /* jsr @r2 */
Lc90c26c: *stop=0xc90c26cu;r[15]+=96u;goto Lc90c26e; /* add #0x60,r15 */
Lc90c26e: *stop=0xc90c26eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc90c270; /* lds.l @r15+,PR */
Lc90c270: *stop=0xc90c270u;if(*pr!=entry_pr)return -3;*stop=0xc90c272u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_fade_filter_state_8c605480(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c605480;
L8c605480: *stop=0x8c605480u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c605482; /* mov.l r14,@-r15 */
L8c605482: *stop=0x8c605482u;r[0]=r[4];goto L8c605484; /* mov r4,r0 */
L8c605484: *stop=0x8c605484u;r[14]=rd(m,0x8c6054ecu,4);goto L8c605486; /* mov.l 0x8c6054ec,r14 */
L8c605486: *stop=0x8c605486u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c605488; /* cmp/eq #0x0,r0 */
L8c605488: *stop=0x8c605488u;r[2]=sx(m,0x8c6054e8u,2);goto L8c60548a; /* mov.w 0x8c6054e8,r2 */
L8c60548a: *stop=0x8c60548au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c60548c; /* sts.l PR,@-r15 */
L8c60548c: *stop=0x8c60548cu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c60548e; /* mov.l @r14,r3 */
L8c60548e: *stop=0x8c60548eu;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);goto L8c605490; /* mov.l r2,@r3 */
L8c605490: take=(*sr&1u);*stop=0x8c605492u;if(!valid_ram(r[14],4u))return -3;r[5]=rd(m,r[14],4);if(take)goto L8c6054a0;goto L8c605494; /* bt/s 0x8c6054a0 */
L8c605494: *stop=0x8c605494u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c605496; /* cmp/eq #0x1,r0 */
L8c605496: if((*sr&1u))goto L8c6054a8;goto L8c605498; /* bt 0x8c6054a8 */
L8c605498: *stop=0x8c605498u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c60549a; /* cmp/eq #0x2,r0 */
L8c60549a: if((*sr&1u))goto L8c6054b0;goto L8c60549c; /* bt 0x8c6054b0 */
L8c60549c: *stop=0x8c60549eu;goto L8c6054b6; /* bra 0x8c6054b6 */
L8c6054a0: *stop=0x8c6054a0u;r[0]=84u;goto L8c6054a2; /* mov #0x54,r0 */
L8c6054a2: *stop=0x8c6054a2u;r[3]=0u;goto L8c6054a4; /* mov #0x0,r3 */
L8c6054a4: *stop=0x8c6054a6u;if(!valid_ram(r[0]+r[5],4u))return -3;wr(m,r[5]+r[0],r[3],4);goto L8c6054b6; /* bra 0x8c6054b6 */
L8c6054a8: *stop=0x8c6054a8u;r[0]=84u;goto L8c6054aa; /* mov #0x54,r0 */
L8c6054aa: *stop=0x8c6054aau;r[1]=2u;goto L8c6054ac; /* mov #0x2,r1 */
L8c6054ac: *stop=0x8c6054aeu;if(!valid_ram(r[0]+r[5],4u))return -3;wr(m,r[5]+r[0],r[1],4);goto L8c6054b6; /* bra 0x8c6054b6 */
L8c6054b0: *stop=0x8c6054b0u;r[0]=84u;goto L8c6054b2; /* mov #0x54,r0 */
L8c6054b2: *stop=0x8c6054b2u;r[2]=1u;goto L8c6054b4; /* mov #0x1,r2 */
L8c6054b4: *stop=0x8c6054b4u;if(!valid_ram(r[0]+r[5],4u))return -3;wr(m,r[5]+r[0],r[2],4);goto L8c6054b6; /* mov.l r2,@(r0,r5) */
L8c6054b6: *stop=0x8c6054b6u;r[2]=rd(m,0x8c6054f4u,4);goto L8c6054b8; /* mov.l 0x8c6054f4,r2 */
L8c6054b8: target=r[2];*pr=0x8c6054bcu;*stop=0x8c6054bau;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6054bc; /* jsr @r2 */
L8c6054bc: *stop=0x8c6054bcu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c6054be; /* lds.l @r15+,PR */
L8c6054be: *stop=0x8c6054beu;r[3]=rd(m,0x8c6054f0u,4);goto L8c6054c0; /* mov.l 0x8c6054f0,r3 */
L8c6054c0: *stop=0x8c6054c0u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);goto L8c6054c2; /* mov.l @r14,r4 */
L8c6054c2: target=r[3];*stop=0x8c6054c4u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_overlay_fade_vector_translate_8c63a804(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c63a804;
L8c63a804: if(f->fpscr&0x180000u)return -1;*stop=0x8c63a804u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a806; /* fmov.s @r5+,fr4 */
L8c63a806: *stop=0x8c63a806u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c63a808; /* tst r4,r4 */
L8c63a808: *stop=0x8c63a808u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a80a; /* fmov.s @r5+,fr5 */
L8c63a80a: *stop=0x8c63a80au;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a80c; /* fmov.s @r5+,fr6 */
L8c63a80c: take=(*sr&1u);*stop=0x8c63a80eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;if(take)goto L8c63a868;goto L8c63a810; /* bt/s 0x8c63a868 */
L8c63a810: *stop=0x8c63a810u;r[0]=16u;goto L8c63a812; /* mov #0x10,r0 */
L8c63a812: *stop=0x8c63a812u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[4]);goto L8c63a814; /* fmov.s @r4,fr0 */
L8c63a814: *stop=0x8c63a814u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto L8c63a816; /* fmov.s @(r0,r4),fr1 */
L8c63a816: *stop=0x8c63a816u;r[0]=32u;goto L8c63a818; /* mov #0x20,r0 */
L8c63a818: *stop=0x8c63a818u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c63a81a; /* fmov.s @(r0,r4),fr2 */
L8c63a81a: *stop=0x8c63a81au;r[0]=48u;goto L8c63a81c; /* mov #0x30,r0 */
L8c63a81c: *stop=0x8c63a81cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c63a81e; /* fmov.s @(r0,r4),fr3 */
L8c63a81e: *stop=0x8c63a81eu;status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c63a820; /* fipr fv4,fv0 */
L8c63a820: *stop=0x8c63a820u;r[0]=48u;goto L8c63a822; /* mov #0x30,r0 */
L8c63a822: *stop=0x8c63a822u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c63a824; /* fmov fr3,@(r0,r4) */
L8c63a824: *stop=0x8c63a824u;r[0]=4u;goto L8c63a826; /* mov #0x4,r0 */
L8c63a826: *stop=0x8c63a826u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto L8c63a828; /* fmov.s @(r0,r4),fr0 */
L8c63a828: *stop=0x8c63a828u;r[0]=20u;goto L8c63a82a; /* mov #0x14,r0 */
L8c63a82a: *stop=0x8c63a82au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto L8c63a82c; /* fmov.s @(r0,r4),fr1 */
L8c63a82c: *stop=0x8c63a82cu;r[0]=36u;goto L8c63a82e; /* mov #0x24,r0 */
L8c63a82e: *stop=0x8c63a82eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c63a830; /* fmov.s @(r0,r4),fr2 */
L8c63a830: *stop=0x8c63a830u;r[0]=52u;goto L8c63a832; /* mov #0x34,r0 */
L8c63a832: *stop=0x8c63a832u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c63a834; /* fmov.s @(r0,r4),fr3 */
L8c63a834: *stop=0x8c63a834u;status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c63a836; /* fipr fv4,fv0 */
L8c63a836: *stop=0x8c63a836u;r[0]=52u;goto L8c63a838; /* mov #0x34,r0 */
L8c63a838: *stop=0x8c63a838u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c63a83a; /* fmov fr3,@(r0,r4) */
L8c63a83a: *stop=0x8c63a83au;r[0]=8u;goto L8c63a83c; /* mov #0x8,r0 */
L8c63a83c: *stop=0x8c63a83cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto L8c63a83e; /* fmov.s @(r0,r4),fr0 */
L8c63a83e: *stop=0x8c63a83eu;r[0]=24u;goto L8c63a840; /* mov #0x18,r0 */
L8c63a840: *stop=0x8c63a840u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto L8c63a842; /* fmov.s @(r0,r4),fr1 */
L8c63a842: *stop=0x8c63a842u;r[0]=40u;goto L8c63a844; /* mov #0x28,r0 */
L8c63a844: *stop=0x8c63a844u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c63a846; /* fmov.s @(r0,r4),fr2 */
L8c63a846: *stop=0x8c63a846u;r[0]=56u;goto L8c63a848; /* mov #0x38,r0 */
L8c63a848: *stop=0x8c63a848u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c63a84a; /* fmov.s @(r0,r4),fr3 */
L8c63a84a: *stop=0x8c63a84au;status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c63a84c; /* fipr fv4,fv0 */
L8c63a84c: *stop=0x8c63a84cu;r[0]=56u;goto L8c63a84e; /* mov #0x38,r0 */
L8c63a84e: *stop=0x8c63a84eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c63a850; /* fmov fr3,@(r0,r4) */
L8c63a850: *stop=0x8c63a850u;r[0]=12u;goto L8c63a852; /* mov #0xc,r0 */
L8c63a852: *stop=0x8c63a852u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto L8c63a854; /* fmov.s @(r0,r4),fr0 */
L8c63a854: *stop=0x8c63a854u;r[0]=28u;goto L8c63a856; /* mov #0x1c,r0 */
L8c63a856: *stop=0x8c63a856u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto L8c63a858; /* fmov.s @(r0,r4),fr1 */
L8c63a858: *stop=0x8c63a858u;r[0]=44u;goto L8c63a85a; /* mov #0x2c,r0 */
L8c63a85a: *stop=0x8c63a85au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c63a85c; /* fmov.s @(r0,r4),fr2 */
L8c63a85c: *stop=0x8c63a85cu;r[0]=60u;goto L8c63a85e; /* mov #0x3c,r0 */
L8c63a85e: *stop=0x8c63a85eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c63a860; /* fmov.s @(r0,r4),fr3 */
L8c63a860: *stop=0x8c63a860u;status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c63a862; /* fipr fv4,fv0 */
L8c63a862: *stop=0x8c63a862u;r[0]=60u;goto L8c63a864; /* mov #0x3c,r0 */
L8c63a864: *stop=0x8c63a864u;if(*pr!=entry_pr)return -3;*stop=0x8c63a866u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);return 0; /* rts */
L8c63a868: *stop=0x8c63a868u;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c63a86a; /* ftrv xmtrx,fv4 */
L8c63a86a: *stop=0x8c63a86au;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a86c; /* fschg */
L8c63a86c: *stop=0x8c63a86cu;fcopy(f,13u,4u);goto L8c63a86e; /* fmov fr4,fr13 */
L8c63a86e: *stop=0x8c63a86eu;fcopy(f,15u,6u);goto L8c63a870; /* fmov fr6,fr15 */
L8c63a870: *stop=0x8c63a870u;if(*pr!=entry_pr)return -3;*stop=0x8c63a872u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;return 0; /* rts */
}
