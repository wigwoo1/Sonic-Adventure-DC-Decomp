/* Original triangle assembly, clipping and emitter-selection wrapper.
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
static int sa_collision_render_run_8c63d66c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c63d6beu)goto L8c63d6be;if(resume_pc==0x8c63d6d4u)goto L8c63d6d4;if(resume_pc==0x8c63d6dau)goto L8c63d6da;if(resume_pc==0x8c63d6e8u)goto L8c63d6e8;if(resume_pc==0x8c63d6eeu)goto L8c63d6ee;if(resume_pc==0x8c63d6feu)goto L8c63d6fe;if(resume_pc==0x8c63d704u)goto L8c63d704;if(resume_pc==0x8c63d70eu)goto L8c63d70e;if(resume_pc==0x8c63d714u)goto L8c63d714;if(resume_pc==0x8c63d7c4u)goto L8c63d7c4;if(resume_pc==0x8c63d7deu)goto L8c63d7de;if(resume_pc==0x8c63d7f8u)goto L8c63d7f8;if(resume_pc==0x8c63d802u)goto L8c63d802;if(resume_pc==0x8c63d8b4u)goto L8c63d8b4;if(resume_pc==0x8c63d8d0u)goto L8c63d8d0;if(resume_pc==0x8c63d8e4u)goto L8c63d8e4;if(resume_pc==0x8c63d8f6u)goto L8c63d8f6;if(resume_pc==0x8c63d922u)goto L8c63d922;if(resume_pc==0x8c63d94cu)goto L8c63d94c;if(resume_pc==0x8c63d952u)goto L8c63d952;goto L8c63d66c;
L8c63d66c: *stop=0x8c63d66cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c63d66e; /* mov.l r14,@-r15 */
L8c63d66e: *stop=0x8c63d66eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c63d670; /* mov.l r13,@-r15 */
L8c63d670: *stop=0x8c63d670u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c63d672; /* mov.l r12,@-r15 */
L8c63d672: *stop=0x8c63d672u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c63d674; /* mov.l r11,@-r15 */
L8c63d674: *stop=0x8c63d674u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c63d676; /* mov.l r10,@-r15 */
L8c63d676: *stop=0x8c63d676u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c63d678; /* mov.l r9,@-r15 */
L8c63d678: *stop=0x8c63d678u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c63d67a; /* mov.l r8,@-r15 */
L8c63d67a: *stop=0x8c63d67au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c63d67c; /* sts.l PR,@-r15 */
L8c63d67c: *stop=0x8c63d67cu;r[0]=sx(m,bus,&io_status,0x8c63d762u,2);if(io_status)return io_status;goto L8c63d67e; /* mov.w 0x8c63d762,r0 */
L8c63d67e: *stop=0x8c63d67eu;r[15]+=r[0];goto L8c63d680; /* add r0,r15 */
L8c63d680: *stop=0x8c63d680u;r[2]=sx(m,bus,&io_status,0x8c63d764u,2);if(io_status)return io_status;goto L8c63d682; /* mov.w 0x8c63d764,r2 */
L8c63d682: *stop=0x8c63d682u;r[1]=r[15];goto L8c63d684; /* mov r15,r1 */
L8c63d684: *stop=0x8c63d684u;r[0]=sx(m,bus,&io_status,0x8c63d766u,2);if(io_status)return io_status;goto L8c63d686; /* mov.w 0x8c63d766,r0 */
L8c63d686: *stop=0x8c63d686u;r[1]+=76u;goto L8c63d688; /* add #0x4c,r1 */
L8c63d688: *stop=0x8c63d688u;r[2]+=r[15];goto L8c63d68a; /* add r15,r2 */
L8c63d68a: *stop=0x8c63d68au;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d774u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d68c; /* mov.l 0x8c63d774,r3 */
L8c63d68c: *stop=0x8c63d68cu;r[9]=r[6];goto L8c63d68e; /* mov r6,r9 */
L8c63d68e: *stop=0x8c63d68eu;r[14]=r[4];goto L8c63d690; /* mov r4,r14 */
L8c63d690: *stop=0x8c63d690u;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[5],4);if(io_status)return io_status;goto L8c63d692; /* mov.l r5,@(0x1c,r15) */
L8c63d692: *stop=0x8c63d692u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;goto L8c63d694; /* fmov.s @r3,fr4 */
L8c63d694: *stop=0x8c63d694u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c63d696; /* mov.l r2,@(r0,r15) */
L8c63d696: *stop=0x8c63d696u;r[2]=r[15];goto L8c63d698; /* mov r15,r2 */
L8c63d698: *stop=0x8c63d698u;r[0]=sx(m,bus,&io_status,0x8c63d768u,2);if(io_status)return io_status;goto L8c63d69a; /* mov.w 0x8c63d768,r0 */
L8c63d69a: *stop=0x8c63d69au;r[2]+=64u;goto L8c63d69c; /* add #0x40,r2 */
L8c63d69c: *stop=0x8c63d69cu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c63d69e; /* mov.l r1,@(r0,r15) */
L8c63d69e: *stop=0x8c63d69eu;r[1]=r[15];goto L8c63d6a0; /* mov r15,r1 */
L8c63d6a0: *stop=0x8c63d6a0u;r[0]=sx(m,bus,&io_status,0x8c63d76au,2);if(io_status)return io_status;goto L8c63d6a2; /* mov.w 0x8c63d76a,r0 */
L8c63d6a2: *stop=0x8c63d6a2u;r[1]+=88u;goto L8c63d6a4; /* add #0x58,r1 */
L8c63d6a4: *stop=0x8c63d6a4u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c63d6a6; /* mov.l r2,@(r0,r15) */
L8c63d6a6: *stop=0x8c63d6a6u;r[2]=r[15];goto L8c63d6a8; /* mov r15,r2 */
L8c63d6a8: *stop=0x8c63d6a8u;r[0]=sx(m,bus,&io_status,0x8c63d76cu,2);if(io_status)return io_status;goto L8c63d6aa; /* mov.w 0x8c63d76c,r0 */
L8c63d6aa: *stop=0x8c63d6aau;r[2]+=48u;goto L8c63d6ac; /* add #0x30,r2 */
L8c63d6ac: *stop=0x8c63d6acu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c63d6ae; /* mov.l r1,@(r0,r15) */
L8c63d6ae: *stop=0x8c63d6aeu;r[1]=r[15];goto L8c63d6b0; /* mov r15,r1 */
L8c63d6b0: *stop=0x8c63d6b0u;r[0]=sx(m,bus,&io_status,0x8c63d76eu,2);if(io_status)return io_status;goto L8c63d6b2; /* mov.w 0x8c63d76e,r0 */
L8c63d6b2: *stop=0x8c63d6b2u;r[1]+=32u;goto L8c63d6b4; /* add #0x20,r1 */
L8c63d6b4: *stop=0x8c63d6b4u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c63d6b6; /* mov.l r2,@(r0,r15) */
L8c63d6b6: *stop=0x8c63d6b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d778u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d6b8; /* mov.l 0x8c63d778,r2 */
L8c63d6b8: *stop=0x8c63d6b8u;r[0]=sx(m,bus,&io_status,0x8c63d770u,2);if(io_status)return io_status;goto L8c63d6ba; /* mov.w 0x8c63d770,r0 */
L8c63d6ba: target=r[2];*pr=0x8c63d6beu;*stop=0x8c63d6bcu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d6be; /* jsr @r2 */
L8c63d6be: *stop=0x8c63d6beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d780u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c63d6c0; /* mov.l 0x8c63d780,r11 */
L8c63d6c0: *stop=0x8c63d6c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d77cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c63d6c2; /* mov.l 0x8c63d77c,r13 */
L8c63d6c2: *stop=0x8c63d6c2u;r[11]&=r[9];goto L8c63d6c4; /* and r9,r11 */
L8c63d6c4: *stop=0x8c63d6c4u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c63d6c6; /* tst r11,r11 */
L8c63d6c6: if((*sr&1u))goto L8c63d6f2;goto L8c63d6c8; /* bt 0x8c63d6f2 */
L8c63d6c8: *stop=0x8c63d6c8u;r[2]=64u;goto L8c63d6ca; /* mov #0x40,r2 */
L8c63d6ca: *stop=0x8c63d6cau;*sr=(*sr&~1u)|!(r[9]&r[2]);goto L8c63d6cc; /* tst r9,r2 */
L8c63d6cc: if((*sr&1u))goto L8c63d6de;goto L8c63d6ce; /* bt 0x8c63d6de */
L8c63d6ce: *stop=0x8c63d6ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d784u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63d6d0; /* mov.l 0x8c63d784,r4 */
L8c63d6d0: target=r[13];*pr=0x8c63d6d4u;*stop=0x8c63d6d2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d6d4; /* jsr @r13 */
L8c63d6d4: *stop=0x8c63d6d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d788u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d6d6; /* mov.l 0x8c63d788,r2 */
L8c63d6d6: target=r[2];*pr=0x8c63d6dau;*stop=0x8c63d6d8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d6da; /* jsr @r2 */
L8c63d6da: *stop=0x8c63d6dcu;goto L8c63d6ee; /* bra 0x8c63d6ee */
L8c63d6de: *stop=0x8c63d6deu;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c63d6e0; /* tst r11,r11 */
L8c63d6e0: if((*sr&1u))goto L8c63d6f2;goto L8c63d6e2; /* bt 0x8c63d6f2 */
L8c63d6e2: *stop=0x8c63d6e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d78cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63d6e4; /* mov.l 0x8c63d78c,r4 */
L8c63d6e4: target=r[13];*pr=0x8c63d6e8u;*stop=0x8c63d6e6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d6e8; /* jsr @r13 */
L8c63d6e8: *stop=0x8c63d6e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d790u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d6ea; /* mov.l 0x8c63d790,r3 */
L8c63d6ea: target=r[3];*pr=0x8c63d6eeu;*stop=0x8c63d6ecu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d6ee; /* jsr @r3 */
L8c63d6ee: *stop=0x8c63d6f0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c63d716; /* bra 0x8c63d716 */
L8c63d6f2: *stop=0x8c63d6f2u;r[2]=64u;goto L8c63d6f4; /* mov #0x40,r2 */
L8c63d6f4: *stop=0x8c63d6f4u;*sr=(*sr&~1u)|!(r[9]&r[2]);goto L8c63d6f6; /* tst r9,r2 */
L8c63d6f6: if((*sr&1u))goto L8c63d708;goto L8c63d6f8; /* bt 0x8c63d708 */
L8c63d6f8: *stop=0x8c63d6f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d794u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63d6fa; /* mov.l 0x8c63d794,r4 */
L8c63d6fa: target=r[13];*pr=0x8c63d6feu;*stop=0x8c63d6fcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d6fe; /* jsr @r13 */
L8c63d6fe: *stop=0x8c63d6feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d798u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d700; /* mov.l 0x8c63d798,r2 */
L8c63d700: target=r[2];*pr=0x8c63d704u;*stop=0x8c63d702u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d704; /* jsr @r2 */
L8c63d704: *stop=0x8c63d706u;goto L8c63d714; /* bra 0x8c63d714 */
L8c63d708: *stop=0x8c63d708u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d79cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63d70a; /* mov.l 0x8c63d79c,r4 */
L8c63d70a: target=r[13];*pr=0x8c63d70eu;*stop=0x8c63d70cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d70e; /* jsr @r13 */
L8c63d70e: *stop=0x8c63d70eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d7a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d710; /* mov.l 0x8c63d7a0,r2 */
L8c63d710: target=r[2];*pr=0x8c63d714u;*stop=0x8c63d712u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d714; /* jsr @r2 */
L8c63d714: *stop=0x8c63d714u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c63d716; /* mov.l r0,@r15 */
L8c63d716: *stop=0x8c63d716u;r[3]=r[9];goto L8c63d718; /* mov r9,r3 */
L8c63d718: *stop=0x8c63d718u;r[2]=r[9];goto L8c63d71a; /* mov r9,r2 */
L8c63d71a: *stop=0x8c63d71au;r[5]=2u;goto L8c63d71c; /* mov #0x2,r5 */
L8c63d71c: *stop=0x8c63d71cu;r[4]=1u;goto L8c63d71e; /* mov #0x1,r4 */
L8c63d71e: *stop=0x8c63d71eu;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c63d720; /* tst r5,r2 */
L8c63d720: *stop=0x8c63d720u;r[3]&=r[4];goto L8c63d722; /* and r4,r3 */
L8c63d722: take=(*sr&1u);*stop=0x8c63d724u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c63d72a;goto L8c63d726; /* bt/s 0x8c63d72a */
L8c63d726: *stop=0x8c63d728u;r[13]=r[4];goto L8c63d734; /* bra 0x8c63d734 */
L8c63d72a: *stop=0x8c63d72au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c63d72c; /* tst r3,r3 */
L8c63d72c: if((*sr&1u))goto L8c63d732;goto L8c63d72e; /* bt 0x8c63d732 */
L8c63d72e: *stop=0x8c63d730u;r[13]=r[5];goto L8c63d734; /* bra 0x8c63d734 */
L8c63d732: *stop=0x8c63d732u;r[13]=3u;goto L8c63d734; /* mov #0x3,r13 */
L8c63d734: *stop=0x8c63d734u;r[3]=0u;goto L8c63d736; /* mov #0x0,r3 */
L8c63d736: *stop=0x8c63d736u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d7a4u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c63d738; /* mov.l 0x8c63d7a4,r12 */
L8c63d738: *stop=0x8c63d738u;r[4]=r[3];goto L8c63d73a; /* mov r3,r4 */
L8c63d73a: *stop=0x8c63d73au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c63d73c; /* mov.l r3,@(0x4,r15) */
L8c63d73c: *stop=0x8c63d73cu;*macl=r[13]*r[4];goto L8c63d73e; /* mul.l r13,r4 */
L8c63d73e: *stop=0x8c63d73eu;r[3]=32u;goto L8c63d740; /* mov #0x20,r3 */
L8c63d740: *stop=0x8c63d740u;r[3]&=r[9];goto L8c63d742; /* and r9,r3 */
L8c63d742: *stop=0x8c63d742u;r[4]=*macl;goto L8c63d744; /* sts MACL,r4 */
L8c63d744: *stop=0x8c63d744u;r[4]+=1u;goto L8c63d746; /* add #0x1,r4 */
L8c63d746: *stop=0x8c63d746u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[4],4);if(io_status)return io_status;goto L8c63d748; /* mov.l r4,@(0x10,r15) */
L8c63d748: *stop=0x8c63d748u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[4],4);if(io_status)return io_status;goto L8c63d74a; /* mov.l r4,@(0x14,r15) */
L8c63d74a: *stop=0x8c63d74au;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[4],4);if(io_status)return io_status;goto L8c63d74c; /* mov.l r4,@(0x18,r15) */
L8c63d74c: *stop=0x8c63d74cu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c63d74e; /* mov.l r3,@(0xc,r15) */
L8c63d74e: *stop=0x8c63d74eu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d750; /* mov.l @(0x1c,r15),r2 */
L8c63d750: *stop=0x8c63d750u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c63d752; /* cmp/pl r2 */
L8c63d752: if((*sr&1u))goto L8c63d758;goto L8c63d754; /* bt 0x8c63d758 */
L8c63d754: *stop=0x8c63d756u;goto L8c63d946; /* bra 0x8c63d946 */
L8c63d758: *stop=0x8c63d758u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d75a; /* mov.l @(0x8,r15),r3 */
L8c63d75a: *stop=0x8c63d75au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c63d75c; /* tst r3,r3 */
L8c63d75c: if((*sr&1u))goto L8c63d7a8;goto L8c63d75e; /* bt 0x8c63d7a8 */
L8c63d75e: *stop=0x8c63d760u;r[10]=0u;goto L8c63d7ae; /* bra 0x8c63d7ae */
L8c63d7a8: *stop=0x8c63d7a8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c63d7aa; /* mov.l @(0x4,r15),r10 */
L8c63d7aa: *stop=0x8c63d7aau;*macl=r[13]*r[10];goto L8c63d7ac; /* mul.l r13,r10 */
L8c63d7ac: *stop=0x8c63d7acu;r[10]=*macl;goto L8c63d7ae; /* sts MACL,r10 */
L8c63d7ae: *stop=0x8c63d7aeu;r[5]=r[10];goto L8c63d7b0; /* mov r10,r5 */
L8c63d7b0: *stop=0x8c63d7b0u;r[3]=r[10];goto L8c63d7b2; /* mov r10,r3 */
L8c63d7b2: *stop=0x8c63d7b2u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c63d7b4; /* shll r5 */
L8c63d7b4: *stop=0x8c63d7b4u;r[0]=sx(m,bus,&io_status,0x8c63d8fau,2);if(io_status)return io_status;goto L8c63d7b6; /* mov.w 0x8c63d8fa,r0 */
L8c63d7b6: *stop=0x8c63d7b6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d7b8; /* mov.l @r14,r2 */
L8c63d7b8: *stop=0x8c63d7b8u;r[5]+=r[3];goto L8c63d7ba; /* add r3,r5 */
L8c63d7ba: *stop=0x8c63d7bau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c63d7bc; /* mov.l @(r0,r15),r6 */
L8c63d7bc: *stop=0x8c63d7bcu;r[5]<<=2;goto L8c63d7be; /* shll2 r5 */
L8c63d7be: *stop=0x8c63d7beu;r[5]+=r[2];goto L8c63d7c0; /* add r2,r5 */
L8c63d7c0: target=r[12];*pr=0x8c63d7c4u;*stop=0x8c63d7c2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d7c4; /* jsr @r12 */
L8c63d7c4: *stop=0x8c63d7c4u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c63d7c6; /* mov.l @(0x18,r15),r8 */
L8c63d7c6: *stop=0x8c63d7c6u;r[0]=sx(m,bus,&io_status,0x8c63d8fau,2);if(io_status)return io_status;goto L8c63d7c8; /* mov.w 0x8c63d8fa,r0 */
L8c63d7c8: *stop=0x8c63d7c8u;r[5]=r[8];goto L8c63d7ca; /* mov r8,r5 */
L8c63d7ca: *stop=0x8c63d7cau;r[3]=r[8];goto L8c63d7cc; /* mov r8,r3 */
L8c63d7cc: *stop=0x8c63d7ccu;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c63d7ce; /* shll r5 */
L8c63d7ce: *stop=0x8c63d7ceu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d7d0; /* mov.l @r14,r2 */
L8c63d7d0: *stop=0x8c63d7d0u;r[5]+=r[3];goto L8c63d7d2; /* add r3,r5 */
L8c63d7d2: *stop=0x8c63d7d2u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c63d7d4; /* mov.l @(r0,r15),r6 */
L8c63d7d4: *stop=0x8c63d7d4u;r[5]<<=2;goto L8c63d7d6; /* shll2 r5 */
L8c63d7d6: *stop=0x8c63d7d6u;r[5]+=r[2];goto L8c63d7d8; /* add r2,r5 */
L8c63d7d8: *stop=0x8c63d7d8u;r[6]+=12u;goto L8c63d7da; /* add #0xc,r6 */
L8c63d7da: target=r[12];*pr=0x8c63d7deu;*stop=0x8c63d7dcu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d7de; /* jsr @r12 */
L8c63d7de: *stop=0x8c63d7deu;r[8]+=1u;goto L8c63d7e0; /* add #0x1,r8 */
L8c63d7e0: *stop=0x8c63d7e0u;r[0]=sx(m,bus,&io_status,0x8c63d8fau,2);if(io_status)return io_status;goto L8c63d7e2; /* mov.w 0x8c63d8fa,r0 */
L8c63d7e2: *stop=0x8c63d7e2u;r[5]=r[8];goto L8c63d7e4; /* mov r8,r5 */
L8c63d7e4: *stop=0x8c63d7e4u;r[3]=r[8];goto L8c63d7e6; /* mov r8,r3 */
L8c63d7e6: *stop=0x8c63d7e6u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c63d7e8; /* shll r5 */
L8c63d7e8: *stop=0x8c63d7e8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d7ea; /* mov.l @r14,r2 */
L8c63d7ea: *stop=0x8c63d7eau;r[5]+=r[3];goto L8c63d7ec; /* add r3,r5 */
L8c63d7ec: *stop=0x8c63d7ecu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c63d7ee; /* mov.l @(r0,r15),r6 */
L8c63d7ee: *stop=0x8c63d7eeu;r[5]<<=2;goto L8c63d7f0; /* shll2 r5 */
L8c63d7f0: *stop=0x8c63d7f0u;r[5]+=r[2];goto L8c63d7f2; /* add r2,r5 */
L8c63d7f2: *stop=0x8c63d7f2u;r[6]+=24u;goto L8c63d7f4; /* add #0x18,r6 */
L8c63d7f4: target=r[12];*pr=0x8c63d7f8u;*stop=0x8c63d7f6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d7f8; /* jsr @r12 */
L8c63d7f8: *stop=0x8c63d7f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d904u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d7fa; /* mov.l 0x8c63d904,r3 */
L8c63d7fa: *stop=0x8c63d7fau;r[5]=3u;goto L8c63d7fc; /* mov #0x3,r5 */
L8c63d7fc: *stop=0x8c63d7fcu;r[4]=sx(m,bus,&io_status,0x8c63d8fau,2);if(io_status)return io_status;goto L8c63d7fe; /* mov.w 0x8c63d8fa,r4 */
L8c63d7fe: target=r[3];*pr=0x8c63d802u;*stop=0x8c63d800u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d802; /* jsr @r3 */
L8c63d802: *stop=0x8c63d802u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c63d804; /* tst r0,r0 */
L8c63d804: if(!(*sr&1u))goto L8c63d80a;goto L8c63d806; /* bf 0x8c63d80a */
L8c63d806: *stop=0x8c63d808u;goto L8c63d922; /* bra 0x8c63d922 */
L8c63d80a: *stop=0x8c63d80au;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c63d80c; /* tst r11,r11 */
L8c63d80c: if((*sr&1u))goto L8c63d86e;goto L8c63d80e; /* bt 0x8c63d86e */
L8c63d80e: *stop=0x8c63d80eu;r[0]=sx(m,bus,&io_status,0x8c63d8fcu,2);if(io_status)return io_status;goto L8c63d810; /* mov.w 0x8c63d8fc,r0 */
L8c63d810: *stop=0x8c63d810u;r[4]=r[10];goto L8c63d812; /* mov r10,r4 */
L8c63d812: *stop=0x8c63d812u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d814; /* mov.l @(0x8,r14),r3 */
L8c63d814: *stop=0x8c63d814u;r[4]<<=2;goto L8c63d816; /* shll2 r4 */
L8c63d816: *stop=0x8c63d816u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d818; /* mov.l @(r0,r15),r2 */
L8c63d818: *stop=0x8c63d818u;r[0]=sx(m,bus,&io_status,0x8c63d8fcu,2);if(io_status)return io_status;goto L8c63d81a; /* mov.w 0x8c63d8fc,r0 */
L8c63d81a: *stop=0x8c63d81au;r[3]+=r[4];goto L8c63d81c; /* add r4,r3 */
L8c63d81c: *stop=0x8c63d81cu;if(!valid_access(r[3],2u))return -3;r[1]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c63d81e; /* mov.w @r3,r1 */
L8c63d81e: *stop=0x8c63d81eu;if(!valid_access(r[2],2u))return -3;wr(m,bus,&io_status,r[2],r[1],2);if(io_status)return io_status;goto L8c63d820; /* mov.w r1,@r2 */
L8c63d820: *stop=0x8c63d820u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d822; /* mov.l @(r0,r15),r3 */
L8c63d822: *stop=0x8c63d822u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c63d824; /* mov.l @(0x8,r14),r0 */
L8c63d824: *stop=0x8c63d824u;r[0]+=r[4];goto L8c63d826; /* add r4,r0 */
L8c63d826: *stop=0x8c63d826u;if(!valid_access(r[0]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c63d828; /* mov.w @(0x2,r0),r0 */
L8c63d828: *stop=0x8c63d828u;if(!valid_access(r[3]+2u,2u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c63d82a; /* mov.w r0,@(0x2,r3) */
L8c63d82a: *stop=0x8c63d82au;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63d82c; /* mov.l @(0x14,r15),r4 */
L8c63d82c: *stop=0x8c63d82cu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d82e; /* mov.l @(0x8,r14),r2 */
L8c63d82e: *stop=0x8c63d82eu;r[5]=r[4];goto L8c63d830; /* mov r4,r5 */
L8c63d830: *stop=0x8c63d830u;r[0]=sx(m,bus,&io_status,0x8c63d8fcu,2);if(io_status)return io_status;goto L8c63d832; /* mov.w 0x8c63d8fc,r0 */
L8c63d832: *stop=0x8c63d832u;r[5]<<=2;goto L8c63d834; /* shll2 r5 */
L8c63d834: *stop=0x8c63d834u;r[2]+=r[5];goto L8c63d836; /* add r5,r2 */
L8c63d836: *stop=0x8c63d836u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d838; /* mov.l @(r0,r15),r3 */
L8c63d838: *stop=0x8c63d838u;if(!valid_access(r[2],2u))return -3;r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c63d83a; /* mov.w @r2,r1 */
L8c63d83a: *stop=0x8c63d83au;r[4]+=1u;goto L8c63d83c; /* add #0x1,r4 */
L8c63d83c: *stop=0x8c63d83cu;r[0]=sx(m,bus,&io_status,0x8c63d8fcu,2);if(io_status)return io_status;goto L8c63d83e; /* mov.w 0x8c63d8fc,r0 */
L8c63d83e: *stop=0x8c63d83eu;r[3]+=4u;goto L8c63d840; /* add #0x4,r3 */
L8c63d840: *stop=0x8c63d840u;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c63d842; /* mov.w r1,@r3 */
L8c63d842: *stop=0x8c63d842u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d844; /* mov.l @(r0,r15),r3 */
L8c63d844: *stop=0x8c63d844u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c63d846; /* mov.l @(0x8,r14),r0 */
L8c63d846: *stop=0x8c63d846u;r[3]+=4u;goto L8c63d848; /* add #0x4,r3 */
L8c63d848: *stop=0x8c63d848u;r[0]+=r[5];goto L8c63d84a; /* add r5,r0 */
L8c63d84a: *stop=0x8c63d84au;if(!valid_access(r[0]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c63d84c; /* mov.w @(0x2,r0),r0 */
L8c63d84c: *stop=0x8c63d84cu;if(!valid_access(r[3]+2u,2u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c63d84e; /* mov.w r0,@(0x2,r3) */
L8c63d84e: *stop=0x8c63d84eu;r[5]=r[4];goto L8c63d850; /* mov r4,r5 */
L8c63d850: *stop=0x8c63d850u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d852; /* mov.l @(0x8,r14),r2 */
L8c63d852: *stop=0x8c63d852u;r[0]=sx(m,bus,&io_status,0x8c63d8fcu,2);if(io_status)return io_status;goto L8c63d854; /* mov.w 0x8c63d8fc,r0 */
L8c63d854: *stop=0x8c63d854u;r[5]<<=2;goto L8c63d856; /* shll2 r5 */
L8c63d856: *stop=0x8c63d856u;r[2]+=r[5];goto L8c63d858; /* add r5,r2 */
L8c63d858: *stop=0x8c63d858u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d85a; /* mov.l @(r0,r15),r3 */
L8c63d85a: *stop=0x8c63d85au;if(!valid_access(r[2],2u))return -3;r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c63d85c; /* mov.w @r2,r1 */
L8c63d85c: *stop=0x8c63d85cu;r[0]=sx(m,bus,&io_status,0x8c63d8fcu,2);if(io_status)return io_status;goto L8c63d85e; /* mov.w 0x8c63d8fc,r0 */
L8c63d85e: *stop=0x8c63d85eu;r[3]+=8u;goto L8c63d860; /* add #0x8,r3 */
L8c63d860: *stop=0x8c63d860u;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c63d862; /* mov.w r1,@r3 */
L8c63d862: *stop=0x8c63d862u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d864; /* mov.l @(r0,r15),r3 */
L8c63d864: *stop=0x8c63d864u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c63d866; /* mov.l @(0x8,r14),r0 */
L8c63d866: *stop=0x8c63d866u;r[3]+=8u;goto L8c63d868; /* add #0x8,r3 */
L8c63d868: *stop=0x8c63d868u;r[0]+=r[5];goto L8c63d86a; /* add r5,r0 */
L8c63d86a: *stop=0x8c63d86au;if(!valid_access(r[0]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c63d86c; /* mov.w @(0x2,r0),r0 */
L8c63d86c: *stop=0x8c63d86cu;if(!valid_access(r[3]+2u,2u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c63d86e; /* mov.w r0,@(0x2,r3) */
L8c63d86e: *stop=0x8c63d86eu;r[2]=r[10];goto L8c63d870; /* mov r10,r2 */
L8c63d870: *stop=0x8c63d870u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d872; /* mov.l @(0x4,r14),r1 */
L8c63d872: *stop=0x8c63d872u;r[0]=sx(m,bus,&io_status,0x8c63d8feu,2);if(io_status)return io_status;goto L8c63d874; /* mov.w 0x8c63d8fe,r0 */
L8c63d874: *stop=0x8c63d874u;r[2]<<=2;goto L8c63d876; /* shll2 r2 */
L8c63d876: *stop=0x8c63d876u;r[2]+=r[1];goto L8c63d878; /* add r1,r2 */
L8c63d878: *stop=0x8c63d878u;r[6]=sx(m,bus,&io_status,0x8c63d900u,2);if(io_status)return io_status;goto L8c63d87a; /* mov.w 0x8c63d900,r6 */
L8c63d87a: *stop=0x8c63d87au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d87c; /* mov.l @(r0,r15),r3 */
L8c63d87c: *stop=0x8c63d87cu;r[5]=3u;goto L8c63d87e; /* mov #0x3,r5 */
L8c63d87e: *stop=0x8c63d87eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c63d880; /* mov.l @r2,r0 */
L8c63d880: *stop=0x8c63d880u;r[6]+=r[15];goto L8c63d882; /* add r15,r6 */
L8c63d882: *stop=0x8c63d882u;r[7]=r[9];goto L8c63d884; /* mov r9,r7 */
L8c63d884: *stop=0x8c63d884u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c63d886; /* mov.l r0,@r3 */
L8c63d886: *stop=0x8c63d886u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63d888; /* mov.l @(0x10,r15),r4 */
L8c63d888: *stop=0x8c63d888u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d88a; /* mov.l @(0x4,r14),r1 */
L8c63d88a: *stop=0x8c63d88au;r[2]=r[4];goto L8c63d88c; /* mov r4,r2 */
L8c63d88c: *stop=0x8c63d88cu;r[0]=sx(m,bus,&io_status,0x8c63d8feu,2);if(io_status)return io_status;goto L8c63d88e; /* mov.w 0x8c63d8fe,r0 */
L8c63d88e: *stop=0x8c63d88eu;r[2]<<=2;goto L8c63d890; /* shll2 r2 */
L8c63d890: *stop=0x8c63d890u;r[2]+=r[1];goto L8c63d892; /* add r1,r2 */
L8c63d892: *stop=0x8c63d892u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d894; /* mov.l @(r0,r15),r3 */
L8c63d894: *stop=0x8c63d894u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c63d896; /* mov.l @r2,r0 */
L8c63d896: *stop=0x8c63d896u;r[4]+=1u;goto L8c63d898; /* add #0x1,r4 */
L8c63d898: *stop=0x8c63d898u;r[3]+=4u;goto L8c63d89a; /* add #0x4,r3 */
L8c63d89a: *stop=0x8c63d89au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c63d89c; /* mov.l r0,@r3 */
L8c63d89c: *stop=0x8c63d89cu;r[4]<<=2;goto L8c63d89e; /* shll2 r4 */
L8c63d89e: *stop=0x8c63d89eu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d8a0; /* mov.l @(0x4,r14),r2 */
L8c63d8a0: *stop=0x8c63d8a0u;r[0]=sx(m,bus,&io_status,0x8c63d8feu,2);if(io_status)return io_status;goto L8c63d8a2; /* mov.w 0x8c63d8fe,r0 */
L8c63d8a2: *stop=0x8c63d8a2u;r[4]+=r[2];goto L8c63d8a4; /* add r2,r4 */
L8c63d8a4: *stop=0x8c63d8a4u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d8a6; /* mov.l @(r0,r15),r3 */
L8c63d8a6: *stop=0x8c63d8a6u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d8a8; /* mov.l @r4,r1 */
L8c63d8a8: *stop=0x8c63d8a8u;r[3]+=8u;goto L8c63d8aa; /* add #0x8,r3 */
L8c63d8aa: *stop=0x8c63d8aau;r[4]=sx(m,bus,&io_status,0x8c63d8fau,2);if(io_status)return io_status;goto L8c63d8ac; /* mov.w 0x8c63d8fa,r4 */
L8c63d8ac: *stop=0x8c63d8acu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c63d8ae; /* mov.l r1,@r3 */
L8c63d8ae: *stop=0x8c63d8aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d908u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d8b0; /* mov.l 0x8c63d908,r3 */
L8c63d8b0: target=r[3];*pr=0x8c63d8b4u;*stop=0x8c63d8b2u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d8b4; /* jsr @r3 */
L8c63d8b4: *stop=0x8c63d8b4u;r[2]=3u;goto L8c63d8b6; /* mov #0x3,r2 */
L8c63d8b6: *stop=0x8c63d8b6u;r[4]=r[0];goto L8c63d8b8; /* mov r0,r4 */
L8c63d8b8: *stop=0x8c63d8b8u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]);goto L8c63d8ba; /* cmp/ge r2,r4 */
L8c63d8ba: if(!(*sr&1u))goto L8c63d922;goto L8c63d8bc; /* bf 0x8c63d922 */
L8c63d8bc: *stop=0x8c63d8bcu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d8be; /* mov.l @(0xc,r15),r1 */
L8c63d8be: *stop=0x8c63d8beu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c63d8c0; /* tst r1,r1 */
L8c63d8c0: if((*sr&1u))goto L8c63d8d4;goto L8c63d8c2; /* bt 0x8c63d8d4 */
L8c63d8c2: *stop=0x8c63d8c2u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c63d8c4; /* tst r11,r11 */
L8c63d8c4: if((*sr&1u))goto L8c63d8d4;goto L8c63d8c6; /* bt 0x8c63d8d4 */
L8c63d8c6: *stop=0x8c63d8c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d90cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d8c8; /* mov.l 0x8c63d90c,r2 */
L8c63d8c8: *stop=0x8c63d8c8u;r[6]=r[15];goto L8c63d8ca; /* mov r15,r6 */
L8c63d8ca: *stop=0x8c63d8cau;r[5]=sx(m,bus,&io_status,0x8c63d900u,2);if(io_status)return io_status;goto L8c63d8cc; /* mov.w 0x8c63d900,r5 */
L8c63d8cc: target=r[2];*pr=0x8c63d8d0u;*stop=0x8c63d8ceu;r[5]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d8d0; /* jsr @r2 */
L8c63d8d0: *stop=0x8c63d8d2u;goto L8c63d922; /* bra 0x8c63d922 */
L8c63d8d4: *stop=0x8c63d8d4u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d8d6; /* mov.l @(0xc,r15),r2 */
L8c63d8d6: *stop=0x8c63d8d6u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c63d8d8; /* tst r2,r2 */
L8c63d8d8: if((*sr&1u))goto L8c63d8e8;goto L8c63d8da; /* bt 0x8c63d8e8 */
L8c63d8da: *stop=0x8c63d8dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d910u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d8dc; /* mov.l 0x8c63d910,r3 */
L8c63d8dc: *stop=0x8c63d8dcu;r[6]=r[15];goto L8c63d8de; /* mov r15,r6 */
L8c63d8de: *stop=0x8c63d8deu;r[5]=sx(m,bus,&io_status,0x8c63d900u,2);if(io_status)return io_status;goto L8c63d8e0; /* mov.w 0x8c63d900,r5 */
L8c63d8e0: target=r[3];*pr=0x8c63d8e4u;*stop=0x8c63d8e2u;r[5]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d8e4; /* jsr @r3 */
L8c63d8e4: *stop=0x8c63d8e6u;goto L8c63d922; /* bra 0x8c63d922 */
L8c63d8e8: *stop=0x8c63d8e8u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c63d8ea; /* tst r11,r11 */
L8c63d8ea: if((*sr&1u))goto L8c63d918;goto L8c63d8ec; /* bt 0x8c63d918 */
L8c63d8ec: *stop=0x8c63d8ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d914u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d8ee; /* mov.l 0x8c63d914,r2 */
L8c63d8ee: *stop=0x8c63d8eeu;r[6]=r[15];goto L8c63d8f0; /* mov r15,r6 */
L8c63d8f0: *stop=0x8c63d8f0u;r[5]=sx(m,bus,&io_status,0x8c63d900u,2);if(io_status)return io_status;goto L8c63d8f2; /* mov.w 0x8c63d900,r5 */
L8c63d8f2: target=r[2];*pr=0x8c63d8f6u;*stop=0x8c63d8f4u;r[5]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d8f6; /* jsr @r2 */
L8c63d8f6: *stop=0x8c63d8f8u;goto L8c63d922; /* bra 0x8c63d922 */
L8c63d918: *stop=0x8c63d918u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d96cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d91a; /* mov.l 0x8c63d96c,r2 */
L8c63d91a: *stop=0x8c63d91au;r[6]=r[15];goto L8c63d91c; /* mov r15,r6 */
L8c63d91c: *stop=0x8c63d91cu;r[5]=sx(m,bus,&io_status,0x8c63d968u,2);if(io_status)return io_status;goto L8c63d91e; /* mov.w 0x8c63d968,r5 */
L8c63d91e: target=r[2];*pr=0x8c63d922u;*stop=0x8c63d920u;r[5]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d922; /* jsr @r2 */
L8c63d922: *stop=0x8c63d922u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d924; /* mov.l @(0x4,r15),r3 */
L8c63d924: *stop=0x8c63d924u;r[3]+=1u;goto L8c63d926; /* add #0x1,r3 */
L8c63d926: *stop=0x8c63d926u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c63d928; /* mov.l r3,@(0x4,r15) */
L8c63d928: *stop=0x8c63d928u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d92a; /* mov.l @(0x10,r15),r2 */
L8c63d92a: *stop=0x8c63d92au;r[2]+=r[13];goto L8c63d92c; /* add r13,r2 */
L8c63d92c: *stop=0x8c63d92cu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c63d92e; /* mov.l r2,@(0x10,r15) */
L8c63d92e: *stop=0x8c63d92eu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d930; /* mov.l @(0x14,r15),r1 */
L8c63d930: *stop=0x8c63d930u;r[1]+=r[13];goto L8c63d932; /* add r13,r1 */
L8c63d932: *stop=0x8c63d932u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[1],4);if(io_status)return io_status;goto L8c63d934; /* mov.l r1,@(0x14,r15) */
L8c63d934: *stop=0x8c63d934u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d936; /* mov.l @(0x18,r15),r3 */
L8c63d936: *stop=0x8c63d936u;r[3]+=r[13];goto L8c63d938; /* add r13,r3 */
L8c63d938: *stop=0x8c63d938u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c63d93a; /* mov.l r3,@(0x18,r15) */
L8c63d93a: *stop=0x8c63d93au;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d93c; /* mov.l @(0x1c,r15),r2 */
L8c63d93c: *stop=0x8c63d93cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c63d93e; /* mov.l @(0x4,r15),r0 */
L8c63d93e: *stop=0x8c63d93eu;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[2]);goto L8c63d940; /* cmp/ge r2,r0 */
L8c63d940: if((*sr&1u))goto L8c63d946;goto L8c63d942; /* bt 0x8c63d946 */
L8c63d942: *stop=0x8c63d944u;goto L8c63d758; /* bra 0x8c63d758 */
L8c63d946: *stop=0x8c63d946u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d970u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d948; /* mov.l 0x8c63d970,r3 */
L8c63d948: target=r[3];*pr=0x8c63d94cu;*stop=0x8c63d94au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d94c; /* jsr @r3 */
L8c63d94c: *stop=0x8c63d94cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d974u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d94e; /* mov.l 0x8c63d974,r2 */
L8c63d94e: target=r[2];*pr=0x8c63d952u;*stop=0x8c63d950u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d952; /* jsr @r2 */
L8c63d952: *stop=0x8c63d952u;r[1]=sx(m,bus,&io_status,0x8c63d96au,2);if(io_status)return io_status;goto L8c63d954; /* mov.w 0x8c63d96a,r1 */
L8c63d954: *stop=0x8c63d954u;r[15]+=r[1];goto L8c63d956; /* add r1,r15 */
L8c63d956: *stop=0x8c63d956u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c63d958; /* lds.l @r15+,PR */
L8c63d958: *stop=0x8c63d958u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c63d95a; /* mov.l @r15+,r8 */
L8c63d95a: *stop=0x8c63d95au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c63d95c; /* mov.l @r15+,r9 */
L8c63d95c: *stop=0x8c63d95cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c63d95e; /* mov.l @r15+,r10 */
L8c63d95e: *stop=0x8c63d95eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c63d960; /* mov.l @r15+,r11 */
L8c63d960: *stop=0x8c63d960u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c63d962; /* mov.l @r15+,r12 */
L8c63d962: *stop=0x8c63d962u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c63d964; /* mov.l @r15+,r13 */
L8c63d964: *stop=0x8c63d964u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c63d966u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_collision_render_8c63d66c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d6be(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d6beu);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d6d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d6d4u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d6da(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d6dau);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d6e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d6e8u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d6ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d6eeu);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d6fe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d6feu);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d704(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d704u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d70e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d70eu);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d714(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d714u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d7c4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d7c4u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d7de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d7deu);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d7f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d7f8u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d802(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d802u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d8b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d8b4u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d8d0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d8d0u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d8e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d8e4u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d8f6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d8f6u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d922(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d922u);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d94c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d94cu);}
SA_STAGE_EXPORT int sa_collision_render_resume_8c63d952(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_render_run_8c63d66c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d952u);}
