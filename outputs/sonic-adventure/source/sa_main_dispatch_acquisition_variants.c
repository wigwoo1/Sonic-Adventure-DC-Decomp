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
L8c605f3c: r[6]=rd(m,bus,&io_status,0x8c606170u,4);if(io_status)return io_status;goto L8c605f3e; /* mov.l 0x8c606170,r6 */
L8c605f3e: r[5]=28u;goto L8c605f40; /* mov #0x1c,r5 */
L8c605f40: r[3]=rd(m,bus,&io_status,0x8c606174u,4);if(io_status)return io_status;goto L8c605f42; /* mov.l 0x8c606174,r3 */
L8c605f42: r[4]=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;goto L8c605f44; /* mov.l @(0x14,r6),r4 */
L8c605f44: r[2]=rd(m,bus,&io_status,0x8c606178u,4);if(io_status)return io_status;goto L8c605f46; /* mov.l 0x8c606178,r2 */
L8c605f46: r[0]=r[4];goto L8c605f48; /* mov r4,r0 */
L8c605f48: r[3]&=r[4];goto L8c605f4a; /* and r4,r3 */
L8c605f4a: r[0]>>=16;goto L8c605f4c; /* shlr16 r0 */
L8c605f4c: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c605f4e; /* mov.l r3,@r2 */
L8c605f4e: r[2]=rd(m,bus,&io_status,0x8c606184u,4);if(io_status)return io_status;goto L8c605f50; /* mov.l 0x8c606184,r2 */
L8c605f50: r[0]>>=8;goto L8c605f52; /* shlr8 r0 */
L8c605f52: r[3]=rd(m,bus,&io_status,0x8c606180u,4);if(io_status)return io_status;goto L8c605f54; /* mov.l 0x8c606180,r3 */
L8c605f54: r[5]&=r[0];goto L8c605f56; /* and r0,r5 */
L8c605f56: r[1]=rd(m,bus,&io_status,0x8c606188u,4);if(io_status)return io_status;goto L8c605f58; /* mov.l 0x8c606188,r1 */
L8c605f58: r[4]&=r[2];goto L8c605f5a; /* and r2,r4 */
L8c605f5a: r[0]=rd(m,bus,&io_status,0x8c60617cu,4);if(io_status)return io_status;goto L8c605f5c; /* mov.l 0x8c60617c,r0 */
L8c605f5c: r[4]|=r[1];goto L8c605f5e; /* or r1,r4 */
L8c605f5e: r[4]+=12u;goto L8c605f60; /* add #0xc,r4 */
L8c605f60: wr(m,bus,&io_status,r[0],r[5],4);if(io_status)return io_status;goto L8c605f62; /* mov.l r5,@r0 */
L8c605f62: wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c605f64; /* mov.l r5,@r3 */
L8c605f64: r[3]=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;goto L8c605f66; /* mov.l @(0x8,r6),r3 */
L8c605f66: r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c605f68; /* mov.l r3,@-r4 */
L8c605f68: r[3]=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;goto L8c605f6a; /* mov.l @(0x4,r6),r3 */
L8c605f6a: r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c605f6c; /* mov.l r3,@-r4 */
L8c605f6c: r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c605f6e; /* mov.l @r6,r3 */
L8c605f6e: r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c605f70; /* mov.l r3,@-r4 */
L8c605f70: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c605f72; /* pref @r4 */
L8c605f72: r[0]=r[6];goto L8c605f74; /* mov r6,r0 */
L8c605f74: r[3]=rd(m,bus,&io_status,0x8c606194u,4);if(io_status)return io_status;goto L8c605f76; /* mov.l 0x8c606194,r3 */
L8c605f76: r[0]+=20u;goto L8c605f78; /* add #0x14,r0 */
L8c605f78: wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c605f7a; /* mov.l r0,@r3 */
L8c605f7a: r[4]+=32u;goto L8c605f7c; /* add #0x20,r4 */
L8c605f7c: r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_acquire_extended_8c6064b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sr;(void)sx;(void)fload;(void)fstore;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6064b0;
L8c6064b0: r[7]=rd(m,bus,&io_status,0x8c606724u,4);if(io_status)return io_status;goto L8c6064b2; /* mov.l 0x8c606724,r7 */
L8c6064b2: r[6]=28u;goto L8c6064b4; /* mov #0x1c,r6 */
L8c6064b4: r[3]=rd(m,bus,&io_status,0x8c606728u,4);if(io_status)return io_status;goto L8c6064b6; /* mov.l 0x8c606728,r3 */
L8c6064b6: r[4]+=12u;goto L8c6064b8; /* add #0xc,r4 */
L8c6064b8: r[5]=rd(m,bus,&io_status,r[7]+0x10u,4);if(io_status)return io_status;goto L8c6064ba; /* mov.l @(0x10,r7),r5 */
L8c6064ba: r[2]=rd(m,bus,&io_status,0x8c60672cu,4);if(io_status)return io_status;goto L8c6064bc; /* mov.l 0x8c60672c,r2 */
L8c6064bc: r[0]=r[5];goto L8c6064be; /* mov r5,r0 */
L8c6064be: r[3]&=r[5];goto L8c6064c0; /* and r5,r3 */
L8c6064c0: r[0]>>=16;goto L8c6064c2; /* shlr16 r0 */
L8c6064c2: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6064c4; /* mov.l r3,@r2 */
L8c6064c4: r[3]=rd(m,bus,&io_status,0x8c606734u,4);if(io_status)return io_status;goto L8c6064c6; /* mov.l 0x8c606734,r3 */
L8c6064c6: r[0]>>=8;goto L8c6064c8; /* shlr8 r0 */
L8c6064c8: r[2]=rd(m,bus,&io_status,0x8c606738u,4);if(io_status)return io_status;goto L8c6064ca; /* mov.l 0x8c606738,r2 */
L8c6064ca: r[6]&=r[0];goto L8c6064cc; /* and r0,r6 */
L8c6064cc: r[1]=rd(m,bus,&io_status,0x8c60673cu,4);if(io_status)return io_status;goto L8c6064ce; /* mov.l 0x8c60673c,r1 */
L8c6064ce: r[5]&=r[2];goto L8c6064d0; /* and r2,r5 */
L8c6064d0: r[0]=rd(m,bus,&io_status,0x8c606730u,4);if(io_status)return io_status;goto L8c6064d2; /* mov.l 0x8c606730,r0 */
L8c6064d2: r[5]|=r[1];goto L8c6064d4; /* or r1,r5 */
L8c6064d4: r[5]+=32u;goto L8c6064d6; /* add #0x20,r5 */
L8c6064d6: wr(m,bus,&io_status,r[0],r[6],4);if(io_status)return io_status;goto L8c6064d8; /* mov.l r6,@r0 */
L8c6064d8: wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c6064da; /* mov.l r6,@r3 */
L8c6064da: r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6064dc; /* mov.l @r4,r3 */
L8c6064dc: r[4]+=4294967292u;goto L8c6064de; /* add #-0x4,r4 */
L8c6064de: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064e0; /* mov.l r3,@-r5 */
L8c6064e0: r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6064e2; /* mov.l @r4,r3 */
L8c6064e2: r[4]+=4294967292u;goto L8c6064e4; /* add #-0x4,r4 */
L8c6064e4: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064e6; /* mov.l r3,@-r5 */
L8c6064e6: r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6064e8; /* mov.l @r4,r3 */
L8c6064e8: r[4]+=4294967292u;goto L8c6064ea; /* add #-0x4,r4 */
L8c6064ea: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064ec; /* mov.l r3,@-r5 */
L8c6064ec: r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6064ee; /* mov.l @r4,r3 */
L8c6064ee: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064f0; /* mov.l r3,@-r5 */
L8c6064f0: r[3]=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;goto L8c6064f2; /* mov.l @(0xc,r7),r3 */
L8c6064f2: r[2]=rd(m,bus,&io_status,0x8c606744u,4);if(io_status)return io_status;goto L8c6064f4; /* mov.l 0x8c606744,r2 */
L8c6064f4: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064f6; /* mov.l r3,@-r5 */
L8c6064f6: r[0]=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;goto L8c6064f8; /* mov.l @(0x8,r7),r0 */
L8c6064f8: r[3]=rd(m,bus,&io_status,0x8c606740u,4);if(io_status)return io_status;goto L8c6064fa; /* mov.l 0x8c606740,r3 */
L8c6064fa: r[0]&=r[3];goto L8c6064fc; /* and r3,r0 */
L8c6064fc: r[0]|=r[2];goto L8c6064fe; /* or r2,r0 */
L8c6064fe: r[5]-=4;wr(m,bus,&io_status,r[5],r[0],4);if(io_status)return io_status;goto L8c606500; /* mov.l r0,@-r5 */
L8c606500: r[1]=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;goto L8c606502; /* mov.l @(0x4,r7),r1 */
L8c606502: r[5]-=4;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c606504; /* mov.l r1,@-r5 */
L8c606504: r[1]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c606506; /* mov.l @r7,r1 */
L8c606506: r[5]-=4;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c606508; /* mov.l r1,@-r5 */
L8c606508: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[5],1u);if(status)return status;goto L8c60650a; /* pref @r5 */
L8c60650a: r[0]=r[7];goto L8c60650c; /* mov r7,r0 */
L8c60650c: r[1]=rd(m,bus,&io_status,0x8c606748u,4);if(io_status)return io_status;goto L8c60650e; /* mov.l 0x8c606748,r1 */
L8c60650e: r[0]+=16u;goto L8c606510; /* add #0x10,r0 */
L8c606510: wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c606512; /* mov.l r0,@r1 */
L8c606512: r[5]+=32u;goto L8c606514; /* add #0x20,r5 */
L8c606514: r[0]=r[5];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_acquire_extended_secondary_8c6065b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sr;(void)sx;(void)fload;(void)fstore;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6065b0;
L8c6065b0: r[7]=rd(m,bus,&io_status,0x8c606724u,4);if(io_status)return io_status;goto L8c6065b2; /* mov.l 0x8c606724,r7 */
L8c6065b2: r[6]=28u;goto L8c6065b4; /* mov #0x1c,r6 */
L8c6065b4: r[3]=rd(m,bus,&io_status,0x8c606728u,4);if(io_status)return io_status;goto L8c6065b6; /* mov.l 0x8c606728,r3 */
L8c6065b6: r[4]+=12u;goto L8c6065b8; /* add #0xc,r4 */
L8c6065b8: r[5]=rd(m,bus,&io_status,r[7]+0x14u,4);if(io_status)return io_status;goto L8c6065ba; /* mov.l @(0x14,r7),r5 */
L8c6065ba: r[2]=rd(m,bus,&io_status,0x8c60672cu,4);if(io_status)return io_status;goto L8c6065bc; /* mov.l 0x8c60672c,r2 */
L8c6065bc: r[0]=r[5];goto L8c6065be; /* mov r5,r0 */
L8c6065be: r[3]&=r[5];goto L8c6065c0; /* and r5,r3 */
L8c6065c0: r[0]>>=16;goto L8c6065c2; /* shlr16 r0 */
L8c6065c2: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6065c4; /* mov.l r3,@r2 */
L8c6065c4: r[3]=rd(m,bus,&io_status,0x8c606734u,4);if(io_status)return io_status;goto L8c6065c6; /* mov.l 0x8c606734,r3 */
L8c6065c6: r[0]>>=8;goto L8c6065c8; /* shlr8 r0 */
L8c6065c8: r[2]=rd(m,bus,&io_status,0x8c606738u,4);if(io_status)return io_status;goto L8c6065ca; /* mov.l 0x8c606738,r2 */
L8c6065ca: r[6]&=r[0];goto L8c6065cc; /* and r0,r6 */
L8c6065cc: r[1]=rd(m,bus,&io_status,0x8c60673cu,4);if(io_status)return io_status;goto L8c6065ce; /* mov.l 0x8c60673c,r1 */
L8c6065ce: r[5]&=r[2];goto L8c6065d0; /* and r2,r5 */
L8c6065d0: r[0]=rd(m,bus,&io_status,0x8c606730u,4);if(io_status)return io_status;goto L8c6065d2; /* mov.l 0x8c606730,r0 */
L8c6065d2: r[5]|=r[1];goto L8c6065d4; /* or r1,r5 */
L8c6065d4: r[5]+=32u;goto L8c6065d6; /* add #0x20,r5 */
L8c6065d6: wr(m,bus,&io_status,r[0],r[6],4);if(io_status)return io_status;goto L8c6065d8; /* mov.l r6,@r0 */
L8c6065d8: wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c6065da; /* mov.l r6,@r3 */
L8c6065da: r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6065dc; /* mov.l @r4,r3 */
L8c6065dc: r[4]+=4294967292u;goto L8c6065de; /* add #-0x4,r4 */
L8c6065de: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065e0; /* mov.l r3,@-r5 */
L8c6065e0: r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6065e2; /* mov.l @r4,r3 */
L8c6065e2: r[4]+=4294967292u;goto L8c6065e4; /* add #-0x4,r4 */
L8c6065e4: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065e6; /* mov.l r3,@-r5 */
L8c6065e6: r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6065e8; /* mov.l @r4,r3 */
L8c6065e8: r[4]+=4294967292u;goto L8c6065ea; /* add #-0x4,r4 */
L8c6065ea: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065ec; /* mov.l r3,@-r5 */
L8c6065ec: r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6065ee; /* mov.l @r4,r3 */
L8c6065ee: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065f0; /* mov.l r3,@-r5 */
L8c6065f0: r[3]=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;goto L8c6065f2; /* mov.l @(0xc,r7),r3 */
L8c6065f2: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065f4; /* mov.l r3,@-r5 */
L8c6065f4: r[3]=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;goto L8c6065f6; /* mov.l @(0x8,r7),r3 */
L8c6065f6: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065f8; /* mov.l r3,@-r5 */
L8c6065f8: r[3]=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;goto L8c6065fa; /* mov.l @(0x4,r7),r3 */
L8c6065fa: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065fc; /* mov.l r3,@-r5 */
L8c6065fc: r[3]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6065fe; /* mov.l @r7,r3 */
L8c6065fe: r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c606600; /* mov.l r3,@-r5 */
L8c606600: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[5],1u);if(status)return status;goto L8c606602; /* pref @r5 */
L8c606602: r[0]=r[7];goto L8c606604; /* mov r7,r0 */
L8c606604: r[3]=rd(m,bus,&io_status,0x8c606748u,4);if(io_status)return io_status;goto L8c606606; /* mov.l 0x8c606748,r3 */
L8c606606: r[0]+=20u;goto L8c606608; /* add #0x14,r0 */
L8c606608: wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c60660a; /* mov.l r0,@r3 */
L8c60660a: r[5]+=32u;goto L8c60660c; /* add #0x20,r5 */
L8c60660c: r[0]=r[5];return 0; /* rts */
}
