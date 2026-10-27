/* Original command7 gateway, in-memory removal, FAT update and dirty marker.
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
static int sa_movie_filesystem_delete_run_8c649d20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c649d5au)goto L8c649d5a;if(resume_pc==0x8c649d64u)goto L8c649d64;if(resume_pc==0x8c649d6eu)goto L8c649d6e;if(resume_pc==0x8c649d8au)goto L8c649d8a;if(resume_pc==0x8c649d9cu)goto L8c649d9c;if(resume_pc==0x8c649dacu)goto L8c649dac;if(resume_pc==0x8c649db6u)goto L8c649db6;goto L8c649d20;
L8c649d20: *stop=0x8c649d20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c649d22; /* mov.l r14,@-r15 */
L8c649d22: *stop=0x8c649d22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c649d24; /* mov.l r13,@-r15 */
L8c649d24: *stop=0x8c649d24u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c649d26; /* sts.l PR,@-r15 */
L8c649d26: *stop=0x8c649d26u;r[15]+=4294967288u;goto L8c649d28; /* add #-0x8,r15 */
L8c649d28: *stop=0x8c649d28u;r[13]=sx(m,bus,&io_status,0x8c649dd8u,2);if(io_status)return io_status;goto L8c649d2a; /* mov.w 0x8c649dd8,r13 */
L8c649d2a: *stop=0x8c649d2au;r[14]=r[4];goto L8c649d2c; /* mov r4,r14 */
L8c649d2c: *stop=0x8c649d2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649de0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649d2e; /* mov.l 0x8c649de0,r3 */
L8c649d2e: *stop=0x8c649d2eu;r[2]=1u;goto L8c649d30; /* mov #0x1,r2 */
L8c649d30: *stop=0x8c649d30u;*macl=r[13]*r[14];goto L8c649d32; /* mul.l r13,r14 */
L8c649d32: *stop=0x8c649d32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649de4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c649d34; /* mov.l 0x8c649de4,r0 */
L8c649d34: *stop=0x8c649d34u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c649d36; /* mov.l r5,@r15 */
L8c649d36: *stop=0x8c649d36u;r[13]=*macl;goto L8c649d38; /* sts MACL,r13 */
L8c649d38: *stop=0x8c649d38u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[13],4);if(io_status)return io_status;goto L8c649d3a; /* mov.l r13,@(0x4,r15) */
L8c649d3a: *stop=0x8c649d3au;r[13]+=r[3];goto L8c649d3c; /* add r3,r13 */
L8c649d3c: *stop=0x8c649d3cu;if(!valid_access(r[13]+48u,4u))return -3;wr(m,bus,&io_status,r[13]+0x30u,r[2],4);if(io_status)return io_status;goto L8c649d3e; /* mov.l r2,@(0x30,r13) */
L8c649d3e: *stop=0x8c649d3eu;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c649d40; /* mov.l @(0x4,r13),r1 */
L8c649d40: *stop=0x8c649d40u;r[2]=r[1];goto L8c649d42; /* mov r1,r2 */
L8c649d42: *stop=0x8c649d42u;r[1]<<=2;goto L8c649d44; /* shll2 r1 */
L8c649d44: *stop=0x8c649d44u;r[1]+=r[2];goto L8c649d46; /* add r2,r1 */
L8c649d46: *stop=0x8c649d46u;r[1]<<=2;goto L8c649d48; /* shll2 r1 */
L8c649d48: *stop=0x8c649d48u;r[1]<<=2;goto L8c649d4a; /* shll2 r1 */
L8c649d4a: *stop=0x8c649d4au;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c649d4c; /* mov.l @(r0,r1),r0 */
L8c649d4c: *stop=0x8c649d4cu;if(!valid_access(r[13]+52u,4u))return -3;wr(m,bus,&io_status,r[13]+0x34u,r[0],4);if(io_status)return io_status;goto L8c649d4e; /* mov.l r0,@(0x34,r13) */
L8c649d4e: *stop=0x8c649d4eu;if(!valid_access(r[13]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x2u,1);if(io_status)return io_status;goto L8c649d50; /* mov.b @(0x2,r13),r0 */
L8c649d50: *stop=0x8c649d50u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649d52; /* tst r0,r0 */
L8c649d52: if((*sr&1u))goto L8c649d7e;goto L8c649d54; /* bt 0x8c649d7e */
L8c649d54: *stop=0x8c649d54u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649de8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649d56; /* mov.l 0x8c649de8,r3 */
L8c649d56: target=r[3];*pr=0x8c649d5au;*stop=0x8c649d58u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649d5a; /* jsr @r3 */
L8c649d5a: *stop=0x8c649d5au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649d5c; /* tst r0,r0 */
L8c649d5c: if(!(*sr&1u))goto L8c649d68;goto L8c649d5e; /* bf 0x8c649d68 */
L8c649d5e: *stop=0x8c649d5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649decu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649d60; /* mov.l 0x8c649dec,r3 */
L8c649d60: target=r[3];*pr=0x8c649d64u;*stop=0x8c649d62u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649d64; /* jsr @r3 */
L8c649d64: *stop=0x8c649d66u;goto L8c649d7e; /* bra 0x8c649d7e */
L8c649d68: *stop=0x8c649d68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649df0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649d6a; /* mov.l 0x8c649df0,r2 */
L8c649d6a: target=r[2];*pr=0x8c649d6eu;*stop=0x8c649d6cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649d6e; /* jsr @r2 */
L8c649d6e: *stop=0x8c649d6eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649d70; /* tst r0,r0 */
L8c649d70: if((*sr&1u))goto L8c649d7e;goto L8c649d72; /* bt 0x8c649d7e */
L8c649d72: *stop=0x8c649d72u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649d74; /* mov.l @(0x4,r15),r2 */
L8c649d74: *stop=0x8c649d74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649de0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649d76; /* mov.l 0x8c649de0,r3 */
L8c649d76: *stop=0x8c649d76u;r[2]+=r[3];goto L8c649d78; /* add r3,r2 */
L8c649d78: *stop=0x8c649d78u;if(!valid_access(r[2]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x8u,1);if(io_status)return io_status;goto L8c649d7a; /* mov.b @(0x8,r2),r0 */
L8c649d7a: *stop=0x8c649d7au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649d7c; /* tst r0,r0 */
L8c649d7c: if(!(*sr&1u))goto L8c649d84;goto L8c649d7e; /* bf 0x8c649d84 */
L8c649d7e: *stop=0x8c649d7eu;r[0]=sx(m,bus,&io_status,0x8c649ddau,2);if(io_status)return io_status;goto L8c649d80; /* mov.w 0x8c649dda,r0 */
L8c649d80: *stop=0x8c649d82u;goto L8c649dce; /* bra 0x8c649dce */
L8c649d84: *stop=0x8c649d84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649df4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649d86; /* mov.l 0x8c649df4,r3 */
L8c649d86: target=r[3];*pr=0x8c649d8au;*stop=0x8c649d88u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649d8a; /* jsr @r3 */
L8c649d8a: *stop=0x8c649d8au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649d8c; /* cmp/pz r0 */
L8c649d8c: if((*sr&1u))goto L8c649d94;goto L8c649d8e; /* bt 0x8c649d94 */
L8c649d8e: *stop=0x8c649d8eu;r[0]=sx(m,bus,&io_status,0x8c649ddcu,2);if(io_status)return io_status;goto L8c649d90; /* mov.w 0x8c649ddc,r0 */
L8c649d90: *stop=0x8c649d92u;goto L8c649dce; /* bra 0x8c649dce */
L8c649d94: *stop=0x8c649d94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649df8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649d96; /* mov.l 0x8c649df8,r3 */
L8c649d96: *stop=0x8c649d96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c649d98; /* mov.l @r15,r5 */
L8c649d98: target=r[3];*pr=0x8c649d9cu;*stop=0x8c649d9au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649d9c; /* jsr @r3 */
L8c649d9c: *stop=0x8c649d9cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649d9e; /* cmp/pz r0 */
L8c649d9e: if((*sr&1u))goto L8c649da6;goto L8c649da0; /* bt 0x8c649da6 */
L8c649da0: *stop=0x8c649da0u;r[0]=sx(m,bus,&io_status,0x8c649ddeu,2);if(io_status)return io_status;goto L8c649da2; /* mov.w 0x8c649dde,r0 */
L8c649da2: *stop=0x8c649da4u;goto L8c649dce; /* bra 0x8c649dce */
L8c649da6: *stop=0x8c649da6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c649da8; /* mov.l @r15,r5 */
L8c649da8: target=0x8c649c68u;*pr=0x8c649dacu;*stop=0x8c649daau;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649dac; /* bsr 0x8c649c68 */
L8c649dac: *stop=0x8c649dacu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649dae; /* cmp/pz r0 */
L8c649dae: take=!(*sr&1u);*stop=0x8c649db0u;r[4]=r[0];if(take)goto L8c649dbc;goto L8c649db2; /* bf/s 0x8c649dbc */
L8c649db2: target=0x8c64ab36u;*pr=0x8c649db6u;*stop=0x8c649db4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649db6; /* bsr 0x8c64ab36 */
L8c649db6: *stop=0x8c649db6u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c649db8; /* cmp/pz r0 */
L8c649db8: take=(*sr&1u);*stop=0x8c649dbau;r[4]=r[0];if(take)goto L8c649dc0;goto L8c649dbc; /* bt/s 0x8c649dc0 */
L8c649dbc: *stop=0x8c649dbeu;r[0]=r[4];goto L8c649dce; /* bra 0x8c649dce */
L8c649dc0: *stop=0x8c649dc0u;r[4]=r[14];goto L8c649dc2; /* mov r14,r4 */
L8c649dc2: *stop=0x8c649dc2u;r[15]+=8u;goto L8c649dc4; /* add #0x8,r15 */
L8c649dc4: *stop=0x8c649dc4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c649dc6; /* lds.l @r15+,PR */
L8c649dc6: *stop=0x8c649dc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649dfcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649dc8; /* mov.l 0x8c649dfc,r2 */
L8c649dc8: *stop=0x8c649dc8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c649dca; /* mov.l @r15+,r13 */
L8c649dca: target=r[2];*stop=0x8c649dccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c649dce: *stop=0x8c649dceu;r[15]+=8u;goto L8c649dd0; /* add #0x8,r15 */
L8c649dd0: *stop=0x8c649dd0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c649dd2; /* lds.l @r15+,PR */
L8c649dd2: *stop=0x8c649dd2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c649dd4; /* mov.l @r15+,r13 */
L8c649dd4: *stop=0x8c649dd4u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c649dd6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_8c649d20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649d20(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649d5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649d20(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649d5au);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649d64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649d20(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649d64u);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649d6e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649d20(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649d6eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649d8a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649d20(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649d8au);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649d9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649d20(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649d9cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649dac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649d20(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649dacu);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649db6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649d20(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649db6u);}

static int sa_movie_filesystem_delete_run_8c649c68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c649c86u)goto L8c649c86;if(resume_pc==0x8c649c9au)goto L8c649c9a;if(resume_pc==0x8c649ca4u)goto L8c649ca4;if(resume_pc==0x8c649cb8u)goto L8c649cb8;if(resume_pc==0x8c649cc4u)goto L8c649cc4;if(resume_pc==0x8c649cd8u)goto L8c649cd8;if(resume_pc==0x8c649ce0u)goto L8c649ce0;goto L8c649c68;
L8c649c68: *stop=0x8c649c68u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c649c6a; /* mov.l r14,@-r15 */
L8c649c6a: *stop=0x8c649c6au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c649c6c; /* mov.l r13,@-r15 */
L8c649c6c: *stop=0x8c649c6cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c649c6e; /* mov.l r12,@-r15 */
L8c649c6e: *stop=0x8c649c6eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c649c70; /* mov.l r11,@-r15 */
L8c649c70: *stop=0x8c649c70u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c649c72; /* mov.l r10,@-r15 */
L8c649c72: *stop=0x8c649c72u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c649c74; /* mov.l r9,@-r15 */
L8c649c74: *stop=0x8c649c74u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c649c76; /* sts.l PR,@-r15 */
L8c649c76: *stop=0x8c649c76u;r[15]+=4294967244u;goto L8c649c78; /* add #-0x34,r15 */
L8c649c78: *stop=0x8c649c78u;r[6]=r[15];goto L8c649c7a; /* mov r15,r6 */
L8c649c7a: *stop=0x8c649c7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c649d00u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649c7c; /* mov.l 0x8c649d00,r3 */
L8c649c7c: *stop=0x8c649c7cu;r[14]=r[4];goto L8c649c7e; /* mov r4,r14 */
L8c649c7e: *stop=0x8c649c7eu;r[6]+=8u;goto L8c649c80; /* add #0x8,r6 */
L8c649c80: *stop=0x8c649c80u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c649c82; /* mov.l r5,@r15 */
L8c649c82: target=r[3];*pr=0x8c649c86u;*stop=0x8c649c84u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649c86; /* jsr @r3 */
L8c649c86: *stop=0x8c649c86u;r[11]=r[0];goto L8c649c88; /* mov r0,r11 */
L8c649c88: *stop=0x8c649c88u;*sr=(*sr&~1u)|((int32_t)r[11]>=0);goto L8c649c8a; /* cmp/pz r11 */
L8c649c8a: if((*sr&1u))goto L8c649c92;goto L8c649c8c; /* bt 0x8c649c92 */
L8c649c8c: *stop=0x8c649c8cu;r[0]=sx(m,bus,&io_status,0x8c649cf4u,2);if(io_status)return io_status;goto L8c649c8e; /* mov.w 0x8c649cf4,r0 */
L8c649c8e: *stop=0x8c649c90u;goto L8c649ce2; /* bra 0x8c649ce2 */
L8c649c92: *stop=0x8c649c92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649d04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649c94; /* mov.l 0x8c649d04,r3 */
L8c649c94: *stop=0x8c649c94u;r[5]=r[11];goto L8c649c96; /* mov r11,r5 */
L8c649c96: target=r[3];*pr=0x8c649c9au;*stop=0x8c649c98u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649c9a; /* jsr @r3 */
L8c649c9a: *stop=0x8c649c9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c649d08u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649c9c; /* mov.l 0x8c649d08,r3 */
L8c649c9c: *stop=0x8c649c9cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c649c9e; /* mov.l r0,@(0x4,r15) */
L8c649c9e: *stop=0x8c649c9eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c649ca0; /* mov.l @r15,r5 */
L8c649ca0: target=r[3];*pr=0x8c649ca4u;*stop=0x8c649ca2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649ca4; /* jsr @r3 */
L8c649ca4: *stop=0x8c649ca4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649ca6; /* tst r0,r0 */
L8c649ca6: if(!(*sr&1u))goto L8c649cce;goto L8c649ca8; /* bf 0x8c649cce */
L8c649ca8: *stop=0x8c649ca8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649d0cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c649caa; /* mov.l 0x8c649d0c,r10 */
L8c649caa: *stop=0x8c649caau;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c649cac; /* mov.l @(0x14,r15),r13 */
L8c649cac: *stop=0x8c649caeu;r[12]=0u;goto L8c649cc8; /* bra 0x8c649cc8 */
L8c649cb0: *stop=0x8c649cb0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649d10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649cb2; /* mov.l 0x8c649d10,r2 */
L8c649cb2: *stop=0x8c649cb2u;r[5]=r[13];goto L8c649cb4; /* mov r13,r5 */
L8c649cb4: target=r[2];*pr=0x8c649cb8u;*stop=0x8c649cb6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649cb8; /* jsr @r2 */
L8c649cb8: *stop=0x8c649cb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649d14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649cba; /* mov.l 0x8c649d14,r3 */
L8c649cba: *stop=0x8c649cbau;r[5]=r[13];goto L8c649cbc; /* mov r13,r5 */
L8c649cbc: *stop=0x8c649cbcu;r[9]=r[0]&65535u;goto L8c649cbe; /* extu.w r0,r9 */
L8c649cbe: *stop=0x8c649cbeu;r[6]=r[10];goto L8c649cc0; /* mov r10,r6 */
L8c649cc0: target=r[3];*pr=0x8c649cc4u;*stop=0x8c649cc2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649cc4; /* jsr @r3 */
L8c649cc4: *stop=0x8c649cc4u;r[12]+=1u;goto L8c649cc6; /* add #0x1,r12 */
L8c649cc6: *stop=0x8c649cc6u;r[13]=r[9];goto L8c649cc8; /* mov r9,r13 */
L8c649cc8: *stop=0x8c649cc8u;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649cca; /* mov.l @(0x30,r15),r2 */
L8c649cca: *stop=0x8c649ccau;*sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[2]);goto L8c649ccc; /* cmp/ge r2,r12 */
L8c649ccc: if(!(*sr&1u))goto L8c649cb0;goto L8c649cce; /* bf 0x8c649cb0 */
L8c649cce: *stop=0x8c649cceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649d18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649cd0; /* mov.l 0x8c649d18,r3 */
L8c649cd0: *stop=0x8c649cd0u;r[5]=0u;goto L8c649cd2; /* mov #0x0,r5 */
L8c649cd2: *stop=0x8c649cd2u;r[6]=32u;goto L8c649cd4; /* mov #0x20,r6 */
L8c649cd4: target=r[3];*pr=0x8c649cd8u;*stop=0x8c649cd6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649cd8; /* jsr @r3 */
L8c649cd8: *stop=0x8c649cd8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649d1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649cda; /* mov.l 0x8c649d1c,r2 */
L8c649cda: *stop=0x8c649cdau;r[5]=r[11];goto L8c649cdc; /* mov r11,r5 */
L8c649cdc: target=r[2];*pr=0x8c649ce0u;*stop=0x8c649cdeu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649ce0; /* jsr @r2 */
L8c649ce0: *stop=0x8c649ce0u;r[0]=0u;goto L8c649ce2; /* mov #0x0,r0 */
L8c649ce2: *stop=0x8c649ce2u;r[15]+=52u;goto L8c649ce4; /* add #0x34,r15 */
L8c649ce4: *stop=0x8c649ce4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c649ce6; /* lds.l @r15+,PR */
L8c649ce6: *stop=0x8c649ce6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c649ce8; /* mov.l @r15+,r9 */
L8c649ce8: *stop=0x8c649ce8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c649cea; /* mov.l @r15+,r10 */
L8c649cea: *stop=0x8c649ceau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c649cec; /* mov.l @r15+,r11 */
L8c649cec: *stop=0x8c649cecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c649cee; /* mov.l @r15+,r12 */
L8c649cee: *stop=0x8c649ceeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c649cf0; /* mov.l @r15+,r13 */
L8c649cf0: *stop=0x8c649cf0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c649cf2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_8c649c68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649c68(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649c86(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649c68(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649c86u);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649c9a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649c68(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649c9au);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649ca4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649c68(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649ca4u);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649cb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649c68(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649cb8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649cc4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649c68(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649cc4u);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649cd8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649c68(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649cd8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_delete_resume_8c649ce0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_delete_run_8c649c68(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649ce0u);}

SA_STAGE_EXPORT int sa_movie_filesystem_delete_8c64b18e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64b18e;
L8c64b18e: *stop=0x8c64b18eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b190; /* mov.l r14,@-r15 */
L8c64b190: *stop=0x8c64b190u;r[15]+=4294967292u;goto L8c64b192; /* add #-0x4,r15 */
L8c64b192: *stop=0x8c64b192u;r[14]=sx(m,bus,&io_status,0x8c64b1e0u,2);if(io_status)return io_status;goto L8c64b194; /* mov.w 0x8c64b1e0,r14 */
L8c64b194: *stop=0x8c64b194u;r[0]=r[5];goto L8c64b196; /* mov r5,r0 */
L8c64b196: *stop=0x8c64b196u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b1e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b198; /* mov.l 0x8c64b1e8,r3 */
L8c64b198: *stop=0x8c64b198u;*macl=r[14]*r[4];goto L8c64b19a; /* mul.l r14,r4 */
L8c64b19a: *stop=0x8c64b19au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64b19c; /* mov.l r5,@r15 */
L8c64b19c: *stop=0x8c64b19cu;r[14]=*macl;goto L8c64b19e; /* sts MACL,r14 */
L8c64b19e: *stop=0x8c64b19eu;r[14]+=r[3];goto L8c64b1a0; /* add r3,r14 */
L8c64b1a0: *stop=0x8c64b1a0u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b1a2; /* mov.l @(0x1c,r14),r2 */
L8c64b1a2: *stop=0x8c64b1a2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b1a4; /* mov.l @r2,r1 */
L8c64b1a4: *stop=0x8c64b1a4u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[1]);goto L8c64b1a6; /* cmp/gt r1,r0 */
L8c64b1a6: if((*sr&1u))goto L8c64b1ba;goto L8c64b1a8; /* bt 0x8c64b1ba */
L8c64b1a8: *stop=0x8c64b1a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b1ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b1aa; /* mov.l 0x8c64b1ec,r2 */
L8c64b1aa: *stop=0x8c64b1aau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b1ac; /* mov.l @r15,r5 */
L8c64b1ac: *stop=0x8c64b1acu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b1ae; /* mov.l @r2,r3 */
L8c64b1ae: *stop=0x8c64b1aeu;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64b1b0; /* mov.l @(0x24,r14),r4 */
L8c64b1b0: *stop=0x8c64b1b0u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c64b1b2; /* shll r5 */
L8c64b1b2: *stop=0x8c64b1b2u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b1b4; /* mov.l @(0x10,r3),r1 */
L8c64b1b4: *stop=0x8c64b1b4u;r[15]+=4u;goto L8c64b1b6; /* add #0x4,r15 */
L8c64b1b6: target=r[1];*stop=0x8c64b1b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r1 */
L8c64b1ba: *stop=0x8c64b1bau;r[15]+=4u;goto L8c64b1bc; /* add #0x4,r15 */
L8c64b1bc: *stop=0x8c64b1bcu;if(*pr!=entry_pr)return -3;*stop=0x8c64b1beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_filesystem_delete_8c64b28c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64b28c;
L8c64b28c: *stop=0x8c64b28cu;r[3]=sx(m,bus,&io_status,0x8c64b2e0u,2);if(io_status)return io_status;goto L8c64b28e; /* mov.w 0x8c64b2e0,r3 */
L8c64b28e: *stop=0x8c64b28eu;r[1]=4294967292u;goto L8c64b290; /* mov #-0x4,r1 */
L8c64b290: *stop=0x8c64b290u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b2e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b292; /* mov.l 0x8c64b2e4,r2 */
L8c64b292: *stop=0x8c64b292u;{uint32_t shift=r[1];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}}goto L8c64b294; /* shad r1,r5 */
L8c64b294: *stop=0x8c64b294u;*macl=r[3]*r[4];goto L8c64b296; /* mul.l r3,r4 */
L8c64b296: *stop=0x8c64b296u;r[1]=1u;goto L8c64b298; /* mov #0x1,r1 */
L8c64b298: *stop=0x8c64b298u;r[4]=*macl;goto L8c64b29a; /* sts MACL,r4 */
L8c64b29a: *stop=0x8c64b29au;r[4]+=r[2];goto L8c64b29c; /* add r2,r4 */
L8c64b29c: *stop=0x8c64b29cu;r[0]=r[4];goto L8c64b29e; /* mov r4,r0 */
L8c64b29e: *stop=0x8c64b29eu;r[0]+=72u;goto L8c64b2a0; /* add #0x48,r0 */
L8c64b2a0: *stop=0x8c64b2a0u;r[5]+=r[0];goto L8c64b2a2; /* add r0,r5 */
L8c64b2a2: *stop=0x8c64b2a2u;if(*pr!=entry_pr)return -3;*stop=0x8c64b2a4u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[1],1);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_filesystem_delete_8c64b2dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64b2dc;
L8c64b2dc: *stop=0x8c64b2dcu;if(*pr!=entry_pr)return -3;*stop=0x8c64b2deu;r[0]=0u;return 0; /* rts */
}
