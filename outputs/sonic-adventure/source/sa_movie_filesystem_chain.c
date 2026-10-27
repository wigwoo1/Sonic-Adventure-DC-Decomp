/* Original file-chain validation and entry-address dependency.
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
static int sa_movie_filesystem_chain_run_8c64b300(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64b31cu)goto L8c64b31c;if(resume_pc==0x8c64b32cu)goto L8c64b32c;if(resume_pc==0x8c64b340u)goto L8c64b340;goto L8c64b300;
L8c64b300: *stop=0x8c64b300u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b302; /* mov.l r14,@-r15 */
L8c64b302: *stop=0x8c64b302u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b304; /* mov.l r13,@-r15 */
L8c64b304: *stop=0x8c64b304u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b306; /* mov.l r12,@-r15 */
L8c64b306: *stop=0x8c64b306u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64b308; /* mov.l r11,@-r15 */
L8c64b308: *stop=0x8c64b308u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b30a; /* sts.l PR,@-r15 */
L8c64b30a: *stop=0x8c64b30au;r[15]+=4294967252u;goto L8c64b30c; /* add #-0x2c,r15 */
L8c64b30c: *stop=0x8c64b30cu;r[11]=sx(m,bus,&io_status,0x8c64b3ccu,2);if(io_status)return io_status;goto L8c64b30e; /* mov.w 0x8c64b3cc,r11 */
L8c64b30e: *stop=0x8c64b30eu;r[12]=r[4];goto L8c64b310; /* mov r4,r12 */
L8c64b310: *stop=0x8c64b310u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b3d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b312; /* mov.l 0x8c64b3d4,r3 */
L8c64b312: *stop=0x8c64b312u;*macl=r[11]*r[12];goto L8c64b314; /* mul.l r11,r12 */
L8c64b314: *stop=0x8c64b314u;r[11]=*macl;goto L8c64b316; /* sts MACL,r11 */
L8c64b316: *stop=0x8c64b316u;r[11]+=r[3];goto L8c64b318; /* add r3,r11 */
L8c64b318: target=0x8c64ad56u;*pr=0x8c64b31cu;*stop=0x8c64b31au;r[6]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b31c; /* bsr 0x8c64ad56 */
L8c64b31c: *stop=0x8c64b31cu;r[5]=r[0];goto L8c64b31e; /* mov r0,r5 */
L8c64b31e: *stop=0x8c64b31eu;*sr=(*sr&~1u)|((int32_t)r[5]>=0);goto L8c64b320; /* cmp/pz r5 */
L8c64b320: if((*sr&1u))goto L8c64b328;goto L8c64b322; /* bt 0x8c64b328 */
L8c64b322: *stop=0x8c64b322u;r[0]=sx(m,bus,&io_status,0x8c64b3ceu,2);if(io_status)return io_status;goto L8c64b324; /* mov.w 0x8c64b3ce,r0 */
L8c64b324: *stop=0x8c64b326u;goto L8c64b382; /* bra 0x8c64b382 */
L8c64b328: target=0x8c64b274u;*pr=0x8c64b32cu;*stop=0x8c64b32au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b32c; /* bsr 0x8c64b274 */
L8c64b32c: *stop=0x8c64b32cu;if(!valid_access(r[11]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b32e; /* mov.l @(0x1c,r11),r3 */
L8c64b32e: *stop=0x8c64b32eu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64b330; /* mov.l @(0xc,r15),r13 */
L8c64b330: *stop=0x8c64b330u;if(!valid_access(r[3]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b332; /* mov.l @(0x30,r3),r2 */
L8c64b332: *stop=0x8c64b332u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[2]);goto L8c64b334; /* cmp/ge r2,r13 */
L8c64b334: if((*sr&1u))goto L8c64b370;goto L8c64b336; /* bt 0x8c64b370 */
L8c64b336: *stop=0x8c64b338u;r[14]=0u;goto L8c64b37a; /* bra 0x8c64b37a */
L8c64b33a: *stop=0x8c64b33au;r[5]=r[13];goto L8c64b33c; /* mov r13,r5 */
L8c64b33c: target=0x8c64b1c0u;*pr=0x8c64b340u;*stop=0x8c64b33eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b340; /* bsr 0x8c64b1c0 */
L8c64b340: *stop=0x8c64b340u;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b342; /* mov.l @(0x28,r15),r2 */
L8c64b342: *stop=0x8c64b342u;r[4]=r[0]&65535u;goto L8c64b344; /* extu.w r0,r4 */
L8c64b344: *stop=0x8c64b344u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b3d8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b346; /* mov.l 0x8c64b3d8,r5 */
L8c64b346: *stop=0x8c64b346u;r[2]+=4294967295u;goto L8c64b348; /* add #-0x1,r2 */
L8c64b348: *stop=0x8c64b348u;*sr=(*sr&~1u)|(r[2]==r[14]);goto L8c64b34a; /* cmp/eq r2,r14 */
L8c64b34a: if(!(*sr&1u))goto L8c64b354;goto L8c64b34c; /* bf 0x8c64b354 */
L8c64b34c: *stop=0x8c64b34cu;*sr=(*sr&~1u)|(r[5]==r[4]);goto L8c64b34e; /* cmp/eq r5,r4 */
L8c64b34e: if((*sr&1u))goto L8c64b376;goto L8c64b350; /* bt 0x8c64b376 */
L8c64b350: *stop=0x8c64b352u;goto L8c64b370; /* bra 0x8c64b370 */
L8c64b354: *stop=0x8c64b354u;*sr=(*sr&~1u)|(r[13]==r[4]);goto L8c64b356; /* cmp/eq r13,r4 */
L8c64b356: if((*sr&1u))goto L8c64b370;goto L8c64b358; /* bt 0x8c64b370 */
L8c64b358: *stop=0x8c64b358u;*sr=(*sr&~1u)|(r[5]==r[4]);goto L8c64b35a; /* cmp/eq r5,r4 */
L8c64b35a: if((*sr&1u))goto L8c64b370;goto L8c64b35c; /* bt 0x8c64b370 */
L8c64b35c: *stop=0x8c64b35cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b3dcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b35e; /* mov.l 0x8c64b3dc,r5 */
L8c64b35e: *stop=0x8c64b35eu;*sr=(*sr&~1u)|(r[5]==r[4]);goto L8c64b360; /* cmp/eq r5,r4 */
L8c64b360: if((*sr&1u))goto L8c64b370;goto L8c64b362; /* bt 0x8c64b370 */
L8c64b362: *stop=0x8c64b362u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b3e0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b364; /* mov.l 0x8c64b3e0,r5 */
L8c64b364: *stop=0x8c64b364u;*sr=(*sr&~1u)|(r[5]==r[4]);goto L8c64b366; /* cmp/eq r5,r4 */
L8c64b366: if((*sr&1u))goto L8c64b370;goto L8c64b368; /* bt 0x8c64b370 */
L8c64b368: *stop=0x8c64b368u;if(!valid_access(r[11]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b36a; /* mov.l @(0x1c,r11),r3 */
L8c64b36a: *stop=0x8c64b36au;if(!valid_access(r[3]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b36c; /* mov.l @(0x30,r3),r2 */
L8c64b36c: *stop=0x8c64b36cu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]);goto L8c64b36e; /* cmp/ge r2,r4 */
L8c64b36e: if(!(*sr&1u))goto L8c64b376;goto L8c64b370; /* bf 0x8c64b376 */
L8c64b370: *stop=0x8c64b370u;r[0]=sx(m,bus,&io_status,0x8c64b3d0u,2);if(io_status)return io_status;goto L8c64b372; /* mov.w 0x8c64b3d0,r0 */
L8c64b372: *stop=0x8c64b374u;goto L8c64b382; /* bra 0x8c64b382 */
L8c64b376: *stop=0x8c64b376u;r[14]+=1u;goto L8c64b378; /* add #0x1,r14 */
L8c64b378: *stop=0x8c64b378u;r[13]=r[4];goto L8c64b37a; /* mov r4,r13 */
L8c64b37a: *stop=0x8c64b37au;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b37c; /* mov.l @(0x28,r15),r2 */
L8c64b37c: *stop=0x8c64b37cu;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[2]);goto L8c64b37e; /* cmp/ge r2,r14 */
L8c64b37e: if(!(*sr&1u))goto L8c64b33a;goto L8c64b380; /* bf 0x8c64b33a */
L8c64b380: *stop=0x8c64b380u;r[0]=0u;goto L8c64b382; /* mov #0x0,r0 */
L8c64b382: *stop=0x8c64b382u;r[15]+=44u;goto L8c64b384; /* add #0x2c,r15 */
L8c64b384: *stop=0x8c64b384u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b386; /* lds.l @r15+,PR */
L8c64b386: *stop=0x8c64b386u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64b388; /* mov.l @r15+,r11 */
L8c64b388: *stop=0x8c64b388u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64b38a; /* mov.l @r15+,r12 */
L8c64b38a: *stop=0x8c64b38au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b38c; /* mov.l @r15+,r13 */
L8c64b38c: *stop=0x8c64b38cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b38eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_chain_8c64b300(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_chain_run_8c64b300(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_chain_resume_8c64b31c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_chain_run_8c64b300(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b31cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_chain_resume_8c64b32c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_chain_run_8c64b300(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b32cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_chain_resume_8c64b340(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_chain_run_8c64b300(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b340u);}

SA_STAGE_EXPORT int sa_movie_filesystem_chain_8c64b274(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64b274;
L8c64b274: *stop=0x8c64b274u;r[3]=sx(m,bus,&io_status,0x8c64b2e0u,2);if(io_status)return io_status;goto L8c64b276; /* mov.w 0x8c64b2e0,r3 */
L8c64b276: *stop=0x8c64b276u;r[0]=r[5];goto L8c64b278; /* mov r5,r0 */
L8c64b278: *stop=0x8c64b278u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b2e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b27a; /* mov.l 0x8c64b2e4,r2 */
L8c64b27a: *stop=0x8c64b27au;r[0]<<=2;goto L8c64b27c; /* shll2 r0 */
L8c64b27c: *stop=0x8c64b27cu;*macl=r[3]*r[4];goto L8c64b27e; /* mul.l r3,r4 */
L8c64b27e: *stop=0x8c64b27eu;r[0]<<=2;goto L8c64b280; /* shll2 r0 */
L8c64b280: *stop=0x8c64b280u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c64b282; /* shll r0 */
L8c64b282: *stop=0x8c64b282u;r[4]=*macl;goto L8c64b284; /* sts MACL,r4 */
L8c64b284: *stop=0x8c64b284u;r[4]+=r[2];goto L8c64b286; /* add r2,r4 */
L8c64b286: *stop=0x8c64b286u;if(!valid_access(r[4]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x28u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b288; /* mov.l @(0x28,r4),r1 */
L8c64b288: *stop=0x8c64b288u;if(*pr!=entry_pr)return -3;*stop=0x8c64b28au;r[0]+=r[1];return 0; /* rts */
}
