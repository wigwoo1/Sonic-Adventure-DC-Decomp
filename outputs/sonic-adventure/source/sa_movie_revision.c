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
SA_STAGE_EXPORT int sa_movie_revision_8c6024fa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6024fa;
L8c6024fa: *stop=0x8c6024fau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6024fc; /* sts.l PR,@-r15 */
L8c6024fc: *stop=0x8c6024fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6024fe; /* mov.l 0x8c6025bc,r3 */
L8c6024fe: target=r[3];*pr=0x8c602502u;*stop=0x8c602500u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602502; /* jsr @r3 */
L8c602502: *stop=0x8c602502u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025c0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602504; /* mov.l 0x8c6025c0,r2 */
L8c602504: *stop=0x8c602504u;r[4]=0u;goto L8c602506; /* mov #0x0,r4 */
L8c602506: target=r[2];*stop=0x8c602508u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_movie_revision_8c601a34(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c601a34;
L8c601a34: *stop=0x8c601a34u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c601a36; /* mov.l r14,@-r15 */
L8c601a36: *stop=0x8c601a36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c601a38; /* mov.l r13,@-r15 */
L8c601a38: *stop=0x8c601a38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c601a3a; /* mov.l r12,@-r15 */
L8c601a3a: *stop=0x8c601a3au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c601a3c; /* mov.l r11,@-r15 */
L8c601a3c: *stop=0x8c601a3cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c601a3e; /* mov.l r10,@-r15 */
L8c601a3e: *stop=0x8c601a3eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c601a40; /* mov.l r9,@-r15 */
L8c601a40: *stop=0x8c601a40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c601a42; /* mov.l r8,@-r15 */
L8c601a42: *stop=0x8c601a42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c601a44; /* sts.l PR,@-r15 */
L8c601a44: *stop=0x8c601a44u;r[15]+=4294967288u;goto L8c601a46; /* add #-0x8,r15 */
L8c601a46: *stop=0x8c601a46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601ad8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601a48; /* mov.l 0x8c601ad8,r3 */
L8c601a48: *stop=0x8c601a48u;r[14]=0u;goto L8c601a4a; /* mov #0x0,r14 */
L8c601a4a: *stop=0x8c601a4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c601adcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601a4c; /* mov.l 0x8c601adc,r2 */
L8c601a4c: *stop=0x8c601a4cu;r[1]=1u;goto L8c601a4e; /* mov #0x1,r1 */
L8c601a4e: *stop=0x8c601a4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601ae0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601a50; /* mov.l 0x8c601ae0,r0 */
L8c601a50: *stop=0x8c601a50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601ae8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c601a52; /* mov.l 0x8c601ae8,r12 */
L8c601a52: *stop=0x8c601a52u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c601a54; /* mov.l r5,@(0x4,r15) */
L8c601a54: *stop=0x8c601a54u;r[5]=r[14];goto L8c601a56; /* mov r14,r5 */
L8c601a56: *stop=0x8c601a56u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c601a58; /* mov.l r6,@r15 */
L8c601a58: *stop=0x8c601a58u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c601a5a; /* mov.l r4,@r3 */
L8c601a5a: *stop=0x8c601a5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c601ae4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601a5c; /* mov.l 0x8c601ae4,r3 */
L8c601a5c: *stop=0x8c601a5cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c601a5e; /* mov.l r14,@r2 */
L8c601a5e: *stop=0x8c601a5eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c601ad2u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c601a60; /* mov.w 0x8c601ad2,r6 */
L8c601a60: *stop=0x8c601a60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601aecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601a62; /* mov.l 0x8c601aec,r2 */
L8c601a62: *stop=0x8c601a62u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c601a64; /* mov.l r1,@r0 */
L8c601a64: *stop=0x8c601a64u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c601a66; /* mov.l r14,@r3 */
L8c601a66: target=r[2];*pr=0x8c601a6au;*stop=0x8c601a68u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601a6a; /* jsr @r2 */
L8c601a6a: *stop=0x8c601a6au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c601a6c; /* mov.l @r15,r3 */
L8c601a6c: *stop=0x8c601a6cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c601a6e; /* tst r3,r3 */
L8c601a6e: if((*sr&1u))goto L8c601a84;goto L8c601a70; /* bt 0x8c601a84 */
L8c601a70: *stop=0x8c601a70u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601a72; /* mov.l @(0x4,r15),r3 */
L8c601a72: *stop=0x8c601a72u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c601a74; /* tst r3,r3 */
L8c601a74: if((*sr&1u))goto L8c601a84;goto L8c601a76; /* bt 0x8c601a84 */
L8c601a76: *stop=0x8c601a76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601adcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601a78; /* mov.l 0x8c601adc,r2 */
L8c601a78: *stop=0x8c601a78u;r[3]=1u;goto L8c601a7a; /* mov #0x1,r3 */
L8c601a7a: *stop=0x8c601a7au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c601a7c; /* mov.l r3,@r2 */
L8c601a7c: *stop=0x8c601a7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601af0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601a7e; /* mov.l 0x8c601af0,r3 */
L8c601a7e: *stop=0x8c601a7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c601a80; /* mov.l @r15,r5 */
L8c601a80: target=r[3];*pr=0x8c601a84u;*stop=0x8c601a82u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601a84; /* jsr @r3 */
L8c601a84: *stop=0x8c601a84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601af4u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c601a86; /* mov.l 0x8c601af4,r8 */
L8c601a86: *stop=0x8c601a86u;r[13]=r[14];goto L8c601a88; /* mov r14,r13 */
L8c601a88: *stop=0x8c601a88u;r[10]=24u;goto L8c601a8a; /* mov #0x18,r10 */
L8c601a8a: *stop=0x8c601a8au;r[9]=52u;goto L8c601a8c; /* mov #0x34,r9 */
L8c601a8c: *stop=0x8c601a8cu;*macl=r[9]*r[13];goto L8c601a8e; /* mul.l r9,r13 */
L8c601a8e: *stop=0x8c601a8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601af8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601a90; /* mov.l 0x8c601af8,r3 */
L8c601a90: *stop=0x8c601a90u;r[11]=*macl;goto L8c601a92; /* sts MACL,r11 */
L8c601a92: *stop=0x8c601a92u;r[11]+=r[12];goto L8c601a94; /* add r12,r11 */
L8c601a94: target=r[3];*pr=0x8c601a98u;*stop=0x8c601a96u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601a98; /* jsr @r3 */
L8c601a98: *stop=0x8c601a98u;if(!valid_access(r[11]+48u,4u))return -3;wr(m,bus,&io_status,r[11]+0x30u,r[0],4);if(io_status)return io_status;goto L8c601a9a; /* mov.l r0,@(0x30,r11) */
L8c601a9a: target=0x8c601bb2u;*pr=0x8c601a9eu;*stop=0x8c601a9cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601a9e; /* bsr 0x8c601bb2 */
L8c601a9e: *stop=0x8c601a9eu;r[0]=r[13];goto L8c601aa0; /* mov r13,r0 */
L8c601aa0: *stop=0x8c601aa0u;r[0]<<=2;goto L8c601aa2; /* shll2 r0 */
L8c601aa2: *stop=0x8c601aa2u;if(!valid_access(r[0]+r[8],4u))return -3;wr(m,bus,&io_status,r[8]+r[0],r[14],4);if(io_status)return io_status;goto L8c601aa4; /* mov.l r14,@(r0,r8) */
L8c601aa4: *stop=0x8c601aa4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601afcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601aa6; /* mov.l 0x8c601afc,r0 */
L8c601aa6: *stop=0x8c601aa6u;if(!valid_access(r[0]+r[13],1u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[14],1);if(io_status)return io_status;goto L8c601aa8; /* mov.b r14,@(r0,r13) */
L8c601aa8: *stop=0x8c601aa8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601b00u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601aaa; /* mov.l 0x8c601b00,r0 */
L8c601aaa: *stop=0x8c601aaau;if(!valid_access(r[0]+r[13],1u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[14],1);if(io_status)return io_status;goto L8c601aac; /* mov.b r14,@(r0,r13) */
L8c601aac: *stop=0x8c601aacu;r[13]+=1u;goto L8c601aae; /* add #0x1,r13 */
L8c601aae: *stop=0x8c601aaeu;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[10]);goto L8c601ab0; /* cmp/ge r10,r13 */
L8c601ab0: if(!(*sr&1u))goto L8c601a8c;goto L8c601ab2; /* bf 0x8c601a8c */
L8c601ab2: *stop=0x8c601ab2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601b08u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601ab4; /* mov.l 0x8c601b08,r0 */
L8c601ab4: *stop=0x8c601ab4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c601ad6u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c601ab6; /* mov.w 0x8c601ad6,r1 */
L8c601ab6: *stop=0x8c601ab6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c601ad4u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c601ab8; /* mov.w 0x8c601ad4,r2 */
L8c601ab8: *stop=0x8c601ab8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601b04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601aba; /* mov.l 0x8c601b04,r3 */
L8c601aba: *stop=0x8c601abau;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[2],1);if(io_status)return io_status;goto L8c601abc; /* mov.b r2,@r3 */
L8c601abc: *stop=0x8c601abcu;if(!valid_access(r[0],1u))return -3;wr(m,bus,&io_status,r[0],r[1],1);if(io_status)return io_status;goto L8c601abe; /* mov.b r1,@r0 */
L8c601abe: *stop=0x8c601abeu;r[15]+=8u;goto L8c601ac0; /* add #0x8,r15 */
L8c601ac0: *stop=0x8c601ac0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c601ac2; /* lds.l @r15+,PR */
L8c601ac2: *stop=0x8c601ac2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c601ac4; /* mov.l @r15+,r8 */
L8c601ac4: *stop=0x8c601ac4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c601ac6; /* mov.l @r15+,r9 */
L8c601ac6: *stop=0x8c601ac6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c601ac8; /* mov.l @r15+,r10 */
L8c601ac8: *stop=0x8c601ac8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c601aca; /* mov.l @r15+,r11 */
L8c601aca: *stop=0x8c601acau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c601acc; /* mov.l @r15+,r12 */
L8c601acc: *stop=0x8c601accu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c601ace; /* mov.l @r15+,r13 */
L8c601ace: *stop=0x8c601aceu;if(*pr!=entry_pr)return -3;*stop=0x8c601ad0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_revision_8c10ba48(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ba48;
L8c10ba48: *stop=0x8c10ba48u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10ba4a; /* sts.l PR,@-r15 */
L8c10ba4a: target=0x8c10be0cu;*pr=0x8c10ba4eu;*stop=0x8c10ba4cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10ba4e; /* bsr 0x8c10be0c */
L8c10ba4e: *stop=0x8c10ba4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10baf8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ba50; /* mov.l 0x8c10baf8,r2 */
L8c10ba50: *stop=0x8c10ba50u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c10ba52; /* mov.l r0,@r2 */
L8c10ba52: *stop=0x8c10ba52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10bafcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ba54; /* mov.l 0x8c10bafc,r1 */
L8c10ba54: *stop=0x8c10ba54u;r[3]=0u;goto L8c10ba56; /* mov #0x0,r3 */
L8c10ba56: *stop=0x8c10ba56u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c10ba58; /* mov.l r3,@r1 */
L8c10ba58: target=0x8c10ba80u;*pr=0x8c10ba5cu;*stop=0x8c10ba5au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10ba5c; /* bsr 0x8c10ba80 */
L8c10ba5c: *stop=0x8c10ba5cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10baf6u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c10ba5e; /* mov.w 0x8c10baf6,r4 */
L8c10ba5e: *stop=0x8c10ba5eu;r[7]=0u;goto L8c10ba60; /* mov #0x0,r7 */
L8c10ba60: *stop=0x8c10ba60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10bb04u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ba62; /* mov.l 0x8c10bb04,r2 */
L8c10ba62: *stop=0x8c10ba62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10bb00u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10ba64; /* mov.l 0x8c10bb00,r5 */
L8c10ba64: target=r[2];*pr=0x8c10ba68u;*stop=0x8c10ba66u;r[6]=92u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10ba68; /* jsr @r2 */
L8c10ba68: *stop=0x8c10ba68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10bb08u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ba6a; /* mov.l 0x8c10bb08,r3 */
L8c10ba6a: *stop=0x8c10ba6au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c10ba6c; /* mov.l r0,@r3 */
L8c10ba6c: *stop=0x8c10ba6cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10ba6e; /* lds.l @r15+,PR */
L8c10ba6e: *stop=0x8c10ba6eu;if(*pr!=entry_pr)return -3;*stop=0x8c10ba70u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_revision_8c6458fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6458fc;
L8c6458fc: *stop=0x8c6458fcu;r[15]+=4294967292u;goto L8c6458fe; /* add #-0x4,r15 */
L8c6458fe: *stop=0x8c6458feu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645900; /* stc SR,r0 */
L8c645900: *stop=0x8c645900u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6459fcu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645902; /* mov.w 0x8c6459fc,r3 */
L8c645902: *stop=0x8c645902u;r[0]>>=2;goto L8c645904; /* shlr2 r0 */
L8c645904: *stop=0x8c645904u;r[0]>>=2;goto L8c645906; /* shlr2 r0 */
L8c645906: *stop=0x8c645906u;r[0]&=0xfu;goto L8c645908; /* and #0xf,r0 */
L8c645908: *stop=0x8c645908u;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c64590a; /* mov.w r0,@r15 */
L8c64590a: *stop=0x8c64590au;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c64590c; /* stc SR,r0 */
L8c64590c: *stop=0x8c64590cu;r[0]&=r[3];goto L8c64590e; /* and r3,r0 */
L8c64590e: *stop=0x8c64590eu;r[0]|=0xf0u;goto L8c645910; /* or #0xf0,r0 */
L8c645910: *stop=0x8c645910u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645912; /* ldc r0,SR */
L8c645912: *stop=0x8c645912u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6459feu,2);if(io_status)return io_status;r[6]=loaded;}goto L8c645914; /* mov.w 0x8c6459fe,r6 */
L8c645914: *stop=0x8c645914u;r[5]=0u;goto L8c645916; /* mov #0x0,r5 */
L8c645916: *stop=0x8c645916u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645a0cu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c645918; /* mov.l 0x8c645a0c,r7 */
L8c645918: *stop=0x8c645918u;r[6]+=r[7];goto L8c64591a; /* add r7,r6 */
L8c64591a: *stop=0x8c64591au;r[4]=r[7];goto L8c64591c; /* mov r7,r4 */
L8c64591c: *stop=0x8c64591cu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[5],4);if(io_status)return io_status;goto L8c64591e; /* mov.l r5,@(0xc,r4) */
L8c64591e: *stop=0x8c64591eu;r[4]+=16u;goto L8c645920; /* add #0x10,r4 */
L8c645920: *stop=0x8c645920u;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c645922; /* cmp/hs r6,r4 */
L8c645922: if(!(*sr&1u))goto L8c64591c;goto L8c645924; /* bf 0x8c64591c */
L8c645924: *stop=0x8c645924u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645a00u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c645926; /* mov.w 0x8c645a00,r6 */
L8c645926: *stop=0x8c645926u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645a10u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c645928; /* mov.l 0x8c645a10,r7 */
L8c645928: *stop=0x8c645928u;r[6]+=r[7];goto L8c64592a; /* add r7,r6 */
L8c64592a: *stop=0x8c64592au;r[4]=r[7];goto L8c64592c; /* mov r7,r4 */
L8c64592c: *stop=0x8c64592cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c64592e; /* mov.l r5,@r4 */
L8c64592e: *stop=0x8c64592eu;r[4]+=4u;goto L8c645930; /* add #0x4,r4 */
L8c645930: *stop=0x8c645930u;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c645932; /* cmp/hs r6,r4 */
L8c645932: if(!(*sr&1u))goto L8c64592c;goto L8c645934; /* bf 0x8c64592c */
L8c645934: *stop=0x8c645934u;if(!valid_access(r[15],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;r[0]=loaded;}goto L8c645936; /* mov.w @r15,r0 */
L8c645936: *stop=0x8c645936u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c645938; /* stc SR,r3 */
L8c645938: *stop=0x8c645938u;r[0]=r[0]&65535u;goto L8c64593a; /* extu.w r0,r0 */
L8c64593a: *stop=0x8c64593au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6459fcu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c64593c; /* mov.w 0x8c6459fc,r2 */
L8c64593c: *stop=0x8c64593cu;r[0]&=0xfu;goto L8c64593e; /* and #0xf,r0 */
L8c64593e: *stop=0x8c64593eu;r[0]<<=2;goto L8c645940; /* shll2 r0 */
L8c645940: *stop=0x8c645940u;r[3]&=r[2];goto L8c645942; /* and r2,r3 */
L8c645942: *stop=0x8c645942u;r[0]<<=2;goto L8c645944; /* shll2 r0 */
L8c645944: *stop=0x8c645944u;r[0]|=r[3];goto L8c645946; /* or r3,r0 */
L8c645946: *stop=0x8c645946u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645948; /* ldc r0,SR */
L8c645948: *stop=0x8c645948u;if(*pr!=entry_pr)return -3;*stop=0x8c64594au;r[15]+=4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_revision_8c646304(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646304;
L8c646304: *stop=0x8c646304u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c646306; /* sts.l PR,@-r15 */
L8c646306: *stop=0x8c646306u;r[15]+=4294967284u;goto L8c646308; /* add #-0xc,r15 */
L8c646308: *stop=0x8c646308u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c64630a; /* stc SR,r0 */
L8c64630a: *stop=0x8c64630au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6463c4u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c64630c; /* mov.w 0x8c6463c4,r3 */
L8c64630c: *stop=0x8c64630cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c64630e; /* mov.l r4,@r15 */
L8c64630e: *stop=0x8c64630eu;r[0]>>=2;goto L8c646310; /* shlr2 r0 */
L8c646310: *stop=0x8c646310u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c646312; /* mov.l r5,@(0x4,r15) */
L8c646312: *stop=0x8c646312u;r[0]>>=2;goto L8c646314; /* shlr2 r0 */
L8c646314: *stop=0x8c646314u;r[0]&=0xfu;goto L8c646316; /* and #0xf,r0 */
L8c646316: *stop=0x8c646316u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c646318; /* mov.l r0,@(0x8,r15) */
L8c646318: *stop=0x8c646318u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c64631a; /* stc SR,r0 */
L8c64631a: *stop=0x8c64631au;r[0]&=r[3];goto L8c64631c; /* and r3,r0 */
L8c64631c: *stop=0x8c64631cu;r[0]|=0xf0u;goto L8c64631e; /* or #0xf0,r0 */
L8c64631e: *stop=0x8c64631eu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c646320; /* ldc r0,SR */
L8c646320: target=0x8c6461f0u;*pr=0x8c646324u;*stop=0x8c646322u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646324; /* bsr 0x8c6461f0 */
L8c646324: *stop=0x8c646324u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6463c8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c646326; /* mov.l 0x8c6463c8,r7 */
L8c646326: *stop=0x8c646326u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6463ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646328; /* mov.l 0x8c6463cc,r3 */
L8c646328: *stop=0x8c646328u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64632a; /* mov.l @r15,r6 */
L8c64632a: *stop=0x8c64632au;r[5]=r[7];goto L8c64632c; /* mov r7,r5 */
L8c64632c: target=r[3];*pr=0x8c646330u;*stop=0x8c64632eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646330; /* jsr @r3 */
L8c646330: target=0x8c6461aeu;*pr=0x8c646334u;*stop=0x8c646332u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646334; /* bsr 0x8c6461ae */
L8c646334: *stop=0x8c646334u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6463d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646336; /* mov.l 0x8c6463d0,r3 */
L8c646336: *stop=0x8c646336u;r[2]=1u;goto L8c646338; /* mov #0x1,r2 */
L8c646338: *stop=0x8c646338u;status=sa_sh4_stc_sr(*sr,&r[1],0u);if(status)return status;goto L8c64633a; /* stc SR,r1 */
L8c64633a: *stop=0x8c64633au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64633c; /* mov.l r2,@r3 */
L8c64633c: *stop=0x8c64633cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64633e; /* mov.l @(0x8,r15),r0 */
L8c64633e: *stop=0x8c64633eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6463c4u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c646340; /* mov.w 0x8c6463c4,r2 */
L8c646340: *stop=0x8c646340u;r[0]&=0xfu;goto L8c646342; /* and #0xf,r0 */
L8c646342: *stop=0x8c646342u;r[0]<<=2;goto L8c646344; /* shll2 r0 */
L8c646344: *stop=0x8c646344u;r[1]&=r[2];goto L8c646346; /* and r2,r1 */
L8c646346: *stop=0x8c646346u;r[0]<<=2;goto L8c646348; /* shll2 r0 */
L8c646348: *stop=0x8c646348u;r[0]|=r[1];goto L8c64634a; /* or r1,r0 */
L8c64634a: *stop=0x8c64634au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c64634c; /* ldc r0,SR */
L8c64634c: *stop=0x8c64634cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64634e; /* mov.l @r15,r0 */
L8c64634e: *stop=0x8c64634eu;r[15]+=12u;goto L8c646350; /* add #0xc,r15 */
L8c646350: *stop=0x8c646350u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646352; /* lds.l @r15+,PR */
L8c646352: *stop=0x8c646352u;if(*pr!=entry_pr)return -3;*stop=0x8c646354u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_revision_8c10be0c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10be0c;
L8c10be0c: *stop=0x8c10be0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10be0e; /* mov.l r14,@-r15 */
L8c10be0e: *stop=0x8c10be0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10be10; /* mov.l r13,@-r15 */
L8c10be10: *stop=0x8c10be10u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10be12; /* mov.l r12,@-r15 */
L8c10be12: *stop=0x8c10be12u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10be14; /* mov.l r11,@-r15 */
L8c10be14: *stop=0x8c10be14u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10be16; /* mov.l r10,@-r15 */
L8c10be16: *stop=0x8c10be16u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10be18; /* sts.l PR,@-r15 */
L8c10be18: *stop=0x8c10be18u;r[15]+=4294967288u;goto L8c10be1a; /* add #-0x8,r15 */
L8c10be1a: *stop=0x8c10be1au;r[12]=0u;goto L8c10be1c; /* mov #0x0,r12 */
L8c10be1c: *stop=0x8c10be1cu;r[13]=1u;goto L8c10be1e; /* mov #0x1,r13 */
L8c10be1e: *stop=0x8c10be1eu;r[14]=r[12];goto L8c10be20; /* mov r12,r14 */
L8c10be20: *stop=0x8c10be20u;r[4]=r[15];goto L8c10be22; /* mov r15,r4 */
L8c10be22: target=0x8c10be78u;*pr=0x8c10be26u;*stop=0x8c10be24u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10be26; /* bsr 0x8c10be78 */
L8c10be26: *stop=0x8c10be26u;r[4]=r[0];goto L8c10be28; /* mov r0,r4 */
L8c10be28: *stop=0x8c10be28u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c10be2a; /* tst r4,r4 */
L8c10be2a: if(!(*sr&1u))goto L8c10be2e;goto L8c10be2c; /* bf 0x8c10be2e */
L8c10be2c: *stop=0x8c10be2cu;r[14]=r[13];goto L8c10be2e; /* mov r13,r14 */
L8c10be2e: *stop=0x8c10be2eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c10be30; /* tst r14,r14 */
L8c10be30: if((*sr&1u))goto L8c10be20;goto L8c10be32; /* bt 0x8c10be20 */
L8c10be32: *stop=0x8c10be32u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c10be34; /* tst r14,r14 */
L8c10be34: if(!(*sr&1u))goto L8c10be3a;goto L8c10be36; /* bf 0x8c10be3a */
L8c10be36: *stop=0x8c10be38u;r[0]=4294967295u;goto L8c10be68; /* bra 0x8c10be68 */
L8c10be3a: *stop=0x8c10be3au;r[10]=3u;goto L8c10be3c; /* mov #0x3,r10 */
L8c10be3c: *stop=0x8c10be3cu;r[11]=r[12];goto L8c10be3e; /* mov r12,r11 */
L8c10be3e: *stop=0x8c10be3eu;r[14]=r[12];goto L8c10be40; /* mov r12,r14 */
L8c10be40: target=0x8c10be78u;*pr=0x8c10be44u;*stop=0x8c10be42u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10be44; /* bsr 0x8c10be78 */
L8c10be44: *stop=0x8c10be44u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10be46; /* tst r0,r0 */
L8c10be46: if(!(*sr&1u))goto L8c10be5a;goto L8c10be48; /* bf 0x8c10be5a */
L8c10be48: *stop=0x8c10be48u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10be4a; /* mov.l @(0x4,r15),r2 */
L8c10be4a: *stop=0x8c10be4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10be4c; /* mov.l @r15,r3 */
L8c10be4c: *stop=0x8c10be4cu;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c10be4e; /* cmp/eq r3,r2 */
L8c10be4e: if(!(*sr&1u))goto L8c10be5a;goto L8c10be50; /* bf 0x8c10be5a */
L8c10be50: *stop=0x8c10be50u;r[14]+=1u;goto L8c10be52; /* add #0x1,r14 */
L8c10be52: *stop=0x8c10be52u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10be54; /* mov.l @r15,r1 */
L8c10be54: *stop=0x8c10be54u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]);goto L8c10be56; /* cmp/ge r10,r14 */
L8c10be56: take=!(*sr&1u);*stop=0x8c10be58u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;if(take)goto L8c10be40;goto L8c10be5a; /* bf/s 0x8c10be40 */
L8c10be5a: *stop=0x8c10be5au;r[0]=r[14];goto L8c10be5c; /* mov r14,r0 */
L8c10be5c: *stop=0x8c10be5cu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c10be5e; /* cmp/eq #0x3,r0 */
L8c10be5e: if(!(*sr&1u))goto L8c10be62;goto L8c10be60; /* bf 0x8c10be62 */
L8c10be60: *stop=0x8c10be60u;r[11]=r[13];goto L8c10be62; /* mov r13,r11 */
L8c10be62: *stop=0x8c10be62u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c10be64; /* tst r11,r11 */
L8c10be64: if((*sr&1u))goto L8c10be3e;goto L8c10be66; /* bt 0x8c10be3e */
L8c10be66: *stop=0x8c10be66u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10be68; /* mov.l @r15,r0 */
L8c10be68: *stop=0x8c10be68u;r[15]+=8u;goto L8c10be6a; /* add #0x8,r15 */
L8c10be6a: *stop=0x8c10be6au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10be6c; /* lds.l @r15+,PR */
L8c10be6c: *stop=0x8c10be6cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10be6e; /* mov.l @r15+,r10 */
L8c10be6e: *stop=0x8c10be6eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10be70; /* mov.l @r15+,r11 */
L8c10be70: *stop=0x8c10be70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10be72; /* mov.l @r15+,r12 */
L8c10be72: *stop=0x8c10be72u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10be74; /* mov.l @r15+,r13 */
L8c10be74: *stop=0x8c10be74u;if(*pr!=entry_pr)return -3;*stop=0x8c10be76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_revision_8c10ba80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ba80;
L8c10ba80: *stop=0x8c10ba80u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10ba82; /* sts.l PR,@-r15 */
L8c10ba82: *stop=0x8c10ba82u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10bafcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ba84; /* mov.l 0x8c10bafc,r3 */
L8c10ba84: *stop=0x8c10ba84u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10ba86; /* mov.l @r3,r0 */
L8c10ba86: *stop=0x8c10ba86u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c10ba88; /* tst #0x1,r0 */
L8c10ba88: if(!(*sr&1u))goto L8c10ba94;goto L8c10ba8a; /* bf 0x8c10ba94 */
L8c10ba8a: *stop=0x8c10ba8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10baf8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ba8c; /* mov.l 0x8c10baf8,r1 */
L8c10ba8c: target=0x8c10bd96u;*pr=0x8c10ba90u;*stop=0x8c10ba8eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10ba90; /* bsr 0x8c10bd96 */
L8c10ba90: *stop=0x8c10ba90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10baf8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ba92; /* mov.l 0x8c10baf8,r3 */
L8c10ba92: *stop=0x8c10ba92u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c10ba94; /* mov.l r0,@r3 */
L8c10ba94: *stop=0x8c10ba94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10bafcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ba96; /* mov.l 0x8c10bafc,r2 */
L8c10ba96: *stop=0x8c10ba96u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10ba98; /* mov.l @r2,r0 */
L8c10ba98: *stop=0x8c10ba98u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c10ba9a; /* tst #0x2,r0 */
L8c10ba9a: if((*sr&1u))goto L8c10baa4;goto L8c10ba9c; /* bt 0x8c10baa4 */
L8c10ba9c: target=0x8c10be0cu;*pr=0x8c10baa0u;*stop=0x8c10ba9eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10baa0; /* bsr 0x8c10be0c */
L8c10baa0: *stop=0x8c10baa0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10baf8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10baa2; /* mov.l 0x8c10baf8,r3 */
L8c10baa2: *stop=0x8c10baa2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c10baa4; /* mov.l r0,@r3 */
L8c10baa4: *stop=0x8c10baa4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10baa6; /* lds.l @r15+,PR */
L8c10baa6: *stop=0x8c10baa6u;if(*pr!=entry_pr)return -3;*stop=0x8c10baa8u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_revision_8c6461f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6461f0;
L8c6461f0: *stop=0x8c6461f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6461f2; /* mov.l r14,@-r15 */
L8c6461f2: *stop=0x8c6461f2u;r[7]=0u;goto L8c6461f4; /* mov #0x0,r7 */
L8c6461f4: *stop=0x8c6461f4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6461f6; /* mov.l r13,@-r15 */
L8c6461f6: *stop=0x8c6461f6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6461f8; /* mov.l r12,@-r15 */
L8c6461f8: *stop=0x8c6461f8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6461fa; /* sts.l PR,@-r15 */
L8c6461fa: *stop=0x8c6461fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462c4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6461fc; /* mov.l 0x8c6462c4,r5 */
L8c6461fc: *stop=0x8c6461fcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c646298u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6461fe; /* mov.w 0x8c646298,r4 */
L8c6461fe: *stop=0x8c6461feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462c0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c646200; /* mov.l 0x8c6462c0,r14 */
L8c646200: *stop=0x8c646200u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462b8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c646202; /* mov.l 0x8c6462b8,r12 */
L8c646202: *stop=0x8c646202u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462bcu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c646204; /* mov.l 0x8c6462bc,r13 */
L8c646204: target=r[13];*pr=0x8c646208u;*stop=0x8c646206u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[6]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646208; /* jsr @r13 */
L8c646208: *stop=0x8c646208u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462c8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64620a; /* mov.l 0x8c6462c8,r5 */
L8c64620a: *stop=0x8c64620au;r[7]=0u;goto L8c64620c; /* mov #0x0,r7 */
L8c64620c: *stop=0x8c64620cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c64629au,2);if(io_status)return io_status;r[4]=loaded;}goto L8c64620e; /* mov.w 0x8c64629a,r4 */
L8c64620e: *stop=0x8c64620eu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c646210; /* mov.l r0,@r14 */
L8c646210: target=r[13];*pr=0x8c646214u;*stop=0x8c646212u;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646214; /* jsr @r13 */
L8c646214: *stop=0x8c646214u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462ccu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c646216; /* mov.l 0x8c6462cc,r5 */
L8c646216: *stop=0x8c646216u;r[7]=0u;goto L8c646218; /* mov #0x0,r7 */
L8c646218: *stop=0x8c646218u;{uint32_t loaded=sx(m,bus,&io_status,0x8c64629au,2);if(io_status)return io_status;r[4]=loaded;}goto L8c64621a; /* mov.w 0x8c64629a,r4 */
L8c64621a: *stop=0x8c64621au;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],4);if(io_status)return io_status;goto L8c64621c; /* mov.l r0,@(0x4,r14) */
L8c64621c: target=r[13];*pr=0x8c646220u;*stop=0x8c64621eu;if(!valid_access(r[12]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646220; /* jsr @r13 */
L8c646220: *stop=0x8c646220u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462d0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c646222; /* mov.l 0x8c6462d0,r5 */
L8c646222: *stop=0x8c646222u;r[7]=0u;goto L8c646224; /* mov #0x0,r7 */
L8c646224: *stop=0x8c646224u;{uint32_t loaded=sx(m,bus,&io_status,0x8c64629cu,2);if(io_status)return io_status;r[4]=loaded;}goto L8c646226; /* mov.w 0x8c64629c,r4 */
L8c646226: *stop=0x8c646226u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],4);if(io_status)return io_status;goto L8c646228; /* mov.l r0,@(0x8,r14) */
L8c646228: target=r[13];*pr=0x8c64622cu;*stop=0x8c64622au;if(!valid_access(r[12]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64622c; /* jsr @r13 */
L8c64622c: *stop=0x8c64622cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64622e; /* mov.l 0x8c6462d4,r2 */
L8c64622e: *stop=0x8c64622eu;r[3]=0u;goto L8c646230; /* mov #0x0,r3 */
L8c646230: *stop=0x8c646230u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],4);if(io_status)return io_status;goto L8c646232; /* mov.l r0,@(0xc,r14) */
L8c646232: *stop=0x8c646232u;r[1]=r[3];goto L8c646234; /* mov r3,r1 */
L8c646234: *stop=0x8c646234u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462acu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c646236; /* mov.l 0x8c6462ac,r0 */
L8c646236: *stop=0x8c646236u;r[6]=56u;goto L8c646238; /* mov #0x38,r6 */
L8c646238: *stop=0x8c646238u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462d8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64623a; /* mov.l 0x8c6462d8,r1 */
L8c64623a: *stop=0x8c64623au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64623c; /* mov.l r3,@r2 */
L8c64623c: *stop=0x8c64623cu;r[2]=r[3];goto L8c64623e; /* mov r3,r2 */
L8c64623e: *stop=0x8c64623eu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c646240; /* mov.l r3,@r0 */
L8c646240: *stop=0x8c646240u;r[0]=0u;goto L8c646242; /* mov #0x0,r0 */
L8c646242: *stop=0x8c646242u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c646244; /* mov.l r3,@r1 */
L8c646244: *stop=0x8c646244u;r[1]=r[0];goto L8c646246; /* mov r0,r1 */
L8c646246: *stop=0x8c646246u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646248; /* mov.l 0x8c6462dc,r3 */
L8c646248: *stop=0x8c646248u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462f8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64624a; /* mov.l 0x8c6462f8,r14 */
L8c64624a: *stop=0x8c64624au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64624c; /* mov.l r2,@r3 */
L8c64624c: *stop=0x8c64624cu;r[3]=4294967295u;goto L8c64624e; /* mov #-0x1,r3 */
L8c64624e: *stop=0x8c64624eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462e0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646250; /* mov.l 0x8c6462e0,r2 */
L8c646250: *stop=0x8c646250u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c646252; /* mov.l r0,@r2 */
L8c646252: *stop=0x8c646252u;r[2]=0u;goto L8c646254; /* mov #0x0,r2 */
L8c646254: *stop=0x8c646254u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462e4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c646256; /* mov.l 0x8c6462e4,r0 */
L8c646256: *stop=0x8c646256u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c646258; /* mov.l r1,@r0 */
L8c646258: *stop=0x8c646258u;r[0]=r[2];goto L8c64625a; /* mov r2,r0 */
L8c64625a: *stop=0x8c64625au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462e8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64625c; /* mov.l 0x8c6462e8,r1 */
L8c64625c: *stop=0x8c64625cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c64625e; /* mov.l r3,@r1 */
L8c64625e: *stop=0x8c64625eu;r[1]=0u;goto L8c646260; /* mov #0x0,r1 */
L8c646260: *stop=0x8c646260u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646262; /* mov.l 0x8c6462ec,r3 */
L8c646262: *stop=0x8c646262u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c646264; /* mov.l r2,@r3 */
L8c646264: *stop=0x8c646264u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646266; /* mov.l 0x8c6462f0,r2 */
L8c646266: *stop=0x8c646266u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c646268; /* mov.l r0,@r2 */
L8c646268: *stop=0x8c646268u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462f4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64626a; /* mov.l 0x8c6462f4,r0 */
L8c64626a: *stop=0x8c64626au;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c64626c; /* mov.l r1,@r0 */
L8c64626c: *stop=0x8c64626cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462a8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64626e; /* mov.l 0x8c6462a8,r4 */
L8c64626e: target=r[14];*pr=0x8c646272u;*stop=0x8c646270u;r[5]=r[1];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646272; /* jsr @r14 */
L8c646272: *stop=0x8c646272u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462b0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c646274; /* mov.l 0x8c6462b0,r4 */
L8c646274: *stop=0x8c646274u;{uint32_t loaded=sx(m,bus,&io_status,0x8c64629eu,2);if(io_status)return io_status;r[6]=loaded;}goto L8c646276; /* mov.w 0x8c64629e,r6 */
L8c646276: target=r[14];*pr=0x8c64627au;*stop=0x8c646278u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64627a; /* jsr @r14 */
L8c64627a: *stop=0x8c64627au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462fcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64627c; /* mov.l 0x8c6462fc,r4 */
L8c64627c: *stop=0x8c64627cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c64629eu,2);if(io_status)return io_status;r[6]=loaded;}goto L8c64627e; /* mov.w 0x8c64629e,r6 */
L8c64627e: target=r[14];*pr=0x8c646282u;*stop=0x8c646280u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646282; /* jsr @r14 */
L8c646282: *stop=0x8c646282u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646300u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c646284; /* mov.l 0x8c646300,r4 */
L8c646284: *stop=0x8c646284u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6462a0u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c646286; /* mov.w 0x8c6462a0,r6 */
L8c646286: target=r[14];*pr=0x8c64628au;*stop=0x8c646288u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64628a; /* jsr @r14 */
L8c64628a: *stop=0x8c64628au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64628c; /* lds.l @r15+,PR */
L8c64628c: *stop=0x8c64628cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64628e; /* mov.l @r15+,r12 */
L8c64628e: *stop=0x8c64628eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c646290; /* mov.l @r15+,r13 */
L8c646290: *stop=0x8c646290u;if(*pr!=entry_pr)return -3;*stop=0x8c646292u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_revision_8c65ebb0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65ebb0;
L8c65ebb0: *stop=0x8c65ebb0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65ebb2; /* mov.l r14,@-r15 */
L8c65ebb2: *stop=0x8c65ebb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ecacu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65ebb4; /* mov.l 0x8c65ecac,r14 */
L8c65ebb4: *stop=0x8c65ebb4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ecb0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ebb6; /* mov.l 0x8c65ecb0,r3 */
L8c65ebb6: *stop=0x8c65ebb6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ec9cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ebb8; /* mov.w 0x8c65ec9c,r0 */
L8c65ebb8: *stop=0x8c65ebb8u;r[2]=r[3];goto L8c65ebba; /* mov r3,r2 */
L8c65ebba: *stop=0x8c65ebbau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65ebbc; /* mov.l r13,@-r15 */
L8c65ebbc: *stop=0x8c65ebbcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65ebbe; /* mov.l r12,@-r15 */
L8c65ebbe: *stop=0x8c65ebbeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65ebc0; /* mov.l r11,@-r15 */
L8c65ebc0: *stop=0x8c65ebc0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65ebc2; /* mov.l r10,@-r15 */
L8c65ebc2: *stop=0x8c65ebc2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65ebc4; /* sts.l PR,@-r15 */
L8c65ebc4: *stop=0x8c65ebc4u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c65ebc6; /* mov.l r3,@r14 */
L8c65ebc6: *stop=0x8c65ebc6u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[4],4);if(io_status)return io_status;goto L8c65ebc8; /* mov.l r4,@(r0,r2) */
L8c65ebc8: *stop=0x8c65ebc8u;r[0]+=4u;goto L8c65ebca; /* add #0x4,r0 */
L8c65ebca: *stop=0x8c65ebcau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ebcc; /* mov.l @r14,r3 */
L8c65ebcc: *stop=0x8c65ebccu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[6],4);if(io_status)return io_status;goto L8c65ebce; /* mov.l r6,@(r0,r3) */
L8c65ebce: *stop=0x8c65ebceu;r[0]+=4294967292u;goto L8c65ebd0; /* add #-0x4,r0 */
L8c65ebd0: *stop=0x8c65ebd0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ebd2; /* mov.l @r14,r3 */
L8c65ebd2: *stop=0x8c65ebd2u;r[6]=31u;goto L8c65ebd4; /* mov #0x1f,r6 */
L8c65ebd4: *stop=0x8c65ebd4u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ebd6; /* mov.l @(r0,r3),r2 */
L8c65ebd6: *stop=0x8c65ebd6u;*sr=(*sr&~1u)|!(r[6]&r[2]);goto L8c65ebd8; /* tst r6,r2 */
L8c65ebd8: take=(*sr&1u);*stop=0x8c65ebdau;r[5]=4294967264u;if(take)goto L8c65ebee;goto L8c65ebdc; /* bt/s 0x8c65ebee */
L8c65ebdc: *stop=0x8c65ebdcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ec9cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ebde; /* mov.w 0x8c65ec9c,r0 */
L8c65ebde: *stop=0x8c65ebdeu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ebe0; /* mov.l @r14,r1 */
L8c65ebe0: *stop=0x8c65ebe0u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ebe2; /* mov.l @(r0,r1),r3 */
L8c65ebe2: *stop=0x8c65ebe2u;r[3]+=32u;goto L8c65ebe4; /* add #0x20,r3 */
L8c65ebe4: *stop=0x8c65ebe4u;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[3],4);if(io_status)return io_status;goto L8c65ebe6; /* mov.l r3,@(r0,r1) */
L8c65ebe6: *stop=0x8c65ebe6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ebe8; /* mov.l @r14,r3 */
L8c65ebe8: *stop=0x8c65ebe8u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ebea; /* mov.l @(r0,r3),r2 */
L8c65ebea: *stop=0x8c65ebeau;r[2]&=r[5];goto L8c65ebec; /* and r5,r2 */
L8c65ebec: *stop=0x8c65ebecu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65ebee; /* mov.l r2,@(r0,r3) */
L8c65ebee: *stop=0x8c65ebeeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ec9eu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ebf0; /* mov.w 0x8c65ec9e,r0 */
L8c65ebf0: *stop=0x8c65ebf0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65ebf2; /* mov.l @r14,r4 */
L8c65ebf2: *stop=0x8c65ebf2u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ebf4; /* mov.l @(r0,r4),r3 */
L8c65ebf4: *stop=0x8c65ebf4u;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c65ebf6; /* tst r3,r6 */
L8c65ebf6: if((*sr&1u))goto L8c65ec06;goto L8c65ebf8; /* bt 0x8c65ec06 */
L8c65ebf8: *stop=0x8c65ebf8u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ebfa; /* mov.l @(r0,r4),r3 */
L8c65ebfa: *stop=0x8c65ebfau;r[3]+=32u;goto L8c65ebfc; /* add #0x20,r3 */
L8c65ebfc: *stop=0x8c65ebfcu;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[3],4);if(io_status)return io_status;goto L8c65ebfe; /* mov.l r3,@(r0,r4) */
L8c65ebfe: *stop=0x8c65ebfeu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec00; /* mov.l @r14,r3 */
L8c65ec00: *stop=0x8c65ec00u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ec02; /* mov.l @(r0,r3),r2 */
L8c65ec02: *stop=0x8c65ec02u;r[2]&=r[5];goto L8c65ec04; /* and r5,r2 */
L8c65ec04: *stop=0x8c65ec04u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65ec06; /* mov.l r2,@(r0,r3) */
L8c65ec06: *stop=0x8c65ec06u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ec9cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ec08; /* mov.w 0x8c65ec9c,r0 */
L8c65ec08: *stop=0x8c65ec08u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec0a; /* mov.l @r14,r3 */
L8c65ec0a: *stop=0x8c65ec0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ecb4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65ec0c; /* mov.l 0x8c65ecb4,r4 */
L8c65ec0c: *stop=0x8c65ec0cu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ec0e; /* mov.l @(r0,r3),r2 */
L8c65ec0e: *stop=0x8c65ec0eu;r[2]|=r[4];goto L8c65ec10; /* or r4,r2 */
L8c65ec10: *stop=0x8c65ec10u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65ec12; /* mov.l r2,@(r0,r3) */
L8c65ec12: *stop=0x8c65ec12u;r[0]+=4u;goto L8c65ec14; /* add #0x4,r0 */
L8c65ec14: *stop=0x8c65ec14u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec16; /* mov.l @r14,r3 */
L8c65ec16: *stop=0x8c65ec16u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ec18; /* mov.l @(r0,r3),r2 */
L8c65ec18: *stop=0x8c65ec18u;r[2]|=r[4];goto L8c65ec1a; /* or r4,r2 */
L8c65ec1a: *stop=0x8c65ec1au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65ec1c; /* mov.l r2,@(r0,r3) */
L8c65ec1c: *stop=0x8c65ec1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ecb8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec1e; /* mov.l 0x8c65ecb8,r3 */
L8c65ec1e: target=r[3];*pr=0x8c65ec22u;*stop=0x8c65ec20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ec22; /* jsr @r3 */
L8c65ec22: *stop=0x8c65ec22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ecbcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65ec24; /* mov.l 0x8c65ecbc,r4 */
L8c65ec24: *stop=0x8c65ec24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ecc0u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c65ec26; /* mov.l 0x8c65ecc0,r11 */
L8c65ec26: *stop=0x8c65ec26u;r[10]=r[4];goto L8c65ec28; /* mov r4,r10 */
L8c65ec28: *stop=0x8c65ec28u;r[10]+=16u;goto L8c65ec2a; /* add #0x10,r10 */
L8c65ec2a: *stop=0x8c65ec2au;r[5]=r[4];goto L8c65ec2c; /* mov r4,r5 */
L8c65ec2c: *stop=0x8c65ec2cu;r[12]=r[4];goto L8c65ec2e; /* mov r4,r12 */
L8c65ec2e: *stop=0x8c65ec2eu;r[13]=r[4];goto L8c65ec30; /* mov r4,r13 */
L8c65ec30: *stop=0x8c65ec30u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65eca0u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c65ec32; /* mov.w 0x8c65eca0,r2 */
L8c65ec32: *stop=0x8c65ec32u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65ec34; /* mov.l @r14,r0 */
L8c65ec34: *stop=0x8c65ec34u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec36; /* mov.l @r13,r3 */
L8c65ec36: *stop=0x8c65ec36u;r[2]+=r[0];goto L8c65ec38; /* add r0,r2 */
L8c65ec38: *stop=0x8c65ec38u;r[2]+=r[3];goto L8c65ec3a; /* add r3,r2 */
L8c65ec3a: *stop=0x8c65ec3au;if(!valid_access(r[2],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;r[1]=loaded;}goto L8c65ec3c; /* mov.b @r2,r1 */
L8c65ec3c: *stop=0x8c65ec3cu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c65ec3e; /* tst r1,r1 */
L8c65ec3e: if((*sr&1u))goto L8c65ec44;goto L8c65ec40; /* bt 0x8c65ec44 */
L8c65ec40: target=r[11];*pr=0x8c65ec44u;*stop=0x8c65ec42u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ec44; /* jsr @r11 */
L8c65ec44: *stop=0x8c65ec44u;r[13]+=4u;goto L8c65ec46; /* add #0x4,r13 */
L8c65ec46: *stop=0x8c65ec46u;*sr=(*sr&~1u)|(r[13]>=r[10]);goto L8c65ec48; /* cmp/hs r10,r13 */
L8c65ec48: take=!(*sr&1u);*stop=0x8c65ec4au;r[12]+=4u;if(take)goto L8c65ec30;goto L8c65ec4c; /* bf/s 0x8c65ec30 */
L8c65ec4c: *stop=0x8c65ec4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ecc4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ec4e; /* mov.l 0x8c65ecc4,r2 */
L8c65ec4e: target=r[2];*pr=0x8c65ec52u;*stop=0x8c65ec50u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ec52; /* jsr @r2 */
L8c65ec52: *stop=0x8c65ec52u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65eca2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ec54; /* mov.w 0x8c65eca2,r0 */
L8c65ec54: *stop=0x8c65ec54u;r[4]=1u;goto L8c65ec56; /* mov #0x1,r4 */
L8c65ec56: *stop=0x8c65ec56u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec58; /* mov.l @r14,r3 */
L8c65ec58: *stop=0x8c65ec58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ecc8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ec5a; /* mov.l 0x8c65ecc8,r1 */
L8c65ec5a: *stop=0x8c65ec5au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[4],4);if(io_status)return io_status;goto L8c65ec5c; /* mov.l r4,@(r0,r3) */
L8c65ec5c: *stop=0x8c65ec5cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65eca4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ec5e; /* mov.w 0x8c65eca4,r0 */
L8c65ec5e: *stop=0x8c65ec5eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec60; /* mov.l @r14,r3 */
L8c65ec60: *stop=0x8c65ec60u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ec62; /* mov.l @(r0,r3),r2 */
L8c65ec62: *stop=0x8c65ec62u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65eca6u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ec64; /* mov.w 0x8c65eca6,r0 */
L8c65ec64: *stop=0x8c65ec64u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c65ec66; /* mov.l r1,@(r0,r2) */
L8c65ec66: *stop=0x8c65ec66u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65eca4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ec68; /* mov.w 0x8c65eca4,r0 */
L8c65ec68: *stop=0x8c65ec68u;r[5]=0u;goto L8c65ec6a; /* mov #0x0,r5 */
L8c65ec6a: *stop=0x8c65ec6au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec6c; /* mov.l @r14,r3 */
L8c65ec6c: *stop=0x8c65ec6cu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ec6e; /* mov.l @(r0,r3),r2 */
L8c65ec6e: *stop=0x8c65ec6eu;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[5],4);if(io_status)return io_status;goto L8c65ec70; /* mov.l r5,@(0x10,r2) */
L8c65ec70: *stop=0x8c65ec70u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec72; /* mov.l @r14,r3 */
L8c65ec72: *stop=0x8c65ec72u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ec74; /* mov.l @(r0,r3),r2 */
L8c65ec74: *stop=0x8c65ec74u;r[0]+=4294967260u;goto L8c65ec76; /* add #-0x24,r0 */
L8c65ec76: *stop=0x8c65ec76u;r[1]=r[3];goto L8c65ec78; /* mov r3,r1 */
L8c65ec78: *stop=0x8c65ec78u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec7a; /* mov.l @(r0,r3),r3 */
L8c65ec7a: *stop=0x8c65ec7au;r[0]+=4294967280u;goto L8c65ec7c; /* add #-0x10,r0 */
L8c65ec7c: *stop=0x8c65ec7cu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ec7e; /* mov.l @(r0,r1),r1 */
L8c65ec7e: *stop=0x8c65ec7eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65eca8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ec80; /* mov.w 0x8c65eca8,r0 */
L8c65ec80: *stop=0x8c65ec80u;r[1]|=r[3];goto L8c65ec82; /* or r3,r1 */
L8c65ec82: *stop=0x8c65ec82u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c65ec84; /* mov.l r1,@(r0,r2) */
L8c65ec84: *stop=0x8c65ec84u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65eca4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ec86; /* mov.w 0x8c65eca4,r0 */
L8c65ec86: *stop=0x8c65ec86u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ec88; /* mov.l @r14,r3 */
L8c65ec88: *stop=0x8c65ec88u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ec8a; /* mov.l @(r0,r3),r2 */
L8c65ec8a: *stop=0x8c65ec8au;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[4],4);if(io_status)return io_status;goto L8c65ec8c; /* mov.l r4,@(0x14,r2) */
L8c65ec8c: *stop=0x8c65ec8cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65ec8e; /* lds.l @r15+,PR */
L8c65ec8e: *stop=0x8c65ec8eu;r[0]=r[5];goto L8c65ec90; /* mov r5,r0 */
L8c65ec90: *stop=0x8c65ec90u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65ec92; /* mov.l @r15+,r10 */
L8c65ec92: *stop=0x8c65ec92u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65ec94; /* mov.l @r15+,r11 */
L8c65ec94: *stop=0x8c65ec94u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65ec96; /* mov.l @r15+,r12 */
L8c65ec96: *stop=0x8c65ec96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65ec98; /* mov.l @r15+,r13 */
L8c65ec98: *stop=0x8c65ec98u;if(*pr!=entry_pr)return -3;*stop=0x8c65ec9au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_revision_8c6461ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6461ae;
L8c6461ae: *stop=0x8c6461aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462b4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6461b0; /* mov.l 0x8c6462b4,r3 */
L8c6461b0: *stop=0x8c6461b0u;r[6]=0u;goto L8c6461b2; /* mov #0x0,r6 */
L8c6461b2: *stop=0x8c6461b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6462b0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6461b4; /* mov.l 0x8c6462b0,r5 */
L8c6461b4: *stop=0x8c6461b4u;r[7]=24u;goto L8c6461b6; /* mov #0x18,r7 */
L8c6461b6: *stop=0x8c6461b6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6461b8; /* mov.l @r3,r4 */
L8c6461b8: *stop=0x8c6461b8u;r[4]+=40u;goto L8c6461ba; /* add #0x28,r4 */
L8c6461ba: *stop=0x8c6461bau;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[2]=loaded;}goto L8c6461bc; /* mov.b @r4,r2 */
L8c6461bc: *stop=0x8c6461bcu;r[6]+=1u;goto L8c6461be; /* add #0x1,r6 */
L8c6461be: *stop=0x8c6461beu;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c6461c0; /* cmp/ge r7,r6 */
L8c6461c0: *stop=0x8c6461c0u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[2],1);if(io_status)return io_status;goto L8c6461c2; /* mov.b r2,@r5 */
L8c6461c2: *stop=0x8c6461c2u;if(!valid_access(r[4]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c6461c4; /* mov.b @(0x1,r4),r0 */
L8c6461c4: *stop=0x8c6461c4u;if(!valid_access(r[5]+1u,1u))return -3;wr(m,bus,&io_status,r[5]+0x1u,r[0],1);if(io_status)return io_status;goto L8c6461c6; /* mov.b r0,@(0x1,r5) */
L8c6461c6: *stop=0x8c6461c6u;if(!valid_access(r[4]+2u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x2u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c6461c8; /* mov.b @(0x2,r4),r0 */
L8c6461c8: *stop=0x8c6461c8u;if(!valid_access(r[5]+2u,1u))return -3;wr(m,bus,&io_status,r[5]+0x2u,r[0],1);if(io_status)return io_status;goto L8c6461ca; /* mov.b r0,@(0x2,r5) */
L8c6461ca: *stop=0x8c6461cau;if(!valid_access(r[4]+3u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x3u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c6461cc; /* mov.b @(0x3,r4),r0 */
L8c6461cc: *stop=0x8c6461ccu;if(!valid_access(r[5]+3u,1u))return -3;wr(m,bus,&io_status,r[5]+0x3u,r[0],1);if(io_status)return io_status;goto L8c6461ce; /* mov.b r0,@(0x3,r5) */
L8c6461ce: *stop=0x8c6461ceu;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6461d0; /* mov.l @(0x10,r4),r3 */
L8c6461d0: *stop=0x8c6461d0u;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6461d2; /* mov.l r3,@(0x8,r5) */
L8c6461d2: *stop=0x8c6461d2u;if(!valid_access(r[4]+8u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x8u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c6461d4; /* mov.b @(0x8,r4),r0 */
L8c6461d4: *stop=0x8c6461d4u;if(!valid_access(r[5]+4u,1u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[0],1);if(io_status)return io_status;goto L8c6461d6; /* mov.b r0,@(0x4,r5) */
L8c6461d6: *stop=0x8c6461d6u;if(!valid_access(r[4]+9u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c6461d8; /* mov.b @(0x9,r4),r0 */
L8c6461d8: *stop=0x8c6461d8u;if(!valid_access(r[5]+5u,1u))return -3;wr(m,bus,&io_status,r[5]+0x5u,r[0],1);if(io_status)return io_status;goto L8c6461da; /* mov.b r0,@(0x5,r5) */
L8c6461da: *stop=0x8c6461dau;if(!valid_access(r[4]+10u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0xau,1);if(io_status)return io_status;r[0]=loaded;}goto L8c6461dc; /* mov.b @(0xa,r4),r0 */
L8c6461dc: *stop=0x8c6461dcu;if(!valid_access(r[5]+6u,1u))return -3;wr(m,bus,&io_status,r[5]+0x6u,r[0],1);if(io_status)return io_status;goto L8c6461de; /* mov.b r0,@(0x6,r5) */
L8c6461de: *stop=0x8c6461deu;if(!valid_access(r[4]+11u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0xbu,1);if(io_status)return io_status;r[0]=loaded;}goto L8c6461e0; /* mov.b @(0xb,r4),r0 */
L8c6461e0: *stop=0x8c6461e0u;if(!valid_access(r[5]+7u,1u))return -3;wr(m,bus,&io_status,r[5]+0x7u,r[0],1);if(io_status)return io_status;goto L8c6461e2; /* mov.b r0,@(0x7,r5) */
L8c6461e2: *stop=0x8c6461e2u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6461e4; /* mov.l @(0x14,r4),r3 */
L8c6461e4: *stop=0x8c6461e4u;if(!valid_access(r[5]+12u,4u))return -3;wr(m,bus,&io_status,r[5]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6461e6; /* mov.l r3,@(0xc,r5) */
L8c6461e6: *stop=0x8c6461e6u;r[5]+=24u;goto L8c6461e8; /* add #0x18,r5 */
L8c6461e8: take=!(*sr&1u);*stop=0x8c6461eau;r[4]+=44u;if(take)goto L8c6461ba;goto L8c6461ec; /* bf/s 0x8c6461ba */
L8c6461ec: *stop=0x8c6461ecu;if(*pr!=entry_pr)return -3;*stop=0x8c6461eeu;return 0; /* rts */
}
