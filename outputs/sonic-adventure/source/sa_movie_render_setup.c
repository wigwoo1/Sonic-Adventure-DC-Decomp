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
SA_STAGE_EXPORT int sa_movie_render_setup_8c653386(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c653386;
L8c653386: *stop=0x8c653386u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6533d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c653388; /* mov.l 0x8c6533d0,r3 */
L8c653388: *stop=0x8c653388u;r[4]+=4294967295u;goto L8c65338a; /* add #-0x1,r4 */
L8c65338a: *stop=0x8c65338au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c65338c; /* mov.l r4,@r3 */
L8c65338c: *stop=0x8c65338cu;if(*pr!=entry_pr)return -3;*stop=0x8c65338eu;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_setup_8c64133e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64133e;
L8c64133e: *stop=0x8c64133eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c641340; /* sts.l PR,@-r15 */
L8c641340: *stop=0x8c641340u;r[15]+=4294967288u;goto L8c641342; /* add #-0x8,r15 */
L8c641342: *stop=0x8c641342u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6413c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641344; /* mov.l 0x8c6413c4,r3 */
L8c641344: *stop=0x8c641344u;r[7]=r[5];goto L8c641346; /* mov r5,r7 */
L8c641346: *stop=0x8c641346u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c641348; /* mov.l r4,@r15 */
L8c641348: *stop=0x8c641348u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c64134a; /* mov.l r5,@(0x4,r15) */
L8c64134a: *stop=0x8c64134au;r[5]=0u;goto L8c64134c; /* mov #0x0,r5 */
L8c64134c: *stop=0x8c64134cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64134e; /* mov.l @r15,r6 */
L8c64134e: target=0x8c6412c2u;*pr=0x8c641352u;*stop=0x8c641350u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641352; /* bsr 0x8c6412c2 */
L8c641352: *stop=0x8c641352u;r[15]+=8u;goto L8c641354; /* add #0x8,r15 */
L8c641354: *stop=0x8c641354u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c641356; /* lds.l @r15+,PR */
L8c641356: *stop=0x8c641356u;if(*pr!=entry_pr)return -3;*stop=0x8c641358u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_setup_8c64162a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64162a;
L8c64162a: *stop=0x8c64162au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64162c; /* sts.l PR,@-r15 */
L8c64162c: *stop=0x8c64162cu;r[15]+=4294967288u;goto L8c64162e; /* add #-0x8,r15 */
L8c64162e: *stop=0x8c64162eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6416c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641630; /* mov.l 0x8c6416c0,r3 */
L8c641630: *stop=0x8c641630u;r[7]=r[5];goto L8c641632; /* mov r5,r7 */
L8c641632: *stop=0x8c641632u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c641634; /* mov.l r4,@r15 */
L8c641634: *stop=0x8c641634u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c641636; /* mov.l r5,@(0x4,r15) */
L8c641636: *stop=0x8c641636u;r[5]=5u;goto L8c641638; /* mov #0x5,r5 */
L8c641638: *stop=0x8c641638u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64163a; /* mov.l @r15,r6 */
L8c64163a: target=0x8c6412c2u;*pr=0x8c64163eu;*stop=0x8c64163cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64163e; /* bsr 0x8c6412c2 */
L8c64163e: *stop=0x8c64163eu;r[15]+=8u;goto L8c641640; /* add #0x8,r15 */
L8c641640: *stop=0x8c641640u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c641642; /* lds.l @r15+,PR */
L8c641642: *stop=0x8c641642u;if(*pr!=entry_pr)return -3;*stop=0x8c641644u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_setup_8c64167e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64167e;
L8c64167e: *stop=0x8c64167eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c641680; /* sts.l PR,@-r15 */
L8c641680: *stop=0x8c641680u;r[15]+=4294967292u;goto L8c641682; /* add #-0x4,r15 */
L8c641682: *stop=0x8c641682u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6416c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641684; /* mov.l 0x8c6416c0,r3 */
L8c641684: *stop=0x8c641684u;r[5]=8u;goto L8c641686; /* mov #0x8,r5 */
L8c641686: *stop=0x8c641686u;r[6]=r[4];goto L8c641688; /* mov r4,r6 */
L8c641688: *stop=0x8c641688u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c64168a; /* mov.l r4,@r15 */
L8c64168a: *stop=0x8c64168au;r[7]=0u;goto L8c64168c; /* mov #0x0,r7 */
L8c64168c: target=0x8c6412c2u;*pr=0x8c641690u;*stop=0x8c64168eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641690; /* bsr 0x8c6412c2 */
L8c641690: *stop=0x8c641690u;r[15]+=4u;goto L8c641692; /* add #0x4,r15 */
L8c641692: *stop=0x8c641692u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c641694; /* lds.l @r15+,PR */
L8c641694: *stop=0x8c641694u;if(*pr!=entry_pr)return -3;*stop=0x8c641696u;r[0]=0u;return 0; /* rts */
}
