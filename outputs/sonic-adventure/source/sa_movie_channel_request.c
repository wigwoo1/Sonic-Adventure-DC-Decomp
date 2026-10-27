/* Original request locking/packet encoding and canonical diagnostic callback.
 * TAS is one uninterrupted guest operation on coherent RAM, with explicit purge.
 * Single guest executor only; host multithreading/cache/TLB hardware unqualified.
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
SA_STAGE_EXPORT int sa_movie_channel_request_8c660494(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c660494;
L8c660494: *stop=0x8c660494u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660610u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660496; /* mov.l 0x8c660610,r3 */
L8c660496: *stop=0x8c660496u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c660498; /* sts.l PR,@-r15 */
L8c660498: *stop=0x8c660498u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66049a; /* mov.l @r3,r0 */
L8c66049a: *stop=0x8c66049au;if(!valid_ram(r[0],1u))return -3;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[0],3u);if(status)return status;{uint32_t value=rd(m,bus,&io_status,r[0],1);if(io_status)return io_status;wr(m,bus,&io_status,r[0],value|0x80u,1);if(io_status)return io_status;*sr=(*sr&~1u)|(value==0);}goto L8c66049c; /* tas.b @r0 */
L8c66049c: if((*sr&1u))goto L8c6604ae;goto L8c66049e; /* bt 0x8c6604ae */
L8c66049e: *stop=0x8c66049eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c660618u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6604a0; /* mov.l 0x8c660618,r2 */
L8c6604a0: *stop=0x8c6604a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660614u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6604a2; /* mov.l 0x8c660614,r1 */
L8c6604a2: target=r[2];*pr=0x8c6604a6u;*stop=0x8c6604a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6604a6; /* jsr @r2 */
L8c6604a6: *stop=0x8c6604a6u;r[15]+=4u;goto L8c6604a8; /* add #0x4,r15 */
L8c6604a8: *stop=0x8c6604a8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6604aa; /* lds.l @r15+,PR */
L8c6604aa: *stop=0x8c6604aau;if(*pr!=entry_pr)return -3;*stop=0x8c6604acu;r[0]=4294967294u;return 0; /* rts */
L8c6604ae: target=0x8c6604c0u;*pr=0x8c6604b2u;*stop=0x8c6604b0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6604b2; /* bsr 0x8c6604c0 */
L8c6604b2: *stop=0x8c6604b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660610u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6604b4; /* mov.l 0x8c660610,r3 */
L8c6604b4: *stop=0x8c6604b4u;r[1]=0u;goto L8c6604b6; /* mov #0x0,r1 */
L8c6604b6: *stop=0x8c6604b6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6604b8; /* mov.l @r3,r2 */
L8c6604b8: *stop=0x8c6604b8u;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[1],1);if(io_status)return io_status;goto L8c6604ba; /* mov.b r1,@r2 */
L8c6604ba: *stop=0x8c6604bau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6604bc; /* lds.l @r15+,PR */
L8c6604bc: *stop=0x8c6604bcu;if(*pr!=entry_pr)return -3;*stop=0x8c6604beu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_channel_request_8c6604e6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6604e6;
L8c6604e6: *stop=0x8c6604e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660610u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6604e8; /* mov.l 0x8c660610,r3 */
L8c6604e8: *stop=0x8c6604e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6604ea; /* sts.l PR,@-r15 */
L8c6604ea: *stop=0x8c6604eau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6604ec; /* mov.l @r3,r0 */
L8c6604ec: *stop=0x8c6604ecu;if(!valid_ram(r[0],1u))return -3;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[0],3u);if(status)return status;{uint32_t value=rd(m,bus,&io_status,r[0],1);if(io_status)return io_status;wr(m,bus,&io_status,r[0],value|0x80u,1);if(io_status)return io_status;*sr=(*sr&~1u)|(value==0);}goto L8c6604ee; /* tas.b @r0 */
L8c6604ee: if((*sr&1u))goto L8c660500;goto L8c6604f0; /* bt 0x8c660500 */
L8c6604f0: *stop=0x8c6604f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660618u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6604f2; /* mov.l 0x8c660618,r2 */
L8c6604f2: *stop=0x8c6604f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660614u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6604f4; /* mov.l 0x8c660614,r1 */
L8c6604f4: target=r[2];*pr=0x8c6604f8u;*stop=0x8c6604f6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6604f8; /* jsr @r2 */
L8c6604f8: *stop=0x8c6604f8u;r[15]+=4u;goto L8c6604fa; /* add #0x4,r15 */
L8c6604fa: *stop=0x8c6604fau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6604fc; /* lds.l @r15+,PR */
L8c6604fc: *stop=0x8c6604fcu;if(*pr!=entry_pr)return -3;*stop=0x8c6604feu;r[0]=4294967294u;return 0; /* rts */
L8c660500: *stop=0x8c660500u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660502; /* mov.l @(0x4,r15),r3 */
L8c660502: *stop=0x8c660502u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660504; /* mov.l r3,@-r15 */
L8c660504: target=0x8c660518u;*pr=0x8c660508u;*stop=0x8c660506u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660508; /* bsr 0x8c660518 */
L8c660508: *stop=0x8c660508u;r[15]+=4u;goto L8c66050a; /* add #0x4,r15 */
L8c66050a: *stop=0x8c66050au;{uint32_t loaded=rd(m,bus,&io_status,0x8c660610u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66050c; /* mov.l 0x8c660610,r3 */
L8c66050c: *stop=0x8c66050cu;r[1]=0u;goto L8c66050e; /* mov #0x0,r1 */
L8c66050e: *stop=0x8c66050eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c660510; /* mov.l @r3,r2 */
L8c660510: *stop=0x8c660510u;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[1],1);if(io_status)return io_status;goto L8c660512; /* mov.b r1,@r2 */
L8c660512: *stop=0x8c660512u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660514; /* lds.l @r15+,PR */
L8c660514: *stop=0x8c660514u;if(*pr!=entry_pr)return -3;*stop=0x8c660516u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_channel_request_8c6604c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6604c0;
L8c6604c0: *stop=0x8c6604c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6604c2; /* mov.l r14,@-r15 */
L8c6604c2: *stop=0x8c6604c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6604c4; /* sts.l PR,@-r15 */
L8c6604c4: *stop=0x8c6604c4u;r[15]+=4294967288u;goto L8c6604c6; /* add #-0x8,r15 */
L8c6604c6: *stop=0x8c6604c6u;r[6]<<=16;goto L8c6604c8; /* shll16 r6 */
L8c6604c8: *stop=0x8c6604c8u;r[14]=r[15];goto L8c6604ca; /* mov r15,r14 */
L8c6604ca: *stop=0x8c6604cau;r[6]<<=8;goto L8c6604cc; /* shll8 r6 */
L8c6604cc: *stop=0x8c6604ccu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c6604ce; /* mov.l r5,@r14 */
L8c6604ce: *stop=0x8c6604ceu;r[3]=0u;goto L8c6604d0; /* mov #0x0,r3 */
L8c6604d0: *stop=0x8c6604d0u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[6],4);if(io_status)return io_status;goto L8c6604d2; /* mov.l r6,@(0x4,r14) */
L8c6604d2: *stop=0x8c6604d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6604d4; /* mov.l r3,@-r15 */
L8c6604d4: *stop=0x8c6604d4u;r[7]=2u;goto L8c6604d6; /* mov #0x2,r7 */
L8c6604d6: *stop=0x8c6604d6u;r[6]=r[14];goto L8c6604d8; /* mov r14,r6 */
L8c6604d8: *stop=0x8c6604d8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6604da; /* mov.l r3,@-r15 */
L8c6604da: target=0x8c65f810u;*pr=0x8c6604deu;*stop=0x8c6604dcu;r[5]=10u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6604de; /* bsr 0x8c65f810 */
L8c6604de: *stop=0x8c6604deu;r[15]+=16u;goto L8c6604e0; /* add #0x10,r15 */
L8c6604e0: *stop=0x8c6604e0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6604e2; /* lds.l @r15+,PR */
L8c6604e2: *stop=0x8c6604e2u;if(*pr!=entry_pr)return -3;*stop=0x8c6604e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_channel_request_8c660518(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c660518;
L8c660518: *stop=0x8c660518u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c66051a; /* mov.l r14,@-r15 */
L8c66051a: *stop=0x8c66051au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66051c; /* sts.l PR,@-r15 */
L8c66051c: *stop=0x8c66051cu;r[15]+=4294967288u;goto L8c66051e; /* add #-0x8,r15 */
L8c66051e: *stop=0x8c66051eu;r[6]<<=16;goto L8c660520; /* shll16 r6 */
L8c660520: *stop=0x8c660520u;r[14]=r[15];goto L8c660522; /* mov r15,r14 */
L8c660522: *stop=0x8c660522u;r[7]<<=16;goto L8c660524; /* shll16 r7 */
L8c660524: *stop=0x8c660524u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c660526; /* mov.l r5,@r14 */
L8c660526: *stop=0x8c660526u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660528; /* mov.l @(0x10,r15),r3 */
L8c660528: *stop=0x8c660528u;r[6]<<=8;goto L8c66052a; /* shll8 r6 */
L8c66052a: *stop=0x8c66052au;r[6]|=r[7];goto L8c66052c; /* or r7,r6 */
L8c66052c: *stop=0x8c66052cu;r[6]|=r[3];goto L8c66052e; /* or r3,r6 */
L8c66052e: *stop=0x8c66052eu;r[3]=0u;goto L8c660530; /* mov #0x0,r3 */
L8c660530: *stop=0x8c660530u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[6],4);if(io_status)return io_status;goto L8c660532; /* mov.l r6,@(0x4,r14) */
L8c660532: *stop=0x8c660532u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660534; /* mov.l r3,@-r15 */
L8c660534: *stop=0x8c660534u;r[7]=2u;goto L8c660536; /* mov #0x2,r7 */
L8c660536: *stop=0x8c660536u;r[6]=r[14];goto L8c660538; /* mov r14,r6 */
L8c660538: *stop=0x8c660538u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c66053a; /* mov.l r3,@-r15 */
L8c66053a: target=0x8c65f810u;*pr=0x8c66053eu;*stop=0x8c66053cu;r[5]=11u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66053e; /* bsr 0x8c65f810 */
L8c66053e: *stop=0x8c66053eu;r[15]+=16u;goto L8c660540; /* add #0x10,r15 */
L8c660540: *stop=0x8c660540u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660542; /* lds.l @r15+,PR */
L8c660542: *stop=0x8c660542u;if(*pr!=entry_pr)return -3;*stop=0x8c660544u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_channel_request_8c65f348(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f348;
L8c65f348: *stop=0x8c65f348u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f3d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f34a; /* mov.l 0x8c65f3d8,r2 */
L8c65f34a: *stop=0x8c65f34au;r[0]=sx(m,bus,&io_status,0x8c65f3c8u,2);if(io_status)return io_status;goto L8c65f34c; /* mov.w 0x8c65f3c8,r0 */
L8c65f34c: *stop=0x8c65f34cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65f34e; /* mov.l r14,@-r15 */
L8c65f34e: *stop=0x8c65f34eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65f350; /* sts.l PR,@-r15 */
L8c65f350: *stop=0x8c65f350u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f352; /* mov.l @r2,r3 */
L8c65f352: *stop=0x8c65f352u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f354; /* mov.l @(r0,r3),r1 */
L8c65f354: *stop=0x8c65f354u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c65f356; /* tst r1,r1 */
L8c65f356: if((*sr&1u))goto L8c65f38a;goto L8c65f358; /* bt 0x8c65f38a */
L8c65f358: *stop=0x8c65f358u;r[0]=r[15];goto L8c65f35a; /* mov r15,r0 */
L8c65f35a: *stop=0x8c65f35au;r[0]+=8u;goto L8c65f35c; /* add #0x8,r0 */
L8c65f35c: *stop=0x8c65f35cu;r[5]=3u;goto L8c65f35e; /* mov #0x3,r5 */
L8c65f35e: *stop=0x8c65f35eu;r[0]+=4u;goto L8c65f360; /* add #0x4,r0 */
L8c65f360: *stop=0x8c65f360u;*sr=(*sr&~1u)|!(r[0]&r[5]);goto L8c65f362; /* tst r0,r5 */
L8c65f362: if((*sr&1u))goto L8c65f36c;goto L8c65f364; /* bt 0x8c65f36c */
L8c65f364: *stop=0x8c65f364u;r[5]=r[15];goto L8c65f366; /* mov r15,r5 */
L8c65f366: *stop=0x8c65f366u;r[5]+=8u;goto L8c65f368; /* add #0x8,r5 */
L8c65f368: *stop=0x8c65f36au;r[5]+=8u;goto L8c65f372; /* bra 0x8c65f372 */
L8c65f36c: *stop=0x8c65f36cu;r[5]=r[15];goto L8c65f36e; /* mov r15,r5 */
L8c65f36e: *stop=0x8c65f36eu;r[5]+=8u;goto L8c65f370; /* add #0x8,r5 */
L8c65f370: *stop=0x8c65f370u;r[5]+=4u;goto L8c65f372; /* add #0x4,r5 */
L8c65f372: *stop=0x8c65f372u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f3ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f374; /* mov.l 0x8c65f3ec,r3 */
L8c65f374: *stop=0x8c65f374u;r[6]=r[5];goto L8c65f376; /* mov r5,r6 */
L8c65f376: *stop=0x8c65f376u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f3e8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f378; /* mov.l 0x8c65f3e8,r4 */
L8c65f378: target=r[3];*pr=0x8c65f37cu;*stop=0x8c65f37au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f37c; /* jsr @r3 */
L8c65f37c: *stop=0x8c65f37cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f3f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f37e; /* mov.l 0x8c65f3f0,r2 */
L8c65f37e: *stop=0x8c65f37eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[14]=loaded;}goto L8c65f380; /* mov.l @r2,r14 */
L8c65f380: *stop=0x8c65f380u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65f382; /* tst r14,r14 */
L8c65f382: if((*sr&1u))goto L8c65f38a;goto L8c65f384; /* bt 0x8c65f38a */
L8c65f384: *stop=0x8c65f384u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f3e8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f386; /* mov.l 0x8c65f3e8,r4 */
L8c65f386: target=r[14];*pr=0x8c65f38au;*stop=0x8c65f388u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f38a; /* jsr @r14 */
L8c65f38a: *stop=0x8c65f38au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f38c; /* lds.l @r15+,PR */
L8c65f38c: *stop=0x8c65f38cu;if(*pr!=entry_pr)return -3;*stop=0x8c65f38eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
