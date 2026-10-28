/* Original collision world and debug initializers plus five canonical leaf adapters.
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
static int sa_collision_setup_run_8c028e60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c028eaau)goto L8c028eaa;if(resume_pc==0x8c028eb4u)goto L8c028eb4;goto L8c028e60;
L8c028e60: *stop=0x8c028e60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028f08u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c028e62; /* mov.l 0x8c028f08,r3 */
L8c028e62: *stop=0x8c028e62u;r[4]=0u;goto L8c028e64; /* mov #0x0,r4 */
L8c028e64: *stop=0x8c028e64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028f10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c028e66; /* mov.l 0x8c028f10,r1 */
L8c028e66: *stop=0x8c028e66u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c028e68; /* sts.l PR,@-r15 */
L8c028e68: *stop=0x8c028e68u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c028e6a; /* mov.l r4,@r3 */
L8c028e6a: *stop=0x8c028e6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c028f14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c028e6c; /* mov.l 0x8c028f14,r3 */
L8c028e6c: *stop=0x8c028e6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c028f0cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c028e6e; /* mov.l 0x8c028f0c,r2 */
L8c028e6e: *stop=0x8c028e6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c028f18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c028e70; /* mov.l 0x8c028f18,r5 */
L8c028e70: *stop=0x8c028e70u;r[6]=sx(m,bus,&io_status,0x8c028f00u,2);if(io_status)return io_status;goto L8c028e72; /* mov.w 0x8c028f00,r6 */
L8c028e72: *stop=0x8c028e72u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c028e74; /* mov.l r4,@r2 */
L8c028e74: *stop=0x8c028e74u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c028e76; /* mov.l r4,@r1 */
L8c028e76: *stop=0x8c028e76u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c028e78; /* mov.l r4,@r3 */
L8c028e78: *stop=0x8c028e78u;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[4],4);if(io_status)return io_status;goto L8c028e7a; /* mov.l r4,@(0x8,r5) */
L8c028e7a: *stop=0x8c028e7au;r[5]+=64u;goto L8c028e7c; /* add #0x40,r5 */
L8c028e7c: *stop=0x8c028e7cu;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[4],4);if(io_status)return io_status;goto L8c028e7e; /* mov.l r4,@(0x8,r5) */
L8c028e7e: *stop=0x8c028e7eu;r[6]+=4294967293u;goto L8c028e80; /* add #-0x3,r6 */
L8c028e80: *stop=0x8c028e80u;r[5]+=64u;goto L8c028e82; /* add #0x40,r5 */
L8c028e82: *stop=0x8c028e82u;*sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c028e84; /* cmp/pl r6 */
L8c028e84: *stop=0x8c028e84u;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[4],4);if(io_status)return io_status;goto L8c028e86; /* mov.l r4,@(0x8,r5) */
L8c028e86: take=(*sr&1u);*stop=0x8c028e88u;r[5]+=64u;if(take)goto L8c028e78;goto L8c028e8a; /* bt/s 0x8c028e78 */
L8c028e8a: *stop=0x8c028e8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c028f1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c028e8c; /* mov.l 0x8c028f1c,r4 */
L8c028e8c: *stop=0x8c028e8cu;r[5]=sx(m,bus,&io_status,0x8c028f02u,2);if(io_status)return io_status;goto L8c028e8e; /* mov.w 0x8c028f02,r5 */
L8c028e8e: *stop=0x8c028e8eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c028e90; /* fldi0 fr4 */
L8c028e90: *stop=0x8c028e90u;r[0]=20u;goto L8c028e92; /* mov #0x14,r0 */
L8c028e92: *stop=0x8c028e92u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c028e94; /* fmov fr4,@(r0,r4) */
L8c028e94: *stop=0x8c028e94u;r[5]+=4294967294u;goto L8c028e96; /* add #-0x2,r5 */
L8c028e96: *stop=0x8c028e96u;r[4]+=64u;goto L8c028e98; /* add #0x40,r4 */
L8c028e98: *stop=0x8c028e98u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c028e9a; /* cmp/pl r5 */
L8c028e9a: *stop=0x8c028e9au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c028e9c; /* fmov fr4,@(r0,r4) */
L8c028e9c: take=(*sr&1u);*stop=0x8c028e9eu;r[4]+=64u;if(take)goto L8c028e90;goto L8c028ea0; /* bt/s 0x8c028e90 */
L8c028ea0: *stop=0x8c028ea0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028f20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c028ea2; /* mov.l 0x8c028f20,r2 */
L8c028ea2: *stop=0x8c028ea2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028f1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c028ea4; /* mov.l 0x8c028f1c,r4 */
L8c028ea4: *stop=0x8c028ea4u;r[5]=sx(m,bus,&io_status,0x8c028f02u,2);if(io_status)return io_status;goto L8c028ea6; /* mov.w 0x8c028f02,r5 */
L8c028ea6: target=r[2];*pr=0x8c028eaau;*stop=0x8c028ea8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c028eaa; /* jsr @r2 */
L8c028eaa: *stop=0x8c028eaau;{uint32_t loaded=rd(m,bus,&io_status,0x8c028f24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c028eac; /* mov.l 0x8c028f24,r3 */
L8c028eac: *stop=0x8c028eacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c028f18u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c028eae; /* mov.l 0x8c028f18,r4 */
L8c028eae: *stop=0x8c028eaeu;r[5]=sx(m,bus,&io_status,0x8c028f00u,2);if(io_status)return io_status;goto L8c028eb0; /* mov.w 0x8c028f00,r5 */
L8c028eb0: target=r[3];*pr=0x8c028eb4u;*stop=0x8c028eb2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c028eb4; /* jsr @r3 */
L8c028eb4: *stop=0x8c028eb4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c028eb6; /* lds.l @r15+,PR */
L8c028eb6: *stop=0x8c028eb6u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c028eb8u;r[0]=1u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_collision_setup_8c028e60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_setup_run_8c028e60(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_collision_setup_resume_8c028eaa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_setup_run_8c028e60(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c028eaau);}
SA_STAGE_EXPORT int sa_collision_setup_resume_8c028eb4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_setup_run_8c028e60(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c028eb4u);}

static int sa_collision_setup_run_8c04e3de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c04e3f2u)goto L8c04e3f2;if(resume_pc==0x8c04e412u)goto L8c04e412;if(resume_pc==0x8c04e47au)goto L8c04e47a;if(resume_pc==0x8c04e480u)goto L8c04e480;if(resume_pc==0x8c04e486u)goto L8c04e486;goto L8c04e3de;
L8c04e3de: *stop=0x8c04e3deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e440u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c04e3e0; /* mov.l 0x8c04e440,r3 */
L8c04e3e0: *stop=0x8c04e3e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c04e3e2; /* mov.l r14,@-r15 */
L8c04e3e2: *stop=0x8c04e3e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c04e3e4; /* sts.l PR,@-r15 */
L8c04e3e4: *stop=0x8c04e3e4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c04e3e6; /* mov.l @r3,r0 */
L8c04e3e6: *stop=0x8c04e3e6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c04e3e8; /* cmp/eq #0x1,r0 */
L8c04e3e8: if(!(*sr&1u))goto L8c04e3f2;goto L8c04e3ea; /* bf 0x8c04e3f2 */
L8c04e3ea: *stop=0x8c04e3eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e448u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c04e3ec; /* mov.l 0x8c04e448,r1 */
L8c04e3ec: *stop=0x8c04e3ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e444u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c04e3ee; /* mov.l 0x8c04e444,r4 */
L8c04e3ee: target=r[1];*pr=0x8c04e3f2u;*stop=0x8c04e3f0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e3f2; /* jsr @r1 */
L8c04e3f2: *stop=0x8c04e3f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e450u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c04e3f4; /* mov.l 0x8c04e450,r1 */
L8c04e3f4: *stop=0x8c04e3f4u;r[2]=4294967295u;goto L8c04e3f6; /* mov #-0x1,r2 */
L8c04e3f6: *stop=0x8c04e3f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e458u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c04e3f8; /* mov.l 0x8c04e458,r0 */
L8c04e3f8: *stop=0x8c04e3f8u;r[14]=0u;goto L8c04e3fa; /* mov #0x0,r14 */
L8c04e3fa: *stop=0x8c04e3fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e44cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c04e3fc; /* mov.l 0x8c04e44c,r3 */
L8c04e3fc: *stop=0x8c04e3fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e454u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c04e3fe; /* mov.l 0x8c04e454,r4 */
L8c04e3fe: *stop=0x8c04e3feu;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[14],2);if(io_status)return io_status;goto L8c04e400; /* mov.w r14,@r3 */
L8c04e400: *stop=0x8c04e400u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[2],1);if(io_status)return io_status;goto L8c04e402; /* mov.b r2,@r1 */
L8c04e402: *stop=0x8c04e402u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c04e404; /* mov.l @r0,r2 */
L8c04e404: *stop=0x8c04e404u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c04e406; /* tst r2,r2 */
L8c04e406: if((*sr&1u))goto L8c04e41c;goto L8c04e408; /* bt 0x8c04e41c */
L8c04e408: *stop=0x8c04e408u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e448u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c04e40a; /* mov.l 0x8c04e448,r3 */
L8c04e40a: *stop=0x8c04e40au;r[2]=1u;goto L8c04e40c; /* mov #0x1,r2 */
L8c04e40c: *stop=0x8c04e40cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c04e40e; /* mov.l r2,@r4 */
L8c04e40e: target=r[3];*pr=0x8c04e412u;*stop=0x8c04e410u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e412; /* jsr @r3 */
L8c04e412: *stop=0x8c04e412u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e458u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c04e414; /* mov.l 0x8c04e458,r2 */
L8c04e414: *stop=0x8c04e414u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e45cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c04e416; /* mov.l 0x8c04e45c,r4 */
L8c04e416: *stop=0x8c04e416u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c04e418; /* mov.l @r2,r0 */
L8c04e418: *stop=0x8c04e418u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c04e41a; /* cmp/eq #0x1,r0 */
L8c04e41a: if(!(*sr&1u))goto L8c04e460;goto L8c04e41c; /* bf 0x8c04e460 */
L8c04e41c: *stop=0x8c04e41eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c04e464; /* bra 0x8c04e464 */
L8c04e460: *stop=0x8c04e460u;r[3]=1u;goto L8c04e462; /* mov #0x1,r3 */
L8c04e462: *stop=0x8c04e462u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c04e464; /* mov.l r3,@r4 */
L8c04e464: *stop=0x8c04e464u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e5a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c04e466; /* mov.l 0x8c04e5a4,r1 */
L8c04e466: *stop=0x8c04e466u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e5a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c04e468; /* mov.l 0x8c04e5a0,r2 */
L8c04e468: *stop=0x8c04e468u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e5b0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c04e46a; /* mov.l 0x8c04e5b0,r0 */
L8c04e46a: *stop=0x8c04e46au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c04e46c; /* mov.l r2,@r1 */
L8c04e46c: *stop=0x8c04e46cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e5acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c04e46e; /* mov.l 0x8c04e5ac,r2 */
L8c04e46e: *stop=0x8c04e46eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e5a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c04e470; /* mov.l 0x8c04e5a8,r3 */
L8c04e470: *stop=0x8c04e470u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c04e472; /* mov.l r14,@r3 */
L8c04e472: *stop=0x8c04e472u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c04e474; /* mov.l r2,@r0 */
L8c04e474: *stop=0x8c04e474u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e5b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c04e476; /* mov.l 0x8c04e5b4,r2 */
L8c04e476: target=r[2];*pr=0x8c04e47au;*stop=0x8c04e478u;r[4]=3u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e47a; /* jsr @r2 */
L8c04e47a: *stop=0x8c04e47au;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e5b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c04e47c; /* mov.l 0x8c04e5b8,r3 */
L8c04e47c: target=r[3];*pr=0x8c04e480u;*stop=0x8c04e47eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e480; /* jsr @r3 */
L8c04e480: *stop=0x8c04e480u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e5bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c04e482; /* mov.l 0x8c04e5bc,r2 */
L8c04e482: target=r[2];*pr=0x8c04e486u;*stop=0x8c04e484u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04e486; /* jsr @r2 */
L8c04e486: *stop=0x8c04e486u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04e5c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c04e488; /* mov.l 0x8c04e5c0,r3 */
L8c04e488: *stop=0x8c04e488u;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[14],1);if(io_status)return io_status;goto L8c04e48a; /* mov.b r14,@r3 */
L8c04e48a: *stop=0x8c04e48au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c04e48c; /* lds.l @r15+,PR */
L8c04e48c: *stop=0x8c04e48cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c04e48eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_collision_setup_8c04e3de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_setup_run_8c04e3de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_collision_setup_resume_8c04e3f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_setup_run_8c04e3de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c04e3f2u);}
SA_STAGE_EXPORT int sa_collision_setup_resume_8c04e412(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_setup_run_8c04e3de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c04e412u);}
SA_STAGE_EXPORT int sa_collision_setup_resume_8c04e47a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_setup_run_8c04e3de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c04e47au);}
SA_STAGE_EXPORT int sa_collision_setup_resume_8c04e480(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_setup_run_8c04e3de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c04e480u);}
SA_STAGE_EXPORT int sa_collision_setup_resume_8c04e486(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_setup_run_8c04e3de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c04e486u);}

SA_STAGE_EXPORT int sa_collision_setup_8c02ce80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c02ce80;
L8c02ce80: *stop=0x8c02ce80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ce9cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02ce82; /* mov.l 0x8c02ce9c,r0 */
L8c02ce82: *stop=0x8c02ce82u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;goto L8c02ce84; /* mov.l r4,@r0 */
L8c02ce84: *stop=0x8c02ce84u;r[1]=r[4];goto L8c02ce86; /* mov r4,r1 */
L8c02ce86: *stop=0x8c02ce86u;r[1]+=64u;goto L8c02ce88; /* add #0x40,r1 */
L8c02ce88: *stop=0x8c02ce88u;r[5]+=4294967295u;goto L8c02ce8a; /* add #-0x1,r5 */
L8c02ce8a: *stop=0x8c02ce8au;if(!valid_access(r[4]+0u,4u))return -3;wr(m,bus,&io_status,r[4]+0x0u,r[1],4);if(io_status)return io_status;goto L8c02ce8c; /* mov.l r1,@(0x0,r4) */
L8c02ce8c: *stop=0x8c02ce8cu;r[4]+=64u;goto L8c02ce8e; /* add #0x40,r4 */
L8c02ce8e: *stop=0x8c02ce8eu;r[1]+=64u;goto L8c02ce90; /* add #0x40,r1 */
L8c02ce90: *stop=0x8c02ce90u;r[5]--;*sr=(*sr&~1u)|(!r[5]);goto L8c02ce92; /* dt r5 */
L8c02ce92: take=!(*sr&1u);*stop=0x8c02ce94u;r[0]=0u;if(take)goto L8c02ce8a;goto L8c02ce96; /* bf/s 0x8c02ce8a */
L8c02ce96: *stop=0x8c02ce96u;if(!valid_access(r[4]+0u,4u))return -3;wr(m,bus,&io_status,r[4]+0x0u,r[0],4);if(io_status)return io_status;goto L8c02ce98; /* mov.l r0,@(0x0,r4) */
L8c02ce98: *stop=0x8c02ce98u;if(*pr!=entry_pr)return -3;*stop=0x8c02ce9au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_collision_setup_8c02ceca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c02ceca;
L8c02ceca: *stop=0x8c02cecau;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ceecu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02cecc; /* mov.l 0x8c02ceec,r0 */
L8c02cecc: *stop=0x8c02ceccu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;goto L8c02cece; /* mov.l r4,@r0 */
L8c02cece: *stop=0x8c02ceceu;r[1]=r[4];goto L8c02ced0; /* mov r4,r1 */
L8c02ced0: *stop=0x8c02ced0u;r[1]+=64u;goto L8c02ced2; /* add #0x40,r1 */
L8c02ced2: *stop=0x8c02ced2u;r[5]+=4294967295u;goto L8c02ced4; /* add #-0x1,r5 */
L8c02ced4: *stop=0x8c02ced4u;if(!valid_access(r[4]+0u,4u))return -3;wr(m,bus,&io_status,r[4]+0x0u,r[1],4);if(io_status)return io_status;goto L8c02ced6; /* mov.l r1,@(0x0,r4) */
L8c02ced6: *stop=0x8c02ced6u;r[4]+=64u;goto L8c02ced8; /* add #0x40,r4 */
L8c02ced8: *stop=0x8c02ced8u;r[1]+=64u;goto L8c02ceda; /* add #0x40,r1 */
L8c02ceda: *stop=0x8c02cedau;r[5]--;*sr=(*sr&~1u)|(!r[5]);goto L8c02cedc; /* dt r5 */
L8c02cedc: take=!(*sr&1u);*stop=0x8c02cedeu;r[0]=0u;if(take)goto L8c02ced4;goto L8c02cee0; /* bf/s 0x8c02ced4 */
L8c02cee0: *stop=0x8c02cee0u;if(!valid_access(r[4]+0u,4u))return -3;wr(m,bus,&io_status,r[4]+0x0u,r[0],4);if(io_status)return io_status;goto L8c02cee2; /* mov.l r0,@(0x0,r4) */
L8c02cee2: *stop=0x8c02cee2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02cef0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c02cee4; /* mov.l 0x8c02cef0,r4 */
L8c02cee4: *stop=0x8c02cee4u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c02cee6; /* mov.l r0,@r4 */
L8c02cee6: *stop=0x8c02cee6u;if(*pr!=entry_pr)return -3;*stop=0x8c02cee8u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_collision_setup_8c0509b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0509b8;
L8c0509b8: *stop=0x8c0509b8u;r[7]=r[4]&255u;goto L8c0509ba; /* extu.b r4,r7 */
L8c0509ba: *stop=0x8c0509bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c050a24u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0509bc; /* mov.l 0x8c050a24,r5 */
L8c0509bc: *stop=0x8c0509bcu;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c0509be; /* tst r7,r7 */
L8c0509be: take=(*sr&1u);*stop=0x8c0509c0u;r[6]=1u;if(take)goto L8c0509d0;goto L8c0509c2; /* bt/s 0x8c0509d0 */
L8c0509c2: *stop=0x8c0509c2u;r[0]=r[7];goto L8c0509c4; /* mov r7,r0 */
L8c0509c4: *stop=0x8c0509c4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0509c6; /* cmp/eq #0x1,r0 */
L8c0509c6: if((*sr&1u))goto L8c0509d0;goto L8c0509c8; /* bt 0x8c0509d0 */
L8c0509c8: *stop=0x8c0509c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050a28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0509ca; /* mov.l 0x8c050a28,r3 */
L8c0509ca: *stop=0x8c0509cau;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[6],1);if(io_status)return io_status;goto L8c0509cc; /* mov.b r6,@r5 */
L8c0509cc: *stop=0x8c0509ccu;if(*pr!=entry_pr)return -3;*stop=0x8c0509ceu;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[6],1);if(io_status)return io_status;return 0; /* rts */
L8c0509d0: *stop=0x8c0509d0u;r[4]=r[4]&255u;goto L8c0509d2; /* extu.b r4,r4 */
L8c0509d2: *stop=0x8c0509d2u;r[0]=r[4];goto L8c0509d4; /* mov r4,r0 */
L8c0509d4: *stop=0x8c0509d4u;if(!valid_access(r[0]+r[5],1u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[6],1);if(io_status)return io_status;goto L8c0509d6; /* mov.b r6,@(r0,r5) */
L8c0509d6: *stop=0x8c0509d6u;if(*pr!=entry_pr)return -3;*stop=0x8c0509d8u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_collision_setup_8c0869e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0869e0;
L8c0869e0: *stop=0x8c0869e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c086b00u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0869e2; /* mov.l 0x8c086b00,r2 */
L8c0869e2: *stop=0x8c0869e2u;r[3]=0u;goto L8c0869e4; /* mov #0x0,r3 */
L8c0869e4: *stop=0x8c0869e4u;if(*pr!=entry_pr)return -3;*stop=0x8c0869e6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_collision_setup_8c08a05c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c08a05c;
L8c08a05c: *stop=0x8c08a05cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c08a0fcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c08a05e; /* mov.l 0x8c08a0fc,r3 */
L8c08a05e: *stop=0x8c08a05eu;r[4]=0u;goto L8c08a060; /* mov #0x0,r4 */
L8c08a060: *stop=0x8c08a060u;{uint32_t loaded=rd(m,bus,&io_status,0x8c08a100u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c08a062; /* mov.l 0x8c08a100,r2 */
L8c08a062: *stop=0x8c08a062u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c08a064; /* mov.l r4,@r3 */
L8c08a064: *stop=0x8c08a064u;{uint32_t loaded=rd(m,bus,&io_status,0x8c08a104u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c08a066; /* mov.l 0x8c08a104,r3 */
L8c08a066: *stop=0x8c08a066u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c08a068; /* mov.l r4,@r2 */
L8c08a068: *stop=0x8c08a068u;{uint32_t loaded=rd(m,bus,&io_status,0x8c08a108u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c08a06a; /* mov.l 0x8c08a108,r2 */
L8c08a06a: *stop=0x8c08a06au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c08a06c; /* mov.l r4,@r3 */
L8c08a06c: *stop=0x8c08a06cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c08a10cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c08a06e; /* mov.l 0x8c08a10c,r3 */
L8c08a06e: *stop=0x8c08a06eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c08a070; /* mov.l r4,@r2 */
L8c08a070: *stop=0x8c08a070u;if(*pr!=entry_pr)return -3;*stop=0x8c08a072u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;return 0; /* rts */
}
