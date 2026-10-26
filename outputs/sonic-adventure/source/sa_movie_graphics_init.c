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
 if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_graphics_init_8c656940(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c656940;
L8c656940: *stop=0x8c656940u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c656942; /* mov.l r14,@-r15 */
L8c656942: *stop=0x8c656942u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c656944; /* mov.l r13,@-r15 */
L8c656944: *stop=0x8c656944u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c656946; /* mov.l r12,@-r15 */
L8c656946: *stop=0x8c656946u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c656948; /* mov.l r11,@-r15 */
L8c656948: *stop=0x8c656948u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65694a; /* mov.l r10,@-r15 */
L8c65694a: *stop=0x8c65694au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65694c; /* mov.l r9,@-r15 */
L8c65694c: *stop=0x8c65694cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65694e; /* mov.l r8,@-r15 */
L8c65694e: *stop=0x8c65694eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c656950; /* fmov.s fr15,@-r15 */
L8c656950: *stop=0x8c656950u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c656952; /* sts.l PR,@-r15 */
L8c656952: *stop=0x8c656952u;r[15]+=4294967192u;goto L8c656954; /* add #-0x68,r15 */
L8c656954: *stop=0x8c656954u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656956; /* mov.l 0x8c6569d0,r3 */
L8c656956: *stop=0x8c656956u;r[11]=2u;goto L8c656958; /* mov #0x2,r11 */
L8c656958: *stop=0x8c656958u;r[14]=r[4];goto L8c65695a; /* mov r4,r14 */
L8c65695a: *stop=0x8c65695au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c65695c; /* fldi0 fr15 */
L8c65695c: target=r[3];*pr=0x8c656960u;*stop=0x8c65695eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[11],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656960; /* jsr @r3 */
L8c656960: *stop=0x8c656960u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656962; /* mov.l 0x8c6569d4,r2 */
L8c656962: target=r[2];*pr=0x8c656966u;*stop=0x8c656964u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656966; /* jsr @r2 */
L8c656966: *stop=0x8c656966u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569d8u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c656968; /* mov.l 0x8c6569d8,r13 */
L8c656968: *stop=0x8c656968u;r[9]=1u;goto L8c65696a; /* mov #0x1,r9 */
L8c65696a: *stop=0x8c65696au;r[12]=0u;goto L8c65696c; /* mov #0x0,r12 */
L8c65696c: *stop=0x8c65696cu;r[0]=84u;goto L8c65696e; /* mov #0x54,r0 */
L8c65696e: *stop=0x8c65696eu;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[12],4);if(io_status)return io_status;goto L8c656970; /* mov.l r12,@(r0,r13) */
L8c656970: *stop=0x8c656970u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656972; /* mov.l @(0x4,r14),r3 */
L8c656972: *stop=0x8c656972u;*sr=(*sr&~1u)|!(r[9]&r[3]);goto L8c656974; /* tst r9,r3 */
L8c656974: if((*sr&1u))goto L8c65697a;goto L8c656976; /* bt 0x8c65697a */
L8c656976: *stop=0x8c656978u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[9],4);if(io_status)return io_status;goto L8c65697c; /* bra 0x8c65697c */
L8c65697a: *stop=0x8c65697au;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[12],4);if(io_status)return io_status;goto L8c65697c; /* mov.l r12,@(0x10,r15) */
L8c65697c: *stop=0x8c65697cu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65697e; /* mov.l @(0x4,r14),r0 */
L8c65697e: *stop=0x8c65697eu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c656980; /* tst #0x20,r0 */
L8c656980: if((*sr&1u))goto L8c656986;goto L8c656982; /* bt 0x8c656986 */
L8c656982: *stop=0x8c656984u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[12],4);if(io_status)return io_status;goto L8c656988; /* bra 0x8c656988 */
L8c656986: *stop=0x8c656986u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[9],4);if(io_status)return io_status;goto L8c656988; /* mov.l r9,@(0x20,r15) */
L8c656988: *stop=0x8c656988u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65698a; /* mov.l @(0x4,r14),r3 */
L8c65698a: *stop=0x8c65698au;*sr=(*sr&~1u)|!(r[11]&r[3]);goto L8c65698c; /* tst r11,r3 */
L8c65698c: if((*sr&1u))goto L8c6569e0;goto L8c65698e; /* bt 0x8c6569e0 */
L8c65698e: *stop=0x8c656990u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[9],4);if(io_status)return io_status;goto L8c6569e2; /* bra 0x8c6569e2 */
L8c6569e0: *stop=0x8c6569e0u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[12],4);if(io_status)return io_status;goto L8c6569e2; /* mov.l r12,@(0xc,r15) */
L8c6569e2: *stop=0x8c6569e2u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6569e4; /* mov.l @(0x4,r14),r2 */
L8c6569e4: *stop=0x8c6569e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6569e6; /* mov.l 0x8c656a84,r3 */
L8c6569e6: *stop=0x8c6569e6u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c6569e8; /* tst r3,r2 */
L8c6569e8: if((*sr&1u))goto L8c6569f0;goto L8c6569ea; /* bt 0x8c6569f0 */
L8c6569ea: *stop=0x8c6569eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a88u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6569ec; /* mov.l 0x8c656a88,r0 */
L8c6569ec: *stop=0x8c6569eeu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[12],4);if(io_status)return io_status;goto L8c6569f4; /* bra 0x8c6569f4 */
L8c6569f0: *stop=0x8c6569f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a88u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6569f2; /* mov.l 0x8c656a88,r1 */
L8c6569f2: *stop=0x8c6569f2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[9],4);if(io_status)return io_status;goto L8c6569f4; /* mov.l r9,@r1 */
L8c6569f4: *stop=0x8c6569f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6569f6; /* mov.l 0x8c656a8c,r3 */
L8c6569f6: *stop=0x8c6569f6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c656a80u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6569f8; /* mov.w 0x8c656a80,r4 */
L8c6569f8: *stop=0x8c6569f8u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6569fa; /* mov.l @(0x14,r14),r6 */
L8c6569fa: target=r[3];*pr=0x8c6569feu;*stop=0x8c6569fcu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6569fe; /* jsr @r3 */
L8c6569fe: *stop=0x8c6569feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a98u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656a00; /* mov.l 0x8c656a98,r3 */
L8c656a00: *stop=0x8c656a00u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656a02; /* mov.l @(0x4,r14),r2 */
L8c656a02: *stop=0x8c656a02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a90u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656a04; /* mov.l 0x8c656a90,r4 */
L8c656a04: *stop=0x8c656a04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a94u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c656a06; /* mov.l 0x8c656a94,r8 */
L8c656a06: *stop=0x8c656a06u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c656a08; /* tst r3,r2 */
L8c656a08: if((*sr&1u))goto L8c656a0e;goto L8c656a0a; /* bt 0x8c656a0e */
L8c656a0a: *stop=0x8c656a0cu;goto L8c656d80; /* bra 0x8c656d80 */
L8c656a0e: *stop=0x8c656a0eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c656a10; /* mov.l r12,@r4 */
L8c656a10: *stop=0x8c656a10u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656a12; /* mov.l @(0x4,r14),r0 */
L8c656a12: *stop=0x8c656a12u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c656a14; /* tst #0x10,r0 */
L8c656a14: take=(*sr&1u);*stop=0x8c656a16u;r[0]=92u;if(take)goto L8c656a22;goto L8c656a18; /* bt/s 0x8c656a22 */
L8c656a18: *stop=0x8c656a18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a9cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656a1a; /* mov.l 0x8c656a9c,r1 */
L8c656a1a: *stop=0x8c656a1au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[9],4);if(io_status)return io_status;goto L8c656a1c; /* mov.l r9,@r1 */
L8c656a1c: *stop=0x8c656a1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a88u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656a1e; /* mov.l 0x8c656a88,r2 */
L8c656a1e: *stop=0x8c656a20u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[9],4);if(io_status)return io_status;goto L8c656a26; /* bra 0x8c656a26 */
L8c656a22: *stop=0x8c656a22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a9cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656a24; /* mov.l 0x8c656a9c,r1 */
L8c656a24: *stop=0x8c656a24u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[12],4);if(io_status)return io_status;goto L8c656a26; /* mov.l r12,@r1 */
L8c656a26: *stop=0x8c656a26u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a9cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656a28; /* mov.l 0x8c656a9c,r3 */
L8c656a28: *stop=0x8c656a28u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c656a2a; /* mov.l @(r0,r14),r5 */
L8c656a2a: target=0x8c656860u;*pr=0x8c656a2eu;*stop=0x8c656a2cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656a2e; /* bsr 0x8c656860 */
L8c656a2e: *stop=0x8c656a2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656aa0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656a30; /* mov.l 0x8c656aa0,r2 */
L8c656a30: *stop=0x8c656a30u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656a32; /* mov.l @(0x24,r14),r3 */
L8c656a32: *stop=0x8c656a32u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656a34; /* mov.l @(0x28,r14),r5 */
L8c656a34: *stop=0x8c656a34u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c656a36; /* mov.l r3,@r2 */
L8c656a36: *stop=0x8c656a36u;r[1]=r[2];goto L8c656a38; /* mov r2,r1 */
L8c656a38: *stop=0x8c656a38u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656a3a; /* mov.l @r1,r3 */
L8c656a3a: *stop=0x8c656a3au;r[4]=31u;goto L8c656a3c; /* mov #0x1f,r4 */
L8c656a3c: *stop=0x8c656a3cu;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c656a3e; /* tst r4,r3 */
L8c656a3e: if((*sr&1u))goto L8c656a58;goto L8c656a40; /* bt 0x8c656a58 */
L8c656a40: *stop=0x8c656a40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656aa0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656a42; /* mov.l 0x8c656aa0,r0 */
L8c656a42: *stop=0x8c656a42u;r[1]=4294967264u;goto L8c656a44; /* mov #-0x20,r1 */
L8c656a44: *stop=0x8c656a44u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656a46; /* mov.l @r0,r3 */
L8c656a46: *stop=0x8c656a46u;r[2]=r[0];goto L8c656a48; /* mov r0,r2 */
L8c656a48: *stop=0x8c656a48u;r[3]&=r[4];goto L8c656a4a; /* and r4,r3 */
L8c656a4a: *stop=0x8c656a4au;r[3]=0u-r[3];goto L8c656a4c; /* neg r3,r3 */
L8c656a4c: *stop=0x8c656a4cu;r[3]+=32u;goto L8c656a4e; /* add #0x20,r3 */
L8c656a4e: *stop=0x8c656a4eu;r[5]-=r[3];goto L8c656a50; /* sub r3,r5 */
L8c656a50: *stop=0x8c656a50u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656a52; /* mov.l @r2,r3 */
L8c656a52: *stop=0x8c656a52u;r[3]+=31u;goto L8c656a54; /* add #0x1f,r3 */
L8c656a54: *stop=0x8c656a54u;r[3]&=r[1];goto L8c656a56; /* and r1,r3 */
L8c656a56: *stop=0x8c656a56u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c656a58; /* mov.l r3,@r0 */
L8c656a58: *stop=0x8c656a58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656a9cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656a5a; /* mov.l 0x8c656a9c,r1 */
L8c656a5a: *stop=0x8c656a5au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656a5c; /* mov.l @r1,r2 */
L8c656a5c: *stop=0x8c656a5cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c656a5e; /* tst r2,r2 */
L8c656a5e: if(!(*sr&1u))goto L8c656a62;goto L8c656a60; /* bf 0x8c656a62 */
L8c656a60: *stop=0x8c656a60u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c656a62; /* shlr r5 */
L8c656a62: *stop=0x8c656a62u;r[11]=r[15];goto L8c656a64; /* mov r15,r11 */
L8c656a64: *stop=0x8c656a64u;r[11]+=64u;goto L8c656a66; /* add #0x40,r11 */
L8c656a66: *stop=0x8c656a66u;r[6]=r[11];goto L8c656a68; /* mov r11,r6 */
L8c656a68: *stop=0x8c656a68u;r[4]=r[11];goto L8c656a6a; /* mov r11,r4 */
L8c656a6a: *stop=0x8c656a6cu;r[6]+=20u;goto L8c656ac4; /* bra 0x8c656ac4 */
L8c656ac0: *stop=0x8c656ac0u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c656ac2; /* mov.l r12,@r4 */
L8c656ac2: *stop=0x8c656ac2u;r[4]+=4u;goto L8c656ac4; /* add #0x4,r4 */
L8c656ac4: *stop=0x8c656ac4u;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c656ac6; /* cmp/hs r6,r4 */
L8c656ac6: if(!(*sr&1u))goto L8c656ac0;goto L8c656ac8; /* bf 0x8c656ac0 */
L8c656ac8: *stop=0x8c656ac8u;r[4]=r[14];goto L8c656aca; /* mov r14,r4 */
L8c656aca: *stop=0x8c656accu;r[4]+=44u;goto L8c656ad2; /* bra 0x8c656ad2 */
L8c656ace: *stop=0x8c656aceu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c656ad0; /* fmov.s @r4+,fr3 */
L8c656ad0: *stop=0x8c656ad0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c656ad2; /* fadd fr3,fr15 */
L8c656ad2: *stop=0x8c656ad2u;r[3]=r[14];goto L8c656ad4; /* mov r14,r3 */
L8c656ad4: *stop=0x8c656ad4u;r[3]+=64u;goto L8c656ad6; /* add #0x40,r3 */
L8c656ad6: *stop=0x8c656ad6u;*sr=(*sr&~1u)|(r[4]>=r[3]);goto L8c656ad8; /* cmp/hs r3,r4 */
L8c656ad8: if(!(*sr&1u))goto L8c656ace;goto L8c656ada; /* bf 0x8c656ace */
L8c656ada: *stop=0x8c656adau;r[0]=0x8c656be0u;goto L8c656adc; /* mova 0x8c656be0,r0 */
L8c656adc: *stop=0x8c656adcu;r[6]=r[15];goto L8c656ade; /* mov r15,r6 */
L8c656ade: *stop=0x8c656adeu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c656ae0; /* fmov.s @r0,fr4 */
L8c656ae0: *stop=0x8c656ae0u;r[6]+=84u;goto L8c656ae2; /* add #0x54,r6 */
L8c656ae2: *stop=0x8c656ae2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c656ae4; /* fcmp/gt fr4,fr15 */
L8c656ae4: take=!(*sr&1u);*stop=0x8c656ae6u;r[10]=20u;if(take)goto L8c656b06;goto L8c656ae8; /* bf/s 0x8c656b06 */
L8c656ae8: *stop=0x8c656aeau;r[4]=r[12];goto L8c656afe; /* bra 0x8c656afe */
L8c656aec: *stop=0x8c656aecu;r[3]=r[14];goto L8c656aee; /* mov r14,r3 */
L8c656aee: *stop=0x8c656aeeu;r[3]+=44u;goto L8c656af0; /* add #0x2c,r3 */
L8c656af0: *stop=0x8c656af0u;r[3]+=r[4];goto L8c656af2; /* add r4,r3 */
L8c656af2: *stop=0x8c656af2u;r[0]=r[4];goto L8c656af4; /* mov r4,r0 */
L8c656af4: *stop=0x8c656af4u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c656af6; /* fmov.s @r3,fr3 */
L8c656af6: *stop=0x8c656af6u;r[4]+=4u;goto L8c656af8; /* add #0x4,r4 */
L8c656af8: *stop=0x8c656af8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c656afa; /* fmul fr4,fr3 */
L8c656afa: *stop=0x8c656afau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c656afc; /* fdiv fr15,fr3 */
L8c656afc: *stop=0x8c656afcu;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c656afe; /* fmov fr3,@(r0,r6) */
L8c656afe: *stop=0x8c656afeu;*sr=(*sr&~1u)|(r[4]>=r[10]);goto L8c656b00; /* cmp/hs r10,r4 */
L8c656b00: if(!(*sr&1u))goto L8c656aec;goto L8c656b02; /* bf 0x8c656aec */
L8c656b02: *stop=0x8c656b04u;goto L8c656b1c; /* bra 0x8c656b1c */
L8c656b06: *stop=0x8c656b08u;r[4]=r[12];goto L8c656b18; /* bra 0x8c656b18 */
L8c656b0a: *stop=0x8c656b0au;r[3]=r[14];goto L8c656b0c; /* mov r14,r3 */
L8c656b0c: *stop=0x8c656b0cu;r[3]+=44u;goto L8c656b0e; /* add #0x2c,r3 */
L8c656b0e: *stop=0x8c656b0eu;r[3]+=r[4];goto L8c656b10; /* add r4,r3 */
L8c656b10: *stop=0x8c656b10u;r[0]=r[4];goto L8c656b12; /* mov r4,r0 */
L8c656b12: *stop=0x8c656b12u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c656b14; /* fmov.s @r3,fr3 */
L8c656b14: *stop=0x8c656b14u;r[4]+=4u;goto L8c656b16; /* add #0x4,r4 */
L8c656b16: *stop=0x8c656b16u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c656b18; /* fmov fr3,@(r0,r6) */
L8c656b18: *stop=0x8c656b18u;*sr=(*sr&~1u)|(r[4]>=r[10]);goto L8c656b1a; /* cmp/hs r10,r4 */
L8c656b1a: if(!(*sr&1u))goto L8c656b0a;goto L8c656b1c; /* bf 0x8c656b0a */
L8c656b1c: *stop=0x8c656b1eu;r[4]=r[12];goto L8c656b56; /* bra 0x8c656b56 */
L8c656b20: *stop=0x8c656b20u;r[0]=r[4];goto L8c656b22; /* mov r4,r0 */
L8c656b22: *stop=0x8c656b22u;*fpul=r[5];goto L8c656b24; /* lds r5,FPUL */
L8c656b24: *stop=0x8c656b24u;r[3]=r[4];goto L8c656b26; /* mov r4,r3 */
L8c656b26: *stop=0x8c656b26u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c656b28; /* fmov.s @(r0,r6),fr3 */
L8c656b28: *stop=0x8c656b28u;*sr=(*sr&~1u)|((int32_t)r[5]>=0);goto L8c656b2a; /* cmp/pz r5 */
L8c656b2a: *stop=0x8c656b2au;r[3]+=r[11];goto L8c656b2c; /* add r11,r3 */
L8c656b2c: take=(*sr&1u);*stop=0x8c656b2eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;if(take)goto L8c656b38;goto L8c656b30; /* bt/s 0x8c656b38 */
L8c656b30: *stop=0x8c656b30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656be4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656b32; /* mov.l 0x8c656be4,r2 */
L8c656b32: *stop=0x8c656b32u;*fpul=r[2];goto L8c656b34; /* lds r2,FPUL */
L8c656b34: *stop=0x8c656b34u;f->bank[(f->fpscr>>21)&1u][1]=*fpul;goto L8c656b36; /* fsts FPUL,fr1 */
L8c656b36: *stop=0x8c656b36u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c656b38; /* fadd fr1,fr2 */
L8c656b38: *stop=0x8c656b38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c656b3a; /* fmul fr2,fr3 */
L8c656b3a: *stop=0x8c656b3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c656be8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656b3c; /* mov.l 0x8c656be8,r1 */
L8c656b3c: *stop=0x8c656b3cu;*fpul=r[1];goto L8c656b3e; /* lds r1,FPUL */
L8c656b3e: *stop=0x8c656b3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c656b40; /* fdiv fr4,fr3 */
L8c656b40: *stop=0x8c656b40u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c656b42; /* fsts FPUL,fr2 */
L8c656b42: *stop=0x8c656b42u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c656b44; /* fcmp/gt fr3,fr2 */
L8c656b44: if((*sr&1u))goto L8c656b4a;goto L8c656b46; /* bt 0x8c656b4a */
L8c656b46: *stop=0x8c656b46u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[2],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c656b48; /* fadd fr2,fr2 */
L8c656b48: *stop=0x8c656b48u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c656b4a; /* fsub fr2,fr3 */
L8c656b4a: *stop=0x8c656b4au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c656b4c; /* ftrc fr3,FPUL */
L8c656b4c: *stop=0x8c656b4cu;r[1]=4294967288u;goto L8c656b4e; /* mov #-0x8,r1 */
L8c656b4e: *stop=0x8c656b4eu;r[4]+=4u;goto L8c656b50; /* add #0x4,r4 */
L8c656b50: *stop=0x8c656b50u;r[2]=*fpul;goto L8c656b52; /* sts FPUL,r2 */
L8c656b52: *stop=0x8c656b52u;r[2]&=r[1];goto L8c656b54; /* and r1,r2 */
L8c656b54: *stop=0x8c656b54u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c656b56; /* mov.l r2,@r3 */
L8c656b56: *stop=0x8c656b56u;*sr=(*sr&~1u)|(r[4]>=r[10]);goto L8c656b58; /* cmp/hs r10,r4 */
L8c656b58: if(!(*sr&1u))goto L8c656b20;goto L8c656b5a; /* bf 0x8c656b20 */
L8c656b5a: *stop=0x8c656b5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c656becu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656b5c; /* mov.l 0x8c656bec,r2 */
L8c656b5c: *stop=0x8c656b5cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c656b5e; /* mov.l @r2,r0 */
L8c656b5e: *stop=0x8c656b5eu;*sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c656b60; /* tst #0x40,r0 */
L8c656b60: take=(*sr&1u);*stop=0x8c656b62u;r[4]=r[11];if(take)goto L8c656b72;goto L8c656b64; /* bt/s 0x8c656b72 */
L8c656b64: *stop=0x8c656b64u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656b66; /* mov.l @r4,r3 */
L8c656b66: *stop=0x8c656b66u;r[1]=56u;goto L8c656b68; /* mov #0x38,r1 */
L8c656b68: *stop=0x8c656b68u;*sr=(*sr&~1u)|(r[3]>=r[1]);goto L8c656b6a; /* cmp/hs r1,r3 */
L8c656b6a: if((*sr&1u))goto L8c656b72;goto L8c656b6c; /* bt 0x8c656b72 */
L8c656b6c: *stop=0x8c656b6cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656b6e; /* mov.l @r4,r3 */
L8c656b6e: *stop=0x8c656b6eu;r[3]+=r[1];goto L8c656b70; /* add r1,r3 */
L8c656b70: *stop=0x8c656b70u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c656b72; /* mov.l r3,@r4 */
L8c656b72: *stop=0x8c656b72u;r[7]=4294967295u;goto L8c656b74; /* mov #-0x1,r7 */
L8c656b74: *stop=0x8c656b74u;r[6]=r[12];goto L8c656b76; /* mov r12,r6 */
L8c656b76: *stop=0x8c656b78u;r[4]=r[12];goto L8c656b94; /* bra 0x8c656b94 */
L8c656b7a: *stop=0x8c656b7au;r[3]=r[4];goto L8c656b7c; /* mov r4,r3 */
L8c656b7c: *stop=0x8c656b7cu;r[3]<<=2;goto L8c656b7e; /* shll2 r3 */
L8c656b7e: *stop=0x8c656b7eu;r[3]+=r[11];goto L8c656b80; /* add r11,r3 */
L8c656b80: *stop=0x8c656b80u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c656b82; /* mov.l r3,@r15 */
L8c656b82: *stop=0x8c656b82u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656b84; /* mov.l @r3,r2 */
L8c656b84: *stop=0x8c656b84u;r[1]=r[2];goto L8c656b86; /* mov r2,r1 */
L8c656b86: *stop=0x8c656b86u;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[7]);goto L8c656b88; /* cmp/gt r7,r1 */
L8c656b88: *stop=0x8c656b88u;r[6]+=r[2];goto L8c656b8a; /* add r2,r6 */
L8c656b8a: if(!(*sr&1u))goto L8c656b92;goto L8c656b8c; /* bf 0x8c656b92 */
L8c656b8c: *stop=0x8c656b8cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c656b8e; /* mov.l r4,@(0x8,r15) */
L8c656b8e: *stop=0x8c656b8eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[7]=loaded;}goto L8c656b90; /* mov.l @r15,r7 */
L8c656b90: *stop=0x8c656b90u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[7]=loaded;}goto L8c656b92; /* mov.l @r7,r7 */
L8c656b92: *stop=0x8c656b92u;r[4]+=1u;goto L8c656b94; /* add #0x1,r4 */
L8c656b94: *stop=0x8c656b94u;r[3]=5u;goto L8c656b96; /* mov #0x5,r3 */
L8c656b96: *stop=0x8c656b96u;*sr=(*sr&~1u)|(r[4]>=r[3]);goto L8c656b98; /* cmp/hs r3,r4 */
L8c656b98: if(!(*sr&1u))goto L8c656b7a;goto L8c656b9a; /* bf 0x8c656b7a */
L8c656b9a: *stop=0x8c656b9au;*sr=(*sr&~1u)|(r[6]>r[5]);goto L8c656b9c; /* cmp/hi r5,r6 */
L8c656b9c: if(!(*sr&1u))goto L8c656bb8;goto L8c656b9e; /* bf 0x8c656bb8 */
L8c656b9e: *stop=0x8c656b9eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656ba0; /* mov.l @(0x8,r15),r4 */
L8c656ba0: *stop=0x8c656ba0u;r[2]=r[6];goto L8c656ba2; /* mov r6,r2 */
L8c656ba2: *stop=0x8c656ba2u;r[2]-=r[5];goto L8c656ba4; /* sub r5,r2 */
L8c656ba4: *stop=0x8c656ba4u;r[4]<<=2;goto L8c656ba6; /* shll2 r4 */
L8c656ba6: *stop=0x8c656ba6u;r[5]=r[2];goto L8c656ba8; /* mov r2,r5 */
L8c656ba8: *stop=0x8c656ba8u;r[4]+=r[11];goto L8c656baa; /* add r11,r4 */
L8c656baa: *stop=0x8c656baau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656bac; /* mov.l @r4,r3 */
L8c656bac: *stop=0x8c656bacu;r[3]-=r[5];goto L8c656bae; /* sub r5,r3 */
L8c656bae: *stop=0x8c656baeu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c656bb0; /* mov.l r3,@r4 */
L8c656bb0: *stop=0x8c656bb0u;r[3]=4294967264u;goto L8c656bb2; /* mov #-0x20,r3 */
L8c656bb2: *stop=0x8c656bb2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656bb4; /* mov.l @r4,r2 */
L8c656bb4: *stop=0x8c656bb4u;r[2]&=r[3];goto L8c656bb6; /* and r3,r2 */
L8c656bb6: *stop=0x8c656bb6u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c656bb8; /* mov.l r2,@r4 */
L8c656bb8: *stop=0x8c656bb8u;r[4]=r[11];goto L8c656bba; /* mov r11,r4 */
L8c656bba: *stop=0x8c656bbau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656bbc; /* mov.l @r4,r3 */
L8c656bbc: *stop=0x8c656bbcu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c656bbe; /* tst r3,r3 */
L8c656bbe: if((*sr&1u))goto L8c656bc6;goto L8c656bc0; /* bt 0x8c656bc6 */
L8c656bc0: *stop=0x8c656bc0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656bc2; /* mov.l @r4,r3 */
L8c656bc2: *stop=0x8c656bc2u;r[3]+=4294967280u;goto L8c656bc4; /* add #-0x10,r3 */
L8c656bc4: *stop=0x8c656bc4u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c656bc6; /* mov.l r3,@r4 */
L8c656bc6: *stop=0x8c656bc6u;r[4]=r[11];goto L8c656bc8; /* mov r11,r4 */
L8c656bc8: *stop=0x8c656bc8u;r[4]+=4u;goto L8c656bca; /* add #0x4,r4 */
L8c656bca: *stop=0x8c656bcau;r[6]=r[11];goto L8c656bcc; /* mov r11,r6 */
L8c656bcc: *stop=0x8c656bccu;r[5]=r[4];goto L8c656bce; /* mov r4,r5 */
L8c656bce: *stop=0x8c656bd0u;r[6]+=20u;goto L8c656c10; /* bra 0x8c656c10 */
L8c656c00: *stop=0x8c656c00u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c02; /* mov.l @r4,r3 */
L8c656c02: *stop=0x8c656c02u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c656c04; /* tst r3,r3 */
L8c656c04: if((*sr&1u))goto L8c656c0c;goto L8c656c06; /* bt 0x8c656c0c */
L8c656c06: *stop=0x8c656c06u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c08; /* mov.l @r5,r3 */
L8c656c08: *stop=0x8c656c08u;r[3]+=4294967280u;goto L8c656c0a; /* add #-0x10,r3 */
L8c656c0a: *stop=0x8c656c0au;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c656c0c; /* mov.l r3,@r5 */
L8c656c0c: *stop=0x8c656c0cu;r[4]+=4u;goto L8c656c0e; /* add #0x4,r4 */
L8c656c0e: *stop=0x8c656c0eu;r[5]+=4u;goto L8c656c10; /* add #0x4,r5 */
L8c656c10: *stop=0x8c656c10u;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c656c12; /* cmp/hs r6,r4 */
L8c656c12: if(!(*sr&1u))goto L8c656c00;goto L8c656c14; /* bf 0x8c656c00 */
L8c656c14: *stop=0x8c656c14u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c16; /* mov.l @(0x20,r14),r3 */
L8c656c16: *stop=0x8c656c16u;r[2]=r[11];goto L8c656c18; /* mov r11,r2 */
L8c656c18: *stop=0x8c656c18u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c656c1a; /* mov.l r3,@r13 */
L8c656c1a: *stop=0x8c656c1au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c656c1c; /* mov.l r2,@r15 */
L8c656c1c: *stop=0x8c656c1cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}r[2]+=4;goto L8c656c1e; /* mov.l @r2+,r3 */
L8c656c1e: *stop=0x8c656c1eu;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[3],4);if(io_status)return io_status;goto L8c656c20; /* mov.l r3,@(0x4,r13) */
L8c656c20: *stop=0x8c656c20u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c656c22; /* mov.l r2,@(0x8,r15) */
L8c656c22: *stop=0x8c656c22u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c24; /* mov.l @r2,r3 */
L8c656c24: *stop=0x8c656c24u;r[2]=r[11];goto L8c656c26; /* mov r11,r2 */
L8c656c26: *stop=0x8c656c26u;r[2]+=8u;goto L8c656c28; /* add #0x8,r2 */
L8c656c28: *stop=0x8c656c28u;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[3],4);if(io_status)return io_status;goto L8c656c2a; /* mov.l r3,@(0x8,r13) */
L8c656c2a: *stop=0x8c656c2au;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c656c2c; /* mov.l r2,@(0x1c,r15) */
L8c656c2c: *stop=0x8c656c2cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c2e; /* mov.l @r2,r3 */
L8c656c2e: *stop=0x8c656c2eu;r[2]=r[11];goto L8c656c30; /* mov r11,r2 */
L8c656c30: *stop=0x8c656c30u;r[2]+=12u;goto L8c656c32; /* add #0xc,r2 */
L8c656c32: *stop=0x8c656c32u;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[3],4);if(io_status)return io_status;goto L8c656c34; /* mov.l r3,@(0xc,r13) */
L8c656c34: *stop=0x8c656c34u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[2],4);if(io_status)return io_status;goto L8c656c36; /* mov.l r2,@(0x18,r15) */
L8c656c36: *stop=0x8c656c36u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c38; /* mov.l @r2,r3 */
L8c656c38: *stop=0x8c656c38u;r[2]=r[11];goto L8c656c3a; /* mov r11,r2 */
L8c656c3a: *stop=0x8c656c3au;r[2]+=16u;goto L8c656c3c; /* add #0x10,r2 */
L8c656c3c: *stop=0x8c656c3cu;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[3],4);if(io_status)return io_status;goto L8c656c3e; /* mov.l r3,@(0x10,r13) */
L8c656c3e: *stop=0x8c656c3eu;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c656c40; /* mov.l r2,@(0x14,r15) */
L8c656c40: *stop=0x8c656c40u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c42; /* mov.l @r2,r3 */
L8c656c42: *stop=0x8c656c42u;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[3],4);if(io_status)return io_status;goto L8c656c44; /* mov.l r3,@(0x14,r13) */
L8c656c44: *stop=0x8c656c44u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656c46; /* mov.l @(0x14,r15),r2 */
L8c656c46: *stop=0x8c656c46u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c48; /* mov.l @r2,r3 */
L8c656c48: *stop=0x8c656c48u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c656c4a; /* mov.l r3,@-r15 */
L8c656c4a: *stop=0x8c656c4au;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656c4c; /* mov.l @(0x20,r15),r6 */
L8c656c4c: *stop=0x8c656c4cu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656c4e; /* mov.l @(0x1c,r15),r7 */
L8c656c4e: *stop=0x8c656c4eu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656c50; /* mov.l @(0xc,r15),r5 */
L8c656c50: *stop=0x8c656c50u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[6]=loaded;}goto L8c656c52; /* mov.l @r6,r6 */
L8c656c52: *stop=0x8c656c52u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[7]=loaded;}goto L8c656c54; /* mov.l @r7,r7 */
L8c656c54: *stop=0x8c656c54u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656c56; /* mov.l @(0x4,r15),r4 */
L8c656c56: *stop=0x8c656c56u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c656c58; /* mov.l @r5,r5 */
L8c656c58: target=0x8c656f50u;*pr=0x8c656c5cu;*stop=0x8c656c5au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656c5c; /* bsr 0x8c656f50 */
L8c656c5c: *stop=0x8c656c5cu;r[15]+=4u;goto L8c656c5e; /* add #0x4,r15 */
L8c656c5e: *stop=0x8c656c5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656d40u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656c60; /* mov.l 0x8c656d40,r1 */
L8c656c60: *stop=0x8c656c60u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656c62; /* mov.l @(0x20,r14),r2 */
L8c656c62: *stop=0x8c656c62u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c64; /* mov.l @r1,r3 */
L8c656c64: *stop=0x8c656c64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656d44u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656c66; /* mov.l 0x8c656d44,r4 */
L8c656c66: *stop=0x8c656c66u;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[3],4);if(io_status)return io_status;goto L8c656c68; /* mov.l r3,@(0x10,r2) */
L8c656c68: *stop=0x8c656c68u;r[5]=r[4];goto L8c656c6a; /* mov r4,r5 */
L8c656c6a: *stop=0x8c656c6cu;r[5]+=20u;goto L8c656c72; /* bra 0x8c656c72 */
L8c656c6e: *stop=0x8c656c6eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c656c70; /* mov.l r12,@r4 */
L8c656c70: *stop=0x8c656c70u;r[4]+=4u;goto L8c656c72; /* add #0x4,r4 */
L8c656c72: *stop=0x8c656c72u;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c656c74; /* cmp/hs r5,r4 */
L8c656c74: if(!(*sr&1u))goto L8c656c6e;goto L8c656c76; /* bf 0x8c656c6e */
L8c656c76: *stop=0x8c656c76u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c656c78; /* mov.l r12,@-r15 */
L8c656c78: *stop=0x8c656c78u;if(!valid_access(r[11]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c7a; /* mov.l @(0x10,r11),r3 */
L8c656c7a: *stop=0x8c656c7au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c656c7c; /* mov.l r3,@-r15 */
L8c656c7c: *stop=0x8c656c7cu;if(!valid_access(r[11]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656c7e; /* mov.l @(0xc,r11),r7 */
L8c656c7e: *stop=0x8c656c7eu;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656c80; /* mov.l @(0x8,r11),r6 */
L8c656c80: *stop=0x8c656c80u;if(!valid_access(r[11]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656c82; /* mov.l @(0x4,r11),r5 */
L8c656c82: target=0x8c657096u;*pr=0x8c656c86u;*stop=0x8c656c84u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656c86; /* bsr 0x8c657096 */
L8c656c86: *stop=0x8c656c86u;r[15]+=8u;goto L8c656c88; /* add #0x8,r15 */
L8c656c88: *stop=0x8c656c88u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c656c8a; /* mov.l r0,@r15 */
L8c656c8a: target=0x8c6573c4u;*pr=0x8c656c8eu;*stop=0x8c656c8cu;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656c8e; /* bsr 0x8c6573c4 */
L8c656c8e: *stop=0x8c656c8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656d48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656c90; /* mov.l 0x8c656d48,r2 */
L8c656c90: *stop=0x8c656c90u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c656c92; /* mov.l @r2,r0 */
L8c656c92: *stop=0x8c656c92u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c656c94; /* cmp/eq #0x1,r0 */
L8c656c94: if(!(*sr&1u))goto L8c656c98;goto L8c656c96; /* bf 0x8c656c98 */
L8c656c96: *stop=0x8c656c96u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c656c98; /* mov.l r9,@(0x4,r15) */
L8c656c98: *stop=0x8c656c98u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656c9a; /* mov.l @(0x4,r15),r3 */
L8c656c9a: *stop=0x8c656c9au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c656c9c; /* mov.l r3,@-r15 */
L8c656c9c: *stop=0x8c656c9cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c656c9e; /* mov.l r12,@-r15 */
L8c656c9e: *stop=0x8c656c9eu;if(!valid_access(r[11]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656ca0; /* mov.l @(0x10,r11),r3 */
L8c656ca0: *stop=0x8c656ca0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c656ca2; /* mov.l r3,@-r15 */
L8c656ca2: *stop=0x8c656ca2u;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656ca4; /* mov.l @(0x8,r11),r6 */
L8c656ca4: *stop=0x8c656ca4u;if(!valid_access(r[11]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656ca6; /* mov.l @(0x4,r11),r5 */
L8c656ca6: *stop=0x8c656ca6u;if(!valid_access(r[11]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656ca8; /* mov.l @(0xc,r11),r7 */
L8c656ca8: target=0x8c6574b8u;*pr=0x8c656cacu;*stop=0x8c656caau;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656cac; /* bsr 0x8c6574b8 */
L8c656cac: *stop=0x8c656cacu;r[15]+=12u;goto L8c656cae; /* add #0xc,r15 */
L8c656cae: *stop=0x8c656caeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656d4cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656cb0; /* mov.l 0x8c656d4c,r5 */
L8c656cb0: *stop=0x8c656cb0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656d50u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656cb2; /* mov.l 0x8c656d50,r6 */
L8c656cb2: *stop=0x8c656cb2u;r[4]=r[5];goto L8c656cb4; /* mov r5,r4 */
L8c656cb4: *stop=0x8c656cb4u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656cb6; /* mov.l @r4,r3 */
L8c656cb6: *stop=0x8c656cb6u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c656cb8; /* mov.l r3,@r8 */
L8c656cb8: *stop=0x8c656cb8u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656cba; /* mov.l @(0x4,r4),r2 */
L8c656cba: *stop=0x8c656cbau;if(!valid_access(r[8]+4u,4u))return -3;wr(m,bus,&io_status,r[8]+0x4u,r[2],4);if(io_status)return io_status;goto L8c656cbc; /* mov.l r2,@(0x4,r8) */
L8c656cbc: *stop=0x8c656cbcu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656cbe; /* mov.l @(0x8,r4),r3 */
L8c656cbe: *stop=0x8c656cbeu;if(!valid_access(r[8]+8u,4u))return -3;wr(m,bus,&io_status,r[8]+0x8u,r[3],4);if(io_status)return io_status;goto L8c656cc0; /* mov.l r3,@(0x8,r8) */
L8c656cc0: *stop=0x8c656cc0u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656cc2; /* mov.l @(0xc,r4),r2 */
L8c656cc2: *stop=0x8c656cc2u;if(!valid_access(r[8]+12u,4u))return -3;wr(m,bus,&io_status,r[8]+0xcu,r[2],4);if(io_status)return io_status;goto L8c656cc4; /* mov.l r2,@(0xc,r8) */
L8c656cc4: *stop=0x8c656cc4u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656cc6; /* mov.l @(0x10,r4),r3 */
L8c656cc6: *stop=0x8c656cc6u;if(!valid_access(r[8]+16u,4u))return -3;wr(m,bus,&io_status,r[8]+0x10u,r[3],4);if(io_status)return io_status;goto L8c656cc8; /* mov.l r3,@(0x10,r8) */
L8c656cc8: *stop=0x8c656ccau;r[4]=r[12];goto L8c656cd4; /* bra 0x8c656cd4 */
L8c656ccc: *stop=0x8c656cccu;r[0]=r[4];goto L8c656cce; /* mov r4,r0 */
L8c656cce: *stop=0x8c656cceu;if(!valid_access(r[0]+r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656cd0; /* mov.l @(r0,r8),r3 */
L8c656cd0: *stop=0x8c656cd0u;r[4]+=4u;goto L8c656cd2; /* add #0x4,r4 */
L8c656cd2: *stop=0x8c656cd2u;if(!valid_access(r[0]+r[6],4u))return -3;wr(m,bus,&io_status,r[6]+r[0],r[3],4);if(io_status)return io_status;goto L8c656cd4; /* mov.l r3,@(r0,r6) */
L8c656cd4: *stop=0x8c656cd4u;*sr=(*sr&~1u)|(r[4]>=r[10]);goto L8c656cd6; /* cmp/hs r10,r4 */
L8c656cd6: if(!(*sr&1u))goto L8c656ccc;goto L8c656cd8; /* bf 0x8c656ccc */
L8c656cd8: *stop=0x8c656cd8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656d54u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656cda; /* mov.l 0x8c656d54,r4 */
L8c656cda: *stop=0x8c656cdau;r[1]=0u;goto L8c656cdc; /* mov #0x0,r1 */
L8c656cdc: *stop=0x8c656cdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656d58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656cde; /* mov.l 0x8c656d58,r3 */
L8c656cde: *stop=0x8c656cdeu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c656ce0; /* mov.l r12,@r4 */
L8c656ce0: *stop=0x8c656ce0u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[12],4);if(io_status)return io_status;goto L8c656ce2; /* mov.l r12,@(0x4,r4) */
L8c656ce2: *stop=0x8c656ce2u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[12],4);if(io_status)return io_status;goto L8c656ce4; /* mov.l r12,@(0x8,r4) */
L8c656ce4: *stop=0x8c656ce4u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[12],4);if(io_status)return io_status;goto L8c656ce6; /* mov.l r12,@(0xc,r4) */
L8c656ce6: *stop=0x8c656ce6u;r[4]=r[5];goto L8c656ce8; /* mov r5,r4 */
L8c656ce8: *stop=0x8c656ce8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c656cea; /* mov.l r5,@r3 */
L8c656cea: *stop=0x8c656ceau;{uint32_t loaded=rd(m,bus,&io_status,0x8c656d5cu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c656cec; /* mov.l 0x8c656d5c,r11 */
L8c656cec: *stop=0x8c656cecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656d60u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656cee; /* mov.l 0x8c656d60,r5 */
L8c656cee: *stop=0x8c656ceeu;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[8],4);if(io_status)return io_status;goto L8c656cf0; /* mov.l r8,@r11 */
L8c656cf0: *stop=0x8c656cf0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656cf2; /* mov.l @(0x4,r15),r2 */
L8c656cf2: *stop=0x8c656cf2u;*sr=(*sr&~1u)|(r[1]>=r[2]);goto L8c656cf4; /* cmp/hs r2,r1 */
L8c656cf4: take=(*sr&1u);*stop=0x8c656cf6u;r[6]=r[12];if(take)goto L8c656d06;goto L8c656cf8; /* bt/s 0x8c656d06 */
L8c656cf8: *stop=0x8c656cf8u;r[6]+=1u;goto L8c656cfa; /* add #0x1,r6 */
L8c656cfa: *stop=0x8c656cfau;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c656cfc; /* mov.l r4,@r5 */
L8c656cfc: *stop=0x8c656cfcu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656cfe; /* mov.l @(0x4,r15),r3 */
L8c656cfe: *stop=0x8c656cfeu;r[4]+=20u;goto L8c656d00; /* add #0x14,r4 */
L8c656d00: *stop=0x8c656d00u;*sr=(*sr&~1u)|(r[6]>=r[3]);goto L8c656d02; /* cmp/hs r3,r6 */
L8c656d02: take=!(*sr&1u);*stop=0x8c656d04u;r[5]+=4u;if(take)goto L8c656cf8;goto L8c656d06; /* bf/s 0x8c656cf8 */
L8c656d06: target=0x8c657240u;*pr=0x8c656d0au;*stop=0x8c656d08u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656d0a; /* bsr 0x8c657240 */
L8c656d0a: *stop=0x8c656d0cu;r[4]=r[12];goto L8c656d20; /* bra 0x8c656d20 */
L8c656d0e: *stop=0x8c656d0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656d44u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656d10; /* mov.l 0x8c656d44,r0 */
L8c656d10: *stop=0x8c656d10u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656d12; /* mov.l @r11,r2 */
L8c656d12: *stop=0x8c656d12u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656d14; /* mov.l @(r0,r4),r3 */
L8c656d14: *stop=0x8c656d14u;r[2]+=r[4];goto L8c656d16; /* add r4,r2 */
L8c656d16: *stop=0x8c656d16u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c656d18; /* mov.l @r2,r1 */
L8c656d18: *stop=0x8c656d18u;r[3]<<=2;goto L8c656d1a; /* shll2 r3 */
L8c656d1a: *stop=0x8c656d1au;r[4]+=4u;goto L8c656d1c; /* add #0x4,r4 */
L8c656d1c: *stop=0x8c656d1cu;r[1]+=r[3];goto L8c656d1e; /* add r3,r1 */
L8c656d1e: *stop=0x8c656d1eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c656d20; /* mov.l r1,@r2 */
L8c656d20: *stop=0x8c656d20u;*sr=(*sr&~1u)|(r[4]>=r[10]);goto L8c656d22; /* cmp/hs r10,r4 */
L8c656d22: if(!(*sr&1u))goto L8c656d0e;goto L8c656d24; /* bf 0x8c656d0e */
L8c656d24: *stop=0x8c656d26u;goto L8c656e14; /* bra 0x8c656e14 */
L8c656d80: *stop=0x8c656d80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656f80u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656d82; /* mov.l 0x8c656f80,r1 */
L8c656d82: *stop=0x8c656d82u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[9],4);if(io_status)return io_status;goto L8c656d84; /* mov.l r9,@r1 */
L8c656d84: *stop=0x8c656d84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656f84u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656d86; /* mov.l 0x8c656f84,r0 */
L8c656d86: *stop=0x8c656d86u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[9],4);if(io_status)return io_status;goto L8c656d88; /* mov.l r9,@r4 */
L8c656d88: *stop=0x8c656d88u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656d8a; /* mov.l @r0,r2 */
L8c656d8a: *stop=0x8c656d8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c656f88u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656d8c; /* mov.l 0x8c656f88,r3 */
L8c656d8c: *stop=0x8c656d8cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c656d8e; /* mov.l r2,@r3 */
L8c656d8e: *stop=0x8c656d8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656f84u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656d90; /* mov.l 0x8c656f84,r2 */
L8c656d90: *stop=0x8c656d90u;r[0]+=4294967280u;goto L8c656d92; /* add #-0x10,r0 */
L8c656d92: *stop=0x8c656d92u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c656d94; /* mov.l @r2,r1 */
L8c656d94: *stop=0x8c656d94u;r[1]<<=2;goto L8c656d96; /* shll2 r1 */
L8c656d96: *stop=0x8c656d96u;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[11],4);if(io_status)return io_status;goto L8c656d98; /* mov.l r11,@(r0,r1) */
L8c656d98: *stop=0x8c656d98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656f8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656d9a; /* mov.l 0x8c656f8c,r3 */
L8c656d9a: *stop=0x8c656d9au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8c656d9c; /* mov.l r12,@r3 */
L8c656d9c: *stop=0x8c656d9cu;r[2]+=24u;goto L8c656d9e; /* add #0x18,r2 */
L8c656d9e: *stop=0x8c656d9eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[9],4);if(io_status)return io_status;goto L8c656da0; /* mov.l r9,@r2 */
L8c656da0: *stop=0x8c656da0u;r[4]=r[15];goto L8c656da2; /* mov r15,r4 */
L8c656da2: *stop=0x8c656da2u;r[4]+=36u;goto L8c656da4; /* add #0x24,r4 */
L8c656da4: *stop=0x8c656da4u;r[1]=r[4];goto L8c656da6; /* mov r4,r1 */
L8c656da6: *stop=0x8c656da6u;r[10]=r[1];goto L8c656da8; /* mov r1,r10 */
L8c656da8: *stop=0x8c656da8u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c656daa; /* mov.l r1,@(0x4,r15) */
L8c656daa: *stop=0x8c656daau;r[10]+=4u;goto L8c656dac; /* add #0x4,r10 */
L8c656dac: *stop=0x8c656dacu;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656dae; /* mov.l @(0x20,r14),r3 */
L8c656dae: *stop=0x8c656daeu;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656db0; /* mov.l @(0x10,r3),r0 */
L8c656db0: *stop=0x8c656db0u;r[0]&=r[9];goto L8c656db2; /* and r9,r0 */
L8c656db2: *stop=0x8c656db2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c656db4; /* mov.l r0,@r1 */
L8c656db4: *stop=0x8c656db4u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656db6; /* mov.l @(0x20,r14),r3 */
L8c656db6: *stop=0x8c656db6u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656db8; /* mov.l @(0x10,r3),r1 */
L8c656db8: *stop=0x8c656db8u;r[3]=r[4];goto L8c656dba; /* mov r4,r3 */
L8c656dba: *stop=0x8c656dbau;r[3]+=12u;goto L8c656dbc; /* add #0xc,r3 */
L8c656dbc: *stop=0x8c656dbcu;r[11]&=r[1];goto L8c656dbe; /* and r1,r11 */
L8c656dbe: *stop=0x8c656dbeu;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[11],4);if(io_status)return io_status;goto L8c656dc0; /* mov.l r11,@r10 */
L8c656dc0: *stop=0x8c656dc0u;r[11]=r[4];goto L8c656dc2; /* mov r4,r11 */
L8c656dc2: *stop=0x8c656dc2u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656dc4; /* mov.l @(0x20,r14),r0 */
L8c656dc4: *stop=0x8c656dc4u;r[11]+=8u;goto L8c656dc6; /* add #0x8,r11 */
L8c656dc6: *stop=0x8c656dc6u;r[4]+=16u;goto L8c656dc8; /* add #0x10,r4 */
L8c656dc8: *stop=0x8c656dc8u;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656dca; /* mov.l @(0x10,r0),r0 */
L8c656dca: *stop=0x8c656dcau;r[0]&=0x4u;goto L8c656dcc; /* and #0x4,r0 */
L8c656dcc: *stop=0x8c656dccu;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[0],4);if(io_status)return io_status;goto L8c656dce; /* mov.l r0,@r11 */
L8c656dce: *stop=0x8c656dceu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c656dd0; /* mov.l r3,@r15 */
L8c656dd0: *stop=0x8c656dd0u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656dd2; /* mov.l @(0x20,r14),r0 */
L8c656dd2: *stop=0x8c656dd2u;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656dd4; /* mov.l @(0x10,r0),r0 */
L8c656dd4: *stop=0x8c656dd4u;r[0]&=0x8u;goto L8c656dd6; /* and #0x8,r0 */
L8c656dd6: *stop=0x8c656dd6u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c656dd8; /* mov.l r0,@r3 */
L8c656dd8: *stop=0x8c656dd8u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c656dda; /* mov.l r4,@(0x8,r15) */
L8c656dda: *stop=0x8c656ddau;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656ddc; /* mov.l @(0x20,r14),r0 */
L8c656ddc: *stop=0x8c656ddcu;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656dde; /* mov.l @(0x10,r0),r0 */
L8c656dde: *stop=0x8c656ddeu;r[0]&=0x10u;goto L8c656de0; /* and #0x10,r0 */
L8c656de0: *stop=0x8c656de0u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c656de2; /* mov.l r0,@r4 */
L8c656de2: *stop=0x8c656de2u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656de4; /* mov.l @(0x8,r15),r3 */
L8c656de4: *stop=0x8c656de4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c656de6; /* mov.l @r3,r1 */
L8c656de6: *stop=0x8c656de6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c656de8; /* mov.l r1,@-r15 */
L8c656de8: *stop=0x8c656de8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656dea; /* mov.l @(0x4,r15),r7 */
L8c656dea: *stop=0x8c656deau;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[5]=loaded;}goto L8c656dec; /* mov.l @r10,r5 */
L8c656dec: *stop=0x8c656decu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656dee; /* mov.l @(0x8,r15),r4 */
L8c656dee: *stop=0x8c656deeu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[7]=loaded;}goto L8c656df0; /* mov.l @r7,r7 */
L8c656df0: *stop=0x8c656df0u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[6]=loaded;}goto L8c656df2; /* mov.l @r11,r6 */
L8c656df2: target=0x8c656f50u;*pr=0x8c656df6u;*stop=0x8c656df4u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656df6; /* bsr 0x8c656f50 */
L8c656df6: *stop=0x8c656df6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c656df8; /* mov.l r9,@-r15 */
L8c656df8: *stop=0x8c656df8u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656dfa; /* mov.l @(0x10,r15),r2 */
L8c656dfa: *stop=0x8c656dfau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656dfc; /* mov.l @r2,r3 */
L8c656dfc: *stop=0x8c656dfcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c656dfe; /* mov.l r3,@-r15 */
L8c656dfe: *stop=0x8c656dfeu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656e00; /* mov.l @(0xc,r15),r7 */
L8c656e00: *stop=0x8c656e00u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[5]=loaded;}goto L8c656e02; /* mov.l @r10,r5 */
L8c656e02: *stop=0x8c656e02u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656e04; /* mov.l @(0x10,r15),r4 */
L8c656e04: *stop=0x8c656e04u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[7]=loaded;}goto L8c656e06; /* mov.l @r7,r7 */
L8c656e06: *stop=0x8c656e06u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[6]=loaded;}goto L8c656e08; /* mov.l @r11,r6 */
L8c656e08: target=0x8c657096u;*pr=0x8c656e0cu;*stop=0x8c656e0au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656e0c; /* bsr 0x8c657096 */
L8c656e0c: *stop=0x8c656e0cu;r[15]+=12u;goto L8c656e0e; /* add #0xc,r15 */
L8c656e0e: *stop=0x8c656e0eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c656e10; /* mov.l r0,@r15 */
L8c656e10: target=0x8c6573c4u;*pr=0x8c656e14u;*stop=0x8c656e12u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656e14; /* bsr 0x8c6573c4 */
L8c656e14: *stop=0x8c656e14u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656e16; /* mov.l @(0x1c,r14),r2 */
L8c656e16: *stop=0x8c656e16u;r[0]=92u;goto L8c656e18; /* mov #0x5c,r0 */
L8c656e18: *stop=0x8c656e18u;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[2],4);if(io_status)return io_status;goto L8c656e1a; /* mov.l r2,@(r0,r13) */
L8c656e1a: *stop=0x8c656e1au;r[0]=r[2];goto L8c656e1c; /* mov r2,r0 */
L8c656e1c: *stop=0x8c656e1cu;*sr=(*sr&~1u)|!(r[0]&0x7u);goto L8c656e1e; /* tst #0x7,r0 */
L8c656e1e: if((*sr&1u))goto L8c656e2c;goto L8c656e20; /* bt 0x8c656e2c */
L8c656e20: *stop=0x8c656e20u;r[0]=92u;goto L8c656e22; /* mov #0x5c,r0 */
L8c656e22: *stop=0x8c656e22u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656e24; /* mov.l @(r0,r13),r2 */
L8c656e24: *stop=0x8c656e24u;r[3]=4294967288u;goto L8c656e26; /* mov #-0x8,r3 */
L8c656e26: *stop=0x8c656e26u;r[2]+=7u;goto L8c656e28; /* add #0x7,r2 */
L8c656e28: *stop=0x8c656e28u;r[2]&=r[3];goto L8c656e2a; /* and r3,r2 */
L8c656e2a: *stop=0x8c656e2au;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[2],4);if(io_status)return io_status;goto L8c656e2c; /* mov.l r2,@(r0,r13) */
L8c656e2c: *stop=0x8c656e2cu;r[0]=92u;goto L8c656e2e; /* mov #0x5c,r0 */
L8c656e2e: *stop=0x8c656e2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656f90u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656e30; /* mov.l 0x8c656f90,r2 */
L8c656e30: *stop=0x8c656e30u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c656e32; /* mov.l @(r0,r13),r1 */
L8c656e32: *stop=0x8c656e32u;r[1]<<=2;goto L8c656e34; /* shll2 r1 */
L8c656e34: *stop=0x8c656e34u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656e36; /* mov.l @r2,r3 */
L8c656e36: *stop=0x8c656e36u;r[0]=96u;goto L8c656e38; /* mov #0x60,r0 */
L8c656e38: *stop=0x8c656e38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656f98u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656e3a; /* mov.l 0x8c656f98,r4 */
L8c656e3a: *stop=0x8c656e3au;r[3]-=r[1];goto L8c656e3c; /* sub r1,r3 */
L8c656e3c: *stop=0x8c656e3cu;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c656e3e; /* mov.l @(r0,r13),r1 */
L8c656e3e: *stop=0x8c656e3eu;r[0]=88u;goto L8c656e40; /* mov #0x58,r0 */
L8c656e40: *stop=0x8c656e40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656f94u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656e42; /* mov.l 0x8c656f94,r5 */
L8c656e42: *stop=0x8c656e42u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c656e44; /* shll r1 */
L8c656e44: *stop=0x8c656e44u;r[3]-=r[1];goto L8c656e46; /* sub r1,r3 */
L8c656e46: *stop=0x8c656e46u;*sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c656e48; /* shlr r3 */
L8c656e48: *stop=0x8c656e48u;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[3],4);if(io_status)return io_status;goto L8c656e4a; /* mov.l r3,@(r0,r13) */
L8c656e4a: *stop=0x8c656e4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c656f9cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656e4c; /* mov.l 0x8c656f9c,r3 */
L8c656e4c: target=r[3];*pr=0x8c656e50u;*stop=0x8c656e4eu;r[6]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656e50; /* jsr @r3 */
L8c656e50: *stop=0x8c656e50u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c656e52; /* cmp/eq #0x1,r0 */
L8c656e52: if(!(*sr&1u))goto L8c656e62;goto L8c656e54; /* bf 0x8c656e62 */
L8c656e54: *stop=0x8c656e54u;r[0]=88u;goto L8c656e56; /* mov #0x58,r0 */
L8c656e56: *stop=0x8c656e56u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fa0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656e58; /* mov.l 0x8c656fa0,r4 */
L8c656e58: *stop=0x8c656e58u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656e5a; /* mov.l @(r0,r13),r3 */
L8c656e5a: *stop=0x8c656e5au;*sr=(*sr&~1u)|(r[3]>r[4]);goto L8c656e5c; /* cmp/hi r4,r3 */
L8c656e5c: if(!(*sr&1u))goto L8c656e6e;goto L8c656e5e; /* bf 0x8c656e6e */
L8c656e5e: *stop=0x8c656e60u;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[4],4);if(io_status)return io_status;goto L8c656e6e; /* bra 0x8c656e6e */
L8c656e62: *stop=0x8c656e62u;r[0]=88u;goto L8c656e64; /* mov #0x58,r0 */
L8c656e64: *stop=0x8c656e64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fa4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656e66; /* mov.l 0x8c656fa4,r4 */
L8c656e66: *stop=0x8c656e66u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656e68; /* mov.l @(r0,r13),r2 */
L8c656e68: *stop=0x8c656e68u;*sr=(*sr&~1u)|(r[2]>r[4]);goto L8c656e6a; /* cmp/hi r4,r2 */
L8c656e6a: if(!(*sr&1u))goto L8c656e6e;goto L8c656e6c; /* bf 0x8c656e6e */
L8c656e6c: *stop=0x8c656e6cu;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[4],4);if(io_status)return io_status;goto L8c656e6e; /* mov.l r4,@(r0,r13) */
L8c656e6e: *stop=0x8c656e6eu;r[0]=88u;goto L8c656e70; /* mov #0x58,r0 */
L8c656e70: *stop=0x8c656e70u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[11]=loaded;}goto L8c656e72; /* mov.l @(r0,r13),r11 */
L8c656e72: *stop=0x8c656e72u;r[0]=100u;goto L8c656e74; /* mov #0x64,r0 */
L8c656e74: *stop=0x8c656e74u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656e76; /* mov.l @(r0,r13),r3 */
L8c656e76: *stop=0x8c656e76u;*sr=(*sr&~1u)|(r[11]>=r[3]);goto L8c656e78; /* cmp/hs r3,r11 */
L8c656e78: if((*sr&1u))goto L8c656e7e;goto L8c656e7a; /* bt 0x8c656e7e */
L8c656e7a: *stop=0x8c656e7cu;r[0]=3u;goto L8c656f3a; /* bra 0x8c656f3a */
L8c656e7e: *stop=0x8c656e7eu;r[5]=r[15];goto L8c656e80; /* mov r15,r5 */
L8c656e80: *stop=0x8c656e80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fa8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656e82; /* mov.l 0x8c656fa8,r3 */
L8c656e82: *stop=0x8c656e82u;r[6]=r[15];goto L8c656e84; /* mov r15,r6 */
L8c656e84: *stop=0x8c656e84u;r[5]+=56u;goto L8c656e86; /* add #0x38,r5 */
L8c656e86: *stop=0x8c656e86u;r[6]+=60u;goto L8c656e88; /* add #0x3c,r6 */
L8c656e88: target=r[3];*pr=0x8c656e8cu;*stop=0x8c656e8au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656e8c; /* jsr @r3 */
L8c656e8c: *stop=0x8c656e8cu;r[4]=r[0];goto L8c656e8e; /* mov r0,r4 */
L8c656e8e: *stop=0x8c656e8eu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c656e90; /* tst r4,r4 */
L8c656e90: if(!(*sr&1u))goto L8c656f38;goto L8c656e92; /* bf 0x8c656f38 */
L8c656e92: *stop=0x8c656e92u;r[0]=88u;goto L8c656e94; /* mov #0x58,r0 */
L8c656e94: *stop=0x8c656e94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656facu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656e96; /* mov.l 0x8c656fac,r2 */
L8c656e96: *stop=0x8c656e96u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656e98; /* mov.l @(r0,r13),r3 */
L8c656e98: *stop=0x8c656e98u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c656e9a; /* mov.l r3,@r2 */
L8c656e9a: *stop=0x8c656e9au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c656e9c; /* mov.l r12,@-r15 */
L8c656e9c: *stop=0x8c656e9cu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656e9e; /* mov.l @(0x10,r15),r3 */
L8c656e9e: *stop=0x8c656e9eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656ea0; /* mov.l @(0x4,r15),r1 */
L8c656ea0: *stop=0x8c656ea0u;r[3]=~r[3];goto L8c656ea2; /* not r3,r3 */
L8c656ea2: *stop=0x8c656ea2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fb0u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656ea4; /* mov.l 0x8c656fb0,r7 */
L8c656ea4: *stop=0x8c656ea4u;r[3]&=r[1];goto L8c656ea6; /* and r1,r3 */
L8c656ea6: *stop=0x8c656ea6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c656ea8; /* mov.l r3,@-r15 */
L8c656ea8: *stop=0x8c656ea8u;if(!valid_access(r[7]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656eaa; /* mov.l @(0xc,r7),r7 */
L8c656eaa: *stop=0x8c656eaau;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fb0u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656eac; /* mov.l 0x8c656fb0,r6 */
L8c656eac: *stop=0x8c656eacu;r[3]=4294967289u;goto L8c656eae; /* mov #-0x7,r3 */
L8c656eae: *stop=0x8c656eaeu;r[7]=(r[3]&0x80000000u)?((r[3]&31u)?r[7]>>((0u-r[3])&31u):0u):(r[7]<<(r[3]&31u));goto L8c656eb0; /* shld r3,r7 */
L8c656eb0: *stop=0x8c656eb0u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656eb2; /* mov.l @(0x8,r6),r6 */
L8c656eb2: *stop=0x8c656eb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fb0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656eb4; /* mov.l 0x8c656fb0,r5 */
L8c656eb4: *stop=0x8c656eb4u;r[6]=(r[3]&0x80000000u)?((r[3]&31u)?r[6]>>((0u-r[3])&31u):0u):(r[6]<<(r[3]&31u));goto L8c656eb6; /* shld r3,r6 */
L8c656eb6: *stop=0x8c656eb6u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656eb8; /* mov.l @(0x4,r5),r5 */
L8c656eb8: *stop=0x8c656eb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fb0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656eba; /* mov.l 0x8c656fb0,r4 */
L8c656eba: *stop=0x8c656ebau;r[5]=(r[3]&0x80000000u)?((r[3]&31u)?r[5]>>((0u-r[3])&31u):0u):(r[5]<<(r[3]&31u));goto L8c656ebc; /* shld r3,r5 */
L8c656ebc: *stop=0x8c656ebcu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c656ebe; /* mov.l @r4,r4 */
L8c656ebe: *stop=0x8c656ebeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fb4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656ec0; /* mov.l 0x8c656fb4,r1 */
L8c656ec0: target=r[1];*pr=0x8c656ec4u;*stop=0x8c656ec2u;r[4]=(r[3]&0x80000000u)?((r[3]&31u)?r[4]>>((0u-r[3])&31u):0u):(r[4]<<(r[3]&31u));if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656ec4; /* jsr @r1 */
L8c656ec4: *stop=0x8c656ec4u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656ec6; /* mov.l @(0x4,r14),r2 */
L8c656ec6: *stop=0x8c656ec6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fb8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656ec8; /* mov.l 0x8c656fb8,r3 */
L8c656ec8: *stop=0x8c656ec8u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c656eca; /* tst r3,r2 */
L8c656eca: take=(*sr&1u);*stop=0x8c656eccu;r[15]+=8u;if(take)goto L8c656ed2;goto L8c656ece; /* bt/s 0x8c656ed2 */
L8c656ece: *stop=0x8c656eceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656f8cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656ed0; /* mov.l 0x8c656f8c,r0 */
L8c656ed0: *stop=0x8c656ed0u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[9],4);if(io_status)return io_status;goto L8c656ed2; /* mov.l r9,@r0 */
L8c656ed2: *stop=0x8c656ed2u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656ed4; /* mov.l @(0x20,r14),r3 */
L8c656ed4: *stop=0x8c656ed4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fc8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656ed6; /* mov.l 0x8c656fc8,r1 */
L8c656ed6: *stop=0x8c656ed6u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[8],4);if(io_status)return io_status;goto L8c656ed8; /* mov.l r8,@r3 */
L8c656ed8: *stop=0x8c656ed8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fbcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656eda; /* mov.l 0x8c656fbc,r3 */
L8c656eda: *stop=0x8c656edau;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656edc; /* mov.l @(0x20,r14),r2 */
L8c656edc: *stop=0x8c656edcu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c656ede; /* mov.l r3,@(0x4,r2) */
L8c656ede: *stop=0x8c656edeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fc0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656ee0; /* mov.l 0x8c656fc0,r3 */
L8c656ee0: *stop=0x8c656ee0u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656ee2; /* mov.l @(0x20,r14),r2 */
L8c656ee2: *stop=0x8c656ee2u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c656ee4; /* mov.l r3,@(0x8,r2) */
L8c656ee4: *stop=0x8c656ee4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fc4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656ee6; /* mov.l 0x8c656fc4,r3 */
L8c656ee6: *stop=0x8c656ee6u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656ee8; /* mov.l @(0x20,r14),r2 */
L8c656ee8: *stop=0x8c656ee8u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[3],4);if(io_status)return io_status;goto L8c656eea; /* mov.l r3,@(0xc,r2) */
L8c656eea: *stop=0x8c656eeau;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656eec; /* mov.l @(0xc,r15),r2 */
L8c656eec: *stop=0x8c656eecu;if(!valid_access(r[13]+24u,4u))return -3;wr(m,bus,&io_status,r[13]+0x18u,r[2],4);if(io_status)return io_status;goto L8c656eee; /* mov.l r2,@(0x18,r13) */
L8c656eee: *stop=0x8c656eeeu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656ef0; /* mov.l @(0xc,r15),r3 */
L8c656ef0: *stop=0x8c656ef0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c656ef2; /* mov.l r3,@r1 */
L8c656ef2: *stop=0x8c656ef2u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656ef4; /* mov.l @(0x8,r14),r2 */
L8c656ef4: *stop=0x8c656ef4u;r[0]=64u;goto L8c656ef6; /* mov #0x40,r0 */
L8c656ef6: *stop=0x8c656ef6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fccu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656ef8; /* mov.l 0x8c656fcc,r4 */
L8c656ef8: *stop=0x8c656ef8u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656efa; /* mov.l @r2,r3 */
L8c656efa: *stop=0x8c656efau;if(!valid_access(r[13]+28u,4u))return -3;wr(m,bus,&io_status,r[13]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c656efc; /* mov.l r3,@(0x1c,r13) */
L8c656efc: *stop=0x8c656efcu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656efe; /* mov.l @(0x10,r14),r2 */
L8c656efe: *stop=0x8c656efeu;if(!valid_access(r[13]+32u,4u))return -3;wr(m,bus,&io_status,r[13]+0x20u,r[2],4);if(io_status)return io_status;goto L8c656f00; /* mov.l r2,@(0x20,r13) */
L8c656f00: *stop=0x8c656f00u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656f02; /* mov.l @(0x14,r14),r3 */
L8c656f02: *stop=0x8c656f02u;if(!valid_access(r[13]+36u,4u))return -3;wr(m,bus,&io_status,r[13]+0x24u,r[3],4);if(io_status)return io_status;goto L8c656f04; /* mov.l r3,@(0x24,r13) */
L8c656f04: *stop=0x8c656f04u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656f06; /* mov.l @(0x10,r15),r2 */
L8c656f06: *stop=0x8c656f06u;if(!valid_access(r[13]+40u,4u))return -3;wr(m,bus,&io_status,r[13]+0x28u,r[2],4);if(io_status)return io_status;goto L8c656f08; /* mov.l r2,@(0x28,r13) */
L8c656f08: *stop=0x8c656f08u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656f0a; /* mov.l @(0x8,r14),r3 */
L8c656f0a: *stop=0x8c656f0au;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656f0c; /* mov.l @(0x4,r3),r2 */
L8c656f0c: *stop=0x8c656f0cu;if(!valid_access(r[13]+44u,4u))return -3;wr(m,bus,&io_status,r[13]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c656f0e; /* mov.l r2,@(0x2c,r13) */
L8c656f0e: *stop=0x8c656f0eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656f10; /* mov.l @(0x10,r14),r3 */
L8c656f10: *stop=0x8c656f10u;if(!valid_access(r[13]+48u,4u))return -3;wr(m,bus,&io_status,r[13]+0x30u,r[3],4);if(io_status)return io_status;goto L8c656f12; /* mov.l r3,@(0x30,r13) */
L8c656f12: *stop=0x8c656f12u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656f14; /* mov.l @(0x14,r14),r2 */
L8c656f14: *stop=0x8c656f14u;if(!valid_access(r[13]+52u,4u))return -3;wr(m,bus,&io_status,r[13]+0x34u,r[2],4);if(io_status)return io_status;goto L8c656f16; /* mov.l r2,@(0x34,r13) */
L8c656f16: *stop=0x8c656f16u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656f18; /* mov.l @(0x10,r15),r3 */
L8c656f18: *stop=0x8c656f18u;if(!valid_access(r[13]+56u,4u))return -3;wr(m,bus,&io_status,r[13]+0x38u,r[3],4);if(io_status)return io_status;goto L8c656f1a; /* mov.l r3,@(0x38,r13) */
L8c656f1a: *stop=0x8c656f1au;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656f1c; /* mov.l @(0x8,r14),r2 */
L8c656f1c: *stop=0x8c656f1cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656f1e; /* mov.l @r2,r3 */
L8c656f1e: *stop=0x8c656f1eu;if(!valid_access(r[13]+60u,4u))return -3;wr(m,bus,&io_status,r[13]+0x3cu,r[3],4);if(io_status)return io_status;goto L8c656f20; /* mov.l r3,@(0x3c,r13) */
L8c656f20: *stop=0x8c656f20u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656f22; /* mov.l @(0x8,r14),r2 */
L8c656f22: *stop=0x8c656f22u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656f24; /* mov.l @(0x4,r2),r3 */
L8c656f24: *stop=0x8c656f24u;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[3],4);if(io_status)return io_status;goto L8c656f26; /* mov.l r3,@(r0,r13) */
L8c656f26: *stop=0x8c656f26u;r[0]=68u;goto L8c656f28; /* mov #0x44,r0 */
L8c656f28: *stop=0x8c656f28u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656f2a; /* mov.l @(0x20,r15),r2 */
L8c656f2a: *stop=0x8c656f2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c656fd0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656f2c; /* mov.l 0x8c656fd0,r3 */
L8c656f2c: *stop=0x8c656f2cu;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[2],4);if(io_status)return io_status;goto L8c656f2e; /* mov.l r2,@(r0,r13) */
L8c656f2e: *stop=0x8c656f2eu;r[0]=84u;goto L8c656f30; /* mov #0x54,r0 */
L8c656f30: *stop=0x8c656f30u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c656f32; /* mov.l r4,@r3 */
L8c656f32: target=0x8c657760u;*pr=0x8c656f36u;*stop=0x8c656f34u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656f36; /* bsr 0x8c657760 */
L8c656f36: *stop=0x8c656f36u;r[4]=r[0];goto L8c656f38; /* mov r0,r4 */
L8c656f38: *stop=0x8c656f38u;r[0]=r[4];goto L8c656f3a; /* mov r4,r0 */
L8c656f3a: *stop=0x8c656f3au;r[15]+=104u;goto L8c656f3c; /* add #0x68,r15 */
L8c656f3c: *stop=0x8c656f3cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c656f3e; /* lds.l @r15+,PR */
L8c656f3e: *stop=0x8c656f3eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c656f40; /* fmov.s @r15+,fr15 */
L8c656f40: *stop=0x8c656f40u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c656f42; /* mov.l @r15+,r8 */
L8c656f42: *stop=0x8c656f42u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c656f44; /* mov.l @r15+,r9 */
L8c656f44: *stop=0x8c656f44u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c656f46; /* mov.l @r15+,r10 */
L8c656f46: *stop=0x8c656f46u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c656f48; /* mov.l @r15+,r11 */
L8c656f48: *stop=0x8c656f48u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c656f4a; /* mov.l @r15+,r12 */
L8c656f4a: *stop=0x8c656f4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c656f4c; /* mov.l @r15+,r13 */
L8c656f4c: *stop=0x8c656f4cu;if(*pr!=entry_pr)return -3;*stop=0x8c656f4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c656860(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c656860;
L8c656860: *stop=0x8c656860u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c656862; /* mov.l r14,@-r15 */
L8c656862: *stop=0x8c656862u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c656864; /* mov.l r13,@-r15 */
L8c656864: *stop=0x8c656864u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c656866; /* sts.l PR,@-r15 */
L8c656866: *stop=0x8c656866u;r[15]+=4294967288u;goto L8c656868; /* add #-0x8,r15 */
L8c656868: *stop=0x8c656868u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65686a; /* mov.l 0x8c6569a0,r3 */
L8c65686a: *stop=0x8c65686au;r[14]=0u;goto L8c65686c; /* mov #0x0,r14 */
L8c65686c: *stop=0x8c65686cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c65686e; /* mov.l r4,@(0x4,r15) */
L8c65686e: *stop=0x8c65686eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c656870; /* mov.l r5,@r15 */
L8c656870: *stop=0x8c656870u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c656872; /* mov.l r14,@r3 */
L8c656872: *stop=0x8c656872u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656874; /* mov.l 0x8c6569a4,r1 */
L8c656874: *stop=0x8c656874u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656876; /* mov.l @r1,r2 */
L8c656876: *stop=0x8c656876u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c656878; /* tst r2,r2 */
L8c656878: take=(*sr&1u);*stop=0x8c65687au;r[13]=10u;if(take)goto L8c656894;goto L8c65687c; /* bt/s 0x8c656894 */
L8c65687c: *stop=0x8c65687cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65687e; /* mov.l 0x8c6569a8,r3 */
L8c65687e: *stop=0x8c65687eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656880; /* mov.l @r3,r2 */
L8c656880: target=r[2];*pr=0x8c656884u;*stop=0x8c656882u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656884; /* jsr @r2 */
L8c656884: *stop=0x8c656884u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656886; /* mov.l 0x8c6569a0,r2 */
L8c656886: *stop=0x8c656886u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c656888; /* mov.l @r2,r1 */
L8c656888: *stop=0x8c656888u;*sr=(*sr&~1u)|(r[1]>r[13]);goto L8c65688a; /* cmp/hi r13,r1 */
L8c65688a: if((*sr&1u))goto L8c656894;goto L8c65688c; /* bt 0x8c656894 */
L8c65688c: *stop=0x8c65688cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569a4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65688e; /* mov.l 0x8c6569a4,r0 */
L8c65688e: *stop=0x8c65688eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656890; /* mov.l @r0,r3 */
L8c656890: *stop=0x8c656890u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c656892; /* tst r3,r3 */
L8c656892: if(!(*sr&1u))goto L8c65687c;goto L8c656894; /* bf 0x8c65687c */
L8c656894: *stop=0x8c656894u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656896; /* mov.l 0x8c6569ac,r2 */
L8c656896: *stop=0x8c656896u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656898; /* mov.l @(0x4,r15),r3 */
L8c656898: *stop=0x8c656898u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c65689a; /* mov.l r3,@r2 */
L8c65689a: *stop=0x8c65689au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65689c; /* mov.l 0x8c6569b0,r3 */
L8c65689c: *stop=0x8c65689cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65689e; /* mov.l @r15,r1 */
L8c65689e: *stop=0x8c65689eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c6568a0; /* mov.l r1,@r3 */
L8c6568a0: *stop=0x8c6568a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6568a2; /* mov.l @r15,r0 */
L8c6568a2: *stop=0x8c6568a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569b4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6568a4; /* mov.l 0x8c6569b4,r1 */
L8c6568a4: *stop=0x8c6568a4u;r[0]&=0xfu;goto L8c6568a6; /* and #0xf,r0 */
L8c6568a6: *stop=0x8c6568a6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c6568a8; /* mov.l r0,@r1 */
L8c6568a8: *stop=0x8c6568a8u;r[2]+=8u;goto L8c6568aa; /* add #0x8,r2 */
L8c6568aa: *stop=0x8c6568aau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c6568ac; /* mov.l r14,@r2 */
L8c6568ac: *stop=0x8c6568acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6568ae; /* mov.l 0x8c6569b8,r3 */
L8c6568ae: *stop=0x8c6568aeu;r[4]=4294967295u;goto L8c6568b0; /* mov #-0x1,r4 */
L8c6568b0: *stop=0x8c6568b0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c6568b2; /* mov.l r4,@r3 */
L8c6568b2: *stop=0x8c6568b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6569bcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6568b4; /* mov.l 0x8c6569bc,r1 */
L8c6568b4: *stop=0x8c6568b4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c6568b6; /* mov.l r4,@r1 */
L8c6568b6: *stop=0x8c6568b6u;r[2]+=12u;goto L8c6568b8; /* add #0xc,r2 */
L8c6568b8: *stop=0x8c6568b8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c6568ba; /* mov.l r4,@r2 */
L8c6568ba: *stop=0x8c6568bau;r[3]+=12u;goto L8c6568bc; /* add #0xc,r3 */
L8c6568bc: *stop=0x8c6568bcu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6568be; /* mov.l r14,@r3 */
L8c6568be: *stop=0x8c6568beu;r[1]+=12u;goto L8c6568c0; /* add #0xc,r1 */
L8c6568c0: *stop=0x8c6568c0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c6568c2; /* mov.l r14,@r1 */
L8c6568c2: *stop=0x8c6568c2u;r[2]+=12u;goto L8c6568c4; /* add #0xc,r2 */
L8c6568c4: *stop=0x8c6568c4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c6568c6; /* mov.l r14,@r2 */
L8c6568c6: *stop=0x8c6568c6u;r[0]=0u;goto L8c6568c8; /* mov #0x0,r0 */
L8c6568c8: *stop=0x8c6568c8u;r[15]+=8u;goto L8c6568ca; /* add #0x8,r15 */
L8c6568ca: *stop=0x8c6568cau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6568cc; /* lds.l @r15+,PR */
L8c6568cc: *stop=0x8c6568ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6568ce; /* mov.l @r15+,r13 */
L8c6568ce: *stop=0x8c6568ceu;if(*pr!=entry_pr)return -3;*stop=0x8c6568d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c656f50(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c656f50;
L8c656f50: *stop=0x8c656f50u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c656f52; /* mov.l r14,@-r15 */
L8c656f52: *stop=0x8c656f52u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c656f54; /* mov.l r13,@-r15 */
L8c656f54: *stop=0x8c656f54u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c656f56; /* mov.l r12,@-r15 */
L8c656f56: *stop=0x8c656f56u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c656f58; /* mov.l r11,@-r15 */
L8c656f58: *stop=0x8c656f58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c656f5a; /* mov.l r10,@-r15 */
L8c656f5a: *stop=0x8c656f5au;r[15]+=4294967276u;goto L8c656f5c; /* add #-0x14,r15 */
L8c656f5c: *stop=0x8c656f5cu;r[14]=r[15];goto L8c656f5e; /* mov r15,r14 */
L8c656f5e: *stop=0x8c656f5eu;r[11]=0u;goto L8c656f60; /* mov #0x0,r11 */
L8c656f60: *stop=0x8c656f60u;r[13]=r[14];goto L8c656f62; /* mov r14,r13 */
L8c656f62: *stop=0x8c656f62u;r[0]=r[14];goto L8c656f64; /* mov r14,r0 */
L8c656f64: *stop=0x8c656f64u;r[12]=r[11];goto L8c656f66; /* mov r11,r12 */
L8c656f66: *stop=0x8c656f66u;r[10]=r[14];goto L8c656f68; /* mov r14,r10 */
L8c656f68: *stop=0x8c656f6au;r[0]+=20u;goto L8c656fe4; /* bra 0x8c656fe4 */
L8c656fe0: *stop=0x8c656fe0u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[11],4);if(io_status)return io_status;goto L8c656fe2; /* mov.l r11,@r13 */
L8c656fe2: *stop=0x8c656fe2u;r[13]+=4u;goto L8c656fe4; /* add #0x4,r13 */
L8c656fe4: *stop=0x8c656fe4u;*sr=(*sr&~1u)|(r[13]>=r[0]);goto L8c656fe6; /* cmp/hs r0,r13 */
L8c656fe6: if(!(*sr&1u))goto L8c656fe0;goto L8c656fe8; /* bf 0x8c656fe0 */
L8c656fe8: *stop=0x8c656fe8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657120u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656fea; /* mov.l 0x8c657120,r3 */
L8c656fea: *stop=0x8c656feau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c656fec; /* mov.l @r3,r0 */
L8c656fec: *stop=0x8c656fecu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c656fee; /* cmp/eq #0x1,r0 */
L8c656fee: take=!(*sr&1u);*stop=0x8c656ff0u;r[13]=1u;if(take)goto L8c657024;goto L8c656ff2; /* bf/s 0x8c657024 */
L8c656ff2: *stop=0x8c656ff2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657124u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656ff4; /* mov.l 0x8c657124,r1 */
L8c656ff4: *stop=0x8c656ff4u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c656ff6; /* mov.l @r1,r0 */
L8c656ff6: *stop=0x8c656ff6u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c656ff8; /* cmp/eq #0x20,r0 */
L8c656ff8: if(!(*sr&1u))goto L8c656ffc;goto L8c656ffa; /* bf 0x8c656ffc */
L8c656ffa: *stop=0x8c656ffau;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[13],4);if(io_status)return io_status;goto L8c656ffc; /* mov.l r13,@r10 */
L8c656ffc: *stop=0x8c656ffcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656ffe; /* mov.l 0x8c657124,r3 */
L8c656ffe: *stop=0x8c656ffeu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657000; /* mov.l @r3,r0 */
L8c657000: *stop=0x8c657000u;*sr=(*sr&~1u)|(r[0]==0x00000021u);goto L8c657002; /* cmp/eq #0x21,r0 */
L8c657002: if(!(*sr&1u))goto L8c657006;goto L8c657004; /* bf 0x8c657006 */
L8c657004: *stop=0x8c657004u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[13],4);if(io_status)return io_status;goto L8c657006; /* mov.l r13,@(0x4,r14) */
L8c657006: *stop=0x8c657006u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657008; /* mov.l 0x8c657124,r3 */
L8c657008: *stop=0x8c657008u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65700a; /* mov.l @r3,r0 */
L8c65700a: *stop=0x8c65700au;*sr=(*sr&~1u)|(r[0]==0x00000022u);goto L8c65700c; /* cmp/eq #0x22,r0 */
L8c65700c: if(!(*sr&1u))goto L8c657010;goto L8c65700e; /* bf 0x8c657010 */
L8c65700e: *stop=0x8c65700eu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[13],4);if(io_status)return io_status;goto L8c657010; /* mov.l r13,@(0x8,r14) */
L8c657010: *stop=0x8c657010u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657012; /* mov.l 0x8c657124,r3 */
L8c657012: *stop=0x8c657012u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657014; /* mov.l @r3,r0 */
L8c657014: *stop=0x8c657014u;*sr=(*sr&~1u)|(r[0]==0x00000023u);goto L8c657016; /* cmp/eq #0x23,r0 */
L8c657016: if(!(*sr&1u))goto L8c65701a;goto L8c657018; /* bf 0x8c65701a */
L8c657018: *stop=0x8c657018u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[13],4);if(io_status)return io_status;goto L8c65701a; /* mov.l r13,@(0xc,r14) */
L8c65701a: *stop=0x8c65701au;{uint32_t loaded=rd(m,bus,&io_status,0x8c657124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65701c; /* mov.l 0x8c657124,r3 */
L8c65701c: *stop=0x8c65701cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65701e; /* mov.l @r3,r0 */
L8c65701e: *stop=0x8c65701eu;*sr=(*sr&~1u)|(r[0]==0x00000024u);goto L8c657020; /* cmp/eq #0x24,r0 */
L8c657020: if(!(*sr&1u))goto L8c657024;goto L8c657022; /* bf 0x8c657024 */
L8c657022: *stop=0x8c657022u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[13],4);if(io_status)return io_status;goto L8c657024; /* mov.l r13,@(0x10,r14) */
L8c657024: *stop=0x8c657024u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657026; /* mov.l @r10,r3 */
L8c657026: *stop=0x8c657026u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657028; /* mov.l @(0x4,r14),r2 */
L8c657028: *stop=0x8c657028u;r[4]+=r[3];goto L8c65702a; /* add r3,r4 */
L8c65702a: *stop=0x8c65702au;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65702c; /* mov.l @(0x8,r14),r3 */
L8c65702c: *stop=0x8c65702cu;r[5]+=r[2];goto L8c65702e; /* add r2,r5 */
L8c65702e: *stop=0x8c65702eu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657030; /* mov.l @(0xc,r14),r2 */
L8c657030: *stop=0x8c657030u;r[6]+=r[3];goto L8c657032; /* add r3,r6 */
L8c657032: *stop=0x8c657032u;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657034; /* mov.l @(0x28,r15),r1 */
L8c657034: *stop=0x8c657034u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657036; /* mov.l @(0x10,r14),r3 */
L8c657036: *stop=0x8c657036u;r[7]+=r[2];goto L8c657038; /* add r2,r7 */
L8c657038: *stop=0x8c657038u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657128u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65703a; /* mov.l 0x8c657128,r2 */
L8c65703a: *stop=0x8c65703au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c65703c; /* tst r4,r4 */
L8c65703c: *stop=0x8c65703cu;r[1]+=r[3];goto L8c65703e; /* add r3,r1 */
L8c65703e: *stop=0x8c65703eu;if(!valid_access(r[15]+40u,4u))return -3;wr(m,bus,&io_status,r[15]+0x28u,r[1],4);if(io_status)return io_status;goto L8c657040; /* mov.l r1,@(0x28,r15) */
L8c657040: take=(*sr&1u);*stop=0x8c657042u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[11],4);if(io_status)return io_status;if(take)goto L8c65704c;goto L8c657044; /* bt/s 0x8c65704c */
L8c657044: *stop=0x8c657044u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657046; /* mov.l @r2,r0 */
L8c657046: *stop=0x8c657046u;r[0]|=r[13];goto L8c657048; /* or r13,r0 */
L8c657048: *stop=0x8c657048u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c65704a; /* mov.l r0,@r2 */
L8c65704a: *stop=0x8c65704au;r[12]+=1u;goto L8c65704c; /* add #0x1,r12 */
L8c65704c: *stop=0x8c65704cu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c65704e; /* tst r5,r5 */
L8c65704e: if((*sr&1u))goto L8c65705a;goto L8c657050; /* bt 0x8c65705a */
L8c657050: *stop=0x8c657050u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657128u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657052; /* mov.l 0x8c657128,r3 */
L8c657052: *stop=0x8c657052u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657054; /* mov.l @r3,r0 */
L8c657054: *stop=0x8c657054u;r[0]|=0x2u;goto L8c657056; /* or #0x2,r0 */
L8c657056: *stop=0x8c657056u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c657058; /* mov.l r0,@r3 */
L8c657058: *stop=0x8c657058u;r[12]+=1u;goto L8c65705a; /* add #0x1,r12 */
L8c65705a: *stop=0x8c65705au;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c65705c; /* tst r6,r6 */
L8c65705c: if((*sr&1u))goto L8c657068;goto L8c65705e; /* bt 0x8c657068 */
L8c65705e: *stop=0x8c65705eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657128u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657060; /* mov.l 0x8c657128,r3 */
L8c657060: *stop=0x8c657060u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657062; /* mov.l @r3,r0 */
L8c657062: *stop=0x8c657062u;r[0]|=0x4u;goto L8c657064; /* or #0x4,r0 */
L8c657064: *stop=0x8c657064u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c657066; /* mov.l r0,@r3 */
L8c657066: *stop=0x8c657066u;r[12]+=1u;goto L8c657068; /* add #0x1,r12 */
L8c657068: *stop=0x8c657068u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c65706a; /* tst r7,r7 */
L8c65706a: if((*sr&1u))goto L8c657076;goto L8c65706c; /* bt 0x8c657076 */
L8c65706c: *stop=0x8c65706cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657128u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65706e; /* mov.l 0x8c657128,r3 */
L8c65706e: *stop=0x8c65706eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657070; /* mov.l @r3,r0 */
L8c657070: *stop=0x8c657070u;r[0]|=0x8u;goto L8c657072; /* or #0x8,r0 */
L8c657072: *stop=0x8c657072u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c657074; /* mov.l r0,@r3 */
L8c657074: *stop=0x8c657074u;r[12]+=1u;goto L8c657076; /* add #0x1,r12 */
L8c657076: *stop=0x8c657076u;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657078; /* mov.l @(0x28,r15),r2 */
L8c657078: *stop=0x8c657078u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65707a; /* tst r2,r2 */
L8c65707a: if((*sr&1u))goto L8c657086;goto L8c65707c; /* bt 0x8c657086 */
L8c65707c: *stop=0x8c65707cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657128u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65707e; /* mov.l 0x8c657128,r3 */
L8c65707e: *stop=0x8c65707eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657080; /* mov.l @r3,r0 */
L8c657080: *stop=0x8c657080u;r[0]|=0x10u;goto L8c657082; /* or #0x10,r0 */
L8c657082: *stop=0x8c657082u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c657084; /* mov.l r0,@r3 */
L8c657084: *stop=0x8c657084u;r[12]+=1u;goto L8c657086; /* add #0x1,r12 */
L8c657086: *stop=0x8c657086u;r[0]=r[12];goto L8c657088; /* mov r12,r0 */
L8c657088: *stop=0x8c657088u;r[15]+=20u;goto L8c65708a; /* add #0x14,r15 */
L8c65708a: *stop=0x8c65708au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65708c; /* mov.l @r15+,r10 */
L8c65708c: *stop=0x8c65708cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65708e; /* mov.l @r15+,r11 */
L8c65708e: *stop=0x8c65708eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c657090; /* mov.l @r15+,r12 */
L8c657090: *stop=0x8c657090u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c657092; /* mov.l @r15+,r13 */
L8c657092: *stop=0x8c657092u;if(*pr!=entry_pr)return -3;*stop=0x8c657094u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c657096(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c657096;
L8c657096: *stop=0x8c657096u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c657098; /* mov.l r14,@-r15 */
L8c657098: *stop=0x8c657098u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65709a; /* mov.l r13,@-r15 */
L8c65709a: *stop=0x8c65709au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65709c; /* mov.l r12,@-r15 */
L8c65709c: *stop=0x8c65709cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65709e; /* mov.l r11,@-r15 */
L8c65709e: *stop=0x8c65709eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6570a0; /* mov.l r10,@-r15 */
L8c6570a0: *stop=0x8c6570a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6570a2; /* mov.l r9,@-r15 */
L8c6570a2: *stop=0x8c6570a2u;r[15]+=4294967280u;goto L8c6570a4; /* add #-0x10,r15 */
L8c6570a4: *stop=0x8c6570a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65712cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6570a6; /* mov.l 0x8c65712c,r3 */
L8c6570a6: *stop=0x8c6570a6u;r[2]=r[15];goto L8c6570a8; /* mov r15,r2 */
L8c6570a8: *stop=0x8c6570a8u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6570aa; /* mov.l @(0x4,r3),r0 */
L8c6570aa: *stop=0x8c6570aau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6570ac; /* mov.l @r3,r1 */
L8c6570ac: *stop=0x8c6570acu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6570ae; /* mov.l r1,@r2 */
L8c6570ae: *stop=0x8c6570aeu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c6570b0; /* mov.l r0,@(0x4,r2) */
L8c6570b0: *stop=0x8c6570b0u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6570b2; /* mov.l @(0xc,r3),r0 */
L8c6570b2: *stop=0x8c6570b2u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6570b4; /* mov.l @(0x8,r3),r1 */
L8c6570b4: *stop=0x8c6570b4u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c6570b6; /* mov.l r1,@(0x8,r2) */
L8c6570b6: *stop=0x8c6570b6u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6570b8; /* mov.l r0,@(0xc,r2) */
L8c6570b8: *stop=0x8c6570b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657130u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6570ba; /* mov.l 0x8c657130,r2 */
L8c6570ba: *stop=0x8c6570bau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[9]=loaded;}goto L8c6570bc; /* mov.l @r2,r9 */
L8c6570bc: *stop=0x8c6570bcu;r[12]=0u;goto L8c6570be; /* mov #0x0,r12 */
L8c6570be: *stop=0x8c6570beu;r[11]=r[12];goto L8c6570c0; /* mov r12,r11 */
L8c6570c0: *stop=0x8c6570c0u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c6570c2; /* tst r5,r5 */
L8c6570c2: *stop=0x8c6570c2u;r[14]=r[12];goto L8c6570c4; /* mov r12,r14 */
L8c6570c4: *stop=0x8c6570c4u;r[9]=r[9]&255u;goto L8c6570c6; /* extu.b r9,r9 */
L8c6570c6: *stop=0x8c6570c6u;r[10]=r[12];goto L8c6570c8; /* mov r12,r10 */
L8c6570c8: take=(*sr&1u);*stop=0x8c6570cau;r[13]=r[12];if(take)goto L8c6570ce;goto L8c6570cc; /* bt/s 0x8c6570ce */
L8c6570cc: *stop=0x8c6570ccu;r[11]=1u;goto L8c6570ce; /* mov #0x1,r11 */
L8c6570ce: *stop=0x8c6570ceu;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c6570d0; /* tst r7,r7 */
L8c6570d0: take=(*sr&1u);*stop=0x8c6570d2u;r[5]=2u;if(take)goto L8c6570d6;goto L8c6570d4; /* bt/s 0x8c6570d6 */
L8c6570d4: *stop=0x8c6570d4u;r[10]=1u;goto L8c6570d6; /* mov #0x1,r10 */
L8c6570d6: *stop=0x8c6570d6u;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6570d8; /* mov.l @(0x28,r15),r2 */
L8c6570d8: *stop=0x8c6570d8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6570da; /* tst r2,r2 */
L8c6570da: take=(*sr&1u);*stop=0x8c6570dcu;r[7]=3u;if(take)goto L8c6570e0;goto L8c6570de; /* bt/s 0x8c6570e0 */
L8c6570de: *stop=0x8c6570deu;r[12]=r[5];goto L8c6570e0; /* mov r5,r12 */
L8c6570e0: *stop=0x8c6570e0u;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6570e2; /* mov.l @(0x2c,r15),r0 */
L8c6570e2: *stop=0x8c6570e2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6570e4; /* cmp/eq #0x1,r0 */
L8c6570e4: if(!(*sr&1u))goto L8c6570f4;goto L8c6570e6; /* bf 0x8c6570f4 */
L8c6570e6: *stop=0x8c6570e6u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6570e8; /* tst r4,r4 */
L8c6570e8: if((*sr&1u))goto L8c6570ec;goto L8c6570ea; /* bt 0x8c6570ec */
L8c6570ea: *stop=0x8c6570eau;r[13]=r[5];goto L8c6570ec; /* mov r5,r13 */
L8c6570ec: *stop=0x8c6570ecu;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c6570ee; /* tst r6,r6 */
L8c6570ee: if((*sr&1u))goto L8c657142;goto L8c6570f0; /* bt 0x8c657142 */
L8c6570f0: *stop=0x8c6570f2u;r[14]=r[5];goto L8c657142; /* bra 0x8c657142 */
L8c6570f4: *stop=0x8c6570f4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6570f6; /* tst r4,r4 */
L8c6570f6: if((*sr&1u))goto L8c6570fa;goto L8c6570f8; /* bt 0x8c6570fa */
L8c6570f8: *stop=0x8c6570f8u;r[13]=r[7];goto L8c6570fa; /* mov r7,r13 */
L8c6570fa: *stop=0x8c6570fau;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c6570fc; /* tst r6,r6 */
L8c6570fc: if((*sr&1u))goto L8c657100;goto L8c6570fe; /* bt 0x8c657100 */
L8c6570fe: *stop=0x8c6570feu;r[14]=r[7];goto L8c657100; /* mov r7,r14 */
L8c657100: *stop=0x8c657100u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c657102; /* tst r4,r4 */
L8c657102: if((*sr&1u))goto L8c657142;goto L8c657104; /* bt 0x8c657142 */
L8c657104: *stop=0x8c657104u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c657106; /* tst r6,r6 */
L8c657106: if((*sr&1u))goto L8c657142;goto L8c657108; /* bt 0x8c657142 */
L8c657108: *stop=0x8c657108u;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c65710a; /* cmp/hs r6,r4 */
L8c65710a: if((*sr&1u))goto L8c657140;goto L8c65710c; /* bt 0x8c657140 */
L8c65710c: *stop=0x8c65710eu;r[13]=r[5];goto L8c657142; /* bra 0x8c657142 */
L8c657140: *stop=0x8c657140u;r[14]=r[5];goto L8c657142; /* mov r5,r14 */
L8c657142: *stop=0x8c657142u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657200u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657144; /* mov.l 0x8c657200,r3 */
L8c657144: *stop=0x8c657144u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657146; /* mov.l @r3,r0 */
L8c657146: *stop=0x8c657146u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c657148; /* cmp/eq #0x1,r0 */
L8c657148: if(!(*sr&1u))goto L8c65717c;goto L8c65714a; /* bf 0x8c65717c */
L8c65714a: *stop=0x8c65714au;{uint32_t loaded=rd(m,bus,&io_status,0x8c657204u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65714c; /* mov.l 0x8c657204,r1 */
L8c65714c: *stop=0x8c65714cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65714e; /* mov.l @r1,r0 */
L8c65714e: *stop=0x8c65714eu;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c657150; /* cmp/eq #0x20,r0 */
L8c657150: if(!(*sr&1u))goto L8c657154;goto L8c657152; /* bf 0x8c657154 */
L8c657152: *stop=0x8c657152u;r[13]=r[7];goto L8c657154; /* mov r7,r13 */
L8c657154: *stop=0x8c657154u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657204u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657156; /* mov.l 0x8c657204,r3 */
L8c657156: *stop=0x8c657156u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657158; /* mov.l @r3,r0 */
L8c657158: *stop=0x8c657158u;*sr=(*sr&~1u)|(r[0]==0x00000021u);goto L8c65715a; /* cmp/eq #0x21,r0 */
L8c65715a: if(!(*sr&1u))goto L8c65715e;goto L8c65715c; /* bf 0x8c65715e */
L8c65715c: *stop=0x8c65715cu;r[11]=r[5];goto L8c65715e; /* mov r5,r11 */
L8c65715e: *stop=0x8c65715eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657204u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657160; /* mov.l 0x8c657204,r3 */
L8c657160: *stop=0x8c657160u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657162; /* mov.l @r3,r0 */
L8c657162: *stop=0x8c657162u;*sr=(*sr&~1u)|(r[0]==0x00000022u);goto L8c657164; /* cmp/eq #0x22,r0 */
L8c657164: if(!(*sr&1u))goto L8c657168;goto L8c657166; /* bf 0x8c657168 */
L8c657166: *stop=0x8c657166u;r[14]=r[7];goto L8c657168; /* mov r7,r14 */
L8c657168: *stop=0x8c657168u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657204u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65716a; /* mov.l 0x8c657204,r3 */
L8c65716a: *stop=0x8c65716au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65716c; /* mov.l @r3,r0 */
L8c65716c: *stop=0x8c65716cu;*sr=(*sr&~1u)|(r[0]==0x00000023u);goto L8c65716e; /* cmp/eq #0x23,r0 */
L8c65716e: if(!(*sr&1u))goto L8c657172;goto L8c657170; /* bf 0x8c657172 */
L8c657170: *stop=0x8c657170u;r[10]=r[5];goto L8c657172; /* mov r5,r10 */
L8c657172: *stop=0x8c657172u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657204u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657174; /* mov.l 0x8c657204,r3 */
L8c657174: *stop=0x8c657174u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657176; /* mov.l @r3,r0 */
L8c657176: *stop=0x8c657176u;*sr=(*sr&~1u)|(r[0]==0x00000024u);goto L8c657178; /* cmp/eq #0x24,r0 */
L8c657178: if(!(*sr&1u))goto L8c65717c;goto L8c65717a; /* bf 0x8c65717c */
L8c65717a: *stop=0x8c65717au;r[12]=r[7];goto L8c65717c; /* mov r7,r12 */
L8c65717c: *stop=0x8c65717cu;r[2]=r[11];goto L8c65717e; /* mov r11,r2 */
L8c65717e: *stop=0x8c65717eu;r[2]<<=16;goto L8c657180; /* shll16 r2 */
L8c657180: *stop=0x8c657180u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65720cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657182; /* mov.l 0x8c65720c,r1 */
L8c657182: *stop=0x8c657182u;r[2]<<=2;goto L8c657184; /* shll2 r2 */
L8c657184: *stop=0x8c657184u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657208u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657186; /* mov.l 0x8c657208,r3 */
L8c657186: *stop=0x8c657186u;r[13]<<=16;goto L8c657188; /* shll16 r13 */
L8c657188: *stop=0x8c657188u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c65718a; /* shll r13 */
L8c65718a: *stop=0x8c65718au;r[0]=r[14];goto L8c65718c; /* mov r14,r0 */
L8c65718c: *stop=0x8c65718cu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c65718e; /* shll r2 */
L8c65718e: *stop=0x8c65718eu;r[2]&=r[1];goto L8c657190; /* and r1,r2 */
L8c657190: *stop=0x8c657190u;r[13]&=r[3];goto L8c657192; /* and r3,r13 */
L8c657192: *stop=0x8c657192u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657210u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657194; /* mov.l 0x8c657210,r3 */
L8c657194: *stop=0x8c657194u;r[13]|=r[2];goto L8c657196; /* or r2,r13 */
L8c657196: *stop=0x8c657196u;r[2]=21u;goto L8c657198; /* mov #0x15,r2 */
L8c657198: *stop=0x8c657198u;r[0]=(r[2]&0x80000000u)?((r[2]&31u)?r[0]>>((0u-r[2])&31u):0u):(r[0]<<(r[2]&31u));goto L8c65719a; /* shld r2,r0 */
L8c65719a: *stop=0x8c65719au;r[2]=23u;goto L8c65719c; /* mov #0x17,r2 */
L8c65719c: *stop=0x8c65719cu;r[10]=(r[2]&0x80000000u)?((r[2]&31u)?r[10]>>((0u-r[2])&31u):0u):(r[10]<<(r[2]&31u));goto L8c65719e; /* shld r2,r10 */
L8c65719e: *stop=0x8c65719eu;r[2]=r[12];goto L8c6571a0; /* mov r12,r2 */
L8c6571a0: *stop=0x8c6571a0u;r[0]&=r[3];goto L8c6571a2; /* and r3,r0 */
L8c6571a2: *stop=0x8c6571a2u;r[2]<<=16;goto L8c6571a4; /* shll16 r2 */
L8c6571a4: *stop=0x8c6571a4u;r[13]|=r[0];goto L8c6571a6; /* or r0,r13 */
L8c6571a6: *stop=0x8c6571a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657214u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6571a8; /* mov.l 0x8c657214,r0 */
L8c6571a8: *stop=0x8c6571a8u;r[2]<<=8;goto L8c6571aa; /* shll8 r2 */
L8c6571aa: *stop=0x8c6571aau;r[3]<<=2;goto L8c6571ac; /* shll2 r3 */
L8c6571ac: *stop=0x8c6571acu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6571ae; /* shll r2 */
L8c6571ae: *stop=0x8c6571aeu;r[10]&=r[3];goto L8c6571b0; /* and r3,r10 */
L8c6571b0: *stop=0x8c6571b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657218u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6571b2; /* mov.l 0x8c657218,r3 */
L8c6571b2: *stop=0x8c6571b2u;r[2]&=r[0];goto L8c6571b4; /* and r0,r2 */
L8c6571b4: *stop=0x8c6571b4u;r[13]|=r[10];goto L8c6571b6; /* or r10,r13 */
L8c6571b6: *stop=0x8c6571b6u;r[13]|=r[2];goto L8c6571b8; /* or r2,r13 */
L8c6571b8: *stop=0x8c6571b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65721cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6571ba; /* mov.l 0x8c65721c,r2 */
L8c6571ba: *stop=0x8c6571bau;r[13]|=r[3];goto L8c6571bc; /* or r3,r13 */
L8c6571bc: *stop=0x8c6571bcu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6571be; /* mov.l @r2,r0 */
L8c6571be: *stop=0x8c6571beu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c6571c0; /* tst #0x20,r0 */
L8c6571c0: take=(*sr&1u);*stop=0x8c6571c2u;r[9]|=r[13];if(take)goto L8c6571da;goto L8c6571c4; /* bt/s 0x8c6571da */
L8c6571c4: *stop=0x8c6571c4u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c6571c6; /* tst r11,r11 */
L8c6571c6: if((*sr&1u))goto L8c6571d4;goto L8c6571c8; /* bt 0x8c6571d4 */
L8c6571c8: *stop=0x8c6571c8u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6571ca; /* tst r14,r14 */
L8c6571ca: if((*sr&1u))goto L8c6571d4;goto L8c6571cc; /* bt 0x8c6571d4 */
L8c6571cc: *stop=0x8c6571ccu;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c6571ce; /* tst r12,r12 */
L8c6571ce: if((*sr&1u))goto L8c6571d4;goto L8c6571d0; /* bt 0x8c6571d4 */
L8c6571d0: *stop=0x8c6571d2u;r[3]=1u;goto L8c6571d6; /* bra 0x8c6571d6 */
L8c6571d4: *stop=0x8c6571d4u;r[3]=0u;goto L8c6571d6; /* mov #0x0,r3 */
L8c6571d6: *stop=0x8c6571d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657220u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6571d8; /* mov.l 0x8c657220,r2 */
L8c6571d8: *stop=0x8c6571d8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6571da; /* mov.l r3,@r2 */
L8c6571da: *stop=0x8c6571dau;r[0]=r[9];goto L8c6571dc; /* mov r9,r0 */
L8c6571dc: *stop=0x8c6571dcu;r[15]+=16u;goto L8c6571de; /* add #0x10,r15 */
L8c6571de: *stop=0x8c6571deu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6571e0; /* mov.l @r15+,r9 */
L8c6571e0: *stop=0x8c6571e0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6571e2; /* mov.l @r15+,r10 */
L8c6571e2: *stop=0x8c6571e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6571e4; /* mov.l @r15+,r11 */
L8c6571e4: *stop=0x8c6571e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6571e6; /* mov.l @r15+,r12 */
L8c6571e6: *stop=0x8c6571e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6571e8; /* mov.l @r15+,r13 */
L8c6571e8: *stop=0x8c6571e8u;if(*pr!=entry_pr)return -3;*stop=0x8c6571eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c657240(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c657240;
L8c657240: *stop=0x8c657240u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c657242; /* mov.l r14,@-r15 */
L8c657242: *stop=0x8c657242u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c657244; /* mov.l r13,@-r15 */
L8c657244: *stop=0x8c657244u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c657246; /* mov.l r12,@-r15 */
L8c657246: *stop=0x8c657246u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c657248; /* mov.l r11,@-r15 */
L8c657248: *stop=0x8c657248u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65724a; /* mov.l r10,@-r15 */
L8c65724a: *stop=0x8c65724au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65724c; /* mov.l r9,@-r15 */
L8c65724c: *stop=0x8c65724cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65724e; /* mov.l r8,@-r15 */
L8c65724e: *stop=0x8c65724eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c657250; /* sts.l PR,@-r15 */
L8c657250: *stop=0x8c657250u;r[15]+=4294967260u;goto L8c657252; /* add #-0x24,r15 */
L8c657252: *stop=0x8c657252u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657440u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657254; /* mov.l 0x8c657440,r3 */
L8c657254: *stop=0x8c657254u;r[2]=r[15];goto L8c657256; /* mov r15,r2 */
L8c657256: *stop=0x8c657256u;r[2]+=4u;goto L8c657258; /* add #0x4,r2 */
L8c657258: *stop=0x8c657258u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657448u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c65725a; /* mov.l 0x8c657448,r12 */
L8c65725a: *stop=0x8c65725au;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65725c; /* mov.l @(0x4,r3),r0 */
L8c65725c: *stop=0x8c65725cu;r[14]=r[15];goto L8c65725e; /* mov r15,r14 */
L8c65725e: *stop=0x8c65725eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657260; /* mov.l @r3,r1 */
L8c657260: *stop=0x8c657260u;r[14]+=4u;goto L8c657262; /* add #0x4,r14 */
L8c657262: *stop=0x8c657262u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657444u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c657264; /* mov.l 0x8c657444,r10 */
L8c657264: *stop=0x8c657264u;r[13]=r[4];goto L8c657266; /* mov r4,r13 */
L8c657266: *stop=0x8c657266u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c657268; /* mov.l r1,@r2 */
L8c657268: *stop=0x8c657268u;r[8]=0u;goto L8c65726a; /* mov #0x0,r8 */
L8c65726a: *stop=0x8c65726au;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c65726c; /* mov.l r0,@(0x4,r2) */
L8c65726c: *stop=0x8c65726cu;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65726e; /* mov.l @(0xc,r3),r0 */
L8c65726e: *stop=0x8c65726eu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657270; /* mov.l @(0x8,r3),r1 */
L8c657270: *stop=0x8c657270u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c657272; /* mov.l r1,@(0x8,r2) */
L8c657272: *stop=0x8c657272u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[0],4);if(io_status)return io_status;goto L8c657274; /* mov.l r0,@(0xc,r2) */
L8c657274: *stop=0x8c657274u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657276; /* mov.l @(0x10,r3),r1 */
L8c657276: *stop=0x8c657276u;if(!valid_access(r[3]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657278; /* mov.l @(0x14,r3),r0 */
L8c657278: *stop=0x8c657278u;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[1],4);if(io_status)return io_status;goto L8c65727a; /* mov.l r1,@(0x10,r2) */
L8c65727a: *stop=0x8c65727au;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[0],4);if(io_status)return io_status;goto L8c65727c; /* mov.l r0,@(0x14,r2) */
L8c65727c: *stop=0x8c65727cu;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65727e; /* mov.l @(0x1c,r3),r0 */
L8c65727e: *stop=0x8c65727eu;if(!valid_access(r[3]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657280; /* mov.l @(0x18,r3),r1 */
L8c657280: *stop=0x8c657280u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[1],4);if(io_status)return io_status;goto L8c657282; /* mov.l r1,@(0x18,r2) */
L8c657282: *stop=0x8c657282u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c657284; /* mov.l r0,@(0x1c,r2) */
L8c657284: *stop=0x8c657284u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65744cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657286; /* mov.l 0x8c65744c,r2 */
L8c657286: *stop=0x8c657286u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657288; /* mov.l @r2,r0 */
L8c657288: *stop=0x8c657288u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c65728a; /* tst #0x1,r0 */
L8c65728a: take=(*sr&1u);*stop=0x8c65728cu;r[9]=8u;if(take)goto L8c6572be;goto L8c65728e; /* bt/s 0x8c6572be */
L8c65728e: *stop=0x8c65728eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657450u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657290; /* mov.l 0x8c657450,r0 */
L8c657290: *stop=0x8c657290u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657292; /* mov.l @r0,r1 */
L8c657292: *stop=0x8c657292u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c657294; /* tst r1,r1 */
L8c657294: if((*sr&1u))goto L8c65729e;goto L8c657296; /* bt 0x8c65729e */
L8c657296: *stop=0x8c657296u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657454u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657298; /* mov.l 0x8c657454,r3 */
L8c657298: *stop=0x8c657298u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65729a; /* mov.l @r3,r0 */
L8c65729a: *stop=0x8c65729au;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c65729c; /* cmp/eq #0x20,r0 */
L8c65729c: if((*sr&1u))goto L8c6572be;goto L8c65729e; /* bt 0x8c6572be */
L8c65729e: *stop=0x8c65729eu;r[3]=0u;goto L8c6572a0; /* mov #0x0,r3 */
L8c6572a0: *stop=0x8c6572a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657458u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6572a2; /* mov.l 0x8c657458,r11 */
L8c6572a2: *stop=0x8c6572a2u;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c6572a4; /* cmp/hs r13,r3 */
L8c6572a4: take=(*sr&1u);*stop=0x8c6572a6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;if(take)goto L8c6572bc;goto L8c6572a8; /* bt/s 0x8c6572bc */
L8c6572a8: *stop=0x8c6572a8u;r[5]=r[14];goto L8c6572aa; /* mov r14,r5 */
L8c6572aa: *stop=0x8c6572aau;r[6]=32u;goto L8c6572ac; /* mov #0x20,r6 */
L8c6572ac: target=r[12];*pr=0x8c6572b0u;*stop=0x8c6572aeu;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6572b0; /* jsr @r12 */
L8c6572b0: *stop=0x8c6572b0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6572b2; /* mov.l @r15,r3 */
L8c6572b2: *stop=0x8c6572b2u;r[3]+=1u;goto L8c6572b4; /* add #0x1,r3 */
L8c6572b4: *stop=0x8c6572b4u;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c6572b6; /* cmp/hs r13,r3 */
L8c6572b6: *stop=0x8c6572b6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6572b8; /* mov.l r3,@r15 */
L8c6572b8: take=!(*sr&1u);*stop=0x8c6572bau;r[11]+=20u;if(take)goto L8c6572a8;goto L8c6572bc; /* bf/s 0x8c6572a8 */
L8c6572bc: *stop=0x8c6572bcu;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[9],4);if(io_status)return io_status;goto L8c6572be; /* mov.l r9,@r10 */
L8c6572be: *stop=0x8c6572beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65744cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6572c0; /* mov.l 0x8c65744c,r3 */
L8c6572c0: *stop=0x8c6572c0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6572c2; /* mov.l @r3,r0 */
L8c6572c2: *stop=0x8c6572c2u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c6572c4; /* tst #0x2,r0 */
L8c6572c4: if((*sr&1u))goto L8c6572fa;goto L8c6572c6; /* bt 0x8c6572fa */
L8c6572c6: *stop=0x8c6572c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65745cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6572c8; /* mov.l 0x8c65745c,r1 */
L8c6572c8: *stop=0x8c6572c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657450u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6572ca; /* mov.l 0x8c657450,r0 */
L8c6572ca: *stop=0x8c6572cau;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c6572cc; /* mov.l r1,@r14 */
L8c6572cc: *stop=0x8c6572ccu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6572ce; /* mov.l @r0,r2 */
L8c6572ce: *stop=0x8c6572ceu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6572d0; /* tst r2,r2 */
L8c6572d0: if((*sr&1u))goto L8c6572da;goto L8c6572d2; /* bt 0x8c6572da */
L8c6572d2: *stop=0x8c6572d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657454u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6572d4; /* mov.l 0x8c657454,r2 */
L8c6572d4: *stop=0x8c6572d4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6572d6; /* mov.l @r2,r0 */
L8c6572d6: *stop=0x8c6572d6u;*sr=(*sr&~1u)|(r[0]==0x00000021u);goto L8c6572d8; /* cmp/eq #0x21,r0 */
L8c6572d8: if((*sr&1u))goto L8c6572fa;goto L8c6572da; /* bt 0x8c6572fa */
L8c6572da: *stop=0x8c6572dau;r[3]=0u;goto L8c6572dc; /* mov #0x0,r3 */
L8c6572dc: *stop=0x8c6572dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657458u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6572de; /* mov.l 0x8c657458,r11 */
L8c6572de: *stop=0x8c6572deu;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c6572e0; /* cmp/hs r13,r3 */
L8c6572e0: take=(*sr&1u);*stop=0x8c6572e2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;if(take)goto L8c6572f8;goto L8c6572e4; /* bt/s 0x8c6572f8 */
L8c6572e4: *stop=0x8c6572e4u;r[5]=r[14];goto L8c6572e6; /* mov r14,r5 */
L8c6572e6: *stop=0x8c6572e6u;r[6]=32u;goto L8c6572e8; /* mov #0x20,r6 */
L8c6572e8: target=r[12];*pr=0x8c6572ecu;*stop=0x8c6572eau;if(!valid_access(r[11]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6572ec; /* jsr @r12 */
L8c6572ec: *stop=0x8c6572ecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6572ee; /* mov.l @r15,r3 */
L8c6572ee: *stop=0x8c6572eeu;r[3]+=1u;goto L8c6572f0; /* add #0x1,r3 */
L8c6572f0: *stop=0x8c6572f0u;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c6572f2; /* cmp/hs r13,r3 */
L8c6572f2: *stop=0x8c6572f2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6572f4; /* mov.l r3,@r15 */
L8c6572f4: take=!(*sr&1u);*stop=0x8c6572f6u;r[11]+=20u;if(take)goto L8c6572e4;goto L8c6572f8; /* bf/s 0x8c6572e4 */
L8c6572f8: *stop=0x8c6572f8u;if(!valid_access(r[10]+4u,4u))return -3;wr(m,bus,&io_status,r[10]+0x4u,r[9],4);if(io_status)return io_status;goto L8c6572fa; /* mov.l r9,@(0x4,r10) */
L8c6572fa: *stop=0x8c6572fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c65744cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6572fc; /* mov.l 0x8c65744c,r3 */
L8c6572fc: *stop=0x8c6572fcu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6572fe; /* mov.l @r3,r0 */
L8c6572fe: *stop=0x8c6572feu;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c657300; /* tst #0x4,r0 */
L8c657300: if((*sr&1u))goto L8c657338;goto L8c657302; /* bt 0x8c657338 */
L8c657302: *stop=0x8c657302u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657460u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657304; /* mov.l 0x8c657460,r1 */
L8c657304: *stop=0x8c657304u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657450u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657306; /* mov.l 0x8c657450,r0 */
L8c657306: *stop=0x8c657306u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c657308; /* mov.l r1,@r14 */
L8c657308: *stop=0x8c657308u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65730a; /* mov.l @r0,r2 */
L8c65730a: *stop=0x8c65730au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65730c; /* tst r2,r2 */
L8c65730c: if((*sr&1u))goto L8c657316;goto L8c65730e; /* bt 0x8c657316 */
L8c65730e: *stop=0x8c65730eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657454u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657310; /* mov.l 0x8c657454,r2 */
L8c657310: *stop=0x8c657310u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657312; /* mov.l @r2,r0 */
L8c657312: *stop=0x8c657312u;*sr=(*sr&~1u)|(r[0]==0x00000022u);goto L8c657314; /* cmp/eq #0x22,r0 */
L8c657314: if((*sr&1u))goto L8c657338;goto L8c657316; /* bt 0x8c657338 */
L8c657316: *stop=0x8c657316u;r[3]=0u;goto L8c657318; /* mov #0x0,r3 */
L8c657318: *stop=0x8c657318u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657458u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c65731a; /* mov.l 0x8c657458,r11 */
L8c65731a: *stop=0x8c65731au;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c65731c; /* cmp/hs r13,r3 */
L8c65731c: take=(*sr&1u);*stop=0x8c65731eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;if(take)goto L8c657334;goto L8c657320; /* bt/s 0x8c657334 */
L8c657320: *stop=0x8c657320u;r[5]=r[14];goto L8c657322; /* mov r14,r5 */
L8c657322: *stop=0x8c657322u;r[6]=32u;goto L8c657324; /* mov #0x20,r6 */
L8c657324: target=r[12];*pr=0x8c657328u;*stop=0x8c657326u;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c657328; /* jsr @r12 */
L8c657328: *stop=0x8c657328u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65732a; /* mov.l @r15,r3 */
L8c65732a: *stop=0x8c65732au;r[3]+=1u;goto L8c65732c; /* add #0x1,r3 */
L8c65732c: *stop=0x8c65732cu;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c65732e; /* cmp/hs r13,r3 */
L8c65732e: *stop=0x8c65732eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c657330; /* mov.l r3,@r15 */
L8c657330: take=!(*sr&1u);*stop=0x8c657332u;r[11]+=20u;if(take)goto L8c657320;goto L8c657334; /* bf/s 0x8c657320 */
L8c657334: *stop=0x8c657334u;r[3]=8u;goto L8c657336; /* mov #0x8,r3 */
L8c657336: *stop=0x8c657336u;if(!valid_access(r[10]+8u,4u))return -3;wr(m,bus,&io_status,r[10]+0x8u,r[3],4);if(io_status)return io_status;goto L8c657338; /* mov.l r3,@(0x8,r10) */
L8c657338: *stop=0x8c657338u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65744cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65733a; /* mov.l 0x8c65744c,r1 */
L8c65733a: *stop=0x8c65733au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65733c; /* mov.l @r1,r2 */
L8c65733c: *stop=0x8c65733cu;*sr=(*sr&~1u)|!(r[9]&r[2]);goto L8c65733e; /* tst r9,r2 */
L8c65733e: if((*sr&1u))goto L8c657374;goto L8c657340; /* bt 0x8c657374 */
L8c657340: *stop=0x8c657340u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657464u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657342; /* mov.l 0x8c657464,r0 */
L8c657342: *stop=0x8c657342u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657450u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657344; /* mov.l 0x8c657450,r2 */
L8c657344: *stop=0x8c657344u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c657346; /* mov.l r0,@r14 */
L8c657346: *stop=0x8c657346u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657348; /* mov.l @r2,r3 */
L8c657348: *stop=0x8c657348u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65734a; /* tst r3,r3 */
L8c65734a: if((*sr&1u))goto L8c657354;goto L8c65734c; /* bt 0x8c657354 */
L8c65734c: *stop=0x8c65734cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657454u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65734e; /* mov.l 0x8c657454,r3 */
L8c65734e: *stop=0x8c65734eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657350; /* mov.l @r3,r0 */
L8c657350: *stop=0x8c657350u;*sr=(*sr&~1u)|(r[0]==0x00000023u);goto L8c657352; /* cmp/eq #0x23,r0 */
L8c657352: if((*sr&1u))goto L8c657374;goto L8c657354; /* bt 0x8c657374 */
L8c657354: *stop=0x8c657354u;r[3]=0u;goto L8c657356; /* mov #0x0,r3 */
L8c657356: *stop=0x8c657356u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657458u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c657358; /* mov.l 0x8c657458,r11 */
L8c657358: *stop=0x8c657358u;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c65735a; /* cmp/hs r13,r3 */
L8c65735a: take=(*sr&1u);*stop=0x8c65735cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;if(take)goto L8c657372;goto L8c65735e; /* bt/s 0x8c657372 */
L8c65735e: *stop=0x8c65735eu;r[5]=r[14];goto L8c657360; /* mov r14,r5 */
L8c657360: *stop=0x8c657360u;r[6]=32u;goto L8c657362; /* mov #0x20,r6 */
L8c657362: target=r[12];*pr=0x8c657366u;*stop=0x8c657364u;if(!valid_access(r[11]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c657366; /* jsr @r12 */
L8c657366: *stop=0x8c657366u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657368; /* mov.l @r15,r3 */
L8c657368: *stop=0x8c657368u;r[3]+=1u;goto L8c65736a; /* add #0x1,r3 */
L8c65736a: *stop=0x8c65736au;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c65736c; /* cmp/hs r13,r3 */
L8c65736c: *stop=0x8c65736cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65736e; /* mov.l r3,@r15 */
L8c65736e: take=!(*sr&1u);*stop=0x8c657370u;r[11]+=20u;if(take)goto L8c65735e;goto L8c657372; /* bf/s 0x8c65735e */
L8c657372: *stop=0x8c657372u;if(!valid_access(r[10]+12u,4u))return -3;wr(m,bus,&io_status,r[10]+0xcu,r[9],4);if(io_status)return io_status;goto L8c657374; /* mov.l r9,@(0xc,r10) */
L8c657374: *stop=0x8c657374u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65744cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657376; /* mov.l 0x8c65744c,r3 */
L8c657376: *stop=0x8c657376u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657378; /* mov.l @r3,r0 */
L8c657378: *stop=0x8c657378u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c65737a; /* tst #0x10,r0 */
L8c65737a: if((*sr&1u))goto L8c6573b0;goto L8c65737c; /* bt 0x8c6573b0 */
L8c65737c: *stop=0x8c65737cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657468u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65737e; /* mov.l 0x8c657468,r1 */
L8c65737e: *stop=0x8c65737eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657450u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657380; /* mov.l 0x8c657450,r0 */
L8c657380: *stop=0x8c657380u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c657382; /* mov.l r1,@r14 */
L8c657382: *stop=0x8c657382u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657384; /* mov.l @r0,r2 */
L8c657384: *stop=0x8c657384u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c657386; /* tst r2,r2 */
L8c657386: if((*sr&1u))goto L8c657390;goto L8c657388; /* bt 0x8c657390 */
L8c657388: *stop=0x8c657388u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657454u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65738a; /* mov.l 0x8c657454,r2 */
L8c65738a: *stop=0x8c65738au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65738c; /* mov.l @r2,r0 */
L8c65738c: *stop=0x8c65738cu;*sr=(*sr&~1u)|(r[0]==0x00000024u);goto L8c65738e; /* cmp/eq #0x24,r0 */
L8c65738e: if((*sr&1u))goto L8c6573b0;goto L8c657390; /* bt 0x8c6573b0 */
L8c657390: *stop=0x8c657390u;r[3]=0u;goto L8c657392; /* mov #0x0,r3 */
L8c657392: *stop=0x8c657392u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657458u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c657394; /* mov.l 0x8c657458,r11 */
L8c657394: *stop=0x8c657394u;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c657396; /* cmp/hs r13,r3 */
L8c657396: take=(*sr&1u);*stop=0x8c657398u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;if(take)goto L8c6573ae;goto L8c65739a; /* bt/s 0x8c6573ae */
L8c65739a: *stop=0x8c65739au;r[5]=r[14];goto L8c65739c; /* mov r14,r5 */
L8c65739c: *stop=0x8c65739cu;r[6]=32u;goto L8c65739e; /* mov #0x20,r6 */
L8c65739e: target=r[12];*pr=0x8c6573a2u;*stop=0x8c6573a0u;if(!valid_access(r[11]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6573a2; /* jsr @r12 */
L8c6573a2: *stop=0x8c6573a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6573a4; /* mov.l @r15,r3 */
L8c6573a4: *stop=0x8c6573a4u;r[3]+=1u;goto L8c6573a6; /* add #0x1,r3 */
L8c6573a6: *stop=0x8c6573a6u;*sr=(*sr&~1u)|(r[3]>=r[13]);goto L8c6573a8; /* cmp/hs r13,r3 */
L8c6573a8: *stop=0x8c6573a8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6573aa; /* mov.l r3,@r15 */
L8c6573aa: take=!(*sr&1u);*stop=0x8c6573acu;r[11]+=20u;if(take)goto L8c65739a;goto L8c6573ae; /* bf/s 0x8c65739a */
L8c6573ae: *stop=0x8c6573aeu;if(!valid_access(r[10]+16u,4u))return -3;wr(m,bus,&io_status,r[10]+0x10u,r[9],4);if(io_status)return io_status;goto L8c6573b0; /* mov.l r9,@(0x10,r10) */
L8c6573b0: *stop=0x8c6573b0u;r[15]+=36u;goto L8c6573b2; /* add #0x24,r15 */
L8c6573b2: *stop=0x8c6573b2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6573b4; /* lds.l @r15+,PR */
L8c6573b4: *stop=0x8c6573b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6573b6; /* mov.l @r15+,r8 */
L8c6573b6: *stop=0x8c6573b6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6573b8; /* mov.l @r15+,r9 */
L8c6573b8: *stop=0x8c6573b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6573ba; /* mov.l @r15+,r10 */
L8c6573ba: *stop=0x8c6573bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6573bc; /* mov.l @r15+,r11 */
L8c6573bc: *stop=0x8c6573bcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6573be; /* mov.l @r15+,r12 */
L8c6573be: *stop=0x8c6573beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6573c0; /* mov.l @r15+,r13 */
L8c6573c0: *stop=0x8c6573c0u;if(*pr!=entry_pr)return -3;*stop=0x8c6573c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c6573c4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6573c4;
L8c6573c4: *stop=0x8c6573c4u;r[15]+=4294967268u;goto L8c6573c6; /* add #-0x1c,r15 */
L8c6573c6: *stop=0x8c6573c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65746cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6573c8; /* mov.l 0x8c65746c,r3 */
L8c6573c8: *stop=0x8c6573c8u;r[2]=r[15];goto L8c6573ca; /* mov r15,r2 */
L8c6573ca: *stop=0x8c6573cau;r[2]+=12u;goto L8c6573cc; /* add #0xc,r2 */
L8c6573cc: *stop=0x8c6573ccu;r[7]=r[4];goto L8c6573ce; /* mov r4,r7 */
L8c6573ce: *stop=0x8c6573ceu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6573d0; /* mov.l @r3,r1 */
L8c6573d0: *stop=0x8c6573d0u;r[7]>>=16;goto L8c6573d2; /* shlr16 r7 */
L8c6573d2: *stop=0x8c6573d2u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6573d4; /* mov.l @(0x4,r3),r0 */
L8c6573d4: *stop=0x8c6573d4u;r[6]=3u;goto L8c6573d6; /* mov #0x3,r6 */
L8c6573d6: *stop=0x8c6573d6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6573d8; /* mov.l r1,@r2 */
L8c6573d8: *stop=0x8c6573d8u;r[7]>>=2;goto L8c6573da; /* shlr2 r7 */
L8c6573da: *stop=0x8c6573dau;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c6573dc; /* mov.l r0,@(0x4,r2) */
L8c6573dc: *stop=0x8c6573dcu;r[5]=r[15];goto L8c6573de; /* mov r15,r5 */
L8c6573de: *stop=0x8c6573deu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6573e0; /* mov.l @(0x8,r3),r1 */
L8c6573e0: *stop=0x8c6573e0u;r[5]+=12u;goto L8c6573e2; /* add #0xc,r5 */
L8c6573e2: *stop=0x8c6573e2u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6573e4; /* mov.l @(0xc,r3),r0 */
L8c6573e4: *stop=0x8c6573e4u;r[3]=r[4];goto L8c6573e6; /* mov r4,r3 */
L8c6573e6: *stop=0x8c6573e6u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c6573e8; /* mov.l r1,@(0x8,r2) */
L8c6573e8: *stop=0x8c6573e8u;r[1]=4294967275u;goto L8c6573ea; /* mov #-0x15,r1 */
L8c6573ea: *stop=0x8c6573eau;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6573ec; /* mov.l r0,@(0xc,r2) */
L8c6573ec: *stop=0x8c6573ecu;r[2]=r[4];goto L8c6573ee; /* mov r4,r2 */
L8c6573ee: *stop=0x8c6573eeu;r[0]=r[4];goto L8c6573f0; /* mov r4,r0 */
L8c6573f0: *stop=0x8c6573f0u;r[2]>>=16;goto L8c6573f2; /* shlr16 r2 */
L8c6573f2: *stop=0x8c6573f2u;r[0]=(r[1]&0x80000000u)?((r[1]&31u)?r[0]>>((0u-r[1])&31u):0u):(r[0]<<(r[1]&31u));goto L8c6573f4; /* shld r1,r0 */
L8c6573f4: *stop=0x8c6573f4u;*sr=(*sr&~1u)|(r[2]&1u);r[2]>>=1;goto L8c6573f6; /* shlr r2 */
L8c6573f6: *stop=0x8c6573f6u;r[0]&=r[6];goto L8c6573f8; /* and r6,r0 */
L8c6573f8: *stop=0x8c6573f8u;r[2]&=r[6];goto L8c6573fa; /* and r6,r2 */
L8c6573fa: *stop=0x8c6573fau;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c6573fc; /* shlr r7 */
L8c6573fc: *stop=0x8c6573fcu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6573fe; /* mov.l r2,@(0x4,r15) */
L8c6573fe: *stop=0x8c6573feu;r[1]=4294967273u;goto L8c657400; /* mov #-0x17,r1 */
L8c657400: *stop=0x8c657400u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c657402; /* mov.l r0,@r15 */
L8c657402: *stop=0x8c657402u;r[3]=(r[1]&0x80000000u)?((r[1]&31u)?r[3]>>((0u-r[1])&31u):0u):(r[3]<<(r[1]&31u));goto L8c657404; /* shld r1,r3 */
L8c657404: *stop=0x8c657404u;r[7]&=r[6];goto L8c657406; /* and r6,r7 */
L8c657406: *stop=0x8c657406u;r[4]>>=16;goto L8c657408; /* shlr16 r4 */
L8c657408: *stop=0x8c657408u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657470u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65740a; /* mov.l 0x8c657470,r0 */
L8c65740a: *stop=0x8c65740au;r[4]>>=8;goto L8c65740c; /* shlr8 r4 */
L8c65740c: *stop=0x8c65740cu;*sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1;goto L8c65740e; /* shlr r4 */
L8c65740e: *stop=0x8c65740eu;r[3]&=r[6];goto L8c657410; /* and r6,r3 */
L8c657410: *stop=0x8c657410u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c657412; /* mov.l r3,@(0x8,r15) */
L8c657412: *stop=0x8c657412u;r[6]&=r[4];goto L8c657414; /* and r4,r6 */
L8c657414: *stop=0x8c657414u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657416; /* mov.l @r0,r1 */
L8c657416: *stop=0x8c657416u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657474u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657418; /* mov.l 0x8c657474,r2 */
L8c657418: *stop=0x8c657418u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65741a; /* mov.l @r2,r0 */
L8c65741a: *stop=0x8c65741au;*macl=r[1]*r[0];goto L8c65741c; /* mul.l r1,r0 */
L8c65741c: *stop=0x8c65741cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657478u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65741e; /* mov.l 0x8c657478,r1 */
L8c65741e: *stop=0x8c65741eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657420; /* mov.l @r1,r0 */
L8c657420: *stop=0x8c657420u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c657422; /* tst #0x20,r0 */
L8c657422: take=(*sr&1u);*stop=0x8c657424u;r[4]=*macl;if(take)goto L8c657482;goto L8c657426; /* bt/s 0x8c657482 */
L8c657426: *stop=0x8c657426u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65747cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657428; /* mov.l 0x8c65747c,r0 */
L8c657428: *stop=0x8c657428u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65742a; /* mov.l @r0,r3 */
L8c65742a: *stop=0x8c65742au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65742c; /* tst r3,r3 */
L8c65742c: if((*sr&1u))goto L8c657480;goto L8c65742e; /* bt 0x8c657480 */
L8c65742e: *stop=0x8c657430u;r[4]<<=2;goto L8c657482; /* bra 0x8c657482 */
L8c657480: *stop=0x8c657480u;r[4]+=r[4];goto L8c657482; /* add r4,r4 */
L8c657482: *stop=0x8c657482u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657484; /* mov.l @(0x4,r15),r0 */
L8c657484: *stop=0x8c657484u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657600u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657486; /* mov.l 0x8c657600,r1 */
L8c657486: *stop=0x8c657486u;r[0]<<=2;goto L8c657488; /* shll2 r0 */
L8c657488: *stop=0x8c657488u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65748a; /* mov.l @(r0,r5),r3 */
L8c65748a: *stop=0x8c65748au;r[0]=r[7];goto L8c65748c; /* mov r7,r0 */
L8c65748c: *stop=0x8c65748cu;r[0]<<=2;goto L8c65748e; /* shll2 r0 */
L8c65748e: *stop=0x8c65748eu;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657490; /* mov.l @(r0,r5),r2 */
L8c657490: *stop=0x8c657490u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657492; /* mov.l @r15,r0 */
L8c657492: *stop=0x8c657492u;r[3]+=r[2];goto L8c657494; /* add r2,r3 */
L8c657494: *stop=0x8c657494u;r[0]<<=2;goto L8c657496; /* shll2 r0 */
L8c657496: *stop=0x8c657496u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657498; /* mov.l @(r0,r5),r2 */
L8c657498: *stop=0x8c657498u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65749a; /* mov.l @(0x8,r15),r0 */
L8c65749a: *stop=0x8c65749au;r[3]+=r[2];goto L8c65749c; /* add r2,r3 */
L8c65749c: *stop=0x8c65749cu;r[0]<<=2;goto L8c65749e; /* shll2 r0 */
L8c65749e: *stop=0x8c65749eu;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6574a0; /* mov.l @(r0,r5),r2 */
L8c6574a0: *stop=0x8c6574a0u;r[0]=r[6];goto L8c6574a2; /* mov r6,r0 */
L8c6574a2: *stop=0x8c6574a2u;r[0]<<=2;goto L8c6574a4; /* shll2 r0 */
L8c6574a4: *stop=0x8c6574a4u;r[3]+=r[2];goto L8c6574a6; /* add r2,r3 */
L8c6574a6: *stop=0x8c6574a6u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6574a8; /* mov.l @(r0,r5),r2 */
L8c6574a8: *stop=0x8c6574a8u;r[3]+=r[2];goto L8c6574aa; /* add r2,r3 */
L8c6574aa: *stop=0x8c6574aau;r[3]<<=2;goto L8c6574ac; /* shll2 r3 */
L8c6574ac: *stop=0x8c6574acu;r[3]+=24u;goto L8c6574ae; /* add #0x18,r3 */
L8c6574ae: *stop=0x8c6574aeu;*macl=r[4]*r[3];goto L8c6574b0; /* mul.l r4,r3 */
L8c6574b0: *stop=0x8c6574b0u;r[3]=*macl;goto L8c6574b2; /* sts MACL,r3 */
L8c6574b2: *stop=0x8c6574b2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c6574b4; /* mov.l r3,@r1 */
L8c6574b4: *stop=0x8c6574b4u;if(*pr!=entry_pr)return -3;*stop=0x8c6574b6u;r[15]+=28u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c6574b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6574b8;
L8c6574b8: *stop=0x8c6574b8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6574ba; /* mov.l r14,@-r15 */
L8c6574ba: *stop=0x8c6574bau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6574bc; /* mov.l r13,@-r15 */
L8c6574bc: *stop=0x8c6574bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6574be; /* mov.l r12,@-r15 */
L8c6574be: *stop=0x8c6574beu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6574c0; /* mov.l r11,@-r15 */
L8c6574c0: *stop=0x8c6574c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6574c2; /* mov.l r10,@-r15 */
L8c6574c2: *stop=0x8c6574c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6574c4; /* mov.l r9,@-r15 */
L8c6574c4: *stop=0x8c6574c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6574c6; /* mov.l r8,@-r15 */
L8c6574c6: *stop=0x8c6574c6u;r[15]+=4294967264u;goto L8c6574c8; /* add #-0x20,r15 */
L8c6574c8: *stop=0x8c6574c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657604u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6574ca; /* mov.l 0x8c657604,r3 */
L8c6574ca: *stop=0x8c6574cau;r[2]=r[4];goto L8c6574cc; /* mov r4,r2 */
L8c6574cc: *stop=0x8c6574ccu;r[12]=0u;goto L8c6574ce; /* mov #0x0,r12 */
L8c6574ce: *stop=0x8c6574ceu;r[2]<<=2;goto L8c6574d0; /* shll2 r2 */
L8c6574d0: *stop=0x8c6574d0u;r[14]=r[12];goto L8c6574d2; /* mov r12,r14 */
L8c6574d2: *stop=0x8c6574d2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6574d4; /* mov.l r2,@r3 */
L8c6574d4: *stop=0x8c6574d4u;r[2]=r[5];goto L8c6574d6; /* mov r5,r2 */
L8c6574d6: *stop=0x8c6574d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657608u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6574d8; /* mov.l 0x8c657608,r1 */
L8c6574d8: *stop=0x8c6574d8u;r[2]<<=2;goto L8c6574da; /* shll2 r2 */
L8c6574da: *stop=0x8c6574dau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c6574dc; /* mov.l r2,@r1 */
L8c6574dc: *stop=0x8c6574dcu;r[3]=r[6];goto L8c6574de; /* mov r6,r3 */
L8c6574de: *stop=0x8c6574deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65760cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6574e0; /* mov.l 0x8c65760c,r0 */
L8c6574e0: *stop=0x8c6574e0u;r[3]<<=2;goto L8c6574e2; /* shll2 r3 */
L8c6574e2: *stop=0x8c6574e2u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c6574e4; /* mov.l r3,@r0 */
L8c6574e4: *stop=0x8c6574e4u;r[3]=r[7];goto L8c6574e6; /* mov r7,r3 */
L8c6574e6: *stop=0x8c6574e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657610u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6574e8; /* mov.l 0x8c657610,r2 */
L8c6574e8: *stop=0x8c6574e8u;r[3]<<=2;goto L8c6574ea; /* shll2 r3 */
L8c6574ea: *stop=0x8c6574eau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6574ec; /* mov.l r3,@r2 */
L8c6574ec: *stop=0x8c6574ecu;if(!valid_access(r[15]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6574ee; /* mov.l @(0x3c,r15),r3 */
L8c6574ee: *stop=0x8c6574eeu;r[1]+=12u;goto L8c6574f0; /* add #0xc,r1 */
L8c6574f0: *stop=0x8c6574f0u;r[3]<<=2;goto L8c6574f2; /* shll2 r3 */
L8c6574f2: *stop=0x8c6574f2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c6574f4; /* mov.l r3,@r1 */
L8c6574f4: *stop=0x8c6574f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657614u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6574f6; /* mov.l 0x8c657614,r0 */
L8c6574f6: *stop=0x8c6574f6u;r[9]=r[15];goto L8c6574f8; /* mov r15,r9 */
L8c6574f8: *stop=0x8c6574f8u;r[9]+=24u;goto L8c6574fa; /* add #0x18,r9 */
L8c6574fa: *stop=0x8c6574fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c657618u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c6574fc; /* mov.l 0x8c657618,r8 */
L8c6574fc: *stop=0x8c6574fcu;r[3]=r[9];goto L8c6574fe; /* mov r9,r3 */
L8c6574fe: *stop=0x8c6574feu;r[1]=r[9];goto L8c657500; /* mov r9,r1 */
L8c657500: *stop=0x8c657500u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c657502; /* mov.l r3,@(0x8,r15) */
L8c657502: *stop=0x8c657502u;r[13]=r[14];goto L8c657504; /* mov r14,r13 */
L8c657504: *stop=0x8c657504u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657506; /* mov.l @r0,r2 */
L8c657506: *stop=0x8c657506u;r[0]=68u;goto L8c657508; /* mov #0x44,r0 */
L8c657508: *stop=0x8c657508u;r[1]+=4u;goto L8c65750a; /* add #0x4,r1 */
L8c65750a: *stop=0x8c65750au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c65750c; /* mov.l r2,@r3 */
L8c65750c: *stop=0x8c65750cu;r[3]=r[8];goto L8c65750e; /* mov r8,r3 */
L8c65750e: *stop=0x8c65750eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c657510; /* mov.l r3,@(0x4,r15) */
L8c657510: *stop=0x8c657510u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c657512; /* mov.l r3,@(0xc,r15) */
L8c657512: *stop=0x8c657512u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657514; /* mov.l @(0x8,r15),r2 */
L8c657514: *stop=0x8c657514u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c657516; /* mov.l r2,@(0x10,r15) */
L8c657516: *stop=0x8c657516u;r[2]=r[13];goto L8c657518; /* mov r13,r2 */
L8c657518: *stop=0x8c657518u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[1],4);if(io_status)return io_status;goto L8c65751a; /* mov.l r1,@(0x14,r15) */
L8c65751a: *stop=0x8c65751au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65751c; /* mov.l @(r0,r15),r0 */
L8c65751c: *stop=0x8c65751cu;*sr=(*sr&~1u)|(r[0]>r[2]);goto L8c65751e; /* cmp/hi r2,r0 */
L8c65751e: if((*sr&1u))goto L8c657524;goto L8c657520; /* bt 0x8c657524 */
L8c657520: *stop=0x8c657522u;goto L8c65771c; /* bra 0x8c65771c */
L8c657524: *stop=0x8c657524u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65761cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c657526; /* mov.l 0x8c65761c,r10 */
L8c657526: *stop=0x8c657526u;r[11]=4294967288u;goto L8c657528; /* mov #-0x8,r11 */
L8c657528: *stop=0x8c657528u;r[0]=r[13];goto L8c65752a; /* mov r13,r0 */
L8c65752a: *stop=0x8c65752au;r[0]<<=2;goto L8c65752c; /* shll2 r0 */
L8c65752c: *stop=0x8c65752cu;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65752e; /* mov.l @(r0,r9),r0 */
L8c65752e: *stop=0x8c65752eu;*sr=(*sr&~1u)|!(r[0]&0x1fu);goto L8c657530; /* tst #0x1f,r0 */
L8c657530: if((*sr&1u))goto L8c657546;goto L8c657532; /* bt 0x8c657546 */
L8c657532: *stop=0x8c657532u;r[2]=r[13];goto L8c657534; /* mov r13,r2 */
L8c657534: *stop=0x8c657534u;r[2]<<=2;goto L8c657536; /* shll2 r2 */
L8c657536: *stop=0x8c657536u;r[2]+=r[9];goto L8c657538; /* add r9,r2 */
L8c657538: *stop=0x8c657538u;r[1]=4294967264u;goto L8c65753a; /* mov #-0x20,r1 */
L8c65753a: *stop=0x8c65753au;r[0]=r[2];goto L8c65753c; /* mov r2,r0 */
L8c65753c: *stop=0x8c65753cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c65753e; /* mov.l r2,@r15 */
L8c65753e: *stop=0x8c65753eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657540; /* mov.l @r2,r3 */
L8c657540: *stop=0x8c657540u;r[3]+=31u;goto L8c657542; /* add #0x1f,r3 */
L8c657542: *stop=0x8c657542u;r[3]&=r[1];goto L8c657544; /* and r1,r3 */
L8c657544: *stop=0x8c657544u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c657546; /* mov.l r3,@r0 */
L8c657546: *stop=0x8c657546u;r[2]=r[13];goto L8c657548; /* mov r13,r2 */
L8c657548: *stop=0x8c657548u;r[2]<<=2;goto L8c65754a; /* shll2 r2 */
L8c65754a: *stop=0x8c65754au;r[3]=r[13];goto L8c65754c; /* mov r13,r3 */
L8c65754c: *stop=0x8c65754cu;r[2]+=r[3];goto L8c65754e; /* add r3,r2 */
L8c65754e: *stop=0x8c65754eu;r[2]<<=2;goto L8c657550; /* shll2 r2 */
L8c657550: *stop=0x8c657550u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657620u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657552; /* mov.l 0x8c657620,r3 */
L8c657552: *stop=0x8c657552u;r[2]+=r[8];goto L8c657554; /* add r8,r2 */
L8c657554: *stop=0x8c657554u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c657556; /* mov.l r2,@r15 */
L8c657556: *stop=0x8c657556u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657558; /* mov.l @r3,r0 */
L8c657558: *stop=0x8c657558u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65755a; /* cmp/eq #0x1,r0 */
L8c65755a: if(!(*sr&1u))goto L8c65756a;goto L8c65755c; /* bf 0x8c65756a */
L8c65755c: *stop=0x8c65755cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657624u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65755e; /* mov.l 0x8c657624,r1 */
L8c65755e: *stop=0x8c65755eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657560; /* mov.l @r1,r0 */
L8c657560: *stop=0x8c657560u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c657562; /* cmp/eq #0x20,r0 */
L8c657562: if(!(*sr&1u))goto L8c65756a;goto L8c657564; /* bf 0x8c65756a */
L8c657564: *stop=0x8c657564u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657566; /* mov.l @r15,r2 */
L8c657566: *stop=0x8c657568u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[10],4);if(io_status)return io_status;goto L8c65758c; /* bra 0x8c65758c */
L8c65756a: *stop=0x8c65756au;r[2]=0u;goto L8c65756c; /* mov #0x0,r2 */
L8c65756c: *stop=0x8c65756cu;*sr=(*sr&~1u)|(r[4]>r[2]);goto L8c65756e; /* cmp/hi r2,r4 */
L8c65756e: if(!(*sr&1u))goto L8c657588;goto L8c657570; /* bf 0x8c657588 */
L8c657570: *stop=0x8c657570u;r[0]=r[13];goto L8c657572; /* mov r13,r0 */
L8c657572: *stop=0x8c657572u;r[0]<<=2;goto L8c657574; /* shll2 r0 */
L8c657574: *stop=0x8c657574u;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657576; /* mov.l @(r0,r9),r3 */
L8c657576: *stop=0x8c657576u;r[0]=r[14];goto L8c657578; /* mov r14,r0 */
L8c657578: *stop=0x8c657578u;r[0]<<=2;goto L8c65757a; /* shll2 r0 */
L8c65757a: *stop=0x8c65757au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65757c; /* mov.l @r15,r1 */
L8c65757c: *stop=0x8c65757cu;r[14]+=r[4];goto L8c65757e; /* add r4,r14 */
L8c65757e: *stop=0x8c65757eu;r[3]+=r[0];goto L8c657580; /* add r0,r3 */
L8c657580: *stop=0x8c657580u;r[14]+=23u;goto L8c657582; /* add #0x17,r14 */
L8c657582: *stop=0x8c657582u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c657584; /* mov.l r3,@r1 */
L8c657584: *stop=0x8c657586u;r[14]&=r[11];goto L8c65758c; /* bra 0x8c65758c */
L8c657588: *stop=0x8c657588u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65758a; /* mov.l @r15,r1 */
L8c65758a: *stop=0x8c65758au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[12],4);if(io_status)return io_status;goto L8c65758c; /* mov.l r12,@r1 */
L8c65758c: *stop=0x8c65758cu;r[3]=r[13];goto L8c65758e; /* mov r13,r3 */
L8c65758e: *stop=0x8c65758eu;r[3]<<=2;goto L8c657590; /* shll2 r3 */
L8c657590: *stop=0x8c657590u;r[2]=r[13];goto L8c657592; /* mov r13,r2 */
L8c657592: *stop=0x8c657592u;r[3]+=r[2];goto L8c657594; /* add r2,r3 */
L8c657594: *stop=0x8c657594u;r[3]<<=2;goto L8c657596; /* shll2 r3 */
L8c657596: *stop=0x8c657596u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657620u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657598; /* mov.l 0x8c657620,r2 */
L8c657598: *stop=0x8c657598u;r[3]+=r[8];goto L8c65759a; /* add r8,r3 */
L8c65759a: *stop=0x8c65759au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65759c; /* mov.l r3,@r15 */
L8c65759c: *stop=0x8c65759cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65759e; /* mov.l @r2,r0 */
L8c65759e: *stop=0x8c65759eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6575a0; /* cmp/eq #0x1,r0 */
L8c6575a0: if(!(*sr&1u))goto L8c6575b0;goto L8c6575a2; /* bf 0x8c6575b0 */
L8c6575a2: *stop=0x8c6575a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657624u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6575a4; /* mov.l 0x8c657624,r1 */
L8c6575a4: *stop=0x8c6575a4u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6575a6; /* mov.l @r1,r0 */
L8c6575a6: *stop=0x8c6575a6u;*sr=(*sr&~1u)|(r[0]==0x00000021u);goto L8c6575a8; /* cmp/eq #0x21,r0 */
L8c6575a8: if(!(*sr&1u))goto L8c6575b0;goto L8c6575aa; /* bf 0x8c6575b0 */
L8c6575aa: *stop=0x8c6575aau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6575ac; /* mov.l @r15,r2 */
L8c6575ac: *stop=0x8c6575aeu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[10],4);if(io_status)return io_status;goto L8c6575d2; /* bra 0x8c6575d2 */
L8c6575b0: *stop=0x8c6575b0u;r[2]=0u;goto L8c6575b2; /* mov #0x0,r2 */
L8c6575b2: *stop=0x8c6575b2u;*sr=(*sr&~1u)|(r[5]>r[2]);goto L8c6575b4; /* cmp/hi r2,r5 */
L8c6575b4: if(!(*sr&1u))goto L8c6575ce;goto L8c6575b6; /* bf 0x8c6575ce */
L8c6575b6: *stop=0x8c6575b6u;r[0]=r[13];goto L8c6575b8; /* mov r13,r0 */
L8c6575b8: *stop=0x8c6575b8u;r[0]<<=2;goto L8c6575ba; /* shll2 r0 */
L8c6575ba: *stop=0x8c6575bau;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6575bc; /* mov.l @(r0,r9),r3 */
L8c6575bc: *stop=0x8c6575bcu;r[0]=r[14];goto L8c6575be; /* mov r14,r0 */
L8c6575be: *stop=0x8c6575beu;r[0]<<=2;goto L8c6575c0; /* shll2 r0 */
L8c6575c0: *stop=0x8c6575c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6575c2; /* mov.l @r15,r1 */
L8c6575c2: *stop=0x8c6575c2u;r[14]+=r[5];goto L8c6575c4; /* add r5,r14 */
L8c6575c4: *stop=0x8c6575c4u;r[3]+=r[0];goto L8c6575c6; /* add r0,r3 */
L8c6575c6: *stop=0x8c6575c6u;r[14]+=23u;goto L8c6575c8; /* add #0x17,r14 */
L8c6575c8: *stop=0x8c6575c8u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6575ca; /* mov.l r3,@(0x4,r1) */
L8c6575ca: *stop=0x8c6575ccu;r[14]&=r[11];goto L8c6575d2; /* bra 0x8c6575d2 */
L8c6575ce: *stop=0x8c6575ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6575d0; /* mov.l @r15,r1 */
L8c6575d0: *stop=0x8c6575d0u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[12],4);if(io_status)return io_status;goto L8c6575d2; /* mov.l r12,@(0x4,r1) */
L8c6575d2: *stop=0x8c6575d2u;r[3]=r[13];goto L8c6575d4; /* mov r13,r3 */
L8c6575d4: *stop=0x8c6575d4u;r[3]<<=2;goto L8c6575d6; /* shll2 r3 */
L8c6575d6: *stop=0x8c6575d6u;r[2]=r[13];goto L8c6575d8; /* mov r13,r2 */
L8c6575d8: *stop=0x8c6575d8u;r[3]+=r[2];goto L8c6575da; /* add r2,r3 */
L8c6575da: *stop=0x8c6575dau;r[3]<<=2;goto L8c6575dc; /* shll2 r3 */
L8c6575dc: *stop=0x8c6575dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657620u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6575de; /* mov.l 0x8c657620,r2 */
L8c6575de: *stop=0x8c6575deu;r[3]+=r[8];goto L8c6575e0; /* add r8,r3 */
L8c6575e0: *stop=0x8c6575e0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6575e2; /* mov.l r3,@r15 */
L8c6575e2: *stop=0x8c6575e2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6575e4; /* mov.l @r2,r0 */
L8c6575e4: *stop=0x8c6575e4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6575e6; /* cmp/eq #0x1,r0 */
L8c6575e6: if(!(*sr&1u))goto L8c657640;goto L8c6575e8; /* bf 0x8c657640 */
L8c6575e8: *stop=0x8c6575e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657624u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6575ea; /* mov.l 0x8c657624,r1 */
L8c6575ea: *stop=0x8c6575eau;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6575ec; /* mov.l @r1,r0 */
L8c6575ec: *stop=0x8c6575ecu;*sr=(*sr&~1u)|(r[0]==0x00000022u);goto L8c6575ee; /* cmp/eq #0x22,r0 */
L8c6575ee: if(!(*sr&1u))goto L8c657640;goto L8c6575f0; /* bf 0x8c657640 */
L8c6575f0: *stop=0x8c6575f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6575f2; /* mov.l @r15,r2 */
L8c6575f2: *stop=0x8c6575f4u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[10],4);if(io_status)return io_status;goto L8c657662; /* bra 0x8c657662 */
L8c657640: *stop=0x8c657640u;r[2]=0u;goto L8c657642; /* mov #0x0,r2 */
L8c657642: *stop=0x8c657642u;*sr=(*sr&~1u)|(r[6]>r[2]);goto L8c657644; /* cmp/hi r2,r6 */
L8c657644: if(!(*sr&1u))goto L8c65765e;goto L8c657646; /* bf 0x8c65765e */
L8c657646: *stop=0x8c657646u;r[0]=r[13];goto L8c657648; /* mov r13,r0 */
L8c657648: *stop=0x8c657648u;r[0]<<=2;goto L8c65764a; /* shll2 r0 */
L8c65764a: *stop=0x8c65764au;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65764c; /* mov.l @(r0,r9),r3 */
L8c65764c: *stop=0x8c65764cu;r[0]=r[14];goto L8c65764e; /* mov r14,r0 */
L8c65764e: *stop=0x8c65764eu;r[0]<<=2;goto L8c657650; /* shll2 r0 */
L8c657650: *stop=0x8c657650u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657652; /* mov.l @r15,r1 */
L8c657652: *stop=0x8c657652u;r[14]+=r[6];goto L8c657654; /* add r6,r14 */
L8c657654: *stop=0x8c657654u;r[3]+=r[0];goto L8c657656; /* add r0,r3 */
L8c657656: *stop=0x8c657656u;r[14]+=23u;goto L8c657658; /* add #0x17,r14 */
L8c657658: *stop=0x8c657658u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[3],4);if(io_status)return io_status;goto L8c65765a; /* mov.l r3,@(0x8,r1) */
L8c65765a: *stop=0x8c65765cu;r[14]&=r[11];goto L8c657662; /* bra 0x8c657662 */
L8c65765e: *stop=0x8c65765eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657660; /* mov.l @r15,r1 */
L8c657660: *stop=0x8c657660u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[12],4);if(io_status)return io_status;goto L8c657662; /* mov.l r12,@(0x8,r1) */
L8c657662: *stop=0x8c657662u;r[3]=r[13];goto L8c657664; /* mov r13,r3 */
L8c657664: *stop=0x8c657664u;r[3]<<=2;goto L8c657666; /* shll2 r3 */
L8c657666: *stop=0x8c657666u;r[2]=r[13];goto L8c657668; /* mov r13,r2 */
L8c657668: *stop=0x8c657668u;r[3]+=r[2];goto L8c65766a; /* add r2,r3 */
L8c65766a: *stop=0x8c65766au;r[3]<<=2;goto L8c65766c; /* shll2 r3 */
L8c65766c: *stop=0x8c65766cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657740u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65766e; /* mov.l 0x8c657740,r2 */
L8c65766e: *stop=0x8c65766eu;r[3]+=r[8];goto L8c657670; /* add r8,r3 */
L8c657670: *stop=0x8c657670u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c657672; /* mov.l r3,@r15 */
L8c657672: *stop=0x8c657672u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657674; /* mov.l @r2,r0 */
L8c657674: *stop=0x8c657674u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c657676; /* cmp/eq #0x1,r0 */
L8c657676: if(!(*sr&1u))goto L8c657686;goto L8c657678; /* bf 0x8c657686 */
L8c657678: *stop=0x8c657678u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657744u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65767a; /* mov.l 0x8c657744,r1 */
L8c65767a: *stop=0x8c65767au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65767c; /* mov.l @r1,r0 */
L8c65767c: *stop=0x8c65767cu;*sr=(*sr&~1u)|(r[0]==0x00000023u);goto L8c65767e; /* cmp/eq #0x23,r0 */
L8c65767e: if(!(*sr&1u))goto L8c657686;goto L8c657680; /* bf 0x8c657686 */
L8c657680: *stop=0x8c657680u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c657682; /* mov.l @r15,r2 */
L8c657682: *stop=0x8c657684u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[10],4);if(io_status)return io_status;goto L8c6576a8; /* bra 0x8c6576a8 */
L8c657686: *stop=0x8c657686u;r[2]=0u;goto L8c657688; /* mov #0x0,r2 */
L8c657688: *stop=0x8c657688u;*sr=(*sr&~1u)|(r[7]>r[2]);goto L8c65768a; /* cmp/hi r2,r7 */
L8c65768a: if(!(*sr&1u))goto L8c6576a4;goto L8c65768c; /* bf 0x8c6576a4 */
L8c65768c: *stop=0x8c65768cu;r[0]=r[13];goto L8c65768e; /* mov r13,r0 */
L8c65768e: *stop=0x8c65768eu;r[0]<<=2;goto L8c657690; /* shll2 r0 */
L8c657690: *stop=0x8c657690u;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657692; /* mov.l @(r0,r9),r3 */
L8c657692: *stop=0x8c657692u;r[0]=r[14];goto L8c657694; /* mov r14,r0 */
L8c657694: *stop=0x8c657694u;r[0]<<=2;goto L8c657696; /* shll2 r0 */
L8c657696: *stop=0x8c657696u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c657698; /* mov.l @r15,r1 */
L8c657698: *stop=0x8c657698u;r[14]+=r[7];goto L8c65769a; /* add r7,r14 */
L8c65769a: *stop=0x8c65769au;r[3]+=r[0];goto L8c65769c; /* add r0,r3 */
L8c65769c: *stop=0x8c65769cu;r[14]+=23u;goto L8c65769e; /* add #0x17,r14 */
L8c65769e: *stop=0x8c65769eu;if(!valid_access(r[1]+12u,4u))return -3;wr(m,bus,&io_status,r[1]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6576a0; /* mov.l r3,@(0xc,r1) */
L8c6576a0: *stop=0x8c6576a2u;r[14]&=r[11];goto L8c6576a8; /* bra 0x8c6576a8 */
L8c6576a4: *stop=0x8c6576a4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6576a6; /* mov.l @r15,r1 */
L8c6576a6: *stop=0x8c6576a6u;if(!valid_access(r[1]+12u,4u))return -3;wr(m,bus,&io_status,r[1]+0xcu,r[12],4);if(io_status)return io_status;goto L8c6576a8; /* mov.l r12,@(0xc,r1) */
L8c6576a8: *stop=0x8c6576a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657740u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6576aa; /* mov.l 0x8c657740,r3 */
L8c6576aa: *stop=0x8c6576aau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6576ac; /* mov.l @r3,r0 */
L8c6576ac: *stop=0x8c6576acu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6576ae; /* cmp/eq #0x1,r0 */
L8c6576ae: if(!(*sr&1u))goto L8c6576ca;goto L8c6576b0; /* bf 0x8c6576ca */
L8c6576b0: *stop=0x8c6576b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657744u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6576b2; /* mov.l 0x8c657744,r1 */
L8c6576b2: *stop=0x8c6576b2u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6576b4; /* mov.l @r1,r0 */
L8c6576b4: *stop=0x8c6576b4u;*sr=(*sr&~1u)|(r[0]==0x00000024u);goto L8c6576b6; /* cmp/eq #0x24,r0 */
L8c6576b6: if(!(*sr&1u))goto L8c6576ca;goto L8c6576b8; /* bf 0x8c6576ca */
L8c6576b8: *stop=0x8c6576b8u;r[3]=r[13];goto L8c6576ba; /* mov r13,r3 */
L8c6576ba: *stop=0x8c6576bau;r[3]<<=2;goto L8c6576bc; /* shll2 r3 */
L8c6576bc: *stop=0x8c6576bcu;r[2]=r[13];goto L8c6576be; /* mov r13,r2 */
L8c6576be: *stop=0x8c6576beu;r[3]+=r[2];goto L8c6576c0; /* add r2,r3 */
L8c6576c0: *stop=0x8c6576c0u;r[3]<<=2;goto L8c6576c2; /* shll2 r3 */
L8c6576c2: *stop=0x8c6576c2u;r[3]+=r[8];goto L8c6576c4; /* add r8,r3 */
L8c6576c4: *stop=0x8c6576c4u;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[10],4);if(io_status)return io_status;goto L8c6576c6; /* mov.l r10,@(0x10,r3) */
L8c6576c6: *stop=0x8c6576c8u;goto L8c6576ee; /* bra 0x8c6576ee */
L8c6576ca: *stop=0x8c6576cau;if(!valid_access(r[15]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6576cc; /* mov.l @(0x3c,r15),r3 */
L8c6576cc: *stop=0x8c6576ccu;r[2]=0u;goto L8c6576ce; /* mov #0x0,r2 */
L8c6576ce: *stop=0x8c6576ceu;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c6576d0; /* cmp/hi r2,r3 */
L8c6576d0: if(!(*sr&1u))goto L8c6576ea;goto L8c6576d2; /* bf 0x8c6576ea */
L8c6576d2: *stop=0x8c6576d2u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6576d4; /* mov.l @(0x10,r15),r0 */
L8c6576d4: *stop=0x8c6576d4u;r[1]=r[14];goto L8c6576d6; /* mov r14,r1 */
L8c6576d6: *stop=0x8c6576d6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6576d8; /* mov.l @(0x4,r15),r3 */
L8c6576d8: *stop=0x8c6576d8u;r[1]<<=2;goto L8c6576da; /* shll2 r1 */
L8c6576da: *stop=0x8c6576dau;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6576dc; /* mov.l @r0,r0 */
L8c6576dc: *stop=0x8c6576dcu;r[1]+=r[0];goto L8c6576de; /* add r0,r1 */
L8c6576de: *stop=0x8c6576deu;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[1],4);if(io_status)return io_status;goto L8c6576e0; /* mov.l r1,@(0x10,r3) */
L8c6576e0: *stop=0x8c6576e0u;if(!valid_access(r[15]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6576e2; /* mov.l @(0x3c,r15),r3 */
L8c6576e2: *stop=0x8c6576e2u;r[14]+=r[3];goto L8c6576e4; /* add r3,r14 */
L8c6576e4: *stop=0x8c6576e4u;r[14]+=23u;goto L8c6576e6; /* add #0x17,r14 */
L8c6576e6: *stop=0x8c6576e8u;r[14]&=r[11];goto L8c6576ee; /* bra 0x8c6576ee */
L8c6576ea: *stop=0x8c6576eau;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6576ec; /* mov.l @(0xc,r15),r3 */
L8c6576ec: *stop=0x8c6576ecu;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[12],4);if(io_status)return io_status;goto L8c6576ee; /* mov.l r12,@(0x10,r3) */
L8c6576ee: *stop=0x8c6576eeu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6576f0; /* mov.l @(0x8,r15),r2 */
L8c6576f0: *stop=0x8c6576f0u;r[14]<<=2;goto L8c6576f2; /* shll2 r14 */
L8c6576f2: *stop=0x8c6576f2u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6576f4; /* mov.l @(0x14,r15),r3 */
L8c6576f4: *stop=0x8c6576f4u;r[0]=68u;goto L8c6576f6; /* mov #0x44,r0 */
L8c6576f6: *stop=0x8c6576f6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6576f8; /* mov.l @r2,r1 */
L8c6576f8: *stop=0x8c6576f8u;r[13]+=1u;goto L8c6576fa; /* add #0x1,r13 */
L8c6576fa: *stop=0x8c6576fau;r[14]+=r[1];goto L8c6576fc; /* add r1,r14 */
L8c6576fc: *stop=0x8c6576fcu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6576fe; /* mov.l r14,@r3 */
L8c6576fe: *stop=0x8c6576feu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657700; /* mov.l @(0xc,r15),r3 */
L8c657700: *stop=0x8c657700u;r[3]+=20u;goto L8c657702; /* add #0x14,r3 */
L8c657702: *stop=0x8c657702u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c657704; /* mov.l r3,@(0xc,r15) */
L8c657704: *stop=0x8c657704u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657706; /* mov.l @(0x10,r15),r2 */
L8c657706: *stop=0x8c657706u;r[2]+=4u;goto L8c657708; /* add #0x4,r2 */
L8c657708: *stop=0x8c657708u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c65770a; /* mov.l r2,@(0x10,r15) */
L8c65770a: *stop=0x8c65770au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65770c; /* mov.l @(0x4,r15),r1 */
L8c65770c: *stop=0x8c65770cu;r[1]+=20u;goto L8c65770e; /* add #0x14,r1 */
L8c65770e: *stop=0x8c65770eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c657710; /* mov.l r1,@(0x4,r15) */
L8c657710: *stop=0x8c657710u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657712; /* mov.l @(r0,r15),r3 */
L8c657712: *stop=0x8c657712u;*sr=(*sr&~1u)|(r[13]>=r[3]);goto L8c657714; /* cmp/hs r3,r13 */
L8c657714: take=(*sr&1u);*stop=0x8c657716u;r[14]=r[12];if(take)goto L8c65771c;goto L8c657718; /* bt/s 0x8c65771c */
L8c657718: *stop=0x8c65771au;goto L8c657528; /* bra 0x8c657528 */
L8c65771c: *stop=0x8c65771cu;r[0]=0u;goto L8c65771e; /* mov #0x0,r0 */
L8c65771e: *stop=0x8c65771eu;r[15]+=32u;goto L8c657720; /* add #0x20,r15 */
L8c657720: *stop=0x8c657720u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c657722; /* mov.l @r15+,r8 */
L8c657722: *stop=0x8c657722u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c657724; /* mov.l @r15+,r9 */
L8c657724: *stop=0x8c657724u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c657726; /* mov.l @r15+,r10 */
L8c657726: *stop=0x8c657726u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c657728; /* mov.l @r15+,r11 */
L8c657728: *stop=0x8c657728u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65772a; /* mov.l @r15+,r12 */
L8c65772a: *stop=0x8c65772au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65772c; /* mov.l @r15+,r13 */
L8c65772c: *stop=0x8c65772cu;if(*pr!=entry_pr)return -3;*stop=0x8c65772eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c657760(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c657760;
L8c657760: *stop=0x8c657760u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c657762; /* mov.l r14,@-r15 */
L8c657762: *stop=0x8c657762u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c657764; /* mov.l r13,@-r15 */
L8c657764: *stop=0x8c657764u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c657766; /* mov.l r12,@-r15 */
L8c657766: *stop=0x8c657766u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c657768; /* mov.l r11,@-r15 */
L8c657768: *stop=0x8c657768u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65776a; /* mov.l r10,@-r15 */
L8c65776a: *stop=0x8c65776au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65776c; /* mov.l r9,@-r15 */
L8c65776c: *stop=0x8c65776cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65776e; /* mov.l r8,@-r15 */
L8c65776e: *stop=0x8c65776eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c657770; /* sts.l PR,@-r15 */
L8c657770: *stop=0x8c657770u;r[15]+=4294967292u;goto L8c657772; /* add #-0x4,r15 */
L8c657772: *stop=0x8c657772u;r[0]=r[4];goto L8c657774; /* mov r4,r0 */
L8c657774: *stop=0x8c657774u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657800u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c657776; /* mov.l 0x8c657800,r14 */
L8c657776: *stop=0x8c657776u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c657778; /* cmp/eq #0x1,r0 */
L8c657778: *stop=0x8c657778u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c65777a; /* mov.l r4,@r15 */
L8c65777a: take=!(*sr&1u);*stop=0x8c65777cu;r[11]=2u;if(take)goto L8c657796;goto L8c65777e; /* bf/s 0x8c657796 */
L8c65777e: *stop=0x8c65777eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657780; /* mov.l @(0x10,r14),r2 */
L8c657780: *stop=0x8c657780u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657804u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657782; /* mov.l 0x8c657804,r3 */
L8c657782: *stop=0x8c657782u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c657784; /* mov.l r2,@-r15 */
L8c657784: *stop=0x8c657784u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c657786; /* mov.l @(0x4,r14),r5 */
L8c657786: *stop=0x8c657786u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c657788; /* mov.l @(0x8,r14),r6 */
L8c657788: *stop=0x8c657788u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c65778a; /* mov.l @(0xc,r14),r7 */
L8c65778a: target=r[3];*pr=0x8c65778eu;*stop=0x8c65778cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65778e; /* jsr @r3 */
L8c65778e: *stop=0x8c65778eu;r[4]=r[0];goto L8c657790; /* mov r0,r4 */
L8c657790: *stop=0x8c657790u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c657792; /* tst r4,r4 */
L8c657792: take=!(*sr&1u);*stop=0x8c657794u;r[15]+=4u;if(take)goto L8c6577e4;goto L8c657796; /* bf/s 0x8c6577e4 */
L8c657796: *stop=0x8c657796u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657798; /* mov.l @r15,r0 */
L8c657798: *stop=0x8c657798u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c65779a; /* cmp/eq #0x2,r0 */
L8c65779a: if(!(*sr&1u))goto L8c6577ac;goto L8c65779c; /* bf 0x8c6577ac */
L8c65779c: *stop=0x8c65779cu;r[0]=88u;goto L8c65779e; /* mov #0x58,r0 */
L8c65779e: *stop=0x8c65779eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657808u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6577a0; /* mov.l 0x8c657808,r3 */
L8c6577a0: *stop=0x8c6577a0u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6577a2; /* mov.l @(r0,r14),r5 */
L8c6577a2: target=r[3];*pr=0x8c6577a6u;*stop=0x8c6577a4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6577a6; /* jsr @r3 */
L8c6577a6: *stop=0x8c6577a6u;r[4]=r[0];goto L8c6577a8; /* mov r0,r4 */
L8c6577a8: *stop=0x8c6577a8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6577aa; /* tst r4,r4 */
L8c6577aa: if(!(*sr&1u))goto L8c6577e4;goto L8c6577ac; /* bf 0x8c6577e4 */
L8c6577ac: *stop=0x8c6577acu;r[4]=r[14];goto L8c6577ae; /* mov r14,r4 */
L8c6577ae: *stop=0x8c6577aeu;r[4]+=28u;goto L8c6577b0; /* add #0x1c,r4 */
L8c6577b0: *stop=0x8c6577b0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6577b2; /* mov.l @r4,r2 */
L8c6577b2: *stop=0x8c6577b2u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6577b4; /* mov.l @(0x10,r4),r3 */
L8c6577b4: *stop=0x8c6577b4u;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c6577b6; /* cmp/eq r3,r2 */
L8c6577b6: take=!(*sr&1u);*stop=0x8c6577b8u;r[8]=1u;if(take)goto L8c6577bc;goto L8c6577ba; /* bf/s 0x8c6577bc */
L8c6577ba: *stop=0x8c6577bau;r[11]=r[8];goto L8c6577bc; /* mov r8,r11 */
L8c6577bc: *stop=0x8c6577bcu;r[10]=0u;goto L8c6577be; /* mov #0x0,r10 */
L8c6577be: *stop=0x8c6577beu;r[3]=r[10];goto L8c6577c0; /* mov r10,r3 */
L8c6577c0: *stop=0x8c6577c0u;*sr=(*sr&~1u)|(r[3]>=r[11]);goto L8c6577c2; /* cmp/hs r11,r3 */
L8c6577c2: take=(*sr&1u);*stop=0x8c6577c4u;r[12]=r[10];if(take)goto L8c657828;goto L8c6577c6; /* bt/s 0x8c657828 */
L8c6577c6: *stop=0x8c6577c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65780cu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c6577c8; /* mov.l 0x8c65780c,r9 */
L8c6577c8: *stop=0x8c6577c8u;r[13]=r[14];goto L8c6577ca; /* mov r14,r13 */
L8c6577ca: *stop=0x8c6577cau;r[13]+=28u;goto L8c6577cc; /* add #0x1c,r13 */
L8c6577cc: *stop=0x8c6577ccu;r[13]+=r[12];goto L8c6577ce; /* add r12,r13 */
L8c6577ce: *stop=0x8c6577ceu;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6577d0; /* mov.l @(0xc,r13),r3 */
L8c6577d0: *stop=0x8c6577d0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6577d2; /* mov.l r3,@-r15 */
L8c6577d2: *stop=0x8c6577d2u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6577d4; /* mov.l @(0x8,r13),r6 */
L8c6577d4: *stop=0x8c6577d4u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6577d6; /* mov.l @(0x4,r13),r5 */
L8c6577d6: *stop=0x8c6577d6u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6577d8; /* mov.l @(0x18,r14),r7 */
L8c6577d8: target=r[9];*pr=0x8c6577dcu;*stop=0x8c6577dau;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6577dc; /* jsr @r9 */
L8c6577dc: *stop=0x8c6577dcu;r[4]=r[0];goto L8c6577de; /* mov r0,r4 */
L8c6577de: *stop=0x8c6577deu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6577e0; /* tst r4,r4 */
L8c6577e0: take=(*sr&1u);*stop=0x8c6577e2u;r[15]+=4u;if(take)goto L8c657820;goto L8c6577e4; /* bt/s 0x8c657820 */
L8c6577e4: *stop=0x8c6577e6u;r[0]=r[4];goto L8c6578cc; /* bra 0x8c6578cc */
L8c657820: *stop=0x8c657820u;r[10]+=1u;goto L8c657822; /* add #0x1,r10 */
L8c657822: *stop=0x8c657822u;*sr=(*sr&~1u)|(r[10]>=r[11]);goto L8c657824; /* cmp/hs r11,r10 */
L8c657824: take=!(*sr&1u);*stop=0x8c657826u;r[12]+=16u;if(take)goto L8c6577c8;goto L8c657828; /* bf/s 0x8c6577c8 */
L8c657828: *stop=0x8c657828u;r[0]=68u;goto L8c65782a; /* mov #0x44,r0 */
L8c65782a: *stop=0x8c65782au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65790cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65782c; /* mov.l 0x8c65790c,r3 */
L8c65782c: *stop=0x8c65782cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[7]=loaded;}goto L8c65782e; /* mov.l @(r0,r14),r7 */
L8c65782e: *stop=0x8c65782eu;r[0]=64u;goto L8c657830; /* mov #0x40,r0 */
L8c657830: *stop=0x8c657830u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c657832; /* mov.l @(r0,r14),r5 */
L8c657832: *stop=0x8c657832u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c657834; /* mov.l @(0x18,r14),r6 */
L8c657834: target=r[3];*pr=0x8c657838u;*stop=0x8c657836u;if(!valid_access(r[14]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x3cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c657838; /* jsr @r3 */
L8c657838: *stop=0x8c657838u;r[13]=r[0];goto L8c65783a; /* mov r0,r13 */
L8c65783a: *stop=0x8c65783au;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c65783c; /* tst r13,r13 */
L8c65783c: if(!(*sr&1u))goto L8c6578ca;goto L8c65783e; /* bf 0x8c6578ca */
L8c65783e: *stop=0x8c65783eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657910u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c657840; /* mov.l 0x8c657910,r3 */
L8c657840: *stop=0x8c657840u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c657842; /* mov.l @r3,r0 */
L8c657842: *stop=0x8c657842u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c657844; /* cmp/eq #0x1,r0 */
L8c657844: if((*sr&1u))goto L8c65784e;goto L8c657846; /* bt 0x8c65784e */
L8c657846: *stop=0x8c657846u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657914u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657848; /* mov.l 0x8c657914,r1 */
L8c657848: *stop=0x8c657848u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65784a; /* mov.l @r1,r0 */
L8c65784a: *stop=0x8c65784au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65784c; /* cmp/eq #0x1,r0 */
L8c65784c: if(!(*sr&1u))goto L8c6578ca;goto L8c65784e; /* bf 0x8c6578ca */
L8c65784e: *stop=0x8c65784eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657918u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657850; /* mov.l 0x8c657918,r2 */
L8c657850: *stop=0x8c657850u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c657852; /* mov.l @r2,r3 */
L8c657852: *stop=0x8c657852u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65791cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657854; /* mov.l 0x8c65791c,r0 */
L8c657854: *stop=0x8c657854u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c657856; /* mov.l r3,@r0 */
L8c657856: *stop=0x8c657856u;r[3]=r[2];goto L8c657858; /* mov r2,r3 */
L8c657858: *stop=0x8c657858u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65785a; /* mov.l @r3,r1 */
L8c65785a: *stop=0x8c65785au;r[2]=0u;goto L8c65785c; /* mov #0x0,r2 */
L8c65785c: *stop=0x8c65785cu;r[1]<<=2;goto L8c65785e; /* shll2 r1 */
L8c65785e: *stop=0x8c65785eu;r[0]+=4294967272u;goto L8c657860; /* add #-0x18,r0 */
L8c657860: *stop=0x8c657860u;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[2],4);if(io_status)return io_status;goto L8c657862; /* mov.l r2,@(r0,r1) */
L8c657862: *stop=0x8c657862u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657920u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c657864; /* mov.l 0x8c657920,r1 */
L8c657864: *stop=0x8c657864u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[8],4);if(io_status)return io_status;goto L8c657866; /* mov.l r8,@r1 */
L8c657866: *stop=0x8c657866u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657918u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c657868; /* mov.l 0x8c657918,r2 */
L8c657868: *stop=0x8c657868u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65786a; /* mov.l @r2,r3 */
L8c65786a: *stop=0x8c65786au;{uint32_t loaded=rd(m,bus,&io_status,0x8c657924u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65786c; /* mov.l 0x8c657924,r14 */
L8c65786c: *stop=0x8c65786cu;r[8]&=r[3];goto L8c65786e; /* and r3,r8 */
L8c65786e: *stop=0x8c65786eu;r[5]=1u;goto L8c657870; /* mov #0x1,r5 */
L8c657870: *stop=0x8c657870u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c657872; /* mov.l r8,@r15 */
L8c657872: target=r[14];*pr=0x8c657876u;*stop=0x8c657874u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c657876; /* jsr @r14 */
L8c657876: *stop=0x8c657876u;r[5]=0u;goto L8c657878; /* mov #0x0,r5 */
L8c657878: target=r[14];*pr=0x8c65787cu;*stop=0x8c65787au;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65787c; /* jsr @r14 */
L8c65787c: *stop=0x8c65787cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}goto L8c65787e; /* mov.l @r15,r12 */
L8c65787e: *stop=0x8c65787eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657928u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657880; /* mov.l 0x8c657928,r0 */
L8c657880: *stop=0x8c657880u;r[12]<<=2;goto L8c657882; /* shll2 r12 */
L8c657882: *stop=0x8c657882u;{uint32_t loaded=sx(m,bus,&io_status,0x8c657900u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c657884; /* mov.w 0x8c657900,r4 */
L8c657884: *stop=0x8c657884u;r[12]<<=2;goto L8c657886; /* shll2 r12 */
L8c657886: *stop=0x8c657886u;*sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c657888; /* shll r12 */
L8c657888: target=r[14];*pr=0x8c65788cu;*stop=0x8c65788au;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65788c; /* jsr @r14 */
L8c65788c: *stop=0x8c65788cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c657902u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65788e; /* mov.w 0x8c657902,r4 */
L8c65788e: *stop=0x8c65788eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65792cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657890; /* mov.l 0x8c65792c,r0 */
L8c657890: target=r[14];*pr=0x8c657894u;*stop=0x8c657892u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c657894; /* jsr @r14 */
L8c657894: *stop=0x8c657894u;{uint32_t loaded=sx(m,bus,&io_status,0x8c657904u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c657896; /* mov.w 0x8c657904,r4 */
L8c657896: *stop=0x8c657896u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657930u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c657898; /* mov.l 0x8c657930,r0 */
L8c657898: target=r[14];*pr=0x8c65789cu;*stop=0x8c65789au;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65789c; /* jsr @r14 */
L8c65789c: *stop=0x8c65789cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c657906u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65789e; /* mov.w 0x8c657906,r4 */
L8c65789e: *stop=0x8c65789eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657934u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6578a0; /* mov.l 0x8c657934,r0 */
L8c6578a0: target=r[14];*pr=0x8c6578a4u;*stop=0x8c6578a2u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6578a4; /* jsr @r14 */
L8c6578a4: *stop=0x8c6578a4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c657908u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6578a6; /* mov.w 0x8c657908,r4 */
L8c6578a6: *stop=0x8c6578a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657938u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6578a8; /* mov.l 0x8c657938,r0 */
L8c6578a8: target=r[14];*pr=0x8c6578acu;*stop=0x8c6578aau;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6578ac; /* jsr @r14 */
L8c6578ac: *stop=0x8c6578acu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65790au,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6578ae; /* mov.w 0x8c65790a,r4 */
L8c6578ae: *stop=0x8c6578aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65793cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6578b0; /* mov.l 0x8c65793c,r5 */
L8c6578b0: target=r[14];*pr=0x8c6578b4u;*stop=0x8c6578b2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6578b4; /* jsr @r14 */
L8c6578b4: *stop=0x8c6578b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657940u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6578b6; /* mov.l 0x8c657940,r3 */
L8c6578b6: *stop=0x8c6578b6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65790au,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6578b8; /* mov.w 0x8c65790a,r4 */
L8c6578b8: target=r[3];*pr=0x8c6578bcu;*stop=0x8c6578bau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6578bc; /* jsr @r3 */
L8c6578bc: *stop=0x8c6578bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c657944u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6578be; /* mov.l 0x8c657944,r3 */
L8c6578be: *stop=0x8c6578beu;r[2]=0u;goto L8c6578c0; /* mov #0x0,r2 */
L8c6578c0: *stop=0x8c6578c0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6578c2; /* mov.l r2,@r3 */
L8c6578c2: *stop=0x8c6578c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657948u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6578c4; /* mov.l 0x8c657948,r0 */
L8c6578c4: *stop=0x8c6578c4u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6578c6; /* mov.l @r0,r1 */
L8c6578c6: *stop=0x8c6578c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65794cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6578c8; /* mov.l 0x8c65794c,r2 */
L8c6578c8: *stop=0x8c6578c8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6578ca; /* mov.l r1,@r2 */
L8c6578ca: *stop=0x8c6578cau;r[0]=r[13];goto L8c6578cc; /* mov r13,r0 */
L8c6578cc: *stop=0x8c6578ccu;r[15]+=4u;goto L8c6578ce; /* add #0x4,r15 */
L8c6578ce: *stop=0x8c6578ceu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6578d0; /* lds.l @r15+,PR */
L8c6578d0: *stop=0x8c6578d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6578d2; /* mov.l @r15+,r8 */
L8c6578d2: *stop=0x8c6578d2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6578d4; /* mov.l @r15+,r9 */
L8c6578d4: *stop=0x8c6578d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6578d6; /* mov.l @r15+,r10 */
L8c6578d6: *stop=0x8c6578d6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6578d8; /* mov.l @r15+,r11 */
L8c6578d8: *stop=0x8c6578d8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6578da; /* mov.l @r15+,r12 */
L8c6578da: *stop=0x8c6578dau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6578dc; /* mov.l @r15+,r13 */
L8c6578dc: *stop=0x8c6578dcu;if(*pr!=entry_pr)return -3;*stop=0x8c6578deu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c6634a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6634a0;
L8c6634a0: *stop=0x8c6634a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6634a2; /* mov.l r14,@-r15 */
L8c6634a2: *stop=0x8c6634a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6634a4; /* mov.l r13,@-r15 */
L8c6634a4: *stop=0x8c6634a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6634a6; /* mov.l r12,@-r15 */
L8c6634a6: *stop=0x8c6634a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6634a8; /* mov.l r8,@-r15 */
L8c6634a8: *stop=0x8c6634a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6634aa; /* sts.l PR,@-r15 */
L8c6634aa: *stop=0x8c6634aau;r[15]+=4294967292u;goto L8c6634ac; /* add #-0x4,r15 */
L8c6634ac: *stop=0x8c6634acu;r[14]=r[4];goto L8c6634ae; /* mov r4,r14 */
L8c6634ae: *stop=0x8c6634aeu;r[0]=r[14];goto L8c6634b0; /* mov r14,r0 */
L8c6634b0: *stop=0x8c6634b0u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c6634b2; /* cmp/pz r0 */
L8c6634b2: *stop=0x8c6634b2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c6634b4; /* mov.l r6,@r15 */
L8c6634b4: take=!(*sr&1u);*stop=0x8c6634b6u;r[13]=r[5];if(take)goto L8c6634bc;goto L8c6634b8; /* bf/s 0x8c6634bc */
L8c6634b8: *stop=0x8c6634bau;r[0]&=0x1fu;goto L8c6634c6; /* bra 0x8c6634c6 */
L8c6634bc: *stop=0x8c6634bcu;r[0]=~r[0];goto L8c6634be; /* not r0,r0 */
L8c6634be: *stop=0x8c6634beu;r[0]+=1u;goto L8c6634c0; /* add #0x1,r0 */
L8c6634c0: *stop=0x8c6634c0u;r[0]&=0x1fu;goto L8c6634c2; /* and #0x1f,r0 */
L8c6634c2: *stop=0x8c6634c2u;r[0]=~r[0];goto L8c6634c4; /* not r0,r0 */
L8c6634c4: *stop=0x8c6634c4u;r[0]+=1u;goto L8c6634c6; /* add #0x1,r0 */
L8c6634c6: *stop=0x8c6634c6u;r[4]=r[0];goto L8c6634c8; /* mov r0,r4 */
L8c6634c8: *stop=0x8c6634c8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6634ca; /* tst r4,r4 */
L8c6634ca: if((*sr&1u))goto L8c6634d2;goto L8c6634cc; /* bt 0x8c6634d2 */
L8c6634cc: *stop=0x8c6634ccu;r[3]=32u;goto L8c6634ce; /* mov #0x20,r3 */
L8c6634ce: *stop=0x8c6634ceu;r[3]-=r[4];goto L8c6634d0; /* sub r4,r3 */
L8c6634d0: *stop=0x8c6634d0u;r[14]+=r[3];goto L8c6634d2; /* add r3,r14 */
L8c6634d2: *stop=0x8c6634d2u;r[5]=r[14];goto L8c6634d4; /* mov r14,r5 */
L8c6634d4: *stop=0x8c6634d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663524u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6634d6; /* mov.l 0x8c663524,r2 */
L8c6634d6: *stop=0x8c6634d6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c663520u,2);if(io_status)return io_status;r[7]=loaded;}goto L8c6634d8; /* mov.w 0x8c663520,r7 */
L8c6634d8: *stop=0x8c6634d8u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6634da; /* shll r5 */
L8c6634da: *stop=0x8c6634dau;r[6]=r[13];goto L8c6634dc; /* mov r13,r6 */
L8c6634dc: target=r[2];*pr=0x8c6634e0u;*stop=0x8c6634deu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6634e0; /* jsr @r2 */
L8c6634e0: *stop=0x8c6634e0u;r[14]=r[0];goto L8c6634e2; /* mov r0,r14 */
L8c6634e2: *stop=0x8c6634e2u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6634e4; /* tst r14,r14 */
L8c6634e4: if(!(*sr&1u))goto L8c6634fc;goto L8c6634e6; /* bf 0x8c6634fc */
L8c6634e6: *stop=0x8c6634e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663528u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6634e8; /* mov.l 0x8c663528,r2 */
L8c6634e8: *stop=0x8c6634e8u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[12]=loaded;}goto L8c6634ea; /* mov.l @r13,r12 */
L8c6634ea: target=r[2];*pr=0x8c6634eeu;*stop=0x8c6634ecu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6634ee; /* jsr @r2 */
L8c6634ee: *stop=0x8c6634eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663528u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6634f0; /* mov.l 0x8c663528,r3 */
L8c6634f0: *stop=0x8c6634f0u;r[4]=r[12];goto L8c6634f2; /* mov r12,r4 */
L8c6634f2: *stop=0x8c6634f2u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[0],4);if(io_status)return io_status;goto L8c6634f4; /* mov.l r0,@r13 */
L8c6634f4: *stop=0x8c6634f4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}goto L8c6634f6; /* mov.l @r15,r8 */
L8c6634f6: target=r[3];*pr=0x8c6634fau;*stop=0x8c6634f8u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6634fa; /* jsr @r3 */
L8c6634fa: *stop=0x8c6634fau;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c6634fc; /* mov.l r0,@r8 */
L8c6634fc: *stop=0x8c6634fcu;r[0]=r[14];goto L8c6634fe; /* mov r14,r0 */
L8c6634fe: *stop=0x8c6634feu;r[15]+=4u;goto L8c663500; /* add #0x4,r15 */
L8c663500: *stop=0x8c663500u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c663502; /* lds.l @r15+,PR */
L8c663502: *stop=0x8c663502u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c663504; /* mov.l @r15+,r8 */
L8c663504: *stop=0x8c663504u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c663506; /* mov.l @r15+,r12 */
L8c663506: *stop=0x8c663506u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c663508; /* mov.l @r15+,r13 */
L8c663508: *stop=0x8c663508u;if(*pr!=entry_pr)return -3;*stop=0x8c66350au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c665cc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c665cc0;
L8c665cc0: *stop=0x8c665cc0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c665cc2; /* mov.l r14,@-r15 */
L8c665cc2: *stop=0x8c665cc2u;r[15]+=4294967272u;goto L8c665cc4; /* add #-0x18,r15 */
L8c665cc4: *stop=0x8c665cc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665d68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665cc6; /* mov.l 0x8c665d68,r3 */
L8c665cc6: *stop=0x8c665cc6u;r[2]=r[15];goto L8c665cc8; /* mov r15,r2 */
L8c665cc8: *stop=0x8c665cc8u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665cca; /* mov.l @(0x4,r3),r0 */
L8c665cca: *stop=0x8c665ccau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c665ccc; /* mov.l @r3,r1 */
L8c665ccc: *stop=0x8c665cccu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c665cce; /* mov.l r1,@r2 */
L8c665cce: *stop=0x8c665cceu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c665cd0; /* mov.l r0,@(0x4,r2) */
L8c665cd0: *stop=0x8c665cd0u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665cd2; /* mov.l @(0xc,r3),r0 */
L8c665cd2: *stop=0x8c665cd2u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665cd4; /* mov.l @(0x8,r3),r1 */
L8c665cd4: *stop=0x8c665cd4u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c665cd6; /* mov.l r1,@(0x8,r2) */
L8c665cd6: *stop=0x8c665cd6u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[0],4);if(io_status)return io_status;goto L8c665cd8; /* mov.l r0,@(0xc,r2) */
L8c665cd8: *stop=0x8c665cd8u;if(!valid_access(r[3]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665cda; /* mov.l @(0x14,r3),r0 */
L8c665cda: *stop=0x8c665cdau;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665cdc; /* mov.l @(0x10,r3),r1 */
L8c665cdc: *stop=0x8c665cdcu;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[1],4);if(io_status)return io_status;goto L8c665cde; /* mov.l r1,@(0x10,r2) */
L8c665cde: *stop=0x8c665cdeu;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[0],4);if(io_status)return io_status;goto L8c665ce0; /* mov.l r0,@(0x14,r2) */
L8c665ce0: *stop=0x8c665ce0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665d6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665ce2; /* mov.l 0x8c665d6c,r2 */
L8c665ce2: *stop=0x8c665ce2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665ce4; /* mov.l @r2,r0 */
L8c665ce4: *stop=0x8c665ce4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c665ce6; /* cmp/eq #0x1,r0 */
L8c665ce6: take=(*sr&1u);*stop=0x8c665ce8u;r[14]=r[15];if(take)goto L8c665cf2;goto L8c665cea; /* bt/s 0x8c665cf2 */
L8c665cea: *stop=0x8c665ceau;{uint32_t loaded=rd(m,bus,&io_status,0x8c665d70u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665cec; /* mov.l 0x8c665d70,r1 */
L8c665cec: *stop=0x8c665cecu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665cee; /* mov.l @r1,r0 */
L8c665cee: *stop=0x8c665ceeu;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c665cf0; /* tst #0x10,r0 */
L8c665cf0: if((*sr&1u))goto L8c665cf8;goto L8c665cf2; /* bt 0x8c665cf8 */
L8c665cf2: *stop=0x8c665cf2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665d60u,2);if(io_status)return io_status;r[7]=loaded;}goto L8c665cf4; /* mov.w 0x8c665d60,r7 */
L8c665cf4: *stop=0x8c665cf6u;goto L8c665cfa; /* bra 0x8c665cfa */
L8c665cf8: *stop=0x8c665cf8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665d62u,2);if(io_status)return io_status;r[7]=loaded;}goto L8c665cfa; /* mov.w 0x8c665d62,r7 */
L8c665cfa: *stop=0x8c665cfau;r[0]=r[5];goto L8c665cfc; /* mov r5,r0 */
L8c665cfc: *stop=0x8c665cfcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c665cfe; /* cmp/eq #0x1,r0 */
L8c665cfe: if(!(*sr&1u))goto L8c665d28;goto L8c665d00; /* bf 0x8c665d28 */
L8c665d00: *stop=0x8c665d00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665d78u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665d02; /* mov.l 0x8c665d78,r0 */
L8c665d02: *stop=0x8c665d02u;r[6]+=31u;goto L8c665d04; /* add #0x1f,r6 */
L8c665d04: *stop=0x8c665d04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665d74u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665d06; /* mov.l 0x8c665d74,r3 */
L8c665d06: *stop=0x8c665d06u;r[2]=4294967264u;goto L8c665d08; /* mov #-0x20,r2 */
L8c665d08: *stop=0x8c665d08u;r[6]&=r[2];goto L8c665d0a; /* and r2,r6 */
L8c665d0a: *stop=0x8c665d0au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c665d0c; /* mov.l @r0,r1 */
L8c665d0c: *stop=0x8c665d0cu;r[0]=r[6];goto L8c665d0e; /* mov r6,r0 */
L8c665d0e: *stop=0x8c665d0eu;r[0]>>=2;goto L8c665d10; /* shlr2 r0 */
L8c665d10: *stop=0x8c665d10u;r[0]>>=2;goto L8c665d12; /* shlr2 r0 */
L8c665d12: *stop=0x8c665d12u;*sr=(*sr&~1u)|(r[0]&1u);r[0]>>=1;goto L8c665d14; /* shlr r0 */
L8c665d14: *stop=0x8c665d14u;r[2]=22u;goto L8c665d16; /* mov #0x16,r2 */
L8c665d16: *stop=0x8c665d16u;r[0]&=0x1fu;goto L8c665d18; /* and #0x1f,r0 */
L8c665d18: *stop=0x8c665d18u;r[5]=(r[2]&0x80000000u)?((r[2]&31u)?r[5]>>((0u-r[2])&31u):0u):(r[5]<<(r[2]&31u));goto L8c665d1a; /* shld r2,r5 */
L8c665d1a: *stop=0x8c665d1au;r[1]&=r[3];goto L8c665d1c; /* and r3,r1 */
L8c665d1c: *stop=0x8c665d1cu;r[0]<<=16;goto L8c665d1e; /* shll16 r0 */
L8c665d1e: *stop=0x8c665d1eu;r[1]|=r[5];goto L8c665d20; /* or r5,r1 */
L8c665d20: *stop=0x8c665d20u;r[1]|=r[0];goto L8c665d22; /* or r0,r1 */
L8c665d22: *stop=0x8c665d22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665d78u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665d24; /* mov.l 0x8c665d78,r0 */
L8c665d24: *stop=0x8c665d26u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c665d90; /* bra 0x8c665d90 */
L8c665d28: *stop=0x8c665d28u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665d6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665d2a; /* mov.l 0x8c665d6c,r2 */
L8c665d2a: *stop=0x8c665d2au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665d2c; /* mov.l @r2,r0 */
L8c665d2c: *stop=0x8c665d2cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c665d2e; /* cmp/eq #0x1,r0 */
L8c665d2e: if((*sr&1u))goto L8c665d88;goto L8c665d30; /* bt 0x8c665d88 */
L8c665d30: *stop=0x8c665d30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665d70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665d32; /* mov.l 0x8c665d70,r3 */
L8c665d32: *stop=0x8c665d32u;r[5]=1u;goto L8c665d34; /* mov #0x1,r5 */
L8c665d34: *stop=0x8c665d34u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665d36; /* mov.l @r3,r0 */
L8c665d36: *stop=0x8c665d36u;r[0]&=r[5];goto L8c665d38; /* and r5,r0 */
L8c665d38: *stop=0x8c665d38u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c665d3a; /* cmp/eq #0x1,r0 */
L8c665d3a: if((*sr&1u))goto L8c665d80;goto L8c665d3c; /* bt 0x8c665d80 */
L8c665d3c: *stop=0x8c665d3cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c665d70u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665d3e; /* mov.l 0x8c665d70,r1 */
L8c665d3e: *stop=0x8c665d3eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665d40; /* mov.l @r1,r0 */
L8c665d40: *stop=0x8c665d40u;r[0]&=0x2cu;goto L8c665d42; /* and #0x2c,r0 */
L8c665d42: *stop=0x8c665d42u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c665d44; /* cmp/eq #0x20,r0 */
L8c665d44: if(!(*sr&1u))goto L8c665d80;goto L8c665d46; /* bf 0x8c665d80 */
L8c665d46: *stop=0x8c665d46u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c665d48; /* tst r5,r5 */
L8c665d48: if((*sr&1u))goto L8c665d80;goto L8c665d4a; /* bt 0x8c665d80 */
L8c665d4a: *stop=0x8c665d4au;{uint32_t loaded=sx(m,bus,&io_status,0x8c665d64u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c665d4c; /* mov.w 0x8c665d64,r6 */
L8c665d4c: *stop=0x8c665d4eu;goto L8c665d90; /* bra 0x8c665d90 */
L8c665d80: *stop=0x8c665d80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665de8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665d82; /* mov.l 0x8c665de8,r2 */
L8c665d82: *stop=0x8c665d82u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665d84; /* mov.l @r2,r0 */
L8c665d84: *stop=0x8c665d84u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c665d86; /* tst #0x20,r0 */
L8c665d86: if((*sr&1u))goto L8c665d8e;goto L8c665d88; /* bt 0x8c665d8e */
L8c665d88: *stop=0x8c665d88u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665de0u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c665d8a; /* mov.w 0x8c665de0,r6 */
L8c665d8a: *stop=0x8c665d8cu;goto L8c665d90; /* bra 0x8c665d90 */
L8c665d8e: *stop=0x8c665d8eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c665de2u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c665d90; /* mov.w 0x8c665de2,r6 */
L8c665d90: *stop=0x8c665d90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665decu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665d92; /* mov.l 0x8c665dec,r5 */
L8c665d92: *stop=0x8c665d92u;r[0]=72u;goto L8c665d94; /* mov #0x48,r0 */
L8c665d94: *stop=0x8c665d94u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665de4u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c665d96; /* mov.w 0x8c665de4,r3 */
L8c665d96: *stop=0x8c665d96u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[7],4);if(io_status)return io_status;goto L8c665d98; /* mov.l r7,@(r0,r5) */
L8c665d98: *stop=0x8c665d98u;r[0]=76u;goto L8c665d9a; /* mov #0x4c,r0 */
L8c665d9a: *stop=0x8c665d9au;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c665d9c; /* cmp/eq r3,r4 */
L8c665d9c: take=(*sr&1u);*stop=0x8c665d9eu;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[6],4);if(io_status)return io_status;if(take)goto L8c665da4;goto L8c665da0; /* bt/s 0x8c665da4 */
L8c665da0: *stop=0x8c665da2u;r[0]=r[4];goto L8c665da8; /* bra 0x8c665da8 */
L8c665da4: *stop=0x8c665da4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665df0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665da6; /* mov.l 0x8c665df0,r1 */
L8c665da6: *stop=0x8c665da6u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665da8; /* mov.l @r1,r0 */
L8c665da8: *stop=0x8c665da8u;r[0]<<=2;goto L8c665daa; /* shll2 r0 */
L8c665daa: *stop=0x8c665daau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665dac; /* mov.l @(r0,r14),r2 */
L8c665dac: *stop=0x8c665dacu;r[0]=80u;goto L8c665dae; /* mov #0x50,r0 */
L8c665dae: *stop=0x8c665daeu;r[2]>>=2;goto L8c665db0; /* shlr2 r2 */
L8c665db0: *stop=0x8c665db0u;*sr=(*sr&~1u)|(r[2]&1u);r[2]>>=1;goto L8c665db2; /* shlr r2 */
L8c665db2: *stop=0x8c665db2u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[2],4);if(io_status)return io_status;goto L8c665db4; /* mov.l r2,@(r0,r5) */
L8c665db4: *stop=0x8c665db4u;r[0]=76u;goto L8c665db6; /* mov #0x4c,r0 */
L8c665db6: *stop=0x8c665db6u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c665db8; /* mov.l @(r0,r5),r3 */
L8c665db8: *stop=0x8c665db8u;r[0]=72u;goto L8c665dba; /* mov #0x48,r0 */
L8c665dba: *stop=0x8c665dbau;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665dbc; /* mov.l @(r0,r5),r2 */
L8c665dbc: *stop=0x8c665dbcu;r[0]=80u;goto L8c665dbe; /* mov #0x50,r0 */
L8c665dbe: *stop=0x8c665dbeu;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c665dc0; /* mov.l @(r0,r5),r1 */
L8c665dc0: *stop=0x8c665dc0u;r[0]=96u;goto L8c665dc2; /* mov #0x60,r0 */
L8c665dc2: *stop=0x8c665dc2u;*macl=r[3]*r[2];goto L8c665dc4; /* mul.l r3,r2 */
L8c665dc4: *stop=0x8c665dc4u;r[3]=*macl;goto L8c665dc6; /* sts MACL,r3 */
L8c665dc6: *stop=0x8c665dc6u;*macl=r[1]*r[3];goto L8c665dc8; /* mul.l r1,r3 */
L8c665dc8: *stop=0x8c665dc8u;r[3]=*macl;goto L8c665dca; /* sts MACL,r3 */
L8c665dca: *stop=0x8c665dcau;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[3],4);if(io_status)return io_status;goto L8c665dcc; /* mov.l r3,@(r0,r5) */
L8c665dcc: *stop=0x8c665dccu;r[15]+=24u;goto L8c665dce; /* add #0x18,r15 */
L8c665dce: *stop=0x8c665dceu;if(*pr!=entry_pr)return -3;*stop=0x8c665dd0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c641c16(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c641c16;
L8c641c16: *stop=0x8c641c16u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c641c18; /* mov.l r14,@-r15 */
L8c641c18: *stop=0x8c641c18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c641c1a; /* mov.l r13,@-r15 */
L8c641c1a: *stop=0x8c641c1au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c641c1c; /* mov.l r12,@-r15 */
L8c641c1c: *stop=0x8c641c1cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c641c1e; /* mov.l r11,@-r15 */
L8c641c1e: *stop=0x8c641c1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c641c20; /* mov.l r10,@-r15 */
L8c641c20: *stop=0x8c641c20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c641c22; /* mov.l r9,@-r15 */
L8c641c22: *stop=0x8c641c22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c641c24; /* sts.l PR,@-r15 */
L8c641c24: *stop=0x8c641c24u;r[15]+=4294967292u;goto L8c641c26; /* add #-0x4,r15 */
L8c641c26: *stop=0x8c641c26u;r[14]=r[4];goto L8c641c28; /* mov r4,r14 */
L8c641c28: *stop=0x8c641c28u;r[10]=0u;goto L8c641c2a; /* mov #0x0,r10 */
L8c641c2a: *stop=0x8c641c2au;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c641c2c; /* tst r14,r14 */
L8c641c2c: *stop=0x8c641c2cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c641c2e; /* mov.l r7,@r15 */
L8c641c2e: *stop=0x8c641c2eu;r[13]=r[5];goto L8c641c30; /* mov r5,r13 */
L8c641c30: *stop=0x8c641c30u;r[11]=r[6];goto L8c641c32; /* mov r6,r11 */
L8c641c32: take=(*sr&1u);*stop=0x8c641c34u;r[9]=r[10];if(take)goto L8c641c3a;goto L8c641c36; /* bt/s 0x8c641c3a */
L8c641c36: *stop=0x8c641c36u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c641c38; /* tst r13,r13 */
L8c641c38: if(!(*sr&1u))goto L8c641c3e;goto L8c641c3a; /* bf 0x8c641c3e */
L8c641c3a: *stop=0x8c641c3cu;r[0]=14u;goto L8c641dc2; /* bra 0x8c641dc2 */
L8c641c3e: *stop=0x8c641c3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ca0u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c641c40; /* mov.l 0x8c641ca0,r12 */
L8c641c40: *stop=0x8c641c40u;r[0]=64u;goto L8c641c42; /* mov #0x40,r0 */
L8c641c42: *stop=0x8c641c42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ca8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641c44; /* mov.l 0x8c641ca8,r2 */
L8c641c44: *stop=0x8c641c44u;if(!valid_access(r[12]+60u,4u))return -3;wr(m,bus,&io_status,r[12]+0x3cu,r[14],4);if(io_status)return io_status;goto L8c641c46; /* mov.l r14,@(0x3c,r12) */
L8c641c46: *stop=0x8c641c46u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[13],4);if(io_status)return io_status;goto L8c641c48; /* mov.l r13,@(r0,r12) */
L8c641c48: *stop=0x8c641c48u;r[0]=68u;goto L8c641c4a; /* mov #0x44,r0 */
L8c641c4a: *stop=0x8c641c4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641c4c; /* mov.l @r15,r3 */
L8c641c4c: *stop=0x8c641c4cu;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[3],4);if(io_status)return io_status;goto L8c641c4e; /* mov.l r3,@(r0,r12) */
L8c641c4e: *stop=0x8c641c4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ca4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641c50; /* mov.l 0x8c641ca4,r3 */
L8c641c50: *stop=0x8c641c50u;if(!valid_access(r[12]+24u,4u))return -3;wr(m,bus,&io_status,r[12]+0x18u,r[11],4);if(io_status)return io_status;goto L8c641c52; /* mov.l r11,@(0x18,r12) */
L8c641c52: target=r[2];*pr=0x8c641c56u;*stop=0x8c641c54u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[11],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641c56; /* jsr @r2 */
L8c641c56: *stop=0x8c641c56u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641cacu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641c58; /* mov.l 0x8c641cac,r1 */
L8c641c58: *stop=0x8c641c58u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c641c5a; /* cmp/eq r1,r0 */
L8c641c5a: if((*sr&1u))goto L8c641c60;goto L8c641c5c; /* bt 0x8c641c60 */
L8c641c5c: *stop=0x8c641c5eu;goto L8c641da0; /* bra 0x8c641da0 */
L8c641c60: *stop=0x8c641c60u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641c62; /* mov.l @(0x18,r14),r3 */
L8c641c62: *stop=0x8c641c62u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641c64; /* mov.l @(0x18,r13),r2 */
L8c641c64: *stop=0x8c641c64u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c641c66; /* cmp/eq r2,r3 */
L8c641c66: if((*sr&1u))goto L8c641cc0;goto L8c641c68; /* bt 0x8c641cc0 */
L8c641c68: *stop=0x8c641c6au;r[0]=15u;goto L8c641dc2; /* bra 0x8c641dc2 */
L8c641cc0: *stop=0x8c641cc0u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641cc2; /* mov.l @(0x14,r14),r2 */
L8c641cc2: *stop=0x8c641cc2u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641cc4; /* mov.l @(0x14,r13),r3 */
L8c641cc4: *stop=0x8c641cc4u;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c641cc6; /* cmp/eq r3,r2 */
L8c641cc6: if((*sr&1u))goto L8c641ccc;goto L8c641cc8; /* bt 0x8c641ccc */
L8c641cc8: *stop=0x8c641ccau;r[0]=16u;goto L8c641dc2; /* bra 0x8c641dc2 */
L8c641ccc: *stop=0x8c641cccu;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641cce; /* mov.l @(0x1c,r14),r2 */
L8c641cce: *stop=0x8c641cceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641cd0; /* mov.l 0x8c641d60,r3 */
L8c641cd0: *stop=0x8c641cd0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c641cd2; /* mov.l r2,@r3 */
L8c641cd2: *stop=0x8c641cd2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d64u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641cd4; /* mov.l 0x8c641d64,r1 */
L8c641cd4: *stop=0x8c641cd4u;r[2]=0u;goto L8c641cd6; /* mov #0x0,r2 */
L8c641cd6: *stop=0x8c641cd6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c641cd8; /* mov.l r2,@r1 */
L8c641cd8: *stop=0x8c641cd8u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641cda; /* mov.l @(0x1c,r13),r3 */
L8c641cda: *stop=0x8c641cdau;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d68u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641cdc; /* mov.l 0x8c641d68,r0 */
L8c641cdc: *stop=0x8c641cdcu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c641cde; /* mov.l r3,@r0 */
L8c641cde: *stop=0x8c641cdeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641ce0; /* mov.l 0x8c641d6c,r2 */
L8c641ce0: *stop=0x8c641ce0u;r[3]=0u;goto L8c641ce2; /* mov #0x0,r3 */
L8c641ce2: *stop=0x8c641ce2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c641ce4; /* mov.l r3,@r2 */
L8c641ce4: *stop=0x8c641ce4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d70u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c641ce6; /* mov.l 0x8c641d70,r14 */
L8c641ce6: *stop=0x8c641ce6u;r[1]+=4294967188u;goto L8c641ce8; /* add #-0x6c,r1 */
L8c641ce8: *stop=0x8c641ce8u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[5]=loaded;}goto L8c641cea; /* mov.l @r1,r5 */
L8c641cea: target=r[14];*pr=0x8c641ceeu;*stop=0x8c641cecu;r[4]=80u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641cee; /* jsr @r14 */
L8c641cee: *stop=0x8c641ceeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d74u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641cf0; /* mov.l 0x8c641d74,r3 */
L8c641cf0: *stop=0x8c641cf0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c641cf2; /* mov.l @r3,r5 */
L8c641cf2: target=r[14];*pr=0x8c641cf6u;*stop=0x8c641cf4u;r[4]=84u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641cf6; /* jsr @r14 */
L8c641cf6: *stop=0x8c641cf6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d60u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c641cf8; /* mov.l 0x8c641d60,r5 */
L8c641cf8: *stop=0x8c641cf8u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c641cfa; /* mov.l @r5,r5 */
L8c641cfa: target=r[14];*pr=0x8c641cfeu;*stop=0x8c641cfcu;r[4]=96u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641cfe; /* jsr @r14 */
L8c641cfe: *stop=0x8c641cfeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d68u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c641d00; /* mov.l 0x8c641d68,r5 */
L8c641d00: *stop=0x8c641d00u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c641d02; /* mov.l @r5,r5 */
L8c641d02: target=r[14];*pr=0x8c641d06u;*stop=0x8c641d04u;r[4]=100u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641d06; /* jsr @r14 */
L8c641d06: *stop=0x8c641d06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d60u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641d08; /* mov.l 0x8c641d60,r2 */
L8c641d08: *stop=0x8c641d08u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641d0a; /* mov.l @r2,r3 */
L8c641d0a: *stop=0x8c641d0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d74u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641d0c; /* mov.l 0x8c641d74,r1 */
L8c641d0c: *stop=0x8c641d0cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c641d0e; /* mov.l r3,@r1 */
L8c641d0e: *stop=0x8c641d0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d68u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641d10; /* mov.l 0x8c641d68,r0 */
L8c641d10: *stop=0x8c641d10u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641d12; /* mov.l @r0,r3 */
L8c641d12: *stop=0x8c641d12u;r[2]+=4294967200u;goto L8c641d14; /* add #-0x60,r2 */
L8c641d14: *stop=0x8c641d14u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c641d16; /* mov.l r3,@r2 */
L8c641d16: *stop=0x8c641d16u;r[0]=r[11];goto L8c641d18; /* mov r11,r0 */
L8c641d18: *stop=0x8c641d18u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c641d1a; /* cmp/eq #0x1,r0 */
L8c641d1a: if(!(*sr&1u))goto L8c641d32;goto L8c641d1c; /* bf 0x8c641d32 */
L8c641d1c: *stop=0x8c641d1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d78u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641d1e; /* mov.l 0x8c641d78,r1 */
L8c641d1e: *stop=0x8c641d1eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[10],4);if(io_status)return io_status;goto L8c641d20; /* mov.l r10,@r1 */
L8c641d20: *stop=0x8c641d20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d7cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641d22; /* mov.l 0x8c641d7c,r3 */
L8c641d22: *stop=0x8c641d22u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[10],4);if(io_status)return io_status;goto L8c641d24; /* mov.l r10,@r3 */
L8c641d24: *stop=0x8c641d24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641d26; /* mov.l 0x8c641d74,r2 */
L8c641d26: *stop=0x8c641d26u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c641d28; /* mov.l @r2,r5 */
L8c641d28: target=r[14];*pr=0x8c641d2cu;*stop=0x8c641d2au;r[4]=80u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641d2c; /* jsr @r14 */
L8c641d2c: *stop=0x8c641d2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d80u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641d2e; /* mov.l 0x8c641d80,r3 */
L8c641d2e: *stop=0x8c641d30u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c641d3e; /* bra 0x8c641d3e */
L8c641d32: *stop=0x8c641d32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d74u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641d34; /* mov.l 0x8c641d74,r3 */
L8c641d34: *stop=0x8c641d34u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c641d36; /* mov.l @r3,r5 */
L8c641d36: target=r[14];*pr=0x8c641d3au;*stop=0x8c641d38u;r[4]=80u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641d3a; /* jsr @r14 */
L8c641d3a: *stop=0x8c641d3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641d3c; /* mov.l 0x8c641d74,r2 */
L8c641d3c: *stop=0x8c641d3cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c641d3e; /* mov.l @r2,r5 */
L8c641d3e: target=r[14];*pr=0x8c641d42u;*stop=0x8c641d40u;r[4]=84u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641d42; /* jsr @r14 */
L8c641d42: *stop=0x8c641d42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d84u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641d44; /* mov.l 0x8c641d84,r2 */
L8c641d44: *stop=0x8c641d44u;r[3]=1u;goto L8c641d46; /* mov #0x1,r3 */
L8c641d46: *stop=0x8c641d46u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c641d48; /* mov.l r3,@r2 */
L8c641d48: *stop=0x8c641d48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641d88u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641d4a; /* mov.l 0x8c641d88,r3 */
L8c641d4a: *stop=0x8c641d4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c641d4c; /* mov.l @r15,r1 */
L8c641d4c: *stop=0x8c641d4cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c641d4e; /* mov.l r1,@r3 */
L8c641d4e: *stop=0x8c641d50u;goto L8c641dc0; /* bra 0x8c641dc0 */
L8c641da0: *stop=0x8c641da0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641de4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641da2; /* mov.l 0x8c641de4,r3 */
L8c641da2: *stop=0x8c641da2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c641de0u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c641da4; /* mov.w 0x8c641de0,r4 */
L8c641da4: target=r[3];*pr=0x8c641da8u;*stop=0x8c641da6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641da8; /* jsr @r3 */
L8c641da8: *stop=0x8c641da8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c641de2u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c641daa; /* mov.w 0x8c641de2,r2 */
L8c641daa: *stop=0x8c641daau;*sr=(*sr&~1u)|(r[2]==r[0]);goto L8c641dac; /* cmp/eq r2,r0 */
L8c641dac: if(!(*sr&1u))goto L8c641dc0;goto L8c641dae; /* bf 0x8c641dc0 */
L8c641dae: *stop=0x8c641daeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641decu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641db0; /* mov.l 0x8c641dec,r1 */
L8c641db0: *stop=0x8c641db0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641de8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c641db2; /* mov.l 0x8c641de8,r4 */
L8c641db2: target=r[1];*pr=0x8c641db6u;*stop=0x8c641db4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641db6; /* jsr @r1 */
L8c641db6: *stop=0x8c641db6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641df0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641db8; /* mov.l 0x8c641df0,r3 */
L8c641db8: *stop=0x8c641db8u;r[0]=84u;goto L8c641dba; /* mov #0x54,r0 */
L8c641dba: target=r[3];*pr=0x8c641dbeu;*stop=0x8c641dbcu;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641dbe; /* jsr @r3 */
L8c641dbe: *stop=0x8c641dbeu;r[9]=r[0];goto L8c641dc0; /* mov r0,r9 */
L8c641dc0: *stop=0x8c641dc0u;r[0]=r[9];goto L8c641dc2; /* mov r9,r0 */
L8c641dc2: *stop=0x8c641dc2u;r[15]+=4u;goto L8c641dc4; /* add #0x4,r15 */
L8c641dc4: *stop=0x8c641dc4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c641dc6; /* lds.l @r15+,PR */
L8c641dc6: *stop=0x8c641dc6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c641dc8; /* mov.l @r15+,r9 */
L8c641dc8: *stop=0x8c641dc8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c641dca; /* mov.l @r15+,r10 */
L8c641dca: *stop=0x8c641dcau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c641dcc; /* mov.l @r15+,r11 */
L8c641dcc: *stop=0x8c641dccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c641dce; /* mov.l @r15+,r12 */
L8c641dce: *stop=0x8c641dceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c641dd0; /* mov.l @r15+,r13 */
L8c641dd0: *stop=0x8c641dd0u;if(*pr!=entry_pr)return -3;*stop=0x8c641dd2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c65614e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65614e;
L8c65614e: *stop=0x8c65614eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c656150; /* mov.l r14,@-r15 */
L8c656150: *stop=0x8c656150u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c656152; /* mov.l r13,@-r15 */
L8c656152: *stop=0x8c656152u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c656154; /* mov.l r12,@-r15 */
L8c656154: *stop=0x8c656154u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c656156; /* mov.l r11,@-r15 */
L8c656156: *stop=0x8c656156u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c656158; /* mov.l r10,@-r15 */
L8c656158: *stop=0x8c656158u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65615a; /* mov.l r9,@-r15 */
L8c65615a: *stop=0x8c65615au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65615c; /* mov.l r8,@-r15 */
L8c65615c: *stop=0x8c65615cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65615e; /* sts.l PR,@-r15 */
L8c65615e: *stop=0x8c65615eu;r[15]+=4294967248u;goto L8c656160; /* add #-0x30,r15 */
L8c656160: *stop=0x8c656160u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6561e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656162; /* mov.l 0x8c6561e8,r3 */
L8c656162: *stop=0x8c656162u;r[2]=r[15];goto L8c656164; /* mov r15,r2 */
L8c656164: *stop=0x8c656164u;r[2]+=16u;goto L8c656166; /* add #0x10,r2 */
L8c656166: *stop=0x8c656166u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6561ecu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c656168; /* mov.l 0x8c6561ec,r14 */
L8c656168: *stop=0x8c656168u;r[13]=0u;goto L8c65616a; /* mov #0x0,r13 */
L8c65616a: *stop=0x8c65616au;r[9]=r[5];goto L8c65616c; /* mov r5,r9 */
L8c65616c: *stop=0x8c65616cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c65616e; /* mov.l r6,@r15 */
L8c65616e: *stop=0x8c65616eu;r[10]=r[4];goto L8c656170; /* mov r4,r10 */
L8c656170: *stop=0x8c656170u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[7],4);if(io_status)return io_status;goto L8c656172; /* mov.l r7,@(0x4,r15) */
L8c656172: *stop=0x8c656172u;r[8]=r[13];goto L8c656174; /* mov r13,r8 */
L8c656174: *stop=0x8c656174u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656176; /* mov.l @(0x4,r3),r0 */
L8c656176: *stop=0x8c656176u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c656178; /* mov.l @r3,r1 */
L8c656178: *stop=0x8c656178u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c65617a; /* mov.l r1,@r2 */
L8c65617a: *stop=0x8c65617au;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c65617c; /* mov.l r0,@(0x4,r2) */
L8c65617c: *stop=0x8c65617cu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65617e; /* mov.l @(0x8,r3),r1 */
L8c65617e: *stop=0x8c65617eu;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656180; /* mov.l @(0xc,r3),r0 */
L8c656180: *stop=0x8c656180u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c656182; /* mov.l r1,@(0x8,r2) */
L8c656182: *stop=0x8c656182u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[0],4);if(io_status)return io_status;goto L8c656184; /* mov.l r0,@(0xc,r2) */
L8c656184: *stop=0x8c656184u;r[2]=1u;goto L8c656186; /* mov #0x1,r2 */
L8c656186: *stop=0x8c656186u;r[0]=84u;goto L8c656188; /* mov #0x54,r0 */
L8c656188: *stop=0x8c656188u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6561f0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65618a; /* mov.l 0x8c6561f0,r1 */
L8c65618a: *stop=0x8c65618au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c65618c; /* mov.l r2,@(r0,r14) */
L8c65618c: *stop=0x8c65618cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65618e; /* mov.l @r1,r0 */
L8c65618e: *stop=0x8c65618eu;*sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c656190; /* tst #0x40,r0 */
L8c656190: take=(*sr&1u);*stop=0x8c656192u;r[12]=2u;if(take)goto L8c656196;goto L8c656194; /* bt/s 0x8c656196 */
L8c656194: *stop=0x8c656194u;r[9]+=32u;goto L8c656196; /* add #0x20,r9 */
L8c656196: *stop=0x8c656196u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6561f4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656198; /* mov.l 0x8c6561f4,r4 */
L8c656198: *stop=0x8c656198u;r[0]=80u;goto L8c65619a; /* mov #0x50,r0 */
L8c65619a: *stop=0x8c65619au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6561f8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65619c; /* mov.l 0x8c6561f8,r5 */
L8c65619c: *stop=0x8c65619cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[10],4);if(io_status)return io_status;goto L8c65619e; /* mov.l r10,@r14 */
L8c65619e: *stop=0x8c65619eu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[9],4);if(io_status)return io_status;goto L8c6561a0; /* mov.l r9,@(0x4,r14) */
L8c6561a0: *stop=0x8c6561a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6561a2; /* mov.l @r15,r3 */
L8c6561a2: *stop=0x8c6561a2u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6561a4; /* mov.l r3,@(0x8,r14) */
L8c6561a4: *stop=0x8c6561a4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6561a6; /* mov.l @(0x4,r15),r2 */
L8c6561a6: *stop=0x8c6561a6u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6561a8; /* mov.l r2,@(0xc,r14) */
L8c6561a8: *stop=0x8c6561a8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6561aa; /* mov.l @(r0,r15),r3 */
L8c6561aa: *stop=0x8c6561aau;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6561ac; /* mov.l r3,@(0x10,r14) */
L8c6561ac: *stop=0x8c6561acu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6561ae; /* mov.l @r4,r6 */
L8c6561ae: *stop=0x8c6561aeu;*sr=(*sr&~1u)|(r[5]==r[6]);goto L8c6561b0; /* cmp/eq r5,r6 */
L8c6561b0: if((*sr&1u))goto L8c6561b6;goto L8c6561b2; /* bt 0x8c6561b6 */
L8c6561b2: *stop=0x8c6561b4u;goto L8c656484; /* bra 0x8c656484 */
L8c6561b6: *stop=0x8c6561b6u;r[0]=84u;goto L8c6561b8; /* mov #0x54,r0 */
L8c6561b8: *stop=0x8c6561b8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6561ba; /* mov.l r13,@-r15 */
L8c6561ba: *stop=0x8c6561bau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6561bc; /* mov.l @(0x4,r15),r5 */
L8c6561bc: *stop=0x8c6561bcu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6561be; /* mov.l @(0x8,r15),r6 */
L8c6561be: *stop=0x8c6561beu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[7]=loaded;}goto L8c6561c0; /* mov.l @(r0,r15),r7 */
L8c6561c0: target=0x8c656f50u;*pr=0x8c6561c4u;*stop=0x8c6561c2u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6561c4; /* bsr 0x8c656f50 */
L8c6561c4: *stop=0x8c6561c4u;r[15]+=4u;goto L8c6561c6; /* add #0x4,r15 */
L8c6561c6: *stop=0x8c6561c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6561fcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6561c8; /* mov.l 0x8c6561fc,r2 */
L8c6561c8: *stop=0x8c6561c8u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c6561ca; /* mov.l r0,@(0x8,r15) */
L8c6561ca: *stop=0x8c6561cau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6561cc; /* mov.l @r2,r3 */
L8c6561cc: *stop=0x8c6561ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656204u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6561ce; /* mov.l 0x8c656204,r4 */
L8c6561ce: *stop=0x8c6561ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656200u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6561d0; /* mov.l 0x8c656200,r1 */
L8c6561d0: *stop=0x8c6561d0u;r[5]=r[4];goto L8c6561d2; /* mov r4,r5 */
L8c6561d2: *stop=0x8c6561d2u;if(!valid_access(r[10]+16u,4u))return -3;wr(m,bus,&io_status,r[10]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6561d4; /* mov.l r3,@(0x10,r10) */
L8c6561d4: *stop=0x8c6561d4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c6561d6; /* mov.l r13,@r1 */
L8c6561d6: *stop=0x8c6561d8u;r[5]+=16u;goto L8c656224; /* bra 0x8c656224 */
L8c656220: *stop=0x8c656220u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[13],4);if(io_status)return io_status;goto L8c656222; /* mov.l r13,@r4 */
L8c656222: *stop=0x8c656222u;r[4]+=4u;goto L8c656224; /* add #0x4,r4 */
L8c656224: *stop=0x8c656224u;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c656226; /* cmp/hs r5,r4 */
L8c656226: if(!(*sr&1u))goto L8c656220;goto L8c656228; /* bf 0x8c656220 */
L8c656228: *stop=0x8c656228u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65622a; /* mov.l r13,@-r15 */
L8c65622a: *stop=0x8c65622au;r[0]=88u;goto L8c65622c; /* mov #0x58,r0 */
L8c65622c: *stop=0x8c65622cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65622e; /* mov.l r13,@-r15 */
L8c65622e: *stop=0x8c65622eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c656230; /* mov.l @(0x8,r15),r8 */
L8c656230: *stop=0x8c656230u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656324u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656232; /* mov.l 0x8c656324,r3 */
L8c656232: *stop=0x8c656232u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[7]=loaded;}goto L8c656234; /* mov.l @(r0,r15),r7 */
L8c656234: *stop=0x8c656234u;r[5]=r[8];goto L8c656236; /* mov r8,r5 */
L8c656236: *stop=0x8c656236u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656238; /* mov.l @(0xc,r15),r6 */
L8c656238: target=r[3];*pr=0x8c65623cu;*stop=0x8c65623au;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65623c; /* jsr @r3 */
L8c65623c: *stop=0x8c65623cu;r[15]+=8u;goto L8c65623e; /* add #0x8,r15 */
L8c65623e: *stop=0x8c65623eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656328u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656240; /* mov.l 0x8c656328,r3 */
L8c656240: *stop=0x8c656240u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c656242; /* mov.l r0,@(0xc,r15) */
L8c656242: target=r[3];*pr=0x8c656246u;*stop=0x8c656244u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656246; /* jsr @r3 */
L8c656246: *stop=0x8c656246u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656248; /* mov.l @(0x4,r15),r3 */
L8c656248: *stop=0x8c656248u;r[11]=r[9];goto L8c65624a; /* mov r9,r11 */
L8c65624a: *stop=0x8c65624au;r[11]+=r[8];goto L8c65624c; /* add r8,r11 */
L8c65624c: *stop=0x8c65624cu;r[4]=r[15];goto L8c65624e; /* mov r15,r4 */
L8c65624e: *stop=0x8c65624eu;r[0]=80u;goto L8c656250; /* mov #0x50,r0 */
L8c656250: *stop=0x8c656250u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656252; /* mov.l @(0x8,r15),r7 */
L8c656252: *stop=0x8c656252u;r[11]+=r[3];goto L8c656254; /* add r3,r11 */
L8c656254: *stop=0x8c656254u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656256; /* mov.l @(r0,r15),r2 */
L8c656256: *stop=0x8c656256u;r[4]+=32u;goto L8c656258; /* add #0x20,r4 */
L8c656258: *stop=0x8c656258u;{uint32_t loaded=sx(m,bus,&io_status,0x8c656320u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65625a; /* mov.w 0x8c656320,r1 */
L8c65625a: *stop=0x8c65625au;r[0]=80u;goto L8c65625c; /* mov #0x50,r0 */
L8c65625c: *stop=0x8c65625cu;r[5]=r[4];goto L8c65625e; /* mov r4,r5 */
L8c65625e: *stop=0x8c65625eu;r[11]+=r[2];goto L8c656260; /* add r2,r11 */
L8c656260: *stop=0x8c656260u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[9],4);if(io_status)return io_status;goto L8c656262; /* mov.l r9,@r5 */
L8c656262: *stop=0x8c656262u;r[7]<<=2;goto L8c656264; /* shll2 r7 */
L8c656264: *stop=0x8c656264u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[8],4);if(io_status)return io_status;goto L8c656266; /* mov.l r8,@(0x4,r4) */
L8c656266: *stop=0x8c656266u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656268; /* mov.l @(0x4,r15),r3 */
L8c656268: *stop=0x8c656268u;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c65626a; /* shll r7 */
L8c65626a: *stop=0x8c65626au;r[7]+=r[11];goto L8c65626c; /* add r11,r7 */
L8c65626c: *stop=0x8c65626cu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[3],4);if(io_status)return io_status;goto L8c65626e; /* mov.l r3,@(0x8,r4) */
L8c65626e: *stop=0x8c65626eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656270; /* mov.l @(r0,r15),r2 */
L8c656270: *stop=0x8c656270u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[2],4);if(io_status)return io_status;goto L8c656272; /* mov.l r2,@(0xc,r4) */
L8c656272: *stop=0x8c656272u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656274; /* mov.l @r5,r3 */
L8c656274: *stop=0x8c656274u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c656276; /* tst r3,r3 */
L8c656276: take=(*sr&1u);*stop=0x8c656278u;r[7]+=r[1];if(take)goto L8c656280;goto L8c65627a; /* bt/s 0x8c656280 */
L8c65627a: *stop=0x8c65627au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65627c; /* mov.l @r4,r3 */
L8c65627c: *stop=0x8c65627cu;r[3]+=4294967280u;goto L8c65627e; /* add #-0x10,r3 */
L8c65627e: *stop=0x8c65627eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c656280; /* mov.l r3,@r4 */
L8c656280: *stop=0x8c656280u;r[5]=r[4];goto L8c656282; /* mov r4,r5 */
L8c656282: *stop=0x8c656282u;r[5]+=4u;goto L8c656284; /* add #0x4,r5 */
L8c656284: *stop=0x8c656284u;r[9]=r[4];goto L8c656286; /* mov r4,r9 */
L8c656286: *stop=0x8c656286u;r[6]=r[5];goto L8c656288; /* mov r5,r6 */
L8c656288: *stop=0x8c65628au;r[9]+=16u;goto L8c65629c; /* bra 0x8c65629c */
L8c65628c: *stop=0x8c65628cu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65628e; /* mov.l @r5,r3 */
L8c65628e: *stop=0x8c65628eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c656290; /* tst r3,r3 */
L8c656290: if((*sr&1u))goto L8c656298;goto L8c656292; /* bt 0x8c656298 */
L8c656292: *stop=0x8c656292u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656294; /* mov.l @r6,r3 */
L8c656294: *stop=0x8c656294u;r[3]+=4294967280u;goto L8c656296; /* add #-0x10,r3 */
L8c656296: *stop=0x8c656296u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c656298; /* mov.l r3,@r6 */
L8c656298: *stop=0x8c656298u;r[5]+=4u;goto L8c65629a; /* add #0x4,r5 */
L8c65629a: *stop=0x8c65629au;r[6]+=4u;goto L8c65629c; /* add #0x4,r6 */
L8c65629c: *stop=0x8c65629cu;*sr=(*sr&~1u)|(r[5]>=r[9]);goto L8c65629e; /* cmp/hs r9,r5 */
L8c65629e: if(!(*sr&1u))goto L8c65628c;goto L8c6562a0; /* bf 0x8c65628c */
L8c6562a0: *stop=0x8c6562a0u;r[3]=r[4];goto L8c6562a2; /* mov r4,r3 */
L8c6562a2: *stop=0x8c6562a2u;r[6]=r[3];goto L8c6562a4; /* mov r3,r6 */
L8c6562a4: *stop=0x8c6562a4u;r[6]+=4u;goto L8c6562a6; /* add #0x4,r6 */
L8c6562a6: *stop=0x8c6562a6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6562a8; /* mov.l r3,@r15 */
L8c6562a8: *stop=0x8c6562a8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6562aa; /* mov.l @r3,r2 */
L8c6562aa: *stop=0x8c6562aau;r[5]=r[4];goto L8c6562ac; /* mov r4,r5 */
L8c6562ac: *stop=0x8c6562acu;r[5]+=12u;goto L8c6562ae; /* add #0xc,r5 */
L8c6562ae: *stop=0x8c6562aeu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6562b0; /* mov.l r2,@(0x4,r14) */
L8c6562b0: *stop=0x8c6562b0u;r[2]=r[4];goto L8c6562b2; /* mov r4,r2 */
L8c6562b2: *stop=0x8c6562b2u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6562b4; /* mov.l @r6,r3 */
L8c6562b4: *stop=0x8c6562b4u;r[2]+=8u;goto L8c6562b6; /* add #0x8,r2 */
L8c6562b6: *stop=0x8c6562b6u;r[0]=80u;goto L8c6562b8; /* mov #0x50,r0 */
L8c6562b8: *stop=0x8c6562b8u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6562ba; /* mov.l r3,@(0x8,r14) */
L8c6562ba: *stop=0x8c6562bau;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6562bc; /* mov.l r2,@(0x8,r15) */
L8c6562bc: *stop=0x8c6562bcu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6562be; /* mov.l @r2,r3 */
L8c6562be: *stop=0x8c6562beu;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6562c0; /* mov.l r3,@(0xc,r14) */
L8c6562c0: *stop=0x8c6562c0u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6562c2; /* mov.l @r5,r2 */
L8c6562c2: *stop=0x8c6562c2u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6562c4; /* mov.l r2,@(0x10,r14) */
L8c6562c4: *stop=0x8c6562c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6562c6; /* mov.l @r15,r3 */
L8c6562c6: *stop=0x8c6562c6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6562c8; /* mov.l @r3,r2 */
L8c6562c8: *stop=0x8c6562c8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6562ca; /* mov.l r2,@r15 */
L8c6562ca: *stop=0x8c6562cau;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6562cc; /* mov.l @r6,r3 */
L8c6562cc: *stop=0x8c6562ccu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6562ce; /* mov.l r3,@(0x4,r15) */
L8c6562ce: *stop=0x8c6562ceu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6562d0; /* mov.l @(0x8,r15),r2 */
L8c6562d0: *stop=0x8c6562d0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6562d2; /* mov.l @r2,r3 */
L8c6562d2: *stop=0x8c6562d2u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6562d4; /* mov.l r3,@(0x8,r15) */
L8c6562d4: *stop=0x8c6562d4u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6562d6; /* mov.l @r5,r2 */
L8c6562d6: *stop=0x8c6562d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65632cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6562d8; /* mov.l 0x8c65632c,r3 */
L8c6562d8: *stop=0x8c6562d8u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c6562da; /* mov.l r2,@(r0,r15) */
L8c6562da: *stop=0x8c6562dau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6562dc; /* mov.l @r3,r0 */
L8c6562dc: *stop=0x8c6562dcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6562de; /* cmp/eq #0x1,r0 */
L8c6562de: if(!(*sr&1u))goto L8c6562e2;goto L8c6562e0; /* bf 0x8c6562e2 */
L8c6562e0: *stop=0x8c6562e0u;r[12]=1u;goto L8c6562e2; /* mov #0x1,r12 */
L8c6562e2: *stop=0x8c6562e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6562e4; /* mov.l r12,@-r15 */
L8c6562e4: *stop=0x8c6562e4u;r[0]=92u;goto L8c6562e6; /* mov #0x5c,r0 */
L8c6562e6: *stop=0x8c6562e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c6562e8; /* mov.l r7,@-r15 */
L8c6562e8: *stop=0x8c6562e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6562ea; /* mov.l r13,@-r15 */
L8c6562ea: *stop=0x8c6562eau;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6562ec; /* mov.l @(0x10,r15),r5 */
L8c6562ec: *stop=0x8c6562ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656330u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6562ee; /* mov.l 0x8c656330,r3 */
L8c6562ee: *stop=0x8c6562eeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[7]=loaded;}goto L8c6562f0; /* mov.l @(r0,r15),r7 */
L8c6562f0: *stop=0x8c6562f0u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6562f2; /* mov.l @(0x14,r15),r6 */
L8c6562f2: target=r[3];*pr=0x8c6562f6u;*stop=0x8c6562f4u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6562f6; /* jsr @r3 */
L8c6562f6: *stop=0x8c6562f6u;r[4]=r[0];goto L8c6562f8; /* mov r0,r4 */
L8c6562f8: *stop=0x8c6562f8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6562fa; /* tst r4,r4 */
L8c6562fa: take=(*sr&1u);*stop=0x8c6562fcu;r[15]+=12u;if(take)goto L8c656340;goto L8c6562fe; /* bt/s 0x8c656340 */
L8c6562fe: *stop=0x8c656300u;r[0]=r[4];goto L8c6564a2; /* bra 0x8c6564a2 */
L8c656340: *stop=0x8c656340u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6563e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656342; /* mov.l 0x8c6563e4,r3 */
L8c656342: *stop=0x8c656342u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6563e0u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c656344; /* mov.w 0x8c6563e0,r4 */
L8c656344: *stop=0x8c656344u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656346; /* mov.l @(0x24,r14),r6 */
L8c656346: target=r[3];*pr=0x8c65634au;*stop=0x8c656348u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65634a; /* jsr @r3 */
L8c65634a: *stop=0x8c65634au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6563e8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65634c; /* mov.l 0x8c6563e8,r4 */
L8c65634c: *stop=0x8c65634cu;*sr=(*sr&~1u)|(r[11]>=r[4]);goto L8c65634e; /* cmp/hs r4,r11 */
L8c65634e: if((*sr&1u))goto L8c656352;goto L8c656350; /* bt 0x8c656352 */
L8c656350: *stop=0x8c656350u;r[11]=r[4];goto L8c656352; /* mov r4,r11 */
L8c656352: *stop=0x8c656352u;r[0]=100u;goto L8c656354; /* mov #0x64,r0 */
L8c656354: *stop=0x8c656354u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656356; /* mov.l @(r0,r14),r3 */
L8c656356: *stop=0x8c656356u;r[11]<<=2;goto L8c656358; /* shll2 r11 */
L8c656358: *stop=0x8c656358u;r[0]=88u;goto L8c65635a; /* mov #0x58,r0 */
L8c65635a: *stop=0x8c65635au;r[11]+=r[3];goto L8c65635c; /* add r3,r11 */
L8c65635c: *stop=0x8c65635cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6563ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65635e; /* mov.l 0x8c6563ec,r3 */
L8c65635e: *stop=0x8c65635eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[11],4);if(io_status)return io_status;goto L8c656360; /* mov.l r11,@(r0,r14) */
L8c656360: *stop=0x8c656360u;r[0]=96u;goto L8c656362; /* mov #0x60,r0 */
L8c656362: *stop=0x8c656362u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c656364; /* mov.l @(r0,r14),r4 */
L8c656364: *stop=0x8c656364u;r[0]=88u;goto L8c656366; /* mov #0x58,r0 */
L8c656366: *stop=0x8c656366u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656368; /* mov.l @(r0,r14),r2 */
L8c656368: *stop=0x8c656368u;r[4]=0u-r[4];goto L8c65636a; /* neg r4,r4 */
L8c65636a: *stop=0x8c65636au;r[4]+=r[3];goto L8c65636c; /* add r3,r4 */
L8c65636c: *stop=0x8c65636cu;*sr=(*sr&~1u)|(r[2]>r[4]);goto L8c65636e; /* cmp/hi r4,r2 */
L8c65636e: if(!(*sr&1u))goto L8c656372;goto L8c656370; /* bf 0x8c656372 */
L8c656370: *stop=0x8c656370u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c656372; /* mov.l r4,@(r0,r14) */
L8c656372: *stop=0x8c656372u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6563f0u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656374; /* mov.l 0x8c6563f0,r6 */
L8c656374: *stop=0x8c656374u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6563f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656376; /* mov.l 0x8c6563f4,r3 */
L8c656376: *stop=0x8c656376u;r[5]=r[6];goto L8c656378; /* mov r6,r5 */
L8c656378: *stop=0x8c656378u;r[5]+=4294967292u;goto L8c65637a; /* add #-0x4,r5 */
L8c65637a: target=r[3];*pr=0x8c65637eu;*stop=0x8c65637cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65637e; /* jsr @r3 */
L8c65637e: *stop=0x8c65637eu;r[8]=r[0];goto L8c656380; /* mov r0,r8 */
L8c656380: *stop=0x8c656380u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c656382; /* tst r8,r8 */
L8c656382: if((*sr&1u))goto L8c656388;goto L8c656384; /* bt 0x8c656388 */
L8c656384: *stop=0x8c656386u;goto L8c6564a0; /* bra 0x8c6564a0 */
L8c656388: *stop=0x8c656388u;r[0]=88u;goto L8c65638a; /* mov #0x58,r0 */
L8c65638a: *stop=0x8c65638au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6563f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65638c; /* mov.l 0x8c6563f8,r2 */
L8c65638c: *stop=0x8c65638cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65638e; /* mov.l @(r0,r14),r3 */
L8c65638e: *stop=0x8c65638eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c656390; /* mov.l r3,@r2 */
L8c656390: *stop=0x8c656390u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c656392; /* mov.l r13,@-r15 */
L8c656392: *stop=0x8c656392u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656394; /* mov.l @(0x18,r14),r3 */
L8c656394: *stop=0x8c656394u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656396; /* mov.l @(0x10,r15),r1 */
L8c656396: *stop=0x8c656396u;r[3]=~r[3];goto L8c656398; /* not r3,r3 */
L8c656398: *stop=0x8c656398u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6563fcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c65639a; /* mov.l 0x8c6563fc,r7 */
L8c65639a: *stop=0x8c65639au;r[3]&=r[1];goto L8c65639c; /* and r1,r3 */
L8c65639c: *stop=0x8c65639cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65639e; /* mov.l r3,@-r15 */
L8c65639e: *stop=0x8c65639eu;if(!valid_access(r[7]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6563a0; /* mov.l @(0xc,r7),r7 */
L8c6563a0: *stop=0x8c6563a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6563fcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6563a2; /* mov.l 0x8c6563fc,r6 */
L8c6563a2: *stop=0x8c6563a2u;r[3]=4294967289u;goto L8c6563a4; /* mov #-0x7,r3 */
L8c6563a4: *stop=0x8c6563a4u;r[7]=(r[3]&0x80000000u)?((r[3]&31u)?r[7]>>((0u-r[3])&31u):0u):(r[7]<<(r[3]&31u));goto L8c6563a6; /* shld r3,r7 */
L8c6563a6: *stop=0x8c6563a6u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6563a8; /* mov.l @(0x8,r6),r6 */
L8c6563a8: *stop=0x8c6563a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6563fcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6563aa; /* mov.l 0x8c6563fc,r5 */
L8c6563aa: *stop=0x8c6563aau;r[6]=(r[3]&0x80000000u)?((r[3]&31u)?r[6]>>((0u-r[3])&31u):0u):(r[6]<<(r[3]&31u));goto L8c6563ac; /* shld r3,r6 */
L8c6563ac: *stop=0x8c6563acu;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6563ae; /* mov.l @(0x4,r5),r5 */
L8c6563ae: *stop=0x8c6563aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6563fcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6563b0; /* mov.l 0x8c6563fc,r4 */
L8c6563b0: *stop=0x8c6563b0u;r[5]=(r[3]&0x80000000u)?((r[3]&31u)?r[5]>>((0u-r[3])&31u):0u):(r[5]<<(r[3]&31u));goto L8c6563b2; /* shld r3,r5 */
L8c6563b2: *stop=0x8c6563b2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6563b4; /* mov.l @r4,r4 */
L8c6563b4: *stop=0x8c6563b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656400u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6563b6; /* mov.l 0x8c656400,r1 */
L8c6563b6: target=r[1];*pr=0x8c6563bau;*stop=0x8c6563b8u;r[4]=(r[3]&0x80000000u)?((r[3]&31u)?r[4]>>((0u-r[3])&31u):0u):(r[4]<<(r[3]&31u));if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6563ba; /* jsr @r1 */
L8c6563ba: *stop=0x8c6563bau;r[15]+=8u;goto L8c6563bc; /* add #0x8,r15 */
L8c6563bc: *stop=0x8c6563bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656404u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6563be; /* mov.l 0x8c656404,r5 */
L8c6563be: *stop=0x8c6563beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656408u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6563c0; /* mov.l 0x8c656408,r14 */
L8c6563c0: *stop=0x8c6563c0u;r[4]=r[5];goto L8c6563c2; /* mov r5,r4 */
L8c6563c2: *stop=0x8c6563c2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6563c4; /* mov.l @r4,r3 */
L8c6563c4: *stop=0x8c6563c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65640cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6563c6; /* mov.l 0x8c65640c,r6 */
L8c6563c6: *stop=0x8c6563c6u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c6563c8; /* mov.l r3,@r14 */
L8c6563c8: *stop=0x8c6563c8u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6563ca; /* mov.l @(0x4,r4),r2 */
L8c6563ca: *stop=0x8c6563cau;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6563cc; /* mov.l r2,@(0x4,r14) */
L8c6563cc: *stop=0x8c6563ccu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6563ce; /* mov.l @(0x8,r4),r3 */
L8c6563ce: *stop=0x8c6563ceu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6563d0; /* mov.l r3,@(0x8,r14) */
L8c6563d0: *stop=0x8c6563d0u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6563d2; /* mov.l @(0xc,r4),r2 */
L8c6563d2: *stop=0x8c6563d2u;r[4]=r[13];goto L8c6563d4; /* mov r13,r4 */
L8c6563d4: *stop=0x8c6563d4u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6563d6; /* mov.l r2,@(0xc,r14) */
L8c6563d6: *stop=0x8c6563d8u;r[11]=16u;goto L8c656428; /* bra 0x8c656428 */
L8c656420: *stop=0x8c656420u;r[0]=r[4];goto L8c656422; /* mov r4,r0 */
L8c656422: *stop=0x8c656422u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656424; /* mov.l @(r0,r14),r3 */
L8c656424: *stop=0x8c656424u;r[4]+=4u;goto L8c656426; /* add #0x4,r4 */
L8c656426: *stop=0x8c656426u;if(!valid_access(r[0]+r[6],4u))return -3;wr(m,bus,&io_status,r[6]+r[0],r[3],4);if(io_status)return io_status;goto L8c656428; /* mov.l r3,@(r0,r6) */
L8c656428: *stop=0x8c656428u;*sr=(*sr&~1u)|(r[4]>=r[11]);goto L8c65642a; /* cmp/hs r11,r4 */
L8c65642a: if(!(*sr&1u))goto L8c656420;goto L8c65642c; /* bf 0x8c656420 */
L8c65642c: *stop=0x8c65642cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6564c4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65642e; /* mov.l 0x8c6564c4,r4 */
L8c65642e: *stop=0x8c65642eu;r[2]=0u;goto L8c656430; /* mov #0x0,r2 */
L8c656430: *stop=0x8c656430u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6564c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656432; /* mov.l 0x8c6564c8,r3 */
L8c656432: *stop=0x8c656432u;*sr=(*sr&~1u)|(r[2]>=r[12]);goto L8c656434; /* cmp/hs r12,r2 */
L8c656434: *stop=0x8c656434u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[13],4);if(io_status)return io_status;goto L8c656436; /* mov.l r13,@r4 */
L8c656436: *stop=0x8c656436u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[13],4);if(io_status)return io_status;goto L8c656438; /* mov.l r13,@(0x4,r4) */
L8c656438: *stop=0x8c656438u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[13],4);if(io_status)return io_status;goto L8c65643a; /* mov.l r13,@(0x8,r4) */
L8c65643a: *stop=0x8c65643au;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[13],4);if(io_status)return io_status;goto L8c65643c; /* mov.l r13,@(0xc,r4) */
L8c65643c: *stop=0x8c65643cu;r[4]=r[5];goto L8c65643e; /* mov r5,r4 */
L8c65643e: *stop=0x8c65643eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6564ccu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c656440; /* mov.l 0x8c6564cc,r9 */
L8c656440: *stop=0x8c656440u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c656442; /* mov.l r5,@r3 */
L8c656442: *stop=0x8c656442u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6564d0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656444; /* mov.l 0x8c6564d0,r5 */
L8c656444: *stop=0x8c656444u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[14],4);if(io_status)return io_status;goto L8c656446; /* mov.l r14,@r9 */
L8c656446: take=(*sr&1u);*stop=0x8c656448u;r[6]=r[13];if(take)goto L8c656456;goto L8c65644a; /* bt/s 0x8c656456 */
L8c65644a: *stop=0x8c65644au;r[6]+=1u;goto L8c65644c; /* add #0x1,r6 */
L8c65644c: *stop=0x8c65644cu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c65644e; /* mov.l r4,@r5 */
L8c65644e: *stop=0x8c65644eu;*sr=(*sr&~1u)|(r[6]>=r[12]);goto L8c656450; /* cmp/hs r12,r6 */
L8c656450: *stop=0x8c656450u;r[4]+=20u;goto L8c656452; /* add #0x14,r4 */
L8c656452: take=!(*sr&1u);*stop=0x8c656454u;r[5]+=4u;if(take)goto L8c65644a;goto L8c656456; /* bf/s 0x8c65644a */
L8c656456: target=0x8c657240u;*pr=0x8c65645au;*stop=0x8c656458u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65645a; /* bsr 0x8c657240 */
L8c65645a: *stop=0x8c65645cu;r[4]=r[13];goto L8c656470; /* bra 0x8c656470 */
L8c65645e: *stop=0x8c65645eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6564d4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c656460; /* mov.l 0x8c6564d4,r0 */
L8c656460: *stop=0x8c656460u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656462; /* mov.l @r9,r2 */
L8c656462: *stop=0x8c656462u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656464; /* mov.l @(r0,r4),r3 */
L8c656464: *stop=0x8c656464u;r[2]+=r[4];goto L8c656466; /* add r4,r2 */
L8c656466: *stop=0x8c656466u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c656468; /* mov.l @r2,r1 */
L8c656468: *stop=0x8c656468u;r[3]<<=2;goto L8c65646a; /* shll2 r3 */
L8c65646a: *stop=0x8c65646au;r[4]+=4u;goto L8c65646c; /* add #0x4,r4 */
L8c65646c: *stop=0x8c65646cu;r[1]+=r[3];goto L8c65646e; /* add r3,r1 */
L8c65646e: *stop=0x8c65646eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c656470; /* mov.l r1,@r2 */
L8c656470: *stop=0x8c656470u;*sr=(*sr&~1u)|(r[4]>=r[11]);goto L8c656472; /* cmp/hs r11,r4 */
L8c656472: if(!(*sr&1u))goto L8c65645e;goto L8c656474; /* bf 0x8c65645e */
L8c656474: *stop=0x8c656474u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6564d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656476; /* mov.l 0x8c6564d8,r2 */
L8c656476: *stop=0x8c656476u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6564c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656478; /* mov.l 0x8c6564c4,r3 */
L8c656478: *stop=0x8c656478u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[14],4);if(io_status)return io_status;goto L8c65647a; /* mov.l r14,@r10 */
L8c65647a: *stop=0x8c65647au;if(!valid_access(r[10]+4u,4u))return -3;wr(m,bus,&io_status,r[10]+0x4u,r[2],4);if(io_status)return io_status;goto L8c65647c; /* mov.l r2,@(0x4,r10) */
L8c65647c: *stop=0x8c65647cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6564d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65647e; /* mov.l 0x8c6564d0,r2 */
L8c65647e: *stop=0x8c65647eu;if(!valid_access(r[10]+8u,4u))return -3;wr(m,bus,&io_status,r[10]+0x8u,r[3],4);if(io_status)return io_status;goto L8c656480; /* mov.l r3,@(0x8,r10) */
L8c656480: *stop=0x8c656482u;if(!valid_access(r[10]+12u,4u))return -3;wr(m,bus,&io_status,r[10]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6564a0; /* bra 0x8c6564a0 */
L8c656484: *stop=0x8c656484u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656486; /* mov.l @r4,r2 */
L8c656486: *stop=0x8c656486u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6564c0u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c656488; /* mov.w 0x8c6564c0,r6 */
L8c656488: *stop=0x8c656488u;r[2]|=r[6];goto L8c65648a; /* or r6,r2 */
L8c65648a: *stop=0x8c65648au;r[6]=r[2];goto L8c65648c; /* mov r2,r6 */
L8c65648c: *stop=0x8c65648cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c65648e; /* mov.l r2,@r4 */
L8c65648e: *stop=0x8c65648eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6564c2u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c656490; /* mov.w 0x8c6564c2,r2 */
L8c656490: *stop=0x8c656490u;*sr=(*sr&~1u)|(r[2]==r[6]);goto L8c656492; /* cmp/eq r2,r6 */
L8c656492: if(!(*sr&1u))goto L8c6564a0;goto L8c656494; /* bf 0x8c6564a0 */
L8c656494: *stop=0x8c656494u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6564dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656496; /* mov.l 0x8c6564dc,r3 */
L8c656496: *stop=0x8c656496u;r[0]=84u;goto L8c656498; /* mov #0x54,r0 */
L8c656498: *stop=0x8c656498u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c65649a; /* mov.l r5,@r4 */
L8c65649a: target=r[3];*pr=0x8c65649eu;*stop=0x8c65649cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65649e; /* jsr @r3 */
L8c65649e: *stop=0x8c65649eu;r[8]=r[0];goto L8c6564a0; /* mov r0,r8 */
L8c6564a0: *stop=0x8c6564a0u;r[0]=r[8];goto L8c6564a2; /* mov r8,r0 */
L8c6564a2: *stop=0x8c6564a2u;r[15]+=48u;goto L8c6564a4; /* add #0x30,r15 */
L8c6564a4: *stop=0x8c6564a4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6564a6; /* lds.l @r15+,PR */
L8c6564a6: *stop=0x8c6564a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6564a8; /* mov.l @r15+,r8 */
L8c6564a8: *stop=0x8c6564a8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6564aa; /* mov.l @r15+,r9 */
L8c6564aa: *stop=0x8c6564aau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6564ac; /* mov.l @r15+,r10 */
L8c6564ac: *stop=0x8c6564acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6564ae; /* mov.l @r15+,r11 */
L8c6564ae: *stop=0x8c6564aeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6564b0; /* mov.l @r15+,r12 */
L8c6564b0: *stop=0x8c6564b0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6564b2; /* mov.l @r15+,r13 */
L8c6564b2: *stop=0x8c6564b2u;if(*pr!=entry_pr)return -3;*stop=0x8c6564b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c656660(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c656660;
L8c656660: *stop=0x8c656660u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c656662; /* mov.l r14,@-r15 */
L8c656662: *stop=0x8c656662u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c656664; /* mov.l r13,@-r15 */
L8c656664: *stop=0x8c656664u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c656666; /* mov.l r12,@-r15 */
L8c656666: *stop=0x8c656666u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c656668; /* mov.l r11,@-r15 */
L8c656668: *stop=0x8c656668u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65666a; /* mov.l r10,@-r15 */
L8c65666a: *stop=0x8c65666au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65666c; /* mov.l r9,@-r15 */
L8c65666c: *stop=0x8c65666cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65666e; /* sts.l PR,@-r15 */
L8c65666e: *stop=0x8c65666eu;r[15]+=4294967276u;goto L8c656670; /* add #-0x14,r15 */
L8c656670: *stop=0x8c656670u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656784u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c656672; /* mov.l 0x8c656784,r14 */
L8c656672: *stop=0x8c656672u;r[0]=84u;goto L8c656674; /* mov #0x54,r0 */
L8c656674: *stop=0x8c656674u;r[6]=2u;goto L8c656676; /* mov #0x2,r6 */
L8c656676: *stop=0x8c656676u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65678cu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656678; /* mov.l 0x8c65678c,r7 */
L8c656678: *stop=0x8c656678u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[6],4);if(io_status)return io_status;goto L8c65667a; /* mov.l r6,@(r0,r14) */
L8c65667a: *stop=0x8c65667au;r[0]=88u;goto L8c65667c; /* mov #0x58,r0 */
L8c65667c: *stop=0x8c65667cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c65667e; /* mov.l r4,@r14 */
L8c65667e: *stop=0x8c65667eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[5],4);if(io_status)return io_status;goto L8c656680; /* mov.l r5,@(r0,r14) */
L8c656680: *stop=0x8c656680u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656788u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656682; /* mov.l 0x8c656788,r5 */
L8c656682: *stop=0x8c656682u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656684; /* mov.l @r5,r3 */
L8c656684: *stop=0x8c656684u;*sr=(*sr&~1u)|(r[7]==r[3]);goto L8c656686; /* cmp/eq r7,r3 */
L8c656686: take=(*sr&1u);*stop=0x8c656688u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c65668e;goto L8c65668a; /* bt/s 0x8c65668e */
L8c65668a: *stop=0x8c65668cu;goto L8c6567c0; /* bra 0x8c6567c0 */
L8c65668e: *stop=0x8c65668eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656790u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656690; /* mov.l 0x8c656790,r3 */
L8c656690: *stop=0x8c656690u;r[13]=1u;goto L8c656692; /* mov #0x1,r13 */
L8c656692: *stop=0x8c656692u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656794u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656694; /* mov.l 0x8c656794,r1 */
L8c656694: *stop=0x8c656694u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c656696; /* mov.l r13,@r3 */
L8c656696: *stop=0x8c656696u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656698; /* mov.l @r1,r2 */
L8c656698: *stop=0x8c656698u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656798u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65669a; /* mov.l 0x8c656798,r0 */
L8c65669a: *stop=0x8c65669au;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c65669c; /* mov.l r2,@r0 */
L8c65669c: *stop=0x8c65669cu;r[2]=r[1];goto L8c65669e; /* mov r1,r2 */
L8c65669e: *stop=0x8c65669eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6566a0; /* mov.l @r2,r3 */
L8c6566a0: *stop=0x8c6566a0u;r[0]+=4294967272u;goto L8c6566a2; /* add #-0x18,r0 */
L8c6566a2: *stop=0x8c6566a2u;r[3]<<=2;goto L8c6566a4; /* shll2 r3 */
L8c6566a4: *stop=0x8c6566a4u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[6],4);if(io_status)return io_status;goto L8c6566a6; /* mov.l r6,@(r0,r3) */
L8c6566a6: *stop=0x8c6566a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65679cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6566a8; /* mov.l 0x8c65679c,r1 */
L8c6566a8: *stop=0x8c6566a8u;r[3]=0u;goto L8c6566aa; /* mov #0x0,r3 */
L8c6566aa: *stop=0x8c6566aau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c6566ac; /* mov.l r3,@r1 */
L8c6566ac: *stop=0x8c6566acu;r[2]+=24u;goto L8c6566ae; /* add #0x18,r2 */
L8c6566ae: *stop=0x8c6566aeu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c6566b0; /* mov.l r13,@r2 */
L8c6566b0: *stop=0x8c6566b0u;r[5]=r[15];goto L8c6566b2; /* mov r15,r5 */
L8c6566b2: *stop=0x8c6566b2u;r[5]+=4u;goto L8c6566b4; /* add #0x4,r5 */
L8c6566b4: *stop=0x8c6566b4u;r[12]=r[5];goto L8c6566b6; /* mov r5,r12 */
L8c6566b6: *stop=0x8c6566b6u;r[10]=r[12];goto L8c6566b8; /* mov r12,r10 */
L8c6566b8: *stop=0x8c6566b8u;r[10]+=8u;goto L8c6566ba; /* add #0x8,r10 */
L8c6566ba: *stop=0x8c6566bau;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6566bc; /* mov.l @(0x10,r4),r3 */
L8c6566bc: *stop=0x8c6566bcu;r[11]=r[12];goto L8c6566be; /* mov r12,r11 */
L8c6566be: *stop=0x8c6566beu;r[11]+=4u;goto L8c6566c0; /* add #0x4,r11 */
L8c6566c0: *stop=0x8c6566c0u;r[3]&=r[13];goto L8c6566c2; /* and r13,r3 */
L8c6566c2: *stop=0x8c6566c2u;r[9]=r[12];goto L8c6566c4; /* mov r12,r9 */
L8c6566c4: *stop=0x8c6566c4u;r[9]+=12u;goto L8c6566c6; /* add #0xc,r9 */
L8c6566c6: *stop=0x8c6566c6u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;goto L8c6566c8; /* mov.l r3,@r12 */
L8c6566c8: *stop=0x8c6566c8u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6566ca; /* mov.l @(0x10,r4),r0 */
L8c6566ca: *stop=0x8c6566cau;r[3]=0u;goto L8c6566cc; /* mov #0x0,r3 */
L8c6566cc: *stop=0x8c6566ccu;r[6]&=r[0];goto L8c6566ce; /* and r0,r6 */
L8c6566ce: *stop=0x8c6566ceu;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[6],4);if(io_status)return io_status;goto L8c6566d0; /* mov.l r6,@r11 */
L8c6566d0: *stop=0x8c6566d0u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6566d2; /* mov.l @(0x10,r4),r0 */
L8c6566d2: *stop=0x8c6566d2u;r[0]&=0x4u;goto L8c6566d4; /* and #0x4,r0 */
L8c6566d4: *stop=0x8c6566d4u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[0],4);if(io_status)return io_status;goto L8c6566d6; /* mov.l r0,@r10 */
L8c6566d6: *stop=0x8c6566d6u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6566d8; /* mov.l @(0x10,r4),r0 */
L8c6566d8: *stop=0x8c6566d8u;r[0]&=0x8u;goto L8c6566da; /* and #0x8,r0 */
L8c6566da: *stop=0x8c6566dau;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[0],4);if(io_status)return io_status;goto L8c6566dc; /* mov.l r0,@r9 */
L8c6566dc: *stop=0x8c6566dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6566de; /* mov.l r3,@-r15 */
L8c6566de: *stop=0x8c6566deu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[7]=loaded;}goto L8c6566e0; /* mov.l @r9,r7 */
L8c6566e0: *stop=0x8c6566e0u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6566e2; /* mov.l @r10,r6 */
L8c6566e2: *stop=0x8c6566e2u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6566e4; /* mov.l @r11,r5 */
L8c6566e4: target=0x8c656f50u;*pr=0x8c6566e8u;*stop=0x8c6566e6u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6566e8; /* bsr 0x8c656f50 */
L8c6566e8: *stop=0x8c6566e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6566ea; /* mov.l r13,@-r15 */
L8c6566ea: *stop=0x8c6566eau;r[3]=0u;goto L8c6566ec; /* mov #0x0,r3 */
L8c6566ec: *stop=0x8c6566ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6566ee; /* mov.l r3,@-r15 */
L8c6566ee: *stop=0x8c6566eeu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[7]=loaded;}goto L8c6566f0; /* mov.l @r9,r7 */
L8c6566f0: *stop=0x8c6566f0u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6566f2; /* mov.l @r10,r6 */
L8c6566f2: *stop=0x8c6566f2u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6566f4; /* mov.l @r11,r5 */
L8c6566f4: target=0x8c657096u;*pr=0x8c6566f8u;*stop=0x8c6566f6u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6566f8; /* bsr 0x8c657096 */
L8c6566f8: *stop=0x8c6566f8u;r[15]+=12u;goto L8c6566fa; /* add #0xc,r15 */
L8c6566fa: *stop=0x8c6566fau;r[11]=r[0];goto L8c6566fc; /* mov r0,r11 */
L8c6566fc: target=0x8c6573c4u;*pr=0x8c656700u;*stop=0x8c6566feu;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656700; /* bsr 0x8c6573c4 */
L8c656700: *stop=0x8c656700u;r[0]=88u;goto L8c656702; /* mov #0x58,r0 */
L8c656702: *stop=0x8c656702u;{uint32_t loaded=sx(m,bus,&io_status,0x8c656780u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c656704; /* mov.w 0x8c656780,r4 */
L8c656704: *stop=0x8c656704u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656706; /* mov.l @(r0,r14),r2 */
L8c656706: *stop=0x8c656706u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6567a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656708; /* mov.l 0x8c6567a0,r3 */
L8c656708: *stop=0x8c656708u;*sr=(*sr&~1u)|(r[2]&1u);r[2]>>=1;goto L8c65670a; /* shlr r2 */
L8c65670a: *stop=0x8c65670au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c65670c; /* mov.l r2,@(r0,r14) */
L8c65670c: *stop=0x8c65670cu;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65670e; /* mov.l @(0x24,r14),r6 */
L8c65670e: target=r[3];*pr=0x8c656712u;*stop=0x8c656710u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656712; /* jsr @r3 */
L8c656712: *stop=0x8c656712u;r[0]=96u;goto L8c656714; /* mov #0x60,r0 */
L8c656714: *stop=0x8c656714u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6567a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656716; /* mov.l 0x8c6567a4,r3 */
L8c656716: *stop=0x8c656716u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c656718; /* mov.l @(r0,r14),r4 */
L8c656718: *stop=0x8c656718u;r[0]=88u;goto L8c65671a; /* mov #0x58,r0 */
L8c65671a: *stop=0x8c65671au;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65671c; /* mov.l @(r0,r14),r2 */
L8c65671c: *stop=0x8c65671cu;r[4]=0u-r[4];goto L8c65671e; /* neg r4,r4 */
L8c65671e: *stop=0x8c65671eu;r[4]+=r[3];goto L8c656720; /* add r3,r4 */
L8c656720: *stop=0x8c656720u;*sr=(*sr&~1u)|(r[2]>r[4]);goto L8c656722; /* cmp/hi r4,r2 */
L8c656722: if(!(*sr&1u))goto L8c656726;goto L8c656724; /* bf 0x8c656726 */
L8c656724: *stop=0x8c656724u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c656726; /* mov.l r4,@(r0,r14) */
L8c656726: *stop=0x8c656726u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6567a8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656728; /* mov.l 0x8c6567a8,r6 */
L8c656728: *stop=0x8c656728u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6567acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65672a; /* mov.l 0x8c6567ac,r3 */
L8c65672a: *stop=0x8c65672au;r[5]=r[6];goto L8c65672c; /* mov r6,r5 */
L8c65672c: *stop=0x8c65672cu;r[5]+=4294967292u;goto L8c65672e; /* add #-0x4,r5 */
L8c65672e: target=r[3];*pr=0x8c656732u;*stop=0x8c656730u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656732; /* jsr @r3 */
L8c656732: *stop=0x8c656732u;r[12]=r[0];goto L8c656734; /* mov r0,r12 */
L8c656734: *stop=0x8c656734u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c656736; /* tst r12,r12 */
L8c656736: if(!(*sr&1u))goto L8c6567de;goto L8c656738; /* bf 0x8c6567de */
L8c656738: *stop=0x8c656738u;r[0]=88u;goto L8c65673a; /* mov #0x58,r0 */
L8c65673a: *stop=0x8c65673au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6567b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65673c; /* mov.l 0x8c6567b0,r2 */
L8c65673c: *stop=0x8c65673cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65673e; /* mov.l @(r0,r14),r3 */
L8c65673e: *stop=0x8c65673eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c656740; /* mov.l r3,@r2 */
L8c656740: *stop=0x8c656740u;r[1]=0u;goto L8c656742; /* mov #0x0,r1 */
L8c656742: *stop=0x8c656742u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6567b4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656744; /* mov.l 0x8c6567b4,r7 */
L8c656744: *stop=0x8c656744u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c656746; /* mov.l r1,@-r15 */
L8c656746: *stop=0x8c656746u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656748; /* mov.l @(0x18,r14),r3 */
L8c656748: *stop=0x8c656748u;r[3]=~r[3];goto L8c65674a; /* not r3,r3 */
L8c65674a: *stop=0x8c65674au;r[11]&=r[3];goto L8c65674c; /* and r3,r11 */
L8c65674c: *stop=0x8c65674cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65674e; /* mov.l r11,@-r15 */
L8c65674e: *stop=0x8c65674eu;if(!valid_access(r[7]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c656750; /* mov.l @(0xc,r7),r7 */
L8c656750: *stop=0x8c656750u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6567b4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656752; /* mov.l 0x8c6567b4,r6 */
L8c656752: *stop=0x8c656752u;r[3]=4294967289u;goto L8c656754; /* mov #-0x7,r3 */
L8c656754: *stop=0x8c656754u;r[7]=(r[3]&0x80000000u)?((r[3]&31u)?r[7]>>((0u-r[3])&31u):0u):(r[7]<<(r[3]&31u));goto L8c656756; /* shld r3,r7 */
L8c656756: *stop=0x8c656756u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656758; /* mov.l @(0x8,r6),r6 */
L8c656758: *stop=0x8c656758u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6567b4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65675a; /* mov.l 0x8c6567b4,r5 */
L8c65675a: *stop=0x8c65675au;r[6]=(r[3]&0x80000000u)?((r[3]&31u)?r[6]>>((0u-r[3])&31u):0u):(r[6]<<(r[3]&31u));goto L8c65675c; /* shld r3,r6 */
L8c65675c: *stop=0x8c65675cu;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65675e; /* mov.l @(0x4,r5),r5 */
L8c65675e: *stop=0x8c65675eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6567b4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c656760; /* mov.l 0x8c6567b4,r4 */
L8c656760: *stop=0x8c656760u;r[5]=(r[3]&0x80000000u)?((r[3]&31u)?r[5]>>((0u-r[3])&31u):0u):(r[5]<<(r[3]&31u));goto L8c656762; /* shld r3,r5 */
L8c656762: *stop=0x8c656762u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c656764; /* mov.l @r4,r4 */
L8c656764: *stop=0x8c656764u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6567b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656766; /* mov.l 0x8c6567b8,r2 */
L8c656766: target=r[2];*pr=0x8c65676au;*stop=0x8c656768u;r[4]=(r[3]&0x80000000u)?((r[3]&31u)?r[4]>>((0u-r[3])&31u):0u):(r[4]<<(r[3]&31u));if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65676a; /* jsr @r2 */
L8c65676a: *stop=0x8c65676au;r[15]+=8u;goto L8c65676c; /* add #0x8,r15 */
L8c65676c: *stop=0x8c65676cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65679cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65676e; /* mov.l 0x8c65679c,r3 */
L8c65676e: *stop=0x8c656770u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c6567de; /* bra 0x8c6567de */
L8c6567c0: *stop=0x8c6567c0u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6567c2; /* mov.l @r5,r3 */
L8c6567c2: *stop=0x8c6567c2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c656840u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6567c4; /* mov.w 0x8c656840,r4 */
L8c6567c4: *stop=0x8c6567c4u;r[3]|=r[4];goto L8c6567c6; /* or r4,r3 */
L8c6567c6: *stop=0x8c6567c6u;r[4]=r[3];goto L8c6567c8; /* mov r3,r4 */
L8c6567c8: *stop=0x8c6567c8u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6567ca; /* mov.l r3,@r5 */
L8c6567ca: *stop=0x8c6567cau;{uint32_t loaded=sx(m,bus,&io_status,0x8c656842u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6567cc; /* mov.w 0x8c656842,r3 */
L8c6567cc: *stop=0x8c6567ccu;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c6567ce; /* cmp/eq r3,r4 */
L8c6567ce: if(!(*sr&1u))goto L8c6567de;goto L8c6567d0; /* bf 0x8c6567de */
L8c6567d0: *stop=0x8c6567d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656844u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6567d2; /* mov.l 0x8c656844,r3 */
L8c6567d2: *stop=0x8c6567d2u;r[0]=84u;goto L8c6567d4; /* mov #0x54,r0 */
L8c6567d4: *stop=0x8c6567d4u;r[4]=r[7];goto L8c6567d6; /* mov r7,r4 */
L8c6567d6: *stop=0x8c6567d6u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[7],4);if(io_status)return io_status;goto L8c6567d8; /* mov.l r7,@r5 */
L8c6567d8: target=r[3];*pr=0x8c6567dcu;*stop=0x8c6567dau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6567dc; /* jsr @r3 */
L8c6567dc: *stop=0x8c6567dcu;r[12]=r[0];goto L8c6567de; /* mov r0,r12 */
L8c6567de: *stop=0x8c6567deu;r[0]=r[12];goto L8c6567e0; /* mov r12,r0 */
L8c6567e0: *stop=0x8c6567e0u;r[15]+=20u;goto L8c6567e2; /* add #0x14,r15 */
L8c6567e2: *stop=0x8c6567e2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6567e4; /* lds.l @r15+,PR */
L8c6567e4: *stop=0x8c6567e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6567e6; /* mov.l @r15+,r9 */
L8c6567e6: *stop=0x8c6567e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6567e8; /* mov.l @r15+,r10 */
L8c6567e8: *stop=0x8c6567e8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6567ea; /* mov.l @r15+,r11 */
L8c6567ea: *stop=0x8c6567eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6567ec; /* mov.l @r15+,r12 */
L8c6567ec: *stop=0x8c6567ecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6567ee; /* mov.l @r15+,r13 */
L8c6567ee: *stop=0x8c6567eeu;if(*pr!=entry_pr)return -3;*stop=0x8c6567f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c664352(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c664352;
L8c664352: *stop=0x8c664352u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c664354; /* mov.l r14,@-r15 */
L8c664354: *stop=0x8c664354u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c664356; /* mov.l r13,@-r15 */
L8c664356: *stop=0x8c664356u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c664358; /* mov.l r12,@-r15 */
L8c664358: *stop=0x8c664358u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c66435a; /* mov.l r11,@-r15 */
L8c66435a: *stop=0x8c66435au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c66435c; /* mov.l r10,@-r15 */
L8c66435c: *stop=0x8c66435cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c66435e; /* mov.l r9,@-r15 */
L8c66435e: *stop=0x8c66435eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c664360; /* mov.l r8,@-r15 */
L8c664360: *stop=0x8c664360u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c664362; /* sts.l PR,@-r15 */
L8c664362: *stop=0x8c664362u;r[15]+=4294967292u;goto L8c664364; /* add #-0x4,r15 */
L8c664364: *stop=0x8c664364u;r[0]=r[5];goto L8c664366; /* mov r5,r0 */
L8c664366: *stop=0x8c664366u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c664368; /* cmp/pz r0 */
L8c664368: *stop=0x8c664368u;r[12]=r[5];goto L8c66436a; /* mov r5,r12 */
L8c66436a: take=!(*sr&1u);*stop=0x8c66436cu;r[9]=r[6];if(take)goto L8c6643a0;goto L8c66436e; /* bf/s 0x8c6643a0 */
L8c66436e: *stop=0x8c664370u;r[0]&=0x1fu;goto L8c6643aa; /* bra 0x8c6643aa */
L8c6643a0: *stop=0x8c6643a0u;r[0]=~r[0];goto L8c6643a2; /* not r0,r0 */
L8c6643a2: *stop=0x8c6643a2u;r[0]+=1u;goto L8c6643a4; /* add #0x1,r0 */
L8c6643a4: *stop=0x8c6643a4u;r[0]&=0x1fu;goto L8c6643a6; /* and #0x1f,r0 */
L8c6643a6: *stop=0x8c6643a6u;r[0]=~r[0];goto L8c6643a8; /* not r0,r0 */
L8c6643a8: *stop=0x8c6643a8u;r[0]+=1u;goto L8c6643aa; /* add #0x1,r0 */
L8c6643aa: *stop=0x8c6643aau;r[5]=r[0];goto L8c6643ac; /* mov r0,r5 */
L8c6643ac: *stop=0x8c6643acu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c6643ae; /* tst r5,r5 */
L8c6643ae: if((*sr&1u))goto L8c6643b6;goto L8c6643b0; /* bt 0x8c6643b6 */
L8c6643b0: *stop=0x8c6643b0u;r[3]=32u;goto L8c6643b2; /* mov #0x20,r3 */
L8c6643b2: *stop=0x8c6643b2u;r[3]-=r[5];goto L8c6643b4; /* sub r5,r3 */
L8c6643b4: *stop=0x8c6643b4u;r[12]+=r[3];goto L8c6643b6; /* add r3,r12 */
L8c6643b6: *stop=0x8c6643b6u;r[10]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c6643b8; /* exts.w r4,r10 */
L8c6643b8: *stop=0x8c6643b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664444u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6643ba; /* mov.l 0x8c664444,r11 */
L8c6643ba: *stop=0x8c6643bau;r[3]=r[10];goto L8c6643bc; /* mov r10,r3 */
L8c6643bc: *stop=0x8c6643bcu;r[10]<<=2;goto L8c6643be; /* shll2 r10 */
L8c6643be: *stop=0x8c6643beu;r[10]+=r[3];goto L8c6643c0; /* add r3,r10 */
L8c6643c0: *stop=0x8c6643c0u;r[10]<<=2;goto L8c6643c2; /* shll2 r10 */
L8c6643c2: *stop=0x8c6643c2u;r[2]=r[10];goto L8c6643c4; /* mov r10,r2 */
L8c6643c4: *stop=0x8c6643c4u;r[2]+=r[11];goto L8c6643c6; /* add r11,r2 */
L8c6643c6: *stop=0x8c6643c6u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6643c8; /* mov.l @(0x8,r2),r4 */
L8c6643c8: *stop=0x8c6643c8u;r[5]=0u;goto L8c6643ca; /* mov #0x0,r5 */
L8c6643ca: *stop=0x8c6643cau;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6643cc; /* tst r4,r4 */
L8c6643cc: take=(*sr&1u);*stop=0x8c6643ceu;r[14]=r[5];if(take)goto L8c6643e2;goto L8c6643d0; /* bt/s 0x8c6643e2 */
L8c6643d0: *stop=0x8c6643d0u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6643d2; /* mov.l @(0x10,r4),r1 */
L8c6643d2: *stop=0x8c6643d2u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[12]);goto L8c6643d4; /* cmp/ge r12,r1 */
L8c6643d4: if(!(*sr&1u))goto L8c6643dc;goto L8c6643d6; /* bf 0x8c6643dc */
L8c6643d6: *stop=0x8c6643d6u;r[14]=r[4];goto L8c6643d8; /* mov r4,r14 */
L8c6643d8: *stop=0x8c6643dau;r[4]=r[5];goto L8c6643de; /* bra 0x8c6643de */
L8c6643dc: *stop=0x8c6643dcu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6643de; /* mov.l @(0x8,r4),r4 */
L8c6643de: *stop=0x8c6643deu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6643e0; /* tst r4,r4 */
L8c6643e0: if(!(*sr&1u))goto L8c6643d0;goto L8c6643e2; /* bf 0x8c6643d0 */
L8c6643e2: *stop=0x8c6643e2u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6643e4; /* tst r14,r14 */
L8c6643e4: if(!(*sr&1u))goto L8c6643ea;goto L8c6643e6; /* bf 0x8c6643ea */
L8c6643e6: *stop=0x8c6643e8u;goto L8c664500; /* bra 0x8c664500 */
L8c6643ea: *stop=0x8c6643eau;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6643ec; /* mov.l @(0x10,r14),r3 */
L8c6643ec: *stop=0x8c6643ecu;*sr=(*sr&~1u)|(r[12]==r[3]);goto L8c6643ee; /* cmp/eq r12,r3 */
L8c6643ee: take=!(*sr&1u);*stop=0x8c6643f0u;r[8]=(uint32_t)(int32_t)(int16_t)r[7];if(take)goto L8c66447c;goto L8c6643f2; /* bf/s 0x8c66447c */
L8c6643f2: *stop=0x8c6643f2u;r[13]=r[10];goto L8c6643f4; /* mov r10,r13 */
L8c6643f4: *stop=0x8c6643f4u;r[13]+=r[11];goto L8c6643f6; /* add r11,r13 */
L8c6643f6: *stop=0x8c6643f6u;r[5]=r[13];goto L8c6643f8; /* mov r13,r5 */
L8c6643f8: *stop=0x8c6643f8u;r[4]=r[13];goto L8c6643fa; /* mov r13,r4 */
L8c6643fa: *stop=0x8c6643fau;r[5]+=12u;goto L8c6643fc; /* add #0xc,r5 */
L8c6643fc: *stop=0x8c6643fcu;r[6]=r[14];goto L8c6643fe; /* mov r14,r6 */
L8c6643fe: target=0x8c664b52u;*pr=0x8c664402u;*stop=0x8c664400u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c664402; /* bsr 0x8c664b52 */
L8c664402: *stop=0x8c664402u;r[5]=r[13];goto L8c664404; /* mov r13,r5 */
L8c664404: *stop=0x8c664404u;r[5]+=4u;goto L8c664406; /* add #0x4,r5 */
L8c664406: *stop=0x8c664406u;r[6]=r[14];goto L8c664408; /* mov r14,r6 */
L8c664408: target=0x8c664b12u;*pr=0x8c66440cu;*stop=0x8c66440au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66440c; /* bsr 0x8c664b12 */
L8c66440c: *stop=0x8c66440cu;r[0]=r[8];goto L8c66440e; /* mov r8,r0 */
L8c66440e: *stop=0x8c66440eu;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c664410; /* cmp/eq #0x8,r0 */
L8c664410: *stop=0x8c664410u;r[3]=1u;goto L8c664412; /* mov #0x1,r3 */
L8c664412: take=(*sr&1u);*stop=0x8c664414u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[3],2);if(io_status)return io_status;if(take)goto L8c664420;goto L8c664416; /* bt/s 0x8c664420 */
L8c664416: *stop=0x8c664416u;{uint32_t loaded=sx(m,bus,&io_status,0x8c664440u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c664418; /* mov.w 0x8c664440,r1 */
L8c664418: *stop=0x8c664418u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c66441a; /* cmp/eq r1,r0 */
L8c66441a: if((*sr&1u))goto L8c664460;goto L8c66441c; /* bt 0x8c664460 */
L8c66441c: *stop=0x8c66441eu;goto L8c664500; /* bra 0x8c664500 */
L8c664420: *stop=0x8c664420u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664422; /* mov.l @(0xc,r14),r2 */
L8c664422: *stop=0x8c664422u;r[4]=r[9];goto L8c664424; /* mov r9,r4 */
L8c664424: *stop=0x8c664424u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c664426; /* mov.l r2,@(0x1c,r4) */
L8c664426: *stop=0x8c664426u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[4],4);if(io_status)return io_status;goto L8c664428; /* mov.l r4,@(0x14,r14) */
L8c664428: *stop=0x8c664428u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[0]=loaded;}goto L8c66442a; /* mov.w @r14,r0 */
L8c66442a: *stop=0x8c66442au;r[0]|=0x8u;goto L8c66442c; /* or #0x8,r0 */
L8c66442c: *stop=0x8c66442eu;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[0],2);if(io_status)return io_status;goto L8c664470; /* bra 0x8c664470 */
L8c664460: *stop=0x8c664460u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664462; /* mov.l @(0xc,r14),r2 */
L8c664462: *stop=0x8c664462u;r[4]=r[9];goto L8c664464; /* mov r9,r4 */
L8c664464: *stop=0x8c664464u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6644e0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c664466; /* mov.w 0x8c6644e0,r3 */
L8c664466: *stop=0x8c664466u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c664468; /* mov.l r2,@r4 */
L8c664468: *stop=0x8c664468u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[4],4);if(io_status)return io_status;goto L8c66446a; /* mov.l r4,@(0x14,r14) */
L8c66446a: *stop=0x8c66446au;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[2]=loaded;}goto L8c66446c; /* mov.w @r14,r2 */
L8c66446c: *stop=0x8c66446cu;r[2]|=r[3];goto L8c66446e; /* or r3,r2 */
L8c66446e: *stop=0x8c66446eu;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c664470; /* mov.w r2,@r14 */
L8c664470: *stop=0x8c664470u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[0]=loaded;}goto L8c664472; /* mov.w @r14,r0 */
L8c664472: *stop=0x8c664472u;r[10]+=r[11];goto L8c664474; /* add r11,r10 */
L8c664474: *stop=0x8c664474u;r[0]|=0x4u;goto L8c664476; /* or #0x4,r0 */
L8c664476: *stop=0x8c664476u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[0],2);if(io_status)return io_status;goto L8c664478; /* mov.w r0,@r14 */
L8c664478: *stop=0x8c66447au;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66450e; /* bra 0x8c66450e */
L8c66447c: target=0x8c664ac0u;*pr=0x8c664480u;*stop=0x8c66447eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c664480; /* bsr 0x8c664ac0 */
L8c664480: *stop=0x8c664480u;r[13]=r[0];goto L8c664482; /* mov r0,r13 */
L8c664482: *stop=0x8c664482u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c664484; /* tst r13,r13 */
L8c664484: if((*sr&1u))goto L8c664500;goto L8c664486; /* bt 0x8c664500 */
L8c664486: *stop=0x8c664486u;r[5]=r[10];goto L8c664488; /* mov r10,r5 */
L8c664488: *stop=0x8c664488u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66448a; /* mov.l @(0xc,r14),r3 */
L8c66448a: *stop=0x8c66448au;r[5]+=r[11];goto L8c66448c; /* add r11,r5 */
L8c66448c: *stop=0x8c66448cu;r[6]=r[13];goto L8c66448e; /* mov r13,r6 */
L8c66448e: *stop=0x8c66448eu;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[3],4);if(io_status)return io_status;goto L8c664490; /* mov.l r3,@(0xc,r13) */
L8c664490: *stop=0x8c664490u;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[12],4);if(io_status)return io_status;goto L8c664492; /* mov.l r12,@(0x10,r13) */
L8c664492: *stop=0x8c664492u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c664494; /* mov.l @(0x10,r14),r4 */
L8c664494: *stop=0x8c664494u;r[4]-=r[12];goto L8c664496; /* sub r12,r4 */
L8c664496: *stop=0x8c664496u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[4],4);if(io_status)return io_status;goto L8c664498; /* mov.l r4,@(0x10,r14) */
L8c664498: *stop=0x8c664498u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66449a; /* mov.l @(0xc,r14),r2 */
L8c66449a: *stop=0x8c66449au;r[12]+=r[2];goto L8c66449c; /* add r2,r12 */
L8c66449c: *stop=0x8c66449cu;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[12],4);if(io_status)return io_status;goto L8c66449e; /* mov.l r12,@(0xc,r14) */
L8c66449e: *stop=0x8c66449eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c6644a0; /* mov.l r5,@r15 */
L8c6644a0: *stop=0x8c6644a0u;r[5]+=4u;goto L8c6644a2; /* add #0x4,r5 */
L8c6644a2: target=0x8c664b12u;*pr=0x8c6644a6u;*stop=0x8c6644a4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6644a6; /* bsr 0x8c664b12 */
L8c6644a6: *stop=0x8c6644a6u;r[0]=r[8];goto L8c6644a8; /* mov r8,r0 */
L8c6644a8: *stop=0x8c6644a8u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c6644aa; /* cmp/eq #0x8,r0 */
L8c6644aa: if((*sr&1u))goto L8c6644b6;goto L8c6644ac; /* bt 0x8c6644b6 */
L8c6644ac: *stop=0x8c6644acu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6644e0u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6644ae; /* mov.w 0x8c6644e0,r1 */
L8c6644ae: *stop=0x8c6644aeu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c6644b0; /* cmp/eq r1,r0 */
L8c6644b0: if((*sr&1u))goto L8c6644c6;goto L8c6644b2; /* bt 0x8c6644c6 */
L8c6644b2: *stop=0x8c6644b4u;goto L8c664500; /* bra 0x8c664500 */
L8c6644b6: *stop=0x8c6644b6u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6644b8; /* mov.l @(0xc,r13),r2 */
L8c6644b8: *stop=0x8c6644b8u;r[4]=r[9];goto L8c6644ba; /* mov r9,r4 */
L8c6644ba: *stop=0x8c6644bau;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c6644bc; /* mov.l r2,@(0x1c,r4) */
L8c6644bc: *stop=0x8c6644bcu;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[4],4);if(io_status)return io_status;goto L8c6644be; /* mov.l r4,@(0x14,r13) */
L8c6644be: *stop=0x8c6644beu;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c6644c0; /* mov.w @r13,r0 */
L8c6644c0: *stop=0x8c6644c0u;r[0]|=0x8u;goto L8c6644c2; /* or #0x8,r0 */
L8c6644c2: *stop=0x8c6644c4u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c664504; /* bra 0x8c664504 */
L8c6644c6: *stop=0x8c6644c6u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6644c8; /* mov.l @(0xc,r13),r2 */
L8c6644c8: *stop=0x8c6644c8u;r[4]=r[9];goto L8c6644ca; /* mov r9,r4 */
L8c6644ca: *stop=0x8c6644cau;{uint32_t loaded=sx(m,bus,&io_status,0x8c6644e0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6644cc; /* mov.w 0x8c6644e0,r3 */
L8c6644cc: *stop=0x8c6644ccu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c6644ce; /* mov.l r2,@r4 */
L8c6644ce: *stop=0x8c6644ceu;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[4],4);if(io_status)return io_status;goto L8c6644d0; /* mov.l r4,@(0x14,r13) */
L8c6644d0: *stop=0x8c6644d0u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[2]=loaded;}goto L8c6644d2; /* mov.w @r13,r2 */
L8c6644d2: *stop=0x8c6644d2u;r[2]|=r[3];goto L8c6644d4; /* or r3,r2 */
L8c6644d4: *stop=0x8c6644d6u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[2],2);if(io_status)return io_status;goto L8c664504; /* bra 0x8c664504 */
L8c664500: *stop=0x8c664502u;r[0]=3u;goto L8c664516; /* bra 0x8c664516 */
L8c664504: *stop=0x8c664504u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c664506; /* mov.w @r13,r0 */
L8c664506: *stop=0x8c664506u;r[10]+=r[11];goto L8c664508; /* add r11,r10 */
L8c664508: *stop=0x8c664508u;r[0]|=0x4u;goto L8c66450a; /* or #0x4,r0 */
L8c66450a: *stop=0x8c66450au;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c66450c; /* mov.w r0,@r13 */
L8c66450c: *stop=0x8c66450cu;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66450e; /* mov.l @(0x10,r13),r3 */
L8c66450e: *stop=0x8c66450eu;if(!valid_access(r[10]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664510; /* mov.l @(0x10,r10),r2 */
L8c664510: *stop=0x8c664510u;r[0]=0u;goto L8c664512; /* mov #0x0,r0 */
L8c664512: *stop=0x8c664512u;r[2]-=r[3];goto L8c664514; /* sub r3,r2 */
L8c664514: *stop=0x8c664514u;if(!valid_access(r[10]+16u,4u))return -3;wr(m,bus,&io_status,r[10]+0x10u,r[2],4);if(io_status)return io_status;goto L8c664516; /* mov.l r2,@(0x10,r10) */
L8c664516: *stop=0x8c664516u;r[15]+=4u;goto L8c664518; /* add #0x4,r15 */
L8c664518: *stop=0x8c664518u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66451a; /* lds.l @r15+,PR */
L8c66451a: *stop=0x8c66451au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c66451c; /* mov.l @r15+,r8 */
L8c66451c: *stop=0x8c66451cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c66451e; /* mov.l @r15+,r9 */
L8c66451e: *stop=0x8c66451eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c664520; /* mov.l @r15+,r10 */
L8c664520: *stop=0x8c664520u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c664522; /* mov.l @r15+,r11 */
L8c664522: *stop=0x8c664522u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c664524; /* mov.l @r15+,r12 */
L8c664524: *stop=0x8c664524u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c664526; /* mov.l @r15+,r13 */
L8c664526: *stop=0x8c664526u;if(*pr!=entry_pr)return -3;*stop=0x8c664528u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c66563e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c66563e;
L8c66563e: *stop=0x8c66563eu;r[5]=4294967288u;goto L8c665640; /* mov #-0x8,r5 */
L8c665640: *stop=0x8c665640u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6656acu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c665642; /* mov.l 0x8c6656ac,r6 */
L8c665642: *stop=0x8c665642u;r[2]=3u;goto L8c665644; /* mov #0x3,r2 */
L8c665644: *stop=0x8c665644u;r[5]&=r[4];goto L8c665646; /* and r4,r5 */
L8c665646: *stop=0x8c665646u;r[3]=4u;goto L8c665648; /* mov #0x4,r3 */
L8c665648: *stop=0x8c665648u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c66564a; /* shlr r5 */
L8c66564a: *stop=0x8c66564au;r[2]&=r[4];goto L8c66564c; /* and r4,r2 */
L8c66564c: *stop=0x8c66564cu;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c66564e; /* tst r4,r3 */
L8c66564e: *stop=0x8c66564eu;r[5]+=r[2];goto L8c665650; /* add r2,r5 */
L8c665650: if((*sr&1u))goto L8c665654;goto L8c665652; /* bt 0x8c665654 */
L8c665652: *stop=0x8c665652u;r[5]+=r[6];goto L8c665654; /* add r6,r5 */
L8c665654: *stop=0x8c665654u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6656b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665656; /* mov.l 0x8c6656b0,r3 */
L8c665656: *stop=0x8c665656u;*sr=(*sr&~1u)|(r[4]>=r[3]);goto L8c665658; /* cmp/hs r3,r4 */
L8c665658: if(!(*sr&1u))goto L8c66565c;goto L8c66565a; /* bf 0x8c66565c */
L8c66565a: *stop=0x8c66565au;r[5]+=r[6];goto L8c66565c; /* add r6,r5 */
L8c66565c: *stop=0x8c66565cu;if(*pr!=entry_pr)return -3;*stop=0x8c66565eu;r[0]=r[5];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c665ae0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c665ae0;
L8c665ae0: *stop=0x8c665ae0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c665ae2; /* mov.l r14,@-r15 */
L8c665ae2: *stop=0x8c665ae2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c665ae4; /* mov.l r13,@-r15 */
L8c665ae4: *stop=0x8c665ae4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c665ae6; /* mov.l r12,@-r15 */
L8c665ae6: *stop=0x8c665ae6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c665ae8; /* mov.l r11,@-r15 */
L8c665ae8: *stop=0x8c665ae8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c665aea; /* mov.l r10,@-r15 */
L8c665aea: *stop=0x8c665aeau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c665aec; /* sts.l PR,@-r15 */
L8c665aec: *stop=0x8c665aecu;{uint32_t loaded=sx(m,bus,&io_status,0x8c665be0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c665aee; /* mov.w 0x8c665be0,r0 */
L8c665aee: *stop=0x8c665aeeu;r[15]+=r[0];goto L8c665af0; /* add r0,r15 */
L8c665af0: *stop=0x8c665af0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665be8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665af2; /* mov.l 0x8c665be8,r3 */
L8c665af2: *stop=0x8c665af2u;r[2]=r[15];goto L8c665af4; /* mov r15,r2 */
L8c665af4: *stop=0x8c665af4u;r[2]+=4u;goto L8c665af6; /* add #0x4,r2 */
L8c665af6: *stop=0x8c665af6u;r[14]=r[4];goto L8c665af8; /* mov r4,r14 */
L8c665af8: *stop=0x8c665af8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665becu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c665afa; /* mov.l 0x8c665bec,r4 */
L8c665afa: *stop=0x8c665afau;r[12]=r[7];goto L8c665afc; /* mov r7,r12 */
L8c665afc: *stop=0x8c665afcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c665bf0u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c665afe; /* mov.l 0x8c665bf0,r13 */
L8c665afe: *stop=0x8c665afeu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c665b00; /* mov.l r6,@r15 */
L8c665b00: *stop=0x8c665b00u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665b02; /* mov.l @(0x4,r3),r0 */
L8c665b02: *stop=0x8c665b02u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c665b04; /* mov.l @r3,r1 */
L8c665b04: *stop=0x8c665b04u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c665b06; /* mov.l r1,@r2 */
L8c665b06: *stop=0x8c665b06u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c665b08; /* mov.l r0,@(0x4,r2) */
L8c665b08: *stop=0x8c665b08u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665b0a; /* mov.l @(0x8,r3),r1 */
L8c665b0a: *stop=0x8c665b0au;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665b0c; /* mov.l @(0xc,r3),r0 */
L8c665b0c: *stop=0x8c665b0cu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c665b0e; /* mov.l r1,@(0x8,r2) */
L8c665b0e: *stop=0x8c665b0eu;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[0],4);if(io_status)return io_status;goto L8c665b10; /* mov.l r0,@(0xc,r2) */
L8c665b10: *stop=0x8c665b10u;if(!valid_access(r[3]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665b12; /* mov.l @(0x14,r3),r0 */
L8c665b12: *stop=0x8c665b12u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665b14; /* mov.l @(0x10,r3),r1 */
L8c665b14: *stop=0x8c665b14u;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[1],4);if(io_status)return io_status;goto L8c665b16; /* mov.l r1,@(0x10,r2) */
L8c665b16: *stop=0x8c665b16u;r[1]=r[13];goto L8c665b18; /* mov r13,r1 */
L8c665b18: *stop=0x8c665b18u;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[0],4);if(io_status)return io_status;goto L8c665b1a; /* mov.l r0,@(0x14,r2) */
L8c665b1a: *stop=0x8c665b1au;r[1]+=28u;goto L8c665b1c; /* add #0x1c,r1 */
L8c665b1c: *stop=0x8c665b1cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665b1e; /* mov.l @r4,r2 */
L8c665b1e: *stop=0x8c665b1eu;r[2]<<=2;goto L8c665b20; /* shll2 r2 */
L8c665b20: *stop=0x8c665b20u;r[2]<<=2;goto L8c665b22; /* shll2 r2 */
L8c665b22: *stop=0x8c665b22u;r[2]+=r[1];goto L8c665b24; /* add r1,r2 */
L8c665b24: *stop=0x8c665b24u;r[1]=r[13];goto L8c665b26; /* mov r13,r1 */
L8c665b26: *stop=0x8c665b26u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c665b28; /* mov.l r14,@r2 */
L8c665b28: *stop=0x8c665b28u;r[1]+=28u;goto L8c665b2a; /* add #0x1c,r1 */
L8c665b2a: *stop=0x8c665b2au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665b2c; /* mov.l @r4,r2 */
L8c665b2c: *stop=0x8c665b2cu;r[2]<<=2;goto L8c665b2e; /* shll2 r2 */
L8c665b2e: *stop=0x8c665b2eu;r[2]<<=2;goto L8c665b30; /* shll2 r2 */
L8c665b30: *stop=0x8c665b30u;r[2]+=r[1];goto L8c665b32; /* add r1,r2 */
L8c665b32: *stop=0x8c665b32u;r[1]=r[13];goto L8c665b34; /* mov r13,r1 */
L8c665b34: *stop=0x8c665b34u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[5],4);if(io_status)return io_status;goto L8c665b36; /* mov.l r5,@(0x4,r2) */
L8c665b36: *stop=0x8c665b36u;r[1]+=28u;goto L8c665b38; /* add #0x1c,r1 */
L8c665b38: *stop=0x8c665b38u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665b3a; /* mov.l @r4,r2 */
L8c665b3a: *stop=0x8c665b3au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665b3c; /* mov.l @r15,r0 */
L8c665b3c: *stop=0x8c665b3cu;r[2]<<=2;goto L8c665b3e; /* shll2 r2 */
L8c665b3e: *stop=0x8c665b3eu;r[2]<<=2;goto L8c665b40; /* shll2 r2 */
L8c665b40: *stop=0x8c665b40u;r[2]+=r[1];goto L8c665b42; /* add r1,r2 */
L8c665b42: *stop=0x8c665b42u;r[1]=r[13];goto L8c665b44; /* mov r13,r1 */
L8c665b44: *stop=0x8c665b44u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[0],4);if(io_status)return io_status;goto L8c665b46; /* mov.l r0,@(0x8,r2) */
L8c665b46: *stop=0x8c665b46u;r[1]+=28u;goto L8c665b48; /* add #0x1c,r1 */
L8c665b48: *stop=0x8c665b48u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665be2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c665b4a; /* mov.w 0x8c665be2,r0 */
L8c665b4a: *stop=0x8c665b4au;if(!valid_access(r[13]+24u,4u))return -3;wr(m,bus,&io_status,r[13]+0x18u,r[12],4);if(io_status)return io_status;goto L8c665b4c; /* mov.l r12,@(0x18,r13) */
L8c665b4c: *stop=0x8c665b4cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665b4e; /* mov.l @r4,r2 */
L8c665b4e: *stop=0x8c665b4eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665b50; /* mov.l @(r0,r15),r0 */
L8c665b50: *stop=0x8c665b50u;r[2]<<=2;goto L8c665b52; /* shll2 r2 */
L8c665b52: *stop=0x8c665b52u;r[2]<<=2;goto L8c665b54; /* shll2 r2 */
L8c665b54: *stop=0x8c665b54u;r[2]+=r[1];goto L8c665b56; /* add r1,r2 */
L8c665b56: *stop=0x8c665b56u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[0],4);if(io_status)return io_status;goto L8c665b58; /* mov.l r0,@(0xc,r2) */
L8c665b58: *stop=0x8c665b58u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665b5a; /* mov.l @r4,r0 */
L8c665b5a: *stop=0x8c665b5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c665bf4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665b5c; /* mov.l 0x8c665bf4,r2 */
L8c665b5c: *stop=0x8c665b5cu;r[0]+=1u;goto L8c665b5e; /* add #0x1,r0 */
L8c665b5e: *stop=0x8c665b5eu;r[0]&=0x1u;goto L8c665b60; /* and #0x1,r0 */
L8c665b60: *stop=0x8c665b60u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c665b62; /* mov.l r0,@r4 */
L8c665b62: *stop=0x8c665b62u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[12],4);if(io_status)return io_status;goto L8c665b64; /* mov.l r12,@r2 */
L8c665b64: *stop=0x8c665b64u;r[0]=84u;goto L8c665b66; /* mov #0x54,r0 */
L8c665b66: *stop=0x8c665b66u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c665b68; /* mov.l @(r0,r13),r1 */
L8c665b68: *stop=0x8c665b68u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c665b6a; /* tst r1,r1 */
L8c665b6a: if((*sr&1u))goto L8c665b74;goto L8c665b6c; /* bt 0x8c665b74 */
L8c665b6c: *stop=0x8c665b6cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c665be4u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665b6e; /* mov.w 0x8c665be4,r4 */
L8c665b6e: *stop=0x8c665b6eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c665b70; /* mov.l @r15,r6 */
L8c665b70: target=0x8c665cc0u;*pr=0x8c665b74u;*stop=0x8c665b72u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665b74; /* bsr 0x8c665cc0 */
L8c665b74: *stop=0x8c665b74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665bf8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665b76; /* mov.l 0x8c665bf8,r2 */
L8c665b76: target=r[2];*pr=0x8c665b7au;*stop=0x8c665b78u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665b7a; /* jsr @r2 */
L8c665b7a: *stop=0x8c665b7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c665bfcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665b7c; /* mov.l 0x8c665bfc,r3 */
L8c665b7c: *stop=0x8c665b7cu;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c665b7e; /* cmp/eq r3,r0 */
L8c665b7e: if(!(*sr&1u))goto L8c665c66;goto L8c665b80; /* bf 0x8c665c66 */
L8c665b80: *stop=0x8c665b80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665c00u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665b82; /* mov.l 0x8c665c00,r2 */
L8c665b82: *stop=0x8c665b82u;r[0]=96u;goto L8c665b84; /* mov #0x60,r0 */
L8c665b84: *stop=0x8c665b84u;r[5]=r[14];goto L8c665b86; /* mov r14,r5 */
L8c665b86: target=r[2];*pr=0x8c665b8au;*stop=0x8c665b88u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665b8a; /* jsr @r2 */
L8c665b8a: *stop=0x8c665b8au;r[4]=r[0];goto L8c665b8c; /* mov r0,r4 */
L8c665b8c: *stop=0x8c665b8cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c665b8e; /* tst r4,r4 */
L8c665b8e: if((*sr&1u))goto L8c665b94;goto L8c665b90; /* bt 0x8c665b94 */
L8c665b90: *stop=0x8c665b92u;r[0]=r[4];goto L8c665c70; /* bra 0x8c665c70 */
L8c665b94: *stop=0x8c665b94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665c04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665b96; /* mov.l 0x8c665c04,r3 */
L8c665b96: *stop=0x8c665b96u;r[1]=r[15];goto L8c665b98; /* mov r15,r1 */
L8c665b98: *stop=0x8c665b98u;r[1]+=4u;goto L8c665b9a; /* add #0x4,r1 */
L8c665b9a: *stop=0x8c665b9au;r[4]=1u;goto L8c665b9c; /* mov #0x1,r4 */
L8c665b9c: *stop=0x8c665b9cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c665b9e; /* mov.l r4,@r14 */
L8c665b9e: *stop=0x8c665b9eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665ba0; /* mov.l @r3,r2 */
L8c665ba0: *stop=0x8c665ba0u;r[2]<<=2;goto L8c665ba2; /* shll2 r2 */
L8c665ba2: *stop=0x8c665ba2u;r[2]+=r[1];goto L8c665ba4; /* add r1,r2 */
L8c665ba4: *stop=0x8c665ba4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665ba6; /* mov.l @r2,r0 */
L8c665ba6: *stop=0x8c665ba6u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],4);if(io_status)return io_status;goto L8c665ba8; /* mov.l r0,@(0x4,r14) */
L8c665ba8: *stop=0x8c665ba8u;r[0]=r[3];goto L8c665baa; /* mov r3,r0 */
L8c665baa: *stop=0x8c665baau;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665bac; /* mov.l @r0,r2 */
L8c665bac: *stop=0x8c665bacu;r[0]=72u;goto L8c665bae; /* mov #0x48,r0 */
L8c665bae: *stop=0x8c665baeu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c665bb0; /* mov.l r2,@(0x8,r14) */
L8c665bb0: *stop=0x8c665bb0u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c665bb2; /* mov.l @(r0,r13),r3 */
L8c665bb2: *stop=0x8c665bb2u;r[0]=76u;goto L8c665bb4; /* mov #0x4c,r0 */
L8c665bb4: *stop=0x8c665bb4u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[3],4);if(io_status)return io_status;goto L8c665bb6; /* mov.l r3,@(0xc,r14) */
L8c665bb6: *stop=0x8c665bb6u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665bb8; /* mov.l @(r0,r13),r2 */
L8c665bb8: *stop=0x8c665bb8u;r[0]=r[12];goto L8c665bba; /* mov r12,r0 */
L8c665bba: *stop=0x8c665bbau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c665bbc; /* cmp/eq #0x1,r0 */
L8c665bbc: *stop=0x8c665bbcu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c665bbe; /* mov.l r2,@(0x10,r14) */
L8c665bbe: take=!(*sr&1u);*stop=0x8c665bc0u;r[5]=0u;if(take)goto L8c665c20;goto L8c665bc2; /* bf/s 0x8c665c20 */
L8c665bc2: *stop=0x8c665bc4u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[4],4);if(io_status)return io_status;goto L8c665c22; /* bra 0x8c665c22 */
L8c665c20: *stop=0x8c665c20u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[5],4);if(io_status)return io_status;goto L8c665c22; /* mov.l r5,@(0x18,r14) */
L8c665c22: *stop=0x8c665c22u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665ca0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c665c24; /* mov.w 0x8c665ca0,r0 */
L8c665c24: *stop=0x8c665c24u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665c26; /* mov.l @(r0,r15),r0 */
L8c665c26: *stop=0x8c665c26u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c665c28; /* cmp/eq #0x1,r0 */
L8c665c28: if(!(*sr&1u))goto L8c665c6e;goto L8c665c2a; /* bf 0x8c665c6e */
L8c665c2a: *stop=0x8c665c2au;{uint32_t loaded=sx(m,bus,&io_status,0x8c665ca2u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c665c2c; /* mov.w 0x8c665ca2,r13 */
L8c665c2c: *stop=0x8c665c2cu;r[6]=r[15];goto L8c665c2e; /* mov r15,r6 */
L8c665c2e: *stop=0x8c665c2eu;r[4]=r[15];goto L8c665c30; /* mov r15,r4 */
L8c665c30: *stop=0x8c665c30u;r[6]+=28u;goto L8c665c32; /* add #0x1c,r6 */
L8c665c32: *stop=0x8c665c32u;r[4]+=28u;goto L8c665c34; /* add #0x1c,r4 */
L8c665c34: *stop=0x8c665c36u;r[6]+=r[13];goto L8c665c3c; /* bra 0x8c665c3c */
L8c665c38: *stop=0x8c665c38u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c665c3a; /* mov.l r5,@r4 */
L8c665c3a: *stop=0x8c665c3au;r[4]+=4u;goto L8c665c3c; /* add #0x4,r4 */
L8c665c3c: *stop=0x8c665c3cu;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c665c3e; /* cmp/hs r6,r4 */
L8c665c3e: if(!(*sr&1u))goto L8c665c38;goto L8c665c40; /* bf 0x8c665c38 */
L8c665c40: *stop=0x8c665c40u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665c42; /* mov.l @(0x14,r14),r2 */
L8c665c42: *stop=0x8c665c42u;r[3]=0u;goto L8c665c44; /* mov #0x0,r3 */
L8c665c44: *stop=0x8c665c44u;r[11]=r[15];goto L8c665c46; /* mov r15,r11 */
L8c665c46: *stop=0x8c665c46u;r[12]=r[5];goto L8c665c48; /* mov r5,r12 */
L8c665c48: *stop=0x8c665c48u;*sr=(*sr&~1u)|(r[3]>=r[2]);goto L8c665c4a; /* cmp/hs r2,r3 */
L8c665c4a: take=(*sr&1u);*stop=0x8c665c4cu;r[11]+=28u;if(take)goto L8c665c6e;goto L8c665c4e; /* bt/s 0x8c665c6e */
L8c665c4e: *stop=0x8c665c4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ca8u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c665c50; /* mov.l 0x8c665ca8,r10 */
L8c665c50: *stop=0x8c665c50u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c665c52; /* mov.l @(0x1c,r14),r4 */
L8c665c52: *stop=0x8c665c52u;r[5]=r[11];goto L8c665c54; /* mov r11,r5 */
L8c665c54: *stop=0x8c665c54u;r[6]=r[13];goto L8c665c56; /* mov r13,r6 */
L8c665c56: target=r[10];*pr=0x8c665c5au;*stop=0x8c665c58u;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665c5a; /* jsr @r10 */
L8c665c5a: *stop=0x8c665c5au;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665c5c; /* mov.l @(0x14,r14),r3 */
L8c665c5c: *stop=0x8c665c5cu;r[12]+=r[13];goto L8c665c5e; /* add r13,r12 */
L8c665c5e: *stop=0x8c665c5eu;*sr=(*sr&~1u)|(r[12]>=r[3]);goto L8c665c60; /* cmp/hs r3,r12 */
L8c665c60: if(!(*sr&1u))goto L8c665c50;goto L8c665c62; /* bf 0x8c665c50 */
L8c665c62: *stop=0x8c665c64u;goto L8c665c6e; /* bra 0x8c665c6e */
L8c665c66: *stop=0x8c665c66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665cacu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665c68; /* mov.l 0x8c665cac,r2 */
L8c665c68: *stop=0x8c665c68u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665ca4u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665c6a; /* mov.w 0x8c665ca4,r4 */
L8c665c6a: target=r[2];*pr=0x8c665c6eu;*stop=0x8c665c6cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665c6e; /* jsr @r2 */
L8c665c6e: *stop=0x8c665c6eu;r[0]=0u;goto L8c665c70; /* mov #0x0,r0 */
L8c665c70: *stop=0x8c665c70u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665ca6u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c665c72; /* mov.w 0x8c665ca6,r1 */
L8c665c72: *stop=0x8c665c72u;r[15]+=r[1];goto L8c665c74; /* add r1,r15 */
L8c665c74: *stop=0x8c665c74u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c665c76; /* lds.l @r15+,PR */
L8c665c76: *stop=0x8c665c76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c665c78; /* mov.l @r15+,r10 */
L8c665c78: *stop=0x8c665c78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c665c7a; /* mov.l @r15+,r11 */
L8c665c7a: *stop=0x8c665c7au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c665c7c; /* mov.l @r15+,r12 */
L8c665c7c: *stop=0x8c665c7cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c665c7e; /* mov.l @r15+,r13 */
L8c665c7e: *stop=0x8c665c7eu;if(*pr!=entry_pr)return -3;*stop=0x8c665c80u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c6578e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6578e0;
L8c6578e0: *stop=0x8c6578e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657950u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6578e2; /* mov.l 0x8c657950,r5 */
L8c6578e2: *stop=0x8c6578e2u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6578e4; /* mov.l @r5,r3 */
L8c6578e4: *stop=0x8c6578e4u;r[3]|=r[4];goto L8c6578e6; /* or r4,r3 */
L8c6578e6: *stop=0x8c6578e6u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6578e8; /* mov.l r3,@r5 */
L8c6578e8: *stop=0x8c6578e8u;if(*pr!=entry_pr)return -3;*stop=0x8c6578eau;r[0]=r[3];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c657960(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c657960;
L8c657960: *stop=0x8c657960u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a00u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c657962; /* mov.l 0x8c657a00,r5 */
L8c657962: *stop=0x8c657962u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c657964; /* mov.l r4,@r5 */
L8c657964: *stop=0x8c657964u;if(*pr!=entry_pr)return -3;*stop=0x8c657966u;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c657968(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c657968;
L8c657968: *stop=0x8c657968u;{uint32_t loaded=rd(m,bus,&io_status,0x8c657a00u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65796a; /* mov.l 0x8c657a00,r3 */
L8c65796a: *stop=0x8c65796au;if(*pr!=entry_pr)return -3;*stop=0x8c65796cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c663320(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c663320;
L8c663320: *stop=0x8c663320u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c663322; /* mov.l r14,@-r15 */
L8c663322: *stop=0x8c663322u;r[0]=r[4];goto L8c663324; /* mov r4,r0 */
L8c663324: *stop=0x8c663324u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c663326; /* mov.l r13,@-r15 */
L8c663326: *stop=0x8c663326u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c663328; /* cmp/pz r0 */
L8c663328: *stop=0x8c663328u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c66332a; /* mov.l r12,@-r15 */
L8c66332a: *stop=0x8c66332au;r[13]=r[4];goto L8c66332c; /* mov r4,r13 */
L8c66332c: *stop=0x8c66332cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c66332e; /* mov.l r11,@-r15 */
L8c66332e: *stop=0x8c66332eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c663330; /* mov.l r10,@-r15 */
L8c663330: *stop=0x8c663330u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c663332; /* sts.l PR,@-r15 */
L8c663332: take=!(*sr&1u);*stop=0x8c663334u;r[14]=r[5];if(take)goto L8c66333a;goto L8c663336; /* bf/s 0x8c66333a */
L8c663336: *stop=0x8c663338u;r[0]&=0x1fu;goto L8c663344; /* bra 0x8c663344 */
L8c66333a: *stop=0x8c66333au;r[0]=~r[0];goto L8c66333c; /* not r0,r0 */
L8c66333c: *stop=0x8c66333cu;r[0]+=1u;goto L8c66333e; /* add #0x1,r0 */
L8c66333e: *stop=0x8c66333eu;r[0]&=0x1fu;goto L8c663340; /* and #0x1f,r0 */
L8c663340: *stop=0x8c663340u;r[0]=~r[0];goto L8c663342; /* not r0,r0 */
L8c663342: *stop=0x8c663342u;r[0]+=1u;goto L8c663344; /* add #0x1,r0 */
L8c663344: *stop=0x8c663344u;r[4]=r[0];goto L8c663346; /* mov r0,r4 */
L8c663346: *stop=0x8c663346u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c663348; /* tst r4,r4 */
L8c663348: if((*sr&1u))goto L8c663350;goto L8c66334a; /* bt 0x8c663350 */
L8c66334a: *stop=0x8c66334au;r[3]=32u;goto L8c66334c; /* mov #0x20,r3 */
L8c66334c: *stop=0x8c66334cu;r[3]-=r[4];goto L8c66334e; /* sub r4,r3 */
L8c66334e: *stop=0x8c66334eu;r[13]+=r[3];goto L8c663350; /* add r3,r13 */
L8c663350: *stop=0x8c663350u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663440u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c663352; /* mov.l 0x8c663440,r10 */
L8c663352: *stop=0x8c663352u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663444u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c663354; /* mov.l 0x8c663444,r11 */
L8c663354: *stop=0x8c663354u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c663356; /* mov.l @r10,r2 */
L8c663356: *stop=0x8c663356u;*sr=(*sr&~1u)|(r[13]==r[2]);goto L8c663358; /* cmp/eq r13,r2 */
L8c663358: if((*sr&1u))goto L8c6633ac;goto L8c66335a; /* bt 0x8c6633ac */
L8c66335a: *stop=0x8c66335au;{uint32_t loaded=rd(m,bus,&io_status,0x8c663448u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66335c; /* mov.l 0x8c663448,r2 */
L8c66335c: *stop=0x8c66335cu;r[5]=r[13];goto L8c66335e; /* mov r13,r5 */
L8c66335e: *stop=0x8c66335eu;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c663360; /* shll r5 */
L8c663360: *stop=0x8c663360u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c663362; /* mov.l @r2,r0 */
L8c663362: *stop=0x8c663362u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c663364; /* tst #0x4,r0 */
L8c663364: if((*sr&1u))goto L8c663388;goto L8c663366; /* bt 0x8c663388 */
L8c663366: *stop=0x8c663366u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66344cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c663368; /* mov.l 0x8c66344c,r4 */
L8c663368: *stop=0x8c663368u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66336a; /* mov.l @(0x10,r4),r3 */
L8c66336a: *stop=0x8c66336au;if(!valid_access(r[4]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66336c; /* mov.l @(0x24,r4),r1 */
L8c66336c: *stop=0x8c66336cu;*sr=(*sr&~1u)|(r[3]>=r[1]);goto L8c66336e; /* cmp/hs r1,r3 */
L8c66336e: if((*sr&1u))goto L8c663378;goto L8c663370; /* bt 0x8c663378 */
L8c663370: *stop=0x8c663370u;r[6]=r[14];goto L8c663372; /* mov r14,r6 */
L8c663372: *stop=0x8c663372u;r[7]=8u;goto L8c663374; /* mov #0x8,r7 */
L8c663374: *stop=0x8c663376u;r[4]=1u;goto L8c66337e; /* bra 0x8c66337e */
L8c663378: *stop=0x8c663378u;r[4]=0u;goto L8c66337a; /* mov #0x0,r4 */
L8c66337a: *stop=0x8c66337au;r[6]=r[14];goto L8c66337c; /* mov r14,r6 */
L8c66337c: *stop=0x8c66337cu;r[7]=8u;goto L8c66337e; /* mov #0x8,r7 */
L8c66337e: *stop=0x8c66337eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663450u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663380; /* mov.l 0x8c663450,r3 */
L8c663380: target=r[3];*pr=0x8c663384u;*stop=0x8c663382u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663384; /* jsr @r3 */
L8c663384: *stop=0x8c663386u;r[12]=r[0];goto L8c663394; /* bra 0x8c663394 */
L8c663388: *stop=0x8c663388u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663450u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66338a; /* mov.l 0x8c663450,r3 */
L8c66338a: *stop=0x8c66338au;r[6]=r[14];goto L8c66338c; /* mov r14,r6 */
L8c66338c: *stop=0x8c66338cu;r[7]=8u;goto L8c66338e; /* mov #0x8,r7 */
L8c66338e: target=r[3];*pr=0x8c663392u;*stop=0x8c663390u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663392; /* jsr @r3 */
L8c663392: *stop=0x8c663392u;r[12]=r[0];goto L8c663394; /* mov r0,r12 */
L8c663394: *stop=0x8c663394u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c663396; /* tst r12,r12 */
L8c663396: if(!(*sr&1u))goto L8c6633a8;goto L8c663398; /* bf 0x8c6633a8 */
L8c663398: *stop=0x8c663398u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663454u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66339a; /* mov.l 0x8c663454,r2 */
L8c66339a: *stop=0x8c66339au;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[13],4);if(io_status)return io_status;goto L8c66339c; /* mov.l r13,@r10 */
L8c66339c: *stop=0x8c66339cu;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66339e; /* mov.l @(0x1c,r14),r3 */
L8c66339e: *stop=0x8c66339eu;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c6633a0; /* mov.l r3,@r11 */
L8c6633a0: *stop=0x8c6633a0u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6633a2; /* mov.l @(0x1c,r14),r4 */
L8c6633a2: target=r[2];*pr=0x8c6633a6u;*stop=0x8c6633a4u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[13],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6633a6; /* jsr @r2 */
L8c6633a6: *stop=0x8c6633a6u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c6633a8; /* mov.l r0,@(0x1c,r14) */
L8c6633a8: *stop=0x8c6633aau;r[0]=r[12];goto L8c6633c0; /* bra 0x8c6633c0 */
L8c6633ac: *stop=0x8c6633acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663454u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6633ae; /* mov.l 0x8c663454,r2 */
L8c6633ae: *stop=0x8c6633aeu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[13],4);if(io_status)return io_status;goto L8c6633b0; /* mov.l r13,@(0x14,r14) */
L8c6633b0: *stop=0x8c6633b0u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6633b2; /* mov.l @r11,r4 */
L8c6633b2: target=r[2];*pr=0x8c6633b6u;*stop=0x8c6633b4u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6633b6; /* jsr @r2 */
L8c6633b6: *stop=0x8c6633b6u;r[4]=0u;goto L8c6633b8; /* mov #0x0,r4 */
L8c6633b8: *stop=0x8c6633b8u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c6633ba; /* mov.l r0,@(0x1c,r14) */
L8c6633ba: *stop=0x8c6633bau;r[0]=r[4];goto L8c6633bc; /* mov r4,r0 */
L8c6633bc: *stop=0x8c6633bcu;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[4],4);if(io_status)return io_status;goto L8c6633be; /* mov.l r4,@r10 */
L8c6633be: *stop=0x8c6633beu;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[4],4);if(io_status)return io_status;goto L8c6633c0; /* mov.l r4,@r11 */
L8c6633c0: *stop=0x8c6633c0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6633c2; /* lds.l @r15+,PR */
L8c6633c2: *stop=0x8c6633c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6633c4; /* mov.l @r15+,r10 */
L8c6633c4: *stop=0x8c6633c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6633c6; /* mov.l @r15+,r11 */
L8c6633c6: *stop=0x8c6633c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6633c8; /* mov.l @r15+,r12 */
L8c6633c8: *stop=0x8c6633c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6633ca; /* mov.l @r15+,r13 */
L8c6633ca: *stop=0x8c6633cau;if(*pr!=entry_pr)return -3;*stop=0x8c6633ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c664b12(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c664b12;
L8c664b12: *stop=0x8c664b12u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c664b14; /* mov.l @r4,r3 */
L8c664b14: *stop=0x8c664b14u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c664b16; /* tst r3,r3 */
L8c664b16: if(!(*sr&1u))goto L8c664b24;goto L8c664b18; /* bf 0x8c664b24 */
L8c664b18: *stop=0x8c664b18u;r[7]=0u;goto L8c664b1a; /* mov #0x0,r7 */
L8c664b1a: *stop=0x8c664b1au;if(!valid_access(r[6]+4u,4u))return -3;wr(m,bus,&io_status,r[6]+0x4u,r[7],4);if(io_status)return io_status;goto L8c664b1c; /* mov.l r7,@(0x4,r6) */
L8c664b1c: *stop=0x8c664b1cu;if(!valid_access(r[6]+8u,4u))return -3;wr(m,bus,&io_status,r[6]+0x8u,r[7],4);if(io_status)return io_status;goto L8c664b1e; /* mov.l r7,@(0x8,r6) */
L8c664b1e: *stop=0x8c664b1eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[6],4);if(io_status)return io_status;goto L8c664b20; /* mov.l r6,@r4 */
L8c664b20: *stop=0x8c664b22u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[6],4);if(io_status)return io_status;goto L8c664b32; /* bra 0x8c664b32 */
L8c664b24: *stop=0x8c664b24u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[5]=loaded;}goto L8c664b26; /* mov.l @r4,r5 */
L8c664b26: *stop=0x8c664b26u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664b28; /* mov.l @(0x4,r5),r2 */
L8c664b28: *stop=0x8c664b28u;if(!valid_access(r[6]+4u,4u))return -3;wr(m,bus,&io_status,r[6]+0x4u,r[2],4);if(io_status)return io_status;goto L8c664b2a; /* mov.l r2,@(0x4,r6) */
L8c664b2a: *stop=0x8c664b2au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c664b2c; /* mov.l @r4,r3 */
L8c664b2c: *stop=0x8c664b2cu;if(!valid_access(r[6]+8u,4u))return -3;wr(m,bus,&io_status,r[6]+0x8u,r[3],4);if(io_status)return io_status;goto L8c664b2e; /* mov.l r3,@(0x8,r6) */
L8c664b2e: *stop=0x8c664b2eu;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[6],4);if(io_status)return io_status;goto L8c664b30; /* mov.l r6,@(0x4,r5) */
L8c664b30: *stop=0x8c664b30u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[6],4);if(io_status)return io_status;goto L8c664b32; /* mov.l r6,@r4 */
L8c664b32: *stop=0x8c664b32u;if(*pr!=entry_pr)return -3;*stop=0x8c664b34u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_init_8c664b52(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c664b52;
L8c664b52: *stop=0x8c664b52u;r[15]+=4294967288u;goto L8c664b54; /* add #-0x8,r15 */
L8c664b54: *stop=0x8c664b54u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c664b56; /* mov.l @(0x4,r6),r7 */
L8c664b56: *stop=0x8c664b56u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c664b58; /* tst r7,r7 */
L8c664b58: *stop=0x8c664b58u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c664b5a; /* mov.l r7,@r15 */
L8c664b5a: *stop=0x8c664b5au;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664b5c; /* mov.l @(0x8,r6),r3 */
L8c664b5c: take=!(*sr&1u);*stop=0x8c664b5eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;if(take)goto L8c664b66;goto L8c664b60; /* bf/s 0x8c664b66 */
L8c664b60: *stop=0x8c664b60u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c664b62; /* mov.l @(0x8,r6),r1 */
L8c664b62: *stop=0x8c664b64u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c664b6c; /* bra 0x8c664b6c */
L8c664b66: *stop=0x8c664b66u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664b68; /* mov.l @(0x8,r6),r3 */
L8c664b68: *stop=0x8c664b68u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c664b6a; /* mov.l @r15,r2 */
L8c664b6a: *stop=0x8c664b6au;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c664b6c; /* mov.l r3,@(0x8,r2) */
L8c664b6c: *stop=0x8c664b6cu;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664b6e; /* mov.l @(0x8,r6),r2 */
L8c664b6e: *stop=0x8c664b6eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c664b70; /* tst r2,r2 */
L8c664b70: if(!(*sr&1u))goto L8c664ba0;goto L8c664b72; /* bf 0x8c664ba0 */
L8c664b72: *stop=0x8c664b72u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664b74; /* mov.l @(0x4,r6),r2 */
L8c664b74: *stop=0x8c664b76u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[2],4);if(io_status)return io_status;goto L8c664ba6; /* bra 0x8c664ba6 */
L8c664ba0: *stop=0x8c664ba0u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664ba2; /* mov.l @(0x4,r6),r3 */
L8c664ba2: *stop=0x8c664ba2u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c664ba4; /* mov.l @(0x4,r15),r1 */
L8c664ba4: *stop=0x8c664ba4u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[3],4);if(io_status)return io_status;goto L8c664ba6; /* mov.l r3,@(0x4,r1) */
L8c664ba6: *stop=0x8c664ba6u;if(*pr!=entry_pr)return -3;*stop=0x8c664ba8u;r[15]+=8u;return 0; /* rts */
}
