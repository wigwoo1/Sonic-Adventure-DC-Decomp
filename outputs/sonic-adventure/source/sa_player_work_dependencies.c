/* Original player auxiliary and record-pool constructors plus canonical flag query.
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
static int sa_player_work_dependencies_run_8c04b440(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c04b456u)goto L8c04b456;goto L8c04b440;
L8c04b440: *stop=0x8c04b440u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c04b442; /* sts.l PR,@-r15 */
L8c04b442: *stop=0x8c04b442u;r[15]+=4294967292u;goto L8c04b444; /* add #-0x4,r15 */
L8c04b444: *stop=0x8c04b444u;r[3]=r[4];goto L8c04b446; /* mov r4,r3 */
L8c04b446: *stop=0x8c04b446u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c04b448; /* mov.l r4,@r15 */
L8c04b448: *stop=0x8c04b448u;if(!valid_access(r[3]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c04b44a; /* mov.l @(0x3c,r3),r2 */
L8c04b44a: *stop=0x8c04b44au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c04b44c; /* tst r2,r2 */
L8c04b44c: if(!(*sr&1u))goto L8c04b460;goto L8c04b44e; /* bf 0x8c04b460 */
L8c04b44e: *stop=0x8c04b44eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c04b5e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c04b450; /* mov.l 0x8c04b5e0,r3 */
L8c04b450: *stop=0x8c04b450u;r[5]=124u;goto L8c04b452; /* mov #0x7c,r5 */
L8c04b452: target=r[3];*pr=0x8c04b456u;*stop=0x8c04b454u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04b456; /* jsr @r3 */
L8c04b456: *stop=0x8c04b456u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c04b458; /* mov.l @r15,r2 */
L8c04b458: *stop=0x8c04b458u;r[3]=3u;goto L8c04b45a; /* mov #0x3,r3 */
L8c04b45a: *stop=0x8c04b45au;r[4]=r[0];goto L8c04b45c; /* mov r0,r4 */
L8c04b45c: *stop=0x8c04b45cu;if(!valid_access(r[2]+60u,4u))return -3;wr(m,bus,&io_status,r[2]+0x3cu,r[4],4);if(io_status)return io_status;goto L8c04b45e; /* mov.l r4,@(0x3c,r2) */
L8c04b45e: *stop=0x8c04b45eu;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c04b460; /* mov.b r3,@r4 */
L8c04b460: *stop=0x8c04b460u;r[15]+=4u;goto L8c04b462; /* add #0x4,r15 */
L8c04b462: *stop=0x8c04b462u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c04b464; /* lds.l @r15+,PR */
L8c04b464: *stop=0x8c04b464u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c04b466u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_work_dependencies_8c04b440(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_dependencies_run_8c04b440(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_work_dependencies_resume_8c04b456(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_dependencies_run_8c04b440(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c04b456u);}

static int sa_player_work_dependencies_run_8c07d67c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c07d690u)goto L8c07d690;goto L8c07d67c;
L8c07d67c: *stop=0x8c07d67cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c07d67e; /* mov.l r8,@-r15 */
L8c07d67e: *stop=0x8c07d67eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c07d680; /* sts.l PR,@-r15 */
L8c07d680: *stop=0x8c07d680u;r[15]+=4294967292u;goto L8c07d682; /* add #-0x4,r15 */
L8c07d682: *stop=0x8c07d682u;r[8]=r[4];goto L8c07d684; /* mov r4,r8 */
L8c07d684: *stop=0x8c07d684u;{uint32_t loaded=rd(m,bus,&io_status,0x8c07d6bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07d686; /* mov.l 0x8c07d6bc,r3 */
L8c07d686: *stop=0x8c07d686u;r[5]=32u;goto L8c07d688; /* mov #0x20,r5 */
L8c07d688: *stop=0x8c07d688u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c07d68a; /* mov.l r4,@r15 */
L8c07d68a: *stop=0x8c07d68au;r[8]+=120u;goto L8c07d68c; /* add #0x78,r8 */
L8c07d68c: target=r[3];*pr=0x8c07d690u;*stop=0x8c07d68eu;r[4]=15u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07d690; /* jsr @r3 */
L8c07d690: *stop=0x8c07d690u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c07d692; /* mov.l r0,@r8 */
L8c07d692: *stop=0x8c07d692u;r[0]=0u;goto L8c07d694; /* mov #0x0,r0 */
L8c07d694: *stop=0x8c07d694u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c07d696; /* mov.l @r15,r3 */
L8c07d696: *stop=0x8c07d696u;if(!valid_access(r[3]+18u,2u))return -3;wr(m,bus,&io_status,r[3]+0x12u,r[0],2);if(io_status)return io_status;goto L8c07d698; /* mov.w r0,@(0x12,r3) */
L8c07d698: *stop=0x8c07d698u;r[15]+=4u;goto L8c07d69a; /* add #0x4,r15 */
L8c07d69a: *stop=0x8c07d69au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c07d69c; /* lds.l @r15+,PR */
L8c07d69c: *stop=0x8c07d69cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c07d69eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_work_dependencies_8c07d67c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_dependencies_run_8c07d67c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_work_dependencies_resume_8c07d690(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_dependencies_run_8c07d67c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07d690u);}

SA_STAGE_EXPORT int sa_player_work_dependencies_8c09068e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c09068e;
L8c09068e: *stop=0x8c09068eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0906c0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c090690; /* mov.l 0x8c0906c0,r0 */
L8c090690: *stop=0x8c090690u;if(!valid_access(r[0]+r[4],1u))return -3;r[3]=sx(m,bus,&io_status,r[4]+r[0],1);if(io_status)return io_status;goto L8c090692; /* mov.b @(r0,r4),r3 */
L8c090692: *stop=0x8c090692u;r[3]=r[3]&255u;goto L8c090694; /* extu.b r3,r3 */
L8c090694: *stop=0x8c090694u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c090696; /* tst r3,r3 */
L8c090696: *stop=0x8c090696u;r[0]=*sr&1u;goto L8c090698; /* movt r0 */
L8c090698: *stop=0x8c090698u;if(*pr!=entry_pr)return -3;*stop=0x8c09069au;r[0]^=0x1u;return 0; /* rts */
}
