/* Geometry projection, sampling and view proposals; explicit GBR/queue state.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
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
 if(a==0xffd8000cu && n==4u)return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_boot_device_8c642440(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c642440;
L8c642440: *stop=0x8c642440u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c642442; /* mov.l r14,@-r15 */
L8c642442: *stop=0x8c642442u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c642444; /* mov.l r13,@-r15 */
L8c642444: *stop=0x8c642444u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c642446; /* mov.l r12,@-r15 */
L8c642446: *stop=0x8c642446u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c642448; /* mov.l r11,@-r15 */
L8c642448: *stop=0x8c642448u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64244a; /* mov.l r10,@-r15 */
L8c64244a: *stop=0x8c64244au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64244c; /* mov.l r9,@-r15 */
L8c64244c: *stop=0x8c64244cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64244e; /* mov.l r8,@-r15 */
L8c64244e: *stop=0x8c64244eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c642450; /* sts.l PR,@-r15 */
L8c642450: *stop=0x8c642450u;r[15]+=4294967208u;goto L8c642452; /* add #-0x58,r15 */
L8c642452: *stop=0x8c642452u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642660u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642454; /* mov.l 0x8c642660,r3 */
L8c642454: *stop=0x8c642454u;r[2]=r[15];goto L8c642456; /* mov r15,r2 */
L8c642456: *stop=0x8c642456u;r[2]+=16u;goto L8c642458; /* add #0x10,r2 */
L8c642458: *stop=0x8c642458u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64245a; /* mov.l @r3,r1 */
L8c64245a: *stop=0x8c64245au;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64245c; /* mov.l @(0x4,r3),r0 */
L8c64245c: *stop=0x8c64245cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c64245e; /* mov.l r1,@r2 */
L8c64245e: *stop=0x8c64245eu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c642460; /* mov.l r0,@(0x4,r2) */
L8c642460: *stop=0x8c642460u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642462; /* mov.l @(0xc,r3),r0 */
L8c642462: *stop=0x8c642462u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c642464; /* mov.l @(0x8,r3),r1 */
L8c642464: *stop=0x8c642464u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c642466; /* mov.l r1,@(0x8,r2) */
L8c642466: *stop=0x8c642466u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[0],4);if(io_status)return io_status;goto L8c642468; /* mov.l r0,@(0xc,r2) */
L8c642468: *stop=0x8c642468u;if(!valid_access(r[3]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64246a; /* mov.l @(0x14,r3),r0 */
L8c64246a: *stop=0x8c64246au;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64246c; /* mov.l @(0x10,r3),r1 */
L8c64246c: *stop=0x8c64246cu;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[1],4);if(io_status)return io_status;goto L8c64246e; /* mov.l r1,@(0x10,r2) */
L8c64246e: *stop=0x8c64246eu;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[0],4);if(io_status)return io_status;goto L8c642470; /* mov.l r0,@(0x14,r2) */
L8c642470: *stop=0x8c642470u;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642472; /* mov.l @(0x1c,r3),r0 */
L8c642472: *stop=0x8c642472u;if(!valid_access(r[3]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c642474; /* mov.l @(0x18,r3),r1 */
L8c642474: *stop=0x8c642474u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[1],4);if(io_status)return io_status;goto L8c642476; /* mov.l r1,@(0x18,r2) */
L8c642476: *stop=0x8c642476u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c642478; /* mov.l r0,@(0x1c,r2) */
L8c642478: *stop=0x8c642478u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642664u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64247a; /* mov.l 0x8c642664,r2 */
L8c64247a: *stop=0x8c64247au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64247c; /* mov.l @r2,r0 */
L8c64247c: *stop=0x8c64247cu;*sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c64247e; /* tst #0x40,r0 */
L8c64247e: if((*sr&1u))goto L8c64249c;goto L8c642480; /* bt 0x8c64249c */
L8c642480: *stop=0x8c642480u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642668u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642482; /* mov.l 0x8c642668,r0 */
L8c642482: *stop=0x8c642482u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c642484; /* mov.l @r0,r1 */
L8c642484: *stop=0x8c642484u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c642486; /* tst r1,r1 */
L8c642486: if((*sr&1u))goto L8c64249c;goto L8c642488; /* bt 0x8c64249c */
L8c642488: *stop=0x8c642488u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64266cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64248a; /* mov.l 0x8c64266c,r1 */
L8c64248a: *stop=0x8c64248au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64248c; /* mov.l @r1,r3 */
L8c64248c: *stop=0x8c64248cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64248e; /* tst r3,r3 */
L8c64248e: if((*sr&1u))goto L8c64249c;goto L8c642490; /* bt 0x8c64249c */
L8c642490: *stop=0x8c642490u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642670u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642492; /* mov.l 0x8c642670,r2 */
L8c642492: *stop=0x8c642492u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642494; /* mov.l @r2,r3 */
L8c642494: *stop=0x8c642494u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c642496; /* tst r3,r3 */
L8c642496: if((*sr&1u))goto L8c64249c;goto L8c642498; /* bt 0x8c64249c */
L8c642498: target=0x8c641bc0u;*pr=0x8c64249cu;*stop=0x8c64249au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64249c; /* bsr 0x8c641bc0 */
L8c64249c: *stop=0x8c64249cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642674u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64249e; /* mov.l 0x8c642674,r1 */
L8c64249e: *stop=0x8c64249eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[13]=loaded;}goto L8c6424a0; /* mov.l @r1,r13 */
L8c6424a0: *stop=0x8c6424a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642678u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6424a2; /* mov.l 0x8c642678,r2 */
L8c6424a2: *stop=0x8c6424a2u;r[13]<<=2;goto L8c6424a4; /* shll2 r13 */
L8c6424a4: *stop=0x8c6424a4u;r[3]=r[15];goto L8c6424a6; /* mov r15,r3 */
L8c6424a6: *stop=0x8c6424a6u;r[3]+=68u;goto L8c6424a8; /* add #0x44,r3 */
L8c6424a8: *stop=0x8c6424a8u;r[2]+=r[13];goto L8c6424aa; /* add r13,r2 */
L8c6424aa: *stop=0x8c6424aau;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6424ac; /* mov.l r3,@(0x4,r15) */
L8c6424ac: *stop=0x8c6424acu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6424ae; /* mov.l r2,@r3 */
L8c6424ae: *stop=0x8c6424aeu;r[5]=r[15];goto L8c6424b0; /* mov r15,r5 */
L8c6424b0: *stop=0x8c6424b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642680u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6424b2; /* mov.l 0x8c642680,r2 */
L8c6424b2: *stop=0x8c6424b2u;r[5]+=76u;goto L8c6424b4; /* add #0x4c,r5 */
L8c6424b4: *stop=0x8c6424b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64267cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6424b6; /* mov.l 0x8c64267c,r3 */
L8c6424b6: *stop=0x8c6424b6u;r[6]=r[15];goto L8c6424b8; /* mov r15,r6 */
L8c6424b8: *stop=0x8c6424b8u;r[2]+=r[13];goto L8c6424ba; /* add r13,r2 */
L8c6424ba: *stop=0x8c6424bau;r[7]=r[15];goto L8c6424bc; /* mov r15,r7 */
L8c6424bc: *stop=0x8c6424bcu;r[6]+=72u;goto L8c6424be; /* add #0x48,r6 */
L8c6424be: *stop=0x8c6424beu;r[14]=r[15];goto L8c6424c0; /* mov r15,r14 */
L8c6424c0: *stop=0x8c6424c0u;r[3]+=r[13];goto L8c6424c2; /* add r13,r3 */
L8c6424c2: *stop=0x8c6424c2u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c6424c4; /* mov.l r3,@r6 */
L8c6424c4: *stop=0x8c6424c4u;r[3]=r[15];goto L8c6424c6; /* mov r15,r3 */
L8c6424c6: *stop=0x8c6424c6u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[2],4);if(io_status)return io_status;goto L8c6424c8; /* mov.l r2,@r5 */
L8c6424c8: *stop=0x8c6424c8u;r[3]+=80u;goto L8c6424ca; /* add #0x50,r3 */
L8c6424ca: *stop=0x8c6424cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c642684u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6424cc; /* mov.l 0x8c642684,r2 */
L8c6424cc: *stop=0x8c6424ccu;r[7]+=84u;goto L8c6424ce; /* add #0x54,r7 */
L8c6424ce: *stop=0x8c6424ceu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6424d0; /* mov.l r3,@r15 */
L8c6424d0: *stop=0x8c6424d0u;r[14]+=48u;goto L8c6424d2; /* add #0x30,r14 */
L8c6424d2: *stop=0x8c6424d2u;r[2]+=r[13];goto L8c6424d4; /* add r13,r2 */
L8c6424d4: *stop=0x8c6424d4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6424d6; /* mov.l r2,@r3 */
L8c6424d6: *stop=0x8c6424d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642688u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6424d8; /* mov.l 0x8c642688,r3 */
L8c6424d8: *stop=0x8c6424d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64268cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6424da; /* mov.l 0x8c64268c,r2 */
L8c6424da: *stop=0x8c6424dau;r[3]+=r[13];goto L8c6424dc; /* add r13,r3 */
L8c6424dc: *stop=0x8c6424dcu;r[2]+=r[13];goto L8c6424de; /* add r13,r2 */
L8c6424de: *stop=0x8c6424deu;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[3],4);if(io_status)return io_status;goto L8c6424e0; /* mov.l r3,@r7 */
L8c6424e0: *stop=0x8c6424e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642690u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6424e2; /* mov.l 0x8c642690,r3 */
L8c6424e2: *stop=0x8c6424e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426a0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6424e4; /* mov.l 0x8c6426a0,r4 */
L8c6424e4: *stop=0x8c6424e4u;r[3]+=r[13];goto L8c6424e6; /* add r13,r3 */
L8c6424e6: *stop=0x8c6424e6u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c6424e8; /* mov.l r2,@r14 */
L8c6424e8: *stop=0x8c6424e8u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6424ea; /* mov.l r3,@(0x4,r14) */
L8c6424ea: *stop=0x8c6424eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c642698u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6424ec; /* mov.l 0x8c642698,r3 */
L8c6424ec: *stop=0x8c6424ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642694u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6424ee; /* mov.l 0x8c642694,r2 */
L8c6424ee: *stop=0x8c6424eeu;r[3]+=r[13];goto L8c6424f0; /* add r13,r3 */
L8c6424f0: *stop=0x8c6424f0u;r[2]+=r[13];goto L8c6424f2; /* add r13,r2 */
L8c6424f2: *stop=0x8c6424f2u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6424f4; /* mov.l r2,@(0x8,r14) */
L8c6424f4: *stop=0x8c6424f4u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6424f6; /* mov.l r3,@(0xc,r14) */
L8c6424f6: *stop=0x8c6424f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64269cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6424f8; /* mov.l 0x8c64269c,r2 */
L8c6424f8: *stop=0x8c6424f8u;r[13]+=r[2];goto L8c6424fa; /* add r2,r13 */
L8c6424fa: *stop=0x8c6424fau;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[13],4);if(io_status)return io_status;goto L8c6424fc; /* mov.l r13,@(0x10,r14) */
L8c6424fc: *stop=0x8c6424fcu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6424fe; /* mov.l @r4,r0 */
L8c6424fe: *stop=0x8c6424feu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642500; /* mov.l @(0x4,r15),r3 */
L8c642500: *stop=0x8c642500u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642502; /* mov.l @r0,r0 */
L8c642502: *stop=0x8c642502u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642504; /* mov.l @r3,r3 */
L8c642504: *stop=0x8c642504u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c642506; /* mov.l r0,@r3 */
L8c642506: *stop=0x8c642506u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642508; /* mov.l @r4,r0 */
L8c642508: *stop=0x8c642508u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64250a; /* mov.l @r6,r3 */
L8c64250a: *stop=0x8c64250au;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64250c; /* mov.l @(0x4,r0),r0 */
L8c64250c: *stop=0x8c64250cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c64250e; /* mov.l r0,@r3 */
L8c64250e: *stop=0x8c64250eu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642510; /* mov.l @r4,r0 */
L8c642510: *stop=0x8c642510u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642512; /* mov.l @r5,r3 */
L8c642512: *stop=0x8c642512u;if(!valid_access(r[0]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642514; /* mov.l @(0x8,r0),r0 */
L8c642514: *stop=0x8c642514u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c642516; /* mov.l r0,@r3 */
L8c642516: *stop=0x8c642516u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642518; /* mov.l @r15,r3 */
L8c642518: *stop=0x8c642518u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64251a; /* mov.l @r4,r0 */
L8c64251a: *stop=0x8c64251au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64251c; /* mov.l @r3,r3 */
L8c64251c: *stop=0x8c64251cu;if(!valid_access(r[0]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64251e; /* mov.l @(0xc,r0),r0 */
L8c64251e: *stop=0x8c64251eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c642520; /* mov.l r0,@r3 */
L8c642520: *stop=0x8c642520u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642522; /* mov.l @r4,r0 */
L8c642522: *stop=0x8c642522u;r[10]=1u;goto L8c642524; /* mov #0x1,r10 */
L8c642524: *stop=0x8c642524u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642526; /* mov.l @r7,r3 */
L8c642526: *stop=0x8c642526u;r[11]=5u;goto L8c642528; /* mov #0x5,r11 */
L8c642528: *stop=0x8c642528u;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64252a; /* mov.l @(0x10,r0),r0 */
L8c64252a: *stop=0x8c64252au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426acu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64252c; /* mov.l 0x8c6426ac,r12 */
L8c64252c: *stop=0x8c64252cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c64252e; /* mov.l r0,@r3 */
L8c64252e: *stop=0x8c64252eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426b0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642530; /* mov.l 0x8c6426b0,r0 */
L8c642530: *stop=0x8c642530u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426a8u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c642532; /* mov.l 0x8c6426a8,r9 */
L8c642532: *stop=0x8c642532u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426a4u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c642534; /* mov.l 0x8c6426a4,r8 */
L8c642534: *stop=0x8c642534u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642536; /* mov.l @r0,r3 */
L8c642536: *stop=0x8c642536u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c642538; /* tst r3,r3 */
L8c642538: take=(*sr&1u);*stop=0x8c64253au;r[13]=0u;if(take)goto L8c642540;goto L8c64253c; /* bt/s 0x8c642540 */
L8c64253c: *stop=0x8c64253eu;goto L8c6426e0; /* bra 0x8c6426e0 */
L8c642540: *stop=0x8c642540u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[13],4);if(io_status)return io_status;goto L8c642542; /* mov.l r13,@(0x4,r15) */
L8c642542: *stop=0x8c642542u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642544; /* mov.l @(0x4,r15),r3 */
L8c642544: *stop=0x8c642544u;r[2]=r[10];goto L8c642546; /* mov r10,r2 */
L8c642546: *stop=0x8c642546u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426b4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642548; /* mov.l 0x8c6426b4,r0 */
L8c642548: *stop=0x8c642548u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c64254a; /* shad r3,r2 */
L8c64254a: *stop=0x8c64254au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64254c; /* mov.l @r0,r1 */
L8c64254c: *stop=0x8c64254cu;*sr=(*sr&~1u)|!(r[1]&r[2]);goto L8c64254e; /* tst r1,r2 */
L8c64254e: if((*sr&1u))goto L8c64258c;goto L8c642550; /* bt 0x8c64258c */
L8c642550: *stop=0x8c642550u;r[0]=r[3];goto L8c642552; /* mov r3,r0 */
L8c642552: *stop=0x8c642552u;r[0]<<=2;goto L8c642554; /* shll2 r0 */
L8c642554: *stop=0x8c642554u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c642556; /* mov.l r0,@r15 */
L8c642556: *stop=0x8c642556u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[1]=loaded;}goto L8c642558; /* mov.l @r9,r1 */
L8c642558: *stop=0x8c642558u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64255a; /* mov.l @(r0,r14),r2 */
L8c64255a: *stop=0x8c64255au;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64255c; /* mov.l @(r0,r1),r1 */
L8c64255c: *stop=0x8c64255cu;r[0]=r[15];goto L8c64255e; /* mov r15,r0 */
L8c64255e: *stop=0x8c64255eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642560; /* mov.l @r15,r3 */
L8c642560: *stop=0x8c642560u;r[0]+=68u;goto L8c642562; /* add #0x44,r0 */
L8c642562: *stop=0x8c642562u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642564; /* mov.l @(r0,r3),r3 */
L8c642564: *stop=0x8c642564u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642566; /* mov.l @r3,r0 */
L8c642566: *stop=0x8c642566u;r[1]-=r[0];goto L8c642568; /* sub r0,r1 */
L8c642568: *stop=0x8c642568u;*sr=(*sr&~1u)|((int32_t)r[1]>=0);goto L8c64256a; /* cmp/pz r1 */
L8c64256a: if((*sr&1u))goto L8c64256e;goto L8c64256c; /* bt 0x8c64256e */
L8c64256c: *stop=0x8c64256cu;r[1]+=3u;goto L8c64256e; /* add #0x3,r1 */
L8c64256e: *stop=0x8c64256eu;*sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u);goto L8c642570; /* shar r1 */
L8c642570: *stop=0x8c642570u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426b8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c642572; /* mov.l 0x8c6426b8,r5 */
L8c642572: *stop=0x8c642572u;*sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u);goto L8c642574; /* shar r1 */
L8c642574: *stop=0x8c642574u;r[1]<<=2;goto L8c642576; /* shll2 r1 */
L8c642576: *stop=0x8c642576u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c642578; /* mov.l r1,@r2 */
L8c642578: *stop=0x8c642578u;r[6]=4u;goto L8c64257a; /* mov #0x4,r6 */
L8c64257a: *stop=0x8c64257au;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64257c; /* mov.l @r9,r2 */
L8c64257c: *stop=0x8c64257cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64257e; /* mov.l @r15,r0 */
L8c64257e: target=r[12];*pr=0x8c642582u;*stop=0x8c642580u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642582; /* jsr @r12 */
L8c642582: *stop=0x8c642582u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642584; /* mov.l @r15,r0 */
L8c642584: *stop=0x8c642584u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642586; /* mov.l @r9,r3 */
L8c642586: *stop=0x8c642586u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642588; /* mov.l @(r0,r3),r2 */
L8c642588: *stop=0x8c642588u;r[2]+=32u;goto L8c64258a; /* add #0x20,r2 */
L8c64258a: *stop=0x8c64258au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c64258c; /* mov.l r2,@(r0,r3) */
L8c64258c: *stop=0x8c64258cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64258e; /* mov.l @(0x4,r15),r3 */
L8c64258e: *stop=0x8c64258eu;r[3]+=1u;goto L8c642590; /* add #0x1,r3 */
L8c642590: *stop=0x8c642590u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[11]);goto L8c642592; /* cmp/ge r11,r3 */
L8c642592: take=!(*sr&1u);*stop=0x8c642594u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;if(take)goto L8c642542;goto L8c642596; /* bf/s 0x8c642542 */
L8c642596: *stop=0x8c642596u;r[4]=r[13];goto L8c642598; /* mov r13,r4 */
L8c642598: *stop=0x8c642598u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426b4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64259a; /* mov.l 0x8c6426b4,r1 */
L8c64259a: *stop=0x8c64259au;r[3]=r[10];goto L8c64259c; /* mov r10,r3 */
L8c64259c: *stop=0x8c64259cu;{uint32_t shift=r[4];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}}goto L8c64259e; /* shad r4,r3 */
L8c64259e: *stop=0x8c64259eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6425a0; /* mov.l @r1,r2 */
L8c6425a0: *stop=0x8c6425a0u;*sr=(*sr&~1u)|!(r[2]&r[3]);goto L8c6425a2; /* tst r2,r3 */
L8c6425a2: if(!(*sr&1u))goto L8c6425ac;goto L8c6425a4; /* bf 0x8c6425ac */
L8c6425a4: *stop=0x8c6425a4u;r[0]=r[4];goto L8c6425a6; /* mov r4,r0 */
L8c6425a6: *stop=0x8c6425a6u;r[0]<<=2;goto L8c6425a8; /* shll2 r0 */
L8c6425a8: *stop=0x8c6425a8u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6425aa; /* mov.l @(r0,r14),r3 */
L8c6425aa: *stop=0x8c6425aau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c6425ac; /* mov.l r13,@r3 */
L8c6425ac: *stop=0x8c6425acu;r[4]+=1u;goto L8c6425ae; /* add #0x1,r4 */
L8c6425ae: *stop=0x8c6425aeu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[11]);goto L8c6425b0; /* cmp/ge r11,r4 */
L8c6425b0: if(!(*sr&1u))goto L8c642598;goto L8c6425b2; /* bf 0x8c642598 */
L8c6425b2: *stop=0x8c6425b2u;r[3]=r[15];goto L8c6425b4; /* mov r15,r3 */
L8c6425b4: *stop=0x8c6425b4u;r[3]+=16u;goto L8c6425b6; /* add #0x10,r3 */
L8c6425b6: *stop=0x8c6425b6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6425b8; /* mov.l r3,@r15 */
L8c6425b8: *stop=0x8c6425b8u;r[4]=r[13];goto L8c6425ba; /* mov r13,r4 */
L8c6425ba: *stop=0x8c6425bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426b4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6425bc; /* mov.l 0x8c6426b4,r1 */
L8c6425bc: *stop=0x8c6425bcu;r[2]=r[10];goto L8c6425be; /* mov r10,r2 */
L8c6425be: *stop=0x8c6425beu;{uint32_t shift=r[4];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c6425c0; /* shad r4,r2 */
L8c6425c0: *stop=0x8c6425c0u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6425c2; /* mov.l @r1,r3 */
L8c6425c2: *stop=0x8c6425c2u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c6425c4; /* tst r3,r2 */
L8c6425c4: if((*sr&1u))goto L8c642654;goto L8c6425c6; /* bt 0x8c642654 */
L8c6425c6: *stop=0x8c6425c6u;r[5]=r[4];goto L8c6425c8; /* mov r4,r5 */
L8c6425c8: *stop=0x8c6425c8u;r[5]<<=2;goto L8c6425ca; /* shll2 r5 */
L8c6425ca: *stop=0x8c6425cau;r[0]=r[5];goto L8c6425cc; /* mov r5,r0 */
L8c6425cc: *stop=0x8c6425ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6425ce; /* mov.l 0x8c6426bc,r2 */
L8c6425ce: *stop=0x8c6425ceu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6425d0; /* mov.l @(r0,r14),r3 */
L8c6425d0: *stop=0x8c6425d0u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6425d2; /* mov.l @(r0,r2),r2 */
L8c6425d2: *stop=0x8c6425d2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6425d4; /* mov.l @r3,r3 */
L8c6425d4: *stop=0x8c6425d4u;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c6425d6; /* cmp/hi r2,r3 */
L8c6425d6: if(!(*sr&1u))goto L8c6425e8;goto L8c6425d8; /* bf 0x8c6425e8 */
L8c6425d8: *stop=0x8c6425d8u;r[5]=r[4];goto L8c6425da; /* mov r4,r5 */
L8c6425da: *stop=0x8c6425dau;r[5]<<=2;goto L8c6425dc; /* shll2 r5 */
L8c6425dc: *stop=0x8c6425dcu;r[0]=r[5];goto L8c6425de; /* mov r5,r0 */
L8c6425de: *stop=0x8c6425deu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6425e0; /* mov.l @(r0,r14),r3 */
L8c6425e0: *stop=0x8c6425e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6425e2; /* mov.l 0x8c6426bc,r2 */
L8c6425e2: *stop=0x8c6425e2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6425e4; /* mov.l @r3,r3 */
L8c6425e4: *stop=0x8c6425e4u;r[2]+=r[5];goto L8c6425e6; /* add r5,r2 */
L8c6425e6: *stop=0x8c6425e6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6425e8; /* mov.l r3,@r2 */
L8c6425e8: *stop=0x8c6425e8u;r[5]=r[4];goto L8c6425ea; /* mov r4,r5 */
L8c6425ea: *stop=0x8c6425eau;r[5]<<=2;goto L8c6425ec; /* shll2 r5 */
L8c6425ec: *stop=0x8c6425ecu;r[0]=r[5];goto L8c6425ee; /* mov r5,r0 */
L8c6425ee: *stop=0x8c6425eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426c0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6425f0; /* mov.l 0x8c6426c0,r2 */
L8c6425f0: *stop=0x8c6425f0u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6425f2; /* mov.l @(r0,r14),r3 */
L8c6425f2: *stop=0x8c6425f2u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6425f4; /* mov.l @(r0,r2),r2 */
L8c6425f4: *stop=0x8c6425f4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6425f6; /* mov.l @r3,r3 */
L8c6425f6: *stop=0x8c6425f6u;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c6425f8; /* cmp/hi r2,r3 */
L8c6425f8: if(!(*sr&1u))goto L8c642654;goto L8c6425fa; /* bf 0x8c642654 */
L8c6425fa: *stop=0x8c6425fau;r[9]=r[13];goto L8c6425fc; /* mov r13,r9 */
L8c6425fc: *stop=0x8c6425fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426b4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6425fe; /* mov.l 0x8c6426b4,r1 */
L8c6425fe: *stop=0x8c6425feu;r[2]=r[10];goto L8c642600; /* mov r10,r2 */
L8c642600: *stop=0x8c642600u;{uint32_t shift=r[9];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c642602; /* shad r9,r2 */
L8c642602: *stop=0x8c642602u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642604; /* mov.l @r1,r3 */
L8c642604: *stop=0x8c642604u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c642606; /* tst r3,r2 */
L8c642606: if((*sr&1u))goto L8c64264a;goto L8c642608; /* bt 0x8c64264a */
L8c642608: *stop=0x8c642608u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64260a; /* mov.l @r15,r3 */
L8c64260a: *stop=0x8c64260au;r[0]=r[9];goto L8c64260c; /* mov r9,r0 */
L8c64260c: *stop=0x8c64260cu;r[0]<<=16;goto L8c64260e; /* shll16 r0 */
L8c64260e: *stop=0x8c64260eu;r[4]=r[9];goto L8c642610; /* mov r9,r4 */
L8c642610: *stop=0x8c642610u;r[2]=r[3];goto L8c642612; /* mov r3,r2 */
L8c642612: *stop=0x8c642612u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642614; /* mov.l @r2,r2 */
L8c642614: *stop=0x8c642614u;r[0]<<=8;goto L8c642616; /* shll8 r0 */
L8c642616: *stop=0x8c642616u;r[5]=r[15];goto L8c642618; /* mov r15,r5 */
L8c642618: *stop=0x8c642618u;r[2]&=r[8];goto L8c64261a; /* and r8,r2 */
L8c64261a: *stop=0x8c64261au;r[2]|=r[0];goto L8c64261c; /* or r0,r2 */
L8c64261c: *stop=0x8c64261cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64261e; /* mov.l r2,@r3 */
L8c64261e: *stop=0x8c64261eu;r[4]<<=2;goto L8c642620; /* shll2 r4 */
L8c642620: *stop=0x8c642620u;r[3]=r[15];goto L8c642622; /* mov r15,r3 */
L8c642622: *stop=0x8c642622u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c642624; /* mov.l r4,@(0x8,r15) */
L8c642624: *stop=0x8c642624u;r[3]+=68u;goto L8c642626; /* add #0x44,r3 */
L8c642626: *stop=0x8c642626u;r[4]+=r[3];goto L8c642628; /* add r3,r4 */
L8c642628: *stop=0x8c642628u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c64262a; /* mov.l r4,@(0x4,r15) */
L8c64262a: *stop=0x8c64262au;r[6]=32u;goto L8c64262c; /* mov #0x20,r6 */
L8c64262c: *stop=0x8c64262cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64262e; /* mov.l @r4,r4 */
L8c64262e: *stop=0x8c64262eu;r[5]+=16u;goto L8c642630; /* add #0x10,r5 */
L8c642630: target=r[12];*pr=0x8c642634u;*stop=0x8c642632u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642634; /* jsr @r12 */
L8c642634: *stop=0x8c642634u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c642636; /* mov.l @(0x4,r15),r4 */
L8c642636: *stop=0x8c642636u;r[6]=4u;goto L8c642638; /* mov #0x4,r6 */
L8c642638: *stop=0x8c642638u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6426b8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64263a; /* mov.l 0x8c6426b8,r5 */
L8c64263a: *stop=0x8c64263au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64263c; /* mov.l @r4,r4 */
L8c64263c: *stop=0x8c64263cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64263e; /* mov.l @r4,r4 */
L8c64263e: target=r[12];*pr=0x8c642642u;*stop=0x8c642640u;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642642; /* jsr @r12 */
L8c642642: *stop=0x8c642642u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642644; /* mov.l @(0x8,r15),r0 */
L8c642644: *stop=0x8c642644u;r[2]=32u;goto L8c642646; /* mov #0x20,r2 */
L8c642646: *stop=0x8c642646u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642648; /* mov.l @(r0,r14),r3 */
L8c642648: *stop=0x8c642648u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64264a; /* mov.l r2,@r3 */
L8c64264a: *stop=0x8c64264au;r[9]+=1u;goto L8c64264c; /* add #0x1,r9 */
L8c64264c: *stop=0x8c64264cu;*sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[11]);goto L8c64264e; /* cmp/ge r11,r9 */
L8c64264e: if(!(*sr&1u))goto L8c6425fc;goto L8c642650; /* bf 0x8c6425fc */
L8c642650: *stop=0x8c642652u;goto L8c6429be; /* bra 0x8c6429be */
L8c642654: *stop=0x8c642654u;r[4]+=1u;goto L8c642656; /* add #0x1,r4 */
L8c642656: *stop=0x8c642656u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[11]);goto L8c642658; /* cmp/ge r11,r4 */
L8c642658: if(!(*sr&1u))goto L8c6425ba;goto L8c64265a; /* bf 0x8c6425ba */
L8c64265a: *stop=0x8c64265cu;goto L8c6429d6; /* bra 0x8c6429d6 */
L8c6426e0: *stop=0x8c6426e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642880u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6426e2; /* mov.l 0x8c642880,r1 */
L8c6426e2: *stop=0x8c6426e2u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6426e4; /* mov.l @r1,r0 */
L8c6426e4: *stop=0x8c6426e4u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c6426e6; /* tst #0x20,r0 */
L8c6426e6: if(!(*sr&1u))goto L8c6426ec;goto L8c6426e8; /* bf 0x8c6426ec */
L8c6426e8: *stop=0x8c6426eau;goto L8c6428c0; /* bra 0x8c6428c0 */
L8c6426ec: *stop=0x8c6426ecu;r[3]=r[15];goto L8c6426ee; /* mov r15,r3 */
L8c6426ee: *stop=0x8c6426eeu;r[3]+=16u;goto L8c6426f0; /* add #0x10,r3 */
L8c6426f0: *stop=0x8c6426f0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6426f2; /* mov.l r13,@r15 */
L8c6426f2: *stop=0x8c6426f2u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6426f4; /* mov.l r3,@(0x4,r15) */
L8c6426f4: *stop=0x8c6426f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642884u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6426f6; /* mov.l 0x8c642884,r3 */
L8c6426f6: *stop=0x8c6426f6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6426f8; /* mov.l @r3,r2 */
L8c6426f8: *stop=0x8c6426f8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6426fa; /* mov.l @r15,r1 */
L8c6426fa: *stop=0x8c6426fau;*sr=(*sr&~1u)|(r[2]==r[1]);goto L8c6426fc; /* cmp/eq r2,r1 */
L8c6426fc: if(!(*sr&1u))goto L8c642702;goto L8c6426fe; /* bf 0x8c642702 */
L8c6426fe: *stop=0x8c642700u;goto L8c6427fe; /* bra 0x8c6427fe */
L8c642702: *stop=0x8c642702u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642704; /* mov.l @r15,r2 */
L8c642704: *stop=0x8c642704u;r[1]=r[10];goto L8c642706; /* mov r10,r1 */
L8c642706: *stop=0x8c642706u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642888u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642708; /* mov.l 0x8c642888,r3 */
L8c642708: *stop=0x8c642708u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}}goto L8c64270a; /* shad r2,r1 */
L8c64270a: *stop=0x8c64270au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64270c; /* mov.l @r3,r0 */
L8c64270c: *stop=0x8c64270cu;*sr=(*sr&~1u)|!(r[0]&r[1]);goto L8c64270e; /* tst r0,r1 */
L8c64270e: if((*sr&1u))goto L8c6427f6;goto L8c642710; /* bt 0x8c6427f6 */
L8c642710: *stop=0x8c642710u;r[2]<<=2;goto L8c642712; /* shll2 r2 */
L8c642712: *stop=0x8c642712u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c642714; /* mov.l r2,@(0x8,r15) */
L8c642714: *stop=0x8c642714u;r[2]+=r[14];goto L8c642716; /* add r14,r2 */
L8c642716: *stop=0x8c642716u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c642718; /* mov.l r2,@(0xc,r15) */
L8c642718: *stop=0x8c642718u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64271a; /* mov.l @r2,r1 */
L8c64271a: *stop=0x8c64271au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64271c; /* mov.l @(0x8,r15),r0 */
L8c64271c: *stop=0x8c64271cu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64271e; /* mov.l @r9,r2 */
L8c64271e: *stop=0x8c64271eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642720; /* mov.l @(0x8,r15),r3 */
L8c642720: *stop=0x8c642720u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642722; /* mov.l @(r0,r2),r2 */
L8c642722: *stop=0x8c642722u;r[0]=r[15];goto L8c642724; /* mov r15,r0 */
L8c642724: *stop=0x8c642724u;r[0]+=68u;goto L8c642726; /* add #0x44,r0 */
L8c642726: *stop=0x8c642726u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642728; /* mov.l @(r0,r3),r3 */
L8c642728: *stop=0x8c642728u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64272a; /* mov.l @r3,r0 */
L8c64272a: *stop=0x8c64272au;r[2]-=r[0];goto L8c64272c; /* sub r0,r2 */
L8c64272c: *stop=0x8c64272cu;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c64272e; /* cmp/pz r2 */
L8c64272e: if((*sr&1u))goto L8c642732;goto L8c642730; /* bt 0x8c642732 */
L8c642730: *stop=0x8c642730u;r[2]+=3u;goto L8c642732; /* add #0x3,r2 */
L8c642732: *stop=0x8c642732u;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c642734; /* shar r2 */
L8c642734: *stop=0x8c642734u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64288cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c642736; /* mov.l 0x8c64288c,r5 */
L8c642736: *stop=0x8c642736u;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c642738; /* shar r2 */
L8c642738: *stop=0x8c642738u;r[2]<<=2;goto L8c64273a; /* shll2 r2 */
L8c64273a: *stop=0x8c64273au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c64273c; /* mov.l r2,@r1 */
L8c64273c: *stop=0x8c64273cu;r[6]=4u;goto L8c64273e; /* mov #0x4,r6 */
L8c64273e: *stop=0x8c64273eu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642740; /* mov.l @r9,r2 */
L8c642740: *stop=0x8c642740u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642742; /* mov.l @(0x8,r15),r0 */
L8c642742: target=r[12];*pr=0x8c642746u;*stop=0x8c642744u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642746; /* jsr @r12 */
L8c642746: *stop=0x8c642746u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642748; /* mov.l @(0x8,r15),r0 */
L8c642748: *stop=0x8c642748u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64274a; /* mov.l @r9,r3 */
L8c64274a: *stop=0x8c64274au;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64274c; /* mov.l @(r0,r3),r2 */
L8c64274c: *stop=0x8c64274cu;r[2]+=32u;goto L8c64274e; /* add #0x20,r2 */
L8c64274e: *stop=0x8c64274eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c642750; /* mov.l r2,@(r0,r3) */
L8c642750: *stop=0x8c642750u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642752; /* mov.l @(0xc,r15),r3 */
L8c642752: *stop=0x8c642752u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642890u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642754; /* mov.l 0x8c642890,r0 */
L8c642754: *stop=0x8c642754u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c642756; /* mov.l @(0x8,r15),r1 */
L8c642756: *stop=0x8c642756u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642758; /* mov.l @r3,r2 */
L8c642758: *stop=0x8c642758u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64275a; /* mov.l @(r0,r1),r3 */
L8c64275a: *stop=0x8c64275au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64275c; /* mov.l @r2,r2 */
L8c64275c: *stop=0x8c64275cu;*sr=(*sr&~1u)|(r[2]>r[3]);goto L8c64275e; /* cmp/hi r3,r2 */
L8c64275e: if(!(*sr&1u))goto L8c642770;goto L8c642760; /* bf 0x8c642770 */
L8c642760: *stop=0x8c642760u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c642762; /* mov.l @r15,r4 */
L8c642762: *stop=0x8c642762u;r[2]=r[0];goto L8c642764; /* mov r0,r2 */
L8c642764: *stop=0x8c642764u;r[4]<<=2;goto L8c642766; /* shll2 r4 */
L8c642766: *stop=0x8c642766u;r[0]=r[4];goto L8c642768; /* mov r4,r0 */
L8c642768: *stop=0x8c642768u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64276a; /* mov.l @(r0,r14),r3 */
L8c64276a: *stop=0x8c64276au;r[2]+=r[4];goto L8c64276c; /* add r4,r2 */
L8c64276c: *stop=0x8c64276cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64276e; /* mov.l @r3,r3 */
L8c64276e: *stop=0x8c64276eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c642770; /* mov.l r3,@r2 */
L8c642770: *stop=0x8c642770u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c642772; /* mov.l @r15,r4 */
L8c642772: *stop=0x8c642772u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642894u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642774; /* mov.l 0x8c642894,r2 */
L8c642774: *stop=0x8c642774u;r[4]<<=2;goto L8c642776; /* shll2 r4 */
L8c642776: *stop=0x8c642776u;r[0]=r[4];goto L8c642778; /* mov r4,r0 */
L8c642778: *stop=0x8c642778u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64277a; /* mov.l @(r0,r14),r3 */
L8c64277a: *stop=0x8c64277au;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64277c; /* mov.l @(r0,r2),r2 */
L8c64277c: *stop=0x8c64277cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64277e; /* mov.l @r3,r3 */
L8c64277e: *stop=0x8c64277eu;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c642780; /* cmp/hi r2,r3 */
L8c642780: if(!(*sr&1u))goto L8c6427fe;goto L8c642782; /* bf 0x8c6427fe */
L8c642782: *stop=0x8c642782u;r[10]=r[13];goto L8c642784; /* mov r13,r10 */
L8c642784: *stop=0x8c642784u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642884u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642786; /* mov.l 0x8c642884,r3 */
L8c642786: *stop=0x8c642786u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642788; /* mov.l @r3,r2 */
L8c642788: *stop=0x8c642788u;*sr=(*sr&~1u)|(r[2]==r[10]);goto L8c64278a; /* cmp/eq r2,r10 */
L8c64278a: if((*sr&1u))goto L8c6427de;goto L8c64278c; /* bt 0x8c6427de */
L8c64278c: *stop=0x8c64278cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642888u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64278e; /* mov.l 0x8c642888,r1 */
L8c64278e: *stop=0x8c64278eu;r[0]=1u;goto L8c642790; /* mov #0x1,r0 */
L8c642790: *stop=0x8c642790u;{uint32_t shift=r[10];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c642792; /* shad r10,r0 */
L8c642792: *stop=0x8c642792u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642794; /* mov.l @r1,r2 */
L8c642794: *stop=0x8c642794u;*sr=(*sr&~1u)|!(r[2]&r[0]);goto L8c642796; /* tst r2,r0 */
L8c642796: if((*sr&1u))goto L8c6427de;goto L8c642798; /* bt 0x8c6427de */
L8c642798: *stop=0x8c642798u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64279a; /* mov.l @(0x4,r15),r3 */
L8c64279a: *stop=0x8c64279au;r[4]=r[10];goto L8c64279c; /* mov r10,r4 */
L8c64279c: *stop=0x8c64279cu;r[4]<<=2;goto L8c64279e; /* shll2 r4 */
L8c64279e: *stop=0x8c64279eu;r[5]=r[15];goto L8c6427a0; /* mov r15,r5 */
L8c6427a0: *stop=0x8c6427a0u;r[2]=r[3];goto L8c6427a2; /* mov r3,r2 */
L8c6427a2: *stop=0x8c6427a2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6427a4; /* mov.l @r2,r1 */
L8c6427a4: *stop=0x8c6427a4u;r[2]=r[10];goto L8c6427a6; /* mov r10,r2 */
L8c6427a6: *stop=0x8c6427a6u;r[2]<<=16;goto L8c6427a8; /* shll16 r2 */
L8c6427a8: *stop=0x8c6427a8u;r[2]<<=8;goto L8c6427aa; /* shll8 r2 */
L8c6427aa: *stop=0x8c6427aau;r[1]&=r[8];goto L8c6427ac; /* and r8,r1 */
L8c6427ac: *stop=0x8c6427acu;r[1]|=r[2];goto L8c6427ae; /* or r2,r1 */
L8c6427ae: *stop=0x8c6427aeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c6427b0; /* mov.l r1,@r3 */
L8c6427b0: *stop=0x8c6427b0u;r[3]=r[15];goto L8c6427b2; /* mov r15,r3 */
L8c6427b2: *stop=0x8c6427b2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c6427b4; /* mov.l r4,@r15 */
L8c6427b4: *stop=0x8c6427b4u;r[3]+=68u;goto L8c6427b6; /* add #0x44,r3 */
L8c6427b6: *stop=0x8c6427b6u;r[4]+=r[3];goto L8c6427b8; /* add r3,r4 */
L8c6427b8: *stop=0x8c6427b8u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c6427ba; /* mov.l r4,@(0x8,r15) */
L8c6427ba: *stop=0x8c6427bau;r[6]=32u;goto L8c6427bc; /* mov #0x20,r6 */
L8c6427bc: *stop=0x8c6427bcu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6427be; /* mov.l @r4,r4 */
L8c6427be: *stop=0x8c6427beu;r[5]+=16u;goto L8c6427c0; /* add #0x10,r5 */
L8c6427c0: target=r[12];*pr=0x8c6427c4u;*stop=0x8c6427c2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6427c4; /* jsr @r12 */
L8c6427c4: *stop=0x8c6427c4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6427c6; /* mov.l @(0x8,r15),r4 */
L8c6427c6: *stop=0x8c6427c6u;r[6]=4u;goto L8c6427c8; /* mov #0x4,r6 */
L8c6427c8: *stop=0x8c6427c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64288cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6427ca; /* mov.l 0x8c64288c,r5 */
L8c6427ca: *stop=0x8c6427cau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6427cc; /* mov.l @r4,r4 */
L8c6427cc: *stop=0x8c6427ccu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6427ce; /* mov.l @r4,r4 */
L8c6427ce: target=r[12];*pr=0x8c6427d2u;*stop=0x8c6427d0u;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6427d2; /* jsr @r12 */
L8c6427d2: *stop=0x8c6427d2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6427d4; /* mov.l @r15,r0 */
L8c6427d4: *stop=0x8c6427d4u;r[2]=32u;goto L8c6427d6; /* mov #0x20,r2 */
L8c6427d6: *stop=0x8c6427d6u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6427d8; /* mov.l @(r0,r14),r3 */
L8c6427d8: *stop=0x8c6427d8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6427da; /* mov.l r2,@r3 */
L8c6427da: *stop=0x8c6427dcu;goto L8c6427e6; /* bra 0x8c6427e6 */
L8c6427de: *stop=0x8c6427deu;r[0]=r[10];goto L8c6427e0; /* mov r10,r0 */
L8c6427e0: *stop=0x8c6427e0u;r[0]<<=2;goto L8c6427e2; /* shll2 r0 */
L8c6427e2: *stop=0x8c6427e2u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6427e4; /* mov.l @(r0,r14),r3 */
L8c6427e4: *stop=0x8c6427e4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c6427e6; /* mov.l r13,@r3 */
L8c6427e6: *stop=0x8c6427e6u;r[10]+=1u;goto L8c6427e8; /* add #0x1,r10 */
L8c6427e8: *stop=0x8c6427e8u;*sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[11]);goto L8c6427ea; /* cmp/ge r11,r10 */
L8c6427ea: if(!(*sr&1u))goto L8c642784;goto L8c6427ec; /* bf 0x8c642784 */
L8c6427ec: *stop=0x8c6427ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642898u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6427ee; /* mov.l 0x8c642898,r3 */
L8c6427ee: target=r[3];*pr=0x8c6427f2u;*stop=0x8c6427f0u;r[4]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6427f2; /* jsr @r3 */
L8c6427f2: *stop=0x8c6427f4u;goto L8c64280c; /* bra 0x8c64280c */
L8c6427f6: *stop=0x8c6427f6u;r[0]=r[2];goto L8c6427f8; /* mov r2,r0 */
L8c6427f8: *stop=0x8c6427f8u;r[0]<<=2;goto L8c6427fa; /* shll2 r0 */
L8c6427fa: *stop=0x8c6427fau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6427fc; /* mov.l @(r0,r14),r1 */
L8c6427fc: *stop=0x8c6427fcu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c6427fe; /* mov.l r13,@r1 */
L8c6427fe: *stop=0x8c6427feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642800; /* mov.l @r15,r3 */
L8c642800: *stop=0x8c642800u;r[3]+=1u;goto L8c642802; /* add #0x1,r3 */
L8c642802: *stop=0x8c642802u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[11]);goto L8c642804; /* cmp/ge r11,r3 */
L8c642804: take=(*sr&1u);*stop=0x8c642806u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c64280c;goto L8c642808; /* bt/s 0x8c64280c */
L8c642808: *stop=0x8c64280au;goto L8c6426f4; /* bra 0x8c6426f4 */
L8c64280c: *stop=0x8c64280cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642884u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64280e; /* mov.l 0x8c642884,r2 */
L8c64280e: *stop=0x8c64280eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642810; /* mov.l @r2,r3 */
L8c642810: *stop=0x8c642810u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642812; /* mov.l @r9,r0 */
L8c642812: *stop=0x8c642812u;r[3]<<=2;goto L8c642814; /* shll2 r3 */
L8c642814: *stop=0x8c642814u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642816; /* mov.l @(r0,r3),r3 */
L8c642816: *stop=0x8c642816u;r[0]=r[2];goto L8c642818; /* mov r2,r0 */
L8c642818: *stop=0x8c642818u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64281a; /* mov.l @r0,r1 */
L8c64281a: *stop=0x8c64281au;r[2]=r[15];goto L8c64281c; /* mov r15,r2 */
L8c64281c: *stop=0x8c64281cu;r[2]+=68u;goto L8c64281e; /* add #0x44,r2 */
L8c64281e: *stop=0x8c64281eu;r[1]<<=2;goto L8c642820; /* shll2 r1 */
L8c642820: *stop=0x8c642820u;r[1]+=r[2];goto L8c642822; /* add r2,r1 */
L8c642822: *stop=0x8c642822u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c642824; /* mov.l @r1,r1 */
L8c642824: *stop=0x8c642824u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c642826; /* mov.l @r1,r1 */
L8c642826: *stop=0x8c642826u;*sr=(*sr&~1u)|(r[1]==r[3]);goto L8c642828; /* cmp/eq r1,r3 */
L8c642828: if((*sr&1u))goto L8c64282e;goto L8c64282a; /* bt 0x8c64282e */
L8c64282a: *stop=0x8c64282cu;goto L8c6429d6; /* bra 0x8c6429d6 */
L8c64282e: *stop=0x8c64282eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64289cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c642830; /* mov.l 0x8c64289c,r1 */
L8c642830: *stop=0x8c642830u;r[4]=16u;goto L8c642832; /* mov #0x10,r4 */
L8c642832: *stop=0x8c642832u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c642834; /* mov.l r4,@r1 */
L8c642834: *stop=0x8c642834u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6428a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642836; /* mov.l 0x8c6428a0,r3 */
L8c642836: *stop=0x8c642836u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c642838; /* mov.l r4,@r3 */
L8c642838: *stop=0x8c642838u;r[4]=r[15];goto L8c64283a; /* mov r15,r4 */
L8c64283a: *stop=0x8c64283au;r[4]+=16u;goto L8c64283c; /* add #0x10,r4 */
L8c64283c: *stop=0x8c64283cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64283e; /* mov.l @r4,r2 */
L8c64283e: *stop=0x8c64283eu;r[8]&=r[2];goto L8c642840; /* and r2,r8 */
L8c642840: *stop=0x8c642840u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642884u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642842; /* mov.l 0x8c642884,r2 */
L8c642842: *stop=0x8c642842u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642844; /* mov.l @r2,r0 */
L8c642844: *stop=0x8c642844u;r[0]&=0x7u;goto L8c642846; /* and #0x7,r0 */
L8c642846: *stop=0x8c642846u;r[1]=r[2];goto L8c642848; /* mov r2,r1 */
L8c642848: *stop=0x8c642848u;r[0]<<=16;goto L8c64284a; /* shll16 r0 */
L8c64284a: *stop=0x8c64284au;r[0]<<=8;goto L8c64284c; /* shll8 r0 */
L8c64284c: *stop=0x8c64284cu;r[8]|=r[0];goto L8c64284e; /* or r0,r8 */
L8c64284e: *stop=0x8c64284eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[8],4);if(io_status)return io_status;goto L8c642850; /* mov.l r8,@r4 */
L8c642850: *stop=0x8c642850u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[14]=loaded;}goto L8c642852; /* mov.l @r1,r14 */
L8c642852: *stop=0x8c642852u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642854; /* mov.l @r9,r0 */
L8c642854: *stop=0x8c642854u;r[14]<<=2;goto L8c642856; /* shll2 r14 */
L8c642856: *stop=0x8c642856u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6428a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642858; /* mov.l 0x8c6428a4,r3 */
L8c642858: *stop=0x8c642858u;r[5]=r[15];goto L8c64285a; /* mov r15,r5 */
L8c64285a: *stop=0x8c64285au;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c64285c; /* mov.l @(r0,r14),r14 */
L8c64285c: *stop=0x8c64285cu;r[5]+=16u;goto L8c64285e; /* add #0x10,r5 */
L8c64285e: *stop=0x8c64285eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6428a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642860; /* mov.l 0x8c6428a8,r2 */
L8c642860: *stop=0x8c642860u;r[6]=32u;goto L8c642862; /* mov #0x20,r6 */
L8c642862: *stop=0x8c642862u;r[14]&=r[3];goto L8c642864; /* and r3,r14 */
L8c642864: *stop=0x8c642864u;r[14]|=r[2];goto L8c642866; /* or r2,r14 */
L8c642866: target=r[12];*pr=0x8c64286au;*stop=0x8c642868u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64286a; /* jsr @r12 */
L8c64286a: *stop=0x8c64286au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c64286c; /* pref @r14 */
L8c64286c: *stop=0x8c64286eu;goto L8c6429d6; /* bra 0x8c6429d6 */
L8c6428c0: *stop=0x8c6428c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6428c2; /* mov.l 0x8c642a40,r3 */
L8c6428c2: target=r[3];*pr=0x8c6428c6u;*stop=0x8c6428c4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6428c6; /* jsr @r3 */
L8c6428c6: *stop=0x8c6428c6u;r[3]=r[15];goto L8c6428c8; /* mov r15,r3 */
L8c6428c8: *stop=0x8c6428c8u;r[3]+=16u;goto L8c6428ca; /* add #0x10,r3 */
L8c6428ca: *stop=0x8c6428cau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6428cc; /* mov.l r13,@r15 */
L8c6428cc: *stop=0x8c6428ccu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6428ce; /* mov.l r3,@(0x8,r15) */
L8c6428ce: *stop=0x8c6428ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6428d0; /* mov.l @r15,r0 */
L8c6428d0: *stop=0x8c6428d0u;r[0]<<=2;goto L8c6428d2; /* shll2 r0 */
L8c6428d2: *stop=0x8c6428d2u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c6428d4; /* mov.l r0,@(0x4,r15) */
L8c6428d4: *stop=0x8c6428d4u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6428d6; /* mov.l @r9,r2 */
L8c6428d6: *stop=0x8c6428d6u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6428d8; /* mov.l @(r0,r2),r1 */
L8c6428d8: *stop=0x8c6428d8u;r[2]=r[15];goto L8c6428da; /* mov r15,r2 */
L8c6428da: *stop=0x8c6428dau;r[2]+=68u;goto L8c6428dc; /* add #0x44,r2 */
L8c6428dc: *stop=0x8c6428dcu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6428de; /* mov.l @(r0,r2),r2 */
L8c6428de: *stop=0x8c6428deu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6428e0; /* mov.l @r2,r2 */
L8c6428e0: *stop=0x8c6428e0u;r[1]-=r[2];goto L8c6428e2; /* sub r2,r1 */
L8c6428e2: *stop=0x8c6428e2u;*sr=(*sr&~1u)|((int32_t)r[1]>=0);goto L8c6428e4; /* cmp/pz r1 */
L8c6428e4: take=(*sr&1u);*stop=0x8c6428e6u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}if(take)goto L8c6428ea;goto L8c6428e8; /* bt/s 0x8c6428ea */
L8c6428e8: *stop=0x8c6428e8u;r[1]+=3u;goto L8c6428ea; /* add #0x3,r1 */
L8c6428ea: *stop=0x8c6428eau;*sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u);goto L8c6428ec; /* shar r1 */
L8c6428ec: *stop=0x8c6428ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a44u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6428ee; /* mov.l 0x8c642a44,r5 */
L8c6428ee: *stop=0x8c6428eeu;*sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u);goto L8c6428f0; /* shar r1 */
L8c6428f0: *stop=0x8c6428f0u;r[1]<<=2;goto L8c6428f2; /* shll2 r1 */
L8c6428f2: *stop=0x8c6428f2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c6428f4; /* mov.l r1,@r3 */
L8c6428f4: *stop=0x8c6428f4u;r[6]=4u;goto L8c6428f6; /* mov #0x4,r6 */
L8c6428f6: *stop=0x8c6428f6u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6428f8; /* mov.l @r9,r3 */
L8c6428f8: *stop=0x8c6428f8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6428fa; /* mov.l @(0x4,r15),r0 */
L8c6428fa: target=r[12];*pr=0x8c6428feu;*stop=0x8c6428fcu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6428fe; /* jsr @r12 */
L8c6428fe: *stop=0x8c6428feu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642900; /* mov.l @(0x4,r15),r0 */
L8c642900: *stop=0x8c642900u;r[1]=r[10];goto L8c642902; /* mov r10,r1 */
L8c642902: *stop=0x8c642902u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642904; /* mov.l @r9,r3 */
L8c642904: *stop=0x8c642904u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642906; /* mov.l @(r0,r3),r2 */
L8c642906: *stop=0x8c642906u;r[2]+=32u;goto L8c642908; /* add #0x20,r2 */
L8c642908: *stop=0x8c642908u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c64290a; /* mov.l r2,@(r0,r3) */
L8c64290a: *stop=0x8c64290au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64290c; /* mov.l @r15,r3 */
L8c64290c: *stop=0x8c64290cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a48u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64290e; /* mov.l 0x8c642a48,r0 */
L8c64290e: *stop=0x8c64290eu;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}}goto L8c642910; /* shad r3,r1 */
L8c642910: *stop=0x8c642910u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642912; /* mov.l @r0,r2 */
L8c642912: *stop=0x8c642912u;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c642914; /* tst r2,r1 */
L8c642914: if(!(*sr&1u))goto L8c64291e;goto L8c642916; /* bf 0x8c64291e */
L8c642916: *stop=0x8c642916u;r[0]=r[3];goto L8c642918; /* mov r3,r0 */
L8c642918: *stop=0x8c642918u;r[0]<<=2;goto L8c64291a; /* shll2 r0 */
L8c64291a: *stop=0x8c64291au;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64291c; /* mov.l @(r0,r14),r2 */
L8c64291c: *stop=0x8c64291cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c64291e; /* mov.l r13,@r2 */
L8c64291e: *stop=0x8c64291eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642920; /* mov.l @r15,r3 */
L8c642920: *stop=0x8c642920u;r[2]=r[10];goto L8c642922; /* mov r10,r2 */
L8c642922: *stop=0x8c642922u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a48u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642924; /* mov.l 0x8c642a48,r0 */
L8c642924: *stop=0x8c642924u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c642926; /* shad r3,r2 */
L8c642926: *stop=0x8c642926u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c642928; /* mov.l @r0,r1 */
L8c642928: *stop=0x8c642928u;*sr=(*sr&~1u)|!(r[1]&r[2]);goto L8c64292a; /* tst r1,r2 */
L8c64292a: if((*sr&1u))goto L8c6429c8;goto L8c64292c; /* bt 0x8c6429c8 */
L8c64292c: *stop=0x8c64292cu;r[3]<<=2;goto L8c64292e; /* shll2 r3 */
L8c64292e: *stop=0x8c64292eu;r[0]=r[3];goto L8c642930; /* mov r3,r0 */
L8c642930: *stop=0x8c642930u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c642932; /* mov.l r3,@(0x4,r15) */
L8c642932: *stop=0x8c642932u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642934; /* mov.l @(r0,r14),r3 */
L8c642934: *stop=0x8c642934u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a4cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642936; /* mov.l 0x8c642a4c,r0 */
L8c642936: *stop=0x8c642936u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642938; /* mov.l @(0x4,r15),r2 */
L8c642938: *stop=0x8c642938u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64293a; /* mov.l @r3,r3 */
L8c64293a: *stop=0x8c64293au;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64293c; /* mov.l @(r0,r2),r1 */
L8c64293c: *stop=0x8c64293cu;*sr=(*sr&~1u)|(r[3]>r[1]);goto L8c64293e; /* cmp/hi r1,r3 */
L8c64293e: if(!(*sr&1u))goto L8c64294c;goto L8c642940; /* bf 0x8c64294c */
L8c642940: *stop=0x8c642940u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642942; /* mov.l @(0x4,r15),r3 */
L8c642942: *stop=0x8c642942u;r[0]=r[3];goto L8c642944; /* mov r3,r0 */
L8c642944: *stop=0x8c642944u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642946; /* mov.l @(r0,r14),r2 */
L8c642946: *stop=0x8c642946u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a4cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642948; /* mov.l 0x8c642a4c,r0 */
L8c642948: *stop=0x8c642948u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64294a; /* mov.l @r2,r1 */
L8c64294a: *stop=0x8c64294au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[1],4);if(io_status)return io_status;goto L8c64294c; /* mov.l r1,@(r0,r3) */
L8c64294c: *stop=0x8c64294cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64294e; /* mov.l @(0x4,r15),r0 */
L8c64294e: *stop=0x8c64294eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642950; /* mov.l @(0x4,r15),r2 */
L8c642950: *stop=0x8c642950u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642952; /* mov.l @(r0,r14),r3 */
L8c642952: *stop=0x8c642952u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a50u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642954; /* mov.l 0x8c642a50,r0 */
L8c642954: *stop=0x8c642954u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c642956; /* mov.l @(r0,r2),r1 */
L8c642956: *stop=0x8c642956u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642958; /* mov.l @r3,r3 */
L8c642958: *stop=0x8c642958u;*sr=(*sr&~1u)|(r[3]>r[1]);goto L8c64295a; /* cmp/hi r1,r3 */
L8c64295a: if(!(*sr&1u))goto L8c6429c8;goto L8c64295c; /* bf 0x8c6429c8 */
L8c64295c: *stop=0x8c64295cu;r[9]=r[13];goto L8c64295e; /* mov r13,r9 */
L8c64295e: *stop=0x8c64295eu;r[2]=r[9];goto L8c642960; /* mov r9,r2 */
L8c642960: *stop=0x8c642960u;r[3]=r[10];goto L8c642962; /* mov r10,r3 */
L8c642962: *stop=0x8c642962u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a48u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642964; /* mov.l 0x8c642a48,r0 */
L8c642964: *stop=0x8c642964u;r[2]<<=2;goto L8c642966; /* shll2 r2 */
L8c642966: *stop=0x8c642966u;{uint32_t shift=r[9];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}}goto L8c642968; /* shad r9,r3 */
L8c642968: *stop=0x8c642968u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64296a; /* mov.l r2,@r15 */
L8c64296a: *stop=0x8c64296au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64296c; /* mov.l @r0,r1 */
L8c64296c: *stop=0x8c64296cu;*sr=(*sr&~1u)|!(r[1]&r[3]);goto L8c64296e; /* tst r1,r3 */
L8c64296e: if((*sr&1u))goto L8c6429b2;goto L8c642970; /* bt 0x8c6429b2 */
L8c642970: *stop=0x8c642970u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642972; /* mov.l @(0x8,r15),r3 */
L8c642972: *stop=0x8c642972u;r[0]=r[9];goto L8c642974; /* mov r9,r0 */
L8c642974: *stop=0x8c642974u;r[0]<<=16;goto L8c642976; /* shll16 r0 */
L8c642976: *stop=0x8c642976u;r[5]=r[15];goto L8c642978; /* mov r15,r5 */
L8c642978: *stop=0x8c642978u;r[1]=r[3];goto L8c64297a; /* mov r3,r1 */
L8c64297a: *stop=0x8c64297au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64297c; /* mov.l @r1,r1 */
L8c64297c: *stop=0x8c64297cu;r[0]<<=8;goto L8c64297e; /* shll8 r0 */
L8c64297e: *stop=0x8c64297eu;r[1]&=r[8];goto L8c642980; /* and r8,r1 */
L8c642980: *stop=0x8c642980u;r[1]|=r[0];goto L8c642982; /* or r0,r1 */
L8c642982: *stop=0x8c642982u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c642984; /* mov.l r1,@r3 */
L8c642984: *stop=0x8c642984u;r[3]=r[15];goto L8c642986; /* mov r15,r3 */
L8c642986: *stop=0x8c642986u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c642988; /* mov.l @r15,r4 */
L8c642988: *stop=0x8c642988u;r[3]+=68u;goto L8c64298a; /* add #0x44,r3 */
L8c64298a: *stop=0x8c64298au;r[5]+=16u;goto L8c64298c; /* add #0x10,r5 */
L8c64298c: *stop=0x8c64298cu;r[4]+=r[3];goto L8c64298e; /* add r3,r4 */
L8c64298e: *stop=0x8c64298eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c642990; /* mov.l r4,@(0x4,r15) */
L8c642990: *stop=0x8c642990u;r[6]=32u;goto L8c642992; /* mov #0x20,r6 */
L8c642992: *stop=0x8c642992u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c642994; /* mov.l @r4,r4 */
L8c642994: target=r[12];*pr=0x8c642998u;*stop=0x8c642996u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642998; /* jsr @r12 */
L8c642998: *stop=0x8c642998u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64299a; /* mov.l @(0x4,r15),r4 */
L8c64299a: *stop=0x8c64299au;r[6]=4u;goto L8c64299c; /* mov #0x4,r6 */
L8c64299c: *stop=0x8c64299cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a44u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64299e; /* mov.l 0x8c642a44,r5 */
L8c64299e: *stop=0x8c64299eu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6429a0; /* mov.l @r4,r4 */
L8c6429a0: *stop=0x8c6429a0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6429a2; /* mov.l @r4,r4 */
L8c6429a2: target=r[12];*pr=0x8c6429a6u;*stop=0x8c6429a4u;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6429a6; /* jsr @r12 */
L8c6429a6: *stop=0x8c6429a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6429a8; /* mov.l @r15,r0 */
L8c6429a8: *stop=0x8c6429a8u;r[2]=32u;goto L8c6429aa; /* mov #0x20,r2 */
L8c6429aa: *stop=0x8c6429aau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6429ac; /* mov.l @(r0,r14),r3 */
L8c6429ac: *stop=0x8c6429acu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6429ae; /* mov.l r2,@r3 */
L8c6429ae: *stop=0x8c6429b0u;goto L8c6429b8; /* bra 0x8c6429b8 */
L8c6429b2: *stop=0x8c6429b2u;r[2]+=r[14];goto L8c6429b4; /* add r14,r2 */
L8c6429b4: *stop=0x8c6429b4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6429b6; /* mov.l @r2,r1 */
L8c6429b6: *stop=0x8c6429b6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c6429b8; /* mov.l r13,@r1 */
L8c6429b8: *stop=0x8c6429b8u;r[9]+=1u;goto L8c6429ba; /* add #0x1,r9 */
L8c6429ba: *stop=0x8c6429bau;*sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[11]);goto L8c6429bc; /* cmp/ge r11,r9 */
L8c6429bc: if(!(*sr&1u))goto L8c64295e;goto L8c6429be; /* bf 0x8c64295e */
L8c6429be: *stop=0x8c6429beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a54u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6429c0; /* mov.l 0x8c642a54,r2 */
L8c6429c0: target=r[2];*pr=0x8c6429c4u;*stop=0x8c6429c2u;r[4]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6429c4; /* jsr @r2 */
L8c6429c4: *stop=0x8c6429c6u;goto L8c6429d6; /* bra 0x8c6429d6 */
L8c6429c8: *stop=0x8c6429c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6429ca; /* mov.l @r15,r2 */
L8c6429ca: *stop=0x8c6429cau;r[2]+=1u;goto L8c6429cc; /* add #0x1,r2 */
L8c6429cc: *stop=0x8c6429ccu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[11]);goto L8c6429ce; /* cmp/ge r11,r2 */
L8c6429ce: take=(*sr&1u);*stop=0x8c6429d0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c6429d6;goto L8c6429d2; /* bt/s 0x8c6429d6 */
L8c6429d2: *stop=0x8c6429d4u;goto L8c6428ce; /* bra 0x8c6428ce */
L8c6429d6: *stop=0x8c6429d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a5cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6429d8; /* mov.l 0x8c642a5c,r14 */
L8c6429d8: *stop=0x8c6429d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6429da; /* mov.l 0x8c642a58,r2 */
L8c6429da: *stop=0x8c6429dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c642a60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6429dc; /* mov.l 0x8c642a60,r3 */
L8c6429dc: *stop=0x8c6429dcu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c6429de; /* mov.l r13,@r2 */
L8c6429de: *stop=0x8c6429deu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6429e0; /* mov.l @r14,r0 */
L8c6429e0: *stop=0x8c6429e0u;r[0]+=1u;goto L8c6429e2; /* add #0x1,r0 */
L8c6429e2: *stop=0x8c6429e2u;r[0]&=0x1u;goto L8c6429e4; /* and #0x1,r0 */
L8c6429e4: *stop=0x8c6429e4u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c6429e6; /* mov.l r0,@r14 */
L8c6429e6: target=r[3];*pr=0x8c6429eau;*stop=0x8c6429e8u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6429ea; /* jsr @r3 */
L8c6429ea: *stop=0x8c6429eau;r[0]=0u;goto L8c6429ec; /* mov #0x0,r0 */
L8c6429ec: *stop=0x8c6429ecu;r[15]+=88u;goto L8c6429ee; /* add #0x58,r15 */
L8c6429ee: *stop=0x8c6429eeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6429f0; /* lds.l @r15+,PR */
L8c6429f0: *stop=0x8c6429f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6429f2; /* mov.l @r15+,r8 */
L8c6429f2: *stop=0x8c6429f2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6429f4; /* mov.l @r15+,r9 */
L8c6429f4: *stop=0x8c6429f4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6429f6; /* mov.l @r15+,r10 */
L8c6429f6: *stop=0x8c6429f6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6429f8; /* mov.l @r15+,r11 */
L8c6429f8: *stop=0x8c6429f8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6429fa; /* mov.l @r15+,r12 */
L8c6429fa: *stop=0x8c6429fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6429fc; /* mov.l @r15+,r13 */
L8c6429fc: *stop=0x8c6429fcu;if(*pr!=entry_pr)return -3;*stop=0x8c6429feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_device_8c641bc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c641bc0;
L8c641bc0: *stop=0x8c641bc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641c84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641bc2; /* mov.l 0x8c641c84,r3 */
L8c641bc2: *stop=0x8c641bc2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c641bc4; /* mov.l r14,@-r15 */
L8c641bc4: *stop=0x8c641bc4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c641bc6; /* mov.l r13,@-r15 */
L8c641bc6: *stop=0x8c641bc6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c641bc8; /* mov.l r12,@-r15 */
L8c641bc8: *stop=0x8c641bc8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c641bca; /* mov.l r11,@-r15 */
L8c641bca: *stop=0x8c641bcau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c641bcc; /* sts.l PR,@-r15 */
L8c641bcc: *stop=0x8c641bccu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c641bce; /* mov.l @r3,r4 */
L8c641bce: *stop=0x8c641bceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641c8cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641bd0; /* mov.l 0x8c641c8c,r1 */
L8c641bd0: *stop=0x8c641bd0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c641bd2; /* mov.l @r4,r4 */
L8c641bd2: *stop=0x8c641bd2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641c88u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c641bd4; /* mov.l 0x8c641c88,r14 */
L8c641bd4: *stop=0x8c641bd4u;r[5]=r[4];goto L8c641bd6; /* mov r4,r5 */
L8c641bd6: *stop=0x8c641bd6u;r[5]>>=16;goto L8c641bd8; /* shlr16 r5 */
L8c641bd8: *stop=0x8c641bd8u;r[5]>>=8;goto L8c641bda; /* shlr8 r5 */
L8c641bda: *stop=0x8c641bdau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c641bdc; /* mov.l r5,@r1 */
L8c641bdc: *stop=0x8c641bdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641c90u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641bde; /* mov.l 0x8c641c90,r2 */
L8c641bde: *stop=0x8c641bdeu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c641be0; /* mov.l r5,@r2 */
L8c641be0: *stop=0x8c641be0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641c94u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c641be2; /* mov.l 0x8c641c94,r13 */
L8c641be2: *stop=0x8c641be2u;r[12]=4u;goto L8c641be4; /* mov #0x4,r12 */
L8c641be4: *stop=0x8c641be4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641c98u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641be6; /* mov.l 0x8c641c98,r3 */
L8c641be6: *stop=0x8c641be6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641c9cu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c641be8; /* mov.l 0x8c641c9c,r11 */
L8c641be8: *stop=0x8c641be8u;r[13]&=r[4];goto L8c641bea; /* and r4,r13 */
L8c641bea: *stop=0x8c641beau;r[13]|=r[3];goto L8c641bec; /* or r3,r13 */
L8c641bec: *stop=0x8c641becu;r[5]=r[14];goto L8c641bee; /* mov r14,r5 */
L8c641bee: *stop=0x8c641beeu;r[6]=32u;goto L8c641bf0; /* mov #0x20,r6 */
L8c641bf0: target=r[11];*pr=0x8c641bf4u;*stop=0x8c641bf2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641bf4; /* jsr @r11 */
L8c641bf4: *stop=0x8c641bf4u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[13],1u);if(status)return status;goto L8c641bf6; /* pref @r13 */
L8c641bf6: *stop=0x8c641bf6u;r[12]--;*sr=(*sr&~1u)|(!r[12]);goto L8c641bf8; /* dt r12 */
L8c641bf8: *stop=0x8c641bf8u;r[13]+=32u;goto L8c641bfa; /* add #0x20,r13 */
L8c641bfa: take=!(*sr&1u);*stop=0x8c641bfcu;r[14]+=32u;if(take)goto L8c641bec;goto L8c641bfe; /* bf/s 0x8c641bec */
L8c641bfe: *stop=0x8c641bfeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641c84u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641c00; /* mov.l 0x8c641c84,r2 */
L8c641c00: *stop=0x8c641c00u;r[1]=sx(m,bus,&io_status,0x8c641c80u,2);if(io_status)return io_status;goto L8c641c02; /* mov.w 0x8c641c80,r1 */
L8c641c02: *stop=0x8c641c02u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641c04; /* mov.l @r2,r3 */
L8c641c04: *stop=0x8c641c04u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c641c06; /* mov.l @r3,r0 */
L8c641c06: *stop=0x8c641c06u;r[0]+=r[1];goto L8c641c08; /* add r1,r0 */
L8c641c08: *stop=0x8c641c08u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c641c0a; /* mov.l r0,@r3 */
L8c641c0a: *stop=0x8c641c0au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c641c0c; /* lds.l @r15+,PR */
L8c641c0c: *stop=0x8c641c0cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c641c0e; /* mov.l @r15+,r11 */
L8c641c0e: *stop=0x8c641c0eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c641c10; /* mov.l @r15+,r12 */
L8c641c10: *stop=0x8c641c10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c641c12; /* mov.l @r15+,r13 */
L8c641c12: *stop=0x8c641c12u;if(*pr!=entry_pr)return -3;*stop=0x8c641c14u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_device_8c65253e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65253e;
L8c65253e: *stop=0x8c65253eu;r[6]>>=2;goto L8c652540; /* shlr2 r6 */
L8c652540: *stop=0x8c652540u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[0]=loaded;}r[5]+=4;goto L8c652542; /* mov.l @r5+,r0 */
L8c652542: *stop=0x8c652542u;r[6]--;*sr=(*sr&~1u)|(!r[6]);goto L8c652544; /* dt r6 */
L8c652544: *stop=0x8c652544u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c652546; /* mov.l r0,@r4 */
L8c652546: take=!(*sr&1u);*stop=0x8c652548u;r[4]+=4u;if(take)goto L8c652540;goto L8c65254a; /* bf/s 0x8c652540 */
L8c65254a: *stop=0x8c65254au;if(*pr!=entry_pr)return -3;*stop=0x8c65254cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_device_8c6564e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6564e0;
L8c6564e0: *stop=0x8c6564e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656588u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6564e2; /* mov.l 0x8c656588,r3 */
L8c6564e2: *stop=0x8c6564e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656584u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6564e4; /* mov.l 0x8c656584,r5 */
L8c6564e4: *stop=0x8c6564e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656580u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6564e6; /* mov.l 0x8c656580,r6 */
L8c6564e6: *stop=0x8c6564e6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6564e8; /* mov.l @r3,r0 */
L8c6564e8: *stop=0x8c6564e8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6564ea; /* cmp/eq #0x1,r0 */
L8c6564ea: if((*sr&1u))goto L8c6564fc;goto L8c6564ec; /* bt 0x8c6564fc */
L8c6564ec: *stop=0x8c6564ecu;r[1]=r[4];goto L8c6564ee; /* mov r4,r1 */
L8c6564ee: *stop=0x8c6564eeu;r[1]<<=2;goto L8c6564f0; /* shll2 r1 */
L8c6564f0: *stop=0x8c6564f0u;r[2]=r[4];goto L8c6564f2; /* mov r4,r2 */
L8c6564f2: *stop=0x8c6564f2u;r[1]+=r[2];goto L8c6564f4; /* add r2,r1 */
L8c6564f4: *stop=0x8c6564f4u;r[1]<<=2;goto L8c6564f6; /* shll2 r1 */
L8c6564f6: *stop=0x8c6564f6u;r[1]+=r[5];goto L8c6564f8; /* add r5,r1 */
L8c6564f8: *stop=0x8c6564fau;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[1],4);if(io_status)return io_status;goto L8c6564fe; /* bra 0x8c6564fe */
L8c6564fc: *stop=0x8c6564fcu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[5],4);if(io_status)return io_status;goto L8c6564fe; /* mov.l r5,@r6 */
L8c6564fe: *stop=0x8c6564feu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656500; /* mov.l @r6,r2 */
L8c656500: *stop=0x8c656500u;r[7]=20u;goto L8c656502; /* mov #0x14,r7 */
L8c656502: *stop=0x8c656502u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65658cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c656504; /* mov.l 0x8c65658c,r5 */
L8c656504: *stop=0x8c656504u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c656506; /* mov.l @r2,r1 */
L8c656506: *stop=0x8c656506u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656508; /* mov.l @r5,r3 */
L8c656508: *stop=0x8c656508u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c65650a; /* mov.l r1,@r3 */
L8c65650a: *stop=0x8c65650au;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65650c; /* mov.l @r6,r2 */
L8c65650c: *stop=0x8c65650cu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65650e; /* mov.l @r5,r3 */
L8c65650e: *stop=0x8c65650eu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656510; /* mov.l @(0x4,r2),r1 */
L8c656510: *stop=0x8c656510u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c656512; /* mov.l r1,@(0x4,r3) */
L8c656512: *stop=0x8c656512u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656514; /* mov.l @r6,r2 */
L8c656514: *stop=0x8c656514u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656516; /* mov.l @r5,r3 */
L8c656516: *stop=0x8c656516u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656518; /* mov.l @(0x8,r2),r1 */
L8c656518: *stop=0x8c656518u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c65651a; /* mov.l r1,@(0x8,r3) */
L8c65651a: *stop=0x8c65651au;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65651c; /* mov.l @r6,r2 */
L8c65651c: *stop=0x8c65651cu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65651e; /* mov.l @r5,r3 */
L8c65651e: *stop=0x8c65651eu;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c656520; /* mov.l @(0xc,r2),r1 */
L8c656520: *stop=0x8c656520u;if(!valid_access(r[3]+12u,4u))return -3;wr(m,bus,&io_status,r[3]+0xcu,r[1],4);if(io_status)return io_status;goto L8c656522; /* mov.l r1,@(0xc,r3) */
L8c656522: *stop=0x8c656522u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c656524; /* mov.l @r6,r2 */
L8c656524: *stop=0x8c656524u;r[6]=0u;goto L8c656526; /* mov #0x0,r6 */
L8c656526: *stop=0x8c656526u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656528; /* mov.l @r5,r3 */
L8c656528: *stop=0x8c656528u;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65652a; /* mov.l @(0x10,r2),r1 */
L8c65652a: *stop=0x8c65652au;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[1],4);if(io_status)return io_status;goto L8c65652c; /* mov.l r1,@(0x10,r3) */
L8c65652c: *stop=0x8c65652cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c656590u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65652e; /* mov.l 0x8c656590,r1 */
L8c65652e: *stop=0x8c65652eu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656530; /* mov.l @r5,r3 */
L8c656530: *stop=0x8c656530u;r[0]=r[6];goto L8c656532; /* mov r6,r0 */
L8c656532: *stop=0x8c656532u;r[2]=r[6];goto L8c656534; /* mov r6,r2 */
L8c656534: *stop=0x8c656534u;r[6]+=4u;goto L8c656536; /* add #0x4,r6 */
L8c656536: *stop=0x8c656536u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656538; /* mov.l @(r0,r3),r3 */
L8c656538: *stop=0x8c656538u;r[2]+=r[1];goto L8c65653a; /* add r1,r2 */
L8c65653a: *stop=0x8c65653au;*sr=(*sr&~1u)|(r[6]>=r[7]);goto L8c65653c; /* cmp/hs r7,r6 */
L8c65653c: *stop=0x8c65653cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c65653e; /* mov.l r3,@r2 */
L8c65653e: if(!(*sr&1u))goto L8c65652e;goto L8c656540; /* bf 0x8c65652e */
L8c656540: *stop=0x8c656540u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656594u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656542; /* mov.l 0x8c656594,r3 */
L8c656542: *stop=0x8c656542u;r[2]=1u;goto L8c656544; /* mov #0x1,r2 */
L8c656544: *stop=0x8c656544u;r[4]&=r[2];goto L8c656546; /* and r2,r4 */
L8c656546: *stop=0x8c656546u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656598u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c656548; /* mov.l 0x8c656598,r6 */
L8c656548: *stop=0x8c656548u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c65654a; /* mov.l r4,@r3 */
L8c65654a: *stop=0x8c65654au;r[4]=0u;goto L8c65654c; /* mov #0x0,r4 */
L8c65654c: *stop=0x8c65654cu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65654e; /* mov.l @r5,r2 */
L8c65654e: *stop=0x8c65654eu;r[0]=r[4];goto L8c656550; /* mov r4,r0 */
L8c656550: *stop=0x8c656550u;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c656552; /* mov.l @(r0,r6),r3 */
L8c656552: *stop=0x8c656552u;r[2]+=r[4];goto L8c656554; /* add r4,r2 */
L8c656554: *stop=0x8c656554u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c656556; /* mov.l @r2,r1 */
L8c656556: *stop=0x8c656556u;r[4]+=4u;goto L8c656558; /* add #0x4,r4 */
L8c656558: *stop=0x8c656558u;r[3]<<=2;goto L8c65655a; /* shll2 r3 */
L8c65655a: *stop=0x8c65655au;*sr=(*sr&~1u)|(r[4]>=r[7]);goto L8c65655c; /* cmp/hs r7,r4 */
L8c65655c: *stop=0x8c65655cu;r[1]+=r[3];goto L8c65655e; /* add r3,r1 */
L8c65655e: take=!(*sr&1u);*stop=0x8c656560u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;if(take)goto L8c65654c;goto L8c656562; /* bf/s 0x8c65654c */
L8c656562: *stop=0x8c656562u;if(*pr!=entry_pr)return -3;*stop=0x8c656564u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_device_8c65661c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65661c;
L8c65661c: *stop=0x8c65661cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65661e; /* sts.l PR,@-r15 */
L8c65661e: *stop=0x8c65661eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65665cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c656620; /* mov.l 0x8c65665c,r2 */
L8c656620: *stop=0x8c656620u;{uint32_t loaded=rd(m,bus,&io_status,0x8c656658u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c656622; /* mov.l 0x8c656658,r3 */
L8c656622: target=r[2];*pr=0x8c656626u;*stop=0x8c656624u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c656626; /* jsr @r2 */
L8c656626: *stop=0x8c656626u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c656628; /* cmp/eq #0x1,r0 */
L8c656628: if((*sr&1u))goto L8c656630;goto L8c65662a; /* bt 0x8c656630 */
L8c65662a: *stop=0x8c65662au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65662c; /* lds.l @r15+,PR */
L8c65662c: *stop=0x8c65662cu;if(*pr!=entry_pr)return -3;*stop=0x8c65662eu;r[0]=13u;return 0; /* rts */
L8c656630: *stop=0x8c656630u;r[0]=0u;goto L8c656632; /* mov #0x0,r0 */
L8c656632: *stop=0x8c656632u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c656634; /* lds.l @r15+,PR */
L8c656634: *stop=0x8c656634u;if(*pr!=entry_pr)return -3;*stop=0x8c656636u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_device_8c650f78(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c650f78;
L8c650f78: *stop=0x8c650f78u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c650f7a; /* mov.l r14,@-r15 */
L8c650f7a: *stop=0x8c650f7au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c650f7c; /* mov.l r12,@-r15 */
L8c650f7c: *stop=0x8c650f7cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c650f7e; /* mov.l r11,@-r15 */
L8c650f7e: *stop=0x8c650f7eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c650f80; /* mov.l r10,@-r15 */
L8c650f80: *stop=0x8c650f80u;r[10]=8u;goto L8c650f82; /* mov #0x8,r10 */
L8c650f82: *stop=0x8c650f82u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c650f84; /* mov.l r9,@-r15 */
L8c650f84: *stop=0x8c650f84u;r[9]=0u;goto L8c650f86; /* mov #0x0,r9 */
L8c650f86: *stop=0x8c650f86u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c650f88; /* mov.l r8,@-r15 */
L8c650f88: *stop=0x8c650f88u;r[8]=sx(m,bus,&io_status,0x8c650fe0u,2);if(io_status)return io_status;goto L8c650f8a; /* mov.w 0x8c650fe0,r8 */
L8c650f8a: *stop=0x8c650f8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c650fe4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c650f8c; /* mov.l 0x8c650fe4,r6 */
L8c650f8c: *stop=0x8c650f8cu;r[14]=sx(m,bus,&io_status,0x8c650fe2u,2);if(io_status)return io_status;goto L8c650f8e; /* mov.w 0x8c650fe2,r14 */
L8c650f8e: *stop=0x8c650f8eu;r[8]+=r[6];goto L8c650f90; /* add r6,r8 */
L8c650f90: *stop=0x8c650f90u;r[7]=r[6];goto L8c650f92; /* mov r6,r7 */
L8c650f92: *stop=0x8c650f92u;*sr=(*sr&~1u)|(r[7]>=r[8]);goto L8c650f94; /* cmp/hs r8,r7 */
L8c650f94: *stop=0x8c650f94u;r[5]=r[6];goto L8c650f96; /* mov r6,r5 */
L8c650f96: *stop=0x8c650f96u;r[12]=r[6];goto L8c650f98; /* mov r6,r12 */
L8c650f98: take=(*sr&1u);*stop=0x8c650f9au;r[11]=3u;if(take)goto L8c650fc4;goto L8c650f9c; /* bt/s 0x8c650fc4 */
L8c650f9c: *stop=0x8c650f9cu;r[6]=r[12];goto L8c650f9e; /* mov r12,r6 */
L8c650f9e: *stop=0x8c650f9eu;r[5]=r[7];goto L8c650fa0; /* mov r7,r5 */
L8c650fa0: *stop=0x8c650fa0u;r[0]=r[9];goto L8c650fa2; /* mov r9,r0 */
L8c650fa2: *stop=0x8c650fa2u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c650fa4; /* mov.l @r6,r2 */
L8c650fa4: *stop=0x8c650fa4u;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c650fa6; /* cmp/eq r4,r2 */
L8c650fa6: if(!(*sr&1u))goto L8c650fb2;goto L8c650fa8; /* bf 0x8c650fb2 */
L8c650fa8: *stop=0x8c650fa8u;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650faa; /* mov.l @(0x18,r5),r1 */
L8c650faa: *stop=0x8c650faau;*sr=(*sr&~1u)|!(r[11]&r[1]);goto L8c650fac; /* tst r11,r1 */
L8c650fac: if((*sr&1u))goto L8c650fb2;goto L8c650fae; /* bt 0x8c650fb2 */
L8c650fae: *stop=0x8c650fb0u;r[0]=0u;goto L8c650fc6; /* bra 0x8c650fc6 */
L8c650fb2: *stop=0x8c650fb2u;r[0]+=1u;goto L8c650fb4; /* add #0x1,r0 */
L8c650fb4: *stop=0x8c650fb4u;*sr=(*sr&~1u)|(r[0]>=r[10]);goto L8c650fb6; /* cmp/hs r10,r0 */
L8c650fb6: *stop=0x8c650fb6u;r[5]+=32u;goto L8c650fb8; /* add #0x20,r5 */
L8c650fb8: take=!(*sr&1u);*stop=0x8c650fbau;r[6]+=32u;if(take)goto L8c650fa2;goto L8c650fbc; /* bf/s 0x8c650fa2 */
L8c650fbc: *stop=0x8c650fbcu;r[7]+=r[14];goto L8c650fbe; /* add r14,r7 */
L8c650fbe: *stop=0x8c650fbeu;*sr=(*sr&~1u)|(r[7]>=r[8]);goto L8c650fc0; /* cmp/hs r8,r7 */
L8c650fc0: take=!(*sr&1u);*stop=0x8c650fc2u;r[12]+=r[14];if(take)goto L8c650f9c;goto L8c650fc4; /* bf/s 0x8c650f9c */
L8c650fc4: *stop=0x8c650fc4u;r[0]=1u;goto L8c650fc6; /* mov #0x1,r0 */
L8c650fc6: *stop=0x8c650fc6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c650fc8; /* mov.l @r15+,r8 */
L8c650fc8: *stop=0x8c650fc8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c650fca; /* mov.l @r15+,r9 */
L8c650fca: *stop=0x8c650fcau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c650fcc; /* mov.l @r15+,r10 */
L8c650fcc: *stop=0x8c650fccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c650fce; /* mov.l @r15+,r11 */
L8c650fce: *stop=0x8c650fceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c650fd0; /* mov.l @r15+,r12 */
L8c650fd0: *stop=0x8c650fd0u;if(*pr!=entry_pr)return -3;*stop=0x8c650fd2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_device_8c6514e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6514e0;
L8c6514e0: *stop=0x8c6514e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651500u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6514e2; /* mov.l 0x8c651500,r3 */
L8c6514e2: *stop=0x8c6514e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c6514e4; /* mov.l r4,@-r15 */
L8c6514e4: *stop=0x8c6514e4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6514e6; /* mov.l @r3,r2 */
L8c6514e6: target=r[2];*stop=0x8c6514e8u;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_boot_device_8c652520(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c652520;
L8c652520: *stop=0x8c652520u;r[0]=r[4];goto L8c652522; /* mov r4,r0 */
L8c652522: *stop=0x8c652522u;r[0]|=r[5];goto L8c652524; /* or r5,r0 */
L8c652524: *stop=0x8c652524u;r[0]|=r[6];goto L8c652526; /* or r6,r0 */
L8c652526: *stop=0x8c652526u;*sr=(*sr&~1u)|!(r[0]&0x7u);goto L8c652528; /* tst #0x7,r0 */
L8c652528: if(!(*sr&1u))goto L8c65253e;goto L8c65252a; /* bf 0x8c65253e */
L8c65252a: *stop=0x8c65252au;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c65252c; /* fschg */
L8c65252c: *stop=0x8c65252cu;r[6]>>=2;goto L8c65252e; /* shlr2 r6 */
L8c65252e: *stop=0x8c65252eu;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c652530; /* shlr r6 */
L8c652530: *stop=0x8c652530u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c652532; /* fmov.s @r5+,fr0 */
L8c652532: *stop=0x8c652532u;r[6]--;*sr=(*sr&~1u)|(!r[6]);goto L8c652534; /* dt r6 */
L8c652534: *stop=0x8c652534u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c652536; /* fmov.s fr0,@r4 */
L8c652536: take=!(*sr&1u);*stop=0x8c652538u;r[4]+=8u;if(take)goto L8c652530;goto L8c65253a; /* bf/s 0x8c652530 */
L8c65253a: *stop=0x8c65253au;if(*pr!=entry_pr)return -3;*stop=0x8c65253cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;return 0; /* rts */
L8c65253e: if(!call)return -2;return call(ctx,0x8c65253eu,m,f,sr,r,pr);
}
