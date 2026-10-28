/* Original command10 directory-record byte/optional-word update and persistence.
 * Explicit register state, original callbacks and nonlocal continuations.
 * No supplied parser results or saved task frames.
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
int sa_movie_channel_io_return_allowed(void *,uint32_t);
static int sa_movie_filesystem_command10_run_8c64a244(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64a288u)goto L8c64a288;if(resume_pc==0x8c64a292u)goto L8c64a292;if(resume_pc==0x8c64a29cu)goto L8c64a29c;if(resume_pc==0x8c64a2b8u)goto L8c64a2b8;if(resume_pc==0x8c64a2cau)goto L8c64a2ca;if(resume_pc==0x8c64a2deu)goto L8c64a2de;if(resume_pc==0x8c64a2f2u)goto L8c64a2f2;if(resume_pc==0x8c64a30eu)goto L8c64a30e;if(resume_pc==0x8c64a31au)goto L8c64a31a;if(resume_pc==0x8c64a31eu)goto L8c64a31e;goto L8c64a244;
L8c64a244: *stop=0x8c64a244u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a246; /* mov.l r14,@-r15 */
L8c64a246: *stop=0x8c64a246u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64a248; /* mov.l r13,@-r15 */
L8c64a248: *stop=0x8c64a248u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64a24a; /* mov.l r12,@-r15 */
L8c64a24a: *stop=0x8c64a24au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64a24c; /* sts.l PR,@-r15 */
L8c64a24c: *stop=0x8c64a24cu;r[15]+=4294967236u;goto L8c64a24e; /* add #-0x3c,r15 */
L8c64a24e: *stop=0x8c64a24eu;r[13]=sx(m,bus,&io_status,0x8c64a32au,2);if(io_status)return io_status;goto L8c64a250; /* mov.w 0x8c64a32a,r13 */
L8c64a250: *stop=0x8c64a250u;r[14]=r[4];goto L8c64a252; /* mov r4,r14 */
L8c64a252: *stop=0x8c64a252u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a334u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a254; /* mov.l 0x8c64a334,r3 */
L8c64a254: *stop=0x8c64a254u;r[2]=1u;goto L8c64a256; /* mov #0x1,r2 */
L8c64a256: *stop=0x8c64a256u;*macl=r[13]*r[14];goto L8c64a258; /* mul.l r13,r14 */
L8c64a258: *stop=0x8c64a258u;r[0]=r[6];goto L8c64a25a; /* mov r6,r0 */
L8c64a25a: *stop=0x8c64a25au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64a25c; /* mov.l r5,@r15 */
L8c64a25c: *stop=0x8c64a25cu;if(!valid_access(r[15]+8u,2u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],2);if(io_status)return io_status;goto L8c64a25e; /* mov.w r0,@(0x8,r15) */
L8c64a25e: *stop=0x8c64a25eu;r[0]=r[7];goto L8c64a260; /* mov r7,r0 */
L8c64a260: *stop=0x8c64a260u;if(!valid_access(r[15]+4u,1u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],1);if(io_status)return io_status;goto L8c64a262; /* mov.b r0,@(0x4,r15) */
L8c64a262: *stop=0x8c64a262u;r[13]=*macl;goto L8c64a264; /* sts MACL,r13 */
L8c64a264: *stop=0x8c64a264u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a338u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64a266; /* mov.l 0x8c64a338,r0 */
L8c64a266: *stop=0x8c64a266u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[13],4);if(io_status)return io_status;goto L8c64a268; /* mov.l r13,@(0xc,r15) */
L8c64a268: *stop=0x8c64a268u;r[13]+=r[3];goto L8c64a26a; /* add r3,r13 */
L8c64a26a: *stop=0x8c64a26au;if(!valid_access(r[13]+48u,4u))return -3;wr(m,bus,&io_status,r[13]+0x30u,r[2],4);if(io_status)return io_status;goto L8c64a26c; /* mov.l r2,@(0x30,r13) */
L8c64a26c: *stop=0x8c64a26cu;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64a26e; /* mov.l @(0x4,r13),r1 */
L8c64a26e: *stop=0x8c64a26eu;r[2]=r[1];goto L8c64a270; /* mov r1,r2 */
L8c64a270: *stop=0x8c64a270u;r[1]<<=2;goto L8c64a272; /* shll2 r1 */
L8c64a272: *stop=0x8c64a272u;r[1]+=r[2];goto L8c64a274; /* add r2,r1 */
L8c64a274: *stop=0x8c64a274u;r[1]<<=2;goto L8c64a276; /* shll2 r1 */
L8c64a276: *stop=0x8c64a276u;r[1]<<=2;goto L8c64a278; /* shll2 r1 */
L8c64a278: *stop=0x8c64a278u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64a27a; /* mov.l @(r0,r1),r0 */
L8c64a27a: *stop=0x8c64a27au;if(!valid_access(r[13]+52u,4u))return -3;wr(m,bus,&io_status,r[13]+0x34u,r[0],4);if(io_status)return io_status;goto L8c64a27c; /* mov.l r0,@(0x34,r13) */
L8c64a27c: *stop=0x8c64a27cu;if(!valid_access(r[13]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x2u,1);if(io_status)return io_status;goto L8c64a27e; /* mov.b @(0x2,r13),r0 */
L8c64a27e: *stop=0x8c64a27eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a280; /* tst r0,r0 */
L8c64a280: if((*sr&1u))goto L8c64a2ac;goto L8c64a282; /* bt 0x8c64a2ac */
L8c64a282: *stop=0x8c64a282u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a33cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a284; /* mov.l 0x8c64a33c,r3 */
L8c64a284: target=r[3];*pr=0x8c64a288u;*stop=0x8c64a286u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a288; /* jsr @r3 */
L8c64a288: *stop=0x8c64a288u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a28a; /* tst r0,r0 */
L8c64a28a: if(!(*sr&1u))goto L8c64a296;goto L8c64a28c; /* bf 0x8c64a296 */
L8c64a28c: *stop=0x8c64a28cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a340u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a28e; /* mov.l 0x8c64a340,r3 */
L8c64a28e: target=r[3];*pr=0x8c64a292u;*stop=0x8c64a290u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a292; /* jsr @r3 */
L8c64a292: *stop=0x8c64a294u;goto L8c64a2ac; /* bra 0x8c64a2ac */
L8c64a296: *stop=0x8c64a296u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a344u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a298; /* mov.l 0x8c64a344,r2 */
L8c64a298: target=r[2];*pr=0x8c64a29cu;*stop=0x8c64a29au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a29c; /* jsr @r2 */
L8c64a29c: *stop=0x8c64a29cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a29e; /* tst r0,r0 */
L8c64a29e: if((*sr&1u))goto L8c64a2ac;goto L8c64a2a0; /* bt 0x8c64a2ac */
L8c64a2a0: *stop=0x8c64a2a0u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a2a2; /* mov.l @(0xc,r15),r2 */
L8c64a2a2: *stop=0x8c64a2a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a334u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a2a4; /* mov.l 0x8c64a334,r3 */
L8c64a2a4: *stop=0x8c64a2a4u;r[2]+=r[3];goto L8c64a2a6; /* add r3,r2 */
L8c64a2a6: *stop=0x8c64a2a6u;if(!valid_access(r[2]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x8u,1);if(io_status)return io_status;goto L8c64a2a8; /* mov.b @(0x8,r2),r0 */
L8c64a2a8: *stop=0x8c64a2a8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a2aa; /* tst r0,r0 */
L8c64a2aa: if(!(*sr&1u))goto L8c64a2b2;goto L8c64a2ac; /* bf 0x8c64a2b2 */
L8c64a2ac: *stop=0x8c64a2acu;r[0]=sx(m,bus,&io_status,0x8c64a32cu,2);if(io_status)return io_status;goto L8c64a2ae; /* mov.w 0x8c64a32c,r0 */
L8c64a2ae: *stop=0x8c64a2b0u;goto L8c64a31e; /* bra 0x8c64a31e */
L8c64a2b2: *stop=0x8c64a2b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a348u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a2b4; /* mov.l 0x8c64a348,r3 */
L8c64a2b4: target=r[3];*pr=0x8c64a2b8u;*stop=0x8c64a2b6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a2b8; /* jsr @r3 */
L8c64a2b8: *stop=0x8c64a2b8u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64a2ba; /* cmp/pz r0 */
L8c64a2ba: if((*sr&1u))goto L8c64a2c2;goto L8c64a2bc; /* bt 0x8c64a2c2 */
L8c64a2bc: *stop=0x8c64a2bcu;r[0]=sx(m,bus,&io_status,0x8c64a32eu,2);if(io_status)return io_status;goto L8c64a2be; /* mov.w 0x8c64a32e,r0 */
L8c64a2be: *stop=0x8c64a2c0u;goto L8c64a31e; /* bra 0x8c64a31e */
L8c64a2c2: *stop=0x8c64a2c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a34cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a2c4; /* mov.l 0x8c64a34c,r3 */
L8c64a2c4: *stop=0x8c64a2c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64a2c6; /* mov.l @r15,r5 */
L8c64a2c6: target=r[3];*pr=0x8c64a2cau;*stop=0x8c64a2c8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a2ca; /* jsr @r3 */
L8c64a2ca: *stop=0x8c64a2cau;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64a2cc; /* cmp/pz r0 */
L8c64a2cc: if((*sr&1u))goto L8c64a2d4;goto L8c64a2ce; /* bt 0x8c64a2d4 */
L8c64a2ce: *stop=0x8c64a2ceu;r[0]=sx(m,bus,&io_status,0x8c64a330u,2);if(io_status)return io_status;goto L8c64a2d0; /* mov.w 0x8c64a330,r0 */
L8c64a2d0: *stop=0x8c64a2d2u;goto L8c64a31e; /* bra 0x8c64a31e */
L8c64a2d4: *stop=0x8c64a2d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64a2d6; /* mov.l @r15,r5 */
L8c64a2d6: *stop=0x8c64a2d6u;r[6]=r[15];goto L8c64a2d8; /* mov r15,r6 */
L8c64a2d8: *stop=0x8c64a2d8u;r[6]+=16u;goto L8c64a2da; /* add #0x10,r6 */
L8c64a2da: target=0x8c64ad56u;*pr=0x8c64a2deu;*stop=0x8c64a2dcu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a2de; /* bsr 0x8c64ad56 */
L8c64a2de: *stop=0x8c64a2deu;r[13]=r[0];goto L8c64a2e0; /* mov r0,r13 */
L8c64a2e0: *stop=0x8c64a2e0u;*sr=(*sr&~1u)|((int32_t)r[13]>=0);goto L8c64a2e2; /* cmp/pz r13 */
L8c64a2e2: if((*sr&1u))goto L8c64a2ea;goto L8c64a2e4; /* bt 0x8c64a2ea */
L8c64a2e4: *stop=0x8c64a2e4u;r[0]=sx(m,bus,&io_status,0x8c64a332u,2);if(io_status)return io_status;goto L8c64a2e6; /* mov.w 0x8c64a332,r0 */
L8c64a2e6: *stop=0x8c64a2e8u;goto L8c64a31e; /* bra 0x8c64a31e */
L8c64a2ea: *stop=0x8c64a2eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a350u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a2ec; /* mov.l 0x8c64a350,r3 */
L8c64a2ec: *stop=0x8c64a2ecu;r[5]=r[13];goto L8c64a2ee; /* mov r13,r5 */
L8c64a2ee: target=r[3];*pr=0x8c64a2f2u;*stop=0x8c64a2f0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a2f2; /* jsr @r3 */
L8c64a2f2: *stop=0x8c64a2f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a354u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a2f4; /* mov.l 0x8c64a354,r3 */
L8c64a2f4: *stop=0x8c64a2f4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a2f6; /* mov.l @r3,r2 */
L8c64a2f6: *stop=0x8c64a2f6u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64a2f8; /* tst r2,r2 */
L8c64a2f8: take=(*sr&1u);*stop=0x8c64a2fau;r[12]=r[0];if(take)goto L8c64a30e;goto L8c64a2fc; /* bt/s 0x8c64a30e */
L8c64a2fc: *stop=0x8c64a2fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a358u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64a2fe; /* mov.l 0x8c64a358,r1 */
L8c64a2fe: *stop=0x8c64a2feu;r[5]=26u;goto L8c64a300; /* mov #0x1a,r5 */
L8c64a300: *stop=0x8c64a300u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a302; /* mov.l @r1,r2 */
L8c64a302: *stop=0x8c64a302u;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64a304; /* mov.l @(0x10,r2),r0 */
L8c64a304: *stop=0x8c64a304u;r[2]=r[15];goto L8c64a306; /* mov r15,r2 */
L8c64a306: *stop=0x8c64a306u;r[2]+=8u;goto L8c64a308; /* add #0x8,r2 */
L8c64a308: *stop=0x8c64a308u;if(!valid_access(r[2],2u))return -3;r[6]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c64a30a; /* mov.w @r2,r6 */
L8c64a30a: target=r[0];*pr=0x8c64a30eu;*stop=0x8c64a30cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a30e; /* jsr @r0 */
L8c64a30e: *stop=0x8c64a30eu;if(!valid_access(r[15]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x4u,1);if(io_status)return io_status;goto L8c64a310; /* mov.b @(0x4,r15),r0 */
L8c64a310: *stop=0x8c64a310u;r[5]=r[13];goto L8c64a312; /* mov r13,r5 */
L8c64a312: *stop=0x8c64a312u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a35cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a314; /* mov.l 0x8c64a35c,r3 */
L8c64a314: *stop=0x8c64a314u;if(!valid_access(r[12]+1u,1u))return -3;wr(m,bus,&io_status,r[12]+0x1u,r[0],1);if(io_status)return io_status;goto L8c64a316; /* mov.b r0,@(0x1,r12) */
L8c64a316: target=r[3];*pr=0x8c64a31au;*stop=0x8c64a318u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a31a; /* jsr @r3 */
L8c64a31a: target=0x8c64ab36u;*pr=0x8c64a31eu;*stop=0x8c64a31cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a31e; /* bsr 0x8c64ab36 */
L8c64a31e: *stop=0x8c64a31eu;r[15]+=60u;goto L8c64a320; /* add #0x3c,r15 */
L8c64a320: *stop=0x8c64a320u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a322; /* lds.l @r15+,PR */
L8c64a322: *stop=0x8c64a322u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64a324; /* mov.l @r15+,r12 */
L8c64a324: *stop=0x8c64a324u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64a326; /* mov.l @r15+,r13 */
L8c64a326: *stop=0x8c64a326u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a328u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_8c64a244(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_resume_8c64a288(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a288u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_resume_8c64a292(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a292u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_resume_8c64a29c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a29cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_resume_8c64a2b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a2b8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_resume_8c64a2ca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a2cau);}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_resume_8c64a2de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a2deu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_resume_8c64a2f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a2f2u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_resume_8c64a30e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a30eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_resume_8c64a31a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a31au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command10_resume_8c64a31e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command10_run_8c64a244(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a31eu);}
