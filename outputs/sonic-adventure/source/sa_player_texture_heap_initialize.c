/* Original complete texture allocator initialization.
 * Exact original instruction order and existing numerical/bus ABI.
 * No upstream implementation code imported.
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
int sa_sh4_sqrt(uint32_t,uint32_t*,uint32_t*);
int sa_movie_channel_io_return_allowed(void *,uint32_t);
SA_STAGE_EXPORT int sa_player_texture_heap_initialize_8c660e58(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c660e58;
L8c660e58: *stop=0x8c660e58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660f38u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c660e5a; /* mov.l 0x8c660f38,r6 */
L8c660e5a: *stop=0x8c660e5au;r[0]=1u;goto L8c660e5c; /* mov #0x1,r0 */
L8c660e5c: *stop=0x8c660e5cu;r[3]=36u;goto L8c660e5e; /* mov #0x24,r3 */
L8c660e5e: *stop=0x8c660e5eu;r[7]=sx(m,bus,&io_status,0x8c660f36u,2);if(io_status)return io_status;goto L8c660e60; /* mov.w 0x8c660f36,r7 */
L8c660e60: *stop=0x8c660e60u;if(!valid_access(r[6]+4u,4u))return -3;wr(m,bus,&io_status,r[6]+0x4u,r[3],4);if(io_status)return io_status;goto L8c660e62; /* mov.l r3,@(0x4,r6) */
L8c660e62: *stop=0x8c660e62u;r[3]=r[6];goto L8c660e64; /* mov r6,r3 */
L8c660e64: *stop=0x8c660e64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660f3cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c660e66; /* mov.l 0x8c660f3c,r1 */
L8c660e66: *stop=0x8c660e66u;if(!valid_access(r[6],2u))return -3;wr(m,bus,&io_status,r[6],r[7],2);if(io_status)return io_status;goto L8c660e68; /* mov.w r7,@r6 */
L8c660e68: *stop=0x8c660e68u;if(!valid_access(r[6]+2u,2u))return -3;wr(m,bus,&io_status,r[6]+0x2u,r[0],2);if(io_status)return io_status;goto L8c660e6a; /* mov.w r0,@(0x2,r6) */
L8c660e6a: *stop=0x8c660e6au;r[0]=0u;goto L8c660e6c; /* mov #0x0,r0 */
L8c660e6c: *stop=0x8c660e6cu;if(!valid_access(r[6]+12u,4u))return -3;wr(m,bus,&io_status,r[6]+0xcu,r[4],4);if(io_status)return io_status;goto L8c660e6e; /* mov.l r4,@(0xc,r6) */
L8c660e6e: *stop=0x8c660e6eu;if(!valid_access(r[6]+8u,4u))return -3;wr(m,bus,&io_status,r[6]+0x8u,r[4],4);if(io_status)return io_status;goto L8c660e70; /* mov.l r4,@(0x8,r6) */
L8c660e70: *stop=0x8c660e70u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c660e72; /* mov.l r5,@(0x4,r4) */
L8c660e72: *stop=0x8c660e72u;if(!valid_access(r[4],2u))return -3;wr(m,bus,&io_status,r[4],r[7],2);if(io_status)return io_status;goto L8c660e74; /* mov.w r7,@r4 */
L8c660e74: *stop=0x8c660e74u;if(!valid_access(r[4]+2u,2u))return -3;wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c660e76; /* mov.w r0,@(0x2,r4) */
L8c660e76: *stop=0x8c660e76u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[6],4);if(io_status)return io_status;goto L8c660e78; /* mov.l r6,@(0xc,r4) */
L8c660e78: *stop=0x8c660e78u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[3],4);if(io_status)return io_status;goto L8c660e7a; /* mov.l r3,@(0x8,r4) */
L8c660e7a: *stop=0x8c660e7au;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660e7c; /* mov.l @(0x8,r6),r2 */
L8c660e7c: *stop=0x8c660e7cu;if(*pr!=entry_pr)return -3;*stop=0x8c660e7eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;return 0; /* rts */
}
