/* Geometry projection, sampling and view proposals; explicit GBR/queue state.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
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
 if(n==4u && ((a&0x1fffffffu)==0x00710000u || (a&0x1fffffffu)==0x00710004u))return 1;
 if((n==1u||n==2u||n==4u) && (a&0x1fffffffu)>=0x00702c00u && (a&0x1fffffffu)+n<=0x00702c04u)return 1;
 if(n==4u && (a==0xff00001cu||a==0xffa00040u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f6800u && (a&0x1fffffffu)<0x005f7d00u)return 1;
 if(n==4u && !(a&31u) && a>=0xf4000000u && a<0xf4004000u)return 1;
 if(n==2u && (a==0xffd00000u||a==0xffd00004u||a==0xffd00008u||a==0xffd0000cu||a==0xffd80010u))return 1;
 if(n==1u && (a==0xffd80000u||a==0xffd80004u))return 1;
 if(n==4u && (a==0xffd80008u||a==0xffd8000cu))return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_disc_init_8c600032(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c600032;
L8c600032: *stop=0x8c600032u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6000e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c600034; /* mov.l 0x8c6000e0,r3 */
L8c600034: target=r[3];*stop=0x8c600036u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c600038(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c600038;
L8c5ffd28: *stop=0x8c5ffd28u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c5ffd2a; /* mov.l r14,@-r15 */
L8c5ffd2a: *stop=0x8c5ffd2au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c5ffd2c; /* mov.l r13,@-r15 */
L8c5ffd2c: *stop=0x8c5ffd2cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c5ffd2e; /* mov.l r12,@-r15 */
L8c5ffd2e: *stop=0x8c5ffd2eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c5ffd30; /* mov.l r11,@-r15 */
L8c5ffd30: *stop=0x8c5ffd30u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c5ffd32; /* mov.l r10,@-r15 */
L8c5ffd32: *stop=0x8c5ffd32u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c5ffd34; /* mov.l r9,@-r15 */
L8c5ffd34: *stop=0x8c5ffd34u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c5ffd36; /* mov.l r8,@-r15 */
L8c5ffd36: *stop=0x8c5ffd36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c5ffd38; /* sts.l PR,@-r15 */
L8c5ffd38: *stop=0x8c5ffd38u;r[15]+=4294967264u;goto L8c5ffd3a; /* add #-0x20,r15 */
L8c5ffd3a: *stop=0x8c5ffd3au;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5ffd3c; /* stc SR,r0 */
L8c5ffd3c: *stop=0x8c5ffd3cu;r[10]=r[15];goto L8c5ffd3e; /* mov r15,r10 */
L8c5ffd3e: *stop=0x8c5ffd3eu;r[10]+=24u;goto L8c5ffd40; /* add #0x18,r10 */
L8c5ffd40: *stop=0x8c5ffd40u;r[3]=r[10];goto L8c5ffd42; /* mov r10,r3 */
L8c5ffd42: *stop=0x8c5ffd42u;r[3]+=4u;goto L8c5ffd44; /* add #0x4,r3 */
L8c5ffd44: *stop=0x8c5ffd44u;r[0]>>=2;goto L8c5ffd46; /* shlr2 r0 */
L8c5ffd46: *stop=0x8c5ffd46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffe2cu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c5ffd48; /* mov.l 0x8c5ffe2c,r8 */
L8c5ffd48: *stop=0x8c5ffd48u;r[0]>>=2;goto L8c5ffd4a; /* shlr2 r0 */
L8c5ffd4a: *stop=0x8c5ffd4au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c5ffd4c; /* mov.l r3,@(0x8,r15) */
L8c5ffd4c: *stop=0x8c5ffd4cu;r[0]&=0xfu;goto L8c5ffd4e; /* and #0xf,r0 */
L8c5ffd4e: *stop=0x8c5ffd4eu;r[4]=r[8];goto L8c5ffd50; /* mov r8,r4 */
L8c5ffd50: *stop=0x8c5ffd50u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5ffe22u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffd52; /* mov.w 0x8c5ffe22,r3 */
L8c5ffd52: *stop=0x8c5ffd52u;r[2]=r[8];goto L8c5ffd54; /* mov r8,r2 */
L8c5ffd54: *stop=0x8c5ffd54u;r[1]=r[4];goto L8c5ffd56; /* mov r4,r1 */
L8c5ffd56: *stop=0x8c5ffd56u;r[1]+=14u;goto L8c5ffd58; /* add #0xe,r1 */
L8c5ffd58: *stop=0x8c5ffd58u;r[2]+=12u;goto L8c5ffd5a; /* add #0xc,r2 */
L8c5ffd5a: *stop=0x8c5ffd5au;r[4]+=15u;goto L8c5ffd5c; /* add #0xf,r4 */
L8c5ffd5c: *stop=0x8c5ffd5cu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c5ffd5e; /* mov.l r2,@(0x10,r15) */
L8c5ffd5e: *stop=0x8c5ffd5eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c5ffd60; /* mov.l r1,@r15 */
L8c5ffd60: *stop=0x8c5ffd60u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c5ffd62; /* mov.l r4,@(0x4,r15) */
L8c5ffd62: *stop=0x8c5ffd62u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c5ffd64; /* mov.l r0,@(0xc,r15) */
L8c5ffd64: *stop=0x8c5ffd64u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5ffd66; /* stc SR,r0 */
L8c5ffd66: *stop=0x8c5ffd66u;r[0]&=r[3];goto L8c5ffd68; /* and r3,r0 */
L8c5ffd68: *stop=0x8c5ffd68u;r[0]|=0xf0u;goto L8c5ffd6a; /* or #0xf0,r0 */
L8c5ffd6a: *stop=0x8c5ffd6au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c5ffd6c; /* ldc r0,SR */
L8c5ffd6c: *stop=0x8c5ffd6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffe30u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c5ffd6e; /* mov.l 0x8c5ffe30,r12 */
L8c5ffd6e: *stop=0x8c5ffd6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffe38u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c5ffd70; /* mov.l 0x8c5ffe38,r14 */
L8c5ffd70: *stop=0x8c5ffd70u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffd72; /* mov.l @r12,r2 */
L8c5ffd72: *stop=0x8c5ffd72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffe34u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c5ffd74; /* mov.l 0x8c5ffe34,r13 */
L8c5ffd74: *stop=0x8c5ffd74u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c5ffd76; /* tst r2,r2 */
L8c5ffd76: if(!(*sr&1u))goto L8c5ffd86;goto L8c5ffd78; /* bf 0x8c5ffd86 */
L8c5ffd78: *stop=0x8c5ffd78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffe3cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c5ffd7a; /* mov.l 0x8c5ffe3c,r5 */
L8c5ffd7a: *stop=0x8c5ffd7au;r[6]=80u;goto L8c5ffd7c; /* mov #0x50,r6 */
L8c5ffd7c: *stop=0x8c5ffd7cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[7]=loaded;}goto L8c5ffd7e; /* mov.l @r14,r7 */
L8c5ffd7e: *stop=0x8c5ffd7eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c5ffe24u,2);if(io_status)return io_status;r[11]=loaded;}goto L8c5ffd80; /* mov.w 0x8c5ffe24,r11 */
L8c5ffd80: target=r[13];*pr=0x8c5ffd84u;*stop=0x8c5ffd82u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffd84; /* jsr @r13 */
L8c5ffd84: *stop=0x8c5ffd84u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[0],4);if(io_status)return io_status;goto L8c5ffd86; /* mov.l r0,@r12 */
L8c5ffd86: *stop=0x8c5ffd86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffe40u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c5ffd88; /* mov.l 0x8c5ffe40,r12 */
L8c5ffd88: *stop=0x8c5ffd88u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffd8a; /* mov.l @r12,r3 */
L8c5ffd8a: *stop=0x8c5ffd8au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c5ffd8c; /* tst r3,r3 */
L8c5ffd8c: if(!(*sr&1u))goto L8c5ffd9c;goto L8c5ffd8e; /* bf 0x8c5ffd9c */
L8c5ffd8e: *stop=0x8c5ffd8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffe44u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c5ffd90; /* mov.l 0x8c5ffe44,r5 */
L8c5ffd90: *stop=0x8c5ffd90u;r[6]=16u;goto L8c5ffd92; /* mov #0x10,r6 */
L8c5ffd92: *stop=0x8c5ffd92u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[7]=loaded;}goto L8c5ffd94; /* mov.l @r14,r7 */
L8c5ffd94: *stop=0x8c5ffd94u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5ffe26u,2);if(io_status)return io_status;r[11]=loaded;}goto L8c5ffd96; /* mov.w 0x8c5ffe26,r11 */
L8c5ffd96: target=r[13];*pr=0x8c5ffd9au;*stop=0x8c5ffd98u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffd9a; /* jsr @r13 */
L8c5ffd9a: *stop=0x8c5ffd9au;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[0],4);if(io_status)return io_status;goto L8c5ffd9c; /* mov.l r0,@r12 */
L8c5ffd9c: *stop=0x8c5ffd9cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffd9e; /* mov.l @r14,r4 */
L8c5ffd9e: *stop=0x8c5ffd9eu;r[7]=0u;goto L8c5ffda0; /* mov #0x0,r7 */
L8c5ffda0: *stop=0x8c5ffda0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffe48u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c5ffda2; /* mov.l 0x8c5ffe48,r12 */
L8c5ffda2: *stop=0x8c5ffda2u;r[5]=r[7];goto L8c5ffda4; /* mov r7,r5 */
L8c5ffda4: *stop=0x8c5ffda4u;r[6]=r[7];goto L8c5ffda6; /* mov r7,r6 */
L8c5ffda6: target=r[12];*pr=0x8c5ffdaau;*stop=0x8c5ffda8u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffdaa; /* jsr @r12 */
L8c5ffdaa: *stop=0x8c5ffdaau;r[13]=r[0];goto L8c5ffdac; /* mov r0,r13 */
L8c5ffdac: *stop=0x8c5ffdacu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c5ffdae; /* tst r13,r13 */
L8c5ffdae: if((*sr&1u))goto L8c5ffe1e;goto L8c5ffdb0; /* bt 0x8c5ffe1e */
L8c5ffdb0: *stop=0x8c5ffdb0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffdb2; /* mov.l @r14,r2 */
L8c5ffdb2: *stop=0x8c5ffdb2u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffdb4; /* mov.l @(0xc,r2),r3 */
L8c5ffdb4: *stop=0x8c5ffdb4u;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffdb6; /* mov.l @(0x1c,r3),r1 */
L8c5ffdb6: target=r[1];*pr=0x8c5ffdbau;*stop=0x8c5ffdb8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffdba; /* jsr @r1 */
L8c5ffdba: *stop=0x8c5ffdbau;r[4]=r[0];goto L8c5ffdbc; /* mov r0,r4 */
L8c5ffdbc: *stop=0x8c5ffdbcu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ffdbe; /* tst r4,r4 */
L8c5ffdbe: if((*sr&1u))goto L8c5ffe1e;goto L8c5ffdc0; /* bt 0x8c5ffe1e */
L8c5ffdc0: *stop=0x8c5ffdc0u;r[0]=80u;goto L8c5ffdc2; /* mov #0x50,r0 */
L8c5ffdc2: *stop=0x8c5ffdc2u;if(!valid_access(r[13]+60u,4u))return -3;wr(m,bus,&io_status,r[13]+0x3cu,r[4],4);if(io_status)return io_status;goto L8c5ffdc4; /* mov.l r4,@(0x3c,r13) */
L8c5ffdc4: *stop=0x8c5ffdc4u;r[9]=4u;goto L8c5ffdc6; /* mov #0x4,r9 */
L8c5ffdc6: *stop=0x8c5ffdc6u;if(!valid_access(r[0]+r[13],2u))return -3;wr(m,bus,&io_status,r[0]+r[13],r[9],2);if(io_status)return io_status;goto L8c5ffdc8; /* mov.w r9,@(r0,r13) */
L8c5ffdc8: *stop=0x8c5ffdc8u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffdca; /* mov.l @r13,r3 */
L8c5ffdca: *stop=0x8c5ffdcau;if(!valid_access(r[3]+28u,4u))return -3;wr(m,bus,&io_status,r[3]+0x1cu,r[13],4);if(io_status)return io_status;goto L8c5ffdcc; /* mov.l r13,@(0x1c,r3) */
L8c5ffdcc: *stop=0x8c5ffdccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffe4cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffdce; /* mov.l 0x8c5ffe4c,r2 */
L8c5ffdce: target=r[2];*pr=0x8c5ffdd2u;*stop=0x8c5ffdd0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffdd2; /* jsr @r2 */
L8c5ffdd2: *stop=0x8c5ffdd2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffdd4; /* mov.l @r14,r3 */
L8c5ffdd4: *stop=0x8c5ffdd4u;r[11]=r[0];goto L8c5ffdd6; /* mov r0,r11 */
L8c5ffdd6: *stop=0x8c5ffdd6u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffdd8; /* mov.l @(0xc,r3),r2 */
L8c5ffdd8: *stop=0x8c5ffdd8u;if(!valid_access(r[2]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffdda; /* mov.l @(0x20,r2),r1 */
L8c5ffdda: target=r[1];*pr=0x8c5ffddeu;*stop=0x8c5ffddcu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffdde; /* jsr @r1 */
L8c5ffdde: *stop=0x8c5ffddeu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c5ffde0; /* tst r0,r0 */
L8c5ffde0: if((*sr&1u))goto L8c5ffe1e;goto L8c5ffde2; /* bt 0x8c5ffe1e */
L8c5ffde2: *stop=0x8c5ffde2u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c5ffde4; /* tst r11,r11 */
L8c5ffde4: if((*sr&1u))goto L8c5ffdfe;goto L8c5ffde6; /* bt 0x8c5ffdfe */
L8c5ffde6: *stop=0x8c5ffde6u;r[0]=r[11];goto L8c5ffde8; /* mov r11,r0 */
L8c5ffde8: *stop=0x8c5ffde8u;*sr=(*sr&~1u)|(r[0]==0xffffffe3u);goto L8c5ffdea; /* cmp/eq #-0x1d,r0 */
L8c5ffdea: if((*sr&1u))goto L8c5ffdf0;goto L8c5ffdec; /* bt 0x8c5ffdf0 */
L8c5ffdec: *stop=0x8c5ffdeeu;goto L8c5fffde; /* bra 0x8c5fffde */
L8c5ffdf0: *stop=0x8c5ffdf0u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c5ffdf2; /* mov.l @(0x18,r15),r0 */
L8c5ffdf2: *stop=0x8c5ffdf2u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c5ffdf4; /* cmp/eq #0x6,r0 */
L8c5ffdf4: if((*sr&1u))goto L8c5ffdfa;goto L8c5ffdf6; /* bt 0x8c5ffdfa */
L8c5ffdf6: *stop=0x8c5ffdf8u;goto L8c5fffde; /* bra 0x8c5fffde */
L8c5ffdfa: *stop=0x8c5ffdfcu;r[5]=4294967273u;goto L8c5ffff6; /* bra 0x8c5ffff6 */
L8c5ffdfe: *stop=0x8c5ffdfeu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffe00; /* mov.l @(0x8,r15),r2 */
L8c5ffe00: *stop=0x8c5ffe00u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5ffe28u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffe02; /* mov.w 0x8c5ffe28,r4 */
L8c5ffe02: *stop=0x8c5ffe02u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffe04; /* mov.l @r2,r3 */
L8c5ffe04: *stop=0x8c5ffe04u;*sr=(*sr&~1u)|(r[4]==r[3]);goto L8c5ffe06; /* cmp/eq r4,r3 */
L8c5ffe06: if((*sr&1u))goto L8c5ffe0c;goto L8c5ffe08; /* bt 0x8c5ffe0c */
L8c5ffe08: *stop=0x8c5ffe0au;goto L8c5ffff4; /* bra 0x8c5ffff4 */
L8c5ffe0c: *stop=0x8c5ffe0cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffe0e; /* mov.l @r14,r4 */
L8c5ffe0e: *stop=0x8c5ffe0eu;r[7]=0u;goto L8c5ffe10; /* mov #0x0,r7 */
L8c5ffe10: *stop=0x8c5ffe10u;r[5]=r[7];goto L8c5ffe12; /* mov r7,r5 */
L8c5ffe12: *stop=0x8c5ffe12u;r[6]=r[7];goto L8c5ffe14; /* mov r7,r6 */
L8c5ffe14: target=r[12];*pr=0x8c5ffe18u;*stop=0x8c5ffe16u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffe18; /* jsr @r12 */
L8c5ffe18: *stop=0x8c5ffe18u;r[12]=r[0];goto L8c5ffe1a; /* mov r0,r12 */
L8c5ffe1a: *stop=0x8c5ffe1au;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c5ffe1c; /* tst r12,r12 */
L8c5ffe1c: if(!(*sr&1u))goto L8c5ffe50;goto L8c5ffe1e; /* bf 0x8c5ffe50 */
L8c5ffe1e: *stop=0x8c5ffe20u;r[5]=4294967294u;goto L8c5ffff6; /* bra 0x8c5ffff6 */
L8c5ffe50: *stop=0x8c5ffe50u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffe52; /* mov.l @r14,r3 */
L8c5ffe52: *stop=0x8c5ffe52u;r[13]=0u;goto L8c5ffe54; /* mov #0x0,r13 */
L8c5ffe54: *stop=0x8c5ffe54u;r[0]=76u;goto L8c5ffe56; /* mov #0x4c,r0 */
L8c5ffe56: *stop=0x8c5ffe56u;if(!valid_access(r[3]+44u,4u))return -3;wr(m,bus,&io_status,r[3]+0x2cu,r[13],4);if(io_status)return io_status;goto L8c5ffe58; /* mov.l r13,@(0x2c,r3) */
L8c5ffe58: *stop=0x8c5ffe58u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffe5a; /* mov.l @r14,r2 */
L8c5ffe5a: *stop=0x8c5ffe5au;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffe5c; /* mov.l @(0xc,r2),r3 */
L8c5ffe5c: *stop=0x8c5ffe5cu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffe5e; /* mov.l @(r0,r3),r1 */
L8c5ffe5e: target=r[1];*pr=0x8c5ffe62u;*stop=0x8c5ffe60u;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffe62; /* jsr @r1 */
L8c5ffe62: *stop=0x8c5ffe62u;r[4]=r[0];goto L8c5ffe64; /* mov r0,r4 */
L8c5ffe64: *stop=0x8c5ffe64u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ffe66; /* tst r4,r4 */
L8c5ffe66: if((*sr&1u))goto L8c5ffeac;goto L8c5ffe68; /* bt 0x8c5ffeac */
L8c5ffe68: *stop=0x8c5ffe68u;r[0]=80u;goto L8c5ffe6a; /* mov #0x50,r0 */
L8c5ffe6a: *stop=0x8c5ffe6au;if(!valid_access(r[12]+60u,4u))return -3;wr(m,bus,&io_status,r[12]+0x3cu,r[4],4);if(io_status)return io_status;goto L8c5ffe6c; /* mov.l r4,@(0x3c,r12) */
L8c5ffe6c: *stop=0x8c5ffe6cu;if(!valid_access(r[0]+r[12],2u))return -3;wr(m,bus,&io_status,r[0]+r[12],r[9],2);if(io_status)return io_status;goto L8c5ffe6e; /* mov.w r9,@(r0,r12) */
L8c5ffe6e: *stop=0x8c5ffe6eu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffe70; /* mov.l @r12,r3 */
L8c5ffe70: *stop=0x8c5ffe70u;if(!valid_access(r[3]+28u,4u))return -3;wr(m,bus,&io_status,r[3]+0x1cu,r[12],4);if(io_status)return io_status;goto L8c5ffe72; /* mov.l r12,@(0x1c,r3) */
L8c5ffe72: *stop=0x8c5ffe72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fff94u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffe74; /* mov.l 0x8c5fff94,r2 */
L8c5ffe74: target=r[2];*pr=0x8c5ffe78u;*stop=0x8c5ffe76u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffe78; /* jsr @r2 */
L8c5ffe78: *stop=0x8c5ffe78u;r[4]=r[0];goto L8c5ffe7a; /* mov r0,r4 */
L8c5ffe7a: *stop=0x8c5ffe7au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ffe7c; /* tst r4,r4 */
L8c5ffe7c: if(!(*sr&1u))goto L8c5ffeac;goto L8c5ffe7e; /* bf 0x8c5ffeac */
L8c5ffe7e: *stop=0x8c5ffe7eu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffe80; /* mov.l @(0x10,r15),r2 */
L8c5ffe80: *stop=0x8c5ffe80u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffe82; /* mov.l @r14,r3 */
L8c5ffe82: *stop=0x8c5ffe82u;if(!valid_access(r[2],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffe84; /* mov.b @r2,r1 */
L8c5ffe84: *stop=0x8c5ffe84u;if(!valid_access(r[3]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x2cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffe86; /* mov.l @(0x2c,r3),r2 */
L8c5ffe86: *stop=0x8c5ffe86u;r[1]+=4294967248u;goto L8c5ffe88; /* add #-0x30,r1 */
L8c5ffe88: *stop=0x8c5ffe88u;r[1]<<=16;goto L8c5ffe8a; /* shll16 r1 */
L8c5ffe8a: *stop=0x8c5ffe8au;r[2]|=r[1];goto L8c5ffe8c; /* or r1,r2 */
L8c5ffe8c: *stop=0x8c5ffe8cu;if(!valid_access(r[3]+44u,4u))return -3;wr(m,bus,&io_status,r[3]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c5ffe8e; /* mov.l r2,@(0x2c,r3) */
L8c5ffe8e: *stop=0x8c5ffe8eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffe90; /* mov.l @r15,r2 */
L8c5ffe90: *stop=0x8c5ffe90u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffe92; /* mov.l @r14,r3 */
L8c5ffe92: *stop=0x8c5ffe92u;if(!valid_access(r[2],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffe94; /* mov.b @r2,r1 */
L8c5ffe94: *stop=0x8c5ffe94u;if(!valid_access(r[3]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x2cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffe96; /* mov.l @(0x2c,r3),r2 */
L8c5ffe96: *stop=0x8c5ffe96u;r[1]+=4294967248u;goto L8c5ffe98; /* add #-0x30,r1 */
L8c5ffe98: *stop=0x8c5ffe98u;r[1]<<=8;goto L8c5ffe9a; /* shll8 r1 */
L8c5ffe9a: *stop=0x8c5ffe9au;r[2]|=r[1];goto L8c5ffe9c; /* or r1,r2 */
L8c5ffe9c: *stop=0x8c5ffe9cu;if(!valid_access(r[3]+44u,4u))return -3;wr(m,bus,&io_status,r[3]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c5ffe9e; /* mov.l r2,@(0x2c,r3) */
L8c5ffe9e: *stop=0x8c5ffe9eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffea0; /* mov.l @(0x4,r15),r2 */
L8c5ffea0: *stop=0x8c5ffea0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffea2; /* mov.l @r14,r3 */
L8c5ffea2: *stop=0x8c5ffea2u;if(!valid_access(r[2],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffea4; /* mov.b @r2,r1 */
L8c5ffea4: *stop=0x8c5ffea4u;if(!valid_access(r[3]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x2cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffea6; /* mov.l @(0x2c,r3),r2 */
L8c5ffea6: *stop=0x8c5ffea6u;r[1]+=4294967248u;goto L8c5ffea8; /* add #-0x30,r1 */
L8c5ffea8: *stop=0x8c5ffea8u;r[2]|=r[1];goto L8c5ffeaa; /* or r1,r2 */
L8c5ffeaa: *stop=0x8c5ffeaau;if(!valid_access(r[3]+44u,4u))return -3;wr(m,bus,&io_status,r[3]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c5ffeac; /* mov.l r2,@(0x2c,r3) */
L8c5ffeac: *stop=0x8c5ffeacu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[6]=loaded;}goto L8c5ffeae; /* mov.l @r14,r6 */
L8c5ffeae: *stop=0x8c5ffeaeu;r[5]=1u;goto L8c5ffeb0; /* mov #0x1,r5 */
L8c5ffeb0: *stop=0x8c5ffeb0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fff98u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c5ffeb2; /* mov.l 0x8c5fff98,r12 */
L8c5ffeb2: *stop=0x8c5ffeb2u;r[6]+=64u;goto L8c5ffeb4; /* add #0x40,r6 */
L8c5ffeb4: target=0x8c5ffc98u;*pr=0x8c5ffeb8u;*stop=0x8c5ffeb6u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffeb8; /* bsr 0x8c5ffc98 */
L8c5ffeb8: *stop=0x8c5ffeb8u;r[4]=r[0];goto L8c5ffeba; /* mov r0,r4 */
L8c5ffeba: *stop=0x8c5ffebau;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ffebc; /* tst r4,r4 */
L8c5ffebc: if((*sr&1u))goto L8c5ffec2;goto L8c5ffebe; /* bt 0x8c5ffec2 */
L8c5ffebe: *stop=0x8c5ffec0u;goto L8c5fffe6; /* bra 0x8c5fffe6 */
L8c5ffec2: *stop=0x8c5ffec2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffec4; /* mov.l @r14,r4 */
L8c5ffec4: target=0x8c5ff9e6u;*pr=0x8c5ffec8u;*stop=0x8c5ffec6u;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffec8; /* bsr 0x8c5ff9e6 */
L8c5ffec8: *stop=0x8c5ffec8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c5ffeca; /* tst r0,r0 */
L8c5ffeca: if(!(*sr&1u))goto L8c5ffed0;goto L8c5ffecc; /* bf 0x8c5ffed0 */
L8c5ffecc: *stop=0x8c5ffeceu;goto L8c5ffff4; /* bra 0x8c5ffff4 */
L8c5ffed0: *stop=0x8c5ffed0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffed2; /* mov.l @r14,r2 */
L8c5ffed2: *stop=0x8c5ffed2u;r[1]=4u;goto L8c5ffed4; /* mov #0x4,r1 */
L8c5ffed4: *stop=0x8c5ffed4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fff8au,2);if(io_status)return io_status;r[12]=loaded;}goto L8c5ffed6; /* mov.w 0x8c5fff8a,r12 */
L8c5ffed6: *stop=0x8c5ffed6u;r[1]+=r[2];goto L8c5ffed8; /* add r2,r1 */
L8c5ffed8: *stop=0x8c5ffed8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c5ffeda; /* mov.l r1,@-r15 */
L8c5ffeda: *stop=0x8c5ffedau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffedc; /* mov.l @r14,r4 */
L8c5ffedc: *stop=0x8c5ffedcu;r[4]+=64u;goto L8c5ffede; /* add #0x40,r4 */
L8c5ffede: target=0x8c5ff9c0u;*pr=0x8c5ffee2u;*stop=0x8c5ffee0u;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffee2; /* bsr 0x8c5ff9c0 */
L8c5ffee2: *stop=0x8c5ffee2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c5ffee4; /* mov.l @r15+,r3 */
L8c5ffee4: *stop=0x8c5ffee4u;r[1]=8u;goto L8c5ffee6; /* mov #0x8,r1 */
L8c5ffee6: *stop=0x8c5ffee6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fff8cu,2);if(io_status)return io_status;r[11]=loaded;}goto L8c5ffee8; /* mov.w 0x8c5fff8c,r11 */
L8c5ffee8: *stop=0x8c5ffee8u;r[12]+=4294967288u;goto L8c5ffeea; /* add #-0x8,r12 */
L8c5ffeea: *stop=0x8c5ffeeau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c5ffeec; /* mov.l r0,@r3 */
L8c5ffeec: *stop=0x8c5ffeecu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffeee; /* mov.l @r14,r2 */
L8c5ffeee: *stop=0x8c5ffeeeu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffef0; /* mov.l @(0x4,r2),r3 */
L8c5ffef0: *stop=0x8c5ffef0u;r[3]+=r[11];goto L8c5ffef2; /* add r11,r3 */
L8c5ffef2: *stop=0x8c5ffef2u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c5ffef4; /* mov.l r3,@(0x4,r2) */
L8c5ffef4: *stop=0x8c5ffef4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffef6; /* mov.l @r14,r2 */
L8c5ffef6: *stop=0x8c5ffef6u;r[1]+=r[2];goto L8c5ffef8; /* add r2,r1 */
L8c5ffef8: *stop=0x8c5ffef8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c5ffefa; /* mov.l r1,@-r15 */
L8c5ffefa: *stop=0x8c5ffefau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffefc; /* mov.l @r14,r4 */
L8c5ffefc: *stop=0x8c5ffefcu;r[4]+=64u;goto L8c5ffefe; /* add #0x40,r4 */
L8c5ffefe: target=0x8c5ff9c0u;*pr=0x8c5fff02u;*stop=0x8c5fff00u;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fff02; /* bsr 0x8c5ff9c0 */
L8c5fff02: *stop=0x8c5fff02u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c5fff04; /* mov.l @r15+,r3 */
L8c5fff04: *stop=0x8c5fff04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fffa4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c5fff06; /* mov.l 0x8c5fffa4,r4 */
L8c5fff06: *stop=0x8c5fff06u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c5fff08; /* mov.l r0,@r3 */
L8c5fff08: *stop=0x8c5fff08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fffa0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fff0a; /* mov.l 0x8c5fffa0,r2 */
L8c5fff0a: *stop=0x8c5fff0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fff9cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c5fff0c; /* mov.l 0x8c5fff9c,r12 */
L8c5fff0c: *stop=0x8c5fff0cu;r[3]=r[2];goto L8c5fff0e; /* mov r2,r3 */
L8c5fff0e: *stop=0x8c5fff0eu;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[2],4);if(io_status)return io_status;goto L8c5fff10; /* mov.l r2,@r12 */
L8c5fff10: *stop=0x8c5fff10u;if(!valid_access(r[4]+12u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0xcu,1);if(io_status)return io_status;r[0]=loaded;}goto L8c5fff12; /* mov.b @(0xc,r4),r0 */
L8c5fff12: *stop=0x8c5fff12u;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c5fff14; /* mov.w r0,@r3 */
L8c5fff14: *stop=0x8c5fff14u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[0]=loaded;}goto L8c5fff16; /* mov.l @r12,r0 */
L8c5fff16: *stop=0x8c5fff16u;if(!valid_access(r[0],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;r[0]=loaded;}goto L8c5fff18; /* mov.w @r0,r0 */
L8c5fff18: *stop=0x8c5fff18u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c5fff1a; /* cmp/eq #0x1,r0 */
L8c5fff1a: if(!(*sr&1u))goto L8c5fff44;goto L8c5fff1c; /* bf 0x8c5fff44 */
L8c5fff1c: *stop=0x8c5fff1cu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fff1e; /* mov.l @r12,r2 */
L8c5fff1e: *stop=0x8c5fff1eu;r[5]=r[4];goto L8c5fff20; /* mov r4,r5 */
L8c5fff20: *stop=0x8c5fff20u;if(!valid_access(r[4]+13u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0xdu,1);if(io_status)return io_status;r[0]=loaded;}goto L8c5fff22; /* mov.b @(0xd,r4),r0 */
L8c5fff22: *stop=0x8c5fff22u;r[4]=r[13];goto L8c5fff24; /* mov r13,r4 */
L8c5fff24: *stop=0x8c5fff24u;if(!valid_access(r[2]+2u,2u))return -3;wr(m,bus,&io_status,r[2]+0x2u,r[0],2);if(io_status)return io_status;goto L8c5fff26; /* mov.w r0,@(0x2,r2) */
L8c5fff26: *stop=0x8c5fff26u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fff28; /* mov.l @r12,r3 */
L8c5fff28: *stop=0x8c5fff28u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[13],4);if(io_status)return io_status;goto L8c5fff2a; /* mov.l r13,@(0x4,r3) */
L8c5fff2a: *stop=0x8c5fff2au;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[6]=loaded;}goto L8c5fff2c; /* mov.l @r12,r6 */
L8c5fff2c: *stop=0x8c5fff2cu;r[6]+=8u;goto L8c5fff2e; /* add #0x8,r6 */
L8c5fff2e: *stop=0x8c5fff2eu;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[0]=loaded;}goto L8c5fff30; /* mov.b @r5,r0 */
L8c5fff30: *stop=0x8c5fff30u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c5fff32; /* cmp/eq #0x20,r0 */
L8c5fff32: if((*sr&1u))goto L8c5fff42;goto L8c5fff34; /* bt 0x8c5fff42 */
L8c5fff34: *stop=0x8c5fff34u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[2]=loaded;}r[5]+=1;goto L8c5fff36; /* mov.b @r5+,r2 */
L8c5fff36: *stop=0x8c5fff36u;r[7]=12u;goto L8c5fff38; /* mov #0xc,r7 */
L8c5fff38: *stop=0x8c5fff38u;r[4]+=1u;goto L8c5fff3a; /* add #0x1,r4 */
L8c5fff3a: *stop=0x8c5fff3au;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[7]);goto L8c5fff3c; /* cmp/ge r7,r4 */
L8c5fff3c: *stop=0x8c5fff3cu;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[2],1);if(io_status)return io_status;goto L8c5fff3e; /* mov.b r2,@r6 */
L8c5fff3e: take=!(*sr&1u);*stop=0x8c5fff40u;r[6]+=1u;if(take)goto L8c5fff2e;goto L8c5fff42; /* bf/s 0x8c5fff2e */
L8c5fff42: *stop=0x8c5fff42u;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[13],1);if(io_status)return io_status;goto L8c5fff44; /* mov.b r13,@r6 */
L8c5fff44: *stop=0x8c5fff44u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5fff46; /* mov.l @r14,r4 */
L8c5fff46: *stop=0x8c5fff46u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fff8eu,2);if(io_status)return io_status;r[9]=loaded;}goto L8c5fff48; /* mov.w 0x8c5fff8e,r9 */
L8c5fff48: *stop=0x8c5fff48u;r[4]+=64u;goto L8c5fff4a; /* add #0x40,r4 */
L8c5fff4a: target=0x8c5ff9c0u;*pr=0x8c5fff4eu;*stop=0x8c5fff4cu;r[4]+=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fff4e; /* bsr 0x8c5ff9c0 */
L8c5fff4e: *stop=0x8c5fff4eu;r[9]=r[0];goto L8c5fff50; /* mov r0,r9 */
L8c5fff50: *stop=0x8c5fff50u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5fff52; /* mov.l @r14,r4 */
L8c5fff52: *stop=0x8c5fff52u;r[9]+=r[11];goto L8c5fff54; /* add r11,r9 */
L8c5fff54: *stop=0x8c5fff54u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fff90u,2);if(io_status)return io_status;r[11]=loaded;}goto L8c5fff56; /* mov.w 0x8c5fff90,r11 */
L8c5fff56: *stop=0x8c5fff56u;r[4]+=64u;goto L8c5fff58; /* add #0x40,r4 */
L8c5fff58: target=0x8c5ff9c0u;*pr=0x8c5fff5cu;*stop=0x8c5fff5au;r[4]+=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fff5c; /* bsr 0x8c5ff9c0 */
L8c5fff5c: *stop=0x8c5fff5cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fff5e; /* mov.l @r14,r2 */
L8c5fff5e: *stop=0x8c5fff5eu;r[7]=r[0];goto L8c5fff60; /* mov r0,r7 */
L8c5fff60: *stop=0x8c5fff60u;r[5]=r[9];goto L8c5fff62; /* mov r9,r5 */
L8c5fff62: *stop=0x8c5fff62u;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fff64; /* mov.l @(0x10,r2),r3 */
L8c5fff64: *stop=0x8c5fff64u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[13],4);if(io_status)return io_status;goto L8c5fff66; /* mov.l r13,@(0x8,r3) */
L8c5fff66: *stop=0x8c5fff66u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[6]=loaded;}goto L8c5fff68; /* mov.l @r14,r6 */
L8c5fff68: *stop=0x8c5fff68u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5fff6a; /* mov.l @r14,r4 */
L8c5fff6a: *stop=0x8c5fff6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fffa8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fff6c; /* mov.l 0x8c5fffa8,r3 */
L8c5fff6c: *stop=0x8c5fff6cu;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c5fff6e; /* mov.l @(0x10,r6),r6 */
L8c5fff6e: target=r[3];*pr=0x8c5fff72u;*stop=0x8c5fff70u;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fff72; /* jsr @r3 */
L8c5fff72: *stop=0x8c5fff72u;*sr=(*sr&~1u)|(r[0]==0xfffffff4u);goto L8c5fff74; /* cmp/eq #-0xc,r0 */
L8c5fff74: take=(*sr&1u);*stop=0x8c5fff76u;r[11]=r[0];if(take)goto L8c5fffde;goto L8c5fff78; /* bt/s 0x8c5fffde */
L8c5fff78: *stop=0x8c5fff78u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c5fff7a; /* tst r11,r11 */
L8c5fff7a: if(!(*sr&1u))goto L8c5fffe6;goto L8c5fff7c; /* bf 0x8c5fffe6 */
L8c5fff7c: *stop=0x8c5fff7cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fff7e; /* mov.l @r14,r2 */
L8c5fff7e: *stop=0x8c5fff7eu;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fff80; /* mov.l @(0x10,r2),r3 */
L8c5fff80: *stop=0x8c5fff80u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c5fff82; /* mov.l @r3,r1 */
L8c5fff82: *stop=0x8c5fff82u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c5fff84; /* tst r1,r1 */
L8c5fff84: if((*sr&1u))goto L8c5ffff4;goto L8c5fff86; /* bt 0x8c5ffff4 */
L8c5fff86: *stop=0x8c5fff88u;goto L8c5fffac; /* bra 0x8c5fffac */
L8c5fffac: *stop=0x8c5fffacu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[11]=loaded;}goto L8c5fffae; /* mov.l @r12,r11 */
L8c5fffae: *stop=0x8c5fffaeu;if(!valid_access(r[11],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[0]=loaded;}goto L8c5fffb0; /* mov.w @r11,r0 */
L8c5fffb0: *stop=0x8c5fffb0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c5fffb2; /* cmp/eq #0x1,r0 */
L8c5fffb2: if(!(*sr&1u))goto L8c600000;goto L8c5fffb4; /* bf 0x8c600000 */
L8c5fffb4: *stop=0x8c5fffb4u;r[5]=r[15];goto L8c5fffb6; /* mov r15,r5 */
L8c5fffb6: *stop=0x8c5fffb6u;r[4]=r[11];goto L8c5fffb8; /* mov r11,r4 */
L8c5fffb8: *stop=0x8c5fffb8u;r[5]+=20u;goto L8c5fffba; /* add #0x14,r5 */
L8c5fffba: target=0x8c60041au;*pr=0x8c5fffbeu;*stop=0x8c5fffbcu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fffbe; /* bsr 0x8c60041a */
L8c5fffbe: *stop=0x8c5fffbeu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c5fffc0; /* tst r0,r0 */
L8c5fffc0: if(!(*sr&1u))goto L8c5fffe6;goto L8c5fffc2; /* bf 0x8c5fffe6 */
L8c5fffc2: *stop=0x8c5fffc2u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fffc4; /* mov.l @(0x14,r15),r3 */
L8c5fffc4: *stop=0x8c5fffc4u;r[7]=1u;goto L8c5fffc6; /* mov #0x1,r7 */
L8c5fffc6: *stop=0x8c5fffc6u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[5]=loaded;}goto L8c5fffc8; /* mov.l @r12,r5 */
L8c5fffc8: *stop=0x8c5fffc8u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[3],4);if(io_status)return io_status;goto L8c5fffca; /* mov.l r3,@(0x4,r5) */
L8c5fffca: *stop=0x8c5fffcau;r[5]=r[3];goto L8c5fffcc; /* mov r3,r5 */
L8c5fffcc: *stop=0x8c5fffccu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[6]=loaded;}goto L8c5fffce; /* mov.l @r14,r6 */
L8c5fffce: *stop=0x8c5fffceu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5fffd0; /* mov.l @r14,r4 */
L8c5fffd0: *stop=0x8c5fffd0u;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c5fffd2; /* mov.l @(0x10,r6),r6 */
L8c5fffd2: *stop=0x8c5fffd2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6000dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fffd4; /* mov.l 0x8c6000dc,r3 */
L8c5fffd4: target=r[3];*pr=0x8c5fffd8u;*stop=0x8c5fffd6u;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fffd8; /* jsr @r3 */
L8c5fffd8: *stop=0x8c5fffd8u;*sr=(*sr&~1u)|(r[0]==0xfffffff4u);goto L8c5fffda; /* cmp/eq #-0xc,r0 */
L8c5fffda: take=!(*sr&1u);*stop=0x8c5fffdcu;r[11]=r[0];if(take)goto L8c5fffe2;goto L8c5fffde; /* bf/s 0x8c5fffe2 */
L8c5fffde: *stop=0x8c5fffe0u;r[4]=r[11];goto L8c60000a; /* bra 0x8c60000a */
L8c5fffe2: *stop=0x8c5fffe2u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c5fffe4; /* tst r11,r11 */
L8c5fffe4: if((*sr&1u))goto L8c5fffea;goto L8c5fffe6; /* bt 0x8c5fffea */
L8c5fffe6: *stop=0x8c5fffe8u;r[5]=4294967292u;goto L8c5ffff6; /* bra 0x8c5ffff6 */
L8c5fffea: *stop=0x8c5fffeau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fffec; /* mov.l @r14,r3 */
L8c5fffec: *stop=0x8c5fffecu;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fffee; /* mov.l @(0x10,r3),r2 */
L8c5fffee: *stop=0x8c5fffeeu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffff0; /* mov.l @r2,r1 */
L8c5ffff0: *stop=0x8c5ffff0u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c5ffff2; /* tst r1,r1 */
L8c5ffff2: if(!(*sr&1u))goto L8c5ffffa;goto L8c5ffff4; /* bf 0x8c5ffffa */
L8c5ffff4: *stop=0x8c5ffff4u;r[5]=4294967291u;goto L8c5ffff6; /* mov #-0x5,r5 */
L8c5ffff6: *stop=0x8c5ffff8u;r[4]=r[5];goto L8c60000a; /* bra 0x8c60000a */
L8c5ffffa: *stop=0x8c5ffffau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffffc; /* mov.l @r14,r4 */
L8c5ffffc: target=0x8c5ffcfeu;*pr=0x8c600000u;*stop=0x8c5ffffeu;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600000; /* bsr 0x8c5ffcfe */
L8c600000: *stop=0x8c600000u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c600002; /* mov.l @r14,r2 */
L8c600002: *stop=0x8c600002u;r[0]=56u;goto L8c600004; /* mov #0x38,r0 */
L8c600004: *stop=0x8c600004u;r[4]=1u;goto L8c600006; /* mov #0x1,r4 */
L8c600006: *stop=0x8c600006u;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[4],2);if(io_status)return io_status;goto L8c600008; /* mov.w r4,@(r0,r2) */
L8c600008: *stop=0x8c600008u;r[4]=r[13];goto L8c60000a; /* mov r13,r4 */
L8c60000a: *stop=0x8c60000au;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c60000c; /* mov.l @(0xc,r15),r0 */
L8c60000c: *stop=0x8c60000cu;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c60000e; /* stc SR,r3 */
L8c60000e: *stop=0x8c60000eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6000d6u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c600010; /* mov.w 0x8c6000d6,r2 */
L8c600010: *stop=0x8c600010u;r[0]&=0xfu;goto L8c600012; /* and #0xf,r0 */
L8c600012: *stop=0x8c600012u;r[0]<<=2;goto L8c600014; /* shll2 r0 */
L8c600014: *stop=0x8c600014u;r[3]&=r[2];goto L8c600016; /* and r2,r3 */
L8c600016: *stop=0x8c600016u;r[0]<<=2;goto L8c600018; /* shll2 r0 */
L8c600018: *stop=0x8c600018u;r[0]|=r[3];goto L8c60001a; /* or r3,r0 */
L8c60001a: *stop=0x8c60001au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c60001c; /* ldc r0,SR */
L8c60001c: *stop=0x8c60001cu;r[0]=r[4];goto L8c60001e; /* mov r4,r0 */
L8c60001e: *stop=0x8c60001eu;r[15]+=32u;goto L8c600020; /* add #0x20,r15 */
L8c600020: *stop=0x8c600020u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c600022; /* lds.l @r15+,PR */
L8c600022: *stop=0x8c600022u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c600024; /* mov.l @r15+,r8 */
L8c600024: *stop=0x8c600024u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c600026; /* mov.l @r15+,r9 */
L8c600026: *stop=0x8c600026u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c600028; /* mov.l @r15+,r10 */
L8c600028: *stop=0x8c600028u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c60002a; /* mov.l @r15+,r11 */
L8c60002a: *stop=0x8c60002au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c60002c; /* mov.l @r15+,r12 */
L8c60002c: *stop=0x8c60002cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c60002e; /* mov.l @r15+,r13 */
L8c60002e: *stop=0x8c60002eu;if(*pr!=entry_pr)return -3;*stop=0x8c600030u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c600038: *stop=0x8c600038u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c60003a; /* mov.l r14,@-r15 */
L8c60003a: *stop=0x8c60003au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c60003c; /* mov.l r13,@-r15 */
L8c60003c: *stop=0x8c60003cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c60003e; /* mov.l r12,@-r15 */
L8c60003e: *stop=0x8c60003eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c600040; /* mov.l r8,@-r15 */
L8c600040: *stop=0x8c600040u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c600042; /* sts.l PR,@-r15 */
L8c600042: *stop=0x8c600042u;r[15]+=4294967276u;goto L8c600044; /* add #-0x14,r15 */
L8c600044: *stop=0x8c600044u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6000e4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c600046; /* mov.l 0x8c6000e4,r14 */
L8c600046: *stop=0x8c600046u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c600048; /* mov.l r4,@(0x4,r15) */
L8c600048: *stop=0x8c600048u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c60004a; /* mov.l r5,@r15 */
L8c60004a: *stop=0x8c60004au;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c60004c; /* mov.l r6,@(0x10,r15) */
L8c60004c: *stop=0x8c60004cu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[7],4);if(io_status)return io_status;goto L8c60004e; /* mov.l r7,@(0xc,r15) */
L8c60004e: *stop=0x8c60004eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c600050; /* mov.l @r14,r4 */
L8c600050: *stop=0x8c600050u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c600052; /* tst r4,r4 */
L8c600052: if((*sr&1u))goto L8c600060;goto L8c600054; /* bt 0x8c600060 */
L8c600054: *stop=0x8c600054u;r[0]=56u;goto L8c600056; /* mov #0x38,r0 */
L8c600056: *stop=0x8c600056u;if(!valid_access(r[0]+r[4],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0]+r[4],2);if(io_status)return io_status;r[0]=loaded;}goto L8c600058; /* mov.w @(r0,r4),r0 */
L8c600058: *stop=0x8c600058u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c60005a; /* cmp/eq #0x1,r0 */
L8c60005a: if(!(*sr&1u))goto L8c600060;goto L8c60005c; /* bf 0x8c600060 */
L8c60005c: *stop=0x8c60005eu;r[0]=0u;goto L8c600148; /* bra 0x8c600148 */
L8c600060: *stop=0x8c600060u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c600062; /* mov.l @r15,r0 */
L8c600062: *stop=0x8c600062u;*sr=(*sr&~1u)|!(r[0]&0x1fu);goto L8c600064; /* tst #0x1f,r0 */
L8c600064: if((*sr&1u))goto L8c60006a;goto L8c600066; /* bt 0x8c60006a */
L8c600066: *stop=0x8c600068u;r[0]=4294967282u;goto L8c600148; /* bra 0x8c600148 */
L8c60006a: *stop=0x8c60006au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6000e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60006c; /* mov.l 0x8c6000e8,r2 */
L8c60006c: target=r[2];*pr=0x8c600070u;*stop=0x8c60006eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600070; /* jsr @r2 */
L8c600070: *stop=0x8c600070u;r[12]=r[0];goto L8c600072; /* mov r0,r12 */
L8c600072: *stop=0x8c600072u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c600074; /* mov.l @r12,r3 */
L8c600074: target=r[3];*pr=0x8c600078u;*stop=0x8c600076u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600078; /* jsr @r3 */
L8c600078: *stop=0x8c600078u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c60007a; /* stc SR,r0 */
L8c60007a: *stop=0x8c60007au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6000d6u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c60007c; /* mov.w 0x8c6000d6,r3 */
L8c60007c: *stop=0x8c60007cu;r[0]>>=2;goto L8c60007e; /* shlr2 r0 */
L8c60007e: *stop=0x8c60007eu;r[0]>>=2;goto L8c600080; /* shlr2 r0 */
L8c600080: *stop=0x8c600080u;r[0]&=0xfu;goto L8c600082; /* and #0xf,r0 */
L8c600082: *stop=0x8c600082u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c600084; /* mov.l r0,@(0x8,r15) */
L8c600084: *stop=0x8c600084u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c600086; /* stc SR,r0 */
L8c600086: *stop=0x8c600086u;r[0]&=r[3];goto L8c600088; /* and r3,r0 */
L8c600088: *stop=0x8c600088u;r[0]|=0xf0u;goto L8c60008a; /* or #0xf0,r0 */
L8c60008a: *stop=0x8c60008au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c60008c; /* ldc r0,SR */
L8c60008c: *stop=0x8c60008cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60008e; /* mov.l @r15,r2 */
L8c60008e: *stop=0x8c60008eu;r[0]=56u;goto L8c600090; /* mov #0x38,r0 */
L8c600090: *stop=0x8c600090u;r[13]=0u;goto L8c600092; /* mov #0x0,r13 */
L8c600092: *stop=0x8c600092u;r[3]=r[2];goto L8c600094; /* mov r2,r3 */
L8c600094: *stop=0x8c600094u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c600096; /* mov.l r2,@r14 */
L8c600096: *stop=0x8c600096u;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[13],2);if(io_status)return io_status;goto L8c600098; /* mov.w r13,@(r0,r3) */
L8c600098: *stop=0x8c600098u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60009a; /* mov.l @r14,r3 */
L8c60009a: *stop=0x8c60009au;if(!valid_access(r[3]+32u,4u))return -3;wr(m,bus,&io_status,r[3]+0x20u,r[13],4);if(io_status)return io_status;goto L8c60009c; /* mov.l r13,@(0x20,r3) */
L8c60009c: *stop=0x8c60009cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60009e; /* mov.l @(0x4,r15),r3 */
L8c60009e: *stop=0x8c60009eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6000a0; /* mov.l @r14,r2 */
L8c6000a0: *stop=0x8c6000a0u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6000a2; /* mov.l r3,@r2 */
L8c6000a2: *stop=0x8c6000a2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[8]=loaded;}goto L8c6000a4; /* mov.l @r14,r8 */
L8c6000a4: *stop=0x8c6000a4u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6000a6; /* mov.l @(0x10,r15),r5 */
L8c6000a6: *stop=0x8c6000a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6000ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6000a8; /* mov.l 0x8c6000ec,r3 */
L8c6000a8: *stop=0x8c6000a8u;r[8]+=16u;goto L8c6000aa; /* add #0x10,r8 */
L8c6000aa: target=r[3];*pr=0x8c6000aeu;*stop=0x8c6000acu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6000ae; /* jsr @r3 */
L8c6000ae: *stop=0x8c6000aeu;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c6000b0; /* mov.l r0,@r8 */
L8c6000b0: *stop=0x8c6000b0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6000b2; /* mov.l @r14,r3 */
L8c6000b2: *stop=0x8c6000b2u;if(!valid_access(r[3]+12u,4u))return -3;wr(m,bus,&io_status,r[3]+0xcu,r[12],4);if(io_status)return io_status;goto L8c6000b4; /* mov.l r12,@(0xc,r3) */
L8c6000b4: *stop=0x8c6000b4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6000b6; /* mov.l @r14,r2 */
L8c6000b6: *stop=0x8c6000b6u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[13],4);if(io_status)return io_status;goto L8c6000b8; /* mov.l r13,@(0x1c,r2) */
L8c6000b8: *stop=0x8c6000b8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6000ba; /* mov.l @r14,r3 */
L8c6000ba: *stop=0x8c6000bau;r[0]=58u;goto L8c6000bc; /* mov #0x3a,r0 */
L8c6000bc: *stop=0x8c6000bcu;r[5]=r[13];goto L8c6000be; /* mov r13,r5 */
L8c6000be: *stop=0x8c6000beu;if(!valid_access(r[0]+r[3],2u))return -3;wr(m,bus,&io_status,r[0]+r[3],r[13],2);if(io_status)return io_status;goto L8c6000c0; /* mov.w r13,@(r0,r3) */
L8c6000c0: *stop=0x8c6000c0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6000c2; /* mov.l @r14,r3 */
L8c6000c2: *stop=0x8c6000c2u;if(!valid_access(r[3]+36u,4u))return -3;wr(m,bus,&io_status,r[3]+0x24u,r[13],4);if(io_status)return io_status;goto L8c6000c4; /* mov.l r13,@(0x24,r3) */
L8c6000c4: *stop=0x8c6000c4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6000c6; /* mov.l @r14,r2 */
L8c6000c6: *stop=0x8c6000c6u;if(!valid_access(r[2]+52u,4u))return -3;wr(m,bus,&io_status,r[2]+0x34u,r[13],4);if(io_status)return io_status;goto L8c6000c8; /* mov.l r13,@(0x34,r2) */
L8c6000c8: *stop=0x8c6000c8u;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6000ca; /* mov.l @(0x28,r15),r2 */
L8c6000ca: *stop=0x8c6000cau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6000cc; /* mov.l @r14,r3 */
L8c6000cc: *stop=0x8c6000ccu;if(!valid_access(r[3]+60u,4u))return -3;wr(m,bus,&io_status,r[3]+0x3cu,r[2],4);if(io_status)return io_status;goto L8c6000ce; /* mov.l r2,@(0x3c,r3) */
L8c6000ce: *stop=0x8c6000ceu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6000d8u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6000d0; /* mov.w 0x8c6000d8,r3 */
L8c6000d0: *stop=0x8c6000d0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6000d2; /* mov.l @r14,r4 */
L8c6000d2: *stop=0x8c6000d4u;r[4]+=r[3];goto L8c6000fc; /* bra 0x8c6000fc */
L8c6000f0: *stop=0x8c6000f0u;r[0]=76u;goto L8c6000f2; /* mov #0x4c,r0 */
L8c6000f2: *stop=0x8c6000f2u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[13],2);if(io_status)return io_status;goto L8c6000f4; /* mov.w r13,@(r0,r4) */
L8c6000f4: *stop=0x8c6000f4u;r[5]+=1u;goto L8c6000f6; /* add #0x1,r5 */
L8c6000f6: *stop=0x8c6000f6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6000f8; /* mov.l @r14,r3 */
L8c6000f8: *stop=0x8c6000f8u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6000fa; /* mov.l r3,@r4 */
L8c6000fa: *stop=0x8c6000fau;r[4]+=84u;goto L8c6000fc; /* add #0x54,r4 */
L8c6000fc: *stop=0x8c6000fcu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6000fe; /* mov.l @r14,r2 */
L8c6000fe: *stop=0x8c6000feu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c600100; /* mov.l @r2,r3 */
L8c600100: *stop=0x8c600100u;r[3]+=1u;goto L8c600102; /* add #0x1,r3 */
L8c600102: *stop=0x8c600102u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]);goto L8c600104; /* cmp/ge r3,r5 */
L8c600104: if(!(*sr&1u))goto L8c6000f0;goto L8c600106; /* bf 0x8c6000f0 */
L8c600106: *stop=0x8c600106u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c600108; /* mov.l @r14,r3 */
L8c600108: *stop=0x8c600108u;r[7]=0u;goto L8c60010a; /* mov #0x0,r7 */
L8c60010a: *stop=0x8c60010au;{uint32_t loaded=sx(m,bus,&io_status,0x8c600234u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c60010c; /* mov.w 0x8c600234,r1 */
L8c60010c: *stop=0x8c60010cu;r[5]=r[7];goto L8c60010e; /* mov r7,r5 */
L8c60010e: *stop=0x8c60010eu;r[2]=r[3];goto L8c600110; /* mov r3,r2 */
L8c600110: *stop=0x8c600110u;r[6]=r[7];goto L8c600112; /* mov r7,r6 */
L8c600112: *stop=0x8c600112u;r[1]+=r[2];goto L8c600114; /* add r2,r1 */
L8c600114: *stop=0x8c600114u;if(!valid_access(r[3]+20u,4u))return -3;wr(m,bus,&io_status,r[3]+0x14u,r[1],4);if(io_status)return io_status;goto L8c600116; /* mov.l r1,@(0x14,r3) */
L8c600116: *stop=0x8c600116u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c600118; /* mov.l @r14,r3 */
L8c600118: *stop=0x8c600118u;{uint32_t loaded=sx(m,bus,&io_status,0x8c600234u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c60011a; /* mov.w 0x8c600234,r1 */
L8c60011a: *stop=0x8c60011au;r[2]=r[3];goto L8c60011c; /* mov r3,r2 */
L8c60011c: *stop=0x8c60011cu;r[1]+=r[2];goto L8c60011e; /* add r2,r1 */
L8c60011e: *stop=0x8c60011eu;r[1]+=84u;goto L8c600120; /* add #0x54,r1 */
L8c600120: *stop=0x8c600120u;if(!valid_access(r[3]+24u,4u))return -3;wr(m,bus,&io_status,r[3]+0x18u,r[1],4);if(io_status)return io_status;goto L8c600122; /* mov.l r1,@(0x18,r3) */
L8c600122: *stop=0x8c600122u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c600124; /* mov.l @r14,r4 */
L8c600124: target=0x8c600464u;*pr=0x8c600128u;*stop=0x8c600126u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600128; /* bsr 0x8c600464 */
L8c600128: *stop=0x8c600128u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c60012a; /* mov.l @(0x8,r15),r0 */
L8c60012a: *stop=0x8c60012au;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c60012c; /* stc SR,r2 */
L8c60012c: *stop=0x8c60012cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c600236u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c60012e; /* mov.w 0x8c600236,r3 */
L8c60012e: *stop=0x8c60012eu;r[0]&=0xfu;goto L8c600130; /* and #0xf,r0 */
L8c600130: *stop=0x8c600130u;r[0]<<=2;goto L8c600132; /* shll2 r0 */
L8c600132: *stop=0x8c600132u;r[2]&=r[3];goto L8c600134; /* and r3,r2 */
L8c600134: *stop=0x8c600134u;r[0]<<=2;goto L8c600136; /* shll2 r0 */
L8c600136: *stop=0x8c600136u;r[0]|=r[2];goto L8c600138; /* or r2,r0 */
L8c600138: *stop=0x8c600138u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c60013a; /* ldc r0,SR */
L8c60013a: *stop=0x8c60013au;r[15]+=20u;goto L8c60013c; /* add #0x14,r15 */
L8c60013c: *stop=0x8c60013cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c60013e; /* lds.l @r15+,PR */
L8c60013e: *stop=0x8c60013eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c600140; /* mov.l @r15+,r8 */
L8c600140: *stop=0x8c600140u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c600142; /* mov.l @r15+,r12 */
L8c600142: *stop=0x8c600142u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c600144; /* mov.l @r15+,r13 */
L8c600144: *stop=0x8c600146u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;goto L8c5ffd28; /* bra 0x8c5ffd28 */
L8c600148: *stop=0x8c600148u;r[15]+=20u;goto L8c60014a; /* add #0x14,r15 */
L8c60014a: *stop=0x8c60014au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c60014c; /* lds.l @r15+,PR */
L8c60014c: *stop=0x8c60014cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c60014e; /* mov.l @r15+,r8 */
L8c60014e: *stop=0x8c60014eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c600150; /* mov.l @r15+,r12 */
L8c600150: *stop=0x8c600150u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c600152; /* mov.l @r15+,r13 */
L8c600152: *stop=0x8c600152u;if(*pr!=entry_pr)return -3;*stop=0x8c600154u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c601618(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c601618;
L8c601618: *stop=0x8c601618u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c60161a; /* mov.l r14,@-r15 */
L8c60161a: *stop=0x8c60161au;{uint32_t loaded=rd(m,bus,&io_status,0x8c60181cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60161c; /* mov.l 0x8c60181c,r3 */
L8c60161c: *stop=0x8c60161cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601818u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c60161e; /* mov.l 0x8c601818,r14 */
L8c60161e: *stop=0x8c60161eu;r[2]=r[3];goto L8c601620; /* mov r3,r2 */
L8c601620: *stop=0x8c601620u;r[1]=r[2];goto L8c601622; /* mov r2,r1 */
L8c601622: *stop=0x8c601622u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c601624; /* sts.l PR,@-r15 */
L8c601624: *stop=0x8c601624u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c601626; /* mov.l r3,@r14 */
L8c601626: *stop=0x8c601626u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601628; /* mov.l @(0xc,r2),r3 */
L8c601628: target=r[3];*pr=0x8c60162cu;*stop=0x8c60162au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60162c; /* jsr @r3 */
L8c60162c: *stop=0x8c60162cu;r[15]+=4u;goto L8c60162e; /* add #0x4,r15 */
L8c60162e: *stop=0x8c60162eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c601630; /* lds.l @r15+,PR */
L8c601630: *stop=0x8c601630u;if(*pr!=entry_pr)return -3;*stop=0x8c601632u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c600dd0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c600dd0;
L8c600dd0: *stop=0x8c600dd0u;r[6]=0u;goto L8c600dd2; /* mov #0x0,r6 */
L8c600dd2: *stop=0x8c600dd2u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[6],4);if(io_status)return io_status;goto L8c600dd4; /* mov.l r6,@r4 */
L8c600dd4: *stop=0x8c600dd4u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c600dd6; /* mov.l r5,@(0x4,r4) */
L8c600dd6: *stop=0x8c600dd6u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[6],4);if(io_status)return io_status;goto L8c600dd8; /* mov.l r6,@(0x8,r4) */
L8c600dd8: *stop=0x8c600dd8u;if(*pr!=entry_pr)return -3;*stop=0x8c600ddau;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c6454fe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6454fe;
L8c6454fe: *stop=0x8c6454feu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645500; /* mov.l r14,@-r15 */
L8c645500: *stop=0x8c645500u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c645502; /* mov.l r13,@-r15 */
L8c645502: *stop=0x8c645502u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c645504; /* mov.l r12,@-r15 */
L8c645504: *stop=0x8c645504u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c645506; /* mov.l r11,@-r15 */
L8c645506: *stop=0x8c645506u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c645508; /* mov.l r10,@-r15 */
L8c645508: *stop=0x8c645508u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64550a; /* mov.l r9,@-r15 */
L8c64550a: *stop=0x8c64550au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64550c; /* mov.l r8,@-r15 */
L8c64550c: *stop=0x8c64550cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64550e; /* sts.l PR,@-r15 */
L8c64550e: *stop=0x8c64550eu;r[15]+=4294967284u;goto L8c645510; /* add #-0xc,r15 */
L8c645510: *stop=0x8c645510u;r[9]=r[6];goto L8c645512; /* mov r6,r9 */
L8c645512: *stop=0x8c645512u;r[3]=r[5];goto L8c645514; /* mov r5,r3 */
L8c645514: *stop=0x8c645514u;r[12]=r[6];goto L8c645516; /* mov r6,r12 */
L8c645516: *stop=0x8c645516u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c645518; /* mov.l r4,@(0x4,r15) */
L8c645518: *stop=0x8c645518u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64551a; /* mov.l r5,@r15 */
L8c64551a: *stop=0x8c64551au;r[11]=r[7];goto L8c64551c; /* mov r7,r11 */
L8c64551c: *stop=0x8c64551cu;if(!valid_access(r[9]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64551e; /* mov.l @(0x8,r9),r2 */
L8c64551e: *stop=0x8c64551eu;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c645520; /* cmp/eq r3,r2 */
L8c645520: take=(*sr&1u);*stop=0x8c645522u;r[12]+=16u;if(take)goto L8c6455b8;goto L8c645524; /* bt/s 0x8c6455b8 */
L8c645524: *stop=0x8c645524u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c645526; /* mov.l @r15,r3 */
L8c645526: *stop=0x8c645526u;r[14]=0u;goto L8c645528; /* mov #0x0,r14 */
L8c645528: *stop=0x8c645528u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645550u,2);if(io_status)return io_status;r[8]=loaded;}goto L8c64552a; /* mov.w 0x8c645550,r8 */
L8c64552a: *stop=0x8c64552au;if(!valid_access(r[9]+8u,4u))return -3;wr(m,bus,&io_status,r[9]+0x8u,r[3],4);if(io_status)return io_status;goto L8c64552c; /* mov.l r3,@(0x8,r9) */
L8c64552c: *stop=0x8c64552cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64552e; /* mov.l @(0x4,r15),r3 */
L8c64552e: *stop=0x8c64552eu;if(!valid_access(r[9]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x4u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c645530; /* mov.l @(0x4,r9),r10 */
L8c645530: *stop=0x8c645530u;r[3]+=r[8];goto L8c645532; /* add r8,r3 */
L8c645532: *stop=0x8c645534u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c645592; /* bra 0x8c645592 */
L8c645536: *stop=0x8c645536u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645554u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645538; /* mov.l 0x8c645554,r3 */
L8c645538: *stop=0x8c645538u;r[5]=1u;goto L8c64553a; /* mov #0x1,r5 */
L8c64553a: *stop=0x8c64553au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64553c; /* mov.l @(0x4,r15),r6 */
L8c64553c: target=r[3];*pr=0x8c645540u;*stop=0x8c64553eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645540; /* jsr @r3 */
L8c645540: *stop=0x8c645540u;r[4]=r[0];goto L8c645542; /* mov r0,r4 */
L8c645542: *stop=0x8c645542u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c645544; /* tst r4,r4 */
L8c645544: if((*sr&1u))goto L8c645558;goto L8c645546; /* bt 0x8c645558 */
L8c645546: *stop=0x8c645546u;r[3]=0u;goto L8c645548; /* mov #0x0,r3 */
L8c645548: *stop=0x8c645548u;if(!valid_access(r[9]+8u,4u))return -3;wr(m,bus,&io_status,r[9]+0x8u,r[3],4);if(io_status)return io_status;goto L8c64554a; /* mov.l r3,@(0x8,r9) */
L8c64554a: *stop=0x8c64554cu;r[0]=r[4];goto L8c6455ba; /* bra 0x8c6455ba */
L8c645558: *stop=0x8c645558u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]);goto L8c64555a; /* cmp/ge r10,r14 */
L8c64555a: take=(*sr&1u);*stop=0x8c64555cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[13]=loaded;}if(take)goto L8c645596;goto L8c64555e; /* bt/s 0x8c645596 */
L8c64555e: *stop=0x8c64555eu;r[5]=r[12];goto L8c645560; /* mov r12,r5 */
L8c645560: target=0x8c645468u;*pr=0x8c645564u;*stop=0x8c645562u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645564; /* bsr 0x8c645468 */
L8c645564: *stop=0x8c645564u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c645566; /* tst r0,r0 */
L8c645566: take=(*sr&1u);*stop=0x8c645568u;r[4]=r[0];if(take)goto L8c645586;goto L8c64556a; /* bt/s 0x8c645586 */
L8c64556a: *stop=0x8c64556au;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c64556c; /* tst r14,r14 */
L8c64556c: if(!(*sr&1u))goto L8c645576;goto L8c64556e; /* bf 0x8c645576 */
L8c64556e: *stop=0x8c64556eu;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645570; /* mov.l @(0x4,r12),r2 */
L8c645570: *stop=0x8c645570u;*sr=(*sr&~1u)|(r[11]==r[2]);goto L8c645572; /* cmp/eq r11,r2 */
L8c645572: if((*sr&1u))goto L8c645576;goto L8c645574; /* bt 0x8c645576 */
L8c645574: *stop=0x8c645574u;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c645576; /* mov.l @(0x4,r12),r11 */
L8c645576: *stop=0x8c645576u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645578; /* mov.l @(0x8,r15),r2 */
L8c645578: *stop=0x8c645578u;r[13]+=r[4];goto L8c64557a; /* add r4,r13 */
L8c64557a: *stop=0x8c64557au;r[12]+=44u;goto L8c64557c; /* add #0x2c,r12 */
L8c64557c: *stop=0x8c64557cu;*sr=(*sr&~1u)|(r[13]>=r[2]);goto L8c64557e; /* cmp/hs r2,r13 */
L8c64557e: take=(*sr&1u);*stop=0x8c645580u;r[14]+=1u;if(take)goto L8c645586;goto L8c645582; /* bt/s 0x8c645586 */
L8c645582: *stop=0x8c645582u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]);goto L8c645584; /* cmp/ge r10,r14 */
L8c645584: if(!(*sr&1u))goto L8c64555e;goto L8c645586; /* bf 0x8c64555e */
L8c645586: *stop=0x8c645586u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]);goto L8c645588; /* cmp/ge r10,r14 */
L8c645588: if((*sr&1u))goto L8c645596;goto L8c64558a; /* bt 0x8c645596 */
L8c64558a: *stop=0x8c64558au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64558c; /* mov.l @r15,r2 */
L8c64558c: *stop=0x8c64558cu;r[11]-=r[8];goto L8c64558e; /* sub r8,r11 */
L8c64558e: *stop=0x8c64558eu;r[2]+=1u;goto L8c645590; /* add #0x1,r2 */
L8c645590: *stop=0x8c645590u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c645592; /* mov.l r2,@r15 */
L8c645592: *stop=0x8c645592u;*sr=(*sr&~1u)|((int32_t)r[11]>0);goto L8c645594; /* cmp/pl r11 */
L8c645594: if((*sr&1u))goto L8c645536;goto L8c645596; /* bt 0x8c645536 */
L8c645596: *stop=0x8c645596u;*sr=(*sr&~1u)|(r[10]==r[14]);goto L8c645598; /* cmp/eq r10,r14 */
L8c645598: take=!(*sr&1u);*stop=0x8c64559au;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[14],4);if(io_status)return io_status;if(take)goto L8c6455b8;goto L8c64559c; /* bf/s 0x8c6455b8 */
L8c64559c: *stop=0x8c64559cu;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c64559e; /* tst r11,r11 */
L8c64559e: if((*sr&1u))goto L8c6455b8;goto L8c6455a0; /* bt 0x8c6455b8 */
L8c6455a0: *stop=0x8c6455a0u;*sr=(*sr&~1u)|(r[8]==r[11]);goto L8c6455a2; /* cmp/eq r8,r11 */
L8c6455a2: if(!(*sr&1u))goto L8c6455b0;goto L8c6455a4; /* bf 0x8c6455b0 */
L8c6455a4: *stop=0x8c6455a4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6455a6; /* mov.l @(0x8,r15),r2 */
L8c6455a6: *stop=0x8c6455a6u;*sr=(*sr&~1u)|(r[13]>=r[2]);goto L8c6455a8; /* cmp/hs r2,r13 */
L8c6455a8: if((*sr&1u))goto L8c6455b8;goto L8c6455aa; /* bt 0x8c6455b8 */
L8c6455aa: *stop=0x8c6455aau;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[1]=loaded;}goto L8c6455ac; /* mov.b @r13,r1 */
L8c6455ac: *stop=0x8c6455acu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c6455ae; /* tst r1,r1 */
L8c6455ae: if((*sr&1u))goto L8c6455b8;goto L8c6455b0; /* bt 0x8c6455b8 */
L8c6455b0: *stop=0x8c6455b0u;r[2]=0u;goto L8c6455b2; /* mov #0x0,r2 */
L8c6455b2: *stop=0x8c6455b2u;if(!valid_access(r[9]+8u,4u))return -3;wr(m,bus,&io_status,r[9]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6455b4; /* mov.l r2,@(0x8,r9) */
L8c6455b4: *stop=0x8c6455b6u;r[0]=4294967284u;goto L8c6455ba; /* bra 0x8c6455ba */
L8c6455b8: *stop=0x8c6455b8u;r[0]=0u;goto L8c6455ba; /* mov #0x0,r0 */
L8c6455ba: *stop=0x8c6455bau;r[15]+=12u;goto L8c6455bc; /* add #0xc,r15 */
L8c6455bc: *stop=0x8c6455bcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6455be; /* lds.l @r15+,PR */
L8c6455be: *stop=0x8c6455beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6455c0; /* mov.l @r15+,r8 */
L8c6455c0: *stop=0x8c6455c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6455c2; /* mov.l @r15+,r9 */
L8c6455c2: *stop=0x8c6455c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6455c4; /* mov.l @r15+,r10 */
L8c6455c4: *stop=0x8c6455c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6455c6; /* mov.l @r15+,r11 */
L8c6455c6: *stop=0x8c6455c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6455c8; /* mov.l @r15+,r12 */
L8c6455c8: *stop=0x8c6455c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6455ca; /* mov.l @r15+,r13 */
L8c6455ca: *stop=0x8c6455cau;if(*pr!=entry_pr)return -3;*stop=0x8c6455ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c5ffc98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ffc98;
L8c5ffc98: *stop=0x8c5ffc98u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c5ffc9a; /* sts.l PR,@-r15 */
L8c5ffc9a: *stop=0x8c5ffc9au;r[15]+=4294967284u;goto L8c5ffc9c; /* add #-0xc,r15 */
L8c5ffc9c: *stop=0x8c5ffc9cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffd18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffc9e; /* mov.l 0x8c5ffd18,r3 */
L8c5ffc9e: *stop=0x8c5ffc9eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c5ffca0; /* mov.l r4,@(0x8,r15) */
L8c5ffca0: *stop=0x8c5ffca0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffd1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffca2; /* mov.l 0x8c5ffd1c,r2 */
L8c5ffca2: *stop=0x8c5ffca2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c5ffca4; /* mov.l r5,@r15 */
L8c5ffca4: *stop=0x8c5ffca4u;r[5]=0u;goto L8c5ffca6; /* mov #0x0,r5 */
L8c5ffca6: *stop=0x8c5ffca6u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c5ffca8; /* mov.l r6,@(0x4,r15) */
L8c5ffca8: *stop=0x8c5ffca8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[7]=loaded;}goto L8c5ffcaa; /* mov.l @r15,r7 */
L8c5ffcaa: *stop=0x8c5ffcaau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffcac; /* mov.l @r3,r4 */
L8c5ffcac: *stop=0x8c5ffcacu;r[7]<<=8;goto L8c5ffcae; /* shll8 r7 */
L8c5ffcae: *stop=0x8c5ffcaeu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c5ffcb0; /* mov.l @(0x8,r15),r6 */
L8c5ffcb0: *stop=0x8c5ffcb0u;r[7]<<=2;goto L8c5ffcb2; /* shll2 r7 */
L8c5ffcb2: *stop=0x8c5ffcb2u;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c5ffcb4; /* shll r7 */
L8c5ffcb4: target=r[2];*pr=0x8c5ffcb8u;*stop=0x8c5ffcb6u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffcb8; /* jsr @r2 */
L8c5ffcb8: *stop=0x8c5ffcb8u;r[4]=r[0];goto L8c5ffcba; /* mov r0,r4 */
L8c5ffcba: *stop=0x8c5ffcbau;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ffcbc; /* tst r4,r4 */
L8c5ffcbc: if(!(*sr&1u))goto L8c5ffcc6;goto L8c5ffcbe; /* bf 0x8c5ffcc6 */
L8c5ffcbe: *stop=0x8c5ffcbeu;r[15]+=12u;goto L8c5ffcc0; /* add #0xc,r15 */
L8c5ffcc0: *stop=0x8c5ffcc0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ffcc2; /* lds.l @r15+,PR */
L8c5ffcc2: *stop=0x8c5ffcc2u;if(*pr!=entry_pr)return -3;*stop=0x8c5ffcc4u;r[0]=4294967283u;return 0; /* rts */
L8c5ffcc6: *stop=0x8c5ffcc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffd20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffcc8; /* mov.l 0x8c5ffd20,r3 */
L8c5ffcc8: *stop=0x8c5ffcc8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c5ffcca; /* mov.l @(0x4,r15),r6 */
L8c5ffcca: target=r[3];*pr=0x8c5ffcceu;*stop=0x8c5ffcccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffcce; /* jsr @r3 */
L8c5ffcce: *stop=0x8c5ffcceu;r[15]+=12u;goto L8c5ffcd0; /* add #0xc,r15 */
L8c5ffcd0: *stop=0x8c5ffcd0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ffcd2; /* lds.l @r15+,PR */
L8c5ffcd2: *stop=0x8c5ffcd2u;if(*pr!=entry_pr)return -3;*stop=0x8c5ffcd4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c5ff9e6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ff9e6;
L8c5ff9e6: *stop=0x8c5ff9e6u;r[6]=r[4];goto L8c5ff9e8; /* mov r4,r6 */
L8c5ff9e8: *stop=0x8c5ff9e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffc08u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c5ff9ea; /* mov.l 0x8c5ffc08,r7 */
L8c5ff9ea: *stop=0x8c5ff9eau;r[5]=5u;goto L8c5ff9ec; /* mov #0x5,r5 */
L8c5ff9ec: *stop=0x8c5ff9ecu;r[4]=0u;goto L8c5ff9ee; /* mov #0x0,r4 */
L8c5ff9ee: *stop=0x8c5ff9eeu;r[6]+=1u;goto L8c5ff9f0; /* add #0x1,r6 */
L8c5ff9f0: *stop=0x8c5ff9f0u;if(!valid_access(r[6],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[2]=loaded;}r[6]+=1;goto L8c5ff9f2; /* mov.b @r6+,r2 */
L8c5ff9f2: *stop=0x8c5ff9f2u;if(!valid_access(r[7],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7],1);if(io_status)return io_status;r[3]=loaded;}r[7]+=1;goto L8c5ff9f4; /* mov.b @r7+,r3 */
L8c5ff9f4: *stop=0x8c5ff9f4u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c5ff9f6; /* cmp/eq r2,r3 */
L8c5ff9f6: if((*sr&1u))goto L8c5ff9fc;goto L8c5ff9f8; /* bt 0x8c5ff9fc */
L8c5ff9f8: *stop=0x8c5ff9f8u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff9fau;r[0]=0u;return 0; /* rts */
L8c5ff9fc: *stop=0x8c5ff9fcu;r[4]+=1u;goto L8c5ff9fe; /* add #0x1,r4 */
L8c5ff9fe: *stop=0x8c5ff9feu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]);goto L8c5ffa00; /* cmp/ge r5,r4 */
L8c5ffa00: if(!(*sr&1u))goto L8c5ff9f0;goto L8c5ffa02; /* bf 0x8c5ff9f0 */
L8c5ffa02: *stop=0x8c5ffa02u;r[0]=1u;goto L8c5ffa04; /* mov #0x1,r0 */
L8c5ffa04: *stop=0x8c5ffa04u;if(*pr!=entry_pr)return -3;*stop=0x8c5ffa06u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c5ff9c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ff9c0;
L8c5ff9c0: *stop=0x8c5ff9c0u;if(!valid_access(r[4]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c5ff9c2; /* mov.b @(0x1,r4),r0 */
L8c5ff9c2: *stop=0x8c5ff9c2u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[2]=loaded;}goto L8c5ff9c4; /* mov.b @r4,r2 */
L8c5ff9c4: *stop=0x8c5ff9c4u;r[0]=r[0]&255u;goto L8c5ff9c6; /* extu.b r0,r0 */
L8c5ff9c6: *stop=0x8c5ff9c6u;r[0]<<=8;goto L8c5ff9c8; /* shll8 r0 */
L8c5ff9c8: *stop=0x8c5ff9c8u;r[3]=r[0];goto L8c5ff9ca; /* mov r0,r3 */
L8c5ff9ca: *stop=0x8c5ff9cau;if(!valid_access(r[4]+2u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x2u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c5ff9cc; /* mov.b @(0x2,r4),r0 */
L8c5ff9cc: *stop=0x8c5ff9ccu;r[2]=r[2]&255u;goto L8c5ff9ce; /* extu.b r2,r2 */
L8c5ff9ce: *stop=0x8c5ff9ceu;r[0]=r[0]&255u;goto L8c5ff9d0; /* extu.b r0,r0 */
L8c5ff9d0: *stop=0x8c5ff9d0u;r[0]<<=16;goto L8c5ff9d2; /* shll16 r0 */
L8c5ff9d2: *stop=0x8c5ff9d2u;r[3]|=r[0];goto L8c5ff9d4; /* or r0,r3 */
L8c5ff9d4: *stop=0x8c5ff9d4u;if(!valid_access(r[4]+3u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x3u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c5ff9d6; /* mov.b @(0x3,r4),r0 */
L8c5ff9d6: *stop=0x8c5ff9d6u;r[0]=r[0]&255u;goto L8c5ff9d8; /* extu.b r0,r0 */
L8c5ff9d8: *stop=0x8c5ff9d8u;r[0]<<=16;goto L8c5ff9da; /* shll16 r0 */
L8c5ff9da: *stop=0x8c5ff9dau;r[0]<<=8;goto L8c5ff9dc; /* shll8 r0 */
L8c5ff9dc: *stop=0x8c5ff9dcu;r[3]|=r[0];goto L8c5ff9de; /* or r0,r3 */
L8c5ff9de: *stop=0x8c5ff9deu;r[4]=r[3];goto L8c5ff9e0; /* mov r3,r4 */
L8c5ff9e0: *stop=0x8c5ff9e0u;r[4]|=r[2];goto L8c5ff9e2; /* or r2,r4 */
L8c5ff9e2: *stop=0x8c5ff9e2u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff9e4u;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c60041a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60041a;
L8c60041a: *stop=0x8c60041au;{uint32_t loaded=rd(m,bus,&io_status,0x8c600478u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60041c; /* mov.l 0x8c600478,r3 */
L8c60041c: *stop=0x8c60041cu;r[6]=0u;goto L8c60041e; /* mov #0x0,r6 */
L8c60041e: *stop=0x8c60041eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c600420; /* mov.l r14,@-r15 */
L8c600420: *stop=0x8c600420u;r[14]=r[5];goto L8c600422; /* mov r5,r14 */
L8c600422: *stop=0x8c600422u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c600424; /* sts.l PR,@-r15 */
L8c600424: *stop=0x8c600424u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c600426; /* mov.l @r3,r5 */
L8c600426: *stop=0x8c600426u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600480u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c600428; /* mov.l 0x8c600480,r2 */
L8c600428: target=r[2];*pr=0x8c60042cu;*stop=0x8c60042au;if(!valid_access(r[5]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60042c; /* jsr @r2 */
L8c60042c: *stop=0x8c60042cu;r[5]=r[0];goto L8c60042e; /* mov r0,r5 */
L8c60042e: *stop=0x8c60042eu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c600430; /* tst r5,r5 */
L8c600430: if(!(*sr&1u))goto L8c60043a;goto L8c600432; /* bf 0x8c60043a */
L8c600432: *stop=0x8c600432u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c600434; /* lds.l @r15+,PR */
L8c600434: *stop=0x8c600434u;r[0]=4294967288u;goto L8c600436; /* mov #-0x8,r0 */
L8c600436: *stop=0x8c600436u;if(*pr!=entry_pr)return -3;*stop=0x8c600438u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c60043a: *stop=0x8c60043au;r[2]=44u;goto L8c60043c; /* mov #0x2c,r2 */
L8c60043c: *stop=0x8c60043cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c600478u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60043e; /* mov.l 0x8c600478,r3 */
L8c60043e: *stop=0x8c60043eu;r[5]+=4294967295u;goto L8c600440; /* add #-0x1,r5 */
L8c600440: *stop=0x8c600440u;{uint32_t loaded=sx(m,bus,&io_status,0x8c600470u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c600442; /* mov.w 0x8c600470,r1 */
L8c600442: *stop=0x8c600442u;*macl=r[2]*r[5];goto L8c600444; /* mul.l r2,r5 */
L8c600444: *stop=0x8c600444u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c600446; /* mov.l @r3,r4 */
L8c600446: *stop=0x8c600446u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c600448; /* mov.l @(0x10,r4),r4 */
L8c600448: *stop=0x8c600448u;r[5]=*macl;goto L8c60044a; /* sts MACL,r5 */
L8c60044a: *stop=0x8c60044au;r[4]+=16u;goto L8c60044c; /* add #0x10,r4 */
L8c60044c: *stop=0x8c60044cu;r[4]+=r[5];goto L8c60044e; /* add r5,r4 */
L8c60044e: *stop=0x8c60044eu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c600450; /* mov.l @r4,r0 */
L8c600450: *stop=0x8c600450u;r[0]+=r[1];goto L8c600452; /* add r1,r0 */
L8c600452: *stop=0x8c600452u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c600454; /* mov.l r0,@r14 */
L8c600454: *stop=0x8c600454u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c600456; /* mov.l @(0x4,r4),r3 */
L8c600456: *stop=0x8c600456u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c600458; /* mov.l r3,@(0x4,r14) */
L8c600458: *stop=0x8c600458u;if(!valid_access(r[4]+8u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x8u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c60045a; /* mov.b @(0x8,r4),r0 */
L8c60045a: *stop=0x8c60045au;if(!valid_access(r[14]+8u,1u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],1);if(io_status)return io_status;goto L8c60045c; /* mov.b r0,@(0x8,r14) */
L8c60045c: *stop=0x8c60045cu;r[0]=0u;goto L8c60045e; /* mov #0x0,r0 */
L8c60045e: *stop=0x8c60045eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c600460; /* lds.l @r15+,PR */
L8c600460: *stop=0x8c600460u;if(*pr!=entry_pr)return -3;*stop=0x8c600462u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c5ffcfe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ffcfe;
L8c5ffcfe: *stop=0x8c5ffcfeu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ffd00; /* tst r4,r4 */
L8c5ffd00: if((*sr&1u))goto L8c5ffd10;goto L8c5ffd02; /* bt 0x8c5ffd10 */
L8c5ffd02: *stop=0x8c5ffd02u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffd04; /* mov.l @r4,r3 */
L8c5ffd04: *stop=0x8c5ffd04u;r[2]=2u;goto L8c5ffd06; /* mov #0x2,r2 */
L8c5ffd06: *stop=0x8c5ffd06u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c5ffd08; /* cmp/ge r2,r3 */
L8c5ffd08: if(!(*sr&1u))goto L8c5ffd10;goto L8c5ffd0a; /* bf 0x8c5ffd10 */
L8c5ffd0a: *stop=0x8c5ffd0au;r[4]+=16u;goto L8c5ffd0c; /* add #0x10,r4 */
L8c5ffd0c: *stop=0x8c5ffd0cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffd0e; /* mov.l @r4,r3 */
L8c5ffd0e: *stop=0x8c5ffd0eu;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c5ffd10; /* mov.l r3,@(0x2c,r4) */
L8c5ffd10: *stop=0x8c5ffd10u;if(*pr!=entry_pr)return -3;*stop=0x8c5ffd12u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c64530a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64530a;
L8c64530a: *stop=0x8c64530au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64530c; /* mov.l r14,@-r15 */
L8c64530c: *stop=0x8c64530cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64530e; /* sts.l PR,@-r15 */
L8c64530e: *stop=0x8c64530eu;r[15]+=4294967288u;goto L8c645310; /* add #-0x8,r15 */
L8c645310: *stop=0x8c645310u;r[3]=0u;goto L8c645312; /* mov #0x0,r3 */
L8c645312: *stop=0x8c645312u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645338u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645314; /* mov.l 0x8c645338,r2 */
L8c645314: *stop=0x8c645314u;r[14]=r[15];goto L8c645316; /* mov r15,r14 */
L8c645316: *stop=0x8c645316u;r[5]=r[14];goto L8c645318; /* mov r14,r5 */
L8c645318: *stop=0x8c645318u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c64531a; /* mov.l r4,@r14 */
L8c64531a: *stop=0x8c64531au;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c64531c; /* mov.l r3,@(0x4,r14) */
L8c64531c: target=r[2];*pr=0x8c645320u;*stop=0x8c64531eu;r[4]=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645320; /* jsr @r2 */
L8c645320: *stop=0x8c645320u;r[15]+=8u;goto L8c645322; /* add #0x8,r15 */
L8c645322: *stop=0x8c645322u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645324; /* lds.l @r15+,PR */
L8c645324: *stop=0x8c645324u;if(*pr!=entry_pr)return -3;*stop=0x8c645326u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c645468(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c645468;
L8c645468: *stop=0x8c645468u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64546a; /* mov.l r14,@-r15 */
L8c64546a: *stop=0x8c64546au;r[2]=34u;goto L8c64546c; /* mov #0x22,r2 */
L8c64546c: *stop=0x8c64546cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64546e; /* mov.l r13,@-r15 */
L8c64546e: *stop=0x8c64546eu;r[13]=r[4];goto L8c645470; /* mov r4,r13 */
L8c645470: *stop=0x8c645470u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c645472; /* mov.l r12,@-r15 */
L8c645472: *stop=0x8c645472u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c645474; /* sts.l PR,@-r15 */
L8c645474: *stop=0x8c645474u;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[3]=loaded;}goto L8c645476; /* mov.b @r13,r3 */
L8c645476: *stop=0x8c645476u;r[3]=r[3]&255u;goto L8c645478; /* extu.b r3,r3 */
L8c645478: *stop=0x8c645478u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c64547a; /* cmp/ge r2,r3 */
L8c64547a: take=(*sr&1u);*stop=0x8c64547cu;r[14]=r[5];if(take)goto L8c645482;goto L8c64547e; /* bt/s 0x8c645482 */
L8c64547e: *stop=0x8c645480u;r[0]=0u;goto L8c6454f4; /* bra 0x8c6454f4 */
L8c645482: *stop=0x8c645482u;r[4]=r[13];goto L8c645484; /* mov r13,r4 */
L8c645484: *stop=0x8c645484u;r[5]=r[14];goto L8c645486; /* mov r14,r5 */
L8c645486: *stop=0x8c645486u;r[6]=4u;goto L8c645488; /* mov #0x4,r6 */
L8c645488: target=0x8c64535cu;*pr=0x8c64548cu;*stop=0x8c64548au;r[4]+=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64548c; /* bsr 0x8c64535c */
L8c64548c: *stop=0x8c64548cu;r[5]=r[14];goto L8c64548e; /* mov r14,r5 */
L8c64548e: *stop=0x8c64548eu;r[4]=r[13];goto L8c645490; /* mov r13,r4 */
L8c645490: *stop=0x8c645490u;r[5]+=4u;goto L8c645492; /* add #0x4,r5 */
L8c645492: *stop=0x8c645492u;r[6]=4u;goto L8c645494; /* mov #0x4,r6 */
L8c645494: target=0x8c64535cu;*pr=0x8c645498u;*stop=0x8c645496u;r[4]+=10u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645498; /* bsr 0x8c64535c */
L8c645498: *stop=0x8c645498u;r[0]=25u;goto L8c64549a; /* mov #0x19,r0 */
L8c64549a: *stop=0x8c64549au;if(!valid_access(r[0]+r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c64549c; /* mov.b @(r0,r13),r0 */
L8c64549c: *stop=0x8c64549cu;if(!valid_access(r[14]+8u,1u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],1);if(io_status)return io_status;goto L8c64549e; /* mov.b r0,@(0x8,r14) */
L8c64549e: *stop=0x8c64549eu;r[0]=32u;goto L8c6454a0; /* mov #0x20,r0 */
L8c6454a0: *stop=0x8c6454a0u;if(!valid_access(r[0]+r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13]+r[0],1);if(io_status)return io_status;r[12]=loaded;}goto L8c6454a2; /* mov.b @(r0,r13),r12 */
L8c6454a2: *stop=0x8c6454a2u;if(!valid_access(r[14]+8u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c6454a4; /* mov.b @(0x8,r14),r0 */
L8c6454a4: *stop=0x8c6454a4u;r[0]=r[0]&255u;goto L8c6454a6; /* extu.b r0,r0 */
L8c6454a6: *stop=0x8c6454a6u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c6454a8; /* tst #0x2,r0 */
L8c6454a8: take=!(*sr&1u);*stop=0x8c6454aau;r[12]=r[12]&255u;if(take)goto L8c6454ae;goto L8c6454ac; /* bf/s 0x8c6454ae */
L8c6454ac: *stop=0x8c6454acu;r[12]+=4294967294u;goto L8c6454ae; /* add #-0x2,r12 */
L8c6454ae: *stop=0x8c6454aeu;r[5]=r[14];goto L8c6454b0; /* mov r14,r5 */
L8c6454b0: *stop=0x8c6454b0u;r[4]=r[13];goto L8c6454b2; /* mov r13,r4 */
L8c6454b2: *stop=0x8c6454b2u;r[5]+=10u;goto L8c6454b4; /* add #0xa,r5 */
L8c6454b4: *stop=0x8c6454b4u;r[6]=r[12];goto L8c6454b6; /* mov r12,r6 */
L8c6454b6: target=0x8c64535cu;*pr=0x8c6454bau;*stop=0x8c6454b8u;r[4]+=33u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6454ba; /* bsr 0x8c64535c */
L8c6454ba: *stop=0x8c6454bau;r[3]=r[14];goto L8c6454bc; /* mov r14,r3 */
L8c6454bc: *stop=0x8c6454bcu;r[3]+=10u;goto L8c6454be; /* add #0xa,r3 */
L8c6454be: *stop=0x8c6454beu;r[4]=0u;goto L8c6454c0; /* mov #0x0,r4 */
L8c6454c0: *stop=0x8c6454c0u;r[12]+=r[3];goto L8c6454c2; /* add r3,r12 */
L8c6454c2: *stop=0x8c6454c2u;if(!valid_access(r[12],1u))return -3;wr(m,bus,&io_status,r[12],r[4],1);if(io_status)return io_status;goto L8c6454c4; /* mov.b r4,@r12 */
L8c6454c4: *stop=0x8c6454c4u;if(!valid_access(r[14]+8u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c6454c6; /* mov.b @(0x8,r14),r0 */
L8c6454c6: *stop=0x8c6454c6u;r[0]=r[0]&255u;goto L8c6454c8; /* extu.b r0,r0 */
L8c6454c8: *stop=0x8c6454c8u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c6454ca; /* tst #0x2,r0 */
L8c6454ca: if((*sr&1u))goto L8c6454e8;goto L8c6454cc; /* bt 0x8c6454e8 */
L8c6454cc: *stop=0x8c6454ccu;if(!valid_access(r[14]+10u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+0xau,1);if(io_status)return io_status;r[0]=loaded;}goto L8c6454ce; /* mov.b @(0xa,r14),r0 */
L8c6454ce: *stop=0x8c6454ceu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6454d0; /* tst r0,r0 */
L8c6454d0: take=!(*sr&1u);*stop=0x8c6454d2u;r[5]=46u;if(take)goto L8c6454da;goto L8c6454d4; /* bf/s 0x8c6454da */
L8c6454d4: *stop=0x8c6454d4u;r[0]=r[5];goto L8c6454d6; /* mov r5,r0 */
L8c6454d6: *stop=0x8c6454d8u;if(!valid_access(r[14]+10u,1u))return -3;wr(m,bus,&io_status,r[14]+0xau,r[0],1);if(io_status)return io_status;goto L8c6454e8; /* bra 0x8c6454e8 */
L8c6454da: *stop=0x8c6454dau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6454dc; /* cmp/eq #0x1,r0 */
L8c6454dc: if(!(*sr&1u))goto L8c6454e8;goto L8c6454de; /* bf 0x8c6454e8 */
L8c6454de: *stop=0x8c6454deu;r[0]=r[5];goto L8c6454e0; /* mov r5,r0 */
L8c6454e0: *stop=0x8c6454e0u;if(!valid_access(r[14]+10u,1u))return -3;wr(m,bus,&io_status,r[14]+0xau,r[0],1);if(io_status)return io_status;goto L8c6454e2; /* mov.b r0,@(0xa,r14) */
L8c6454e2: *stop=0x8c6454e2u;if(!valid_access(r[14]+11u,1u))return -3;wr(m,bus,&io_status,r[14]+0xbu,r[0],1);if(io_status)return io_status;goto L8c6454e4; /* mov.b r0,@(0xb,r14) */
L8c6454e4: *stop=0x8c6454e4u;r[0]=r[4];goto L8c6454e6; /* mov r4,r0 */
L8c6454e6: *stop=0x8c6454e6u;if(!valid_access(r[14]+12u,1u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],1);if(io_status)return io_status;goto L8c6454e8; /* mov.b r0,@(0xc,r14) */
L8c6454e8: *stop=0x8c6454e8u;r[4]=r[14];goto L8c6454ea; /* mov r14,r4 */
L8c6454ea: target=0x8c645442u;*pr=0x8c6454eeu;*stop=0x8c6454ecu;r[4]+=10u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6454ee; /* bsr 0x8c645442 */
L8c6454ee: *stop=0x8c6454eeu;if(!valid_access(r[14]+9u,1u))return -3;wr(m,bus,&io_status,r[14]+0x9u,r[0],1);if(io_status)return io_status;goto L8c6454f0; /* mov.b r0,@(0x9,r14) */
L8c6454f0: *stop=0x8c6454f0u;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[0]=loaded;}goto L8c6454f2; /* mov.b @r13,r0 */
L8c6454f2: *stop=0x8c6454f2u;r[0]=r[0]&255u;goto L8c6454f4; /* extu.b r0,r0 */
L8c6454f4: *stop=0x8c6454f4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6454f6; /* lds.l @r15+,PR */
L8c6454f6: *stop=0x8c6454f6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6454f8; /* mov.l @r15+,r12 */
L8c6454f8: *stop=0x8c6454f8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6454fa; /* mov.l @r15+,r13 */
L8c6454fa: *stop=0x8c6454fau;if(*pr!=entry_pr)return -3;*stop=0x8c6454fcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c64535c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64535c;
L8c64535c: *stop=0x8c64535cu;*sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c64535e; /* cmp/pl r6 */
L8c64535e: *stop=0x8c64535eu;r[7]=r[4];goto L8c645360; /* mov r4,r7 */
L8c645360: take=!(*sr&1u);*stop=0x8c645362u;r[4]=r[5];if(take)goto L8c645370;goto L8c645364; /* bf/s 0x8c645370 */
L8c645364: *stop=0x8c645364u;if(!valid_access(r[7],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7],1);if(io_status)return io_status;r[3]=loaded;}r[7]+=1;goto L8c645366; /* mov.b @r7+,r3 */
L8c645366: *stop=0x8c645366u;r[6]+=4294967295u;goto L8c645368; /* add #-0x1,r6 */
L8c645368: *stop=0x8c645368u;*sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c64536a; /* cmp/pl r6 */
L8c64536a: *stop=0x8c64536au;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c64536c; /* mov.b r3,@r4 */
L8c64536c: take=(*sr&1u);*stop=0x8c64536eu;r[4]+=1u;if(take)goto L8c645364;goto L8c645370; /* bt/s 0x8c645364 */
L8c645370: *stop=0x8c645370u;if(*pr!=entry_pr)return -3;*stop=0x8c645372u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c600ddc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c600ddc;
L8c600ddc: *stop=0x8c600ddcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c600df0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c600dde; /* mov.l 0x8c600df0,r3 */
L8c600dde: target=r[3];*stop=0x8c600de0u;r[5]=0u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c10d974(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d974;
L8c10d974: *stop=0x8c10d974u;r[6]=0u;goto L8c10d976; /* mov #0x0,r6 */
L8c10d976: *stop=0x8c10d976u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d980u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d978; /* mov.l 0x8c10d980,r7 */
L8c10d978: *stop=0x8c10d978u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d984u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d97a; /* mov.l 0x8c10d984,r0 */
L8c10d97a: *stop=0x8c10d97au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d97c; /* mov.l @r0,r0 */
L8c10d97c: target=r[0];*stop=0x8c10d97eu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c60167e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60167e;
L8c60167e: *stop=0x8c60167eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601818u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601680; /* mov.l 0x8c601818,r3 */
L8c601680: *stop=0x8c601680u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c601682; /* mov.l r14,@-r15 */
L8c601682: *stop=0x8c601682u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[14]=loaded;}goto L8c601684; /* mov.l @r3,r14 */
L8c601684: *stop=0x8c601684u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c601686; /* tst r14,r14 */
L8c601686: if((*sr&1u))goto L8c60168e;goto L8c601688; /* bt 0x8c60168e */
L8c601688: *stop=0x8c601688u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c60168a; /* mov.l @(0x20,r14),r0 */
L8c60168a: target=r[0];*stop=0x8c60168cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
L8c60168e: *stop=0x8c60168eu;r[0]=0u;goto L8c601690; /* mov #0x0,r0 */
L8c601690: *stop=0x8c601690u;if(*pr!=entry_pr)return -3;*stop=0x8c601692u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_init_8c6002a2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6002a2;
L8c6002a2: *stop=0x8c6002a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6002a4; /* mov.l r14,@-r15 */
L8c6002a4: *stop=0x8c6002a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6002a6; /* mov.l r13,@-r15 */
L8c6002a6: *stop=0x8c6002a6u;r[13]=r[5];goto L8c6002a8; /* mov r5,r13 */
L8c6002a8: *stop=0x8c6002a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6002aa; /* mov.l r12,@-r15 */
L8c6002aa: *stop=0x8c6002aau;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c6002ac; /* tst r13,r13 */
L8c6002ac: *stop=0x8c6002acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6002ae; /* mov.l r11,@-r15 */
L8c6002ae: *stop=0x8c6002aeu;r[12]=r[4];goto L8c6002b0; /* mov r4,r12 */
L8c6002b0: *stop=0x8c6002b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6002b2; /* mov.l r10,@-r15 */
L8c6002b2: *stop=0x8c6002b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6002b4; /* mov.l r9,@-r15 */
L8c6002b4: *stop=0x8c6002b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6002b6; /* sts.l PR,@-r15 */
L8c6002b6: *stop=0x8c6002b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600354u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c6002b8; /* mov.l 0x8c600354,r9 */
L8c6002b8: take=!(*sr&1u);*stop=0x8c6002bau;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[14]=loaded;}if(take)goto L8c6002be;goto L8c6002bc; /* bf/s 0x8c6002be */
L8c6002bc: *stop=0x8c6002bcu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c6002be; /* mov.l @(0x10,r14),r13 */
L8c6002be: *stop=0x8c6002beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c600358u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6002c0; /* mov.l 0x8c600358,r11 */
L8c6002c0: *stop=0x8c6002c0u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c6002c2; /* mov.b @r12,r0 */
L8c6002c2: *stop=0x8c6002c2u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[10]=loaded;}goto L8c6002c4; /* mov.l @r11,r10 */
L8c6002c4: *stop=0x8c6002c4u;*sr=(*sr&~1u)|(r[0]==0x0000002fu);goto L8c6002c6; /* cmp/eq #0x2f,r0 */
L8c6002c6: take=(*sr&1u);*stop=0x8c6002c8u;r[4]=r[0];if(take)goto L8c6002d0;goto L8c6002ca; /* bt/s 0x8c6002d0 */
L8c6002ca: *stop=0x8c6002cau;r[0]=r[4];goto L8c6002cc; /* mov r4,r0 */
L8c6002cc: *stop=0x8c6002ccu;*sr=(*sr&~1u)|(r[0]==0x0000005cu);goto L8c6002ce; /* cmp/eq #0x5c,r0 */
L8c6002ce: if(!(*sr&1u))goto L8c60030e;goto L8c6002d0; /* bf 0x8c60030e */
L8c6002d0: *stop=0x8c6002d0u;if(!valid_access(r[10],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[0]=loaded;}goto L8c6002d2; /* mov.w @r10,r0 */
L8c6002d2: *stop=0x8c6002d2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6002d4; /* cmp/eq #0x1,r0 */
L8c6002d4: if(!(*sr&1u))goto L8c6002f0;goto L8c6002d6; /* bf 0x8c6002f0 */
L8c6002d6: *stop=0x8c6002d6u;r[2]=8u;goto L8c6002d8; /* mov #0x8,r2 */
L8c6002d8: *stop=0x8c6002d8u;r[4]=r[14];goto L8c6002da; /* mov r14,r4 */
L8c6002da: *stop=0x8c6002dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c60035cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6002dc; /* mov.l 0x8c60035c,r3 */
L8c6002dc: *stop=0x8c6002dcu;r[2]+=r[10];goto L8c6002de; /* add r10,r2 */
L8c6002de: *stop=0x8c6002deu;r[4]+=64u;goto L8c6002e0; /* add #0x40,r4 */
L8c6002e0: *stop=0x8c6002e0u;r[7]=r[12];goto L8c6002e2; /* mov r12,r7 */
L8c6002e2: *stop=0x8c6002e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6002e4; /* mov.l r2,@-r15 */
L8c6002e4: *stop=0x8c6002e4u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6002e6; /* mov.l @(0x4,r14),r5 */
L8c6002e6: *stop=0x8c6002e6u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6002e8; /* mov.l @(0x8,r14),r6 */
L8c6002e8: target=r[3];*pr=0x8c6002ecu;*stop=0x8c6002eau;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6002ec; /* jsr @r3 */
L8c6002ec: *stop=0x8c6002eeu;r[15]+=4u;goto L8c600300; /* bra 0x8c600300 */
L8c6002f0: *stop=0x8c6002f0u;r[4]=r[14];goto L8c6002f2; /* mov r14,r4 */
L8c6002f2: *stop=0x8c6002f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600360u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6002f4; /* mov.l 0x8c600360,r3 */
L8c6002f4: *stop=0x8c6002f4u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6002f6; /* mov.l @(0x4,r14),r5 */
L8c6002f6: *stop=0x8c6002f6u;r[7]=r[12];goto L8c6002f8; /* mov r12,r7 */
L8c6002f8: *stop=0x8c6002f8u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6002fa; /* mov.l @(0x8,r14),r6 */
L8c6002fa: *stop=0x8c6002fau;r[4]+=64u;goto L8c6002fc; /* add #0x40,r4 */
L8c6002fc: target=r[3];*pr=0x8c600300u;*stop=0x8c6002feu;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600300; /* jsr @r3 */
L8c600300: *stop=0x8c600300u;r[4]=r[0];goto L8c600302; /* mov r0,r4 */
L8c600302: *stop=0x8c600302u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c600304; /* tst r4,r4 */
L8c600304: if((*sr&1u))goto L8c60031e;goto L8c600306; /* bt 0x8c60031e */
L8c600306: *stop=0x8c600306u;{uint32_t loaded=sx(m,bus,&io_status,0x8c600342u,2);if(io_status)return io_status;r[7]=loaded;}goto L8c600308; /* mov.w 0x8c600342,r7 */
L8c600308: *stop=0x8c600308u;{uint32_t loaded=sx(m,bus,&io_status,0x8c600340u,2);if(io_status)return io_status;r[12]=loaded;}goto L8c60030a; /* mov.w 0x8c600340,r12 */
L8c60030a: *stop=0x8c60030cu;r[12]+=r[4];goto L8c600370; /* bra 0x8c600370 */
L8c60030e: *stop=0x8c60030eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c600364u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c600310; /* mov.l 0x8c600364,r3 */
L8c600310: *stop=0x8c600310u;r[6]=0u;goto L8c600312; /* mov #0x0,r6 */
L8c600312: *stop=0x8c600312u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c600314; /* mov.l @(0x10,r14),r5 */
L8c600314: target=r[3];*pr=0x8c600318u;*stop=0x8c600316u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600318; /* jsr @r3 */
L8c600318: *stop=0x8c600318u;r[5]=r[0];goto L8c60031a; /* mov r0,r5 */
L8c60031a: *stop=0x8c60031au;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c60031c; /* tst r5,r5 */
L8c60031c: if(!(*sr&1u))goto L8c600322;goto L8c60031e; /* bf 0x8c600322 */
L8c60031e: *stop=0x8c600320u;r[0]=4294967288u;goto L8c60039e; /* bra 0x8c60039e */
L8c600322: *stop=0x8c600322u;r[5]+=4294967295u;goto L8c600324; /* add #-0x1,r5 */
L8c600324: *stop=0x8c600324u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[4]=loaded;}goto L8c600326; /* mov.l @r9,r4 */
L8c600326: *stop=0x8c600326u;r[3]=44u;goto L8c600328; /* mov #0x2c,r3 */
L8c600328: *stop=0x8c600328u;*macl=r[3]*r[5];goto L8c60032a; /* mul.l r3,r5 */
L8c60032a: *stop=0x8c60032au;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c60032c; /* mov.l @(0x10,r4),r4 */
L8c60032c: *stop=0x8c60032cu;r[4]+=16u;goto L8c60032e; /* add #0x10,r4 */
L8c60032e: *stop=0x8c60032eu;r[5]=*macl;goto L8c600330; /* sts MACL,r5 */
L8c600330: *stop=0x8c600330u;r[4]+=r[5];goto L8c600332; /* add r5,r4 */
L8c600332: *stop=0x8c600332u;if(!valid_access(r[4]+8u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x8u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c600334; /* mov.b @(0x8,r4),r0 */
L8c600334: *stop=0x8c600334u;r[0]=r[0]&255u;goto L8c600336; /* extu.b r0,r0 */
L8c600336: *stop=0x8c600336u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c600338; /* tst #0x2,r0 */
L8c600338: if(!(*sr&1u))goto L8c600368;goto L8c60033a; /* bf 0x8c600368 */
L8c60033a: *stop=0x8c60033cu;r[0]=4294967285u;goto L8c60039e; /* bra 0x8c60039e */
L8c600368: *stop=0x8c600368u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[12]=loaded;}goto L8c60036a; /* mov.l @r4,r12 */
L8c60036a: *stop=0x8c60036au;{uint32_t loaded=sx(m,bus,&io_status,0x8c600470u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c60036c; /* mov.w 0x8c600470,r3 */
L8c60036c: *stop=0x8c60036cu;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c60036e; /* mov.l @(0x4,r4),r7 */
L8c60036e: *stop=0x8c60036eu;r[12]+=r[3];goto L8c600370; /* add r3,r12 */
L8c600370: *stop=0x8c600370u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[4]=loaded;}goto L8c600372; /* mov.l @r9,r4 */
L8c600372: *stop=0x8c600372u;r[6]=r[13];goto L8c600374; /* mov r13,r6 */
L8c600374: *stop=0x8c600374u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600474u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c600376; /* mov.l 0x8c600474,r3 */
L8c600376: *stop=0x8c600376u;r[5]=r[12];goto L8c600378; /* mov r12,r5 */
L8c600378: target=r[3];*pr=0x8c60037cu;*stop=0x8c60037au;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60037c; /* jsr @r3 */
L8c60037c: *stop=0x8c60037cu;r[14]=r[0];goto L8c60037e; /* mov r0,r14 */
L8c60037e: *stop=0x8c60037eu;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[0]=loaded;}goto L8c600380; /* mov.l @r11,r0 */
L8c600380: *stop=0x8c600380u;if(!valid_access(r[0],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;r[0]=loaded;}goto L8c600382; /* mov.w @r0,r0 */
L8c600382: *stop=0x8c600382u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c600384; /* cmp/eq #0x1,r0 */
L8c600384: if(!(*sr&1u))goto L8c60039c;goto L8c600386; /* bf 0x8c60039c */
L8c600386: *stop=0x8c600386u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c600388; /* mov.l @r11,r2 */
L8c600388: *stop=0x8c600388u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60038a; /* mov.l @(0x4,r2),r3 */
L8c60038a: *stop=0x8c60038au;*sr=(*sr&~1u)|(r[12]==r[3]);goto L8c60038c; /* cmp/eq r12,r3 */
L8c60038c: if(!(*sr&1u))goto L8c60039c;goto L8c60038e; /* bf 0x8c60039c */
L8c60038e: *stop=0x8c60038eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c600390; /* tst r14,r14 */
L8c600390: if((*sr&1u))goto L8c600398;goto L8c600392; /* bt 0x8c600398 */
L8c600392: *stop=0x8c600392u;r[0]=r[14];goto L8c600394; /* mov r14,r0 */
L8c600394: *stop=0x8c600394u;*sr=(*sr&~1u)|(r[0]==0xfffffff4u);goto L8c600396; /* cmp/eq #-0xc,r0 */
L8c600396: if(!(*sr&1u))goto L8c60039c;goto L8c600398; /* bf 0x8c60039c */
L8c600398: target=0x8c5ffcfeu;*pr=0x8c60039cu;*stop=0x8c60039au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60039c; /* bsr 0x8c5ffcfe */
L8c60039c: *stop=0x8c60039cu;r[0]=r[14];goto L8c60039e; /* mov r14,r0 */
L8c60039e: *stop=0x8c60039eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6003a0; /* lds.l @r15+,PR */
L8c6003a0: *stop=0x8c6003a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6003a2; /* mov.l @r15+,r9 */
L8c6003a2: *stop=0x8c6003a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6003a4; /* mov.l @r15+,r10 */
L8c6003a4: *stop=0x8c6003a4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6003a6; /* mov.l @r15+,r11 */
L8c6003a6: *stop=0x8c6003a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6003a8; /* mov.l @r15+,r12 */
L8c6003a8: *stop=0x8c6003a8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6003aa; /* mov.l @r15+,r13 */
L8c6003aa: *stop=0x8c6003aau;if(*pr!=entry_pr)return -3;*stop=0x8c6003acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
