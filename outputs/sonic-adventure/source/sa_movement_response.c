/* Original contact response, signed vector angle, cross/length, vector addition and canonical cosine adapter.
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
static int sa_movement_response_run_8c075416(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c075494u)goto L8c075494;if(resume_pc==0x8c0754acu)goto L8c0754ac;if(resume_pc==0x8c0754c0u)goto L8c0754c0;if(resume_pc==0x8c0754ccu)goto L8c0754cc;if(resume_pc==0x8c075536u)goto L8c075536;if(resume_pc==0x8c075544u)goto L8c075544;if(resume_pc==0x8c075550u)goto L8c075550;if(resume_pc==0x8c075590u)goto L8c075590;if(resume_pc==0x8c0755aeu)goto L8c0755ae;if(resume_pc==0x8c0755c2u)goto L8c0755c2;if(resume_pc==0x8c0755ceu)goto L8c0755ce;if(resume_pc==0x8c0755f2u)goto L8c0755f2;if(resume_pc==0x8c07560eu)goto L8c07560e;if(resume_pc==0x8c075622u)goto L8c075622;if(resume_pc==0x8c07563eu)goto L8c07563e;goto L8c075416;
L8c075416: *stop=0x8c075416u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c075418; /* mov.l r14,@-r15 */
L8c075418: *stop=0x8c075418u;r[0]=4294967295u;goto L8c07541a; /* mov #-0x1,r0 */
L8c07541a: *stop=0x8c07541au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c07541c; /* mov.l r13,@-r15 */
L8c07541c: *stop=0x8c07541cu;r[0]<<=8;goto L8c07541e; /* shll8 r0 */
L8c07541e: *stop=0x8c07541eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c075420; /* mov.l r12,@-r15 */
L8c075420: *stop=0x8c075420u;r[0]+=88u;goto L8c075422; /* add #0x58,r0 */
L8c075422: *stop=0x8c075422u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c075424; /* mov.l r11,@-r15 */
L8c075424: *stop=0x8c075424u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c075426; /* mov.l r10,@-r15 */
L8c075426: *stop=0x8c075426u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c075428; /* mov.l r9,@-r15 */
L8c075428: *stop=0x8c075428u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c07542a; /* mov.l r8,@-r15 */
L8c07542a: *stop=0x8c07542au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c07542c; /* fmov.s fr15,@-r15 */
L8c07542c: *stop=0x8c07542cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c07542e; /* fmov.s fr14,@-r15 */
L8c07542e: *stop=0x8c07542eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c075430; /* fmov.s fr13,@-r15 */
L8c075430: *stop=0x8c075430u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c075432; /* fmov.s fr12,@-r15 */
L8c075432: *stop=0x8c075432u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c075434; /* sts.l PR,@-r15 */
L8c075434: *stop=0x8c075434u;r[15]+=r[0];goto L8c075436; /* add r0,r15 */
L8c075436: *stop=0x8c075436u;r[0]=0x8c0754e4u;goto L8c075438; /* mova 0x8c0754e4,r0 */
L8c075438: *stop=0x8c075438u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0;goto L8c07543a; /* fldi0 fr14 */
L8c07543a: *stop=0x8c07543au;r[3]=0u;goto L8c07543c; /* mov #0x0,r3 */
L8c07543c: *stop=0x8c07543cu;r[14]=r[7];goto L8c07543e; /* mov r7,r14 */
L8c07543e: *stop=0x8c07543eu;r[12]=r[3];goto L8c075440; /* mov r3,r12 */
L8c075440: *stop=0x8c075440u;r[9]=r[15];goto L8c075442; /* mov r15,r9 */
L8c075442: *stop=0x8c075442u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0754ecu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c075444; /* mov.l 0x8c0754ec,r8 */
L8c075444: *stop=0x8c075444u;r[2]=4294967295u;goto L8c075446; /* mov #-0x1,r2 */
L8c075446: *stop=0x8c075446u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c075448; /* mov.l r6,@(0x8,r15) */
L8c075448: *stop=0x8c075448u;r[13]=r[5];goto L8c07544a; /* mov r5,r13 */
L8c07544a: *stop=0x8c07544au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c07544c; /* mov.l r3,@(0x4,r15) */
L8c07544c: *stop=0x8c07544cu;r[9]+=40u;goto L8c07544e; /* add #0x28,r9 */
L8c07544e: *stop=0x8c07544eu;r[10]=r[15];goto L8c075450; /* mov r15,r10 */
L8c075450: *stop=0x8c075450u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c075452; /* mov.l r2,@(0xc,r15) */
L8c075452: *stop=0x8c075452u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]);if(io_status)return io_status;goto L8c075454; /* fmov.s @r0,fr13 */
L8c075454: *stop=0x8c075454u;r[0]=36u;goto L8c075456; /* mov #0x24,r0 */
L8c075456: *stop=0x8c075456u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[15]);if(io_status)return io_status;goto L8c075458; /* fmov fr14,@(r0,r15) */
L8c075458: *stop=0x8c075458u;r[0]=32u;goto L8c07545a; /* mov #0x20,r0 */
L8c07545a: *stop=0x8c07545au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[15]);if(io_status)return io_status;goto L8c07545c; /* fmov fr14,@(r0,r15) */
L8c07545c: *stop=0x8c07545cu;r[0]=28u;goto L8c07545e; /* mov #0x1c,r0 */
L8c07545e: *stop=0x8c07545eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[15]);if(io_status)return io_status;goto L8c075460; /* fmov fr14,@(r0,r15) */
L8c075460: *stop=0x8c075460u;r[0]=0x8c0754e8u;goto L8c075462; /* mova 0x8c0754e8,r0 */
L8c075462: *stop=0x8c075462u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c075464; /* fmov.s @r0,fr12 */
L8c075464: *stop=0x8c075464u;r[0]=40u;goto L8c075466; /* mov #0x28,r0 */
L8c075466: *stop=0x8c075466u;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c075468; /* mov.w @(r0,r14),r3 */
L8c075468: *stop=0x8c075468u;r[3]=r[3]&65535u;goto L8c07546a; /* extu.w r3,r3 */
L8c07546a: *stop=0x8c07546au;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c07546c; /* cmp/pl r3 */
L8c07546c: take=(*sr&1u);*stop=0x8c07546eu;r[10]+=104u;if(take)goto L8c075474;goto L8c075470; /* bt/s 0x8c075474 */
L8c075470: *stop=0x8c075472u;goto L8c075602; /* bra 0x8c075602 */
L8c075474: *stop=0x8c075474u;r[11]=88u;goto L8c075476; /* mov #0x58,r11 */
L8c075476: *stop=0x8c075476u;r[2]=r[12];goto L8c075478; /* mov r12,r2 */
L8c075478: *stop=0x8c075478u;*macl=r[11]*r[12];goto L8c07547a; /* mul.l r11,r12 */
L8c07547a: *stop=0x8c07547au;r[2]<<=2;goto L8c07547c; /* shll2 r2 */
L8c07547c: *stop=0x8c07547cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0754f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07547e; /* mov.l 0x8c0754f0,r3 */
L8c07547e: *stop=0x8c07547eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c075480; /* mov.l r2,@r15 */
L8c075480: *stop=0x8c075480u;r[2]+=r[10];goto L8c075482; /* add r10,r2 */
L8c075482: *stop=0x8c075482u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c075484; /* mov.l r2,@-r15 */
L8c075484: *stop=0x8c075484u;r[5]=r[14];goto L8c075486; /* mov r14,r5 */
L8c075486: *stop=0x8c075486u;r[11]=*macl;goto L8c075488; /* sts MACL,r11 */
L8c075488: *stop=0x8c075488u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0754f4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07548a; /* mov.l 0x8c0754f4,r2 */
L8c07548a: *stop=0x8c07548au;r[5]+=4u;goto L8c07548c; /* add #0x4,r5 */
L8c07548c: *stop=0x8c07548cu;r[11]+=r[3];goto L8c07548e; /* add r3,r11 */
L8c07548e: *stop=0x8c07548eu;r[4]=r[11];goto L8c075490; /* mov r11,r4 */
L8c075490: target=r[2];*pr=0x8c075494u;*stop=0x8c075492u;r[4]+=44u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075494; /* jsr @r2 */
L8c075494: *stop=0x8c075494u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c075496; /* mov.l @r15+,r3 */
L8c075496: *stop=0x8c075496u;r[5]=r[11];goto L8c075498; /* mov r11,r5 */
L8c075498: *stop=0x8c075498u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0754f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07549a; /* mov.l 0x8c0754f8,r2 */
L8c07549a: *stop=0x8c07549au;r[4]=r[14];goto L8c07549c; /* mov r14,r4 */
L8c07549c: *stop=0x8c07549cu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[3]);if(io_status)return io_status;goto L8c07549e; /* fmov.s fr0,@r3 */
L8c07549e: *stop=0x8c07549eu;r[3]=0u;goto L8c0754a0; /* mov #0x0,r3 */
L8c0754a0: *stop=0x8c0754a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c0754a2; /* mov.l @r15,r0 */
L8c0754a2: *stop=0x8c0754a2u;r[5]+=20u;goto L8c0754a4; /* add #0x14,r5 */
L8c0754a4: *stop=0x8c0754a4u;r[6]=r[3];goto L8c0754a6; /* mov r3,r6 */
L8c0754a6: *stop=0x8c0754a6u;if(!valid_access(r[0]+r[9],4u))return -3;wr(m,bus,&io_status,r[9]+r[0],r[3],4);if(io_status)return io_status;goto L8c0754a8; /* mov.l r3,@(r0,r9) */
L8c0754a8: target=r[2];*pr=0x8c0754acu;*stop=0x8c0754aau;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0754ac; /* jsr @r2 */
L8c0754ac: *stop=0x8c0754acu;r[3]=44u;goto L8c0754ae; /* mov #0x2c,r3 */
L8c0754ae: *stop=0x8c0754aeu;r[3]<<=8;goto L8c0754b0; /* shll8 r3 */
L8c0754b0: *stop=0x8c0754b0u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[3]);goto L8c0754b2; /* cmp/gt r3,r0 */
L8c0754b2: take=!(*sr&1u);*stop=0x8c0754b4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;if(take)goto L8c0754d2;goto L8c0754b6; /* bf/s 0x8c0754d2 */
L8c0754b6: *stop=0x8c0754b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075500u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0754b8; /* mov.l 0x8c075500,r3 */
L8c0754b8: *stop=0x8c0754b8u;r[5]=r[14];goto L8c0754ba; /* mov r14,r5 */
L8c0754ba: *stop=0x8c0754bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c0754fcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0754bc; /* mov.l 0x8c0754fc,r4 */
L8c0754bc: target=r[3];*pr=0x8c0754c0u;*stop=0x8c0754beu;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0754c0; /* jsr @r3 */
L8c0754c0: *stop=0x8c0754c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0754fcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0754c2; /* mov.l 0x8c0754fc,r4 */
L8c0754c2: *stop=0x8c0754c2u;r[5]=r[11];goto L8c0754c4; /* mov r11,r5 */
L8c0754c4: *stop=0x8c0754c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075500u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0754c6; /* mov.l 0x8c075500,r2 */
L8c0754c6: *stop=0x8c0754c6u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c0754c8; /* fmov.s fr0,@-r15 */
L8c0754c8: target=r[2];*pr=0x8c0754ccu;*stop=0x8c0754cau;r[5]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0754cc; /* jsr @r2 */
L8c0754cc: *stop=0x8c0754ccu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0754ce; /* fmov.s @r15+,fr3 */
L8c0754ce: *stop=0x8c0754ceu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0754d0; /* fcmp/gt fr0,fr3 */
L8c0754d0: if(!(*sr&1u))goto L8c075520;goto L8c0754d2; /* bf 0x8c075520 */
L8c0754d2: *stop=0x8c0754d2u;r[0]=r[12];goto L8c0754d4; /* mov r12,r0 */
L8c0754d4: *stop=0x8c0754d4u;r[0]<<=2;goto L8c0754d6; /* shll2 r0 */
L8c0754d6: *stop=0x8c0754d6u;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0754d8; /* mov.l @(r0,r9),r2 */
L8c0754d8: *stop=0x8c0754d8u;r[3]=2u;goto L8c0754da; /* mov #0x2,r3 */
L8c0754da: *stop=0x8c0754dau;r[2]|=r[3];goto L8c0754dc; /* or r3,r2 */
L8c0754dc: *stop=0x8c0754deu;if(!valid_access(r[0]+r[9],4u))return -3;wr(m,bus,&io_status,r[9]+r[0],r[2],4);if(io_status)return io_status;goto L8c0755f2; /* bra 0x8c0755f2 */
L8c075520: *stop=0x8c075520u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075522; /* mov.l @(0x10,r14),r1 */
L8c075522: *stop=0x8c075522u;r[0]=r[15];goto L8c075524; /* mov r15,r0 */
L8c075524: *stop=0x8c075524u;r[0]+=16u;goto L8c075526; /* add #0x10,r0 */
L8c075526: *stop=0x8c075526u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075528; /* mov.l @(0x14,r14),r3 */
L8c075528: *stop=0x8c075528u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c07552a; /* mov.l r1,@r0 */
L8c07552a: *stop=0x8c07552au;r[4]=r[15];goto L8c07552c; /* mov r15,r4 */
L8c07552c: *stop=0x8c07552cu;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[3],4);if(io_status)return io_status;goto L8c07552e; /* mov.l r3,@(0x4,r0) */
L8c07552e: *stop=0x8c07552eu;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075530; /* mov.l @(0x18,r14),r1 */
L8c075530: *stop=0x8c075530u;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[1],4);if(io_status)return io_status;goto L8c075532; /* mov.l r1,@(0x8,r0) */
L8c075532: target=r[8];*pr=0x8c075536u;*stop=0x8c075534u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075536; /* jsr @r8 */
L8c075536: *stop=0x8c075536u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075538; /* fldi0 fr3 */
L8c075538: *stop=0x8c075538u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c07553a; /* fcmp/eq fr3,fr0 */
L8c07553a: if((*sr&1u))goto L8c075544;goto L8c07553c; /* bt 0x8c075544 */
L8c07553c: *stop=0x8c07553cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0756c0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07553e; /* mov.l 0x8c0756c0,r2 */
L8c07553e: *stop=0x8c07553eu;r[4]=r[15];goto L8c075540; /* mov r15,r4 */
L8c075540: target=r[2];*pr=0x8c075544u;*stop=0x8c075542u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075544; /* jsr @r2 */
L8c075544: *stop=0x8c075544u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0756c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075546; /* mov.l 0x8c0756c4,r3 */
L8c075546: *stop=0x8c075546u;r[5]=r[11];goto L8c075548; /* mov r11,r5 */
L8c075548: *stop=0x8c075548u;r[4]=r[15];goto L8c07554a; /* mov r15,r4 */
L8c07554a: *stop=0x8c07554au;r[5]+=20u;goto L8c07554c; /* add #0x14,r5 */
L8c07554c: target=r[3];*pr=0x8c075550u;*stop=0x8c07554eu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075550; /* jsr @r3 */
L8c075550: *stop=0x8c075550u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075552; /* fldi0 fr3 */
L8c075552: *stop=0x8c075552u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c075554; /* fcmp/gt fr0,fr3 */
L8c075554: take=!(*sr&1u);*stop=0x8c075556u;fcopy(f,15u,0u);if(take)goto L8c0755f2;goto L8c075558; /* bf/s 0x8c0755f2 */
L8c075558: *stop=0x8c075558u;r[0]=r[12];goto L8c07555a; /* mov r12,r0 */
L8c07555a: *stop=0x8c07555au;r[0]<<=2;goto L8c07555c; /* shll2 r0 */
L8c07555c: *stop=0x8c07555cu;r[3]=8u;goto L8c07555e; /* mov #0x8,r3 */
L8c07555e: *stop=0x8c07555eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c075560; /* mov.l r0,@r15 */
L8c075560: *stop=0x8c075560u;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c075562; /* mov.l @(r0,r9),r1 */
L8c075562: *stop=0x8c075562u;r[1]|=r[3];goto L8c075564; /* or r3,r1 */
L8c075564: *stop=0x8c075564u;if(!valid_access(r[0]+r[9],4u))return -3;wr(m,bus,&io_status,r[9]+r[0],r[1],4);if(io_status)return io_status;goto L8c075566; /* mov.l r1,@(r0,r9) */
L8c075566: *stop=0x8c075566u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075568; /* mov.l @(0x4,r15),r2 */
L8c075568: *stop=0x8c075568u;r[2]+=1u;goto L8c07556a; /* add #0x1,r2 */
L8c07556a: *stop=0x8c07556au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c07556c; /* mov.l r2,@(0x4,r15) */
L8c07556c: *stop=0x8c07556cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c07556e; /* mov.l @r15,r0 */
L8c07556e: *stop=0x8c07556eu;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c075570; /* fmov.s @(r0,r10),fr2 */
L8c075570: *stop=0x8c075570u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][13],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c075572; /* fcmp/gt fr2,fr13 */
L8c075572: if(!(*sr&1u))goto L8c07557a;goto L8c075574; /* bf 0x8c07557a */
L8c075574: *stop=0x8c075574u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c075576; /* mov.l @r15,r0 */
L8c075576: *stop=0x8c075576u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]+r[10]);if(io_status)return io_status;goto L8c075578; /* fmov.s @(r0,r10),fr13 */
L8c075578: *stop=0x8c075578u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[12],4);if(io_status)return io_status;goto L8c07557a; /* mov.l r12,@(0xc,r15) */
L8c07557a: *stop=0x8c07557au;r[0]=4294967256u;goto L8c07557c; /* mov #-0x28,r0 */
L8c07557c: *stop=0x8c07557cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0756c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07557e; /* mov.l 0x8c0756c8,r3 */
L8c07557e: *stop=0x8c07557eu;r[0]=r[0]&255u;goto L8c075580; /* extu.b r0,r0 */
L8c075580: *stop=0x8c075580u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c075582; /* mov.l @(r0,r15),r2 */
L8c075582: *stop=0x8c075582u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c075584; /* tst r3,r2 */
L8c075584: if(!(*sr&1u))goto L8c0755e6;goto L8c075586; /* bf 0x8c0755e6 */
L8c075586: *stop=0x8c075586u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0756ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075588; /* mov.l 0x8c0756cc,r2 */
L8c075588: *stop=0x8c075588u;r[4]=6u;goto L8c07558a; /* mov #0x6,r4 */
L8c07558a: *stop=0x8c07558au;r[4]<<=8;goto L8c07558c; /* shll8 r4 */
L8c07558c: target=r[2];*pr=0x8c075590u;*stop=0x8c07558eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075590; /* jsr @r2 */
L8c075590: *stop=0x8c075590u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto L8c075592; /* fneg fr0 */
L8c075592: *stop=0x8c075592u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][15],&f->fpscr,sr,1);if(status)return status;goto L8c075594; /* fcmp/gt fr15,fr0 */
L8c075594: take=!(*sr&1u);*stop=0x8c075596u;r[5]=r[11];if(take)goto L8c0755a4;goto L8c075598; /* bf/s 0x8c0755a4 */
L8c075598: *stop=0x8c075598u;r[0]=12u;goto L8c07559a; /* mov #0xc,r0 */
L8c07559a: *stop=0x8c07559au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[13]);if(io_status)return io_status;goto L8c07559c; /* fmov fr14,@(r0,r13) */
L8c07559c: *stop=0x8c07559cu;r[0]=8u;goto L8c07559e; /* mov #0x8,r0 */
L8c07559e: *stop=0x8c07559eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[13]);if(io_status)return io_status;goto L8c0755a0; /* fmov fr14,@(r0,r13) */
L8c0755a0: *stop=0x8c0755a0u;r[0]=4u;goto L8c0755a2; /* mov #0x4,r0 */
L8c0755a2: *stop=0x8c0755a2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[13]);if(io_status)return io_status;goto L8c0755a4; /* fmov fr14,@(r0,r13) */
L8c0755a4: *stop=0x8c0755a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0756c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0755a6; /* mov.l 0x8c0756c4,r3 */
L8c0755a6: *stop=0x8c0755a6u;r[4]=r[14];goto L8c0755a8; /* mov r14,r4 */
L8c0755a8: *stop=0x8c0755a8u;r[5]+=20u;goto L8c0755aa; /* add #0x14,r5 */
L8c0755aa: target=r[3];*pr=0x8c0755aeu;*stop=0x8c0755acu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0755ae; /* jsr @r3 */
L8c0755ae: *stop=0x8c0755aeu;fcopy(f,4u,0u);goto L8c0755b0; /* fmov fr0,fr4 */
L8c0755b0: *stop=0x8c0755b0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c0755b2; /* fcmp/gt fr4,fr12 */
L8c0755b2: if(!(*sr&1u))goto L8c0755e6;goto L8c0755b4; /* bf 0x8c0755e6 */
L8c0755b4: *stop=0x8c0755b4u;r[0]=4294967236u;goto L8c0755b6; /* mov #-0x3c,r0 */
L8c0755b6: *stop=0x8c0755b6u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0755b8; /* mov.l @(0x8,r15),r3 */
L8c0755b8: *stop=0x8c0755b8u;r[0]=r[0]&255u;goto L8c0755ba; /* extu.b r0,r0 */
L8c0755ba: *stop=0x8c0755bau;r[4]=r[13];goto L8c0755bc; /* mov r13,r4 */
L8c0755bc: *stop=0x8c0755bcu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[3]);if(io_status)return io_status;goto L8c0755be; /* fmov.s @(r0,r3),fr15 */
L8c0755be: target=r[8];*pr=0x8c0755c2u;*stop=0x8c0755c0u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0755c2; /* jsr @r8 */
L8c0755c2: *stop=0x8c0755c2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][15],&f->fpscr,sr,1);if(status)return status;goto L8c0755c4; /* fcmp/gt fr15,fr0 */
L8c0755c4: if(!(*sr&1u))goto L8c0755e6;goto L8c0755c6; /* bf 0x8c0755e6 */
L8c0755c6: *stop=0x8c0755c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0756c0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0755c8; /* mov.l 0x8c0756c0,r2 */
L8c0755c8: *stop=0x8c0755c8u;r[4]=r[13];goto L8c0755ca; /* mov r13,r4 */
L8c0755ca: target=r[2];*pr=0x8c0755ceu;*stop=0x8c0755ccu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0755ce; /* jsr @r2 */
L8c0755ce: *stop=0x8c0755ceu;r[0]=4u;goto L8c0755d0; /* mov #0x4,r0 */
L8c0755d0: *stop=0x8c0755d0u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0755d2; /* fmov.s @(r0,r13),fr3 */
L8c0755d2: *stop=0x8c0755d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0755d4; /* fmul fr15,fr3 */
L8c0755d4: *stop=0x8c0755d4u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0755d6; /* fmov fr3,@(r0,r13) */
L8c0755d6: *stop=0x8c0755d6u;r[0]=8u;goto L8c0755d8; /* mov #0x8,r0 */
L8c0755d8: *stop=0x8c0755d8u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0755da; /* fmov.s @(r0,r13),fr2 */
L8c0755da: *stop=0x8c0755dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0755dc; /* fmul fr15,fr2 */
L8c0755dc: *stop=0x8c0755dcu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0755de; /* fmov fr2,@(r0,r13) */
L8c0755de: *stop=0x8c0755deu;r[0]=12u;goto L8c0755e0; /* mov #0xc,r0 */
L8c0755e0: *stop=0x8c0755e0u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0755e2; /* fmov.s @(r0,r13),fr3 */
L8c0755e2: *stop=0x8c0755e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0755e4; /* fmul fr15,fr3 */
L8c0755e4: *stop=0x8c0755e4u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0755e6; /* fmov fr3,@(r0,r13) */
L8c0755e6: *stop=0x8c0755e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0756d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0755e8; /* mov.l 0x8c0756d0,r3 */
L8c0755e8: *stop=0x8c0755e8u;r[5]=r[11];goto L8c0755ea; /* mov r11,r5 */
L8c0755ea: *stop=0x8c0755eau;r[4]=r[15];goto L8c0755ec; /* mov r15,r4 */
L8c0755ec: *stop=0x8c0755ecu;r[5]+=20u;goto L8c0755ee; /* add #0x14,r5 */
L8c0755ee: target=r[3];*pr=0x8c0755f2u;*stop=0x8c0755f0u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0755f2; /* jsr @r3 */
L8c0755f2: *stop=0x8c0755f2u;r[0]=40u;goto L8c0755f4; /* mov #0x28,r0 */
L8c0755f4: *stop=0x8c0755f4u;if(!valid_access(r[0]+r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c0755f6; /* mov.w @(r0,r14),r2 */
L8c0755f6: *stop=0x8c0755f6u;r[12]+=1u;goto L8c0755f8; /* add #0x1,r12 */
L8c0755f8: *stop=0x8c0755f8u;r[2]=r[2]&65535u;goto L8c0755fa; /* extu.w r2,r2 */
L8c0755fa: *stop=0x8c0755fau;*sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[2]);goto L8c0755fc; /* cmp/ge r2,r12 */
L8c0755fc: if((*sr&1u))goto L8c075602;goto L8c0755fe; /* bt 0x8c075602 */
L8c0755fe: *stop=0x8c075600u;goto L8c075474; /* bra 0x8c075474 */
L8c075602: *stop=0x8c075602u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075604; /* mov.l @(0x4,r15),r2 */
L8c075604: *stop=0x8c075604u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c075606; /* cmp/pl r2 */
L8c075606: if(!(*sr&1u))goto L8c075664;goto L8c075608; /* bf 0x8c075664 */
L8c075608: *stop=0x8c075608u;r[4]=r[14];goto L8c07560a; /* mov r14,r4 */
L8c07560a: target=r[8];*pr=0x8c07560eu;*stop=0x8c07560cu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07560e; /* jsr @r8 */
L8c07560e: *stop=0x8c07560eu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c075610; /* mov.l @(0xc,r15),r13 */
L8c075610: *stop=0x8c075610u;fcopy(f,4u,0u);goto L8c075612; /* fmov fr0,fr4 */
L8c075612: *stop=0x8c075612u;r[13]<<=2;goto L8c075614; /* shll2 r13 */
L8c075614: *stop=0x8c075614u;r[0]=r[13];goto L8c075616; /* mov r13,r0 */
L8c075616: *stop=0x8c075616u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c075618; /* fmov.s @(r0,r10),fr3 */
L8c075618: *stop=0x8c075618u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c07561a; /* fcmp/gt fr3,fr4 */
L8c07561a: if(!(*sr&1u))goto L8c075664;goto L8c07561c; /* bf 0x8c075664 */
L8c07561c: *stop=0x8c07561cu;r[4]=r[14];goto L8c07561e; /* mov r14,r4 */
L8c07561e: target=r[8];*pr=0x8c075622u;*stop=0x8c075620u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075622; /* jsr @r8 */
L8c075622: *stop=0x8c075622u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075624; /* fldi0 fr3 */
L8c075624: *stop=0x8c075624u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c075626; /* fcmp/eq fr3,fr0 */
L8c075626: if(!(*sr&1u))goto L8c075636;goto L8c075628; /* bf 0x8c075636 */
L8c075628: *stop=0x8c075628u;r[0]=24u;goto L8c07562a; /* mov #0x18,r0 */
L8c07562a: *stop=0x8c07562au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c07562c; /* fmov fr14,@(r0,r14) */
L8c07562c: *stop=0x8c07562cu;r[0]=20u;goto L8c07562e; /* mov #0x14,r0 */
L8c07562e: *stop=0x8c07562eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c075630; /* fmov fr14,@(r0,r14) */
L8c075630: *stop=0x8c075630u;r[0]=16u;goto L8c075632; /* mov #0x10,r0 */
L8c075632: *stop=0x8c075634u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c075660; /* bra 0x8c075660 */
L8c075636: *stop=0x8c075636u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0756c0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075638; /* mov.l 0x8c0756c0,r2 */
L8c075638: *stop=0x8c075638u;r[4]=r[14];goto L8c07563a; /* mov r14,r4 */
L8c07563a: target=r[2];*pr=0x8c07563eu;*stop=0x8c07563cu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07563e; /* jsr @r2 */
L8c07563e: *stop=0x8c07563eu;r[4]=r[13];goto L8c075640; /* mov r13,r4 */
L8c075640: *stop=0x8c075640u;r[4]+=r[10];goto L8c075642; /* add r10,r4 */
L8c075642: *stop=0x8c075642u;r[0]=16u;goto L8c075644; /* mov #0x10,r0 */
L8c075644: *stop=0x8c075644u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c075646; /* fmov.s @r4,fr3 */
L8c075646: *stop=0x8c075646u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075648; /* fmov.s @(r0,r14),fr2 */
L8c075648: *stop=0x8c075648u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c07564a; /* fmul fr3,fr2 */
L8c07564a: *stop=0x8c07564au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07564c; /* fmov fr2,@(r0,r14) */
L8c07564c: *stop=0x8c07564cu;r[0]=20u;goto L8c07564e; /* mov #0x14,r0 */
L8c07564e: *stop=0x8c07564eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075650; /* fmov.s @(r0,r14),fr2 */
L8c075650: *stop=0x8c075650u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c075652; /* fmov.s @r4,fr3 */
L8c075652: *stop=0x8c075652u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c075654; /* fmul fr3,fr2 */
L8c075654: *stop=0x8c075654u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075656; /* fmov fr2,@(r0,r14) */
L8c075656: *stop=0x8c075656u;r[0]=24u;goto L8c075658; /* mov #0x18,r0 */
L8c075658: *stop=0x8c075658u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07565a; /* fmov.s @(r0,r14),fr2 */
L8c07565a: *stop=0x8c07565au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c07565c; /* fmov.s @r4,fr3 */
L8c07565c: *stop=0x8c07565cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c07565e; /* fmul fr3,fr2 */
L8c07565e: *stop=0x8c07565eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075660; /* fmov fr2,@(r0,r14) */
L8c075660: *stop=0x8c075662u;r[0]=1u;goto L8c075666; /* bra 0x8c075666 */
L8c075664: *stop=0x8c075664u;r[0]=0u;goto L8c075666; /* mov #0x0,r0 */
L8c075666: *stop=0x8c075666u;r[1]=4294967208u;goto L8c075668; /* mov #-0x58,r1 */
L8c075668: *stop=0x8c075668u;r[1]=r[1]&255u;goto L8c07566a; /* extu.b r1,r1 */
L8c07566a: *stop=0x8c07566au;r[15]+=r[1];goto L8c07566c; /* add r1,r15 */
L8c07566c: *stop=0x8c07566cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c07566e; /* lds.l @r15+,PR */
L8c07566e: *stop=0x8c07566eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075670; /* fmov.s @r15+,fr12 */
L8c075670: *stop=0x8c075670u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075672; /* fmov.s @r15+,fr13 */
L8c075672: *stop=0x8c075672u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075674; /* fmov.s @r15+,fr14 */
L8c075674: *stop=0x8c075674u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075676; /* fmov.s @r15+,fr15 */
L8c075676: *stop=0x8c075676u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c075678; /* mov.l @r15+,r8 */
L8c075678: *stop=0x8c075678u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c07567a; /* mov.l @r15+,r9 */
L8c07567a: *stop=0x8c07567au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c07567c; /* mov.l @r15+,r10 */
L8c07567c: *stop=0x8c07567cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c07567e; /* mov.l @r15+,r11 */
L8c07567e: *stop=0x8c07567eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c075680; /* mov.l @r15+,r12 */
L8c075680: *stop=0x8c075680u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c075682; /* mov.l @r15+,r13 */
L8c075682: *stop=0x8c075682u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c075684u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_response_8c075416(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c075494(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075494u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c0754ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0754acu);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c0754c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0754c0u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c0754cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0754ccu);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c075536(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075536u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c075544(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075544u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c075550(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075550u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c075590(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075590u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c0755ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0755aeu);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c0755c2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0755c2u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c0755ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0755ceu);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c0755f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0755f2u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c07560e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07560eu);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c075622(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075622u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c07563e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c075416(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07563eu);}

static int sa_movement_response_run_8c055dac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c055dc0u)goto L8c055dc0;if(resume_pc==0x8c055dc6u)goto L8c055dc6;if(resume_pc==0x8c055ddeu)goto L8c055dde;if(resume_pc==0x8c055deeu)goto L8c055dee;goto L8c055dac;
L8c055dac: *stop=0x8c055dacu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c055dae; /* mov.l r14,@-r15 */
L8c055dae: *stop=0x8c055daeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c055db0; /* sts.l PR,@-r15 */
L8c055db0: *stop=0x8c055db0u;r[15]+=4294967272u;goto L8c055db2; /* add #-0x18,r15 */
L8c055db2: *stop=0x8c055db2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c055e28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c055db4; /* mov.l 0x8c055e28,r3 */
L8c055db4: *stop=0x8c055db4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c055db6; /* mov.l r4,@r15 */
L8c055db6: *stop=0x8c055db6u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c055db8; /* mov.l r5,@(0x8,r15) */
L8c055db8: *stop=0x8c055db8u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c055dba; /* mov.l r6,@(0x4,r15) */
L8c055dba: *stop=0x8c055dbau;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c055dbc; /* mov.l @(0x8,r15),r5 */
L8c055dbc: target=r[3];*pr=0x8c055dc0u;*stop=0x8c055dbeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c055dc0; /* jsr @r3 */
L8c055dc0: *stop=0x8c055dc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c055e2cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c055dc2; /* mov.l 0x8c055e2c,r2 */
L8c055dc2: target=r[2];*pr=0x8c055dc6u;*stop=0x8c055dc4u;fcopy(f,4u,0u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c055dc6; /* jsr @r2 */
L8c055dc6: *stop=0x8c055dc6u;r[0]=0x8c055e30u;goto L8c055dc8; /* mova 0x8c055e30,r0 */
L8c055dc8: *stop=0x8c055dc8u;fcopy(f,2u,0u);goto L8c055dca; /* fmov fr0,fr2 */
L8c055dca: *stop=0x8c055dcau;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c055dcc; /* fmov.s @r0,fr3 */
L8c055dcc: *stop=0x8c055dccu;r[6]=r[15];goto L8c055dce; /* mov r15,r6 */
L8c055dce: *stop=0x8c055dceu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c055dd0; /* mov.l @(0x8,r15),r5 */
L8c055dd0: *stop=0x8c055dd0u;r[6]+=12u;goto L8c055dd2; /* add #0xc,r6 */
L8c055dd2: *stop=0x8c055dd2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c055dd4; /* fmul fr3,fr2 */
L8c055dd4: *stop=0x8c055dd4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c055e34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c055dd6; /* mov.l 0x8c055e34,r3 */
L8c055dd6: *stop=0x8c055dd6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c055dd8; /* ftrc fr2,FPUL */
L8c055dd8: *stop=0x8c055dd8u;r[14]=*fpul;goto L8c055dda; /* sts FPUL,r14 */
L8c055dda: target=r[3];*pr=0x8c055ddeu;*stop=0x8c055ddcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c055dde; /* jsr @r3 */
L8c055dde: *stop=0x8c055ddeu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c055de0; /* mov.l @(0x4,r15),r2 */
L8c055de0: *stop=0x8c055de0u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c055de2; /* tst r2,r2 */
L8c055de2: if((*sr&1u))goto L8c055dfa;goto L8c055de4; /* bt 0x8c055dfa */
L8c055de4: *stop=0x8c055de4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c055e28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c055de6; /* mov.l 0x8c055e28,r3 */
L8c055de6: *stop=0x8c055de6u;r[5]=r[15];goto L8c055de8; /* mov r15,r5 */
L8c055de8: *stop=0x8c055de8u;r[5]+=12u;goto L8c055dea; /* add #0xc,r5 */
L8c055dea: target=r[3];*pr=0x8c055deeu;*stop=0x8c055decu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c055dee; /* jsr @r3 */
L8c055dee: *stop=0x8c055deeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c055df0; /* fldi0 fr3 */
L8c055df0: *stop=0x8c055df0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c055df2; /* fcmp/gt fr0,fr3 */
L8c055df2: if(!(*sr&1u))goto L8c055dfa;goto L8c055df4; /* bf 0x8c055dfa */
L8c055df4: *stop=0x8c055df4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c055e24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c055df6; /* mov.l 0x8c055e24,r2 */
L8c055df6: *stop=0x8c055df6u;r[2]-=r[14];goto L8c055df8; /* sub r14,r2 */
L8c055df8: *stop=0x8c055df8u;r[14]=r[2];goto L8c055dfa; /* mov r2,r14 */
L8c055dfa: *stop=0x8c055dfau;r[0]=r[14];goto L8c055dfc; /* mov r14,r0 */
L8c055dfc: *stop=0x8c055dfcu;r[15]+=24u;goto L8c055dfe; /* add #0x18,r15 */
L8c055dfe: *stop=0x8c055dfeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c055e00; /* lds.l @r15+,PR */
L8c055e00: *stop=0x8c055e00u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c055e02u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_response_8c055dac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c055dac(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c055dc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c055dac(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c055dc0u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c055dc6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c055dac(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c055dc6u);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c055dde(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c055dac(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c055ddeu);}
SA_STAGE_EXPORT int sa_movement_response_resume_8c055dee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_response_run_8c055dac(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c055deeu);}

SA_STAGE_EXPORT int sa_movement_response_8c63a970(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63a970;
L8c63a970: *stop=0x8c63a970u;r[4]=r[4]&65535u;goto L8c63a972; /* extu.w r4,r4 */
L8c63a972: *stop=0x8c63a972u;*fpul=r[4];goto L8c63a974; /* lds r4,FPUL */
L8c63a974: *stop=0x8c63a974u;status=sa_sh4_vector_numeric(0xf0fdu,f,*fpul);if(status)return status;goto L8c63a976; /* fsca FPUL,dr0 */
L8c63a976: *stop=0x8c63a976u;if(*pr!=entry_pr)return -3;*stop=0x8c63a978u;fcopy(f,0u,1u);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movement_response_8c638eac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c638eac;
L8c638eac: *stop=0x8c638eacu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c638eae; /* fmov.s @r4+,fr4 */
L8c638eae: *stop=0x8c638eaeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c638eb0; /* fmov.s @r5+,fr1 */
L8c638eb0: *stop=0x8c638eb0u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c638eb2; /* fmov.s @r4+,fr5 */
L8c638eb2: *stop=0x8c638eb2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c638eb4; /* fadd fr4,fr1 */
L8c638eb4: *stop=0x8c638eb4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c638eb6; /* fmov.s @r5+,fr2 */
L8c638eb6: *stop=0x8c638eb6u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c638eb8; /* fmov.s @r4,fr6 */
L8c638eb8: *stop=0x8c638eb8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c638eba; /* fadd fr5,fr2 */
L8c638eba: *stop=0x8c638ebau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c638ebc; /* fmov.s @r5,fr3 */
L8c638ebc: *stop=0x8c638ebcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c638ebe; /* fadd fr6,fr3 */
L8c638ebe: *stop=0x8c638ebeu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c638ec0; /* fmov.s fr3,@r4 */
L8c638ec0: *stop=0x8c638ec0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c638ec2; /* fmov.s fr2,@-r4 */
L8c638ec2: *stop=0x8c638ec2u;if(*pr!=entry_pr)return -3;*stop=0x8c638ec4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movement_response_8c639b50(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c639b50;
L8c639b50: *stop=0x8c639b50u;r[0]=4u;goto L8c639b52; /* mov #0x4,r0 */
L8c639b52: *stop=0x8c639b52u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c639b54; /* fmov.s @r4,fr10 */
L8c639b54: *stop=0x8c639b54u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;goto L8c639b56; /* fmov.s @r5,fr6 */
L8c639b56: *stop=0x8c639b56u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b58; /* fmov.s @(r0,r4),fr4 */
L8c639b58: *stop=0x8c639b58u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c639b5a; /* fmov.s @(r0,r5),fr5 */
L8c639b5a: *stop=0x8c639b5au;r[0]=8u;goto L8c639b5c; /* mov #0x8,r0 */
L8c639b5c: *stop=0x8c639b5cu;fcopy(f,8u,4u);goto L8c639b5e; /* fmov fr4,fr8 */
L8c639b5e: *stop=0x8c639b5eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b60; /* fmov.s @(r0,r4),fr9 */
L8c639b60: *stop=0x8c639b60u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]+r[5]);if(io_status)return io_status;goto L8c639b62; /* fmov.s @(r0,r5),fr11 */
L8c639b62: *stop=0x8c639b62u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[11],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639b64; /* fmul fr11,fr8 */
L8c639b64: *stop=0x8c639b64u;fcopy(f,0u,9u);goto L8c639b66; /* fmov fr9,fr0 */
L8c639b66: *stop=0x8c639b66u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto L8c639b68; /* fneg fr0 */
L8c639b68: *stop=0x8c639b68u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[5],&fr[8],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639b6a; /* fmac fr0,fr5,fr8 */
L8c639b6a: *stop=0x8c639b6au;r[0]=4u;goto L8c639b6c; /* mov #0x4,r0 */
L8c639b6c: *stop=0x8c639b6cu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c639b6e; /* fmov.s fr8,@r6 */
L8c639b6e: *stop=0x8c639b6eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[6],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639b70; /* fmul fr6,fr9 */
L8c639b70: *stop=0x8c639b70u;fcopy(f,0u,10u);goto L8c639b72; /* fmov fr10,fr0 */
L8c639b72: *stop=0x8c639b72u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]^=0x80000000u;goto L8c639b74; /* fneg fr11 */
L8c639b74: *stop=0x8c639b74u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[11],&fr[9],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639b76; /* fmac fr0,fr11,fr9 */
L8c639b76: *stop=0x8c639b76u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,9u,r[0]+r[6]);if(io_status)return io_status;goto L8c639b78; /* fmov fr9,@(r0,r6) */
L8c639b78: *stop=0x8c639b78u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[5],&f->fpscr,&fr[10]);if(status)return status;}goto L8c639b7a; /* fmul fr5,fr10 */
L8c639b7a: *stop=0x8c639b7au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto L8c639b7c; /* fneg fr6 */
L8c639b7c: *stop=0x8c639b7cu;fcopy(f,0u,4u);goto L8c639b7e; /* fmov fr4,fr0 */
L8c639b7e: *stop=0x8c639b7eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[10],&f->fpscr,&fr[10]);if(status)return status;}goto L8c639b80; /* fmac fr0,fr6,fr10 */
L8c639b80: *stop=0x8c639b80u;r[0]=8u;goto L8c639b82; /* mov #0x8,r0 */
L8c639b82: *stop=0x8c639b82u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,10u,r[0]+r[6]);if(io_status)return io_status;goto L8c639b84; /* fmov fr10,@(r0,r6) */
L8c639b84: *stop=0x8c639b84u;fcopy(f,0u,8u);goto L8c639b86; /* fmov fr8,fr0 */
L8c639b86: *stop=0x8c639b86u;fcopy(f,1u,9u);goto L8c639b88; /* fmov fr9,fr1 */
L8c639b88: *stop=0x8c639b88u;fcopy(f,2u,10u);goto L8c639b8a; /* fmov fr10,fr2 */
L8c639b8a: *stop=0x8c639b8au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c639b8c; /* fldi0 fr3 */
L8c639b8c: *stop=0x8c639b8cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]=0;goto L8c639b8e; /* fldi0 fr11 */
L8c639b8e: *stop=0x8c639b8eu;status=sa_sh4_vector_numeric(0xf8edu,f,*fpul);if(status)return status;goto L8c639b90; /* fipr fv0,fv8 */
L8c639b90: *stop=0x8c639b90u;fcopy(f,0u,11u);goto L8c639b92; /* fmov fr11,fr0 */
L8c639b92: *stop=0x8c639b92u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[0],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639b94; /* fsqrt fr0 */
L8c639b94: *stop=0x8c639b94u;if(*pr!=entry_pr)return -3;*stop=0x8c639b96u;return 0; /* rts */
}
