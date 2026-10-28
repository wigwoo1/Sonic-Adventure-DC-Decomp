/* Original movement contact projection and complete plane math closure; canonical angle difference reused.
 * Exact original instruction order and existing numerical/bus ABI.
 * No upstream implementation code imported.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_integer_state.h"
#include "sh4_vector_base.h"
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
static uint32_t rd(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,unsigned n){
 if((a&0x1f000000u)!=0x0c000000u){uint32_t value=0;if(!bus||!bus->read){*io_status=-2;return 0;}*io_status=bus->read(bus->context,a,n,&value);return value;}
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,unsigned n){
 uint32_t value=rd(m,bus,io_status,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,uint32_t value,unsigned n){
 if((a&0x1f000000u)!=0x0c000000u){if(!bus||!bus->write){*io_status=-2;return;}*io_status=bus->write(bus->context,a,value,n);return;}
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,bus,io_status,a,4);f->bank[bank][reg+1]=rd(m,bus,io_status,a+4u,4);
 }else f->bank[bank][reg]=rd(m,bus,io_status,a,4);
}
static void fstore(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,bus,io_status,a,f->bank[bank][reg],4);wr(m,bus,io_status,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,bus,io_status,a,f->bank[bank][reg],4);
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
static int valid_access(uint32_t a,unsigned n){
 if(valid_ram(a,n))return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
int sa_sh4_sqrt(uint32_t,uint32_t*,uint32_t*);
int sa_movie_channel_io_return_allowed(void *,uint32_t);
static int sa_movement_projection_response_run_8c075a40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c075ae4u)goto L8c075ae4;if(resume_pc==0x8c075b02u)goto L8c075b02;if(resume_pc==0x8c075b16u)goto L8c075b16;if(resume_pc==0x8c075b22u)goto L8c075b22;if(resume_pc==0x8c075b58u)goto L8c075b58;if(resume_pc==0x8c075b66u)goto L8c075b66;if(resume_pc==0x8c075b72u)goto L8c075b72;if(resume_pc==0x8c075b86u)goto L8c075b86;if(resume_pc==0x8c075b90u)goto L8c075b90;if(resume_pc==0x8c075bd2u)goto L8c075bd2;if(resume_pc==0x8c075bf2u)goto L8c075bf2;if(resume_pc==0x8c075c18u)goto L8c075c18;if(resume_pc==0x8c075c88u)goto L8c075c88;if(resume_pc==0x8c075cf2u)goto L8c075cf2;if(resume_pc==0x8c075d06u)goto L8c075d06;if(resume_pc==0x8c075d16u)goto L8c075d16;if(resume_pc==0x8c075d60u)goto L8c075d60;if(resume_pc==0x8c075d8eu)goto L8c075d8e;if(resume_pc==0x8c075d96u)goto L8c075d96;if(resume_pc==0x8c075deau)goto L8c075dea;if(resume_pc==0x8c075e7eu)goto L8c075e7e;if(resume_pc==0x8c075e98u)goto L8c075e98;if(resume_pc==0x8c075ea6u)goto L8c075ea6;if(resume_pc==0x8c075eb4u)goto L8c075eb4;if(resume_pc==0x8c075eeau)goto L8c075eea;if(resume_pc==0x8c075f38u)goto L8c075f38;if(resume_pc==0x8c075f54u)goto L8c075f54;goto L8c075a40;
L8c075a40: *stop=0x8c075a40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c075a42; /* mov.l r14,@-r15 */
L8c075a42: *stop=0x8c075a42u;r[0]=4294967295u;goto L8c075a44; /* mov #-0x1,r0 */
L8c075a44: *stop=0x8c075a44u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c075a46; /* mov.l r13,@-r15 */
L8c075a46: *stop=0x8c075a46u;r[0]<<=8;goto L8c075a48; /* shll8 r0 */
L8c075a48: *stop=0x8c075a48u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c075a4a; /* mov.l r12,@-r15 */
L8c075a4a: *stop=0x8c075a4au;r[0]+=4294967272u;goto L8c075a4c; /* add #-0x18,r0 */
L8c075a4c: *stop=0x8c075a4cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c075a4e; /* mov.l r11,@-r15 */
L8c075a4e: *stop=0x8c075a4eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c075a50; /* mov.l r10,@-r15 */
L8c075a50: *stop=0x8c075a50u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c075a52; /* mov.l r9,@-r15 */
L8c075a52: *stop=0x8c075a52u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c075a54; /* mov.l r8,@-r15 */
L8c075a54: *stop=0x8c075a54u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c075a56; /* fmov.s fr15,@-r15 */
L8c075a56: *stop=0x8c075a56u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c075a58; /* fmov.s fr14,@-r15 */
L8c075a58: *stop=0x8c075a58u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c075a5a; /* fmov.s fr13,@-r15 */
L8c075a5a: *stop=0x8c075a5au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c075a5c; /* fmov.s fr12,@-r15 */
L8c075a5c: *stop=0x8c075a5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c075a5e; /* sts.l PR,@-r15 */
L8c075a5e: *stop=0x8c075a5eu;r[15]+=r[0];goto L8c075a60; /* add r0,r15 */
L8c075a60: *stop=0x8c075a60u;r[0]=0x8c075c40u;goto L8c075a62; /* mova 0x8c075c40,r0 */
L8c075a62: *stop=0x8c075a62u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0;goto L8c075a64; /* fldi0 fr12 */
L8c075a64: *stop=0x8c075a64u;r[3]=0u;goto L8c075a66; /* mov #0x0,r3 */
L8c075a66: *stop=0x8c075a66u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c075a68; /* mov.l r4,@(0x4,r15) */
L8c075a68: *stop=0x8c075a68u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c075a6a; /* mov.l r3,@(0x8,r15) */
L8c075a6a: *stop=0x8c075a6au;r[3]=4294967212u;goto L8c075a6c; /* mov #-0x54,r3 */
L8c075a6c: *stop=0x8c075a6cu;r[2]=4294967295u;goto L8c075a6e; /* mov #-0x1,r2 */
L8c075a6e: *stop=0x8c075a6eu;r[14]=r[7];goto L8c075a70; /* mov r7,r14 */
L8c075a70: *stop=0x8c075a70u;r[3]=r[3]&255u;goto L8c075a72; /* extu.b r3,r3 */
L8c075a72: *stop=0x8c075a72u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c075a74; /* mov.l r2,@(0x10,r15) */
L8c075a74: *stop=0x8c075a74u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c075a76; /* fmov.s @r0,fr15 */
L8c075a76: *stop=0x8c075a76u;r[0]=4294967220u;goto L8c075a78; /* mov #-0x4c,r0 */
L8c075a78: *stop=0x8c075a78u;r[0]=r[0]&255u;goto L8c075a7a; /* extu.b r0,r0 */
L8c075a7a: *stop=0x8c075a7au;r[12]=r[5];goto L8c075a7c; /* mov r5,r12 */
L8c075a7c: *stop=0x8c075a7cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[15]);if(io_status)return io_status;goto L8c075a7e; /* fmov fr12,@(r0,r15) */
L8c075a7e: *stop=0x8c075a7eu;r[0]=4294967216u;goto L8c075a80; /* mov #-0x50,r0 */
L8c075a80: *stop=0x8c075a80u;r[0]=r[0]&255u;goto L8c075a82; /* extu.b r0,r0 */
L8c075a82: *stop=0x8c075a82u;r[10]=r[6];goto L8c075a84; /* mov r6,r10 */
L8c075a84: *stop=0x8c075a84u;r[3]+=r[15];goto L8c075a86; /* add r15,r3 */
L8c075a86: *stop=0x8c075a86u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[15]);if(io_status)return io_status;goto L8c075a88; /* fmov fr12,@(r0,r15) */
L8c075a88: *stop=0x8c075a88u;r[2]=4294967200u;goto L8c075a8a; /* mov #-0x60,r2 */
L8c075a8a: *stop=0x8c075a8au;r[2]=r[2]&255u;goto L8c075a8c; /* extu.b r2,r2 */
L8c075a8c: *stop=0x8c075a8cu;r[0]=4294967212u;goto L8c075a8e; /* mov #-0x54,r0 */
L8c075a8e: *stop=0x8c075a8eu;r[0]=r[0]&255u;goto L8c075a90; /* extu.b r0,r0 */
L8c075a90: *stop=0x8c075a90u;r[2]+=r[15];goto L8c075a92; /* add r15,r2 */
L8c075a92: *stop=0x8c075a92u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[15]);if(io_status)return io_status;goto L8c075a94; /* fmov fr12,@(r0,r15) */
L8c075a94: *stop=0x8c075a94u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c075a96; /* mov.l @r3,r1 */
L8c075a96: *stop=0x8c075a96u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c075a98; /* mov.l @(0x4,r3),r0 */
L8c075a98: *stop=0x8c075a98u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c075a9a; /* mov.l r1,@r2 */
L8c075a9a: *stop=0x8c075a9au;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c075a9c; /* mov.l r0,@(0x4,r2) */
L8c075a9c: *stop=0x8c075a9cu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075a9e; /* mov.l @(0x8,r3),r1 */
L8c075a9e: *stop=0x8c075a9eu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c075aa0; /* mov.l r1,@(0x8,r2) */
L8c075aa0: *stop=0x8c075aa0u;r[0]=82u;goto L8c075aa2; /* mov #0x52,r0 */
L8c075aa2: *stop=0x8c075aa2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c44u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c075aa4; /* mov.l 0x8c075c44,r8 */
L8c075aa4: *stop=0x8c075aa4u;r[0]<<=2;goto L8c075aa6; /* shll2 r0 */
L8c075aa6: *stop=0x8c075aa6u;r[9]=r[15];goto L8c075aa8; /* mov r15,r9 */
L8c075aa8: *stop=0x8c075aa8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c075aaa; /* mov.l @(r0,r15),r2 */
L8c075aaa: *stop=0x8c075aaau;r[3]=64u;goto L8c075aac; /* mov #0x40,r3 */
L8c075aac: *stop=0x8c075aacu;r[3]<<=8;goto L8c075aae; /* shll8 r3 */
L8c075aae: *stop=0x8c075aaeu;r[2]&=r[3];goto L8c075ab0; /* and r3,r2 */
L8c075ab0: *stop=0x8c075ab0u;r[0]=40u;goto L8c075ab2; /* mov #0x28,r0 */
L8c075ab2: *stop=0x8c075ab2u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c075ab4; /* mov.l r2,@(0xc,r15) */
L8c075ab4: *stop=0x8c075ab4u;if(!valid_access(r[0]+r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c075ab6; /* mov.w @(r0,r14),r2 */
L8c075ab6: *stop=0x8c075ab6u;r[13]=0u;goto L8c075ab8; /* mov #0x0,r13 */
L8c075ab8: *stop=0x8c075ab8u;r[2]=r[2]&65535u;goto L8c075aba; /* extu.w r2,r2 */
L8c075aba: *stop=0x8c075abau;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c075abc; /* cmp/pl r2 */
L8c075abc: take=(*sr&1u);*stop=0x8c075abeu;r[9]+=96u;if(take)goto L8c075ac4;goto L8c075ac0; /* bt/s 0x8c075ac4 */
L8c075ac0: *stop=0x8c075ac2u;goto L8c075be2; /* bra 0x8c075be2 */
L8c075ac4: *stop=0x8c075ac4u;r[11]=88u;goto L8c075ac6; /* mov #0x58,r11 */
L8c075ac6: *stop=0x8c075ac6u;r[2]=r[13];goto L8c075ac8; /* mov r13,r2 */
L8c075ac8: *stop=0x8c075ac8u;*macl=r[11]*r[13];goto L8c075aca; /* mul.l r11,r13 */
L8c075aca: *stop=0x8c075acau;r[2]<<=2;goto L8c075acc; /* shll2 r2 */
L8c075acc: *stop=0x8c075accu;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075ace; /* mov.l 0x8c075c48,r3 */
L8c075ace: *stop=0x8c075aceu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c075ad0; /* mov.l r2,@r15 */
L8c075ad0: *stop=0x8c075ad0u;r[2]+=r[9];goto L8c075ad2; /* add r9,r2 */
L8c075ad2: *stop=0x8c075ad2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c075ad4; /* mov.l r2,@-r15 */
L8c075ad4: *stop=0x8c075ad4u;r[5]=r[14];goto L8c075ad6; /* mov r14,r5 */
L8c075ad6: *stop=0x8c075ad6u;r[11]=*macl;goto L8c075ad8; /* sts MACL,r11 */
L8c075ad8: *stop=0x8c075ad8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c4cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075ada; /* mov.l 0x8c075c4c,r2 */
L8c075ada: *stop=0x8c075adau;r[5]+=4u;goto L8c075adc; /* add #0x4,r5 */
L8c075adc: *stop=0x8c075adcu;r[11]+=r[3];goto L8c075ade; /* add r3,r11 */
L8c075ade: *stop=0x8c075adeu;r[4]=r[11];goto L8c075ae0; /* mov r11,r4 */
L8c075ae0: target=r[2];*pr=0x8c075ae4u;*stop=0x8c075ae2u;r[4]+=44u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075ae4; /* jsr @r2 */
L8c075ae4: *stop=0x8c075ae4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c075ae6; /* mov.l @r15+,r3 */
L8c075ae6: *stop=0x8c075ae6u;r[1]=0u;goto L8c075ae8; /* mov #0x0,r1 */
L8c075ae8: *stop=0x8c075ae8u;r[5]=r[11];goto L8c075aea; /* mov r11,r5 */
L8c075aea: *stop=0x8c075aeau;r[6]=r[1];goto L8c075aec; /* mov r1,r6 */
L8c075aec: *stop=0x8c075aecu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[3]);if(io_status)return io_status;goto L8c075aee; /* fmov.s fr0,@r3 */
L8c075aee: *stop=0x8c075aeeu;r[3]=r[15];goto L8c075af0; /* mov r15,r3 */
L8c075af0: *stop=0x8c075af0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c075af2; /* mov.l @r15,r2 */
L8c075af2: *stop=0x8c075af2u;r[3]+=32u;goto L8c075af4; /* add #0x20,r3 */
L8c075af4: *stop=0x8c075af4u;r[4]=r[14];goto L8c075af6; /* mov r14,r4 */
L8c075af6: *stop=0x8c075af6u;r[5]+=20u;goto L8c075af8; /* add #0x14,r5 */
L8c075af8: *stop=0x8c075af8u;r[2]+=r[3];goto L8c075afa; /* add r3,r2 */
L8c075afa: *stop=0x8c075afau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c075afc; /* mov.l r1,@r2 */
L8c075afc: *stop=0x8c075afcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c50u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075afe; /* mov.l 0x8c075c50,r2 */
L8c075afe: target=r[2];*pr=0x8c075b02u;*stop=0x8c075b00u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075b02; /* jsr @r2 */
L8c075b02: *stop=0x8c075b02u;r[3]=44u;goto L8c075b04; /* mov #0x2c,r3 */
L8c075b04: *stop=0x8c075b04u;r[3]<<=8;goto L8c075b06; /* shll8 r3 */
L8c075b06: *stop=0x8c075b06u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[3]);goto L8c075b08; /* cmp/gt r3,r0 */
L8c075b08: take=!(*sr&1u);*stop=0x8c075b0au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;if(take)goto L8c075b2e;goto L8c075b0c; /* bf/s 0x8c075b2e */
L8c075b0c: *stop=0x8c075b0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075b0e; /* mov.l 0x8c075c58,r3 */
L8c075b0e: *stop=0x8c075b0eu;r[5]=r[14];goto L8c075b10; /* mov r14,r5 */
L8c075b10: *stop=0x8c075b10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c54u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c075b12; /* mov.l 0x8c075c54,r4 */
L8c075b12: target=r[3];*pr=0x8c075b16u;*stop=0x8c075b14u;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075b16; /* jsr @r3 */
L8c075b16: *stop=0x8c075b16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c54u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c075b18; /* mov.l 0x8c075c54,r4 */
L8c075b18: *stop=0x8c075b18u;r[5]=r[11];goto L8c075b1a; /* mov r11,r5 */
L8c075b1a: *stop=0x8c075b1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075b1c; /* mov.l 0x8c075c58,r2 */
L8c075b1c: *stop=0x8c075b1cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c075b1e; /* fmov.s fr0,@-r15 */
L8c075b1e: target=r[2];*pr=0x8c075b22u;*stop=0x8c075b20u;r[5]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075b22; /* jsr @r2 */
L8c075b22: *stop=0x8c075b22u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075b24; /* fmov.s @r15+,fr3 */
L8c075b24: *stop=0x8c075b24u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c075b26; /* fcmp/gt fr0,fr3 */
L8c075b26: if((*sr&1u))goto L8c075b2e;goto L8c075b28; /* bt 0x8c075b2e */
L8c075b28: *stop=0x8c075b28u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075b2a; /* mov.l @(0xc,r15),r2 */
L8c075b2a: *stop=0x8c075b2au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c075b2c; /* tst r2,r2 */
L8c075b2c: if((*sr&1u))goto L8c075b42;goto L8c075b2e; /* bt 0x8c075b42 */
L8c075b2e: *stop=0x8c075b2eu;r[2]=r[15];goto L8c075b30; /* mov r15,r2 */
L8c075b30: *stop=0x8c075b30u;r[3]=r[13];goto L8c075b32; /* mov r13,r3 */
L8c075b32: *stop=0x8c075b32u;r[2]+=32u;goto L8c075b34; /* add #0x20,r2 */
L8c075b34: *stop=0x8c075b34u;r[3]<<=2;goto L8c075b36; /* shll2 r3 */
L8c075b36: *stop=0x8c075b36u;r[3]+=r[2];goto L8c075b38; /* add r2,r3 */
L8c075b38: *stop=0x8c075b38u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c075b3a; /* mov.l @r3,r0 */
L8c075b3a: *stop=0x8c075b3au;r[0]|=0x2u;goto L8c075b3c; /* or #0x2,r0 */
L8c075b3c: *stop=0x8c075b3cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c075b3e; /* mov.l r0,@r3 */
L8c075b3e: *stop=0x8c075b40u;goto L8c075bd2; /* bra 0x8c075bd2 */
L8c075b42: *stop=0x8c075b42u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075b44; /* mov.l @(0x10,r14),r3 */
L8c075b44: *stop=0x8c075b44u;r[2]=r[15];goto L8c075b46; /* mov r15,r2 */
L8c075b46: *stop=0x8c075b46u;r[2]+=20u;goto L8c075b48; /* add #0x14,r2 */
L8c075b48: *stop=0x8c075b48u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075b4a; /* mov.l @(0x14,r14),r1 */
L8c075b4a: *stop=0x8c075b4au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c075b4c; /* mov.l r3,@r2 */
L8c075b4c: *stop=0x8c075b4cu;r[4]=r[15];goto L8c075b4e; /* mov r15,r4 */
L8c075b4e: *stop=0x8c075b4eu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c075b50; /* mov.l r1,@(0x4,r2) */
L8c075b50: *stop=0x8c075b50u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075b52; /* mov.l @(0x18,r14),r3 */
L8c075b52: *stop=0x8c075b52u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c075b54; /* mov.l r3,@(0x8,r2) */
L8c075b54: target=r[8];*pr=0x8c075b58u;*stop=0x8c075b56u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075b58; /* jsr @r8 */
L8c075b58: *stop=0x8c075b58u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075b5a; /* fldi0 fr3 */
L8c075b5a: *stop=0x8c075b5au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c075b5c; /* fcmp/eq fr3,fr0 */
L8c075b5c: if((*sr&1u))goto L8c075b66;goto L8c075b5e; /* bt 0x8c075b66 */
L8c075b5e: *stop=0x8c075b5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c5cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075b60; /* mov.l 0x8c075c5c,r2 */
L8c075b60: *stop=0x8c075b60u;r[4]=r[15];goto L8c075b62; /* mov r15,r4 */
L8c075b62: target=r[2];*pr=0x8c075b66u;*stop=0x8c075b64u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075b66; /* jsr @r2 */
L8c075b66: *stop=0x8c075b66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075b68; /* mov.l 0x8c075c58,r3 */
L8c075b68: *stop=0x8c075b68u;r[5]=r[11];goto L8c075b6a; /* mov r11,r5 */
L8c075b6a: *stop=0x8c075b6au;r[4]=r[15];goto L8c075b6c; /* mov r15,r4 */
L8c075b6c: *stop=0x8c075b6cu;r[5]+=20u;goto L8c075b6e; /* add #0x14,r5 */
L8c075b6e: target=r[3];*pr=0x8c075b72u;*stop=0x8c075b70u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075b72; /* jsr @r3 */
L8c075b72: *stop=0x8c075b72u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075b74; /* fldi0 fr3 */
L8c075b74: *stop=0x8c075b74u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c075b76; /* fcmp/gt fr0,fr3 */
L8c075b76: take=!(*sr&1u);*stop=0x8c075b78u;fcopy(f,14u,0u);if(take)goto L8c075bd2;goto L8c075b7a; /* bf/s 0x8c075bd2 */
L8c075b7a: *stop=0x8c075b7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075b7c; /* mov.l 0x8c075c58,r2 */
L8c075b7c: *stop=0x8c075b7cu;r[5]=r[11];goto L8c075b7e; /* mov r11,r5 */
L8c075b7e: *stop=0x8c075b7eu;r[4]=r[14];goto L8c075b80; /* mov r14,r4 */
L8c075b80: *stop=0x8c075b80u;r[5]+=20u;goto L8c075b82; /* add #0x14,r5 */
L8c075b82: target=r[2];*pr=0x8c075b86u;*stop=0x8c075b84u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075b86; /* jsr @r2 */
L8c075b86: *stop=0x8c075b86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075b88; /* mov.l 0x8c075c60,r3 */
L8c075b88: *stop=0x8c075b88u;r[4]=6u;goto L8c075b8a; /* mov #0x6,r4 */
L8c075b8a: *stop=0x8c075b8au;r[4]<<=8;goto L8c075b8c; /* shll8 r4 */
L8c075b8c: target=r[3];*pr=0x8c075b90u;*stop=0x8c075b8eu;fcopy(f,13u,0u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075b90; /* jsr @r3 */
L8c075b90: *stop=0x8c075b90u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto L8c075b92; /* fneg fr0 */
L8c075b92: *stop=0x8c075b92u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,1);if(status)return status;goto L8c075b94; /* fcmp/gt fr14,fr0 */
L8c075b94: if((*sr&1u))goto L8c075b9e;goto L8c075b96; /* bt 0x8c075b9e */
L8c075b96: *stop=0x8c075b96u;r[0]=0x8c075c64u;goto L8c075b98; /* mova 0x8c075c64,r0 */
L8c075b98: *stop=0x8c075b98u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c075b9a; /* fmov.s @r0,fr3 */
L8c075b9a: *stop=0x8c075b9au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][13],&f->fpscr,sr,1);if(status)return status;goto L8c075b9c; /* fcmp/gt fr13,fr3 */
L8c075b9c: if(!(*sr&1u))goto L8c075bc4;goto L8c075b9e; /* bf 0x8c075bc4 */
L8c075b9e: *stop=0x8c075b9eu;r[3]=r[13];goto L8c075ba0; /* mov r13,r3 */
L8c075ba0: *stop=0x8c075ba0u;r[2]=r[15];goto L8c075ba2; /* mov r15,r2 */
L8c075ba2: *stop=0x8c075ba2u;r[3]<<=2;goto L8c075ba4; /* shll2 r3 */
L8c075ba4: *stop=0x8c075ba4u;r[2]+=32u;goto L8c075ba6; /* add #0x20,r2 */
L8c075ba6: *stop=0x8c075ba6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c075ba8; /* mov.l r3,@r15 */
L8c075ba8: *stop=0x8c075ba8u;r[3]+=r[2];goto L8c075baa; /* add r2,r3 */
L8c075baa: *stop=0x8c075baau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c075bac; /* mov.l @r3,r0 */
L8c075bac: *stop=0x8c075bacu;r[0]|=0x8u;goto L8c075bae; /* or #0x8,r0 */
L8c075bae: *stop=0x8c075baeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c075bb0; /* mov.l r0,@r3 */
L8c075bb0: *stop=0x8c075bb0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075bb2; /* mov.l @(0x8,r15),r3 */
L8c075bb2: *stop=0x8c075bb2u;r[3]+=1u;goto L8c075bb4; /* add #0x1,r3 */
L8c075bb4: *stop=0x8c075bb4u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c075bb6; /* mov.l r3,@(0x8,r15) */
L8c075bb6: *stop=0x8c075bb6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c075bb8; /* mov.l @r15,r0 */
L8c075bb8: *stop=0x8c075bb8u;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c075bba; /* fmov.s @(r0,r9),fr3 */
L8c075bba: *stop=0x8c075bbau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c075bbc; /* fcmp/gt fr3,fr15 */
L8c075bbc: if(!(*sr&1u))goto L8c075bc4;goto L8c075bbe; /* bf 0x8c075bc4 */
L8c075bbe: *stop=0x8c075bbeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c075bc0; /* mov.l @r15,r0 */
L8c075bc0: *stop=0x8c075bc0u;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[9]);if(io_status)return io_status;goto L8c075bc2; /* fmov.s @(r0,r9),fr15 */
L8c075bc2: *stop=0x8c075bc2u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[13],4);if(io_status)return io_status;goto L8c075bc4; /* mov.l r13,@(0x10,r15) */
L8c075bc4: *stop=0x8c075bc4u;r[4]=4294967212u;goto L8c075bc6; /* mov #-0x54,r4 */
L8c075bc6: *stop=0x8c075bc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075c68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075bc8; /* mov.l 0x8c075c68,r3 */
L8c075bc8: *stop=0x8c075bc8u;r[5]=r[11];goto L8c075bca; /* mov r11,r5 */
L8c075bca: *stop=0x8c075bcau;r[4]=r[4]&255u;goto L8c075bcc; /* extu.b r4,r4 */
L8c075bcc: *stop=0x8c075bccu;r[5]+=20u;goto L8c075bce; /* add #0x14,r5 */
L8c075bce: target=r[3];*pr=0x8c075bd2u;*stop=0x8c075bd0u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075bd2; /* jsr @r3 */
L8c075bd2: *stop=0x8c075bd2u;r[0]=40u;goto L8c075bd4; /* mov #0x28,r0 */
L8c075bd4: *stop=0x8c075bd4u;if(!valid_access(r[0]+r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c075bd6; /* mov.w @(r0,r14),r2 */
L8c075bd6: *stop=0x8c075bd6u;r[13]+=1u;goto L8c075bd8; /* add #0x1,r13 */
L8c075bd8: *stop=0x8c075bd8u;r[2]=r[2]&65535u;goto L8c075bda; /* extu.w r2,r2 */
L8c075bda: *stop=0x8c075bdau;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[2]);goto L8c075bdc; /* cmp/ge r2,r13 */
L8c075bdc: if((*sr&1u))goto L8c075be2;goto L8c075bde; /* bt 0x8c075be2 */
L8c075bde: *stop=0x8c075be0u;goto L8c075ac4; /* bra 0x8c075ac4 */
L8c075be2: *stop=0x8c075be2u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075be4; /* mov.l @(0x8,r15),r2 */
L8c075be4: *stop=0x8c075be4u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c075be6; /* cmp/pl r2 */
L8c075be6: if((*sr&1u))goto L8c075bec;goto L8c075be8; /* bt 0x8c075bec */
L8c075be8: *stop=0x8c075beau;goto L8c075cf8; /* bra 0x8c075cf8 */
L8c075bec: *stop=0x8c075becu;r[4]=r[14];goto L8c075bee; /* mov r14,r4 */
L8c075bee: target=r[8];*pr=0x8c075bf2u;*stop=0x8c075bf0u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075bf2; /* jsr @r8 */
L8c075bf2: *stop=0x8c075bf2u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c075bf4; /* mov.l @(0x10,r15),r13 */
L8c075bf4: *stop=0x8c075bf4u;fcopy(f,4u,0u);goto L8c075bf6; /* fmov fr0,fr4 */
L8c075bf6: *stop=0x8c075bf6u;r[13]<<=2;goto L8c075bf8; /* shll2 r13 */
L8c075bf8: *stop=0x8c075bf8u;r[0]=r[13];goto L8c075bfa; /* mov r13,r0 */
L8c075bfa: *stop=0x8c075bfau;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c075bfc; /* fmov.s @(r0,r9),fr3 */
L8c075bfc: *stop=0x8c075bfcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c075bfe; /* fcmp/gt fr3,fr4 */
L8c075bfe: if((*sr&1u))goto L8c075c04;goto L8c075c00; /* bt 0x8c075c04 */
L8c075c00: *stop=0x8c075c02u;goto L8c075cf8; /* bra 0x8c075cf8 */
L8c075c04: *stop=0x8c075c04u;r[2]=96u;goto L8c075c06; /* mov #0x60,r2 */
L8c075c06: *stop=0x8c075c06u;r[2]+=r[10];goto L8c075c08; /* add r10,r2 */
L8c075c08: *stop=0x8c075c08u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]=loaded;}goto L8c075c0a; /* mov.l @r2,r2 */
L8c075c0a: *stop=0x8c075c0au;r[3]=64u;goto L8c075c0c; /* mov #0x40,r3 */
L8c075c0c: *stop=0x8c075c0cu;r[3]<<=8;goto L8c075c0e; /* shll8 r3 */
L8c075c0e: *stop=0x8c075c0eu;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c075c10; /* tst r3,r2 */
L8c075c10: if(!(*sr&1u))goto L8c075cf8;goto L8c075c12; /* bf 0x8c075cf8 */
L8c075c12: *stop=0x8c075c12u;r[4]=r[14];goto L8c075c14; /* mov r14,r4 */
L8c075c14: target=r[8];*pr=0x8c075c18u;*stop=0x8c075c16u;r[4]+=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075c18; /* jsr @r8 */
L8c075c18: *stop=0x8c075c18u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075c1a; /* fldi0 fr3 */
L8c075c1a: *stop=0x8c075c1au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c075c1c; /* fcmp/eq fr3,fr0 */
L8c075c1c: if(!(*sr&1u))goto L8c075c80;goto L8c075c1e; /* bf 0x8c075c80 */
L8c075c1e: *stop=0x8c075c1eu;r[0]=116u;goto L8c075c20; /* mov #0x74,r0 */
L8c075c20: *stop=0x8c075c20u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[14]);if(io_status)return io_status;goto L8c075c22; /* fmov fr12,@(r0,r14) */
L8c075c22: *stop=0x8c075c22u;r[0]=112u;goto L8c075c24; /* mov #0x70,r0 */
L8c075c24: *stop=0x8c075c24u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[14]);if(io_status)return io_status;goto L8c075c26; /* fmov fr12,@(r0,r14) */
L8c075c26: *stop=0x8c075c26u;r[0]=108u;goto L8c075c28; /* mov #0x6c,r0 */
L8c075c28: *stop=0x8c075c2au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[14]);if(io_status)return io_status;goto L8c075cf8; /* bra 0x8c075cf8 */
L8c075c80: *stop=0x8c075c80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075dc0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075c82; /* mov.l 0x8c075dc0,r2 */
L8c075c82: *stop=0x8c075c82u;r[4]=r[14];goto L8c075c84; /* mov r14,r4 */
L8c075c84: target=r[2];*pr=0x8c075c88u;*stop=0x8c075c86u;r[4]+=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075c88; /* jsr @r2 */
L8c075c88: *stop=0x8c075c88u;r[4]=r[13];goto L8c075c8a; /* mov r13,r4 */
L8c075c8a: *stop=0x8c075c8au;r[4]+=r[9];goto L8c075c8c; /* add r9,r4 */
L8c075c8c: *stop=0x8c075c8cu;r[0]=108u;goto L8c075c8e; /* mov #0x6c,r0 */
L8c075c8e: *stop=0x8c075c8eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c075c90; /* fmov.s @r4,fr3 */
L8c075c90: *stop=0x8c075c90u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075c92; /* fmov.s @(r0,r14),fr2 */
L8c075c92: *stop=0x8c075c92u;r[3]=88u;goto L8c075c94; /* mov #0x58,r3 */
L8c075c94: *stop=0x8c075c94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075dc4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075c96; /* mov.l 0x8c075dc4,r1 */
L8c075c96: *stop=0x8c075c96u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c075c98; /* fmul fr3,fr2 */
L8c075c98: *stop=0x8c075c98u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075c9a; /* fmov fr2,@(r0,r14) */
L8c075c9a: *stop=0x8c075c9au;r[0]=112u;goto L8c075c9c; /* mov #0x70,r0 */
L8c075c9c: *stop=0x8c075c9cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075c9e; /* fmov.s @(r0,r14),fr2 */
L8c075c9e: *stop=0x8c075c9eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c075ca0; /* fmov.s @r4,fr3 */
L8c075ca0: *stop=0x8c075ca0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c075ca2; /* fmul fr3,fr2 */
L8c075ca2: *stop=0x8c075ca2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075ca4; /* fmov fr2,@(r0,r14) */
L8c075ca4: *stop=0x8c075ca4u;r[0]=116u;goto L8c075ca6; /* mov #0x74,r0 */
L8c075ca6: *stop=0x8c075ca6u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c075ca8; /* fmov.s @r4,fr3 */
L8c075ca8: *stop=0x8c075ca8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075caa; /* fmov.s @(r0,r14),fr2 */
L8c075caa: *stop=0x8c075caau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c075cac; /* fmul fr3,fr2 */
L8c075cac: *stop=0x8c075cacu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075cae; /* fmov fr2,@(r0,r14) */
L8c075cae: *stop=0x8c075caeu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075cb0; /* mov.l @(0x10,r15),r2 */
L8c075cb0: *stop=0x8c075cb0u;*macl=r[3]*r[2];goto L8c075cb2; /* mul.l r3,r2 */
L8c075cb2: *stop=0x8c075cb2u;r[2]=r[10];goto L8c075cb4; /* mov r10,r2 */
L8c075cb4: *stop=0x8c075cb4u;r[2]+=68u;goto L8c075cb6; /* add #0x44,r2 */
L8c075cb6: *stop=0x8c075cb6u;r[13]=*macl;goto L8c075cb8; /* sts MACL,r13 */
L8c075cb8: *stop=0x8c075cb8u;r[13]+=r[1];goto L8c075cba; /* add r1,r13 */
L8c075cba: *stop=0x8c075cbau;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c075cbc; /* mov.l @(0x14,r13),r0 */
L8c075cbc: *stop=0x8c075cbcu;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075cbe; /* mov.l @(0x18,r13),r3 */
L8c075cbe: *stop=0x8c075cbeu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c075cc0; /* mov.l r0,@r2 */
L8c075cc0: *stop=0x8c075cc0u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c075cc2; /* mov.l r3,@(0x4,r2) */
L8c075cc2: *stop=0x8c075cc2u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c075cc4; /* mov.l @(0x1c,r13),r0 */
L8c075cc4: *stop=0x8c075cc4u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[0],4);if(io_status)return io_status;goto L8c075cc6; /* mov.l r0,@(0x8,r2) */
L8c075cc6: *stop=0x8c075cc6u;r[0]=4u;goto L8c075cc8; /* mov #0x4,r0 */
L8c075cc8: *stop=0x8c075cc8u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075cca; /* fmov.s @(r0,r12),fr3 */
L8c075cca: *stop=0x8c075ccau;r[0]=4294967224u;goto L8c075ccc; /* mov #-0x48,r0 */
L8c075ccc: *stop=0x8c075cccu;r[0]=r[0]&255u;goto L8c075cce; /* extu.b r0,r0 */
L8c075cce: *stop=0x8c075cceu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c075cd0; /* fneg fr3 */
L8c075cd0: *stop=0x8c075cd0u;r[4]=4294967224u;goto L8c075cd2; /* mov #-0x48,r4 */
L8c075cd2: *stop=0x8c075cd2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075cd4; /* fmov fr3,@(r0,r15) */
L8c075cd4: *stop=0x8c075cd4u;r[0]=8u;goto L8c075cd6; /* mov #0x8,r0 */
L8c075cd6: *stop=0x8c075cd6u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075cd8; /* fmov.s @(r0,r12),fr3 */
L8c075cd8: *stop=0x8c075cd8u;r[0]=4294967228u;goto L8c075cda; /* mov #-0x44,r0 */
L8c075cda: *stop=0x8c075cdau;r[0]=r[0]&255u;goto L8c075cdc; /* extu.b r0,r0 */
L8c075cdc: *stop=0x8c075cdcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c075cde; /* fneg fr3 */
L8c075cde: *stop=0x8c075cdeu;r[4]=r[4]&255u;goto L8c075ce0; /* extu.b r4,r4 */
L8c075ce0: *stop=0x8c075ce0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075ce2; /* fmov fr3,@(r0,r15) */
L8c075ce2: *stop=0x8c075ce2u;r[0]=12u;goto L8c075ce4; /* mov #0xc,r0 */
L8c075ce4: *stop=0x8c075ce4u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075ce6; /* fmov.s @(r0,r12),fr3 */
L8c075ce6: *stop=0x8c075ce6u;r[0]=4294967232u;goto L8c075ce8; /* mov #-0x40,r0 */
L8c075ce8: *stop=0x8c075ce8u;r[0]=r[0]&255u;goto L8c075cea; /* extu.b r0,r0 */
L8c075cea: *stop=0x8c075ceau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c075cec; /* fneg fr3 */
L8c075cec: *stop=0x8c075cecu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075cee; /* fmov fr3,@(r0,r15) */
L8c075cee: target=r[8];*pr=0x8c075cf2u;*stop=0x8c075cf0u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075cf2; /* jsr @r8 */
L8c075cf2: *stop=0x8c075cf2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075cf4; /* fldi0 fr3 */
L8c075cf4: *stop=0x8c075cf4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c075cf6; /* fcmp/eq fr3,fr0 */
L8c075cf6: if(!(*sr&1u))goto L8c075cfc;goto L8c075cf8; /* bf 0x8c075cfc */
L8c075cf8: *stop=0x8c075cfau;r[0]=0u;goto L8c075f6c; /* bra 0x8c075f6c */
L8c075cfc: *stop=0x8c075cfcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c075dc0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075cfe; /* mov.l 0x8c075dc0,r2 */
L8c075cfe: *stop=0x8c075cfeu;r[4]=4294967224u;goto L8c075d00; /* mov #-0x48,r4 */
L8c075d00: *stop=0x8c075d00u;r[4]=r[4]&255u;goto L8c075d02; /* extu.b r4,r4 */
L8c075d02: target=r[2];*pr=0x8c075d06u;*stop=0x8c075d04u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075d06; /* jsr @r2 */
L8c075d06: *stop=0x8c075d06u;r[4]=4294967224u;goto L8c075d08; /* mov #-0x48,r4 */
L8c075d08: *stop=0x8c075d08u;r[5]=r[10];goto L8c075d0a; /* mov r10,r5 */
L8c075d0a: *stop=0x8c075d0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c075dc8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075d0c; /* mov.l 0x8c075dc8,r3 */
L8c075d0c: *stop=0x8c075d0cu;r[4]=r[4]&255u;goto L8c075d0e; /* extu.b r4,r4 */
L8c075d0e: *stop=0x8c075d0eu;r[6]=0u;goto L8c075d10; /* mov #0x0,r6 */
L8c075d10: *stop=0x8c075d10u;r[5]+=68u;goto L8c075d12; /* add #0x44,r5 */
L8c075d12: target=r[3];*pr=0x8c075d16u;*stop=0x8c075d14u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075d16; /* jsr @r3 */
L8c075d16: *stop=0x8c075d16u;r[2]=16u;goto L8c075d18; /* mov #0x10,r2 */
L8c075d18: *stop=0x8c075d18u;r[4]=r[0];goto L8c075d1a; /* mov r0,r4 */
L8c075d1a: *stop=0x8c075d1au;r[2]<<=8;goto L8c075d1c; /* shll8 r2 */
L8c075d1c: *stop=0x8c075d1cu;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]);goto L8c075d1e; /* cmp/gt r2,r4 */
L8c075d1e: if((*sr&1u))goto L8c075d7c;goto L8c075d20; /* bt 0x8c075d7c */
L8c075d20: *stop=0x8c075d20u;r[0]=4294967244u;goto L8c075d22; /* mov #-0x34,r0 */
L8c075d22: *stop=0x8c075d22u;r[0]=r[0]&255u;goto L8c075d24; /* extu.b r0,r0 */
L8c075d24: *stop=0x8c075d24u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c075d26; /* fmov.s @(r0,r10),fr3 */
L8c075d26: *stop=0x8c075d26u;r[0]=56u;goto L8c075d28; /* mov #0x38,r0 */
L8c075d28: *stop=0x8c075d28u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c075d2a; /* fmov.s @(r0,r10),fr2 */
L8c075d2a: *stop=0x8c075d2au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c075d2c; /* fcmp/gt fr2,fr3 */
L8c075d2c: if((*sr&1u))goto L8c075d3e;goto L8c075d2e; /* bt 0x8c075d3e */
L8c075d2e: *stop=0x8c075d2eu;r[0]=116u;goto L8c075d30; /* mov #0x74,r0 */
L8c075d30: *stop=0x8c075d30u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[14]);if(io_status)return io_status;goto L8c075d32; /* fmov fr12,@(r0,r14) */
L8c075d32: *stop=0x8c075d32u;r[0]=112u;goto L8c075d34; /* mov #0x70,r0 */
L8c075d34: *stop=0x8c075d34u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[14]);if(io_status)return io_status;goto L8c075d36; /* fmov fr12,@(r0,r14) */
L8c075d36: *stop=0x8c075d36u;r[0]=108u;goto L8c075d38; /* mov #0x6c,r0 */
L8c075d38: *stop=0x8c075d38u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[14]);if(io_status)return io_status;goto L8c075d3a; /* fmov fr12,@(r0,r14) */
L8c075d3a: *stop=0x8c075d3cu;r[0]=2u;goto L8c075f6c; /* bra 0x8c075f6c */
L8c075d3e: *stop=0x8c075d3eu;r[0]=4294967232u;goto L8c075d40; /* mov #-0x40,r0 */
L8c075d40: *stop=0x8c075d40u;r[0]=r[0]&255u;goto L8c075d42; /* extu.b r0,r0 */
L8c075d42: *stop=0x8c075d42u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c075d44; /* fmov.s @(r0,r10),fr3 */
L8c075d44: *stop=0x8c075d44u;r[0]=56u;goto L8c075d46; /* mov #0x38,r0 */
L8c075d46: *stop=0x8c075d46u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c075d48; /* fmov.s @(r0,r10),fr2 */
L8c075d48: *stop=0x8c075d48u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c075d4a; /* fcmp/gt fr3,fr2 */
L8c075d4a: if(!(*sr&1u))goto L8c075d78;goto L8c075d4c; /* bf 0x8c075d78 */
L8c075d4c: *stop=0x8c075d4cu;r[0]=0x8c075dccu;goto L8c075d4e; /* mova 0x8c075dcc,r0 */
L8c075d4e: *stop=0x8c075d4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c075dc0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075d50; /* mov.l 0x8c075dc0,r3 */
L8c075d50: *stop=0x8c075d50u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c075d52; /* fmov.s @r0,fr3 */
L8c075d52: *stop=0x8c075d52u;r[0]=4294967232u;goto L8c075d54; /* mov #-0x40,r0 */
L8c075d54: *stop=0x8c075d54u;r[0]=r[0]&255u;goto L8c075d56; /* extu.b r0,r0 */
L8c075d56: *stop=0x8c075d56u;r[4]=r[12];goto L8c075d58; /* mov r12,r4 */
L8c075d58: *stop=0x8c075d58u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[10]);if(io_status)return io_status;goto L8c075d5a; /* fmov.s @(r0,r10),fr15 */
L8c075d5a: *stop=0x8c075d5au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[3],&f->fpscr,&fr[15]);if(status)return status;}goto L8c075d5c; /* fmul fr3,fr15 */
L8c075d5c: target=r[3];*pr=0x8c075d60u;*stop=0x8c075d5eu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075d60; /* jsr @r3 */
L8c075d60: *stop=0x8c075d60u;r[0]=4u;goto L8c075d62; /* mov #0x4,r0 */
L8c075d62: *stop=0x8c075d62u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075d64; /* fmov.s @(r0,r12),fr3 */
L8c075d64: *stop=0x8c075d64u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c075d66; /* fmul fr15,fr3 */
L8c075d66: *stop=0x8c075d66u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075d68; /* fmov fr3,@(r0,r12) */
L8c075d68: *stop=0x8c075d68u;r[0]=8u;goto L8c075d6a; /* mov #0x8,r0 */
L8c075d6a: *stop=0x8c075d6au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c075d6c; /* fmov.s @(r0,r12),fr2 */
L8c075d6c: *stop=0x8c075d6cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c075d6e; /* fmul fr15,fr2 */
L8c075d6e: *stop=0x8c075d6eu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c075d70; /* fmov fr2,@(r0,r12) */
L8c075d70: *stop=0x8c075d70u;r[0]=12u;goto L8c075d72; /* mov #0xc,r0 */
L8c075d72: *stop=0x8c075d72u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075d74; /* fmov.s @(r0,r12),fr3 */
L8c075d74: *stop=0x8c075d74u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c075d76; /* fmul fr15,fr3 */
L8c075d76: *stop=0x8c075d76u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075d78; /* fmov fr3,@(r0,r12) */
L8c075d78: *stop=0x8c075d7au;r[0]=1u;goto L8c075f6c; /* bra 0x8c075f6c */
L8c075d7c: *stop=0x8c075d7cu;r[6]=4294967236u;goto L8c075d7e; /* mov #-0x3c,r6 */
L8c075d7e: *stop=0x8c075d7eu;r[5]=r[10];goto L8c075d80; /* mov r10,r5 */
L8c075d80: *stop=0x8c075d80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075dd0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075d82; /* mov.l 0x8c075dd0,r3 */
L8c075d82: *stop=0x8c075d82u;r[6]=r[6]&255u;goto L8c075d84; /* extu.b r6,r6 */
L8c075d84: *stop=0x8c075d84u;r[4]=r[14];goto L8c075d86; /* mov r14,r4 */
L8c075d86: *stop=0x8c075d86u;r[6]+=r[15];goto L8c075d88; /* add r15,r6 */
L8c075d88: *stop=0x8c075d88u;r[5]+=68u;goto L8c075d8a; /* add #0x44,r5 */
L8c075d8a: target=r[3];*pr=0x8c075d8eu;*stop=0x8c075d8cu;r[4]+=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075d8e; /* jsr @r3 */
L8c075d8e: *stop=0x8c075d8eu;r[4]=4294967236u;goto L8c075d90; /* mov #-0x3c,r4 */
L8c075d90: *stop=0x8c075d90u;r[4]=r[4]&255u;goto L8c075d92; /* extu.b r4,r4 */
L8c075d92: target=r[8];*pr=0x8c075d96u;*stop=0x8c075d94u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075d96; /* jsr @r8 */
L8c075d96: *stop=0x8c075d96u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075d98; /* fldi0 fr3 */
L8c075d98: *stop=0x8c075d98u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c075d9a; /* fcmp/eq fr3,fr0 */
L8c075d9a: if(!(*sr&1u))goto L8c075de0;goto L8c075d9c; /* bf 0x8c075de0 */
L8c075d9c: *stop=0x8c075d9cu;r[2]=4294967236u;goto L8c075d9e; /* mov #-0x3c,r2 */
L8c075d9e: *stop=0x8c075d9eu;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075da0; /* mov.l @(0x1c,r14),r3 */
L8c075da0: *stop=0x8c075da0u;r[2]=r[2]&255u;goto L8c075da2; /* extu.b r2,r2 */
L8c075da2: *stop=0x8c075da2u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075da4; /* mov.l @(0x20,r14),r1 */
L8c075da4: *stop=0x8c075da4u;r[2]+=r[15];goto L8c075da6; /* add r15,r2 */
L8c075da6: *stop=0x8c075da6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c075da8; /* mov.l r3,@r2 */
L8c075da8: *stop=0x8c075da8u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c075daa; /* mov.l r1,@(0x4,r2) */
L8c075daa: *stop=0x8c075daau;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075dac; /* mov.l @(0x24,r14),r3 */
L8c075dac: *stop=0x8c075daeu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c075dea; /* bra 0x8c075dea */
L8c075de0: *stop=0x8c075de0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076040u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075de2; /* mov.l 0x8c076040,r3 */
L8c075de2: *stop=0x8c075de2u;r[4]=4294967236u;goto L8c075de4; /* mov #-0x3c,r4 */
L8c075de4: *stop=0x8c075de4u;r[4]=r[4]&255u;goto L8c075de6; /* extu.b r4,r4 */
L8c075de6: target=r[3];*pr=0x8c075deau;*stop=0x8c075de8u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075dea; /* jsr @r3 */
L8c075dea: *stop=0x8c075deau;r[0]=32u;goto L8c075dec; /* mov #0x20,r0 */
L8c075dec: *stop=0x8c075decu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c075dee; /* fmov.s @(r0,r13),fr3 */
L8c075dee: *stop=0x8c075deeu;r[0]=1u;goto L8c075df0; /* mov #0x1,r0 */
L8c075df0: *stop=0x8c075df0u;r[0]<<=8;goto L8c075df2; /* shll8 r0 */
L8c075df2: *stop=0x8c075df2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075df4; /* fmov fr3,@(r0,r15) */
L8c075df4: *stop=0x8c075df4u;r[0]=36u;goto L8c075df6; /* mov #0x24,r0 */
L8c075df6: *stop=0x8c075df6u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c075df8; /* fmov.s @(r0,r13),fr3 */
L8c075df8: *stop=0x8c075df8u;r[0]=65u;goto L8c075dfa; /* mov #0x41,r0 */
L8c075dfa: *stop=0x8c075dfau;r[0]<<=2;goto L8c075dfc; /* shll2 r0 */
L8c075dfc: *stop=0x8c075dfcu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075dfe; /* fmov fr3,@(r0,r15) */
L8c075dfe: *stop=0x8c075dfeu;r[0]=40u;goto L8c075e00; /* mov #0x28,r0 */
L8c075e00: *stop=0x8c075e00u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c075e02; /* fmov.s @(r0,r13),fr3 */
L8c075e02: *stop=0x8c075e02u;r[0]=66u;goto L8c075e04; /* mov #0x42,r0 */
L8c075e04: *stop=0x8c075e04u;r[0]<<=2;goto L8c075e06; /* shll2 r0 */
L8c075e06: *stop=0x8c075e06u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e08; /* fmov fr3,@(r0,r15) */
L8c075e08: *stop=0x8c075e08u;r[0]=68u;goto L8c075e0a; /* mov #0x44,r0 */
L8c075e0a: *stop=0x8c075e0au;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c075e0c; /* fmov.s @(r0,r10),fr3 */
L8c075e0c: *stop=0x8c075e0cu;r[0]=67u;goto L8c075e0e; /* mov #0x43,r0 */
L8c075e0e: *stop=0x8c075e0eu;r[0]<<=2;goto L8c075e10; /* shll2 r0 */
L8c075e10: *stop=0x8c075e10u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e12; /* fmov fr3,@(r0,r15) */
L8c075e12: *stop=0x8c075e12u;r[0]=72u;goto L8c075e14; /* mov #0x48,r0 */
L8c075e14: *stop=0x8c075e14u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c075e16; /* fmov.s @(r0,r10),fr3 */
L8c075e16: *stop=0x8c075e16u;r[0]=68u;goto L8c075e18; /* mov #0x44,r0 */
L8c075e18: *stop=0x8c075e18u;r[0]<<=2;goto L8c075e1a; /* shll2 r0 */
L8c075e1a: *stop=0x8c075e1au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e1c; /* fmov fr3,@(r0,r15) */
L8c075e1c: *stop=0x8c075e1cu;r[0]=76u;goto L8c075e1e; /* mov #0x4c,r0 */
L8c075e1e: *stop=0x8c075e1eu;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c075e20; /* fmov.s @(r0,r10),fr3 */
L8c075e20: *stop=0x8c075e20u;r[0]=69u;goto L8c075e22; /* mov #0x45,r0 */
L8c075e22: *stop=0x8c075e22u;r[0]<<=2;goto L8c075e24; /* shll2 r0 */
L8c075e24: *stop=0x8c075e24u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e26; /* fmov fr3,@(r0,r15) */
L8c075e26: *stop=0x8c075e26u;r[0]=96u;goto L8c075e28; /* mov #0x60,r0 */
L8c075e28: *stop=0x8c075e28u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c075e2a; /* fmov.s @(r0,r14),fr3 */
L8c075e2a: *stop=0x8c075e2au;r[0]=4294967272u;goto L8c075e2c; /* mov #-0x18,r0 */
L8c075e2c: *stop=0x8c075e2cu;r[0]=r[0]&255u;goto L8c075e2e; /* extu.b r0,r0 */
L8c075e2e: *stop=0x8c075e2eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e30; /* fmov fr3,@(r0,r15) */
L8c075e30: *stop=0x8c075e30u;r[0]=100u;goto L8c075e32; /* mov #0x64,r0 */
L8c075e32: *stop=0x8c075e32u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c075e34; /* fmov.s @(r0,r14),fr3 */
L8c075e34: *stop=0x8c075e34u;r[0]=4294967276u;goto L8c075e36; /* mov #-0x14,r0 */
L8c075e36: *stop=0x8c075e36u;r[0]=r[0]&255u;goto L8c075e38; /* extu.b r0,r0 */
L8c075e38: *stop=0x8c075e38u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e3a; /* fmov fr3,@(r0,r15) */
L8c075e3a: *stop=0x8c075e3au;r[0]=104u;goto L8c075e3c; /* mov #0x68,r0 */
L8c075e3c: *stop=0x8c075e3cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c075e3e; /* fmov.s @(r0,r14),fr3 */
L8c075e3e: *stop=0x8c075e3eu;r[0]=4294967280u;goto L8c075e40; /* mov #-0x10,r0 */
L8c075e40: *stop=0x8c075e40u;r[0]=r[0]&255u;goto L8c075e42; /* extu.b r0,r0 */
L8c075e42: *stop=0x8c075e42u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e44; /* fmov fr3,@(r0,r15) */
L8c075e44: *stop=0x8c075e44u;r[0]=4294967236u;goto L8c075e46; /* mov #-0x3c,r0 */
L8c075e46: *stop=0x8c075e46u;r[0]=r[0]&255u;goto L8c075e48; /* extu.b r0,r0 */
L8c075e48: *stop=0x8c075e48u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e4a; /* fmov.s @(r0,r15),fr3 */
L8c075e4a: *stop=0x8c075e4au;r[0]=4294967284u;goto L8c075e4c; /* mov #-0xc,r0 */
L8c075e4c: *stop=0x8c075e4cu;r[0]=r[0]&255u;goto L8c075e4e; /* extu.b r0,r0 */
L8c075e4e: *stop=0x8c075e4eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e50; /* fmov fr3,@(r0,r15) */
L8c075e50: *stop=0x8c075e50u;r[0]=4294967240u;goto L8c075e52; /* mov #-0x38,r0 */
L8c075e52: *stop=0x8c075e52u;r[0]=r[0]&255u;goto L8c075e54; /* extu.b r0,r0 */
L8c075e54: *stop=0x8c075e54u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e56; /* fmov.s @(r0,r15),fr3 */
L8c075e56: *stop=0x8c075e56u;r[0]=4294967288u;goto L8c075e58; /* mov #-0x8,r0 */
L8c075e58: *stop=0x8c075e58u;r[0]=r[0]&255u;goto L8c075e5a; /* extu.b r0,r0 */
L8c075e5a: *stop=0x8c075e5au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e5c; /* fmov fr3,@(r0,r15) */
L8c075e5c: *stop=0x8c075e5cu;r[0]=4294967244u;goto L8c075e5e; /* mov #-0x34,r0 */
L8c075e5e: *stop=0x8c075e5eu;r[0]=r[0]&255u;goto L8c075e60; /* extu.b r0,r0 */
L8c075e60: *stop=0x8c075e60u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e62; /* fmov.s @(r0,r15),fr3 */
L8c075e62: *stop=0x8c075e62u;r[0]=4294967292u;goto L8c075e64; /* mov #-0x4,r0 */
L8c075e64: *stop=0x8c075e64u;r[0]=r[0]&255u;goto L8c075e66; /* extu.b r0,r0 */
L8c075e66: *stop=0x8c075e66u;r[6]=4294967248u;goto L8c075e68; /* mov #-0x30,r6 */
L8c075e68: *stop=0x8c075e68u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075e6a; /* fmov fr3,@(r0,r15) */
L8c075e6a: *stop=0x8c075e6au;r[5]=4294967272u;goto L8c075e6c; /* mov #-0x18,r5 */
L8c075e6c: *stop=0x8c075e6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076044u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075e6e; /* mov.l 0x8c076044,r3 */
L8c075e6e: *stop=0x8c075e6eu;r[4]=1u;goto L8c075e70; /* mov #0x1,r4 */
L8c075e70: *stop=0x8c075e70u;r[5]=r[5]&255u;goto L8c075e72; /* extu.b r5,r5 */
L8c075e72: *stop=0x8c075e72u;r[6]=r[6]&255u;goto L8c075e74; /* extu.b r6,r6 */
L8c075e74: *stop=0x8c075e74u;r[4]<<=8;goto L8c075e76; /* shll8 r4 */
L8c075e76: *stop=0x8c075e76u;r[5]+=r[15];goto L8c075e78; /* add r15,r5 */
L8c075e78: *stop=0x8c075e78u;r[6]+=r[15];goto L8c075e7a; /* add r15,r6 */
L8c075e7a: target=r[3];*pr=0x8c075e7eu;*stop=0x8c075e7cu;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075e7e; /* jsr @r3 */
L8c075e7e: *stop=0x8c075e7eu;r[2]=r[14];goto L8c075e80; /* mov r14,r2 */
L8c075e80: *stop=0x8c075e80u;r[2]+=108u;goto L8c075e82; /* add #0x6c,r2 */
L8c075e82: *stop=0x8c075e82u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c075e84; /* mov.l @r2,r1 */
L8c075e84: *stop=0x8c075e84u;r[3]=r[15];goto L8c075e86; /* mov r15,r3 */
L8c075e86: *stop=0x8c075e86u;r[3]+=20u;goto L8c075e88; /* add #0x14,r3 */
L8c075e88: *stop=0x8c075e88u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c075e8a; /* mov.l @(0x4,r2),r0 */
L8c075e8a: *stop=0x8c075e8au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c075e8c; /* mov.l r1,@r3 */
L8c075e8c: *stop=0x8c075e8cu;r[4]=r[15];goto L8c075e8e; /* mov r15,r4 */
L8c075e8e: *stop=0x8c075e8eu;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c075e90; /* mov.l r0,@(0x4,r3) */
L8c075e90: *stop=0x8c075e90u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075e92; /* mov.l @(0x8,r2),r1 */
L8c075e92: *stop=0x8c075e92u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c075e94; /* mov.l r1,@(0x8,r3) */
L8c075e94: target=r[8];*pr=0x8c075e98u;*stop=0x8c075e96u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075e98; /* jsr @r8 */
L8c075e98: *stop=0x8c075e98u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075e9a; /* fldi0 fr3 */
L8c075e9a: *stop=0x8c075e9au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c075e9c; /* fcmp/eq fr3,fr0 */
L8c075e9c: if((*sr&1u))goto L8c075ea6;goto L8c075e9e; /* bt 0x8c075ea6 */
L8c075e9e: *stop=0x8c075e9eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076040u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075ea0; /* mov.l 0x8c076040,r2 */
L8c075ea0: *stop=0x8c075ea0u;r[4]=r[15];goto L8c075ea2; /* mov r15,r4 */
L8c075ea2: target=r[2];*pr=0x8c075ea6u;*stop=0x8c075ea4u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075ea6; /* jsr @r2 */
L8c075ea6: *stop=0x8c075ea6u;r[4]=4294967260u;goto L8c075ea8; /* mov #-0x24,r4 */
L8c075ea8: *stop=0x8c075ea8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076048u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075eaa; /* mov.l 0x8c076048,r3 */
L8c075eaa: *stop=0x8c075eaau;r[5]=r[15];goto L8c075eac; /* mov r15,r5 */
L8c075eac: *stop=0x8c075eacu;r[4]=r[4]&255u;goto L8c075eae; /* extu.b r4,r4 */
L8c075eae: *stop=0x8c075eaeu;r[5]+=20u;goto L8c075eb0; /* add #0x14,r5 */
L8c075eb0: target=r[3];*pr=0x8c075eb4u;*stop=0x8c075eb2u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075eb4; /* jsr @r3 */
L8c075eb4: *stop=0x8c075eb4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075eb6; /* fldi0 fr3 */
L8c075eb6: *stop=0x8c075eb6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c075eb8; /* fcmp/gt fr0,fr3 */
L8c075eb8: take=!(*sr&1u);*stop=0x8c075ebau;r[4]=r[12];if(take)goto L8c075ee6;goto L8c075ebc; /* bf/s 0x8c075ee6 */
L8c075ebc: *stop=0x8c075ebcu;r[0]=4294967260u;goto L8c075ebe; /* mov #-0x24,r0 */
L8c075ebe: *stop=0x8c075ebeu;r[0]=r[0]&255u;goto L8c075ec0; /* extu.b r0,r0 */
L8c075ec0: *stop=0x8c075ec0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c075ec2; /* fmov.s @(r0,r15),fr2 */
L8c075ec2: *stop=0x8c075ec2u;r[0]=4294967260u;goto L8c075ec4; /* mov #-0x24,r0 */
L8c075ec4: *stop=0x8c075ec4u;r[0]=r[0]&255u;goto L8c075ec6; /* extu.b r0,r0 */
L8c075ec6: *stop=0x8c075ec6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c075ec8; /* fneg fr2 */
L8c075ec8: *stop=0x8c075ec8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c075eca; /* fmov fr2,@(r0,r15) */
L8c075eca: *stop=0x8c075ecau;r[0]=4294967264u;goto L8c075ecc; /* mov #-0x20,r0 */
L8c075ecc: *stop=0x8c075eccu;r[0]=r[0]&255u;goto L8c075ece; /* extu.b r0,r0 */
L8c075ece: *stop=0x8c075eceu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c075ed0; /* fmov.s @(r0,r15),fr2 */
L8c075ed0: *stop=0x8c075ed0u;r[0]=4294967264u;goto L8c075ed2; /* mov #-0x20,r0 */
L8c075ed2: *stop=0x8c075ed2u;r[0]=r[0]&255u;goto L8c075ed4; /* extu.b r0,r0 */
L8c075ed4: *stop=0x8c075ed4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c075ed6; /* fneg fr2 */
L8c075ed6: *stop=0x8c075ed6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c075ed8; /* fmov fr2,@(r0,r15) */
L8c075ed8: *stop=0x8c075ed8u;r[0]=4294967268u;goto L8c075eda; /* mov #-0x1c,r0 */
L8c075eda: *stop=0x8c075edau;r[0]=r[0]&255u;goto L8c075edc; /* extu.b r0,r0 */
L8c075edc: *stop=0x8c075edcu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c075ede; /* fmov.s @(r0,r15),fr2 */
L8c075ede: *stop=0x8c075edeu;r[0]=4294967268u;goto L8c075ee0; /* mov #-0x1c,r0 */
L8c075ee0: *stop=0x8c075ee0u;r[0]=r[0]&255u;goto L8c075ee2; /* extu.b r0,r0 */
L8c075ee2: *stop=0x8c075ee2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c075ee4; /* fneg fr2 */
L8c075ee4: *stop=0x8c075ee4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c075ee6; /* fmov fr2,@(r0,r15) */
L8c075ee6: target=r[8];*pr=0x8c075eeau;*stop=0x8c075ee8u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075eea; /* jsr @r8 */
L8c075eea: *stop=0x8c075eeau;r[0]=4294967260u;goto L8c075eec; /* mov #-0x24,r0 */
L8c075eec: *stop=0x8c075eecu;fcopy(f,4u,0u);goto L8c075eee; /* fmov fr0,fr4 */
L8c075eee: *stop=0x8c075eeeu;r[0]=r[0]&255u;goto L8c075ef0; /* extu.b r0,r0 */
L8c075ef0: *stop=0x8c075ef0u;r[3]=r[14];goto L8c075ef2; /* mov r14,r3 */
L8c075ef2: *stop=0x8c075ef2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075ef4; /* fmov.s @(r0,r15),fr3 */
L8c075ef4: *stop=0x8c075ef4u;r[0]=4u;goto L8c075ef6; /* mov #0x4,r0 */
L8c075ef6: *stop=0x8c075ef6u;r[3]+=108u;goto L8c075ef8; /* add #0x6c,r3 */
L8c075ef8: *stop=0x8c075ef8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c075efa; /* fmul fr4,fr3 */
L8c075efa: *stop=0x8c075efau;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075efc; /* fmov fr3,@(r0,r12) */
L8c075efc: *stop=0x8c075efcu;r[0]=4294967264u;goto L8c075efe; /* mov #-0x20,r0 */
L8c075efe: *stop=0x8c075efeu;r[0]=r[0]&255u;goto L8c075f00; /* extu.b r0,r0 */
L8c075f00: *stop=0x8c075f00u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075f02; /* fmov.s @(r0,r15),fr3 */
L8c075f02: *stop=0x8c075f02u;r[0]=8u;goto L8c075f04; /* mov #0x8,r0 */
L8c075f04: *stop=0x8c075f04u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c075f06; /* fmul fr4,fr3 */
L8c075f06: *stop=0x8c075f06u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075f08; /* fmov fr3,@(r0,r12) */
L8c075f08: *stop=0x8c075f08u;r[0]=4294967268u;goto L8c075f0a; /* mov #-0x1c,r0 */
L8c075f0a: *stop=0x8c075f0au;r[0]=r[0]&255u;goto L8c075f0c; /* extu.b r0,r0 */
L8c075f0c: *stop=0x8c075f0cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c075f0e; /* fmov.s @(r0,r15),fr3 */
L8c075f0e: *stop=0x8c075f0eu;r[0]=12u;goto L8c075f10; /* mov #0xc,r0 */
L8c075f10: *stop=0x8c075f10u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c075f12; /* fmul fr4,fr3 */
L8c075f12: *stop=0x8c075f12u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075f14; /* fmov fr3,@(r0,r12) */
L8c075f14: *stop=0x8c075f14u;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075f16; /* mov.l @(0x4,r12),r2 */
L8c075f16: *stop=0x8c075f16u;if(!valid_access(r[12]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075f18; /* mov.l @(0x8,r12),r1 */
L8c075f18: *stop=0x8c075f18u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c075f1a; /* mov.l r2,@r3 */
L8c075f1a: *stop=0x8c075f1au;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c075f1c; /* mov.l r1,@(0x4,r3) */
L8c075f1c: *stop=0x8c075f1cu;r[1]=24u;goto L8c075f1e; /* mov #0x18,r1 */
L8c075f1e: *stop=0x8c075f1eu;if(!valid_access(r[12]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075f20; /* mov.l @(0xc,r12),r2 */
L8c075f20: *stop=0x8c075f20u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[2],4);if(io_status)return io_status;goto L8c075f22; /* mov.l r2,@(0x8,r3) */
L8c075f22: *stop=0x8c075f22u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c075f24; /* mov.l @(0x4,r15),r0 */
L8c075f24: *stop=0x8c075f24u;r[1]+=r[0];goto L8c075f26; /* add r0,r1 */
L8c075f26: *stop=0x8c075f26u;r[0]=4294967264u;goto L8c075f28; /* mov #-0x20,r0 */
L8c075f28: *stop=0x8c075f28u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c075f2a; /* mov.l r1,@-r15 */
L8c075f2a: *stop=0x8c075f2au;r[0]=r[0]&255u;goto L8c075f2c; /* extu.b r0,r0 */
L8c075f2c: *stop=0x8c075f2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07604cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075f2e; /* mov.l 0x8c07604c,r3 */
L8c075f2e: *stop=0x8c075f2eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c075f30; /* fmov.s @(r0,r15),fr5 */
L8c075f30: *stop=0x8c075f30u;r[0]=4294967272u;goto L8c075f32; /* mov #-0x18,r0 */
L8c075f32: *stop=0x8c075f32u;r[0]=r[0]&255u;goto L8c075f34; /* extu.b r0,r0 */
L8c075f34: target=r[3];*pr=0x8c075f38u;*stop=0x8c075f36u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075f38; /* jsr @r3 */
L8c075f38: *stop=0x8c075f38u;r[0]=0x8c076050u;goto L8c075f3a; /* mova 0x8c076050,r0 */
L8c075f3a: *stop=0x8c075f3au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c075f3c; /* mov.l @r15+,r1 */
L8c075f3c: *stop=0x8c075f3cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c075f3e; /* fmov.s @r0,fr3 */
L8c075f3e: *stop=0x8c075f3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076054u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075f40; /* mov.l 0x8c076054,r2 */
L8c075f40: *stop=0x8c075f40u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c075f42; /* fmul fr3,fr0 */
L8c075f42: *stop=0x8c075f42u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c075f44; /* ftrc fr0,FPUL */
L8c075f44: *stop=0x8c075f44u;r[3]=*fpul;goto L8c075f46; /* sts FPUL,r3 */
L8c075f46: *stop=0x8c075f46u;r[3]&=r[2];goto L8c075f48; /* and r2,r3 */
L8c075f48: *stop=0x8c075f48u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c075f4a; /* mov.l r3,@r1 */
L8c075f4a: *stop=0x8c075f4au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c075f4c; /* mov.l @(0x4,r15),r5 */
L8c075f4c: *stop=0x8c075f4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076058u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075f4e; /* mov.l 0x8c076058,r3 */
L8c075f4e: *stop=0x8c075f4eu;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c075f50; /* mov.l @(0x18,r5),r5 */
L8c075f50: target=r[3];*pr=0x8c075f54u;*stop=0x8c075f52u;if(!valid_access(r[12]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x20u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075f54; /* jsr @r3 */
L8c075f54: *stop=0x8c075f54u;r[2]=96u;goto L8c075f56; /* mov #0x60,r2 */
L8c075f56: *stop=0x8c075f56u;r[2]<<=8;goto L8c075f58; /* shll8 r2 */
L8c075f58: *stop=0x8c075f58u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[2]);goto L8c075f5a; /* cmp/gt r2,r0 */
L8c075f5a: if(!(*sr&1u))goto L8c075f66;goto L8c075f5c; /* bf 0x8c075f66 */
L8c075f5c: *stop=0x8c075f5cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075f5e; /* mov.l @(0x4,r15),r1 */
L8c075f5e: *stop=0x8c075f5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07605cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075f60; /* mov.l 0x8c07605c,r3 */
L8c075f60: *stop=0x8c075f60u;if(!valid_access(r[1]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c075f62; /* mov.l @(0x18,r1),r0 */
L8c075f62: *stop=0x8c075f62u;r[0]+=r[3];goto L8c075f64; /* add r3,r0 */
L8c075f64: *stop=0x8c075f64u;if(!valid_access(r[1]+24u,4u))return -3;wr(m,bus,&io_status,r[1]+0x18u,r[0],4);if(io_status)return io_status;goto L8c075f66; /* mov.l r0,@(0x18,r1) */
L8c075f66: *stop=0x8c075f66u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075f68; /* mov.l @(0x4,r15),r2 */
L8c075f68: *stop=0x8c075f68u;if(!valid_access(r[2]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075f6a; /* mov.l @(0x18,r2),r3 */
L8c075f6a: *stop=0x8c075f6au;if(!valid_access(r[12]+32u,4u))return -3;wr(m,bus,&io_status,r[12]+0x20u,r[3],4);if(io_status)return io_status;goto L8c075f6c; /* mov.l r3,@(0x20,r12) */
L8c075f6c: *stop=0x8c075f6cu;r[1]=70u;goto L8c075f6e; /* mov #0x46,r1 */
L8c075f6e: *stop=0x8c075f6eu;r[1]<<=2;goto L8c075f70; /* shll2 r1 */
L8c075f70: *stop=0x8c075f70u;r[15]+=r[1];goto L8c075f72; /* add r1,r15 */
L8c075f72: *stop=0x8c075f72u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c075f74; /* lds.l @r15+,PR */
L8c075f74: *stop=0x8c075f74u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075f76; /* fmov.s @r15+,fr12 */
L8c075f76: *stop=0x8c075f76u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075f78; /* fmov.s @r15+,fr13 */
L8c075f78: *stop=0x8c075f78u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075f7a; /* fmov.s @r15+,fr14 */
L8c075f7a: *stop=0x8c075f7au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075f7c; /* fmov.s @r15+,fr15 */
L8c075f7c: *stop=0x8c075f7cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c075f7e; /* mov.l @r15+,r8 */
L8c075f7e: *stop=0x8c075f7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c075f80; /* mov.l @r15+,r9 */
L8c075f80: *stop=0x8c075f80u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c075f82; /* mov.l @r15+,r10 */
L8c075f82: *stop=0x8c075f82u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c075f84; /* mov.l @r15+,r11 */
L8c075f84: *stop=0x8c075f84u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c075f86; /* mov.l @r15+,r12 */
L8c075f86: *stop=0x8c075f86u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c075f88; /* mov.l @r15+,r13 */
L8c075f88: *stop=0x8c075f88u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c075f8au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_projection_response_8c075a40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075ae4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075ae4u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075b02(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075b02u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075b16(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075b16u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075b22(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075b22u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075b58(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075b58u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075b66(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075b66u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075b72(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075b72u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075b86(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075b86u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075b90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075b90u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075bd2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075bd2u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075bf2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075bf2u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075c18(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075c18u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075c88(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075c88u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075cf2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075cf2u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075d06(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075d06u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075d16(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075d16u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075d60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075d60u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075d8e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075d8eu);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075d96(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075d96u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075dea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075deau);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075e7e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075e7eu);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075e98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075e98u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075ea6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075ea6u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075eb4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075eb4u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075eea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075eeau);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075f38(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075f38u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c075f54(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c075a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075f54u);}

static int sa_movement_projection_response_run_8c64016c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c640184u)goto L8c640184;if(resume_pc==0x8c6401c0u)goto L8c6401c0;goto L8c64016c;
L8c64016c: *stop=0x8c64016cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64016e; /* mov.l r14,@-r15 */
L8c64016e: *stop=0x8c64016eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c640170; /* mov.l r13,@-r15 */
L8c640170: *stop=0x8c640170u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c640172; /* mov.l r12,@-r15 */
L8c640172: *stop=0x8c640172u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c640174; /* fmov.s fr15,@-r15 */
L8c640174: *stop=0x8c640174u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c640176; /* sts.l PR,@-r15 */
L8c640176: *stop=0x8c640176u;r[15]+=4294967284u;goto L8c640178; /* add #-0xc,r15 */
L8c640178: *stop=0x8c640178u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640364u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64017a; /* mov.l 0x8c640364,r3 */
L8c64017a: *stop=0x8c64017au;r[12]=r[6];goto L8c64017c; /* mov r6,r12 */
L8c64017c: *stop=0x8c64017cu;r[14]=r[4];goto L8c64017e; /* mov r4,r14 */
L8c64017e: *stop=0x8c64017eu;r[13]=r[5];goto L8c640180; /* mov r5,r13 */
L8c640180: target=r[3];*pr=0x8c640184u;*stop=0x8c640182u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c640184; /* jsr @r3 */
L8c640184: *stop=0x8c640184u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c640186; /* tst r0,r0 */
L8c640186: if((*sr&1u))goto L8c6401c4;goto L8c640188; /* bt 0x8c6401c4 */
L8c640188: *stop=0x8c640188u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c64018a; /* fmov.s @r14,fr3 */
L8c64018a: *stop=0x8c64018au;r[0]=4u;goto L8c64018c; /* mov #0x4,r0 */
L8c64018c: *stop=0x8c64018cu;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c64018e; /* tst r12,r12 */
L8c64018e: *stop=0x8c64018eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c640190; /* fmov.s fr3,@r15 */
L8c640190: *stop=0x8c640190u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c640192; /* fmov.s @(r0,r14),fr3 */
L8c640192: *stop=0x8c640192u;r[0]=4u;goto L8c640194; /* mov #0x4,r0 */
L8c640194: *stop=0x8c640194u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c640196; /* fmov fr3,@(r0,r15) */
L8c640196: *stop=0x8c640196u;r[0]=8u;goto L8c640198; /* mov #0x8,r0 */
L8c640198: *stop=0x8c640198u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c64019a; /* fmov.s @(r0,r14),fr3 */
L8c64019a: *stop=0x8c64019au;r[0]=8u;goto L8c64019c; /* mov #0x8,r0 */
L8c64019c: take=(*sr&1u);*stop=0x8c64019eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;if(take)goto L8c6401b6;goto L8c6401a0; /* bt/s 0x8c6401b6 */
L8c6401a0: *stop=0x8c6401a0u;r[0]=4u;goto L8c6401a2; /* mov #0x4,r0 */
L8c6401a2: *stop=0x8c6401a2u;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[12]);if(io_status)return io_status;goto L8c6401a4; /* fmov.s fr15,@r12 */
L8c6401a4: *stop=0x8c6401a4u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c6401a6; /* fmov fr15,@(r0,r12) */
L8c6401a6: *stop=0x8c6401a6u;r[0]=8u;goto L8c6401a8; /* mov #0x8,r0 */
L8c6401a8: *stop=0x8c6401a8u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c6401aa; /* fmov fr15,@(r0,r12) */
L8c6401aa: *stop=0x8c6401aau;r[0]=12u;goto L8c6401ac; /* mov #0xc,r0 */
L8c6401ac: *stop=0x8c6401acu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c6401ae; /* fmov fr15,@(r0,r12) */
L8c6401ae: *stop=0x8c6401aeu;r[0]=16u;goto L8c6401b0; /* mov #0x10,r0 */
L8c6401b0: *stop=0x8c6401b0u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c6401b2; /* fmov fr15,@(r0,r12) */
L8c6401b2: *stop=0x8c6401b2u;r[0]=20u;goto L8c6401b4; /* mov #0x14,r0 */
L8c6401b4: *stop=0x8c6401b4u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c6401b6; /* fmov fr15,@(r0,r12) */
L8c6401b6: *stop=0x8c6401b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640368u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6401b8; /* mov.l 0x8c640368,r3 */
L8c6401b8: *stop=0x8c6401b8u;r[5]=r[13];goto L8c6401ba; /* mov r13,r5 */
L8c6401ba: *stop=0x8c6401bau;r[6]=0u;goto L8c6401bc; /* mov #0x0,r6 */
L8c6401bc: target=r[3];*pr=0x8c6401c0u;*stop=0x8c6401beu;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6401c0; /* jsr @r3 */
L8c6401c0: *stop=0x8c6401c2u;goto L8c640354; /* bra 0x8c640354 */
L8c6401c4: *stop=0x8c6401c4u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c6401c6; /* tst r12,r12 */
L8c6401c6: if(!(*sr&1u))goto L8c6401cc;goto L8c6401c8; /* bf 0x8c6401cc */
L8c6401c8: *stop=0x8c6401cau;goto L8c640352; /* bra 0x8c640352 */
L8c6401cc: *stop=0x8c6401ccu;r[0]=20u;goto L8c6401ce; /* mov #0x14,r0 */
L8c6401ce: *stop=0x8c6401ceu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6401d0; /* fmov.s @(r0,r13),fr3 */
L8c6401d0: *stop=0x8c6401d0u;r[0]=16u;goto L8c6401d2; /* mov #0x10,r0 */
L8c6401d2: *stop=0x8c6401d2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6401d4; /* fmov.s @(r0,r14),fr2 */
L8c6401d4: *stop=0x8c6401d4u;r[0]=20u;goto L8c6401d6; /* mov #0x14,r0 */
L8c6401d6: *stop=0x8c6401d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6401d8; /* fmul fr3,fr2 */
L8c6401d8: *stop=0x8c6401d8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6401da; /* fmov.s @(r0,r14),fr3 */
L8c6401da: *stop=0x8c6401dau;r[0]=16u;goto L8c6401dc; /* mov #0x10,r0 */
L8c6401dc: *stop=0x8c6401dcu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[13]);if(io_status)return io_status;goto L8c6401de; /* fmov.s @(r0,r13),fr1 */
L8c6401de: *stop=0x8c6401deu;r[0]=12u;goto L8c6401e0; /* mov #0xc,r0 */
L8c6401e0: *stop=0x8c6401e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6401e2; /* fmul fr3,fr1 */
L8c6401e2: *stop=0x8c6401e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6401e4; /* fsub fr1,fr2 */
L8c6401e4: *stop=0x8c6401e4u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6401e6; /* fmov fr2,@(r0,r12) */
L8c6401e6: *stop=0x8c6401e6u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6401e8; /* fmov.s @(r0,r13),fr3 */
L8c6401e8: *stop=0x8c6401e8u;r[0]=20u;goto L8c6401ea; /* mov #0x14,r0 */
L8c6401ea: *stop=0x8c6401eau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6401ec; /* fmov.s @(r0,r14),fr2 */
L8c6401ec: *stop=0x8c6401ecu;r[0]=12u;goto L8c6401ee; /* mov #0xc,r0 */
L8c6401ee: *stop=0x8c6401eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6401f0; /* fmul fr3,fr2 */
L8c6401f0: *stop=0x8c6401f0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6401f2; /* fmov.s @(r0,r14),fr3 */
L8c6401f2: *stop=0x8c6401f2u;r[0]=20u;goto L8c6401f4; /* mov #0x14,r0 */
L8c6401f4: *stop=0x8c6401f4u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[13]);if(io_status)return io_status;goto L8c6401f6; /* fmov.s @(r0,r13),fr1 */
L8c6401f6: *stop=0x8c6401f6u;r[0]=16u;goto L8c6401f8; /* mov #0x10,r0 */
L8c6401f8: *stop=0x8c6401f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6401fa; /* fmul fr3,fr1 */
L8c6401fa: *stop=0x8c6401fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6401fc; /* fsub fr1,fr2 */
L8c6401fc: *stop=0x8c6401fcu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6401fe; /* fmov fr2,@(r0,r12) */
L8c6401fe: *stop=0x8c6401feu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c640200; /* fmov.s @(r0,r13),fr3 */
L8c640200: *stop=0x8c640200u;r[0]=12u;goto L8c640202; /* mov #0xc,r0 */
L8c640202: *stop=0x8c640202u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c640204; /* fmov.s @(r0,r14),fr2 */
L8c640204: *stop=0x8c640204u;r[0]=16u;goto L8c640206; /* mov #0x10,r0 */
L8c640206: *stop=0x8c640206u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640208; /* fmul fr3,fr2 */
L8c640208: *stop=0x8c640208u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c64020a; /* fmov.s @(r0,r14),fr3 */
L8c64020a: *stop=0x8c64020au;r[0]=12u;goto L8c64020c; /* mov #0xc,r0 */
L8c64020c: *stop=0x8c64020cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[13]);if(io_status)return io_status;goto L8c64020e; /* fmov.s @(r0,r13),fr1 */
L8c64020e: *stop=0x8c64020eu;r[0]=20u;goto L8c640210; /* mov #0x14,r0 */
L8c640210: *stop=0x8c640210u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c640212; /* fmul fr3,fr1 */
L8c640212: *stop=0x8c640212u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c640214; /* fsub fr1,fr2 */
L8c640214: *stop=0x8c640214u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c640216; /* fmov fr2,@(r0,r12) */
L8c640216: *stop=0x8c640216u;r[0]=12u;goto L8c640218; /* mov #0xc,r0 */
L8c640218: *stop=0x8c640218u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64021a; /* fmov.s @(r0,r14),fr2 */
L8c64021a: *stop=0x8c64021au;r[0]=4u;goto L8c64021c; /* mov #0x4,r0 */
L8c64021c: *stop=0x8c64021cu;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c64021e; /* fmov.s @r14,fr3 */
L8c64021e: *stop=0x8c64021eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640220; /* fmul fr3,fr2 */
L8c640220: *stop=0x8c640220u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c640222; /* fmov.s @(r0,r14),fr3 */
L8c640222: *stop=0x8c640222u;r[0]=16u;goto L8c640224; /* mov #0x10,r0 */
L8c640224: *stop=0x8c640224u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c640226; /* fmov.s @(r0,r14),fr1 */
L8c640226: *stop=0x8c640226u;r[0]=8u;goto L8c640228; /* mov #0x8,r0 */
L8c640228: *stop=0x8c640228u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64022a; /* fmul fr3,fr1 */
L8c64022a: *stop=0x8c64022au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c64022c; /* fneg fr2 */
L8c64022c: *stop=0x8c64022cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c64022e; /* fmov.s @(r0,r14),fr3 */
L8c64022e: *stop=0x8c64022eu;r[0]=20u;goto L8c640230; /* mov #0x14,r0 */
L8c640230: *stop=0x8c640230u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c640232; /* fsub fr1,fr2 */
L8c640232: *stop=0x8c640232u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c640234; /* fmov.s @(r0,r14),fr1 */
L8c640234: *stop=0x8c640234u;r[0]=12u;goto L8c640236; /* mov #0xc,r0 */
L8c640236: *stop=0x8c640236u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c640238; /* fmul fr3,fr1 */
L8c640238: *stop=0x8c640238u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c64023a; /* fmov.s @r13,fr3 */
L8c64023a: *stop=0x8c64023au;fcopy(f,5u,2u);goto L8c64023c; /* fmov fr2,fr5 */
L8c64023c: *stop=0x8c64023cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c64023e; /* fmov.s @(r0,r13),fr2 */
L8c64023e: *stop=0x8c64023eu;r[0]=4u;goto L8c640240; /* mov #0x4,r0 */
L8c640240: *stop=0x8c640240u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[1],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c640242; /* fsub fr1,fr5 */
L8c640242: *stop=0x8c640242u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640244; /* fmul fr3,fr2 */
L8c640244: *stop=0x8c640244u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c640246; /* fmov.s @(r0,r13),fr3 */
L8c640246: *stop=0x8c640246u;r[0]=16u;goto L8c640248; /* mov #0x10,r0 */
L8c640248: *stop=0x8c640248u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[13]);if(io_status)return io_status;goto L8c64024a; /* fmov.s @(r0,r13),fr1 */
L8c64024a: *stop=0x8c64024au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c64024c; /* fneg fr2 */
L8c64024c: *stop=0x8c64024cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64024e; /* fmul fr3,fr1 */
L8c64024e: *stop=0x8c64024eu;r[0]=8u;goto L8c640250; /* mov #0x8,r0 */
L8c640250: *stop=0x8c640250u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c640252; /* fmov.s @(r0,r13),fr3 */
L8c640252: *stop=0x8c640252u;r[0]=20u;goto L8c640254; /* mov #0x14,r0 */
L8c640254: *stop=0x8c640254u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c640256; /* fsub fr1,fr2 */
L8c640256: *stop=0x8c640256u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[13]);if(io_status)return io_status;goto L8c640258; /* fmov.s @(r0,r13),fr1 */
L8c640258: *stop=0x8c640258u;r[0]=12u;goto L8c64025a; /* mov #0xc,r0 */
L8c64025a: *stop=0x8c64025au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64025c; /* fmul fr3,fr1 */
L8c64025c: *stop=0x8c64025cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c64025e; /* fldi0 fr3 */
L8c64025e: *stop=0x8c64025eu;fcopy(f,4u,2u);goto L8c640260; /* fmov fr2,fr4 */
L8c640260: *stop=0x8c640260u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c640262; /* fmov.s @(r0,r12),fr2 */
L8c640262: *stop=0x8c640262u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c640264; /* fcmp/eq fr3,fr2 */
L8c640264: take=!(*sr&1u);*stop=0x8c640266u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}if(take)goto L8c640308;goto L8c640268; /* bf/s 0x8c640308 */
L8c640268: *stop=0x8c640268u;r[0]=16u;goto L8c64026a; /* mov #0x10,r0 */
L8c64026a: *stop=0x8c64026au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c64026c; /* fmov.s @(r0,r12),fr2 */
L8c64026c: *stop=0x8c64026cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c64026e; /* fcmp/eq fr3,fr2 */
L8c64026e: if(!(*sr&1u))goto L8c6402b8;goto L8c640270; /* bf 0x8c6402b8 */
L8c640270: *stop=0x8c640270u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[13]);if(io_status)return io_status;goto L8c640272; /* fmov.s @(r0,r13),fr6 */
L8c640272: *stop=0x8c640272u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[14]);if(io_status)return io_status;goto L8c640274; /* fmov.s @(r0,r14),fr7 */
L8c640274: *stop=0x8c640274u;r[0]=12u;goto L8c640276; /* mov #0xc,r0 */
L8c640276: *stop=0x8c640276u;fcopy(f,2u,6u);goto L8c640278; /* fmov fr6,fr2 */
L8c640278: *stop=0x8c640278u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64027a; /* fmul fr5,fr2 */
L8c64027a: *stop=0x8c64027au;fcopy(f,3u,7u);goto L8c64027c; /* fmov fr7,fr3 */
L8c64027c: *stop=0x8c64027cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64027e; /* fmul fr4,fr3 */
L8c64027e: *stop=0x8c64027eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c640280; /* fmov.s @(r0,r14),fr1 */
L8c640280: *stop=0x8c640280u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c640282; /* fmul fr6,fr1 */
L8c640282: *stop=0x8c640282u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c640284; /* fsub fr2,fr3 */
L8c640284: *stop=0x8c640284u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c640286; /* fmov.s @(r0,r13),fr2 */
L8c640286: *stop=0x8c640286u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640288; /* fmul fr7,fr2 */
L8c640288: *stop=0x8c640288u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c64028a; /* fsub fr2,fr1 */
L8c64028a: *stop=0x8c64028au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[1],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64028c; /* fdiv fr1,fr3 */
L8c64028c: *stop=0x8c64028cu;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c64028e; /* fmov.s fr3,@r12 */
L8c64028e: *stop=0x8c64028eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[13]);if(io_status)return io_status;goto L8c640290; /* fmov.s @(r0,r13),fr6 */
L8c640290: *stop=0x8c640290u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[14]);if(io_status)return io_status;goto L8c640292; /* fmov.s @(r0,r14),fr7 */
L8c640292: *stop=0x8c640292u;r[0]=16u;goto L8c640294; /* mov #0x10,r0 */
L8c640294: *stop=0x8c640294u;fcopy(f,2u,6u);goto L8c640296; /* fmov fr6,fr2 */
L8c640296: *stop=0x8c640296u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640298; /* fmul fr5,fr2 */
L8c640298: *stop=0x8c640298u;fcopy(f,3u,7u);goto L8c64029a; /* fmov fr7,fr3 */
L8c64029a: *stop=0x8c64029au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64029c; /* fmul fr4,fr3 */
L8c64029c: *stop=0x8c64029cu;fcopy(f,1u,6u);goto L8c64029e; /* fmov fr6,fr1 */
L8c64029e: *stop=0x8c64029eu;fcopy(f,0u,7u);goto L8c6402a0; /* fmov fr7,fr0 */
L8c6402a0: *stop=0x8c6402a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6402a2; /* fsub fr2,fr3 */
L8c6402a2: *stop=0x8c6402a2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6402a4; /* fmov.s @(r0,r14),fr2 */
L8c6402a4: *stop=0x8c6402a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6402a6; /* fmul fr2,fr1 */
L8c6402a6: *stop=0x8c6402a6u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c6402a8; /* fmov.s @(r0,r13),fr2 */
L8c6402a8: *stop=0x8c6402a8u;r[0]=4u;goto L8c6402aa; /* mov #0x4,r0 */
L8c6402aa: *stop=0x8c6402aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[2],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6402ac; /* fmul fr2,fr0 */
L8c6402ac: *stop=0x8c6402acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[0],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6402ae; /* fsub fr0,fr1 */
L8c6402ae: *stop=0x8c6402aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[1],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6402b0; /* fdiv fr1,fr3 */
L8c6402b0: *stop=0x8c6402b0u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6402b2; /* fmov fr3,@(r0,r12) */
L8c6402b2: *stop=0x8c6402b2u;r[0]=8u;goto L8c6402b4; /* mov #0x8,r0 */
L8c6402b4: *stop=0x8c6402b6u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c640352; /* bra 0x8c640352 */
L8c6402b8: *stop=0x8c6402b8u;r[0]=12u;goto L8c6402ba; /* mov #0xc,r0 */
L8c6402ba: *stop=0x8c6402bau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6402bc; /* fldi0 fr3 */
L8c6402bc: *stop=0x8c6402bcu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6402be; /* fmov.s @(r0,r12),fr2 */
L8c6402be: *stop=0x8c6402beu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c6402c0; /* fcmp/eq fr3,fr2 */
L8c6402c0: if(!(*sr&1u))goto L8c640308;goto L8c6402c2; /* bf 0x8c640308 */
L8c6402c2: *stop=0x8c6402c2u;r[0]=20u;goto L8c6402c4; /* mov #0x14,r0 */
L8c6402c4: *stop=0x8c6402c4u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6402c6; /* fmov.s @(r0,r12),fr2 */
L8c6402c6: *stop=0x8c6402c6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c6402c8; /* fcmp/eq fr3,fr2 */
L8c6402c8: if(!(*sr&1u))goto L8c640308;goto L8c6402ca; /* bf 0x8c640308 */
L8c6402ca: *stop=0x8c6402cau;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[13]);if(io_status)return io_status;goto L8c6402cc; /* fmov.s @(r0,r13),fr6 */
L8c6402cc: *stop=0x8c6402ccu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[14]);if(io_status)return io_status;goto L8c6402ce; /* fmov.s @(r0,r14),fr7 */
L8c6402ce: *stop=0x8c6402ceu;r[0]=12u;goto L8c6402d0; /* mov #0xc,r0 */
L8c6402d0: *stop=0x8c6402d0u;fcopy(f,2u,6u);goto L8c6402d2; /* fmov fr6,fr2 */
L8c6402d2: *stop=0x8c6402d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6402d4; /* fmul fr5,fr2 */
L8c6402d4: *stop=0x8c6402d4u;fcopy(f,3u,7u);goto L8c6402d6; /* fmov fr7,fr3 */
L8c6402d6: *stop=0x8c6402d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6402d8; /* fmul fr4,fr3 */
L8c6402d8: *stop=0x8c6402d8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c6402da; /* fmov.s @(r0,r14),fr1 */
L8c6402da: *stop=0x8c6402dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6402dc; /* fmul fr6,fr1 */
L8c6402dc: *stop=0x8c6402dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6402de; /* fsub fr2,fr3 */
L8c6402de: *stop=0x8c6402deu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c6402e0; /* fmov.s @(r0,r13),fr2 */
L8c6402e0: *stop=0x8c6402e0u;r[0]=4u;goto L8c6402e2; /* mov #0x4,r0 */
L8c6402e2: *stop=0x8c6402e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6402e4; /* fmul fr7,fr2 */
L8c6402e4: *stop=0x8c6402e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6402e6; /* fsub fr2,fr1 */
L8c6402e6: *stop=0x8c6402e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[1],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6402e8; /* fdiv fr1,fr3 */
L8c6402e8: *stop=0x8c6402e8u;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c6402ea; /* fmov.s fr3,@r12 */
L8c6402ea: *stop=0x8c6402eau;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c6402ec; /* fmov fr15,@(r0,r12) */
L8c6402ec: *stop=0x8c6402ecu;r[0]=12u;goto L8c6402ee; /* mov #0xc,r0 */
L8c6402ee: *stop=0x8c6402eeu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[14]);if(io_status)return io_status;goto L8c6402f0; /* fmov.s @(r0,r14),fr7 */
L8c6402f0: *stop=0x8c6402f0u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[13]);if(io_status)return io_status;goto L8c6402f2; /* fmov.s @(r0,r13),fr6 */
L8c6402f2: *stop=0x8c6402f2u;r[0]=20u;goto L8c6402f4; /* mov #0x14,r0 */
L8c6402f4: *stop=0x8c6402f4u;fcopy(f,3u,7u);goto L8c6402f6; /* fmov fr7,fr3 */
L8c6402f6: *stop=0x8c6402f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6402f8; /* fmul fr4,fr3 */
L8c6402f8: *stop=0x8c6402f8u;fcopy(f,2u,6u);goto L8c6402fa; /* fmov fr6,fr2 */
L8c6402fa: *stop=0x8c6402fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6402fc; /* fmul fr5,fr2 */
L8c6402fc: *stop=0x8c6402fcu;fcopy(f,1u,6u);goto L8c6402fe; /* fmov fr6,fr1 */
L8c6402fe: *stop=0x8c6402feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c640300; /* fsub fr2,fr3 */
L8c640300: *stop=0x8c640300u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c640302; /* fmov.s @(r0,r14),fr2 */
L8c640302: *stop=0x8c640302u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c640304; /* fmul fr2,fr1 */
L8c640304: *stop=0x8c640306u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c640346; /* bra 0x8c640346 */
L8c640308: *stop=0x8c640308u;r[0]=20u;goto L8c64030a; /* mov #0x14,r0 */
L8c64030a: *stop=0x8c64030au;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[12]);if(io_status)return io_status;goto L8c64030c; /* fmov.s fr15,@r12 */
L8c64030c: *stop=0x8c64030cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[14]);if(io_status)return io_status;goto L8c64030e; /* fmov.s @(r0,r14),fr6 */
L8c64030e: *stop=0x8c64030eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[13]);if(io_status)return io_status;goto L8c640310; /* fmov.s @(r0,r13),fr7 */
L8c640310: *stop=0x8c640310u;r[0]=16u;goto L8c640312; /* mov #0x10,r0 */
L8c640312: *stop=0x8c640312u;fcopy(f,2u,6u);goto L8c640314; /* fmov fr6,fr2 */
L8c640314: *stop=0x8c640314u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640316; /* fmul fr4,fr2 */
L8c640316: *stop=0x8c640316u;fcopy(f,3u,7u);goto L8c640318; /* fmov fr7,fr3 */
L8c640318: *stop=0x8c640318u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64031a; /* fmul fr5,fr3 */
L8c64031a: *stop=0x8c64031au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[13]);if(io_status)return io_status;goto L8c64031c; /* fmov.s @(r0,r13),fr1 */
L8c64031c: *stop=0x8c64031cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64031e; /* fmul fr6,fr1 */
L8c64031e: *stop=0x8c64031eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c640320; /* fsub fr2,fr3 */
L8c640320: *stop=0x8c640320u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c640322; /* fmov.s @(r0,r14),fr2 */
L8c640322: *stop=0x8c640322u;r[0]=4u;goto L8c640324; /* mov #0x4,r0 */
L8c640324: *stop=0x8c640324u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640326; /* fmul fr7,fr2 */
L8c640326: *stop=0x8c640326u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c640328; /* fsub fr2,fr1 */
L8c640328: *stop=0x8c640328u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[1],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64032a; /* fdiv fr1,fr3 */
L8c64032a: *stop=0x8c64032au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c64032c; /* fmov fr3,@(r0,r12) */
L8c64032c: *stop=0x8c64032cu;r[0]=16u;goto L8c64032e; /* mov #0x10,r0 */
L8c64032e: *stop=0x8c64032eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[14]);if(io_status)return io_status;goto L8c640330; /* fmov.s @(r0,r14),fr6 */
L8c640330: *stop=0x8c640330u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[13]);if(io_status)return io_status;goto L8c640332; /* fmov.s @(r0,r13),fr7 */
L8c640332: *stop=0x8c640332u;r[0]=20u;goto L8c640334; /* mov #0x14,r0 */
L8c640334: *stop=0x8c640334u;fcopy(f,2u,6u);goto L8c640336; /* fmov fr6,fr2 */
L8c640336: *stop=0x8c640336u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640338; /* fmul fr4,fr2 */
L8c640338: *stop=0x8c640338u;fcopy(f,3u,7u);goto L8c64033a; /* fmov fr7,fr3 */
L8c64033a: *stop=0x8c64033au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64033c; /* fmul fr5,fr3 */
L8c64033c: *stop=0x8c64033cu;fcopy(f,1u,6u);goto L8c64033e; /* fmov fr6,fr1 */
L8c64033e: *stop=0x8c64033eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c640340; /* fsub fr2,fr3 */
L8c640340: *stop=0x8c640340u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c640342; /* fmov.s @(r0,r13),fr2 */
L8c640342: *stop=0x8c640342u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c640344; /* fmul fr2,fr1 */
L8c640344: *stop=0x8c640344u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c640346; /* fmov.s @(r0,r14),fr2 */
L8c640346: *stop=0x8c640346u;fcopy(f,0u,7u);goto L8c640348; /* fmov fr7,fr0 */
L8c640348: *stop=0x8c640348u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[2],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64034a; /* fmul fr2,fr0 */
L8c64034a: *stop=0x8c64034au;r[0]=8u;goto L8c64034c; /* mov #0x8,r0 */
L8c64034c: *stop=0x8c64034cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[0],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c64034e; /* fsub fr0,fr1 */
L8c64034e: *stop=0x8c64034eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[1],&f->fpscr,&fr[3]);if(status)return status;}goto L8c640350; /* fdiv fr1,fr3 */
L8c640350: *stop=0x8c640350u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c640352; /* fmov fr3,@(r0,r12) */
L8c640352: *stop=0x8c640352u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c640354; /* fldi0 fr0 */
L8c640354: *stop=0x8c640354u;r[15]+=12u;goto L8c640356; /* add #0xc,r15 */
L8c640356: *stop=0x8c640356u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c640358; /* lds.l @r15+,PR */
L8c640358: *stop=0x8c640358u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64035a; /* fmov.s @r15+,fr15 */
L8c64035a: *stop=0x8c64035au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64035c; /* mov.l @r15+,r12 */
L8c64035c: *stop=0x8c64035cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64035e; /* mov.l @r15+,r13 */
L8c64035e: *stop=0x8c64035eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c640360u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_projection_response_8c64016c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c64016c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c640184(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c64016c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c640184u);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c6401c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c64016c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6401c0u);}

SA_STAGE_EXPORT int sa_short_absolute_difference_8c055d5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c055d5a;
L8c055d5a: *stop=0x8c055d5au;r[5]=r[5]&65535u;goto L8c055d5c; /* extu.w r5,r5 */
L8c055d5c: *stop=0x8c055d5cu;r[3]=r[5];goto L8c055d5e; /* mov r5,r3 */
L8c055d5e: *stop=0x8c055d5eu;r[4]=r[4]&65535u;goto L8c055d60; /* extu.w r4,r4 */
L8c055d60: *stop=0x8c055d60u;r[3]-=r[4];goto L8c055d62; /* sub r4,r3 */
L8c055d62: *stop=0x8c055d62u;r[4]=r[3];goto L8c055d64; /* mov r3,r4 */
L8c055d64: *stop=0x8c055d64u;r[2]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c055d66; /* exts.w r4,r2 */
L8c055d66: *stop=0x8c055d66u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c055d68; /* cmp/pz r2 */
L8c055d68: if((*sr&1u))goto L8c055d6c;goto L8c055d6a; /* bt 0x8c055d6c */
L8c055d6a: *stop=0x8c055d6au;r[4]=0u-r[4];goto L8c055d6c; /* neg r4,r4 */
L8c055d6c: *stop=0x8c055d6cu;if(*pr!=entry_pr)return -3;*stop=0x8c055d6eu;r[0]=r[4]&65535u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movement_projection_response_8c650614(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c650614;
L8c650614: *stop=0x8c650614u;r[0]=12u;goto L8c650616; /* mov #0xc,r0 */
L8c650616: *stop=0x8c650616u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c650618; /* fmov.s fr15,@-r15 */
L8c650618: *stop=0x8c650618u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[0]+r[4]);if(io_status)return io_status;goto L8c65061a; /* fmov.s @(r0,r4),fr10 */
L8c65061a: *stop=0x8c65061au;r[0]=16u;goto L8c65061c; /* mov #0x10,r0 */
L8c65061c: *stop=0x8c65061cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c65061e; /* fmov.s @(r0,r4),fr5 */
L8c65061e: *stop=0x8c65061eu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[5]);if(io_status)return io_status;goto L8c650620; /* fmov.s @(r0,r5),fr6 */
L8c650620: *stop=0x8c650620u;r[0]=12u;goto L8c650622; /* mov #0xc,r0 */
L8c650622: *stop=0x8c650622u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[5]);if(io_status)return io_status;goto L8c650624; /* fmov.s @(r0,r5),fr9 */
L8c650624: *stop=0x8c650624u;r[0]=20u;goto L8c650626; /* mov #0x14,r0 */
L8c650626: *stop=0x8c650626u;fcopy(f,0u,10u);goto L8c650628; /* fmov fr10,fr0 */
L8c650628: *stop=0x8c650628u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c65062a; /* fmul fr6,fr0 */
L8c65062a: *stop=0x8c65062au;fcopy(f,3u,5u);goto L8c65062c; /* fmov fr5,fr3 */
L8c65062c: *stop=0x8c65062cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65062e; /* fmul fr9,fr3 */
L8c65062e: *stop=0x8c65062eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[0]+r[4]);if(io_status)return io_status;goto L8c650630; /* fmov.s @(r0,r4),fr8 */
L8c650630: *stop=0x8c650630u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]+r[5]);if(io_status)return io_status;goto L8c650632; /* fmov.s @(r0,r5),fr11 */
L8c650632: *stop=0x8c650632u;fcopy(f,4u,0u);goto L8c650634; /* fmov fr0,fr4 */
L8c650634: *stop=0x8c650634u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c650636; /* fsub fr3,fr4 */
L8c650636: *stop=0x8c650636u;fcopy(f,3u,5u);goto L8c650638; /* fmov fr5,fr3 */
L8c650638: *stop=0x8c650638u;fcopy(f,2u,8u);goto L8c65063a; /* fmov fr8,fr2 */
L8c65063a: *stop=0x8c65063au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[11],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65063c; /* fmul fr11,fr3 */
L8c65063c: *stop=0x8c65063cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c65063e; /* fmul fr6,fr2 */
L8c65063e: *stop=0x8c65063eu;fcopy(f,1u,10u);goto L8c650640; /* fmov fr10,fr1 */
L8c650640: *stop=0x8c650640u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[11],&f->fpscr,&fr[1]);if(status)return status;}goto L8c650642; /* fmul fr11,fr1 */
L8c650642: *stop=0x8c650642u;fcopy(f,0u,4u);goto L8c650644; /* fmov fr4,fr0 */
L8c650644: *stop=0x8c650644u;fcopy(f,7u,3u);goto L8c650646; /* fmov fr3,fr7 */
L8c650646: *stop=0x8c650646u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[2],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c650648; /* fsub fr2,fr7 */
L8c650648: *stop=0x8c650648u;fcopy(f,2u,8u);goto L8c65064a; /* fmov fr8,fr2 */
L8c65064a: *stop=0x8c65064au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c65064c; /* fmul fr9,fr2 */
L8c65064c: *stop=0x8c65064cu;fcopy(f,3u,7u);goto L8c65064e; /* fmov fr7,fr3 */
L8c65064e: *stop=0x8c65064eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650650; /* fmul fr7,fr3 */
L8c650650: *stop=0x8c650650u;fcopy(f,15u,2u);goto L8c650652; /* fmov fr2,fr15 */
L8c650652: *stop=0x8c650652u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650654; /* fmac fr0,fr4,fr3 */
L8c650654: *stop=0x8c650654u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[1],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c650656; /* fsub fr1,fr15 */
L8c650656: *stop=0x8c650656u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c650658; /* fldi0 fr2 */
L8c650658: *stop=0x8c650658u;fcopy(f,0u,15u);goto L8c65065a; /* fmov fr15,fr0 */
L8c65065a: *stop=0x8c65065au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[15],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65065c; /* fmac fr0,fr15,fr3 */
L8c65065c: *stop=0x8c65065cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,0);if(status)return status;goto L8c65065e; /* fcmp/eq fr2,fr3 */
L8c65065e: *stop=0x8c65065eu;r[0]=*sr&1u;goto L8c650660; /* movt r0 */
L8c650660: *stop=0x8c650660u;if(*pr!=entry_pr)return -3;*stop=0x8c650662u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;return 0; /* rts */
}

static int sa_movement_projection_response_run_8c640080(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c640114u)goto L8c640114;goto L8c640080;
L8c640080: *stop=0x8c640080u;r[0]=12u;goto L8c640082; /* mov #0xc,r0 */
L8c640082: *stop=0x8c640082u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c640084; /* fmov.s fr15,@-r15 */
L8c640084: *stop=0x8c640084u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c640086; /* fmov.s fr14,@-r15 */
L8c640086: *stop=0x8c640086u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c640088; /* sts.l PR,@-r15 */
L8c640088: *stop=0x8c640088u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[5]);if(io_status)return io_status;goto L8c64008a; /* fmov.s @(r0,r5),fr6 */
L8c64008a: *stop=0x8c64008au;r[0]=16u;goto L8c64008c; /* mov #0x10,r0 */
L8c64008c: *stop=0x8c64008cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c64008e; /* fmov.s @r5,fr3 */
L8c64008e: *stop=0x8c64008eu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c640090; /* fmov.s @(r0,r5),fr5 */
L8c640090: *stop=0x8c640090u;r[0]=4u;goto L8c640092; /* mov #0x4,r0 */
L8c640092: *stop=0x8c640092u;fcopy(f,2u,6u);goto L8c640094; /* fmov fr6,fr2 */
L8c640094: *stop=0x8c640094u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640096; /* fmul fr3,fr2 */
L8c640096: *stop=0x8c640096u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c640098; /* fmov.s @(r0,r5),fr3 */
L8c640098: *stop=0x8c640098u;r[0]=20u;goto L8c64009a; /* mov #0x14,r0 */
L8c64009a: *stop=0x8c64009au;fcopy(f,1u,5u);goto L8c64009c; /* fmov fr5,fr1 */
L8c64009c: *stop=0x8c64009cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64009e; /* fmul fr3,fr1 */
L8c64009e: *stop=0x8c64009eu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[5]);if(io_status)return io_status;goto L8c6400a0; /* fmov.s @(r0,r5),fr7 */
L8c6400a0: *stop=0x8c6400a0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c6400a2; /* fneg fr2 */
L8c6400a2: *stop=0x8c6400a2u;r[0]=8u;goto L8c6400a4; /* mov #0x8,r0 */
L8c6400a4: *stop=0x8c6400a4u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6400a6; /* fmov.s @(r0,r5),fr3 */
L8c6400a6: *stop=0x8c6400a6u;r[0]=4u;goto L8c6400a8; /* mov #0x4,r0 */
L8c6400a8: *stop=0x8c6400a8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c6400aa; /* fmov.s @(r0,r4),fr0 */
L8c6400aa: *stop=0x8c6400aau;r[0]=8u;goto L8c6400ac; /* mov #0x8,r0 */
L8c6400ac: *stop=0x8c6400acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6400ae; /* fsub fr1,fr2 */
L8c6400ae: *stop=0x8c6400aeu;fcopy(f,1u,7u);goto L8c6400b0; /* fmov fr7,fr1 */
L8c6400b0: *stop=0x8c6400b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6400b2; /* fmul fr3,fr1 */
L8c6400b2: *stop=0x8c6400b2u;fcopy(f,3u,5u);goto L8c6400b4; /* fmov fr5,fr3 */
L8c6400b4: *stop=0x8c6400b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6400b6; /* fmul fr0,fr3 */
L8c6400b6: *stop=0x8c6400b6u;fcopy(f,0u,6u);goto L8c6400b8; /* fmov fr6,fr0 */
L8c6400b8: *stop=0x8c6400b8u;fcopy(f,4u,2u);goto L8c6400ba; /* fmov fr2,fr4 */
L8c6400ba: *stop=0x8c6400bau;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6400bc; /* fmov.s @r4,fr2 */
L8c6400bc: *stop=0x8c6400bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6400be; /* fsub fr1,fr4 */
L8c6400be: *stop=0x8c6400beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6400c0; /* fmac fr0,fr2,fr3 */
L8c6400c0: *stop=0x8c6400c0u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c6400c2; /* fmov.s @(r0,r4),fr1 */
L8c6400c2: *stop=0x8c6400c2u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c6400c4; /* tst r6,r6 */
L8c6400c4: *stop=0x8c6400c4u;fcopy(f,0u,7u);goto L8c6400c6; /* fmov fr7,fr0 */
L8c6400c6: *stop=0x8c6400c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6400c8; /* fmac fr0,fr1,fr3 */
L8c6400c8: *stop=0x8c6400c8u;fcopy(f,0u,6u);goto L8c6400ca; /* fmov fr6,fr0 */
L8c6400ca: *stop=0x8c6400cau;fcopy(f,15u,3u);goto L8c6400cc; /* fmov fr3,fr15 */
L8c6400cc: *stop=0x8c6400ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[4],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c6400ce; /* fadd fr4,fr15 */
L8c6400ce: *stop=0x8c6400ceu;fcopy(f,3u,5u);goto L8c6400d0; /* fmov fr5,fr3 */
L8c6400d0: *stop=0x8c6400d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6400d2; /* fmul fr5,fr3 */
L8c6400d2: *stop=0x8c6400d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6400d4; /* fmac fr0,fr6,fr3 */
L8c6400d4: *stop=0x8c6400d4u;fcopy(f,0u,7u);goto L8c6400d6; /* fmov fr7,fr0 */
L8c6400d6: *stop=0x8c6400d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[7],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6400d8; /* fmac fr0,fr7,fr3 */
L8c6400d8: take=(*sr&1u);*stop=0x8c6400dau;fcopy(f,14u,3u);if(take)goto L8c64010c;goto L8c6400dc; /* bt/s 0x8c64010c */
L8c6400dc: *stop=0x8c6400dcu;fcopy(f,3u,15u);goto L8c6400de; /* fmov fr15,fr3 */
L8c6400de: *stop=0x8c6400deu;r[0]=12u;goto L8c6400e0; /* mov #0xc,r0 */
L8c6400e0: *stop=0x8c6400e0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c6400e2; /* fneg fr3 */
L8c6400e2: *stop=0x8c6400e2u;fcopy(f,4u,3u);goto L8c6400e4; /* fmov fr3,fr4 */
L8c6400e4: *stop=0x8c6400e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[14],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6400e6; /* fdiv fr14,fr4 */
L8c6400e6: *stop=0x8c6400e6u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6400e8; /* fmov.s @(r0,r5),fr3 */
L8c6400e8: *stop=0x8c6400e8u;r[0]=4u;goto L8c6400ea; /* mov #0x4,r0 */
L8c6400ea: *stop=0x8c6400eau;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6400ec; /* fmov.s @r4,fr2 */
L8c6400ec: *stop=0x8c6400ecu;fcopy(f,0u,4u);goto L8c6400ee; /* fmov fr4,fr0 */
L8c6400ee: *stop=0x8c6400eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6400f0; /* fmac fr0,fr3,fr2 */
L8c6400f0: *stop=0x8c6400f0u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c6400f2; /* fmov.s fr2,@r6 */
L8c6400f2: *stop=0x8c6400f2u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6400f4; /* fmov.s @(r0,r4),fr3 */
L8c6400f4: *stop=0x8c6400f4u;r[0]=16u;goto L8c6400f6; /* mov #0x10,r0 */
L8c6400f6: *stop=0x8c6400f6u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6400f8; /* fmov.s @(r0,r5),fr2 */
L8c6400f8: *stop=0x8c6400f8u;r[0]=4u;goto L8c6400fa; /* mov #0x4,r0 */
L8c6400fa: *stop=0x8c6400fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6400fc; /* fmac fr0,fr2,fr3 */
L8c6400fc: *stop=0x8c6400fcu;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6400fe; /* fmov fr3,@(r0,r6) */
L8c6400fe: *stop=0x8c6400feu;r[0]=8u;goto L8c640100; /* mov #0x8,r0 */
L8c640100: *stop=0x8c640100u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c640102; /* fmov.s @(r0,r4),fr3 */
L8c640102: *stop=0x8c640102u;r[0]=20u;goto L8c640104; /* mov #0x14,r0 */
L8c640104: *stop=0x8c640104u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c640106; /* fmov.s @(r0,r5),fr2 */
L8c640106: *stop=0x8c640106u;r[0]=8u;goto L8c640108; /* mov #0x8,r0 */
L8c640108: *stop=0x8c640108u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64010a; /* fmac fr0,fr2,fr3 */
L8c64010a: *stop=0x8c64010au;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c64010c; /* fmov fr3,@(r0,r6) */
L8c64010c: *stop=0x8c64010cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c640124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64010e; /* mov.l 0x8c640124,r3 */
L8c64010e: *stop=0x8c64010eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]&=0x7fffffffu;goto L8c640110; /* fabs fr15 */
L8c640110: target=r[3];*pr=0x8c640114u;*stop=0x8c640112u;fcopy(f,4u,14u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c640114; /* jsr @r3 */
L8c640114: *stop=0x8c640114u;fcopy(f,3u,0u);goto L8c640116; /* fmov fr0,fr3 */
L8c640116: *stop=0x8c640116u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c640118; /* lds.l @r15+,PR */
L8c640118: *stop=0x8c640118u;fcopy(f,0u,15u);goto L8c64011a; /* fmov fr15,fr0 */
L8c64011a: *stop=0x8c64011au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64011c; /* fmul fr3,fr0 */
L8c64011c: *stop=0x8c64011cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64011e; /* fmov.s @r15+,fr14 */
L8c64011e: *stop=0x8c64011eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c640120u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_projection_response_8c640080(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c640080(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_projection_response_resume_8c640114(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_projection_response_run_8c640080(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c640114u);}

SA_STAGE_EXPORT int sa_movement_projection_response_8c63a9dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c63a9dc;
L8c63a9dc: *stop=0x8c63a9dcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c63a9de; /* fldi0 fr0 */
L8c63a9de: *stop=0x8c63a9deu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c63a9e0; /* fcmp/gt fr4,fr0 */
L8c63a9e0: take=!(*sr&1u);*stop=0x8c63a9e2u;fcopy(f,0u,4u);if(take)goto L8c63a9ec;goto L8c63a9e4; /* bf/s 0x8c63a9ec */
L8c63a9e4: *stop=0x8c63a9e4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]&=0x7fffffffu;goto L8c63a9e6; /* fabs fr0 */
L8c63a9e6: *stop=0x8c63a9e6u;status=sa_sh4_vector_numeric(0xf07du,f,*fpul);if(status)return status;goto L8c63a9e8; /* fsrra fr0 */
L8c63a9e8: *stop=0x8c63a9e8u;if(*pr!=entry_pr)return -3;*stop=0x8c63a9eau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;return 0; /* rts */
L8c63a9ec: *stop=0x8c63a9ecu;if(*pr!=entry_pr)return -3;*stop=0x8c63a9eeu;status=sa_sh4_vector_numeric(0xf07du,f,*fpul);if(status)return status;return 0; /* rts */
}
