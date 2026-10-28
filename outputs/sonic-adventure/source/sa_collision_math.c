/* Original cross-product and vector-length helpers.
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
SA_STAGE_EXPORT int sa_collision_math_8c027360(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c027360;
L8c027360: *stop=0x8c027360u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c027362; /* fmov.s @r5+,fr9 */
L8c027362: *stop=0x8c027362u;r[0]=4u;goto L8c027364; /* mov #0x4,r0 */
L8c027364: *stop=0x8c027364u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c027366; /* fmov.s @r4+,fr4 */
L8c027366: *stop=0x8c027366u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c027368; /* fmov.s @r6+,fr3 */
L8c027368: *stop=0x8c027368u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[4],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c02736a; /* fsub fr4,fr9 */
L8c02736a: *stop=0x8c02736au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02736c; /* fmov.s @r5+,fr8 */
L8c02736c: *stop=0x8c02736cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c02736e; /* fsub fr3,fr4 */
L8c02736e: *stop=0x8c02736eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c027370; /* fmov.s @r4+,fr6 */
L8c027370: *stop=0x8c027370u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c027372; /* fmov.s @r6+,fr3 */
L8c027372: *stop=0x8c027372u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[6],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c027374; /* fsub fr6,fr8 */
L8c027374: *stop=0x8c027374u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c027376; /* fmov.s @r4,fr5 */
L8c027376: *stop=0x8c027376u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c027378; /* fsub fr3,fr6 */
L8c027378: *stop=0x8c027378u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;goto L8c02737a; /* fmov.s @r5,fr7 */
L8c02737a: *stop=0x8c02737au;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c02737c; /* fmov.s @r6,fr3 */
L8c02737c: *stop=0x8c02737cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[5],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c02737e; /* fsub fr5,fr7 */
L8c02737e: *stop=0x8c02737eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c027380; /* fsub fr3,fr5 */
L8c027380: *stop=0x8c027380u;fcopy(f,3u,8u);goto L8c027382; /* fmov fr8,fr3 */
L8c027382: *stop=0x8c027382u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[4],&f->fpscr,&fr[8]);if(status)return status;}goto L8c027384; /* fmul fr4,fr8 */
L8c027384: *stop=0x8c027384u;fcopy(f,2u,7u);goto L8c027386; /* fmov fr7,fr2 */
L8c027386: *stop=0x8c027386u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c027388; /* fmul fr6,fr2 */
L8c027388: *stop=0x8c027388u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c02738a; /* fmul fr5,fr3 */
L8c02738a: *stop=0x8c02738au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[4],&f->fpscr,&fr[7]);if(status)return status;}goto L8c02738c; /* fmul fr4,fr7 */
L8c02738c: *stop=0x8c02738cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c02738e; /* fsub fr3,fr2 */
L8c02738e: *stop=0x8c02738eu;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c027390; /* fmov.s fr2,@r7 */
L8c027390: *stop=0x8c027390u;fcopy(f,2u,9u);goto L8c027392; /* fmov fr9,fr2 */
L8c027392: *stop=0x8c027392u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c027394; /* fmul fr5,fr2 */
L8c027394: *stop=0x8c027394u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[6],&f->fpscr,&fr[9]);if(status)return status;}goto L8c027396; /* fmul fr6,fr9 */
L8c027396: *stop=0x8c027396u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[7],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c027398; /* fsub fr7,fr2 */
L8c027398: *stop=0x8c027398u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[9],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c02739a; /* fsub fr9,fr8 */
L8c02739a: *stop=0x8c02739au;if(!valid_access(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c02739c; /* fmov fr2,@(r0,r7) */
L8c02739c: *stop=0x8c02739cu;r[0]=8u;goto L8c02739e; /* mov #0x8,r0 */
L8c02739e: *stop=0x8c02739eu;if(*pr!=entry_pr)return -3;*stop=0x8c0273a0u;if(!valid_access(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,8u,r[0]+r[7]);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_collision_math_8c63a75c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63a75c;
L8c63a75c: *stop=0x8c63a75cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0;goto L8c63a75e; /* fldi0 fr7 */
L8c63a75e: *stop=0x8c63a75eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a760; /* fmov.s @r4+,fr4 */
L8c63a760: *stop=0x8c63a760u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a762; /* fmov.s @r4+,fr5 */
L8c63a762: *stop=0x8c63a762u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a764; /* fmov.s @r4+,fr6 */
L8c63a764: *stop=0x8c63a764u;status=sa_sh4_vector_numeric(0xf5edu,f,*fpul);if(status)return status;goto L8c63a766; /* fipr fv4,fv4 */
L8c63a766: *stop=0x8c63a766u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[7],&f->fpscr,&fr[7]);if(status)return status;}goto L8c63a768; /* fsqrt fr7 */
L8c63a768: *stop=0x8c63a768u;if(*pr!=entry_pr)return -3;*stop=0x8c63a76au;fcopy(f,0u,7u);return 0; /* rts */
}
