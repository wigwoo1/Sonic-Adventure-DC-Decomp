/* Original complete texture linking and helper family.
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
static int sa_player_texture_link_run_8c64c374(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64c408u)goto L8c64c408;if(resume_pc==0x8c64c440u)goto L8c64c440;if(resume_pc==0x8c64c458u)goto L8c64c458;if(resume_pc==0x8c64c464u)goto L8c64c464;if(resume_pc==0x8c64c4bau)goto L8c64c4ba;if(resume_pc==0x8c64c50cu)goto L8c64c50c;if(resume_pc==0x8c64c524u)goto L8c64c524;if(resume_pc==0x8c64c53cu)goto L8c64c53c;if(resume_pc==0x8c64c54eu)goto L8c64c54e;if(resume_pc==0x8c64c5ccu)goto L8c64c5cc;if(resume_pc==0x8c64c628u)goto L8c64c628;if(resume_pc==0x8c64c644u)goto L8c64c644;if(resume_pc==0x8c64c65cu)goto L8c64c65c;if(resume_pc==0x8c64c66eu)goto L8c64c66e;if(resume_pc==0x8c64c698u)goto L8c64c698;if(resume_pc==0x8c64c6acu)goto L8c64c6ac;if(resume_pc==0x8c64c6beu)goto L8c64c6be;if(resume_pc==0x8c64c6e8u)goto L8c64c6e8;if(resume_pc==0x8c64c6eeu)goto L8c64c6ee;if(resume_pc==0x8c64c6feu)goto L8c64c6fe;if(resume_pc==0x8c64c710u)goto L8c64c710;if(resume_pc==0x8c64c71cu)goto L8c64c71c;if(resume_pc==0x8c64c766u)goto L8c64c766;if(resume_pc==0x8c64c78cu)goto L8c64c78c;if(resume_pc==0x8c64c7a6u)goto L8c64c7a6;if(resume_pc==0x8c64c7d0u)goto L8c64c7d0;if(resume_pc==0x8c64c7eeu)goto L8c64c7ee;if(resume_pc==0x8c64c804u)goto L8c64c804;if(resume_pc==0x8c64c8b8u)goto L8c64c8b8;if(resume_pc==0x8c64c8dcu)goto L8c64c8dc;if(resume_pc==0x8c64c8e8u)goto L8c64c8e8;if(resume_pc==0x8c64c90au)goto L8c64c90a;if(resume_pc==0x8c64c938u)goto L8c64c938;if(resume_pc==0x8c64c952u)goto L8c64c952;if(resume_pc==0x8c64c976u)goto L8c64c976;if(resume_pc==0x8c64c982u)goto L8c64c982;if(resume_pc==0x8c64ca34u)goto L8c64ca34;if(resume_pc==0x8c64ca58u)goto L8c64ca58;if(resume_pc==0x8c64ca64u)goto L8c64ca64;if(resume_pc==0x8c64ca94u)goto L8c64ca94;if(resume_pc==0x8c64cac2u)goto L8c64cac2;if(resume_pc==0x8c64cb1au)goto L8c64cb1a;if(resume_pc==0x8c64cb38u)goto L8c64cb38;if(resume_pc==0x8c64cb52u)goto L8c64cb52;if(resume_pc==0x8c64cb64u)goto L8c64cb64;if(resume_pc==0x8c64cb92u)goto L8c64cb92;if(resume_pc==0x8c64cbacu)goto L8c64cbac;if(resume_pc==0x8c64cbcau)goto L8c64cbca;if(resume_pc==0x8c64cc78u)goto L8c64cc78;if(resume_pc==0x8c64cca6u)goto L8c64cca6;if(resume_pc==0x8c64cceau)goto L8c64ccea;if(resume_pc==0x8c64cd0eu)goto L8c64cd0e;if(resume_pc==0x8c64cd3cu)goto L8c64cd3c;if(resume_pc==0x8c64cdb6u)goto L8c64cdb6;if(resume_pc==0x8c64cde4u)goto L8c64cde4;if(resume_pc==0x8c64ce12u)goto L8c64ce12;if(resume_pc==0x8c64ce40u)goto L8c64ce40;if(resume_pc==0x8c64ce80u)goto L8c64ce80;if(resume_pc==0x8c64ce86u)goto L8c64ce86;if(resume_pc==0x8c64ced4u)goto L8c64ced4;if(resume_pc==0x8c64cee2u)goto L8c64cee2;if(resume_pc==0x8c64cf00u)goto L8c64cf00;if(resume_pc==0x8c64cf32u)goto L8c64cf32;if(resume_pc==0x8c64cf4au)goto L8c64cf4a;if(resume_pc==0x8c64cf62u)goto L8c64cf62;if(resume_pc==0x8c64cf78u)goto L8c64cf78;if(resume_pc==0x8c64cfbau)goto L8c64cfba;if(resume_pc==0x8c64cfc8u)goto L8c64cfc8;if(resume_pc==0x8c64cfe6u)goto L8c64cfe6;if(resume_pc==0x8c64d00cu)goto L8c64d00c;if(resume_pc==0x8c64d024u)goto L8c64d024;if(resume_pc==0x8c64d03au)goto L8c64d03a;if(resume_pc==0x8c64d048u)goto L8c64d048;if(resume_pc==0x8c64d066u)goto L8c64d066;if(resume_pc==0x8c64d098u)goto L8c64d098;if(resume_pc==0x8c64d0a6u)goto L8c64d0a6;if(resume_pc==0x8c64d0c4u)goto L8c64d0c4;if(resume_pc==0x8c64d118u)goto L8c64d118;if(resume_pc==0x8c64d130u)goto L8c64d130;if(resume_pc==0x8c64d142u)goto L8c64d142;if(resume_pc==0x8c64d148u)goto L8c64d148;if(resume_pc==0x8c64d156u)goto L8c64d156;if(resume_pc==0x8c64d168u)goto L8c64d168;goto L8c64c374;
L8c64c374: *stop=0x8c64c374u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64c376; /* sts.l PR,@-r15 */
L8c64c376: *stop=0x8c64c376u;r[0]=sx(m,bus,&io_status,0x8c64c472u,2);if(io_status)return io_status;goto L8c64c378; /* mov.w 0x8c64c472,r0 */
L8c64c378: *stop=0x8c64c378u;r[15]+=r[0];goto L8c64c37a; /* add r0,r15 */
L8c64c37a: *stop=0x8c64c37au;r[3]=sx(m,bus,&io_status,0x8c64c474u,2);if(io_status)return io_status;goto L8c64c37c; /* mov.w 0x8c64c474,r3 */
L8c64c37c: *stop=0x8c64c37cu;r[2]=sx(m,bus,&io_status,0x8c64c476u,2);if(io_status)return io_status;goto L8c64c37e; /* mov.w 0x8c64c476,r2 */
L8c64c37e: *stop=0x8c64c37eu;r[3]+=r[15];goto L8c64c380; /* add r15,r3 */
L8c64c380: *stop=0x8c64c380u;r[0]=sx(m,bus,&io_status,0x8c64c474u,2);if(io_status)return io_status;goto L8c64c382; /* mov.w 0x8c64c474,r0 */
L8c64c382: *stop=0x8c64c382u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c64c384; /* mov.l r4,@r3 */
L8c64c384: *stop=0x8c64c384u;r[3]=0u;goto L8c64c386; /* mov #0x0,r3 */
L8c64c386: *stop=0x8c64c386u;r[2]+=r[15];goto L8c64c388; /* add r15,r2 */
L8c64c388: *stop=0x8c64c388u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c64c38a; /* mov.l r5,@r2 */
L8c64c38a: *stop=0x8c64c38au;r[2]=4294967295u;goto L8c64c38c; /* mov #-0x1,r2 */
L8c64c38c: *stop=0x8c64c38cu;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c64c38e; /* mov.l r3,@(0x18,r15) */
L8c64c38e: *stop=0x8c64c38eu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c64c390; /* mov.l r2,@(0x10,r15) */
L8c64c390: *stop=0x8c64c390u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c392; /* mov.l @(r0,r15),r3 */
L8c64c392: *stop=0x8c64c392u;r[0]=sx(m,bus,&io_status,0x8c64c476u,2);if(io_status)return io_status;goto L8c64c394; /* mov.w 0x8c64c476,r0 */
L8c64c394: *stop=0x8c64c394u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c396; /* mov.l @r3,r3 */
L8c64c396: *stop=0x8c64c396u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c398; /* mov.l @(r0,r15),r2 */
L8c64c398: *stop=0x8c64c398u;r[0]=sx(m,bus,&io_status,0x8c64c478u,2);if(io_status)return io_status;goto L8c64c39a; /* mov.w 0x8c64c478,r0 */
L8c64c39a: *stop=0x8c64c39au;r[1]=r[2];goto L8c64c39c; /* mov r2,r1 */
L8c64c39c: *stop=0x8c64c39cu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c64c39e; /* shll r2 */
L8c64c39e: *stop=0x8c64c39eu;r[2]+=r[1];goto L8c64c3a0; /* add r1,r2 */
L8c64c3a0: *stop=0x8c64c3a0u;r[2]<<=2;goto L8c64c3a2; /* shll2 r2 */
L8c64c3a2: *stop=0x8c64c3a2u;r[2]+=r[3];goto L8c64c3a4; /* add r3,r2 */
L8c64c3a4: *stop=0x8c64c3a4u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c64c3a6; /* mov.l r2,@(r0,r15) */
L8c64c3a6: *stop=0x8c64c3a6u;r[3]=r[2];goto L8c64c3a8; /* mov r2,r3 */
L8c64c3a8: *stop=0x8c64c3a8u;r[0]=sx(m,bus,&io_status,0x8c64c47au,2);if(io_status)return io_status;goto L8c64c3aa; /* mov.w 0x8c64c47a,r0 */
L8c64c3aa: *stop=0x8c64c3aau;r[3]+=8u;goto L8c64c3ac; /* add #0x8,r3 */
L8c64c3ac: *stop=0x8c64c3acu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c64c3ae; /* mov.l r3,@(r0,r15) */
L8c64c3ae: *stop=0x8c64c3aeu;r[0]=sx(m,bus,&io_status,0x8c64c478u,2);if(io_status)return io_status;goto L8c64c3b0; /* mov.w 0x8c64c478,r0 */
L8c64c3b0: *stop=0x8c64c3b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c48cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c3b2; /* mov.l 0x8c64c48c,r3 */
L8c64c3b2: *stop=0x8c64c3b2u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c3b4; /* mov.l @(r0,r15),r2 */
L8c64c3b4: *stop=0x8c64c3b4u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c3b6; /* mov.l @(0x4,r2),r1 */
L8c64c3b6: *stop=0x8c64c3b6u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c64c3b8; /* tst r3,r1 */
L8c64c3b8: if((*sr&1u))goto L8c64c4ac;goto L8c64c3ba; /* bt 0x8c64c4ac */
L8c64c3ba: *stop=0x8c64c3bau;r[2]=sx(m,bus,&io_status,0x8c64c476u,2);if(io_status)return io_status;goto L8c64c3bc; /* mov.w 0x8c64c476,r2 */
L8c64c3bc: *stop=0x8c64c3bcu;r[0]=0u;goto L8c64c3be; /* mov #0x0,r0 */
L8c64c3be: *stop=0x8c64c3beu;r[2]+=r[15];goto L8c64c3c0; /* add r15,r2 */
L8c64c3c0: *stop=0x8c64c3c0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c3c2; /* mov.l @r2,r2 */
L8c64c3c2: *stop=0x8c64c3c2u;*sr=(*sr&~1u)|(r[2]>r[0]);goto L8c64c3c4; /* cmp/hi r0,r2 */
L8c64c3c4: if(!(*sr&1u))goto L8c64c4ac;goto L8c64c3c6; /* bf 0x8c64c4ac */
L8c64c3c6: *stop=0x8c64c3c6u;r[0]=sx(m,bus,&io_status,0x8c64c474u,2);if(io_status)return io_status;goto L8c64c3c8; /* mov.w 0x8c64c474,r0 */
L8c64c3c8: *stop=0x8c64c3c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c494u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c3ca; /* mov.l 0x8c64c494,r3 */
L8c64c3ca: *stop=0x8c64c3cau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c3cc; /* mov.l @(r0,r15),r2 */
L8c64c3cc: *stop=0x8c64c3ccu;r[0]=sx(m,bus,&io_status,0x8c64c476u,2);if(io_status)return io_status;goto L8c64c3ce; /* mov.w 0x8c64c476,r0 */
L8c64c3ce: *stop=0x8c64c3ceu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c3d0; /* mov.l @r2,r2 */
L8c64c3d0: *stop=0x8c64c3d0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c3d2; /* mov.l @(r0,r15),r1 */
L8c64c3d2: *stop=0x8c64c3d2u;r[1]+=4294967295u;goto L8c64c3d4; /* add #-0x1,r1 */
L8c64c3d4: *stop=0x8c64c3d4u;r[0]=r[1];goto L8c64c3d6; /* mov r1,r0 */
L8c64c3d6: *stop=0x8c64c3d6u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c64c3d8; /* shll r1 */
L8c64c3d8: *stop=0x8c64c3d8u;r[1]+=r[0];goto L8c64c3da; /* add r0,r1 */
L8c64c3da: *stop=0x8c64c3dau;r[0]=sx(m,bus,&io_status,0x8c64c47cu,2);if(io_status)return io_status;goto L8c64c3dc; /* mov.w 0x8c64c47c,r0 */
L8c64c3dc: *stop=0x8c64c3dcu;r[1]<<=2;goto L8c64c3de; /* shll2 r1 */
L8c64c3de: *stop=0x8c64c3deu;r[1]+=r[2];goto L8c64c3e0; /* add r2,r1 */
L8c64c3e0: *stop=0x8c64c3e0u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c64c3e2; /* mov.l r1,@(r0,r15) */
L8c64c3e2: *stop=0x8c64c3e2u;r[2]=r[1];goto L8c64c3e4; /* mov r1,r2 */
L8c64c3e4: *stop=0x8c64c3e4u;r[0]=sx(m,bus,&io_status,0x8c64c47eu,2);if(io_status)return io_status;goto L8c64c3e6; /* mov.w 0x8c64c47e,r0 */
L8c64c3e6: *stop=0x8c64c3e6u;r[2]+=8u;goto L8c64c3e8; /* add #0x8,r2 */
L8c64c3e8: *stop=0x8c64c3e8u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c64c3ea; /* mov.l r2,@(r0,r15) */
L8c64c3ea: *stop=0x8c64c3eau;r[0]=sx(m,bus,&io_status,0x8c64c478u,2);if(io_status)return io_status;goto L8c64c3ec; /* mov.w 0x8c64c478,r0 */
L8c64c3ec: *stop=0x8c64c3ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c490u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c3ee; /* mov.l 0x8c64c490,r2 */
L8c64c3ee: *stop=0x8c64c3eeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c3f0; /* mov.l @(r0,r15),r1 */
L8c64c3f0: *stop=0x8c64c3f0u;if(!valid_access(r[1]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c3f2; /* mov.l @(0x8,r1),r1 */
L8c64c3f2: *stop=0x8c64c3f2u;r[1]&=r[2];goto L8c64c3f4; /* and r2,r1 */
L8c64c3f4: *stop=0x8c64c3f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c498u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c3f6; /* mov.l 0x8c64c498,r2 */
L8c64c3f6: *stop=0x8c64c3f6u;r[1]>>=16;goto L8c64c3f8; /* shlr16 r1 */
L8c64c3f8: *stop=0x8c64c3f8u;r[1]>>=8;goto L8c64c3fa; /* shlr8 r1 */
L8c64c3fa: *stop=0x8c64c3fau;r[1]>>=2;goto L8c64c3fc; /* shlr2 r1 */
L8c64c3fc: *stop=0x8c64c3fcu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c64c3fe; /* mov.l r1,@(0x10,r15) */
L8c64c3fe: *stop=0x8c64c3feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c49cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c400; /* mov.l 0x8c64c49c,r1 */
L8c64c400: *stop=0x8c64c400u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c402; /* mov.l @(0x14,r15),r6 */
L8c64c402: *stop=0x8c64c402u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c404; /* mov.l @r3,r5 */
L8c64c404: target=r[1];*pr=0x8c64c408u;*stop=0x8c64c406u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c408; /* jsr @r1 */
L8c64c408: *stop=0x8c64c408u;r[3]=r[0];goto L8c64c40a; /* mov r0,r3 */
L8c64c40a: *stop=0x8c64c40au;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[0],4);if(io_status)return io_status;goto L8c64c40c; /* mov.l r0,@(0x20,r15) */
L8c64c40c: *stop=0x8c64c40cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c498u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c40e; /* mov.l 0x8c64c498,r1 */
L8c64c40e: *stop=0x8c64c40eu;r[0]<<=2;goto L8c64c410; /* shll2 r0 */
L8c64c410: *stop=0x8c64c410u;r[0]<<=2;goto L8c64c412; /* shll2 r0 */
L8c64c412: *stop=0x8c64c412u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c414; /* mov.l @r1,r2 */
L8c64c414: *stop=0x8c64c414u;r[0]+=r[3];goto L8c64c416; /* add r3,r0 */
L8c64c416: *stop=0x8c64c416u;r[0]<<=2;goto L8c64c418; /* shll2 r0 */
L8c64c418: *stop=0x8c64c418u;r[0]+=r[2];goto L8c64c41a; /* add r2,r0 */
L8c64c41a: *stop=0x8c64c41au;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c41c; /* mov.l @(0x10,r0),r0 */
L8c64c41c: *stop=0x8c64c41cu;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c64c41e; /* tst #0x1,r0 */
L8c64c41e: if((*sr&1u))goto L8c64c428;goto L8c64c420; /* bt 0x8c64c428 */
L8c64c420: *stop=0x8c64c420u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c422; /* mov.l @(0x10,r15),r2 */
L8c64c422: *stop=0x8c64c422u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64c424; /* mov.l r2,@-r15 */
L8c64c424: *stop=0x8c64c426u;r[7]=3u;goto L8c64c42e; /* bra 0x8c64c42e */
L8c64c428: *stop=0x8c64c428u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c42a; /* mov.l @(0x10,r15),r3 */
L8c64c42a: *stop=0x8c64c42au;r[7]=2u;goto L8c64c42c; /* mov #0x2,r7 */
L8c64c42c: *stop=0x8c64c42cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64c42e; /* mov.l r3,@-r15 */
L8c64c42e: *stop=0x8c64c42eu;r[0]=sx(m,bus,&io_status,0x8c64c47au,2);if(io_status)return io_status;goto L8c64c430; /* mov.w 0x8c64c47a,r0 */
L8c64c430: *stop=0x8c64c430u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c4a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c432; /* mov.l 0x8c64c4a0,r3 */
L8c64c432: *stop=0x8c64c432u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c434; /* mov.l @(r0,r15),r5 */
L8c64c434: *stop=0x8c64c434u;r[0]=sx(m,bus,&io_status,0x8c64c476u,2);if(io_status)return io_status;goto L8c64c436; /* mov.w 0x8c64c476,r0 */
L8c64c436: *stop=0x8c64c436u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c438; /* mov.l @r5,r5 */
L8c64c438: *stop=0x8c64c438u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c43a; /* mov.l @(0x18,r15),r6 */
L8c64c43a: *stop=0x8c64c43au;r[5]+=20u;goto L8c64c43c; /* add #0x14,r5 */
L8c64c43c: target=r[3];*pr=0x8c64c440u;*stop=0x8c64c43eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c440; /* jsr @r3 */
L8c64c440: *stop=0x8c64c440u;r[15]+=4u;goto L8c64c442; /* add #0x4,r15 */
L8c64c442: *stop=0x8c64c442u;r[0]=sx(m,bus,&io_status,0x8c64c47au,2);if(io_status)return io_status;goto L8c64c444; /* mov.w 0x8c64c47a,r0 */
L8c64c444: *stop=0x8c64c444u;r[1]=0u;goto L8c64c446; /* mov #0x0,r1 */
L8c64c446: *stop=0x8c64c446u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c448; /* mov.l @(r0,r15),r2 */
L8c64c448: *stop=0x8c64c448u;r[0]=66u;goto L8c64c44a; /* mov #0x42,r0 */
L8c64c44a: *stop=0x8c64c44au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c44c; /* mov.l @r2,r3 */
L8c64c44c: *stop=0x8c64c44cu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64c44e; /* mov.w r1,@(r0,r3) */
L8c64c44e: *stop=0x8c64c44eu;r[0]=sx(m,bus,&io_status,0x8c64c47au,2);if(io_status)return io_status;goto L8c64c450; /* mov.w 0x8c64c47a,r0 */
L8c64c450: *stop=0x8c64c450u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c4a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c452; /* mov.l 0x8c64c4a4,r3 */
L8c64c452: *stop=0x8c64c452u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c454; /* mov.l @(r0,r15),r4 */
L8c64c454: target=r[3];*pr=0x8c64c458u;*stop=0x8c64c456u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c458; /* jsr @r3 */
L8c64c458: *stop=0x8c64c458u;r[0]=sx(m,bus,&io_status,0x8c64c47au,2);if(io_status)return io_status;goto L8c64c45a; /* mov.w 0x8c64c47a,r0 */
L8c64c45a: *stop=0x8c64c45au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c4a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c45c; /* mov.l 0x8c64c4a8,r3 */
L8c64c45c: *stop=0x8c64c45cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c45e; /* mov.l @(r0,r15),r4 */
L8c64c45e: *stop=0x8c64c45eu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c460; /* mov.l @(0x10,r15),r5 */
L8c64c460: target=r[3];*pr=0x8c64c464u;*stop=0x8c64c462u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c464; /* jsr @r3 */
L8c64c464: *stop=0x8c64c464u;r[1]=sx(m,bus,&io_status,0x8c64c480u,2);if(io_status)return io_status;goto L8c64c466; /* mov.w 0x8c64c480,r1 */
L8c64c466: *stop=0x8c64c466u;r[2]=1u;goto L8c64c468; /* mov #0x1,r2 */
L8c64c468: *stop=0x8c64c468u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[2],4);if(io_status)return io_status;goto L8c64c46a; /* mov.l r2,@(0x18,r15) */
L8c64c46a: *stop=0x8c64c46au;r[15]+=r[1];goto L8c64c46c; /* add r1,r15 */
L8c64c46c: *stop=0x8c64c46cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64c46e; /* lds.l @r15+,PR */
L8c64c46e: *stop=0x8c64c46eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64c470u;r[0]=r[2];if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c64c4ac: *stop=0x8c64c4acu;r[0]=sx(m,bus,&io_status,0x8c64c5d6u,2);if(io_status)return io_status;goto L8c64c4ae; /* mov.w 0x8c64c5d6,r0 */
L8c64c4ae: *stop=0x8c64c4aeu;r[4]=r[15];goto L8c64c4b0; /* mov r15,r4 */
L8c64c4b0: *stop=0x8c64c4b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c4b2; /* mov.l 0x8c64c5e0,r3 */
L8c64c4b2: *stop=0x8c64c4b2u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c4b4; /* mov.l @(r0,r15),r5 */
L8c64c4b4: *stop=0x8c64c4b4u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c4b6; /* mov.l @r5,r5 */
L8c64c4b6: target=r[3];*pr=0x8c64c4bau;*stop=0x8c64c4b8u;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c4ba; /* jsr @r3 */
L8c64c4ba: *stop=0x8c64c4bau;r[1]=sx(m,bus,&io_status,0x8c64c5d8u,2);if(io_status)return io_status;goto L8c64c4bc; /* mov.w 0x8c64c5d8,r1 */
L8c64c4bc: *stop=0x8c64c4bcu;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c64c4be; /* cmp/pl r0 */
L8c64c4be: *stop=0x8c64c4beu;r[1]+=r[15];goto L8c64c4c0; /* add r15,r1 */
L8c64c4c0: take=(*sr&1u);*stop=0x8c64c4c2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64c4c8;goto L8c64c4c4; /* bt/s 0x8c64c4c8 */
L8c64c4c4: *stop=0x8c64c4c6u;goto L8c64d14c; /* bra 0x8c64d14c */
L8c64c4c8: *stop=0x8c64c4c8u;r[0]=sx(m,bus,&io_status,0x8c64c5dau,2);if(io_status)return io_status;goto L8c64c4ca; /* mov.w 0x8c64c5da,r0 */
L8c64c4ca: *stop=0x8c64c4cau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c4cc; /* mov.l @(r0,r15),r2 */
L8c64c4cc: *stop=0x8c64c4ccu;r[0]=sx(m,bus,&io_status,0x8c64c5dcu,2);if(io_status)return io_status;goto L8c64c4ce; /* mov.w 0x8c64c5dc,r0 */
L8c64c4ce: *stop=0x8c64c4ceu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c4d0; /* mov.l @r2,r2 */
L8c64c4d0: *stop=0x8c64c4d0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c4d2; /* mov.l @(r0,r15),r3 */
L8c64c4d2: *stop=0x8c64c4d2u;r[1]=r[3];goto L8c64c4d4; /* mov r3,r1 */
L8c64c4d4: *stop=0x8c64c4d4u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c64c4d6; /* shll r3 */
L8c64c4d6: *stop=0x8c64c4d6u;r[3]+=r[1];goto L8c64c4d8; /* add r1,r3 */
L8c64c4d8: *stop=0x8c64c4d8u;r[3]<<=2;goto L8c64c4da; /* shll2 r3 */
L8c64c4da: *stop=0x8c64c4dau;r[3]+=r[2];goto L8c64c4dc; /* add r2,r3 */
L8c64c4dc: *stop=0x8c64c4dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c4de; /* mov.l 0x8c64c5e4,r2 */
L8c64c4de: *stop=0x8c64c4deu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c4e0; /* mov.l @(0x4,r3),r1 */
L8c64c4e0: *stop=0x8c64c4e0u;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c64c4e2; /* tst r2,r1 */
L8c64c4e2: if((*sr&1u))goto L8c64c570;goto L8c64c4e4; /* bt 0x8c64c570 */
L8c64c4e4: *stop=0x8c64c4e4u;r[1]=sx(m,bus,&io_status,0x8c64c5dcu,2);if(io_status)return io_status;goto L8c64c4e6; /* mov.w 0x8c64c5dc,r1 */
L8c64c4e6: *stop=0x8c64c4e6u;r[0]=sx(m,bus,&io_status,0x8c64c5dau,2);if(io_status)return io_status;goto L8c64c4e8; /* mov.w 0x8c64c5da,r0 */
L8c64c4e8: *stop=0x8c64c4e8u;r[1]+=r[15];goto L8c64c4ea; /* add r15,r1 */
L8c64c4ea: *stop=0x8c64c4eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c4ec; /* mov.l 0x8c64c5f0,r2 */
L8c64c4ec: *stop=0x8c64c4ecu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c4ee; /* mov.l @r1,r3 */
L8c64c4ee: *stop=0x8c64c4eeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c4f0; /* mov.l @(r0,r15),r0 */
L8c64c4f0: *stop=0x8c64c4f0u;r[1]=r[3];goto L8c64c4f2; /* mov r3,r1 */
L8c64c4f2: *stop=0x8c64c4f2u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c64c4f4; /* shll r3 */
L8c64c4f4: *stop=0x8c64c4f4u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c4f6; /* mov.l @r0,r0 */
L8c64c4f6: *stop=0x8c64c4f6u;r[3]+=r[1];goto L8c64c4f8; /* add r1,r3 */
L8c64c4f8: *stop=0x8c64c4f8u;r[3]<<=2;goto L8c64c4fa; /* shll2 r3 */
L8c64c4fa: *stop=0x8c64c4fau;r[3]+=r[0];goto L8c64c4fc; /* add r0,r3 */
L8c64c4fc: *stop=0x8c64c4fcu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c4fe; /* mov.l @(0x8,r3),r1 */
L8c64c4fe: *stop=0x8c64c4feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c500; /* mov.l 0x8c64c5e8,r3 */
L8c64c500: *stop=0x8c64c500u;r[6]=r[1];goto L8c64c502; /* mov r1,r6 */
L8c64c502: *stop=0x8c64c502u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[1],4);if(io_status)return io_status;goto L8c64c504; /* mov.l r1,@(0x14,r15) */
L8c64c504: *stop=0x8c64c504u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5ecu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c506; /* mov.l 0x8c64c5ec,r1 */
L8c64c506: *stop=0x8c64c506u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c508; /* mov.l @r3,r5 */
L8c64c508: target=r[2];*pr=0x8c64c50cu;*stop=0x8c64c50au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c50c; /* jsr @r2 */
L8c64c50c: *stop=0x8c64c50cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64c50e; /* cmp/pz r0 */
L8c64c50e: take=(*sr&1u);*stop=0x8c64c510u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[0],4);if(io_status)return io_status;if(take)goto L8c64c518;goto L8c64c512; /* bt/s 0x8c64c518 */
L8c64c512: *stop=0x8c64c512u;r[2]=2u;goto L8c64c514; /* mov #0x2,r2 */
L8c64c514: *stop=0x8c64c516u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c64c51c; /* bra 0x8c64c51c */
L8c64c518: *stop=0x8c64c518u;r[1]=1u;goto L8c64c51a; /* mov #0x1,r1 */
L8c64c51a: *stop=0x8c64c51au;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c64c51c; /* mov.l r1,@(0xc,r15) */
L8c64c51c: *stop=0x8c64c51cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c51e; /* mov.l 0x8c64c5f4,r3 */
L8c64c51e: *stop=0x8c64c51eu;r[4]=r[15];goto L8c64c520; /* mov r15,r4 */
L8c64c520: target=r[3];*pr=0x8c64c524u;*stop=0x8c64c522u;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c524; /* jsr @r3 */
L8c64c524: *stop=0x8c64c524u;r[1]=sx(m,bus,&io_status,0x8c64c5d8u,2);if(io_status)return io_status;goto L8c64c526; /* mov.w 0x8c64c5d8,r1 */
L8c64c526: *stop=0x8c64c526u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64c528; /* cmp/pz r0 */
L8c64c528: *stop=0x8c64c528u;r[1]+=r[15];goto L8c64c52a; /* add r15,r1 */
L8c64c52a: take=!(*sr&1u);*stop=0x8c64c52cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64c532;goto L8c64c52e; /* bf/s 0x8c64c532 */
L8c64c52e: *stop=0x8c64c530u;goto L8c64c814; /* bra 0x8c64c814 */
L8c64c532: *stop=0x8c64c532u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c600u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c534; /* mov.l 0x8c64c600,r2 */
L8c64c534: *stop=0x8c64c534u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5fcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c536; /* mov.l 0x8c64c5fc,r4 */
L8c64c536: *stop=0x8c64c536u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5f8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c538; /* mov.l 0x8c64c5f8,r5 */
L8c64c538: target=r[2];*pr=0x8c64c53cu;*stop=0x8c64c53au;r[6]=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c53c; /* jsr @r2 */
L8c64c53c: *stop=0x8c64c53cu;r[3]=4294967295u;goto L8c64c53e; /* mov #-0x1,r3 */
L8c64c53e: *stop=0x8c64c53eu;r[0]=sx(m,bus,&io_status,0x8c64c5dcu,2);if(io_status)return io_status;goto L8c64c540; /* mov.w 0x8c64c5dc,r0 */
L8c64c540: *stop=0x8c64c540u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64c542; /* mov.l r3,@-r15 */
L8c64c542: *stop=0x8c64c542u;r[7]=0u;goto L8c64c544; /* mov #0x0,r7 */
L8c64c544: *stop=0x8c64c544u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c608u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c546; /* mov.l 0x8c64c608,r2 */
L8c64c546: *stop=0x8c64c546u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c604u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c548; /* mov.l 0x8c64c604,r5 */
L8c64c548: *stop=0x8c64c548u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c54a; /* mov.l @(0x18,r15),r6 */
L8c64c54a: target=r[2];*pr=0x8c64c54eu;*stop=0x8c64c54cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c54e; /* jsr @r2 */
L8c64c54e: *stop=0x8c64c54eu;r[15]+=4u;goto L8c64c550; /* add #0x4,r15 */
L8c64c550: *stop=0x8c64c550u;r[0]=sx(m,bus,&io_status,0x8c64c5deu,2);if(io_status)return io_status;goto L8c64c552; /* mov.w 0x8c64c5de,r0 */
L8c64c552: *stop=0x8c64c552u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c554; /* mov.l @(r0,r15),r3 */
L8c64c554: *stop=0x8c64c554u;r[0]=sx(m,bus,&io_status,0x8c64c5d8u,2);if(io_status)return io_status;goto L8c64c556; /* mov.w 0x8c64c5d8,r0 */
L8c64c556: *stop=0x8c64c556u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c558; /* mov.l @r3,r2 */
L8c64c558: *stop=0x8c64c558u;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64c55a; /* mov.w @(r0,r15),r1 */
L8c64c55a: *stop=0x8c64c55au;r[0]=66u;goto L8c64c55c; /* mov #0x42,r0 */
L8c64c55c: *stop=0x8c64c55cu;r[1]=0u-r[1];goto L8c64c55e; /* neg r1,r1 */
L8c64c55e: *stop=0x8c64c55eu;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64c560; /* mov.w r1,@(r0,r2) */
L8c64c560: *stop=0x8c64c560u;r[1]=0u;goto L8c64c562; /* mov #0x0,r1 */
L8c64c562: *stop=0x8c64c562u;r[0]=sx(m,bus,&io_status,0x8c64c5deu,2);if(io_status)return io_status;goto L8c64c564; /* mov.w 0x8c64c5de,r0 */
L8c64c564: *stop=0x8c64c564u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c566; /* mov.l @(r0,r15),r3 */
L8c64c566: *stop=0x8c64c566u;r[0]=64u;goto L8c64c568; /* mov #0x40,r0 */
L8c64c568: *stop=0x8c64c568u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c56a; /* mov.l @r3,r2 */
L8c64c56a: *stop=0x8c64c56au;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64c56c; /* mov.w r1,@(r0,r2) */
L8c64c56c: *stop=0x8c64c56eu;goto L8c64d190; /* bra 0x8c64d190 */
L8c64c570: *stop=0x8c64c570u;r[0]=sx(m,bus,&io_status,0x8c64c5dau,2);if(io_status)return io_status;goto L8c64c572; /* mov.w 0x8c64c5da,r0 */
L8c64c572: *stop=0x8c64c572u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c574; /* mov.l @(r0,r15),r1 */
L8c64c574: *stop=0x8c64c574u;r[0]=sx(m,bus,&io_status,0x8c64c5dcu,2);if(io_status)return io_status;goto L8c64c576; /* mov.w 0x8c64c5dc,r0 */
L8c64c576: *stop=0x8c64c576u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c578; /* mov.l @r1,r1 */
L8c64c578: *stop=0x8c64c578u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c57a; /* mov.l @(r0,r15),r3 */
L8c64c57a: *stop=0x8c64c57au;r[0]=r[3];goto L8c64c57c; /* mov r3,r0 */
L8c64c57c: *stop=0x8c64c57cu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c64c57e; /* shll r3 */
L8c64c57e: *stop=0x8c64c57eu;r[3]+=r[0];goto L8c64c580; /* add r0,r3 */
L8c64c580: *stop=0x8c64c580u;r[3]<<=2;goto L8c64c582; /* shll2 r3 */
L8c64c582: *stop=0x8c64c582u;r[3]+=r[1];goto L8c64c584; /* add r1,r3 */
L8c64c584: *stop=0x8c64c584u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c60cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c586; /* mov.l 0x8c64c60c,r1 */
L8c64c586: *stop=0x8c64c586u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c588; /* mov.l @(0x4,r3),r0 */
L8c64c588: *stop=0x8c64c588u;*sr=(*sr&~1u)|!(r[1]&r[0]);goto L8c64c58a; /* tst r1,r0 */
L8c64c58a: if(!(*sr&1u))goto L8c64c590;goto L8c64c58c; /* bf 0x8c64c590 */
L8c64c58c: *stop=0x8c64c58eu;goto L8c64c690; /* bra 0x8c64c690 */
L8c64c590: *stop=0x8c64c590u;r[0]=sx(m,bus,&io_status,0x8c64c5dau,2);if(io_status)return io_status;goto L8c64c592; /* mov.w 0x8c64c5da,r0 */
L8c64c592: *stop=0x8c64c592u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c594; /* mov.l @(r0,r15),r3 */
L8c64c594: *stop=0x8c64c594u;r[0]=sx(m,bus,&io_status,0x8c64c5dcu,2);if(io_status)return io_status;goto L8c64c596; /* mov.w 0x8c64c5dc,r0 */
L8c64c596: *stop=0x8c64c596u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c598; /* mov.l @r3,r3 */
L8c64c598: *stop=0x8c64c598u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c59a; /* mov.l @(r0,r15),r0 */
L8c64c59a: *stop=0x8c64c59au;r[2]=r[0];goto L8c64c59c; /* mov r0,r2 */
L8c64c59c: *stop=0x8c64c59cu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c64c59e; /* shll r0 */
L8c64c59e: *stop=0x8c64c59eu;r[0]+=r[2];goto L8c64c5a0; /* add r2,r0 */
L8c64c5a0: *stop=0x8c64c5a0u;r[0]<<=2;goto L8c64c5a2; /* shll2 r0 */
L8c64c5a2: *stop=0x8c64c5a2u;r[0]+=r[3];goto L8c64c5a4; /* add r3,r0 */
L8c64c5a4: *stop=0x8c64c5a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c610u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c5a6; /* mov.l 0x8c64c610,r3 */
L8c64c5a6: *stop=0x8c64c5a6u;if(!valid_access(r[0]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c5a8; /* mov.l @(0x8,r0),r2 */
L8c64c5a8: *stop=0x8c64c5a8u;r[0]=sx(m,bus,&io_status,0x8c64c5d6u,2);if(io_status)return io_status;goto L8c64c5aa; /* mov.w 0x8c64c5d6,r0 */
L8c64c5aa: *stop=0x8c64c5aau;r[2]&=r[3];goto L8c64c5ac; /* and r3,r2 */
L8c64c5ac: *stop=0x8c64c5acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c5ae; /* mov.l 0x8c64c5e8,r3 */
L8c64c5ae: *stop=0x8c64c5aeu;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c64c5b0; /* mov.l r2,@(0x14,r15) */
L8c64c5b0: *stop=0x8c64c5b0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c5b2; /* mov.l @(r0,r15),r1 */
L8c64c5b2: *stop=0x8c64c5b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c614u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c5b4; /* mov.l 0x8c64c614,r2 */
L8c64c5b4: *stop=0x8c64c5b4u;if(!valid_access(r[1]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c5b6; /* mov.l @(0x8,r1),r1 */
L8c64c5b6: *stop=0x8c64c5b6u;r[1]&=r[2];goto L8c64c5b8; /* and r2,r1 */
L8c64c5b8: *stop=0x8c64c5b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c5ba; /* mov.l 0x8c64c5ec,r2 */
L8c64c5ba: *stop=0x8c64c5bau;r[1]>>=16;goto L8c64c5bc; /* shlr16 r1 */
L8c64c5bc: *stop=0x8c64c5bcu;r[1]>>=8;goto L8c64c5be; /* shlr8 r1 */
L8c64c5be: *stop=0x8c64c5beu;r[1]>>=2;goto L8c64c5c0; /* shlr2 r1 */
L8c64c5c0: *stop=0x8c64c5c0u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c64c5c2; /* mov.l r1,@(0x10,r15) */
L8c64c5c2: *stop=0x8c64c5c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c5f0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c5c4; /* mov.l 0x8c64c5f0,r1 */
L8c64c5c4: *stop=0x8c64c5c4u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c5c6; /* mov.l @(0x14,r15),r6 */
L8c64c5c6: *stop=0x8c64c5c6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c5c8; /* mov.l @r3,r5 */
L8c64c5c8: target=r[1];*pr=0x8c64c5ccu;*stop=0x8c64c5cau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c5cc; /* jsr @r1 */
L8c64c5cc: *stop=0x8c64c5ccu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64c5ce; /* cmp/pz r0 */
L8c64c5ce: take=(*sr&1u);*stop=0x8c64c5d0u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[0],4);if(io_status)return io_status;if(take)goto L8c64c618;goto L8c64c5d2; /* bt/s 0x8c64c618 */
L8c64c5d2: *stop=0x8c64c5d4u;r[2]=2u;goto L8c64c630; /* bra 0x8c64c630 */
L8c64c618: *stop=0x8c64c618u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c728u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c61a; /* mov.l 0x8c64c728,r3 */
L8c64c61a: *stop=0x8c64c61au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c72cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c61c; /* mov.l 0x8c64c72c,r2 */
L8c64c61c: *stop=0x8c64c61cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c730u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c61e; /* mov.l 0x8c64c730,r1 */
L8c64c61e: *stop=0x8c64c61eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c620; /* mov.l @r3,r5 */
L8c64c620: *stop=0x8c64c620u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c622; /* mov.l @(0x14,r15),r6 */
L8c64c622: *stop=0x8c64c622u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64c624; /* mov.l @(0x10,r15),r7 */
L8c64c624: target=r[1];*pr=0x8c64c628u;*stop=0x8c64c626u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c628; /* jsr @r1 */
L8c64c628: *stop=0x8c64c628u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64c62a; /* cmp/pz r0 */
L8c64c62a: take=(*sr&1u);*stop=0x8c64c62cu;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;if(take)goto L8c64c634;goto L8c64c62e; /* bt/s 0x8c64c634 */
L8c64c62e: *stop=0x8c64c62eu;r[2]=0u;goto L8c64c630; /* mov #0x0,r2 */
L8c64c630: *stop=0x8c64c632u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c64c638; /* bra 0x8c64c638 */
L8c64c634: *stop=0x8c64c634u;r[1]=1u;goto L8c64c636; /* mov #0x1,r1 */
L8c64c636: *stop=0x8c64c636u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c64c638; /* mov.l r1,@(0xc,r15) */
L8c64c638: *stop=0x8c64c638u;r[3]=1u;goto L8c64c63a; /* mov #0x1,r3 */
L8c64c63a: *stop=0x8c64c63au;r[4]=r[15];goto L8c64c63c; /* mov r15,r4 */
L8c64c63c: *stop=0x8c64c63cu;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c64c63e; /* mov.l r3,@(0x18,r15) */
L8c64c63e: *stop=0x8c64c63eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c734u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c640; /* mov.l 0x8c64c734,r3 */
L8c64c640: target=r[3];*pr=0x8c64c644u;*stop=0x8c64c642u;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c644; /* jsr @r3 */
L8c64c644: *stop=0x8c64c644u;r[1]=sx(m,bus,&io_status,0x8c64c720u,2);if(io_status)return io_status;goto L8c64c646; /* mov.w 0x8c64c720,r1 */
L8c64c646: *stop=0x8c64c646u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64c648; /* cmp/pz r0 */
L8c64c648: *stop=0x8c64c648u;r[1]+=r[15];goto L8c64c64a; /* add r15,r1 */
L8c64c64a: take=!(*sr&1u);*stop=0x8c64c64cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64c652;goto L8c64c64e; /* bf/s 0x8c64c652 */
L8c64c64e: *stop=0x8c64c650u;goto L8c64c814; /* bra 0x8c64c814 */
L8c64c652: *stop=0x8c64c652u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c740u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c654; /* mov.l 0x8c64c740,r2 */
L8c64c654: *stop=0x8c64c654u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c73cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c656; /* mov.l 0x8c64c73c,r4 */
L8c64c656: *stop=0x8c64c656u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c738u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c658; /* mov.l 0x8c64c738,r5 */
L8c64c658: target=r[2];*pr=0x8c64c65cu;*stop=0x8c64c65au;r[6]=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c65c; /* jsr @r2 */
L8c64c65c: *stop=0x8c64c65cu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c65e; /* mov.l @(0x10,r15),r3 */
L8c64c65e: *stop=0x8c64c65eu;r[7]=0u;goto L8c64c660; /* mov #0x0,r7 */
L8c64c660: *stop=0x8c64c660u;r[0]=sx(m,bus,&io_status,0x8c64c722u,2);if(io_status)return io_status;goto L8c64c662; /* mov.w 0x8c64c722,r0 */
L8c64c662: *stop=0x8c64c662u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64c664; /* mov.l r3,@-r15 */
L8c64c664: *stop=0x8c64c664u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c748u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c666; /* mov.l 0x8c64c748,r3 */
L8c64c666: *stop=0x8c64c666u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c744u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c668; /* mov.l 0x8c64c744,r5 */
L8c64c668: *stop=0x8c64c668u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c66a; /* mov.l @(0x18,r15),r6 */
L8c64c66a: target=r[3];*pr=0x8c64c66eu;*stop=0x8c64c66cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c66e; /* jsr @r3 */
L8c64c66e: *stop=0x8c64c66eu;r[15]+=4u;goto L8c64c670; /* add #0x4,r15 */
L8c64c670: *stop=0x8c64c670u;r[0]=sx(m,bus,&io_status,0x8c64c724u,2);if(io_status)return io_status;goto L8c64c672; /* mov.w 0x8c64c724,r0 */
L8c64c672: *stop=0x8c64c672u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c674; /* mov.l @(r0,r15),r2 */
L8c64c674: *stop=0x8c64c674u;r[0]=sx(m,bus,&io_status,0x8c64c720u,2);if(io_status)return io_status;goto L8c64c676; /* mov.w 0x8c64c720,r0 */
L8c64c676: *stop=0x8c64c676u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c678; /* mov.l @r2,r3 */
L8c64c678: *stop=0x8c64c678u;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64c67a; /* mov.w @(r0,r15),r1 */
L8c64c67a: *stop=0x8c64c67au;r[0]=66u;goto L8c64c67c; /* mov #0x42,r0 */
L8c64c67c: *stop=0x8c64c67cu;r[1]=0u-r[1];goto L8c64c67e; /* neg r1,r1 */
L8c64c67e: *stop=0x8c64c67eu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64c680; /* mov.w r1,@(r0,r3) */
L8c64c680: *stop=0x8c64c680u;r[1]=0u;goto L8c64c682; /* mov #0x0,r1 */
L8c64c682: *stop=0x8c64c682u;r[0]=sx(m,bus,&io_status,0x8c64c724u,2);if(io_status)return io_status;goto L8c64c684; /* mov.w 0x8c64c724,r0 */
L8c64c684: *stop=0x8c64c684u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c686; /* mov.l @(r0,r15),r3 */
L8c64c686: *stop=0x8c64c686u;r[0]=64u;goto L8c64c688; /* mov #0x40,r0 */
L8c64c688: *stop=0x8c64c688u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c68a; /* mov.l @r3,r2 */
L8c64c68a: *stop=0x8c64c68au;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64c68c; /* mov.w r1,@(r0,r2) */
L8c64c68c: *stop=0x8c64c68eu;goto L8c64d190; /* bra 0x8c64d190 */
L8c64c690: *stop=0x8c64c690u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c734u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c692; /* mov.l 0x8c64c734,r3 */
L8c64c692: *stop=0x8c64c692u;r[4]=r[15];goto L8c64c694; /* mov r15,r4 */
L8c64c694: target=r[3];*pr=0x8c64c698u;*stop=0x8c64c696u;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c698; /* jsr @r3 */
L8c64c698: *stop=0x8c64c698u;r[1]=sx(m,bus,&io_status,0x8c64c720u,2);if(io_status)return io_status;goto L8c64c69a; /* mov.w 0x8c64c720,r1 */
L8c64c69a: *stop=0x8c64c69au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64c69c; /* cmp/pz r0 */
L8c64c69c: *stop=0x8c64c69cu;r[1]+=r[15];goto L8c64c69e; /* add r15,r1 */
L8c64c69e: take=(*sr&1u);*stop=0x8c64c6a0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64c6e0;goto L8c64c6a2; /* bt/s 0x8c64c6e0 */
L8c64c6a2: *stop=0x8c64c6a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c740u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c6a4; /* mov.l 0x8c64c740,r2 */
L8c64c6a4: *stop=0x8c64c6a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c73cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c6a6; /* mov.l 0x8c64c73c,r4 */
L8c64c6a6: *stop=0x8c64c6a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c738u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c6a8; /* mov.l 0x8c64c738,r5 */
L8c64c6a8: target=r[2];*pr=0x8c64c6acu;*stop=0x8c64c6aau;r[6]=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c6ac; /* jsr @r2 */
L8c64c6ac: *stop=0x8c64c6acu;r[3]=4294967295u;goto L8c64c6ae; /* mov #-0x1,r3 */
L8c64c6ae: *stop=0x8c64c6aeu;r[0]=sx(m,bus,&io_status,0x8c64c722u,2);if(io_status)return io_status;goto L8c64c6b0; /* mov.w 0x8c64c722,r0 */
L8c64c6b0: *stop=0x8c64c6b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64c6b2; /* mov.l r3,@-r15 */
L8c64c6b2: *stop=0x8c64c6b2u;r[7]=0u;goto L8c64c6b4; /* mov #0x0,r7 */
L8c64c6b4: *stop=0x8c64c6b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c748u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c6b6; /* mov.l 0x8c64c748,r2 */
L8c64c6b6: *stop=0x8c64c6b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c744u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c6b8; /* mov.l 0x8c64c744,r5 */
L8c64c6b8: *stop=0x8c64c6b8u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c6ba; /* mov.l @(0x18,r15),r6 */
L8c64c6ba: target=r[2];*pr=0x8c64c6beu;*stop=0x8c64c6bcu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c6be; /* jsr @r2 */
L8c64c6be: *stop=0x8c64c6beu;r[15]+=4u;goto L8c64c6c0; /* add #0x4,r15 */
L8c64c6c0: *stop=0x8c64c6c0u;r[0]=sx(m,bus,&io_status,0x8c64c724u,2);if(io_status)return io_status;goto L8c64c6c2; /* mov.w 0x8c64c724,r0 */
L8c64c6c2: *stop=0x8c64c6c2u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c6c4; /* mov.l @(r0,r15),r3 */
L8c64c6c4: *stop=0x8c64c6c4u;r[0]=sx(m,bus,&io_status,0x8c64c720u,2);if(io_status)return io_status;goto L8c64c6c6; /* mov.w 0x8c64c720,r0 */
L8c64c6c6: *stop=0x8c64c6c6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c6c8; /* mov.l @r3,r2 */
L8c64c6c8: *stop=0x8c64c6c8u;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64c6ca; /* mov.w @(r0,r15),r1 */
L8c64c6ca: *stop=0x8c64c6cau;r[0]=66u;goto L8c64c6cc; /* mov #0x42,r0 */
L8c64c6cc: *stop=0x8c64c6ccu;r[1]=0u-r[1];goto L8c64c6ce; /* neg r1,r1 */
L8c64c6ce: *stop=0x8c64c6ceu;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64c6d0; /* mov.w r1,@(r0,r2) */
L8c64c6d0: *stop=0x8c64c6d0u;r[1]=0u;goto L8c64c6d2; /* mov #0x0,r1 */
L8c64c6d2: *stop=0x8c64c6d2u;r[0]=sx(m,bus,&io_status,0x8c64c724u,2);if(io_status)return io_status;goto L8c64c6d4; /* mov.w 0x8c64c724,r0 */
L8c64c6d4: *stop=0x8c64c6d4u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c6d6; /* mov.l @(r0,r15),r3 */
L8c64c6d6: *stop=0x8c64c6d6u;r[0]=64u;goto L8c64c6d8; /* mov #0x40,r0 */
L8c64c6d8: *stop=0x8c64c6d8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c6da; /* mov.l @r3,r2 */
L8c64c6da: *stop=0x8c64c6dau;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64c6dc; /* mov.w r1,@(r0,r2) */
L8c64c6dc: *stop=0x8c64c6deu;goto L8c64d190; /* bra 0x8c64d190 */
L8c64c6e0: *stop=0x8c64c6e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c74cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c6e2; /* mov.l 0x8c64c74c,r3 */
L8c64c6e2: *stop=0x8c64c6e2u;r[0]=sx(m,bus,&io_status,0x8c64c726u,2);if(io_status)return io_status;goto L8c64c6e4; /* mov.w 0x8c64c726,r0 */
L8c64c6e4: target=r[3];*pr=0x8c64c6e8u;*stop=0x8c64c6e6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c6e8; /* jsr @r3 */
L8c64c6e8: *stop=0x8c64c6e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c750u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c6ea; /* mov.l 0x8c64c750,r3 */
L8c64c6ea: target=r[3];*pr=0x8c64c6eeu;*stop=0x8c64c6ecu;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c6ee; /* jsr @r3 */
L8c64c6ee: *stop=0x8c64c6eeu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c64c6f0; /* cmp/eq #-0x1,r0 */
L8c64c6f0: take=!(*sr&1u);*stop=0x8c64c6f2u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;if(take)goto L8c64c75c;goto L8c64c6f4; /* bf/s 0x8c64c75c */
L8c64c6f4: *stop=0x8c64c6f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c740u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c6f6; /* mov.l 0x8c64c740,r2 */
L8c64c6f6: *stop=0x8c64c6f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c73cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c6f8; /* mov.l 0x8c64c73c,r4 */
L8c64c6f8: *stop=0x8c64c6f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c738u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c6fa; /* mov.l 0x8c64c738,r5 */
L8c64c6fa: target=r[2];*pr=0x8c64c6feu;*stop=0x8c64c6fcu;r[6]=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c6fe; /* jsr @r2 */
L8c64c6fe: *stop=0x8c64c6feu;r[3]=4294967295u;goto L8c64c700; /* mov #-0x1,r3 */
L8c64c700: *stop=0x8c64c700u;r[0]=sx(m,bus,&io_status,0x8c64c722u,2);if(io_status)return io_status;goto L8c64c702; /* mov.w 0x8c64c722,r0 */
L8c64c702: *stop=0x8c64c702u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64c704; /* mov.l r3,@-r15 */
L8c64c704: *stop=0x8c64c704u;r[7]=0u;goto L8c64c706; /* mov #0x0,r7 */
L8c64c706: *stop=0x8c64c706u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c748u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c708; /* mov.l 0x8c64c748,r2 */
L8c64c708: *stop=0x8c64c708u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c744u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c70a; /* mov.l 0x8c64c744,r5 */
L8c64c70a: *stop=0x8c64c70au;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c70c; /* mov.l @(0x18,r15),r6 */
L8c64c70c: target=r[2];*pr=0x8c64c710u;*stop=0x8c64c70eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c710; /* jsr @r2 */
L8c64c710: *stop=0x8c64c710u;r[15]+=4u;goto L8c64c712; /* add #0x4,r15 */
L8c64c712: *stop=0x8c64c712u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c758u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c714; /* mov.l 0x8c64c758,r1 */
L8c64c714: *stop=0x8c64c714u;r[3]=1u;goto L8c64c716; /* mov #0x1,r3 */
L8c64c716: *stop=0x8c64c716u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c754u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c718; /* mov.l 0x8c64c754,r2 */
L8c64c718: target=r[1];*pr=0x8c64c71cu;*stop=0x8c64c71au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c71c; /* jsr @r1 */
L8c64c71c: *stop=0x8c64c71eu;goto L8c64d190; /* bra 0x8c64d190 */
L8c64c75c: *stop=0x8c64c75cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c838u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c75e; /* mov.l 0x8c64c838,r2 */
L8c64c75e: *stop=0x8c64c75eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c760; /* mov.l @(0x8,r15),r4 */
L8c64c760: *stop=0x8c64c760u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c834u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c762; /* mov.l 0x8c64c834,r3 */
L8c64c762: target=r[2];*pr=0x8c64c766u;*stop=0x8c64c764u;r[4]&=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c766; /* jsr @r2 */
L8c64c766: *stop=0x8c64c766u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64c768; /* cmp/eq #0x1,r0 */
L8c64c768: take=!(*sr&1u);*stop=0x8c64c76au;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;if(take)goto L8c64c7b8;goto L8c64c76c; /* bf/s 0x8c64c7b8 */
L8c64c76c: *stop=0x8c64c76cu;r[0]=sx(m,bus,&io_status,0x8c64c82eu,2);if(io_status)return io_status;goto L8c64c76e; /* mov.w 0x8c64c82e,r0 */
L8c64c76e: *stop=0x8c64c76eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c83cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c770; /* mov.l 0x8c64c83c,r3 */
L8c64c770: *stop=0x8c64c770u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c772; /* mov.l @(r0,r15),r2 */
L8c64c772: *stop=0x8c64c772u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c774; /* mov.l @(0x8,r2),r1 */
L8c64c774: *stop=0x8c64c774u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c840u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c776; /* mov.l 0x8c64c840,r2 */
L8c64c776: *stop=0x8c64c776u;r[1]&=r[3];goto L8c64c778; /* and r3,r1 */
L8c64c778: *stop=0x8c64c778u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c844u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c77a; /* mov.l 0x8c64c844,r3 */
L8c64c77a: *stop=0x8c64c77au;r[1]>>=16;goto L8c64c77c; /* shlr16 r1 */
L8c64c77c: *stop=0x8c64c77cu;r[1]>>=8;goto L8c64c77e; /* shlr8 r1 */
L8c64c77e: *stop=0x8c64c77eu;r[1]>>=2;goto L8c64c780; /* shlr2 r1 */
L8c64c780: *stop=0x8c64c780u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c64c782; /* mov.l r1,@(0x10,r15) */
L8c64c782: *stop=0x8c64c782u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c848u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c784; /* mov.l 0x8c64c848,r1 */
L8c64c784: *stop=0x8c64c784u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c786; /* mov.l @(0x14,r15),r6 */
L8c64c786: *stop=0x8c64c786u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c788; /* mov.l @r2,r5 */
L8c64c788: target=r[1];*pr=0x8c64c78cu;*stop=0x8c64c78au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c78c; /* jsr @r1 */
L8c64c78c: *stop=0x8c64c78cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64c78e; /* cmp/pz r0 */
L8c64c78e: take=(*sr&1u);*stop=0x8c64c790u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[0],4);if(io_status)return io_status;if(take)goto L8c64c796;goto L8c64c792; /* bt/s 0x8c64c796 */
L8c64c792: *stop=0x8c64c794u;r[2]=2u;goto L8c64c80c; /* bra 0x8c64c80c */
L8c64c796: *stop=0x8c64c796u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c840u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c798; /* mov.l 0x8c64c840,r3 */
L8c64c798: *stop=0x8c64c798u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c844u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c79a; /* mov.l 0x8c64c844,r2 */
L8c64c79a: *stop=0x8c64c79au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c84cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c79c; /* mov.l 0x8c64c84c,r1 */
L8c64c79c: *stop=0x8c64c79cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c79e; /* mov.l @r3,r5 */
L8c64c79e: *stop=0x8c64c79eu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c7a0; /* mov.l @(0x14,r15),r6 */
L8c64c7a0: *stop=0x8c64c7a0u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64c7a2; /* mov.l @(0x10,r15),r7 */
L8c64c7a2: target=r[1];*pr=0x8c64c7a6u;*stop=0x8c64c7a4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c7a6; /* jsr @r1 */
L8c64c7a6: *stop=0x8c64c7a6u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64c7a8; /* cmp/pz r0 */
L8c64c7a8: take=(*sr&1u);*stop=0x8c64c7aau;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;if(take)goto L8c64c7b2;goto L8c64c7ac; /* bt/s 0x8c64c7b2 */
L8c64c7ac: *stop=0x8c64c7acu;r[2]=0u;goto L8c64c7ae; /* mov #0x0,r2 */
L8c64c7ae: *stop=0x8c64c7b0u;goto L8c64c80c; /* bra 0x8c64c80c */
L8c64c7b2: *stop=0x8c64c7b2u;r[1]=1u;goto L8c64c7b4; /* mov #0x1,r1 */
L8c64c7b4: *stop=0x8c64c7b6u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c64c814; /* bra 0x8c64c814 */
L8c64c7b8: *stop=0x8c64c7b8u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c7ba; /* mov.l @(0x14,r15),r0 */
L8c64c7ba: *stop=0x8c64c7bau;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c64c7bc; /* cmp/eq #-0x1,r0 */
L8c64c7bc: if(!(*sr&1u))goto L8c64c7f6;goto L8c64c7be; /* bf 0x8c64c7f6 */
L8c64c7be: *stop=0x8c64c7beu;r[2]=4294967295u;goto L8c64c7c0; /* mov #-0x1,r2 */
L8c64c7c0: *stop=0x8c64c7c0u;r[0]=sx(m,bus,&io_status,0x8c64c830u,2);if(io_status)return io_status;goto L8c64c7c2; /* mov.w 0x8c64c830,r0 */
L8c64c7c2: *stop=0x8c64c7c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64c7c4; /* mov.l r2,@-r15 */
L8c64c7c4: *stop=0x8c64c7c4u;r[7]=0u;goto L8c64c7c6; /* mov #0x0,r7 */
L8c64c7c6: *stop=0x8c64c7c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c854u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c7c8; /* mov.l 0x8c64c854,r3 */
L8c64c7c8: *stop=0x8c64c7c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c850u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c7ca; /* mov.l 0x8c64c850,r5 */
L8c64c7ca: *stop=0x8c64c7cau;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c7cc; /* mov.l @(0x18,r15),r6 */
L8c64c7cc: target=r[3];*pr=0x8c64c7d0u;*stop=0x8c64c7ceu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c7d0; /* jsr @r3 */
L8c64c7d0: *stop=0x8c64c7d0u;r[15]+=4u;goto L8c64c7d2; /* add #0x4,r15 */
L8c64c7d2: *stop=0x8c64c7d2u;r[0]=sx(m,bus,&io_status,0x8c64c832u,2);if(io_status)return io_status;goto L8c64c7d4; /* mov.w 0x8c64c832,r0 */
L8c64c7d4: *stop=0x8c64c7d4u;r[1]=9u;goto L8c64c7d6; /* mov #0x9,r1 */
L8c64c7d6: *stop=0x8c64c7d6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c7d8; /* mov.l @(r0,r15),r2 */
L8c64c7d8: *stop=0x8c64c7d8u;r[0]=66u;goto L8c64c7da; /* mov #0x42,r0 */
L8c64c7da: *stop=0x8c64c7dau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c7dc; /* mov.l @r2,r3 */
L8c64c7dc: *stop=0x8c64c7dcu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64c7de; /* mov.w r1,@(r0,r3) */
L8c64c7de: *stop=0x8c64c7deu;r[1]=0u;goto L8c64c7e0; /* mov #0x0,r1 */
L8c64c7e0: *stop=0x8c64c7e0u;r[0]=sx(m,bus,&io_status,0x8c64c832u,2);if(io_status)return io_status;goto L8c64c7e2; /* mov.w 0x8c64c832,r0 */
L8c64c7e2: *stop=0x8c64c7e2u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c7e4; /* mov.l @(r0,r15),r3 */
L8c64c7e4: *stop=0x8c64c7e4u;r[0]=64u;goto L8c64c7e6; /* mov #0x40,r0 */
L8c64c7e6: *stop=0x8c64c7e6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c7e8; /* mov.l @r3,r2 */
L8c64c7e8: *stop=0x8c64c7e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c858u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c7ea; /* mov.l 0x8c64c858,r3 */
L8c64c7ea: target=r[3];*pr=0x8c64c7eeu;*stop=0x8c64c7ecu;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c7ee; /* jsr @r3 */
L8c64c7ee: *stop=0x8c64c7eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c85cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c7f0; /* mov.l 0x8c64c85c,r3 */
L8c64c7f0: *stop=0x8c64c7f0u;r[2]=9u;goto L8c64c7f2; /* mov #0x9,r2 */
L8c64c7f2: *stop=0x8c64c7f4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64d190; /* bra 0x8c64d190 */
L8c64c7f6: *stop=0x8c64c7f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c840u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c7f8; /* mov.l 0x8c64c840,r3 */
L8c64c7f8: *stop=0x8c64c7f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c844u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c7fa; /* mov.l 0x8c64c844,r2 */
L8c64c7fa: *stop=0x8c64c7fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c848u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c7fc; /* mov.l 0x8c64c848,r1 */
L8c64c7fc: *stop=0x8c64c7fcu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c7fe; /* mov.l @(0x14,r15),r6 */
L8c64c7fe: *stop=0x8c64c7feu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c800; /* mov.l @r3,r5 */
L8c64c800: target=r[1];*pr=0x8c64c804u;*stop=0x8c64c802u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c804; /* jsr @r1 */
L8c64c804: *stop=0x8c64c804u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64c806; /* cmp/pz r0 */
L8c64c806: take=(*sr&1u);*stop=0x8c64c808u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[0],4);if(io_status)return io_status;if(take)goto L8c64c810;goto L8c64c80a; /* bt/s 0x8c64c810 */
L8c64c80a: *stop=0x8c64c80au;r[2]=2u;goto L8c64c80c; /* mov #0x2,r2 */
L8c64c80c: *stop=0x8c64c80eu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c64c814; /* bra 0x8c64c814 */
L8c64c810: *stop=0x8c64c810u;r[1]=1u;goto L8c64c812; /* mov #0x1,r1 */
L8c64c812: *stop=0x8c64c812u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c64c814; /* mov.l r1,@(0xc,r15) */
L8c64c814: *stop=0x8c64c814u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c816; /* mov.l @(0xc,r15),r0 */
L8c64c816: *stop=0x8c64c816u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64c818; /* cmp/eq #0x0,r0 */
L8c64c818: if((*sr&1u))goto L8c64c860;goto L8c64c81a; /* bt 0x8c64c860 */
L8c64c81a: *stop=0x8c64c81au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64c81c; /* cmp/eq #0x1,r0 */
L8c64c81c: if(!(*sr&1u))goto L8c64c822;goto L8c64c81e; /* bf 0x8c64c822 */
L8c64c81e: *stop=0x8c64c820u;goto L8c64cbce; /* bra 0x8c64cbce */
L8c64c822: *stop=0x8c64c822u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c64c824; /* cmp/eq #0x2,r0 */
L8c64c824: if(!(*sr&1u))goto L8c64c82a;goto L8c64c826; /* bf 0x8c64c82a */
L8c64c826: *stop=0x8c64c828u;goto L8c64ceac; /* bra 0x8c64ceac */
L8c64c82a: *stop=0x8c64c82cu;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64c860: *stop=0x8c64c860u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c862; /* mov.l @(0x20,r15),r3 */
L8c64c862: *stop=0x8c64c862u;r[0]=sx(m,bus,&io_status,0x8c64c988u,2);if(io_status)return io_status;goto L8c64c864; /* mov.w 0x8c64c988,r0 */
L8c64c864: *stop=0x8c64c864u;r[2]=r[3];goto L8c64c866; /* mov r3,r2 */
L8c64c866: *stop=0x8c64c866u;r[3]<<=2;goto L8c64c868; /* shll2 r3 */
L8c64c868: *stop=0x8c64c868u;r[3]<<=2;goto L8c64c86a; /* shll2 r3 */
L8c64c86a: *stop=0x8c64c86au;r[3]+=r[2];goto L8c64c86c; /* add r2,r3 */
L8c64c86c: *stop=0x8c64c86cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c994u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c86e; /* mov.l 0x8c64c994,r2 */
L8c64c86e: *stop=0x8c64c86eu;r[3]<<=2;goto L8c64c870; /* shll2 r3 */
L8c64c870: *stop=0x8c64c870u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c872; /* mov.l @r2,r1 */
L8c64c872: *stop=0x8c64c872u;r[3]+=r[1];goto L8c64c874; /* add r1,r3 */
L8c64c874: *stop=0x8c64c874u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c64c876; /* mov.l r3,@(r0,r15) */
L8c64c876: *stop=0x8c64c876u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c878; /* mov.l @(0x10,r3),r2 */
L8c64c878: *stop=0x8c64c878u;r[0]=r[2];goto L8c64c87a; /* mov r2,r0 */
L8c64c87a: *stop=0x8c64c87au;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c64c87c; /* tst #0x2,r0 */
L8c64c87c: *stop=0x8c64c87cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c64c87e; /* mov.l r2,@(0x4,r15) */
L8c64c87e: if(!(*sr&1u))goto L8c64c884;goto L8c64c880; /* bf 0x8c64c884 */
L8c64c880: *stop=0x8c64c882u;goto L8c64ca68; /* bra 0x8c64ca68 */
L8c64c884: *stop=0x8c64c884u;r[0]=sx(m,bus,&io_status,0x8c64c98au,2);if(io_status)return io_status;goto L8c64c886; /* mov.w 0x8c64c98a,r0 */
L8c64c886: *stop=0x8c64c886u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c998u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c888; /* mov.l 0x8c64c998,r3 */
L8c64c888: *stop=0x8c64c888u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c88a; /* mov.l @(r0,r15),r0 */
L8c64c88a: *stop=0x8c64c88au;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c88c; /* mov.l @(0x4,r0),r0 */
L8c64c88c: *stop=0x8c64c88cu;r[0]&=r[3];goto L8c64c88e; /* and r3,r0 */
L8c64c88e: *stop=0x8c64c88eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64c890; /* cmp/eq #0x0,r0 */
L8c64c890: if((*sr&1u))goto L8c64c8a2;goto L8c64c892; /* bt 0x8c64c8a2 */
L8c64c892: *stop=0x8c64c892u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c99cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c894; /* mov.l 0x8c64c99c,r1 */
L8c64c894: *stop=0x8c64c894u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64c896; /* cmp/eq r1,r0 */
L8c64c896: if((*sr&1u))goto L8c64c8ec;goto L8c64c898; /* bt 0x8c64c8ec */
L8c64c898: *stop=0x8c64c898u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9a0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c89a; /* mov.l 0x8c64c9a0,r1 */
L8c64c89a: *stop=0x8c64c89au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64c89c; /* cmp/eq r1,r0 */
L8c64c89c: if((*sr&1u))goto L8c64c8ec;goto L8c64c89e; /* bt 0x8c64c8ec */
L8c64c89e: *stop=0x8c64c8a0u;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64c8a2: *stop=0x8c64c8a2u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c8a4; /* mov.l @(0x10,r15),r2 */
L8c64c8a4: *stop=0x8c64c8a4u;r[7]=2u;goto L8c64c8a6; /* mov #0x2,r7 */
L8c64c8a6: *stop=0x8c64c8a6u;r[0]=sx(m,bus,&io_status,0x8c64c98cu,2);if(io_status)return io_status;goto L8c64c8a8; /* mov.w 0x8c64c98c,r0 */
L8c64c8a8: *stop=0x8c64c8a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64c8aa; /* mov.l r2,@-r15 */
L8c64c8aa: *stop=0x8c64c8aau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c8ac; /* mov.l @(r0,r15),r5 */
L8c64c8ac: *stop=0x8c64c8acu;r[0]=sx(m,bus,&io_status,0x8c64c98eu,2);if(io_status)return io_status;goto L8c64c8ae; /* mov.w 0x8c64c98e,r0 */
L8c64c8ae: *stop=0x8c64c8aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c8b0; /* mov.l 0x8c64c9a4,r2 */
L8c64c8b0: *stop=0x8c64c8b0u;r[5]+=20u;goto L8c64c8b2; /* add #0x14,r5 */
L8c64c8b2: *stop=0x8c64c8b2u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c8b4; /* mov.l @(0x18,r15),r6 */
L8c64c8b4: target=r[2];*pr=0x8c64c8b8u;*stop=0x8c64c8b6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c8b8; /* jsr @r2 */
L8c64c8b8: *stop=0x8c64c8b8u;r[15]+=4u;goto L8c64c8ba; /* add #0x4,r15 */
L8c64c8ba: *stop=0x8c64c8bau;r[0]=sx(m,bus,&io_status,0x8c64c990u,2);if(io_status)return io_status;goto L8c64c8bc; /* mov.w 0x8c64c990,r0 */
L8c64c8bc: *stop=0x8c64c8bcu;r[1]=0u;goto L8c64c8be; /* mov #0x0,r1 */
L8c64c8be: *stop=0x8c64c8beu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c8c0; /* mov.l @(r0,r15),r3 */
L8c64c8c0: *stop=0x8c64c8c0u;r[0]=66u;goto L8c64c8c2; /* mov #0x42,r0 */
L8c64c8c2: *stop=0x8c64c8c2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c8c4; /* mov.l @r3,r2 */
L8c64c8c4: *stop=0x8c64c8c4u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64c8c6; /* mov.w r1,@(r0,r2) */
L8c64c8c6: *stop=0x8c64c8c6u;r[1]=1u;goto L8c64c8c8; /* mov #0x1,r1 */
L8c64c8c8: *stop=0x8c64c8c8u;r[0]=sx(m,bus,&io_status,0x8c64c990u,2);if(io_status)return io_status;goto L8c64c8ca; /* mov.w 0x8c64c990,r0 */
L8c64c8ca: *stop=0x8c64c8cau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c8cc; /* mov.l @(r0,r15),r3 */
L8c64c8cc: *stop=0x8c64c8ccu;r[0]=64u;goto L8c64c8ce; /* mov #0x40,r0 */
L8c64c8ce: *stop=0x8c64c8ceu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c8d0; /* mov.l @r3,r2 */
L8c64c8d0: *stop=0x8c64c8d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c8d2; /* mov.l 0x8c64c9a8,r3 */
L8c64c8d2: *stop=0x8c64c8d2u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64c8d4; /* mov.w r1,@(r0,r2) */
L8c64c8d4: *stop=0x8c64c8d4u;r[0]=sx(m,bus,&io_status,0x8c64c990u,2);if(io_status)return io_status;goto L8c64c8d6; /* mov.w 0x8c64c990,r0 */
L8c64c8d6: *stop=0x8c64c8d6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c8d8; /* mov.l @(r0,r15),r4 */
L8c64c8d8: target=r[3];*pr=0x8c64c8dcu;*stop=0x8c64c8dau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c8dc; /* jsr @r3 */
L8c64c8dc: *stop=0x8c64c8dcu;r[0]=sx(m,bus,&io_status,0x8c64c990u,2);if(io_status)return io_status;goto L8c64c8de; /* mov.w 0x8c64c990,r0 */
L8c64c8de: *stop=0x8c64c8deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c8e0; /* mov.l 0x8c64c9ac,r3 */
L8c64c8e0: *stop=0x8c64c8e0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c8e2; /* mov.l @(r0,r15),r4 */
L8c64c8e2: *stop=0x8c64c8e2u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c8e4; /* mov.l @(0x10,r15),r5 */
L8c64c8e4: target=r[3];*pr=0x8c64c8e8u;*stop=0x8c64c8e6u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c8e8; /* jsr @r3 */
L8c64c8e8: *stop=0x8c64c8eau;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64c8ec: *stop=0x8c64c8ecu;r[0]=sx(m,bus,&io_status,0x8c64c988u,2);if(io_status)return io_status;goto L8c64c8ee; /* mov.w 0x8c64c988,r0 */
L8c64c8ee: *stop=0x8c64c8eeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c8f0; /* mov.l @(r0,r15),r0 */
L8c64c8f0: *stop=0x8c64c8f0u;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c8f2; /* mov.l @(0x10,r0),r0 */
L8c64c8f2: *stop=0x8c64c8f2u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c64c8f4; /* tst #0x1,r0 */
L8c64c8f4: if((*sr&1u))goto L8c64c8fa;goto L8c64c8f6; /* bt 0x8c64c8fa */
L8c64c8f6: *stop=0x8c64c8f8u;goto L8c64ca1e; /* bra 0x8c64ca1e */
L8c64c8fa: *stop=0x8c64c8fau;r[0]=sx(m,bus,&io_status,0x8c64c990u,2);if(io_status)return io_status;goto L8c64c8fc; /* mov.w 0x8c64c990,r0 */
L8c64c8fc: *stop=0x8c64c8fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c8fe; /* mov.l 0x8c64c9b0,r2 */
L8c64c8fe: *stop=0x8c64c8feu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c900; /* mov.l @(r0,r15),r5 */
L8c64c900: *stop=0x8c64c900u;r[0]=sx(m,bus,&io_status,0x8c64c98au,2);if(io_status)return io_status;goto L8c64c902; /* mov.w 0x8c64c98a,r0 */
L8c64c902: *stop=0x8c64c902u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c904; /* mov.l @r5,r5 */
L8c64c904: *stop=0x8c64c904u;r[5]+=20u;goto L8c64c906; /* add #0x14,r5 */
L8c64c906: target=r[2];*pr=0x8c64c90au;*stop=0x8c64c908u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c90a; /* jsr @r2 */
L8c64c90a: *stop=0x8c64c90au;r[1]=sx(m,bus,&io_status,0x8c64c992u,2);if(io_status)return io_status;goto L8c64c90c; /* mov.w 0x8c64c992,r1 */
L8c64c90c: *stop=0x8c64c90cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64c90e; /* cmp/pz r0 */
L8c64c90e: *stop=0x8c64c90eu;r[1]+=r[15];goto L8c64c910; /* add r15,r1 */
L8c64c910: take=(*sr&1u);*stop=0x8c64c912u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64c93c;goto L8c64c914; /* bt/s 0x8c64c93c */
L8c64c914: *stop=0x8c64c914u;r[0]=sx(m,bus,&io_status,0x8c64c990u,2);if(io_status)return io_status;goto L8c64c916; /* mov.w 0x8c64c990,r0 */
L8c64c916: *stop=0x8c64c916u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c918; /* mov.l @(r0,r15),r2 */
L8c64c918: *stop=0x8c64c918u;r[0]=sx(m,bus,&io_status,0x8c64c992u,2);if(io_status)return io_status;goto L8c64c91a; /* mov.w 0x8c64c992,r0 */
L8c64c91a: *stop=0x8c64c91au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c91c; /* mov.l @r2,r3 */
L8c64c91c: *stop=0x8c64c91cu;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64c91e; /* mov.w @(r0,r15),r1 */
L8c64c91e: *stop=0x8c64c91eu;r[0]=66u;goto L8c64c920; /* mov #0x42,r0 */
L8c64c920: *stop=0x8c64c920u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c922; /* mov.l 0x8c64c9b4,r2 */
L8c64c922: *stop=0x8c64c922u;r[1]=0u-r[1];goto L8c64c924; /* neg r1,r1 */
L8c64c924: *stop=0x8c64c924u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64c926; /* mov.w r1,@(r0,r3) */
L8c64c926: *stop=0x8c64c926u;r[0]=sx(m,bus,&io_status,0x8c64c98eu,2);if(io_status)return io_status;goto L8c64c928; /* mov.w 0x8c64c98e,r0 */
L8c64c928: *stop=0x8c64c928u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c92a; /* mov.l @(r0,r15),r3 */
L8c64c92a: *stop=0x8c64c92au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64c92c; /* mov.l r3,@r2 */
L8c64c92c: *stop=0x8c64c92cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c92e; /* mov.l 0x8c64c9b8,r3 */
L8c64c92e: *stop=0x8c64c92eu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c930; /* mov.l @(0x14,r15),r1 */
L8c64c930: *stop=0x8c64c930u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64c932; /* mov.l r1,@r3 */
L8c64c932: *stop=0x8c64c932u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9bcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c934; /* mov.l 0x8c64c9bc,r1 */
L8c64c934: target=r[1];*pr=0x8c64c938u;*stop=0x8c64c936u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c938; /* jsr @r1 */
L8c64c938: *stop=0x8c64c93au;goto L8c64d190; /* bra 0x8c64d190 */
L8c64c93c: *stop=0x8c64c93cu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c93e; /* mov.l @(0x10,r15),r2 */
L8c64c93e: *stop=0x8c64c93eu;r[7]=2u;goto L8c64c940; /* mov #0x2,r7 */
L8c64c940: *stop=0x8c64c940u;r[0]=sx(m,bus,&io_status,0x8c64c98cu,2);if(io_status)return io_status;goto L8c64c942; /* mov.w 0x8c64c98c,r0 */
L8c64c942: *stop=0x8c64c942u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64c944; /* mov.l r2,@-r15 */
L8c64c944: *stop=0x8c64c944u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c946; /* mov.l @(r0,r15),r5 */
L8c64c946: *stop=0x8c64c946u;r[0]=sx(m,bus,&io_status,0x8c64c98eu,2);if(io_status)return io_status;goto L8c64c948; /* mov.w 0x8c64c98e,r0 */
L8c64c948: *stop=0x8c64c948u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c94a; /* mov.l 0x8c64c9a4,r3 */
L8c64c94a: *stop=0x8c64c94au;r[5]+=20u;goto L8c64c94c; /* add #0x14,r5 */
L8c64c94c: *stop=0x8c64c94cu;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64c94e; /* mov.l @(0x18,r15),r6 */
L8c64c94e: target=r[3];*pr=0x8c64c952u;*stop=0x8c64c950u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c952; /* jsr @r3 */
L8c64c952: *stop=0x8c64c952u;r[15]+=4u;goto L8c64c954; /* add #0x4,r15 */
L8c64c954: *stop=0x8c64c954u;r[0]=sx(m,bus,&io_status,0x8c64c990u,2);if(io_status)return io_status;goto L8c64c956; /* mov.w 0x8c64c990,r0 */
L8c64c956: *stop=0x8c64c956u;r[1]=0u;goto L8c64c958; /* mov #0x0,r1 */
L8c64c958: *stop=0x8c64c958u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c95a; /* mov.l @(r0,r15),r2 */
L8c64c95a: *stop=0x8c64c95au;r[0]=66u;goto L8c64c95c; /* mov #0x42,r0 */
L8c64c95c: *stop=0x8c64c95cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c95e; /* mov.l @r2,r3 */
L8c64c95e: *stop=0x8c64c95eu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64c960; /* mov.w r1,@(r0,r3) */
L8c64c960: *stop=0x8c64c960u;r[1]=1u;goto L8c64c962; /* mov #0x1,r1 */
L8c64c962: *stop=0x8c64c962u;r[0]=sx(m,bus,&io_status,0x8c64c990u,2);if(io_status)return io_status;goto L8c64c964; /* mov.w 0x8c64c990,r0 */
L8c64c964: *stop=0x8c64c964u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c966; /* mov.l @(r0,r15),r3 */
L8c64c966: *stop=0x8c64c966u;r[0]=64u;goto L8c64c968; /* mov #0x40,r0 */
L8c64c968: *stop=0x8c64c968u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c96a; /* mov.l @r3,r2 */
L8c64c96a: *stop=0x8c64c96au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c96c; /* mov.l 0x8c64c9a8,r3 */
L8c64c96c: *stop=0x8c64c96cu;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64c96e; /* mov.w r1,@(r0,r2) */
L8c64c96e: *stop=0x8c64c96eu;r[0]=sx(m,bus,&io_status,0x8c64c990u,2);if(io_status)return io_status;goto L8c64c970; /* mov.w 0x8c64c990,r0 */
L8c64c970: *stop=0x8c64c970u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c972; /* mov.l @(r0,r15),r4 */
L8c64c972: target=r[3];*pr=0x8c64c976u;*stop=0x8c64c974u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c976; /* jsr @r3 */
L8c64c976: *stop=0x8c64c976u;r[0]=sx(m,bus,&io_status,0x8c64c990u,2);if(io_status)return io_status;goto L8c64c978; /* mov.w 0x8c64c990,r0 */
L8c64c978: *stop=0x8c64c978u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c9acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c97a; /* mov.l 0x8c64c9ac,r3 */
L8c64c97a: *stop=0x8c64c97au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64c97c; /* mov.l @(r0,r15),r4 */
L8c64c97c: *stop=0x8c64c97cu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64c97e; /* mov.l @(0x10,r15),r5 */
L8c64c97e: target=r[3];*pr=0x8c64c982u;*stop=0x8c64c980u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c982; /* jsr @r3 */
L8c64c982: *stop=0x8c64c982u;r[2]=0u;goto L8c64c984; /* mov #0x0,r2 */
L8c64c984: *stop=0x8c64c986u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64ca10; /* bra 0x8c64ca10 */
L8c64c9c0: *stop=0x8c64c9c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c9c2; /* mov.l @r15,r1 */
L8c64c9c2: *stop=0x8c64c9c2u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c9c4; /* mov.l @(0x14,r15),r0 */
L8c64c9c4: *stop=0x8c64c9c4u;r[3]=r[1];goto L8c64c9c6; /* mov r1,r3 */
L8c64c9c6: *stop=0x8c64c9c6u;r[1]<<=2;goto L8c64c9c8; /* shll2 r1 */
L8c64c9c8: *stop=0x8c64c9c8u;r[1]<<=2;goto L8c64c9ca; /* shll2 r1 */
L8c64c9ca: *stop=0x8c64c9cau;r[1]+=r[3];goto L8c64c9cc; /* add r3,r1 */
L8c64c9cc: *stop=0x8c64c9ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cad4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64c9ce; /* mov.l 0x8c64cad4,r3 */
L8c64c9ce: *stop=0x8c64c9ceu;r[1]<<=2;goto L8c64c9d0; /* shll2 r1 */
L8c64c9d0: *stop=0x8c64c9d0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c9d2; /* mov.l @r3,r2 */
L8c64c9d2: *stop=0x8c64c9d2u;r[1]+=r[2];goto L8c64c9d4; /* add r2,r1 */
L8c64c9d4: *stop=0x8c64c9d4u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c9d6; /* mov.l @r1,r2 */
L8c64c9d6: *stop=0x8c64c9d6u;*sr=(*sr&~1u)|(r[0]==r[2]);goto L8c64c9d8; /* cmp/eq r0,r2 */
L8c64c9d8: if(!(*sr&1u))goto L8c64ca0a;goto L8c64c9da; /* bf 0x8c64ca0a */
L8c64c9da: *stop=0x8c64c9dau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c9dc; /* mov.l @r15,r2 */
L8c64c9dc: *stop=0x8c64c9dcu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c9de; /* mov.l @r3,r1 */
L8c64c9de: *stop=0x8c64c9deu;r[0]=r[2];goto L8c64c9e0; /* mov r2,r0 */
L8c64c9e0: *stop=0x8c64c9e0u;r[2]<<=2;goto L8c64c9e2; /* shll2 r2 */
L8c64c9e2: *stop=0x8c64c9e2u;r[2]<<=2;goto L8c64c9e4; /* shll2 r2 */
L8c64c9e4: *stop=0x8c64c9e4u;r[2]+=r[0];goto L8c64c9e6; /* add r0,r2 */
L8c64c9e6: *stop=0x8c64c9e6u;r[2]<<=2;goto L8c64c9e8; /* shll2 r2 */
L8c64c9e8: *stop=0x8c64c9e8u;r[2]+=r[1];goto L8c64c9ea; /* add r1,r2 */
L8c64c9ea: *stop=0x8c64c9eau;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c9ec; /* mov.l @(0x10,r2),r0 */
L8c64c9ec: *stop=0x8c64c9ecu;r[0]|=0x1u;goto L8c64c9ee; /* or #0x1,r0 */
L8c64c9ee: *stop=0x8c64c9eeu;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[0],4);if(io_status)return io_status;goto L8c64c9f0; /* mov.l r0,@(0x10,r2) */
L8c64c9f0: *stop=0x8c64c9f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c9f2; /* mov.l @r15,r2 */
L8c64c9f2: *stop=0x8c64c9f2u;r[0]=sx(m,bus,&io_status,0x8c64cac6u,2);if(io_status)return io_status;goto L8c64c9f4; /* mov.w 0x8c64cac6,r0 */
L8c64c9f4: *stop=0x8c64c9f4u;r[1]=r[2];goto L8c64c9f6; /* mov r2,r1 */
L8c64c9f6: *stop=0x8c64c9f6u;r[2]<<=2;goto L8c64c9f8; /* shll2 r2 */
L8c64c9f8: *stop=0x8c64c9f8u;r[2]<<=2;goto L8c64c9fa; /* shll2 r2 */
L8c64c9fa: *stop=0x8c64c9fau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c9fc; /* mov.l @(r0,r15),r0 */
L8c64c9fc: *stop=0x8c64c9fcu;r[2]+=r[1];goto L8c64c9fe; /* add r1,r2 */
L8c64c9fe: *stop=0x8c64c9feu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ca00; /* mov.l @r3,r1 */
L8c64ca00: *stop=0x8c64ca00u;r[2]<<=2;goto L8c64ca02; /* shll2 r2 */
L8c64ca02: *stop=0x8c64ca02u;r[2]+=r[1];goto L8c64ca04; /* add r1,r2 */
L8c64ca04: *stop=0x8c64ca04u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ca06; /* mov.l @r0,r1 */
L8c64ca06: *stop=0x8c64ca06u;if(!valid_access(r[1]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ca08; /* mov.l @(0x14,r1),r1 */
L8c64ca08: *stop=0x8c64ca08u;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[1],4);if(io_status)return io_status;goto L8c64ca0a; /* mov.l r1,@(0x14,r2) */
L8c64ca0a: *stop=0x8c64ca0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ca0c; /* mov.l @r15,r3 */
L8c64ca0c: *stop=0x8c64ca0cu;r[3]+=1u;goto L8c64ca0e; /* add #0x1,r3 */
L8c64ca0e: *stop=0x8c64ca0eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64ca10; /* mov.l r3,@r15 */
L8c64ca10: *stop=0x8c64ca10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cad8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ca12; /* mov.l 0x8c64cad8,r3 */
L8c64ca12: *stop=0x8c64ca12u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ca14; /* mov.l @r15,r1 */
L8c64ca14: *stop=0x8c64ca14u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ca16; /* mov.l @r3,r2 */
L8c64ca16: *stop=0x8c64ca16u;*sr=(*sr&~1u)|(r[1]>=r[2]);goto L8c64ca18; /* cmp/hs r2,r1 */
L8c64ca18: if(!(*sr&1u))goto L8c64c9c0;goto L8c64ca1a; /* bf 0x8c64c9c0 */
L8c64ca1a: *stop=0x8c64ca1cu;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64ca1e: *stop=0x8c64ca1eu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ca20; /* mov.l @(0x10,r15),r2 */
L8c64ca20: *stop=0x8c64ca20u;r[7]=3u;goto L8c64ca22; /* mov #0x3,r7 */
L8c64ca22: *stop=0x8c64ca22u;r[0]=sx(m,bus,&io_status,0x8c64cac8u,2);if(io_status)return io_status;goto L8c64ca24; /* mov.w 0x8c64cac8,r0 */
L8c64ca24: *stop=0x8c64ca24u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64ca26; /* mov.l r2,@-r15 */
L8c64ca26: *stop=0x8c64ca26u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ca28; /* mov.l @(r0,r15),r5 */
L8c64ca28: *stop=0x8c64ca28u;r[0]=sx(m,bus,&io_status,0x8c64cacau,2);if(io_status)return io_status;goto L8c64ca2a; /* mov.w 0x8c64caca,r0 */
L8c64ca2a: *stop=0x8c64ca2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cadcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ca2c; /* mov.l 0x8c64cadc,r2 */
L8c64ca2c: *stop=0x8c64ca2cu;r[5]+=20u;goto L8c64ca2e; /* add #0x14,r5 */
L8c64ca2e: *stop=0x8c64ca2eu;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64ca30; /* mov.l @(0x18,r15),r6 */
L8c64ca30: target=r[2];*pr=0x8c64ca34u;*stop=0x8c64ca32u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ca34; /* jsr @r2 */
L8c64ca34: *stop=0x8c64ca34u;r[15]+=4u;goto L8c64ca36; /* add #0x4,r15 */
L8c64ca36: *stop=0x8c64ca36u;r[0]=sx(m,bus,&io_status,0x8c64cac6u,2);if(io_status)return io_status;goto L8c64ca38; /* mov.w 0x8c64cac6,r0 */
L8c64ca38: *stop=0x8c64ca38u;r[1]=0u;goto L8c64ca3a; /* mov #0x0,r1 */
L8c64ca3a: *stop=0x8c64ca3au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ca3c; /* mov.l @(r0,r15),r3 */
L8c64ca3c: *stop=0x8c64ca3cu;r[0]=66u;goto L8c64ca3e; /* mov #0x42,r0 */
L8c64ca3e: *stop=0x8c64ca3eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ca40; /* mov.l @r3,r2 */
L8c64ca40: *stop=0x8c64ca40u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64ca42; /* mov.w r1,@(r0,r2) */
L8c64ca42: *stop=0x8c64ca42u;r[1]=1u;goto L8c64ca44; /* mov #0x1,r1 */
L8c64ca44: *stop=0x8c64ca44u;r[0]=sx(m,bus,&io_status,0x8c64cac6u,2);if(io_status)return io_status;goto L8c64ca46; /* mov.w 0x8c64cac6,r0 */
L8c64ca46: *stop=0x8c64ca46u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ca48; /* mov.l @(r0,r15),r3 */
L8c64ca48: *stop=0x8c64ca48u;r[0]=64u;goto L8c64ca4a; /* mov #0x40,r0 */
L8c64ca4a: *stop=0x8c64ca4au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ca4c; /* mov.l @r3,r2 */
L8c64ca4c: *stop=0x8c64ca4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cae0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ca4e; /* mov.l 0x8c64cae0,r3 */
L8c64ca4e: *stop=0x8c64ca4eu;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64ca50; /* mov.w r1,@(r0,r2) */
L8c64ca50: *stop=0x8c64ca50u;r[0]=sx(m,bus,&io_status,0x8c64cac6u,2);if(io_status)return io_status;goto L8c64ca52; /* mov.w 0x8c64cac6,r0 */
L8c64ca52: *stop=0x8c64ca52u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64ca54; /* mov.l @(r0,r15),r4 */
L8c64ca54: target=r[3];*pr=0x8c64ca58u;*stop=0x8c64ca56u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ca58; /* jsr @r3 */
L8c64ca58: *stop=0x8c64ca58u;r[0]=sx(m,bus,&io_status,0x8c64cac6u,2);if(io_status)return io_status;goto L8c64ca5a; /* mov.w 0x8c64cac6,r0 */
L8c64ca5a: *stop=0x8c64ca5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cae4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ca5c; /* mov.l 0x8c64cae4,r3 */
L8c64ca5c: *stop=0x8c64ca5cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64ca5e; /* mov.l @(r0,r15),r4 */
L8c64ca5e: *stop=0x8c64ca5eu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ca60; /* mov.l @(0x10,r15),r5 */
L8c64ca60: target=r[3];*pr=0x8c64ca64u;*stop=0x8c64ca62u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ca64; /* jsr @r3 */
L8c64ca64: *stop=0x8c64ca66u;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64ca68: *stop=0x8c64ca68u;r[0]=sx(m,bus,&io_status,0x8c64caccu,2);if(io_status)return io_status;goto L8c64ca6a; /* mov.w 0x8c64cacc,r0 */
L8c64ca6a: *stop=0x8c64ca6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cae8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ca6c; /* mov.l 0x8c64cae8,r3 */
L8c64ca6c: *stop=0x8c64ca6cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ca6e; /* mov.l @(r0,r15),r0 */
L8c64ca6e: *stop=0x8c64ca6eu;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ca70; /* mov.l @(0x4,r0),r0 */
L8c64ca70: *stop=0x8c64ca70u;r[0]&=r[3];goto L8c64ca72; /* and r3,r0 */
L8c64ca72: *stop=0x8c64ca72u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64ca74; /* cmp/eq #0x0,r0 */
L8c64ca74: if((*sr&1u))goto L8c64ca86;goto L8c64ca76; /* bt 0x8c64ca86 */
L8c64ca76: *stop=0x8c64ca76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64caecu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ca78; /* mov.l 0x8c64caec,r1 */
L8c64ca78: *stop=0x8c64ca78u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64ca7a; /* cmp/eq r1,r0 */
L8c64ca7a: if((*sr&1u))goto L8c64cb3c;goto L8c64ca7c; /* bt 0x8c64cb3c */
L8c64ca7c: *stop=0x8c64ca7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64caf0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ca7e; /* mov.l 0x8c64caf0,r1 */
L8c64ca7e: *stop=0x8c64ca7eu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64ca80; /* cmp/eq r1,r0 */
L8c64ca80: if((*sr&1u))goto L8c64cb56;goto L8c64ca82; /* bt 0x8c64cb56 */
L8c64ca82: *stop=0x8c64ca84u;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64ca86: *stop=0x8c64ca86u;r[0]=sx(m,bus,&io_status,0x8c64caceu,2);if(io_status)return io_status;goto L8c64ca88; /* mov.w 0x8c64cace,r0 */
L8c64ca88: *stop=0x8c64ca88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64caf4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ca8a; /* mov.l 0x8c64caf4,r2 */
L8c64ca8a: *stop=0x8c64ca8au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ca8c; /* mov.l @(r0,r15),r5 */
L8c64ca8c: *stop=0x8c64ca8cu;r[0]=sx(m,bus,&io_status,0x8c64caccu,2);if(io_status)return io_status;goto L8c64ca8e; /* mov.w 0x8c64cacc,r0 */
L8c64ca8e: *stop=0x8c64ca8eu;r[5]+=20u;goto L8c64ca90; /* add #0x14,r5 */
L8c64ca90: target=r[2];*pr=0x8c64ca94u;*stop=0x8c64ca92u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ca94; /* jsr @r2 */
L8c64ca94: *stop=0x8c64ca94u;r[1]=sx(m,bus,&io_status,0x8c64cad0u,2);if(io_status)return io_status;goto L8c64ca96; /* mov.w 0x8c64cad0,r1 */
L8c64ca96: *stop=0x8c64ca96u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64ca98; /* cmp/pz r0 */
L8c64ca98: *stop=0x8c64ca98u;r[1]+=r[15];goto L8c64ca9a; /* add r15,r1 */
L8c64ca9a: take=(*sr&1u);*stop=0x8c64ca9cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64cb04;goto L8c64ca9e; /* bt/s 0x8c64cb04 */
L8c64ca9e: *stop=0x8c64ca9eu;r[0]=sx(m,bus,&io_status,0x8c64cac6u,2);if(io_status)return io_status;goto L8c64caa0; /* mov.w 0x8c64cac6,r0 */
L8c64caa0: *stop=0x8c64caa0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64caa2; /* mov.l @(r0,r15),r2 */
L8c64caa2: *stop=0x8c64caa2u;r[0]=sx(m,bus,&io_status,0x8c64cad0u,2);if(io_status)return io_status;goto L8c64caa4; /* mov.w 0x8c64cad0,r0 */
L8c64caa4: *stop=0x8c64caa4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64caa6; /* mov.l @r2,r3 */
L8c64caa6: *stop=0x8c64caa6u;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64caa8; /* mov.w @(r0,r15),r1 */
L8c64caa8: *stop=0x8c64caa8u;r[0]=66u;goto L8c64caaa; /* mov #0x42,r0 */
L8c64caaa: *stop=0x8c64caaau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64caf8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64caac; /* mov.l 0x8c64caf8,r2 */
L8c64caac: *stop=0x8c64caacu;r[1]=0u-r[1];goto L8c64caae; /* neg r1,r1 */
L8c64caae: *stop=0x8c64caaeu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64cab0; /* mov.w r1,@(r0,r3) */
L8c64cab0: *stop=0x8c64cab0u;r[0]=sx(m,bus,&io_status,0x8c64cacau,2);if(io_status)return io_status;goto L8c64cab2; /* mov.w 0x8c64caca,r0 */
L8c64cab2: *stop=0x8c64cab2u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cab4; /* mov.l @(r0,r15),r3 */
L8c64cab4: *stop=0x8c64cab4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64cab6; /* mov.l r3,@r2 */
L8c64cab6: *stop=0x8c64cab6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cafcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cab8; /* mov.l 0x8c64cafc,r3 */
L8c64cab8: *stop=0x8c64cab8u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64caba; /* mov.l @(0x14,r15),r1 */
L8c64caba: *stop=0x8c64cabau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64cabc; /* mov.l r1,@r3 */
L8c64cabc: *stop=0x8c64cabcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cb00u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cabe; /* mov.l 0x8c64cb00,r1 */
L8c64cabe: target=r[1];*pr=0x8c64cac2u;*stop=0x8c64cac0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cac2; /* jsr @r1 */
L8c64cac2: *stop=0x8c64cac4u;goto L8c64d190; /* bra 0x8c64d190 */
L8c64cb04: *stop=0x8c64cb04u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cb06; /* mov.l @(0x10,r15),r2 */
L8c64cb06: *stop=0x8c64cb06u;r[7]=1u;goto L8c64cb08; /* mov #0x1,r7 */
L8c64cb08: *stop=0x8c64cb08u;r[0]=sx(m,bus,&io_status,0x8c64cc26u,2);if(io_status)return io_status;goto L8c64cb0a; /* mov.w 0x8c64cc26,r0 */
L8c64cb0a: *stop=0x8c64cb0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64cb0c; /* mov.l r2,@-r15 */
L8c64cb0c: *stop=0x8c64cb0cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cb0e; /* mov.l @(r0,r15),r5 */
L8c64cb0e: *stop=0x8c64cb0eu;r[0]=sx(m,bus,&io_status,0x8c64cc28u,2);if(io_status)return io_status;goto L8c64cb10; /* mov.w 0x8c64cc28,r0 */
L8c64cb10: *stop=0x8c64cb10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cb12; /* mov.l 0x8c64cc34,r3 */
L8c64cb12: *stop=0x8c64cb12u;r[5]+=20u;goto L8c64cb14; /* add #0x14,r5 */
L8c64cb14: *stop=0x8c64cb14u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cb16; /* mov.l @(0x18,r15),r6 */
L8c64cb16: target=r[3];*pr=0x8c64cb1au;*stop=0x8c64cb18u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cb1a; /* jsr @r3 */
L8c64cb1a: *stop=0x8c64cb1au;r[15]+=4u;goto L8c64cb1c; /* add #0x4,r15 */
L8c64cb1c: *stop=0x8c64cb1cu;r[0]=sx(m,bus,&io_status,0x8c64cc2au,2);if(io_status)return io_status;goto L8c64cb1e; /* mov.w 0x8c64cc2a,r0 */
L8c64cb1e: *stop=0x8c64cb1eu;r[1]=0u;goto L8c64cb20; /* mov #0x0,r1 */
L8c64cb20: *stop=0x8c64cb20u;r[5]=2u;goto L8c64cb22; /* mov #0x2,r5 */
L8c64cb22: *stop=0x8c64cb22u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cb24; /* mov.l @(r0,r15),r2 */
L8c64cb24: *stop=0x8c64cb24u;r[0]=66u;goto L8c64cb26; /* mov #0x42,r0 */
L8c64cb26: *stop=0x8c64cb26u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cb28; /* mov.l @r2,r3 */
L8c64cb28: *stop=0x8c64cb28u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64cb2a; /* mov.w r1,@(r0,r3) */
L8c64cb2a: *stop=0x8c64cb2au;r[0]=sx(m,bus,&io_status,0x8c64cc2au,2);if(io_status)return io_status;goto L8c64cb2c; /* mov.w 0x8c64cc2a,r0 */
L8c64cb2c: *stop=0x8c64cb2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cb2e; /* mov.l 0x8c64cc38,r3 */
L8c64cb2e: *stop=0x8c64cb2eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cb30; /* mov.l @(r0,r15),r6 */
L8c64cb30: *stop=0x8c64cb30u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cb32; /* mov.l @r6,r6 */
L8c64cb32: *stop=0x8c64cb32u;if(!valid_access(r[6]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cb34; /* mov.l @(0x14,r6),r6 */
L8c64cb34: target=r[3];*pr=0x8c64cb38u;*stop=0x8c64cb36u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cb38; /* jsr @r3 */
L8c64cb38: *stop=0x8c64cb3au;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64cb3c: *stop=0x8c64cb3cu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cb3e; /* mov.l @(0x10,r15),r1 */
L8c64cb3e: *stop=0x8c64cb3eu;r[7]=1u;goto L8c64cb40; /* mov #0x1,r7 */
L8c64cb40: *stop=0x8c64cb40u;r[0]=sx(m,bus,&io_status,0x8c64cc26u,2);if(io_status)return io_status;goto L8c64cb42; /* mov.w 0x8c64cc26,r0 */
L8c64cb42: *stop=0x8c64cb42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c64cb44; /* mov.l r1,@-r15 */
L8c64cb44: *stop=0x8c64cb44u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cb46; /* mov.l @(r0,r15),r5 */
L8c64cb46: *stop=0x8c64cb46u;r[0]=sx(m,bus,&io_status,0x8c64cc28u,2);if(io_status)return io_status;goto L8c64cb48; /* mov.w 0x8c64cc28,r0 */
L8c64cb48: *stop=0x8c64cb48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc34u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cb4a; /* mov.l 0x8c64cc34,r2 */
L8c64cb4a: *stop=0x8c64cb4au;r[5]+=20u;goto L8c64cb4c; /* add #0x14,r5 */
L8c64cb4c: *stop=0x8c64cb4cu;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cb4e; /* mov.l @(0x18,r15),r6 */
L8c64cb4e: target=r[2];*pr=0x8c64cb52u;*stop=0x8c64cb50u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cb52; /* jsr @r2 */
L8c64cb52: *stop=0x8c64cb54u;r[15]+=4u;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64cb56: *stop=0x8c64cb56u;r[0]=sx(m,bus,&io_status,0x8c64cc2cu,2);if(io_status)return io_status;goto L8c64cb58; /* mov.w 0x8c64cc2c,r0 */
L8c64cb58: *stop=0x8c64cb58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cb5a; /* mov.l 0x8c64cc3c,r2 */
L8c64cb5a: *stop=0x8c64cb5au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cb5c; /* mov.l @(r0,r15),r5 */
L8c64cb5c: *stop=0x8c64cb5cu;r[0]=sx(m,bus,&io_status,0x8c64cc2eu,2);if(io_status)return io_status;goto L8c64cb5e; /* mov.w 0x8c64cc2e,r0 */
L8c64cb5e: *stop=0x8c64cb5eu;r[5]+=20u;goto L8c64cb60; /* add #0x14,r5 */
L8c64cb60: target=r[2];*pr=0x8c64cb64u;*stop=0x8c64cb62u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cb64; /* jsr @r2 */
L8c64cb64: *stop=0x8c64cb64u;r[1]=sx(m,bus,&io_status,0x8c64cc30u,2);if(io_status)return io_status;goto L8c64cb66; /* mov.w 0x8c64cc30,r1 */
L8c64cb66: *stop=0x8c64cb66u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64cb68; /* cmp/pz r0 */
L8c64cb68: *stop=0x8c64cb68u;r[1]+=r[15];goto L8c64cb6a; /* add r15,r1 */
L8c64cb6a: take=(*sr&1u);*stop=0x8c64cb6cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64cb96;goto L8c64cb6e; /* bt/s 0x8c64cb96 */
L8c64cb6e: *stop=0x8c64cb6eu;r[0]=sx(m,bus,&io_status,0x8c64cc2au,2);if(io_status)return io_status;goto L8c64cb70; /* mov.w 0x8c64cc2a,r0 */
L8c64cb70: *stop=0x8c64cb70u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cb72; /* mov.l @(r0,r15),r2 */
L8c64cb72: *stop=0x8c64cb72u;r[0]=sx(m,bus,&io_status,0x8c64cc30u,2);if(io_status)return io_status;goto L8c64cb74; /* mov.w 0x8c64cc30,r0 */
L8c64cb74: *stop=0x8c64cb74u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cb76; /* mov.l @r2,r3 */
L8c64cb76: *stop=0x8c64cb76u;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64cb78; /* mov.w @(r0,r15),r1 */
L8c64cb78: *stop=0x8c64cb78u;r[0]=66u;goto L8c64cb7a; /* mov #0x42,r0 */
L8c64cb7a: *stop=0x8c64cb7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc40u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cb7c; /* mov.l 0x8c64cc40,r2 */
L8c64cb7c: *stop=0x8c64cb7cu;r[1]=0u-r[1];goto L8c64cb7e; /* neg r1,r1 */
L8c64cb7e: *stop=0x8c64cb7eu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64cb80; /* mov.w r1,@(r0,r3) */
L8c64cb80: *stop=0x8c64cb80u;r[0]=sx(m,bus,&io_status,0x8c64cc28u,2);if(io_status)return io_status;goto L8c64cb82; /* mov.w 0x8c64cc28,r0 */
L8c64cb82: *stop=0x8c64cb82u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cb84; /* mov.l @(r0,r15),r3 */
L8c64cb84: *stop=0x8c64cb84u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64cb86; /* mov.l r3,@r2 */
L8c64cb86: *stop=0x8c64cb86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cb88; /* mov.l 0x8c64cc44,r3 */
L8c64cb88: *stop=0x8c64cb88u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cb8a; /* mov.l @(0x14,r15),r1 */
L8c64cb8a: *stop=0x8c64cb8au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64cb8c; /* mov.l r1,@r3 */
L8c64cb8c: *stop=0x8c64cb8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc48u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cb8e; /* mov.l 0x8c64cc48,r1 */
L8c64cb8e: target=r[1];*pr=0x8c64cb92u;*stop=0x8c64cb90u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cb92; /* jsr @r1 */
L8c64cb92: *stop=0x8c64cb94u;goto L8c64d190; /* bra 0x8c64d190 */
L8c64cb96: *stop=0x8c64cb96u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cb98; /* mov.l @(0x10,r15),r2 */
L8c64cb98: *stop=0x8c64cb98u;r[7]=1u;goto L8c64cb9a; /* mov #0x1,r7 */
L8c64cb9a: *stop=0x8c64cb9au;r[0]=sx(m,bus,&io_status,0x8c64cc26u,2);if(io_status)return io_status;goto L8c64cb9c; /* mov.w 0x8c64cc26,r0 */
L8c64cb9c: *stop=0x8c64cb9cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64cb9e; /* mov.l r2,@-r15 */
L8c64cb9e: *stop=0x8c64cb9eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cba0; /* mov.l @(r0,r15),r5 */
L8c64cba0: *stop=0x8c64cba0u;r[0]=sx(m,bus,&io_status,0x8c64cc28u,2);if(io_status)return io_status;goto L8c64cba2; /* mov.w 0x8c64cc28,r0 */
L8c64cba2: *stop=0x8c64cba2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cba4; /* mov.l 0x8c64cc34,r3 */
L8c64cba4: *stop=0x8c64cba4u;r[5]+=20u;goto L8c64cba6; /* add #0x14,r5 */
L8c64cba6: *stop=0x8c64cba6u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cba8; /* mov.l @(0x18,r15),r6 */
L8c64cba8: target=r[3];*pr=0x8c64cbacu;*stop=0x8c64cbaau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cbac; /* jsr @r3 */
L8c64cbac: *stop=0x8c64cbacu;r[15]+=4u;goto L8c64cbae; /* add #0x4,r15 */
L8c64cbae: *stop=0x8c64cbaeu;r[0]=sx(m,bus,&io_status,0x8c64cc2au,2);if(io_status)return io_status;goto L8c64cbb0; /* mov.w 0x8c64cc2a,r0 */
L8c64cbb0: *stop=0x8c64cbb0u;r[1]=0u;goto L8c64cbb2; /* mov #0x0,r1 */
L8c64cbb2: *stop=0x8c64cbb2u;r[5]=2u;goto L8c64cbb4; /* mov #0x2,r5 */
L8c64cbb4: *stop=0x8c64cbb4u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cbb6; /* mov.l @(r0,r15),r2 */
L8c64cbb6: *stop=0x8c64cbb6u;r[0]=66u;goto L8c64cbb8; /* mov #0x42,r0 */
L8c64cbb8: *stop=0x8c64cbb8u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cbba; /* mov.l @r2,r3 */
L8c64cbba: *stop=0x8c64cbbau;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64cbbc; /* mov.w r1,@(r0,r3) */
L8c64cbbc: *stop=0x8c64cbbcu;r[0]=sx(m,bus,&io_status,0x8c64cc2au,2);if(io_status)return io_status;goto L8c64cbbe; /* mov.w 0x8c64cc2a,r0 */
L8c64cbbe: *stop=0x8c64cbbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cbc0; /* mov.l 0x8c64cc38,r3 */
L8c64cbc0: *stop=0x8c64cbc0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cbc2; /* mov.l @(r0,r15),r6 */
L8c64cbc2: *stop=0x8c64cbc2u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cbc4; /* mov.l @r6,r6 */
L8c64cbc4: *stop=0x8c64cbc4u;if(!valid_access(r[6]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cbc6; /* mov.l @(0x14,r6),r6 */
L8c64cbc6: target=r[3];*pr=0x8c64cbcau;*stop=0x8c64cbc8u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cbca; /* jsr @r3 */
L8c64cbca: *stop=0x8c64cbccu;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64cbce: *stop=0x8c64cbceu;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cbd0; /* mov.l @(0x20,r15),r2 */
L8c64cbd0: *stop=0x8c64cbd0u;r[0]=sx(m,bus,&io_status,0x8c64cc2au,2);if(io_status)return io_status;goto L8c64cbd2; /* mov.w 0x8c64cc2a,r0 */
L8c64cbd2: *stop=0x8c64cbd2u;r[1]=r[2];goto L8c64cbd4; /* mov r2,r1 */
L8c64cbd4: *stop=0x8c64cbd4u;r[2]<<=2;goto L8c64cbd6; /* shll2 r2 */
L8c64cbd6: *stop=0x8c64cbd6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cbd8; /* mov.l @(r0,r15),r3 */
L8c64cbd8: *stop=0x8c64cbd8u;r[2]<<=2;goto L8c64cbda; /* shll2 r2 */
L8c64cbda: *stop=0x8c64cbdau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc4cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cbdc; /* mov.l 0x8c64cc4c,r0 */
L8c64cbdc: *stop=0x8c64cbdcu;r[2]+=r[1];goto L8c64cbde; /* add r1,r2 */
L8c64cbde: *stop=0x8c64cbdeu;r[2]<<=2;goto L8c64cbe0; /* shll2 r2 */
L8c64cbe0: *stop=0x8c64cbe0u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cbe2; /* mov.l @r0,r1 */
L8c64cbe2: *stop=0x8c64cbe2u;r[0]=sx(m,bus,&io_status,0x8c64cc2eu,2);if(io_status)return io_status;goto L8c64cbe4; /* mov.w 0x8c64cc2e,r0 */
L8c64cbe4: *stop=0x8c64cbe4u;r[2]+=r[1];goto L8c64cbe6; /* add r1,r2 */
L8c64cbe6: *stop=0x8c64cbe6u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64cbe8; /* mov.l r2,@r3 */
L8c64cbe8: *stop=0x8c64cbe8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cbea; /* mov.l @(r0,r15),r0 */
L8c64cbea: *stop=0x8c64cbeau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc50u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cbec; /* mov.l 0x8c64cc50,r3 */
L8c64cbec: *stop=0x8c64cbecu;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cbee; /* mov.l @(0x4,r0),r0 */
L8c64cbee: *stop=0x8c64cbeeu;r[0]&=r[3];goto L8c64cbf0; /* and r3,r0 */
L8c64cbf0: *stop=0x8c64cbf0u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64cbf2; /* cmp/eq #0x0,r0 */
L8c64cbf2: if((*sr&1u))goto L8c64cc08;goto L8c64cbf4; /* bt 0x8c64cc08 */
L8c64cbf4: *stop=0x8c64cbf4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc54u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cbf6; /* mov.l 0x8c64cc54,r1 */
L8c64cbf6: *stop=0x8c64cbf6u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64cbf8; /* cmp/eq r1,r0 */
L8c64cbf8: if((*sr&1u))goto L8c64ccee;goto L8c64cbfa; /* bt 0x8c64ccee */
L8c64cbfa: *stop=0x8c64cbfau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cc58u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cbfc; /* mov.l 0x8c64cc58,r1 */
L8c64cbfc: *stop=0x8c64cbfcu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64cbfe; /* cmp/eq r1,r0 */
L8c64cbfe: if(!(*sr&1u))goto L8c64cc04;goto L8c64cc00; /* bf 0x8c64cc04 */
L8c64cc00: *stop=0x8c64cc02u;goto L8c64cd80; /* bra 0x8c64cd80 */
L8c64cc04: *stop=0x8c64cc06u;goto L8c64ce80; /* bra 0x8c64ce80 */
L8c64cc08: *stop=0x8c64cc08u;r[0]=sx(m,bus,&io_status,0x8c64cc2au,2);if(io_status)return io_status;goto L8c64cc0a; /* mov.w 0x8c64cc2a,r0 */
L8c64cc0a: *stop=0x8c64cc0au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cc0c; /* mov.l @(r0,r15),r0 */
L8c64cc0c: *stop=0x8c64cc0cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cc0e; /* mov.l @r0,r0 */
L8c64cc0e: *stop=0x8c64cc0eu;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cc10; /* mov.l @(0x10,r0),r0 */
L8c64cc10: *stop=0x8c64cc10u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c64cc12; /* tst #0x2,r0 */
L8c64cc12: if((*sr&1u))goto L8c64cc5c;goto L8c64cc14; /* bt 0x8c64cc5c */
L8c64cc14: *stop=0x8c64cc14u;r[0]=sx(m,bus,&io_status,0x8c64cc2au,2);if(io_status)return io_status;goto L8c64cc16; /* mov.w 0x8c64cc2a,r0 */
L8c64cc16: *stop=0x8c64cc16u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cc18; /* mov.l @(r0,r15),r1 */
L8c64cc18: *stop=0x8c64cc18u;r[0]=64u;goto L8c64cc1a; /* mov #0x40,r0 */
L8c64cc1a: *stop=0x8c64cc1au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cc1c; /* mov.l @r1,r2 */
L8c64cc1c: *stop=0x8c64cc1cu;if(!valid_access(r[0]+r[2],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[2],2);if(io_status)return io_status;goto L8c64cc1e; /* mov.w @(r0,r2),r1 */
L8c64cc1e: *stop=0x8c64cc1eu;r[1]+=1u;goto L8c64cc20; /* add #0x1,r1 */
L8c64cc20: *stop=0x8c64cc20u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64cc22; /* mov.w r1,@(r0,r2) */
L8c64cc22: *stop=0x8c64cc24u;goto L8c64ce80; /* bra 0x8c64ce80 */
L8c64cc5c: *stop=0x8c64cc5cu;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cc5e; /* mov.l @(0x20,r15),r5 */
L8c64cc5e: *stop=0x8c64cc5eu;r[0]=sx(m,bus,&io_status,0x8c64cd5au,2);if(io_status)return io_status;goto L8c64cc60; /* mov.w 0x8c64cd5a,r0 */
L8c64cc60: *stop=0x8c64cc60u;r[2]=r[5];goto L8c64cc62; /* mov r5,r2 */
L8c64cc62: *stop=0x8c64cc62u;r[5]<<=2;goto L8c64cc64; /* shll2 r5 */
L8c64cc64: *stop=0x8c64cc64u;r[5]<<=2;goto L8c64cc66; /* shll2 r5 */
L8c64cc66: *stop=0x8c64cc66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cd68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cc68; /* mov.l 0x8c64cd68,r3 */
L8c64cc68: *stop=0x8c64cc68u;r[5]+=r[2];goto L8c64cc6a; /* add r2,r5 */
L8c64cc6a: *stop=0x8c64cc6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cd64u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cc6c; /* mov.l 0x8c64cd64,r2 */
L8c64cc6c: *stop=0x8c64cc6cu;r[5]<<=2;goto L8c64cc6e; /* shll2 r5 */
L8c64cc6e: *stop=0x8c64cc6eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cc70; /* mov.l @r2,r1 */
L8c64cc70: *stop=0x8c64cc70u;r[5]+=r[1];goto L8c64cc72; /* add r1,r5 */
L8c64cc72: *stop=0x8c64cc72u;r[5]+=20u;goto L8c64cc74; /* add #0x14,r5 */
L8c64cc74: target=r[3];*pr=0x8c64cc78u;*stop=0x8c64cc76u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cc78; /* jsr @r3 */
L8c64cc78: *stop=0x8c64cc78u;r[1]=sx(m,bus,&io_status,0x8c64cd5cu,2);if(io_status)return io_status;goto L8c64cc7a; /* mov.w 0x8c64cd5c,r1 */
L8c64cc7a: *stop=0x8c64cc7au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64cc7c; /* cmp/pz r0 */
L8c64cc7c: *stop=0x8c64cc7cu;r[1]+=r[15];goto L8c64cc7e; /* add r15,r1 */
L8c64cc7e: take=(*sr&1u);*stop=0x8c64cc80u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64ccaa;goto L8c64cc82; /* bt/s 0x8c64ccaa */
L8c64cc82: *stop=0x8c64cc82u;r[0]=sx(m,bus,&io_status,0x8c64cd5eu,2);if(io_status)return io_status;goto L8c64cc84; /* mov.w 0x8c64cd5e,r0 */
L8c64cc84: *stop=0x8c64cc84u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cc86; /* mov.l @(r0,r15),r2 */
L8c64cc86: *stop=0x8c64cc86u;r[0]=sx(m,bus,&io_status,0x8c64cd5cu,2);if(io_status)return io_status;goto L8c64cc88; /* mov.w 0x8c64cd5c,r0 */
L8c64cc88: *stop=0x8c64cc88u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cc8a; /* mov.l @r2,r3 */
L8c64cc8a: *stop=0x8c64cc8au;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64cc8c; /* mov.w @(r0,r15),r1 */
L8c64cc8c: *stop=0x8c64cc8cu;r[0]=66u;goto L8c64cc8e; /* mov #0x42,r0 */
L8c64cc8e: *stop=0x8c64cc8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cd6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cc90; /* mov.l 0x8c64cd6c,r2 */
L8c64cc90: *stop=0x8c64cc90u;r[1]=0u-r[1];goto L8c64cc92; /* neg r1,r1 */
L8c64cc92: *stop=0x8c64cc92u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64cc94; /* mov.w r1,@(r0,r3) */
L8c64cc94: *stop=0x8c64cc94u;r[0]=sx(m,bus,&io_status,0x8c64cd60u,2);if(io_status)return io_status;goto L8c64cc96; /* mov.w 0x8c64cd60,r0 */
L8c64cc96: *stop=0x8c64cc96u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cc98; /* mov.l @(r0,r15),r3 */
L8c64cc98: *stop=0x8c64cc98u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64cc9a; /* mov.l r3,@r2 */
L8c64cc9a: *stop=0x8c64cc9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cd70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cc9c; /* mov.l 0x8c64cd70,r3 */
L8c64cc9c: *stop=0x8c64cc9cu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cc9e; /* mov.l @(0x14,r15),r1 */
L8c64cc9e: *stop=0x8c64cc9eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64cca0; /* mov.l r1,@r3 */
L8c64cca0: *stop=0x8c64cca0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cd74u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cca2; /* mov.l 0x8c64cd74,r1 */
L8c64cca2: target=r[1];*pr=0x8c64cca6u;*stop=0x8c64cca4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cca6; /* jsr @r1 */
L8c64cca6: *stop=0x8c64cca8u;goto L8c64d190; /* bra 0x8c64d190 */
L8c64ccaa: *stop=0x8c64ccaau;r[0]=sx(m,bus,&io_status,0x8c64cd5eu,2);if(io_status)return io_status;goto L8c64ccac; /* mov.w 0x8c64cd5e,r0 */
L8c64ccac: *stop=0x8c64ccacu;r[1]=0u;goto L8c64ccae; /* mov #0x0,r1 */
L8c64ccae: *stop=0x8c64ccaeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ccb0; /* mov.l @(r0,r15),r2 */
L8c64ccb0: *stop=0x8c64ccb0u;r[0]=66u;goto L8c64ccb2; /* mov #0x42,r0 */
L8c64ccb2: *stop=0x8c64ccb2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ccb4; /* mov.l @r2,r3 */
L8c64ccb4: *stop=0x8c64ccb4u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64ccb6; /* mov.w r1,@(r0,r3) */
L8c64ccb6: *stop=0x8c64ccb6u;r[0]=sx(m,bus,&io_status,0x8c64cd5eu,2);if(io_status)return io_status;goto L8c64ccb8; /* mov.w 0x8c64cd5e,r0 */
L8c64ccb8: *stop=0x8c64ccb8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ccba; /* mov.l @(r0,r15),r3 */
L8c64ccba: *stop=0x8c64ccbau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ccbc; /* mov.l @r3,r3 */
L8c64ccbc: *stop=0x8c64ccbcu;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ccbe; /* mov.l @(0x10,r3),r0 */
L8c64ccbe: *stop=0x8c64ccbeu;r[0]|=0x2u;goto L8c64ccc0; /* or #0x2,r0 */
L8c64ccc0: *stop=0x8c64ccc0u;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[0],4);if(io_status)return io_status;goto L8c64ccc2; /* mov.l r0,@(0x10,r3) */
L8c64ccc2: *stop=0x8c64ccc2u;r[0]=sx(m,bus,&io_status,0x8c64cd5eu,2);if(io_status)return io_status;goto L8c64ccc4; /* mov.w 0x8c64cd5e,r0 */
L8c64ccc4: *stop=0x8c64ccc4u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ccc6; /* mov.l @(r0,r15),r2 */
L8c64ccc6: *stop=0x8c64ccc6u;r[0]=64u;goto L8c64ccc8; /* mov #0x40,r0 */
L8c64ccc8: *stop=0x8c64ccc8u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ccca; /* mov.l @r2,r3 */
L8c64ccca: *stop=0x8c64cccau;if(!valid_access(r[0]+r[3],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[3],2);if(io_status)return io_status;goto L8c64cccc; /* mov.w @(r0,r3),r2 */
L8c64cccc: *stop=0x8c64ccccu;r[2]+=1u;goto L8c64ccce; /* add #0x1,r2 */
L8c64ccce: *stop=0x8c64ccceu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[2],2);if(io_status)return io_status;goto L8c64ccd0; /* mov.w r2,@(r0,r3) */
L8c64ccd0: *stop=0x8c64ccd0u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ccd2; /* mov.l @(0x18,r15),r3 */
L8c64ccd2: *stop=0x8c64ccd2u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64ccd4; /* tst r3,r3 */
L8c64ccd4: if(!(*sr&1u))goto L8c64ccda;goto L8c64ccd6; /* bf 0x8c64ccda */
L8c64ccd6: *stop=0x8c64ccd8u;goto L8c64ce80; /* bra 0x8c64ce80 */
L8c64ccda: *stop=0x8c64ccdau;r[0]=sx(m,bus,&io_status,0x8c64cd5eu,2);if(io_status)return io_status;goto L8c64ccdc; /* mov.w 0x8c64cd5e,r0 */
L8c64ccdc: *stop=0x8c64ccdcu;r[5]=2u;goto L8c64ccde; /* mov #0x2,r5 */
L8c64ccde: *stop=0x8c64ccdeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cd78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cce0; /* mov.l 0x8c64cd78,r3 */
L8c64cce0: *stop=0x8c64cce0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cce2; /* mov.l @(r0,r15),r6 */
L8c64cce2: *stop=0x8c64cce2u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cce4; /* mov.l @r6,r6 */
L8c64cce4: *stop=0x8c64cce4u;if(!valid_access(r[6]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cce6; /* mov.l @(0x14,r6),r6 */
L8c64cce6: target=r[3];*pr=0x8c64cceau;*stop=0x8c64cce8u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ccea; /* jsr @r3 */
L8c64ccea: *stop=0x8c64ccecu;goto L8c64ce80; /* bra 0x8c64ce80 */
L8c64ccee: *stop=0x8c64cceeu;r[0]=sx(m,bus,&io_status,0x8c64cd5eu,2);if(io_status)return io_status;goto L8c64ccf0; /* mov.w 0x8c64cd5e,r0 */
L8c64ccf0: *stop=0x8c64ccf0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ccf2; /* mov.l @(r0,r15),r0 */
L8c64ccf2: *stop=0x8c64ccf2u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ccf4; /* mov.l @r0,r0 */
L8c64ccf4: *stop=0x8c64ccf4u;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ccf6; /* mov.l @(0x10,r0),r0 */
L8c64ccf6: *stop=0x8c64ccf6u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c64ccf8; /* tst #0x1,r0 */
L8c64ccf8: if((*sr&1u))goto L8c64ccfe;goto L8c64ccfa; /* bt 0x8c64ccfe */
L8c64ccfa: *stop=0x8c64ccfcu;goto L8c64ce80; /* bra 0x8c64ce80 */
L8c64ccfe: *stop=0x8c64ccfeu;r[0]=sx(m,bus,&io_status,0x8c64cd5eu,2);if(io_status)return io_status;goto L8c64cd00; /* mov.w 0x8c64cd5e,r0 */
L8c64cd00: *stop=0x8c64cd00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cd7cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cd02; /* mov.l 0x8c64cd7c,r2 */
L8c64cd02: *stop=0x8c64cd02u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cd04; /* mov.l @(r0,r15),r5 */
L8c64cd04: *stop=0x8c64cd04u;r[0]=sx(m,bus,&io_status,0x8c64cd5au,2);if(io_status)return io_status;goto L8c64cd06; /* mov.w 0x8c64cd5a,r0 */
L8c64cd06: *stop=0x8c64cd06u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cd08; /* mov.l @r5,r5 */
L8c64cd08: *stop=0x8c64cd08u;r[5]+=20u;goto L8c64cd0a; /* add #0x14,r5 */
L8c64cd0a: target=r[2];*pr=0x8c64cd0eu;*stop=0x8c64cd0cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cd0e; /* jsr @r2 */
L8c64cd0e: *stop=0x8c64cd0eu;r[1]=sx(m,bus,&io_status,0x8c64cd5cu,2);if(io_status)return io_status;goto L8c64cd10; /* mov.w 0x8c64cd5c,r1 */
L8c64cd10: *stop=0x8c64cd10u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64cd12; /* cmp/pz r0 */
L8c64cd12: *stop=0x8c64cd12u;r[1]+=r[15];goto L8c64cd14; /* add r15,r1 */
L8c64cd14: take=(*sr&1u);*stop=0x8c64cd16u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64cd40;goto L8c64cd18; /* bt/s 0x8c64cd40 */
L8c64cd18: *stop=0x8c64cd18u;r[0]=sx(m,bus,&io_status,0x8c64cd5eu,2);if(io_status)return io_status;goto L8c64cd1a; /* mov.w 0x8c64cd5e,r0 */
L8c64cd1a: *stop=0x8c64cd1au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cd1c; /* mov.l @(r0,r15),r2 */
L8c64cd1c: *stop=0x8c64cd1cu;r[0]=sx(m,bus,&io_status,0x8c64cd5cu,2);if(io_status)return io_status;goto L8c64cd1e; /* mov.w 0x8c64cd5c,r0 */
L8c64cd1e: *stop=0x8c64cd1eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cd20; /* mov.l @r2,r3 */
L8c64cd20: *stop=0x8c64cd20u;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64cd22; /* mov.w @(r0,r15),r1 */
L8c64cd22: *stop=0x8c64cd22u;r[0]=66u;goto L8c64cd24; /* mov #0x42,r0 */
L8c64cd24: *stop=0x8c64cd24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cd6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cd26; /* mov.l 0x8c64cd6c,r2 */
L8c64cd26: *stop=0x8c64cd26u;r[1]=0u-r[1];goto L8c64cd28; /* neg r1,r1 */
L8c64cd28: *stop=0x8c64cd28u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64cd2a; /* mov.w r1,@(r0,r3) */
L8c64cd2a: *stop=0x8c64cd2au;r[0]=sx(m,bus,&io_status,0x8c64cd60u,2);if(io_status)return io_status;goto L8c64cd2c; /* mov.w 0x8c64cd60,r0 */
L8c64cd2c: *stop=0x8c64cd2cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cd2e; /* mov.l @(r0,r15),r3 */
L8c64cd2e: *stop=0x8c64cd2eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64cd30; /* mov.l r3,@r2 */
L8c64cd30: *stop=0x8c64cd30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cd70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cd32; /* mov.l 0x8c64cd70,r3 */
L8c64cd32: *stop=0x8c64cd32u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cd34; /* mov.l @(0x14,r15),r1 */
L8c64cd34: *stop=0x8c64cd34u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64cd36; /* mov.l r1,@r3 */
L8c64cd36: *stop=0x8c64cd36u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cd74u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cd38; /* mov.l 0x8c64cd74,r1 */
L8c64cd38: target=r[1];*pr=0x8c64cd3cu;*stop=0x8c64cd3au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cd3c; /* jsr @r1 */
L8c64cd3c: *stop=0x8c64cd3eu;goto L8c64d190; /* bra 0x8c64d190 */
L8c64cd40: *stop=0x8c64cd40u;r[0]=sx(m,bus,&io_status,0x8c64cd5eu,2);if(io_status)return io_status;goto L8c64cd42; /* mov.w 0x8c64cd5e,r0 */
L8c64cd42: *stop=0x8c64cd42u;r[1]=0u;goto L8c64cd44; /* mov #0x0,r1 */
L8c64cd44: *stop=0x8c64cd44u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cd46; /* mov.l @(r0,r15),r2 */
L8c64cd46: *stop=0x8c64cd46u;r[0]=66u;goto L8c64cd48; /* mov #0x42,r0 */
L8c64cd48: *stop=0x8c64cd48u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cd4a; /* mov.l @r2,r3 */
L8c64cd4a: *stop=0x8c64cd4au;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64cd4c; /* mov.w r1,@(r0,r3) */
L8c64cd4c: *stop=0x8c64cd4cu;r[0]=sx(m,bus,&io_status,0x8c64cd5eu,2);if(io_status)return io_status;goto L8c64cd4e; /* mov.w 0x8c64cd5e,r0 */
L8c64cd4e: *stop=0x8c64cd4eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cd50; /* mov.l @(r0,r15),r3 */
L8c64cd50: *stop=0x8c64cd50u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cd52; /* mov.l @r3,r3 */
L8c64cd52: *stop=0x8c64cd52u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cd54; /* mov.l @(0x10,r3),r0 */
L8c64cd54: *stop=0x8c64cd54u;r[0]|=0x1u;goto L8c64cd56; /* or #0x1,r0 */
L8c64cd56: *stop=0x8c64cd58u;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[0],4);if(io_status)return io_status;goto L8c64ce80; /* bra 0x8c64ce80 */
L8c64cd80: *stop=0x8c64cd80u;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64cd82; /* mov.w 0x8c64ce8a,r0 */
L8c64cd82: *stop=0x8c64cd82u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cd84; /* mov.l @(r0,r15),r0 */
L8c64cd84: *stop=0x8c64cd84u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cd86; /* mov.l @r0,r0 */
L8c64cd86: *stop=0x8c64cd86u;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cd88; /* mov.l @(0x10,r0),r0 */
L8c64cd88: *stop=0x8c64cd88u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c64cd8a; /* tst #0x2,r0 */
L8c64cd8a: if((*sr&1u))goto L8c64ce02;goto L8c64cd8c; /* bt 0x8c64ce02 */
L8c64cd8c: *stop=0x8c64cd8cu;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64cd8e; /* mov.w 0x8c64ce8a,r0 */
L8c64cd8e: *stop=0x8c64cd8eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cd90; /* mov.l @(r0,r15),r1 */
L8c64cd90: *stop=0x8c64cd90u;r[0]=64u;goto L8c64cd92; /* mov #0x40,r0 */
L8c64cd92: *stop=0x8c64cd92u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cd94; /* mov.l @r1,r2 */
L8c64cd94: *stop=0x8c64cd94u;if(!valid_access(r[0]+r[2],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[2],2);if(io_status)return io_status;goto L8c64cd96; /* mov.w @(r0,r2),r1 */
L8c64cd96: *stop=0x8c64cd96u;r[1]+=1u;goto L8c64cd98; /* add #0x1,r1 */
L8c64cd98: *stop=0x8c64cd98u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64cd9a; /* mov.w r1,@(r0,r2) */
L8c64cd9a: *stop=0x8c64cd9au;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64cd9c; /* mov.w 0x8c64ce8a,r0 */
L8c64cd9c: *stop=0x8c64cd9cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cd9e; /* mov.l @(r0,r15),r0 */
L8c64cd9e: *stop=0x8c64cd9eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cda0; /* mov.l @r0,r0 */
L8c64cda0: *stop=0x8c64cda0u;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cda2; /* mov.l @(0x10,r0),r0 */
L8c64cda2: *stop=0x8c64cda2u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c64cda4; /* tst #0x1,r0 */
L8c64cda4: if(!(*sr&1u))goto L8c64ce80;goto L8c64cda6; /* bf 0x8c64ce80 */
L8c64cda6: *stop=0x8c64cda6u;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64cda8; /* mov.w 0x8c64ce8a,r0 */
L8c64cda8: *stop=0x8c64cda8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ce94u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cdaa; /* mov.l 0x8c64ce94,r2 */
L8c64cdaa: *stop=0x8c64cdaau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cdac; /* mov.l @(r0,r15),r5 */
L8c64cdac: *stop=0x8c64cdacu;r[0]=sx(m,bus,&io_status,0x8c64ce8cu,2);if(io_status)return io_status;goto L8c64cdae; /* mov.w 0x8c64ce8c,r0 */
L8c64cdae: *stop=0x8c64cdaeu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cdb0; /* mov.l @r5,r5 */
L8c64cdb0: *stop=0x8c64cdb0u;r[5]+=20u;goto L8c64cdb2; /* add #0x14,r5 */
L8c64cdb2: target=r[2];*pr=0x8c64cdb6u;*stop=0x8c64cdb4u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cdb6; /* jsr @r2 */
L8c64cdb6: *stop=0x8c64cdb6u;r[1]=sx(m,bus,&io_status,0x8c64ce8eu,2);if(io_status)return io_status;goto L8c64cdb8; /* mov.w 0x8c64ce8e,r1 */
L8c64cdb8: *stop=0x8c64cdb8u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64cdba; /* cmp/pz r0 */
L8c64cdba: *stop=0x8c64cdbau;r[1]+=r[15];goto L8c64cdbc; /* add r15,r1 */
L8c64cdbc: take=(*sr&1u);*stop=0x8c64cdbeu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64cde8;goto L8c64cdc0; /* bt/s 0x8c64cde8 */
L8c64cdc0: *stop=0x8c64cdc0u;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64cdc2; /* mov.w 0x8c64ce8a,r0 */
L8c64cdc2: *stop=0x8c64cdc2u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cdc4; /* mov.l @(r0,r15),r2 */
L8c64cdc4: *stop=0x8c64cdc4u;r[0]=sx(m,bus,&io_status,0x8c64ce8eu,2);if(io_status)return io_status;goto L8c64cdc6; /* mov.w 0x8c64ce8e,r0 */
L8c64cdc6: *stop=0x8c64cdc6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cdc8; /* mov.l @r2,r3 */
L8c64cdc8: *stop=0x8c64cdc8u;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64cdca; /* mov.w @(r0,r15),r1 */
L8c64cdca: *stop=0x8c64cdcau;r[0]=66u;goto L8c64cdcc; /* mov #0x42,r0 */
L8c64cdcc: *stop=0x8c64cdccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ce98u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cdce; /* mov.l 0x8c64ce98,r2 */
L8c64cdce: *stop=0x8c64cdceu;r[1]=0u-r[1];goto L8c64cdd0; /* neg r1,r1 */
L8c64cdd0: *stop=0x8c64cdd0u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64cdd2; /* mov.w r1,@(r0,r3) */
L8c64cdd2: *stop=0x8c64cdd2u;r[0]=sx(m,bus,&io_status,0x8c64ce90u,2);if(io_status)return io_status;goto L8c64cdd4; /* mov.w 0x8c64ce90,r0 */
L8c64cdd4: *stop=0x8c64cdd4u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cdd6; /* mov.l @(r0,r15),r3 */
L8c64cdd6: *stop=0x8c64cdd6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64cdd8; /* mov.l r3,@r2 */
L8c64cdd8: *stop=0x8c64cdd8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ce9cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cdda; /* mov.l 0x8c64ce9c,r3 */
L8c64cdda: *stop=0x8c64cddau;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cddc; /* mov.l @(0x14,r15),r1 */
L8c64cddc: *stop=0x8c64cddcu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64cdde; /* mov.l r1,@r3 */
L8c64cdde: *stop=0x8c64cddeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cea0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cde0; /* mov.l 0x8c64cea0,r1 */
L8c64cde0: target=r[1];*pr=0x8c64cde4u;*stop=0x8c64cde2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cde4; /* jsr @r1 */
L8c64cde4: *stop=0x8c64cde6u;goto L8c64d190; /* bra 0x8c64d190 */
L8c64cde8: *stop=0x8c64cde8u;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64cdea; /* mov.w 0x8c64ce8a,r0 */
L8c64cdea: *stop=0x8c64cdeau;r[1]=0u;goto L8c64cdec; /* mov #0x0,r1 */
L8c64cdec: *stop=0x8c64cdecu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cdee; /* mov.l @(r0,r15),r2 */
L8c64cdee: *stop=0x8c64cdeeu;r[0]=66u;goto L8c64cdf0; /* mov #0x42,r0 */
L8c64cdf0: *stop=0x8c64cdf0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cdf2; /* mov.l @r2,r3 */
L8c64cdf2: *stop=0x8c64cdf2u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64cdf4; /* mov.w r1,@(r0,r3) */
L8c64cdf4: *stop=0x8c64cdf4u;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64cdf6; /* mov.w 0x8c64ce8a,r0 */
L8c64cdf6: *stop=0x8c64cdf6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cdf8; /* mov.l @(r0,r15),r3 */
L8c64cdf8: *stop=0x8c64cdf8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cdfa; /* mov.l @r3,r3 */
L8c64cdfa: *stop=0x8c64cdfau;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64cdfc; /* mov.l @(0x10,r3),r0 */
L8c64cdfc: *stop=0x8c64cdfcu;r[0]|=0x1u;goto L8c64cdfe; /* or #0x1,r0 */
L8c64cdfe: *stop=0x8c64ce00u;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[0],4);if(io_status)return io_status;goto L8c64ce80; /* bra 0x8c64ce80 */
L8c64ce02: *stop=0x8c64ce02u;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64ce04; /* mov.w 0x8c64ce8a,r0 */
L8c64ce04: *stop=0x8c64ce04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cea4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ce06; /* mov.l 0x8c64cea4,r2 */
L8c64ce06: *stop=0x8c64ce06u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ce08; /* mov.l @(r0,r15),r5 */
L8c64ce08: *stop=0x8c64ce08u;r[0]=sx(m,bus,&io_status,0x8c64ce8cu,2);if(io_status)return io_status;goto L8c64ce0a; /* mov.w 0x8c64ce8c,r0 */
L8c64ce0a: *stop=0x8c64ce0au;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ce0c; /* mov.l @r5,r5 */
L8c64ce0c: *stop=0x8c64ce0cu;r[5]+=20u;goto L8c64ce0e; /* add #0x14,r5 */
L8c64ce0e: target=r[2];*pr=0x8c64ce12u;*stop=0x8c64ce10u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ce12; /* jsr @r2 */
L8c64ce12: *stop=0x8c64ce12u;r[1]=sx(m,bus,&io_status,0x8c64ce8eu,2);if(io_status)return io_status;goto L8c64ce14; /* mov.w 0x8c64ce8e,r1 */
L8c64ce14: *stop=0x8c64ce14u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64ce16; /* cmp/pz r0 */
L8c64ce16: *stop=0x8c64ce16u;r[1]+=r[15];goto L8c64ce18; /* add r15,r1 */
L8c64ce18: take=(*sr&1u);*stop=0x8c64ce1au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64ce44;goto L8c64ce1c; /* bt/s 0x8c64ce44 */
L8c64ce1c: *stop=0x8c64ce1cu;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64ce1e; /* mov.w 0x8c64ce8a,r0 */
L8c64ce1e: *stop=0x8c64ce1eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ce20; /* mov.l @(r0,r15),r2 */
L8c64ce20: *stop=0x8c64ce20u;r[0]=sx(m,bus,&io_status,0x8c64ce8eu,2);if(io_status)return io_status;goto L8c64ce22; /* mov.w 0x8c64ce8e,r0 */
L8c64ce22: *stop=0x8c64ce22u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ce24; /* mov.l @r2,r3 */
L8c64ce24: *stop=0x8c64ce24u;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64ce26; /* mov.w @(r0,r15),r1 */
L8c64ce26: *stop=0x8c64ce26u;r[0]=66u;goto L8c64ce28; /* mov #0x42,r0 */
L8c64ce28: *stop=0x8c64ce28u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ce98u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ce2a; /* mov.l 0x8c64ce98,r2 */
L8c64ce2a: *stop=0x8c64ce2au;r[1]=0u-r[1];goto L8c64ce2c; /* neg r1,r1 */
L8c64ce2c: *stop=0x8c64ce2cu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64ce2e; /* mov.w r1,@(r0,r3) */
L8c64ce2e: *stop=0x8c64ce2eu;r[0]=sx(m,bus,&io_status,0x8c64ce90u,2);if(io_status)return io_status;goto L8c64ce30; /* mov.w 0x8c64ce90,r0 */
L8c64ce30: *stop=0x8c64ce30u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ce32; /* mov.l @(r0,r15),r3 */
L8c64ce32: *stop=0x8c64ce32u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64ce34; /* mov.l r3,@r2 */
L8c64ce34: *stop=0x8c64ce34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ce9cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ce36; /* mov.l 0x8c64ce9c,r3 */
L8c64ce36: *stop=0x8c64ce36u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ce38; /* mov.l @(0x14,r15),r1 */
L8c64ce38: *stop=0x8c64ce38u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64ce3a; /* mov.l r1,@r3 */
L8c64ce3a: *stop=0x8c64ce3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cea0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64ce3c; /* mov.l 0x8c64cea0,r1 */
L8c64ce3c: target=r[1];*pr=0x8c64ce40u;*stop=0x8c64ce3eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ce40; /* jsr @r1 */
L8c64ce40: *stop=0x8c64ce42u;goto L8c64d190; /* bra 0x8c64d190 */
L8c64ce44: *stop=0x8c64ce44u;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64ce46; /* mov.w 0x8c64ce8a,r0 */
L8c64ce46: *stop=0x8c64ce46u;r[1]=0u;goto L8c64ce48; /* mov #0x0,r1 */
L8c64ce48: *stop=0x8c64ce48u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ce4a; /* mov.l @(r0,r15),r2 */
L8c64ce4a: *stop=0x8c64ce4au;r[0]=66u;goto L8c64ce4c; /* mov #0x42,r0 */
L8c64ce4c: *stop=0x8c64ce4cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ce4e; /* mov.l @r2,r3 */
L8c64ce4e: *stop=0x8c64ce4eu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64ce50; /* mov.w r1,@(r0,r3) */
L8c64ce50: *stop=0x8c64ce50u;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64ce52; /* mov.w 0x8c64ce8a,r0 */
L8c64ce52: *stop=0x8c64ce52u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ce54; /* mov.l @(r0,r15),r3 */
L8c64ce54: *stop=0x8c64ce54u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ce56; /* mov.l @r3,r3 */
L8c64ce56: *stop=0x8c64ce56u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ce58; /* mov.l @(0x10,r3),r0 */
L8c64ce58: *stop=0x8c64ce58u;r[0]|=0x2u;goto L8c64ce5a; /* or #0x2,r0 */
L8c64ce5a: *stop=0x8c64ce5au;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[0],4);if(io_status)return io_status;goto L8c64ce5c; /* mov.l r0,@(0x10,r3) */
L8c64ce5c: *stop=0x8c64ce5cu;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64ce5e; /* mov.w 0x8c64ce8a,r0 */
L8c64ce5e: *stop=0x8c64ce5eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ce60; /* mov.l @(r0,r15),r2 */
L8c64ce60: *stop=0x8c64ce60u;r[0]=64u;goto L8c64ce62; /* mov #0x40,r0 */
L8c64ce62: *stop=0x8c64ce62u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ce64; /* mov.l @r2,r3 */
L8c64ce64: *stop=0x8c64ce64u;if(!valid_access(r[0]+r[3],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[3],2);if(io_status)return io_status;goto L8c64ce66; /* mov.w @(r0,r3),r2 */
L8c64ce66: *stop=0x8c64ce66u;r[2]+=1u;goto L8c64ce68; /* add #0x1,r2 */
L8c64ce68: *stop=0x8c64ce68u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[2],2);if(io_status)return io_status;goto L8c64ce6a; /* mov.w r2,@(r0,r3) */
L8c64ce6a: *stop=0x8c64ce6au;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ce6c; /* mov.l @(0x18,r15),r3 */
L8c64ce6c: *stop=0x8c64ce6cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64ce6e; /* tst r3,r3 */
L8c64ce6e: if((*sr&1u))goto L8c64ce80;goto L8c64ce70; /* bt 0x8c64ce80 */
L8c64ce70: *stop=0x8c64ce70u;r[0]=sx(m,bus,&io_status,0x8c64ce8au,2);if(io_status)return io_status;goto L8c64ce72; /* mov.w 0x8c64ce8a,r0 */
L8c64ce72: *stop=0x8c64ce72u;r[5]=2u;goto L8c64ce74; /* mov #0x2,r5 */
L8c64ce74: *stop=0x8c64ce74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cea8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ce76; /* mov.l 0x8c64cea8,r3 */
L8c64ce76: *stop=0x8c64ce76u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64ce78; /* mov.l @(r0,r15),r6 */
L8c64ce78: *stop=0x8c64ce78u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64ce7a; /* mov.l @r6,r6 */
L8c64ce7a: *stop=0x8c64ce7au;if(!valid_access(r[6]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64ce7c; /* mov.l @(0x14,r6),r6 */
L8c64ce7c: target=r[3];*pr=0x8c64ce80u;*stop=0x8c64ce7eu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ce80; /* jsr @r3 */
L8c64ce80: *stop=0x8c64ce80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cea0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ce82; /* mov.l 0x8c64cea0,r2 */
L8c64ce82: target=r[2];*pr=0x8c64ce86u;*stop=0x8c64ce84u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ce86; /* jsr @r2 */
L8c64ce86: *stop=0x8c64ce88u;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64ceac: *stop=0x8c64ceacu;r[0]=sx(m,bus,&io_status,0x8c64cf7cu,2);if(io_status)return io_status;goto L8c64ceae; /* mov.w 0x8c64cf7c,r0 */
L8c64ceae: *stop=0x8c64ceaeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cf84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ceb0; /* mov.l 0x8c64cf84,r3 */
L8c64ceb0: *stop=0x8c64ceb0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ceb2; /* mov.l @(r0,r15),r0 */
L8c64ceb2: *stop=0x8c64ceb2u;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ceb4; /* mov.l @(0x4,r0),r0 */
L8c64ceb4: *stop=0x8c64ceb4u;r[0]&=r[3];goto L8c64ceb6; /* and r3,r0 */
L8c64ceb6: *stop=0x8c64ceb6u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64ceb8; /* cmp/eq #0x0,r0 */
L8c64ceb8: if((*sr&1u))goto L8c64cece;goto L8c64ceba; /* bt 0x8c64cece */
L8c64ceba: *stop=0x8c64cebau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cf88u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cebc; /* mov.l 0x8c64cf88,r1 */
L8c64cebc: *stop=0x8c64cebcu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64cebe; /* cmp/eq r1,r0 */
L8c64cebe: if((*sr&1u))goto L8c64cfb4;goto L8c64cec0; /* bt 0x8c64cfb4 */
L8c64cec0: *stop=0x8c64cec0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cf8cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cec2; /* mov.l 0x8c64cf8c,r1 */
L8c64cec2: *stop=0x8c64cec2u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64cec4; /* cmp/eq r1,r0 */
L8c64cec4: if(!(*sr&1u))goto L8c64ceca;goto L8c64cec6; /* bf 0x8c64ceca */
L8c64cec6: *stop=0x8c64cec8u;goto L8c64d034; /* bra 0x8c64d034 */
L8c64ceca: *stop=0x8c64ceccu;goto L8c64d142; /* bra 0x8c64d142 */
L8c64cece: *stop=0x8c64ceceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cf90u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ced0; /* mov.l 0x8c64cf90,r2 */
L8c64ced0: target=r[2];*pr=0x8c64ced4u;*stop=0x8c64ced2u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ced4; /* jsr @r2 */
L8c64ced4: *stop=0x8c64ced4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cf94u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ced6; /* mov.l 0x8c64cf94,r3 */
L8c64ced6: *stop=0x8c64ced6u;r[5]=r[0];goto L8c64ced8; /* mov r0,r5 */
L8c64ced8: *stop=0x8c64ced8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cf98u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ceda; /* mov.l 0x8c64cf98,r2 */
L8c64ceda: *stop=0x8c64cedau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c64cedc; /* mov.l r0,@r3 */
L8c64cedc: *stop=0x8c64cedcu;r[0]=sx(m,bus,&io_status,0x8c64cf7cu,2);if(io_status)return io_status;goto L8c64cede; /* mov.w 0x8c64cf7c,r0 */
L8c64cede: target=r[2];*pr=0x8c64cee2u;*stop=0x8c64cee0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cee2; /* jsr @r2 */
L8c64cee2: *stop=0x8c64cee2u;r[1]=sx(m,bus,&io_status,0x8c64cf7eu,2);if(io_status)return io_status;goto L8c64cee4; /* mov.w 0x8c64cf7e,r1 */
L8c64cee4: *stop=0x8c64cee4u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64cee6; /* cmp/pz r0 */
L8c64cee6: *stop=0x8c64cee6u;r[1]+=r[15];goto L8c64cee8; /* add r15,r1 */
L8c64cee8: take=(*sr&1u);*stop=0x8c64ceeau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64cf36;goto L8c64ceec; /* bt/s 0x8c64cf36 */
L8c64ceec: *stop=0x8c64ceecu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ceee; /* mov.l @(0x10,r15),r2 */
L8c64ceee: *stop=0x8c64ceeeu;r[7]=0u;goto L8c64cef0; /* mov #0x0,r7 */
L8c64cef0: *stop=0x8c64cef0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cf94u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cef2; /* mov.l 0x8c64cf94,r3 */
L8c64cef2: *stop=0x8c64cef2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64cef4; /* mov.l r2,@-r15 */
L8c64cef4: *stop=0x8c64cef4u;r[0]=sx(m,bus,&io_status,0x8c64cf80u,2);if(io_status)return io_status;goto L8c64cef6; /* mov.w 0x8c64cf80,r0 */
L8c64cef6: *stop=0x8c64cef6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cf9cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cef8; /* mov.l 0x8c64cf9c,r2 */
L8c64cef8: *stop=0x8c64cef8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cefa; /* mov.l @r3,r5 */
L8c64cefa: *stop=0x8c64cefau;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cefc; /* mov.l @(0x18,r15),r6 */
L8c64cefc: target=r[2];*pr=0x8c64cf00u;*stop=0x8c64cefeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cf00; /* jsr @r2 */
L8c64cf00: *stop=0x8c64cf00u;r[15]+=4u;goto L8c64cf02; /* add #0x4,r15 */
L8c64cf02: *stop=0x8c64cf02u;r[0]=sx(m,bus,&io_status,0x8c64cf82u,2);if(io_status)return io_status;goto L8c64cf04; /* mov.w 0x8c64cf82,r0 */
L8c64cf04: *stop=0x8c64cf04u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cf06; /* mov.l @(r0,r15),r3 */
L8c64cf06: *stop=0x8c64cf06u;r[0]=sx(m,bus,&io_status,0x8c64cf7eu,2);if(io_status)return io_status;goto L8c64cf08; /* mov.w 0x8c64cf7e,r0 */
L8c64cf08: *stop=0x8c64cf08u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cf0a; /* mov.l @r3,r2 */
L8c64cf0a: *stop=0x8c64cf0au;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64cf0c; /* mov.w @(r0,r15),r1 */
L8c64cf0c: *stop=0x8c64cf0cu;r[0]=66u;goto L8c64cf0e; /* mov #0x42,r0 */
L8c64cf0e: *stop=0x8c64cf0eu;r[1]=0u-r[1];goto L8c64cf10; /* neg r1,r1 */
L8c64cf10: *stop=0x8c64cf10u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64cf12; /* mov.w r1,@(r0,r2) */
L8c64cf12: *stop=0x8c64cf12u;r[1]=0u;goto L8c64cf14; /* mov #0x0,r1 */
L8c64cf14: *stop=0x8c64cf14u;r[0]=sx(m,bus,&io_status,0x8c64cf82u,2);if(io_status)return io_status;goto L8c64cf16; /* mov.w 0x8c64cf82,r0 */
L8c64cf16: *stop=0x8c64cf16u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cf18; /* mov.l @(r0,r15),r3 */
L8c64cf18: *stop=0x8c64cf18u;r[0]=64u;goto L8c64cf1a; /* mov #0x40,r0 */
L8c64cf1a: *stop=0x8c64cf1au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cf1c; /* mov.l @r3,r2 */
L8c64cf1c: *stop=0x8c64cf1cu;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64cf1e; /* mov.w r1,@(r0,r2) */
L8c64cf1e: *stop=0x8c64cf1eu;r[0]=sx(m,bus,&io_status,0x8c64cf80u,2);if(io_status)return io_status;goto L8c64cf20; /* mov.w 0x8c64cf80,r0 */
L8c64cf20: *stop=0x8c64cf20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cfa0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cf22; /* mov.l 0x8c64cfa0,r2 */
L8c64cf22: *stop=0x8c64cf22u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cf24; /* mov.l @(r0,r15),r3 */
L8c64cf24: *stop=0x8c64cf24u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64cf26; /* mov.l r3,@r2 */
L8c64cf26: *stop=0x8c64cf26u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cfa4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cf28; /* mov.l 0x8c64cfa4,r3 */
L8c64cf28: *stop=0x8c64cf28u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cf2a; /* mov.l @(0x14,r15),r1 */
L8c64cf2a: *stop=0x8c64cf2au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64cf2c; /* mov.l r1,@r3 */
L8c64cf2c: *stop=0x8c64cf2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cfa8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64cf2e; /* mov.l 0x8c64cfa8,r1 */
L8c64cf2e: target=r[1];*pr=0x8c64cf32u;*stop=0x8c64cf30u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cf32; /* jsr @r1 */
L8c64cf32: *stop=0x8c64cf34u;goto L8c64d190; /* bra 0x8c64d190 */
L8c64cf36: *stop=0x8c64cf36u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cf38; /* mov.l @(0x10,r15),r2 */
L8c64cf38: *stop=0x8c64cf38u;r[7]=2u;goto L8c64cf3a; /* mov #0x2,r7 */
L8c64cf3a: *stop=0x8c64cf3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cf94u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cf3c; /* mov.l 0x8c64cf94,r3 */
L8c64cf3c: *stop=0x8c64cf3cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64cf3e; /* mov.l r2,@-r15 */
L8c64cf3e: *stop=0x8c64cf3eu;r[0]=sx(m,bus,&io_status,0x8c64cf80u,2);if(io_status)return io_status;goto L8c64cf40; /* mov.w 0x8c64cf80,r0 */
L8c64cf40: *stop=0x8c64cf40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cf9cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cf42; /* mov.l 0x8c64cf9c,r2 */
L8c64cf42: *stop=0x8c64cf42u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cf44; /* mov.l @r3,r5 */
L8c64cf44: *stop=0x8c64cf44u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cf46; /* mov.l @(0x18,r15),r6 */
L8c64cf46: target=r[2];*pr=0x8c64cf4au;*stop=0x8c64cf48u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cf4a; /* jsr @r2 */
L8c64cf4a: *stop=0x8c64cf4au;r[15]+=4u;goto L8c64cf4c; /* add #0x4,r15 */
L8c64cf4c: *stop=0x8c64cf4cu;r[0]=sx(m,bus,&io_status,0x8c64cf82u,2);if(io_status)return io_status;goto L8c64cf4e; /* mov.w 0x8c64cf82,r0 */
L8c64cf4e: *stop=0x8c64cf4eu;r[1]=0u;goto L8c64cf50; /* mov #0x0,r1 */
L8c64cf50: *stop=0x8c64cf50u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cf52; /* mov.l @(r0,r15),r3 */
L8c64cf52: *stop=0x8c64cf52u;r[0]=66u;goto L8c64cf54; /* mov #0x42,r0 */
L8c64cf54: *stop=0x8c64cf54u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cf56; /* mov.l @r3,r2 */
L8c64cf56: *stop=0x8c64cf56u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cfacu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cf58; /* mov.l 0x8c64cfac,r3 */
L8c64cf58: *stop=0x8c64cf58u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64cf5a; /* mov.w r1,@(r0,r2) */
L8c64cf5a: *stop=0x8c64cf5au;r[0]=sx(m,bus,&io_status,0x8c64cf82u,2);if(io_status)return io_status;goto L8c64cf5c; /* mov.w 0x8c64cf82,r0 */
L8c64cf5c: *stop=0x8c64cf5cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64cf5e; /* mov.l @(r0,r15),r4 */
L8c64cf5e: target=r[3];*pr=0x8c64cf62u;*stop=0x8c64cf60u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cf62; /* jsr @r3 */
L8c64cf62: *stop=0x8c64cf62u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cf64; /* mov.l @(0x18,r15),r2 */
L8c64cf64: *stop=0x8c64cf64u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64cf66; /* tst r2,r2 */
L8c64cf66: if(!(*sr&1u))goto L8c64cf6c;goto L8c64cf68; /* bf 0x8c64cf6c */
L8c64cf68: *stop=0x8c64cf6au;goto L8c64d142; /* bra 0x8c64d142 */
L8c64cf6c: *stop=0x8c64cf6cu;r[0]=sx(m,bus,&io_status,0x8c64cf82u,2);if(io_status)return io_status;goto L8c64cf6e; /* mov.w 0x8c64cf82,r0 */
L8c64cf6e: *stop=0x8c64cf6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64cfb0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cf70; /* mov.l 0x8c64cfb0,r3 */
L8c64cf70: *stop=0x8c64cf70u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64cf72; /* mov.l @(r0,r15),r4 */
L8c64cf72: *stop=0x8c64cf72u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cf74; /* mov.l @(0x10,r15),r5 */
L8c64cf74: target=r[3];*pr=0x8c64cf78u;*stop=0x8c64cf76u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cf78; /* jsr @r3 */
L8c64cf78: *stop=0x8c64cf7au;goto L8c64d142; /* bra 0x8c64d142 */
L8c64cfb4: *stop=0x8c64cfb4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0e0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cfb6; /* mov.l 0x8c64d0e0,r2 */
L8c64cfb6: target=r[2];*pr=0x8c64cfbau;*stop=0x8c64cfb8u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cfba; /* jsr @r2 */
L8c64cfba: *stop=0x8c64cfbau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cfbc; /* mov.l 0x8c64d0e4,r3 */
L8c64cfbc: *stop=0x8c64cfbcu;r[5]=r[0];goto L8c64cfbe; /* mov r0,r5 */
L8c64cfbe: *stop=0x8c64cfbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cfc0; /* mov.l 0x8c64d0e8,r2 */
L8c64cfc0: *stop=0x8c64cfc0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c64cfc2; /* mov.l r0,@r3 */
L8c64cfc2: *stop=0x8c64cfc2u;r[0]=sx(m,bus,&io_status,0x8c64d0d8u,2);if(io_status)return io_status;goto L8c64cfc4; /* mov.w 0x8c64d0d8,r0 */
L8c64cfc4: target=r[2];*pr=0x8c64cfc8u;*stop=0x8c64cfc6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cfc8; /* jsr @r2 */
L8c64cfc8: *stop=0x8c64cfc8u;r[1]=sx(m,bus,&io_status,0x8c64d0dau,2);if(io_status)return io_status;goto L8c64cfca; /* mov.w 0x8c64d0da,r1 */
L8c64cfca: *stop=0x8c64cfcau;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64cfcc; /* cmp/pz r0 */
L8c64cfcc: *stop=0x8c64cfccu;r[1]+=r[15];goto L8c64cfce; /* add r15,r1 */
L8c64cfce: take=(*sr&1u);*stop=0x8c64cfd0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64d010;goto L8c64cfd2; /* bt/s 0x8c64d010 */
L8c64cfd2: *stop=0x8c64cfd2u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cfd4; /* mov.l @(0x10,r15),r2 */
L8c64cfd4: *stop=0x8c64cfd4u;r[7]=0u;goto L8c64cfd6; /* mov #0x0,r7 */
L8c64cfd6: *stop=0x8c64cfd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cfd8; /* mov.l 0x8c64d0e4,r3 */
L8c64cfd8: *stop=0x8c64cfd8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64cfda; /* mov.l r2,@-r15 */
L8c64cfda: *stop=0x8c64cfdau;r[0]=sx(m,bus,&io_status,0x8c64d0dcu,2);if(io_status)return io_status;goto L8c64cfdc; /* mov.w 0x8c64d0dc,r0 */
L8c64cfdc: *stop=0x8c64cfdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cfde; /* mov.l 0x8c64d0ec,r2 */
L8c64cfde: *stop=0x8c64cfdeu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64cfe0; /* mov.l @r3,r5 */
L8c64cfe0: *stop=0x8c64cfe0u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64cfe2; /* mov.l @(0x18,r15),r6 */
L8c64cfe2: target=r[2];*pr=0x8c64cfe6u;*stop=0x8c64cfe4u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64cfe6; /* jsr @r2 */
L8c64cfe6: *stop=0x8c64cfe6u;r[15]+=4u;goto L8c64cfe8; /* add #0x4,r15 */
L8c64cfe8: *stop=0x8c64cfe8u;r[0]=sx(m,bus,&io_status,0x8c64d0deu,2);if(io_status)return io_status;goto L8c64cfea; /* mov.w 0x8c64d0de,r0 */
L8c64cfea: *stop=0x8c64cfeau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cfec; /* mov.l @(r0,r15),r3 */
L8c64cfec: *stop=0x8c64cfecu;r[0]=sx(m,bus,&io_status,0x8c64d0dau,2);if(io_status)return io_status;goto L8c64cfee; /* mov.w 0x8c64d0da,r0 */
L8c64cfee: *stop=0x8c64cfeeu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cff0; /* mov.l @r3,r2 */
L8c64cff0: *stop=0x8c64cff0u;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64cff2; /* mov.w @(r0,r15),r1 */
L8c64cff2: *stop=0x8c64cff2u;r[0]=66u;goto L8c64cff4; /* mov #0x42,r0 */
L8c64cff4: *stop=0x8c64cff4u;r[1]=0u-r[1];goto L8c64cff6; /* neg r1,r1 */
L8c64cff6: *stop=0x8c64cff6u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64cff8; /* mov.w r1,@(r0,r2) */
L8c64cff8: *stop=0x8c64cff8u;r[0]=sx(m,bus,&io_status,0x8c64d0dcu,2);if(io_status)return io_status;goto L8c64cffa; /* mov.w 0x8c64d0dc,r0 */
L8c64cffa: *stop=0x8c64cffau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64cffc; /* mov.l 0x8c64d0f0,r2 */
L8c64cffc: *stop=0x8c64cffcu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64cffe; /* mov.l @(r0,r15),r3 */
L8c64cffe: *stop=0x8c64cffeu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64d000; /* mov.l r3,@r2 */
L8c64d000: *stop=0x8c64d000u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d002; /* mov.l 0x8c64d0f4,r3 */
L8c64d002: *stop=0x8c64d002u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64d004; /* mov.l @(0x14,r15),r1 */
L8c64d004: *stop=0x8c64d004u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64d006; /* mov.l r1,@r3 */
L8c64d006: *stop=0x8c64d006u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0f8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64d008; /* mov.l 0x8c64d0f8,r1 */
L8c64d008: target=r[1];*pr=0x8c64d00cu;*stop=0x8c64d00au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d00c; /* jsr @r1 */
L8c64d00c: *stop=0x8c64d00eu;goto L8c64d190; /* bra 0x8c64d190 */
L8c64d010: *stop=0x8c64d010u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d012; /* mov.l @(0x10,r15),r2 */
L8c64d012: *stop=0x8c64d012u;r[7]=1u;goto L8c64d014; /* mov #0x1,r7 */
L8c64d014: *stop=0x8c64d014u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d016; /* mov.l 0x8c64d0e4,r3 */
L8c64d016: *stop=0x8c64d016u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64d018; /* mov.l r2,@-r15 */
L8c64d018: *stop=0x8c64d018u;r[0]=sx(m,bus,&io_status,0x8c64d0dcu,2);if(io_status)return io_status;goto L8c64d01a; /* mov.w 0x8c64d0dc,r0 */
L8c64d01a: *stop=0x8c64d01au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d01c; /* mov.l 0x8c64d0ec,r2 */
L8c64d01c: *stop=0x8c64d01cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64d01e; /* mov.l @r3,r5 */
L8c64d01e: *stop=0x8c64d01eu;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64d020; /* mov.l @(0x18,r15),r6 */
L8c64d020: target=r[2];*pr=0x8c64d024u;*stop=0x8c64d022u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d024; /* jsr @r2 */
L8c64d024: *stop=0x8c64d024u;r[15]+=4u;goto L8c64d026; /* add #0x4,r15 */
L8c64d026: *stop=0x8c64d026u;r[0]=sx(m,bus,&io_status,0x8c64d0deu,2);if(io_status)return io_status;goto L8c64d028; /* mov.w 0x8c64d0de,r0 */
L8c64d028: *stop=0x8c64d028u;r[1]=0u;goto L8c64d02a; /* mov #0x0,r1 */
L8c64d02a: *stop=0x8c64d02au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d02c; /* mov.l @(r0,r15),r3 */
L8c64d02c: *stop=0x8c64d02cu;r[0]=66u;goto L8c64d02e; /* mov #0x42,r0 */
L8c64d02e: *stop=0x8c64d02eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d030; /* mov.l @r3,r2 */
L8c64d030: *stop=0x8c64d032u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64d142; /* bra 0x8c64d142 */
L8c64d034: *stop=0x8c64d034u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0e0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d036; /* mov.l 0x8c64d0e0,r2 */
L8c64d036: target=r[2];*pr=0x8c64d03au;*stop=0x8c64d038u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d03a; /* jsr @r2 */
L8c64d03a: *stop=0x8c64d03au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d03c; /* mov.l 0x8c64d0e4,r3 */
L8c64d03c: *stop=0x8c64d03cu;r[5]=r[0];goto L8c64d03e; /* mov r0,r5 */
L8c64d03e: *stop=0x8c64d03eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0fcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d040; /* mov.l 0x8c64d0fc,r2 */
L8c64d040: *stop=0x8c64d040u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c64d042; /* mov.l r0,@r3 */
L8c64d042: *stop=0x8c64d042u;r[0]=sx(m,bus,&io_status,0x8c64d0d8u,2);if(io_status)return io_status;goto L8c64d044; /* mov.w 0x8c64d0d8,r0 */
L8c64d044: target=r[2];*pr=0x8c64d048u;*stop=0x8c64d046u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d048; /* jsr @r2 */
L8c64d048: *stop=0x8c64d048u;r[1]=sx(m,bus,&io_status,0x8c64d0dau,2);if(io_status)return io_status;goto L8c64d04a; /* mov.w 0x8c64d0da,r1 */
L8c64d04a: *stop=0x8c64d04au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64d04c; /* cmp/pz r0 */
L8c64d04c: *stop=0x8c64d04cu;r[1]+=r[15];goto L8c64d04e; /* add r15,r1 */
L8c64d04e: take=(*sr&1u);*stop=0x8c64d050u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64d09c;goto L8c64d052; /* bt/s 0x8c64d09c */
L8c64d052: *stop=0x8c64d052u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d054; /* mov.l @(0x10,r15),r2 */
L8c64d054: *stop=0x8c64d054u;r[7]=0u;goto L8c64d056; /* mov #0x0,r7 */
L8c64d056: *stop=0x8c64d056u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d058; /* mov.l 0x8c64d0e4,r3 */
L8c64d058: *stop=0x8c64d058u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64d05a; /* mov.l r2,@-r15 */
L8c64d05a: *stop=0x8c64d05au;r[0]=sx(m,bus,&io_status,0x8c64d0dcu,2);if(io_status)return io_status;goto L8c64d05c; /* mov.w 0x8c64d0dc,r0 */
L8c64d05c: *stop=0x8c64d05cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d05e; /* mov.l 0x8c64d0ec,r2 */
L8c64d05e: *stop=0x8c64d05eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64d060; /* mov.l @r3,r5 */
L8c64d060: *stop=0x8c64d060u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64d062; /* mov.l @(0x18,r15),r6 */
L8c64d062: target=r[2];*pr=0x8c64d066u;*stop=0x8c64d064u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d066; /* jsr @r2 */
L8c64d066: *stop=0x8c64d066u;r[15]+=4u;goto L8c64d068; /* add #0x4,r15 */
L8c64d068: *stop=0x8c64d068u;r[0]=sx(m,bus,&io_status,0x8c64d0deu,2);if(io_status)return io_status;goto L8c64d06a; /* mov.w 0x8c64d0de,r0 */
L8c64d06a: *stop=0x8c64d06au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d06c; /* mov.l @(r0,r15),r3 */
L8c64d06c: *stop=0x8c64d06cu;r[0]=sx(m,bus,&io_status,0x8c64d0dau,2);if(io_status)return io_status;goto L8c64d06e; /* mov.w 0x8c64d0da,r0 */
L8c64d06e: *stop=0x8c64d06eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d070; /* mov.l @r3,r2 */
L8c64d070: *stop=0x8c64d070u;if(!valid_access(r[0]+r[15],2u))return -3;r[1]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c64d072; /* mov.w @(r0,r15),r1 */
L8c64d072: *stop=0x8c64d072u;r[0]=66u;goto L8c64d074; /* mov #0x42,r0 */
L8c64d074: *stop=0x8c64d074u;r[1]=0u-r[1];goto L8c64d076; /* neg r1,r1 */
L8c64d076: *stop=0x8c64d076u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64d078; /* mov.w r1,@(r0,r2) */
L8c64d078: *stop=0x8c64d078u;r[1]=0u;goto L8c64d07a; /* mov #0x0,r1 */
L8c64d07a: *stop=0x8c64d07au;r[0]=sx(m,bus,&io_status,0x8c64d0deu,2);if(io_status)return io_status;goto L8c64d07c; /* mov.w 0x8c64d0de,r0 */
L8c64d07c: *stop=0x8c64d07cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d07e; /* mov.l @(r0,r15),r3 */
L8c64d07e: *stop=0x8c64d07eu;r[0]=64u;goto L8c64d080; /* mov #0x40,r0 */
L8c64d080: *stop=0x8c64d080u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d082; /* mov.l @r3,r2 */
L8c64d082: *stop=0x8c64d082u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64d084; /* mov.w r1,@(r0,r2) */
L8c64d084: *stop=0x8c64d084u;r[0]=sx(m,bus,&io_status,0x8c64d0dcu,2);if(io_status)return io_status;goto L8c64d086; /* mov.w 0x8c64d0dc,r0 */
L8c64d086: *stop=0x8c64d086u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d088; /* mov.l 0x8c64d0f0,r2 */
L8c64d088: *stop=0x8c64d088u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d08a; /* mov.l @(r0,r15),r3 */
L8c64d08a: *stop=0x8c64d08au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64d08c; /* mov.l r3,@r2 */
L8c64d08c: *stop=0x8c64d08cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d08e; /* mov.l 0x8c64d0f4,r3 */
L8c64d08e: *stop=0x8c64d08eu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64d090; /* mov.l @(0x14,r15),r1 */
L8c64d090: *stop=0x8c64d090u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64d092; /* mov.l r1,@r3 */
L8c64d092: *stop=0x8c64d092u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0f8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64d094; /* mov.l 0x8c64d0f8,r1 */
L8c64d094: target=r[1];*pr=0x8c64d098u;*stop=0x8c64d096u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d098; /* jsr @r1 */
L8c64d098: *stop=0x8c64d09au;goto L8c64d190; /* bra 0x8c64d190 */
L8c64d09c: *stop=0x8c64d09cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d09e; /* mov.l 0x8c64d0e4,r2 */
L8c64d09e: *stop=0x8c64d09eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d100u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d0a0; /* mov.l 0x8c64d100,r3 */
L8c64d0a0: *stop=0x8c64d0a0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64d0a2; /* mov.l @r2,r5 */
L8c64d0a2: target=r[3];*pr=0x8c64d0a6u;*stop=0x8c64d0a4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d0a6; /* jsr @r3 */
L8c64d0a6: *stop=0x8c64d0a6u;r[1]=sx(m,bus,&io_status,0x8c64d0dau,2);if(io_status)return io_status;goto L8c64d0a8; /* mov.w 0x8c64d0da,r1 */
L8c64d0a8: *stop=0x8c64d0a8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64d0aa; /* tst r0,r0 */
L8c64d0aa: *stop=0x8c64d0aau;r[1]+=r[15];goto L8c64d0ac; /* add r15,r1 */
L8c64d0ac: take=!(*sr&1u);*stop=0x8c64d0aeu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c64d104;goto L8c64d0b0; /* bf/s 0x8c64d104 */
L8c64d0b0: *stop=0x8c64d0b0u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d0b2; /* mov.l @(0x10,r15),r2 */
L8c64d0b2: *stop=0x8c64d0b2u;r[7]=2u;goto L8c64d0b4; /* mov #0x2,r7 */
L8c64d0b4: *stop=0x8c64d0b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d0b6; /* mov.l 0x8c64d0e4,r3 */
L8c64d0b6: *stop=0x8c64d0b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64d0b8; /* mov.l r2,@-r15 */
L8c64d0b8: *stop=0x8c64d0b8u;r[0]=sx(m,bus,&io_status,0x8c64d0dcu,2);if(io_status)return io_status;goto L8c64d0ba; /* mov.w 0x8c64d0dc,r0 */
L8c64d0ba: *stop=0x8c64d0bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d0bc; /* mov.l 0x8c64d0ec,r2 */
L8c64d0bc: *stop=0x8c64d0bcu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64d0be; /* mov.l @r3,r5 */
L8c64d0be: *stop=0x8c64d0beu;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64d0c0; /* mov.l @(0x18,r15),r6 */
L8c64d0c0: target=r[2];*pr=0x8c64d0c4u;*stop=0x8c64d0c2u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d0c4; /* jsr @r2 */
L8c64d0c4: *stop=0x8c64d0c4u;r[15]+=4u;goto L8c64d0c6; /* add #0x4,r15 */
L8c64d0c6: *stop=0x8c64d0c6u;r[0]=sx(m,bus,&io_status,0x8c64d0dcu,2);if(io_status)return io_status;goto L8c64d0c8; /* mov.w 0x8c64d0dc,r0 */
L8c64d0c8: *stop=0x8c64d0c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d0ca; /* mov.l 0x8c64d0f0,r2 */
L8c64d0ca: *stop=0x8c64d0cau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d0cc; /* mov.l @(r0,r15),r3 */
L8c64d0cc: *stop=0x8c64d0ccu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64d0ce; /* mov.l r3,@r2 */
L8c64d0ce: *stop=0x8c64d0ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d0f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d0d0; /* mov.l 0x8c64d0f4,r3 */
L8c64d0d0: *stop=0x8c64d0d0u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64d0d2; /* mov.l @(0x14,r15),r1 */
L8c64d0d2: *stop=0x8c64d0d2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64d0d4; /* mov.l r1,@r3 */
L8c64d0d4: *stop=0x8c64d0d6u;goto L8c64d190; /* bra 0x8c64d190 */
L8c64d104: *stop=0x8c64d104u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d106; /* mov.l @(0x10,r15),r2 */
L8c64d106: *stop=0x8c64d106u;r[7]=3u;goto L8c64d108; /* mov #0x3,r7 */
L8c64d108: *stop=0x8c64d108u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d234u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d10a; /* mov.l 0x8c64d234,r3 */
L8c64d10a: *stop=0x8c64d10au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64d10c; /* mov.l r2,@-r15 */
L8c64d10c: *stop=0x8c64d10cu;r[0]=sx(m,bus,&io_status,0x8c64d22eu,2);if(io_status)return io_status;goto L8c64d10e; /* mov.w 0x8c64d22e,r0 */
L8c64d10e: *stop=0x8c64d10eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d238u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d110; /* mov.l 0x8c64d238,r2 */
L8c64d110: *stop=0x8c64d110u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64d112; /* mov.l @r3,r5 */
L8c64d112: *stop=0x8c64d112u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64d114; /* mov.l @(0x18,r15),r6 */
L8c64d114: target=r[2];*pr=0x8c64d118u;*stop=0x8c64d116u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d118; /* jsr @r2 */
L8c64d118: *stop=0x8c64d118u;r[15]+=4u;goto L8c64d11a; /* add #0x4,r15 */
L8c64d11a: *stop=0x8c64d11au;r[0]=sx(m,bus,&io_status,0x8c64d230u,2);if(io_status)return io_status;goto L8c64d11c; /* mov.w 0x8c64d230,r0 */
L8c64d11c: *stop=0x8c64d11cu;r[1]=0u;goto L8c64d11e; /* mov #0x0,r1 */
L8c64d11e: *stop=0x8c64d11eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d120; /* mov.l @(r0,r15),r3 */
L8c64d120: *stop=0x8c64d120u;r[0]=66u;goto L8c64d122; /* mov #0x42,r0 */
L8c64d122: *stop=0x8c64d122u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d124; /* mov.l @r3,r2 */
L8c64d124: *stop=0x8c64d124u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d23cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d126; /* mov.l 0x8c64d23c,r3 */
L8c64d126: *stop=0x8c64d126u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64d128; /* mov.w r1,@(r0,r2) */
L8c64d128: *stop=0x8c64d128u;r[0]=sx(m,bus,&io_status,0x8c64d230u,2);if(io_status)return io_status;goto L8c64d12a; /* mov.w 0x8c64d230,r0 */
L8c64d12a: *stop=0x8c64d12au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64d12c; /* mov.l @(r0,r15),r4 */
L8c64d12c: target=r[3];*pr=0x8c64d130u;*stop=0x8c64d12eu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d130; /* jsr @r3 */
L8c64d130: *stop=0x8c64d130u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d132; /* mov.l @(0x18,r15),r2 */
L8c64d132: *stop=0x8c64d132u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64d134; /* tst r2,r2 */
L8c64d134: if((*sr&1u))goto L8c64d142;goto L8c64d136; /* bt 0x8c64d142 */
L8c64d136: *stop=0x8c64d136u;r[0]=sx(m,bus,&io_status,0x8c64d230u,2);if(io_status)return io_status;goto L8c64d138; /* mov.w 0x8c64d230,r0 */
L8c64d138: *stop=0x8c64d138u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d240u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d13a; /* mov.l 0x8c64d240,r3 */
L8c64d13a: *stop=0x8c64d13au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64d13c; /* mov.l @(r0,r15),r4 */
L8c64d13c: *stop=0x8c64d13cu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64d13e; /* mov.l @(0x10,r15),r5 */
L8c64d13e: target=r[3];*pr=0x8c64d142u;*stop=0x8c64d140u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d142; /* jsr @r3 */
L8c64d142: *stop=0x8c64d142u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d244u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d144; /* mov.l 0x8c64d244,r3 */
L8c64d144: target=r[3];*pr=0x8c64d148u;*stop=0x8c64d146u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d148; /* jsr @r3 */
L8c64d148: *stop=0x8c64d14au;goto L8c64d19a; /* bra 0x8c64d19a */
L8c64d14c: *stop=0x8c64d14cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d250u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d14e; /* mov.l 0x8c64d250,r2 */
L8c64d14e: *stop=0x8c64d14eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d24cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64d150; /* mov.l 0x8c64d24c,r4 */
L8c64d150: *stop=0x8c64d150u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d248u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64d152; /* mov.l 0x8c64d248,r5 */
L8c64d152: target=r[2];*pr=0x8c64d156u;*stop=0x8c64d154u;r[6]=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d156; /* jsr @r2 */
L8c64d156: *stop=0x8c64d156u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d158; /* mov.l @(0x10,r15),r3 */
L8c64d158: *stop=0x8c64d158u;r[7]=0u;goto L8c64d15a; /* mov #0x0,r7 */
L8c64d15a: *stop=0x8c64d15au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d254u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64d15c; /* mov.l 0x8c64d254,r5 */
L8c64d15c: *stop=0x8c64d15cu;r[6]=4294967295u;goto L8c64d15e; /* mov #-0x1,r6 */
L8c64d15e: *stop=0x8c64d15eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64d160; /* mov.l r3,@-r15 */
L8c64d160: *stop=0x8c64d160u;r[0]=sx(m,bus,&io_status,0x8c64d22eu,2);if(io_status)return io_status;goto L8c64d162; /* mov.w 0x8c64d22e,r0 */
L8c64d162: *stop=0x8c64d162u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d238u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d164; /* mov.l 0x8c64d238,r3 */
L8c64d164: target=r[3];*pr=0x8c64d168u;*stop=0x8c64d166u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64d168; /* jsr @r3 */
L8c64d168: *stop=0x8c64d168u;r[15]+=4u;goto L8c64d16a; /* add #0x4,r15 */
L8c64d16a: *stop=0x8c64d16au;r[0]=sx(m,bus,&io_status,0x8c64d230u,2);if(io_status)return io_status;goto L8c64d16c; /* mov.w 0x8c64d230,r0 */
L8c64d16c: *stop=0x8c64d16cu;r[1]=3u;goto L8c64d16e; /* mov #0x3,r1 */
L8c64d16e: *stop=0x8c64d16eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d170; /* mov.l @(r0,r15),r2 */
L8c64d170: *stop=0x8c64d170u;r[0]=66u;goto L8c64d172; /* mov #0x42,r0 */
L8c64d172: *stop=0x8c64d172u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d174; /* mov.l @r2,r3 */
L8c64d174: *stop=0x8c64d174u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[1],2);if(io_status)return io_status;goto L8c64d176; /* mov.w r1,@(r0,r3) */
L8c64d176: *stop=0x8c64d176u;r[1]=0u;goto L8c64d178; /* mov #0x0,r1 */
L8c64d178: *stop=0x8c64d178u;r[0]=sx(m,bus,&io_status,0x8c64d230u,2);if(io_status)return io_status;goto L8c64d17a; /* mov.w 0x8c64d230,r0 */
L8c64d17a: *stop=0x8c64d17au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d17c; /* mov.l @(r0,r15),r3 */
L8c64d17c: *stop=0x8c64d17cu;r[0]=64u;goto L8c64d17e; /* mov #0x40,r0 */
L8c64d17e: *stop=0x8c64d17eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d180; /* mov.l @r3,r2 */
L8c64d180: *stop=0x8c64d180u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[1],2);if(io_status)return io_status;goto L8c64d182; /* mov.w r1,@(r0,r2) */
L8c64d182: *stop=0x8c64d182u;r[0]=sx(m,bus,&io_status,0x8c64d22eu,2);if(io_status)return io_status;goto L8c64d184; /* mov.w 0x8c64d22e,r0 */
L8c64d184: *stop=0x8c64d184u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d258u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64d186; /* mov.l 0x8c64d258,r2 */
L8c64d186: *stop=0x8c64d186u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d188; /* mov.l @(r0,r15),r3 */
L8c64d188: *stop=0x8c64d188u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64d18a; /* mov.l r3,@r2 */
L8c64d18a: *stop=0x8c64d18au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64d25cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64d18c; /* mov.l 0x8c64d25c,r3 */
L8c64d18c: *stop=0x8c64d18cu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64d18e; /* mov.l @(0x14,r15),r1 */
L8c64d18e: *stop=0x8c64d18eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64d190; /* mov.l r1,@r3 */
L8c64d190: *stop=0x8c64d190u;r[1]=sx(m,bus,&io_status,0x8c64d232u,2);if(io_status)return io_status;goto L8c64d192; /* mov.w 0x8c64d232,r1 */
L8c64d192: *stop=0x8c64d192u;r[15]+=r[1];goto L8c64d194; /* add r1,r15 */
L8c64d194: *stop=0x8c64d194u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64d196; /* lds.l @r15+,PR */
L8c64d196: *stop=0x8c64d196u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64d198u;r[0]=4294967295u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c64d19a: *stop=0x8c64d19au;r[0]=1u;goto L8c64d19c; /* mov #0x1,r0 */
L8c64d19c: *stop=0x8c64d19cu;r[1]=sx(m,bus,&io_status,0x8c64d232u,2);if(io_status)return io_status;goto L8c64d19e; /* mov.w 0x8c64d232,r1 */
L8c64d19e: *stop=0x8c64d19eu;r[15]+=r[1];goto L8c64d1a0; /* add r1,r15 */
L8c64d1a0: *stop=0x8c64d1a0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64d1a2; /* lds.l @r15+,PR */
L8c64d1a2: *stop=0x8c64d1a2u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64d1a4u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c64c374(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c408(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c408u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c440(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c440u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c458(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c458u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c464(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c464u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c4ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c4bau);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c50c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c50cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c524(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c524u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c53c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c53cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c54e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c54eu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c5cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c5ccu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c628(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c628u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c644(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c644u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c65c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c65cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c66e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c66eu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c698(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c698u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c6ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c6acu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c6be(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c6beu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c6e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c6e8u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c6ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c6eeu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c6fe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c6feu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c710(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c710u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c71c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c71cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c766(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c766u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c78c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c78cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c7a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c7a6u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c7d0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c7d0u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c7ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c7eeu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c804(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c804u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c8b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c8b8u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c8dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c8dcu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c8e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c8e8u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c90a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c90au);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c938(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c938u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c952(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c952u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c976(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c976u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c982(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c982u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ca34(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ca34u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ca58(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ca58u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ca64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ca64u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ca94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ca94u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cac2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cac2u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cb1a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cb1au);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cb38(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cb38u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cb52(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cb52u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cb64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cb64u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cb92(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cb92u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cbac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cbacu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cbca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cbcau);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cc78(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cc78u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cca6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cca6u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ccea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cceau);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cd0e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cd0eu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cd3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cd3cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cdb6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cdb6u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cde4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cde4u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ce12(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ce12u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ce40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ce40u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ce80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ce80u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ce86(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ce86u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ced4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ced4u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cee2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cee2u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cf00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cf00u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cf32(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cf32u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cf4a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cf4au);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cf62(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cf62u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cf78(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cf78u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cfba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cfbau);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cfc8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cfc8u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64cfe6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64cfe6u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d00c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d00cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d024(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d024u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d03a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d03au);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d048(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d048u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d066(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d066u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d098(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d098u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d0a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d0a6u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d0c4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d0c4u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d118(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d118u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d130(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d130u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d142(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d142u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d148(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d148u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d156(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d156u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64d168(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c374(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64d168u);}

static int sa_player_texture_link_run_8c64efcc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64efe8u)goto L8c64efe8;if(resume_pc==0x8c64eff8u)goto L8c64eff8;if(resume_pc==0x8c64effeu)goto L8c64effe;if(resume_pc==0x8c64f01eu)goto L8c64f01e;if(resume_pc==0x8c64f03au)goto L8c64f03a;if(resume_pc==0x8c64f040u)goto L8c64f040;if(resume_pc==0x8c64f04cu)goto L8c64f04c;if(resume_pc==0x8c64f052u)goto L8c64f052;if(resume_pc==0x8c64f058u)goto L8c64f058;goto L8c64efcc;
L8c64efcc: *stop=0x8c64efccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64efce; /* mov.l r14,@-r15 */
L8c64efce: *stop=0x8c64efceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64efd0; /* mov.l r13,@-r15 */
L8c64efd0: *stop=0x8c64efd0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64efd2; /* mov.l r12,@-r15 */
L8c64efd2: *stop=0x8c64efd2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64efd4; /* mov.l r11,@-r15 */
L8c64efd4: *stop=0x8c64efd4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64efd6; /* mov.l r10,@-r15 */
L8c64efd6: *stop=0x8c64efd6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64efd8; /* mov.l r9,@-r15 */
L8c64efd8: *stop=0x8c64efd8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64efda; /* mov.l r8,@-r15 */
L8c64efda: *stop=0x8c64efdau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64efdc; /* sts.l PR,@-r15 */
L8c64efdc: *stop=0x8c64efdcu;r[15]+=4294967288u;goto L8c64efde; /* add #-0x8,r15 */
L8c64efde: *stop=0x8c64efdeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f12cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64efe0; /* mov.l 0x8c64f12c,r3 */
L8c64efe0: *stop=0x8c64efe0u;r[12]=r[5];goto L8c64efe2; /* mov r5,r12 */
L8c64efe2: *stop=0x8c64efe2u;r[13]=r[4];goto L8c64efe4; /* mov r4,r13 */
L8c64efe4: target=r[3];*pr=0x8c64efe8u;*stop=0x8c64efe6u;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64efe8; /* jsr @r3 */
L8c64efe8: *stop=0x8c64efe8u;r[3]=r[0];goto L8c64efea; /* mov r0,r3 */
L8c64efea: *stop=0x8c64efeau;r[3]+=4294967292u;goto L8c64efec; /* add #-0x4,r3 */
L8c64efec: *stop=0x8c64efecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f130u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64efee; /* mov.l 0x8c64f130,r2 */
L8c64efee: *stop=0x8c64efeeu;r[3]+=r[12];goto L8c64eff0; /* add r12,r3 */
L8c64eff0: *stop=0x8c64eff0u;r[4]=r[0];goto L8c64eff2; /* mov r0,r4 */
L8c64eff2: *stop=0x8c64eff2u;r[1]=r[3];goto L8c64eff4; /* mov r3,r1 */
L8c64eff4: target=r[2];*pr=0x8c64eff8u;*stop=0x8c64eff6u;r[0]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64eff8; /* jsr @r2 */
L8c64eff8: *stop=0x8c64eff8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f134u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64effa; /* mov.l 0x8c64f134,r3 */
L8c64effa: target=r[3];*pr=0x8c64effeu;*stop=0x8c64effcu;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64effe; /* jsr @r3 */
L8c64effe: *stop=0x8c64effeu;if(!valid_access(r[15],1u))return -3;r[0]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c64f000; /* mov.b @r15,r0 */
L8c64f000: *stop=0x8c64f000u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f138u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c64f002; /* mov.l 0x8c64f138,r9 */
L8c64f002: *stop=0x8c64f002u;*sr=(*sr&~1u)|(r[0]==0x0000002eu);goto L8c64f004; /* cmp/eq #0x2e,r0 */
L8c64f004: if(!(*sr&1u))goto L8c64f044;goto L8c64f006; /* bf 0x8c64f044 */
L8c64f006: *stop=0x8c64f006u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f13cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64f008; /* mov.l 0x8c64f13c,r10 */
L8c64f008: *stop=0x8c64f008u;r[11]=3u;goto L8c64f00a; /* mov #0x3,r11 */
L8c64f00a: *stop=0x8c64f00au;r[14]=0u;goto L8c64f00c; /* mov #0x0,r14 */
L8c64f00c: *stop=0x8c64f00cu;r[8]=r[15];goto L8c64f00e; /* mov r15,r8 */
L8c64f00e: *stop=0x8c64f00eu;r[5]=r[14];goto L8c64f010; /* mov r14,r5 */
L8c64f010: *stop=0x8c64f010u;r[5]<<=2;goto L8c64f012; /* shll2 r5 */
L8c64f012: *stop=0x8c64f012u;r[3]=r[14];goto L8c64f014; /* mov r14,r3 */
L8c64f014: *stop=0x8c64f014u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f140u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f016; /* mov.l 0x8c64f140,r2 */
L8c64f016: *stop=0x8c64f016u;r[5]+=r[3];goto L8c64f018; /* add r3,r5 */
L8c64f018: *stop=0x8c64f018u;r[5]+=r[10];goto L8c64f01a; /* add r10,r5 */
L8c64f01a: target=r[2];*pr=0x8c64f01eu;*stop=0x8c64f01cu;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f01e; /* jsr @r2 */
L8c64f01e: *stop=0x8c64f01eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64f020; /* tst r0,r0 */
L8c64f020: if((*sr&1u))goto L8c64f028;goto L8c64f022; /* bt 0x8c64f028 */
L8c64f022: *stop=0x8c64f022u;r[14]+=1u;goto L8c64f024; /* add #0x1,r14 */
L8c64f024: *stop=0x8c64f024u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[11]);goto L8c64f026; /* cmp/ge r11,r14 */
L8c64f026: if(!(*sr&1u))goto L8c64f00e;goto L8c64f028; /* bf 0x8c64f00e */
L8c64f028: *stop=0x8c64f028u;r[0]=r[14];goto L8c64f02a; /* mov r14,r0 */
L8c64f02a: *stop=0x8c64f02au;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c64f02c; /* cmp/eq #0x3,r0 */
L8c64f02c: if((*sr&1u))goto L8c64f044;goto L8c64f02e; /* bt 0x8c64f044 */
L8c64f02e: *stop=0x8c64f02eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f144u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f030; /* mov.l 0x8c64f144,r2 */
L8c64f030: *stop=0x8c64f030u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f148u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f032; /* mov.l 0x8c64f148,r1 */
L8c64f032: *stop=0x8c64f032u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f130u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f034; /* mov.l 0x8c64f130,r3 */
L8c64f034: *stop=0x8c64f034u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c64f036; /* mov.l r14,@r2 */
L8c64f036: target=r[3];*pr=0x8c64f03au;*stop=0x8c64f038u;r[0]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f03a; /* jsr @r3 */
L8c64f03a: *stop=0x8c64f03au;r[5]=r[12];goto L8c64f03c; /* mov r12,r5 */
L8c64f03c: target=r[9];*pr=0x8c64f040u;*stop=0x8c64f03eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f040; /* jsr @r9 */
L8c64f040: *stop=0x8c64f042u;goto L8c64f05e; /* bra 0x8c64f05e */
L8c64f044: *stop=0x8c64f044u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f130u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f046; /* mov.l 0x8c64f130,r2 */
L8c64f046: *stop=0x8c64f046u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f148u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f048; /* mov.l 0x8c64f148,r1 */
L8c64f048: target=r[2];*pr=0x8c64f04cu;*stop=0x8c64f04au;r[0]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f04c; /* jsr @r2 */
L8c64f04c: *stop=0x8c64f04cu;r[5]=r[12];goto L8c64f04e; /* mov r12,r5 */
L8c64f04e: target=r[9];*pr=0x8c64f052u;*stop=0x8c64f050u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f052; /* jsr @r9 */
L8c64f052: *stop=0x8c64f052u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f14cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64f054; /* mov.l 0x8c64f14c,r5 */
L8c64f054: target=r[9];*pr=0x8c64f058u;*stop=0x8c64f056u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f058; /* jsr @r9 */
L8c64f058: *stop=0x8c64f058u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f144u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f05a; /* mov.l 0x8c64f144,r2 */
L8c64f05a: *stop=0x8c64f05au;r[3]=0u;goto L8c64f05c; /* mov #0x0,r3 */
L8c64f05c: *stop=0x8c64f05cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64f05e; /* mov.l r3,@r2 */
L8c64f05e: *stop=0x8c64f05eu;r[0]=1u;goto L8c64f060; /* mov #0x1,r0 */
L8c64f060: *stop=0x8c64f060u;r[15]+=8u;goto L8c64f062; /* add #0x8,r15 */
L8c64f062: *stop=0x8c64f062u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64f064; /* lds.l @r15+,PR */
L8c64f064: *stop=0x8c64f064u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64f066; /* mov.l @r15+,r8 */
L8c64f066: *stop=0x8c64f066u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64f068; /* mov.l @r15+,r9 */
L8c64f068: *stop=0x8c64f068u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64f06a; /* mov.l @r15+,r10 */
L8c64f06a: *stop=0x8c64f06au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64f06c; /* mov.l @r15+,r11 */
L8c64f06c: *stop=0x8c64f06cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64f06e; /* mov.l @r15+,r12 */
L8c64f06e: *stop=0x8c64f06eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64f070; /* mov.l @r15+,r13 */
L8c64f070: *stop=0x8c64f070u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64f072u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c64efcc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64efcc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64efe8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64efcc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64efe8u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64eff8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64efcc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64eff8u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64effe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64efcc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64effeu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f01e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64efcc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f01eu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f03a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64efcc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f03au);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f040(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64efcc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f040u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f04c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64efcc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f04cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f052(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64efcc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f052u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f058(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64efcc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f058u);}

static int sa_player_texture_link_run_8c64e054(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64e05cu)goto L8c64e05c;if(resume_pc==0x8c64e06eu)goto L8c64e06e;goto L8c64e054;
L8c64e054: *stop=0x8c64e054u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64e056; /* sts.l PR,@-r15 */
L8c64e056: *stop=0x8c64e056u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e130u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e058; /* mov.l 0x8c64e130,r3 */
L8c64e058: target=r[3];*pr=0x8c64e05cu;*stop=0x8c64e05au;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e05c; /* jsr @r3 */
L8c64e05c: *stop=0x8c64e05cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e134u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e05e; /* mov.l 0x8c64e134,r2 */
L8c64e05e: *stop=0x8c64e05eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64e060; /* tst r0,r0 */
L8c64e060: take=(*sr&1u);*stop=0x8c64e062u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;if(take)goto L8c64e072;goto L8c64e064; /* bt/s 0x8c64e072 */
L8c64e064: *stop=0x8c64e064u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e13cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e066; /* mov.l 0x8c64e13c,r2 */
L8c64e066: *stop=0x8c64e066u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e134u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e068; /* mov.l 0x8c64e134,r3 */
L8c64e068: *stop=0x8c64e068u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e138u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64e06a; /* mov.l 0x8c64e138,r5 */
L8c64e06a: target=r[2];*pr=0x8c64e06eu;*stop=0x8c64e06cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e06e; /* jsr @r2 */
L8c64e06e: *stop=0x8c64e06eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64e070; /* tst r0,r0 */
L8c64e070: if(!(*sr&1u))goto L8c64e07e;goto L8c64e072; /* bf 0x8c64e07e */
L8c64e072: *stop=0x8c64e072u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e140u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e074; /* mov.l 0x8c64e140,r1 */
L8c64e074: *stop=0x8c64e074u;r[2]=2u;goto L8c64e076; /* mov #0x2,r2 */
L8c64e076: *stop=0x8c64e076u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c64e078; /* mov.l r2,@r1 */
L8c64e078: *stop=0x8c64e078u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64e07a; /* lds.l @r15+,PR */
L8c64e07a: *stop=0x8c64e07au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64e07cu;r[0]=4294967294u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c64e07e: *stop=0x8c64e07eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e138u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e080; /* mov.l 0x8c64e138,r1 */
L8c64e080: *stop=0x8c64e080u;r[4]=0u;goto L8c64e082; /* mov #0x0,r4 */
L8c64e082: *stop=0x8c64e082u;r[3]=sx(m,bus,&io_status,0x8c64e12eu,2);if(io_status)return io_status;goto L8c64e084; /* mov.w 0x8c64e12e,r3 */
L8c64e084: *stop=0x8c64e084u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e086; /* mov.l @r1,r2 */
L8c64e086: *stop=0x8c64e086u;r[1]=4294967285u;goto L8c64e088; /* mov #-0xb,r1 */
L8c64e088: *stop=0x8c64e088u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e144u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e08a; /* mov.l 0x8c64e144,r0 */
L8c64e08a: *stop=0x8c64e08au;r[2]+=r[3];goto L8c64e08c; /* add r3,r2 */
L8c64e08c: *stop=0x8c64e08cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e148u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e08e; /* mov.l 0x8c64e148,r3 */
L8c64e08e: *stop=0x8c64e08eu;{uint32_t shift=r[1];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c64e090; /* shad r1,r2 */
L8c64e090: *stop=0x8c64e090u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e150u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e092; /* mov.l 0x8c64e150,r1 */
L8c64e092: *stop=0x8c64e092u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c64e094; /* mov.l r2,@r0 */
L8c64e094: *stop=0x8c64e094u;r[0]=r[4];goto L8c64e096; /* mov r4,r0 */
L8c64e096: *stop=0x8c64e096u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e14cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e098; /* mov.l 0x8c64e14c,r2 */
L8c64e098: *stop=0x8c64e098u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c64e09a; /* mov.l r4,@r3 */
L8c64e09a: *stop=0x8c64e09au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c64e09c; /* mov.l r4,@r2 */
L8c64e09c: *stop=0x8c64e09cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e154u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e09e; /* mov.l 0x8c64e154,r3 */
L8c64e09e: *stop=0x8c64e09eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e158u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e0a0; /* mov.l 0x8c64e158,r2 */
L8c64e0a0: *stop=0x8c64e0a0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c64e0a2; /* mov.l r4,@r1 */
L8c64e0a2: *stop=0x8c64e0a2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c64e0a4; /* mov.l r4,@r3 */
L8c64e0a4: *stop=0x8c64e0a4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c64e0a6; /* mov.l r4,@r2 */
L8c64e0a6: *stop=0x8c64e0a6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64e0a8; /* lds.l @r15+,PR */
L8c64e0a8: *stop=0x8c64e0a8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64e0aau;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c64e054(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e054(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e05c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e054(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e05cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e06e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e054(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e06eu);}

SA_STAGE_EXPORT int sa_player_texture_link_8c64e0ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64e0ac;
L8c64e0ac: *stop=0x8c64e0acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e15cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e0ae; /* mov.l 0x8c64e15c,r2 */
L8c64e0ae: *stop=0x8c64e0aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e134u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e0b0; /* mov.l 0x8c64e134,r3 */
L8c64e0b0: target=r[2];*stop=0x8c64e0b2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

static int sa_player_texture_link_run_8c64ed8a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64eda6u)goto L8c64eda6;if(resume_pc==0x8c64edb8u)goto L8c64edb8;if(resume_pc==0x8c64edd6u)goto L8c64edd6;if(resume_pc==0x8c64edeau)goto L8c64edea;goto L8c64ed8a;
L8c64ed8a: *stop=0x8c64ed8au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ed8c; /* mov.l r14,@-r15 */
L8c64ed8c: *stop=0x8c64ed8cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ed8e; /* mov.l r13,@-r15 */
L8c64ed8e: *stop=0x8c64ed8eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64ed90; /* mov.l r12,@-r15 */
L8c64ed90: *stop=0x8c64ed90u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64ed92; /* mov.l r11,@-r15 */
L8c64ed92: *stop=0x8c64ed92u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ed94; /* sts.l PR,@-r15 */
L8c64ed94: *stop=0x8c64ed94u;r[15]+=4294967272u;goto L8c64ed96; /* add #-0x18,r15 */
L8c64ed96: *stop=0x8c64ed96u;r[13]=r[5];goto L8c64ed98; /* mov r5,r13 */
L8c64ed98: *stop=0x8c64ed98u;r[5]=r[4];goto L8c64ed9a; /* mov r4,r5 */
L8c64ed9a: *stop=0x8c64ed9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ed9c; /* mov.l 0x8c64ee78,r3 */
L8c64ed9c: *stop=0x8c64ed9cu;r[6]=r[15];goto L8c64ed9e; /* mov r15,r6 */
L8c64ed9e: *stop=0x8c64ed9eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c64eda0; /* mov.l r4,@(0x4,r15) */
L8c64eda0: *stop=0x8c64eda0u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64eda2; /* mov.l @(0x4,r5),r5 */
L8c64eda2: target=r[3];*pr=0x8c64eda6u;*stop=0x8c64eda4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64eda6; /* jsr @r3 */
L8c64eda6: *stop=0x8c64eda6u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64eda8; /* cmp/pz r0 */
L8c64eda8: take=(*sr&1u);*stop=0x8c64edaau;r[4]=r[0];if(take)goto L8c64edb0;goto L8c64edac; /* bt/s 0x8c64edb0 */
L8c64edac: *stop=0x8c64edaeu;r[0]=r[4];goto L8c64edee; /* bra 0x8c64edee */
L8c64edb0: *stop=0x8c64edb0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee7cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64edb2; /* mov.l 0x8c64ee7c,r3 */
L8c64edb2: *stop=0x8c64edb2u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64edb4; /* mov.l @(0x18,r13),r14 */
L8c64edb4: target=r[3];*pr=0x8c64edb8u;*stop=0x8c64edb6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64edb8; /* jsr @r3 */
L8c64edb8: *stop=0x8c64edb8u;r[12]=r[0];goto L8c64edba; /* mov r0,r12 */
L8c64edba: *stop=0x8c64edbau;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c64edbc; /* tst r12,r12 */
L8c64edbc: if(!(*sr&1u))goto L8c64edc8;goto L8c64edbe; /* bf 0x8c64edc8 */
L8c64edbe: *stop=0x8c64edbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee80u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64edc0; /* mov.l 0x8c64ee80,r2 */
L8c64edc0: *stop=0x8c64edc0u;r[3]=7u;goto L8c64edc2; /* mov #0x7,r3 */
L8c64edc2: *stop=0x8c64edc2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64edc4; /* mov.l r3,@r2 */
L8c64edc4: *stop=0x8c64edc6u;r[0]=4294967289u;goto L8c64edee; /* bra 0x8c64edee */
L8c64edc8: *stop=0x8c64edc8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}goto L8c64edca; /* mov.l @r15,r11 */
L8c64edca: *stop=0x8c64edcau;r[5]=r[14];goto L8c64edcc; /* mov r14,r5 */
L8c64edcc: *stop=0x8c64edccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64edce; /* mov.l 0x8c64ee68,r3 */
L8c64edce: *stop=0x8c64edceu;r[11]-=r[14];goto L8c64edd0; /* sub r14,r11 */
L8c64edd0: *stop=0x8c64edd0u;r[11]+=4294967288u;goto L8c64edd2; /* add #-0x8,r11 */
L8c64edd2: target=r[3];*pr=0x8c64edd6u;*stop=0x8c64edd4u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64edd6; /* jsr @r3 */
L8c64edd6: *stop=0x8c64edd6u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64edd8; /* cmp/pz r0 */
L8c64edd8: if((*sr&1u))goto L8c64edde;goto L8c64edda; /* bt 0x8c64edde */
L8c64edda: *stop=0x8c64eddcu;r[0]=4294967295u;goto L8c64edee; /* bra 0x8c64edee */
L8c64edde: *stop=0x8c64eddeu;*sr=(*sr&~1u)|((int32_t)r[11]>0);goto L8c64ede0; /* cmp/pl r11 */
L8c64ede0: if(!(*sr&1u))goto L8c64edea;goto L8c64ede2; /* bf 0x8c64edea */
L8c64ede2: *stop=0x8c64ede2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee84u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ede4; /* mov.l 0x8c64ee84,r2 */
L8c64ede4: *stop=0x8c64ede4u;r[5]=1u;goto L8c64ede6; /* mov #0x1,r5 */
L8c64ede6: target=r[2];*pr=0x8c64edeau;*stop=0x8c64ede8u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64edea; /* jsr @r2 */
L8c64edea: *stop=0x8c64edeau;r[0]=1u;goto L8c64edec; /* mov #0x1,r0 */
L8c64edec: *stop=0x8c64edecu;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[12],4);if(io_status)return io_status;goto L8c64edee; /* mov.l r12,@r13 */
L8c64edee: *stop=0x8c64edeeu;r[15]+=24u;goto L8c64edf0; /* add #0x18,r15 */
L8c64edf0: *stop=0x8c64edf0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64edf2; /* lds.l @r15+,PR */
L8c64edf2: *stop=0x8c64edf2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64edf4; /* mov.l @r15+,r11 */
L8c64edf4: *stop=0x8c64edf4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64edf6; /* mov.l @r15+,r12 */
L8c64edf6: *stop=0x8c64edf6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64edf8; /* mov.l @r15+,r13 */
L8c64edf8: *stop=0x8c64edf8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64edfau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c64ed8a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ed8a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64eda6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ed8a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64eda6u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64edb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ed8a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64edb8u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64edd6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ed8a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64edd6u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64edea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ed8a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64edeau);}

static int sa_player_texture_link_run_8c64ec58(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64ec7au)goto L8c64ec7a;if(resume_pc==0x8c64ecd0u)goto L8c64ecd0;if(resume_pc==0x8c64ecdcu)goto L8c64ecdc;if(resume_pc==0x8c64eceau)goto L8c64ecea;if(resume_pc==0x8c64ecfcu)goto L8c64ecfc;if(resume_pc==0x8c64ed34u)goto L8c64ed34;if(resume_pc==0x8c64ed40u)goto L8c64ed40;if(resume_pc==0x8c64ed4eu)goto L8c64ed4e;if(resume_pc==0x8c64ed52u)goto L8c64ed52;if(resume_pc==0x8c64ed64u)goto L8c64ed64;if(resume_pc==0x8c64ed70u)goto L8c64ed70;goto L8c64ec58;
L8c64ec58: *stop=0x8c64ec58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ec5a; /* mov.l r14,@-r15 */
L8c64ec5a: *stop=0x8c64ec5au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ec5c; /* mov.l r13,@-r15 */
L8c64ec5c: *stop=0x8c64ec5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64ec5e; /* mov.l r12,@-r15 */
L8c64ec5e: *stop=0x8c64ec5eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64ec60; /* mov.l r11,@-r15 */
L8c64ec60: *stop=0x8c64ec60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64ec62; /* mov.l r10,@-r15 */
L8c64ec62: *stop=0x8c64ec62u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64ec64; /* mov.l r9,@-r15 */
L8c64ec64: *stop=0x8c64ec64u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64ec66; /* mov.l r8,@-r15 */
L8c64ec66: *stop=0x8c64ec66u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ec68; /* sts.l PR,@-r15 */
L8c64ec68: *stop=0x8c64ec68u;r[15]+=4294967272u;goto L8c64ec6a; /* add #-0x18,r15 */
L8c64ec6a: *stop=0x8c64ec6au;r[10]=r[5];goto L8c64ec6c; /* mov r5,r10 */
L8c64ec6c: *stop=0x8c64ec6cu;r[5]=r[4];goto L8c64ec6e; /* mov r4,r5 */
L8c64ec6e: *stop=0x8c64ec6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ed0cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ec70; /* mov.l 0x8c64ed0c,r3 */
L8c64ec70: *stop=0x8c64ec70u;r[6]=r[15];goto L8c64ec72; /* mov r15,r6 */
L8c64ec72: *stop=0x8c64ec72u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c64ec74; /* mov.l r4,@(0x4,r15) */
L8c64ec74: *stop=0x8c64ec74u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ec76; /* mov.l @(0x4,r5),r5 */
L8c64ec76: target=r[3];*pr=0x8c64ec7au;*stop=0x8c64ec78u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ec7a; /* jsr @r3 */
L8c64ec7a: *stop=0x8c64ec7au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64ec7c; /* cmp/pz r0 */
L8c64ec7c: take=(*sr&1u);*stop=0x8c64ec7eu;r[4]=r[0];if(take)goto L8c64ec84;goto L8c64ec80; /* bt/s 0x8c64ec84 */
L8c64ec80: *stop=0x8c64ec82u;r[0]=r[4];goto L8c64ed76; /* bra 0x8c64ed76 */
L8c64ec84: *stop=0x8c64ec84u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ec86; /* mov.l @r15,r3 */
L8c64ec86: *stop=0x8c64ec86u;if(!valid_access(r[10]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x18u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64ec88; /* mov.l @(0x18,r10),r14 */
L8c64ec88: *stop=0x8c64ec88u;r[3]+=4294967288u;goto L8c64ec8a; /* add #-0x8,r3 */
L8c64ec8a: *stop=0x8c64ec8au;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[14]);goto L8c64ec8c; /* cmp/gt r14,r3 */
L8c64ec8c: *stop=0x8c64ec8cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64ec8e; /* mov.l r3,@r15 */
L8c64ec8e: take=!(*sr&1u);*stop=0x8c64ec90u;r[5]=0u;if(take)goto L8c64ec98;goto L8c64ec92; /* bf/s 0x8c64ec98 */
L8c64ec92: *stop=0x8c64ec92u;r[3]-=r[14];goto L8c64ec94; /* sub r14,r3 */
L8c64ec94: *stop=0x8c64ec96u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64ec9c; /* bra 0x8c64ec9c */
L8c64ec98: *stop=0x8c64ec98u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64ec9a; /* mov.l r5,@r15 */
L8c64ec9a: *stop=0x8c64ec9au;r[14]=r[3];goto L8c64ec9c; /* mov r3,r14 */
L8c64ec9c: *stop=0x8c64ec9cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ed10u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c64ec9e; /* mov.l 0x8c64ed10,r8 */
L8c64ec9e: *stop=0x8c64ec9eu;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64eca0; /* mov.l @r8,r4 */
L8c64eca0: *stop=0x8c64eca0u;*sr=(*sr&~1u)|(r[14]>r[4]);goto L8c64eca2; /* cmp/hi r4,r14 */
L8c64eca2: take=(*sr&1u);*stop=0x8c64eca4u;r[11]=r[5];if(take)goto L8c64ecaa;goto L8c64eca6; /* bt/s 0x8c64ecaa */
L8c64eca6: *stop=0x8c64eca8u;r[12]=r[14];goto L8c64ecac; /* bra 0x8c64ecac */
L8c64ecaa: *stop=0x8c64ecaau;r[12]=r[4];goto L8c64ecac; /* mov r4,r12 */
L8c64ecac: *stop=0x8c64ecacu;if(!valid_access(r[10]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64ecae; /* mov.l @(0x4,r10),r4 */
L8c64ecae: *stop=0x8c64ecaeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ed14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ecb0; /* mov.l 0x8c64ed14,r2 */
L8c64ecb0: *stop=0x8c64ecb0u;r[3]=sx(m,bus,&io_status,0x8c64ed08u,2);if(io_status)return io_status;goto L8c64ecb2; /* mov.w 0x8c64ed08,r3 */
L8c64ecb2: *stop=0x8c64ecb2u;r[4]>>=16;goto L8c64ecb4; /* shlr16 r4 */
L8c64ecb4: *stop=0x8c64ecb4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ed1cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64ecb6; /* mov.l 0x8c64ed1c,r13 */
L8c64ecb6: *stop=0x8c64ecb6u;r[4]&=r[2];goto L8c64ecb8; /* and r2,r4 */
L8c64ecb8: *stop=0x8c64ecb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ed18u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c64ecba; /* mov.l 0x8c64ed18,r9 */
L8c64ecba: *stop=0x8c64ecbau;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c64ecbc; /* cmp/eq r3,r4 */
L8c64ecbc: if((*sr&1u))goto L8c64ed2c;goto L8c64ecbe; /* bt 0x8c64ed2c */
L8c64ecbe: *stop=0x8c64ecbeu;r[0]=sx(m,bus,&io_status,0x8c64ed0au,2);if(io_status)return io_status;goto L8c64ecc0; /* mov.w 0x8c64ed0a,r0 */
L8c64ecc0: *stop=0x8c64ecc0u;*sr=(*sr&~1u)|(r[0]==r[4]);goto L8c64ecc2; /* cmp/eq r0,r4 */
L8c64ecc2: if((*sr&1u))goto L8c64ed2c;goto L8c64ecc4; /* bt 0x8c64ed2c */
L8c64ecc4: *stop=0x8c64ecc4u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c64ecc6; /* cmp/pl r14 */
L8c64ecc6: if(!(*sr&1u))goto L8c64ed56;goto L8c64ecc8; /* bf 0x8c64ed56 */
L8c64ecc8: *stop=0x8c64ecc8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ed20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ecca; /* mov.l 0x8c64ed20,r2 */
L8c64ecca: *stop=0x8c64eccau;r[5]=r[12];goto L8c64eccc; /* mov r12,r5 */
L8c64eccc: target=r[2];*pr=0x8c64ecd0u;*stop=0x8c64ecceu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ecd0; /* jsr @r2 */
L8c64ecd0: *stop=0x8c64ecd0u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64ecd2; /* cmp/pz r0 */
L8c64ecd2: if(!(*sr&1u))goto L8c64ed68;goto L8c64ecd4; /* bf 0x8c64ed68 */
L8c64ecd4: *stop=0x8c64ecd4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ed24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ecd6; /* mov.l 0x8c64ed24,r2 */
L8c64ecd6: *stop=0x8c64ecd6u;r[5]=r[12];goto L8c64ecd8; /* mov r12,r5 */
L8c64ecd8: target=r[2];*pr=0x8c64ecdcu;*stop=0x8c64ecdau;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ecdc; /* jsr @r2 */
L8c64ecdc: *stop=0x8c64ecdcu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ecde; /* mov.l @r13,r5 */
L8c64ecde: *stop=0x8c64ecdeu;r[7]=r[12];goto L8c64ece0; /* mov r12,r7 */
L8c64ece0: *stop=0x8c64ece0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ed28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ece2; /* mov.l 0x8c64ed28,r3 */
L8c64ece2: *stop=0x8c64ece2u;r[4]=r[10];goto L8c64ece4; /* mov r10,r4 */
L8c64ece4: *stop=0x8c64ece4u;r[6]=r[11];goto L8c64ece6; /* mov r11,r6 */
L8c64ece6: target=r[3];*pr=0x8c64eceau;*stop=0x8c64ece8u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ecea; /* jsr @r3 */
L8c64ecea: *stop=0x8c64eceau;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ecec; /* mov.l @r8,r2 */
L8c64ecec: *stop=0x8c64ececu;r[4]=r[14];goto L8c64ecee; /* mov r14,r4 */
L8c64ecee: *stop=0x8c64eceeu;r[11]+=r[12];goto L8c64ecf0; /* add r12,r11 */
L8c64ecf0: *stop=0x8c64ecf0u;r[4]-=r[11];goto L8c64ecf2; /* sub r11,r4 */
L8c64ecf2: *stop=0x8c64ecf2u;*sr=(*sr&~1u)|(r[4]>=r[2]);goto L8c64ecf4; /* cmp/hs r2,r4 */
L8c64ecf4: if((*sr&1u))goto L8c64ecf8;goto L8c64ecf6; /* bt 0x8c64ecf8 */
L8c64ecf6: *stop=0x8c64ecf6u;r[12]=r[4];goto L8c64ecf8; /* mov r4,r12 */
L8c64ecf8: target=r[9];*pr=0x8c64ecfcu;*stop=0x8c64ecfau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ecfc; /* jsr @r9 */
L8c64ecfc: *stop=0x8c64ecfcu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64ecfe; /* tst r0,r0 */
L8c64ecfe: if(!(*sr&1u))goto L8c64ecf8;goto L8c64ed00; /* bf 0x8c64ecf8 */
L8c64ed00: *stop=0x8c64ed00u;*sr=(*sr&~1u)|(r[14]==r[11]);goto L8c64ed02; /* cmp/eq r14,r11 */
L8c64ed02: if(!(*sr&1u))goto L8c64ecc8;goto L8c64ed04; /* bf 0x8c64ecc8 */
L8c64ed04: *stop=0x8c64ed06u;goto L8c64ed56; /* bra 0x8c64ed56 */
L8c64ed2c: *stop=0x8c64ed2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee68u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ed2e; /* mov.l 0x8c64ee68,r2 */
L8c64ed2e: *stop=0x8c64ed2eu;r[5]=r[14];goto L8c64ed30; /* mov r14,r5 */
L8c64ed30: target=r[2];*pr=0x8c64ed34u;*stop=0x8c64ed32u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ed34; /* jsr @r2 */
L8c64ed34: *stop=0x8c64ed34u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64ed36; /* cmp/pz r0 */
L8c64ed36: if(!(*sr&1u))goto L8c64ed68;goto L8c64ed38; /* bf 0x8c64ed68 */
L8c64ed38: *stop=0x8c64ed38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee6cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ed3a; /* mov.l 0x8c64ee6c,r3 */
L8c64ed3a: *stop=0x8c64ed3au;r[5]=r[14];goto L8c64ed3c; /* mov r14,r5 */
L8c64ed3c: target=r[3];*pr=0x8c64ed40u;*stop=0x8c64ed3eu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ed40; /* jsr @r3 */
L8c64ed40: *stop=0x8c64ed40u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ed42; /* mov.l @r13,r5 */
L8c64ed42: *stop=0x8c64ed42u;r[7]=0u;goto L8c64ed44; /* mov #0x0,r7 */
L8c64ed44: *stop=0x8c64ed44u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee70u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ed46; /* mov.l 0x8c64ee70,r2 */
L8c64ed46: *stop=0x8c64ed46u;r[4]=r[10];goto L8c64ed48; /* mov r10,r4 */
L8c64ed48: *stop=0x8c64ed48u;r[6]=r[7];goto L8c64ed4a; /* mov r7,r6 */
L8c64ed4a: target=r[2];*pr=0x8c64ed4eu;*stop=0x8c64ed4cu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ed4e; /* jsr @r2 */
L8c64ed4e: target=r[9];*pr=0x8c64ed52u;*stop=0x8c64ed50u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ed52; /* jsr @r9 */
L8c64ed52: *stop=0x8c64ed52u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64ed54; /* tst r0,r0 */
L8c64ed54: if(!(*sr&1u))goto L8c64ed4e;goto L8c64ed56; /* bf 0x8c64ed4e */
L8c64ed56: *stop=0x8c64ed56u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ed58; /* mov.l @r15,r3 */
L8c64ed58: *stop=0x8c64ed58u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c64ed5a; /* cmp/pl r3 */
L8c64ed5a: if(!(*sr&1u))goto L8c64ed74;goto L8c64ed5c; /* bf 0x8c64ed74 */
L8c64ed5c: *stop=0x8c64ed5cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ed5e; /* mov.l 0x8c64ee68,r3 */
L8c64ed5e: *stop=0x8c64ed5eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ed60; /* mov.l @r15,r5 */
L8c64ed60: target=r[3];*pr=0x8c64ed64u;*stop=0x8c64ed62u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ed64; /* jsr @r3 */
L8c64ed64: *stop=0x8c64ed64u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64ed66; /* cmp/pz r0 */
L8c64ed66: if((*sr&1u))goto L8c64ed74;goto L8c64ed68; /* bt 0x8c64ed74 */
L8c64ed68: *stop=0x8c64ed68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ee74u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ed6a; /* mov.l 0x8c64ee74,r3 */
L8c64ed6a: *stop=0x8c64ed6au;r[4]=r[10];goto L8c64ed6c; /* mov r10,r4 */
L8c64ed6c: target=r[3];*pr=0x8c64ed70u;*stop=0x8c64ed6eu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ed70; /* jsr @r3 */
L8c64ed70: *stop=0x8c64ed72u;r[0]=4294967295u;goto L8c64ed76; /* bra 0x8c64ed76 */
L8c64ed74: *stop=0x8c64ed74u;r[0]=1u;goto L8c64ed76; /* mov #0x1,r0 */
L8c64ed76: *stop=0x8c64ed76u;r[15]+=24u;goto L8c64ed78; /* add #0x18,r15 */
L8c64ed78: *stop=0x8c64ed78u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64ed7a; /* lds.l @r15+,PR */
L8c64ed7a: *stop=0x8c64ed7au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64ed7c; /* mov.l @r15+,r8 */
L8c64ed7c: *stop=0x8c64ed7cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64ed7e; /* mov.l @r15+,r9 */
L8c64ed7e: *stop=0x8c64ed7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64ed80; /* mov.l @r15+,r10 */
L8c64ed80: *stop=0x8c64ed80u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64ed82; /* mov.l @r15+,r11 */
L8c64ed82: *stop=0x8c64ed82u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64ed84; /* mov.l @r15+,r12 */
L8c64ed84: *stop=0x8c64ed84u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64ed86; /* mov.l @r15+,r13 */
L8c64ed86: *stop=0x8c64ed86u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64ed88u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c64ec58(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ec7a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ec7au);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ecd0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ecd0u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ecdc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ecdcu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ecea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64eceau);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ecfc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ecfcu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ed34(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ed34u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ed40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ed40u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ed4e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ed4eu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ed52(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ed52u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ed64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ed64u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64ed70(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64ec58(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ed70u);}

static int sa_player_texture_link_run_8c64f074(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64f0aau)goto L8c64f0aa;if(resume_pc==0x8c64f0bcu)goto L8c64f0bc;if(resume_pc==0x8c64f0c4u)goto L8c64f0c4;if(resume_pc==0x8c64f0d4u)goto L8c64f0d4;if(resume_pc==0x8c64f0e2u)goto L8c64f0e2;if(resume_pc==0x8c64f10cu)goto L8c64f10c;if(resume_pc==0x8c64f182u)goto L8c64f182;if(resume_pc==0x8c64f18eu)goto L8c64f18e;goto L8c64f074;
L8c64f074: *stop=0x8c64f074u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64f076; /* mov.l r14,@-r15 */
L8c64f076: *stop=0x8c64f076u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64f078; /* mov.l r13,@-r15 */
L8c64f078: *stop=0x8c64f078u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64f07a; /* mov.l r12,@-r15 */
L8c64f07a: *stop=0x8c64f07au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64f07c; /* mov.l r11,@-r15 */
L8c64f07c: *stop=0x8c64f07cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64f07e; /* mov.l r10,@-r15 */
L8c64f07e: *stop=0x8c64f07eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64f080; /* mov.l r9,@-r15 */
L8c64f080: *stop=0x8c64f080u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64f082; /* sts.l PR,@-r15 */
L8c64f082: *stop=0x8c64f082u;r[15]+=4294967276u;goto L8c64f084; /* add #-0x14,r15 */
L8c64f084: *stop=0x8c64f084u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f086; /* mov.l @(0x4,r4),r2 */
L8c64f086: *stop=0x8c64f086u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f154u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f088; /* mov.l 0x8c64f154,r3 */
L8c64f088: *stop=0x8c64f088u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f150u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64f08a; /* mov.l 0x8c64f150,r12 */
L8c64f08a: *stop=0x8c64f08au;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c64f08c; /* tst r3,r2 */
L8c64f08c: if((*sr&1u))goto L8c64f092;goto L8c64f08e; /* bt 0x8c64f092 */
L8c64f08e: *stop=0x8c64f090u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64f188; /* bra 0x8c64f188 */
L8c64f092: *stop=0x8c64f092u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f144u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f094; /* mov.l 0x8c64f144,r2 */
L8c64f094: *stop=0x8c64f094u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f164u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64f096; /* mov.l 0x8c64f164,r14 */
L8c64f096: *stop=0x8c64f096u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f098; /* mov.l @r2,r1 */
L8c64f098: *stop=0x8c64f098u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f158u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c64f09a; /* mov.l 0x8c64f158,r9 */
L8c64f09a: *stop=0x8c64f09au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f15cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64f09c; /* mov.l 0x8c64f15c,r10 */
L8c64f09c: *stop=0x8c64f09cu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c64f09e; /* tst r1,r1 */
L8c64f09e: *stop=0x8c64f09eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f160u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64f0a0; /* mov.l 0x8c64f160,r11 */
L8c64f0a0: if(!(*sr&1u))goto L8c64f17a;goto L8c64f0a2; /* bf 0x8c64f17a */
L8c64f0a2: *stop=0x8c64f0a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f168u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64f0a4; /* mov.l 0x8c64f168,r13 */
L8c64f0a4: *stop=0x8c64f0a4u;r[5]=4u;goto L8c64f0a6; /* mov #0x4,r5 */
L8c64f0a6: target=r[13];*pr=0x8c64f0aau;*stop=0x8c64f0a8u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f0aa; /* jsr @r13 */
L8c64f0aa: *stop=0x8c64f0aau;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64f0ac; /* cmp/pz r0 */
L8c64f0ac: if(!(*sr&1u))goto L8c64f0d8;goto L8c64f0ae; /* bf 0x8c64f0d8 */
L8c64f0ae: *stop=0x8c64f0aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f16cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f0b0; /* mov.l 0x8c64f16c,r1 */
L8c64f0b0: *stop=0x8c64f0b0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64f0b2; /* mov.l @r15,r0 */
L8c64f0b2: *stop=0x8c64f0b2u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64f0b4; /* cmp/eq r1,r0 */
L8c64f0b4: if(!(*sr&1u))goto L8c64f0cc;goto L8c64f0b6; /* bf 0x8c64f0cc */
L8c64f0b6: *stop=0x8c64f0b6u;r[5]=1u;goto L8c64f0b8; /* mov #0x1,r5 */
L8c64f0b8: target=r[12];*pr=0x8c64f0bcu;*stop=0x8c64f0bau;r[4]=4294967292u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f0bc; /* jsr @r12 */
L8c64f0bc: *stop=0x8c64f0bcu;r[4]=r[15];goto L8c64f0be; /* mov r15,r4 */
L8c64f0be: *stop=0x8c64f0beu;r[5]=12u;goto L8c64f0c0; /* mov #0xc,r5 */
L8c64f0c0: target=r[13];*pr=0x8c64f0c4u;*stop=0x8c64f0c2u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f0c4; /* jsr @r13 */
L8c64f0c4: *stop=0x8c64f0c4u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64f0c6; /* cmp/pz r0 */
L8c64f0c6: if((*sr&1u))goto L8c64f0e2;goto L8c64f0c8; /* bt 0x8c64f0e2 */
L8c64f0c8: *stop=0x8c64f0cau;goto L8c64f0d8; /* bra 0x8c64f0d8 */
L8c64f0cc: *stop=0x8c64f0ccu;r[4]=r[15];goto L8c64f0ce; /* mov r15,r4 */
L8c64f0ce: *stop=0x8c64f0ceu;r[5]=4u;goto L8c64f0d0; /* mov #0x4,r5 */
L8c64f0d0: target=r[13];*pr=0x8c64f0d4u;*stop=0x8c64f0d2u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f0d4; /* jsr @r13 */
L8c64f0d4: *stop=0x8c64f0d4u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64f0d6; /* cmp/pz r0 */
L8c64f0d6: if((*sr&1u))goto L8c64f0dc;goto L8c64f0d8; /* bt 0x8c64f0dc */
L8c64f0d8: *stop=0x8c64f0dau;r[0]=4294967295u;goto L8c64f190; /* bra 0x8c64f190 */
L8c64f0dc: *stop=0x8c64f0dcu;r[5]=1u;goto L8c64f0de; /* mov #0x1,r5 */
L8c64f0de: target=r[12];*pr=0x8c64f0e2u;*stop=0x8c64f0e0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f0e2; /* jsr @r12 */
L8c64f0e2: *stop=0x8c64f0e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f0e4; /* mov.l @r15,r2 */
L8c64f0e4: *stop=0x8c64f0e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f170u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64f0e6; /* mov.l 0x8c64f170,r4 */
L8c64f0e6: *stop=0x8c64f0e6u;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c64f0e8; /* cmp/eq r4,r2 */
L8c64f0e8: if((*sr&1u))goto L8c64f0f2;goto L8c64f0ea; /* bt 0x8c64f0f2 */
L8c64f0ea: *stop=0x8c64f0eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f0ec; /* mov.l @r15,r3 */
L8c64f0ec: *stop=0x8c64f0ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f16cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f0ee; /* mov.l 0x8c64f16c,r1 */
L8c64f0ee: *stop=0x8c64f0eeu;*sr=(*sr&~1u)|(r[1]==r[3]);goto L8c64f0f0; /* cmp/eq r1,r3 */
L8c64f0f0: if(!(*sr&1u))goto L8c64f0a2;goto L8c64f0f2; /* bf 0x8c64f0a2 */
L8c64f0f2: *stop=0x8c64f0f2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f0f4; /* mov.l @r15,r2 */
L8c64f0f4: *stop=0x8c64f0f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f16cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f0f6; /* mov.l 0x8c64f16c,r3 */
L8c64f0f6: *stop=0x8c64f0f6u;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c64f0f8; /* cmp/eq r3,r2 */
L8c64f0f8: if(!(*sr&1u))goto L8c64f104;goto L8c64f0fa; /* bf 0x8c64f104 */
L8c64f0fa: *stop=0x8c64f0fcu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64f188; /* bra 0x8c64f188 */
L8c64f0fe: *stop=0x8c64f0feu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f100; /* mov.l @r14,r3 */
L8c64f100: *stop=0x8c64f100u;r[3]+=4294967295u;goto L8c64f102; /* add #-0x1,r3 */
L8c64f102: *stop=0x8c64f102u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c64f104; /* mov.l r3,@r14 */
L8c64f104: *stop=0x8c64f104u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64f106; /* mov.l @r11,r5 */
L8c64f106: *stop=0x8c64f106u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64f108; /* mov.l @r14,r6 */
L8c64f108: target=r[9];*pr=0x8c64f10cu;*stop=0x8c64f10au;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f10c; /* jsr @r9 */
L8c64f10c: *stop=0x8c64f10cu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c64f10e; /* cmp/eq #-0x1,r0 */
L8c64f10e: if(!(*sr&1u))goto L8c64f0fe;goto L8c64f110; /* bf 0x8c64f0fe */
L8c64f110: *stop=0x8c64f112u;goto L8c64f186; /* bra 0x8c64f186 */
L8c64f174: *stop=0x8c64f174u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64f176; /* mov.l @r14,r3 */
L8c64f176: *stop=0x8c64f176u;r[3]+=4294967295u;goto L8c64f178; /* add #-0x1,r3 */
L8c64f178: *stop=0x8c64f178u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c64f17a; /* mov.l r3,@r14 */
L8c64f17a: *stop=0x8c64f17au;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64f17c; /* mov.l @r11,r5 */
L8c64f17c: *stop=0x8c64f17cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64f17e; /* mov.l @r14,r6 */
L8c64f17e: target=r[9];*pr=0x8c64f182u;*stop=0x8c64f180u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f182; /* jsr @r9 */
L8c64f182: *stop=0x8c64f182u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c64f184; /* cmp/eq #-0x1,r0 */
L8c64f184: if(!(*sr&1u))goto L8c64f174;goto L8c64f186; /* bf 0x8c64f174 */
L8c64f186: *stop=0x8c64f186u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[13]=loaded;}goto L8c64f188; /* mov.l @r14,r13 */
L8c64f188: *stop=0x8c64f188u;r[5]=0u;goto L8c64f18a; /* mov #0x0,r5 */
L8c64f18a: target=r[12];*pr=0x8c64f18eu;*stop=0x8c64f18cu;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f18e; /* jsr @r12 */
L8c64f18e: *stop=0x8c64f18eu;r[0]=r[13];goto L8c64f190; /* mov r13,r0 */
L8c64f190: *stop=0x8c64f190u;r[15]+=20u;goto L8c64f192; /* add #0x14,r15 */
L8c64f192: *stop=0x8c64f192u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64f194; /* lds.l @r15+,PR */
L8c64f194: *stop=0x8c64f194u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64f196; /* mov.l @r15+,r9 */
L8c64f196: *stop=0x8c64f196u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64f198; /* mov.l @r15+,r10 */
L8c64f198: *stop=0x8c64f198u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64f19a; /* mov.l @r15+,r11 */
L8c64f19a: *stop=0x8c64f19au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64f19c; /* mov.l @r15+,r12 */
L8c64f19c: *stop=0x8c64f19cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64f19e; /* mov.l @r15+,r13 */
L8c64f19e: *stop=0x8c64f19eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64f1a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c64f074(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f074(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f0aa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f074(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f0aau);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f0bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f074(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f0bcu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f0c4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f074(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f0c4u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f0d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f074(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f0d4u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f0e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f074(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f0e2u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f10c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f074(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f10cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f182(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f074(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f182u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f18e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f074(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f18eu);}

static int sa_player_texture_link_run_8c64f1a2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64f1b2u)goto L8c64f1b2;if(resume_pc==0x8c64f1c6u)goto L8c64f1c6;if(resume_pc==0x8c64f1ceu)goto L8c64f1ce;if(resume_pc==0x8c64f1deu)goto L8c64f1de;if(resume_pc==0x8c64f1ecu)goto L8c64f1ec;if(resume_pc==0x8c64f1fau)goto L8c64f1fa;goto L8c64f1a2;
L8c64f1a2: *stop=0x8c64f1a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64f1a4; /* mov.l r14,@-r15 */
L8c64f1a4: *stop=0x8c64f1a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64f1a6; /* mov.l r13,@-r15 */
L8c64f1a6: *stop=0x8c64f1a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64f1a8; /* sts.l PR,@-r15 */
L8c64f1a8: *stop=0x8c64f1a8u;r[15]+=4294967272u;goto L8c64f1aa; /* add #-0x18,r15 */
L8c64f1aa: *stop=0x8c64f1aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f238u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64f1ac; /* mov.l 0x8c64f238,r14 */
L8c64f1ac: *stop=0x8c64f1acu;r[5]=4u;goto L8c64f1ae; /* mov #0x4,r5 */
L8c64f1ae: target=r[14];*pr=0x8c64f1b2u;*stop=0x8c64f1b0u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f1b2; /* jsr @r14 */
L8c64f1b2: *stop=0x8c64f1b2u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64f1b4; /* cmp/pz r0 */
L8c64f1b4: if(!(*sr&1u))goto L8c64f1e2;goto L8c64f1b6; /* bf 0x8c64f1e2 */
L8c64f1b6: *stop=0x8c64f1b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f240u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64f1b8; /* mov.l 0x8c64f240,r1 */
L8c64f1b8: *stop=0x8c64f1b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64f1ba; /* mov.l @r15,r0 */
L8c64f1ba: *stop=0x8c64f1bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f23cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64f1bc; /* mov.l 0x8c64f23c,r13 */
L8c64f1bc: *stop=0x8c64f1bcu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64f1be; /* cmp/eq r1,r0 */
L8c64f1be: if(!(*sr&1u))goto L8c64f1d6;goto L8c64f1c0; /* bf 0x8c64f1d6 */
L8c64f1c0: *stop=0x8c64f1c0u;r[5]=1u;goto L8c64f1c2; /* mov #0x1,r5 */
L8c64f1c2: target=r[13];*pr=0x8c64f1c6u;*stop=0x8c64f1c4u;r[4]=4294967292u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f1c6; /* jsr @r13 */
L8c64f1c6: *stop=0x8c64f1c6u;r[4]=r[15];goto L8c64f1c8; /* mov r15,r4 */
L8c64f1c8: *stop=0x8c64f1c8u;r[5]=16u;goto L8c64f1ca; /* mov #0x10,r5 */
L8c64f1ca: target=r[14];*pr=0x8c64f1ceu;*stop=0x8c64f1ccu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f1ce; /* jsr @r14 */
L8c64f1ce: *stop=0x8c64f1ceu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64f1d0; /* cmp/pz r0 */
L8c64f1d0: if((*sr&1u))goto L8c64f1ec;goto L8c64f1d2; /* bt 0x8c64f1ec */
L8c64f1d2: *stop=0x8c64f1d4u;goto L8c64f1e2; /* bra 0x8c64f1e2 */
L8c64f1d6: *stop=0x8c64f1d6u;r[4]=r[15];goto L8c64f1d8; /* mov r15,r4 */
L8c64f1d8: *stop=0x8c64f1d8u;r[5]=4u;goto L8c64f1da; /* mov #0x4,r5 */
L8c64f1da: target=r[14];*pr=0x8c64f1deu;*stop=0x8c64f1dcu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f1de; /* jsr @r14 */
L8c64f1de: *stop=0x8c64f1deu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64f1e0; /* cmp/pz r0 */
L8c64f1e0: if((*sr&1u))goto L8c64f1e6;goto L8c64f1e2; /* bt 0x8c64f1e6 */
L8c64f1e2: *stop=0x8c64f1e4u;r[0]=4294967295u;goto L8c64f1fc; /* bra 0x8c64f1fc */
L8c64f1e6: *stop=0x8c64f1e6u;r[5]=1u;goto L8c64f1e8; /* mov #0x1,r5 */
L8c64f1e8: target=r[13];*pr=0x8c64f1ecu;*stop=0x8c64f1eau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f1ec; /* jsr @r13 */
L8c64f1ec: *stop=0x8c64f1ecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64f1ee; /* mov.l @r15,r2 */
L8c64f1ee: *stop=0x8c64f1eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64f240u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64f1f0; /* mov.l 0x8c64f240,r4 */
L8c64f1f0: *stop=0x8c64f1f0u;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c64f1f2; /* cmp/eq r4,r2 */
L8c64f1f2: if(!(*sr&1u))goto L8c64f1aa;goto L8c64f1f4; /* bf 0x8c64f1aa */
L8c64f1f4: *stop=0x8c64f1f4u;r[5]=0u;goto L8c64f1f6; /* mov #0x0,r5 */
L8c64f1f6: target=r[13];*pr=0x8c64f1fau;*stop=0x8c64f1f8u;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f1fa; /* jsr @r13 */
L8c64f1fa: *stop=0x8c64f1fau;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64f1fc; /* mov.l @(0x10,r15),r0 */
L8c64f1fc: *stop=0x8c64f1fcu;r[15]+=24u;goto L8c64f1fe; /* add #0x18,r15 */
L8c64f1fe: *stop=0x8c64f1feu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64f200; /* lds.l @r15+,PR */
L8c64f200: *stop=0x8c64f200u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64f202; /* mov.l @r15+,r13 */
L8c64f202: *stop=0x8c64f202u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64f204u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c64f1a2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f1a2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f1b2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f1a2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f1b2u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f1c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f1a2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f1c6u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f1ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f1a2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f1ceu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f1de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f1a2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f1deu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f1ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f1a2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f1ecu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64f1fa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64f1a2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64f1fau);}

SA_STAGE_EXPORT int sa_player_texture_link_8c64f206(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64f206;
L8c64f206: *stop=0x8c64f206u;r[1]=sx(m,bus,&io_status,0x8c64f22eu,2);if(io_status)return io_status;goto L8c64f208; /* mov.w 0x8c64f22e,r1 */
L8c64f208: *stop=0x8c64f208u;r[0]=r[4];goto L8c64f20a; /* mov r4,r0 */
L8c64f20a: *stop=0x8c64f20au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64f20c; /* cmp/eq r1,r0 */
L8c64f20c: if((*sr&1u))goto L8c64f224;goto L8c64f20e; /* bt 0x8c64f224 */
L8c64f20e: *stop=0x8c64f20eu;r[1]=sx(m,bus,&io_status,0x8c64f230u,2);if(io_status)return io_status;goto L8c64f210; /* mov.w 0x8c64f230,r1 */
L8c64f210: *stop=0x8c64f210u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64f212; /* cmp/eq r1,r0 */
L8c64f212: if((*sr&1u))goto L8c64f224;goto L8c64f214; /* bt 0x8c64f224 */
L8c64f214: *stop=0x8c64f214u;r[1]=sx(m,bus,&io_status,0x8c64f232u,2);if(io_status)return io_status;goto L8c64f216; /* mov.w 0x8c64f232,r1 */
L8c64f216: *stop=0x8c64f216u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64f218; /* cmp/eq r1,r0 */
L8c64f218: if((*sr&1u))goto L8c64f224;goto L8c64f21a; /* bt 0x8c64f224 */
L8c64f21a: *stop=0x8c64f21au;r[1]=sx(m,bus,&io_status,0x8c64f234u,2);if(io_status)return io_status;goto L8c64f21c; /* mov.w 0x8c64f234,r1 */
L8c64f21c: *stop=0x8c64f21cu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64f21e; /* cmp/eq r1,r0 */
L8c64f21e: if((*sr&1u))goto L8c64f224;goto L8c64f220; /* bt 0x8c64f224 */
L8c64f220: *stop=0x8c64f222u;goto L8c64f228; /* bra 0x8c64f228 */
L8c64f224: *stop=0x8c64f224u;if(*pr!=entry_pr)return -3;*stop=0x8c64f226u;r[0]=1u;return 0; /* rts */
L8c64f228: *stop=0x8c64f228u;r[0]=0u;goto L8c64f22a; /* mov #0x0,r0 */
L8c64f22a: *stop=0x8c64f22au;if(*pr!=entry_pr)return -3;*stop=0x8c64f22cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_link_8c10d3a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10d3a8;
L8c10d3a8: *stop=0x8c10d3a8u;r[6]=r[4];goto L8c10d3aa; /* mov r4,r6 */
L8c10d3aa: *stop=0x8c10d3acu;r[7]=r[5];goto L8c10d3b0; /* bra 0x8c10d3b0 */
L8c10d3ae: *stop=0x8c10d3aeu;r[6]+=1u;goto L8c10d3b0; /* add #0x1,r6 */
L8c10d3b0: *stop=0x8c10d3b0u;if(!valid_access(r[6],1u))return -3;r[2]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;goto L8c10d3b2; /* mov.b @r6,r2 */
L8c10d3b2: *stop=0x8c10d3b2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c10d3b4; /* tst r2,r2 */
L8c10d3b4: if(!(*sr&1u))goto L8c10d3ae;goto L8c10d3b6; /* bf 0x8c10d3ae */
L8c10d3b6: *stop=0x8c10d3b6u;if(!valid_access(r[7],1u))return -3;r[2]=sx(m,bus,&io_status,r[7],1);if(io_status)return io_status;r[7]+=1;goto L8c10d3b8; /* mov.b @r7+,r2 */
L8c10d3b8: *stop=0x8c10d3b8u;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[2],1);if(io_status)return io_status;goto L8c10d3ba; /* mov.b r2,@r6 */
L8c10d3ba: *stop=0x8c10d3bau;r[2]=r[2]&255u;goto L8c10d3bc; /* extu.b r2,r2 */
L8c10d3bc: *stop=0x8c10d3bcu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c10d3be; /* tst r2,r2 */
L8c10d3be: take=!(*sr&1u);*stop=0x8c10d3c0u;r[6]+=1u;if(take)goto L8c10d3b6;goto L8c10d3c2; /* bf/s 0x8c10d3b6 */
L8c10d3c2: *stop=0x8c10d3c2u;if(*pr!=entry_pr)return -3;*stop=0x8c10d3c4u;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_link_8c10dacc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10dacc;
L8c10dacc: *stop=0x8c10daccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10dace; /* mov.l r3,@-r15 */
L8c10dace: *stop=0x8c10daceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c10dad0; /* mov.l r4,@-r15 */
L8c10dad0: *stop=0x8c10dad0u;r[3]=3u;goto L8c10dad2; /* mov #0x3,r3 */
L8c10dad2: *stop=0x8c10dad2u;r[4]=r[0];goto L8c10dad4; /* mov r0,r4 */
L8c10dad4: *stop=0x8c10dad4u;r[4]&=r[3];goto L8c10dad6; /* and r3,r4 */
L8c10dad6: *stop=0x8c10dad6u;r[3]&=r[1];goto L8c10dad8; /* and r1,r3 */
L8c10dad8: *stop=0x8c10dad8u;r[3]|=r[4];goto L8c10dada; /* or r4,r3 */
L8c10dada: *stop=0x8c10dadau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c10dadc; /* tst r3,r3 */
L8c10dadc: if((*sr&1u))goto L8c10db10;goto L8c10dade; /* bt 0x8c10db10 */
L8c10dade: *stop=0x8c10dadeu;r[4]=r[0];goto L8c10dae0; /* mov r0,r4 */
L8c10dae0: *stop=0x8c10dae0u;if(!valid_access(r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[1]+=1;goto L8c10dae2; /* mov.b @r1+,r0 */
L8c10dae2: *stop=0x8c10dae2u;r[3]=r[4];goto L8c10dae4; /* mov r4,r3 */
L8c10dae4: *stop=0x8c10dae4u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10dae6; /* cmp/eq #0x0,r0 */
L8c10dae6: *stop=0x8c10dae6u;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[0],1);if(io_status)return io_status;goto L8c10dae8; /* mov.b r0,@r3 */
L8c10dae8: if((*sr&1u))goto L8c10db08;goto L8c10daea; /* bt 0x8c10db08 */
L8c10daea: *stop=0x8c10daeau;if(!valid_access(r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[1]+=1;goto L8c10daec; /* mov.b @r1+,r0 */
L8c10daec: *stop=0x8c10daecu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10daee; /* cmp/eq #0x0,r0 */
L8c10daee: *stop=0x8c10daeeu;if(!valid_access(r[3]+1u,1u))return -3;wr(m,bus,&io_status,r[3]+0x1u,r[0],1);if(io_status)return io_status;goto L8c10daf0; /* mov.b r0,@(0x1,r3) */
L8c10daf0: if((*sr&1u))goto L8c10db08;goto L8c10daf2; /* bt 0x8c10db08 */
L8c10daf2: *stop=0x8c10daf2u;if(!valid_access(r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[1]+=1;goto L8c10daf4; /* mov.b @r1+,r0 */
L8c10daf4: *stop=0x8c10daf4u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10daf6; /* cmp/eq #0x0,r0 */
L8c10daf6: *stop=0x8c10daf6u;if(!valid_access(r[3]+2u,1u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],1);if(io_status)return io_status;goto L8c10daf8; /* mov.b r0,@(0x2,r3) */
L8c10daf8: if((*sr&1u))goto L8c10db08;goto L8c10dafa; /* bt 0x8c10db08 */
L8c10dafa: *stop=0x8c10dafau;if(!valid_access(r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[1]+=1;goto L8c10dafc; /* mov.b @r1+,r0 */
L8c10dafc: *stop=0x8c10dafcu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10dafe; /* cmp/eq #0x0,r0 */
L8c10dafe: *stop=0x8c10dafeu;if(!valid_access(r[3]+3u,1u))return -3;wr(m,bus,&io_status,r[3]+0x3u,r[0],1);if(io_status)return io_status;goto L8c10db00; /* mov.b r0,@(0x3,r3) */
L8c10db00: if((*sr&1u))goto L8c10db08;goto L8c10db02; /* bt 0x8c10db08 */
L8c10db02: *stop=0x8c10db02u;if(!valid_access(r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[1]+=1;goto L8c10db04; /* mov.b @r1+,r0 */
L8c10db04: *stop=0x8c10db06u;r[3]+=4u;goto L8c10dae4; /* bra 0x8c10dae4 */
L8c10db08: *stop=0x8c10db08u;r[0]=r[4];goto L8c10db0a; /* mov r4,r0 */
L8c10db0a: *stop=0x8c10db0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c10db0c; /* mov.l @r15+,r4 */
L8c10db0c: *stop=0x8c10db0cu;if(*pr!=entry_pr)return -3;*stop=0x8c10db0eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;return 0; /* rts */
L8c10db10: *stop=0x8c10db10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10db18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10db12; /* mov.l 0x8c10db18,r3 */
L8c10db12: target=r[3];*stop=0x8c10db14u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

static int sa_player_texture_link_run_8c64c320(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64c330u)goto L8c64c330;goto L8c64c320;
L8c64c320: *stop=0x8c64c320u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64c322; /* mov.l r14,@-r15 */
L8c64c322: *stop=0x8c64c322u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64c324; /* mov.l r13,@-r15 */
L8c64c324: *stop=0x8c64c324u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64c326; /* sts.l PR,@-r15 */
L8c64c326: *stop=0x8c64c326u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c348u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64c328; /* mov.l 0x8c64c348,r13 */
L8c64c328: *stop=0x8c64c32au;r[14]=r[4];goto L8c64c334; /* bra 0x8c64c334 */
L8c64c32c: target=r[13];*pr=0x8c64c330u;*stop=0x8c64c32eu;if(!valid_access(r[14],1u))return -3;r[4]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64c330; /* jsr @r13 */
L8c64c330: *stop=0x8c64c330u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c64c332; /* mov.b r0,@r14 */
L8c64c332: *stop=0x8c64c332u;r[14]+=1u;goto L8c64c334; /* add #0x1,r14 */
L8c64c334: *stop=0x8c64c334u;if(!valid_access(r[14],1u))return -3;r[2]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c64c336; /* mov.b @r14,r2 */
L8c64c336: *stop=0x8c64c336u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64c338; /* tst r2,r2 */
L8c64c338: if(!(*sr&1u))goto L8c64c32c;goto L8c64c33a; /* bf 0x8c64c32c */
L8c64c33a: *stop=0x8c64c33au;r[3]=0u;goto L8c64c33c; /* mov #0x0,r3 */
L8c64c33c: *stop=0x8c64c33cu;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[3],1);if(io_status)return io_status;goto L8c64c33e; /* mov.b r3,@r14 */
L8c64c33e: *stop=0x8c64c33eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64c340; /* lds.l @r15+,PR */
L8c64c340: *stop=0x8c64c340u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64c342; /* mov.l @r15+,r13 */
L8c64c342: *stop=0x8c64c342u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64c344u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c64c320(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c320(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64c330(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64c320(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64c330u);}

static int sa_player_texture_link_run_8c64e0b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64e0eeu)goto L8c64e0ee;if(resume_pc==0x8c64e106u)goto L8c64e106;if(resume_pc==0x8c64e124u)goto L8c64e124;if(resume_pc==0x8c64e182u)goto L8c64e182;if(resume_pc==0x8c64e18cu)goto L8c64e18c;if(resume_pc==0x8c64e194u)goto L8c64e194;if(resume_pc==0x8c64e1ccu)goto L8c64e1cc;if(resume_pc==0x8c64e1f8u)goto L8c64e1f8;if(resume_pc==0x8c64e224u)goto L8c64e224;if(resume_pc==0x8c64e236u)goto L8c64e236;if(resume_pc==0x8c64e240u)goto L8c64e240;if(resume_pc==0x8c64e248u)goto L8c64e248;if(resume_pc==0x8c64e282u)goto L8c64e282;if(resume_pc==0x8c64e28cu)goto L8c64e28c;if(resume_pc==0x8c64e294u)goto L8c64e294;if(resume_pc==0x8c64e2acu)goto L8c64e2ac;goto L8c64e0b4;
L8c64e0b4: *stop=0x8c64e0b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64e0b6; /* mov.l r14,@-r15 */
L8c64e0b6: *stop=0x8c64e0b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64e0b8; /* mov.l r13,@-r15 */
L8c64e0b8: *stop=0x8c64e0b8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64e0ba; /* mov.l r12,@-r15 */
L8c64e0ba: *stop=0x8c64e0bau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64e0bc; /* mov.l r11,@-r15 */
L8c64e0bc: *stop=0x8c64e0bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64e0be; /* mov.l r10,@-r15 */
L8c64e0be: *stop=0x8c64e0beu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64e0c0; /* mov.l r9,@-r15 */
L8c64e0c0: *stop=0x8c64e0c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64e0c2; /* mov.l r8,@-r15 */
L8c64e0c2: *stop=0x8c64e0c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64e0c4; /* sts.l PR,@-r15 */
L8c64e0c4: *stop=0x8c64e0c4u;r[15]+=4294967280u;goto L8c64e0c6; /* add #-0x10,r15 */
L8c64e0c6: *stop=0x8c64e0c6u;r[3]=r[5];goto L8c64e0c8; /* mov r5,r3 */
L8c64e0c8: *stop=0x8c64e0c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e160u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64e0ca; /* mov.l 0x8c64e160,r14 */
L8c64e0ca: *stop=0x8c64e0cau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64e0cc; /* tst r3,r3 */
L8c64e0cc: *stop=0x8c64e0ccu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c64e0ce; /* mov.l r4,@(0x8,r15) */
L8c64e0ce: take=(*sr&1u);*stop=0x8c64e0d0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;if(take)goto L8c64e128;goto L8c64e0d2; /* bt/s 0x8c64e128 */
L8c64e0d2: *stop=0x8c64e0d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e14cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64e0d4; /* mov.l 0x8c64e14c,r12 */
L8c64e0d4: *stop=0x8c64e0d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e138u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e0d6; /* mov.l 0x8c64e138,r1 */
L8c64e0d6: *stop=0x8c64e0d6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e0d8; /* mov.l @r15,r2 */
L8c64e0d8: *stop=0x8c64e0d8u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64e0da; /* mov.l @r12,r4 */
L8c64e0da: *stop=0x8c64e0dau;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e0dc; /* mov.l @r1,r3 */
L8c64e0dc: *stop=0x8c64e0dcu;r[2]+=r[4];goto L8c64e0de; /* add r4,r2 */
L8c64e0de: *stop=0x8c64e0deu;*sr=(*sr&~1u)|(r[3]>=r[2]);goto L8c64e0e0; /* cmp/hs r2,r3 */
L8c64e0e0: if(!(*sr&1u))goto L8c64e128;goto L8c64e0e2; /* bf 0x8c64e128 */
L8c64e0e2: *stop=0x8c64e0e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e164u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64e0e4; /* mov.l 0x8c64e164,r10 */
L8c64e0e4: *stop=0x8c64e0e4u;r[1]=r[4];goto L8c64e0e6; /* mov r4,r1 */
L8c64e0e6: *stop=0x8c64e0e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e168u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e0e8; /* mov.l 0x8c64e168,r3 */
L8c64e0e8: *stop=0x8c64e0e8u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64e0ea; /* mov.l @r10,r5 */
L8c64e0ea: target=r[3];*pr=0x8c64e0eeu;*stop=0x8c64e0ecu;r[0]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e0ee; /* jsr @r3 */
L8c64e0ee: *stop=0x8c64e0eeu;r[11]=r[0];goto L8c64e0f0; /* mov r0,r11 */
L8c64e0f0: *stop=0x8c64e0f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e148u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c64e0f2; /* mov.l 0x8c64e148,r8 */
L8c64e0f2: *stop=0x8c64e0f2u;*macl=r[11]*r[5];goto L8c64e0f4; /* mul.l r11,r5 */
L8c64e0f4: *stop=0x8c64e0f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e16cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e0f6; /* mov.l 0x8c64e16c,r2 */
L8c64e0f6: *stop=0x8c64e0f6u;r[9]=r[4];goto L8c64e0f8; /* mov r4,r9 */
L8c64e0f8: *stop=0x8c64e0f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e168u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e0fa; /* mov.l 0x8c64e168,r3 */
L8c64e0fa: *stop=0x8c64e0fau;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e0fc; /* mov.l @r8,r1 */
L8c64e0fc: *stop=0x8c64e0fcu;r[5]=*macl;goto L8c64e0fe; /* sts MACL,r5 */
L8c64e0fe: *stop=0x8c64e0feu;r[9]-=r[5];goto L8c64e100; /* sub r5,r9 */
L8c64e100: *stop=0x8c64e100u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64e102; /* mov.l @r2,r5 */
L8c64e102: target=r[3];*pr=0x8c64e106u;*stop=0x8c64e104u;r[0]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e106; /* jsr @r3 */
L8c64e106: *stop=0x8c64e106u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e134u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64e108; /* mov.l 0x8c64e134,r13 */
L8c64e108: *stop=0x8c64e108u;r[4]=r[0];goto L8c64e10a; /* mov r0,r4 */
L8c64e10a: *stop=0x8c64e10au;*sr=(*sr&~1u)|(r[4]==r[11]);goto L8c64e10c; /* cmp/eq r4,r11 */
L8c64e10c: if(!(*sr&1u))goto L8c64e116;goto L8c64e10e; /* bf 0x8c64e116 */
L8c64e10e: *stop=0x8c64e10eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e154u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e110; /* mov.l 0x8c64e154,r1 */
L8c64e110: *stop=0x8c64e110u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e112; /* mov.l @r1,r3 */
L8c64e112: *stop=0x8c64e112u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64e114; /* tst r3,r3 */
L8c64e114: if(!(*sr&1u))goto L8c64e19e;goto L8c64e116; /* bf 0x8c64e19e */
L8c64e116: *stop=0x8c64e116u;*macl=r[5]*r[11];goto L8c64e118; /* mul.l r5,r11 */
L8c64e118: *stop=0x8c64e118u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e170u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e11a; /* mov.l 0x8c64e170,r3 */
L8c64e11a: *stop=0x8c64e11au;r[6]=0u;goto L8c64e11c; /* mov #0x0,r6 */
L8c64e11c: *stop=0x8c64e11cu;r[11]=*macl;goto L8c64e11e; /* sts MACL,r11 */
L8c64e11e: *stop=0x8c64e11eu;r[5]=r[11];goto L8c64e120; /* mov r11,r5 */
L8c64e120: target=r[3];*pr=0x8c64e124u;*stop=0x8c64e122u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e124; /* jsr @r3 */
L8c64e124: *stop=0x8c64e124u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64e126; /* tst r0,r0 */
L8c64e126: if((*sr&1u))goto L8c64e174;goto L8c64e128; /* bt 0x8c64e174 */
L8c64e128: *stop=0x8c64e128u;r[3]=5u;goto L8c64e12a; /* mov #0x5,r3 */
L8c64e12a: *stop=0x8c64e12cu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c64e29a; /* bra 0x8c64e29a */
L8c64e174: *stop=0x8c64e174u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e394u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e176; /* mov.l 0x8c64e394,r2 */
L8c64e176: *stop=0x8c64e176u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e390u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e178; /* mov.l 0x8c64e390,r3 */
L8c64e178: *stop=0x8c64e178u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e398u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e17a; /* mov.l 0x8c64e398,r1 */
L8c64e17a: *stop=0x8c64e17au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64e17c; /* mov.l @r2,r5 */
L8c64e17c: *stop=0x8c64e17cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64e17e; /* mov.l @r3,r6 */
L8c64e17e: target=r[1];*pr=0x8c64e182u;*stop=0x8c64e180u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e182; /* jsr @r1 */
L8c64e182: *stop=0x8c64e182u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64e184; /* tst r0,r0 */
L8c64e184: if((*sr&1u))goto L8c64e19c;goto L8c64e186; /* bt 0x8c64e19c */
L8c64e186: *stop=0x8c64e186u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e39cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e188; /* mov.l 0x8c64e39c,r2 */
L8c64e188: target=r[2];*pr=0x8c64e18cu;*stop=0x8c64e18au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e18c; /* jsr @r2 */
L8c64e18c: *stop=0x8c64e18cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e18e; /* mov.l 0x8c64e3a0,r3 */
L8c64e18e: *stop=0x8c64e18eu;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],4);if(io_status)return io_status;goto L8c64e190; /* mov.l r0,@(0xc,r14) */
L8c64e190: target=r[3];*pr=0x8c64e194u;*stop=0x8c64e192u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e194; /* jsr @r3 */
L8c64e194: *stop=0x8c64e194u;r[2]=5u;goto L8c64e196; /* mov #0x5,r2 */
L8c64e196: *stop=0x8c64e196u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[0],4);if(io_status)return io_status;goto L8c64e198; /* mov.l r0,@(0x10,r14) */
L8c64e198: *stop=0x8c64e19au;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c64e29a; /* bra 0x8c64e29a */
L8c64e19c: *stop=0x8c64e19cu;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[11],4);if(io_status)return io_status;goto L8c64e19e; /* mov.l r11,@r8 */
L8c64e19e: *stop=0x8c64e19eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3a4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64e1a0; /* mov.l 0x8c64e3a4,r11 */
L8c64e1a0: *stop=0x8c64e1a0u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e1a2; /* mov.l @r11,r3 */
L8c64e1a2: *stop=0x8c64e1a2u;*sr=(*sr&~1u)|(r[3]==r[9]);goto L8c64e1a4; /* cmp/eq r3,r9 */
L8c64e1a4: if((*sr&1u))goto L8c64e1a8;goto L8c64e1a6; /* bt 0x8c64e1a8 */
L8c64e1a6: *stop=0x8c64e1a6u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[9],4);if(io_status)return io_status;goto L8c64e1a8; /* mov.l r9,@r11 */
L8c64e1a8: *stop=0x8c64e1a8u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e1aa; /* mov.l @r10,r2 */
L8c64e1aa: *stop=0x8c64e1aau;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e1ac; /* mov.l @r11,r3 */
L8c64e1ac: *stop=0x8c64e1acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3a8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e1ae; /* mov.l 0x8c64e3a8,r1 */
L8c64e1ae: *stop=0x8c64e1aeu;r[2]-=r[3];goto L8c64e1b0; /* sub r3,r2 */
L8c64e1b0: *stop=0x8c64e1b0u;r[6]=r[2];goto L8c64e1b2; /* mov r2,r6 */
L8c64e1b2: *stop=0x8c64e1b2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c64e1b4; /* mov.l r2,@r1 */
L8c64e1b4: *stop=0x8c64e1b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e1b6; /* mov.l @r15,r3 */
L8c64e1b6: *stop=0x8c64e1b6u;*sr=(*sr&~1u)|(r[6]>=r[3]);goto L8c64e1b8; /* cmp/hs r3,r6 */
L8c64e1b8: if(!(*sr&1u))goto L8c64e1e4;goto L8c64e1ba; /* bf 0x8c64e1e4 */
L8c64e1ba: *stop=0x8c64e1bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e390u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e1bc; /* mov.l 0x8c64e390,r0 */
L8c64e1bc: *stop=0x8c64e1bcu;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e1be; /* mov.l @r11,r3 */
L8c64e1be: *stop=0x8c64e1beu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64e1c0; /* mov.l @r0,r5 */
L8c64e1c0: *stop=0x8c64e1c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}goto L8c64e1c2; /* mov.l @r15,r14 */
L8c64e1c2: *stop=0x8c64e1c2u;r[5]+=r[3];goto L8c64e1c4; /* add r3,r5 */
L8c64e1c4: *stop=0x8c64e1c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e1c6; /* mov.l 0x8c64e3ac,r3 */
L8c64e1c6: *stop=0x8c64e1c6u;r[6]=r[14];goto L8c64e1c8; /* mov r14,r6 */
L8c64e1c8: target=r[3];*pr=0x8c64e1ccu;*stop=0x8c64e1cau;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e1cc; /* jsr @r3 */
L8c64e1cc: *stop=0x8c64e1ccu;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e1ce; /* mov.l @r11,r2 */
L8c64e1ce: *stop=0x8c64e1ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3a8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e1d0; /* mov.l 0x8c64e3a8,r1 */
L8c64e1d0: *stop=0x8c64e1d0u;r[2]+=r[14];goto L8c64e1d2; /* add r14,r2 */
L8c64e1d2: *stop=0x8c64e1d2u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[2],4);if(io_status)return io_status;goto L8c64e1d4; /* mov.l r2,@r11 */
L8c64e1d4: *stop=0x8c64e1d4u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e1d6; /* mov.l @r12,r3 */
L8c64e1d6: *stop=0x8c64e1d6u;r[3]+=r[14];goto L8c64e1d8; /* add r14,r3 */
L8c64e1d8: *stop=0x8c64e1d8u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;goto L8c64e1da; /* mov.l r3,@r12 */
L8c64e1da: *stop=0x8c64e1dau;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e1dc; /* mov.l @r10,r2 */
L8c64e1dc: *stop=0x8c64e1dcu;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e1de; /* mov.l @r11,r3 */
L8c64e1de: *stop=0x8c64e1deu;r[2]-=r[3];goto L8c64e1e0; /* sub r3,r2 */
L8c64e1e0: *stop=0x8c64e1e2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c64e2d6; /* bra 0x8c64e2d6 */
L8c64e1e4: *stop=0x8c64e1e4u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c64e1e6; /* tst r6,r6 */
L8c64e1e6: take=(*sr&1u);*stop=0x8c64e1e8u;r[9]=0u;if(take)goto L8c64e204;goto L8c64e1ea; /* bt/s 0x8c64e204 */
L8c64e1ea: *stop=0x8c64e1eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e390u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e1ec; /* mov.l 0x8c64e390,r0 */
L8c64e1ec: *stop=0x8c64e1ecu;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e1ee; /* mov.l @r11,r3 */
L8c64e1ee: *stop=0x8c64e1eeu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64e1f0; /* mov.l @r0,r5 */
L8c64e1f0: *stop=0x8c64e1f0u;r[5]+=r[3];goto L8c64e1f2; /* add r3,r5 */
L8c64e1f2: *stop=0x8c64e1f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e1f4; /* mov.l 0x8c64e3ac,r3 */
L8c64e1f4: target=r[3];*pr=0x8c64e1f8u;*stop=0x8c64e1f6u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e1f8; /* jsr @r3 */
L8c64e1f8: *stop=0x8c64e1f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e1fa; /* mov.l 0x8c64e3a8,r2 */
L8c64e1fa: *stop=0x8c64e1fau;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e1fc; /* mov.l @r12,r1 */
L8c64e1fc: *stop=0x8c64e1fcu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[9]=loaded;}goto L8c64e1fe; /* mov.l @r2,r9 */
L8c64e1fe: *stop=0x8c64e1feu;r[3]=r[9];goto L8c64e200; /* mov r9,r3 */
L8c64e200: *stop=0x8c64e200u;r[1]+=r[3];goto L8c64e202; /* add r3,r1 */
L8c64e202: *stop=0x8c64e202u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[1],4);if(io_status)return io_status;goto L8c64e204; /* mov.l r1,@r12 */
L8c64e204: *stop=0x8c64e204u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e206; /* mov.l @r15,r2 */
L8c64e206: *stop=0x8c64e206u;*sr=(*sr&~1u)|(r[2]>r[9]);goto L8c64e208; /* cmp/hi r9,r2 */
L8c64e208: if(!(*sr&1u))goto L8c64e2d6;goto L8c64e20a; /* bf 0x8c64e2d6 */
L8c64e20a: *stop=0x8c64e20au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e394u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e20c; /* mov.l 0x8c64e394,r2 */
L8c64e20c: *stop=0x8c64e20cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64e20e; /* mov.l @r15,r5 */
L8c64e20e: *stop=0x8c64e20eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e210; /* mov.l @r2,r3 */
L8c64e210: *stop=0x8c64e210u;r[5]-=r[9];goto L8c64e212; /* sub r9,r5 */
L8c64e212: *stop=0x8c64e212u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c64e214; /* mov.l r3,@(0xc,r15) */
L8c64e214: *stop=0x8c64e214u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e216; /* mov.l @r10,r3 */
L8c64e216: *stop=0x8c64e216u;*sr=(*sr&~1u)|(r[5]>r[3]);goto L8c64e218; /* cmp/hi r3,r5 */
L8c64e218: take=!(*sr&1u);*stop=0x8c64e21au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(take)goto L8c64e276;goto L8c64e21c; /* bf/s 0x8c64e276 */
L8c64e21c: *stop=0x8c64e21cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e21e; /* mov.l 0x8c64e3b0,r3 */
L8c64e21e: *stop=0x8c64e21eu;r[1]=r[5];goto L8c64e220; /* mov r5,r1 */
L8c64e220: target=r[3];*pr=0x8c64e224u;*stop=0x8c64e222u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[0]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e224; /* jsr @r3 */
L8c64e224: *stop=0x8c64e224u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e398u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e226; /* mov.l 0x8c64e398,r2 */
L8c64e226: *stop=0x8c64e226u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c64e228; /* mov.l r0,@(0x4,r15) */
L8c64e228: *stop=0x8c64e228u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e22a; /* mov.l @(0x4,r15),r3 */
L8c64e22a: *stop=0x8c64e22au;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e22c; /* mov.l @(0xc,r15),r1 */
L8c64e22c: *stop=0x8c64e22cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64e22e; /* mov.l @(0x8,r15),r6 */
L8c64e22e: *stop=0x8c64e22eu;*macl=r[3]*r[1];goto L8c64e230; /* mul.l r3,r1 */
L8c64e230: *stop=0x8c64e230u;r[6]+=r[9];goto L8c64e232; /* add r9,r6 */
L8c64e232: target=r[2];*pr=0x8c64e236u;*stop=0x8c64e234u;r[5]=*macl;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e236; /* jsr @r2 */
L8c64e236: *stop=0x8c64e236u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64e238; /* tst r0,r0 */
L8c64e238: if((*sr&1u))goto L8c64e250;goto L8c64e23a; /* bt 0x8c64e250 */
L8c64e23a: *stop=0x8c64e23au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e39cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e23c; /* mov.l 0x8c64e39c,r2 */
L8c64e23c: target=r[2];*pr=0x8c64e240u;*stop=0x8c64e23eu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e240; /* jsr @r2 */
L8c64e240: *stop=0x8c64e240u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e242; /* mov.l 0x8c64e3a0,r3 */
L8c64e242: *stop=0x8c64e242u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],4);if(io_status)return io_status;goto L8c64e244; /* mov.l r0,@(0xc,r14) */
L8c64e244: target=r[3];*pr=0x8c64e248u;*stop=0x8c64e246u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e248; /* jsr @r3 */
L8c64e248: *stop=0x8c64e248u;r[2]=5u;goto L8c64e24a; /* mov #0x5,r2 */
L8c64e24a: *stop=0x8c64e24au;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[0],4);if(io_status)return io_status;goto L8c64e24c; /* mov.l r0,@(0x10,r14) */
L8c64e24c: *stop=0x8c64e24eu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c64e29a; /* bra 0x8c64e29a */
L8c64e250: *stop=0x8c64e250u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e252; /* mov.l @r10,r2 */
L8c64e252: *stop=0x8c64e252u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e254; /* mov.l @(0x4,r15),r3 */
L8c64e254: *stop=0x8c64e254u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e394u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e256; /* mov.l 0x8c64e394,r1 */
L8c64e256: *stop=0x8c64e256u;*macl=r[3]*r[2];goto L8c64e258; /* mul.l r3,r2 */
L8c64e258: *stop=0x8c64e258u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e25a; /* mov.l @r12,r2 */
L8c64e25a: *stop=0x8c64e25au;r[4]=*macl;goto L8c64e25c; /* sts MACL,r4 */
L8c64e25c: *stop=0x8c64e25cu;r[2]+=r[4];goto L8c64e25e; /* add r4,r2 */
L8c64e25e: *stop=0x8c64e25eu;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[2],4);if(io_status)return io_status;goto L8c64e260; /* mov.l r2,@r12 */
L8c64e260: *stop=0x8c64e260u;r[9]+=r[4];goto L8c64e262; /* add r4,r9 */
L8c64e262: *stop=0x8c64e262u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e264; /* mov.l @r1,r2 */
L8c64e264: *stop=0x8c64e264u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e266; /* mov.l @(0x4,r15),r3 */
L8c64e266: *stop=0x8c64e266u;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e268; /* mov.l @r8,r0 */
L8c64e268: *stop=0x8c64e268u;*macl=r[3]*r[2];goto L8c64e26a; /* mul.l r3,r2 */
L8c64e26a: *stop=0x8c64e26au;r[3]=*macl;goto L8c64e26c; /* sts MACL,r3 */
L8c64e26c: *stop=0x8c64e26cu;r[0]+=r[3];goto L8c64e26e; /* add r3,r0 */
L8c64e26e: *stop=0x8c64e26eu;r[3]=0u;goto L8c64e270; /* mov #0x0,r3 */
L8c64e270: *stop=0x8c64e270u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c64e272; /* mov.l r0,@r8 */
L8c64e272: *stop=0x8c64e274u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c64e2d0; /* bra 0x8c64e2d0 */
L8c64e276: *stop=0x8c64e276u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e390u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e278; /* mov.l 0x8c64e390,r3 */
L8c64e278: *stop=0x8c64e278u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e398u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e27a; /* mov.l 0x8c64e398,r1 */
L8c64e27a: *stop=0x8c64e27au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c64e27c; /* mov.l r5,@(0x4,r15) */
L8c64e27c: *stop=0x8c64e27cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64e27e; /* mov.l @r3,r6 */
L8c64e27e: target=r[1];*pr=0x8c64e282u;*stop=0x8c64e280u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e282; /* jsr @r1 */
L8c64e282: *stop=0x8c64e282u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64e284; /* tst r0,r0 */
L8c64e284: if((*sr&1u))goto L8c64e29e;goto L8c64e286; /* bt 0x8c64e29e */
L8c64e286: *stop=0x8c64e286u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e39cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e288; /* mov.l 0x8c64e39c,r2 */
L8c64e288: target=r[2];*pr=0x8c64e28cu;*stop=0x8c64e28au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e28c; /* jsr @r2 */
L8c64e28c: *stop=0x8c64e28cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e28e; /* mov.l 0x8c64e3a0,r3 */
L8c64e28e: *stop=0x8c64e28eu;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],4);if(io_status)return io_status;goto L8c64e290; /* mov.l r0,@(0xc,r14) */
L8c64e290: target=r[3];*pr=0x8c64e294u;*stop=0x8c64e292u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e294; /* jsr @r3 */
L8c64e294: *stop=0x8c64e294u;r[2]=5u;goto L8c64e296; /* mov #0x5,r2 */
L8c64e296: *stop=0x8c64e296u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[0],4);if(io_status)return io_status;goto L8c64e298; /* mov.l r0,@(0x10,r14) */
L8c64e298: *stop=0x8c64e298u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c64e29a; /* mov.l r2,@(0x8,r14) */
L8c64e29a: *stop=0x8c64e29cu;r[0]=4294967295u;goto L8c64e2d8; /* bra 0x8c64e2d8 */
L8c64e29e: *stop=0x8c64e29eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e390u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e2a0; /* mov.l 0x8c64e390,r3 */
L8c64e2a0: *stop=0x8c64e2a0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64e2a2; /* mov.l @(0x8,r15),r4 */
L8c64e2a2: *stop=0x8c64e2a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e2a4; /* mov.l 0x8c64e3ac,r2 */
L8c64e2a4: *stop=0x8c64e2a4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64e2a6; /* mov.l @(0x4,r15),r6 */
L8c64e2a6: *stop=0x8c64e2a6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64e2a8; /* mov.l @r3,r5 */
L8c64e2a8: target=r[2];*pr=0x8c64e2acu;*stop=0x8c64e2aau;r[4]+=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e2ac; /* jsr @r2 */
L8c64e2ac: *stop=0x8c64e2acu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e2ae; /* mov.l @r12,r2 */
L8c64e2ae: *stop=0x8c64e2aeu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e2b0; /* mov.l @(0x4,r15),r3 */
L8c64e2b0: *stop=0x8c64e2b0u;r[2]+=r[3];goto L8c64e2b2; /* add r3,r2 */
L8c64e2b2: *stop=0x8c64e2b2u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[2],4);if(io_status)return io_status;goto L8c64e2b4; /* mov.l r2,@r12 */
L8c64e2b4: *stop=0x8c64e2b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e394u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e2b6; /* mov.l 0x8c64e394,r2 */
L8c64e2b6: *stop=0x8c64e2b6u;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e2b8; /* mov.l @r8,r1 */
L8c64e2b8: *stop=0x8c64e2b8u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e2ba; /* mov.l @r2,r3 */
L8c64e2ba: *stop=0x8c64e2bau;r[1]+=r[3];goto L8c64e2bc; /* add r3,r1 */
L8c64e2bc: *stop=0x8c64e2bcu;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c64e2be; /* mov.l r1,@r8 */
L8c64e2be: *stop=0x8c64e2beu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e2c0; /* mov.l @(0x4,r15),r0 */
L8c64e2c0: *stop=0x8c64e2c0u;r[3]=r[0];goto L8c64e2c2; /* mov r0,r3 */
L8c64e2c2: *stop=0x8c64e2c2u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[0],4);if(io_status)return io_status;goto L8c64e2c4; /* mov.l r0,@r11 */
L8c64e2c4: *stop=0x8c64e2c4u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e2c6; /* mov.l @r10,r1 */
L8c64e2c6: *stop=0x8c64e2c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e3a8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e2c8; /* mov.l 0x8c64e3a8,r0 */
L8c64e2c8: *stop=0x8c64e2c8u;r[1]-=r[3];goto L8c64e2ca; /* sub r3,r1 */
L8c64e2ca: *stop=0x8c64e2cau;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c64e2cc; /* mov.l r1,@r0 */
L8c64e2cc: *stop=0x8c64e2ccu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e2ce; /* mov.l @(0x4,r15),r3 */
L8c64e2ce: *stop=0x8c64e2ceu;r[9]+=r[3];goto L8c64e2d0; /* add r3,r9 */
L8c64e2d0: *stop=0x8c64e2d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e2d2; /* mov.l @r15,r2 */
L8c64e2d2: *stop=0x8c64e2d2u;*sr=(*sr&~1u)|(r[2]>r[9]);goto L8c64e2d4; /* cmp/hi r9,r2 */
L8c64e2d4: if((*sr&1u))goto L8c64e20a;goto L8c64e2d6; /* bt 0x8c64e20a */
L8c64e2d6: *stop=0x8c64e2d6u;r[0]=1u;goto L8c64e2d8; /* mov #0x1,r0 */
L8c64e2d8: *stop=0x8c64e2d8u;r[15]+=16u;goto L8c64e2da; /* add #0x10,r15 */
L8c64e2da: *stop=0x8c64e2dau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64e2dc; /* lds.l @r15+,PR */
L8c64e2dc: *stop=0x8c64e2dcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64e2de; /* mov.l @r15+,r8 */
L8c64e2de: *stop=0x8c64e2deu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64e2e0; /* mov.l @r15+,r9 */
L8c64e2e0: *stop=0x8c64e2e0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64e2e2; /* mov.l @r15+,r10 */
L8c64e2e2: *stop=0x8c64e2e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64e2e4; /* mov.l @r15+,r11 */
L8c64e2e4: *stop=0x8c64e2e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64e2e6; /* mov.l @r15+,r12 */
L8c64e2e6: *stop=0x8c64e2e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64e2e8; /* mov.l @r15+,r13 */
L8c64e2e8: *stop=0x8c64e2e8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64e2eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c64e0b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e0ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e0eeu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e106(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e106u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e124(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e124u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e182(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e182u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e18c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e18cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e194(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e194u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e1cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e1ccu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e1f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e1f8u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e224(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e224u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e236(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e236u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e240(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e240u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e248(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e248u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e282(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e282u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e28c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e28cu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e294(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e294u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c64e2ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c64e0b4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64e2acu);}

SA_STAGE_EXPORT int sa_player_texture_link_8c64e58e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64e58e;
L8c64e58e: *stop=0x8c64e58eu;r[0]=r[5];goto L8c64e590; /* mov r5,r0 */
L8c64e590: *stop=0x8c64e590u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e5d8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64e592; /* mov.l 0x8c64e5d8,r6 */
L8c64e592: *stop=0x8c64e592u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64e594; /* cmp/eq #0x1,r0 */
L8c64e594: if((*sr&1u))goto L8c64e5a2;goto L8c64e596; /* bt 0x8c64e5a2 */
L8c64e596: *stop=0x8c64e596u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64e598; /* cmp/eq #0x0,r0 */
L8c64e598: if((*sr&1u))goto L8c64e5aa;goto L8c64e59a; /* bt 0x8c64e5aa */
L8c64e59a: *stop=0x8c64e59au;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c64e59c; /* cmp/eq #0x2,r0 */
L8c64e59c: if((*sr&1u))goto L8c64e5ae;goto L8c64e59e; /* bt 0x8c64e5ae */
L8c64e59e: *stop=0x8c64e5a0u;goto L8c64e5b6; /* bra 0x8c64e5b6 */
L8c64e5a2: *stop=0x8c64e5a2u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e5a4; /* mov.l @r6,r3 */
L8c64e5a4: *stop=0x8c64e5a4u;r[3]+=r[4];goto L8c64e5a6; /* add r4,r3 */
L8c64e5a6: *stop=0x8c64e5a8u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c64e5b6; /* bra 0x8c64e5b6 */
L8c64e5aa: *stop=0x8c64e5acu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c64e5b6; /* bra 0x8c64e5b6 */
L8c64e5ae: *stop=0x8c64e5aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e5dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e5b0; /* mov.l 0x8c64e5dc,r3 */
L8c64e5b0: *stop=0x8c64e5b0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e5b2; /* mov.l @r3,r2 */
L8c64e5b2: *stop=0x8c64e5b2u;r[2]-=r[4];goto L8c64e5b4; /* sub r4,r2 */
L8c64e5b4: *stop=0x8c64e5b4u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c64e5b6; /* mov.l r2,@r6 */
L8c64e5b6: *stop=0x8c64e5b6u;if(*pr!=entry_pr)return -3;*stop=0x8c64e5b8u;return 0; /* rts */
}

static int sa_player_texture_link_run_8c653a36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c653a9au)goto L8c653a9a;goto L8c653a36;
L8c653a36: *stop=0x8c653a36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c653a38; /* mov.l r14,@-r15 */
L8c653a38: *stop=0x8c653a38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c653a3a; /* mov.l r13,@-r15 */
L8c653a3a: *stop=0x8c653a3au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c653a3c; /* sts.l PR,@-r15 */
L8c653a3c: *stop=0x8c653a3cu;r[15]+=4294967292u;goto L8c653a3e; /* add #-0x4,r15 */
L8c653a3e: *stop=0x8c653a3eu;r[3]=4294967292u;goto L8c653a40; /* mov #-0x4,r3 */
L8c653a40: *stop=0x8c653a40u;r[7]&=r[3];goto L8c653a42; /* and r3,r7 */
L8c653a42: *stop=0x8c653a42u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c653a44; /* tst r7,r7 */
L8c653a44: *stop=0x8c653a44u;r[6]&=r[3];goto L8c653a46; /* and r3,r6 */
L8c653a46: take=(*sr&1u);*stop=0x8c653a48u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;if(take)goto L8c653a9a;goto L8c653a4a; /* bt/s 0x8c653a9a */
L8c653a4a: *stop=0x8c653a4au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c653a4c; /* mov.l @r4,r0 */
L8c653a4c: *stop=0x8c653a4cu;r[0]=r[0]&65535u;goto L8c653a4e; /* extu.w r0,r0 */
L8c653a4e: *stop=0x8c653a4eu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c653a50; /* cmp/eq #0x3,r0 */
L8c653a50: if(!(*sr&1u))goto L8c653a56;goto L8c653a52; /* bf 0x8c653a56 */
L8c653a52: *stop=0x8c653a54u;r[0]=4u;goto L8c653a9c; /* bra 0x8c653a9c */
L8c653a56: *stop=0x8c653a56u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c653a58; /* mov.l @(0x18,r4),r6 */
L8c653a58: *stop=0x8c653a58u;r[3]=4u;goto L8c653a5a; /* mov #0x4,r3 */
L8c653a5a: *stop=0x8c653a5au;*sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c653a5c; /* tst r6,r3 */
L8c653a5c: if((*sr&1u))goto L8c653a74;goto L8c653a5e; /* bt 0x8c653a74 */
L8c653a5e: *stop=0x8c653a5eu;r[1]=1u;goto L8c653a60; /* mov #0x1,r1 */
L8c653a60: *stop=0x8c653a60u;*sr=(*sr&~1u)|!(r[6]&r[1]);goto L8c653a62; /* tst r6,r1 */
L8c653a62: if((*sr&1u))goto L8c653a74;goto L8c653a64; /* bt 0x8c653a74 */
L8c653a64: *stop=0x8c653a64u;r[2]=8u;goto L8c653a66; /* mov #0x8,r2 */
L8c653a66: *stop=0x8c653a66u;*sr=(*sr&~1u)|!(r[6]&r[2]);goto L8c653a68; /* tst r6,r2 */
L8c653a68: if(!(*sr&1u))goto L8c653a74;goto L8c653a6a; /* bf 0x8c653a74 */
L8c653a6a: *stop=0x8c653a6au;r[1]=32u;goto L8c653a6c; /* mov #0x20,r1 */
L8c653a6c: *stop=0x8c653a6cu;*sr=(*sr&~1u)|!(r[6]&r[1]);goto L8c653a6e; /* tst r6,r1 */
L8c653a6e: if(!(*sr&1u))goto L8c653a74;goto L8c653a70; /* bf 0x8c653a74 */
L8c653a70: *stop=0x8c653a72u;r[6]=4u;goto L8c653a76; /* bra 0x8c653a76 */
L8c653a74: *stop=0x8c653a74u;r[6]=0u;goto L8c653a76; /* mov #0x0,r6 */
L8c653a76: *stop=0x8c653a76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}goto L8c653a78; /* mov.l @r15,r14 */
L8c653a78: *stop=0x8c653a78u;r[14]+=r[6];goto L8c653a7a; /* add r6,r14 */
L8c653a7a: *stop=0x8c653a7au;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c653a7c; /* mov.l @(0x14,r4),r6 */
L8c653a7c: *stop=0x8c653a7cu;*sr=(*sr&~1u)|(r[14]>=r[6]);goto L8c653a7e; /* cmp/hs r6,r14 */
L8c653a7e: *stop=0x8c653a7eu;r[13]=r[6];goto L8c653a80; /* mov r6,r13 */
L8c653a80: take=!(*sr&1u);*stop=0x8c653a82u;r[13]-=r[14];if(take)goto L8c653a88;goto L8c653a84; /* bf/s 0x8c653a88 */
L8c653a84: *stop=0x8c653a86u;r[0]=7u;goto L8c653a9c; /* bra 0x8c653a9c */
L8c653a88: *stop=0x8c653a88u;*sr=(*sr&~1u)|(r[13]>r[7]);goto L8c653a8a; /* cmp/hi r7,r13 */
L8c653a8a: take=!(*sr&1u);*stop=0x8c653a8cu;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}if(take)goto L8c653a90;goto L8c653a8e; /* bf/s 0x8c653a90 */
L8c653a8e: *stop=0x8c653a8eu;r[13]=r[7];goto L8c653a90; /* mov r7,r13 */
L8c653a90: *stop=0x8c653a90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653ae0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c653a92; /* mov.l 0x8c653ae0,r2 */
L8c653a92: *stop=0x8c653a92u;r[6]=r[13];goto L8c653a94; /* mov r13,r6 */
L8c653a94: *stop=0x8c653a94u;r[14]+=r[3];goto L8c653a96; /* add r3,r14 */
L8c653a96: target=r[2];*pr=0x8c653a9au;*stop=0x8c653a98u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653a9a; /* jsr @r2 */
L8c653a9a: *stop=0x8c653a9au;r[0]=0u;goto L8c653a9c; /* mov #0x0,r0 */
L8c653a9c: *stop=0x8c653a9cu;r[15]+=4u;goto L8c653a9e; /* add #0x4,r15 */
L8c653a9e: *stop=0x8c653a9eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c653aa0; /* lds.l @r15+,PR */
L8c653aa0: *stop=0x8c653aa0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c653aa2; /* mov.l @r15+,r13 */
L8c653aa2: *stop=0x8c653aa2u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c653aa4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c653a36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c653a36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c653a9a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c653a36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c653a9au);}

static int sa_player_texture_link_run_8c661048(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c661062u)goto L8c661062;if(resume_pc==0x8c66109au)goto L8c66109a;goto L8c661048;
L8c661048: *stop=0x8c661048u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c66104a; /* mov.l r14,@-r15 */
L8c66104a: *stop=0x8c66104au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66104c; /* sts.l PR,@-r15 */
L8c66104c: *stop=0x8c66104cu;r[15]+=4294967276u;goto L8c66104e; /* add #-0x14,r15 */
L8c66104e: *stop=0x8c66104eu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c661050; /* mov.l r5,@(0xc,r15) */
L8c661050: *stop=0x8c661050u;r[7]=r[6];goto L8c661052; /* mov r6,r7 */
L8c661052: *stop=0x8c661052u;r[5]=r[15];goto L8c661054; /* mov r15,r5 */
L8c661054: *stop=0x8c661054u;r[14]=r[4];goto L8c661056; /* mov r4,r14 */
L8c661056: *stop=0x8c661056u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c661058; /* mov.l r6,@(0x10,r15) */
L8c661058: *stop=0x8c661058u;r[4]=r[15];goto L8c66105a; /* mov r15,r4 */
L8c66105a: *stop=0x8c66105au;r[5]+=8u;goto L8c66105c; /* add #0x8,r5 */
L8c66105c: *stop=0x8c66105cu;r[6]=r[15];goto L8c66105e; /* mov r15,r6 */
L8c66105e: target=0x8c660f44u;*pr=0x8c661062u;*stop=0x8c661060u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c661062; /* bsr 0x8c660f44 */
L8c661062: *stop=0x8c661062u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c661064; /* cmp/pz r0 */
L8c661064: take=(*sr&1u);*stop=0x8c661066u;r[4]=r[0];if(take)goto L8c661072;goto L8c661068; /* bt/s 0x8c661072 */
L8c661068: *stop=0x8c661068u;r[0]=r[4];goto L8c66106a; /* mov r4,r0 */
L8c66106a: *stop=0x8c66106au;r[15]+=20u;goto L8c66106c; /* add #0x14,r15 */
L8c66106c: *stop=0x8c66106cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66106e; /* lds.l @r15+,PR */
L8c66106e: *stop=0x8c66106eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c661070u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c661072: *stop=0x8c661072u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c661074; /* mov.l @r15,r2 */
L8c661074: *stop=0x8c661074u;r[3]=sx(m,bus,&io_status,0x8c661136u,2);if(io_status)return io_status;goto L8c661076; /* mov.w 0x8c661136,r3 */
L8c661076: *stop=0x8c661076u;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c661078; /* cmp/eq r3,r2 */
L8c661078: if(!(*sr&1u))goto L8c66108c;goto L8c66107a; /* bf 0x8c66108c */
L8c66107a: *stop=0x8c66107au;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66107c; /* mov.l @(0xc,r15),r1 */
L8c66107c: *stop=0x8c66107cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66113cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66107e; /* mov.l 0x8c66113c,r2 */
L8c66107e: *stop=0x8c66107eu;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c661080; /* tst r2,r1 */
L8c661080: if((*sr&1u))goto L8c66108c;goto L8c661082; /* bt 0x8c66108c */
L8c661082: *stop=0x8c661082u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c661084; /* mov.l @r15,r1 */
L8c661084: *stop=0x8c661084u;r[0]=sx(m,bus,&io_status,0x8c661138u,2);if(io_status)return io_status;goto L8c661086; /* mov.w 0x8c661138,r0 */
L8c661086: *stop=0x8c661086u;r[1]=r[1]&255u;goto L8c661088; /* extu.b r1,r1 */
L8c661088: *stop=0x8c661088u;r[1]|=r[0];goto L8c66108a; /* or r0,r1 */
L8c66108a: *stop=0x8c66108au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c66108c; /* mov.l r1,@r15 */
L8c66108c: *stop=0x8c66108cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c66108e; /* mov.l @(0x4,r15),r5 */
L8c66108e: *stop=0x8c66108eu;r[4]=r[14];goto L8c661090; /* mov r14,r4 */
L8c661090: *stop=0x8c661090u;{uint32_t loaded=rd(m,bus,&io_status,0x8c661140u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c661092; /* mov.l 0x8c661140,r3 */
L8c661092: *stop=0x8c661092u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[7]=loaded;}goto L8c661094; /* mov.l @r15,r7 */
L8c661094: *stop=0x8c661094u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c661096; /* mov.l @(0x8,r15),r6 */
L8c661096: target=r[3];*pr=0x8c66109au;*stop=0x8c661098u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66109a; /* jsr @r3 */
L8c66109a: *stop=0x8c66109au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c66109c; /* tst r0,r0 */
L8c66109c: if((*sr&1u))goto L8c6610ae;goto L8c66109e; /* bt 0x8c6610ae */
L8c66109e: *stop=0x8c66109eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c661144u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6610a0; /* mov.l 0x8c661144,r2 */
L8c6610a0: *stop=0x8c6610a0u;r[3]=6u;goto L8c6610a2; /* mov #0x6,r3 */
L8c6610a2: *stop=0x8c6610a2u;r[0]=4294967290u;goto L8c6610a4; /* mov #-0x6,r0 */
L8c6610a4: *stop=0x8c6610a4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6610a6; /* mov.l r3,@r2 */
L8c6610a6: *stop=0x8c6610a6u;r[15]+=20u;goto L8c6610a8; /* add #0x14,r15 */
L8c6610a8: *stop=0x8c6610a8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6610aa; /* lds.l @r15+,PR */
L8c6610aa: *stop=0x8c6610aau;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6610acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c6610ae: *stop=0x8c6610aeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6610b0; /* mov.l @r15,r3 */
L8c6610b0: *stop=0x8c6610b0u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6610b2; /* mov.l @(0x4,r14),r1 */
L8c6610b2: *stop=0x8c6610b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c661148u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6610b4; /* mov.l 0x8c661148,r2 */
L8c6610b4: *stop=0x8c6610b4u;r[3]<<=16;goto L8c6610b6; /* shll16 r3 */
L8c6610b6: *stop=0x8c6610b6u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6610b8; /* mov.l @(0x4,r14),r0 */
L8c6610b8: *stop=0x8c6610b8u;r[1]=r[1]&65535u;goto L8c6610ba; /* extu.w r1,r1 */
L8c6610ba: *stop=0x8c6610bau;r[3]&=r[2];goto L8c6610bc; /* and r2,r3 */
L8c6610bc: *stop=0x8c6610bcu;r[3]|=r[1];goto L8c6610be; /* or r1,r3 */
L8c6610be: *stop=0x8c6610beu;r[0]|=r[3];goto L8c6610c0; /* or r3,r0 */
L8c6610c0: *stop=0x8c6610c0u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],4);if(io_status)return io_status;goto L8c6610c2; /* mov.l r0,@(0x4,r14) */
L8c6610c2: *stop=0x8c6610c2u;r[0]=1u;goto L8c6610c4; /* mov #0x1,r0 */
L8c6610c4: *stop=0x8c6610c4u;r[15]+=20u;goto L8c6610c6; /* add #0x14,r15 */
L8c6610c6: *stop=0x8c6610c6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6610c8; /* lds.l @r15+,PR */
L8c6610c8: *stop=0x8c6610c8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6610cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c661048(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c661048(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c661062(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c661048(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c661062u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c66109a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c661048(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c66109au);}

static int sa_player_texture_link_run_8c6610cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c6610e4u)goto L8c6610e4;if(resume_pc==0x8c66111au)goto L8c66111a;goto L8c6610cc;
L8c6610cc: *stop=0x8c6610ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6610ce; /* sts.l PR,@-r15 */
L8c6610ce: *stop=0x8c6610ceu;r[15]+=4294967272u;goto L8c6610d0; /* add #-0x18,r15 */
L8c6610d0: *stop=0x8c6610d0u;r[7]=r[6];goto L8c6610d2; /* mov r6,r7 */
L8c6610d2: *stop=0x8c6610d2u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;goto L8c6610d4; /* mov.l r4,@(0xc,r15) */
L8c6610d4: *stop=0x8c6610d4u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[5],4);if(io_status)return io_status;goto L8c6610d6; /* mov.l r5,@(0x10,r15) */
L8c6610d6: *stop=0x8c6610d6u;r[5]=r[15];goto L8c6610d8; /* mov r15,r5 */
L8c6610d8: *stop=0x8c6610d8u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[6],4);if(io_status)return io_status;goto L8c6610da; /* mov.l r6,@(0x14,r15) */
L8c6610da: *stop=0x8c6610dau;r[4]=r[15];goto L8c6610dc; /* mov r15,r4 */
L8c6610dc: *stop=0x8c6610dcu;r[5]+=8u;goto L8c6610de; /* add #0x8,r5 */
L8c6610de: *stop=0x8c6610deu;r[6]=r[15];goto L8c6610e0; /* mov r15,r6 */
L8c6610e0: target=0x8c660f44u;*pr=0x8c6610e4u;*stop=0x8c6610e2u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6610e4; /* bsr 0x8c660f44 */
L8c6610e4: *stop=0x8c6610e4u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c6610e6; /* cmp/pz r0 */
L8c6610e6: take=(*sr&1u);*stop=0x8c6610e8u;r[4]=r[0];if(take)goto L8c6610f2;goto L8c6610ea; /* bt/s 0x8c6610f2 */
L8c6610ea: *stop=0x8c6610eau;r[15]+=24u;goto L8c6610ec; /* add #0x18,r15 */
L8c6610ec: *stop=0x8c6610ecu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6610ee; /* lds.l @r15+,PR */
L8c6610ee: *stop=0x8c6610eeu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6610f0u;r[0]=r[4];if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c6610f2: *stop=0x8c6610f2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6610f4; /* mov.l @r15,r2 */
L8c6610f4: *stop=0x8c6610f4u;r[3]=sx(m,bus,&io_status,0x8c661136u,2);if(io_status)return io_status;goto L8c6610f6; /* mov.w 0x8c661136,r3 */
L8c6610f6: *stop=0x8c6610f6u;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c6610f8; /* cmp/eq r3,r2 */
L8c6610f8: if(!(*sr&1u))goto L8c66110c;goto L8c6610fa; /* bf 0x8c66110c */
L8c6610fa: *stop=0x8c6610fau;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6610fc; /* mov.l @(0x10,r15),r1 */
L8c6610fc: *stop=0x8c6610fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66113cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6610fe; /* mov.l 0x8c66113c,r2 */
L8c6610fe: *stop=0x8c6610feu;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c661100; /* tst r2,r1 */
L8c661100: if((*sr&1u))goto L8c66110c;goto L8c661102; /* bt 0x8c66110c */
L8c661102: *stop=0x8c661102u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c661104; /* mov.l @r15,r1 */
L8c661104: *stop=0x8c661104u;r[0]=sx(m,bus,&io_status,0x8c661138u,2);if(io_status)return io_status;goto L8c661106; /* mov.w 0x8c661138,r0 */
L8c661106: *stop=0x8c661106u;r[1]=r[1]&255u;goto L8c661108; /* extu.b r1,r1 */
L8c661108: *stop=0x8c661108u;r[1]|=r[0];goto L8c66110a; /* or r0,r1 */
L8c66110a: *stop=0x8c66110au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c66110c; /* mov.l r1,@r15 */
L8c66110c: *stop=0x8c66110cu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c66110e; /* mov.l @(0xc,r15),r4 */
L8c66110e: *stop=0x8c66110eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66114cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c661110; /* mov.l 0x8c66114c,r3 */
L8c661110: *stop=0x8c661110u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c661112; /* mov.l @(0x4,r15),r5 */
L8c661112: *stop=0x8c661112u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[7]=loaded;}goto L8c661114; /* mov.l @r15,r7 */
L8c661114: *stop=0x8c661114u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c661116; /* mov.l @(0x8,r15),r6 */
L8c661116: target=r[3];*pr=0x8c66111au;*stop=0x8c661118u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66111a; /* jsr @r3 */
L8c66111a: *stop=0x8c66111au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c66111c; /* cmp/eq #0x1,r0 */
L8c66111c: if((*sr&1u))goto L8c66112c;goto L8c66111e; /* bt 0x8c66112c */
L8c66111e: *stop=0x8c66111eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c661144u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c661120; /* mov.l 0x8c661144,r2 */
L8c661120: *stop=0x8c661120u;r[3]=6u;goto L8c661122; /* mov #0x6,r3 */
L8c661122: *stop=0x8c661122u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c661124; /* mov.l r3,@r2 */
L8c661124: *stop=0x8c661124u;r[15]+=24u;goto L8c661126; /* add #0x18,r15 */
L8c661126: *stop=0x8c661126u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c661128; /* lds.l @r15+,PR */
L8c661128: *stop=0x8c661128u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c66112au;r[0]=4294967290u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c66112c: *stop=0x8c66112cu;r[0]=1u;goto L8c66112e; /* mov #0x1,r0 */
L8c66112e: *stop=0x8c66112eu;r[15]+=24u;goto L8c661130; /* add #0x18,r15 */
L8c661130: *stop=0x8c661130u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c661132; /* lds.l @r15+,PR */
L8c661132: *stop=0x8c661132u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c661134u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c6610cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c6610cc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c6610e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c6610cc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6610e4u);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c66111a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c6610cc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c66111au);}

SA_STAGE_EXPORT int sa_player_texture_link_8c10ef20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ef20;
L8c10ef20: *stop=0x8c10ef20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c10ef22; /* mov.l r5,@-r15 */
L8c10ef22: *stop=0x8c10ef22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c10ef24; /* mov.l r6,@-r15 */
L8c10ef24: *stop=0x8c10ef24u;r[3]=r[0];goto L8c10ef26; /* mov r0,r3 */
L8c10ef26: *stop=0x8c10ef26u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}r[1]+=4;goto L8c10ef28; /* mov.l @r1+,r4 */
L8c10ef28: *stop=0x8c10ef28u;r[6]=0u;goto L8c10ef2a; /* mov #0x0,r6 */
L8c10ef2a: *stop=0x8c10ef2au;{uint32_t x=r[4]^r[6];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));}goto L8c10ef2c; /* cmp/str r4,r6 */
L8c10ef2c: if((*sr&1u))goto L8c10ef58;goto L8c10ef2e; /* bt 0x8c10ef58 */
L8c10ef2e: *stop=0x8c10ef2eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[5]=loaded;}r[1]+=4;goto L8c10ef30; /* mov.l @r1+,r5 */
L8c10ef30: *stop=0x8c10ef30u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;goto L8c10ef32; /* mov.l r4,@r0 */
L8c10ef32: *stop=0x8c10ef32u;{uint32_t x=r[5]^r[6];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));}goto L8c10ef34; /* cmp/str r5,r6 */
L8c10ef34: if((*sr&1u))goto L8c10ef4e;goto L8c10ef36; /* bt 0x8c10ef4e */
L8c10ef36: *stop=0x8c10ef36u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}r[1]+=4;goto L8c10ef38; /* mov.l @r1+,r4 */
L8c10ef38: *stop=0x8c10ef38u;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[5],4);if(io_status)return io_status;goto L8c10ef3a; /* mov.l r5,@(0x4,r0) */
L8c10ef3a: *stop=0x8c10ef3au;{uint32_t x=r[4]^r[6];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));}goto L8c10ef3c; /* cmp/str r4,r6 */
L8c10ef3c: if((*sr&1u))goto L8c10ef52;goto L8c10ef3e; /* bt 0x8c10ef52 */
L8c10ef3e: *stop=0x8c10ef3eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[5]=loaded;}r[1]+=4;goto L8c10ef40; /* mov.l @r1+,r5 */
L8c10ef40: *stop=0x8c10ef40u;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[4],4);if(io_status)return io_status;goto L8c10ef42; /* mov.l r4,@(0x8,r0) */
L8c10ef42: *stop=0x8c10ef42u;{uint32_t x=r[5]^r[6];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));}goto L8c10ef44; /* cmp/str r5,r6 */
L8c10ef44: if((*sr&1u))goto L8c10ef56;goto L8c10ef46; /* bt 0x8c10ef56 */
L8c10ef46: *stop=0x8c10ef46u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}r[1]+=4;goto L8c10ef48; /* mov.l @r1+,r4 */
L8c10ef48: *stop=0x8c10ef48u;if(!valid_access(r[0]+12u,4u))return -3;wr(m,bus,&io_status,r[0]+0xcu,r[5],4);if(io_status)return io_status;goto L8c10ef4a; /* mov.l r5,@(0xc,r0) */
L8c10ef4a: *stop=0x8c10ef4cu;r[0]+=16u;goto L8c10ef2a; /* bra 0x8c10ef2a */
L8c10ef4e: *stop=0x8c10ef50u;r[0]+=4u;goto L8c10ef58; /* bra 0x8c10ef58 */
L8c10ef52: *stop=0x8c10ef54u;r[0]+=8u;goto L8c10ef58; /* bra 0x8c10ef58 */
L8c10ef56: *stop=0x8c10ef56u;r[0]+=12u;goto L8c10ef58; /* add #0xc,r0 */
L8c10ef58: *stop=0x8c10ef58u;r[1]+=4294967292u;goto L8c10ef5a; /* add #-0x4,r1 */
L8c10ef5a: *stop=0x8c10ef5au;r[6]=r[0];goto L8c10ef5c; /* mov r0,r6 */
L8c10ef5c: *stop=0x8c10ef5cu;if(!valid_access(r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[1]+=1;goto L8c10ef5e; /* mov.b @r1+,r0 */
L8c10ef5e: *stop=0x8c10ef5eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10ef60; /* cmp/eq #0x0,r0 */
L8c10ef60: *stop=0x8c10ef60u;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[0],1);if(io_status)return io_status;goto L8c10ef62; /* mov.b r0,@r6 */
L8c10ef62: if((*sr&1u))goto L8c10ef78;goto L8c10ef64; /* bt 0x8c10ef78 */
L8c10ef64: *stop=0x8c10ef64u;if(!valid_access(r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[1]+=1;goto L8c10ef66; /* mov.b @r1+,r0 */
L8c10ef66: *stop=0x8c10ef66u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10ef68; /* cmp/eq #0x0,r0 */
L8c10ef68: *stop=0x8c10ef68u;if(!valid_access(r[6]+1u,1u))return -3;wr(m,bus,&io_status,r[6]+0x1u,r[0],1);if(io_status)return io_status;goto L8c10ef6a; /* mov.b r0,@(0x1,r6) */
L8c10ef6a: if((*sr&1u))goto L8c10ef78;goto L8c10ef6c; /* bt 0x8c10ef78 */
L8c10ef6c: *stop=0x8c10ef6cu;if(!valid_access(r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[1]+=1;goto L8c10ef6e; /* mov.b @r1+,r0 */
L8c10ef6e: *stop=0x8c10ef6eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10ef70; /* cmp/eq #0x0,r0 */
L8c10ef70: *stop=0x8c10ef70u;if(!valid_access(r[6]+2u,1u))return -3;wr(m,bus,&io_status,r[6]+0x2u,r[0],1);if(io_status)return io_status;goto L8c10ef72; /* mov.b r0,@(0x2,r6) */
L8c10ef72: if((*sr&1u))goto L8c10ef78;goto L8c10ef74; /* bt 0x8c10ef78 */
L8c10ef74: *stop=0x8c10ef74u;if(!valid_access(r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[1]+=1;goto L8c10ef76; /* mov.b @r1+,r0 */
L8c10ef76: *stop=0x8c10ef76u;if(!valid_access(r[6]+3u,1u))return -3;wr(m,bus,&io_status,r[6]+0x3u,r[0],1);if(io_status)return io_status;goto L8c10ef78; /* mov.b r0,@(0x3,r6) */
L8c10ef78: *stop=0x8c10ef78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}r[15]+=4;goto L8c10ef7a; /* mov.l @r15+,r6 */
L8c10ef7a: *stop=0x8c10ef7au;r[0]=r[3];goto L8c10ef7c; /* mov r3,r0 */
L8c10ef7c: *stop=0x8c10ef7cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}r[15]+=4;goto L8c10ef7e; /* mov.l @r15+,r5 */
L8c10ef7e: *stop=0x8c10ef7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c10ef80; /* mov.l @r15+,r4 */
L8c10ef80: *stop=0x8c10ef80u;if(*pr!=entry_pr)return -3;*stop=0x8c10ef82u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_link_8c10f98c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10f98c;
L8c10f98c: *stop=0x8c10f98cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f9a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f98e; /* mov.l 0x8c10f9a4,r1 */
L8c10f98e: *stop=0x8c10f98eu;r[0]=r[4]&255u;goto L8c10f990; /* extu.b r4,r0 */
L8c10f990: *stop=0x8c10f990u;if(!valid_access(r[0]+r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;goto L8c10f992; /* mov.b @(r0,r1),r0 */
L8c10f992: *stop=0x8c10f992u;r[0]=r[0]&255u;goto L8c10f994; /* extu.b r0,r0 */
L8c10f994: *stop=0x8c10f994u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c10f996; /* tst #0x2,r0 */
L8c10f996: if((*sr&1u))goto L8c10f99e;goto L8c10f998; /* bt 0x8c10f99e */
L8c10f998: *stop=0x8c10f998u;r[0]=r[4];goto L8c10f99a; /* mov r4,r0 */
L8c10f99a: *stop=0x8c10f99au;if(*pr!=entry_pr)return -3;*stop=0x8c10f99cu;r[0]+=4294967264u;return 0; /* rts */
L8c10f99e: *stop=0x8c10f99eu;r[0]=r[4];goto L8c10f9a0; /* mov r4,r0 */
L8c10f9a0: *stop=0x8c10f9a0u;if(*pr!=entry_pr)return -3;*stop=0x8c10f9a2u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_link_8c600e7a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c600e7a;
L8c600e7a: *stop=0x8c600e7au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c600e7c; /* tst r4,r4 */
L8c600e7c: if((*sr&1u))goto L8c600e86;goto L8c600e7e; /* bt 0x8c600e86 */
L8c600e7e: *stop=0x8c600e7eu;r[0]=76u;goto L8c600e80; /* mov #0x4c,r0 */
L8c600e80: *stop=0x8c600e80u;if(!valid_access(r[0]+r[4],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[4],2);if(io_status)return io_status;goto L8c600e82; /* mov.w @(r0,r4),r3 */
L8c600e82: *stop=0x8c600e82u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c600e84; /* tst r3,r3 */
L8c600e84: if(!(*sr&1u))goto L8c600e8a;goto L8c600e86; /* bf 0x8c600e8a */
L8c600e86: *stop=0x8c600e86u;if(*pr!=entry_pr)return -3;*stop=0x8c600e88u;r[0]=4294967286u;return 0; /* rts */
L8c600e8a: *stop=0x8c600e8au;r[0]=r[6];goto L8c600e8c; /* mov r6,r0 */
L8c600e8c: *stop=0x8c600e8cu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c600e8e; /* cmp/eq #0x0,r0 */
L8c600e8e: take=(*sr&1u);*stop=0x8c600e90u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[7]=loaded;}if(take)goto L8c600e9e;goto L8c600e92; /* bt/s 0x8c600e9e */
L8c600e92: *stop=0x8c600e92u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c600e94; /* cmp/eq #0x1,r0 */
L8c600e94: if((*sr&1u))goto L8c600ea2;goto L8c600e96; /* bt 0x8c600ea2 */
L8c600e96: *stop=0x8c600e96u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c600e98; /* cmp/eq #0x2,r0 */
L8c600e98: if((*sr&1u))goto L8c600ea8;goto L8c600e9a; /* bt 0x8c600ea8 */
L8c600e9a: *stop=0x8c600e9cu;goto L8c600eae; /* bra 0x8c600eae */
L8c600e9e: *stop=0x8c600ea0u;r[6]=r[5];goto L8c600eb2; /* bra 0x8c600eb2 */
L8c600ea2: *stop=0x8c600ea2u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c600ea4; /* mov.l @(0x14,r4),r6 */
L8c600ea4: *stop=0x8c600ea6u;r[6]+=r[5];goto L8c600eb2; /* bra 0x8c600eb2 */
L8c600ea8: *stop=0x8c600ea8u;r[6]=r[5];goto L8c600eaa; /* mov r5,r6 */
L8c600eaa: *stop=0x8c600eacu;r[6]+=r[7];goto L8c600eb2; /* bra 0x8c600eb2 */
L8c600eae: *stop=0x8c600eaeu;if(*pr!=entry_pr)return -3;*stop=0x8c600eb0u;r[0]=4294967280u;return 0; /* rts */
L8c600eb2: *stop=0x8c600eb2u;*sr=(*sr&~1u)|((int32_t)r[6]>=0);goto L8c600eb4; /* cmp/pz r6 */
L8c600eb4: if(!(*sr&1u))goto L8c600eba;goto L8c600eb6; /* bf 0x8c600eba */
L8c600eb6: *stop=0x8c600eb6u;*sr=(*sr&~1u)|((int32_t)r[6]>(int32_t)r[7]);goto L8c600eb8; /* cmp/gt r7,r6 */
L8c600eb8: if(!(*sr&1u))goto L8c600ebe;goto L8c600eba; /* bf 0x8c600ebe */
L8c600eba: *stop=0x8c600ebau;if(*pr!=entry_pr)return -3;*stop=0x8c600ebcu;r[0]=4294967279u;return 0; /* rts */
L8c600ebe: *stop=0x8c600ebeu;r[0]=0u;goto L8c600ec0; /* mov #0x0,r0 */
L8c600ec0: *stop=0x8c600ec0u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[6],4);if(io_status)return io_status;goto L8c600ec2; /* mov.l r6,@(0x14,r4) */
L8c600ec2: *stop=0x8c600ec2u;if(*pr!=entry_pr)return -3;*stop=0x8c600ec4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_texture_link_8c601500(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c601500;
L8c601500: *stop=0x8c601500u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c601502; /* tst r4,r4 */
L8c601502: if((*sr&1u))goto L8c60150c;goto L8c601504; /* bt 0x8c60150c */
L8c601504: *stop=0x8c601504u;r[0]=76u;goto L8c601506; /* mov #0x4c,r0 */
L8c601506: *stop=0x8c601506u;if(!valid_access(r[0]+r[4],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[4],2);if(io_status)return io_status;goto L8c601508; /* mov.w @(r0,r4),r3 */
L8c601508: *stop=0x8c601508u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c60150a; /* tst r3,r3 */
L8c60150a: if(!(*sr&1u))goto L8c601510;goto L8c60150c; /* bf 0x8c601510 */
L8c60150c: *stop=0x8c60150cu;if(*pr!=entry_pr)return -3;*stop=0x8c60150eu;r[0]=0u;return 0; /* rts */
L8c601510: *stop=0x8c601510u;r[0]=82u;goto L8c601512; /* mov #0x52,r0 */
L8c601512: *stop=0x8c601512u;if(!valid_access(r[0]+r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[4],2);if(io_status)return io_status;goto L8c601514; /* mov.w @(r0,r4),r0 */
L8c601514: *stop=0x8c601514u;if(*pr!=entry_pr)return -3;*stop=0x8c601516u;return 0; /* rts */
}

static int sa_player_texture_link_run_8c660f44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c660f5au)goto L8c660f5a;if(resume_pc==0x8c660f6eu)goto L8c660f6e;if(resume_pc==0x8c660f7eu)goto L8c660f7e;if(resume_pc==0x8c660f94u)goto L8c660f94;goto L8c660f44;
L8c660f44: *stop=0x8c660f44u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c660f46; /* mov.l r14,@-r15 */
L8c660f46: *stop=0x8c660f46u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c660f48; /* sts.l PR,@-r15 */
L8c660f48: *stop=0x8c660f48u;r[15]+=4294967260u;goto L8c660f4a; /* add #-0x24,r15 */
L8c660f4a: *stop=0x8c660f4au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c660f4c; /* mov.l r4,@(0x8,r15) */
L8c660f4c: *stop=0x8c660f4cu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c660f4e; /* mov.l r5,@(0xc,r15) */
L8c660f4e: *stop=0x8c660f4eu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c660f50; /* mov.l r6,@(0x10,r15) */
L8c660f50: *stop=0x8c660f50u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[7],4);if(io_status)return io_status;goto L8c660f52; /* mov.l r7,@(0x14,r15) */
L8c660f52: *stop=0x8c660f52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c661038u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c660f54; /* mov.l 0x8c661038,r14 */
L8c660f54: *stop=0x8c660f54u;r[5]=4u;goto L8c660f56; /* mov #0x4,r5 */
L8c660f56: target=r[14];*pr=0x8c660f5au;*stop=0x8c660f58u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660f5a; /* jsr @r14 */
L8c660f5a: *stop=0x8c660f5au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c660f5c; /* cmp/pz r0 */
L8c660f5c: if(!(*sr&1u))goto L8c660f82;goto L8c660f5e; /* bf 0x8c660f82 */
L8c660f5e: *stop=0x8c660f5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66103cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c660f60; /* mov.l 0x8c66103c,r1 */
L8c660f60: *stop=0x8c660f60u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c660f62; /* mov.l @r15,r0 */
L8c660f62: *stop=0x8c660f62u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660f64; /* cmp/eq r1,r0 */
L8c660f64: if(!(*sr&1u))goto L8c660f76;goto L8c660f66; /* bf 0x8c660f76 */
L8c660f66: *stop=0x8c660f66u;r[4]=r[15];goto L8c660f68; /* mov r15,r4 */
L8c660f68: *stop=0x8c660f68u;r[5]=12u;goto L8c660f6a; /* mov #0xc,r5 */
L8c660f6a: target=r[14];*pr=0x8c660f6eu;*stop=0x8c660f6cu;r[4]+=24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660f6e; /* jsr @r14 */
L8c660f6e: *stop=0x8c660f6eu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c660f70; /* cmp/pz r0 */
L8c660f70: if((*sr&1u))goto L8c660f94;goto L8c660f72; /* bt 0x8c660f94 */
L8c660f72: *stop=0x8c660f74u;goto L8c660f82; /* bra 0x8c660f82 */
L8c660f76: *stop=0x8c660f76u;r[4]=r[15];goto L8c660f78; /* mov r15,r4 */
L8c660f78: *stop=0x8c660f78u;r[5]=4u;goto L8c660f7a; /* mov #0x4,r5 */
L8c660f7a: target=r[14];*pr=0x8c660f7eu;*stop=0x8c660f7cu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660f7e; /* jsr @r14 */
L8c660f7e: *stop=0x8c660f7eu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c660f80; /* cmp/pz r0 */
L8c660f80: if((*sr&1u))goto L8c660f8c;goto L8c660f82; /* bt 0x8c660f8c */
L8c660f82: *stop=0x8c660f82u;r[0]=4294967295u;goto L8c660f84; /* mov #-0x1,r0 */
L8c660f84: *stop=0x8c660f84u;r[15]+=36u;goto L8c660f86; /* add #0x24,r15 */
L8c660f86: *stop=0x8c660f86u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660f88; /* lds.l @r15+,PR */
L8c660f88: *stop=0x8c660f88u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c660f8au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c660f8c: *stop=0x8c660f8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c661040u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c660f8e; /* mov.l 0x8c661040,r14 */
L8c660f8e: *stop=0x8c660f8eu;r[5]=1u;goto L8c660f90; /* mov #0x1,r5 */
L8c660f90: target=r[14];*pr=0x8c660f94u;*stop=0x8c660f92u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660f94; /* jsr @r14 */
L8c660f94: *stop=0x8c660f94u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c660f96; /* mov.l @r15,r2 */
L8c660f96: *stop=0x8c660f96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66103cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c660f98; /* mov.l 0x8c66103c,r4 */
L8c660f98: *stop=0x8c660f98u;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c660f9a; /* cmp/eq r4,r2 */
L8c660f9a: if(!(*sr&1u))goto L8c660f52;goto L8c660f9c; /* bf 0x8c660f52 */
L8c660f9c: *stop=0x8c660f9cu;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660f9e; /* mov.l @(0x18,r15),r3 */
L8c660f9e: *stop=0x8c660f9eu;r[0]=32u;goto L8c660fa0; /* mov #0x20,r0 */
L8c660fa0: *stop=0x8c660fa0u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c660fa2; /* mov.l @(0x14,r15),r1 */
L8c660fa2: *stop=0x8c660fa2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c661044u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c660fa4; /* mov.l 0x8c661044,r4 */
L8c660fa4: *stop=0x8c660fa4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c660fa6; /* mov.l r3,@r1 */
L8c660fa6: *stop=0x8c660fa6u;if(!valid_access(r[0]+r[15],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c660fa8; /* mov.w @(r0,r15),r3 */
L8c660fa8: *stop=0x8c660fa8u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660faa; /* mov.l @(0x8,r15),r2 */
L8c660faa: *stop=0x8c660faau;r[3]=r[3]&65535u;goto L8c660fac; /* extu.w r3,r3 */
L8c660fac: *stop=0x8c660facu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c660fae; /* mov.l r3,@r2 */
L8c660fae: *stop=0x8c660faeu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660fb0; /* mov.l @(0x8,r15),r0 */
L8c660fb0: *stop=0x8c660fb0u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c660fb2; /* mov.l @r0,r0 */
L8c660fb2: *stop=0x8c660fb2u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c660fb4; /* cmp/eq #0x8,r0 */
L8c660fb4: take=(*sr&1u);*stop=0x8c660fb6u;r[5]=4u;if(take)goto L8c660fdc;goto L8c660fb8; /* bt/s 0x8c660fdc */
L8c660fb8: *stop=0x8c660fb8u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c660fba; /* cmp/eq #0x10,r0 */
L8c660fba: if((*sr&1u))goto L8c660fdc;goto L8c660fbc; /* bt 0x8c660fdc */
L8c660fbc: *stop=0x8c660fbcu;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c660fbe; /* cmp/eq #0x20,r0 */
L8c660fbe: if((*sr&1u))goto L8c660fdc;goto L8c660fc0; /* bt 0x8c660fdc */
L8c660fc0: *stop=0x8c660fc0u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c660fc2; /* cmp/eq #0x40,r0 */
L8c660fc2: if((*sr&1u))goto L8c660fdc;goto L8c660fc4; /* bt 0x8c660fdc */
L8c660fc4: *stop=0x8c660fc4u;r[1]=sx(m,bus,&io_status,0x8c66102eu,2);if(io_status)return io_status;goto L8c660fc6; /* mov.w 0x8c66102e,r1 */
L8c660fc6: *stop=0x8c660fc6u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660fc8; /* cmp/eq r1,r0 */
L8c660fc8: if((*sr&1u))goto L8c660fdc;goto L8c660fca; /* bt 0x8c660fdc */
L8c660fca: *stop=0x8c660fcau;r[1]=sx(m,bus,&io_status,0x8c661030u,2);if(io_status)return io_status;goto L8c660fcc; /* mov.w 0x8c661030,r1 */
L8c660fcc: *stop=0x8c660fccu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660fce; /* cmp/eq r1,r0 */
L8c660fce: if((*sr&1u))goto L8c660fdc;goto L8c660fd0; /* bt 0x8c660fdc */
L8c660fd0: *stop=0x8c660fd0u;r[1]=sx(m,bus,&io_status,0x8c661032u,2);if(io_status)return io_status;goto L8c660fd2; /* mov.w 0x8c661032,r1 */
L8c660fd2: *stop=0x8c660fd2u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660fd4; /* cmp/eq r1,r0 */
L8c660fd4: if((*sr&1u))goto L8c660fdc;goto L8c660fd6; /* bt 0x8c660fdc */
L8c660fd6: *stop=0x8c660fd6u;r[1]=sx(m,bus,&io_status,0x8c661034u,2);if(io_status)return io_status;goto L8c660fd8; /* mov.w 0x8c661034,r1 */
L8c660fd8: *stop=0x8c660fd8u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660fda; /* cmp/eq r1,r0 */
L8c660fda: if(!(*sr&1u))goto L8c661012;goto L8c660fdc; /* bf 0x8c661012 */
L8c660fdc: *stop=0x8c660fdcu;r[0]=34u;goto L8c660fde; /* mov #0x22,r0 */
L8c660fde: *stop=0x8c660fdeu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660fe0; /* mov.l @(0xc,r15),r3 */
L8c660fe0: *stop=0x8c660fe0u;if(!valid_access(r[0]+r[15],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[15],2);if(io_status)return io_status;goto L8c660fe2; /* mov.w @(r0,r15),r2 */
L8c660fe2: *stop=0x8c660fe2u;r[2]=r[2]&65535u;goto L8c660fe4; /* extu.w r2,r2 */
L8c660fe4: *stop=0x8c660fe4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c660fe6; /* mov.l r2,@r3 */
L8c660fe6: *stop=0x8c660fe6u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660fe8; /* mov.l @(0xc,r15),r0 */
L8c660fe8: *stop=0x8c660fe8u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c660fea; /* mov.l @r0,r0 */
L8c660fea: *stop=0x8c660feau;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c660fec; /* cmp/eq #0x8,r0 */
L8c660fec: if((*sr&1u))goto L8c66101e;goto L8c660fee; /* bt 0x8c66101e */
L8c660fee: *stop=0x8c660feeu;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c660ff0; /* cmp/eq #0x10,r0 */
L8c660ff0: if((*sr&1u))goto L8c66101e;goto L8c660ff2; /* bt 0x8c66101e */
L8c660ff2: *stop=0x8c660ff2u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c660ff4; /* cmp/eq #0x20,r0 */
L8c660ff4: if((*sr&1u))goto L8c66101e;goto L8c660ff6; /* bt 0x8c66101e */
L8c660ff6: *stop=0x8c660ff6u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c660ff8; /* cmp/eq #0x40,r0 */
L8c660ff8: if((*sr&1u))goto L8c66101e;goto L8c660ffa; /* bt 0x8c66101e */
L8c660ffa: *stop=0x8c660ffau;r[1]=sx(m,bus,&io_status,0x8c66102eu,2);if(io_status)return io_status;goto L8c660ffc; /* mov.w 0x8c66102e,r1 */
L8c660ffc: *stop=0x8c660ffcu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660ffe; /* cmp/eq r1,r0 */
L8c660ffe: if((*sr&1u))goto L8c66101e;goto L8c661000; /* bt 0x8c66101e */
L8c661000: *stop=0x8c661000u;r[1]=sx(m,bus,&io_status,0x8c661030u,2);if(io_status)return io_status;goto L8c661002; /* mov.w 0x8c661030,r1 */
L8c661002: *stop=0x8c661002u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c661004; /* cmp/eq r1,r0 */
L8c661004: if((*sr&1u))goto L8c66101e;goto L8c661006; /* bt 0x8c66101e */
L8c661006: *stop=0x8c661006u;r[1]=sx(m,bus,&io_status,0x8c661032u,2);if(io_status)return io_status;goto L8c661008; /* mov.w 0x8c661032,r1 */
L8c661008: *stop=0x8c661008u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c66100a; /* cmp/eq r1,r0 */
L8c66100a: if((*sr&1u))goto L8c66101e;goto L8c66100c; /* bt 0x8c66101e */
L8c66100c: *stop=0x8c66100cu;r[1]=sx(m,bus,&io_status,0x8c661034u,2);if(io_status)return io_status;goto L8c66100e; /* mov.w 0x8c661034,r1 */
L8c66100e: *stop=0x8c66100eu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c661010; /* cmp/eq r1,r0 */
L8c661010: if((*sr&1u))goto L8c66101e;goto L8c661012; /* bt 0x8c66101e */
L8c661012: *stop=0x8c661012u;r[0]=4294967292u;goto L8c661014; /* mov #-0x4,r0 */
L8c661014: *stop=0x8c661014u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;goto L8c661016; /* mov.l r5,@(0x8,r4) */
L8c661016: *stop=0x8c661016u;r[15]+=36u;goto L8c661018; /* add #0x24,r15 */
L8c661018: *stop=0x8c661018u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66101a; /* lds.l @r15+,PR */
L8c66101a: *stop=0x8c66101au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c66101cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c66101e: *stop=0x8c66101eu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c661020; /* mov.l @(0x1c,r15),r3 */
L8c661020: *stop=0x8c661020u;r[0]=1u;goto L8c661022; /* mov #0x1,r0 */
L8c661022: *stop=0x8c661022u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c661024; /* mov.l @(0x10,r15),r2 */
L8c661024: *stop=0x8c661024u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c661026; /* mov.l r3,@r2 */
L8c661026: *stop=0x8c661026u;r[15]+=36u;goto L8c661028; /* add #0x24,r15 */
L8c661028: *stop=0x8c661028u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66102a; /* lds.l @r15+,PR */
L8c66102a: *stop=0x8c66102au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c66102cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_texture_link_8c660f44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c660f44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c660f5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c660f44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660f5au);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c660f6e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c660f44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660f6eu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c660f7e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c660f44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660f7eu);}
SA_STAGE_EXPORT int sa_player_texture_link_resume_8c660f94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_texture_link_run_8c660f44(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660f94u);}
