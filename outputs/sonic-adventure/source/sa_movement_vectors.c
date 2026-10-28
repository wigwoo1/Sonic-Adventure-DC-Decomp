/* Original movement-vector rotation/response and dot product plus canonical adapters.
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
static int sa_movement_vectors_run_8c079000(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c079032u)goto L8c079032;if(resume_pc==0x8c079044u)goto L8c079044;if(resume_pc==0x8c07904eu)goto L8c07904e;if(resume_pc==0x8c07905au)goto L8c07905a;if(resume_pc==0x8c079066u)goto L8c079066;if(resume_pc==0x8c079078u)goto L8c079078;if(resume_pc==0x8c079084u)goto L8c079084;if(resume_pc==0x8c07908au)goto L8c07908a;goto L8c079000;
L8c079000: *stop=0x8c079000u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c079002; /* mov.l r14,@-r15 */
L8c079002: *stop=0x8c079002u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c079004; /* mov.l r13,@-r15 */
L8c079004: *stop=0x8c079004u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c079006; /* fmov.s fr15,@-r15 */
L8c079006: *stop=0x8c079006u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c079008; /* sts.l PR,@-r15 */
L8c079008: *stop=0x8c079008u;r[15]+=4294967272u;goto L8c07900a; /* add #-0x18,r15 */
L8c07900a: *stop=0x8c07900au;r[14]=r[4];goto L8c07900c; /* mov r4,r14 */
L8c07900c: *stop=0x8c07900cu;r[0]=12u;goto L8c07900e; /* mov #0xc,r0 */
L8c07900e: *stop=0x8c07900eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0791c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c079010; /* mov.l 0x8c0791c4,r3 */
L8c079010: *stop=0x8c079010u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c079012; /* mov.l r5,@r15 */
L8c079012: *stop=0x8c079012u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c079014; /* mov.l r6,@(0x4,r15) */
L8c079014: *stop=0x8c079014u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c079016; /* mov.l @r15,r4 */
L8c079016: *stop=0x8c079016u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c079018; /* fmov.s @r4+,fr3 */
L8c079018: *stop=0x8c079018u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07901a; /* fmov fr3,@(r0,r15) */
L8c07901a: *stop=0x8c07901au;r[0]=16u;goto L8c07901c; /* mov #0x10,r0 */
L8c07901c: *stop=0x8c07901cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c07901e; /* fmov.s @r4+,fr3 */
L8c07901e: *stop=0x8c07901eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c079020; /* fmov fr3,@(r0,r15) */
L8c079020: *stop=0x8c079020u;r[0]=20u;goto L8c079022; /* mov #0x14,r0 */
L8c079022: *stop=0x8c079022u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c079024; /* fmov.s @r4,fr3 */
L8c079024: *stop=0x8c079024u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c079026; /* fmov fr3,@(r0,r15) */
L8c079026: *stop=0x8c079026u;r[0]=0x8c0791c0u;goto L8c079028; /* mova 0x8c0791c0,r0 */
L8c079028: *stop=0x8c079028u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c07902a; /* fmov.s @r0,fr15 */
L8c07902a: *stop=0x8c07902au;r[0]=4u;goto L8c07902c; /* mov #0x4,r0 */
L8c07902c: *stop=0x8c07902cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c07902e; /* fmov.s @(r0,r14),fr5 */
L8c07902e: target=r[3];*pr=0x8c079032u;*stop=0x8c079030u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c079032; /* jsr @r3 */
L8c079032: *stop=0x8c079032u;fcopy(f,3u,0u);goto L8c079034; /* fmov fr0,fr3 */
L8c079034: *stop=0x8c079034u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c079036; /* fmul fr15,fr3 */
L8c079036: *stop=0x8c079036u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0791c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c079038; /* mov.l 0x8c0791c8,r3 */
L8c079038: *stop=0x8c079038u;r[0]=8u;goto L8c07903a; /* mov #0x8,r0 */
L8c07903a: *stop=0x8c07903au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c07903c; /* ftrc fr3,FPUL */
L8c07903c: *stop=0x8c07903cu;r[13]=*fpul;goto L8c07903e; /* sts FPUL,r13 */
L8c07903e: *stop=0x8c07903eu;r[13]=0u-r[13];goto L8c079040; /* neg r13,r13 */
L8c079040: target=r[3];*pr=0x8c079044u;*stop=0x8c079042u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c079044; /* jsr @r3 */
L8c079044: *stop=0x8c079044u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[15],&f->fpscr,&fr[0]);if(status)return status;}goto L8c079046; /* fmul fr15,fr0 */
L8c079046: *stop=0x8c079046u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0791ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c079048; /* mov.l 0x8c0791cc,r2 */
L8c079048: *stop=0x8c079048u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c07904a; /* ftrc fr0,FPUL */
L8c07904a: target=r[2];*pr=0x8c07904eu;*stop=0x8c07904cu;r[14]=*fpul;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07904e; /* jsr @r2 */
L8c07904e: *stop=0x8c07904eu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c079050; /* tst r13,r13 */
L8c079050: if((*sr&1u))goto L8c07905a;goto L8c079052; /* bt 0x8c07905a */
L8c079052: *stop=0x8c079052u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0791d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c079054; /* mov.l 0x8c0791d0,r2 */
L8c079054: *stop=0x8c079054u;r[5]=r[13];goto L8c079056; /* mov r13,r5 */
L8c079056: target=r[2];*pr=0x8c07905au;*stop=0x8c079058u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07905a; /* jsr @r2 */
L8c07905a: *stop=0x8c07905au;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c07905c; /* tst r14,r14 */
L8c07905c: if((*sr&1u))goto L8c079066;goto L8c07905e; /* bt 0x8c079066 */
L8c07905e: *stop=0x8c07905eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0791d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c079060; /* mov.l 0x8c0791d4,r3 */
L8c079060: *stop=0x8c079060u;r[5]=r[14];goto L8c079062; /* mov r14,r5 */
L8c079062: target=r[3];*pr=0x8c079066u;*stop=0x8c079064u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c079066; /* jsr @r3 */
L8c079066: *stop=0x8c079066u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c079068; /* mov.l @(0x4,r15),r3 */
L8c079068: *stop=0x8c079068u;r[3]=0u-r[3];goto L8c07906a; /* neg r3,r3 */
L8c07906a: *stop=0x8c07906au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c07906c; /* tst r3,r3 */
L8c07906c: take=(*sr&1u);*stop=0x8c07906eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c079078;goto L8c079070; /* bt/s 0x8c079078 */
L8c079070: *stop=0x8c079070u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0791d8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c079072; /* mov.l 0x8c0791d8,r1 */
L8c079072: *stop=0x8c079072u;r[5]=r[3];goto L8c079074; /* mov r3,r5 */
L8c079074: target=r[1];*pr=0x8c079078u;*stop=0x8c079076u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c079078; /* jsr @r1 */
L8c079078: *stop=0x8c079078u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0791dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07907a; /* mov.l 0x8c0791dc,r3 */
L8c07907a: *stop=0x8c07907au;r[5]=r[15];goto L8c07907c; /* mov r15,r5 */
L8c07907c: *stop=0x8c07907cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c07907e; /* mov.l @r15,r6 */
L8c07907e: *stop=0x8c07907eu;r[5]+=12u;goto L8c079080; /* add #0xc,r5 */
L8c079080: target=r[3];*pr=0x8c079084u;*stop=0x8c079082u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c079084; /* jsr @r3 */
L8c079084: *stop=0x8c079084u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0791e0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c079086; /* mov.l 0x8c0791e0,r2 */
L8c079086: target=r[2];*pr=0x8c07908au;*stop=0x8c079088u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07908a; /* jsr @r2 */
L8c07908a: *stop=0x8c07908au;r[15]+=24u;goto L8c07908c; /* add #0x18,r15 */
L8c07908c: *stop=0x8c07908cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c07908e; /* lds.l @r15+,PR */
L8c07908e: *stop=0x8c07908eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c079090; /* fmov.s @r15+,fr15 */
L8c079090: *stop=0x8c079090u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c079092; /* mov.l @r15+,r13 */
L8c079092: *stop=0x8c079092u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c079094u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_vectors_8c079000(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c079000(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c079032(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c079000(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c079032u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c079044(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c079000(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c079044u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c07904e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c079000(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07904eu);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c07905a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c079000(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07905au);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c079066(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c079000(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c079066u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c079078(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c079000(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c079078u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c079084(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c079000(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c079084u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c07908a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c079000(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07908au);}

static int sa_movement_vectors_run_8c074cf0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c074d94u)goto L8c074d94;if(resume_pc==0x8c074e60u)goto L8c074e60;if(resume_pc==0x8c074e74u)goto L8c074e74;goto L8c074cf0;
L8c074cf0: *stop=0x8c074cf0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c074cf2; /* mov.l r14,@-r15 */
L8c074cf2: *stop=0x8c074cf2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c074cf4; /* mov.l r13,@-r15 */
L8c074cf4: *stop=0x8c074cf4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c074cf6; /* mov.l r12,@-r15 */
L8c074cf6: *stop=0x8c074cf6u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c074cf8; /* fmov.s fr15,@-r15 */
L8c074cf8: *stop=0x8c074cf8u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c074cfa; /* fmov.s fr14,@-r15 */
L8c074cfa: *stop=0x8c074cfau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c074cfc; /* fmov.s fr13,@-r15 */
L8c074cfc: *stop=0x8c074cfcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c074cfe; /* sts.l PR,@-r15 */
L8c074cfe: *stop=0x8c074cfeu;r[15]+=4294967280u;goto L8c074d00; /* add #-0x10,r15 */
L8c074d00: *stop=0x8c074d00u;r[0]=4294967200u;goto L8c074d02; /* mov #-0x60,r0 */
L8c074d02: *stop=0x8c074d02u;r[14]=r[5];goto L8c074d04; /* mov r5,r14 */
L8c074d04: *stop=0x8c074d04u;r[0]=r[0]&255u;goto L8c074d06; /* extu.b r0,r0 */
L8c074d06: *stop=0x8c074d06u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][13]=0;goto L8c074d08; /* fldi0 fr13 */
L8c074d08: *stop=0x8c074d08u;r[13]=r[7];goto L8c074d0a; /* mov r7,r13 */
L8c074d0a: *stop=0x8c074d0au;r[12]=r[4];goto L8c074d0c; /* mov r4,r12 */
L8c074d0c: *stop=0x8c074d0cu;r[4]=64u;goto L8c074d0e; /* mov #0x40,r4 */
L8c074d0e: *stop=0x8c074d0eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c074d10; /* mov.l r6,@r15 */
L8c074d10: *stop=0x8c074d10u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c074d12; /* fmov.s @(r0,r14),fr15 */
L8c074d12: *stop=0x8c074d12u;r[0]=8u;goto L8c074d14; /* mov #0x8,r0 */
L8c074d14: *stop=0x8c074d14u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,13u,r[0]+r[13]);if(io_status)return io_status;goto L8c074d16; /* fmov fr13,@(r0,r13) */
L8c074d16: *stop=0x8c074d16u;r[0]=96u;goto L8c074d18; /* mov #0x60,r0 */
L8c074d18: *stop=0x8c074d18u;r[4]<<=8;goto L8c074d1a; /* shll8 r4 */
L8c074d1a: *stop=0x8c074d1au;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,13u,r[13]);if(io_status)return io_status;goto L8c074d1c; /* fmov.s fr13,@r13 */
L8c074d1c: *stop=0x8c074d1cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c074d1e; /* mov.l @(r0,r14),r3 */
L8c074d1e: *stop=0x8c074d1eu;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c074d20; /* tst r4,r3 */
L8c074d20: if((*sr&1u))goto L8c074d36;goto L8c074d22; /* bt 0x8c074d36 */
L8c074d22: *stop=0x8c074d22u;r[0]=8u;goto L8c074d24; /* mov #0x8,r0 */
L8c074d24: *stop=0x8c074d24u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074d26; /* fldi0 fr3 */
L8c074d26: *stop=0x8c074d26u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c074d28; /* fmov.s @(r0,r12),fr2 */
L8c074d28: *stop=0x8c074d28u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074d2a; /* fcmp/gt fr3,fr2 */
L8c074d2a: if((*sr&1u))goto L8c074d36;goto L8c074d2c; /* bt 0x8c074d36 */
L8c074d2c: *stop=0x8c074d2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074de4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c074d2e; /* mov.l 0x8c074de4,r1 */
L8c074d2e: *stop=0x8c074d2eu;r[0]=4u;goto L8c074d30; /* mov #0x4,r0 */
L8c074d30: *stop=0x8c074d30u;*fpul=r[1];goto L8c074d32; /* lds r1,FPUL */
L8c074d32: *stop=0x8c074d34u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto L8c074d50; /* bra 0x8c074d50 */
L8c074d36: *stop=0x8c074d36u;r[0]=96u;goto L8c074d38; /* mov #0x60,r0 */
L8c074d38: *stop=0x8c074d38u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c074d3a; /* mov.l @(r0,r14),r2 */
L8c074d3a: *stop=0x8c074d3au;*sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c074d3c; /* tst r2,r4 */
L8c074d3c: if((*sr&1u))goto L8c074d5a;goto L8c074d3e; /* bt 0x8c074d5a */
L8c074d3e: *stop=0x8c074d3eu;r[0]=8u;goto L8c074d40; /* mov #0x8,r0 */
L8c074d40: *stop=0x8c074d40u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074d42; /* fldi0 fr3 */
L8c074d42: *stop=0x8c074d42u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c074d44; /* fmov.s @(r0,r12),fr2 */
L8c074d44: *stop=0x8c074d44u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074d46; /* fcmp/gt fr3,fr2 */
L8c074d46: if(!(*sr&1u))goto L8c074d5a;goto L8c074d48; /* bf 0x8c074d5a */
L8c074d48: *stop=0x8c074d48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074de8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c074d4a; /* mov.l 0x8c074de8,r1 */
L8c074d4a: *stop=0x8c074d4au;r[0]=4u;goto L8c074d4c; /* mov #0x4,r0 */
L8c074d4c: *stop=0x8c074d4cu;*fpul=r[1];goto L8c074d4e; /* lds r1,FPUL */
L8c074d4e: *stop=0x8c074d4eu;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto L8c074d50; /* fsts FPUL,fr3 */
L8c074d50: *stop=0x8c074d50u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[3],&f->fpscr,&fr[15]);if(status)return status;}goto L8c074d52; /* fmul fr3,fr15 */
L8c074d52: *stop=0x8c074d52u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074d54; /* fmov.s @(r0,r13),fr2 */
L8c074d54: *stop=0x8c074d54u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[15],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074d56; /* fsub fr15,fr2 */
L8c074d56: *stop=0x8c074d58u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074ed2; /* bra 0x8c074ed2 */
L8c074d5a: *stop=0x8c074d5au;r[0]=96u;goto L8c074d5c; /* mov #0x60,r0 */
L8c074d5c: *stop=0x8c074d5cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074decu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074d5e; /* mov.l 0x8c074dec,r3 */
L8c074d5e: *stop=0x8c074d5eu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c074d60; /* mov.l @(r0,r14),r2 */
L8c074d60: *stop=0x8c074d60u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c074d62; /* tst r3,r2 */
L8c074d62: if((*sr&1u))goto L8c074d68;goto L8c074d64; /* bt 0x8c074d68 */
L8c074d64: *stop=0x8c074d66u;goto L8c074eb6; /* bra 0x8c074eb6 */
L8c074d68: *stop=0x8c074d68u;r[0]=60u;goto L8c074d6a; /* mov #0x3c,r0 */
L8c074d6a: *stop=0x8c074d6au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074d6c; /* fldi0 fr3 */
L8c074d6c: *stop=0x8c074d6cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074d6e; /* fmov.s @(r0,r14),fr2 */
L8c074d6e: *stop=0x8c074d6eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074d70; /* fcmp/gt fr3,fr2 */
L8c074d70: if(!(*sr&1u))goto L8c074d76;goto L8c074d72; /* bf 0x8c074d76 */
L8c074d72: *stop=0x8c074d74u;goto L8c074ec0; /* bra 0x8c074ec0 */
L8c074d76: *stop=0x8c074d76u;r[0]=0x8c074df0u;goto L8c074d78; /* mova 0x8c074df0,r0 */
L8c074d78: *stop=0x8c074d78u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c074d7a; /* fmov.s @r0,fr14 */
L8c074d7a: *stop=0x8c074d7au;r[0]=96u;goto L8c074d7c; /* mov #0x60,r0 */
L8c074d7c: *stop=0x8c074d7cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c074d7e; /* mov.l @(r0,r14),r0 */
L8c074d7e: *stop=0x8c074d7eu;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c074d80; /* tst #0x8,r0 */
L8c074d80: if((*sr&1u))goto L8c074d8a;goto L8c074d82; /* bt 0x8c074d8a */
L8c074d82: *stop=0x8c074d82u;if(!valid_access(r[14]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x10u,2);if(io_status)return io_status;goto L8c074d84; /* mov.w @(0x10,r14),r0 */
L8c074d84: *stop=0x8c074d84u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c074d86; /* tst #0x3,r0 */
L8c074d86: *stop=0x8c074d88u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[14],&f->fpscr,&fr[15]);if(status)return status;}goto L8c074eb6; /* bra 0x8c074eb6 */
L8c074d8a: *stop=0x8c074d8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c074df8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074d8c; /* mov.l 0x8c074df8,r2 */
L8c074d8c: *stop=0x8c074d8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074df4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c074d8e; /* mov.l 0x8c074df4,r4 */
L8c074d8e: *stop=0x8c074d8eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c074d90; /* mov.l @r15,r5 */
L8c074d90: target=r[2];*pr=0x8c074d94u;*stop=0x8c074d92u;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074d94; /* jsr @r2 */
L8c074d94: *stop=0x8c074d94u;r[3]=32u;goto L8c074d96; /* mov #0x20,r3 */
L8c074d96: *stop=0x8c074d96u;if(!valid_access(r[14]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x10u,2);if(io_status)return io_status;goto L8c074d98; /* mov.w @(0x10,r14),r0 */
L8c074d98: *stop=0x8c074d98u;r[3]<<=8;goto L8c074d9a; /* shll8 r3 */
L8c074d9a: *stop=0x8c074d9au;fcopy(f,4u,0u);goto L8c074d9c; /* fmov fr0,fr4 */
L8c074d9c: *stop=0x8c074d9cu;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c074d9e; /* tst r3,r0 */
L8c074d9e: take=(*sr&1u);*stop=0x8c074da0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;if(take)goto L8c074e20;goto L8c074da2; /* bt/s 0x8c074e20 */
L8c074da2: *stop=0x8c074da2u;r[0]=0x8c074dfcu;goto L8c074da4; /* mova 0x8c074dfc,r0 */
L8c074da4: *stop=0x8c074da4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c074da6; /* fmov.s @r0,fr3 */
L8c074da6: *stop=0x8c074da6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074da8; /* fcmp/gt fr3,fr4 */
L8c074da8: if((*sr&1u))goto L8c074db2;goto L8c074daa; /* bt 0x8c074db2 */
L8c074daa: *stop=0x8c074daau;r[0]=0x8c074e00u;goto L8c074dac; /* mova 0x8c074e00,r0 */
L8c074dac: *stop=0x8c074dacu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c074dae; /* fmov.s @r0,fr2 */
L8c074dae: *stop=0x8c074db0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[2],&f->fpscr,&fr[15]);if(status)return status;}goto L8c074eb6; /* bra 0x8c074eb6 */
L8c074db2: *stop=0x8c074db2u;r[0]=0x8c074e04u;goto L8c074db4; /* mova 0x8c074e04,r0 */
L8c074db4: *stop=0x8c074db4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c074db6; /* fmov.s @r0,fr2 */
L8c074db6: *stop=0x8c074db6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c074db8; /* fcmp/gt fr2,fr4 */
L8c074db8: if((*sr&1u))goto L8c074dc0;goto L8c074dba; /* bt 0x8c074dc0 */
L8c074dba: *stop=0x8c074dbau;r[0]=0x8c074e08u;goto L8c074dbc; /* mova 0x8c074e08,r0 */
L8c074dbc: *stop=0x8c074dbeu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c074dc4; /* bra 0x8c074dc4 */
L8c074dc0: *stop=0x8c074dc0u;r[0]=0x8c074e0cu;goto L8c074dc2; /* mova 0x8c074e0c,r0 */
L8c074dc2: *stop=0x8c074dc2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c074dc4; /* fmov.s @r0,fr1 */
L8c074dc4: *stop=0x8c074dc6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[1],&f->fpscr,&fr[15]);if(status)return status;}goto L8c074eb6; /* bra 0x8c074eb6 */
L8c074e20: *stop=0x8c074e20u;r[0]=4294967248u;goto L8c074e22; /* mov #-0x30,r0 */
L8c074e22: *stop=0x8c074e22u;r[0]=r[0]&255u;goto L8c074e24; /* extu.b r0,r0 */
L8c074e24: *stop=0x8c074e24u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c074e26; /* fmov.s @(r0,r14),fr3 */
L8c074e26: *stop=0x8c074e26u;r[0]=56u;goto L8c074e28; /* mov #0x38,r0 */
L8c074e28: *stop=0x8c074e28u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074e2a; /* fmov.s @(r0,r14),fr2 */
L8c074e2a: *stop=0x8c074e2au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c074e2c; /* fcmp/gt fr2,fr3 */
L8c074e2c: if((*sr&1u))goto L8c074e34;goto L8c074e2e; /* bt 0x8c074e34 */
L8c074e2e: *stop=0x8c074e2eu;if(!valid_access(r[14]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x10u,2);if(io_status)return io_status;goto L8c074e30; /* mov.w @(0x10,r14),r0 */
L8c074e30: *stop=0x8c074e30u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c074e32; /* tst #0x3,r0 */
L8c074e32: if(!(*sr&1u))goto L8c074eb6;goto L8c074e34; /* bf 0x8c074eb6 */
L8c074e34: *stop=0x8c074e34u;r[0]=0x8c074f40u;goto L8c074e36; /* mova 0x8c074f40,r0 */
L8c074e36: *stop=0x8c074e36u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c074e38; /* fmov.s @r0,fr5 */
L8c074e38: *stop=0x8c074e38u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c074e3a; /* fcmp/gt fr5,fr4 */
L8c074e3a: if(!(*sr&1u))goto L8c074eb6;goto L8c074e3c; /* bf 0x8c074eb6 */
L8c074e3c: *stop=0x8c074e3cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c074e3e; /* fcmp/gt fr5,fr4 */
L8c074e3e: if(!(*sr&1u))goto L8c074e8c;goto L8c074e40; /* bf 0x8c074e8c */
L8c074e40: *stop=0x8c074e40u;r[0]=0x8c074f44u;goto L8c074e42; /* mova 0x8c074f44,r0 */
L8c074e42: *stop=0x8c074e42u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c074e44; /* fmov.s @r0,fr3 */
L8c074e44: *stop=0x8c074e44u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c074e46; /* fcmp/gt fr4,fr3 */
L8c074e46: if(!(*sr&1u))goto L8c074e8c;goto L8c074e48; /* bf 0x8c074e8c */
L8c074e48: *stop=0x8c074e48u;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074e4a; /* mov.l @(0x4,r12),r3 */
L8c074e4a: *stop=0x8c074e4au;r[2]=r[15];goto L8c074e4c; /* mov r15,r2 */
L8c074e4c: *stop=0x8c074e4cu;r[2]+=4u;goto L8c074e4e; /* add #0x4,r2 */
L8c074e4e: *stop=0x8c074e4eu;if(!valid_access(r[12]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c074e50; /* mov.l @(0x8,r12),r1 */
L8c074e50: *stop=0x8c074e50u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c074e52; /* mov.l r3,@r2 */
L8c074e52: *stop=0x8c074e52u;r[4]=r[15];goto L8c074e54; /* mov r15,r4 */
L8c074e54: *stop=0x8c074e54u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c074e56; /* mov.l r1,@(0x4,r2) */
L8c074e56: *stop=0x8c074e56u;if(!valid_access(r[12]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074e58; /* mov.l @(0xc,r12),r3 */
L8c074e58: *stop=0x8c074e58u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c074e5a; /* mov.l r3,@(0x8,r2) */
L8c074e5a: *stop=0x8c074e5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c074f48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074e5c; /* mov.l 0x8c074f48,r3 */
L8c074e5c: target=r[3];*pr=0x8c074e60u;*stop=0x8c074e5eu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074e60; /* jsr @r3 */
L8c074e60: *stop=0x8c074e60u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074e62; /* fldi0 fr3 */
L8c074e62: *stop=0x8c074e62u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c074e64; /* fcmp/eq fr3,fr0 */
L8c074e64: if(!(*sr&1u))goto L8c074e6c;goto L8c074e66; /* bf 0x8c074e6c */
L8c074e66: *stop=0x8c074e66u;r[0]=8u;goto L8c074e68; /* mov #0x8,r0 */
L8c074e68: *stop=0x8c074e6au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,13u,r[0]+r[15]);if(io_status)return io_status;goto L8c074e74; /* bra 0x8c074e74 */
L8c074e6c: *stop=0x8c074e6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074f4cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074e6e; /* mov.l 0x8c074f4c,r2 */
L8c074e6e: *stop=0x8c074e6eu;r[4]=r[15];goto L8c074e70; /* mov r15,r4 */
L8c074e70: target=r[2];*pr=0x8c074e74u;*stop=0x8c074e72u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074e74; /* jsr @r2 */
L8c074e74: *stop=0x8c074e74u;r[0]=8u;goto L8c074e76; /* mov #0x8,r0 */
L8c074e76: *stop=0x8c074e76u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074e78; /* fmov.s @(r0,r15),fr3 */
L8c074e78: *stop=0x8c074e78u;r[0]=0x8c074f50u;goto L8c074e7a; /* mova 0x8c074f50,r0 */
L8c074e7a: *stop=0x8c074e7au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c074e7c; /* fmov.s @r0,fr2 */
L8c074e7c: *stop=0x8c074e7cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]&=0x7fffffffu;goto L8c074e7e; /* fabs fr3 */
L8c074e7e: *stop=0x8c074e7eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074e80; /* fcmp/gt fr3,fr2 */
L8c074e80: if(!(*sr&1u))goto L8c074e88;goto L8c074e82; /* bf 0x8c074e88 */
L8c074e82: *stop=0x8c074e82u;r[0]=0x8c074f54u;goto L8c074e84; /* mova 0x8c074f54,r0 */
L8c074e84: *stop=0x8c074e86u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c074e9e; /* bra 0x8c074e9e */
L8c074e88: *stop=0x8c074e8au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[14],&f->fpscr,&fr[15]);if(status)return status;}goto L8c074eb6; /* bra 0x8c074eb6 */
L8c074e8c: *stop=0x8c074e8cu;r[0]=4294967236u;goto L8c074e8e; /* mov #-0x3c,r0 */
L8c074e8e: *stop=0x8c074e8eu;r[0]=r[0]&255u;goto L8c074e90; /* extu.b r0,r0 */
L8c074e90: *stop=0x8c074e90u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c074e92; /* fmov.s @(r0,r14),fr3 */
L8c074e92: *stop=0x8c074e92u;r[0]=56u;goto L8c074e94; /* mov #0x38,r0 */
L8c074e94: *stop=0x8c074e94u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074e96; /* fmov.s @(r0,r14),fr2 */
L8c074e96: *stop=0x8c074e96u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074e98; /* fcmp/gt fr3,fr2 */
L8c074e98: if((*sr&1u))goto L8c074ea2;goto L8c074e9a; /* bt 0x8c074ea2 */
L8c074e9a: *stop=0x8c074e9au;r[0]=0x8c074f58u;goto L8c074e9c; /* mova 0x8c074f58,r0 */
L8c074e9c: *stop=0x8c074e9cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c074e9e; /* fmov.s @r0,fr3 */
L8c074e9e: *stop=0x8c074ea0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[3],&f->fpscr,&fr[15]);if(status)return status;}goto L8c074eb6; /* bra 0x8c074eb6 */
L8c074ea2: *stop=0x8c074ea2u;if(!valid_access(r[14]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x10u,2);if(io_status)return io_status;goto L8c074ea4; /* mov.w @(0x10,r14),r0 */
L8c074ea4: *stop=0x8c074ea4u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c074ea6; /* tst #0x3,r0 */
L8c074ea6: if((*sr&1u))goto L8c074eb6;goto L8c074ea8; /* bt 0x8c074eb6 */
L8c074ea8: *stop=0x8c074ea8u;r[0]=4294967248u;goto L8c074eaa; /* mov #-0x30,r0 */
L8c074eaa: *stop=0x8c074eaau;r[0]=r[0]&255u;goto L8c074eac; /* extu.b r0,r0 */
L8c074eac: *stop=0x8c074eacu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c074eae; /* fmov.s @(r0,r14),fr3 */
L8c074eae: *stop=0x8c074eaeu;r[0]=56u;goto L8c074eb0; /* mov #0x38,r0 */
L8c074eb0: *stop=0x8c074eb0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074eb2; /* fmov.s @(r0,r14),fr2 */
L8c074eb2: *stop=0x8c074eb2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c074eb4; /* fdiv fr3,fr2 */
L8c074eb4: *stop=0x8c074eb4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[2],&f->fpscr,&fr[15]);if(status)return status;}goto L8c074eb6; /* fmul fr2,fr15 */
L8c074eb6: *stop=0x8c074eb6u;r[0]=4u;goto L8c074eb8; /* mov #0x4,r0 */
L8c074eb8: *stop=0x8c074eb8u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c074eba; /* fmov.s @(r0,r13),fr3 */
L8c074eba: *stop=0x8c074ebau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[15],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c074ebc; /* fsub fr15,fr3 */
L8c074ebc: *stop=0x8c074ebeu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c074ed2; /* bra 0x8c074ed2 */
L8c074ec0: *stop=0x8c074ec0u;r[2]=32u;goto L8c074ec2; /* mov #0x20,r2 */
L8c074ec2: *stop=0x8c074ec2u;if(!valid_access(r[14]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x10u,2);if(io_status)return io_status;goto L8c074ec4; /* mov.w @(0x10,r14),r0 */
L8c074ec4: *stop=0x8c074ec4u;r[2]<<=8;goto L8c074ec6; /* shll8 r2 */
L8c074ec6: *stop=0x8c074ec6u;*sr=(*sr&~1u)|!(r[2]&r[0]);goto L8c074ec8; /* tst r2,r0 */
L8c074ec8: if((*sr&1u))goto L8c074ed2;goto L8c074eca; /* bt 0x8c074ed2 */
L8c074eca: *stop=0x8c074ecau;r[0]=4u;goto L8c074ecc; /* mov #0x4,r0 */
L8c074ecc: *stop=0x8c074eccu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074ece; /* fmov.s @(r0,r13),fr2 */
L8c074ece: *stop=0x8c074eceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[15],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074ed0; /* fsub fr15,fr2 */
L8c074ed0: *stop=0x8c074ed0u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074ed2; /* fmov fr2,@(r0,r13) */
L8c074ed2: *stop=0x8c074ed2u;r[15]+=16u;goto L8c074ed4; /* add #0x10,r15 */
L8c074ed4: *stop=0x8c074ed4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c074ed6; /* lds.l @r15+,PR */
L8c074ed6: *stop=0x8c074ed6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c074ed8; /* fmov.s @r15+,fr13 */
L8c074ed8: *stop=0x8c074ed8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c074eda; /* fmov.s @r15+,fr14 */
L8c074eda: *stop=0x8c074edau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c074edc; /* fmov.s @r15+,fr15 */
L8c074edc: *stop=0x8c074edcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c074ede; /* mov.l @r15+,r12 */
L8c074ede: *stop=0x8c074edeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c074ee0; /* mov.l @r15+,r13 */
L8c074ee0: *stop=0x8c074ee0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c074ee2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_vectors_8c074cf0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c074cf0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c074d94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c074cf0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074d94u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c074e60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c074cf0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074e60u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c074e74(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c074cf0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074e74u);}

static int sa_movement_vectors_run_8c078eb0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c078ed2u)goto L8c078ed2;if(resume_pc==0x8c078ee2u)goto L8c078ee2;if(resume_pc==0x8c078ef2u)goto L8c078ef2;if(resume_pc==0x8c078f04u)goto L8c078f04;if(resume_pc==0x8c078f10u)goto L8c078f10;if(resume_pc==0x8c078f16u)goto L8c078f16;goto L8c078eb0;
L8c078eb0: *stop=0x8c078eb0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c078eb2; /* mov.l r14,@-r15 */
L8c078eb2: *stop=0x8c078eb2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c078eb4; /* sts.l PR,@-r15 */
L8c078eb4: *stop=0x8c078eb4u;r[15]+=4294967276u;goto L8c078eb6; /* add #-0x14,r15 */
L8c078eb6: *stop=0x8c078eb6u;r[14]=r[4];goto L8c078eb8; /* mov r4,r14 */
L8c078eb8: *stop=0x8c078eb8u;r[4]=r[5];goto L8c078eba; /* mov r5,r4 */
L8c078eba: *stop=0x8c078ebau;r[0]=8u;goto L8c078ebc; /* mov #0x8,r0 */
L8c078ebc: *stop=0x8c078ebcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fc0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078ebe; /* mov.l 0x8c078fc0,r3 */
L8c078ebe: *stop=0x8c078ebeu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c078ec0; /* mov.l r5,@(0x4,r15) */
L8c078ec0: *stop=0x8c078ec0u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c078ec2; /* fmov.s @r4+,fr3 */
L8c078ec2: *stop=0x8c078ec2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078ec4; /* fmov fr3,@(r0,r15) */
L8c078ec4: *stop=0x8c078ec4u;r[0]=12u;goto L8c078ec6; /* mov #0xc,r0 */
L8c078ec6: *stop=0x8c078ec6u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c078ec8; /* fmov.s @r4+,fr3 */
L8c078ec8: *stop=0x8c078ec8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078eca; /* fmov fr3,@(r0,r15) */
L8c078eca: *stop=0x8c078ecau;r[0]=16u;goto L8c078ecc; /* mov #0x10,r0 */
L8c078ecc: *stop=0x8c078eccu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c078ece; /* fmov.s @r4,fr3 */
L8c078ece: target=r[3];*pr=0x8c078ed2u;*stop=0x8c078ed0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078ed2; /* jsr @r3 */
L8c078ed2: *stop=0x8c078ed2u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c078ed4; /* mov.l @(0x1c,r14),r2 */
L8c078ed4: *stop=0x8c078ed4u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c078ed6; /* tst r2,r2 */
L8c078ed6: take=(*sr&1u);*stop=0x8c078ed8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c078ee2;goto L8c078eda; /* bt/s 0x8c078ee2 */
L8c078eda: *stop=0x8c078edau;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078edc; /* mov.l 0x8c078fcc,r3 */
L8c078edc: *stop=0x8c078edcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c078ede; /* mov.l @r15,r5 */
L8c078ede: target=r[3];*pr=0x8c078ee2u;*stop=0x8c078ee0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078ee2; /* jsr @r3 */
L8c078ee2: *stop=0x8c078ee2u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078ee4; /* mov.l @(0x14,r14),r3 */
L8c078ee4: *stop=0x8c078ee4u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c078ee6; /* tst r3,r3 */
L8c078ee6: take=(*sr&1u);*stop=0x8c078ee8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c078ef2;goto L8c078eea; /* bt/s 0x8c078ef2 */
L8c078eea: *stop=0x8c078eeau;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fc8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078eec; /* mov.l 0x8c078fc8,r3 */
L8c078eec: *stop=0x8c078eecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c078eee; /* mov.l @r15,r5 */
L8c078eee: target=r[3];*pr=0x8c078ef2u;*stop=0x8c078ef0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078ef2; /* jsr @r3 */
L8c078ef2: *stop=0x8c078ef2u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078ef4; /* mov.l @(0x18,r14),r3 */
L8c078ef4: *stop=0x8c078ef4u;r[3]=0u-r[3];goto L8c078ef6; /* neg r3,r3 */
L8c078ef6: *stop=0x8c078ef6u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c078ef8; /* tst r3,r3 */
L8c078ef8: take=(*sr&1u);*stop=0x8c078efau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c078f04;goto L8c078efc; /* bt/s 0x8c078f04 */
L8c078efc: *stop=0x8c078efcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fc4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c078efe; /* mov.l 0x8c078fc4,r1 */
L8c078efe: *stop=0x8c078efeu;r[5]=r[3];goto L8c078f00; /* mov r3,r5 */
L8c078f00: target=r[1];*pr=0x8c078f04u;*stop=0x8c078f02u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078f04; /* jsr @r1 */
L8c078f04: *stop=0x8c078f04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fd0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078f06; /* mov.l 0x8c078fd0,r3 */
L8c078f06: *stop=0x8c078f06u;r[5]=r[15];goto L8c078f08; /* mov r15,r5 */
L8c078f08: *stop=0x8c078f08u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c078f0a; /* mov.l @(0x4,r15),r6 */
L8c078f0a: *stop=0x8c078f0au;r[5]+=8u;goto L8c078f0c; /* add #0x8,r5 */
L8c078f0c: target=r[3];*pr=0x8c078f10u;*stop=0x8c078f0eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078f10; /* jsr @r3 */
L8c078f10: *stop=0x8c078f10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fd4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c078f12; /* mov.l 0x8c078fd4,r2 */
L8c078f12: target=r[2];*pr=0x8c078f16u;*stop=0x8c078f14u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078f16; /* jsr @r2 */
L8c078f16: *stop=0x8c078f16u;r[15]+=20u;goto L8c078f18; /* add #0x14,r15 */
L8c078f18: *stop=0x8c078f18u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c078f1a; /* lds.l @r15+,PR */
L8c078f1a: *stop=0x8c078f1au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c078f1cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_vectors_8c078eb0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c078eb0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c078ed2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c078eb0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078ed2u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c078ee2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c078eb0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078ee2u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c078ef2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c078eb0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078ef2u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c078f04(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c078eb0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078f04u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c078f10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c078eb0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078f10u);}
SA_STAGE_EXPORT int sa_movement_vectors_resume_8c078f16(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_vectors_run_8c078eb0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078f16u);}

SA_STAGE_EXPORT int sa_movement_vectors_8c639090(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c639090;
L8c639090: *stop=0x8c639090u;r[0]=4u;goto L8c639092; /* mov #0x4,r0 */
L8c639092: *stop=0x8c639092u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c639094; /* fmov.s @r4,fr0 */
L8c639094: *stop=0x8c639094u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;goto L8c639096; /* fmov.s @r5,fr4 */
L8c639096: *stop=0x8c639096u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c639098; /* fmov.s @(r0,r4),fr1 */
L8c639098: *stop=0x8c639098u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c63909a; /* fmov.s @(r0,r5),fr5 */
L8c63909a: *stop=0x8c63909au;r[0]=8u;goto L8c63909c; /* mov #0x8,r0 */
L8c63909c: *stop=0x8c63909cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0;goto L8c63909e; /* fldi0 fr7 */
L8c63909e: *stop=0x8c63909eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6390a0; /* fmov.s @(r0,r4),fr2 */
L8c6390a0: *stop=0x8c6390a0u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[5]);if(io_status)return io_status;goto L8c6390a2; /* fmov.s @(r0,r5),fr6 */
L8c6390a2: *stop=0x8c6390a2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6390a4; /* fldi0 fr3 */
L8c6390a4: *stop=0x8c6390a4u;status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c6390a6; /* fipr fv4,fv0 */
L8c6390a6: *stop=0x8c6390a6u;if(*pr!=entry_pr)return -3;*stop=0x8c6390a8u;fcopy(f,0u,3u);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movement_vectors_8c057b40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c057b40;
L8c057b40: *stop=0x8c057b40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c057b50u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c057b42; /* mov.l 0x8c057b50,r1 */
L8c057b42: *stop=0x8c057b42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c057b54u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c057b44; /* mov.l 0x8c057b54,r6 */
L8c057b44: *stop=0x8c057b44u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c057b46; /* mov.l @r1,r3 */
L8c057b46: *stop=0x8c057b46u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c057b48; /* mov.l @r6,r2 */
L8c057b48: *stop=0x8c057b48u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c057b4a; /* cmp/ge r3,r2 */
L8c057b4a: if(!(*sr&1u))goto L8c057b58;goto L8c057b4c; /* bf 0x8c057b58 */
L8c057b4c: *stop=0x8c057b4cu;if(*pr!=entry_pr)return -3;*stop=0x8c057b4eu;r[0]=0u;return 0; /* rts */
L8c057b58: *stop=0x8c057b58u;r[2]+=1u;goto L8c057b5a; /* add #0x1,r2 */
L8c057b5a: *stop=0x8c057b5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c057bacu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c057b5c; /* mov.l 0x8c057bac,r1 */
L8c057b5c: *stop=0x8c057b5cu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c057b5e; /* mov.l r2,@r6 */
L8c057b5e: *stop=0x8c057b5eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[5]=loaded;}goto L8c057b60; /* mov.l @r1,r5 */
L8c057b60: *stop=0x8c057b60u;r[5]+=64u;goto L8c057b62; /* add #0x40,r5 */
L8c057b62: *stop=0x8c057b62u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c057b64; /* mov.l r5,@r1 */
L8c057b64: *stop=0x8c057b64u;r[4]=r[5];goto L8c057b66; /* mov r5,r4 */
L8c057b66: *stop=0x8c057b66u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;goto L8c057b68; /* fldi1 fr0 */
L8c057b68: *stop=0x8c057b68u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0;goto L8c057b6a; /* fldi0 fr1 */
L8c057b6a: *stop=0x8c057b6au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c057b6c; /* fldi0 fr2 */
L8c057b6c: *stop=0x8c057b6cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c057b6e; /* fldi1 fr3 */
L8c057b6e: *stop=0x8c057b6eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c057b70; /* fldi0 fr4 */
L8c057b70: *stop=0x8c057b70u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0;goto L8c057b72; /* fldi0 fr5 */
L8c057b72: *stop=0x8c057b72u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c057b74; /* fschg */
L8c057b74: *stop=0x8c057b74u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[5]);if(io_status)return io_status;goto L8c057b76; /* fmov.s fr15,@-r5 */
L8c057b76: *stop=0x8c057b76u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[5]);if(io_status)return io_status;goto L8c057b78; /* fmov.s fr13,@-r5 */
L8c057b78: *stop=0x8c057b78u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;goto L8c057b7a; /* fmov.s fr11,@-r5 */
L8c057b7a: *stop=0x8c057b7au;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;goto L8c057b7c; /* fmov.s fr9,@-r5 */
L8c057b7c: *stop=0x8c057b7cu;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;goto L8c057b7e; /* fmov.s fr7,@-r5 */
L8c057b7e: *stop=0x8c057b7eu;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;goto L8c057b80; /* fmov.s fr5,@-r5 */
L8c057b80: *stop=0x8c057b80u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c057b82; /* fmov.s fr3,@-r5 */
L8c057b82: *stop=0x8c057b82u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;goto L8c057b84; /* fmov.s fr1,@-r5 */
L8c057b84: *stop=0x8c057b84u;r[4]+=64u;goto L8c057b86; /* add #0x40,r4 */
L8c057b86: *stop=0x8c057b86u;fcopy(f,1u,0u);goto L8c057b88; /* fmov fr0,fr1 */
L8c057b88: *stop=0x8c057b88u;fcopy(f,3u,4u);goto L8c057b8a; /* fmov fr4,fr3 */
L8c057b8a: *stop=0x8c057b8au;fcopy(f,5u,2u);goto L8c057b8c; /* fmov fr2,fr5 */
L8c057b8c: *stop=0x8c057b8cu;fcopy(f,7u,4u);goto L8c057b8e; /* fmov fr4,fr7 */
L8c057b8e: *stop=0x8c057b8eu;fcopy(f,9u,4u);goto L8c057b90; /* fmov fr4,fr9 */
L8c057b90: *stop=0x8c057b90u;fcopy(f,11u,0u);goto L8c057b92; /* fmov fr0,fr11 */
L8c057b92: *stop=0x8c057b92u;fcopy(f,13u,4u);goto L8c057b94; /* fmov fr4,fr13 */
L8c057b94: *stop=0x8c057b94u;fcopy(f,15u,2u);goto L8c057b96; /* fmov fr2,fr15 */
L8c057b96: *stop=0x8c057b96u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;goto L8c057b98; /* fmov.s fr15,@-r4 */
L8c057b98: *stop=0x8c057b98u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;goto L8c057b9a; /* fmov.s fr13,@-r4 */
L8c057b9a: *stop=0x8c057b9au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;goto L8c057b9c; /* fmov.s fr11,@-r4 */
L8c057b9c: *stop=0x8c057b9cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c057b9e; /* fmov.s fr9,@-r4 */
L8c057b9e: *stop=0x8c057b9eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c057ba0; /* fmov.s fr7,@-r4 */
L8c057ba0: *stop=0x8c057ba0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c057ba2; /* fmov.s fr5,@-r4 */
L8c057ba2: *stop=0x8c057ba2u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c057ba4; /* fmov.s fr3,@-r4 */
L8c057ba4: *stop=0x8c057ba4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c057ba6; /* fmov.s fr1,@-r4 */
L8c057ba6: *stop=0x8c057ba6u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c057ba8; /* fschg */
L8c057ba8: *stop=0x8c057ba8u;if(*pr!=entry_pr)return -3;*stop=0x8c057baau;r[0]=1u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movement_vectors_8c638f28(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c638f28;
L8c638f28: *stop=0x8c638f28u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c638f2a; /* fmov.s @r5+,fr0 */
L8c638f2a: *stop=0x8c638f2au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c638f2c; /* tst r4,r4 */
L8c638f2c: *stop=0x8c638f2cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c638f2e; /* fmov.s @r5+,fr1 */
L8c638f2e: *stop=0x8c638f2eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c638f30; /* fmov.s @r5+,fr2 */
L8c638f30: take=!(*sr&1u);*stop=0x8c638f32u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;if(take)goto L8c638f38;goto L8c638f34; /* bf/s 0x8c638f38 */
L8c638f34: *stop=0x8c638f36u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c638f6e; /* bra 0x8c638f6e */
L8c638f38: *stop=0x8c638f38u;r[0]=16u;goto L8c638f3a; /* mov #0x10,r0 */
L8c638f3a: *stop=0x8c638f3au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c638f3c; /* fmov.s @r4,fr4 */
L8c638f3c: *stop=0x8c638f3cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c638f3e; /* fmov.s @(r0,r4),fr5 */
L8c638f3e: *stop=0x8c638f3eu;r[0]=32u;goto L8c638f40; /* mov #0x20,r0 */
L8c638f40: *stop=0x8c638f40u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[4]);if(io_status)return io_status;goto L8c638f42; /* fmov.s @(r0,r4),fr6 */
L8c638f42: *stop=0x8c638f42u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0;goto L8c638f44; /* fldi0 fr7 */
L8c638f44: *stop=0x8c638f44u;status=sa_sh4_vector_numeric(0xf4edu,f,*fpul);if(status)return status;goto L8c638f46; /* fipr fv0,fv4 */
L8c638f46: *stop=0x8c638f46u;r[0]=4u;goto L8c638f48; /* mov #0x4,r0 */
L8c638f48: *stop=0x8c638f48u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[0]+r[4]);if(io_status)return io_status;goto L8c638f4a; /* fmov.s @(r0,r4),fr8 */
L8c638f4a: *stop=0x8c638f4au;r[0]=20u;goto L8c638f4c; /* mov #0x14,r0 */
L8c638f4c: *stop=0x8c638f4cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[4]);if(io_status)return io_status;goto L8c638f4e; /* fmov.s @(r0,r4),fr9 */
L8c638f4e: *stop=0x8c638f4eu;r[0]=36u;goto L8c638f50; /* mov #0x24,r0 */
L8c638f50: *stop=0x8c638f50u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[0]+r[4]);if(io_status)return io_status;goto L8c638f52; /* fmov.s @(r0,r4),fr10 */
L8c638f52: *stop=0x8c638f52u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]=0;goto L8c638f54; /* fldi0 fr11 */
L8c638f54: *stop=0x8c638f54u;status=sa_sh4_vector_numeric(0xf8edu,f,*fpul);if(status)return status;goto L8c638f56; /* fipr fv0,fv8 */
L8c638f56: *stop=0x8c638f56u;fcopy(f,10u,7u);goto L8c638f58; /* fmov fr7,fr10 */
L8c638f58: *stop=0x8c638f58u;r[0]=8u;goto L8c638f5a; /* mov #0x8,r0 */
L8c638f5a: *stop=0x8c638f5au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c638f5c; /* fmov.s @(r0,r4),fr4 */
L8c638f5c: *stop=0x8c638f5cu;r[0]=24u;goto L8c638f5e; /* mov #0x18,r0 */
L8c638f5e: *stop=0x8c638f5eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c638f60; /* fmov.s @(r0,r4),fr5 */
L8c638f60: *stop=0x8c638f60u;r[0]=40u;goto L8c638f62; /* mov #0x28,r0 */
L8c638f62: *stop=0x8c638f62u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[4]);if(io_status)return io_status;goto L8c638f64; /* fmov.s @(r0,r4),fr6 */
L8c638f64: *stop=0x8c638f64u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0;goto L8c638f66; /* fldi0 fr7 */
L8c638f66: *stop=0x8c638f66u;status=sa_sh4_vector_numeric(0xf4edu,f,*fpul);if(status)return status;goto L8c638f68; /* fipr fv0,fv4 */
L8c638f68: *stop=0x8c638f68u;fcopy(f,0u,10u);goto L8c638f6a; /* fmov fr10,fr0 */
L8c638f6a: *stop=0x8c638f6au;fcopy(f,1u,11u);goto L8c638f6c; /* fmov fr11,fr1 */
L8c638f6c: *stop=0x8c638f6cu;fcopy(f,2u,7u);goto L8c638f6e; /* fmov fr7,fr2 */
L8c638f6e: *stop=0x8c638f6eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c638f70; /* fldi0 fr3 */
L8c638f70: *stop=0x8c638f70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c638f8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c638f72; /* mov.l 0x8c638f8c,r3 */
L8c638f72: *stop=0x8c638f72u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c638f74; /* mov.l @r3,r3 */
L8c638f74: *stop=0x8c638f74u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c638f76; /* tst r3,r3 */
L8c638f76: take=(*sr&1u);*stop=0x8c638f78u;r[6]+=12u;if(take)goto L8c638f84;goto L8c638f7a; /* bt/s 0x8c638f84 */
L8c638f7a: *stop=0x8c638f7au;status=sa_sh4_vector_numeric(0xf0edu,f,*fpul);if(status)return status;goto L8c638f7c; /* fipr fv0,fv0 */
L8c638f7c: *stop=0x8c638f7cu;status=sa_sh4_vector_numeric(0xf37du,f,*fpul);if(status)return status;goto L8c638f7e; /* fsrra fr3 */
L8c638f7e: *stop=0x8c638f7eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c638f80; /* fmul fr3,fr2 */
L8c638f80: *stop=0x8c638f80u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c638f82; /* fmul fr3,fr1 */
L8c638f82: *stop=0x8c638f82u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c638f84; /* fmul fr3,fr0 */
L8c638f84: *stop=0x8c638f84u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c638f86; /* fmov.s fr2,@-r6 */
L8c638f86: *stop=0x8c638f86u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[6]);if(io_status)return io_status;goto L8c638f88; /* fmov.s fr1,@-r6 */
L8c638f88: *stop=0x8c638f88u;if(*pr!=entry_pr)return -3;*stop=0x8c638f8au;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;return 0; /* rts */
}
