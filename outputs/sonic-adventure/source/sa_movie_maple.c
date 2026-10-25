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
SA_STAGE_EXPORT int sa_movie_maple_8c65f558(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65f558;
L8c65f558: *stop=0x8c65f558u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65f55a; /* mov.l r14,@-r15 */
L8c65f55a: *stop=0x8c65f55au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65f55c; /* mov.l r13,@-r15 */
L8c65f55c: *stop=0x8c65f55cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65f55e; /* mov.l r12,@-r15 */
L8c65f55e: *stop=0x8c65f55eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65f560; /* mov.l r11,@-r15 */
L8c65f560: *stop=0x8c65f560u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65f562; /* mov.l r10,@-r15 */
L8c65f562: *stop=0x8c65f562u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65f564; /* mov.l r9,@-r15 */
L8c65f564: *stop=0x8c65f564u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65f566; /* mov.l r8,@-r15 */
L8c65f566: *stop=0x8c65f566u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65f568; /* sts.l PR,@-r15 */
L8c65f568: *stop=0x8c65f568u;r[15]+=4294967288u;goto L8c65f56a; /* add #-0x8,r15 */
L8c65f56a: *stop=0x8c65f56au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f65cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65f56c; /* mov.l 0x8c65f65c,r14 */
L8c65f56c: *stop=0x8c65f56cu;r[13]=0u;goto L8c65f56e; /* mov #0x0,r13 */
L8c65f56e: *stop=0x8c65f56eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f63cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f570; /* mov.w 0x8c65f63c,r0 */
L8c65f570: *stop=0x8c65f570u;r[4]=r[13];goto L8c65f572; /* mov r13,r4 */
L8c65f572: *stop=0x8c65f572u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f574; /* mov.l @r14,r3 */
L8c65f574: *stop=0x8c65f574u;r[12]=96u;goto L8c65f576; /* mov #0x60,r12 */
L8c65f576: *stop=0x8c65f576u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f63eu,2);if(io_status)return io_status;r[6]=loaded;}goto L8c65f578; /* mov.w 0x8c65f63e,r6 */
L8c65f578: *stop=0x8c65f578u;r[7]=r[13];goto L8c65f57a; /* mov r13,r7 */
L8c65f57a: *stop=0x8c65f57au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f57c; /* mov.l r13,@(r0,r3) */
L8c65f57c: *stop=0x8c65f57cu;r[0]+=4u;goto L8c65f57e; /* add #0x4,r0 */
L8c65f57e: *stop=0x8c65f57eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f580; /* mov.l @r14,r3 */
L8c65f580: *stop=0x8c65f580u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f640u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c65f582; /* mov.w 0x8c65f640,r5 */
L8c65f582: *stop=0x8c65f582u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f584; /* mov.l r13,@(r0,r3) */
L8c65f584: *stop=0x8c65f584u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f642u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65f586; /* mov.w 0x8c65f642,r3 */
L8c65f586: *stop=0x8c65f586u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f588; /* mov.l @r14,r0 */
L8c65f588: *stop=0x8c65f588u;r[2]=r[0];goto L8c65f58a; /* mov r0,r2 */
L8c65f58a: *stop=0x8c65f58au;r[3]+=r[0];goto L8c65f58c; /* add r0,r3 */
L8c65f58c: *stop=0x8c65f58cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f644u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f58e; /* mov.w 0x8c65f644,r0 */
L8c65f58e: *stop=0x8c65f58eu;r[3]+=r[4];goto L8c65f590; /* add r4,r3 */
L8c65f590: *stop=0x8c65f590u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f592; /* mov.l @(r0,r2),r1 */
L8c65f592: *stop=0x8c65f592u;r[1]+=r[7];goto L8c65f594; /* add r7,r1 */
L8c65f594: *stop=0x8c65f594u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c65f596; /* mov.l r1,@r3 */
L8c65f596: *stop=0x8c65f596u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f646u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65f598; /* mov.w 0x8c65f646,r3 */
L8c65f598: *stop=0x8c65f598u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f59a; /* mov.l @r14,r0 */
L8c65f59a: *stop=0x8c65f59au;r[2]=r[0];goto L8c65f59c; /* mov r0,r2 */
L8c65f59c: *stop=0x8c65f59cu;r[3]+=r[0];goto L8c65f59e; /* add r0,r3 */
L8c65f59e: *stop=0x8c65f59eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f644u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f5a0; /* mov.w 0x8c65f644,r0 */
L8c65f5a0: *stop=0x8c65f5a0u;r[3]+=r[4];goto L8c65f5a2; /* add r4,r3 */
L8c65f5a2: *stop=0x8c65f5a2u;r[4]+=4u;goto L8c65f5a4; /* add #0x4,r4 */
L8c65f5a4: *stop=0x8c65f5a4u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f5a6; /* mov.l @(r0,r2),r1 */
L8c65f5a6: *stop=0x8c65f5a6u;*sr=(*sr&~1u)|(r[4]>=r[12]);goto L8c65f5a8; /* cmp/hs r12,r4 */
L8c65f5a8: *stop=0x8c65f5a8u;r[1]+=r[6];goto L8c65f5aa; /* add r6,r1 */
L8c65f5aa: *stop=0x8c65f5aau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c65f5ac; /* mov.l r1,@r3 */
L8c65f5ac: *stop=0x8c65f5acu;r[6]+=r[5];goto L8c65f5ae; /* add r5,r6 */
L8c65f5ae: take=!(*sr&1u);*stop=0x8c65f5b0u;r[7]+=r[5];if(take)goto L8c65f584;goto L8c65f5b2; /* bf/s 0x8c65f584 */
L8c65f5b2: *stop=0x8c65f5b2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f5b4; /* mov.l @r14,r2 */
L8c65f5b4: *stop=0x8c65f5b4u;r[6]=8u;goto L8c65f5b6; /* mov #0x8,r6 */
L8c65f5b6: *stop=0x8c65f5b6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f648u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f5b8; /* mov.w 0x8c65f648,r0 */
L8c65f5b8: *stop=0x8c65f5b8u;r[5]=0u;goto L8c65f5ba; /* mov #0x0,r5 */
L8c65f5ba: *stop=0x8c65f5bau;r[3]=r[2];goto L8c65f5bc; /* mov r2,r3 */
L8c65f5bc: *stop=0x8c65f5bcu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f5be; /* mov.l @(r0,r3),r1 */
L8c65f5be: *stop=0x8c65f5beu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f64au,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f5c0; /* mov.w 0x8c65f64a,r0 */
L8c65f5c0: *stop=0x8c65f5c0u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[1],4);if(io_status)return io_status;goto L8c65f5c2; /* mov.l r1,@(r0,r2) */
L8c65f5c2: *stop=0x8c65f5c2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f5c4; /* mov.l @r14,r3 */
L8c65f5c4: *stop=0x8c65f5c4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f648u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f5c6; /* mov.w 0x8c65f648,r0 */
L8c65f5c6: *stop=0x8c65f5c6u;r[2]=r[3];goto L8c65f5c8; /* mov r3,r2 */
L8c65f5c8: *stop=0x8c65f5c8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f64cu,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65f5ca; /* mov.w 0x8c65f64c,r1 */
L8c65f5ca: *stop=0x8c65f5cau;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f5cc; /* mov.l @(r0,r2),r2 */
L8c65f5cc: *stop=0x8c65f5ccu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f64eu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f5ce; /* mov.w 0x8c65f64e,r0 */
L8c65f5ce: *stop=0x8c65f5ceu;r[2]+=r[1];goto L8c65f5d0; /* add r1,r2 */
L8c65f5d0: *stop=0x8c65f5d0u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f5d2; /* mov.l r2,@(r0,r3) */
L8c65f5d2: *stop=0x8c65f5d2u;r[0]+=4294967292u;goto L8c65f5d4; /* add #-0x4,r0 */
L8c65f5d4: *stop=0x8c65f5d4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f5d6; /* mov.l @r14,r3 */
L8c65f5d6: *stop=0x8c65f5d6u;r[2]=r[3];goto L8c65f5d8; /* mov r3,r2 */
L8c65f5d8: *stop=0x8c65f5d8u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f5da; /* mov.l @(r0,r2),r2 */
L8c65f5da: *stop=0x8c65f5dau;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f650u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f5dc; /* mov.w 0x8c65f650,r0 */
L8c65f5dc: *stop=0x8c65f5dcu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f5de; /* mov.l r2,@(r0,r3) */
L8c65f5de: *stop=0x8c65f5deu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f5e0; /* mov.l @r14,r4 */
L8c65f5e0: *stop=0x8c65f5e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f660u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f5e2; /* mov.l 0x8c65f660,r3 */
L8c65f5e2: target=r[3];*pr=0x8c65f5e6u;*stop=0x8c65f5e4u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f5e6; /* jsr @r3 */
L8c65f5e6: *stop=0x8c65f5e6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f5e8; /* mov.l @r14,r4 */
L8c65f5e8: *stop=0x8c65f5e8u;r[5]=0u;goto L8c65f5ea; /* mov #0x0,r5 */
L8c65f5ea: *stop=0x8c65f5eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f660u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f5ec; /* mov.l 0x8c65f660,r3 */
L8c65f5ec: *stop=0x8c65f5ecu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f652u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c65f5ee; /* mov.w 0x8c65f652,r6 */
L8c65f5ee: target=r[3];*pr=0x8c65f5f2u;*stop=0x8c65f5f0u;r[4]+=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f5f2; /* jsr @r3 */
L8c65f5f2: *stop=0x8c65f5f2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f654u,2);if(io_status)return io_status;r[11]=loaded;}goto L8c65f5f4; /* mov.w 0x8c65f654,r11 */
L8c65f5f4: *stop=0x8c65f5f4u;r[12]=r[13];goto L8c65f5f6; /* mov r13,r12 */
L8c65f5f6: *stop=0x8c65f5f6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f656u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65f5f8; /* mov.w 0x8c65f656,r3 */
L8c65f5f8: *stop=0x8c65f5f8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f5fa; /* mov.l @r14,r4 */
L8c65f5fa: *stop=0x8c65f5fau;r[4]+=r[3];goto L8c65f5fc; /* add r3,r4 */
L8c65f5fc: target=0x8c6602d2u;*pr=0x8c65f600u;*stop=0x8c65f5feu;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f600; /* bsr 0x8c6602d2 */
L8c65f600: *stop=0x8c65f600u;r[12]+=120u;goto L8c65f602; /* add #0x78,r12 */
L8c65f602: *stop=0x8c65f602u;*sr=(*sr&~1u)|(r[12]>=r[11]);goto L8c65f604; /* cmp/hs r11,r12 */
L8c65f604: if(!(*sr&1u))goto L8c65f5f6;goto L8c65f606; /* bf 0x8c65f5f6 */
L8c65f606: *stop=0x8c65f606u;r[4]=r[13];goto L8c65f608; /* mov r13,r4 */
L8c65f608: *stop=0x8c65f608u;r[5]=24u;goto L8c65f60a; /* mov #0x18,r5 */
L8c65f60a: *stop=0x8c65f60au;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f658u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65f60c; /* mov.w 0x8c65f658,r3 */
L8c65f60c: *stop=0x8c65f60cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f60e; /* mov.l @r14,r0 */
L8c65f60e: *stop=0x8c65f60eu;r[3]+=r[0];goto L8c65f610; /* add r0,r3 */
L8c65f610: *stop=0x8c65f610u;r[3]+=r[4];goto L8c65f612; /* add r4,r3 */
L8c65f612: *stop=0x8c65f612u;r[4]+=1u;goto L8c65f614; /* add #0x1,r4 */
L8c65f614: *stop=0x8c65f614u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]);goto L8c65f616; /* cmp/ge r5,r4 */
L8c65f616: take=!(*sr&1u);*stop=0x8c65f618u;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[13],1);if(io_status)return io_status;if(take)goto L8c65f60a;goto L8c65f61a; /* bf/s 0x8c65f60a */
L8c65f61a: *stop=0x8c65f61au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f664u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f61c; /* mov.l 0x8c65f664,r2 */
L8c65f61c: *stop=0x8c65f61cu;r[1]=r[15];goto L8c65f61e; /* mov r15,r1 */
L8c65f61e: *stop=0x8c65f61eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f668u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f620; /* mov.l 0x8c65f668,r3 */
L8c65f620: *stop=0x8c65f620u;r[4]=r[15];goto L8c65f622; /* mov r15,r4 */
L8c65f622: *stop=0x8c65f622u;r[4]+=4u;goto L8c65f624; /* add #0x4,r4 */
L8c65f624: *stop=0x8c65f624u;r[1]+=4u;goto L8c65f626; /* add #0x4,r1 */
L8c65f626: target=r[3];*pr=0x8c65f62au;*stop=0x8c65f628u;r[0]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f62a; /* jsr @r3 */
L8c65f62a: *stop=0x8c65f62au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f66cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f62c; /* mov.l 0x8c65f66c,r0 */
L8c65f62c: *stop=0x8c65f62cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f62e; /* mov.l @r0,r1 */
L8c65f62e: *stop=0x8c65f62eu;r[3]=1u;goto L8c65f630; /* mov #0x1,r3 */
L8c65f630: *stop=0x8c65f630u;r[1]=r[1]&255u;goto L8c65f632; /* extu.b r1,r1 */
L8c65f632: *stop=0x8c65f632u;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[3]);goto L8c65f634; /* cmp/gt r3,r1 */
L8c65f634: take=(*sr&1u);*stop=0x8c65f636u;r[11]=r[13];if(take)goto L8c65f670;goto L8c65f638; /* bt/s 0x8c65f670 */
L8c65f638: *stop=0x8c65f63au;r[9]=7u;goto L8c65f672; /* bra 0x8c65f672 */
L8c65f670: *stop=0x8c65f670u;r[9]=15u;goto L8c65f672; /* mov #0xf,r9 */
L8c65f672: *stop=0x8c65f672u;r[5]=r[4];goto L8c65f674; /* mov r4,r5 */
L8c65f674: *stop=0x8c65f674u;r[4]+=4u;goto L8c65f676; /* add #0x4,r4 */
L8c65f676: *stop=0x8c65f676u;r[6]=5u;goto L8c65f678; /* mov #0x5,r6 */
L8c65f678: *stop=0x8c65f678u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c65f67a; /* mov.l r4,@r15 */
L8c65f67a: *stop=0x8c65f67au;r[7]=r[13];goto L8c65f67c; /* mov r13,r7 */
L8c65f67c: *stop=0x8c65f67cu;r[8]=82u;goto L8c65f67e; /* mov #0x52,r8 */
L8c65f67e: *stop=0x8c65f67eu;r[10]=69u;goto L8c65f680; /* mov #0x45,r10 */
L8c65f680: *stop=0x8c65f680u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[3]=loaded;}goto L8c65f682; /* mov.b @r5,r3 */
L8c65f682: *stop=0x8c65f682u;r[2]=(uint32_t)(int32_t)(int8_t)r[9];goto L8c65f684; /* exts.b r9,r2 */
L8c65f684: *stop=0x8c65f684u;*sr=(*sr&~1u)|!(r[2]&r[3]);goto L8c65f686; /* tst r2,r3 */
L8c65f686: if((*sr&1u))goto L8c65f6aa;goto L8c65f688; /* bt 0x8c65f6aa */
L8c65f688: *stop=0x8c65f688u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f788u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65f68a; /* mov.w 0x8c65f788,r3 */
L8c65f68a: *stop=0x8c65f68au;r[4]=r[11];goto L8c65f68c; /* mov r11,r4 */
L8c65f68c: *stop=0x8c65f68cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f68e; /* mov.l @r14,r0 */
L8c65f68e: *stop=0x8c65f68eu;r[4]+=1u;goto L8c65f690; /* add #0x1,r4 */
L8c65f690: *stop=0x8c65f690u;r[12]=r[13];goto L8c65f692; /* mov r13,r12 */
L8c65f692: *stop=0x8c65f692u;r[3]+=r[0];goto L8c65f694; /* add r0,r3 */
L8c65f694: *stop=0x8c65f694u;r[3]+=r[7];goto L8c65f696; /* add r7,r3 */
L8c65f696: *stop=0x8c65f696u;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[8],1);if(io_status)return io_status;goto L8c65f698; /* mov.b r8,@r3 */
L8c65f698: *stop=0x8c65f698u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f788u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65f69a; /* mov.w 0x8c65f788,r3 */
L8c65f69a: *stop=0x8c65f69au;r[12]+=1u;goto L8c65f69c; /* add #0x1,r12 */
L8c65f69c: *stop=0x8c65f69cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f69e; /* mov.l @r14,r0 */
L8c65f69e: *stop=0x8c65f69eu;*sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[6]);goto L8c65f6a0; /* cmp/ge r6,r12 */
L8c65f6a0: *stop=0x8c65f6a0u;r[3]+=r[0];goto L8c65f6a2; /* add r0,r3 */
L8c65f6a2: *stop=0x8c65f6a2u;r[3]+=r[4];goto L8c65f6a4; /* add r4,r3 */
L8c65f6a4: *stop=0x8c65f6a4u;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[10],1);if(io_status)return io_status;goto L8c65f6a6; /* mov.b r10,@r3 */
L8c65f6a6: take=!(*sr&1u);*stop=0x8c65f6a8u;r[4]+=1u;if(take)goto L8c65f698;goto L8c65f6aa; /* bf/s 0x8c65f698 */
L8c65f6aa: *stop=0x8c65f6aau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6ac; /* mov.l @r15,r3 */
L8c65f6ac: *stop=0x8c65f6acu;r[5]+=1u;goto L8c65f6ae; /* add #0x1,r5 */
L8c65f6ae: *stop=0x8c65f6aeu;r[11]+=6u;goto L8c65f6b0; /* add #0x6,r11 */
L8c65f6b0: *stop=0x8c65f6b0u;*sr=(*sr&~1u)|(r[5]>=r[3]);goto L8c65f6b2; /* cmp/hs r3,r5 */
L8c65f6b2: take=!(*sr&1u);*stop=0x8c65f6b4u;r[7]+=6u;if(take)goto L8c65f680;goto L8c65f6b6; /* bf/s 0x8c65f680 */
L8c65f6b6: *stop=0x8c65f6b6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f78au,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f6b8; /* mov.w 0x8c65f78a,r0 */
L8c65f6b8: *stop=0x8c65f6b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f798u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6ba; /* mov.l 0x8c65f798,r3 */
L8c65f6ba: *stop=0x8c65f6bau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f6bc; /* mov.l @r14,r1 */
L8c65f6bc: *stop=0x8c65f6bcu;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[3],4);if(io_status)return io_status;goto L8c65f6be; /* mov.l r3,@(r0,r1) */
L8c65f6be: *stop=0x8c65f6beu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f78cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f6c0; /* mov.w 0x8c65f78c,r0 */
L8c65f6c0: *stop=0x8c65f6c0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f6c2; /* mov.l @r14,r2 */
L8c65f6c2: *stop=0x8c65f6c2u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f6c4; /* mov.l r13,@(r0,r2) */
L8c65f6c4: *stop=0x8c65f6c4u;r[0]+=4294967292u;goto L8c65f6c6; /* add #-0x4,r0 */
L8c65f6c6: *stop=0x8c65f6c6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6c8; /* mov.l @r14,r3 */
L8c65f6c8: *stop=0x8c65f6c8u;r[2]=1u;goto L8c65f6ca; /* mov #0x1,r2 */
L8c65f6ca: *stop=0x8c65f6cau;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f6cc; /* mov.l r13,@(r0,r3) */
L8c65f6cc: *stop=0x8c65f6ccu;r[0]+=8u;goto L8c65f6ce; /* add #0x8,r0 */
L8c65f6ce: *stop=0x8c65f6ceu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6d0; /* mov.l @r14,r3 */
L8c65f6d0: *stop=0x8c65f6d0u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f6d2; /* mov.l r2,@(r0,r3) */
L8c65f6d2: *stop=0x8c65f6d2u;r[2]=10u;goto L8c65f6d4; /* mov #0xa,r2 */
L8c65f6d4: *stop=0x8c65f6d4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f78eu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f6d6; /* mov.w 0x8c65f78e,r0 */
L8c65f6d6: *stop=0x8c65f6d6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6d8; /* mov.l @r14,r3 */
L8c65f6d8: *stop=0x8c65f6d8u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f6da; /* mov.l r13,@(r0,r3) */
L8c65f6da: *stop=0x8c65f6dau;r[0]+=36u;goto L8c65f6dc; /* add #0x24,r0 */
L8c65f6dc: *stop=0x8c65f6dcu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6de; /* mov.l @r14,r3 */
L8c65f6de: *stop=0x8c65f6deu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f6e0; /* mov.l r13,@(r0,r3) */
L8c65f6e0: *stop=0x8c65f6e0u;r[0]+=4294967292u;goto L8c65f6e2; /* add #-0x4,r0 */
L8c65f6e2: *stop=0x8c65f6e2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6e4; /* mov.l @r14,r3 */
L8c65f6e4: *stop=0x8c65f6e4u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f6e6; /* mov.l r13,@(r0,r3) */
L8c65f6e6: *stop=0x8c65f6e6u;r[0]+=4294967268u;goto L8c65f6e8; /* add #-0x1c,r0 */
L8c65f6e8: *stop=0x8c65f6e8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6ea; /* mov.l @r14,r3 */
L8c65f6ea: *stop=0x8c65f6eau;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[6],4);if(io_status)return io_status;goto L8c65f6ec; /* mov.l r6,@(r0,r3) */
L8c65f6ec: *stop=0x8c65f6ecu;r[0]+=4u;goto L8c65f6ee; /* add #0x4,r0 */
L8c65f6ee: *stop=0x8c65f6eeu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6f0; /* mov.l @r14,r3 */
L8c65f6f0: *stop=0x8c65f6f0u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f6f2; /* mov.l r13,@(r0,r3) */
L8c65f6f2: *stop=0x8c65f6f2u;r[0]+=8u;goto L8c65f6f4; /* add #0x8,r0 */
L8c65f6f4: *stop=0x8c65f6f4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6f6; /* mov.l @r14,r3 */
L8c65f6f6: *stop=0x8c65f6f6u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f6f8; /* mov.l r2,@(r0,r3) */
L8c65f6f8: *stop=0x8c65f6f8u;r[0]+=4u;goto L8c65f6fa; /* add #0x4,r0 */
L8c65f6fa: *stop=0x8c65f6fau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f6fc; /* mov.l @r14,r3 */
L8c65f6fc: *stop=0x8c65f6fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f79cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f6fe; /* mov.l 0x8c65f79c,r2 */
L8c65f6fe: *stop=0x8c65f6feu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[6],4);if(io_status)return io_status;goto L8c65f700; /* mov.l r6,@(r0,r3) */
L8c65f700: *stop=0x8c65f700u;r[0]+=4u;goto L8c65f702; /* add #0x4,r0 */
L8c65f702: *stop=0x8c65f702u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f704; /* mov.l @r14,r3 */
L8c65f704: *stop=0x8c65f704u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f706; /* mov.l r13,@(r0,r3) */
L8c65f706: *stop=0x8c65f706u;r[0]+=4u;goto L8c65f708; /* add #0x4,r0 */
L8c65f708: *stop=0x8c65f708u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f70a; /* mov.l @r14,r3 */
L8c65f70a: *stop=0x8c65f70au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f70c; /* mov.l r13,@(r0,r3) */
L8c65f70c: *stop=0x8c65f70cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f790u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f70e; /* mov.w 0x8c65f790,r0 */
L8c65f70e: *stop=0x8c65f70eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f710; /* mov.l @r14,r3 */
L8c65f710: *stop=0x8c65f710u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f712; /* mov.l r13,@(r0,r3) */
L8c65f712: *stop=0x8c65f712u;r[0]+=4u;goto L8c65f714; /* add #0x4,r0 */
L8c65f714: *stop=0x8c65f714u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f716; /* mov.l @r14,r3 */
L8c65f716: *stop=0x8c65f716u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f718; /* mov.l r13,@(r0,r3) */
L8c65f718: *stop=0x8c65f718u;r[0]+=12u;goto L8c65f71a; /* add #0xc,r0 */
L8c65f71a: *stop=0x8c65f71au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f71c; /* mov.l @r14,r3 */
L8c65f71c: *stop=0x8c65f71cu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c65f71e; /* mov.l r13,@(r0,r3) */
L8c65f71e: *stop=0x8c65f71eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f720; /* mov.l @r14,r3 */
L8c65f720: *stop=0x8c65f720u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f792u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f722; /* mov.w 0x8c65f792,r0 */
L8c65f722: *stop=0x8c65f722u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c65f724; /* mov.l r2,@r3 */
L8c65f724: *stop=0x8c65f724u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f726; /* mov.l 0x8c65f7a0,r2 */
L8c65f726: *stop=0x8c65f726u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f728; /* mov.l @r14,r3 */
L8c65f728: *stop=0x8c65f728u;if(!valid_access(r[3]+36u,4u))return -3;wr(m,bus,&io_status,r[3]+0x24u,r[2],4);if(io_status)return io_status;goto L8c65f72a; /* mov.l r2,@(0x24,r3) */
L8c65f72a: *stop=0x8c65f72au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f72c; /* mov.l 0x8c65f7a4,r2 */
L8c65f72c: *stop=0x8c65f72cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f72e; /* mov.l @r14,r3 */
L8c65f72e: *stop=0x8c65f72eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f730; /* mov.l r2,@(r0,r3) */
L8c65f730: *stop=0x8c65f730u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f794u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f732; /* mov.w 0x8c65f794,r0 */
L8c65f732: *stop=0x8c65f732u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f734; /* mov.l 0x8c65f7a8,r2 */
L8c65f734: *stop=0x8c65f734u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f736; /* mov.l @r14,r3 */
L8c65f736: *stop=0x8c65f736u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f738; /* mov.l r2,@(r0,r3) */
L8c65f738: *stop=0x8c65f738u;r[0]+=28u;goto L8c65f73a; /* add #0x1c,r0 */
L8c65f73a: *stop=0x8c65f73au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f73c; /* mov.l 0x8c65f7ac,r2 */
L8c65f73c: *stop=0x8c65f73cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f73e; /* mov.l @r14,r3 */
L8c65f73e: *stop=0x8c65f73eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f740; /* mov.l r2,@(r0,r3) */
L8c65f740: *stop=0x8c65f740u;r[0]+=60u;goto L8c65f742; /* add #0x3c,r0 */
L8c65f742: *stop=0x8c65f742u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f744; /* mov.l 0x8c65f7b0,r2 */
L8c65f744: *stop=0x8c65f744u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f746; /* mov.l @r14,r3 */
L8c65f746: *stop=0x8c65f746u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f748; /* mov.l r2,@(r0,r3) */
L8c65f748: *stop=0x8c65f748u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f796u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f74a; /* mov.w 0x8c65f796,r0 */
L8c65f74a: *stop=0x8c65f74au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f74c; /* mov.l 0x8c65f7b4,r2 */
L8c65f74c: *stop=0x8c65f74cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f74e; /* mov.l @r14,r3 */
L8c65f74e: *stop=0x8c65f74eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f750; /* mov.l r2,@(r0,r3) */
L8c65f750: *stop=0x8c65f750u;r[0]+=24u;goto L8c65f752; /* add #0x18,r0 */
L8c65f752: *stop=0x8c65f752u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f754; /* mov.l 0x8c65f7b8,r2 */
L8c65f754: *stop=0x8c65f754u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f756; /* mov.l @r14,r3 */
L8c65f756: *stop=0x8c65f756u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f758; /* mov.l r2,@(r0,r3) */
L8c65f758: *stop=0x8c65f758u;r[0]+=28u;goto L8c65f75a; /* add #0x1c,r0 */
L8c65f75a: *stop=0x8c65f75au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f75c; /* mov.l 0x8c65f7bc,r2 */
L8c65f75c: *stop=0x8c65f75cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f75e; /* mov.l @r14,r3 */
L8c65f75e: *stop=0x8c65f75eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f760; /* mov.l r2,@(r0,r3) */
L8c65f760: *stop=0x8c65f760u;r[0]+=8u;goto L8c65f762; /* add #0x8,r0 */
L8c65f762: *stop=0x8c65f762u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7c0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f764; /* mov.l 0x8c65f7c0,r2 */
L8c65f764: *stop=0x8c65f764u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f766; /* mov.l @r14,r3 */
L8c65f766: *stop=0x8c65f766u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f768; /* mov.l r2,@(r0,r3) */
L8c65f768: *stop=0x8c65f768u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f76a; /* mov.l 0x8c65f7c4,r3 */
L8c65f76a: target=r[3];*pr=0x8c65f76eu;*stop=0x8c65f76cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f76e; /* jsr @r3 */
L8c65f76e: *stop=0x8c65f76eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f7c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f770; /* mov.l 0x8c65f7c8,r2 */
L8c65f770: target=r[2];*pr=0x8c65f774u;*stop=0x8c65f772u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f774; /* jsr @r2 */
L8c65f774: *stop=0x8c65f774u;r[15]+=8u;goto L8c65f776; /* add #0x8,r15 */
L8c65f776: *stop=0x8c65f776u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f778; /* lds.l @r15+,PR */
L8c65f778: *stop=0x8c65f778u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65f77a; /* mov.l @r15+,r8 */
L8c65f77a: *stop=0x8c65f77au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65f77c; /* mov.l @r15+,r9 */
L8c65f77c: *stop=0x8c65f77cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65f77e; /* mov.l @r15+,r10 */
L8c65f77e: *stop=0x8c65f77eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65f780; /* mov.l @r15+,r11 */
L8c65f780: *stop=0x8c65f780u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65f782; /* mov.l @r15+,r12 */
L8c65f782: *stop=0x8c65f782u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65f784; /* mov.l @r15+,r13 */
L8c65f784: *stop=0x8c65f784u;if(*pr!=entry_pr)return -3;*stop=0x8c65f786u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_maple_8c6607a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6607a8;
L8c6607a8: *stop=0x8c6607a8u;*sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c6607aa; /* cmp/pl r6 */
L8c6607aa: *stop=0x8c6607aau;r[7]=0u;goto L8c6607ac; /* mov #0x0,r7 */
L8c6607ac: take=!(*sr&1u);*stop=0x8c6607aeu;r[0]=r[4];if(take)goto L8c6607ba;goto L8c6607b0; /* bf/s 0x8c6607ba */
L8c6607b0: *stop=0x8c6607b0u;r[7]+=1u;goto L8c6607b2; /* add #0x1,r7 */
L8c6607b2: *stop=0x8c6607b2u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[5],4);if(io_status)return io_status;goto L8c6607b4; /* mov.l r5,@r0 */
L8c6607b4: *stop=0x8c6607b4u;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[6]);goto L8c6607b6; /* cmp/ge r6,r7 */
L8c6607b6: take=!(*sr&1u);*stop=0x8c6607b8u;r[0]+=4u;if(take)goto L8c6607b0;goto L8c6607ba; /* bf/s 0x8c6607b0 */
L8c6607ba: *stop=0x8c6607bau;if(*pr!=entry_pr)return -3;*stop=0x8c6607bcu;r[0]=r[6];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_maple_8c6602d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6602d2;
L8c6602d2: *stop=0x8c6602d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6602d4; /* mov.l r14,@-r15 */
L8c6602d4: *stop=0x8c6602d4u;r[6]=30u;goto L8c6602d6; /* mov #0x1e,r6 */
L8c6602d6: *stop=0x8c6602d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6602d8; /* sts.l PR,@-r15 */
L8c6602d8: *stop=0x8c6602d8u;r[14]=r[4];goto L8c6602da; /* mov r4,r14 */
L8c6602da: target=0x8c6607a8u;*pr=0x8c6602deu;*stop=0x8c6602dcu;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6602de; /* bsr 0x8c6607a8 */
L8c6602de: *stop=0x8c6602deu;r[2]=r[14];goto L8c6602e0; /* mov r14,r2 */
L8c6602e0: *stop=0x8c6602e0u;r[4]=110u;goto L8c6602e2; /* mov #0x6e,r4 */
L8c6602e2: *stop=0x8c6602e2u;r[0]=21u;goto L8c6602e4; /* mov #0x15,r0 */
L8c6602e4: *stop=0x8c6602e4u;r[2]+=20u;goto L8c6602e6; /* add #0x14,r2 */
L8c6602e6: *stop=0x8c6602e6u;r[3]=40u;goto L8c6602e8; /* mov #0x28,r3 */
L8c6602e8: *stop=0x8c6602e8u;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[3],1);if(io_status)return io_status;goto L8c6602ea; /* mov.b r3,@r2 */
L8c6602ea: *stop=0x8c6602eau;r[3]=111u;goto L8c6602ec; /* mov #0x6f,r3 */
L8c6602ec: *stop=0x8c6602ecu;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],1);if(io_status)return io_status;goto L8c6602ee; /* mov.b r4,@(r0,r14) */
L8c6602ee: *stop=0x8c6602eeu;r[0]=22u;goto L8c6602f0; /* mov #0x16,r0 */
L8c6602f0: *stop=0x8c6602f0u;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],1);if(io_status)return io_status;goto L8c6602f2; /* mov.b r3,@(r0,r14) */
L8c6602f2: *stop=0x8c6602f2u;r[0]=23u;goto L8c6602f4; /* mov #0x17,r0 */
L8c6602f4: *stop=0x8c6602f4u;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],1);if(io_status)return io_status;goto L8c6602f6; /* mov.b r4,@(r0,r14) */
L8c6602f6: *stop=0x8c6602f6u;r[0]=24u;goto L8c6602f8; /* mov #0x18,r0 */
L8c6602f8: *stop=0x8c6602f8u;r[3]=101u;goto L8c6602fa; /* mov #0x65,r3 */
L8c6602fa: *stop=0x8c6602fau;r[4]=r[14];goto L8c6602fc; /* mov r14,r4 */
L8c6602fc: *stop=0x8c6602fcu;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],1);if(io_status)return io_status;goto L8c6602fe; /* mov.b r3,@(r0,r14) */
L8c6602fe: *stop=0x8c6602feu;r[0]=25u;goto L8c660300; /* mov #0x19,r0 */
L8c660300: *stop=0x8c660300u;r[2]=41u;goto L8c660302; /* mov #0x29,r2 */
L8c660302: *stop=0x8c660302u;r[4]+=26u;goto L8c660304; /* add #0x1a,r4 */
L8c660304: *stop=0x8c660304u;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],1);if(io_status)return io_status;goto L8c660306; /* mov.b r2,@(r0,r14) */
L8c660306: *stop=0x8c660306u;r[5]=32u;goto L8c660308; /* mov #0x20,r5 */
L8c660308: *stop=0x8c660308u;r[3]=r[14];goto L8c66030a; /* mov r14,r3 */
L8c66030a: *stop=0x8c66030au;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[5],1);if(io_status)return io_status;goto L8c66030c; /* mov.b r5,@r4 */
L8c66030c: *stop=0x8c66030cu;r[3]+=51u;goto L8c66030e; /* add #0x33,r3 */
L8c66030e: *stop=0x8c66030eu;r[4]+=1u;goto L8c660310; /* add #0x1,r4 */
L8c660310: *stop=0x8c660310u;*sr=(*sr&~1u)|(r[4]>=r[3]);goto L8c660312; /* cmp/hs r3,r4 */
L8c660312: if(!(*sr&1u))goto L8c660308;goto L8c660314; /* bf 0x8c660308 */
L8c660314: *stop=0x8c660314u;r[0]=51u;goto L8c660316; /* mov #0x33,r0 */
L8c660316: *stop=0x8c660316u;r[3]=0u;goto L8c660318; /* mov #0x0,r3 */
L8c660318: *stop=0x8c660318u;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],1);if(io_status)return io_status;goto L8c66031a; /* mov.b r3,@(r0,r14) */
L8c66031a: *stop=0x8c66031au;r[0]=r[3];goto L8c66031c; /* mov r3,r0 */
L8c66031c: *stop=0x8c66031cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66031e; /* lds.l @r15+,PR */
L8c66031e: *stop=0x8c66031eu;if(*pr!=entry_pr)return -3;*stop=0x8c660320u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_maple_8c10fb5c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10fb5c;
L8c10fb5c: *stop=0x8c10fb5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10fb5e; /* mov.l r2,@-r15 */
L8c10fb5e: *stop=0x8c10fb5eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10fb60; /* mov.l r3,@-r15 */
L8c10fb60: *stop=0x8c10fb60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c10fb62; /* mov.l r4,@-r15 */
L8c10fb62: *stop=0x8c10fb62u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10fb64; /* cmp/eq #0x0,r0 */
L8c10fb64: if((*sr&1u))goto L8c10fb8c;goto L8c10fb66; /* bt 0x8c10fb8c */
L8c10fb66: *stop=0x8c10fb66u;r[4]=r[2];goto L8c10fb68; /* mov r2,r4 */
L8c10fb68: *stop=0x8c10fb68u;r[4]+=r[0];goto L8c10fb6a; /* add r0,r4 */
L8c10fb6a: *stop=0x8c10fb6au;if(!valid_access(r[2],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;r[0]=loaded;}r[2]+=1;goto L8c10fb6c; /* mov.b @r2+,r0 */
L8c10fb6c: *stop=0x8c10fb6cu;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[0],1);if(io_status)return io_status;goto L8c10fb6e; /* mov.b r0,@r1 */
L8c10fb6e: *stop=0x8c10fb6eu;*sr=(*sr&~1u)|(r[4]>r[2]);goto L8c10fb70; /* cmp/hi r2,r4 */
L8c10fb70: if(!(*sr&1u))goto L8c10fb8c;goto L8c10fb72; /* bf 0x8c10fb8c */
L8c10fb72: *stop=0x8c10fb72u;if(!valid_access(r[2],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;r[0]=loaded;}r[2]+=1;goto L8c10fb74; /* mov.b @r2+,r0 */
L8c10fb74: *stop=0x8c10fb74u;if(!valid_access(r[1]+1u,1u))return -3;wr(m,bus,&io_status,r[1]+0x1u,r[0],1);if(io_status)return io_status;goto L8c10fb76; /* mov.b r0,@(0x1,r1) */
L8c10fb76: *stop=0x8c10fb76u;*sr=(*sr&~1u)|(r[4]>r[2]);goto L8c10fb78; /* cmp/hi r2,r4 */
L8c10fb78: if(!(*sr&1u))goto L8c10fb8c;goto L8c10fb7a; /* bf 0x8c10fb8c */
L8c10fb7a: *stop=0x8c10fb7au;if(!valid_access(r[2],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;r[0]=loaded;}r[2]+=1;goto L8c10fb7c; /* mov.b @r2+,r0 */
L8c10fb7c: *stop=0x8c10fb7cu;if(!valid_access(r[1]+2u,1u))return -3;wr(m,bus,&io_status,r[1]+0x2u,r[0],1);if(io_status)return io_status;goto L8c10fb7e; /* mov.b r0,@(0x2,r1) */
L8c10fb7e: *stop=0x8c10fb7eu;*sr=(*sr&~1u)|(r[4]>r[2]);goto L8c10fb80; /* cmp/hi r2,r4 */
L8c10fb80: if(!(*sr&1u))goto L8c10fb8c;goto L8c10fb82; /* bf 0x8c10fb8c */
L8c10fb82: *stop=0x8c10fb82u;if(!valid_access(r[2],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;r[0]=loaded;}r[2]+=1;goto L8c10fb84; /* mov.b @r2+,r0 */
L8c10fb84: *stop=0x8c10fb84u;if(!valid_access(r[1]+3u,1u))return -3;wr(m,bus,&io_status,r[1]+0x3u,r[0],1);if(io_status)return io_status;goto L8c10fb86; /* mov.b r0,@(0x3,r1) */
L8c10fb86: *stop=0x8c10fb86u;*sr=(*sr&~1u)|(r[4]>r[2]);goto L8c10fb88; /* cmp/hi r2,r4 */
L8c10fb88: *stop=0x8c10fb88u;r[1]+=4u;goto L8c10fb8a; /* add #0x4,r1 */
L8c10fb8a: if((*sr&1u))goto L8c10fb6a;goto L8c10fb8c; /* bt 0x8c10fb6a */
L8c10fb8c: *stop=0x8c10fb8cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c10fb8e; /* mov.l @r15+,r4 */
L8c10fb8e: *stop=0x8c10fb8eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c10fb90; /* mov.l @r15+,r3 */
L8c10fb90: *stop=0x8c10fb90u;if(*pr!=entry_pr)return -3;*stop=0x8c10fb92u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_maple_8c65f44a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f44a;
L8c65f44a: *stop=0x8c65f44au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f530u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f44c; /* mov.l 0x8c65f530,r2 */
L8c65f44c: *stop=0x8c65f44cu;r[1]=1u;goto L8c65f44e; /* mov #0x1,r1 */
L8c65f44e: *stop=0x8c65f44eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f518u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f450; /* mov.w 0x8c65f518,r0 */
L8c65f450: *stop=0x8c65f450u;r[4]&=r[1];goto L8c65f452; /* and r1,r4 */
L8c65f452: *stop=0x8c65f452u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f454; /* mov.l @r2,r3 */
L8c65f454: *stop=0x8c65f454u;if(*pr!=entry_pr)return -3;*stop=0x8c65f456u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[4],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_maple_8c6607c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6607c8;
L8c6607c8: *stop=0x8c6607c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6607e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6607ca; /* mov.l 0x8c6607e4,r2 */
L8c6607ca: *stop=0x8c6607cau;{uint32_t loaded=sx(m,bus,&io_status,0x8c6607d6u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6607cc; /* mov.w 0x8c6607d6,r0 */
L8c6607cc: *stop=0x8c6607ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6607f0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6607ce; /* mov.l 0x8c6607f0,r1 */
L8c6607ce: *stop=0x8c6607ceu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6607d0; /* mov.l @r2,r3 */
L8c6607d0: *stop=0x8c6607d0u;if(*pr!=entry_pr)return -3;*stop=0x8c6607d2u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[1],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_maple_8c6603b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6603b0;
L8c6603b0: *stop=0x8c6603b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6603b2; /* sts.l PR,@-r15 */
L8c6603b2: *stop=0x8c6603b2u;r[3]=0u;goto L8c6603b4; /* mov #0x0,r3 */
L8c6603b4: *stop=0x8c6603b4u;r[6]=r[3];goto L8c6603b6; /* mov r3,r6 */
L8c6603b6: *stop=0x8c6603b6u;r[7]=r[3];goto L8c6603b8; /* mov r3,r7 */
L8c6603b8: *stop=0x8c6603b8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6603ba; /* mov.l r3,@-r15 */
L8c6603ba: *stop=0x8c6603bau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6603bc; /* mov.l r3,@-r15 */
L8c6603bc: target=0x8c65f810u;*pr=0x8c6603c0u;*stop=0x8c6603beu;r[5]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6603c0; /* bsr 0x8c65f810 */
L8c6603c0: *stop=0x8c6603c0u;r[15]+=8u;goto L8c6603c2; /* add #0x8,r15 */
L8c6603c2: *stop=0x8c6603c2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6603c4; /* lds.l @r15+,PR */
L8c6603c4: *stop=0x8c6603c4u;if(*pr!=entry_pr)return -3;*stop=0x8c6603c6u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_maple_8c660794(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c660794;
L8c660794: *stop=0x8c660794u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6607e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660796; /* mov.l 0x8c6607e4,r3 */
L8c660796: *stop=0x8c660796u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6607d4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c660798; /* mov.w 0x8c6607d4,r0 */
L8c660798: *stop=0x8c660798u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c66079a; /* mov.l @r3,r4 */
L8c66079a: *stop=0x8c66079au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6607e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66079c; /* mov.l 0x8c6607e8,r2 */
L8c66079c: *stop=0x8c66079cu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c66079e; /* mov.l @(r0,r4),r4 */
L8c66079e: *stop=0x8c66079eu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6607a0; /* mov.l @r4,r1 */
L8c6607a0: *stop=0x8c6607a0u;r[1]|=r[2];goto L8c6607a2; /* or r2,r1 */
L8c6607a2: *stop=0x8c6607a2u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c6607a4; /* mov.l r1,@r4 */
L8c6607a4: *stop=0x8c6607a4u;if(*pr!=entry_pr)return -3;*stop=0x8c6607a6u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_maple_8c65f810(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65f810;
L8c65f810: *stop=0x8c65f810u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65f812; /* mov.l r14,@-r15 */
L8c65f812: *stop=0x8c65f812u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65f814; /* mov.l r13,@-r15 */
L8c65f814: *stop=0x8c65f814u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65f816; /* mov.l r12,@-r15 */
L8c65f816: *stop=0x8c65f816u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65f818; /* mov.l r11,@-r15 */
L8c65f818: *stop=0x8c65f818u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65f81a; /* mov.l r10,@-r15 */
L8c65f81a: *stop=0x8c65f81au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65f81c; /* mov.l r9,@-r15 */
L8c65f81c: *stop=0x8c65f81cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65f81e; /* mov.l r8,@-r15 */
L8c65f81e: *stop=0x8c65f81eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65f820; /* sts.l PR,@-r15 */
L8c65f820: *stop=0x8c65f820u;r[15]+=4294967268u;goto L8c65f822; /* add #-0x1c,r15 */
L8c65f822: *stop=0x8c65f822u;r[10]=44u;goto L8c65f824; /* mov #0x2c,r10 */
L8c65f824: *stop=0x8c65f824u;r[9]=r[4];goto L8c65f826; /* mov r4,r9 */
L8c65f826: *stop=0x8c65f826u;*macl=r[10]*r[9];goto L8c65f828; /* mul.l r10,r9 */
L8c65f828: *stop=0x8c65f828u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f850u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65f82a; /* mov.l 0x8c65f850,r14 */
L8c65f82a: *stop=0x8c65f82au;r[0]=r[5];goto L8c65f82c; /* mov r5,r0 */
L8c65f82c: *stop=0x8c65f82cu;if(!valid_access(r[15]+4u,1u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],1);if(io_status)return io_status;goto L8c65f82e; /* mov.b r0,@(0x4,r15) */
L8c65f82e: *stop=0x8c65f82eu;r[0]=64u;goto L8c65f830; /* mov #0x40,r0 */
L8c65f830: *stop=0x8c65f830u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[6],4);if(io_status)return io_status;goto L8c65f832; /* mov.l r6,@(0x18,r15) */
L8c65f832: *stop=0x8c65f832u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f834; /* mov.l @r14,r4 */
L8c65f834: *stop=0x8c65f834u;r[10]=*macl;goto L8c65f836; /* sts MACL,r10 */
L8c65f836: *stop=0x8c65f836u;r[3]=r[4];goto L8c65f838; /* mov r4,r3 */
L8c65f838: *stop=0x8c65f838u;r[3]+=40u;goto L8c65f83a; /* add #0x28,r3 */
L8c65f83a: *stop=0x8c65f83au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[12]=loaded;}goto L8c65f83c; /* mov.l @(r0,r15),r12 */
L8c65f83c: *stop=0x8c65f83cu;r[3]+=r[10];goto L8c65f83e; /* add r10,r3 */
L8c65f83e: *stop=0x8c65f83eu;if(!valid_access(r[3]+6u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3]+0x6u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c65f840; /* mov.b @(0x6,r3),r0 */
L8c65f840: *stop=0x8c65f840u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c65f842; /* tst r0,r0 */
L8c65f842: take=(*sr&1u);*stop=0x8c65f844u;r[11]=r[7];if(take)goto L8c65f854;goto L8c65f846; /* bt/s 0x8c65f854 */
L8c65f846: *stop=0x8c65f848u;r[0]=4294967295u;goto L8c65fa66; /* bra 0x8c65fa66 */
L8c65f854: *stop=0x8c65f854u;if(!valid_access(r[15]+4u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c65f856; /* mov.b @(0x4,r15),r0 */
L8c65f856: *stop=0x8c65f856u;r[2]=r[15];goto L8c65f858; /* mov r15,r2 */
L8c65f858: *stop=0x8c65f858u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f950u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c65f85a; /* mov.w 0x8c65f950,r5 */
L8c65f85a: *stop=0x8c65f85au;r[2]+=16u;goto L8c65f85c; /* add #0x10,r2 */
L8c65f85c: *stop=0x8c65f85cu;if(!valid_access(r[3]+6u,1u))return -3;wr(m,bus,&io_status,r[3]+0x6u,r[0],1);if(io_status)return io_status;goto L8c65f85e; /* mov.b r0,@(0x6,r3) */
L8c65f85e: *stop=0x8c65f85eu;r[8]=r[11];goto L8c65f860; /* mov r11,r8 */
L8c65f860: *stop=0x8c65f860u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f960u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f862; /* mov.l 0x8c65f960,r0 */
L8c65f862: *stop=0x8c65f862u;r[8]+=r[12];goto L8c65f864; /* add r12,r8 */
L8c65f864: *stop=0x8c65f864u;r[4]=r[8];goto L8c65f866; /* mov r8,r4 */
L8c65f866: *stop=0x8c65f866u;r[4]+=3u;goto L8c65f868; /* add #0x3,r4 */
L8c65f868: *stop=0x8c65f868u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c65f86a; /* mov.l r4,@(0x8,r15) */
L8c65f86a: *stop=0x8c65f86au;r[4]=3u;goto L8c65f86c; /* mov #0x3,r4 */
L8c65f86c: *stop=0x8c65f86cu;if(!valid_access(r[0]+r[9],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[9]+r[0],1);if(io_status)return io_status;r[3]=loaded;}goto L8c65f86e; /* mov.b @(r0,r9),r3 */
L8c65f86e: *stop=0x8c65f86eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f952u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f870; /* mov.w 0x8c65f952,r0 */
L8c65f870: *stop=0x8c65f870u;r[5]&=r[3];goto L8c65f872; /* and r3,r5 */
L8c65f872: *stop=0x8c65f872u;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[3],1);if(io_status)return io_status;goto L8c65f874; /* mov.b r3,@r2 */
L8c65f874: *stop=0x8c65f874u;r[5]=r[5]&255u;goto L8c65f876; /* extu.b r5,r5 */
L8c65f876: *stop=0x8c65f876u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c65f878; /* mov.l r5,@(0xc,r15) */
L8c65f878: *stop=0x8c65f878u;r[2]=4294967290u;goto L8c65f87a; /* mov #-0x6,r2 */
L8c65f87a: *stop=0x8c65f87au;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}}goto L8c65f87c; /* shad r2,r5 */
L8c65f87c: *stop=0x8c65f87cu;r[5]&=r[4];goto L8c65f87e; /* and r4,r5 */
L8c65f87e: *stop=0x8c65f87eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c65f880; /* mov.l r5,@r15 */
L8c65f880: *stop=0x8c65f880u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[13]=loaded;}goto L8c65f882; /* mov.l @r14,r13 */
L8c65f882: *stop=0x8c65f882u;r[2]=r[13];goto L8c65f884; /* mov r13,r2 */
L8c65f884: *stop=0x8c65f884u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f886; /* mov.l @(r0,r2),r3 */
L8c65f886: *stop=0x8c65f886u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c65f888; /* cmp/pl r3 */
L8c65f888: if(!(*sr&1u))goto L8c65f8aa;goto L8c65f88a; /* bf 0x8c65f8aa */
L8c65f88a: *stop=0x8c65f88au;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f954u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f88c; /* mov.w 0x8c65f954,r0 */
L8c65f88c: *stop=0x8c65f88cu;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f88e; /* mov.l @(r0,r13),r1 */
L8c65f88e: *stop=0x8c65f88eu;*sr=(*sr&~1u)|((int32_t)r[1]>0);goto L8c65f890; /* cmp/pl r1 */
L8c65f890: if(!(*sr&1u))goto L8c65f8aa;goto L8c65f892; /* bf 0x8c65f8aa */
L8c65f892: *stop=0x8c65f892u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f956u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f894; /* mov.w 0x8c65f956,r0 */
L8c65f894: *stop=0x8c65f894u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f896; /* mov.l @(r0,r13),r2 */
L8c65f896: *stop=0x8c65f896u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f898; /* mov.l @r2,r3 */
L8c65f898: *stop=0x8c65f898u;r[3]>>=16;goto L8c65f89a; /* shlr16 r3 */
L8c65f89a: *stop=0x8c65f89au;r[3]=(uint32_t)(int32_t)(int16_t)r[3];goto L8c65f89c; /* exts.w r3,r3 */
L8c65f89c: *stop=0x8c65f89cu;r[4]&=r[3];goto L8c65f89e; /* and r3,r4 */
L8c65f89e: *stop=0x8c65f89eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f8a0; /* mov.l @r15,r3 */
L8c65f8a0: *stop=0x8c65f8a0u;*sr=(*sr&~1u)|(r[4]==r[3]);goto L8c65f8a2; /* cmp/eq r4,r3 */
L8c65f8a2: if(!(*sr&1u))goto L8c65f8aa;goto L8c65f8a4; /* bf 0x8c65f8aa */
L8c65f8a4: *stop=0x8c65f8a4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f952u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f8a6; /* mov.w 0x8c65f952,r0 */
L8c65f8a6: target=0x8c65f7ccu;*pr=0x8c65f8aau;*stop=0x8c65f8a8u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f8aa; /* bsr 0x8c65f7cc */
L8c65f8aa: *stop=0x8c65f8aau;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f956u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f8ac; /* mov.w 0x8c65f956,r0 */
L8c65f8ac: *stop=0x8c65f8acu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f8ae; /* mov.l @r14,r2 */
L8c65f8ae: *stop=0x8c65f8aeu;r[1]=r[0];goto L8c65f8b0; /* mov r0,r1 */
L8c65f8b0: *stop=0x8c65f8b0u;r[1]+=12u;goto L8c65f8b2; /* add #0xc,r1 */
L8c65f8b2: *stop=0x8c65f8b2u;r[3]=r[2];goto L8c65f8b4; /* mov r2,r3 */
L8c65f8b4: *stop=0x8c65f8b4u;r[1]+=r[3];goto L8c65f8b6; /* add r3,r1 */
L8c65f8b6: *stop=0x8c65f8b6u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f8b8; /* mov.l @r1,r1 */
L8c65f8b8: *stop=0x8c65f8b8u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f8ba; /* mov.l @(r0,r2),r3 */
L8c65f8ba: *stop=0x8c65f8bau;r[1]<<=2;goto L8c65f8bc; /* shll2 r1 */
L8c65f8bc: *stop=0x8c65f8bcu;r[3]+=r[1];goto L8c65f8be; /* add r1,r3 */
L8c65f8be: *stop=0x8c65f8beu;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[3],4);if(io_status)return io_status;goto L8c65f8c0; /* mov.l r3,@(r0,r2) */
L8c65f8c0: *stop=0x8c65f8c0u;r[1]=r[8]&255u;goto L8c65f8c2; /* extu.b r8,r1 */
L8c65f8c2: *stop=0x8c65f8c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f8c4; /* mov.l @r15,r2 */
L8c65f8c4: *stop=0x8c65f8c4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[13]=loaded;}goto L8c65f8c6; /* mov.l @r14,r13 */
L8c65f8c6: *stop=0x8c65f8c6u;r[2]<<=16;goto L8c65f8c8; /* shll16 r2 */
L8c65f8c8: *stop=0x8c65f8c8u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[13]=loaded;}goto L8c65f8ca; /* mov.l @(r0,r13),r13 */
L8c65f8ca: *stop=0x8c65f8cau;r[2]|=r[1];goto L8c65f8cc; /* or r1,r2 */
L8c65f8cc: *stop=0x8c65f8ccu;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[1],4);if(io_status)return io_status;goto L8c65f8ce; /* mov.l r1,@(0x14,r15) */
L8c65f8ce: *stop=0x8c65f8ceu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f958u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65f8d0; /* mov.w 0x8c65f958,r1 */
L8c65f8d0: *stop=0x8c65f8d0u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c65f8d2; /* mov.l r2,@r13 */
L8c65f8d2: *stop=0x8c65f8d2u;r[13]+=4u;goto L8c65f8d4; /* add #0x4,r13 */
L8c65f8d4: *stop=0x8c65f8d4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65f8d6; /* mov.l r13,@-r15 */
L8c65f8d6: *stop=0x8c65f8d6u;r[13]+=4u;goto L8c65f8d8; /* add #0x4,r13 */
L8c65f8d8: *stop=0x8c65f8d8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f8da; /* mov.l @r14,r0 */
L8c65f8da: *stop=0x8c65f8dau;r[3]=r[0];goto L8c65f8dc; /* mov r0,r3 */
L8c65f8dc: *stop=0x8c65f8dcu;r[1]+=r[3];goto L8c65f8de; /* add r3,r1 */
L8c65f8de: *stop=0x8c65f8deu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f8e0; /* mov.l @r1,r3 */
L8c65f8e0: *stop=0x8c65f8e0u;r[2]=r[3];goto L8c65f8e2; /* mov r3,r2 */
L8c65f8e2: *stop=0x8c65f8e2u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c65f8e4; /* shll r3 */
L8c65f8e4: *stop=0x8c65f8e4u;r[3]+=r[2];goto L8c65f8e6; /* add r2,r3 */
L8c65f8e6: *stop=0x8c65f8e6u;r[3]<<=2;goto L8c65f8e8; /* shll2 r3 */
L8c65f8e8: *stop=0x8c65f8e8u;r[3]<<=2;goto L8c65f8ea; /* shll2 r3 */
L8c65f8ea: *stop=0x8c65f8eau;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f95au,2);if(io_status)return io_status;r[2]=loaded;}goto L8c65f8ec; /* mov.w 0x8c65f95a,r2 */
L8c65f8ec: *stop=0x8c65f8ecu;r[4]=r[9];goto L8c65f8ee; /* mov r9,r4 */
L8c65f8ee: *stop=0x8c65f8eeu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c65f8f0; /* shll r3 */
L8c65f8f0: *stop=0x8c65f8f0u;r[0]+=r[2];goto L8c65f8f2; /* add r2,r0 */
L8c65f8f2: *stop=0x8c65f8f2u;r[4]<<=2;goto L8c65f8f4; /* shll2 r4 */
L8c65f8f4: *stop=0x8c65f8f4u;r[0]+=r[3];goto L8c65f8f6; /* add r3,r0 */
L8c65f8f6: target=0x8c6607beu;*pr=0x8c65f8fau;*stop=0x8c65f8f8u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f8fa; /* bsr 0x8c6607be */
L8c65f8fa: *stop=0x8c65f8fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;goto L8c65f8fc; /* mov.l @r15+,r2 */
L8c65f8fc: *stop=0x8c65f8fcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f95cu,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65f8fe; /* mov.w 0x8c65f95c,r1 */
L8c65f8fe: *stop=0x8c65f8feu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c65f900; /* mov.l r0,@r2 */
L8c65f900: *stop=0x8c65f900u;if(!valid_access(r[15]+4u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c65f902; /* mov.b @(0x4,r15),r0 */
L8c65f902: *stop=0x8c65f902u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f964u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65f904; /* mov.l 0x8c65f964,r6 */
L8c65f904: *stop=0x8c65f904u;r[0]=r[0]&255u;goto L8c65f906; /* extu.b r0,r0 */
L8c65f906: *stop=0x8c65f906u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c65f908; /* mov.l @(0x18,r15),r7 */
L8c65f908: *stop=0x8c65f908u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c65f90a; /* mov.l r0,@r15 */
L8c65f90a: *stop=0x8c65f90au;r[0]=16u;goto L8c65f90c; /* mov #0x10,r0 */
L8c65f90c: *stop=0x8c65f90cu;if(!valid_access(r[0]+r[15],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+r[0],1);if(io_status)return io_status;r[9]=loaded;}goto L8c65f90e; /* mov.b @(r0,r15),r9 */
L8c65f90e: *stop=0x8c65f90eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f910; /* mov.l @r14,r0 */
L8c65f910: *stop=0x8c65f910u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f96cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f912; /* mov.l 0x8c65f96c,r4 */
L8c65f912: *stop=0x8c65f912u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f914; /* mov.l @(r0,r1),r0 */
L8c65f914: *stop=0x8c65f914u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f968u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65f916; /* mov.l 0x8c65f968,r5 */
L8c65f916: *stop=0x8c65f916u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65f918; /* cmp/eq #0x1,r0 */
L8c65f918: take=!(*sr&1u);*stop=0x8c65f91au;r[9]=r[9]&255u;if(take)goto L8c65f970;goto L8c65f91c; /* bf/s 0x8c65f970 */
L8c65f91c: *stop=0x8c65f91cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f91e; /* mov.l @r15,r3 */
L8c65f91e: *stop=0x8c65f91eu;r[9]<<=16;goto L8c65f920; /* shll16 r9 */
L8c65f920: *stop=0x8c65f920u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f922; /* mov.l @(0xc,r15),r0 */
L8c65f922: *stop=0x8c65f922u;r[9]&=r[4];goto L8c65f924; /* and r4,r9 */
L8c65f924: *stop=0x8c65f924u;r[3]<<=16;goto L8c65f926; /* shll16 r3 */
L8c65f926: *stop=0x8c65f926u;*sr=(*sr&~1u)|((int32_t)r[11]>0);goto L8c65f928; /* cmp/pl r11 */
L8c65f928: *stop=0x8c65f928u;r[3]<<=8;goto L8c65f92a; /* shll8 r3 */
L8c65f92a: *stop=0x8c65f92au;r[0]<<=8;goto L8c65f92c; /* shll8 r0 */
L8c65f92c: *stop=0x8c65f92cu;r[3]&=r[5];goto L8c65f92e; /* and r5,r3 */
L8c65f92e: *stop=0x8c65f92eu;r[0]&=r[6];goto L8c65f930; /* and r6,r0 */
L8c65f930: *stop=0x8c65f930u;r[3]|=r[9];goto L8c65f932; /* or r9,r3 */
L8c65f932: *stop=0x8c65f932u;r[3]|=r[0];goto L8c65f934; /* or r0,r3 */
L8c65f934: *stop=0x8c65f934u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f936; /* mov.l @(0x14,r15),r0 */
L8c65f936: *stop=0x8c65f936u;r[3]|=r[0];goto L8c65f938; /* or r0,r3 */
L8c65f938: *stop=0x8c65f938u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c65f93a; /* mov.l r3,@r13 */
L8c65f93a: *stop=0x8c65f93au;r[13]+=4u;goto L8c65f93c; /* add #0x4,r13 */
L8c65f93c: take=!(*sr&1u);*stop=0x8c65f93eu;r[9]=0u;if(take)goto L8c65f9c4;goto L8c65f940; /* bf/s 0x8c65f9c4 */
L8c65f940: *stop=0x8c65f940u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}r[7]+=4;goto L8c65f942; /* mov.l @r7+,r3 */
L8c65f942: *stop=0x8c65f942u;r[9]+=1u;goto L8c65f944; /* add #0x1,r9 */
L8c65f944: *stop=0x8c65f944u;*sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[11]);goto L8c65f946; /* cmp/ge r11,r9 */
L8c65f946: *stop=0x8c65f946u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c65f948; /* mov.l r3,@r13 */
L8c65f948: take=!(*sr&1u);*stop=0x8c65f94au;r[13]+=4u;if(take)goto L8c65f940;goto L8c65f94c; /* bf/s 0x8c65f940 */
L8c65f94c: *stop=0x8c65f94eu;goto L8c65f9c4; /* bra 0x8c65f9c4 */
L8c65f970: *stop=0x8c65f970u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f972; /* mov.l @(0xc,r15),r3 */
L8c65f972: *stop=0x8c65f972u;r[9]<<=8;goto L8c65f974; /* shll8 r9 */
L8c65f974: *stop=0x8c65f974u;r[8]<<=16;goto L8c65f976; /* shll16 r8 */
L8c65f976: *stop=0x8c65f976u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f978; /* mov.l @r15,r0 */
L8c65f978: *stop=0x8c65f978u;r[3]<<=16;goto L8c65f97a; /* shll16 r3 */
L8c65f97a: *stop=0x8c65f97au;*sr=(*sr&~1u)|((int32_t)r[11]>0);goto L8c65f97c; /* cmp/pl r11 */
L8c65f97c: *stop=0x8c65f97cu;r[8]<<=8;goto L8c65f97e; /* shll8 r8 */
L8c65f97e: *stop=0x8c65f97eu;r[3]&=r[4];goto L8c65f980; /* and r4,r3 */
L8c65f980: *stop=0x8c65f980u;r[9]&=r[6];goto L8c65f982; /* and r6,r9 */
L8c65f982: *stop=0x8c65f982u;r[8]&=r[5];goto L8c65f984; /* and r5,r8 */
L8c65f984: *stop=0x8c65f984u;r[3]|=r[9];goto L8c65f986; /* or r9,r3 */
L8c65f986: *stop=0x8c65f986u;r[3]|=r[8];goto L8c65f988; /* or r8,r3 */
L8c65f988: *stop=0x8c65f988u;r[3]|=r[0];goto L8c65f98a; /* or r0,r3 */
L8c65f98a: *stop=0x8c65f98au;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c65f98c; /* mov.l r3,@r13 */
L8c65f98c: *stop=0x8c65f98cu;r[13]+=4u;goto L8c65f98e; /* add #0x4,r13 */
L8c65f98e: take=!(*sr&1u);*stop=0x8c65f990u;r[8]=0u;if(take)goto L8c65f9c4;goto L8c65f992; /* bf/s 0x8c65f9c4 */
L8c65f992: *stop=0x8c65f992u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[9]=loaded;}r[7]+=4;goto L8c65f994; /* mov.l @r7+,r9 */
L8c65f994: *stop=0x8c65f994u;r[8]+=1u;goto L8c65f996; /* add #0x1,r8 */
L8c65f996: *stop=0x8c65f996u;*sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[11]);goto L8c65f998; /* cmp/ge r11,r8 */
L8c65f998: *stop=0x8c65f998u;r[2]=r[9];goto L8c65f99a; /* mov r9,r2 */
L8c65f99a: *stop=0x8c65f99au;r[3]=r[9];goto L8c65f99c; /* mov r9,r3 */
L8c65f99c: *stop=0x8c65f99cu;r[2]<<=16;goto L8c65f99e; /* shll16 r2 */
L8c65f99e: *stop=0x8c65f99eu;r[1]=r[9];goto L8c65f9a0; /* mov r9,r1 */
L8c65f9a0: *stop=0x8c65f9a0u;r[3]<<=8;goto L8c65f9a2; /* shll8 r3 */
L8c65f9a2: *stop=0x8c65f9a2u;r[2]<<=8;goto L8c65f9a4; /* shll8 r2 */
L8c65f9a4: *stop=0x8c65f9a4u;r[3]&=r[4];goto L8c65f9a6; /* and r4,r3 */
L8c65f9a6: *stop=0x8c65f9a6u;r[2]&=r[5];goto L8c65f9a8; /* and r5,r2 */
L8c65f9a8: *stop=0x8c65f9a8u;r[2]|=r[3];goto L8c65f9aa; /* or r3,r2 */
L8c65f9aa: *stop=0x8c65f9aau;r[3]=4294967288u;goto L8c65f9ac; /* mov #-0x8,r3 */
L8c65f9ac: *stop=0x8c65f9acu;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}}goto L8c65f9ae; /* shad r3,r1 */
L8c65f9ae: *stop=0x8c65f9aeu;r[1]&=r[6];goto L8c65f9b0; /* and r6,r1 */
L8c65f9b0: *stop=0x8c65f9b0u;r[2]|=r[1];goto L8c65f9b2; /* or r1,r2 */
L8c65f9b2: *stop=0x8c65f9b2u;r[1]=r[9];goto L8c65f9b4; /* mov r9,r1 */
L8c65f9b4: *stop=0x8c65f9b4u;r[1]>>=16;goto L8c65f9b6; /* shlr16 r1 */
L8c65f9b6: *stop=0x8c65f9b6u;r[1]>>=8;goto L8c65f9b8; /* shlr8 r1 */
L8c65f9b8: *stop=0x8c65f9b8u;r[1]=(uint32_t)(int32_t)(int8_t)r[1];goto L8c65f9ba; /* exts.b r1,r1 */
L8c65f9ba: *stop=0x8c65f9bau;r[1]=r[1]&255u;goto L8c65f9bc; /* extu.b r1,r1 */
L8c65f9bc: *stop=0x8c65f9bcu;r[2]|=r[1];goto L8c65f9be; /* or r1,r2 */
L8c65f9be: *stop=0x8c65f9beu;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c65f9c0; /* mov.l r2,@r13 */
L8c65f9c0: take=!(*sr&1u);*stop=0x8c65f9c2u;r[13]+=4u;if(take)goto L8c65f992;goto L8c65f9c4; /* bf/s 0x8c65f992 */
L8c65f9c4: *stop=0x8c65f9c4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65fa7au,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65f9c6; /* mov.w 0x8c65fa7a,r1 */
L8c65f9c6: *stop=0x8c65f9c6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f9c8; /* mov.l @r14,r0 */
L8c65f9c8: *stop=0x8c65f9c8u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f9ca; /* mov.l @(r0,r1),r0 */
L8c65f9ca: *stop=0x8c65f9cau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65f9cc; /* cmp/eq #0x1,r0 */
L8c65f9cc: if(!(*sr&1u))goto L8c65fa06;goto L8c65f9ce; /* bf 0x8c65fa06 */
L8c65f9ce: *stop=0x8c65f9ceu;if(!valid_access(r[15]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x3cu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c65f9d0; /* mov.l @(0x3c,r15),r7 */
L8c65f9d0: *stop=0x8c65f9d0u;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c65f9d2; /* cmp/pl r12 */
L8c65f9d2: take=!(*sr&1u);*stop=0x8c65f9d4u;r[11]=0u;if(take)goto L8c65fa1a;goto L8c65f9d6; /* bf/s 0x8c65fa1a */
L8c65f9d6: *stop=0x8c65f9d6u;if(!valid_access(r[7]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c65f9d8; /* mov.b @(0x1,r7),r0 */
L8c65f9d8: *stop=0x8c65f9d8u;r[11]+=1u;goto L8c65f9da; /* add #0x1,r11 */
L8c65f9da: *stop=0x8c65f9dau;if(!valid_access(r[7],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7],1);if(io_status)return io_status;r[1]=loaded;}goto L8c65f9dc; /* mov.b @r7,r1 */
L8c65f9dc: *stop=0x8c65f9dcu;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[12]);goto L8c65f9de; /* cmp/ge r12,r11 */
L8c65f9de: *stop=0x8c65f9deu;r[0]<<=16;goto L8c65f9e0; /* shll16 r0 */
L8c65f9e0: *stop=0x8c65f9e0u;r[0]&=r[4];goto L8c65f9e2; /* and r4,r0 */
L8c65f9e2: *stop=0x8c65f9e2u;r[3]=r[0];goto L8c65f9e4; /* mov r0,r3 */
L8c65f9e4: *stop=0x8c65f9e4u;if(!valid_access(r[7]+2u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7]+0x2u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c65f9e6; /* mov.b @(0x2,r7),r0 */
L8c65f9e6: *stop=0x8c65f9e6u;r[1]<<=16;goto L8c65f9e8; /* shll16 r1 */
L8c65f9e8: *stop=0x8c65f9e8u;r[0]<<=8;goto L8c65f9ea; /* shll8 r0 */
L8c65f9ea: *stop=0x8c65f9eau;r[0]&=r[6];goto L8c65f9ec; /* and r6,r0 */
L8c65f9ec: *stop=0x8c65f9ecu;r[3]|=r[0];goto L8c65f9ee; /* or r0,r3 */
L8c65f9ee: *stop=0x8c65f9eeu;if(!valid_access(r[7]+3u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7]+0x3u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c65f9f0; /* mov.b @(0x3,r7),r0 */
L8c65f9f0: *stop=0x8c65f9f0u;r[1]<<=8;goto L8c65f9f2; /* shll8 r1 */
L8c65f9f2: *stop=0x8c65f9f2u;r[1]&=r[5];goto L8c65f9f4; /* and r5,r1 */
L8c65f9f4: *stop=0x8c65f9f4u;r[0]=r[0]&255u;goto L8c65f9f6; /* extu.b r0,r0 */
L8c65f9f6: *stop=0x8c65f9f6u;r[3]|=r[1];goto L8c65f9f8; /* or r1,r3 */
L8c65f9f8: *stop=0x8c65f9f8u;r[3]|=r[0];goto L8c65f9fa; /* or r0,r3 */
L8c65f9fa: *stop=0x8c65f9fau;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c65f9fc; /* mov.l r3,@r13 */
L8c65f9fc: *stop=0x8c65f9fcu;r[13]+=4u;goto L8c65f9fe; /* add #0x4,r13 */
L8c65f9fe: take=!(*sr&1u);*stop=0x8c65fa00u;r[7]+=4u;if(take)goto L8c65f9d6;goto L8c65fa02; /* bf/s 0x8c65f9d6 */
L8c65fa02: *stop=0x8c65fa04u;goto L8c65fa1a; /* bra 0x8c65fa1a */
L8c65fa06: *stop=0x8c65fa06u;if(!valid_access(r[15]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x3cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65fa08; /* mov.l @(0x3c,r15),r5 */
L8c65fa08: *stop=0x8c65fa08u;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c65fa0a; /* cmp/pl r12 */
L8c65fa0a: take=!(*sr&1u);*stop=0x8c65fa0cu;r[4]=0u;if(take)goto L8c65fa1a;goto L8c65fa0e; /* bf/s 0x8c65fa1a */
L8c65fa0e: *stop=0x8c65fa0eu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}r[5]+=4;goto L8c65fa10; /* mov.l @r5+,r3 */
L8c65fa10: *stop=0x8c65fa10u;r[4]+=1u;goto L8c65fa12; /* add #0x1,r4 */
L8c65fa12: *stop=0x8c65fa12u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[12]);goto L8c65fa14; /* cmp/ge r12,r4 */
L8c65fa14: *stop=0x8c65fa14u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c65fa16; /* mov.l r3,@r13 */
L8c65fa16: take=!(*sr&1u);*stop=0x8c65fa18u;r[13]+=4u;if(take)goto L8c65fa0e;goto L8c65fa1a; /* bf/s 0x8c65fa0e */
L8c65fa1a: *stop=0x8c65fa1au;{uint32_t loaded=sx(m,bus,&io_status,0x8c65fa7cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65fa1c; /* mov.w 0x8c65fa7c,r0 */
L8c65fa1c: *stop=0x8c65fa1cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65fa1e; /* mov.l @(0x8,r15),r2 */
L8c65fa1e: *stop=0x8c65fa1eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65fa20; /* mov.l @r14,r3 */
L8c65fa20: *stop=0x8c65fa20u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65fa22; /* mov.l r2,@(r0,r3) */
L8c65fa22: *stop=0x8c65fa22u;r[0]+=4u;goto L8c65fa24; /* add #0x4,r0 */
L8c65fa24: *stop=0x8c65fa24u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65fa26; /* mov.l @r14,r3 */
L8c65fa26: *stop=0x8c65fa26u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65fa28; /* mov.l @(0x8,r15),r2 */
L8c65fa28: *stop=0x8c65fa28u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65fa2a; /* mov.l @(r0,r3),r1 */
L8c65fa2a: *stop=0x8c65fa2au;r[2]<<=2;goto L8c65fa2c; /* shll2 r2 */
L8c65fa2c: *stop=0x8c65fa2cu;r[1]+=r[2];goto L8c65fa2e; /* add r2,r1 */
L8c65fa2e: *stop=0x8c65fa2eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[1],4);if(io_status)return io_status;goto L8c65fa30; /* mov.l r1,@(r0,r3) */
L8c65fa30: *stop=0x8c65fa30u;r[0]+=4u;goto L8c65fa32; /* add #0x4,r0 */
L8c65fa32: *stop=0x8c65fa32u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65fa34; /* mov.l @r14,r3 */
L8c65fa34: *stop=0x8c65fa34u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65fa36; /* mov.l @r14,r2 */
L8c65fa36: *stop=0x8c65fa36u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65fa38; /* mov.l @(r0,r3),r1 */
L8c65fa38: *stop=0x8c65fa38u;r[2]+=40u;goto L8c65fa3a; /* add #0x28,r2 */
L8c65fa3a: *stop=0x8c65fa3au;r[1]+=1u;goto L8c65fa3c; /* add #0x1,r1 */
L8c65fa3c: *stop=0x8c65fa3cu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[1],4);if(io_status)return io_status;goto L8c65fa3e; /* mov.l r1,@(r0,r3) */
L8c65fa3e: *stop=0x8c65fa3eu;r[1]+=4294967295u;goto L8c65fa40; /* add #-0x1,r1 */
L8c65fa40: *stop=0x8c65fa40u;r[2]+=r[10];goto L8c65fa42; /* add r10,r2 */
L8c65fa42: *stop=0x8c65fa42u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[1],4);if(io_status)return io_status;goto L8c65fa44; /* mov.l r1,@(0x18,r2) */
L8c65fa44: *stop=0x8c65fa44u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65fa46; /* mov.l @r14,r3 */
L8c65fa46: *stop=0x8c65fa46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65fa80u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65fa48; /* mov.l 0x8c65fa80,r2 */
L8c65fa48: *stop=0x8c65fa48u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65fa4a; /* mov.l @(r0,r3),r1 */
L8c65fa4a: *stop=0x8c65fa4au;r[1]&=r[2];goto L8c65fa4c; /* and r2,r1 */
L8c65fa4c: *stop=0x8c65fa4cu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[1],4);if(io_status)return io_status;goto L8c65fa4e; /* mov.l r1,@(r0,r3) */
L8c65fa4e: *stop=0x8c65fa4eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65fa50; /* mov.l @r14,r3 */
L8c65fa50: *stop=0x8c65fa50u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65fa52; /* mov.l @(r0,r3),r1 */
L8c65fa52: *stop=0x8c65fa52u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c65fa54; /* tst r1,r1 */
L8c65fa54: if(!(*sr&1u))goto L8c65fa5e;goto L8c65fa56; /* bf 0x8c65fa5e */
L8c65fa56: *stop=0x8c65fa56u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65fa7eu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65fa58; /* mov.w 0x8c65fa7e,r0 */
L8c65fa58: *stop=0x8c65fa58u;r[3]=1u;goto L8c65fa5a; /* mov #0x1,r3 */
L8c65fa5a: *stop=0x8c65fa5au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65fa5c; /* mov.l @r14,r1 */
L8c65fa5c: *stop=0x8c65fa5cu;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[3],4);if(io_status)return io_status;goto L8c65fa5e; /* mov.l r3,@(r0,r1) */
L8c65fa5e: *stop=0x8c65fa5eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fa60; /* mov.l @r14,r0 */
L8c65fa60: *stop=0x8c65fa60u;r[0]+=40u;goto L8c65fa62; /* add #0x28,r0 */
L8c65fa62: *stop=0x8c65fa62u;r[0]+=r[10];goto L8c65fa64; /* add r10,r0 */
L8c65fa64: *stop=0x8c65fa64u;if(!valid_access(r[0]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fa66; /* mov.l @(0x18,r0),r0 */
L8c65fa66: *stop=0x8c65fa66u;r[15]+=28u;goto L8c65fa68; /* add #0x1c,r15 */
L8c65fa68: *stop=0x8c65fa68u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65fa6a; /* lds.l @r15+,PR */
L8c65fa6a: *stop=0x8c65fa6au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65fa6c; /* mov.l @r15+,r8 */
L8c65fa6c: *stop=0x8c65fa6cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65fa6e; /* mov.l @r15+,r9 */
L8c65fa6e: *stop=0x8c65fa6eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65fa70; /* mov.l @r15+,r10 */
L8c65fa70: *stop=0x8c65fa70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65fa72; /* mov.l @r15+,r11 */
L8c65fa72: *stop=0x8c65fa72u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65fa74; /* mov.l @r15+,r12 */
L8c65fa74: *stop=0x8c65fa74u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65fa76; /* mov.l @r15+,r13 */
L8c65fa76: *stop=0x8c65fa76u;if(*pr!=entry_pr)return -3;*stop=0x8c65fa78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_maple_8c65f7cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65f7cc;
L8c65f7cc: *stop=0x8c65f7ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f850u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65f7ce; /* mov.l 0x8c65f850,r5 */
L8c65f7ce: *stop=0x8c65f7ceu;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c65f7d0; /* cmp/pl r4 */
L8c65f7d0: *stop=0x8c65f7d0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f84au,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f7d2; /* mov.w 0x8c65f84a,r0 */
L8c65f7d2: *stop=0x8c65f7d2u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f7d4; /* mov.l @r5,r3 */
L8c65f7d4: *stop=0x8c65f7d4u;r[1]=r[0];goto L8c65f7d6; /* mov r0,r1 */
L8c65f7d6: *stop=0x8c65f7d6u;r[1]+=12u;goto L8c65f7d8; /* add #0xc,r1 */
L8c65f7d8: *stop=0x8c65f7d8u;r[2]=r[3];goto L8c65f7da; /* mov r3,r2 */
L8c65f7da: *stop=0x8c65f7dau;r[1]+=r[2];goto L8c65f7dc; /* add r2,r1 */
L8c65f7dc: *stop=0x8c65f7dcu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f7de; /* mov.l @r1,r1 */
L8c65f7de: *stop=0x8c65f7deu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f7e0; /* mov.l @(r0,r3),r2 */
L8c65f7e0: *stop=0x8c65f7e0u;r[1]<<=2;goto L8c65f7e2; /* shll2 r1 */
L8c65f7e2: *stop=0x8c65f7e2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f84cu,2);if(io_status)return io_status;r[7]=loaded;}goto L8c65f7e4; /* mov.w 0x8c65f84c,r7 */
L8c65f7e4: *stop=0x8c65f7e4u;r[2]+=r[1];goto L8c65f7e6; /* add r1,r2 */
L8c65f7e6: *stop=0x8c65f7e6u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f7e8; /* mov.l r2,@(r0,r3) */
L8c65f7e8: *stop=0x8c65f7e8u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f7ea; /* mov.l @r5,r1 */
L8c65f7ea: *stop=0x8c65f7eau;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f7ec; /* mov.l @(r0,r1),r1 */
L8c65f7ec: take=!(*sr&1u);*stop=0x8c65f7eeu;r[6]=0u;if(take)goto L8c65f7fa;goto L8c65f7f0; /* bf/s 0x8c65f7fa */
L8c65f7f0: *stop=0x8c65f7f0u;r[6]+=1u;goto L8c65f7f2; /* add #0x1,r6 */
L8c65f7f2: *stop=0x8c65f7f2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[7],4);if(io_status)return io_status;goto L8c65f7f4; /* mov.l r7,@r1 */
L8c65f7f4: *stop=0x8c65f7f4u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[4]);goto L8c65f7f6; /* cmp/ge r4,r6 */
L8c65f7f6: take=!(*sr&1u);*stop=0x8c65f7f8u;r[1]+=4u;if(take)goto L8c65f7f0;goto L8c65f7fa; /* bf/s 0x8c65f7f0 */
L8c65f7fa: *stop=0x8c65f7fau;{uint32_t loaded=sx(m,bus,&io_status,0x8c65f84eu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65f7fc; /* mov.w 0x8c65f84e,r0 */
L8c65f7fc: *stop=0x8c65f7fcu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f7fe; /* mov.l @r5,r2 */
L8c65f7fe: *stop=0x8c65f7feu;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[4],4);if(io_status)return io_status;goto L8c65f800; /* mov.l r4,@(r0,r2) */
L8c65f800: *stop=0x8c65f800u;r[0]+=4u;goto L8c65f802; /* add #0x4,r0 */
L8c65f802: *stop=0x8c65f802u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f804; /* mov.l @r5,r3 */
L8c65f804: *stop=0x8c65f804u;r[4]<<=2;goto L8c65f806; /* shll2 r4 */
L8c65f806: *stop=0x8c65f806u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f808; /* mov.l @(r0,r3),r2 */
L8c65f808: *stop=0x8c65f808u;r[2]+=r[4];goto L8c65f80a; /* add r4,r2 */
L8c65f80a: *stop=0x8c65f80au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c65f80c; /* mov.l r2,@(r0,r3) */
L8c65f80c: *stop=0x8c65f80cu;if(*pr!=entry_pr)return -3;*stop=0x8c65f80eu;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_maple_8c6607be(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6607be;
L8c6607be: *stop=0x8c6607beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6607ecu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6607c0; /* mov.l 0x8c6607ec,r0 */
L8c6607c0: *stop=0x8c6607c0u;if(*pr!=entry_pr)return -3;*stop=0x8c6607c2u;r[0]&=r[4];return 0; /* rts */
}
