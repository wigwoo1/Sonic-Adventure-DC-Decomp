/* Original inverse-trigonometry wrappers and quadrant path.
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
static int sa_collision_angle_run_8c10d058(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c10d062u)goto L8c10d062;goto L8c10d058;
L8c10d058: *stop=0x8c10d058u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d05a; /* sts.l PR,@-r15 */
L8c10d05a: *stop=0x8c10d05au;r[15]+=4294967292u;goto L8c10d05c; /* add #-0x4,r15 */
L8c10d05c: *stop=0x8c10d05cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d098u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d05e; /* mov.l 0x8c10d098,r3 */
L8c10d05e: target=r[3];*pr=0x8c10d062u;*stop=0x8c10d060u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d062; /* jsr @r3 */
L8c10d062: *stop=0x8c10d062u;r[3]=r[15];goto L8c10d064; /* mov r15,r3 */
L8c10d064: *stop=0x8c10d064u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d09cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10d066; /* mov.l 0x8c10d09c,r4 */
L8c10d066: *stop=0x8c10d066u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c10d068; /* fmov.s fr0,@r15 */
L8c10d068: *stop=0x8c10d068u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d06a; /* mov.l @r3,r2 */
L8c10d06a: *stop=0x8c10d06au;r[2]&=r[4];goto L8c10d06c; /* and r4,r2 */
L8c10d06c: *stop=0x8c10d06cu;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c10d06e; /* cmp/eq r4,r2 */
L8c10d06e: if(!(*sr&1u))goto L8c10d088;goto L8c10d070; /* bf 0x8c10d088 */
L8c10d070: *stop=0x8c10d070u;r[2]=r[15];goto L8c10d072; /* mov r15,r2 */
L8c10d072: *stop=0x8c10d072u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d074; /* mov.l @r2,r1 */
L8c10d074: *stop=0x8c10d074u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d0a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d076; /* mov.l 0x8c10d0a0,r3 */
L8c10d076: *stop=0x8c10d076u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c10d078; /* tst r3,r1 */
L8c10d078: if((*sr&1u))goto L8c10d082;goto L8c10d07a; /* bt 0x8c10d082 */
L8c10d07a: *stop=0x8c10d07au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d0a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d07c; /* mov.l 0x8c10d0a4,r2 */
L8c10d07c: *stop=0x8c10d07cu;r[0]=sx(m,bus,&io_status,0x8c10d092u,2);if(io_status)return io_status;goto L8c10d07e; /* mov.w 0x8c10d092,r0 */
L8c10d07e: *stop=0x8c10d080u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c10d088; /* bra 0x8c10d088 */
L8c10d082: *stop=0x8c10d082u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d0a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d084; /* mov.l 0x8c10d0a4,r1 */
L8c10d084: *stop=0x8c10d084u;r[2]=sx(m,bus,&io_status,0x8c10d094u,2);if(io_status)return io_status;goto L8c10d086; /* mov.w 0x8c10d094,r2 */
L8c10d086: *stop=0x8c10d086u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c10d088; /* mov.l r2,@r1 */
L8c10d088: *stop=0x8c10d088u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c10d08a; /* fmov.s @r15,fr0 */
L8c10d08a: *stop=0x8c10d08au;r[15]+=4u;goto L8c10d08c; /* add #0x4,r15 */
L8c10d08c: *stop=0x8c10d08cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d08e; /* lds.l @r15+,PR */
L8c10d08e: *stop=0x8c10d08eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c10d090u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_collision_angle_8c10d058(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_angle_run_8c10d058(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_collision_angle_resume_8c10d062(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_angle_run_8c10d058(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c10d062u);}

static int sa_collision_angle_run_8c10d0f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c10d102u)goto L8c10d102;goto L8c10d0f8;
L8c10d0f8: *stop=0x8c10d0f8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d0fa; /* sts.l PR,@-r15 */
L8c10d0fa: *stop=0x8c10d0fau;r[15]+=4294967292u;goto L8c10d0fc; /* add #-0x4,r15 */
L8c10d0fc: *stop=0x8c10d0fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d138u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d0fe; /* mov.l 0x8c10d138,r3 */
L8c10d0fe: target=r[3];*pr=0x8c10d102u;*stop=0x8c10d100u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d102; /* jsr @r3 */
L8c10d102: *stop=0x8c10d102u;r[3]=r[15];goto L8c10d104; /* mov r15,r3 */
L8c10d104: *stop=0x8c10d104u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d13cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10d106; /* mov.l 0x8c10d13c,r4 */
L8c10d106: *stop=0x8c10d106u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c10d108; /* fmov.s fr0,@r15 */
L8c10d108: *stop=0x8c10d108u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d10a; /* mov.l @r3,r2 */
L8c10d10a: *stop=0x8c10d10au;r[2]&=r[4];goto L8c10d10c; /* and r4,r2 */
L8c10d10c: *stop=0x8c10d10cu;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c10d10e; /* cmp/eq r4,r2 */
L8c10d10e: if(!(*sr&1u))goto L8c10d128;goto L8c10d110; /* bf 0x8c10d128 */
L8c10d110: *stop=0x8c10d110u;r[2]=r[15];goto L8c10d112; /* mov r15,r2 */
L8c10d112: *stop=0x8c10d112u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d114; /* mov.l @r2,r1 */
L8c10d114: *stop=0x8c10d114u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d140u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d116; /* mov.l 0x8c10d140,r3 */
L8c10d116: *stop=0x8c10d116u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c10d118; /* tst r3,r1 */
L8c10d118: if((*sr&1u))goto L8c10d122;goto L8c10d11a; /* bt 0x8c10d122 */
L8c10d11a: *stop=0x8c10d11au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d144u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d11c; /* mov.l 0x8c10d144,r2 */
L8c10d11c: *stop=0x8c10d11cu;r[0]=sx(m,bus,&io_status,0x8c10d132u,2);if(io_status)return io_status;goto L8c10d11e; /* mov.w 0x8c10d132,r0 */
L8c10d11e: *stop=0x8c10d120u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c10d128; /* bra 0x8c10d128 */
L8c10d122: *stop=0x8c10d122u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d144u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d124; /* mov.l 0x8c10d144,r1 */
L8c10d124: *stop=0x8c10d124u;r[2]=sx(m,bus,&io_status,0x8c10d134u,2);if(io_status)return io_status;goto L8c10d126; /* mov.w 0x8c10d134,r2 */
L8c10d126: *stop=0x8c10d126u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c10d128; /* mov.l r2,@r1 */
L8c10d128: *stop=0x8c10d128u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c10d12a; /* fmov.s @r15,fr0 */
L8c10d12a: *stop=0x8c10d12au;r[15]+=4u;goto L8c10d12c; /* add #0x4,r15 */
L8c10d12c: *stop=0x8c10d12cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d12e; /* lds.l @r15+,PR */
L8c10d12e: *stop=0x8c10d12eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c10d130u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_collision_angle_8c10d0f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_angle_run_8c10d0f8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_collision_angle_resume_8c10d102(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_angle_run_8c10d0f8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c10d102u);}

static int sa_collision_angle_run_8c10e6a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c10e6b8u)goto L8c10e6b8;goto L8c10e6a0;
L8c10e6a0: *stop=0x8c10e6a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10e6a2; /* sts.l PR,@-r15 */
L8c10e6a2: *stop=0x8c10e6a2u;r[15]+=4294967288u;goto L8c10e6a4; /* add #-0x8,r15 */
L8c10e6a4: *stop=0x8c10e6a4u;r[3]=r[15];goto L8c10e6a6; /* mov r15,r3 */
L8c10e6a6: *stop=0x8c10e6a6u;r[3]+=4u;goto L8c10e6a8; /* add #0x4,r3 */
L8c10e6a8: *stop=0x8c10e6a8u;r[0]=4u;goto L8c10e6aa; /* mov #0x4,r0 */
L8c10e6aa: *stop=0x8c10e6aau;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;goto L8c10e6ac; /* fmov.s fr4,@r3 */
L8c10e6ac: *stop=0x8c10e6acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e6e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e6ae; /* mov.l 0x8c10e6e8,r3 */
L8c10e6ae: *stop=0x8c10e6aeu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c10e6b0; /* fmov.s fr5,@r15 */
L8c10e6b0: *stop=0x8c10e6b0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;goto L8c10e6b2; /* fmov.s @(r0,r15),fr4 */
L8c10e6b2: *stop=0x8c10e6b2u;fcopy(f,3u,5u);goto L8c10e6b4; /* fmov fr5,fr3 */
L8c10e6b4: target=r[3];*pr=0x8c10e6b8u;*stop=0x8c10e6b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e6b8; /* jsr @r3 */
L8c10e6b8: *stop=0x8c10e6b8u;r[2]=r[15];goto L8c10e6ba; /* mov r15,r2 */
L8c10e6ba: *stop=0x8c10e6bau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e6bc; /* mov.l @r2,r3 */
L8c10e6bc: *stop=0x8c10e6bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e6ecu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e6be; /* mov.l 0x8c10e6ec,r4 */
L8c10e6be: *stop=0x8c10e6beu;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c10e6c0; /* tst r4,r3 */
L8c10e6c0: take=(*sr&1u);*stop=0x8c10e6c2u;fcopy(f,4u,0u);if(take)goto L8c10e6de;goto L8c10e6c4; /* bt/s 0x8c10e6de */
L8c10e6c4: *stop=0x8c10e6c4u;r[1]=r[15];goto L8c10e6c6; /* mov r15,r1 */
L8c10e6c6: *stop=0x8c10e6c6u;r[1]+=4u;goto L8c10e6c8; /* add #0x4,r1 */
L8c10e6c8: *stop=0x8c10e6c8u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e6ca; /* mov.l @r1,r3 */
L8c10e6ca: *stop=0x8c10e6cau;*sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c10e6cc; /* tst r3,r4 */
L8c10e6cc: if((*sr&1u))goto L8c10e6d6;goto L8c10e6ce; /* bt 0x8c10e6d6 */
L8c10e6ce: *stop=0x8c10e6ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e6f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e6d0; /* mov.l 0x8c10e6f0,r2 */
L8c10e6d0: *stop=0x8c10e6d0u;fcopy(f,3u,4u);goto L8c10e6d2; /* fmov fr4,fr3 */
L8c10e6d2: *stop=0x8c10e6d4u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c10e6dc; /* bra 0x8c10e6dc */
L8c10e6d6: *stop=0x8c10e6d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e6f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e6d8; /* mov.l 0x8c10e6f4,r3 */
L8c10e6d8: *stop=0x8c10e6d8u;fcopy(f,3u,4u);goto L8c10e6da; /* fmov fr4,fr3 */
L8c10e6da: *stop=0x8c10e6dau;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;goto L8c10e6dc; /* fmov.s @r3,fr4 */
L8c10e6dc: *stop=0x8c10e6dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c10e6de; /* fadd fr3,fr4 */
L8c10e6de: *stop=0x8c10e6deu;r[15]+=8u;goto L8c10e6e0; /* add #0x8,r15 */
L8c10e6e0: *stop=0x8c10e6e0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10e6e2; /* lds.l @r15+,PR */
L8c10e6e2: *stop=0x8c10e6e2u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c10e6e4u;fcopy(f,0u,4u);if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_collision_angle_8c10e6a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_angle_run_8c10e6a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_collision_angle_resume_8c10e6b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_angle_run_8c10e6a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c10e6b8u);}

SA_STAGE_EXPORT int sa_collision_angle_8c10fb94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10fb94;
L8c10fb94: *stop=0x8c10fb94u;fcopy(f,6u,4u);goto L8c10fb96; /* fmov fr4,fr6 */
L8c10fb96: *stop=0x8c10fb96u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[4],&f->fpscr,&fr[6]);if(status)return status;}goto L8c10fb98; /* fmul fr4,fr6 */
L8c10fb98: *stop=0x8c10fb98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fbb4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10fb9a; /* mov.l 0x8c10fbb4,r5 */
L8c10fb9a: *stop=0x8c10fb9au;r[4]=5u;goto L8c10fb9c; /* mov #0x5,r4 */
L8c10fb9c: *stop=0x8c10fb9cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c10fb9e; /* fmov.s @r5+,fr5 */
L8c10fb9e: *stop=0x8c10fb9eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c10fba0; /* fmov.s @r5+,fr3 */
L8c10fba0: *stop=0x8c10fba0u;r[4]+=4294967295u;goto L8c10fba2; /* add #-0x1,r4 */
L8c10fba2: *stop=0x8c10fba2u;fcopy(f,0u,6u);goto L8c10fba4; /* fmov fr6,fr0 */
L8c10fba4: *stop=0x8c10fba4u;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c10fba6; /* cmp/pl r4 */
L8c10fba6: *stop=0x8c10fba6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[5],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c10fba8; /* fmac fr0,fr5,fr3 */
L8c10fba8: take=(*sr&1u);*stop=0x8c10fbaau;fcopy(f,5u,3u);if(take)goto L8c10fb9e;goto L8c10fbac; /* bt/s 0x8c10fb9e */
L8c10fbac: *stop=0x8c10fbacu;fcopy(f,0u,4u);goto L8c10fbae; /* fmov fr4,fr0 */
L8c10fbae: *stop=0x8c10fbaeu;fcopy(f,6u,5u);goto L8c10fbb0; /* fmov fr5,fr6 */
L8c10fbb0: *stop=0x8c10fbb0u;if(*pr!=entry_pr)return -3;*stop=0x8c10fbb2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_collision_angle_8c10fbb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10fbb8;
L8c10fbb8: *stop=0x8c10fbb8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10fbba; /* mov.l r14,@-r15 */
L8c10fbba: *stop=0x8c10fbbau;r[15]+=4294967268u;goto L8c10fbbc; /* add #-0x1c,r15 */
L8c10fbbc: *stop=0x8c10fbbcu;r[6]=r[15];goto L8c10fbbe; /* mov r15,r6 */
L8c10fbbe: *stop=0x8c10fbbeu;r[6]+=24u;goto L8c10fbc0; /* add #0x18,r6 */
L8c10fbc0: *stop=0x8c10fbc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fc9cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10fbc2; /* mov.l 0x8c10fc9c,r5 */
L8c10fbc2: *stop=0x8c10fbc2u;r[14]=r[15];goto L8c10fbc4; /* mov r15,r14 */
L8c10fbc4: *stop=0x8c10fbc4u;r[14]+=20u;goto L8c10fbc6; /* add #0x14,r14 */
L8c10fbc6: *stop=0x8c10fbc6u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[6]);if(io_status)return io_status;goto L8c10fbc8; /* fmov.s fr4,@r6 */
L8c10fbc8: *stop=0x8c10fbc8u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c10fbca; /* fmov.s fr5,@r14 */
L8c10fbca: *stop=0x8c10fbcau;r[7]=r[15];goto L8c10fbcc; /* mov r15,r7 */
L8c10fbcc: *stop=0x8c10fbccu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fbce; /* mov.l @r6,r3 */
L8c10fbce: *stop=0x8c10fbceu;r[3]&=r[5];goto L8c10fbd0; /* and r5,r3 */
L8c10fbd0: *stop=0x8c10fbd0u;*sr=(*sr&~1u)|(r[5]==r[3]);goto L8c10fbd2; /* cmp/eq r5,r3 */
L8c10fbd2: take=(*sr&1u);*stop=0x8c10fbd4u;r[7]+=16u;if(take)goto L8c10fbf0;goto L8c10fbd6; /* bt/s 0x8c10fbf0 */
L8c10fbd6: *stop=0x8c10fbd6u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fbd8; /* mov.l @(0x14,r15),r3 */
L8c10fbd8: *stop=0x8c10fbd8u;r[3]&=r[5];goto L8c10fbda; /* and r5,r3 */
L8c10fbda: *stop=0x8c10fbdau;*sr=(*sr&~1u)|(r[5]==r[3]);goto L8c10fbdc; /* cmp/eq r5,r3 */
L8c10fbdc: if(!(*sr&1u))goto L8c10fbe6;goto L8c10fbde; /* bf 0x8c10fbe6 */
L8c10fbde: *stop=0x8c10fbdeu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fbe0; /* mov.l @(0x14,r15),r2 */
L8c10fbe0: *stop=0x8c10fbe0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fca0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fbe2; /* mov.l 0x8c10fca0,r3 */
L8c10fbe2: *stop=0x8c10fbe2u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c10fbe4; /* tst r3,r2 */
L8c10fbe4: if(!(*sr&1u))goto L8c10fbf0;goto L8c10fbe6; /* bf 0x8c10fbf0 */
L8c10fbe6: *stop=0x8c10fbe6u;r[0]=20u;goto L8c10fbe8; /* mov #0x14,r0 */
L8c10fbe8: *stop=0x8c10fbe8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c10fbea; /* fldi0 fr3 */
L8c10fbea: *stop=0x8c10fbeau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c10fbec; /* fmov.s @(r0,r15),fr2 */
L8c10fbec: *stop=0x8c10fbecu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c10fbee; /* fcmp/eq fr3,fr2 */
L8c10fbee: if(!(*sr&1u))goto L8c10fbfc;goto L8c10fbf0; /* bf 0x8c10fbfc */
L8c10fbf0: *stop=0x8c10fbf0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fca4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fbf2; /* mov.l 0x8c10fca4,r3 */
L8c10fbf2: *stop=0x8c10fbf2u;r[2]=0u;goto L8c10fbf4; /* mov #0x0,r2 */
L8c10fbf4: *stop=0x8c10fbf4u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c10fbf6; /* mov.l r2,@r4 */
L8c10fbf6: *stop=0x8c10fbf6u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c10fbf8; /* fmov.s @r3,fr3 */
L8c10fbf8: *stop=0x8c10fbfau;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c10fc94; /* bra 0x8c10fc94 */
L8c10fbfc: *stop=0x8c10fbfcu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10fbfe; /* mov.l @r14,r1 */
L8c10fbfe: *stop=0x8c10fbfeu;r[1]&=r[5];goto L8c10fc00; /* and r5,r1 */
L8c10fc00: *stop=0x8c10fc00u;*sr=(*sr&~1u)|(r[5]==r[1]);goto L8c10fc02; /* cmp/eq r5,r1 */
L8c10fc02: if(!(*sr&1u))goto L8c10fc0c;goto L8c10fc04; /* bf 0x8c10fc0c */
L8c10fc04: *stop=0x8c10fc04u;r[2]=0u;goto L8c10fc06; /* mov #0x0,r2 */
L8c10fc06: *stop=0x8c10fc06u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c10fc08; /* mov.l r2,@r4 */
L8c10fc08: *stop=0x8c10fc0au;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[7]);if(io_status)return io_status;goto L8c10fc94; /* bra 0x8c10fc94 */
L8c10fc0c: *stop=0x8c10fc0cu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fc0e; /* mov.l @r6,r2 */
L8c10fc0e: *stop=0x8c10fc0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fca8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fc10; /* mov.l 0x8c10fca8,r3 */
L8c10fc10: *stop=0x8c10fc10u;r[2]&=r[3];goto L8c10fc12; /* and r3,r2 */
L8c10fc12: *stop=0x8c10fc12u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c10fc14; /* mov.l r2,@(0x8,r15) */
L8c10fc14: *stop=0x8c10fc14u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10fc16; /* mov.l @r6,r1 */
L8c10fc16: *stop=0x8c10fc16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fcacu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fc18; /* mov.l 0x8c10fcac,r2 */
L8c10fc18: *stop=0x8c10fc18u;r[1]&=r[2];goto L8c10fc1a; /* and r2,r1 */
L8c10fc1a: *stop=0x8c10fc1au;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[1],4);if(io_status)return io_status;goto L8c10fc1c; /* mov.l r1,@r6 */
L8c10fc1c: *stop=0x8c10fc1cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10fc1e; /* mov.l @r14,r0 */
L8c10fc1e: *stop=0x8c10fc1eu;r[0]&=r[2];goto L8c10fc20; /* and r2,r0 */
L8c10fc20: *stop=0x8c10fc20u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c10fc22; /* mov.l r0,@r14 */
L8c10fc22: *stop=0x8c10fc22u;r[0]=12u;goto L8c10fc24; /* mov #0xc,r0 */
L8c10fc24: *stop=0x8c10fc24u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c10fc26; /* fmov.s @r14,fr3 */
L8c10fc26: *stop=0x8c10fc26u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[6]);if(io_status)return io_status;goto L8c10fc28; /* fmov.s @r6,fr4 */
L8c10fc28: *stop=0x8c10fc28u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c10fc2a; /* fdiv fr3,fr4 */
L8c10fc2a: *stop=0x8c10fc2au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;goto L8c10fc2c; /* fmov fr4,@(r0,r15) */
L8c10fc2c: *stop=0x8c10fc2cu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10fc2e; /* mov.l @(0xc,r15),r1 */
L8c10fc2e: *stop=0x8c10fc2eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c10fc30; /* mov.l r1,@r15 */
L8c10fc30: *stop=0x8c10fc30u;r[1]&=r[3];goto L8c10fc32; /* and r3,r1 */
L8c10fc32: *stop=0x8c10fc32u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c10fc34; /* mov.l r1,@(0x4,r15) */
L8c10fc34: *stop=0x8c10fc34u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10fc36; /* mov.l @r15,r1 */
L8c10fc36: *stop=0x8c10fc36u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fcb0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fc38; /* mov.l 0x8c10fcb0,r3 */
L8c10fc38: *stop=0x8c10fc38u;r[5]&=r[1];goto L8c10fc3a; /* and r1,r5 */
L8c10fc3a: *stop=0x8c10fc3au;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]);goto L8c10fc3c; /* cmp/ge r3,r5 */
L8c10fc3c: if((*sr&1u))goto L8c10fc4a;goto L8c10fc3e; /* bt 0x8c10fc4a */
L8c10fc3e: *stop=0x8c10fc3eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,fpul);if(status)return status;goto L8c10fc40; /* ftrc fr4,FPUL */
L8c10fc40: *stop=0x8c10fc40u;r[0]=*fpul;goto L8c10fc42; /* sts FPUL,r0 */
L8c10fc42: *stop=0x8c10fc42u;*fpul=r[0];goto L8c10fc44; /* lds r0,FPUL */
L8c10fc44: *stop=0x8c10fc44u;r[0]=12u;goto L8c10fc46; /* mov #0xc,r0 */
L8c10fc46: *stop=0x8c10fc46u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c10fc48; /* float FPUL,fr3 */
L8c10fc48: *stop=0x8c10fc48u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c10fc4a; /* fmov fr3,@(r0,r15) */
L8c10fc4a: *stop=0x8c10fc4au;r[0]=12u;goto L8c10fc4c; /* mov #0xc,r0 */
L8c10fc4c: *stop=0x8c10fc4cu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fc4e; /* mov.l @(0xc,r15),r2 */
L8c10fc4e: *stop=0x8c10fc4eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fc50; /* mov.l @(0x4,r15),r3 */
L8c10fc50: *stop=0x8c10fc50u;r[2]|=r[3];goto L8c10fc52; /* or r3,r2 */
L8c10fc52: *stop=0x8c10fc52u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c10fc54; /* mov.l r2,@(0xc,r15) */
L8c10fc54: *stop=0x8c10fc54u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[15]);if(io_status)return io_status;goto L8c10fc56; /* fmov.s @(r0,r15),fr0 */
L8c10fc56: *stop=0x8c10fc56u;r[0]=0x8c10fcb4u;goto L8c10fc58; /* mova 0x8c10fcb4,r0 */
L8c10fc58: *stop=0x8c10fc58u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c10fc5a; /* fmov.s @r0,fr2 */
L8c10fc5a: *stop=0x8c10fc5au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c10fc5c; /* fcmp/gt fr2,fr0 */
L8c10fc5c: if(!(*sr&1u))goto L8c10fc68;goto L8c10fc5e; /* bf 0x8c10fc68 */
L8c10fc5e: *stop=0x8c10fc5eu;r[0]=0x8c10fcb8u;goto L8c10fc60; /* mova 0x8c10fcb8,r0 */
L8c10fc60: *stop=0x8c10fc60u;fcopy(f,3u,0u);goto L8c10fc62; /* fmov fr0,fr3 */
L8c10fc62: *stop=0x8c10fc62u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c10fc64; /* fmov.s @r0,fr1 */
L8c10fc64: *stop=0x8c10fc66u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c10fc6a; /* bra 0x8c10fc6a */
L8c10fc68: *stop=0x8c10fc68u;fcopy(f,3u,0u);goto L8c10fc6a; /* fmov fr0,fr3 */
L8c10fc6a: *stop=0x8c10fc6au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c10fc6c; /* ftrc fr3,FPUL */
L8c10fc6c: *stop=0x8c10fc6cu;r[2]=*fpul;goto L8c10fc6e; /* sts FPUL,r2 */
L8c10fc6e: *stop=0x8c10fc6eu;r[3]=r[2];goto L8c10fc70; /* mov r2,r3 */
L8c10fc70: *stop=0x8c10fc70u;*fpul=r[3];goto L8c10fc72; /* lds r3,FPUL */
L8c10fc72: *stop=0x8c10fc72u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c10fc74; /* cmp/pz r3 */
L8c10fc74: *stop=0x8c10fc74u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c10fc76; /* mov.l r2,@r4 */
L8c10fc76: take=(*sr&1u);*stop=0x8c10fc78u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c10fc82;goto L8c10fc7a; /* bt/s 0x8c10fc82 */
L8c10fc7a: *stop=0x8c10fc7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fcb0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10fc7c; /* mov.l 0x8c10fcb0,r1 */
L8c10fc7c: *stop=0x8c10fc7cu;*fpul=r[1];goto L8c10fc7e; /* lds r1,FPUL */
L8c10fc7e: *stop=0x8c10fc7eu;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c10fc80; /* fsts FPUL,fr2 */
L8c10fc80: *stop=0x8c10fc80u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c10fc82; /* fadd fr2,fr3 */
L8c10fc82: *stop=0x8c10fc82u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c10fc84; /* fmov.s @r14,fr2 */
L8c10fc84: *stop=0x8c10fc84u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[6]);if(io_status)return io_status;goto L8c10fc86; /* fmov.s @r6,fr1 */
L8c10fc86: *stop=0x8c10fc86u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c10fc88; /* fmul fr2,fr3 */
L8c10fc88: *stop=0x8c10fc88u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c10fc8a; /* fsub fr3,fr1 */
L8c10fc8a: *stop=0x8c10fc8au;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[7]);if(io_status)return io_status;goto L8c10fc8c; /* fmov.s fr1,@r7 */
L8c10fc8c: *stop=0x8c10fc8cu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fc8e; /* mov.l @r7,r2 */
L8c10fc8e: *stop=0x8c10fc8eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fc90; /* mov.l @(0x8,r15),r3 */
L8c10fc90: *stop=0x8c10fc90u;r[2]|=r[3];goto L8c10fc92; /* or r3,r2 */
L8c10fc92: *stop=0x8c10fc92u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c10fc94; /* mov.l r2,@r7 */
L8c10fc94: *stop=0x8c10fc94u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c10fc96; /* fmov.s @r7,fr0 */
L8c10fc96: *stop=0x8c10fc96u;r[15]+=28u;goto L8c10fc98; /* add #0x1c,r15 */
L8c10fc98: *stop=0x8c10fc98u;if(*pr!=entry_pr)return -3;*stop=0x8c10fc9au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
