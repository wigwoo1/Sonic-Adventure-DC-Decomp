/* Original linked-directory read and table-link lookup.
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
static int sa_movie_filesystem_directory_run_8c64a9a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64a9e0u)goto L8c64a9e0;if(resume_pc==0x8c64a9f0u)goto L8c64a9f0;goto L8c64a9a8;
L8c64a9a8: *stop=0x8c64a9a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a9aa; /* mov.l r14,@-r15 */
L8c64a9aa: *stop=0x8c64a9aau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64a9ac; /* mov.l r13,@-r15 */
L8c64a9ac: *stop=0x8c64a9acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64a9ae; /* mov.l r12,@-r15 */
L8c64a9ae: *stop=0x8c64a9aeu;r[12]=r[4];goto L8c64a9b0; /* mov r4,r12 */
L8c64a9b0: *stop=0x8c64a9b0u;r[4]=sx(m,bus,&io_status,0x8c64aa80u,2);if(io_status)return io_status;goto L8c64a9b2; /* mov.w 0x8c64aa80,r4 */
L8c64a9b2: *stop=0x8c64a9b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aa84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a9b4; /* mov.l 0x8c64aa84,r3 */
L8c64a9b4: *stop=0x8c64a9b4u;*macl=r[4]*r[12];goto L8c64a9b6; /* mul.l r4,r12 */
L8c64a9b6: *stop=0x8c64a9b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64a9b8; /* mov.l r11,@-r15 */
L8c64a9b8: *stop=0x8c64a9b8u;r[11]=0u;goto L8c64a9ba; /* mov #0x0,r11 */
L8c64a9ba: *stop=0x8c64a9bau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64a9bc; /* mov.l r10,@-r15 */
L8c64a9bc: *stop=0x8c64a9bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64a9be; /* mov.l r9,@-r15 */
L8c64a9be: *stop=0x8c64a9beu;r[4]=*macl;goto L8c64a9c0; /* sts MACL,r4 */
L8c64a9c0: *stop=0x8c64a9c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64a9c2; /* mov.l r8,@-r15 */
L8c64a9c2: *stop=0x8c64a9c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64a9c4; /* sts.l PR,@-r15 */
L8c64a9c4: *stop=0x8c64a9c4u;r[4]+=r[3];goto L8c64a9c6; /* add r3,r4 */
L8c64a9c6: *stop=0x8c64a9c6u;r[8]=sx(m,bus,&io_status,0x8c64aa82u,2);if(io_status)return io_status;goto L8c64a9c8; /* mov.w 0x8c64aa82,r8 */
L8c64a9c8: *stop=0x8c64a9c8u;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64a9ca; /* mov.l @(0x1c,r4),r5 */
L8c64a9ca: *stop=0x8c64a9cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aa88u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c64a9cc; /* mov.l 0x8c64aa88,r9 */
L8c64a9cc: *stop=0x8c64a9ccu;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64a9ce; /* mov.l @(0x18,r5),r10 */
L8c64a9ce: *stop=0x8c64a9ceu;if(!valid_access(r[5]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x14u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64a9d0; /* mov.l @(0x14,r5),r13 */
L8c64a9d0: *stop=0x8c64a9d0u;*sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c64a9d2; /* cmp/pl r10 */
L8c64a9d2: take=!(*sr&1u);*stop=0x8c64a9d4u;if(!valid_access(r[4]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x28u,4);if(io_status)return io_status;r[14]=loaded;}if(take)goto L8c64aa02;goto L8c64a9d6; /* bf/s 0x8c64aa02 */
L8c64a9d6: *stop=0x8c64a9d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aa8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a9d8; /* mov.l 0x8c64aa8c,r3 */
L8c64a9d8: *stop=0x8c64a9d8u;r[5]=r[13]&65535u;goto L8c64a9da; /* extu.w r13,r5 */
L8c64a9da: *stop=0x8c64a9dau;r[6]=r[14];goto L8c64a9dc; /* mov r14,r6 */
L8c64a9dc: target=r[3];*pr=0x8c64a9e0u;*stop=0x8c64a9deu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a9e0; /* jsr @r3 */
L8c64a9e0: *stop=0x8c64a9e0u;r[4]=r[0];goto L8c64a9e2; /* mov r0,r4 */
L8c64a9e2: *stop=0x8c64a9e2u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64a9e4; /* cmp/pz r4 */
L8c64a9e4: if((*sr&1u))goto L8c64a9ea;goto L8c64a9e6; /* bt 0x8c64a9ea */
L8c64a9e6: *stop=0x8c64a9e8u;r[0]=r[4];goto L8c64aa04; /* bra 0x8c64aa04 */
L8c64a9ea: *stop=0x8c64a9eau;r[5]=r[13]&65535u;goto L8c64a9ec; /* extu.w r13,r5 */
L8c64a9ec: target=0x8c64b1c0u;*pr=0x8c64a9f0u;*stop=0x8c64a9eeu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a9f0; /* bsr 0x8c64b1c0 */
L8c64a9f0: *stop=0x8c64a9f0u;r[4]=r[0];goto L8c64a9f2; /* mov r0,r4 */
L8c64a9f2: *stop=0x8c64a9f2u;r[2]=r[4]&65535u;goto L8c64a9f4; /* extu.w r4,r2 */
L8c64a9f4: *stop=0x8c64a9f4u;*sr=(*sr&~1u)|(r[9]==r[2]);goto L8c64a9f6; /* cmp/eq r9,r2 */
L8c64a9f6: if((*sr&1u))goto L8c64aa02;goto L8c64a9f8; /* bt 0x8c64aa02 */
L8c64a9f8: *stop=0x8c64a9f8u;r[11]+=1u;goto L8c64a9fa; /* add #0x1,r11 */
L8c64a9fa: *stop=0x8c64a9fau;r[13]=r[4];goto L8c64a9fc; /* mov r4,r13 */
L8c64a9fc: *stop=0x8c64a9fcu;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[10]);goto L8c64a9fe; /* cmp/ge r10,r11 */
L8c64a9fe: take=!(*sr&1u);*stop=0x8c64aa00u;r[14]+=r[8];if(take)goto L8c64a9d6;goto L8c64aa02; /* bf/s 0x8c64a9d6 */
L8c64aa02: *stop=0x8c64aa02u;r[0]=0u;goto L8c64aa04; /* mov #0x0,r0 */
L8c64aa04: *stop=0x8c64aa04u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64aa06; /* lds.l @r15+,PR */
L8c64aa06: *stop=0x8c64aa06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64aa08; /* mov.l @r15+,r8 */
L8c64aa08: *stop=0x8c64aa08u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64aa0a; /* mov.l @r15+,r9 */
L8c64aa0a: *stop=0x8c64aa0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64aa0c; /* mov.l @r15+,r10 */
L8c64aa0c: *stop=0x8c64aa0cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64aa0e; /* mov.l @r15+,r11 */
L8c64aa0e: *stop=0x8c64aa0eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64aa10; /* mov.l @r15+,r12 */
L8c64aa10: *stop=0x8c64aa10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64aa12; /* mov.l @r15+,r13 */
L8c64aa12: *stop=0x8c64aa12u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64aa14u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_directory_8c64a9a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_directory_run_8c64a9a8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_directory_resume_8c64a9e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_directory_run_8c64a9a8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a9e0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_directory_resume_8c64a9f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_directory_run_8c64a9a8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a9f0u);}

SA_STAGE_EXPORT int sa_movie_filesystem_directory_8c64b1c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64b1c0;
L8c64b1c0: *stop=0x8c64b1c0u;r[15]+=4294967288u;goto L8c64b1c2; /* add #-0x8,r15 */
L8c64b1c2: *stop=0x8c64b1c2u;r[3]=sx(m,bus,&io_status,0x8c64b1e0u,2);if(io_status)return io_status;goto L8c64b1c4; /* mov.w 0x8c64b1e0,r3 */
L8c64b1c4: *stop=0x8c64b1c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b1ecu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b1c6; /* mov.l 0x8c64b1ec,r1 */
L8c64b1c6: *stop=0x8c64b1c6u;*macl=r[3]*r[4];goto L8c64b1c8; /* mul.l r3,r4 */
L8c64b1c8: *stop=0x8c64b1c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b1e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b1ca; /* mov.l 0x8c64b1e8,r2 */
L8c64b1ca: *stop=0x8c64b1cau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64b1cc; /* mov.l r5,@r15 */
L8c64b1cc: *stop=0x8c64b1ccu;r[4]=*macl;goto L8c64b1ce; /* sts MACL,r4 */
L8c64b1ce: *stop=0x8c64b1ceu;r[4]+=r[2];goto L8c64b1d0; /* add r2,r4 */
L8c64b1d0: *stop=0x8c64b1d0u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c64b1d2; /* mov.l r4,@(0x4,r15) */
L8c64b1d2: *stop=0x8c64b1d2u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b1d4; /* mov.l @r1,r0 */
L8c64b1d4: *stop=0x8c64b1d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b1d6; /* mov.l @r15,r5 */
L8c64b1d6: *stop=0x8c64b1d6u;if(!valid_access(r[4]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x24u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64b1d8; /* mov.l @(0x24,r4),r4 */
L8c64b1d8: *stop=0x8c64b1d8u;if(!valid_access(r[0]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b1da; /* mov.l @(0xc,r0),r0 */
L8c64b1da: *stop=0x8c64b1dau;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c64b1dc; /* shll r5 */
L8c64b1dc: target=r[0];*stop=0x8c64b1deu;r[15]+=8u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}
