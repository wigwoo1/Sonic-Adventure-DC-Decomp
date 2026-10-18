/* Sequential integer-angle interpolation; explicit GBR/queue state.
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
SA_STAGE_EXPORT int sa_angle_blend_8c60a250(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60a250;
L8c60a250: *stop=0x8c60a250u;if(!valid_access(r[4]+12u,4u))return -3;r[3]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c60a252; /* mov.l @(0xc,r4),r3 */
L8c60a252: *stop=0x8c60a252u;fcopy(f,5u,4u);goto L8c60a254; /* fmov fr4,fr5 */
L8c60a254: *stop=0x8c60a254u;*fpul=r[3];goto L8c60a256; /* lds r3,FPUL */
L8c60a256: *stop=0x8c60a256u;fcopy(f,3u,4u);goto L8c60a258; /* fmov fr4,fr3 */
L8c60a258: *stop=0x8c60a258u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c60a25a; /* fldi1 fr4 */
L8c60a25a: *stop=0x8c60a25au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c60a25c; /* fsub fr3,fr4 */
L8c60a25c: *stop=0x8c60a25cu;if(!valid_access(r[4],4u))return -3;r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c60a25e; /* mov.l @r4,r2 */
L8c60a25e: *stop=0x8c60a25eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60a260; /* float FPUL,fr3 */
L8c60a260: *stop=0x8c60a260u;fcopy(f,0u,5u);goto L8c60a262; /* fmov fr5,fr0 */
L8c60a262: *stop=0x8c60a262u;*fpul=r[2];goto L8c60a264; /* lds r2,FPUL */
L8c60a264: *stop=0x8c60a264u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c60a266; /* float FPUL,fr2 */
L8c60a266: *stop=0x8c60a266u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60a268; /* fmul fr4,fr2 */
L8c60a268: *stop=0x8c60a268u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60a26a; /* fmac fr0,fr3,fr2 */
L8c60a26a: *stop=0x8c60a26au;fcopy(f,0u,4u);goto L8c60a26c; /* fmov fr4,fr0 */
L8c60a26c: *stop=0x8c60a26cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c60a26e; /* ftrc fr2,FPUL */
L8c60a26e: *stop=0x8c60a26eu;r[3]=*fpul;goto L8c60a270; /* sts FPUL,r3 */
L8c60a270: *stop=0x8c60a270u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c60a272; /* mov.l r3,@r5 */
L8c60a272: *stop=0x8c60a272u;if(!valid_access(r[4]+4u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c60a274; /* mov.l @(0x4,r4),r2 */
L8c60a274: *stop=0x8c60a274u;if(!valid_access(r[4]+16u,4u))return -3;r[3]=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;goto L8c60a276; /* mov.l @(0x10,r4),r3 */
L8c60a276: *stop=0x8c60a276u;*fpul=r[2];goto L8c60a278; /* lds r2,FPUL */
L8c60a278: *stop=0x8c60a278u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60a27a; /* float FPUL,fr3 */
L8c60a27a: *stop=0x8c60a27au;*fpul=r[3];goto L8c60a27c; /* lds r3,FPUL */
L8c60a27c: *stop=0x8c60a27cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c60a27e; /* float FPUL,fr2 */
L8c60a27e: *stop=0x8c60a27eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60a280; /* fmul fr5,fr2 */
L8c60a280: *stop=0x8c60a280u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60a282; /* fmac fr0,fr3,fr2 */
L8c60a282: *stop=0x8c60a282u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c60a284; /* ftrc fr2,FPUL */
L8c60a284: *stop=0x8c60a284u;r[3]=*fpul;goto L8c60a286; /* sts FPUL,r3 */
L8c60a286: *stop=0x8c60a286u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[3],4);if(io_status)return io_status;goto L8c60a288; /* mov.l r3,@(0x4,r5) */
L8c60a288: *stop=0x8c60a288u;if(!valid_access(r[4]+8u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c60a28a; /* mov.l @(0x8,r4),r2 */
L8c60a28a: *stop=0x8c60a28au;if(!valid_access(r[4]+20u,4u))return -3;r[3]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c60a28c; /* mov.l @(0x14,r4),r3 */
L8c60a28c: *stop=0x8c60a28cu;*fpul=r[2];goto L8c60a28e; /* lds r2,FPUL */
L8c60a28e: *stop=0x8c60a28eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60a290; /* float FPUL,fr3 */
L8c60a290: *stop=0x8c60a290u;*fpul=r[3];goto L8c60a292; /* lds r3,FPUL */
L8c60a292: *stop=0x8c60a292u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c60a294; /* float FPUL,fr2 */
L8c60a294: *stop=0x8c60a294u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60a296; /* fmul fr5,fr2 */
L8c60a296: *stop=0x8c60a296u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60a298; /* fmac fr0,fr3,fr2 */
L8c60a298: *stop=0x8c60a298u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c60a29a; /* ftrc fr2,FPUL */
L8c60a29a: *stop=0x8c60a29au;r[3]=*fpul;goto L8c60a29c; /* sts FPUL,r3 */
L8c60a29c: *stop=0x8c60a29cu;if(*pr!=entry_pr)return -3;*stop=0x8c60a29eu;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[3],4);if(io_status)return io_status;return 0; /* rts */
}
