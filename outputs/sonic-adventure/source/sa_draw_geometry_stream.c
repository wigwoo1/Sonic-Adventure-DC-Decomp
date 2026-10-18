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
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_geometry_stream_8c037294(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c037294;
L8c037294: *stop=0x8c037294u;if(!valid_access(r[4]+0u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0x0u,4);if(io_status)return io_status;goto L8c037296; /* mov.l @(0x0,r4),r2 */
L8c037296: *stop=0x8c037296u;if(!valid_access(r[4]+8u,4u))return -3;r[7]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c037298; /* mov.l @(0x8,r4),r7 */
L8c037298: *stop=0x8c037298u;r[1]=rd(m,bus,&io_status,0x8c037338u,4);if(io_status)return io_status;goto L8c03729a; /* mov.l 0x8c037338,r1 */
L8c03729a: *stop=0x8c03729au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03729c; /* fmov.s @r1+,fr4 */
L8c03729c: *stop=0x8c03729cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03729e; /* fmov.s @r1+,fr5 */
L8c03729e: *stop=0x8c03729eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0372a0; /* fmov.s @r1+,fr6 */
L8c0372a0: *stop=0x8c0372a0u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0372a2; /* fmov.s @r1+,fr7 */
L8c0372a2: *stop=0x8c0372a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[6],&f->fpscr,&fr[7]);if(status)return status;}goto L8c0372a4; /* fmul fr6,fr7 */
L8c0372a4: *stop=0x8c0372a4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0372a6; /* fmov.s @r1+,fr14 */
L8c0372a6: *stop=0x8c0372a6u;r[3]=rd(m,bus,&io_status,0x8c03733cu,4);if(io_status)return io_status;goto L8c0372a8; /* mov.l 0x8c03733c,r3 */
L8c0372a8: *stop=0x8c0372a8u;if(!valid_access(r[3],4u))return -3;r[3]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c0372aa; /* mov.l @r3,r3 */
L8c0372aa: *stop=0x8c0372aau;r[0]=r[3];goto L8c0372ac; /* mov r3,r0 */
L8c0372ac: *stop=0x8c0372acu;if(!valid_ram(*gbr+0x3cu,4u))return -3;wr(m,bus,&io_status,*gbr+0x3cu,r[0],4);if(io_status)return io_status;goto L8c0372ae; /* mov.l r0,@(0x3c,GBR) */
L8c0372ae: *stop=0x8c0372aeu;r[6]=r[3];goto L8c0372b0; /* mov r3,r6 */
L8c0372b0: *stop=0x8c0372b0u;r[0]=r[3];goto L8c0372b2; /* mov r3,r0 */
L8c0372b2: *stop=0x8c0372b2u;r[0]>>=16;goto L8c0372b4; /* shlr16 r0 */
L8c0372b4: *stop=0x8c0372b4u;r[0]>>=8;goto L8c0372b6; /* shlr8 r0 */
L8c0372b6: *stop=0x8c0372b6u;r[0]&=0x1cu;goto L8c0372b8; /* and #0x1c,r0 */
L8c0372b8: *stop=0x8c0372b8u;r[1]=rd(m,bus,&io_status,0x8c037340u,4);if(io_status)return io_status;goto L8c0372ba; /* mov.l 0x8c037340,r1 */
L8c0372ba: *stop=0x8c0372bau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0372bc; /* mov.l r0,@r1 */
L8c0372bc: *stop=0x8c0372bcu;r[1]=rd(m,bus,&io_status,0x8c037344u,4);if(io_status)return io_status;goto L8c0372be; /* mov.l 0x8c037344,r1 */
L8c0372be: *stop=0x8c0372beu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0372c0; /* mov.l r0,@r1 */
L8c0372c0: *stop=0x8c0372c0u;r[1]=rd(m,bus,&io_status,0x8c037348u,4);if(io_status)return io_status;goto L8c0372c2; /* mov.l 0x8c037348,r1 */
L8c0372c2: *stop=0x8c0372c2u;r[0]=rd(m,bus,&io_status,0x8c03734cu,4);if(io_status)return io_status;goto L8c0372c4; /* mov.l 0x8c03734c,r0 */
L8c0372c4: *stop=0x8c0372c4u;r[3]&=r[1];goto L8c0372c6; /* and r1,r3 */
L8c0372c6: *stop=0x8c0372c6u;r[3]|=r[0];goto L8c0372c8; /* or r0,r3 */
L8c0372c8: *stop=0x8c0372c8u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0372ca; /* fmov.s @r2+,fr0 */
L8c0372ca: *stop=0x8c0372cau;r[3]+=4294967280u;goto L8c0372cc; /* add #-0x10,r3 */
L8c0372cc: *stop=0x8c0372ccu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0372ce; /* fmov.s @r2+,fr1 */
L8c0372ce: *stop=0x8c0372ceu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0372d0; /* fmov.s @r2+,fr2 */
L8c0372d0: *stop=0x8c0372d0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c0372d2; /* fldi1 fr3 */
L8c0372d2: *stop=0x8c0372d2u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c0372d4; /* ftrv xmtrx,fv0 */
L8c0372d4: *stop=0x8c0372d4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c0372d6; /* fldi1 fr3 */
L8c0372d6: *stop=0x8c0372d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0372d8; /* fdiv fr2,fr3 */
L8c0372d8: *stop=0x8c0372d8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,1);if(status)return status;goto L8c0372da; /* fcmp/gt fr14,fr2 */
L8c0372da: if((*sr&1u))goto L8c0372de;goto L8c0372dc; /* bt 0x8c0372de */
L8c0372dc: *stop=0x8c0372dcu;r[13]+=1u;goto L8c0372de; /* add #0x1,r13 */
L8c0372de: *stop=0x8c0372deu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0372e0; /* fmov.s @r2+,fr8 */
L8c0372e0: *stop=0x8c0372e0u;r[3]+=28u;goto L8c0372e2; /* add #0x1c,r3 */
L8c0372e2: *stop=0x8c0372e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c0372e4; /* fmul fr6,fr0 */
L8c0372e4: *stop=0x8c0372e4u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0372e6; /* fmov.s @r2+,fr9 */
L8c0372e6: *stop=0x8c0372e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c0372e8; /* fmul fr7,fr1 */
L8c0372e8: *stop=0x8c0372e8u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0372ea; /* fmov.s @r2+,fr10 */
L8c0372ea: *stop=0x8c0372eau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]=0x3f800000u;goto L8c0372ec; /* fldi1 fr11 */
L8c0372ec: *stop=0x8c0372ecu;status=sa_sh4_vector_numeric(0xf9fdu,f,*fpul);if(status)return status;goto L8c0372ee; /* ftrv xmtrx,fv8 */
L8c0372ee: *stop=0x8c0372eeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]=0x3f800000u;goto L8c0372f0; /* fldi1 fr11 */
L8c0372f0: *stop=0x8c0372f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[11],&fr[10],&f->fpscr,&fr[11]);if(status)return status;}goto L8c0372f2; /* fdiv fr10,fr11 */
L8c0372f2: *stop=0x8c0372f2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][10],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,1);if(status)return status;goto L8c0372f4; /* fcmp/gt fr14,fr10 */
L8c0372f4: if((*sr&1u))goto L8c0372f8;goto L8c0372f6; /* bt 0x8c0372f8 */
L8c0372f6: *stop=0x8c0372f6u;r[13]+=1u;goto L8c0372f8; /* add #0x1,r13 */
L8c0372f8: *stop=0x8c0372f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c0372fa; /* fmul fr3,fr0 */
L8c0372fa: *stop=0x8c0372fau;if(!valid_access(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c0372fc; /* fmov.s fr3,@-r3 */
L8c0372fc: *stop=0x8c0372fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c0372fe; /* fmul fr3,fr1 */
L8c0372fe: *stop=0x8c0372feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[4],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c037300; /* fadd fr4,fr0 */
L8c037300: *stop=0x8c037300u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c037302; /* fldi1 fr3 */
L8c037302: *stop=0x8c037302u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[5],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c037304; /* fadd fr5,fr1 */
L8c037304: *stop=0x8c037304u;if(!valid_access(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c037306; /* fmov.s fr1,@-r3 */
L8c037306: *stop=0x8c037306u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[6],&f->fpscr,&fr[8]);if(status)return status;}goto L8c037308; /* fmul fr6,fr8 */
L8c037308: *stop=0x8c037308u;if(!valid_access(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[3]);if(io_status)return io_status;goto L8c03730a; /* fmov.s fr0,@-r3 */
L8c03730a: *stop=0x8c03730au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[7],&f->fpscr,&fr[9]);if(status)return status;}goto L8c03730c; /* fmul fr7,fr9 */
L8c03730c: *stop=0x8c03730cu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03730e; /* fmov.s @r2+,fr0 */
L8c03730e: *stop=0x8c03730eu;r[3]+=28u;goto L8c037310; /* add #0x1c,r3 */
L8c037310: *stop=0x8c037310u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037312; /* fmov.s @r2+,fr1 */
L8c037312: *stop=0x8c037312u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c037314; /* fldi1 fr3 */
L8c037314: *stop=0x8c037314u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037316; /* fmov.s @r2+,fr2 */
L8c037316: *stop=0x8c037316u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[11],&f->fpscr,&fr[9]);if(status)return status;}goto L8c037318; /* fmul fr11,fr9 */
L8c037318: *stop=0x8c037318u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c03731a; /* ftrv xmtrx,fv0 */
L8c03731a: *stop=0x8c03731au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c03731c; /* fdiv fr2,fr3 */
L8c03731c: *stop=0x8c03731cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[11],&f->fpscr,&fr[8]);if(status)return status;}goto L8c03731e; /* fmul fr11,fr8 */
L8c03731e: *stop=0x8c03731eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[5],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c037320; /* fadd fr5,fr9 */
L8c037320: *stop=0x8c037320u;if(!valid_access(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[3]);if(io_status)return io_status;goto L8c037322; /* fmov.s fr11,@-r3 */
L8c037322: *stop=0x8c037322u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[4],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c037324; /* fadd fr4,fr8 */
L8c037324: *stop=0x8c037324u;if(!valid_access(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[3]);if(io_status)return io_status;goto L8c037326; /* fmov.s fr9,@-r3 */
L8c037326: *stop=0x8c037326u;r[7]+=4294967294u;goto L8c037328; /* add #-0x2,r7 */
L8c037328: *stop=0x8c037328u;if(!valid_access(r[3]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[3]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[3]);if(io_status)return io_status;goto L8c03732a; /* fmov.s fr8,@-r3 */
L8c03732a: *stop=0x8c03732au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[6],0u);if(status)return status;goto L8c03732c; /* ocbi @r6 */
L8c03732c: *stop=0x8c03732cu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c03732e; /* pref @r3 */
L8c03732e: *stop=0x8c03732eu;*sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c037330; /* cmp/pl r7 */
L8c037330: take=(*sr&1u);*stop=0x8c037332u;r[6]+=32u;if(take)goto L8c0372d8;goto L8c037334; /* bt/s 0x8c0372d8 */
L8c037334: *stop=0x8c037334u;if(*pr!=entry_pr)return -3;*stop=0x8c037336u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_geometry_stream_8c60a61a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60a61a;
L8c60a61a: *stop=0x8c60a61au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c60a61c; /* mov.l r14,@-r15 */
L8c60a61c: *stop=0x8c60a61cu;r[5]=28u;goto L8c60a61e; /* mov #0x1c,r5 */
L8c60a61e: *stop=0x8c60a61eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c60a620; /* mov.l r13,@-r15 */
L8c60a620: *stop=0x8c60a620u;r[13]=rd(m,bus,&io_status,0x8c60a87cu,4);if(io_status)return io_status;goto L8c60a622; /* mov.l 0x8c60a87c,r13 */
L8c60a622: *stop=0x8c60a622u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c60a624; /* mov.l r12,@-r15 */
L8c60a624: *stop=0x8c60a624u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c60a626; /* mov.l r11,@-r15 */
L8c60a626: *stop=0x8c60a626u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c60a628; /* mov.l r8,@-r15 */
L8c60a628: *stop=0x8c60a628u;r[8]=rd(m,bus,&io_status,0x8c60a878u,4);if(io_status)return io_status;goto L8c60a62a; /* mov.l 0x8c60a878,r8 */
L8c60a62a: *stop=0x8c60a62au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c60a62c; /* sts.l PR,@-r15 */
L8c60a62c: *stop=0x8c60a62cu;if(!valid_access(r[13],2u))return -3;r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c60a62e; /* mov.w @r13,r11 */
L8c60a62e: *stop=0x8c60a62eu;r[12]=rd(m,bus,&io_status,0x8c60a880u,4);if(io_status)return io_status;goto L8c60a630; /* mov.l 0x8c60a880,r12 */
L8c60a630: *stop=0x8c60a630u;*macl=(r[5]&65535u)*(r[11]&65535u);goto L8c60a632; /* mulu.w r5,r11 */
L8c60a632: *stop=0x8c60a632u;r[5]=*macl;goto L8c60a634; /* sts MACL,r5 */
L8c60a634: *stop=0x8c60a634u;r[5]+=r[8];goto L8c60a636; /* add r8,r5 */
L8c60a636: *stop=0x8c60a636u;if(!valid_access(r[5]+20u,4u))return -3;r[14]=rd(m,bus,&io_status,r[5]+0x14u,4);if(io_status)return io_status;goto L8c60a638; /* mov.l @(0x14,r5),r14 */
L8c60a638: *stop=0x8c60a638u;if(!valid_access(r[5]+4u,4u))return -3;r[0]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c60a63a; /* mov.l @(0x4,r5),r0 */
L8c60a63a: *stop=0x8c60a63au;r[14]<<=2;goto L8c60a63c; /* shll2 r14 */
L8c60a63c: *stop=0x8c60a63cu;if(!valid_access(r[0]+r[14],4u))return -3;r[14]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c60a63e; /* mov.l @(r0,r14),r14 */
L8c60a63e: *stop=0x8c60a63eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c60a640; /* tst r14,r14 */
L8c60a640: if((*sr&1u))goto L8c60a6ba;goto L8c60a642; /* bt 0x8c60a6ba */
L8c60a642: *stop=0x8c60a642u;r[4]=28u;goto L8c60a644; /* mov #0x1c,r4 */
L8c60a644: *stop=0x8c60a644u;*macl=(r[4]&65535u)*(r[11]&65535u);goto L8c60a646; /* mulu.w r4,r11 */
L8c60a646: *stop=0x8c60a646u;r[5]=0u;goto L8c60a648; /* mov #0x0,r5 */
L8c60a648: *stop=0x8c60a648u;r[4]=*macl;goto L8c60a64a; /* sts MACL,r4 */
L8c60a64a: *stop=0x8c60a64au;r[4]+=r[8];goto L8c60a64c; /* add r8,r4 */
L8c60a64c: *stop=0x8c60a64cu;if(!valid_access(r[4]+20u,4u))return -3;r[6]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c60a64e; /* mov.l @(0x14,r4),r6 */
L8c60a64e: *stop=0x8c60a64eu;if(!valid_access(r[4]+8u,4u))return -3;r[0]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c60a650; /* mov.l @(0x8,r4),r0 */
L8c60a650: *stop=0x8c60a650u;r[6]<<=2;goto L8c60a652; /* shll2 r6 */
L8c60a652: *stop=0x8c60a652u;if(!valid_access(r[0]+r[6],4u))return -3;r[6]=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;goto L8c60a654; /* mov.l @(r0,r6),r6 */
L8c60a654: *stop=0x8c60a654u;r[0]=12u;goto L8c60a656; /* mov #0xc,r0 */
L8c60a656: *stop=0x8c60a656u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c60a658; /* fmov.s @(r0,r4),fr4 */
L8c60a658: *stop=0x8c60a658u;r[0]=0x8c60a884u;goto L8c60a65a; /* mova 0x8c60a884,r0 */
L8c60a65a: *stop=0x8c60a65au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c60a65c; /* fmov.s @r0,fr3 */
L8c60a65c: *stop=0x8c60a65cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c60a65e; /* fcmp/gt fr3,fr4 */
L8c60a65e: take=!(*sr&1u);*stop=0x8c60a660u;r[7]=r[6];if(take)goto L8c60a66c;goto L8c60a662; /* bf/s 0x8c60a66c */
L8c60a662: *stop=0x8c60a662u;r[0]=0x8c60a888u;goto L8c60a664; /* mova 0x8c60a888,r0 */
L8c60a664: *stop=0x8c60a664u;fcopy(f,3u,4u);goto L8c60a666; /* fmov fr4,fr3 */
L8c60a666: *stop=0x8c60a666u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c60a668; /* fmov.s @r0,fr2 */
L8c60a668: *stop=0x8c60a66au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60a66e; /* bra 0x8c60a66e */
L8c60a66c: *stop=0x8c60a66cu;fcopy(f,3u,4u);goto L8c60a66e; /* fmov fr4,fr3 */
L8c60a66e: *stop=0x8c60a66eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c60a670; /* ftrc fr3,FPUL */
L8c60a670: *stop=0x8c60a670u;r[1]=*fpul;goto L8c60a672; /* sts FPUL,r1 */
L8c60a672: *stop=0x8c60a674u;r[0]=1u;goto L8c60a690; /* bra 0x8c60a690 */
L8c60a676: *stop=0x8c60a676u;r[4]=r[5];goto L8c60a678; /* mov r5,r4 */
L8c60a678: *stop=0x8c60a678u;r[4]+=r[7];goto L8c60a67a; /* add r7,r4 */
L8c60a67a: *stop=0x8c60a67au;*sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1;goto L8c60a67c; /* shlr r4 */
L8c60a67c: *stop=0x8c60a67cu;r[2]=r[4];goto L8c60a67e; /* mov r4,r2 */
L8c60a67e: *stop=0x8c60a67eu;r[2]<<=2;goto L8c60a680; /* shll2 r2 */
L8c60a680: *stop=0x8c60a680u;r[2]<<=2;goto L8c60a682; /* shll2 r2 */
L8c60a682: *stop=0x8c60a682u;r[2]+=r[14];goto L8c60a684; /* add r14,r2 */
L8c60a684: *stop=0x8c60a684u;if(!valid_access(r[2],4u))return -3;r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c60a686; /* mov.l @r2,r3 */
L8c60a686: *stop=0x8c60a686u;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c60a688; /* cmp/hs r3,r1 */
L8c60a688: if(!(*sr&1u))goto L8c60a68e;goto L8c60a68a; /* bf 0x8c60a68e */
L8c60a68a: *stop=0x8c60a68cu;r[5]=r[4];goto L8c60a690; /* bra 0x8c60a690 */
L8c60a68e: *stop=0x8c60a68eu;r[7]=r[4];goto L8c60a690; /* mov r4,r7 */
L8c60a690: *stop=0x8c60a690u;r[2]=r[7];goto L8c60a692; /* mov r7,r2 */
L8c60a692: *stop=0x8c60a692u;r[2]-=r[5];goto L8c60a694; /* sub r5,r2 */
L8c60a694: *stop=0x8c60a694u;*sr=(*sr&~1u)|(r[2]>r[0]);goto L8c60a696; /* cmp/hi r0,r2 */
L8c60a696: if((*sr&1u))goto L8c60a676;goto L8c60a698; /* bt 0x8c60a676 */
L8c60a698: *stop=0x8c60a698u;r[7]=r[11]&65535u;goto L8c60a69a; /* extu.w r11,r7 */
L8c60a69a: *stop=0x8c60a69au;r[3]=rd(m,bus,&io_status,0x8c60a88cu,4);if(io_status)return io_status;goto L8c60a69c; /* mov.l 0x8c60a88c,r3 */
L8c60a69c: *stop=0x8c60a69cu;r[1]=r[7];goto L8c60a69e; /* mov r7,r1 */
L8c60a69e: *stop=0x8c60a69eu;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c60a6a0; /* shll r7 */
L8c60a6a0: *stop=0x8c60a6a0u;r[7]+=r[1];goto L8c60a6a2; /* add r1,r7 */
L8c60a6a2: *stop=0x8c60a6a2u;if(!valid_access(r[3],4u))return -3;r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c60a6a4; /* mov.l @r3,r2 */
L8c60a6a4: *stop=0x8c60a6a4u;r[7]<<=2;goto L8c60a6a6; /* shll2 r7 */
L8c60a6a6: *stop=0x8c60a6a6u;r[7]+=r[12];goto L8c60a6a8; /* add r12,r7 */
L8c60a6a8: target=r[2];*pr=0x8c60a6acu;*stop=0x8c60a6aau;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60a6ac; /* jsr @r2 */
L8c60a6ac: *stop=0x8c60a6acu;r[4]=rd(m,bus,&io_status,0x8c60a890u,4);if(io_status)return io_status;goto L8c60a6ae; /* mov.l 0x8c60a890,r4 */
L8c60a6ae: *stop=0x8c60a6aeu;if(!valid_access(r[13],2u))return -3;r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c60a6b0; /* mov.w @r13,r3 */
L8c60a6b0: *stop=0x8c60a6b0u;if(!valid_access(r[4],2u))return -3;r[2]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c60a6b2; /* mov.w @r4,r2 */
L8c60a6b2: *stop=0x8c60a6b2u;r[3]+=1u;goto L8c60a6b4; /* add #0x1,r3 */
L8c60a6b4: *stop=0x8c60a6b4u;r[2]|=r[3];goto L8c60a6b6; /* or r3,r2 */
L8c60a6b6: *stop=0x8c60a6b8u;if(!valid_access(r[4],2u))return -3;wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c60a6ec; /* bra 0x8c60a6ec */
L8c60a6ba: *stop=0x8c60a6bau;r[0]=r[11]&65535u;goto L8c60a6bc; /* extu.w r11,r0 */
L8c60a6bc: *stop=0x8c60a6bcu;if(!valid_access(r[4]+20u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c60a6be; /* mov.l @(0x14,r4),r2 */
L8c60a6be: *stop=0x8c60a6beu;r[3]=r[0];goto L8c60a6c0; /* mov r0,r3 */
L8c60a6c0: *stop=0x8c60a6c0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60a6c2; /* shll r0 */
L8c60a6c2: *stop=0x8c60a6c2u;r[0]+=r[3];goto L8c60a6c4; /* add r3,r0 */
L8c60a6c4: *stop=0x8c60a6c4u;r[0]<<=2;goto L8c60a6c6; /* shll2 r0 */
L8c60a6c6: *stop=0x8c60a6c6u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[2],4);if(io_status)return io_status;goto L8c60a6c8; /* mov.l r2,@(r0,r12) */
L8c60a6c8: *stop=0x8c60a6c8u;if(!valid_access(r[13],2u))return -3;r[0]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c60a6ca; /* mov.w @r13,r0 */
L8c60a6ca: *stop=0x8c60a6cau;if(!valid_access(r[4]+24u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;goto L8c60a6cc; /* mov.l @(0x18,r4),r2 */
L8c60a6cc: *stop=0x8c60a6ccu;r[0]=r[0]&65535u;goto L8c60a6ce; /* extu.w r0,r0 */
L8c60a6ce: *stop=0x8c60a6ceu;r[3]=r[0];goto L8c60a6d0; /* mov r0,r3 */
L8c60a6d0: *stop=0x8c60a6d0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60a6d2; /* shll r0 */
L8c60a6d2: *stop=0x8c60a6d2u;r[0]+=r[3];goto L8c60a6d4; /* add r3,r0 */
L8c60a6d4: *stop=0x8c60a6d4u;r[0]+=1u;goto L8c60a6d6; /* add #0x1,r0 */
L8c60a6d6: *stop=0x8c60a6d6u;r[0]<<=2;goto L8c60a6d8; /* shll2 r0 */
L8c60a6d8: *stop=0x8c60a6d8u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[2],4);if(io_status)return io_status;goto L8c60a6da; /* mov.l r2,@(r0,r12) */
L8c60a6da: *stop=0x8c60a6dau;if(!valid_access(r[13],2u))return -3;r[0]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c60a6dc; /* mov.w @r13,r0 */
L8c60a6dc: *stop=0x8c60a6dcu;if(!valid_access(r[4]+28u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;goto L8c60a6de; /* mov.l @(0x1c,r4),r2 */
L8c60a6de: *stop=0x8c60a6deu;r[0]=r[0]&65535u;goto L8c60a6e0; /* extu.w r0,r0 */
L8c60a6e0: *stop=0x8c60a6e0u;r[3]=r[0];goto L8c60a6e2; /* mov r0,r3 */
L8c60a6e2: *stop=0x8c60a6e2u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60a6e4; /* shll r0 */
L8c60a6e4: *stop=0x8c60a6e4u;r[0]+=r[3];goto L8c60a6e6; /* add r3,r0 */
L8c60a6e6: *stop=0x8c60a6e6u;r[0]+=2u;goto L8c60a6e8; /* add #0x2,r0 */
L8c60a6e8: *stop=0x8c60a6e8u;r[0]<<=2;goto L8c60a6ea; /* shll2 r0 */
L8c60a6ea: *stop=0x8c60a6eau;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[2],4);if(io_status)return io_status;goto L8c60a6ec; /* mov.l r2,@(r0,r12) */
L8c60a6ec: *stop=0x8c60a6ecu;if(!valid_access(r[13],2u))return -3;r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c60a6ee; /* mov.w @r13,r2 */
L8c60a6ee: *stop=0x8c60a6eeu;r[3]=28u;goto L8c60a6f0; /* mov #0x1c,r3 */
L8c60a6f0: *stop=0x8c60a6f0u;*macl=(r[3]&65535u)*(r[2]&65535u);goto L8c60a6f2; /* mulu.w r3,r2 */
L8c60a6f2: *stop=0x8c60a6f2u;r[3]=*macl;goto L8c60a6f4; /* sts MACL,r3 */
L8c60a6f4: *stop=0x8c60a6f4u;r[3]+=r[8];goto L8c60a6f6; /* add r8,r3 */
L8c60a6f6: *stop=0x8c60a6f6u;if(!valid_access(r[3]+20u,4u))return -3;r[2]=rd(m,bus,&io_status,r[3]+0x14u,4);if(io_status)return io_status;goto L8c60a6f8; /* mov.l @(0x14,r3),r2 */
L8c60a6f8: *stop=0x8c60a6f8u;r[2]+=1u;goto L8c60a6fa; /* add #0x1,r2 */
L8c60a6fa: *stop=0x8c60a6fau;if(!valid_access(r[3]+20u,4u))return -3;wr(m,bus,&io_status,r[3]+0x14u,r[2],4);if(io_status)return io_status;goto L8c60a6fc; /* mov.l r2,@(0x14,r3) */
L8c60a6fc: *stop=0x8c60a6fcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c60a6fe; /* lds.l @r15+,PR */
L8c60a6fe: *stop=0x8c60a6feu;if(!valid_access(r[15],4u))return -3;r[8]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c60a700; /* mov.l @r15+,r8 */
L8c60a700: *stop=0x8c60a700u;if(!valid_access(r[15],4u))return -3;r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c60a702; /* mov.l @r15+,r11 */
L8c60a702: *stop=0x8c60a702u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c60a704; /* mov.l @r15+,r12 */
L8c60a704: *stop=0x8c60a704u;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c60a706; /* mov.l @r15+,r13 */
L8c60a706: *stop=0x8c60a706u;if(*pr!=entry_pr)return -3;*stop=0x8c60a708u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_geometry_stream_8c037350(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c037350;
L8c037350: *stop=0x8c037350u;if(!valid_ram(*gbr+0x34u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x34u,4);if(io_status)return io_status;goto L8c037352; /* mov.l @(0x34,GBR),r0 */
L8c037352: *stop=0x8c037352u;r[5]=r[0];goto L8c037354; /* mov r0,r5 */
L8c037354: *stop=0x8c037354u;if(!valid_ram(*gbr+0x2cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x2cu,4);if(io_status)return io_status;goto L8c037356; /* mov.l @(0x2c,GBR),r0 */
L8c037356: *stop=0x8c037356u;r[10]=r[0];goto L8c037358; /* mov r0,r10 */
L8c037358: *stop=0x8c037358u;if(!valid_access(r[4]+4u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c03735a; /* mov.l @(0x4,r4),r2 */
L8c03735a: *stop=0x8c03735au;if(!valid_access(r[4]+8u,4u))return -3;r[7]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c03735c; /* mov.l @(0x8,r4),r7 */
L8c03735c: *stop=0x8c03735cu;if(!valid_ram(*gbr+0x3cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x3cu,4);if(io_status)return io_status;goto L8c03735e; /* mov.l @(0x3c,GBR),r0 */
L8c03735e: *stop=0x8c03735eu;r[8]=r[0];goto L8c037360; /* mov r0,r8 */
L8c037360: *stop=0x8c037360u;r[8]+=12u;goto L8c037362; /* add #0xc,r8 */
L8c037362: *stop=0x8c037362u;r[0]=rd(m,bus,&io_status,0x8c0373b0u,4);if(io_status)return io_status;goto L8c037364; /* mov.l 0x8c0373b0,r0 */
L8c037364: *stop=0x8c037364u;if(!valid_ram(*gbr+0x40u,4u))return -3;wr(m,bus,&io_status,*gbr+0x40u,r[0],4);if(io_status)return io_status;goto L8c037366; /* mov.l r0,@(0x40,GBR) */
L8c037366: *stop=0x8c037366u;r[9]=r[0];goto L8c037368; /* mov r0,r9 */
L8c037368: *stop=0x8c037368u;r[1]=rd(m,bus,&io_status,0x8c0373b4u,4);if(io_status)return io_status;goto L8c03736a; /* mov.l 0x8c0373b4,r1 */
L8c03736a: *stop=0x8c03736au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03736c; /* fmov.s @r1+,fr12 */
L8c03736c: *stop=0x8c03736cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03736e; /* fmov.s @r1+,fr13 */
L8c03736e: *stop=0x8c03736eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037370; /* fmov.s @r1+,fr14 */
L8c037370: *stop=0x8c037370u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c037372; /* fldi0 fr15 */
L8c037372: *stop=0x8c037372u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c037374; /* fschg */
L8c037374: *stop=0x8c037374u;fcopy(f,0u,1u);goto L8c037376; /* fmov fr1,fr0 */
L8c037376: *stop=0x8c037376u;fcopy(f,2u,3u);goto L8c037378; /* fmov fr3,fr2 */
L8c037378: *stop=0x8c037378u;status=sa_sh4_vector_numeric(0xf3edu,f,*fpul);if(status)return status;goto L8c03737a; /* fipr fv12,fv0 */
L8c03737a: *stop=0x8c03737au;fcopy(f,4u,5u);goto L8c03737c; /* fmov fr5,fr4 */
L8c03737c: *stop=0x8c03737cu;fcopy(f,6u,7u);goto L8c03737e; /* fmov fr7,fr6 */
L8c03737e: *stop=0x8c03737eu;status=sa_sh4_vector_numeric(0xf7edu,f,*fpul);if(status)return status;goto L8c037380; /* fipr fv12,fv4 */
L8c037380: *stop=0x8c037380u;fcopy(f,8u,9u);goto L8c037382; /* fmov fr9,fr8 */
L8c037382: *stop=0x8c037382u;fcopy(f,10u,11u);goto L8c037384; /* fmov fr11,fr10 */
L8c037384: *stop=0x8c037384u;status=sa_sh4_vector_numeric(0xfbedu,f,*fpul);if(status)return status;goto L8c037386; /* fipr fv12,fv8 */
L8c037386: *stop=0x8c037386u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c037388; /* fschg */
L8c037388: *stop=0x8c037388u;fcopy(f,12u,3u);goto L8c03738a; /* fmov fr3,fr12 */
L8c03738a: *stop=0x8c03738au;fcopy(f,13u,7u);goto L8c03738c; /* fmov fr7,fr13 */
L8c03738c: *stop=0x8c03738cu;fcopy(f,14u,11u);goto L8c03738e; /* fmov fr11,fr14 */
L8c03738e: *stop=0x8c03738eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c037390; /* fldi0 fr15 */
L8c037390: *stop=0x8c037390u;r[0]=rd(m,bus,&io_status,0x8c0373b8u,4);if(io_status)return io_status;goto L8c037392; /* mov.l 0x8c0373b8,r0 */
L8c037392: *stop=0x8c037392u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c037394; /* fmov.s @r0,fr7 */
L8c037394: *stop=0x8c037394u;r[12]=sx(m,bus,&io_status,0x8c0373acu,2);if(io_status)return io_status;goto L8c037396; /* mov.w 0x8c0373ac,r12 */
L8c037396: *stop=0x8c037396u;r[0]=rd(m,bus,&io_status,0x8c0373bcu,4);if(io_status)return io_status;goto L8c037398; /* mov.l 0x8c0373bc,r0 */
L8c037398: *stop=0x8c037398u;*sr=(*sr&~1u)|!(r[10]&r[0]);goto L8c03739a; /* tst r10,r0 */
L8c03739a: if(!(*sr&1u))goto L8c0373c0;goto L8c03739c; /* bf 0x8c0373c0 */
L8c03739c: *stop=0x8c03739cu;if(!valid_access(r[5]+4u,4u))return -3;r[0]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c03739e; /* mov.l @(0x4,r5),r0 */
L8c03739e: *stop=0x8c03739eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0373a0; /* tst r0,r0 */
L8c0373a0: if((*sr&1u))goto L8c0373c0;goto L8c0373a2; /* bt 0x8c0373c0 */
L8c0373a2: *stop=0x8c0373a2u;if(!valid_ram(*gbr+0x58u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x58u,4);if(io_status)return io_status;goto L8c0373a4; /* mov.l @(0x58,GBR),r0 */
L8c0373a4: *stop=0x8c0373a4u;r[5]=r[0];goto L8c0373a6; /* mov r0,r5 */
L8c0373a6: *stop=0x8c0373a6u;r[11]=sx(m,bus,&io_status,0x8c0373aeu,2);if(io_status)return io_status;goto L8c0373a8; /* mov.w 0x8c0373ae,r11 */
L8c0373a8: *stop=0x8c0373aau;r[10]=3u;goto L8c0373c8; /* bra 0x8c0373c8 */
L8c0373c0: *stop=0x8c0373c0u;if(!valid_ram(*gbr+0x58u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x58u,4);if(io_status)return io_status;goto L8c0373c2; /* mov.l @(0x58,GBR),r0 */
L8c0373c2: *stop=0x8c0373c2u;r[5]=r[0];goto L8c0373c4; /* mov r0,r5 */
L8c0373c4: *stop=0x8c0373c4u;r[11]=0u;goto L8c0373c6; /* mov #0x0,r11 */
L8c0373c6: *stop=0x8c0373c6u;r[10]=3u;goto L8c0373c8; /* mov #0x3,r10 */
L8c0373c8: *stop=0x8c0373c8u;r[7]+=4294967295u;goto L8c0373ca; /* add #-0x1,r7 */
L8c0373ca: *stop=0x8c0373cau;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0373cc; /* fmov.s @r2+,fr8 */
L8c0373cc: *stop=0x8c0373ccu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0373ce; /* fmov.s @r2+,fr9 */
L8c0373ce: *stop=0x8c0373ceu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0373d0; /* fmov.s @r2+,fr10 */
L8c0373d0: *stop=0x8c0373d0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]=0x3f800000u;goto L8c0373d2; /* fldi1 fr11 */
L8c0373d2: *stop=0x8c0373d2u;status=sa_sh4_vector_numeric(0xfbedu,f,*fpul);if(status)return status;goto L8c0373d4; /* fipr fv12,fv8 */
L8c0373d4: *stop=0x8c0373d4u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0373d6; /* fmov.s @r2+,fr0 */
L8c0373d6: *stop=0x8c0373d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[7],&f->fpscr,&fr[11]);if(status)return status;}goto L8c0373d8; /* fmul fr7,fr11 */
L8c0373d8: *stop=0x8c0373d8u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0373da; /* fmov.s @r2+,fr1 */
L8c0373da: *stop=0x8c0373dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[7],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c0373dc; /* fadd fr7,fr11 */
L8c0373dc: *stop=0x8c0373dcu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0373de; /* fmov.s @r2+,fr2 */
L8c0373de: *stop=0x8c0373deu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,fpul);if(status)return status;goto L8c0373e0; /* ftrc fr11,FPUL */
L8c0373e0: *stop=0x8c0373e0u;r[0]=*fpul;goto L8c0373e2; /* sts FPUL,r0 */
L8c0373e2: *stop=0x8c0373e2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c0373e4; /* fldi1 fr3 */
L8c0373e4: *stop=0x8c0373e4u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c0373e6; /* cmp/pz r0 */
L8c0373e6: *stop=0x8c0373e6u;status=sa_sh4_vector_numeric(0xf3edu,f,*fpul);if(status)return status;goto L8c0373e8; /* fipr fv12,fv0 */
L8c0373e8: if((*sr&1u))goto L8c0373ec;goto L8c0373ea; /* bt 0x8c0373ec */
L8c0373ea: *stop=0x8c0373eau;r[0]=0u;goto L8c0373ec; /* mov #0x0,r0 */
L8c0373ec: *stop=0x8c0373ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0373ee; /* fmul fr7,fr3 */
L8c0373ee: *stop=0x8c0373eeu;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[12]);goto L8c0373f0; /* cmp/gt r12,r0 */
L8c0373f0: *stop=0x8c0373f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c0373f2; /* fadd fr7,fr3 */
L8c0373f2: if(!(*sr&1u))goto L8c0373f6;goto L8c0373f4; /* bf 0x8c0373f6 */
L8c0373f4: *stop=0x8c0373f4u;r[0]=r[12];goto L8c0373f6; /* mov r12,r0 */
L8c0373f6: *stop=0x8c0373f6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c0373f8; /* ftrc fr3,FPUL */
L8c0373f8: *stop=0x8c0373f8u;r[14]=*fpul;goto L8c0373fa; /* sts FPUL,r14 */
L8c0373fa: *stop=0x8c0373fau;r[0]=(r[10]&0x80000000u)?((r[10]&31u)?r[0]>>((0u-r[10])&31u):0u):(r[0]<<(r[10]&31u));goto L8c0373fc; /* shld r10,r0 */
L8c0373fc: *stop=0x8c0373fcu;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c0373fe; /* cmp/pz r14 */
L8c0373fe: *stop=0x8c0373feu;r[0]+=r[5];goto L8c037400; /* add r5,r0 */
L8c037400: if((*sr&1u))goto L8c037404;goto L8c037402; /* bt 0x8c037404 */
L8c037402: *stop=0x8c037402u;r[14]=0u;goto L8c037404; /* mov #0x0,r14 */
L8c037404: *stop=0x8c037404u;if(!valid_access(r[0],4u))return -3;r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]+=4;goto L8c037406; /* mov.l @r0+,r3 */
L8c037406: *stop=0x8c037406u;*sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[12]);goto L8c037408; /* cmp/gt r12,r14 */
L8c037408: *stop=0x8c037408u;if(!valid_access(r[0]+r[11],4u))return -3;r[0]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c03740a; /* mov.l @(r0,r11),r0 */
L8c03740a: if(!(*sr&1u))goto L8c03740e;goto L8c03740c; /* bf 0x8c03740e */
L8c03740c: *stop=0x8c03740cu;r[14]=r[12];goto L8c03740e; /* mov r12,r14 */
L8c03740e: *stop=0x8c03740eu;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c037410; /* mov.l r3,@r8 */
L8c037410: *stop=0x8c037410u;r[14]=(r[10]&0x80000000u)?((r[10]&31u)?r[14]>>((0u-r[10])&31u):0u):(r[14]<<(r[10]&31u));goto L8c037412; /* shld r10,r14 */
L8c037412: *stop=0x8c037412u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[0],4);if(io_status)return io_status;goto L8c037414; /* mov.l r0,@r9 */
L8c037414: *stop=0x8c037414u;r[14]+=r[5];goto L8c037416; /* add r5,r14 */
L8c037416: *stop=0x8c037416u;r[8]+=16u;goto L8c037418; /* add #0x10,r8 */
L8c037418: *stop=0x8c037418u;if(!valid_access(r[14],4u))return -3;r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[14]+=4;goto L8c03741a; /* mov.l @r14+,r3 */
L8c03741a: *stop=0x8c03741au;r[9]+=4u;goto L8c03741c; /* add #0x4,r9 */
L8c03741c: *stop=0x8c03741cu;r[14]+=r[11];goto L8c03741e; /* add r11,r14 */
L8c03741e: *stop=0x8c03741eu;if(!valid_access(r[14],4u))return -3;r[14]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c037420; /* mov.l @r14,r14 */
L8c037420: *stop=0x8c037420u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037422; /* fmov.s @r2+,fr8 */
L8c037422: *stop=0x8c037422u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c037424; /* mov.l r3,@r8 */
L8c037424: *stop=0x8c037424u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037426; /* fmov.s @r2+,fr9 */
L8c037426: *stop=0x8c037426u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[14],4);if(io_status)return io_status;goto L8c037428; /* mov.l r14,@r9 */
L8c037428: *stop=0x8c037428u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03742a; /* fmov.s @r2+,fr10 */
L8c03742a: *stop=0x8c03742au;r[8]+=16u;goto L8c03742c; /* add #0x10,r8 */
L8c03742c: *stop=0x8c03742cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]=0x3f800000u;goto L8c03742e; /* fldi1 fr11 */
L8c03742e: *stop=0x8c03742eu;r[7]+=4294967294u;goto L8c037430; /* add #-0x2,r7 */
L8c037430: *stop=0x8c037430u;*sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c037432; /* cmp/pl r7 */
L8c037432: take=(*sr&1u);*stop=0x8c037434u;r[9]+=4u;if(take)goto L8c0373d2;goto L8c037436; /* bt/s 0x8c0373d2 */
L8c037436: *stop=0x8c037436u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c037438; /* tst r7,r7 */
L8c037438: if(!(*sr&1u))goto L8c03745c;goto L8c03743a; /* bf 0x8c03745c */
L8c03743a: *stop=0x8c03743au;status=sa_sh4_vector_numeric(0xfbedu,f,*fpul);if(status)return status;goto L8c03743c; /* fipr fv12,fv8 */
L8c03743c: *stop=0x8c03743cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[7],&f->fpscr,&fr[11]);if(status)return status;}goto L8c03743e; /* fmul fr7,fr11 */
L8c03743e: *stop=0x8c03743eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[7],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c037440; /* fadd fr7,fr11 */
L8c037440: *stop=0x8c037440u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,fpul);if(status)return status;goto L8c037442; /* ftrc fr11,FPUL */
L8c037442: *stop=0x8c037442u;r[0]=*fpul;goto L8c037444; /* sts FPUL,r0 */
L8c037444: *stop=0x8c037444u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c037446; /* cmp/pz r0 */
L8c037446: if((*sr&1u))goto L8c03744a;goto L8c037448; /* bt 0x8c03744a */
L8c037448: *stop=0x8c037448u;r[0]=0u;goto L8c03744a; /* mov #0x0,r0 */
L8c03744a: *stop=0x8c03744au;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[12]);goto L8c03744c; /* cmp/gt r12,r0 */
L8c03744c: if(!(*sr&1u))goto L8c037450;goto L8c03744e; /* bf 0x8c037450 */
L8c03744e: *stop=0x8c03744eu;r[0]=r[12];goto L8c037450; /* mov r12,r0 */
L8c037450: *stop=0x8c037450u;r[0]=(r[10]&0x80000000u)?((r[10]&31u)?r[0]>>((0u-r[10])&31u):0u):(r[0]<<(r[10]&31u));goto L8c037452; /* shld r10,r0 */
L8c037452: *stop=0x8c037452u;r[0]+=r[5];goto L8c037454; /* add r5,r0 */
L8c037454: *stop=0x8c037454u;if(!valid_access(r[0],4u))return -3;r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]+=4;goto L8c037456; /* mov.l @r0+,r3 */
L8c037456: *stop=0x8c037456u;if(!valid_access(r[0]+r[11],4u))return -3;r[0]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c037458; /* mov.l @(r0,r11),r0 */
L8c037458: *stop=0x8c037458u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c03745a; /* mov.l r3,@r8 */
L8c03745a: *stop=0x8c03745au;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[0],4);if(io_status)return io_status;goto L8c03745c; /* mov.l r0,@r9 */
L8c03745c: *stop=0x8c03745cu;if(*pr!=entry_pr)return -3;*stop=0x8c03745eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_geometry_stream_8c0376d0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0376d0;
L8c0376d0: *stop=0x8c0376d0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0376d2; /* sts.l PR,@-r15 */
L8c0376d2: *stop=0x8c0376d2u;if(!valid_access(r[4]+12u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c0376d4; /* mov.l @(0xc,r4),r2 */
L8c0376d4: *stop=0x8c0376d4u;if(!valid_access(r[4]+16u,4u))return -3;r[3]=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;goto L8c0376d6; /* mov.l @(0x10,r4),r3 */
L8c0376d6: *stop=0x8c0376d6u;if(!valid_access(r[4]+20u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x14u,2);if(io_status)return io_status;goto L8c0376d8; /* mov.w @(0x14,r4),r0 */
L8c0376d8: *stop=0x8c0376d8u;r[7]=r[0];goto L8c0376da; /* mov r0,r7 */
L8c0376da: *stop=0x8c0376dau;if(!valid_access(r[2]+0u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x0u,2);if(io_status)return io_status;goto L8c0376dc; /* mov.w @(0x0,r2),r0 */
L8c0376dc: *stop=0x8c0376dcu;r[8]=sx(m,bus,&io_status,0x8c03772cu,2);if(io_status)return io_status;goto L8c0376de; /* mov.w 0x8c03772c,r8 */
L8c0376de: *stop=0x8c0376deu;r[0]&=r[8];goto L8c0376e0; /* and r8,r0 */
L8c0376e0: *stop=0x8c0376e0u;r[5]=20u;goto L8c0376e2; /* mov #0x14,r5 */
L8c0376e2: *stop=0x8c0376e2u;*macl=(r[0]&65535u)*(r[5]&65535u);goto L8c0376e4; /* mulu.w r0,r5 */
L8c0376e4: *stop=0x8c0376e4u;r[5]=*macl;goto L8c0376e6; /* sts MACL,r5 */
L8c0376e6: *stop=0x8c0376e6u;r[5]+=r[3];goto L8c0376e8; /* add r3,r5 */
L8c0376e8: *stop=0x8c0376e8u;if(!valid_access(r[5]+16u,4u))return -3;r[0]=rd(m,bus,&io_status,r[5]+0x10u,4);if(io_status)return io_status;goto L8c0376ea; /* mov.l @(0x10,r5),r0 */
L8c0376ea: *stop=0x8c0376eau;r[1]=rd(m,bus,&io_status,0x8c037730u,4);if(io_status)return io_status;goto L8c0376ec; /* mov.l 0x8c037730,r1 */
L8c0376ec: *stop=0x8c0376ecu;if(!valid_access(r[1],4u))return -3;r[8]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]+=4;goto L8c0376ee; /* mov.l @r1+,r8 */
L8c0376ee: *stop=0x8c0376eeu;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c0376f0; /* tst r8,r8 */
L8c0376f0: if((*sr&1u))goto L8c0376fa;goto L8c0376f2; /* bt 0x8c0376fa */
L8c0376f2: *stop=0x8c0376f2u;if(!valid_access(r[1],4u))return -3;r[8]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]+=4;goto L8c0376f4; /* mov.l @r1+,r8 */
L8c0376f4: *stop=0x8c0376f4u;r[0]&=r[8];goto L8c0376f6; /* and r8,r0 */
L8c0376f6: *stop=0x8c0376f6u;if(!valid_access(r[1],4u))return -3;r[8]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]+=4;goto L8c0376f8; /* mov.l @r1+,r8 */
L8c0376f8: *stop=0x8c0376f8u;r[0]|=r[8];goto L8c0376fa; /* or r8,r0 */
L8c0376fa: target=0x8c037618u;*pr=0x8c0376feu;*stop=0x8c0376fcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0376fe; /* bsr 0x8c037618 */
L8c0376fe: *stop=0x8c0376feu;if(!valid_ram(*gbr+0x2cu,4u))return -3;wr(m,bus,&io_status,*gbr+0x2cu,r[0],4);if(io_status)return io_status;goto L8c037700; /* mov.l r0,@(0x2c,GBR) */
L8c037700: *stop=0x8c037700u;r[8]=rd(m,bus,&io_status,0x8c037734u,4);if(io_status)return io_status;goto L8c037702; /* mov.l 0x8c037734,r8 */
L8c037702: *stop=0x8c037702u;*sr=(*sr&~1u)|!(r[8]&r[0]);goto L8c037704; /* tst r8,r0 */
L8c037704: if((*sr&1u))goto L8c037794;goto L8c037706; /* bt 0x8c037794 */
L8c037706: *stop=0x8c037706u;if(!valid_access(r[5]+12u,4u))return -3;r[0]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c037708; /* mov.l @(0xc,r5),r0 */
L8c037708: *stop=0x8c037708u;r[8]=rd(m,bus,&io_status,0x8c037738u,4);if(io_status)return io_status;goto L8c03770a; /* mov.l 0x8c037738,r8 */
L8c03770a: *stop=0x8c03770au;r[0]&=r[8];goto L8c03770c; /* and r8,r0 */
L8c03770c: target=0x8c037638u;*pr=0x8c037710u;*stop=0x8c03770eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037710; /* bsr 0x8c037638 */
L8c037710: *stop=0x8c037710u;if(!valid_ram(*gbr+0x2cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x2cu,4);if(io_status)return io_status;goto L8c037712; /* mov.l @(0x2c,GBR),r0 */
L8c037712: *stop=0x8c037712u;r[8]=rd(m,bus,&io_status,0x8c03773cu,4);if(io_status)return io_status;goto L8c037714; /* mov.l 0x8c03773c,r8 */
L8c037714: *stop=0x8c037714u;*sr=(*sr&~1u)|!(r[8]&r[0]);goto L8c037716; /* tst r8,r0 */
L8c037716: if((*sr&1u))goto L8c037760;goto L8c037718; /* bt 0x8c037760 */
L8c037718: target=0x8c037872u;*pr=0x8c03771cu;*stop=0x8c03771au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03771c; /* bsr 0x8c037872 */
L8c03771c: *stop=0x8c03771cu;if(!valid_ram(*gbr+0x2cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x2cu,4);if(io_status)return io_status;goto L8c03771e; /* mov.l @(0x2c,GBR),r0 */
L8c03771e: *stop=0x8c03771eu;r[8]=rd(m,bus,&io_status,0x8c037740u,4);if(io_status)return io_status;goto L8c037720; /* mov.l 0x8c037740,r8 */
L8c037720: *stop=0x8c037720u;*sr=(*sr&~1u)|!(r[8]&r[0]);goto L8c037722; /* tst r8,r0 */
L8c037722: if((*sr&1u))goto L8c037748;goto L8c037724; /* bt 0x8c037748 */
L8c037724: *stop=0x8c037724u;r[0]=rd(m,bus,&io_status,0x8c037744u,4);if(io_status)return io_status;goto L8c037726; /* mov.l 0x8c037744,r0 */
L8c037726: *stop=0x8c037726u;if(!valid_ram(*gbr+0x38u,4u))return -3;wr(m,bus,&io_status,*gbr+0x38u,r[0],4);if(io_status)return io_status;goto L8c037728; /* mov.l r0,@(0x38,GBR) */
L8c037728: *stop=0x8c03772au;goto L8c03774c; /* bra 0x8c03774c */
L8c037748: *stop=0x8c037748u;if(!valid_ram(*gbr+0x40u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x40u,4);if(io_status)return io_status;goto L8c03774a; /* mov.l @(0x40,GBR),r0 */
L8c03774a: *stop=0x8c03774au;if(!valid_ram(*gbr+0x38u,4u))return -3;wr(m,bus,&io_status,*gbr+0x38u,r[0],4);if(io_status)return io_status;goto L8c03774c; /* mov.l r0,@(0x38,GBR) */
L8c03774c: *stop=0x8c03774cu;r[0]=rd(m,bus,&io_status,0x8c03775cu,4);if(io_status)return io_status;goto L8c03774e; /* mov.l 0x8c03775c,r0 */
L8c03774e: target=0x8c03767cu;*pr=0x8c037752u;*stop=0x8c037750u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037752; /* bsr 0x8c03767c */
L8c037752: target=0x8c0375b0u;*pr=0x8c037756u;*stop=0x8c037754u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037756; /* bsr 0x8c0375b0 */
L8c037756: *stop=0x8c037758u;goto L8c0377c8; /* bra 0x8c0377c8 */
L8c037760: *stop=0x8c037760u;r[8]=rd(m,bus,&io_status,0x8c037770u,4);if(io_status)return io_status;goto L8c037762; /* mov.l 0x8c037770,r8 */
L8c037762: *stop=0x8c037762u;*sr=(*sr&~1u)|!(r[8]&r[0]);goto L8c037764; /* tst r8,r0 */
L8c037764: if((*sr&1u))goto L8c03777c;goto L8c037766; /* bt 0x8c03777c */
L8c037766: *stop=0x8c037766u;r[0]=rd(m,bus,&io_status,0x8c037774u,4);if(io_status)return io_status;goto L8c037768; /* mov.l 0x8c037774,r0 */
L8c037768: *stop=0x8c037768u;if(!valid_ram(*gbr+0x38u,4u))return -3;wr(m,bus,&io_status,*gbr+0x38u,r[0],4);if(io_status)return io_status;goto L8c03776a; /* mov.l r0,@(0x38,GBR) */
L8c03776a: *stop=0x8c03776au;r[0]=rd(m,bus,&io_status,0x8c037778u,4);if(io_status)return io_status;goto L8c03776c; /* mov.l 0x8c037778,r0 */
L8c03776c: *stop=0x8c03776eu;goto L8c037782; /* bra 0x8c037782 */
L8c03777c: *stop=0x8c03777cu;if(!valid_ram(*gbr+0x40u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x40u,4);if(io_status)return io_status;goto L8c03777e; /* mov.l @(0x40,GBR),r0 */
L8c03777e: *stop=0x8c03777eu;if(!valid_ram(*gbr+0x38u,4u))return -3;wr(m,bus,&io_status,*gbr+0x38u,r[0],4);if(io_status)return io_status;goto L8c037780; /* mov.l r0,@(0x38,GBR) */
L8c037780: *stop=0x8c037780u;r[0]=rd(m,bus,&io_status,0x8c037790u,4);if(io_status)return io_status;goto L8c037782; /* mov.l 0x8c037790,r0 */
L8c037782: target=0x8c03767cu;*pr=0x8c037786u;*stop=0x8c037784u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037786; /* bsr 0x8c03767c */
L8c037786: target=0x8c037538u;*pr=0x8c03778au;*stop=0x8c037788u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03778a; /* bsr 0x8c037538 */
L8c03778a: *stop=0x8c03778cu;goto L8c0377c8; /* bra 0x8c0377c8 */
L8c037794: *stop=0x8c037794u;if(!valid_ram(*gbr+0x40u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x40u,4);if(io_status)return io_status;goto L8c037796; /* mov.l @(0x40,GBR),r0 */
L8c037796: *stop=0x8c037796u;if(!valid_ram(*gbr+0x38u,4u))return -3;wr(m,bus,&io_status,*gbr+0x38u,r[0],4);if(io_status)return io_status;goto L8c037798; /* mov.l r0,@(0x38,GBR) */
L8c037798: *stop=0x8c037798u;if(!valid_ram(*gbr+0x2cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x2cu,4);if(io_status)return io_status;goto L8c03779a; /* mov.l @(0x2c,GBR),r0 */
L8c03779a: *stop=0x8c03779au;r[8]=rd(m,bus,&io_status,0x8c0377b4u,4);if(io_status)return io_status;goto L8c03779c; /* mov.l 0x8c0377b4,r8 */
L8c03779c: *stop=0x8c03779cu;*sr=(*sr&~1u)|!(r[8]&r[0]);goto L8c03779e; /* tst r8,r0 */
L8c03779e: if((*sr&1u))goto L8c0377bc;goto L8c0377a0; /* bt 0x8c0377bc */
L8c0377a0: target=0x8c037872u;*pr=0x8c0377a4u;*stop=0x8c0377a2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0377a4; /* bsr 0x8c037872 */
L8c0377a4: *stop=0x8c0377a4u;r[0]=rd(m,bus,&io_status,0x8c0377b8u,4);if(io_status)return io_status;goto L8c0377a6; /* mov.l 0x8c0377b8,r0 */
L8c0377a6: target=0x8c03767cu;*pr=0x8c0377aau;*stop=0x8c0377a8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0377aa; /* bsr 0x8c03767c */
L8c0377aa: target=0x8c0374d4u;*pr=0x8c0377aeu;*stop=0x8c0377acu;r[0]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0377ae; /* bsr 0x8c0374d4 */
L8c0377ae: *stop=0x8c0377b0u;goto L8c0377c8; /* bra 0x8c0377c8 */
L8c0377bc: *stop=0x8c0377bcu;r[0]=rd(m,bus,&io_status,0x8c0377e0u,4);if(io_status)return io_status;goto L8c0377be; /* mov.l 0x8c0377e0,r0 */
L8c0377be: target=0x8c03767cu;*pr=0x8c0377c2u;*stop=0x8c0377c0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0377c2; /* bsr 0x8c03767c */
L8c0377c2: *stop=0x8c0377c2u;r[12]=rd(m,bus,&io_status,0x8c0377e4u,4);if(io_status)return io_status;goto L8c0377c4; /* mov.l 0x8c0377e4,r12 */
L8c0377c4: target=0x8c037460u;*pr=0x8c0377c8u;*stop=0x8c0377c6u;r[0]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0377c8; /* bsr 0x8c037460 */
L8c0377c8: *stop=0x8c0377c8u;r[14]=r[0];goto L8c0377ca; /* mov r0,r14 */
L8c0377ca: *stop=0x8c0377cau;if(!valid_access(r[2]+0u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x0u,2);if(io_status)return io_status;goto L8c0377cc; /* mov.w @(0x0,r2),r0 */
L8c0377cc: *stop=0x8c0377ccu;r[9]=r[0]&65535u;goto L8c0377ce; /* extu.w r0,r9 */
L8c0377ce: *stop=0x8c0377ceu;r[8]=sx(m,bus,&io_status,0x8c0377deu,2);if(io_status)return io_status;goto L8c0377d0; /* mov.w 0x8c0377de,r8 */
L8c0377d0: *stop=0x8c0377d0u;r[9]&=r[8];goto L8c0377d2; /* and r8,r9 */
L8c0377d2: *stop=0x8c0377d2u;r[8]=4294967284u;goto L8c0377d4; /* mov #-0xc,r8 */
L8c0377d4: *stop=0x8c0377d4u;r[9]=(r[8]&0x80000000u)?((r[8]&31u)?r[9]>>((0u-r[8])&31u):0u):(r[9]<<(r[8]&31u));goto L8c0377d6; /* shld r8,r9 */
L8c0377d6: *stop=0x8c0377d6u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c0377d8; /* tst r13,r13 */
L8c0377d8: if((*sr&1u))goto L8c0377e8;goto L8c0377da; /* bt 0x8c0377e8 */
L8c0377da: *stop=0x8c0377dcu;goto L8c037824; /* bra 0x8c037824 */
L8c0377e8: *stop=0x8c0377e8u;if(!valid_access(r[5]+16u,4u))return -3;r[6]=rd(m,bus,&io_status,r[5]+0x10u,4);if(io_status)return io_status;goto L8c0377ea; /* mov.l @(0x10,r5),r6 */
L8c0377ea: *stop=0x8c0377eau;r[8]=rd(m,bus,&io_status,0x8c037800u,4);if(io_status)return io_status;goto L8c0377ec; /* mov.l 0x8c037800,r8 */
L8c0377ec: *stop=0x8c0377ecu;*sr=(*sr&~1u)|!(r[8]&r[6]);goto L8c0377ee; /* tst r8,r6 */
L8c0377ee: if((*sr&1u))goto L8c03780c;goto L8c0377f0; /* bt 0x8c03780c */
L8c0377f0: *stop=0x8c0377f0u;r[0]=rd(m,bus,&io_status,0x8c037804u,4);if(io_status)return io_status;goto L8c0377f2; /* mov.l 0x8c037804,r0 */
L8c0377f2: *stop=0x8c0377f2u;if(!valid_access(r[0]+r[9],4u))return -3;r[0]=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;goto L8c0377f4; /* mov.l @(r0,r9),r0 */
L8c0377f4: *stop=0x8c0377f4u;r[9]=rd(m,bus,&io_status,0x8c037808u,4);if(io_status)return io_status;goto L8c0377f6; /* mov.l 0x8c037808,r9 */
L8c0377f6: target=r[9];*pr=0x8c0377fau;*stop=0x8c0377f8u;if(!valid_ram(*gbr+0x54u,4u))return -3;wr(m,bus,&io_status,*gbr+0x54u,r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0377fa; /* jsr @r9 */
L8c0377fa: *stop=0x8c0377fcu;r[14]=r[0];goto L8c037854; /* bra 0x8c037854 */
L8c03780c: *stop=0x8c03780cu;r[0]=rd(m,bus,&io_status,0x8c03781cu,4);if(io_status)return io_status;goto L8c03780e; /* mov.l 0x8c03781c,r0 */
L8c03780e: *stop=0x8c03780eu;if(!valid_access(r[0]+r[9],4u))return -3;r[0]=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;goto L8c037810; /* mov.l @(r0,r9),r0 */
L8c037810: *stop=0x8c037810u;r[9]=rd(m,bus,&io_status,0x8c037820u,4);if(io_status)return io_status;goto L8c037812; /* mov.l 0x8c037820,r9 */
L8c037812: target=r[9];*pr=0x8c037816u;*stop=0x8c037814u;if(!valid_ram(*gbr+0x54u,4u))return -3;wr(m,bus,&io_status,*gbr+0x54u,r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037816; /* jsr @r9 */
L8c037816: *stop=0x8c037818u;r[14]=r[0];goto L8c037854; /* bra 0x8c037854 */
L8c037824: *stop=0x8c037824u;if(!valid_access(r[5]+16u,4u))return -3;r[6]=rd(m,bus,&io_status,r[5]+0x10u,4);if(io_status)return io_status;goto L8c037826; /* mov.l @(0x10,r5),r6 */
L8c037826: *stop=0x8c037826u;r[8]=rd(m,bus,&io_status,0x8c03783cu,4);if(io_status)return io_status;goto L8c037828; /* mov.l 0x8c03783c,r8 */
L8c037828: *stop=0x8c037828u;*sr=(*sr&~1u)|!(r[8]&r[6]);goto L8c03782a; /* tst r8,r6 */
L8c03782a: if((*sr&1u))goto L8c037848;goto L8c03782c; /* bt 0x8c037848 */
L8c03782c: *stop=0x8c03782cu;r[0]=rd(m,bus,&io_status,0x8c037840u,4);if(io_status)return io_status;goto L8c03782e; /* mov.l 0x8c037840,r0 */
L8c03782e: *stop=0x8c03782eu;if(!valid_access(r[0]+r[9],4u))return -3;r[0]=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;goto L8c037830; /* mov.l @(r0,r9),r0 */
L8c037830: *stop=0x8c037830u;r[9]=rd(m,bus,&io_status,0x8c037844u,4);if(io_status)return io_status;goto L8c037832; /* mov.l 0x8c037844,r9 */
L8c037832: target=r[9];*pr=0x8c037836u;*stop=0x8c037834u;if(!valid_ram(*gbr+0x54u,4u))return -3;wr(m,bus,&io_status,*gbr+0x54u,r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037836; /* jsr @r9 */
L8c037836: *stop=0x8c037838u;r[14]=r[0];goto L8c037854; /* bra 0x8c037854 */
L8c037848: *stop=0x8c037848u;r[0]=rd(m,bus,&io_status,0x8c037864u,4);if(io_status)return io_status;goto L8c03784a; /* mov.l 0x8c037864,r0 */
L8c03784a: *stop=0x8c03784au;if(!valid_access(r[0]+r[9],4u))return -3;r[0]=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;goto L8c03784c; /* mov.l @(r0,r9),r0 */
L8c03784c: *stop=0x8c03784cu;r[9]=rd(m,bus,&io_status,0x8c037868u,4);if(io_status)return io_status;goto L8c03784e; /* mov.l 0x8c037868,r9 */
L8c03784e: target=r[9];*pr=0x8c037852u;*stop=0x8c037850u;if(!valid_ram(*gbr+0x54u,4u))return -3;wr(m,bus,&io_status,*gbr+0x54u,r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037852; /* jsr @r9 */
L8c037852: *stop=0x8c037852u;r[14]=r[0];goto L8c037854; /* mov r0,r14 */
L8c037854: target=0x8c0376b0u;*pr=0x8c037858u;*stop=0x8c037856u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037858; /* bsr 0x8c0376b0 */
L8c037858: *stop=0x8c037858u;r[2]+=24u;goto L8c03785a; /* add #0x18,r2 */
L8c03785a: *stop=0x8c03785au;r[7]--;*sr=(*sr&~1u)|(!r[7]);goto L8c03785c; /* dt r7 */
L8c03785c: if((*sr&1u))goto L8c03786c;goto L8c03785e; /* bt 0x8c03786c */
L8c03785e: *stop=0x8c037860u;goto L8c0376da; /* bra 0x8c0376da */
L8c03786c: *stop=0x8c03786cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c03786e; /* lds.l @r15+,PR */
L8c03786e: *stop=0x8c03786eu;if(*pr!=entry_pr)return -3;*stop=0x8c037870u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_geometry_stream_8c036ffc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c036ffc;
L8c036ffc: *stop=0x8c036ffcu;r[1]=rd(m,bus,&io_status,0x8c037008u,4);if(io_status)return io_status;goto L8c036ffe; /* mov.l 0x8c037008,r1 */
L8c036ffe: *stop=0x8c036ffeu;if(!valid_access(r[1]+8u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c037000; /* mov.l @(0x8,r1),r0 */
L8c037000: *stop=0x8c037000u;r[0]|=0xc0u;goto L8c037002; /* or #0xc0,r0 */
L8c037002: *stop=0x8c037002u;if(*pr!=entry_pr)return -3;*stop=0x8c037004u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_geometry_stream_8c604ba0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c604ba0;
L8c604ba0: *stop=0x8c604ba0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c604ba2; /* mov.l r14,@-r15 */
L8c604ba2: *stop=0x8c604ba2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c604ba4; /* mov.l r13,@-r15 */
L8c604ba4: *stop=0x8c604ba4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c604ba6; /* mov.l r12,@-r15 */
L8c604ba6: *stop=0x8c604ba6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c604ba8; /* mov.l r11,@-r15 */
L8c604ba8: *stop=0x8c604ba8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c604baa; /* mov.l r10,@-r15 */
L8c604baa: *stop=0x8c604baau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c604bac; /* mov.l r9,@-r15 */
L8c604bac: *stop=0x8c604bacu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c604bae; /* sts.l PR,@-r15 */
L8c604bae: *stop=0x8c604baeu;r[0]=sx(m,bus,&io_status,0x8c604cccu,2);if(io_status)return io_status;goto L8c604bb0; /* mov.w 0x8c604ccc,r0 */
L8c604bb0: *stop=0x8c604bb0u;r[15]+=r[0];goto L8c604bb2; /* add r0,r15 */
L8c604bb2: *stop=0x8c604bb2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c604bb4; /* fldi1 fr4 */
L8c604bb4: *stop=0x8c604bb4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[4],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c604bb6; /* fadd fr4,fr4 */
L8c604bb6: *stop=0x8c604bb6u;r[4]=rd(m,bus,&io_status,0x8c604ce0u,4);if(io_status)return io_status;goto L8c604bb8; /* mov.l 0x8c604ce0,r4 */
L8c604bb8: *stop=0x8c604bb8u;r[0]=4u;goto L8c604bba; /* mov #0x4,r0 */
L8c604bba: *stop=0x8c604bbau;r[1]=rd(m,bus,&io_status,0x8c604cdcu,4);if(io_status)return io_status;goto L8c604bbc; /* mov.l 0x8c604cdc,r1 */
L8c604bbc: *stop=0x8c604bbcu;r[13]=0u;goto L8c604bbe; /* mov #0x0,r13 */
L8c604bbe: *stop=0x8c604bbeu;r[2]=rd(m,bus,&io_status,0x8c604cd8u,4);if(io_status)return io_status;goto L8c604bc0; /* mov.l 0x8c604cd8,r2 */
L8c604bc0: *stop=0x8c604bc0u;r[3]=rd(m,bus,&io_status,0x8c604cd4u,4);if(io_status)return io_status;goto L8c604bc2; /* mov.l 0x8c604cd4,r3 */
L8c604bc2: *stop=0x8c604bc2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c604bc4; /* mov.l r13,@r3 */
L8c604bc4: *stop=0x8c604bc4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c604bc6; /* mov.l r13,@r2 */
L8c604bc6: *stop=0x8c604bc6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c604bc8; /* mov.l r13,@r1 */
L8c604bc8: *stop=0x8c604bc8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604bca; /* fmov.s @(r0,r4),fr3 */
L8c604bca: *stop=0x8c604bcau;r[0]=12u;goto L8c604bcc; /* mov #0xc,r0 */
L8c604bcc: *stop=0x8c604bccu;r[1]=rd(m,bus,&io_status,0x8c604cf0u,4);if(io_status)return io_status;goto L8c604bce; /* mov.l 0x8c604cf0,r1 */
L8c604bce: *stop=0x8c604bceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c604bd0; /* fdiv fr4,fr3 */
L8c604bd0: *stop=0x8c604bd0u;r[2]=rd(m,bus,&io_status,0x8c604ce8u,4);if(io_status)return io_status;goto L8c604bd2; /* mov.l 0x8c604ce8,r2 */
L8c604bd2: *stop=0x8c604bd2u;r[3]=rd(m,bus,&io_status,0x8c604ce4u,4);if(io_status)return io_status;goto L8c604bd4; /* mov.l 0x8c604ce4,r3 */
L8c604bd4: *stop=0x8c604bd4u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604bd6; /* fmov fr3,@(r0,r4) */
L8c604bd6: *stop=0x8c604bd6u;r[0]=8u;goto L8c604bd8; /* mov #0x8,r0 */
L8c604bd8: *stop=0x8c604bd8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604bda; /* fmov.s @(r0,r4),fr3 */
L8c604bda: *stop=0x8c604bdau;r[0]=16u;goto L8c604bdc; /* mov #0x10,r0 */
L8c604bdc: *stop=0x8c604bdcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c604bde; /* fdiv fr4,fr3 */
L8c604bde: *stop=0x8c604bdeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c604be0; /* fldi1 fr4 */
L8c604be0: *stop=0x8c604be0u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604be2; /* fmov fr3,@(r0,r4) */
L8c604be2: *stop=0x8c604be2u;r[0]=0x8c604cecu;goto L8c604be4; /* mova 0x8c604cec,r0 */
L8c604be4: *stop=0x8c604be4u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;goto L8c604be6; /* fmov.s fr4,@r3 */
L8c604be6: *stop=0x8c604be6u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c604be8; /* fmov.s fr4,@r2 */
L8c604be8: *stop=0x8c604be8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c604bea; /* fmov.s @r0,fr3 */
L8c604bea: *stop=0x8c604beau;r[0]=0x8c604cf4u;goto L8c604bec; /* mova 0x8c604cf4,r0 */
L8c604bec: *stop=0x8c604becu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c604bee; /* fmov.s fr3,@r4 */
L8c604bee: *stop=0x8c604beeu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c604bf0; /* fmov.s fr4,@r1 */
L8c604bf0: *stop=0x8c604bf0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c604bf2; /* fmov.s @r0,fr3 */
L8c604bf2: *stop=0x8c604bf2u;r[7]=sx(m,bus,&io_status,0x8c604cceu,2);if(io_status)return io_status;goto L8c604bf4; /* mov.w 0x8c604cce,r7 */
L8c604bf4: *stop=0x8c604bf4u;r[0]=4u;goto L8c604bf6; /* mov #0x4,r0 */
L8c604bf6: *stop=0x8c604bf6u;r[3]=rd(m,bus,&io_status,0x8c604cf8u,4);if(io_status)return io_status;goto L8c604bf8; /* mov.l 0x8c604cf8,r3 */
L8c604bf8: *stop=0x8c604bf8u;r[7]+=r[15];goto L8c604bfa; /* add r15,r7 */
L8c604bfa: *stop=0x8c604bfau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c604bfc; /* fldi0 fr4 */
L8c604bfc: *stop=0x8c604bfcu;r[5]=r[7];goto L8c604bfe; /* mov r7,r5 */
L8c604bfe: *stop=0x8c604bfeu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c604c00; /* fmov.s fr3,@r3 */
L8c604c00: *stop=0x8c604c00u;r[6]=r[5];goto L8c604c02; /* mov r5,r6 */
L8c604c02: *stop=0x8c604c02u;r[6]+=8u;goto L8c604c04; /* add #0x8,r6 */
L8c604c04: *stop=0x8c604c04u;r[7]=rd(m,bus,&io_status,0x8c604d08u,4);if(io_status)return io_status;goto L8c604c06; /* mov.l 0x8c604d08,r7 */
L8c604c06: *stop=0x8c604c06u;r[2]=rd(m,bus,&io_status,0x8c604cfcu,4);if(io_status)return io_status;goto L8c604c08; /* mov.l 0x8c604cfc,r2 */
L8c604c08: *stop=0x8c604c08u;r[3]=rd(m,bus,&io_status,0x8c604d04u,4);if(io_status)return io_status;goto L8c604c0a; /* mov.l 0x8c604d04,r3 */
L8c604c0a: *stop=0x8c604c0au;r[1]=rd(m,bus,&io_status,0x8c604d00u,4);if(io_status)return io_status;goto L8c604c0c; /* mov.l 0x8c604d00,r1 */
L8c604c0c: *stop=0x8c604c0cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c604c0e; /* mov.l r13,@r2 */
L8c604c0e: *stop=0x8c604c0eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c604c10; /* mov.l r13,@r1 */
L8c604c10: *stop=0x8c604c10u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c604c12; /* mov.l r13,@r3 */
L8c604c12: *stop=0x8c604c12u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;goto L8c604c14; /* fmov.s fr4,@r5 */
L8c604c14: *stop=0x8c604c14u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c604c16; /* fmov fr4,@(r0,r5) */
L8c604c16: *stop=0x8c604c16u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604c18; /* fmov.s @(r0,r4),fr3 */
L8c604c18: *stop=0x8c604c18u;r[0]=8u;goto L8c604c1a; /* mov #0x8,r0 */
L8c604c1a: *stop=0x8c604c1au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c604c1c; /* fldi1 fr4 */
L8c604c1c: *stop=0x8c604c1cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c604c1e; /* fneg fr4 */
L8c604c1e: *stop=0x8c604c1eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c604c20; /* fadd fr4,fr3 */
L8c604c20: *stop=0x8c604c20u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c604c22; /* fmov.s fr3,@r6 */
L8c604c22: *stop=0x8c604c22u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604c24; /* fmov.s @(r0,r4),fr3 */
L8c604c24: *stop=0x8c604c24u;r[0]=4u;goto L8c604c26; /* mov #0x4,r0 */
L8c604c26: *stop=0x8c604c26u;r[4]=r[7];goto L8c604c28; /* mov r7,r4 */
L8c604c28: *stop=0x8c604c28u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c604c2a; /* fadd fr4,fr3 */
L8c604c2a: *stop=0x8c604c2au;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c604c2c; /* fmov fr3,@(r0,r6) */
L8c604c2c: *stop=0x8c604c2cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c604c2e; /* fmov.s @r5,fr3 */
L8c604c2e: *stop=0x8c604c2eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c604c30; /* fmov.s fr3,@r4 */
L8c604c30: *stop=0x8c604c30u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c604c32; /* fmov.s @(r0,r5),fr2 */
L8c604c32: *stop=0x8c604c32u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c604c34; /* fmov fr2,@(r0,r4) */
L8c604c34: *stop=0x8c604c34u;r[4]+=8u;goto L8c604c36; /* add #0x8,r4 */
L8c604c36: *stop=0x8c604c36u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c604c38; /* fmov.s @r6,fr3 */
L8c604c38: *stop=0x8c604c38u;r[2]=rd(m,bus,&io_status,0x8c604d0cu,4);if(io_status)return io_status;goto L8c604c3a; /* mov.l 0x8c604d0c,r2 */
L8c604c3a: *stop=0x8c604c3au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c604c3c; /* fmov.s fr3,@r4 */
L8c604c3c: *stop=0x8c604c3cu;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c604c3e; /* fmov.s @(r0,r6),fr2 */
L8c604c3e: target=r[2];*pr=0x8c604c42u;*stop=0x8c604c40u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604c42; /* jsr @r2 */
L8c604c42: *stop=0x8c604c42u;r[4]=rd(m,bus,&io_status,0x8c604d14u,4);if(io_status)return io_status;goto L8c604c44; /* mov.l 0x8c604d14,r4 */
L8c604c44: *stop=0x8c604c44u;r[9]=sx(m,bus,&io_status,0x8c604cd0u,2);if(io_status)return io_status;goto L8c604c46; /* mov.w 0x8c604cd0,r9 */
L8c604c46: *stop=0x8c604c46u;r[14]=sx(m,bus,&io_status,0x8c604cceu,2);if(io_status)return io_status;goto L8c604c48; /* mov.w 0x8c604cce,r14 */
L8c604c48: *stop=0x8c604c48u;r[12]=r[4];goto L8c604c4a; /* mov r4,r12 */
L8c604c4a: *stop=0x8c604c4au;r[10]=rd(m,bus,&io_status,0x8c604d18u,4);if(io_status)return io_status;goto L8c604c4c; /* mov.l 0x8c604d18,r10 */
L8c604c4c: *stop=0x8c604c4cu;r[9]+=r[4];goto L8c604c4e; /* add r4,r9 */
L8c604c4e: *stop=0x8c604c4eu;r[11]=rd(m,bus,&io_status,0x8c604d10u,4);if(io_status)return io_status;goto L8c604c50; /* mov.l 0x8c604d10,r11 */
L8c604c50: target=0x8c605006u;*pr=0x8c604c54u;*stop=0x8c604c52u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604c54; /* bsr 0x8c605006 */
L8c604c54: *stop=0x8c604c54u;r[12]+=r[14];goto L8c604c56; /* add r14,r12 */
L8c604c56: *stop=0x8c604c56u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[10],4);if(io_status)return io_status;goto L8c604c58; /* mov.l r10,@r11 */
L8c604c58: *stop=0x8c604c58u;*sr=(*sr&~1u)|(r[12]>=r[9]);goto L8c604c5a; /* cmp/hs r9,r12 */
L8c604c5a: take=!(*sr&1u);*stop=0x8c604c5cu;r[11]+=4u;if(take)goto L8c604c50;goto L8c604c5e; /* bf/s 0x8c604c50 */
L8c604c5e: *stop=0x8c604c5eu;r[4]=rd(m,bus,&io_status,0x8c604d1cu,4);if(io_status)return io_status;goto L8c604c60; /* mov.l 0x8c604d1c,r4 */
L8c604c60: target=0x8c605006u;*pr=0x8c604c64u;*stop=0x8c604c62u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604c64; /* bsr 0x8c605006 */
L8c604c64: *stop=0x8c604c64u;r[4]=rd(m,bus,&io_status,0x8c604d1cu,4);if(io_status)return io_status;goto L8c604c66; /* mov.l 0x8c604d1c,r4 */
L8c604c66: *stop=0x8c604c66u;r[2]=10u;goto L8c604c68; /* mov #0xa,r2 */
L8c604c68: *stop=0x8c604c68u;r[3]=11u;goto L8c604c6a; /* mov #0xb,r3 */
L8c604c6a: *stop=0x8c604c6au;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c604c6c; /* mov.l r14,@r4 */
L8c604c6c: *stop=0x8c604c6cu;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c604c6e; /* mov.l r2,@(0x2c,r4) */
L8c604c6e: *stop=0x8c604c6eu;r[2]=rd(m,bus,&io_status,0x8c604d20u,4);if(io_status)return io_status;goto L8c604c70; /* mov.l 0x8c604d20,r2 */
L8c604c70: target=r[2];*pr=0x8c604c74u;*stop=0x8c604c72u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604c74; /* jsr @r2 */
L8c604c74: *stop=0x8c604c74u;r[3]=rd(m,bus,&io_status,0x8c604d24u,4);if(io_status)return io_status;goto L8c604c76; /* mov.l 0x8c604d24,r3 */
L8c604c76: *stop=0x8c604c76u;r[4]=rd(m,bus,&io_status,0x8c604d1cu,4);if(io_status)return io_status;goto L8c604c78; /* mov.l 0x8c604d1c,r4 */
L8c604c78: target=r[3];*pr=0x8c604c7cu;*stop=0x8c604c7au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604c7c; /* jsr @r3 */
L8c604c7c: *stop=0x8c604c7cu;r[2]=rd(m,bus,&io_status,0x8c604d28u,4);if(io_status)return io_status;goto L8c604c7e; /* mov.l 0x8c604d28,r2 */
L8c604c7e: target=r[2];*pr=0x8c604c82u;*stop=0x8c604c80u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604c82; /* jsr @r2 */
L8c604c82: *stop=0x8c604c82u;r[3]=rd(m,bus,&io_status,0x8c604d30u,4);if(io_status)return io_status;goto L8c604c84; /* mov.l 0x8c604d30,r3 */
L8c604c84: *stop=0x8c604c84u;r[0]=0x8c604d2cu;goto L8c604c86; /* mova 0x8c604d2c,r0 */
L8c604c86: target=r[3];*pr=0x8c604c8au;*stop=0x8c604c88u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604c8a; /* jsr @r3 */
L8c604c8a: target=0x8c604fd0u;*pr=0x8c604c8eu;*stop=0x8c604c8cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604c8e; /* bsr 0x8c604fd0 */
L8c604c8e: *stop=0x8c604c8eu;r[0]=sx(m,bus,&io_status,0x8c604cceu,2);if(io_status)return io_status;goto L8c604c90; /* mov.w 0x8c604cce,r0 */
L8c604c90: *stop=0x8c604c90u;r[6]=r[15];goto L8c604c92; /* mov r15,r6 */
L8c604c92: *stop=0x8c604c92u;r[3]=rd(m,bus,&io_status,0x8c604d38u,4);if(io_status)return io_status;goto L8c604c94; /* mov.l 0x8c604d38,r3 */
L8c604c94: *stop=0x8c604c94u;r[2]=rd(m,bus,&io_status,0x8c604d34u,4);if(io_status)return io_status;goto L8c604c96; /* mov.l 0x8c604d34,r2 */
L8c604c96: target=r[3];*pr=0x8c604c9au;*stop=0x8c604c98u;r[1]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604c9a; /* jsr @r3 */
L8c604c9a: *stop=0x8c604c9au;r[5]=rd(m,bus,&io_status,0x8c604d3cu,4);if(io_status)return io_status;goto L8c604c9c; /* mov.l 0x8c604d3c,r5 */
L8c604c9c: *stop=0x8c604c9cu;r[4]=r[13];goto L8c604c9e; /* mov r13,r4 */
L8c604c9e: *stop=0x8c604c9eu;r[0]=r[4];goto L8c604ca0; /* mov r4,r0 */
L8c604ca0: *stop=0x8c604ca0u;r[4]+=4u;goto L8c604ca2; /* add #0x4,r4 */
L8c604ca2: *stop=0x8c604ca2u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c604ca4; /* fmov.s @(r0,r6),fr3 */
L8c604ca4: *stop=0x8c604ca4u;*sr=(*sr&~1u)|(r[4]>=r[14]);goto L8c604ca6; /* cmp/hs r14,r4 */
L8c604ca6: take=!(*sr&1u);*stop=0x8c604ca8u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;if(take)goto L8c604c9e;goto L8c604caa; /* bf/s 0x8c604c9e */
L8c604caa: *stop=0x8c604caau;r[0]=sx(m,bus,&io_status,0x8c604cceu,2);if(io_status)return io_status;goto L8c604cac; /* mov.w 0x8c604cce,r0 */
L8c604cac: *stop=0x8c604cacu;r[7]=3u;goto L8c604cae; /* mov #0x3,r7 */
L8c604cae: *stop=0x8c604caeu;r[2]=rd(m,bus,&io_status,0x8c604d40u,4);if(io_status)return io_status;goto L8c604cb0; /* mov.l 0x8c604d40,r2 */
L8c604cb0: *stop=0x8c604cb0u;r[5]+=64u;goto L8c604cb2; /* add #0x40,r5 */
L8c604cb2: *stop=0x8c604cb2u;r[3]=rd(m,bus,&io_status,0x8c604d38u,4);if(io_status)return io_status;goto L8c604cb4; /* mov.l 0x8c604d38,r3 */
L8c604cb4: *stop=0x8c604cb4u;r[6]=r[15];goto L8c604cb6; /* mov r15,r6 */
L8c604cb6: *stop=0x8c604cb6u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[7],4);if(io_status)return io_status;goto L8c604cb8; /* mov.l r7,@r5 */
L8c604cb8: target=r[3];*pr=0x8c604cbcu;*stop=0x8c604cbau;r[1]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604cbc; /* jsr @r3 */
L8c604cbc: *stop=0x8c604cbcu;r[5]=rd(m,bus,&io_status,0x8c604d44u,4);if(io_status)return io_status;goto L8c604cbe; /* mov.l 0x8c604d44,r5 */
L8c604cbe: *stop=0x8c604cbeu;r[4]=r[13];goto L8c604cc0; /* mov r13,r4 */
L8c604cc0: *stop=0x8c604cc0u;r[0]=r[4];goto L8c604cc2; /* mov r4,r0 */
L8c604cc2: *stop=0x8c604cc2u;r[4]+=4u;goto L8c604cc4; /* add #0x4,r4 */
L8c604cc4: *stop=0x8c604cc4u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c604cc6; /* fmov.s @(r0,r6),fr3 */
L8c604cc6: *stop=0x8c604cc6u;*sr=(*sr&~1u)|(r[4]>=r[14]);goto L8c604cc8; /* cmp/hs r14,r4 */
L8c604cc8: *stop=0x8c604ccau;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c604d48; /* bra 0x8c604d48 */
L8c604d48: take=!(*sr&1u);*stop=0x8c604d4au;if(take)goto L8c604cc0;goto L8c604d4c; /* bf/s 0x8c604cc0 */
L8c604d4c: *stop=0x8c604d4cu;r[2]=rd(m,bus,&io_status,0x8c604e34u,4);if(io_status)return io_status;goto L8c604d4e; /* mov.l 0x8c604e34,r2 */
L8c604d4e: *stop=0x8c604d4eu;r[5]+=64u;goto L8c604d50; /* add #0x40,r5 */
L8c604d50: *stop=0x8c604d50u;r[1]=sx(m,bus,&io_status,0x8c604e30u,2);if(io_status)return io_status;goto L8c604d52; /* mov.w 0x8c604e30,r1 */
L8c604d52: *stop=0x8c604d52u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[7],4);if(io_status)return io_status;goto L8c604d54; /* mov.l r7,@r5 */
L8c604d54: *stop=0x8c604d54u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c604d56; /* mov.l r13,@r2 */
L8c604d56: *stop=0x8c604d56u;r[15]+=r[1];goto L8c604d58; /* add r1,r15 */
L8c604d58: *stop=0x8c604d58u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c604d5a; /* lds.l @r15+,PR */
L8c604d5a: *stop=0x8c604d5au;if(!valid_access(r[15],4u))return -3;r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c604d5c; /* mov.l @r15+,r9 */
L8c604d5c: *stop=0x8c604d5cu;if(!valid_access(r[15],4u))return -3;r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c604d5e; /* mov.l @r15+,r10 */
L8c604d5e: *stop=0x8c604d5eu;if(!valid_access(r[15],4u))return -3;r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c604d60; /* mov.l @r15+,r11 */
L8c604d60: *stop=0x8c604d60u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c604d62; /* mov.l @r15+,r12 */
L8c604d62: *stop=0x8c604d62u;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c604d64; /* mov.l @r15+,r13 */
L8c604d64: *stop=0x8c604d64u;if(*pr!=entry_pr)return -3;*stop=0x8c604d66u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_geometry_stream_8c604d68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c604d68;
L8c604d68: *stop=0x8c604d68u;r[15]+=4294967280u;goto L8c604d6a; /* add #-0x10,r15 */
L8c604d6a: *stop=0x8c604d6au;r[1]=rd(m,bus,&io_status,0x8c604e40u,4);if(io_status)return io_status;goto L8c604d6c; /* mov.l 0x8c604e40,r1 */
L8c604d6c: *stop=0x8c604d6cu;r[4]=0u;goto L8c604d6e; /* mov #0x0,r4 */
L8c604d6e: *stop=0x8c604d6eu;r[2]=rd(m,bus,&io_status,0x8c604e3cu,4);if(io_status)return io_status;goto L8c604d70; /* mov.l 0x8c604e3c,r2 */
L8c604d70: *stop=0x8c604d70u;r[0]=4u;goto L8c604d72; /* mov #0x4,r0 */
L8c604d72: *stop=0x8c604d72u;r[3]=rd(m,bus,&io_status,0x8c604e38u,4);if(io_status)return io_status;goto L8c604d74; /* mov.l 0x8c604e38,r3 */
L8c604d74: *stop=0x8c604d74u;r[7]=r[15];goto L8c604d76; /* mov r15,r7 */
L8c604d76: *stop=0x8c604d76u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c604d78; /* fldi1 fr4 */
L8c604d78: *stop=0x8c604d78u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[4],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c604d7a; /* fadd fr4,fr4 */
L8c604d7a: *stop=0x8c604d7au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c604d7c; /* mov.l r4,@r3 */
L8c604d7c: *stop=0x8c604d7cu;r[5]=r[7];goto L8c604d7e; /* mov r7,r5 */
L8c604d7e: *stop=0x8c604d7eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c604d80; /* mov.l r4,@r2 */
L8c604d80: *stop=0x8c604d80u;r[6]=r[5];goto L8c604d82; /* mov r5,r6 */
L8c604d82: *stop=0x8c604d82u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c604d84; /* mov.l r4,@r1 */
L8c604d84: *stop=0x8c604d84u;r[6]+=8u;goto L8c604d86; /* add #0x8,r6 */
L8c604d86: *stop=0x8c604d86u;r[4]=rd(m,bus,&io_status,0x8c604e44u,4);if(io_status)return io_status;goto L8c604d88; /* mov.l 0x8c604e44,r4 */
L8c604d88: *stop=0x8c604d88u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604d8a; /* fmov.s @(r0,r4),fr3 */
L8c604d8a: *stop=0x8c604d8au;r[0]=12u;goto L8c604d8c; /* mov #0xc,r0 */
L8c604d8c: *stop=0x8c604d8cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c604d8e; /* fdiv fr4,fr3 */
L8c604d8e: *stop=0x8c604d8eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604d90; /* fmov fr3,@(r0,r4) */
L8c604d90: *stop=0x8c604d90u;r[0]=8u;goto L8c604d92; /* mov #0x8,r0 */
L8c604d92: *stop=0x8c604d92u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604d94; /* fmov.s @(r0,r4),fr3 */
L8c604d94: *stop=0x8c604d94u;r[0]=16u;goto L8c604d96; /* mov #0x10,r0 */
L8c604d96: *stop=0x8c604d96u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c604d98; /* fdiv fr4,fr3 */
L8c604d98: *stop=0x8c604d98u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c604d9a; /* fldi0 fr4 */
L8c604d9a: *stop=0x8c604d9au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604d9c; /* fmov fr3,@(r0,r4) */
L8c604d9c: *stop=0x8c604d9cu;r[0]=4u;goto L8c604d9e; /* mov #0x4,r0 */
L8c604d9e: *stop=0x8c604d9eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;goto L8c604da0; /* fmov.s fr4,@r5 */
L8c604da0: *stop=0x8c604da0u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c604da2; /* fmov fr4,@(r0,r5) */
L8c604da2: *stop=0x8c604da2u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604da4; /* fmov.s @(r0,r4),fr3 */
L8c604da4: *stop=0x8c604da4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c604da6; /* fldi1 fr4 */
L8c604da6: *stop=0x8c604da6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c604da8; /* fneg fr4 */
L8c604da8: *stop=0x8c604da8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c604daa; /* fadd fr4,fr3 */
L8c604daa: *stop=0x8c604daau;r[0]=8u;goto L8c604dac; /* mov #0x8,r0 */
L8c604dac: *stop=0x8c604dacu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c604dae; /* fmov.s fr3,@r6 */
L8c604dae: *stop=0x8c604daeu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c604db0; /* fmov.s @(r0,r4),fr3 */
L8c604db0: *stop=0x8c604db0u;r[0]=4u;goto L8c604db2; /* mov #0x4,r0 */
L8c604db2: *stop=0x8c604db2u;r[4]=rd(m,bus,&io_status,0x8c604e48u,4);if(io_status)return io_status;goto L8c604db4; /* mov.l 0x8c604e48,r4 */
L8c604db4: *stop=0x8c604db4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c604db6; /* fadd fr4,fr3 */
L8c604db6: *stop=0x8c604db6u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c604db8; /* fmov fr3,@(r0,r6) */
L8c604db8: *stop=0x8c604db8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c604dba; /* fmov.s @r5,fr3 */
L8c604dba: *stop=0x8c604dbau;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c604dbc; /* fmov.s fr3,@r4 */
L8c604dbc: *stop=0x8c604dbcu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c604dbe; /* fmov.s @(r0,r5),fr2 */
L8c604dbe: *stop=0x8c604dbeu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c604dc0; /* fmov fr2,@(r0,r4) */
L8c604dc0: *stop=0x8c604dc0u;r[4]+=8u;goto L8c604dc2; /* add #0x8,r4 */
L8c604dc2: *stop=0x8c604dc2u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c604dc4; /* fmov.s @r6,fr3 */
L8c604dc4: *stop=0x8c604dc4u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c604dc6; /* fmov.s fr3,@r4 */
L8c604dc6: *stop=0x8c604dc6u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c604dc8; /* fmov.s @(r0,r6),fr2 */
L8c604dc8: *stop=0x8c604dc8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c604dca; /* fmov fr2,@(r0,r4) */
L8c604dca: *stop=0x8c604dcau;if(*pr!=entry_pr)return -3;*stop=0x8c604dccu;r[15]+=16u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_geometry_stream_8c0544e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0544e2;
L8c0544e2: *stop=0x8c0544e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0544e4; /* sts.l PR,@-r15 */
L8c0544e4: *stop=0x8c0544e4u;r[15]+=4294967280u;goto L8c0544e6; /* add #-0x10,r15 */
L8c0544e6: *stop=0x8c0544e6u;r[4]=rd(m,bus,&io_status,0x8c05461cu,4);if(io_status)return io_status;goto L8c0544e8; /* mov.l 0x8c05461c,r4 */
L8c0544e8: *stop=0x8c0544e8u;r[5]=4294967168u;goto L8c0544ea; /* mov #-0x80,r5 */
L8c0544ea: *stop=0x8c0544eau;r[3]=rd(m,bus,&io_status,0x8c054620u,4);if(io_status)return io_status;goto L8c0544ec; /* mov.l 0x8c054620,r3 */
L8c0544ec: *stop=0x8c0544ecu;r[5]=r[5]&255u;goto L8c0544ee; /* extu.b r5,r5 */
L8c0544ee: target=r[3];*pr=0x8c0544f2u;*stop=0x8c0544f0u;r[6]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0544f2; /* jsr @r3 */
L8c0544f2: *stop=0x8c0544f2u;r[0]=0x8c054628u;goto L8c0544f4; /* mova 0x8c054628,r0 */
L8c0544f4: *stop=0x8c0544f4u;r[4]=rd(m,bus,&io_status,0x8c054624u,4);if(io_status)return io_status;goto L8c0544f6; /* mov.l 0x8c054624,r4 */
L8c0544f6: *stop=0x8c0544f6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0544f8; /* fmov.s @r0,fr3 */
L8c0544f8: *stop=0x8c0544f8u;r[0]=4u;goto L8c0544fa; /* mov #0x4,r0 */
L8c0544fa: *stop=0x8c0544fau;r[3]=rd(m,bus,&io_status,0x8c054634u,4);if(io_status)return io_status;goto L8c0544fc; /* mov.l 0x8c054634,r3 */
L8c0544fc: *stop=0x8c0544fcu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0544fe; /* fmov fr3,@(r0,r4) */
L8c0544fe: *stop=0x8c0544feu;r[0]=0x8c05462cu;goto L8c054500; /* mova 0x8c05462c,r0 */
L8c054500: *stop=0x8c054500u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c054502; /* fmov.s @r0,fr3 */
L8c054502: *stop=0x8c054502u;r[0]=8u;goto L8c054504; /* mov #0x8,r0 */
L8c054504: *stop=0x8c054504u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c054506; /* fmov fr3,@(r0,r4) */
L8c054506: *stop=0x8c054506u;r[0]=0x8c054630u;goto L8c054508; /* mova 0x8c054630,r0 */
L8c054508: *stop=0x8c054508u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c05450a; /* fmov.s @r0,fr4 */
L8c05450a: *stop=0x8c05450au;r[0]=4u;goto L8c05450c; /* mov #0x4,r0 */
L8c05450c: *stop=0x8c05450cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c05450e; /* fmov.s @(r0,r4),fr3 */
L8c05450e: *stop=0x8c05450eu;r[0]=12u;goto L8c054510; /* mov #0xc,r0 */
L8c054510: *stop=0x8c054510u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c054512; /* fmul fr4,fr3 */
L8c054512: *stop=0x8c054512u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c054514; /* fmov fr3,@(r0,r4) */
L8c054514: *stop=0x8c054514u;r[0]=8u;goto L8c054516; /* mov #0x8,r0 */
L8c054516: *stop=0x8c054516u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c054518; /* fmov.s @(r0,r4),fr3 */
L8c054518: *stop=0x8c054518u;r[0]=16u;goto L8c05451a; /* mov #0x10,r0 */
L8c05451a: *stop=0x8c05451au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c05451c; /* fmul fr4,fr3 */
L8c05451c: target=r[3];*pr=0x8c054520u;*stop=0x8c05451eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054520; /* jsr @r3 */
L8c054520: *stop=0x8c054520u;r[4]=50u;goto L8c054522; /* mov #0x32,r4 */
L8c054522: *stop=0x8c054522u;r[2]=rd(m,bus,&io_status,0x8c054638u,4);if(io_status)return io_status;goto L8c054524; /* mov.l 0x8c054638,r2 */
L8c054524: *stop=0x8c054524u;r[4]<<=8;goto L8c054526; /* shll8 r4 */
L8c054526: *stop=0x8c054526u;r[4]+=4294967239u;goto L8c054528; /* add #-0x39,r4 */
L8c054528: target=r[2];*pr=0x8c05452cu;*stop=0x8c05452au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05452c; /* jsr @r2 */
L8c05452c: *stop=0x8c05452cu;r[3]=rd(m,bus,&io_status,0x8c054640u,4);if(io_status)return io_status;goto L8c05452e; /* mov.l 0x8c054640,r3 */
L8c05452e: *stop=0x8c05452eu;r[4]=rd(m,bus,&io_status,0x8c05463cu,4);if(io_status)return io_status;goto L8c054530; /* mov.l 0x8c05463c,r4 */
L8c054530: target=r[3];*pr=0x8c054534u;*stop=0x8c054532u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054534; /* jsr @r3 */
L8c054534: *stop=0x8c054534u;r[2]=rd(m,bus,&io_status,0x8c054644u,4);if(io_status)return io_status;goto L8c054536; /* mov.l 0x8c054644,r2 */
L8c054536: *stop=0x8c054536u;r[4]=rd(m,bus,&io_status,0x8c05463cu,4);if(io_status)return io_status;goto L8c054538; /* mov.l 0x8c05463c,r4 */
L8c054538: target=r[2];*pr=0x8c05453cu;*stop=0x8c05453au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05453c; /* jsr @r2 */
L8c05453c: *stop=0x8c05453cu;r[4]=rd(m,bus,&io_status,0x8c054648u,4);if(io_status)return io_status;goto L8c05453e; /* mov.l 0x8c054648,r4 */
L8c05453e: *stop=0x8c05453eu;r[5]=8u;goto L8c054540; /* mov #0x8,r5 */
L8c054540: *stop=0x8c054540u;r[3]=rd(m,bus,&io_status,0x8c05464cu,4);if(io_status)return io_status;goto L8c054542; /* mov.l 0x8c05464c,r3 */
L8c054542: *stop=0x8c054542u;r[5]<<=8;goto L8c054544; /* shll8 r5 */
L8c054544: target=r[3];*pr=0x8c054548u;*stop=0x8c054546u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054548; /* jsr @r3 */
L8c054548: *stop=0x8c054548u;r[2]=rd(m,bus,&io_status,0x8c054650u,4);if(io_status)return io_status;goto L8c05454a; /* mov.l 0x8c054650,r2 */
L8c05454a: *stop=0x8c05454au;r[6]=0u;goto L8c05454c; /* mov #0x0,r6 */
L8c05454c: *stop=0x8c05454cu;r[5]=r[6];goto L8c05454e; /* mov r6,r5 */
L8c05454e: target=r[2];*pr=0x8c054552u;*stop=0x8c054550u;r[4]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054552; /* jsr @r2 */
L8c054552: *stop=0x8c054552u;r[3]=rd(m,bus,&io_status,0x8c054654u,4);if(io_status)return io_status;goto L8c054554; /* mov.l 0x8c054654,r3 */
L8c054554: target=r[3];*pr=0x8c054558u;*stop=0x8c054556u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054558; /* jsr @r3 */
L8c054558: *stop=0x8c054558u;r[2]=rd(m,bus,&io_status,0x8c054658u,4);if(io_status)return io_status;goto L8c05455a; /* mov.l 0x8c054658,r2 */
L8c05455a: target=r[2];*pr=0x8c05455eu;*stop=0x8c05455cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05455e; /* jsr @r2 */
L8c05455e: *stop=0x8c05455eu;r[3]=rd(m,bus,&io_status,0x8c05465cu,4);if(io_status)return io_status;goto L8c054560; /* mov.l 0x8c05465c,r3 */
L8c054560: target=r[3];*pr=0x8c054564u;*stop=0x8c054562u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054564; /* jsr @r3 */
L8c054564: *stop=0x8c054564u;r[2]=rd(m,bus,&io_status,0x8c054660u,4);if(io_status)return io_status;goto L8c054566; /* mov.l 0x8c054660,r2 */
L8c054566: target=r[2];*pr=0x8c05456au;*stop=0x8c054568u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05456a; /* jsr @r2 */
L8c05456a: *stop=0x8c05456au;r[4]=3u;goto L8c05456c; /* mov #0x3,r4 */
L8c05456c: *stop=0x8c05456cu;r[3]=rd(m,bus,&io_status,0x8c054664u,4);if(io_status)return io_status;goto L8c05456e; /* mov.l 0x8c054664,r3 */
L8c05456e: *stop=0x8c05456eu;r[4]<<=8;goto L8c054570; /* shll8 r4 */
L8c054570: *stop=0x8c054570u;r[4]+=108u;goto L8c054572; /* add #0x6c,r4 */
L8c054572: target=r[3];*pr=0x8c054576u;*stop=0x8c054574u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054576; /* jsr @r3 */
L8c054576: *stop=0x8c054576u;r[2]=rd(m,bus,&io_status,0x8c054668u,4);if(io_status)return io_status;goto L8c054578; /* mov.l 0x8c054668,r2 */
L8c054578: *stop=0x8c054578u;r[5]=32u;goto L8c05457a; /* mov #0x20,r5 */
L8c05457a: *stop=0x8c05457au;r[5]<<=8;goto L8c05457c; /* shll8 r5 */
L8c05457c: target=r[2];*pr=0x8c054580u;*stop=0x8c05457eu;r[4]=4294967295u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054580; /* jsr @r2 */
L8c054580: *stop=0x8c054580u;r[0]=12u;goto L8c054582; /* mov #0xc,r0 */
L8c054582: *stop=0x8c054582u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c054584; /* fldi0 fr3 */
L8c054584: *stop=0x8c054584u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c054586; /* fmov fr3,@(r0,r15) */
L8c054586: *stop=0x8c054586u;r[0]=8u;goto L8c054588; /* mov #0x8,r0 */
L8c054588: *stop=0x8c054588u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c05458a; /* fmov fr3,@(r0,r15) */
L8c05458a: *stop=0x8c05458au;r[0]=4u;goto L8c05458c; /* mov #0x4,r0 */
L8c05458c: *stop=0x8c05458cu;r[3]=rd(m,bus,&io_status,0x8c05466cu,4);if(io_status)return io_status;goto L8c05458e; /* mov.l 0x8c05466c,r3 */
L8c05458e: *stop=0x8c05458eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c054590; /* fmov fr3,@(r0,r15) */
L8c054590: *stop=0x8c054590u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c054592; /* fmov.s fr3,@r15 */
L8c054592: target=r[3];*pr=0x8c054596u;*stop=0x8c054594u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054596; /* jsr @r3 */
L8c054596: *stop=0x8c054596u;r[0]=0x8c054670u;goto L8c054598; /* mova 0x8c054670,r0 */
L8c054598: *stop=0x8c054598u;r[3]=rd(m,bus,&io_status,0x8c054678u,4);if(io_status)return io_status;goto L8c05459a; /* mov.l 0x8c054678,r3 */
L8c05459a: *stop=0x8c05459au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c05459c; /* fmov.s @r0,fr5 */
L8c05459c: *stop=0x8c05459cu;r[0]=0x8c054674u;goto L8c05459e; /* mova 0x8c054674,r0 */
L8c05459e: target=r[3];*pr=0x8c0545a2u;*stop=0x8c0545a0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545a2; /* jsr @r3 */
L8c0545a2: *stop=0x8c0545a2u;r[0]=0x8c05467cu;goto L8c0545a4; /* mova 0x8c05467c,r0 */
L8c0545a4: *stop=0x8c0545a4u;r[3]=rd(m,bus,&io_status,0x8c054684u,4);if(io_status)return io_status;goto L8c0545a6; /* mov.l 0x8c054684,r3 */
L8c0545a6: *stop=0x8c0545a6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c0545a8; /* fmov.s @r0,fr5 */
L8c0545a8: *stop=0x8c0545a8u;r[0]=0x8c054680u;goto L8c0545aa; /* mova 0x8c054680,r0 */
L8c0545aa: target=r[3];*pr=0x8c0545aeu;*stop=0x8c0545acu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545ae; /* jsr @r3 */
L8c0545ae: *stop=0x8c0545aeu;r[2]=rd(m,bus,&io_status,0x8c054688u,4);if(io_status)return io_status;goto L8c0545b0; /* mov.l 0x8c054688,r2 */
L8c0545b0: target=r[2];*pr=0x8c0545b4u;*stop=0x8c0545b2u;r[4]=4294967295u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545b4; /* jsr @r2 */
L8c0545b4: *stop=0x8c0545b4u;r[3]=rd(m,bus,&io_status,0x8c054690u,4);if(io_status)return io_status;goto L8c0545b6; /* mov.l 0x8c054690,r3 */
L8c0545b6: *stop=0x8c0545b6u;r[4]=rd(m,bus,&io_status,0x8c05468cu,4);if(io_status)return io_status;goto L8c0545b8; /* mov.l 0x8c05468c,r4 */
L8c0545b8: target=r[3];*pr=0x8c0545bcu;*stop=0x8c0545bau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545bc; /* jsr @r3 */
L8c0545bc: *stop=0x8c0545bcu;r[2]=rd(m,bus,&io_status,0x8c054698u,4);if(io_status)return io_status;goto L8c0545be; /* mov.l 0x8c054698,r2 */
L8c0545be: *stop=0x8c0545beu;r[4]=rd(m,bus,&io_status,0x8c054694u,4);if(io_status)return io_status;goto L8c0545c0; /* mov.l 0x8c054694,r4 */
L8c0545c0: target=r[2];*pr=0x8c0545c4u;*stop=0x8c0545c2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545c4; /* jsr @r2 */
L8c0545c4: *stop=0x8c0545c4u;r[3]=rd(m,bus,&io_status,0x8c05469cu,4);if(io_status)return io_status;goto L8c0545c6; /* mov.l 0x8c05469c,r3 */
L8c0545c6: *stop=0x8c0545c6u;r[4]=rd(m,bus,&io_status,0x8c054694u,4);if(io_status)return io_status;goto L8c0545c8; /* mov.l 0x8c054694,r4 */
L8c0545c8: target=r[3];*pr=0x8c0545ccu;*stop=0x8c0545cau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545cc; /* jsr @r3 */
L8c0545cc: *stop=0x8c0545ccu;r[2]=rd(m,bus,&io_status,0x8c0546a0u,4);if(io_status)return io_status;goto L8c0545ce; /* mov.l 0x8c0546a0,r2 */
L8c0545ce: target=r[2];*pr=0x8c0545d2u;*stop=0x8c0545d0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545d2; /* jsr @r2 */
L8c0545d2: *stop=0x8c0545d2u;r[3]=rd(m,bus,&io_status,0x8c0546a4u,4);if(io_status)return io_status;goto L8c0545d4; /* mov.l 0x8c0546a4,r3 */
L8c0545d4: target=r[3];*pr=0x8c0545d8u;*stop=0x8c0545d6u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545d8; /* jsr @r3 */
L8c0545d8: *stop=0x8c0545d8u;r[3]=rd(m,bus,&io_status,0x8c0546acu,4);if(io_status)return io_status;goto L8c0545da; /* mov.l 0x8c0546ac,r3 */
L8c0545da: *stop=0x8c0545dau;r[0]=0x8c0546a8u;goto L8c0545dc; /* mova 0x8c0546a8,r0 */
L8c0545dc: target=r[3];*pr=0x8c0545e0u;*stop=0x8c0545deu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545e0; /* jsr @r3 */
L8c0545e0: *stop=0x8c0545e0u;r[2]=rd(m,bus,&io_status,0x8c0546b4u,4);if(io_status)return io_status;goto L8c0545e2; /* mov.l 0x8c0546b4,r2 */
L8c0545e2: *stop=0x8c0545e2u;r[4]=rd(m,bus,&io_status,0x8c0546b0u,4);if(io_status)return io_status;goto L8c0545e4; /* mov.l 0x8c0546b0,r4 */
L8c0545e4: target=r[2];*pr=0x8c0545e8u;*stop=0x8c0545e6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545e8; /* jsr @r2 */
L8c0545e8: *stop=0x8c0545e8u;r[3]=rd(m,bus,&io_status,0x8c0546b8u,4);if(io_status)return io_status;goto L8c0545ea; /* mov.l 0x8c0546b8,r3 */
L8c0545ea: target=r[3];*pr=0x8c0545eeu;*stop=0x8c0545ecu;r[4]=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545ee; /* jsr @r3 */
L8c0545ee: *stop=0x8c0545eeu;r[2]=rd(m,bus,&io_status,0x8c0546bcu,4);if(io_status)return io_status;goto L8c0545f0; /* mov.l 0x8c0546bc,r2 */
L8c0545f0: target=r[2];*pr=0x8c0545f4u;*stop=0x8c0545f2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0545f4; /* jsr @r2 */
L8c0545f4: *stop=0x8c0545f4u;r[15]+=16u;goto L8c0545f6; /* add #0x10,r15 */
L8c0545f6: *stop=0x8c0545f6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0545f8; /* lds.l @r15+,PR */
L8c0545f8: *stop=0x8c0545f8u;if(*pr!=entry_pr)return -3;*stop=0x8c0545fau;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_geometry_stream_8c10b840(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10b840;
L8c10b840: *stop=0x8c10b840u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10b842; /* sts.l PR,@-r15 */
L8c10b842: *stop=0x8c10b842u;r[15]+=4294967292u;goto L8c10b844; /* add #-0x4,r15 */
L8c10b844: *stop=0x8c10b844u;r[3]=rd(m,bus,&io_status,0x8c10b920u,4);if(io_status)return io_status;goto L8c10b846; /* mov.l 0x8c10b920,r3 */
L8c10b846: target=r[3];*pr=0x8c10b84au;*stop=0x8c10b848u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b84a; /* jsr @r3 */
L8c10b84a: *stop=0x8c10b84au;r[2]=rd(m,bus,&io_status,0x8c10b924u,4);if(io_status)return io_status;goto L8c10b84c; /* mov.l 0x8c10b924,r2 */
L8c10b84c: target=r[2];*pr=0x8c10b850u;*stop=0x8c10b84eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b850; /* jsr @r2 */
L8c10b850: *stop=0x8c10b850u;r[3]=rd(m,bus,&io_status,0x8c10b928u,4);if(io_status)return io_status;goto L8c10b852; /* mov.l 0x8c10b928,r3 */
L8c10b852: *stop=0x8c10b852u;r[6]=0u;goto L8c10b854; /* mov #0x0,r6 */
L8c10b854: *stop=0x8c10b854u;r[5]=r[6];goto L8c10b856; /* mov r6,r5 */
L8c10b856: target=r[3];*pr=0x8c10b85au;*stop=0x8c10b858u;r[4]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b85a; /* jsr @r3 */
L8c10b85a: *stop=0x8c10b85au;r[2]=rd(m,bus,&io_status,0x8c10b92cu,4);if(io_status)return io_status;goto L8c10b85c; /* mov.l 0x8c10b92c,r2 */
L8c10b85c: target=r[2];*pr=0x8c10b860u;*stop=0x8c10b85eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b860; /* jsr @r2 */
L8c10b860: *stop=0x8c10b860u;r[3]=rd(m,bus,&io_status,0x8c10b934u,4);if(io_status)return io_status;goto L8c10b862; /* mov.l 0x8c10b934,r3 */
L8c10b862: *stop=0x8c10b862u;r[4]=rd(m,bus,&io_status,0x8c10b930u,4);if(io_status)return io_status;goto L8c10b864; /* mov.l 0x8c10b930,r4 */
L8c10b864: target=r[3];*pr=0x8c10b868u;*stop=0x8c10b866u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b868; /* jsr @r3 */
L8c10b868: *stop=0x8c10b868u;r[2]=rd(m,bus,&io_status,0x8c10b940u,4);if(io_status)return io_status;goto L8c10b86a; /* mov.l 0x8c10b940,r2 */
L8c10b86a: *stop=0x8c10b86au;r[4]=rd(m,bus,&io_status,0x8c10b93cu,4);if(io_status)return io_status;goto L8c10b86c; /* mov.l 0x8c10b93c,r4 */
L8c10b86c: *stop=0x8c10b86cu;r[5]=rd(m,bus,&io_status,0x8c10b938u,4);if(io_status)return io_status;goto L8c10b86e; /* mov.l 0x8c10b938,r5 */
L8c10b86e: target=r[2];*pr=0x8c10b872u;*stop=0x8c10b870u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b872; /* jsr @r2 */
L8c10b872: *stop=0x8c10b872u;r[2]=rd(m,bus,&io_status,0x8c10b948u,4);if(io_status)return io_status;goto L8c10b874; /* mov.l 0x8c10b948,r2 */
L8c10b874: *stop=0x8c10b874u;r[3]=rd(m,bus,&io_status,0x8c10b944u,4);if(io_status)return io_status;goto L8c10b876; /* mov.l 0x8c10b944,r3 */
L8c10b876: *stop=0x8c10b876u;r[2]|=r[3];goto L8c10b878; /* or r3,r2 */
L8c10b878: target=r[2];*pr=0x8c10b87cu;*stop=0x8c10b87au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b87c; /* jsr @r2 */
L8c10b87c: *stop=0x8c10b87cu;r[3]=rd(m,bus,&io_status,0x8c10b938u,4);if(io_status)return io_status;goto L8c10b87e; /* mov.l 0x8c10b938,r3 */
L8c10b87e: target=r[3];*pr=0x8c10b882u;*stop=0x8c10b880u;if(!valid_access(r[15],4u))return -3;r[4]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b882; /* jsr @r3 */
L8c10b882: *stop=0x8c10b882u;r[3]=rd(m,bus,&io_status,0x8c10b94cu,4);if(io_status)return io_status;goto L8c10b884; /* mov.l 0x8c10b94c,r3 */
L8c10b884: target=r[3];*pr=0x8c10b888u;*stop=0x8c10b886u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b888; /* jsr @r3 */
L8c10b888: *stop=0x8c10b888u;r[2]=rd(m,bus,&io_status,0x8c10b950u,4);if(io_status)return io_status;goto L8c10b88a; /* mov.l 0x8c10b950,r2 */
L8c10b88a: target=r[2];*pr=0x8c10b88eu;*stop=0x8c10b88cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b88e; /* jsr @r2 */
L8c10b88e: *stop=0x8c10b88eu;r[3]=rd(m,bus,&io_status,0x8c10b934u,4);if(io_status)return io_status;goto L8c10b890; /* mov.l 0x8c10b934,r3 */
L8c10b890: *stop=0x8c10b890u;r[4]=rd(m,bus,&io_status,0x8c10b954u,4);if(io_status)return io_status;goto L8c10b892; /* mov.l 0x8c10b954,r4 */
L8c10b892: target=r[3];*pr=0x8c10b896u;*stop=0x8c10b894u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b896; /* jsr @r3 */
L8c10b896: *stop=0x8c10b896u;r[3]=rd(m,bus,&io_status,0x8c10b958u,4);if(io_status)return io_status;goto L8c10b898; /* mov.l 0x8c10b958,r3 */
L8c10b898: *stop=0x8c10b898u;r[2]=0u;goto L8c10b89a; /* mov #0x0,r2 */
L8c10b89a: *stop=0x8c10b89au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c10b89c; /* mov.l r2,@r3 */
L8c10b89c: *stop=0x8c10b89cu;if(!valid_access(r[15],4u))return -3;r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c10b89e; /* mov.l @r15,r0 */
L8c10b89e: *stop=0x8c10b89eu;r[15]+=4u;goto L8c10b8a0; /* add #0x4,r15 */
L8c10b8a0: *stop=0x8c10b8a0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10b8a2; /* lds.l @r15+,PR */
L8c10b8a2: *stop=0x8c10b8a2u;if(*pr!=entry_pr)return -3;*stop=0x8c10b8a4u;return 0; /* rts */
}
