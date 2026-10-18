#include "sa_draw_motion_stop.h"
/* Command-header acquisition variants with portable queues; names inferred.
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
SA_STAGE_EXPORT int sa_material_acquire_secondary_8c605f3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sr;(void)sx;(void)fload;(void)fstore;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c605f3c;
L8c605f3c: sa_motion_publish_stop(ctx,0x8c605f3cu); r[6]=rd(m,bus,&io_status,0x8c606170u,4);if(io_status)return io_status;goto L8c605f3e; /* mov.l 0x8c606170,r6 */
L8c605f3e: sa_motion_publish_stop(ctx,0x8c605f3eu); r[5]=28u;goto L8c605f40; /* mov #0x1c,r5 */
L8c605f40: sa_motion_publish_stop(ctx,0x8c605f40u); r[3]=rd(m,bus,&io_status,0x8c606174u,4);if(io_status)return io_status;goto L8c605f42; /* mov.l 0x8c606174,r3 */
L8c605f42: sa_motion_publish_stop(ctx,0x8c605f42u); r[4]=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;goto L8c605f44; /* mov.l @(0x14,r6),r4 */
L8c605f44: sa_motion_publish_stop(ctx,0x8c605f44u); r[2]=rd(m,bus,&io_status,0x8c606178u,4);if(io_status)return io_status;goto L8c605f46; /* mov.l 0x8c606178,r2 */
L8c605f46: sa_motion_publish_stop(ctx,0x8c605f46u); r[0]=r[4];goto L8c605f48; /* mov r4,r0 */
L8c605f48: sa_motion_publish_stop(ctx,0x8c605f48u); r[3]&=r[4];goto L8c605f4a; /* and r4,r3 */
L8c605f4a: sa_motion_publish_stop(ctx,0x8c605f4au); r[0]>>=16;goto L8c605f4c; /* shlr16 r0 */
L8c605f4c: sa_motion_publish_stop(ctx,0x8c605f4cu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c605f4e; /* mov.l r3,@r2 */
L8c605f4e: sa_motion_publish_stop(ctx,0x8c605f4eu); r[2]=rd(m,bus,&io_status,0x8c606184u,4);if(io_status)return io_status;goto L8c605f50; /* mov.l 0x8c606184,r2 */
L8c605f50: sa_motion_publish_stop(ctx,0x8c605f50u); r[0]>>=8;goto L8c605f52; /* shlr8 r0 */
L8c605f52: sa_motion_publish_stop(ctx,0x8c605f52u); r[3]=rd(m,bus,&io_status,0x8c606180u,4);if(io_status)return io_status;goto L8c605f54; /* mov.l 0x8c606180,r3 */
L8c605f54: sa_motion_publish_stop(ctx,0x8c605f54u); r[5]&=r[0];goto L8c605f56; /* and r0,r5 */
L8c605f56: sa_motion_publish_stop(ctx,0x8c605f56u); r[1]=rd(m,bus,&io_status,0x8c606188u,4);if(io_status)return io_status;goto L8c605f58; /* mov.l 0x8c606188,r1 */
L8c605f58: sa_motion_publish_stop(ctx,0x8c605f58u); r[4]&=r[2];goto L8c605f5a; /* and r2,r4 */
L8c605f5a: sa_motion_publish_stop(ctx,0x8c605f5au); r[0]=rd(m,bus,&io_status,0x8c60617cu,4);if(io_status)return io_status;goto L8c605f5c; /* mov.l 0x8c60617c,r0 */
L8c605f5c: sa_motion_publish_stop(ctx,0x8c605f5cu); r[4]|=r[1];goto L8c605f5e; /* or r1,r4 */
L8c605f5e: sa_motion_publish_stop(ctx,0x8c605f5eu); r[4]+=12u;goto L8c605f60; /* add #0xc,r4 */
L8c605f60: sa_motion_publish_stop(ctx,0x8c605f60u); wr(m,bus,&io_status,r[0],r[5],4);if(io_status)return io_status;goto L8c605f62; /* mov.l r5,@r0 */
L8c605f62: sa_motion_publish_stop(ctx,0x8c605f62u); wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c605f64; /* mov.l r5,@r3 */
L8c605f64: sa_motion_publish_stop(ctx,0x8c605f64u); r[3]=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;goto L8c605f66; /* mov.l @(0x8,r6),r3 */
L8c605f66: sa_motion_publish_stop(ctx,0x8c605f66u); r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c605f68; /* mov.l r3,@-r4 */
L8c605f68: sa_motion_publish_stop(ctx,0x8c605f68u); r[3]=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;goto L8c605f6a; /* mov.l @(0x4,r6),r3 */
L8c605f6a: sa_motion_publish_stop(ctx,0x8c605f6au); r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c605f6c; /* mov.l r3,@-r4 */
L8c605f6c: sa_motion_publish_stop(ctx,0x8c605f6cu); r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c605f6e; /* mov.l @r6,r3 */
L8c605f6e: sa_motion_publish_stop(ctx,0x8c605f6eu); r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c605f70; /* mov.l r3,@-r4 */
L8c605f70: sa_motion_publish_stop(ctx,0x8c605f70u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c605f72; /* pref @r4 */
L8c605f72: sa_motion_publish_stop(ctx,0x8c605f72u); r[0]=r[6];goto L8c605f74; /* mov r6,r0 */
L8c605f74: sa_motion_publish_stop(ctx,0x8c605f74u); r[3]=rd(m,bus,&io_status,0x8c606194u,4);if(io_status)return io_status;goto L8c605f76; /* mov.l 0x8c606194,r3 */
L8c605f76: sa_motion_publish_stop(ctx,0x8c605f76u); r[0]+=20u;goto L8c605f78; /* add #0x14,r0 */
L8c605f78: sa_motion_publish_stop(ctx,0x8c605f78u); wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c605f7a; /* mov.l r0,@r3 */
L8c605f7a: sa_motion_publish_stop(ctx,0x8c605f7au); r[4]+=32u;goto L8c605f7c; /* add #0x20,r4 */
L8c605f7c: sa_motion_publish_stop(ctx,0x8c605f7eu); r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_acquire_extended_8c6064b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sr;(void)sx;(void)fload;(void)fstore;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6064b0;
L8c6064b0: sa_motion_publish_stop(ctx,0x8c6064b0u); r[7]=rd(m,bus,&io_status,0x8c606724u,4);if(io_status)return io_status;goto L8c6064b2; /* mov.l 0x8c606724,r7 */
L8c6064b2: sa_motion_publish_stop(ctx,0x8c6064b2u); r[6]=28u;goto L8c6064b4; /* mov #0x1c,r6 */
L8c6064b4: sa_motion_publish_stop(ctx,0x8c6064b4u); r[3]=rd(m,bus,&io_status,0x8c606728u,4);if(io_status)return io_status;goto L8c6064b6; /* mov.l 0x8c606728,r3 */
L8c6064b6: sa_motion_publish_stop(ctx,0x8c6064b6u); r[4]+=12u;goto L8c6064b8; /* add #0xc,r4 */
L8c6064b8: sa_motion_publish_stop(ctx,0x8c6064b8u); r[5]=rd(m,bus,&io_status,r[7]+0x10u,4);if(io_status)return io_status;goto L8c6064ba; /* mov.l @(0x10,r7),r5 */
L8c6064ba: sa_motion_publish_stop(ctx,0x8c6064bau); r[2]=rd(m,bus,&io_status,0x8c60672cu,4);if(io_status)return io_status;goto L8c6064bc; /* mov.l 0x8c60672c,r2 */
L8c6064bc: sa_motion_publish_stop(ctx,0x8c6064bcu); r[0]=r[5];goto L8c6064be; /* mov r5,r0 */
L8c6064be: sa_motion_publish_stop(ctx,0x8c6064beu); r[3]&=r[5];goto L8c6064c0; /* and r5,r3 */
L8c6064c0: sa_motion_publish_stop(ctx,0x8c6064c0u); r[0]>>=16;goto L8c6064c2; /* shlr16 r0 */
L8c6064c2: sa_motion_publish_stop(ctx,0x8c6064c2u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6064c4; /* mov.l r3,@r2 */
L8c6064c4: sa_motion_publish_stop(ctx,0x8c6064c4u); r[3]=rd(m,bus,&io_status,0x8c606734u,4);if(io_status)return io_status;goto L8c6064c6; /* mov.l 0x8c606734,r3 */
L8c6064c6: sa_motion_publish_stop(ctx,0x8c6064c6u); r[0]>>=8;goto L8c6064c8; /* shlr8 r0 */
L8c6064c8: sa_motion_publish_stop(ctx,0x8c6064c8u); r[2]=rd(m,bus,&io_status,0x8c606738u,4);if(io_status)return io_status;goto L8c6064ca; /* mov.l 0x8c606738,r2 */
L8c6064ca: sa_motion_publish_stop(ctx,0x8c6064cau); r[6]&=r[0];goto L8c6064cc; /* and r0,r6 */
L8c6064cc: sa_motion_publish_stop(ctx,0x8c6064ccu); r[1]=rd(m,bus,&io_status,0x8c60673cu,4);if(io_status)return io_status;goto L8c6064ce; /* mov.l 0x8c60673c,r1 */
L8c6064ce: sa_motion_publish_stop(ctx,0x8c6064ceu); r[5]&=r[2];goto L8c6064d0; /* and r2,r5 */
L8c6064d0: sa_motion_publish_stop(ctx,0x8c6064d0u); r[0]=rd(m,bus,&io_status,0x8c606730u,4);if(io_status)return io_status;goto L8c6064d2; /* mov.l 0x8c606730,r0 */
L8c6064d2: sa_motion_publish_stop(ctx,0x8c6064d2u); r[5]|=r[1];goto L8c6064d4; /* or r1,r5 */
L8c6064d4: sa_motion_publish_stop(ctx,0x8c6064d4u); r[5]+=32u;goto L8c6064d6; /* add #0x20,r5 */
L8c6064d6: sa_motion_publish_stop(ctx,0x8c6064d6u); wr(m,bus,&io_status,r[0],r[6],4);if(io_status)return io_status;goto L8c6064d8; /* mov.l r6,@r0 */
L8c6064d8: sa_motion_publish_stop(ctx,0x8c6064d8u); wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c6064da; /* mov.l r6,@r3 */
L8c6064da: sa_motion_publish_stop(ctx,0x8c6064dau); r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6064dc; /* mov.l @r4,r3 */
L8c6064dc: sa_motion_publish_stop(ctx,0x8c6064dcu); r[4]+=4294967292u;goto L8c6064de; /* add #-0x4,r4 */
L8c6064de: sa_motion_publish_stop(ctx,0x8c6064deu); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064e0; /* mov.l r3,@-r5 */
L8c6064e0: sa_motion_publish_stop(ctx,0x8c6064e0u); r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6064e2; /* mov.l @r4,r3 */
L8c6064e2: sa_motion_publish_stop(ctx,0x8c6064e2u); r[4]+=4294967292u;goto L8c6064e4; /* add #-0x4,r4 */
L8c6064e4: sa_motion_publish_stop(ctx,0x8c6064e4u); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064e6; /* mov.l r3,@-r5 */
L8c6064e6: sa_motion_publish_stop(ctx,0x8c6064e6u); r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6064e8; /* mov.l @r4,r3 */
L8c6064e8: sa_motion_publish_stop(ctx,0x8c6064e8u); r[4]+=4294967292u;goto L8c6064ea; /* add #-0x4,r4 */
L8c6064ea: sa_motion_publish_stop(ctx,0x8c6064eau); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064ec; /* mov.l r3,@-r5 */
L8c6064ec: sa_motion_publish_stop(ctx,0x8c6064ecu); r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6064ee; /* mov.l @r4,r3 */
L8c6064ee: sa_motion_publish_stop(ctx,0x8c6064eeu); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064f0; /* mov.l r3,@-r5 */
L8c6064f0: sa_motion_publish_stop(ctx,0x8c6064f0u); r[3]=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;goto L8c6064f2; /* mov.l @(0xc,r7),r3 */
L8c6064f2: sa_motion_publish_stop(ctx,0x8c6064f2u); r[2]=rd(m,bus,&io_status,0x8c606744u,4);if(io_status)return io_status;goto L8c6064f4; /* mov.l 0x8c606744,r2 */
L8c6064f4: sa_motion_publish_stop(ctx,0x8c6064f4u); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064f6; /* mov.l r3,@-r5 */
L8c6064f6: sa_motion_publish_stop(ctx,0x8c6064f6u); r[0]=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;goto L8c6064f8; /* mov.l @(0x8,r7),r0 */
L8c6064f8: sa_motion_publish_stop(ctx,0x8c6064f8u); r[3]=rd(m,bus,&io_status,0x8c606740u,4);if(io_status)return io_status;goto L8c6064fa; /* mov.l 0x8c606740,r3 */
L8c6064fa: sa_motion_publish_stop(ctx,0x8c6064fau); r[0]&=r[3];goto L8c6064fc; /* and r3,r0 */
L8c6064fc: sa_motion_publish_stop(ctx,0x8c6064fcu); r[0]|=r[2];goto L8c6064fe; /* or r2,r0 */
L8c6064fe: sa_motion_publish_stop(ctx,0x8c6064feu); r[5]-=4;wr(m,bus,&io_status,r[5],r[0],4);if(io_status)return io_status;goto L8c606500; /* mov.l r0,@-r5 */
L8c606500: sa_motion_publish_stop(ctx,0x8c606500u); r[1]=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;goto L8c606502; /* mov.l @(0x4,r7),r1 */
L8c606502: sa_motion_publish_stop(ctx,0x8c606502u); r[5]-=4;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c606504; /* mov.l r1,@-r5 */
L8c606504: sa_motion_publish_stop(ctx,0x8c606504u); r[1]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c606506; /* mov.l @r7,r1 */
L8c606506: sa_motion_publish_stop(ctx,0x8c606506u); r[5]-=4;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c606508; /* mov.l r1,@-r5 */
L8c606508: sa_motion_publish_stop(ctx,0x8c606508u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[5],1u);if(status)return status;goto L8c60650a; /* pref @r5 */
L8c60650a: sa_motion_publish_stop(ctx,0x8c60650au); r[0]=r[7];goto L8c60650c; /* mov r7,r0 */
L8c60650c: sa_motion_publish_stop(ctx,0x8c60650cu); r[1]=rd(m,bus,&io_status,0x8c606748u,4);if(io_status)return io_status;goto L8c60650e; /* mov.l 0x8c606748,r1 */
L8c60650e: sa_motion_publish_stop(ctx,0x8c60650eu); r[0]+=16u;goto L8c606510; /* add #0x10,r0 */
L8c606510: sa_motion_publish_stop(ctx,0x8c606510u); wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c606512; /* mov.l r0,@r1 */
L8c606512: sa_motion_publish_stop(ctx,0x8c606512u); r[5]+=32u;goto L8c606514; /* add #0x20,r5 */
L8c606514: sa_motion_publish_stop(ctx,0x8c606516u); r[0]=r[5];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_acquire_extended_secondary_8c6065b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sr;(void)sx;(void)fload;(void)fstore;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6065b0;
L8c6065b0: sa_motion_publish_stop(ctx,0x8c6065b0u); r[7]=rd(m,bus,&io_status,0x8c606724u,4);if(io_status)return io_status;goto L8c6065b2; /* mov.l 0x8c606724,r7 */
L8c6065b2: sa_motion_publish_stop(ctx,0x8c6065b2u); r[6]=28u;goto L8c6065b4; /* mov #0x1c,r6 */
L8c6065b4: sa_motion_publish_stop(ctx,0x8c6065b4u); r[3]=rd(m,bus,&io_status,0x8c606728u,4);if(io_status)return io_status;goto L8c6065b6; /* mov.l 0x8c606728,r3 */
L8c6065b6: sa_motion_publish_stop(ctx,0x8c6065b6u); r[4]+=12u;goto L8c6065b8; /* add #0xc,r4 */
L8c6065b8: sa_motion_publish_stop(ctx,0x8c6065b8u); r[5]=rd(m,bus,&io_status,r[7]+0x14u,4);if(io_status)return io_status;goto L8c6065ba; /* mov.l @(0x14,r7),r5 */
L8c6065ba: sa_motion_publish_stop(ctx,0x8c6065bau); r[2]=rd(m,bus,&io_status,0x8c60672cu,4);if(io_status)return io_status;goto L8c6065bc; /* mov.l 0x8c60672c,r2 */
L8c6065bc: sa_motion_publish_stop(ctx,0x8c6065bcu); r[0]=r[5];goto L8c6065be; /* mov r5,r0 */
L8c6065be: sa_motion_publish_stop(ctx,0x8c6065beu); r[3]&=r[5];goto L8c6065c0; /* and r5,r3 */
L8c6065c0: sa_motion_publish_stop(ctx,0x8c6065c0u); r[0]>>=16;goto L8c6065c2; /* shlr16 r0 */
L8c6065c2: sa_motion_publish_stop(ctx,0x8c6065c2u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6065c4; /* mov.l r3,@r2 */
L8c6065c4: sa_motion_publish_stop(ctx,0x8c6065c4u); r[3]=rd(m,bus,&io_status,0x8c606734u,4);if(io_status)return io_status;goto L8c6065c6; /* mov.l 0x8c606734,r3 */
L8c6065c6: sa_motion_publish_stop(ctx,0x8c6065c6u); r[0]>>=8;goto L8c6065c8; /* shlr8 r0 */
L8c6065c8: sa_motion_publish_stop(ctx,0x8c6065c8u); r[2]=rd(m,bus,&io_status,0x8c606738u,4);if(io_status)return io_status;goto L8c6065ca; /* mov.l 0x8c606738,r2 */
L8c6065ca: sa_motion_publish_stop(ctx,0x8c6065cau); r[6]&=r[0];goto L8c6065cc; /* and r0,r6 */
L8c6065cc: sa_motion_publish_stop(ctx,0x8c6065ccu); r[1]=rd(m,bus,&io_status,0x8c60673cu,4);if(io_status)return io_status;goto L8c6065ce; /* mov.l 0x8c60673c,r1 */
L8c6065ce: sa_motion_publish_stop(ctx,0x8c6065ceu); r[5]&=r[2];goto L8c6065d0; /* and r2,r5 */
L8c6065d0: sa_motion_publish_stop(ctx,0x8c6065d0u); r[0]=rd(m,bus,&io_status,0x8c606730u,4);if(io_status)return io_status;goto L8c6065d2; /* mov.l 0x8c606730,r0 */
L8c6065d2: sa_motion_publish_stop(ctx,0x8c6065d2u); r[5]|=r[1];goto L8c6065d4; /* or r1,r5 */
L8c6065d4: sa_motion_publish_stop(ctx,0x8c6065d4u); r[5]+=32u;goto L8c6065d6; /* add #0x20,r5 */
L8c6065d6: sa_motion_publish_stop(ctx,0x8c6065d6u); wr(m,bus,&io_status,r[0],r[6],4);if(io_status)return io_status;goto L8c6065d8; /* mov.l r6,@r0 */
L8c6065d8: sa_motion_publish_stop(ctx,0x8c6065d8u); wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c6065da; /* mov.l r6,@r3 */
L8c6065da: sa_motion_publish_stop(ctx,0x8c6065dau); r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6065dc; /* mov.l @r4,r3 */
L8c6065dc: sa_motion_publish_stop(ctx,0x8c6065dcu); r[4]+=4294967292u;goto L8c6065de; /* add #-0x4,r4 */
L8c6065de: sa_motion_publish_stop(ctx,0x8c6065deu); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065e0; /* mov.l r3,@-r5 */
L8c6065e0: sa_motion_publish_stop(ctx,0x8c6065e0u); r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6065e2; /* mov.l @r4,r3 */
L8c6065e2: sa_motion_publish_stop(ctx,0x8c6065e2u); r[4]+=4294967292u;goto L8c6065e4; /* add #-0x4,r4 */
L8c6065e4: sa_motion_publish_stop(ctx,0x8c6065e4u); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065e6; /* mov.l r3,@-r5 */
L8c6065e6: sa_motion_publish_stop(ctx,0x8c6065e6u); r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6065e8; /* mov.l @r4,r3 */
L8c6065e8: sa_motion_publish_stop(ctx,0x8c6065e8u); r[4]+=4294967292u;goto L8c6065ea; /* add #-0x4,r4 */
L8c6065ea: sa_motion_publish_stop(ctx,0x8c6065eau); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065ec; /* mov.l r3,@-r5 */
L8c6065ec: sa_motion_publish_stop(ctx,0x8c6065ecu); r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6065ee; /* mov.l @r4,r3 */
L8c6065ee: sa_motion_publish_stop(ctx,0x8c6065eeu); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065f0; /* mov.l r3,@-r5 */
L8c6065f0: sa_motion_publish_stop(ctx,0x8c6065f0u); r[3]=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;goto L8c6065f2; /* mov.l @(0xc,r7),r3 */
L8c6065f2: sa_motion_publish_stop(ctx,0x8c6065f2u); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065f4; /* mov.l r3,@-r5 */
L8c6065f4: sa_motion_publish_stop(ctx,0x8c6065f4u); r[3]=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;goto L8c6065f6; /* mov.l @(0x8,r7),r3 */
L8c6065f6: sa_motion_publish_stop(ctx,0x8c6065f6u); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065f8; /* mov.l r3,@-r5 */
L8c6065f8: sa_motion_publish_stop(ctx,0x8c6065f8u); r[3]=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;goto L8c6065fa; /* mov.l @(0x4,r7),r3 */
L8c6065fa: sa_motion_publish_stop(ctx,0x8c6065fau); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065fc; /* mov.l r3,@-r5 */
L8c6065fc: sa_motion_publish_stop(ctx,0x8c6065fcu); r[3]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6065fe; /* mov.l @r7,r3 */
L8c6065fe: sa_motion_publish_stop(ctx,0x8c6065feu); r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c606600; /* mov.l r3,@-r5 */
L8c606600: sa_motion_publish_stop(ctx,0x8c606600u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[5],1u);if(status)return status;goto L8c606602; /* pref @r5 */
L8c606602: sa_motion_publish_stop(ctx,0x8c606602u); r[0]=r[7];goto L8c606604; /* mov r7,r0 */
L8c606604: sa_motion_publish_stop(ctx,0x8c606604u); r[3]=rd(m,bus,&io_status,0x8c606748u,4);if(io_status)return io_status;goto L8c606606; /* mov.l 0x8c606748,r3 */
L8c606606: sa_motion_publish_stop(ctx,0x8c606606u); r[0]+=20u;goto L8c606608; /* add #0x14,r0 */
L8c606608: sa_motion_publish_stop(ctx,0x8c606608u); wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c60660a; /* mov.l r0,@r3 */
L8c60660a: sa_motion_publish_stop(ctx,0x8c60660au); r[5]+=32u;goto L8c60660c; /* add #0x20,r5 */
L8c60660c: sa_motion_publish_stop(ctx,0x8c60660eu); r[0]=r[5];return 0; /* rts */
}
