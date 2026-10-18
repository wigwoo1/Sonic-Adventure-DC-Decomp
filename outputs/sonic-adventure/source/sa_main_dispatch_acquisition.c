/* Material-header acquisition with portable queue interface; name inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_vector_numeric.h"
#include "sh4_fmac.h"
#include "sh4_divide.h"
#include "sh4_add_sub.h"
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
SA_STAGE_EXPORT int sa_material_acquire_8c605e68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sr;(void)sx;(void)fload;(void)fstore;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c605e68;
L8c605e68: r[6]=rd(m,bus,&io_status,0x8c605ebcu,4);if(io_status)return io_status;goto L8c605e6a; /* mov.l 0x8c605ebc,r6 */
L8c605e6a: r[5]=28u;goto L8c605e6c; /* mov #0x1c,r5 */
L8c605e6c: r[3]=rd(m,bus,&io_status,0x8c605ec8u,4);if(io_status)return io_status;goto L8c605e6e; /* mov.l 0x8c605ec8,r3 */
L8c605e6e: r[4]=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;goto L8c605e70; /* mov.l @(0x10,r6),r4 */
L8c605e70: r[2]=rd(m,bus,&io_status,0x8c605eccu,4);if(io_status)return io_status;goto L8c605e72; /* mov.l 0x8c605ecc,r2 */
L8c605e72: r[0]=r[4];goto L8c605e74; /* mov r4,r0 */
L8c605e74: r[0]>>=16;goto L8c605e76; /* shlr16 r0 */
L8c605e76: r[0]>>=8;goto L8c605e78; /* shlr8 r0 */
L8c605e78: r[1]=rd(m,bus,&io_status,0x8c605edcu,4);if(io_status)return io_status;goto L8c605e7a; /* mov.l 0x8c605edc,r1 */
L8c605e7a: r[3]&=r[4];goto L8c605e7c; /* and r4,r3 */
L8c605e7c: r[5]&=r[0];goto L8c605e7e; /* and r0,r5 */
L8c605e7e: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c605e80; /* mov.l r3,@r2 */
L8c605e80: r[3]=rd(m,bus,&io_status,0x8c605ed4u,4);if(io_status)return io_status;goto L8c605e82; /* mov.l 0x8c605ed4,r3 */
L8c605e82: r[0]=rd(m,bus,&io_status,0x8c605ed0u,4);if(io_status)return io_status;goto L8c605e84; /* mov.l 0x8c605ed0,r0 */
L8c605e84: r[2]=rd(m,bus,&io_status,0x8c605ed8u,4);if(io_status)return io_status;goto L8c605e86; /* mov.l 0x8c605ed8,r2 */
L8c605e86: wr(m,bus,&io_status,r[0],r[5],4);if(io_status)return io_status;goto L8c605e88; /* mov.l r5,@r0 */
L8c605e88: wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c605e8a; /* mov.l r5,@r3 */
L8c605e8a: r[4]&=r[2];goto L8c605e8c; /* and r2,r4 */
L8c605e8c: r[0]=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;goto L8c605e8e; /* mov.l @(0x8,r6),r0 */
L8c605e8e: r[4]|=r[1];goto L8c605e90; /* or r1,r4 */
L8c605e90: r[3]=rd(m,bus,&io_status,0x8c605ee0u,4);if(io_status)return io_status;goto L8c605e92; /* mov.l 0x8c605ee0,r3 */
L8c605e92: r[4]+=12u;goto L8c605e94; /* add #0xc,r4 */
L8c605e94: r[2]=rd(m,bus,&io_status,0x8c605ee4u,4);if(io_status)return io_status;goto L8c605e96; /* mov.l 0x8c605ee4,r2 */
L8c605e96: r[0]&=r[3];goto L8c605e98; /* and r3,r0 */
L8c605e98: r[0]|=r[2];goto L8c605e9a; /* or r2,r0 */
L8c605e9a: r[4]-=4;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c605e9c; /* mov.l r0,@-r4 */
L8c605e9c: r[1]=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;goto L8c605e9e; /* mov.l @(0x4,r6),r1 */
L8c605e9e: r[4]-=4;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c605ea0; /* mov.l r1,@-r4 */
L8c605ea0: r[1]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c605ea2; /* mov.l @r6,r1 */
L8c605ea2: r[4]-=4;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c605ea4; /* mov.l r1,@-r4 */
L8c605ea4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c605ea6; /* pref @r4 */
L8c605ea6: r[0]=r[6];goto L8c605ea8; /* mov r6,r0 */
L8c605ea8: r[1]=rd(m,bus,&io_status,0x8c605ee8u,4);if(io_status)return io_status;goto L8c605eaa; /* mov.l 0x8c605ee8,r1 */
L8c605eaa: r[0]+=16u;goto L8c605eac; /* add #0x10,r0 */
L8c605eac: wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c605eae; /* mov.l r0,@r1 */
L8c605eae: r[4]+=32u;goto L8c605eb0; /* add #0x20,r4 */
L8c605eb0: r[0]=r[4];return 0; /* rts */
}
