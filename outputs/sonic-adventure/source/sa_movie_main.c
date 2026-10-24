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
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_main_8c10b8a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10b8a6;
L8c10b8a6: *stop=0x8c10b8a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10b8a8; /* sts.l PR,@-r15 */
L8c10b8a8: *stop=0x8c10b8a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b95cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10b8aa; /* mov.l 0x8c10b95c,r3 */
L8c10b8aa: target=r[3];*pr=0x8c10b8aeu;*stop=0x8c10b8acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b8ae; /* jsr @r3 */
L8c10b8ae: *stop=0x8c10b8aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b960u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10b8b0; /* mov.l 0x8c10b960,r2 */
L8c10b8b0: target=r[2];*pr=0x8c10b8b4u;*stop=0x8c10b8b2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b8b4; /* jsr @r2 */
L8c10b8b4: *stop=0x8c10b8b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10b8b6; /* mov.l @r15,r3 */
L8c10b8b6: *stop=0x8c10b8b6u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c10b8b8; /* tst r3,r3 */
L8c10b8b8: if(!(*sr&1u))goto L8c10b8dc;goto L8c10b8ba; /* bf 0x8c10b8dc */
L8c10b8ba: *stop=0x8c10b8bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b964u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10b8bc; /* mov.l 0x8c10b964,r3 */
L8c10b8bc: *stop=0x8c10b8bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b968u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10b8be; /* mov.l 0x8c10b968,r2 */
L8c10b8be: *stop=0x8c10b8beu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[6]=loaded;}goto L8c10b8c0; /* mov.l @r3,r6 */
L8c10b8c0: *stop=0x8c10b8c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b970u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10b8c2; /* mov.l 0x8c10b970,r3 */
L8c10b8c2: *stop=0x8c10b8c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b96cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10b8c4; /* mov.l 0x8c10b96c,r1 */
L8c10b8c4: *stop=0x8c10b8c4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10b8c6; /* mov.l @r2,r5 */
L8c10b8c6: target=r[3];*pr=0x8c10b8cau;*stop=0x8c10b8c8u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b8ca; /* jsr @r3 */
L8c10b8ca: *stop=0x8c10b8cau;r[6]=78u;goto L8c10b8cc; /* mov #0x4e,r6 */
L8c10b8cc: *stop=0x8c10b8ccu;r[2]=0u;goto L8c10b8ce; /* mov #0x0,r2 */
L8c10b8ce: *stop=0x8c10b8ceu;r[6]<<=8;goto L8c10b8d0; /* shll8 r6 */
L8c10b8d0: *stop=0x8c10b8d0u;r[5]=r[2];goto L8c10b8d2; /* mov r2,r5 */
L8c10b8d2: *stop=0x8c10b8d2u;r[6]+=32u;goto L8c10b8d4; /* add #0x20,r6 */
L8c10b8d4: *stop=0x8c10b8d4u;r[7]=r[2];goto L8c10b8d6; /* mov r2,r7 */
L8c10b8d6: *stop=0x8c10b8d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10b8d8; /* mov.l r2,@-r15 */
L8c10b8d8: *stop=0x8c10b8dau;goto L8c10b8f2; /* bra 0x8c10b8f2 */
L8c10b8dc: *stop=0x8c10b8dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b974u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10b8de; /* mov.l 0x8c10b974,r3 */
L8c10b8de: *stop=0x8c10b8deu;r[6]=1u;goto L8c10b8e0; /* mov #0x1,r6 */
L8c10b8e0: *stop=0x8c10b8e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b970u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10b8e2; /* mov.l 0x8c10b970,r2 */
L8c10b8e2: *stop=0x8c10b8e2u;r[5]=0u;goto L8c10b8e4; /* mov #0x0,r5 */
L8c10b8e4: target=r[2];*pr=0x8c10b8e8u;*stop=0x8c10b8e6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b8e8; /* jsr @r2 */
L8c10b8e8: *stop=0x8c10b8e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b978u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10b8ea; /* mov.l 0x8c10b978,r1 */
L8c10b8ea: *stop=0x8c10b8eau;r[7]=0u;goto L8c10b8ec; /* mov #0x0,r7 */
L8c10b8ec: *stop=0x8c10b8ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b97cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c10b8ee; /* mov.l 0x8c10b97c,r6 */
L8c10b8ee: *stop=0x8c10b8eeu;r[5]=r[7];goto L8c10b8f0; /* mov r7,r5 */
L8c10b8f0: *stop=0x8c10b8f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c10b8f2; /* mov.l r1,@-r15 */
L8c10b8f2: *stop=0x8c10b8f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b980u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10b8f4; /* mov.l 0x8c10b980,r3 */
L8c10b8f4: target=r[3];*pr=0x8c10b8f8u;*stop=0x8c10b8f6u;r[4]=4294967295u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b8f8; /* jsr @r3 */
L8c10b8f8: *stop=0x8c10b8f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b984u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10b8fa; /* mov.l 0x8c10b984,r2 */
L8c10b8fa: *stop=0x8c10b8fau;r[6]=0u;goto L8c10b8fc; /* mov #0x0,r6 */
L8c10b8fc: *stop=0x8c10b8fcu;r[5]=r[6];goto L8c10b8fe; /* mov r6,r5 */
L8c10b8fe: target=r[2];*pr=0x8c10b902u;*stop=0x8c10b900u;r[4]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b902; /* jsr @r2 */
L8c10b902: *stop=0x8c10b902u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b988u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10b904; /* mov.l 0x8c10b988,r3 */
L8c10b904: *stop=0x8c10b904u;r[6]=0u;goto L8c10b906; /* mov #0x0,r6 */
L8c10b906: *stop=0x8c10b906u;r[5]=r[6];goto L8c10b908; /* mov r6,r5 */
L8c10b908: target=r[3];*pr=0x8c10b90cu;*stop=0x8c10b90au;r[4]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10b90c; /* jsr @r3 */
L8c10b90c: *stop=0x8c10b90cu;r[4]=16u;goto L8c10b90e; /* mov #0x10,r4 */
L8c10b90e: *stop=0x8c10b90eu;r[15]+=8u;goto L8c10b910; /* add #0x8,r15 */
L8c10b910: *stop=0x8c10b910u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10b98cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10b912; /* mov.l 0x8c10b98c,r2 */
L8c10b912: target=r[2];*stop=0x8c10b914u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_movie_main_8c6436d0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6436d0;
L8c6436d0: *stop=0x8c6436d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6436e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6436d2; /* mov.l 0x8c6436e4,r3 */
L8c6436d2: *stop=0x8c6436d2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6436d4; /* mov.l @r3,r5 */
L8c6436d4: *stop=0x8c6436d4u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c6436d6; /* tst r5,r5 */
L8c6436d6: if((*sr&1u))goto L8c6436dc;goto L8c6436d8; /* bt 0x8c6436dc */
L8c6436d8: *stop=0x8c6436d8u;r[0]=sx(m,bus,&io_status,0x8c6436e0u,2);if(io_status)return io_status;goto L8c6436da; /* mov.w 0x8c6436e0,r0 */
L8c6436da: *stop=0x8c6436dau;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[4],4);if(io_status)return io_status;goto L8c6436dc; /* mov.l r4,@(r0,r5) */
L8c6436dc: *stop=0x8c6436dcu;if(*pr!=entry_pr)return -3;*stop=0x8c6436deu;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c0509a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0509a0;
L8c0509a0: *stop=0x8c0509a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050a20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0509a2; /* mov.l 0x8c050a20,r2 */
L8c0509a2: *stop=0x8c0509a2u;r[3]=1u;goto L8c0509a4; /* mov #0x1,r3 */
L8c0509a4: *stop=0x8c0509a4u;if(*pr!=entry_pr)return -3;*stop=0x8c0509a6u;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[3],1);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c050be4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c050be4;
L8c050be4: *stop=0x8c050be4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c050be6; /* mov.l r14,@-r15 */
L8c050be6: *stop=0x8c050be6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c050be8; /* mov.l r13,@-r15 */
L8c050be8: *stop=0x8c050be8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c050bea; /* mov.l r12,@-r15 */
L8c050bea: *stop=0x8c050beau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c050bec; /* mov.l r11,@-r15 */
L8c050bec: *stop=0x8c050becu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c050bee; /* mov.l r10,@-r15 */
L8c050bee: *stop=0x8c050beeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c050bf0; /* mov.l r9,@-r15 */
L8c050bf0: *stop=0x8c050bf0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c050bf2; /* mov.l r8,@-r15 */
L8c050bf2: *stop=0x8c050bf2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c050bf4; /* sts.l PR,@-r15 */
L8c050bf4: *stop=0x8c050bf4u;r[15]+=4294967284u;goto L8c050bf6; /* add #-0xc,r15 */
L8c050bf6: *stop=0x8c050bf6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d0cu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c050bf8; /* mov.l 0x8c050d0c,r9 */
L8c050bf8: *stop=0x8c050bf8u;r[14]=0u;goto L8c050bfa; /* mov #0x0,r14 */
L8c050bfa: *stop=0x8c050bfau;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050bfc; /* mov.l 0x8c050d10,r2 */
L8c050bfc: *stop=0x8c050bfcu;r[13]=4294967295u;goto L8c050bfe; /* mov #-0x1,r13 */
L8c050bfe: *stop=0x8c050bfeu;r[3]=r[9];goto L8c050c00; /* mov r9,r3 */
L8c050c00: *stop=0x8c050c00u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c050c02; /* mov.l r3,@r15 */
L8c050c02: *stop=0x8c050c02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d14u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c050c04; /* mov.l 0x8c050d14,r10 */
L8c050c04: *stop=0x8c050c04u;r[12]=r[14];goto L8c050c06; /* mov r14,r12 */
L8c050c06: *stop=0x8c050c06u;if(!valid_access(r[9]+12u,4u))return -3;wr(m,bus,&io_status,r[9]+0xcu,r[2],4);if(io_status)return io_status;goto L8c050c08; /* mov.l r2,@(0xc,r9) */
L8c050c08: *stop=0x8c050c08u;if(!valid_access(r[9]+8u,4u))return -3;wr(m,bus,&io_status,r[9]+0x8u,r[2],4);if(io_status)return io_status;goto L8c050c0a; /* mov.l r2,@(0x8,r9) */
L8c050c0a: *stop=0x8c050c0au;if(!valid_access(r[9]+4u,4u))return -3;wr(m,bus,&io_status,r[9]+0x4u,r[2],4);if(io_status)return io_status;goto L8c050c0c; /* mov.l r2,@(0x4,r9) */
L8c050c0c: *stop=0x8c050c0cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c050c0e; /* mov.l r2,@r3 */
L8c050c0e: *stop=0x8c050c0eu;if(!valid_access(r[10]+12u,4u))return -3;wr(m,bus,&io_status,r[10]+0xcu,r[13],4);if(io_status)return io_status;goto L8c050c10; /* mov.l r13,@(0xc,r10) */
L8c050c10: *stop=0x8c050c10u;if(!valid_access(r[10]+8u,4u))return -3;wr(m,bus,&io_status,r[10]+0x8u,r[13],4);if(io_status)return io_status;goto L8c050c12; /* mov.l r13,@(0x8,r10) */
L8c050c12: *stop=0x8c050c12u;if(!valid_access(r[10]+4u,4u))return -3;wr(m,bus,&io_status,r[10]+0x4u,r[13],4);if(io_status)return io_status;goto L8c050c14; /* mov.l r13,@(0x4,r10) */
L8c050c14: *stop=0x8c050c14u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[13],4);if(io_status)return io_status;goto L8c050c16; /* mov.l r13,@r10 */
L8c050c16: *stop=0x8c050c16u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[14],4);if(io_status)return io_status;goto L8c050c18; /* mov.l r14,@(0x4,r15) */
L8c050c18: *stop=0x8c050c18u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050c1a; /* mov.l @r15,r3 */
L8c050c1a: *stop=0x8c050c1au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c050c1c; /* mov.l r3,@-r15 */
L8c050c1c: *stop=0x8c050c1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050c1e; /* mov.l 0x8c050d18,r3 */
L8c050c1e: target=r[3];*pr=0x8c050c22u;*stop=0x8c050c20u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c050c22; /* jsr @r3 */
L8c050c22: *stop=0x8c050c22u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;goto L8c050c24; /* mov.l @r15+,r2 */
L8c050c24: *stop=0x8c050c24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d1cu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c050c26; /* mov.l 0x8c050d1c,r11 */
L8c050c26: *stop=0x8c050c26u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c050c28; /* mov.l r0,@r2 */
L8c050c28: *stop=0x8c050c28u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050c2a; /* mov.l @r15,r3 */
L8c050c2a: *stop=0x8c050c2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d20u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c050c2c; /* mov.l 0x8c050d20,r4 */
L8c050c2c: *stop=0x8c050c2cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c050c2e; /* mov.l @r3,r2 */
L8c050c2e: *stop=0x8c050c2eu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050c30; /* mov.l @(0x4,r2),r1 */
L8c050c30: *stop=0x8c050c30u;r[1]&=r[11];goto L8c050c32; /* and r11,r1 */
L8c050c32: *stop=0x8c050c32u;*sr=(*sr&~1u)|(r[11]==r[1]);goto L8c050c34; /* cmp/eq r11,r1 */
L8c050c34: take=!(*sr&1u);*stop=0x8c050c36u;r[8]=1u;if(take)goto L8c050c42;goto L8c050c38; /* bf/s 0x8c050c42 */
L8c050c38: *stop=0x8c050c38u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[8],4);if(io_status)return io_status;goto L8c050c3a; /* mov.l r8,@r4 */
L8c050c3a: *stop=0x8c050c3au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050c3c; /* mov.l @(0x4,r15),r2 */
L8c050c3c: *stop=0x8c050c3cu;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[2],4);if(io_status)return io_status;goto L8c050c3e; /* mov.l r2,@r10 */
L8c050c3e: *stop=0x8c050c40u;r[12]+=1u;goto L8c050c44; /* bra 0x8c050c44 */
L8c050c42: *stop=0x8c050c42u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c050c44; /* mov.l r14,@r4 */
L8c050c44: *stop=0x8c050c44u;r[3]=r[12];goto L8c050c46; /* mov r12,r3 */
L8c050c46: *stop=0x8c050c46u;r[3]<<=2;goto L8c050c48; /* shll2 r3 */
L8c050c48: *stop=0x8c050c48u;r[3]+=r[9];goto L8c050c4a; /* add r9,r3 */
L8c050c4a: *stop=0x8c050c4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050c4c; /* mov.l 0x8c050d18,r2 */
L8c050c4c: *stop=0x8c050c4cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[8],4);if(io_status)return io_status;goto L8c050c4e; /* mov.l r8,@(0x4,r15) */
L8c050c4e: *stop=0x8c050c4eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c050c50; /* mov.l r3,@r15 */
L8c050c50: *stop=0x8c050c50u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c050c52; /* mov.l r3,@-r15 */
L8c050c52: target=r[2];*pr=0x8c050c56u;*stop=0x8c050c54u;r[4]=6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c050c56; /* jsr @r2 */
L8c050c56: *stop=0x8c050c56u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c050c58; /* mov.l @r15+,r1 */
L8c050c58: *stop=0x8c050c58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d24u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c050c5a; /* mov.l 0x8c050d24,r4 */
L8c050c5a: *stop=0x8c050c5au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c050c5c; /* mov.l r0,@r1 */
L8c050c5c: *stop=0x8c050c5cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050c5e; /* mov.l @r15,r3 */
L8c050c5e: *stop=0x8c050c5eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c050c60; /* mov.l @r3,r2 */
L8c050c60: *stop=0x8c050c60u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050c62; /* mov.l @(0x4,r2),r1 */
L8c050c62: *stop=0x8c050c62u;r[1]&=r[11];goto L8c050c64; /* and r11,r1 */
L8c050c64: *stop=0x8c050c64u;*sr=(*sr&~1u)|(r[11]==r[1]);goto L8c050c66; /* cmp/eq r11,r1 */
L8c050c66: take=!(*sr&1u);*stop=0x8c050c68u;r[2]=2u;if(take)goto L8c050c78;goto L8c050c6a; /* bf/s 0x8c050c78 */
L8c050c6a: *stop=0x8c050c6au;r[0]=r[12];goto L8c050c6c; /* mov r12,r0 */
L8c050c6c: *stop=0x8c050c6cu;r[0]<<=2;goto L8c050c6e; /* shll2 r0 */
L8c050c6e: *stop=0x8c050c6eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[8],4);if(io_status)return io_status;goto L8c050c70; /* mov.l r8,@r4 */
L8c050c70: *stop=0x8c050c70u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050c72; /* mov.l @(0x4,r15),r3 */
L8c050c72: *stop=0x8c050c72u;if(!valid_access(r[0]+r[10],4u))return -3;wr(m,bus,&io_status,r[10]+r[0],r[3],4);if(io_status)return io_status;goto L8c050c74; /* mov.l r3,@(r0,r10) */
L8c050c74: *stop=0x8c050c76u;r[12]+=1u;goto L8c050c7a; /* bra 0x8c050c7a */
L8c050c78: *stop=0x8c050c78u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c050c7a; /* mov.l r14,@r4 */
L8c050c7a: *stop=0x8c050c7au;r[3]=r[12];goto L8c050c7c; /* mov r12,r3 */
L8c050c7c: *stop=0x8c050c7cu;r[3]<<=2;goto L8c050c7e; /* shll2 r3 */
L8c050c7e: *stop=0x8c050c7eu;r[3]+=r[9];goto L8c050c80; /* add r9,r3 */
L8c050c80: *stop=0x8c050c80u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c050c82; /* mov.l r2,@(0x4,r15) */
L8c050c82: *stop=0x8c050c82u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050c84; /* mov.l 0x8c050d18,r2 */
L8c050c84: *stop=0x8c050c84u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c050c86; /* mov.l r3,@r15 */
L8c050c86: *stop=0x8c050c86u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c050c88; /* mov.l r3,@-r15 */
L8c050c88: target=r[2];*pr=0x8c050c8cu;*stop=0x8c050c8au;r[4]=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c050c8c; /* jsr @r2 */
L8c050c8c: *stop=0x8c050c8cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c050c8e; /* mov.l @r15+,r1 */
L8c050c8e: *stop=0x8c050c8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d28u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c050c90; /* mov.l 0x8c050d28,r4 */
L8c050c90: *stop=0x8c050c90u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c050c92; /* mov.l r0,@r1 */
L8c050c92: *stop=0x8c050c92u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050c94; /* mov.l @r15,r3 */
L8c050c94: *stop=0x8c050c94u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c050c96; /* mov.l @r3,r2 */
L8c050c96: *stop=0x8c050c96u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050c98; /* mov.l @(0x4,r2),r1 */
L8c050c98: *stop=0x8c050c98u;r[1]&=r[11];goto L8c050c9a; /* and r11,r1 */
L8c050c9a: *stop=0x8c050c9au;*sr=(*sr&~1u)|(r[11]==r[1]);goto L8c050c9c; /* cmp/eq r11,r1 */
L8c050c9c: take=!(*sr&1u);*stop=0x8c050c9eu;r[2]=3u;if(take)goto L8c050cae;goto L8c050ca0; /* bf/s 0x8c050cae */
L8c050ca0: *stop=0x8c050ca0u;r[0]=r[12];goto L8c050ca2; /* mov r12,r0 */
L8c050ca2: *stop=0x8c050ca2u;r[0]<<=2;goto L8c050ca4; /* shll2 r0 */
L8c050ca4: *stop=0x8c050ca4u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[8],4);if(io_status)return io_status;goto L8c050ca6; /* mov.l r8,@r4 */
L8c050ca6: *stop=0x8c050ca6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050ca8; /* mov.l @(0x4,r15),r3 */
L8c050ca8: *stop=0x8c050ca8u;if(!valid_access(r[0]+r[10],4u))return -3;wr(m,bus,&io_status,r[10]+r[0],r[3],4);if(io_status)return io_status;goto L8c050caa; /* mov.l r3,@(r0,r10) */
L8c050caa: *stop=0x8c050cacu;r[12]+=1u;goto L8c050cb0; /* bra 0x8c050cb0 */
L8c050cae: *stop=0x8c050caeu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c050cb0; /* mov.l r14,@r4 */
L8c050cb0: *stop=0x8c050cb0u;r[3]=r[12];goto L8c050cb2; /* mov r12,r3 */
L8c050cb2: *stop=0x8c050cb2u;r[3]<<=2;goto L8c050cb4; /* shll2 r3 */
L8c050cb4: *stop=0x8c050cb4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c050cb6; /* mov.l r2,@r15 */
L8c050cb6: *stop=0x8c050cb6u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c050cb8; /* mov.l r3,@(0x4,r15) */
L8c050cb8: *stop=0x8c050cb8u;r[3]+=r[9];goto L8c050cba; /* add r9,r3 */
L8c050cba: *stop=0x8c050cbau;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050cbc; /* mov.l 0x8c050d18,r2 */
L8c050cbc: *stop=0x8c050cbcu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c050cbe; /* mov.l r3,@(0x8,r15) */
L8c050cbe: *stop=0x8c050cbeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c050cc0; /* mov.l r3,@-r15 */
L8c050cc0: target=r[2];*pr=0x8c050cc4u;*stop=0x8c050cc2u;r[4]=18u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c050cc4; /* jsr @r2 */
L8c050cc4: *stop=0x8c050cc4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c050cc6; /* mov.l @r15+,r1 */
L8c050cc6: *stop=0x8c050cc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050d2cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c050cc8; /* mov.l 0x8c050d2c,r4 */
L8c050cc8: *stop=0x8c050cc8u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c050cca; /* mov.l r0,@r1 */
L8c050cca: *stop=0x8c050ccau;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050ccc; /* mov.l @(0x8,r15),r3 */
L8c050ccc: *stop=0x8c050cccu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c050cce; /* mov.l @r3,r2 */
L8c050cce: *stop=0x8c050cceu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050cd0; /* mov.l @(0x4,r2),r1 */
L8c050cd0: *stop=0x8c050cd0u;r[1]&=r[11];goto L8c050cd2; /* and r11,r1 */
L8c050cd2: *stop=0x8c050cd2u;*sr=(*sr&~1u)|(r[11]==r[1]);goto L8c050cd4; /* cmp/eq r11,r1 */
L8c050cd4: if(!(*sr&1u))goto L8c050d40;goto L8c050cd6; /* bf 0x8c050d40 */
L8c050cd6: *stop=0x8c050cd6u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[8],4);if(io_status)return io_status;goto L8c050cd8; /* mov.l r8,@r4 */
L8c050cd8: *stop=0x8c050cd8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050cda; /* mov.l @r15,r3 */
L8c050cda: *stop=0x8c050cdau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050cdc; /* mov.l @(0x4,r15),r0 */
L8c050cdc: *stop=0x8c050cdcu;if(!valid_access(r[0]+r[10],4u))return -3;wr(m,bus,&io_status,r[10]+r[0],r[3],4);if(io_status)return io_status;goto L8c050cde; /* mov.l r3,@(r0,r10) */
L8c050cde: *stop=0x8c050ce0u;r[12]+=1u;goto L8c050d42; /* bra 0x8c050d42 */
L8c050d40: *stop=0x8c050d40u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c050d42; /* mov.l r14,@r4 */
L8c050d42: *stop=0x8c050d42u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c050d44; /* tst r12,r12 */
L8c050d44: if(!(*sr&1u))goto L8c050d4a;goto L8c050d46; /* bf 0x8c050d4a */
L8c050d46: *stop=0x8c050d46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050ee0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050d48; /* mov.l 0x8c050ee0,r3 */
L8c050d48: *stop=0x8c050d48u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[3],4);if(io_status)return io_status;goto L8c050d4a; /* mov.l r3,@r9 */
L8c050d4a: *stop=0x8c050d4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c050ee4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050d4c; /* mov.l 0x8c050ee4,r2 */
L8c050d4c: target=r[2];*pr=0x8c050d50u;*stop=0x8c050d4eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c050d50; /* jsr @r2 */
L8c050d50: *stop=0x8c050d50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050ef0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050d52; /* mov.l 0x8c050ef0,r3 */
L8c050d52: *stop=0x8c050d52u;r[6]=4294967168u;goto L8c050d54; /* mov #-0x80,r6 */
L8c050d54: *stop=0x8c050d54u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050ee8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c050d56; /* mov.l 0x8c050ee8,r12 */
L8c050d56: *stop=0x8c050d56u;r[6]<<=8;goto L8c050d58; /* shll8 r6 */
L8c050d58: *stop=0x8c050d58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050eecu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c050d5a; /* mov.l 0x8c050eec,r5 */
L8c050d5a: *stop=0x8c050d5au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c050d5c; /* mov.l r12,@r15 */
L8c050d5c: *stop=0x8c050d5cu;if(!valid_access(r[3],1u))return -3;r[0]=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;goto L8c050d5e; /* mov.b @r3,r0 */
L8c050d5e: *stop=0x8c050d5eu;r[0]=r[0]&255u;goto L8c050d60; /* extu.b r0,r0 */
L8c050d60: *stop=0x8c050d60u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050d62; /* cmp/eq #0x1,r0 */
L8c050d62: if((*sr&1u))goto L8c050d68;goto L8c050d64; /* bt 0x8c050d68 */
L8c050d64: *stop=0x8c050d66u;goto L8c0510b8; /* bra 0x8c0510b8 */
L8c050d68: *stop=0x8c050d68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c050ef4u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c050d6a; /* mov.l 0x8c050ef4,r8 */
L8c050d6a: *stop=0x8c050d6au;r[7]=r[9];goto L8c050d6c; /* mov r9,r7 */
L8c050d6c: *stop=0x8c050d6cu;r[11]=r[9];goto L8c050d6e; /* mov r9,r11 */
L8c050d6e: *stop=0x8c050d6eu;r[7]+=4u;goto L8c050d70; /* add #0x4,r7 */
L8c050d70: *stop=0x8c050d70u;r[9]=r[8];goto L8c050d72; /* mov r8,r9 */
L8c050d72: *stop=0x8c050d72u;r[4]=r[14];goto L8c050d74; /* mov r14,r4 */
L8c050d74: *stop=0x8c050d74u;r[9]+=1u;goto L8c050d76; /* add #0x1,r9 */
L8c050d76: *stop=0x8c050d76u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[8],4);if(io_status)return io_status;goto L8c050d78; /* mov.l r8,@(0x4,r15) */
L8c050d78: *stop=0x8c050d78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}goto L8c050d7a; /* mov.l @r15,r10 */
L8c050d7a: *stop=0x8c050d7au;r[2]=r[10];goto L8c050d7c; /* mov r10,r2 */
L8c050d7c: *stop=0x8c050d7cu;r[2]+=52u;goto L8c050d7e; /* add #0x34,r2 */
L8c050d7e: *stop=0x8c050d7eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c050d80; /* mov.l r2,@(0x4,r15) */
L8c050d80: *stop=0x8c050d80u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c050d82; /* mov.l r7,@r15 */
L8c050d82: *stop=0x8c050d82u;r[3]=r[4]&255u;goto L8c050d84; /* extu.b r4,r3 */
L8c050d84: *stop=0x8c050d84u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c050d86; /* tst r3,r3 */
L8c050d86: if(!(*sr&1u))goto L8c050dc8;goto L8c050d88; /* bf 0x8c050dc8 */
L8c050d88: *stop=0x8c050d88u;if(!valid_access(r[8],1u))return -3;r[0]=sx(m,bus,&io_status,r[8],1);if(io_status)return io_status;goto L8c050d8a; /* mov.b @r8,r0 */
L8c050d8a: *stop=0x8c050d8au;r[0]=r[0]&255u;goto L8c050d8c; /* extu.b r0,r0 */
L8c050d8c: *stop=0x8c050d8cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050d8e; /* cmp/eq #0x1,r0 */
L8c050d8e: if(!(*sr&1u))goto L8c050dc8;goto L8c050d90; /* bf 0x8c050dc8 */
L8c050d90: *stop=0x8c050d90u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c050d92; /* mov.l @r11,r2 */
L8c050d92: *stop=0x8c050d92u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050d94; /* mov.l @r2,r3 */
L8c050d94: *stop=0x8c050d94u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050d96; /* mov.l @(0x4,r2),r1 */
L8c050d96: *stop=0x8c050d96u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c050d98; /* mov.l r3,@r10 */
L8c050d98: *stop=0x8c050d98u;if(!valid_access(r[10]+4u,4u))return -3;wr(m,bus,&io_status,r[10]+0x4u,r[1],4);if(io_status)return io_status;goto L8c050d9a; /* mov.l r1,@(0x4,r10) */
L8c050d9a: *stop=0x8c050d9au;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050d9c; /* mov.l @(0x8,r2),r3 */
L8c050d9c: *stop=0x8c050d9cu;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050d9e; /* mov.l @(0xc,r2),r1 */
L8c050d9e: *stop=0x8c050d9eu;if(!valid_access(r[10]+8u,4u))return -3;wr(m,bus,&io_status,r[10]+0x8u,r[3],4);if(io_status)return io_status;goto L8c050da0; /* mov.l r3,@(0x8,r10) */
L8c050da0: *stop=0x8c050da0u;if(!valid_access(r[10]+12u,4u))return -3;wr(m,bus,&io_status,r[10]+0xcu,r[1],4);if(io_status)return io_status;goto L8c050da2; /* mov.l r1,@(0xc,r10) */
L8c050da2: *stop=0x8c050da2u;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050da4; /* mov.l @(0x10,r2),r3 */
L8c050da4: *stop=0x8c050da4u;if(!valid_access(r[2]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050da6; /* mov.l @(0x14,r2),r1 */
L8c050da6: *stop=0x8c050da6u;if(!valid_access(r[10]+16u,4u))return -3;wr(m,bus,&io_status,r[10]+0x10u,r[3],4);if(io_status)return io_status;goto L8c050da8; /* mov.l r3,@(0x10,r10) */
L8c050da8: *stop=0x8c050da8u;if(!valid_access(r[10]+20u,4u))return -3;wr(m,bus,&io_status,r[10]+0x14u,r[1],4);if(io_status)return io_status;goto L8c050daa; /* mov.l r1,@(0x14,r10) */
L8c050daa: *stop=0x8c050daau;if(!valid_access(r[2]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050dac; /* mov.l @(0x18,r2),r3 */
L8c050dac: *stop=0x8c050dacu;if(!valid_access(r[2]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050dae; /* mov.l @(0x1c,r2),r1 */
L8c050dae: *stop=0x8c050daeu;if(!valid_access(r[10]+24u,4u))return -3;wr(m,bus,&io_status,r[10]+0x18u,r[3],4);if(io_status)return io_status;goto L8c050db0; /* mov.l r3,@(0x18,r10) */
L8c050db0: *stop=0x8c050db0u;if(!valid_access(r[10]+28u,4u))return -3;wr(m,bus,&io_status,r[10]+0x1cu,r[1],4);if(io_status)return io_status;goto L8c050db2; /* mov.l r1,@(0x1c,r10) */
L8c050db2: *stop=0x8c050db2u;if(!valid_access(r[2]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050db4; /* mov.l @(0x20,r2),r3 */
L8c050db4: *stop=0x8c050db4u;if(!valid_access(r[2]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050db6; /* mov.l @(0x24,r2),r1 */
L8c050db6: *stop=0x8c050db6u;if(!valid_access(r[10]+32u,4u))return -3;wr(m,bus,&io_status,r[10]+0x20u,r[3],4);if(io_status)return io_status;goto L8c050db8; /* mov.l r3,@(0x20,r10) */
L8c050db8: *stop=0x8c050db8u;if(!valid_access(r[10]+36u,4u))return -3;wr(m,bus,&io_status,r[10]+0x24u,r[1],4);if(io_status)return io_status;goto L8c050dba; /* mov.l r1,@(0x24,r10) */
L8c050dba: *stop=0x8c050dbau;if(!valid_access(r[2]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050dbc; /* mov.l @(0x28,r2),r3 */
L8c050dbc: *stop=0x8c050dbcu;if(!valid_access(r[2]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x2cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050dbe; /* mov.l @(0x2c,r2),r1 */
L8c050dbe: *stop=0x8c050dbeu;if(!valid_access(r[10]+40u,4u))return -3;wr(m,bus,&io_status,r[10]+0x28u,r[3],4);if(io_status)return io_status;goto L8c050dc0; /* mov.l r3,@(0x28,r10) */
L8c050dc0: *stop=0x8c050dc0u;if(!valid_access(r[10]+44u,4u))return -3;wr(m,bus,&io_status,r[10]+0x2cu,r[1],4);if(io_status)return io_status;goto L8c050dc2; /* mov.l r1,@(0x2c,r10) */
L8c050dc2: *stop=0x8c050dc2u;if(!valid_access(r[2]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050dc4; /* mov.l @(0x30,r2),r3 */
L8c050dc4: *stop=0x8c050dc6u;if(!valid_access(r[10]+48u,4u))return -3;wr(m,bus,&io_status,r[10]+0x30u,r[3],4);if(io_status)return io_status;goto L8c050e3e; /* bra 0x8c050e3e */
L8c050dc8: *stop=0x8c050dc8u;r[0]=r[4]&255u;goto L8c050dca; /* extu.b r4,r0 */
L8c050dca: *stop=0x8c050dcau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050dcc; /* cmp/eq #0x1,r0 */
L8c050dcc: if(!(*sr&1u))goto L8c050e14;goto L8c050dce; /* bf 0x8c050e14 */
L8c050dce: *stop=0x8c050dceu;if(!valid_access(r[9],1u))return -3;r[0]=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;goto L8c050dd0; /* mov.b @r9,r0 */
L8c050dd0: *stop=0x8c050dd0u;r[0]=r[0]&255u;goto L8c050dd2; /* extu.b r0,r0 */
L8c050dd2: *stop=0x8c050dd2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050dd4; /* cmp/eq #0x1,r0 */
L8c050dd4: if(!(*sr&1u))goto L8c050e14;goto L8c050dd6; /* bf 0x8c050e14 */
L8c050dd6: *stop=0x8c050dd6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050dd8; /* mov.l @r15,r3 */
L8c050dd8: *stop=0x8c050dd8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050dda; /* mov.l @(0x4,r15),r2 */
L8c050dda: *stop=0x8c050ddau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c050ddc; /* mov.l @r3,r1 */
L8c050ddc: *stop=0x8c050ddcu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c050dde; /* mov.l @r1,r0 */
L8c050dde: *stop=0x8c050ddeu;if(!valid_access(r[1]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050de0; /* mov.l @(0x4,r1),r3 */
L8c050de0: *stop=0x8c050de0u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c050de2; /* mov.l r0,@r2 */
L8c050de2: *stop=0x8c050de2u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c050de4; /* mov.l r3,@(0x4,r2) */
L8c050de4: *stop=0x8c050de4u;if(!valid_access(r[1]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050de6; /* mov.l @(0x8,r1),r0 */
L8c050de6: *stop=0x8c050de6u;if(!valid_access(r[1]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050de8; /* mov.l @(0xc,r1),r3 */
L8c050de8: *stop=0x8c050de8u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[0],4);if(io_status)return io_status;goto L8c050dea; /* mov.l r0,@(0x8,r2) */
L8c050dea: *stop=0x8c050deau;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[3],4);if(io_status)return io_status;goto L8c050dec; /* mov.l r3,@(0xc,r2) */
L8c050dec: *stop=0x8c050decu;if(!valid_access(r[1]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050dee; /* mov.l @(0x10,r1),r0 */
L8c050dee: *stop=0x8c050deeu;if(!valid_access(r[1]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050df0; /* mov.l @(0x14,r1),r3 */
L8c050df0: *stop=0x8c050df0u;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[0],4);if(io_status)return io_status;goto L8c050df2; /* mov.l r0,@(0x10,r2) */
L8c050df2: *stop=0x8c050df2u;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[3],4);if(io_status)return io_status;goto L8c050df4; /* mov.l r3,@(0x14,r2) */
L8c050df4: *stop=0x8c050df4u;if(!valid_access(r[1]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050df6; /* mov.l @(0x18,r1),r0 */
L8c050df6: *stop=0x8c050df6u;if(!valid_access(r[1]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050df8; /* mov.l @(0x1c,r1),r3 */
L8c050df8: *stop=0x8c050df8u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[0],4);if(io_status)return io_status;goto L8c050dfa; /* mov.l r0,@(0x18,r2) */
L8c050dfa: *stop=0x8c050dfau;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c050dfc; /* mov.l r3,@(0x1c,r2) */
L8c050dfc: *stop=0x8c050dfcu;if(!valid_access(r[1]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050dfe; /* mov.l @(0x20,r1),r0 */
L8c050dfe: *stop=0x8c050dfeu;if(!valid_access(r[1]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e00; /* mov.l @(0x24,r1),r3 */
L8c050e00: *stop=0x8c050e00u;if(!valid_access(r[2]+32u,4u))return -3;wr(m,bus,&io_status,r[2]+0x20u,r[0],4);if(io_status)return io_status;goto L8c050e02; /* mov.l r0,@(0x20,r2) */
L8c050e02: *stop=0x8c050e02u;if(!valid_access(r[2]+36u,4u))return -3;wr(m,bus,&io_status,r[2]+0x24u,r[3],4);if(io_status)return io_status;goto L8c050e04; /* mov.l r3,@(0x24,r2) */
L8c050e04: *stop=0x8c050e04u;if(!valid_access(r[1]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050e06; /* mov.l @(0x28,r1),r0 */
L8c050e06: *stop=0x8c050e06u;if(!valid_access(r[1]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e08; /* mov.l @(0x2c,r1),r3 */
L8c050e08: *stop=0x8c050e08u;if(!valid_access(r[2]+40u,4u))return -3;wr(m,bus,&io_status,r[2]+0x28u,r[0],4);if(io_status)return io_status;goto L8c050e0a; /* mov.l r0,@(0x28,r2) */
L8c050e0a: *stop=0x8c050e0au;if(!valid_access(r[2]+44u,4u))return -3;wr(m,bus,&io_status,r[2]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c050e0c; /* mov.l r3,@(0x2c,r2) */
L8c050e0c: *stop=0x8c050e0cu;if(!valid_access(r[1]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050e0e; /* mov.l @(0x30,r1),r0 */
L8c050e0e: *stop=0x8c050e0eu;if(!valid_access(r[2]+48u,4u))return -3;wr(m,bus,&io_status,r[2]+0x30u,r[0],4);if(io_status)return io_status;goto L8c050e10; /* mov.l r0,@(0x30,r2) */
L8c050e10: *stop=0x8c050e12u;goto L8c050e3e; /* bra 0x8c050e3e */
L8c050e14: *stop=0x8c050e14u;r[3]=52u;goto L8c050e16; /* mov #0x34,r3 */
L8c050e16: *stop=0x8c050e16u;r[0]=r[14];goto L8c050e18; /* mov r14,r0 */
L8c050e18: *stop=0x8c050e18u;r[7]=r[4]&255u;goto L8c050e1a; /* extu.b r4,r7 */
L8c050e1a: *stop=0x8c050e1au;*macl=(r[3]&65535u)*(r[7]&65535u);goto L8c050e1c; /* mulu.w r3,r7 */
L8c050e1c: *stop=0x8c050e1cu;r[7]=*macl;goto L8c050e1e; /* sts MACL,r7 */
L8c050e1e: *stop=0x8c050e1eu;r[7]+=r[12];goto L8c050e20; /* add r12,r7 */
L8c050e20: *stop=0x8c050e20u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050e22; /* mov.l @(0x8,r7),r2 */
L8c050e22: *stop=0x8c050e22u;if(!valid_access(r[7]+44u,4u))return -3;wr(m,bus,&io_status,r[7]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c050e24; /* mov.l r2,@(0x2c,r7) */
L8c050e24: *stop=0x8c050e24u;if(!valid_access(r[7]+8u,4u))return -3;wr(m,bus,&io_status,r[7]+0x8u,r[14],4);if(io_status)return io_status;goto L8c050e26; /* mov.l r14,@(0x8,r7) */
L8c050e26: *stop=0x8c050e26u;if(!valid_access(r[7]+12u,4u))return -3;wr(m,bus,&io_status,r[7]+0xcu,r[13],4);if(io_status)return io_status;goto L8c050e28; /* mov.l r13,@(0xc,r7) */
L8c050e28: *stop=0x8c050e28u;if(!valid_access(r[7]+16u,4u))return -3;wr(m,bus,&io_status,r[7]+0x10u,r[14],4);if(io_status)return io_status;goto L8c050e2a; /* mov.l r14,@(0x10,r7) */
L8c050e2a: *stop=0x8c050e2au;if(!valid_access(r[7]+20u,4u))return -3;wr(m,bus,&io_status,r[7]+0x14u,r[14],4);if(io_status)return io_status;goto L8c050e2c; /* mov.l r14,@(0x14,r7) */
L8c050e2c: *stop=0x8c050e2cu;if(!valid_access(r[7]+28u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c050e2e; /* mov.w r0,@(0x1c,r7) */
L8c050e2e: *stop=0x8c050e2eu;if(!valid_access(r[7]+30u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1eu,r[0],2);if(io_status)return io_status;goto L8c050e30; /* mov.w r0,@(0x1e,r7) */
L8c050e30: *stop=0x8c050e30u;r[0]=r[5];goto L8c050e32; /* mov r5,r0 */
L8c050e32: *stop=0x8c050e32u;if(!valid_access(r[7]+26u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1au,r[0],2);if(io_status)return io_status;goto L8c050e34; /* mov.w r0,@(0x1a,r7) */
L8c050e34: *stop=0x8c050e34u;if(!valid_access(r[7]+24u,2u))return -3;wr(m,bus,&io_status,r[7]+0x18u,r[0],2);if(io_status)return io_status;goto L8c050e36; /* mov.w r0,@(0x18,r7) */
L8c050e36: *stop=0x8c050e36u;r[0]=32u;goto L8c050e38; /* mov #0x20,r0 */
L8c050e38: *stop=0x8c050e38u;if(!valid_access(r[0]+r[7],2u))return -3;wr(m,bus,&io_status,r[0]+r[7],r[6],2);if(io_status)return io_status;goto L8c050e3a; /* mov.w r6,@(r0,r7) */
L8c050e3a: *stop=0x8c050e3au;r[0]=34u;goto L8c050e3c; /* mov #0x22,r0 */
L8c050e3c: *stop=0x8c050e3cu;if(!valid_access(r[0]+r[7],2u))return -3;wr(m,bus,&io_status,r[0]+r[7],r[6],2);if(io_status)return io_status;goto L8c050e3e; /* mov.w r6,@(r0,r7) */
L8c050e3e: *stop=0x8c050e3eu;r[4]+=1u;goto L8c050e40; /* add #0x1,r4 */
L8c050e40: *stop=0x8c050e40u;r[3]=r[4]&255u;goto L8c050e42; /* extu.b r4,r3 */
L8c050e42: *stop=0x8c050e42u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c050e44; /* tst r3,r3 */
L8c050e44: if(!(*sr&1u))goto L8c050e86;goto L8c050e46; /* bf 0x8c050e86 */
L8c050e46: *stop=0x8c050e46u;if(!valid_access(r[8],1u))return -3;r[0]=sx(m,bus,&io_status,r[8],1);if(io_status)return io_status;goto L8c050e48; /* mov.b @r8,r0 */
L8c050e48: *stop=0x8c050e48u;r[0]=r[0]&255u;goto L8c050e4a; /* extu.b r0,r0 */
L8c050e4a: *stop=0x8c050e4au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050e4c; /* cmp/eq #0x1,r0 */
L8c050e4c: if(!(*sr&1u))goto L8c050e86;goto L8c050e4e; /* bf 0x8c050e86 */
L8c050e4e: *stop=0x8c050e4eu;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c050e50; /* mov.l @r11,r2 */
L8c050e50: *stop=0x8c050e50u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e52; /* mov.l @r2,r3 */
L8c050e52: *stop=0x8c050e52u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050e54; /* mov.l @(0x4,r2),r1 */
L8c050e54: *stop=0x8c050e54u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c050e56; /* mov.l r3,@r10 */
L8c050e56: *stop=0x8c050e56u;if(!valid_access(r[10]+4u,4u))return -3;wr(m,bus,&io_status,r[10]+0x4u,r[1],4);if(io_status)return io_status;goto L8c050e58; /* mov.l r1,@(0x4,r10) */
L8c050e58: *stop=0x8c050e58u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e5a; /* mov.l @(0x8,r2),r3 */
L8c050e5a: *stop=0x8c050e5au;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050e5c; /* mov.l @(0xc,r2),r1 */
L8c050e5c: *stop=0x8c050e5cu;if(!valid_access(r[10]+8u,4u))return -3;wr(m,bus,&io_status,r[10]+0x8u,r[3],4);if(io_status)return io_status;goto L8c050e5e; /* mov.l r3,@(0x8,r10) */
L8c050e5e: *stop=0x8c050e5eu;if(!valid_access(r[10]+12u,4u))return -3;wr(m,bus,&io_status,r[10]+0xcu,r[1],4);if(io_status)return io_status;goto L8c050e60; /* mov.l r1,@(0xc,r10) */
L8c050e60: *stop=0x8c050e60u;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e62; /* mov.l @(0x10,r2),r3 */
L8c050e62: *stop=0x8c050e62u;if(!valid_access(r[2]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050e64; /* mov.l @(0x14,r2),r1 */
L8c050e64: *stop=0x8c050e64u;if(!valid_access(r[10]+16u,4u))return -3;wr(m,bus,&io_status,r[10]+0x10u,r[3],4);if(io_status)return io_status;goto L8c050e66; /* mov.l r3,@(0x10,r10) */
L8c050e66: *stop=0x8c050e66u;if(!valid_access(r[10]+20u,4u))return -3;wr(m,bus,&io_status,r[10]+0x14u,r[1],4);if(io_status)return io_status;goto L8c050e68; /* mov.l r1,@(0x14,r10) */
L8c050e68: *stop=0x8c050e68u;if(!valid_access(r[2]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e6a; /* mov.l @(0x18,r2),r3 */
L8c050e6a: *stop=0x8c050e6au;if(!valid_access(r[2]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050e6c; /* mov.l @(0x1c,r2),r1 */
L8c050e6c: *stop=0x8c050e6cu;if(!valid_access(r[10]+24u,4u))return -3;wr(m,bus,&io_status,r[10]+0x18u,r[3],4);if(io_status)return io_status;goto L8c050e6e; /* mov.l r3,@(0x18,r10) */
L8c050e6e: *stop=0x8c050e6eu;if(!valid_access(r[10]+28u,4u))return -3;wr(m,bus,&io_status,r[10]+0x1cu,r[1],4);if(io_status)return io_status;goto L8c050e70; /* mov.l r1,@(0x1c,r10) */
L8c050e70: *stop=0x8c050e70u;if(!valid_access(r[2]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e72; /* mov.l @(0x20,r2),r3 */
L8c050e72: *stop=0x8c050e72u;if(!valid_access(r[2]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050e74; /* mov.l @(0x24,r2),r1 */
L8c050e74: *stop=0x8c050e74u;if(!valid_access(r[10]+32u,4u))return -3;wr(m,bus,&io_status,r[10]+0x20u,r[3],4);if(io_status)return io_status;goto L8c050e76; /* mov.l r3,@(0x20,r10) */
L8c050e76: *stop=0x8c050e76u;if(!valid_access(r[10]+36u,4u))return -3;wr(m,bus,&io_status,r[10]+0x24u,r[1],4);if(io_status)return io_status;goto L8c050e78; /* mov.l r1,@(0x24,r10) */
L8c050e78: *stop=0x8c050e78u;if(!valid_access(r[2]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e7a; /* mov.l @(0x28,r2),r3 */
L8c050e7a: *stop=0x8c050e7au;if(!valid_access(r[2]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x2cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050e7c; /* mov.l @(0x2c,r2),r1 */
L8c050e7c: *stop=0x8c050e7cu;if(!valid_access(r[10]+40u,4u))return -3;wr(m,bus,&io_status,r[10]+0x28u,r[3],4);if(io_status)return io_status;goto L8c050e7e; /* mov.l r3,@(0x28,r10) */
L8c050e7e: *stop=0x8c050e7eu;if(!valid_access(r[10]+44u,4u))return -3;wr(m,bus,&io_status,r[10]+0x2cu,r[1],4);if(io_status)return io_status;goto L8c050e80; /* mov.l r1,@(0x2c,r10) */
L8c050e80: *stop=0x8c050e80u;if(!valid_access(r[2]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e82; /* mov.l @(0x30,r2),r3 */
L8c050e82: *stop=0x8c050e84u;if(!valid_access(r[10]+48u,4u))return -3;wr(m,bus,&io_status,r[10]+0x30u,r[3],4);if(io_status)return io_status;goto L8c050f2a; /* bra 0x8c050f2a */
L8c050e86: *stop=0x8c050e86u;r[0]=r[4]&255u;goto L8c050e88; /* extu.b r4,r0 */
L8c050e88: *stop=0x8c050e88u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050e8a; /* cmp/eq #0x1,r0 */
L8c050e8a: if(!(*sr&1u))goto L8c050f00;goto L8c050e8c; /* bf 0x8c050f00 */
L8c050e8c: *stop=0x8c050e8cu;if(!valid_access(r[9],1u))return -3;r[0]=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;goto L8c050e8e; /* mov.b @r9,r0 */
L8c050e8e: *stop=0x8c050e8eu;r[0]=r[0]&255u;goto L8c050e90; /* extu.b r0,r0 */
L8c050e90: *stop=0x8c050e90u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050e92; /* cmp/eq #0x1,r0 */
L8c050e92: if(!(*sr&1u))goto L8c050f00;goto L8c050e94; /* bf 0x8c050f00 */
L8c050e94: *stop=0x8c050e94u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e96; /* mov.l @r15,r3 */
L8c050e96: *stop=0x8c050e96u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050e98; /* mov.l @(0x4,r15),r2 */
L8c050e98: *stop=0x8c050e98u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c050e9a; /* mov.l @r3,r1 */
L8c050e9a: *stop=0x8c050e9au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c050e9c; /* mov.l @r1,r0 */
L8c050e9c: *stop=0x8c050e9cu;if(!valid_access(r[1]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050e9e; /* mov.l @(0x4,r1),r3 */
L8c050e9e: *stop=0x8c050e9eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c050ea0; /* mov.l r0,@r2 */
L8c050ea0: *stop=0x8c050ea0u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c050ea2; /* mov.l r3,@(0x4,r2) */
L8c050ea2: *stop=0x8c050ea2u;if(!valid_access(r[1]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050ea4; /* mov.l @(0x8,r1),r0 */
L8c050ea4: *stop=0x8c050ea4u;if(!valid_access(r[1]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050ea6; /* mov.l @(0xc,r1),r3 */
L8c050ea6: *stop=0x8c050ea6u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[0],4);if(io_status)return io_status;goto L8c050ea8; /* mov.l r0,@(0x8,r2) */
L8c050ea8: *stop=0x8c050ea8u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[3],4);if(io_status)return io_status;goto L8c050eaa; /* mov.l r3,@(0xc,r2) */
L8c050eaa: *stop=0x8c050eaau;if(!valid_access(r[1]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050eac; /* mov.l @(0x10,r1),r0 */
L8c050eac: *stop=0x8c050eacu;if(!valid_access(r[1]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050eae; /* mov.l @(0x14,r1),r3 */
L8c050eae: *stop=0x8c050eaeu;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[0],4);if(io_status)return io_status;goto L8c050eb0; /* mov.l r0,@(0x10,r2) */
L8c050eb0: *stop=0x8c050eb0u;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[3],4);if(io_status)return io_status;goto L8c050eb2; /* mov.l r3,@(0x14,r2) */
L8c050eb2: *stop=0x8c050eb2u;if(!valid_access(r[1]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050eb4; /* mov.l @(0x18,r1),r0 */
L8c050eb4: *stop=0x8c050eb4u;if(!valid_access(r[1]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050eb6; /* mov.l @(0x1c,r1),r3 */
L8c050eb6: *stop=0x8c050eb6u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[0],4);if(io_status)return io_status;goto L8c050eb8; /* mov.l r0,@(0x18,r2) */
L8c050eb8: *stop=0x8c050eb8u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c050eba; /* mov.l r3,@(0x1c,r2) */
L8c050eba: *stop=0x8c050ebau;if(!valid_access(r[1]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050ebc; /* mov.l @(0x20,r1),r0 */
L8c050ebc: *stop=0x8c050ebcu;if(!valid_access(r[1]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050ebe; /* mov.l @(0x24,r1),r3 */
L8c050ebe: *stop=0x8c050ebeu;if(!valid_access(r[2]+32u,4u))return -3;wr(m,bus,&io_status,r[2]+0x20u,r[0],4);if(io_status)return io_status;goto L8c050ec0; /* mov.l r0,@(0x20,r2) */
L8c050ec0: *stop=0x8c050ec0u;if(!valid_access(r[2]+36u,4u))return -3;wr(m,bus,&io_status,r[2]+0x24u,r[3],4);if(io_status)return io_status;goto L8c050ec2; /* mov.l r3,@(0x24,r2) */
L8c050ec2: *stop=0x8c050ec2u;if(!valid_access(r[1]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050ec4; /* mov.l @(0x28,r1),r0 */
L8c050ec4: *stop=0x8c050ec4u;if(!valid_access(r[1]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050ec6; /* mov.l @(0x2c,r1),r3 */
L8c050ec6: *stop=0x8c050ec6u;if(!valid_access(r[2]+40u,4u))return -3;wr(m,bus,&io_status,r[2]+0x28u,r[0],4);if(io_status)return io_status;goto L8c050ec8; /* mov.l r0,@(0x28,r2) */
L8c050ec8: *stop=0x8c050ec8u;if(!valid_access(r[2]+44u,4u))return -3;wr(m,bus,&io_status,r[2]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c050eca; /* mov.l r3,@(0x2c,r2) */
L8c050eca: *stop=0x8c050ecau;if(!valid_access(r[1]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050ecc; /* mov.l @(0x30,r1),r0 */
L8c050ecc: *stop=0x8c050eccu;if(!valid_access(r[2]+48u,4u))return -3;wr(m,bus,&io_status,r[2]+0x30u,r[0],4);if(io_status)return io_status;goto L8c050ece; /* mov.l r0,@(0x30,r2) */
L8c050ece: *stop=0x8c050ed0u;goto L8c050f2a; /* bra 0x8c050f2a */
L8c050f00: *stop=0x8c050f00u;r[3]=52u;goto L8c050f02; /* mov #0x34,r3 */
L8c050f02: *stop=0x8c050f02u;r[0]=r[14];goto L8c050f04; /* mov r14,r0 */
L8c050f04: *stop=0x8c050f04u;r[7]=r[4]&255u;goto L8c050f06; /* extu.b r4,r7 */
L8c050f06: *stop=0x8c050f06u;*macl=(r[3]&65535u)*(r[7]&65535u);goto L8c050f08; /* mulu.w r3,r7 */
L8c050f08: *stop=0x8c050f08u;r[7]=*macl;goto L8c050f0a; /* sts MACL,r7 */
L8c050f0a: *stop=0x8c050f0au;r[7]+=r[12];goto L8c050f0c; /* add r12,r7 */
L8c050f0c: *stop=0x8c050f0cu;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050f0e; /* mov.l @(0x8,r7),r2 */
L8c050f0e: *stop=0x8c050f0eu;if(!valid_access(r[7]+44u,4u))return -3;wr(m,bus,&io_status,r[7]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c050f10; /* mov.l r2,@(0x2c,r7) */
L8c050f10: *stop=0x8c050f10u;if(!valid_access(r[7]+8u,4u))return -3;wr(m,bus,&io_status,r[7]+0x8u,r[14],4);if(io_status)return io_status;goto L8c050f12; /* mov.l r14,@(0x8,r7) */
L8c050f12: *stop=0x8c050f12u;if(!valid_access(r[7]+12u,4u))return -3;wr(m,bus,&io_status,r[7]+0xcu,r[13],4);if(io_status)return io_status;goto L8c050f14; /* mov.l r13,@(0xc,r7) */
L8c050f14: *stop=0x8c050f14u;if(!valid_access(r[7]+16u,4u))return -3;wr(m,bus,&io_status,r[7]+0x10u,r[14],4);if(io_status)return io_status;goto L8c050f16; /* mov.l r14,@(0x10,r7) */
L8c050f16: *stop=0x8c050f16u;if(!valid_access(r[7]+20u,4u))return -3;wr(m,bus,&io_status,r[7]+0x14u,r[14],4);if(io_status)return io_status;goto L8c050f18; /* mov.l r14,@(0x14,r7) */
L8c050f18: *stop=0x8c050f18u;if(!valid_access(r[7]+28u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c050f1a; /* mov.w r0,@(0x1c,r7) */
L8c050f1a: *stop=0x8c050f1au;if(!valid_access(r[7]+30u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1eu,r[0],2);if(io_status)return io_status;goto L8c050f1c; /* mov.w r0,@(0x1e,r7) */
L8c050f1c: *stop=0x8c050f1cu;r[0]=r[5];goto L8c050f1e; /* mov r5,r0 */
L8c050f1e: *stop=0x8c050f1eu;if(!valid_access(r[7]+26u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1au,r[0],2);if(io_status)return io_status;goto L8c050f20; /* mov.w r0,@(0x1a,r7) */
L8c050f20: *stop=0x8c050f20u;if(!valid_access(r[7]+24u,2u))return -3;wr(m,bus,&io_status,r[7]+0x18u,r[0],2);if(io_status)return io_status;goto L8c050f22; /* mov.w r0,@(0x18,r7) */
L8c050f22: *stop=0x8c050f22u;r[0]=32u;goto L8c050f24; /* mov #0x20,r0 */
L8c050f24: *stop=0x8c050f24u;if(!valid_access(r[0]+r[7],2u))return -3;wr(m,bus,&io_status,r[0]+r[7],r[6],2);if(io_status)return io_status;goto L8c050f26; /* mov.w r6,@(r0,r7) */
L8c050f26: *stop=0x8c050f26u;r[0]=34u;goto L8c050f28; /* mov #0x22,r0 */
L8c050f28: *stop=0x8c050f28u;if(!valid_access(r[0]+r[7],2u))return -3;wr(m,bus,&io_status,r[0]+r[7],r[6],2);if(io_status)return io_status;goto L8c050f2a; /* mov.w r6,@(r0,r7) */
L8c050f2a: *stop=0x8c050f2au;r[4]+=1u;goto L8c050f2c; /* add #0x1,r4 */
L8c050f2c: *stop=0x8c050f2cu;r[3]=r[4]&255u;goto L8c050f2e; /* extu.b r4,r3 */
L8c050f2e: *stop=0x8c050f2eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c050f30; /* tst r3,r3 */
L8c050f30: if(!(*sr&1u))goto L8c050f72;goto L8c050f32; /* bf 0x8c050f72 */
L8c050f32: *stop=0x8c050f32u;if(!valid_access(r[8],1u))return -3;r[0]=sx(m,bus,&io_status,r[8],1);if(io_status)return io_status;goto L8c050f34; /* mov.b @r8,r0 */
L8c050f34: *stop=0x8c050f34u;r[0]=r[0]&255u;goto L8c050f36; /* extu.b r0,r0 */
L8c050f36: *stop=0x8c050f36u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050f38; /* cmp/eq #0x1,r0 */
L8c050f38: if(!(*sr&1u))goto L8c050f72;goto L8c050f3a; /* bf 0x8c050f72 */
L8c050f3a: *stop=0x8c050f3au;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c050f3c; /* mov.l @r11,r2 */
L8c050f3c: *stop=0x8c050f3cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f3e; /* mov.l @r2,r3 */
L8c050f3e: *stop=0x8c050f3eu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050f40; /* mov.l @(0x4,r2),r1 */
L8c050f40: *stop=0x8c050f40u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c050f42; /* mov.l r3,@r10 */
L8c050f42: *stop=0x8c050f42u;if(!valid_access(r[10]+4u,4u))return -3;wr(m,bus,&io_status,r[10]+0x4u,r[1],4);if(io_status)return io_status;goto L8c050f44; /* mov.l r1,@(0x4,r10) */
L8c050f44: *stop=0x8c050f44u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f46; /* mov.l @(0x8,r2),r3 */
L8c050f46: *stop=0x8c050f46u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050f48; /* mov.l @(0xc,r2),r1 */
L8c050f48: *stop=0x8c050f48u;if(!valid_access(r[10]+8u,4u))return -3;wr(m,bus,&io_status,r[10]+0x8u,r[3],4);if(io_status)return io_status;goto L8c050f4a; /* mov.l r3,@(0x8,r10) */
L8c050f4a: *stop=0x8c050f4au;if(!valid_access(r[10]+12u,4u))return -3;wr(m,bus,&io_status,r[10]+0xcu,r[1],4);if(io_status)return io_status;goto L8c050f4c; /* mov.l r1,@(0xc,r10) */
L8c050f4c: *stop=0x8c050f4cu;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f4e; /* mov.l @(0x10,r2),r3 */
L8c050f4e: *stop=0x8c050f4eu;if(!valid_access(r[2]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050f50; /* mov.l @(0x14,r2),r1 */
L8c050f50: *stop=0x8c050f50u;if(!valid_access(r[10]+16u,4u))return -3;wr(m,bus,&io_status,r[10]+0x10u,r[3],4);if(io_status)return io_status;goto L8c050f52; /* mov.l r3,@(0x10,r10) */
L8c050f52: *stop=0x8c050f52u;if(!valid_access(r[10]+20u,4u))return -3;wr(m,bus,&io_status,r[10]+0x14u,r[1],4);if(io_status)return io_status;goto L8c050f54; /* mov.l r1,@(0x14,r10) */
L8c050f54: *stop=0x8c050f54u;if(!valid_access(r[2]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f56; /* mov.l @(0x18,r2),r3 */
L8c050f56: *stop=0x8c050f56u;if(!valid_access(r[2]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050f58; /* mov.l @(0x1c,r2),r1 */
L8c050f58: *stop=0x8c050f58u;if(!valid_access(r[10]+24u,4u))return -3;wr(m,bus,&io_status,r[10]+0x18u,r[3],4);if(io_status)return io_status;goto L8c050f5a; /* mov.l r3,@(0x18,r10) */
L8c050f5a: *stop=0x8c050f5au;if(!valid_access(r[10]+28u,4u))return -3;wr(m,bus,&io_status,r[10]+0x1cu,r[1],4);if(io_status)return io_status;goto L8c050f5c; /* mov.l r1,@(0x1c,r10) */
L8c050f5c: *stop=0x8c050f5cu;if(!valid_access(r[2]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f5e; /* mov.l @(0x20,r2),r3 */
L8c050f5e: *stop=0x8c050f5eu;if(!valid_access(r[2]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050f60; /* mov.l @(0x24,r2),r1 */
L8c050f60: *stop=0x8c050f60u;if(!valid_access(r[10]+32u,4u))return -3;wr(m,bus,&io_status,r[10]+0x20u,r[3],4);if(io_status)return io_status;goto L8c050f62; /* mov.l r3,@(0x20,r10) */
L8c050f62: *stop=0x8c050f62u;if(!valid_access(r[10]+36u,4u))return -3;wr(m,bus,&io_status,r[10]+0x24u,r[1],4);if(io_status)return io_status;goto L8c050f64; /* mov.l r1,@(0x24,r10) */
L8c050f64: *stop=0x8c050f64u;if(!valid_access(r[2]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f66; /* mov.l @(0x28,r2),r3 */
L8c050f66: *stop=0x8c050f66u;if(!valid_access(r[2]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x2cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050f68; /* mov.l @(0x2c,r2),r1 */
L8c050f68: *stop=0x8c050f68u;if(!valid_access(r[10]+40u,4u))return -3;wr(m,bus,&io_status,r[10]+0x28u,r[3],4);if(io_status)return io_status;goto L8c050f6a; /* mov.l r3,@(0x28,r10) */
L8c050f6a: *stop=0x8c050f6au;if(!valid_access(r[10]+44u,4u))return -3;wr(m,bus,&io_status,r[10]+0x2cu,r[1],4);if(io_status)return io_status;goto L8c050f6c; /* mov.l r1,@(0x2c,r10) */
L8c050f6c: *stop=0x8c050f6cu;if(!valid_access(r[2]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f6e; /* mov.l @(0x30,r2),r3 */
L8c050f6e: *stop=0x8c050f70u;if(!valid_access(r[10]+48u,4u))return -3;wr(m,bus,&io_status,r[10]+0x30u,r[3],4);if(io_status)return io_status;goto L8c050fe8; /* bra 0x8c050fe8 */
L8c050f72: *stop=0x8c050f72u;r[0]=r[4]&255u;goto L8c050f74; /* extu.b r4,r0 */
L8c050f74: *stop=0x8c050f74u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050f76; /* cmp/eq #0x1,r0 */
L8c050f76: if(!(*sr&1u))goto L8c050fbe;goto L8c050f78; /* bf 0x8c050fbe */
L8c050f78: *stop=0x8c050f78u;if(!valid_access(r[9],1u))return -3;r[0]=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;goto L8c050f7a; /* mov.b @r9,r0 */
L8c050f7a: *stop=0x8c050f7au;r[0]=r[0]&255u;goto L8c050f7c; /* extu.b r0,r0 */
L8c050f7c: *stop=0x8c050f7cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050f7e; /* cmp/eq #0x1,r0 */
L8c050f7e: if(!(*sr&1u))goto L8c050fbe;goto L8c050f80; /* bf 0x8c050fbe */
L8c050f80: *stop=0x8c050f80u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f82; /* mov.l @r15,r3 */
L8c050f82: *stop=0x8c050f82u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050f84; /* mov.l @(0x4,r15),r2 */
L8c050f84: *stop=0x8c050f84u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c050f86; /* mov.l @r3,r1 */
L8c050f86: *stop=0x8c050f86u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c050f88; /* mov.l @r1,r0 */
L8c050f88: *stop=0x8c050f88u;if(!valid_access(r[1]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f8a; /* mov.l @(0x4,r1),r3 */
L8c050f8a: *stop=0x8c050f8au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c050f8c; /* mov.l r0,@r2 */
L8c050f8c: *stop=0x8c050f8cu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c050f8e; /* mov.l r3,@(0x4,r2) */
L8c050f8e: *stop=0x8c050f8eu;if(!valid_access(r[1]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050f90; /* mov.l @(0x8,r1),r0 */
L8c050f90: *stop=0x8c050f90u;if(!valid_access(r[1]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f92; /* mov.l @(0xc,r1),r3 */
L8c050f92: *stop=0x8c050f92u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[0],4);if(io_status)return io_status;goto L8c050f94; /* mov.l r0,@(0x8,r2) */
L8c050f94: *stop=0x8c050f94u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[3],4);if(io_status)return io_status;goto L8c050f96; /* mov.l r3,@(0xc,r2) */
L8c050f96: *stop=0x8c050f96u;if(!valid_access(r[1]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050f98; /* mov.l @(0x10,r1),r0 */
L8c050f98: *stop=0x8c050f98u;if(!valid_access(r[1]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050f9a; /* mov.l @(0x14,r1),r3 */
L8c050f9a: *stop=0x8c050f9au;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[0],4);if(io_status)return io_status;goto L8c050f9c; /* mov.l r0,@(0x10,r2) */
L8c050f9c: *stop=0x8c050f9cu;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[3],4);if(io_status)return io_status;goto L8c050f9e; /* mov.l r3,@(0x14,r2) */
L8c050f9e: *stop=0x8c050f9eu;if(!valid_access(r[1]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050fa0; /* mov.l @(0x18,r1),r0 */
L8c050fa0: *stop=0x8c050fa0u;if(!valid_access(r[1]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050fa2; /* mov.l @(0x1c,r1),r3 */
L8c050fa2: *stop=0x8c050fa2u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[0],4);if(io_status)return io_status;goto L8c050fa4; /* mov.l r0,@(0x18,r2) */
L8c050fa4: *stop=0x8c050fa4u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c050fa6; /* mov.l r3,@(0x1c,r2) */
L8c050fa6: *stop=0x8c050fa6u;if(!valid_access(r[1]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050fa8; /* mov.l @(0x20,r1),r0 */
L8c050fa8: *stop=0x8c050fa8u;if(!valid_access(r[1]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050faa; /* mov.l @(0x24,r1),r3 */
L8c050faa: *stop=0x8c050faau;if(!valid_access(r[2]+32u,4u))return -3;wr(m,bus,&io_status,r[2]+0x20u,r[0],4);if(io_status)return io_status;goto L8c050fac; /* mov.l r0,@(0x20,r2) */
L8c050fac: *stop=0x8c050facu;if(!valid_access(r[2]+36u,4u))return -3;wr(m,bus,&io_status,r[2]+0x24u,r[3],4);if(io_status)return io_status;goto L8c050fae; /* mov.l r3,@(0x24,r2) */
L8c050fae: *stop=0x8c050faeu;if(!valid_access(r[1]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050fb0; /* mov.l @(0x28,r1),r0 */
L8c050fb0: *stop=0x8c050fb0u;if(!valid_access(r[1]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c050fb2; /* mov.l @(0x2c,r1),r3 */
L8c050fb2: *stop=0x8c050fb2u;if(!valid_access(r[2]+40u,4u))return -3;wr(m,bus,&io_status,r[2]+0x28u,r[0],4);if(io_status)return io_status;goto L8c050fb4; /* mov.l r0,@(0x28,r2) */
L8c050fb4: *stop=0x8c050fb4u;if(!valid_access(r[2]+44u,4u))return -3;wr(m,bus,&io_status,r[2]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c050fb6; /* mov.l r3,@(0x2c,r2) */
L8c050fb6: *stop=0x8c050fb6u;if(!valid_access(r[1]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c050fb8; /* mov.l @(0x30,r1),r0 */
L8c050fb8: *stop=0x8c050fb8u;if(!valid_access(r[2]+48u,4u))return -3;wr(m,bus,&io_status,r[2]+0x30u,r[0],4);if(io_status)return io_status;goto L8c050fba; /* mov.l r0,@(0x30,r2) */
L8c050fba: *stop=0x8c050fbcu;goto L8c050fe8; /* bra 0x8c050fe8 */
L8c050fbe: *stop=0x8c050fbeu;r[3]=52u;goto L8c050fc0; /* mov #0x34,r3 */
L8c050fc0: *stop=0x8c050fc0u;r[0]=r[14];goto L8c050fc2; /* mov r14,r0 */
L8c050fc2: *stop=0x8c050fc2u;r[7]=r[4]&255u;goto L8c050fc4; /* extu.b r4,r7 */
L8c050fc4: *stop=0x8c050fc4u;*macl=(r[3]&65535u)*(r[7]&65535u);goto L8c050fc6; /* mulu.w r3,r7 */
L8c050fc6: *stop=0x8c050fc6u;r[7]=*macl;goto L8c050fc8; /* sts MACL,r7 */
L8c050fc8: *stop=0x8c050fc8u;r[7]+=r[12];goto L8c050fca; /* add r12,r7 */
L8c050fca: *stop=0x8c050fcau;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c050fcc; /* mov.l @(0x8,r7),r2 */
L8c050fcc: *stop=0x8c050fccu;if(!valid_access(r[7]+44u,4u))return -3;wr(m,bus,&io_status,r[7]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c050fce; /* mov.l r2,@(0x2c,r7) */
L8c050fce: *stop=0x8c050fceu;if(!valid_access(r[7]+8u,4u))return -3;wr(m,bus,&io_status,r[7]+0x8u,r[14],4);if(io_status)return io_status;goto L8c050fd0; /* mov.l r14,@(0x8,r7) */
L8c050fd0: *stop=0x8c050fd0u;if(!valid_access(r[7]+12u,4u))return -3;wr(m,bus,&io_status,r[7]+0xcu,r[13],4);if(io_status)return io_status;goto L8c050fd2; /* mov.l r13,@(0xc,r7) */
L8c050fd2: *stop=0x8c050fd2u;if(!valid_access(r[7]+16u,4u))return -3;wr(m,bus,&io_status,r[7]+0x10u,r[14],4);if(io_status)return io_status;goto L8c050fd4; /* mov.l r14,@(0x10,r7) */
L8c050fd4: *stop=0x8c050fd4u;if(!valid_access(r[7]+20u,4u))return -3;wr(m,bus,&io_status,r[7]+0x14u,r[14],4);if(io_status)return io_status;goto L8c050fd6; /* mov.l r14,@(0x14,r7) */
L8c050fd6: *stop=0x8c050fd6u;if(!valid_access(r[7]+28u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c050fd8; /* mov.w r0,@(0x1c,r7) */
L8c050fd8: *stop=0x8c050fd8u;if(!valid_access(r[7]+30u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1eu,r[0],2);if(io_status)return io_status;goto L8c050fda; /* mov.w r0,@(0x1e,r7) */
L8c050fda: *stop=0x8c050fdau;r[0]=r[5];goto L8c050fdc; /* mov r5,r0 */
L8c050fdc: *stop=0x8c050fdcu;if(!valid_access(r[7]+26u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1au,r[0],2);if(io_status)return io_status;goto L8c050fde; /* mov.w r0,@(0x1a,r7) */
L8c050fde: *stop=0x8c050fdeu;if(!valid_access(r[7]+24u,2u))return -3;wr(m,bus,&io_status,r[7]+0x18u,r[0],2);if(io_status)return io_status;goto L8c050fe0; /* mov.w r0,@(0x18,r7) */
L8c050fe0: *stop=0x8c050fe0u;r[0]=32u;goto L8c050fe2; /* mov #0x20,r0 */
L8c050fe2: *stop=0x8c050fe2u;if(!valid_access(r[0]+r[7],2u))return -3;wr(m,bus,&io_status,r[0]+r[7],r[6],2);if(io_status)return io_status;goto L8c050fe4; /* mov.w r6,@(r0,r7) */
L8c050fe4: *stop=0x8c050fe4u;r[0]=34u;goto L8c050fe6; /* mov #0x22,r0 */
L8c050fe6: *stop=0x8c050fe6u;if(!valid_access(r[0]+r[7],2u))return -3;wr(m,bus,&io_status,r[0]+r[7],r[6],2);if(io_status)return io_status;goto L8c050fe8; /* mov.w r6,@(r0,r7) */
L8c050fe8: *stop=0x8c050fe8u;r[4]+=1u;goto L8c050fea; /* add #0x1,r4 */
L8c050fea: *stop=0x8c050feau;r[7]=r[4]&255u;goto L8c050fec; /* extu.b r4,r7 */
L8c050fec: *stop=0x8c050fecu;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c050fee; /* tst r7,r7 */
L8c050fee: if(!(*sr&1u))goto L8c051030;goto L8c050ff0; /* bf 0x8c051030 */
L8c050ff0: *stop=0x8c050ff0u;if(!valid_access(r[8],1u))return -3;r[0]=sx(m,bus,&io_status,r[8],1);if(io_status)return io_status;goto L8c050ff2; /* mov.b @r8,r0 */
L8c050ff2: *stop=0x8c050ff2u;r[0]=r[0]&255u;goto L8c050ff4; /* extu.b r0,r0 */
L8c050ff4: *stop=0x8c050ff4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c050ff6; /* cmp/eq #0x1,r0 */
L8c050ff6: if(!(*sr&1u))goto L8c051030;goto L8c050ff8; /* bf 0x8c051030 */
L8c050ff8: *stop=0x8c050ff8u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c050ffa; /* mov.l @r11,r2 */
L8c050ffa: *stop=0x8c050ffau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c050ffc; /* mov.l @r2,r3 */
L8c050ffc: *stop=0x8c050ffcu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c050ffe; /* mov.l @(0x4,r2),r1 */
L8c050ffe: *stop=0x8c050ffeu;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c051000; /* mov.l r3,@r10 */
L8c051000: *stop=0x8c051000u;if(!valid_access(r[10]+4u,4u))return -3;wr(m,bus,&io_status,r[10]+0x4u,r[1],4);if(io_status)return io_status;goto L8c051002; /* mov.l r1,@(0x4,r10) */
L8c051002: *stop=0x8c051002u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051004; /* mov.l @(0x8,r2),r3 */
L8c051004: *stop=0x8c051004u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c051006; /* mov.l @(0xc,r2),r1 */
L8c051006: *stop=0x8c051006u;if(!valid_access(r[10]+8u,4u))return -3;wr(m,bus,&io_status,r[10]+0x8u,r[3],4);if(io_status)return io_status;goto L8c051008; /* mov.l r3,@(0x8,r10) */
L8c051008: *stop=0x8c051008u;if(!valid_access(r[10]+12u,4u))return -3;wr(m,bus,&io_status,r[10]+0xcu,r[1],4);if(io_status)return io_status;goto L8c05100a; /* mov.l r1,@(0xc,r10) */
L8c05100a: *stop=0x8c05100au;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05100c; /* mov.l @(0x10,r2),r3 */
L8c05100c: *stop=0x8c05100cu;if(!valid_access(r[2]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c05100e; /* mov.l @(0x14,r2),r1 */
L8c05100e: *stop=0x8c05100eu;if(!valid_access(r[10]+16u,4u))return -3;wr(m,bus,&io_status,r[10]+0x10u,r[3],4);if(io_status)return io_status;goto L8c051010; /* mov.l r3,@(0x10,r10) */
L8c051010: *stop=0x8c051010u;if(!valid_access(r[10]+20u,4u))return -3;wr(m,bus,&io_status,r[10]+0x14u,r[1],4);if(io_status)return io_status;goto L8c051012; /* mov.l r1,@(0x14,r10) */
L8c051012: *stop=0x8c051012u;if(!valid_access(r[2]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051014; /* mov.l @(0x18,r2),r3 */
L8c051014: *stop=0x8c051014u;if(!valid_access(r[2]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c051016; /* mov.l @(0x1c,r2),r1 */
L8c051016: *stop=0x8c051016u;if(!valid_access(r[10]+24u,4u))return -3;wr(m,bus,&io_status,r[10]+0x18u,r[3],4);if(io_status)return io_status;goto L8c051018; /* mov.l r3,@(0x18,r10) */
L8c051018: *stop=0x8c051018u;if(!valid_access(r[10]+28u,4u))return -3;wr(m,bus,&io_status,r[10]+0x1cu,r[1],4);if(io_status)return io_status;goto L8c05101a; /* mov.l r1,@(0x1c,r10) */
L8c05101a: *stop=0x8c05101au;if(!valid_access(r[2]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05101c; /* mov.l @(0x20,r2),r3 */
L8c05101c: *stop=0x8c05101cu;if(!valid_access(r[2]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c05101e; /* mov.l @(0x24,r2),r1 */
L8c05101e: *stop=0x8c05101eu;if(!valid_access(r[10]+32u,4u))return -3;wr(m,bus,&io_status,r[10]+0x20u,r[3],4);if(io_status)return io_status;goto L8c051020; /* mov.l r3,@(0x20,r10) */
L8c051020: *stop=0x8c051020u;if(!valid_access(r[10]+36u,4u))return -3;wr(m,bus,&io_status,r[10]+0x24u,r[1],4);if(io_status)return io_status;goto L8c051022; /* mov.l r1,@(0x24,r10) */
L8c051022: *stop=0x8c051022u;if(!valid_access(r[2]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051024; /* mov.l @(0x28,r2),r3 */
L8c051024: *stop=0x8c051024u;if(!valid_access(r[2]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x2cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c051026; /* mov.l @(0x2c,r2),r1 */
L8c051026: *stop=0x8c051026u;if(!valid_access(r[10]+40u,4u))return -3;wr(m,bus,&io_status,r[10]+0x28u,r[3],4);if(io_status)return io_status;goto L8c051028; /* mov.l r3,@(0x28,r10) */
L8c051028: *stop=0x8c051028u;if(!valid_access(r[10]+44u,4u))return -3;wr(m,bus,&io_status,r[10]+0x2cu,r[1],4);if(io_status)return io_status;goto L8c05102a; /* mov.l r1,@(0x2c,r10) */
L8c05102a: *stop=0x8c05102au;if(!valid_access(r[2]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05102c; /* mov.l @(0x30,r2),r3 */
L8c05102c: *stop=0x8c05102eu;if(!valid_access(r[10]+48u,4u))return -3;wr(m,bus,&io_status,r[10]+0x30u,r[3],4);if(io_status)return io_status;goto L8c0510a6; /* bra 0x8c0510a6 */
L8c051030: *stop=0x8c051030u;r[0]=r[7];goto L8c051032; /* mov r7,r0 */
L8c051032: *stop=0x8c051032u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c051034; /* cmp/eq #0x1,r0 */
L8c051034: if(!(*sr&1u))goto L8c05107c;goto L8c051036; /* bf 0x8c05107c */
L8c051036: *stop=0x8c051036u;if(!valid_access(r[9],1u))return -3;r[0]=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;goto L8c051038; /* mov.b @r9,r0 */
L8c051038: *stop=0x8c051038u;r[0]=r[0]&255u;goto L8c05103a; /* extu.b r0,r0 */
L8c05103a: *stop=0x8c05103au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c05103c; /* cmp/eq #0x1,r0 */
L8c05103c: if(!(*sr&1u))goto L8c05107c;goto L8c05103e; /* bf 0x8c05107c */
L8c05103e: *stop=0x8c05103eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c051040; /* mov.l @r15,r3 */
L8c051040: *stop=0x8c051040u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c051042; /* mov.l @(0x4,r15),r2 */
L8c051042: *stop=0x8c051042u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c051044; /* mov.l @r3,r1 */
L8c051044: *stop=0x8c051044u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c051046; /* mov.l @r1,r0 */
L8c051046: *stop=0x8c051046u;if(!valid_access(r[1]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051048; /* mov.l @(0x4,r1),r3 */
L8c051048: *stop=0x8c051048u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c05104a; /* mov.l r0,@r2 */
L8c05104a: *stop=0x8c05104au;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c05104c; /* mov.l r3,@(0x4,r2) */
L8c05104c: *stop=0x8c05104cu;if(!valid_access(r[1]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c05104e; /* mov.l @(0x8,r1),r0 */
L8c05104e: *stop=0x8c05104eu;if(!valid_access(r[1]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051050; /* mov.l @(0xc,r1),r3 */
L8c051050: *stop=0x8c051050u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[0],4);if(io_status)return io_status;goto L8c051052; /* mov.l r0,@(0x8,r2) */
L8c051052: *stop=0x8c051052u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[3],4);if(io_status)return io_status;goto L8c051054; /* mov.l r3,@(0xc,r2) */
L8c051054: *stop=0x8c051054u;if(!valid_access(r[1]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c051056; /* mov.l @(0x10,r1),r0 */
L8c051056: *stop=0x8c051056u;if(!valid_access(r[1]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051058; /* mov.l @(0x14,r1),r3 */
L8c051058: *stop=0x8c051058u;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[0],4);if(io_status)return io_status;goto L8c05105a; /* mov.l r0,@(0x10,r2) */
L8c05105a: *stop=0x8c05105au;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[3],4);if(io_status)return io_status;goto L8c05105c; /* mov.l r3,@(0x14,r2) */
L8c05105c: *stop=0x8c05105cu;if(!valid_access(r[1]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c05105e; /* mov.l @(0x18,r1),r0 */
L8c05105e: *stop=0x8c05105eu;if(!valid_access(r[1]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051060; /* mov.l @(0x1c,r1),r3 */
L8c051060: *stop=0x8c051060u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[0],4);if(io_status)return io_status;goto L8c051062; /* mov.l r0,@(0x18,r2) */
L8c051062: *stop=0x8c051062u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c051064; /* mov.l r3,@(0x1c,r2) */
L8c051064: *stop=0x8c051064u;if(!valid_access(r[1]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c051066; /* mov.l @(0x20,r1),r0 */
L8c051066: *stop=0x8c051066u;if(!valid_access(r[1]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051068; /* mov.l @(0x24,r1),r3 */
L8c051068: *stop=0x8c051068u;if(!valid_access(r[2]+32u,4u))return -3;wr(m,bus,&io_status,r[2]+0x20u,r[0],4);if(io_status)return io_status;goto L8c05106a; /* mov.l r0,@(0x20,r2) */
L8c05106a: *stop=0x8c05106au;if(!valid_access(r[2]+36u,4u))return -3;wr(m,bus,&io_status,r[2]+0x24u,r[3],4);if(io_status)return io_status;goto L8c05106c; /* mov.l r3,@(0x24,r2) */
L8c05106c: *stop=0x8c05106cu;if(!valid_access(r[1]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c05106e; /* mov.l @(0x28,r1),r0 */
L8c05106e: *stop=0x8c05106eu;if(!valid_access(r[1]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051070; /* mov.l @(0x2c,r1),r3 */
L8c051070: *stop=0x8c051070u;if(!valid_access(r[2]+40u,4u))return -3;wr(m,bus,&io_status,r[2]+0x28u,r[0],4);if(io_status)return io_status;goto L8c051072; /* mov.l r0,@(0x28,r2) */
L8c051072: *stop=0x8c051072u;if(!valid_access(r[2]+44u,4u))return -3;wr(m,bus,&io_status,r[2]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c051074; /* mov.l r3,@(0x2c,r2) */
L8c051074: *stop=0x8c051074u;if(!valid_access(r[1]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c051076; /* mov.l @(0x30,r1),r0 */
L8c051076: *stop=0x8c051076u;if(!valid_access(r[2]+48u,4u))return -3;wr(m,bus,&io_status,r[2]+0x30u,r[0],4);if(io_status)return io_status;goto L8c051078; /* mov.l r0,@(0x30,r2) */
L8c051078: *stop=0x8c05107au;goto L8c0510a6; /* bra 0x8c0510a6 */
L8c05107c: *stop=0x8c05107cu;r[3]=52u;goto L8c05107e; /* mov #0x34,r3 */
L8c05107e: *stop=0x8c05107eu;r[0]=r[14];goto L8c051080; /* mov r14,r0 */
L8c051080: *stop=0x8c051080u;r[7]=r[4]&255u;goto L8c051082; /* extu.b r4,r7 */
L8c051082: *stop=0x8c051082u;*macl=(r[3]&65535u)*(r[7]&65535u);goto L8c051084; /* mulu.w r3,r7 */
L8c051084: *stop=0x8c051084u;r[7]=*macl;goto L8c051086; /* sts MACL,r7 */
L8c051086: *stop=0x8c051086u;r[7]+=r[12];goto L8c051088; /* add r12,r7 */
L8c051088: *stop=0x8c051088u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c05108a; /* mov.l @(0x8,r7),r2 */
L8c05108a: *stop=0x8c05108au;if(!valid_access(r[7]+44u,4u))return -3;wr(m,bus,&io_status,r[7]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c05108c; /* mov.l r2,@(0x2c,r7) */
L8c05108c: *stop=0x8c05108cu;if(!valid_access(r[7]+8u,4u))return -3;wr(m,bus,&io_status,r[7]+0x8u,r[14],4);if(io_status)return io_status;goto L8c05108e; /* mov.l r14,@(0x8,r7) */
L8c05108e: *stop=0x8c05108eu;if(!valid_access(r[7]+12u,4u))return -3;wr(m,bus,&io_status,r[7]+0xcu,r[13],4);if(io_status)return io_status;goto L8c051090; /* mov.l r13,@(0xc,r7) */
L8c051090: *stop=0x8c051090u;if(!valid_access(r[7]+16u,4u))return -3;wr(m,bus,&io_status,r[7]+0x10u,r[14],4);if(io_status)return io_status;goto L8c051092; /* mov.l r14,@(0x10,r7) */
L8c051092: *stop=0x8c051092u;if(!valid_access(r[7]+20u,4u))return -3;wr(m,bus,&io_status,r[7]+0x14u,r[14],4);if(io_status)return io_status;goto L8c051094; /* mov.l r14,@(0x14,r7) */
L8c051094: *stop=0x8c051094u;if(!valid_access(r[7]+28u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c051096; /* mov.w r0,@(0x1c,r7) */
L8c051096: *stop=0x8c051096u;if(!valid_access(r[7]+30u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1eu,r[0],2);if(io_status)return io_status;goto L8c051098; /* mov.w r0,@(0x1e,r7) */
L8c051098: *stop=0x8c051098u;r[0]=r[5];goto L8c05109a; /* mov r5,r0 */
L8c05109a: *stop=0x8c05109au;if(!valid_access(r[7]+26u,2u))return -3;wr(m,bus,&io_status,r[7]+0x1au,r[0],2);if(io_status)return io_status;goto L8c05109c; /* mov.w r0,@(0x1a,r7) */
L8c05109c: *stop=0x8c05109cu;if(!valid_access(r[7]+24u,2u))return -3;wr(m,bus,&io_status,r[7]+0x18u,r[0],2);if(io_status)return io_status;goto L8c05109e; /* mov.w r0,@(0x18,r7) */
L8c05109e: *stop=0x8c05109eu;r[0]=32u;goto L8c0510a0; /* mov #0x20,r0 */
L8c0510a0: *stop=0x8c0510a0u;if(!valid_access(r[0]+r[7],2u))return -3;wr(m,bus,&io_status,r[0]+r[7],r[6],2);if(io_status)return io_status;goto L8c0510a2; /* mov.w r6,@(r0,r7) */
L8c0510a2: *stop=0x8c0510a2u;r[0]=34u;goto L8c0510a4; /* mov #0x22,r0 */
L8c0510a4: *stop=0x8c0510a4u;if(!valid_access(r[0]+r[7],2u))return -3;wr(m,bus,&io_status,r[0]+r[7],r[6],2);if(io_status)return io_status;goto L8c0510a6; /* mov.w r6,@(r0,r7) */
L8c0510a6: *stop=0x8c0510a6u;r[4]+=1u;goto L8c0510a8; /* add #0x1,r4 */
L8c0510a8: *stop=0x8c0510a8u;r[2]=8u;goto L8c0510aa; /* mov #0x8,r2 */
L8c0510aa: *stop=0x8c0510aau;r[3]=r[4]&255u;goto L8c0510ac; /* extu.b r4,r3 */
L8c0510ac: *stop=0x8c0510acu;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c0510ae; /* cmp/ge r2,r3 */
L8c0510ae: if((*sr&1u))goto L8c0510b4;goto L8c0510b0; /* bt 0x8c0510b4 */
L8c0510b0: *stop=0x8c0510b2u;goto L8c050d82; /* bra 0x8c050d82 */
L8c0510b4: *stop=0x8c0510b6u;goto L8c05114e; /* bra 0x8c05114e */
L8c0510b8: *stop=0x8c0510b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c0510ba; /* mov.l @r15,r4 */
L8c0510ba: *stop=0x8c0510bau;r[7]=r[14];goto L8c0510bc; /* mov r14,r7 */
L8c0510bc: *stop=0x8c0510bcu;r[0]=r[14];goto L8c0510be; /* mov r14,r0 */
L8c0510be: *stop=0x8c0510beu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0510c0; /* mov.l @(0x8,r4),r3 */
L8c0510c0: *stop=0x8c0510c0u;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c0510c2; /* mov.l r3,@(0x2c,r4) */
L8c0510c2: *stop=0x8c0510c2u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[14],4);if(io_status)return io_status;goto L8c0510c4; /* mov.l r14,@(0x8,r4) */
L8c0510c4: *stop=0x8c0510c4u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[13],4);if(io_status)return io_status;goto L8c0510c6; /* mov.l r13,@(0xc,r4) */
L8c0510c6: *stop=0x8c0510c6u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[14],4);if(io_status)return io_status;goto L8c0510c8; /* mov.l r14,@(0x10,r4) */
L8c0510c8: *stop=0x8c0510c8u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[14],4);if(io_status)return io_status;goto L8c0510ca; /* mov.l r14,@(0x14,r4) */
L8c0510ca: *stop=0x8c0510cau;if(!valid_access(r[4]+28u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c0510cc; /* mov.w r0,@(0x1c,r4) */
L8c0510cc: *stop=0x8c0510ccu;if(!valid_access(r[4]+30u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1eu,r[0],2);if(io_status)return io_status;goto L8c0510ce; /* mov.w r0,@(0x1e,r4) */
L8c0510ce: *stop=0x8c0510ceu;r[0]=r[5];goto L8c0510d0; /* mov r5,r0 */
L8c0510d0: *stop=0x8c0510d0u;if(!valid_access(r[4]+26u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1au,r[0],2);if(io_status)return io_status;goto L8c0510d2; /* mov.w r0,@(0x1a,r4) */
L8c0510d2: *stop=0x8c0510d2u;if(!valid_access(r[4]+24u,2u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[0],2);if(io_status)return io_status;goto L8c0510d4; /* mov.w r0,@(0x18,r4) */
L8c0510d4: *stop=0x8c0510d4u;r[0]=32u;goto L8c0510d6; /* mov #0x20,r0 */
L8c0510d6: *stop=0x8c0510d6u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[6],2);if(io_status)return io_status;goto L8c0510d8; /* mov.w r6,@(r0,r4) */
L8c0510d8: *stop=0x8c0510d8u;r[0]=34u;goto L8c0510da; /* mov #0x22,r0 */
L8c0510da: *stop=0x8c0510dau;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[6],2);if(io_status)return io_status;goto L8c0510dc; /* mov.w r6,@(r0,r4) */
L8c0510dc: *stop=0x8c0510dcu;r[0]=r[14];goto L8c0510de; /* mov r14,r0 */
L8c0510de: *stop=0x8c0510deu;r[4]+=52u;goto L8c0510e0; /* add #0x34,r4 */
L8c0510e0: *stop=0x8c0510e0u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0510e2; /* mov.l @(0x8,r4),r3 */
L8c0510e2: *stop=0x8c0510e2u;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c0510e4; /* mov.l r3,@(0x2c,r4) */
L8c0510e4: *stop=0x8c0510e4u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[14],4);if(io_status)return io_status;goto L8c0510e6; /* mov.l r14,@(0x8,r4) */
L8c0510e6: *stop=0x8c0510e6u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[13],4);if(io_status)return io_status;goto L8c0510e8; /* mov.l r13,@(0xc,r4) */
L8c0510e8: *stop=0x8c0510e8u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[14],4);if(io_status)return io_status;goto L8c0510ea; /* mov.l r14,@(0x10,r4) */
L8c0510ea: *stop=0x8c0510eau;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[14],4);if(io_status)return io_status;goto L8c0510ec; /* mov.l r14,@(0x14,r4) */
L8c0510ec: *stop=0x8c0510ecu;if(!valid_access(r[4]+28u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c0510ee; /* mov.w r0,@(0x1c,r4) */
L8c0510ee: *stop=0x8c0510eeu;if(!valid_access(r[4]+30u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1eu,r[0],2);if(io_status)return io_status;goto L8c0510f0; /* mov.w r0,@(0x1e,r4) */
L8c0510f0: *stop=0x8c0510f0u;r[0]=r[5];goto L8c0510f2; /* mov r5,r0 */
L8c0510f2: *stop=0x8c0510f2u;if(!valid_access(r[4]+26u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1au,r[0],2);if(io_status)return io_status;goto L8c0510f4; /* mov.w r0,@(0x1a,r4) */
L8c0510f4: *stop=0x8c0510f4u;if(!valid_access(r[4]+24u,2u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[0],2);if(io_status)return io_status;goto L8c0510f6; /* mov.w r0,@(0x18,r4) */
L8c0510f6: *stop=0x8c0510f6u;r[0]=32u;goto L8c0510f8; /* mov #0x20,r0 */
L8c0510f8: *stop=0x8c0510f8u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[6],2);if(io_status)return io_status;goto L8c0510fa; /* mov.w r6,@(r0,r4) */
L8c0510fa: *stop=0x8c0510fau;r[0]=34u;goto L8c0510fc; /* mov #0x22,r0 */
L8c0510fc: *stop=0x8c0510fcu;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[6],2);if(io_status)return io_status;goto L8c0510fe; /* mov.w r6,@(r0,r4) */
L8c0510fe: *stop=0x8c0510feu;r[0]=r[14];goto L8c051100; /* mov r14,r0 */
L8c051100: *stop=0x8c051100u;r[4]+=52u;goto L8c051102; /* add #0x34,r4 */
L8c051102: *stop=0x8c051102u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051104; /* mov.l @(0x8,r4),r3 */
L8c051104: *stop=0x8c051104u;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c051106; /* mov.l r3,@(0x2c,r4) */
L8c051106: *stop=0x8c051106u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[14],4);if(io_status)return io_status;goto L8c051108; /* mov.l r14,@(0x8,r4) */
L8c051108: *stop=0x8c051108u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[13],4);if(io_status)return io_status;goto L8c05110a; /* mov.l r13,@(0xc,r4) */
L8c05110a: *stop=0x8c05110au;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[14],4);if(io_status)return io_status;goto L8c05110c; /* mov.l r14,@(0x10,r4) */
L8c05110c: *stop=0x8c05110cu;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[14],4);if(io_status)return io_status;goto L8c05110e; /* mov.l r14,@(0x14,r4) */
L8c05110e: *stop=0x8c05110eu;if(!valid_access(r[4]+28u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c051110; /* mov.w r0,@(0x1c,r4) */
L8c051110: *stop=0x8c051110u;if(!valid_access(r[4]+30u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1eu,r[0],2);if(io_status)return io_status;goto L8c051112; /* mov.w r0,@(0x1e,r4) */
L8c051112: *stop=0x8c051112u;r[0]=r[5];goto L8c051114; /* mov r5,r0 */
L8c051114: *stop=0x8c051114u;if(!valid_access(r[4]+26u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1au,r[0],2);if(io_status)return io_status;goto L8c051116; /* mov.w r0,@(0x1a,r4) */
L8c051116: *stop=0x8c051116u;if(!valid_access(r[4]+24u,2u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[0],2);if(io_status)return io_status;goto L8c051118; /* mov.w r0,@(0x18,r4) */
L8c051118: *stop=0x8c051118u;r[0]=32u;goto L8c05111a; /* mov #0x20,r0 */
L8c05111a: *stop=0x8c05111au;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[6],2);if(io_status)return io_status;goto L8c05111c; /* mov.w r6,@(r0,r4) */
L8c05111c: *stop=0x8c05111cu;r[0]=34u;goto L8c05111e; /* mov #0x22,r0 */
L8c05111e: *stop=0x8c05111eu;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[6],2);if(io_status)return io_status;goto L8c051120; /* mov.w r6,@(r0,r4) */
L8c051120: *stop=0x8c051120u;r[0]=r[14];goto L8c051122; /* mov r14,r0 */
L8c051122: *stop=0x8c051122u;r[4]+=52u;goto L8c051124; /* add #0x34,r4 */
L8c051124: *stop=0x8c051124u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051126; /* mov.l @(0x8,r4),r3 */
L8c051126: *stop=0x8c051126u;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c051128; /* mov.l r3,@(0x2c,r4) */
L8c051128: *stop=0x8c051128u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[14],4);if(io_status)return io_status;goto L8c05112a; /* mov.l r14,@(0x8,r4) */
L8c05112a: *stop=0x8c05112au;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[13],4);if(io_status)return io_status;goto L8c05112c; /* mov.l r13,@(0xc,r4) */
L8c05112c: *stop=0x8c05112cu;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[14],4);if(io_status)return io_status;goto L8c05112e; /* mov.l r14,@(0x10,r4) */
L8c05112e: *stop=0x8c05112eu;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[14],4);if(io_status)return io_status;goto L8c051130; /* mov.l r14,@(0x14,r4) */
L8c051130: *stop=0x8c051130u;if(!valid_access(r[4]+28u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c051132; /* mov.w r0,@(0x1c,r4) */
L8c051132: *stop=0x8c051132u;if(!valid_access(r[4]+30u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1eu,r[0],2);if(io_status)return io_status;goto L8c051134; /* mov.w r0,@(0x1e,r4) */
L8c051134: *stop=0x8c051134u;r[0]=r[5];goto L8c051136; /* mov r5,r0 */
L8c051136: *stop=0x8c051136u;if(!valid_access(r[4]+26u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1au,r[0],2);if(io_status)return io_status;goto L8c051138; /* mov.w r0,@(0x1a,r4) */
L8c051138: *stop=0x8c051138u;if(!valid_access(r[4]+24u,2u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[0],2);if(io_status)return io_status;goto L8c05113a; /* mov.w r0,@(0x18,r4) */
L8c05113a: *stop=0x8c05113au;r[0]=32u;goto L8c05113c; /* mov #0x20,r0 */
L8c05113c: *stop=0x8c05113cu;r[7]+=4u;goto L8c05113e; /* add #0x4,r7 */
L8c05113e: *stop=0x8c05113eu;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[6],2);if(io_status)return io_status;goto L8c051140; /* mov.w r6,@(r0,r4) */
L8c051140: *stop=0x8c051140u;r[3]=r[7]&255u;goto L8c051142; /* extu.b r7,r3 */
L8c051142: *stop=0x8c051142u;r[2]=8u;goto L8c051144; /* mov #0x8,r2 */
L8c051144: *stop=0x8c051144u;r[0]=34u;goto L8c051146; /* mov #0x22,r0 */
L8c051146: *stop=0x8c051146u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c051148; /* cmp/ge r2,r3 */
L8c051148: *stop=0x8c051148u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[6],2);if(io_status)return io_status;goto L8c05114a; /* mov.w r6,@(r0,r4) */
L8c05114a: take=!(*sr&1u);*stop=0x8c05114cu;r[4]+=52u;if(take)goto L8c0510bc;goto L8c05114e; /* bf/s 0x8c0510bc */
L8c05114e: target=0x8c050a40u;*pr=0x8c051152u;*stop=0x8c051150u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051152; /* bsr 0x8c050a40 */
L8c051152: target=0x8c05117eu;*pr=0x8c051156u;*stop=0x8c051154u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051156; /* bsr 0x8c05117e */
L8c051156: *stop=0x8c051156u;r[13]=r[14];goto L8c051158; /* mov r14,r13 */
L8c051158: *stop=0x8c051158u;r[14]=4u;goto L8c05115a; /* mov #0x4,r14 */
L8c05115a: target=0x8c050aceu;*pr=0x8c05115eu;*stop=0x8c05115cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05115e; /* bsr 0x8c050ace */
L8c05115e: *stop=0x8c05115eu;r[13]+=1u;goto L8c051160; /* add #0x1,r13 */
L8c051160: target=0x8c050aceu;*pr=0x8c051164u;*stop=0x8c051162u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051164; /* bsr 0x8c050ace */
L8c051164: *stop=0x8c051164u;r[13]+=1u;goto L8c051166; /* add #0x1,r13 */
L8c051166: *stop=0x8c051166u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[14]);goto L8c051168; /* cmp/ge r14,r13 */
L8c051168: if(!(*sr&1u))goto L8c05115a;goto L8c05116a; /* bf 0x8c05115a */
L8c05116a: *stop=0x8c05116au;r[15]+=12u;goto L8c05116c; /* add #0xc,r15 */
L8c05116c: *stop=0x8c05116cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c05116e; /* lds.l @r15+,PR */
L8c05116e: *stop=0x8c05116eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c051170; /* mov.l @r15+,r8 */
L8c051170: *stop=0x8c051170u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c051172; /* mov.l @r15+,r9 */
L8c051172: *stop=0x8c051172u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c051174; /* mov.l @r15+,r10 */
L8c051174: *stop=0x8c051174u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c051176; /* mov.l @r15+,r11 */
L8c051176: *stop=0x8c051176u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c051178; /* mov.l @r15+,r12 */
L8c051178: *stop=0x8c051178u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c05117a; /* mov.l @r15+,r13 */
L8c05117a: *stop=0x8c05117au;if(*pr!=entry_pr)return -3;*stop=0x8c05117cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c10d31c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10d31c;
L8c10d31c: *stop=0x8c10d31cu;r[7]=0u;goto L8c10d31e; /* mov #0x0,r7 */
L8c10d31e: *stop=0x8c10d31eu;r[3]=r[7];goto L8c10d320; /* mov r7,r3 */
L8c10d320: *stop=0x8c10d320u;*sr=(*sr&~1u)|(r[3]>=r[6]);goto L8c10d322; /* cmp/hs r6,r3 */
L8c10d322: take=(*sr&1u);*stop=0x8c10d324u;r[0]=r[4];if(take)goto L8c10d330;goto L8c10d326; /* bt/s 0x8c10d330 */
L8c10d326: *stop=0x8c10d326u;r[7]+=1u;goto L8c10d328; /* add #0x1,r7 */
L8c10d328: *stop=0x8c10d328u;if(!valid_access(r[0],1u))return -3;wr(m,bus,&io_status,r[0],r[5],1);if(io_status)return io_status;goto L8c10d32a; /* mov.b r5,@r0 */
L8c10d32a: *stop=0x8c10d32au;*sr=(*sr&~1u)|(r[7]>=r[6]);goto L8c10d32c; /* cmp/hs r6,r7 */
L8c10d32c: take=!(*sr&1u);*stop=0x8c10d32eu;r[0]+=1u;if(take)goto L8c10d326;goto L8c10d330; /* bf/s 0x8c10d326 */
L8c10d330: *stop=0x8c10d330u;if(*pr!=entry_pr)return -3;*stop=0x8c10d332u;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c6087f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6087f4;
L8c6087f4: *stop=0x8c6087f4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6087f6; /* mov.l r14,@-r15 */
L8c6087f6: *stop=0x8c6087f6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6087f8; /* mov.l r13,@-r15 */
L8c6087f8: *stop=0x8c6087f8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6087fa; /* mov.l r12,@-r15 */
L8c6087fa: *stop=0x8c6087fau;r[12]=0u;goto L8c6087fc; /* mov #0x0,r12 */
L8c6087fc: *stop=0x8c6087fcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6087fe; /* mov.l r11,@-r15 */
L8c6087fe: *stop=0x8c6087feu;r[11]=r[12];goto L8c608800; /* mov r12,r11 */
L8c608800: *stop=0x8c608800u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c608802; /* mov.l r10,@-r15 */
L8c608802: *stop=0x8c608802u;r[10]=r[12];goto L8c608804; /* mov r12,r10 */
L8c608804: *stop=0x8c608804u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c608806; /* mov.l r9,@-r15 */
L8c608806: *stop=0x8c608806u;r[9]=r[12];goto L8c608808; /* mov r12,r9 */
L8c608808: *stop=0x8c608808u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c60880a; /* mov.l r8,@-r15 */
L8c60880a: *stop=0x8c60880au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c60880c; /* sts.l PR,@-r15 */
L8c60880c: *stop=0x8c60880cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6088a8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c60880e; /* mov.l 0x8c6088a8,r14 */
L8c60880e: *stop=0x8c608810u;r[8]=r[12];goto L8c608886; /* bra 0x8c608886 */
L8c608812: *stop=0x8c608812u;r[13]=r[11];goto L8c608814; /* mov r11,r13 */
L8c608814: *stop=0x8c608814u;r[13]<<=2;goto L8c608816; /* shll2 r13 */
L8c608816: *stop=0x8c608816u;r[13]<<=2;goto L8c608818; /* shll2 r13 */
L8c608818: *stop=0x8c608818u;r[3]=r[11];goto L8c60881a; /* mov r11,r3 */
L8c60881a: *stop=0x8c60881au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c60881c; /* mov.l @r14,r0 */
L8c60881c: *stop=0x8c60881cu;r[13]+=r[3];goto L8c60881e; /* add r3,r13 */
L8c60881e: *stop=0x8c60881eu;r[13]<<=2;goto L8c608820; /* shll2 r13 */
L8c608820: *stop=0x8c608820u;r[0]+=r[13];goto L8c608822; /* add r13,r0 */
L8c608822: *stop=0x8c608822u;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c608824; /* mov.l @(0x10,r0),r0 */
L8c608824: *stop=0x8c608824u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c608826; /* cmp/eq #0x2,r0 */
L8c608826: if(!(*sr&1u))goto L8c608858;goto L8c608828; /* bf 0x8c608858 */
L8c608828: *stop=0x8c608828u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c60882a; /* mov.l @r14,r4 */
L8c60882a: *stop=0x8c60882au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6088acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60882c; /* mov.l 0x8c6088ac,r3 */
L8c60882c: *stop=0x8c60882cu;r[4]+=r[13];goto L8c60882e; /* add r13,r4 */
L8c60882e: target=r[3];*pr=0x8c608832u;*stop=0x8c608830u;r[4]+=24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608832; /* jsr @r3 */
L8c608832: *stop=0x8c608832u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608834; /* mov.l @r14,r3 */
L8c608834: *stop=0x8c608834u;r[4]=4294967293u;goto L8c608836; /* mov #-0x3,r4 */
L8c608836: *stop=0x8c608836u;r[3]+=r[13];goto L8c608838; /* add r13,r3 */
L8c608838: *stop=0x8c608838u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60883a; /* mov.l @(0x10,r3),r2 */
L8c60883a: *stop=0x8c60883au;r[2]&=r[4];goto L8c60883c; /* and r4,r2 */
L8c60883c: *stop=0x8c60883cu;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[2],4);if(io_status)return io_status;goto L8c60883e; /* mov.l r2,@(0x10,r3) */
L8c60883e: *stop=0x8c60883eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608840; /* mov.l @r14,r3 */
L8c608840: *stop=0x8c608840u;r[3]+=r[13];goto L8c608842; /* add r13,r3 */
L8c608842: *stop=0x8c608842u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608844; /* mov.l @(0x10,r3),r2 */
L8c608844: *stop=0x8c608844u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c608846; /* tst r2,r2 */
L8c608846: if((*sr&1u))goto L8c60885c;goto L8c608848; /* bt 0x8c60885c */
L8c608848: *stop=0x8c608848u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60884a; /* mov.l @r14,r2 */
L8c60884a: *stop=0x8c60884au;r[0]=64u;goto L8c60884c; /* mov #0x40,r0 */
L8c60884c: *stop=0x8c60884cu;r[2]+=r[13];goto L8c60884e; /* add r13,r2 */
L8c60884e: *stop=0x8c60884eu;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[12],2);if(io_status)return io_status;goto L8c608850; /* mov.w r12,@(r0,r2) */
L8c608850: *stop=0x8c608850u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c608852; /* mov.l @r14,r3 */
L8c608852: *stop=0x8c608852u;r[3]+=r[13];goto L8c608854; /* add r13,r3 */
L8c608854: *stop=0x8c608856u;if(!valid_access(r[3]+52u,4u))return -3;wr(m,bus,&io_status,r[3]+0x34u,r[12],4);if(io_status)return io_status;goto L8c608864; /* bra 0x8c608864 */
L8c608858: *stop=0x8c608858u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c60885a; /* cmp/eq #0x4,r0 */
L8c60885a: if(!(*sr&1u))goto L8c608864;goto L8c60885c; /* bf 0x8c608864 */
L8c60885c: *stop=0x8c60885cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6088b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60885e; /* mov.l 0x8c6088b0,r2 */
L8c60885e: *stop=0x8c60885eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c608860; /* mov.l @r14,r4 */
L8c608860: target=r[2];*pr=0x8c608864u;*stop=0x8c608862u;r[4]+=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608864; /* jsr @r2 */
L8c608864: *stop=0x8c608864u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c608866; /* mov.l @r14,r0 */
L8c608866: *stop=0x8c608866u;if(!valid_access(r[0]+r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c608868; /* mov.l @(r0,r8),r0 */
L8c608868: *stop=0x8c608868u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c60886a; /* cmp/eq #-0x1,r0 */
L8c60886a: if((*sr&1u))goto L8c60887e;goto L8c60886c; /* bt 0x8c60887e */
L8c60886c: *stop=0x8c60886cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60886e; /* mov.l @r14,r2 */
L8c60886e: *stop=0x8c60886eu;r[2]+=r[9];goto L8c608870; /* add r9,r2 */
L8c608870: *stop=0x8c608870u;if(!valid_access(r[2]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608872; /* mov.l @(0x10,r2),r3 */
L8c608872: *stop=0x8c608872u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c608874; /* tst r3,r3 */
L8c608874: if(!(*sr&1u))goto L8c60887e;goto L8c608876; /* bf 0x8c60887e */
L8c608876: *stop=0x8c608876u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6088b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608878; /* mov.l 0x8c6088b0,r3 */
L8c608878: *stop=0x8c608878u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c60887a; /* mov.l @r14,r4 */
L8c60887a: target=r[3];*pr=0x8c60887eu;*stop=0x8c60887cu;r[4]+=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60887e; /* jsr @r3 */
L8c60887e: *stop=0x8c60887eu;r[9]+=68u;goto L8c608880; /* add #0x44,r9 */
L8c608880: *stop=0x8c608880u;r[8]+=68u;goto L8c608882; /* add #0x44,r8 */
L8c608882: *stop=0x8c608882u;r[11]+=1u;goto L8c608884; /* add #0x1,r11 */
L8c608884: *stop=0x8c608884u;r[10]+=68u;goto L8c608886; /* add #0x44,r10 */
L8c608886: *stop=0x8c608886u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6088b4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c608888; /* mov.l 0x8c6088b4,r4 */
L8c608888: *stop=0x8c608888u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60888a; /* mov.l @r4,r2 */
L8c60888a: *stop=0x8c60888au;*sr=(*sr&~1u)|(r[11]>=r[2]);goto L8c60888c; /* cmp/hs r2,r11 */
L8c60888c: if(!(*sr&1u))goto L8c608812;goto L8c60888e; /* bf 0x8c608812 */
L8c60888e: *stop=0x8c60888eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6088b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608890; /* mov.l 0x8c6088b8,r3 */
L8c608890: *stop=0x8c608890u;r[1]=4294967295u;goto L8c608892; /* mov #-0x1,r1 */
L8c608892: *stop=0x8c608892u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c608894; /* mov.l r1,@r3 */
L8c608894: *stop=0x8c608894u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c608896; /* lds.l @r15+,PR */
L8c608896: *stop=0x8c608896u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c608898; /* mov.l @r15+,r8 */
L8c608898: *stop=0x8c608898u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c60889a; /* mov.l @r15+,r9 */
L8c60889a: *stop=0x8c60889au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c60889c; /* mov.l @r15+,r10 */
L8c60889c: *stop=0x8c60889cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c60889e; /* mov.l @r15+,r11 */
L8c60889e: *stop=0x8c60889eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6088a0; /* mov.l @r15+,r12 */
L8c6088a0: *stop=0x8c6088a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6088a2; /* mov.l @r15+,r13 */
L8c6088a2: *stop=0x8c6088a2u;if(*pr!=entry_pr)return -3;*stop=0x8c6088a4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c640752(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c640752;
L8c640752: *stop=0x8c640752u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6407acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c640754; /* mov.l 0x8c6407ac,r3 */
L8c640754: *stop=0x8c640754u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640780u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c640756; /* mov.l 0x8c640780,r4 */
L8c640756: target=r[3];*stop=0x8c640758u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_main_8c6049c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6049c0;
L8c6049c0: *stop=0x8c6049c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6049c2; /* mov.l r14,@-r15 */
L8c6049c2: *stop=0x8c6049c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6049c4; /* mov.l r13,@-r15 */
L8c6049c4: *stop=0x8c6049c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6049c6; /* mov.l r12,@-r15 */
L8c6049c6: *stop=0x8c6049c6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6049c8; /* mov.l r11,@-r15 */
L8c6049c8: *stop=0x8c6049c8u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6049ca; /* fmov.s fr15,@-r15 */
L8c6049ca: *stop=0x8c6049cau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6049cc; /* fmov.s fr14,@-r15 */
L8c6049cc: *stop=0x8c6049ccu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6049ce; /* fmov.s fr13,@-r15 */
L8c6049ce: *stop=0x8c6049ceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6049d0; /* sts.l PR,@-r15 */
L8c6049d0: *stop=0x8c6049d0u;r[15]+=4294967284u;goto L8c6049d2; /* add #-0xc,r15 */
L8c6049d2: *stop=0x8c6049d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604ab4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6049d4; /* mov.l 0x8c604ab4,r2 */
L8c6049d4: *stop=0x8c6049d4u;r[13]=r[4];goto L8c6049d6; /* mov r4,r13 */
L8c6049d6: *stop=0x8c6049d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604ab8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6049d8; /* mov.l 0x8c604ab8,r1 */
L8c6049d8: *stop=0x8c6049d8u;r[4]=3u;goto L8c6049da; /* mov #0x3,r4 */
L8c6049da: *stop=0x8c6049dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c604ac0u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c6049dc; /* mov.l 0x8c604ac0,r12 */
L8c6049dc: *stop=0x8c6049dcu;r[0]=0x8c604abcu;goto L8c6049de; /* mova 0x8c604abc,r0 */
L8c6049de: *stop=0x8c6049deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604ac4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6049e0; /* mov.l 0x8c604ac4,r14 */
L8c6049e0: *stop=0x8c6049e0u;r[3]=r[5];goto L8c6049e2; /* mov r5,r3 */
L8c6049e2: *stop=0x8c6049e2u;r[3]=sx(m,bus,&io_status,0x8c604aa4u,2);if(io_status)return io_status;goto L8c6049e4; /* mov.w 0x8c604aa4,r3 */
L8c6049e4: *stop=0x8c6049e4u;r[4]&=r[13];goto L8c6049e6; /* and r13,r4 */
L8c6049e6: *stop=0x8c6049e6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c6049e8; /* mov.l r5,@r15 */
L8c6049e8: *stop=0x8c6049e8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c6049ea; /* mov.l r5,@r2 */
L8c6049ea: *stop=0x8c6049eau;r[3]&=r[13];goto L8c6049ec; /* and r13,r3 */
L8c6049ec: *stop=0x8c6049ecu;r[2]=16u;goto L8c6049ee; /* mov #0x10,r2 */
L8c6049ee: *stop=0x8c6049eeu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[6],4);if(io_status)return io_status;goto L8c6049f0; /* mov.l r6,@r1 */
L8c6049f0: *stop=0x8c6049f0u;*sr=(*sr&~1u)|!(r[13]&r[2]);goto L8c6049f2; /* tst r13,r2 */
L8c6049f2: *stop=0x8c6049f2u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6049f4; /* mov.l r3,@(0x8,r15) */
L8c6049f4: take=(*sr&1u);*stop=0x8c6049f6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]);if(io_status)return io_status;if(take)goto L8c604a00;goto L8c6049f8; /* bt/s 0x8c604a00 */
L8c6049f8: *stop=0x8c6049f8u;r[3]=sx(m,bus,&io_status,0x8c604aa6u,2);if(io_status)return io_status;goto L8c6049fa; /* mov.w 0x8c604aa6,r3 */
L8c6049fa: *stop=0x8c6049fau;r[0]=4u;goto L8c6049fc; /* mov #0x4,r0 */
L8c6049fc: *stop=0x8c6049feu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,13u,r[0]+r[14]);if(io_status)return io_status;goto L8c604a0a; /* bra 0x8c604a0a */
L8c604a00: *stop=0x8c604a00u;r[0]=0x8c604ac8u;goto L8c604a02; /* mova 0x8c604ac8,r0 */
L8c604a02: *stop=0x8c604a02u;r[3]=sx(m,bus,&io_status,0x8c604aa8u,2);if(io_status)return io_status;goto L8c604a04; /* mov.w 0x8c604aa8,r3 */
L8c604a04: *stop=0x8c604a04u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c604a06; /* fmov.s @r0,fr3 */
L8c604a06: *stop=0x8c604a06u;r[0]=4u;goto L8c604a08; /* mov #0x4,r0 */
L8c604a08: *stop=0x8c604a08u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c604a0a; /* fmov fr3,@(r0,r14) */
L8c604a0a: *stop=0x8c604a0au;r[0]=0x8c604accu;goto L8c604a0c; /* mova 0x8c604acc,r0 */
L8c604a0c: *stop=0x8c604a0cu;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;goto L8c604a0e; /* mov.l r3,@r12 */
L8c604a0e: *stop=0x8c604a0eu;r[3]=32u;goto L8c604a10; /* mov #0x20,r3 */
L8c604a10: *stop=0x8c604a10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604ad0u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c604a12; /* mov.l 0x8c604ad0,r11 */
L8c604a12: *stop=0x8c604a12u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c604a14; /* fmov.s @r0,fr14 */
L8c604a14: *stop=0x8c604a14u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c604a16; /* tst r13,r3 */
L8c604a16: *stop=0x8c604a16u;r[0]=0x8c604ad4u;goto L8c604a18; /* mova 0x8c604ad4,r0 */
L8c604a18: take=(*sr&1u);*stop=0x8c604a1au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;if(take)goto L8c604a24;goto L8c604a1c; /* bt/s 0x8c604a24 */
L8c604a1c: *stop=0x8c604a1cu;r[3]=sx(m,bus,&io_status,0x8c604aaau,2);if(io_status)return io_status;goto L8c604a1e; /* mov.w 0x8c604aaa,r3 */
L8c604a1e: *stop=0x8c604a1eu;r[0]=8u;goto L8c604a20; /* mov #0x8,r0 */
L8c604a20: *stop=0x8c604a22u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c604a2a; /* bra 0x8c604a2a */
L8c604a24: *stop=0x8c604a24u;r[3]=sx(m,bus,&io_status,0x8c604aacu,2);if(io_status)return io_status;goto L8c604a26; /* mov.w 0x8c604aac,r3 */
L8c604a26: *stop=0x8c604a26u;r[0]=8u;goto L8c604a28; /* mov #0x8,r0 */
L8c604a28: *stop=0x8c604a28u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c604a2a; /* fmov fr14,@(r0,r14) */
L8c604a2a: *stop=0x8c604a2au;r[0]=r[4];goto L8c604a2c; /* mov r4,r0 */
L8c604a2c: *stop=0x8c604a2cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c604a2e; /* cmp/eq #0x1,r0 */
L8c604a2e: take=!(*sr&1u);*stop=0x8c604a30u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;if(take)goto L8c604a4c;goto L8c604a32; /* bf/s 0x8c604a4c */
L8c604a32: target=0x8c604594u;*pr=0x8c604a36u;*stop=0x8c604a34u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604a36; /* bsr 0x8c604594 */
L8c604a36: target=0x8c6045b4u;*pr=0x8c604a3au;*stop=0x8c604a38u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604a3a; /* bsr 0x8c6045b4 */
L8c604a3a: *stop=0x8c604a3au;r[3]=sx(m,bus,&io_status,0x8c604aa6u,2);if(io_status)return io_status;goto L8c604a3c; /* mov.w 0x8c604aa6,r3 */
L8c604a3c: *stop=0x8c604a3cu;r[0]=4u;goto L8c604a3e; /* mov #0x4,r0 */
L8c604a3e: *stop=0x8c604a3eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,13u,r[0]+r[14]);if(io_status)return io_status;goto L8c604a40; /* fmov fr13,@(r0,r14) */
L8c604a40: *stop=0x8c604a40u;r[0]=8u;goto L8c604a42; /* mov #0x8,r0 */
L8c604a42: *stop=0x8c604a42u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;goto L8c604a44; /* mov.l r3,@r12 */
L8c604a44: *stop=0x8c604a44u;r[3]=sx(m,bus,&io_status,0x8c604aaau,2);if(io_status)return io_status;goto L8c604a46; /* mov.w 0x8c604aaa,r3 */
L8c604a46: *stop=0x8c604a46u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c604a48; /* fmov fr15,@(r0,r14) */
L8c604a48: *stop=0x8c604a4au;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c604a80; /* bra 0x8c604a80 */
L8c604a4c: *stop=0x8c604a4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604ad8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c604a4e; /* mov.l 0x8c604ad8,r1 */
L8c604a4e: target=r[1];*pr=0x8c604a52u;*stop=0x8c604a50u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604a52; /* jsr @r1 */
L8c604a52: target=0x8c604594u;*pr=0x8c604a56u;*stop=0x8c604a54u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604a56; /* bsr 0x8c604594 */
L8c604a56: *stop=0x8c604a56u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c604a58; /* mov.l @(0x4,r15),r0 */
L8c604a58: *stop=0x8c604a58u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c604a5a; /* cmp/eq #0x2,r0 */
L8c604a5a: if((*sr&1u))goto L8c604a68;goto L8c604a5c; /* bt 0x8c604a68 */
L8c604a5c: *stop=0x8c604a5cu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c604a5e; /* cmp/eq #0x3,r0 */
L8c604a5e: if((*sr&1u))goto L8c604a6e;goto L8c604a60; /* bt 0x8c604a6e */
L8c604a60: *stop=0x8c604a60u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c604a62; /* cmp/eq #0x1,r0 */
L8c604a62: if((*sr&1u))goto L8c604a74;goto L8c604a64; /* bt 0x8c604a74 */
L8c604a64: *stop=0x8c604a66u;goto L8c604a7a; /* bra 0x8c604a7a */
L8c604a68: *stop=0x8c604a68u;r[4]=sx(m,bus,&io_status,0x8c604aaeu,2);if(io_status)return io_status;goto L8c604a6a; /* mov.w 0x8c604aae,r4 */
L8c604a6a: *stop=0x8c604a6cu;goto L8c604a7c; /* bra 0x8c604a7c */
L8c604a6e: *stop=0x8c604a6eu;r[4]=sx(m,bus,&io_status,0x8c604ab0u,2);if(io_status)return io_status;goto L8c604a70; /* mov.w 0x8c604ab0,r4 */
L8c604a70: *stop=0x8c604a72u;goto L8c604a7c; /* bra 0x8c604a7c */
L8c604a74: *stop=0x8c604a74u;r[4]=sx(m,bus,&io_status,0x8c604ab2u,2);if(io_status)return io_status;goto L8c604a76; /* mov.w 0x8c604ab2,r4 */
L8c604a76: *stop=0x8c604a78u;goto L8c604a7c; /* bra 0x8c604a7c */
L8c604a7a: *stop=0x8c604a7au;r[4]=sx(m,bus,&io_status,0x8c604ab0u,2);if(io_status)return io_status;goto L8c604a7c; /* mov.w 0x8c604ab0,r4 */
L8c604a7c: target=0x8c6046c0u;*pr=0x8c604a80u;*stop=0x8c604a7eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604a80; /* bsr 0x8c6046c0 */
L8c604a80: *stop=0x8c604a80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604adcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604a82; /* mov.l 0x8c604adc,r3 */
L8c604a82: *stop=0x8c604a82u;r[4]=1u;goto L8c604a84; /* mov #0x1,r4 */
L8c604a84: *stop=0x8c604a84u;*sr=(*sr&~1u)|!(r[3]&r[13]);goto L8c604a86; /* tst r3,r13 */
L8c604a86: take=(*sr&1u);*stop=0x8c604a88u;r[12]=0u;if(take)goto L8c604ae0;goto L8c604a8a; /* bt/s 0x8c604ae0 */
L8c604a8a: *stop=0x8c604a8au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c604a8c; /* fldi1 fr3 */
L8c604a8c: *stop=0x8c604a8cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c604a8e; /* fadd fr3,fr3 */
L8c604a8e: *stop=0x8c604a8eu;r[0]=4u;goto L8c604a90; /* mov #0x4,r0 */
L8c604a90: *stop=0x8c604a90u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c604a92; /* fmov.s @(r0,r14),fr2 */
L8c604a92: *stop=0x8c604a92u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c604a94; /* fmul fr3,fr2 */
L8c604a94: *stop=0x8c604a94u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c604a96; /* fmov fr2,@(r0,r14) */
L8c604a96: *stop=0x8c604a96u;r[0]=8u;goto L8c604a98; /* mov #0x8,r0 */
L8c604a98: *stop=0x8c604a98u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c604a9a; /* fmov.s @(r0,r14),fr2 */
L8c604a9a: *stop=0x8c604a9au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,0);if(status)return status;goto L8c604a9c; /* fcmp/eq fr14,fr2 */
L8c604a9c: take=!(*sr&1u);*stop=0x8c604a9eu;r[7]=r[4];if(take)goto L8c604ae2;goto L8c604aa0; /* bf/s 0x8c604ae2 */
L8c604aa0: *stop=0x8c604aa2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c604ae2; /* bra 0x8c604ae2 */
L8c604ae0: *stop=0x8c604ae0u;r[7]=r[12];goto L8c604ae2; /* mov r12,r7 */
L8c604ae2: *stop=0x8c604ae2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c604ae4; /* mov.l @r15,r0 */
L8c604ae4: *stop=0x8c604ae4u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c604ae6; /* cmp/eq #0x0,r0 */
L8c604ae6: if((*sr&1u))goto L8c604afc;goto L8c604ae8; /* bt 0x8c604afc */
L8c604ae8: *stop=0x8c604ae8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c604aea; /* cmp/eq #0x1,r0 */
L8c604aea: if((*sr&1u))goto L8c604b00;goto L8c604aec; /* bt 0x8c604b00 */
L8c604aec: *stop=0x8c604aecu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c604aee; /* cmp/eq #0x3,r0 */
L8c604aee: if((*sr&1u))goto L8c604b04;goto L8c604af0; /* bt 0x8c604b04 */
L8c604af0: *stop=0x8c604af0u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c604af2; /* cmp/eq #0x4,r0 */
L8c604af2: if((*sr&1u))goto L8c604b08;goto L8c604af4; /* bt 0x8c604b08 */
L8c604af4: *stop=0x8c604af4u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c604af6; /* cmp/eq #0x5,r0 */
L8c604af6: if((*sr&1u))goto L8c604b0c;goto L8c604af8; /* bt 0x8c604b0c */
L8c604af8: *stop=0x8c604afau;goto L8c604b0e; /* bra 0x8c604b0e */
L8c604afc: *stop=0x8c604afeu;r[5]=r[12];goto L8c604b0e; /* bra 0x8c604b0e */
L8c604b00: *stop=0x8c604b02u;r[5]=r[4];goto L8c604b0e; /* bra 0x8c604b0e */
L8c604b04: *stop=0x8c604b06u;r[5]=3u;goto L8c604b0e; /* bra 0x8c604b0e */
L8c604b08: *stop=0x8c604b0au;r[5]=4u;goto L8c604b0e; /* bra 0x8c604b0e */
L8c604b0c: *stop=0x8c604b0cu;r[5]=5u;goto L8c604b0e; /* mov #0x5,r5 */
L8c604b0e: *stop=0x8c604b0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604b10; /* mov.l 0x8c604b64,r3 */
L8c604b10: *stop=0x8c604b10u;r[6]=1u;goto L8c604b12; /* mov #0x1,r6 */
L8c604b12: *stop=0x8c604b12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b68u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c604b14; /* mov.l 0x8c604b68,r1 */
L8c604b14: *stop=0x8c604b14u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604b16; /* mov.l @r3,r2 */
L8c604b16: *stop=0x8c604b16u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c604b18; /* mov.l r2,@r1 */
L8c604b18: *stop=0x8c604b18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604b1a; /* mov.l 0x8c604b6c,r2 */
L8c604b1a: target=r[2];*pr=0x8c604b1eu;*stop=0x8c604b1cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604b1e; /* jsr @r2 */
L8c604b1e: *stop=0x8c604b1eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604b20; /* mov.l 0x8c604b74,r2 */
L8c604b20: *stop=0x8c604b20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604b22; /* mov.l 0x8c604b70,r3 */
L8c604b22: target=r[2];*pr=0x8c604b26u;*stop=0x8c604b24u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604b26; /* jsr @r2 */
L8c604b26: *stop=0x8c604b26u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b78u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c604b28; /* mov.l 0x8c604b78,r1 */
L8c604b28: *stop=0x8c604b28u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[12],4);if(io_status)return io_status;goto L8c604b2a; /* mov.l r12,@r1 */
L8c604b2a: *stop=0x8c604b2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b7cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604b2c; /* mov.l 0x8c604b7c,r3 */
L8c604b2c: *stop=0x8c604b2cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8c604b2e; /* mov.l r12,@r3 */
L8c604b2e: *stop=0x8c604b2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b80u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c604b30; /* mov.l 0x8c604b80,r0 */
L8c604b30: *stop=0x8c604b30u;r[2]=1u;goto L8c604b32; /* mov #0x1,r2 */
L8c604b32: *stop=0x8c604b32u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c604b34; /* mov.l r2,@r0 */
L8c604b34: *stop=0x8c604b34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b84u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c604b36; /* mov.l 0x8c604b84,r1 */
L8c604b36: *stop=0x8c604b36u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[12],4);if(io_status)return io_status;goto L8c604b38; /* mov.l r12,@r1 */
L8c604b38: *stop=0x8c604b38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604b3a; /* mov.l 0x8c604b8c,r3 */
L8c604b3a: *stop=0x8c604b3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b88u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604b3c; /* mov.l 0x8c604b88,r4 */
L8c604b3c: target=r[3];*pr=0x8c604b40u;*stop=0x8c604b3eu;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604b40; /* jsr @r3 */
L8c604b40: *stop=0x8c604b40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b94u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604b42; /* mov.l 0x8c604b94,r2 */
L8c604b42: *stop=0x8c604b42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b90u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604b44; /* mov.l 0x8c604b90,r4 */
L8c604b44: target=r[2];*pr=0x8c604b48u;*stop=0x8c604b46u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604b48; /* jsr @r2 */
L8c604b48: *stop=0x8c604b48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b9cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604b4a; /* mov.l 0x8c604b9c,r3 */
L8c604b4a: *stop=0x8c604b4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c604b98u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604b4c; /* mov.l 0x8c604b98,r4 */
L8c604b4c: target=r[3];*pr=0x8c604b50u;*stop=0x8c604b4eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604b50; /* jsr @r3 */
L8c604b50: *stop=0x8c604b50u;r[15]+=12u;goto L8c604b52; /* add #0xc,r15 */
L8c604b52: *stop=0x8c604b52u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c604b54; /* lds.l @r15+,PR */
L8c604b54: *stop=0x8c604b54u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c604b56; /* fmov.s @r15+,fr13 */
L8c604b56: *stop=0x8c604b56u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c604b58; /* fmov.s @r15+,fr14 */
L8c604b58: *stop=0x8c604b58u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c604b5a; /* fmov.s @r15+,fr15 */
L8c604b5a: *stop=0x8c604b5au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c604b5c; /* mov.l @r15+,r11 */
L8c604b5c: *stop=0x8c604b5cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c604b5e; /* mov.l @r15+,r12 */
L8c604b5e: *stop=0x8c604b5eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c604b60; /* mov.l @r15+,r13 */
L8c604b60: *stop=0x8c604b62u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,0x8c604d68u,m,f,sr,r,pr); /* bra 0x8c604d68 */
}

SA_STAGE_EXPORT int sa_movie_main_8c604dce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c604dce;
L8c604dce: *stop=0x8c604dceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c604dd0; /* mov.l r14,@-r15 */
L8c604dd0: *stop=0x8c604dd0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c604dd2; /* mov.l r13,@-r15 */
L8c604dd2: *stop=0x8c604dd2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c604dd4; /* mov.l r12,@-r15 */
L8c604dd4: *stop=0x8c604dd4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c604dd6; /* mov.l r11,@-r15 */
L8c604dd6: *stop=0x8c604dd6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c604dd8; /* mov.l r10,@-r15 */
L8c604dd8: *stop=0x8c604dd8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c604dda; /* mov.l r9,@-r15 */
L8c604dda: *stop=0x8c604ddau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c604ddc; /* mov.l r8,@-r15 */
L8c604ddc: *stop=0x8c604ddcu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c604dde; /* fmov.s fr15,@-r15 */
L8c604dde: *stop=0x8c604ddeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c604de0; /* sts.l PR,@-r15 */
L8c604de0: *stop=0x8c604de0u;r[15]+=4294967288u;goto L8c604de2; /* add #-0x8,r15 */
L8c604de2: *stop=0x8c604de2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c604de4; /* mov.l r5,@r15 */
L8c604de4: *stop=0x8c604de4u;r[8]=r[7];goto L8c604de6; /* mov r7,r8 */
L8c604de6: *stop=0x8c604de6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604e50u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c604de8; /* mov.l 0x8c604e50,r5 */
L8c604de8: *stop=0x8c604de8u;r[9]=0u;goto L8c604dea; /* mov #0x0,r9 */
L8c604dea: *stop=0x8c604deau;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c604dec; /* mov.l @(0x2c,r15),r11 */
L8c604dec: *stop=0x8c604decu;r[12]=r[4];goto L8c604dee; /* mov r4,r12 */
L8c604dee: *stop=0x8c604deeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604e54u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c604df0; /* mov.l 0x8c604e54,r7 */
L8c604df0: *stop=0x8c604df0u;r[10]=r[6];goto L8c604df2; /* mov r6,r10 */
L8c604df2: *stop=0x8c604df2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604e4cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c604df4; /* mov.l 0x8c604e4c,r14 */
L8c604df4: *stop=0x8c604df4u;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c604df6; /* cmp/pz r12 */
L8c604df6: *stop=0x8c604df6u;r[6]=1u;goto L8c604df8; /* mov #0x1,r6 */
L8c604df8: *stop=0x8c604df8u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[9],4);if(io_status)return io_status;goto L8c604dfa; /* mov.l r9,@r5 */
L8c604dfa: take=(*sr&1u);*stop=0x8c604dfcu;r[4]=100u;if(take)goto L8c604e0c;goto L8c604dfe; /* bt/s 0x8c604e0c */
L8c604dfe: *stop=0x8c604dfeu;r[0]=92u;goto L8c604e00; /* mov #0x5c,r0 */
L8c604e00: *stop=0x8c604e00u;r[12]=r[4];goto L8c604e02; /* mov r4,r12 */
L8c604e02: *stop=0x8c604e02u;r[2]=32u;goto L8c604e04; /* mov #0x20,r2 */
L8c604e04: *stop=0x8c604e04u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[7],4);if(io_status)return io_status;goto L8c604e06; /* mov.l r7,@(0x4,r14) */
L8c604e06: *stop=0x8c604e06u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c604e08; /* mov.l r2,@(r0,r14) */
L8c604e08: *stop=0x8c604e0au;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[6],4);if(io_status)return io_status;goto L8c604e60; /* bra 0x8c604e60 */
L8c604e0c: *stop=0x8c604e0cu;*sr=(*sr&~1u)|((int32_t)r[10]>=0);goto L8c604e0e; /* cmp/pz r10 */
L8c604e0e: if((*sr&1u))goto L8c604e1c;goto L8c604e10; /* bt 0x8c604e1c */
L8c604e10: *stop=0x8c604e10u;r[0]=92u;goto L8c604e12; /* mov #0x5c,r0 */
L8c604e12: *stop=0x8c604e12u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[7],4);if(io_status)return io_status;goto L8c604e14; /* mov.l r7,@(0x4,r14) */
L8c604e14: *stop=0x8c604e14u;r[3]=34u;goto L8c604e16; /* mov #0x22,r3 */
L8c604e16: *stop=0x8c604e16u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c604e18; /* mov.l r3,@(r0,r14) */
L8c604e18: *stop=0x8c604e1au;r[10]=r[4];goto L8c604e2a; /* bra 0x8c604e2a */
L8c604e1c: *stop=0x8c604e1cu;*sr=(*sr&~1u)|((int32_t)r[11]>=0);goto L8c604e1e; /* cmp/pz r11 */
L8c604e1e: if((*sr&1u))goto L8c604e58;goto L8c604e20; /* bt 0x8c604e58 */
L8c604e20: *stop=0x8c604e20u;r[0]=92u;goto L8c604e22; /* mov #0x5c,r0 */
L8c604e22: *stop=0x8c604e22u;r[11]=r[4];goto L8c604e24; /* mov r4,r11 */
L8c604e24: *stop=0x8c604e24u;r[2]=36u;goto L8c604e26; /* mov #0x24,r2 */
L8c604e26: *stop=0x8c604e26u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[6],4);if(io_status)return io_status;goto L8c604e28; /* mov.l r6,@(0x4,r14) */
L8c604e28: *stop=0x8c604e28u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c604e2a; /* mov.l r2,@(r0,r14) */
L8c604e2a: *stop=0x8c604e2au;r[3]=2u;goto L8c604e2c; /* mov #0x2,r3 */
L8c604e2c: *stop=0x8c604e2eu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c604e60; /* bra 0x8c604e60 */
L8c604e58: *stop=0x8c604e58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c604e5a; /* mov.l 0x8c604f1c,r1 */
L8c604e5a: *stop=0x8c604e5au;r[0]=92u;goto L8c604e5c; /* mov #0x5c,r0 */
L8c604e5c: *stop=0x8c604e5cu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[1],4);if(io_status)return io_status;goto L8c604e5e; /* mov.l r1,@(0x4,r14) */
L8c604e5e: *stop=0x8c604e5eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[9],4);if(io_status)return io_status;goto L8c604e60; /* mov.l r9,@(r0,r14) */
L8c604e60: *stop=0x8c604e60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c604e62; /* mov.l 0x8c604f24,r1 */
L8c604e62: *stop=0x8c604e62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f34u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c604e64; /* mov.l 0x8c604f34,r13 */
L8c604e64: *stop=0x8c604e64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604e66; /* mov.l 0x8c604f20,r3 */
L8c604e66: *stop=0x8c604e66u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8c604e68; /* mov.l r12,@r3 */
L8c604e68: *stop=0x8c604e68u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604e6a; /* mov.l @r15,r2 */
L8c604e6a: *stop=0x8c604e6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604e6c; /* mov.l 0x8c604f2c,r3 */
L8c604e6c: *stop=0x8c604e6cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c604e6e; /* mov.l r2,@r1 */
L8c604e6e: *stop=0x8c604e6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604e70; /* mov.l 0x8c604f28,r2 */
L8c604e70: *stop=0x8c604e70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f30u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c604e72; /* mov.l 0x8c604f30,r1 */
L8c604e72: *stop=0x8c604e72u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[10],4);if(io_status)return io_status;goto L8c604e74; /* mov.l r10,@r2 */
L8c604e74: *stop=0x8c604e74u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[8],4);if(io_status)return io_status;goto L8c604e76; /* mov.l r8,@r3 */
L8c604e76: *stop=0x8c604e76u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[11],4);if(io_status)return io_status;goto L8c604e78; /* mov.l r11,@r1 */
L8c604e78: *stop=0x8c604e78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604e7a; /* mov.l @r15,r2 */
L8c604e7a: *stop=0x8c604e7au;r[2]+=r[12];goto L8c604e7c; /* add r12,r2 */
L8c604e7c: *stop=0x8c604e7cu;r[2]+=r[10];goto L8c604e7e; /* add r10,r2 */
L8c604e7e: *stop=0x8c604e7eu;r[2]+=r[8];goto L8c604e80; /* add r8,r2 */
L8c604e80: *stop=0x8c604e80u;r[2]+=r[11];goto L8c604e82; /* add r11,r2 */
L8c604e82: *stop=0x8c604e82u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c604e84; /* mov.l r2,@r13 */
L8c604e84: *stop=0x8c604e84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604e86; /* mov.l 0x8c604f38,r2 */
L8c604e86: *stop=0x8c604e86u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto L8c604e88; /* mov.l @r2,r4 */
L8c604e88: *stop=0x8c604e88u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c604e8a; /* tst r4,r4 */
L8c604e8a: if(!(*sr&1u))goto L8c604ece;goto L8c604e8c; /* bf 0x8c604ece */
L8c604e8c: *stop=0x8c604e8cu;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c604e8e; /* mov.l @(0x24,r14),r0 */
L8c604e8e: *stop=0x8c604e8eu;r[3]=r[0];goto L8c604e90; /* mov r0,r3 */
L8c604e90: *stop=0x8c604e90u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c604e92; /* tst r3,r3 */
L8c604e92: *stop=0x8c604e92u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c604e94; /* mov.l r0,@(0x4,r15) */
L8c604e94: if((*sr&1u))goto L8c604ea0;goto L8c604e96; /* bt 0x8c604ea0 */
L8c604e96: *stop=0x8c604e96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f40u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c604e98; /* mov.l 0x8c604f40,r1 */
L8c604e98: *stop=0x8c604e98u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604e9a; /* mov.l @(0x4,r15),r4 */
L8c604e9a: *stop=0x8c604e9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604e9c; /* mov.l 0x8c604f3c,r3 */
L8c604e9c: target=r[1];*pr=0x8c604ea0u;*stop=0x8c604e9eu;r[4]&=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604ea0; /* jsr @r1 */
L8c604ea0: *stop=0x8c604ea0u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c604ea2; /* mov.l @r13,r3 */
L8c604ea2: *stop=0x8c604ea2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c604ea4; /* fldi0 fr2 */
L8c604ea4: *stop=0x8c604ea4u;*fpul=r[3];goto L8c604ea6; /* lds r3,FPUL */
L8c604ea6: *stop=0x8c604ea6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c604ea8; /* float FPUL,fr3 */
L8c604ea8: *stop=0x8c604ea8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c604eaa; /* fcmp/gt fr3,fr2 */
L8c604eaa: take=!(*sr&1u);*stop=0x8c604eacu;fcopy(f,4u,3u);if(take)goto L8c604eb8;goto L8c604eae; /* bf/s 0x8c604eb8 */
L8c604eae: *stop=0x8c604eaeu;r[0]=0x8c604f44u;goto L8c604eb0; /* mova 0x8c604f44,r0 */
L8c604eb0: *stop=0x8c604eb0u;fcopy(f,15u,4u);goto L8c604eb2; /* fmov fr4,fr15 */
L8c604eb2: *stop=0x8c604eb2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c604eb4; /* fmov.s @r0,fr3 */
L8c604eb4: *stop=0x8c604eb6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c604eba; /* bra 0x8c604eba */
L8c604eb8: *stop=0x8c604eb8u;fcopy(f,15u,4u);goto L8c604eba; /* fmov fr4,fr15 */
L8c604eba: *stop=0x8c604ebau;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}goto L8c604ebc; /* mov.l @r13,r4 */
L8c604ebc: *stop=0x8c604ebcu;r[3]=sx(m,bus,&io_status,0x8c604f1au,2);if(io_status)return io_status;goto L8c604ebe; /* mov.w 0x8c604f1a,r3 */
L8c604ebe: *stop=0x8c604ebeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604ec0; /* mov.l 0x8c604f48,r2 */
L8c604ec0: *stop=0x8c604ec0u;r[4]<<=2;goto L8c604ec2; /* shll2 r4 */
L8c604ec2: target=r[2];*pr=0x8c604ec6u;*stop=0x8c604ec4u;r[4]+=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604ec6; /* jsr @r2 */
L8c604ec6: *stop=0x8c604ec6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f4cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604ec8; /* mov.l 0x8c604f4c,r3 */
L8c604ec8: *stop=0x8c604ec8u;r[0]|=r[3];goto L8c604eca; /* or r3,r0 */
L8c604eca: *stop=0x8c604eccu;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[0],4);if(io_status)return io_status;goto L8c604efa; /* bra 0x8c604efa */
L8c604ece: *stop=0x8c604eceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604f50u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604ed0; /* mov.l 0x8c604f50,r2 */
L8c604ed0: *stop=0x8c604ed0u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c604ed2; /* mov.l @r13,r3 */
L8c604ed2: *stop=0x8c604ed2u;r[0]=sx(m,bus,&io_status,0x8c604f1au,2);if(io_status)return io_status;goto L8c604ed4; /* mov.w 0x8c604f1a,r0 */
L8c604ed4: *stop=0x8c604ed4u;r[3]<<=2;goto L8c604ed6; /* shll2 r3 */
L8c604ed6: *stop=0x8c604ed6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c604ed8; /* mov.l @r2,r1 */
L8c604ed8: *stop=0x8c604ed8u;r[3]+=r[0];goto L8c604eda; /* add r0,r3 */
L8c604eda: *stop=0x8c604edau;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c604edc; /* cmp/hs r3,r1 */
L8c604edc: if(!(*sr&1u))goto L8c604f02;goto L8c604ede; /* bf 0x8c604f02 */
L8c604ede: *stop=0x8c604edeu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c604ee0; /* mov.l @r13,r3 */
L8c604ee0: *stop=0x8c604ee0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c604ee2; /* fldi0 fr2 */
L8c604ee2: *stop=0x8c604ee2u;*fpul=r[3];goto L8c604ee4; /* lds r3,FPUL */
L8c604ee4: *stop=0x8c604ee4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c604ee6; /* float FPUL,fr3 */
L8c604ee6: *stop=0x8c604ee6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c604ee8; /* fcmp/gt fr3,fr2 */
L8c604ee8: take=!(*sr&1u);*stop=0x8c604eeau;fcopy(f,4u,3u);if(take)goto L8c604ef6;goto L8c604eec; /* bf/s 0x8c604ef6 */
L8c604eec: *stop=0x8c604eecu;r[0]=0x8c604f44u;goto L8c604eee; /* mova 0x8c604f44,r0 */
L8c604eee: *stop=0x8c604eeeu;fcopy(f,15u,4u);goto L8c604ef0; /* fmov fr4,fr15 */
L8c604ef0: *stop=0x8c604ef0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c604ef2; /* fmov.s @r0,fr3 */
L8c604ef2: *stop=0x8c604ef4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c604ef8; /* bra 0x8c604ef8 */
L8c604ef6: *stop=0x8c604ef6u;fcopy(f,15u,4u);goto L8c604ef8; /* fmov fr4,fr15 */
L8c604ef8: *stop=0x8c604ef8u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[4],4);if(io_status)return io_status;goto L8c604efa; /* mov.l r4,@(0x24,r14) */
L8c604efa: *stop=0x8c604efau;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604efc; /* mov.l @r13,r2 */
L8c604efc: *stop=0x8c604efcu;r[2]+=88u;goto L8c604efe; /* add #0x58,r2 */
L8c604efe: *stop=0x8c604f00u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[2],4);if(io_status)return io_status;goto L8c604f5a; /* bra 0x8c604f5a */
L8c604f02: *stop=0x8c604f02u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[1]=loaded;}goto L8c604f04; /* mov.l @r13,r1 */
L8c604f04: *stop=0x8c604f04u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c604f06; /* fldi0 fr2 */
L8c604f06: *stop=0x8c604f06u;*fpul=r[1];goto L8c604f08; /* lds r1,FPUL */
L8c604f08: *stop=0x8c604f08u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c604f0a; /* float FPUL,fr3 */
L8c604f0a: *stop=0x8c604f0au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c604f0c; /* fcmp/gt fr3,fr2 */
L8c604f0c: take=!(*sr&1u);*stop=0x8c604f0eu;fcopy(f,4u,3u);if(take)goto L8c604f54;goto L8c604f10; /* bf/s 0x8c604f54 */
L8c604f10: *stop=0x8c604f10u;r[0]=0x8c604f44u;goto L8c604f12; /* mova 0x8c604f44,r0 */
L8c604f12: *stop=0x8c604f12u;fcopy(f,15u,4u);goto L8c604f14; /* fmov fr4,fr15 */
L8c604f14: *stop=0x8c604f14u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c604f16; /* fmov.s @r0,fr3 */
L8c604f16: *stop=0x8c604f18u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c604f56; /* bra 0x8c604f56 */
L8c604f54: *stop=0x8c604f54u;fcopy(f,15u,4u);goto L8c604f56; /* fmov fr4,fr15 */
L8c604f56: *stop=0x8c604f56u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[9],4);if(io_status)return io_status;goto L8c604f58; /* mov.l r9,@(0x24,r14) */
L8c604f58: *stop=0x8c604f58u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[9],4);if(io_status)return io_status;goto L8c604f5a; /* mov.l r9,@(0x28,r14) */
L8c604f5a: *stop=0x8c604f5au;*fpul=r[12];goto L8c604f5c; /* lds r12,FPUL */
L8c604f5c: *stop=0x8c604f5cu;r[0]=0x8c6051a8u;goto L8c604f5e; /* mova 0x8c6051a8,r0 */
L8c604f5e: *stop=0x8c604f5eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c604f60; /* fmov.s @r0,fr4 */
L8c604f60: *stop=0x8c604f60u;r[0]=44u;goto L8c604f62; /* mov #0x2c,r0 */
L8c604f62: *stop=0x8c604f62u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c604f64; /* mov.l @r15,r3 */
L8c604f64: *stop=0x8c604f64u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c604f66; /* float FPUL,fr3 */
L8c604f66: *stop=0x8c604f66u;*fpul=r[3];goto L8c604f68; /* lds r3,FPUL */
L8c604f68: *stop=0x8c604f68u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c604f6a; /* fmul fr4,fr3 */
L8c604f6a: *stop=0x8c604f6au;fcopy(f,8u,3u);goto L8c604f6c; /* fmov fr3,fr8 */
L8c604f6c: *stop=0x8c604f6cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c604f6e; /* float FPUL,fr3 */
L8c604f6e: *stop=0x8c604f6eu;*fpul=r[10];goto L8c604f70; /* lds r10,FPUL */
L8c604f70: *stop=0x8c604f70u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[8],&fr[15],&f->fpscr,&fr[8]);if(status)return status;}goto L8c604f72; /* fdiv fr15,fr8 */
L8c604f72: *stop=0x8c604f72u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c604f74; /* fmul fr4,fr3 */
L8c604f74: *stop=0x8c604f74u;fcopy(f,7u,3u);goto L8c604f76; /* fmov fr3,fr7 */
L8c604f76: *stop=0x8c604f76u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c604f78; /* float FPUL,fr3 */
L8c604f78: *stop=0x8c604f78u;*fpul=r[8];goto L8c604f7a; /* lds r8,FPUL */
L8c604f7a: *stop=0x8c604f7au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[15],&f->fpscr,&fr[7]);if(status)return status;}goto L8c604f7c; /* fdiv fr15,fr7 */
L8c604f7c: *stop=0x8c604f7cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c604f7e; /* float FPUL,fr2 */
L8c604f7e: *stop=0x8c604f7eu;*fpul=r[11];goto L8c604f80; /* lds r11,FPUL */
L8c604f80: *stop=0x8c604f80u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c604f82; /* fmul fr4,fr3 */
L8c604f82: *stop=0x8c604f82u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,8u,r[0]+r[14]);if(io_status)return io_status;goto L8c604f84; /* fmov fr8,@(r0,r14) */
L8c604f84: *stop=0x8c604f84u;r[0]=48u;goto L8c604f86; /* mov #0x30,r0 */
L8c604f86: *stop=0x8c604f86u;fcopy(f,6u,3u);goto L8c604f88; /* fmov fr3,fr6 */
L8c604f88: *stop=0x8c604f88u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c604f8a; /* float FPUL,fr3 */
L8c604f8a: *stop=0x8c604f8au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c604f8c; /* fmul fr4,fr2 */
L8c604f8c: *stop=0x8c604f8cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[15],&f->fpscr,&fr[6]);if(status)return status;}goto L8c604f8e; /* fdiv fr15,fr6 */
L8c604f8e: *stop=0x8c604f8eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c604f90; /* fmul fr4,fr3 */
L8c604f90: *stop=0x8c604f90u;fcopy(f,5u,2u);goto L8c604f92; /* fmov fr2,fr5 */
L8c604f92: *stop=0x8c604f92u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[15],&f->fpscr,&fr[5]);if(status)return status;}goto L8c604f94; /* fdiv fr15,fr5 */
L8c604f94: *stop=0x8c604f94u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,7u,r[0]+r[14]);if(io_status)return io_status;goto L8c604f96; /* fmov fr7,@(r0,r14) */
L8c604f96: *stop=0x8c604f96u;fcopy(f,4u,3u);goto L8c604f98; /* fmov fr3,fr4 */
L8c604f98: *stop=0x8c604f98u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c604f9a; /* fdiv fr15,fr4 */
L8c604f9a: *stop=0x8c604f9au;r[0]=52u;goto L8c604f9c; /* mov #0x34,r0 */
L8c604f9c: *stop=0x8c604f9cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,6u,r[0]+r[14]);if(io_status)return io_status;goto L8c604f9e; /* fmov fr6,@(r0,r14) */
L8c604f9e: *stop=0x8c604f9eu;r[0]=56u;goto L8c604fa0; /* mov #0x38,r0 */
L8c604fa0: *stop=0x8c604fa0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c604fa2; /* fmov fr5,@(r0,r14) */
L8c604fa2: *stop=0x8c604fa2u;r[0]=60u;goto L8c604fa4; /* mov #0x3c,r0 */
L8c604fa4: target=0x8c604594u;*pr=0x8c604fa8u;*stop=0x8c604fa6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604fa8; /* bsr 0x8c604594 */
L8c604fa8: *stop=0x8c604fa8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604faa; /* mov.l 0x8c6051b0,r2 */
L8c604faa: *stop=0x8c604faau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051acu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604fac; /* mov.l 0x8c6051ac,r4 */
L8c604fac: target=r[2];*pr=0x8c604fb0u;*stop=0x8c604faeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604fb0; /* jsr @r2 */
L8c604fb0: *stop=0x8c604fb0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051b4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604fb2; /* mov.l 0x8c6051b4,r3 */
L8c604fb2: target=r[3];*pr=0x8c604fb6u;*stop=0x8c604fb4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604fb6; /* jsr @r3 */
L8c604fb6: *stop=0x8c604fb6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604fb8; /* mov.l 0x8c6051b8,r2 */
L8c604fb8: *stop=0x8c604fb8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c604fba; /* mov.l r0,@r2 */
L8c604fba: *stop=0x8c604fbau;r[15]+=8u;goto L8c604fbc; /* add #0x8,r15 */
L8c604fbc: *stop=0x8c604fbcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c604fbe; /* lds.l @r15+,PR */
L8c604fbe: *stop=0x8c604fbeu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c604fc0; /* fmov.s @r15+,fr15 */
L8c604fc0: *stop=0x8c604fc0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c604fc2; /* mov.l @r15+,r8 */
L8c604fc2: *stop=0x8c604fc2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c604fc4; /* mov.l @r15+,r9 */
L8c604fc4: *stop=0x8c604fc4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c604fc6; /* mov.l @r15+,r10 */
L8c604fc6: *stop=0x8c604fc6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c604fc8; /* mov.l @r15+,r11 */
L8c604fc8: *stop=0x8c604fc8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c604fca; /* mov.l @r15+,r12 */
L8c604fca: *stop=0x8c604fcau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c604fcc; /* mov.l @r15+,r13 */
L8c604fcc: *stop=0x8c604fccu;if(*pr!=entry_pr)return -3;*stop=0x8c604fceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c6406ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6406ac;
L8c6406ac: *stop=0x8c6406acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6406ae; /* mov.l r14,@-r15 */
L8c6406ae: *stop=0x8c6406aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6406b0; /* sts.l PR,@-r15 */
L8c6406b0: *stop=0x8c6406b0u;r[15]+=4294967284u;goto L8c6406b2; /* add #-0xc,r15 */
L8c6406b2: *stop=0x8c6406b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640774u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6406b4; /* mov.l 0x8c640774,r2 */
L8c6406b4: *stop=0x8c6406b4u;r[0]=0x8c640778u;goto L8c6406b6; /* mova 0x8c640778,r0 */
L8c6406b6: *stop=0x8c6406b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640770u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6406b8; /* mov.l 0x8c640770,r3 */
L8c6406b8: *stop=0x8c6406b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64077cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6406ba; /* mov.l 0x8c64077c,r1 */
L8c6406ba: *stop=0x8c6406bau;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c6406bc; /* mov.l r4,@(0x4,r15) */
L8c6406bc: *stop=0x8c6406bcu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c6406be; /* mov.l r5,@r15 */
L8c6406be: *stop=0x8c6406beu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6406c0; /* mov.l r6,@(0x8,r15) */
L8c6406c0: *stop=0x8c6406c0u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6406c2; /* mov.l r3,@r2 */
L8c6406c2: *stop=0x8c6406c2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c6406c4; /* fmov.s @r0,fr3 */
L8c6406c4: *stop=0x8c6406c4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6406c6; /* fmov.s fr3,@r1 */
L8c6406c6: *stop=0x8c6406c6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6406c8; /* mov.l @(0x4,r15),r3 */
L8c6406c8: *stop=0x8c6406c8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6406ca; /* tst r3,r3 */
L8c6406ca: if(!(*sr&1u))goto L8c6406ea;goto L8c6406cc; /* bf 0x8c6406ea */
L8c6406cc: *stop=0x8c6406ccu;r[7]=sx(m,bus,&io_status,0x8c64076cu,2);if(io_status)return io_status;goto L8c6406ce; /* mov.w 0x8c64076c,r7 */
L8c6406ce: *stop=0x8c6406ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c640780u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6406d0; /* mov.l 0x8c640780,r4 */
L8c6406d0: *stop=0x8c6406d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640784u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6406d2; /* mov.l 0x8c640784,r3 */
L8c6406d2: *stop=0x8c6406d2u;r[6]=r[7];goto L8c6406d4; /* mov r7,r6 */
L8c6406d4: *stop=0x8c6406d4u;r[6]+=4294967168u;goto L8c6406d6; /* add #-0x80,r6 */
L8c6406d6: target=r[3];*pr=0x8c6406dau;*stop=0x8c6406d8u;r[5]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6406da; /* jsr @r3 */
L8c6406da: *stop=0x8c6406dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c640780u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6406dc; /* mov.l 0x8c640780,r4 */
L8c6406dc: *stop=0x8c6406dcu;r[7]=0u;goto L8c6406de; /* mov #0x0,r7 */
L8c6406de: *stop=0x8c6406deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64078cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6406e0; /* mov.l 0x8c64078c,r2 */
L8c6406e0: *stop=0x8c6406e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640788u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6406e2; /* mov.l 0x8c640788,r5 */
L8c6406e2: target=r[2];*pr=0x8c6406e6u;*stop=0x8c6406e4u;r[6]=r[7];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6406e6; /* jsr @r2 */
L8c6406e6: *stop=0x8c6406e8u;goto L8c640702; /* bra 0x8c640702 */
L8c6406ea: *stop=0x8c6406eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c640780u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6406ec; /* mov.l 0x8c640780,r4 */
L8c6406ec: *stop=0x8c6406ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c640784u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6406ee; /* mov.l 0x8c640784,r3 */
L8c6406ee: *stop=0x8c6406eeu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6406f0; /* mov.l @(0x8,r15),r7 */
L8c6406f0: *stop=0x8c6406f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6406f2; /* mov.l @r15,r6 */
L8c6406f2: target=r[3];*pr=0x8c6406f6u;*stop=0x8c6406f4u;r[5]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6406f6; /* jsr @r3 */
L8c6406f6: *stop=0x8c6406f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640780u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6406f8; /* mov.l 0x8c640780,r4 */
L8c6406f8: *stop=0x8c6406f8u;r[7]=0u;goto L8c6406fa; /* mov #0x0,r7 */
L8c6406fa: *stop=0x8c6406fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64078cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6406fc; /* mov.l 0x8c64078c,r3 */
L8c6406fc: *stop=0x8c6406fcu;r[6]=r[7];goto L8c6406fe; /* mov r7,r6 */
L8c6406fe: target=r[3];*pr=0x8c640702u;*stop=0x8c640700u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c640702; /* jsr @r3 */
L8c640702: *stop=0x8c640702u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640780u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c640704; /* mov.l 0x8c640780,r3 */
L8c640704: *stop=0x8c640704u;r[0]=108u;goto L8c640706; /* mov #0x6c,r0 */
L8c640706: *stop=0x8c640706u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640790u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c640708; /* mov.l 0x8c640790,r14 */
L8c640708: *stop=0x8c640708u;r[4]=0u;goto L8c64070a; /* mov #0x0,r4 */
L8c64070a: *stop=0x8c64070au;{uint32_t loaded=rd(m,bus,&io_status,0x8c640794u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64070c; /* mov.l 0x8c640794,r2 */
L8c64070c: *stop=0x8c64070cu;r[5]=2u;goto L8c64070e; /* mov #0x2,r5 */
L8c64070e: *stop=0x8c64070eu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c640710; /* mov.l r2,@r14 */
L8c640710: *stop=0x8c640710u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[4],4);if(io_status)return io_status;goto L8c640712; /* mov.l r4,@(0x4,r14) */
L8c640712: *stop=0x8c640712u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[5],4);if(io_status)return io_status;goto L8c640714; /* mov.l r5,@(0x8,r14) */
L8c640714: *stop=0x8c640714u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[4],4);if(io_status)return io_status;goto L8c640716; /* mov.l r4,@(0xc,r14) */
L8c640716: *stop=0x8c640716u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[5],4);if(io_status)return io_status;goto L8c640718; /* mov.l r5,@(0x20,r14) */
L8c640718: *stop=0x8c640718u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c64071a; /* mov.l r3,@(r0,r14) */
L8c64071a: *stop=0x8c64071au;r[0]=64u;goto L8c64071c; /* mov #0x40,r0 */
L8c64071c: *stop=0x8c64071cu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c64071e; /* mov.l r4,@(r0,r14) */
L8c64071e: *stop=0x8c64071eu;r[3]=1u;goto L8c640720; /* mov #0x1,r3 */
L8c640720: *stop=0x8c640720u;r[0]=68u;goto L8c640722; /* mov #0x44,r0 */
L8c640722: *stop=0x8c640722u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c640724; /* mov.l r3,@(r0,r14) */
L8c640724: *stop=0x8c640724u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640798u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c640726; /* mov.l 0x8c640798,r3 */
L8c640726: target=r[3];*pr=0x8c64072au;*stop=0x8c640728u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64072a; /* jsr @r3 */
L8c64072a: *stop=0x8c64072au;r[0]=sx(m,bus,&io_status,0x8c64076eu,2);if(io_status)return io_status;goto L8c64072c; /* mov.w 0x8c64076e,r0 */
L8c64072c: *stop=0x8c64072cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64079cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64072e; /* mov.l 0x8c64079c,r3 */
L8c64072e: *stop=0x8c64072eu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c640730; /* mov.l @(r0,r14),r2 */
L8c640730: *stop=0x8c640730u;r[0]+=4u;goto L8c640732; /* add #0x4,r0 */
L8c640732: *stop=0x8c640732u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c640734; /* mov.l r2,@r3 */
L8c640734: *stop=0x8c640734u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6407a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c640736; /* mov.l 0x8c6407a0,r2 */
L8c640736: *stop=0x8c640736u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c640738; /* mov.l @(r0,r14),r1 */
L8c640738: *stop=0x8c640738u;r[0]+=4u;goto L8c64073a; /* add #0x4,r0 */
L8c64073a: *stop=0x8c64073au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c64073c; /* mov.l r1,@r2 */
L8c64073c: *stop=0x8c64073cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6407a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64073e; /* mov.l 0x8c6407a4,r1 */
L8c64073e: *stop=0x8c64073eu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640740; /* mov.l @(r0,r14),r3 */
L8c640740: *stop=0x8c640740u;r[0]+=4u;goto L8c640742; /* add #0x4,r0 */
L8c640742: *stop=0x8c640742u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c640744; /* mov.l r3,@r1 */
L8c640744: *stop=0x8c640744u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6407a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c640746; /* mov.l 0x8c6407a8,r3 */
L8c640746: *stop=0x8c640746u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c640748; /* mov.l @(r0,r14),r2 */
L8c640748: *stop=0x8c640748u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64074a; /* mov.l r2,@r3 */
L8c64074a: *stop=0x8c64074au;r[15]+=12u;goto L8c64074c; /* add #0xc,r15 */
L8c64074c: *stop=0x8c64074cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64074e; /* lds.l @r15+,PR */
L8c64074e: *stop=0x8c64074eu;if(*pr!=entry_pr)return -3;*stop=0x8c640750u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c041e60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c041e60;
L8c041e60: *stop=0x8c041e60u;r[0]=0x8c041ec8u;goto L8c041e62; /* mova 0x8c041ec8,r0 */
L8c041e62: *stop=0x8c041e62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c041ec0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c041e64; /* mov.l 0x8c041ec0,r3 */
L8c041e64: *stop=0x8c041e64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c041ec4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c041e66; /* mov.l 0x8c041ec4,r2 */
L8c041e66: *stop=0x8c041e66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c041ed0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c041e68; /* mov.l 0x8c041ed0,r4 */
L8c041e68: *stop=0x8c041e68u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c041e6a; /* mov.l r3,@r2 */
L8c041e6a: *stop=0x8c041e6au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c041e6c; /* fmov.s @r0,fr3 */
L8c041e6c: *stop=0x8c041e6cu;r[0]=4294967184u;goto L8c041e6e; /* mov #-0x70,r0 */
L8c041e6e: *stop=0x8c041e6eu;r[0]=r[0]&255u;goto L8c041e70; /* extu.b r0,r0 */
L8c041e70: *stop=0x8c041e70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c041ed4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c041e72; /* mov.l 0x8c041ed4,r2 */
L8c041e72: *stop=0x8c041e72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c041eccu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c041e74; /* mov.l 0x8c041ecc,r1 */
L8c041e74: *stop=0x8c041e74u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c041e76; /* fmov.s fr3,@r1 */
L8c041e76: *stop=0x8c041e76u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c041e78; /* mov.l @(r0,r4),r3 */
L8c041e78: *stop=0x8c041e78u;r[0]+=4u;goto L8c041e7a; /* add #0x4,r0 */
L8c041e7a: *stop=0x8c041e7au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c041e7c; /* mov.l r3,@r2 */
L8c041e7c: *stop=0x8c041e7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c041ed8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c041e7e; /* mov.l 0x8c041ed8,r3 */
L8c041e7e: *stop=0x8c041e7eu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c041e80; /* mov.l @(r0,r4),r1 */
L8c041e80: *stop=0x8c041e80u;r[0]+=4u;goto L8c041e82; /* add #0x4,r0 */
L8c041e82: *stop=0x8c041e82u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c041e84; /* mov.l r1,@r3 */
L8c041e84: *stop=0x8c041e84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c041edcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c041e86; /* mov.l 0x8c041edc,r1 */
L8c041e86: *stop=0x8c041e86u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c041e88; /* mov.l @(r0,r4),r2 */
L8c041e88: *stop=0x8c041e88u;r[0]+=4u;goto L8c041e8a; /* add #0x4,r0 */
L8c041e8a: *stop=0x8c041e8au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c041e8c; /* mov.l r2,@r1 */
L8c041e8c: *stop=0x8c041e8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c041ee0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c041e8e; /* mov.l 0x8c041ee0,r2 */
L8c041e8e: *stop=0x8c041e8eu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c041e90; /* mov.l @(r0,r4),r3 */
L8c041e90: *stop=0x8c041e90u;if(*pr!=entry_pr)return -3;*stop=0x8c041e92u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c653d68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c653d68;
L8c653d68: *stop=0x8c653d68u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c653d6a; /* mov.l r14,@-r15 */
L8c653d6a: *stop=0x8c653d6au;r[14]=r[4];goto L8c653d6c; /* mov r4,r14 */
L8c653d6c: *stop=0x8c653d6cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c653d6e; /* sts.l PR,@-r15 */
L8c653d6e: *stop=0x8c653d6eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c653d70; /* mov.l @r14,r0 */
L8c653d70: *stop=0x8c653d70u;r[0]=r[0]&65535u;goto L8c653d72; /* extu.w r0,r0 */
L8c653d72: *stop=0x8c653d72u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c653d74; /* cmp/eq #0x3,r0 */
L8c653d74: if(!(*sr&1u))goto L8c653d80;goto L8c653d76; /* bf 0x8c653d80 */
L8c653d76: *stop=0x8c653d76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653dacu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c653d78; /* mov.l 0x8c653dac,r2 */
L8c653d78: target=r[2];*pr=0x8c653d7cu;*stop=0x8c653d7au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653d7c; /* jsr @r2 */
L8c653d7c: *stop=0x8c653d7eu;goto L8c653d86; /* bra 0x8c653d86 */
L8c653d80: *stop=0x8c653d80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653db0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c653d82; /* mov.l 0x8c653db0,r2 */
L8c653d82: target=r[2];*pr=0x8c653d86u;*stop=0x8c653d84u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653d86; /* jsr @r2 */
L8c653d86: *stop=0x8c653d86u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c653d88; /* lds.l @r15+,PR */
L8c653d88: *stop=0x8c653d88u;if(*pr!=entry_pr)return -3;*stop=0x8c653d8au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c64e720(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64e720;
L8c64e720: *stop=0x8c64e720u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e810u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e722; /* mov.l 0x8c64e810,r2 */
L8c64e722: *stop=0x8c64e722u;r[5]=4294967295u;goto L8c64e724; /* mov #-0x1,r5 */
L8c64e724: *stop=0x8c64e724u;r[0]=64u;goto L8c64e726; /* mov #0x40,r0 */
L8c64e726: *stop=0x8c64e726u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64e728; /* sts.l PR,@-r15 */
L8c64e728: *stop=0x8c64e728u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c64e72a; /* mov.l r5,@r4 */
L8c64e72a: *stop=0x8c64e72au;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c64e72c; /* mov.l r5,@(0x4,r4) */
L8c64e72c: *stop=0x8c64e72cu;r[5]=0u;goto L8c64e72e; /* mov #0x0,r5 */
L8c64e72e: *stop=0x8c64e72eu;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[5],4);if(io_status)return io_status;goto L8c64e730; /* mov.l r5,@(0x14,r4) */
L8c64e730: *stop=0x8c64e730u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[5],4);if(io_status)return io_status;goto L8c64e732; /* mov.l r5,@(0x18,r4) */
L8c64e732: *stop=0x8c64e732u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[5],4);if(io_status)return io_status;goto L8c64e734; /* mov.l r5,@(0x1c,r4) */
L8c64e734: *stop=0x8c64e734u;if(!valid_access(r[4]+32u,4u))return -3;wr(m,bus,&io_status,r[4]+0x20u,r[5],4);if(io_status)return io_status;goto L8c64e736; /* mov.l r5,@(0x20,r4) */
L8c64e736: *stop=0x8c64e736u;if(!valid_access(r[4]+36u,4u))return -3;wr(m,bus,&io_status,r[4]+0x24u,r[5],4);if(io_status)return io_status;goto L8c64e738; /* mov.l r5,@(0x24,r4) */
L8c64e738: *stop=0x8c64e738u;if(!valid_access(r[4]+40u,4u))return -3;wr(m,bus,&io_status,r[4]+0x28u,r[5],4);if(io_status)return io_status;goto L8c64e73a; /* mov.l r5,@(0x28,r4) */
L8c64e73a: *stop=0x8c64e73au;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[5],4);if(io_status)return io_status;goto L8c64e73c; /* mov.l r5,@(0x2c,r4) */
L8c64e73c: *stop=0x8c64e73cu;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[5],4);if(io_status)return io_status;goto L8c64e73e; /* mov.l r5,@(0x30,r4) */
L8c64e73e: *stop=0x8c64e73eu;if(!valid_access(r[4]+52u,4u))return -3;wr(m,bus,&io_status,r[4]+0x34u,r[5],4);if(io_status)return io_status;goto L8c64e740; /* mov.l r5,@(0x34,r4) */
L8c64e740: *stop=0x8c64e740u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[5],4);if(io_status)return io_status;goto L8c64e742; /* mov.l r5,@(0x10,r4) */
L8c64e742: *stop=0x8c64e742u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c64e744; /* mov.w r5,@(r0,r4) */
L8c64e744: *stop=0x8c64e744u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e746; /* mov.l @r2,r3 */
L8c64e746: *stop=0x8c64e746u;r[4]-=r[3];goto L8c64e748; /* sub r3,r4 */
L8c64e748: *stop=0x8c64e748u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e820u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e74a; /* mov.l 0x8c64e820,r3 */
L8c64e74a: *stop=0x8c64e74au;r[1]=r[4];goto L8c64e74c; /* mov r4,r1 */
L8c64e74c: target=r[3];*pr=0x8c64e750u;*stop=0x8c64e74eu;r[0]=68u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64e750; /* jsr @r3 */
L8c64e750: *stop=0x8c64e750u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e81cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64e752; /* mov.l 0x8c64e81c,r4 */
L8c64e752: *stop=0x8c64e752u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e754; /* mov.l @r4,r1 */
L8c64e754: *stop=0x8c64e754u;*sr=(*sr&~1u)|((int32_t)r[1]>0);goto L8c64e756; /* cmp/pl r1 */
L8c64e756: take=!(*sr&1u);*stop=0x8c64e758u;r[6]=r[0];if(take)goto L8c64e78a;goto L8c64e75a; /* bf/s 0x8c64e78a */
L8c64e75a: *stop=0x8c64e75au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e75c; /* mov.l @r4,r0 */
L8c64e75c: *stop=0x8c64e75cu;r[0]+=4294967295u;goto L8c64e75e; /* add #-0x1,r0 */
L8c64e75e: *stop=0x8c64e75eu;*sr=(*sr&~1u)|(r[0]==r[6]);goto L8c64e760; /* cmp/eq r0,r6 */
L8c64e760: if(!(*sr&1u))goto L8c64e78a;goto L8c64e762; /* bf 0x8c64e78a */
L8c64e762: *stop=0x8c64e764u;goto L8c64e76c; /* bra 0x8c64e76c */
L8c64e766: *stop=0x8c64e766u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64e768; /* mov.l @r4,r3 */
L8c64e768: *stop=0x8c64e768u;r[3]+=4294967295u;goto L8c64e76a; /* add #-0x1,r3 */
L8c64e76a: *stop=0x8c64e76au;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c64e76c; /* mov.l r3,@r4 */
L8c64e76c: *stop=0x8c64e76cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e76e; /* mov.l @r4,r2 */
L8c64e76e: *stop=0x8c64e76eu;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c64e770; /* cmp/pl r2 */
L8c64e770: if(!(*sr&1u))goto L8c64e78a;goto L8c64e772; /* bf 0x8c64e78a */
L8c64e772: *stop=0x8c64e772u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e774; /* mov.l @r4,r0 */
L8c64e774: *stop=0x8c64e774u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e810u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64e776; /* mov.l 0x8c64e810,r1 */
L8c64e776: *stop=0x8c64e776u;r[0]+=4294967295u;goto L8c64e778; /* add #-0x1,r0 */
L8c64e778: *stop=0x8c64e778u;r[3]=r[0];goto L8c64e77a; /* mov r0,r3 */
L8c64e77a: *stop=0x8c64e77au;r[0]<<=2;goto L8c64e77c; /* shll2 r0 */
L8c64e77c: *stop=0x8c64e77cu;r[0]<<=2;goto L8c64e77e; /* shll2 r0 */
L8c64e77e: *stop=0x8c64e77eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e780; /* mov.l @r1,r2 */
L8c64e780: *stop=0x8c64e780u;r[0]+=r[3];goto L8c64e782; /* add r3,r0 */
L8c64e782: *stop=0x8c64e782u;r[0]<<=2;goto L8c64e784; /* shll2 r0 */
L8c64e784: *stop=0x8c64e784u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64e786; /* mov.l @(r0,r2),r0 */
L8c64e786: *stop=0x8c64e786u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c64e788; /* cmp/eq #-0x1,r0 */
L8c64e788: if((*sr&1u))goto L8c64e766;goto L8c64e78a; /* bt 0x8c64e766 */
L8c64e78a: *stop=0x8c64e78au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64e818u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64e78c; /* mov.l 0x8c64e818,r6 */
L8c64e78c: *stop=0x8c64e78cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64e78e; /* mov.l @r4,r4 */
L8c64e78e: *stop=0x8c64e78eu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64e790; /* mov.l @r6,r2 */
L8c64e790: *stop=0x8c64e790u;r[4]+=4294967295u;goto L8c64e792; /* add #-0x1,r4 */
L8c64e792: *stop=0x8c64e792u;*sr=(*sr&~1u)|(r[2]>r[4]);goto L8c64e794; /* cmp/hi r4,r2 */
L8c64e794: if(!(*sr&1u))goto L8c64e79a;goto L8c64e796; /* bf 0x8c64e79a */
L8c64e796: *stop=0x8c64e798u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c64e79c; /* bra 0x8c64e79c */
L8c64e79a: *stop=0x8c64e79au;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[5],4);if(io_status)return io_status;goto L8c64e79c; /* mov.l r5,@r6 */
L8c64e79c: *stop=0x8c64e79cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64e79e; /* lds.l @r15+,PR */
L8c64e79e: *stop=0x8c64e79eu;if(*pr!=entry_pr)return -3;*stop=0x8c64e7a0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c66385a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c66385a;
L8c66385a: *stop=0x8c66385au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c66385c; /* mov.l r14,@-r15 */
L8c66385c: *stop=0x8c66385cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c66385e; /* mov.l r13,@-r15 */
L8c66385e: *stop=0x8c66385eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c663860; /* sts.l PR,@-r15 */
L8c663860: *stop=0x8c663860u;r[15]+=4294967288u;goto L8c663862; /* add #-0x8,r15 */
L8c663862: *stop=0x8c663862u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663908u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663864; /* mov.l 0x8c663908,r3 */
L8c663864: *stop=0x8c663864u;r[13]=r[4];goto L8c663866; /* mov r4,r13 */
L8c663866: target=r[3];*pr=0x8c66386au;*stop=0x8c663868u;r[5]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66386a; /* jsr @r3 */
L8c66386a: *stop=0x8c66386au;r[14]=r[0];goto L8c66386c; /* mov r0,r14 */
L8c66386c: *stop=0x8c66386cu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c66386e; /* tst r14,r14 */
L8c66386e: if(!(*sr&1u))goto L8c663874;goto L8c663870; /* bf 0x8c663874 */
L8c663870: *stop=0x8c663872u;r[0]=7u;goto L8c6638e0; /* bra 0x8c6638e0 */
L8c663874: *stop=0x8c663874u;if(!valid_access(r[14],2u))return -3;r[0]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c663876; /* mov.w @r14,r0 */
L8c663876: *stop=0x8c663876u;r[0]=r[0]&65535u;goto L8c663878; /* extu.w r0,r0 */
L8c663878: *stop=0x8c663878u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c66387a; /* tst #0x2,r0 */
L8c66387a: if((*sr&1u))goto L8c6638d4;goto L8c66387c; /* bt 0x8c6638d4 */
L8c66387c: *stop=0x8c66387cu;if(!valid_access(r[15],2u))return -3;r[3]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c66387e; /* mov.w @r15,r3 */
L8c66387e: *stop=0x8c66387eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66390cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663880; /* mov.l 0x8c66390c,r2 */
L8c663880: *stop=0x8c663880u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c663882; /* tst r3,r3 */
L8c663882: take=!(*sr&1u);*stop=0x8c663884u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}if(take)goto L8c66388c;goto L8c663886; /* bf/s 0x8c66388c */
L8c663886: *stop=0x8c663886u;r[4]=r[5];goto L8c663888; /* mov r5,r4 */
L8c663888: *stop=0x8c66388au;r[4]+=r[13];goto L8c663890; /* bra 0x8c663890 */
L8c66388c: *stop=0x8c66388cu;r[4]=r[13];goto L8c66388e; /* mov r13,r4 */
L8c66388e: *stop=0x8c66388eu;r[4]-=r[5];goto L8c663890; /* sub r5,r4 */
L8c663890: *stop=0x8c663890u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663908u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663892; /* mov.l 0x8c663908,r2 */
L8c663892: *stop=0x8c663892u;r[5]=r[15];goto L8c663894; /* mov r15,r5 */
L8c663894: target=r[2];*pr=0x8c663898u;*stop=0x8c663896u;r[5]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663898; /* jsr @r2 */
L8c663898: *stop=0x8c663898u;r[13]=r[0];goto L8c66389a; /* mov r0,r13 */
L8c66389a: *stop=0x8c66389au;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c66389c; /* tst r13,r13 */
L8c66389c: if(!(*sr&1u))goto L8c6638aa;goto L8c66389e; /* bf 0x8c6638aa */
L8c66389e: *stop=0x8c66389eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663910u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6638a0; /* mov.l 0x8c663910,r3 */
L8c6638a0: *stop=0x8c6638a0u;r[5]=r[14];goto L8c6638a2; /* mov r14,r5 */
L8c6638a2: target=r[3];*pr=0x8c6638a6u;*stop=0x8c6638a4u;if(!valid_access(r[15],2u))return -3;r[4]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6638a6; /* jsr @r3 */
L8c6638a6: *stop=0x8c6638a8u;r[0]=0u;goto L8c6638e0; /* bra 0x8c6638e0 */
L8c6638aa: *stop=0x8c6638aau;if(!valid_access(r[13],2u))return -3;r[0]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6638ac; /* mov.w @r13,r0 */
L8c6638ac: *stop=0x8c6638acu;r[0]=r[0]&65535u;goto L8c6638ae; /* extu.w r0,r0 */
L8c6638ae: *stop=0x8c6638aeu;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c6638b0; /* tst #0x4,r0 */
L8c6638b0: if((*sr&1u))goto L8c6638be;goto L8c6638b2; /* bt 0x8c6638be */
L8c6638b2: *stop=0x8c6638b2u;if(!valid_access(r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6638b4; /* mov.w @r14,r2 */
L8c6638b4: *stop=0x8c6638b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663914u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6638b6; /* mov.l 0x8c663914,r3 */
L8c6638b6: *stop=0x8c6638b6u;r[2]&=r[3];goto L8c6638b8; /* and r3,r2 */
L8c6638b8: *stop=0x8c6638b8u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c6638ba; /* mov.w r2,@r14 */
L8c6638ba: *stop=0x8c6638bcu;r[4]=0u;goto L8c6638de; /* bra 0x8c6638de */
L8c6638be: *stop=0x8c6638beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663910u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6638c0; /* mov.l 0x8c663910,r3 */
L8c6638c0: *stop=0x8c6638c0u;r[5]=r[14];goto L8c6638c2; /* mov r14,r5 */
L8c6638c2: target=r[3];*pr=0x8c6638c6u;*stop=0x8c6638c4u;if(!valid_access(r[15],2u))return -3;r[4]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6638c6; /* jsr @r3 */
L8c6638c6: *stop=0x8c6638c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663910u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6638c8; /* mov.l 0x8c663910,r3 */
L8c6638c8: *stop=0x8c6638c8u;r[5]=r[13];goto L8c6638ca; /* mov r13,r5 */
L8c6638ca: *stop=0x8c6638cau;if(!valid_access(r[15]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c6638cc; /* mov.w @(0x4,r15),r0 */
L8c6638cc: target=r[3];*pr=0x8c6638d0u;*stop=0x8c6638ceu;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6638d0; /* jsr @r3 */
L8c6638d0: *stop=0x8c6638d2u;r[4]=r[0];goto L8c6638de; /* bra 0x8c6638de */
L8c6638d4: *stop=0x8c6638d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663910u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6638d6; /* mov.l 0x8c663910,r3 */
L8c6638d6: *stop=0x8c6638d6u;r[5]=r[14];goto L8c6638d8; /* mov r14,r5 */
L8c6638d8: target=r[3];*pr=0x8c6638dcu;*stop=0x8c6638dau;if(!valid_access(r[15],2u))return -3;r[4]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6638dc; /* jsr @r3 */
L8c6638dc: *stop=0x8c6638dcu;r[4]=r[0];goto L8c6638de; /* mov r0,r4 */
L8c6638de: *stop=0x8c6638deu;r[0]=r[4];goto L8c6638e0; /* mov r4,r0 */
L8c6638e0: *stop=0x8c6638e0u;r[15]+=8u;goto L8c6638e2; /* add #0x8,r15 */
L8c6638e2: *stop=0x8c6638e2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6638e4; /* lds.l @r15+,PR */
L8c6638e4: *stop=0x8c6638e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6638e6; /* mov.l @r15+,r13 */
L8c6638e6: *stop=0x8c6638e6u;if(*pr!=entry_pr)return -3;*stop=0x8c6638e8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c664940(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c664940;
L8c664940: *stop=0x8c664940u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664a80u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664942; /* mov.l 0x8c664a80,r2 */
L8c664942: *stop=0x8c664942u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c664944; /* mov.l @r2,r3 */
L8c664944: *stop=0x8c664944u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c664946; /* cmp/ge r3,r4 */
L8c664946: take=(*sr&1u);*stop=0x8c664948u;r[6]=0u;if(take)goto L8c66494e;goto L8c66494a; /* bt/s 0x8c66494e */
L8c66494a: *stop=0x8c66494cu;if(!valid_access(r[5],2u))return -3;wr(m,bus,&io_status,r[5],r[6],2);if(io_status)return io_status;goto L8c664952; /* bra 0x8c664952 */
L8c66494e: *stop=0x8c66494eu;r[1]=1u;goto L8c664950; /* mov #0x1,r1 */
L8c664950: *stop=0x8c664950u;if(!valid_access(r[5],2u))return -3;wr(m,bus,&io_status,r[5],r[1],2);if(io_status)return io_status;goto L8c664952; /* mov.w r1,@r5 */
L8c664952: *stop=0x8c664952u;if(!valid_access(r[5],2u))return -3;r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c664954; /* mov.w @r5,r3 */
L8c664954: *stop=0x8c664954u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664a84u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c664956; /* mov.l 0x8c664a84,r0 */
L8c664956: *stop=0x8c664956u;r[2]=r[3];goto L8c664958; /* mov r3,r2 */
L8c664958: *stop=0x8c664958u;r[3]<<=2;goto L8c66495a; /* shll2 r3 */
L8c66495a: *stop=0x8c66495au;r[3]+=r[2];goto L8c66495c; /* add r2,r3 */
L8c66495c: *stop=0x8c66495cu;r[3]<<=2;goto L8c66495e; /* shll2 r3 */
L8c66495e: *stop=0x8c66495eu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c664960; /* mov.l @(r0,r3),r5 */
L8c664960: *stop=0x8c664960u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c664962; /* tst r5,r5 */
L8c664962: take=(*sr&1u);*stop=0x8c664964u;r[7]=r[6];if(take)goto L8c664978;goto L8c664966; /* bt/s 0x8c664978 */
L8c664966: *stop=0x8c664966u;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c664968; /* mov.l @(0xc,r5),r1 */
L8c664968: *stop=0x8c664968u;*sr=(*sr&~1u)|(r[4]==r[1]);goto L8c66496a; /* cmp/eq r4,r1 */
L8c66496a: if(!(*sr&1u))goto L8c664972;goto L8c66496c; /* bf 0x8c664972 */
L8c66496c: *stop=0x8c66496cu;r[7]=r[5];goto L8c66496e; /* mov r5,r7 */
L8c66496e: *stop=0x8c664970u;r[5]=r[6];goto L8c664974; /* bra 0x8c664974 */
L8c664972: *stop=0x8c664972u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c664974; /* mov.l @(0x8,r5),r5 */
L8c664974: *stop=0x8c664974u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c664976; /* tst r5,r5 */
L8c664976: if(!(*sr&1u))goto L8c664966;goto L8c664978; /* bf 0x8c664966 */
L8c664978: *stop=0x8c664978u;if(*pr!=entry_pr)return -3;*stop=0x8c66497au;r[0]=r[7];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_main_8c66497c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c66497c;
L8c66497c: *stop=0x8c66497cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c66497e; /* mov.l r14,@-r15 */
L8c66497e: *stop=0x8c66497eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c664980; /* mov.l r13,@-r15 */
L8c664980: *stop=0x8c664980u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c664982; /* mov.l r12,@-r15 */
L8c664982: *stop=0x8c664982u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c664984; /* mov.l r11,@-r15 */
L8c664984: *stop=0x8c664984u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c664986; /* mov.l r10,@-r15 */
L8c664986: *stop=0x8c664986u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c664988; /* mov.l r9,@-r15 */
L8c664988: *stop=0x8c664988u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66498a; /* sts.l PR,@-r15 */
L8c66498a: *stop=0x8c66498au;r[15]+=4294967292u;goto L8c66498c; /* add #-0x4,r15 */
L8c66498c: *stop=0x8c66498cu;r[14]=r[5];goto L8c66498e; /* mov r5,r14 */
L8c66498e: *stop=0x8c66498eu;if(!valid_access(r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c664990; /* mov.w @r14,r2 */
L8c664990: *stop=0x8c664990u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664a88u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664992; /* mov.l 0x8c664a88,r3 */
L8c664992: *stop=0x8c664992u;r[10]=r[4];goto L8c664994; /* mov r4,r10 */
L8c664994: *stop=0x8c664994u;r[11]=(uint32_t)(int32_t)(int16_t)r[10];goto L8c664996; /* exts.w r10,r11 */
L8c664996: *stop=0x8c664996u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664a84u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c664998; /* mov.l 0x8c664a84,r9 */
L8c664998: *stop=0x8c664998u;r[2]&=r[3];goto L8c66499a; /* and r3,r2 */
L8c66499a: *stop=0x8c66499au;r[6]=r[14];goto L8c66499c; /* mov r14,r6 */
L8c66499c: *stop=0x8c66499cu;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c66499e; /* mov.w r2,@r14 */
L8c66499e: *stop=0x8c66499eu;r[2]=r[11];goto L8c6649a0; /* mov r11,r2 */
L8c6649a0: *stop=0x8c6649a0u;r[11]<<=2;goto L8c6649a2; /* shll2 r11 */
L8c6649a2: *stop=0x8c6649a2u;r[11]+=r[2];goto L8c6649a4; /* add r2,r11 */
L8c6649a4: *stop=0x8c6649a4u;r[11]<<=2;goto L8c6649a6; /* shll2 r11 */
L8c6649a6: *stop=0x8c6649a6u;r[11]+=r[9];goto L8c6649a8; /* add r9,r11 */
L8c6649a8: *stop=0x8c6649a8u;r[5]=r[11];goto L8c6649aa; /* mov r11,r5 */
L8c6649aa: *stop=0x8c6649aau;r[5]+=4u;goto L8c6649ac; /* add #0x4,r5 */
L8c6649ac: target=0x8c664b52u;*pr=0x8c6649b0u;*stop=0x8c6649aeu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6649b0; /* bsr 0x8c664b52 */
L8c6649b0: *stop=0x8c6649b0u;if(!valid_access(r[11]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6649b2; /* mov.l @(0x10,r11),r2 */
L8c6649b2: *stop=0x8c6649b2u;r[12]=0u;goto L8c6649b4; /* mov #0x0,r12 */
L8c6649b4: *stop=0x8c6649b4u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6649b6; /* mov.l @(0x10,r14),r3 */
L8c6649b6: *stop=0x8c6649b6u;r[2]+=r[3];goto L8c6649b8; /* add r3,r2 */
L8c6649b8: *stop=0x8c6649b8u;if(!valid_access(r[11]+16u,4u))return -3;wr(m,bus,&io_status,r[11]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6649ba; /* mov.l r2,@(0x10,r11) */
L8c6649ba: *stop=0x8c6649bau;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6649bc; /* mov.l @(0x8,r11),r4 */
L8c6649bc: *stop=0x8c6649bcu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6649be; /* tst r4,r4 */
L8c6649be: take=(*sr&1u);*stop=0x8c6649c0u;r[13]=r[12];if(take)goto L8c6649e2;goto L8c6649c2; /* bt/s 0x8c6649e2 */
L8c6649c2: *stop=0x8c6649c2u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6649c4; /* mov.l @(0x10,r14),r2 */
L8c6649c4: *stop=0x8c6649c4u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6649c6; /* mov.l @(0xc,r14),r6 */
L8c6649c6: *stop=0x8c6649c6u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6649c8; /* mov.l @(0xc,r4),r5 */
L8c6649c8: *stop=0x8c6649c8u;r[2]+=r[6];goto L8c6649ca; /* add r6,r2 */
L8c6649ca: *stop=0x8c6649cau;*sr=(*sr&~1u)|(r[2]==r[5]);goto L8c6649cc; /* cmp/eq r2,r5 */
L8c6649cc: if((*sr&1u))goto L8c6649d6;goto L8c6649ce; /* bt 0x8c6649d6 */
L8c6649ce: *stop=0x8c6649ceu;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6649d0; /* mov.l @(0x10,r4),r2 */
L8c6649d0: *stop=0x8c6649d0u;r[2]+=r[5];goto L8c6649d2; /* add r5,r2 */
L8c6649d2: *stop=0x8c6649d2u;*sr=(*sr&~1u)|(r[2]==r[6]);goto L8c6649d4; /* cmp/eq r2,r6 */
L8c6649d4: if(!(*sr&1u))goto L8c6649dc;goto L8c6649d6; /* bf 0x8c6649dc */
L8c6649d6: *stop=0x8c6649d6u;r[13]=r[4];goto L8c6649d8; /* mov r4,r13 */
L8c6649d8: *stop=0x8c6649dau;r[4]=r[12];goto L8c6649de; /* bra 0x8c6649de */
L8c6649dc: *stop=0x8c6649dcu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6649de; /* mov.l @(0x8,r4),r4 */
L8c6649de: *stop=0x8c6649deu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6649e0; /* tst r4,r4 */
L8c6649e0: if(!(*sr&1u))goto L8c6649c2;goto L8c6649e2; /* bf 0x8c6649c2 */
L8c6649e2: *stop=0x8c6649e2u;r[11]=(uint32_t)(int32_t)(int16_t)r[10];goto L8c6649e4; /* exts.w r10,r11 */
L8c6649e4: *stop=0x8c6649e4u;r[3]=r[11];goto L8c6649e6; /* mov r11,r3 */
L8c6649e6: *stop=0x8c6649e6u;r[11]<<=2;goto L8c6649e8; /* shll2 r11 */
L8c6649e8: *stop=0x8c6649e8u;r[11]+=r[3];goto L8c6649ea; /* add r3,r11 */
L8c6649ea: *stop=0x8c6649eau;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c6649ec; /* tst r13,r13 */
L8c6649ec: *stop=0x8c6649ecu;r[11]<<=2;goto L8c6649ee; /* shll2 r11 */
L8c6649ee: take=(*sr&1u);*stop=0x8c6649f0u;r[11]+=r[9];if(take)goto L8c664aa0;goto L8c6649f2; /* bt/s 0x8c664aa0 */
L8c6649f2: *stop=0x8c6649f2u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6649f4; /* mov.l @(0xc,r14),r2 */
L8c6649f4: *stop=0x8c6649f4u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6649f6; /* mov.l @(0x10,r14),r3 */
L8c6649f6: *stop=0x8c6649f6u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6649f8; /* mov.l @(0xc,r13),r1 */
L8c6649f8: *stop=0x8c6649f8u;r[2]+=r[3];goto L8c6649fa; /* add r3,r2 */
L8c6649fa: *stop=0x8c6649fau;*sr=(*sr&~1u)|(r[2]==r[1]);goto L8c6649fc; /* cmp/eq r2,r1 */
L8c6649fc: take=!(*sr&1u);*stop=0x8c6649feu;r[4]=r[14];if(take)goto L8c664a0e;goto L8c664a00; /* bf/s 0x8c664a0e */
L8c664a00: *stop=0x8c664a00u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664a02; /* mov.l @(0xc,r14),r2 */
L8c664a02: *stop=0x8c664a02u;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[2],4);if(io_status)return io_status;goto L8c664a04; /* mov.l r2,@(0xc,r13) */
L8c664a04: *stop=0x8c664a04u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664a06; /* mov.l @(0x10,r14),r3 */
L8c664a06: *stop=0x8c664a06u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c664a08; /* mov.l @(0x10,r13),r1 */
L8c664a08: *stop=0x8c664a08u;r[1]+=r[3];goto L8c664a0a; /* add r3,r1 */
L8c664a0a: *stop=0x8c664a0cu;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[1],4);if(io_status)return io_status;goto L8c664a16; /* bra 0x8c664a16 */
L8c664a0e: *stop=0x8c664a0eu;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664a10; /* mov.l @(0x10,r13),r2 */
L8c664a10: *stop=0x8c664a10u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664a12; /* mov.l @(0x10,r14),r3 */
L8c664a12: *stop=0x8c664a12u;r[2]+=r[3];goto L8c664a14; /* add r3,r2 */
L8c664a14: *stop=0x8c664a14u;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[2],4);if(io_status)return io_status;goto L8c664a16; /* mov.l r2,@(0x10,r13) */
L8c664a16: target=0x8c664aecu;*pr=0x8c664a1au;*stop=0x8c664a18u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c664a1a; /* bsr 0x8c664aec */
L8c664a1a: *stop=0x8c664a1au;r[5]=r[11];goto L8c664a1c; /* mov r11,r5 */
L8c664a1c: *stop=0x8c664a1cu;r[4]=r[11];goto L8c664a1e; /* mov r11,r4 */
L8c664a1e: *stop=0x8c664a1eu;r[5]+=12u;goto L8c664a20; /* add #0xc,r5 */
L8c664a20: *stop=0x8c664a20u;r[6]=r[13];goto L8c664a22; /* mov r13,r6 */
L8c664a22: target=0x8c664b52u;*pr=0x8c664a26u;*stop=0x8c664a24u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c664a26; /* bsr 0x8c664b52 */
L8c664a26: *stop=0x8c664a26u;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c664a28; /* mov.l @(0x8,r11),r4 */
L8c664a28: *stop=0x8c664a28u;r[14]=r[13];goto L8c664a2a; /* mov r13,r14 */
L8c664a2a: *stop=0x8c664a2au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c664a2c; /* tst r4,r4 */
L8c664a2c: take=(*sr&1u);*stop=0x8c664a2eu;r[13]=r[12];if(take)goto L8c664a50;goto L8c664a30; /* bt/s 0x8c664a50 */
L8c664a30: *stop=0x8c664a30u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664a32; /* mov.l @(0x10,r14),r2 */
L8c664a32: *stop=0x8c664a32u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c664a34; /* mov.l @(0xc,r14),r5 */
L8c664a34: *stop=0x8c664a34u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c664a36; /* mov.l @(0xc,r4),r6 */
L8c664a36: *stop=0x8c664a36u;r[2]+=r[5];goto L8c664a38; /* add r5,r2 */
L8c664a38: *stop=0x8c664a38u;*sr=(*sr&~1u)|(r[2]==r[6]);goto L8c664a3a; /* cmp/eq r2,r6 */
L8c664a3a: if((*sr&1u))goto L8c664a44;goto L8c664a3c; /* bt 0x8c664a44 */
L8c664a3c: *stop=0x8c664a3cu;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664a3e; /* mov.l @(0x10,r4),r2 */
L8c664a3e: *stop=0x8c664a3eu;r[2]+=r[6];goto L8c664a40; /* add r6,r2 */
L8c664a40: *stop=0x8c664a40u;*sr=(*sr&~1u)|(r[2]==r[5]);goto L8c664a42; /* cmp/eq r2,r5 */
L8c664a42: if(!(*sr&1u))goto L8c664a4a;goto L8c664a44; /* bf 0x8c664a4a */
L8c664a44: *stop=0x8c664a44u;r[13]=r[4];goto L8c664a46; /* mov r4,r13 */
L8c664a46: *stop=0x8c664a48u;r[4]=r[12];goto L8c664a4c; /* bra 0x8c664a4c */
L8c664a4a: *stop=0x8c664a4au;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c664a4c; /* mov.l @(0x8,r4),r4 */
L8c664a4c: *stop=0x8c664a4cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c664a4e; /* tst r4,r4 */
L8c664a4e: if(!(*sr&1u))goto L8c664a30;goto L8c664a50; /* bf 0x8c664a30 */
L8c664a50: *stop=0x8c664a50u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c664a52; /* tst r13,r13 */
L8c664a52: if(!(*sr&1u))goto L8c6649f2;goto L8c664a54; /* bf 0x8c6649f2 */
L8c664a54: *stop=0x8c664a54u;r[5]=(uint32_t)(int32_t)(int16_t)r[10];goto L8c664a56; /* exts.w r10,r5 */
L8c664a56: *stop=0x8c664a56u;r[6]=r[14];goto L8c664a58; /* mov r14,r6 */
L8c664a58: *stop=0x8c664a58u;r[3]=r[5];goto L8c664a5a; /* mov r5,r3 */
L8c664a5a: *stop=0x8c664a5au;r[5]<<=2;goto L8c664a5c; /* shll2 r5 */
L8c664a5c: *stop=0x8c664a5cu;r[5]+=r[3];goto L8c664a5e; /* add r3,r5 */
L8c664a5e: *stop=0x8c664a5eu;r[5]<<=2;goto L8c664a60; /* shll2 r5 */
L8c664a60: *stop=0x8c664a60u;r[5]+=r[9];goto L8c664a62; /* add r9,r5 */
L8c664a62: *stop=0x8c664a62u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c664a64; /* mov.l r5,@r15 */
L8c664a64: *stop=0x8c664a64u;r[5]+=12u;goto L8c664a66; /* add #0xc,r5 */
L8c664a66: *stop=0x8c664a68u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c664aa8; /* bra 0x8c664aa8 */
L8c664aa0: *stop=0x8c664aa0u;r[5]=r[11];goto L8c664aa2; /* mov r11,r5 */
L8c664aa2: *stop=0x8c664aa2u;r[4]=r[11];goto L8c664aa4; /* mov r11,r4 */
L8c664aa4: *stop=0x8c664aa4u;r[5]+=12u;goto L8c664aa6; /* add #0xc,r5 */
L8c664aa6: *stop=0x8c664aa6u;r[6]=r[14];goto L8c664aa8; /* mov r14,r6 */
L8c664aa8: target=0x8c664b12u;*pr=0x8c664aacu;*stop=0x8c664aaau;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c664aac; /* bsr 0x8c664b12 */
L8c664aac: *stop=0x8c664aacu;r[0]=0u;goto L8c664aae; /* mov #0x0,r0 */
L8c664aae: *stop=0x8c664aaeu;r[15]+=4u;goto L8c664ab0; /* add #0x4,r15 */
L8c664ab0: *stop=0x8c664ab0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c664ab2; /* lds.l @r15+,PR */
L8c664ab2: *stop=0x8c664ab2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c664ab4; /* mov.l @r15+,r9 */
L8c664ab4: *stop=0x8c664ab4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c664ab6; /* mov.l @r15+,r10 */
L8c664ab6: *stop=0x8c664ab6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c664ab8; /* mov.l @r15+,r11 */
L8c664ab8: *stop=0x8c664ab8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c664aba; /* mov.l @r15+,r12 */
L8c664aba: *stop=0x8c664abau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c664abc; /* mov.l @r15+,r13 */
L8c664abc: *stop=0x8c664abcu;if(*pr!=entry_pr)return -3;*stop=0x8c664abeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
