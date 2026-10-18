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
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_frame_context_8c661300(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c661300;
L8c661300: *stop=0x8c661300u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c661302; /* mov.l r14,@-r15 */
L8c661302: *stop=0x8c661302u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c661304; /* mov.l r13,@-r15 */
L8c661304: *stop=0x8c661304u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c661306; /* sts.l PR,@-r15 */
L8c661306: *stop=0x8c661306u;r[15]+=4294967280u;goto L8c661308; /* add #-0x10,r15 */
L8c661308: *stop=0x8c661308u;r[13]=r[15];goto L8c66130a; /* mov r15,r13 */
L8c66130a: *stop=0x8c66130au;r[13]+=4u;goto L8c66130c; /* add #0x4,r13 */
L8c66130c: *stop=0x8c66130cu;if(!valid_access(r[4],4u))return -3;r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c66130e; /* mov.l @r4,r3 */
L8c66130e: *stop=0x8c66130eu;r[14]=r[13];goto L8c661310; /* mov r13,r14 */
L8c661310: *stop=0x8c661310u;r[6]=r[14];goto L8c661312; /* mov r14,r6 */
L8c661312: *stop=0x8c661312u;r[6]+=4u;goto L8c661314; /* add #0x4,r6 */
L8c661314: *stop=0x8c661314u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c661316; /* mov.l r3,@r14 */
L8c661316: *stop=0x8c661316u;r[7]=r[14];goto L8c661318; /* mov r14,r7 */
L8c661318: *stop=0x8c661318u;if(!valid_access(r[4]+4u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c66131a; /* mov.l @(0x4,r4),r2 */
L8c66131a: *stop=0x8c66131au;r[7]+=8u;goto L8c66131c; /* add #0x8,r7 */
L8c66131c: *stop=0x8c66131cu;r[5]=0u;goto L8c66131e; /* mov #0x0,r5 */
L8c66131e: *stop=0x8c66131eu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c661320; /* mov.l r2,@r6 */
L8c661320: *stop=0x8c661320u;if(!valid_access(r[4]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c661322; /* mov.l @(0x8,r4),r3 */
L8c661322: *stop=0x8c661322u;r[2]=rd(m,bus,&io_status,0x8c6615e0u,4);if(io_status)return io_status;goto L8c661324; /* mov.l 0x8c6615e0,r2 */
L8c661324: *stop=0x8c661324u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[3],4);if(io_status)return io_status;goto L8c661326; /* mov.l r3,@r7 */
L8c661326: *stop=0x8c661326u;if(!valid_access(r[2],4u))return -3;r[13]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c661328; /* mov.l @r2,r13 */
L8c661328: *stop=0x8c661328u;r[1]=rd(m,bus,&io_status,0x8c6615e4u,4);if(io_status)return io_status;goto L8c66132a; /* mov.l 0x8c6615e4,r1 */
L8c66132a: *stop=0x8c66132au;if(!valid_access(r[1],4u))return -3;r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c66132c; /* mov.l @r1,r3 */
L8c66132c: *stop=0x8c66132cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c66132e; /* mov.l r3,@r15 */
L8c66132e: *stop=0x8c66132eu;r[3]=rd(m,bus,&io_status,0x8c6615e8u,4);if(io_status)return io_status;goto L8c661330; /* mov.l 0x8c6615e8,r3 */
L8c661330: *stop=0x8c661330u;if(!valid_access(r[3],4u))return -3;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c661332; /* mov.l @r3,r4 */
L8c661332: *stop=0x8c661332u;r[2]=r[5];goto L8c661334; /* mov r5,r2 */
L8c661334: *stop=0x8c661334u;r[0]=rd(m,bus,&io_status,0x8c6615ecu,4);if(io_status)return io_status;goto L8c661336; /* mov.l 0x8c6615ec,r0 */
L8c661336: *stop=0x8c661336u;r[2]<<=2;goto L8c661338; /* shll2 r2 */
L8c661338: *stop=0x8c661338u;r[5]+=1u;goto L8c66133a; /* add #0x1,r5 */
L8c66133a: *stop=0x8c66133au;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[13],4);if(io_status)return io_status;goto L8c66133c; /* mov.l r13,@(r0,r2) */
L8c66133c: *stop=0x8c66133cu;if(!valid_access(r[15],4u))return -3;r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c66133e; /* mov.l @r15,r2 */
L8c66133e: *stop=0x8c66133eu;r[1]=r[5];goto L8c661340; /* mov r5,r1 */
L8c661340: *stop=0x8c661340u;r[1]<<=2;goto L8c661342; /* shll2 r1 */
L8c661342: *stop=0x8c661342u;r[5]+=1u;goto L8c661344; /* add #0x1,r5 */
L8c661344: *stop=0x8c661344u;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[2],4);if(io_status)return io_status;goto L8c661346; /* mov.l r2,@(r0,r1) */
L8c661346: *stop=0x8c661346u;r[1]=r[5];goto L8c661348; /* mov r5,r1 */
L8c661348: *stop=0x8c661348u;r[1]<<=2;goto L8c66134a; /* shll2 r1 */
L8c66134a: *stop=0x8c66134au;r[5]+=1u;goto L8c66134c; /* add #0x1,r5 */
L8c66134c: *stop=0x8c66134cu;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[4],4);if(io_status)return io_status;goto L8c66134e; /* mov.l r4,@(r0,r1) */
L8c66134e: *stop=0x8c66134eu;if(!valid_access(r[14],4u))return -3;r[1]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c661350; /* mov.l @r14,r1 */
L8c661350: *stop=0x8c661350u;r[2]=r[5];goto L8c661352; /* mov r5,r2 */
L8c661352: *stop=0x8c661352u;r[5]+=1u;goto L8c661354; /* add #0x1,r5 */
L8c661354: *stop=0x8c661354u;if(!valid_access(r[1]+4u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;goto L8c661356; /* mov.l @(0x4,r1),r1 */
L8c661356: *stop=0x8c661356u;r[2]<<=2;goto L8c661358; /* shll2 r2 */
L8c661358: *stop=0x8c661358u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c66135a; /* mov.l r1,@(r0,r2) */
L8c66135a: *stop=0x8c66135au;if(!valid_access(r[14],4u))return -3;r[1]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c66135c; /* mov.l @r14,r1 */
L8c66135c: *stop=0x8c66135cu;r[2]=r[5];goto L8c66135e; /* mov r5,r2 */
L8c66135e: *stop=0x8c66135eu;r[5]+=1u;goto L8c661360; /* add #0x1,r5 */
L8c661360: *stop=0x8c661360u;if(!valid_access(r[1]+8u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c661362; /* mov.l @(0x8,r1),r1 */
L8c661362: *stop=0x8c661362u;r[2]<<=2;goto L8c661364; /* shll2 r2 */
L8c661364: *stop=0x8c661364u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c661366; /* mov.l r1,@(r0,r2) */
L8c661366: *stop=0x8c661366u;if(!valid_access(r[14],4u))return -3;r[1]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c661368; /* mov.l @r14,r1 */
L8c661368: *stop=0x8c661368u;r[2]=r[5];goto L8c66136a; /* mov r5,r2 */
L8c66136a: *stop=0x8c66136au;r[5]+=1u;goto L8c66136c; /* add #0x1,r5 */
L8c66136c: *stop=0x8c66136cu;if(!valid_access(r[1]+12u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c66136e; /* mov.l @(0xc,r1),r1 */
L8c66136e: *stop=0x8c66136eu;r[2]<<=2;goto L8c661370; /* shll2 r2 */
L8c661370: *stop=0x8c661370u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c661372; /* mov.l r1,@(r0,r2) */
L8c661372: *stop=0x8c661372u;if(!valid_access(r[14],4u))return -3;r[1]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c661374; /* mov.l @r14,r1 */
L8c661374: *stop=0x8c661374u;r[2]=r[5];goto L8c661376; /* mov r5,r2 */
L8c661376: *stop=0x8c661376u;r[5]+=1u;goto L8c661378; /* add #0x1,r5 */
L8c661378: *stop=0x8c661378u;if(!valid_access(r[1]+24u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x18u,4);if(io_status)return io_status;goto L8c66137a; /* mov.l @(0x18,r1),r1 */
L8c66137a: *stop=0x8c66137au;r[2]<<=2;goto L8c66137c; /* shll2 r2 */
L8c66137c: *stop=0x8c66137cu;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c66137e; /* mov.l r1,@(r0,r2) */
L8c66137e: *stop=0x8c66137eu;if(!valid_access(r[6],4u))return -3;r[1]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c661380; /* mov.l @r6,r1 */
L8c661380: *stop=0x8c661380u;r[2]=r[5];goto L8c661382; /* mov r5,r2 */
L8c661382: *stop=0x8c661382u;r[5]+=1u;goto L8c661384; /* add #0x1,r5 */
L8c661384: *stop=0x8c661384u;if(!valid_access(r[1]+4u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;goto L8c661386; /* mov.l @(0x4,r1),r1 */
L8c661386: *stop=0x8c661386u;r[2]<<=2;goto L8c661388; /* shll2 r2 */
L8c661388: *stop=0x8c661388u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c66138a; /* mov.l r1,@(r0,r2) */
L8c66138a: *stop=0x8c66138au;if(!valid_access(r[6],4u))return -3;r[1]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c66138c; /* mov.l @r6,r1 */
L8c66138c: *stop=0x8c66138cu;r[2]=r[5];goto L8c66138e; /* mov r5,r2 */
L8c66138e: *stop=0x8c66138eu;r[5]+=1u;goto L8c661390; /* add #0x1,r5 */
L8c661390: *stop=0x8c661390u;if(!valid_access(r[1]+8u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c661392; /* mov.l @(0x8,r1),r1 */
L8c661392: *stop=0x8c661392u;r[2]<<=2;goto L8c661394; /* shll2 r2 */
L8c661394: *stop=0x8c661394u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c661396; /* mov.l r1,@(r0,r2) */
L8c661396: *stop=0x8c661396u;if(!valid_access(r[6],4u))return -3;r[1]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c661398; /* mov.l @r6,r1 */
L8c661398: *stop=0x8c661398u;r[2]=r[5];goto L8c66139a; /* mov r5,r2 */
L8c66139a: *stop=0x8c66139au;r[5]+=1u;goto L8c66139c; /* add #0x1,r5 */
L8c66139c: *stop=0x8c66139cu;if(!valid_access(r[1]+12u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c66139e; /* mov.l @(0xc,r1),r1 */
L8c66139e: *stop=0x8c66139eu;r[2]<<=2;goto L8c6613a0; /* shll2 r2 */
L8c6613a0: *stop=0x8c6613a0u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c6613a2; /* mov.l r1,@(r0,r2) */
L8c6613a2: *stop=0x8c6613a2u;if(!valid_access(r[6],4u))return -3;r[1]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6613a4; /* mov.l @r6,r1 */
L8c6613a4: *stop=0x8c6613a4u;r[2]=r[5];goto L8c6613a6; /* mov r5,r2 */
L8c6613a6: *stop=0x8c6613a6u;r[5]+=1u;goto L8c6613a8; /* add #0x1,r5 */
L8c6613a8: *stop=0x8c6613a8u;if(!valid_access(r[1]+24u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x18u,4);if(io_status)return io_status;goto L8c6613aa; /* mov.l @(0x18,r1),r1 */
L8c6613aa: *stop=0x8c6613aau;r[2]<<=2;goto L8c6613ac; /* shll2 r2 */
L8c6613ac: *stop=0x8c6613acu;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c6613ae; /* mov.l r1,@(r0,r2) */
L8c6613ae: *stop=0x8c6613aeu;if(!valid_access(r[7],4u))return -3;r[1]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6613b0; /* mov.l @r7,r1 */
L8c6613b0: *stop=0x8c6613b0u;r[2]=r[5];goto L8c6613b2; /* mov r5,r2 */
L8c6613b2: *stop=0x8c6613b2u;r[5]+=1u;goto L8c6613b4; /* add #0x1,r5 */
L8c6613b4: *stop=0x8c6613b4u;if(!valid_access(r[1]+4u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;goto L8c6613b6; /* mov.l @(0x4,r1),r1 */
L8c6613b6: *stop=0x8c6613b6u;r[2]<<=2;goto L8c6613b8; /* shll2 r2 */
L8c6613b8: *stop=0x8c6613b8u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c6613ba; /* mov.l r1,@(r0,r2) */
L8c6613ba: *stop=0x8c6613bau;if(!valid_access(r[7],4u))return -3;r[1]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6613bc; /* mov.l @r7,r1 */
L8c6613bc: *stop=0x8c6613bcu;r[2]=r[5];goto L8c6613be; /* mov r5,r2 */
L8c6613be: *stop=0x8c6613beu;r[5]+=1u;goto L8c6613c0; /* add #0x1,r5 */
L8c6613c0: *stop=0x8c6613c0u;if(!valid_access(r[1]+8u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c6613c2; /* mov.l @(0x8,r1),r1 */
L8c6613c2: *stop=0x8c6613c2u;r[2]<<=2;goto L8c6613c4; /* shll2 r2 */
L8c6613c4: *stop=0x8c6613c4u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c6613c6; /* mov.l r1,@(r0,r2) */
L8c6613c6: *stop=0x8c6613c6u;if(!valid_access(r[7],4u))return -3;r[1]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6613c8; /* mov.l @r7,r1 */
L8c6613c8: *stop=0x8c6613c8u;r[2]=r[5];goto L8c6613ca; /* mov r5,r2 */
L8c6613ca: *stop=0x8c6613cau;r[5]+=1u;goto L8c6613cc; /* add #0x1,r5 */
L8c6613cc: *stop=0x8c6613ccu;if(!valid_access(r[1]+12u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c6613ce; /* mov.l @(0xc,r1),r1 */
L8c6613ce: *stop=0x8c6613ceu;r[2]<<=2;goto L8c6613d0; /* shll2 r2 */
L8c6613d0: *stop=0x8c6613d0u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c6613d2; /* mov.l r1,@(r0,r2) */
L8c6613d2: *stop=0x8c6613d2u;if(!valid_access(r[7],4u))return -3;r[1]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6613d4; /* mov.l @r7,r1 */
L8c6613d4: *stop=0x8c6613d4u;r[2]=r[5];goto L8c6613d6; /* mov r5,r2 */
L8c6613d6: *stop=0x8c6613d6u;r[5]+=1u;goto L8c6613d8; /* add #0x1,r5 */
L8c6613d8: *stop=0x8c6613d8u;if(!valid_access(r[1]+24u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x18u,4);if(io_status)return io_status;goto L8c6613da; /* mov.l @(0x18,r1),r1 */
L8c6613da: *stop=0x8c6613dau;r[2]<<=2;goto L8c6613dc; /* shll2 r2 */
L8c6613dc: *stop=0x8c6613dcu;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c6613de; /* mov.l r1,@(r0,r2) */
L8c6613de: *stop=0x8c6613deu;r[2]=rd(m,bus,&io_status,0x8c6615f0u,4);if(io_status)return io_status;goto L8c6613e0; /* mov.l 0x8c6615f0,r2 */
L8c6613e0: *stop=0x8c6613e0u;r[5]<<=2;goto L8c6613e2; /* shll2 r5 */
L8c6613e2: *stop=0x8c6613e2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c6613e4; /* mov.l r5,@r2 */
L8c6613e4: target=0x8c66118au;*pr=0x8c6613e8u;*stop=0x8c6613e6u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6613e8; /* bsr 0x8c66118a */
L8c6613e8: *stop=0x8c6613e8u;r[5]=0u;goto L8c6613ea; /* mov #0x0,r5 */
L8c6613ea: *stop=0x8c6613eau;r[4]=r[0];goto L8c6613ec; /* mov r0,r4 */
L8c6613ec: target=0x8c661160u;*pr=0x8c6613f0u;*stop=0x8c6613eeu;r[6]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6613f0; /* bsr 0x8c661160 */
L8c6613f0: *stop=0x8c6613f0u;if(!valid_access(r[14],4u))return -3;r[2]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6613f2; /* mov.l @r14,r2 */
L8c6613f2: *stop=0x8c6613f2u;r[1]=rd(m,bus,&io_status,0x8c6615f4u,4);if(io_status)return io_status;goto L8c6613f4; /* mov.l 0x8c6615f4,r1 */
L8c6613f4: *stop=0x8c6613f4u;if(!valid_access(r[2]+12u,4u))return -3;r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c6613f6; /* mov.l @(0xc,r2),r3 */
L8c6613f6: *stop=0x8c6613f6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c6613f8; /* mov.l r3,@r1 */
L8c6613f8: *stop=0x8c6613f8u;r[0]=0u;goto L8c6613fa; /* mov #0x0,r0 */
L8c6613fa: *stop=0x8c6613fau;r[15]+=16u;goto L8c6613fc; /* add #0x10,r15 */
L8c6613fc: *stop=0x8c6613fcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6613fe; /* lds.l @r15+,PR */
L8c6613fe: *stop=0x8c6613feu;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c661400; /* mov.l @r15+,r13 */
L8c661400: *stop=0x8c661400u;if(*pr!=entry_pr)return -3;*stop=0x8c661402u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_context_8c66118a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c66118a;
L8c66118a: *stop=0x8c66118au;r[3]=rd(m,bus,&io_status,0x8c66128cu,4);if(io_status)return io_status;goto L8c66118c; /* mov.l 0x8c66128c,r3 */
L8c66118c: *stop=0x8c66118cu;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c66118e; /* tst r4,r3 */
L8c66118e: take=(*sr&1u);*stop=0x8c661190u;r[5]=1u;if(take)goto L8c6611a4;goto L8c661192; /* bt/s 0x8c6611a4 */
L8c661192: *stop=0x8c661192u;r[3]=rd(m,bus,&io_status,0x8c661290u,4);if(io_status)return io_status;goto L8c661194; /* mov.l 0x8c661290,r3 */
L8c661194: *stop=0x8c661194u;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c661196; /* tst r4,r3 */
L8c661196: take=(*sr&1u);*stop=0x8c661198u;r[5]+=1u;if(take)goto L8c66119c;goto L8c66119a; /* bt/s 0x8c66119c */
L8c66119a: *stop=0x8c66119au;r[5]+=1u;goto L8c66119c; /* add #0x1,r5 */
L8c66119c: *stop=0x8c66119cu;r[3]=rd(m,bus,&io_status,0x8c661294u,4);if(io_status)return io_status;goto L8c66119e; /* mov.l 0x8c661294,r3 */
L8c66119e: *stop=0x8c66119eu;*sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c6611a0; /* tst r3,r4 */
L8c6611a0: if(!(*sr&1u))goto L8c6611a4;goto L8c6611a2; /* bf 0x8c6611a4 */
L8c6611a2: *stop=0x8c6611a2u;r[5]+=1u;goto L8c6611a4; /* add #0x1,r5 */
L8c6611a4: *stop=0x8c6611a4u;if(*pr!=entry_pr)return -3;*stop=0x8c6611a6u;r[0]=r[5];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_context_8c661160(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c661160;
L8c661160: *stop=0x8c661160u;r[3]=7u;goto L8c661162; /* mov #0x7,r3 */
L8c661162: *stop=0x8c661162u;r[2]=rd(m,bus,&io_status,0x8c661280u,4);if(io_status)return io_status;goto L8c661164; /* mov.l 0x8c661280,r2 */
L8c661164: *stop=0x8c661164u;r[4]&=r[3];goto L8c661166; /* and r3,r4 */
L8c661166: *stop=0x8c661166u;r[4]<<=16;goto L8c661168; /* shll16 r4 */
L8c661168: *stop=0x8c661168u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c66116a; /* tst r6,r6 */
L8c66116a: *stop=0x8c66116au;r[4]<<=8;goto L8c66116c; /* shll8 r4 */
L8c66116c: take=(*sr&1u);*stop=0x8c66116eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;if(take)goto L8c661178;goto L8c661170; /* bt/s 0x8c661178 */
L8c661170: *stop=0x8c661170u;r[1]=rd(m,bus,&io_status,0x8c661284u,4);if(io_status)return io_status;goto L8c661172; /* mov.l 0x8c661284,r1 */
L8c661172: *stop=0x8c661172u;if(!valid_access(r[2],4u))return -3;r[0]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c661174; /* mov.l @r2,r0 */
L8c661174: *stop=0x8c661174u;r[0]|=r[1];goto L8c661176; /* or r1,r0 */
L8c661176: *stop=0x8c661176u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c661178; /* mov.l r0,@r2 */
L8c661178: *stop=0x8c661178u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c66117a; /* tst r5,r5 */
L8c66117a: if((*sr&1u))goto L8c661186;goto L8c66117c; /* bt 0x8c661186 */
L8c66117c: *stop=0x8c66117cu;r[2]=rd(m,bus,&io_status,0x8c661288u,4);if(io_status)return io_status;goto L8c66117e; /* mov.l 0x8c661288,r2 */
L8c66117e: *stop=0x8c66117eu;r[3]=rd(m,bus,&io_status,0x8c661280u,4);if(io_status)return io_status;goto L8c661180; /* mov.l 0x8c661280,r3 */
L8c661180: *stop=0x8c661180u;if(!valid_access(r[3],4u))return -3;r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c661182; /* mov.l @r3,r1 */
L8c661182: *stop=0x8c661182u;r[1]|=r[2];goto L8c661184; /* or r2,r1 */
L8c661184: *stop=0x8c661184u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c661186; /* mov.l r1,@r3 */
L8c661186: *stop=0x8c661186u;if(*pr!=entry_pr)return -3;*stop=0x8c661188u;return 0; /* rts */
}
