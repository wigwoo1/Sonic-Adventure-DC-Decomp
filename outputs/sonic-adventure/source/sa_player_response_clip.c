/* Original complete response near-plane line clipping body.
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
SA_STAGE_EXPORT int sa_player_response_clip_8c64f5e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64f5e8;
L8c64f5e8: *stop=0x8c64f5e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64f5ea; /* mov.l r13,@-r15 */
L8c64f5ea: *stop=0x8c64f5eau;r[15]+=4294967264u;goto L8c64f5ec; /* add #-0x20,r15 */
L8c64f5ec: *stop=0x8c64f5ecu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f5ee; /* mov.l @r4,r3 */
L8c64f5ee: *stop=0x8c64f5eeu;r[0]=20u;goto L8c64f5f0; /* mov #0x14,r0 */
L8c64f5f0: *stop=0x8c64f5f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f838u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64f5f2; /* mov.l 0x8c64f838,r7 */
L8c64f5f2: *stop=0x8c64f5f2u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[7]);if(io_status)return io_status;goto L8c64f5f4; /* fmov.s @r7,fr4 */
L8c64f5f4: *stop=0x8c64f5f4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64f5f6; /* mov.l r3,@r15 */
L8c64f5f6: *stop=0x8c64f5f6u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f5f8; /* fmov.s @(r0,r3),fr3 */
L8c64f5f8: *stop=0x8c64f5f8u;r[0]=8u;goto L8c64f5fa; /* mov #0x8,r0 */
L8c64f5fa: *stop=0x8c64f5fau;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f5fc; /* fmov.s @(r0,r3),fr2 */
L8c64f5fc: *stop=0x8c64f5fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c64f5fe; /* fsub fr4,fr3 */
L8c64f5fe: *stop=0x8c64f5feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c64f600; /* fsub fr4,fr2 */
L8c64f600: *stop=0x8c64f600u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f602; /* fmul fr2,fr3 */
L8c64f602: *stop=0x8c64f602u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c64f604; /* fldi0 fr2 */
L8c64f604: *stop=0x8c64f604u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c64f606; /* fcmp/gt fr3,fr2 */
L8c64f606: take=(*sr&1u);*stop=0x8c64f608u;r[1]=0u;if(take)goto L8c64f60e;goto L8c64f60a; /* bt/s 0x8c64f60e */
L8c64f60a: *stop=0x8c64f60cu;goto L8c64f754; /* bra 0x8c64f754 */
L8c64f60e: *stop=0x8c64f60eu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f610; /* mov.l @r4,r1 */
L8c64f610: *stop=0x8c64f610u;r[0]=8u;goto L8c64f612; /* mov #0x8,r0 */
L8c64f612: *stop=0x8c64f612u;r[2]=r[1];goto L8c64f614; /* mov r1,r2 */
L8c64f614: *stop=0x8c64f614u;r[2]+=12u;goto L8c64f616; /* add #0xc,r2 */
L8c64f616: *stop=0x8c64f616u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64f618; /* mov.l r2,@r15 */
L8c64f618: *stop=0x8c64f618u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c64f61a; /* fmov.s @(r0,r2),fr5 */
L8c64f61a: *stop=0x8c64f61au;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c64f61c; /* fmov.s @(r0,r1),fr3 */
L8c64f61c: *stop=0x8c64f61cu;r[0]=4u;goto L8c64f61e; /* mov #0x4,r0 */
L8c64f61e: *stop=0x8c64f61eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[5],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c64f620; /* fsub fr5,fr4 */
L8c64f620: *stop=0x8c64f620u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f622; /* mov.l @r5,r3 */
L8c64f622: *stop=0x8c64f622u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c64f624; /* fsub fr5,fr3 */
L8c64f624: *stop=0x8c64f624u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[2]);if(io_status)return io_status;goto L8c64f626; /* fmov.s @r2,fr5 */
L8c64f626: *stop=0x8c64f626u;fcopy(f,1u,5u);goto L8c64f628; /* fmov fr5,fr1 */
L8c64f628: *stop=0x8c64f628u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c64f62a; /* fdiv fr3,fr4 */
L8c64f62a: *stop=0x8c64f62au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c64f62c; /* fmov.s @r1,fr3 */
L8c64f62c: *stop=0x8c64f62cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c64f62e; /* fsub fr5,fr3 */
L8c64f62e: *stop=0x8c64f62eu;fcopy(f,0u,4u);goto L8c64f630; /* fmov fr4,fr0 */
L8c64f630: *stop=0x8c64f630u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f632; /* fmac fr0,fr3,fr1 */
L8c64f632: *stop=0x8c64f632u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c64f634; /* fmov.s fr1,@r3 */
L8c64f634: *stop=0x8c64f634u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f636; /* mov.l @r4,r2 */
L8c64f636: *stop=0x8c64f636u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f638; /* mov.l @r5,r3 */
L8c64f638: *stop=0x8c64f638u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64f63a; /* mov.l r2,@r15 */
L8c64f63a: *stop=0x8c64f63au;r[2]+=12u;goto L8c64f63c; /* add #0xc,r2 */
L8c64f63c: *stop=0x8c64f63cu;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c64f63e; /* mov.l r2,@(0x1c,r15) */
L8c64f63e: *stop=0x8c64f63eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f640; /* mov.l @r15,r1 */
L8c64f640: *stop=0x8c64f640u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c64f642; /* fmov.s @(r0,r2),fr5 */
L8c64f642: *stop=0x8c64f642u;r[0]=4u;goto L8c64f644; /* mov #0x4,r0 */
L8c64f644: *stop=0x8c64f644u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c64f646; /* fmov.s @(r0,r1),fr3 */
L8c64f646: *stop=0x8c64f646u;fcopy(f,1u,5u);goto L8c64f648; /* fmov fr5,fr1 */
L8c64f648: *stop=0x8c64f648u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c64f64a; /* fsub fr5,fr3 */
L8c64f64a: *stop=0x8c64f64au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f64c; /* fmac fr0,fr3,fr1 */
L8c64f64c: *stop=0x8c64f64cu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f64e; /* fmov fr1,@(r0,r3) */
L8c64f64e: *stop=0x8c64f64eu;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c64f650; /* fmov.s @r7,fr3 */
L8c64f650: *stop=0x8c64f650u;r[0]=8u;goto L8c64f652; /* mov #0x8,r0 */
L8c64f652: *stop=0x8c64f652u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f654; /* mov.l @r5,r3 */
L8c64f654: *stop=0x8c64f654u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f656; /* fmov fr3,@(r0,r3) */
L8c64f656: *stop=0x8c64f656u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f83cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f658; /* mov.l 0x8c64f83c,r3 */
L8c64f658: *stop=0x8c64f658u;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c64f65a; /* tst r3,r6 */
L8c64f65a: if((*sr&1u))goto L8c64f6a8;goto L8c64f65c; /* bt 0x8c64f6a8 */
L8c64f65c: *stop=0x8c64f65cu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64f65e; /* mov.l @(0x8,r4),r6 */
L8c64f65e: *stop=0x8c64f65eu;fcopy(f,0u,4u);goto L8c64f660; /* fmov fr4,fr0 */
L8c64f660: *stop=0x8c64f660u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f662; /* mov.l @(0x8,r5),r1 */
L8c64f662: *stop=0x8c64f662u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c64f664; /* mov.l r6,@r15 */
L8c64f664: *stop=0x8c64f664u;r[6]+=4u;goto L8c64f666; /* add #0x4,r6 */
L8c64f666: *stop=0x8c64f666u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[6],4);if(io_status)return io_status;goto L8c64f668; /* mov.l r6,@(0x18,r15) */
L8c64f668: *stop=0x8c64f668u;if(!valid_access(r[6],2u))return -3;r[6]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c64f66a; /* mov.w @r6,r6 */
L8c64f66a: *stop=0x8c64f66au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64f66c; /* mov.l @r15,r0 */
L8c64f66c: *stop=0x8c64f66cu;r[2]=(uint32_t)(int32_t)(int16_t)r[6];goto L8c64f66e; /* exts.w r6,r2 */
L8c64f66e: *stop=0x8c64f66eu;*fpul=r[2];goto L8c64f670; /* lds r2,FPUL */
L8c64f670: *stop=0x8c64f670u;if(!valid_access(r[0],2u))return -3;r[2]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c64f672; /* mov.w @r0,r2 */
L8c64f672: *stop=0x8c64f672u;r[0]=(uint32_t)(int32_t)(int16_t)r[6];goto L8c64f674; /* exts.w r6,r0 */
L8c64f674: *stop=0x8c64f674u;r[2]-=r[0];goto L8c64f676; /* sub r0,r2 */
L8c64f676: *stop=0x8c64f676u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64f678; /* float FPUL,fr3 */
L8c64f678: *stop=0x8c64f678u;*fpul=r[2];goto L8c64f67a; /* lds r2,FPUL */
L8c64f67a: *stop=0x8c64f67au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c64f67c; /* float FPUL,fr1 */
L8c64f67c: *stop=0x8c64f67cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f67e; /* fmac fr0,fr1,fr3 */
L8c64f67e: *stop=0x8c64f67eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c64f680; /* ftrc fr3,FPUL */
L8c64f680: *stop=0x8c64f680u;r[2]=*fpul;goto L8c64f682; /* sts FPUL,r2 */
L8c64f682: *stop=0x8c64f682u;if(!valid_access(r[1],2u))return -3;wr(m,bus,&io_status,r[1],r[2],2);if(io_status)return io_status;goto L8c64f684; /* mov.w r2,@r1 */
L8c64f684: *stop=0x8c64f684u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64f686; /* mov.l @(0x8,r4),r6 */
L8c64f686: *stop=0x8c64f686u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f688; /* mov.l @(0x8,r5),r1 */
L8c64f688: *stop=0x8c64f688u;r[2]=r[6];goto L8c64f68a; /* mov r6,r2 */
L8c64f68a: *stop=0x8c64f68au;r[2]+=4u;goto L8c64f68c; /* add #0x4,r2 */
L8c64f68c: *stop=0x8c64f68cu;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c64f68e; /* mov.l r2,@(0x14,r15) */
L8c64f68e: *stop=0x8c64f68eu;if(!valid_access(r[2]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c64f690; /* mov.w @(0x2,r2),r0 */
L8c64f690: *stop=0x8c64f690u;*fpul=r[0];goto L8c64f692; /* lds r0,FPUL */
L8c64f692: *stop=0x8c64f692u;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c64f694; /* mov.w r0,@r15 */
L8c64f694: *stop=0x8c64f694u;if(!valid_access(r[15],2u))return -3;r[2]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c64f696; /* mov.w @r15,r2 */
L8c64f696: *stop=0x8c64f696u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64f698; /* float FPUL,fr3 */
L8c64f698: *stop=0x8c64f698u;if(!valid_access(r[6]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c64f69a; /* mov.w @(0x2,r6),r0 */
L8c64f69a: *stop=0x8c64f69au;r[0]-=r[2];goto L8c64f69c; /* sub r2,r0 */
L8c64f69c: *stop=0x8c64f69cu;*fpul=r[0];goto L8c64f69e; /* lds r0,FPUL */
L8c64f69e: *stop=0x8c64f69eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c64f6a0; /* float FPUL,fr1 */
L8c64f6a0: *stop=0x8c64f6a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f6a2; /* fmac fr0,fr1,fr3 */
L8c64f6a2: *stop=0x8c64f6a2u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c64f6a4; /* ftrc fr3,FPUL */
L8c64f6a4: *stop=0x8c64f6a4u;r[0]=*fpul;goto L8c64f6a6; /* sts FPUL,r0 */
L8c64f6a6: *stop=0x8c64f6a6u;if(!valid_access(r[1]+2u,2u))return -3;wr(m,bus,&io_status,r[1]+0x2u,r[0],2);if(io_status)return io_status;goto L8c64f6a8; /* mov.w r0,@(0x2,r1) */
L8c64f6a8: *stop=0x8c64f6a8u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64f6aa; /* mov.l @(0x4,r4),r6 */
L8c64f6aa: *stop=0x8c64f6aau;fcopy(f,0u,4u);goto L8c64f6ac; /* fmov fr4,fr0 */
L8c64f6ac: *stop=0x8c64f6acu;r[2]=r[6];goto L8c64f6ae; /* mov r6,r2 */
L8c64f6ae: *stop=0x8c64f6aeu;r[2]+=4u;goto L8c64f6b0; /* add #0x4,r2 */
L8c64f6b0: *stop=0x8c64f6b0u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f6b2; /* mov.l @(0x4,r5),r3 */
L8c64f6b2: *stop=0x8c64f6b2u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c64f6b4; /* mov.l r2,@(0x10,r15) */
L8c64f6b4: *stop=0x8c64f6b4u;if(!valid_access(r[2]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x3u,1);if(io_status)return io_status;goto L8c64f6b6; /* mov.b @(0x3,r2),r0 */
L8c64f6b6: *stop=0x8c64f6b6u;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[0],1);if(io_status)return io_status;goto L8c64f6b8; /* mov.b r0,@r15 */
L8c64f6b8: *stop=0x8c64f6b8u;r[0]=r[0]&255u;goto L8c64f6ba; /* extu.b r0,r0 */
L8c64f6ba: *stop=0x8c64f6bau;*fpul=r[0];goto L8c64f6bc; /* lds r0,FPUL */
L8c64f6bc: *stop=0x8c64f6bcu;if(!valid_access(r[6]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x3u,1);if(io_status)return io_status;goto L8c64f6be; /* mov.b @(0x3,r6),r0 */
L8c64f6be: *stop=0x8c64f6beu;if(!valid_access(r[15],1u))return -3;r[2]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c64f6c0; /* mov.b @r15,r2 */
L8c64f6c0: *stop=0x8c64f6c0u;r[0]=r[0]&255u;goto L8c64f6c2; /* extu.b r0,r0 */
L8c64f6c2: *stop=0x8c64f6c2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64f6c4; /* float FPUL,fr3 */
L8c64f6c4: *stop=0x8c64f6c4u;r[2]=r[2]&255u;goto L8c64f6c6; /* extu.b r2,r2 */
L8c64f6c6: *stop=0x8c64f6c6u;r[0]-=r[2];goto L8c64f6c8; /* sub r2,r0 */
L8c64f6c8: *stop=0x8c64f6c8u;*fpul=r[0];goto L8c64f6ca; /* lds r0,FPUL */
L8c64f6ca: *stop=0x8c64f6cau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c64f6cc; /* float FPUL,fr2 */
L8c64f6cc: *stop=0x8c64f6ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f6ce; /* fmac fr0,fr2,fr3 */
L8c64f6ce: *stop=0x8c64f6ceu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c64f6d0; /* ftrc fr3,FPUL */
L8c64f6d0: *stop=0x8c64f6d0u;r[0]=*fpul;goto L8c64f6d2; /* sts FPUL,r0 */
L8c64f6d2: *stop=0x8c64f6d2u;if(!valid_access(r[3]+3u,1u))return -3;wr(m,bus,&io_status,r[3]+0x3u,r[0],1);if(io_status)return io_status;goto L8c64f6d4; /* mov.b r0,@(0x3,r3) */
L8c64f6d4: *stop=0x8c64f6d4u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64f6d6; /* mov.l @(0x4,r4),r6 */
L8c64f6d6: *stop=0x8c64f6d6u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f6d8; /* mov.l @(0x4,r5),r3 */
L8c64f6d8: *stop=0x8c64f6d8u;r[2]=r[6];goto L8c64f6da; /* mov r6,r2 */
L8c64f6da: *stop=0x8c64f6dau;r[2]+=4u;goto L8c64f6dc; /* add #0x4,r2 */
L8c64f6dc: *stop=0x8c64f6dcu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c64f6de; /* mov.l r2,@(0xc,r15) */
L8c64f6de: *stop=0x8c64f6deu;if(!valid_access(r[2]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,1);if(io_status)return io_status;goto L8c64f6e0; /* mov.b @(0x2,r2),r0 */
L8c64f6e0: *stop=0x8c64f6e0u;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[0],1);if(io_status)return io_status;goto L8c64f6e2; /* mov.b r0,@r15 */
L8c64f6e2: *stop=0x8c64f6e2u;r[0]=r[0]&255u;goto L8c64f6e4; /* extu.b r0,r0 */
L8c64f6e4: *stop=0x8c64f6e4u;*fpul=r[0];goto L8c64f6e6; /* lds r0,FPUL */
L8c64f6e6: *stop=0x8c64f6e6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64f6e8; /* float FPUL,fr3 */
L8c64f6e8: *stop=0x8c64f6e8u;if(!valid_access(r[15],1u))return -3;r[2]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c64f6ea; /* mov.b @r15,r2 */
L8c64f6ea: *stop=0x8c64f6eau;if(!valid_access(r[6]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x2u,1);if(io_status)return io_status;goto L8c64f6ec; /* mov.b @(0x2,r6),r0 */
L8c64f6ec: *stop=0x8c64f6ecu;r[2]=r[2]&255u;goto L8c64f6ee; /* extu.b r2,r2 */
L8c64f6ee: *stop=0x8c64f6eeu;r[0]=r[0]&255u;goto L8c64f6f0; /* extu.b r0,r0 */
L8c64f6f0: *stop=0x8c64f6f0u;r[0]-=r[2];goto L8c64f6f2; /* sub r2,r0 */
L8c64f6f2: *stop=0x8c64f6f2u;*fpul=r[0];goto L8c64f6f4; /* lds r0,FPUL */
L8c64f6f4: *stop=0x8c64f6f4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c64f6f6; /* float FPUL,fr2 */
L8c64f6f6: *stop=0x8c64f6f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f6f8; /* fmac fr0,fr2,fr3 */
L8c64f6f8: *stop=0x8c64f6f8u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c64f6fa; /* ftrc fr3,FPUL */
L8c64f6fa: *stop=0x8c64f6fau;r[0]=*fpul;goto L8c64f6fc; /* sts FPUL,r0 */
L8c64f6fc: *stop=0x8c64f6fcu;if(!valid_access(r[3]+2u,1u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],1);if(io_status)return io_status;goto L8c64f6fe; /* mov.b r0,@(0x2,r3) */
L8c64f6fe: *stop=0x8c64f6feu;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64f700; /* mov.l @(0x4,r4),r6 */
L8c64f700: *stop=0x8c64f700u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f702; /* mov.l @(0x4,r5),r3 */
L8c64f702: *stop=0x8c64f702u;r[2]=r[6];goto L8c64f704; /* mov r6,r2 */
L8c64f704: *stop=0x8c64f704u;r[2]+=4u;goto L8c64f706; /* add #0x4,r2 */
L8c64f706: *stop=0x8c64f706u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c64f708; /* mov.l r2,@(0x8,r15) */
L8c64f708: *stop=0x8c64f708u;if(!valid_access(r[2]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x1u,1);if(io_status)return io_status;goto L8c64f70a; /* mov.b @(0x1,r2),r0 */
L8c64f70a: *stop=0x8c64f70au;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[0],1);if(io_status)return io_status;goto L8c64f70c; /* mov.b r0,@r15 */
L8c64f70c: *stop=0x8c64f70cu;r[0]=r[0]&255u;goto L8c64f70e; /* extu.b r0,r0 */
L8c64f70e: *stop=0x8c64f70eu;*fpul=r[0];goto L8c64f710; /* lds r0,FPUL */
L8c64f710: *stop=0x8c64f710u;if(!valid_access(r[15],1u))return -3;r[2]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c64f712; /* mov.b @r15,r2 */
L8c64f712: *stop=0x8c64f712u;if(!valid_access(r[6]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x1u,1);if(io_status)return io_status;goto L8c64f714; /* mov.b @(0x1,r6),r0 */
L8c64f714: *stop=0x8c64f714u;r[2]=r[2]&255u;goto L8c64f716; /* extu.b r2,r2 */
L8c64f716: *stop=0x8c64f716u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64f718; /* float FPUL,fr3 */
L8c64f718: *stop=0x8c64f718u;r[0]=r[0]&255u;goto L8c64f71a; /* extu.b r0,r0 */
L8c64f71a: *stop=0x8c64f71au;r[0]-=r[2];goto L8c64f71c; /* sub r2,r0 */
L8c64f71c: *stop=0x8c64f71cu;*fpul=r[0];goto L8c64f71e; /* lds r0,FPUL */
L8c64f71e: *stop=0x8c64f71eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c64f720; /* float FPUL,fr2 */
L8c64f720: *stop=0x8c64f720u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f722; /* fmac fr0,fr2,fr3 */
L8c64f722: *stop=0x8c64f722u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c64f724; /* ftrc fr3,FPUL */
L8c64f724: *stop=0x8c64f724u;r[0]=*fpul;goto L8c64f726; /* sts FPUL,r0 */
L8c64f726: *stop=0x8c64f726u;if(!valid_access(r[3]+1u,1u))return -3;wr(m,bus,&io_status,r[3]+0x1u,r[0],1);if(io_status)return io_status;goto L8c64f728; /* mov.b r0,@(0x1,r3) */
L8c64f728: *stop=0x8c64f728u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64f72a; /* mov.l @(0x4,r4),r6 */
L8c64f72a: *stop=0x8c64f72au;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f72c; /* mov.l @(0x4,r5),r3 */
L8c64f72c: *stop=0x8c64f72cu;r[2]=r[6];goto L8c64f72e; /* mov r6,r2 */
L8c64f72e: *stop=0x8c64f72eu;r[2]+=4u;goto L8c64f730; /* add #0x4,r2 */
L8c64f730: *stop=0x8c64f730u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c64f732; /* mov.l r2,@(0x4,r15) */
L8c64f732: *stop=0x8c64f732u;if(!valid_access(r[2],1u))return -3;r[1]=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;goto L8c64f734; /* mov.b @r2,r1 */
L8c64f734: *stop=0x8c64f734u;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[1],1);if(io_status)return io_status;goto L8c64f736; /* mov.b r1,@r15 */
L8c64f736: *stop=0x8c64f736u;r[1]=r[1]&255u;goto L8c64f738; /* extu.b r1,r1 */
L8c64f738: *stop=0x8c64f738u;*fpul=r[1];goto L8c64f73a; /* lds r1,FPUL */
L8c64f73a: *stop=0x8c64f73au;if(!valid_access(r[6],1u))return -3;r[2]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;goto L8c64f73c; /* mov.b @r6,r2 */
L8c64f73c: *stop=0x8c64f73cu;if(!valid_access(r[15],1u))return -3;r[1]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c64f73e; /* mov.b @r15,r1 */
L8c64f73e: *stop=0x8c64f73eu;r[2]=r[2]&255u;goto L8c64f740; /* extu.b r2,r2 */
L8c64f740: *stop=0x8c64f740u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64f742; /* float FPUL,fr3 */
L8c64f742: *stop=0x8c64f742u;r[1]=r[1]&255u;goto L8c64f744; /* extu.b r1,r1 */
L8c64f744: *stop=0x8c64f744u;r[2]-=r[1];goto L8c64f746; /* sub r1,r2 */
L8c64f746: *stop=0x8c64f746u;*fpul=r[2];goto L8c64f748; /* lds r2,FPUL */
L8c64f748: *stop=0x8c64f748u;r[1]=1u;goto L8c64f74a; /* mov #0x1,r1 */
L8c64f74a: *stop=0x8c64f74au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c64f74c; /* float FPUL,fr2 */
L8c64f74c: *stop=0x8c64f74cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f74e; /* fmac fr0,fr2,fr3 */
L8c64f74e: *stop=0x8c64f74eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c64f750; /* ftrc fr3,FPUL */
L8c64f750: *stop=0x8c64f750u;r[2]=*fpul;goto L8c64f752; /* sts FPUL,r2 */
L8c64f752: *stop=0x8c64f752u;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[2],1);if(io_status)return io_status;goto L8c64f754; /* mov.b r2,@r3 */
L8c64f754: *stop=0x8c64f754u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f756; /* mov.l @r4,r3 */
L8c64f756: *stop=0x8c64f756u;r[0]=8u;goto L8c64f758; /* mov #0x8,r0 */
L8c64f758: *stop=0x8c64f758u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c64f75a; /* fmov.s @r7,fr3 */
L8c64f75a: *stop=0x8c64f75au;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f75c; /* fmov.s @(r0,r3),fr2 */
L8c64f75c: *stop=0x8c64f75cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c64f75e; /* fcmp/gt fr2,fr3 */
L8c64f75e: if((*sr&1u))goto L8c64f7b4;goto L8c64f760; /* bt 0x8c64f7b4 */
L8c64f760: *stop=0x8c64f760u;r[6]=r[1];goto L8c64f762; /* mov r1,r6 */
L8c64f762: *stop=0x8c64f762u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c64f764; /* shll r6 */
L8c64f764: *stop=0x8c64f764u;r[3]=r[1];goto L8c64f766; /* mov r1,r3 */
L8c64f766: *stop=0x8c64f766u;r[6]+=r[3];goto L8c64f768; /* add r3,r6 */
L8c64f768: *stop=0x8c64f768u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f76a; /* mov.l @r4,r3 */
L8c64f76a: *stop=0x8c64f76au;r[6]<<=2;goto L8c64f76c; /* shll2 r6 */
L8c64f76c: *stop=0x8c64f76cu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f76e; /* mov.l @r5,r2 */
L8c64f76e: *stop=0x8c64f76eu;r[0]=4u;goto L8c64f770; /* mov #0x4,r0 */
L8c64f770: *stop=0x8c64f770u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c64f772; /* fmov.s @r3,fr3 */
L8c64f772: *stop=0x8c64f772u;r[13]=r[1];goto L8c64f774; /* mov r1,r13 */
L8c64f774: *stop=0x8c64f774u;r[2]+=r[6];goto L8c64f776; /* add r6,r2 */
L8c64f776: *stop=0x8c64f776u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c64f778; /* fmov.s fr3,@r2 */
L8c64f778: *stop=0x8c64f778u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f77a; /* mov.l @r4,r2 */
L8c64f77a: *stop=0x8c64f77au;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f77c; /* mov.l @r5,r3 */
L8c64f77c: *stop=0x8c64f77cu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64f77e; /* fmov.s @(r0,r2),fr3 */
L8c64f77e: *stop=0x8c64f77eu;r[3]+=r[6];goto L8c64f780; /* add r6,r3 */
L8c64f780: *stop=0x8c64f780u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f782; /* fmov fr3,@(r0,r3) */
L8c64f782: *stop=0x8c64f782u;r[0]=8u;goto L8c64f784; /* mov #0x8,r0 */
L8c64f784: *stop=0x8c64f784u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f786; /* mov.l @r5,r3 */
L8c64f786: *stop=0x8c64f786u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f788; /* mov.l @r4,r2 */
L8c64f788: *stop=0x8c64f788u;r[6]+=r[3];goto L8c64f78a; /* add r3,r6 */
L8c64f78a: *stop=0x8c64f78au;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64f78c; /* fmov.s @(r0,r2),fr3 */
L8c64f78c: *stop=0x8c64f78cu;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c64f78e; /* fmov fr3,@(r0,r6) */
L8c64f78e: *stop=0x8c64f78eu;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f790; /* mov.l @(0x8,r5),r3 */
L8c64f790: *stop=0x8c64f790u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64f792; /* tst r3,r3 */
L8c64f792: take=(*sr&1u);*stop=0x8c64f794u;r[13]<<=2;if(take)goto L8c64f7aa;goto L8c64f796; /* bt/s 0x8c64f7aa */
L8c64f796: *stop=0x8c64f796u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f798; /* mov.l @(0x8,r4),r2 */
L8c64f798: *stop=0x8c64f798u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f79a; /* mov.l @(0x8,r5),r3 */
L8c64f79a: *stop=0x8c64f79au;if(!valid_access(r[2],2u))return -3;r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c64f79c; /* mov.w @r2,r1 */
L8c64f79c: *stop=0x8c64f79cu;r[3]+=r[13];goto L8c64f79e; /* add r13,r3 */
L8c64f79e: *stop=0x8c64f79eu;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c64f7a0; /* mov.w r1,@r3 */
L8c64f7a0: *stop=0x8c64f7a0u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64f7a2; /* mov.l @(0x8,r4),r0 */
L8c64f7a2: *stop=0x8c64f7a2u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f7a4; /* mov.l @(0x8,r5),r3 */
L8c64f7a4: *stop=0x8c64f7a4u;if(!valid_access(r[0]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c64f7a6; /* mov.w @(0x2,r0),r0 */
L8c64f7a6: *stop=0x8c64f7a6u;r[3]+=r[13];goto L8c64f7a8; /* add r13,r3 */
L8c64f7a8: *stop=0x8c64f7a8u;if(!valid_access(r[3]+2u,2u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c64f7aa; /* mov.w r0,@(0x2,r3) */
L8c64f7aa: *stop=0x8c64f7aau;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f7ac; /* mov.l @(0x4,r4),r2 */
L8c64f7ac: *stop=0x8c64f7acu;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f7ae; /* mov.l @(0x4,r5),r3 */
L8c64f7ae: *stop=0x8c64f7aeu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f7b0; /* mov.l @r2,r1 */
L8c64f7b0: *stop=0x8c64f7b0u;r[3]+=r[13];goto L8c64f7b2; /* add r13,r3 */
L8c64f7b2: *stop=0x8c64f7b2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64f7b4; /* mov.l r1,@r3 */
L8c64f7b4: *stop=0x8c64f7b4u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f7b6; /* mov.l @r4,r3 */
L8c64f7b6: *stop=0x8c64f7b6u;r[0]=8u;goto L8c64f7b8; /* mov #0x8,r0 */
L8c64f7b8: *stop=0x8c64f7b8u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c64f7ba; /* fmov.s @r7,fr3 */
L8c64f7ba: *stop=0x8c64f7bau;r[3]+=12u;goto L8c64f7bc; /* add #0xc,r3 */
L8c64f7bc: *stop=0x8c64f7bcu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f7be; /* fmov.s @(r0,r3),fr2 */
L8c64f7be: *stop=0x8c64f7beu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c64f7c0; /* fcmp/gt fr2,fr3 */
L8c64f7c0: if((*sr&1u))goto L8c64f814;goto L8c64f7c2; /* bt 0x8c64f814 */
L8c64f7c2: *stop=0x8c64f7c2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f7c4; /* mov.l @r4,r3 */
L8c64f7c4: *stop=0x8c64f7c4u;r[0]=4u;goto L8c64f7c6; /* mov #0x4,r0 */
L8c64f7c6: *stop=0x8c64f7c6u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f7c8; /* mov.l @r5,r2 */
L8c64f7c8: *stop=0x8c64f7c8u;r[3]+=12u;goto L8c64f7ca; /* add #0xc,r3 */
L8c64f7ca: *stop=0x8c64f7cau;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c64f7cc; /* fmov.s @r3,fr3 */
L8c64f7cc: *stop=0x8c64f7ccu;r[2]+=12u;goto L8c64f7ce; /* add #0xc,r2 */
L8c64f7ce: *stop=0x8c64f7ceu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c64f7d0; /* fmov.s fr3,@r2 */
L8c64f7d0: *stop=0x8c64f7d0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f7d2; /* mov.l @r4,r2 */
L8c64f7d2: *stop=0x8c64f7d2u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f7d4; /* mov.l @r5,r3 */
L8c64f7d4: *stop=0x8c64f7d4u;r[2]+=12u;goto L8c64f7d6; /* add #0xc,r2 */
L8c64f7d6: *stop=0x8c64f7d6u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64f7d8; /* fmov.s @(r0,r2),fr3 */
L8c64f7d8: *stop=0x8c64f7d8u;r[3]+=12u;goto L8c64f7da; /* add #0xc,r3 */
L8c64f7da: *stop=0x8c64f7dau;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f7dc; /* fmov fr3,@(r0,r3) */
L8c64f7dc: *stop=0x8c64f7dcu;r[0]=8u;goto L8c64f7de; /* mov #0x8,r0 */
L8c64f7de: *stop=0x8c64f7deu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f7e0; /* mov.l @r4,r2 */
L8c64f7e0: *stop=0x8c64f7e0u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f7e2; /* mov.l @r5,r3 */
L8c64f7e2: *stop=0x8c64f7e2u;r[2]+=12u;goto L8c64f7e4; /* add #0xc,r2 */
L8c64f7e4: *stop=0x8c64f7e4u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64f7e6; /* fmov.s @(r0,r2),fr3 */
L8c64f7e6: *stop=0x8c64f7e6u;r[3]+=12u;goto L8c64f7e8; /* add #0xc,r3 */
L8c64f7e8: *stop=0x8c64f7e8u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f7ea; /* fmov fr3,@(r0,r3) */
L8c64f7ea: *stop=0x8c64f7eau;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f7ec; /* mov.l @(0x8,r5),r3 */
L8c64f7ec: *stop=0x8c64f7ecu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64f7ee; /* tst r3,r3 */
L8c64f7ee: if((*sr&1u))goto L8c64f808;goto L8c64f7f0; /* bt 0x8c64f808 */
L8c64f7f0: *stop=0x8c64f7f0u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f7f2; /* mov.l @(0x8,r4),r2 */
L8c64f7f2: *stop=0x8c64f7f2u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f7f4; /* mov.l @(0x8,r5),r3 */
L8c64f7f4: *stop=0x8c64f7f4u;r[2]+=4u;goto L8c64f7f6; /* add #0x4,r2 */
L8c64f7f6: *stop=0x8c64f7f6u;if(!valid_access(r[2],2u))return -3;r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c64f7f8; /* mov.w @r2,r1 */
L8c64f7f8: *stop=0x8c64f7f8u;r[3]+=4u;goto L8c64f7fa; /* add #0x4,r3 */
L8c64f7fa: *stop=0x8c64f7fau;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c64f7fc; /* mov.w r1,@r3 */
L8c64f7fc: *stop=0x8c64f7fcu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64f7fe; /* mov.l @(0x8,r4),r0 */
L8c64f7fe: *stop=0x8c64f7feu;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f800; /* mov.l @(0x8,r5),r3 */
L8c64f800: *stop=0x8c64f800u;r[0]+=4u;goto L8c64f802; /* add #0x4,r0 */
L8c64f802: *stop=0x8c64f802u;if(!valid_access(r[0]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c64f804; /* mov.w @(0x2,r0),r0 */
L8c64f804: *stop=0x8c64f804u;r[3]+=4u;goto L8c64f806; /* add #0x4,r3 */
L8c64f806: *stop=0x8c64f806u;if(!valid_access(r[3]+2u,2u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c64f808; /* mov.w r0,@(0x2,r3) */
L8c64f808: *stop=0x8c64f808u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f80a; /* mov.l @(0x4,r4),r2 */
L8c64f80a: *stop=0x8c64f80au;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f80c; /* mov.l @(0x4,r5),r3 */
L8c64f80c: *stop=0x8c64f80cu;r[2]+=4u;goto L8c64f80e; /* add #0x4,r2 */
L8c64f80e: *stop=0x8c64f80eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f810; /* mov.l @r2,r1 */
L8c64f810: *stop=0x8c64f810u;r[3]+=4u;goto L8c64f812; /* add #0x4,r3 */
L8c64f812: *stop=0x8c64f812u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64f814; /* mov.l r1,@r3 */
L8c64f814: *stop=0x8c64f814u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64f816; /* mov.l @r4,r5 */
L8c64f816: *stop=0x8c64f816u;r[0]=8u;goto L8c64f818; /* mov #0x8,r0 */
L8c64f818: *stop=0x8c64f818u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[7]);if(io_status)return io_status;goto L8c64f81a; /* fmov.s @r7,fr4 */
L8c64f81a: *stop=0x8c64f81au;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c64f81c; /* fmov.s @(r0,r5),fr3 */
L8c64f81c: *stop=0x8c64f81cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c64f81e; /* fcmp/gt fr3,fr4 */
L8c64f81e: if(!(*sr&1u))goto L8c64f830;goto L8c64f820; /* bf 0x8c64f830 */
L8c64f820: *stop=0x8c64f820u;r[0]=20u;goto L8c64f822; /* mov #0x14,r0 */
L8c64f822: *stop=0x8c64f822u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c64f824; /* fmov.s @(r0,r5),fr3 */
L8c64f824: *stop=0x8c64f824u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c64f826; /* fcmp/gt fr3,fr4 */
L8c64f826: if(!(*sr&1u))goto L8c64f830;goto L8c64f828; /* bf 0x8c64f830 */
L8c64f828: *stop=0x8c64f828u;r[0]=0u;goto L8c64f82a; /* mov #0x0,r0 */
L8c64f82a: *stop=0x8c64f82au;r[15]+=32u;goto L8c64f82c; /* add #0x20,r15 */
L8c64f82c: *stop=0x8c64f82cu;if(*pr!=entry_pr)return -3;*stop=0x8c64f82eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;return 0; /* rts */
L8c64f830: *stop=0x8c64f830u;r[0]=2u;goto L8c64f832; /* mov #0x2,r0 */
L8c64f832: *stop=0x8c64f832u;r[15]+=32u;goto L8c64f834; /* add #0x20,r15 */
L8c64f834: *stop=0x8c64f834u;if(*pr!=entry_pr)return -3;*stop=0x8c64f836u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;return 0; /* rts */
}
