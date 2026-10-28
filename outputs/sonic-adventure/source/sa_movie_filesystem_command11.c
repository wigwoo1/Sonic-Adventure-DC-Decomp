/* Original command11 file-block read and original handle dependencies.
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
static int sa_movie_filesystem_command11_run_8c64b67a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64b69cu)goto L8c64b69c;if(resume_pc==0x8c64b6c8u)goto L8c64b6c8;if(resume_pc==0x8c64b6f2u)goto L8c64b6f2;if(resume_pc==0x8c64b70au)goto L8c64b70a;goto L8c64b67a;
L8c64b67a: *stop=0x8c64b67au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b67c; /* mov.l r14,@-r15 */
L8c64b67c: *stop=0x8c64b67cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b67e; /* mov.l r13,@-r15 */
L8c64b67e: *stop=0x8c64b67eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b680; /* mov.l r12,@-r15 */
L8c64b680: *stop=0x8c64b680u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64b682; /* mov.l r11,@-r15 */
L8c64b682: *stop=0x8c64b682u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64b684; /* mov.l r10,@-r15 */
L8c64b684: *stop=0x8c64b684u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b686; /* sts.l PR,@-r15 */
L8c64b686: *stop=0x8c64b686u;r[15]+=4294967284u;goto L8c64b688; /* add #-0xc,r15 */
L8c64b688: *stop=0x8c64b688u;r[13]=r[7];goto L8c64b68a; /* mov r7,r13 */
L8c64b68a: *stop=0x8c64b68au;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c64b68c; /* tst r13,r13 */
L8c64b68c: *stop=0x8c64b68cu;r[11]=r[4];goto L8c64b68e; /* mov r4,r11 */
L8c64b68e: *stop=0x8c64b68eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c64b690; /* mov.l r5,@(0x4,r15) */
L8c64b690: take=!(*sr&1u);*stop=0x8c64b692u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;if(take)goto L8c64b6be;goto L8c64b694; /* bf/s 0x8c64b6be */
L8c64b694: *stop=0x8c64b694u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b6b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b696; /* mov.l 0x8c64b6b8,r3 */
L8c64b696: *stop=0x8c64b696u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b698; /* mov.l @(0x4,r15),r5 */
L8c64b698: target=r[3];*pr=0x8c64b69cu;*stop=0x8c64b69au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b69c; /* jsr @r3 */
L8c64b69c: *stop=0x8c64b69cu;r[4]=r[0];goto L8c64b69e; /* mov r0,r4 */
L8c64b69e: *stop=0x8c64b69eu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64b6a0; /* cmp/pz r4 */
L8c64b6a0: if((*sr&1u))goto L8c64b6bc;goto L8c64b6a2; /* bt 0x8c64b6bc */
L8c64b6a2: *stop=0x8c64b6a4u;r[0]=r[4];goto L8c64b70c; /* bra 0x8c64b70c */
L8c64b6bc: *stop=0x8c64b6bcu;r[13]=r[4];goto L8c64b6be; /* mov r4,r13 */
L8c64b6be: *stop=0x8c64b6beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b780u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b6c0; /* mov.l 0x8c64b780,r3 */
L8c64b6c0: *stop=0x8c64b6c0u;r[6]=r[15];goto L8c64b6c2; /* mov r15,r6 */
L8c64b6c2: *stop=0x8c64b6c2u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b6c4; /* mov.l @(0x4,r15),r5 */
L8c64b6c4: target=r[3];*pr=0x8c64b6c8u;*stop=0x8c64b6c6u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b6c8; /* jsr @r3 */
L8c64b6c8: *stop=0x8c64b6c8u;r[14]=r[0];goto L8c64b6ca; /* mov r0,r14 */
L8c64b6ca: *stop=0x8c64b6cau;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b6cc; /* cmp/pz r14 */
L8c64b6cc: if(!(*sr&1u))goto L8c64b70a;goto L8c64b6ce; /* bf 0x8c64b70a */
L8c64b6ce: *stop=0x8c64b6ceu;r[4]=sx(m,bus,&io_status,0x8c64b778u,2);if(io_status)return io_status;goto L8c64b6d0; /* mov.w 0x8c64b778,r4 */
L8c64b6d0: *stop=0x8c64b6d0u;r[2]=1u;goto L8c64b6d2; /* mov #0x1,r2 */
L8c64b6d2: *stop=0x8c64b6d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b784u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b6d4; /* mov.l 0x8c64b784,r3 */
L8c64b6d4: *stop=0x8c64b6d4u;*macl=r[4]*r[11];goto L8c64b6d6; /* mul.l r4,r11 */
L8c64b6d6: *stop=0x8c64b6d6u;r[11]=0u;goto L8c64b6d8; /* mov #0x0,r11 */
L8c64b6d8: *stop=0x8c64b6d8u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64b6da; /* mov.l @(0x8,r15),r12 */
L8c64b6da: *stop=0x8c64b6dau;r[1]=r[11];goto L8c64b6dc; /* mov r11,r1 */
L8c64b6dc: *stop=0x8c64b6dcu;*sr=(*sr&~1u)|(r[1]>=r[13]);goto L8c64b6de; /* cmp/hs r13,r1 */
L8c64b6de: *stop=0x8c64b6deu;r[4]=*macl;goto L8c64b6e0; /* sts MACL,r4 */
L8c64b6e0: *stop=0x8c64b6e0u;r[4]+=r[3];goto L8c64b6e2; /* add r3,r4 */
L8c64b6e2: *stop=0x8c64b6e2u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[2],4);if(io_status)return io_status;goto L8c64b6e4; /* mov.l r2,@(0x30,r4) */
L8c64b6e4: take=(*sr&1u);*stop=0x8c64b6e6u;if(!valid_access(r[4]+52u,4u))return -3;wr(m,bus,&io_status,r[4]+0x34u,r[13],4);if(io_status)return io_status;if(take)goto L8c64b700;goto L8c64b6e8; /* bt/s 0x8c64b700 */
L8c64b6e8: *stop=0x8c64b6e8u;r[10]=sx(m,bus,&io_status,0x8c64b77au,2);if(io_status)return io_status;goto L8c64b6ea; /* mov.w 0x8c64b77a,r10 */
L8c64b6ea: *stop=0x8c64b6eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b788u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b6ec; /* mov.l 0x8c64b788,r3 */
L8c64b6ec: *stop=0x8c64b6ecu;r[5]=r[12];goto L8c64b6ee; /* mov r12,r5 */
L8c64b6ee: target=r[3];*pr=0x8c64b6f2u;*stop=0x8c64b6f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b6f2; /* jsr @r3 */
L8c64b6f2: *stop=0x8c64b6f2u;r[14]=r[0];goto L8c64b6f4; /* mov r0,r14 */
L8c64b6f4: *stop=0x8c64b6f4u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b6f6; /* cmp/pz r14 */
L8c64b6f6: if(!(*sr&1u))goto L8c64b702;goto L8c64b6f8; /* bf 0x8c64b702 */
L8c64b6f8: *stop=0x8c64b6f8u;r[11]+=1u;goto L8c64b6fa; /* add #0x1,r11 */
L8c64b6fa: *stop=0x8c64b6fau;*sr=(*sr&~1u)|(r[11]>=r[13]);goto L8c64b6fc; /* cmp/hs r13,r11 */
L8c64b6fc: take=!(*sr&1u);*stop=0x8c64b6feu;r[12]+=r[10];if(take)goto L8c64b6ea;goto L8c64b700; /* bf/s 0x8c64b6ea */
L8c64b700: *stop=0x8c64b700u;r[14]=0u;goto L8c64b702; /* mov #0x0,r14 */
L8c64b702: *stop=0x8c64b702u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b78cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b704; /* mov.l 0x8c64b78c,r3 */
L8c64b704: *stop=0x8c64b704u;r[5]=0u;goto L8c64b706; /* mov #0x0,r5 */
L8c64b706: target=r[3];*pr=0x8c64b70au;*stop=0x8c64b708u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b70a; /* jsr @r3 */
L8c64b70a: *stop=0x8c64b70au;r[0]=r[14];goto L8c64b70c; /* mov r14,r0 */
L8c64b70c: *stop=0x8c64b70cu;r[15]+=12u;goto L8c64b70e; /* add #0xc,r15 */
L8c64b70e: *stop=0x8c64b70eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b710; /* lds.l @r15+,PR */
L8c64b710: *stop=0x8c64b710u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64b712; /* mov.l @r15+,r10 */
L8c64b712: *stop=0x8c64b712u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64b714; /* mov.l @r15+,r11 */
L8c64b714: *stop=0x8c64b714u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64b716; /* mov.l @r15+,r12 */
L8c64b716: *stop=0x8c64b716u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b718; /* mov.l @r15+,r13 */
L8c64b718: *stop=0x8c64b718u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b71au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_8c64b67a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64b67a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64b69c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64b67a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b69cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64b6c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64b67a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b6c8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64b6f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64b67a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b6f2u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64b70a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64b67a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b70au);}

static int sa_movie_filesystem_command11_run_8c64a360(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64a37eu)goto L8c64a37e;if(resume_pc==0x8c64a388u)goto L8c64a388;if(resume_pc==0x8c64a392u)goto L8c64a392;if(resume_pc==0x8c64a3b4u)goto L8c64a3b4;if(resume_pc==0x8c64a3c8u)goto L8c64a3c8;if(resume_pc==0x8c64a3e0u)goto L8c64a3e0;goto L8c64a360;
L8c64a360: *stop=0x8c64a360u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a362; /* mov.l r14,@-r15 */
L8c64a362: *stop=0x8c64a362u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64a364; /* sts.l PR,@-r15 */
L8c64a364: *stop=0x8c64a364u;r[15]+=4294967248u;goto L8c64a366; /* add #-0x30,r15 */
L8c64a366: *stop=0x8c64a366u;r[3]=sx(m,bus,&io_status,0x8c64a446u,2);if(io_status)return io_status;goto L8c64a368; /* mov.w 0x8c64a446,r3 */
L8c64a368: *stop=0x8c64a368u;r[14]=r[4];goto L8c64a36a; /* mov r4,r14 */
L8c64a36a: *stop=0x8c64a36au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a450u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64a36c; /* mov.l 0x8c64a450,r0 */
L8c64a36c: *stop=0x8c64a36cu;*macl=r[3]*r[14];goto L8c64a36e; /* mul.l r3,r14 */
L8c64a36e: *stop=0x8c64a36eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64a370; /* mov.l r5,@r15 */
L8c64a370: *stop=0x8c64a370u;r[3]=*macl;goto L8c64a372; /* sts MACL,r3 */
L8c64a372: *stop=0x8c64a372u;if(!valid_access(r[0]+r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c64a374; /* mov.b @(r0,r3),r2 */
L8c64a374: *stop=0x8c64a374u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64a376; /* tst r2,r2 */
L8c64a376: if((*sr&1u))goto L8c64a3a4;goto L8c64a378; /* bt 0x8c64a3a4 */
L8c64a378: *stop=0x8c64a378u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a454u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a37a; /* mov.l 0x8c64a454,r3 */
L8c64a37a: target=r[3];*pr=0x8c64a37eu;*stop=0x8c64a37cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a37e; /* jsr @r3 */
L8c64a37e: *stop=0x8c64a37eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a380; /* tst r0,r0 */
L8c64a380: if(!(*sr&1u))goto L8c64a38c;goto L8c64a382; /* bf 0x8c64a38c */
L8c64a382: *stop=0x8c64a382u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a458u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a384; /* mov.l 0x8c64a458,r3 */
L8c64a384: target=r[3];*pr=0x8c64a388u;*stop=0x8c64a386u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a388; /* jsr @r3 */
L8c64a388: *stop=0x8c64a38au;goto L8c64a3a4; /* bra 0x8c64a3a4 */
L8c64a38c: *stop=0x8c64a38cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a45cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a38e; /* mov.l 0x8c64a45c,r2 */
L8c64a38e: target=r[2];*pr=0x8c64a392u;*stop=0x8c64a390u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a392; /* jsr @r2 */
L8c64a392: *stop=0x8c64a392u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a394; /* tst r0,r0 */
L8c64a394: if((*sr&1u))goto L8c64a3a4;goto L8c64a396; /* bt 0x8c64a3a4 */
L8c64a396: *stop=0x8c64a396u;r[3]=sx(m,bus,&io_status,0x8c64a446u,2);if(io_status)return io_status;goto L8c64a398; /* mov.w 0x8c64a446,r3 */
L8c64a398: *stop=0x8c64a398u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a460u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64a39a; /* mov.l 0x8c64a460,r0 */
L8c64a39a: *stop=0x8c64a39au;*macl=r[3]*r[14];goto L8c64a39c; /* mul.l r3,r14 */
L8c64a39c: *stop=0x8c64a39cu;r[3]=*macl;goto L8c64a39e; /* sts MACL,r3 */
L8c64a39e: *stop=0x8c64a39eu;if(!valid_access(r[0]+r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c64a3a0; /* mov.b @(r0,r3),r2 */
L8c64a3a0: *stop=0x8c64a3a0u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64a3a2; /* tst r2,r2 */
L8c64a3a2: if(!(*sr&1u))goto L8c64a3ae;goto L8c64a3a4; /* bf 0x8c64a3ae */
L8c64a3a4: *stop=0x8c64a3a4u;r[0]=sx(m,bus,&io_status,0x8c64a448u,2);if(io_status)return io_status;goto L8c64a3a6; /* mov.w 0x8c64a448,r0 */
L8c64a3a6: *stop=0x8c64a3a6u;r[15]+=48u;goto L8c64a3a8; /* add #0x30,r15 */
L8c64a3a8: *stop=0x8c64a3a8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a3aa; /* lds.l @r15+,PR */
L8c64a3aa: *stop=0x8c64a3aau;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a3acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c64a3ae: *stop=0x8c64a3aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a464u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a3b0; /* mov.l 0x8c64a464,r3 */
L8c64a3b0: target=r[3];*pr=0x8c64a3b4u;*stop=0x8c64a3b2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a3b4; /* jsr @r3 */
L8c64a3b4: *stop=0x8c64a3b4u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64a3b6; /* cmp/pz r0 */
L8c64a3b6: if((*sr&1u))goto L8c64a3c2;goto L8c64a3b8; /* bt 0x8c64a3c2 */
L8c64a3b8: *stop=0x8c64a3b8u;r[0]=sx(m,bus,&io_status,0x8c64a44au,2);if(io_status)return io_status;goto L8c64a3ba; /* mov.w 0x8c64a44a,r0 */
L8c64a3ba: *stop=0x8c64a3bau;r[15]+=48u;goto L8c64a3bc; /* add #0x30,r15 */
L8c64a3bc: *stop=0x8c64a3bcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a3be; /* lds.l @r15+,PR */
L8c64a3be: *stop=0x8c64a3beu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a3c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c64a3c2: *stop=0x8c64a3c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64a3c4; /* mov.l @r15,r5 */
L8c64a3c4: target=0x8c64b2dcu;*pr=0x8c64a3c8u;*stop=0x8c64a3c6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a3c8; /* bsr 0x8c64b2dc */
L8c64a3c8: *stop=0x8c64a3c8u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64a3ca; /* cmp/pz r0 */
L8c64a3ca: if((*sr&1u))goto L8c64a3d6;goto L8c64a3cc; /* bt 0x8c64a3d6 */
L8c64a3cc: *stop=0x8c64a3ccu;r[0]=sx(m,bus,&io_status,0x8c64a44cu,2);if(io_status)return io_status;goto L8c64a3ce; /* mov.w 0x8c64a44c,r0 */
L8c64a3ce: *stop=0x8c64a3ceu;r[15]+=48u;goto L8c64a3d0; /* add #0x30,r15 */
L8c64a3d0: *stop=0x8c64a3d0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a3d2; /* lds.l @r15+,PR */
L8c64a3d2: *stop=0x8c64a3d2u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a3d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c64a3d6: *stop=0x8c64a3d6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64a3d8; /* mov.l @r15,r5 */
L8c64a3d8: *stop=0x8c64a3d8u;r[6]=r[15];goto L8c64a3da; /* mov r15,r6 */
L8c64a3da: *stop=0x8c64a3dau;r[6]+=4u;goto L8c64a3dc; /* add #0x4,r6 */
L8c64a3dc: target=0x8c64ad56u;*pr=0x8c64a3e0u;*stop=0x8c64a3deu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a3e0; /* bsr 0x8c64ad56 */
L8c64a3e0: *stop=0x8c64a3e0u;r[4]=r[0];goto L8c64a3e2; /* mov r0,r4 */
L8c64a3e2: *stop=0x8c64a3e2u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64a3e4; /* cmp/pz r4 */
L8c64a3e4: if((*sr&1u))goto L8c64a3f0;goto L8c64a3e6; /* bt 0x8c64a3f0 */
L8c64a3e6: *stop=0x8c64a3e6u;r[0]=sx(m,bus,&io_status,0x8c64a44eu,2);if(io_status)return io_status;goto L8c64a3e8; /* mov.w 0x8c64a44e,r0 */
L8c64a3e8: *stop=0x8c64a3e8u;r[15]+=48u;goto L8c64a3ea; /* add #0x30,r15 */
L8c64a3ea: *stop=0x8c64a3eau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a3ec; /* lds.l @r15+,PR */
L8c64a3ec: *stop=0x8c64a3ecu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a3eeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c64a3f0: *stop=0x8c64a3f0u;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64a3f2; /* mov.l @(0x2c,r15),r0 */
L8c64a3f2: *stop=0x8c64a3f2u;r[15]+=48u;goto L8c64a3f4; /* add #0x30,r15 */
L8c64a3f4: *stop=0x8c64a3f4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a3f6; /* lds.l @r15+,PR */
L8c64a3f6: *stop=0x8c64a3f6u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a3f8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_8c64a360(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64a360(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64a37e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64a360(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a37eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64a388(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64a360(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a388u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64a392(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64a360(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a392u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64a3b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64a360(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a3b4u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64a3c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64a360(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a3c8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64a3e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64a360(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a3e0u);}

static int sa_movie_filesystem_command11_run_8c648d24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c648d50u)goto L8c648d50;if(resume_pc==0x8c648d58u)goto L8c648d58;if(resume_pc==0x8c648d62u)goto L8c648d62;if(resume_pc==0x8c648d80u)goto L8c648d80;if(resume_pc==0x8c648d92u)goto L8c648d92;if(resume_pc==0x8c648d9eu)goto L8c648d9e;if(resume_pc==0x8c648daeu)goto L8c648dae;if(resume_pc==0x8c648df4u)goto L8c648df4;if(resume_pc==0x8c648e02u)goto L8c648e02;goto L8c648d24;
L8c648d24: *stop=0x8c648d24u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c648d26; /* mov.l r14,@-r15 */
L8c648d26: *stop=0x8c648d26u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c648d28; /* mov.l r13,@-r15 */
L8c648d28: *stop=0x8c648d28u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c648d2a; /* mov.l r12,@-r15 */
L8c648d2a: *stop=0x8c648d2au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c648d2c; /* mov.l r11,@-r15 */
L8c648d2c: *stop=0x8c648d2cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c648d2e; /* sts.l PR,@-r15 */
L8c648d2e: *stop=0x8c648d2eu;r[15]+=4294967248u;goto L8c648d30; /* add #-0x30,r15 */
L8c648d30: *stop=0x8c648d30u;r[12]=0u;goto L8c648d32; /* mov #0x0,r12 */
L8c648d32: *stop=0x8c648d32u;r[3]=r[6];goto L8c648d34; /* mov r6,r3 */
L8c648d34: *stop=0x8c648d34u;r[13]=r[4];goto L8c648d36; /* mov r4,r13 */
L8c648d36: *stop=0x8c648d36u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648dc8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c648d38; /* mov.l 0x8c648dc8,r0 */
L8c648d38: *stop=0x8c648d38u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c648d3a; /* mov.l r6,@r15 */
L8c648d3a: *stop=0x8c648d3au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8c648d3c; /* mov.l r12,@r3 */
L8c648d3c: *stop=0x8c648d3cu;r[3]=sx(m,bus,&io_status,0x8c648dbau,2);if(io_status)return io_status;goto L8c648d3e; /* mov.w 0x8c648dba,r3 */
L8c648d3e: *stop=0x8c648d3eu;*macl=r[3]*r[13];goto L8c648d40; /* mul.l r3,r13 */
L8c648d40: *stop=0x8c648d40u;r[3]=*macl;goto L8c648d42; /* sts MACL,r3 */
L8c648d42: *stop=0x8c648d42u;if(!valid_access(r[0]+r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c648d44; /* mov.b @(r0,r3),r2 */
L8c648d44: *stop=0x8c648d44u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c648d46; /* tst r2,r2 */
L8c648d46: take=(*sr&1u);*stop=0x8c648d48u;r[11]=r[5];if(take)goto L8c648d74;goto L8c648d4a; /* bt/s 0x8c648d74 */
L8c648d4a: *stop=0x8c648d4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c648dccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648d4c; /* mov.l 0x8c648dcc,r3 */
L8c648d4c: target=r[3];*pr=0x8c648d50u;*stop=0x8c648d4eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648d50; /* jsr @r3 */
L8c648d50: *stop=0x8c648d50u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648d52; /* tst r0,r0 */
L8c648d52: if(!(*sr&1u))goto L8c648d5c;goto L8c648d54; /* bf 0x8c648d5c */
L8c648d54: target=0x8c64850au;*pr=0x8c648d58u;*stop=0x8c648d56u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648d58; /* bsr 0x8c64850a */
L8c648d58: *stop=0x8c648d5au;goto L8c648d74; /* bra 0x8c648d74 */
L8c648d5c: *stop=0x8c648d5cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c648dd0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648d5e; /* mov.l 0x8c648dd0,r2 */
L8c648d5e: target=r[2];*pr=0x8c648d62u;*stop=0x8c648d60u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648d62; /* jsr @r2 */
L8c648d62: *stop=0x8c648d62u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648d64; /* tst r0,r0 */
L8c648d64: if((*sr&1u))goto L8c648d74;goto L8c648d66; /* bt 0x8c648d74 */
L8c648d66: *stop=0x8c648d66u;r[3]=sx(m,bus,&io_status,0x8c648dbau,2);if(io_status)return io_status;goto L8c648d68; /* mov.w 0x8c648dba,r3 */
L8c648d68: *stop=0x8c648d68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648dd4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c648d6a; /* mov.l 0x8c648dd4,r0 */
L8c648d6a: *stop=0x8c648d6au;*macl=r[3]*r[13];goto L8c648d6c; /* mul.l r3,r13 */
L8c648d6c: *stop=0x8c648d6cu;r[3]=*macl;goto L8c648d6e; /* sts MACL,r3 */
L8c648d6e: *stop=0x8c648d6eu;if(!valid_access(r[0]+r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c648d70; /* mov.b @(r0,r3),r2 */
L8c648d70: *stop=0x8c648d70u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c648d72; /* tst r2,r2 */
L8c648d72: if(!(*sr&1u))goto L8c648d7a;goto L8c648d74; /* bf 0x8c648d7a */
L8c648d74: *stop=0x8c648d74u;r[0]=sx(m,bus,&io_status,0x8c648dbcu,2);if(io_status)return io_status;goto L8c648d76; /* mov.w 0x8c648dbc,r0 */
L8c648d76: *stop=0x8c648d78u;goto L8c648e28; /* bra 0x8c648e28 */
L8c648d7a: *stop=0x8c648d7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c648dd8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648d7c; /* mov.l 0x8c648dd8,r3 */
L8c648d7c: target=r[3];*pr=0x8c648d80u;*stop=0x8c648d7eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648d80; /* jsr @r3 */
L8c648d80: *stop=0x8c648d80u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648d82; /* cmp/pz r0 */
L8c648d82: if((*sr&1u))goto L8c648d8a;goto L8c648d84; /* bt 0x8c648d8a */
L8c648d84: *stop=0x8c648d84u;r[0]=sx(m,bus,&io_status,0x8c648dbeu,2);if(io_status)return io_status;goto L8c648d86; /* mov.w 0x8c648dbe,r0 */
L8c648d86: *stop=0x8c648d88u;goto L8c648e28; /* bra 0x8c648e28 */
L8c648d8a: *stop=0x8c648d8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c648ddcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648d8c; /* mov.l 0x8c648ddc,r3 */
L8c648d8c: *stop=0x8c648d8cu;r[5]=r[11];goto L8c648d8e; /* mov r11,r5 */
L8c648d8e: target=r[3];*pr=0x8c648d92u;*stop=0x8c648d90u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648d92; /* jsr @r3 */
L8c648d92: *stop=0x8c648d92u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648d94; /* cmp/pz r0 */
L8c648d94: if(!(*sr&1u))goto L8c648db4;goto L8c648d96; /* bf 0x8c648db4 */
L8c648d96: *stop=0x8c648d96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648de0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648d98; /* mov.l 0x8c648de0,r2 */
L8c648d98: *stop=0x8c648d98u;r[5]=r[11];goto L8c648d9a; /* mov r11,r5 */
L8c648d9a: target=r[2];*pr=0x8c648d9eu;*stop=0x8c648d9cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648d9e; /* jsr @r2 */
L8c648d9e: *stop=0x8c648d9eu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648da0; /* cmp/pz r0 */
L8c648da0: if((*sr&1u))goto L8c648da8;goto L8c648da2; /* bt 0x8c648da8 */
L8c648da2: *stop=0x8c648da2u;r[0]=sx(m,bus,&io_status,0x8c648dc0u,2);if(io_status)return io_status;goto L8c648da4; /* mov.w 0x8c648dc0,r0 */
L8c648da4: *stop=0x8c648da6u;goto L8c648e28; /* bra 0x8c648e28 */
L8c648da8: *stop=0x8c648da8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648de4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648daa; /* mov.l 0x8c648de4,r2 */
L8c648daa: target=r[2];*pr=0x8c648daeu;*stop=0x8c648dacu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648dae; /* jsr @r2 */
L8c648dae: *stop=0x8c648daeu;r[14]=r[0];goto L8c648db0; /* mov r0,r14 */
L8c648db0: *stop=0x8c648db0u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c648db2; /* tst r14,r14 */
L8c648db2: if(!(*sr&1u))goto L8c648de8;goto L8c648db4; /* bf 0x8c648de8 */
L8c648db4: *stop=0x8c648db4u;r[0]=sx(m,bus,&io_status,0x8c648dc2u,2);if(io_status)return io_status;goto L8c648db6; /* mov.w 0x8c648dc2,r0 */
L8c648db6: *stop=0x8c648db8u;goto L8c648e28; /* bra 0x8c648e28 */
L8c648de8: *stop=0x8c648de8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648edcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648dea; /* mov.l 0x8c648edc,r2 */
L8c648dea: *stop=0x8c648deau;r[6]=r[15];goto L8c648dec; /* mov r15,r6 */
L8c648dec: *stop=0x8c648decu;r[5]=r[11];goto L8c648dee; /* mov r11,r5 */
L8c648dee: *stop=0x8c648deeu;r[6]+=4u;goto L8c648df0; /* add #0x4,r6 */
L8c648df0: target=r[2];*pr=0x8c648df4u;*stop=0x8c648df2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648df4; /* jsr @r2 */
L8c648df4: *stop=0x8c648df4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648ee0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648df6; /* mov.l 0x8c648ee0,r3 */
L8c648df6: *stop=0x8c648df6u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648df8; /* cmp/pz r0 */
L8c648df8: take=(*sr&1u);*stop=0x8c648dfau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;if(take)goto L8c648e08;goto L8c648dfc; /* bt/s 0x8c648e08 */
L8c648dfc: *stop=0x8c648dfcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c648ee4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c648dfe; /* mov.l 0x8c648ee4,r1 */
L8c648dfe: target=r[1];*pr=0x8c648e02u;*stop=0x8c648e00u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648e02; /* jsr @r1 */
L8c648e02: *stop=0x8c648e02u;r[0]=sx(m,bus,&io_status,0x8c648ed2u,2);if(io_status)return io_status;goto L8c648e04; /* mov.w 0x8c648ed2,r0 */
L8c648e04: *stop=0x8c648e06u;goto L8c648e28; /* bra 0x8c648e28 */
L8c648e08: *stop=0x8c648e08u;r[0]=64u;goto L8c648e0a; /* mov #0x40,r0 */
L8c648e0a: *stop=0x8c648e0au;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[12],4);if(io_status)return io_status;goto L8c648e0c; /* mov.l r12,@(0x4,r14) */
L8c648e0c: *stop=0x8c648e0cu;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648e0e; /* mov.l @(0x2c,r15),r3 */
L8c648e0e: *stop=0x8c648e0eu;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[3],4);if(io_status)return io_status;goto L8c648e10; /* mov.l r3,@(0x34,r14) */
L8c648e10: *stop=0x8c648e10u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648e12; /* mov.l @(0x10,r15),r2 */
L8c648e12: *stop=0x8c648e12u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[2],4);if(io_status)return io_status;goto L8c648e14; /* mov.l r2,@(0x38,r14) */
L8c648e14: *stop=0x8c648e14u;r[3]=r[2];goto L8c648e16; /* mov r2,r3 */
L8c648e16: *stop=0x8c648e16u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c648e18; /* mov.l r2,@(r0,r14) */
L8c648e18: *stop=0x8c648e18u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[13],4);if(io_status)return io_status;goto L8c648e1a; /* mov.l r13,@(0x28,r14) */
L8c648e1a: *stop=0x8c648e1au;if(!valid_access(r[15]+12u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0xcu,2);if(io_status)return io_status;goto L8c648e1c; /* mov.w @(0xc,r15),r0 */
L8c648e1c: *stop=0x8c648e1cu;if(!valid_access(r[14]+2u,2u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c648e1e; /* mov.w r0,@(0x2,r14) */
L8c648e1e: *stop=0x8c648e1eu;if(!valid_access(r[15]+14u,1u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0xeu,1);if(io_status)return io_status;goto L8c648e20; /* mov.b @(0xe,r15),r0 */
L8c648e20: *stop=0x8c648e20u;if(!valid_access(r[14]+1u,1u))return -3;wr(m,bus,&io_status,r[14]+0x1u,r[0],1);if(io_status)return io_status;goto L8c648e22; /* mov.b r0,@(0x1,r14) */
L8c648e22: *stop=0x8c648e22u;r[0]=r[12];goto L8c648e24; /* mov r12,r0 */
L8c648e24: *stop=0x8c648e24u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c648e26; /* mov.l @r15,r3 */
L8c648e26: *stop=0x8c648e26u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c648e28; /* mov.l r14,@r3 */
L8c648e28: *stop=0x8c648e28u;r[15]+=48u;goto L8c648e2a; /* add #0x30,r15 */
L8c648e2a: *stop=0x8c648e2au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c648e2c; /* lds.l @r15+,PR */
L8c648e2c: *stop=0x8c648e2cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c648e2e; /* mov.l @r15+,r11 */
L8c648e2e: *stop=0x8c648e2eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c648e30; /* mov.l @r15+,r12 */
L8c648e30: *stop=0x8c648e30u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c648e32; /* mov.l @r15+,r13 */
L8c648e32: *stop=0x8c648e32u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c648e34u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_8c648d24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c648d24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c648d50(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c648d24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648d50u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c648d58(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c648d24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648d58u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c648d62(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c648d24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648d62u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c648d80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c648d24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648d80u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c648d92(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c648d24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648d92u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c648d9e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c648d24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648d9eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c648dae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c648d24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648daeu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c648df4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c648d24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648df4u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c648e02(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c648d24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648e02u);}

static int sa_movie_filesystem_command11_run_8c64965c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c649670u)goto L8c649670;if(resume_pc==0x8c64967au)goto L8c64967a;if(resume_pc==0x8c649684u)goto L8c649684;if(resume_pc==0x8c6496ceu)goto L8c6496ce;if(resume_pc==0x8c64971cu)goto L8c64971c;if(resume_pc==0x8c64972eu)goto L8c64972e;goto L8c64965c;
L8c64965c: *stop=0x8c64965cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64965e; /* mov.l r14,@-r15 */
L8c64965e: *stop=0x8c64965eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c649660; /* mov.l r13,@-r15 */
L8c649660: *stop=0x8c649660u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c649662; /* mov.l r12,@-r15 */
L8c649662: *stop=0x8c649662u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c649664; /* sts.l PR,@-r15 */
L8c649664: *stop=0x8c649664u;r[15]+=4294967292u;goto L8c649666; /* add #-0x4,r15 */
L8c649666: *stop=0x8c649666u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6496b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649668; /* mov.l 0x8c6496b8,r3 */
L8c649668: *stop=0x8c649668u;r[14]=r[4];goto L8c64966a; /* mov r4,r14 */
L8c64966a: *stop=0x8c64966au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64966c; /* mov.l r5,@r15 */
L8c64966c: target=r[3];*pr=0x8c649670u;*stop=0x8c64966eu;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649670; /* jsr @r3 */
L8c649670: *stop=0x8c649670u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649672; /* tst r0,r0 */
L8c649672: if(!(*sr&1u))goto L8c64967e;goto L8c649674; /* bf 0x8c64967e */
L8c649674: *stop=0x8c649674u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6496bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649676; /* mov.l 0x8c6496bc,r3 */
L8c649676: target=r[3];*pr=0x8c64967au;*stop=0x8c649678u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64967a; /* jsr @r3 */
L8c64967a: *stop=0x8c64967cu;goto L8c649698; /* bra 0x8c649698 */
L8c64967e: *stop=0x8c64967eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6496c0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649680; /* mov.l 0x8c6496c0,r2 */
L8c649680: target=r[2];*pr=0x8c649684u;*stop=0x8c649682u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649684; /* jsr @r2 */
L8c649684: *stop=0x8c649684u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649686; /* tst r0,r0 */
L8c649686: if((*sr&1u))goto L8c649698;goto L8c649688; /* bt 0x8c649698 */
L8c649688: *stop=0x8c649688u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64968a; /* mov.l @(0x28,r14),r2 */
L8c64968a: *stop=0x8c64968au;r[3]=sx(m,bus,&io_status,0x8c6496a0u,2);if(io_status)return io_status;goto L8c64968c; /* mov.w 0x8c6496a0,r3 */
L8c64968c: *stop=0x8c64968cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6496c4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64968e; /* mov.l 0x8c6496c4,r0 */
L8c64968e: *stop=0x8c64968eu;*macl=r[3]*r[2];goto L8c649690; /* mul.l r3,r2 */
L8c649690: *stop=0x8c649690u;r[3]=*macl;goto L8c649692; /* sts MACL,r3 */
L8c649692: *stop=0x8c649692u;if(!valid_access(r[0]+r[3],1u))return -3;r[1]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c649694; /* mov.b @(r0,r3),r1 */
L8c649694: *stop=0x8c649694u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c649696; /* tst r1,r1 */
L8c649696: if(!(*sr&1u))goto L8c6496c8;goto L8c649698; /* bf 0x8c6496c8 */
L8c649698: *stop=0x8c649698u;r[0]=sx(m,bus,&io_status,0x8c6496a2u,2);if(io_status)return io_status;goto L8c64969a; /* mov.w 0x8c6496a2,r0 */
L8c64969a: *stop=0x8c64969cu;goto L8c64973e; /* bra 0x8c64973e */
L8c6496c8: *stop=0x8c6496c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6497a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6496ca; /* mov.l 0x8c6497a0,r3 */
L8c6496ca: target=r[3];*pr=0x8c6496ceu;*stop=0x8c6496ccu;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6496ce; /* jsr @r3 */
L8c6496ce: *stop=0x8c6496ceu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c6496d0; /* cmp/pz r0 */
L8c6496d0: if((*sr&1u))goto L8c6496d8;goto L8c6496d2; /* bt 0x8c6496d8 */
L8c6496d2: *stop=0x8c6496d2u;r[0]=sx(m,bus,&io_status,0x8c649798u,2);if(io_status)return io_status;goto L8c6496d4; /* mov.w 0x8c649798,r0 */
L8c6496d4: *stop=0x8c6496d6u;goto L8c64973e; /* bra 0x8c64973e */
L8c6496d8: *stop=0x8c6496d8u;r[0]=64u;goto L8c6496da; /* mov #0x40,r0 */
L8c6496da: *stop=0x8c6496dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6497a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6496dc; /* mov.l 0x8c6497a4,r3 */
L8c6496dc: *stop=0x8c6496dcu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6496de; /* mov.l @(r0,r14),r2 */
L8c6496de: *stop=0x8c6496deu;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c6496e0; /* cmp/eq r3,r2 */
L8c6496e0: take=!(*sr&1u);*stop=0x8c6496e2u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[13]=loaded;}if(take)goto L8c6496f0;goto L8c6496e4; /* bf/s 0x8c6496f0 */
L8c6496e4: *stop=0x8c6496e4u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6496e6; /* mov.l @(0x34,r14),r2 */
L8c6496e6: *stop=0x8c6496e6u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6496e8; /* tst r2,r2 */
L8c6496e8: if((*sr&1u))goto L8c6496f6;goto L8c6496ea; /* bt 0x8c6496f6 */
L8c6496ea: *stop=0x8c6496eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6497a8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6496ec; /* mov.l 0x8c6497a8,r0 */
L8c6496ec: *stop=0x8c6496eeu;goto L8c64973e; /* bra 0x8c64973e */
L8c6496f0: *stop=0x8c6496f0u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6496f2; /* mov.l @(0x34,r14),r2 */
L8c6496f2: *stop=0x8c6496f2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6496f4; /* tst r2,r2 */
L8c6496f4: if(!(*sr&1u))goto L8c6496fa;goto L8c6496f6; /* bf 0x8c6496fa */
L8c6496f6: *stop=0x8c6496f8u;r[0]=0u;goto L8c64973e; /* bra 0x8c64973e */
L8c6496fa: *stop=0x8c6496fau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6496fc; /* mov.l @(r0,r14),r2 */
L8c6496fc: *stop=0x8c6496fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6497acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6496fe; /* mov.l 0x8c6497ac,r3 */
L8c6496fe: *stop=0x8c6496feu;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c649700; /* cmp/eq r3,r2 */
L8c649700: if((*sr&1u))goto L8c64970a;goto L8c649702; /* bt 0x8c64970a */
L8c649702: *stop=0x8c649702u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c649704; /* mov.l @(r0,r14),r2 */
L8c649704: *stop=0x8c649704u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6497b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649706; /* mov.l 0x8c6497b0,r3 */
L8c649706: *stop=0x8c649706u;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c649708; /* cmp/eq r3,r2 */
L8c649708: if(!(*sr&1u))goto L8c649710;goto L8c64970a; /* bf 0x8c649710 */
L8c64970a: *stop=0x8c64970au;r[0]=sx(m,bus,&io_status,0x8c64979au,2);if(io_status)return io_status;goto L8c64970c; /* mov.w 0x8c64979a,r0 */
L8c64970c: *stop=0x8c64970eu;goto L8c64973e; /* bra 0x8c64973e */
L8c649710: *stop=0x8c649710u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[12]=loaded;}goto L8c649712; /* mov.l @(r0,r14),r12 */
L8c649712: *stop=0x8c649712u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6497b4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649714; /* mov.l 0x8c6497b4,r3 */
L8c649714: *stop=0x8c649714u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c649716; /* mov.l @r15,r6 */
L8c649716: *stop=0x8c649716u;r[5]=r[12];goto L8c649718; /* mov r12,r5 */
L8c649718: target=r[3];*pr=0x8c64971cu;*stop=0x8c64971au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64971c; /* jsr @r3 */
L8c64971c: *stop=0x8c64971cu;r[4]=r[0];goto L8c64971e; /* mov r0,r4 */
L8c64971e: *stop=0x8c64971eu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c649720; /* cmp/pz r4 */
L8c649720: if((*sr&1u))goto L8c649726;goto L8c649722; /* bt 0x8c649726 */
L8c649722: *stop=0x8c649724u;r[0]=r[4];goto L8c64973e; /* bra 0x8c64973e */
L8c649726: *stop=0x8c649726u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6497b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649728; /* mov.l 0x8c6497b8,r3 */
L8c649728: *stop=0x8c649728u;r[5]=r[12];goto L8c64972a; /* mov r12,r5 */
L8c64972a: target=r[3];*pr=0x8c64972eu;*stop=0x8c64972cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64972e; /* jsr @r3 */
L8c64972e: *stop=0x8c64972eu;r[1]=64u;goto L8c649730; /* mov #0x40,r1 */
L8c649730: *stop=0x8c649730u;r[1]+=r[14];goto L8c649732; /* add r14,r1 */
L8c649732: *stop=0x8c649732u;r[0]=r[0]&65535u;goto L8c649734; /* extu.w r0,r0 */
L8c649734: *stop=0x8c649734u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c649736; /* mov.l r0,@r1 */
L8c649736: *stop=0x8c649736u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649738; /* mov.l @(0x34,r14),r3 */
L8c649738: *stop=0x8c649738u;r[3]+=4294967295u;goto L8c64973a; /* add #-0x1,r3 */
L8c64973a: *stop=0x8c64973au;r[0]=r[3];goto L8c64973c; /* mov r3,r0 */
L8c64973c: *stop=0x8c64973cu;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[3],4);if(io_status)return io_status;goto L8c64973e; /* mov.l r3,@(0x34,r14) */
L8c64973e: *stop=0x8c64973eu;r[15]+=4u;goto L8c649740; /* add #0x4,r15 */
L8c649740: *stop=0x8c649740u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c649742; /* lds.l @r15+,PR */
L8c649742: *stop=0x8c649742u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c649744; /* mov.l @r15+,r12 */
L8c649744: *stop=0x8c649744u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c649746; /* mov.l @r15+,r13 */
L8c649746: *stop=0x8c649746u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c649748u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_8c64965c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64965c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c649670(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64965c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649670u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64967a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64965c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64967au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c649684(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64965c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649684u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c6496ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64965c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6496ceu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64971c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64965c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64971cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64972e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64965c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64972eu);}

static int sa_movie_filesystem_command11_run_8c64919e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c6491d8u)goto L8c6491d8;if(resume_pc==0x8c6491f8u)goto L8c6491f8;if(resume_pc==0x8c649200u)goto L8c649200;if(resume_pc==0x8c64920au)goto L8c64920a;if(resume_pc==0x8c649240u)goto L8c649240;if(resume_pc==0x8c64928au)goto L8c64928a;if(resume_pc==0x8c649296u)goto L8c649296;if(resume_pc==0x8c6492a4u)goto L8c6492a4;if(resume_pc==0x8c6492dau)goto L8c6492da;if(resume_pc==0x8c6492e0u)goto L8c6492e0;if(resume_pc==0x8c6492ecu)goto L8c6492ec;if(resume_pc==0x8c6492f4u)goto L8c6492f4;goto L8c64919e;
L8c64919e: *stop=0x8c64919eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6491a0; /* mov.l r14,@-r15 */
L8c6491a0: *stop=0x8c6491a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6491a2; /* mov.l r13,@-r15 */
L8c6491a2: *stop=0x8c6491a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6491a4; /* mov.l r12,@-r15 */
L8c6491a4: *stop=0x8c6491a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6491a6; /* mov.l r11,@-r15 */
L8c6491a6: *stop=0x8c6491a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6491a8; /* mov.l r10,@-r15 */
L8c6491a8: *stop=0x8c6491a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6491aa; /* sts.l PR,@-r15 */
L8c6491aa: *stop=0x8c6491aau;r[15]+=4294967292u;goto L8c6491ac; /* add #-0x4,r15 */
L8c6491ac: *stop=0x8c6491acu;r[14]=r[4];goto L8c6491ae; /* mov r4,r14 */
L8c6491ae: *stop=0x8c6491aeu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6491b0; /* tst r14,r14 */
L8c6491b0: take=!(*sr&1u);*stop=0x8c6491b2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;if(take)goto L8c6491ba;goto L8c6491b4; /* bf/s 0x8c6491ba */
L8c6491b4: *stop=0x8c6491b4u;r[0]=sx(m,bus,&io_status,0x8c649254u,2);if(io_status)return io_status;goto L8c6491b6; /* mov.w 0x8c649254,r0 */
L8c6491b6: *stop=0x8c6491b8u;goto L8c6492f6; /* bra 0x8c6492f6 */
L8c6491ba: *stop=0x8c6491bau;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6491bc; /* mov.l @(0x28,r14),r2 */
L8c6491bc: *stop=0x8c6491bcu;r[11]=sx(m,bus,&io_status,0x8c64924eu,2);if(io_status)return io_status;goto L8c6491be; /* mov.w 0x8c64924e,r11 */
L8c6491be: *stop=0x8c6491beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649264u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c6491c0; /* mov.l 0x8c649264,r10 */
L8c6491c0: *stop=0x8c6491c0u;*macl=r[11]*r[2];goto L8c6491c2; /* mul.l r11,r2 */
L8c6491c2: *stop=0x8c6491c2u;r[2]=*macl;goto L8c6491c4; /* sts MACL,r2 */
L8c6491c4: *stop=0x8c6491c4u;r[2]+=r[10];goto L8c6491c6; /* add r10,r2 */
L8c6491c6: *stop=0x8c6491c6u;if(!valid_access(r[2]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,1);if(io_status)return io_status;goto L8c6491c8; /* mov.b @(0x2,r2),r0 */
L8c6491c8: *stop=0x8c6491c8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6491ca; /* tst r0,r0 */
L8c6491ca: take=!(*sr&1u);*stop=0x8c6491ccu;r[13]=0u;if(take)goto L8c6491d2;goto L8c6491ce; /* bf/s 0x8c6491d2 */
L8c6491ce: *stop=0x8c6491d0u;goto L8c6492ee; /* bra 0x8c6492ee */
L8c6491d2: *stop=0x8c6491d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649258u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6491d4; /* mov.l 0x8c649258,r2 */
L8c6491d4: target=r[2];*pr=0x8c6491d8u;*stop=0x8c6491d6u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6491d8; /* jsr @r2 */
L8c6491d8: *stop=0x8c6491d8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6491da; /* tst r0,r0 */
L8c6491da: if(!(*sr&1u))goto L8c6491e0;goto L8c6491dc; /* bf 0x8c6491e0 */
L8c6491dc: *stop=0x8c6491deu;goto L8c6492ee; /* bra 0x8c6492ee */
L8c6491e0: *stop=0x8c6491e0u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6491e2; /* mov.l @(0x28,r14),r3 */
L8c6491e2: *stop=0x8c6491e2u;*macl=r[11]*r[3];goto L8c6491e4; /* mul.l r11,r3 */
L8c6491e4: *stop=0x8c6491e4u;r[3]=*macl;goto L8c6491e6; /* sts MACL,r3 */
L8c6491e6: *stop=0x8c6491e6u;r[3]+=r[10];goto L8c6491e8; /* add r10,r3 */
L8c6491e8: *stop=0x8c6491e8u;if(!valid_access(r[3]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[3]+0x8u,1);if(io_status)return io_status;goto L8c6491ea; /* mov.b @(0x8,r3),r0 */
L8c6491ea: *stop=0x8c6491eau;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6491ec; /* tst r0,r0 */
L8c6491ec: if(!(*sr&1u))goto L8c6491f2;goto L8c6491ee; /* bf 0x8c6491f2 */
L8c6491ee: *stop=0x8c6491f0u;goto L8c6492ee; /* bra 0x8c6492ee */
L8c6491f2: *stop=0x8c6491f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649268u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6491f4; /* mov.l 0x8c649268,r3 */
L8c6491f4: target=r[3];*pr=0x8c6491f8u;*stop=0x8c6491f6u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6491f8; /* jsr @r3 */
L8c6491f8: *stop=0x8c6491f8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6491fa; /* tst r0,r0 */
L8c6491fa: if(!(*sr&1u))goto L8c649204;goto L8c6491fc; /* bf 0x8c649204 */
L8c6491fc: target=0x8c64850au;*pr=0x8c649200u;*stop=0x8c6491feu;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649200; /* bsr 0x8c64850a */
L8c649200: *stop=0x8c649202u;goto L8c6492ee; /* bra 0x8c6492ee */
L8c649204: *stop=0x8c649204u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649260u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649206; /* mov.l 0x8c649260,r3 */
L8c649206: target=r[3];*pr=0x8c64920au;*stop=0x8c649208u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64920a; /* jsr @r3 */
L8c64920a: *stop=0x8c64920au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64920c; /* cmp/pz r0 */
L8c64920c: if(!(*sr&1u))goto L8c6492ee;goto L8c64920e; /* bf 0x8c6492ee */
L8c64920e: *stop=0x8c64920eu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649210; /* mov.l @(0x4,r14),r2 */
L8c649210: *stop=0x8c649210u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c649212; /* tst r2,r2 */
L8c649212: take=(*sr&1u);*stop=0x8c649214u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[12]=loaded;}if(take)goto L8c6492ec;goto L8c649216; /* bt/s 0x8c6492ec */
L8c649216: *stop=0x8c649216u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c649218; /* mov.l @r15,r3 */
L8c649218: *stop=0x8c649218u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64921a; /* tst r3,r3 */
L8c64921a: if((*sr&1u))goto L8c649238;goto L8c64921c; /* bt 0x8c649238 */
L8c64921c: *stop=0x8c64921cu;*macl=r[11]*r[12];goto L8c64921e; /* mul.l r11,r12 */
L8c64921e: *stop=0x8c64921eu;r[3]=1u;goto L8c649220; /* mov #0x1,r3 */
L8c649220: *stop=0x8c649220u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64926cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c649222; /* mov.l 0x8c64926c,r0 */
L8c649222: *stop=0x8c649222u;r[4]=*macl;goto L8c649224; /* sts MACL,r4 */
L8c649224: *stop=0x8c649224u;r[4]+=r[10];goto L8c649226; /* add r10,r4 */
L8c649226: *stop=0x8c649226u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[3],4);if(io_status)return io_status;goto L8c649228; /* mov.l r3,@(0x30,r4) */
L8c649228: *stop=0x8c649228u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64922a; /* mov.l @(0x4,r4),r2 */
L8c64922a: *stop=0x8c64922au;r[3]=r[2];goto L8c64922c; /* mov r2,r3 */
L8c64922c: *stop=0x8c64922cu;r[2]<<=2;goto L8c64922e; /* shll2 r2 */
L8c64922e: *stop=0x8c64922eu;r[2]+=r[3];goto L8c649230; /* add r3,r2 */
L8c649230: *stop=0x8c649230u;r[2]<<=2;goto L8c649232; /* shll2 r2 */
L8c649232: *stop=0x8c649232u;r[2]<<=2;goto L8c649234; /* shll2 r2 */
L8c649234: *stop=0x8c649234u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c649236; /* mov.l @(r0,r2),r1 */
L8c649236: *stop=0x8c649236u;if(!valid_access(r[4]+52u,4u))return -3;wr(m,bus,&io_status,r[4]+0x34u,r[1],4);if(io_status)return io_status;goto L8c649238; /* mov.l r1,@(0x34,r4) */
L8c649238: *stop=0x8c649238u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649270u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64923a; /* mov.l 0x8c649270,r3 */
L8c64923a: *stop=0x8c64923au;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64923c; /* mov.l @(0x2c,r14),r5 */
L8c64923c: target=r[3];*pr=0x8c649240u;*stop=0x8c64923eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649240; /* jsr @r3 */
L8c649240: *stop=0x8c649240u;r[13]=r[0];goto L8c649242; /* mov r0,r13 */
L8c649242: *stop=0x8c649242u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c649244; /* mov.l @(0x8,r14),r0 */
L8c649244: *stop=0x8c649244u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c649246; /* cmp/eq #0x1,r0 */
L8c649246: if(!(*sr&1u))goto L8c649274;goto L8c649248; /* bf 0x8c649274 */
L8c649248: *stop=0x8c649248u;r[4]=sx(m,bus,&io_status,0x8c649256u,2);if(io_status)return io_status;goto L8c64924a; /* mov.w 0x8c649256,r4 */
L8c64924a: *stop=0x8c64924cu;goto L8c649276; /* bra 0x8c649276 */
L8c649274: *stop=0x8c649274u;r[4]=51u;goto L8c649276; /* mov #0x33,r4 */
L8c649276: *stop=0x8c649276u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649364u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c649278; /* mov.l 0x8c649364,r11 */
L8c649278: *stop=0x8c649278u;r[5]=2u;goto L8c64927a; /* mov #0x2,r5 */
L8c64927a: *stop=0x8c64927au;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[4],1);if(io_status)return io_status;goto L8c64927c; /* mov.b r4,@r13 */
L8c64927c: *stop=0x8c64927cu;if(!valid_access(r[14]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x1u,1);if(io_status)return io_status;goto L8c64927e; /* mov.b @(0x1,r14),r0 */
L8c64927e: *stop=0x8c64927eu;if(!valid_access(r[13]+1u,1u))return -3;wr(m,bus,&io_status,r[13]+0x1u,r[0],1);if(io_status)return io_status;goto L8c649280; /* mov.b r0,@(0x1,r13) */
L8c649280: *stop=0x8c649280u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c649282; /* mov.l @r11,r3 */
L8c649282: *stop=0x8c649282u;if(!valid_access(r[14]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x38u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c649284; /* mov.l @(0x38,r14),r6 */
L8c649284: *stop=0x8c649284u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649286; /* mov.l @(0x10,r3),r2 */
L8c649286: target=r[2];*pr=0x8c64928au;*stop=0x8c649288u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64928a; /* jsr @r2 */
L8c64928a: *stop=0x8c64928au;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64928c; /* mov.l @r11,r3 */
L8c64928c: *stop=0x8c64928cu;r[5]=24u;goto L8c64928e; /* mov #0x18,r5 */
L8c64928e: *stop=0x8c64928eu;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c649290; /* mov.l @(0x30,r14),r6 */
L8c649290: *stop=0x8c649290u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649292; /* mov.l @(0x10,r3),r2 */
L8c649292: target=r[2];*pr=0x8c649296u;*stop=0x8c649294u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649296; /* jsr @r2 */
L8c649296: *stop=0x8c649296u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c649298; /* mov.l @r11,r3 */
L8c649298: *stop=0x8c649298u;r[5]=26u;goto L8c64929a; /* mov #0x1a,r5 */
L8c64929a: *stop=0x8c64929au;if(!valid_access(r[14]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c64929c; /* mov.w @(0x2,r14),r0 */
L8c64929c: *stop=0x8c64929cu;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64929e; /* mov.l @(0x10,r3),r2 */
L8c64929e: *stop=0x8c64929eu;r[6]=r[0];goto L8c6492a0; /* mov r0,r6 */
L8c6492a0: target=r[2];*pr=0x8c6492a4u;*stop=0x8c6492a2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6492a4; /* jsr @r2 */
L8c6492a4: *stop=0x8c6492a4u;r[4]=r[13];goto L8c6492a6; /* mov r13,r4 */
L8c6492a6: *stop=0x8c6492a6u;r[5]=r[14];goto L8c6492a8; /* mov r14,r5 */
L8c6492a8: *stop=0x8c6492a8u;r[11]=r[13];goto L8c6492aa; /* mov r13,r11 */
L8c6492aa: *stop=0x8c6492aau;r[4]+=4u;goto L8c6492ac; /* add #0x4,r4 */
L8c6492ac: *stop=0x8c6492acu;r[5]+=16u;goto L8c6492ae; /* add #0x10,r5 */
L8c6492ae: *stop=0x8c6492b0u;r[11]+=16u;goto L8c6492b8; /* bra 0x8c6492b8 */
L8c6492b2: *stop=0x8c6492b2u;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c6492b4; /* mov.b @r5+,r3 */
L8c6492b4: *stop=0x8c6492b4u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c6492b6; /* mov.b r3,@r4 */
L8c6492b6: *stop=0x8c6492b6u;r[4]+=1u;goto L8c6492b8; /* add #0x1,r4 */
L8c6492b8: *stop=0x8c6492b8u;*sr=(*sr&~1u)|(r[4]>=r[11]);goto L8c6492ba; /* cmp/hs r11,r4 */
L8c6492ba: if(!(*sr&1u))goto L8c6492b2;goto L8c6492bc; /* bf 0x8c6492b2 */
L8c6492bc: *stop=0x8c6492bcu;r[5]=r[14];goto L8c6492be; /* mov r14,r5 */
L8c6492be: *stop=0x8c6492beu;r[6]=r[13];goto L8c6492c0; /* mov r13,r6 */
L8c6492c0: *stop=0x8c6492c0u;r[4]=r[11];goto L8c6492c2; /* mov r11,r4 */
L8c6492c2: *stop=0x8c6492c2u;r[5]+=32u;goto L8c6492c4; /* add #0x20,r5 */
L8c6492c4: *stop=0x8c6492c6u;r[6]+=24u;goto L8c6492ce; /* bra 0x8c6492ce */
L8c6492c8: *stop=0x8c6492c8u;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c6492ca; /* mov.b @r5+,r3 */
L8c6492ca: *stop=0x8c6492cau;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c6492cc; /* mov.b r3,@r4 */
L8c6492cc: *stop=0x8c6492ccu;r[4]+=1u;goto L8c6492ce; /* add #0x1,r4 */
L8c6492ce: *stop=0x8c6492ceu;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c6492d0; /* cmp/hs r6,r4 */
L8c6492d0: if(!(*sr&1u))goto L8c6492c8;goto L8c6492d2; /* bf 0x8c6492c8 */
L8c6492d2: *stop=0x8c6492d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649368u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6492d4; /* mov.l 0x8c649368,r3 */
L8c6492d4: *stop=0x8c6492d4u;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6492d6; /* mov.l @(0x2c,r14),r5 */
L8c6492d6: target=r[3];*pr=0x8c6492dau;*stop=0x8c6492d8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6492da; /* jsr @r3 */
L8c6492da: *stop=0x8c6492dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64936cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6492dc; /* mov.l 0x8c64936c,r2 */
L8c6492dc: target=r[2];*pr=0x8c6492e0u;*stop=0x8c6492deu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6492e0; /* jsr @r2 */
L8c6492e0: *stop=0x8c6492e0u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c6492e2; /* cmp/pz r0 */
L8c6492e2: take=!(*sr&1u);*stop=0x8c6492e4u;r[13]=r[0];if(take)goto L8c6492ee;goto L8c6492e6; /* bf/s 0x8c6492ee */
L8c6492e6: *stop=0x8c6492e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649370u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6492e8; /* mov.l 0x8c649370,r3 */
L8c6492e8: target=r[3];*pr=0x8c6492ecu;*stop=0x8c6492eau;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6492ec; /* jsr @r3 */
L8c6492ec: *stop=0x8c6492ecu;r[13]=0u;goto L8c6492ee; /* mov #0x0,r13 */
L8c6492ee: *stop=0x8c6492eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649374u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6492f0; /* mov.l 0x8c649374,r2 */
L8c6492f0: target=r[2];*pr=0x8c6492f4u;*stop=0x8c6492f2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6492f4; /* jsr @r2 */
L8c6492f4: *stop=0x8c6492f4u;r[0]=r[13];goto L8c6492f6; /* mov r13,r0 */
L8c6492f6: *stop=0x8c6492f6u;r[15]+=4u;goto L8c6492f8; /* add #0x4,r15 */
L8c6492f8: *stop=0x8c6492f8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6492fa; /* lds.l @r15+,PR */
L8c6492fa: *stop=0x8c6492fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6492fc; /* mov.l @r15+,r10 */
L8c6492fc: *stop=0x8c6492fcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6492fe; /* mov.l @r15+,r11 */
L8c6492fe: *stop=0x8c6492feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c649300; /* mov.l @r15+,r12 */
L8c649300: *stop=0x8c649300u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c649302; /* mov.l @r15+,r13 */
L8c649302: *stop=0x8c649302u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c649304u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_8c64919e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c6491d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6491d8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c6491f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6491f8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c649200(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649200u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64920a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64920au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c649240(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649240u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c64928a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64928au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c649296(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649296u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c6492a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6492a4u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c6492da(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6492dau);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c6492e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6492e0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c6492ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6492ecu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command11_resume_8c6492f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command11_run_8c64919e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6492f4u);}

SA_STAGE_EXPORT int sa_movie_filesystem_command11_8c64b2a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64b2a6;
L8c64b2a6: *stop=0x8c64b2a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b2f4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64b2a8; /* mov.l 0x8c64b2f4,r7 */
L8c64b2a8: *stop=0x8c64b2a8u;r[6]=1u;goto L8c64b2aa; /* mov #0x1,r6 */
L8c64b2aa: *stop=0x8c64b2aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b2f0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64b2ac; /* mov.l 0x8c64b2f0,r4 */
L8c64b2ac: *stop=0x8c64b2aeu;r[5]=0u;goto L8c64b2c2; /* bra 0x8c64b2c2 */
L8c64b2b0: *stop=0x8c64b2b0u;if(!valid_access(r[4],1u))return -3;r[2]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c64b2b2; /* mov.b @r4,r2 */
L8c64b2b2: *stop=0x8c64b2b2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64b2b4; /* tst r2,r2 */
L8c64b2b4: if(!(*sr&1u))goto L8c64b2be;goto L8c64b2b6; /* bf 0x8c64b2be */
L8c64b2b6: *stop=0x8c64b2b6u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[6],1);if(io_status)return io_status;goto L8c64b2b8; /* mov.b r6,@r4 */
L8c64b2b8: *stop=0x8c64b2b8u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[5],4);if(io_status)return io_status;goto L8c64b2ba; /* mov.l r5,@(0xc,r4) */
L8c64b2ba: *stop=0x8c64b2bau;if(*pr!=entry_pr)return -3;*stop=0x8c64b2bcu;r[0]=r[4];return 0; /* rts */
L8c64b2be: *stop=0x8c64b2beu;r[4]+=72u;goto L8c64b2c0; /* add #0x48,r4 */
L8c64b2c0: *stop=0x8c64b2c0u;r[5]+=1u;goto L8c64b2c2; /* add #0x1,r5 */
L8c64b2c2: *stop=0x8c64b2c2u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b2c4; /* mov.l @r7,r3 */
L8c64b2c4: *stop=0x8c64b2c4u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]);goto L8c64b2c6; /* cmp/ge r3,r5 */
L8c64b2c6: if(!(*sr&1u))goto L8c64b2b0;goto L8c64b2c8; /* bf 0x8c64b2b0 */
L8c64b2c8: *stop=0x8c64b2c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b2f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b2ca; /* mov.l 0x8c64b2f8,r2 */
L8c64b2ca: *stop=0x8c64b2cau;r[0]=0u;goto L8c64b2cc; /* mov #0x0,r0 */
L8c64b2cc: *stop=0x8c64b2ccu;r[1]=sx(m,bus,&io_status,0x8c64b2e2u,2);if(io_status)return io_status;goto L8c64b2ce; /* mov.w 0x8c64b2e2,r1 */
L8c64b2ce: *stop=0x8c64b2ceu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c64b2d0; /* mov.l r1,@r2 */
L8c64b2d0: *stop=0x8c64b2d0u;if(*pr!=entry_pr)return -3;*stop=0x8c64b2d2u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_filesystem_command11_8c64b2d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64b2d4;
L8c64b2d4: *stop=0x8c64b2d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b2fcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b2d6; /* mov.l 0x8c64b2fc,r3 */
L8c64b2d6: *stop=0x8c64b2d6u;r[6]=72u;goto L8c64b2d8; /* mov #0x48,r6 */
L8c64b2d8: target=r[3];*stop=0x8c64b2dau;r[5]=0u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}
