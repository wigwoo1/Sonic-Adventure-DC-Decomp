/* Original filesystem root/table reads and retrying block reader.
 * Explicit SH4 register state and original call continuations.
 * Missing services stay boundaries; no callback result or task frame is supplied.
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
SA_STAGE_EXPORT int sa_movie_filesystem_state_8c64ae32(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64ae32;
L8c64ae32: *stop=0x8c64ae32u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ae34; /* mov.l r14,@-r15 */
L8c64ae34: *stop=0x8c64ae34u;r[14]=sx(m,bus,&io_status,0x8c64aebau,2);if(io_status)return io_status;goto L8c64ae36; /* mov.w 0x8c64aeba,r14 */
L8c64ae36: *stop=0x8c64ae36u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aec0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ae38; /* mov.l 0x8c64aec0,r3 */
L8c64ae38: *stop=0x8c64ae38u;*macl=r[14]*r[4];goto L8c64ae3a; /* mul.l r14,r4 */
L8c64ae3a: *stop=0x8c64ae3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aec4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ae3c; /* mov.l 0x8c64aec4,r2 */
L8c64ae3c: *stop=0x8c64ae3cu;r[14]=*macl;goto L8c64ae3e; /* sts MACL,r14 */
L8c64ae3e: *stop=0x8c64ae3eu;r[14]+=r[3];goto L8c64ae40; /* add r3,r14 */
L8c64ae40: *stop=0x8c64ae40u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ae42; /* mov.l @(0x1c,r14),r5 */
L8c64ae42: *stop=0x8c64ae42u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64ae44; /* mov.l @(0x20,r14),r6 */
L8c64ae44: *stop=0x8c64ae44u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ae46; /* mov.l @(0x8,r5),r5 */
L8c64ae46: target=r[2];*stop=0x8c64ae48u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

static int sa_movie_filesystem_state_run_8c64ae62(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64ae9au)goto L8c64ae9a;goto L8c64ae62;
L8c64ae62: *stop=0x8c64ae62u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ae64; /* mov.l r14,@-r15 */
L8c64ae64: *stop=0x8c64ae64u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ae66; /* mov.l r13,@-r15 */
L8c64ae66: *stop=0x8c64ae66u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64ae68; /* mov.l r12,@-r15 */
L8c64ae68: *stop=0x8c64ae68u;r[13]=sx(m,bus,&io_status,0x8c64aebau,2);if(io_status)return io_status;goto L8c64ae6a; /* mov.w 0x8c64aeba,r13 */
L8c64ae6a: *stop=0x8c64ae6au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64ae6c; /* mov.l r11,@-r15 */
L8c64ae6c: *stop=0x8c64ae6cu;r[11]=r[4];goto L8c64ae6e; /* mov r4,r11 */
L8c64ae6e: *stop=0x8c64ae6eu;*macl=r[13]*r[11];goto L8c64ae70; /* mul.l r13,r11 */
L8c64ae70: *stop=0x8c64ae70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aec0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ae72; /* mov.l 0x8c64aec0,r3 */
L8c64ae72: *stop=0x8c64ae72u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64ae74; /* mov.l r10,@-r15 */
L8c64ae74: *stop=0x8c64ae74u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ae76; /* sts.l PR,@-r15 */
L8c64ae76: *stop=0x8c64ae76u;r[13]=*macl;goto L8c64ae78; /* sts MACL,r13 */
L8c64ae78: *stop=0x8c64ae78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64aec4u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64ae7a; /* mov.l 0x8c64aec4,r10 */
L8c64ae7a: *stop=0x8c64ae7au;r[13]+=r[3];goto L8c64ae7c; /* add r3,r13 */
L8c64ae7c: *stop=0x8c64ae7cu;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64ae7e; /* mov.l @(0x1c,r13),r12 */
L8c64ae7e: *stop=0x8c64ae7eu;if(!valid_access(r[12]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x10u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64ae80; /* mov.l @(0x10,r12),r12 */
L8c64ae80: *stop=0x8c64ae80u;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c64ae82; /* cmp/pl r12 */
L8c64ae82: take=!(*sr&1u);*stop=0x8c64ae84u;r[14]=0u;if(take)goto L8c64aeaa;goto L8c64ae86; /* bf/s 0x8c64aeaa */
L8c64ae86: *stop=0x8c64ae86u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ae88; /* mov.l @(0x1c,r13),r5 */
L8c64ae88: *stop=0x8c64ae88u;r[6]=r[14];goto L8c64ae8a; /* mov r14,r6 */
L8c64ae8a: *stop=0x8c64ae8au;if(!valid_access(r[13]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ae8c; /* mov.l @(0x24,r13),r3 */
L8c64ae8c: *stop=0x8c64ae8cu;r[6]<<=8;goto L8c64ae8e; /* shll8 r6 */
L8c64ae8e: *stop=0x8c64ae8eu;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ae90; /* mov.l @(0xc,r5),r5 */
L8c64ae90: *stop=0x8c64ae90u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c64ae92; /* shll r6 */
L8c64ae92: *stop=0x8c64ae92u;r[6]+=r[3];goto L8c64ae94; /* add r3,r6 */
L8c64ae94: *stop=0x8c64ae94u;r[5]+=r[14];goto L8c64ae96; /* add r14,r5 */
L8c64ae96: target=r[10];*pr=0x8c64ae9au;*stop=0x8c64ae98u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ae9a; /* jsr @r10 */
L8c64ae9a: *stop=0x8c64ae9au;r[4]=r[0];goto L8c64ae9c; /* mov r0,r4 */
L8c64ae9c: *stop=0x8c64ae9cu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64ae9e; /* cmp/pz r4 */
L8c64ae9e: if((*sr&1u))goto L8c64aea4;goto L8c64aea0; /* bt 0x8c64aea4 */
L8c64aea0: *stop=0x8c64aea2u;r[0]=r[4];goto L8c64aeac; /* bra 0x8c64aeac */
L8c64aea4: *stop=0x8c64aea4u;r[14]+=1u;goto L8c64aea6; /* add #0x1,r14 */
L8c64aea6: *stop=0x8c64aea6u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[12]);goto L8c64aea8; /* cmp/ge r12,r14 */
L8c64aea8: if(!(*sr&1u))goto L8c64ae86;goto L8c64aeaa; /* bf 0x8c64ae86 */
L8c64aeaa: *stop=0x8c64aeaau;r[0]=0u;goto L8c64aeac; /* mov #0x0,r0 */
L8c64aeac: *stop=0x8c64aeacu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64aeae; /* lds.l @r15+,PR */
L8c64aeae: *stop=0x8c64aeaeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64aeb0; /* mov.l @r15+,r10 */
L8c64aeb0: *stop=0x8c64aeb0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64aeb2; /* mov.l @r15+,r11 */
L8c64aeb2: *stop=0x8c64aeb2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64aeb4; /* mov.l @r15+,r12 */
L8c64aeb4: *stop=0x8c64aeb4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64aeb6; /* mov.l @r15+,r13 */
L8c64aeb6: *stop=0x8c64aeb6u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64aeb8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_state_8c64ae62(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_state_run_8c64ae62(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_state_resume_8c64ae9a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_state_run_8c64ae62(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ae9au);}

static int sa_movie_filesystem_state_run_8c647610(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c647646u)goto L8c647646;if(resume_pc==0x8c647694u)goto L8c647694;goto L8c647610;
L8c647610: *stop=0x8c647610u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c647612; /* mov.l r14,@-r15 */
L8c647612: *stop=0x8c647612u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c647614; /* mov.l r13,@-r15 */
L8c647614: *stop=0x8c647614u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c647616; /* mov.l r12,@-r15 */
L8c647616: *stop=0x8c647616u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c647618; /* mov.l r11,@-r15 */
L8c647618: *stop=0x8c647618u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64761a; /* mov.l r10,@-r15 */
L8c64761a: *stop=0x8c64761au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64761c; /* mov.l r9,@-r15 */
L8c64761c: *stop=0x8c64761cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64761e; /* mov.l r8,@-r15 */
L8c64761e: *stop=0x8c64761eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c647620; /* sts.l PR,@-r15 */
L8c647620: *stop=0x8c647620u;r[15]+=4294967284u;goto L8c647622; /* add #-0xc,r15 */
L8c647622: *stop=0x8c647622u;r[12]=sx(m,bus,&io_status,0x8c6476c4u,2);if(io_status)return io_status;goto L8c647624; /* mov.w 0x8c6476c4,r12 */
L8c647624: *stop=0x8c647624u;r[13]=r[4];goto L8c647626; /* mov r4,r13 */
L8c647626: *stop=0x8c647626u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6476ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647628; /* mov.l 0x8c6476cc,r3 */
L8c647628: *stop=0x8c647628u;*macl=r[12]*r[13];goto L8c64762a; /* mul.l r12,r13 */
L8c64762a: *stop=0x8c64762au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6476d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64762c; /* mov.l 0x8c6476d0,r2 */
L8c64762c: *stop=0x8c64762cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6476d4u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c64762e; /* mov.l 0x8c6476d4,r8 */
L8c64762e: *stop=0x8c64762eu;r[11]=sx(m,bus,&io_status,0x8c6476c6u,2);if(io_status)return io_status;goto L8c647630; /* mov.w 0x8c6476c6,r11 */
L8c647630: *stop=0x8c647630u;r[12]=*macl;goto L8c647632; /* sts MACL,r12 */
L8c647632: *stop=0x8c647632u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6476d8u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c647634; /* mov.l 0x8c6476d8,r10 */
L8c647634: *stop=0x8c647634u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c647636; /* mov.l r5,@r15 */
L8c647636: *stop=0x8c647636u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c647638; /* mov.l r6,@(0x4,r15) */
L8c647638: *stop=0x8c647638u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[9]=loaded;}goto L8c64763a; /* mov.l @r3,r9 */
L8c64763a: *stop=0x8c64763au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[12],4);if(io_status)return io_status;goto L8c64763c; /* mov.l r12,@(0x8,r15) */
L8c64763c: *stop=0x8c64763cu;r[12]+=r[2];goto L8c64763e; /* add r2,r12 */
L8c64763e: *stop=0x8c64763eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c647640; /* mov.l @r15,r5 */
L8c647640: *stop=0x8c647640u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647642; /* mov.l @(0x4,r15),r6 */
L8c647642: target=r[10];*pr=0x8c647646u;*stop=0x8c647644u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647646; /* jsr @r10 */
L8c647646: *stop=0x8c647646u;r[14]=r[0];goto L8c647648; /* mov r0,r14 */
L8c647648: *stop=0x8c647648u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c64764a; /* tst r14,r14 */
L8c64764a: if((*sr&1u))goto L8c64766a;goto L8c64764c; /* bt 0x8c64766a */
L8c64764c: *stop=0x8c64764cu;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64764e; /* mov.l @r8,r3 */
L8c64764e: *stop=0x8c64764eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c647650; /* tst r3,r3 */
L8c647650: if((*sr&1u))goto L8c647656;goto L8c647652; /* bt 0x8c647656 */
L8c647652: *stop=0x8c647652u;*sr=(*sr&~1u)|(r[11]==r[14]);goto L8c647654; /* cmp/eq r11,r14 */
L8c647654: if((*sr&1u))goto L8c6476a8;goto L8c647656; /* bt 0x8c6476a8 */
L8c647656: *stop=0x8c647656u;if(!valid_access(r[12]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647658; /* mov.l @(0x34,r12),r3 */
L8c647658: *stop=0x8c647658u;*sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c64765a; /* tst r9,r9 */
L8c64765a: *stop=0x8c64765au;r[3]+=1u;goto L8c64765c; /* add #0x1,r3 */
L8c64765c: *stop=0x8c64765cu;if(!valid_access(r[12]+52u,4u))return -3;wr(m,bus,&io_status,r[12]+0x34u,r[3],4);if(io_status)return io_status;goto L8c64765e; /* mov.l r3,@(0x34,r12) */
L8c64765e: take=!(*sr&1u);*stop=0x8c647660u;r[9]+=4294967295u;if(take)goto L8c64763e;goto L8c647662; /* bf/s 0x8c64763e */
L8c647662: *stop=0x8c647662u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6476ccu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c647664; /* mov.l 0x8c6476cc,r1 */
L8c647664: *stop=0x8c647664u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c647666; /* mov.l @r1,r0 */
L8c647666: *stop=0x8c647666u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c647668; /* cmp/eq #-0x1,r0 */
L8c647668: if((*sr&1u))goto L8c64763e;goto L8c64766a; /* bt 0x8c64763e */
L8c64766a: *stop=0x8c64766au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64766c; /* mov.l @(0x8,r15),r12 */
L8c64766c: *stop=0x8c64766cu;r[2]=r[13];goto L8c64766e; /* mov r13,r2 */
L8c64766e: *stop=0x8c64766eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6476d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647670; /* mov.l 0x8c6476d0,r3 */
L8c647670: *stop=0x8c647670u;r[2]<<=2;goto L8c647672; /* shll2 r2 */
L8c647672: *stop=0x8c647672u;r[2]<<=2;goto L8c647674; /* shll2 r2 */
L8c647674: *stop=0x8c647674u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6476dcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c647676; /* mov.l 0x8c6476dc,r1 */
L8c647676: *stop=0x8c647676u;r[12]+=r[3];goto L8c647678; /* add r3,r12 */
L8c647678: *stop=0x8c647678u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6476e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64767a; /* mov.l 0x8c6476e0,r3 */
L8c64767a: *stop=0x8c64767au;r[2]<<=2;goto L8c64767c; /* shll2 r2 */
L8c64767c: *stop=0x8c64767cu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c64767e; /* shll r2 */
L8c64767e: *stop=0x8c64767eu;r[2]+=r[1];goto L8c647680; /* add r1,r2 */
L8c647680: *stop=0x8c647680u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c647682; /* mov.l r2,@r15 */
L8c647682: *stop=0x8c647682u;if(!valid_access(r[12]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x30u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647684; /* mov.l @(0x30,r12),r6 */
L8c647684: *stop=0x8c647684u;if(!valid_access(r[12]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x34u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c647686; /* mov.l @(0x34,r12),r7 */
L8c647686: *stop=0x8c647686u;r[6]+=1u;goto L8c647688; /* add #0x1,r6 */
L8c647688: *stop=0x8c647688u;if(!valid_access(r[12]+48u,4u))return -3;wr(m,bus,&io_status,r[12]+0x30u,r[6],4);if(io_status)return io_status;goto L8c64768a; /* mov.l r6,@(0x30,r12) */
L8c64768a: *stop=0x8c64768au;r[6]+=4294967295u;goto L8c64768c; /* add #-0x1,r6 */
L8c64768c: *stop=0x8c64768cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64768e; /* mov.l @r15,r5 */
L8c64768e: *stop=0x8c64768eu;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647690; /* mov.l @(0x8,r5),r5 */
L8c647690: target=r[3];*pr=0x8c647694u;*stop=0x8c647692u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647694; /* jsr @r3 */
L8c647694: *stop=0x8c647694u;r[0]=r[14];goto L8c647696; /* mov r14,r0 */
L8c647696: *stop=0x8c647696u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c647698; /* cmp/eq #0x0,r0 */
L8c647698: if((*sr&1u))goto L8c6476a4;goto L8c64769a; /* bt 0x8c6476a4 */
L8c64769a: *stop=0x8c64769au;r[1]=sx(m,bus,&io_status,0x8c6476c8u,2);if(io_status)return io_status;goto L8c64769c; /* mov.w 0x8c6476c8,r1 */
L8c64769c: *stop=0x8c64769cu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64769e; /* cmp/eq r1,r0 */
L8c64769e: if((*sr&1u))goto L8c6476a8;goto L8c6476a0; /* bt 0x8c6476a8 */
L8c6476a0: *stop=0x8c6476a2u;goto L8c6476ae; /* bra 0x8c6476ae */
L8c6476a4: *stop=0x8c6476a6u;r[0]=0u;goto L8c6476b0; /* bra 0x8c6476b0 */
L8c6476a8: *stop=0x8c6476a8u;r[0]=sx(m,bus,&io_status,0x8c6476c8u,2);if(io_status)return io_status;goto L8c6476aa; /* mov.w 0x8c6476c8,r0 */
L8c6476aa: *stop=0x8c6476acu;goto L8c6476b0; /* bra 0x8c6476b0 */
L8c6476ae: *stop=0x8c6476aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6476e4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6476b0; /* mov.l 0x8c6476e4,r0 */
L8c6476b0: *stop=0x8c6476b0u;r[15]+=12u;goto L8c6476b2; /* add #0xc,r15 */
L8c6476b2: *stop=0x8c6476b2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6476b4; /* lds.l @r15+,PR */
L8c6476b4: *stop=0x8c6476b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6476b6; /* mov.l @r15+,r8 */
L8c6476b6: *stop=0x8c6476b6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6476b8; /* mov.l @r15+,r9 */
L8c6476b8: *stop=0x8c6476b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6476ba; /* mov.l @r15+,r10 */
L8c6476ba: *stop=0x8c6476bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6476bc; /* mov.l @r15+,r11 */
L8c6476bc: *stop=0x8c6476bcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6476be; /* mov.l @r15+,r12 */
L8c6476be: *stop=0x8c6476beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6476c0; /* mov.l @r15+,r13 */
L8c6476c0: *stop=0x8c6476c0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6476c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_state_8c647610(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_state_run_8c647610(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_state_resume_8c647646(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_state_run_8c647610(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c647646u);}
SA_STAGE_EXPORT int sa_movie_filesystem_state_resume_8c647694(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_state_run_8c647610(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c647694u);}
