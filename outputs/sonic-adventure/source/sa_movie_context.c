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
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f7800u && (a&0x1fffffffu)<0x005f78c0u)return 1;
 if(n==4u && (a&0x1fffffffu)==0x005f688cu)return 1;
 if((n==1u||n==2u||n==4u) && !(a&(n-1u)) && (a&0x1fffffffu)>=0x00702c00u && (a&0x1fffffffu)+n<=0x00702c04u)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_context_8c10d5a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10d5a8;
L8c10d5a8: *stop=0x8c10d5a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10d5aa; /* mov.l r14,@-r15 */
L8c10d5aa: *stop=0x8c10d5aau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10d5ac; /* mov.l r13,@-r15 */
L8c10d5ac: *stop=0x8c10d5acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10d5ae; /* mov.l r12,@-r15 */
L8c10d5ae: *stop=0x8c10d5aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10d5b0; /* mov.l r11,@-r15 */
L8c10d5b0: *stop=0x8c10d5b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10d5b2; /* mov.l r10,@-r15 */
L8c10d5b2: *stop=0x8c10d5b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10d5b4; /* mov.l r9,@-r15 */
L8c10d5b4: *stop=0x8c10d5b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10d5b6; /* mov.l r8,@-r15 */
L8c10d5b6: *stop=0x8c10d5b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d5b8; /* sts.l PR,@-r15 */
L8c10d5b8: *stop=0x8c10d5b8u;r[15]+=4294967292u;goto L8c10d5ba; /* add #-0x4,r15 */
L8c10d5ba: *stop=0x8c10d5bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d748u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d5bc; /* mov.l 0x8c10d748,r2 */
L8c10d5bc: *stop=0x8c10d5bcu;r[3]=4294967264u;goto L8c10d5be; /* mov #-0x20,r3 */
L8c10d5be: *stop=0x8c10d5beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d744u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c10d5c0; /* mov.l 0x8c10d744,r14 */
L8c10d5c0: *stop=0x8c10d5c0u;r[5]=0u;goto L8c10d5c2; /* mov #0x0,r5 */
L8c10d5c2: *stop=0x8c10d5c2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d72au,2);if(io_status)return io_status;r[6]=loaded;}goto L8c10d5c4; /* mov.w 0x8c10d72a,r6 */
L8c10d5c4: *stop=0x8c10d5c4u;r[2]&=r[3];goto L8c10d5c6; /* and r3,r2 */
L8c10d5c6: *stop=0x8c10d5c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d74cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d5c8; /* mov.l 0x8c10d74c,r1 */
L8c10d5c8: *stop=0x8c10d5c8u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c10d5ca; /* mov.l r2,@r14 */
L8c10d5ca: target=r[1];*pr=0x8c10d5ceu;*stop=0x8c10d5ccu;r[4]=r[2];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d5ce; /* jsr @r1 */
L8c10d5ce: *stop=0x8c10d5ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d750u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d5d0; /* mov.l 0x8c10d750,r2 */
L8c10d5d0: *stop=0x8c10d5d0u;r[13]=0u;goto L8c10d5d2; /* mov #0x0,r13 */
L8c10d5d2: *stop=0x8c10d5d2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d5d4; /* mov.l @r14,r3 */
L8c10d5d4: *stop=0x8c10d5d4u;r[10]=3u;goto L8c10d5d6; /* mov #0x3,r10 */
L8c10d5d6: *stop=0x8c10d5d6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d72cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10d5d8; /* mov.w 0x8c10d72c,r0 */
L8c10d5d8: *stop=0x8c10d5d8u;r[11]=r[13];goto L8c10d5da; /* mov r13,r11 */
L8c10d5da: *stop=0x8c10d5dau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c10d5dc; /* mov.l r2,@r3 */
L8c10d5dc: *stop=0x8c10d5dcu;r[2]=1u;goto L8c10d5de; /* mov #0x1,r2 */
L8c10d5de: *stop=0x8c10d5deu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d5e0; /* mov.l @r14,r3 */
L8c10d5e0: *stop=0x8c10d5e0u;r[9]=4u;goto L8c10d5e2; /* mov #0x4,r9 */
L8c10d5e2: *stop=0x8c10d5e2u;r[8]=52u;goto L8c10d5e4; /* mov #0x34,r8 */
L8c10d5e4: *stop=0x8c10d5e4u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c10d5e6; /* mov.l r2,@(r0,r3) */
L8c10d5e6: *stop=0x8c10d5e6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d5e8; /* mov.l @r14,r3 */
L8c10d5e8: *stop=0x8c10d5e8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d72eu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10d5ea; /* mov.w 0x8c10d72e,r0 */
L8c10d5ea: *stop=0x8c10d5eau;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c10d5ec; /* mov.l r13,@(r0,r3) */
L8c10d5ec: *stop=0x8c10d5ecu;*macl=r[8]*r[11];goto L8c10d5ee; /* mul.l r8,r11 */
L8c10d5ee: *stop=0x8c10d5eeu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d5f0; /* mov.l @r14,r2 */
L8c10d5f0: *stop=0x8c10d5f0u;r[2]+=4u;goto L8c10d5f2; /* add #0x4,r2 */
L8c10d5f2: *stop=0x8c10d5f2u;r[12]=*macl;goto L8c10d5f4; /* sts MACL,r12 */
L8c10d5f4: *stop=0x8c10d5f4u;r[2]+=r[12];goto L8c10d5f6; /* add r12,r2 */
L8c10d5f6: *stop=0x8c10d5f6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[11],4);if(io_status)return io_status;goto L8c10d5f8; /* mov.l r11,@r2 */
L8c10d5f8: *stop=0x8c10d5f8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d5fa; /* mov.l @r14,r2 */
L8c10d5fa: *stop=0x8c10d5fau;r[2]+=4u;goto L8c10d5fc; /* add #0x4,r2 */
L8c10d5fc: *stop=0x8c10d5fcu;r[2]+=r[12];goto L8c10d5fe; /* add r12,r2 */
L8c10d5fe: *stop=0x8c10d5feu;if(!valid_access(r[2]+40u,4u))return -3;wr(m,bus,&io_status,r[2]+0x28u,r[13],4);if(io_status)return io_status;goto L8c10d600; /* mov.l r13,@(0x28,r2) */
L8c10d600: *stop=0x8c10d600u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d602; /* mov.l @r14,r2 */
L8c10d602: *stop=0x8c10d602u;r[2]+=4u;goto L8c10d604; /* add #0x4,r2 */
L8c10d604: *stop=0x8c10d604u;r[2]+=r[12];goto L8c10d606; /* add r12,r2 */
L8c10d606: *stop=0x8c10d606u;if(!valid_access(r[2]+44u,4u))return -3;wr(m,bus,&io_status,r[2]+0x2cu,r[13],4);if(io_status)return io_status;goto L8c10d608; /* mov.l r13,@(0x2c,r2) */
L8c10d608: *stop=0x8c10d608u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d60a; /* mov.l @r14,r2 */
L8c10d60a: *stop=0x8c10d60au;r[2]+=4u;goto L8c10d60c; /* add #0x4,r2 */
L8c10d60c: *stop=0x8c10d60cu;r[2]+=r[12];goto L8c10d60e; /* add r12,r2 */
L8c10d60e: *stop=0x8c10d60eu;if(!valid_access(r[2]+48u,4u))return -3;wr(m,bus,&io_status,r[2]+0x30u,r[13],4);if(io_status)return io_status;goto L8c10d610; /* mov.l r13,@(0x30,r2) */
L8c10d610: *stop=0x8c10d610u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d612; /* mov.l @r14,r2 */
L8c10d612: *stop=0x8c10d612u;r[2]+=4u;goto L8c10d614; /* add #0x4,r2 */
L8c10d614: *stop=0x8c10d614u;r[2]+=r[12];goto L8c10d616; /* add r12,r2 */
L8c10d616: *stop=0x8c10d616u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[13],4);if(io_status)return io_status;goto L8c10d618; /* mov.l r13,@(0x4,r2) */
L8c10d618: *stop=0x8c10d618u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d61a; /* mov.l @r14,r2 */
L8c10d61a: *stop=0x8c10d61au;r[2]+=4u;goto L8c10d61c; /* add #0x4,r2 */
L8c10d61c: *stop=0x8c10d61cu;r[2]+=r[12];goto L8c10d61e; /* add r12,r2 */
L8c10d61e: *stop=0x8c10d61eu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[13],4);if(io_status)return io_status;goto L8c10d620; /* mov.l r13,@(0x8,r2) */
L8c10d620: *stop=0x8c10d620u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d622; /* mov.l @r14,r2 */
L8c10d622: *stop=0x8c10d622u;r[2]+=4u;goto L8c10d624; /* add #0x4,r2 */
L8c10d624: *stop=0x8c10d624u;r[2]+=r[12];goto L8c10d626; /* add r12,r2 */
L8c10d626: *stop=0x8c10d626u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[13],4);if(io_status)return io_status;goto L8c10d628; /* mov.l r13,@(0xc,r2) */
L8c10d628: *stop=0x8c10d628u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d62a; /* mov.l @r14,r2 */
L8c10d62a: *stop=0x8c10d62au;r[2]+=4u;goto L8c10d62c; /* add #0x4,r2 */
L8c10d62c: *stop=0x8c10d62cu;r[2]+=r[12];goto L8c10d62e; /* add r12,r2 */
L8c10d62e: *stop=0x8c10d62eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d754u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d630; /* mov.l 0x8c10d754,r3 */
L8c10d630: *stop=0x8c10d630u;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[13],4);if(io_status)return io_status;goto L8c10d632; /* mov.l r13,@(0x10,r2) */
L8c10d632: *stop=0x8c10d632u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d634; /* mov.l @r14,r2 */
L8c10d634: *stop=0x8c10d634u;r[2]+=4u;goto L8c10d636; /* add #0x4,r2 */
L8c10d636: *stop=0x8c10d636u;r[2]+=r[12];goto L8c10d638; /* add r12,r2 */
L8c10d638: *stop=0x8c10d638u;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[13],4);if(io_status)return io_status;goto L8c10d63a; /* mov.l r13,@(0x14,r2) */
L8c10d63a: *stop=0x8c10d63au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d63c; /* mov.l @r14,r2 */
L8c10d63c: *stop=0x8c10d63cu;r[2]+=4u;goto L8c10d63e; /* add #0x4,r2 */
L8c10d63e: *stop=0x8c10d63eu;r[2]+=r[12];goto L8c10d640; /* add r12,r2 */
L8c10d640: *stop=0x8c10d640u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[10],4);if(io_status)return io_status;goto L8c10d642; /* mov.l r10,@(0x18,r2) */
L8c10d642: *stop=0x8c10d642u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d644; /* mov.l @r14,r2 */
L8c10d644: *stop=0x8c10d644u;r[2]+=4u;goto L8c10d646; /* add #0x4,r2 */
L8c10d646: *stop=0x8c10d646u;r[2]+=r[12];goto L8c10d648; /* add r12,r2 */
L8c10d648: *stop=0x8c10d648u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[13],4);if(io_status)return io_status;goto L8c10d64a; /* mov.l r13,@(0x1c,r2) */
L8c10d64a: *stop=0x8c10d64au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d64c; /* mov.l @r14,r2 */
L8c10d64c: *stop=0x8c10d64cu;r[2]+=4u;goto L8c10d64e; /* add #0x4,r2 */
L8c10d64e: *stop=0x8c10d64eu;r[2]+=r[12];goto L8c10d650; /* add r12,r2 */
L8c10d650: *stop=0x8c10d650u;if(!valid_access(r[2]+32u,4u))return -3;wr(m,bus,&io_status,r[2]+0x20u,r[13],4);if(io_status)return io_status;goto L8c10d652; /* mov.l r13,@(0x20,r2) */
L8c10d652: *stop=0x8c10d652u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10d654; /* mov.l @r14,r4 */
L8c10d654: *stop=0x8c10d654u;r[4]+=4u;goto L8c10d656; /* add #0x4,r4 */
L8c10d656: target=r[3];*pr=0x8c10d65au;*stop=0x8c10d658u;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d65a; /* jsr @r3 */
L8c10d65a: *stop=0x8c10d65au;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d730u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d65c; /* mov.w 0x8c10d730,r3 */
L8c10d65c: *stop=0x8c10d65cu;r[4]=r[11];goto L8c10d65e; /* mov r11,r4 */
L8c10d65e: *stop=0x8c10d65eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d660; /* mov.l @r14,r0 */
L8c10d660: *stop=0x8c10d660u;r[4]<<=2;goto L8c10d662; /* shll2 r4 */
L8c10d662: *stop=0x8c10d662u;r[11]+=1u;goto L8c10d664; /* add #0x1,r11 */
L8c10d664: *stop=0x8c10d664u;r[3]+=r[0];goto L8c10d666; /* add r0,r3 */
L8c10d666: *stop=0x8c10d666u;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[9]);goto L8c10d668; /* cmp/ge r9,r11 */
L8c10d668: *stop=0x8c10d668u;r[3]+=r[4];goto L8c10d66a; /* add r4,r3 */
L8c10d66a: *stop=0x8c10d66au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c10d66c; /* mov.l r13,@r3 */
L8c10d66c: *stop=0x8c10d66cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d732u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d66e; /* mov.w 0x8c10d732,r3 */
L8c10d66e: *stop=0x8c10d66eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d670; /* mov.l @r14,r0 */
L8c10d670: *stop=0x8c10d670u;r[3]+=r[0];goto L8c10d672; /* add r0,r3 */
L8c10d672: *stop=0x8c10d672u;r[3]+=r[4];goto L8c10d674; /* add r4,r3 */
L8c10d674: *stop=0x8c10d674u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c10d676; /* mov.l r13,@r3 */
L8c10d676: *stop=0x8c10d676u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d734u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d678; /* mov.w 0x8c10d734,r3 */
L8c10d678: *stop=0x8c10d678u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d67a; /* mov.l @r14,r0 */
L8c10d67a: *stop=0x8c10d67au;r[3]+=r[0];goto L8c10d67c; /* add r0,r3 */
L8c10d67c: *stop=0x8c10d67cu;r[4]+=r[3];goto L8c10d67e; /* add r3,r4 */
L8c10d67e: take=!(*sr&1u);*stop=0x8c10d680u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[13],4);if(io_status)return io_status;if(take)goto L8c10d5ec;goto L8c10d682; /* bf/s 0x8c10d5ec */
L8c10d682: *stop=0x8c10d682u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d736u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10d684; /* mov.w 0x8c10d736,r0 */
L8c10d684: *stop=0x8c10d684u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d686; /* mov.l @r14,r3 */
L8c10d686: *stop=0x8c10d686u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d75cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10d688; /* mov.l 0x8c10d75c,r4 */
L8c10d688: *stop=0x8c10d688u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c10d68a; /* mov.l r13,@(r0,r3) */
L8c10d68a: *stop=0x8c10d68au;r[0]+=4u;goto L8c10d68c; /* add #0x4,r0 */
L8c10d68c: *stop=0x8c10d68cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d68e; /* mov.l @r14,r3 */
L8c10d68e: *stop=0x8c10d68eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d758u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10d690; /* mov.l 0x8c10d758,r5 */
L8c10d690: *stop=0x8c10d690u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c10d692; /* mov.l r13,@(r0,r3) */
L8c10d692: *stop=0x8c10d692u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d760u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d694; /* mov.l 0x8c10d760,r3 */
L8c10d694: target=r[3];*pr=0x8c10d698u;*stop=0x8c10d696u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d698; /* jsr @r3 */
L8c10d698: *stop=0x8c10d698u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d72cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10d69a; /* mov.w 0x8c10d72c,r0 */
L8c10d69a: *stop=0x8c10d69au;r[3]=1u;goto L8c10d69c; /* mov #0x1,r3 */
L8c10d69c: *stop=0x8c10d69cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d69e; /* mov.l @r14,r2 */
L8c10d69e: *stop=0x8c10d69eu;r[6]=96u;goto L8c10d6a0; /* mov #0x60,r6 */
L8c10d6a0: *stop=0x8c10d6a0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d738u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c10d6a2; /* mov.w 0x8c10d738,r4 */
L8c10d6a2: *stop=0x8c10d6a2u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[3],4);if(io_status)return io_status;goto L8c10d6a4; /* mov.l r3,@(r0,r2) */
L8c10d6a4: *stop=0x8c10d6a4u;r[3]=36u;goto L8c10d6a6; /* mov #0x24,r3 */
L8c10d6a6: *stop=0x8c10d6a6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d6a8; /* mov.l @r14,r2 */
L8c10d6a8: *stop=0x8c10d6a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d768u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c10d6aa; /* mov.l 0x8c10d768,r12 */
L8c10d6aa: *stop=0x8c10d6aau;r[2]+=4u;goto L8c10d6ac; /* add #0x4,r2 */
L8c10d6ac: *stop=0x8c10d6acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d764u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c10d6ae; /* mov.l 0x8c10d764,r11 */
L8c10d6ae: *stop=0x8c10d6aeu;r[3]+=r[2];goto L8c10d6b0; /* add r2,r3 */
L8c10d6b0: *stop=0x8c10d6b0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10d6b2; /* mov.l r2,@r15 */
L8c10d6b2: *stop=0x8c10d6b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10d6b4; /* mov.l r3,@-r15 */
L8c10d6b4: *stop=0x8c10d6b4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d6b6; /* mov.l @(0x4,r15),r7 */
L8c10d6b6: target=r[12];*pr=0x8c10d6bau;*stop=0x8c10d6b8u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d6ba; /* jsr @r12 */
L8c10d6ba: *stop=0x8c10d6bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c10d6bc; /* mov.l @r15+,r1 */
L8c10d6bc: *stop=0x8c10d6bcu;r[3]=36u;goto L8c10d6be; /* mov #0x24,r3 */
L8c10d6be: *stop=0x8c10d6beu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d73au,2);if(io_status)return io_status;r[4]=loaded;}goto L8c10d6c0; /* mov.w 0x8c10d73a,r4 */
L8c10d6c0: *stop=0x8c10d6c0u;r[6]=96u;goto L8c10d6c2; /* mov #0x60,r6 */
L8c10d6c2: *stop=0x8c10d6c2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c10d6c4; /* mov.l r0,@r1 */
L8c10d6c4: *stop=0x8c10d6c4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d6c6; /* mov.l @r14,r2 */
L8c10d6c6: *stop=0x8c10d6c6u;r[2]+=56u;goto L8c10d6c8; /* add #0x38,r2 */
L8c10d6c8: *stop=0x8c10d6c8u;r[3]+=r[2];goto L8c10d6ca; /* add r2,r3 */
L8c10d6ca: *stop=0x8c10d6cau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10d6cc; /* mov.l r2,@r15 */
L8c10d6cc: *stop=0x8c10d6ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10d6ce; /* mov.l r3,@-r15 */
L8c10d6ce: *stop=0x8c10d6ceu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d6d0; /* mov.l @(0x4,r15),r7 */
L8c10d6d0: target=r[12];*pr=0x8c10d6d4u;*stop=0x8c10d6d2u;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d6d4; /* jsr @r12 */
L8c10d6d4: *stop=0x8c10d6d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c10d6d6; /* mov.l @r15+,r1 */
L8c10d6d6: *stop=0x8c10d6d6u;r[3]=36u;goto L8c10d6d8; /* mov #0x24,r3 */
L8c10d6d8: *stop=0x8c10d6d8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d73cu,2);if(io_status)return io_status;r[4]=loaded;}goto L8c10d6da; /* mov.w 0x8c10d73c,r4 */
L8c10d6da: *stop=0x8c10d6dau;r[6]=96u;goto L8c10d6dc; /* mov #0x60,r6 */
L8c10d6dc: *stop=0x8c10d6dcu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c10d6de; /* mov.l r0,@r1 */
L8c10d6de: *stop=0x8c10d6deu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d6e0; /* mov.l @r14,r2 */
L8c10d6e0: *stop=0x8c10d6e0u;r[2]+=108u;goto L8c10d6e2; /* add #0x6c,r2 */
L8c10d6e2: *stop=0x8c10d6e2u;r[3]+=r[2];goto L8c10d6e4; /* add r2,r3 */
L8c10d6e4: *stop=0x8c10d6e4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10d6e6; /* mov.l r2,@r15 */
L8c10d6e6: *stop=0x8c10d6e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10d6e8; /* mov.l r3,@-r15 */
L8c10d6e8: *stop=0x8c10d6e8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d6ea; /* mov.l @(0x4,r15),r7 */
L8c10d6ea: target=r[12];*pr=0x8c10d6eeu;*stop=0x8c10d6ecu;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d6ee; /* jsr @r12 */
L8c10d6ee: *stop=0x8c10d6eeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c10d6f0; /* mov.l @r15+,r1 */
L8c10d6f0: *stop=0x8c10d6f0u;r[3]=36u;goto L8c10d6f2; /* mov #0x24,r3 */
L8c10d6f2: *stop=0x8c10d6f2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d73eu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c10d6f4; /* mov.w 0x8c10d73e,r2 */
L8c10d6f4: *stop=0x8c10d6f4u;r[6]=96u;goto L8c10d6f6; /* mov #0x60,r6 */
L8c10d6f6: *stop=0x8c10d6f6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c10d6f8; /* mov.l r0,@r1 */
L8c10d6f8: *stop=0x8c10d6f8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d6fa; /* mov.l @r14,r1 */
L8c10d6fa: *stop=0x8c10d6fau;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d740u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c10d6fc; /* mov.w 0x8c10d740,r4 */
L8c10d6fc: *stop=0x8c10d6fcu;r[1]+=4u;goto L8c10d6fe; /* add #0x4,r1 */
L8c10d6fe: *stop=0x8c10d6feu;r[1]+=r[2];goto L8c10d700; /* add r2,r1 */
L8c10d700: *stop=0x8c10d700u;r[3]+=r[1];goto L8c10d702; /* add r1,r3 */
L8c10d702: *stop=0x8c10d702u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c10d704; /* mov.l r1,@r15 */
L8c10d704: *stop=0x8c10d704u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10d706; /* mov.l r3,@-r15 */
L8c10d706: *stop=0x8c10d706u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d708; /* mov.l @(0x4,r15),r7 */
L8c10d708: target=r[12];*pr=0x8c10d70cu;*stop=0x8c10d70au;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d70c; /* jsr @r12 */
L8c10d70c: *stop=0x8c10d70cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;goto L8c10d70e; /* mov.l @r15+,r2 */
L8c10d70e: *stop=0x8c10d70eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d76cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d710; /* mov.l 0x8c10d76c,r3 */
L8c10d710: *stop=0x8c10d710u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c10d712; /* mov.l r0,@r2 */
L8c10d712: *stop=0x8c10d712u;r[0]=0u;goto L8c10d714; /* mov #0x0,r0 */
L8c10d714: *stop=0x8c10d714u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c10d716; /* mov.l r13,@r3 */
L8c10d716: *stop=0x8c10d716u;r[15]+=4u;goto L8c10d718; /* add #0x4,r15 */
L8c10d718: *stop=0x8c10d718u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d71a; /* lds.l @r15+,PR */
L8c10d71a: *stop=0x8c10d71au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c10d71c; /* mov.l @r15+,r8 */
L8c10d71c: *stop=0x8c10d71cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c10d71e; /* mov.l @r15+,r9 */
L8c10d71e: *stop=0x8c10d71eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10d720; /* mov.l @r15+,r10 */
L8c10d720: *stop=0x8c10d720u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10d722; /* mov.l @r15+,r11 */
L8c10d722: *stop=0x8c10d722u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10d724; /* mov.l @r15+,r12 */
L8c10d724: *stop=0x8c10d724u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10d726; /* mov.l @r15+,r13 */
L8c10d726: *stop=0x8c10d726u;if(*pr!=entry_pr)return -3;*stop=0x8c10d728u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_context_8c10ed54(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ed54;
L8c10ed54: *stop=0x8c10ed54u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[7]=loaded;}goto L8c10ed56; /* mov.l @r4,r7 */
L8c10ed56: *stop=0x8c10ed56u;r[6]=52u;goto L8c10ed58; /* mov #0x34,r6 */
L8c10ed58: *stop=0x8c10ed58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ee1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10ed5a; /* mov.l 0x8c10ee1c,r5 */
L8c10ed5a: *stop=0x8c10ed5au;*macl=r[6]*r[7];goto L8c10ed5c; /* mul.l r6,r7 */
L8c10ed5c: *stop=0x8c10ed5cu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ed5e; /* mov.l @r5,r3 */
L8c10ed5e: *stop=0x8c10ed5eu;r[3]+=4u;goto L8c10ed60; /* add #0x4,r3 */
L8c10ed60: *stop=0x8c10ed60u;r[2]=*macl;goto L8c10ed62; /* sts MACL,r2 */
L8c10ed62: *stop=0x8c10ed62u;r[2]+=r[3];goto L8c10ed64; /* add r3,r2 */
L8c10ed64: *stop=0x8c10ed64u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[7],4);if(io_status)return io_status;goto L8c10ed66; /* mov.l r7,@r2 */
L8c10ed66: *stop=0x8c10ed66u;r[7]=0u;goto L8c10ed68; /* mov #0x0,r7 */
L8c10ed68: *stop=0x8c10ed68u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ed6a; /* mov.l @r4,r3 */
L8c10ed6a: *stop=0x8c10ed6au;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ed6c; /* mov.l @r5,r2 */
L8c10ed6c: *stop=0x8c10ed6cu;*macl=r[6]*r[3];goto L8c10ed6e; /* mul.l r6,r3 */
L8c10ed6e: *stop=0x8c10ed6eu;r[2]+=4u;goto L8c10ed70; /* add #0x4,r2 */
L8c10ed70: *stop=0x8c10ed70u;r[3]=*macl;goto L8c10ed72; /* sts MACL,r3 */
L8c10ed72: *stop=0x8c10ed72u;r[3]+=r[2];goto L8c10ed74; /* add r2,r3 */
L8c10ed74: *stop=0x8c10ed74u;if(!valid_access(r[3]+40u,4u))return -3;wr(m,bus,&io_status,r[3]+0x28u,r[7],4);if(io_status)return io_status;goto L8c10ed76; /* mov.l r7,@(0x28,r3) */
L8c10ed76: *stop=0x8c10ed76u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ed78; /* mov.l @r4,r2 */
L8c10ed78: *stop=0x8c10ed78u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ed7a; /* mov.l @r5,r3 */
L8c10ed7a: *stop=0x8c10ed7au;*macl=r[6]*r[2];goto L8c10ed7c; /* mul.l r6,r2 */
L8c10ed7c: *stop=0x8c10ed7cu;r[3]+=4u;goto L8c10ed7e; /* add #0x4,r3 */
L8c10ed7e: *stop=0x8c10ed7eu;r[2]=*macl;goto L8c10ed80; /* sts MACL,r2 */
L8c10ed80: *stop=0x8c10ed80u;r[2]+=r[3];goto L8c10ed82; /* add r3,r2 */
L8c10ed82: *stop=0x8c10ed82u;if(!valid_access(r[2]+44u,4u))return -3;wr(m,bus,&io_status,r[2]+0x2cu,r[7],4);if(io_status)return io_status;goto L8c10ed84; /* mov.l r7,@(0x2c,r2) */
L8c10ed84: *stop=0x8c10ed84u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ed86; /* mov.l @r4,r3 */
L8c10ed86: *stop=0x8c10ed86u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ed88; /* mov.l @r5,r2 */
L8c10ed88: *stop=0x8c10ed88u;*macl=r[6]*r[3];goto L8c10ed8a; /* mul.l r6,r3 */
L8c10ed8a: *stop=0x8c10ed8au;r[2]+=4u;goto L8c10ed8c; /* add #0x4,r2 */
L8c10ed8c: *stop=0x8c10ed8cu;r[3]=*macl;goto L8c10ed8e; /* sts MACL,r3 */
L8c10ed8e: *stop=0x8c10ed8eu;r[3]+=r[2];goto L8c10ed90; /* add r2,r3 */
L8c10ed90: *stop=0x8c10ed90u;if(!valid_access(r[3]+48u,4u))return -3;wr(m,bus,&io_status,r[3]+0x30u,r[7],4);if(io_status)return io_status;goto L8c10ed92; /* mov.l r7,@(0x30,r3) */
L8c10ed92: *stop=0x8c10ed92u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ed94; /* mov.l @r5,r3 */
L8c10ed94: *stop=0x8c10ed94u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ed96; /* mov.l @r4,r2 */
L8c10ed96: *stop=0x8c10ed96u;r[3]+=4u;goto L8c10ed98; /* add #0x4,r3 */
L8c10ed98: *stop=0x8c10ed98u;*macl=r[6]*r[2];goto L8c10ed9a; /* mul.l r6,r2 */
L8c10ed9a: *stop=0x8c10ed9au;r[2]=*macl;goto L8c10ed9c; /* sts MACL,r2 */
L8c10ed9c: *stop=0x8c10ed9cu;r[2]+=r[3];goto L8c10ed9e; /* add r3,r2 */
L8c10ed9e: *stop=0x8c10ed9eu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[7],4);if(io_status)return io_status;goto L8c10eda0; /* mov.l r7,@(0x4,r2) */
L8c10eda0: *stop=0x8c10eda0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10eda2; /* mov.l @r4,r3 */
L8c10eda2: *stop=0x8c10eda2u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10eda4; /* mov.l @r5,r2 */
L8c10eda4: *stop=0x8c10eda4u;*macl=r[6]*r[3];goto L8c10eda6; /* mul.l r6,r3 */
L8c10eda6: *stop=0x8c10eda6u;r[2]+=4u;goto L8c10eda8; /* add #0x4,r2 */
L8c10eda8: *stop=0x8c10eda8u;r[3]=*macl;goto L8c10edaa; /* sts MACL,r3 */
L8c10edaa: *stop=0x8c10edaau;r[3]+=r[2];goto L8c10edac; /* add r2,r3 */
L8c10edac: *stop=0x8c10edacu;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[7],4);if(io_status)return io_status;goto L8c10edae; /* mov.l r7,@(0x8,r3) */
L8c10edae: *stop=0x8c10edaeu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10edb0; /* mov.l @r4,r2 */
L8c10edb0: *stop=0x8c10edb0u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10edb2; /* mov.l @r5,r3 */
L8c10edb2: *stop=0x8c10edb2u;*macl=r[6]*r[2];goto L8c10edb4; /* mul.l r6,r2 */
L8c10edb4: *stop=0x8c10edb4u;r[3]+=4u;goto L8c10edb6; /* add #0x4,r3 */
L8c10edb6: *stop=0x8c10edb6u;r[2]=*macl;goto L8c10edb8; /* sts MACL,r2 */
L8c10edb8: *stop=0x8c10edb8u;r[2]+=r[3];goto L8c10edba; /* add r3,r2 */
L8c10edba: *stop=0x8c10edbau;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[7],4);if(io_status)return io_status;goto L8c10edbc; /* mov.l r7,@(0xc,r2) */
L8c10edbc: *stop=0x8c10edbcu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10edbe; /* mov.l @r4,r3 */
L8c10edbe: *stop=0x8c10edbeu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10edc0; /* mov.l @r5,r2 */
L8c10edc0: *stop=0x8c10edc0u;*macl=r[6]*r[3];goto L8c10edc2; /* mul.l r6,r3 */
L8c10edc2: *stop=0x8c10edc2u;r[2]+=4u;goto L8c10edc4; /* add #0x4,r2 */
L8c10edc4: *stop=0x8c10edc4u;r[3]=*macl;goto L8c10edc6; /* sts MACL,r3 */
L8c10edc6: *stop=0x8c10edc6u;r[3]+=r[2];goto L8c10edc8; /* add r2,r3 */
L8c10edc8: *stop=0x8c10edc8u;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[7],4);if(io_status)return io_status;goto L8c10edca; /* mov.l r7,@(0x10,r3) */
L8c10edca: *stop=0x8c10edcau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10edcc; /* mov.l @r4,r2 */
L8c10edcc: *stop=0x8c10edccu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10edce; /* mov.l @r5,r3 */
L8c10edce: *stop=0x8c10edceu;*macl=r[6]*r[2];goto L8c10edd0; /* mul.l r6,r2 */
L8c10edd0: *stop=0x8c10edd0u;r[3]+=4u;goto L8c10edd2; /* add #0x4,r3 */
L8c10edd2: *stop=0x8c10edd2u;r[2]=*macl;goto L8c10edd4; /* sts MACL,r2 */
L8c10edd4: *stop=0x8c10edd4u;r[2]+=r[3];goto L8c10edd6; /* add r3,r2 */
L8c10edd6: *stop=0x8c10edd6u;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[7],4);if(io_status)return io_status;goto L8c10edd8; /* mov.l r7,@(0x14,r2) */
L8c10edd8: *stop=0x8c10edd8u;r[1]=3u;goto L8c10edda; /* mov #0x3,r1 */
L8c10edda: *stop=0x8c10eddau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10eddc; /* mov.l @r4,r3 */
L8c10eddc: *stop=0x8c10eddcu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10edde; /* mov.l @r5,r2 */
L8c10edde: *stop=0x8c10eddeu;*macl=r[6]*r[3];goto L8c10ede0; /* mul.l r6,r3 */
L8c10ede0: *stop=0x8c10ede0u;r[2]+=4u;goto L8c10ede2; /* add #0x4,r2 */
L8c10ede2: *stop=0x8c10ede2u;r[3]=*macl;goto L8c10ede4; /* sts MACL,r3 */
L8c10ede4: *stop=0x8c10ede4u;r[3]+=r[2];goto L8c10ede6; /* add r2,r3 */
L8c10ede6: *stop=0x8c10ede6u;if(!valid_access(r[3]+24u,4u))return -3;wr(m,bus,&io_status,r[3]+0x18u,r[1],4);if(io_status)return io_status;goto L8c10ede8; /* mov.l r1,@(0x18,r3) */
L8c10ede8: *stop=0x8c10ede8u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10edea; /* mov.l @r4,r2 */
L8c10edea: *stop=0x8c10edeau;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10edec; /* mov.l @r5,r3 */
L8c10edec: *stop=0x8c10edecu;*macl=r[6]*r[2];goto L8c10edee; /* mul.l r6,r2 */
L8c10edee: *stop=0x8c10edeeu;r[3]+=4u;goto L8c10edf0; /* add #0x4,r3 */
L8c10edf0: *stop=0x8c10edf0u;r[2]=*macl;goto L8c10edf2; /* sts MACL,r2 */
L8c10edf2: *stop=0x8c10edf2u;r[2]+=r[3];goto L8c10edf4; /* add r3,r2 */
L8c10edf4: *stop=0x8c10edf4u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[7],4);if(io_status)return io_status;goto L8c10edf6; /* mov.l r7,@(0x1c,r2) */
L8c10edf6: *stop=0x8c10edf6u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10edf8; /* mov.l @r4,r3 */
L8c10edf8: *stop=0x8c10edf8u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10edfa; /* mov.l @r5,r2 */
L8c10edfa: *stop=0x8c10edfau;*macl=r[6]*r[3];goto L8c10edfc; /* mul.l r6,r3 */
L8c10edfc: *stop=0x8c10edfcu;r[2]+=4u;goto L8c10edfe; /* add #0x4,r2 */
L8c10edfe: *stop=0x8c10edfeu;r[6]=*macl;goto L8c10ee00; /* sts MACL,r6 */
L8c10ee00: *stop=0x8c10ee00u;r[6]+=r[2];goto L8c10ee02; /* add r2,r6 */
L8c10ee02: *stop=0x8c10ee02u;if(!valid_access(r[6]+32u,4u))return -3;wr(m,bus,&io_status,r[6]+0x20u,r[7],4);if(io_status)return io_status;goto L8c10ee04; /* mov.l r7,@(0x20,r6) */
L8c10ee04: *stop=0x8c10ee04u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ee06; /* mov.l @r4,r1 */
L8c10ee06: *stop=0x8c10ee06u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ee08; /* mov.l @r5,r3 */
L8c10ee08: *stop=0x8c10ee08u;r[1]<<=2;goto L8c10ee0a; /* shll2 r1 */
L8c10ee0a: *stop=0x8c10ee0au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ee0c; /* mov.l @r3,r3 */
L8c10ee0c: *stop=0x8c10ee0cu;r[1]<<=2;goto L8c10ee0e; /* shll2 r1 */
L8c10ee0e: *stop=0x8c10ee0eu;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c10ee10; /* shll r1 */
L8c10ee10: *stop=0x8c10ee10u;r[1]+=r[3];goto L8c10ee12; /* add r3,r1 */
L8c10ee12: *stop=0x8c10ee12u;r[4]=r[1];goto L8c10ee14; /* mov r1,r4 */
L8c10ee14: *stop=0x8c10ee14u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[7],4);if(io_status)return io_status;goto L8c10ee16; /* mov.l r7,@(0x14,r4) */
L8c10ee16: *stop=0x8c10ee16u;if(*pr!=entry_pr)return -3;*stop=0x8c10ee18u;r[0]=r[7];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_context_8c10ee64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ee64;
L8c10ee64: *stop=0x8c10ee64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ee8cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c10ee66; /* mov.l 0x8c10ee8c,r6 */
L8c10ee66: *stop=0x8c10ee66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ee90u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ee68; /* mov.l 0x8c10ee90,r3 */
L8c10ee68: *stop=0x8c10ee68u;r[4]&=r[6];goto L8c10ee6a; /* and r6,r4 */
L8c10ee6a: *stop=0x8c10ee6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ee94u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ee6c; /* mov.l 0x8c10ee94,r2 */
L8c10ee6c: *stop=0x8c10ee6cu;r[4]>>=16;goto L8c10ee6e; /* shlr16 r4 */
L8c10ee6e: *stop=0x8c10ee6eu;r[5]&=r[6];goto L8c10ee70; /* and r6,r5 */
L8c10ee70: *stop=0x8c10ee70u;r[4]>>=2;goto L8c10ee72; /* shlr2 r4 */
L8c10ee72: *stop=0x8c10ee72u;r[5]>>=16;goto L8c10ee74; /* shlr16 r5 */
L8c10ee74: *stop=0x8c10ee74u;r[4]>>=2;goto L8c10ee76; /* shlr2 r4 */
L8c10ee76: *stop=0x8c10ee76u;r[4]<<=8;goto L8c10ee78; /* shll8 r4 */
L8c10ee78: *stop=0x8c10ee78u;r[5]>>=2;goto L8c10ee7a; /* shlr2 r5 */
L8c10ee7a: *stop=0x8c10ee7au;r[6]=r[4];goto L8c10ee7c; /* mov r4,r6 */
L8c10ee7c: *stop=0x8c10ee7cu;r[5]>>=2;goto L8c10ee7e; /* shlr2 r5 */
L8c10ee7e: *stop=0x8c10ee7eu;r[6]|=r[5];goto L8c10ee80; /* or r5,r6 */
L8c10ee80: *stop=0x8c10ee80u;r[6]|=r[3];goto L8c10ee82; /* or r3,r6 */
L8c10ee82: *stop=0x8c10ee82u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[6],4);if(io_status)return io_status;goto L8c10ee84; /* mov.l r6,@r2 */
L8c10ee84: *stop=0x8c10ee84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ee98u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ee86; /* mov.l 0x8c10ee98,r1 */
L8c10ee86: *stop=0x8c10ee86u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[6],4);if(io_status)return io_status;goto L8c10ee88; /* mov.l r6,@r1 */
L8c10ee88: *stop=0x8c10ee88u;if(*pr!=entry_pr)return -3;*stop=0x8c10ee8au;r[0]=0u;return 0; /* rts */
}
