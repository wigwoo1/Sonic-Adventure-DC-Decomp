/* Original complete texture resource loading and decoding family.
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
static int sa_player_texture_load_run_8c0990de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c099116u)goto L8c099116;if(resume_pc==0x8c09911eu)goto L8c09911e;if(resume_pc==0x8c09913cu)goto L8c09913c;if(resume_pc==0x8c099154u)goto L8c099154;if(resume_pc==0x8c09915eu)goto L8c09915e;if(resume_pc==0x8c0991c4u)goto L8c0991c4;goto L8c0990de;
L8c0990de: *stop=0x8c0990deu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0990e0; /* mov.l r14,@-r15 */
L8c0990e0: *stop=0x8c0990e0u;r[14]=r[4];goto L8c0990e2; /* mov r4,r14 */
L8c0990e2: *stop=0x8c0990e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0990e4; /* mov.l r13,@-r15 */
L8c0990e4: *stop=0x8c0990e4u;r[14]<<=2;goto L8c0990e6; /* shll2 r14 */
L8c0990e6: *stop=0x8c0990e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0990e8; /* mov.l r12,@-r15 */
L8c0990e8: *stop=0x8c0990e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0990ea; /* mov.l r11,@-r15 */
L8c0990ea: *stop=0x8c0990eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c0990ec; /* mov.l r10,@-r15 */
L8c0990ec: *stop=0x8c0990ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c099190u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0990ee; /* mov.l 0x8c099190,r0 */
L8c0990ee: *stop=0x8c0990eeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c0990f0; /* mov.l r9,@-r15 */
L8c0990f0: *stop=0x8c0990f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c0990f2; /* mov.l r8,@-r15 */
L8c0990f2: *stop=0x8c0990f2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0990f4; /* sts.l PR,@-r15 */
L8c0990f4: *stop=0x8c0990f4u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c0990f6; /* mov.l @(r0,r14),r14 */
L8c0990f6: *stop=0x8c0990f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09918cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0990f8; /* mov.l 0x8c09918c,r5 */
L8c0990f8: *stop=0x8c0990f8u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0990fa; /* tst r14,r14 */
L8c0990fa: if(!(*sr&1u))goto L8c099102;goto L8c0990fc; /* bf 0x8c099102 */
L8c0990fc: *stop=0x8c0990fcu;r[2]=0u;goto L8c0990fe; /* mov #0x0,r2 */
L8c0990fe: *stop=0x8c099100u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[2],4);if(io_status)return io_status;goto L8c0991d2; /* bra 0x8c0991d2 */
L8c099102: *stop=0x8c099102u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c099104; /* mov.l @(0x4,r14),r13 */
L8c099104: *stop=0x8c099104u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c099106; /* tst r13,r13 */
L8c099106: take=(*sr&1u);*stop=0x8c099108u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[13],4);if(io_status)return io_status;if(take)goto L8c09911e;goto L8c09910a; /* bt/s 0x8c09911e */
L8c09910a: *stop=0x8c09910au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c09910c; /* mov.l @r14,r3 */
L8c09910c: *stop=0x8c09910cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09910e; /* tst r3,r3 */
L8c09910e: if((*sr&1u))goto L8c09911a;goto L8c099110; /* bt 0x8c09911a */
L8c099110: *stop=0x8c099110u;r[5]=r[13];goto L8c099112; /* mov r13,r5 */
L8c099112: target=0x8c099750u;*pr=0x8c099116u;*stop=0x8c099114u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099116; /* bsr 0x8c099750 */
L8c099116: *stop=0x8c099118u;goto L8c09911e; /* bra 0x8c09911e */
L8c09911a: target=0x8c098ee0u;*pr=0x8c09911eu;*stop=0x8c09911cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09911e; /* bsr 0x8c098ee0 */
L8c09911e: *stop=0x8c09911eu;r[14]+=8u;goto L8c099120; /* add #0x8,r14 */
L8c099120: *stop=0x8c099120u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0991a0u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c099122; /* mov.l 0x8c0991a0,r10 */
L8c099122: *stop=0x8c099122u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c099124; /* mov.l @(0x4,r14),r13 */
L8c099124: *stop=0x8c099124u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0991a4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c099126; /* mov.l 0x8c0991a4,r11 */
L8c099126: *stop=0x8c099126u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09919cu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c099128; /* mov.l 0x8c09919c,r9 */
L8c099128: *stop=0x8c099128u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c09912a; /* tst r13,r13 */
L8c09912a: *stop=0x8c09912au;{uint32_t loaded=rd(m,bus,&io_status,0x8c099194u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c09912c; /* mov.l 0x8c099194,r12 */
L8c09912c: *stop=0x8c09912cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c099198u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c09912e; /* mov.l 0x8c099198,r8 */
L8c09912e: if((*sr&1u))goto L8c0991ce;goto L8c099130; /* bt 0x8c0991ce */
L8c099130: *stop=0x8c099130u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c099132; /* mov.l @r14,r3 */
L8c099132: *stop=0x8c099132u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c099134; /* tst r3,r3 */
L8c099134: if((*sr&1u))goto L8c0991c0;goto L8c099136; /* bt 0x8c0991c0 */
L8c099136: *stop=0x8c099136u;r[5]=r[9];goto L8c099138; /* mov r9,r5 */
L8c099138: target=r[10];*pr=0x8c09913cu;*stop=0x8c09913au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09913c; /* jsr @r10 */
L8c09913c: *stop=0x8c09913cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09913e; /* tst r0,r0 */
L8c09913e: if(!(*sr&1u))goto L8c099158;goto L8c099140; /* bf 0x8c099158 */
L8c099140: *stop=0x8c099140u;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[2]=loaded;}goto L8c099142; /* mov.l @r8,r2 */
L8c099142: *stop=0x8c099142u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c099144; /* tst r2,r2 */
L8c099144: if(!(*sr&1u))goto L8c09914c;goto L8c099146; /* bf 0x8c09914c */
L8c099146: *stop=0x8c099146u;r[5]=r[13];goto L8c099148; /* mov r13,r5 */
L8c099148: *stop=0x8c09914au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c099150; /* bra 0x8c099150 */
L8c09914c: *stop=0x8c09914cu;r[4]=r[11];goto L8c09914e; /* mov r11,r4 */
L8c09914e: *stop=0x8c09914eu;r[5]=r[13];goto L8c099150; /* mov r13,r5 */
L8c099150: target=0x8c099750u;*pr=0x8c099154u;*stop=0x8c099152u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099154; /* bsr 0x8c099750 */
L8c099154: *stop=0x8c099156u;goto L8c0991c4; /* bra 0x8c0991c4 */
L8c099158: *stop=0x8c099158u;r[5]=r[13];goto L8c09915a; /* mov r13,r5 */
L8c09915a: target=0x8c099750u;*pr=0x8c09915eu;*stop=0x8c09915cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09915e; /* bsr 0x8c099750 */
L8c09915e: *stop=0x8c099160u;goto L8c0991c4; /* bra 0x8c0991c4 */
L8c0991c0: target=0x8c098ee0u;*pr=0x8c0991c4u;*stop=0x8c0991c2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0991c4; /* bsr 0x8c098ee0 */
L8c0991c4: *stop=0x8c0991c4u;r[14]+=8u;goto L8c0991c6; /* add #0x8,r14 */
L8c0991c6: *stop=0x8c0991c6u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[13],4);if(io_status)return io_status;goto L8c0991c8; /* mov.l r13,@r12 */
L8c0991c8: *stop=0x8c0991c8u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c0991ca; /* mov.l @(0x4,r14),r13 */
L8c0991ca: *stop=0x8c0991cau;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c0991cc; /* tst r13,r13 */
L8c0991cc: if(!(*sr&1u))goto L8c099130;goto L8c0991ce; /* bf 0x8c099130 */
L8c0991ce: *stop=0x8c0991ceu;r[3]=0u;goto L8c0991d0; /* mov #0x0,r3 */
L8c0991d0: *stop=0x8c0991d0u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;goto L8c0991d2; /* mov.l r3,@r12 */
L8c0991d2: *stop=0x8c0991d2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0991d4; /* lds.l @r15+,PR */
L8c0991d4: *stop=0x8c0991d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c0991d6; /* mov.l @r15+,r8 */
L8c0991d6: *stop=0x8c0991d6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c0991d8; /* mov.l @r15+,r9 */
L8c0991d8: *stop=0x8c0991d8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c0991da; /* mov.l @r15+,r10 */
L8c0991da: *stop=0x8c0991dau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c0991dc; /* mov.l @r15+,r11 */
L8c0991dc: *stop=0x8c0991dcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c0991de; /* mov.l @r15+,r12 */
L8c0991de: *stop=0x8c0991deu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0991e0; /* mov.l @r15+,r13 */
L8c0991e0: *stop=0x8c0991e0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0991e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c0990de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0990de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c099116(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0990de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c099116u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c09911e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0990de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09911eu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c09913c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0990de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09913cu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c099154(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0990de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c099154u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c09915e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0990de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09915eu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c0991c4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0990de(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0991c4u);}

static int sa_player_texture_load_run_8c099750(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c099778u)goto L8c099778;if(resume_pc==0x8c099784u)goto L8c099784;goto L8c099750;
L8c099750: *stop=0x8c099750u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c099752; /* mov.l r14,@-r15 */
L8c099752: *stop=0x8c099752u;r[0]=4294967240u;goto L8c099754; /* mov #-0x38,r0 */
L8c099754: *stop=0x8c099754u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c099756; /* sts.l PR,@-r15 */
L8c099756: *stop=0x8c099756u;r[0]<<=8;goto L8c099758; /* shll8 r0 */
L8c099758: *stop=0x8c099758u;r[0]+=4294967280u;goto L8c09975a; /* add #-0x10,r0 */
L8c09975a: *stop=0x8c09975au;r[15]+=r[0];goto L8c09975c; /* add r0,r15 */
L8c09975c: *stop=0x8c09975cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c09975e; /* mov.l r4,@(0x4,r15) */
L8c09975e: *stop=0x8c09975eu;r[4]=56u;goto L8c099760; /* mov #0x38,r4 */
L8c099760: *stop=0x8c099760u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998c8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c099762; /* mov.l 0x8c0998c8,r14 */
L8c099762: *stop=0x8c099762u;r[4]<<=8;goto L8c099764; /* shll8 r4 */
L8c099764: *stop=0x8c099764u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c099766; /* mov.l 0x8c0998cc,r3 */
L8c099766: *stop=0x8c099766u;r[6]=r[15];goto L8c099768; /* mov r15,r6 */
L8c099768: *stop=0x8c099768u;r[7]=2u;goto L8c09976a; /* mov #0x2,r7 */
L8c09976a: *stop=0x8c09976au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c09976c; /* mov.l r5,@r15 */
L8c09976c: *stop=0x8c09976cu;r[4]+=8u;goto L8c09976e; /* add #0x8,r4 */
L8c09976e: *stop=0x8c09976eu;r[5]=r[14];goto L8c099770; /* mov r14,r5 */
L8c099770: *stop=0x8c099770u;r[6]+=8u;goto L8c099772; /* add #0x8,r6 */
L8c099772: *stop=0x8c099772u;r[7]<<=8;goto L8c099774; /* shll8 r7 */
L8c099774: target=r[3];*pr=0x8c099778u;*stop=0x8c099776u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099778; /* jsr @r3 */
L8c099778: *stop=0x8c099778u;r[5]=56u;goto L8c09977a; /* mov #0x38,r5 */
L8c09977a: *stop=0x8c09977au;r[5]<<=8;goto L8c09977c; /* shll8 r5 */
L8c09977c: *stop=0x8c09977cu;r[5]+=8u;goto L8c09977e; /* add #0x8,r5 */
L8c09977e: *stop=0x8c09977eu;r[5]+=r[15];goto L8c099780; /* add r15,r5 */
L8c099780: target=0x8c0997bcu;*pr=0x8c099784u;*stop=0x8c099782u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099784; /* bsr 0x8c0997bc */
L8c099784: *stop=0x8c099784u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c099786; /* mov.l @r15,r6 */
L8c099786: *stop=0x8c099786u;r[7]=0u;goto L8c099788; /* mov #0x0,r7 */
L8c099788: *stop=0x8c099788u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c09978a; /* mov.l @r15,r1 */
L8c09978a: *stop=0x8c09978au;r[5]=r[7];goto L8c09978c; /* mov r7,r5 */
L8c09978c: *stop=0x8c09978cu;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c09978e; /* mov.l @(0x4,r6),r6 */
L8c09978e: *stop=0x8c09978eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c099790; /* mov.l @r1,r1 */
L8c099790: *stop=0x8c099790u;*sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c099792; /* cmp/pl r6 */
L8c099792: take=!(*sr&1u);*stop=0x8c099794u;r[4]=r[7];if(take)goto L8c0997ae;goto L8c099796; /* bf/s 0x8c0997ae */
L8c099796: *stop=0x8c099796u;r[7]=r[4];goto L8c099798; /* mov r4,r7 */
L8c099798: *stop=0x8c099798u;r[7]+=r[14];goto L8c09979a; /* add r14,r7 */
L8c09979a: *stop=0x8c09979au;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09979c; /* mov.l @(0x8,r7),r2 */
L8c09979c: *stop=0x8c09979cu;r[0]=r[4];goto L8c09979e; /* mov r4,r0 */
L8c09979e: *stop=0x8c09979eu;r[5]+=1u;goto L8c0997a0; /* add #0x1,r5 */
L8c0997a0: *stop=0x8c0997a0u;r[0]+=r[1];goto L8c0997a2; /* add r1,r0 */
L8c0997a2: *stop=0x8c0997a2u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c0997a4; /* cmp/ge r6,r5 */
L8c0997a4: *stop=0x8c0997a4u;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[2],4);if(io_status)return io_status;goto L8c0997a6; /* mov.l r2,@(0x8,r0) */
L8c0997a6: *stop=0x8c0997a6u;if(!valid_access(r[7]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0997a8; /* mov.l @(0x4,r7),r3 */
L8c0997a8: *stop=0x8c0997a8u;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[3],4);if(io_status)return io_status;goto L8c0997aa; /* mov.l r3,@(0x4,r0) */
L8c0997aa: take=!(*sr&1u);*stop=0x8c0997acu;r[4]+=12u;if(take)goto L8c099796;goto L8c0997ae; /* bf/s 0x8c099796 */
L8c0997ae: *stop=0x8c0997aeu;r[1]=56u;goto L8c0997b0; /* mov #0x38,r1 */
L8c0997b0: *stop=0x8c0997b0u;r[1]<<=8;goto L8c0997b2; /* shll8 r1 */
L8c0997b2: *stop=0x8c0997b2u;r[1]+=16u;goto L8c0997b4; /* add #0x10,r1 */
L8c0997b4: *stop=0x8c0997b4u;r[15]+=r[1];goto L8c0997b6; /* add r1,r15 */
L8c0997b6: *stop=0x8c0997b6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0997b8; /* lds.l @r15+,PR */
L8c0997b8: *stop=0x8c0997b8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0997bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c099750(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c099750(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c099778(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c099750(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c099778u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c099784(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c099750(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c099784u);}

static int sa_player_texture_load_run_8c0997bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c0997e2u)goto L8c0997e2;if(resume_pc==0x8c0997f4u)goto L8c0997f4;if(resume_pc==0x8c099818u)goto L8c099818;if(resume_pc==0x8c09983eu)goto L8c09983e;if(resume_pc==0x8c09984eu)goto L8c09984e;if(resume_pc==0x8c099856u)goto L8c099856;if(resume_pc==0x8c09985eu)goto L8c09985e;goto L8c0997bc;
L8c0997bc: *stop=0x8c0997bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0997be; /* mov.l r14,@-r15 */
L8c0997be: *stop=0x8c0997beu;r[0]=4294967295u;goto L8c0997c0; /* mov #-0x1,r0 */
L8c0997c0: *stop=0x8c0997c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0997c2; /* mov.l r13,@-r15 */
L8c0997c2: *stop=0x8c0997c2u;r[0]<<=8;goto L8c0997c4; /* shll8 r0 */
L8c0997c4: *stop=0x8c0997c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0997c6; /* mov.l r12,@-r15 */
L8c0997c6: *stop=0x8c0997c6u;r[0]+=4294967288u;goto L8c0997c8; /* add #-0x8,r0 */
L8c0997c8: *stop=0x8c0997c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0997ca; /* mov.l r11,@-r15 */
L8c0997ca: *stop=0x8c0997cau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c0997cc; /* mov.l r10,@-r15 */
L8c0997cc: *stop=0x8c0997ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0997ce; /* sts.l PR,@-r15 */
L8c0997ce: *stop=0x8c0997ceu;r[15]+=r[0];goto L8c0997d0; /* add r0,r15 */
L8c0997d0: *stop=0x8c0997d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0997d2; /* mov.l 0x8c0998d0,r3 */
L8c0997d2: *stop=0x8c0997d2u;r[12]=r[15];goto L8c0997d4; /* mov r15,r12 */
L8c0997d4: *stop=0x8c0997d4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c0997d6; /* mov.l r5,@(0x4,r15) */
L8c0997d6: *stop=0x8c0997d6u;r[12]+=8u;goto L8c0997d8; /* add #0x8,r12 */
L8c0997d8: *stop=0x8c0997d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0997da; /* mov.l 0x8c0998d4,r2 */
L8c0997da: *stop=0x8c0997dau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c0997dc; /* mov.l r4,@-r15 */
L8c0997dc: *stop=0x8c0997dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0997de; /* mov.l r3,@-r15 */
L8c0997de: target=r[2];*pr=0x8c0997e2u;*stop=0x8c0997e0u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0997e2; /* jsr @r2 */
L8c0997e2: *stop=0x8c0997e2u;r[15]+=8u;goto L8c0997e4; /* add #0x8,r15 */
L8c0997e4: *stop=0x8c0997e4u;r[11]=4u;goto L8c0997e6; /* mov #0x4,r11 */
L8c0997e6: *stop=0x8c0997e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998d8u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c0997e8; /* mov.l 0x8c0998d8,r10 */
L8c0997e8: *stop=0x8c0997e8u;r[11]<<=8;goto L8c0997ea; /* shll8 r11 */
L8c0997ea: *stop=0x8c0997eau;r[11]+=4294967272u;goto L8c0997ec; /* add #-0x18,r11 */
L8c0997ec: *stop=0x8c0997ecu;r[13]=3u;goto L8c0997ee; /* mov #0x3,r13 */
L8c0997ee: *stop=0x8c0997eeu;r[5]=0u;goto L8c0997f0; /* mov #0x0,r5 */
L8c0997f0: target=r[10];*pr=0x8c0997f4u;*stop=0x8c0997f2u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0997f4; /* jsr @r10 */
L8c0997f4: *stop=0x8c0997f4u;r[14]=r[0];goto L8c0997f6; /* mov r0,r14 */
L8c0997f6: *stop=0x8c0997f6u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0997f8; /* tst r14,r14 */
L8c0997f8: if(!(*sr&1u))goto L8c099810;goto L8c0997fa; /* bf 0x8c099810 */
L8c0997fa: *stop=0x8c0997fau;r[4]=r[11];goto L8c0997fc; /* mov r11,r4 */
L8c0997fc: *stop=0x8c0997fcu;r[4]+=4294967294u;goto L8c0997fe; /* add #-0x2,r4 */
L8c0997fe: *stop=0x8c0997feu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c099800; /* tst r4,r4 */
L8c099800: if(!(*sr&1u))goto L8c0997fc;goto L8c099802; /* bf 0x8c0997fc */
L8c099802: *stop=0x8c099802u;r[13]+=4294967295u;goto L8c099804; /* add #-0x1,r13 */
L8c099804: *stop=0x8c099804u;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c099806; /* cmp/pl r13 */
L8c099806: if((*sr&1u))goto L8c0997ee;goto L8c099808; /* bt 0x8c0997ee */
L8c099808: *stop=0x8c099808u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c09980a; /* tst r14,r14 */
L8c09980a: if(!(*sr&1u))goto L8c099810;goto L8c09980c; /* bf 0x8c099810 */
L8c09980c: *stop=0x8c09980eu;r[0]=4294967295u;goto L8c09985e; /* bra 0x8c09985e */
L8c099810: *stop=0x8c099810u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c099812; /* mov.l 0x8c0998dc,r2 */
L8c099812: *stop=0x8c099812u;r[5]=r[15];goto L8c099814; /* mov r15,r5 */
L8c099814: target=r[2];*pr=0x8c099818u;*stop=0x8c099816u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099818; /* jsr @r2 */
L8c099818: *stop=0x8c099818u;r[3]=8u;goto L8c09981a; /* mov #0x8,r3 */
L8c09981a: *stop=0x8c09981au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}goto L8c09981c; /* mov.l @r15,r11 */
L8c09981c: *stop=0x8c09981cu;r[3]<<=8;goto L8c09981e; /* shll8 r3 */
L8c09981e: *stop=0x8c09981eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998e0u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c099820; /* mov.l 0x8c0998e0,r12 */
L8c099820: *stop=0x8c099820u;r[3]+=4294967295u;goto L8c099822; /* add #-0x1,r3 */
L8c099822: *stop=0x8c099822u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998e4u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c099824; /* mov.l 0x8c0998e4,r10 */
L8c099824: *stop=0x8c099824u;r[2]=4294967285u;goto L8c099826; /* mov #-0xb,r2 */
L8c099826: *stop=0x8c099826u;r[11]+=r[3];goto L8c099828; /* add r3,r11 */
L8c099828: *stop=0x8c099828u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[11]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[11]=n?(uint32_t)((int32_t)r[11]>>n):((r[11]&0x80000000u)?0xffffffffu:0u);}}goto L8c09982a; /* shad r2,r11 */
L8c09982a: *stop=0x8c09982au;r[1]=r[11];goto L8c09982c; /* mov r11,r1 */
L8c09982c: *stop=0x8c09982cu;r[1]<<=8;goto L8c09982e; /* shll8 r1 */
L8c09982e: *stop=0x8c09982eu;r[1]<<=2;goto L8c099830; /* shll2 r1 */
L8c099830: *stop=0x8c099830u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c099832; /* shll r1 */
L8c099832: *stop=0x8c099832u;r[13]=3u;goto L8c099834; /* mov #0x3,r13 */
L8c099834: *stop=0x8c099834u;r[12]-=r[1];goto L8c099836; /* sub r1,r12 */
L8c099836: *stop=0x8c099836u;r[5]=r[11];goto L8c099838; /* mov r11,r5 */
L8c099838: *stop=0x8c099838u;r[6]=r[12];goto L8c09983a; /* mov r12,r6 */
L8c09983a: target=r[10];*pr=0x8c09983eu;*stop=0x8c09983cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09983e; /* jsr @r10 */
L8c09983e: *stop=0x8c09983eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c099840; /* tst r0,r0 */
L8c099840: if((*sr&1u))goto L8c099848;goto L8c099842; /* bt 0x8c099848 */
L8c099842: *stop=0x8c099842u;r[13]+=4294967295u;goto L8c099844; /* add #-0x1,r13 */
L8c099844: *stop=0x8c099844u;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c099846; /* cmp/pl r13 */
L8c099846: if((*sr&1u))goto L8c099836;goto L8c099848; /* bt 0x8c099836 */
L8c099848: *stop=0x8c099848u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09984a; /* mov.l 0x8c0998e8,r2 */
L8c09984a: target=r[2];*pr=0x8c09984eu;*stop=0x8c09984cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09984e; /* jsr @r2 */
L8c09984e: *stop=0x8c09984eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c099850; /* mov.l 0x8c0998f0,r3 */
L8c099850: *stop=0x8c099850u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998ecu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c099852; /* mov.l 0x8c0998ec,r5 */
L8c099852: target=r[3];*pr=0x8c099856u;*stop=0x8c099854u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099856; /* jsr @r3 */
L8c099856: *stop=0x8c099856u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c099858; /* mov.l 0x8c0998f4,r3 */
L8c099858: *stop=0x8c099858u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0998ecu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c09985a; /* mov.l 0x8c0998ec,r4 */
L8c09985a: target=r[3];*pr=0x8c09985eu;*stop=0x8c09985cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09985e; /* jsr @r3 */
L8c09985e: *stop=0x8c09985eu;r[1]=66u;goto L8c099860; /* mov #0x42,r1 */
L8c099860: *stop=0x8c099860u;r[1]<<=2;goto L8c099862; /* shll2 r1 */
L8c099862: *stop=0x8c099862u;r[15]+=r[1];goto L8c099864; /* add r1,r15 */
L8c099864: *stop=0x8c099864u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c099866; /* lds.l @r15+,PR */
L8c099866: *stop=0x8c099866u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c099868; /* mov.l @r15+,r10 */
L8c099868: *stop=0x8c099868u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c09986a; /* mov.l @r15+,r11 */
L8c09986a: *stop=0x8c09986au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c09986c; /* mov.l @r15+,r12 */
L8c09986c: *stop=0x8c09986cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c09986e; /* mov.l @r15+,r13 */
L8c09986e: *stop=0x8c09986eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c099870u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c0997bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0997bc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c0997e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0997bc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0997e2u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c0997f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0997bc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0997f4u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c099818(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0997bc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c099818u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c09983e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0997bc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09983eu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c09984e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0997bc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09984eu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c099856(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0997bc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c099856u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c09985e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c0997bc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09985eu);}

SA_STAGE_EXPORT int sa_player_texture_load_8c607264(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c607264;
L8c607264: *stop=0x8c607264u;*sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c607266; /* cmp/pl r7 */
L8c607266: *stop=0x8c607266u;r[0]=0u;goto L8c607268; /* mov #0x0,r0 */
L8c607268: *stop=0x8c607268u;r[1]=0u;goto L8c60726a; /* mov #0x0,r1 */
L8c60726a: take=!(*sr&1u);*stop=0x8c60726cu;r[0]+=r[5];if(take)goto L8c60727a;goto L8c60726e; /* bf/s 0x8c60727a */
L8c60726e: *stop=0x8c60726eu;r[1]+=1u;goto L8c607270; /* add #0x1,r1 */
L8c607270: *stop=0x8c607270u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[6],4);if(io_status)return io_status;goto L8c607272; /* mov.l r6,@r0 */
L8c607272: *stop=0x8c607272u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[7]);goto L8c607274; /* cmp/ge r7,r1 */
L8c607274: *stop=0x8c607274u;r[6]+=28u;goto L8c607276; /* add #0x1c,r6 */
L8c607276: take=!(*sr&1u);*stop=0x8c607278u;r[0]+=12u;if(take)goto L8c60726e;goto L8c60727a; /* bf/s 0x8c60726e */
L8c60727a: *stop=0x8c60727au;if(*pr!=entry_pr)return -3;*stop=0x8c60727cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;return 0; /* rts */
}

static int sa_player_texture_load_run_8c10d360(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c10d38eu)goto L8c10d38e;goto L8c10d360;
L8c10d360: *stop=0x8c10d360u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d362; /* sts.l PR,@-r15 */
L8c10d362: *stop=0x8c10d362u;r[15]+=4294967292u;goto L8c10d364; /* add #-0x4,r15 */
L8c10d364: *stop=0x8c10d364u;r[0]=r[15];goto L8c10d366; /* mov r15,r0 */
L8c10d366: *stop=0x8c10d366u;r[0]+=8u;goto L8c10d368; /* add #0x8,r0 */
L8c10d368: *stop=0x8c10d368u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c10d36a; /* mov.l r4,@r15 */
L8c10d36a: *stop=0x8c10d36au;r[4]=3u;goto L8c10d36c; /* mov #0x3,r4 */
L8c10d36c: *stop=0x8c10d36cu;r[0]+=4u;goto L8c10d36e; /* add #0x4,r0 */
L8c10d36e: *stop=0x8c10d36eu;*sr=(*sr&~1u)|!(r[0]&r[4]);goto L8c10d370; /* tst r0,r4 */
L8c10d370: if((*sr&1u))goto L8c10d37a;goto L8c10d372; /* bt 0x8c10d37a */
L8c10d372: *stop=0x8c10d372u;r[4]=r[15];goto L8c10d374; /* mov r15,r4 */
L8c10d374: *stop=0x8c10d374u;r[4]+=8u;goto L8c10d376; /* add #0x8,r4 */
L8c10d376: *stop=0x8c10d378u;r[4]+=8u;goto L8c10d380; /* bra 0x8c10d380 */
L8c10d37a: *stop=0x8c10d37au;r[4]=r[15];goto L8c10d37c; /* mov r15,r4 */
L8c10d37c: *stop=0x8c10d37cu;r[4]+=8u;goto L8c10d37e; /* add #0x8,r4 */
L8c10d37e: *stop=0x8c10d37eu;r[4]+=4u;goto L8c10d380; /* add #0x4,r4 */
L8c10d380: *stop=0x8c10d380u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c10d382; /* mov.l r4,@-r15 */
L8c10d382: *stop=0x8c10d382u;r[5]=0u;goto L8c10d384; /* mov #0x0,r5 */
L8c10d384: *stop=0x8c10d384u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d398u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d386; /* mov.l 0x8c10d398,r3 */
L8c10d386: *stop=0x8c10d386u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c10d388; /* mov.l @(0x4,r15),r6 */
L8c10d388: *stop=0x8c10d388u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d38a; /* mov.l @(0xc,r15),r7 */
L8c10d38a: target=r[3];*pr=0x8c10d38eu;*stop=0x8c10d38cu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d38e; /* jsr @r3 */
L8c10d38e: *stop=0x8c10d38eu;r[15]+=8u;goto L8c10d390; /* add #0x8,r15 */
L8c10d390: *stop=0x8c10d390u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d392; /* lds.l @r15+,PR */
L8c10d392: *stop=0x8c10d392u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c10d394u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c10d360(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c10d360(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c10d38e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c10d360(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c10d38eu);}

SA_STAGE_EXPORT int sa_player_texture_load_8c04c5e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c04c5e0;
L8c04c5e0: *stop=0x8c04c5e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c04c5e2; /* mov.l r9,@-r15 */
L8c04c5e2: *stop=0x8c04c5e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c04c5e4; /* mov.l r8,@-r15 */
L8c04c5e4: *stop=0x8c04c5e4u;r[9]=sx(m,bus,&io_status,0x8c04c5f0u,2);if(io_status)return io_status;goto L8c04c5e6; /* mov.w 0x8c04c5f0,r9 */
L8c04c5e6: *stop=0x8c04c5e6u;r[8]=sx(m,bus,&io_status,0x8c04c5f2u,2);if(io_status)return io_status;goto L8c04c5e8; /* mov.w 0x8c04c5f2,r8 */
L8c04c5e8: *stop=0x8c04c5e8u;if(!valid_access(r[4],1u))return -3;r[6]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c04c5ea; /* mov.b @r4+,r6 */
L8c04c5ea: *stop=0x8c04c5eau;r[7]=9u;goto L8c04c5ec; /* mov #0x9,r7 */
L8c04c5ec: *stop=0x8c04c5eeu;r[3]=r[5];goto L8c04c5fa; /* bra 0x8c04c5fa */
L8c04c5f4: *stop=0x8c04c5f4u;if(!valid_access(r[4],1u))return -3;r[1]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c04c5f6; /* mov.b @r4+,r1 */
L8c04c5f6: *stop=0x8c04c5f6u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[1],1);if(io_status)return io_status;goto L8c04c5f8; /* mov.b r1,@r5 */
L8c04c5f8: *stop=0x8c04c5f8u;r[5]+=1u;goto L8c04c5fa; /* add #0x1,r5 */
L8c04c5fa: *stop=0x8c04c5fau;r[7]--;*sr=(*sr&~1u)|(!r[7]);goto L8c04c5fc; /* dt r7 */
L8c04c5fc: take=!(*sr&1u);*stop=0x8c04c5feu;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;if(take)goto L8c04c606;goto L8c04c600; /* bf/s 0x8c04c606 */
L8c04c600: *stop=0x8c04c600u;if(!valid_access(r[4],1u))return -3;r[6]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c04c602; /* mov.b @r4+,r6 */
L8c04c602: *stop=0x8c04c602u;r[7]=8u;goto L8c04c604; /* mov #0x8,r7 */
L8c04c604: *stop=0x8c04c604u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c04c606; /* shlr r6 */
L8c04c606: if((*sr&1u))goto L8c04c5f4;goto L8c04c608; /* bt 0x8c04c5f4 */
L8c04c608: *stop=0x8c04c608u;r[7]--;*sr=(*sr&~1u)|(!r[7]);goto L8c04c60a; /* dt r7 */
L8c04c60a: take=!(*sr&1u);*stop=0x8c04c60cu;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;if(take)goto L8c04c614;goto L8c04c60e; /* bf/s 0x8c04c614 */
L8c04c60e: *stop=0x8c04c60eu;if(!valid_access(r[4],1u))return -3;r[6]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c04c610; /* mov.b @r4+,r6 */
L8c04c610: *stop=0x8c04c610u;r[7]=8u;goto L8c04c612; /* mov #0x8,r7 */
L8c04c612: *stop=0x8c04c612u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c04c614; /* shlr r6 */
L8c04c614: if((*sr&1u))goto L8c04c64a;goto L8c04c616; /* bt 0x8c04c64a */
L8c04c616: *stop=0x8c04c616u;r[0]=0u;goto L8c04c618; /* mov #0x0,r0 */
L8c04c618: *stop=0x8c04c618u;r[7]--;*sr=(*sr&~1u)|(!r[7]);goto L8c04c61a; /* dt r7 */
L8c04c61a: take=!(*sr&1u);*stop=0x8c04c61cu;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;if(take)goto L8c04c624;goto L8c04c61e; /* bf/s 0x8c04c624 */
L8c04c61e: *stop=0x8c04c61eu;if(!valid_access(r[4],1u))return -3;r[6]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c04c620; /* mov.b @r4+,r6 */
L8c04c620: *stop=0x8c04c620u;r[7]=8u;goto L8c04c622; /* mov #0x8,r7 */
L8c04c622: *stop=0x8c04c622u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c04c624; /* shlr r6 */
L8c04c624: *stop=0x8c04c624u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[0]>>31);r[0]=(r[0]<<1)|incoming;}goto L8c04c626; /* rotcl r0 */
L8c04c626: *stop=0x8c04c626u;r[7]--;*sr=(*sr&~1u)|(!r[7]);goto L8c04c628; /* dt r7 */
L8c04c628: take=!(*sr&1u);*stop=0x8c04c62au;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;if(take)goto L8c04c632;goto L8c04c62c; /* bf/s 0x8c04c632 */
L8c04c62c: *stop=0x8c04c62cu;if(!valid_access(r[4],1u))return -3;r[6]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c04c62e; /* mov.b @r4+,r6 */
L8c04c62e: *stop=0x8c04c62eu;r[7]=8u;goto L8c04c630; /* mov #0x8,r7 */
L8c04c630: *stop=0x8c04c630u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c04c632; /* shlr r6 */
L8c04c632: *stop=0x8c04c632u;if(!valid_access(r[4],1u))return -3;r[2]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c04c634; /* mov.b @r4+,r2 */
L8c04c634: *stop=0x8c04c634u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[0]>>31);r[0]=(r[0]<<1)|incoming;}goto L8c04c636; /* rotcl r0 */
L8c04c636: *stop=0x8c04c636u;r[2]|=r[9];goto L8c04c638; /* or r9,r2 */
L8c04c638: *stop=0x8c04c638u;r[0]+=2u;goto L8c04c63a; /* add #0x2,r0 */
L8c04c63a: *stop=0x8c04c63au;r[2]+=r[5];goto L8c04c63c; /* add r5,r2 */
L8c04c63c: *stop=0x8c04c63cu;if(!valid_access(r[2],1u))return -3;r[1]=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;r[2]+=1;goto L8c04c63e; /* mov.b @r2+,r1 */
L8c04c63e: *stop=0x8c04c63eu;r[0]--;*sr=(*sr&~1u)|(!r[0]);goto L8c04c640; /* dt r0 */
L8c04c640: *stop=0x8c04c640u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[1],1);if(io_status)return io_status;goto L8c04c642; /* mov.b r1,@r5 */
L8c04c642: take=!(*sr&1u);*stop=0x8c04c644u;r[5]+=1u;if(take)goto L8c04c63c;goto L8c04c646; /* bf/s 0x8c04c63c */
L8c04c646: *stop=0x8c04c648u;goto L8c04c5fa; /* bra 0x8c04c5fa */
L8c04c64a: *stop=0x8c04c64au;if(!valid_access(r[4],1u))return -3;r[0]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c04c64c; /* mov.b @r4+,r0 */
L8c04c64c: *stop=0x8c04c64cu;if(!valid_access(r[4],1u))return -3;r[1]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c04c64e; /* mov.b @r4+,r1 */
L8c04c64e: *stop=0x8c04c64eu;r[2]=r[0]&255u;goto L8c04c650; /* extu.b r0,r2 */
L8c04c650: *stop=0x8c04c650u;r[1]<<=8;goto L8c04c652; /* shll8 r1 */
L8c04c652: *stop=0x8c04c652u;r[2]|=r[1];goto L8c04c654; /* or r1,r2 */
L8c04c654: *stop=0x8c04c654u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c04c656; /* tst r2,r2 */
L8c04c656: if((*sr&1u))goto L8c04c66e;goto L8c04c658; /* bt 0x8c04c66e */
L8c04c658: *stop=0x8c04c658u;r[2]>>=2;goto L8c04c65a; /* shlr2 r2 */
L8c04c65a: *stop=0x8c04c65au;*sr=(*sr&~1u)|(r[2]&1u);r[2]>>=1;goto L8c04c65c; /* shlr r2 */
L8c04c65c: *stop=0x8c04c65cu;r[0]&=0x7u;goto L8c04c65e; /* and #0x7,r0 */
L8c04c65e: *stop=0x8c04c65eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c04c660; /* tst r0,r0 */
L8c04c660: take=!(*sr&1u);*stop=0x8c04c662u;r[2]|=r[8];if(take)goto L8c04c638;goto L8c04c664; /* bf/s 0x8c04c638 */
L8c04c664: *stop=0x8c04c664u;if(!valid_access(r[4],1u))return -3;r[0]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c04c666; /* mov.b @r4+,r0 */
L8c04c666: *stop=0x8c04c666u;r[2]+=r[5];goto L8c04c668; /* add r5,r2 */
L8c04c668: *stop=0x8c04c668u;r[0]=r[0]&255u;goto L8c04c66a; /* extu.b r0,r0 */
L8c04c66a: *stop=0x8c04c66cu;r[0]+=1u;goto L8c04c63c; /* bra 0x8c04c63c */
L8c04c66e: *stop=0x8c04c66eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c04c670; /* mov.l @r15+,r8 */
L8c04c670: *stop=0x8c04c670u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c04c672; /* mov.l @r15+,r9 */
L8c04c672: *stop=0x8c04c672u;r[0]=r[5];goto L8c04c674; /* mov r5,r0 */
L8c04c674: *stop=0x8c04c674u;if(*pr!=entry_pr)return -3;*stop=0x8c04c676u;r[0]-=r[3];return 0; /* rts */
}

static int sa_player_texture_load_run_8c608134(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c608156u)goto L8c608156;if(resume_pc==0x8c60815cu)goto L8c60815c;if(resume_pc==0x8c608166u)goto L8c608166;if(resume_pc==0x8c608178u)goto L8c608178;if(resume_pc==0x8c608190u)goto L8c608190;if(resume_pc==0x8c60819au)goto L8c60819a;if(resume_pc==0x8c6081a2u)goto L8c6081a2;if(resume_pc==0x8c6081aeu)goto L8c6081ae;if(resume_pc==0x8c6081b2u)goto L8c6081b2;if(resume_pc==0x8c6081f6u)goto L8c6081f6;if(resume_pc==0x8c60822au)goto L8c60822a;if(resume_pc==0x8c60825au)goto L8c60825a;if(resume_pc==0x8c608272u)goto L8c608272;if(resume_pc==0x8c6082b8u)goto L8c6082b8;if(resume_pc==0x8c6082f6u)goto L8c6082f6;if(resume_pc==0x8c60831au)goto L8c60831a;if(resume_pc==0x8c608330u)goto L8c608330;if(resume_pc==0x8c608338u)goto L8c608338;if(resume_pc==0x8c60835au)goto L8c60835a;if(resume_pc==0x8c608374u)goto L8c608374;if(resume_pc==0x8c60837cu)goto L8c60837c;if(resume_pc==0x8c6083dcu)goto L8c6083dc;if(resume_pc==0x8c6083f0u)goto L8c6083f0;if(resume_pc==0x8c60842eu)goto L8c60842e;if(resume_pc==0x8c608480u)goto L8c608480;if(resume_pc==0x8c6084a4u)goto L8c6084a4;if(resume_pc==0x8c6084c6u)goto L8c6084c6;if(resume_pc==0x8c6084dcu)goto L8c6084dc;if(resume_pc==0x8c60852au)goto L8c60852a;if(resume_pc==0x8c60854eu)goto L8c60854e;if(resume_pc==0x8c608580u)goto L8c608580;if(resume_pc==0x8c6085a2u)goto L8c6085a2;if(resume_pc==0x8c6085d4u)goto L8c6085d4;if(resume_pc==0x8c6085eau)goto L8c6085ea;if(resume_pc==0x8c6085f4u)goto L8c6085f4;if(resume_pc==0x8c608618u)goto L8c608618;if(resume_pc==0x8c608626u)goto L8c608626;if(resume_pc==0x8c60862eu)goto L8c60862e;if(resume_pc==0x8c608644u)goto L8c608644;if(resume_pc==0x8c608652u)goto L8c608652;if(resume_pc==0x8c608666u)goto L8c608666;if(resume_pc==0x8c608674u)goto L8c608674;if(resume_pc==0x8c60867cu)goto L8c60867c;if(resume_pc==0x8c608690u)goto L8c608690;if(resume_pc==0x8c6086b4u)goto L8c6086b4;if(resume_pc==0x8c6086eeu)goto L8c6086ee;if(resume_pc==0x8c6086fcu)goto L8c6086fc;if(resume_pc==0x8c608704u)goto L8c608704;if(resume_pc==0x8c608718u)goto L8c608718;if(resume_pc==0x8c608736u)goto L8c608736;if(resume_pc==0x8c60874au)goto L8c60874a;if(resume_pc==0x8c608762u)goto L8c608762;if(resume_pc==0x8c608770u)goto L8c608770;if(resume_pc==0x8c60878cu)goto L8c60878c;if(resume_pc==0x8c608794u)goto L8c608794;if(resume_pc==0x8c6087a8u)goto L8c6087a8;goto L8c608134;
L8c608134: *stop=0x8c608134u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c608136; /* mov.l r14,@-r15 */
L8c608136: *stop=0x8c608136u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c608138; /* mov.l r13,@-r15 */
L8c608138: *stop=0x8c608138u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c60813a; /* mov.l r12,@-r15 */
L8c60813a: *stop=0x8c60813au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c60813c; /* mov.l r11,@-r15 */
L8c60813c: *stop=0x8c60813cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c60813e; /* mov.l r10,@-r15 */
L8c60813e: *stop=0x8c60813eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c608140; /* mov.l r9,@-r15 */
L8c608140: *stop=0x8c608140u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c608142; /* mov.l r8,@-r15 */
L8c608142: *stop=0x8c608142u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c608144; /* sts.l PR,@-r15 */
L8c608144: *stop=0x8c608144u;r[15]+=4294967208u;goto L8c608146; /* add #-0x58,r15 */
L8c608146: *stop=0x8c608146u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081d8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c608148; /* mov.l 0x8c6081d8,r12 */
L8c608148: *stop=0x8c608148u;r[10]=1u;goto L8c60814a; /* mov #0x1,r10 */
L8c60814a: *stop=0x8c60814au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60814c; /* mov.l 0x8c6081dc,r3 */
L8c60814c: *stop=0x8c60814cu;r[13]=0u;goto L8c60814e; /* mov #0x0,r13 */
L8c60814e: *stop=0x8c60814eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c608150; /* mov.l r4,@r15 */
L8c608150: *stop=0x8c608150u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c608152; /* mov.l r5,@(0xc,r15) */
L8c608152: target=r[3];*pr=0x8c608156u;*stop=0x8c608154u;r[8]=4294967295u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608156; /* jsr @r3 */
L8c608156: *stop=0x8c608156u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608158; /* mov.l 0x8c6081e0,r3 */
L8c608158: target=r[3];*pr=0x8c60815cu;*stop=0x8c60815au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60815c; /* jsr @r3 */
L8c60815c: *stop=0x8c60815cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60815e; /* mov.l 0x8c6081c8,r2 */
L8c60815e: *stop=0x8c60815eu;r[4]=r[15];goto L8c608160; /* mov r15,r4 */
L8c608160: *stop=0x8c608160u;r[5]=4u;goto L8c608162; /* mov #0x4,r5 */
L8c608162: target=r[2];*pr=0x8c608166u;*stop=0x8c608164u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608166; /* jsr @r2 */
L8c608166: *stop=0x8c608166u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608168; /* mov.l @(0x14,r15),r2 */
L8c608168: *stop=0x8c608168u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60816a; /* mov.l 0x8c6081e4,r3 */
L8c60816a: *stop=0x8c60816au;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c60816c; /* cmp/eq r3,r2 */
L8c60816c: if(!(*sr&1u))goto L8c6081bc;goto L8c60816e; /* bf 0x8c6081bc */
L8c60816e: *stop=0x8c60816eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608170; /* mov.l 0x8c6081c8,r2 */
L8c608170: *stop=0x8c608170u;r[4]=r[15];goto L8c608172; /* mov r15,r4 */
L8c608172: *stop=0x8c608172u;r[5]=8u;goto L8c608174; /* mov #0x8,r5 */
L8c608174: target=r[2];*pr=0x8c608178u;*stop=0x8c608176u;r[4]+=80u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608178; /* jsr @r2 */
L8c608178: *stop=0x8c608178u;r[0]=84u;goto L8c60817a; /* mov #0x54,r0 */
L8c60817a: *stop=0x8c60817au;if(!valid_access(r[0]+r[15],2u))return -3;r[4]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c60817c; /* mov.w @(r0,r15),r4 */
L8c60817c: *stop=0x8c60817cu;r[0]=86u;goto L8c60817e; /* mov #0x56,r0 */
L8c60817e: *stop=0x8c60817eu;if(!valid_access(r[0]+r[15],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c608180; /* mov.w @(r0,r15),r0 */
L8c608180: *stop=0x8c608180u;if(!valid_access(r[15]+24u,2u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[0],2);if(io_status)return io_status;goto L8c608182; /* mov.w r0,@(0x18,r15) */
L8c608182: *stop=0x8c608182u;if(!valid_access(r[15]+24u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x18u,2);if(io_status)return io_status;goto L8c608184; /* mov.w @(0x18,r15),r0 */
L8c608184: *stop=0x8c608184u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608186; /* mov.l @(0xc,r15),r3 */
L8c608186: *stop=0x8c608186u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c608188; /* mov.l r0,@r15 */
L8c608188: *stop=0x8c608188u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c60818a; /* mov.l r0,@(0x4,r3) */
L8c60818a: *stop=0x8c60818au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60818c; /* mov.l 0x8c6081e8,r3 */
L8c60818c: target=r[3];*pr=0x8c608190u;*stop=0x8c60818eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608190; /* jsr @r3 */
L8c608190: *stop=0x8c608190u;r[6]=r[15];goto L8c608192; /* mov r15,r6 */
L8c608192: *stop=0x8c608192u;r[6]+=80u;goto L8c608194; /* add #0x50,r6 */
L8c608194: *stop=0x8c608194u;r[5]=r[0];goto L8c608196; /* mov r0,r5 */
L8c608196: target=0x8c607ebcu;*pr=0x8c60819au;*stop=0x8c608198u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60819a; /* bsr 0x8c607ebc */
L8c60819a: *stop=0x8c60819au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60819c; /* mov.l 0x8c6081cc,r2 */
L8c60819c: *stop=0x8c60819cu;r[5]=0u;goto L8c60819e; /* mov #0x0,r5 */
L8c60819e: target=r[2];*pr=0x8c6081a2u;*stop=0x8c6081a0u;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6081a2; /* jsr @r2 */
L8c6081a2: *stop=0x8c6081a2u;r[0]=80u;goto L8c6081a4; /* mov #0x50,r0 */
L8c6081a4: *stop=0x8c6081a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6081a6; /* mov.l 0x8c6081cc,r3 */
L8c6081a6: *stop=0x8c6081a6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6081a8; /* mov.l @(r0,r15),r4 */
L8c6081a8: *stop=0x8c6081a8u;r[5]=1u;goto L8c6081aa; /* mov #0x1,r5 */
L8c6081aa: target=r[3];*pr=0x8c6081aeu;*stop=0x8c6081acu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6081ae; /* jsr @r3 */
L8c6081ae: target=0x8c607f90u;*pr=0x8c6081b2u;*stop=0x8c6081b0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6081b2; /* bsr 0x8c607f90 */
L8c6081b2: *stop=0x8c6081b2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6081b4; /* mov.l @r15,r3 */
L8c6081b4: *stop=0x8c6081b4u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6081b6; /* mov.l @(0xc,r15),r2 */
L8c6081b6: *stop=0x8c6081b6u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6081b8; /* mov.l r3,@(0x4,r2) */
L8c6081b8: *stop=0x8c6081bau;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[13],4);if(io_status)return io_status;goto L8c6087ae; /* bra 0x8c6087ae */
L8c6081bc: *stop=0x8c6081bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6081be; /* mov.l 0x8c6081ec,r2 */
L8c6081be: *stop=0x8c6081beu;r[1]=4u;goto L8c6081c0; /* mov #0x4,r1 */
L8c6081c0: *stop=0x8c6081c2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c60877a; /* bra 0x8c60877a */
L8c6081f0: *stop=0x8c6081f0u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6081f2; /* mov.l @(0x10,r15),r5 */
L8c6081f2: target=0x8c607244u;*pr=0x8c6081f6u;*stop=0x8c6081f4u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6081f6; /* bsr 0x8c607244 */
L8c6081f6: *stop=0x8c6081f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608434u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6081f8; /* mov.l 0x8c608434,r7 */
L8c6081f8: *stop=0x8c6081f8u;r[9]=2u;goto L8c6081fa; /* mov #0x2,r9 */
L8c6081fa: *stop=0x8c6081fau;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c6081fc; /* mov.l r0,@(0x1c,r15) */
L8c6081fc: *stop=0x8c6081fcu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6081fe; /* mov.l @(0x1c,r15),r4 */
L8c6081fe: *stop=0x8c6081feu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c608200; /* mov.l @(0xc,r15),r6 */
L8c608200: *stop=0x8c608200u;r[3]=r[4];goto L8c608202; /* mov r4,r3 */
L8c608202: *stop=0x8c608202u;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c608204; /* shll r4 */
L8c608204: *stop=0x8c608204u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[6]=loaded;}goto L8c608206; /* mov.l @r6,r6 */
L8c608206: *stop=0x8c608206u;r[4]+=r[3];goto L8c608208; /* add r3,r4 */
L8c608208: *stop=0x8c608208u;r[4]<<=2;goto L8c60820a; /* shll2 r4 */
L8c60820a: *stop=0x8c60820au;{uint32_t loaded=rd(m,bus,&io_status,0x8c608438u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60820c; /* mov.l 0x8c608438,r3 */
L8c60820c: *stop=0x8c60820cu;r[6]+=r[4];goto L8c60820e; /* add r4,r6 */
L8c60820e: *stop=0x8c60820eu;r[14]=r[6];goto L8c608210; /* mov r6,r14 */
L8c608210: *stop=0x8c608210u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c608212; /* mov.l r6,@r15 */
L8c608212: *stop=0x8c608212u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[13],4);if(io_status)return io_status;goto L8c608214; /* mov.l r13,@(0x8,r15) */
L8c608214: *stop=0x8c608214u;r[14]+=8u;goto L8c608216; /* add #0x8,r14 */
L8c608216: *stop=0x8c608216u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608218; /* mov.l @(0x4,r6),r2 */
L8c608218: *stop=0x8c608218u;*sr=(*sr&~1u)|!(r[7]&r[2]);goto L8c60821a; /* tst r7,r2 */
L8c60821a: take=!(*sr&1u);*stop=0x8c60821cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}if(take)goto L8c608238;goto L8c60821e; /* bf/s 0x8c608238 */
L8c60821e: *stop=0x8c60821eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}goto L8c608220; /* mov.l @r15,r11 */
L8c608220: *stop=0x8c608220u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60843cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608222; /* mov.l 0x8c60843c,r2 */
L8c608222: *stop=0x8c608222u;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c608224; /* mov.l @(0x8,r11),r11 */
L8c608224: *stop=0x8c608224u;r[6]=r[11];goto L8c608226; /* mov r11,r6 */
L8c608226: target=r[2];*pr=0x8c60822au;*stop=0x8c608228u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60822a; /* jsr @r2 */
L8c60822a: *stop=0x8c60822au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c60822c; /* cmp/pz r0 */
L8c60822c: take=(*sr&1u);*stop=0x8c60822eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;if(take)goto L8c608234;goto L8c608230; /* bt/s 0x8c608234 */
L8c608230: *stop=0x8c608232u;r[4]=r[9];goto L8c608280; /* bra 0x8c608280 */
L8c608234: *stop=0x8c608236u;r[4]=r[10];goto L8c608280; /* bra 0x8c608280 */
L8c608238: *stop=0x8c608238u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c60823a; /* mov.l @(0xc,r15),r11 */
L8c60823a: *stop=0x8c60823au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}goto L8c60823c; /* mov.l @r15,r8 */
L8c60823c: *stop=0x8c60823cu;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[11]=loaded;}goto L8c60823e; /* mov.l @r11,r11 */
L8c60823e: *stop=0x8c60823eu;if(!valid_access(r[8]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8]+0x8u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c608240; /* mov.l @(0x8,r8),r8 */
L8c608240: *stop=0x8c608240u;r[11]+=r[4];goto L8c608242; /* add r4,r11 */
L8c608242: *stop=0x8c608242u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608444u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c608244; /* mov.l 0x8c608444,r4 */
L8c608244: *stop=0x8c608244u;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c608246; /* mov.l @(0x8,r11),r11 */
L8c608246: *stop=0x8c608246u;r[8]&=r[4];goto L8c608248; /* and r4,r8 */
L8c608248: *stop=0x8c608248u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608440u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c60824a; /* mov.l 0x8c608440,r6 */
L8c60824a: *stop=0x8c60824au;r[8]>>=16;goto L8c60824c; /* shlr16 r8 */
L8c60824c: *stop=0x8c60824cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60843cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c60824e; /* mov.l 0x8c60843c,r1 */
L8c60824e: *stop=0x8c60824eu;r[8]>>=8;goto L8c608250; /* shlr8 r8 */
L8c608250: *stop=0x8c608250u;r[11]&=r[6];goto L8c608252; /* and r6,r11 */
L8c608252: *stop=0x8c608252u;r[6]=r[11];goto L8c608254; /* mov r11,r6 */
L8c608254: *stop=0x8c608254u;r[8]>>=2;goto L8c608256; /* shlr2 r8 */
L8c608256: target=r[1];*pr=0x8c60825au;*stop=0x8c608258u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60825a; /* jsr @r1 */
L8c60825a: *stop=0x8c60825au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c60825c; /* cmp/pz r0 */
L8c60825c: take=(*sr&1u);*stop=0x8c60825eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;if(take)goto L8c608264;goto L8c608260; /* bt/s 0x8c608264 */
L8c608260: *stop=0x8c608262u;r[4]=r[9];goto L8c60827e; /* bra 0x8c60827e */
L8c608264: *stop=0x8c608264u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608438u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608266; /* mov.l 0x8c608438,r3 */
L8c608266: *stop=0x8c608266u;r[6]=r[11];goto L8c608268; /* mov r11,r6 */
L8c608268: *stop=0x8c608268u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608448u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60826a; /* mov.l 0x8c608448,r2 */
L8c60826a: *stop=0x8c60826au;r[7]=r[8];goto L8c60826c; /* mov r8,r7 */
L8c60826c: *stop=0x8c60826cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c60826e; /* mov.l @r3,r5 */
L8c60826e: target=r[2];*pr=0x8c608272u;*stop=0x8c608270u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608272; /* jsr @r2 */
L8c608272: *stop=0x8c608272u;r[4]=r[0];goto L8c608274; /* mov r0,r4 */
L8c608274: *stop=0x8c608274u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c608276; /* cmp/pz r4 */
L8c608276: if((*sr&1u))goto L8c60827c;goto L8c608278; /* bt 0x8c60827c */
L8c608278: *stop=0x8c60827au;r[4]=r[13];goto L8c60827e; /* bra 0x8c60827e */
L8c60827c: *stop=0x8c60827cu;r[4]=r[10];goto L8c60827e; /* mov r10,r4 */
L8c60827e: *stop=0x8c60827eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[10],4);if(io_status)return io_status;goto L8c608280; /* mov.l r10,@(0x8,r15) */
L8c608280: *stop=0x8c608280u;r[0]=r[4];goto L8c608282; /* mov r4,r0 */
L8c608282: *stop=0x8c608282u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c608284; /* cmp/eq #0x0,r0 */
L8c608284: if((*sr&1u))goto L8c60829a;goto L8c608286; /* bt 0x8c60829a */
L8c608286: *stop=0x8c608286u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c608288; /* cmp/eq #0x1,r0 */
L8c608288: if(!(*sr&1u))goto L8c60828e;goto L8c60828a; /* bf 0x8c60828e */
L8c60828a: *stop=0x8c60828cu;goto L8c6084d6; /* bra 0x8c6084d6 */
L8c60828e: *stop=0x8c60828eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c608290; /* cmp/eq #0x2,r0 */
L8c608290: if(!(*sr&1u))goto L8c608296;goto L8c608292; /* bf 0x8c608296 */
L8c608292: *stop=0x8c608294u;goto L8c6085ee; /* bra 0x8c6085ee */
L8c608296: *stop=0x8c608298u;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c60829a: *stop=0x8c60829au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60829c; /* mov.l @(0x4,r15),r3 */
L8c60829c: *stop=0x8c60829cu;r[4]=r[15];goto L8c60829e; /* mov r15,r4 */
L8c60829e: *stop=0x8c60829eu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6082a0; /* mov.l @r12,r1 */
L8c6082a0: *stop=0x8c6082a0u;r[2]=r[3];goto L8c6082a2; /* mov r3,r2 */
L8c6082a2: *stop=0x8c6082a2u;r[3]<<=2;goto L8c6082a4; /* shll2 r3 */
L8c6082a4: *stop=0x8c6082a4u;r[3]<<=2;goto L8c6082a6; /* shll2 r3 */
L8c6082a6: *stop=0x8c6082a6u;r[3]+=r[2];goto L8c6082a8; /* add r2,r3 */
L8c6082a8: *stop=0x8c6082a8u;r[3]<<=2;goto L8c6082aa; /* shll2 r3 */
L8c6082aa: *stop=0x8c6082aau;r[3]+=r[1];goto L8c6082ac; /* add r1,r3 */
L8c6082ac: *stop=0x8c6082acu;r[2]=r[3];goto L8c6082ae; /* mov r3,r2 */
L8c6082ae: *stop=0x8c6082aeu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6082b0; /* mov.l r3,@r15 */
L8c6082b0: *stop=0x8c6082b0u;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6082b2; /* mov.l @(0x10,r2),r3 */
L8c6082b2: *stop=0x8c6082b2u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6082b4; /* mov.l r3,@(0x8,r15) */
L8c6082b4: target=0x8c6080c8u;*pr=0x8c6082b8u;*stop=0x8c6082b6u;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6082b8; /* bsr 0x8c6080c8 */
L8c6082b8: *stop=0x8c6082b8u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6082ba; /* mov.l @(0x8,r15),r2 */
L8c6082ba: *stop=0x8c6082bau;*sr=(*sr&~1u)|!(r[2]&r[9]);goto L8c6082bc; /* tst r2,r9 */
L8c6082bc: if(!(*sr&1u))goto L8c6082c2;goto L8c6082be; /* bf 0x8c6082c2 */
L8c6082be: *stop=0x8c6082c0u;goto L8c6083f4; /* bra 0x8c6083f4 */
L8c6082c2: *stop=0x8c6082c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6082c4; /* mov.l @r15,r0 */
L8c6082c4: *stop=0x8c6082c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60844cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6082c6; /* mov.l 0x8c60844c,r3 */
L8c6082c6: *stop=0x8c6082c6u;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6082c8; /* mov.l @(0x4,r0),r0 */
L8c6082c8: *stop=0x8c6082c8u;r[0]&=r[3];goto L8c6082ca; /* and r3,r0 */
L8c6082ca: *stop=0x8c6082cau;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c6082cc; /* cmp/eq #0x0,r0 */
L8c6082cc: if((*sr&1u))goto L8c6082de;goto L8c6082ce; /* bt 0x8c6082de */
L8c6082ce: *stop=0x8c6082ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c608450u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6082d0; /* mov.l 0x8c608450,r1 */
L8c6082d0: *stop=0x8c6082d0u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c6082d2; /* cmp/eq r1,r0 */
L8c6082d2: if((*sr&1u))goto L8c6082e4;goto L8c6082d4; /* bt 0x8c6082e4 */
L8c6082d4: *stop=0x8c6082d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608454u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6082d6; /* mov.l 0x8c608454,r1 */
L8c6082d6: *stop=0x8c6082d6u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c6082d8; /* cmp/eq r1,r0 */
L8c6082d8: if((*sr&1u))goto L8c6082e4;goto L8c6082da; /* bt 0x8c6082e4 */
L8c6082da: *stop=0x8c6082dcu;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c6082de: *stop=0x8c6082deu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6082e0; /* mov.l r8,@-r15 */
L8c6082e0: *stop=0x8c6082e2u;r[7]=2u;goto L8c6083c2; /* bra 0x8c6083c2 */
L8c6082e4: *stop=0x8c6082e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6082e6; /* mov.l @r15,r1 */
L8c6082e6: *stop=0x8c6082e6u;if(!valid_access(r[1]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6082e8; /* mov.l @(0x10,r1),r2 */
L8c6082e8: *stop=0x8c6082e8u;*sr=(*sr&~1u)|!(r[10]&r[2]);goto L8c6082ea; /* tst r10,r2 */
L8c6082ea: if(!(*sr&1u))goto L8c6083be;goto L8c6082ec; /* bf 0x8c6083be */
L8c6082ec: *stop=0x8c6082ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c608458u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6082ee; /* mov.l 0x8c608458,r2 */
L8c6082ee: *stop=0x8c6082eeu;r[5]=r[15];goto L8c6082f0; /* mov r15,r5 */
L8c6082f0: *stop=0x8c6082f0u;r[5]+=36u;goto L8c6082f2; /* add #0x24,r5 */
L8c6082f2: target=r[2];*pr=0x8c6082f6u;*stop=0x8c6082f4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6082f6; /* jsr @r2 */
L8c6082f6: *stop=0x8c6082f6u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6082f8; /* tst r0,r0 */
L8c6082f8: take=!(*sr&1u);*stop=0x8c6082fau;r[9]=r[0];if(take)goto L8c60833c;goto L8c6082fc; /* bf/s 0x8c60833c */
L8c6082fc: *stop=0x8c6082fcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6082fe; /* mov.l r8,@-r15 */
L8c6082fe: *stop=0x8c6082feu;r[7]=2u;goto L8c608300; /* mov #0x2,r7 */
L8c608300: *stop=0x8c608300u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c608302; /* mov.l @(0x8,r15),r5 */
L8c608302: *stop=0x8c608302u;r[6]=r[11];goto L8c608304; /* mov r11,r6 */
L8c608304: *stop=0x8c608304u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c608306; /* mov.l @r12,r2 */
L8c608306: *stop=0x8c608306u;r[3]=r[5];goto L8c608308; /* mov r5,r3 */
L8c608308: *stop=0x8c608308u;r[5]<<=2;goto L8c60830a; /* shll2 r5 */
L8c60830a: *stop=0x8c60830au;r[5]<<=2;goto L8c60830c; /* shll2 r5 */
L8c60830c: *stop=0x8c60830cu;r[5]+=r[3];goto L8c60830e; /* add r3,r5 */
L8c60830e: *stop=0x8c60830eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60845cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608310; /* mov.l 0x8c60845c,r3 */
L8c608310: *stop=0x8c608310u;r[5]<<=2;goto L8c608312; /* shll2 r5 */
L8c608312: *stop=0x8c608312u;r[5]+=r[2];goto L8c608314; /* add r2,r5 */
L8c608314: *stop=0x8c608314u;r[5]+=20u;goto L8c608316; /* add #0x14,r5 */
L8c608316: target=r[3];*pr=0x8c60831au;*stop=0x8c608318u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60831a; /* jsr @r3 */
L8c60831a: *stop=0x8c60831au;r[15]+=4u;goto L8c60831c; /* add #0x4,r15 */
L8c60831c: *stop=0x8c60831cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60831e; /* mov.l @r14,r2 */
L8c60831e: *stop=0x8c60831eu;r[0]=66u;goto L8c608320; /* mov #0x42,r0 */
L8c608320: *stop=0x8c608320u;r[9]=0u-r[9];goto L8c608322; /* neg r9,r9 */
L8c608322: *stop=0x8c608322u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[9],2);if(io_status)return io_status;goto L8c608324; /* mov.w r9,@(r0,r2) */
L8c608324: *stop=0x8c608324u;r[0]=64u;goto L8c608326; /* mov #0x40,r0 */
L8c608326: *stop=0x8c608326u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608328; /* mov.l @r14,r3 */
L8c608328: *stop=0x8c608328u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[10],2);if(io_status)return io_status;goto L8c60832a; /* mov.w r10,@(r0,r3) */
L8c60832a: *stop=0x8c60832au;{uint32_t loaded=rd(m,bus,&io_status,0x8c608460u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60832c; /* mov.l 0x8c608460,r3 */
L8c60832c: target=r[3];*pr=0x8c608330u;*stop=0x8c60832eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608330; /* jsr @r3 */
L8c608330: *stop=0x8c608330u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608464u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608332; /* mov.l 0x8c608464,r2 */
L8c608332: *stop=0x8c608332u;r[5]=r[8];goto L8c608334; /* mov r8,r5 */
L8c608334: target=r[2];*pr=0x8c608338u;*stop=0x8c608336u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608338; /* jsr @r2 */
L8c608338: *stop=0x8c60833au;goto L8c608770; /* bra 0x8c608770 */
L8c60833c: *stop=0x8c60833cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c60833e; /* mov.l r8,@-r15 */
L8c60833e: *stop=0x8c60833eu;r[7]=2u;goto L8c608340; /* mov #0x2,r7 */
L8c608340: *stop=0x8c608340u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c608342; /* mov.l @(0x8,r15),r5 */
L8c608342: *stop=0x8c608342u;r[6]=r[11];goto L8c608344; /* mov r11,r6 */
L8c608344: *stop=0x8c608344u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c608346; /* mov.l @r12,r2 */
L8c608346: *stop=0x8c608346u;r[3]=r[5];goto L8c608348; /* mov r5,r3 */
L8c608348: *stop=0x8c608348u;r[5]<<=2;goto L8c60834a; /* shll2 r5 */
L8c60834a: *stop=0x8c60834au;r[5]<<=2;goto L8c60834c; /* shll2 r5 */
L8c60834c: *stop=0x8c60834cu;r[5]+=r[3];goto L8c60834e; /* add r3,r5 */
L8c60834e: *stop=0x8c60834eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60845cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608350; /* mov.l 0x8c60845c,r3 */
L8c608350: *stop=0x8c608350u;r[5]<<=2;goto L8c608352; /* shll2 r5 */
L8c608352: *stop=0x8c608352u;r[5]+=r[2];goto L8c608354; /* add r2,r5 */
L8c608354: *stop=0x8c608354u;r[5]+=20u;goto L8c608356; /* add #0x14,r5 */
L8c608356: target=r[3];*pr=0x8c60835au;*stop=0x8c608358u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60835a; /* jsr @r3 */
L8c60835a: *stop=0x8c60835au;r[15]+=4u;goto L8c60835c; /* add #0x4,r15 */
L8c60835c: *stop=0x8c60835cu;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60835e; /* mov.l @(0x24,r15),r3 */
L8c60835e: *stop=0x8c60835eu;r[0]=66u;goto L8c608360; /* mov #0x42,r0 */
L8c608360: *stop=0x8c608360u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c608362; /* mov.l @r14,r2 */
L8c608362: *stop=0x8c608362u;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[3],4);if(io_status)return io_status;goto L8c608364; /* mov.l r3,@(0x14,r2) */
L8c608364: *stop=0x8c608364u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c608366; /* mov.l @r14,r2 */
L8c608366: *stop=0x8c608366u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[13],2);if(io_status)return io_status;goto L8c608368; /* mov.w r13,@(r0,r2) */
L8c608368: *stop=0x8c608368u;r[0]=64u;goto L8c60836a; /* mov #0x40,r0 */
L8c60836a: *stop=0x8c60836au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60836c; /* mov.l @r14,r3 */
L8c60836c: *stop=0x8c60836cu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[10],2);if(io_status)return io_status;goto L8c60836e; /* mov.w r10,@(r0,r3) */
L8c60836e: *stop=0x8c60836eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c608460u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608370; /* mov.l 0x8c608460,r3 */
L8c608370: target=r[3];*pr=0x8c608374u;*stop=0x8c608372u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608374; /* jsr @r3 */
L8c608374: *stop=0x8c608374u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608464u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608376; /* mov.l 0x8c608464,r2 */
L8c608376: *stop=0x8c608376u;r[5]=r[8];goto L8c608378; /* mov r8,r5 */
L8c608378: target=r[2];*pr=0x8c60837cu;*stop=0x8c60837au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60837c; /* jsr @r2 */
L8c60837c: *stop=0x8c60837eu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[13],4);if(io_status)return io_status;goto L8c6083b0; /* bra 0x8c6083b0 */
L8c608380: *stop=0x8c608380u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c608382; /* mov.l @(0x10,r15),r9 */
L8c608382: *stop=0x8c608382u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c608384; /* mov.l @r12,r2 */
L8c608384: *stop=0x8c608384u;r[3]=r[9];goto L8c608386; /* mov r9,r3 */
L8c608386: *stop=0x8c608386u;r[9]<<=2;goto L8c608388; /* shll2 r9 */
L8c608388: *stop=0x8c608388u;r[9]<<=2;goto L8c60838a; /* shll2 r9 */
L8c60838a: *stop=0x8c60838au;r[9]+=r[3];goto L8c60838c; /* add r3,r9 */
L8c60838c: *stop=0x8c60838cu;r[9]<<=2;goto L8c60838e; /* shll2 r9 */
L8c60838e: *stop=0x8c60838eu;r[2]+=r[9];goto L8c608390; /* add r9,r2 */
L8c608390: *stop=0x8c608390u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608392; /* mov.l @r2,r3 */
L8c608392: *stop=0x8c608392u;*sr=(*sr&~1u)|(r[11]==r[3]);goto L8c608394; /* cmp/eq r11,r3 */
L8c608394: if(!(*sr&1u))goto L8c6083aa;goto L8c608396; /* bf 0x8c6083aa */
L8c608396: *stop=0x8c608396u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608398; /* mov.l @r12,r3 */
L8c608398: *stop=0x8c608398u;r[3]+=r[9];goto L8c60839a; /* add r9,r3 */
L8c60839a: *stop=0x8c60839au;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60839c; /* mov.l @(0x10,r3),r2 */
L8c60839c: *stop=0x8c60839cu;r[2]|=r[10];goto L8c60839e; /* or r10,r2 */
L8c60839e: *stop=0x8c60839eu;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6083a0; /* mov.l r2,@(0x10,r3) */
L8c6083a0: *stop=0x8c6083a0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6083a2; /* mov.l @r14,r2 */
L8c6083a2: *stop=0x8c6083a2u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6083a4; /* mov.l @r12,r3 */
L8c6083a4: *stop=0x8c6083a4u;if(!valid_access(r[2]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6083a6; /* mov.l @(0x14,r2),r1 */
L8c6083a6: *stop=0x8c6083a6u;r[3]+=r[9];goto L8c6083a8; /* add r9,r3 */
L8c6083a8: *stop=0x8c6083a8u;if(!valid_access(r[3]+20u,4u))return -3;wr(m,bus,&io_status,r[3]+0x14u,r[1],4);if(io_status)return io_status;goto L8c6083aa; /* mov.l r1,@(0x14,r3) */
L8c6083aa: *stop=0x8c6083aau;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6083ac; /* mov.l @(0x10,r15),r3 */
L8c6083ac: *stop=0x8c6083acu;r[3]+=1u;goto L8c6083ae; /* add #0x1,r3 */
L8c6083ae: *stop=0x8c6083aeu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6083b0; /* mov.l r3,@(0x10,r15) */
L8c6083b0: *stop=0x8c6083b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608438u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6083b2; /* mov.l 0x8c608438,r3 */
L8c6083b2: *stop=0x8c6083b2u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6083b4; /* mov.l @(0x10,r15),r1 */
L8c6083b4: *stop=0x8c6083b4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6083b6; /* mov.l @r3,r2 */
L8c6083b6: *stop=0x8c6083b6u;*sr=(*sr&~1u)|(r[1]>=r[2]);goto L8c6083b8; /* cmp/hs r2,r1 */
L8c6083b8: if(!(*sr&1u))goto L8c608380;goto L8c6083ba; /* bf 0x8c608380 */
L8c6083ba: *stop=0x8c6083bcu;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c6083be: *stop=0x8c6083beu;r[7]=3u;goto L8c6083c0; /* mov #0x3,r7 */
L8c6083c0: *stop=0x8c6083c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6083c2; /* mov.l r8,@-r15 */
L8c6083c2: *stop=0x8c6083c2u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6083c4; /* mov.l @(0x8,r15),r5 */
L8c6083c4: *stop=0x8c6083c4u;r[6]=r[11];goto L8c6083c6; /* mov r11,r6 */
L8c6083c6: *stop=0x8c6083c6u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6083c8; /* mov.l @r12,r1 */
L8c6083c8: *stop=0x8c6083c8u;r[2]=r[5];goto L8c6083ca; /* mov r5,r2 */
L8c6083ca: *stop=0x8c6083cau;r[5]<<=2;goto L8c6083cc; /* shll2 r5 */
L8c6083cc: *stop=0x8c6083ccu;r[5]<<=2;goto L8c6083ce; /* shll2 r5 */
L8c6083ce: *stop=0x8c6083ceu;r[5]+=r[2];goto L8c6083d0; /* add r2,r5 */
L8c6083d0: *stop=0x8c6083d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60845cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6083d2; /* mov.l 0x8c60845c,r2 */
L8c6083d2: *stop=0x8c6083d2u;r[5]<<=2;goto L8c6083d4; /* shll2 r5 */
L8c6083d4: *stop=0x8c6083d4u;r[5]+=r[1];goto L8c6083d6; /* add r1,r5 */
L8c6083d6: *stop=0x8c6083d6u;r[5]+=20u;goto L8c6083d8; /* add #0x14,r5 */
L8c6083d8: target=r[2];*pr=0x8c6083dcu;*stop=0x8c6083dau;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6083dc; /* jsr @r2 */
L8c6083dc: *stop=0x8c6083dcu;r[15]+=4u;goto L8c6083de; /* add #0x4,r15 */
L8c6083de: *stop=0x8c6083deu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6083e0; /* mov.l @r14,r3 */
L8c6083e0: *stop=0x8c6083e0u;r[0]=66u;goto L8c6083e2; /* mov #0x42,r0 */
L8c6083e2: *stop=0x8c6083e2u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[13],2);if(io_status)return io_status;goto L8c6083e4; /* mov.w r13,@(r0,r3) */
L8c6083e4: *stop=0x8c6083e4u;r[0]=64u;goto L8c6083e6; /* mov #0x40,r0 */
L8c6083e6: *stop=0x8c6083e6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6083e8; /* mov.l @r14,r3 */
L8c6083e8: *stop=0x8c6083e8u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[10],2);if(io_status)return io_status;goto L8c6083ea; /* mov.w r10,@(r0,r3) */
L8c6083ea: *stop=0x8c6083eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c608460u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6083ec; /* mov.l 0x8c608460,r3 */
L8c6083ec: target=r[3];*pr=0x8c6083f0u;*stop=0x8c6083eeu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6083f0; /* jsr @r3 */
L8c6083f0: *stop=0x8c6083f2u;goto L8c6087a0; /* bra 0x8c6087a0 */
L8c6083f4: *stop=0x8c6083f4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6083f6; /* mov.l @r15,r0 */
L8c6083f6: *stop=0x8c6083f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60844cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6083f8; /* mov.l 0x8c60844c,r3 */
L8c6083f8: *stop=0x8c6083f8u;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6083fa; /* mov.l @(0x4,r0),r0 */
L8c6083fa: *stop=0x8c6083fau;r[0]&=r[3];goto L8c6083fc; /* and r3,r0 */
L8c6083fc: *stop=0x8c6083fcu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c6083fe; /* cmp/eq #0x0,r0 */
L8c6083fe: if((*sr&1u))goto L8c608468;goto L8c608400; /* bt 0x8c608468 */
L8c608400: *stop=0x8c608400u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608450u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c608402; /* mov.l 0x8c608450,r1 */
L8c608402: *stop=0x8c608402u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c608404; /* cmp/eq r1,r0 */
L8c608404: if((*sr&1u))goto L8c608410;goto L8c608406; /* bt 0x8c608410 */
L8c608406: *stop=0x8c608406u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608454u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c608408; /* mov.l 0x8c608454,r1 */
L8c608408: *stop=0x8c608408u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c60840a; /* cmp/eq r1,r0 */
L8c60840a: if((*sr&1u))goto L8c608468;goto L8c60840c; /* bt 0x8c608468 */
L8c60840c: *stop=0x8c60840eu;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c608410: *stop=0x8c608410u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c608412; /* mov.l r8,@-r15 */
L8c608412: *stop=0x8c608412u;r[7]=1u;goto L8c608414; /* mov #0x1,r7 */
L8c608414: *stop=0x8c608414u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c608416; /* mov.l @(0x8,r15),r5 */
L8c608416: *stop=0x8c608416u;r[6]=r[11];goto L8c608418; /* mov r11,r6 */
L8c608418: *stop=0x8c608418u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[1]=loaded;}goto L8c60841a; /* mov.l @r12,r1 */
L8c60841a: *stop=0x8c60841au;r[2]=r[5];goto L8c60841c; /* mov r5,r2 */
L8c60841c: *stop=0x8c60841cu;r[5]<<=2;goto L8c60841e; /* shll2 r5 */
L8c60841e: *stop=0x8c60841eu;r[5]<<=2;goto L8c608420; /* shll2 r5 */
L8c608420: *stop=0x8c608420u;r[5]+=r[2];goto L8c608422; /* add r2,r5 */
L8c608422: *stop=0x8c608422u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60845cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608424; /* mov.l 0x8c60845c,r2 */
L8c608424: *stop=0x8c608424u;r[5]<<=2;goto L8c608426; /* shll2 r5 */
L8c608426: *stop=0x8c608426u;r[5]+=r[1];goto L8c608428; /* add r1,r5 */
L8c608428: *stop=0x8c608428u;r[5]+=20u;goto L8c60842a; /* add #0x14,r5 */
L8c60842a: target=r[2];*pr=0x8c60842eu;*stop=0x8c60842cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60842e; /* jsr @r2 */
L8c60842e: *stop=0x8c608430u;goto L8c6086b4; /* bra 0x8c6086b4 */
L8c608468: *stop=0x8c608468u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c60846a; /* mov.l @(0x4,r15),r5 */
L8c60846a: *stop=0x8c60846au;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60846c; /* mov.l @r12,r2 */
L8c60846c: *stop=0x8c60846cu;r[3]=r[5];goto L8c60846e; /* mov r5,r3 */
L8c60846e: *stop=0x8c60846eu;r[5]<<=2;goto L8c608470; /* shll2 r5 */
L8c608470: *stop=0x8c608470u;r[5]<<=2;goto L8c608472; /* shll2 r5 */
L8c608472: *stop=0x8c608472u;r[5]+=r[3];goto L8c608474; /* add r3,r5 */
L8c608474: *stop=0x8c608474u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608476; /* mov.l 0x8c6086b8,r3 */
L8c608476: *stop=0x8c608476u;r[5]<<=2;goto L8c608478; /* shll2 r5 */
L8c608478: *stop=0x8c608478u;r[5]+=r[2];goto L8c60847a; /* add r2,r5 */
L8c60847a: *stop=0x8c60847au;r[5]+=20u;goto L8c60847c; /* add #0x14,r5 */
L8c60847c: target=r[3];*pr=0x8c608480u;*stop=0x8c60847eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608480; /* jsr @r3 */
L8c608480: *stop=0x8c608480u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c608482; /* cmp/pz r0 */
L8c608482: take=(*sr&1u);*stop=0x8c608484u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[0],4);if(io_status)return io_status;if(take)goto L8c6084a8;goto L8c608486; /* bt/s 0x8c6084a8 */
L8c608486: *stop=0x8c608486u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c608488; /* mov.l r8,@-r15 */
L8c608488: *stop=0x8c608488u;r[7]=1u;goto L8c60848a; /* mov #0x1,r7 */
L8c60848a: *stop=0x8c60848au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c60848c; /* mov.l @(0x8,r15),r5 */
L8c60848c: *stop=0x8c60848cu;r[6]=r[11];goto L8c60848e; /* mov r11,r6 */
L8c60848e: *stop=0x8c60848eu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c608490; /* mov.l @r12,r2 */
L8c608490: *stop=0x8c608490u;r[3]=r[5];goto L8c608492; /* mov r5,r3 */
L8c608492: *stop=0x8c608492u;r[5]<<=2;goto L8c608494; /* shll2 r5 */
L8c608494: *stop=0x8c608494u;r[5]<<=2;goto L8c608496; /* shll2 r5 */
L8c608496: *stop=0x8c608496u;r[5]+=r[3];goto L8c608498; /* add r3,r5 */
L8c608498: *stop=0x8c608498u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60849a; /* mov.l 0x8c6086bc,r3 */
L8c60849a: *stop=0x8c60849au;r[5]<<=2;goto L8c60849c; /* shll2 r5 */
L8c60849c: *stop=0x8c60849cu;r[5]+=r[2];goto L8c60849e; /* add r2,r5 */
L8c60849e: *stop=0x8c60849eu;r[5]+=20u;goto L8c6084a0; /* add #0x14,r5 */
L8c6084a0: target=r[3];*pr=0x8c6084a4u;*stop=0x8c6084a2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6084a4; /* jsr @r3 */
L8c6084a4: *stop=0x8c6084a6u;r[15]+=4u;goto L8c6085a8; /* bra 0x8c6085a8 */
L8c6084a8: *stop=0x8c6084a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6084aa; /* mov.l r8,@-r15 */
L8c6084aa: *stop=0x8c6084aau;r[7]=1u;goto L8c6084ac; /* mov #0x1,r7 */
L8c6084ac: *stop=0x8c6084acu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6084ae; /* mov.l @(0x8,r15),r5 */
L8c6084ae: *stop=0x8c6084aeu;r[6]=r[11];goto L8c6084b0; /* mov r11,r6 */
L8c6084b0: *stop=0x8c6084b0u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6084b2; /* mov.l @r12,r2 */
L8c6084b2: *stop=0x8c6084b2u;r[3]=r[5];goto L8c6084b4; /* mov r5,r3 */
L8c6084b4: *stop=0x8c6084b4u;r[5]<<=2;goto L8c6084b6; /* shll2 r5 */
L8c6084b6: *stop=0x8c6084b6u;r[5]<<=2;goto L8c6084b8; /* shll2 r5 */
L8c6084b8: *stop=0x8c6084b8u;r[5]+=r[3];goto L8c6084ba; /* add r3,r5 */
L8c6084ba: *stop=0x8c6084bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6084bc; /* mov.l 0x8c6086bc,r3 */
L8c6084bc: *stop=0x8c6084bcu;r[5]<<=2;goto L8c6084be; /* shll2 r5 */
L8c6084be: *stop=0x8c6084beu;r[5]+=r[2];goto L8c6084c0; /* add r2,r5 */
L8c6084c0: *stop=0x8c6084c0u;r[5]+=20u;goto L8c6084c2; /* add #0x14,r5 */
L8c6084c2: target=r[3];*pr=0x8c6084c6u;*stop=0x8c6084c4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6084c6; /* jsr @r3 */
L8c6084c6: *stop=0x8c6084c6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6084c8; /* mov.l @r14,r2 */
L8c6084c8: *stop=0x8c6084c8u;r[0]=66u;goto L8c6084ca; /* mov #0x42,r0 */
L8c6084ca: *stop=0x8c6084cau;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[13],2);if(io_status)return io_status;goto L8c6084cc; /* mov.w r13,@(r0,r2) */
L8c6084cc: *stop=0x8c6084ccu;r[0]=64u;goto L8c6084ce; /* mov #0x40,r0 */
L8c6084ce: *stop=0x8c6084ceu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6084d0; /* mov.l @r14,r3 */
L8c6084d0: *stop=0x8c6084d0u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[10],2);if(io_status)return io_status;goto L8c6084d2; /* mov.w r10,@(r0,r3) */
L8c6084d2: *stop=0x8c6084d4u;r[15]+=4u;goto L8c6085de; /* bra 0x8c6085de */
L8c6084d6: *stop=0x8c6084d6u;r[4]=r[15];goto L8c6084d8; /* mov r15,r4 */
L8c6084d8: target=0x8c6080c8u;*pr=0x8c6084dcu;*stop=0x8c6084dau;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6084dc; /* bsr 0x8c6080c8 */
L8c6084dc: *stop=0x8c6084dcu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6084de; /* mov.l @(0x4,r15),r3 */
L8c6084de: *stop=0x8c6084deu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6084e0; /* mov.l @r12,r1 */
L8c6084e0: *stop=0x8c6084e0u;r[2]=r[3];goto L8c6084e2; /* mov r3,r2 */
L8c6084e2: *stop=0x8c6084e2u;r[3]<<=2;goto L8c6084e4; /* shll2 r3 */
L8c6084e4: *stop=0x8c6084e4u;r[3]<<=2;goto L8c6084e6; /* shll2 r3 */
L8c6084e6: *stop=0x8c6084e6u;r[3]+=r[2];goto L8c6084e8; /* add r2,r3 */
L8c6084e8: *stop=0x8c6084e8u;r[3]<<=2;goto L8c6084ea; /* shll2 r3 */
L8c6084ea: *stop=0x8c6084eau;r[3]+=r[1];goto L8c6084ec; /* add r1,r3 */
L8c6084ec: *stop=0x8c6084ecu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c6084ee; /* mov.l r3,@r14 */
L8c6084ee: *stop=0x8c6084eeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6084f0; /* mov.l @r15,r0 */
L8c6084f0: *stop=0x8c6084f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6084f2; /* mov.l 0x8c6086c0,r3 */
L8c6084f2: *stop=0x8c6084f2u;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6084f4; /* mov.l @(0x4,r0),r0 */
L8c6084f4: *stop=0x8c6084f4u;r[0]&=r[3];goto L8c6084f6; /* and r3,r0 */
L8c6084f6: *stop=0x8c6084f6u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c6084f8; /* cmp/eq #0x0,r0 */
L8c6084f8: if((*sr&1u))goto L8c60850a;goto L8c6084fa; /* bt 0x8c60850a */
L8c6084fa: *stop=0x8c6084fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086c4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6084fc; /* mov.l 0x8c6086c4,r1 */
L8c6084fc: *stop=0x8c6084fcu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c6084fe; /* cmp/eq r1,r0 */
L8c6084fe: if((*sr&1u))goto L8c60856a;goto L8c608500; /* bt 0x8c60856a */
L8c608500: *stop=0x8c608500u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086c8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c608502; /* mov.l 0x8c6086c8,r1 */
L8c608502: *stop=0x8c608502u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c608504; /* cmp/eq r1,r0 */
L8c608504: if((*sr&1u))goto L8c608558;goto L8c608506; /* bt 0x8c608558 */
L8c608506: *stop=0x8c608508u;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c60850a: *stop=0x8c60850au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60850c; /* mov.l @r14,r2 */
L8c60850c: *stop=0x8c60850cu;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c60850e; /* mov.l @(0x10,r2),r1 */
L8c60850e: *stop=0x8c60850eu;*sr=(*sr&~1u)|!(r[9]&r[1]);goto L8c608510; /* tst r9,r1 */
L8c608510: if((*sr&1u))goto L8c608520;goto L8c608512; /* bt 0x8c608520 */
L8c608512: *stop=0x8c608512u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c608514; /* mov.l @r14,r0 */
L8c608514: *stop=0x8c608514u;r[2]=64u;goto L8c608516; /* mov #0x40,r2 */
L8c608516: *stop=0x8c608516u;if(!valid_access(r[0]+r[2],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[2],2);if(io_status)return io_status;goto L8c608518; /* mov.w @(r0,r2),r1 */
L8c608518: *stop=0x8c608518u;r[1]+=1u;goto L8c60851a; /* add #0x1,r1 */
L8c60851a: *stop=0x8c60851au;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c60851c; /* mov.w r1,@(r0,r2) */
L8c60851c: *stop=0x8c60851eu;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c608520: *stop=0x8c608520u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c608522; /* mov.l @r14,r5 */
L8c608522: *stop=0x8c608522u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608524; /* mov.l 0x8c6086b8,r2 */
L8c608524: *stop=0x8c608524u;r[5]+=20u;goto L8c608526; /* add #0x14,r5 */
L8c608526: target=r[2];*pr=0x8c60852au;*stop=0x8c608528u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60852a; /* jsr @r2 */
L8c60852a: *stop=0x8c60852au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c60852c; /* cmp/pz r0 */
L8c60852c: take=!(*sr&1u);*stop=0x8c60852eu;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[0],4);if(io_status)return io_status;if(take)goto L8c6085a8;goto L8c608530; /* bf/s 0x8c6085a8 */
L8c608530: *stop=0x8c608530u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c608532; /* mov.l @r14,r2 */
L8c608532: *stop=0x8c608532u;r[0]=66u;goto L8c608534; /* mov #0x42,r0 */
L8c608534: *stop=0x8c608534u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[13],2);if(io_status)return io_status;goto L8c608536; /* mov.w r13,@(r0,r2) */
L8c608536: *stop=0x8c608536u;r[0]=64u;goto L8c608538; /* mov #0x40,r0 */
L8c608538: *stop=0x8c608538u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60853a; /* mov.l @r14,r3 */
L8c60853a: *stop=0x8c60853au;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60853c; /* mov.l @(0x10,r3),r2 */
L8c60853c: *stop=0x8c60853cu;r[2]|=r[9];goto L8c60853e; /* or r9,r2 */
L8c60853e: *stop=0x8c60853eu;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[2],4);if(io_status)return io_status;goto L8c608540; /* mov.l r2,@(0x10,r3) */
L8c608540: *stop=0x8c608540u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608542; /* mov.l @r14,r3 */
L8c608542: *stop=0x8c608542u;if(!valid_access(r[0]+r[3],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[3],2);if(io_status)return io_status;goto L8c608544; /* mov.w @(r0,r3),r2 */
L8c608544: *stop=0x8c608544u;r[2]+=1u;goto L8c608546; /* add #0x1,r2 */
L8c608546: *stop=0x8c608546u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[2],2);if(io_status)return io_status;goto L8c608548; /* mov.w r2,@(r0,r3) */
L8c608548: *stop=0x8c608548u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60854a; /* mov.l 0x8c6086cc,r3 */
L8c60854a: target=r[3];*pr=0x8c60854eu;*stop=0x8c60854cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60854e; /* jsr @r3 */
L8c60854e: *stop=0x8c60854eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608550; /* mov.l @(0x8,r15),r2 */
L8c608550: *stop=0x8c608550u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c608552; /* tst r2,r2 */
L8c608552: if(!(*sr&1u))goto L8c6085de;goto L8c608554; /* bf 0x8c6085de */
L8c608554: *stop=0x8c608556u;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c608558: *stop=0x8c608558u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c60855a; /* mov.l @r14,r1 */
L8c60855a: *stop=0x8c60855au;if(!valid_access(r[1]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60855c; /* mov.l @(0x10,r1),r2 */
L8c60855c: *stop=0x8c60855cu;*sr=(*sr&~1u)|!(r[9]&r[2]);goto L8c60855e; /* tst r9,r2 */
L8c60855e: if((*sr&1u))goto L8c608598;goto L8c608560; /* bt 0x8c608598 */
L8c608560: *stop=0x8c608560u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c608562; /* mov.l @r14,r0 */
L8c608562: *stop=0x8c608562u;r[2]=64u;goto L8c608564; /* mov #0x40,r2 */
L8c608564: *stop=0x8c608564u;if(!valid_access(r[0]+r[2],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[2],2);if(io_status)return io_status;goto L8c608566; /* mov.w @(r0,r2),r1 */
L8c608566: *stop=0x8c608566u;r[1]+=1u;goto L8c608568; /* add #0x1,r1 */
L8c608568: *stop=0x8c608568u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c60856a; /* mov.w r1,@(r0,r2) */
L8c60856a: *stop=0x8c60856au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60856c; /* mov.l @r14,r3 */
L8c60856c: *stop=0x8c60856cu;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60856e; /* mov.l @(0x10,r3),r2 */
L8c60856e: *stop=0x8c60856eu;*sr=(*sr&~1u)|!(r[10]&r[2]);goto L8c608570; /* tst r10,r2 */
L8c608570: if((*sr&1u))goto L8c608576;goto L8c608572; /* bt 0x8c608576 */
L8c608572: *stop=0x8c608574u;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c608576: *stop=0x8c608576u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608578; /* mov.l 0x8c6086d0,r2 */
L8c608578: *stop=0x8c608578u;r[5]=r[15];goto L8c60857a; /* mov r15,r5 */
L8c60857a: *stop=0x8c60857au;r[5]+=36u;goto L8c60857c; /* add #0x24,r5 */
L8c60857c: target=r[2];*pr=0x8c608580u;*stop=0x8c60857eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608580; /* jsr @r2 */
L8c608580: *stop=0x8c608580u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c608582; /* tst r0,r0 */
L8c608582: take=(*sr&1u);*stop=0x8c608584u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[0],4);if(io_status)return io_status;if(take)goto L8c6085a8;goto L8c608586; /* bt/s 0x8c6085a8 */
L8c608586: *stop=0x8c608586u;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608588; /* mov.l @(0x24,r15),r2 */
L8c608588: *stop=0x8c608588u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60858a; /* mov.l @r14,r3 */
L8c60858a: *stop=0x8c60858au;if(!valid_access(r[3]+20u,4u))return -3;wr(m,bus,&io_status,r[3]+0x14u,r[2],4);if(io_status)return io_status;goto L8c60858c; /* mov.l r2,@(0x14,r3) */
L8c60858c: *stop=0x8c60858cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60858e; /* mov.l @r14,r3 */
L8c60858e: *stop=0x8c60858eu;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608590; /* mov.l @(0x10,r3),r2 */
L8c608590: *stop=0x8c608590u;r[2]|=r[10];goto L8c608592; /* or r10,r2 */
L8c608592: *stop=0x8c608592u;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[2],4);if(io_status)return io_status;goto L8c608594; /* mov.l r2,@(0x10,r3) */
L8c608594: *stop=0x8c608596u;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c608598: *stop=0x8c608598u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c60859a; /* mov.l @r14,r5 */
L8c60859a: *stop=0x8c60859au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60859c; /* mov.l 0x8c6086b8,r2 */
L8c60859c: *stop=0x8c60859cu;r[5]+=20u;goto L8c60859e; /* add #0x14,r5 */
L8c60859e: target=r[2];*pr=0x8c6085a2u;*stop=0x8c6085a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6085a2; /* jsr @r2 */
L8c6085a2: *stop=0x8c6085a2u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c6085a4; /* cmp/pz r0 */
L8c6085a4: take=(*sr&1u);*stop=0x8c6085a6u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[0],4);if(io_status)return io_status;if(take)goto L8c6085b6;goto L8c6085a8; /* bt/s 0x8c6085b6 */
L8c6085a8: *stop=0x8c6085a8u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6085aa; /* mov.l @(0x20,r15),r3 */
L8c6085aa: *stop=0x8c6085aau;r[0]=66u;goto L8c6085ac; /* mov #0x42,r0 */
L8c6085ac: *stop=0x8c6085acu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6085ae; /* mov.l @r14,r2 */
L8c6085ae: *stop=0x8c6085aeu;r[3]=0u-r[3];goto L8c6085b0; /* neg r3,r3 */
L8c6085b0: *stop=0x8c6085b0u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[3],2);if(io_status)return io_status;goto L8c6085b2; /* mov.w r3,@(r0,r2) */
L8c6085b2: *stop=0x8c6085b4u;goto L8c608770; /* bra 0x8c608770 */
L8c6085b6: *stop=0x8c6085b6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6085b8; /* mov.l @r14,r2 */
L8c6085b8: *stop=0x8c6085b8u;r[0]=66u;goto L8c6085ba; /* mov #0x42,r0 */
L8c6085ba: *stop=0x8c6085bau;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[13],2);if(io_status)return io_status;goto L8c6085bc; /* mov.w r13,@(r0,r2) */
L8c6085bc: *stop=0x8c6085bcu;r[0]=64u;goto L8c6085be; /* mov #0x40,r0 */
L8c6085be: *stop=0x8c6085beu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6085c0; /* mov.l @r14,r3 */
L8c6085c0: *stop=0x8c6085c0u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6085c2; /* mov.l @(0x10,r3),r2 */
L8c6085c2: *stop=0x8c6085c2u;r[2]|=r[9];goto L8c6085c4; /* or r9,r2 */
L8c6085c4: *stop=0x8c6085c4u;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6085c6; /* mov.l r2,@(0x10,r3) */
L8c6085c6: *stop=0x8c6085c6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6085c8; /* mov.l @r14,r3 */
L8c6085c8: *stop=0x8c6085c8u;if(!valid_access(r[0]+r[3],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[3],2);if(io_status)return io_status;goto L8c6085ca; /* mov.w @(r0,r3),r2 */
L8c6085ca: *stop=0x8c6085cau;r[2]+=1u;goto L8c6085cc; /* add #0x1,r2 */
L8c6085cc: *stop=0x8c6085ccu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[2],2);if(io_status)return io_status;goto L8c6085ce; /* mov.w r2,@(r0,r3) */
L8c6085ce: *stop=0x8c6085ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6085d0; /* mov.l 0x8c6086cc,r3 */
L8c6085d0: target=r[3];*pr=0x8c6085d4u;*stop=0x8c6085d2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6085d4; /* jsr @r3 */
L8c6085d4: *stop=0x8c6085d4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6085d6; /* mov.l @(0x8,r15),r2 */
L8c6085d6: *stop=0x8c6085d6u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6085d8; /* tst r2,r2 */
L8c6085d8: if(!(*sr&1u))goto L8c6085de;goto L8c6085da; /* bf 0x8c6085de */
L8c6085da: *stop=0x8c6085dcu;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c6085de: *stop=0x8c6085deu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6085e0; /* mov.l @r14,r6 */
L8c6085e0: *stop=0x8c6085e0u;r[5]=2u;goto L8c6085e2; /* mov #0x2,r5 */
L8c6085e2: *stop=0x8c6085e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086d4u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c6085e4; /* mov.l 0x8c6086d4,r9 */
L8c6085e4: *stop=0x8c6085e4u;if(!valid_access(r[6]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6085e6; /* mov.l @(0x14,r6),r6 */
L8c6085e6: target=r[9];*pr=0x8c6085eau;*stop=0x8c6085e8u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6085ea; /* jsr @r9 */
L8c6085ea: *stop=0x8c6085ecu;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c6085ee: *stop=0x8c6085eeu;r[4]=r[15];goto L8c6085f0; /* mov r15,r4 */
L8c6085f0: target=0x8c6080c8u;*pr=0x8c6085f4u;*stop=0x8c6085f2u;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6085f4; /* bsr 0x8c6080c8 */
L8c6085f4: *stop=0x8c6085f4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6085f6; /* mov.l @r15,r0 */
L8c6085f6: *stop=0x8c6085f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6085f8; /* mov.l 0x8c6086c0,r3 */
L8c6085f8: *stop=0x8c6085f8u;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6085fa; /* mov.l @(0x4,r0),r0 */
L8c6085fa: *stop=0x8c6085fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086d8u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c6085fc; /* mov.l 0x8c6086d8,r9 */
L8c6085fc: *stop=0x8c6085fcu;r[0]&=r[3];goto L8c6085fe; /* and r3,r0 */
L8c6085fe: *stop=0x8c6085feu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c608600; /* cmp/eq #0x0,r0 */
L8c608600: if((*sr&1u))goto L8c608612;goto L8c608602; /* bt 0x8c608612 */
L8c608602: *stop=0x8c608602u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086c4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c608604; /* mov.l 0x8c6086c4,r1 */
L8c608604: *stop=0x8c608604u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c608606; /* cmp/eq r1,r0 */
L8c608606: if((*sr&1u))goto L8c608660;goto L8c608608; /* bt 0x8c608660 */
L8c608608: *stop=0x8c608608u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086c8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c60860a; /* mov.l 0x8c6086c8,r1 */
L8c60860a: *stop=0x8c60860au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c60860c; /* cmp/eq r1,r0 */
L8c60860c: if((*sr&1u))goto L8c6086e8;goto L8c60860e; /* bt 0x8c6086e8 */
L8c60860e: *stop=0x8c608610u;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c608612: *stop=0x8c608612u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608614; /* mov.l 0x8c6086dc,r2 */
L8c608614: target=r[2];*pr=0x8c608618u;*stop=0x8c608616u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608618; /* jsr @r2 */
L8c608618: *stop=0x8c608618u;r[5]=r[15];goto L8c60861a; /* mov r15,r5 */
L8c60861a: *stop=0x8c60861au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60861c; /* mov.l 0x8c6086e0,r3 */
L8c60861c: *stop=0x8c60861cu;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[0],4);if(io_status)return io_status;goto L8c60861e; /* mov.l r0,@r9 */
L8c60861e: *stop=0x8c60861eu;r[5]+=36u;goto L8c608620; /* add #0x24,r5 */
L8c608620: *stop=0x8c608620u;r[6]=44u;goto L8c608622; /* mov #0x2c,r6 */
L8c608622: target=r[3];*pr=0x8c608626u;*stop=0x8c608624u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608626; /* jsr @r3 */
L8c608626: *stop=0x8c608626u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608628; /* mov.l 0x8c6086e4,r3 */
L8c608628: *stop=0x8c608628u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c60862a; /* mov.l @r9,r5 */
L8c60862a: target=r[3];*pr=0x8c60862eu;*stop=0x8c60862cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60862e; /* jsr @r3 */
L8c60862e: *stop=0x8c60862eu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c608630; /* cmp/pz r0 */
L8c608630: take=!(*sr&1u);*stop=0x8c608632u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;if(take)goto L8c608682;goto L8c608634; /* bf/s 0x8c608682 */
L8c608634: *stop=0x8c608634u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c608636; /* mov.l r8,@-r15 */
L8c608636: *stop=0x8c608636u;r[7]=2u;goto L8c608638; /* mov #0x2,r7 */
L8c608638: *stop=0x8c608638u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c60863a; /* mov.l @r9,r5 */
L8c60863a: *stop=0x8c60863au;r[6]=r[11];goto L8c60863c; /* mov r11,r6 */
L8c60863c: *stop=0x8c60863cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c60863e; /* mov.l @(0x4,r15),r4 */
L8c60863e: *stop=0x8c60863eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608640; /* mov.l 0x8c6086bc,r3 */
L8c608640: target=r[3];*pr=0x8c608644u;*stop=0x8c608642u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608644; /* jsr @r3 */
L8c608644: *stop=0x8c608644u;r[15]+=4u;goto L8c608646; /* add #0x4,r15 */
L8c608646: *stop=0x8c608646u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c608648; /* mov.l @r14,r2 */
L8c608648: *stop=0x8c608648u;r[0]=66u;goto L8c60864a; /* mov #0x42,r0 */
L8c60864a: *stop=0x8c60864au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60864c; /* mov.l 0x8c6086cc,r3 */
L8c60864c: *stop=0x8c60864cu;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[13],2);if(io_status)return io_status;goto L8c60864e; /* mov.w r13,@(r0,r2) */
L8c60864e: target=r[3];*pr=0x8c608652u;*stop=0x8c608650u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608652; /* jsr @r3 */
L8c608652: *stop=0x8c608652u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608654; /* mov.l @(0x8,r15),r2 */
L8c608654: *stop=0x8c608654u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c608656; /* tst r2,r2 */
L8c608656: if(!(*sr&1u))goto L8c60865c;goto L8c608658; /* bf 0x8c60865c */
L8c608658: *stop=0x8c60865au;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c60865c: *stop=0x8c60865eu;goto L8c6087a0; /* bra 0x8c6087a0 */
L8c608660: *stop=0x8c608660u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086dcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c608662; /* mov.l 0x8c6086dc,r1 */
L8c608662: target=r[1];*pr=0x8c608666u;*stop=0x8c608664u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608666; /* jsr @r1 */
L8c608666: *stop=0x8c608666u;r[5]=r[15];goto L8c608668; /* mov r15,r5 */
L8c608668: *stop=0x8c608668u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60866a; /* mov.l 0x8c6086e0,r3 */
L8c60866a: *stop=0x8c60866au;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[0],4);if(io_status)return io_status;goto L8c60866c; /* mov.l r0,@r9 */
L8c60866c: *stop=0x8c60866cu;r[5]+=36u;goto L8c60866e; /* add #0x24,r5 */
L8c60866e: *stop=0x8c60866eu;r[6]=44u;goto L8c608670; /* mov #0x2c,r6 */
L8c608670: target=r[3];*pr=0x8c608674u;*stop=0x8c608672u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608674; /* jsr @r3 */
L8c608674: *stop=0x8c608674u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608676; /* mov.l 0x8c6086d0,r2 */
L8c608676: *stop=0x8c608676u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c608678; /* mov.l @r9,r5 */
L8c608678: target=r[2];*pr=0x8c60867cu;*stop=0x8c60867au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60867c; /* jsr @r2 */
L8c60867c: *stop=0x8c60867cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c60867e; /* tst r0,r0 */
L8c60867e: take=!(*sr&1u);*stop=0x8c608680u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;if(take)goto L8c6086a6;goto L8c608682; /* bf/s 0x8c6086a6 */
L8c608682: *stop=0x8c608682u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608684; /* mov.l 0x8c6086bc,r2 */
L8c608684: *stop=0x8c608684u;r[6]=r[11];goto L8c608686; /* mov r11,r6 */
L8c608686: *stop=0x8c608686u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c608688; /* mov.l r8,@-r15 */
L8c608688: *stop=0x8c608688u;r[7]=0u;goto L8c60868a; /* mov #0x0,r7 */
L8c60868a: *stop=0x8c60868au;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c60868c; /* mov.l @r9,r5 */
L8c60868c: target=r[2];*pr=0x8c608690u;*stop=0x8c60868eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608690; /* jsr @r2 */
L8c608690: *stop=0x8c608690u;r[15]+=4u;goto L8c608692; /* add #0x4,r15 */
L8c608692: *stop=0x8c608692u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608694; /* mov.l @(0x4,r15),r2 */
L8c608694: *stop=0x8c608694u;r[0]=66u;goto L8c608696; /* mov #0x42,r0 */
L8c608696: *stop=0x8c608696u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608698; /* mov.l @r14,r3 */
L8c608698: *stop=0x8c608698u;r[2]=0u-r[2];goto L8c60869a; /* neg r2,r2 */
L8c60869a: *stop=0x8c60869au;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[2],2);if(io_status)return io_status;goto L8c60869c; /* mov.w r2,@(r0,r3) */
L8c60869c: *stop=0x8c60869cu;r[0]=64u;goto L8c60869e; /* mov #0x40,r0 */
L8c60869e: *stop=0x8c60869eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6086a0; /* mov.l @r14,r3 */
L8c6086a0: *stop=0x8c6086a0u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[13],2);if(io_status)return io_status;goto L8c6086a2; /* mov.w r13,@(r0,r3) */
L8c6086a2: *stop=0x8c6086a4u;goto L8c608770; /* bra 0x8c608770 */
L8c6086a6: *stop=0x8c6086a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6086bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6086a8; /* mov.l 0x8c6086bc,r3 */
L8c6086a8: *stop=0x8c6086a8u;r[6]=r[11];goto L8c6086aa; /* mov r11,r6 */
L8c6086aa: *stop=0x8c6086aau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6086ac; /* mov.l r8,@-r15 */
L8c6086ac: *stop=0x8c6086acu;r[7]=1u;goto L8c6086ae; /* mov #0x1,r7 */
L8c6086ae: *stop=0x8c6086aeu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6086b0; /* mov.l @r9,r5 */
L8c6086b0: target=r[3];*pr=0x8c6086b4u;*stop=0x8c6086b2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6086b4; /* jsr @r3 */
L8c6086b4: *stop=0x8c6086b6u;r[15]+=4u;goto L8c6087a8; /* bra 0x8c6087a8 */
L8c6086e8: *stop=0x8c6086e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087d0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6086ea; /* mov.l 0x8c6087d0,r1 */
L8c6086ea: target=r[1];*pr=0x8c6086eeu;*stop=0x8c6086ecu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6086ee; /* jsr @r1 */
L8c6086ee: *stop=0x8c6086eeu;r[5]=r[15];goto L8c6086f0; /* mov r15,r5 */
L8c6086f0: *stop=0x8c6086f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6086f2; /* mov.l 0x8c6087d4,r3 */
L8c6086f2: *stop=0x8c6086f2u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[0],4);if(io_status)return io_status;goto L8c6086f4; /* mov.l r0,@r9 */
L8c6086f4: *stop=0x8c6086f4u;r[5]+=36u;goto L8c6086f6; /* add #0x24,r5 */
L8c6086f6: *stop=0x8c6086f6u;r[6]=44u;goto L8c6086f8; /* mov #0x2c,r6 */
L8c6086f8: target=r[3];*pr=0x8c6086fcu;*stop=0x8c6086fau;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6086fc; /* jsr @r3 */
L8c6086fc: *stop=0x8c6086fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6086fe; /* mov.l 0x8c6087d8,r3 */
L8c6086fe: *stop=0x8c6086feu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c608700; /* mov.l @r9,r5 */
L8c608700: target=r[3];*pr=0x8c608704u;*stop=0x8c608702u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608704; /* jsr @r3 */
L8c608704: *stop=0x8c608704u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c608706; /* cmp/pz r0 */
L8c608706: take=(*sr&1u);*stop=0x8c608708u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;if(take)goto L8c60872e;goto L8c60870a; /* bt/s 0x8c60872e */
L8c60870a: *stop=0x8c60870au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60870c; /* mov.l 0x8c6087dc,r2 */
L8c60870c: *stop=0x8c60870cu;r[6]=r[11];goto L8c60870e; /* mov r11,r6 */
L8c60870e: *stop=0x8c60870eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c608710; /* mov.l r8,@-r15 */
L8c608710: *stop=0x8c608710u;r[7]=0u;goto L8c608712; /* mov #0x0,r7 */
L8c608712: *stop=0x8c608712u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c608714; /* mov.l @r9,r5 */
L8c608714: target=r[2];*pr=0x8c608718u;*stop=0x8c608716u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608718; /* jsr @r2 */
L8c608718: *stop=0x8c608718u;r[15]+=4u;goto L8c60871a; /* add #0x4,r15 */
L8c60871a: *stop=0x8c60871au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60871c; /* mov.l @(0x4,r15),r2 */
L8c60871c: *stop=0x8c60871cu;r[0]=66u;goto L8c60871e; /* mov #0x42,r0 */
L8c60871e: *stop=0x8c60871eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608720; /* mov.l @r14,r3 */
L8c608720: *stop=0x8c608720u;r[2]=0u-r[2];goto L8c608722; /* neg r2,r2 */
L8c608722: *stop=0x8c608722u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[2],2);if(io_status)return io_status;goto L8c608724; /* mov.w r2,@(r0,r3) */
L8c608724: *stop=0x8c608724u;r[0]=64u;goto L8c608726; /* mov #0x40,r0 */
L8c608726: *stop=0x8c608726u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608728; /* mov.l @r14,r3 */
L8c608728: *stop=0x8c608728u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[13],2);if(io_status)return io_status;goto L8c60872a; /* mov.w r13,@(r0,r3) */
L8c60872a: *stop=0x8c60872cu;goto L8c60877a; /* bra 0x8c60877a */
L8c60872e: *stop=0x8c60872eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608730; /* mov.l 0x8c6087e0,r3 */
L8c608730: *stop=0x8c608730u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c608732; /* mov.l @r9,r5 */
L8c608732: target=r[3];*pr=0x8c608736u;*stop=0x8c608734u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608736; /* jsr @r3 */
L8c608736: *stop=0x8c608736u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c608738; /* tst r0,r0 */
L8c608738: take=!(*sr&1u);*stop=0x8c60873au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;if(take)goto L8c60877e;goto L8c60873c; /* bf/s 0x8c60877e */
L8c60873c: *stop=0x8c60873cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60873e; /* mov.l 0x8c6087dc,r2 */
L8c60873e: *stop=0x8c60873eu;r[6]=r[11];goto L8c608740; /* mov r11,r6 */
L8c608740: *stop=0x8c608740u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c608742; /* mov.l r8,@-r15 */
L8c608742: *stop=0x8c608742u;r[7]=2u;goto L8c608744; /* mov #0x2,r7 */
L8c608744: *stop=0x8c608744u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c608746; /* mov.l @r9,r5 */
L8c608746: target=r[2];*pr=0x8c60874au;*stop=0x8c608748u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60874a; /* jsr @r2 */
L8c60874a: *stop=0x8c60874au;r[15]+=4u;goto L8c60874c; /* add #0x4,r15 */
L8c60874c: *stop=0x8c60874cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60874e; /* mov.l @r15,r2 */
L8c60874e: *stop=0x8c60874eu;r[0]=66u;goto L8c608750; /* mov #0x42,r0 */
L8c608750: *stop=0x8c608750u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608752; /* mov.l @r14,r3 */
L8c608752: *stop=0x8c608752u;r[2]=0u-r[2];goto L8c608754; /* neg r2,r2 */
L8c608754: *stop=0x8c608754u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[2],2);if(io_status)return io_status;goto L8c608756; /* mov.w r2,@(r0,r3) */
L8c608756: *stop=0x8c608756u;r[0]=64u;goto L8c608758; /* mov #0x40,r0 */
L8c608758: *stop=0x8c608758u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60875a; /* mov.l @r14,r3 */
L8c60875a: *stop=0x8c60875au;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[13],2);if(io_status)return io_status;goto L8c60875c; /* mov.w r13,@(r0,r3) */
L8c60875c: *stop=0x8c60875cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60875e; /* mov.l 0x8c6087e4,r3 */
L8c60875e: target=r[3];*pr=0x8c608762u;*stop=0x8c608760u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608762; /* jsr @r3 */
L8c608762: *stop=0x8c608762u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608764; /* mov.l @(0x8,r15),r2 */
L8c608764: *stop=0x8c608764u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c608766; /* tst r2,r2 */
L8c608766: if((*sr&1u))goto L8c608770;goto L8c608768; /* bt 0x8c608770 */
L8c608768: *stop=0x8c608768u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60876a; /* mov.l 0x8c6087e8,r3 */
L8c60876a: *stop=0x8c60876au;r[5]=r[8];goto L8c60876c; /* mov r8,r5 */
L8c60876c: target=r[3];*pr=0x8c608770u;*stop=0x8c60876eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608770; /* jsr @r3 */
L8c608770: *stop=0x8c608770u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087f0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c608772; /* mov.l 0x8c6087f0,r1 */
L8c608772: *stop=0x8c608772u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608774; /* mov.l 0x8c6087ec,r3 */
L8c608774: *stop=0x8c608774u;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608776; /* mov.l @(0x1c,r15),r2 */
L8c608776: *stop=0x8c608776u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c608778; /* mov.l r2,@r3 */
L8c608778: *stop=0x8c608778u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[11],4);if(io_status)return io_status;goto L8c60877a; /* mov.l r11,@r1 */
L8c60877a: *stop=0x8c60877cu;r[0]=4294967295u;goto L8c6087bc; /* bra 0x8c6087bc */
L8c60877e: *stop=0x8c60877eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608780; /* mov.l 0x8c6087dc,r3 */
L8c608780: *stop=0x8c608780u;r[6]=r[11];goto L8c608782; /* mov r11,r6 */
L8c608782: *stop=0x8c608782u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c608784; /* mov.l r8,@-r15 */
L8c608784: *stop=0x8c608784u;r[7]=3u;goto L8c608786; /* mov #0x3,r7 */
L8c608786: *stop=0x8c608786u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c608788; /* mov.l @r9,r5 */
L8c608788: target=r[3];*pr=0x8c60878cu;*stop=0x8c60878au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60878c; /* jsr @r3 */
L8c60878c: *stop=0x8c60878cu;r[15]+=4u;goto L8c60878e; /* add #0x4,r15 */
L8c60878e: *stop=0x8c60878eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608790; /* mov.l 0x8c6087e4,r2 */
L8c608790: target=r[2];*pr=0x8c608794u;*stop=0x8c608792u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608794; /* jsr @r2 */
L8c608794: *stop=0x8c608794u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608796; /* mov.l @r14,r3 */
L8c608796: *stop=0x8c608796u;r[0]=66u;goto L8c608798; /* mov #0x42,r0 */
L8c608798: *stop=0x8c608798u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[13],2);if(io_status)return io_status;goto L8c60879a; /* mov.w r13,@(r0,r3) */
L8c60879a: *stop=0x8c60879au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60879c; /* mov.l @(0x8,r15),r3 */
L8c60879c: *stop=0x8c60879cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c60879e; /* tst r3,r3 */
L8c60879e: if((*sr&1u))goto L8c6087a8;goto L8c6087a0; /* bt 0x8c6087a8 */
L8c6087a0: *stop=0x8c6087a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6087e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6087a2; /* mov.l 0x8c6087e8,r3 */
L8c6087a2: *stop=0x8c6087a2u;r[5]=r[8];goto L8c6087a4; /* mov r8,r5 */
L8c6087a4: target=r[3];*pr=0x8c6087a8u;*stop=0x8c6087a6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6087a8; /* jsr @r3 */
L8c6087a8: *stop=0x8c6087a8u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6087aa; /* mov.l @(0x10,r15),r2 */
L8c6087aa: *stop=0x8c6087aau;r[2]+=1u;goto L8c6087ac; /* add #0x1,r2 */
L8c6087ac: *stop=0x8c6087acu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6087ae; /* mov.l r2,@(0x10,r15) */
L8c6087ae: *stop=0x8c6087aeu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6087b0; /* mov.l @(0x10,r15),r3 */
L8c6087b0: *stop=0x8c6087b0u;if(!valid_access(r[15]+24u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x18u,2);if(io_status)return io_status;goto L8c6087b2; /* mov.w @(0x18,r15),r0 */
L8c6087b2: *stop=0x8c6087b2u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c6087b4; /* cmp/ge r0,r3 */
L8c6087b4: if((*sr&1u))goto L8c6087ba;goto L8c6087b6; /* bt 0x8c6087ba */
L8c6087b6: *stop=0x8c6087b8u;goto L8c6081f0; /* bra 0x8c6081f0 */
L8c6087ba: *stop=0x8c6087bau;r[0]=1u;goto L8c6087bc; /* mov #0x1,r0 */
L8c6087bc: *stop=0x8c6087bcu;r[15]+=88u;goto L8c6087be; /* add #0x58,r15 */
L8c6087be: *stop=0x8c6087beu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6087c0; /* lds.l @r15+,PR */
L8c6087c0: *stop=0x8c6087c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6087c2; /* mov.l @r15+,r8 */
L8c6087c2: *stop=0x8c6087c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6087c4; /* mov.l @r15+,r9 */
L8c6087c4: *stop=0x8c6087c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6087c6; /* mov.l @r15+,r10 */
L8c6087c6: *stop=0x8c6087c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6087c8; /* mov.l @r15+,r11 */
L8c6087c8: *stop=0x8c6087c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6087ca; /* mov.l @r15+,r12 */
L8c6087ca: *stop=0x8c6087cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6087cc; /* mov.l @r15+,r13 */
L8c6087cc: *stop=0x8c6087ccu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6087ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c608134(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608156(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608156u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60815c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60815cu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608166(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608166u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608178(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608178u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608190(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608190u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60819a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60819au);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6081a2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6081a2u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6081ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6081aeu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6081b2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6081b2u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6081f6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6081f6u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60822a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60822au);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60825a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60825au);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608272(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608272u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6082b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6082b8u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6082f6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6082f6u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60831a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60831au);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608330(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608330u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608338(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608338u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60835a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60835au);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608374(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608374u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60837c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60837cu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6083dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6083dcu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6083f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6083f0u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60842e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60842eu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608480(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608480u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6084a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6084a4u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6084c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6084c6u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6084dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6084dcu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60852a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60852au);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60854e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60854eu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608580(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608580u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6085a2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6085a2u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6085d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6085d4u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6085ea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6085eau);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6085f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6085f4u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608618(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608618u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608626(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608626u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60862e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60862eu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608644(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608644u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608652(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608652u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608666(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608666u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608674(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608674u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60867c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60867cu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608690(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608690u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6086b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6086b4u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6086ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6086eeu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6086fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6086fcu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608704(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608704u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608718(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608718u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608736(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608736u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60874a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60874au);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608762(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608762u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608770(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608770u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60878c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60878cu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608794(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608794u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6087a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c608134(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6087a8u);}

SA_STAGE_EXPORT int sa_player_texture_load_8c606f14(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c606f14;
L8c606f14: *stop=0x8c606f14u;{uint32_t loaded=rd(m,bus,&io_status,0x8c607054u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c606f16; /* mov.l 0x8c607054,r5 */
L8c606f16: *stop=0x8c606f16u;r[3]=r[4];goto L8c606f18; /* mov r4,r3 */
L8c606f18: *stop=0x8c606f18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c607058u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c606f1a; /* mov.l 0x8c607058,r2 */
L8c606f1a: *stop=0x8c606f1au;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c606f1c; /* mov.l r4,@r5 */
L8c606f1c: *stop=0x8c606f1cu;if(*pr!=entry_pr)return -3;*stop=0x8c606f1eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;return 0; /* rts */
}

static int sa_player_texture_load_run_8c606f20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c606f32u)goto L8c606f32;goto L8c606f20;
L8c606f20: *stop=0x8c606f20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c606f22; /* mov.l r14,@-r15 */
L8c606f22: *stop=0x8c606f22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c606f24; /* sts.l PR,@-r15 */
L8c606f24: *stop=0x8c606f24u;r[15]+=4294967292u;goto L8c606f26; /* add #-0x4,r15 */
L8c606f26: *stop=0x8c606f26u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60705cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c606f28; /* mov.l 0x8c60705c,r3 */
L8c606f28: *stop=0x8c606f28u;r[6]=r[5];goto L8c606f2a; /* mov r5,r6 */
L8c606f2a: *stop=0x8c606f2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c607058u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c606f2c; /* mov.l 0x8c607058,r14 */
L8c606f2c: *stop=0x8c606f2cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c606f2e; /* mov.l r5,@r15 */
L8c606f2e: target=r[3];*pr=0x8c606f32u;*stop=0x8c606f30u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c606f32; /* jsr @r3 */
L8c606f32: *stop=0x8c606f32u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c606f34; /* mov.l @r14,r2 */
L8c606f34: *stop=0x8c606f34u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c606f36; /* mov.l @r15,r3 */
L8c606f36: *stop=0x8c606f36u;r[2]+=r[3];goto L8c606f38; /* add r3,r2 */
L8c606f38: *stop=0x8c606f38u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c606f3a; /* mov.l r2,@r14 */
L8c606f3a: *stop=0x8c606f3au;r[15]+=4u;goto L8c606f3c; /* add #0x4,r15 */
L8c606f3c: *stop=0x8c606f3cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c606f3e; /* lds.l @r15+,PR */
L8c606f3e: *stop=0x8c606f3eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c606f40u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c606f20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c606f20(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c606f32(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c606f20(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c606f32u);}

SA_STAGE_EXPORT int sa_player_texture_load_8c606f42(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c606f42;
L8c606f42: *stop=0x8c606f42u;r[0]=r[5];goto L8c606f44; /* mov r5,r0 */
L8c606f44: *stop=0x8c606f44u;{uint32_t loaded=rd(m,bus,&io_status,0x8c607058u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c606f46; /* mov.l 0x8c607058,r6 */
L8c606f46: *stop=0x8c606f46u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c606f48; /* cmp/eq #0x1,r0 */
L8c606f48: if((*sr&1u))goto L8c606f52;goto L8c606f4a; /* bt 0x8c606f52 */
L8c606f4a: *stop=0x8c606f4au;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c606f4c; /* cmp/eq #0x0,r0 */
L8c606f4c: if((*sr&1u))goto L8c606f5a;goto L8c606f4e; /* bt 0x8c606f5a */
L8c606f4e: *stop=0x8c606f50u;goto L8c606f62; /* bra 0x8c606f62 */
L8c606f52: *stop=0x8c606f52u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c606f54; /* mov.l @r6,r3 */
L8c606f54: *stop=0x8c606f54u;r[3]+=r[4];goto L8c606f56; /* add r4,r3 */
L8c606f56: *stop=0x8c606f58u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c606f62; /* bra 0x8c606f62 */
L8c606f5a: *stop=0x8c606f5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c607054u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c606f5c; /* mov.l 0x8c607054,r2 */
L8c606f5c: *stop=0x8c606f5cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c606f5e; /* mov.l @r2,r1 */
L8c606f5e: *stop=0x8c606f5eu;r[4]+=r[1];goto L8c606f60; /* add r1,r4 */
L8c606f60: *stop=0x8c606f60u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c606f62; /* mov.l r4,@r6 */
L8c606f62: *stop=0x8c606f62u;if(*pr!=entry_pr)return -3;*stop=0x8c606f64u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c606fe4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c606fe4;
L8c606fe4: *stop=0x8c606fe4u;r[3]=1u;goto L8c606fe6; /* mov #0x1,r3 */
L8c606fe6: *stop=0x8c606fe6u;r[6]=r[4]&65535u;goto L8c606fe8; /* extu.w r4,r6 */
L8c606fe8: *stop=0x8c606fe8u;*sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c606fea; /* tst r6,r3 */
L8c606fea: *stop=0x8c606feau;r[7]=2u;goto L8c606fec; /* mov #0x2,r7 */
L8c606fec: take=(*sr&1u);*stop=0x8c606feeu;r[5]=r[7];if(take)goto L8c606ff2;goto L8c606ff0; /* bt/s 0x8c606ff2 */
L8c606ff0: *stop=0x8c606ff0u;r[5]=30u;goto L8c606ff2; /* mov #0x1e,r5 */
L8c606ff2: *stop=0x8c606ff2u;*sr=(*sr&~1u)|!(r[6]&r[7]);goto L8c606ff4; /* tst r6,r7 */
L8c606ff4: take=(*sr&1u);*stop=0x8c606ff6u;r[2]=4u;if(take)goto L8c606ffa;goto L8c606ff8; /* bt/s 0x8c606ffa */
L8c606ff8: *stop=0x8c606ff8u;r[5]+=2u;goto L8c606ffa; /* add #0x2,r5 */
L8c606ffa: *stop=0x8c606ffau;*sr=(*sr&~1u)|!(r[6]&r[2]);goto L8c606ffc; /* tst r6,r2 */
L8c606ffc: take=(*sr&1u);*stop=0x8c606ffeu;r[2]=8u;if(take)goto L8c607002;goto L8c607000; /* bt/s 0x8c607002 */
L8c607000: *stop=0x8c607000u;r[5]+=2u;goto L8c607002; /* add #0x2,r5 */
L8c607002: *stop=0x8c607002u;*sr=(*sr&~1u)|!(r[2]&r[6]);goto L8c607004; /* tst r2,r6 */
L8c607004: if((*sr&1u))goto L8c607008;goto L8c607006; /* bt 0x8c607008 */
L8c607006: *stop=0x8c607006u;r[5]+=4u;goto L8c607008; /* add #0x4,r5 */
L8c607008: *stop=0x8c607008u;if(*pr!=entry_pr)return -3;*stop=0x8c60700au;r[0]=r[5];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c607244(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c607244;
L8c607244: *stop=0x8c607244u;r[6]=0u;goto L8c607246; /* mov #0x0,r6 */
L8c607246: *stop=0x8c607246u;r[2]=r[6];goto L8c607248; /* mov r6,r2 */
L8c607248: *stop=0x8c607248u;r[3]=r[2];goto L8c60724a; /* mov r2,r3 */
L8c60724a: *stop=0x8c60724au;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c60724c; /* shll r2 */
L8c60724c: *stop=0x8c60724cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c60724e; /* mov.l @r4,r1 */
L8c60724e: *stop=0x8c60724eu;r[2]+=r[3];goto L8c607250; /* add r3,r2 */
L8c607250: *stop=0x8c607250u;r[2]<<=2;goto L8c607252; /* shll2 r2 */
L8c607252: *stop=0x8c607252u;r[2]+=r[1];goto L8c607254; /* add r1,r2 */
L8c607254: *stop=0x8c607254u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c607256; /* mov.l @(0x4,r2),r3 */
L8c607256: *stop=0x8c607256u;r[3]=r[3]&65535u;goto L8c607258; /* extu.w r3,r3 */
L8c607258: *stop=0x8c607258u;*sr=(*sr&~1u)|(r[5]==r[3]);goto L8c60725a; /* cmp/eq r5,r3 */
L8c60725a: take=!(*sr&1u);*stop=0x8c60725cu;r[6]+=1u;if(take)goto L8c607246;goto L8c60725e; /* bf/s 0x8c607246 */
L8c60725e: *stop=0x8c60725eu;r[0]=r[6];goto L8c607260; /* mov r6,r0 */
L8c607260: *stop=0x8c607260u;if(*pr!=entry_pr)return -3;*stop=0x8c607262u;r[0]+=4294967295u;return 0; /* rts */
}

static int sa_player_texture_load_run_8c607ebc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c607efcu)goto L8c607efc;if(resume_pc==0x8c607f04u)goto L8c607f04;if(resume_pc==0x8c607f24u)goto L8c607f24;if(resume_pc==0x8c607f4cu)goto L8c607f4c;if(resume_pc==0x8c607f62u)goto L8c607f62;goto L8c607ebc;
L8c607ebc: *stop=0x8c607ebcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c607ebe; /* mov.l r14,@-r15 */
L8c607ebe: *stop=0x8c607ebeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c607ec0; /* mov.l r13,@-r15 */
L8c607ec0: *stop=0x8c607ec0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c607ec2; /* mov.l r12,@-r15 */
L8c607ec2: *stop=0x8c607ec2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c607ec4; /* mov.l r11,@-r15 */
L8c607ec4: *stop=0x8c607ec4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c607ec6; /* mov.l r10,@-r15 */
L8c607ec6: *stop=0x8c607ec6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c607ec8; /* mov.l r9,@-r15 */
L8c607ec8: *stop=0x8c607ec8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c607eca; /* mov.l r8,@-r15 */
L8c607eca: *stop=0x8c607ecau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c607ecc; /* sts.l PR,@-r15 */
L8c607ecc: *stop=0x8c607eccu;r[15]+=4294967236u;goto L8c607ece; /* add #-0x3c,r15 */
L8c607ece: *stop=0x8c607eceu;r[12]=r[15];goto L8c607ed0; /* mov r15,r12 */
L8c607ed0: *stop=0x8c607ed0u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c607ed2; /* mov.l r5,@(0xc,r15) */
L8c607ed2: *stop=0x8c607ed2u;if(!valid_access(r[6]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x4u,2);if(io_status)return io_status;goto L8c607ed4; /* mov.w @(0x4,r6),r0 */
L8c607ed4: *stop=0x8c607ed4u;r[12]+=20u;goto L8c607ed6; /* add #0x14,r12 */
L8c607ed6: *stop=0x8c607ed6u;r[3]=r[12];goto L8c607ed8; /* mov r12,r3 */
L8c607ed8: *stop=0x8c607ed8u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[9]=loaded;}goto L8c607eda; /* mov.l @r4,r9 */
L8c607eda: *stop=0x8c607edau;r[7]=r[0];goto L8c607edc; /* mov r0,r7 */
L8c607edc: *stop=0x8c607edcu;if(!valid_access(r[6]+6u,2u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x6u,2);if(io_status)return io_status;goto L8c607ede; /* mov.w @(0x6,r6),r0 */
L8c607ede: *stop=0x8c607edeu;r[3]+=2u;goto L8c607ee0; /* add #0x2,r3 */
L8c607ee0: *stop=0x8c607ee0u;r[5]=r[0];goto L8c607ee2; /* mov r0,r5 */
L8c607ee2: *stop=0x8c607ee2u;r[5]=(uint32_t)(int32_t)(int16_t)r[5];goto L8c607ee4; /* exts.w r5,r5 */
L8c607ee4: *stop=0x8c607ee4u;r[2]=r[5];goto L8c607ee6; /* mov r5,r2 */
L8c607ee6: *stop=0x8c607ee6u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c607ee8; /* cmp/pl r2 */
L8c607ee8: *stop=0x8c607ee8u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c607eea; /* mov.l r3,@(0x10,r15) */
L8c607eea: *stop=0x8c607eeau;r[8]=0u;goto L8c607eec; /* mov #0x0,r8 */
L8c607eec: *stop=0x8c607eecu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c607eee; /* mov.l r5,@(0x4,r15) */
L8c607eee: *stop=0x8c607eeeu;r[11]=2u;goto L8c607ef0; /* mov #0x2,r11 */
L8c607ef0: *stop=0x8c607ef0u;r[10]=8u;goto L8c607ef2; /* mov #0x8,r10 */
L8c607ef2: take=!(*sr&1u);*stop=0x8c607ef4u;r[14]=r[7]&65535u;if(take)goto L8c607f6c;goto L8c607ef6; /* bf/s 0x8c607f6c */
L8c607ef6: *stop=0x8c607ef6u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c607ef8; /* mov.l @(0xc,r15),r5 */
L8c607ef8: target=0x8c606f20u;*pr=0x8c607efcu;*stop=0x8c607efau;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c607efc; /* bsr 0x8c606f20 */
L8c607efc: *stop=0x8c607efcu;r[5]=r[12];goto L8c607efe; /* mov r12,r5 */
L8c607efe: *stop=0x8c607efeu;r[13]=r[9];goto L8c607f00; /* mov r9,r13 */
L8c607f00: target=0x8c606f66u;*pr=0x8c607f04u;*stop=0x8c607f02u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c607f04; /* bsr 0x8c606f66 */
L8c607f04: *stop=0x8c607f04u;if(!valid_access(r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c607f06; /* mov.w @r15,r1 */
L8c607f06: *stop=0x8c607f06u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c607f08; /* mov.l @(0x4,r13),r2 */
L8c607f08: *stop=0x8c607f08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c607f84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c607f0a; /* mov.l 0x8c607f84,r3 */
L8c607f0a: *stop=0x8c607f0au;r[1]=r[1]&65535u;goto L8c607f0c; /* extu.w r1,r1 */
L8c607f0c: *stop=0x8c607f0cu;r[2]&=r[3];goto L8c607f0e; /* and r3,r2 */
L8c607f0e: *stop=0x8c607f0eu;r[2]|=r[1];goto L8c607f10; /* or r1,r2 */
L8c607f10: *stop=0x8c607f10u;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[2],4);if(io_status)return io_status;goto L8c607f12; /* mov.l r2,@(0x4,r13) */
L8c607f12: *stop=0x8c607f12u;r[2]=1u;goto L8c607f14; /* mov #0x1,r2 */
L8c607f14: *stop=0x8c607f14u;*sr=(*sr&~1u)|!(r[14]&r[2]);goto L8c607f16; /* tst r14,r2 */
L8c607f16: take=(*sr&1u);*stop=0x8c607f18u;r[4]=r[11];if(take)goto L8c607f26;goto L8c607f1a; /* bt/s 0x8c607f26 */
L8c607f1a: *stop=0x8c607f1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c607f88u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c607f1c; /* mov.l 0x8c607f88,r2 */
L8c607f1c: *stop=0x8c607f1cu;r[6]=28u;goto L8c607f1e; /* mov #0x1c,r6 */
L8c607f1e: *stop=0x8c607f1eu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c607f20; /* mov.l @(0x10,r15),r5 */
L8c607f20: target=r[2];*pr=0x8c607f24u;*stop=0x8c607f22u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c607f24; /* jsr @r2 */
L8c607f24: *stop=0x8c607f24u;r[4]=30u;goto L8c607f26; /* mov #0x1e,r4 */
L8c607f26: *stop=0x8c607f26u;r[3]=r[14];goto L8c607f28; /* mov r14,r3 */
L8c607f28: *stop=0x8c607f28u;*sr=(*sr&~1u)|!(r[11]&r[3]);goto L8c607f2a; /* tst r11,r3 */
L8c607f2a: take=(*sr&1u);*stop=0x8c607f2cu;r[3]=4u;if(take)goto L8c607f30;goto L8c607f2e; /* bt/s 0x8c607f30 */
L8c607f2e: *stop=0x8c607f2eu;r[4]+=2u;goto L8c607f30; /* add #0x2,r4 */
L8c607f30: *stop=0x8c607f30u;*sr=(*sr&~1u)|!(r[14]&r[3]);goto L8c607f32; /* tst r14,r3 */
L8c607f32: if((*sr&1u))goto L8c607f36;goto L8c607f34; /* bt 0x8c607f36 */
L8c607f34: *stop=0x8c607f34u;r[4]+=2u;goto L8c607f36; /* add #0x2,r4 */
L8c607f36: *stop=0x8c607f36u;r[3]=sx(m,bus,&io_status,0x8c607f80u,2);if(io_status)return io_status;goto L8c607f38; /* mov.w 0x8c607f80,r3 */
L8c607f38: *stop=0x8c607f38u;r[5]=r[4];goto L8c607f3a; /* mov r4,r5 */
L8c607f3a: *stop=0x8c607f3au;*sr=(*sr&~1u)|!(r[14]&r[3]);goto L8c607f3c; /* tst r14,r3 */
L8c607f3c: take=(*sr&1u);*stop=0x8c607f3eu;r[5]+=r[12];if(take)goto L8c607f56;goto L8c607f40; /* bt/s 0x8c607f56 */
L8c607f40: *stop=0x8c607f40u;r[1]=r[14];goto L8c607f42; /* mov r14,r1 */
L8c607f42: *stop=0x8c607f42u;*sr=(*sr&~1u)|!(r[10]&r[1]);goto L8c607f44; /* tst r10,r1 */
L8c607f44: if((*sr&1u))goto L8c607f62;goto L8c607f46; /* bt 0x8c607f62 */
L8c607f46: *stop=0x8c607f46u;r[4]=r[15];goto L8c607f48; /* mov r15,r4 */
L8c607f48: target=0x8c606f6cu;*pr=0x8c607f4cu;*stop=0x8c607f4au;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c607f4c; /* bsr 0x8c606f6c */
L8c607f4c: *stop=0x8c607f4cu;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c607f4e; /* mov.l @(0x4,r13),r2 */
L8c607f4e: *stop=0x8c607f4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c607f8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c607f50; /* mov.l 0x8c607f8c,r3 */
L8c607f50: *stop=0x8c607f50u;r[2]|=r[3];goto L8c607f52; /* or r3,r2 */
L8c607f52: *stop=0x8c607f54u;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[2],4);if(io_status)return io_status;goto L8c607f62; /* bra 0x8c607f62 */
L8c607f56: *stop=0x8c607f56u;r[0]=r[14];goto L8c607f58; /* mov r14,r0 */
L8c607f58: *stop=0x8c607f58u;*sr=(*sr&~1u)|!(r[10]&r[0]);goto L8c607f5a; /* tst r10,r0 */
L8c607f5a: if((*sr&1u))goto L8c607f62;goto L8c607f5c; /* bt 0x8c607f62 */
L8c607f5c: *stop=0x8c607f5cu;r[4]=r[13];goto L8c607f5e; /* mov r13,r4 */
L8c607f5e: target=0x8c606f6cu;*pr=0x8c607f62u;*stop=0x8c607f60u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c607f62; /* bsr 0x8c606f6c */
L8c607f62: *stop=0x8c607f62u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c607f64; /* mov.l @(0x4,r15),r3 */
L8c607f64: *stop=0x8c607f64u;r[8]+=1u;goto L8c607f66; /* add #0x1,r8 */
L8c607f66: *stop=0x8c607f66u;*sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[3]);goto L8c607f68; /* cmp/ge r3,r8 */
L8c607f68: take=!(*sr&1u);*stop=0x8c607f6au;r[9]+=12u;if(take)goto L8c607ef6;goto L8c607f6c; /* bf/s 0x8c607ef6 */
L8c607f6c: *stop=0x8c607f6cu;r[15]+=60u;goto L8c607f6e; /* add #0x3c,r15 */
L8c607f6e: *stop=0x8c607f6eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c607f70; /* lds.l @r15+,PR */
L8c607f70: *stop=0x8c607f70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c607f72; /* mov.l @r15+,r8 */
L8c607f72: *stop=0x8c607f72u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c607f74; /* mov.l @r15+,r9 */
L8c607f74: *stop=0x8c607f74u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c607f76; /* mov.l @r15+,r10 */
L8c607f76: *stop=0x8c607f76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c607f78; /* mov.l @r15+,r11 */
L8c607f78: *stop=0x8c607f78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c607f7a; /* mov.l @r15+,r12 */
L8c607f7a: *stop=0x8c607f7au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c607f7c; /* mov.l @r15+,r13 */
L8c607f7c: *stop=0x8c607f7cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c607f7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c607ebc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607ebc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c607efc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607ebc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c607efcu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c607f04(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607ebc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c607f04u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c607f24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607ebc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c607f24u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c607f4c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607ebc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c607f4cu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c607f62(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607ebc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c607f62u);}

static int sa_player_texture_load_run_8c607f90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c607fa4u)goto L8c607fa4;if(resume_pc==0x8c607fdcu)goto L8c607fdc;if(resume_pc==0x8c607feau)goto L8c607fea;if(resume_pc==0x8c607ff4u)goto L8c607ff4;if(resume_pc==0x8c608016u)goto L8c608016;if(resume_pc==0x8c608030u)goto L8c608030;if(resume_pc==0x8c60803au)goto L8c60803a;if(resume_pc==0x8c608052u)goto L8c608052;if(resume_pc==0x8c60805eu)goto L8c60805e;if(resume_pc==0x8c6080a8u)goto L8c6080a8;if(resume_pc==0x8c6080b0u)goto L8c6080b0;goto L8c607f90;
L8c607f90: *stop=0x8c607f90u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c607f92; /* mov.l r14,@-r15 */
L8c607f92: *stop=0x8c607f92u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c607f94; /* mov.l r13,@-r15 */
L8c607f94: *stop=0x8c607f94u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c607f96; /* sts.l PR,@-r15 */
L8c607f96: *stop=0x8c607f96u;r[0]=sx(m,bus,&io_status,0x8c608062u,2);if(io_status)return io_status;goto L8c607f98; /* mov.w 0x8c608062,r0 */
L8c607f98: *stop=0x8c607f98u;r[15]+=r[0];goto L8c607f9a; /* add r0,r15 */
L8c607f9a: *stop=0x8c607f9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c608070u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c607f9c; /* mov.l 0x8c608070,r3 */
L8c607f9c: *stop=0x8c607f9cu;r[4]=r[15];goto L8c607f9e; /* mov r15,r4 */
L8c607f9e: *stop=0x8c607f9eu;r[5]=4u;goto L8c607fa0; /* mov #0x4,r5 */
L8c607fa0: target=r[3];*pr=0x8c607fa4u;*stop=0x8c607fa2u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c607fa4; /* jsr @r3 */
L8c607fa4: *stop=0x8c607fa4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608074u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c607fa6; /* mov.l 0x8c608074,r1 */
L8c607fa6: *stop=0x8c607fa6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c607fa8; /* mov.l @(0x4,r15),r0 */
L8c607fa8: *stop=0x8c607fa8u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c607faa; /* cmp/eq r1,r0 */
L8c607faa: if((*sr&1u))goto L8c607fd4;goto L8c607fac; /* bt 0x8c607fd4 */
L8c607fac: *stop=0x8c607facu;{uint32_t loaded=rd(m,bus,&io_status,0x8c608078u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c607fae; /* mov.l 0x8c608078,r1 */
L8c607fae: *stop=0x8c607faeu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c607fb0; /* cmp/eq r1,r0 */
L8c607fb0: if((*sr&1u))goto L8c607fd4;goto L8c607fb2; /* bt 0x8c607fd4 */
L8c607fb2: *stop=0x8c607fb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60807cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c607fb4; /* mov.l 0x8c60807c,r1 */
L8c607fb4: *stop=0x8c607fb4u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c607fb6; /* cmp/eq r1,r0 */
L8c607fb6: if((*sr&1u))goto L8c607fd4;goto L8c607fb8; /* bt 0x8c607fd4 */
L8c607fb8: *stop=0x8c607fb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608080u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c607fba; /* mov.l 0x8c608080,r1 */
L8c607fba: *stop=0x8c607fbau;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c607fbc; /* cmp/eq r1,r0 */
L8c607fbc: if((*sr&1u))goto L8c607fd4;goto L8c607fbe; /* bt 0x8c607fd4 */
L8c607fbe: *stop=0x8c607fbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c608084u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c607fc0; /* mov.l 0x8c608084,r1 */
L8c607fc0: *stop=0x8c607fc0u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c607fc2; /* cmp/eq r1,r0 */
L8c607fc2: if((*sr&1u))goto L8c607fd4;goto L8c607fc4; /* bt 0x8c607fd4 */
L8c607fc4: *stop=0x8c607fc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608088u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c607fc6; /* mov.l 0x8c608088,r1 */
L8c607fc6: *stop=0x8c607fc6u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c607fc8; /* cmp/eq r1,r0 */
L8c607fc8: if((*sr&1u))goto L8c607fe0;goto L8c607fca; /* bt 0x8c607fe0 */
L8c607fca: *stop=0x8c607fcau;{uint32_t loaded=rd(m,bus,&io_status,0x8c60808cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c607fcc; /* mov.l 0x8c60808c,r1 */
L8c607fcc: *stop=0x8c607fccu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c607fce; /* cmp/eq r1,r0 */
L8c607fce: if((*sr&1u))goto L8c608056;goto L8c607fd0; /* bt 0x8c608056 */
L8c607fd0: *stop=0x8c607fd2u;goto L8c6080a0; /* bra 0x8c6080a0 */
L8c607fd4: *stop=0x8c607fd4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608070u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c607fd6; /* mov.l 0x8c608070,r3 */
L8c607fd6: *stop=0x8c607fd6u;r[5]=4u;goto L8c607fd8; /* mov #0x4,r5 */
L8c607fd8: target=r[3];*pr=0x8c607fdcu;*stop=0x8c607fdau;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c607fdc; /* jsr @r3 */
L8c607fdc: *stop=0x8c607fdeu;goto L8c6080a8; /* bra 0x8c6080a8 */
L8c607fe0: *stop=0x8c607fe0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608070u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c607fe2; /* mov.l 0x8c608070,r2 */
L8c607fe2: *stop=0x8c607fe2u;r[5]=12u;goto L8c607fe4; /* mov #0xc,r5 */
L8c607fe4: *stop=0x8c607fe4u;r[4]=sx(m,bus,&io_status,0x8c608064u,2);if(io_status)return io_status;goto L8c607fe6; /* mov.w 0x8c608064,r4 */
L8c607fe6: target=r[2];*pr=0x8c607feau;*stop=0x8c607fe8u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c607fea; /* jsr @r2 */
L8c607fea: *stop=0x8c607feau;r[0]=sx(m,bus,&io_status,0x8c608066u,2);if(io_status)return io_status;goto L8c607fec; /* mov.w 0x8c608066,r0 */
L8c607fec: *stop=0x8c607fecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c608090u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c607fee; /* mov.l 0x8c608090,r14 */
L8c607fee: *stop=0x8c607feeu;if(!valid_access(r[0]+r[15],2u))return -3;r[4]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c607ff0; /* mov.w @(r0,r15),r4 */
L8c607ff0: target=r[14];*pr=0x8c607ff4u;*stop=0x8c607ff2u;r[4]=r[4]&65535u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c607ff4; /* jsr @r14 */
L8c607ff4: *stop=0x8c607ff4u;r[0]=sx(m,bus,&io_status,0x8c608068u,2);if(io_status)return io_status;goto L8c607ff6; /* mov.w 0x8c608068,r0 */
L8c607ff6: *stop=0x8c607ff6u;if(!valid_access(r[0]+r[15],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c607ff8; /* mov.w @(r0,r15),r0 */
L8c607ff8: *stop=0x8c607ff8u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c607ffa; /* cmp/eq #0x6,r0 */
L8c607ffa: if(!(*sr&1u))goto L8c608000;goto L8c607ffc; /* bf 0x8c608000 */
L8c607ffc: *stop=0x8c607ffeu;r[4]=4u;goto L8c608002; /* bra 0x8c608002 */
L8c608000: *stop=0x8c608000u;r[4]=2u;goto L8c608002; /* mov #0x2,r4 */
L8c608002: *stop=0x8c608002u;r[0]=sx(m,bus,&io_status,0x8c60806au,2);if(io_status)return io_status;goto L8c608004; /* mov.w 0x8c60806a,r0 */
L8c608004: *stop=0x8c608004u;r[14]=r[4];goto L8c608006; /* mov r4,r14 */
L8c608006: *stop=0x8c608006u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608070u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608008; /* mov.l 0x8c608070,r3 */
L8c608008: *stop=0x8c608008u;r[13]=r[15];goto L8c60800a; /* mov r15,r13 */
L8c60800a: *stop=0x8c60800au;if(!valid_access(r[0]+r[15],2u))return -3;r[5]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c60800c; /* mov.w @(r0,r15),r5 */
L8c60800c: *stop=0x8c60800cu;r[13]+=8u;goto L8c60800e; /* add #0x8,r13 */
L8c60800e: *stop=0x8c60800eu;*macl=r[14]*r[5];goto L8c608010; /* mul.l r14,r5 */
L8c608010: *stop=0x8c608010u;r[5]=*macl;goto L8c608012; /* sts MACL,r5 */
L8c608012: target=r[3];*pr=0x8c608016u;*stop=0x8c608014u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608016; /* jsr @r3 */
L8c608016: *stop=0x8c608016u;r[0]=sx(m,bus,&io_status,0x8c60806au,2);if(io_status)return io_status;goto L8c608018; /* mov.w 0x8c60806a,r0 */
L8c608018: *stop=0x8c608018u;r[5]=1u;goto L8c60801a; /* mov #0x1,r5 */
L8c60801a: *stop=0x8c60801au;if(!valid_access(r[0]+r[15],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c60801c; /* mov.w @(r0,r15),r2 */
L8c60801c: *stop=0x8c60801cu;r[0]=sx(m,bus,&io_status,0x8c608064u,2);if(io_status)return io_status;goto L8c60801e; /* mov.w 0x8c608064,r0 */
L8c60801e: *stop=0x8c60801eu;*macl=r[14]*r[2];goto L8c608020; /* mul.l r14,r2 */
L8c608020: *stop=0x8c608020u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608022; /* mov.l @(r0,r15),r3 */
L8c608022: *stop=0x8c608022u;r[2]=*macl;goto L8c608024; /* sts MACL,r2 */
L8c608024: *stop=0x8c608024u;r[3]-=r[2];goto L8c608026; /* sub r2,r3 */
L8c608026: *stop=0x8c608026u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608094u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608028; /* mov.l 0x8c608094,r2 */
L8c608028: *stop=0x8c608028u;r[3]+=4294967288u;goto L8c60802a; /* add #-0x8,r3 */
L8c60802a: *stop=0x8c60802au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c60802c; /* mov.l r3,@r15 */
L8c60802c: target=r[2];*pr=0x8c608030u;*stop=0x8c60802eu;r[4]=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608030; /* jsr @r2 */
L8c608030: *stop=0x8c608030u;r[0]=sx(m,bus,&io_status,0x8c608066u,2);if(io_status)return io_status;goto L8c608032; /* mov.w 0x8c608066,r0 */
L8c608032: *stop=0x8c608032u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608098u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c608034; /* mov.l 0x8c608098,r14 */
L8c608034: *stop=0x8c608034u;if(!valid_access(r[0]+r[15],2u))return -3;r[4]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c608036; /* mov.w @(r0,r15),r4 */
L8c608036: target=r[14];*pr=0x8c60803au;*stop=0x8c608038u;r[4]=r[4]&65535u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60803a; /* jsr @r14 */
L8c60803a: *stop=0x8c60803au;r[0]=sx(m,bus,&io_status,0x8c60806au,2);if(io_status)return io_status;goto L8c60803c; /* mov.w 0x8c60806a,r0 */
L8c60803c: *stop=0x8c60803cu;r[6]=r[13];goto L8c60803e; /* mov r13,r6 */
L8c60803e: *stop=0x8c60803eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60809cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c608040; /* mov.l 0x8c60809c,r14 */
L8c608040: *stop=0x8c608040u;if(!valid_access(r[0]+r[15],2u))return -3;r[5]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c608042; /* mov.w @(r0,r15),r5 */
L8c608042: *stop=0x8c608042u;r[0]=sx(m,bus,&io_status,0x8c60806cu,2);if(io_status)return io_status;goto L8c608044; /* mov.w 0x8c60806c,r0 */
L8c608044: *stop=0x8c608044u;if(!valid_access(r[0]+r[15],2u))return -3;r[4]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c608046; /* mov.w @(r0,r15),r4 */
L8c608046: *stop=0x8c608046u;r[0]=sx(m,bus,&io_status,0x8c608068u,2);if(io_status)return io_status;goto L8c608048; /* mov.w 0x8c608068,r0 */
L8c608048: *stop=0x8c608048u;if(!valid_access(r[0]+r[15],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c60804a; /* mov.w @(r0,r15),r3 */
L8c60804a: *stop=0x8c60804au;r[3]<<=2;goto L8c60804c; /* shll2 r3 */
L8c60804c: *stop=0x8c60804cu;r[3]<<=2;goto L8c60804e; /* shll2 r3 */
L8c60804e: target=r[14];*pr=0x8c608052u;*stop=0x8c608050u;r[4]+=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608052; /* jsr @r14 */
L8c608052: *stop=0x8c608054u;goto L8c6080b0; /* bra 0x8c6080b0 */
L8c608056: *stop=0x8c608056u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608094u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608058; /* mov.l 0x8c608094,r2 */
L8c608058: *stop=0x8c608058u;r[5]=1u;goto L8c60805a; /* mov #0x1,r5 */
L8c60805a: target=r[2];*pr=0x8c60805eu;*stop=0x8c60805cu;r[4]=4294967292u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60805e; /* jsr @r2 */
L8c60805e: *stop=0x8c608060u;goto L8c6080b0; /* bra 0x8c6080b0 */
L8c6080a0: *stop=0x8c6080a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6080a2; /* mov.l 0x8c6081c8,r2 */
L8c6080a2: *stop=0x8c6080a2u;r[5]=4u;goto L8c6080a4; /* mov #0x4,r5 */
L8c6080a4: target=r[2];*pr=0x8c6080a8u;*stop=0x8c6080a6u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6080a8; /* jsr @r2 */
L8c6080a8: *stop=0x8c6080a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6080aa; /* mov.l 0x8c6081cc,r3 */
L8c6080aa: *stop=0x8c6080aau;r[5]=1u;goto L8c6080ac; /* mov #0x1,r5 */
L8c6080ac: target=r[3];*pr=0x8c6080b0u;*stop=0x8c6080aeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6080b0; /* jsr @r3 */
L8c6080b0: *stop=0x8c6080b0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6080b2; /* mov.l @(0x4,r15),r3 */
L8c6080b2: *stop=0x8c6080b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081d0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6080b4; /* mov.l 0x8c6081d0,r4 */
L8c6080b4: *stop=0x8c6080b4u;*sr=(*sr&~1u)|(r[4]==r[3]);goto L8c6080b6; /* cmp/eq r4,r3 */
L8c6080b6: if((*sr&1u))goto L8c6080bc;goto L8c6080b8; /* bt 0x8c6080bc */
L8c6080b8: *stop=0x8c6080bau;goto L8c607f9a; /* bra 0x8c607f9a */
L8c6080bc: *stop=0x8c6080bcu;r[1]=sx(m,bus,&io_status,0x8c6081c4u,2);if(io_status)return io_status;goto L8c6080be; /* mov.w 0x8c6081c4,r1 */
L8c6080be: *stop=0x8c6080beu;r[15]+=r[1];goto L8c6080c0; /* add r1,r15 */
L8c6080c0: *stop=0x8c6080c0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6080c2; /* lds.l @r15+,PR */
L8c6080c2: *stop=0x8c6080c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6080c4; /* mov.l @r15+,r13 */
L8c6080c4: *stop=0x8c6080c4u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6080c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c607f90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c607fa4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c607fa4u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c607fdc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c607fdcu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c607fea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c607feau);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c607ff4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c607ff4u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608016(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608016u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608030(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608030u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60803a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60803au);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608052(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608052u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60805e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60805eu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6080a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6080a8u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6080b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c607f90(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6080b0u);}

static int sa_player_texture_load_run_8c6080c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c6080dcu)goto L8c6080dc;if(resume_pc==0x8c6080eeu)goto L8c6080ee;if(resume_pc==0x8c60810cu)goto L8c60810c;if(resume_pc==0x8c608114u)goto L8c608114;if(resume_pc==0x8c60812au)goto L8c60812a;goto L8c6080c8;
L8c6080c8: *stop=0x8c6080c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6080ca; /* mov.l r14,@-r15 */
L8c6080ca: *stop=0x8c6080cau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6080cc; /* mov.l r13,@-r15 */
L8c6080cc: *stop=0x8c6080ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6080ce; /* sts.l PR,@-r15 */
L8c6080ce: *stop=0x8c6080ceu;r[15]+=4294967276u;goto L8c6080d0; /* add #-0x14,r15 */
L8c6080d0: *stop=0x8c6080d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081d0u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c6080d2; /* mov.l 0x8c6081d0,r13 */
L8c6080d2: *stop=0x8c6080d2u;r[14]=r[4];goto L8c6080d4; /* mov r4,r14 */
L8c6080d4: *stop=0x8c6080d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6080d6; /* mov.l 0x8c6081c8,r3 */
L8c6080d6: *stop=0x8c6080d6u;r[5]=4u;goto L8c6080d8; /* mov #0x4,r5 */
L8c6080d8: target=r[3];*pr=0x8c6080dcu;*stop=0x8c6080dau;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6080dc; /* jsr @r3 */
L8c6080dc: *stop=0x8c6080dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081d0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6080de; /* mov.l 0x8c6081d0,r1 */
L8c6080de: *stop=0x8c6080deu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6080e0; /* mov.l @r15,r0 */
L8c6080e0: *stop=0x8c6080e0u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c6080e2; /* cmp/eq r1,r0 */
L8c6080e2: if(!(*sr&1u))goto L8c608102;goto L8c6080e4; /* bf 0x8c608102 */
L8c6080e4: *stop=0x8c6080e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6080e6; /* mov.l 0x8c6081c8,r3 */
L8c6080e6: *stop=0x8c6080e6u;r[4]=r[15];goto L8c6080e8; /* mov r15,r4 */
L8c6080e8: *stop=0x8c6080e8u;r[5]=12u;goto L8c6080ea; /* mov #0xc,r5 */
L8c6080ea: target=r[3];*pr=0x8c6080eeu;*stop=0x8c6080ecu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6080ee; /* jsr @r3 */
L8c6080ee: *stop=0x8c6080eeu;if(!valid_access(r[15]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x10u,2);if(io_status)return io_status;goto L8c6080f0; /* mov.w @(0x10,r15),r0 */
L8c6080f0: *stop=0x8c6080f0u;r[0]=r[0]&65535u;goto L8c6080f2; /* extu.w r0,r0 */
L8c6080f2: *stop=0x8c6080f2u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[0],4);if(io_status)return io_status;goto L8c6080f4; /* mov.l r0,@(0x10,r14) */
L8c6080f4: *stop=0x8c6080f4u;if(!valid_access(r[15]+18u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x12u,2);if(io_status)return io_status;goto L8c6080f6; /* mov.w @(0x12,r15),r0 */
L8c6080f6: *stop=0x8c6080f6u;r[0]=r[0]&65535u;goto L8c6080f8; /* extu.w r0,r0 */
L8c6080f8: *stop=0x8c6080f8u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[0],4);if(io_status)return io_status;goto L8c6080fa; /* mov.l r0,@(0x14,r14) */
L8c6080fa: *stop=0x8c6080fau;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6080fc; /* mov.l @(0xc,r15),r3 */
L8c6080fc: *stop=0x8c6080fcu;r[3]<<=16;goto L8c6080fe; /* shll16 r3 */
L8c6080fe: *stop=0x8c608100u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c608114; /* bra 0x8c608114 */
L8c608102: *stop=0x8c608102u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608104; /* mov.l 0x8c6081c8,r3 */
L8c608104: *stop=0x8c608104u;r[4]=r[15];goto L8c608106; /* mov r15,r4 */
L8c608106: *stop=0x8c608106u;r[5]=4u;goto L8c608108; /* mov #0x4,r5 */
L8c608108: target=r[3];*pr=0x8c60810cu;*stop=0x8c60810au;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60810c; /* jsr @r3 */
L8c60810c: *stop=0x8c60810cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60810e; /* mov.l 0x8c6081cc,r3 */
L8c60810e: *stop=0x8c60810eu;r[5]=1u;goto L8c608110; /* mov #0x1,r5 */
L8c608110: target=r[3];*pr=0x8c608114u;*stop=0x8c608112u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608114; /* jsr @r3 */
L8c608114: *stop=0x8c608114u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c608116; /* mov.l @r15,r2 */
L8c608116: *stop=0x8c608116u;*sr=(*sr&~1u)|(r[13]==r[2]);goto L8c608118; /* cmp/eq r13,r2 */
L8c608118: if(!(*sr&1u))goto L8c6080d4;goto L8c60811a; /* bf 0x8c6080d4 */
L8c60811a: *stop=0x8c60811au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60811c; /* mov.l 0x8c6081d4,r3 */
L8c60811c: *stop=0x8c60811cu;r[5]=1u;goto L8c60811e; /* mov #0x1,r5 */
L8c60811e: *stop=0x8c60811eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6081ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608120; /* mov.l 0x8c6081cc,r2 */
L8c608120: *stop=0x8c608120u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c608122; /* mov.l @r3,r1 */
L8c608122: *stop=0x8c608122u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c608124; /* mov.l r1,@r14 */
L8c608124: *stop=0x8c608124u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c608126; /* mov.l @(0x8,r15),r4 */
L8c608126: target=r[2];*pr=0x8c60812au;*stop=0x8c608128u;r[4]+=4294967288u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60812a; /* jsr @r2 */
L8c60812a: *stop=0x8c60812au;r[15]+=20u;goto L8c60812c; /* add #0x14,r15 */
L8c60812c: *stop=0x8c60812cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c60812e; /* lds.l @r15+,PR */
L8c60812e: *stop=0x8c60812eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c608130; /* mov.l @r15+,r13 */
L8c608130: *stop=0x8c608130u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c608132u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c6080c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c6080c8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6080dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c6080c8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6080dcu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6080ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c6080c8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6080eeu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60810c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c6080c8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60810cu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c608114(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c6080c8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c608114u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c60812a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c6080c8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c60812au);}

SA_STAGE_EXPORT int sa_player_texture_load_8c64da9e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64da9e;
L8c64da9e: *stop=0x8c64da9eu;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64daa0; /* mov.l @(0x18,r4),r6 */
L8c64daa0: *stop=0x8c64daa0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64db98u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64daa2; /* mov.l 0x8c64db98,r2 */
L8c64daa2: *stop=0x8c64daa2u;r[6]>>=16;goto L8c64daa4; /* shlr16 r6 */
L8c64daa4: *stop=0x8c64daa4u;r[1]=sx(m,bus,&io_status,0x8c64db86u,2);if(io_status)return io_status;goto L8c64daa6; /* mov.w 0x8c64db86,r1 */
L8c64daa6: *stop=0x8c64daa6u;r[6]&=r[2];goto L8c64daa8; /* and r2,r6 */
L8c64daa8: *stop=0x8c64daa8u;r[0]=r[6];goto L8c64daaa; /* mov r6,r0 */
L8c64daaa: *stop=0x8c64daaau;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64daac; /* cmp/eq r1,r0 */
L8c64daac: if((*sr&1u))goto L8c64dac4;goto L8c64daae; /* bt 0x8c64dac4 */
L8c64daae: *stop=0x8c64daaeu;r[1]=sx(m,bus,&io_status,0x8c64db88u,2);if(io_status)return io_status;goto L8c64dab0; /* mov.w 0x8c64db88,r1 */
L8c64dab0: *stop=0x8c64dab0u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64dab2; /* cmp/eq r1,r0 */
L8c64dab2: if((*sr&1u))goto L8c64dac4;goto L8c64dab4; /* bt 0x8c64dac4 */
L8c64dab4: *stop=0x8c64dab4u;r[1]=sx(m,bus,&io_status,0x8c64db8au,2);if(io_status)return io_status;goto L8c64dab6; /* mov.w 0x8c64db8a,r1 */
L8c64dab6: *stop=0x8c64dab6u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64dab8; /* cmp/eq r1,r0 */
L8c64dab8: if((*sr&1u))goto L8c64dac8;goto L8c64daba; /* bt 0x8c64dac8 */
L8c64daba: *stop=0x8c64dabau;r[1]=sx(m,bus,&io_status,0x8c64db8cu,2);if(io_status)return io_status;goto L8c64dabc; /* mov.w 0x8c64db8c,r1 */
L8c64dabc: *stop=0x8c64dabcu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64dabe; /* cmp/eq r1,r0 */
L8c64dabe: if((*sr&1u))goto L8c64dac8;goto L8c64dac0; /* bt 0x8c64dac8 */
L8c64dac0: *stop=0x8c64dac0u;if(*pr!=entry_pr)return -3;*stop=0x8c64dac2u;return 0; /* rts */
L8c64dac4: *stop=0x8c64dac6u;r[6]=63u;goto L8c64daca; /* bra 0x8c64daca */
L8c64dac8: *stop=0x8c64dac8u;r[6]=48u;goto L8c64daca; /* mov #0x30,r6 */
L8c64daca: *stop=0x8c64dacau;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64dacc; /* mov.l @(0xc,r4),r2 */
L8c64dacc: *stop=0x8c64daccu;r[6]&=r[5];goto L8c64dace; /* and r5,r6 */
L8c64dace: *stop=0x8c64daceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64db9cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64dad0; /* mov.l 0x8c64db9c,r3 */
L8c64dad0: *stop=0x8c64dad0u;r[2]&=r[3];goto L8c64dad2; /* and r3,r2 */
L8c64dad2: *stop=0x8c64dad2u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[2],4);if(io_status)return io_status;goto L8c64dad4; /* mov.l r2,@(0xc,r4) */
L8c64dad4: *stop=0x8c64dad4u;r[2]=21u;goto L8c64dad6; /* mov #0x15,r2 */
L8c64dad6: *stop=0x8c64dad6u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64dad8; /* mov.l @(0xc,r4),r1 */
L8c64dad8: *stop=0x8c64dad8u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[6]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[6]=n?(uint32_t)((int32_t)r[6]>>n):((r[6]&0x80000000u)?0xffffffffu:0u);}}goto L8c64dada; /* shad r2,r6 */
L8c64dada: *stop=0x8c64dadau;r[1]|=r[6];goto L8c64dadc; /* or r6,r1 */
L8c64dadc: *stop=0x8c64dadcu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[1],4);if(io_status)return io_status;goto L8c64dade; /* mov.l r1,@(0xc,r4) */
L8c64dade: *stop=0x8c64dadeu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c64dae0; /* mov.l r5,@(0x4,r4) */
L8c64dae0: *stop=0x8c64dae0u;if(*pr!=entry_pr)return -3;*stop=0x8c64dae2u;return 0; /* rts */
}

static int sa_player_texture_load_run_8c64df66(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64df78u)goto L8c64df78;if(resume_pc==0x8c64df86u)goto L8c64df86;goto L8c64df66;
L8c64df66: *stop=0x8c64df66u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64df68; /* mov.l r14,@-r15 */
L8c64df68: *stop=0x8c64df68u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64df6a; /* mov.l r13,@-r15 */
L8c64df6a: *stop=0x8c64df6au;r[13]=r[4];goto L8c64df6c; /* mov r4,r13 */
L8c64df6c: *stop=0x8c64df6cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64df6e; /* mov.l r12,@-r15 */
L8c64df6e: *stop=0x8c64df6eu;r[13]+=24u;goto L8c64df70; /* add #0x18,r13 */
L8c64df70: *stop=0x8c64df70u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64df72; /* sts.l PR,@-r15 */
L8c64df72: *stop=0x8c64df72u;r[12]=r[4];goto L8c64df74; /* mov r4,r12 */
L8c64df74: target=0x8c64df10u;*pr=0x8c64df78u;*stop=0x8c64df76u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64df78; /* bsr 0x8c64df10 */
L8c64df78: *stop=0x8c64df78u;r[14]=r[0];goto L8c64df7a; /* mov r0,r14 */
L8c64df7a: *stop=0x8c64df7au;r[14]<<=2;goto L8c64df7c; /* shll2 r14 */
L8c64df7c: *stop=0x8c64df7cu;r[2]=4294967288u;goto L8c64df7e; /* mov #-0x8,r2 */
L8c64df7e: *stop=0x8c64df7eu;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c64df80; /* shll r14 */
L8c64df80: *stop=0x8c64df80u;r[14]&=r[2];goto L8c64df82; /* and r2,r14 */
L8c64df82: target=0x8c64df10u;*pr=0x8c64df86u;*stop=0x8c64df84u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64df86; /* bsr 0x8c64df10 */
L8c64df86: *stop=0x8c64df86u;r[4]=r[14];goto L8c64df88; /* mov r14,r4 */
L8c64df88: *stop=0x8c64df88u;r[4]|=r[0];goto L8c64df8a; /* or r0,r4 */
L8c64df8a: *stop=0x8c64df8au;r[0]=1u;goto L8c64df8c; /* mov #0x1,r0 */
L8c64df8c: *stop=0x8c64df8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e034u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64df8e; /* mov.l 0x8c64e034,r3 */
L8c64df8e: *stop=0x8c64df8eu;if(!valid_access(r[12]+8u,4u))return -3;wr(m,bus,&io_status,r[12]+0x8u,r[4],4);if(io_status)return io_status;goto L8c64df90; /* mov.l r4,@(0x8,r12) */
L8c64df90: *stop=0x8c64df90u;r[2]=8u;goto L8c64df92; /* mov #0x8,r2 */
L8c64df92: *stop=0x8c64df92u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64df94; /* mov.l @(0x18,r13),r6 */
L8c64df94: *stop=0x8c64df94u;r[1]=27u;goto L8c64df96; /* mov #0x1b,r1 */
L8c64df96: *stop=0x8c64df96u;r[5]=4u;goto L8c64df98; /* mov #0x4,r5 */
L8c64df98: *stop=0x8c64df98u;r[0]&=r[6];goto L8c64df9a; /* and r6,r0 */
L8c64df9a: *stop=0x8c64df9au;r[0]&=0x1u;goto L8c64df9c; /* and #0x1,r0 */
L8c64df9c: *stop=0x8c64df9cu;*sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]<<31);goto L8c64df9e; /* rotr r0 */
L8c64df9e: *stop=0x8c64df9eu;r[4]=r[0];goto L8c64dfa0; /* mov r0,r4 */
L8c64dfa0: *stop=0x8c64dfa0u;r[4]&=r[3];goto L8c64dfa2; /* and r3,r4 */
L8c64dfa2: *stop=0x8c64dfa2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e03cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64dfa4; /* mov.l 0x8c64e03c,r3 */
L8c64dfa4: *stop=0x8c64dfa4u;r[2]&=r[6];goto L8c64dfa6; /* and r6,r2 */
L8c64dfa6: *stop=0x8c64dfa6u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64dfa8; /* mov.l @(0x8,r13),r0 */
L8c64dfa8: *stop=0x8c64dfa8u;r[2]=(r[1]&0x80000000u)?((r[1]&31u)?r[2]>>((0u-r[1])&31u):0u):(r[2]<<(r[1]&31u));goto L8c64dfaa; /* shld r1,r2 */
L8c64dfaa: *stop=0x8c64dfaau;r[4]|=r[2];goto L8c64dfac; /* or r2,r4 */
L8c64dfac: *stop=0x8c64dfacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e038u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64dfae; /* mov.l 0x8c64e038,r2 */
L8c64dfae: *stop=0x8c64dfaeu;r[0]&=r[3];goto L8c64dfb0; /* and r3,r0 */
L8c64dfb0: *stop=0x8c64dfb0u;r[3]=r[6];goto L8c64dfb2; /* mov r6,r3 */
L8c64dfb2: *stop=0x8c64dfb2u;r[3]&=r[5];goto L8c64dfb4; /* and r5,r3 */
L8c64dfb4: *stop=0x8c64dfb4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e040u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64dfb6; /* mov.l 0x8c64e040,r1 */
L8c64dfb6: *stop=0x8c64dfb6u;r[3]=~r[3];goto L8c64dfb8; /* not r3,r3 */
L8c64dfb8: *stop=0x8c64dfb8u;r[3]&=r[5];goto L8c64dfba; /* and r5,r3 */
L8c64dfba: *stop=0x8c64dfbau;r[4]&=r[2];goto L8c64dfbc; /* and r2,r4 */
L8c64dfbc: *stop=0x8c64dfbcu;r[3]<<=16;goto L8c64dfbe; /* shll16 r3 */
L8c64dfbe: *stop=0x8c64dfbeu;r[4]|=r[0];goto L8c64dfc0; /* or r0,r4 */
L8c64dfc0: *stop=0x8c64dfc0u;r[3]<<=8;goto L8c64dfc2; /* shll8 r3 */
L8c64dfc2: *stop=0x8c64dfc2u;r[4]&=r[1];goto L8c64dfc4; /* and r1,r4 */
L8c64dfc4: *stop=0x8c64dfc4u;r[4]|=r[3];goto L8c64dfc6; /* or r3,r4 */
L8c64dfc6: *stop=0x8c64dfc6u;r[3]=16u;goto L8c64dfc8; /* mov #0x10,r3 */
L8c64dfc8: *stop=0x8c64dfc8u;if(!valid_access(r[12]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64dfca; /* mov.l @(0x18,r12),r5 */
L8c64dfca: *stop=0x8c64dfcau;r[2]=21u;goto L8c64dfcc; /* mov #0x15,r2 */
L8c64dfcc: *stop=0x8c64dfccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e04cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64dfce; /* mov.l 0x8c64e04c,r1 */
L8c64dfce: *stop=0x8c64dfceu;r[6]&=r[3];goto L8c64dfd0; /* and r3,r6 */
L8c64dfd0: *stop=0x8c64dfd0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e044u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64dfd2; /* mov.l 0x8c64e044,r0 */
L8c64dfd2: *stop=0x8c64dfd2u;r[5]>>=16;goto L8c64dfd4; /* shlr16 r5 */
L8c64dfd4: *stop=0x8c64dfd4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e048u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64dfd6; /* mov.l 0x8c64e048,r3 */
L8c64dfd6: *stop=0x8c64dfd6u;r[6]=(r[2]&0x80000000u)?((r[2]&31u)?r[6]>>((0u-r[2])&31u):0u):(r[6]<<(r[2]&31u));goto L8c64dfd8; /* shld r2,r6 */
L8c64dfd8: *stop=0x8c64dfd8u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64dfda; /* mov.l @(0x1c,r13),r2 */
L8c64dfda: *stop=0x8c64dfdau;r[5]&=r[1];goto L8c64dfdc; /* and r1,r5 */
L8c64dfdc: *stop=0x8c64dfdcu;r[1]=sx(m,bus,&io_status,0x8c64e02au,2);if(io_status)return io_status;goto L8c64dfde; /* mov.w 0x8c64e02a,r1 */
L8c64dfde: *stop=0x8c64dfdeu;r[4]&=r[0];goto L8c64dfe0; /* and r0,r4 */
L8c64dfe0: *stop=0x8c64dfe0u;r[0]=r[5];goto L8c64dfe2; /* mov r5,r0 */
L8c64dfe2: *stop=0x8c64dfe2u;r[4]|=r[6];goto L8c64dfe4; /* or r6,r4 */
L8c64dfe4: *stop=0x8c64dfe4u;r[2]>>=2;goto L8c64dfe6; /* shlr2 r2 */
L8c64dfe6: *stop=0x8c64dfe6u;r[4]&=r[3];goto L8c64dfe8; /* and r3,r4 */
L8c64dfe8: *stop=0x8c64dfe8u;*sr=(*sr&~1u)|(r[2]&1u);r[2]>>=1;goto L8c64dfea; /* shlr r2 */
L8c64dfea: *stop=0x8c64dfeau;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64dfec; /* cmp/eq r1,r0 */
L8c64dfec: take=(*sr&1u);*stop=0x8c64dfeeu;r[4]|=r[2];if(take)goto L8c64e006;goto L8c64dff0; /* bt/s 0x8c64e006 */
L8c64dff0: *stop=0x8c64dff0u;r[1]=sx(m,bus,&io_status,0x8c64e02cu,2);if(io_status)return io_status;goto L8c64dff2; /* mov.w 0x8c64e02c,r1 */
L8c64dff2: *stop=0x8c64dff2u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64dff4; /* cmp/eq r1,r0 */
L8c64dff4: if((*sr&1u))goto L8c64e006;goto L8c64dff6; /* bt 0x8c64e006 */
L8c64dff6: *stop=0x8c64dff6u;r[1]=sx(m,bus,&io_status,0x8c64e02eu,2);if(io_status)return io_status;goto L8c64dff8; /* mov.w 0x8c64e02e,r1 */
L8c64dff8: *stop=0x8c64dff8u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64dffa; /* cmp/eq r1,r0 */
L8c64dffa: if((*sr&1u))goto L8c64e00c;goto L8c64dffc; /* bt 0x8c64e00c */
L8c64dffc: *stop=0x8c64dffcu;r[1]=sx(m,bus,&io_status,0x8c64e030u,2);if(io_status)return io_status;goto L8c64dffe; /* mov.w 0x8c64e030,r1 */
L8c64dffe: *stop=0x8c64dffeu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64e000; /* cmp/eq r1,r0 */
L8c64e000: if((*sr&1u))goto L8c64e00c;goto L8c64e002; /* bt 0x8c64e00c */
L8c64e002: *stop=0x8c64e004u;goto L8c64e01e; /* bra 0x8c64e01e */
L8c64e006: *stop=0x8c64e006u;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e008; /* mov.l @(0x4,r12),r0 */
L8c64e008: *stop=0x8c64e00au;r[0]&=0x3fu;goto L8c64e010; /* bra 0x8c64e010 */
L8c64e00c: *stop=0x8c64e00cu;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e00e; /* mov.l @(0x4,r12),r0 */
L8c64e00e: *stop=0x8c64e00eu;r[0]&=0x30u;goto L8c64e010; /* and #0x30,r0 */
L8c64e010: *stop=0x8c64e010u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e050u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e012; /* mov.l 0x8c64e050,r2 */
L8c64e012: *stop=0x8c64e012u;r[5]=r[0];goto L8c64e014; /* mov r0,r5 */
L8c64e014: *stop=0x8c64e014u;r[3]=21u;goto L8c64e016; /* mov #0x15,r3 */
L8c64e016: *stop=0x8c64e016u;r[2]&=r[4];goto L8c64e018; /* and r4,r2 */
L8c64e018: *stop=0x8c64e018u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}}goto L8c64e01a; /* shad r3,r5 */
L8c64e01a: *stop=0x8c64e01au;r[4]=r[5];goto L8c64e01c; /* mov r5,r4 */
L8c64e01c: *stop=0x8c64e01cu;r[4]|=r[2];goto L8c64e01e; /* or r2,r4 */
L8c64e01e: *stop=0x8c64e01eu;if(!valid_access(r[12]+12u,4u))return -3;wr(m,bus,&io_status,r[12]+0xcu,r[4],4);if(io_status)return io_status;goto L8c64e020; /* mov.l r4,@(0xc,r12) */
L8c64e020: *stop=0x8c64e020u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64e022; /* lds.l @r15+,PR */
L8c64e022: *stop=0x8c64e022u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64e024; /* mov.l @r15+,r12 */
L8c64e024: *stop=0x8c64e024u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64e026; /* mov.l @r15+,r13 */
L8c64e026: *stop=0x8c64e026u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64e028u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c64df66(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64df66(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64df78(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64df66(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64df78u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64df86(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64df66(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64df86u);}

SA_STAGE_EXPORT int sa_player_texture_load_8c64e61e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64e61e;
L8c64e61e: *stop=0x8c64e61eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64e620; /* mov.l r14,@-r15 */
L8c64e620: *stop=0x8c64e620u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64e622; /* mov.l r13,@-r15 */
L8c64e622: *stop=0x8c64e622u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64e624; /* mov.l r12,@-r15 */
L8c64e624: *stop=0x8c64e624u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64e626; /* mov.l r11,@-r15 */
L8c64e626: *stop=0x8c64e626u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64e628; /* mov.l r10,@-r15 */
L8c64e628: *stop=0x8c64e628u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64e62a; /* mov.l r9,@-r15 */
L8c64e62a: *stop=0x8c64e62au;r[9]=0u;goto L8c64e62c; /* mov #0x0,r9 */
L8c64e62c: *stop=0x8c64e62cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64e62e; /* mov.l r8,@-r15 */
L8c64e62e: *stop=0x8c64e62eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e818u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c64e630; /* mov.l 0x8c64e818,r8 */
L8c64e630: *stop=0x8c64e630u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e810u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64e632; /* mov.l 0x8c64e810,r14 */
L8c64e632: *stop=0x8c64e632u;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[10]=loaded;}goto L8c64e634; /* mov.l @r8,r10 */
L8c64e634: *stop=0x8c64e634u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[11]=loaded;}goto L8c64e636; /* mov.l @r14,r11 */
L8c64e636: *stop=0x8c64e636u;r[0]=r[10];goto L8c64e638; /* mov r10,r0 */
L8c64e638: *stop=0x8c64e638u;r[0]<<=2;goto L8c64e63a; /* shll2 r0 */
L8c64e63a: *stop=0x8c64e63au;r[2]=r[10];goto L8c64e63c; /* mov r10,r2 */
L8c64e63c: *stop=0x8c64e63cu;r[0]<<=2;goto L8c64e63e; /* shll2 r0 */
L8c64e63e: *stop=0x8c64e63eu;r[0]+=r[2];goto L8c64e640; /* add r2,r0 */
L8c64e640: *stop=0x8c64e640u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e814u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e642; /* mov.l 0x8c64e814,r3 */
L8c64e642: *stop=0x8c64e642u;r[0]<<=2;goto L8c64e644; /* shll2 r0 */
L8c64e644: *stop=0x8c64e644u;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e646; /* mov.l @(r0,r11),r0 */
L8c64e646: *stop=0x8c64e646u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c64e648; /* cmp/eq #-0x1,r0 */
L8c64e648: take=!(*sr&1u);*stop=0x8c64e64au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[13]=loaded;}if(take)goto L8c64e650;goto L8c64e64c; /* bf/s 0x8c64e650 */
L8c64e64c: *stop=0x8c64e64eu;r[12]=r[10];goto L8c64e66c; /* bra 0x8c64e66c */
L8c64e650: *stop=0x8c64e652u;r[12]=r[9];goto L8c64e668; /* bra 0x8c64e668 */
L8c64e654: *stop=0x8c64e654u;r[0]=r[12];goto L8c64e656; /* mov r12,r0 */
L8c64e656: *stop=0x8c64e656u;r[0]<<=2;goto L8c64e658; /* shll2 r0 */
L8c64e658: *stop=0x8c64e658u;r[0]<<=2;goto L8c64e65a; /* shll2 r0 */
L8c64e65a: *stop=0x8c64e65au;r[3]=r[12];goto L8c64e65c; /* mov r12,r3 */
L8c64e65c: *stop=0x8c64e65cu;r[0]+=r[3];goto L8c64e65e; /* add r3,r0 */
L8c64e65e: *stop=0x8c64e65eu;r[0]<<=2;goto L8c64e660; /* shll2 r0 */
L8c64e660: *stop=0x8c64e660u;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e662; /* mov.l @(r0,r11),r0 */
L8c64e662: *stop=0x8c64e662u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c64e664; /* cmp/eq #-0x1,r0 */
L8c64e664: if((*sr&1u))goto L8c64e66c;goto L8c64e666; /* bt 0x8c64e66c */
L8c64e666: *stop=0x8c64e666u;r[12]+=1u;goto L8c64e668; /* add #0x1,r12 */
L8c64e668: *stop=0x8c64e668u;*sr=(*sr&~1u)|(r[12]>=r[13]);goto L8c64e66a; /* cmp/hs r13,r12 */
L8c64e66a: if(!(*sr&1u))goto L8c64e654;goto L8c64e66c; /* bf 0x8c64e654 */
L8c64e66c: *stop=0x8c64e66cu;r[13]+=4294967295u;goto L8c64e66e; /* add #-0x1,r13 */
L8c64e66e: *stop=0x8c64e66eu;*sr=(*sr&~1u)|(r[13]==r[12]);goto L8c64e670; /* cmp/eq r13,r12 */
L8c64e670: take=(*sr&1u);*stop=0x8c64e672u;r[13]=r[12];if(take)goto L8c64e67c;goto L8c64e674; /* bt/s 0x8c64e67c */
L8c64e674: *stop=0x8c64e674u;r[2]=r[12];goto L8c64e676; /* mov r12,r2 */
L8c64e676: *stop=0x8c64e676u;r[2]+=1u;goto L8c64e678; /* add #0x1,r2 */
L8c64e678: *stop=0x8c64e67au;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c64e67e; /* bra 0x8c64e67e */
L8c64e67c: *stop=0x8c64e67cu;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[9],4);if(io_status)return io_status;goto L8c64e67e; /* mov.l r9,@r8 */
L8c64e67e: *stop=0x8c64e67eu;r[13]<<=2;goto L8c64e680; /* shll2 r13 */
L8c64e680: *stop=0x8c64e680u;r[3]=r[12];goto L8c64e682; /* mov r12,r3 */
L8c64e682: *stop=0x8c64e682u;r[13]<<=2;goto L8c64e684; /* shll2 r13 */
L8c64e684: *stop=0x8c64e684u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e686; /* mov.l @r14,r2 */
L8c64e686: *stop=0x8c64e686u;r[13]+=r[3];goto L8c64e688; /* add r3,r13 */
L8c64e688: *stop=0x8c64e688u;r[13]<<=2;goto L8c64e68a; /* shll2 r13 */
L8c64e68a: *stop=0x8c64e68au;r[2]+=r[13];goto L8c64e68c; /* add r13,r2 */
L8c64e68c: *stop=0x8c64e68cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c64e68e; /* mov.l r2,@r4 */
L8c64e68e: *stop=0x8c64e68eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e690; /* mov.l @r14,r3 */
L8c64e690: *stop=0x8c64e690u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e692; /* mov.l @(0x4,r5),r2 */
L8c64e692: *stop=0x8c64e692u;r[3]+=r[13];goto L8c64e694; /* add r13,r3 */
L8c64e694: *stop=0x8c64e694u;if(!valid_access(r[3]+24u,4u))return -3;wr(m,bus,&io_status,r[3]+0x18u,r[2],4);if(io_status)return io_status;goto L8c64e696; /* mov.l r2,@(0x18,r3) */
L8c64e696: *stop=0x8c64e696u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e698; /* mov.l @r14,r3 */
L8c64e698: *stop=0x8c64e698u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e69a; /* mov.l @(0x8,r5),r2 */
L8c64e69a: *stop=0x8c64e69au;r[3]+=r[13];goto L8c64e69c; /* add r13,r3 */
L8c64e69c: *stop=0x8c64e69cu;if(!valid_access(r[3]+28u,4u))return -3;wr(m,bus,&io_status,r[3]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c64e69e; /* mov.l r2,@(0x1c,r3) */
L8c64e69e: *stop=0x8c64e69eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e6a0; /* mov.l @r14,r3 */
L8c64e6a0: *stop=0x8c64e6a0u;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e6a2; /* mov.l @(0xc,r5),r2 */
L8c64e6a2: *stop=0x8c64e6a2u;r[3]+=r[13];goto L8c64e6a4; /* add r13,r3 */
L8c64e6a4: *stop=0x8c64e6a4u;if(!valid_access(r[3]+32u,4u))return -3;wr(m,bus,&io_status,r[3]+0x20u,r[2],4);if(io_status)return io_status;goto L8c64e6a6; /* mov.l r2,@(0x20,r3) */
L8c64e6a6: *stop=0x8c64e6a6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e6a8; /* mov.l @r14,r3 */
L8c64e6a8: *stop=0x8c64e6a8u;if(!valid_access(r[5]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e6aa; /* mov.l @(0x10,r5),r2 */
L8c64e6aa: *stop=0x8c64e6aau;r[3]+=r[13];goto L8c64e6ac; /* add r13,r3 */
L8c64e6ac: *stop=0x8c64e6acu;if(!valid_access(r[3]+36u,4u))return -3;wr(m,bus,&io_status,r[3]+0x24u,r[2],4);if(io_status)return io_status;goto L8c64e6ae; /* mov.l r2,@(0x24,r3) */
L8c64e6ae: *stop=0x8c64e6aeu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e6b0; /* mov.l @r14,r3 */
L8c64e6b0: *stop=0x8c64e6b0u;if(!valid_access(r[5]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e6b2; /* mov.l @(0x14,r5),r2 */
L8c64e6b2: *stop=0x8c64e6b2u;r[3]+=r[13];goto L8c64e6b4; /* add r13,r3 */
L8c64e6b4: *stop=0x8c64e6b4u;if(!valid_access(r[3]+40u,4u))return -3;wr(m,bus,&io_status,r[3]+0x28u,r[2],4);if(io_status)return io_status;goto L8c64e6b6; /* mov.l r2,@(0x28,r3) */
L8c64e6b6: *stop=0x8c64e6b6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e6b8; /* mov.l @r14,r3 */
L8c64e6b8: *stop=0x8c64e6b8u;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e6ba; /* mov.l @(0x18,r5),r2 */
L8c64e6ba: *stop=0x8c64e6bau;r[3]+=r[13];goto L8c64e6bc; /* add r13,r3 */
L8c64e6bc: *stop=0x8c64e6bcu;if(!valid_access(r[3]+44u,4u))return -3;wr(m,bus,&io_status,r[3]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c64e6be; /* mov.l r2,@(0x2c,r3) */
L8c64e6be: *stop=0x8c64e6beu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e6c0; /* mov.l @r14,r3 */
L8c64e6c0: *stop=0x8c64e6c0u;if(!valid_access(r[5]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e6c2; /* mov.l @(0x1c,r5),r2 */
L8c64e6c2: *stop=0x8c64e6c2u;r[3]+=r[13];goto L8c64e6c4; /* add r13,r3 */
L8c64e6c4: *stop=0x8c64e6c4u;if(!valid_access(r[3]+48u,4u))return -3;wr(m,bus,&io_status,r[3]+0x30u,r[2],4);if(io_status)return io_status;goto L8c64e6c6; /* mov.l r2,@(0x30,r3) */
L8c64e6c6: *stop=0x8c64e6c6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e6c8; /* mov.l @r14,r3 */
L8c64e6c8: *stop=0x8c64e6c8u;if(!valid_access(r[5]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e6ca; /* mov.l @(0x20,r5),r2 */
L8c64e6ca: *stop=0x8c64e6cau;r[3]+=r[13];goto L8c64e6cc; /* add r13,r3 */
L8c64e6cc: *stop=0x8c64e6ccu;if(!valid_access(r[3]+52u,4u))return -3;wr(m,bus,&io_status,r[3]+0x34u,r[2],4);if(io_status)return io_status;goto L8c64e6ce; /* mov.l r2,@(0x34,r3) */
L8c64e6ce: *stop=0x8c64e6ceu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e6d0; /* mov.l @r14,r3 */
L8c64e6d0: *stop=0x8c64e6d0u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e6d2; /* mov.l @r5,r2 */
L8c64e6d2: *stop=0x8c64e6d2u;r[3]+=r[13];goto L8c64e6d4; /* add r13,r3 */
L8c64e6d4: *stop=0x8c64e6d4u;if(!valid_access(r[3]+20u,4u))return -3;wr(m,bus,&io_status,r[3]+0x14u,r[2],4);if(io_status)return io_status;goto L8c64e6d6; /* mov.l r2,@(0x14,r3) */
L8c64e6d6: *stop=0x8c64e6d6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e6d8; /* mov.l @r14,r3 */
L8c64e6d8: *stop=0x8c64e6d8u;r[3]+=r[13];goto L8c64e6da; /* add r13,r3 */
L8c64e6da: *stop=0x8c64e6dau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c64e6dc; /* mov.l r6,@r3 */
L8c64e6dc: *stop=0x8c64e6dcu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e6de; /* mov.l @r14,r2 */
L8c64e6de: *stop=0x8c64e6deu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e6e0; /* mov.l @(0x1c,r15),r3 */
L8c64e6e0: *stop=0x8c64e6e0u;r[2]+=r[13];goto L8c64e6e2; /* add r13,r2 */
L8c64e6e2: *stop=0x8c64e6e2u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c64e6e4; /* mov.l r3,@(0x4,r2) */
L8c64e6e4: *stop=0x8c64e6e4u;r[3]=6u;goto L8c64e6e6; /* mov #0x6,r3 */
L8c64e6e6: *stop=0x8c64e6e6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e6e8; /* mov.l @r14,r2 */
L8c64e6e8: *stop=0x8c64e6e8u;*sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c64e6ea; /* tst r3,r7 */
L8c64e6ea: *stop=0x8c64e6eau;r[2]+=r[13];goto L8c64e6ec; /* add r13,r2 */
L8c64e6ec: *stop=0x8c64e6ecu;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[7],4);if(io_status)return io_status;goto L8c64e6ee; /* mov.l r7,@(0x10,r2) */
L8c64e6ee: take=(*sr&1u);*stop=0x8c64e6f0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(take)goto L8c64e6fc;goto L8c64e6f2; /* bt/s 0x8c64e6fc */
L8c64e6f2: *stop=0x8c64e6f2u;r[0]=64u;goto L8c64e6f4; /* mov #0x40,r0 */
L8c64e6f4: *stop=0x8c64e6f4u;r[1]=1u;goto L8c64e6f6; /* mov #0x1,r1 */
L8c64e6f6: *stop=0x8c64e6f6u;r[4]+=r[13];goto L8c64e6f8; /* add r13,r4 */
L8c64e6f8: *stop=0x8c64e6fau;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[1],2);if(io_status)return io_status;goto L8c64e702; /* bra 0x8c64e702 */
L8c64e6fc: *stop=0x8c64e6fcu;r[0]=64u;goto L8c64e6fe; /* mov #0x40,r0 */
L8c64e6fe: *stop=0x8c64e6feu;r[4]+=r[13];goto L8c64e700; /* add r13,r4 */
L8c64e700: *stop=0x8c64e700u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[9],2);if(io_status)return io_status;goto L8c64e702; /* mov.w r9,@(r0,r4) */
L8c64e702: *stop=0x8c64e702u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e81cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64e704; /* mov.l 0x8c64e81c,r4 */
L8c64e704: *stop=0x8c64e704u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e706; /* mov.l @r4,r3 */
L8c64e706: *stop=0x8c64e706u;*sr=(*sr&~1u)|(r[3]>r[12]);goto L8c64e708; /* cmp/hi r12,r3 */
L8c64e708: if((*sr&1u))goto L8c64e710;goto L8c64e70a; /* bt 0x8c64e710 */
L8c64e70a: *stop=0x8c64e70au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e70c; /* mov.l @r4,r1 */
L8c64e70c: *stop=0x8c64e70cu;r[1]+=1u;goto L8c64e70e; /* add #0x1,r1 */
L8c64e70e: *stop=0x8c64e70eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c64e710; /* mov.l r1,@r4 */
L8c64e710: *stop=0x8c64e710u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64e712; /* mov.l @r15+,r8 */
L8c64e712: *stop=0x8c64e712u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64e714; /* mov.l @r15+,r9 */
L8c64e714: *stop=0x8c64e714u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64e716; /* mov.l @r15+,r10 */
L8c64e716: *stop=0x8c64e716u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64e718; /* mov.l @r15+,r11 */
L8c64e718: *stop=0x8c64e718u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64e71a; /* mov.l @r15+,r12 */
L8c64e71a: *stop=0x8c64e71au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64e71c; /* mov.l @r15+,r13 */
L8c64e71c: *stop=0x8c64e71cu;if(*pr!=entry_pr)return -3;*stop=0x8c64e71eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c64e7a2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64e7a2;
L8c64e7a2: *stop=0x8c64e7a2u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64e7a4; /* tst r4,r4 */
L8c64e7a4: if((*sr&1u))goto L8c64e7cc;goto L8c64e7a6; /* bt 0x8c64e7cc */
L8c64e7a6: *stop=0x8c64e7a6u;r[7]=0u;goto L8c64e7a8; /* mov #0x0,r7 */
L8c64e7a8: *stop=0x8c64e7a8u;r[3]=r[7];goto L8c64e7aa; /* mov r7,r3 */
L8c64e7aa: *stop=0x8c64e7aau;*sr=(*sr&~1u)|(r[3]>=r[5]);goto L8c64e7ac; /* cmp/hs r5,r3 */
L8c64e7ac: if((*sr&1u))goto L8c64e7cc;goto L8c64e7ae; /* bt 0x8c64e7cc */
L8c64e7ae: *stop=0x8c64e7aeu;r[1]=r[7];goto L8c64e7b0; /* mov r7,r1 */
L8c64e7b0: *stop=0x8c64e7b0u;r[1]<<=2;goto L8c64e7b2; /* shll2 r1 */
L8c64e7b2: *stop=0x8c64e7b2u;r[1]<<=2;goto L8c64e7b4; /* shll2 r1 */
L8c64e7b4: *stop=0x8c64e7b4u;r[3]=r[7];goto L8c64e7b6; /* mov r7,r3 */
L8c64e7b6: *stop=0x8c64e7b6u;r[1]+=r[3];goto L8c64e7b8; /* add r3,r1 */
L8c64e7b8: *stop=0x8c64e7b8u;r[1]<<=2;goto L8c64e7ba; /* shll2 r1 */
L8c64e7ba: *stop=0x8c64e7bau;r[1]+=r[4];goto L8c64e7bc; /* add r4,r1 */
L8c64e7bc: *stop=0x8c64e7bcu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e7be; /* mov.l @r1,r2 */
L8c64e7be: *stop=0x8c64e7beu;*sr=(*sr&~1u)|(r[6]==r[2]);goto L8c64e7c0; /* cmp/eq r6,r2 */
L8c64e7c0: if(!(*sr&1u))goto L8c64e7c6;goto L8c64e7c2; /* bf 0x8c64e7c6 */
L8c64e7c2: *stop=0x8c64e7c2u;if(*pr!=entry_pr)return -3;*stop=0x8c64e7c4u;r[0]=r[7];return 0; /* rts */
L8c64e7c6: *stop=0x8c64e7c6u;r[7]+=1u;goto L8c64e7c8; /* add #0x1,r7 */
L8c64e7c8: *stop=0x8c64e7c8u;*sr=(*sr&~1u)|(r[7]>=r[5]);goto L8c64e7ca; /* cmp/hs r5,r7 */
L8c64e7ca: if(!(*sr&1u))goto L8c64e7ae;goto L8c64e7cc; /* bf 0x8c64e7ae */
L8c64e7cc: *stop=0x8c64e7ccu;r[0]=4294967295u;goto L8c64e7ce; /* mov #-0x1,r0 */
L8c64e7ce: *stop=0x8c64e7ceu;if(*pr!=entry_pr)return -3;*stop=0x8c64e7d0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c64e828(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64e828;
L8c64e828: *stop=0x8c64e828u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64e82a; /* tst r4,r4 */
L8c64e82a: *stop=0x8c64e82au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64e82c; /* mov.l r14,@-r15 */
L8c64e82c: if((*sr&1u))goto L8c64e86a;goto L8c64e82e; /* bt 0x8c64e86a */
L8c64e82e: *stop=0x8c64e82eu;r[14]=0u;goto L8c64e830; /* mov #0x0,r14 */
L8c64e830: *stop=0x8c64e830u;r[3]=r[14];goto L8c64e832; /* mov r14,r3 */
L8c64e832: *stop=0x8c64e832u;*sr=(*sr&~1u)|(r[3]>=r[5]);goto L8c64e834; /* cmp/hs r5,r3 */
L8c64e834: if((*sr&1u))goto L8c64e86a;goto L8c64e836; /* bt 0x8c64e86a */
L8c64e836: *stop=0x8c64e836u;r[1]=r[14];goto L8c64e838; /* mov r14,r1 */
L8c64e838: *stop=0x8c64e838u;r[1]<<=2;goto L8c64e83a; /* shll2 r1 */
L8c64e83a: *stop=0x8c64e83au;r[1]<<=2;goto L8c64e83c; /* shll2 r1 */
L8c64e83c: *stop=0x8c64e83cu;r[3]=r[14];goto L8c64e83e; /* mov r14,r3 */
L8c64e83e: *stop=0x8c64e83eu;r[1]+=r[3];goto L8c64e840; /* add r3,r1 */
L8c64e840: *stop=0x8c64e840u;r[1]<<=2;goto L8c64e842; /* shll2 r1 */
L8c64e842: *stop=0x8c64e842u;r[1]+=r[4];goto L8c64e844; /* add r4,r1 */
L8c64e844: *stop=0x8c64e844u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e846; /* mov.l @r1,r2 */
L8c64e846: *stop=0x8c64e846u;*sr=(*sr&~1u)|(r[6]==r[2]);goto L8c64e848; /* cmp/eq r6,r2 */
L8c64e848: if(!(*sr&1u))goto L8c64e864;goto L8c64e84a; /* bf 0x8c64e864 */
L8c64e84a: *stop=0x8c64e84au;r[1]=r[14];goto L8c64e84c; /* mov r14,r1 */
L8c64e84c: *stop=0x8c64e84cu;r[1]<<=2;goto L8c64e84e; /* shll2 r1 */
L8c64e84e: *stop=0x8c64e84eu;r[1]<<=2;goto L8c64e850; /* shll2 r1 */
L8c64e850: *stop=0x8c64e850u;r[3]=r[14];goto L8c64e852; /* mov r14,r3 */
L8c64e852: *stop=0x8c64e852u;r[1]+=r[3];goto L8c64e854; /* add r3,r1 */
L8c64e854: *stop=0x8c64e854u;r[1]<<=2;goto L8c64e856; /* shll2 r1 */
L8c64e856: *stop=0x8c64e856u;r[1]+=r[4];goto L8c64e858; /* add r4,r1 */
L8c64e858: *stop=0x8c64e858u;if(!valid_access(r[1]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e85a; /* mov.l @(0x4,r1),r2 */
L8c64e85a: *stop=0x8c64e85au;*sr=(*sr&~1u)|(r[7]==r[2]);goto L8c64e85c; /* cmp/eq r7,r2 */
L8c64e85c: if(!(*sr&1u))goto L8c64e864;goto L8c64e85e; /* bf 0x8c64e864 */
L8c64e85e: *stop=0x8c64e85eu;r[0]=r[14];goto L8c64e860; /* mov r14,r0 */
L8c64e860: *stop=0x8c64e860u;if(*pr!=entry_pr)return -3;*stop=0x8c64e862u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c64e864: *stop=0x8c64e864u;r[14]+=1u;goto L8c64e866; /* add #0x1,r14 */
L8c64e866: *stop=0x8c64e866u;*sr=(*sr&~1u)|(r[14]>=r[5]);goto L8c64e868; /* cmp/hs r5,r14 */
L8c64e868: if(!(*sr&1u))goto L8c64e836;goto L8c64e86a; /* bf 0x8c64e836 */
L8c64e86a: *stop=0x8c64e86au;r[0]=4294967295u;goto L8c64e86c; /* mov #-0x1,r0 */
L8c64e86c: *stop=0x8c64e86cu;if(*pr!=entry_pr)return -3;*stop=0x8c64e86eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

static int sa_player_texture_load_run_8c64e870(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64e8beu)goto L8c64e8be;if(resume_pc==0x8c64e8c4u)goto L8c64e8c4;goto L8c64e870;
L8c64e870: *stop=0x8c64e870u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64e872; /* mov.l r14,@-r15 */
L8c64e872: *stop=0x8c64e872u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64e874; /* mov.l r13,@-r15 */
L8c64e874: *stop=0x8c64e874u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64e876; /* mov.l r12,@-r15 */
L8c64e876: *stop=0x8c64e876u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64e878; /* mov.l r11,@-r15 */
L8c64e878: *stop=0x8c64e878u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64e87a; /* mov.l r10,@-r15 */
L8c64e87a: *stop=0x8c64e87au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64e87c; /* mov.l r9,@-r15 */
L8c64e87c: *stop=0x8c64e87cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64e87e; /* mov.l r8,@-r15 */
L8c64e87e: *stop=0x8c64e87eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64e880; /* sts.l PR,@-r15 */
L8c64e880: *stop=0x8c64e880u;r[15]+=4294967288u;goto L8c64e882; /* add #-0x8,r15 */
L8c64e882: *stop=0x8c64e882u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c64e884; /* mov.l r4,@(0x4,r15) */
L8c64e884: *stop=0x8c64e884u;r[4]=0u;goto L8c64e886; /* mov #0x0,r4 */
L8c64e886: *stop=0x8c64e886u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e934u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c64e888; /* mov.l 0x8c64e934,r8 */
L8c64e888: *stop=0x8c64e888u;r[9]=2u;goto L8c64e88a; /* mov #0x2,r9 */
L8c64e88a: *stop=0x8c64e88au;r[11]=1u;goto L8c64e88c; /* mov #0x1,r11 */
L8c64e88c: *stop=0x8c64e88cu;r[10]=r[5];goto L8c64e88e; /* mov r5,r10 */
L8c64e88e: *stop=0x8c64e88eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c64e890; /* mov.l r6,@r15 */
L8c64e890: *stop=0x8c64e890u;r[12]=r[4];goto L8c64e892; /* mov r4,r12 */
L8c64e892: *stop=0x8c64e894u;r[13]=r[4];goto L8c64e8c8; /* bra 0x8c64e8c8 */
L8c64e896: *stop=0x8c64e896u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e938u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e898; /* mov.l 0x8c64e938,r2 */
L8c64e898: *stop=0x8c64e898u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e89a; /* mov.l @(0x4,r15),r3 */
L8c64e89a: *stop=0x8c64e89au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[14]=loaded;}goto L8c64e89c; /* mov.l @r2,r14 */
L8c64e89c: *stop=0x8c64e89cu;r[14]+=r[13];goto L8c64e89e; /* add r13,r14 */
L8c64e89e: *stop=0x8c64e89eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e8a0; /* mov.l @r14,r1 */
L8c64e8a0: *stop=0x8c64e8a0u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c64e8a2; /* cmp/eq r3,r1 */
L8c64e8a2: if(!(*sr&1u))goto L8c64e8c4;goto L8c64e8a4; /* bf 0x8c64e8c4 */
L8c64e8a4: *stop=0x8c64e8a4u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e8a6; /* mov.l @(0x10,r14),r3 */
L8c64e8a6: *stop=0x8c64e8a6u;r[3]|=r[10];goto L8c64e8a8; /* or r10,r3 */
L8c64e8a8: *stop=0x8c64e8a8u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c64e8aa; /* mov.l r3,@(0x10,r14) */
L8c64e8aa: *stop=0x8c64e8aau;r[3]=r[10];goto L8c64e8ac; /* mov r10,r3 */
L8c64e8ac: *stop=0x8c64e8acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e8ae; /* mov.l @r15,r1 */
L8c64e8ae: *stop=0x8c64e8aeu;*sr=(*sr&~1u)|!(r[9]&r[3]);goto L8c64e8b0; /* tst r9,r3 */
L8c64e8b0: *stop=0x8c64e8b0u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[1],4);if(io_status)return io_status;goto L8c64e8b2; /* mov.l r1,@(0x14,r14) */
L8c64e8b2: if((*sr&1u))goto L8c64e8c4;goto L8c64e8b4; /* bt 0x8c64e8c4 */
L8c64e8b4: *stop=0x8c64e8b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e93cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e8b6; /* mov.l 0x8c64e93c,r3 */
L8c64e8b6: *stop=0x8c64e8b6u;r[0]=64u;goto L8c64e8b8; /* mov #0x40,r0 */
L8c64e8b8: *stop=0x8c64e8b8u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c64e8ba; /* mov.w r11,@(r0,r14) */
L8c64e8ba: target=r[3];*pr=0x8c64e8beu;*stop=0x8c64e8bcu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e8be; /* jsr @r3 */
L8c64e8be: *stop=0x8c64e8beu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64e8c0; /* mov.l @(0x4,r14),r5 */
L8c64e8c0: target=r[8];*pr=0x8c64e8c4u;*stop=0x8c64e8c2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e8c4; /* jsr @r8 */
L8c64e8c4: *stop=0x8c64e8c4u;r[13]+=68u;goto L8c64e8c6; /* add #0x44,r13 */
L8c64e8c6: *stop=0x8c64e8c6u;r[12]+=1u;goto L8c64e8c8; /* add #0x1,r12 */
L8c64e8c8: *stop=0x8c64e8c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e940u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e8ca; /* mov.l 0x8c64e940,r3 */
L8c64e8ca: *stop=0x8c64e8cau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e8cc; /* mov.l @r3,r2 */
L8c64e8cc: *stop=0x8c64e8ccu;*sr=(*sr&~1u)|(r[12]>=r[2]);goto L8c64e8ce; /* cmp/hs r2,r12 */
L8c64e8ce: if(!(*sr&1u))goto L8c64e896;goto L8c64e8d0; /* bf 0x8c64e896 */
L8c64e8d0: *stop=0x8c64e8d0u;r[15]+=8u;goto L8c64e8d2; /* add #0x8,r15 */
L8c64e8d2: *stop=0x8c64e8d2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64e8d4; /* lds.l @r15+,PR */
L8c64e8d4: *stop=0x8c64e8d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64e8d6; /* mov.l @r15+,r8 */
L8c64e8d6: *stop=0x8c64e8d6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64e8d8; /* mov.l @r15+,r9 */
L8c64e8d8: *stop=0x8c64e8d8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64e8da; /* mov.l @r15+,r10 */
L8c64e8da: *stop=0x8c64e8dau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64e8dc; /* mov.l @r15+,r11 */
L8c64e8dc: *stop=0x8c64e8dcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64e8de; /* mov.l @r15+,r12 */
L8c64e8de: *stop=0x8c64e8deu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64e8e0; /* mov.l @r15+,r13 */
L8c64e8e0: *stop=0x8c64e8e0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64e8e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c64e870(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64e870(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64e8be(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64e870(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e8beu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64e8c4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64e870(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e8c4u);}

SA_STAGE_EXPORT int sa_player_texture_load_8c64e8e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64e8e4;
L8c64e8e4: *stop=0x8c64e8e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e944u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e8e6; /* mov.l 0x8c64e944,r1 */
L8c64e8e6: *stop=0x8c64e8e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e938u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e8e8; /* mov.l 0x8c64e938,r3 */
L8c64e8e8: *stop=0x8c64e8e8u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64e8ea; /* mov.l @r1,r6 */
L8c64e8ea: *stop=0x8c64e8eau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64e8ec; /* mov.l @r3,r5 */
L8c64e8ec: *stop=0x8c64e8ecu;r[0]=r[6];goto L8c64e8ee; /* mov r6,r0 */
L8c64e8ee: *stop=0x8c64e8eeu;r[0]<<=2;goto L8c64e8f0; /* shll2 r0 */
L8c64e8f0: *stop=0x8c64e8f0u;r[0]<<=2;goto L8c64e8f2; /* shll2 r0 */
L8c64e8f2: *stop=0x8c64e8f2u;r[2]=r[6];goto L8c64e8f4; /* mov r6,r2 */
L8c64e8f4: *stop=0x8c64e8f4u;r[0]+=r[2];goto L8c64e8f6; /* add r2,r0 */
L8c64e8f6: *stop=0x8c64e8f6u;r[0]<<=2;goto L8c64e8f8; /* shll2 r0 */
L8c64e8f8: *stop=0x8c64e8f8u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e8fa; /* mov.l @(r0,r5),r0 */
L8c64e8fa: *stop=0x8c64e8fau;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c64e8fc; /* cmp/eq #-0x1,r0 */
L8c64e8fc: if(!(*sr&1u))goto L8c64e902;goto L8c64e8fe; /* bf 0x8c64e902 */
L8c64e8fe: *stop=0x8c64e900u;r[4]=r[6];goto L8c64e922; /* bra 0x8c64e922 */
L8c64e902: *stop=0x8c64e902u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e940u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64e904; /* mov.l 0x8c64e940,r6 */
L8c64e904: *stop=0x8c64e906u;r[4]=0u;goto L8c64e91c; /* bra 0x8c64e91c */
L8c64e908: *stop=0x8c64e908u;r[0]=r[4];goto L8c64e90a; /* mov r4,r0 */
L8c64e90a: *stop=0x8c64e90au;r[0]<<=2;goto L8c64e90c; /* shll2 r0 */
L8c64e90c: *stop=0x8c64e90cu;r[0]<<=2;goto L8c64e90e; /* shll2 r0 */
L8c64e90e: *stop=0x8c64e90eu;r[3]=r[4];goto L8c64e910; /* mov r4,r3 */
L8c64e910: *stop=0x8c64e910u;r[0]+=r[3];goto L8c64e912; /* add r3,r0 */
L8c64e912: *stop=0x8c64e912u;r[0]<<=2;goto L8c64e914; /* shll2 r0 */
L8c64e914: *stop=0x8c64e914u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e916; /* mov.l @(r0,r5),r0 */
L8c64e916: *stop=0x8c64e916u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c64e918; /* cmp/eq #-0x1,r0 */
L8c64e918: if((*sr&1u))goto L8c64e922;goto L8c64e91a; /* bt 0x8c64e922 */
L8c64e91a: *stop=0x8c64e91au;r[4]+=1u;goto L8c64e91c; /* add #0x1,r4 */
L8c64e91c: *stop=0x8c64e91cu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e91e; /* mov.l @r6,r2 */
L8c64e91e: *stop=0x8c64e91eu;*sr=(*sr&~1u)|(r[4]>=r[2]);goto L8c64e920; /* cmp/hs r2,r4 */
L8c64e920: if(!(*sr&1u))goto L8c64e908;goto L8c64e922; /* bf 0x8c64e908 */
L8c64e922: *stop=0x8c64e922u;r[0]=r[4];goto L8c64e924; /* mov r4,r0 */
L8c64e924: *stop=0x8c64e924u;r[0]<<=2;goto L8c64e926; /* shll2 r0 */
L8c64e926: *stop=0x8c64e926u;r[0]<<=2;goto L8c64e928; /* shll2 r0 */
L8c64e928: *stop=0x8c64e928u;r[3]=r[4];goto L8c64e92a; /* mov r4,r3 */
L8c64e92a: *stop=0x8c64e92au;r[0]+=r[3];goto L8c64e92c; /* add r3,r0 */
L8c64e92c: *stop=0x8c64e92cu;r[0]<<=2;goto L8c64e92e; /* shll2 r0 */
L8c64e92e: *stop=0x8c64e92eu;r[0]+=r[5];goto L8c64e930; /* add r5,r0 */
L8c64e930: *stop=0x8c64e930u;if(*pr!=entry_pr)return -3;*stop=0x8c64e932u;r[0]+=20u;return 0; /* rts */
}

static int sa_player_texture_load_run_8c64edfc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64ee08u)goto L8c64ee08;if(resume_pc==0x8c64ee16u)goto L8c64ee16;if(resume_pc==0x8c64ee24u)goto L8c64ee24;goto L8c64edfc;
L8c64edfc: *stop=0x8c64edfcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64edfe; /* mov.l r14,@-r15 */
L8c64edfe: *stop=0x8c64edfeu;r[14]=r[5];goto L8c64ee00; /* mov r5,r14 */
L8c64ee00: *stop=0x8c64ee00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ee02; /* mov.l r13,@-r15 */
L8c64ee02: *stop=0x8c64ee02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ee04; /* sts.l PR,@-r15 */
L8c64ee04: target=0x8c64ee8cu;*pr=0x8c64ee08u;*stop=0x8c64ee06u;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ee08; /* bsr 0x8c64ee8c */
L8c64ee08: *stop=0x8c64ee08u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64ee0a; /* cmp/pz r0 */
L8c64ee0a: take=!(*sr&1u);*stop=0x8c64ee0cu;r[13]=r[0];if(take)goto L8c64ee24;goto L8c64ee0e; /* bf/s 0x8c64ee24 */
L8c64ee0e: *stop=0x8c64ee0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee88u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ee10; /* mov.l 0x8c64ee88,r2 */
L8c64ee10: *stop=0x8c64ee10u;r[5]=r[14];goto L8c64ee12; /* mov r14,r5 */
L8c64ee12: target=r[2];*pr=0x8c64ee16u;*stop=0x8c64ee14u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ee16; /* jsr @r2 */
L8c64ee16: *stop=0x8c64ee16u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64ee18; /* cmp/pz r0 */
L8c64ee18: take=(*sr&1u);*stop=0x8c64ee1au;r[13]=r[0];if(take)goto L8c64ee28;goto L8c64ee1c; /* bt/s 0x8c64ee28 */
L8c64ee1c: *stop=0x8c64ee1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ee1e; /* mov.l 0x8c64ee74,r2 */
L8c64ee1e: *stop=0x8c64ee1eu;r[4]=r[14];goto L8c64ee20; /* mov r14,r4 */
L8c64ee20: target=r[2];*pr=0x8c64ee24u;*stop=0x8c64ee22u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ee24; /* jsr @r2 */
L8c64ee24: *stop=0x8c64ee26u;r[0]=r[13];goto L8c64ee2a; /* bra 0x8c64ee2a */
L8c64ee28: *stop=0x8c64ee28u;r[0]=1u;goto L8c64ee2a; /* mov #0x1,r0 */
L8c64ee2a: *stop=0x8c64ee2au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64ee2c; /* lds.l @r15+,PR */
L8c64ee2c: *stop=0x8c64ee2cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64ee2e; /* mov.l @r15+,r13 */
L8c64ee2e: *stop=0x8c64ee2eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64ee30u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c64edfc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64edfc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64ee08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64edfc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ee08u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64ee16(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64edfc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ee16u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64ee24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64edfc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ee24u);}

static int sa_player_texture_load_run_8c64ee32(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64ee3eu)goto L8c64ee3e;if(resume_pc==0x8c64ee4cu)goto L8c64ee4c;if(resume_pc==0x8c64ee5au)goto L8c64ee5a;goto L8c64ee32;
L8c64ee32: *stop=0x8c64ee32u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ee34; /* mov.l r14,@-r15 */
L8c64ee34: *stop=0x8c64ee34u;r[14]=r[5];goto L8c64ee36; /* mov r5,r14 */
L8c64ee36: *stop=0x8c64ee36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ee38; /* mov.l r13,@-r15 */
L8c64ee38: *stop=0x8c64ee38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ee3a; /* sts.l PR,@-r15 */
L8c64ee3a: target=0x8c64ee8cu;*pr=0x8c64ee3eu;*stop=0x8c64ee3cu;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ee3e; /* bsr 0x8c64ee8c */
L8c64ee3e: *stop=0x8c64ee3eu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64ee40; /* cmp/pz r0 */
L8c64ee40: take=!(*sr&1u);*stop=0x8c64ee42u;r[13]=r[0];if(take)goto L8c64ee5a;goto L8c64ee44; /* bf/s 0x8c64ee5a */
L8c64ee44: *stop=0x8c64ee44u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee88u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ee46; /* mov.l 0x8c64ee88,r2 */
L8c64ee46: *stop=0x8c64ee46u;r[5]=r[14];goto L8c64ee48; /* mov r14,r5 */
L8c64ee48: target=r[2];*pr=0x8c64ee4cu;*stop=0x8c64ee4au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ee4c; /* jsr @r2 */
L8c64ee4c: *stop=0x8c64ee4cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64ee4e; /* cmp/pz r0 */
L8c64ee4e: take=(*sr&1u);*stop=0x8c64ee50u;r[13]=r[0];if(take)goto L8c64ee5e;goto L8c64ee52; /* bt/s 0x8c64ee5e */
L8c64ee52: *stop=0x8c64ee52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ee54; /* mov.l 0x8c64ee74,r2 */
L8c64ee54: *stop=0x8c64ee54u;r[4]=r[14];goto L8c64ee56; /* mov r14,r4 */
L8c64ee56: target=r[2];*pr=0x8c64ee5au;*stop=0x8c64ee58u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ee5a; /* jsr @r2 */
L8c64ee5a: *stop=0x8c64ee5cu;r[0]=r[13];goto L8c64ee60; /* bra 0x8c64ee60 */
L8c64ee5e: *stop=0x8c64ee5eu;r[0]=1u;goto L8c64ee60; /* mov #0x1,r0 */
L8c64ee60: *stop=0x8c64ee60u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64ee62; /* lds.l @r15+,PR */
L8c64ee62: *stop=0x8c64ee62u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64ee64; /* mov.l @r15+,r13 */
L8c64ee64: *stop=0x8c64ee64u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64ee66u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c64ee32(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ee32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64ee3e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ee32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ee3eu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64ee4c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ee32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ee4cu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64ee5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ee32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ee5au);}

static int sa_player_texture_load_run_8c64ef64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64ef84u)goto L8c64ef84;if(resume_pc==0x8c64efa2u)goto L8c64efa2;if(resume_pc==0x8c64efbcu)goto L8c64efbc;goto L8c64ef64;
L8c64ef64: *stop=0x8c64ef64u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ef66; /* mov.l r14,@-r15 */
L8c64ef66: *stop=0x8c64ef66u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ef68; /* mov.l r13,@-r15 */
L8c64ef68: *stop=0x8c64ef68u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64ef6a; /* mov.l r12,@-r15 */
L8c64ef6a: *stop=0x8c64ef6au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ef6c; /* sts.l PR,@-r15 */
L8c64ef6c: *stop=0x8c64ef6cu;r[15]+=4294967292u;goto L8c64ef6e; /* add #-0x4,r15 */
L8c64ef6e: *stop=0x8c64ef6eu;r[14]=r[5];goto L8c64ef70; /* mov r5,r14 */
L8c64ef70: *stop=0x8c64ef70u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64ef72; /* mov.l @(0x4,r14),r4 */
L8c64ef72: *stop=0x8c64ef72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f120u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ef74; /* mov.l 0x8c64f120,r2 */
L8c64ef74: *stop=0x8c64ef74u;r[7]=r[4];goto L8c64ef76; /* mov r4,r7 */
L8c64ef76: *stop=0x8c64ef76u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ef78; /* mov.l @(0x10,r14),r5 */
L8c64ef78: *stop=0x8c64ef78u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64ef7a; /* mov.l @(0x14,r14),r6 */
L8c64ef7a: *stop=0x8c64ef7au;r[7]>>=16;goto L8c64ef7c; /* shlr16 r7 */
L8c64ef7c: *stop=0x8c64ef7cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c64ef7e; /* mov.l r4,@r15 */
L8c64ef7e: *stop=0x8c64ef7eu;r[4]=r[14];goto L8c64ef80; /* mov r14,r4 */
L8c64ef80: target=r[2];*pr=0x8c64ef84u;*stop=0x8c64ef82u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ef84; /* jsr @r2 */
L8c64ef84: *stop=0x8c64ef84u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64ef86; /* cmp/eq #0x1,r0 */
L8c64ef86: if((*sr&1u))goto L8c64ef92;goto L8c64ef88; /* bt 0x8c64ef92 */
L8c64ef88: *stop=0x8c64ef88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f118u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ef8a; /* mov.l 0x8c64f118,r3 */
L8c64ef8a: *stop=0x8c64ef8au;r[2]=4u;goto L8c64ef8c; /* mov #0x4,r2 */
L8c64ef8c: *stop=0x8c64ef8cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64ef8e; /* mov.l r2,@r3 */
L8c64ef8e: *stop=0x8c64ef90u;r[0]=4294967292u;goto L8c64efc0; /* bra 0x8c64efc0 */
L8c64ef92: *stop=0x8c64ef92u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ef94; /* mov.l @(0x4,r14),r2 */
L8c64ef94: *stop=0x8c64ef94u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ef96; /* mov.l @r15,r3 */
L8c64ef96: *stop=0x8c64ef96u;r[2]|=r[3];goto L8c64ef98; /* or r3,r2 */
L8c64ef98: *stop=0x8c64ef98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ef9a; /* mov.l 0x8c64f124,r3 */
L8c64ef9a: *stop=0x8c64ef9au;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c64ef9c; /* mov.l r2,@(0x4,r14) */
L8c64ef9c: *stop=0x8c64ef9cu;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64ef9e; /* mov.l @(0x18,r14),r12 */
L8c64ef9e: target=r[3];*pr=0x8c64efa2u;*stop=0x8c64efa0u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64efa2; /* jsr @r3 */
L8c64efa2: *stop=0x8c64efa2u;r[13]=r[0];goto L8c64efa4; /* mov r0,r13 */
L8c64efa4: *stop=0x8c64efa4u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c64efa6; /* tst r13,r13 */
L8c64efa6: if(!(*sr&1u))goto L8c64efb2;goto L8c64efa8; /* bf 0x8c64efb2 */
L8c64efa8: *stop=0x8c64efa8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f118u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64efaa; /* mov.l 0x8c64f118,r2 */
L8c64efaa: *stop=0x8c64efaau;r[3]=7u;goto L8c64efac; /* mov #0x7,r3 */
L8c64efac: *stop=0x8c64efacu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64efae; /* mov.l r3,@r2 */
L8c64efae: *stop=0x8c64efb0u;r[0]=4294967289u;goto L8c64efc0; /* bra 0x8c64efc0 */
L8c64efb2: *stop=0x8c64efb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f128u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64efb4; /* mov.l 0x8c64f128,r3 */
L8c64efb4: *stop=0x8c64efb4u;r[6]=r[12];goto L8c64efb6; /* mov r12,r6 */
L8c64efb6: *stop=0x8c64efb6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64efb8; /* mov.l @r14,r5 */
L8c64efb8: target=r[3];*pr=0x8c64efbcu;*stop=0x8c64efbau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64efbc; /* jsr @r3 */
L8c64efbc: *stop=0x8c64efbcu;r[0]=r[13];goto L8c64efbe; /* mov r13,r0 */
L8c64efbe: *stop=0x8c64efbeu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[13],4);if(io_status)return io_status;goto L8c64efc0; /* mov.l r13,@r14 */
L8c64efc0: *stop=0x8c64efc0u;r[15]+=4u;goto L8c64efc2; /* add #0x4,r15 */
L8c64efc2: *stop=0x8c64efc2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64efc4; /* lds.l @r15+,PR */
L8c64efc4: *stop=0x8c64efc4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64efc6; /* mov.l @r15+,r12 */
L8c64efc6: *stop=0x8c64efc6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64efc8; /* mov.l @r15+,r13 */
L8c64efc8: *stop=0x8c64efc8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64efcau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c64ef64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ef64(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64ef84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ef64(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ef84u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64efa2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ef64(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64efa2u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64efbc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ef64(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64efbcu);}

static int sa_player_texture_load_run_8c098ee0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c098f3cu)goto L8c098f3c;if(resume_pc==0x8c098f7au)goto L8c098f7a;goto L8c098ee0;
L8c098ee0: *stop=0x8c098ee0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c098ee2; /* mov.l r14,@-r15 */
L8c098ee2: *stop=0x8c098ee2u;r[0]=4294967294u;goto L8c098ee4; /* mov #-0x2,r0 */
L8c098ee4: *stop=0x8c098ee4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c098ee6; /* mov.l r13,@-r15 */
L8c098ee6: *stop=0x8c098ee6u;r[0]<<=8;goto L8c098ee8; /* shll8 r0 */
L8c098ee8: *stop=0x8c098ee8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c098eea; /* mov.l r12,@-r15 */
L8c098eea: *stop=0x8c098eeau;r[0]+=4294967284u;goto L8c098eec; /* add #-0xc,r0 */
L8c098eec: *stop=0x8c098eecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c098eee; /* mov.l r11,@-r15 */
L8c098eee: *stop=0x8c098eeeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c098ef0; /* mov.l r10,@-r15 */
L8c098ef0: *stop=0x8c098ef0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c098ef2; /* mov.l r9,@-r15 */
L8c098ef2: *stop=0x8c098ef2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c098ef4; /* mov.l r8,@-r15 */
L8c098ef4: *stop=0x8c098ef4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c098ef6; /* sts.l PR,@-r15 */
L8c098ef6: *stop=0x8c098ef6u;r[15]+=r[0];goto L8c098ef8; /* add r0,r15 */
L8c098ef8: *stop=0x8c098ef8u;r[3]=r[4];goto L8c098efa; /* mov r4,r3 */
L8c098efa: *stop=0x8c098efau;r[13]=0u;goto L8c098efc; /* mov #0x0,r13 */
L8c098efc: *stop=0x8c098efcu;r[9]=r[15];goto L8c098efe; /* mov r15,r9 */
L8c098efe: *stop=0x8c098efeu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c098f00; /* mov.l r4,@r15 */
L8c098f00: *stop=0x8c098f00u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c098f02; /* mov.l @r3,r2 */
L8c098f02: *stop=0x8c098f02u;r[8]=r[13];goto L8c098f04; /* mov r13,r8 */
L8c098f04: *stop=0x8c098f04u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c098f06; /* mov.l r2,@(0x8,r15) */
L8c098f06: *stop=0x8c098f06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c098f08; /* mov.l @r15,r3 */
L8c098f08: *stop=0x8c098f08u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c098f0a; /* mov.l @(0x4,r3),r2 */
L8c098f0a: *stop=0x8c098f0au;r[3]=r[2];goto L8c098f0c; /* mov r2,r3 */
L8c098f0c: *stop=0x8c098f0cu;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c098f0e; /* cmp/pl r3 */
L8c098f0e: *stop=0x8c098f0eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c098f10; /* mov.l r2,@(0x4,r15) */
L8c098f10: take=!(*sr&1u);*stop=0x8c098f12u;r[9]+=12u;if(take)goto L8c098f74;goto L8c098f14; /* bf/s 0x8c098f74 */
L8c098f14: *stop=0x8c098f14u;r[14]=r[8];goto L8c098f16; /* mov r8,r14 */
L8c098f16: *stop=0x8c098f16u;r[3]=r[8];goto L8c098f18; /* mov r8,r3 */
L8c098f18: *stop=0x8c098f18u;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c098f1a; /* shll r14 */
L8c098f1a: *stop=0x8c098f1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c099060u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c098f1c; /* mov.l 0x8c099060,r0 */
L8c098f1c: *stop=0x8c098f1cu;r[14]+=r[3];goto L8c098f1e; /* add r3,r14 */
L8c098f1e: *stop=0x8c098f1eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c098f20; /* mov.l @(0x8,r15),r2 */
L8c098f20: *stop=0x8c098f20u;r[14]<<=2;goto L8c098f22; /* shll2 r14 */
L8c098f22: *stop=0x8c098f22u;r[12]=r[0];goto L8c098f24; /* mov r0,r12 */
L8c098f24: *stop=0x8c098f24u;r[14]+=r[2];goto L8c098f26; /* add r2,r14 */
L8c098f26: *stop=0x8c098f26u;r[11]=r[0];goto L8c098f28; /* mov r0,r11 */
L8c098f28: *stop=0x8c098f28u;r[10]=0u;goto L8c098f2a; /* mov #0x0,r10 */
L8c098f2a: *stop=0x8c098f2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c099064u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c098f2c; /* mov.l 0x8c099064,r2 */
L8c098f2c: *stop=0x8c098f2cu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[1]=loaded;}goto L8c098f2e; /* mov.l @r12,r1 */
L8c098f2e: *stop=0x8c098f2eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c098f30; /* mov.l @r2,r3 */
L8c098f30: *stop=0x8c098f30u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c098f32; /* cmp/eq r3,r1 */
L8c098f32: if(!(*sr&1u))goto L8c098f5a;goto L8c098f34; /* bf 0x8c098f5a */
L8c098f34: *stop=0x8c098f34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c099068u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c098f36; /* mov.l 0x8c099068,r3 */
L8c098f36: *stop=0x8c098f36u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c098f38; /* mov.l @r14,r5 */
L8c098f38: target=r[3];*pr=0x8c098f3cu;*stop=0x8c098f3au;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c098f3c; /* jsr @r3 */
L8c098f3c: *stop=0x8c098f3cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c098f3e; /* tst r0,r0 */
L8c098f3e: if(!(*sr&1u))goto L8c098f5a;goto L8c098f40; /* bf 0x8c098f5a */
L8c098f40: *stop=0x8c098f40u;r[3]=67u;goto L8c098f42; /* mov #0x43,r3 */
L8c098f42: *stop=0x8c098f42u;r[0]=r[13];goto L8c098f44; /* mov r13,r0 */
L8c098f44: *stop=0x8c098f44u;r[3]<<=2;goto L8c098f46; /* shll2 r3 */
L8c098f46: *stop=0x8c098f46u;r[3]+=r[15];goto L8c098f48; /* add r15,r3 */
L8c098f48: *stop=0x8c098f48u;r[0]<<=2;goto L8c098f4a; /* shll2 r0 */
L8c098f4a: *stop=0x8c098f4au;r[3]+=r[0];goto L8c098f4c; /* add r0,r3 */
L8c098f4c: *stop=0x8c098f4cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c098f4e; /* mov.l r14,@r3 */
L8c098f4e: *stop=0x8c098f4eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c098f50; /* mov.l @r14,r3 */
L8c098f50: *stop=0x8c098f50u;if(!valid_access(r[0]+r[9],4u))return -3;wr(m,bus,&io_status,r[9]+r[0],r[3],4);if(io_status)return io_status;goto L8c098f52; /* mov.l r3,@(r0,r9) */
L8c098f52: *stop=0x8c098f52u;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c098f54; /* mov.l @(0x8,r11),r2 */
L8c098f54: *stop=0x8c098f54u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c098f56; /* mov.l r2,@r14 */
L8c098f56: *stop=0x8c098f58u;r[13]+=1u;goto L8c098f66; /* bra 0x8c098f66 */
L8c098f5a: *stop=0x8c098f5au;r[2]=1u;goto L8c098f5c; /* mov #0x1,r2 */
L8c098f5c: *stop=0x8c098f5cu;r[10]+=1u;goto L8c098f5e; /* add #0x1,r10 */
L8c098f5e: *stop=0x8c098f5eu;*sr=(*sr&~1u)|(r[10]>=r[2]);goto L8c098f60; /* cmp/hs r2,r10 */
L8c098f60: *stop=0x8c098f60u;r[11]+=12u;goto L8c098f62; /* add #0xc,r11 */
L8c098f62: take=!(*sr&1u);*stop=0x8c098f64u;r[12]+=12u;if(take)goto L8c098f2a;goto L8c098f66; /* bf/s 0x8c098f2a */
L8c098f66: *stop=0x8c098f66u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c098f68; /* mov.l @(0x4,r15),r2 */
L8c098f68: *stop=0x8c098f68u;r[8]+=1u;goto L8c098f6a; /* add #0x1,r8 */
L8c098f6a: *stop=0x8c098f6au;*sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[2]);goto L8c098f6c; /* cmp/ge r2,r8 */
L8c098f6c: if((*sr&1u))goto L8c098f74;goto L8c098f6e; /* bt 0x8c098f74 */
L8c098f6e: *stop=0x8c098f6eu;r[1]=63u;goto L8c098f70; /* mov #0x3f,r1 */
L8c098f70: *stop=0x8c098f70u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[1]);goto L8c098f72; /* cmp/ge r1,r13 */
L8c098f72: if(!(*sr&1u))goto L8c098f14;goto L8c098f74; /* bf 0x8c098f14 */
L8c098f74: *stop=0x8c098f74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09906cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c098f76; /* mov.l 0x8c09906c,r3 */
L8c098f76: target=r[3];*pr=0x8c098f7au;*stop=0x8c098f78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c098f7a; /* jsr @r3 */
L8c098f7a: *stop=0x8c098f7cu;goto L8c098f8e; /* bra 0x8c098f8e */
L8c098f7e: *stop=0x8c098f7eu;r[3]=67u;goto L8c098f80; /* mov #0x43,r3 */
L8c098f80: *stop=0x8c098f80u;r[0]=r[13];goto L8c098f82; /* mov r13,r0 */
L8c098f82: *stop=0x8c098f82u;r[3]<<=2;goto L8c098f84; /* shll2 r3 */
L8c098f84: *stop=0x8c098f84u;r[3]+=r[15];goto L8c098f86; /* add r15,r3 */
L8c098f86: *stop=0x8c098f86u;r[0]<<=2;goto L8c098f88; /* shll2 r0 */
L8c098f88: *stop=0x8c098f88u;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c098f8a; /* mov.l @(r0,r9),r2 */
L8c098f8a: *stop=0x8c098f8au;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c098f8c; /* mov.l @(r0,r3),r3 */
L8c098f8c: *stop=0x8c098f8cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c098f8e; /* mov.l r2,@r3 */
L8c098f8e: *stop=0x8c098f8eu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c098f90; /* tst r13,r13 */
L8c098f90: take=!(*sr&1u);*stop=0x8c098f92u;r[13]+=4294967295u;if(take)goto L8c098f7e;goto L8c098f94; /* bf/s 0x8c098f7e */
L8c098f94: *stop=0x8c098f94u;r[1]=2u;goto L8c098f96; /* mov #0x2,r1 */
L8c098f96: *stop=0x8c098f96u;r[1]<<=8;goto L8c098f98; /* shll8 r1 */
L8c098f98: *stop=0x8c098f98u;r[1]+=12u;goto L8c098f9a; /* add #0xc,r1 */
L8c098f9a: *stop=0x8c098f9au;r[15]+=r[1];goto L8c098f9c; /* add r1,r15 */
L8c098f9c: *stop=0x8c098f9cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c098f9e; /* lds.l @r15+,PR */
L8c098f9e: *stop=0x8c098f9eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c098fa0; /* mov.l @r15+,r8 */
L8c098fa0: *stop=0x8c098fa0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c098fa2; /* mov.l @r15+,r9 */
L8c098fa2: *stop=0x8c098fa2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c098fa4; /* mov.l @r15+,r10 */
L8c098fa4: *stop=0x8c098fa4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c098fa6; /* mov.l @r15+,r11 */
L8c098fa6: *stop=0x8c098fa6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c098fa8; /* mov.l @r15+,r12 */
L8c098fa8: *stop=0x8c098fa8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c098faa; /* mov.l @r15+,r13 */
L8c098faa: *stop=0x8c098faau;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c098facu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c098ee0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c098ee0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c098f3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c098ee0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c098f3cu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c098f7a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c098ee0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c098f7au);}

SA_STAGE_EXPORT int sa_player_texture_load_8c606f66(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c606f66;
L8c606f66: *stop=0x8c606f66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60705cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c606f68; /* mov.l 0x8c60705c,r3 */
L8c606f68: target=r[3];*stop=0x8c606f6au;r[6]=2u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c606f6c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c606f6c;
L8c606f6c: *stop=0x8c606f6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60705cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c606f6e; /* mov.l 0x8c60705c,r3 */
L8c606f6e: target=r[3];*stop=0x8c606f70u;r[6]=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c64da90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64da90;
L8c64da90: *stop=0x8c64da90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64db90u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64da92; /* mov.l 0x8c64db90,r2 */
L8c64da92: *stop=0x8c64da92u;r[0]=104u;goto L8c64da94; /* mov #0x68,r0 */
L8c64da94: *stop=0x8c64da94u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64da96; /* mov.l @r2,r3 */
L8c64da96: *stop=0x8c64da96u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[4],4);if(io_status)return io_status;goto L8c64da98; /* mov.l r4,@(r0,r3) */
L8c64da98: *stop=0x8c64da98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64db94u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64da9a; /* mov.l 0x8c64db94,r3 */
L8c64da9a: target=r[3];*stop=0x8c64da9cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c64db42(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64db42;
L8c64db42: *stop=0x8c64db42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64dbb0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64db44; /* mov.l 0x8c64dbb0,r3 */
L8c64db44: *stop=0x8c64db44u;r[0]=r[4];goto L8c64db46; /* mov r4,r0 */
L8c64db46: *stop=0x8c64db46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64dbb4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64db48; /* mov.l 0x8c64dbb4,r6 */
L8c64db48: *stop=0x8c64db48u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64db4a; /* cmp/eq #0x0,r0 */
L8c64db4a: *stop=0x8c64db4au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c64db4c; /* mov.l r4,@r3 */
L8c64db4c: take=(*sr&1u);*stop=0x8c64db4eu;r[5]=0u;if(take)goto L8c64db60;goto L8c64db50; /* bt/s 0x8c64db60 */
L8c64db50: *stop=0x8c64db50u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64db52; /* cmp/eq #0x1,r0 */
L8c64db52: if((*sr&1u))goto L8c64db64;goto L8c64db54; /* bt 0x8c64db64 */
L8c64db54: *stop=0x8c64db54u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c64db56; /* cmp/eq #0x2,r0 */
L8c64db56: if((*sr&1u))goto L8c64db68;goto L8c64db58; /* bt 0x8c64db68 */
L8c64db58: *stop=0x8c64db58u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c64db5a; /* cmp/eq #0x6,r0 */
L8c64db5a: if((*sr&1u))goto L8c64db6c;goto L8c64db5c; /* bt 0x8c64db6c */
L8c64db5c: *stop=0x8c64db5eu;goto L8c64db74; /* bra 0x8c64db74 */
L8c64db60: *stop=0x8c64db62u;r[4]=r[5];goto L8c64db76; /* bra 0x8c64db76 */
L8c64db64: *stop=0x8c64db66u;r[4]=1u;goto L8c64db76; /* bra 0x8c64db76 */
L8c64db68: *stop=0x8c64db6au;r[4]=2u;goto L8c64db76; /* bra 0x8c64db76 */
L8c64db6c: *stop=0x8c64db6cu;r[1]=32u;goto L8c64db6e; /* mov #0x20,r1 */
L8c64db6e: *stop=0x8c64db6eu;r[4]=3u;goto L8c64db70; /* mov #0x3,r4 */
L8c64db70: *stop=0x8c64db72u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[1],4);if(io_status)return io_status;goto L8c64db7a; /* bra 0x8c64db7a */
L8c64db74: *stop=0x8c64db74u;r[4]=r[5];goto L8c64db76; /* mov r5,r4 */
L8c64db76: *stop=0x8c64db76u;r[3]=16u;goto L8c64db78; /* mov #0x10,r3 */
L8c64db78: *stop=0x8c64db78u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c64db7a; /* mov.l r3,@r6 */
L8c64db7a: *stop=0x8c64db7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64dbb8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64db7c; /* mov.l 0x8c64dbb8,r3 */
L8c64db7c: target=r[3];*stop=0x8c64db7eu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c64dbbc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64dbbc;
L8c64dbbc: *stop=0x8c64dbbcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64dbbe; /* mov.l r14,@-r15 */
L8c64dbbe: *stop=0x8c64dbbeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64dbc0; /* mov.l r13,@-r15 */
L8c64dbc0: *stop=0x8c64dbc0u;r[15]+=4294967288u;goto L8c64dbc2; /* add #-0x8,r15 */
L8c64dbc2: *stop=0x8c64dbc2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64dcf8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64dbc4; /* mov.l 0x8c64dcf8,r3 */
L8c64dbc4: *stop=0x8c64dbc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64dcf4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64dbc6; /* mov.l 0x8c64dcf4,r14 */
L8c64dbc6: *stop=0x8c64dbc6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64dbc8; /* mov.l r5,@r15 */
L8c64dbc8: *stop=0x8c64dbc8u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c64dbca; /* mov.l r6,@(0x4,r15) */
L8c64dbca: *stop=0x8c64dbcau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64dbcc; /* mov.l @r3,r0 */
L8c64dbcc: *stop=0x8c64dbccu;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c64dbce; /* cmp/eq #0x10,r0 */
L8c64dbce: take=!(*sr&1u);*stop=0x8c64dbd0u;r[13]=r[4];if(take)goto L8c64dbfc;goto L8c64dbd2; /* bf/s 0x8c64dbfc */
L8c64dbd2: *stop=0x8c64dbd2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64dbd4; /* mov.l @r15,r6 */
L8c64dbd4: *stop=0x8c64dbd4u;r[4]=r[13];goto L8c64dbd6; /* mov r13,r4 */
L8c64dbd6: *stop=0x8c64dbd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64dcfcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64dbd8; /* mov.l 0x8c64dcfc,r2 */
L8c64dbd8: *stop=0x8c64dbd8u;r[5]=r[13];goto L8c64dbda; /* mov r13,r5 */
L8c64dbda: *stop=0x8c64dbdau;r[6]+=r[13];goto L8c64dbdc; /* add r13,r6 */
L8c64dbdc: *stop=0x8c64dbdcu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64dbde; /* mov.l @(0x4,r15),r7 */
L8c64dbde: *stop=0x8c64dbdeu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[6]);goto L8c64dbe0; /* cmp/ge r6,r4 */
L8c64dbe0: *stop=0x8c64dbe0u;r[5]<<=2;goto L8c64dbe2; /* shll2 r5 */
L8c64dbe2: take=(*sr&1u);*stop=0x8c64dbe4u;r[5]+=r[2];if(take)goto L8c64dc14;goto L8c64dbe6; /* bt/s 0x8c64dc14 */
L8c64dbe6: *stop=0x8c64dbe6u;if(!valid_access(r[7],2u))return -3;r[2]=sx(m,bus,&io_status,r[7],2);if(io_status)return io_status;r[7]+=2;goto L8c64dbe8; /* mov.w @r7+,r2 */
L8c64dbe8: *stop=0x8c64dbe8u;r[3]=r[5];goto L8c64dbea; /* mov r5,r3 */
L8c64dbea: *stop=0x8c64dbeau;r[4]+=1u;goto L8c64dbec; /* add #0x1,r4 */
L8c64dbec: *stop=0x8c64dbecu;r[2]=r[2]&65535u;goto L8c64dbee; /* extu.w r2,r2 */
L8c64dbee: *stop=0x8c64dbeeu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[6]);goto L8c64dbf0; /* cmp/ge r6,r4 */
L8c64dbf0: *stop=0x8c64dbf0u;r[3]|=r[14];goto L8c64dbf2; /* or r14,r3 */
L8c64dbf2: *stop=0x8c64dbf2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64dbf4; /* mov.l r2,@r3 */
L8c64dbf4: take=!(*sr&1u);*stop=0x8c64dbf6u;r[5]+=4u;if(take)goto L8c64dbe6;goto L8c64dbf8; /* bf/s 0x8c64dbe6 */
L8c64dbf8: *stop=0x8c64dbfau;goto L8c64dc14; /* bra 0x8c64dc14 */
L8c64dbfc: *stop=0x8c64dbfcu;r[4]=r[13];goto L8c64dbfe; /* mov r13,r4 */
L8c64dbfe: *stop=0x8c64dbfeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64dcfcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64dc00; /* mov.l 0x8c64dcfc,r2 */
L8c64dc00: *stop=0x8c64dc00u;r[4]<<=2;goto L8c64dc02; /* shll2 r4 */
L8c64dc02: *stop=0x8c64dc02u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64dc04; /* mov.l @r15,r6 */
L8c64dc04: *stop=0x8c64dc04u;r[4]+=r[2];goto L8c64dc06; /* add r2,r4 */
L8c64dc06: *stop=0x8c64dc06u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64dc08; /* mov.l @(0x4,r15),r5 */
L8c64dc08: *stop=0x8c64dc08u;r[4]|=r[14];goto L8c64dc0a; /* or r14,r4 */
L8c64dc0a: *stop=0x8c64dc0au;r[15]+=8u;goto L8c64dc0c; /* add #0x8,r15 */
L8c64dc0c: *stop=0x8c64dc0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64dd00u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64dc0e; /* mov.l 0x8c64dd00,r1 */
L8c64dc0e: *stop=0x8c64dc0eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64dc10; /* mov.l @r15+,r13 */
L8c64dc10: target=r[1];*stop=0x8c64dc12u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r1 */
L8c64dc14: *stop=0x8c64dc14u;r[15]+=8u;goto L8c64dc16; /* add #0x8,r15 */
L8c64dc16: *stop=0x8c64dc16u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64dc18; /* mov.l @r15+,r13 */
L8c64dc18: *stop=0x8c64dc18u;if(*pr!=entry_pr)return -3;*stop=0x8c64dc1au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c64df10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64df10;
L8c64df10: *stop=0x8c64df10u;r[0]=r[4];goto L8c64df12; /* mov r4,r0 */
L8c64df12: *stop=0x8c64df12u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c64df14; /* cmp/eq #0x8,r0 */
L8c64df14: if(!(*sr&1u))goto L8c64df1a;goto L8c64df16; /* bf 0x8c64df1a */
L8c64df16: *stop=0x8c64df18u;r[4]=0u;goto L8c64df5a; /* bra 0x8c64df5a */
L8c64df1a: *stop=0x8c64df1au;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c64df1c; /* cmp/eq #0x10,r0 */
L8c64df1c: if(!(*sr&1u))goto L8c64df22;goto L8c64df1e; /* bf 0x8c64df22 */
L8c64df1e: *stop=0x8c64df20u;r[4]=1u;goto L8c64df5a; /* bra 0x8c64df5a */
L8c64df22: *stop=0x8c64df22u;r[0]=r[4];goto L8c64df24; /* mov r4,r0 */
L8c64df24: *stop=0x8c64df24u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c64df26; /* cmp/eq #0x20,r0 */
L8c64df26: if(!(*sr&1u))goto L8c64df2c;goto L8c64df28; /* bf 0x8c64df2c */
L8c64df28: *stop=0x8c64df2au;r[4]=2u;goto L8c64df5a; /* bra 0x8c64df5a */
L8c64df2c: *stop=0x8c64df2cu;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c64df2e; /* cmp/eq #0x40,r0 */
L8c64df2e: if(!(*sr&1u))goto L8c64df34;goto L8c64df30; /* bf 0x8c64df34 */
L8c64df30: *stop=0x8c64df32u;r[4]=3u;goto L8c64df5a; /* bra 0x8c64df5a */
L8c64df34: *stop=0x8c64df34u;r[3]=sx(m,bus,&io_status,0x8c64df5eu,2);if(io_status)return io_status;goto L8c64df36; /* mov.w 0x8c64df5e,r3 */
L8c64df36: *stop=0x8c64df36u;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c64df38; /* cmp/eq r3,r4 */
L8c64df38: if(!(*sr&1u))goto L8c64df3e;goto L8c64df3a; /* bf 0x8c64df3e */
L8c64df3a: *stop=0x8c64df3cu;r[4]=4u;goto L8c64df5a; /* bra 0x8c64df5a */
L8c64df3e: *stop=0x8c64df3eu;r[2]=sx(m,bus,&io_status,0x8c64df60u,2);if(io_status)return io_status;goto L8c64df40; /* mov.w 0x8c64df60,r2 */
L8c64df40: *stop=0x8c64df40u;*sr=(*sr&~1u)|(r[2]==r[4]);goto L8c64df42; /* cmp/eq r2,r4 */
L8c64df42: if(!(*sr&1u))goto L8c64df48;goto L8c64df44; /* bf 0x8c64df48 */
L8c64df44: *stop=0x8c64df46u;r[4]=5u;goto L8c64df5a; /* bra 0x8c64df5a */
L8c64df48: *stop=0x8c64df48u;r[1]=sx(m,bus,&io_status,0x8c64df62u,2);if(io_status)return io_status;goto L8c64df4a; /* mov.w 0x8c64df62,r1 */
L8c64df4a: *stop=0x8c64df4au;*sr=(*sr&~1u)|(r[1]==r[4]);goto L8c64df4c; /* cmp/eq r1,r4 */
L8c64df4c: if(!(*sr&1u))goto L8c64df52;goto L8c64df4e; /* bf 0x8c64df52 */
L8c64df4e: *stop=0x8c64df50u;r[4]=6u;goto L8c64df5a; /* bra 0x8c64df5a */
L8c64df52: *stop=0x8c64df52u;r[3]=sx(m,bus,&io_status,0x8c64df64u,2);if(io_status)return io_status;goto L8c64df54; /* mov.w 0x8c64df64,r3 */
L8c64df54: *stop=0x8c64df54u;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c64df56; /* cmp/eq r3,r4 */
L8c64df56: if(!(*sr&1u))goto L8c64df5a;goto L8c64df58; /* bf 0x8c64df5a */
L8c64df58: *stop=0x8c64df58u;r[4]=7u;goto L8c64df5a; /* mov #0x7,r4 */
L8c64df5a: *stop=0x8c64df5au;if(*pr!=entry_pr)return -3;*stop=0x8c64df5cu;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c64e9b2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64e9b2;
L8c64e9b2: *stop=0x8c64e9b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64e9b4; /* mov.l r14,@-r15 */
L8c64e9b4: *stop=0x8c64e9b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64e9b6; /* mov.l r13,@-r15 */
L8c64e9b6: *stop=0x8c64e9b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64e9b8; /* mov.l r12,@-r15 */
L8c64e9b8: *stop=0x8c64e9b8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64e9ba; /* mov.l r11,@-r15 */
L8c64e9ba: *stop=0x8c64e9bau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64e9bc; /* mov.l r10,@-r15 */
L8c64e9bc: *stop=0x8c64e9bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64e9be; /* mov.l r9,@-r15 */
L8c64e9be: *stop=0x8c64e9beu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64e9c0; /* mov.l r8,@-r15 */
L8c64e9c0: *stop=0x8c64e9c0u;r[15]+=4294967288u;goto L8c64e9c2; /* add #-0x8,r15 */
L8c64e9c2: *stop=0x8c64e9c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ea88u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e9c4; /* mov.l 0x8c64ea88,r3 */
L8c64e9c4: *stop=0x8c64e9c4u;r[2]=r[7]&255u;goto L8c64e9c6; /* extu.b r7,r2 */
L8c64e9c6: *stop=0x8c64e9c6u;r[1]=sx(m,bus,&io_status,0x8c64ea6eu,2);if(io_status)return io_status;goto L8c64e9c8; /* mov.w 0x8c64ea6e,r1 */
L8c64e9c8: *stop=0x8c64e9c8u;r[12]=8u;goto L8c64e9ca; /* mov #0x8,r12 */
L8c64e9ca: *stop=0x8c64e9cau;r[3]&=r[7];goto L8c64e9cc; /* and r7,r3 */
L8c64e9cc: *stop=0x8c64e9ccu;r[0]=r[3];goto L8c64e9ce; /* mov r3,r0 */
L8c64e9ce: *stop=0x8c64e9ceu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64e9d0; /* cmp/eq r1,r0 */
L8c64e9d0: *stop=0x8c64e9d0u;r[9]=4u;goto L8c64e9d2; /* mov #0x4,r9 */
L8c64e9d2: *stop=0x8c64e9d2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64e9d4; /* mov.l r3,@r15 */
L8c64e9d4: *stop=0x8c64e9d4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c64e9d6; /* mov.l r2,@(0x4,r15) */
L8c64e9d6: *stop=0x8c64e9d6u;r[8]=32u;goto L8c64e9d8; /* mov #0x20,r8 */
L8c64e9d8: *stop=0x8c64e9d8u;r[10]=0u;goto L8c64e9da; /* mov #0x0,r10 */
L8c64e9da: take=(*sr&1u);*stop=0x8c64e9dcu;r[13]=9u;if(take)goto L8c64ea2a;goto L8c64e9de; /* bt/s 0x8c64ea2a */
L8c64e9de: *stop=0x8c64e9deu;r[1]=sx(m,bus,&io_status,0x8c64ea70u,2);if(io_status)return io_status;goto L8c64e9e0; /* mov.w 0x8c64ea70,r1 */
L8c64e9e0: *stop=0x8c64e9e0u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64e9e2; /* cmp/eq r1,r0 */
L8c64e9e2: if((*sr&1u))goto L8c64ea2a;goto L8c64e9e4; /* bt 0x8c64ea2a */
L8c64e9e4: *stop=0x8c64e9e4u;r[1]=sx(m,bus,&io_status,0x8c64ea72u,2);if(io_status)return io_status;goto L8c64e9e6; /* mov.w 0x8c64ea72,r1 */
L8c64e9e6: *stop=0x8c64e9e6u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64e9e8; /* cmp/eq r1,r0 */
L8c64e9e8: if((*sr&1u))goto L8c64ea2e;goto L8c64e9ea; /* bt 0x8c64ea2e */
L8c64e9ea: *stop=0x8c64e9eau;r[1]=sx(m,bus,&io_status,0x8c64ea74u,2);if(io_status)return io_status;goto L8c64e9ec; /* mov.w 0x8c64ea74,r1 */
L8c64e9ec: *stop=0x8c64e9ecu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64e9ee; /* cmp/eq r1,r0 */
L8c64e9ee: if((*sr&1u))goto L8c64ea36;goto L8c64e9f0; /* bt 0x8c64ea36 */
L8c64e9f0: *stop=0x8c64e9f0u;r[1]=sx(m,bus,&io_status,0x8c64ea76u,2);if(io_status)return io_status;goto L8c64e9f2; /* mov.w 0x8c64ea76,r1 */
L8c64e9f2: *stop=0x8c64e9f2u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64e9f4; /* cmp/eq r1,r0 */
L8c64e9f4: if((*sr&1u))goto L8c64ea3c;goto L8c64e9f6; /* bt 0x8c64ea3c */
L8c64e9f6: *stop=0x8c64e9f6u;r[1]=sx(m,bus,&io_status,0x8c64ea78u,2);if(io_status)return io_status;goto L8c64e9f8; /* mov.w 0x8c64ea78,r1 */
L8c64e9f8: *stop=0x8c64e9f8u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64e9fa; /* cmp/eq r1,r0 */
L8c64e9fa: if((*sr&1u))goto L8c64ea42;goto L8c64e9fc; /* bt 0x8c64ea42 */
L8c64e9fc: *stop=0x8c64e9fcu;r[1]=sx(m,bus,&io_status,0x8c64ea7au,2);if(io_status)return io_status;goto L8c64e9fe; /* mov.w 0x8c64ea7a,r1 */
L8c64e9fe: *stop=0x8c64e9feu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64ea00; /* cmp/eq r1,r0 */
L8c64ea00: if((*sr&1u))goto L8c64ea48;goto L8c64ea02; /* bt 0x8c64ea48 */
L8c64ea02: *stop=0x8c64ea02u;r[1]=sx(m,bus,&io_status,0x8c64ea7cu,2);if(io_status)return io_status;goto L8c64ea04; /* mov.w 0x8c64ea7c,r1 */
L8c64ea04: *stop=0x8c64ea04u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64ea06; /* cmp/eq r1,r0 */
L8c64ea06: if((*sr&1u))goto L8c64ea4e;goto L8c64ea08; /* bt 0x8c64ea4e */
L8c64ea08: *stop=0x8c64ea08u;r[1]=sx(m,bus,&io_status,0x8c64ea7eu,2);if(io_status)return io_status;goto L8c64ea0a; /* mov.w 0x8c64ea7e,r1 */
L8c64ea0a: *stop=0x8c64ea0au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64ea0c; /* cmp/eq r1,r0 */
L8c64ea0c: if((*sr&1u))goto L8c64ea58;goto L8c64ea0e; /* bt 0x8c64ea58 */
L8c64ea0e: *stop=0x8c64ea0eu;r[1]=sx(m,bus,&io_status,0x8c64ea80u,2);if(io_status)return io_status;goto L8c64ea10; /* mov.w 0x8c64ea80,r1 */
L8c64ea10: *stop=0x8c64ea10u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64ea12; /* cmp/eq r1,r0 */
L8c64ea12: if((*sr&1u))goto L8c64ea64;goto L8c64ea14; /* bt 0x8c64ea64 */
L8c64ea14: *stop=0x8c64ea14u;r[1]=sx(m,bus,&io_status,0x8c64ea82u,2);if(io_status)return io_status;goto L8c64ea16; /* mov.w 0x8c64ea82,r1 */
L8c64ea16: *stop=0x8c64ea16u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64ea18; /* cmp/eq r1,r0 */
L8c64ea18: if((*sr&1u))goto L8c64ea94;goto L8c64ea1a; /* bt 0x8c64ea94 */
L8c64ea1a: *stop=0x8c64ea1au;r[1]=sx(m,bus,&io_status,0x8c64ea84u,2);if(io_status)return io_status;goto L8c64ea1c; /* mov.w 0x8c64ea84,r1 */
L8c64ea1c: *stop=0x8c64ea1cu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64ea1e; /* cmp/eq r1,r0 */
L8c64ea1e: if((*sr&1u))goto L8c64eaa0;goto L8c64ea20; /* bt 0x8c64eaa0 */
L8c64ea20: *stop=0x8c64ea20u;r[1]=sx(m,bus,&io_status,0x8c64ea86u,2);if(io_status)return io_status;goto L8c64ea22; /* mov.w 0x8c64ea86,r1 */
L8c64ea22: *stop=0x8c64ea22u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64ea24; /* cmp/eq r1,r0 */
L8c64ea24: if((*sr&1u))goto L8c64eaa4;goto L8c64ea26; /* bt 0x8c64eaa4 */
L8c64ea26: *stop=0x8c64ea28u;goto L8c64eb78; /* bra 0x8c64eb78 */
L8c64ea2a: *stop=0x8c64ea2cu;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[9],4);if(io_status)return io_status;goto L8c64eaa8; /* bra 0x8c64eaa8 */
L8c64ea2e: *stop=0x8c64ea2eu;r[2]=5u;goto L8c64ea30; /* mov #0x5,r2 */
L8c64ea30: *stop=0x8c64ea30u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[2],4);if(io_status)return io_status;goto L8c64ea32; /* mov.l r2,@(0x18,r4) */
L8c64ea32: *stop=0x8c64ea34u;r[14]=1u;goto L8c64eaaa; /* bra 0x8c64eaaa */
L8c64ea36: *stop=0x8c64ea36u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[12],4);if(io_status)return io_status;goto L8c64ea38; /* mov.l r12,@(0x18,r4) */
L8c64ea38: *stop=0x8c64ea3au;r[14]=2u;goto L8c64eaaa; /* bra 0x8c64eaaa */
L8c64ea3c: *stop=0x8c64ea3cu;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[12],4);if(io_status)return io_status;goto L8c64ea3e; /* mov.l r12,@(0x18,r4) */
L8c64ea3e: *stop=0x8c64ea40u;r[14]=3u;goto L8c64eaaa; /* bra 0x8c64eaaa */
L8c64ea42: *stop=0x8c64ea42u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[13],4);if(io_status)return io_status;goto L8c64ea44; /* mov.l r13,@(0x18,r4) */
L8c64ea44: *stop=0x8c64ea46u;r[14]=r[9];goto L8c64eaaa; /* bra 0x8c64eaaa */
L8c64ea48: *stop=0x8c64ea48u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[13],4);if(io_status)return io_status;goto L8c64ea4a; /* mov.l r13,@(0x18,r4) */
L8c64ea4a: *stop=0x8c64ea4cu;r[14]=5u;goto L8c64eaaa; /* bra 0x8c64eaaa */
L8c64ea4e: *stop=0x8c64ea4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ea8cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ea50; /* mov.l 0x8c64ea8c,r2 */
L8c64ea50: *stop=0x8c64ea50u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[8],4);if(io_status)return io_status;goto L8c64ea52; /* mov.l r8,@(0x18,r4) */
L8c64ea52: *stop=0x8c64ea52u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c64ea54; /* mov.l r2,@(0x8,r4) */
L8c64ea54: *stop=0x8c64ea56u;r[14]=6u;goto L8c64eaaa; /* bra 0x8c64eaaa */
L8c64ea58: *stop=0x8c64ea58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ea8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ea5a; /* mov.l 0x8c64ea8c,r3 */
L8c64ea5a: *stop=0x8c64ea5au;r[2]=33u;goto L8c64ea5c; /* mov #0x21,r2 */
L8c64ea5c: *stop=0x8c64ea5cu;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[2],4);if(io_status)return io_status;goto L8c64ea5e; /* mov.l r2,@(0x18,r4) */
L8c64ea5e: *stop=0x8c64ea5eu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[3],4);if(io_status)return io_status;goto L8c64ea60; /* mov.l r3,@(0x8,r4) */
L8c64ea60: *stop=0x8c64ea62u;r[14]=7u;goto L8c64eaaa; /* bra 0x8c64eaaa */
L8c64ea64: *stop=0x8c64ea64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ea90u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ea66; /* mov.l 0x8c64ea90,r3 */
L8c64ea66: *stop=0x8c64ea66u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[8],4);if(io_status)return io_status;goto L8c64ea68; /* mov.l r8,@(0x18,r4) */
L8c64ea68: *stop=0x8c64ea68u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[3],4);if(io_status)return io_status;goto L8c64ea6a; /* mov.l r3,@(0x8,r4) */
L8c64ea6a: *stop=0x8c64ea6cu;r[14]=r[12];goto L8c64eaaa; /* bra 0x8c64eaaa */
L8c64ea94: *stop=0x8c64ea94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64eb38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ea96; /* mov.l 0x8c64eb38,r2 */
L8c64ea96: *stop=0x8c64ea96u;r[3]=33u;goto L8c64ea98; /* mov #0x21,r3 */
L8c64ea98: *stop=0x8c64ea98u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[3],4);if(io_status)return io_status;goto L8c64ea9a; /* mov.l r3,@(0x18,r4) */
L8c64ea9a: *stop=0x8c64ea9au;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c64ea9c; /* mov.l r2,@(0x8,r4) */
L8c64ea9c: *stop=0x8c64ea9eu;r[14]=r[13];goto L8c64eaaa; /* bra 0x8c64eaaa */
L8c64eaa0: *stop=0x8c64eaa2u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[10],4);if(io_status)return io_status;goto L8c64eaa8; /* bra 0x8c64eaa8 */
L8c64eaa4: *stop=0x8c64eaa4u;r[2]=16u;goto L8c64eaa6; /* mov #0x10,r2 */
L8c64eaa6: *stop=0x8c64eaa6u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[2],4);if(io_status)return io_status;goto L8c64eaa8; /* mov.l r2,@(0x18,r4) */
L8c64eaa8: *stop=0x8c64eaa8u;r[14]=r[10];goto L8c64eaaa; /* mov r10,r14 */
L8c64eaaa: *stop=0x8c64eaaau;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64eaac; /* mov.l @(0x18,r4),r3 */
L8c64eaac: *stop=0x8c64eaacu;*sr=(*sr&~1u)|!(r[8]&r[3]);goto L8c64eaae; /* tst r8,r3 */
L8c64eaae: if(!(*sr&1u))goto L8c64eae4;goto L8c64eab0; /* bf 0x8c64eae4 */
L8c64eab0: *stop=0x8c64eab0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64eab2; /* mov.l @(0x4,r15),r0 */
L8c64eab2: *stop=0x8c64eab2u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64eab4; /* cmp/eq #0x0,r0 */
L8c64eab4: if((*sr&1u))goto L8c64eaca;goto L8c64eab6; /* bt 0x8c64eaca */
L8c64eab6: *stop=0x8c64eab6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64eab8; /* cmp/eq #0x1,r0 */
L8c64eab8: if((*sr&1u))goto L8c64eace;goto L8c64eaba; /* bt 0x8c64eace */
L8c64eaba: *stop=0x8c64eabau;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c64eabc; /* cmp/eq #0x2,r0 */
L8c64eabc: if((*sr&1u))goto L8c64ead4;goto L8c64eabe; /* bt 0x8c64ead4 */
L8c64eabe: *stop=0x8c64eabeu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c64eac0; /* cmp/eq #0x3,r0 */
L8c64eac0: if((*sr&1u))goto L8c64eada;goto L8c64eac2; /* bt 0x8c64eada */
L8c64eac2: *stop=0x8c64eac2u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c64eac4; /* cmp/eq #0x4,r0 */
L8c64eac4: if((*sr&1u))goto L8c64eae0;goto L8c64eac6; /* bt 0x8c64eae0 */
L8c64eac6: *stop=0x8c64eac8u;goto L8c64eb78; /* bra 0x8c64eb78 */
L8c64eaca: *stop=0x8c64eaccu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[10],4);if(io_status)return io_status;goto L8c64eae4; /* bra 0x8c64eae4 */
L8c64eace: *stop=0x8c64eaceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64eb3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ead0; /* mov.l 0x8c64eb3c,r2 */
L8c64ead0: *stop=0x8c64ead2u;goto L8c64eadc; /* bra 0x8c64eadc */
L8c64ead4: *stop=0x8c64ead4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64eb40u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ead6; /* mov.l 0x8c64eb40,r1 */
L8c64ead6: *stop=0x8c64ead8u;goto L8c64eae2; /* bra 0x8c64eae2 */
L8c64eada: *stop=0x8c64eadau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64eb44u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64eadc; /* mov.l 0x8c64eb44,r2 */
L8c64eadc: *stop=0x8c64eadeu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c64eae4; /* bra 0x8c64eae4 */
L8c64eae0: *stop=0x8c64eae0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64eb48u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64eae2; /* mov.l 0x8c64eb48,r1 */
L8c64eae2: *stop=0x8c64eae2u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[1],4);if(io_status)return io_status;goto L8c64eae4; /* mov.l r1,@(0x8,r4) */
L8c64eae4: *stop=0x8c64eae4u;r[0]=r[5];goto L8c64eae6; /* mov r5,r0 */
L8c64eae6: *stop=0x8c64eae6u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c64eae8; /* cmp/eq #0x8,r0 */
L8c64eae8: if((*sr&1u))goto L8c64eb12;goto L8c64eaea; /* bt 0x8c64eb12 */
L8c64eaea: *stop=0x8c64eaeau;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c64eaec; /* cmp/eq #0x10,r0 */
L8c64eaec: if((*sr&1u))goto L8c64eb16;goto L8c64eaee; /* bt 0x8c64eb16 */
L8c64eaee: *stop=0x8c64eaeeu;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c64eaf0; /* cmp/eq #0x20,r0 */
L8c64eaf0: if((*sr&1u))goto L8c64eb1a;goto L8c64eaf2; /* bt 0x8c64eb1a */
L8c64eaf2: *stop=0x8c64eaf2u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c64eaf4; /* cmp/eq #0x40,r0 */
L8c64eaf4: if((*sr&1u))goto L8c64eb1e;goto L8c64eaf6; /* bt 0x8c64eb1e */
L8c64eaf6: *stop=0x8c64eaf6u;r[1]=sx(m,bus,&io_status,0x8c64eb2eu,2);if(io_status)return io_status;goto L8c64eaf8; /* mov.w 0x8c64eb2e,r1 */
L8c64eaf8: *stop=0x8c64eaf8u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64eafa; /* cmp/eq r1,r0 */
L8c64eafa: if((*sr&1u))goto L8c64eb22;goto L8c64eafc; /* bt 0x8c64eb22 */
L8c64eafc: *stop=0x8c64eafcu;r[1]=sx(m,bus,&io_status,0x8c64eb30u,2);if(io_status)return io_status;goto L8c64eafe; /* mov.w 0x8c64eb30,r1 */
L8c64eafe: *stop=0x8c64eafeu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64eb00; /* cmp/eq r1,r0 */
L8c64eb00: if((*sr&1u))goto L8c64eb26;goto L8c64eb02; /* bt 0x8c64eb26 */
L8c64eb02: *stop=0x8c64eb02u;r[1]=sx(m,bus,&io_status,0x8c64eb32u,2);if(io_status)return io_status;goto L8c64eb04; /* mov.w 0x8c64eb32,r1 */
L8c64eb04: *stop=0x8c64eb04u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64eb06; /* cmp/eq r1,r0 */
L8c64eb06: if((*sr&1u))goto L8c64eb2a;goto L8c64eb08; /* bt 0x8c64eb2a */
L8c64eb08: *stop=0x8c64eb08u;r[1]=sx(m,bus,&io_status,0x8c64eb34u,2);if(io_status)return io_status;goto L8c64eb0a; /* mov.w 0x8c64eb34,r1 */
L8c64eb0a: *stop=0x8c64eb0au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64eb0c; /* cmp/eq r1,r0 */
L8c64eb0c: if((*sr&1u))goto L8c64eb4c;goto L8c64eb0e; /* bt 0x8c64eb4c */
L8c64eb0e: *stop=0x8c64eb10u;goto L8c64eb78; /* bra 0x8c64eb78 */
L8c64eb12: *stop=0x8c64eb14u;r[13]=7u;goto L8c64eb4e; /* bra 0x8c64eb4e */
L8c64eb16: *stop=0x8c64eb18u;r[13]=6u;goto L8c64eb4e; /* bra 0x8c64eb4e */
L8c64eb1a: *stop=0x8c64eb1cu;r[13]=5u;goto L8c64eb4e; /* bra 0x8c64eb4e */
L8c64eb1e: *stop=0x8c64eb20u;r[13]=r[9];goto L8c64eb4e; /* bra 0x8c64eb4e */
L8c64eb22: *stop=0x8c64eb24u;r[13]=3u;goto L8c64eb4e; /* bra 0x8c64eb4e */
L8c64eb26: *stop=0x8c64eb28u;r[13]=2u;goto L8c64eb4e; /* bra 0x8c64eb4e */
L8c64eb2a: *stop=0x8c64eb2cu;r[13]=1u;goto L8c64eb4e; /* bra 0x8c64eb4e */
L8c64eb4c: *stop=0x8c64eb4cu;r[13]=r[10];goto L8c64eb4e; /* mov r10,r13 */
L8c64eb4e: *stop=0x8c64eb4eu;r[0]=r[6];goto L8c64eb50; /* mov r6,r0 */
L8c64eb50: *stop=0x8c64eb50u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c64eb52; /* cmp/eq #0x8,r0 */
L8c64eb52: if((*sr&1u))goto L8c64eb7c;goto L8c64eb54; /* bt 0x8c64eb7c */
L8c64eb54: *stop=0x8c64eb54u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c64eb56; /* cmp/eq #0x10,r0 */
L8c64eb56: if((*sr&1u))goto L8c64eb7c;goto L8c64eb58; /* bt 0x8c64eb7c */
L8c64eb58: *stop=0x8c64eb58u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c64eb5a; /* cmp/eq #0x20,r0 */
L8c64eb5a: if((*sr&1u))goto L8c64eb7c;goto L8c64eb5c; /* bt 0x8c64eb7c */
L8c64eb5c: *stop=0x8c64eb5cu;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c64eb5e; /* cmp/eq #0x40,r0 */
L8c64eb5e: if((*sr&1u))goto L8c64eb7c;goto L8c64eb60; /* bt 0x8c64eb7c */
L8c64eb60: *stop=0x8c64eb60u;r[1]=sx(m,bus,&io_status,0x8c64ec12u,2);if(io_status)return io_status;goto L8c64eb62; /* mov.w 0x8c64ec12,r1 */
L8c64eb62: *stop=0x8c64eb62u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64eb64; /* cmp/eq r1,r0 */
L8c64eb64: if((*sr&1u))goto L8c64eb7c;goto L8c64eb66; /* bt 0x8c64eb7c */
L8c64eb66: *stop=0x8c64eb66u;r[1]=sx(m,bus,&io_status,0x8c64ec14u,2);if(io_status)return io_status;goto L8c64eb68; /* mov.w 0x8c64ec14,r1 */
L8c64eb68: *stop=0x8c64eb68u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64eb6a; /* cmp/eq r1,r0 */
L8c64eb6a: if((*sr&1u))goto L8c64eb7c;goto L8c64eb6c; /* bt 0x8c64eb7c */
L8c64eb6c: *stop=0x8c64eb6cu;r[1]=sx(m,bus,&io_status,0x8c64ec16u,2);if(io_status)return io_status;goto L8c64eb6e; /* mov.w 0x8c64ec16,r1 */
L8c64eb6e: *stop=0x8c64eb6eu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64eb70; /* cmp/eq r1,r0 */
L8c64eb70: if((*sr&1u))goto L8c64eb7c;goto L8c64eb72; /* bt 0x8c64eb7c */
L8c64eb72: *stop=0x8c64eb72u;r[1]=sx(m,bus,&io_status,0x8c64ec18u,2);if(io_status)return io_status;goto L8c64eb74; /* mov.w 0x8c64ec18,r1 */
L8c64eb74: *stop=0x8c64eb74u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64eb76; /* cmp/eq r1,r0 */
L8c64eb76: if((*sr&1u))goto L8c64eb7c;goto L8c64eb78; /* bt 0x8c64eb7c */
L8c64eb78: *stop=0x8c64eb7au;r[0]=4294967295u;goto L8c64ec46; /* bra 0x8c64ec46 */
L8c64eb7c: *stop=0x8c64eb7cu;*macl=r[6]*r[5];goto L8c64eb7e; /* mul.l r6,r5 */
L8c64eb7e: *stop=0x8c64eb7eu;r[7]<<=16;goto L8c64eb80; /* shll16 r7 */
L8c64eb80: *stop=0x8c64eb80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ec1cu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64eb82; /* mov.l 0x8c64ec1c,r11 */
L8c64eb82: *stop=0x8c64eb82u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[7],4);if(io_status)return io_status;goto L8c64eb84; /* mov.l r7,@r4 */
L8c64eb84: *stop=0x8c64eb84u;r[0]=r[14];goto L8c64eb86; /* mov r14,r0 */
L8c64eb86: *stop=0x8c64eb86u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64eb88; /* cmp/eq #0x0,r0 */
L8c64eb88: *stop=0x8c64eb88u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[5],4);if(io_status)return io_status;goto L8c64eb8a; /* mov.l r5,@(0xc,r4) */
L8c64eb8a: *stop=0x8c64eb8au;r[7]=*macl;goto L8c64eb8c; /* sts MACL,r7 */
L8c64eb8c: *stop=0x8c64eb8cu;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[6],4);if(io_status)return io_status;goto L8c64eb8e; /* mov.l r6,@(0x10,r4) */
L8c64eb8e: *stop=0x8c64eb8eu;r[12]=r[13];goto L8c64eb90; /* mov r13,r12 */
L8c64eb90: take=(*sr&1u);*stop=0x8c64eb92u;r[12]<<=2;if(take)goto L8c64ebbc;goto L8c64eb94; /* bt/s 0x8c64ebbc */
L8c64eb94: *stop=0x8c64eb94u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64eb96; /* cmp/eq #0x1,r0 */
L8c64eb96: if((*sr&1u))goto L8c64ebc0;goto L8c64eb98; /* bt 0x8c64ebc0 */
L8c64eb98: *stop=0x8c64eb98u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c64eb9a; /* cmp/eq #0x2,r0 */
L8c64eb9a: if((*sr&1u))goto L8c64ebca;goto L8c64eb9c; /* bt 0x8c64ebca */
L8c64eb9c: *stop=0x8c64eb9cu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c64eb9e; /* cmp/eq #0x3,r0 */
L8c64eb9e: if((*sr&1u))goto L8c64ebd4;goto L8c64eba0; /* bt 0x8c64ebd4 */
L8c64eba0: *stop=0x8c64eba0u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c64eba2; /* cmp/eq #0x4,r0 */
L8c64eba2: if((*sr&1u))goto L8c64ebe4;goto L8c64eba4; /* bt 0x8c64ebe4 */
L8c64eba4: *stop=0x8c64eba4u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c64eba6; /* cmp/eq #0x5,r0 */
L8c64eba6: if((*sr&1u))goto L8c64ebf8;goto L8c64eba8; /* bt 0x8c64ebf8 */
L8c64eba8: *stop=0x8c64eba8u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c64ebaa; /* cmp/eq #0x6,r0 */
L8c64ebaa: if((*sr&1u))goto L8c64ec0e;goto L8c64ebac; /* bt 0x8c64ec0e */
L8c64ebac: *stop=0x8c64ebacu;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c64ebae; /* cmp/eq #0x7,r0 */
L8c64ebae: if((*sr&1u))goto L8c64ec28;goto L8c64ebb0; /* bt 0x8c64ec28 */
L8c64ebb0: *stop=0x8c64ebb0u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c64ebb2; /* cmp/eq #0x8,r0 */
L8c64ebb2: if((*sr&1u))goto L8c64ec36;goto L8c64ebb4; /* bt 0x8c64ec36 */
L8c64ebb4: *stop=0x8c64ebb4u;*sr=(*sr&~1u)|(r[0]==0x00000009u);goto L8c64ebb6; /* cmp/eq #0x9,r0 */
L8c64ebb6: if((*sr&1u))goto L8c64ec3a;goto L8c64ebb8; /* bt 0x8c64ec3a */
L8c64ebb8: *stop=0x8c64ebbau;goto L8c64ec44; /* bra 0x8c64ec44 */
L8c64ebbc: *stop=0x8c64ebbeu;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c64ec36; /* bra 0x8c64ec36 */
L8c64ebc0: *stop=0x8c64ebc0u;r[0]=r[12];goto L8c64ebc2; /* mov r12,r0 */
L8c64ebc2: *stop=0x8c64ebc2u;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ebc4; /* mov.l @(r0,r11),r3 */
L8c64ebc4: *stop=0x8c64ebc4u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[3],4);if(io_status)return io_status;goto L8c64ebc6; /* mov.l r3,@(0x14,r4) */
L8c64ebc6: *stop=0x8c64ebc8u;goto L8c64ec44; /* bra 0x8c64ec44 */
L8c64ebca: *stop=0x8c64ebcau;r[3]=sx(m,bus,&io_status,0x8c64ec1au,2);if(io_status)return io_status;goto L8c64ebcc; /* mov.w 0x8c64ec1a,r3 */
L8c64ebcc: *stop=0x8c64ebccu;*sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u);goto L8c64ebce; /* shar r7 */
L8c64ebce: *stop=0x8c64ebceu;*sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u);goto L8c64ebd0; /* shar r7 */
L8c64ebd0: *stop=0x8c64ebd2u;r[7]+=r[3];goto L8c64ec36; /* bra 0x8c64ec36 */
L8c64ebd4: *stop=0x8c64ebd4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ec20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ebd6; /* mov.l 0x8c64ec20,r0 */
L8c64ebd6: *stop=0x8c64ebd6u;*sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u);goto L8c64ebd8; /* shar r7 */
L8c64ebd8: *stop=0x8c64ebd8u;*sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u);goto L8c64ebda; /* shar r7 */
L8c64ebda: *stop=0x8c64ebdau;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ebdc; /* mov.l @(r0,r12),r3 */
L8c64ebdc: *stop=0x8c64ebdcu;r[3]+=r[7];goto L8c64ebde; /* add r7,r3 */
L8c64ebde: *stop=0x8c64ebdeu;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[3],4);if(io_status)return io_status;goto L8c64ebe0; /* mov.l r3,@(0x14,r4) */
L8c64ebe0: *stop=0x8c64ebe2u;goto L8c64ec44; /* bra 0x8c64ec44 */
L8c64ebe4: *stop=0x8c64ebe4u;r[0]=r[12];goto L8c64ebe6; /* mov r12,r0 */
L8c64ebe6: *stop=0x8c64ebe6u;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ebe8; /* mov.l @(r0,r11),r3 */
L8c64ebe8: *stop=0x8c64ebe8u;r[2]=sx(m,bus,&io_status,0x8c64ec1au,2);if(io_status)return io_status;goto L8c64ebea; /* mov.w 0x8c64ec1a,r2 */
L8c64ebea: *stop=0x8c64ebeau;r[3]+=4u;goto L8c64ebec; /* add #0x4,r3 */
L8c64ebec: *stop=0x8c64ebecu;r[3]>>=2;goto L8c64ebee; /* shlr2 r3 */
L8c64ebee: *stop=0x8c64ebeeu;*sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c64ebf0; /* shlr r3 */
L8c64ebf0: *stop=0x8c64ebf0u;r[3]+=r[2];goto L8c64ebf2; /* add r2,r3 */
L8c64ebf2: *stop=0x8c64ebf2u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[3],4);if(io_status)return io_status;goto L8c64ebf4; /* mov.l r3,@(0x14,r4) */
L8c64ebf4: *stop=0x8c64ebf6u;goto L8c64ec44; /* bra 0x8c64ec44 */
L8c64ebf8: *stop=0x8c64ebf8u;r[0]=r[12];goto L8c64ebfa; /* mov r12,r0 */
L8c64ebfa: *stop=0x8c64ebfau;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ebfc; /* mov.l @(r0,r11),r3 */
L8c64ebfc: *stop=0x8c64ebfcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ec24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ebfe; /* mov.l 0x8c64ec24,r2 */
L8c64ebfe: *stop=0x8c64ebfeu;r[3]+=4u;goto L8c64ec00; /* add #0x4,r3 */
L8c64ec00: *stop=0x8c64ec00u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ec02; /* mov.l @(r0,r2),r2 */
L8c64ec02: *stop=0x8c64ec02u;r[3]>>=2;goto L8c64ec04; /* shlr2 r3 */
L8c64ec04: *stop=0x8c64ec04u;*sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c64ec06; /* shlr r3 */
L8c64ec06: *stop=0x8c64ec06u;r[3]+=r[2];goto L8c64ec08; /* add r2,r3 */
L8c64ec08: *stop=0x8c64ec08u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[3],4);if(io_status)return io_status;goto L8c64ec0a; /* mov.l r3,@(0x14,r4) */
L8c64ec0a: *stop=0x8c64ec0cu;goto L8c64ec44; /* bra 0x8c64ec44 */
L8c64ec0e: *stop=0x8c64ec10u;*sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u);goto L8c64ec36; /* bra 0x8c64ec36 */
L8c64ec28: *stop=0x8c64ec28u;r[0]=r[12];goto L8c64ec2a; /* mov r12,r0 */
L8c64ec2a: *stop=0x8c64ec2au;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ec2c; /* mov.l @(r0,r11),r3 */
L8c64ec2c: *stop=0x8c64ec2cu;r[3]+=4u;goto L8c64ec2e; /* add #0x4,r3 */
L8c64ec2e: *stop=0x8c64ec2eu;r[3]>>=2;goto L8c64ec30; /* shlr2 r3 */
L8c64ec30: *stop=0x8c64ec30u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[3],4);if(io_status)return io_status;goto L8c64ec32; /* mov.l r3,@(0x14,r4) */
L8c64ec32: *stop=0x8c64ec34u;goto L8c64ec44; /* bra 0x8c64ec44 */
L8c64ec36: *stop=0x8c64ec38u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[7],4);if(io_status)return io_status;goto L8c64ec44; /* bra 0x8c64ec44 */
L8c64ec3a: *stop=0x8c64ec3au;r[0]=r[12];goto L8c64ec3c; /* mov r12,r0 */
L8c64ec3c: *stop=0x8c64ec3cu;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ec3e; /* mov.l @(r0,r11),r3 */
L8c64ec3e: *stop=0x8c64ec3eu;r[3]+=4u;goto L8c64ec40; /* add #0x4,r3 */
L8c64ec40: *stop=0x8c64ec40u;*sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c64ec42; /* shlr r3 */
L8c64ec42: *stop=0x8c64ec42u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[3],4);if(io_status)return io_status;goto L8c64ec44; /* mov.l r3,@(0x14,r4) */
L8c64ec44: *stop=0x8c64ec44u;r[0]=1u;goto L8c64ec46; /* mov #0x1,r0 */
L8c64ec46: *stop=0x8c64ec46u;r[15]+=8u;goto L8c64ec48; /* add #0x8,r15 */
L8c64ec48: *stop=0x8c64ec48u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64ec4a; /* mov.l @r15+,r8 */
L8c64ec4a: *stop=0x8c64ec4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64ec4c; /* mov.l @r15+,r9 */
L8c64ec4c: *stop=0x8c64ec4cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64ec4e; /* mov.l @r15+,r10 */
L8c64ec4e: *stop=0x8c64ec4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64ec50; /* mov.l @r15+,r11 */
L8c64ec50: *stop=0x8c64ec50u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64ec52; /* mov.l @r15+,r12 */
L8c64ec52: *stop=0x8c64ec52u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64ec54; /* mov.l @r15+,r13 */
L8c64ec54: *stop=0x8c64ec54u;if(*pr!=entry_pr)return -3;*stop=0x8c64ec56u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

static int sa_player_texture_load_run_8c64ee8c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64eed4u)goto L8c64eed4;goto L8c64ee8c;
L8c64ee8c: *stop=0x8c64ee8cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ee8e; /* mov.l r14,@-r15 */
L8c64ee8e: *stop=0x8c64ee8eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ee90; /* mov.l r13,@-r15 */
L8c64ee90: *stop=0x8c64ee90u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ee92; /* sts.l PR,@-r15 */
L8c64ee92: *stop=0x8c64ee92u;r[15]+=4294967292u;goto L8c64ee94; /* add #-0x4,r15 */
L8c64ee94: *stop=0x8c64ee94u;r[13]=r[4];goto L8c64ee96; /* mov r4,r13 */
L8c64ee96: *stop=0x8c64ee96u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64ee98; /* mov.l @(0x4,r13),r4 */
L8c64ee98: *stop=0x8c64ee98u;r[1]=sx(m,bus,&io_status,0x8c64eef6u,2);if(io_status)return io_status;goto L8c64ee9a; /* mov.w 0x8c64eef6,r1 */
L8c64ee9a: *stop=0x8c64ee9au;r[14]=r[4];goto L8c64ee9c; /* mov r4,r14 */
L8c64ee9c: *stop=0x8c64ee9cu;r[14]>>=16;goto L8c64ee9e; /* shlr16 r14 */
L8c64ee9e: *stop=0x8c64ee9eu;r[0]=r[14];goto L8c64eea0; /* mov r14,r0 */
L8c64eea0: *stop=0x8c64eea0u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64eea2; /* mov.l @(0x14,r13),r6 */
L8c64eea2: *stop=0x8c64eea2u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64eea4; /* mov.l @(0x10,r13),r5 */
L8c64eea4: *stop=0x8c64eea4u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64eea6; /* cmp/eq r1,r0 */
L8c64eea6: *stop=0x8c64eea6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c64eea8; /* mov.l r4,@r15 */
L8c64eea8: take=(*sr&1u);*stop=0x8c64eeaau;r[4]=r[14]&255u;if(take)goto L8c64eeb6;goto L8c64eeac; /* bt/s 0x8c64eeb6 */
L8c64eeac: *stop=0x8c64eeacu;r[1]=sx(m,bus,&io_status,0x8c64eef8u,2);if(io_status)return io_status;goto L8c64eeae; /* mov.w 0x8c64eef8,r1 */
L8c64eeae: *stop=0x8c64eeaeu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64eeb0; /* cmp/eq r1,r0 */
L8c64eeb0: if((*sr&1u))goto L8c64eec6;goto L8c64eeb2; /* bt 0x8c64eec6 */
L8c64eeb2: *stop=0x8c64eeb4u;goto L8c64eeca; /* bra 0x8c64eeca */
L8c64eeb6: *stop=0x8c64eeb6u;*sr=(*sr&~1u)|(r[6]==r[5]);goto L8c64eeb8; /* cmp/eq r6,r5 */
L8c64eeb8: if(!(*sr&1u))goto L8c64eec0;goto L8c64eeba; /* bf 0x8c64eec0 */
L8c64eeba: *stop=0x8c64eebau;r[14]=sx(m,bus,&io_status,0x8c64eefau,2);if(io_status)return io_status;goto L8c64eebc; /* mov.w 0x8c64eefa,r14 */
L8c64eebc: *stop=0x8c64eebeu;goto L8c64eec2; /* bra 0x8c64eec2 */
L8c64eec0: *stop=0x8c64eec0u;r[14]=sx(m,bus,&io_status,0x8c64eefcu,2);if(io_status)return io_status;goto L8c64eec2; /* mov.w 0x8c64eefc,r14 */
L8c64eec2: *stop=0x8c64eec4u;r[14]|=r[4];goto L8c64eeca; /* bra 0x8c64eeca */
L8c64eec6: *stop=0x8c64eec6u;r[14]=sx(m,bus,&io_status,0x8c64eefeu,2);if(io_status)return io_status;goto L8c64eec8; /* mov.w 0x8c64eefe,r14 */
L8c64eec8: *stop=0x8c64eec8u;r[14]|=r[4];goto L8c64eeca; /* or r4,r14 */
L8c64eeca: *stop=0x8c64eecau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ef00u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64eecc; /* mov.l 0x8c64ef00,r2 */
L8c64eecc: *stop=0x8c64eeccu;r[4]=r[13];goto L8c64eece; /* mov r13,r4 */
L8c64eece: *stop=0x8c64eeceu;r[7]=r[14];goto L8c64eed0; /* mov r14,r7 */
L8c64eed0: target=r[2];*pr=0x8c64eed4u;*stop=0x8c64eed2u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64eed4; /* jsr @r2 */
L8c64eed4: *stop=0x8c64eed4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64eed6; /* tst r0,r0 */
L8c64eed6: if((*sr&1u))goto L8c64eee2;goto L8c64eed8; /* bt 0x8c64eee2 */
L8c64eed8: *stop=0x8c64eed8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ef04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64eeda; /* mov.l 0x8c64ef04,r3 */
L8c64eeda: *stop=0x8c64eedau;r[2]=6u;goto L8c64eedc; /* mov #0x6,r2 */
L8c64eedc: *stop=0x8c64eedcu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64eede; /* mov.l r2,@r3 */
L8c64eede: *stop=0x8c64eee0u;r[0]=4294967290u;goto L8c64eeec; /* bra 0x8c64eeec */
L8c64eee2: *stop=0x8c64eee2u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64eee4; /* mov.l @(0x4,r13),r2 */
L8c64eee4: *stop=0x8c64eee4u;r[0]=1u;goto L8c64eee6; /* mov #0x1,r0 */
L8c64eee6: *stop=0x8c64eee6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64eee8; /* mov.l @r15,r3 */
L8c64eee8: *stop=0x8c64eee8u;r[2]|=r[3];goto L8c64eeea; /* or r3,r2 */
L8c64eeea: *stop=0x8c64eeeau;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[2],4);if(io_status)return io_status;goto L8c64eeec; /* mov.l r2,@(0x4,r13) */
L8c64eeec: *stop=0x8c64eeecu;r[15]+=4u;goto L8c64eeee; /* add #0x4,r15 */
L8c64eeee: *stop=0x8c64eeeeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64eef0; /* lds.l @r15+,PR */
L8c64eef0: *stop=0x8c64eef0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64eef2; /* mov.l @r15+,r13 */
L8c64eef2: *stop=0x8c64eef2u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64eef4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c64ee8c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ee8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64eed4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ee8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64eed4u);}

static int sa_player_texture_load_run_8c64ef08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64ef26u)goto L8c64ef26;if(resume_pc==0x8c64ef3cu)goto L8c64ef3c;if(resume_pc==0x8c64ef56u)goto L8c64ef56;goto L8c64ef08;
L8c64ef08: *stop=0x8c64ef08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ef0a; /* mov.l r14,@-r15 */
L8c64ef0a: *stop=0x8c64ef0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ef0c; /* sts.l PR,@-r15 */
L8c64ef0c: *stop=0x8c64ef0cu;r[15]+=4294967292u;goto L8c64ef0e; /* add #-0x4,r15 */
L8c64ef0e: *stop=0x8c64ef0eu;r[3]=r[4];goto L8c64ef10; /* mov r4,r3 */
L8c64ef10: *stop=0x8c64ef10u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64ef12; /* tst r3,r3 */
L8c64ef12: *stop=0x8c64ef12u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c64ef14; /* mov.l r4,@r15 */
L8c64ef14: take=!(*sr&1u);*stop=0x8c64ef16u;r[14]=r[5];if(take)goto L8c64ef2e;goto L8c64ef18; /* bf/s 0x8c64ef2e */
L8c64ef18: *stop=0x8c64ef18u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ef1a; /* mov.l @r14,r5 */
L8c64ef1a: *stop=0x8c64ef1au;r[7]=0u;goto L8c64ef1c; /* mov #0x0,r7 */
L8c64ef1c: *stop=0x8c64ef1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f114u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ef1e; /* mov.l 0x8c64f114,r3 */
L8c64ef1e: *stop=0x8c64ef1eu;r[4]=r[14];goto L8c64ef20; /* mov r14,r4 */
L8c64ef20: *stop=0x8c64ef20u;r[6]=r[7];goto L8c64ef22; /* mov r7,r6 */
L8c64ef22: target=r[3];*pr=0x8c64ef26u;*stop=0x8c64ef24u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ef26; /* jsr @r3 */
L8c64ef26: *stop=0x8c64ef26u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64ef28; /* tst r0,r0 */
L8c64ef28: if(!(*sr&1u))goto L8c64ef40;goto L8c64ef2a; /* bf 0x8c64ef40 */
L8c64ef2a: *stop=0x8c64ef2cu;goto L8c64ef50; /* bra 0x8c64ef50 */
L8c64ef2e: *stop=0x8c64ef2eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ef30; /* mov.l @r15,r5 */
L8c64ef30: *stop=0x8c64ef30u;r[7]=0u;goto L8c64ef32; /* mov #0x0,r7 */
L8c64ef32: *stop=0x8c64ef32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f114u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ef34; /* mov.l 0x8c64f114,r3 */
L8c64ef34: *stop=0x8c64ef34u;r[4]=r[14];goto L8c64ef36; /* mov r14,r4 */
L8c64ef36: *stop=0x8c64ef36u;r[6]=r[7];goto L8c64ef38; /* mov r7,r6 */
L8c64ef38: target=r[3];*pr=0x8c64ef3cu;*stop=0x8c64ef3au;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ef3c; /* jsr @r3 */
L8c64ef3c: *stop=0x8c64ef3cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64ef3e; /* tst r0,r0 */
L8c64ef3e: if((*sr&1u))goto L8c64ef50;goto L8c64ef40; /* bt 0x8c64ef50 */
L8c64ef40: *stop=0x8c64ef40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f118u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ef42; /* mov.l 0x8c64f118,r2 */
L8c64ef42: *stop=0x8c64ef42u;r[3]=8u;goto L8c64ef44; /* mov #0x8,r3 */
L8c64ef44: *stop=0x8c64ef44u;r[0]=4294967288u;goto L8c64ef46; /* mov #-0x8,r0 */
L8c64ef46: *stop=0x8c64ef46u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64ef48; /* mov.l r3,@r2 */
L8c64ef48: *stop=0x8c64ef48u;r[15]+=4u;goto L8c64ef4a; /* add #0x4,r15 */
L8c64ef4a: *stop=0x8c64ef4au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64ef4c; /* lds.l @r15+,PR */
L8c64ef4c: *stop=0x8c64ef4cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64ef4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c64ef50: *stop=0x8c64ef50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f11cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64ef52; /* mov.l 0x8c64f11c,r14 */
L8c64ef52: target=r[14];*pr=0x8c64ef56u;*stop=0x8c64ef54u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ef56; /* jsr @r14 */
L8c64ef56: *stop=0x8c64ef56u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64ef58; /* tst r0,r0 */
L8c64ef58: if(!(*sr&1u))goto L8c64ef52;goto L8c64ef5a; /* bf 0x8c64ef52 */
L8c64ef5a: *stop=0x8c64ef5au;r[0]=1u;goto L8c64ef5c; /* mov #0x1,r0 */
L8c64ef5c: *stop=0x8c64ef5cu;r[15]+=4u;goto L8c64ef5e; /* add #0x4,r15 */
L8c64ef5e: *stop=0x8c64ef5eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64ef60; /* lds.l @r15+,PR */
L8c64ef60: *stop=0x8c64ef60u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64ef62u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c64ef08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ef08(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64ef26(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ef08(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ef26u);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64ef3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ef08(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ef3cu);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c64ef56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c64ef08(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ef56u);}

SA_STAGE_EXPORT int sa_player_texture_load_8c660e80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c660e80;
L8c660e80: *stop=0x8c660e80u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c660e82; /* mov.l r14,@-r15 */
L8c660e82: *stop=0x8c660e82u;r[6]=r[4];goto L8c660e84; /* mov r4,r6 */
L8c660e84: *stop=0x8c660e84u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c660e86; /* mov.l r13,@-r15 */
L8c660e86: *stop=0x8c660e86u;r[6]+=63u;goto L8c660e88; /* add #0x3f,r6 */
L8c660e88: *stop=0x8c660e88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660f40u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660e8a; /* mov.l 0x8c660f40,r2 */
L8c660e8a: *stop=0x8c660e8au;r[3]=4294967264u;goto L8c660e8c; /* mov #-0x20,r3 */
L8c660e8c: *stop=0x8c660e8cu;r[13]=sx(m,bus,&io_status,0x8c660f36u,2);if(io_status)return io_status;goto L8c660e8e; /* mov.w 0x8c660f36,r13 */
L8c660e8e: *stop=0x8c660e8eu;r[5]=64u;goto L8c660e90; /* mov #0x40,r5 */
L8c660e90: *stop=0x8c660e90u;r[7]=0u;goto L8c660e92; /* mov #0x0,r7 */
L8c660e92: *stop=0x8c660e92u;r[6]&=r[3];goto L8c660e94; /* and r3,r6 */
L8c660e94: *stop=0x8c660e94u;r[14]=1u;goto L8c660e96; /* mov #0x1,r14 */
L8c660e96: *stop=0x8c660e98u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto L8c660ed8; /* bra 0x8c660ed8 */
L8c660e9a: *stop=0x8c660e9au;if(!valid_access(r[4]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c660e9c; /* mov.w @(0x2,r4),r0 */
L8c660e9c: *stop=0x8c660e9cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660e9e; /* tst r0,r0 */
L8c660e9e: if(!(*sr&1u))goto L8c660ed6;goto L8c660ea0; /* bf 0x8c660ed6 */
L8c660ea0: *stop=0x8c660ea0u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660ea2; /* mov.l @(0x4,r4),r3 */
L8c660ea2: *stop=0x8c660ea2u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[6]);goto L8c660ea4; /* cmp/ge r6,r3 */
L8c660ea4: if(!(*sr&1u))goto L8c660ed6;goto L8c660ea6; /* bf 0x8c660ed6 */
L8c660ea6: *stop=0x8c660ea6u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c660ea8; /* mov.l @(0x4,r4),r1 */
L8c660ea8: *stop=0x8c660ea8u;r[1]-=r[6];goto L8c660eaa; /* sub r6,r1 */
L8c660eaa: *stop=0x8c660eaau;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[5]);goto L8c660eac; /* cmp/gt r5,r1 */
L8c660eac: if(!(*sr&1u))goto L8c660ecc;goto L8c660eae; /* bf 0x8c660ecc */
L8c660eae: *stop=0x8c660eaeu;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660eb0; /* mov.l @(0x4,r4),r2 */
L8c660eb0: *stop=0x8c660eb0u;r[5]=r[6];goto L8c660eb2; /* mov r6,r5 */
L8c660eb2: *stop=0x8c660eb2u;r[5]+=r[4];goto L8c660eb4; /* add r4,r5 */
L8c660eb4: *stop=0x8c660eb4u;r[0]=r[7];goto L8c660eb6; /* mov r7,r0 */
L8c660eb6: *stop=0x8c660eb6u;r[2]-=r[6];goto L8c660eb8; /* sub r6,r2 */
L8c660eb8: *stop=0x8c660eb8u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[2],4);if(io_status)return io_status;goto L8c660eba; /* mov.l r2,@(0x4,r5) */
L8c660eba: *stop=0x8c660ebau;if(!valid_access(r[5],2u))return -3;wr(m,bus,&io_status,r[5],r[13],2);if(io_status)return io_status;goto L8c660ebc; /* mov.w r13,@r5 */
L8c660ebc: *stop=0x8c660ebcu;if(!valid_access(r[5]+2u,2u))return -3;wr(m,bus,&io_status,r[5]+0x2u,r[0],2);if(io_status)return io_status;goto L8c660ebe; /* mov.w r0,@(0x2,r5) */
L8c660ebe: *stop=0x8c660ebeu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660ec0; /* mov.l @(0x8,r4),r3 */
L8c660ec0: *stop=0x8c660ec0u;if(!valid_access(r[3]+12u,4u))return -3;wr(m,bus,&io_status,r[3]+0xcu,r[5],4);if(io_status)return io_status;goto L8c660ec2; /* mov.l r5,@(0xc,r3) */
L8c660ec2: *stop=0x8c660ec2u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660ec4; /* mov.l @(0x8,r4),r2 */
L8c660ec4: *stop=0x8c660ec4u;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[2],4);if(io_status)return io_status;goto L8c660ec6; /* mov.l r2,@(0x8,r5) */
L8c660ec6: *stop=0x8c660ec6u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;goto L8c660ec8; /* mov.l r5,@(0x8,r4) */
L8c660ec8: *stop=0x8c660ec8u;if(!valid_access(r[5]+12u,4u))return -3;wr(m,bus,&io_status,r[5]+0xcu,r[4],4);if(io_status)return io_status;goto L8c660eca; /* mov.l r4,@(0xc,r5) */
L8c660eca: *stop=0x8c660ecau;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[6],4);if(io_status)return io_status;goto L8c660ecc; /* mov.l r6,@(0x4,r4) */
L8c660ecc: *stop=0x8c660eccu;r[0]=r[14];goto L8c660ece; /* mov r14,r0 */
L8c660ece: *stop=0x8c660eceu;if(!valid_access(r[4]+2u,2u))return -3;wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c660ed0; /* mov.w r0,@(0x2,r4) */
L8c660ed0: *stop=0x8c660ed0u;r[0]=r[4];goto L8c660ed2; /* mov r4,r0 */
L8c660ed2: *stop=0x8c660ed4u;r[0]+=32u;goto L8c660ee0; /* bra 0x8c660ee0 */
L8c660ed6: *stop=0x8c660ed6u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c660ed8; /* mov.l @(0x8,r4),r4 */
L8c660ed8: *stop=0x8c660ed8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660f38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660eda; /* mov.l 0x8c660f38,r3 */
L8c660eda: *stop=0x8c660edau;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c660edc; /* cmp/eq r3,r4 */
L8c660edc: if(!(*sr&1u))goto L8c660e9a;goto L8c660ede; /* bf 0x8c660e9a */
L8c660ede: *stop=0x8c660edeu;r[0]=0u;goto L8c660ee0; /* mov #0x0,r0 */
L8c660ee0: *stop=0x8c660ee0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c660ee2; /* mov.l @r15+,r13 */
L8c660ee2: *stop=0x8c660ee2u;if(*pr!=entry_pr)return -3;*stop=0x8c660ee4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_load_8c6044e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6044e0;
L8c6044e0: *stop=0x8c6044e0u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[0]=loaded;}r[5]+=4;goto L8c6044e2; /* mov.l @r5+,r0 */
L8c6044e2: *stop=0x8c6044e2u;r[6]--;*sr=(*sr&~1u)|(!r[6]);goto L8c6044e4; /* dt r6 */
L8c6044e4: *stop=0x8c6044e4u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c6044e6; /* mov.l r0,@r4 */
L8c6044e6: take=!(*sr&1u);*stop=0x8c6044e8u;r[4]+=4u;if(take)goto L8c6044e0;goto L8c6044ea; /* bf/s 0x8c6044e0 */
L8c6044ea: *stop=0x8c6044eau;if(*pr!=entry_pr)return -3;*stop=0x8c6044ecu;return 0; /* rts */
}

static int sa_player_texture_load_run_8c652e80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c652e98u)goto L8c652e98;goto L8c652e80;
L8c652e80: *stop=0x8c652e80u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c652e82; /* mov.l r14,@-r15 */
L8c652e82: *stop=0x8c652e82u;r[3]=4294967292u;goto L8c652e84; /* mov #-0x4,r3 */
L8c652e84: *stop=0x8c652e84u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c652e86; /* sts.l PR,@-r15 */
L8c652e86: *stop=0x8c652e86u;r[14]=r[4];goto L8c652e88; /* mov r4,r14 */
L8c652e88: *stop=0x8c652e88u;*sr=(*sr&~1u)|!(r[14]&r[3]);goto L8c652e8a; /* tst r14,r3 */
L8c652e8a: if(!(*sr&1u))goto L8c652e98;goto L8c652e8c; /* bf 0x8c652e98 */
L8c652e8c: *stop=0x8c652e8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652f08u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c652e8e; /* mov.l 0x8c652f08,r2 */
L8c652e8e: *stop=0x8c652e8eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c652e90; /* mov.l r14,@r2 */
L8c652e90: *stop=0x8c652e90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652f0cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652e92; /* mov.l 0x8c652f0c,r3 */
L8c652e92: *stop=0x8c652e92u;r[4]=sx(m,bus,&io_status,0x8c652f00u,2);if(io_status)return io_status;goto L8c652e94; /* mov.w 0x8c652f00,r4 */
L8c652e94: target=r[3];*pr=0x8c652e98u;*stop=0x8c652e96u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652e98; /* jsr @r3 */
L8c652e98: *stop=0x8c652e98u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c652e9a; /* lds.l @r15+,PR */
L8c652e9a: *stop=0x8c652e9au;r[0]=0u;goto L8c652e9c; /* mov #0x0,r0 */
L8c652e9c: *stop=0x8c652e9cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c652e9eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c652e80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c652e80(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c652e98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c652e80(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c652e98u);}

static int sa_player_texture_load_run_8c6533e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c6533f2u)goto L8c6533f2;goto L8c6533e0;
L8c6533e0: *stop=0x8c6533e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6533e2; /* sts.l PR,@-r15 */
L8c6533e2: *stop=0x8c6533e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653420u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6533e4; /* mov.l 0x8c653420,r3 */
L8c6533e4: *stop=0x8c6533e4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6533e6; /* mov.l @r3,r0 */
L8c6533e6: *stop=0x8c6533e6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6533e8; /* cmp/eq #0x1,r0 */
L8c6533e8: if((*sr&1u))goto L8c6533f6;goto L8c6533ea; /* bt 0x8c6533f6 */
L8c6533ea: *stop=0x8c6533eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c653428u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6533ec; /* mov.l 0x8c653428,r2 */
L8c6533ec: *stop=0x8c6533ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c653424u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6533ee; /* mov.l 0x8c653424,r3 */
L8c6533ee: target=r[2];*pr=0x8c6533f2u;*stop=0x8c6533f0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6533f2; /* jsr @r2 */
L8c6533f2: *stop=0x8c6533f2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6533f4; /* cmp/eq #0x1,r0 */
L8c6533f4: if(!(*sr&1u))goto L8c6533fc;goto L8c6533f6; /* bf 0x8c6533fc */
L8c6533f6: *stop=0x8c6533f6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6533f8; /* lds.l @r15+,PR */
L8c6533f8: *stop=0x8c6533f8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6533fau;r[0]=0u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c6533fc: *stop=0x8c6533fcu;r[0]=17u;goto L8c6533fe; /* mov #0x11,r0 */
L8c6533fe: *stop=0x8c6533feu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c653400; /* lds.l @r15+,PR */
L8c653400: *stop=0x8c653400u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c653402u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_load_8c6533e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c6533e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_load_resume_8c6533f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_load_run_8c6533e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6533f2u);}
