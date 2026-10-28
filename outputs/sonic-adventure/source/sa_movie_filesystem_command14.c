/* Original command14 file range read and seek.
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
static int sa_movie_filesystem_command14_run_8c64bbb2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64bbd6u)goto L8c64bbd6;if(resume_pc==0x8c64bbf0u)goto L8c64bbf0;if(resume_pc==0x8c64bc10u)goto L8c64bc10;if(resume_pc==0x8c64bc22u)goto L8c64bc22;if(resume_pc==0x8c64bc3au)goto L8c64bc3a;goto L8c64bbb2;
L8c64bbb2: *stop=0x8c64bbb2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64bbb4; /* mov.l r14,@-r15 */
L8c64bbb4: *stop=0x8c64bbb4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64bbb6; /* mov.l r13,@-r15 */
L8c64bbb6: *stop=0x8c64bbb6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64bbb8; /* mov.l r12,@-r15 */
L8c64bbb8: *stop=0x8c64bbb8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64bbba; /* mov.l r11,@-r15 */
L8c64bbba: *stop=0x8c64bbbau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64bbbc; /* mov.l r10,@-r15 */
L8c64bbbc: *stop=0x8c64bbbcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64bbbe; /* sts.l PR,@-r15 */
L8c64bbbe: *stop=0x8c64bbbeu;r[15]+=4294967280u;goto L8c64bbc0; /* add #-0x10,r15 */
L8c64bbc0: *stop=0x8c64bbc0u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c64bbc2; /* mov.l r5,@(0x8,r15) */
L8c64bbc2: *stop=0x8c64bbc2u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[6],4);if(io_status)return io_status;goto L8c64bbc4; /* mov.l r6,@(0xc,r15) */
L8c64bbc4: *stop=0x8c64bbc4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[7],4);if(io_status)return io_status;goto L8c64bbc6; /* mov.l r7,@(0x4,r15) */
L8c64bbc6: *stop=0x8c64bbc6u;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64bbc8; /* mov.l @(0x28,r15),r13 */
L8c64bbc8: *stop=0x8c64bbc8u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c64bbca; /* tst r13,r13 */
L8c64bbca: take=!(*sr&1u);*stop=0x8c64bbccu;r[11]=r[4];if(take)goto L8c64bbe6;goto L8c64bbce; /* bf/s 0x8c64bbe6 */
L8c64bbce: *stop=0x8c64bbceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bc58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bbd0; /* mov.l 0x8c64bc58,r3 */
L8c64bbd0: *stop=0x8c64bbd0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64bbd2; /* mov.l @(0x8,r15),r5 */
L8c64bbd2: target=r[3];*pr=0x8c64bbd6u;*stop=0x8c64bbd4u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bbd6; /* jsr @r3 */
L8c64bbd6: *stop=0x8c64bbd6u;r[4]=r[0];goto L8c64bbd8; /* mov r0,r4 */
L8c64bbd8: *stop=0x8c64bbd8u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64bbda; /* cmp/pz r4 */
L8c64bbda: if((*sr&1u))goto L8c64bbe0;goto L8c64bbdc; /* bt 0x8c64bbe0 */
L8c64bbdc: *stop=0x8c64bbdeu;r[0]=r[4];goto L8c64bc3c; /* bra 0x8c64bc3c */
L8c64bbe0: *stop=0x8c64bbe0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bbe2; /* mov.l @(0x4,r15),r3 */
L8c64bbe2: *stop=0x8c64bbe2u;r[13]=r[4];goto L8c64bbe4; /* mov r4,r13 */
L8c64bbe4: *stop=0x8c64bbe4u;r[13]-=r[3];goto L8c64bbe6; /* sub r3,r13 */
L8c64bbe6: *stop=0x8c64bbe6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bc5cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bbe8; /* mov.l 0x8c64bc5c,r3 */
L8c64bbe8: *stop=0x8c64bbe8u;r[6]=r[15];goto L8c64bbea; /* mov r15,r6 */
L8c64bbea: *stop=0x8c64bbeau;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64bbec; /* mov.l @(0x8,r15),r5 */
L8c64bbec: target=r[3];*pr=0x8c64bbf0u;*stop=0x8c64bbeeu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bbf0; /* jsr @r3 */
L8c64bbf0: *stop=0x8c64bbf0u;r[14]=r[0];goto L8c64bbf2; /* mov r0,r14 */
L8c64bbf2: *stop=0x8c64bbf2u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64bbf4; /* cmp/pz r14 */
L8c64bbf4: if(!(*sr&1u))goto L8c64bc3a;goto L8c64bbf6; /* bf 0x8c64bc3a */
L8c64bbf6: *stop=0x8c64bbf6u;r[4]=sx(m,bus,&io_status,0x8c64bc4cu,2);if(io_status)return io_status;goto L8c64bbf8; /* mov.w 0x8c64bc4c,r4 */
L8c64bbf8: *stop=0x8c64bbf8u;r[2]=1u;goto L8c64bbfa; /* mov #0x1,r2 */
L8c64bbfa: *stop=0x8c64bbfau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bc60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bbfc; /* mov.l 0x8c64bc60,r3 */
L8c64bbfc: *stop=0x8c64bbfcu;r[6]=0u;goto L8c64bbfe; /* mov #0x0,r6 */
L8c64bbfe: *stop=0x8c64bbfeu;*macl=r[4]*r[11];goto L8c64bc00; /* mul.l r4,r11 */
L8c64bc00: *stop=0x8c64bc00u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64bc02; /* mov.l @(0xc,r15),r12 */
L8c64bc02: *stop=0x8c64bc02u;r[4]=*macl;goto L8c64bc04; /* sts MACL,r4 */
L8c64bc04: *stop=0x8c64bc04u;r[4]+=r[3];goto L8c64bc06; /* add r3,r4 */
L8c64bc06: *stop=0x8c64bc06u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[2],4);if(io_status)return io_status;goto L8c64bc08; /* mov.l r2,@(0x30,r4) */
L8c64bc08: *stop=0x8c64bc08u;if(!valid_access(r[4]+52u,4u))return -3;wr(m,bus,&io_status,r[4]+0x34u,r[13],4);if(io_status)return io_status;goto L8c64bc0a; /* mov.l r13,@(0x34,r4) */
L8c64bc0a: *stop=0x8c64bc0au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64bc0c; /* mov.l @(0x4,r15),r5 */
L8c64bc0c: target=0x8c64bb4cu;*pr=0x8c64bc10u;*stop=0x8c64bc0eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bc10; /* bsr 0x8c64bb4c */
L8c64bc10: *stop=0x8c64bc10u;r[11]=0u;goto L8c64bc12; /* mov #0x0,r11 */
L8c64bc12: *stop=0x8c64bc12u;r[3]=r[11];goto L8c64bc14; /* mov r11,r3 */
L8c64bc14: *stop=0x8c64bc14u;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c64bc16; /* cmp/hs r13,r3 */
L8c64bc16: if((*sr&1u))goto L8c64bc30;goto L8c64bc18; /* bt 0x8c64bc30 */
L8c64bc18: *stop=0x8c64bc18u;r[10]=sx(m,bus,&io_status,0x8c64bc4eu,2);if(io_status)return io_status;goto L8c64bc1a; /* mov.w 0x8c64bc4e,r10 */
L8c64bc1a: *stop=0x8c64bc1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bc64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bc1c; /* mov.l 0x8c64bc64,r3 */
L8c64bc1c: *stop=0x8c64bc1cu;r[5]=r[12];goto L8c64bc1e; /* mov r12,r5 */
L8c64bc1e: target=r[3];*pr=0x8c64bc22u;*stop=0x8c64bc20u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bc22; /* jsr @r3 */
L8c64bc22: *stop=0x8c64bc22u;r[14]=r[0];goto L8c64bc24; /* mov r0,r14 */
L8c64bc24: *stop=0x8c64bc24u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64bc26; /* cmp/pz r14 */
L8c64bc26: if(!(*sr&1u))goto L8c64bc32;goto L8c64bc28; /* bf 0x8c64bc32 */
L8c64bc28: *stop=0x8c64bc28u;r[11]+=1u;goto L8c64bc2a; /* add #0x1,r11 */
L8c64bc2a: *stop=0x8c64bc2au;*sr=(*sr&~1u)|(r[11]>=r[13]);goto L8c64bc2c; /* cmp/hs r13,r11 */
L8c64bc2c: take=!(*sr&1u);*stop=0x8c64bc2eu;r[12]+=r[10];if(take)goto L8c64bc1a;goto L8c64bc30; /* bf/s 0x8c64bc1a */
L8c64bc30: *stop=0x8c64bc30u;r[14]=0u;goto L8c64bc32; /* mov #0x0,r14 */
L8c64bc32: *stop=0x8c64bc32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bc68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bc34; /* mov.l 0x8c64bc68,r3 */
L8c64bc34: *stop=0x8c64bc34u;r[5]=0u;goto L8c64bc36; /* mov #0x0,r5 */
L8c64bc36: target=r[3];*pr=0x8c64bc3au;*stop=0x8c64bc38u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bc3a; /* jsr @r3 */
L8c64bc3a: *stop=0x8c64bc3au;r[0]=r[14];goto L8c64bc3c; /* mov r14,r0 */
L8c64bc3c: *stop=0x8c64bc3cu;r[15]+=16u;goto L8c64bc3e; /* add #0x10,r15 */
L8c64bc3e: *stop=0x8c64bc3eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64bc40; /* lds.l @r15+,PR */
L8c64bc40: *stop=0x8c64bc40u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64bc42; /* mov.l @r15+,r10 */
L8c64bc42: *stop=0x8c64bc42u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64bc44; /* mov.l @r15+,r11 */
L8c64bc44: *stop=0x8c64bc44u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64bc46; /* mov.l @r15+,r12 */
L8c64bc46: *stop=0x8c64bc46u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64bc48; /* mov.l @r15+,r13 */
L8c64bc48: *stop=0x8c64bc48u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64bc4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command14_8c64bbb2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command14_run_8c64bbb2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command14_resume_8c64bbd6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command14_run_8c64bbb2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bbd6u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command14_resume_8c64bbf0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command14_run_8c64bbb2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bbf0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command14_resume_8c64bc10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command14_run_8c64bbb2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bc10u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command14_resume_8c64bc22(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command14_run_8c64bbb2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bc22u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command14_resume_8c64bc3a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command14_run_8c64bbb2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bc3au);}

static int sa_movie_filesystem_command14_run_8c64bb4c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64bb90u)goto L8c64bb90;goto L8c64bb4c;
L8c64bb4c: *stop=0x8c64bb4cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64bb4e; /* mov.l r14,@-r15 */
L8c64bb4e: *stop=0x8c64bb4eu;r[14]=r[4];goto L8c64bb50; /* mov r4,r14 */
L8c64bb50: *stop=0x8c64bb50u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64bb52; /* mov.l r13,@-r15 */
L8c64bb52: *stop=0x8c64bb52u;r[0]=64u;goto L8c64bb54; /* mov #0x40,r0 */
L8c64bb54: *stop=0x8c64bb54u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64bb56; /* mov.l r12,@-r15 */
L8c64bb56: *stop=0x8c64bb56u;r[13]=0u;goto L8c64bb58; /* mov #0x0,r13 */
L8c64bb58: *stop=0x8c64bb58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64bb5a; /* mov.l r11,@-r15 */
L8c64bb5a: *stop=0x8c64bb5au;r[12]=r[5];goto L8c64bb5c; /* mov r5,r12 */
L8c64bb5c: *stop=0x8c64bb5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64bb5e; /* sts.l PR,@-r15 */
L8c64bb5e: *stop=0x8c64bb5eu;if(!valid_access(r[14]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bb60; /* mov.l @(0x38,r14),r3 */
L8c64bb60: *stop=0x8c64bb60u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64bb62; /* mov.l @(0x28,r14),r11 */
L8c64bb62: *stop=0x8c64bb62u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c64bb64; /* mov.l r3,@(r0,r14) */
L8c64bb64: *stop=0x8c64bb64u;r[3]=r[13];goto L8c64bb66; /* mov r13,r3 */
L8c64bb66: *stop=0x8c64bb66u;*sr=(*sr&~1u)|(r[3]>=r[12]);goto L8c64bb68; /* cmp/hs r12,r3 */
L8c64bb68: if((*sr&1u))goto L8c64bba4;goto L8c64bb6a; /* bt 0x8c64bba4 */
L8c64bb6a: *stop=0x8c64bb6au;r[0]=64u;goto L8c64bb6c; /* mov #0x40,r0 */
L8c64bb6c: *stop=0x8c64bb6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bc50u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64bb6e; /* mov.l 0x8c64bc50,r4 */
L8c64bb6e: *stop=0x8c64bb6eu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bb70; /* mov.l @(r0,r14),r3 */
L8c64bb70: *stop=0x8c64bb70u;*sr=(*sr&~1u)|(r[4]==r[3]);goto L8c64bb72; /* cmp/eq r4,r3 */
L8c64bb72: if(!(*sr&1u))goto L8c64bb7e;goto L8c64bb74; /* bf 0x8c64bb7e */
L8c64bb74: *stop=0x8c64bb74u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64bb76; /* mov.l @(0x34,r14),r1 */
L8c64bb76: *stop=0x8c64bb76u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c64bb78; /* tst r1,r1 */
L8c64bb78: if(!(*sr&1u))goto L8c64bb84;goto L8c64bb7a; /* bf 0x8c64bb84 */
L8c64bb7a: *stop=0x8c64bb7cu;r[0]=0u;goto L8c64bba6; /* bra 0x8c64bba6 */
L8c64bb7e: *stop=0x8c64bb7eu;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bb80; /* mov.l @(0x34,r14),r3 */
L8c64bb80: *stop=0x8c64bb80u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64bb82; /* tst r3,r3 */
L8c64bb82: if(!(*sr&1u))goto L8c64bb8a;goto L8c64bb84; /* bf 0x8c64bb8a */
L8c64bb84: *stop=0x8c64bb84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bc54u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64bb86; /* mov.l 0x8c64bc54,r0 */
L8c64bb86: *stop=0x8c64bb88u;goto L8c64bba6; /* bra 0x8c64bba6 */
L8c64bb8a: *stop=0x8c64bb8au;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64bb8c; /* mov.l @(r0,r14),r5 */
L8c64bb8c: target=0x8c64b1c0u;*pr=0x8c64bb90u;*stop=0x8c64bb8eu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bb90; /* bsr 0x8c64b1c0 */
L8c64bb90: *stop=0x8c64bb90u;r[1]=64u;goto L8c64bb92; /* mov #0x40,r1 */
L8c64bb92: *stop=0x8c64bb92u;r[13]+=1u;goto L8c64bb94; /* add #0x1,r13 */
L8c64bb94: *stop=0x8c64bb94u;r[0]=r[0]&65535u;goto L8c64bb96; /* extu.w r0,r0 */
L8c64bb96: *stop=0x8c64bb96u;*sr=(*sr&~1u)|(r[13]>=r[12]);goto L8c64bb98; /* cmp/hs r12,r13 */
L8c64bb98: *stop=0x8c64bb98u;r[1]+=r[14];goto L8c64bb9a; /* add r14,r1 */
L8c64bb9a: *stop=0x8c64bb9au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c64bb9c; /* mov.l r0,@r1 */
L8c64bb9c: *stop=0x8c64bb9cu;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bb9e; /* mov.l @(0x34,r14),r3 */
L8c64bb9e: *stop=0x8c64bb9eu;r[3]+=4294967295u;goto L8c64bba0; /* add #-0x1,r3 */
L8c64bba0: take=!(*sr&1u);*stop=0x8c64bba2u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[3],4);if(io_status)return io_status;if(take)goto L8c64bb6a;goto L8c64bba4; /* bf/s 0x8c64bb6a */
L8c64bba4: *stop=0x8c64bba4u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64bba6; /* mov.l @(0x34,r14),r0 */
L8c64bba6: *stop=0x8c64bba6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64bba8; /* lds.l @r15+,PR */
L8c64bba8: *stop=0x8c64bba8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64bbaa; /* mov.l @r15+,r11 */
L8c64bbaa: *stop=0x8c64bbaau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64bbac; /* mov.l @r15+,r12 */
L8c64bbac: *stop=0x8c64bbacu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64bbae; /* mov.l @r15+,r13 */
L8c64bbae: *stop=0x8c64bbaeu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64bbb0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command14_8c64bb4c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command14_run_8c64bb4c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command14_resume_8c64bb90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command14_run_8c64bb4c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bb90u);}
