/* Original dirty-directory and FAT persistence, root write and relocation allocators.
 * Explicit register state, original callbacks and nonlocal continuations.
 * No supplied parser results or saved task frames.
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
int sa_movie_channel_io_return_allowed(void *,uint32_t);
static int sa_movie_filesystem_persist_run_8c64ab36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64ab88u)goto L8c64ab88;if(resume_pc==0x8c64ab94u)goto L8c64ab94;if(resume_pc==0x8c64abd0u)goto L8c64abd0;if(resume_pc==0x8c64abdau)goto L8c64abda;if(resume_pc==0x8c64abe2u)goto L8c64abe2;if(resume_pc==0x8c64abf6u)goto L8c64abf6;if(resume_pc==0x8c64ac08u)goto L8c64ac08;if(resume_pc==0x8c64ac14u)goto L8c64ac14;if(resume_pc==0x8c64ac20u)goto L8c64ac20;if(resume_pc==0x8c64ac38u)goto L8c64ac38;if(resume_pc==0x8c64ac42u)goto L8c64ac42;goto L8c64ab36;
L8c64ab36: *stop=0x8c64ab36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ab38; /* mov.l r14,@-r15 */
L8c64ab38: *stop=0x8c64ab38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ab3a; /* mov.l r13,@-r15 */
L8c64ab3a: *stop=0x8c64ab3au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64ab3c; /* mov.l r12,@-r15 */
L8c64ab3c: *stop=0x8c64ab3cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64ab3e; /* mov.l r11,@-r15 */
L8c64ab3e: *stop=0x8c64ab3eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64ab40; /* mov.l r10,@-r15 */
L8c64ab40: *stop=0x8c64ab40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64ab42; /* mov.l r9,@-r15 */
L8c64ab42: *stop=0x8c64ab42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64ab44; /* mov.l r8,@-r15 */
L8c64ab44: *stop=0x8c64ab44u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ab46; /* sts.l PR,@-r15 */
L8c64ab46: *stop=0x8c64ab46u;r[15]+=4294967284u;goto L8c64ab48; /* add #-0xc,r15 */
L8c64ab48: *stop=0x8c64ab48u;r[9]=sx(m,bus,&io_status,0x8c64aba8u,2);if(io_status)return io_status;goto L8c64ab4a; /* mov.w 0x8c64aba8,r9 */
L8c64ab4a: *stop=0x8c64ab4au;r[13]=r[4];goto L8c64ab4c; /* mov r4,r13 */
L8c64ab4c: *stop=0x8c64ab4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64abbcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ab4e; /* mov.l 0x8c64abbc,r3 */
L8c64ab4e: *stop=0x8c64ab4eu;*macl=r[9]*r[13];goto L8c64ab50; /* mul.l r9,r13 */
L8c64ab50: *stop=0x8c64ab50u;r[9]=*macl;goto L8c64ab52; /* sts MACL,r9 */
L8c64ab52: *stop=0x8c64ab52u;r[9]+=r[3];goto L8c64ab54; /* add r3,r9 */
L8c64ab54: *stop=0x8c64ab54u;if(!valid_access(r[9]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ab56; /* mov.l @(0x1c,r9),r5 */
L8c64ab56: *stop=0x8c64ab56u;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ab58; /* mov.l @(0x18,r5),r2 */
L8c64ab58: *stop=0x8c64ab58u;if(!valid_access(r[5]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64ab5a; /* mov.l @(0x14,r5),r4 */
L8c64ab5a: *stop=0x8c64ab5au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c64ab5c; /* mov.l r2,@(0x4,r15) */
L8c64ab5c: *stop=0x8c64ab5cu;r[2]=0u;goto L8c64ab5e; /* mov #0x0,r2 */
L8c64ab5e: *stop=0x8c64ab5eu;r[10]=r[2];goto L8c64ab60; /* mov r2,r10 */
L8c64ab60: *stop=0x8c64ab60u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c64ab62; /* mov.l r2,@(0x8,r15) */
L8c64ab62: *stop=0x8c64ab64u;r[14]=r[4]&65535u;goto L8c64ac28; /* bra 0x8c64ac28 */
L8c64ab66: *stop=0x8c64ab66u;r[3]=r[9];goto L8c64ab68; /* mov r9,r3 */
L8c64ab68: *stop=0x8c64ab68u;r[3]+=72u;goto L8c64ab6a; /* add #0x48,r3 */
L8c64ab6a: *stop=0x8c64ab6au;r[3]+=r[10];goto L8c64ab6c; /* add r10,r3 */
L8c64ab6c: *stop=0x8c64ab6cu;if(!valid_access(r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;goto L8c64ab6e; /* mov.b @r3,r2 */
L8c64ab6e: *stop=0x8c64ab6eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64ab70; /* tst r2,r2 */
L8c64ab70: if((*sr&1u))goto L8c64ac1a;goto L8c64ab72; /* bt 0x8c64ac1a */
L8c64ab72: *stop=0x8c64ab72u;r[2]=r[10];goto L8c64ab74; /* mov r10,r2 */
L8c64ab74: *stop=0x8c64ab74u;r[2]<<=8;goto L8c64ab76; /* shll8 r2 */
L8c64ab76: *stop=0x8c64ab76u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c64ab78; /* shll r2 */
L8c64ab78: *stop=0x8c64ab78u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64ab7a; /* mov.l r2,@r15 */
L8c64ab7a: *stop=0x8c64ab7au;if(!valid_access(r[9]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x28u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64ab7c; /* mov.l @(0x28,r9),r6 */
L8c64ab7c: *stop=0x8c64ab7cu;r[5]=r[14];goto L8c64ab7e; /* mov r14,r5 */
L8c64ab7e: *stop=0x8c64ab7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ab80; /* mov.l @r15,r3 */
L8c64ab80: *stop=0x8c64ab80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64abacu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ab82; /* mov.l 0x8c64abac,r2 */
L8c64ab82: *stop=0x8c64ab82u;r[6]+=r[3];goto L8c64ab84; /* add r3,r6 */
L8c64ab84: target=r[2];*pr=0x8c64ab88u;*stop=0x8c64ab86u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ab88; /* jsr @r2 */
L8c64ab88: *stop=0x8c64ab88u;r[4]=r[0];goto L8c64ab8a; /* mov r0,r4 */
L8c64ab8a: *stop=0x8c64ab8au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64ab8c; /* tst r4,r4 */
L8c64ab8c: if(!(*sr&1u))goto L8c64abc0;goto L8c64ab8e; /* bf 0x8c64abc0 */
L8c64ab8e: *stop=0x8c64ab8eu;r[5]=r[14];goto L8c64ab90; /* mov r14,r5 */
L8c64ab90: target=0x8c64b1c0u;*pr=0x8c64ab94u;*stop=0x8c64ab92u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ab94; /* bsr 0x8c64b1c0 */
L8c64ab94: *stop=0x8c64ab94u;r[3]=r[9];goto L8c64ab96; /* mov r9,r3 */
L8c64ab96: *stop=0x8c64ab96u;r[3]+=72u;goto L8c64ab98; /* add #0x48,r3 */
L8c64ab98: *stop=0x8c64ab98u;r[3]+=r[10];goto L8c64ab9a; /* add r10,r3 */
L8c64ab9a: *stop=0x8c64ab9au;r[8]=r[14];goto L8c64ab9c; /* mov r14,r8 */
L8c64ab9c: *stop=0x8c64ab9cu;r[2]=0u;goto L8c64ab9e; /* mov #0x0,r2 */
L8c64ab9e: *stop=0x8c64ab9eu;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[2],1);if(io_status)return io_status;goto L8c64aba0; /* mov.b r2,@r3 */
L8c64aba0: *stop=0x8c64aba0u;r[11]=r[0]&65535u;goto L8c64aba2; /* extu.w r0,r11 */
L8c64aba2: *stop=0x8c64aba4u;r[14]=r[11];goto L8c64ac26; /* bra 0x8c64ac26 */
L8c64abc0: *stop=0x8c64abc0u;r[2]=sx(m,bus,&io_status,0x8c64ac7cu,2);if(io_status)return io_status;goto L8c64abc2; /* mov.w 0x8c64ac7c,r2 */
L8c64abc2: *stop=0x8c64abc2u;*sr=(*sr&~1u)|(r[2]==r[4]);goto L8c64abc4; /* cmp/eq r2,r4 */
L8c64abc4: if((*sr&1u))goto L8c64ac48;goto L8c64abc6; /* bt 0x8c64ac48 */
L8c64abc6: *stop=0x8c64abc6u;r[2]=1u;goto L8c64abc8; /* mov #0x1,r2 */
L8c64abc8: *stop=0x8c64abc8u;r[5]=r[14];goto L8c64abca; /* mov r14,r5 */
L8c64abca: *stop=0x8c64abcau;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c64abcc; /* mov.l r2,@(0x8,r15) */
L8c64abcc: target=0x8c64b1c0u;*pr=0x8c64abd0u;*stop=0x8c64abceu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64abd0; /* bsr 0x8c64b1c0 */
L8c64abd0: *stop=0x8c64abd0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ac80u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64abd2; /* mov.l 0x8c64ac80,r6 */
L8c64abd2: *stop=0x8c64abd2u;r[5]=r[14];goto L8c64abd4; /* mov r14,r5 */
L8c64abd4: *stop=0x8c64abd4u;r[11]=r[0]&65535u;goto L8c64abd6; /* extu.w r0,r11 */
L8c64abd6: target=0x8c64b18eu;*pr=0x8c64abdau;*stop=0x8c64abd8u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64abda; /* bsr 0x8c64b18e */
L8c64abda: *stop=0x8c64abdau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ac88u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64abdc; /* mov.l 0x8c64ac88,r3 */
L8c64abdc: *stop=0x8c64abdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ac84u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64abde; /* mov.l 0x8c64ac84,r5 */
L8c64abde: target=r[3];*pr=0x8c64abe2u;*stop=0x8c64abe0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64abe2; /* jsr @r3 */
L8c64abe2: *stop=0x8c64abe2u;r[12]=r[0];goto L8c64abe4; /* mov r0,r12 */
L8c64abe4: *stop=0x8c64abe4u;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c64abe6; /* cmp/pz r12 */
L8c64abe6: if((*sr&1u))goto L8c64abee;goto L8c64abe8; /* bt 0x8c64abee */
L8c64abe8: *stop=0x8c64abe8u;r[0]=sx(m,bus,&io_status,0x8c64ac7eu,2);if(io_status)return io_status;goto L8c64abea; /* mov.w 0x8c64ac7e,r0 */
L8c64abea: *stop=0x8c64abecu;goto L8c64ac4e; /* bra 0x8c64ac4e */
L8c64abee: *stop=0x8c64abeeu;r[5]=r[12];goto L8c64abf0; /* mov r12,r5 */
L8c64abf0: *stop=0x8c64abf0u;r[6]=r[11];goto L8c64abf2; /* mov r11,r6 */
L8c64abf2: target=0x8c64b18eu;*pr=0x8c64abf6u;*stop=0x8c64abf4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64abf6; /* bsr 0x8c64b18e */
L8c64abf6: *stop=0x8c64abf6u;*sr=(*sr&~1u)|!(r[10]&r[10]);goto L8c64abf8; /* tst r10,r10 */
L8c64abf8: if(!(*sr&1u))goto L8c64ac0c;goto L8c64abfa; /* bf 0x8c64ac0c */
L8c64abfa: *stop=0x8c64abfau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ac8cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64abfc; /* mov.l 0x8c64ac8c,r2 */
L8c64abfc: *stop=0x8c64abfcu;r[5]=74u;goto L8c64abfe; /* mov #0x4a,r5 */
L8c64abfe: *stop=0x8c64abfeu;r[6]=r[12];goto L8c64ac00; /* mov r12,r6 */
L8c64ac00: *stop=0x8c64ac00u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ac02; /* mov.l @r2,r3 */
L8c64ac02: *stop=0x8c64ac02u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ac04; /* mov.l @(0x10,r3),r1 */
L8c64ac04: target=r[1];*pr=0x8c64ac08u;*stop=0x8c64ac06u;if(!valid_access(r[9]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x20u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ac08; /* jsr @r1 */
L8c64ac08: *stop=0x8c64ac0au;goto L8c64ac14; /* bra 0x8c64ac14 */
L8c64ac0c: *stop=0x8c64ac0cu;r[5]=r[8];goto L8c64ac0e; /* mov r8,r5 */
L8c64ac0e: *stop=0x8c64ac0eu;r[6]=r[12];goto L8c64ac10; /* mov r12,r6 */
L8c64ac10: target=0x8c64b18eu;*pr=0x8c64ac14u;*stop=0x8c64ac12u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ac14; /* bsr 0x8c64b18e */
L8c64ac14: *stop=0x8c64ac14u;r[8]=r[14];goto L8c64ac16; /* mov r14,r8 */
L8c64ac16: *stop=0x8c64ac18u;r[14]=r[12];goto L8c64ab7a; /* bra 0x8c64ab7a */
L8c64ac1a: *stop=0x8c64ac1au;r[5]=r[14];goto L8c64ac1c; /* mov r14,r5 */
L8c64ac1c: target=0x8c64b1c0u;*pr=0x8c64ac20u;*stop=0x8c64ac1eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ac20; /* bsr 0x8c64b1c0 */
L8c64ac20: *stop=0x8c64ac20u;r[8]=r[14];goto L8c64ac22; /* mov r14,r8 */
L8c64ac22: *stop=0x8c64ac22u;r[11]=r[0]&65535u;goto L8c64ac24; /* extu.w r0,r11 */
L8c64ac24: *stop=0x8c64ac24u;r[14]=r[11];goto L8c64ac26; /* mov r11,r14 */
L8c64ac26: *stop=0x8c64ac26u;r[10]+=1u;goto L8c64ac28; /* add #0x1,r10 */
L8c64ac28: *stop=0x8c64ac28u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ac2a; /* mov.l @(0x4,r15),r3 */
L8c64ac2a: *stop=0x8c64ac2au;*sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c64ac2c; /* cmp/ge r3,r10 */
L8c64ac2c: if(!(*sr&1u))goto L8c64ab66;goto L8c64ac2e; /* bf 0x8c64ab66 */
L8c64ac2e: *stop=0x8c64ac2eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ac30; /* mov.l @(0x8,r15),r1 */
L8c64ac30: *stop=0x8c64ac30u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c64ac32; /* tst r1,r1 */
L8c64ac32: if((*sr&1u))goto L8c64ac4c;goto L8c64ac34; /* bt 0x8c64ac4c */
L8c64ac34: target=0x8c64afe0u;*pr=0x8c64ac38u;*stop=0x8c64ac36u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ac38; /* bsr 0x8c64afe0 */
L8c64ac38: *stop=0x8c64ac38u;r[4]=r[0];goto L8c64ac3a; /* mov r0,r4 */
L8c64ac3a: *stop=0x8c64ac3au;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64ac3c; /* cmp/pz r4 */
L8c64ac3c: if(!(*sr&1u))goto L8c64ac48;goto L8c64ac3e; /* bf 0x8c64ac48 */
L8c64ac3e: target=0x8c64ae4au;*pr=0x8c64ac42u;*stop=0x8c64ac40u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ac42; /* bsr 0x8c64ae4a */
L8c64ac42: *stop=0x8c64ac42u;r[4]=r[0];goto L8c64ac44; /* mov r0,r4 */
L8c64ac44: *stop=0x8c64ac44u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64ac46; /* cmp/pz r4 */
L8c64ac46: if((*sr&1u))goto L8c64ac4c;goto L8c64ac48; /* bt 0x8c64ac4c */
L8c64ac48: *stop=0x8c64ac4au;r[0]=r[4];goto L8c64ac4e; /* bra 0x8c64ac4e */
L8c64ac4c: *stop=0x8c64ac4cu;r[0]=0u;goto L8c64ac4e; /* mov #0x0,r0 */
L8c64ac4e: *stop=0x8c64ac4eu;r[15]+=12u;goto L8c64ac50; /* add #0xc,r15 */
L8c64ac50: *stop=0x8c64ac50u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64ac52; /* lds.l @r15+,PR */
L8c64ac52: *stop=0x8c64ac52u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64ac54; /* mov.l @r15+,r8 */
L8c64ac54: *stop=0x8c64ac54u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64ac56; /* mov.l @r15+,r9 */
L8c64ac56: *stop=0x8c64ac56u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64ac58; /* mov.l @r15+,r10 */
L8c64ac58: *stop=0x8c64ac58u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64ac5a; /* mov.l @r15+,r11 */
L8c64ac5a: *stop=0x8c64ac5au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64ac5c; /* mov.l @r15+,r12 */
L8c64ac5c: *stop=0x8c64ac5cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64ac5e; /* mov.l @r15+,r13 */
L8c64ac5e: *stop=0x8c64ac5eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64ac60u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_8c64ab36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64ab88(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ab88u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64ab94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ab94u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64abd0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64abd0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64abda(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64abdau);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64abe2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64abe2u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64abf6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64abf6u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64ac08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ac08u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64ac14(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ac14u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64ac20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ac20u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64ac38(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ac38u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64ac42(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64ab36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ac42u);}

static int sa_movie_filesystem_persist_run_8c64894a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64897cu)goto L8c64897c;if(resume_pc==0x8c64899au)goto L8c64899a;goto L8c64894a;
L8c64894a: *stop=0x8c64894au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64894c; /* mov.l r14,@-r15 */
L8c64894c: *stop=0x8c64894cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64894e; /* mov.l r13,@-r15 */
L8c64894e: *stop=0x8c64894eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c648950; /* sts.l PR,@-r15 */
L8c648950: *stop=0x8c648950u;r[15]+=4294967288u;goto L8c648952; /* add #-0x8,r15 */
L8c648952: *stop=0x8c648952u;r[13]=sx(m,bus,&io_status,0x8c6489b0u,2);if(io_status)return io_status;goto L8c648954; /* mov.w 0x8c6489b0,r13 */
L8c648954: *stop=0x8c648954u;r[14]=r[4];goto L8c648956; /* mov r4,r14 */
L8c648956: *stop=0x8c648956u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6489d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648958; /* mov.l 0x8c6489d0,r3 */
L8c648958: *stop=0x8c648958u;r[0]=r[5];goto L8c64895a; /* mov r5,r0 */
L8c64895a: *stop=0x8c64895au;*macl=r[13]*r[14];goto L8c64895c; /* mul.l r13,r14 */
L8c64895c: *stop=0x8c64895cu;if(!valid_access(r[15]+4u,2u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],2);if(io_status)return io_status;goto L8c64895e; /* mov.w r0,@(0x4,r15) */
L8c64895e: *stop=0x8c64895eu;if(!valid_access(r[15]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c648960; /* mov.w @(0x4,r15),r0 */
L8c648960: *stop=0x8c648960u;r[13]=*macl;goto L8c648962; /* sts MACL,r13 */
L8c648962: *stop=0x8c648962u;r[13]+=r[3];goto L8c648964; /* add r3,r13 */
L8c648964: *stop=0x8c648964u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c648966; /* mov.l @(0x1c,r13),r4 */
L8c648966: *stop=0x8c648966u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c648968; /* mov.l @(0x8,r4),r5 */
L8c648968: *stop=0x8c648968u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64896a; /* mov.l @(0x10,r4),r2 */
L8c64896a: *stop=0x8c64896au;r[5]-=r[2];goto L8c64896c; /* sub r2,r5 */
L8c64896c: *stop=0x8c64896cu;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64896e; /* mov.l @(0x18,r4),r2 */
L8c64896e: *stop=0x8c64896eu;r[7]=r[5];goto L8c648970; /* mov r5,r7 */
L8c648970: *stop=0x8c648970u;r[6]=r[5];goto L8c648972; /* mov r5,r6 */
L8c648972: *stop=0x8c648972u;r[5]=r[0];goto L8c648974; /* mov r0,r5 */
L8c648974: *stop=0x8c648974u;r[6]-=r[2];goto L8c648976; /* sub r2,r6 */
L8c648976: *stop=0x8c648976u;r[7]+=4294967295u;goto L8c648978; /* add #-0x1,r7 */
L8c648978: target=0x8c6489d4u;*pr=0x8c64897cu;*stop=0x8c64897au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64897c; /* bsr 0x8c6489d4 */
L8c64897c: *stop=0x8c64897cu;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c64897e; /* cmp/pl r0 */
L8c64897e: take=(*sr&1u);*stop=0x8c648980u;r[4]=r[0];if(take)goto L8c6489a0;goto L8c648982; /* bt/s 0x8c6489a0 */
L8c648982: *stop=0x8c648982u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c648984; /* mov.l @(0x1c,r13),r4 */
L8c648984: *stop=0x8c648984u;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648986; /* mov.l @(0x1c,r4),r3 */
L8c648986: *stop=0x8c648986u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c648988; /* mov.l r3,@r15 */
L8c648988: *stop=0x8c648988u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[7]=loaded;}goto L8c64898a; /* mov.l @r15,r7 */
L8c64898a: *stop=0x8c64898au;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64898c; /* mov.l @(0x20,r4),r3 */
L8c64898c: *stop=0x8c64898cu;if(!valid_access(r[15]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c64898e; /* mov.w @(0x4,r15),r0 */
L8c64898e: *stop=0x8c64898eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c648990; /* mov.l @r15,r6 */
L8c648990: *stop=0x8c648990u;r[7]+=r[3];goto L8c648992; /* add r3,r7 */
L8c648992: *stop=0x8c648992u;r[5]=r[0];goto L8c648994; /* mov r0,r5 */
L8c648994: *stop=0x8c648994u;r[7]+=4294967295u;goto L8c648996; /* add #-0x1,r7 */
L8c648996: target=0x8c6489d4u;*pr=0x8c64899au;*stop=0x8c648998u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64899a; /* bsr 0x8c6489d4 */
L8c64899a: *stop=0x8c64899au;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c64899c; /* cmp/pl r0 */
L8c64899c: take=!(*sr&1u);*stop=0x8c64899eu;r[4]=r[0];if(take)goto L8c6489a4;goto L8c6489a0; /* bf/s 0x8c6489a4 */
L8c6489a0: *stop=0x8c6489a2u;r[0]=r[4];goto L8c6489a6; /* bra 0x8c6489a6 */
L8c6489a4: *stop=0x8c6489a4u;r[0]=4294967295u;goto L8c6489a6; /* mov #-0x1,r0 */
L8c6489a6: *stop=0x8c6489a6u;r[15]+=8u;goto L8c6489a8; /* add #0x8,r15 */
L8c6489a8: *stop=0x8c6489a8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6489aa; /* lds.l @r15+,PR */
L8c6489aa: *stop=0x8c6489aau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6489ac; /* mov.l @r15+,r13 */
L8c6489ac: *stop=0x8c6489acu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6489aeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_8c64894a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64894a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64897c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64894a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64897cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64899a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64894a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64899au);}

static int sa_movie_filesystem_persist_run_8c64afe0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64b00au)goto L8c64b00a;if(resume_pc==0x8c64b02au)goto L8c64b02a;if(resume_pc==0x8c64b046u)goto L8c64b046;if(resume_pc==0x8c64b056u)goto L8c64b056;if(resume_pc==0x8c64b06cu)goto L8c64b06c;goto L8c64afe0;
L8c64afe0: *stop=0x8c64afe0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64afe2; /* mov.l r14,@-r15 */
L8c64afe2: *stop=0x8c64afe2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64afe4; /* mov.l r13,@-r15 */
L8c64afe4: *stop=0x8c64afe4u;r[14]=sx(m,bus,&io_status,0x8c64b0d2u,2);if(io_status)return io_status;goto L8c64afe6; /* mov.w 0x8c64b0d2,r14 */
L8c64afe6: *stop=0x8c64afe6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64afe8; /* mov.l r12,@-r15 */
L8c64afe8: *stop=0x8c64afe8u;r[12]=r[4];goto L8c64afea; /* mov r4,r12 */
L8c64afea: *stop=0x8c64afeau;*macl=r[14]*r[12];goto L8c64afec; /* mul.l r14,r12 */
L8c64afec: *stop=0x8c64afecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64afee; /* mov.l r11,@-r15 */
L8c64afee: *stop=0x8c64afeeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64aff0; /* mov.l r10,@-r15 */
L8c64aff0: *stop=0x8c64aff0u;r[10]=0u;goto L8c64aff2; /* mov #0x0,r10 */
L8c64aff2: *stop=0x8c64aff2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64aff4; /* mov.l r9,@-r15 */
L8c64aff4: *stop=0x8c64aff4u;r[9]=1u;goto L8c64aff6; /* mov #0x1,r9 */
L8c64aff6: *stop=0x8c64aff6u;r[14]=*macl;goto L8c64aff8; /* sts MACL,r14 */
L8c64aff8: *stop=0x8c64aff8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64affa; /* mov.l r8,@-r15 */
L8c64affa: *stop=0x8c64affau;r[13]=r[10];goto L8c64affc; /* mov r10,r13 */
L8c64affc: *stop=0x8c64affcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b0d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64affe; /* mov.l 0x8c64b0d8,r3 */
L8c64affe: *stop=0x8c64affeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b000; /* sts.l PR,@-r15 */
L8c64b000: *stop=0x8c64b000u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b0dcu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c64b002; /* mov.l 0x8c64b0dc,r8 */
L8c64b002: *stop=0x8c64b002u;r[14]+=r[3];goto L8c64b004; /* add r3,r14 */
L8c64b004: *stop=0x8c64b004u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b0e0u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64b006; /* mov.l 0x8c64b0e0,r11 */
L8c64b006: target=0x8c64aeccu;*pr=0x8c64b00au;*stop=0x8c64b008u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b00a; /* bsr 0x8c64aecc */
L8c64b00a: *stop=0x8c64b00au;r[1]=sx(m,bus,&io_status,0x8c64b0d4u,2);if(io_status)return io_status;goto L8c64b00c; /* mov.w 0x8c64b0d4,r1 */
L8c64b00c: *stop=0x8c64b00cu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64b00e; /* cmp/eq r1,r0 */
L8c64b00e: take=(*sr&1u);*stop=0x8c64b010u;r[4]=r[0];if(take)goto L8c64b020;goto L8c64b012; /* bt/s 0x8c64b020 */
L8c64b012: *stop=0x8c64b012u;r[1]=sx(m,bus,&io_status,0x8c64b0d6u,2);if(io_status)return io_status;goto L8c64b014; /* mov.w 0x8c64b0d6,r1 */
L8c64b014: *stop=0x8c64b014u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64b016; /* cmp/eq r1,r0 */
L8c64b016: if((*sr&1u))goto L8c64b026;goto L8c64b018; /* bt 0x8c64b026 */
L8c64b018: *stop=0x8c64b018u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64b01a; /* cmp/eq #0x0,r0 */
L8c64b01a: if((*sr&1u))goto L8c64b064;goto L8c64b01c; /* bt 0x8c64b064 */
L8c64b01c: *stop=0x8c64b01eu;goto L8c64b064; /* bra 0x8c64b064 */
L8c64b020: *stop=0x8c64b020u;r[0]=sx(m,bus,&io_status,0x8c64b0d4u,2);if(io_status)return io_status;goto L8c64b022; /* mov.w 0x8c64b0d4,r0 */
L8c64b022: *stop=0x8c64b024u;goto L8c64b06e; /* bra 0x8c64b06e */
L8c64b026: target=0x8c64af4au;*pr=0x8c64b02au;*stop=0x8c64b028u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b02a; /* bsr 0x8c64af4a */
L8c64b02a: *stop=0x8c64b02au;r[13]=r[0];goto L8c64b02c; /* mov r0,r13 */
L8c64b02c: *stop=0x8c64b02cu;*sr=(*sr&~1u)|((int32_t)r[13]>=0);goto L8c64b02e; /* cmp/pz r13 */
L8c64b02e: if((*sr&1u))goto L8c64b036;goto L8c64b030; /* bt 0x8c64b036 */
L8c64b030: *stop=0x8c64b030u;r[0]=sx(m,bus,&io_status,0x8c64b0d6u,2);if(io_status)return io_status;goto L8c64b032; /* mov.w 0x8c64b0d6,r0 */
L8c64b032: *stop=0x8c64b034u;goto L8c64b06e; /* bra 0x8c64b06e */
L8c64b036: *stop=0x8c64b036u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b038; /* mov.l @(0x1c,r14),r2 */
L8c64b038: *stop=0x8c64b038u;r[5]=70u;goto L8c64b03a; /* mov #0x46,r5 */
L8c64b03a: *stop=0x8c64b03au;r[6]=r[13];goto L8c64b03c; /* mov r13,r6 */
L8c64b03c: *stop=0x8c64b03cu;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[13],4);if(io_status)return io_status;goto L8c64b03e; /* mov.l r13,@(0xc,r2) */
L8c64b03e: *stop=0x8c64b03eu;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b040; /* mov.l @r8,r3 */
L8c64b040: *stop=0x8c64b040u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b042; /* mov.l @(0x10,r3),r2 */
L8c64b042: target=r[2];*pr=0x8c64b046u;*stop=0x8c64b044u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b046; /* jsr @r2 */
L8c64b046: *stop=0x8c64b048u;r[13]=r[10];goto L8c64b058; /* bra 0x8c64b058 */
L8c64b04a: *stop=0x8c64b04au;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b04c; /* mov.l @(0x1c,r14),r5 */
L8c64b04c: *stop=0x8c64b04cu;r[6]=r[11];goto L8c64b04e; /* mov r11,r6 */
L8c64b04e: *stop=0x8c64b04eu;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b050; /* mov.l @(0xc,r5),r5 */
L8c64b050: *stop=0x8c64b050u;r[5]+=r[13];goto L8c64b052; /* add r13,r5 */
L8c64b052: target=0x8c64b18eu;*pr=0x8c64b056u;*stop=0x8c64b054u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b056; /* bsr 0x8c64b18e */
L8c64b056: *stop=0x8c64b056u;r[13]+=1u;goto L8c64b058; /* add #0x1,r13 */
L8c64b058: *stop=0x8c64b058u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b05a; /* mov.l @(0x1c,r14),r2 */
L8c64b05a: *stop=0x8c64b05au;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b05c; /* mov.l @(0x10,r2),r3 */
L8c64b05c: *stop=0x8c64b05cu;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[3]);goto L8c64b05e; /* cmp/ge r3,r13 */
L8c64b05e: if(!(*sr&1u))goto L8c64b04a;goto L8c64b060; /* bf 0x8c64b04a */
L8c64b060: *stop=0x8c64b062u;r[13]=r[9];goto L8c64b006; /* bra 0x8c64b006 */
L8c64b064: *stop=0x8c64b064u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c64b066; /* tst r13,r13 */
L8c64b066: if((*sr&1u))goto L8c64b06c;goto L8c64b068; /* bt 0x8c64b06c */
L8c64b068: target=0x8c64ae4au;*pr=0x8c64b06cu;*stop=0x8c64b06au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b06c; /* bsr 0x8c64ae4a */
L8c64b06c: *stop=0x8c64b06cu;r[0]=0u;goto L8c64b06e; /* mov #0x0,r0 */
L8c64b06e: *stop=0x8c64b06eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b070; /* lds.l @r15+,PR */
L8c64b070: *stop=0x8c64b070u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64b072; /* mov.l @r15+,r8 */
L8c64b072: *stop=0x8c64b072u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64b074; /* mov.l @r15+,r9 */
L8c64b074: *stop=0x8c64b074u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64b076; /* mov.l @r15+,r10 */
L8c64b076: *stop=0x8c64b076u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64b078; /* mov.l @r15+,r11 */
L8c64b078: *stop=0x8c64b078u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64b07a; /* mov.l @r15+,r12 */
L8c64b07a: *stop=0x8c64b07au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b07c; /* mov.l @r15+,r13 */
L8c64b07c: *stop=0x8c64b07cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b07eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_8c64afe0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64afe0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64b00a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64afe0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b00au);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64b02a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64afe0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b02au);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64b046(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64afe0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b046u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64b056(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64afe0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b056u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64b06c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64afe0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b06cu);}

SA_STAGE_EXPORT int sa_movie_filesystem_persist_8c64ae4a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64ae4a;
L8c64ae4a: *stop=0x8c64ae4au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ae4c; /* mov.l r14,@-r15 */
L8c64ae4c: *stop=0x8c64ae4cu;r[14]=sx(m,bus,&io_status,0x8c64aebau,2);if(io_status)return io_status;goto L8c64ae4e; /* mov.w 0x8c64aeba,r14 */
L8c64ae4e: *stop=0x8c64ae4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aec0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ae50; /* mov.l 0x8c64aec0,r3 */
L8c64ae50: *stop=0x8c64ae50u;*macl=r[14]*r[4];goto L8c64ae52; /* mul.l r14,r4 */
L8c64ae52: *stop=0x8c64ae52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aec8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ae54; /* mov.l 0x8c64aec8,r2 */
L8c64ae54: *stop=0x8c64ae54u;r[14]=*macl;goto L8c64ae56; /* sts MACL,r14 */
L8c64ae56: *stop=0x8c64ae56u;r[14]+=r[3];goto L8c64ae58; /* add r3,r14 */
L8c64ae58: *stop=0x8c64ae58u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ae5a; /* mov.l @(0x1c,r14),r5 */
L8c64ae5a: *stop=0x8c64ae5au;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64ae5c; /* mov.l @(0x20,r14),r6 */
L8c64ae5c: *stop=0x8c64ae5cu;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ae5e; /* mov.l @(0x8,r5),r5 */
L8c64ae5e: target=r[2];*stop=0x8c64ae60u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

static int sa_movie_filesystem_persist_run_8c64aecc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64af0eu)goto L8c64af0e;if(resume_pc==0x8c64af20u)goto L8c64af20;if(resume_pc==0x8c64af30u)goto L8c64af30;goto L8c64aecc;
L8c64aecc: *stop=0x8c64aeccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64aece; /* mov.l r14,@-r15 */
L8c64aece: *stop=0x8c64aeceu;r[14]=0u;goto L8c64aed0; /* mov #0x0,r14 */
L8c64aed0: *stop=0x8c64aed0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64aed2; /* mov.l r13,@-r15 */
L8c64aed2: *stop=0x8c64aed2u;r[13]=sx(m,bus,&io_status,0x8c64afc8u,2);if(io_status)return io_status;goto L8c64aed4; /* mov.w 0x8c64afc8,r13 */
L8c64aed4: *stop=0x8c64aed4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64aed6; /* mov.l r12,@-r15 */
L8c64aed6: *stop=0x8c64aed6u;r[12]=r[4];goto L8c64aed8; /* mov r4,r12 */
L8c64aed8: *stop=0x8c64aed8u;*macl=r[13]*r[12];goto L8c64aeda; /* mul.l r13,r12 */
L8c64aeda: *stop=0x8c64aedau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64afccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64aedc; /* mov.l 0x8c64afcc,r3 */
L8c64aedc: *stop=0x8c64aedcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64aede; /* mov.l r11,@-r15 */
L8c64aede: *stop=0x8c64aedeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64aee0; /* mov.l r10,@-r15 */
L8c64aee0: *stop=0x8c64aee0u;r[13]=*macl;goto L8c64aee2; /* sts MACL,r13 */
L8c64aee2: *stop=0x8c64aee2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64aee4; /* mov.l r9,@-r15 */
L8c64aee4: *stop=0x8c64aee4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64aee6; /* mov.l r8,@-r15 */
L8c64aee6: *stop=0x8c64aee6u;r[13]+=r[3];goto L8c64aee8; /* add r3,r13 */
L8c64aee8: *stop=0x8c64aee8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64afd0u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c64aeea; /* mov.l 0x8c64afd0,r8 */
L8c64aeea: *stop=0x8c64aeeau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64aeec; /* sts.l PR,@-r15 */
L8c64aeec: *stop=0x8c64aeecu;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64aeee; /* mov.l @(0x1c,r13),r10 */
L8c64aeee: *stop=0x8c64aeeeu;r[9]=r[8];goto L8c64aef0; /* mov r8,r9 */
L8c64aef0: *stop=0x8c64aef0u;if(!valid_access(r[10]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x10u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64aef2; /* mov.l @(0x10,r10),r10 */
L8c64aef2: *stop=0x8c64aef2u;*sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c64aef4; /* cmp/pl r10 */
L8c64aef4: take=!(*sr&1u);*stop=0x8c64aef6u;r[9]+=5u;if(take)goto L8c64af36;goto L8c64aef8; /* bf/s 0x8c64af36 */
L8c64aef8: *stop=0x8c64aef8u;r[6]=r[14];goto L8c64aefa; /* mov r14,r6 */
L8c64aefa: *stop=0x8c64aefau;if(!valid_access(r[13]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64aefc; /* mov.l @(0x24,r13),r3 */
L8c64aefc: *stop=0x8c64aefcu;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64aefe; /* mov.l @(0x1c,r13),r5 */
L8c64aefe: *stop=0x8c64aefeu;r[6]<<=8;goto L8c64af00; /* shll8 r6 */
L8c64af00: *stop=0x8c64af00u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c64af02; /* shll r6 */
L8c64af02: *stop=0x8c64af02u;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64af04; /* mov.l @(0xc,r5),r5 */
L8c64af04: *stop=0x8c64af04u;r[6]+=r[3];goto L8c64af06; /* add r3,r6 */
L8c64af06: *stop=0x8c64af06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64afd4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64af08; /* mov.l 0x8c64afd4,r3 */
L8c64af08: *stop=0x8c64af08u;r[5]+=r[14];goto L8c64af0a; /* add r14,r5 */
L8c64af0a: target=r[3];*pr=0x8c64af0eu;*stop=0x8c64af0cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64af0e; /* jsr @r3 */
L8c64af0e: *stop=0x8c64af0eu;r[11]=r[0];goto L8c64af10; /* mov r0,r11 */
L8c64af10: *stop=0x8c64af10u;*sr=(*sr&~1u)|((int32_t)r[11]>=0);goto L8c64af12; /* cmp/pz r11 */
L8c64af12: if((*sr&1u))goto L8c64af24;goto L8c64af14; /* bt 0x8c64af24 */
L8c64af14: *stop=0x8c64af14u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64af16; /* mov.l @(0x1c,r13),r5 */
L8c64af16: *stop=0x8c64af16u;r[6]=r[9];goto L8c64af18; /* mov r9,r6 */
L8c64af18: *stop=0x8c64af18u;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64af1a; /* mov.l @(0xc,r5),r5 */
L8c64af1a: *stop=0x8c64af1au;r[5]+=r[14];goto L8c64af1c; /* add r14,r5 */
L8c64af1c: target=0x8c64b18eu;*pr=0x8c64af20u;*stop=0x8c64af1eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64af20; /* bsr 0x8c64b18e */
L8c64af20: *stop=0x8c64af22u;r[0]=r[11];goto L8c64af38; /* bra 0x8c64af38 */
L8c64af24: *stop=0x8c64af24u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64af26; /* mov.l @(0x1c,r13),r5 */
L8c64af26: *stop=0x8c64af26u;r[6]=r[8];goto L8c64af28; /* mov r8,r6 */
L8c64af28: *stop=0x8c64af28u;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64af2a; /* mov.l @(0xc,r5),r5 */
L8c64af2a: *stop=0x8c64af2au;r[5]+=r[14];goto L8c64af2c; /* add r14,r5 */
L8c64af2c: target=0x8c64b18eu;*pr=0x8c64af30u;*stop=0x8c64af2eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64af30; /* bsr 0x8c64b18e */
L8c64af30: *stop=0x8c64af30u;r[14]+=1u;goto L8c64af32; /* add #0x1,r14 */
L8c64af32: *stop=0x8c64af32u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]);goto L8c64af34; /* cmp/ge r10,r14 */
L8c64af34: if(!(*sr&1u))goto L8c64aef8;goto L8c64af36; /* bf 0x8c64aef8 */
L8c64af36: *stop=0x8c64af36u;r[0]=0u;goto L8c64af38; /* mov #0x0,r0 */
L8c64af38: *stop=0x8c64af38u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64af3a; /* lds.l @r15+,PR */
L8c64af3a: *stop=0x8c64af3au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64af3c; /* mov.l @r15+,r8 */
L8c64af3c: *stop=0x8c64af3cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64af3e; /* mov.l @r15+,r9 */
L8c64af3e: *stop=0x8c64af3eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64af40; /* mov.l @r15+,r10 */
L8c64af40: *stop=0x8c64af40u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64af42; /* mov.l @r15+,r11 */
L8c64af42: *stop=0x8c64af42u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64af44; /* mov.l @r15+,r12 */
L8c64af44: *stop=0x8c64af44u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64af46; /* mov.l @r15+,r13 */
L8c64af46: *stop=0x8c64af46u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64af48u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_8c64aecc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64aecc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64af0e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64aecc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64af0eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64af20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64aecc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64af20u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64af30(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64aecc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64af30u);}

static int sa_movie_filesystem_persist_run_8c64af4a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64af80u)goto L8c64af80;if(resume_pc==0x8c64af98u)goto L8c64af98;goto L8c64af4a;
L8c64af4a: *stop=0x8c64af4au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64af4c; /* mov.l r14,@-r15 */
L8c64af4c: *stop=0x8c64af4cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64af4e; /* mov.l r13,@-r15 */
L8c64af4e: *stop=0x8c64af4eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64af50; /* mov.l r12,@-r15 */
L8c64af50: *stop=0x8c64af50u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64af52; /* mov.l r11,@-r15 */
L8c64af52: *stop=0x8c64af52u;r[5]=sx(m,bus,&io_status,0x8c64afc8u,2);if(io_status)return io_status;goto L8c64af54; /* mov.w 0x8c64afc8,r5 */
L8c64af54: *stop=0x8c64af54u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64af56; /* mov.l r10,@-r15 */
L8c64af56: *stop=0x8c64af56u;r[10]=r[4];goto L8c64af58; /* mov r4,r10 */
L8c64af58: *stop=0x8c64af58u;*macl=r[5]*r[10];goto L8c64af5a; /* mul.l r5,r10 */
L8c64af5a: *stop=0x8c64af5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64afccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64af5c; /* mov.l 0x8c64afcc,r3 */
L8c64af5c: *stop=0x8c64af5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64af5e; /* mov.l r9,@-r15 */
L8c64af5e: *stop=0x8c64af5eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64af60; /* mov.l r8,@-r15 */
L8c64af60: *stop=0x8c64af60u;r[5]=*macl;goto L8c64af62; /* sts MACL,r5 */
L8c64af62: *stop=0x8c64af62u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64af64; /* sts.l PR,@-r15 */
L8c64af64: *stop=0x8c64af64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64afd8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64af66; /* mov.l 0x8c64afd8,r11 */
L8c64af66: *stop=0x8c64af66u;r[5]+=r[3];goto L8c64af68; /* add r3,r5 */
L8c64af68: *stop=0x8c64af68u;if(!valid_access(r[5]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64af6a; /* mov.l @(0x1c,r5),r4 */
L8c64af6a: *stop=0x8c64af6au;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c64af6c; /* mov.l @(0x20,r4),r9 */
L8c64af6c: *stop=0x8c64af6cu;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c64af6e; /* mov.l @(0x1c,r4),r8 */
L8c64af6e: *stop=0x8c64af6eu;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64af70; /* mov.l @(0x10,r4),r12 */
L8c64af70: *stop=0x8c64af70u;r[9]+=r[8];goto L8c64af72; /* add r8,r9 */
L8c64af72: *stop=0x8c64af72u;r[9]+=4294967295u;goto L8c64af74; /* add #-0x1,r9 */
L8c64af74: *stop=0x8c64af74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64afdcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64af76; /* mov.l 0x8c64afdc,r3 */
L8c64af76: *stop=0x8c64af76u;r[5]=r[11];goto L8c64af78; /* mov r11,r5 */
L8c64af78: *stop=0x8c64af78u;r[7]=r[9];goto L8c64af7a; /* mov r9,r7 */
L8c64af7a: *stop=0x8c64af7au;r[6]=r[8];goto L8c64af7c; /* mov r8,r6 */
L8c64af7c: target=r[3];*pr=0x8c64af80u;*stop=0x8c64af7eu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64af80; /* jsr @r3 */
L8c64af80: *stop=0x8c64af80u;r[13]=r[0];goto L8c64af82; /* mov r0,r13 */
L8c64af82: *stop=0x8c64af82u;*sr=(*sr&~1u)|((int32_t)r[13]>=0);goto L8c64af84; /* cmp/pz r13 */
L8c64af84: if((*sr&1u))goto L8c64af8a;goto L8c64af86; /* bt 0x8c64af8a */
L8c64af86: *stop=0x8c64af88u;r[0]=4294967295u;goto L8c64afb6; /* bra 0x8c64afb6 */
L8c64af8a: *stop=0x8c64af8au;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c64af8c; /* cmp/pl r12 */
L8c64af8c: take=!(*sr&1u);*stop=0x8c64af8eu;r[14]=0u;if(take)goto L8c64afac;goto L8c64af90; /* bf/s 0x8c64afac */
L8c64af90: *stop=0x8c64af90u;r[5]=r[13];goto L8c64af92; /* mov r13,r5 */
L8c64af92: *stop=0x8c64af92u;r[5]-=r[14];goto L8c64af94; /* sub r14,r5 */
L8c64af94: target=0x8c64b1c0u;*pr=0x8c64af98u;*stop=0x8c64af96u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64af98; /* bsr 0x8c64b1c0 */
L8c64af98: *stop=0x8c64af98u;r[0]=r[0]&65535u;goto L8c64af9a; /* extu.w r0,r0 */
L8c64af9a: *stop=0x8c64af9au;*sr=(*sr&~1u)|(r[11]==r[0]);goto L8c64af9c; /* cmp/eq r11,r0 */
L8c64af9c: if((*sr&1u))goto L8c64afa6;goto L8c64af9e; /* bt 0x8c64afa6 */
L8c64af9e: *stop=0x8c64af9eu;r[9]=r[13];goto L8c64afa0; /* mov r13,r9 */
L8c64afa0: *stop=0x8c64afa0u;r[9]-=r[14];goto L8c64afa2; /* sub r14,r9 */
L8c64afa2: *stop=0x8c64afa4u;r[9]+=4294967295u;goto L8c64afac; /* bra 0x8c64afac */
L8c64afa6: *stop=0x8c64afa6u;r[14]+=1u;goto L8c64afa8; /* add #0x1,r14 */
L8c64afa8: *stop=0x8c64afa8u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[12]);goto L8c64afaa; /* cmp/ge r12,r14 */
L8c64afaa: if(!(*sr&1u))goto L8c64af90;goto L8c64afac; /* bf 0x8c64af90 */
L8c64afac: *stop=0x8c64afacu;*sr=(*sr&~1u)|(r[12]==r[14]);goto L8c64afae; /* cmp/eq r12,r14 */
L8c64afae: if(!(*sr&1u))goto L8c64af74;goto L8c64afb0; /* bf 0x8c64af74 */
L8c64afb0: *stop=0x8c64afb0u;r[13]-=r[12];goto L8c64afb2; /* sub r12,r13 */
L8c64afb2: *stop=0x8c64afb2u;r[0]=r[13];goto L8c64afb4; /* mov r13,r0 */
L8c64afb4: *stop=0x8c64afb4u;r[0]+=1u;goto L8c64afb6; /* add #0x1,r0 */
L8c64afb6: *stop=0x8c64afb6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64afb8; /* lds.l @r15+,PR */
L8c64afb8: *stop=0x8c64afb8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64afba; /* mov.l @r15+,r8 */
L8c64afba: *stop=0x8c64afbau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64afbc; /* mov.l @r15+,r9 */
L8c64afbc: *stop=0x8c64afbcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64afbe; /* mov.l @r15+,r10 */
L8c64afbe: *stop=0x8c64afbeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64afc0; /* mov.l @r15+,r11 */
L8c64afc0: *stop=0x8c64afc0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64afc2; /* mov.l @r15+,r12 */
L8c64afc2: *stop=0x8c64afc2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64afc4; /* mov.l @r15+,r13 */
L8c64afc4: *stop=0x8c64afc4u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64afc6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_8c64af4a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64af4a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64af80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64af4a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64af80u);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c64af98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c64af4a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64af98u);}

static int sa_movie_filesystem_persist_run_8c6489d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c648a02u)goto L8c648a02;goto L8c6489d4;
L8c6489d4: *stop=0x8c6489d4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6489d6; /* mov.l r14,@-r15 */
L8c6489d6: *stop=0x8c6489d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6489d8; /* mov.l r13,@-r15 */
L8c6489d8: *stop=0x8c6489d8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6489da; /* mov.l r12,@-r15 */
L8c6489da: *stop=0x8c6489dau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6489dc; /* mov.l r11,@-r15 */
L8c6489dc: *stop=0x8c6489dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6489de; /* sts.l PR,@-r15 */
L8c6489de: *stop=0x8c6489deu;r[15]+=4294967292u;goto L8c6489e0; /* add #-0x4,r15 */
L8c6489e0: *stop=0x8c6489e0u;r[12]=sx(m,bus,&io_status,0x8c648ab0u,2);if(io_status)return io_status;goto L8c6489e2; /* mov.w 0x8c648ab0,r12 */
L8c6489e2: *stop=0x8c6489e2u;r[14]=r[7];goto L8c6489e4; /* mov r7,r14 */
L8c6489e4: *stop=0x8c6489e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648abcu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6489e6; /* mov.l 0x8c648abc,r11 */
L8c6489e6: *stop=0x8c6489e6u;r[13]=r[6];goto L8c6489e8; /* mov r6,r13 */
L8c6489e8: *stop=0x8c6489e8u;*macl=r[12]*r[4];goto L8c6489ea; /* mul.l r12,r4 */
L8c6489ea: *stop=0x8c6489eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c648ab8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6489ec; /* mov.l 0x8c648ab8,r3 */
L8c6489ec: *stop=0x8c6489ecu;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[13]);goto L8c6489ee; /* cmp/ge r13,r14 */
L8c6489ee: *stop=0x8c6489eeu;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[5],2);if(io_status)return io_status;goto L8c6489f0; /* mov.w r5,@r15 */
L8c6489f0: *stop=0x8c6489f0u;r[12]=*macl;goto L8c6489f2; /* sts MACL,r12 */
L8c6489f2: take=!(*sr&1u);*stop=0x8c6489f4u;r[12]+=r[3];if(take)goto L8c648a16;goto L8c6489f6; /* bf/s 0x8c648a16 */
L8c6489f6: *stop=0x8c6489f6u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6489f8; /* mov.l @r11,r3 */
L8c6489f8: *stop=0x8c6489f8u;r[5]=r[14];goto L8c6489fa; /* mov r14,r5 */
L8c6489fa: *stop=0x8c6489fau;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6489fc; /* shll r5 */
L8c6489fc: *stop=0x8c6489fcu;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6489fe; /* mov.l @(0xc,r3),r2 */
L8c6489fe: target=r[2];*pr=0x8c648a02u;*stop=0x8c648a00u;if(!valid_access(r[12]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x24u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648a02; /* jsr @r2 */
L8c648a02: *stop=0x8c648a02u;if(!valid_access(r[15],2u))return -3;r[3]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c648a04; /* mov.w @r15,r3 */
L8c648a04: *stop=0x8c648a04u;r[0]=r[0]&65535u;goto L8c648a06; /* extu.w r0,r0 */
L8c648a06: *stop=0x8c648a06u;r[3]=r[3]&65535u;goto L8c648a08; /* extu.w r3,r3 */
L8c648a08: *stop=0x8c648a08u;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c648a0a; /* cmp/eq r3,r0 */
L8c648a0a: if(!(*sr&1u))goto L8c648a10;goto L8c648a0c; /* bf 0x8c648a10 */
L8c648a0c: *stop=0x8c648a0eu;r[0]=r[14];goto L8c648a18; /* bra 0x8c648a18 */
L8c648a10: *stop=0x8c648a10u;r[14]+=4294967295u;goto L8c648a12; /* add #-0x1,r14 */
L8c648a12: *stop=0x8c648a12u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[13]);goto L8c648a14; /* cmp/ge r13,r14 */
L8c648a14: if((*sr&1u))goto L8c6489f6;goto L8c648a16; /* bt 0x8c6489f6 */
L8c648a16: *stop=0x8c648a16u;r[0]=4294967295u;goto L8c648a18; /* mov #-0x1,r0 */
L8c648a18: *stop=0x8c648a18u;r[15]+=4u;goto L8c648a1a; /* add #0x4,r15 */
L8c648a1a: *stop=0x8c648a1au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c648a1c; /* lds.l @r15+,PR */
L8c648a1c: *stop=0x8c648a1cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c648a1e; /* mov.l @r15+,r11 */
L8c648a1e: *stop=0x8c648a1eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c648a20; /* mov.l @r15+,r12 */
L8c648a20: *stop=0x8c648a20u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c648a22; /* mov.l @r15+,r13 */
L8c648a22: *stop=0x8c648a22u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c648a24u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_8c6489d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c6489d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_persist_resume_8c648a02(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_persist_run_8c6489d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648a02u);}
