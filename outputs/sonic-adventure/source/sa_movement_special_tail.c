/* Original complete special movement path with actual world and contact dependencies.
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
static int sa_movement_special_tail_run_8c0777e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c077822u)goto L8c077822;if(resume_pc==0x8c077832u)goto L8c077832;if(resume_pc==0x8c077842u)goto L8c077842;if(resume_pc==0x8c077854u)goto L8c077854;if(resume_pc==0x8c077882u)goto L8c077882;if(resume_pc==0x8c0778b2u)goto L8c0778b2;if(resume_pc==0x8c0778c2u)goto L8c0778c2;if(resume_pc==0x8c0778d4u)goto L8c0778d4;if(resume_pc==0x8c0778dau)goto L8c0778da;if(resume_pc==0x8c077938u)goto L8c077938;if(resume_pc==0x8c0779b0u)goto L8c0779b0;if(resume_pc==0x8c0779ccu)goto L8c0779cc;if(resume_pc==0x8c077a2cu)goto L8c077a2c;if(resume_pc==0x8c077aaeu)goto L8c077aae;if(resume_pc==0x8c077c84u)goto L8c077c84;if(resume_pc==0x8c077c9au)goto L8c077c9a;if(resume_pc==0x8c077cc0u)goto L8c077cc0;if(resume_pc==0x8c077d0eu)goto L8c077d0e;if(resume_pc==0x8c077d52u)goto L8c077d52;if(resume_pc==0x8c077dd4u)goto L8c077dd4;if(resume_pc==0x8c077de6u)goto L8c077de6;if(resume_pc==0x8c077e10u)goto L8c077e10;if(resume_pc==0x8c077e1cu)goto L8c077e1c;if(resume_pc==0x8c077e2cu)goto L8c077e2c;if(resume_pc==0x8c077e36u)goto L8c077e36;if(resume_pc==0x8c077e46u)goto L8c077e46;if(resume_pc==0x8c077e56u)goto L8c077e56;if(resume_pc==0x8c077e68u)goto L8c077e68;if(resume_pc==0x8c077e94u)goto L8c077e94;if(resume_pc==0x8c077ec0u)goto L8c077ec0;if(resume_pc==0x8c077ed0u)goto L8c077ed0;if(resume_pc==0x8c077ee2u)goto L8c077ee2;if(resume_pc==0x8c077ee8u)goto L8c077ee8;if(resume_pc==0x8c077f0cu)goto L8c077f0c;if(resume_pc==0x8c077f1cu)goto L8c077f1c;if(resume_pc==0x8c077f24u)goto L8c077f24;if(resume_pc==0x8c078064u)goto L8c078064;if(resume_pc==0x8c0780b2u)goto L8c0780b2;if(resume_pc==0x8c078100u)goto L8c078100;if(resume_pc==0x8c07814cu)goto L8c07814c;if(resume_pc==0x8c078174u)goto L8c078174;if(resume_pc==0x8c0781c4u)goto L8c0781c4;if(resume_pc==0x8c078202u)goto L8c078202;if(resume_pc==0x8c0782b0u)goto L8c0782b0;goto L8c0777e0;
L8c0777e0: *stop=0x8c0777e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0777e2; /* mov.l r14,@-r15 */
L8c0777e2: *stop=0x8c0777e2u;r[0]=4294967294u;goto L8c0777e4; /* mov #-0x2,r0 */
L8c0777e4: *stop=0x8c0777e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0777e6; /* mov.l r13,@-r15 */
L8c0777e6: *stop=0x8c0777e6u;r[0]<<=8;goto L8c0777e8; /* shll8 r0 */
L8c0777e8: *stop=0x8c0777e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0777ea; /* mov.l r12,@-r15 */
L8c0777ea: *stop=0x8c0777eau;r[0]+=4294967288u;goto L8c0777ec; /* add #-0x8,r0 */
L8c0777ec: *stop=0x8c0777ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0777ee; /* mov.l r11,@-r15 */
L8c0777ee: *stop=0x8c0777eeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c0777f0; /* mov.l r10,@-r15 */
L8c0777f0: *stop=0x8c0777f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c0777f2; /* mov.l r9,@-r15 */
L8c0777f2: *stop=0x8c0777f2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c0777f4; /* mov.l r8,@-r15 */
L8c0777f4: *stop=0x8c0777f4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c0777f6; /* fmov.s fr15,@-r15 */
L8c0777f6: *stop=0x8c0777f6u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c0777f8; /* fmov.s fr14,@-r15 */
L8c0777f8: *stop=0x8c0777f8u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c0777fa; /* fmov.s fr13,@-r15 */
L8c0777fa: *stop=0x8c0777fau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c0777fc; /* fmov.s fr12,@-r15 */
L8c0777fc: *stop=0x8c0777fcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0777fe; /* sts.l PR,@-r15 */
L8c0777fe: *stop=0x8c0777feu;r[15]+=r[0];goto L8c077800; /* add r0,r15 */
L8c077800: *stop=0x8c077800u;r[12]=r[6];goto L8c077802; /* mov r6,r12 */
L8c077802: *stop=0x8c077802u;r[0]=100u;goto L8c077804; /* mov #0x64,r0 */
L8c077804: *stop=0x8c077804u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c077806; /* mov.l @(r0,r12),r14 */
L8c077806: *stop=0x8c077806u;r[13]=r[4];goto L8c077808; /* mov r4,r13 */
L8c077808: *stop=0x8c077808u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c07780a; /* mov.w @(0x10,r12),r0 */
L8c07780a: *stop=0x8c07780au;r[3]=4294967294u;goto L8c07780c; /* mov #-0x2,r3 */
L8c07780c: *stop=0x8c07780cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0779e0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07780e; /* mov.l 0x8c0779e0,r2 */
L8c07780e: *stop=0x8c07780eu;r[10]=0u;goto L8c077810; /* mov #0x0,r10 */
L8c077810: *stop=0x8c077810u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[0],4);if(io_status)return io_status;goto L8c077812; /* mov.l r0,@(0x14,r15) */
L8c077812: *stop=0x8c077812u;r[8]=r[10];goto L8c077814; /* mov r10,r8 */
L8c077814: *stop=0x8c077814u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c077816; /* mov.w @(0x4,r13),r0 */
L8c077816: *stop=0x8c077816u;r[11]=r[5];goto L8c077818; /* mov r5,r11 */
L8c077818: *stop=0x8c077818u;if(!valid_access(r[12]+16u,2u))return -3;wr(m,bus,&io_status,r[12]+0x10u,r[0],2);if(io_status)return io_status;goto L8c07781a; /* mov.w r0,@(0x10,r12) */
L8c07781a: *stop=0x8c07781au;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c07781c; /* mov.w @(0x4,r13),r0 */
L8c07781c: *stop=0x8c07781cu;r[0]&=r[3];goto L8c07781e; /* and r3,r0 */
L8c07781e: target=r[2];*pr=0x8c077822u;*stop=0x8c077820u;if(!valid_access(r[13]+4u,2u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[0],2);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077822; /* jsr @r2 */
L8c077822: *stop=0x8c077822u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077824; /* mov.l @(0x1c,r13),r1 */
L8c077824: *stop=0x8c077824u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c077826; /* tst r1,r1 */
L8c077826: take=(*sr&1u);*stop=0x8c077828u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;if(take)goto L8c077832;goto L8c07782a; /* bt/s 0x8c077832 */
L8c07782a: *stop=0x8c07782au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0779e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07782c; /* mov.l 0x8c0779e4,r3 */
L8c07782c: *stop=0x8c07782cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c07782e; /* mov.l @(0x4,r15),r5 */
L8c07782e: target=r[3];*pr=0x8c077832u;*stop=0x8c077830u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077832; /* jsr @r3 */
L8c077832: *stop=0x8c077832u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077834; /* mov.l @(0x14,r13),r3 */
L8c077834: *stop=0x8c077834u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c077836; /* tst r3,r3 */
L8c077836: take=(*sr&1u);*stop=0x8c077838u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;if(take)goto L8c077842;goto L8c07783a; /* bt/s 0x8c077842 */
L8c07783a: *stop=0x8c07783au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0779e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07783c; /* mov.l 0x8c0779e8,r3 */
L8c07783c: *stop=0x8c07783cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c07783e; /* mov.l @(0x4,r15),r5 */
L8c07783e: target=r[3];*pr=0x8c077842u;*stop=0x8c077840u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077842; /* jsr @r3 */
L8c077842: *stop=0x8c077842u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077844; /* mov.l @(0x18,r13),r3 */
L8c077844: *stop=0x8c077844u;r[3]=0u-r[3];goto L8c077846; /* neg r3,r3 */
L8c077846: *stop=0x8c077846u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c077848; /* tst r3,r3 */
L8c077848: take=(*sr&1u);*stop=0x8c07784au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;if(take)goto L8c077854;goto L8c07784c; /* bt/s 0x8c077854 */
L8c07784c: *stop=0x8c07784cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0779ecu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07784e; /* mov.l 0x8c0779ec,r1 */
L8c07784e: *stop=0x8c07784eu;r[5]=r[3];goto L8c077850; /* mov r3,r5 */
L8c077850: target=r[1];*pr=0x8c077854u;*stop=0x8c077852u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077854; /* jsr @r1 */
L8c077854: *stop=0x8c077854u;r[0]=123u;goto L8c077856; /* mov #0x7b,r0 */
L8c077856: *stop=0x8c077856u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c077858; /* fldi0 fr15 */
L8c077858: *stop=0x8c077858u;r[0]<<=2;goto L8c07785a; /* shll2 r0 */
L8c07785a: *stop=0x8c07785au;r[6]=r[14];goto L8c07785c; /* mov r14,r6 */
L8c07785c: *stop=0x8c07785cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c07785e; /* fmov fr15,@(r0,r15) */
L8c07785e: *stop=0x8c07785eu;r[0]=121u;goto L8c077860; /* mov #0x79,r0 */
L8c077860: *stop=0x8c077860u;r[0]<<=2;goto L8c077862; /* shll2 r0 */
L8c077862: *stop=0x8c077862u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c077864; /* fldi1 fr3 */
L8c077864: *stop=0x8c077864u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c077866; /* fmov fr15,@(r0,r15) */
L8c077866: *stop=0x8c077866u;r[5]=121u;goto L8c077868; /* mov #0x79,r5 */
L8c077868: *stop=0x8c077868u;r[0]=122u;goto L8c07786a; /* mov #0x7a,r0 */
L8c07786a: *stop=0x8c07786au;r[5]<<=2;goto L8c07786c; /* shll2 r5 */
L8c07786c: *stop=0x8c07786cu;r[0]<<=2;goto L8c07786e; /* shll2 r0 */
L8c07786e: *stop=0x8c07786eu;r[5]+=r[15];goto L8c077870; /* add r15,r5 */
L8c077870: *stop=0x8c077870u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c077872; /* fmov fr3,@(r0,r15) */
L8c077872: *stop=0x8c077872u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077874; /* mov.l @(0x14,r13),r3 */
L8c077874: *stop=0x8c077874u;r[6]+=28u;goto L8c077876; /* add #0x1c,r6 */
L8c077876: *stop=0x8c077876u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[3],4);if(io_status)return io_status;goto L8c077878; /* mov.l r3,@(0x34,r14) */
L8c077878: *stop=0x8c077878u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07787a; /* mov.l @(0x1c,r13),r2 */
L8c07787a: *stop=0x8c07787au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0779f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07787c; /* mov.l 0x8c0779f0,r3 */
L8c07787c: *stop=0x8c07787cu;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[2],4);if(io_status)return io_status;goto L8c07787e; /* mov.l r2,@(0x38,r14) */
L8c07787e: target=r[3];*pr=0x8c077882u;*stop=0x8c077880u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077882; /* jsr @r3 */
L8c077882: *stop=0x8c077882u;r[0]=123u;goto L8c077884; /* mov #0x7b,r0 */
L8c077884: *stop=0x8c077884u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0779f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077886; /* mov.l 0x8c0779f0,r3 */
L8c077886: *stop=0x8c077886u;r[0]<<=2;goto L8c077888; /* shll2 r0 */
L8c077888: *stop=0x8c077888u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c07788a; /* fmov fr15,@(r0,r15) */
L8c07788a: *stop=0x8c07788au;r[0]=121u;goto L8c07788c; /* mov #0x79,r0 */
L8c07788c: *stop=0x8c07788cu;r[0]<<=2;goto L8c07788e; /* shll2 r0 */
L8c07788e: *stop=0x8c07788eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c077890; /* fmov fr15,@(r0,r15) */
L8c077890: *stop=0x8c077890u;r[0]=0x8c0779f4u;goto L8c077892; /* mova 0x8c0779f4,r0 */
L8c077892: *stop=0x8c077892u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c077894; /* fmov.s @r0,fr14 */
L8c077894: *stop=0x8c077894u;r[0]=68u;goto L8c077896; /* mov #0x44,r0 */
L8c077896: *stop=0x8c077896u;r[0]<<=2;goto L8c077898; /* shll2 r0 */
L8c077898: *stop=0x8c077898u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c07789a; /* fmov.s @(r0,r12),fr3 */
L8c07789a: *stop=0x8c07789au;r[6]=127u;goto L8c07789c; /* mov #0x7f,r6 */
L8c07789c: *stop=0x8c07789cu;r[5]=121u;goto L8c07789e; /* mov #0x79,r5 */
L8c07789e: *stop=0x8c07789eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0778a0; /* fmul fr14,fr3 */
L8c0778a0: *stop=0x8c0778a0u;r[0]=122u;goto L8c0778a2; /* mov #0x7a,r0 */
L8c0778a2: *stop=0x8c0778a2u;r[5]<<=2;goto L8c0778a4; /* shll2 r5 */
L8c0778a4: *stop=0x8c0778a4u;r[6]<<=2;goto L8c0778a6; /* shll2 r6 */
L8c0778a6: *stop=0x8c0778a6u;r[0]<<=2;goto L8c0778a8; /* shll2 r0 */
L8c0778a8: *stop=0x8c0778a8u;r[5]+=r[15];goto L8c0778aa; /* add r15,r5 */
L8c0778aa: *stop=0x8c0778aau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0778ac; /* fmov fr3,@(r0,r15) */
L8c0778ac: *stop=0x8c0778acu;r[6]+=r[15];goto L8c0778ae; /* add r15,r6 */
L8c0778ae: target=r[3];*pr=0x8c0778b2u;*stop=0x8c0778b0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0778b2; /* jsr @r3 */
L8c0778b2: *stop=0x8c0778b2u;r[7]=121u;goto L8c0778b4; /* mov #0x79,r7 */
L8c0778b4: *stop=0x8c0778b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0779f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0778b6; /* mov.l 0x8c0779f8,r2 */
L8c0778b6: *stop=0x8c0778b6u;r[7]<<=2;goto L8c0778b8; /* shll2 r7 */
L8c0778b8: *stop=0x8c0778b8u;r[5]=r[12];goto L8c0778ba; /* mov r12,r5 */
L8c0778ba: *stop=0x8c0778bau;r[7]+=r[15];goto L8c0778bc; /* add r15,r7 */
L8c0778bc: *stop=0x8c0778bcu;r[6]=r[14];goto L8c0778be; /* mov r14,r6 */
L8c0778be: target=r[2];*pr=0x8c0778c2u;*stop=0x8c0778c0u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0778c2; /* jsr @r2 */
L8c0778c2: *stop=0x8c0778c2u;r[5]=121u;goto L8c0778c4; /* mov #0x79,r5 */
L8c0778c4: *stop=0x8c0778c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0779f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0778c6; /* mov.l 0x8c0779f0,r3 */
L8c0778c6: *stop=0x8c0778c6u;r[6]=124u;goto L8c0778c8; /* mov #0x7c,r6 */
L8c0778c8: *stop=0x8c0778c8u;r[5]<<=2;goto L8c0778ca; /* shll2 r5 */
L8c0778ca: *stop=0x8c0778cau;r[6]<<=2;goto L8c0778cc; /* shll2 r6 */
L8c0778cc: *stop=0x8c0778ccu;r[6]+=r[15];goto L8c0778ce; /* add r15,r6 */
L8c0778ce: *stop=0x8c0778ceu;r[5]+=r[15];goto L8c0778d0; /* add r15,r5 */
L8c0778d0: target=r[3];*pr=0x8c0778d4u;*stop=0x8c0778d2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0778d4; /* jsr @r3 */
L8c0778d4: *stop=0x8c0778d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0779fcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0778d6; /* mov.l 0x8c0779fc,r2 */
L8c0778d6: target=r[2];*pr=0x8c0778dau;*stop=0x8c0778d8u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0778da; /* jsr @r2 */
L8c0778da: *stop=0x8c0778dau;r[0]=124u;goto L8c0778dc; /* mov #0x7c,r0 */
L8c0778dc: *stop=0x8c0778dcu;r[0]<<=2;goto L8c0778de; /* shll2 r0 */
L8c0778de: *stop=0x8c0778deu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0778e0; /* fmov.s @(r0,r15),fr3 */
L8c0778e0: *stop=0x8c0778e0u;r[0]=32u;goto L8c0778e2; /* mov #0x20,r0 */
L8c0778e2: *stop=0x8c0778e2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0778e4; /* fmov.s @(r0,r13),fr2 */
L8c0778e4: *stop=0x8c0778e4u;r[0]=4u;goto L8c0778e6; /* mov #0x4,r0 */
L8c0778e6: *stop=0x8c0778e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0778e8; /* fadd fr3,fr2 */
L8c0778e8: *stop=0x8c0778e8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0778ea; /* fmov fr2,@(r0,r14) */
L8c0778ea: *stop=0x8c0778eau;r[0]=125u;goto L8c0778ec; /* mov #0x7d,r0 */
L8c0778ec: *stop=0x8c0778ecu;r[0]<<=2;goto L8c0778ee; /* shll2 r0 */
L8c0778ee: *stop=0x8c0778eeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0778f0; /* fmov.s @(r0,r15),fr3 */
L8c0778f0: *stop=0x8c0778f0u;r[0]=36u;goto L8c0778f2; /* mov #0x24,r0 */
L8c0778f2: *stop=0x8c0778f2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0778f4; /* fmov.s @(r0,r13),fr2 */
L8c0778f4: *stop=0x8c0778f4u;r[0]=8u;goto L8c0778f6; /* mov #0x8,r0 */
L8c0778f6: *stop=0x8c0778f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0778f8; /* fadd fr3,fr2 */
L8c0778f8: *stop=0x8c0778f8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0778fa; /* fmov fr2,@(r0,r14) */
L8c0778fa: *stop=0x8c0778fau;r[0]=126u;goto L8c0778fc; /* mov #0x7e,r0 */
L8c0778fc: *stop=0x8c0778fcu;r[0]<<=2;goto L8c0778fe; /* shll2 r0 */
L8c0778fe: *stop=0x8c0778feu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c077900; /* fmov.s @(r0,r15),fr3 */
L8c077900: *stop=0x8c077900u;r[0]=40u;goto L8c077902; /* mov #0x28,r0 */
L8c077902: *stop=0x8c077902u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077904; /* fmov.s @(r0,r13),fr2 */
L8c077904: *stop=0x8c077904u;r[0]=12u;goto L8c077906; /* mov #0xc,r0 */
L8c077906: *stop=0x8c077906u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c077908; /* fadd fr3,fr2 */
L8c077908: *stop=0x8c077908u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07790a; /* fmov fr2,@(r0,r14) */
L8c07790a: *stop=0x8c07790au;r[0]=68u;goto L8c07790c; /* mov #0x44,r0 */
L8c07790c: *stop=0x8c07790cu;r[0]<<=2;goto L8c07790e; /* shll2 r0 */
L8c07790e: *stop=0x8c07790eu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c077910; /* fmov.s @(r0,r12),fr3 */
L8c077910: *stop=0x8c077910u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c077912; /* fmul fr14,fr3 */
L8c077912: *stop=0x8c077912u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c077914; /* fmov.s fr3,@r14 */
L8c077914: *stop=0x8c077914u;if(!valid_access(r[11]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077916; /* mov.l @(0x4,r11),r3 */
L8c077916: *stop=0x8c077916u;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077918; /* mov.l @(0x8,r11),r2 */
L8c077918: *stop=0x8c077918u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c07791a; /* mov.l r3,@(0x10,r14) */
L8c07791a: *stop=0x8c07791au;r[1]=118u;goto L8c07791c; /* mov #0x76,r1 */
L8c07791c: *stop=0x8c07791cu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[2],4);if(io_status)return io_status;goto L8c07791e; /* mov.l r2,@(0x14,r14) */
L8c07791e: *stop=0x8c07791eu;r[1]<<=2;goto L8c077920; /* shll2 r1 */
L8c077920: *stop=0x8c077920u;if(!valid_access(r[11]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077922; /* mov.l @(0xc,r11),r3 */
L8c077922: *stop=0x8c077922u;r[1]+=r[15];goto L8c077924; /* add r15,r1 */
L8c077924: *stop=0x8c077924u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077a00u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077926; /* mov.l 0x8c077a00,r2 */
L8c077926: *stop=0x8c077926u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c077928; /* mov.l r3,@(0x18,r14) */
L8c077928: *stop=0x8c077928u;if(!valid_access(r[13]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c07792a; /* mov.l @(0x20,r13),r0 */
L8c07792a: *stop=0x8c07792au;if(!valid_access(r[13]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07792c; /* mov.l @(0x24,r13),r3 */
L8c07792c: *stop=0x8c07792cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c07792e; /* mov.l r0,@r1 */
L8c07792e: *stop=0x8c07792eu;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[3],4);if(io_status)return io_status;goto L8c077930; /* mov.l r3,@(0x4,r1) */
L8c077930: *stop=0x8c077930u;if(!valid_access(r[13]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c077932; /* mov.l @(0x28,r13),r0 */
L8c077932: *stop=0x8c077932u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c077934; /* mov.l r0,@(0x8,r1) */
L8c077934: target=r[2];*pr=0x8c077938u;*stop=0x8c077936u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077938; /* jsr @r2 */
L8c077938: *stop=0x8c077938u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077a04u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07793a; /* mov.l 0x8c077a04,r2 */
L8c07793a: *stop=0x8c07793au;if(!valid_access(r[2],2u))return -3;r[3]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c07793c; /* mov.w @r2,r3 */
L8c07793c: *stop=0x8c07793cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c07793e; /* tst r3,r3 */
L8c07793e: if(!(*sr&1u))goto L8c077966;goto L8c077940; /* bf 0x8c077966 */
L8c077940: *stop=0x8c077940u;r[1]=4u;goto L8c077942; /* mov #0x4,r1 */
L8c077942: *stop=0x8c077942u;r[1]+=r[11];goto L8c077944; /* add r11,r1 */
L8c077944: *stop=0x8c077944u;r[0]=32u;goto L8c077946; /* mov #0x20,r0 */
L8c077946: *stop=0x8c077946u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c077948; /* fmov.s @r1,fr3 */
L8c077948: *stop=0x8c077948u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07794a; /* fmov.s @(r0,r13),fr2 */
L8c07794a: *stop=0x8c07794au;r[1]=8u;goto L8c07794c; /* mov #0x8,r1 */
L8c07794c: *stop=0x8c07794cu;r[1]+=r[11];goto L8c07794e; /* add r11,r1 */
L8c07794e: *stop=0x8c07794eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c077950; /* fadd fr3,fr2 */
L8c077950: *stop=0x8c077950u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077952; /* fmov fr2,@(r0,r13) */
L8c077952: *stop=0x8c077952u;r[0]=36u;goto L8c077954; /* mov #0x24,r0 */
L8c077954: *stop=0x8c077954u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077956; /* fmov.s @(r0,r13),fr2 */
L8c077956: *stop=0x8c077956u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c077958; /* fmov.s @r1,fr3 */
L8c077958: *stop=0x8c077958u;r[1]=12u;goto L8c07795a; /* mov #0xc,r1 */
L8c07795a: *stop=0x8c07795au;r[1]+=r[11];goto L8c07795c; /* add r11,r1 */
L8c07795c: *stop=0x8c07795cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07795e; /* fadd fr3,fr2 */
L8c07795e: *stop=0x8c07795eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077960; /* fmov fr2,@(r0,r13) */
L8c077960: *stop=0x8c077960u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c077962; /* fmov.s @r1,fr3 */
L8c077962: *stop=0x8c077964u;r[0]=40u;goto L8c077a9c; /* bra 0x8c077a9c */
L8c077966: *stop=0x8c077966u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077a08u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077968; /* mov.l 0x8c077a08,r3 */
L8c077968: *stop=0x8c077968u;r[9]=r[10];goto L8c07796a; /* mov r10,r9 */
L8c07796a: *stop=0x8c07796au;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[10],4);if(io_status)return io_status;goto L8c07796c; /* mov.l r10,@(0x20,r15) */
L8c07796c: *stop=0x8c07796cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c07796e; /* mov.l r3,@r15 */
L8c07796e: *stop=0x8c07796eu;if(!valid_access(r[15]+36u,4u))return -3;wr(m,bus,&io_status,r[15]+0x24u,r[10],4);if(io_status)return io_status;goto L8c077970; /* mov.l r10,@(0x24,r15) */
L8c077970: *stop=0x8c077970u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[10],4);if(io_status)return io_status;goto L8c077972; /* mov.l r10,@(0xc,r15) */
L8c077972: *stop=0x8c077972u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[10],4);if(io_status)return io_status;goto L8c077974; /* mov.l r10,@(0x4,r15) */
L8c077974: *stop=0x8c077976u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[10],4);if(io_status)return io_status;goto L8c077a68; /* bra 0x8c077a68 */
L8c077978: *stop=0x8c077978u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c07797a; /* mov.l @r15,r2 */
L8c07797a: *stop=0x8c07797au;{uint32_t loaded=rd(m,bus,&io_status,0x8c077a0cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07797c; /* mov.l 0x8c077a0c,r3 */
L8c07797c: *stop=0x8c07797cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c07797e; /* mov.l @r2,r1 */
L8c07797e: *stop=0x8c07797eu;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c077980; /* tst r3,r1 */
L8c077980: if((*sr&1u))goto L8c07798e;goto L8c077982; /* bt 0x8c07798e */
L8c077982: *stop=0x8c077982u;if(!valid_access(r[13]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x9u,1);if(io_status)return io_status;goto L8c077984; /* mov.b @(0x9,r13),r0 */
L8c077984: *stop=0x8c077984u;r[0]=r[0]&255u;goto L8c077986; /* extu.b r0,r0 */
L8c077986: *stop=0x8c077986u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c077988; /* cmp/eq #0x6,r0 */
L8c077988: if((*sr&1u))goto L8c07798e;goto L8c07798a; /* bt 0x8c07798e */
L8c07798a: *stop=0x8c07798cu;goto L8c077a5c; /* bra 0x8c077a5c */
L8c07798e: *stop=0x8c07798eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c077990; /* mov.l @r15,r4 */
L8c077990: *stop=0x8c077990u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c077992; /* mov.l @(0x8,r4),r4 */
L8c077992: *stop=0x8c077992u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c077994; /* tst r4,r4 */
L8c077994: if((*sr&1u))goto L8c0779a2;goto L8c077996; /* bt 0x8c0779a2 */
L8c077996: *stop=0x8c077996u;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077998; /* mov.l @(0x20,r4),r2 */
L8c077998: *stop=0x8c077998u;r[3]=1u;goto L8c07799a; /* mov #0x1,r3 */
L8c07799a: *stop=0x8c07799au;r[3]<<=8;goto L8c07799c; /* shll8 r3 */
L8c07799c: *stop=0x8c07799cu;if(!valid_access(r[2]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x4u,2);if(io_status)return io_status;goto L8c07799e; /* mov.w @(0x4,r2),r0 */
L8c07799e: *stop=0x8c07799eu;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c0779a0; /* tst r3,r0 */
L8c0779a0: if((*sr&1u))goto L8c077a5c;goto L8c0779a2; /* bt 0x8c077a5c */
L8c0779a2: *stop=0x8c0779a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c0779a4; /* mov.l @r15,r5 */
L8c0779a4: *stop=0x8c0779a4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c0779a6; /* mov.l @r15,r6 */
L8c0779a6: *stop=0x8c0779a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077a10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0779a8; /* mov.l 0x8c077a10,r3 */
L8c0779a8: *stop=0x8c0779a8u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0779aa; /* mov.l @(0x4,r5),r5 */
L8c0779aa: *stop=0x8c0779aau;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[6]=loaded;}goto L8c0779ac; /* mov.l @r6,r6 */
L8c0779ac: target=r[3];*pr=0x8c0779b0u;*stop=0x8c0779aeu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0779b0; /* jsr @r3 */
L8c0779b0: *stop=0x8c0779b0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0779b2; /* tst r0,r0 */
L8c0779b2: if((*sr&1u))goto L8c077a36;goto L8c0779b4; /* bt 0x8c077a36 */
L8c0779b4: *stop=0x8c0779b4u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c0779b6; /* mov.w @(0x10,r12),r0 */
L8c0779b6: *stop=0x8c0779b6u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c0779b8; /* tst #0x3,r0 */
L8c0779b8: if((*sr&1u))goto L8c077a20;goto L8c0779ba; /* bt 0x8c077a20 */
L8c0779ba: *stop=0x8c0779bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0779bc; /* mov.l @r15,r2 */
L8c0779bc: *stop=0x8c0779bcu;r[5]=r[11];goto L8c0779be; /* mov r11,r5 */
L8c0779be: *stop=0x8c0779beu;r[6]=r[12];goto L8c0779c0; /* mov r12,r6 */
L8c0779c0: *stop=0x8c0779c0u;r[7]=r[14];goto L8c0779c2; /* mov r14,r7 */
L8c0779c2: *stop=0x8c0779c2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0779c4; /* mov.l @r2,r3 */
L8c0779c4: *stop=0x8c0779c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077a14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0779c6; /* mov.l 0x8c077a14,r2 */
L8c0779c6: *stop=0x8c0779c6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0779c8; /* mov.l r3,@-r15 */
L8c0779c8: target=r[2];*pr=0x8c0779ccu;*stop=0x8c0779cau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0779cc; /* jsr @r2 */
L8c0779cc: *stop=0x8c0779ccu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0779ce; /* cmp/eq #0x1,r0 */
L8c0779ce: take=!(*sr&1u);*stop=0x8c0779d0u;r[15]+=4u;if(take)goto L8c077a36;goto L8c0779d2; /* bf/s 0x8c077a36 */
L8c0779d2: *stop=0x8c0779d2u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0779d4; /* mov.l @(0x4,r15),r2 */
L8c0779d4: *stop=0x8c0779d4u;r[9]=1u;goto L8c0779d6; /* mov #0x1,r9 */
L8c0779d6: *stop=0x8c0779d8u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c077a36; /* bra 0x8c077a36 */
L8c077a20: *stop=0x8c077a20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077ba0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077a22; /* mov.l 0x8c077ba0,r2 */
L8c077a22: *stop=0x8c077a22u;r[5]=r[11];goto L8c077a24; /* mov r11,r5 */
L8c077a24: *stop=0x8c077a24u;r[7]=r[14];goto L8c077a26; /* mov r14,r7 */
L8c077a26: *stop=0x8c077a26u;r[6]=r[12];goto L8c077a28; /* mov r12,r6 */
L8c077a28: target=r[2];*pr=0x8c077a2cu;*stop=0x8c077a2au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077a2c; /* jsr @r2 */
L8c077a2c: *stop=0x8c077a2cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c077a2e; /* cmp/eq #0x1,r0 */
L8c077a2e: if(!(*sr&1u))goto L8c077a36;goto L8c077a30; /* bf 0x8c077a36 */
L8c077a30: *stop=0x8c077a30u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077a32; /* mov.l @(0x4,r15),r2 */
L8c077a32: *stop=0x8c077a32u;r[9]=1u;goto L8c077a34; /* mov #0x1,r9 */
L8c077a34: *stop=0x8c077a34u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c077a36; /* mov.l r2,@(0xc,r15) */
L8c077a36: *stop=0x8c077a36u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077a38; /* mov.l @(0x4,r15),r3 */
L8c077a38: *stop=0x8c077a38u;r[1]=70u;goto L8c077a3a; /* mov #0x46,r1 */
L8c077a3a: *stop=0x8c077a3au;r[1]<<=2;goto L8c077a3c; /* shll2 r1 */
L8c077a3c: *stop=0x8c077a3cu;r[2]=r[3];goto L8c077a3e; /* mov r3,r2 */
L8c077a3e: *stop=0x8c077a3eu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c077a40; /* shll r3 */
L8c077a40: *stop=0x8c077a40u;r[3]+=r[2];goto L8c077a42; /* add r2,r3 */
L8c077a42: *stop=0x8c077a42u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c077a44; /* mov.l @r15,r2 */
L8c077a44: *stop=0x8c077a44u;r[3]<<=2;goto L8c077a46; /* shll2 r3 */
L8c077a46: *stop=0x8c077a46u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c077a48; /* mov.l @r2,r0 */
L8c077a48: *stop=0x8c077a48u;r[1]+=r[15];goto L8c077a4a; /* add r15,r1 */
L8c077a4a: *stop=0x8c077a4au;r[3]+=r[1];goto L8c077a4c; /* add r1,r3 */
L8c077a4c: *stop=0x8c077a4cu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077a4e; /* mov.l @(0x4,r2),r1 */
L8c077a4e: *stop=0x8c077a4eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c077a50; /* mov.l r0,@r3 */
L8c077a50: *stop=0x8c077a50u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c077a52; /* mov.l r1,@(0x4,r3) */
L8c077a52: *stop=0x8c077a52u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c077a54; /* mov.l @(0x8,r2),r0 */
L8c077a54: *stop=0x8c077a54u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[0],4);if(io_status)return io_status;goto L8c077a56; /* mov.l r0,@(0x8,r3) */
L8c077a56: *stop=0x8c077a56u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c077a58; /* mov.l @(0x4,r15),r0 */
L8c077a58: *stop=0x8c077a58u;r[0]+=1u;goto L8c077a5a; /* add #0x1,r0 */
L8c077a5a: *stop=0x8c077a5au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c077a5c; /* mov.l r0,@(0x4,r15) */
L8c077a5c: *stop=0x8c077a5cu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077a5e; /* mov.l @(0x10,r15),r3 */
L8c077a5e: *stop=0x8c077a5eu;r[3]+=1u;goto L8c077a60; /* add #0x1,r3 */
L8c077a60: *stop=0x8c077a60u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c077a62; /* mov.l r3,@(0x10,r15) */
L8c077a62: *stop=0x8c077a62u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c077a64; /* mov.l @r15,r2 */
L8c077a64: *stop=0x8c077a64u;r[2]+=12u;goto L8c077a66; /* add #0xc,r2 */
L8c077a66: *stop=0x8c077a66u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c077a68; /* mov.l r2,@r15 */
L8c077a68: *stop=0x8c077a68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077ba4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077a6a; /* mov.l 0x8c077ba4,r1 */
L8c077a6a: *stop=0x8c077a6au;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077a6c; /* mov.l @(0x10,r15),r2 */
L8c077a6c: *stop=0x8c077a6cu;if(!valid_access(r[1],2u))return -3;r[3]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;goto L8c077a6e; /* mov.w @r1,r3 */
L8c077a6e: *stop=0x8c077a6eu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c077a70; /* cmp/ge r3,r2 */
L8c077a70: if(!(*sr&1u))goto L8c077978;goto L8c077a72; /* bf 0x8c077978 */
L8c077a72: *stop=0x8c077a72u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077a74; /* mov.l @(0x4,r15),r1 */
L8c077a74: *stop=0x8c077a74u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c077a76; /* tst r1,r1 */
L8c077a76: if(!(*sr&1u))goto L8c077aba;goto L8c077a78; /* bf 0x8c077aba */
L8c077a78: *stop=0x8c077a78u;r[1]=4u;goto L8c077a7a; /* mov #0x4,r1 */
L8c077a7a: *stop=0x8c077a7au;r[1]+=r[11];goto L8c077a7c; /* add r11,r1 */
L8c077a7c: *stop=0x8c077a7cu;r[0]=32u;goto L8c077a7e; /* mov #0x20,r0 */
L8c077a7e: *stop=0x8c077a7eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c077a80; /* fmov.s @r1,fr3 */
L8c077a80: *stop=0x8c077a80u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077a82; /* fmov.s @(r0,r13),fr2 */
L8c077a82: *stop=0x8c077a82u;r[1]=8u;goto L8c077a84; /* mov #0x8,r1 */
L8c077a84: *stop=0x8c077a84u;r[1]+=r[11];goto L8c077a86; /* add r11,r1 */
L8c077a86: *stop=0x8c077a86u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c077a88; /* fadd fr3,fr2 */
L8c077a88: *stop=0x8c077a88u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077a8a; /* fmov fr2,@(r0,r13) */
L8c077a8a: *stop=0x8c077a8au;r[0]=36u;goto L8c077a8c; /* mov #0x24,r0 */
L8c077a8c: *stop=0x8c077a8cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077a8e; /* fmov.s @(r0,r13),fr2 */
L8c077a8e: *stop=0x8c077a8eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c077a90; /* fmov.s @r1,fr3 */
L8c077a90: *stop=0x8c077a90u;r[1]=12u;goto L8c077a92; /* mov #0xc,r1 */
L8c077a92: *stop=0x8c077a92u;r[1]+=r[11];goto L8c077a94; /* add r11,r1 */
L8c077a94: *stop=0x8c077a94u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c077a96; /* fadd fr3,fr2 */
L8c077a96: *stop=0x8c077a96u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077a98; /* fmov fr2,@(r0,r13) */
L8c077a98: *stop=0x8c077a98u;r[0]=40u;goto L8c077a9a; /* mov #0x28,r0 */
L8c077a9a: *stop=0x8c077a9au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c077a9c; /* fmov.s @r1,fr3 */
L8c077a9c: *stop=0x8c077a9cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077a9e; /* fmov.s @(r0,r13),fr2 */
L8c077a9e: *stop=0x8c077a9eu;r[5]=r[11];goto L8c077aa0; /* mov r11,r5 */
L8c077aa0: *stop=0x8c077aa0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077ba8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077aa2; /* mov.l 0x8c077ba8,r3 */
L8c077aa2: *stop=0x8c077aa2u;r[6]=r[12];goto L8c077aa4; /* mov r12,r6 */
L8c077aa4: *stop=0x8c077aa4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c077aa6; /* fadd fr3,fr2 */
L8c077aa6: *stop=0x8c077aa6u;r[7]=0u;goto L8c077aa8; /* mov #0x0,r7 */
L8c077aa8: *stop=0x8c077aa8u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077aaa; /* fmov fr2,@(r0,r13) */
L8c077aaa: target=r[3];*pr=0x8c077aaeu;*stop=0x8c077aacu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077aae; /* jsr @r3 */
L8c077aae: *stop=0x8c077aaeu;r[0]=108u;goto L8c077ab0; /* mov #0x6c,r0 */
L8c077ab0: *stop=0x8c077ab0u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[10],4);if(io_status)return io_status;goto L8c077ab2; /* mov.l r10,@(r0,r12) */
L8c077ab2: *stop=0x8c077ab2u;r[0]=116u;goto L8c077ab4; /* mov #0x74,r0 */
L8c077ab4: *stop=0x8c077ab4u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[10],4);if(io_status)return io_status;goto L8c077ab6; /* mov.l r10,@(r0,r12) */
L8c077ab6: *stop=0x8c077ab8u;r[0]=0u;goto L8c0782e6; /* bra 0x8c0782e6 */
L8c077aba: *stop=0x8c077abau;r[0]=r[9];goto L8c077abc; /* mov r9,r0 */
L8c077abc: *stop=0x8c077abcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c077abe; /* cmp/eq #0x1,r0 */
L8c077abe: *stop=0x8c077abeu;r[4]=r[10];goto L8c077ac0; /* mov r10,r4 */
L8c077ac0: *stop=0x8c077ac0u;r[6]=r[15];goto L8c077ac2; /* mov r15,r6 */
L8c077ac2: take=!(*sr&1u);*stop=0x8c077ac4u;r[6]+=88u;if(take)goto L8c077ae6;goto L8c077ac6; /* bf/s 0x8c077ae6 */
L8c077ac6: *stop=0x8c077ac6u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077ac8; /* mov.l @(0xc,r15),r2 */
L8c077ac8: *stop=0x8c077ac8u;r[1]=70u;goto L8c077aca; /* mov #0x46,r1 */
L8c077aca: *stop=0x8c077acau;r[1]<<=2;goto L8c077acc; /* shll2 r1 */
L8c077acc: *stop=0x8c077accu;r[3]=r[2];goto L8c077ace; /* mov r2,r3 */
L8c077ace: *stop=0x8c077aceu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c077ad0; /* shll r2 */
L8c077ad0: *stop=0x8c077ad0u;r[2]+=r[3];goto L8c077ad2; /* add r3,r2 */
L8c077ad2: *stop=0x8c077ad2u;r[1]+=r[15];goto L8c077ad4; /* add r15,r1 */
L8c077ad4: *stop=0x8c077ad4u;r[2]<<=2;goto L8c077ad6; /* shll2 r2 */
L8c077ad6: *stop=0x8c077ad6u;r[2]+=r[1];goto L8c077ad8; /* add r1,r2 */
L8c077ad8: *stop=0x8c077ad8u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c077ada; /* mov.l @r2,r3 */
L8c077ada: *stop=0x8c077adau;r[4]+=1u;goto L8c077adc; /* add #0x1,r4 */
L8c077adc: *stop=0x8c077adcu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c077ade; /* mov.l @(0x4,r2),r0 */
L8c077ade: *stop=0x8c077adeu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c077ae0; /* mov.l r3,@r6 */
L8c077ae0: *stop=0x8c077ae0u;if(!valid_access(r[6]+4u,4u))return -3;wr(m,bus,&io_status,r[6]+0x4u,r[0],4);if(io_status)return io_status;goto L8c077ae2; /* mov.l r0,@(0x4,r6) */
L8c077ae2: *stop=0x8c077ae2u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077ae4; /* mov.l @(0x8,r2),r3 */
L8c077ae4: *stop=0x8c077ae4u;if(!valid_access(r[6]+8u,4u))return -3;wr(m,bus,&io_status,r[6]+0x8u,r[3],4);if(io_status)return io_status;goto L8c077ae6; /* mov.l r3,@(0x8,r6) */
L8c077ae6: *stop=0x8c077ae6u;r[5]=r[4];goto L8c077ae8; /* mov r4,r5 */
L8c077ae8: *stop=0x8c077ae8u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c077aea; /* shll r5 */
L8c077aea: *stop=0x8c077aeau;r[3]=r[4];goto L8c077aec; /* mov r4,r3 */
L8c077aec: *stop=0x8c077aecu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077aee; /* mov.l @(0x4,r15),r2 */
L8c077aee: *stop=0x8c077aeeu;r[0]=108u;goto L8c077af0; /* mov #0x6c,r0 */
L8c077af0: *stop=0x8c077af0u;r[5]+=r[3];goto L8c077af2; /* add r3,r5 */
L8c077af2: *stop=0x8c077af2u;r[2]+=4294967295u;goto L8c077af4; /* add #-0x1,r2 */
L8c077af4: *stop=0x8c077af4u;r[5]<<=2;goto L8c077af6; /* shll2 r5 */
L8c077af6: *stop=0x8c077af6u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c077af8; /* mov.l r2,@(0x8,r15) */
L8c077af8: *stop=0x8c077af8u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c077afa; /* mov.l @(r0,r12),r3 */
L8c077afa: *stop=0x8c077afau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c077afc; /* tst r3,r3 */
L8c077afc: take=(*sr&1u);*stop=0x8c077afeu;r[5]+=r[6];if(take)goto L8c077b74;goto L8c077b00; /* bt/s 0x8c077b74 */
L8c077b00: *stop=0x8c077b00u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077b02; /* mov.l @(0x8,r15),r3 */
L8c077b02: *stop=0x8c077b02u;r[4]=r[10];goto L8c077b04; /* mov r10,r4 */
L8c077b04: *stop=0x8c077b04u;r[2]=r[3];goto L8c077b06; /* mov r3,r2 */
L8c077b06: *stop=0x8c077b06u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c077b08; /* shll r3 */
L8c077b08: *stop=0x8c077b08u;r[3]+=r[2];goto L8c077b0a; /* add r2,r3 */
L8c077b0a: *stop=0x8c077b0au;r[3]<<=2;goto L8c077b0c; /* shll2 r3 */
L8c077b0c: *stop=0x8c077b0cu;r[3]+=r[6];goto L8c077b0e; /* add r6,r3 */
L8c077b0e: *stop=0x8c077b0eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c077b10; /* mov.l r3,@r15 */
L8c077b10: *stop=0x8c077b10u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077b12; /* mov.l @(0x4,r15),r2 */
L8c077b12: *stop=0x8c077b12u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c077b14; /* cmp/pl r2 */
L8c077b14: take=(*sr&1u);*stop=0x8c077b16u;r[7]=r[5];if(take)goto L8c077b1c;goto L8c077b18; /* bt/s 0x8c077b1c */
L8c077b18: *stop=0x8c077b1au;goto L8c077c02; /* bra 0x8c077c02 */
L8c077b1c: *stop=0x8c077b1cu;r[0]=r[9];goto L8c077b1e; /* mov r9,r0 */
L8c077b1e: *stop=0x8c077b1eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c077b20; /* cmp/eq #0x1,r0 */
L8c077b20: if(!(*sr&1u))goto L8c077b2c;goto L8c077b22; /* bf 0x8c077b2c */
L8c077b22: *stop=0x8c077b22u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077b24; /* mov.l @(0xc,r15),r2 */
L8c077b24: *stop=0x8c077b24u;*sr=(*sr&~1u)|(r[2]==r[4]);goto L8c077b26; /* cmp/eq r2,r4 */
L8c077b26: if(!(*sr&1u))goto L8c077b2c;goto L8c077b28; /* bf 0x8c077b2c */
L8c077b28: *stop=0x8c077b2au;r[9]=r[10];goto L8c077b68; /* bra 0x8c077b68 */
L8c077b2c: *stop=0x8c077b2cu;r[5]=r[4];goto L8c077b2e; /* mov r4,r5 */
L8c077b2e: *stop=0x8c077b2eu;r[2]=70u;goto L8c077b30; /* mov #0x46,r2 */
L8c077b30: *stop=0x8c077b30u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c077b32; /* shll r5 */
L8c077b32: *stop=0x8c077b32u;r[3]=r[4];goto L8c077b34; /* mov r4,r3 */
L8c077b34: *stop=0x8c077b34u;r[2]<<=2;goto L8c077b36; /* shll2 r2 */
L8c077b36: *stop=0x8c077b36u;r[5]+=r[3];goto L8c077b38; /* add r3,r5 */
L8c077b38: *stop=0x8c077b38u;r[2]+=r[15];goto L8c077b3a; /* add r15,r2 */
L8c077b3a: *stop=0x8c077b3au;r[5]<<=2;goto L8c077b3c; /* shll2 r5 */
L8c077b3c: *stop=0x8c077b3cu;r[0]=116u;goto L8c077b3e; /* mov #0x74,r0 */
L8c077b3e: *stop=0x8c077b3eu;r[5]+=r[2];goto L8c077b40; /* add r2,r5 */
L8c077b40: *stop=0x8c077b40u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c077b42; /* mov.l @(r0,r12),r1 */
L8c077b42: *stop=0x8c077b42u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077b44; /* mov.l @(0x4,r5),r3 */
L8c077b44: *stop=0x8c077b44u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c077b46; /* cmp/eq r3,r1 */
L8c077b46: if(!(*sr&1u))goto L8c077b5a;goto L8c077b48; /* bf 0x8c077b5a */
L8c077b48: *stop=0x8c077b48u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[1]=loaded;}goto L8c077b4a; /* mov.l @r5,r1 */
L8c077b4a: *stop=0x8c077b4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c077b4c; /* mov.l @r15,r3 */
L8c077b4c: *stop=0x8c077b4cu;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c077b4e; /* mov.l @(0x4,r5),r0 */
L8c077b4e: *stop=0x8c077b4eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c077b50; /* mov.l r1,@r3 */
L8c077b50: *stop=0x8c077b50u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c077b52; /* mov.l r0,@(0x4,r3) */
L8c077b52: *stop=0x8c077b52u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077b54; /* mov.l @(0x8,r5),r1 */
L8c077b54: *stop=0x8c077b54u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c077b56; /* mov.l r1,@(0x8,r3) */
L8c077b56: *stop=0x8c077b58u;goto L8c077b68; /* bra 0x8c077b68 */
L8c077b5a: *stop=0x8c077b5au;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c077b5c; /* mov.l @r5,r3 */
L8c077b5c: *stop=0x8c077b5cu;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077b5e; /* mov.l @(0x4,r5),r1 */
L8c077b5e: *stop=0x8c077b5eu;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[3],4);if(io_status)return io_status;goto L8c077b60; /* mov.l r3,@r7 */
L8c077b60: *stop=0x8c077b60u;if(!valid_access(r[7]+4u,4u))return -3;wr(m,bus,&io_status,r[7]+0x4u,r[1],4);if(io_status)return io_status;goto L8c077b62; /* mov.l r1,@(0x4,r7) */
L8c077b62: *stop=0x8c077b62u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077b64; /* mov.l @(0x8,r5),r3 */
L8c077b64: *stop=0x8c077b64u;if(!valid_access(r[7]+8u,4u))return -3;wr(m,bus,&io_status,r[7]+0x8u,r[3],4);if(io_status)return io_status;goto L8c077b66; /* mov.l r3,@(0x8,r7) */
L8c077b66: *stop=0x8c077b66u;r[7]+=12u;goto L8c077b68; /* add #0xc,r7 */
L8c077b68: *stop=0x8c077b68u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077b6a; /* mov.l @(0x4,r15),r3 */
L8c077b6a: *stop=0x8c077b6au;r[4]+=1u;goto L8c077b6c; /* add #0x1,r4 */
L8c077b6c: *stop=0x8c077b6cu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c077b6e; /* cmp/ge r3,r4 */
L8c077b6e: if(!(*sr&1u))goto L8c077b1c;goto L8c077b70; /* bf 0x8c077b1c */
L8c077b70: *stop=0x8c077b72u;goto L8c077c02; /* bra 0x8c077c02 */
L8c077b74: *stop=0x8c077b74u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c077b76; /* mov.l @(0x8,r15),r4 */
L8c077b76: *stop=0x8c077b76u;r[7]=r[5];goto L8c077b78; /* mov r5,r7 */
L8c077b78: *stop=0x8c077b78u;r[3]=r[4];goto L8c077b7a; /* mov r4,r3 */
L8c077b7a: *stop=0x8c077b7au;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c077b7c; /* shll r3 */
L8c077b7c: *stop=0x8c077b7cu;r[2]=r[4];goto L8c077b7e; /* mov r4,r2 */
L8c077b7e: *stop=0x8c077b7eu;r[3]+=r[2];goto L8c077b80; /* add r2,r3 */
L8c077b80: *stop=0x8c077b80u;r[3]<<=2;goto L8c077b82; /* shll2 r3 */
L8c077b82: *stop=0x8c077b82u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c077b84; /* cmp/pz r4 */
L8c077b84: *stop=0x8c077b84u;r[3]+=r[6];goto L8c077b86; /* add r6,r3 */
L8c077b86: take=!(*sr&1u);*stop=0x8c077b88u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c077c02;goto L8c077b8a; /* bf/s 0x8c077c02 */
L8c077b8a: *stop=0x8c077b8au;r[0]=r[9];goto L8c077b8c; /* mov r9,r0 */
L8c077b8c: *stop=0x8c077b8cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c077b8e; /* cmp/eq #0x1,r0 */
L8c077b8e: if(!(*sr&1u))goto L8c077bc0;goto L8c077b90; /* bf 0x8c077bc0 */
L8c077b90: *stop=0x8c077b90u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077b92; /* mov.l @(0xc,r15),r2 */
L8c077b92: *stop=0x8c077b92u;*sr=(*sr&~1u)|(r[2]==r[4]);goto L8c077b94; /* cmp/eq r2,r4 */
L8c077b94: if(!(*sr&1u))goto L8c077bc0;goto L8c077b96; /* bf 0x8c077bc0 */
L8c077b96: *stop=0x8c077b98u;r[9]=r[10];goto L8c077bfc; /* bra 0x8c077bfc */
L8c077bc0: *stop=0x8c077bc0u;r[5]=r[4];goto L8c077bc2; /* mov r4,r5 */
L8c077bc2: *stop=0x8c077bc2u;r[2]=70u;goto L8c077bc4; /* mov #0x46,r2 */
L8c077bc4: *stop=0x8c077bc4u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c077bc6; /* shll r5 */
L8c077bc6: *stop=0x8c077bc6u;r[3]=r[4];goto L8c077bc8; /* mov r4,r3 */
L8c077bc8: *stop=0x8c077bc8u;r[2]<<=2;goto L8c077bca; /* shll2 r2 */
L8c077bca: *stop=0x8c077bcau;r[5]+=r[3];goto L8c077bcc; /* add r3,r5 */
L8c077bcc: *stop=0x8c077bccu;r[2]+=r[15];goto L8c077bce; /* add r15,r2 */
L8c077bce: *stop=0x8c077bceu;r[5]<<=2;goto L8c077bd0; /* shll2 r5 */
L8c077bd0: *stop=0x8c077bd0u;r[0]=116u;goto L8c077bd2; /* mov #0x74,r0 */
L8c077bd2: *stop=0x8c077bd2u;r[5]+=r[2];goto L8c077bd4; /* add r2,r5 */
L8c077bd4: *stop=0x8c077bd4u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c077bd6; /* mov.l @(r0,r12),r1 */
L8c077bd6: *stop=0x8c077bd6u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077bd8; /* mov.l @(0x4,r5),r3 */
L8c077bd8: *stop=0x8c077bd8u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c077bda; /* cmp/eq r3,r1 */
L8c077bda: if(!(*sr&1u))goto L8c077bee;goto L8c077bdc; /* bf 0x8c077bee */
L8c077bdc: *stop=0x8c077bdcu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[1]=loaded;}goto L8c077bde; /* mov.l @r5,r1 */
L8c077bde: *stop=0x8c077bdeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c077be0; /* mov.l @r15,r3 */
L8c077be0: *stop=0x8c077be0u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c077be2; /* mov.l @(0x4,r5),r0 */
L8c077be2: *stop=0x8c077be2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c077be4; /* mov.l r1,@r3 */
L8c077be4: *stop=0x8c077be4u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c077be6; /* mov.l r0,@(0x4,r3) */
L8c077be6: *stop=0x8c077be6u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077be8; /* mov.l @(0x8,r5),r1 */
L8c077be8: *stop=0x8c077be8u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c077bea; /* mov.l r1,@(0x8,r3) */
L8c077bea: *stop=0x8c077becu;goto L8c077bfc; /* bra 0x8c077bfc */
L8c077bee: *stop=0x8c077beeu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c077bf0; /* mov.l @r5,r3 */
L8c077bf0: *stop=0x8c077bf0u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077bf2; /* mov.l @(0x4,r5),r1 */
L8c077bf2: *stop=0x8c077bf2u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[3],4);if(io_status)return io_status;goto L8c077bf4; /* mov.l r3,@r7 */
L8c077bf4: *stop=0x8c077bf4u;if(!valid_access(r[7]+4u,4u))return -3;wr(m,bus,&io_status,r[7]+0x4u,r[1],4);if(io_status)return io_status;goto L8c077bf6; /* mov.l r1,@(0x4,r7) */
L8c077bf6: *stop=0x8c077bf6u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077bf8; /* mov.l @(0x8,r5),r3 */
L8c077bf8: *stop=0x8c077bf8u;if(!valid_access(r[7]+8u,4u))return -3;wr(m,bus,&io_status,r[7]+0x8u,r[3],4);if(io_status)return io_status;goto L8c077bfa; /* mov.l r3,@(0x8,r7) */
L8c077bfa: *stop=0x8c077bfau;r[7]+=12u;goto L8c077bfc; /* add #0xc,r7 */
L8c077bfc: *stop=0x8c077bfcu;r[4]+=4294967295u;goto L8c077bfe; /* add #-0x1,r4 */
L8c077bfe: *stop=0x8c077bfeu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c077c00; /* cmp/pz r4 */
L8c077c00: if((*sr&1u))goto L8c077b8a;goto L8c077c02; /* bt 0x8c077b8a */
L8c077c02: *stop=0x8c077c02u;r[0]=124u;goto L8c077c04; /* mov #0x7c,r0 */
L8c077c04: *stop=0x8c077c04u;r[0]<<=2;goto L8c077c06; /* shll2 r0 */
L8c077c06: *stop=0x8c077c06u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c077c08; /* fmov.s @(r0,r15),fr3 */
L8c077c08: *stop=0x8c077c08u;r[0]=32u;goto L8c077c0a; /* mov #0x20,r0 */
L8c077c0a: *stop=0x8c077c0au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077c0c; /* fmov.s @(r0,r13),fr2 */
L8c077c0c: *stop=0x8c077c0cu;r[0]=4u;goto L8c077c0e; /* mov #0x4,r0 */
L8c077c0e: *stop=0x8c077c0eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c077c10; /* fadd fr3,fr2 */
L8c077c10: *stop=0x8c077c10u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077c12; /* fmov fr2,@(r0,r14) */
L8c077c12: *stop=0x8c077c12u;r[0]=125u;goto L8c077c14; /* mov #0x7d,r0 */
L8c077c14: *stop=0x8c077c14u;r[0]<<=2;goto L8c077c16; /* shll2 r0 */
L8c077c16: *stop=0x8c077c16u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c077c18; /* fmov.s @(r0,r15),fr3 */
L8c077c18: *stop=0x8c077c18u;r[0]=36u;goto L8c077c1a; /* mov #0x24,r0 */
L8c077c1a: *stop=0x8c077c1au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077c1c; /* fmov.s @(r0,r13),fr2 */
L8c077c1c: *stop=0x8c077c1cu;r[0]=8u;goto L8c077c1e; /* mov #0x8,r0 */
L8c077c1e: *stop=0x8c077c1eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c077c20; /* fadd fr3,fr2 */
L8c077c20: *stop=0x8c077c20u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077c22; /* fmov fr2,@(r0,r14) */
L8c077c22: *stop=0x8c077c22u;r[0]=126u;goto L8c077c24; /* mov #0x7e,r0 */
L8c077c24: *stop=0x8c077c24u;r[0]<<=2;goto L8c077c26; /* shll2 r0 */
L8c077c26: *stop=0x8c077c26u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c077c28; /* fmov.s @(r0,r15),fr3 */
L8c077c28: *stop=0x8c077c28u;r[0]=40u;goto L8c077c2a; /* mov #0x28,r0 */
L8c077c2a: *stop=0x8c077c2au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c077c2c; /* fmov.s @(r0,r13),fr2 */
L8c077c2c: *stop=0x8c077c2cu;r[0]=12u;goto L8c077c2e; /* mov #0xc,r0 */
L8c077c2e: *stop=0x8c077c2eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c077c30; /* fadd fr3,fr2 */
L8c077c30: *stop=0x8c077c30u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077c32; /* fmov fr2,@(r0,r14) */
L8c077c32: *stop=0x8c077c32u;r[0]=108u;goto L8c077c34; /* mov #0x6c,r0 */
L8c077c34: *stop=0x8c077c34u;if(!valid_access(r[11]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077c36; /* mov.l @(0x4,r11),r3 */
L8c077c36: *stop=0x8c077c36u;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077c38; /* mov.l @(0x8,r11),r2 */
L8c077c38: *stop=0x8c077c38u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c077c3a; /* mov.l r3,@(0x10,r14) */
L8c077c3a: *stop=0x8c077c3au;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[2],4);if(io_status)return io_status;goto L8c077c3c; /* mov.l r2,@(0x14,r14) */
L8c077c3c: *stop=0x8c077c3cu;if(!valid_access(r[11]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077c3e; /* mov.l @(0xc,r11),r3 */
L8c077c3e: *stop=0x8c077c3eu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c077c40; /* mov.l r3,@(0x18,r14) */
L8c077c40: *stop=0x8c077c40u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[10],4);if(io_status)return io_status;goto L8c077c42; /* mov.l r10,@(r0,r12) */
L8c077c42: *stop=0x8c077c42u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c077c44; /* mov.l r6,@(0x10,r15) */
L8c077c44: *stop=0x8c077c44u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c077c46; /* mov.l r10,@r15 */
L8c077c46: *stop=0x8c077c46u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c077c48; /* mov.l @(0x14,r15),r0 */
L8c077c48: *stop=0x8c077c48u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c077c4a; /* tst #0x3,r0 */
L8c077c4a: *stop=0x8c077c4au;r[0]=*sr&1u;goto L8c077c4c; /* movt r0 */
L8c077c4c: *stop=0x8c077c4cu;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c077c4e; /* mov.l r0,@(0x1c,r15) */
L8c077c4e: *stop=0x8c077c4eu;r[0]=0x8c077d80u;goto L8c077c50; /* mova 0x8c077d80,r0 */
L8c077c50: *stop=0x8c077c50u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077c52; /* mov.l @(0x8,r15),r3 */
L8c077c52: *stop=0x8c077c52u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077c54; /* mov.l @(0x10,r15),r1 */
L8c077c54: *stop=0x8c077c54u;r[2]=r[3];goto L8c077c56; /* mov r3,r2 */
L8c077c56: *stop=0x8c077c56u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c077c58; /* shll r3 */
L8c077c58: *stop=0x8c077c58u;r[3]+=r[2];goto L8c077c5a; /* add r2,r3 */
L8c077c5a: *stop=0x8c077c5au;r[3]<<=2;goto L8c077c5c; /* shll2 r3 */
L8c077c5c: *stop=0x8c077c5cu;r[3]+=r[1];goto L8c077c5e; /* add r1,r3 */
L8c077c5e: *stop=0x8c077c5eu;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c077c60; /* mov.l r3,@(0x14,r15) */
L8c077c60: *stop=0x8c077c60u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c077c62; /* fmov.s @r0,fr12 */
L8c077c62: *stop=0x8c077c62u;r[0]=0x8c077d84u;goto L8c077c64; /* mova 0x8c077d84,r0 */
L8c077c64: *stop=0x8c077c66u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]);if(io_status)return io_status;goto L8c0781ce; /* bra 0x8c0781ce */
L8c077c68: *stop=0x8c077c68u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}goto L8c077c6a; /* mov.l @r15,r9 */
L8c077c6a: *stop=0x8c077c6au;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077c6c; /* mov.l @(0x10,r15),r2 */
L8c077c6c: *stop=0x8c077c6cu;r[3]=r[9];goto L8c077c6e; /* mov r9,r3 */
L8c077c6e: *stop=0x8c077c6eu;*sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c077c70; /* shll r9 */
L8c077c70: *stop=0x8c077c70u;r[9]+=r[3];goto L8c077c72; /* add r3,r9 */
L8c077c72: *stop=0x8c077c72u;r[9]<<=2;goto L8c077c74; /* shll2 r9 */
L8c077c74: *stop=0x8c077c74u;r[9]+=r[2];goto L8c077c76; /* add r2,r9 */
L8c077c76: *stop=0x8c077c76u;if(!valid_access(r[9]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077c78; /* mov.l @(0x8,r9),r3 */
L8c077c78: *stop=0x8c077c78u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c077c7a; /* mov.l r3,@(0xc,r15) */
L8c077c7a: *stop=0x8c077c7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c077d88u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077c7c; /* mov.l 0x8c077d88,r3 */
L8c077c7c: *stop=0x8c077c7cu;if(!valid_access(r[9]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c077c7e; /* mov.l @(0x4,r9),r5 */
L8c077c7e: *stop=0x8c077c7eu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[6]=loaded;}goto L8c077c80; /* mov.l @r9,r6 */
L8c077c80: target=r[3];*pr=0x8c077c84u;*stop=0x8c077c82u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077c84; /* jsr @r3 */
L8c077c84: *stop=0x8c077c84u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c077c86; /* tst r0,r0 */
L8c077c86: if(!(*sr&1u))goto L8c077c8c;goto L8c077c88; /* bf 0x8c077c8c */
L8c077c88: *stop=0x8c077c8au;goto L8c0781c8; /* bra 0x8c0781c8 */
L8c077c8c: *stop=0x8c077c8cu;r[5]=r[14];goto L8c077c8e; /* mov r14,r5 */
L8c077c8e: *stop=0x8c077c8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c077d8cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077c90; /* mov.l 0x8c077d8c,r2 */
L8c077c90: *stop=0x8c077c90u;r[4]=r[14];goto L8c077c92; /* mov r14,r4 */
L8c077c92: *stop=0x8c077c92u;r[6]=0u;goto L8c077c94; /* mov #0x0,r6 */
L8c077c94: *stop=0x8c077c94u;r[5]+=72u;goto L8c077c96; /* add #0x48,r5 */
L8c077c96: target=r[2];*pr=0x8c077c9au;*stop=0x8c077c98u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077c9a; /* jsr @r2 */
L8c077c9a: *stop=0x8c077c9au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c077c9c; /* mov.l r0,@(0x8,r15) */
L8c077c9c: *stop=0x8c077c9cu;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c077c9e; /* mov.w @(0x4,r13),r0 */
L8c077c9e: *stop=0x8c077c9eu;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c077ca0; /* tst #0x2,r0 */
L8c077ca0: if((*sr&1u))goto L8c077ca6;goto L8c077ca2; /* bt 0x8c077ca6 */
L8c077ca2: *stop=0x8c077ca4u;goto L8c0781a0; /* bra 0x8c0781a0 */
L8c077ca6: *stop=0x8c077ca6u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c077ca8; /* mov.l @r9,r2 */
L8c077ca8: *stop=0x8c077ca8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077d90u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077caa; /* mov.l 0x8c077d90,r3 */
L8c077caa: *stop=0x8c077caau;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c077cac; /* tst r3,r2 */
L8c077cac: if((*sr&1u))goto L8c077cda;goto L8c077cae; /* bt 0x8c077cda */
L8c077cae: *stop=0x8c077caeu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[1]=loaded;}goto L8c077cb0; /* mov.l @r9,r1 */
L8c077cb0: *stop=0x8c077cb0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077d94u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077cb2; /* mov.l 0x8c077d94,r2 */
L8c077cb2: *stop=0x8c077cb2u;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c077cb4; /* tst r2,r1 */
L8c077cb4: if(!(*sr&1u))goto L8c077cba;goto L8c077cb6; /* bf 0x8c077cba */
L8c077cb6: *stop=0x8c077cb8u;goto L8c0781a0; /* bra 0x8c0781a0 */
L8c077cba: *stop=0x8c077cbau;{uint32_t loaded=rd(m,bus,&io_status,0x8c077d98u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077cbc; /* mov.l 0x8c077d98,r1 */
L8c077cbc: target=r[1];*pr=0x8c077cc0u;*stop=0x8c077cbeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077cc0; /* jsr @r1 */
L8c077cc0: *stop=0x8c077cc0u;r[0]=r[0]&255u;goto L8c077cc2; /* extu.b r0,r0 */
L8c077cc2: *stop=0x8c077cc2u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c077cc4; /* cmp/eq #0x3,r0 */
L8c077cc4: if((*sr&1u))goto L8c077cca;goto L8c077cc6; /* bt 0x8c077cca */
L8c077cc6: *stop=0x8c077cc8u;goto L8c0781a0; /* bra 0x8c0781a0 */
L8c077cca: *stop=0x8c077ccau;r[0]=76u;goto L8c077ccc; /* mov #0x4c,r0 */
L8c077ccc: *stop=0x8c077cccu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c077cce; /* fmov.s @(r0,r14),fr3 */
L8c077cce: *stop=0x8c077cceu;r[0]=0x8c077d9cu;goto L8c077cd0; /* mova 0x8c077d9c,r0 */
L8c077cd0: *stop=0x8c077cd0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c077cd2; /* fmov.s @r0,fr2 */
L8c077cd2: *stop=0x8c077cd2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c077cd4; /* fcmp/gt fr2,fr3 */
L8c077cd4: if((*sr&1u))goto L8c077cda;goto L8c077cd6; /* bt 0x8c077cda */
L8c077cd6: *stop=0x8c077cd8u;goto L8c0781a0; /* bra 0x8c0781a0 */
L8c077cda: *stop=0x8c077cdau;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077cdc; /* mov.l @(0x8,r15),r3 */
L8c077cdc: *stop=0x8c077cdcu;r[2]=44u;goto L8c077cde; /* mov #0x2c,r2 */
L8c077cde: *stop=0x8c077cdeu;r[2]<<=8;goto L8c077ce0; /* shll8 r2 */
L8c077ce0: *stop=0x8c077ce0u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[2]);goto L8c077ce2; /* cmp/gt r2,r3 */
L8c077ce2: if(!(*sr&1u))goto L8c077cf4;goto L8c077ce4; /* bf 0x8c077cf4 */
L8c077ce4: *stop=0x8c077ce4u;r[0]=76u;goto L8c077ce6; /* mov #0x4c,r0 */
L8c077ce6: *stop=0x8c077ce6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c077ce8; /* fmov.s @(r0,r14),fr3 */
L8c077ce8: *stop=0x8c077ce8u;r[0]=32u;goto L8c077cea; /* mov #0x20,r0 */
L8c077cea: *stop=0x8c077ceau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077cec; /* fmov.s @(r0,r14),fr2 */
L8c077cec: *stop=0x8c077cecu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c077cee; /* fcmp/gt fr2,fr3 */
L8c077cee: if((*sr&1u))goto L8c077cf4;goto L8c077cf0; /* bt 0x8c077cf4 */
L8c077cf0: *stop=0x8c077cf2u;goto L8c0781a0; /* bra 0x8c0781a0 */
L8c077cf4: *stop=0x8c077cf4u;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077cf6; /* mov.l @(0x1c,r15),r2 */
L8c077cf6: *stop=0x8c077cf6u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c077cf8; /* tst r2,r2 */
L8c077cf8: if(!(*sr&1u))goto L8c077d04;goto L8c077cfa; /* bf 0x8c077d04 */
L8c077cfa: *stop=0x8c077cfau;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c077cfc; /* mov.w @(0x10,r12),r0 */
L8c077cfc: *stop=0x8c077cfcu;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c077cfe; /* tst #0x3,r0 */
L8c077cfe: if(!(*sr&1u))goto L8c077d04;goto L8c077d00; /* bf 0x8c077d04 */
L8c077d00: *stop=0x8c077d02u;goto L8c0781a0; /* bra 0x8c0781a0 */
L8c077d04: *stop=0x8c077d04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077da4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077d06; /* mov.l 0x8c077da4,r2 */
L8c077d06: *stop=0x8c077d06u;r[5]=r[14];goto L8c077d08; /* mov r14,r5 */
L8c077d08: *stop=0x8c077d08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077da0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c077d0a; /* mov.l 0x8c077da0,r4 */
L8c077d0a: target=r[2];*pr=0x8c077d0eu;*stop=0x8c077d0cu;r[5]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077d0e; /* jsr @r2 */
L8c077d0e: *stop=0x8c077d0eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c077d10; /* fldi0 fr3 */
L8c077d10: *stop=0x8c077d10u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c077d12; /* fcmp/gt fr3,fr0 */
L8c077d12: if(!(*sr&1u))goto L8c077d1e;goto L8c077d14; /* bf 0x8c077d1e */
L8c077d14: *stop=0x8c077d14u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c077d16; /* mov.w @(0x10,r12),r0 */
L8c077d16: *stop=0x8c077d16u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c077d18; /* tst #0x3,r0 */
L8c077d18: if(!(*sr&1u))goto L8c077d1e;goto L8c077d1a; /* bf 0x8c077d1e */
L8c077d1a: *stop=0x8c077d1cu;goto L8c0781a0; /* bra 0x8c0781a0 */
L8c077d1e: *stop=0x8c077d1eu;r[0]=60u;goto L8c077d20; /* mov #0x3c,r0 */
L8c077d20: *stop=0x8c077d20u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c077d22; /* fldi0 fr3 */
L8c077d22: *stop=0x8c077d22u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c077d24; /* fmov.s @(r0,r12),fr4 */
L8c077d24: *stop=0x8c077d24u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c077d26; /* fcmp/gt fr3,fr4 */
L8c077d26: if(!(*sr&1u))goto L8c077d3a;goto L8c077d28; /* bf 0x8c077d3a */
L8c077d28: *stop=0x8c077d28u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c077d2a; /* fcmp/gt fr3,fr4 */
L8c077d2a: if((*sr&1u))goto L8c077d30;goto L8c077d2c; /* bt 0x8c077d30 */
L8c077d2c: *stop=0x8c077d2eu;goto L8c078150; /* bra 0x8c078150 */
L8c077d30: *stop=0x8c077d30u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c077d32; /* mov.w @(0x10,r12),r0 */
L8c077d32: *stop=0x8c077d32u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c077d34; /* tst #0x3,r0 */
L8c077d34: if((*sr&1u))goto L8c077d3a;goto L8c077d36; /* bt 0x8c077d3a */
L8c077d36: *stop=0x8c077d38u;goto L8c078150; /* bra 0x8c078150 */
L8c077d3a: *stop=0x8c077d3au;r[2]=1u;goto L8c077d3c; /* mov #0x1,r2 */
L8c077d3c: *stop=0x8c077d3cu;r[5]=r[11];goto L8c077d3e; /* mov r11,r5 */
L8c077d3e: *stop=0x8c077d3eu;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[2],4);if(io_status)return io_status;goto L8c077d40; /* mov.l r2,@(0x20,r15) */
L8c077d40: *stop=0x8c077d40u;r[6]=r[12];goto L8c077d42; /* mov r12,r6 */
L8c077d42: *stop=0x8c077d42u;if(!valid_access(r[9]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077d44; /* mov.l @(0x4,r9),r3 */
L8c077d44: *stop=0x8c077d44u;r[7]=r[14];goto L8c077d46; /* mov r14,r7 */
L8c077d46: *stop=0x8c077d46u;if(!valid_access(r[15]+36u,4u))return -3;wr(m,bus,&io_status,r[15]+0x24u,r[3],4);if(io_status)return io_status;goto L8c077d48; /* mov.l r3,@(0x24,r15) */
L8c077d48: *stop=0x8c077d48u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c077d4a; /* mov.l @r9,r2 */
L8c077d4a: *stop=0x8c077d4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c077da8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077d4c; /* mov.l 0x8c077da8,r3 */
L8c077d4c: *stop=0x8c077d4cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c077d4e; /* mov.l r2,@-r15 */
L8c077d4e: target=r[3];*pr=0x8c077d52u;*stop=0x8c077d50u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077d52; /* jsr @r3 */
L8c077d52: *stop=0x8c077d52u;r[4]=r[0];goto L8c077d54; /* mov r0,r4 */
L8c077d54: *stop=0x8c077d54u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c077d56; /* tst r4,r4 */
L8c077d56: take=(*sr&1u);*stop=0x8c077d58u;r[15]+=4u;if(take)goto L8c077dc8;goto L8c077d5a; /* bt/s 0x8c077dc8 */
L8c077d5a: *stop=0x8c077d5au;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c077d5c; /* tst r8,r8 */
L8c077d5c: if(!(*sr&1u))goto L8c077dc0;goto L8c077d5e; /* bf 0x8c077dc0 */
L8c077d5e: *stop=0x8c077d60u;r[8]=r[4];goto L8c077dc8; /* bra 0x8c077dc8 */
L8c077dc0: *stop=0x8c077dc0u;r[0]=r[4];goto L8c077dc2; /* mov r4,r0 */
L8c077dc2: *stop=0x8c077dc2u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c077dc4; /* cmp/eq #0x2,r0 */
L8c077dc4: if(!(*sr&1u))goto L8c077dc8;goto L8c077dc6; /* bf 0x8c077dc8 */
L8c077dc6: *stop=0x8c077dc6u;r[8]=r[4];goto L8c077dc8; /* mov r4,r8 */
L8c077dc8: *stop=0x8c077dc8u;r[0]=76u;goto L8c077dca; /* mov #0x4c,r0 */
L8c077dca: *stop=0x8c077dcau;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fa0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077dcc; /* mov.l 0x8c077fa0,r3 */
L8c077dcc: *stop=0x8c077dccu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c077dce; /* fmov.s @(r0,r14),fr5 */
L8c077dce: *stop=0x8c077dceu;r[0]=72u;goto L8c077dd0; /* mov #0x48,r0 */
L8c077dd0: target=r[3];*pr=0x8c077dd4u;*stop=0x8c077dd2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077dd4; /* jsr @r3 */
L8c077dd4: *stop=0x8c077dd4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[13],&f->fpscr,&fr[0]);if(status)return status;}goto L8c077dd6; /* fmul fr13,fr0 */
L8c077dd6: *stop=0x8c077dd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fa4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077dd8; /* mov.l 0x8c077fa4,r3 */
L8c077dd8: *stop=0x8c077dd8u;r[0]=80u;goto L8c077dda; /* mov #0x50,r0 */
L8c077dda: *stop=0x8c077ddau;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c077ddc; /* ftrc fr0,FPUL */
L8c077ddc: *stop=0x8c077ddcu;r[2]=*fpul;goto L8c077dde; /* sts FPUL,r2 */
L8c077dde: *stop=0x8c077ddeu;r[2]=0u-r[2];goto L8c077de0; /* neg r2,r2 */
L8c077de0: *stop=0x8c077de0u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[2],4);if(io_status)return io_status;goto L8c077de2; /* mov.l r2,@(0x38,r14) */
L8c077de2: target=r[3];*pr=0x8c077de6u;*stop=0x8c077de4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077de6; /* jsr @r3 */
L8c077de6: *stop=0x8c077de6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[13],&f->fpscr,&fr[0]);if(status)return status;}goto L8c077de8; /* fmul fr13,fr0 */
L8c077de8: *stop=0x8c077de8u;r[0]=4u;goto L8c077dea; /* mov #0x4,r0 */
L8c077dea: *stop=0x8c077deau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c077dec; /* fldi0 fr3 */
L8c077dec: *stop=0x8c077decu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c077dee; /* ftrc fr0,FPUL */
L8c077dee: *stop=0x8c077deeu;r[2]=*fpul;goto L8c077df0; /* sts FPUL,r2 */
L8c077df0: *stop=0x8c077df0u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[2],4);if(io_status)return io_status;goto L8c077df2; /* mov.l r2,@(0x34,r14) */
L8c077df2: *stop=0x8c077df2u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c077df4; /* fmov.s @(r0,r11),fr2 */
L8c077df4: *stop=0x8c077df4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c077df6; /* fcmp/eq fr3,fr2 */
L8c077df6: if(!(*sr&1u))goto L8c077e22;goto L8c077df8; /* bf 0x8c077e22 */
L8c077df8: *stop=0x8c077df8u;r[0]=12u;goto L8c077dfa; /* mov #0xc,r0 */
L8c077dfa: *stop=0x8c077dfau;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c077dfc; /* fmov.s @(r0,r11),fr2 */
L8c077dfc: *stop=0x8c077dfcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c077dfe; /* fcmp/eq fr3,fr2 */
L8c077dfe: if(!(*sr&1u))goto L8c077e22;goto L8c077e00; /* bf 0x8c077e22 */
L8c077e00: *stop=0x8c077e00u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c077e02; /* mov.w @(0x10,r12),r0 */
L8c077e02: *stop=0x8c077e02u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c077e04; /* tst #0x1,r0 */
L8c077e04: if((*sr&1u))goto L8c077e22;goto L8c077e06; /* bt 0x8c077e22 */
L8c077e06: *stop=0x8c077e06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077facu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077e08; /* mov.l 0x8c077fac,r2 */
L8c077e08: *stop=0x8c077e08u;r[5]=r[14];goto L8c077e0a; /* mov r14,r5 */
L8c077e0a: *stop=0x8c077e0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fa8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c077e0c; /* mov.l 0x8c077fa8,r4 */
L8c077e0c: target=r[2];*pr=0x8c077e10u;*stop=0x8c077e0eu;r[5]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077e10; /* jsr @r2 */
L8c077e10: *stop=0x8c077e10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fa8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c077e12; /* mov.l 0x8c077fa8,r4 */
L8c077e12: *stop=0x8c077e12u;r[5]=r[14];goto L8c077e14; /* mov r14,r5 */
L8c077e14: *stop=0x8c077e14u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077facu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077e16; /* mov.l 0x8c077fac,r3 */
L8c077e16: *stop=0x8c077e16u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c077e18; /* fmov.s fr0,@-r15 */
L8c077e18: target=r[3];*pr=0x8c077e1cu;*stop=0x8c077e1au;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077e1c; /* jsr @r3 */
L8c077e1c: *stop=0x8c077e1cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c077e1e; /* fmov.s @r15+,fr3 */
L8c077e1e: *stop=0x8c077e1eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c077e20; /* fcmp/gt fr3,fr0 */
L8c077e20: if(!(*sr&1u))goto L8c077e2c;goto L8c077e22; /* bf 0x8c077e2c */
L8c077e22: *stop=0x8c077e22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fb0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077e24; /* mov.l 0x8c077fb0,r2 */
L8c077e24: *stop=0x8c077e24u;r[5]=r[12];goto L8c077e26; /* mov r12,r5 */
L8c077e26: *stop=0x8c077e26u;r[6]=r[14];goto L8c077e28; /* mov r14,r6 */
L8c077e28: target=r[2];*pr=0x8c077e2cu;*stop=0x8c077e2au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077e2c; /* jsr @r2 */
L8c077e2c: *stop=0x8c077e2cu;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c077e2e; /* tst r8,r8 */
L8c077e2e: if(!(*sr&1u))goto L8c077ee8;goto L8c077e30; /* bf 0x8c077ee8 */
L8c077e30: *stop=0x8c077e30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fb4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077e32; /* mov.l 0x8c077fb4,r2 */
L8c077e32: target=r[2];*pr=0x8c077e36u;*stop=0x8c077e34u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077e36; /* jsr @r2 */
L8c077e36: *stop=0x8c077e36u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077e38; /* mov.l @(0x1c,r13),r3 */
L8c077e38: *stop=0x8c077e38u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c077e3a; /* tst r3,r3 */
L8c077e3a: take=(*sr&1u);*stop=0x8c077e3cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c077e46;goto L8c077e3e; /* bt/s 0x8c077e46 */
L8c077e3e: *stop=0x8c077e3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fb8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077e40; /* mov.l 0x8c077fb8,r3 */
L8c077e40: *stop=0x8c077e40u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c077e42; /* mov.l @(0x8,r15),r5 */
L8c077e42: target=r[3];*pr=0x8c077e46u;*stop=0x8c077e44u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077e46; /* jsr @r3 */
L8c077e46: *stop=0x8c077e46u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077e48; /* mov.l @(0x14,r13),r3 */
L8c077e48: *stop=0x8c077e48u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c077e4a; /* tst r3,r3 */
L8c077e4a: take=(*sr&1u);*stop=0x8c077e4cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c077e56;goto L8c077e4e; /* bt/s 0x8c077e56 */
L8c077e4e: *stop=0x8c077e4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fbcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077e50; /* mov.l 0x8c077fbc,r3 */
L8c077e50: *stop=0x8c077e50u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c077e52; /* mov.l @(0x8,r15),r5 */
L8c077e52: target=r[3];*pr=0x8c077e56u;*stop=0x8c077e54u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077e56; /* jsr @r3 */
L8c077e56: *stop=0x8c077e56u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077e58; /* mov.l @(0x18,r13),r3 */
L8c077e58: *stop=0x8c077e58u;r[3]=0u-r[3];goto L8c077e5a; /* neg r3,r3 */
L8c077e5a: *stop=0x8c077e5au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c077e5c; /* tst r3,r3 */
L8c077e5c: take=(*sr&1u);*stop=0x8c077e5eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c077e68;goto L8c077e60; /* bt/s 0x8c077e68 */
L8c077e60: *stop=0x8c077e60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fc0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077e62; /* mov.l 0x8c077fc0,r1 */
L8c077e62: *stop=0x8c077e62u;r[5]=r[3];goto L8c077e64; /* mov r3,r5 */
L8c077e64: target=r[1];*pr=0x8c077e68u;*stop=0x8c077e66u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077e68; /* jsr @r1 */
L8c077e68: *stop=0x8c077e68u;r[0]=123u;goto L8c077e6a; /* mov #0x7b,r0 */
L8c077e6a: *stop=0x8c077e6au;r[6]=r[14];goto L8c077e6c; /* mov r14,r6 */
L8c077e6c: *stop=0x8c077e6cu;r[0]<<=2;goto L8c077e6e; /* shll2 r0 */
L8c077e6e: *stop=0x8c077e6eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c077e70; /* fldi1 fr3 */
L8c077e70: *stop=0x8c077e70u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c077e72; /* fmov fr15,@(r0,r15) */
L8c077e72: *stop=0x8c077e72u;r[0]=121u;goto L8c077e74; /* mov #0x79,r0 */
L8c077e74: *stop=0x8c077e74u;r[0]<<=2;goto L8c077e76; /* shll2 r0 */
L8c077e76: *stop=0x8c077e76u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c077e78; /* fmov fr15,@(r0,r15) */
L8c077e78: *stop=0x8c077e78u;r[0]=122u;goto L8c077e7a; /* mov #0x7a,r0 */
L8c077e7a: *stop=0x8c077e7au;r[5]=121u;goto L8c077e7c; /* mov #0x79,r5 */
L8c077e7c: *stop=0x8c077e7cu;r[5]<<=2;goto L8c077e7e; /* shll2 r5 */
L8c077e7e: *stop=0x8c077e7eu;r[0]<<=2;goto L8c077e80; /* shll2 r0 */
L8c077e80: *stop=0x8c077e80u;r[5]+=r[15];goto L8c077e82; /* add r15,r5 */
L8c077e82: *stop=0x8c077e82u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c077e84; /* fmov fr3,@(r0,r15) */
L8c077e84: *stop=0x8c077e84u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077e86; /* mov.l @(0x14,r13),r3 */
L8c077e86: *stop=0x8c077e86u;r[6]+=28u;goto L8c077e88; /* add #0x1c,r6 */
L8c077e88: *stop=0x8c077e88u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[3],4);if(io_status)return io_status;goto L8c077e8a; /* mov.l r3,@(0x34,r14) */
L8c077e8a: *stop=0x8c077e8au;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077e8c; /* mov.l @(0x1c,r13),r2 */
L8c077e8c: *stop=0x8c077e8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fc4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077e8e; /* mov.l 0x8c077fc4,r3 */
L8c077e8e: *stop=0x8c077e8eu;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[2],4);if(io_status)return io_status;goto L8c077e90; /* mov.l r2,@(0x38,r14) */
L8c077e90: target=r[3];*pr=0x8c077e94u;*stop=0x8c077e92u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077e94; /* jsr @r3 */
L8c077e94: *stop=0x8c077e94u;r[0]=123u;goto L8c077e96; /* mov #0x7b,r0 */
L8c077e96: *stop=0x8c077e96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fc4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077e98; /* mov.l 0x8c077fc4,r3 */
L8c077e98: *stop=0x8c077e98u;r[0]<<=2;goto L8c077e9a; /* shll2 r0 */
L8c077e9a: *stop=0x8c077e9au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c077e9c; /* fmov fr15,@(r0,r15) */
L8c077e9c: *stop=0x8c077e9cu;r[0]=121u;goto L8c077e9e; /* mov #0x79,r0 */
L8c077e9e: *stop=0x8c077e9eu;r[0]<<=2;goto L8c077ea0; /* shll2 r0 */
L8c077ea0: *stop=0x8c077ea0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c077ea2; /* fmov fr15,@(r0,r15) */
L8c077ea2: *stop=0x8c077ea2u;r[0]=68u;goto L8c077ea4; /* mov #0x44,r0 */
L8c077ea4: *stop=0x8c077ea4u;r[0]<<=2;goto L8c077ea6; /* shll2 r0 */
L8c077ea6: *stop=0x8c077ea6u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c077ea8; /* fmov.s @(r0,r12),fr3 */
L8c077ea8: *stop=0x8c077ea8u;r[6]=127u;goto L8c077eaa; /* mov #0x7f,r6 */
L8c077eaa: *stop=0x8c077eaau;r[5]=121u;goto L8c077eac; /* mov #0x79,r5 */
L8c077eac: *stop=0x8c077eacu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c077eae; /* fmul fr14,fr3 */
L8c077eae: *stop=0x8c077eaeu;r[0]=122u;goto L8c077eb0; /* mov #0x7a,r0 */
L8c077eb0: *stop=0x8c077eb0u;r[5]<<=2;goto L8c077eb2; /* shll2 r5 */
L8c077eb2: *stop=0x8c077eb2u;r[6]<<=2;goto L8c077eb4; /* shll2 r6 */
L8c077eb4: *stop=0x8c077eb4u;r[0]<<=2;goto L8c077eb6; /* shll2 r0 */
L8c077eb6: *stop=0x8c077eb6u;r[5]+=r[15];goto L8c077eb8; /* add r15,r5 */
L8c077eb8: *stop=0x8c077eb8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c077eba; /* fmov fr3,@(r0,r15) */
L8c077eba: *stop=0x8c077ebau;r[6]+=r[15];goto L8c077ebc; /* add r15,r6 */
L8c077ebc: target=r[3];*pr=0x8c077ec0u;*stop=0x8c077ebeu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077ec0; /* jsr @r3 */
L8c077ec0: *stop=0x8c077ec0u;r[7]=121u;goto L8c077ec2; /* mov #0x79,r7 */
L8c077ec2: *stop=0x8c077ec2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fc8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077ec4; /* mov.l 0x8c077fc8,r2 */
L8c077ec4: *stop=0x8c077ec4u;r[7]<<=2;goto L8c077ec6; /* shll2 r7 */
L8c077ec6: *stop=0x8c077ec6u;r[5]=r[12];goto L8c077ec8; /* mov r12,r5 */
L8c077ec8: *stop=0x8c077ec8u;r[7]+=r[15];goto L8c077eca; /* add r15,r7 */
L8c077eca: *stop=0x8c077ecau;r[6]=r[14];goto L8c077ecc; /* mov r14,r6 */
L8c077ecc: target=r[2];*pr=0x8c077ed0u;*stop=0x8c077eceu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077ed0; /* jsr @r2 */
L8c077ed0: *stop=0x8c077ed0u;r[5]=121u;goto L8c077ed2; /* mov #0x79,r5 */
L8c077ed2: *stop=0x8c077ed2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fc4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077ed4; /* mov.l 0x8c077fc4,r3 */
L8c077ed4: *stop=0x8c077ed4u;r[6]=124u;goto L8c077ed6; /* mov #0x7c,r6 */
L8c077ed6: *stop=0x8c077ed6u;r[5]<<=2;goto L8c077ed8; /* shll2 r5 */
L8c077ed8: *stop=0x8c077ed8u;r[6]<<=2;goto L8c077eda; /* shll2 r6 */
L8c077eda: *stop=0x8c077edau;r[6]+=r[15];goto L8c077edc; /* add r15,r6 */
L8c077edc: *stop=0x8c077edcu;r[5]+=r[15];goto L8c077ede; /* add r15,r5 */
L8c077ede: target=r[3];*pr=0x8c077ee2u;*stop=0x8c077ee0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077ee2; /* jsr @r3 */
L8c077ee2: *stop=0x8c077ee2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077ee4; /* mov.l 0x8c077fcc,r2 */
L8c077ee4: target=r[2];*pr=0x8c077ee8u;*stop=0x8c077ee6u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077ee8; /* jsr @r2 */
L8c077ee8: *stop=0x8c077ee8u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077eea; /* mov.l @(0x34,r14),r3 */
L8c077eea: *stop=0x8c077eeau;r[1]=r[12];goto L8c077eec; /* mov r12,r1 */
L8c077eec: *stop=0x8c077eecu;r[1]+=80u;goto L8c077eee; /* add #0x50,r1 */
L8c077eee: *stop=0x8c077eeeu;r[4]=r[14];goto L8c077ef0; /* mov r14,r4 */
L8c077ef0: *stop=0x8c077ef0u;if(!valid_access(r[11]+28u,4u))return -3;wr(m,bus,&io_status,r[11]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c077ef2; /* mov.l r3,@(0x1c,r11) */
L8c077ef2: *stop=0x8c077ef2u;r[3]=r[14];goto L8c077ef4; /* mov r14,r3 */
L8c077ef4: *stop=0x8c077ef4u;r[3]+=72u;goto L8c077ef6; /* add #0x48,r3 */
L8c077ef6: *stop=0x8c077ef6u;if(!valid_access(r[14]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077ef8; /* mov.l @(0x38,r14),r2 */
L8c077ef8: *stop=0x8c077ef8u;if(!valid_access(r[11]+36u,4u))return -3;wr(m,bus,&io_status,r[11]+0x24u,r[2],4);if(io_status)return io_status;goto L8c077efa; /* mov.l r2,@(0x24,r11) */
L8c077efa: *stop=0x8c077efau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c077efc; /* mov.l @r3,r0 */
L8c077efc: *stop=0x8c077efcu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077efe; /* mov.l @(0x4,r3),r2 */
L8c077efe: *stop=0x8c077efeu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c077f00; /* mov.l r0,@r1 */
L8c077f00: *stop=0x8c077f00u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[2],4);if(io_status)return io_status;goto L8c077f02; /* mov.l r2,@(0x4,r1) */
L8c077f02: *stop=0x8c077f02u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c077f04; /* mov.l @(0x8,r3),r0 */
L8c077f04: *stop=0x8c077f04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fd0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077f06; /* mov.l 0x8c077fd0,r3 */
L8c077f06: *stop=0x8c077f06u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c077f08; /* mov.l r0,@(0x8,r1) */
L8c077f08: target=r[3];*pr=0x8c077f0cu;*stop=0x8c077f0au;r[4]+=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077f0c; /* jsr @r3 */
L8c077f0c: *stop=0x8c077f0cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,sr,1);if(status)return status;goto L8c077f0e; /* fcmp/gt fr12,fr0 */
L8c077f0e: if((*sr&1u))goto L8c077f14;goto L8c077f10; /* bt 0x8c077f14 */
L8c077f10: *stop=0x8c077f12u;goto L8c077fe0; /* bra 0x8c077fe0 */
L8c077f14: *stop=0x8c077f14u;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fd4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077f16; /* mov.l 0x8c077fd4,r3 */
L8c077f16: *stop=0x8c077f16u;r[4]=r[14];goto L8c077f18; /* mov r14,r4 */
L8c077f18: target=r[3];*pr=0x8c077f1cu;*stop=0x8c077f1au;r[4]+=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077f1c; /* jsr @r3 */
L8c077f1c: *stop=0x8c077f1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c077fd0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077f1e; /* mov.l 0x8c077fd0,r2 */
L8c077f1e: *stop=0x8c077f1eu;r[4]=r[14];goto L8c077f20; /* mov r14,r4 */
L8c077f20: target=r[2];*pr=0x8c077f24u;*stop=0x8c077f22u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c077f24; /* jsr @r2 */
L8c077f24: *stop=0x8c077f24u;r[0]=0x8c077fd8u;goto L8c077f26; /* mova 0x8c077fd8,r0 */
L8c077f26: *stop=0x8c077f26u;fcopy(f,4u,0u);goto L8c077f28; /* fmov fr0,fr4 */
L8c077f28: *stop=0x8c077f28u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c077f2a; /* fmov.s @r0,fr3 */
L8c077f2a: *stop=0x8c077f2au;r[0]=108u;goto L8c077f2c; /* mov #0x6c,r0 */
L8c077f2c: *stop=0x8c077f2cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f2e; /* fmov.s @(r0,r14),fr2 */
L8c077f2e: *stop=0x8c077f2eu;r[0]=16u;goto L8c077f30; /* mov #0x10,r0 */
L8c077f30: *stop=0x8c077f30u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c077f32; /* fadd fr3,fr4 */
L8c077f32: *stop=0x8c077f32u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c077f34; /* fmul fr4,fr2 */
L8c077f34: *stop=0x8c077f34u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f36; /* fmov fr2,@(r0,r14) */
L8c077f36: *stop=0x8c077f36u;r[0]=112u;goto L8c077f38; /* mov #0x70,r0 */
L8c077f38: *stop=0x8c077f38u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f3a; /* fmov.s @(r0,r14),fr2 */
L8c077f3a: *stop=0x8c077f3au;r[0]=20u;goto L8c077f3c; /* mov #0x14,r0 */
L8c077f3c: *stop=0x8c077f3cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c077f3e; /* fmul fr4,fr2 */
L8c077f3e: *stop=0x8c077f3eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f40; /* fmov fr2,@(r0,r14) */
L8c077f40: *stop=0x8c077f40u;r[0]=116u;goto L8c077f42; /* mov #0x74,r0 */
L8c077f42: *stop=0x8c077f42u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f44; /* fmov.s @(r0,r14),fr2 */
L8c077f44: *stop=0x8c077f44u;r[0]=24u;goto L8c077f46; /* mov #0x18,r0 */
L8c077f46: *stop=0x8c077f46u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c077f48; /* fmul fr4,fr2 */
L8c077f48: *stop=0x8c077f48u;fcopy(f,4u,12u);goto L8c077f4a; /* fmov fr12,fr4 */
L8c077f4a: *stop=0x8c077f4au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f4c; /* fmov fr2,@(r0,r14) */
L8c077f4c: *stop=0x8c077f4cu;r[0]=108u;goto L8c077f4e; /* mov #0x6c,r0 */
L8c077f4e: *stop=0x8c077f4eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f50; /* fmov.s @(r0,r14),fr2 */
L8c077f50: *stop=0x8c077f50u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c077f52; /* fmul fr4,fr2 */
L8c077f52: *stop=0x8c077f52u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f54; /* fmov fr2,@(r0,r14) */
L8c077f54: *stop=0x8c077f54u;r[0]=112u;goto L8c077f56; /* mov #0x70,r0 */
L8c077f56: *stop=0x8c077f56u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f58; /* fmov.s @(r0,r14),fr1 */
L8c077f58: *stop=0x8c077f58u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c077f5a; /* fmul fr4,fr1 */
L8c077f5a: *stop=0x8c077f5au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f5c; /* fmov fr1,@(r0,r14) */
L8c077f5c: *stop=0x8c077f5cu;r[0]=116u;goto L8c077f5e; /* mov #0x74,r0 */
L8c077f5e: *stop=0x8c077f5eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f60; /* fmov.s @(r0,r14),fr2 */
L8c077f60: *stop=0x8c077f60u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c077f62; /* fmul fr4,fr2 */
L8c077f62: *stop=0x8c077f62u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077f64; /* fmov fr2,@(r0,r14) */
L8c077f64: *stop=0x8c077f64u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077f66; /* mov.l @(0x14,r15),r3 */
L8c077f66: *stop=0x8c077f66u;r[2]=r[15];goto L8c077f68; /* mov r15,r2 */
L8c077f68: *stop=0x8c077f68u;r[2]+=40u;goto L8c077f6a; /* add #0x28,r2 */
L8c077f6a: *stop=0x8c077f6au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c077f6c; /* mov.l @r3,r1 */
L8c077f6c: *stop=0x8c077f6cu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c077f6e; /* mov.l @(0x4,r3),r0 */
L8c077f6e: *stop=0x8c077f6eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c077f70; /* mov.l r1,@r2 */
L8c077f70: *stop=0x8c077f70u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c077f72; /* mov.l r0,@(0x4,r2) */
L8c077f72: *stop=0x8c077f72u;r[0]=r[15];goto L8c077f74; /* mov r15,r0 */
L8c077f74: *stop=0x8c077f74u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077f76; /* mov.l @(0x8,r3),r1 */
L8c077f76: *stop=0x8c077f76u;r[0]+=40u;goto L8c077f78; /* add #0x28,r0 */
L8c077f78: *stop=0x8c077f78u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c077f7a; /* mov.l r1,@(0x8,r2) */
L8c077f7a: *stop=0x8c077f7au;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c077f7c; /* mov.l @r9,r3 */
L8c077f7c: *stop=0x8c077f7cu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077f7e; /* mov.l @(0x14,r15),r2 */
L8c077f7e: *stop=0x8c077f7eu;if(!valid_access(r[9]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077f80; /* mov.l @(0x4,r9),r1 */
L8c077f80: *stop=0x8c077f80u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c077f82; /* mov.l r3,@r2 */
L8c077f82: *stop=0x8c077f82u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c077f84; /* mov.l r1,@(0x4,r2) */
L8c077f84: *stop=0x8c077f84u;r[1]=4294967295u;goto L8c077f86; /* mov #-0x1,r1 */
L8c077f86: *stop=0x8c077f86u;if(!valid_access(r[9]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077f88; /* mov.l @(0x8,r9),r3 */
L8c077f88: *stop=0x8c077f88u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c077f8a; /* mov.l r3,@(0x8,r2) */
L8c077f8a: *stop=0x8c077f8au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c077f8c; /* mov.l @r0,r3 */
L8c077f8c: *stop=0x8c077f8cu;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c077f8e; /* mov.l @(0x4,r0),r2 */
L8c077f8e: *stop=0x8c077f8eu;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[3],4);if(io_status)return io_status;goto L8c077f90; /* mov.l r3,@r9 */
L8c077f90: *stop=0x8c077f90u;if(!valid_access(r[9]+4u,4u))return -3;wr(m,bus,&io_status,r[9]+0x4u,r[2],4);if(io_status)return io_status;goto L8c077f92; /* mov.l r2,@(0x4,r9) */
L8c077f92: *stop=0x8c077f92u;if(!valid_access(r[0]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c077f94; /* mov.l @(0x8,r0),r3 */
L8c077f94: *stop=0x8c077f94u;if(!valid_access(r[9]+8u,4u))return -3;wr(m,bus,&io_status,r[9]+0x8u,r[3],4);if(io_status)return io_status;goto L8c077f96; /* mov.l r3,@(0x8,r9) */
L8c077f96: *stop=0x8c077f98u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c077fec; /* bra 0x8c077fec */
L8c077fe0: *stop=0x8c077fe0u;r[0]=24u;goto L8c077fe2; /* mov #0x18,r0 */
L8c077fe2: *stop=0x8c077fe2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c077fe4; /* fmov fr15,@(r0,r14) */
L8c077fe4: *stop=0x8c077fe4u;r[0]=20u;goto L8c077fe6; /* mov #0x14,r0 */
L8c077fe6: *stop=0x8c077fe6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c077fe8; /* fmov fr15,@(r0,r14) */
L8c077fe8: *stop=0x8c077fe8u;r[0]=16u;goto L8c077fea; /* mov #0x10,r0 */
L8c077fea: *stop=0x8c077feau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c077fec; /* fmov fr15,@(r0,r14) */
L8c077fec: *stop=0x8c077fecu;r[0]=108u;goto L8c077fee; /* mov #0x6c,r0 */
L8c077fee: *stop=0x8c077feeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c078180u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c077ff0; /* mov.l 0x8c078180,r1 */
L8c077ff0: *stop=0x8c077ff0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c077ff2; /* fmov.s @(r0,r14),fr3 */
L8c077ff2: *stop=0x8c077ff2u;r[0]=96u;goto L8c077ff4; /* mov #0x60,r0 */
L8c077ff4: *stop=0x8c077ff4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077ff6; /* fmov.s @(r0,r14),fr2 */
L8c077ff6: *stop=0x8c077ff6u;r[0]=4u;goto L8c077ff8; /* mov #0x4,r0 */
L8c077ff8: *stop=0x8c077ff8u;r[3]=88u;goto L8c077ffa; /* mov #0x58,r3 */
L8c077ffa: *stop=0x8c077ffau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c077ffc; /* fadd fr3,fr2 */
L8c077ffc: *stop=0x8c077ffcu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c077ffe; /* fmov fr2,@(r0,r14) */
L8c077ffe: *stop=0x8c077ffeu;r[0]=112u;goto L8c078000; /* mov #0x70,r0 */
L8c078000: *stop=0x8c078000u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c078002; /* fmov.s @(r0,r14),fr3 */
L8c078002: *stop=0x8c078002u;r[0]=100u;goto L8c078004; /* mov #0x64,r0 */
L8c078004: *stop=0x8c078004u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c078006; /* fmov.s @(r0,r14),fr2 */
L8c078006: *stop=0x8c078006u;r[0]=8u;goto L8c078008; /* mov #0x8,r0 */
L8c078008: *stop=0x8c078008u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07800a; /* fadd fr3,fr2 */
L8c07800a: *stop=0x8c07800au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07800c; /* fmov fr2,@(r0,r14) */
L8c07800c: *stop=0x8c07800cu;r[0]=116u;goto L8c07800e; /* mov #0x74,r0 */
L8c07800e: *stop=0x8c07800eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c078010; /* fmov.s @(r0,r14),fr3 */
L8c078010: *stop=0x8c078010u;r[0]=104u;goto L8c078012; /* mov #0x68,r0 */
L8c078012: *stop=0x8c078012u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c078014; /* fmov.s @(r0,r14),fr2 */
L8c078014: *stop=0x8c078014u;r[0]=12u;goto L8c078016; /* mov #0xc,r0 */
L8c078016: *stop=0x8c078016u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c078018; /* fadd fr3,fr2 */
L8c078018: *stop=0x8c078018u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07801a; /* fmov fr2,@(r0,r14) */
L8c07801a: *stop=0x8c07801au;r[0]=42u;goto L8c07801c; /* mov #0x2a,r0 */
L8c07801c: *stop=0x8c07801cu;if(!valid_access(r[0]+r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c07801e; /* mov.w @(r0,r14),r2 */
L8c07801e: *stop=0x8c07801eu;r[0]=28u;goto L8c078020; /* mov #0x1c,r0 */
L8c078020: *stop=0x8c078020u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c078022; /* fmov.s @(r0,r14),fr3 */
L8c078022: *stop=0x8c078022u;r[0]=76u;goto L8c078024; /* mov #0x4c,r0 */
L8c078024: *stop=0x8c078024u;*macl=(r[3]&65535u)*(r[2]&65535u);goto L8c078026; /* mulu.w r3,r2 */
L8c078026: *stop=0x8c078026u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078028; /* fmov fr3,@(r0,r15) */
L8c078028: *stop=0x8c078028u;r[4]=*macl;goto L8c07802a; /* sts MACL,r4 */
L8c07802a: *stop=0x8c07802au;r[4]+=r[1];goto L8c07802c; /* add r1,r4 */
L8c07802c: *stop=0x8c07802cu;r[0]=32u;goto L8c07802e; /* mov #0x20,r0 */
L8c07802e: *stop=0x8c07802eu;r[6]=r[15];goto L8c078030; /* mov r15,r6 */
L8c078030: *stop=0x8c078030u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c078032; /* fmov.s @(r0,r14),fr3 */
L8c078032: *stop=0x8c078032u;r[0]=80u;goto L8c078034; /* mov #0x50,r0 */
L8c078034: *stop=0x8c078034u;r[5]=r[15];goto L8c078036; /* mov r15,r5 */
L8c078036: *stop=0x8c078036u;{uint32_t loaded=rd(m,bus,&io_status,0x8c078184u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c078038; /* mov.l 0x8c078184,r2 */
L8c078038: *stop=0x8c078038u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07803a; /* fmov fr3,@(r0,r15) */
L8c07803a: *stop=0x8c07803au;r[0]=36u;goto L8c07803c; /* mov #0x24,r0 */
L8c07803c: *stop=0x8c07803cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07803e; /* fmov.s @(r0,r14),fr3 */
L8c07803e: *stop=0x8c07803eu;r[0]=84u;goto L8c078040; /* mov #0x54,r0 */
L8c078040: *stop=0x8c078040u;r[5]+=64u;goto L8c078042; /* add #0x40,r5 */
L8c078042: *stop=0x8c078042u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078044; /* fmov fr3,@(r0,r15) */
L8c078044: *stop=0x8c078044u;r[0]=32u;goto L8c078046; /* mov #0x20,r0 */
L8c078046: *stop=0x8c078046u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c078048; /* fmov.s @(r0,r4),fr3 */
L8c078048: *stop=0x8c078048u;r[0]=64u;goto L8c07804a; /* mov #0x40,r0 */
L8c07804a: *stop=0x8c07804au;r[6]+=52u;goto L8c07804c; /* add #0x34,r6 */
L8c07804c: *stop=0x8c07804cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07804e; /* fmov fr3,@(r0,r15) */
L8c07804e: *stop=0x8c07804eu;r[0]=36u;goto L8c078050; /* mov #0x24,r0 */
L8c078050: *stop=0x8c078050u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c078052; /* fmov.s @(r0,r4),fr3 */
L8c078052: *stop=0x8c078052u;r[0]=68u;goto L8c078054; /* mov #0x44,r0 */
L8c078054: *stop=0x8c078054u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078056; /* fmov fr3,@(r0,r15) */
L8c078056: *stop=0x8c078056u;r[0]=40u;goto L8c078058; /* mov #0x28,r0 */
L8c078058: *stop=0x8c078058u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c07805a; /* fmov.s @(r0,r4),fr3 */
L8c07805a: *stop=0x8c07805au;r[0]=72u;goto L8c07805c; /* mov #0x48,r0 */
L8c07805c: *stop=0x8c07805cu;r[4]=r[14];goto L8c07805e; /* mov r14,r4 */
L8c07805e: *stop=0x8c07805eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078060; /* fmov fr3,@(r0,r15) */
L8c078060: target=r[2];*pr=0x8c078064u;*stop=0x8c078062u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078064; /* jsr @r2 */
L8c078064: *stop=0x8c078064u;r[0]=52u;goto L8c078066; /* mov #0x34,r0 */
L8c078066: *stop=0x8c078066u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078068; /* fmov.s @(r0,r15),fr3 */
L8c078068: *stop=0x8c078068u;r[0]=127u;goto L8c07806a; /* mov #0x7f,r0 */
L8c07806a: *stop=0x8c07806au;r[0]<<=2;goto L8c07806c; /* shll2 r0 */
L8c07806c: *stop=0x8c07806cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07806e; /* fmov.s @(r0,r15),fr2 */
L8c07806e: *stop=0x8c07806eu;r[0]=4u;goto L8c078070; /* mov #0x4,r0 */
L8c078070: *stop=0x8c078070u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c078072; /* fadd fr3,fr2 */
L8c078072: *stop=0x8c078072u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c078074; /* fmov fr2,@(r0,r14) */
L8c078074: *stop=0x8c078074u;r[0]=56u;goto L8c078076; /* mov #0x38,r0 */
L8c078076: *stop=0x8c078076u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078078; /* fmov.s @(r0,r15),fr3 */
L8c078078: *stop=0x8c078078u;r[0]=2u;goto L8c07807a; /* mov #0x2,r0 */
L8c07807a: *stop=0x8c07807au;r[0]<<=8;goto L8c07807c; /* shll8 r0 */
L8c07807c: *stop=0x8c07807cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07807e; /* fmov.s @(r0,r15),fr2 */
L8c07807e: *stop=0x8c07807eu;r[0]=8u;goto L8c078080; /* mov #0x8,r0 */
L8c078080: *stop=0x8c078080u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c078082; /* fadd fr3,fr2 */
L8c078082: *stop=0x8c078082u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c078084; /* fmov fr2,@(r0,r14) */
L8c078084: *stop=0x8c078084u;r[0]=60u;goto L8c078086; /* mov #0x3c,r0 */
L8c078086: *stop=0x8c078086u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078088; /* fmov.s @(r0,r15),fr3 */
L8c078088: *stop=0x8c078088u;r[0]=2u;goto L8c07808a; /* mov #0x2,r0 */
L8c07808a: *stop=0x8c07808au;r[0]<<=8;goto L8c07808c; /* shll8 r0 */
L8c07808c: *stop=0x8c07808cu;r[0]+=4u;goto L8c07808e; /* add #0x4,r0 */
L8c07808e: *stop=0x8c07808eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c078090; /* fmov.s @(r0,r15),fr2 */
L8c078090: *stop=0x8c078090u;r[0]=12u;goto L8c078092; /* mov #0xc,r0 */
L8c078092: *stop=0x8c078092u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c078094; /* fadd fr3,fr2 */
L8c078094: *stop=0x8c078094u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c078096; /* fmov fr2,@(r0,r14) */
L8c078096: *stop=0x8c078096u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078098; /* mov.l @(0xc,r15),r3 */
L8c078098: *stop=0x8c078098u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c07809a; /* tst r3,r3 */
L8c07809a: if((*sr&1u))goto L8c0780b6;goto L8c07809c; /* bt 0x8c0780b6 */
L8c07809c: *stop=0x8c07809cu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07809e; /* mov.l @(0xc,r15),r3 */
L8c07809e: *stop=0x8c07809eu;r[0]=108u;goto L8c0780a0; /* mov #0x6c,r0 */
L8c0780a0: *stop=0x8c0780a0u;r[7]=127u;goto L8c0780a2; /* mov #0x7f,r7 */
L8c0780a2: *stop=0x8c0780a2u;r[5]=r[12];goto L8c0780a4; /* mov r12,r5 */
L8c0780a4: *stop=0x8c0780a4u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[3],4);if(io_status)return io_status;goto L8c0780a6; /* mov.l r3,@(r0,r12) */
L8c0780a6: *stop=0x8c0780a6u;r[7]<<=2;goto L8c0780a8; /* shll2 r7 */
L8c0780a8: *stop=0x8c0780a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c078188u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0780aa; /* mov.l 0x8c078188,r3 */
L8c0780aa: *stop=0x8c0780aau;r[7]+=r[15];goto L8c0780ac; /* add r15,r7 */
L8c0780ac: *stop=0x8c0780acu;r[6]=r[14];goto L8c0780ae; /* mov r14,r6 */
L8c0780ae: target=r[3];*pr=0x8c0780b2u;*stop=0x8c0780b0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0780b2; /* jsr @r3 */
L8c0780b2: *stop=0x8c0780b4u;goto L8c0780ba; /* bra 0x8c0780ba */
L8c0780b6: *stop=0x8c0780b6u;r[0]=108u;goto L8c0780b8; /* mov #0x6c,r0 */
L8c0780b8: *stop=0x8c0780b8u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[10],4);if(io_status)return io_status;goto L8c0780ba; /* mov.l r10,@(r0,r12) */
L8c0780ba: *stop=0x8c0780bau;r[0]=127u;goto L8c0780bc; /* mov #0x7f,r0 */
L8c0780bc: *stop=0x8c0780bcu;r[4]=r[13];goto L8c0780be; /* mov r13,r4 */
L8c0780be: *stop=0x8c0780beu;r[0]<<=2;goto L8c0780c0; /* shll2 r0 */
L8c0780c0: *stop=0x8c0780c0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0780c2; /* fmov.s @(r0,r15),fr3 */
L8c0780c2: *stop=0x8c0780c2u;r[0]=4u;goto L8c0780c4; /* mov #0x4,r0 */
L8c0780c4: *stop=0x8c0780c4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0780c6; /* fmov.s @(r0,r14),fr2 */
L8c0780c6: *stop=0x8c0780c6u;r[0]=121u;goto L8c0780c8; /* mov #0x79,r0 */
L8c0780c8: *stop=0x8c0780c8u;r[0]<<=2;goto L8c0780ca; /* shll2 r0 */
L8c0780ca: *stop=0x8c0780cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0780cc; /* fsub fr3,fr2 */
L8c0780cc: *stop=0x8c0780ccu;r[5]=121u;goto L8c0780ce; /* mov #0x79,r5 */
L8c0780ce: *stop=0x8c0780ceu;r[5]<<=2;goto L8c0780d0; /* shll2 r5 */
L8c0780d0: *stop=0x8c0780d0u;r[5]+=r[15];goto L8c0780d2; /* add r15,r5 */
L8c0780d2: *stop=0x8c0780d2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0780d4; /* fmov fr2,@(r0,r15) */
L8c0780d4: *stop=0x8c0780d4u;r[0]=2u;goto L8c0780d6; /* mov #0x2,r0 */
L8c0780d6: *stop=0x8c0780d6u;r[0]<<=8;goto L8c0780d8; /* shll8 r0 */
L8c0780d8: *stop=0x8c0780d8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0780da; /* fmov.s @(r0,r15),fr3 */
L8c0780da: *stop=0x8c0780dau;r[0]=8u;goto L8c0780dc; /* mov #0x8,r0 */
L8c0780dc: *stop=0x8c0780dcu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0780de; /* fmov.s @(r0,r14),fr2 */
L8c0780de: *stop=0x8c0780deu;r[0]=122u;goto L8c0780e0; /* mov #0x7a,r0 */
L8c0780e0: *stop=0x8c0780e0u;r[0]<<=2;goto L8c0780e2; /* shll2 r0 */
L8c0780e2: *stop=0x8c0780e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0780e4; /* fsub fr3,fr2 */
L8c0780e4: *stop=0x8c0780e4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0780e6; /* fmov fr2,@(r0,r15) */
L8c0780e6: *stop=0x8c0780e6u;r[0]=2u;goto L8c0780e8; /* mov #0x2,r0 */
L8c0780e8: *stop=0x8c0780e8u;r[0]<<=8;goto L8c0780ea; /* shll8 r0 */
L8c0780ea: *stop=0x8c0780eau;r[0]+=4u;goto L8c0780ec; /* add #0x4,r0 */
L8c0780ec: *stop=0x8c0780ecu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0780ee; /* fmov.s @(r0,r15),fr3 */
L8c0780ee: *stop=0x8c0780eeu;r[0]=12u;goto L8c0780f0; /* mov #0xc,r0 */
L8c0780f0: *stop=0x8c0780f0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0780f2; /* fmov.s @(r0,r14),fr2 */
L8c0780f2: *stop=0x8c0780f2u;r[0]=123u;goto L8c0780f4; /* mov #0x7b,r0 */
L8c0780f4: *stop=0x8c0780f4u;r[0]<<=2;goto L8c0780f6; /* shll2 r0 */
L8c0780f6: *stop=0x8c0780f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0780f8; /* fsub fr3,fr2 */
L8c0780f8: *stop=0x8c0780f8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0780fa; /* fmov fr2,@(r0,r15) */
L8c0780fa: *stop=0x8c0780fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c07818cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0780fc; /* mov.l 0x8c07818c,r3 */
L8c0780fc: target=r[3];*pr=0x8c078100u;*stop=0x8c0780feu;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078100; /* jsr @r3 */
L8c078100: *stop=0x8c078100u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c078102; /* mov.l @(0xc,r15),r2 */
L8c078102: *stop=0x8c078102u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c078104; /* tst r2,r2 */
L8c078104: take=!(*sr&1u);*stop=0x8c078106u;fcopy(f,4u,0u);if(take)goto L8c078120;goto L8c078108; /* bf/s 0x8c078120 */
L8c078108: *stop=0x8c078108u;r[0]=56u;goto L8c07810a; /* mov #0x38,r0 */
L8c07810a: *stop=0x8c07810au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c07810c; /* fldi0 fr3 */
L8c07810c: *stop=0x8c07810cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c07810e; /* fmov.s @(r0,r12),fr2 */
L8c07810e: *stop=0x8c07810eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c078110; /* fcmp/eq fr3,fr2 */
L8c078110: if(!(*sr&1u))goto L8c078120;goto L8c078112; /* bf 0x8c078120 */
L8c078112: *stop=0x8c078112u;r[0]=64u;goto L8c078114; /* mov #0x40,r0 */
L8c078114: *stop=0x8c078114u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c078116; /* fmov.s @(r0,r12),fr2 */
L8c078116: *stop=0x8c078116u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c078118; /* fcmp/eq fr3,fr2 */
L8c078118: if(!(*sr&1u))goto L8c078120;goto L8c07811a; /* bf 0x8c078120 */
L8c07811a: *stop=0x8c07811au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c07811c; /* fldi1 fr3 */
L8c07811c: *stop=0x8c07811cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c07811e; /* fcmp/gt fr3,fr4 */
L8c07811e: if(!(*sr&1u))goto L8c07813a;goto L8c078120; /* bf 0x8c07813a */
L8c078120: *stop=0x8c078120u;r[0]=0x8c078190u;goto L8c078122; /* mova 0x8c078190,r0 */
L8c078122: *stop=0x8c078122u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c078124; /* fmov.s @r0,fr3 */
L8c078124: *stop=0x8c078124u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c078126; /* fcmp/gt fr3,fr4 */
L8c078126: if(!(*sr&1u))goto L8c07813a;goto L8c078128; /* bf 0x8c07813a */
L8c078128: *stop=0x8c078128u;r[2]=121u;goto L8c07812a; /* mov #0x79,r2 */
L8c07812a: *stop=0x8c07812au;r[2]<<=2;goto L8c07812c; /* shll2 r2 */
L8c07812c: *stop=0x8c07812cu;r[2]+=r[15];goto L8c07812e; /* add r15,r2 */
L8c07812e: *stop=0x8c07812eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c078130; /* mov.l @r2,r3 */
L8c078130: *stop=0x8c078130u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c078132; /* mov.l @(0x4,r2),r1 */
L8c078132: *stop=0x8c078132u;if(!valid_access(r[13]+32u,4u))return -3;wr(m,bus,&io_status,r[13]+0x20u,r[3],4);if(io_status)return io_status;goto L8c078134; /* mov.l r3,@(0x20,r13) */
L8c078134: *stop=0x8c078134u;if(!valid_access(r[13]+36u,4u))return -3;wr(m,bus,&io_status,r[13]+0x24u,r[1],4);if(io_status)return io_status;goto L8c078136; /* mov.l r1,@(0x24,r13) */
L8c078136: *stop=0x8c078136u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078138; /* mov.l @(0x8,r2),r3 */
L8c078138: *stop=0x8c078138u;if(!valid_access(r[13]+40u,4u))return -3;wr(m,bus,&io_status,r[13]+0x28u,r[3],4);if(io_status)return io_status;goto L8c07813a; /* mov.l r3,@(0x28,r13) */
L8c07813a: *stop=0x8c07813au;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c07813c; /* mov.w @(0x4,r13),r0 */
L8c07813c: *stop=0x8c07813cu;r[5]=r[11];goto L8c07813e; /* mov r11,r5 */
L8c07813e: *stop=0x8c07813eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c078194u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078140; /* mov.l 0x8c078194,r3 */
L8c078140: *stop=0x8c078140u;r[6]=r[12];goto L8c078142; /* mov r12,r6 */
L8c078142: *stop=0x8c078142u;r[0]|=0x1u;goto L8c078144; /* or #0x1,r0 */
L8c078144: *stop=0x8c078144u;if(!valid_access(r[13]+4u,2u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[0],2);if(io_status)return io_status;goto L8c078146; /* mov.w r0,@(0x4,r13) */
L8c078146: *stop=0x8c078146u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[7]=loaded;}goto L8c078148; /* mov.l @r9,r7 */
L8c078148: target=r[3];*pr=0x8c07814cu;*stop=0x8c07814au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07814c; /* jsr @r3 */
L8c07814c: *stop=0x8c07814eu;goto L8c0781c8; /* bra 0x8c0781c8 */
L8c078150: *stop=0x8c078150u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c078152; /* mov.l @r9,r3 */
L8c078152: *stop=0x8c078152u;r[2]=2u;goto L8c078154; /* mov #0x2,r2 */
L8c078154: *stop=0x8c078154u;r[2]<<=8;goto L8c078156; /* shll8 r2 */
L8c078156: *stop=0x8c078156u;r[5]=r[11];goto L8c078158; /* mov r11,r5 */
L8c078158: *stop=0x8c078158u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c07815a; /* mov.l r3,@-r15 */
L8c07815a: *stop=0x8c07815au;r[2]+=4u;goto L8c07815c; /* add #0x4,r2 */
L8c07815c: *stop=0x8c07815cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c07815e; /* mov.l r8,@-r15 */
L8c07815e: *stop=0x8c07815eu;r[3]=121u;goto L8c078160; /* mov #0x79,r3 */
L8c078160: *stop=0x8c078160u;r[2]+=r[15];goto L8c078162; /* add r15,r2 */
L8c078162: *stop=0x8c078162u;r[6]=r[12];goto L8c078164; /* mov r12,r6 */
L8c078164: *stop=0x8c078164u;r[3]<<=2;goto L8c078166; /* shll2 r3 */
L8c078166: *stop=0x8c078166u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c078168; /* mov.l r2,@-r15 */
L8c078168: *stop=0x8c078168u;{uint32_t loaded=rd(m,bus,&io_status,0x8c078198u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07816a; /* mov.l 0x8c078198,r2 */
L8c07816a: *stop=0x8c07816au;r[3]+=r[15];goto L8c07816c; /* add r15,r3 */
L8c07816c: *stop=0x8c07816cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c07816e; /* mov.l r3,@-r15 */
L8c07816e: *stop=0x8c07816eu;r[7]=r[14];goto L8c078170; /* mov r14,r7 */
L8c078170: target=r[2];*pr=0x8c078174u;*stop=0x8c078172u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078174; /* jsr @r2 */
L8c078174: *stop=0x8c078174u;r[15]+=16u;goto L8c078176; /* add #0x10,r15 */
L8c078176: *stop=0x8c078178u;r[8]=r[0];goto L8c0781c8; /* bra 0x8c0781c8 */
L8c0781a0: *stop=0x8c0781a0u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0781a2; /* mov.l @r9,r3 */
L8c0781a2: *stop=0x8c0781a2u;r[2]=2u;goto L8c0781a4; /* mov #0x2,r2 */
L8c0781a4: *stop=0x8c0781a4u;r[2]<<=8;goto L8c0781a6; /* shll8 r2 */
L8c0781a6: *stop=0x8c0781a6u;r[5]=r[11];goto L8c0781a8; /* mov r11,r5 */
L8c0781a8: *stop=0x8c0781a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0781aa; /* mov.l r3,@-r15 */
L8c0781aa: *stop=0x8c0781aau;r[2]+=4u;goto L8c0781ac; /* add #0x4,r2 */
L8c0781ac: *stop=0x8c0781acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c0781ae; /* mov.l r8,@-r15 */
L8c0781ae: *stop=0x8c0781aeu;r[3]=121u;goto L8c0781b0; /* mov #0x79,r3 */
L8c0781b0: *stop=0x8c0781b0u;r[2]+=r[15];goto L8c0781b2; /* add r15,r2 */
L8c0781b2: *stop=0x8c0781b2u;r[6]=r[12];goto L8c0781b4; /* mov r12,r6 */
L8c0781b4: *stop=0x8c0781b4u;r[3]<<=2;goto L8c0781b6; /* shll2 r3 */
L8c0781b6: *stop=0x8c0781b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c0781b8; /* mov.l r2,@-r15 */
L8c0781b8: *stop=0x8c0781b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c078380u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0781ba; /* mov.l 0x8c078380,r2 */
L8c0781ba: *stop=0x8c0781bau;r[3]+=r[15];goto L8c0781bc; /* add r15,r3 */
L8c0781bc: *stop=0x8c0781bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0781be; /* mov.l r3,@-r15 */
L8c0781be: *stop=0x8c0781beu;r[7]=r[14];goto L8c0781c0; /* mov r14,r7 */
L8c0781c0: target=r[2];*pr=0x8c0781c4u;*stop=0x8c0781c2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0781c4; /* jsr @r2 */
L8c0781c4: *stop=0x8c0781c4u;r[15]+=16u;goto L8c0781c6; /* add #0x10,r15 */
L8c0781c6: *stop=0x8c0781c6u;r[8]=r[0];goto L8c0781c8; /* mov r0,r8 */
L8c0781c8: *stop=0x8c0781c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0781ca; /* mov.l @r15,r3 */
L8c0781ca: *stop=0x8c0781cau;r[3]+=1u;goto L8c0781cc; /* add #0x1,r3 */
L8c0781cc: *stop=0x8c0781ccu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0781ce; /* mov.l r3,@r15 */
L8c0781ce: *stop=0x8c0781ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0781d0; /* mov.l @r15,r1 */
L8c0781d0: *stop=0x8c0781d0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0781d2; /* mov.l @(0x4,r15),r2 */
L8c0781d2: *stop=0x8c0781d2u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[2]);goto L8c0781d4; /* cmp/ge r2,r1 */
L8c0781d4: if((*sr&1u))goto L8c0781da;goto L8c0781d6; /* bt 0x8c0781da */
L8c0781d6: *stop=0x8c0781d8u;goto L8c077c68; /* bra 0x8c077c68 */
L8c0781da: *stop=0x8c0781dau;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0781dc; /* mov.l @(0x20,r15),r0 */
L8c0781dc: *stop=0x8c0781dcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0781de; /* cmp/eq #0x1,r0 */
L8c0781de: if((*sr&1u))goto L8c0782b4;goto L8c0781e0; /* bt 0x8c0782b4 */
L8c0781e0: *stop=0x8c0781e0u;r[0]=24u;goto L8c0781e2; /* mov #0x18,r0 */
L8c0781e2: *stop=0x8c0781e2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0781e4; /* fmov fr15,@(r0,r15) */
L8c0781e4: *stop=0x8c0781e4u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c0781e6; /* mov.w @(0x10,r12),r0 */
L8c0781e6: *stop=0x8c0781e6u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c0781e8; /* tst #0x1,r0 */
L8c0781e8: take=(*sr&1u);*stop=0x8c0781eau;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;if(take)goto L8c07827a;goto L8c0781ec; /* bt/s 0x8c07827a */
L8c0781ec: *stop=0x8c0781ecu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0781ee; /* mov.l @(0x4,r15),r2 */
L8c0781ee: *stop=0x8c0781eeu;r[3]=64u;goto L8c0781f0; /* mov #0x40,r3 */
L8c0781f0: *stop=0x8c0781f0u;r[3]<<=8;goto L8c0781f2; /* shll8 r3 */
L8c0781f2: *stop=0x8c0781f2u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c0781f4; /* tst r3,r2 */
L8c0781f4: if(!(*sr&1u))goto L8c07827a;goto L8c0781f6; /* bf 0x8c07827a */
L8c0781f6: *stop=0x8c0781f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c078384u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0781f8; /* mov.l 0x8c078384,r2 */
L8c0781f8: *stop=0x8c0781f8u;r[6]=r[15];goto L8c0781fa; /* mov r15,r6 */
L8c0781fa: *stop=0x8c0781fau;r[5]=0u;goto L8c0781fc; /* mov #0x0,r5 */
L8c0781fc: *stop=0x8c0781fcu;r[6]+=24u;goto L8c0781fe; /* add #0x18,r6 */
L8c0781fe: target=r[2];*pr=0x8c078202u;*stop=0x8c078200u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078202; /* jsr @r2 */
L8c078202: *stop=0x8c078202u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c078204; /* tst r0,r0 */
L8c078204: if((*sr&1u))goto L8c078218;goto L8c078206; /* bt 0x8c078218 */
L8c078206: *stop=0x8c078206u;r[0]=0x8c078388u;goto L8c078208; /* mova 0x8c078388,r0 */
L8c078208: *stop=0x8c078208u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c07820a; /* fmov.s @r0,fr3 */
L8c07820a: *stop=0x8c07820au;r[0]=24u;goto L8c07820c; /* mov #0x18,r0 */
L8c07820c: *stop=0x8c07820cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07820e; /* fmov.s @(r0,r15),fr2 */
L8c07820e: *stop=0x8c07820eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c078210; /* fcmp/gt fr3,fr2 */
L8c078210: if((*sr&1u))goto L8c07827a;goto L8c078212; /* bt 0x8c07827a */
L8c078212: *stop=0x8c078212u;if(!valid_access(r[12]+10u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0xau,2);if(io_status)return io_status;goto L8c078214; /* mov.w @(0xa,r12),r0 */
L8c078214: *stop=0x8c078214u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c078216; /* tst r0,r0 */
L8c078216: if(!(*sr&1u))goto L8c07827a;goto L8c078218; /* bf 0x8c07827a */
L8c078218: *stop=0x8c078218u;r[0]=60u;goto L8c07821a; /* mov #0x3c,r0 */
L8c07821a: *stop=0x8c07821au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c07821c; /* fldi0 fr3 */
L8c07821c: *stop=0x8c07821cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c07821e; /* fmov.s @(r0,r12),fr2 */
L8c07821e: *stop=0x8c07821eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c078220; /* fcmp/gt fr3,fr2 */
L8c078220: if((*sr&1u))goto L8c07827a;goto L8c078222; /* bt 0x8c07827a */
L8c078222: *stop=0x8c078222u;r[0]=4294967236u;goto L8c078224; /* mov #-0x3c,r0 */
L8c078224: *stop=0x8c078224u;r[0]=r[0]&255u;goto L8c078226; /* extu.b r0,r0 */
L8c078226: *stop=0x8c078226u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c078228; /* fmov.s @(r0,r12),fr2 */
L8c078228: *stop=0x8c078228u;r[0]=56u;goto L8c07822a; /* mov #0x38,r0 */
L8c07822a: *stop=0x8c07822au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c07822c; /* fmov.s @(r0,r12),fr1 */
L8c07822c: *stop=0x8c07822cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c07822e; /* fcmp/gt fr2,fr1 */
L8c07822e: if((*sr&1u))goto L8c07827a;goto L8c078230; /* bt 0x8c07827a */
L8c078230: *stop=0x8c078230u;r[0]=0x8c07838cu;goto L8c078232; /* mova 0x8c07838c,r0 */
L8c078232: *stop=0x8c078232u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c078234; /* fmov.s @r0,fr2 */
L8c078234: *stop=0x8c078234u;r[0]=28u;goto L8c078236; /* mov #0x1c,r0 */
L8c078236: *stop=0x8c078236u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c078238; /* fmov.s @(r0,r12),fr1 */
L8c078238: *stop=0x8c078238u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c07823a; /* fcmp/gt fr2,fr1 */
L8c07823a: if(!(*sr&1u))goto L8c07827a;goto L8c07823c; /* bf 0x8c07827a */
L8c07823c: *stop=0x8c07823cu;r[1]=4u;goto L8c07823e; /* mov #0x4,r1 */
L8c07823e: *stop=0x8c07823eu;r[1]+=r[11];goto L8c078240; /* add r11,r1 */
L8c078240: *stop=0x8c078240u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c078242; /* fmov.s @r1,fr3 */
L8c078242: *stop=0x8c078242u;r[0]=32u;goto L8c078244; /* mov #0x20,r0 */
L8c078244: *stop=0x8c078244u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c078246; /* fmov.s @(r0,r13),fr2 */
L8c078246: *stop=0x8c078246u;r[1]=8u;goto L8c078248; /* mov #0x8,r1 */
L8c078248: *stop=0x8c078248u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c07824a; /* fmul fr14,fr3 */
L8c07824a: *stop=0x8c07824au;r[1]+=r[11];goto L8c07824c; /* add r11,r1 */
L8c07824c: *stop=0x8c07824cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07824e; /* fsub fr3,fr2 */
L8c07824e: *stop=0x8c07824eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c078250; /* fmov fr2,@(r0,r13) */
L8c078250: *stop=0x8c078250u;r[0]=36u;goto L8c078252; /* mov #0x24,r0 */
L8c078252: *stop=0x8c078252u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c078254; /* fmov.s @r1,fr3 */
L8c078254: *stop=0x8c078254u;r[1]=12u;goto L8c078256; /* mov #0xc,r1 */
L8c078256: *stop=0x8c078256u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c078258; /* fmov.s @(r0,r13),fr2 */
L8c078258: *stop=0x8c078258u;r[1]+=r[11];goto L8c07825a; /* add r11,r1 */
L8c07825a: *stop=0x8c07825au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c07825c; /* fmul fr14,fr3 */
L8c07825c: *stop=0x8c07825cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07825e; /* fsub fr3,fr2 */
L8c07825e: *stop=0x8c07825eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c078260; /* fmov fr2,@(r0,r13) */
L8c078260: *stop=0x8c078260u;r[0]=40u;goto L8c078262; /* mov #0x28,r0 */
L8c078262: *stop=0x8c078262u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c078264; /* fmov.s @r1,fr3 */
L8c078264: *stop=0x8c078264u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c078266; /* fmov.s @(r0,r13),fr2 */
L8c078266: *stop=0x8c078266u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c078268; /* fmul fr14,fr3 */
L8c078268: *stop=0x8c078268u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07826a; /* fsub fr3,fr2 */
L8c07826a: *stop=0x8c07826au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07826c; /* fmov fr2,@(r0,r13) */
L8c07826c: *stop=0x8c07826cu;r[0]=4u;goto L8c07826e; /* mov #0x4,r0 */
L8c07826e: *stop=0x8c07826eu;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[11]);if(io_status)return io_status;goto L8c078270; /* fmov fr15,@(r0,r11) */
L8c078270: *stop=0x8c078270u;r[0]=8u;goto L8c078272; /* mov #0x8,r0 */
L8c078272: *stop=0x8c078272u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[11]);if(io_status)return io_status;goto L8c078274; /* fmov fr15,@(r0,r11) */
L8c078274: *stop=0x8c078274u;r[0]=12u;goto L8c078276; /* mov #0xc,r0 */
L8c078276: *stop=0x8c078278u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[11]);if(io_status)return io_status;goto L8c0782a4; /* bra 0x8c0782a4 */
L8c07827a: *stop=0x8c07827au;r[1]=16u;goto L8c07827c; /* mov #0x10,r1 */
L8c07827c: *stop=0x8c07827cu;r[1]+=r[14];goto L8c07827e; /* add r14,r1 */
L8c07827e: *stop=0x8c07827eu;r[0]=32u;goto L8c078280; /* mov #0x20,r0 */
L8c078280: *stop=0x8c078280u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c078282; /* fmov.s @r1,fr3 */
L8c078282: *stop=0x8c078282u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c078284; /* fmov.s @(r0,r13),fr2 */
L8c078284: *stop=0x8c078284u;r[1]=20u;goto L8c078286; /* mov #0x14,r1 */
L8c078286: *stop=0x8c078286u;r[1]+=r[14];goto L8c078288; /* add r14,r1 */
L8c078288: *stop=0x8c078288u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07828a; /* fadd fr3,fr2 */
L8c07828a: *stop=0x8c07828au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07828c; /* fmov fr2,@(r0,r13) */
L8c07828c: *stop=0x8c07828cu;r[0]=36u;goto L8c07828e; /* mov #0x24,r0 */
L8c07828e: *stop=0x8c07828eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c078290; /* fmov.s @(r0,r13),fr2 */
L8c078290: *stop=0x8c078290u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c078292; /* fmov.s @r1,fr3 */
L8c078292: *stop=0x8c078292u;r[1]=24u;goto L8c078294; /* mov #0x18,r1 */
L8c078294: *stop=0x8c078294u;r[1]+=r[14];goto L8c078296; /* add r14,r1 */
L8c078296: *stop=0x8c078296u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c078298; /* fadd fr3,fr2 */
L8c078298: *stop=0x8c078298u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07829a; /* fmov fr2,@(r0,r13) */
L8c07829a: *stop=0x8c07829au;r[0]=40u;goto L8c07829c; /* mov #0x28,r0 */
L8c07829c: *stop=0x8c07829cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07829e; /* fmov.s @(r0,r13),fr2 */
L8c07829e: *stop=0x8c07829eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0782a0; /* fmov.s @r1,fr3 */
L8c0782a0: *stop=0x8c0782a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0782a2; /* fadd fr3,fr2 */
L8c0782a2: *stop=0x8c0782a2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0782a4; /* fmov fr2,@(r0,r13) */
L8c0782a4: *stop=0x8c0782a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c078390u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0782a6; /* mov.l 0x8c078390,r3 */
L8c0782a6: *stop=0x8c0782a6u;r[5]=r[11];goto L8c0782a8; /* mov r11,r5 */
L8c0782a8: *stop=0x8c0782a8u;r[7]=0u;goto L8c0782aa; /* mov #0x0,r7 */
L8c0782aa: *stop=0x8c0782aau;r[6]=r[12];goto L8c0782ac; /* mov r12,r6 */
L8c0782ac: target=r[3];*pr=0x8c0782b0u;*stop=0x8c0782aeu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0782b0; /* jsr @r3 */
L8c0782b0: *stop=0x8c0782b2u;goto L8c0782e4; /* bra 0x8c0782e4 */
L8c0782b4: *stop=0x8c0782b4u;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0782b6; /* mov.l @(0x24,r15),r3 */
L8c0782b6: *stop=0x8c0782b6u;r[0]=116u;goto L8c0782b8; /* mov #0x74,r0 */
L8c0782b8: *stop=0x8c0782b8u;r[1]=16u;goto L8c0782ba; /* mov #0x10,r1 */
L8c0782ba: *stop=0x8c0782bau;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[3],4);if(io_status)return io_status;goto L8c0782bc; /* mov.l r3,@(r0,r12) */
L8c0782bc: *stop=0x8c0782bcu;r[1]+=r[14];goto L8c0782be; /* add r14,r1 */
L8c0782be: *stop=0x8c0782beu;r[0]=32u;goto L8c0782c0; /* mov #0x20,r0 */
L8c0782c0: *stop=0x8c0782c0u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0782c2; /* fmov.s @r1,fr3 */
L8c0782c2: *stop=0x8c0782c2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0782c4; /* fmov.s @(r0,r13),fr2 */
L8c0782c4: *stop=0x8c0782c4u;r[1]=20u;goto L8c0782c6; /* mov #0x14,r1 */
L8c0782c6: *stop=0x8c0782c6u;r[1]+=r[14];goto L8c0782c8; /* add r14,r1 */
L8c0782c8: *stop=0x8c0782c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0782ca; /* fadd fr3,fr2 */
L8c0782ca: *stop=0x8c0782cau;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0782cc; /* fmov fr2,@(r0,r13) */
L8c0782cc: *stop=0x8c0782ccu;r[0]=36u;goto L8c0782ce; /* mov #0x24,r0 */
L8c0782ce: *stop=0x8c0782ceu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0782d0; /* fmov.s @(r0,r13),fr2 */
L8c0782d0: *stop=0x8c0782d0u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0782d2; /* fmov.s @r1,fr3 */
L8c0782d2: *stop=0x8c0782d2u;r[1]=24u;goto L8c0782d4; /* mov #0x18,r1 */
L8c0782d4: *stop=0x8c0782d4u;r[1]+=r[14];goto L8c0782d6; /* add r14,r1 */
L8c0782d6: *stop=0x8c0782d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0782d8; /* fadd fr3,fr2 */
L8c0782d8: *stop=0x8c0782d8u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0782da; /* fmov fr2,@(r0,r13) */
L8c0782da: *stop=0x8c0782dau;r[0]=40u;goto L8c0782dc; /* mov #0x28,r0 */
L8c0782dc: *stop=0x8c0782dcu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0782de; /* fmov.s @(r0,r13),fr2 */
L8c0782de: *stop=0x8c0782deu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0782e0; /* fmov.s @r1,fr3 */
L8c0782e0: *stop=0x8c0782e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0782e2; /* fadd fr3,fr2 */
L8c0782e2: *stop=0x8c0782e2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0782e4; /* fmov fr2,@(r0,r13) */
L8c0782e4: *stop=0x8c0782e4u;r[0]=r[8];goto L8c0782e6; /* mov r8,r0 */
L8c0782e6: *stop=0x8c0782e6u;r[1]=2u;goto L8c0782e8; /* mov #0x2,r1 */
L8c0782e8: *stop=0x8c0782e8u;r[1]<<=8;goto L8c0782ea; /* shll8 r1 */
L8c0782ea: *stop=0x8c0782eau;r[1]+=8u;goto L8c0782ec; /* add #0x8,r1 */
L8c0782ec: *stop=0x8c0782ecu;r[15]+=r[1];goto L8c0782ee; /* add r1,r15 */
L8c0782ee: *stop=0x8c0782eeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0782f0; /* lds.l @r15+,PR */
L8c0782f0: *stop=0x8c0782f0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0782f2; /* fmov.s @r15+,fr12 */
L8c0782f2: *stop=0x8c0782f2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0782f4; /* fmov.s @r15+,fr13 */
L8c0782f4: *stop=0x8c0782f4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0782f6; /* fmov.s @r15+,fr14 */
L8c0782f6: *stop=0x8c0782f6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0782f8; /* fmov.s @r15+,fr15 */
L8c0782f8: *stop=0x8c0782f8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c0782fa; /* mov.l @r15+,r8 */
L8c0782fa: *stop=0x8c0782fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c0782fc; /* mov.l @r15+,r9 */
L8c0782fc: *stop=0x8c0782fcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c0782fe; /* mov.l @r15+,r10 */
L8c0782fe: *stop=0x8c0782feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c078300; /* mov.l @r15+,r11 */
L8c078300: *stop=0x8c078300u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c078302; /* mov.l @r15+,r12 */
L8c078302: *stop=0x8c078302u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c078304; /* mov.l @r15+,r13 */
L8c078304: *stop=0x8c078304u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c078306u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_special_tail_8c0777e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077822(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077822u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077832(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077832u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077842(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077842u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077854(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077854u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077882(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077882u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c0778b2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0778b2u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c0778c2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0778c2u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c0778d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0778d4u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c0778da(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0778dau);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077938(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077938u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c0779b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0779b0u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c0779cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0779ccu);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077a2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077a2cu);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077aae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077aaeu);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077c84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077c84u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077c9a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077c9au);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077cc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077cc0u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077d0e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077d0eu);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077d52(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077d52u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077dd4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077dd4u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077de6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077de6u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077e10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077e10u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077e1c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077e1cu);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077e2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077e2cu);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077e36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077e36u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077e46(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077e46u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077e56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077e56u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077e68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077e68u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077e94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077e94u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077ec0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077ec0u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077ed0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077ed0u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077ee2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077ee2u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077ee8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077ee8u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077f0c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077f0cu);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077f1c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077f1cu);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c077f24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c077f24u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c078064(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078064u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c0780b2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0780b2u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c078100(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078100u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c07814c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07814cu);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c078174(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078174u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c0781c4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0781c4u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c078202(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078202u);}
SA_STAGE_EXPORT int sa_movement_special_tail_resume_8c0782b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_special_tail_run_8c0777e0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0782b0u);}
