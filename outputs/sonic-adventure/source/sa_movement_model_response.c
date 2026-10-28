/* Original movement response with recursive model traversal and rendering-state wrapper.
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
static int sa_movement_model_response_run_8c0742d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c074314u)goto L8c074314;if(resume_pc==0x8c074348u)goto L8c074348;if(resume_pc==0x8c074362u)goto L8c074362;if(resume_pc==0x8c0743ceu)goto L8c0743ce;if(resume_pc==0x8c07440cu)goto L8c07440c;if(resume_pc==0x8c07441eu)goto L8c07441e;if(resume_pc==0x8c074582u)goto L8c074582;if(resume_pc==0x8c0745a8u)goto L8c0745a8;if(resume_pc==0x8c0745b4u)goto L8c0745b4;if(resume_pc==0x8c0745bcu)goto L8c0745bc;if(resume_pc==0x8c074672u)goto L8c074672;if(resume_pc==0x8c074684u)goto L8c074684;if(resume_pc==0x8c074696u)goto L8c074696;if(resume_pc==0x8c0746a6u)goto L8c0746a6;if(resume_pc==0x8c0746aeu)goto L8c0746ae;if(resume_pc==0x8c0746bau)goto L8c0746ba;if(resume_pc==0x8c0746c0u)goto L8c0746c0;if(resume_pc==0x8c0746cau)goto L8c0746ca;if(resume_pc==0x8c0746dau)goto L8c0746da;if(resume_pc==0x8c07474eu)goto L8c07474e;if(resume_pc==0x8c074762u)goto L8c074762;if(resume_pc==0x8c07476eu)goto L8c07476e;if(resume_pc==0x8c074786u)goto L8c074786;if(resume_pc==0x8c0747b0u)goto L8c0747b0;goto L8c0742d4;
L8c0742d4: *stop=0x8c0742d4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0742d6; /* mov.l r14,@-r15 */
L8c0742d6: *stop=0x8c0742d6u;r[0]=4294967295u;goto L8c0742d8; /* mov #-0x1,r0 */
L8c0742d8: *stop=0x8c0742d8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0742da; /* mov.l r13,@-r15 */
L8c0742da: *stop=0x8c0742dau;r[0]<<=8;goto L8c0742dc; /* shll8 r0 */
L8c0742dc: *stop=0x8c0742dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0742de; /* mov.l r12,@-r15 */
L8c0742de: *stop=0x8c0742deu;r[0]+=84u;goto L8c0742e0; /* add #0x54,r0 */
L8c0742e0: *stop=0x8c0742e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0742e2; /* mov.l r11,@-r15 */
L8c0742e2: *stop=0x8c0742e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c0742e4; /* mov.l r10,@-r15 */
L8c0742e4: *stop=0x8c0742e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c0742e6; /* mov.l r9,@-r15 */
L8c0742e6: *stop=0x8c0742e6u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c0742e8; /* fmov.s fr15,@-r15 */
L8c0742e8: *stop=0x8c0742e8u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c0742ea; /* fmov.s fr14,@-r15 */
L8c0742ea: *stop=0x8c0742eau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c0742ec; /* fmov.s fr12,@-r15 */
L8c0742ec: *stop=0x8c0742ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0742ee; /* sts.l PR,@-r15 */
L8c0742ee: *stop=0x8c0742eeu;r[15]+=r[0];goto L8c0742f0; /* add r0,r15 */
L8c0742f0: *stop=0x8c0742f0u;r[12]=r[4];goto L8c0742f2; /* mov r4,r12 */
L8c0742f2: *stop=0x8c0742f2u;if(!valid_access(r[12]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x8u,1);if(io_status)return io_status;goto L8c0742f4; /* mov.b @(0x8,r12),r0 */
L8c0742f4: *stop=0x8c0742f4u;r[14]=r[5];goto L8c0742f6; /* mov r5,r14 */
L8c0742f6: *stop=0x8c0742f6u;r[10]=r[15];goto L8c0742f8; /* mov r15,r10 */
L8c0742f8: *stop=0x8c0742f8u;r[5]=r[0]&255u;goto L8c0742fa; /* extu.b r0,r5 */
L8c0742fa: *stop=0x8c0742fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c074390u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0742fc; /* mov.l 0x8c074390,r0 */
L8c0742fc: *stop=0x8c0742fcu;r[5]<<=2;goto L8c0742fe; /* shll2 r5 */
L8c0742fe: *stop=0x8c0742feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074398u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074300; /* mov.l 0x8c074398,r3 */
L8c074300: *stop=0x8c074300u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c074302; /* mov.l @(r0,r5),r4 */
L8c074302: *stop=0x8c074302u;r[10]+=40u;goto L8c074304; /* add #0x28,r10 */
L8c074304: *stop=0x8c074304u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074394u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c074306; /* mov.l 0x8c074394,r0 */
L8c074306: *stop=0x8c074306u;r[11]=r[6];goto L8c074308; /* mov r6,r11 */
L8c074308: *stop=0x8c074308u;r[1]=r[10];goto L8c07430a; /* mov r10,r1 */
L8c07430a: *stop=0x8c07430au;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[13]=loaded;}goto L8c07430c; /* mov.l @(r0,r5),r13 */
L8c07430c: *stop=0x8c07430cu;r[0]=4294967172u;goto L8c07430e; /* mov #-0x7c,r0 */
L8c07430e: *stop=0x8c07430eu;r[0]=r[0]&255u;goto L8c074310; /* extu.b r0,r0 */
L8c074310: target=r[3];*pr=0x8c074314u;*stop=0x8c074312u;r[2]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074314; /* jsr @r3 */
L8c074314: *stop=0x8c074314u;r[0]=24u;goto L8c074316; /* mov #0x18,r0 */
L8c074316: *stop=0x8c074316u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c074318; /* fldi0 fr15 */
L8c074318: *stop=0x8c074318u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c07431a; /* fmov fr15,@(r0,r15) */
L8c07431a: *stop=0x8c07431au;r[0]=16u;goto L8c07431c; /* mov #0x10,r0 */
L8c07431c: *stop=0x8c07431cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0743a0u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c07431e; /* mov.l 0x8c0743a0,r9 */
L8c07431e: *stop=0x8c07431eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c074320; /* fmov fr15,@(r0,r15) */
L8c074320: *stop=0x8c074320u;r[0]=0x8c07439cu;goto L8c074322; /* mova 0x8c07439c,r0 */
L8c074322: *stop=0x8c074322u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c074324; /* fmov.s @r0,fr3 */
L8c074324: *stop=0x8c074324u;r[0]=20u;goto L8c074326; /* mov #0x14,r0 */
L8c074326: *stop=0x8c074326u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074328; /* fmov fr3,@(r0,r15) */
L8c074328: *stop=0x8c074328u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c07432a; /* mov.l @r9,r3 */
L8c07432a: *stop=0x8c07432au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c07432c; /* tst r3,r3 */
L8c07432c: if((*sr&1u))goto L8c07433e;goto L8c07432e; /* bt 0x8c07433e */
L8c07432e: *stop=0x8c07432eu;if(!valid_access(r[12]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x8u,1);if(io_status)return io_status;goto L8c074330; /* mov.b @(0x8,r12),r0 */
L8c074330: *stop=0x8c074330u;r[0]=r[0]&255u;goto L8c074332; /* extu.b r0,r0 */
L8c074332: *stop=0x8c074332u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c074334; /* cmp/eq #0x5,r0 */
L8c074334: if(!(*sr&1u))goto L8c07433e;goto L8c074336; /* bf 0x8c07433e */
L8c074336: *stop=0x8c074336u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0743a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074338; /* mov.l 0x8c0743a4,r3 */
L8c074338: *stop=0x8c074338u;r[0]=20u;goto L8c07433a; /* mov #0x14,r0 */
L8c07433a: *stop=0x8c07433au;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c07433c; /* fmov.s @r3,fr3 */
L8c07433c: *stop=0x8c07433cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07433e; /* fmov fr3,@(r0,r15) */
L8c07433e: *stop=0x8c07433eu;r[7]=r[15];goto L8c074340; /* mov r15,r7 */
L8c074340: *stop=0x8c074340u;r[6]=r[14];goto L8c074342; /* mov r14,r6 */
L8c074342: *stop=0x8c074342u;r[7]+=16u;goto L8c074344; /* add #0x10,r7 */
L8c074344: target=0x8c074cf0u;*pr=0x8c074348u;*stop=0x8c074346u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074348; /* bsr 0x8c074cf0 */
L8c074348: *stop=0x8c074348u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c07434a; /* mov.l @r9,r2 */
L8c07434a: *stop=0x8c07434au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c07434c; /* tst r2,r2 */
L8c07434c: if((*sr&1u))goto L8c074358;goto L8c07434e; /* bt 0x8c074358 */
L8c07434e: *stop=0x8c07434eu;r[3]=32u;goto L8c074350; /* mov #0x20,r3 */
L8c074350: *stop=0x8c074350u;if(!valid_access(r[12]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x4u,2);if(io_status)return io_status;goto L8c074352; /* mov.w @(0x4,r12),r0 */
L8c074352: *stop=0x8c074352u;r[3]<<=8;goto L8c074354; /* shll8 r3 */
L8c074354: *stop=0x8c074354u;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c074356; /* tst r3,r0 */
L8c074356: if((*sr&1u))goto L8c0743c0;goto L8c074358; /* bt 0x8c0743c0 */
L8c074358: *stop=0x8c074358u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0743a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07435a; /* mov.l 0x8c0743a8,r3 */
L8c07435a: *stop=0x8c07435au;r[5]=r[15];goto L8c07435c; /* mov r15,r5 */
L8c07435c: *stop=0x8c07435cu;r[5]+=16u;goto L8c07435e; /* add #0x10,r5 */
L8c07435e: target=r[3];*pr=0x8c074362u;*stop=0x8c074360u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074362; /* jsr @r3 */
L8c074362: *stop=0x8c074364u;goto L8c0743ce; /* bra 0x8c0743ce */
L8c0743c0: *stop=0x8c0743c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074520u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0743c2; /* mov.l 0x8c074520,r3 */
L8c0743c2: *stop=0x8c0743c2u;r[5]=r[15];goto L8c0743c4; /* mov r15,r5 */
L8c0743c4: *stop=0x8c0743c4u;if(!valid_access(r[12]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c0743c6; /* mov.l @(0x18,r12),r6 */
L8c0743c6: *stop=0x8c0743c6u;r[4]=r[14];goto L8c0743c8; /* mov r14,r4 */
L8c0743c8: *stop=0x8c0743c8u;r[5]+=16u;goto L8c0743ca; /* add #0x10,r5 */
L8c0743ca: target=r[3];*pr=0x8c0743ceu;*stop=0x8c0743ccu;r[4]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0743ce; /* jsr @r3 */
L8c0743ce: *stop=0x8c0743ceu;r[0]=36u;goto L8c0743d0; /* mov #0x24,r0 */
L8c0743d0: *stop=0x8c0743d0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0743d2; /* fmov fr15,@(r0,r15) */
L8c0743d2: *stop=0x8c0743d2u;r[0]=28u;goto L8c0743d4; /* mov #0x1c,r0 */
L8c0743d4: *stop=0x8c0743d4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0743d6; /* fmov fr15,@(r0,r15) */
L8c0743d6: *stop=0x8c0743d6u;r[0]=32u;goto L8c0743d8; /* mov #0x20,r0 */
L8c0743d8: *stop=0x8c0743d8u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c0743da; /* fmov.s @r14,fr3 */
L8c0743da: *stop=0x8c0743dau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0743dc; /* fmov fr3,@(r0,r15) */
L8c0743dc: *stop=0x8c0743dcu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0743de; /* mov.l @r9,r3 */
L8c0743de: *stop=0x8c0743deu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0743e0; /* tst r3,r3 */
L8c0743e0: if((*sr&1u))goto L8c0743f2;goto L8c0743e2; /* bt 0x8c0743f2 */
L8c0743e2: *stop=0x8c0743e2u;if(!valid_access(r[12]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x8u,1);if(io_status)return io_status;goto L8c0743e4; /* mov.b @(0x8,r12),r0 */
L8c0743e4: *stop=0x8c0743e4u;r[0]=r[0]&255u;goto L8c0743e6; /* extu.b r0,r0 */
L8c0743e6: *stop=0x8c0743e6u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c0743e8; /* cmp/eq #0x5,r0 */
L8c0743e8: if(!(*sr&1u))goto L8c0743f2;goto L8c0743ea; /* bf 0x8c0743f2 */
L8c0743ea: *stop=0x8c0743eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c074524u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0743ec; /* mov.l 0x8c074524,r3 */
L8c0743ec: *stop=0x8c0743ecu;r[0]=32u;goto L8c0743ee; /* mov #0x20,r0 */
L8c0743ee: *stop=0x8c0743eeu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c0743f0; /* fmov.s @r3,fr3 */
L8c0743f0: *stop=0x8c0743f0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0743f2; /* fmov fr3,@(r0,r15) */
L8c0743f2: *stop=0x8c0743f2u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0743f4; /* mov.l @r9,r2 */
L8c0743f4: *stop=0x8c0743f4u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0743f6; /* tst r2,r2 */
L8c0743f6: if((*sr&1u))goto L8c074402;goto L8c0743f8; /* bt 0x8c074402 */
L8c0743f8: *stop=0x8c0743f8u;r[3]=32u;goto L8c0743fa; /* mov #0x20,r3 */
L8c0743fa: *stop=0x8c0743fau;if(!valid_access(r[12]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x4u,2);if(io_status)return io_status;goto L8c0743fc; /* mov.w @(0x4,r12),r0 */
L8c0743fc: *stop=0x8c0743fcu;r[3]<<=8;goto L8c0743fe; /* shll8 r3 */
L8c0743fe: *stop=0x8c0743feu;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c074400; /* tst r3,r0 */
L8c074400: if((*sr&1u))goto L8c074410;goto L8c074402; /* bt 0x8c074410 */
L8c074402: *stop=0x8c074402u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074528u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074404; /* mov.l 0x8c074528,r3 */
L8c074404: *stop=0x8c074404u;r[5]=r[15];goto L8c074406; /* mov r15,r5 */
L8c074406: *stop=0x8c074406u;r[5]+=28u;goto L8c074408; /* add #0x1c,r5 */
L8c074408: target=r[3];*pr=0x8c07440cu;*stop=0x8c07440au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07440c; /* jsr @r3 */
L8c07440c: *stop=0x8c07440eu;goto L8c07441e; /* bra 0x8c07441e */
L8c074410: *stop=0x8c074410u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074520u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074412; /* mov.l 0x8c074520,r3 */
L8c074412: *stop=0x8c074412u;r[5]=r[15];goto L8c074414; /* mov r15,r5 */
L8c074414: *stop=0x8c074414u;if(!valid_access(r[12]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c074416; /* mov.l @(0x18,r12),r6 */
L8c074416: *stop=0x8c074416u;r[4]=r[14];goto L8c074418; /* mov r14,r4 */
L8c074418: *stop=0x8c074418u;r[5]+=28u;goto L8c07441a; /* add #0x1c,r5 */
L8c07441a: target=r[3];*pr=0x8c07441eu;*stop=0x8c07441cu;r[4]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07441e; /* jsr @r3 */
L8c07441e: *stop=0x8c07441eu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c074420; /* mov.l @r9,r3 */
L8c074420: *stop=0x8c074420u;r[0]=16u;goto L8c074422; /* mov #0x10,r0 */
L8c074422: *stop=0x8c074422u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0x3f800000u;goto L8c074424; /* fldi1 fr14 */
L8c074424: *stop=0x8c074424u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c074426; /* tst r3,r3 */
L8c074426: take=(*sr&1u);*stop=0x8c074428u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;if(take)goto L8c074434;goto L8c07442a; /* bt/s 0x8c074434 */
L8c07442a: *stop=0x8c07442au;r[3]=32u;goto L8c07442c; /* mov #0x20,r3 */
L8c07442c: *stop=0x8c07442cu;if(!valid_access(r[12]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x4u,2);if(io_status)return io_status;goto L8c07442e; /* mov.w @(0x4,r12),r0 */
L8c07442e: *stop=0x8c07442eu;r[3]<<=8;goto L8c074430; /* shll8 r3 */
L8c074430: *stop=0x8c074430u;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c074432; /* tst r3,r0 */
L8c074432: if((*sr&1u))goto L8c0744aa;goto L8c074434; /* bt 0x8c0744aa */
L8c074434: *stop=0x8c074434u;if(!valid_access(r[12]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x8u,1);if(io_status)return io_status;goto L8c074436; /* mov.b @(0x8,r12),r0 */
L8c074436: *stop=0x8c074436u;r[0]=r[0]&255u;goto L8c074438; /* extu.b r0,r0 */
L8c074438: *stop=0x8c074438u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c07443a; /* cmp/eq #0x7,r0 */
L8c07443a: if(!(*sr&1u))goto L8c07445e;goto L8c07443c; /* bf 0x8c07445e */
L8c07443c: *stop=0x8c07443cu;r[0]=84u;goto L8c07443e; /* mov #0x54,r0 */
L8c07443e: *stop=0x8c07443eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c074440; /* fmov.s @(r0,r14),fr3 */
L8c074440: *stop=0x8c074440u;r[0]=4u;goto L8c074442; /* mov #0x4,r0 */
L8c074442: *stop=0x8c074442u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c074444; /* fadd fr4,fr3 */
L8c074444: *stop=0x8c074444u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c074446; /* fmov fr3,@(r0,r14) */
L8c074446: *stop=0x8c074446u;r[0]=20u;goto L8c074448; /* mov #0x14,r0 */
L8c074448: *stop=0x8c074448u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07444a; /* fmov.s @(r0,r15),fr3 */
L8c07444a: *stop=0x8c07444au;r[0]=88u;goto L8c07444c; /* mov #0x58,r0 */
L8c07444c: *stop=0x8c07444cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07444e; /* fmov.s @(r0,r14),fr2 */
L8c07444e: *stop=0x8c07444eu;r[0]=8u;goto L8c074450; /* mov #0x8,r0 */
L8c074450: *stop=0x8c074450u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c074452; /* fadd fr3,fr2 */
L8c074452: *stop=0x8c074452u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074454; /* fmov fr2,@(r0,r14) */
L8c074454: *stop=0x8c074454u;r[0]=24u;goto L8c074456; /* mov #0x18,r0 */
L8c074456: *stop=0x8c074456u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074458; /* fmov.s @(r0,r15),fr3 */
L8c074458: *stop=0x8c074458u;r[0]=92u;goto L8c07445a; /* mov #0x5c,r0 */
L8c07445a: *stop=0x8c07445cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0744a2; /* bra 0x8c0744a2 */
L8c07445e: *stop=0x8c07445eu;r[0]=4u;goto L8c074460; /* mov #0x4,r0 */
L8c074460: *stop=0x8c074460u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c074462; /* fmov.s @(r0,r14),fr3 */
L8c074462: *stop=0x8c074462u;r[0]=96u;goto L8c074464; /* mov #0x60,r0 */
L8c074464: *stop=0x8c074464u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074466; /* fmov.s @(r0,r14),fr2 */
L8c074466: *stop=0x8c074466u;r[0]=32u;goto L8c074468; /* mov #0x20,r0 */
L8c074468: *stop=0x8c074468u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07446a; /* fsub fr3,fr2 */
L8c07446a: *stop=0x8c07446au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c07446c; /* fmov.s @(r0,r12),fr3 */
L8c07446c: *stop=0x8c07446cu;r[0]=4u;goto L8c07446e; /* mov #0x4,r0 */
L8c07446e: *stop=0x8c07446eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c074470; /* fadd fr3,fr2 */
L8c074470: *stop=0x8c074470u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c074472; /* fadd fr4,fr2 */
L8c074472: *stop=0x8c074472u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074474; /* fmov fr2,@(r0,r14) */
L8c074474: *stop=0x8c074474u;r[0]=8u;goto L8c074476; /* mov #0x8,r0 */
L8c074476: *stop=0x8c074476u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c074478; /* fmov.s @(r0,r14),fr3 */
L8c074478: *stop=0x8c074478u;r[0]=100u;goto L8c07447a; /* mov #0x64,r0 */
L8c07447a: *stop=0x8c07447au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07447c; /* fmov.s @(r0,r14),fr2 */
L8c07447c: *stop=0x8c07447cu;r[0]=36u;goto L8c07447e; /* mov #0x24,r0 */
L8c07447e: *stop=0x8c07447eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074480; /* fsub fr3,fr2 */
L8c074480: *stop=0x8c074480u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c074482; /* fmov.s @(r0,r12),fr3 */
L8c074482: *stop=0x8c074482u;r[0]=20u;goto L8c074484; /* mov #0x14,r0 */
L8c074484: *stop=0x8c074484u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c074486; /* fadd fr3,fr2 */
L8c074486: *stop=0x8c074486u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074488; /* fmov.s @(r0,r15),fr3 */
L8c074488: *stop=0x8c074488u;r[0]=8u;goto L8c07448a; /* mov #0x8,r0 */
L8c07448a: *stop=0x8c07448au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07448c; /* fadd fr3,fr2 */
L8c07448c: *stop=0x8c07448cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07448e; /* fmov fr2,@(r0,r14) */
L8c07448e: *stop=0x8c07448eu;r[0]=12u;goto L8c074490; /* mov #0xc,r0 */
L8c074490: *stop=0x8c074490u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c074492; /* fmov.s @(r0,r14),fr3 */
L8c074492: *stop=0x8c074492u;r[0]=104u;goto L8c074494; /* mov #0x68,r0 */
L8c074494: *stop=0x8c074494u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074496; /* fmov.s @(r0,r14),fr2 */
L8c074496: *stop=0x8c074496u;r[0]=40u;goto L8c074498; /* mov #0x28,r0 */
L8c074498: *stop=0x8c074498u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07449a; /* fsub fr3,fr2 */
L8c07449a: *stop=0x8c07449au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c07449c; /* fmov.s @(r0,r12),fr3 */
L8c07449c: *stop=0x8c07449cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07449e; /* fadd fr3,fr2 */
L8c07449e: *stop=0x8c07449eu;r[0]=24u;goto L8c0744a0; /* mov #0x18,r0 */
L8c0744a0: *stop=0x8c0744a0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0744a2; /* fmov.s @(r0,r15),fr3 */
L8c0744a2: *stop=0x8c0744a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0744a4; /* fadd fr3,fr2 */
L8c0744a4: *stop=0x8c0744a4u;r[0]=12u;goto L8c0744a6; /* mov #0xc,r0 */
L8c0744a6: *stop=0x8c0744a8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0745dc; /* bra 0x8c0745dc */
L8c0744aa: *stop=0x8c0744aau;r[3]=64u;goto L8c0744ac; /* mov #0x40,r3 */
L8c0744ac: *stop=0x8c0744acu;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0744ae; /* mov.l @r11,r2 */
L8c0744ae: *stop=0x8c0744aeu;r[3]<<=8;goto L8c0744b0; /* shll8 r3 */
L8c0744b0: *stop=0x8c0744b0u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c0744b2; /* tst r3,r2 */
L8c0744b2: *stop=0x8c0744b2u;r[0]=28u;goto L8c0744b4; /* mov #0x1c,r0 */
L8c0744b4: take=(*sr&1u);*stop=0x8c0744b6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;if(take)goto L8c074540;goto L8c0744b8; /* bt/s 0x8c074540 */
L8c0744b8: *stop=0x8c0744b8u;r[0]=96u;goto L8c0744ba; /* mov #0x60,r0 */
L8c0744ba: *stop=0x8c0744bau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0744bc; /* fmov.s @(r0,r14),fr3 */
L8c0744bc: *stop=0x8c0744bcu;r[0]=108u;goto L8c0744be; /* mov #0x6c,r0 */
L8c0744be: *stop=0x8c0744beu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0744c0; /* fmov.s @(r0,r14),fr2 */
L8c0744c0: *stop=0x8c0744c0u;r[0]=4u;goto L8c0744c2; /* mov #0x4,r0 */
L8c0744c2: *stop=0x8c0744c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0744c4; /* fadd fr3,fr2 */
L8c0744c4: *stop=0x8c0744c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0744c6; /* fsub fr5,fr2 */
L8c0744c6: *stop=0x8c0744c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0744c8; /* fadd fr4,fr2 */
L8c0744c8: *stop=0x8c0744c8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0744ca; /* fmov fr2,@(r0,r14) */
L8c0744ca: *stop=0x8c0744cau;r[0]=100u;goto L8c0744cc; /* mov #0x64,r0 */
L8c0744cc: *stop=0x8c0744ccu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0744ce; /* fmov.s @(r0,r14),fr3 */
L8c0744ce: *stop=0x8c0744ceu;r[0]=112u;goto L8c0744d0; /* mov #0x70,r0 */
L8c0744d0: *stop=0x8c0744d0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0744d2; /* fmov.s @(r0,r14),fr2 */
L8c0744d2: *stop=0x8c0744d2u;r[0]=32u;goto L8c0744d4; /* mov #0x20,r0 */
L8c0744d4: *stop=0x8c0744d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0744d6; /* fadd fr3,fr2 */
L8c0744d6: *stop=0x8c0744d6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0744d8; /* fmov.s @(r0,r15),fr3 */
L8c0744d8: *stop=0x8c0744d8u;r[0]=20u;goto L8c0744da; /* mov #0x14,r0 */
L8c0744da: *stop=0x8c0744dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0744dc; /* fsub fr3,fr2 */
L8c0744dc: *stop=0x8c0744dcu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0744de; /* fmov.s @(r0,r15),fr3 */
L8c0744de: *stop=0x8c0744deu;r[0]=8u;goto L8c0744e0; /* mov #0x8,r0 */
L8c0744e0: *stop=0x8c0744e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0744e2; /* fadd fr3,fr2 */
L8c0744e2: *stop=0x8c0744e2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0744e4; /* fmov fr2,@(r0,r14) */
L8c0744e4: *stop=0x8c0744e4u;r[0]=104u;goto L8c0744e6; /* mov #0x68,r0 */
L8c0744e6: *stop=0x8c0744e6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0744e8; /* fmov.s @(r0,r14),fr3 */
L8c0744e8: *stop=0x8c0744e8u;r[0]=116u;goto L8c0744ea; /* mov #0x74,r0 */
L8c0744ea: *stop=0x8c0744eau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0744ec; /* fmov.s @(r0,r14),fr2 */
L8c0744ec: *stop=0x8c0744ecu;r[0]=36u;goto L8c0744ee; /* mov #0x24,r0 */
L8c0744ee: *stop=0x8c0744eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0744f0; /* fadd fr3,fr2 */
L8c0744f0: *stop=0x8c0744f0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0744f2; /* fmov.s @(r0,r15),fr3 */
L8c0744f2: *stop=0x8c0744f2u;r[0]=24u;goto L8c0744f4; /* mov #0x18,r0 */
L8c0744f4: *stop=0x8c0744f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0744f6; /* fsub fr3,fr2 */
L8c0744f6: *stop=0x8c0744f6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0744f8; /* fmov.s @(r0,r15),fr3 */
L8c0744f8: *stop=0x8c0744f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0744fa; /* fadd fr3,fr2 */
L8c0744fa: *stop=0x8c0744fau;r[0]=12u;goto L8c0744fc; /* mov #0xc,r0 */
L8c0744fc: *stop=0x8c0744fcu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0744fe; /* fmov fr2,@(r0,r14) */
L8c0744fe: *stop=0x8c0744feu;r[0]=24u;goto L8c074500; /* mov #0x18,r0 */
L8c074500: *stop=0x8c074500u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c074502; /* fmov fr15,@(r0,r14) */
L8c074502: *stop=0x8c074502u;r[0]=20u;goto L8c074504; /* mov #0x14,r0 */
L8c074504: *stop=0x8c074504u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c074506; /* fmov fr15,@(r0,r14) */
L8c074506: *stop=0x8c074506u;r[0]=16u;goto L8c074508; /* mov #0x10,r0 */
L8c074508: *stop=0x8c07450au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c0745dc; /* bra 0x8c0745dc */
L8c074540: *stop=0x8c074540u;r[0]=96u;goto L8c074542; /* mov #0x60,r0 */
L8c074542: *stop=0x8c074542u;r[5]=r[14];goto L8c074544; /* mov r14,r5 */
L8c074544: *stop=0x8c074544u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c074546; /* fmov.s @(r0,r14),fr3 */
L8c074546: *stop=0x8c074546u;r[0]=4u;goto L8c074548; /* mov #0x4,r0 */
L8c074548: *stop=0x8c074548u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0746e0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07454a; /* mov.l 0x8c0746e0,r2 */
L8c07454a: *stop=0x8c07454au;r[4]=r[14];goto L8c07454c; /* mov r14,r4 */
L8c07454c: *stop=0x8c07454cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c07454e; /* fsub fr5,fr3 */
L8c07454e: *stop=0x8c07454eu;r[5]+=72u;goto L8c074550; /* add #0x48,r5 */
L8c074550: *stop=0x8c074550u;r[6]=0u;goto L8c074552; /* mov #0x0,r6 */
L8c074552: *stop=0x8c074552u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c074554; /* fadd fr4,fr3 */
L8c074554: *stop=0x8c074554u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c074556; /* fmov fr3,@(r0,r14) */
L8c074556: *stop=0x8c074556u;r[0]=32u;goto L8c074558; /* mov #0x20,r0 */
L8c074558: *stop=0x8c074558u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07455a; /* fmov.s @(r0,r15),fr3 */
L8c07455a: *stop=0x8c07455au;r[0]=100u;goto L8c07455c; /* mov #0x64,r0 */
L8c07455c: *stop=0x8c07455cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07455e; /* fmov.s @(r0,r14),fr2 */
L8c07455e: *stop=0x8c07455eu;r[0]=20u;goto L8c074560; /* mov #0x14,r0 */
L8c074560: *stop=0x8c074560u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074562; /* fsub fr3,fr2 */
L8c074562: *stop=0x8c074562u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074564; /* fmov.s @(r0,r15),fr3 */
L8c074564: *stop=0x8c074564u;r[0]=8u;goto L8c074566; /* mov #0x8,r0 */
L8c074566: *stop=0x8c074566u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c074568; /* fadd fr3,fr2 */
L8c074568: *stop=0x8c074568u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07456a; /* fmov fr2,@(r0,r14) */
L8c07456a: *stop=0x8c07456au;r[0]=36u;goto L8c07456c; /* mov #0x24,r0 */
L8c07456c: *stop=0x8c07456cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07456e; /* fmov.s @(r0,r15),fr3 */
L8c07456e: *stop=0x8c07456eu;r[0]=104u;goto L8c074570; /* mov #0x68,r0 */
L8c074570: *stop=0x8c074570u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074572; /* fmov.s @(r0,r14),fr2 */
L8c074572: *stop=0x8c074572u;r[0]=24u;goto L8c074574; /* mov #0x18,r0 */
L8c074574: *stop=0x8c074574u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074576; /* fsub fr3,fr2 */
L8c074576: *stop=0x8c074576u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074578; /* fmov.s @(r0,r15),fr3 */
L8c074578: *stop=0x8c074578u;r[0]=12u;goto L8c07457a; /* mov #0xc,r0 */
L8c07457a: *stop=0x8c07457au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07457c; /* fadd fr3,fr2 */
L8c07457c: *stop=0x8c07457cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07457e; /* fmov fr2,@(r0,r14) */
L8c07457e: target=r[2];*pr=0x8c074582u;*stop=0x8c074580u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074582; /* jsr @r2 */
L8c074582: *stop=0x8c074582u;r[3]=44u;goto L8c074584; /* mov #0x2c,r3 */
L8c074584: *stop=0x8c074584u;r[4]=r[0];goto L8c074586; /* mov r0,r4 */
L8c074586: *stop=0x8c074586u;r[3]<<=8;goto L8c074588; /* shll8 r3 */
L8c074588: *stop=0x8c074588u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c07458a; /* cmp/ge r3,r4 */
L8c07458a: if(!(*sr&1u))goto L8c0745dc;goto L8c07458c; /* bf 0x8c0745dc */
L8c07458c: *stop=0x8c07458cu;r[1]=r[14];goto L8c07458e; /* mov r14,r1 */
L8c07458e: *stop=0x8c07458eu;r[1]+=108u;goto L8c074590; /* add #0x6c,r1 */
L8c074590: *stop=0x8c074590u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c074592; /* mov.l @r1,r0 */
L8c074592: *stop=0x8c074592u;r[2]=r[15];goto L8c074594; /* mov r15,r2 */
L8c074594: *stop=0x8c074594u;r[2]+=4u;goto L8c074596; /* add #0x4,r2 */
L8c074596: *stop=0x8c074596u;if(!valid_access(r[1]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074598; /* mov.l @(0x4,r1),r3 */
L8c074598: *stop=0x8c074598u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c07459a; /* mov.l r0,@r2 */
L8c07459a: *stop=0x8c07459au;r[4]=r[15];goto L8c07459c; /* mov r15,r4 */
L8c07459c: *stop=0x8c07459cu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c07459e; /* mov.l r3,@(0x4,r2) */
L8c07459e: *stop=0x8c07459eu;if(!valid_access(r[1]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0745a0; /* mov.l @(0x8,r1),r0 */
L8c0745a0: *stop=0x8c0745a0u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[0],4);if(io_status)return io_status;goto L8c0745a2; /* mov.l r0,@(0x8,r2) */
L8c0745a2: *stop=0x8c0745a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0746e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0745a4; /* mov.l 0x8c0746e4,r2 */
L8c0745a4: target=r[2];*pr=0x8c0745a8u;*stop=0x8c0745a6u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0745a8; /* jsr @r2 */
L8c0745a8: *stop=0x8c0745a8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][14],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0745aa; /* fcmp/gt fr0,fr14 */
L8c0745aa: if((*sr&1u))goto L8c0745dc;goto L8c0745ac; /* bt 0x8c0745dc */
L8c0745ac: *stop=0x8c0745acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0746e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0745ae; /* mov.l 0x8c0746e8,r2 */
L8c0745ae: *stop=0x8c0745aeu;r[4]=r[15];goto L8c0745b0; /* mov r15,r4 */
L8c0745b0: target=r[2];*pr=0x8c0745b4u;*stop=0x8c0745b2u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0745b4; /* jsr @r2 */
L8c0745b4: *stop=0x8c0745b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0746e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0745b6; /* mov.l 0x8c0746e4,r3 */
L8c0745b6: *stop=0x8c0745b6u;r[4]=r[14];goto L8c0745b8; /* mov r14,r4 */
L8c0745b8: target=r[3];*pr=0x8c0745bcu;*stop=0x8c0745bau;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0745bc; /* jsr @r3 */
L8c0745bc: *stop=0x8c0745bcu;r[0]=4u;goto L8c0745be; /* mov #0x4,r0 */
L8c0745be: *stop=0x8c0745beu;fcopy(f,4u,0u);goto L8c0745c0; /* fmov fr0,fr4 */
L8c0745c0: *stop=0x8c0745c0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0745c2; /* fmov.s @(r0,r15),fr3 */
L8c0745c2: *stop=0x8c0745c2u;r[0]=16u;goto L8c0745c4; /* mov #0x10,r0 */
L8c0745c4: *stop=0x8c0745c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0745c6; /* fmul fr4,fr3 */
L8c0745c6: *stop=0x8c0745c6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0745c8; /* fmov fr3,@(r0,r14) */
L8c0745c8: *stop=0x8c0745c8u;r[0]=8u;goto L8c0745ca; /* mov #0x8,r0 */
L8c0745ca: *stop=0x8c0745cau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0745cc; /* fmov.s @(r0,r15),fr3 */
L8c0745cc: *stop=0x8c0745ccu;r[0]=20u;goto L8c0745ce; /* mov #0x14,r0 */
L8c0745ce: *stop=0x8c0745ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0745d0; /* fmul fr4,fr3 */
L8c0745d0: *stop=0x8c0745d0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0745d2; /* fmov fr3,@(r0,r14) */
L8c0745d2: *stop=0x8c0745d2u;r[0]=12u;goto L8c0745d4; /* mov #0xc,r0 */
L8c0745d4: *stop=0x8c0745d4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0745d6; /* fmov.s @(r0,r15),fr3 */
L8c0745d6: *stop=0x8c0745d6u;r[0]=24u;goto L8c0745d8; /* mov #0x18,r0 */
L8c0745d8: *stop=0x8c0745d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0745da; /* fmul fr4,fr3 */
L8c0745da: *stop=0x8c0745dau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0745dc; /* fmov fr3,@(r0,r14) */
L8c0745dc: *stop=0x8c0745dcu;r[0]=4294967248u;goto L8c0745de; /* mov #-0x30,r0 */
L8c0745de: *stop=0x8c0745deu;r[0]=r[0]&255u;goto L8c0745e0; /* extu.b r0,r0 */
L8c0745e0: *stop=0x8c0745e0u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0745e2; /* fmov.s @(r0,r13),fr3 */
L8c0745e2: *stop=0x8c0745e2u;r[0]=56u;goto L8c0745e4; /* mov #0x38,r0 */
L8c0745e4: *stop=0x8c0745e4u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0745e6; /* fmov.s @(r0,r13),fr2 */
L8c0745e6: *stop=0x8c0745e6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c0745e8; /* fcmp/gt fr2,fr3 */
L8c0745e8: if(!(*sr&1u))goto L8c074606;goto L8c0745ea; /* bf 0x8c074606 */
L8c0745ea: *stop=0x8c0745eau;r[0]=96u;goto L8c0745ec; /* mov #0x60,r0 */
L8c0745ec: *stop=0x8c0745ecu;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0745ee; /* mov.l @(r0,r13),r2 */
L8c0745ee: *stop=0x8c0745eeu;r[3]=64u;goto L8c0745f0; /* mov #0x40,r3 */
L8c0745f0: *stop=0x8c0745f0u;r[3]<<=8;goto L8c0745f2; /* shll8 r3 */
L8c0745f2: *stop=0x8c0745f2u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c0745f4; /* tst r3,r2 */
L8c0745f4: if(!(*sr&1u))goto L8c074606;goto L8c0745f6; /* bf 0x8c074606 */
L8c0745f6: *stop=0x8c0745f6u;r[0]=60u;goto L8c0745f8; /* mov #0x3c,r0 */
L8c0745f8: *stop=0x8c0745f8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0745fa; /* fldi0 fr3 */
L8c0745fa: *stop=0x8c0745fau;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0745fc; /* fmov.s @(r0,r13),fr2 */
L8c0745fc: *stop=0x8c0745fcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0745fe; /* fcmp/gt fr3,fr2 */
L8c0745fe: if((*sr&1u))goto L8c07460c;goto L8c074600; /* bt 0x8c07460c */
L8c074600: *stop=0x8c074600u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[1]=loaded;}goto L8c074602; /* mov.l @r11,r1 */
L8c074602: *stop=0x8c074602u;*sr=(*sr&~1u)|!(r[1]&r[3]);goto L8c074604; /* tst r1,r3 */
L8c074604: if((*sr&1u))goto L8c07460c;goto L8c074606; /* bt 0x8c07460c */
L8c074606: *stop=0x8c074606u;r[0]=0x8c0746ecu;goto L8c074608; /* mova 0x8c0746ec,r0 */
L8c074608: *stop=0x8c07460au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c074640; /* bra 0x8c074640 */
L8c07460c: *stop=0x8c07460cu;r[0]=0x8c0746f0u;goto L8c07460e; /* mova 0x8c0746f0,r0 */
L8c07460e: *stop=0x8c07460eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c074610; /* fmov.s @r0,fr4 */
L8c074610: *stop=0x8c074610u;r[0]=56u;goto L8c074612; /* mov #0x38,r0 */
L8c074612: *stop=0x8c074612u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c074614; /* fmov.s @(r0,r13),fr5 */
L8c074614: *stop=0x8c074614u;r[0]=4294967232u;goto L8c074616; /* mov #-0x40,r0 */
L8c074616: *stop=0x8c074616u;r[0]=r[0]&255u;goto L8c074618; /* extu.b r0,r0 */
L8c074618: *stop=0x8c074618u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c07461a; /* fmov.s @(r0,r13),fr3 */
L8c07461a: *stop=0x8c07461au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c07461c; /* fcmp/gt fr5,fr3 */
L8c07461c: if((*sr&1u))goto L8c074644;goto L8c07461e; /* bt 0x8c074644 */
L8c07461e: *stop=0x8c07461eu;r[0]=4294967248u;goto L8c074620; /* mov #-0x30,r0 */
L8c074620: *stop=0x8c074620u;r[0]=r[0]&255u;goto L8c074622; /* extu.b r0,r0 */
L8c074622: *stop=0x8c074622u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c074624; /* fmov.s @(r0,r13),fr3 */
L8c074624: *stop=0x8c074624u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074626; /* fcmp/gt fr3,fr5 */
L8c074626: if((*sr&1u))goto L8c074644;goto L8c074628; /* bt 0x8c074644 */
L8c074628: *stop=0x8c074628u;r[0]=4294967232u;goto L8c07462a; /* mov #-0x40,r0 */
L8c07462a: *stop=0x8c07462au;r[0]=r[0]&255u;goto L8c07462c; /* extu.b r0,r0 */
L8c07462c: *stop=0x8c07462cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c07462e; /* fmov.s @(r0,r13),fr5 */
L8c07462e: *stop=0x8c07462eu;r[0]=56u;goto L8c074630; /* mov #0x38,r0 */
L8c074630: *stop=0x8c074630u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c074632; /* fmov.s @(r0,r13),fr3 */
L8c074632: *stop=0x8c074632u;r[0]=4294967248u;goto L8c074634; /* mov #-0x30,r0 */
L8c074634: *stop=0x8c074634u;r[0]=r[0]&255u;goto L8c074636; /* extu.b r0,r0 */
L8c074636: *stop=0x8c074636u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074638; /* fmov.s @(r0,r13),fr2 */
L8c074638: *stop=0x8c074638u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c07463a; /* fsub fr5,fr3 */
L8c07463a: *stop=0x8c07463au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07463c; /* fsub fr5,fr2 */
L8c07463c: *stop=0x8c07463cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c07463e; /* fdiv fr2,fr3 */
L8c07463e: *stop=0x8c07463eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c074640; /* fadd fr4,fr3 */
L8c074640: *stop=0x8c074642u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c074652; /* bra 0x8c074652 */
L8c074644: *stop=0x8c074644u;r[0]=4294967232u;goto L8c074646; /* mov #-0x40,r0 */
L8c074646: *stop=0x8c074646u;r[0]=r[0]&255u;goto L8c074648; /* extu.b r0,r0 */
L8c074648: *stop=0x8c074648u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c07464a; /* fmov.s @(r0,r13),fr3 */
L8c07464a: *stop=0x8c07464au;r[0]=56u;goto L8c07464c; /* mov #0x38,r0 */
L8c07464c: *stop=0x8c07464cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07464e; /* fmov.s @(r0,r13),fr2 */
L8c07464e: *stop=0x8c07464eu;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c074650; /* fmov.s fr4,@r14 */
L8c074650: *stop=0x8c074650u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c074652; /* fcmp/gt fr2,fr3 */
L8c074652: *stop=0x8c074652u;if(!valid_access(r[12]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x8u,1);if(io_status)return io_status;goto L8c074654; /* mov.b @(0x8,r12),r0 */
L8c074654: *stop=0x8c074654u;r[0]=r[0]&255u;goto L8c074656; /* extu.b r0,r0 */
L8c074656: *stop=0x8c074656u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c074658; /* cmp/eq #0x7,r0 */
L8c074658: if(!(*sr&1u))goto L8c074664;goto L8c07465a; /* bf 0x8c074664 */
L8c07465a: *stop=0x8c07465au;r[0]=0x8c0746f4u;goto L8c07465c; /* mova 0x8c0746f4,r0 */
L8c07465c: *stop=0x8c07465cu;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c07465e; /* fmov.s @r14,fr2 */
L8c07465e: *stop=0x8c07465eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c074660; /* fmov.s @r0,fr3 */
L8c074660: *stop=0x8c074660u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c074662; /* fadd fr3,fr2 */
L8c074662: *stop=0x8c074662u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c074664; /* fmov.s fr2,@r14 */
L8c074664: *stop=0x8c074664u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0746f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074666; /* mov.l 0x8c0746f8,r2 */
L8c074666: *stop=0x8c074666u;if(!valid_access(r[2],1u))return -3;r[3]=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;goto L8c074668; /* mov.b @r2,r3 */
L8c074668: *stop=0x8c074668u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c07466a; /* cmp/pz r3 */
L8c07466a: if(!(*sr&1u))goto L8c0746c0;goto L8c07466c; /* bf 0x8c0746c0 */
L8c07466c: *stop=0x8c07466cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0746fcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07466e; /* mov.l 0x8c0746fc,r1 */
L8c07466e: target=r[1];*pr=0x8c074672u;*stop=0x8c074670u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074672; /* jsr @r1 */
L8c074672: *stop=0x8c074672u;r[0]=12u;goto L8c074674; /* mov #0xc,r0 */
L8c074674: *stop=0x8c074674u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074700u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074676; /* mov.l 0x8c074700,r3 */
L8c074676: *stop=0x8c074676u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[14]);if(io_status)return io_status;goto L8c074678; /* fmov.s @(r0,r14),fr6 */
L8c074678: *stop=0x8c074678u;r[0]=8u;goto L8c07467a; /* mov #0x8,r0 */
L8c07467a: *stop=0x8c07467au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c07467c; /* fmov.s @(r0,r14),fr5 */
L8c07467c: *stop=0x8c07467cu;r[0]=4u;goto L8c07467e; /* mov #0x4,r0 */
L8c07467e: *stop=0x8c07467eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c074680; /* fmov.s @(r0,r14),fr4 */
L8c074680: target=r[3];*pr=0x8c074684u;*stop=0x8c074682u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074684; /* jsr @r3 */
L8c074684: *stop=0x8c074684u;r[0]=0x8c074704u;goto L8c074686; /* mova 0x8c074704,r0 */
L8c074686: *stop=0x8c074686u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074710u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074688; /* mov.l 0x8c074710,r3 */
L8c074688: *stop=0x8c074688u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c07468a; /* fmov.s @r0,fr7 */
L8c07468a: *stop=0x8c07468au;r[0]=0x8c074708u;goto L8c07468c; /* mova 0x8c074708,r0 */
L8c07468c: *stop=0x8c07468cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c07468e; /* fmov.s @r0,fr6 */
L8c07468e: *stop=0x8c07468eu;r[0]=0x8c07470cu;goto L8c074690; /* mova 0x8c07470c,r0 */
L8c074690: *stop=0x8c074690u;fcopy(f,5u,14u);goto L8c074692; /* fmov fr14,fr5 */
L8c074692: target=r[3];*pr=0x8c074696u;*stop=0x8c074694u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074696; /* jsr @r3 */
L8c074696: *stop=0x8c074696u;r[0]=0x8c074714u;goto L8c074698; /* mova 0x8c074714,r0 */
L8c074698: *stop=0x8c074698u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074718u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07469a; /* mov.l 0x8c074718,r3 */
L8c07469a: *stop=0x8c07469au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c07469c; /* fmov.s @r0,fr14 */
L8c07469c: *stop=0x8c07469cu;fcopy(f,4u,14u);goto L8c07469e; /* fmov fr14,fr4 */
L8c07469e: *stop=0x8c07469eu;fcopy(f,6u,14u);goto L8c0746a0; /* fmov fr14,fr6 */
L8c0746a0: *stop=0x8c0746a0u;fcopy(f,5u,14u);goto L8c0746a2; /* fmov fr14,fr5 */
L8c0746a2: target=r[3];*pr=0x8c0746a6u;*stop=0x8c0746a4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0746a6; /* jsr @r3 */
L8c0746a6: *stop=0x8c0746a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074720u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0746a8; /* mov.l 0x8c074720,r2 */
L8c0746a8: *stop=0x8c0746a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c07471cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0746aa; /* mov.l 0x8c07471c,r4 */
L8c0746aa: target=r[2];*pr=0x8c0746aeu;*stop=0x8c0746acu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0746ae; /* jsr @r2 */
L8c0746ae: *stop=0x8c0746aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074710u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0746b0; /* mov.l 0x8c074710,r3 */
L8c0746b0: *stop=0x8c0746b0u;fcopy(f,5u,15u);goto L8c0746b2; /* fmov fr15,fr5 */
L8c0746b2: *stop=0x8c0746b2u;fcopy(f,7u,15u);goto L8c0746b4; /* fmov fr15,fr7 */
L8c0746b4: *stop=0x8c0746b4u;fcopy(f,6u,15u);goto L8c0746b6; /* fmov fr15,fr6 */
L8c0746b6: target=r[3];*pr=0x8c0746bau;*stop=0x8c0746b8u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0746ba; /* jsr @r3 */
L8c0746ba: *stop=0x8c0746bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c074724u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0746bc; /* mov.l 0x8c074724,r2 */
L8c0746bc: target=r[2];*pr=0x8c0746c0u;*stop=0x8c0746beu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0746c0; /* jsr @r2 */
L8c0746c0: *stop=0x8c0746c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074728u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0746c2; /* mov.l 0x8c074728,r3 */
L8c0746c2: *stop=0x8c0746c2u;if(!valid_access(r[11]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0746c4; /* mov.l @(0x4,r11),r5 */
L8c0746c4: *stop=0x8c0746c4u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[6]=loaded;}goto L8c0746c6; /* mov.l @r11,r6 */
L8c0746c6: target=r[3];*pr=0x8c0746cau;*stop=0x8c0746c8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0746ca; /* jsr @r3 */
L8c0746ca: *stop=0x8c0746cau;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0746cc; /* tst r0,r0 */
L8c0746cc: if(!(*sr&1u))goto L8c074740;goto L8c0746ce; /* bf 0x8c074740 */
L8c0746ce: *stop=0x8c0746ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07472cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0746d0; /* mov.l 0x8c07472c,r3 */
L8c0746d0: *stop=0x8c0746d0u;r[0]=4294967172u;goto L8c0746d2; /* mov #-0x7c,r0 */
L8c0746d2: *stop=0x8c0746d2u;r[1]=r[14];goto L8c0746d4; /* mov r14,r1 */
L8c0746d4: *stop=0x8c0746d4u;r[0]=r[0]&255u;goto L8c0746d6; /* extu.b r0,r0 */
L8c0746d6: target=r[3];*pr=0x8c0746dau;*stop=0x8c0746d8u;r[2]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0746da; /* jsr @r3 */
L8c0746da: *stop=0x8c0746dcu;goto L8c0747b0; /* bra 0x8c0747b0 */
L8c074740: *stop=0x8c074740u;r[5]=r[14];goto L8c074742; /* mov r14,r5 */
L8c074742: *stop=0x8c074742u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748e0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074744; /* mov.l 0x8c0748e0,r2 */
L8c074744: *stop=0x8c074744u;r[4]=r[14];goto L8c074746; /* mov r14,r4 */
L8c074746: *stop=0x8c074746u;r[6]=0u;goto L8c074748; /* mov #0x0,r6 */
L8c074748: *stop=0x8c074748u;r[5]+=72u;goto L8c07474a; /* add #0x48,r5 */
L8c07474a: target=r[2];*pr=0x8c07474eu;*stop=0x8c07474cu;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07474e; /* jsr @r2 */
L8c07474e: *stop=0x8c07474eu;r[3]=44u;goto L8c074750; /* mov #0x2c,r3 */
L8c074750: *stop=0x8c074750u;r[3]<<=8;goto L8c074752; /* shll8 r3 */
L8c074752: *stop=0x8c074752u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[3]);goto L8c074754; /* cmp/gt r3,r0 */
L8c074754: take=!(*sr&1u);*stop=0x8c074756u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;if(take)goto L8c07477c;goto L8c074758; /* bf/s 0x8c07477c */
L8c074758: *stop=0x8c074758u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07475a; /* mov.l 0x8c0748e8,r3 */
L8c07475a: *stop=0x8c07475au;r[5]=r[14];goto L8c07475c; /* mov r14,r5 */
L8c07475c: *stop=0x8c07475cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748e4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c07475e; /* mov.l 0x8c0748e4,r4 */
L8c07475e: target=r[3];*pr=0x8c074762u;*stop=0x8c074760u;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074762; /* jsr @r3 */
L8c074762: *stop=0x8c074762u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748e4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c074764; /* mov.l 0x8c0748e4,r4 */
L8c074764: *stop=0x8c074764u;r[5]=r[14];goto L8c074766; /* mov r14,r5 */
L8c074766: *stop=0x8c074766u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074768; /* mov.l 0x8c0748e8,r2 */
L8c074768: *stop=0x8c074768u;fcopy(f,12u,0u);goto L8c07476a; /* fmov fr0,fr12 */
L8c07476a: target=r[2];*pr=0x8c07476eu;*stop=0x8c07476cu;r[5]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07476e; /* jsr @r2 */
L8c07476e: *stop=0x8c07476eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c074770; /* fcmp/gt fr0,fr12 */
L8c074770: if((*sr&1u))goto L8c07477c;goto L8c074772; /* bt 0x8c07477c */
L8c074772: *stop=0x8c074772u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c074774; /* mov.l @r11,r2 */
L8c074774: *stop=0x8c074774u;r[3]=64u;goto L8c074776; /* mov #0x40,r3 */
L8c074776: *stop=0x8c074776u;r[3]<<=8;goto L8c074778; /* shll8 r3 */
L8c074778: *stop=0x8c074778u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c07477a; /* tst r3,r2 */
L8c07477a: if((*sr&1u))goto L8c0747a4;goto L8c07477c; /* bt 0x8c0747a4 */
L8c07477c: *stop=0x8c07477cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07477e; /* mov.l 0x8c0748e8,r3 */
L8c07477e: *stop=0x8c07477eu;r[5]=r[14];goto L8c074780; /* mov r14,r5 */
L8c074780: *stop=0x8c074780u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748e4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c074782; /* mov.l 0x8c0748e4,r4 */
L8c074782: target=r[3];*pr=0x8c074786u;*stop=0x8c074784u;r[5]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074786; /* jsr @r3 */
L8c074786: *stop=0x8c074786u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074788; /* fldi0 fr3 */
L8c074788: *stop=0x8c074788u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c07478a; /* fcmp/gt fr3,fr0 */
L8c07478a: if(!(*sr&1u))goto L8c074792;goto L8c07478c; /* bf 0x8c074792 */
L8c07478c: *stop=0x8c07478cu;if(!valid_access(r[13]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x10u,2);if(io_status)return io_status;goto L8c07478e; /* mov.w @(0x10,r13),r0 */
L8c07478e: *stop=0x8c07478eu;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c074790; /* tst #0x3,r0 */
L8c074790: if((*sr&1u))goto L8c0747a4;goto L8c074792; /* bt 0x8c0747a4 */
L8c074792: *stop=0x8c074792u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[10]);if(io_status)return io_status;goto L8c074794; /* fmov.s @r10,fr3 */
L8c074794: *stop=0x8c074794u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c074796; /* fmov.s fr3,@r14 */
L8c074796: *stop=0x8c074796u;if(!valid_access(r[10]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074798; /* mov.l @(0x10,r10),r3 */
L8c074798: *stop=0x8c074798u;if(!valid_access(r[10]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07479a; /* mov.l @(0x14,r10),r2 */
L8c07479a: *stop=0x8c07479au;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c07479c; /* mov.l r3,@(0x10,r14) */
L8c07479c: *stop=0x8c07479cu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[2],4);if(io_status)return io_status;goto L8c07479e; /* mov.l r2,@(0x14,r14) */
L8c07479e: *stop=0x8c07479eu;if(!valid_access(r[10]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0747a0; /* mov.l @(0x18,r10),r3 */
L8c0747a0: *stop=0x8c0747a2u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c0747b4; /* bra 0x8c0747b4 */
L8c0747a4: *stop=0x8c0747a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0747a6; /* mov.l 0x8c0748ec,r3 */
L8c0747a6: *stop=0x8c0747a6u;r[0]=4294967172u;goto L8c0747a8; /* mov #-0x7c,r0 */
L8c0747a8: *stop=0x8c0747a8u;r[1]=r[14];goto L8c0747aa; /* mov r14,r1 */
L8c0747aa: *stop=0x8c0747aau;r[0]=r[0]&255u;goto L8c0747ac; /* extu.b r0,r0 */
L8c0747ac: target=r[3];*pr=0x8c0747b0u;*stop=0x8c0747aeu;r[2]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0747b0; /* jsr @r3 */
L8c0747b0: *stop=0x8c0747b2u;r[0]=0u;goto L8c0747b6; /* bra 0x8c0747b6 */
L8c0747b4: *stop=0x8c0747b4u;r[0]=1u;goto L8c0747b6; /* mov #0x1,r0 */
L8c0747b6: *stop=0x8c0747b6u;r[1]=4294967212u;goto L8c0747b8; /* mov #-0x54,r1 */
L8c0747b8: *stop=0x8c0747b8u;r[1]=r[1]&255u;goto L8c0747ba; /* extu.b r1,r1 */
L8c0747ba: *stop=0x8c0747bau;r[15]+=r[1];goto L8c0747bc; /* add r1,r15 */
L8c0747bc: *stop=0x8c0747bcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0747be; /* lds.l @r15+,PR */
L8c0747be: *stop=0x8c0747beu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0747c0; /* fmov.s @r15+,fr12 */
L8c0747c0: *stop=0x8c0747c0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0747c2; /* fmov.s @r15+,fr14 */
L8c0747c2: *stop=0x8c0747c2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0747c4; /* fmov.s @r15+,fr15 */
L8c0747c4: *stop=0x8c0747c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c0747c6; /* mov.l @r15+,r9 */
L8c0747c6: *stop=0x8c0747c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c0747c8; /* mov.l @r15+,r10 */
L8c0747c8: *stop=0x8c0747c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c0747ca; /* mov.l @r15+,r11 */
L8c0747ca: *stop=0x8c0747cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c0747cc; /* mov.l @r15+,r12 */
L8c0747cc: *stop=0x8c0747ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0747ce; /* mov.l @r15+,r13 */
L8c0747ce: *stop=0x8c0747ceu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0747d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_model_response_8c0742d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c074314(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074314u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c074348(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074348u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c074362(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074362u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0743ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0743ceu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c07440c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07440cu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c07441e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07441eu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c074582(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074582u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0745a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0745a8u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0745b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0745b4u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0745bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0745bcu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c074672(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074672u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c074684(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074684u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c074696(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074696u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0746a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0746a6u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0746ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0746aeu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0746ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0746bau);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0746c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0746c0u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0746ca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0746cau);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0746da(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0746dau);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c07474e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07474eu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c074762(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074762u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c07476e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07476eu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c074786(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074786u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c0747b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c0742d4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0747b0u);}

static int sa_movement_model_response_run_8c036f12(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c036f1au)goto L8c036f1a;if(resume_pc==0x8c036f1eu)goto L8c036f1e;goto L8c036f12;
L8c036f12: *stop=0x8c036f12u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c036f14; /* sts.l PR,@-r15 */
L8c036f14: *stop=0x8c036f14u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036f44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c036f16; /* mov.l 0x8c036f44,r3 */
L8c036f16: target=r[3];*pr=0x8c036f1au;*stop=0x8c036f18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036f1a; /* jsr @r3 */
L8c036f1a: target=0x8c036bc0u;*pr=0x8c036f1eu;*stop=0x8c036f1cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036f1e; /* bsr 0x8c036bc0 */
L8c036f1e: *stop=0x8c036f1eu;r[15]+=4u;goto L8c036f20; /* add #0x4,r15 */
L8c036f20: *stop=0x8c036f20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036f48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036f22; /* mov.l 0x8c036f48,r2 */
L8c036f22: target=r[2];*stop=0x8c036f24u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}
SA_STAGE_EXPORT int sa_movement_model_response_8c036f12(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036f12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036f1a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036f12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036f1au);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036f1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036f12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036f1eu);}

static int sa_movement_model_response_run_8c036bc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c036c26u)goto L8c036c26;if(resume_pc==0x8c036c34u)goto L8c036c34;if(resume_pc==0x8c036c3cu)goto L8c036c3c;if(resume_pc==0x8c036c54u)goto L8c036c54;if(resume_pc==0x8c036c6eu)goto L8c036c6e;if(resume_pc==0x8c036c86u)goto L8c036c86;if(resume_pc==0x8c036c9eu)goto L8c036c9e;if(resume_pc==0x8c036ca8u)goto L8c036ca8;if(resume_pc==0x8c036cb6u)goto L8c036cb6;if(resume_pc==0x8c036cbeu)goto L8c036cbe;if(resume_pc==0x8c036cd8u)goto L8c036cd8;if(resume_pc==0x8c036cf0u)goto L8c036cf0;if(resume_pc==0x8c036d08u)goto L8c036d08;if(resume_pc==0x8c036d12u)goto L8c036d12;if(resume_pc==0x8c036d20u)goto L8c036d20;if(resume_pc==0x8c036d64u)goto L8c036d64;if(resume_pc==0x8c036d7cu)goto L8c036d7c;if(resume_pc==0x8c036d94u)goto L8c036d94;if(resume_pc==0x8c036d9eu)goto L8c036d9e;if(resume_pc==0x8c036dacu)goto L8c036dac;if(resume_pc==0x8c036db4u)goto L8c036db4;if(resume_pc==0x8c036dccu)goto L8c036dcc;if(resume_pc==0x8c036dd6u)goto L8c036dd6;if(resume_pc==0x8c036de4u)goto L8c036de4;if(resume_pc==0x8c036decu)goto L8c036dec;if(resume_pc==0x8c036e04u)goto L8c036e04;if(resume_pc==0x8c036e1eu)goto L8c036e1e;if(resume_pc==0x8c036e36u)goto L8c036e36;if(resume_pc==0x8c036e40u)goto L8c036e40;if(resume_pc==0x8c036e4au)goto L8c036e4a;if(resume_pc==0x8c036e52u)goto L8c036e52;if(resume_pc==0x8c036e6cu)goto L8c036e6c;if(resume_pc==0x8c036e84u)goto L8c036e84;if(resume_pc==0x8c036e8eu)goto L8c036e8e;if(resume_pc==0x8c036e98u)goto L8c036e98;if(resume_pc==0x8c036ec4u)goto L8c036ec4;if(resume_pc==0x8c036edcu)goto L8c036edc;if(resume_pc==0x8c036ee6u)goto L8c036ee6;if(resume_pc==0x8c036ef0u)goto L8c036ef0;if(resume_pc==0x8c036ef4u)goto L8c036ef4;goto L8c036bc0;
L8c036bc0: *stop=0x8c036bc0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c036bc2; /* mov.l r14,@-r15 */
L8c036bc2: *stop=0x8c036bc2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c036bc4; /* mov.l r13,@-r15 */
L8c036bc4: *stop=0x8c036bc4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c036bc6; /* mov.l r12,@-r15 */
L8c036bc6: *stop=0x8c036bc6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c036bc8; /* mov.l r11,@-r15 */
L8c036bc8: *stop=0x8c036bc8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c036bca; /* mov.l r10,@-r15 */
L8c036bca: *stop=0x8c036bcau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c036bcc; /* mov.l r9,@-r15 */
L8c036bcc: *stop=0x8c036bccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c036bce; /* mov.l r8,@-r15 */
L8c036bce: *stop=0x8c036bceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c036bd0; /* sts.l PR,@-r15 */
L8c036bd0: *stop=0x8c036bd0u;r[15]+=4294967292u;goto L8c036bd2; /* add #-0x4,r15 */
L8c036bd2: *stop=0x8c036bd2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036d40u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c036bd4; /* mov.l 0x8c036d40,r10 */
L8c036bd4: *stop=0x8c036bd4u;r[14]=r[4];goto L8c036bd6; /* mov r4,r14 */
L8c036bd6: *stop=0x8c036bd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036d44u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c036bd8; /* mov.l 0x8c036d44,r11 */
L8c036bd8: *stop=0x8c036bd8u;r[13]=16u;goto L8c036bda; /* mov #0x10,r13 */
L8c036bda: *stop=0x8c036bdau;{uint32_t loaded=rd(m,bus,&io_status,0x8c036d48u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c036bdc; /* mov.l 0x8c036d48,r12 */
L8c036bdc: *stop=0x8c036bdcu;r[9]=32u;goto L8c036bde; /* mov #0x20,r9 */
L8c036bde: *stop=0x8c036bdeu;r[8]=8u;goto L8c036be0; /* mov #0x8,r8 */
L8c036be0: *stop=0x8c036be0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c036be2; /* mov.l @r14,r0 */
L8c036be2: *stop=0x8c036be2u;r[0]&=0x7u;goto L8c036be4; /* and #0x7,r0 */
L8c036be4: *stop=0x8c036be4u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c036be6; /* cmp/eq #0x7,r0 */
L8c036be6: if((*sr&1u))goto L8c036c1c;goto L8c036be8; /* bt 0x8c036c1c */
L8c036be8: *stop=0x8c036be8u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c036bea; /* cmp/eq #0x0,r0 */
L8c036bea: if((*sr&1u))goto L8c036c38;goto L8c036bec; /* bt 0x8c036c38 */
L8c036bec: *stop=0x8c036becu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c036bee; /* cmp/eq #0x1,r0 */
L8c036bee: if((*sr&1u))goto L8c036cba;goto L8c036bf0; /* bt 0x8c036cba */
L8c036bf0: *stop=0x8c036bf0u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c036bf2; /* cmp/eq #0x2,r0 */
L8c036bf2: if(!(*sr&1u))goto L8c036bf8;goto L8c036bf4; /* bf 0x8c036bf8 */
L8c036bf4: *stop=0x8c036bf6u;goto L8c036d60; /* bra 0x8c036d60 */
L8c036bf8: *stop=0x8c036bf8u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c036bfa; /* cmp/eq #0x3,r0 */
L8c036bfa: if(!(*sr&1u))goto L8c036c00;goto L8c036bfc; /* bf 0x8c036c00 */
L8c036bfc: *stop=0x8c036bfeu;goto L8c036db0; /* bra 0x8c036db0 */
L8c036c00: *stop=0x8c036c00u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c036c02; /* cmp/eq #0x4,r0 */
L8c036c02: if(!(*sr&1u))goto L8c036c08;goto L8c036c04; /* bf 0x8c036c08 */
L8c036c04: *stop=0x8c036c06u;goto L8c036de8; /* bra 0x8c036de8 */
L8c036c08: *stop=0x8c036c08u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c036c0a; /* cmp/eq #0x5,r0 */
L8c036c0a: if(!(*sr&1u))goto L8c036c10;goto L8c036c0c; /* bf 0x8c036c10 */
L8c036c0c: *stop=0x8c036c0eu;goto L8c036e4e; /* bra 0x8c036e4e */
L8c036c10: *stop=0x8c036c10u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c036c12; /* cmp/eq #0x6,r0 */
L8c036c12: if(!(*sr&1u))goto L8c036c18;goto L8c036c14; /* bf 0x8c036c18 */
L8c036c14: *stop=0x8c036c16u;goto L8c036ec0; /* bra 0x8c036ec0 */
L8c036c18: *stop=0x8c036c1au;goto L8c036ef4; /* bra 0x8c036ef4 */
L8c036c1c: *stop=0x8c036c1cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036c1e; /* mov.l @r14,r3 */
L8c036c1e: *stop=0x8c036c1eu;*sr=(*sr&~1u)|!(r[8]&r[3]);goto L8c036c20; /* tst r8,r3 */
L8c036c20: if(!(*sr&1u))goto L8c036c26;goto L8c036c22; /* bf 0x8c036c26 */
L8c036c22: target=r[12];*pr=0x8c036c26u;*stop=0x8c036c24u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036c26; /* jsr @r12 */
L8c036c26: *stop=0x8c036c26u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036c28; /* mov.l @r14,r3 */
L8c036c28: *stop=0x8c036c28u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c036c2a; /* tst r13,r3 */
L8c036c2a: if((*sr&1u))goto L8c036c30;goto L8c036c2c; /* bt 0x8c036c30 */
L8c036c2c: *stop=0x8c036c2eu;goto L8c036ef4; /* bra 0x8c036ef4 */
L8c036c30: target=0x8c036bc0u;*pr=0x8c036c34u;*stop=0x8c036c32u;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036c34; /* bsr 0x8c036bc0 */
L8c036c34: *stop=0x8c036c36u;goto L8c036ef4; /* bra 0x8c036ef4 */
L8c036c38: target=r[11];*pr=0x8c036c3cu;*stop=0x8c036c3au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036c3c; /* jsr @r11 */
L8c036c3c: *stop=0x8c036c3cu;r[3]=r[14];goto L8c036c3e; /* mov r14,r3 */
L8c036c3e: *stop=0x8c036c3eu;r[0]=8u;goto L8c036c40; /* mov #0x8,r0 */
L8c036c40: *stop=0x8c036c40u;r[3]+=8u;goto L8c036c42; /* add #0x8,r3 */
L8c036c42: *stop=0x8c036c42u;r[2]=r[3];goto L8c036c44; /* mov r3,r2 */
L8c036c44: *stop=0x8c036c44u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c036c46; /* mov.l r3,@r15 */
L8c036c46: *stop=0x8c036c46u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[3]);if(io_status)return io_status;goto L8c036c48; /* fmov.s @(r0,r3),fr6 */
L8c036c48: *stop=0x8c036c48u;r[0]=4u;goto L8c036c4a; /* mov #0x4,r0 */
L8c036c4a: *stop=0x8c036c4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c036d4cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c036c4c; /* mov.l 0x8c036d4c,r3 */
L8c036c4c: *stop=0x8c036c4cu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c036c4e; /* fmov.s @r2,fr4 */
L8c036c4e: *stop=0x8c036c4eu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c036c50; /* fmov.s @(r0,r2),fr5 */
L8c036c50: target=r[3];*pr=0x8c036c54u;*stop=0x8c036c52u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036c54; /* jsr @r3 */
L8c036c54: *stop=0x8c036c54u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c036c56; /* mov.l @r14,r2 */
L8c036c56: *stop=0x8c036c56u;*sr=(*sr&~1u)|!(r[9]&r[2]);goto L8c036c58; /* tst r9,r2 */
L8c036c58: if((*sr&1u))goto L8c036c72;goto L8c036c5a; /* bt 0x8c036c72 */
L8c036c5a: *stop=0x8c036c5au;r[7]=r[14];goto L8c036c5c; /* mov r14,r7 */
L8c036c5c: *stop=0x8c036c5cu;r[7]+=20u;goto L8c036c5e; /* add #0x14,r7 */
L8c036c5e: *stop=0x8c036c5eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c036c60; /* mov.l r7,@r15 */
L8c036c60: *stop=0x8c036c60u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036c62; /* mov.l @r15,r3 */
L8c036c62: *stop=0x8c036c62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036d50u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036c64; /* mov.l 0x8c036d50,r2 */
L8c036c64: *stop=0x8c036c64u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c036c66; /* mov.l @r3,r5 */
L8c036c66: *stop=0x8c036c66u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c036c68; /* mov.l @(0x8,r7),r7 */
L8c036c68: *stop=0x8c036c68u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c036c6a; /* mov.l @(0x4,r3),r6 */
L8c036c6a: target=r[2];*pr=0x8c036c6eu;*stop=0x8c036c6cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036c6e; /* jsr @r2 */
L8c036c6e: *stop=0x8c036c70u;goto L8c036c86; /* bra 0x8c036c86 */
L8c036c72: *stop=0x8c036c72u;r[7]=r[14];goto L8c036c74; /* mov r14,r7 */
L8c036c74: *stop=0x8c036c74u;r[7]+=20u;goto L8c036c76; /* add #0x14,r7 */
L8c036c76: *stop=0x8c036c76u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c036c78; /* mov.l r7,@r15 */
L8c036c78: *stop=0x8c036c78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036c7a; /* mov.l @r15,r3 */
L8c036c7a: *stop=0x8c036c7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c036d54u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036c7c; /* mov.l 0x8c036d54,r2 */
L8c036c7c: *stop=0x8c036c7cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c036c7e; /* mov.l @r3,r5 */
L8c036c7e: *stop=0x8c036c7eu;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c036c80; /* mov.l @(0x8,r7),r7 */
L8c036c80: *stop=0x8c036c80u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c036c82; /* mov.l @(0x4,r3),r6 */
L8c036c82: target=r[2];*pr=0x8c036c86u;*stop=0x8c036c84u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036c86; /* jsr @r2 */
L8c036c86: *stop=0x8c036c86u;r[3]=r[14];goto L8c036c88; /* mov r14,r3 */
L8c036c88: *stop=0x8c036c88u;r[0]=8u;goto L8c036c8a; /* mov #0x8,r0 */
L8c036c8a: *stop=0x8c036c8au;r[3]+=32u;goto L8c036c8c; /* add #0x20,r3 */
L8c036c8c: *stop=0x8c036c8cu;r[2]=r[3];goto L8c036c8e; /* mov r3,r2 */
L8c036c8e: *stop=0x8c036c8eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c036c90; /* mov.l r3,@r15 */
L8c036c90: *stop=0x8c036c90u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[3]);if(io_status)return io_status;goto L8c036c92; /* fmov.s @(r0,r3),fr6 */
L8c036c92: *stop=0x8c036c92u;r[0]=4u;goto L8c036c94; /* mov #0x4,r0 */
L8c036c94: *stop=0x8c036c94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036d58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c036c96; /* mov.l 0x8c036d58,r3 */
L8c036c96: *stop=0x8c036c96u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c036c98; /* fmov.s @r2,fr4 */
L8c036c98: *stop=0x8c036c98u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c036c9a; /* fmov.s @(r0,r2),fr5 */
L8c036c9a: target=r[3];*pr=0x8c036c9eu;*stop=0x8c036c9cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036c9e; /* jsr @r3 */
L8c036c9e: *stop=0x8c036c9eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c036ca0; /* mov.l @r14,r0 */
L8c036ca0: *stop=0x8c036ca0u;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c036ca2; /* tst #0x8,r0 */
L8c036ca2: if(!(*sr&1u))goto L8c036ca8;goto L8c036ca4; /* bf 0x8c036ca8 */
L8c036ca4: target=r[12];*pr=0x8c036ca8u;*stop=0x8c036ca6u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036ca8; /* jsr @r12 */
L8c036ca8: *stop=0x8c036ca8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036caa; /* mov.l @r14,r3 */
L8c036caa: *stop=0x8c036caau;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c036cac; /* tst r13,r3 */
L8c036cac: if((*sr&1u))goto L8c036cb2;goto L8c036cae; /* bt 0x8c036cb2 */
L8c036cae: *stop=0x8c036cb0u;goto L8c036ef0; /* bra 0x8c036ef0 */
L8c036cb2: target=0x8c036bc0u;*pr=0x8c036cb6u;*stop=0x8c036cb4u;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036cb6; /* bsr 0x8c036bc0 */
L8c036cb6: *stop=0x8c036cb8u;goto L8c036ef0; /* bra 0x8c036ef0 */
L8c036cba: target=r[11];*pr=0x8c036cbeu;*stop=0x8c036cbcu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036cbe; /* jsr @r11 */
L8c036cbe: *stop=0x8c036cbeu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036cc0; /* mov.l @r14,r3 */
L8c036cc0: *stop=0x8c036cc0u;*sr=(*sr&~1u)|!(r[9]&r[3]);goto L8c036cc2; /* tst r9,r3 */
L8c036cc2: if((*sr&1u))goto L8c036cdc;goto L8c036cc4; /* bt 0x8c036cdc */
L8c036cc4: *stop=0x8c036cc4u;r[7]=r[14];goto L8c036cc6; /* mov r14,r7 */
L8c036cc6: *stop=0x8c036cc6u;r[7]+=20u;goto L8c036cc8; /* add #0x14,r7 */
L8c036cc8: *stop=0x8c036cc8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c036cca; /* mov.l r7,@r15 */
L8c036cca: *stop=0x8c036ccau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036ccc; /* mov.l @r15,r3 */
L8c036ccc: *stop=0x8c036cccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c036d50u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036cce; /* mov.l 0x8c036d50,r2 */
L8c036cce: *stop=0x8c036cceu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c036cd0; /* mov.l @r3,r5 */
L8c036cd0: *stop=0x8c036cd0u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c036cd2; /* mov.l @(0x8,r7),r7 */
L8c036cd2: *stop=0x8c036cd2u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c036cd4; /* mov.l @(0x4,r3),r6 */
L8c036cd4: target=r[2];*pr=0x8c036cd8u;*stop=0x8c036cd6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036cd8; /* jsr @r2 */
L8c036cd8: *stop=0x8c036cdau;goto L8c036cf0; /* bra 0x8c036cf0 */
L8c036cdc: *stop=0x8c036cdcu;r[7]=r[14];goto L8c036cde; /* mov r14,r7 */
L8c036cde: *stop=0x8c036cdeu;r[7]+=20u;goto L8c036ce0; /* add #0x14,r7 */
L8c036ce0: *stop=0x8c036ce0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c036ce2; /* mov.l r7,@r15 */
L8c036ce2: *stop=0x8c036ce2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036ce4; /* mov.l @r15,r3 */
L8c036ce4: *stop=0x8c036ce4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036d54u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036ce6; /* mov.l 0x8c036d54,r2 */
L8c036ce6: *stop=0x8c036ce6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c036ce8; /* mov.l @r3,r5 */
L8c036ce8: *stop=0x8c036ce8u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c036cea; /* mov.l @(0x8,r7),r7 */
L8c036cea: *stop=0x8c036ceau;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c036cec; /* mov.l @(0x4,r3),r6 */
L8c036cec: target=r[2];*pr=0x8c036cf0u;*stop=0x8c036ceeu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036cf0; /* jsr @r2 */
L8c036cf0: *stop=0x8c036cf0u;r[3]=r[14];goto L8c036cf2; /* mov r14,r3 */
L8c036cf2: *stop=0x8c036cf2u;r[0]=8u;goto L8c036cf4; /* mov #0x8,r0 */
L8c036cf4: *stop=0x8c036cf4u;r[3]+=32u;goto L8c036cf6; /* add #0x20,r3 */
L8c036cf6: *stop=0x8c036cf6u;r[2]=r[3];goto L8c036cf8; /* mov r3,r2 */
L8c036cf8: *stop=0x8c036cf8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c036cfa; /* mov.l r3,@r15 */
L8c036cfa: *stop=0x8c036cfau;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[3]);if(io_status)return io_status;goto L8c036cfc; /* fmov.s @(r0,r3),fr6 */
L8c036cfc: *stop=0x8c036cfcu;r[0]=4u;goto L8c036cfe; /* mov #0x4,r0 */
L8c036cfe: *stop=0x8c036cfeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c036d58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c036d00; /* mov.l 0x8c036d58,r3 */
L8c036d00: *stop=0x8c036d00u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c036d02; /* fmov.s @r2,fr4 */
L8c036d02: *stop=0x8c036d02u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c036d04; /* fmov.s @(r0,r2),fr5 */
L8c036d04: target=r[3];*pr=0x8c036d08u;*stop=0x8c036d06u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036d08; /* jsr @r3 */
L8c036d08: *stop=0x8c036d08u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c036d0a; /* mov.l @r14,r0 */
L8c036d0a: *stop=0x8c036d0au;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c036d0c; /* tst #0x8,r0 */
L8c036d0c: if(!(*sr&1u))goto L8c036d12;goto L8c036d0e; /* bf 0x8c036d12 */
L8c036d0e: target=r[12];*pr=0x8c036d12u;*stop=0x8c036d10u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036d12; /* jsr @r12 */
L8c036d12: *stop=0x8c036d12u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036d14; /* mov.l @r14,r3 */
L8c036d14: *stop=0x8c036d14u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c036d16; /* tst r13,r3 */
L8c036d16: if((*sr&1u))goto L8c036d1c;goto L8c036d18; /* bt 0x8c036d1c */
L8c036d18: *stop=0x8c036d1au;goto L8c036ef0; /* bra 0x8c036ef0 */
L8c036d1c: target=0x8c036bc0u;*pr=0x8c036d20u;*stop=0x8c036d1eu;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036d20; /* bsr 0x8c036bc0 */
L8c036d20: *stop=0x8c036d22u;goto L8c036ef0; /* bra 0x8c036ef0 */
L8c036d60: target=r[11];*pr=0x8c036d64u;*stop=0x8c036d62u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036d64; /* jsr @r11 */
L8c036d64: *stop=0x8c036d64u;r[3]=r[14];goto L8c036d66; /* mov r14,r3 */
L8c036d66: *stop=0x8c036d66u;r[0]=8u;goto L8c036d68; /* mov #0x8,r0 */
L8c036d68: *stop=0x8c036d68u;r[3]+=8u;goto L8c036d6a; /* add #0x8,r3 */
L8c036d6a: *stop=0x8c036d6au;r[2]=r[3];goto L8c036d6c; /* mov r3,r2 */
L8c036d6c: *stop=0x8c036d6cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c036d6e; /* mov.l r3,@r15 */
L8c036d6e: *stop=0x8c036d6eu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[3]);if(io_status)return io_status;goto L8c036d70; /* fmov.s @(r0,r3),fr6 */
L8c036d70: *stop=0x8c036d70u;r[0]=4u;goto L8c036d72; /* mov #0x4,r0 */
L8c036d72: *stop=0x8c036d72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036ea0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c036d74; /* mov.l 0x8c036ea0,r3 */
L8c036d74: *stop=0x8c036d74u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c036d76; /* fmov.s @r2,fr4 */
L8c036d76: *stop=0x8c036d76u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c036d78; /* fmov.s @(r0,r2),fr5 */
L8c036d78: target=r[3];*pr=0x8c036d7cu;*stop=0x8c036d7au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036d7c; /* jsr @r3 */
L8c036d7c: *stop=0x8c036d7cu;r[2]=r[14];goto L8c036d7e; /* mov r14,r2 */
L8c036d7e: *stop=0x8c036d7eu;r[0]=8u;goto L8c036d80; /* mov #0x8,r0 */
L8c036d80: *stop=0x8c036d80u;r[2]+=32u;goto L8c036d82; /* add #0x20,r2 */
L8c036d82: *stop=0x8c036d82u;r[3]=r[2];goto L8c036d84; /* mov r2,r3 */
L8c036d84: *stop=0x8c036d84u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c036d86; /* mov.l r2,@r15 */
L8c036d86: *stop=0x8c036d86u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c036d88; /* fmov.s @(r0,r2),fr6 */
L8c036d88: *stop=0x8c036d88u;r[0]=4u;goto L8c036d8a; /* mov #0x4,r0 */
L8c036d8a: *stop=0x8c036d8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c036ea4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036d8c; /* mov.l 0x8c036ea4,r2 */
L8c036d8c: *stop=0x8c036d8cu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;goto L8c036d8e; /* fmov.s @r3,fr4 */
L8c036d8e: *stop=0x8c036d8eu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c036d90; /* fmov.s @(r0,r3),fr5 */
L8c036d90: target=r[2];*pr=0x8c036d94u;*stop=0x8c036d92u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036d94; /* jsr @r2 */
L8c036d94: *stop=0x8c036d94u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c036d96; /* mov.l @r14,r0 */
L8c036d96: *stop=0x8c036d96u;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c036d98; /* tst #0x8,r0 */
L8c036d98: if(!(*sr&1u))goto L8c036d9e;goto L8c036d9a; /* bf 0x8c036d9e */
L8c036d9a: target=r[12];*pr=0x8c036d9eu;*stop=0x8c036d9cu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036d9e; /* jsr @r12 */
L8c036d9e: *stop=0x8c036d9eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c036da0; /* mov.l @r14,r2 */
L8c036da0: *stop=0x8c036da0u;*sr=(*sr&~1u)|!(r[13]&r[2]);goto L8c036da2; /* tst r13,r2 */
L8c036da2: if((*sr&1u))goto L8c036da8;goto L8c036da4; /* bt 0x8c036da8 */
L8c036da4: *stop=0x8c036da6u;goto L8c036ef0; /* bra 0x8c036ef0 */
L8c036da8: target=0x8c036bc0u;*pr=0x8c036dacu;*stop=0x8c036daau;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036dac; /* bsr 0x8c036bc0 */
L8c036dac: *stop=0x8c036daeu;goto L8c036ef0; /* bra 0x8c036ef0 */
L8c036db0: target=r[11];*pr=0x8c036db4u;*stop=0x8c036db2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036db4; /* jsr @r11 */
L8c036db4: *stop=0x8c036db4u;r[2]=r[14];goto L8c036db6; /* mov r14,r2 */
L8c036db6: *stop=0x8c036db6u;r[0]=8u;goto L8c036db8; /* mov #0x8,r0 */
L8c036db8: *stop=0x8c036db8u;r[2]+=32u;goto L8c036dba; /* add #0x20,r2 */
L8c036dba: *stop=0x8c036dbau;r[3]=r[2];goto L8c036dbc; /* mov r2,r3 */
L8c036dbc: *stop=0x8c036dbcu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c036dbe; /* mov.l r2,@r15 */
L8c036dbe: *stop=0x8c036dbeu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c036dc0; /* fmov.s @(r0,r2),fr6 */
L8c036dc0: *stop=0x8c036dc0u;r[0]=4u;goto L8c036dc2; /* mov #0x4,r0 */
L8c036dc2: *stop=0x8c036dc2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036ea4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036dc4; /* mov.l 0x8c036ea4,r2 */
L8c036dc4: *stop=0x8c036dc4u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;goto L8c036dc6; /* fmov.s @r3,fr4 */
L8c036dc6: *stop=0x8c036dc6u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c036dc8; /* fmov.s @(r0,r3),fr5 */
L8c036dc8: target=r[2];*pr=0x8c036dccu;*stop=0x8c036dcau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036dcc; /* jsr @r2 */
L8c036dcc: *stop=0x8c036dccu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c036dce; /* mov.l @r14,r0 */
L8c036dce: *stop=0x8c036dceu;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c036dd0; /* tst #0x8,r0 */
L8c036dd0: if(!(*sr&1u))goto L8c036dd6;goto L8c036dd2; /* bf 0x8c036dd6 */
L8c036dd2: target=r[12];*pr=0x8c036dd6u;*stop=0x8c036dd4u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036dd6; /* jsr @r12 */
L8c036dd6: *stop=0x8c036dd6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c036dd8; /* mov.l @r14,r2 */
L8c036dd8: *stop=0x8c036dd8u;*sr=(*sr&~1u)|!(r[13]&r[2]);goto L8c036dda; /* tst r13,r2 */
L8c036dda: if((*sr&1u))goto L8c036de0;goto L8c036ddc; /* bt 0x8c036de0 */
L8c036ddc: *stop=0x8c036ddeu;goto L8c036ef0; /* bra 0x8c036ef0 */
L8c036de0: target=0x8c036bc0u;*pr=0x8c036de4u;*stop=0x8c036de2u;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036de4; /* bsr 0x8c036bc0 */
L8c036de4: *stop=0x8c036de6u;goto L8c036ef0; /* bra 0x8c036ef0 */
L8c036de8: target=r[11];*pr=0x8c036decu;*stop=0x8c036deau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036dec; /* jsr @r11 */
L8c036dec: *stop=0x8c036decu;r[2]=r[14];goto L8c036dee; /* mov r14,r2 */
L8c036dee: *stop=0x8c036deeu;r[0]=8u;goto L8c036df0; /* mov #0x8,r0 */
L8c036df0: *stop=0x8c036df0u;r[2]+=8u;goto L8c036df2; /* add #0x8,r2 */
L8c036df2: *stop=0x8c036df2u;r[3]=r[2];goto L8c036df4; /* mov r2,r3 */
L8c036df4: *stop=0x8c036df4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c036df6; /* mov.l r2,@r15 */
L8c036df6: *stop=0x8c036df6u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c036df8; /* fmov.s @(r0,r2),fr6 */
L8c036df8: *stop=0x8c036df8u;r[0]=4u;goto L8c036dfa; /* mov #0x4,r0 */
L8c036dfa: *stop=0x8c036dfau;{uint32_t loaded=rd(m,bus,&io_status,0x8c036ea0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036dfc; /* mov.l 0x8c036ea0,r2 */
L8c036dfc: *stop=0x8c036dfcu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;goto L8c036dfe; /* fmov.s @r3,fr4 */
L8c036dfe: *stop=0x8c036dfeu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c036e00; /* fmov.s @(r0,r3),fr5 */
L8c036e00: target=r[2];*pr=0x8c036e04u;*stop=0x8c036e02u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036e04; /* jsr @r2 */
L8c036e04: *stop=0x8c036e04u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036e06; /* mov.l @r14,r3 */
L8c036e06: *stop=0x8c036e06u;*sr=(*sr&~1u)|!(r[9]&r[3]);goto L8c036e08; /* tst r9,r3 */
L8c036e08: if((*sr&1u))goto L8c036e22;goto L8c036e0a; /* bt 0x8c036e22 */
L8c036e0a: *stop=0x8c036e0au;r[7]=r[14];goto L8c036e0c; /* mov r14,r7 */
L8c036e0c: *stop=0x8c036e0cu;r[7]+=20u;goto L8c036e0e; /* add #0x14,r7 */
L8c036e0e: *stop=0x8c036e0eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c036e10; /* mov.l r7,@r15 */
L8c036e10: *stop=0x8c036e10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036e12; /* mov.l @r15,r3 */
L8c036e12: *stop=0x8c036e12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036ea8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036e14; /* mov.l 0x8c036ea8,r2 */
L8c036e14: *stop=0x8c036e14u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c036e16; /* mov.l @r3,r5 */
L8c036e16: *stop=0x8c036e16u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c036e18; /* mov.l @(0x8,r7),r7 */
L8c036e18: *stop=0x8c036e18u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c036e1a; /* mov.l @(0x4,r3),r6 */
L8c036e1a: target=r[2];*pr=0x8c036e1eu;*stop=0x8c036e1cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036e1e; /* jsr @r2 */
L8c036e1e: *stop=0x8c036e20u;goto L8c036e36; /* bra 0x8c036e36 */
L8c036e22: *stop=0x8c036e22u;r[7]=r[14];goto L8c036e24; /* mov r14,r7 */
L8c036e24: *stop=0x8c036e24u;r[7]+=20u;goto L8c036e26; /* add #0x14,r7 */
L8c036e26: *stop=0x8c036e26u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c036e28; /* mov.l r7,@r15 */
L8c036e28: *stop=0x8c036e28u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036e2a; /* mov.l @r15,r3 */
L8c036e2a: *stop=0x8c036e2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c036eacu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036e2c; /* mov.l 0x8c036eac,r2 */
L8c036e2c: *stop=0x8c036e2cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c036e2e; /* mov.l @r3,r5 */
L8c036e2e: *stop=0x8c036e2eu;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c036e30; /* mov.l @(0x8,r7),r7 */
L8c036e30: *stop=0x8c036e30u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c036e32; /* mov.l @(0x4,r3),r6 */
L8c036e32: target=r[2];*pr=0x8c036e36u;*stop=0x8c036e34u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036e36; /* jsr @r2 */
L8c036e36: *stop=0x8c036e36u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036e38; /* mov.l @r14,r3 */
L8c036e38: *stop=0x8c036e38u;*sr=(*sr&~1u)|!(r[8]&r[3]);goto L8c036e3a; /* tst r8,r3 */
L8c036e3a: if(!(*sr&1u))goto L8c036e40;goto L8c036e3c; /* bf 0x8c036e40 */
L8c036e3c: target=r[12];*pr=0x8c036e40u;*stop=0x8c036e3eu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036e40; /* jsr @r12 */
L8c036e40: *stop=0x8c036e40u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036e42; /* mov.l @r14,r3 */
L8c036e42: *stop=0x8c036e42u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c036e44; /* tst r13,r3 */
L8c036e44: if(!(*sr&1u))goto L8c036ef0;goto L8c036e46; /* bf 0x8c036ef0 */
L8c036e46: target=0x8c036bc0u;*pr=0x8c036e4au;*stop=0x8c036e48u;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036e4a; /* bsr 0x8c036bc0 */
L8c036e4a: *stop=0x8c036e4cu;goto L8c036ef0; /* bra 0x8c036ef0 */
L8c036e4e: target=r[11];*pr=0x8c036e52u;*stop=0x8c036e50u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036e52; /* jsr @r11 */
L8c036e52: *stop=0x8c036e52u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036e54; /* mov.l @r14,r3 */
L8c036e54: *stop=0x8c036e54u;*sr=(*sr&~1u)|!(r[9]&r[3]);goto L8c036e56; /* tst r9,r3 */
L8c036e56: if((*sr&1u))goto L8c036e70;goto L8c036e58; /* bt 0x8c036e70 */
L8c036e58: *stop=0x8c036e58u;r[7]=r[14];goto L8c036e5a; /* mov r14,r7 */
L8c036e5a: *stop=0x8c036e5au;r[7]+=20u;goto L8c036e5c; /* add #0x14,r7 */
L8c036e5c: *stop=0x8c036e5cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c036e5e; /* mov.l r7,@r15 */
L8c036e5e: *stop=0x8c036e5eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036e60; /* mov.l @r15,r3 */
L8c036e60: *stop=0x8c036e60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036ea8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036e62; /* mov.l 0x8c036ea8,r2 */
L8c036e62: *stop=0x8c036e62u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c036e64; /* mov.l @r3,r5 */
L8c036e64: *stop=0x8c036e64u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c036e66; /* mov.l @(0x8,r7),r7 */
L8c036e66: *stop=0x8c036e66u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c036e68; /* mov.l @(0x4,r3),r6 */
L8c036e68: target=r[2];*pr=0x8c036e6cu;*stop=0x8c036e6au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036e6c; /* jsr @r2 */
L8c036e6c: *stop=0x8c036e6eu;goto L8c036e84; /* bra 0x8c036e84 */
L8c036e70: *stop=0x8c036e70u;r[7]=r[14];goto L8c036e72; /* mov r14,r7 */
L8c036e72: *stop=0x8c036e72u;r[7]+=20u;goto L8c036e74; /* add #0x14,r7 */
L8c036e74: *stop=0x8c036e74u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c036e76; /* mov.l r7,@r15 */
L8c036e76: *stop=0x8c036e76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036e78; /* mov.l @r15,r3 */
L8c036e78: *stop=0x8c036e78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036eacu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c036e7a; /* mov.l 0x8c036eac,r2 */
L8c036e7a: *stop=0x8c036e7au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c036e7c; /* mov.l @r3,r5 */
L8c036e7c: *stop=0x8c036e7cu;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c036e7e; /* mov.l @(0x8,r7),r7 */
L8c036e7e: *stop=0x8c036e7eu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c036e80; /* mov.l @(0x4,r3),r6 */
L8c036e80: target=r[2];*pr=0x8c036e84u;*stop=0x8c036e82u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036e84; /* jsr @r2 */
L8c036e84: *stop=0x8c036e84u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036e86; /* mov.l @r14,r3 */
L8c036e86: *stop=0x8c036e86u;*sr=(*sr&~1u)|!(r[8]&r[3]);goto L8c036e88; /* tst r8,r3 */
L8c036e88: if(!(*sr&1u))goto L8c036e8e;goto L8c036e8a; /* bf 0x8c036e8e */
L8c036e8a: target=r[12];*pr=0x8c036e8eu;*stop=0x8c036e8cu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036e8e; /* jsr @r12 */
L8c036e8e: *stop=0x8c036e8eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036e90; /* mov.l @r14,r3 */
L8c036e90: *stop=0x8c036e90u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c036e92; /* tst r13,r3 */
L8c036e92: if(!(*sr&1u))goto L8c036ef0;goto L8c036e94; /* bf 0x8c036ef0 */
L8c036e94: target=0x8c036bc0u;*pr=0x8c036e98u;*stop=0x8c036e96u;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036e98; /* bsr 0x8c036bc0 */
L8c036e98: *stop=0x8c036e9au;goto L8c036ef0; /* bra 0x8c036ef0 */
L8c036ec0: target=r[11];*pr=0x8c036ec4u;*stop=0x8c036ec2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036ec4; /* jsr @r11 */
L8c036ec4: *stop=0x8c036ec4u;r[3]=r[14];goto L8c036ec6; /* mov r14,r3 */
L8c036ec6: *stop=0x8c036ec6u;r[0]=8u;goto L8c036ec8; /* mov #0x8,r0 */
L8c036ec8: *stop=0x8c036ec8u;r[3]+=8u;goto L8c036eca; /* add #0x8,r3 */
L8c036eca: *stop=0x8c036ecau;r[2]=r[3];goto L8c036ecc; /* mov r3,r2 */
L8c036ecc: *stop=0x8c036eccu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c036ece; /* mov.l r3,@r15 */
L8c036ece: *stop=0x8c036eceu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[3]);if(io_status)return io_status;goto L8c036ed0; /* fmov.s @(r0,r3),fr6 */
L8c036ed0: *stop=0x8c036ed0u;r[0]=4u;goto L8c036ed2; /* mov #0x4,r0 */
L8c036ed2: *stop=0x8c036ed2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c036f40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c036ed4; /* mov.l 0x8c036f40,r3 */
L8c036ed4: *stop=0x8c036ed4u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c036ed6; /* fmov.s @r2,fr4 */
L8c036ed6: *stop=0x8c036ed6u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c036ed8; /* fmov.s @(r0,r2),fr5 */
L8c036ed8: target=r[3];*pr=0x8c036edcu;*stop=0x8c036edau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036edc; /* jsr @r3 */
L8c036edc: *stop=0x8c036edcu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c036ede; /* mov.l @r14,r0 */
L8c036ede: *stop=0x8c036edeu;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c036ee0; /* tst #0x8,r0 */
L8c036ee0: if(!(*sr&1u))goto L8c036ee6;goto L8c036ee2; /* bf 0x8c036ee6 */
L8c036ee2: target=r[12];*pr=0x8c036ee6u;*stop=0x8c036ee4u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036ee6; /* jsr @r12 */
L8c036ee6: *stop=0x8c036ee6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c036ee8; /* mov.l @r14,r3 */
L8c036ee8: *stop=0x8c036ee8u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c036eea; /* tst r13,r3 */
L8c036eea: if(!(*sr&1u))goto L8c036ef0;goto L8c036eec; /* bf 0x8c036ef0 */
L8c036eec: target=0x8c036bc0u;*pr=0x8c036ef0u;*stop=0x8c036eeeu;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036ef0; /* bsr 0x8c036bc0 */
L8c036ef0: target=r[10];*pr=0x8c036ef4u;*stop=0x8c036ef2u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c036ef4; /* jsr @r10 */
L8c036ef4: *stop=0x8c036ef4u;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c036ef6; /* mov.l @(0x30,r14),r14 */
L8c036ef6: *stop=0x8c036ef6u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c036ef8; /* tst r14,r14 */
L8c036ef8: if((*sr&1u))goto L8c036efe;goto L8c036efa; /* bt 0x8c036efe */
L8c036efa: *stop=0x8c036efcu;goto L8c036be0; /* bra 0x8c036be0 */
L8c036efe: *stop=0x8c036efeu;r[15]+=4u;goto L8c036f00; /* add #0x4,r15 */
L8c036f00: *stop=0x8c036f00u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c036f02; /* lds.l @r15+,PR */
L8c036f02: *stop=0x8c036f02u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c036f04; /* mov.l @r15+,r8 */
L8c036f04: *stop=0x8c036f04u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c036f06; /* mov.l @r15+,r9 */
L8c036f06: *stop=0x8c036f06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c036f08; /* mov.l @r15+,r10 */
L8c036f08: *stop=0x8c036f08u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c036f0a; /* mov.l @r15+,r11 */
L8c036f0a: *stop=0x8c036f0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c036f0c; /* mov.l @r15+,r12 */
L8c036f0c: *stop=0x8c036f0cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c036f0e; /* mov.l @r15+,r13 */
L8c036f0e: *stop=0x8c036f0eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c036f10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_model_response_8c036bc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036c26(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036c26u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036c34(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036c34u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036c3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036c3cu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036c54(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036c54u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036c6e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036c6eu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036c86(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036c86u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036c9e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036c9eu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036ca8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036ca8u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036cb6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036cb6u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036cbe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036cbeu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036cd8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036cd8u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036cf0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036cf0u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036d08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036d08u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036d12(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036d12u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036d20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036d20u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036d64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036d64u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036d7c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036d7cu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036d94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036d94u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036d9e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036d9eu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036dac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036dacu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036db4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036db4u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036dcc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036dccu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036dd6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036dd6u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036de4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036de4u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036dec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036decu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036e04(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036e04u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036e1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036e1eu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036e36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036e36u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036e40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036e40u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036e4a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036e4au);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036e52(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036e52u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036e6c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036e6cu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036e84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036e84u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036e8e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036e8eu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036e98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036e98u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036ec4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036ec4u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036edc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036edcu);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036ee6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036ee6u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036ef0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036ef0u);}
SA_STAGE_EXPORT int sa_movement_model_response_resume_8c036ef4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_model_response_run_8c036bc0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c036ef4u);}
