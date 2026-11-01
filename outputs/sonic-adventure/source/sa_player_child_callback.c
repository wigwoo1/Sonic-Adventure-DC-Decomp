/* Original complete player child initializer and character selector bodies.
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
static int sa_player_child_callback_run_8c0b3a2e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c0b3a5au)goto L8c0b3a5a;if(resume_pc==0x8c0b3a78u)goto L8c0b3a78;goto L8c0b3a2e;
L8c0b3a2e: *stop=0x8c0b3a2eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0b3a30; /* mov.l r14,@-r15 */
L8c0b3a30: *stop=0x8c0b3a30u;r[14]=0u;goto L8c0b3a32; /* mov #0x0,r14 */
L8c0b3a32: *stop=0x8c0b3a32u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0b3a34; /* mov.l r13,@-r15 */
L8c0b3a34: *stop=0x8c0b3a34u;r[0]=r[14];goto L8c0b3a36; /* mov r14,r0 */
L8c0b3a36: *stop=0x8c0b3a36u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3ac0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3a38; /* mov.l 0x8c0b3ac0,r3 */
L8c0b3a38: *stop=0x8c0b3a38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0b3a3a; /* mov.l r12,@-r15 */
L8c0b3a3a: *stop=0x8c0b3a3au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0b3a3c; /* mov.l r11,@-r15 */
L8c0b3a3c: *stop=0x8c0b3a3cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c0b3a3e; /* mov.l r10,@-r15 */
L8c0b3a3e: *stop=0x8c0b3a3eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c0b3a40; /* mov.l r9,@-r15 */
L8c0b3a40: *stop=0x8c0b3a40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c0b3a42; /* mov.l r8,@-r15 */
L8c0b3a42: *stop=0x8c0b3a42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0b3a44; /* sts.l PR,@-r15 */
L8c0b3a44: *stop=0x8c0b3a44u;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c0b3a46; /* mov.l @(0x20,r4),r13 */
L8c0b3a46: *stop=0x8c0b3a46u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[3],4);if(io_status)return io_status;goto L8c0b3a48; /* mov.l r3,@(0x14,r4) */
L8c0b3a48: *stop=0x8c0b3a48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3ac8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3a4a; /* mov.l 0x8c0b3ac8,r3 */
L8c0b3a4a: *stop=0x8c0b3a4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3ac4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3a4c; /* mov.l 0x8c0b3ac4,r2 */
L8c0b3a4c: *stop=0x8c0b3a4cu;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[2],4);if(io_status)return io_status;goto L8c0b3a4e; /* mov.l r2,@(0x10,r4) */
L8c0b3a4e: *stop=0x8c0b3a4eu;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[3],4);if(io_status)return io_status;goto L8c0b3a50; /* mov.l r3,@(0x18,r4) */
L8c0b3a50: *stop=0x8c0b3a50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3accu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3a52; /* mov.l 0x8c0b3acc,r3 */
L8c0b3a52: *stop=0x8c0b3a52u;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[14],4);if(io_status)return io_status;goto L8c0b3a54; /* mov.l r14,@(0x10,r13) */
L8c0b3a54: *stop=0x8c0b3a54u;if(!valid_access(r[13]+1u,1u))return -3;wr(m,bus,&io_status,r[13]+0x1u,r[0],1);if(io_status)return io_status;goto L8c0b3a56; /* mov.b r0,@(0x1,r13) */
L8c0b3a56: target=r[3];*pr=0x8c0b3a5au;*stop=0x8c0b3a58u;if(!valid_access(r[13]+6u,2u))return -3;wr(m,bus,&io_status,r[13]+0x6u,r[0],2);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3a5a; /* jsr @r3 */
L8c0b3a5a: *stop=0x8c0b3a5au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0b3a5c; /* tst r0,r0 */
L8c0b3a5c: if(!(*sr&1u))goto L8c0b3aea;goto L8c0b3a5e; /* bf 0x8c0b3aea */
L8c0b3a5e: *stop=0x8c0b3a5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3ad0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0b3a60; /* mov.l 0x8c0b3ad0,r4 */
L8c0b3a60: *stop=0x8c0b3a60u;r[0]=r[14];goto L8c0b3a62; /* mov r14,r0 */
L8c0b3a62: *stop=0x8c0b3a62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3ad4u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c0b3a64; /* mov.l 0x8c0b3ad4,r8 */
L8c0b3a64: *stop=0x8c0b3a64u;r[14]=4294967256u;goto L8c0b3a66; /* mov #-0x28,r14 */
L8c0b3a66: *stop=0x8c0b3a66u;r[14]=r[14]&255u;goto L8c0b3a68; /* extu.b r14,r14 */
L8c0b3a68: *stop=0x8c0b3a68u;r[11]=r[4];goto L8c0b3a6a; /* mov r4,r11 */
L8c0b3a6a: *stop=0x8c0b3a6au;r[9]=15u;goto L8c0b3a6c; /* mov #0xf,r9 */
L8c0b3a6c: *stop=0x8c0b3a6cu;if(!valid_access(r[13]+3u,1u))return -3;wr(m,bus,&io_status,r[13]+0x3u,r[0],1);if(io_status)return io_status;goto L8c0b3a6e; /* mov.b r0,@(0x3,r13) */
L8c0b3a6e: *stop=0x8c0b3a6eu;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[0],4);if(io_status)return io_status;goto L8c0b3a70; /* mov.l r0,@(0x8,r13) */
L8c0b3a70: *stop=0x8c0b3a70u;r[12]=r[4];goto L8c0b3a72; /* mov r4,r12 */
L8c0b3a72: *stop=0x8c0b3a72u;r[10]=r[0];goto L8c0b3a74; /* mov r0,r10 */
L8c0b3a74: target=r[8];*pr=0x8c0b3a78u;*stop=0x8c0b3a76u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3a78; /* jsr @r8 */
L8c0b3a78: *stop=0x8c0b3a78u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3a7a; /* mov.l @r11,r3 */
L8c0b3a7a: *stop=0x8c0b3a7au;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c0b3a7c; /* cmp/eq r3,r0 */
L8c0b3a7c: if(!(*sr&1u))goto L8c0b3ae0;goto L8c0b3a7e; /* bf 0x8c0b3ae0 */
L8c0b3a7e: *stop=0x8c0b3a80u;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[12],4);if(io_status)return io_status;goto L8c0b3aea; /* bra 0x8c0b3aea */
L8c0b3ae0: *stop=0x8c0b3ae0u;r[10]+=3u;goto L8c0b3ae2; /* add #0x3,r10 */
L8c0b3ae2: *stop=0x8c0b3ae2u;*sr=(*sr&~1u)|(r[10]>=r[9]);goto L8c0b3ae4; /* cmp/hs r9,r10 */
L8c0b3ae4: *stop=0x8c0b3ae4u;r[12]+=r[14];goto L8c0b3ae6; /* add r14,r12 */
L8c0b3ae6: take=!(*sr&1u);*stop=0x8c0b3ae8u;r[11]+=r[14];if(take)goto L8c0b3a74;goto L8c0b3aea; /* bf/s 0x8c0b3a74 */
L8c0b3aea: *stop=0x8c0b3aeau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0b3aec; /* lds.l @r15+,PR */
L8c0b3aec: *stop=0x8c0b3aecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c0b3aee; /* mov.l @r15+,r8 */
L8c0b3aee: *stop=0x8c0b3aeeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c0b3af0; /* mov.l @r15+,r9 */
L8c0b3af0: *stop=0x8c0b3af0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c0b3af2; /* mov.l @r15+,r10 */
L8c0b3af2: *stop=0x8c0b3af2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c0b3af4; /* mov.l @r15+,r11 */
L8c0b3af4: *stop=0x8c0b3af4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c0b3af6; /* mov.l @r15+,r12 */
L8c0b3af6: *stop=0x8c0b3af6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0b3af8; /* mov.l @r15+,r13 */
L8c0b3af8: *stop=0x8c0b3af8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0b3afau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_child_callback_8c0b3a2e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_callback_run_8c0b3a2e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_child_callback_resume_8c0b3a5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_callback_run_8c0b3a2e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3a5au);}
SA_STAGE_EXPORT int sa_player_child_callback_resume_8c0b3a78(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_callback_run_8c0b3a2e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3a78u);}

SA_STAGE_EXPORT int sa_player_child_callback_8c07ce40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c07ce40;
L8c07ce40: *stop=0x8c07ce40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c07cf80u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c07ce42; /* mov.l 0x8c07cf80,r0 */
L8c07ce42: *stop=0x8c07ce42u;r[4]=r[4]&255u;goto L8c07ce44; /* extu.b r4,r4 */
L8c07ce44: *stop=0x8c07ce44u;r[4]<<=2;goto L8c07ce46; /* shll2 r4 */
L8c07ce46: *stop=0x8c07ce46u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c07ce48; /* mov.l @(r0,r4),r4 */
L8c07ce48: *stop=0x8c07ce48u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c07ce4a; /* tst r4,r4 */
L8c07ce4a: if(!(*sr&1u))goto L8c07ce52;goto L8c07ce4c; /* bf 0x8c07ce52 */
L8c07ce4c: *stop=0x8c07ce4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07cf84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07ce4e; /* mov.l 0x8c07cf84,r3 */
L8c07ce4e: target=r[3];*stop=0x8c07ce50u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c07ce52: *stop=0x8c07ce52u;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c07ce54; /* mov.b @(0x9,r4),r0 */
L8c07ce54: *stop=0x8c07ce54u;if(*pr!=entry_pr)return -3;*stop=0x8c07ce56u;r[0]=r[0]&255u;return 0; /* rts */
}
