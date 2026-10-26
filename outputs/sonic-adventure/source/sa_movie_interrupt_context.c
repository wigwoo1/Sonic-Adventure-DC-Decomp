/* Original vector fragments and interrupt context save/restore; explicit SSR/SPC state.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; internal status2 propagates an actual RTE unwind. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_integer_state.h"
#include "sh4_exception_state.h"
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
 if(n==4u && (a==0xff000020u||a==0xff000024u||a==0xff000028u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f6800u && (a&0x1fffffffu)<0x005f7d00u)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_interrupt_context_8c65e80c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t *vbr,struct sa_sh4_exception_state *exception){
 if(!exception)return -2;
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e80c;
L8c65e80c: *stop=0x8c65e80cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c65e80e; /* mov.l @r15+,r1 */
L8c65e80e: *stop=0x8c65e80eu;{uint32_t value;status=sa_sh4_exception_stc(exception,SA_EXCEPTION_SSR,*sr,&value,0u);if(status)return status;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e810; /* stc.l SSR,@-r15 */
L8c65e810: *stop=0x8c65e810u;{uint32_t value;status=sa_sh4_exception_stc(exception,SA_EXCEPTION_SPC,*sr,&value,0u);if(status)return status;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e812; /* stc.l SPC,@-r15 */
L8c65e812: *stop=0x8c65e812u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65e814; /* sts.l PR,@-r15 */
L8c65e814: *stop=0x8c65e814u;{uint32_t value;value=*gbr;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e816; /* stc.l GBR,@-r15 */
L8c65e816: *stop=0x8c65e816u;{uint32_t value;status=sa_sh4_stc_vbr(*vbr,*sr,&value,0u);if(status)return status;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e818; /* stc.l VBR,@-r15 */
L8c65e818: *stop=0x8c65e818u;{uint32_t value;value=integer->mach;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e81a; /* sts.l MACH,@-r15 */
L8c65e81a: *stop=0x8c65e81au;{uint32_t value;value=*macl;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e81c; /* sts.l MACL,@-r15 */
L8c65e81c: *stop=0x8c65e81cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65e81e; /* mov.l r14,@-r15 */
L8c65e81e: *stop=0x8c65e81eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65e820; /* mov.l r13,@-r15 */
L8c65e820: *stop=0x8c65e820u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65e822; /* mov.l r12,@-r15 */
L8c65e822: *stop=0x8c65e822u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65e824; /* mov.l r11,@-r15 */
L8c65e824: *stop=0x8c65e824u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65e826; /* mov.l r10,@-r15 */
L8c65e826: *stop=0x8c65e826u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65e828; /* mov.l r9,@-r15 */
L8c65e828: *stop=0x8c65e828u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65e82a; /* mov.l r8,@-r15 */
L8c65e82a: *stop=0x8c65e82au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c65e82c; /* mov.l r7,@-r15 */
L8c65e82c: *stop=0x8c65e82cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c65e82e; /* mov.l r6,@-r15 */
L8c65e82e: *stop=0x8c65e82eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c65e830; /* mov.l r5,@-r15 */
L8c65e830: *stop=0x8c65e830u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c65e832; /* mov.l r4,@-r15 */
L8c65e832: *stop=0x8c65e832u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65e834; /* mov.l r3,@-r15 */
L8c65e834: *stop=0x8c65e834u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c65e836; /* mov.l r2,@-r15 */
L8c65e836: *stop=0x8c65e836u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c65e838; /* mov.l r1,@-r15 */
L8c65e838: *stop=0x8c65e838u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c65e83a; /* mov.l r0,@-r15 */
L8c65e83a: *stop=0x8c65e83au;{uint32_t value;status=sa_sh4_stc_sr(*sr,&value,0u);if(status)return status;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e83c; /* stc.l SR,@-r15 */
L8c65e83c: *stop=0x8c65e83cu;{uint32_t value;value=*fpul;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e83e; /* sts.l FPUL,@-r15 */
L8c65e83e: *stop=0x8c65e83eu;{uint32_t value;value=f->fpscr;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e840; /* sts.l FPSCR,@-r15 */
L8c65e840: *stop=0x8c65e840u;r[0]=4u;goto L8c65e842; /* mov #0x4,r0 */
L8c65e842: *stop=0x8c65e842u;r[0]<<=16;goto L8c65e844; /* shll16 r0 */
L8c65e844: *stop=0x8c65e844u;r[0]+=1u;goto L8c65e846; /* add #0x1,r0 */
L8c65e846: *stop=0x8c65e846u;f->fpscr=r[0]&0x3fffffu;goto L8c65e848; /* lds r0,fpscr */
L8c65e848: *stop=0x8c65e848u;r[0]^=r[0];goto L8c65e84a; /* xor r0,r0 */
L8c65e84a: *stop=0x8c65e84au;*fpul=r[0];goto L8c65e84c; /* lds r0,FPUL */
L8c65e84c: *stop=0x8c65e84cu;if(*sr&0x8000u)return -4;*stop=0x8c65e84cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c65e84e; /* fmov.s fr15,@-r15 */
L8c65e84e: *stop=0x8c65e84eu;if(*sr&0x8000u)return -4;*stop=0x8c65e84eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c65e850; /* fmov.s fr14,@-r15 */
L8c65e850: *stop=0x8c65e850u;if(*sr&0x8000u)return -4;*stop=0x8c65e850u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c65e852; /* fmov.s fr13,@-r15 */
L8c65e852: *stop=0x8c65e852u;if(*sr&0x8000u)return -4;*stop=0x8c65e852u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c65e854; /* fmov.s fr12,@-r15 */
L8c65e854: *stop=0x8c65e854u;if(*sr&0x8000u)return -4;*stop=0x8c65e854u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c65e856; /* fmov.s fr11,@-r15 */
L8c65e856: *stop=0x8c65e856u;if(*sr&0x8000u)return -4;*stop=0x8c65e856u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c65e858; /* fmov.s fr10,@-r15 */
L8c65e858: *stop=0x8c65e858u;if(*sr&0x8000u)return -4;*stop=0x8c65e858u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c65e85a; /* fmov.s fr9,@-r15 */
L8c65e85a: *stop=0x8c65e85au;if(*sr&0x8000u)return -4;*stop=0x8c65e85au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c65e85c; /* fmov.s fr8,@-r15 */
L8c65e85c: *stop=0x8c65e85cu;if(*sr&0x8000u)return -4;*stop=0x8c65e85cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c65e85e; /* fmov.s fr7,@-r15 */
L8c65e85e: *stop=0x8c65e85eu;if(*sr&0x8000u)return -4;*stop=0x8c65e85eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c65e860; /* fmov.s fr6,@-r15 */
L8c65e860: *stop=0x8c65e860u;if(*sr&0x8000u)return -4;*stop=0x8c65e860u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c65e862; /* fmov.s fr5,@-r15 */
L8c65e862: *stop=0x8c65e862u;if(*sr&0x8000u)return -4;*stop=0x8c65e862u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c65e864; /* fmov.s fr4,@-r15 */
L8c65e864: *stop=0x8c65e864u;if(*sr&0x8000u)return -4;*stop=0x8c65e864u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c65e866; /* fmov.s fr3,@-r15 */
L8c65e866: *stop=0x8c65e866u;if(*sr&0x8000u)return -4;*stop=0x8c65e866u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c65e868; /* fmov.s fr2,@-r15 */
L8c65e868: *stop=0x8c65e868u;if(*sr&0x8000u)return -4;*stop=0x8c65e868u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c65e86a; /* fmov.s fr1,@-r15 */
L8c65e86a: *stop=0x8c65e86au;if(*sr&0x8000u)return -4;*stop=0x8c65e86au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c65e86c; /* fmov.s fr0,@-r15 */
L8c65e86c: *stop=0x8c65e86cu;if(*sr&0x8000u)return -4;*stop=0x8c65e86cu;f->fpscr^=0x200000u;goto L8c65e86e; /* frchg */
L8c65e86e: *stop=0x8c65e86eu;if(*sr&0x8000u)return -4;*stop=0x8c65e86eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c65e870; /* fmov.s fr15,@-r15 */
L8c65e870: *stop=0x8c65e870u;if(*sr&0x8000u)return -4;*stop=0x8c65e870u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c65e872; /* fmov.s fr14,@-r15 */
L8c65e872: *stop=0x8c65e872u;if(*sr&0x8000u)return -4;*stop=0x8c65e872u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c65e874; /* fmov.s fr13,@-r15 */
L8c65e874: *stop=0x8c65e874u;if(*sr&0x8000u)return -4;*stop=0x8c65e874u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c65e876; /* fmov.s fr12,@-r15 */
L8c65e876: *stop=0x8c65e876u;if(*sr&0x8000u)return -4;*stop=0x8c65e876u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c65e878; /* fmov.s fr11,@-r15 */
L8c65e878: *stop=0x8c65e878u;if(*sr&0x8000u)return -4;*stop=0x8c65e878u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c65e87a; /* fmov.s fr10,@-r15 */
L8c65e87a: *stop=0x8c65e87au;if(*sr&0x8000u)return -4;*stop=0x8c65e87au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c65e87c; /* fmov.s fr9,@-r15 */
L8c65e87c: *stop=0x8c65e87cu;if(*sr&0x8000u)return -4;*stop=0x8c65e87cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c65e87e; /* fmov.s fr8,@-r15 */
L8c65e87e: *stop=0x8c65e87eu;if(*sr&0x8000u)return -4;*stop=0x8c65e87eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c65e880; /* fmov.s fr7,@-r15 */
L8c65e880: *stop=0x8c65e880u;if(*sr&0x8000u)return -4;*stop=0x8c65e880u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c65e882; /* fmov.s fr6,@-r15 */
L8c65e882: *stop=0x8c65e882u;if(*sr&0x8000u)return -4;*stop=0x8c65e882u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c65e884; /* fmov.s fr5,@-r15 */
L8c65e884: *stop=0x8c65e884u;if(*sr&0x8000u)return -4;*stop=0x8c65e884u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c65e886; /* fmov.s fr4,@-r15 */
L8c65e886: *stop=0x8c65e886u;if(*sr&0x8000u)return -4;*stop=0x8c65e886u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c65e888; /* fmov.s fr3,@-r15 */
L8c65e888: *stop=0x8c65e888u;if(*sr&0x8000u)return -4;*stop=0x8c65e888u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c65e88a; /* fmov.s fr2,@-r15 */
L8c65e88a: *stop=0x8c65e88au;if(*sr&0x8000u)return -4;*stop=0x8c65e88au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c65e88c; /* fmov.s fr1,@-r15 */
L8c65e88c: *stop=0x8c65e88cu;if(*sr&0x8000u)return -4;*stop=0x8c65e88cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c65e88e; /* fmov.s fr0,@-r15 */
L8c65e88e: *stop=0x8c65e88eu;if(*sr&0x8000u)return -4;*stop=0x8c65e88eu;f->fpscr^=0x200000u;goto L8c65e890; /* frchg */
L8c65e890: *stop=0x8c65e890u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65e892; /* stc SR,r0 */
L8c65e892: *stop=0x8c65e892u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65e94cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65e894; /* mov.l 0x8c65e94c,r1 */
L8c65e894: *stop=0x8c65e894u;r[0]&=r[1];goto L8c65e896; /* and r1,r0 */
L8c65e896: *stop=0x8c65e896u;r[0]|=0xf0u;goto L8c65e898; /* or #0xf0,r0 */
L8c65e898: *stop=0x8c65e898u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65e89a; /* ldc r0,SR */
L8c65e89a: *stop=0x8c65e89au;r[3]=4294967294u;goto L8c65e89c; /* mov #-0x2,r3 */
L8c65e89c: *stop=0x8c65e89cu;r[3]<<=8;goto L8c65e89e; /* shll8 r3 */
L8c65e89e: *stop=0x8c65e89eu;status=sa_sh4_stc_vbr(*vbr,*sr,&r[14],0u);if(status)return status;goto L8c65e8a0; /* stc VBR,r14 */
L8c65e8a0: *stop=0x8c65e8a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65e954u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65e8a2; /* mov.l 0x8c65e954,r4 */
L8c65e8a2: *stop=0x8c65e8a2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65e8a4; /* mov.l @r4,r4 */
L8c65e8a4: *stop=0x8c65e8a4u;r[2]=r[4];goto L8c65e8a6; /* mov r4,r2 */
L8c65e8a6: *stop=0x8c65e8a6u;r[2]+=r[3];goto L8c65e8a8; /* add r3,r2 */
L8c65e8a8: *stop=0x8c65e8a8u;r[0]=2u;goto L8c65e8aa; /* mov #0x2,r0 */
L8c65e8aa: *stop=0x8c65e8aau;r[0]<<=8;goto L8c65e8ac; /* shll8 r0 */
L8c65e8ac: *stop=0x8c65e8acu;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c65e8ae; /* shar r2 */
L8c65e8ae: *stop=0x8c65e8aeu;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c65e8b0; /* shar r2 */
L8c65e8b0: *stop=0x8c65e8b0u;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c65e8b2; /* shar r2 */
L8c65e8b2: *stop=0x8c65e8b2u;r[14]+=r[2];goto L8c65e8b4; /* add r2,r14 */
L8c65e8b4: *stop=0x8c65e8b4u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c65e8b6; /* mov.l @(r0,r14),r14 */
L8c65e8b6: *stop=0x8c65e8b6u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65e8b8; /* tst r14,r14 */
L8c65e8b8: if((*sr&1u))goto L8c65e8c6;goto L8c65e8ba; /* bt 0x8c65e8c6 */
L8c65e8ba: target=r[14];*pr=0x8c65e8beu;*stop=0x8c65e8bcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65e8be; /* jsr @r14 */
L8c65e8be: *stop=0x8c65e8beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65e950u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65e8c0; /* mov.l 0x8c65e950,r0 */
L8c65e8c0: *stop=0x8c65e8c0u;status=sa_sh4_stc_sr(*sr,&r[1],0u);if(status)return status;goto L8c65e8c2; /* stc SR,r1 */
L8c65e8c2: *stop=0x8c65e8c2u;r[1]|=r[0];goto L8c65e8c4; /* or r0,r1 */
L8c65e8c4: *stop=0x8c65e8c4u;status=sa_sh4_ldc_sr(r[1],sr,r,integer,0u);if(status)return status;goto L8c65e8c6; /* ldc r1,SR */
L8c65e8c6: *stop=0x8c65e8c6u;r[0]=4u;goto L8c65e8c8; /* mov #0x4,r0 */
L8c65e8c8: *stop=0x8c65e8c8u;r[0]<<=16;goto L8c65e8ca; /* shll16 r0 */
L8c65e8ca: *stop=0x8c65e8cau;r[0]+=1u;goto L8c65e8cc; /* add #0x1,r0 */
L8c65e8cc: *stop=0x8c65e8ccu;f->fpscr=r[0]&0x3fffffu;goto L8c65e8ce; /* lds r0,fpscr */
L8c65e8ce: *stop=0x8c65e8ceu;r[0]^=r[0];goto L8c65e8d0; /* xor r0,r0 */
L8c65e8d0: *stop=0x8c65e8d0u;*fpul=r[0];goto L8c65e8d2; /* lds r0,FPUL */
L8c65e8d2: *stop=0x8c65e8d2u;if(*sr&0x8000u)return -4;*stop=0x8c65e8d2u;f->fpscr^=0x200000u;goto L8c65e8d4; /* frchg */
L8c65e8d4: *stop=0x8c65e8d4u;if(*sr&0x8000u)return -4;*stop=0x8c65e8d4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8d6; /* fmov.s @r15+,fr0 */
L8c65e8d6: *stop=0x8c65e8d6u;if(*sr&0x8000u)return -4;*stop=0x8c65e8d6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8d8; /* fmov.s @r15+,fr1 */
L8c65e8d8: *stop=0x8c65e8d8u;if(*sr&0x8000u)return -4;*stop=0x8c65e8d8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8da; /* fmov.s @r15+,fr2 */
L8c65e8da: *stop=0x8c65e8dau;if(*sr&0x8000u)return -4;*stop=0x8c65e8dau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8dc; /* fmov.s @r15+,fr3 */
L8c65e8dc: *stop=0x8c65e8dcu;if(*sr&0x8000u)return -4;*stop=0x8c65e8dcu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8de; /* fmov.s @r15+,fr4 */
L8c65e8de: *stop=0x8c65e8deu;if(*sr&0x8000u)return -4;*stop=0x8c65e8deu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8e0; /* fmov.s @r15+,fr5 */
L8c65e8e0: *stop=0x8c65e8e0u;if(*sr&0x8000u)return -4;*stop=0x8c65e8e0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8e2; /* fmov.s @r15+,fr6 */
L8c65e8e2: *stop=0x8c65e8e2u;if(*sr&0x8000u)return -4;*stop=0x8c65e8e2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8e4; /* fmov.s @r15+,fr7 */
L8c65e8e4: *stop=0x8c65e8e4u;if(*sr&0x8000u)return -4;*stop=0x8c65e8e4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8e6; /* fmov.s @r15+,fr8 */
L8c65e8e6: *stop=0x8c65e8e6u;if(*sr&0x8000u)return -4;*stop=0x8c65e8e6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8e8; /* fmov.s @r15+,fr9 */
L8c65e8e8: *stop=0x8c65e8e8u;if(*sr&0x8000u)return -4;*stop=0x8c65e8e8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8ea; /* fmov.s @r15+,fr10 */
L8c65e8ea: *stop=0x8c65e8eau;if(*sr&0x8000u)return -4;*stop=0x8c65e8eau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8ec; /* fmov.s @r15+,fr11 */
L8c65e8ec: *stop=0x8c65e8ecu;if(*sr&0x8000u)return -4;*stop=0x8c65e8ecu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8ee; /* fmov.s @r15+,fr12 */
L8c65e8ee: *stop=0x8c65e8eeu;if(*sr&0x8000u)return -4;*stop=0x8c65e8eeu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8f0; /* fmov.s @r15+,fr13 */
L8c65e8f0: *stop=0x8c65e8f0u;if(*sr&0x8000u)return -4;*stop=0x8c65e8f0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8f2; /* fmov.s @r15+,fr14 */
L8c65e8f2: *stop=0x8c65e8f2u;if(*sr&0x8000u)return -4;*stop=0x8c65e8f2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8f4; /* fmov.s @r15+,fr15 */
L8c65e8f4: *stop=0x8c65e8f4u;if(*sr&0x8000u)return -4;*stop=0x8c65e8f4u;f->fpscr^=0x200000u;goto L8c65e8f6; /* frchg */
L8c65e8f6: *stop=0x8c65e8f6u;if(*sr&0x8000u)return -4;*stop=0x8c65e8f6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8f8; /* fmov.s @r15+,fr0 */
L8c65e8f8: *stop=0x8c65e8f8u;if(*sr&0x8000u)return -4;*stop=0x8c65e8f8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8fa; /* fmov.s @r15+,fr1 */
L8c65e8fa: *stop=0x8c65e8fau;if(*sr&0x8000u)return -4;*stop=0x8c65e8fau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8fc; /* fmov.s @r15+,fr2 */
L8c65e8fc: *stop=0x8c65e8fcu;if(*sr&0x8000u)return -4;*stop=0x8c65e8fcu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e8fe; /* fmov.s @r15+,fr3 */
L8c65e8fe: *stop=0x8c65e8feu;if(*sr&0x8000u)return -4;*stop=0x8c65e8feu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e900; /* fmov.s @r15+,fr4 */
L8c65e900: *stop=0x8c65e900u;if(*sr&0x8000u)return -4;*stop=0x8c65e900u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e902; /* fmov.s @r15+,fr5 */
L8c65e902: *stop=0x8c65e902u;if(*sr&0x8000u)return -4;*stop=0x8c65e902u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e904; /* fmov.s @r15+,fr6 */
L8c65e904: *stop=0x8c65e904u;if(*sr&0x8000u)return -4;*stop=0x8c65e904u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e906; /* fmov.s @r15+,fr7 */
L8c65e906: *stop=0x8c65e906u;if(*sr&0x8000u)return -4;*stop=0x8c65e906u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e908; /* fmov.s @r15+,fr8 */
L8c65e908: *stop=0x8c65e908u;if(*sr&0x8000u)return -4;*stop=0x8c65e908u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e90a; /* fmov.s @r15+,fr9 */
L8c65e90a: *stop=0x8c65e90au;if(*sr&0x8000u)return -4;*stop=0x8c65e90au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e90c; /* fmov.s @r15+,fr10 */
L8c65e90c: *stop=0x8c65e90cu;if(*sr&0x8000u)return -4;*stop=0x8c65e90cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e90e; /* fmov.s @r15+,fr11 */
L8c65e90e: *stop=0x8c65e90eu;if(*sr&0x8000u)return -4;*stop=0x8c65e90eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e910; /* fmov.s @r15+,fr12 */
L8c65e910: *stop=0x8c65e910u;if(*sr&0x8000u)return -4;*stop=0x8c65e910u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e912; /* fmov.s @r15+,fr13 */
L8c65e912: *stop=0x8c65e912u;if(*sr&0x8000u)return -4;*stop=0x8c65e912u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e914; /* fmov.s @r15+,fr14 */
L8c65e914: *stop=0x8c65e914u;if(*sr&0x8000u)return -4;*stop=0x8c65e914u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65e916; /* fmov.s @r15+,fr15 */
L8c65e916: *stop=0x8c65e916u;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;f->fpscr=value&0x3fffffu;r[15]+=4u;}goto L8c65e918; /* lds.l @r15+,fpscr */
L8c65e918: *stop=0x8c65e918u;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*fpul=value;r[15]+=4u;}goto L8c65e91a; /* lds.l @r15+,FPUL */
L8c65e91a: *stop=0x8c65e91au;if(!(*sr&0x40000000u))return -4;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;status=sa_sh4_ldc_sr(value,sr,r,integer,0u);if(status)return status;r[15]+=4u;}goto L8c65e91c; /* ldc.l @r15+,SR */
L8c65e91c: *stop=0x8c65e91cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}r[15]+=4;goto L8c65e91e; /* mov.l @r15+,r0 */
L8c65e91e: *stop=0x8c65e91eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c65e920; /* mov.l @r15+,r1 */
L8c65e920: *stop=0x8c65e920u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;goto L8c65e922; /* mov.l @r15+,r2 */
L8c65e922: *stop=0x8c65e922u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c65e924; /* mov.l @r15+,r3 */
L8c65e924: *stop=0x8c65e924u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c65e926; /* mov.l @r15+,r4 */
L8c65e926: *stop=0x8c65e926u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}r[15]+=4;goto L8c65e928; /* mov.l @r15+,r5 */
L8c65e928: *stop=0x8c65e928u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}r[15]+=4;goto L8c65e92a; /* mov.l @r15+,r6 */
L8c65e92a: *stop=0x8c65e92au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[7]=loaded;}r[15]+=4;goto L8c65e92c; /* mov.l @r15+,r7 */
L8c65e92c: *stop=0x8c65e92cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65e92e; /* mov.l @r15+,r8 */
L8c65e92e: *stop=0x8c65e92eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65e930; /* mov.l @r15+,r9 */
L8c65e930: *stop=0x8c65e930u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65e932; /* mov.l @r15+,r10 */
L8c65e932: *stop=0x8c65e932u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65e934; /* mov.l @r15+,r11 */
L8c65e934: *stop=0x8c65e934u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65e936; /* mov.l @r15+,r12 */
L8c65e936: *stop=0x8c65e936u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65e938; /* mov.l @r15+,r13 */
L8c65e938: *stop=0x8c65e938u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;goto L8c65e93a; /* mov.l @r15+,r14 */
L8c65e93a: *stop=0x8c65e93au;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*macl=value;r[15]+=4u;}goto L8c65e93c; /* lds.l @r15+,MACL */
L8c65e93c: *stop=0x8c65e93cu;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;integer->mach=value;r[15]+=4u;}goto L8c65e93e; /* lds.l @r15+,MACH */
L8c65e93e: *stop=0x8c65e93eu;if(!(*sr&0x40000000u))return -4;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*vbr=value;r[15]+=4u;}goto L8c65e940; /* ldc.l @r15+,VBR */
L8c65e940: *stop=0x8c65e940u;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*gbr=value;r[15]+=4u;}goto L8c65e942; /* ldc.l @r15+,GBR */
L8c65e942: *stop=0x8c65e942u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65e944; /* lds.l @r15+,PR */
L8c65e944: *stop=0x8c65e944u;if(!(*sr&0x40000000u))return -4;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;status=sa_sh4_exception_ldc(exception,SA_EXCEPTION_SPC,value,*sr,0u);if(status)return status;r[15]+=4u;}goto L8c65e946; /* ldc.l @r15+,SPC */
L8c65e946: *stop=0x8c65e946u;if(!(*sr&0x40000000u))return -4;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;status=sa_sh4_exception_ldc(exception,SA_EXCEPTION_SSR,value,*sr,0u);if(status)return status;r[15]+=4u;}goto L8c65e948; /* ldc.l @r15+,SSR */
L8c65e948: *stop=0x8c65e948u;status=sa_sh4_rte_begin(exception,sr,r,integer,&target,0);if(status)return status;*stop=0x8c65e94au;exception->returned=1;*stop=target;return 2; /* rte */
}

SA_STAGE_EXPORT int sa_movie_interrupt_context_8c65e958(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t *vbr,struct sa_sh4_exception_state *exception){
 if(!exception)return -2;
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65e958;
L8c65e958: *stop=0x8c65e958u;r[1]=r[0];goto L8c65e95a; /* mov r0,r1 */
L8c65e95a: *stop=0x8c65e95au;r[0]=0x8c65eaecu;goto L8c65e95c; /* mova 0x8c65eaec,r0 */
L8c65e95c: *stop=0x8c65e95cu;if(!valid_access(r[0]-4u,4u))return -3;r[0]-=4;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c65e95e; /* mov.l r1,@-r0 */
L8c65e95e: *stop=0x8c65e95eu;r[0]+=4294967292u;goto L8c65e960; /* add #-0x4,r0 */
L8c65e960: *stop=0x8c65e960u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65e962; /* mov.l @r0,r1 */
L8c65e962: *stop=0x8c65e962u;r[1]+=1u;goto L8c65e964; /* add #0x1,r1 */
L8c65e964: *stop=0x8c65e964u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c65e966; /* mov.l r1,@r0 */
L8c65e966: *stop=0x8c65e966u;r[0]+=4u;goto L8c65e968; /* add #0x4,r0 */
L8c65e968: *stop=0x8c65e968u;r[1]--;*sr=(*sr&~1u)|(!r[1]);goto L8c65e96a; /* dt r1 */
L8c65e96a: *stop=0x8c65e96au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65e96c; /* mov.l @r0,r0 */
L8c65e96c: take=!(*sr&1u);*stop=0x8c65e96eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;if(take)goto L8c65e97e;goto L8c65e970; /* bf/s 0x8c65e97e */
L8c65e970: *stop=0x8c65e970u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c65e972; /* mov.l r0,@-r15 */
L8c65e972: *stop=0x8c65e972u;r[0]=0x8c65ead8u;goto L8c65e974; /* mova 0x8c65ead8,r0 */
L8c65e974: *stop=0x8c65e974u;r[15]+=4u;goto L8c65e976; /* add #0x4,r15 */
L8c65e976: *stop=0x8c65e976u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[15],4);if(io_status)return io_status;goto L8c65e978; /* mov.l r15,@r0 */
L8c65e978: *stop=0x8c65e978u;r[15]+=4294967292u;goto L8c65e97a; /* add #-0x4,r15 */
L8c65e97a: *stop=0x8c65e97au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}r[15]+=4;goto L8c65e97c; /* mov.l @r15+,r0 */
L8c65e97c: *stop=0x8c65e97cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65eadcu,4);if(io_status)return io_status;r[15]=loaded;}goto L8c65e97e; /* mov.l 0x8c65eadc,r15 */
L8c65e97e: *stop=0x8c65e97eu;{uint32_t value;status=sa_sh4_exception_stc(exception,SA_EXCEPTION_SSR,*sr,&value,0u);if(status)return status;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e980; /* stc.l SSR,@-r15 */
L8c65e980: *stop=0x8c65e980u;{uint32_t value;status=sa_sh4_exception_stc(exception,SA_EXCEPTION_SPC,*sr,&value,0u);if(status)return status;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e982; /* stc.l SPC,@-r15 */
L8c65e982: *stop=0x8c65e982u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65e984; /* sts.l PR,@-r15 */
L8c65e984: *stop=0x8c65e984u;{uint32_t value;value=*gbr;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e986; /* stc.l GBR,@-r15 */
L8c65e986: *stop=0x8c65e986u;{uint32_t value;status=sa_sh4_stc_vbr(*vbr,*sr,&value,0u);if(status)return status;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e988; /* stc.l VBR,@-r15 */
L8c65e988: *stop=0x8c65e988u;{uint32_t value;value=integer->mach;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e98a; /* sts.l MACH,@-r15 */
L8c65e98a: *stop=0x8c65e98au;{uint32_t value;value=*macl;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e98c; /* sts.l MACL,@-r15 */
L8c65e98c: *stop=0x8c65e98cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65e98e; /* mov.l r14,@-r15 */
L8c65e98e: *stop=0x8c65e98eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65e990; /* mov.l r13,@-r15 */
L8c65e990: *stop=0x8c65e990u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65e992; /* mov.l r12,@-r15 */
L8c65e992: *stop=0x8c65e992u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65e994; /* mov.l r11,@-r15 */
L8c65e994: *stop=0x8c65e994u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65e996; /* mov.l r10,@-r15 */
L8c65e996: *stop=0x8c65e996u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65e998; /* mov.l r9,@-r15 */
L8c65e998: *stop=0x8c65e998u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65e99a; /* mov.l r8,@-r15 */
L8c65e99a: *stop=0x8c65e99au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c65e99c; /* mov.l r7,@-r15 */
L8c65e99c: *stop=0x8c65e99cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c65e99e; /* mov.l r6,@-r15 */
L8c65e99e: *stop=0x8c65e99eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c65e9a0; /* mov.l r5,@-r15 */
L8c65e9a0: *stop=0x8c65e9a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c65e9a2; /* mov.l r4,@-r15 */
L8c65e9a2: *stop=0x8c65e9a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65e9a4; /* mov.l r3,@-r15 */
L8c65e9a4: *stop=0x8c65e9a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c65e9a6; /* mov.l r2,@-r15 */
L8c65e9a6: *stop=0x8c65e9a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c65e9a8; /* mov.l r1,@-r15 */
L8c65e9a8: *stop=0x8c65e9a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c65e9aa; /* mov.l r0,@-r15 */
L8c65e9aa: *stop=0x8c65e9aau;{uint32_t value;status=sa_sh4_stc_sr(*sr,&value,0u);if(status)return status;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e9ac; /* stc.l SR,@-r15 */
L8c65e9ac: *stop=0x8c65e9acu;{uint32_t value;value=*fpul;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e9ae; /* sts.l FPUL,@-r15 */
L8c65e9ae: *stop=0x8c65e9aeu;{uint32_t value;value=f->fpscr;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],value,4);if(io_status)return io_status;}goto L8c65e9b0; /* sts.l FPSCR,@-r15 */
L8c65e9b0: *stop=0x8c65e9b0u;r[0]=4u;goto L8c65e9b2; /* mov #0x4,r0 */
L8c65e9b2: *stop=0x8c65e9b2u;r[0]<<=16;goto L8c65e9b4; /* shll16 r0 */
L8c65e9b4: *stop=0x8c65e9b4u;r[0]+=1u;goto L8c65e9b6; /* add #0x1,r0 */
L8c65e9b6: *stop=0x8c65e9b6u;f->fpscr=r[0]&0x3fffffu;goto L8c65e9b8; /* lds r0,fpscr */
L8c65e9b8: *stop=0x8c65e9b8u;r[0]^=r[0];goto L8c65e9ba; /* xor r0,r0 */
L8c65e9ba: *stop=0x8c65e9bau;*fpul=r[0];goto L8c65e9bc; /* lds r0,FPUL */
L8c65e9bc: *stop=0x8c65e9bcu;if(*sr&0x8000u)return -4;*stop=0x8c65e9bcu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c65e9be; /* fmov.s fr15,@-r15 */
L8c65e9be: *stop=0x8c65e9beu;if(*sr&0x8000u)return -4;*stop=0x8c65e9beu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c65e9c0; /* fmov.s fr14,@-r15 */
L8c65e9c0: *stop=0x8c65e9c0u;if(*sr&0x8000u)return -4;*stop=0x8c65e9c0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c65e9c2; /* fmov.s fr13,@-r15 */
L8c65e9c2: *stop=0x8c65e9c2u;if(*sr&0x8000u)return -4;*stop=0x8c65e9c2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c65e9c4; /* fmov.s fr12,@-r15 */
L8c65e9c4: *stop=0x8c65e9c4u;if(*sr&0x8000u)return -4;*stop=0x8c65e9c4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c65e9c6; /* fmov.s fr11,@-r15 */
L8c65e9c6: *stop=0x8c65e9c6u;if(*sr&0x8000u)return -4;*stop=0x8c65e9c6u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c65e9c8; /* fmov.s fr10,@-r15 */
L8c65e9c8: *stop=0x8c65e9c8u;if(*sr&0x8000u)return -4;*stop=0x8c65e9c8u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c65e9ca; /* fmov.s fr9,@-r15 */
L8c65e9ca: *stop=0x8c65e9cau;if(*sr&0x8000u)return -4;*stop=0x8c65e9cau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c65e9cc; /* fmov.s fr8,@-r15 */
L8c65e9cc: *stop=0x8c65e9ccu;if(*sr&0x8000u)return -4;*stop=0x8c65e9ccu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c65e9ce; /* fmov.s fr7,@-r15 */
L8c65e9ce: *stop=0x8c65e9ceu;if(*sr&0x8000u)return -4;*stop=0x8c65e9ceu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c65e9d0; /* fmov.s fr6,@-r15 */
L8c65e9d0: *stop=0x8c65e9d0u;if(*sr&0x8000u)return -4;*stop=0x8c65e9d0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c65e9d2; /* fmov.s fr5,@-r15 */
L8c65e9d2: *stop=0x8c65e9d2u;if(*sr&0x8000u)return -4;*stop=0x8c65e9d2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c65e9d4; /* fmov.s fr4,@-r15 */
L8c65e9d4: *stop=0x8c65e9d4u;if(*sr&0x8000u)return -4;*stop=0x8c65e9d4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c65e9d6; /* fmov.s fr3,@-r15 */
L8c65e9d6: *stop=0x8c65e9d6u;if(*sr&0x8000u)return -4;*stop=0x8c65e9d6u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c65e9d8; /* fmov.s fr2,@-r15 */
L8c65e9d8: *stop=0x8c65e9d8u;if(*sr&0x8000u)return -4;*stop=0x8c65e9d8u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c65e9da; /* fmov.s fr1,@-r15 */
L8c65e9da: *stop=0x8c65e9dau;if(*sr&0x8000u)return -4;*stop=0x8c65e9dau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c65e9dc; /* fmov.s fr0,@-r15 */
L8c65e9dc: *stop=0x8c65e9dcu;if(*sr&0x8000u)return -4;*stop=0x8c65e9dcu;f->fpscr^=0x200000u;goto L8c65e9de; /* frchg */
L8c65e9de: *stop=0x8c65e9deu;if(*sr&0x8000u)return -4;*stop=0x8c65e9deu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c65e9e0; /* fmov.s fr15,@-r15 */
L8c65e9e0: *stop=0x8c65e9e0u;if(*sr&0x8000u)return -4;*stop=0x8c65e9e0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c65e9e2; /* fmov.s fr14,@-r15 */
L8c65e9e2: *stop=0x8c65e9e2u;if(*sr&0x8000u)return -4;*stop=0x8c65e9e2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c65e9e4; /* fmov.s fr13,@-r15 */
L8c65e9e4: *stop=0x8c65e9e4u;if(*sr&0x8000u)return -4;*stop=0x8c65e9e4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c65e9e6; /* fmov.s fr12,@-r15 */
L8c65e9e6: *stop=0x8c65e9e6u;if(*sr&0x8000u)return -4;*stop=0x8c65e9e6u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c65e9e8; /* fmov.s fr11,@-r15 */
L8c65e9e8: *stop=0x8c65e9e8u;if(*sr&0x8000u)return -4;*stop=0x8c65e9e8u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c65e9ea; /* fmov.s fr10,@-r15 */
L8c65e9ea: *stop=0x8c65e9eau;if(*sr&0x8000u)return -4;*stop=0x8c65e9eau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c65e9ec; /* fmov.s fr9,@-r15 */
L8c65e9ec: *stop=0x8c65e9ecu;if(*sr&0x8000u)return -4;*stop=0x8c65e9ecu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c65e9ee; /* fmov.s fr8,@-r15 */
L8c65e9ee: *stop=0x8c65e9eeu;if(*sr&0x8000u)return -4;*stop=0x8c65e9eeu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c65e9f0; /* fmov.s fr7,@-r15 */
L8c65e9f0: *stop=0x8c65e9f0u;if(*sr&0x8000u)return -4;*stop=0x8c65e9f0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c65e9f2; /* fmov.s fr6,@-r15 */
L8c65e9f2: *stop=0x8c65e9f2u;if(*sr&0x8000u)return -4;*stop=0x8c65e9f2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c65e9f4; /* fmov.s fr5,@-r15 */
L8c65e9f4: *stop=0x8c65e9f4u;if(*sr&0x8000u)return -4;*stop=0x8c65e9f4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c65e9f6; /* fmov.s fr4,@-r15 */
L8c65e9f6: *stop=0x8c65e9f6u;if(*sr&0x8000u)return -4;*stop=0x8c65e9f6u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c65e9f8; /* fmov.s fr3,@-r15 */
L8c65e9f8: *stop=0x8c65e9f8u;if(*sr&0x8000u)return -4;*stop=0x8c65e9f8u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c65e9fa; /* fmov.s fr2,@-r15 */
L8c65e9fa: *stop=0x8c65e9fau;if(*sr&0x8000u)return -4;*stop=0x8c65e9fau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c65e9fc; /* fmov.s fr1,@-r15 */
L8c65e9fc: *stop=0x8c65e9fcu;if(*sr&0x8000u)return -4;*stop=0x8c65e9fcu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c65e9fe; /* fmov.s fr0,@-r15 */
L8c65e9fe: *stop=0x8c65e9feu;if(*sr&0x8000u)return -4;*stop=0x8c65e9feu;f->fpscr^=0x200000u;goto L8c65ea00; /* frchg */
L8c65ea00: *stop=0x8c65ea00u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65ea02; /* stc SR,r0 */
L8c65ea02: *stop=0x8c65ea02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65eaccu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ea04; /* mov.l 0x8c65eacc,r1 */
L8c65ea04: *stop=0x8c65ea04u;r[0]&=r[1];goto L8c65ea06; /* and r1,r0 */
L8c65ea06: *stop=0x8c65ea06u;r[0]|=0xf0u;goto L8c65ea08; /* or #0xf0,r0 */
L8c65ea08: *stop=0x8c65ea08u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65ea0a; /* ldc r0,SR */
L8c65ea0a: *stop=0x8c65ea0au;r[3]=4294967294u;goto L8c65ea0c; /* mov #-0x2,r3 */
L8c65ea0c: *stop=0x8c65ea0cu;r[3]<<=8;goto L8c65ea0e; /* shll8 r3 */
L8c65ea0e: *stop=0x8c65ea0eu;status=sa_sh4_stc_vbr(*vbr,*sr,&r[14],0u);if(status)return status;goto L8c65ea10; /* stc VBR,r14 */
L8c65ea10: *stop=0x8c65ea10u;r[0]=0x8c65eae0u;goto L8c65ea12; /* mova 0x8c65eae0,r0 */
L8c65ea12: *stop=0x8c65ea12u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65ea14; /* mov.l @r0,r4 */
L8c65ea14: *stop=0x8c65ea14u;r[2]=r[4];goto L8c65ea16; /* mov r4,r2 */
L8c65ea16: *stop=0x8c65ea16u;r[2]+=r[3];goto L8c65ea18; /* add r3,r2 */
L8c65ea18: *stop=0x8c65ea18u;r[0]=2u;goto L8c65ea1a; /* mov #0x2,r0 */
L8c65ea1a: *stop=0x8c65ea1au;r[0]<<=8;goto L8c65ea1c; /* shll8 r0 */
L8c65ea1c: *stop=0x8c65ea1cu;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c65ea1e; /* shar r2 */
L8c65ea1e: *stop=0x8c65ea1eu;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c65ea20; /* shar r2 */
L8c65ea20: *stop=0x8c65ea20u;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c65ea22; /* shar r2 */
L8c65ea22: *stop=0x8c65ea22u;r[14]+=r[2];goto L8c65ea24; /* add r2,r14 */
L8c65ea24: *stop=0x8c65ea24u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c65ea26; /* mov.l @(r0,r14),r14 */
L8c65ea26: *stop=0x8c65ea26u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65ea28; /* tst r14,r14 */
L8c65ea28: if((*sr&1u))goto L8c65ea2e;goto L8c65ea2a; /* bt 0x8c65ea2e */
L8c65ea2a: target=r[14];*pr=0x8c65ea2eu;*stop=0x8c65ea2cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ea2e; /* jsr @r14 */
L8c65ea2e: *stop=0x8c65ea2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ead0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65ea30; /* mov.l 0x8c65ead0,r0 */
L8c65ea30: *stop=0x8c65ea30u;status=sa_sh4_stc_sr(*sr,&r[1],0u);if(status)return status;goto L8c65ea32; /* stc SR,r1 */
L8c65ea32: *stop=0x8c65ea32u;r[1]|=r[0];goto L8c65ea34; /* or r0,r1 */
L8c65ea34: *stop=0x8c65ea34u;status=sa_sh4_ldc_sr(r[1],sr,r,integer,0u);if(status)return status;goto L8c65ea36; /* ldc r1,SR */
L8c65ea36: *stop=0x8c65ea36u;r[0]=0x8c65eae4u;goto L8c65ea38; /* mova 0x8c65eae4,r0 */
L8c65ea38: *stop=0x8c65ea38u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ea3a; /* mov.l @r0,r1 */
L8c65ea3a: *stop=0x8c65ea3au;r[1]+=4294967295u;goto L8c65ea3c; /* add #-0x1,r1 */
L8c65ea3c: *stop=0x8c65ea3cu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c65ea3e; /* mov.l r1,@r0 */
L8c65ea3e: *stop=0x8c65ea3eu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c65ea40; /* tst r1,r1 */
L8c65ea40: *stop=0x8c65ea40u;r[0]=4u;goto L8c65ea42; /* mov #0x4,r0 */
L8c65ea42: *stop=0x8c65ea42u;r[0]<<=16;goto L8c65ea44; /* shll16 r0 */
L8c65ea44: *stop=0x8c65ea44u;r[0]+=1u;goto L8c65ea46; /* add #0x1,r0 */
L8c65ea46: *stop=0x8c65ea46u;f->fpscr=r[0]&0x3fffffu;goto L8c65ea48; /* lds r0,fpscr */
L8c65ea48: *stop=0x8c65ea48u;r[0]^=r[0];goto L8c65ea4a; /* xor r0,r0 */
L8c65ea4a: *stop=0x8c65ea4au;*fpul=r[0];goto L8c65ea4c; /* lds r0,FPUL */
L8c65ea4c: *stop=0x8c65ea4cu;if(*sr&0x8000u)return -4;*stop=0x8c65ea4cu;f->fpscr^=0x200000u;goto L8c65ea4e; /* frchg */
L8c65ea4e: *stop=0x8c65ea4eu;if(*sr&0x8000u)return -4;*stop=0x8c65ea4eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea50; /* fmov.s @r15+,fr0 */
L8c65ea50: *stop=0x8c65ea50u;if(*sr&0x8000u)return -4;*stop=0x8c65ea50u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea52; /* fmov.s @r15+,fr1 */
L8c65ea52: *stop=0x8c65ea52u;if(*sr&0x8000u)return -4;*stop=0x8c65ea52u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea54; /* fmov.s @r15+,fr2 */
L8c65ea54: *stop=0x8c65ea54u;if(*sr&0x8000u)return -4;*stop=0x8c65ea54u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea56; /* fmov.s @r15+,fr3 */
L8c65ea56: *stop=0x8c65ea56u;if(*sr&0x8000u)return -4;*stop=0x8c65ea56u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea58; /* fmov.s @r15+,fr4 */
L8c65ea58: *stop=0x8c65ea58u;if(*sr&0x8000u)return -4;*stop=0x8c65ea58u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea5a; /* fmov.s @r15+,fr5 */
L8c65ea5a: *stop=0x8c65ea5au;if(*sr&0x8000u)return -4;*stop=0x8c65ea5au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea5c; /* fmov.s @r15+,fr6 */
L8c65ea5c: *stop=0x8c65ea5cu;if(*sr&0x8000u)return -4;*stop=0x8c65ea5cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea5e; /* fmov.s @r15+,fr7 */
L8c65ea5e: *stop=0x8c65ea5eu;if(*sr&0x8000u)return -4;*stop=0x8c65ea5eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea60; /* fmov.s @r15+,fr8 */
L8c65ea60: *stop=0x8c65ea60u;if(*sr&0x8000u)return -4;*stop=0x8c65ea60u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea62; /* fmov.s @r15+,fr9 */
L8c65ea62: *stop=0x8c65ea62u;if(*sr&0x8000u)return -4;*stop=0x8c65ea62u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea64; /* fmov.s @r15+,fr10 */
L8c65ea64: *stop=0x8c65ea64u;if(*sr&0x8000u)return -4;*stop=0x8c65ea64u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea66; /* fmov.s @r15+,fr11 */
L8c65ea66: *stop=0x8c65ea66u;if(*sr&0x8000u)return -4;*stop=0x8c65ea66u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea68; /* fmov.s @r15+,fr12 */
L8c65ea68: *stop=0x8c65ea68u;if(*sr&0x8000u)return -4;*stop=0x8c65ea68u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea6a; /* fmov.s @r15+,fr13 */
L8c65ea6a: *stop=0x8c65ea6au;if(*sr&0x8000u)return -4;*stop=0x8c65ea6au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea6c; /* fmov.s @r15+,fr14 */
L8c65ea6c: *stop=0x8c65ea6cu;if(*sr&0x8000u)return -4;*stop=0x8c65ea6cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea6e; /* fmov.s @r15+,fr15 */
L8c65ea6e: *stop=0x8c65ea6eu;if(*sr&0x8000u)return -4;*stop=0x8c65ea6eu;f->fpscr^=0x200000u;goto L8c65ea70; /* frchg */
L8c65ea70: *stop=0x8c65ea70u;if(*sr&0x8000u)return -4;*stop=0x8c65ea70u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea72; /* fmov.s @r15+,fr0 */
L8c65ea72: *stop=0x8c65ea72u;if(*sr&0x8000u)return -4;*stop=0x8c65ea72u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea74; /* fmov.s @r15+,fr1 */
L8c65ea74: *stop=0x8c65ea74u;if(*sr&0x8000u)return -4;*stop=0x8c65ea74u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea76; /* fmov.s @r15+,fr2 */
L8c65ea76: *stop=0x8c65ea76u;if(*sr&0x8000u)return -4;*stop=0x8c65ea76u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea78; /* fmov.s @r15+,fr3 */
L8c65ea78: *stop=0x8c65ea78u;if(*sr&0x8000u)return -4;*stop=0x8c65ea78u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea7a; /* fmov.s @r15+,fr4 */
L8c65ea7a: *stop=0x8c65ea7au;if(*sr&0x8000u)return -4;*stop=0x8c65ea7au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea7c; /* fmov.s @r15+,fr5 */
L8c65ea7c: *stop=0x8c65ea7cu;if(*sr&0x8000u)return -4;*stop=0x8c65ea7cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea7e; /* fmov.s @r15+,fr6 */
L8c65ea7e: *stop=0x8c65ea7eu;if(*sr&0x8000u)return -4;*stop=0x8c65ea7eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea80; /* fmov.s @r15+,fr7 */
L8c65ea80: *stop=0x8c65ea80u;if(*sr&0x8000u)return -4;*stop=0x8c65ea80u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea82; /* fmov.s @r15+,fr8 */
L8c65ea82: *stop=0x8c65ea82u;if(*sr&0x8000u)return -4;*stop=0x8c65ea82u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea84; /* fmov.s @r15+,fr9 */
L8c65ea84: *stop=0x8c65ea84u;if(*sr&0x8000u)return -4;*stop=0x8c65ea84u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea86; /* fmov.s @r15+,fr10 */
L8c65ea86: *stop=0x8c65ea86u;if(*sr&0x8000u)return -4;*stop=0x8c65ea86u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea88; /* fmov.s @r15+,fr11 */
L8c65ea88: *stop=0x8c65ea88u;if(*sr&0x8000u)return -4;*stop=0x8c65ea88u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea8a; /* fmov.s @r15+,fr12 */
L8c65ea8a: *stop=0x8c65ea8au;if(*sr&0x8000u)return -4;*stop=0x8c65ea8au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea8c; /* fmov.s @r15+,fr13 */
L8c65ea8c: *stop=0x8c65ea8cu;if(*sr&0x8000u)return -4;*stop=0x8c65ea8cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea8e; /* fmov.s @r15+,fr14 */
L8c65ea8e: *stop=0x8c65ea8eu;if(*sr&0x8000u)return -4;*stop=0x8c65ea8eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65ea90; /* fmov.s @r15+,fr15 */
L8c65ea90: *stop=0x8c65ea90u;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;f->fpscr=value&0x3fffffu;r[15]+=4u;}goto L8c65ea92; /* lds.l @r15+,fpscr */
L8c65ea92: *stop=0x8c65ea92u;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*fpul=value;r[15]+=4u;}goto L8c65ea94; /* lds.l @r15+,FPUL */
L8c65ea94: *stop=0x8c65ea94u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c65ea96; /* mov.l @r15+,r4 */
L8c65ea96: *stop=0x8c65ea96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}r[15]+=4;goto L8c65ea98; /* mov.l @r15+,r0 */
L8c65ea98: *stop=0x8c65ea98u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c65ea9a; /* mov.l @r15+,r1 */
L8c65ea9a: *stop=0x8c65ea9au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;goto L8c65ea9c; /* mov.l @r15+,r2 */
L8c65ea9c: *stop=0x8c65ea9cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c65ea9e; /* mov.l @r15+,r3 */
L8c65ea9e: *stop=0x8c65ea9eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c65eaa0; /* mov.l @r15+,r4 */
L8c65eaa0: *stop=0x8c65eaa0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}r[15]+=4;goto L8c65eaa2; /* mov.l @r15+,r5 */
L8c65eaa2: *stop=0x8c65eaa2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}r[15]+=4;goto L8c65eaa4; /* mov.l @r15+,r6 */
L8c65eaa4: *stop=0x8c65eaa4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[7]=loaded;}r[15]+=4;goto L8c65eaa6; /* mov.l @r15+,r7 */
L8c65eaa6: *stop=0x8c65eaa6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65eaa8; /* mov.l @r15+,r8 */
L8c65eaa8: *stop=0x8c65eaa8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65eaaa; /* mov.l @r15+,r9 */
L8c65eaaa: *stop=0x8c65eaaau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65eaac; /* mov.l @r15+,r10 */
L8c65eaac: *stop=0x8c65eaacu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65eaae; /* mov.l @r15+,r11 */
L8c65eaae: *stop=0x8c65eaaeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65eab0; /* mov.l @r15+,r12 */
L8c65eab0: *stop=0x8c65eab0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65eab2; /* mov.l @r15+,r13 */
L8c65eab2: *stop=0x8c65eab2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;goto L8c65eab4; /* mov.l @r15+,r14 */
L8c65eab4: *stop=0x8c65eab4u;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*macl=value;r[15]+=4u;}goto L8c65eab6; /* lds.l @r15+,MACL */
L8c65eab6: *stop=0x8c65eab6u;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;integer->mach=value;r[15]+=4u;}goto L8c65eab8; /* lds.l @r15+,MACH */
L8c65eab8: *stop=0x8c65eab8u;if(!(*sr&0x40000000u))return -4;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*vbr=value;r[15]+=4u;}goto L8c65eaba; /* ldc.l @r15+,VBR */
L8c65eaba: *stop=0x8c65eabau;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*gbr=value;r[15]+=4u;}goto L8c65eabc; /* ldc.l @r15+,GBR */
L8c65eabc: *stop=0x8c65eabcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65eabe; /* lds.l @r15+,PR */
L8c65eabe: *stop=0x8c65eabeu;if(!(*sr&0x40000000u))return -4;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;status=sa_sh4_exception_ldc(exception,SA_EXCEPTION_SPC,value,*sr,0u);if(status)return status;r[15]+=4u;}goto L8c65eac0; /* ldc.l @r15+,SPC */
L8c65eac0: *stop=0x8c65eac0u;if(!(*sr&0x40000000u))return -4;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;status=sa_sh4_exception_ldc(exception,SA_EXCEPTION_SSR,value,*sr,0u);if(status)return status;r[15]+=4u;}goto L8c65eac2; /* ldc.l @r15+,SSR */
L8c65eac2: if(!(*sr&1u))goto L8c65eac6;goto L8c65eac4; /* bf 0x8c65eac6 */
L8c65eac4: *stop=0x8c65eac4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ead8u,4);if(io_status)return io_status;r[15]=loaded;}goto L8c65eac6; /* mov.l 0x8c65ead8,r15 */
L8c65eac6: *stop=0x8c65eac6u;status=sa_sh4_rte_begin(exception,sr,r,integer,&target,0);if(status)return status;*stop=0x8c65eac8u;exception->returned=1;*stop=target;return 2; /* rte */
}

SA_STAGE_EXPORT int sa_movie_interrupt_context_8c65eaec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t *vbr,struct sa_sh4_exception_state *exception){
 if(!exception)return -2;
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65eaec;
L8c65eaec: *stop=0x8c65eaecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65eba0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65eaee; /* mov.l 0x8c65eba0,r0 */
L8c65eaee: *stop=0x8c65eaeeu;status=sa_sh4_stc_sr(*sr,&r[1],0u);if(status)return status;goto L8c65eaf0; /* stc SR,r1 */
L8c65eaf0: *stop=0x8c65eaf0u;r[1]|=r[0];goto L8c65eaf2; /* or r0,r1 */
L8c65eaf2: *stop=0x8c65eaf2u;status=sa_sh4_ldc_sr(r[1],sr,r,integer,0u);if(status)return status;goto L8c65eaf4; /* ldc r1,SR */
L8c65eaf4: *stop=0x8c65eaf4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ebacu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65eaf6; /* mov.l 0x8c65ebac,r0 */
L8c65eaf6: *stop=0x8c65eaf6u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65eaf8; /* mov.l @r0,r0 */
L8c65eaf8: *stop=0x8c65eaf8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c65eafa; /* tst r0,r0 */
L8c65eafa: take=(*sr&1u);*stop=0x8c65eafcu;*sr&=~1u;if(take)goto L8c65eb08;goto L8c65eafe; /* bt/s 0x8c65eb08 */
L8c65eafe: *stop=0x8c65eafeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65eba4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65eb00; /* mov.l 0x8c65eba4,r0 */
L8c65eb00: *stop=0x8c65eb00u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65eb02; /* mov.l @r0,r1 */
L8c65eb02: *stop=0x8c65eb02u;r[1]+=4294967295u;goto L8c65eb04; /* add #-0x1,r1 */
L8c65eb04: *stop=0x8c65eb04u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c65eb06; /* mov.l r1,@r0 */
L8c65eb06: *stop=0x8c65eb06u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c65eb08; /* tst r1,r1 */
L8c65eb08: *stop=0x8c65eb08u;r[0]=4u;goto L8c65eb0a; /* mov #0x4,r0 */
L8c65eb0a: *stop=0x8c65eb0au;r[0]<<=16;goto L8c65eb0c; /* shll16 r0 */
L8c65eb0c: *stop=0x8c65eb0cu;r[0]+=1u;goto L8c65eb0e; /* add #0x1,r0 */
L8c65eb0e: *stop=0x8c65eb0eu;f->fpscr=r[0]&0x3fffffu;goto L8c65eb10; /* lds r0,fpscr */
L8c65eb10: *stop=0x8c65eb10u;r[0]^=r[0];goto L8c65eb12; /* xor r0,r0 */
L8c65eb12: *stop=0x8c65eb12u;*fpul=r[0];goto L8c65eb14; /* lds r0,FPUL */
L8c65eb14: *stop=0x8c65eb14u;if(*sr&0x8000u)return -4;*stop=0x8c65eb14u;f->fpscr^=0x200000u;goto L8c65eb16; /* frchg */
L8c65eb16: *stop=0x8c65eb16u;if(*sr&0x8000u)return -4;*stop=0x8c65eb16u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb18; /* fmov.s @r15+,fr0 */
L8c65eb18: *stop=0x8c65eb18u;if(*sr&0x8000u)return -4;*stop=0x8c65eb18u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb1a; /* fmov.s @r15+,fr1 */
L8c65eb1a: *stop=0x8c65eb1au;if(*sr&0x8000u)return -4;*stop=0x8c65eb1au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb1c; /* fmov.s @r15+,fr2 */
L8c65eb1c: *stop=0x8c65eb1cu;if(*sr&0x8000u)return -4;*stop=0x8c65eb1cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb1e; /* fmov.s @r15+,fr3 */
L8c65eb1e: *stop=0x8c65eb1eu;if(*sr&0x8000u)return -4;*stop=0x8c65eb1eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb20; /* fmov.s @r15+,fr4 */
L8c65eb20: *stop=0x8c65eb20u;if(*sr&0x8000u)return -4;*stop=0x8c65eb20u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb22; /* fmov.s @r15+,fr5 */
L8c65eb22: *stop=0x8c65eb22u;if(*sr&0x8000u)return -4;*stop=0x8c65eb22u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb24; /* fmov.s @r15+,fr6 */
L8c65eb24: *stop=0x8c65eb24u;if(*sr&0x8000u)return -4;*stop=0x8c65eb24u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb26; /* fmov.s @r15+,fr7 */
L8c65eb26: *stop=0x8c65eb26u;if(*sr&0x8000u)return -4;*stop=0x8c65eb26u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb28; /* fmov.s @r15+,fr8 */
L8c65eb28: *stop=0x8c65eb28u;if(*sr&0x8000u)return -4;*stop=0x8c65eb28u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb2a; /* fmov.s @r15+,fr9 */
L8c65eb2a: *stop=0x8c65eb2au;if(*sr&0x8000u)return -4;*stop=0x8c65eb2au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb2c; /* fmov.s @r15+,fr10 */
L8c65eb2c: *stop=0x8c65eb2cu;if(*sr&0x8000u)return -4;*stop=0x8c65eb2cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb2e; /* fmov.s @r15+,fr11 */
L8c65eb2e: *stop=0x8c65eb2eu;if(*sr&0x8000u)return -4;*stop=0x8c65eb2eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb30; /* fmov.s @r15+,fr12 */
L8c65eb30: *stop=0x8c65eb30u;if(*sr&0x8000u)return -4;*stop=0x8c65eb30u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb32; /* fmov.s @r15+,fr13 */
L8c65eb32: *stop=0x8c65eb32u;if(*sr&0x8000u)return -4;*stop=0x8c65eb32u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb34; /* fmov.s @r15+,fr14 */
L8c65eb34: *stop=0x8c65eb34u;if(*sr&0x8000u)return -4;*stop=0x8c65eb34u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb36; /* fmov.s @r15+,fr15 */
L8c65eb36: *stop=0x8c65eb36u;if(*sr&0x8000u)return -4;*stop=0x8c65eb36u;f->fpscr^=0x200000u;goto L8c65eb38; /* frchg */
L8c65eb38: *stop=0x8c65eb38u;if(*sr&0x8000u)return -4;*stop=0x8c65eb38u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb3a; /* fmov.s @r15+,fr0 */
L8c65eb3a: *stop=0x8c65eb3au;if(*sr&0x8000u)return -4;*stop=0x8c65eb3au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb3c; /* fmov.s @r15+,fr1 */
L8c65eb3c: *stop=0x8c65eb3cu;if(*sr&0x8000u)return -4;*stop=0x8c65eb3cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb3e; /* fmov.s @r15+,fr2 */
L8c65eb3e: *stop=0x8c65eb3eu;if(*sr&0x8000u)return -4;*stop=0x8c65eb3eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb40; /* fmov.s @r15+,fr3 */
L8c65eb40: *stop=0x8c65eb40u;if(*sr&0x8000u)return -4;*stop=0x8c65eb40u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb42; /* fmov.s @r15+,fr4 */
L8c65eb42: *stop=0x8c65eb42u;if(*sr&0x8000u)return -4;*stop=0x8c65eb42u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb44; /* fmov.s @r15+,fr5 */
L8c65eb44: *stop=0x8c65eb44u;if(*sr&0x8000u)return -4;*stop=0x8c65eb44u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb46; /* fmov.s @r15+,fr6 */
L8c65eb46: *stop=0x8c65eb46u;if(*sr&0x8000u)return -4;*stop=0x8c65eb46u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb48; /* fmov.s @r15+,fr7 */
L8c65eb48: *stop=0x8c65eb48u;if(*sr&0x8000u)return -4;*stop=0x8c65eb48u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb4a; /* fmov.s @r15+,fr8 */
L8c65eb4a: *stop=0x8c65eb4au;if(*sr&0x8000u)return -4;*stop=0x8c65eb4au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb4c; /* fmov.s @r15+,fr9 */
L8c65eb4c: *stop=0x8c65eb4cu;if(*sr&0x8000u)return -4;*stop=0x8c65eb4cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb4e; /* fmov.s @r15+,fr10 */
L8c65eb4e: *stop=0x8c65eb4eu;if(*sr&0x8000u)return -4;*stop=0x8c65eb4eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb50; /* fmov.s @r15+,fr11 */
L8c65eb50: *stop=0x8c65eb50u;if(*sr&0x8000u)return -4;*stop=0x8c65eb50u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb52; /* fmov.s @r15+,fr12 */
L8c65eb52: *stop=0x8c65eb52u;if(*sr&0x8000u)return -4;*stop=0x8c65eb52u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb54; /* fmov.s @r15+,fr13 */
L8c65eb54: *stop=0x8c65eb54u;if(*sr&0x8000u)return -4;*stop=0x8c65eb54u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb56; /* fmov.s @r15+,fr14 */
L8c65eb56: *stop=0x8c65eb56u;if(*sr&0x8000u)return -4;*stop=0x8c65eb56u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65eb58; /* fmov.s @r15+,fr15 */
L8c65eb58: *stop=0x8c65eb58u;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;f->fpscr=value&0x3fffffu;r[15]+=4u;}goto L8c65eb5a; /* lds.l @r15+,fpscr */
L8c65eb5a: *stop=0x8c65eb5au;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*fpul=value;r[15]+=4u;}goto L8c65eb5c; /* lds.l @r15+,FPUL */
L8c65eb5c: *stop=0x8c65eb5cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c65eb5e; /* mov.l @r15+,r4 */
L8c65eb5e: *stop=0x8c65eb5eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}r[15]+=4;goto L8c65eb60; /* mov.l @r15+,r0 */
L8c65eb60: *stop=0x8c65eb60u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c65eb62; /* mov.l @r15+,r1 */
L8c65eb62: *stop=0x8c65eb62u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;goto L8c65eb64; /* mov.l @r15+,r2 */
L8c65eb64: *stop=0x8c65eb64u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c65eb66; /* mov.l @r15+,r3 */
L8c65eb66: *stop=0x8c65eb66u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c65eb68; /* mov.l @r15+,r4 */
L8c65eb68: *stop=0x8c65eb68u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}r[15]+=4;goto L8c65eb6a; /* mov.l @r15+,r5 */
L8c65eb6a: *stop=0x8c65eb6au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}r[15]+=4;goto L8c65eb6c; /* mov.l @r15+,r6 */
L8c65eb6c: *stop=0x8c65eb6cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[7]=loaded;}r[15]+=4;goto L8c65eb6e; /* mov.l @r15+,r7 */
L8c65eb6e: *stop=0x8c65eb6eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65eb70; /* mov.l @r15+,r8 */
L8c65eb70: *stop=0x8c65eb70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65eb72; /* mov.l @r15+,r9 */
L8c65eb72: *stop=0x8c65eb72u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65eb74; /* mov.l @r15+,r10 */
L8c65eb74: *stop=0x8c65eb74u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65eb76; /* mov.l @r15+,r11 */
L8c65eb76: *stop=0x8c65eb76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65eb78; /* mov.l @r15+,r12 */
L8c65eb78: *stop=0x8c65eb78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65eb7a; /* mov.l @r15+,r13 */
L8c65eb7a: *stop=0x8c65eb7au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;goto L8c65eb7c; /* mov.l @r15+,r14 */
L8c65eb7c: *stop=0x8c65eb7cu;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*macl=value;r[15]+=4u;}goto L8c65eb7e; /* lds.l @r15+,MACL */
L8c65eb7e: *stop=0x8c65eb7eu;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;integer->mach=value;r[15]+=4u;}goto L8c65eb80; /* lds.l @r15+,MACH */
L8c65eb80: *stop=0x8c65eb80u;if(!(*sr&0x40000000u))return -4;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*vbr=value;r[15]+=4u;}goto L8c65eb82; /* ldc.l @r15+,VBR */
L8c65eb82: *stop=0x8c65eb82u;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;*gbr=value;r[15]+=4u;}goto L8c65eb84; /* ldc.l @r15+,GBR */
L8c65eb84: *stop=0x8c65eb84u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65eb86; /* lds.l @r15+,PR */
L8c65eb86: *stop=0x8c65eb86u;if(!(*sr&0x40000000u))return -4;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;status=sa_sh4_exception_ldc(exception,SA_EXCEPTION_SPC,value,*sr,0u);if(status)return status;r[15]+=4u;}goto L8c65eb88; /* ldc.l @r15+,SPC */
L8c65eb88: *stop=0x8c65eb88u;if(!(*sr&0x40000000u))return -4;if(!valid_access(r[15],4u))return -3;{uint32_t value=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;status=sa_sh4_exception_ldc(exception,SA_EXCEPTION_SSR,value,*sr,0u);if(status)return status;r[15]+=4u;}goto L8c65eb8a; /* ldc.l @r15+,SSR */
L8c65eb8a: if(!(*sr&1u))goto L8c65eb90;goto L8c65eb8c; /* bf 0x8c65eb90 */
L8c65eb8c: *stop=0x8c65eb8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65eba8u,4);if(io_status)return io_status;r[15]=loaded;}goto L8c65eb8e; /* mov.l 0x8c65eba8,r15 */
L8c65eb8e: *stop=0x8c65eb8eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]=loaded;}goto L8c65eb90; /* mov.l @r15,r15 */
L8c65eb90: *stop=0x8c65eb90u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c65eb92; /* mov.l r0,@-r15 */
L8c65eb92: *stop=0x8c65eb92u;r[0]=4294967212u;goto L8c65eb94; /* mov #-0x54,r0 */
L8c65eb94: *stop=0x8c65eb94u;r[0]<<=16;goto L8c65eb96; /* shll16 r0 */
L8c65eb96: *stop=0x8c65eb96u;r[0]<<=8;goto L8c65eb98; /* shll8 r0 */
L8c65eb98: *stop=0x8c65eb98u;r[0]|=0x10u;goto L8c65eb9a; /* or #0x10,r0 */
L8c65eb9a: target=r[0];*stop=0x8c65eb9cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_movie_interrupt_context_8c65e7a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t *vbr,struct sa_sh4_exception_state *exception,uint32_t origin){
 if(!exception)return -2;
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e7a4;
L8c65e7a4: *stop=origin+0x0u;goto L8c65e7a6; /* nop */
L8c65e7a6: *stop=origin+0x2u;goto L8c65e7a8; /* nop */
L8c65e7a8: *stop=origin+0x4u;goto L8c65e7aa; /* nop */
L8c65e7aa: *stop=origin+0x6u;goto L8c65e7ac; /* nop */
L8c65e7ac: *stop=origin+0x8u;goto L8c65e7ae; /* nop */
L8c65e7ae: *stop=origin+0xau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c65e7b0; /* mov.l r1,@-r15 */
L8c65e7b0: *stop=origin+0xcu;r[1]=r[0];goto L8c65e7b2; /* mov r0,r1 */
L8c65e7b2: *stop=origin+0xeu;r[0]=(origin+0x2cu);goto L8c65e7b4; /* mova 0x8c65e7d0,r0 */
L8c65e7b4: *stop=origin+0x10u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c65e7b6; /* mov.l r1,@r0 */
L8c65e7b6: *stop=origin+0x12u;r[1]=4294967295u;goto L8c65e7b8; /* mov #-0x1,r1 */
L8c65e7b8: *stop=origin+0x14u;r[1]<<=16;goto L8c65e7ba; /* shll16 r1 */
L8c65e7ba: *stop=origin+0x16u;r[1]<<=8;goto L8c65e7bc; /* shll8 r1 */
L8c65e7bc: *stop=origin+0x18u;r[1]+=36u;goto L8c65e7be; /* add #0x24,r1 */
L8c65e7be: *stop=origin+0x1au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65e7c0; /* mov.l @r1,r1 */
L8c65e7c0: *stop=origin+0x1cu;{uint32_t loaded=rd(m,bus,&io_status,(origin+0x28u),4);if(io_status)return io_status;r[0]=loaded;}goto L8c65e7c2; /* mov.l 0x8c65e7cc,r0 */
L8c65e7c2: *stop=origin+0x1eu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c65e7c4; /* mov.l r1,@r0 */
L8c65e7c4: *stop=origin+0x20u;{uint32_t loaded=rd(m,bus,&io_status,(origin+0x30u),4);if(io_status)return io_status;r[1]=loaded;}goto L8c65e7c6; /* mov.l 0x8c65e7d4,r1 */
L8c65e7c6: *stop=origin+0x22u;{uint32_t loaded=rd(m,bus,&io_status,(origin+0x2cu),4);if(io_status)return io_status;r[0]=loaded;}goto L8c65e7c8; /* mov.l 0x8c65e7d0,r0 */
L8c65e7c8: target=r[1];*stop=origin+0x26u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r1 */
}

SA_STAGE_EXPORT int sa_movie_interrupt_context_8c65e7d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t *vbr,struct sa_sh4_exception_state *exception,uint32_t origin){
 if(!exception)return -2;
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e7d8;
L8c65e7d8: *stop=origin+0x0u;goto L8c65e7da; /* nop */
L8c65e7da: *stop=origin+0x2u;goto L8c65e7dc; /* nop */
L8c65e7dc: *stop=origin+0x4u;goto L8c65e7de; /* nop */
L8c65e7de: *stop=origin+0x6u;goto L8c65e7e0; /* nop */
L8c65e7e0: *stop=origin+0x8u;goto L8c65e7e2; /* nop */
L8c65e7e2: *stop=origin+0xau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c65e7e4; /* mov.l r1,@-r15 */
L8c65e7e4: *stop=origin+0xcu;r[1]=r[0];goto L8c65e7e6; /* mov r0,r1 */
L8c65e7e6: *stop=origin+0xeu;r[0]=(origin+0x2cu);goto L8c65e7e8; /* mova 0x8c65e804,r0 */
L8c65e7e8: *stop=origin+0x10u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c65e7ea; /* mov.l r1,@r0 */
L8c65e7ea: *stop=origin+0x12u;r[1]=4294967295u;goto L8c65e7ec; /* mov #-0x1,r1 */
L8c65e7ec: *stop=origin+0x14u;r[1]<<=16;goto L8c65e7ee; /* shll16 r1 */
L8c65e7ee: *stop=origin+0x16u;r[1]<<=8;goto L8c65e7f0; /* shll8 r1 */
L8c65e7f0: *stop=origin+0x18u;r[1]+=40u;goto L8c65e7f2; /* add #0x28,r1 */
L8c65e7f2: *stop=origin+0x1au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65e7f4; /* mov.l @r1,r1 */
L8c65e7f4: *stop=origin+0x1cu;{uint32_t loaded=rd(m,bus,&io_status,(origin+0x28u),4);if(io_status)return io_status;r[0]=loaded;}goto L8c65e7f6; /* mov.l 0x8c65e800,r0 */
L8c65e7f6: *stop=origin+0x1eu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c65e7f8; /* mov.l r1,@r0 */
L8c65e7f8: *stop=origin+0x20u;{uint32_t loaded=rd(m,bus,&io_status,(origin+0x30u),4);if(io_status)return io_status;r[1]=loaded;}goto L8c65e7fa; /* mov.l 0x8c65e808,r1 */
L8c65e7fa: *stop=origin+0x22u;{uint32_t loaded=rd(m,bus,&io_status,(origin+0x2cu),4);if(io_status)return io_status;r[0]=loaded;}goto L8c65e7fc; /* mov.l 0x8c65e804,r0 */
L8c65e7fc: target=r[1];*stop=origin+0x26u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r1 */
}
