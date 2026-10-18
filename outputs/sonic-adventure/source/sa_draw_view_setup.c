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
SA_STAGE_EXPORT int sa_view_setup_8c64f548(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64f548;
L8c64f548: *stop=0x8c64f548u;r[4]=rd(m,bus,&io_status,0x8c64f5acu,4);if(io_status)return io_status;goto L8c64f54a; /* mov.l 0x8c64f5ac,r4 */
L8c64f54a: *stop=0x8c64f54au;r[0]=16u;goto L8c64f54c; /* mov #0x10,r0 */
L8c64f54c: *stop=0x8c64f54cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c64f54e; /* fldi1 fr5 */
L8c64f54e: *stop=0x8c64f54eu;r[5]=0u;goto L8c64f550; /* mov #0x0,r5 */
L8c64f550: *stop=0x8c64f550u;r[3]=rd(m,bus,&io_status,0x8c64f5a8u,4);if(io_status)return io_status;goto L8c64f552; /* mov.l 0x8c64f5a8,r3 */
L8c64f552: *stop=0x8c64f552u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c64f554; /* mov.l r5,@r3 */
L8c64f554: *stop=0x8c64f554u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f556; /* fmov fr5,@(r0,r4) */
L8c64f556: *stop=0x8c64f556u;r[0]=0x8c64f5b0u;goto L8c64f558; /* mova 0x8c64f5b0,r0 */
L8c64f558: *stop=0x8c64f558u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c64f55a; /* fmov.s @r0,fr4 */
L8c64f55a: *stop=0x8c64f55au;r[0]=20u;goto L8c64f55c; /* mov #0x14,r0 */
L8c64f55c: *stop=0x8c64f55cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f55e; /* fmov fr4,@(r0,r4) */
L8c64f55e: *stop=0x8c64f55eu;r[0]=24u;goto L8c64f560; /* mov #0x18,r0 */
L8c64f560: *stop=0x8c64f560u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f562; /* fmov fr4,@(r0,r4) */
L8c64f562: *stop=0x8c64f562u;r[0]=28u;goto L8c64f564; /* mov #0x1c,r0 */
L8c64f564: *stop=0x8c64f564u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f566; /* fmov fr4,@(r0,r4) */
L8c64f566: *stop=0x8c64f566u;r[0]=32u;goto L8c64f568; /* mov #0x20,r0 */
L8c64f568: *stop=0x8c64f568u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f56a; /* fmov fr5,@(r0,r4) */
L8c64f56a: *stop=0x8c64f56au;r[0]=36u;goto L8c64f56c; /* mov #0x24,r0 */
L8c64f56c: *stop=0x8c64f56cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f56e; /* fmov fr4,@(r0,r4) */
L8c64f56e: *stop=0x8c64f56eu;r[0]=40u;goto L8c64f570; /* mov #0x28,r0 */
L8c64f570: *stop=0x8c64f570u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f572; /* fmov fr4,@(r0,r4) */
L8c64f572: *stop=0x8c64f572u;r[0]=44u;goto L8c64f574; /* mov #0x2c,r0 */
L8c64f574: *stop=0x8c64f574u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f576; /* fmov fr4,@(r0,r4) */
L8c64f576: *stop=0x8c64f576u;r[0]=4u;goto L8c64f578; /* mov #0x4,r0 */
L8c64f578: *stop=0x8c64f578u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c64f57a; /* fldi0 fr4 */
L8c64f57a: *stop=0x8c64f57au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c64f57c; /* fmov.s fr5,@r4 */
L8c64f57c: *stop=0x8c64f57cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f57e; /* fmov fr4,@(r0,r4) */
L8c64f57e: *stop=0x8c64f57eu;r[0]=8u;goto L8c64f580; /* mov #0x8,r0 */
L8c64f580: *stop=0x8c64f580u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f582; /* fmov fr4,@(r0,r4) */
L8c64f582: *stop=0x8c64f582u;r[0]=12u;goto L8c64f584; /* mov #0xc,r0 */
L8c64f584: *stop=0x8c64f584u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f586; /* fmov fr4,@(r0,r4) */
L8c64f586: *stop=0x8c64f586u;r[0]=0x8c64f5b4u;goto L8c64f588; /* mova 0x8c64f5b4,r0 */
L8c64f588: *stop=0x8c64f588u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c64f58a; /* fmov.s @r0,fr3 */
L8c64f58a: *stop=0x8c64f58au;r[0]=48u;goto L8c64f58c; /* mov #0x30,r0 */
L8c64f58c: *stop=0x8c64f58cu;r[6]=rd(m,bus,&io_status,0x8c64f5b8u,4);if(io_status)return io_status;goto L8c64f58e; /* mov.l 0x8c64f5b8,r6 */
L8c64f58e: *stop=0x8c64f58eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c64f590; /* fmov fr3,@(r0,r4) */
L8c64f590: *stop=0x8c64f590u;r[0]+=80u;goto L8c64f592; /* add #0x50,r0 */
L8c64f592: *stop=0x8c64f592u;r[4]=r[5];goto L8c64f594; /* mov r5,r4 */
L8c64f594: *stop=0x8c64f594u;r[7]=r[0];goto L8c64f596; /* mov r0,r7 */
L8c64f596: *stop=0x8c64f596u;if(!valid_access(r[0]+r[6],4u))return -3;wr(m,bus,&io_status,r[6]+r[0],r[5],4);if(io_status)return io_status;goto L8c64f598; /* mov.l r5,@(r0,r6) */
L8c64f598: *stop=0x8c64f598u;r[3]=r[6];goto L8c64f59a; /* mov r6,r3 */
L8c64f59a: *stop=0x8c64f59au;r[3]+=r[4];goto L8c64f59c; /* add r4,r3 */
L8c64f59c: *stop=0x8c64f59cu;r[4]+=4u;goto L8c64f59e; /* add #0x4,r4 */
L8c64f59e: *stop=0x8c64f59eu;*sr=(*sr&~1u)|(r[4]>=r[7]);goto L8c64f5a0; /* cmp/hs r7,r4 */
L8c64f5a0: take=!(*sr&1u);*stop=0x8c64f5a2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;if(take)goto L8c64f598;goto L8c64f5a4; /* bf/s 0x8c64f598 */
L8c64f5a4: *stop=0x8c64f5a4u;if(*pr!=entry_pr)return -3;*stop=0x8c64f5a6u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_view_setup_8c605006(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c605006;
L8c605006: *stop=0x8c605006u;r[5]=0u;goto L8c605008; /* mov #0x0,r5 */
L8c605008: *stop=0x8c605008u;r[3]=rd(m,bus,&io_status,0x8c6051d0u,4);if(io_status)return io_status;goto L8c60500a; /* mov.l 0x8c6051d0,r3 */
L8c60500a: *stop=0x8c60500au;r[0]=64u;goto L8c60500c; /* mov #0x40,r0 */
L8c60500c: *stop=0x8c60500cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c60500e; /* mov.l r3,@r4 */
L8c60500e: *stop=0x8c60500eu;r[2]=8u;goto L8c605010; /* mov #0x8,r2 */
L8c605010: *stop=0x8c605010u;r[3]=2u;goto L8c605012; /* mov #0x2,r3 */
L8c605012: *stop=0x8c605012u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c605014; /* mov.l r5,@(0x4,r4) */
L8c605014: *stop=0x8c605014u;r[7]=6u;goto L8c605016; /* mov #0x6,r7 */
L8c605016: *stop=0x8c605016u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;goto L8c605018; /* mov.l r5,@(0x8,r4) */
L8c605018: *stop=0x8c605018u;r[6]=1u;goto L8c60501a; /* mov #0x1,r6 */
L8c60501a: *stop=0x8c60501au;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[6],4);if(io_status)return io_status;goto L8c60501c; /* mov.l r6,@(0xc,r4) */
L8c60501c: *stop=0x8c60501cu;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[5],4);if(io_status)return io_status;goto L8c60501e; /* mov.l r5,@(0x10,r4) */
L8c60501e: *stop=0x8c60501eu;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[7],4);if(io_status)return io_status;goto L8c605020; /* mov.l r7,@(0x14,r4) */
L8c605020: *stop=0x8c605020u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[6],4);if(io_status)return io_status;goto L8c605022; /* mov.l r6,@(0x18,r4) */
L8c605022: *stop=0x8c605022u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[5],4);if(io_status)return io_status;goto L8c605024; /* mov.l r5,@(0x1c,r4) */
L8c605024: *stop=0x8c605024u;if(!valid_access(r[4]+32u,4u))return -3;wr(m,bus,&io_status,r[4]+0x20u,r[6],4);if(io_status)return io_status;goto L8c605026; /* mov.l r6,@(0x20,r4) */
L8c605026: *stop=0x8c605026u;if(!valid_access(r[4]+36u,4u))return -3;wr(m,bus,&io_status,r[4]+0x24u,r[5],4);if(io_status)return io_status;goto L8c605028; /* mov.l r5,@(0x24,r4) */
L8c605028: *stop=0x8c605028u;if(!valid_access(r[4]+40u,4u))return -3;wr(m,bus,&io_status,r[4]+0x28u,r[5],4);if(io_status)return io_status;goto L8c60502a; /* mov.l r5,@(0x28,r4) */
L8c60502a: *stop=0x8c60502au;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c60502c; /* mov.l r2,@(0x2c,r4) */
L8c60502c: *stop=0x8c60502cu;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[7],4);if(io_status)return io_status;goto L8c60502e; /* mov.l r7,@(0x30,r4) */
L8c60502e: *stop=0x8c60502eu;if(!valid_access(r[4]+52u,4u))return -3;wr(m,bus,&io_status,r[4]+0x34u,r[5],4);if(io_status)return io_status;goto L8c605030; /* mov.l r5,@(0x34,r4) */
L8c605030: *stop=0x8c605030u;if(!valid_access(r[4]+56u,4u))return -3;wr(m,bus,&io_status,r[4]+0x38u,r[5],4);if(io_status)return io_status;goto L8c605032; /* mov.l r5,@(0x38,r4) */
L8c605032: *stop=0x8c605032u;if(!valid_access(r[4]+60u,4u))return -3;wr(m,bus,&io_status,r[4]+0x3cu,r[3],4);if(io_status)return io_status;goto L8c605034; /* mov.l r3,@(0x3c,r4) */
L8c605034: *stop=0x8c605034u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c605036; /* mov.l r5,@(r0,r4) */
L8c605036: *stop=0x8c605036u;r[0]=68u;goto L8c605038; /* mov #0x44,r0 */
L8c605038: *stop=0x8c605038u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c60503a; /* mov.l r5,@(r0,r4) */
L8c60503a: *stop=0x8c60503au;r[0]=72u;goto L8c60503c; /* mov #0x48,r0 */
L8c60503c: *stop=0x8c60503cu;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c60503e; /* mov.l r5,@(r0,r4) */
L8c60503e: *stop=0x8c60503eu;r[0]=80u;goto L8c605040; /* mov #0x50,r0 */
L8c605040: *stop=0x8c605040u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c605042; /* mov.l r5,@(r0,r4) */
L8c605042: *stop=0x8c605042u;r[0]=76u;goto L8c605044; /* mov #0x4c,r0 */
L8c605044: *stop=0x8c605044u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c605046; /* mov.l r5,@(r0,r4) */
L8c605046: *stop=0x8c605046u;r[0]=84u;goto L8c605048; /* mov #0x54,r0 */
L8c605048: *stop=0x8c605048u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c60504a; /* fldi1 fr4 */
L8c60504a: *stop=0x8c60504au;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c60504c; /* mov.l r5,@(r0,r4) */
L8c60504c: *stop=0x8c60504cu;r[0]=88u;goto L8c60504e; /* mov #0x58,r0 */
L8c60504e: *stop=0x8c60504eu;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c605050; /* mov.l r5,@(r0,r4) */
L8c605050: *stop=0x8c605050u;r[0]=92u;goto L8c605052; /* mov #0x5c,r0 */
L8c605052: *stop=0x8c605052u;r[3]=4u;goto L8c605054; /* mov #0x4,r3 */
L8c605054: *stop=0x8c605054u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[3],4);if(io_status)return io_status;goto L8c605056; /* mov.l r3,@(r0,r4) */
L8c605056: *stop=0x8c605056u;r[0]=96u;goto L8c605058; /* mov #0x60,r0 */
L8c605058: *stop=0x8c605058u;r[2]=3u;goto L8c60505a; /* mov #0x3,r2 */
L8c60505a: *stop=0x8c60505au;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[2],4);if(io_status)return io_status;goto L8c60505c; /* mov.l r2,@(r0,r4) */
L8c60505c: *stop=0x8c60505cu;r[0]=108u;goto L8c60505e; /* mov #0x6c,r0 */
L8c60505e: *stop=0x8c60505eu;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c605060; /* mov.l r5,@(r0,r4) */
L8c605060: *stop=0x8c605060u;r[0]=100u;goto L8c605062; /* mov #0x64,r0 */
L8c605062: *stop=0x8c605062u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c605064; /* mov.l r5,@(r0,r4) */
L8c605064: *stop=0x8c605064u;r[0]=104u;goto L8c605066; /* mov #0x68,r0 */
L8c605066: *stop=0x8c605066u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c605068; /* mov.l r5,@(r0,r4) */
L8c605068: *stop=0x8c605068u;r[0]=112u;goto L8c60506a; /* mov #0x70,r0 */
L8c60506a: *stop=0x8c60506au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c60506c; /* fmov fr4,@(r0,r4) */
L8c60506c: *stop=0x8c60506cu;r[0]=116u;goto L8c60506e; /* mov #0x74,r0 */
L8c60506e: *stop=0x8c60506eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c605070; /* fmov fr4,@(r0,r4) */
L8c605070: *stop=0x8c605070u;r[0]=120u;goto L8c605072; /* mov #0x78,r0 */
L8c605072: *stop=0x8c605072u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c605074; /* fmov fr4,@(r0,r4) */
L8c605074: *stop=0x8c605074u;r[0]=124u;goto L8c605076; /* mov #0x7c,r0 */
L8c605076: *stop=0x8c605076u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c605078; /* fmov fr4,@(r0,r4) */
L8c605078: *stop=0x8c605078u;r[0]+=4u;goto L8c60507a; /* add #0x4,r0 */
L8c60507a: *stop=0x8c60507au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c60507c; /* fmov fr4,@(r0,r4) */
L8c60507c: *stop=0x8c60507cu;r[0]+=4u;goto L8c60507e; /* add #0x4,r0 */
L8c60507e: *stop=0x8c60507eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c605080; /* fldi0 fr4 */
L8c605080: *stop=0x8c605080u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c605082; /* fmov fr4,@(r0,r4) */
L8c605082: *stop=0x8c605082u;r[0]+=4u;goto L8c605084; /* add #0x4,r0 */
L8c605084: *stop=0x8c605084u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c605086; /* fmov fr4,@(r0,r4) */
L8c605086: *stop=0x8c605086u;r[3]=rd(m,bus,&io_status,0x8c6051d4u,4);if(io_status)return io_status;goto L8c605088; /* mov.l 0x8c6051d4,r3 */
L8c605088: *stop=0x8c605088u;r[0]+=4u;goto L8c60508a; /* add #0x4,r0 */
L8c60508a: target=r[3];*stop=0x8c60508cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_view_setup_8c652f66(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c652f66;
L8c652f66: *stop=0x8c652f66u;r[0]=0x8c653008u;goto L8c652f68; /* mova 0x8c653008,r0 */
L8c652f68: *stop=0x8c652f68u;r[1]=rd(m,bus,&io_status,0x8c65300cu,4);if(io_status)return io_status;goto L8c652f6a; /* mov.l 0x8c65300c,r1 */
L8c652f6a: *stop=0x8c652f6au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c652f6c; /* fmov.s @r0,fr4 */
L8c652f6c: *stop=0x8c652f6cu;r[0]=112u;goto L8c652f6e; /* mov #0x70,r0 */
L8c652f6e: *stop=0x8c652f6eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c652f70; /* fmov.s @(r0,r4),fr3 */
L8c652f70: *stop=0x8c652f70u;r[0]=116u;goto L8c652f72; /* mov #0x74,r0 */
L8c652f72: *stop=0x8c652f72u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c652f74; /* fmul fr4,fr3 */
L8c652f74: *stop=0x8c652f74u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c652f76; /* ftrc fr3,FPUL */
L8c652f76: *stop=0x8c652f76u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c652f78; /* fmov.s @(r0,r4),fr3 */
L8c652f78: *stop=0x8c652f78u;r[0]=120u;goto L8c652f7a; /* mov #0x78,r0 */
L8c652f7a: *stop=0x8c652f7au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c652f7c; /* fmul fr4,fr3 */
L8c652f7c: *stop=0x8c652f7cu;r[3]=*fpul;goto L8c652f7e; /* sts FPUL,r3 */
L8c652f7e: *stop=0x8c652f7eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c652f80; /* ftrc fr3,FPUL */
L8c652f80: *stop=0x8c652f80u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c652f82; /* fmov.s @(r0,r4),fr3 */
L8c652f82: *stop=0x8c652f82u;r[0]=124u;goto L8c652f84; /* mov #0x7c,r0 */
L8c652f84: *stop=0x8c652f84u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c652f86; /* fmul fr4,fr3 */
L8c652f86: *stop=0x8c652f86u;r[3]<<=16;goto L8c652f88; /* shll16 r3 */
L8c652f88: *stop=0x8c652f88u;r[3]<<=8;goto L8c652f8a; /* shll8 r3 */
L8c652f8a: *stop=0x8c652f8au;r[2]=*fpul;goto L8c652f8c; /* sts FPUL,r2 */
L8c652f8c: *stop=0x8c652f8cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c652f8e; /* ftrc fr3,FPUL */
L8c652f8e: *stop=0x8c652f8eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c652f90; /* fmov.s @(r0,r4),fr3 */
L8c652f90: *stop=0x8c652f90u;r[2]<<=16;goto L8c652f92; /* shll16 r2 */
L8c652f92: *stop=0x8c652f92u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c652f94; /* fmul fr4,fr3 */
L8c652f94: *stop=0x8c652f94u;r[3]|=r[2];goto L8c652f96; /* or r2,r3 */
L8c652f96: *stop=0x8c652f96u;r[2]=*fpul;goto L8c652f98; /* sts FPUL,r2 */
L8c652f98: *stop=0x8c652f98u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c652f9a; /* ftrc fr3,FPUL */
L8c652f9a: *stop=0x8c652f9au;r[2]<<=8;goto L8c652f9c; /* shll8 r2 */
L8c652f9c: *stop=0x8c652f9cu;r[3]|=r[2];goto L8c652f9e; /* or r2,r3 */
L8c652f9e: *stop=0x8c652f9eu;r[2]=*fpul;goto L8c652fa0; /* sts FPUL,r2 */
L8c652fa0: *stop=0x8c652fa0u;r[3]|=r[2];goto L8c652fa2; /* or r2,r3 */
L8c652fa2: *stop=0x8c652fa2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c652fa4; /* mov.l r3,@r1 */
L8c652fa4: *stop=0x8c652fa4u;r[0]+=4u;goto L8c652fa6; /* add #0x4,r0 */
L8c652fa6: *stop=0x8c652fa6u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c652fa8; /* fmov.s @(r0,r4),fr3 */
L8c652fa8: *stop=0x8c652fa8u;r[0]+=4u;goto L8c652faa; /* add #0x4,r0 */
L8c652faa: *stop=0x8c652faau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c652fac; /* fmul fr4,fr3 */
L8c652fac: *stop=0x8c652facu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c652fae; /* ftrc fr3,FPUL */
L8c652fae: *stop=0x8c652faeu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c652fb0; /* fmov.s @(r0,r4),fr3 */
L8c652fb0: *stop=0x8c652fb0u;r[0]+=4u;goto L8c652fb2; /* add #0x4,r0 */
L8c652fb2: *stop=0x8c652fb2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c652fb4; /* fmul fr4,fr3 */
L8c652fb4: *stop=0x8c652fb4u;r[3]=*fpul;goto L8c652fb6; /* sts FPUL,r3 */
L8c652fb6: *stop=0x8c652fb6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c652fb8; /* ftrc fr3,FPUL */
L8c652fb8: *stop=0x8c652fb8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c652fba; /* fmov.s @(r0,r4),fr3 */
L8c652fba: *stop=0x8c652fbau;r[0]+=4u;goto L8c652fbc; /* add #0x4,r0 */
L8c652fbc: *stop=0x8c652fbcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c652fbe; /* fmul fr4,fr3 */
L8c652fbe: *stop=0x8c652fbeu;r[3]<<=16;goto L8c652fc0; /* shll16 r3 */
L8c652fc0: *stop=0x8c652fc0u;r[3]<<=8;goto L8c652fc2; /* shll8 r3 */
L8c652fc2: *stop=0x8c652fc2u;r[2]=*fpul;goto L8c652fc4; /* sts FPUL,r2 */
L8c652fc4: *stop=0x8c652fc4u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c652fc6; /* ftrc fr3,FPUL */
L8c652fc6: *stop=0x8c652fc6u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c652fc8; /* fmov.s @(r0,r4),fr3 */
L8c652fc8: *stop=0x8c652fc8u;r[2]<<=16;goto L8c652fca; /* shll16 r2 */
L8c652fca: *stop=0x8c652fcau;r[0]=rd(m,bus,&io_status,0x8c653010u,4);if(io_status)return io_status;goto L8c652fcc; /* mov.l 0x8c653010,r0 */
L8c652fcc: *stop=0x8c652fccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c652fce; /* fmul fr4,fr3 */
L8c652fce: *stop=0x8c652fceu;r[3]|=r[2];goto L8c652fd0; /* or r2,r3 */
L8c652fd0: *stop=0x8c652fd0u;r[2]=*fpul;goto L8c652fd2; /* sts FPUL,r2 */
L8c652fd2: *stop=0x8c652fd2u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c652fd4; /* ftrc fr3,FPUL */
L8c652fd4: *stop=0x8c652fd4u;r[2]<<=8;goto L8c652fd6; /* shll8 r2 */
L8c652fd6: *stop=0x8c652fd6u;r[3]|=r[2];goto L8c652fd8; /* or r2,r3 */
L8c652fd8: *stop=0x8c652fd8u;r[2]=*fpul;goto L8c652fda; /* sts FPUL,r2 */
L8c652fda: *stop=0x8c652fdau;r[3]|=r[2];goto L8c652fdc; /* or r2,r3 */
L8c652fdc: *stop=0x8c652fdcu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c652fde; /* mov.l r3,@r0 */
L8c652fde: *stop=0x8c652fdeu;r[0]=sx(m,bus,&io_status,0x8c653002u,2);if(io_status)return io_status;goto L8c652fe0; /* mov.w 0x8c653002,r0 */
L8c652fe0: *stop=0x8c652fe0u;r[3]=rd(m,bus,&io_status,0x8c653014u,4);if(io_status)return io_status;goto L8c652fe2; /* mov.l 0x8c653014,r3 */
L8c652fe2: *stop=0x8c652fe2u;if(!valid_access(r[0]+r[4],4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;goto L8c652fe4; /* mov.l @(r0,r4),r2 */
L8c652fe4: *stop=0x8c652fe4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c652fe6; /* mov.l r2,@r3 */
L8c652fe6: *stop=0x8c652fe6u;r[0]+=4u;goto L8c652fe8; /* add #0x4,r0 */
L8c652fe8: *stop=0x8c652fe8u;r[2]=rd(m,bus,&io_status,0x8c653018u,4);if(io_status)return io_status;goto L8c652fea; /* mov.l 0x8c653018,r2 */
L8c652fea: *stop=0x8c652feau;if(!valid_access(r[0]+r[4],4u))return -3;r[1]=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;goto L8c652fec; /* mov.l @(r0,r4),r1 */
L8c652fec: *stop=0x8c652fecu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c652fee; /* mov.l r1,@r2 */
L8c652fee: *stop=0x8c652feeu;r[0]+=4u;goto L8c652ff0; /* add #0x4,r0 */
L8c652ff0: *stop=0x8c652ff0u;r[1]=rd(m,bus,&io_status,0x8c65301cu,4);if(io_status)return io_status;goto L8c652ff2; /* mov.l 0x8c65301c,r1 */
L8c652ff2: *stop=0x8c652ff2u;if(!valid_access(r[0]+r[4],4u))return -3;r[3]=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;goto L8c652ff4; /* mov.l @(r0,r4),r3 */
L8c652ff4: *stop=0x8c652ff4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c652ff6; /* mov.l r3,@r1 */
L8c652ff6: *stop=0x8c652ff6u;if(*pr!=entry_pr)return -3;*stop=0x8c652ff8u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_view_setup_8c604fd0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c604fd0;
L8c604fd0: *stop=0x8c604fd0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c604fd2; /* sts.l PR,@-r15 */
L8c604fd2: *stop=0x8c604fd2u;r[15]+=4294967292u;goto L8c604fd4; /* add #-0x4,r15 */
L8c604fd4: *stop=0x8c604fd4u;r[3]=r[4];goto L8c604fd6; /* mov r4,r3 */
L8c604fd6: *stop=0x8c604fd6u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c604fd8; /* shll r3 */
L8c604fd8: *stop=0x8c604fd8u;r[2]=r[4];goto L8c604fda; /* mov r4,r2 */
L8c604fda: *stop=0x8c604fdau;r[3]+=r[2];goto L8c604fdc; /* add r2,r3 */
L8c604fdc: *stop=0x8c604fdcu;r[3]<<=2;goto L8c604fde; /* shll2 r3 */
L8c604fde: *stop=0x8c604fdeu;r[1]=rd(m,bus,&io_status,0x8c6051bcu,4);if(io_status)return io_status;goto L8c604fe0; /* mov.l 0x8c6051bc,r1 */
L8c604fe0: *stop=0x8c604fe0u;r[3]<<=2;goto L8c604fe2; /* shll2 r3 */
L8c604fe2: *stop=0x8c604fe2u;r[2]=rd(m,bus,&io_status,0x8c6051c0u,4);if(io_status)return io_status;goto L8c604fe4; /* mov.l 0x8c6051c0,r2 */
L8c604fe4: *stop=0x8c604fe4u;r[3]<<=2;goto L8c604fe6; /* shll2 r3 */
L8c604fe6: *stop=0x8c604fe6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c604fe8; /* mov.l r4,@r15 */
L8c604fe8: *stop=0x8c604fe8u;r[3]+=r[1];goto L8c604fea; /* add r1,r3 */
L8c604fea: *stop=0x8c604feau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c604fec; /* mov.l r3,@r2 */
L8c604fec: *stop=0x8c604fecu;r[2]=rd(m,bus,&io_status,0x8c6051c4u,4);if(io_status)return io_status;goto L8c604fee; /* mov.l 0x8c6051c4,r2 */
L8c604fee: target=r[2];*pr=0x8c604ff2u;*stop=0x8c604ff0u;r[4]=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604ff2; /* jsr @r2 */
L8c604ff2: *stop=0x8c604ff2u;if(!valid_access(r[15],4u))return -3;r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c604ff4; /* mov.l @r15,r1 */
L8c604ff4: *stop=0x8c604ff4u;r[3]=rd(m,bus,&io_status,0x8c6051c8u,4);if(io_status)return io_status;goto L8c604ff6; /* mov.l 0x8c6051c8,r3 */
L8c604ff6: *stop=0x8c604ff6u;r[2]=rd(m,bus,&io_status,0x8c6051ccu,4);if(io_status)return io_status;goto L8c604ff8; /* mov.l 0x8c6051cc,r2 */
L8c604ff8: *stop=0x8c604ff8u;r[1]<<=2;goto L8c604ffa; /* shll2 r1 */
L8c604ffa: *stop=0x8c604ffau;r[1]+=r[3];goto L8c604ffc; /* add r3,r1 */
L8c604ffc: *stop=0x8c604ffcu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c604ffe; /* mov.l r1,@r2 */
L8c604ffe: *stop=0x8c604ffeu;r[15]+=4u;goto L8c605000; /* add #0x4,r15 */
L8c605000: *stop=0x8c605000u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c605002; /* lds.l @r15+,PR */
L8c605002: *stop=0x8c605002u;if(*pr!=entry_pr)return -3;*stop=0x8c605004u;return 0; /* rts */
}
