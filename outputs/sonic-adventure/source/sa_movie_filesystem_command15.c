/* Original command15 whole-device read.
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
static int sa_movie_filesystem_command15_run_8c64b584(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64b5e4u)goto L8c64b5e4;goto L8c64b584;
L8c64b584: *stop=0x8c64b584u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b586; /* mov.l r14,@-r15 */
L8c64b586: *stop=0x8c64b586u;r[2]=1u;goto L8c64b588; /* mov #0x1,r2 */
L8c64b588: *stop=0x8c64b588u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b58a; /* mov.l r13,@-r15 */
L8c64b58a: *stop=0x8c64b58au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b58c; /* mov.l r12,@-r15 */
L8c64b58c: *stop=0x8c64b58cu;r[12]=r[5];goto L8c64b58e; /* mov r5,r12 */
L8c64b58e: *stop=0x8c64b58eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64b590; /* mov.l r11,@-r15 */
L8c64b590: *stop=0x8c64b590u;r[13]=sx(m,bus,&io_status,0x8c64b5c6u,2);if(io_status)return io_status;goto L8c64b592; /* mov.w 0x8c64b5c6,r13 */
L8c64b592: *stop=0x8c64b592u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64b594; /* mov.l r10,@-r15 */
L8c64b594: *stop=0x8c64b594u;r[10]=r[4];goto L8c64b596; /* mov r4,r10 */
L8c64b596: *stop=0x8c64b596u;*macl=r[13]*r[10];goto L8c64b598; /* mul.l r13,r10 */
L8c64b598: *stop=0x8c64b598u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b5d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b59a; /* mov.l 0x8c64b5d0,r3 */
L8c64b59a: *stop=0x8c64b59au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b5d4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b59c; /* mov.l 0x8c64b5d4,r0 */
L8c64b59c: *stop=0x8c64b59cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64b59e; /* mov.l r9,@-r15 */
L8c64b59e: *stop=0x8c64b59eu;r[13]=*macl;goto L8c64b5a0; /* sts MACL,r13 */
L8c64b5a0: *stop=0x8c64b5a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b5a2; /* sts.l PR,@-r15 */
L8c64b5a2: *stop=0x8c64b5a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b5d8u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c64b5a4; /* mov.l 0x8c64b5d8,r9 */
L8c64b5a4: *stop=0x8c64b5a4u;r[13]+=r[3];goto L8c64b5a6; /* add r3,r13 */
L8c64b5a6: *stop=0x8c64b5a6u;r[11]=sx(m,bus,&io_status,0x8c64b5c8u,2);if(io_status)return io_status;goto L8c64b5a8; /* mov.w 0x8c64b5c8,r11 */
L8c64b5a8: *stop=0x8c64b5a8u;if(!valid_access(r[13]+48u,4u))return -3;wr(m,bus,&io_status,r[13]+0x30u,r[2],4);if(io_status)return io_status;goto L8c64b5aa; /* mov.l r2,@(0x30,r13) */
L8c64b5aa: *stop=0x8c64b5aau;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b5ac; /* mov.l @(0x4,r13),r1 */
L8c64b5ac: *stop=0x8c64b5acu;r[2]=r[1];goto L8c64b5ae; /* mov r1,r2 */
L8c64b5ae: *stop=0x8c64b5aeu;r[1]<<=2;goto L8c64b5b0; /* shll2 r1 */
L8c64b5b0: *stop=0x8c64b5b0u;r[1]+=r[2];goto L8c64b5b2; /* add r2,r1 */
L8c64b5b2: *stop=0x8c64b5b2u;r[1]<<=2;goto L8c64b5b4; /* shll2 r1 */
L8c64b5b4: *stop=0x8c64b5b4u;r[1]<<=2;goto L8c64b5b6; /* shll2 r1 */
L8c64b5b6: *stop=0x8c64b5b6u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b5b8; /* mov.l @(r0,r1),r0 */
L8c64b5b8: *stop=0x8c64b5b8u;if(!valid_access(r[13]+52u,4u))return -3;wr(m,bus,&io_status,r[13]+0x34u,r[0],4);if(io_status)return io_status;goto L8c64b5ba; /* mov.l r0,@(0x34,r13) */
L8c64b5ba: *stop=0x8c64b5bcu;r[14]=0u;goto L8c64b5f2; /* bra 0x8c64b5f2 */
L8c64b5dc: *stop=0x8c64b5dcu;r[5]=r[14];goto L8c64b5de; /* mov r14,r5 */
L8c64b5de: *stop=0x8c64b5deu;r[6]=r[12];goto L8c64b5e0; /* mov r12,r6 */
L8c64b5e0: target=r[9];*pr=0x8c64b5e4u;*stop=0x8c64b5e2u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b5e4; /* jsr @r9 */
L8c64b5e4: *stop=0x8c64b5e4u;r[4]=r[0];goto L8c64b5e6; /* mov r0,r4 */
L8c64b5e6: *stop=0x8c64b5e6u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64b5e8; /* cmp/pz r4 */
L8c64b5e8: if((*sr&1u))goto L8c64b5ee;goto L8c64b5ea; /* bt 0x8c64b5ee */
L8c64b5ea: *stop=0x8c64b5ecu;r[0]=r[4];goto L8c64b5fe; /* bra 0x8c64b5fe */
L8c64b5ee: *stop=0x8c64b5eeu;r[14]+=1u;goto L8c64b5f0; /* add #0x1,r14 */
L8c64b5f0: *stop=0x8c64b5f0u;r[12]+=r[11];goto L8c64b5f2; /* add r11,r12 */
L8c64b5f2: *stop=0x8c64b5f2u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b5f4; /* mov.l @(0x1c,r13),r2 */
L8c64b5f4: *stop=0x8c64b5f4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b5f6; /* mov.l @r2,r3 */
L8c64b5f6: *stop=0x8c64b5f6u;r[3]+=1u;goto L8c64b5f8; /* add #0x1,r3 */
L8c64b5f8: *stop=0x8c64b5f8u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[3]);goto L8c64b5fa; /* cmp/ge r3,r14 */
L8c64b5fa: if(!(*sr&1u))goto L8c64b5dc;goto L8c64b5fc; /* bf 0x8c64b5dc */
L8c64b5fc: *stop=0x8c64b5fcu;r[0]=0u;goto L8c64b5fe; /* mov #0x0,r0 */
L8c64b5fe: *stop=0x8c64b5feu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b600; /* lds.l @r15+,PR */
L8c64b600: *stop=0x8c64b600u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64b602; /* mov.l @r15+,r9 */
L8c64b602: *stop=0x8c64b602u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64b604; /* mov.l @r15+,r10 */
L8c64b604: *stop=0x8c64b604u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64b606; /* mov.l @r15+,r11 */
L8c64b606: *stop=0x8c64b606u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64b608; /* mov.l @r15+,r12 */
L8c64b608: *stop=0x8c64b608u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b60a; /* mov.l @r15+,r13 */
L8c64b60a: *stop=0x8c64b60au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b60cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command15_8c64b584(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command15_run_8c64b584(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command15_resume_8c64b5e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command15_run_8c64b584(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b5e4u);}
