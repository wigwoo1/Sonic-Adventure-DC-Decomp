/* Original glyph quad emitter and string drawing wrapper.
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
SA_STAGE_EXPORT int sa_collision_text_8c6407b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6407b0;
L8c6407b0: *stop=0x8c6407b0u;r[7]=15u;goto L8c6407b2; /* mov #0xf,r7 */
L8c6407b2: *stop=0x8c6407b2u;r[0]=r[6]&255u;goto L8c6407b4; /* extu.b r6,r0 */
L8c6407b4: *stop=0x8c6407b4u;r[7]&=r[0];goto L8c6407b6; /* and r0,r7 */
L8c6407b6: *stop=0x8c6407b6u;r[7]=(uint32_t)(int32_t)(int16_t)r[7];goto L8c6407b8; /* exts.w r7,r7 */
L8c6407b8: *stop=0x8c6407b8u;*fpul=r[7];goto L8c6407ba; /* lds r7,FPUL */
L8c6407ba: *stop=0x8c6407bau;r[0]=0x8c640a28u;goto L8c6407bc; /* mova 0x8c640a28,r0 */
L8c6407bc: *stop=0x8c6407bcu;r[6]=r[6]&255u;goto L8c6407be; /* extu.b r6,r6 */
L8c6407be: *stop=0x8c6407beu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c6407c0; /* fmov.s @r0,fr5 */
L8c6407c0: *stop=0x8c6407c0u;r[6]>>=2;goto L8c6407c2; /* shlr2 r6 */
L8c6407c2: *stop=0x8c6407c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a38u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6407c4; /* mov.l 0x8c640a38,r7 */
L8c6407c4: *stop=0x8c6407c4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6407c6; /* float FPUL,fr3 */
L8c6407c6: *stop=0x8c6407c6u;r[6]>>=2;goto L8c6407c8; /* shlr2 r6 */
L8c6407c8: *stop=0x8c6407c8u;r[0]=0x8c640a2cu;goto L8c6407ca; /* mova 0x8c640a2c,r0 */
L8c6407ca: *stop=0x8c6407cau;fcopy(f,6u,5u);goto L8c6407cc; /* fmov fr5,fr6 */
L8c6407cc: *stop=0x8c6407ccu;r[6]=(uint32_t)(int32_t)(int16_t)r[6];goto L8c6407ce; /* exts.w r6,r6 */
L8c6407ce: *stop=0x8c6407ceu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c6407d0; /* fmov.s @r0,fr4 */
L8c6407d0: *stop=0x8c6407d0u;*fpul=r[6];goto L8c6407d2; /* lds r6,FPUL */
L8c6407d2: *stop=0x8c6407d2u;r[0]=0x8c640a30u;goto L8c6407d4; /* mova 0x8c640a30,r0 */
L8c6407d4: *stop=0x8c6407d4u;fcopy(f,0u,4u);goto L8c6407d6; /* fmov fr4,fr0 */
L8c6407d6: *stop=0x8c6407d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[6],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6407d8; /* fmac fr0,fr3,fr6 */
L8c6407d8: *stop=0x8c6407d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a34u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6407da; /* mov.l 0x8c640a34,r6 */
L8c6407da: *stop=0x8c6407dau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6407dc; /* float FPUL,fr3 */
L8c6407dc: *stop=0x8c6407dcu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c6407de; /* fmov.s @r0,fr4 */
L8c6407de: *stop=0x8c6407deu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6407e0; /* mov.l @r6,r3 */
L8c6407e0: *stop=0x8c6407e0u;fcopy(f,10u,6u);goto L8c6407e2; /* fmov fr6,fr10 */
L8c6407e2: *stop=0x8c6407e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[10],&fr[4],&f->fpscr,&fr[10],0);if(status)return status;}goto L8c6407e4; /* fadd fr4,fr10 */
L8c6407e4: *stop=0x8c6407e4u;r[3]+=28u;goto L8c6407e6; /* add #0x1c,r3 */
L8c6407e6: *stop=0x8c6407e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[5],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6407e8; /* fmac fr0,fr3,fr5 */
L8c6407e8: *stop=0x8c6407e8u;r[2]=r[3];goto L8c6407ea; /* mov r3,r2 */
L8c6407ea: *stop=0x8c6407eau;fcopy(f,9u,5u);goto L8c6407ec; /* fmov fr5,fr9 */
L8c6407ec: *stop=0x8c6407ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[4],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c6407ee; /* fadd fr4,fr9 */
L8c6407ee: *stop=0x8c6407eeu;fcopy(f,7u,5u);goto L8c6407f0; /* fmov fr5,fr7 */
L8c6407f0: *stop=0x8c6407f0u;r[2]+=4294967292u;goto L8c6407f2; /* add #-0x4,r2 */
L8c6407f2: *stop=0x8c6407f2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c6407f4; /* fldi1 fr5 */
L8c6407f4: *stop=0x8c6407f4u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c6407f6; /* mov.l r2,@r6 */
L8c6407f6: *stop=0x8c6407f6u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6407f8; /* mov.l @r7,r3 */
L8c6407f8: *stop=0x8c6407f8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6407fa; /* mov.l r3,@r2 */
L8c6407fa: *stop=0x8c6407fau;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6407fc; /* mov.l @r6,r2 */
L8c6407fc: *stop=0x8c6407fcu;r[2]+=4294967292u;goto L8c6407fe; /* add #-0x4,r2 */
L8c6407fe: *stop=0x8c6407feu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c640800; /* mov.l r2,@r6 */
L8c640800: *stop=0x8c640800u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c640802; /* fmov.s fr7,@r2 */
L8c640802: *stop=0x8c640802u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640804; /* mov.l @r6,r3 */
L8c640804: *stop=0x8c640804u;r[3]+=4294967292u;goto L8c640806; /* add #-0x4,r3 */
L8c640806: *stop=0x8c640806u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c640808; /* mov.l r3,@r6 */
L8c640808: *stop=0x8c640808u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,6u,r[3]);if(io_status)return io_status;goto L8c64080a; /* fmov.s fr6,@r3 */
L8c64080a: *stop=0x8c64080au;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64080c; /* mov.l @r6,r2 */
L8c64080c: *stop=0x8c64080cu;r[2]+=4294967292u;goto L8c64080e; /* add #-0x4,r2 */
L8c64080e: *stop=0x8c64080eu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c640810; /* mov.l r2,@r6 */
L8c640810: *stop=0x8c640810u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[2]);if(io_status)return io_status;goto L8c640812; /* fmov.s fr5,@r2 */
L8c640812: *stop=0x8c640812u;r[2]=(uint32_t)(int32_t)(int16_t)r[5];goto L8c640814; /* exts.w r5,r2 */
L8c640814: *stop=0x8c640814u;*fpul=r[2];goto L8c640816; /* lds r2,FPUL */
L8c640816: *stop=0x8c640816u;r[2]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c640818; /* exts.w r4,r2 */
L8c640818: *stop=0x8c640818u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64081a; /* mov.l @r6,r3 */
L8c64081a: *stop=0x8c64081au;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a3cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64081c; /* mov.l 0x8c640a3c,r4 */
L8c64081c: *stop=0x8c64081cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][4]);if(status)return status;goto L8c64081e; /* float FPUL,fr4 */
L8c64081e: *stop=0x8c64081eu;*fpul=r[2];goto L8c640820; /* lds r2,FPUL */
L8c640820: *stop=0x8c640820u;r[3]+=4294967292u;goto L8c640822; /* add #-0x4,r3 */
L8c640822: *stop=0x8c640822u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c640824; /* mov.l r3,@r6 */
L8c640824: *stop=0x8c640824u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][8]);if(status)return status;goto L8c640826; /* float FPUL,fr8 */
L8c640826: *stop=0x8c640826u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;goto L8c640828; /* fmov.s fr4,@r3 */
L8c640828: *stop=0x8c640828u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64082a; /* mov.l @r6,r3 */
L8c64082a: *stop=0x8c64082au;r[3]+=4294967292u;goto L8c64082c; /* add #-0x4,r3 */
L8c64082c: *stop=0x8c64082cu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c64082e; /* mov.l r3,@r6 */
L8c64082e: *stop=0x8c64082eu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,8u,r[3]);if(io_status)return io_status;goto L8c640830; /* fmov.s fr8,@r3 */
L8c640830: *stop=0x8c640830u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640832; /* mov.l @r6,r3 */
L8c640832: *stop=0x8c640832u;r[3]+=4294967292u;goto L8c640834; /* add #-0x4,r3 */
L8c640834: *stop=0x8c640834u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c640836; /* mov.l r3,@r6 */
L8c640836: *stop=0x8c640836u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c640838; /* mov.l r4,@r3 */
L8c640838: *stop=0x8c640838u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64083a; /* mov.l @r6,r2 */
L8c64083a: *stop=0x8c64083au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c64083c; /* pref @r2 */
L8c64083c: *stop=0x8c64083cu;r[3]=r[2];goto L8c64083e; /* mov r2,r3 */
L8c64083e: *stop=0x8c64083eu;r[3]+=60u;goto L8c640840; /* add #0x3c,r3 */
L8c640840: *stop=0x8c640840u;r[2]=r[3];goto L8c640842; /* mov r3,r2 */
L8c640842: *stop=0x8c640842u;r[2]+=4294967292u;goto L8c640844; /* add #-0x4,r2 */
L8c640844: *stop=0x8c640844u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a40u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c640846; /* mov.l 0x8c640a40,r5 */
L8c640846: *stop=0x8c640846u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c640848; /* mov.l r2,@r6 */
L8c640848: *stop=0x8c640848u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64084a; /* mov.l @r7,r3 */
L8c64084a: *stop=0x8c64084au;fcopy(f,2u,4u);goto L8c64084c; /* fmov fr4,fr2 */
L8c64084c: *stop=0x8c64084cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c64084e; /* mov.l r3,@r2 */
L8c64084e: *stop=0x8c64084eu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c640850; /* mov.l @r6,r2 */
L8c640850: *stop=0x8c640850u;r[2]+=4294967292u;goto L8c640852; /* add #-0x4,r2 */
L8c640852: *stop=0x8c640852u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c640854; /* mov.l r2,@r6 */
L8c640854: *stop=0x8c640854u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,9u,r[2]);if(io_status)return io_status;goto L8c640856; /* fmov.s fr9,@r2 */
L8c640856: *stop=0x8c640856u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640858; /* mov.l @r6,r3 */
L8c640858: *stop=0x8c640858u;r[3]+=4294967292u;goto L8c64085a; /* add #-0x4,r3 */
L8c64085a: *stop=0x8c64085au;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c64085c; /* mov.l r3,@r6 */
L8c64085c: *stop=0x8c64085cu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,6u,r[3]);if(io_status)return io_status;goto L8c64085e; /* fmov.s fr6,@r3 */
L8c64085e: *stop=0x8c64085eu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c640860; /* mov.l @r6,r2 */
L8c640860: *stop=0x8c640860u;r[2]+=4294967292u;goto L8c640862; /* add #-0x4,r2 */
L8c640862: *stop=0x8c640862u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c640864; /* mov.l r2,@r6 */
L8c640864: *stop=0x8c640864u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[2]);if(io_status)return io_status;goto L8c640866; /* fmov.s fr5,@r2 */
L8c640866: *stop=0x8c640866u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640868; /* mov.l @r6,r3 */
L8c640868: *stop=0x8c640868u;r[3]+=4294967292u;goto L8c64086a; /* add #-0x4,r3 */
L8c64086a: *stop=0x8c64086au;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c64086c; /* mov.l r3,@r6 */
L8c64086c: *stop=0x8c64086cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c64086e; /* fmov.s @r5,fr3 */
L8c64086e: *stop=0x8c64086eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c640870; /* fadd fr3,fr2 */
L8c640870: *stop=0x8c640870u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c640872; /* fmov.s fr2,@r3 */
L8c640872: *stop=0x8c640872u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640874; /* mov.l @r6,r3 */
L8c640874: *stop=0x8c640874u;r[3]+=4294967292u;goto L8c640876; /* add #-0x4,r3 */
L8c640876: *stop=0x8c640876u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c640878; /* mov.l r3,@r6 */
L8c640878: *stop=0x8c640878u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,8u,r[3]);if(io_status)return io_status;goto L8c64087a; /* fmov.s fr8,@r3 */
L8c64087a: *stop=0x8c64087au;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64087c; /* mov.l @r6,r2 */
L8c64087c: *stop=0x8c64087cu;r[2]+=4294967292u;goto L8c64087e; /* add #-0x4,r2 */
L8c64087e: *stop=0x8c64087eu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c640880; /* mov.l r2,@r6 */
L8c640880: *stop=0x8c640880u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c640882; /* mov.l r4,@r2 */
L8c640882: *stop=0x8c640882u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640884; /* mov.l @r6,r3 */
L8c640884: *stop=0x8c640884u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c640886; /* pref @r3 */
L8c640886: *stop=0x8c640886u;r[2]=r[3];goto L8c640888; /* mov r3,r2 */
L8c640888: *stop=0x8c640888u;r[2]+=60u;goto L8c64088a; /* add #0x3c,r2 */
L8c64088a: *stop=0x8c64088au;r[3]=r[2];goto L8c64088c; /* mov r2,r3 */
L8c64088c: *stop=0x8c64088cu;r[3]+=4294967292u;goto L8c64088e; /* add #-0x4,r3 */
L8c64088e: *stop=0x8c64088eu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c640890; /* mov.l r3,@r6 */
L8c640890: *stop=0x8c640890u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c640892; /* mov.l @r7,r2 */
L8c640892: *stop=0x8c640892u;fcopy(f,2u,8u);goto L8c640894; /* fmov fr8,fr2 */
L8c640894: *stop=0x8c640894u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c640896; /* mov.l r2,@r3 */
L8c640896: *stop=0x8c640896u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640898; /* mov.l @r6,r3 */
L8c640898: *stop=0x8c640898u;r[3]+=4294967292u;goto L8c64089a; /* add #-0x4,r3 */
L8c64089a: *stop=0x8c64089au;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c64089c; /* mov.l r3,@r6 */
L8c64089c: *stop=0x8c64089cu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,7u,r[3]);if(io_status)return io_status;goto L8c64089e; /* fmov.s fr7,@r3 */
L8c64089e: *stop=0x8c64089eu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6408a0; /* mov.l @r6,r2 */
L8c6408a0: *stop=0x8c6408a0u;r[2]+=4294967292u;goto L8c6408a2; /* add #-0x4,r2 */
L8c6408a2: *stop=0x8c6408a2u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c6408a4; /* mov.l r2,@r6 */
L8c6408a4: *stop=0x8c6408a4u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;goto L8c6408a6; /* fmov.s fr10,@r2 */
L8c6408a6: *stop=0x8c6408a6u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6408a8; /* mov.l @r6,r3 */
L8c6408a8: *stop=0x8c6408a8u;r[3]+=4294967292u;goto L8c6408aa; /* add #-0x4,r3 */
L8c6408aa: *stop=0x8c6408aau;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c6408ac; /* mov.l r3,@r6 */
L8c6408ac: *stop=0x8c6408acu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c6408ae; /* fmov.s fr5,@r3 */
L8c6408ae: *stop=0x8c6408aeu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6408b0; /* mov.l @r6,r2 */
L8c6408b0: *stop=0x8c6408b0u;r[2]+=4294967292u;goto L8c6408b2; /* add #-0x4,r2 */
L8c6408b2: *stop=0x8c6408b2u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c6408b4; /* mov.l r2,@r6 */
L8c6408b4: *stop=0x8c6408b4u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c6408b6; /* fmov.s fr4,@r2 */
L8c6408b6: *stop=0x8c6408b6u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6408b8; /* mov.l @r6,r3 */
L8c6408b8: *stop=0x8c6408b8u;r[3]+=4294967292u;goto L8c6408ba; /* add #-0x4,r3 */
L8c6408ba: *stop=0x8c6408bau;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c6408bc; /* mov.l r3,@r6 */
L8c6408bc: *stop=0x8c6408bcu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c6408be; /* fmov.s @r5,fr3 */
L8c6408be: *stop=0x8c6408beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c6408c0; /* fadd fr3,fr2 */
L8c6408c0: *stop=0x8c6408c0u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6408c2; /* fmov.s fr2,@r3 */
L8c6408c2: *stop=0x8c6408c2u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6408c4; /* mov.l @r6,r3 */
L8c6408c4: *stop=0x8c6408c4u;r[3]+=4294967292u;goto L8c6408c6; /* add #-0x4,r3 */
L8c6408c6: *stop=0x8c6408c6u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c6408c8; /* mov.l r3,@r6 */
L8c6408c8: *stop=0x8c6408c8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c6408ca; /* mov.l r4,@r3 */
L8c6408ca: *stop=0x8c6408cau;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6408cc; /* mov.l @r6,r2 */
L8c6408cc: *stop=0x8c6408ccu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c6408ce; /* pref @r2 */
L8c6408ce: *stop=0x8c6408ceu;r[3]=r[2];goto L8c6408d0; /* mov r2,r3 */
L8c6408d0: *stop=0x8c6408d0u;r[3]+=60u;goto L8c6408d2; /* add #0x3c,r3 */
L8c6408d2: *stop=0x8c6408d2u;r[2]=r[3];goto L8c6408d4; /* mov r3,r2 */
L8c6408d4: *stop=0x8c6408d4u;r[2]+=4294967292u;goto L8c6408d6; /* add #-0x4,r2 */
L8c6408d6: *stop=0x8c6408d6u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c6408d8; /* mov.l r2,@r6 */
L8c6408d8: *stop=0x8c6408d8u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6408da; /* mov.l @r7,r3 */
L8c6408da: *stop=0x8c6408dau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6408dc; /* mov.l r3,@r2 */
L8c6408dc: *stop=0x8c6408dcu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6408de; /* mov.l @r6,r2 */
L8c6408de: *stop=0x8c6408deu;r[2]+=4294967292u;goto L8c6408e0; /* add #-0x4,r2 */
L8c6408e0: *stop=0x8c6408e0u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c6408e2; /* mov.l r2,@r6 */
L8c6408e2: *stop=0x8c6408e2u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,9u,r[2]);if(io_status)return io_status;goto L8c6408e4; /* fmov.s fr9,@r2 */
L8c6408e4: *stop=0x8c6408e4u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6408e6; /* mov.l @r6,r3 */
L8c6408e6: *stop=0x8c6408e6u;r[3]+=4294967292u;goto L8c6408e8; /* add #-0x4,r3 */
L8c6408e8: *stop=0x8c6408e8u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c6408ea; /* mov.l r3,@r6 */
L8c6408ea: *stop=0x8c6408eau;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c6408ec; /* fmov.s fr10,@r3 */
L8c6408ec: *stop=0x8c6408ecu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6408ee; /* mov.l @r6,r2 */
L8c6408ee: *stop=0x8c6408eeu;r[2]+=4294967292u;goto L8c6408f0; /* add #-0x4,r2 */
L8c6408f0: *stop=0x8c6408f0u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c6408f2; /* mov.l r2,@r6 */
L8c6408f2: *stop=0x8c6408f2u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[2]);if(io_status)return io_status;goto L8c6408f4; /* fmov.s fr5,@r2 */
L8c6408f4: *stop=0x8c6408f4u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6408f6; /* mov.l @r6,r3 */
L8c6408f6: *stop=0x8c6408f6u;r[3]+=4294967292u;goto L8c6408f8; /* add #-0x4,r3 */
L8c6408f8: *stop=0x8c6408f8u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c6408fa; /* mov.l r3,@r6 */
L8c6408fa: *stop=0x8c6408fau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c6408fc; /* fmov.s @r5,fr3 */
L8c6408fc: *stop=0x8c6408fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c6408fe; /* fadd fr3,fr4 */
L8c6408fe: *stop=0x8c6408feu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;goto L8c640900; /* fmov.s fr4,@r3 */
L8c640900: *stop=0x8c640900u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640902; /* mov.l @r6,r3 */
L8c640902: *stop=0x8c640902u;r[3]+=4294967292u;goto L8c640904; /* add #-0x4,r3 */
L8c640904: *stop=0x8c640904u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c640906; /* mov.l r3,@r6 */
L8c640906: *stop=0x8c640906u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c640908; /* fmov.s @r5,fr3 */
L8c640908: *stop=0x8c640908u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c64090a; /* fadd fr3,fr8 */
L8c64090a: *stop=0x8c64090au;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,8u,r[3]);if(io_status)return io_status;goto L8c64090c; /* fmov.s fr8,@r3 */
L8c64090c: *stop=0x8c64090cu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64090e; /* mov.l @r6,r3 */
L8c64090e: *stop=0x8c64090eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a44u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c640910; /* mov.l 0x8c640a44,r2 */
L8c640910: *stop=0x8c640910u;r[3]+=4294967292u;goto L8c640912; /* add #-0x4,r3 */
L8c640912: *stop=0x8c640912u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c640914; /* mov.l r3,@r6 */
L8c640914: *stop=0x8c640914u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c640916; /* mov.l r2,@r3 */
L8c640916: *stop=0x8c640916u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640918; /* mov.l @r6,r3 */
L8c640918: *stop=0x8c640918u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c64091a; /* pref @r3 */
L8c64091a: *stop=0x8c64091au;r[2]=r[3];goto L8c64091c; /* mov r3,r2 */
L8c64091c: *stop=0x8c64091cu;r[2]+=32u;goto L8c64091e; /* add #0x20,r2 */
L8c64091e: *stop=0x8c64091eu;if(*pr!=entry_pr)return -3;*stop=0x8c640920u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;return 0; /* rts */
}

static int sa_collision_text_run_8c640922(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c6409d6u)goto L8c6409d6;if(resume_pc==0x8c6409eau)goto L8c6409ea;goto L8c640922;
L8c640922: *stop=0x8c640922u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c640924; /* mov.l r14,@-r15 */
L8c640924: *stop=0x8c640924u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c640926; /* mov.l r13,@-r15 */
L8c640926: *stop=0x8c640926u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c640928; /* mov.l r12,@-r15 */
L8c640928: *stop=0x8c640928u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64092a; /* mov.l r11,@-r15 */
L8c64092a: *stop=0x8c64092au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64092c; /* mov.l r10,@-r15 */
L8c64092c: *stop=0x8c64092cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64092e; /* mov.l r9,@-r15 */
L8c64092e: *stop=0x8c64092eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c640930; /* mov.l r8,@-r15 */
L8c640930: *stop=0x8c640930u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c640932; /* sts.l PR,@-r15 */
L8c640932: *stop=0x8c640932u;r[15]+=4294967292u;goto L8c640934; /* add #-0x4,r15 */
L8c640934: *stop=0x8c640934u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c640936; /* mov.l 0x8c640a48,r2 */
L8c640936: *stop=0x8c640936u;r[3]=4294967294u;goto L8c640938; /* mov #-0x2,r3 */
L8c640938: *stop=0x8c640938u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a50u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64093a; /* mov.l 0x8c640a50,r1 */
L8c64093a: *stop=0x8c64093au;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a4cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64093c; /* mov.l 0x8c640a4c,r6 */
L8c64093c: *stop=0x8c64093cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64093e; /* mov.l r5,@r15 */
L8c64093e: *stop=0x8c64093eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c640940; /* mov.l r3,@r2 */
L8c640940: *stop=0x8c640940u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640942; /* mov.l @r1,r3 */
L8c640942: *stop=0x8c640942u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c640944; /* mov.l 0x8c640a58,r2 */
L8c640944: *stop=0x8c640944u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c640946; /* mov.l @(0x8,r3),r0 */
L8c640946: *stop=0x8c640946u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a54u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c640948; /* mov.l 0x8c640a54,r3 */
L8c640948: *stop=0x8c640948u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[0],4);if(io_status)return io_status;goto L8c64094a; /* mov.l r0,@r6 */
L8c64094a: *stop=0x8c64094au;r[0]&=r[3];goto L8c64094c; /* and r3,r0 */
L8c64094c: *stop=0x8c64094cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64094e; /* mov.l 0x8c640a60,r3 */
L8c64094e: *stop=0x8c64094eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c640950; /* mov.l r0,@r2 */
L8c640950: *stop=0x8c640950u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[0]=loaded;}goto L8c640952; /* mov.l @r6,r0 */
L8c640952: *stop=0x8c640952u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a5cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c640954; /* mov.l 0x8c640a5c,r5 */
L8c640954: *stop=0x8c640954u;r[0]>>=16;goto L8c640956; /* shlr16 r0 */
L8c640956: *stop=0x8c640956u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a68u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c640958; /* mov.l 0x8c640a68,r2 */
L8c640958: *stop=0x8c640958u;r[0]>>=8;goto L8c64095a; /* shlr8 r0 */
L8c64095a: *stop=0x8c64095au;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a34u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64095c; /* mov.l 0x8c640a34,r14 */
L8c64095c: *stop=0x8c64095cu;r[0]&=0x1cu;goto L8c64095e; /* and #0x1c,r0 */
L8c64095e: *stop=0x8c64095eu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[0],4);if(io_status)return io_status;goto L8c640960; /* mov.l r0,@r5 */
L8c640960: *stop=0x8c640960u;r[1]=r[0];goto L8c640962; /* mov r0,r1 */
L8c640962: *stop=0x8c640962u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c640964; /* mov.l r0,@r3 */
L8c640964: *stop=0x8c640964u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a64u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c640966; /* mov.l 0x8c640a64,r1 */
L8c640966: *stop=0x8c640966u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[0]=loaded;}goto L8c640968; /* mov.l @r5,r0 */
L8c640968: *stop=0x8c640968u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64096a; /* mov.l 0x8c640a3c,r3 */
L8c64096a: *stop=0x8c64096au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c64096c; /* mov.l r0,@r1 */
L8c64096c: *stop=0x8c64096cu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64096e; /* mov.l @r6,r0 */
L8c64096e: *stop=0x8c64096eu;r[0]&=r[2];goto L8c640970; /* and r2,r0 */
L8c640970: *stop=0x8c640970u;r[0]|=r[3];goto L8c640972; /* or r3,r0 */
L8c640972: *stop=0x8c640972u;r[1]=r[0];goto L8c640974; /* mov r0,r1 */
L8c640974: *stop=0x8c640974u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a6cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c640976; /* mov.l 0x8c640a6c,r3 */
L8c640976: *stop=0x8c640976u;r[1]+=16u;goto L8c640978; /* add #0x10,r1 */
L8c640978: *stop=0x8c640978u;r[0]=r[1];goto L8c64097a; /* mov r1,r0 */
L8c64097a: *stop=0x8c64097au;r[0]+=4294967292u;goto L8c64097c; /* add #-0x4,r0 */
L8c64097c: *stop=0x8c64097cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a70u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64097e; /* mov.l 0x8c640a70,r2 */
L8c64097e: *stop=0x8c64097eu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c640980; /* mov.l r0,@r14 */
L8c640980: *stop=0x8c640980u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c640982; /* mov.l @r3,r1 */
L8c640982: *stop=0x8c640982u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a74u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c640984; /* mov.l 0x8c640a74,r3 */
L8c640984: *stop=0x8c640984u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c640986; /* mov.l r1,@r0 */
L8c640986: *stop=0x8c640986u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c640988; /* mov.l @r14,r0 */
L8c640988: *stop=0x8c640988u;r[0]+=4294967292u;goto L8c64098a; /* add #-0x4,r0 */
L8c64098a: *stop=0x8c64098au;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c64098c; /* mov.l r0,@r14 */
L8c64098c: *stop=0x8c64098cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64098e; /* mov.l @r2,r1 */
L8c64098e: *stop=0x8c64098eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a78u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c640990; /* mov.l 0x8c640a78,r2 */
L8c640990: *stop=0x8c640990u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c640992; /* mov.l r1,@r0 */
L8c640992: *stop=0x8c640992u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c640994; /* mov.l @r14,r0 */
L8c640994: *stop=0x8c640994u;r[0]+=4294967292u;goto L8c640996; /* add #-0x4,r0 */
L8c640996: *stop=0x8c640996u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c640998; /* mov.l r0,@r14 */
L8c640998: *stop=0x8c640998u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64099a; /* mov.l @r3,r1 */
L8c64099a: *stop=0x8c64099au;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c64099c; /* mov.l r1,@r0 */
L8c64099c: *stop=0x8c64099cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64099e; /* mov.l @r14,r0 */
L8c64099e: *stop=0x8c64099eu;r[0]+=4294967292u;goto L8c6409a0; /* add #-0x4,r0 */
L8c6409a0: *stop=0x8c6409a0u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c6409a2; /* mov.l r0,@r14 */
L8c6409a2: *stop=0x8c6409a2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6409a4; /* mov.l @r2,r1 */
L8c6409a4: *stop=0x8c6409a4u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c6409a6; /* mov.l r1,@r0 */
L8c6409a6: *stop=0x8c6409a6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6409a8; /* mov.l @r14,r0 */
L8c6409a8: *stop=0x8c6409a8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[0],1u);if(status)return status;goto L8c6409aa; /* pref @r0 */
L8c6409aa: *stop=0x8c6409aau;r[12]=r[4];goto L8c6409ac; /* mov r4,r12 */
L8c6409ac: *stop=0x8c6409acu;r[12]>>=16;goto L8c6409ae; /* shlr16 r12 */
L8c6409ae: *stop=0x8c6409aeu;r[12]=(uint32_t)(int32_t)(int16_t)r[12];goto L8c6409b0; /* exts.w r12,r12 */
L8c6409b0: *stop=0x8c6409b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a40u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6409b2; /* mov.l 0x8c640a40,r11 */
L8c6409b2: *stop=0x8c6409b2u;*fpul=r[12];goto L8c6409b4; /* lds r12,FPUL */
L8c6409b4: *stop=0x8c6409b4u;r[1]=r[0];goto L8c6409b6; /* mov r0,r1 */
L8c6409b6: *stop=0x8c6409b6u;r[1]+=32u;goto L8c6409b8; /* add #0x20,r1 */
L8c6409b8: *stop=0x8c6409b8u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c6409ba; /* mov.l r1,@r14 */
L8c6409ba: *stop=0x8c6409bau;r[1]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c6409bc; /* exts.w r4,r1 */
L8c6409bc: *stop=0x8c6409bcu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6409be; /* float FPUL,fr3 */
L8c6409be: *stop=0x8c6409beu;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c6409c0; /* fmov.s @r11,fr2 */
L8c6409c0: *stop=0x8c6409c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6409c2; /* fmul fr2,fr3 */
L8c6409c2: *stop=0x8c6409c2u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6409c4; /* ftrc fr3,FPUL */
L8c6409c4: *stop=0x8c6409c4u;r[12]=*fpul;goto L8c6409c6; /* sts FPUL,r12 */
L8c6409c6: *stop=0x8c6409c6u;*fpul=r[1];goto L8c6409c8; /* lds r1,FPUL */
L8c6409c8: *stop=0x8c6409c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a7cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6409ca; /* mov.l 0x8c640a7c,r1 */
L8c6409ca: *stop=0x8c6409cau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6409cc; /* float FPUL,fr3 */
L8c6409cc: *stop=0x8c6409ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6409ce; /* fmul fr2,fr3 */
L8c6409ce: *stop=0x8c6409ceu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6409d0; /* ftrc fr3,FPUL */
L8c6409d0: *stop=0x8c6409d0u;r[8]=*fpul;goto L8c6409d2; /* sts FPUL,r8 */
L8c6409d2: target=r[1];*pr=0x8c6409d6u;*stop=0x8c6409d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6409d6; /* jsr @r1 */
L8c6409d6: *stop=0x8c6409d6u;r[4]=r[0];goto L8c6409d8; /* mov r0,r4 */
L8c6409d8: *stop=0x8c6409d8u;r[9]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c6409da; /* exts.w r4,r9 */
L8c6409da: *stop=0x8c6409dau;*sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c6409dc; /* cmp/pl r9 */
L8c6409dc: *stop=0x8c6409dcu;r[10]=0u;goto L8c6409de; /* mov #0x0,r10 */
L8c6409de: take=!(*sr&1u);*stop=0x8c6409e0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}if(take)goto L8c640a00;goto L8c6409e2; /* bf/s 0x8c640a00 */
L8c6409e2: *stop=0x8c6409e2u;if(!valid_access(r[13],1u))return -3;r[6]=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[13]+=1;goto L8c6409e4; /* mov.b @r13+,r6 */
L8c6409e4: *stop=0x8c6409e4u;r[5]=r[8];goto L8c6409e6; /* mov r8,r5 */
L8c6409e6: target=0x8c6407b0u;*pr=0x8c6409eau;*stop=0x8c6409e8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6409ea; /* bsr 0x8c6407b0 */
L8c6409ea: *stop=0x8c6409eau;r[12]=(uint32_t)(int32_t)(int16_t)r[12];goto L8c6409ec; /* exts.w r12,r12 */
L8c6409ec: *stop=0x8c6409ecu;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c6409ee; /* fmov.s @r11,fr3 */
L8c6409ee: *stop=0x8c6409eeu;*fpul=r[12];goto L8c6409f0; /* lds r12,FPUL */
L8c6409f0: *stop=0x8c6409f0u;r[10]+=1u;goto L8c6409f2; /* add #0x1,r10 */
L8c6409f2: *stop=0x8c6409f2u;r[3]=(uint32_t)(int32_t)(int16_t)r[10];goto L8c6409f4; /* exts.w r10,r3 */
L8c6409f4: *stop=0x8c6409f4u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[9]);goto L8c6409f6; /* cmp/ge r9,r3 */
L8c6409f6: *stop=0x8c6409f6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6409f8; /* float FPUL,fr2 */
L8c6409f8: *stop=0x8c6409f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c6409fa; /* fadd fr3,fr2 */
L8c6409fa: *stop=0x8c6409fau;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c6409fc; /* ftrc fr2,FPUL */
L8c6409fc: take=!(*sr&1u);*stop=0x8c6409feu;r[12]=*fpul;if(take)goto L8c6409e2;goto L8c640a00; /* bf/s 0x8c6409e2 */
L8c640a00: *stop=0x8c640a00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a68u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c640a02; /* mov.l 0x8c640a68,r1 */
L8c640a02: *stop=0x8c640a02u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c640a04; /* mov.l @r14,r0 */
L8c640a04: *stop=0x8c640a04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a50u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c640a06; /* mov.l 0x8c640a50,r2 */
L8c640a06: *stop=0x8c640a06u;r[0]&=r[1];goto L8c640a08; /* and r1,r0 */
L8c640a08: *stop=0x8c640a08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c640a58u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c640a0a; /* mov.l 0x8c640a58,r1 */
L8c640a0a: *stop=0x8c640a0au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c640a0c; /* mov.l @r2,r3 */
L8c640a0c: *stop=0x8c640a0cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c640a0e; /* mov.l @r1,r2 */
L8c640a0e: *stop=0x8c640a0eu;r[0]|=r[2];goto L8c640a10; /* or r2,r0 */
L8c640a10: *stop=0x8c640a10u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[0],4);if(io_status)return io_status;goto L8c640a12; /* mov.l r0,@(0x8,r3) */
L8c640a12: *stop=0x8c640a12u;r[15]+=4u;goto L8c640a14; /* add #0x4,r15 */
L8c640a14: *stop=0x8c640a14u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c640a16; /* lds.l @r15+,PR */
L8c640a16: *stop=0x8c640a16u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c640a18; /* mov.l @r15+,r8 */
L8c640a18: *stop=0x8c640a18u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c640a1a; /* mov.l @r15+,r9 */
L8c640a1a: *stop=0x8c640a1au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c640a1c; /* mov.l @r15+,r10 */
L8c640a1c: *stop=0x8c640a1cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c640a1e; /* mov.l @r15+,r11 */
L8c640a1e: *stop=0x8c640a1eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c640a20; /* mov.l @r15+,r12 */
L8c640a20: *stop=0x8c640a20u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c640a22; /* mov.l @r15+,r13 */
L8c640a22: *stop=0x8c640a22u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c640a24u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_collision_text_8c640922(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_text_run_8c640922(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_collision_text_resume_8c6409d6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_text_run_8c640922(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6409d6u);}
SA_STAGE_EXPORT int sa_collision_text_resume_8c6409ea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_text_run_8c640922(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6409eau);}
