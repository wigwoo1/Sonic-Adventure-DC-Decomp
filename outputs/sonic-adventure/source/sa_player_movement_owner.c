/* Original complete movement owner and original argument forwarding path.
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
static int sa_player_movement_owner_run_8c072b44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c072bf2u)goto L8c072bf2;if(resume_pc==0x8c072c0eu)goto L8c072c0e;if(resume_pc==0x8c072c2au)goto L8c072c2a;if(resume_pc==0x8c072c32u)goto L8c072c32;if(resume_pc==0x8c072c88u)goto L8c072c88;if(resume_pc==0x8c072c94u)goto L8c072c94;if(resume_pc==0x8c072caau)goto L8c072caa;if(resume_pc==0x8c072d3au)goto L8c072d3a;if(resume_pc==0x8c072d48u)goto L8c072d48;if(resume_pc==0x8c072d5cu)goto L8c072d5c;if(resume_pc==0x8c072d74u)goto L8c072d74;if(resume_pc==0x8c072d80u)goto L8c072d80;if(resume_pc==0x8c072d90u)goto L8c072d90;if(resume_pc==0x8c072da0u)goto L8c072da0;if(resume_pc==0x8c072dbeu)goto L8c072dbe;if(resume_pc==0x8c072dc4u)goto L8c072dc4;if(resume_pc==0x8c072e88u)goto L8c072e88;if(resume_pc==0x8c072e9eu)goto L8c072e9e;if(resume_pc==0x8c072f1eu)goto L8c072f1e;if(resume_pc==0x8c072f2cu)goto L8c072f2c;if(resume_pc==0x8c072f5cu)goto L8c072f5c;if(resume_pc==0x8c072fdeu)goto L8c072fde;if(resume_pc==0x8c072feeu)goto L8c072fee;if(resume_pc==0x8c072ffcu)goto L8c072ffc;if(resume_pc==0x8c073010u)goto L8c073010;if(resume_pc==0x8c073020u)goto L8c073020;if(resume_pc==0x8c073030u)goto L8c073030;if(resume_pc==0x8c073042u)goto L8c073042;if(resume_pc==0x8c07305eu)goto L8c07305e;if(resume_pc==0x8c073090u)goto L8c073090;if(resume_pc==0x8c073096u)goto L8c073096;if(resume_pc==0x8c0730b2u)goto L8c0730b2;goto L8c072b44;
L8c072b44: *stop=0x8c072b44u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c072b46; /* mov.l r14,@-r15 */
L8c072b46: *stop=0x8c072b46u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c072b48; /* mov.l r13,@-r15 */
L8c072b48: *stop=0x8c072b48u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c072b4a; /* mov.l r12,@-r15 */
L8c072b4a: *stop=0x8c072b4au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c072b4c; /* mov.l r11,@-r15 */
L8c072b4c: *stop=0x8c072b4cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c072b4e; /* mov.l r10,@-r15 */
L8c072b4e: *stop=0x8c072b4eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c072b50; /* mov.l r9,@-r15 */
L8c072b50: *stop=0x8c072b50u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c072b52; /* mov.l r8,@-r15 */
L8c072b52: *stop=0x8c072b52u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c072b54; /* fmov.s fr15,@-r15 */
L8c072b54: *stop=0x8c072b54u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c072b56; /* fmov.s fr14,@-r15 */
L8c072b56: *stop=0x8c072b56u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c072b58; /* fmov.s fr13,@-r15 */
L8c072b58: *stop=0x8c072b58u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c072b5a; /* fmov.s fr12,@-r15 */
L8c072b5a: *stop=0x8c072b5au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c072b5c; /* sts.l PR,@-r15 */
L8c072b5c: *stop=0x8c072b5cu;r[15]+=4294967268u;goto L8c072b5e; /* add #-0x1c,r15 */
L8c072b5e: *stop=0x8c072b5eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c072b60; /* fldi0 fr15 */
L8c072b60: *stop=0x8c072b60u;r[0]=76u;goto L8c072b62; /* mov #0x4c,r0 */
L8c072b62: *stop=0x8c072b62u;r[14]=r[6];goto L8c072b64; /* mov r6,r14 */
L8c072b64: *stop=0x8c072b64u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c072b66; /* fmov fr15,@(r0,r14) */
L8c072b66: *stop=0x8c072b66u;r[0]=72u;goto L8c072b68; /* mov #0x48,r0 */
L8c072b68: *stop=0x8c072b68u;r[13]=r[4];goto L8c072b6a; /* mov r4,r13 */
L8c072b6a: *stop=0x8c072b6au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c072b6c; /* fmov fr15,@(r0,r14) */
L8c072b6c: *stop=0x8c072b6cu;r[0]=68u;goto L8c072b6e; /* mov #0x44,r0 */
L8c072b6e: *stop=0x8c072b6eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c072b70; /* fmov fr15,@(r0,r14) */
L8c072b70: *stop=0x8c072b70u;r[0]=92u;goto L8c072b72; /* mov #0x5c,r0 */
L8c072b72: *stop=0x8c072b72u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c072b74; /* mov.l @(r0,r14),r3 */
L8c072b74: *stop=0x8c072b74u;r[0]=96u;goto L8c072b76; /* mov #0x60,r0 */
L8c072b76: *stop=0x8c072b76u;r[11]=0u;goto L8c072b78; /* mov #0x0,r11 */
L8c072b78: *stop=0x8c072b78u;r[12]=r[5];goto L8c072b7a; /* mov r5,r12 */
L8c072b7a: *stop=0x8c072b7au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c072b7c; /* mov.l r3,@(r0,r14) */
L8c072b7c: *stop=0x8c072b7cu;r[0]=92u;goto L8c072b7e; /* mov #0x5c,r0 */
L8c072b7e: *stop=0x8c072b7eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[11],4);if(io_status)return io_status;goto L8c072b80; /* mov.l r11,@(r0,r14) */
L8c072b80: *stop=0x8c072b80u;r[8]=2u;goto L8c072b82; /* mov #0x2,r8 */
L8c072b82: *stop=0x8c072b82u;if(!valid_access(r[13]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x38u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c072b84; /* mov.l @(0x38,r13),r4 */
L8c072b84: *stop=0x8c072b84u;r[3]=r[8];goto L8c072b86; /* mov r8,r3 */
L8c072b86: *stop=0x8c072b86u;r[3]+=126u;goto L8c072b88; /* add #0x7e,r3 */
L8c072b88: *stop=0x8c072b88u;if(!valid_access(r[4]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c072b8a; /* mov.w @(0x2,r4),r0 */
L8c072b8a: *stop=0x8c072b8au;r[5]=r[0]&65535u;goto L8c072b8c; /* extu.w r0,r5 */
L8c072b8c: *stop=0x8c072b8cu;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c072b8e; /* tst r5,r3 */
L8c072b8e: take=!(*sr&1u);*stop=0x8c072b90u;r[10]=1u;if(take)goto L8c072b96;goto L8c072b92; /* bf/s 0x8c072b96 */
L8c072b92: *stop=0x8c072b94u;goto L8c072f80; /* bra 0x8c072f80 */
L8c072b96: *stop=0x8c072b96u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c072b98; /* mov.w @(0x4,r13),r0 */
L8c072b98: *stop=0x8c072b98u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c072b9a; /* tst #0x4,r0 */
L8c072b9a: if((*sr&1u))goto L8c072ba0;goto L8c072b9c; /* bt 0x8c072ba0 */
L8c072b9c: *stop=0x8c072b9eu;goto L8c072f80; /* bra 0x8c072f80 */
L8c072ba0: *stop=0x8c072ba0u;r[2]=8u;goto L8c072ba2; /* mov #0x8,r2 */
L8c072ba2: *stop=0x8c072ba2u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c072ba4; /* tst r5,r2 */
L8c072ba4: if(!(*sr&1u))goto L8c072baa;goto L8c072ba6; /* bf 0x8c072baa */
L8c072ba6: *stop=0x8c072ba8u;goto L8c072f80; /* bra 0x8c072f80 */
L8c072baa: *stop=0x8c072baau;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c072bac; /* mov.l @(0x18,r4),r0 */
L8c072bac: *stop=0x8c072bacu;r[3]=52u;goto L8c072bae; /* mov #0x34,r3 */
L8c072bae: *stop=0x8c072baeu;if(!valid_access(r[0]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072bb0; /* mov.l @(0xc,r0),r2 */
L8c072bb0: *stop=0x8c072bb0u;if(!valid_access(r[4]+22u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x16u,2);if(io_status)return io_status;goto L8c072bb2; /* mov.w @(0x16,r4),r0 */
L8c072bb2: *stop=0x8c072bb2u;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[0]);goto L8c072bb4; /* muls.w r3,r0 */
L8c072bb4: *stop=0x8c072bb4u;r[0]=*macl;goto L8c072bb6; /* sts MACL,r0 */
L8c072bb6: *stop=0x8c072bb6u;r[0]+=r[2];goto L8c072bb8; /* add r2,r0 */
L8c072bb8: *stop=0x8c072bb8u;if(!valid_access(r[0]+5u,1u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x5u,1);if(io_status)return io_status;goto L8c072bba; /* mov.b @(0x5,r0),r0 */
L8c072bba: *stop=0x8c072bbau;r[0]=r[0]&255u;goto L8c072bbc; /* extu.b r0,r0 */
L8c072bbc: *stop=0x8c072bbcu;*sr=(*sr&~1u)|!(r[0]&0xfu);goto L8c072bbe; /* tst #0xf,r0 */
L8c072bbe: if(!(*sr&1u))goto L8c072bc4;goto L8c072bc0; /* bf 0x8c072bc4 */
L8c072bc0: *stop=0x8c072bc2u;goto L8c072f80; /* bra 0x8c072f80 */
L8c072bc4: *stop=0x8c072bc4u;if(!valid_access(r[13]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x38u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c072bc6; /* mov.l @(0x38,r13),r4 */
L8c072bc6: *stop=0x8c072bc6u;r[9]=52u;goto L8c072bc8; /* mov #0x34,r9 */
L8c072bc8: *stop=0x8c072bc8u;r[2]=r[15];goto L8c072bca; /* mov r15,r2 */
L8c072bca: *stop=0x8c072bcau;r[2]+=16u;goto L8c072bcc; /* add #0x10,r2 */
L8c072bcc: *stop=0x8c072bccu;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072bce; /* mov.l @(0xc,r4),r3 */
L8c072bce: *stop=0x8c072bceu;r[5]=r[15];goto L8c072bd0; /* mov r15,r5 */
L8c072bd0: *stop=0x8c072bd0u;r[5]+=4u;goto L8c072bd2; /* add #0x4,r5 */
L8c072bd2: *stop=0x8c072bd2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c072bd4; /* mov.l r3,@r15 */
L8c072bd4: *stop=0x8c072bd4u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c072bd6; /* mov.l @(0x18,r4),r4 */
L8c072bd6: *stop=0x8c072bd6u;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072bd8; /* mov.l @(0x4,r12),r1 */
L8c072bd8: *stop=0x8c072bd8u;if(!valid_access(r[4]+6u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x6u,2);if(io_status)return io_status;goto L8c072bda; /* mov.w @(0x6,r4),r0 */
L8c072bda: *stop=0x8c072bdau;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072bdc; /* mov.l @(0xc,r4),r3 */
L8c072bdc: *stop=0x8c072bdcu;*macl=(r[9]&65535u)*(r[0]&65535u);goto L8c072bde; /* mulu.w r9,r0 */
L8c072bde: *stop=0x8c072bdeu;if(!valid_access(r[12]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c072be0; /* mov.l @(0x8,r12),r0 */
L8c072be0: *stop=0x8c072be0u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c072be2; /* mov.l r1,@r2 */
L8c072be2: *stop=0x8c072be2u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c072be4; /* mov.l r0,@(0x4,r2) */
L8c072be4: *stop=0x8c072be4u;r[9]=*macl;goto L8c072be6; /* sts MACL,r9 */
L8c072be6: *stop=0x8c072be6u;if(!valid_access(r[12]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072be8; /* mov.l @(0xc,r12),r1 */
L8c072be8: *stop=0x8c072be8u;r[9]+=r[3];goto L8c072bea; /* add r3,r9 */
L8c072bea: *stop=0x8c072beau;{uint32_t loaded=rd(m,bus,&io_status,0x8c072c74u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072bec; /* mov.l 0x8c072c74,r3 */
L8c072bec: *stop=0x8c072becu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c072bee; /* mov.l r1,@(0x8,r2) */
L8c072bee: target=r[3];*pr=0x8c072bf2u;*stop=0x8c072bf0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072bf2; /* jsr @r3 */
L8c072bf2: *stop=0x8c072bf2u;r[2]=r[15];goto L8c072bf4; /* mov r15,r2 */
L8c072bf4: *stop=0x8c072bf4u;r[2]+=4u;goto L8c072bf6; /* add #0x4,r2 */
L8c072bf6: *stop=0x8c072bf6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c072bf8; /* mov.l @r2,r1 */
L8c072bf8: *stop=0x8c072bf8u;r[3]=r[14];goto L8c072bfa; /* mov r14,r3 */
L8c072bfa: *stop=0x8c072bfau;r[3]+=68u;goto L8c072bfc; /* add #0x44,r3 */
L8c072bfc: *stop=0x8c072bfcu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c072bfe; /* mov.l @(0x4,r2),r0 */
L8c072bfe: *stop=0x8c072bfeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c072c00; /* mov.l r1,@r3 */
L8c072c00: *stop=0x8c072c00u;r[4]=r[15];goto L8c072c02; /* mov r15,r4 */
L8c072c02: *stop=0x8c072c02u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c072c04; /* mov.l r0,@(0x4,r3) */
L8c072c04: *stop=0x8c072c04u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072c06; /* mov.l @(0x8,r2),r1 */
L8c072c06: *stop=0x8c072c06u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c072c08; /* mov.l r1,@(0x8,r3) */
L8c072c08: *stop=0x8c072c08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072c78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072c0a; /* mov.l 0x8c072c78,r3 */
L8c072c0a: target=r[3];*pr=0x8c072c0eu;*stop=0x8c072c0cu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072c0e; /* jsr @r3 */
L8c072c0e: *stop=0x8c072c0eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c072c10; /* fldi0 fr3 */
L8c072c10: *stop=0x8c072c10u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c072c12; /* fcmp/eq fr3,fr0 */
L8c072c12: if(!(*sr&1u))goto L8c072c22;goto L8c072c14; /* bf 0x8c072c22 */
L8c072c14: *stop=0x8c072c14u;r[0]=4u;goto L8c072c16; /* mov #0x4,r0 */
L8c072c16: *stop=0x8c072c16u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072c18; /* fmov fr15,@(r0,r15) */
L8c072c18: *stop=0x8c072c18u;r[0]=8u;goto L8c072c1a; /* mov #0x8,r0 */
L8c072c1a: *stop=0x8c072c1au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072c1c; /* fmov fr15,@(r0,r15) */
L8c072c1c: *stop=0x8c072c1cu;r[0]=12u;goto L8c072c1e; /* mov #0xc,r0 */
L8c072c1e: *stop=0x8c072c20u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072c2a; /* bra 0x8c072c2a */
L8c072c22: *stop=0x8c072c22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072c7cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072c24; /* mov.l 0x8c072c7c,r2 */
L8c072c24: *stop=0x8c072c24u;r[4]=r[15];goto L8c072c26; /* mov r15,r4 */
L8c072c26: target=r[2];*pr=0x8c072c2au;*stop=0x8c072c28u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072c2a; /* jsr @r2 */
L8c072c2a: *stop=0x8c072c2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c072c78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072c2c; /* mov.l 0x8c072c78,r3 */
L8c072c2c: *stop=0x8c072c2cu;r[4]=r[15];goto L8c072c2e; /* mov r15,r4 */
L8c072c2e: target=r[3];*pr=0x8c072c32u;*stop=0x8c072c30u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072c32; /* jsr @r3 */
L8c072c32: *stop=0x8c072c32u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c072c34; /* fldi0 fr3 */
L8c072c34: *stop=0x8c072c34u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c072c36; /* fcmp/eq fr3,fr0 */
L8c072c36: if(!(*sr&1u))goto L8c072c80;goto L8c072c38; /* bf 0x8c072c80 */
L8c072c38: *stop=0x8c072c38u;r[0]=16u;goto L8c072c3a; /* mov #0x10,r0 */
L8c072c3a: *stop=0x8c072c3au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072c3c; /* fmov fr15,@(r0,r15) */
L8c072c3c: *stop=0x8c072c3cu;r[0]=20u;goto L8c072c3e; /* mov #0x14,r0 */
L8c072c3e: *stop=0x8c072c3eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072c40; /* fmov fr15,@(r0,r15) */
L8c072c40: *stop=0x8c072c40u;r[0]=24u;goto L8c072c42; /* mov #0x18,r0 */
L8c072c42: *stop=0x8c072c44u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072c88; /* bra 0x8c072c88 */
L8c072c80: *stop=0x8c072c80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e40u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072c82; /* mov.l 0x8c072e40,r2 */
L8c072c82: *stop=0x8c072c82u;r[4]=r[15];goto L8c072c84; /* mov r15,r4 */
L8c072c84: target=r[2];*pr=0x8c072c88u;*stop=0x8c072c86u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072c88; /* jsr @r2 */
L8c072c88: *stop=0x8c072c88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072c8a; /* mov.l 0x8c072e44,r3 */
L8c072c8a: *stop=0x8c072c8au;r[5]=r[15];goto L8c072c8c; /* mov r15,r5 */
L8c072c8c: *stop=0x8c072c8cu;r[4]=r[15];goto L8c072c8e; /* mov r15,r4 */
L8c072c8e: *stop=0x8c072c8eu;r[5]+=4u;goto L8c072c90; /* add #0x4,r5 */
L8c072c90: target=r[3];*pr=0x8c072c94u;*stop=0x8c072c92u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072c94; /* jsr @r3 */
L8c072c94: *stop=0x8c072c94u;r[0]=0x8c072e48u;goto L8c072c96; /* mova 0x8c072e48,r0 */
L8c072c96: *stop=0x8c072c96u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c072c98; /* fmov.s @r0,fr3 */
L8c072c98: *stop=0x8c072c98u;r[0]=56u;goto L8c072c9a; /* mov #0x38,r0 */
L8c072c9a: *stop=0x8c072c9au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072c9c; /* fmov.s @(r0,r14),fr2 */
L8c072c9c: *stop=0x8c072c9cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c072c9e; /* fcmp/gt fr2,fr3 */
L8c072c9e: take=(*sr&1u);*stop=0x8c072ca0u;fcopy(f,4u,0u);if(take)goto L8c072cae;goto L8c072ca2; /* bt/s 0x8c072cae */
L8c072ca2: *stop=0x8c072ca2u;r[5]=r[14];goto L8c072ca4; /* mov r14,r5 */
L8c072ca4: *stop=0x8c072ca4u;r[11]=r[8];goto L8c072ca6; /* mov r8,r11 */
L8c072ca6: target=0x8c07256au;*pr=0x8c072caau;*stop=0x8c072ca8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072caa; /* bsr 0x8c07256a */
L8c072caa: *stop=0x8c072cacu;goto L8c072fee; /* bra 0x8c072fee */
L8c072cae: *stop=0x8c072caeu;r[0]=4294967244u;goto L8c072cb0; /* mov #-0x34,r0 */
L8c072cb0: *stop=0x8c072cb0u;r[0]=r[0]&255u;goto L8c072cb2; /* extu.b r0,r0 */
L8c072cb2: *stop=0x8c072cb2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072cb4; /* fmov.s @(r0,r14),fr2 */
L8c072cb4: *stop=0x8c072cb4u;r[0]=56u;goto L8c072cb6; /* mov #0x38,r0 */
L8c072cb6: *stop=0x8c072cb6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c072cb8; /* fmov.s @(r0,r14),fr1 */
L8c072cb8: *stop=0x8c072cb8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c072cba; /* fcmp/gt fr1,fr2 */
L8c072cba: if((*sr&1u))goto L8c072cc8;goto L8c072cbc; /* bt 0x8c072cc8 */
L8c072cbc: *stop=0x8c072cbcu;r[0]=0x8c072e4cu;goto L8c072cbe; /* mova 0x8c072e4c,r0 */
L8c072cbe: *stop=0x8c072cbeu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c072cc0; /* fmov.s @r0,fr2 */
L8c072cc0: *stop=0x8c072cc0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c072cc2; /* fcmp/gt fr4,fr2 */
L8c072cc2: if(!(*sr&1u))goto L8c072cc8;goto L8c072cc4; /* bf 0x8c072cc8 */
L8c072cc4: *stop=0x8c072cc6u;r[11]=r[8];goto L8c072e26; /* bra 0x8c072e26 */
L8c072cc8: *stop=0x8c072cc8u;r[0]=4294967236u;goto L8c072cca; /* mov #-0x3c,r0 */
L8c072cca: *stop=0x8c072ccau;r[0]=r[0]&255u;goto L8c072ccc; /* extu.b r0,r0 */
L8c072ccc: *stop=0x8c072cccu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c072cce; /* fmov.s @(r0,r14),fr3 */
L8c072cce: *stop=0x8c072cceu;r[0]=56u;goto L8c072cd0; /* mov #0x38,r0 */
L8c072cd0: *stop=0x8c072cd0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072cd2; /* fmov.s @(r0,r14),fr2 */
L8c072cd2: *stop=0x8c072cd2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c072cd4; /* fcmp/gt fr2,fr3 */
L8c072cd4: if(!(*sr&1u))goto L8c072cda;goto L8c072cd6; /* bf 0x8c072cda */
L8c072cd6: *stop=0x8c072cd8u;goto L8c072e8c; /* bra 0x8c072e8c */
L8c072cda: *stop=0x8c072cdau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c072cdc; /* fldi0 fr3 */
L8c072cdc: *stop=0x8c072cdcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c072cde; /* fcmp/gt fr4,fr3 */
L8c072cde: if((*sr&1u))goto L8c072ce4;goto L8c072ce0; /* bt 0x8c072ce4 */
L8c072ce0: *stop=0x8c072ce2u;goto L8c072e8c; /* bra 0x8c072e8c */
L8c072ce4: *stop=0x8c072ce4u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c072ce6; /* mov.l @r9,r5 */
L8c072ce6: *stop=0x8c072ce6u;r[4]=15u;goto L8c072ce8; /* mov #0xf,r4 */
L8c072ce8: *stop=0x8c072ce8u;r[5]&=r[4];goto L8c072cea; /* and r4,r5 */
L8c072cea: *stop=0x8c072ceau;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c072cec; /* tst r5,r5 */
L8c072cec: take=(*sr&1u);*stop=0x8c072ceeu;r[11]=r[10];if(take)goto L8c072cf6;goto L8c072cf0; /* bt/s 0x8c072cf6 */
L8c072cf0: *stop=0x8c072cf0u;r[0]=r[5];goto L8c072cf2; /* mov r5,r0 */
L8c072cf2: *stop=0x8c072cf2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c072cf4; /* cmp/eq #0x1,r0 */
L8c072cf4: if(!(*sr&1u))goto L8c072ddc;goto L8c072cf6; /* bf 0x8c072ddc */
L8c072cf6: *stop=0x8c072cf6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c072cf8; /* mov.l @r15,r2 */
L8c072cf8: *stop=0x8c072cf8u;r[0]=20u;goto L8c072cfa; /* mov #0x14,r0 */
L8c072cfa: *stop=0x8c072cfau;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c072cfc; /* fmov.s @(r0,r9),fr2 */
L8c072cfc: *stop=0x8c072cfcu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c072cfe; /* fmov.s @(r0,r2),fr3 */
L8c072cfe: *stop=0x8c072cfeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c072d00; /* fcmp/gt fr3,fr2 */
L8c072d00: if(!(*sr&1u))goto L8c072ddc;goto L8c072d02; /* bf 0x8c072ddc */
L8c072d02: *stop=0x8c072d02u;if(!valid_access(r[9]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[9]+0x4u,1);if(io_status)return io_status;goto L8c072d04; /* mov.b @(0x4,r9),r0 */
L8c072d04: *stop=0x8c072d04u;r[0]=r[0]&255u;goto L8c072d06; /* extu.b r0,r0 */
L8c072d06: *stop=0x8c072d06u;r[0]&=0x3u;goto L8c072d08; /* and #0x3,r0 */
L8c072d08: *stop=0x8c072d08u;r[2]=r[0];goto L8c072d0a; /* mov r0,r2 */
L8c072d0a: *stop=0x8c072d0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c072d0c; /* mov.l @r15,r0 */
L8c072d0c: *stop=0x8c072d0cu;if(!valid_access(r[0]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x4u,1);if(io_status)return io_status;goto L8c072d0e; /* mov.b @(0x4,r0),r0 */
L8c072d0e: *stop=0x8c072d0eu;r[0]=r[0]&255u;goto L8c072d10; /* extu.b r0,r0 */
L8c072d10: *stop=0x8c072d10u;r[0]&=0xcu;goto L8c072d12; /* and #0xc,r0 */
L8c072d12: *stop=0x8c072d12u;*sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]&0x80000000u);goto L8c072d14; /* shar r0 */
L8c072d14: *stop=0x8c072d14u;*sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]&0x80000000u);goto L8c072d16; /* shar r0 */
L8c072d16: *stop=0x8c072d16u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[0]);goto L8c072d18; /* cmp/gt r0,r2 */
L8c072d18: if((*sr&1u))goto L8c072ddc;goto L8c072d1a; /* bt 0x8c072ddc */
L8c072d1a: *stop=0x8c072d1au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c072d1c; /* mov.l @r15,r2 */
L8c072d1c: *stop=0x8c072d1cu;r[0]=20u;goto L8c072d1e; /* mov #0x14,r0 */
L8c072d1e: *stop=0x8c072d1eu;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c072d20; /* fmov.s @(r0,r9),fr3 */
L8c072d20: *stop=0x8c072d20u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]+r[2]);if(io_status)return io_status;goto L8c072d22; /* fmov.s @(r0,r2),fr13 */
L8c072d22: *stop=0x8c072d22u;r[0]=16u;goto L8c072d24; /* mov #0x10,r0 */
L8c072d24: *stop=0x8c072d24u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]+r[15]);if(io_status)return io_status;goto L8c072d26; /* fmov.s @(r0,r15),fr14 */
L8c072d26: *stop=0x8c072d26u;r[0]=24u;goto L8c072d28; /* mov #0x18,r0 */
L8c072d28: *stop=0x8c072d28u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]+r[15]);if(io_status)return io_status;goto L8c072d2a; /* fmov.s @(r0,r15),fr12 */
L8c072d2a: *stop=0x8c072d2au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[3],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c072d2c; /* fadd fr3,fr13 */
L8c072d2c: *stop=0x8c072d2cu;fcopy(f,0u,14u);goto L8c072d2e; /* fmov fr14,fr0 */
L8c072d2e: *stop=0x8c072d2eu;fcopy(f,3u,12u);goto L8c072d30; /* fmov fr12,fr3 */
L8c072d30: *stop=0x8c072d30u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[12],&f->fpscr,&fr[3]);if(status)return status;}goto L8c072d32; /* fmul fr12,fr3 */
L8c072d32: *stop=0x8c072d32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e50u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072d34; /* mov.l 0x8c072e50,r3 */
L8c072d34: *stop=0x8c072d34u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[14],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c072d36; /* fmac fr0,fr14,fr3 */
L8c072d36: target=r[3];*pr=0x8c072d3au;*stop=0x8c072d38u;fcopy(f,4u,3u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072d3a; /* jsr @r3 */
L8c072d3a: *stop=0x8c072d3au;r[0]=0x8c072e54u;goto L8c072d3c; /* mova 0x8c072e54,r0 */
L8c072d3c: *stop=0x8c072d3cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072d3e; /* mov.l 0x8c072e58,r3 */
L8c072d3e: *stop=0x8c072d3eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c072d40; /* fmov.s @r0,fr14 */
L8c072d40: *stop=0x8c072d40u;fcopy(f,12u,0u);goto L8c072d42; /* fmov fr0,fr12 */
L8c072d42: *stop=0x8c072d42u;fcopy(f,4u,0u);goto L8c072d44; /* fmov fr0,fr4 */
L8c072d44: target=r[3];*pr=0x8c072d48u;*stop=0x8c072d46u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[13],&f->fpscr,&fr[4]);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072d48; /* jsr @r3 */
L8c072d48: *stop=0x8c072d48u;fcopy(f,3u,0u);goto L8c072d4a; /* fmov fr0,fr3 */
L8c072d4a: *stop=0x8c072d4au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c072d4c; /* fmul fr14,fr3 */
L8c072d4c: *stop=0x8c072d4cu;r[0]=4u;goto L8c072d4e; /* mov #0x4,r0 */
L8c072d4e: *stop=0x8c072d4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e5cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072d50; /* mov.l 0x8c072e5c,r3 */
L8c072d50: *stop=0x8c072d50u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c072d52; /* fmov.s @(r0,r15),fr5 */
L8c072d52: *stop=0x8c072d52u;r[0]=12u;goto L8c072d54; /* mov #0xc,r0 */
L8c072d54: *stop=0x8c072d54u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c072d56; /* ftrc fr3,FPUL */
L8c072d56: *stop=0x8c072d56u;r[9]=*fpul;goto L8c072d58; /* sts FPUL,r9 */
L8c072d58: target=r[3];*pr=0x8c072d5cu;*stop=0x8c072d5au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072d5c; /* jsr @r3 */
L8c072d5c: *stop=0x8c072d5cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c072d5e; /* fmul fr14,fr0 */
L8c072d5e: *stop=0x8c072d5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072d60; /* mov.l 0x8c072e60,r3 */
L8c072d60: *stop=0x8c072d60u;r[0]=16u;goto L8c072d62; /* mov #0x10,r0 */
L8c072d62: *stop=0x8c072d62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e5cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072d64; /* mov.l 0x8c072e5c,r1 */
L8c072d64: *stop=0x8c072d64u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c072d66; /* ftrc fr0,FPUL */
L8c072d66: *stop=0x8c072d66u;r[2]=*fpul;goto L8c072d68; /* sts FPUL,r2 */
L8c072d68: *stop=0x8c072d68u;r[2]+=r[3];goto L8c072d6a; /* add r3,r2 */
L8c072d6a: *stop=0x8c072d6au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c072d6c; /* mov.l r2,@r15 */
L8c072d6c: *stop=0x8c072d6cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c072d6e; /* fmov.s @(r0,r15),fr5 */
L8c072d6e: *stop=0x8c072d6eu;r[0]=24u;goto L8c072d70; /* mov #0x18,r0 */
L8c072d70: target=r[1];*pr=0x8c072d74u;*stop=0x8c072d72u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072d74; /* jsr @r1 */
L8c072d74: *stop=0x8c072d74u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c072d76; /* fmul fr14,fr0 */
L8c072d76: *stop=0x8c072d76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072d78; /* mov.l 0x8c072e64,r3 */
L8c072d78: *stop=0x8c072d78u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c072d7a; /* ftrc fr0,FPUL */
L8c072d7a: *stop=0x8c072d7au;r[5]=*fpul;goto L8c072d7c; /* sts FPUL,r5 */
L8c072d7c: target=r[3];*pr=0x8c072d80u;*stop=0x8c072d7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072d80; /* jsr @r3 */
L8c072d80: *stop=0x8c072d80u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c072d82; /* cmp/pz r0 */
L8c072d82: if((*sr&1u))goto L8c072d8a;goto L8c072d84; /* bt 0x8c072d8a */
L8c072d84: *stop=0x8c072d84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072d86; /* mov.l 0x8c072e68,r3 */
L8c072d86: *stop=0x8c072d86u;r[3]-=r[9];goto L8c072d88; /* sub r9,r3 */
L8c072d88: *stop=0x8c072d88u;r[9]=r[3];goto L8c072d8a; /* mov r3,r9 */
L8c072d8a: *stop=0x8c072d8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072d8c; /* mov.l 0x8c072e6c,r2 */
L8c072d8c: target=r[2];*pr=0x8c072d90u;*stop=0x8c072d8eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072d90; /* jsr @r2 */
L8c072d90: *stop=0x8c072d90u;r[1]=0u-r[9];goto L8c072d92; /* neg r9,r1 */
L8c072d92: *stop=0x8c072d92u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c072d94; /* tst r1,r1 */
L8c072d94: take=(*sr&1u);*stop=0x8c072d96u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(take)goto L8c072da0;goto L8c072d98; /* bt/s 0x8c072da0 */
L8c072d98: *stop=0x8c072d98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e70u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072d9a; /* mov.l 0x8c072e70,r2 */
L8c072d9a: *stop=0x8c072d9au;r[5]=r[1];goto L8c072d9c; /* mov r1,r5 */
L8c072d9c: target=r[2];*pr=0x8c072da0u;*stop=0x8c072d9eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072da0; /* jsr @r2 */
L8c072da0: *stop=0x8c072da0u;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072da2; /* mov.l @(0x4,r12),r3 */
L8c072da2: *stop=0x8c072da2u;r[2]=r[15];goto L8c072da4; /* mov r15,r2 */
L8c072da4: *stop=0x8c072da4u;r[2]+=16u;goto L8c072da6; /* add #0x10,r2 */
L8c072da6: *stop=0x8c072da6u;if(!valid_access(r[12]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072da8; /* mov.l @(0x8,r12),r1 */
L8c072da8: *stop=0x8c072da8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c072daa; /* mov.l r3,@r2 */
L8c072daa: *stop=0x8c072daau;r[5]=r[15];goto L8c072dac; /* mov r15,r5 */
L8c072dac: *stop=0x8c072dacu;r[6]=r[15];goto L8c072dae; /* mov r15,r6 */
L8c072dae: *stop=0x8c072daeu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c072db0; /* mov.l r1,@(0x4,r2) */
L8c072db0: *stop=0x8c072db0u;if(!valid_access(r[12]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072db2; /* mov.l @(0xc,r12),r3 */
L8c072db2: *stop=0x8c072db2u;r[5]+=16u;goto L8c072db4; /* add #0x10,r5 */
L8c072db4: *stop=0x8c072db4u;r[6]+=4u;goto L8c072db6; /* add #0x4,r6 */
L8c072db6: *stop=0x8c072db6u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c072db8; /* mov.l r3,@(0x8,r2) */
L8c072db8: *stop=0x8c072db8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e74u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072dba; /* mov.l 0x8c072e74,r3 */
L8c072dba: target=r[3];*pr=0x8c072dbeu;*stop=0x8c072dbcu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072dbe; /* jsr @r3 */
L8c072dbe: *stop=0x8c072dbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c072e78u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072dc0; /* mov.l 0x8c072e78,r2 */
L8c072dc0: target=r[2];*pr=0x8c072dc4u;*stop=0x8c072dc2u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072dc4; /* jsr @r2 */
L8c072dc4: *stop=0x8c072dc4u;r[0]=4u;goto L8c072dc6; /* mov #0x4,r0 */
L8c072dc6: *stop=0x8c072dc6u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072dc8; /* mov.l @(0x18,r13),r3 */
L8c072dc8: *stop=0x8c072dc8u;r[3]+=r[9];goto L8c072dca; /* add r9,r3 */
L8c072dca: *stop=0x8c072dcau;if(!valid_access(r[13]+24u,4u))return -3;wr(m,bus,&io_status,r[13]+0x18u,r[3],4);if(io_status)return io_status;goto L8c072dcc; /* mov.l r3,@(0x18,r13) */
L8c072dcc: *stop=0x8c072dccu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072dce; /* fmov.s @(r0,r15),fr3 */
L8c072dce: *stop=0x8c072dceu;r[0]=4u;goto L8c072dd0; /* mov #0x4,r0 */
L8c072dd0: *stop=0x8c072dd0u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c072dd2; /* fmov fr3,@(r0,r12) */
L8c072dd2: *stop=0x8c072dd2u;r[0]=12u;goto L8c072dd4; /* mov #0xc,r0 */
L8c072dd4: *stop=0x8c072dd4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072dd6; /* fmov.s @(r0,r15),fr3 */
L8c072dd6: *stop=0x8c072dd6u;r[0]=12u;goto L8c072dd8; /* mov #0xc,r0 */
L8c072dd8: *stop=0x8c072ddau;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c072fee; /* bra 0x8c072fee */
L8c072ddc: *stop=0x8c072ddcu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c072dde; /* mov.l @r9,r5 */
L8c072dde: *stop=0x8c072ddeu;r[5]&=r[4];goto L8c072de0; /* and r4,r5 */
L8c072de0: *stop=0x8c072de0u;r[0]=r[5];goto L8c072de2; /* mov r5,r0 */
L8c072de2: *stop=0x8c072de2u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c072de4; /* cmp/eq #0x4,r0 */
L8c072de4: if((*sr&1u))goto L8c072dea;goto L8c072de6; /* bt 0x8c072dea */
L8c072de6: *stop=0x8c072de8u;goto L8c072fee; /* bra 0x8c072fee */
L8c072dea: *stop=0x8c072deau;r[0]=r[5];goto L8c072dec; /* mov r5,r0 */
L8c072dec: *stop=0x8c072decu;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c072dee; /* cmp/eq #0x5,r0 */
L8c072dee: if((*sr&1u))goto L8c072df4;goto L8c072df0; /* bt 0x8c072df4 */
L8c072df0: *stop=0x8c072df2u;goto L8c072fee; /* bra 0x8c072fee */
L8c072df4: *stop=0x8c072df4u;if(!valid_access(r[13]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x38u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c072df6; /* mov.l @(0x38,r13),r4 */
L8c072df6: *stop=0x8c072df6u;r[3]=4294967168u;goto L8c072df8; /* mov #-0x80,r3 */
L8c072df8: *stop=0x8c072df8u;r[3]=r[3]&255u;goto L8c072dfa; /* extu.b r3,r3 */
L8c072dfa: *stop=0x8c072dfau;if(!valid_access(r[4]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c072dfc; /* mov.w @(0x2,r4),r0 */
L8c072dfc: *stop=0x8c072dfcu;r[5]=r[0]&65535u;goto L8c072dfe; /* extu.w r0,r5 */
L8c072dfe: *stop=0x8c072dfeu;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c072e00; /* tst r5,r3 */
L8c072e00: if((*sr&1u))goto L8c072e80;goto L8c072e02; /* bt 0x8c072e80 */
L8c072e02: *stop=0x8c072e02u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c072e04; /* mov.w @(0x4,r13),r0 */
L8c072e04: *stop=0x8c072e04u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c072e06; /* tst #0x4,r0 */
L8c072e06: if(!(*sr&1u))goto L8c072e80;goto L8c072e08; /* bf 0x8c072e80 */
L8c072e08: *stop=0x8c072e08u;if(!valid_access(r[4]+22u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x16u,2);if(io_status)return io_status;goto L8c072e0a; /* mov.w @(0x16,r4),r0 */
L8c072e0a: *stop=0x8c072e0au;r[3]=52u;goto L8c072e0c; /* mov #0x34,r3 */
L8c072e0c: *stop=0x8c072e0cu;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072e0e; /* mov.l @(0x18,r4),r2 */
L8c072e0e: *stop=0x8c072e0eu;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[0]);goto L8c072e10; /* muls.w r3,r0 */
L8c072e10: *stop=0x8c072e10u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072e12; /* mov.l @(0xc,r2),r2 */
L8c072e12: *stop=0x8c072e12u;r[0]=*macl;goto L8c072e14; /* sts MACL,r0 */
L8c072e14: *stop=0x8c072e14u;r[0]+=r[2];goto L8c072e16; /* add r2,r0 */
L8c072e16: *stop=0x8c072e16u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c072e18; /* mov.l @r0,r1 */
L8c072e18: *stop=0x8c072e18u;r[2]=8u;goto L8c072e1a; /* mov #0x8,r2 */
L8c072e1a: *stop=0x8c072e1au;r[2]<<=8;goto L8c072e1c; /* shll8 r2 */
L8c072e1c: *stop=0x8c072e1cu;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c072e1e; /* tst r2,r1 */
L8c072e1e: if(!(*sr&1u))goto L8c072e80;goto L8c072e20; /* bf 0x8c072e80 */
L8c072e20: *stop=0x8c072e20u;r[1]=8u;goto L8c072e22; /* mov #0x8,r1 */
L8c072e22: *stop=0x8c072e22u;*sr=(*sr&~1u)|!(r[5]&r[1]);goto L8c072e24; /* tst r5,r1 */
L8c072e24: if((*sr&1u))goto L8c072e80;goto L8c072e26; /* bt 0x8c072e80 */
L8c072e26: *stop=0x8c072e26u;r[0]=12u;goto L8c072e28; /* mov #0xc,r0 */
L8c072e28: *stop=0x8c072e28u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c072e2a; /* fmov fr15,@(r0,r12) */
L8c072e2a: *stop=0x8c072e2au;r[0]=4u;goto L8c072e2c; /* mov #0x4,r0 */
L8c072e2c: *stop=0x8c072e2eu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c072fee; /* bra 0x8c072fee */
L8c072e80: *stop=0x8c072e80u;r[5]=r[12];goto L8c072e82; /* mov r12,r5 */
L8c072e82: *stop=0x8c072e82u;r[6]=r[14];goto L8c072e84; /* mov r14,r6 */
L8c072e84: target=0x8c072946u;*pr=0x8c072e88u;*stop=0x8c072e86u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072e88; /* bsr 0x8c072946 */
L8c072e88: *stop=0x8c072e8au;goto L8c072fee; /* bra 0x8c072fee */
L8c072e8c: *stop=0x8c072e8cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c072e8e; /* fldi0 fr3 */
L8c072e8e: *stop=0x8c072e8eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c072e90; /* fcmp/gt fr4,fr3 */
L8c072e90: take=(*sr&1u);*stop=0x8c072e92u;r[11]=r[10];if(take)goto L8c072e98;goto L8c072e94; /* bt/s 0x8c072e98 */
L8c072e94: *stop=0x8c072e96u;goto L8c072fee; /* bra 0x8c072fee */
L8c072e98: *stop=0x8c072e98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072f60u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072e9a; /* mov.l 0x8c072f60,r2 */
L8c072e9a: target=r[2];*pr=0x8c072e9eu;*stop=0x8c072e9cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072e9e; /* jsr @r2 */
L8c072e9e: *stop=0x8c072e9eu;r[3]=4u;goto L8c072ea0; /* mov #0x4,r3 */
L8c072ea0: *stop=0x8c072ea0u;r[3]<<=8;goto L8c072ea2; /* shll8 r3 */
L8c072ea2: *stop=0x8c072ea2u;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c072ea4; /* tst r3,r0 */
L8c072ea4: if((*sr&1u))goto L8c072ec4;goto L8c072ea6; /* bt 0x8c072ec4 */
L8c072ea6: *stop=0x8c072ea6u;r[0]=4u;goto L8c072ea8; /* mov #0x4,r0 */
L8c072ea8: *stop=0x8c072ea8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072eaa; /* fmov.s @(r0,r15),fr3 */
L8c072eaa: *stop=0x8c072eaau;r[0]=56u;goto L8c072eac; /* mov #0x38,r0 */
L8c072eac: *stop=0x8c072eacu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072eae; /* fmov.s @(r0,r14),fr2 */
L8c072eae: *stop=0x8c072eaeu;r[0]=4u;goto L8c072eb0; /* mov #0x4,r0 */
L8c072eb0: *stop=0x8c072eb0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c072eb2; /* fmul fr3,fr2 */
L8c072eb2: *stop=0x8c072eb2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c072eb4; /* fneg fr2 */
L8c072eb4: *stop=0x8c072eb4u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c072eb6; /* fmov fr2,@(r0,r12) */
L8c072eb6: *stop=0x8c072eb6u;r[0]=12u;goto L8c072eb8; /* mov #0xc,r0 */
L8c072eb8: *stop=0x8c072eb8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072eba; /* fmov.s @(r0,r15),fr3 */
L8c072eba: *stop=0x8c072ebau;r[0]=56u;goto L8c072ebc; /* mov #0x38,r0 */
L8c072ebc: *stop=0x8c072ebcu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072ebe; /* fmov.s @(r0,r14),fr2 */
L8c072ebe: *stop=0x8c072ebeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c072ec0; /* fmul fr3,fr2 */
L8c072ec0: *stop=0x8c072ec2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c072f44; /* bra 0x8c072f44 */
L8c072ec4: *stop=0x8c072ec4u;if(!valid_access(r[13]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x38u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c072ec6; /* mov.l @(0x38,r13),r4 */
L8c072ec6: *stop=0x8c072ec6u;r[2]=4294967168u;goto L8c072ec8; /* mov #-0x80,r2 */
L8c072ec8: *stop=0x8c072ec8u;r[2]=r[2]&255u;goto L8c072eca; /* extu.b r2,r2 */
L8c072eca: *stop=0x8c072ecau;if(!valid_access(r[4]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c072ecc; /* mov.w @(0x2,r4),r0 */
L8c072ecc: *stop=0x8c072eccu;r[5]=r[0]&65535u;goto L8c072ece; /* extu.w r0,r5 */
L8c072ece: *stop=0x8c072eceu;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c072ed0; /* tst r5,r2 */
L8c072ed0: if((*sr&1u))goto L8c072f54;goto L8c072ed2; /* bt 0x8c072f54 */
L8c072ed2: *stop=0x8c072ed2u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c072ed4; /* mov.w @(0x4,r13),r0 */
L8c072ed4: *stop=0x8c072ed4u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c072ed6; /* tst #0x4,r0 */
L8c072ed6: if(!(*sr&1u))goto L8c072f54;goto L8c072ed8; /* bf 0x8c072f54 */
L8c072ed8: *stop=0x8c072ed8u;if(!valid_access(r[4]+22u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x16u,2);if(io_status)return io_status;goto L8c072eda; /* mov.w @(0x16,r4),r0 */
L8c072eda: *stop=0x8c072edau;r[2]=52u;goto L8c072edc; /* mov #0x34,r2 */
L8c072edc: *stop=0x8c072edcu;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072ede; /* mov.l @(0x18,r4),r1 */
L8c072ede: *stop=0x8c072edeu;*macl=(uint32_t)((int32_t)(int16_t)r[2]*(int32_t)(int16_t)r[0]);goto L8c072ee0; /* muls.w r2,r0 */
L8c072ee0: *stop=0x8c072ee0u;if(!valid_access(r[1]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072ee2; /* mov.l @(0xc,r1),r1 */
L8c072ee2: *stop=0x8c072ee2u;r[0]=*macl;goto L8c072ee4; /* sts MACL,r0 */
L8c072ee4: *stop=0x8c072ee4u;r[0]+=r[1];goto L8c072ee6; /* add r1,r0 */
L8c072ee6: *stop=0x8c072ee6u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c072ee8; /* mov.l @r0,r0 */
L8c072ee8: *stop=0x8c072ee8u;r[1]=8u;goto L8c072eea; /* mov #0x8,r1 */
L8c072eea: *stop=0x8c072eeau;r[1]<<=8;goto L8c072eec; /* shll8 r1 */
L8c072eec: *stop=0x8c072eecu;*sr=(*sr&~1u)|!(r[1]&r[0]);goto L8c072eee; /* tst r1,r0 */
L8c072eee: if(!(*sr&1u))goto L8c072f54;goto L8c072ef0; /* bf 0x8c072f54 */
L8c072ef0: *stop=0x8c072ef0u;r[3]=8u;goto L8c072ef2; /* mov #0x8,r3 */
L8c072ef2: *stop=0x8c072ef2u;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c072ef4; /* tst r5,r3 */
L8c072ef4: if((*sr&1u))goto L8c072f54;goto L8c072ef6; /* bt 0x8c072f54 */
L8c072ef6: *stop=0x8c072ef6u;r[0]=4u;goto L8c072ef8; /* mov #0x4,r0 */
L8c072ef8: *stop=0x8c072ef8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072f64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072efa; /* mov.l 0x8c072f64,r3 */
L8c072efa: *stop=0x8c072efau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072efc; /* fmov.s @(r0,r15),fr3 */
L8c072efc: *stop=0x8c072efcu;r[0]=16u;goto L8c072efe; /* mov #0x10,r0 */
L8c072efe: *stop=0x8c072efeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c072f00; /* fmov.s @(r0,r15),fr2 */
L8c072f00: *stop=0x8c072f00u;r[0]=16u;goto L8c072f02; /* mov #0x10,r0 */
L8c072f02: *stop=0x8c072f02u;r[4]=r[15];goto L8c072f04; /* mov r15,r4 */
L8c072f04: *stop=0x8c072f04u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c072f06; /* fadd fr3,fr2 */
L8c072f06: *stop=0x8c072f06u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c072f08; /* fmov fr2,@(r0,r15) */
L8c072f08: *stop=0x8c072f08u;r[0]=20u;goto L8c072f0a; /* mov #0x14,r0 */
L8c072f0a: *stop=0x8c072f0au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072f0c; /* fmov fr15,@(r0,r15) */
L8c072f0c: *stop=0x8c072f0cu;r[0]=12u;goto L8c072f0e; /* mov #0xc,r0 */
L8c072f0e: *stop=0x8c072f0eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072f10; /* fmov.s @(r0,r15),fr3 */
L8c072f10: *stop=0x8c072f10u;r[0]=24u;goto L8c072f12; /* mov #0x18,r0 */
L8c072f12: *stop=0x8c072f12u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c072f14; /* fmov.s @(r0,r15),fr2 */
L8c072f14: *stop=0x8c072f14u;r[0]=24u;goto L8c072f16; /* mov #0x18,r0 */
L8c072f16: *stop=0x8c072f16u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c072f18; /* fadd fr3,fr2 */
L8c072f18: *stop=0x8c072f18u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c072f1a; /* fmov fr2,@(r0,r15) */
L8c072f1a: target=r[3];*pr=0x8c072f1eu;*stop=0x8c072f1cu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072f1e; /* jsr @r3 */
L8c072f1e: *stop=0x8c072f1eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c072f20; /* fldi0 fr3 */
L8c072f20: *stop=0x8c072f20u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c072f22; /* fcmp/eq fr3,fr0 */
L8c072f22: if((*sr&1u))goto L8c072f4a;goto L8c072f24; /* bt 0x8c072f4a */
L8c072f24: *stop=0x8c072f24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072f68u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072f26; /* mov.l 0x8c072f68,r2 */
L8c072f26: *stop=0x8c072f26u;r[4]=r[15];goto L8c072f28; /* mov r15,r4 */
L8c072f28: target=r[2];*pr=0x8c072f2cu;*stop=0x8c072f2au;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072f2c; /* jsr @r2 */
L8c072f2c: *stop=0x8c072f2cu;r[0]=16u;goto L8c072f2e; /* mov #0x10,r0 */
L8c072f2e: *stop=0x8c072f2eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072f30; /* fmov.s @(r0,r15),fr3 */
L8c072f30: *stop=0x8c072f30u;r[0]=56u;goto L8c072f32; /* mov #0x38,r0 */
L8c072f32: *stop=0x8c072f32u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072f34; /* fmov.s @(r0,r14),fr2 */
L8c072f34: *stop=0x8c072f34u;r[0]=4u;goto L8c072f36; /* mov #0x4,r0 */
L8c072f36: *stop=0x8c072f36u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c072f38; /* fmul fr3,fr2 */
L8c072f38: *stop=0x8c072f38u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c072f3a; /* fmov fr2,@(r0,r12) */
L8c072f3a: *stop=0x8c072f3au;r[0]=24u;goto L8c072f3c; /* mov #0x18,r0 */
L8c072f3c: *stop=0x8c072f3cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072f3e; /* fmov.s @(r0,r15),fr3 */
L8c072f3e: *stop=0x8c072f3eu;r[0]=56u;goto L8c072f40; /* mov #0x38,r0 */
L8c072f40: *stop=0x8c072f40u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072f42; /* fmov.s @(r0,r14),fr2 */
L8c072f42: *stop=0x8c072f42u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c072f44; /* fmul fr3,fr2 */
L8c072f44: *stop=0x8c072f44u;r[0]=12u;goto L8c072f46; /* mov #0xc,r0 */
L8c072f46: *stop=0x8c072f48u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c072fee; /* bra 0x8c072fee */
L8c072f4a: *stop=0x8c072f4au;r[0]=4u;goto L8c072f4c; /* mov #0x4,r0 */
L8c072f4c: *stop=0x8c072f4cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c072f4e; /* fmov fr15,@(r0,r12) */
L8c072f4e: *stop=0x8c072f4eu;r[0]=12u;goto L8c072f50; /* mov #0xc,r0 */
L8c072f50: *stop=0x8c072f52u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c072fee; /* bra 0x8c072fee */
L8c072f54: *stop=0x8c072f54u;r[5]=r[12];goto L8c072f56; /* mov r12,r5 */
L8c072f56: *stop=0x8c072f56u;r[6]=r[14];goto L8c072f58; /* mov r14,r6 */
L8c072f58: target=0x8c072946u;*pr=0x8c072f5cu;*stop=0x8c072f5au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072f5c; /* bsr 0x8c072946 */
L8c072f5c: *stop=0x8c072f5eu;goto L8c072fee; /* bra 0x8c072fee */
L8c072f80: *stop=0x8c072f80u;if(!valid_access(r[13]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072f82; /* mov.l @(0x38,r13),r2 */
L8c072f82: *stop=0x8c072f82u;if(!valid_access(r[2]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c072f84; /* mov.w @(0x2,r2),r0 */
L8c072f84: *stop=0x8c072f84u;r[0]=r[0]&65535u;goto L8c072f86; /* extu.w r0,r0 */
L8c072f86: *stop=0x8c072f86u;*sr=(*sr&~1u)|!(r[8]&r[0]);goto L8c072f88; /* tst r8,r0 */
L8c072f88: if((*sr&1u))goto L8c072fee;goto L8c072f8a; /* bt 0x8c072fee */
L8c072f8a: *stop=0x8c072f8au;if(!valid_access(r[13]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x38u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c072f8c; /* mov.l @(0x38,r13),r4 */
L8c072f8c: *stop=0x8c072f8cu;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c072f8e; /* mov.l @(0x18,r4),r4 */
L8c072f8e: *stop=0x8c072f8eu;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072f90; /* mov.l @(0x10,r4),r3 */
L8c072f90: *stop=0x8c072f90u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c072f92; /* tst r3,r3 */
L8c072f92: if((*sr&1u))goto L8c072fee;goto L8c072f94; /* bt 0x8c072fee */
L8c072f94: *stop=0x8c072f94u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c072f96; /* mov.l @(0x10,r4),r9 */
L8c072f96: *stop=0x8c072f96u;if(!valid_access(r[9]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x20u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c072f98; /* mov.l @(0x20,r9),r9 */
L8c072f98: *stop=0x8c072f98u;*sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c072f9a; /* tst r9,r9 */
L8c072f9a: if((*sr&1u))goto L8c072fee;goto L8c072f9c; /* bt 0x8c072fee */
L8c072f9c: *stop=0x8c072f9cu;if(!valid_access(r[9]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[9]+0x2u,1);if(io_status)return io_status;goto L8c072f9e; /* mov.b @(0x2,r9),r0 */
L8c072f9e: *stop=0x8c072f9eu;r[0]=r[0]&255u;goto L8c072fa0; /* extu.b r0,r0 */
L8c072fa0: *stop=0x8c072fa0u;*sr=(*sr&~1u)|(r[0]==0x0000000cu);goto L8c072fa2; /* cmp/eq #0xc,r0 */
L8c072fa2: if(!(*sr&1u))goto L8c072fee;goto L8c072fa4; /* bf 0x8c072fee */
L8c072fa4: *stop=0x8c072fa4u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c072fa6; /* mov.w @(0x4,r13),r0 */
L8c072fa6: *stop=0x8c072fa6u;r[3]=r[0];goto L8c072fa8; /* mov r0,r3 */
L8c072fa8: *stop=0x8c072fa8u;*sr=(*sr&~1u)|!(r[10]&r[3]);goto L8c072faa; /* tst r10,r3 */
L8c072faa: take=(*sr&1u);*stop=0x8c072facu;r[4]=r[0];if(take)goto L8c072fee;goto L8c072fae; /* bt/s 0x8c072fee */
L8c072fae: *stop=0x8c072faeu;r[0]=60u;goto L8c072fb0; /* mov #0x3c,r0 */
L8c072fb0: *stop=0x8c072fb0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c072fb2; /* fldi0 fr3 */
L8c072fb2: *stop=0x8c072fb2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072fb4; /* fmov.s @(r0,r14),fr2 */
L8c072fb4: *stop=0x8c072fb4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c072fb6; /* fcmp/gt fr3,fr2 */
L8c072fb6: if((*sr&1u))goto L8c072fee;goto L8c072fb8; /* bt 0x8c072fee */
L8c072fb8: *stop=0x8c072fb8u;r[0]=4294967236u;goto L8c072fba; /* mov #-0x3c,r0 */
L8c072fba: *stop=0x8c072fbau;r[0]=r[0]&255u;goto L8c072fbc; /* extu.b r0,r0 */
L8c072fbc: *stop=0x8c072fbcu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072fbe; /* fmov.s @(r0,r14),fr2 */
L8c072fbe: *stop=0x8c072fbeu;r[0]=56u;goto L8c072fc0; /* mov #0x38,r0 */
L8c072fc0: *stop=0x8c072fc0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c072fc2; /* fmov.s @(r0,r14),fr1 */
L8c072fc2: *stop=0x8c072fc2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c072fc4; /* fcmp/gt fr1,fr2 */
L8c072fc4: if(!(*sr&1u))goto L8c072fce;goto L8c072fc6; /* bf 0x8c072fce */
L8c072fc6: *stop=0x8c072fc6u;r[2]=1u;goto L8c072fc8; /* mov #0x1,r2 */
L8c072fc8: *stop=0x8c072fc8u;r[2]<<=8;goto L8c072fca; /* shll8 r2 */
L8c072fca: *stop=0x8c072fcau;*sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c072fcc; /* tst r4,r2 */
L8c072fcc: if((*sr&1u))goto L8c072fee;goto L8c072fce; /* bt 0x8c072fee */
L8c072fce: *stop=0x8c072fceu;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c072fd0; /* mov.l @(0x18,r13),r4 */
L8c072fd0: *stop=0x8c072fd0u;r[3]=64u;goto L8c072fd2; /* mov #0x40,r3 */
L8c072fd2: *stop=0x8c072fd2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073120u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072fd4; /* mov.l 0x8c073120,r2 */
L8c072fd4: *stop=0x8c072fd4u;r[3]<<=8;goto L8c072fd6; /* shll8 r3 */
L8c072fd6: *stop=0x8c072fd6u;if(!valid_access(r[9]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c072fd8; /* mov.l @(0x18,r9),r5 */
L8c072fd8: *stop=0x8c072fd8u;r[4]=0u-r[4];goto L8c072fda; /* neg r4,r4 */
L8c072fda: target=r[2];*pr=0x8c072fdeu;*stop=0x8c072fdcu;r[4]+=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072fde; /* jsr @r2 */
L8c072fde: *stop=0x8c072fdeu;r[3]=48u;goto L8c072fe0; /* mov #0x30,r3 */
L8c072fe0: *stop=0x8c072fe0u;r[3]<<=8;goto L8c072fe2; /* shll8 r3 */
L8c072fe2: *stop=0x8c072fe2u;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]);goto L8c072fe4; /* cmp/ge r3,r0 */
L8c072fe4: if((*sr&1u))goto L8c072fee;goto L8c072fe6; /* bt 0x8c072fee */
L8c072fe6: *stop=0x8c072fe6u;r[5]=r[14];goto L8c072fe8; /* mov r14,r5 */
L8c072fe8: *stop=0x8c072fe8u;r[11]=3u;goto L8c072fea; /* mov #0x3,r11 */
L8c072fea: target=0x8c07256au;*pr=0x8c072feeu;*stop=0x8c072fecu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072fee; /* bsr 0x8c07256a */
L8c072fee: *stop=0x8c072feeu;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c072ff0; /* mov.w @(0x4,r13),r0 */
L8c072ff0: *stop=0x8c072ff0u;*sr=(*sr&~1u)|!(r[8]&r[0]);goto L8c072ff2; /* tst r8,r0 */
L8c072ff2: if((*sr&1u))goto L8c0730aa;goto L8c072ff4; /* bt 0x8c0730aa */
L8c072ff4: *stop=0x8c072ff4u;r[5]=r[12];goto L8c072ff6; /* mov r12,r5 */
L8c072ff6: *stop=0x8c072ff6u;r[6]=r[14];goto L8c072ff8; /* mov r14,r6 */
L8c072ff8: target=0x8c0730d8u;*pr=0x8c072ffcu;*stop=0x8c072ffau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072ffc; /* bsr 0x8c0730d8 */
L8c072ffc: *stop=0x8c072ffcu;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c072ffe; /* tst r11,r11 */
L8c072ffe: take=!(*sr&1u);*stop=0x8c073000u;r[4]=r[0];if(take)goto L8c073004;goto L8c073002; /* bf/s 0x8c073004 */
L8c073002: *stop=0x8c073002u;r[11]=r[4];goto L8c073004; /* mov r4,r11 */
L8c073004: *stop=0x8c073004u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c073006; /* mov.w @(0x4,r13),r0 */
L8c073006: *stop=0x8c073006u;*sr=(*sr&~1u)|!(r[10]&r[0]);goto L8c073008; /* tst r10,r0 */
L8c073008: if(!(*sr&1u))goto L8c0730ba;goto L8c07300a; /* bf 0x8c0730ba */
L8c07300a: *stop=0x8c07300au;{uint32_t loaded=rd(m,bus,&io_status,0x8c073124u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07300c; /* mov.l 0x8c073124,r2 */
L8c07300c: target=r[2];*pr=0x8c073010u;*stop=0x8c07300eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073010; /* jsr @r2 */
L8c073010: *stop=0x8c073010u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073012; /* mov.l @(0x1c,r13),r3 */
L8c073012: *stop=0x8c073012u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c073014; /* tst r3,r3 */
L8c073014: take=(*sr&1u);*stop=0x8c073016u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c073020;goto L8c073018; /* bt/s 0x8c073020 */
L8c073018: *stop=0x8c073018u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073128u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07301a; /* mov.l 0x8c073128,r3 */
L8c07301a: *stop=0x8c07301au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c07301c; /* mov.l @r15,r5 */
L8c07301c: target=r[3];*pr=0x8c073020u;*stop=0x8c07301eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073020; /* jsr @r3 */
L8c073020: *stop=0x8c073020u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073022; /* mov.l @(0x14,r13),r3 */
L8c073022: *stop=0x8c073022u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c073024; /* tst r3,r3 */
L8c073024: take=(*sr&1u);*stop=0x8c073026u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c073030;goto L8c073028; /* bt/s 0x8c073030 */
L8c073028: *stop=0x8c073028u;{uint32_t loaded=rd(m,bus,&io_status,0x8c07312cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07302a; /* mov.l 0x8c07312c,r3 */
L8c07302a: *stop=0x8c07302au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c07302c; /* mov.l @r15,r5 */
L8c07302c: target=r[3];*pr=0x8c073030u;*stop=0x8c07302eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073030; /* jsr @r3 */
L8c073030: *stop=0x8c073030u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073032; /* mov.l @(0x18,r13),r3 */
L8c073032: *stop=0x8c073032u;r[3]=0u-r[3];goto L8c073034; /* neg r3,r3 */
L8c073034: *stop=0x8c073034u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c073036; /* tst r3,r3 */
L8c073036: take=(*sr&1u);*stop=0x8c073038u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c073042;goto L8c07303a; /* bt/s 0x8c073042 */
L8c07303a: *stop=0x8c07303au;{uint32_t loaded=rd(m,bus,&io_status,0x8c073130u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07303c; /* mov.l 0x8c073130,r1 */
L8c07303c: *stop=0x8c07303cu;r[5]=r[3];goto L8c07303e; /* mov r3,r5 */
L8c07303e: target=r[1];*pr=0x8c073042u;*stop=0x8c073040u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073042; /* jsr @r1 */
L8c073042: *stop=0x8c073042u;r[0]=24u;goto L8c073044; /* mov #0x18,r0 */
L8c073044: *stop=0x8c073044u;r[6]=r[15];goto L8c073046; /* mov r15,r6 */
L8c073046: *stop=0x8c073046u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c073048; /* fmov fr15,@(r0,r15) */
L8c073048: *stop=0x8c073048u;r[0]=16u;goto L8c07304a; /* mov #0x10,r0 */
L8c07304a: *stop=0x8c07304au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c07304c; /* fmov fr15,@(r0,r15) */
L8c07304c: *stop=0x8c07304cu;r[5]=r[15];goto L8c07304e; /* mov r15,r5 */
L8c07304e: *stop=0x8c07304eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073134u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073050; /* mov.l 0x8c073134,r3 */
L8c073050: *stop=0x8c073050u;r[0]=20u;goto L8c073052; /* mov #0x14,r0 */
L8c073052: *stop=0x8c073052u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c073054; /* fldi1 fr3 */
L8c073054: *stop=0x8c073054u;r[5]+=16u;goto L8c073056; /* add #0x10,r5 */
L8c073056: *stop=0x8c073056u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073058; /* fmov fr3,@(r0,r15) */
L8c073058: *stop=0x8c073058u;r[6]+=4u;goto L8c07305a; /* add #0x4,r6 */
L8c07305a: target=r[3];*pr=0x8c07305eu;*stop=0x8c07305cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07305e; /* jsr @r3 */
L8c07305e: *stop=0x8c07305eu;r[2]=r[15];goto L8c073060; /* mov r15,r2 */
L8c073060: *stop=0x8c073060u;r[2]+=4u;goto L8c073062; /* add #0x4,r2 */
L8c073062: *stop=0x8c073062u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c073064; /* mov.l @(0x4,r2),r0 */
L8c073064: *stop=0x8c073064u;r[3]=r[14];goto L8c073066; /* mov r14,r3 */
L8c073066: *stop=0x8c073066u;r[3]+=80u;goto L8c073068; /* add #0x50,r3 */
L8c073068: *stop=0x8c073068u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c07306a; /* mov.l @r2,r1 */
L8c07306a: *stop=0x8c07306au;r[5]=r[15];goto L8c07306c; /* mov r15,r5 */
L8c07306c: *stop=0x8c07306cu;r[6]=r[15];goto L8c07306e; /* mov r15,r6 */
L8c07306e: *stop=0x8c07306eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c073070; /* mov.l r1,@r3 */
L8c073070: *stop=0x8c073070u;r[5]+=16u;goto L8c073072; /* add #0x10,r5 */
L8c073072: *stop=0x8c073072u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c073074; /* mov.l r0,@(0x4,r3) */
L8c073074: *stop=0x8c073074u;r[0]=24u;goto L8c073076; /* mov #0x18,r0 */
L8c073076: *stop=0x8c073076u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073078; /* mov.l @(0x8,r2),r1 */
L8c073078: *stop=0x8c073078u;r[6]+=4u;goto L8c07307a; /* add #0x4,r6 */
L8c07307a: *stop=0x8c07307au;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c07307c; /* mov.l r1,@(0x8,r3) */
L8c07307c: *stop=0x8c07307cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c07307e; /* fmov fr15,@(r0,r15) */
L8c07307e: *stop=0x8c07307eu;r[0]=16u;goto L8c073080; /* mov #0x10,r0 */
L8c073080: *stop=0x8c073080u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c073082; /* fmov fr15,@(r0,r15) */
L8c073082: *stop=0x8c073082u;r[0]=0x8c073138u;goto L8c073084; /* mova 0x8c073138,r0 */
L8c073084: *stop=0x8c073084u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c073086; /* fmov.s @r0,fr3 */
L8c073086: *stop=0x8c073086u;r[0]=20u;goto L8c073088; /* mov #0x14,r0 */
L8c073088: *stop=0x8c073088u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073134u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07308a; /* mov.l 0x8c073134,r3 */
L8c07308a: *stop=0x8c07308au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07308c; /* fmov fr3,@(r0,r15) */
L8c07308c: target=r[3];*pr=0x8c073090u;*stop=0x8c07308eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073090; /* jsr @r3 */
L8c073090: *stop=0x8c073090u;{uint32_t loaded=rd(m,bus,&io_status,0x8c07313cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073092; /* mov.l 0x8c07313c,r2 */
L8c073092: target=r[2];*pr=0x8c073096u;*stop=0x8c073094u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073096; /* jsr @r2 */
L8c073096: *stop=0x8c073096u;r[1]=r[15];goto L8c073098; /* mov r15,r1 */
L8c073098: *stop=0x8c073098u;r[1]+=8u;goto L8c07309a; /* add #0x8,r1 */
L8c07309a: *stop=0x8c07309au;r[0]=36u;goto L8c07309c; /* mov #0x24,r0 */
L8c07309c: *stop=0x8c07309cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c07309e; /* fmov.s @r1,fr3 */
L8c07309e: *stop=0x8c07309eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0730a0; /* fmov.s @(r0,r13),fr2 */
L8c0730a0: *stop=0x8c0730a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0730a2; /* fadd fr3,fr2 */
L8c0730a2: *stop=0x8c0730a2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0730a4; /* fmov fr2,@(r0,r13) */
L8c0730a4: *stop=0x8c0730a4u;r[0]=8u;goto L8c0730a6; /* mov #0x8,r0 */
L8c0730a6: *stop=0x8c0730a8u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c0730ba; /* bra 0x8c0730ba */
L8c0730aa: *stop=0x8c0730aau;r[5]=r[12];goto L8c0730ac; /* mov r12,r5 */
L8c0730ac: *stop=0x8c0730acu;r[6]=r[14];goto L8c0730ae; /* mov r14,r6 */
L8c0730ae: target=0x8c0730d8u;*pr=0x8c0730b2u;*stop=0x8c0730b0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0730b2; /* bsr 0x8c0730d8 */
L8c0730b2: *stop=0x8c0730b2u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c0730b4; /* tst r11,r11 */
L8c0730b4: take=!(*sr&1u);*stop=0x8c0730b6u;r[4]=r[0];if(take)goto L8c0730ba;goto L8c0730b8; /* bf/s 0x8c0730ba */
L8c0730b8: *stop=0x8c0730b8u;r[11]=r[4];goto L8c0730ba; /* mov r4,r11 */
L8c0730ba: *stop=0x8c0730bau;r[0]=r[11];goto L8c0730bc; /* mov r11,r0 */
L8c0730bc: *stop=0x8c0730bcu;r[15]+=28u;goto L8c0730be; /* add #0x1c,r15 */
L8c0730be: *stop=0x8c0730beu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0730c0; /* lds.l @r15+,PR */
L8c0730c0: *stop=0x8c0730c0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0730c2; /* fmov.s @r15+,fr12 */
L8c0730c2: *stop=0x8c0730c2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0730c4; /* fmov.s @r15+,fr13 */
L8c0730c4: *stop=0x8c0730c4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0730c6; /* fmov.s @r15+,fr14 */
L8c0730c6: *stop=0x8c0730c6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0730c8; /* fmov.s @r15+,fr15 */
L8c0730c8: *stop=0x8c0730c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c0730ca; /* mov.l @r15+,r8 */
L8c0730ca: *stop=0x8c0730cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c0730cc; /* mov.l @r15+,r9 */
L8c0730cc: *stop=0x8c0730ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c0730ce; /* mov.l @r15+,r10 */
L8c0730ce: *stop=0x8c0730ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c0730d0; /* mov.l @r15+,r11 */
L8c0730d0: *stop=0x8c0730d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c0730d2; /* mov.l @r15+,r12 */
L8c0730d2: *stop=0x8c0730d2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0730d4; /* mov.l @r15+,r13 */
L8c0730d4: *stop=0x8c0730d4u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0730d6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_movement_owner_8c072b44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072bf2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072bf2u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072c0e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072c0eu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072c2a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072c2au);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072c32(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072c32u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072c88(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072c88u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072c94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072c94u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072caa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072caau);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072d3a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072d3au);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072d48(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072d48u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072d5c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072d5cu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072d74(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072d74u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072d80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072d80u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072d90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072d90u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072da0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072da0u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072dbe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072dbeu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072dc4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072dc4u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072e88(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072e88u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072e9e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072e9eu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072f1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072f1eu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072f2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072f2cu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072f5c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072f5cu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072fde(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072fdeu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072fee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072feeu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c072ffc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072ffcu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c073010(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073010u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c073020(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073020u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c073030(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073030u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c073042(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073042u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c07305e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07305eu);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c073090(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073090u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c073096(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073096u);}
SA_STAGE_EXPORT int sa_player_movement_owner_resume_8c0730b2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_owner_run_8c072b44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0730b2u);}
