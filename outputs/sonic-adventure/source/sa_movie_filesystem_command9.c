/* Original command9 storage-block relocation and actual metadata helpers.
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
static int sa_movie_filesystem_command9_run_8c64a828(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64a854u)goto L8c64a854;if(resume_pc==0x8c64a85eu)goto L8c64a85e;if(resume_pc==0x8c64a882u)goto L8c64a882;if(resume_pc==0x8c64a89eu)goto L8c64a89e;if(resume_pc==0x8c64a8b8u)goto L8c64a8b8;if(resume_pc==0x8c64a8c2u)goto L8c64a8c2;if(resume_pc==0x8c64a8d0u)goto L8c64a8d0;if(resume_pc==0x8c64a8deu)goto L8c64a8de;if(resume_pc==0x8c64a8eeu)goto L8c64a8ee;if(resume_pc==0x8c64a8feu)goto L8c64a8fe;if(resume_pc==0x8c64a90au)goto L8c64a90a;if(resume_pc==0x8c64a916u)goto L8c64a916;if(resume_pc==0x8c64a920u)goto L8c64a920;if(resume_pc==0x8c64a92eu)goto L8c64a92e;if(resume_pc==0x8c64a934u)goto L8c64a934;if(resume_pc==0x8c64a93cu)goto L8c64a93c;if(resume_pc==0x8c64a946u)goto L8c64a946;if(resume_pc==0x8c64a988u)goto L8c64a988;if(resume_pc==0x8c64a992u)goto L8c64a992;goto L8c64a828;
L8c64a828: *stop=0x8c64a828u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a82a; /* mov.l r14,@-r15 */
L8c64a82a: *stop=0x8c64a82au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64a82c; /* mov.l r13,@-r15 */
L8c64a82c: *stop=0x8c64a82cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64a82e; /* mov.l r12,@-r15 */
L8c64a82e: *stop=0x8c64a82eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64a830; /* mov.l r11,@-r15 */
L8c64a830: *stop=0x8c64a830u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64a832; /* mov.l r10,@-r15 */
L8c64a832: *stop=0x8c64a832u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64a834; /* mov.l r9,@-r15 */
L8c64a834: *stop=0x8c64a834u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64a836; /* sts.l PR,@-r15 */
L8c64a836: *stop=0x8c64a836u;r[15]+=4294967276u;goto L8c64a838; /* add #-0x14,r15 */
L8c64a838: *stop=0x8c64a838u;r[11]=sx(m,bus,&io_status,0x8c64a862u,2);if(io_status)return io_status;goto L8c64a83a; /* mov.w 0x8c64a862,r11 */
L8c64a83a: *stop=0x8c64a83au;r[14]=r[4];goto L8c64a83c; /* mov r4,r14 */
L8c64a83c: *stop=0x8c64a83cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a864u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a83e; /* mov.l 0x8c64a864,r3 */
L8c64a83e: *stop=0x8c64a83eu;*macl=r[11]*r[14];goto L8c64a840; /* mul.l r11,r14 */
L8c64a840: *stop=0x8c64a840u;r[11]=*macl;goto L8c64a842; /* sts MACL,r11 */
L8c64a842: *stop=0x8c64a842u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64a844; /* mov.l r11,@r15 */
L8c64a844: *stop=0x8c64a844u;r[11]+=r[3];goto L8c64a846; /* add r3,r11 */
L8c64a846: *stop=0x8c64a846u;if(!valid_access(r[11]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[11]+0x2u,1);if(io_status)return io_status;goto L8c64a848; /* mov.b @(0x2,r11),r0 */
L8c64a848: *stop=0x8c64a848u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a84a; /* tst r0,r0 */
L8c64a84a: take=(*sr&1u);*stop=0x8c64a84cu;r[10]=r[5];if(take)goto L8c64a892;goto L8c64a84e; /* bt/s 0x8c64a892 */
L8c64a84e: *stop=0x8c64a84eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a874u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a850; /* mov.l 0x8c64a874,r3 */
L8c64a850: target=r[3];*pr=0x8c64a854u;*stop=0x8c64a852u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a854; /* jsr @r3 */
L8c64a854: *stop=0x8c64a854u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a856; /* tst r0,r0 */
L8c64a856: if(!(*sr&1u))goto L8c64a87c;goto L8c64a858; /* bf 0x8c64a87c */
L8c64a858: *stop=0x8c64a858u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a878u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a85a; /* mov.l 0x8c64a878,r3 */
L8c64a85a: target=r[3];*pr=0x8c64a85eu;*stop=0x8c64a85cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a85e; /* jsr @r3 */
L8c64a85e: *stop=0x8c64a860u;goto L8c64a892; /* bra 0x8c64a892 */
L8c64a87c: *stop=0x8c64a87cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a950u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a87e; /* mov.l 0x8c64a950,r2 */
L8c64a87e: target=r[2];*pr=0x8c64a882u;*stop=0x8c64a880u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a882; /* jsr @r2 */
L8c64a882: *stop=0x8c64a882u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a884; /* tst r0,r0 */
L8c64a884: if((*sr&1u))goto L8c64a892;goto L8c64a886; /* bt 0x8c64a892 */
L8c64a886: *stop=0x8c64a886u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a888; /* mov.l @r15,r2 */
L8c64a888: *stop=0x8c64a888u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a954u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a88a; /* mov.l 0x8c64a954,r3 */
L8c64a88a: *stop=0x8c64a88au;r[2]+=r[3];goto L8c64a88c; /* add r3,r2 */
L8c64a88c: *stop=0x8c64a88cu;if(!valid_access(r[2]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x8u,1);if(io_status)return io_status;goto L8c64a88e; /* mov.b @(0x8,r2),r0 */
L8c64a88e: *stop=0x8c64a88eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a890; /* tst r0,r0 */
L8c64a890: if(!(*sr&1u))goto L8c64a898;goto L8c64a892; /* bf 0x8c64a898 */
L8c64a892: *stop=0x8c64a892u;r[0]=sx(m,bus,&io_status,0x8c64a94au,2);if(io_status)return io_status;goto L8c64a894; /* mov.w 0x8c64a94a,r0 */
L8c64a894: *stop=0x8c64a896u;goto L8c64a996; /* bra 0x8c64a996 */
L8c64a898: *stop=0x8c64a898u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a958u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a89a; /* mov.l 0x8c64a958,r3 */
L8c64a89a: target=r[3];*pr=0x8c64a89eu;*stop=0x8c64a89cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a89e; /* jsr @r3 */
L8c64a89e: *stop=0x8c64a89eu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64a8a0; /* cmp/pz r0 */
L8c64a8a0: if((*sr&1u))goto L8c64a8a8;goto L8c64a8a2; /* bt 0x8c64a8a8 */
L8c64a8a2: *stop=0x8c64a8a2u;r[0]=sx(m,bus,&io_status,0x8c64a94cu,2);if(io_status)return io_status;goto L8c64a8a4; /* mov.w 0x8c64a94c,r0 */
L8c64a8a4: *stop=0x8c64a8a6u;goto L8c64a996; /* bra 0x8c64a996 */
L8c64a8a8: *stop=0x8c64a8a8u;if(!valid_access(r[11]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a8aa; /* mov.l @(0x1c,r11),r3 */
L8c64a8aa: *stop=0x8c64a8aau;if(!valid_access(r[3]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a8ac; /* mov.l @(0x38,r3),r2 */
L8c64a8ac: *stop=0x8c64a8acu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64a8ae; /* tst r2,r2 */
L8c64a8ae: if((*sr&1u))goto L8c64a8c6;goto L8c64a8b0; /* bt 0x8c64a8c6 */
L8c64a8b0: *stop=0x8c64a8b0u;r[5]=r[15];goto L8c64a8b2; /* mov r15,r5 */
L8c64a8b2: *stop=0x8c64a8b2u;r[5]+=4u;goto L8c64a8b4; /* add #0x4,r5 */
L8c64a8b4: target=0x8c64a3fau;*pr=0x8c64a8b8u;*stop=0x8c64a8b6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a8b8; /* bsr 0x8c64a3fa */
L8c64a8b8: *stop=0x8c64a8b8u;r[4]=r[0];goto L8c64a8ba; /* mov r0,r4 */
L8c64a8ba: *stop=0x8c64a8bau;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64a8bc; /* tst r4,r4 */
L8c64a8bc: if((*sr&1u))goto L8c64a8c6;goto L8c64a8be; /* bt 0x8c64a8c6 */
L8c64a8be: target=0x8c64a7bau;*pr=0x8c64a8c2u;*stop=0x8c64a8c0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a8c2; /* bsr 0x8c64a7ba */
L8c64a8c2: *stop=0x8c64a8c2u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64a8c4; /* cmp/pz r0 */
L8c64a8c4: if((*sr&1u))goto L8c64a8cc;goto L8c64a8c6; /* bt 0x8c64a8cc */
L8c64a8c6: *stop=0x8c64a8c6u;r[0]=sx(m,bus,&io_status,0x8c64a94eu,2);if(io_status)return io_status;goto L8c64a8c8; /* mov.w 0x8c64a94e,r0 */
L8c64a8c8: *stop=0x8c64a8cau;goto L8c64a996; /* bra 0x8c64a996 */
L8c64a8cc: target=0x8c64a6aeu;*pr=0x8c64a8d0u;*stop=0x8c64a8ceu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a8d0; /* bsr 0x8c64a6ae */
L8c64a8d0: *stop=0x8c64a8d0u;r[13]=r[0];goto L8c64a8d2; /* mov r0,r13 */
L8c64a8d2: *stop=0x8c64a8d2u;*sr=(*sr&~1u)|((int32_t)r[13]>=0);goto L8c64a8d4; /* cmp/pz r13 */
L8c64a8d4: if(!(*sr&1u))goto L8c64a96c;goto L8c64a8d6; /* bf 0x8c64a96c */
L8c64a8d6: *stop=0x8c64a8d6u;r[5]=r[13];goto L8c64a8d8; /* mov r13,r5 */
L8c64a8d8: *stop=0x8c64a8d8u;r[5]+=1u;goto L8c64a8da; /* add #0x1,r5 */
L8c64a8da: target=0x8c64a6fcu;*pr=0x8c64a8deu;*stop=0x8c64a8dcu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a8de; /* bsr 0x8c64a6fc */
L8c64a8de: *stop=0x8c64a8deu;r[12]=r[0];goto L8c64a8e0; /* mov r0,r12 */
L8c64a8e0: *stop=0x8c64a8e0u;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c64a8e2; /* cmp/pz r12 */
L8c64a8e2: if(!(*sr&1u))goto L8c64a96c;goto L8c64a8e4; /* bf 0x8c64a96c */
L8c64a8e4: *stop=0x8c64a8e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a95cu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c64a8e6; /* mov.l 0x8c64a95c,r9 */
L8c64a8e6: *stop=0x8c64a8e6u;r[5]=r[13];goto L8c64a8e8; /* mov r13,r5 */
L8c64a8e8: *stop=0x8c64a8e8u;r[6]=r[10];goto L8c64a8ea; /* mov r10,r6 */
L8c64a8ea: target=r[9];*pr=0x8c64a8eeu;*stop=0x8c64a8ecu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a8ee; /* jsr @r9 */
L8c64a8ee: *stop=0x8c64a8eeu;r[4]=r[0];goto L8c64a8f0; /* mov r0,r4 */
L8c64a8f0: *stop=0x8c64a8f0u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64a8f2; /* cmp/pz r4 */
L8c64a8f2: if(!(*sr&1u))goto L8c64a994;goto L8c64a8f4; /* bf 0x8c64a994 */
L8c64a8f4: *stop=0x8c64a8f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a960u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c64a8f6; /* mov.l 0x8c64a960,r9 */
L8c64a8f6: *stop=0x8c64a8f6u;r[5]=r[12];goto L8c64a8f8; /* mov r12,r5 */
L8c64a8f8: *stop=0x8c64a8f8u;r[6]=r[10];goto L8c64a8fa; /* mov r10,r6 */
L8c64a8fa: target=r[9];*pr=0x8c64a8feu;*stop=0x8c64a8fcu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a8fe; /* jsr @r9 */
L8c64a8fe: *stop=0x8c64a8feu;r[4]=r[0];goto L8c64a900; /* mov r0,r4 */
L8c64a900: *stop=0x8c64a900u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64a902; /* cmp/pz r4 */
L8c64a902: if(!(*sr&1u))goto L8c64a994;goto L8c64a904; /* bf 0x8c64a994 */
L8c64a904: *stop=0x8c64a904u;r[5]=r[13];goto L8c64a906; /* mov r13,r5 */
L8c64a906: target=0x8c64b080u;*pr=0x8c64a90au;*stop=0x8c64a908u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a90a; /* bsr 0x8c64b080 */
L8c64a90a: *stop=0x8c64a90au;r[5]=r[0];goto L8c64a90c; /* mov r0,r5 */
L8c64a90c: *stop=0x8c64a90cu;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c64a90e; /* cmp/pl r5 */
L8c64a90e: if(!(*sr&1u))goto L8c64a91a;goto L8c64a910; /* bf 0x8c64a91a */
L8c64a910: *stop=0x8c64a910u;r[6]=r[12];goto L8c64a912; /* mov r12,r6 */
L8c64a912: target=0x8c64b18eu;*pr=0x8c64a916u;*stop=0x8c64a914u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a916; /* bsr 0x8c64b18e */
L8c64a916: *stop=0x8c64a918u;goto L8c64a92e; /* bra 0x8c64a92e */
L8c64a91a: *stop=0x8c64a91au;r[5]=r[13];goto L8c64a91c; /* mov r13,r5 */
L8c64a91c: target=0x8c64a74eu;*pr=0x8c64a920u;*stop=0x8c64a91eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a920; /* bsr 0x8c64a74e */
L8c64a920: *stop=0x8c64a920u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a964u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c64a922; /* mov.l 0x8c64a964,r9 */
L8c64a922: *stop=0x8c64a922u;r[6]=r[12];goto L8c64a924; /* mov r12,r6 */
L8c64a924: *stop=0x8c64a924u;r[4]=r[0];goto L8c64a926; /* mov r0,r4 */
L8c64a926: *stop=0x8c64a926u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a928; /* mov.l @r9,r3 */
L8c64a928: *stop=0x8c64a928u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a92a; /* mov.l @(0x10,r3),r2 */
L8c64a92a: target=r[2];*pr=0x8c64a92eu;*stop=0x8c64a92cu;r[5]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a92e; /* jsr @r2 */
L8c64a92e: *stop=0x8c64a92eu;r[5]=r[13];goto L8c64a930; /* mov r13,r5 */
L8c64a930: target=0x8c64b1c0u;*pr=0x8c64a934u;*stop=0x8c64a932u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a934; /* bsr 0x8c64b1c0 */
L8c64a934: *stop=0x8c64a934u;r[5]=r[12];goto L8c64a936; /* mov r12,r5 */
L8c64a936: *stop=0x8c64a936u;r[6]=r[0];goto L8c64a938; /* mov r0,r6 */
L8c64a938: target=0x8c64b18eu;*pr=0x8c64a93cu;*stop=0x8c64a93au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a93c; /* bsr 0x8c64b18e */
L8c64a93c: *stop=0x8c64a93cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a968u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64a93e; /* mov.l 0x8c64a968,r12 */
L8c64a93e: *stop=0x8c64a93eu;r[5]=r[13];goto L8c64a940; /* mov r13,r5 */
L8c64a940: *stop=0x8c64a940u;r[6]=r[12];goto L8c64a942; /* mov r12,r6 */
L8c64a942: target=0x8c64b18eu;*pr=0x8c64a946u;*stop=0x8c64a944u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a946; /* bsr 0x8c64b18e */
L8c64a946: *stop=0x8c64a948u;goto L8c64a8cc; /* bra 0x8c64a8cc */
L8c64a96c: *stop=0x8c64a96cu;r[5]=sx(m,bus,&io_status,0x8c64aa7eu,2);if(io_status)return io_status;goto L8c64a96e; /* mov.w 0x8c64aa7e,r5 */
L8c64a96e: *stop=0x8c64a96eu;r[4]=r[11];goto L8c64a970; /* mov r11,r4 */
L8c64a970: *stop=0x8c64a970u;r[4]+=72u;goto L8c64a972; /* add #0x48,r4 */
L8c64a972: *stop=0x8c64a974u;r[6]=1u;goto L8c64a97a; /* bra 0x8c64a97a */
L8c64a976: *stop=0x8c64a976u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[6],1);if(io_status)return io_status;goto L8c64a978; /* mov.b r6,@r4 */
L8c64a978: *stop=0x8c64a978u;r[4]+=1u;goto L8c64a97a; /* add #0x1,r4 */
L8c64a97a: *stop=0x8c64a97au;r[2]=r[11];goto L8c64a97c; /* mov r11,r2 */
L8c64a97c: *stop=0x8c64a97cu;r[2]+=72u;goto L8c64a97e; /* add #0x48,r2 */
L8c64a97e: *stop=0x8c64a97eu;r[2]+=r[5];goto L8c64a980; /* add r5,r2 */
L8c64a980: *stop=0x8c64a980u;*sr=(*sr&~1u)|(r[4]>=r[2]);goto L8c64a982; /* cmp/hs r2,r4 */
L8c64a982: if(!(*sr&1u))goto L8c64a976;goto L8c64a984; /* bf 0x8c64a976 */
L8c64a984: target=0x8c64ab36u;*pr=0x8c64a988u;*stop=0x8c64a986u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a988; /* bsr 0x8c64ab36 */
L8c64a988: *stop=0x8c64a988u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64a98a; /* cmp/pz r0 */
L8c64a98a: take=!(*sr&1u);*stop=0x8c64a98cu;r[4]=r[0];if(take)goto L8c64a994;goto L8c64a98e; /* bf/s 0x8c64a994 */
L8c64a98e: target=0x8c64afe0u;*pr=0x8c64a992u;*stop=0x8c64a990u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a992; /* bsr 0x8c64afe0 */
L8c64a992: *stop=0x8c64a992u;r[4]=r[0];goto L8c64a994; /* mov r0,r4 */
L8c64a994: *stop=0x8c64a994u;r[0]=r[4];goto L8c64a996; /* mov r4,r0 */
L8c64a996: *stop=0x8c64a996u;r[15]+=20u;goto L8c64a998; /* add #0x14,r15 */
L8c64a998: *stop=0x8c64a998u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a99a; /* lds.l @r15+,PR */
L8c64a99a: *stop=0x8c64a99au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64a99c; /* mov.l @r15+,r9 */
L8c64a99c: *stop=0x8c64a99cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64a99e; /* mov.l @r15+,r10 */
L8c64a99e: *stop=0x8c64a99eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64a9a0; /* mov.l @r15+,r11 */
L8c64a9a0: *stop=0x8c64a9a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64a9a2; /* mov.l @r15+,r12 */
L8c64a9a2: *stop=0x8c64a9a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64a9a4; /* mov.l @r15+,r13 */
L8c64a9a4: *stop=0x8c64a9a4u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a9a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_8c64a828(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a854(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a854u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a85e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a85eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a882(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a882u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a89e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a89eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a8b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a8b8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a8c2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a8c2u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a8d0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a8d0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a8de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a8deu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a8ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a8eeu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a8fe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a8feu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a90a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a90au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a916(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a916u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a920(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a920u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a92e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a92eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a934(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a934u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a93c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a93cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a946(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a946u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a988(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a988u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a992(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a828(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a992u);}

static int sa_movie_filesystem_command9_run_8c64a3fa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64a41au)goto L8c64a41a;if(resume_pc==0x8c64a424u)goto L8c64a424;if(resume_pc==0x8c64a42eu)goto L8c64a42e;if(resume_pc==0x8c64a46eu)goto L8c64a46e;if(resume_pc==0x8c64a47eu)goto L8c64a47e;goto L8c64a3fa;
L8c64a3fa: *stop=0x8c64a3fau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a3fc; /* mov.l r14,@-r15 */
L8c64a3fc: *stop=0x8c64a3fcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64a3fe; /* mov.l r13,@-r15 */
L8c64a3fe: *stop=0x8c64a3feu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64a400; /* sts.l PR,@-r15 */
L8c64a400: *stop=0x8c64a400u;r[15]+=4294967252u;goto L8c64a402; /* add #-0x2c,r15 */
L8c64a402: *stop=0x8c64a402u;r[3]=sx(m,bus,&io_status,0x8c64a446u,2);if(io_status)return io_status;goto L8c64a404; /* mov.w 0x8c64a446,r3 */
L8c64a404: *stop=0x8c64a404u;r[14]=r[4];goto L8c64a406; /* mov r4,r14 */
L8c64a406: *stop=0x8c64a406u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a450u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64a408; /* mov.l 0x8c64a450,r0 */
L8c64a408: *stop=0x8c64a408u;*macl=r[3]*r[14];goto L8c64a40a; /* mul.l r3,r14 */
L8c64a40a: *stop=0x8c64a40au;r[3]=*macl;goto L8c64a40c; /* sts MACL,r3 */
L8c64a40c: *stop=0x8c64a40cu;if(!valid_access(r[0]+r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c64a40e; /* mov.b @(r0,r3),r2 */
L8c64a40e: *stop=0x8c64a40eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64a410; /* tst r2,r2 */
L8c64a410: take=(*sr&1u);*stop=0x8c64a412u;r[13]=r[5];if(take)goto L8c64a440;goto L8c64a414; /* bt/s 0x8c64a440 */
L8c64a414: *stop=0x8c64a414u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a454u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a416; /* mov.l 0x8c64a454,r3 */
L8c64a416: target=r[3];*pr=0x8c64a41au;*stop=0x8c64a418u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a41a; /* jsr @r3 */
L8c64a41a: *stop=0x8c64a41au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a41c; /* tst r0,r0 */
L8c64a41c: if(!(*sr&1u))goto L8c64a428;goto L8c64a41e; /* bf 0x8c64a428 */
L8c64a41e: *stop=0x8c64a41eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a458u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a420; /* mov.l 0x8c64a458,r3 */
L8c64a420: target=r[3];*pr=0x8c64a424u;*stop=0x8c64a422u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a424; /* jsr @r3 */
L8c64a424: *stop=0x8c64a426u;goto L8c64a440; /* bra 0x8c64a440 */
L8c64a428: *stop=0x8c64a428u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a45cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a42a; /* mov.l 0x8c64a45c,r2 */
L8c64a42a: target=r[2];*pr=0x8c64a42eu;*stop=0x8c64a42cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a42e; /* jsr @r2 */
L8c64a42e: *stop=0x8c64a42eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a430; /* tst r0,r0 */
L8c64a430: if((*sr&1u))goto L8c64a440;goto L8c64a432; /* bt 0x8c64a440 */
L8c64a432: *stop=0x8c64a432u;r[3]=sx(m,bus,&io_status,0x8c64a446u,2);if(io_status)return io_status;goto L8c64a434; /* mov.w 0x8c64a446,r3 */
L8c64a434: *stop=0x8c64a434u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a460u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64a436; /* mov.l 0x8c64a460,r0 */
L8c64a436: *stop=0x8c64a436u;*macl=r[3]*r[14];goto L8c64a438; /* mul.l r3,r14 */
L8c64a438: *stop=0x8c64a438u;r[3]=*macl;goto L8c64a43a; /* sts MACL,r3 */
L8c64a43a: *stop=0x8c64a43au;if(!valid_access(r[0]+r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c64a43c; /* mov.b @(r0,r3),r2 */
L8c64a43c: *stop=0x8c64a43cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64a43e; /* tst r2,r2 */
L8c64a43e: if(!(*sr&1u))goto L8c64a468;goto L8c64a440; /* bf 0x8c64a468 */
L8c64a440: *stop=0x8c64a440u;r[0]=sx(m,bus,&io_status,0x8c64a448u,2);if(io_status)return io_status;goto L8c64a442; /* mov.w 0x8c64a448,r0 */
L8c64a442: *stop=0x8c64a444u;goto L8c64a4a8; /* bra 0x8c64a4a8 */
L8c64a468: *stop=0x8c64a468u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a544u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a46a; /* mov.l 0x8c64a544,r3 */
L8c64a46a: target=r[3];*pr=0x8c64a46eu;*stop=0x8c64a46cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a46e; /* jsr @r3 */
L8c64a46e: *stop=0x8c64a46eu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64a470; /* cmp/pz r0 */
L8c64a470: if((*sr&1u))goto L8c64a478;goto L8c64a472; /* bt 0x8c64a478 */
L8c64a472: *stop=0x8c64a472u;r[0]=sx(m,bus,&io_status,0x8c64a53au,2);if(io_status)return io_status;goto L8c64a474; /* mov.w 0x8c64a53a,r0 */
L8c64a474: *stop=0x8c64a476u;goto L8c64a4a8; /* bra 0x8c64a4a8 */
L8c64a478: *stop=0x8c64a478u;r[5]=r[15];goto L8c64a47a; /* mov r15,r5 */
L8c64a47a: target=0x8c64adc6u;*pr=0x8c64a47eu;*stop=0x8c64a47cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a47e; /* bsr 0x8c64adc6 */
L8c64a47e: *stop=0x8c64a47eu;r[4]=r[0];goto L8c64a480; /* mov r0,r4 */
L8c64a480: *stop=0x8c64a480u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64a482; /* cmp/pz r4 */
L8c64a482: if((*sr&1u))goto L8c64a48a;goto L8c64a484; /* bt 0x8c64a48a */
L8c64a484: *stop=0x8c64a484u;r[0]=sx(m,bus,&io_status,0x8c64a53cu,2);if(io_status)return io_status;goto L8c64a486; /* mov.w 0x8c64a53c,r0 */
L8c64a486: *stop=0x8c64a488u;goto L8c64a4a8; /* bra 0x8c64a4a8 */
L8c64a48a: *stop=0x8c64a48au;r[6]=0u;goto L8c64a48c; /* mov #0x0,r6 */
L8c64a48c: *stop=0x8c64a48cu;r[4]=r[6];goto L8c64a48e; /* mov r6,r4 */
L8c64a48e: *stop=0x8c64a490u;r[5]=12u;goto L8c64a49e; /* bra 0x8c64a49e */
L8c64a492: *stop=0x8c64a492u;r[3]=r[15];goto L8c64a494; /* mov r15,r3 */
L8c64a494: *stop=0x8c64a494u;r[3]+=16u;goto L8c64a496; /* add #0x10,r3 */
L8c64a496: *stop=0x8c64a496u;r[0]=r[4];goto L8c64a498; /* mov r4,r0 */
L8c64a498: *stop=0x8c64a498u;if(!valid_access(r[0]+r[3],1u))return -3;r[3]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c64a49a; /* mov.b @(r0,r3),r3 */
L8c64a49a: *stop=0x8c64a49au;r[4]+=1u;goto L8c64a49c; /* add #0x1,r4 */
L8c64a49c: *stop=0x8c64a49cu;if(!valid_access(r[0]+r[13],1u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[3],1);if(io_status)return io_status;goto L8c64a49e; /* mov.b r3,@(r0,r13) */
L8c64a49e: *stop=0x8c64a49eu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]);goto L8c64a4a0; /* cmp/ge r5,r4 */
L8c64a4a0: if(!(*sr&1u))goto L8c64a492;goto L8c64a4a2; /* bf 0x8c64a492 */
L8c64a4a2: *stop=0x8c64a4a2u;r[0]=r[6];goto L8c64a4a4; /* mov r6,r0 */
L8c64a4a4: *stop=0x8c64a4a4u;if(!valid_access(r[13]+12u,1u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[0],1);if(io_status)return io_status;goto L8c64a4a6; /* mov.b r0,@(0xc,r13) */
L8c64a4a6: *stop=0x8c64a4a6u;r[0]=0u;goto L8c64a4a8; /* mov #0x0,r0 */
L8c64a4a8: *stop=0x8c64a4a8u;r[15]+=44u;goto L8c64a4aa; /* add #0x2c,r15 */
L8c64a4aa: *stop=0x8c64a4aau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a4ac; /* lds.l @r15+,PR */
L8c64a4ac: *stop=0x8c64a4acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64a4ae; /* mov.l @r15+,r13 */
L8c64a4ae: *stop=0x8c64a4aeu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a4b0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_8c64a3fa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a3fa(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a41a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a3fa(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a41au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a424(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a3fa(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a424u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a42e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a3fa(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a42eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a46e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a3fa(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a46eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a47e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a3fa(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a47eu);}

static int sa_movie_filesystem_command9_run_8c64a7ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64a7d8u)goto L8c64a7d8;goto L8c64a7ba;
L8c64a7ba: *stop=0x8c64a7bau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a7bc; /* mov.l r14,@-r15 */
L8c64a7bc: *stop=0x8c64a7bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64a7be; /* mov.l r13,@-r15 */
L8c64a7be: *stop=0x8c64a7beu;r[14]=sx(m,bus,&io_status,0x8c64a862u,2);if(io_status)return io_status;goto L8c64a7c0; /* mov.w 0x8c64a862,r14 */
L8c64a7c0: *stop=0x8c64a7c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64a7c2; /* mov.l r12,@-r15 */
L8c64a7c2: *stop=0x8c64a7c2u;r[12]=r[4];goto L8c64a7c4; /* mov r4,r12 */
L8c64a7c4: *stop=0x8c64a7c4u;*macl=r[14]*r[12];goto L8c64a7c6; /* mul.l r14,r12 */
L8c64a7c6: *stop=0x8c64a7c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a864u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a7c8; /* mov.l 0x8c64a864,r3 */
L8c64a7c8: *stop=0x8c64a7c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64a7ca; /* sts.l PR,@-r15 */
L8c64a7ca: *stop=0x8c64a7cau;r[14]=*macl;goto L8c64a7cc; /* sts MACL,r14 */
L8c64a7cc: *stop=0x8c64a7ccu;r[14]+=r[3];goto L8c64a7ce; /* add r3,r14 */
L8c64a7ce: *stop=0x8c64a7d0u;r[13]=0u;goto L8c64a812; /* bra 0x8c64a812 */
L8c64a7d2: *stop=0x8c64a7d2u;r[5]=r[13];goto L8c64a7d4; /* mov r13,r5 */
L8c64a7d4: target=0x8c64b1c0u;*pr=0x8c64a7d8u;*stop=0x8c64a7d6u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a7d8; /* bsr 0x8c64b1c0 */
L8c64a7d8: *stop=0x8c64a7d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a868u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64a7da; /* mov.l 0x8c64a868,r1 */
L8c64a7da: *stop=0x8c64a7dau;r[4]=r[0]&65535u;goto L8c64a7dc; /* extu.w r0,r4 */
L8c64a7dc: *stop=0x8c64a7dcu;r[0]=r[4];goto L8c64a7de; /* mov r4,r0 */
L8c64a7de: *stop=0x8c64a7deu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64a7e0; /* cmp/eq r1,r0 */
L8c64a7e0: if((*sr&1u))goto L8c64a810;goto L8c64a7e2; /* bt 0x8c64a810 */
L8c64a7e2: *stop=0x8c64a7e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a86cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64a7e4; /* mov.l 0x8c64a86c,r1 */
L8c64a7e4: *stop=0x8c64a7e4u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64a7e6; /* cmp/eq r1,r0 */
L8c64a7e6: if((*sr&1u))goto L8c64a810;goto L8c64a7e8; /* bt 0x8c64a810 */
L8c64a7e8: *stop=0x8c64a7e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a870u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64a7ea; /* mov.l 0x8c64a870,r1 */
L8c64a7ea: *stop=0x8c64a7eau;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64a7ec; /* cmp/eq r1,r0 */
L8c64a7ec: if((*sr&1u))goto L8c64a810;goto L8c64a7ee; /* bt 0x8c64a810 */
L8c64a7ee: *stop=0x8c64a7eeu;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a7f0; /* mov.l @(0x1c,r14),r3 */
L8c64a7f0: *stop=0x8c64a7f0u;if(!valid_access(r[3]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a7f2; /* mov.l @(0x30,r3),r2 */
L8c64a7f2: *stop=0x8c64a7f2u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[2]);goto L8c64a7f4; /* cmp/ge r2,r13 */
L8c64a7f4: if((*sr&1u))goto L8c64a802;goto L8c64a7f6; /* bt 0x8c64a802 */
L8c64a7f6: *stop=0x8c64a7f6u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64a7f8; /* mov.l @(0x1c,r14),r1 */
L8c64a7f8: *stop=0x8c64a7f8u;if(!valid_access(r[1]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a7fa; /* mov.l @(0x30,r1),r3 */
L8c64a7fa: *stop=0x8c64a7fau;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c64a7fc; /* cmp/ge r3,r4 */
L8c64a7fc: if((*sr&1u))goto L8c64a80c;goto L8c64a7fe; /* bt 0x8c64a80c */
L8c64a7fe: *stop=0x8c64a800u;goto L8c64a810; /* bra 0x8c64a810 */
L8c64a802: *stop=0x8c64a802u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a804; /* mov.l @(0x1c,r14),r2 */
L8c64a804: *stop=0x8c64a804u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a806; /* mov.l @r2,r3 */
L8c64a806: *stop=0x8c64a806u;r[3]+=1u;goto L8c64a808; /* add #0x1,r3 */
L8c64a808: *stop=0x8c64a808u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c64a80a; /* cmp/ge r3,r4 */
L8c64a80a: if(!(*sr&1u))goto L8c64a810;goto L8c64a80c; /* bf 0x8c64a810 */
L8c64a80c: *stop=0x8c64a80eu;r[0]=4294967295u;goto L8c64a81e; /* bra 0x8c64a81e */
L8c64a810: *stop=0x8c64a810u;r[13]+=1u;goto L8c64a812; /* add #0x1,r13 */
L8c64a812: *stop=0x8c64a812u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a814; /* mov.l @(0x1c,r14),r2 */
L8c64a814: *stop=0x8c64a814u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a816; /* mov.l @r2,r3 */
L8c64a816: *stop=0x8c64a816u;r[3]+=1u;goto L8c64a818; /* add #0x1,r3 */
L8c64a818: *stop=0x8c64a818u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[3]);goto L8c64a81a; /* cmp/ge r3,r13 */
L8c64a81a: if(!(*sr&1u))goto L8c64a7d2;goto L8c64a81c; /* bf 0x8c64a7d2 */
L8c64a81c: *stop=0x8c64a81cu;r[0]=0u;goto L8c64a81e; /* mov #0x0,r0 */
L8c64a81e: *stop=0x8c64a81eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a820; /* lds.l @r15+,PR */
L8c64a820: *stop=0x8c64a820u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64a822; /* mov.l @r15+,r12 */
L8c64a822: *stop=0x8c64a822u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64a824; /* mov.l @r15+,r13 */
L8c64a824: *stop=0x8c64a824u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a826u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_8c64a7ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a7ba(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a7d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a7ba(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a7d8u);}

static int sa_movie_filesystem_command9_run_8c64a6ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64a6d2u)goto L8c64a6d2;goto L8c64a6ae;
L8c64a6ae: *stop=0x8c64a6aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a6b0; /* mov.l r14,@-r15 */
L8c64a6b0: *stop=0x8c64a6b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64a6b2; /* mov.l r13,@-r15 */
L8c64a6b2: *stop=0x8c64a6b2u;r[13]=sx(m,bus,&io_status,0x8c64a76eu,2);if(io_status)return io_status;goto L8c64a6b4; /* mov.w 0x8c64a76e,r13 */
L8c64a6b4: *stop=0x8c64a6b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64a6b6; /* mov.l r12,@-r15 */
L8c64a6b6: *stop=0x8c64a6b6u;*macl=r[13]*r[4];goto L8c64a6b8; /* mul.l r13,r4 */
L8c64a6b8: *stop=0x8c64a6b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a770u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a6ba; /* mov.l 0x8c64a770,r3 */
L8c64a6ba: *stop=0x8c64a6bau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64a6bc; /* sts.l PR,@-r15 */
L8c64a6bc: *stop=0x8c64a6bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a774u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64a6be; /* mov.l 0x8c64a774,r12 */
L8c64a6be: *stop=0x8c64a6beu;r[13]=*macl;goto L8c64a6c0; /* sts MACL,r13 */
L8c64a6c0: *stop=0x8c64a6c0u;r[13]+=r[3];goto L8c64a6c2; /* add r3,r13 */
L8c64a6c2: *stop=0x8c64a6c4u;r[14]=0u;goto L8c64a6e8; /* bra 0x8c64a6e8 */
L8c64a6c6: *stop=0x8c64a6c6u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a6c8; /* mov.l @r12,r3 */
L8c64a6c8: *stop=0x8c64a6c8u;r[5]=r[14];goto L8c64a6ca; /* mov r14,r5 */
L8c64a6ca: *stop=0x8c64a6cau;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c64a6cc; /* shll r5 */
L8c64a6cc: *stop=0x8c64a6ccu;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a6ce; /* mov.l @(0xc,r3),r2 */
L8c64a6ce: target=r[2];*pr=0x8c64a6d2u;*stop=0x8c64a6d0u;if(!valid_access(r[13]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x24u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a6d2; /* jsr @r2 */
L8c64a6d2: *stop=0x8c64a6d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a778u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64a6d4; /* mov.l 0x8c64a778,r1 */
L8c64a6d4: *stop=0x8c64a6d4u;r[4]=r[0]&65535u;goto L8c64a6d6; /* extu.w r0,r4 */
L8c64a6d6: *stop=0x8c64a6d6u;r[0]=r[4];goto L8c64a6d8; /* mov r4,r0 */
L8c64a6d8: *stop=0x8c64a6d8u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64a6da; /* cmp/eq r1,r0 */
L8c64a6da: if((*sr&1u))goto L8c64a6e6;goto L8c64a6dc; /* bt 0x8c64a6e6 */
L8c64a6dc: *stop=0x8c64a6dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a77cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64a6de; /* mov.l 0x8c64a77c,r1 */
L8c64a6de: *stop=0x8c64a6deu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64a6e0; /* cmp/eq r1,r0 */
L8c64a6e0: if((*sr&1u))goto L8c64a6e6;goto L8c64a6e2; /* bt 0x8c64a6e6 */
L8c64a6e2: *stop=0x8c64a6e4u;r[0]=r[14];goto L8c64a6f2; /* bra 0x8c64a6f2 */
L8c64a6e6: *stop=0x8c64a6e6u;r[14]+=1u;goto L8c64a6e8; /* add #0x1,r14 */
L8c64a6e8: *stop=0x8c64a6e8u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a6ea; /* mov.l @(0x1c,r13),r2 */
L8c64a6ea: *stop=0x8c64a6eau;if(!valid_access(r[2]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a6ec; /* mov.l @(0x30,r2),r3 */
L8c64a6ec: *stop=0x8c64a6ecu;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[3]);goto L8c64a6ee; /* cmp/ge r3,r14 */
L8c64a6ee: if(!(*sr&1u))goto L8c64a6c6;goto L8c64a6f0; /* bf 0x8c64a6c6 */
L8c64a6f0: *stop=0x8c64a6f0u;r[0]=4294967295u;goto L8c64a6f2; /* mov #-0x1,r0 */
L8c64a6f2: *stop=0x8c64a6f2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a6f4; /* lds.l @r15+,PR */
L8c64a6f4: *stop=0x8c64a6f4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64a6f6; /* mov.l @r15+,r12 */
L8c64a6f6: *stop=0x8c64a6f6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64a6f8; /* mov.l @r15+,r13 */
L8c64a6f8: *stop=0x8c64a6f8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a6fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_8c64a6ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a6ae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a6d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a6ae(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a6d2u);}

static int sa_movie_filesystem_command9_run_8c64a6fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64a72eu)goto L8c64a72e;goto L8c64a6fc;
L8c64a6fc: *stop=0x8c64a6fcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a6fe; /* mov.l r14,@-r15 */
L8c64a6fe: *stop=0x8c64a6feu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64a700; /* mov.l r13,@-r15 */
L8c64a700: *stop=0x8c64a700u;r[13]=sx(m,bus,&io_status,0x8c64a76eu,2);if(io_status)return io_status;goto L8c64a702; /* mov.w 0x8c64a76e,r13 */
L8c64a702: *stop=0x8c64a702u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a770u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a704; /* mov.l 0x8c64a770,r3 */
L8c64a704: *stop=0x8c64a704u;*macl=r[13]*r[4];goto L8c64a706; /* mul.l r13,r4 */
L8c64a706: *stop=0x8c64a706u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64a708; /* mov.l r12,@-r15 */
L8c64a708: *stop=0x8c64a708u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64a70a; /* mov.l r11,@-r15 */
L8c64a70a: *stop=0x8c64a70au;r[11]=r[5];goto L8c64a70c; /* mov r5,r11 */
L8c64a70c: *stop=0x8c64a70cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64a70e; /* mov.l r10,@-r15 */
L8c64a70e: *stop=0x8c64a70eu;r[13]=*macl;goto L8c64a710; /* sts MACL,r13 */
L8c64a710: *stop=0x8c64a710u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64a712; /* sts.l PR,@-r15 */
L8c64a712: *stop=0x8c64a712u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a774u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64a714; /* mov.l 0x8c64a774,r10 */
L8c64a714: *stop=0x8c64a714u;r[13]+=r[3];goto L8c64a716; /* add r3,r13 */
L8c64a716: *stop=0x8c64a716u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a778u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64a718; /* mov.l 0x8c64a778,r12 */
L8c64a718: *stop=0x8c64a718u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64a71a; /* mov.l @(0x1c,r13),r14 */
L8c64a71a: *stop=0x8c64a71au;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64a71c; /* mov.l @(0x30,r14),r14 */
L8c64a71c: *stop=0x8c64a71cu;r[14]+=4294967295u;goto L8c64a71e; /* add #-0x1,r14 */
L8c64a71e: *stop=0x8c64a71eu;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[11]);goto L8c64a720; /* cmp/ge r11,r14 */
L8c64a720: if(!(*sr&1u))goto L8c64a73e;goto L8c64a722; /* bf 0x8c64a73e */
L8c64a722: *stop=0x8c64a722u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a724; /* mov.l @r10,r3 */
L8c64a724: *stop=0x8c64a724u;r[5]=r[14];goto L8c64a726; /* mov r14,r5 */
L8c64a726: *stop=0x8c64a726u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c64a728; /* shll r5 */
L8c64a728: *stop=0x8c64a728u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a72a; /* mov.l @(0xc,r3),r2 */
L8c64a72a: target=r[2];*pr=0x8c64a72eu;*stop=0x8c64a72cu;if(!valid_access(r[13]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x24u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a72e; /* jsr @r2 */
L8c64a72e: *stop=0x8c64a72eu;r[0]=r[0]&65535u;goto L8c64a730; /* extu.w r0,r0 */
L8c64a730: *stop=0x8c64a730u;*sr=(*sr&~1u)|(r[12]==r[0]);goto L8c64a732; /* cmp/eq r12,r0 */
L8c64a732: if(!(*sr&1u))goto L8c64a738;goto L8c64a734; /* bf 0x8c64a738 */
L8c64a734: *stop=0x8c64a736u;r[0]=r[14];goto L8c64a740; /* bra 0x8c64a740 */
L8c64a738: *stop=0x8c64a738u;r[14]+=4294967295u;goto L8c64a73a; /* add #-0x1,r14 */
L8c64a73a: *stop=0x8c64a73au;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[11]);goto L8c64a73c; /* cmp/ge r11,r14 */
L8c64a73c: if((*sr&1u))goto L8c64a722;goto L8c64a73e; /* bt 0x8c64a722 */
L8c64a73e: *stop=0x8c64a73eu;r[0]=4294967295u;goto L8c64a740; /* mov #-0x1,r0 */
L8c64a740: *stop=0x8c64a740u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a742; /* lds.l @r15+,PR */
L8c64a742: *stop=0x8c64a742u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64a744; /* mov.l @r15+,r10 */
L8c64a744: *stop=0x8c64a744u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64a746; /* mov.l @r15+,r11 */
L8c64a746: *stop=0x8c64a746u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64a748; /* mov.l @r15+,r12 */
L8c64a748: *stop=0x8c64a748u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64a74a; /* mov.l @r15+,r13 */
L8c64a74a: *stop=0x8c64a74au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a74cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_8c64a6fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a6fc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a72e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a6fc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a72eu);}

static int sa_movie_filesystem_command9_run_8c64b080(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64b0b0u)goto L8c64b0b0;goto L8c64b080;
L8c64b080: *stop=0x8c64b080u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b082; /* mov.l r14,@-r15 */
L8c64b082: *stop=0x8c64b082u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b084; /* mov.l r13,@-r15 */
L8c64b084: *stop=0x8c64b084u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b086; /* mov.l r12,@-r15 */
L8c64b086: *stop=0x8c64b086u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b088; /* sts.l PR,@-r15 */
L8c64b088: *stop=0x8c64b088u;r[15]+=4294967292u;goto L8c64b08a; /* add #-0x4,r15 */
L8c64b08a: *stop=0x8c64b08au;r[13]=sx(m,bus,&io_status,0x8c64b0d2u,2);if(io_status)return io_status;goto L8c64b08c; /* mov.w 0x8c64b0d2,r13 */
L8c64b08c: *stop=0x8c64b08cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b0d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b08e; /* mov.l 0x8c64b0d8,r3 */
L8c64b08e: *stop=0x8c64b08eu;*macl=r[13]*r[4];goto L8c64b090; /* mul.l r13,r4 */
L8c64b090: *stop=0x8c64b090u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b0dcu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64b092; /* mov.l 0x8c64b0dc,r12 */
L8c64b092: *stop=0x8c64b092u;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[5],2);if(io_status)return io_status;goto L8c64b094; /* mov.w r5,@r15 */
L8c64b094: *stop=0x8c64b094u;r[13]=*macl;goto L8c64b096; /* sts MACL,r13 */
L8c64b096: *stop=0x8c64b096u;r[13]+=r[3];goto L8c64b098; /* add r3,r13 */
L8c64b098: *stop=0x8c64b098u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64b09a; /* mov.l @(0x1c,r13),r4 */
L8c64b09a: *stop=0x8c64b09au;if(!valid_access(r[4]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x30u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64b09c; /* mov.l @(0x30,r4),r4 */
L8c64b09c: *stop=0x8c64b09cu;r[14]=r[4];goto L8c64b09e; /* mov r4,r14 */
L8c64b09e: *stop=0x8c64b09eu;r[14]+=4294967295u;goto L8c64b0a0; /* add #-0x1,r14 */
L8c64b0a0: *stop=0x8c64b0a0u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b0a2; /* cmp/pz r14 */
L8c64b0a2: if(!(*sr&1u))goto L8c64b0c4;goto L8c64b0a4; /* bf 0x8c64b0c4 */
L8c64b0a4: *stop=0x8c64b0a4u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b0a6; /* mov.l @r12,r3 */
L8c64b0a6: *stop=0x8c64b0a6u;r[5]=r[14];goto L8c64b0a8; /* mov r14,r5 */
L8c64b0a8: *stop=0x8c64b0a8u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c64b0aa; /* shll r5 */
L8c64b0aa: *stop=0x8c64b0aau;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b0ac; /* mov.l @(0xc,r3),r2 */
L8c64b0ac: target=r[2];*pr=0x8c64b0b0u;*stop=0x8c64b0aeu;if(!valid_access(r[13]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x24u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b0b0; /* jsr @r2 */
L8c64b0b0: *stop=0x8c64b0b0u;if(!valid_access(r[15],2u))return -3;r[3]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c64b0b2; /* mov.w @r15,r3 */
L8c64b0b2: *stop=0x8c64b0b2u;r[0]=r[0]&65535u;goto L8c64b0b4; /* extu.w r0,r0 */
L8c64b0b4: *stop=0x8c64b0b4u;r[3]=r[3]&65535u;goto L8c64b0b6; /* extu.w r3,r3 */
L8c64b0b6: *stop=0x8c64b0b6u;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c64b0b8; /* cmp/eq r3,r0 */
L8c64b0b8: if(!(*sr&1u))goto L8c64b0be;goto L8c64b0ba; /* bf 0x8c64b0be */
L8c64b0ba: *stop=0x8c64b0bcu;r[0]=r[14];goto L8c64b0c6; /* bra 0x8c64b0c6 */
L8c64b0be: *stop=0x8c64b0beu;r[14]+=4294967295u;goto L8c64b0c0; /* add #-0x1,r14 */
L8c64b0c0: *stop=0x8c64b0c0u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b0c2; /* cmp/pz r14 */
L8c64b0c2: if((*sr&1u))goto L8c64b0a4;goto L8c64b0c4; /* bt 0x8c64b0a4 */
L8c64b0c4: *stop=0x8c64b0c4u;r[0]=4294967295u;goto L8c64b0c6; /* mov #-0x1,r0 */
L8c64b0c6: *stop=0x8c64b0c6u;r[15]+=4u;goto L8c64b0c8; /* add #0x4,r15 */
L8c64b0c8: *stop=0x8c64b0c8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b0ca; /* lds.l @r15+,PR */
L8c64b0ca: *stop=0x8c64b0cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64b0cc; /* mov.l @r15+,r12 */
L8c64b0cc: *stop=0x8c64b0ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b0ce; /* mov.l @r15+,r13 */
L8c64b0ce: *stop=0x8c64b0ceu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b0d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_8c64b080(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64b080(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64b0b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64b080(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b0b0u);}

static int sa_movie_filesystem_command9_run_8c64a74e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64a792u)goto L8c64a792;goto L8c64a74e;
L8c64a74e: *stop=0x8c64a74eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a750; /* mov.l r14,@-r15 */
L8c64a750: *stop=0x8c64a750u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64a752; /* mov.l r13,@-r15 */
L8c64a752: *stop=0x8c64a752u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64a754; /* mov.l r12,@-r15 */
L8c64a754: *stop=0x8c64a754u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64a756; /* mov.l r11,@-r15 */
L8c64a756: *stop=0x8c64a756u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64a758; /* sts.l PR,@-r15 */
L8c64a758: *stop=0x8c64a758u;r[15]+=4294967292u;goto L8c64a75a; /* add #-0x4,r15 */
L8c64a75a: *stop=0x8c64a75au;r[12]=sx(m,bus,&io_status,0x8c64a76eu,2);if(io_status)return io_status;goto L8c64a75c; /* mov.w 0x8c64a76e,r12 */
L8c64a75c: *stop=0x8c64a75cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a770u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a75e; /* mov.l 0x8c64a770,r3 */
L8c64a75e: *stop=0x8c64a75eu;*macl=r[12]*r[4];goto L8c64a760; /* mul.l r12,r4 */
L8c64a760: *stop=0x8c64a760u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a774u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64a762; /* mov.l 0x8c64a774,r11 */
L8c64a762: *stop=0x8c64a762u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64a764; /* mov.l r5,@r15 */
L8c64a764: *stop=0x8c64a764u;r[12]=*macl;goto L8c64a766; /* sts MACL,r12 */
L8c64a766: *stop=0x8c64a766u;r[12]+=r[3];goto L8c64a768; /* add r3,r12 */
L8c64a768: *stop=0x8c64a768u;if(!valid_access(r[12]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x28u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64a76a; /* mov.l @(0x28,r12),r14 */
L8c64a76a: *stop=0x8c64a76cu;r[13]=0u;goto L8c64a7a2; /* bra 0x8c64a7a2 */
L8c64a780: *stop=0x8c64a780u;if(!valid_access(r[14],1u))return -3;r[4]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c64a782; /* mov.b @r14,r4 */
L8c64a782: *stop=0x8c64a782u;r[4]=r[4]&255u;goto L8c64a784; /* extu.b r4,r4 */
L8c64a784: *stop=0x8c64a784u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64a786; /* tst r4,r4 */
L8c64a786: if((*sr&1u))goto L8c64a79e;goto L8c64a788; /* bt 0x8c64a79e */
L8c64a788: *stop=0x8c64a788u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a78a; /* mov.l @r11,r2 */
L8c64a78a: *stop=0x8c64a78au;r[5]=2u;goto L8c64a78c; /* mov #0x2,r5 */
L8c64a78c: *stop=0x8c64a78cu;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a78e; /* mov.l @(0xc,r2),r3 */
L8c64a78e: target=r[3];*pr=0x8c64a792u;*stop=0x8c64a790u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a792; /* jsr @r3 */
L8c64a792: *stop=0x8c64a792u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a794; /* mov.l @r15,r2 */
L8c64a794: *stop=0x8c64a794u;r[0]=r[0]&65535u;goto L8c64a796; /* extu.w r0,r0 */
L8c64a796: *stop=0x8c64a796u;*sr=(*sr&~1u)|(r[2]==r[0]);goto L8c64a798; /* cmp/eq r2,r0 */
L8c64a798: if(!(*sr&1u))goto L8c64a79e;goto L8c64a79a; /* bf 0x8c64a79e */
L8c64a79a: *stop=0x8c64a79cu;r[0]=r[14];goto L8c64a7ac; /* bra 0x8c64a7ac */
L8c64a79e: *stop=0x8c64a79eu;r[14]+=32u;goto L8c64a7a0; /* add #0x20,r14 */
L8c64a7a0: *stop=0x8c64a7a0u;r[13]+=1u;goto L8c64a7a2; /* add #0x1,r13 */
L8c64a7a2: *stop=0x8c64a7a2u;if(!valid_access(r[12]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a7a4; /* mov.l @(0x1c,r12),r2 */
L8c64a7a4: *stop=0x8c64a7a4u;if(!valid_access(r[2]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a7a6; /* mov.l @(0x30,r2),r3 */
L8c64a7a6: *stop=0x8c64a7a6u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[3]);goto L8c64a7a8; /* cmp/ge r3,r13 */
L8c64a7a8: if(!(*sr&1u))goto L8c64a780;goto L8c64a7aa; /* bf 0x8c64a780 */
L8c64a7aa: *stop=0x8c64a7aau;r[0]=0u;goto L8c64a7ac; /* mov #0x0,r0 */
L8c64a7ac: *stop=0x8c64a7acu;r[15]+=4u;goto L8c64a7ae; /* add #0x4,r15 */
L8c64a7ae: *stop=0x8c64a7aeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a7b0; /* lds.l @r15+,PR */
L8c64a7b0: *stop=0x8c64a7b0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64a7b2; /* mov.l @r15+,r11 */
L8c64a7b2: *stop=0x8c64a7b2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64a7b4; /* mov.l @r15+,r12 */
L8c64a7b4: *stop=0x8c64a7b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64a7b6; /* mov.l @r15+,r13 */
L8c64a7b6: *stop=0x8c64a7b6u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a7b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_8c64a74e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a74e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64a792(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64a74e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a792u);}

static int sa_movie_filesystem_command9_run_8c64adc6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64adeau)goto L8c64adea;goto L8c64adc6;
L8c64adc6: *stop=0x8c64adc6u;r[6]=sx(m,bus,&io_status,0x8c64aebau,2);if(io_status)return io_status;goto L8c64adc8; /* mov.w 0x8c64aeba,r6 */
L8c64adc8: *stop=0x8c64adc8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aec0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64adca; /* mov.l 0x8c64aec0,r3 */
L8c64adca: *stop=0x8c64adcau;*macl=r[6]*r[4];goto L8c64adcc; /* mul.l r6,r4 */
L8c64adcc: *stop=0x8c64adccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64adce; /* mov.l r14,@-r15 */
L8c64adce: *stop=0x8c64adceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64add0; /* mov.l r13,@-r15 */
L8c64add0: *stop=0x8c64add0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64add2; /* sts.l PR,@-r15 */
L8c64add2: *stop=0x8c64add2u;r[6]=*macl;goto L8c64add4; /* sts MACL,r6 */
L8c64add4: *stop=0x8c64add4u;r[4]=sx(m,bus,&io_status,0x8c64aebcu,2);if(io_status)return io_status;goto L8c64add6; /* mov.w 0x8c64aebc,r4 */
L8c64add6: *stop=0x8c64add6u;r[6]+=r[3];goto L8c64add8; /* add r3,r6 */
L8c64add8: *stop=0x8c64add8u;if(!valid_access(r[6]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x28u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64adda; /* mov.l @(0x28,r6),r13 */
L8c64adda: *stop=0x8c64addcu;r[14]=0u;goto L8c64adf2; /* bra 0x8c64adf2 */
L8c64adde: *stop=0x8c64addeu;if(!valid_access(r[13],1u))return -3;r[7]=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;goto L8c64ade0; /* mov.b @r13,r7 */
L8c64ade0: *stop=0x8c64ade0u;r[7]=r[7]&255u;goto L8c64ade2; /* extu.b r7,r7 */
L8c64ade2: *stop=0x8c64ade2u;*sr=(*sr&~1u)|(r[4]==r[7]);goto L8c64ade4; /* cmp/eq r4,r7 */
L8c64ade4: if(!(*sr&1u))goto L8c64adee;goto L8c64ade6; /* bf 0x8c64adee */
L8c64ade6: target=0x8c64acdau;*pr=0x8c64adeau;*stop=0x8c64ade8u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64adea; /* bsr 0x8c64acda */
L8c64adea: *stop=0x8c64adecu;r[0]=r[14];goto L8c64adfc; /* bra 0x8c64adfc */
L8c64adee: *stop=0x8c64adeeu;r[13]+=32u;goto L8c64adf0; /* add #0x20,r13 */
L8c64adf0: *stop=0x8c64adf0u;r[14]+=1u;goto L8c64adf2; /* add #0x1,r14 */
L8c64adf2: *stop=0x8c64adf2u;if(!valid_access(r[6]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64adf4; /* mov.l @(0x1c,r6),r3 */
L8c64adf4: *stop=0x8c64adf4u;if(!valid_access(r[3]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64adf6; /* mov.l @(0x30,r3),r2 */
L8c64adf6: *stop=0x8c64adf6u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[2]);goto L8c64adf8; /* cmp/ge r2,r14 */
L8c64adf8: if(!(*sr&1u))goto L8c64adde;goto L8c64adfa; /* bf 0x8c64adde */
L8c64adfa: *stop=0x8c64adfau;r[0]=4294967295u;goto L8c64adfc; /* mov #-0x1,r0 */
L8c64adfc: *stop=0x8c64adfcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64adfe; /* lds.l @r15+,PR */
L8c64adfe: *stop=0x8c64adfeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64ae00; /* mov.l @r15+,r13 */
L8c64ae00: *stop=0x8c64ae00u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64ae02u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_8c64adc6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64adc6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command9_resume_8c64adea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command9_run_8c64adc6(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64adeau);}
