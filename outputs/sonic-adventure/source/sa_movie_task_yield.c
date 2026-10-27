/* Original task termination/yield wrappers and shared save/restore path.
 * Static C from byte-checked SH-4 instructions; complete explicit state.
 * Missing callees remain reached boundaries; no callback result is substituted.
 * Context and interrupt entry remain explicit platform state.
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
SA_STAGE_EXPORT int sa_movie_task_yield_8c668d56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c668d56;
L8c668d56: *stop=0x8c668d56u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668d8cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c668d58; /* mov.l 0x8c668d8c,r2 */
L8c668d58: *stop=0x8c668d58u;r[0]=sx(m,bus,&io_status,0x8c668d78u,2);if(io_status)return io_status;goto L8c668d5a; /* mov.w 0x8c668d78,r0 */
L8c668d5a: *stop=0x8c668d5au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668d5c; /* mov.l @r2,r3 */
L8c668d5c: *stop=0x8c668d5cu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c668d5e; /* mov.l @(r0,r3),r1 */
L8c668d5e: *stop=0x8c668d5eu;r[3]=3u;goto L8c668d60; /* mov #0x3,r3 */
L8c668d60: *stop=0x8c668d60u;if(!valid_access(r[1]+12u,4u))return -3;wr(m,bus,&io_status,r[1]+0xcu,r[3],4);if(io_status)return io_status;goto L8c668d62; /* mov.l r3,@(0xc,r1) */
L8c668d62: *stop=0x8c668d64u;r[4]=0u;if(!call)return -2;return call(ctx,0x8c668e1eu,m,f,sr,r,pr); /* bra 0x8c668e1e */
}

static int task_yield_run(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c668e48u)goto L8c668e48;goto L8c668e1e;
L8c668e1e: *stop=0x8c668e1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c668e20; /* mov.l r14,@-r15 */
L8c668e20: *stop=0x8c668e20u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c668e22; /* cmp/pz r4 */
L8c668e22: *stop=0x8c668e22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668ea4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c668e24; /* mov.l 0x8c668ea4,r14 */
L8c668e24: *stop=0x8c668e24u;r[0]=sx(m,bus,&io_status,0x8c668ea0u,2);if(io_status)return io_status;goto L8c668e26; /* mov.w 0x8c668ea0,r0 */
L8c668e26: *stop=0x8c668e26u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c668e28; /* sts.l PR,@-r15 */
L8c668e28: *stop=0x8c668e28u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668e2a; /* mov.l @r14,r3 */
L8c668e2a: *stop=0x8c668e2au;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668e2c; /* mov.l @(r0,r3),r2 */
L8c668e2c: if(!(*sr&1u))goto L8c668e38;goto L8c668e2e; /* bf 0x8c668e38 */
L8c668e2e: *stop=0x8c668e2eu;r[0]=sx(m,bus,&io_status,0x8c668e9eu,2);if(io_status)return io_status;goto L8c668e30; /* mov.w 0x8c668e9e,r0 */
L8c668e30: *stop=0x8c668e30u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c668e32; /* mov.l @r14,r1 */
L8c668e32: *stop=0x8c668e32u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c668e34; /* mov.l @(r0,r1),r1 */
L8c668e34: *stop=0x8c668e36u;r[1]+=r[4];goto L8c668e3a; /* bra 0x8c668e3a */
L8c668e38: *stop=0x8c668e38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668ea8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c668e3a; /* mov.l 0x8c668ea8,r1 */
L8c668e3a: *stop=0x8c668e3au;r[0]=sx(m,bus,&io_status,0x8c668ea0u,2);if(io_status)return io_status;goto L8c668e3c; /* mov.w 0x8c668ea0,r0 */
L8c668e3c: *stop=0x8c668e3cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668eacu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668e3e; /* mov.l 0x8c668eac,r3 */
L8c668e3e: *stop=0x8c668e3eu;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[1],4);if(io_status)return io_status;goto L8c668e40; /* mov.l r1,@(0x14,r2) */
L8c668e40: *stop=0x8c668e40u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c668e42; /* mov.l @r14,r4 */
L8c668e42: *stop=0x8c668e42u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c668e44; /* mov.l @(r0,r4),r4 */
L8c668e44: target=r[3];*pr=0x8c668e48u;*stop=0x8c668e46u;r[4]+=24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668e48; /* jsr @r3 */
L8c668e48: *stop=0x8c668e48u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c668e4a; /* tst r0,r0 */
L8c668e4a: if(!(*sr&1u))goto L8c668e56;goto L8c668e4c; /* bf 0x8c668e56 */
L8c668e4c: *stop=0x8c668e4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668eb0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668e4e; /* mov.l 0x8c668eb0,r3 */
L8c668e4e: *stop=0x8c668e4eu;r[5]=1u;goto L8c668e50; /* mov #0x1,r5 */
L8c668e50: *stop=0x8c668e50u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c668e52; /* mov.l @r14,r4 */
L8c668e52: target=r[3];*pr=0x8c668e56u;*stop=0x8c668e54u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668e56; /* jsr @r3 */
L8c668e56: *stop=0x8c668e56u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c668e58; /* lds.l @r15+,PR */
L8c668e58: *stop=0x8c668e58u;*stop=0x8c668e5au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(*pr==entry_pr)return 0;if(!call)return -2;return call(ctx,*pr,m,f,sr,r,pr); /* rts */
}

SA_STAGE_EXPORT int sa_movie_task_yield_8c668e1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return task_yield_run(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_task_yield_resume_8c668e48(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return task_yield_run(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c668e48u);}
SA_STAGE_EXPORT int sa_movie_task_yield_8c668e5c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c668e5c;
L8c668e5c: *stop=0x8c668e5eu;r[4]=1u;if(!call)return -2;return call(ctx,0x8c668e1eu,m,f,sr,r,pr); /* bra 0x8c668e1e */
}
