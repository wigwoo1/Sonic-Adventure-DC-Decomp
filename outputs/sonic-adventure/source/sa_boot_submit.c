/* Geometry projection, sampling and view proposals; explicit GBR/queue state.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_integer_state.h"
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
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_boot_submit_8c641f00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 unsigned guard_visits[3]={0};(void)guard_visits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c641f00;
L8c641f00: *stop=0x8c641f00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c641f02; /* mov.l r14,@-r15 */
L8c641f02: *stop=0x8c641f02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c641f04; /* mov.l r13,@-r15 */
L8c641f04: *stop=0x8c641f04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c641f06; /* mov.l r12,@-r15 */
L8c641f06: *stop=0x8c641f06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c641f08; /* mov.l r11,@-r15 */
L8c641f08: *stop=0x8c641f08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c641f0a; /* mov.l r10,@-r15 */
L8c641f0a: *stop=0x8c641f0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c641f0c; /* mov.l r9,@-r15 */
L8c641f0c: *stop=0x8c641f0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c641f0e; /* mov.l r8,@-r15 */
L8c641f0e: *stop=0x8c641f0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c641f10; /* sts.l PR,@-r15 */
L8c641f10: *stop=0x8c641f10u;r[15]+=4294967288u;goto L8c641f12; /* add #-0x8,r15 */
L8c641f12: *stop=0x8c641f12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fa0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641f14; /* mov.l 0x8c641fa0,r2 */
L8c641f14: *stop=0x8c641f14u;r[13]=0u;goto L8c641f16; /* mov #0x0,r13 */
L8c641f16: *stop=0x8c641f16u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f18; /* mov.l @r2,r3 */
L8c641f18: *stop=0x8c641f18u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c641f1a; /* tst r3,r3 */
L8c641f1a: take=(*sr&1u);*stop=0x8c641f1cu;r[14]=1u;if(take)goto L8c641f22;goto L8c641f1e; /* bt/s 0x8c641f22 */
L8c641f1e: *stop=0x8c641f20u;goto L8c64204a; /* bra 0x8c64204a */
L8c641f22: *stop=0x8c641f22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fa4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641f24; /* mov.l 0x8c641fa4,r1 */
L8c641f24: *stop=0x8c641f24u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f26; /* mov.l @r1,r3 */
L8c641f26: *stop=0x8c641f26u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c641f28; /* tst r3,r3 */
L8c641f28: if(!(*sr&1u))goto L8c641f3a;goto L8c641f2a; /* bf 0x8c641f3a */
L8c641f2a: *stop=0x8c641f2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fa8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f2c; /* mov.l 0x8c641fa8,r0 */
L8c641f2c: *stop=0x8c641f2cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f2e; /* mov.l @r0,r3 */
L8c641f2e: *stop=0x8c641f2eu;r[0]+=4294967280u;goto L8c641f30; /* add #-0x10,r0 */
L8c641f30: *stop=0x8c641f30u;r[3]<<=2;goto L8c641f32; /* shll2 r3 */
L8c641f32: *stop=0x8c641f32u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[14],4);if(io_status)return io_status;goto L8c641f34; /* mov.l r14,@(r0,r3) */
L8c641f34: *stop=0x8c641f34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641facu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f36; /* mov.l 0x8c641fac,r3 */
L8c641f36: *stop=0x8c641f38u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c64204a; /* bra 0x8c64204a */
L8c641f3a: *stop=0x8c641f3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fb4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f3c; /* mov.l 0x8c641fb4,r0 */
L8c641f3c: *stop=0x8c641f3cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fb0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c641f3e; /* mov.l 0x8c641fb0,r5 */
L8c641f3e: *stop=0x8c641f3eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f40; /* mov.l @r0,r3 */
L8c641f40: *stop=0x8c641f40u;r[0]=72u;goto L8c641f42; /* mov #0x48,r0 */
L8c641f42: *stop=0x8c641f42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fa8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641f44; /* mov.l 0x8c641fa8,r2 */
L8c641f44: *stop=0x8c641f44u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[3],4);if(io_status)return io_status;goto L8c641f46; /* mov.l r3,@(r0,r5) */
L8c641f46: *stop=0x8c641f46u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto L8c641f48; /* mov.l @r2,r4 */
L8c641f48: *stop=0x8c641f48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fb8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f4a; /* mov.l 0x8c641fb8,r3 */
L8c641f4a: *stop=0x8c641f4au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c641f4c; /* mov.l r4,@r3 */
L8c641f4c: *stop=0x8c641f4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fbcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641f4e; /* mov.l 0x8c641fbc,r1 */
L8c641f4e: *stop=0x8c641f4eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c641f50; /* mov.l r14,@r1 */
L8c641f50: *stop=0x8c641f50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fc0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641f52; /* mov.l 0x8c641fc0,r2 */
L8c641f52: *stop=0x8c641f52u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c641f54; /* mov.l r13,@r2 */
L8c641f54: *stop=0x8c641f54u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fc4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f56; /* mov.l 0x8c641fc4,r0 */
L8c641f56: *stop=0x8c641f56u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f58; /* mov.l @r0,r3 */
L8c641f58: *stop=0x8c641f58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fc8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641f5a; /* mov.l 0x8c641fc8,r1 */
L8c641f5a: *stop=0x8c641f5au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c641f5c; /* mov.l r3,@r1 */
L8c641f5c: *stop=0x8c641f5cu;r[2]+=8u;goto L8c641f5e; /* add #0x8,r2 */
L8c641f5e: *stop=0x8c641f5eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c641f60; /* mov.l r14,@r2 */
L8c641f60: *stop=0x8c641f60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f62; /* mov.l 0x8c641fcc,r0 */
L8c641f62: *stop=0x8c641f62u;r[3]=2u;goto L8c641f64; /* mov #0x2,r3 */
L8c641f64: *stop=0x8c641f64u;r[4]<<=2;goto L8c641f66; /* shll2 r4 */
L8c641f66: *stop=0x8c641f66u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[13],4);if(io_status)return io_status;goto L8c641f68; /* mov.l r13,@(0x4,r5) */
L8c641f68: *stop=0x8c641f68u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[3],4);if(io_status)return io_status;goto L8c641f6a; /* mov.l r3,@(r0,r4) */
L8c641f6a: *stop=0x8c641f6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fd0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641f6c; /* mov.l 0x8c641fd0,r1 */
L8c641f6c: *stop=0x8c641f6cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f6e; /* mov.l @r1,r0 */
L8c641f6e: *stop=0x8c641f6eu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c641f70; /* tst #0x20,r0 */
L8c641f70: if((*sr&1u))goto L8c642000;goto L8c641f72; /* bt 0x8c642000 */
L8c641f72: *stop=0x8c641f72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fd4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f74; /* mov.l 0x8c641fd4,r0 */
L8c641f74: *stop=0x8c641f74u;r[4]=16u;goto L8c641f76; /* mov #0x10,r4 */
L8c641f76: *stop=0x8c641f76u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;goto L8c641f78; /* mov.l r4,@r0 */
L8c641f78: *stop=0x8c641f78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fd8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f7a; /* mov.l 0x8c641fd8,r3 */
L8c641f7a: *stop=0x8c641f7au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c641f7c; /* mov.l r4,@r3 */
L8c641f7c: *stop=0x8c641f7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fe4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641f7e; /* mov.l 0x8c641fe4,r2 */
L8c641f7e: *stop=0x8c641f7eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fe0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c641f80; /* mov.l 0x8c641fe0,r4 */
L8c641f80: *stop=0x8c641f80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fdcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c641f82; /* mov.l 0x8c641fdc,r5 */
L8c641f82: target=r[2];*pr=0x8c641f86u;*stop=0x8c641f84u;r[6]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641f86; /* jsr @r2 */
L8c641f86: *stop=0x8c641f86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fe0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f88; /* mov.l 0x8c641fe0,r3 */
L8c641f88: *stop=0x8c641f88u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c641f8a; /* pref @r3 */
L8c641f8a: *stop=0x8c641f8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fe8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641f8c; /* mov.l 0x8c641fe8,r2 */
L8c641f8c: target=0x8c641e00u;*pr=0x8c641f90u;*stop=0x8c641f8eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641f90; /* bsr 0x8c641e00 */
L8c641f90: *stop=0x8c641f92u;goto L8c64204a; /* bra 0x8c64204a */
L8c642000: *stop=0x8c642000u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642002; /* mov.l 0x8c6420c4,r3 */
L8c642002: *stop=0x8c642002u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642004; /* mov.l 0x8c6420c8,r2 */
L8c642004: *stop=0x8c642004u;r[1]=r[3];goto L8c642006; /* mov r3,r1 */
L8c642006: *stop=0x8c642006u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[10]=loaded;}goto L8c642008; /* mov.l @r3,r10 */
L8c642008: *stop=0x8c642008u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[9]=loaded;}goto L8c64200a; /* mov.l @r1,r9 */
L8c64200a: *stop=0x8c64200au;r[3]+=116u;goto L8c64200c; /* add #0x74,r3 */
L8c64200c: *stop=0x8c64200cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420ccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64200e; /* mov.l 0x8c6420cc,r0 */
L8c64200e: *stop=0x8c64200eu;r[10]<<=2;goto L8c642010; /* shll2 r10 */
L8c642010: *stop=0x8c642010u;r[9]<<=2;goto L8c642012; /* shll2 r9 */
L8c642012: *stop=0x8c642012u;r[10]+=r[2];goto L8c642014; /* add r2,r10 */
L8c642014: *stop=0x8c642014u;r[9]+=r[3];goto L8c642016; /* add r3,r9 */
L8c642016: *stop=0x8c642016u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642018; /* mov.l @r0,r2 */
L8c642018: *stop=0x8c642018u;r[2]<<=2;goto L8c64201a; /* shll2 r2 */
L8c64201a: *stop=0x8c64201au;r[2]<<=2;goto L8c64201c; /* shll2 r2 */
L8c64201c: *stop=0x8c64201cu;r[9]+=r[2];goto L8c64201e; /* add r2,r9 */
L8c64201e: *stop=0x8c64201eu;r[2]=r[0];goto L8c642020; /* mov r0,r2 */
L8c642020: *stop=0x8c642020u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642022; /* mov.l @r2,r3 */
L8c642022: *stop=0x8c642022u;r[3]<<=2;goto L8c642024; /* shll2 r3 */
L8c642024: *stop=0x8c642024u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d0u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c642026; /* mov.l 0x8c6420d0,r8 */
L8c642026: *stop=0x8c642026u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c642028; /* mov.l 0x8c6420d4,r11 */
L8c642028: *stop=0x8c642028u;r[3]<<=2;goto L8c64202a; /* shll2 r3 */
L8c64202a: *stop=0x8c64202au;r[12]=sx(m,bus,&io_status,0x8c6420c0u,2);if(io_status)return io_status;goto L8c64202c; /* mov.w 0x8c6420c0,r12 */
L8c64202c: *stop=0x8c64202cu;r[10]+=r[3];goto L8c64202e; /* add r3,r10 */
L8c64202e: *stop=0x8c64202eu;r[2]=r[15];goto L8c642030; /* mov r15,r2 */
L8c642030: *stop=0x8c642030u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c642032; /* mov.l r2,@-r15 */
L8c642032: *stop=0x8c642032u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[6]=loaded;}goto L8c642034; /* mov.l @r9,r6 */
L8c642034: *stop=0x8c642034u;r[7]=16u;goto L8c642036; /* mov #0x10,r7 */
L8c642036: *stop=0x8c642036u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[5]=loaded;}goto L8c642038; /* mov.l @r10,r5 */
L8c642038: *stop=0x8c642038u;r[6]+=32u;goto L8c64203a; /* add #0x20,r6 */
L8c64203a: target=r[8];*pr=0x8c64203eu;*stop=0x8c64203cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64203e; /* jsr @r8 */
L8c64203e: *stop=0x8c64203eu;*sr=(*sr&~1u)|(r[12]==r[0]);goto L8c642040; /* cmp/eq r12,r0 */
L8c642040: 
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c642040u;if((*sr&1u) && ++guard_visits[0]>=2u)return -4;
#endif
 take=(*sr&1u);*stop=0x8c642042u;r[15]+=4u;if(take)goto L8c64202e;goto L8c642044; /* bt/s 0x8c64202e */
L8c642044: *stop=0x8c642044u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642046; /* mov.l 0x8c6420cc,r2 */
L8c642046: target=0x8c641e00u;*pr=0x8c64204au;*stop=0x8c642048u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64204a; /* bsr 0x8c641e00 */
L8c64204a: *stop=0x8c64204au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64204c; /* mov.l 0x8c6420d8,r1 */
L8c64204c: *stop=0x8c64204cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c64204e; /* mov.l r13,@r1 */
L8c64204e: *stop=0x8c64204eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642050; /* mov.l 0x8c6420dc,r2 */
L8c642050: *stop=0x8c642050u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642052; /* mov.l @r2,r3 */
L8c642052: *stop=0x8c642052u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c642054; /* tst r3,r3 */
L8c642054: take=(*sr&1u);*stop=0x8c642056u;r[12]=r[13];if(take)goto L8c642080;goto L8c642058; /* bt/s 0x8c642080 */
L8c642058: *stop=0x8c642058u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64205a; /* mov.l 0x8c6420e0,r3 */
L8c64205a: *stop=0x8c64205au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64205c; /* mov.l @r3,r2 */
L8c64205c: target=r[2];*pr=0x8c642060u;*stop=0x8c64205eu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642060; /* jsr @r2 */
L8c642060: *stop=0x8c642060u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420e4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c642062; /* mov.l 0x8c6420e4,r1 */
L8c642062: *stop=0x8c642062u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642064; /* mov.l @r1,r3 */
L8c642064: *stop=0x8c642064u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420e8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642066; /* mov.l 0x8c6420e8,r0 */
L8c642066: *stop=0x8c642066u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642068; /* mov.l @r0,r2 */
L8c642068: *stop=0x8c642068u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64206a; /* mov.l 0x8c6420d8,r1 */
L8c64206a: *stop=0x8c64206au;r[2]+=r[3];goto L8c64206c; /* add r3,r2 */
L8c64206c: *stop=0x8c64206cu;r[2]+=1u;goto L8c64206e; /* add #0x1,r2 */
L8c64206e: *stop=0x8c64206eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642070; /* mov.l @r1,r3 */
L8c642070: *stop=0x8c642070u;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c642072; /* cmp/hi r2,r3 */
L8c642072: if(!(*sr&1u))goto L8c642078;goto L8c642074; /* bf 0x8c642078 */
L8c642074: *stop=0x8c642076u;r[12]=r[14];goto L8c642080; /* bra 0x8c642080 */
L8c642078: *stop=0x8c642078u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64207a; /* mov.l 0x8c6420dc,r3 */
L8c64207a: *stop=0x8c64207au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64207c; /* mov.l @r3,r2 */
L8c64207c: *stop=0x8c64207cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64207e; /* tst r2,r2 */
L8c64207e: 
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c64207eu;if(!(*sr&1u) && ++guard_visits[1]>=2u)return -4;
#endif
 if(!(*sr&1u))goto L8c642058;goto L8c642080; /* bf 0x8c642058 */
L8c642080: *stop=0x8c642080u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c642082; /* tst r12,r12 */
L8c642082: if((*sr&1u))goto L8c642112;goto L8c642084; /* bt 0x8c642112 */
L8c642084: *stop=0x8c642084u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642086; /* mov.l 0x8c6420ec,r3 */
L8c642086: *stop=0x8c642086u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c642088; /* mov.l r13,@r3 */
L8c642088: *stop=0x8c642088u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64208a; /* mov.l 0x8c6420d8,r2 */
L8c64208a: *stop=0x8c64208au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c64208c; /* mov.l r13,@r2 */
L8c64208c: *stop=0x8c64208cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420dcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64208e; /* mov.l 0x8c6420dc,r0 */
L8c64208e: *stop=0x8c64208eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c642090; /* mov.l @r0,r1 */
L8c642090: *stop=0x8c642090u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c642092; /* tst r1,r1 */
L8c642092: take=(*sr&1u);*stop=0x8c642094u;r[12]=r[13];if(take)goto L8c642108;goto L8c642096; /* bt/s 0x8c642108 */
L8c642096: *stop=0x8c642096u;r[11]=20u;goto L8c642098; /* mov #0x14,r11 */
L8c642098: *stop=0x8c642098u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64209a; /* mov.l 0x8c6420e0,r3 */
L8c64209a: *stop=0x8c64209au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64209c; /* mov.l @r3,r2 */
L8c64209c: target=r[2];*pr=0x8c6420a0u;*stop=0x8c64209eu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6420a0; /* jsr @r2 */
L8c6420a0: *stop=0x8c6420a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6420a2; /* mov.l 0x8c6420d8,r2 */
L8c6420a2: *stop=0x8c6420a2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6420a4; /* mov.l @r2,r1 */
L8c6420a4: *stop=0x8c6420a4u;*sr=(*sr&~1u)|(r[1]>r[11]);goto L8c6420a6; /* cmp/hi r11,r1 */
L8c6420a6: if(!(*sr&1u))goto L8c642100;goto L8c6420a8; /* bf 0x8c642100 */
L8c6420a8: *stop=0x8c6420aau;r[12]=r[14];goto L8c642108; /* bra 0x8c642108 */
L8c642100: *stop=0x8c642100u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642102; /* mov.l 0x8c6421ec,r2 */
L8c642102: *stop=0x8c642102u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642104; /* mov.l @r2,r3 */
L8c642104: *stop=0x8c642104u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c642106; /* tst r3,r3 */
L8c642106: 
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c642106u;if(!(*sr&1u) && ++guard_visits[2]>=2u)return -4;
#endif
 if(!(*sr&1u))goto L8c642098;goto L8c642108; /* bf 0x8c642098 */
L8c642108: *stop=0x8c642108u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c64210a; /* tst r12,r12 */
L8c64210a: if((*sr&1u))goto L8c642112;goto L8c64210c; /* bt 0x8c642112 */
L8c64210c: *stop=0x8c64210cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64210e; /* mov.l 0x8c6421f0,r2 */
L8c64210e: target=r[2];*pr=0x8c642112u;*stop=0x8c642110u;r[4]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642112; /* jsr @r2 */
L8c642112: *stop=0x8c642112u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642114; /* mov.l 0x8c6421f4,r3 */
L8c642114: *stop=0x8c642114u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642116; /* mov.l @r3,r2 */
L8c642116: *stop=0x8c642116u;r[2]+=1u;goto L8c642118; /* add #0x1,r2 */
L8c642118: *stop=0x8c642118u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64211a; /* mov.l r2,@r3 */
L8c64211a: *stop=0x8c64211au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64211c; /* mov.l @r3,r0 */
L8c64211c: *stop=0x8c64211cu;r[0]&=0x3u;goto L8c64211e; /* and #0x3,r0 */
L8c64211e: *stop=0x8c64211eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c642120; /* mov.l r0,@r3 */
L8c642120: *stop=0x8c642120u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642122; /* mov.l @r3,r2 */
L8c642122: *stop=0x8c642122u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421f8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642124; /* mov.l 0x8c6421f8,r0 */
L8c642124: *stop=0x8c642124u;r[2]<<=2;goto L8c642126; /* shll2 r2 */
L8c642126: *stop=0x8c642126u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[13],4);if(io_status)return io_status;goto L8c642128; /* mov.l r13,@(r0,r2) */
L8c642128: *stop=0x8c642128u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421fcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64212a; /* mov.l 0x8c6421fc,r2 */
L8c64212a: *stop=0x8c64212au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64212c; /* mov.l @r2,r0 */
L8c64212c: *stop=0x8c64212cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64212e; /* cmp/eq #0x1,r0 */
L8c64212e: if((*sr&1u))goto L8c642138;goto L8c642130; /* bt 0x8c642138 */
L8c642130: *stop=0x8c642130u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642200u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c642132; /* mov.l 0x8c642200,r1 */
L8c642132: *stop=0x8c642132u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642134; /* mov.l @r1,r0 */
L8c642134: *stop=0x8c642134u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c642136; /* cmp/eq #0x1,r0 */
L8c642136: if(!(*sr&1u))goto L8c6421c0;goto L8c642138; /* bf 0x8c6421c0 */
L8c642138: *stop=0x8c642138u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64213a; /* mov.l 0x8c6421f4,r3 */
L8c64213a: *stop=0x8c64213au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64213c; /* mov.l @r3,r2 */
L8c64213c: *stop=0x8c64213cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642204u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64213e; /* mov.l 0x8c642204,r13 */
L8c64213e: *stop=0x8c64213eu;r[2]&=r[14];goto L8c642140; /* and r14,r2 */
L8c642140: *stop=0x8c642140u;r[5]=1u;goto L8c642142; /* mov #0x1,r5 */
L8c642142: *stop=0x8c642142u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c642144; /* mov.l r2,@(0x4,r15) */
L8c642144: target=r[13];*pr=0x8c642148u;*stop=0x8c642146u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642148; /* jsr @r13 */
L8c642148: *stop=0x8c642148u;r[5]=0u;goto L8c64214a; /* mov #0x0,r5 */
L8c64214a: target=r[13];*pr=0x8c64214eu;*stop=0x8c64214cu;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64214e; /* jsr @r13 */
L8c64214e: *stop=0x8c64214eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c642150; /* mov.l @(0x4,r15),r12 */
L8c642150: *stop=0x8c642150u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642208u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642152; /* mov.l 0x8c642208,r0 */
L8c642152: *stop=0x8c642152u;r[12]<<=2;goto L8c642154; /* shll2 r12 */
L8c642154: *stop=0x8c642154u;r[4]=sx(m,bus,&io_status,0x8c6421e0u,2);if(io_status)return io_status;goto L8c642156; /* mov.w 0x8c6421e0,r4 */
L8c642156: *stop=0x8c642156u;r[12]<<=2;goto L8c642158; /* shll2 r12 */
L8c642158: *stop=0x8c642158u;*sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c64215a; /* shll r12 */
L8c64215a: target=r[13];*pr=0x8c64215eu;*stop=0x8c64215cu;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64215e; /* jsr @r13 */
L8c64215e: *stop=0x8c64215eu;r[4]=sx(m,bus,&io_status,0x8c6421e2u,2);if(io_status)return io_status;goto L8c642160; /* mov.w 0x8c6421e2,r4 */
L8c642160: *stop=0x8c642160u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64220cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642162; /* mov.l 0x8c64220c,r0 */
L8c642162: target=r[13];*pr=0x8c642166u;*stop=0x8c642164u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642166; /* jsr @r13 */
L8c642166: *stop=0x8c642166u;r[4]=sx(m,bus,&io_status,0x8c6421e4u,2);if(io_status)return io_status;goto L8c642168; /* mov.w 0x8c6421e4,r4 */
L8c642168: *stop=0x8c642168u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642210u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64216a; /* mov.l 0x8c642210,r0 */
L8c64216a: target=r[13];*pr=0x8c64216eu;*stop=0x8c64216cu;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64216e; /* jsr @r13 */
L8c64216e: *stop=0x8c64216eu;r[4]=sx(m,bus,&io_status,0x8c6421e6u,2);if(io_status)return io_status;goto L8c642170; /* mov.w 0x8c6421e6,r4 */
L8c642170: *stop=0x8c642170u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642214u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642172; /* mov.l 0x8c642214,r0 */
L8c642172: target=r[13];*pr=0x8c642176u;*stop=0x8c642174u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642176; /* jsr @r13 */
L8c642176: *stop=0x8c642176u;r[4]=sx(m,bus,&io_status,0x8c6421e8u,2);if(io_status)return io_status;goto L8c642178; /* mov.w 0x8c6421e8,r4 */
L8c642178: *stop=0x8c642178u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642218u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64217a; /* mov.l 0x8c642218,r0 */
L8c64217a: target=r[13];*pr=0x8c64217eu;*stop=0x8c64217cu;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64217e; /* jsr @r13 */
L8c64217e: *stop=0x8c64217eu;r[4]=sx(m,bus,&io_status,0x8c6421eau,2);if(io_status)return io_status;goto L8c642180; /* mov.w 0x8c6421ea,r4 */
L8c642180: *stop=0x8c642180u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64221cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c642182; /* mov.l 0x8c64221c,r5 */
L8c642182: target=r[13];*pr=0x8c642186u;*stop=0x8c642184u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642186; /* jsr @r13 */
L8c642186: *stop=0x8c642186u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642220u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642188; /* mov.l 0x8c642220,r3 */
L8c642188: *stop=0x8c642188u;r[4]=sx(m,bus,&io_status,0x8c6421eau,2);if(io_status)return io_status;goto L8c64218a; /* mov.w 0x8c6421ea,r4 */
L8c64218a: target=r[3];*pr=0x8c64218eu;*stop=0x8c64218cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64218e; /* jsr @r3 */
L8c64218e: *stop=0x8c64218eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642224u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642190; /* mov.l 0x8c642224,r3 */
L8c642190: *stop=0x8c642190u;r[2]=0u;goto L8c642192; /* mov #0x0,r2 */
L8c642192: *stop=0x8c642192u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c642194; /* mov.l r2,@r3 */
L8c642194: *stop=0x8c642194u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642228u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642196; /* mov.l 0x8c642228,r0 */
L8c642196: *stop=0x8c642196u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c642198; /* mov.l @r0,r1 */
L8c642198: *stop=0x8c642198u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64222cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64219a; /* mov.l 0x8c64222c,r2 */
L8c64219a: *stop=0x8c64219au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c64219c; /* mov.l r1,@r2 */
L8c64219c: *stop=0x8c64219cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642200u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64219e; /* mov.l 0x8c642200,r1 */
L8c64219e: *stop=0x8c64219eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6421a0; /* mov.l @r1,r3 */
L8c6421a0: *stop=0x8c6421a0u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6421a2; /* tst r3,r3 */
L8c6421a2: if((*sr&1u))goto L8c6421c0;goto L8c6421a4; /* bt 0x8c6421c0 */
L8c6421a4: *stop=0x8c6421a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421f4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6421a6; /* mov.l 0x8c6421f4,r2 */
L8c6421a6: *stop=0x8c6421a6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6421a8; /* mov.l @r2,r3 */
L8c6421a8: *stop=0x8c6421a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642230u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6421aa; /* mov.l 0x8c642230,r0 */
L8c6421aa: *stop=0x8c6421aau;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c6421ac; /* mov.l r3,@r0 */
L8c6421ac: *stop=0x8c6421acu;r[3]=r[2];goto L8c6421ae; /* mov r2,r3 */
L8c6421ae: *stop=0x8c6421aeu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6421b0; /* mov.l @r3,r1 */
L8c6421b0: *stop=0x8c6421b0u;r[2]=2u;goto L8c6421b2; /* mov #0x2,r2 */
L8c6421b2: *stop=0x8c6421b2u;r[1]<<=2;goto L8c6421b4; /* shll2 r1 */
L8c6421b4: *stop=0x8c6421b4u;r[0]+=4294967272u;goto L8c6421b6; /* add #-0x18,r0 */
L8c6421b6: *stop=0x8c6421b6u;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[2],4);if(io_status)return io_status;goto L8c6421b8; /* mov.l r2,@(r0,r1) */
L8c6421b8: *stop=0x8c6421b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642234u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6421ba; /* mov.l 0x8c642234,r1 */
L8c6421ba: *stop=0x8c6421bau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c6421bc; /* mov.l r14,@r1 */
L8c6421bc: *stop=0x8c6421bcu;r[3]+=32u;goto L8c6421be; /* add #0x20,r3 */
L8c6421be: *stop=0x8c6421beu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6421c0; /* mov.l r14,@r3 */
L8c6421c0: *stop=0x8c6421c0u;r[0]=0u;goto L8c6421c2; /* mov #0x0,r0 */
L8c6421c2: *stop=0x8c6421c2u;r[15]+=8u;goto L8c6421c4; /* add #0x8,r15 */
L8c6421c4: *stop=0x8c6421c4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6421c6; /* lds.l @r15+,PR */
L8c6421c6: *stop=0x8c6421c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6421c8; /* mov.l @r15+,r8 */
L8c6421c8: *stop=0x8c6421c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6421ca; /* mov.l @r15+,r9 */
L8c6421ca: *stop=0x8c6421cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6421cc; /* mov.l @r15+,r10 */
L8c6421cc: *stop=0x8c6421ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6421ce; /* mov.l @r15+,r11 */
L8c6421ce: *stop=0x8c6421ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6421d0; /* mov.l @r15+,r12 */
L8c6421d0: *stop=0x8c6421d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6421d2; /* mov.l @r15+,r13 */
L8c6421d2: *stop=0x8c6421d2u;if(*pr!=entry_pr)return -3;*stop=0x8c6421d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_submit_8c641e00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 unsigned guard_visits[2]={0};(void)guard_visits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c641e00;
L8c641e00: *stop=0x8c641e00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c641e02; /* mov.l r14,@-r15 */
L8c641e02: *stop=0x8c641e02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c641e04; /* mov.l r13,@-r15 */
L8c641e04: *stop=0x8c641e04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c641e06; /* mov.l r12,@-r15 */
L8c641e06: *stop=0x8c641e06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c641e08; /* mov.l r11,@-r15 */
L8c641e08: *stop=0x8c641e08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c641e0a; /* mov.l r10,@-r15 */
L8c641e0a: *stop=0x8c641e0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c641e0c; /* mov.l r9,@-r15 */
L8c641e0c: *stop=0x8c641e0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c641e0e; /* mov.l r8,@-r15 */
L8c641e0e: *stop=0x8c641e0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c641e10; /* sts.l PR,@-r15 */
L8c641e10: *stop=0x8c641e10u;r[15]+=4294967288u;goto L8c641e12; /* add #-0x8,r15 */
L8c641e12: *stop=0x8c641e12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ec4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641e14; /* mov.l 0x8c641ec4,r3 */
L8c641e14: *stop=0x8c641e14u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c641e16; /* mov.l r4,@(0x4,r15) */
L8c641e16: *stop=0x8c641e16u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c641e18; /* mov.l @r3,r4 */
L8c641e18: *stop=0x8c641e18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ec8u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c641e1a; /* mov.l 0x8c641ec8,r9 */
L8c641e1a: *stop=0x8c641e1au;r[5]=r[4];goto L8c641e1c; /* mov r4,r5 */
L8c641e1c: *stop=0x8c641e1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641eccu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c641e1e; /* mov.l 0x8c641ecc,r11 */
L8c641e1e: *stop=0x8c641e1eu;r[5]<<=2;goto L8c641e20; /* shll2 r5 */
L8c641e20: *stop=0x8c641e20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ed0u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c641e22; /* mov.l 0x8c641ed0,r8 */
L8c641e22: *stop=0x8c641e22u;r[14]=0u;goto L8c641e24; /* mov #0x0,r14 */
L8c641e24: *stop=0x8c641e24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ed8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c641e26; /* mov.l 0x8c641ed8,r12 */
L8c641e26: *stop=0x8c641e26u;r[9]+=r[5];goto L8c641e28; /* add r5,r9 */
L8c641e28: *stop=0x8c641e28u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ed4u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c641e2a; /* mov.l 0x8c641ed4,r10 */
L8c641e2a: *stop=0x8c641e2au;r[11]+=r[5];goto L8c641e2c; /* add r5,r11 */
L8c641e2c: *stop=0x8c641e2cu;r[13]=sx(m,bus,&io_status,0x8c641ec0u,2);if(io_status)return io_status;goto L8c641e2e; /* mov.w 0x8c641ec0,r13 */
L8c641e2e: *stop=0x8c641e2eu;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c641e30; /* mov.l @r11,r2 */
L8c641e30: *stop=0x8c641e30u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c641e32; /* tst r2,r2 */
L8c641e32: if((*sr&1u))goto L8c641e92;goto L8c641e34; /* bt 0x8c641e92 */
L8c641e34: *stop=0x8c641e34u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641e36; /* mov.l @(0x4,r15),r3 */
L8c641e36: *stop=0x8c641e36u;*sr=(*sr&~1u)|(r[14]==r[3]);goto L8c641e38; /* cmp/eq r14,r3 */
L8c641e38: if((*sr&1u))goto L8c641e92;goto L8c641e3a; /* bt 0x8c641e92 */
L8c641e3a: *stop=0x8c641e3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641edcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641e3c; /* mov.l 0x8c641edc,r1 */
L8c641e3c: *stop=0x8c641e3cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c641e3e; /* mov.l @r1,r0 */
L8c641e3e: *stop=0x8c641e3eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c641e40; /* cmp/eq #0x1,r0 */
L8c641e40: if(!(*sr&1u))goto L8c641e7c;goto L8c641e42; /* bf 0x8c641e7c */
L8c641e42: *stop=0x8c641e42u;r[0]=r[14];goto L8c641e44; /* mov r14,r0 */
L8c641e44: *stop=0x8c641e44u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c641e46; /* cmp/eq #0x1,r0 */
L8c641e46: if((*sr&1u))goto L8c641e4e;goto L8c641e48; /* bt 0x8c641e4e */
L8c641e48: *stop=0x8c641e48u;r[0]=r[14];goto L8c641e4a; /* mov r14,r0 */
L8c641e4a: *stop=0x8c641e4au;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c641e4c; /* cmp/eq #0x3,r0 */
L8c641e4c: if(!(*sr&1u))goto L8c641e7c;goto L8c641e4e; /* bf 0x8c641e7c */
L8c641e4e: *stop=0x8c641e4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ee0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641e50; /* mov.l 0x8c641ee0,r3 */
L8c641e50: *stop=0x8c641e50u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c641e52; /* mov.l @r3,r2 */
L8c641e52: *stop=0x8c641e52u;r[0]=r[3];goto L8c641e54; /* mov r3,r0 */
L8c641e54: *stop=0x8c641e54u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c641e56; /* mov.l @r0,r1 */
L8c641e56: *stop=0x8c641e56u;r[3]=r[14];goto L8c641e58; /* mov r14,r3 */
L8c641e58: *stop=0x8c641e58u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c641e5a; /* mov.l @r1,r1 */
L8c641e5a: *stop=0x8c641e5au;r[3]<<=16;goto L8c641e5c; /* shll16 r3 */
L8c641e5c: *stop=0x8c641e5cu;r[3]<<=8;goto L8c641e5e; /* shll8 r3 */
L8c641e5e: *stop=0x8c641e5eu;r[1]&=r[8];goto L8c641e60; /* and r8,r1 */
L8c641e60: *stop=0x8c641e60u;r[1]|=r[3];goto L8c641e62; /* or r3,r1 */
L8c641e62: *stop=0x8c641e62u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c641e64; /* mov.l r1,@r2 */
L8c641e64: *stop=0x8c641e64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ee4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641e66; /* mov.l 0x8c641ee4,r2 */
L8c641e66: *stop=0x8c641e66u;r[3]=r[15];goto L8c641e68; /* mov r15,r3 */
L8c641e68: *stop=0x8c641e68u;r[7]=16u;goto L8c641e6a; /* mov #0x10,r7 */
L8c641e6a: *stop=0x8c641e6au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c641e6c; /* mov.l r3,@-r15 */
L8c641e6c: *stop=0x8c641e6cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[6]=loaded;}goto L8c641e6e; /* mov.l @r2,r6 */
L8c641e6e: *stop=0x8c641e6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641ee0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641e70; /* mov.l 0x8c641ee0,r3 */
L8c641e70: *stop=0x8c641e70u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c641e72; /* mov.l @r3,r5 */
L8c641e72: target=r[10];*pr=0x8c641e76u;*stop=0x8c641e74u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641e76; /* jsr @r10 */
L8c641e76: *stop=0x8c641e76u;*sr=(*sr&~1u)|(r[13]==r[0]);goto L8c641e78; /* cmp/eq r13,r0 */
L8c641e78: 
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c641e78u;if((*sr&1u) && ++guard_visits[0]>=2u)return -4;
#endif
 take=(*sr&1u);*stop=0x8c641e7au;r[15]+=4u;if(take)goto L8c641e64;goto L8c641e7c; /* bt/s 0x8c641e64 */
L8c641e7c: *stop=0x8c641e7cu;r[2]=r[15];goto L8c641e7e; /* mov r15,r2 */
L8c641e7e: *stop=0x8c641e7eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c641e80; /* mov.l r2,@-r15 */
L8c641e80: *stop=0x8c641e80u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[6]=loaded;}goto L8c641e82; /* mov.l @r11,r6 */
L8c641e82: *stop=0x8c641e82u;r[7]=16u;goto L8c641e84; /* mov #0x10,r7 */
L8c641e84: *stop=0x8c641e84u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8c641e86; /* mov.l @r9,r5 */
L8c641e86: *stop=0x8c641e86u;r[6]+=32u;goto L8c641e88; /* add #0x20,r6 */
L8c641e88: target=r[10];*pr=0x8c641e8cu;*stop=0x8c641e8au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641e8c; /* jsr @r10 */
L8c641e8c: *stop=0x8c641e8cu;*sr=(*sr&~1u)|(r[13]==r[0]);goto L8c641e8e; /* cmp/eq r13,r0 */
L8c641e8e: 
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c641e8eu;if((*sr&1u) && ++guard_visits[1]>=2u)return -4;
#endif
 take=(*sr&1u);*stop=0x8c641e90u;r[15]+=4u;if(take)goto L8c641e7c;goto L8c641e92; /* bt/s 0x8c641e7c */
L8c641e92: *stop=0x8c641e92u;r[2]=5u;goto L8c641e94; /* mov #0x5,r2 */
L8c641e94: *stop=0x8c641e94u;r[14]+=1u;goto L8c641e96; /* add #0x1,r14 */
L8c641e96: *stop=0x8c641e96u;*sr=(*sr&~1u)|(r[14]>=r[2]);goto L8c641e98; /* cmp/hs r2,r14 */
L8c641e98: *stop=0x8c641e98u;r[11]+=16u;goto L8c641e9a; /* add #0x10,r11 */
L8c641e9a: take=!(*sr&1u);*stop=0x8c641e9cu;r[9]+=16u;if(take)goto L8c641e2e;goto L8c641e9e; /* bf/s 0x8c641e2e */
L8c641e9e: *stop=0x8c641e9eu;r[15]+=8u;goto L8c641ea0; /* add #0x8,r15 */
L8c641ea0: *stop=0x8c641ea0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c641ea2; /* lds.l @r15+,PR */
L8c641ea2: *stop=0x8c641ea2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c641ea4; /* mov.l @r15+,r8 */
L8c641ea4: *stop=0x8c641ea4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c641ea6; /* mov.l @r15+,r9 */
L8c641ea6: *stop=0x8c641ea6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c641ea8; /* mov.l @r15+,r10 */
L8c641ea8: *stop=0x8c641ea8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c641eaa; /* mov.l @r15+,r11 */
L8c641eaa: *stop=0x8c641eaau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c641eac; /* mov.l @r15+,r12 */
L8c641eac: *stop=0x8c641eacu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c641eae; /* mov.l @r15+,r13 */
L8c641eae: *stop=0x8c641eaeu;if(*pr!=entry_pr)return -3;*stop=0x8c641eb0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_submit_8c650bc8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c650bc8;
L8c650bc8: *stop=0x8c650bc8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c650bca; /* mov.l r14,@-r15 */
L8c650bca: *stop=0x8c650bcau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c650bcc; /* mov.l r13,@-r15 */
L8c650bcc: *stop=0x8c650bccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c650bce; /* mov.l r12,@-r15 */
L8c650bce: *stop=0x8c650bceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c650bd0; /* mov.l r11,@-r15 */
L8c650bd0: *stop=0x8c650bd0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c650bd2; /* mov.l r10,@-r15 */
L8c650bd2: *stop=0x8c650bd2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c650bd4; /* mov.l r9,@-r15 */
L8c650bd4: *stop=0x8c650bd4u;r[15]+=4294967288u;goto L8c650bd6; /* add #-0x8,r15 */
L8c650bd6: *stop=0x8c650bd6u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c650bd8; /* stc SR,r0 */
L8c650bd8: *stop=0x8c650bd8u;r[3]=sx(m,bus,&io_status,0x8c650c22u,2);if(io_status)return io_status;goto L8c650bda; /* mov.w 0x8c650c22,r3 */
L8c650bda: *stop=0x8c650bdau;r[0]>>=2;goto L8c650bdc; /* shlr2 r0 */
L8c650bdc: *stop=0x8c650bdcu;r[0]>>=2;goto L8c650bde; /* shlr2 r0 */
L8c650bde: *stop=0x8c650bdeu;r[0]&=0xfu;goto L8c650be0; /* and #0xf,r0 */
L8c650be0: *stop=0x8c650be0u;r[9]=r[0];goto L8c650be2; /* mov r0,r9 */
L8c650be2: *stop=0x8c650be2u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c650be4; /* stc SR,r0 */
L8c650be4: *stop=0x8c650be4u;r[0]&=r[3];goto L8c650be6; /* and r3,r0 */
L8c650be6: *stop=0x8c650be6u;r[0]|=0xe0u;goto L8c650be8; /* or #0xe0,r0 */
L8c650be8: *stop=0x8c650be8u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c650bea; /* ldc r0,SR */
L8c650bea: *stop=0x8c650beau;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c650bec; /* tst r6,r6 */
L8c650bec: if(!(*sr&1u))goto L8c650c60;goto L8c650bee; /* bf 0x8c650c60 */
L8c650bee: *stop=0x8c650beeu;status=sa_sh4_stc_sr(*sr,&r[1],0u);if(status)return status;goto L8c650bf0; /* stc SR,r1 */
L8c650bf0: *stop=0x8c650bf0u;r[0]=r[9];goto L8c650bf2; /* mov r9,r0 */
L8c650bf2: *stop=0x8c650bf2u;r[2]=sx(m,bus,&io_status,0x8c650c22u,2);if(io_status)return io_status;goto L8c650bf4; /* mov.w 0x8c650c22,r2 */
L8c650bf4: *stop=0x8c650bf4u;r[0]&=0xfu;goto L8c650bf6; /* and #0xf,r0 */
L8c650bf6: *stop=0x8c650bf6u;r[0]<<=2;goto L8c650bf8; /* shll2 r0 */
L8c650bf8: *stop=0x8c650bf8u;r[1]&=r[2];goto L8c650bfa; /* and r2,r1 */
L8c650bfa: *stop=0x8c650bfau;r[0]<<=2;goto L8c650bfc; /* shll2 r0 */
L8c650bfc: *stop=0x8c650bfcu;r[0]|=r[1];goto L8c650bfe; /* or r1,r0 */
L8c650bfe: *stop=0x8c650bfeu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c650c00; /* ldc r0,SR */
L8c650c00: *stop=0x8c650c00u;r[0]=sx(m,bus,&io_status,0x8c650c24u,2);if(io_status)return io_status;goto L8c650c02; /* mov.w 0x8c650c24,r0 */
L8c650c02: *stop=0x8c650c04u;goto L8c650dd8; /* bra 0x8c650dd8 */
L8c650c60: *stop=0x8c650c60u;r[10]=8u;goto L8c650c62; /* mov #0x8,r10 */
L8c650c62: *stop=0x8c650c62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650d04u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650c64; /* mov.l 0x8c650d04,r0 */
L8c650c64: *stop=0x8c650c64u;r[10]&=r[7];goto L8c650c66; /* and r7,r10 */
L8c650c66: *stop=0x8c650c66u;r[12]=r[5];goto L8c650c68; /* mov r5,r12 */
L8c650c68: *stop=0x8c650c68u;r[10]>>=2;goto L8c650c6a; /* shlr2 r10 */
L8c650c6a: *stop=0x8c650c6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c650d08u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c650c6c; /* mov.l 0x8c650d08,r11 */
L8c650c6c: *stop=0x8c650c6cu;*sr=(*sr&~1u)|(r[10]&1u);r[10]>>=1;goto L8c650c6e; /* shlr r10 */
L8c650c6e: *stop=0x8c650c6eu;r[14]=sx(m,bus,&io_status,0x8c650d00u,2);if(io_status)return io_status;goto L8c650c70; /* mov.w 0x8c650d00,r14 */
L8c650c70: *stop=0x8c650c70u;r[2]=r[10];goto L8c650c72; /* mov r10,r2 */
L8c650c72: *stop=0x8c650c72u;r[2]<<=2;goto L8c650c74; /* shll2 r2 */
L8c650c74: *stop=0x8c650c74u;r[5]=r[10];goto L8c650c76; /* mov r10,r5 */
L8c650c76: *stop=0x8c650c76u;r[5]<<=8;goto L8c650c78; /* shll8 r5 */
L8c650c78: *stop=0x8c650c78u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c650c7a; /* mov.l r2,@(0x4,r15) */
L8c650c7a: *stop=0x8c650c7au;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c650c7c; /* mov.l @(r0,r2),r3 */
L8c650c7c: *stop=0x8c650c7cu;r[0]=r[5];goto L8c650c7e; /* mov r5,r0 */
L8c650c7e: *stop=0x8c650c7eu;r[0]+=r[11];goto L8c650c80; /* add r11,r0 */
L8c650c80: *stop=0x8c650c80u;r[13]=r[3];goto L8c650c82; /* mov r3,r13 */
L8c650c82: *stop=0x8c650c82u;r[13]<<=2;goto L8c650c84; /* shll2 r13 */
L8c650c84: *stop=0x8c650c84u;r[13]<<=2;goto L8c650c86; /* shll2 r13 */
L8c650c86: *stop=0x8c650c86u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c650c88; /* mov.l r3,@r15 */
L8c650c88: *stop=0x8c650c88u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c650c8a; /* shll r13 */
L8c650c8a: *stop=0x8c650c8au;r[0]+=r[13];goto L8c650c8c; /* add r13,r0 */
L8c650c8c: *stop=0x8c650c8cu;if(!valid_access(r[0]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650c8e; /* mov.l @(0x18,r0),r0 */
L8c650c8e: *stop=0x8c650c8eu;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c650c90; /* tst #0x3,r0 */
L8c650c90: if(!(*sr&1u))goto L8c650cdc;goto L8c650c92; /* bf 0x8c650cdc */
L8c650c92: *stop=0x8c650c92u;r[2]=31u;goto L8c650c94; /* mov #0x1f,r2 */
L8c650c94: *stop=0x8c650c94u;*sr=(*sr&~1u)|!(r[12]&r[2]);goto L8c650c96; /* tst r12,r2 */
L8c650c96: if((*sr&1u))goto L8c650c9e;goto L8c650c98; /* bt 0x8c650c9e */
L8c650c98: *stop=0x8c650c98u;r[2]=4294967264u;goto L8c650c9a; /* mov #-0x20,r2 */
L8c650c9a: *stop=0x8c650c9au;r[12]+=31u;goto L8c650c9c; /* add #0x1f,r12 */
L8c650c9c: *stop=0x8c650c9cu;r[12]&=r[2];goto L8c650c9e; /* and r2,r12 */
L8c650c9e: *stop=0x8c650c9eu;r[3]=31u;goto L8c650ca0; /* mov #0x1f,r3 */
L8c650ca0: *stop=0x8c650ca0u;*sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c650ca2; /* tst r6,r3 */
L8c650ca2: if((*sr&1u))goto L8c650caa;goto L8c650ca4; /* bt 0x8c650caa */
L8c650ca4: *stop=0x8c650ca4u;r[3]=4294967264u;goto L8c650ca6; /* mov #-0x20,r3 */
L8c650ca6: *stop=0x8c650ca6u;r[6]+=31u;goto L8c650ca8; /* add #0x1f,r6 */
L8c650ca8: *stop=0x8c650ca8u;r[6]&=r[3];goto L8c650caa; /* and r3,r6 */
L8c650caa: *stop=0x8c650caau;{uint32_t loaded=rd(m,bus,&io_status,0x8c650d0cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650cac; /* mov.l 0x8c650d0c,r1 */
L8c650cac: *stop=0x8c650cacu;r[14]=r[5];goto L8c650cae; /* mov r5,r14 */
L8c650cae: *stop=0x8c650caeu;r[14]+=r[11];goto L8c650cb0; /* add r11,r14 */
L8c650cb0: *stop=0x8c650cb0u;r[0]=r[7];goto L8c650cb2; /* mov r7,r0 */
L8c650cb2: *stop=0x8c650cb2u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c650cb4; /* mov.l @r1,r3 */
L8c650cb4: *stop=0x8c650cb4u;r[14]+=r[13];goto L8c650cb6; /* add r13,r14 */
L8c650cb6: *stop=0x8c650cb6u;r[0]|=0x1u;goto L8c650cb8; /* or #0x1,r0 */
L8c650cb8: *stop=0x8c650cb8u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c650cba; /* mov.l r3,@r14 */
L8c650cba: *stop=0x8c650cbau;{uint32_t loaded=rd(m,bus,&io_status,0x8c650d04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650cbc; /* mov.l 0x8c650d04,r3 */
L8c650cbc: *stop=0x8c650cbcu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[12],4);if(io_status)return io_status;goto L8c650cbe; /* mov.l r12,@(0x4,r14) */
L8c650cbe: *stop=0x8c650cbeu;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[4],4);if(io_status)return io_status;goto L8c650cc0; /* mov.l r4,@(0xc,r14) */
L8c650cc0: *stop=0x8c650cc0u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[6],4);if(io_status)return io_status;goto L8c650cc2; /* mov.l r6,@(0x14,r14) */
L8c650cc2: *stop=0x8c650cc2u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[0],4);if(io_status)return io_status;goto L8c650cc4; /* mov.l r0,@(0x18,r14) */
L8c650cc4: *stop=0x8c650cc4u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650cc6; /* mov.l @(0x20,r15),r2 */
L8c650cc6: *stop=0x8c650cc6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c650cc8; /* mov.l r14,@r2 */
L8c650cc8: *stop=0x8c650cc8u;r[14]=0u;goto L8c650cca; /* mov #0x0,r14 */
L8c650cca: *stop=0x8c650ccau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c650ccc; /* mov.l @(0x4,r15),r4 */
L8c650ccc: *stop=0x8c650cccu;r[4]+=r[3];goto L8c650cce; /* add r3,r4 */
L8c650cce: *stop=0x8c650cceu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c650cd0; /* mov.l @r4,r0 */
L8c650cd0: *stop=0x8c650cd0u;r[0]+=1u;goto L8c650cd2; /* add #0x1,r0 */
L8c650cd2: *stop=0x8c650cd2u;r[0]&=0x7u;goto L8c650cd4; /* and #0x7,r0 */
L8c650cd4: *stop=0x8c650cd4u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c650cd6; /* mov.l r0,@r4 */
L8c650cd6: *stop=0x8c650cd6u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c650cd8; /* mov.l @r1,r2 */
L8c650cd8: *stop=0x8c650cd8u;r[2]+=1u;goto L8c650cda; /* add #0x1,r2 */
L8c650cda: *stop=0x8c650cdau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c650cdc; /* mov.l r2,@r1 */
L8c650cdc: *stop=0x8c650cdcu;r[3]=sx(m,bus,&io_status,0x8c650d00u,2);if(io_status)return io_status;goto L8c650cde; /* mov.w 0x8c650d00,r3 */
L8c650cde: *stop=0x8c650cdeu;*sr=(*sr&~1u)|(r[3]==r[14]);goto L8c650ce0; /* cmp/eq r3,r14 */
L8c650ce0: if(!(*sr&1u))goto L8c650d20;goto L8c650ce2; /* bf 0x8c650d20 */
L8c650ce2: *stop=0x8c650ce2u;status=sa_sh4_stc_sr(*sr,&r[1],0u);if(status)return status;goto L8c650ce4; /* stc SR,r1 */
L8c650ce4: *stop=0x8c650ce4u;r[0]=r[9];goto L8c650ce6; /* mov r9,r0 */
L8c650ce6: *stop=0x8c650ce6u;r[2]=sx(m,bus,&io_status,0x8c650d02u,2);if(io_status)return io_status;goto L8c650ce8; /* mov.w 0x8c650d02,r2 */
L8c650ce8: *stop=0x8c650ce8u;r[0]&=0xfu;goto L8c650cea; /* and #0xf,r0 */
L8c650cea: *stop=0x8c650ceau;r[0]<<=2;goto L8c650cec; /* shll2 r0 */
L8c650cec: *stop=0x8c650cecu;r[1]&=r[2];goto L8c650cee; /* and r2,r1 */
L8c650cee: *stop=0x8c650ceeu;r[0]<<=2;goto L8c650cf0; /* shll2 r0 */
L8c650cf0: *stop=0x8c650cf0u;r[0]|=r[1];goto L8c650cf2; /* or r1,r0 */
L8c650cf2: *stop=0x8c650cf4u;goto L8c650dd4; /* bra 0x8c650dd4 */
L8c650d20: *stop=0x8c650d20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e0cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650d22; /* mov.l 0x8c650e0c,r1 */
L8c650d22: *stop=0x8c650d22u;r[4]=r[11];goto L8c650d24; /* mov r11,r4 */
L8c650d24: *stop=0x8c650d24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e08u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650d26; /* mov.l 0x8c650e08,r3 */
L8c650d26: *stop=0x8c650d26u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c650d28; /* mov.l @r1,r2 */
L8c650d28: *stop=0x8c650d28u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c650d2a; /* mov.l @r3,r0 */
L8c650d2a: *stop=0x8c650d2au;r[2]<<=2;goto L8c650d2c; /* shll2 r2 */
L8c650d2c: *stop=0x8c650d2cu;r[0]<<=8;goto L8c650d2e; /* shll8 r0 */
L8c650d2e: *stop=0x8c650d2eu;r[2]<<=2;goto L8c650d30; /* shll2 r2 */
L8c650d30: *stop=0x8c650d30u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c650d32; /* shll r2 */
L8c650d32: *stop=0x8c650d32u;r[0]+=r[4];goto L8c650d34; /* add r4,r0 */
L8c650d34: *stop=0x8c650d34u;r[0]+=r[2];goto L8c650d36; /* add r2,r0 */
L8c650d36: *stop=0x8c650d36u;if(!valid_access(r[0]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650d38; /* mov.l @(0x18,r0),r0 */
L8c650d38: *stop=0x8c650d38u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c650d3a; /* tst #0x3,r0 */
L8c650d3a: if(!(*sr&1u))goto L8c650dc4;goto L8c650d3c; /* bf 0x8c650dc4 */
L8c650d3c: *stop=0x8c650d3cu;r[0]=r[5];goto L8c650d3e; /* mov r5,r0 */
L8c650d3e: *stop=0x8c650d3eu;r[0]+=r[4];goto L8c650d40; /* add r4,r0 */
L8c650d40: *stop=0x8c650d40u;r[0]+=r[13];goto L8c650d42; /* add r13,r0 */
L8c650d42: *stop=0x8c650d42u;if(!valid_access(r[0]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650d44; /* mov.l @(0x18,r0),r0 */
L8c650d44: *stop=0x8c650d44u;r[0]&=0x3u;goto L8c650d46; /* and #0x3,r0 */
L8c650d46: *stop=0x8c650d46u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c650d48; /* cmp/eq #0x1,r0 */
L8c650d48: if(!(*sr&1u))goto L8c650dc4;goto L8c650d4a; /* bf 0x8c650dc4 */
L8c650d4a: *stop=0x8c650d4au;r[7]=r[5];goto L8c650d4c; /* mov r5,r7 */
L8c650d4c: *stop=0x8c650d4cu;r[7]+=r[4];goto L8c650d4e; /* add r4,r7 */
L8c650d4e: *stop=0x8c650d4eu;r[7]+=r[13];goto L8c650d50; /* add r13,r7 */
L8c650d50: *stop=0x8c650d50u;if(!valid_access(r[7]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650d52; /* mov.l @(0x18,r7),r0 */
L8c650d52: *stop=0x8c650d52u;r[2]=4294967292u;goto L8c650d54; /* mov #-0x4,r2 */
L8c650d54: *stop=0x8c650d54u;r[0]&=r[2];goto L8c650d56; /* and r2,r0 */
L8c650d56: *stop=0x8c650d56u;r[0]|=0x2u;goto L8c650d58; /* or #0x2,r0 */
L8c650d58: *stop=0x8c650d58u;if(!valid_access(r[7]+24u,4u))return -3;wr(m,bus,&io_status,r[7]+0x18u,r[0],4);if(io_status)return io_status;goto L8c650d5a; /* mov.l r0,@(0x18,r7) */
L8c650d5a: *stop=0x8c650d5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650d5c; /* mov.l 0x8c650e10,r0 */
L8c650d5c: *stop=0x8c650d5cu;if(!valid_access(r[7]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650d5e; /* mov.l @(0x4,r7),r3 */
L8c650d5e: *stop=0x8c650d5eu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c650d60; /* mov.l r3,@r0 */
L8c650d60: *stop=0x8c650d60u;if(!valid_access(r[7]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650d62; /* mov.l @(0xc,r7),r1 */
L8c650d62: *stop=0x8c650d62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650d64; /* mov.l 0x8c650e14,r3 */
L8c650d64: *stop=0x8c650d64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650d66; /* mov.l 0x8c650e18,r2 */
L8c650d66: *stop=0x8c650d66u;r[1]+=r[3];goto L8c650d68; /* add r3,r1 */
L8c650d68: *stop=0x8c650d68u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c650d6a; /* shll r1 */
L8c650d6a: *stop=0x8c650d6au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c650d6c; /* mov.l r1,@r2 */
L8c650d6c: *stop=0x8c650d6cu;r[6]>>=2;goto L8c650d6e; /* shlr2 r6 */
L8c650d6e: *stop=0x8c650d6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650d70; /* mov.l 0x8c650e1c,r1 */
L8c650d70: *stop=0x8c650d70u;r[6]>>=2;goto L8c650d72; /* shlr2 r6 */
L8c650d72: *stop=0x8c650d72u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c650d74; /* shlr r6 */
L8c650d74: *stop=0x8c650d74u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[6],4);if(io_status)return io_status;goto L8c650d76; /* mov.l r6,@r1 */
L8c650d76: *stop=0x8c650d76u;if(!valid_access(r[7]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650d78; /* mov.l @(0x18,r7),r0 */
L8c650d78: *stop=0x8c650d78u;r[4]=sx(m,bus,&io_status,0x8c650e00u,2);if(io_status)return io_status;goto L8c650d7a; /* mov.w 0x8c650e00,r4 */
L8c650d7a: *stop=0x8c650d7au;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c650d7c; /* tst #0x4,r0 */
L8c650d7c: take=!(*sr&1u);*stop=0x8c650d7eu;r[0]=r[5];if(take)goto L8c650d82;goto L8c650d80; /* bf/s 0x8c650d82 */
L8c650d80: *stop=0x8c650d80u;r[4]=sx(m,bus,&io_status,0x8c650e02u,2);if(io_status)return io_status;goto L8c650d82; /* mov.w 0x8c650e02,r4 */
L8c650d82: *stop=0x8c650d82u;r[0]+=r[11];goto L8c650d84; /* add r11,r0 */
L8c650d84: *stop=0x8c650d84u;r[0]+=r[13];goto L8c650d86; /* add r13,r0 */
L8c650d86: *stop=0x8c650d86u;if(!valid_access(r[0]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650d88; /* mov.l @(0x18,r0),r0 */
L8c650d88: *stop=0x8c650d88u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c650d8a; /* tst #0x10,r0 */
L8c650d8a: if((*sr&1u))goto L8c650d90;goto L8c650d8c; /* bt 0x8c650d90 */
L8c650d8c: *stop=0x8c650d8cu;r[2]=sx(m,bus,&io_status,0x8c650e04u,2);if(io_status)return io_status;goto L8c650d8e; /* mov.w 0x8c650e04,r2 */
L8c650d8e: *stop=0x8c650d8eu;r[4]|=r[2];goto L8c650d90; /* or r2,r4 */
L8c650d90: *stop=0x8c650d90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650d92; /* mov.l 0x8c650e20,r1 */
L8c650d92: *stop=0x8c650d92u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c650d94; /* mov.l r4,@r1 */
L8c650d94: *stop=0x8c650d94u;r[4]=r[5];goto L8c650d96; /* mov r5,r4 */
L8c650d96: *stop=0x8c650d96u;r[4]+=r[11];goto L8c650d98; /* add r11,r4 */
L8c650d98: *stop=0x8c650d98u;r[4]+=r[13];goto L8c650d9a; /* add r13,r4 */
L8c650d9a: *stop=0x8c650d9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650d9c; /* mov.l 0x8c650e24,r2 */
L8c650d9c: *stop=0x8c650d9cu;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650d9e; /* mov.l @(0xc,r4),r3 */
L8c650d9e: *stop=0x8c650d9eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c650da0; /* mov.l r3,@r2 */
L8c650da0: *stop=0x8c650da0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650da2; /* mov.l 0x8c650e28,r3 */
L8c650da2: *stop=0x8c650da2u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650da4; /* mov.l @(0x14,r4),r0 */
L8c650da4: *stop=0x8c650da4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c650da6; /* mov.l r0,@r3 */
L8c650da6: *stop=0x8c650da6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e2cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650da8; /* mov.l 0x8c650e2c,r0 */
L8c650da8: *stop=0x8c650da8u;r[1]=1u;goto L8c650daa; /* mov #0x1,r1 */
L8c650daa: *stop=0x8c650daau;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c650dac; /* mov.l r1,@r0 */
L8c650dac: *stop=0x8c650dacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650dae; /* mov.l 0x8c650e30,r3 */
L8c650dae: *stop=0x8c650daeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e0cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650db0; /* mov.l 0x8c650e0c,r1 */
L8c650db0: *stop=0x8c650db0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c650db2; /* mov.l @r15,r2 */
L8c650db2: *stop=0x8c650db2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c650db4; /* mov.l r2,@r1 */
L8c650db4: *stop=0x8c650db4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650e08u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650db6; /* mov.l 0x8c650e08,r2 */
L8c650db6: *stop=0x8c650db6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[10],4);if(io_status)return io_status;goto L8c650db8; /* mov.l r10,@r2 */
L8c650db8: *stop=0x8c650db8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c650dba; /* mov.l @(0x4,r15),r4 */
L8c650dba: *stop=0x8c650dbau;r[4]+=r[3];goto L8c650dbc; /* add r3,r4 */
L8c650dbc: *stop=0x8c650dbcu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c650dbe; /* mov.l @r4,r0 */
L8c650dbe: *stop=0x8c650dbeu;r[0]+=1u;goto L8c650dc0; /* add #0x1,r0 */
L8c650dc0: *stop=0x8c650dc0u;r[0]&=0x7u;goto L8c650dc2; /* and #0x7,r0 */
L8c650dc2: *stop=0x8c650dc2u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c650dc4; /* mov.l r0,@r4 */
L8c650dc4: *stop=0x8c650dc4u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c650dc6; /* stc SR,r3 */
L8c650dc6: *stop=0x8c650dc6u;r[0]=r[9];goto L8c650dc8; /* mov r9,r0 */
L8c650dc8: *stop=0x8c650dc8u;r[2]=sx(m,bus,&io_status,0x8c650e06u,2);if(io_status)return io_status;goto L8c650dca; /* mov.w 0x8c650e06,r2 */
L8c650dca: *stop=0x8c650dcau;r[0]&=0xfu;goto L8c650dcc; /* and #0xf,r0 */
L8c650dcc: *stop=0x8c650dccu;r[0]<<=2;goto L8c650dce; /* shll2 r0 */
L8c650dce: *stop=0x8c650dceu;r[3]&=r[2];goto L8c650dd0; /* and r2,r3 */
L8c650dd0: *stop=0x8c650dd0u;r[0]<<=2;goto L8c650dd2; /* shll2 r0 */
L8c650dd2: *stop=0x8c650dd2u;r[0]|=r[3];goto L8c650dd4; /* or r3,r0 */
L8c650dd4: *stop=0x8c650dd4u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c650dd6; /* ldc r0,SR */
L8c650dd6: *stop=0x8c650dd6u;r[0]=r[14];goto L8c650dd8; /* mov r14,r0 */
L8c650dd8: *stop=0x8c650dd8u;r[15]+=8u;goto L8c650dda; /* add #0x8,r15 */
L8c650dda: *stop=0x8c650ddau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c650ddc; /* mov.l @r15+,r9 */
L8c650ddc: *stop=0x8c650ddcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c650dde; /* mov.l @r15+,r10 */
L8c650dde: *stop=0x8c650ddeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c650de0; /* mov.l @r15+,r11 */
L8c650de0: *stop=0x8c650de0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c650de2; /* mov.l @r15+,r12 */
L8c650de2: *stop=0x8c650de2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c650de4; /* mov.l @r15+,r13 */
L8c650de4: *stop=0x8c650de4u;if(*pr!=entry_pr)return -3;*stop=0x8c650de6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_submit_8c653476(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c653476;
L8c653476: *stop=0x8c653476u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6534c0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c653478; /* mov.l 0x8c6534c0,r0 */
L8c653478: *stop=0x8c653478u;if(*pr!=entry_pr)return -3;*stop=0x8c65347au;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}
