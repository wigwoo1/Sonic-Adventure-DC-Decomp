/* Original renderer release ownership family.
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
static int sa_renderer_context_owner_run_8c6052e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c6052eau)goto L8c6052ea;goto L8c6052e0;
L8c6052e0: *stop=0x8c6052e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6052e2; /* sts.l PR,@-r15 */
L8c6052e2: *stop=0x8c6052e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6053a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6052e4; /* mov.l 0x8c6053a0,r3 */
L8c6052e4: *stop=0x8c6052e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60539cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6052e6; /* mov.l 0x8c60539c,r4 */
L8c6052e6: target=r[3];*pr=0x8c6052eau;*stop=0x8c6052e8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6052ea; /* jsr @r3 */
L8c6052ea: *stop=0x8c6052eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6053a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6052ec; /* mov.l 0x8c6053a4,r3 */
L8c6052ec: *stop=0x8c6052ecu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6052ee; /* mov.l @r3,r2 */
L8c6052ee: *stop=0x8c6052eeu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6052f0; /* tst r2,r2 */
L8c6052f0: if(!(*sr&1u))goto L8c605300;goto L8c6052f2; /* bf 0x8c605300 */
L8c6052f2: *stop=0x8c6052f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6053acu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6052f4; /* mov.l 0x8c6053ac,r1 */
L8c6052f4: *stop=0x8c6052f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6053a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6052f6; /* mov.l 0x8c6053a8,r2 */
L8c6052f6: *stop=0x8c6052f6u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6052f8; /* mov.l @r1,r4 */
L8c6052f8: *stop=0x8c6052f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6053b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6052fa; /* mov.l 0x8c6053b0,r3 */
L8c6052fa: *stop=0x8c6052fau;r[4]&=r[2];goto L8c6052fc; /* and r2,r4 */
L8c6052fc: target=r[3];*stop=0x8c6052feu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c605300: *stop=0x8c605300u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c605302; /* lds.l @r15+,PR */
L8c605302: *stop=0x8c605302u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c605304u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_renderer_context_owner_8c6052e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_renderer_context_owner_run_8c6052e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_renderer_context_owner_resume_8c6052ea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_renderer_context_owner_run_8c6052e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6052eau);}

static int sa_renderer_context_owner_run_8c6567f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c656800u)goto L8c656800;goto L8c6567f2;
L8c6567f2: *stop=0x8c6567f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656848u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6567f4; /* mov.l 0x8c656848,r5 */
L8c6567f4: *stop=0x8c6567f4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6567f6; /* sts.l PR,@-r15 */
L8c6567f6: *stop=0x8c6567f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656850u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6567f8; /* mov.l 0x8c656850,r3 */
L8c6567f8: *stop=0x8c6567f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65684cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6567fa; /* mov.l 0x8c65684c,r4 */
L8c6567fa: *stop=0x8c6567fau;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6567fc; /* mov.l @r5,r5 */
L8c6567fc: target=r[3];*pr=0x8c656800u;*stop=0x8c6567feu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656800; /* jsr @r3 */
L8c656800: *stop=0x8c656800u;r[4]=r[0];goto L8c656802; /* mov r0,r4 */
L8c656802: *stop=0x8c656802u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c656804; /* tst r4,r4 */
L8c656804: if((*sr&1u))goto L8c65680c;goto L8c656806; /* bt 0x8c65680c */
L8c656806: *stop=0x8c656806u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c656808; /* lds.l @r15+,PR */
L8c656808: *stop=0x8c656808u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c65680au;r[0]=r[4];if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c65680c: *stop=0x8c65680cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656854u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65680e; /* mov.l 0x8c656854,r6 */
L8c65680e: *stop=0x8c65680eu;r[5]=r[6];goto L8c656810; /* mov r6,r5 */
L8c656810: *stop=0x8c656810u;r[5]+=20u;goto L8c656812; /* add #0x14,r5 */
L8c656812: *stop=0x8c656812u;r[4]=r[6];goto L8c656814; /* mov r6,r4 */
L8c656814: *stop=0x8c656816u;r[6]=0u;goto L8c65681c; /* bra 0x8c65681c */
L8c656818: *stop=0x8c656818u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[6],4);if(io_status)return io_status;goto L8c65681a; /* mov.l r6,@r4 */
L8c65681a: *stop=0x8c65681au;r[4]+=4u;goto L8c65681c; /* add #0x4,r4 */
L8c65681c: *stop=0x8c65681cu;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c65681e; /* cmp/hs r5,r4 */
L8c65681e: if(!(*sr&1u))goto L8c656818;goto L8c656820; /* bf 0x8c656818 */
L8c656820: *stop=0x8c656820u;r[0]=0u;goto L8c656822; /* mov #0x0,r0 */
L8c656822: *stop=0x8c656822u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c656824; /* lds.l @r15+,PR */
L8c656824: *stop=0x8c656824u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c656826u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_renderer_context_owner_8c6567f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_renderer_context_owner_run_8c6567f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_renderer_context_owner_resume_8c656800(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_renderer_context_owner_run_8c6567f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c656800u);}

static int sa_renderer_context_owner_run_8c663920(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c66392eu)goto L8c66392e;if(resume_pc==0x8c663934u)goto L8c663934;if(resume_pc==0x8c663940u)goto L8c663940;if(resume_pc==0x8c663946u)goto L8c663946;goto L8c663920;
L8c663920: *stop=0x8c663920u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c663922; /* sts.l PR,@-r15 */
L8c663922: *stop=0x8c663922u;r[15]+=4294967288u;goto L8c663924; /* add #-0x8,r15 */
L8c663924: *stop=0x8c663924u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663a40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663926; /* mov.l 0x8c663a40,r3 */
L8c663926: *stop=0x8c663926u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c663928; /* mov.l r4,@r15 */
L8c663928: *stop=0x8c663928u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c66392a; /* mov.l r5,@(0x4,r15) */
L8c66392a: target=r[3];*pr=0x8c66392eu;*stop=0x8c66392cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66392e; /* jsr @r3 */
L8c66392e: *stop=0x8c66392eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c663930; /* mov.l r0,@r15 */
L8c663930: target=0x8c66385au;*pr=0x8c663934u;*stop=0x8c663932u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663934; /* bsr 0x8c66385a */
L8c663934: *stop=0x8c663934u;r[4]=r[0];goto L8c663936; /* mov r0,r4 */
L8c663936: *stop=0x8c663936u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c663938; /* tst r4,r4 */
L8c663938: if((*sr&1u))goto L8c663948;goto L8c66393a; /* bt 0x8c663948 */
L8c66393a: *stop=0x8c66393au;{uint32_t loaded=rd(m,bus,&io_status,0x8c663a40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66393c; /* mov.l 0x8c663a40,r3 */
L8c66393c: target=r[3];*pr=0x8c663940u;*stop=0x8c66393eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663940; /* jsr @r3 */
L8c663940: *stop=0x8c663940u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c663942; /* mov.l r0,@r15 */
L8c663942: target=0x8c66385au;*pr=0x8c663946u;*stop=0x8c663944u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663946; /* bsr 0x8c66385a */
L8c663946: *stop=0x8c663946u;r[4]=r[0];goto L8c663948; /* mov r0,r4 */
L8c663948: *stop=0x8c663948u;r[15]+=8u;goto L8c66394a; /* add #0x8,r15 */
L8c66394a: *stop=0x8c66394au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66394c; /* lds.l @r15+,PR */
L8c66394c: *stop=0x8c66394cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c66394eu;r[0]=r[4];if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_renderer_context_owner_8c663920(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_renderer_context_owner_run_8c663920(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_renderer_context_owner_resume_8c66392e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_renderer_context_owner_run_8c663920(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c66392eu);}
SA_STAGE_EXPORT int sa_renderer_context_owner_resume_8c663934(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_renderer_context_owner_run_8c663920(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c663934u);}
SA_STAGE_EXPORT int sa_renderer_context_owner_resume_8c663940(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_renderer_context_owner_run_8c663920(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c663940u);}
SA_STAGE_EXPORT int sa_renderer_context_owner_resume_8c663946(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_renderer_context_owner_run_8c663920(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c663946u);}

SA_STAGE_EXPORT int sa_renderer_context_owner_8c665660(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c665660;
L8c665660: *stop=0x8c665660u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6656b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665662; /* mov.l 0x8c6656b0,r3 */
L8c665662: *stop=0x8c665662u;r[5]=0u;goto L8c665664; /* mov #0x0,r5 */
L8c665664: *stop=0x8c665664u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6656acu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c665666; /* mov.l 0x8c6656ac,r6 */
L8c665666: *stop=0x8c665666u;*sr=(*sr&~1u)|(r[4]>=r[3]);goto L8c665668; /* cmp/hs r3,r4 */
L8c665668: take=(*sr&1u);*stop=0x8c66566au;r[7]=3u;if(take)goto L8c665682;goto L8c66566c; /* bt/s 0x8c665682 */
L8c66566c: *stop=0x8c66566cu;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c66566e; /* cmp/hs r6,r4 */
L8c66566e: if(!(*sr&1u))goto L8c665674;goto L8c665670; /* bf 0x8c665674 */
L8c665670: *stop=0x8c665670u;r[4]-=r[6];goto L8c665672; /* sub r6,r4 */
L8c665672: *stop=0x8c665672u;r[5]=4u;goto L8c665674; /* mov #0x4,r5 */
L8c665674: *stop=0x8c665674u;r[6]=4294967292u;goto L8c665676; /* mov #-0x4,r6 */
L8c665676: *stop=0x8c665676u;r[6]&=r[4];goto L8c665678; /* and r4,r6 */
L8c665678: *stop=0x8c665678u;r[7]&=r[4];goto L8c66567a; /* and r4,r7 */
L8c66567a: *stop=0x8c66567au;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c66567c; /* shll r6 */
L8c66567c: *stop=0x8c66567cu;r[6]+=r[7];goto L8c66567e; /* add r7,r6 */
L8c66567e: *stop=0x8c665680u;r[6]+=r[5];goto L8c66569c; /* bra 0x8c66569c */
L8c665682: *stop=0x8c665682u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6656b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665684; /* mov.l 0x8c6656b4,r2 */
L8c665684: *stop=0x8c665684u;*sr=(*sr&~1u)|(r[4]>=r[2]);goto L8c665686; /* cmp/hs r2,r4 */
L8c665686: if(!(*sr&1u))goto L8c66568c;goto L8c665688; /* bf 0x8c66568c */
L8c665688: *stop=0x8c665688u;r[4]-=r[6];goto L8c66568a; /* sub r6,r4 */
L8c66568a: *stop=0x8c66568au;r[5]=4u;goto L8c66568c; /* mov #0x4,r5 */
L8c66568c: *stop=0x8c66568cu;r[6]=4294967292u;goto L8c66568e; /* mov #-0x4,r6 */
L8c66568e: *stop=0x8c66568eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6656b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665690; /* mov.l 0x8c6656b8,r3 */
L8c665690: *stop=0x8c665690u;r[6]&=r[4];goto L8c665692; /* and r4,r6 */
L8c665692: *stop=0x8c665692u;r[7]&=r[4];goto L8c665694; /* and r4,r7 */
L8c665694: *stop=0x8c665694u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c665696; /* shll r6 */
L8c665696: *stop=0x8c665696u;r[6]+=r[7];goto L8c665698; /* add r7,r6 */
L8c665698: *stop=0x8c665698u;r[6]+=r[5];goto L8c66569a; /* add r5,r6 */
L8c66569a: *stop=0x8c66569au;r[6]+=r[3];goto L8c66569c; /* add r3,r6 */
L8c66569c: *stop=0x8c66569cu;if(*pr!=entry_pr)return -3;*stop=0x8c66569eu;r[0]=r[6];return 0; /* rts */
}
