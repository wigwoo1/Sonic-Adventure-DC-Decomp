/* Original complete player work initializer and query allocation path.
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
static int sa_player_work_initialize_run_8c06c350(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c06c370u)goto L8c06c370;if(resume_pc==0x8c06c394u)goto L8c06c394;if(resume_pc==0x8c06c39cu)goto L8c06c39c;if(resume_pc==0x8c06c3a6u)goto L8c06c3a6;if(resume_pc==0x8c06c420u)goto L8c06c420;if(resume_pc==0x8c06c432u)goto L8c06c432;if(resume_pc==0x8c06c444u)goto L8c06c444;if(resume_pc==0x8c06c456u)goto L8c06c456;if(resume_pc==0x8c06c468u)goto L8c06c468;if(resume_pc==0x8c06c4cau)goto L8c06c4ca;if(resume_pc==0x8c06c4deu)goto L8c06c4de;if(resume_pc==0x8c06c4f4u)goto L8c06c4f4;if(resume_pc==0x8c06c50cu)goto L8c06c50c;if(resume_pc==0x8c06c522u)goto L8c06c522;if(resume_pc==0x8c06c53au)goto L8c06c53a;if(resume_pc==0x8c06c550u)goto L8c06c550;if(resume_pc==0x8c06c568u)goto L8c06c568;if(resume_pc==0x8c06c57eu)goto L8c06c57e;if(resume_pc==0x8c06c596u)goto L8c06c596;if(resume_pc==0x8c06c5acu)goto L8c06c5ac;goto L8c06c350;
L8c06c350: *stop=0x8c06c350u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c06c352; /* mov.l r14,@-r15 */
L8c06c352: *stop=0x8c06c352u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c06c354; /* mov.l r13,@-r15 */
L8c06c354: *stop=0x8c06c354u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c06c356; /* mov.l r12,@-r15 */
L8c06c356: *stop=0x8c06c356u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c06c358; /* mov.l r11,@-r15 */
L8c06c358: *stop=0x8c06c358u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c06c35a; /* fmov.s fr15,@-r15 */
L8c06c35a: *stop=0x8c06c35au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c06c35c; /* sts.l PR,@-r15 */
L8c06c35c: *stop=0x8c06c35cu;r[15]+=4294967292u;goto L8c06c35e; /* add #-0x4,r15 */
L8c06c35e: *stop=0x8c06c35eu;r[13]=r[5];goto L8c06c360; /* mov r5,r13 */
L8c06c360: *stop=0x8c06c360u;r[5]=2u;goto L8c06c362; /* mov #0x2,r5 */
L8c06c362: *stop=0x8c06c362u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c49cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c06c364; /* mov.l 0x8c06c49c,r3 */
L8c06c364: *stop=0x8c06c364u;r[5]<<=8;goto L8c06c366; /* shll8 r5 */
L8c06c366: *stop=0x8c06c366u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c06c368; /* mov.l r4,@r15 */
L8c06c368: *stop=0x8c06c368u;if(!valid_access(r[13]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x24u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c06c36a; /* mov.l @(0x24,r13),r11 */
L8c06c36a: *stop=0x8c06c36au;if(!valid_access(r[13]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x20u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c06c36c; /* mov.l @(0x20,r13),r12 */
L8c06c36c: target=r[3];*pr=0x8c06c370u;*stop=0x8c06c36eu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c370; /* jsr @r3 */
L8c06c370: *stop=0x8c06c370u;r[14]=r[0];goto L8c06c372; /* mov r0,r14 */
L8c06c372: *stop=0x8c06c372u;if(!valid_access(r[12]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x9u,1);if(io_status)return io_status;goto L8c06c374; /* mov.b @(0x9,r12),r0 */
L8c06c374: *stop=0x8c06c374u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c4a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c06c376; /* mov.l 0x8c06c4a0,r2 */
L8c06c376: *stop=0x8c06c376u;r[1]=4294967196u;goto L8c06c378; /* mov #-0x64,r1 */
L8c06c378: *stop=0x8c06c378u;r[0]=r[0]&255u;goto L8c06c37a; /* extu.b r0,r0 */
L8c06c37a: *stop=0x8c06c37au;r[3]=r[0];goto L8c06c37c; /* mov r0,r3 */
L8c06c37c: *stop=0x8c06c37cu;r[0]<<=2;goto L8c06c37e; /* shll2 r0 */
L8c06c37e: *stop=0x8c06c37eu;r[0]<<=2;goto L8c06c380; /* shll2 r0 */
L8c06c380: *stop=0x8c06c380u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c06c382; /* shll r0 */
L8c06c382: *stop=0x8c06c382u;r[0]+=r[3];goto L8c06c384; /* add r3,r0 */
L8c06c384: *stop=0x8c06c384u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c4a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c06c386; /* mov.l 0x8c06c4a4,r3 */
L8c06c386: *stop=0x8c06c386u;r[0]<<=2;goto L8c06c388; /* shll2 r0 */
L8c06c388: *stop=0x8c06c388u;r[2]+=r[0];goto L8c06c38a; /* add r0,r2 */
L8c06c38a: *stop=0x8c06c38au;r[0]=4294967172u;goto L8c06c38c; /* mov #-0x7c,r0 */
L8c06c38c: *stop=0x8c06c38cu;r[1]=r[1]&255u;goto L8c06c38e; /* extu.b r1,r1 */
L8c06c38e: *stop=0x8c06c38eu;r[0]=r[0]&255u;goto L8c06c390; /* extu.b r0,r0 */
L8c06c390: target=r[3];*pr=0x8c06c394u;*stop=0x8c06c392u;r[1]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c394; /* jsr @r3 */
L8c06c394: *stop=0x8c06c394u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c4a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c06c396; /* mov.l 0x8c06c4a8,r2 */
L8c06c396: *stop=0x8c06c396u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[14],4);if(io_status)return io_status;goto L8c06c398; /* mov.l r14,@r11 */
L8c06c398: target=r[2];*pr=0x8c06c39cu;*stop=0x8c06c39au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c39c; /* jsr @r2 */
L8c06c39c: *stop=0x8c06c39cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c49cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c06c39e; /* mov.l 0x8c06c49c,r3 */
L8c06c39e: *stop=0x8c06c39eu;r[5]=4294967172u;goto L8c06c3a0; /* mov #-0x7c,r5 */
L8c06c3a0: *stop=0x8c06c3a0u;r[5]=r[5]&255u;goto L8c06c3a2; /* extu.b r5,r5 */
L8c06c3a2: target=r[3];*pr=0x8c06c3a6u;*stop=0x8c06c3a4u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c3a6; /* jsr @r3 */
L8c06c3a6: *stop=0x8c06c3a6u;r[1]=100u;goto L8c06c3a8; /* mov #0x64,r1 */
L8c06c3a8: *stop=0x8c06c3a8u;r[3]=r[14];goto L8c06c3aa; /* mov r14,r3 */
L8c06c3aa: *stop=0x8c06c3aau;r[1]+=r[14];goto L8c06c3ac; /* add r14,r1 */
L8c06c3ac: *stop=0x8c06c3acu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c06c3ae; /* mov.l r0,@r1 */
L8c06c3ae: *stop=0x8c06c3aeu;r[3]+=56u;goto L8c06c3b0; /* add #0x38,r3 */
L8c06c3b0: *stop=0x8c06c3b0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c06c3b2; /* mov.l @r15,r4 */
L8c06c3b2: *stop=0x8c06c3b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c48cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c06c3b4; /* mov.l 0x8c06c48c,r0 */
L8c06c3b4: *stop=0x8c06c3b4u;r[4]<<=2;goto L8c06c3b6; /* shll2 r4 */
L8c06c3b6: *stop=0x8c06c3b6u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[13],4);if(io_status)return io_status;goto L8c06c3b8; /* mov.l r13,@(r0,r4) */
L8c06c3b8: *stop=0x8c06c3b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c488u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c06c3ba; /* mov.l 0x8c06c488,r0 */
L8c06c3ba: *stop=0x8c06c3bau;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[12],4);if(io_status)return io_status;goto L8c06c3bc; /* mov.l r12,@(r0,r4) */
L8c06c3bc: *stop=0x8c06c3bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c484u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c06c3be; /* mov.l 0x8c06c484,r0 */
L8c06c3be: *stop=0x8c06c3beu;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[11],4);if(io_status)return io_status;goto L8c06c3c0; /* mov.l r11,@(r0,r4) */
L8c06c3c0: *stop=0x8c06c3c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c480u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c06c3c2; /* mov.l 0x8c06c480,r0 */
L8c06c3c2: *stop=0x8c06c3c2u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[14],4);if(io_status)return io_status;goto L8c06c3c4; /* mov.l r14,@(r0,r4) */
L8c06c3c4: *stop=0x8c06c3c4u;r[0]=72u;goto L8c06c3c6; /* mov #0x48,r0 */
L8c06c3c6: *stop=0x8c06c3c6u;r[0]<<=2;goto L8c06c3c8; /* shll2 r0 */
L8c06c3c8: *stop=0x8c06c3c8u;r[4]=0u;goto L8c06c3ca; /* mov #0x0,r4 */
L8c06c3ca: *stop=0x8c06c3cau;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[4],2);if(io_status)return io_status;goto L8c06c3cc; /* mov.w r4,@(r0,r14) */
L8c06c3cc: *stop=0x8c06c3ccu;r[0]+=2u;goto L8c06c3ce; /* add #0x2,r0 */
L8c06c3ce: *stop=0x8c06c3ceu;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[4],2);if(io_status)return io_status;goto L8c06c3d0; /* mov.w r4,@(r0,r14) */
L8c06c3d0: *stop=0x8c06c3d0u;r[0]+=22u;goto L8c06c3d2; /* add #0x16,r0 */
L8c06c3d2: *stop=0x8c06c3d2u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c06c3d4; /* mov.l r3,@(r0,r14) */
L8c06c3d4: *stop=0x8c06c3d4u;r[0]+=4u;goto L8c06c3d6; /* add #0x4,r0 */
L8c06c3d6: *stop=0x8c06c3d6u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[14],4);if(io_status)return io_status;goto L8c06c3d8; /* mov.l r14,@(r0,r14) */
L8c06c3d8: *stop=0x8c06c3d8u;r[0]=0x8c06c4acu;goto L8c06c3da; /* mova 0x8c06c4ac,r0 */
L8c06c3da: *stop=0x8c06c3dau;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c06c3dc; /* fmov.s @r0,fr3 */
L8c06c3dc: *stop=0x8c06c3dcu;r[0]=67u;goto L8c06c3de; /* mov #0x43,r0 */
L8c06c3de: *stop=0x8c06c3deu;r[0]<<=2;goto L8c06c3e0; /* shll2 r0 */
L8c06c3e0: *stop=0x8c06c3e0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c06c3e2; /* fmov.s @(r0,r14),fr2 */
L8c06c3e2: *stop=0x8c06c3e2u;r[0]=127u;goto L8c06c3e4; /* mov #0x7f,r0 */
L8c06c3e4: *stop=0x8c06c3e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c06c3e6; /* fmul fr3,fr2 */
L8c06c3e6: *stop=0x8c06c3e6u;r[0]<<=2;goto L8c06c3e8; /* shll2 r0 */
L8c06c3e8: *stop=0x8c06c3e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c4b0u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c06c3ea; /* mov.l 0x8c06c4b0,r13 */
L8c06c3ea: *stop=0x8c06c3eau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c06c3ec; /* fmov fr2,@(r0,r14) */
L8c06c3ec: *stop=0x8c06c3ecu;if(!valid_access(r[12]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x9u,1);if(io_status)return io_status;goto L8c06c3ee; /* mov.b @(0x9,r12),r0 */
L8c06c3ee: *stop=0x8c06c3eeu;r[0]=r[0]&255u;goto L8c06c3f0; /* extu.b r0,r0 */
L8c06c3f0: *stop=0x8c06c3f0u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c06c3f2; /* cmp/eq #0x0,r0 */
L8c06c3f2: if((*sr&1u))goto L8c06c418;goto L8c06c3f4; /* bt 0x8c06c418 */
L8c06c3f4: *stop=0x8c06c3f4u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c06c3f6; /* cmp/eq #0x2,r0 */
L8c06c3f6: if((*sr&1u))goto L8c06c44e;goto L8c06c3f8; /* bt 0x8c06c44e */
L8c06c3f8: *stop=0x8c06c3f8u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c06c3fa; /* cmp/eq #0x3,r0 */
L8c06c3fa: if((*sr&1u))goto L8c06c4c0;goto L8c06c3fc; /* bt 0x8c06c4c0 */
L8c06c3fc: *stop=0x8c06c3fcu;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c3fe; /* cmp/eq #0x5,r0 */
L8c06c3fe: if(!(*sr&1u))goto L8c06c404;goto L8c06c400; /* bf 0x8c06c404 */
L8c06c400: *stop=0x8c06c402u;goto L8c06c4ea; /* bra 0x8c06c4ea */
L8c06c404: *stop=0x8c06c404u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c406; /* cmp/eq #0x6,r0 */
L8c06c406: if(!(*sr&1u))goto L8c06c40c;goto L8c06c408; /* bf 0x8c06c40c */
L8c06c408: *stop=0x8c06c40au;goto L8c06c518; /* bra 0x8c06c518 */
L8c06c40c: *stop=0x8c06c40cu;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06c40e; /* cmp/eq #0x7,r0 */
L8c06c40e: if(!(*sr&1u))goto L8c06c414;goto L8c06c410; /* bf 0x8c06c414 */
L8c06c410: *stop=0x8c06c412u;goto L8c06c548; /* bra 0x8c06c548 */
L8c06c414: *stop=0x8c06c416u;goto L8c06c576; /* bra 0x8c06c576 */
L8c06c418: *stop=0x8c06c418u;r[4]=4294967181u;goto L8c06c41a; /* mov #-0x73,r4 */
L8c06c41a: *stop=0x8c06c41au;r[4]=r[4]&255u;goto L8c06c41c; /* extu.b r4,r4 */
L8c06c41c: target=r[13];*pr=0x8c06c420u;*stop=0x8c06c41eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c420; /* jsr @r13 */
L8c06c420: *stop=0x8c06c420u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c422; /* tst r0,r0 */
L8c06c422: if((*sr&1u))goto L8c06c42a;goto L8c06c424; /* bt 0x8c06c42a */
L8c06c424: *stop=0x8c06c424u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c426; /* mov.w @(0x4,r14),r0 */
L8c06c426: *stop=0x8c06c426u;r[0]|=0x2u;goto L8c06c428; /* or #0x2,r0 */
L8c06c428: *stop=0x8c06c428u;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c42a; /* mov.w r0,@(0x4,r14) */
L8c06c42a: *stop=0x8c06c42au;r[4]=4294967182u;goto L8c06c42c; /* mov #-0x72,r4 */
L8c06c42c: *stop=0x8c06c42cu;r[4]=r[4]&255u;goto L8c06c42e; /* extu.b r4,r4 */
L8c06c42e: target=r[13];*pr=0x8c06c432u;*stop=0x8c06c430u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c432; /* jsr @r13 */
L8c06c432: *stop=0x8c06c432u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c434; /* tst r0,r0 */
L8c06c434: if((*sr&1u))goto L8c06c43c;goto L8c06c436; /* bt 0x8c06c43c */
L8c06c436: *stop=0x8c06c436u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c438; /* mov.w @(0x4,r14),r0 */
L8c06c438: *stop=0x8c06c438u;r[0]|=0x1u;goto L8c06c43a; /* or #0x1,r0 */
L8c06c43a: *stop=0x8c06c43au;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c43c; /* mov.w r0,@(0x4,r14) */
L8c06c43c: *stop=0x8c06c43cu;r[4]=4294967199u;goto L8c06c43e; /* mov #-0x61,r4 */
L8c06c43e: *stop=0x8c06c43eu;r[4]=r[4]&255u;goto L8c06c440; /* extu.b r4,r4 */
L8c06c440: target=r[13];*pr=0x8c06c444u;*stop=0x8c06c442u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c444; /* jsr @r13 */
L8c06c444: *stop=0x8c06c444u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c446; /* tst r0,r0 */
L8c06c446: if((*sr&1u))goto L8c06c44e;goto L8c06c448; /* bt 0x8c06c44e */
L8c06c448: *stop=0x8c06c448u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c44a; /* mov.w @(0x4,r14),r0 */
L8c06c44a: *stop=0x8c06c44au;r[0]|=0x4u;goto L8c06c44c; /* or #0x4,r0 */
L8c06c44c: *stop=0x8c06c44cu;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c44e; /* mov.w r0,@(0x4,r14) */
L8c06c44e: *stop=0x8c06c44eu;r[4]=4294967245u;goto L8c06c450; /* mov #-0x33,r4 */
L8c06c450: *stop=0x8c06c450u;r[4]=r[4]&255u;goto L8c06c452; /* extu.b r4,r4 */
L8c06c452: target=r[13];*pr=0x8c06c456u;*stop=0x8c06c454u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c456; /* jsr @r13 */
L8c06c456: *stop=0x8c06c456u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c458; /* tst r0,r0 */
L8c06c458: if((*sr&1u))goto L8c06c460;goto L8c06c45a; /* bt 0x8c06c460 */
L8c06c45a: *stop=0x8c06c45au;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c45c; /* mov.w @(0x4,r14),r0 */
L8c06c45c: *stop=0x8c06c45cu;r[0]|=0x8u;goto L8c06c45e; /* or #0x8,r0 */
L8c06c45e: *stop=0x8c06c45eu;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c460; /* mov.w r0,@(0x4,r14) */
L8c06c460: *stop=0x8c06c460u;r[4]=4294967259u;goto L8c06c462; /* mov #-0x25,r4 */
L8c06c462: *stop=0x8c06c462u;r[4]=r[4]&255u;goto L8c06c464; /* extu.b r4,r4 */
L8c06c464: target=r[13];*pr=0x8c06c468u;*stop=0x8c06c466u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c468; /* jsr @r13 */
L8c06c468: *stop=0x8c06c468u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c46a; /* tst r0,r0 */
L8c06c46a: if(!(*sr&1u))goto L8c06c470;goto L8c06c46c; /* bf 0x8c06c470 */
L8c06c46c: *stop=0x8c06c46eu;goto L8c06c576; /* bra 0x8c06c576 */
L8c06c470: *stop=0x8c06c470u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c472; /* mov.w @(0x4,r14),r0 */
L8c06c472: *stop=0x8c06c472u;r[0]|=0x10u;goto L8c06c474; /* or #0x10,r0 */
L8c06c474: *stop=0x8c06c476u;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c576; /* bra 0x8c06c576 */
L8c06c4c0: *stop=0x8c06c4c0u;r[4]=1u;goto L8c06c4c2; /* mov #0x1,r4 */
L8c06c4c2: *stop=0x8c06c4c2u;r[4]<<=8;goto L8c06c4c4; /* shll8 r4 */
L8c06c4c4: *stop=0x8c06c4c4u;r[4]+=25u;goto L8c06c4c6; /* add #0x19,r4 */
L8c06c4c6: target=r[13];*pr=0x8c06c4cau;*stop=0x8c06c4c8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c4ca; /* jsr @r13 */
L8c06c4ca: *stop=0x8c06c4cau;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c4cc; /* tst r0,r0 */
L8c06c4cc: if((*sr&1u))goto L8c06c4d4;goto L8c06c4ce; /* bt 0x8c06c4d4 */
L8c06c4ce: *stop=0x8c06c4ceu;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c4d0; /* mov.w @(0x4,r14),r0 */
L8c06c4d0: *stop=0x8c06c4d0u;r[0]|=0x40u;goto L8c06c4d2; /* or #0x40,r0 */
L8c06c4d2: *stop=0x8c06c4d2u;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c4d4; /* mov.w r0,@(0x4,r14) */
L8c06c4d4: *stop=0x8c06c4d4u;r[4]=1u;goto L8c06c4d6; /* mov #0x1,r4 */
L8c06c4d6: *stop=0x8c06c4d6u;r[4]<<=8;goto L8c06c4d8; /* shll8 r4 */
L8c06c4d8: *stop=0x8c06c4d8u;r[4]+=26u;goto L8c06c4da; /* add #0x1a,r4 */
L8c06c4da: target=r[13];*pr=0x8c06c4deu;*stop=0x8c06c4dcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c4de; /* jsr @r13 */
L8c06c4de: *stop=0x8c06c4deu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c4e0; /* tst r0,r0 */
L8c06c4e0: if((*sr&1u))goto L8c06c576;goto L8c06c4e2; /* bt 0x8c06c576 */
L8c06c4e2: *stop=0x8c06c4e2u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c4e4; /* mov.w @(0x4,r14),r0 */
L8c06c4e4: *stop=0x8c06c4e4u;r[0]|=0x20u;goto L8c06c4e6; /* or #0x20,r0 */
L8c06c4e6: *stop=0x8c06c4e8u;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c576; /* bra 0x8c06c576 */
L8c06c4ea: *stop=0x8c06c4eau;r[4]=1u;goto L8c06c4ec; /* mov #0x1,r4 */
L8c06c4ec: *stop=0x8c06c4ecu;r[4]<<=8;goto L8c06c4ee; /* shll8 r4 */
L8c06c4ee: *stop=0x8c06c4eeu;r[4]+=82u;goto L8c06c4f0; /* add #0x52,r4 */
L8c06c4f0: target=r[13];*pr=0x8c06c4f4u;*stop=0x8c06c4f2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c4f4; /* jsr @r13 */
L8c06c4f4: *stop=0x8c06c4f4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c4f6; /* tst r0,r0 */
L8c06c4f6: if((*sr&1u))goto L8c06c502;goto L8c06c4f8; /* bt 0x8c06c502 */
L8c06c4f8: *stop=0x8c06c4f8u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c4fa; /* mov.w @(0x4,r14),r0 */
L8c06c4fa: *stop=0x8c06c4fau;r[3]=1u;goto L8c06c4fc; /* mov #0x1,r3 */
L8c06c4fc: *stop=0x8c06c4fcu;r[3]<<=8;goto L8c06c4fe; /* shll8 r3 */
L8c06c4fe: *stop=0x8c06c4feu;r[0]|=r[3];goto L8c06c500; /* or r3,r0 */
L8c06c500: *stop=0x8c06c500u;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c502; /* mov.w r0,@(0x4,r14) */
L8c06c502: *stop=0x8c06c502u;r[4]=1u;goto L8c06c504; /* mov #0x1,r4 */
L8c06c504: *stop=0x8c06c504u;r[4]<<=8;goto L8c06c506; /* shll8 r4 */
L8c06c506: *stop=0x8c06c506u;r[4]+=94u;goto L8c06c508; /* add #0x5e,r4 */
L8c06c508: target=r[13];*pr=0x8c06c50cu;*stop=0x8c06c50au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c50c; /* jsr @r13 */
L8c06c50c: *stop=0x8c06c50cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c50e; /* tst r0,r0 */
L8c06c50e: if((*sr&1u))goto L8c06c576;goto L8c06c510; /* bt 0x8c06c576 */
L8c06c510: *stop=0x8c06c510u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c512; /* mov.w @(0x4,r14),r0 */
L8c06c512: *stop=0x8c06c512u;r[0]|=0x80u;goto L8c06c514; /* or #0x80,r0 */
L8c06c514: *stop=0x8c06c516u;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c576; /* bra 0x8c06c576 */
L8c06c518: *stop=0x8c06c518u;r[4]=2u;goto L8c06c51a; /* mov #0x2,r4 */
L8c06c51a: *stop=0x8c06c51au;r[4]<<=8;goto L8c06c51c; /* shll8 r4 */
L8c06c51c: *stop=0x8c06c51cu;r[4]+=4294967177u;goto L8c06c51e; /* add #-0x77,r4 */
L8c06c51e: target=r[13];*pr=0x8c06c522u;*stop=0x8c06c520u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c522; /* jsr @r13 */
L8c06c522: *stop=0x8c06c522u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c524; /* tst r0,r0 */
L8c06c524: if((*sr&1u))goto L8c06c530;goto L8c06c526; /* bt 0x8c06c530 */
L8c06c526: *stop=0x8c06c526u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c528; /* mov.w @(0x4,r14),r0 */
L8c06c528: *stop=0x8c06c528u;r[3]=4u;goto L8c06c52a; /* mov #0x4,r3 */
L8c06c52a: *stop=0x8c06c52au;r[3]<<=8;goto L8c06c52c; /* shll8 r3 */
L8c06c52c: *stop=0x8c06c52cu;r[0]|=r[3];goto L8c06c52e; /* or r3,r0 */
L8c06c52e: *stop=0x8c06c52eu;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c530; /* mov.w r0,@(0x4,r14) */
L8c06c530: *stop=0x8c06c530u;r[4]=2u;goto L8c06c532; /* mov #0x2,r4 */
L8c06c532: *stop=0x8c06c532u;r[4]<<=8;goto L8c06c534; /* shll8 r4 */
L8c06c534: *stop=0x8c06c534u;r[4]+=4294967178u;goto L8c06c536; /* add #-0x76,r4 */
L8c06c536: target=r[13];*pr=0x8c06c53au;*stop=0x8c06c538u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c53a; /* jsr @r13 */
L8c06c53a: *stop=0x8c06c53au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c53c; /* tst r0,r0 */
L8c06c53c: if((*sr&1u))goto L8c06c548;goto L8c06c53e; /* bt 0x8c06c548 */
L8c06c53e: *stop=0x8c06c53eu;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c540; /* mov.w @(0x4,r14),r0 */
L8c06c540: *stop=0x8c06c540u;r[3]=2u;goto L8c06c542; /* mov #0x2,r3 */
L8c06c542: *stop=0x8c06c542u;r[3]<<=8;goto L8c06c544; /* shll8 r3 */
L8c06c544: *stop=0x8c06c544u;r[0]|=r[3];goto L8c06c546; /* or r3,r0 */
L8c06c546: *stop=0x8c06c546u;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c548; /* mov.w r0,@(0x4,r14) */
L8c06c548: *stop=0x8c06c548u;r[4]=116u;goto L8c06c54a; /* mov #0x74,r4 */
L8c06c54a: *stop=0x8c06c54au;r[4]<<=2;goto L8c06c54c; /* shll2 r4 */
L8c06c54c: target=r[13];*pr=0x8c06c550u;*stop=0x8c06c54eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c550; /* jsr @r13 */
L8c06c550: *stop=0x8c06c550u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c552; /* tst r0,r0 */
L8c06c552: if((*sr&1u))goto L8c06c55e;goto L8c06c554; /* bt 0x8c06c55e */
L8c06c554: *stop=0x8c06c554u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c556; /* mov.w @(0x4,r14),r0 */
L8c06c556: *stop=0x8c06c556u;r[3]=16u;goto L8c06c558; /* mov #0x10,r3 */
L8c06c558: *stop=0x8c06c558u;r[3]<<=8;goto L8c06c55a; /* shll8 r3 */
L8c06c55a: *stop=0x8c06c55au;r[0]|=r[3];goto L8c06c55c; /* or r3,r0 */
L8c06c55c: *stop=0x8c06c55cu;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c55e; /* mov.w r0,@(0x4,r14) */
L8c06c55e: *stop=0x8c06c55eu;r[4]=2u;goto L8c06c560; /* mov #0x2,r4 */
L8c06c560: *stop=0x8c06c560u;r[4]<<=8;goto L8c06c562; /* shll8 r4 */
L8c06c562: *stop=0x8c06c562u;r[4]+=4294967249u;goto L8c06c564; /* add #-0x2f,r4 */
L8c06c564: target=r[13];*pr=0x8c06c568u;*stop=0x8c06c566u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c568; /* jsr @r13 */
L8c06c568: *stop=0x8c06c568u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c56a; /* tst r0,r0 */
L8c06c56a: if((*sr&1u))goto L8c06c576;goto L8c06c56c; /* bt 0x8c06c576 */
L8c06c56c: *stop=0x8c06c56cu;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06c56e; /* mov.w @(0x4,r14),r0 */
L8c06c56e: *stop=0x8c06c56eu;r[3]=8u;goto L8c06c570; /* mov #0x8,r3 */
L8c06c570: *stop=0x8c06c570u;r[3]<<=8;goto L8c06c572; /* shll8 r3 */
L8c06c572: *stop=0x8c06c572u;r[0]|=r[3];goto L8c06c574; /* or r3,r0 */
L8c06c574: *stop=0x8c06c574u;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c06c576; /* mov.w r0,@(0x4,r14) */
L8c06c576: *stop=0x8c06c576u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c660u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c06c578; /* mov.l 0x8c06c660,r3 */
L8c06c578: *stop=0x8c06c578u;r[5]=r[14];goto L8c06c57a; /* mov r14,r5 */
L8c06c57a: target=r[3];*pr=0x8c06c57eu;*stop=0x8c06c57cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c57e; /* jsr @r3 */
L8c06c57e: *stop=0x8c06c57eu;r[0]=60u;goto L8c06c580; /* mov #0x3c,r0 */
L8c06c580: *stop=0x8c06c580u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0x3f800000u;goto L8c06c582; /* fldi1 fr15 */
L8c06c582: *stop=0x8c06c582u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[11]);if(io_status)return io_status;goto L8c06c584; /* fmov fr15,@(r0,r11) */
L8c06c584: *stop=0x8c06c584u;if(!valid_access(r[12]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x2u,1);if(io_status)return io_status;goto L8c06c586; /* mov.b @(0x2,r12),r0 */
L8c06c586: *stop=0x8c06c586u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06c588; /* tst r0,r0 */
L8c06c588: take=!(*sr&1u);*stop=0x8c06c58au;r[4]=r[14];if(take)goto L8c06c590;goto L8c06c58c; /* bf/s 0x8c06c590 */
L8c06c58c: *stop=0x8c06c58cu;r[0]=1u;goto L8c06c58e; /* mov #0x1,r0 */
L8c06c58e: *stop=0x8c06c58eu;if(!valid_access(r[12]+2u,1u))return -3;wr(m,bus,&io_status,r[12]+0x2u,r[0],1);if(io_status)return io_status;goto L8c06c590; /* mov.b r0,@(0x2,r12) */
L8c06c590: *stop=0x8c06c590u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c664u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c06c592; /* mov.l 0x8c06c664,r3 */
L8c06c592: target=r[3];*pr=0x8c06c596u;*stop=0x8c06c594u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c596; /* jsr @r3 */
L8c06c596: *stop=0x8c06c596u;r[0]=52u;goto L8c06c598; /* mov #0x34,r0 */
L8c06c598: *stop=0x8c06c598u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c668u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c06c59a; /* mov.l 0x8c06c668,r6 */
L8c06c59a: *stop=0x8c06c59au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c06c59c; /* fmov fr15,@(r0,r12) */
L8c06c59c: *stop=0x8c06c59cu;r[0]=48u;goto L8c06c59e; /* mov #0x30,r0 */
L8c06c59e: *stop=0x8c06c59eu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c06c5a0; /* fmov fr15,@(r0,r12) */
L8c06c5a0: *stop=0x8c06c5a0u;r[0]=44u;goto L8c06c5a2; /* mov #0x2c,r0 */
L8c06c5a2: *stop=0x8c06c5a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c66cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c06c5a4; /* mov.l 0x8c06c66c,r3 */
L8c06c5a4: *stop=0x8c06c5a4u;r[5]=2u;goto L8c06c5a6; /* mov #0x2,r5 */
L8c06c5a6: *stop=0x8c06c5a6u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c06c5a8; /* fmov fr15,@(r0,r12) */
L8c06c5a8: target=r[3];*pr=0x8c06c5acu;*stop=0x8c06c5aau;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06c5ac; /* jsr @r3 */
L8c06c5ac: *stop=0x8c06c5acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c06c5ae; /* mov.l @r15,r3 */
L8c06c5ae: *stop=0x8c06c5aeu;r[4]=r[0];goto L8c06c5b0; /* mov r0,r4 */
L8c06c5b0: *stop=0x8c06c5b0u;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c06c5b2; /* mov.l @(0x20,r4),r2 */
L8c06c5b2: *stop=0x8c06c5b2u;r[0]=52u;goto L8c06c5b4; /* mov #0x34,r0 */
L8c06c5b4: *stop=0x8c06c5b4u;*fpul=r[3];goto L8c06c5b6; /* lds r3,FPUL */
L8c06c5b6: *stop=0x8c06c5b6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c06c5b8; /* float FPUL,fr3 */
L8c06c5b8: *stop=0x8c06c5b8u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c06c5ba; /* fmov fr3,@(r0,r2) */
L8c06c5ba: *stop=0x8c06c5bau;r[15]+=4u;goto L8c06c5bc; /* add #0x4,r15 */
L8c06c5bc: *stop=0x8c06c5bcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c06c5be; /* lds.l @r15+,PR */
L8c06c5be: *stop=0x8c06c5beu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c06c5c0; /* fmov.s @r15+,fr15 */
L8c06c5c0: *stop=0x8c06c5c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c06c5c2; /* mov.l @r15+,r11 */
L8c06c5c2: *stop=0x8c06c5c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c06c5c4; /* mov.l @r15+,r12 */
L8c06c5c4: *stop=0x8c06c5c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c06c5c6; /* mov.l @r15+,r13 */
L8c06c5c6: *stop=0x8c06c5c6u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c06c5c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_work_initialize_8c06c350(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c370(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c370u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c394(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c394u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c39c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c39cu);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c3a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c3a6u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c420(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c420u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c432(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c432u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c444(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c444u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c456(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c456u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c468(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c468u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c4ca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c4cau);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c4de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c4deu);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c4f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c4f4u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c50c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c50cu);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c522(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c522u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c53a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c53au);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c550(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c550u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c568(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c568u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c57e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c57eu);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c596(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c596u);}
SA_STAGE_EXPORT int sa_player_work_initialize_resume_8c06c5ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_work_initialize_run_8c06c350(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06c5acu);}
