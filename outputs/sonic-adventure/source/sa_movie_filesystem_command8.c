/* Original command8 directory rename with unchanged persistence callees.
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
static int sa_movie_filesystem_command8_run_8c649f1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c649f5au)goto L8c649f5a;if(resume_pc==0x8c649f64u)goto L8c649f64;if(resume_pc==0x8c649f6eu)goto L8c649f6e;if(resume_pc==0x8c649f8au)goto L8c649f8a;if(resume_pc==0x8c649f9cu)goto L8c649f9c;if(resume_pc==0x8c649fb0u)goto L8c649fb0;if(resume_pc==0x8c649fc6u)goto L8c649fc6;if(resume_pc==0x8c649fd8u)goto L8c649fd8;if(resume_pc==0x8c64a026u)goto L8c64a026;if(resume_pc==0x8c64a02au)goto L8c64a02a;goto L8c649f1e;
L8c649f1e: *stop=0x8c649f1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c649f20; /* mov.l r14,@-r15 */
L8c649f20: *stop=0x8c649f20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c649f22; /* mov.l r13,@-r15 */
L8c649f22: *stop=0x8c649f22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c649f24; /* sts.l PR,@-r15 */
L8c649f24: *stop=0x8c649f24u;r[15]+=4294967240u;goto L8c649f26; /* add #-0x38,r15 */
L8c649f26: *stop=0x8c649f26u;r[13]=sx(m,bus,&io_status,0x8c649fe6u,2);if(io_status)return io_status;goto L8c649f28; /* mov.w 0x8c649fe6,r13 */
L8c649f28: *stop=0x8c649f28u;r[14]=r[4];goto L8c649f2a; /* mov r4,r14 */
L8c649f2a: *stop=0x8c649f2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c649ff4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649f2c; /* mov.l 0x8c649ff4,r3 */
L8c649f2c: *stop=0x8c649f2cu;r[2]=1u;goto L8c649f2e; /* mov #0x1,r2 */
L8c649f2e: *stop=0x8c649f2eu;*macl=r[13]*r[14];goto L8c649f30; /* mul.l r13,r14 */
L8c649f30: *stop=0x8c649f30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649ff8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c649f32; /* mov.l 0x8c649ff8,r0 */
L8c649f32: *stop=0x8c649f32u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c649f34; /* mov.l r5,@r15 */
L8c649f34: *stop=0x8c649f34u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c649f36; /* mov.l r6,@(0x4,r15) */
L8c649f36: *stop=0x8c649f36u;r[13]=*macl;goto L8c649f38; /* sts MACL,r13 */
L8c649f38: *stop=0x8c649f38u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[13],4);if(io_status)return io_status;goto L8c649f3a; /* mov.l r13,@(0x8,r15) */
L8c649f3a: *stop=0x8c649f3au;r[13]+=r[3];goto L8c649f3c; /* add r3,r13 */
L8c649f3c: *stop=0x8c649f3cu;if(!valid_access(r[13]+48u,4u))return -3;wr(m,bus,&io_status,r[13]+0x30u,r[2],4);if(io_status)return io_status;goto L8c649f3e; /* mov.l r2,@(0x30,r13) */
L8c649f3e: *stop=0x8c649f3eu;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c649f40; /* mov.l @(0x4,r13),r1 */
L8c649f40: *stop=0x8c649f40u;r[2]=r[1];goto L8c649f42; /* mov r1,r2 */
L8c649f42: *stop=0x8c649f42u;r[1]<<=2;goto L8c649f44; /* shll2 r1 */
L8c649f44: *stop=0x8c649f44u;r[1]+=r[2];goto L8c649f46; /* add r2,r1 */
L8c649f46: *stop=0x8c649f46u;r[1]<<=2;goto L8c649f48; /* shll2 r1 */
L8c649f48: *stop=0x8c649f48u;r[1]<<=2;goto L8c649f4a; /* shll2 r1 */
L8c649f4a: *stop=0x8c649f4au;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c649f4c; /* mov.l @(r0,r1),r0 */
L8c649f4c: *stop=0x8c649f4cu;if(!valid_access(r[13]+52u,4u))return -3;wr(m,bus,&io_status,r[13]+0x34u,r[0],4);if(io_status)return io_status;goto L8c649f4e; /* mov.l r0,@(0x34,r13) */
L8c649f4e: *stop=0x8c649f4eu;if(!valid_access(r[13]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x2u,1);if(io_status)return io_status;goto L8c649f50; /* mov.b @(0x2,r13),r0 */
L8c649f50: *stop=0x8c649f50u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649f52; /* tst r0,r0 */
L8c649f52: if((*sr&1u))goto L8c649f7e;goto L8c649f54; /* bt 0x8c649f7e */
L8c649f54: *stop=0x8c649f54u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649ffcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649f56; /* mov.l 0x8c649ffc,r3 */
L8c649f56: target=r[3];*pr=0x8c649f5au;*stop=0x8c649f58u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649f5a; /* jsr @r3 */
L8c649f5a: *stop=0x8c649f5au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649f5c; /* tst r0,r0 */
L8c649f5c: if(!(*sr&1u))goto L8c649f68;goto L8c649f5e; /* bf 0x8c649f68 */
L8c649f5e: *stop=0x8c649f5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a000u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649f60; /* mov.l 0x8c64a000,r3 */
L8c649f60: target=r[3];*pr=0x8c649f64u;*stop=0x8c649f62u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649f64; /* jsr @r3 */
L8c649f64: *stop=0x8c649f66u;goto L8c649f7e; /* bra 0x8c649f7e */
L8c649f68: *stop=0x8c649f68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a004u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649f6a; /* mov.l 0x8c64a004,r2 */
L8c649f6a: target=r[2];*pr=0x8c649f6eu;*stop=0x8c649f6cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649f6e; /* jsr @r2 */
L8c649f6e: *stop=0x8c649f6eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649f70; /* tst r0,r0 */
L8c649f70: if((*sr&1u))goto L8c649f7e;goto L8c649f72; /* bt 0x8c649f7e */
L8c649f72: *stop=0x8c649f72u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649f74; /* mov.l @(0x8,r15),r2 */
L8c649f74: *stop=0x8c649f74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649ff4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649f76; /* mov.l 0x8c649ff4,r3 */
L8c649f76: *stop=0x8c649f76u;r[2]+=r[3];goto L8c649f78; /* add r3,r2 */
L8c649f78: *stop=0x8c649f78u;if(!valid_access(r[2]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x8u,1);if(io_status)return io_status;goto L8c649f7a; /* mov.b @(0x8,r2),r0 */
L8c649f7a: *stop=0x8c649f7au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649f7c; /* tst r0,r0 */
L8c649f7c: if(!(*sr&1u))goto L8c649f84;goto L8c649f7e; /* bf 0x8c649f84 */
L8c649f7e: *stop=0x8c649f7eu;r[0]=sx(m,bus,&io_status,0x8c649fe8u,2);if(io_status)return io_status;goto L8c649f80; /* mov.w 0x8c649fe8,r0 */
L8c649f80: *stop=0x8c649f82u;goto L8c64a02a; /* bra 0x8c64a02a */
L8c649f84: *stop=0x8c649f84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a008u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649f86; /* mov.l 0x8c64a008,r3 */
L8c649f86: target=r[3];*pr=0x8c649f8au;*stop=0x8c649f88u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649f8a; /* jsr @r3 */
L8c649f8a: *stop=0x8c649f8au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649f8c; /* cmp/pz r0 */
L8c649f8c: if((*sr&1u))goto L8c649f94;goto L8c649f8e; /* bt 0x8c649f94 */
L8c649f8e: *stop=0x8c649f8eu;r[0]=sx(m,bus,&io_status,0x8c649feau,2);if(io_status)return io_status;goto L8c649f90; /* mov.w 0x8c649fea,r0 */
L8c649f90: *stop=0x8c649f92u;goto L8c64a02a; /* bra 0x8c64a02a */
L8c649f94: *stop=0x8c649f94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a00cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649f96; /* mov.l 0x8c64a00c,r3 */
L8c649f96: *stop=0x8c649f96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c649f98; /* mov.l @r15,r5 */
L8c649f98: target=r[3];*pr=0x8c649f9cu;*stop=0x8c649f9au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649f9c; /* jsr @r3 */
L8c649f9c: *stop=0x8c649f9cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649f9e; /* cmp/pz r0 */
L8c649f9e: if((*sr&1u))goto L8c649fa6;goto L8c649fa0; /* bt 0x8c649fa6 */
L8c649fa0: *stop=0x8c649fa0u;r[0]=sx(m,bus,&io_status,0x8c649fecu,2);if(io_status)return io_status;goto L8c649fa2; /* mov.w 0x8c649fec,r0 */
L8c649fa2: *stop=0x8c649fa4u;goto L8c64a02a; /* bra 0x8c64a02a */
L8c649fa6: *stop=0x8c649fa6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c649fa8; /* mov.l @r15,r5 */
L8c649fa8: *stop=0x8c649fa8u;r[6]=r[15];goto L8c649faa; /* mov r15,r6 */
L8c649faa: *stop=0x8c649faau;r[6]+=12u;goto L8c649fac; /* add #0xc,r6 */
L8c649fac: target=0x8c64ad56u;*pr=0x8c649fb0u;*stop=0x8c649faeu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649fb0; /* bsr 0x8c64ad56 */
L8c649fb0: *stop=0x8c649fb0u;r[13]=r[0];goto L8c649fb2; /* mov r0,r13 */
L8c649fb2: *stop=0x8c649fb2u;*sr=(*sr&~1u)|((int32_t)r[13]>=0);goto L8c649fb4; /* cmp/pz r13 */
L8c649fb4: if((*sr&1u))goto L8c649fbc;goto L8c649fb6; /* bt 0x8c649fbc */
L8c649fb6: *stop=0x8c649fb6u;r[0]=sx(m,bus,&io_status,0x8c649feeu,2);if(io_status)return io_status;goto L8c649fb8; /* mov.w 0x8c649fee,r0 */
L8c649fb8: *stop=0x8c649fbau;goto L8c64a02a; /* bra 0x8c64a02a */
L8c649fbc: *stop=0x8c649fbcu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c649fbe; /* mov.l @(0x4,r15),r5 */
L8c649fbe: *stop=0x8c649fbeu;r[6]=r[15];goto L8c649fc0; /* mov r15,r6 */
L8c649fc0: *stop=0x8c649fc0u;r[6]+=12u;goto L8c649fc2; /* add #0xc,r6 */
L8c649fc2: target=0x8c64ad56u;*pr=0x8c649fc6u;*stop=0x8c649fc4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649fc6; /* bsr 0x8c64ad56 */
L8c649fc6: *stop=0x8c649fc6u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649fc8; /* cmp/pz r0 */
L8c649fc8: if(!(*sr&1u))goto L8c649fd0;goto L8c649fca; /* bf 0x8c649fd0 */
L8c649fca: *stop=0x8c649fcau;r[0]=sx(m,bus,&io_status,0x8c649ff0u,2);if(io_status)return io_status;goto L8c649fcc; /* mov.w 0x8c649ff0,r0 */
L8c649fcc: *stop=0x8c649fceu;goto L8c64a02a; /* bra 0x8c64a02a */
L8c649fd0: *stop=0x8c649fd0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a010u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649fd2; /* mov.l 0x8c64a010,r3 */
L8c649fd2: *stop=0x8c649fd2u;r[5]=r[13];goto L8c649fd4; /* mov r13,r5 */
L8c649fd4: target=r[3];*pr=0x8c649fd8u;*stop=0x8c649fd6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649fd8; /* jsr @r3 */
L8c649fd8: *stop=0x8c649fd8u;r[5]=r[0];goto L8c649fda; /* mov r0,r5 */
L8c649fda: *stop=0x8c649fdau;r[4]=r[5];goto L8c649fdc; /* mov r5,r4 */
L8c649fdc: *stop=0x8c649fdcu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c649fde; /* mov.l @(0x4,r15),r7 */
L8c649fde: *stop=0x8c649fdeu;r[6]=r[5];goto L8c649fe0; /* mov r5,r6 */
L8c649fe0: *stop=0x8c649fe0u;r[4]+=4u;goto L8c649fe2; /* add #0x4,r4 */
L8c649fe2: *stop=0x8c649fe4u;r[6]+=16u;goto L8c64a01a; /* bra 0x8c64a01a */
L8c64a014: *stop=0x8c64a014u;if(!valid_access(r[7],1u))return -3;r[3]=sx(m,bus,&io_status,r[7],1);if(io_status)return io_status;r[7]+=1;goto L8c64a016; /* mov.b @r7+,r3 */
L8c64a016: *stop=0x8c64a016u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c64a018; /* mov.b r3,@r4 */
L8c64a018: *stop=0x8c64a018u;r[4]+=1u;goto L8c64a01a; /* add #0x1,r4 */
L8c64a01a: *stop=0x8c64a01au;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c64a01c; /* cmp/hs r6,r4 */
L8c64a01c: if(!(*sr&1u))goto L8c64a014;goto L8c64a01e; /* bf 0x8c64a014 */
L8c64a01e: *stop=0x8c64a01eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a100u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a020; /* mov.l 0x8c64a100,r3 */
L8c64a020: *stop=0x8c64a020u;r[5]=r[13];goto L8c64a022; /* mov r13,r5 */
L8c64a022: target=r[3];*pr=0x8c64a026u;*stop=0x8c64a024u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a026; /* jsr @r3 */
L8c64a026: target=0x8c64ab36u;*pr=0x8c64a02au;*stop=0x8c64a028u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a02a; /* bsr 0x8c64ab36 */
L8c64a02a: *stop=0x8c64a02au;r[15]+=56u;goto L8c64a02c; /* add #0x38,r15 */
L8c64a02c: *stop=0x8c64a02cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a02e; /* lds.l @r15+,PR */
L8c64a02e: *stop=0x8c64a02eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64a030; /* mov.l @r15+,r13 */
L8c64a030: *stop=0x8c64a030u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64a032u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_8c649f1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_resume_8c649f5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649f5au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_resume_8c649f64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649f64u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_resume_8c649f6e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649f6eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_resume_8c649f8a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649f8au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_resume_8c649f9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649f9cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_resume_8c649fb0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649fb0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_resume_8c649fc6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649fc6u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_resume_8c649fd8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649fd8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_resume_8c64a026(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a026u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command8_resume_8c64a02a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command8_run_8c649f1e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64a02au);}
