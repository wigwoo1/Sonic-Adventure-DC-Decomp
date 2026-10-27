/* Canonical input pump, response decoding and status publication integration.
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
SA_STAGE_EXPORT int sa_movie_input_pump_8c646654(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c646654;
L8c646654: *stop=0x8c646654u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c646656; /* mov.l r14,@-r15 */
L8c646656: *stop=0x8c646656u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c646658; /* mov.l r13,@-r15 */
L8c646658: *stop=0x8c646658u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64665a; /* mov.l r12,@-r15 */
L8c64665a: *stop=0x8c64665au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64665c; /* mov.l r11,@-r15 */
L8c64665c: *stop=0x8c64665cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64665e; /* mov.l r10,@-r15 */
L8c64665e: *stop=0x8c64665eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c646660; /* mov.l r9,@-r15 */
L8c646660: *stop=0x8c646660u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c646662; /* mov.l r8,@-r15 */
L8c646662: *stop=0x8c646662u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c646664; /* sts.l PR,@-r15 */
L8c646664: *stop=0x8c646664u;r[15]+=4294967292u;goto L8c646666; /* add #-0x4,r15 */
L8c646666: *stop=0x8c646666u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646738u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646668; /* mov.l 0x8c646738,r2 */
L8c646668: *stop=0x8c646668u;r[10]=1u;goto L8c64666a; /* mov #0x1,r10 */
L8c64666a: *stop=0x8c64666au;{uint32_t loaded=rd(m,bus,&io_status,0x8c646734u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64666c; /* mov.l 0x8c646734,r3 */
L8c64666c: *stop=0x8c64666cu;r[13]=0u;goto L8c64666e; /* mov #0x0,r13 */
L8c64666e: *stop=0x8c64666eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64672cu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c646670; /* mov.l 0x8c64672c,r8 */
L8c646670: *stop=0x8c646670u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646730u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c646672; /* mov.l 0x8c646730,r9 */
L8c646672: target=r[2];*pr=0x8c646676u;*stop=0x8c646674u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[10],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646676; /* jsr @r2 */
L8c646676: *stop=0x8c646676u;*sr=(*sr&~1u)|(r[0]==0xfffffffeu);goto L8c646678; /* cmp/eq #-0x2,r0 */
L8c646678: take=!(*sr&1u);*stop=0x8c64667au;r[12]=r[0];if(take)goto L8c646684;goto L8c64667c; /* bf/s 0x8c646684 */
L8c64667c: *stop=0x8c64667cu;r[0]=sx(m,bus,&io_status,0x8c646726u,2);if(io_status)return io_status;goto L8c64667e; /* mov.w 0x8c646726,r0 */
L8c64667e: *stop=0x8c64667eu;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646680; /* mov.l @r8,r2 */
L8c646680: *stop=0x8c646682u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[10],4);if(io_status)return io_status;goto L8c6467da; /* bra 0x8c6467da */
L8c646684: *stop=0x8c646684u;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c646686; /* cmp/pz r12 */
L8c646686: if((*sr&1u))goto L8c64668c;goto L8c646688; /* bt 0x8c64668c */
L8c646688: *stop=0x8c64668au;goto L8c6467c4; /* bra 0x8c6467c4 */
L8c64668c: *stop=0x8c64668cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64673cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64668e; /* mov.l 0x8c64673c,r3 */
L8c64668e: target=r[3];*pr=0x8c646692u;*stop=0x8c646690u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646692; /* jsr @r3 */
L8c646692: *stop=0x8c646692u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646740u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646694; /* mov.l 0x8c646740,r2 */
L8c646694: *stop=0x8c646694u;r[3]=r[9];goto L8c646696; /* mov r9,r3 */
L8c646696: *stop=0x8c646696u;r[3]+=48u;goto L8c646698; /* add #0x30,r3 */
L8c646698: *stop=0x8c646698u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646744u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64669a; /* mov.l 0x8c646744,r0 */
L8c64669a: *stop=0x8c64669au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64669c; /* mov.l @r2,r4 */
L8c64669c: *stop=0x8c64669cu;r[5]=sx(m,bus,&io_status,0x8c646728u,2);if(io_status)return io_status;goto L8c64669e; /* mov.w 0x8c646728,r5 */
L8c64669e: *stop=0x8c64669eu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6466a0; /* shll r4 */
L8c6466a0: *stop=0x8c6466a0u;r[3]+=r[4];goto L8c6466a2; /* add r4,r3 */
L8c6466a2: *stop=0x8c6466a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6466a4; /* mov.l r3,@-r15 */
L8c6466a4: *stop=0x8c6466a4u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6466a6; /* mov.l @r0,r1 */
L8c6466a6: *stop=0x8c6466a6u;r[2]=r[9];goto L8c6466a8; /* mov r9,r2 */
L8c6466a8: *stop=0x8c6466a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646748u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6466aa; /* mov.l 0x8c646748,r3 */
L8c6466aa: *stop=0x8c6466aau;r[2]+=52u;goto L8c6466ac; /* add #0x34,r2 */
L8c6466ac: *stop=0x8c6466acu;r[1]&=r[5];goto L8c6466ae; /* and r5,r1 */
L8c6466ae: *stop=0x8c6466aeu;r[2]+=r[4];goto L8c6466b0; /* add r4,r2 */
L8c6466b0: *stop=0x8c6466b0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6466b2; /* mov.l @r3,r0 */
L8c6466b2: *stop=0x8c6466b2u;if(!valid_access(r[2],2u))return -3;r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c6466b4; /* mov.w @r2,r2 */
L8c6466b4: *stop=0x8c6466b4u;r[0]&=r[5];goto L8c6466b6; /* and r5,r0 */
L8c6466b6: *stop=0x8c6466b6u;*macl=(r[0]&65535u)*(r[2]&65535u);goto L8c6466b8; /* mulu.w r0,r2 */
L8c6466b8: *stop=0x8c6466b8u;r[0]=*macl;goto L8c6466ba; /* sts MACL,r0 */
L8c6466ba: *stop=0x8c6466bau;r[1]+=r[0];goto L8c6466bc; /* add r0,r1 */
L8c6466bc: *stop=0x8c6466bcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}r[15]+=4;goto L8c6466be; /* mov.l @r15+,r0 */
L8c6466be: target=0x8c6461aeu;*pr=0x8c6466c2u;*stop=0x8c6466c0u;if(!valid_access(r[0],2u))return -3;wr(m,bus,&io_status,r[0],r[1],2);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6466c2; /* bsr 0x8c6461ae */
L8c6466c2: *stop=0x8c6466c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64674cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6466c4; /* mov.l 0x8c64674c,r3 */
L8c6466c4: target=r[3];*pr=0x8c6466c8u;*stop=0x8c6466c6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6466c8; /* jsr @r3 */
L8c6466c8: *stop=0x8c6466c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646750u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6466ca; /* mov.l 0x8c646750,r2 */
L8c6466ca: target=r[2];*pr=0x8c6466ceu;*stop=0x8c6466ccu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6466ce; /* jsr @r2 */
L8c6466ce: *stop=0x8c6466ceu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6466d0; /* tst r0,r0 */
L8c6466d0: if((*sr&1u))goto L8c646720;goto L8c6466d2; /* bt 0x8c646720 */
L8c6466d2: *stop=0x8c6466d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646754u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6466d4; /* mov.l 0x8c646754,r2 */
L8c6466d4: *stop=0x8c6466d4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[14]=loaded;}goto L8c6466d6; /* mov.l @r2,r14 */
L8c6466d6: *stop=0x8c6466d6u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6466d8; /* tst r14,r14 */
L8c6466d8: if((*sr&1u))goto L8c6466e2;goto L8c6466da; /* bt 0x8c6466e2 */
L8c6466da: target=r[14];*pr=0x8c6466deu;*stop=0x8c6466dcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6466de; /* jsr @r14 */
L8c6466de: *stop=0x8c6466deu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6466e0; /* tst r0,r0 */
L8c6466e0: if((*sr&1u))goto L8c6466ea;goto L8c6466e2; /* bt 0x8c6466ea */
L8c6466e2: target=0x8c64658au;*pr=0x8c6466e6u;*stop=0x8c6466e4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6466e6; /* bsr 0x8c64658a */
L8c6466e6: *stop=0x8c6466e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646758u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6466e8; /* mov.l 0x8c646758,r1 */
L8c6466e8: *stop=0x8c6466e8u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c6466ea; /* mov.l r0,@r1 */
L8c6466ea: *stop=0x8c6466eau;r[0]=sx(m,bus,&io_status,0x8c64672au,2);if(io_status)return io_status;goto L8c6466ec; /* mov.w 0x8c64672a,r0 */
L8c6466ec: *stop=0x8c6466ecu;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6466ee; /* mov.l @r8,r3 */
L8c6466ee: *stop=0x8c6466eeu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6466f0; /* mov.l @(r0,r3),r2 */
L8c6466f0: *stop=0x8c6466f0u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6466f2; /* tst r2,r2 */
L8c6466f2: if((*sr&1u))goto L8c646720;goto L8c6466f4; /* bt 0x8c646720 */
L8c6466f4: *stop=0x8c6466f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646758u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6466f6; /* mov.l 0x8c646758,r1 */
L8c6466f6: *stop=0x8c6466f6u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6466f8; /* mov.l @r1,r2 */
L8c6466f8: *stop=0x8c6466f8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6466fa; /* tst r2,r2 */
L8c6466fa: if((*sr&1u))goto L8c646720;goto L8c6466fc; /* bt 0x8c646720 */
L8c6466fc: *stop=0x8c6466fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64675cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6466fe; /* mov.l 0x8c64675c,r4 */
L8c6466fe: *stop=0x8c6466feu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c646700; /* mov.l @r4,r3 */
L8c646700: *stop=0x8c646700u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646760u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646702; /* mov.l 0x8c646760,r2 */
L8c646702: *stop=0x8c646702u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c646704; /* mov.l r3,@r2 */
L8c646704: *stop=0x8c646704u;r[3]=sx(m,bus,&io_status,0x8c64672au,2);if(io_status)return io_status;goto L8c646706; /* mov.w 0x8c64672a,r3 */
L8c646706: *stop=0x8c646706u;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[0]=loaded;}goto L8c646708; /* mov.l @r8,r0 */
L8c646708: *stop=0x8c646708u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64670a; /* mov.l @(r0,r3),r3 */
L8c64670a: *stop=0x8c64670au;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c64670c; /* mov.l r3,@r4 */
L8c64670c: *stop=0x8c64670cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c646764u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64670e; /* mov.l 0x8c646764,r3 */
L8c64670e: *stop=0x8c64670eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c646710; /* mov.l @r3,r0 */
L8c646710: *stop=0x8c646710u;r[0]|=0x8u;goto L8c646712; /* or #0x8,r0 */
L8c646712: *stop=0x8c646712u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c646714; /* mov.l r0,@r3 */
L8c646714: *stop=0x8c646714u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64676cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c646716; /* mov.l 0x8c64676c,r4 */
L8c646716: *stop=0x8c646716u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646768u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c646718; /* mov.l 0x8c646768,r1 */
L8c646718: *stop=0x8c646718u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[10],4);if(io_status)return io_status;goto L8c64671a; /* mov.l r10,@r1 */
L8c64671a: *stop=0x8c64671au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64671c; /* mov.l @r4,r3 */
L8c64671c: *stop=0x8c64671cu;r[3]+=1u;goto L8c64671e; /* add #0x1,r3 */
L8c64671e: *stop=0x8c64671eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c646720; /* mov.l r3,@r4 */
L8c646720: *stop=0x8c646720u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646770u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c646722; /* mov.l 0x8c646770,r14 */
L8c646722: *stop=0x8c646724u;r[11]=r[13];goto L8c6467a8; /* bra 0x8c6467a8 */
L8c646774: *stop=0x8c646774u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646776; /* mov.l @(0x8,r14),r3 */
L8c646776: *stop=0x8c646776u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c646778; /* tst r3,r3 */
L8c646778: if((*sr&1u))goto L8c6467a4;goto L8c64677a; /* bt 0x8c6467a4 */
L8c64677a: *stop=0x8c64677au;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64677c; /* mov.l @(0x4,r14),r5 */
L8c64677c: target=0x8c646884u;*pr=0x8c646780u;*stop=0x8c64677eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646780; /* bsr 0x8c646884 */
L8c646780: *stop=0x8c646780u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c646782; /* tst r0,r0 */
L8c646782: if((*sr&1u))goto L8c6467a4;goto L8c646784; /* bt 0x8c6467a4 */
L8c646784: *stop=0x8c646784u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646786; /* mov.l @(0x8,r14),r3 */
L8c646786: *stop=0x8c646786u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c646788; /* mov.l r3,@r15 */
L8c646788: *stop=0x8c646788u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[13],4);if(io_status)return io_status;goto L8c64678a; /* mov.l r13,@(0x8,r14) */
L8c64678a: *stop=0x8c64678au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64678c; /* mov.l @r15,r3 */
L8c64678c: *stop=0x8c64678cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64678e; /* mov.l r3,@-r15 */
L8c64678e: target=0x8c646868u;*pr=0x8c646792u;*stop=0x8c646790u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646792; /* bsr 0x8c646868 */
L8c646792: *stop=0x8c646792u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c646794; /* mov.l r0,@-r15 */
L8c646794: target=0x8c646856u;*pr=0x8c646798u;*stop=0x8c646796u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646798; /* bsr 0x8c646856 */
L8c646798: *stop=0x8c646798u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[7]=loaded;}r[15]+=4;goto L8c64679a; /* mov.l @r15+,r7 */
L8c64679a: *stop=0x8c64679au;r[6]=r[0];goto L8c64679c; /* mov r0,r6 */
L8c64679c: *stop=0x8c64679cu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64679e; /* mov.l @(0x4,r14),r5 */
L8c64679e: *stop=0x8c64679eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}r[15]+=4;goto L8c6467a0; /* mov.l @r15+,r1 */
L8c6467a0: target=r[1];*pr=0x8c6467a4u;*stop=0x8c6467a2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6467a4; /* jsr @r1 */
L8c6467a4: *stop=0x8c6467a4u;r[14]+=12u;goto L8c6467a6; /* add #0xc,r14 */
L8c6467a6: *stop=0x8c6467a6u;r[11]+=1u;goto L8c6467a8; /* add #0x1,r11 */
L8c6467a8: *stop=0x8c6467a8u;r[3]=48u;goto L8c6467aa; /* mov #0x30,r3 */
L8c6467aa: *stop=0x8c6467aau;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]);goto L8c6467ac; /* cmp/ge r3,r11 */
L8c6467ac: if(!(*sr&1u))goto L8c646774;goto L8c6467ae; /* bf 0x8c646774 */
L8c6467ae: *stop=0x8c6467aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c646894u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6467b0; /* mov.l 0x8c646894,r3 */
L8c6467b0: *stop=0x8c6467b0u;r[0]=sx(m,bus,&io_status,0x8c64688au,2);if(io_status)return io_status;goto L8c6467b2; /* mov.w 0x8c64688a,r0 */
L8c6467b2: *stop=0x8c6467b2u;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6467b4; /* mov.l @r8,r1 */
L8c6467b4: *stop=0x8c6467b4u;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[13],4);if(io_status)return io_status;goto L8c6467b6; /* mov.l r13,@(r0,r1) */
L8c6467b6: *stop=0x8c6467b6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6467b8; /* mov.l @r3,r2 */
L8c6467b8: *stop=0x8c6467b8u;r[2]^=r[10];goto L8c6467ba; /* xor r10,r2 */
L8c6467ba: *stop=0x8c6467bau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6467bc; /* mov.l r2,@r3 */
L8c6467bc: *stop=0x8c6467bcu;if(!valid_access(r[9]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6467be; /* mov.l @(0x4,r9),r3 */
L8c6467be: *stop=0x8c6467beu;r[3]+=1u;goto L8c6467c0; /* add #0x1,r3 */
L8c6467c0: *stop=0x8c6467c2u;if(!valid_access(r[9]+4u,4u))return -3;wr(m,bus,&io_status,r[9]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6467d6; /* bra 0x8c6467d6 */
L8c6467c4: *stop=0x8c6467c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646898u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6467c6; /* mov.l 0x8c646898,r1 */
L8c6467c6: *stop=0x8c6467c6u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[14]=loaded;}goto L8c6467c8; /* mov.l @r1,r14 */
L8c6467c8: *stop=0x8c6467c8u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6467ca; /* tst r14,r14 */
L8c6467ca: if((*sr&1u))goto L8c6467d6;goto L8c6467cc; /* bt 0x8c6467d6 */
L8c6467cc: *stop=0x8c6467ccu;if(!valid_access(r[9]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6467ce; /* mov.l @(0x8,r9),r6 */
L8c6467ce: *stop=0x8c6467ceu;r[5]=4294967288u;goto L8c6467d0; /* mov #-0x8,r5 */
L8c6467d0: *stop=0x8c6467d0u;r[7]=r[12];goto L8c6467d2; /* mov r12,r7 */
L8c6467d2: target=r[14];*pr=0x8c6467d6u;*stop=0x8c6467d4u;r[4]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6467d6; /* jsr @r14 */
L8c6467d6: *stop=0x8c6467d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64689cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6467d8; /* mov.l 0x8c64689c,r2 */
L8c6467d8: *stop=0x8c6467d8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c6467da; /* mov.l r13,@r2 */
L8c6467da: *stop=0x8c6467dau;r[0]=r[12];goto L8c6467dc; /* mov r12,r0 */
L8c6467dc: *stop=0x8c6467dcu;r[15]+=4u;goto L8c6467de; /* add #0x4,r15 */
L8c6467de: *stop=0x8c6467deu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6467e0; /* lds.l @r15+,PR */
L8c6467e0: *stop=0x8c6467e0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6467e2; /* mov.l @r15+,r8 */
L8c6467e2: *stop=0x8c6467e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6467e4; /* mov.l @r15+,r9 */
L8c6467e4: *stop=0x8c6467e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6467e6; /* mov.l @r15+,r10 */
L8c6467e6: *stop=0x8c6467e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6467e8; /* mov.l @r15+,r11 */
L8c6467e8: *stop=0x8c6467e8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6467ea; /* mov.l @r15+,r12 */
L8c6467ea: *stop=0x8c6467eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6467ec; /* mov.l @r15+,r13 */
L8c6467ec: *stop=0x8c6467ecu;if(*pr!=entry_pr)return -3;*stop=0x8c6467eeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c65f1aa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f1aa;
L8c65f1aa: *stop=0x8c65f1aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f290u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f1ac; /* mov.l 0x8c65f290,r3 */
L8c65f1ac: *stop=0x8c65f1acu;r[0]=sx(m,bus,&io_status,0x8c65f286u,2);if(io_status)return io_status;goto L8c65f1ae; /* mov.w 0x8c65f286,r0 */
L8c65f1ae: *stop=0x8c65f1aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65f1b0; /* sts.l PR,@-r15 */
L8c65f1b0: *stop=0x8c65f1b0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65f1b2; /* mov.l @r3,r4 */
L8c65f1b2: *stop=0x8c65f1b2u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f1b4; /* mov.l @(r0,r4),r2 */
L8c65f1b4: *stop=0x8c65f1b4u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65f1b6; /* tst r2,r2 */
L8c65f1b6: if((*sr&1u))goto L8c65f1be;goto L8c65f1b8; /* bt 0x8c65f1be */
L8c65f1b8: *stop=0x8c65f1b8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f1ba; /* lds.l @r15+,PR */
L8c65f1ba: *stop=0x8c65f1bau;if(*pr!=entry_pr)return -3;*stop=0x8c65f1bcu;r[0]=4294967293u;return 0; /* rts */
L8c65f1be: *stop=0x8c65f1beu;r[0]=sx(m,bus,&io_status,0x8c65f288u,2);if(io_status)return io_status;goto L8c65f1c0; /* mov.w 0x8c65f288,r0 */
L8c65f1c0: *stop=0x8c65f1c0u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f1c2; /* mov.l @(r0,r4),r3 */
L8c65f1c2: *stop=0x8c65f1c2u;if(!valid_access(r[3]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f1c4; /* mov.l @(0x18,r3),r0 */
L8c65f1c4: *stop=0x8c65f1c4u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c65f1c6; /* tst #0x1,r0 */
L8c65f1c6: if((*sr&1u))goto L8c65f1ce;goto L8c65f1c8; /* bt 0x8c65f1ce */
L8c65f1c8: *stop=0x8c65f1c8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f1ca; /* lds.l @r15+,PR */
L8c65f1ca: *stop=0x8c65f1cau;if(*pr!=entry_pr)return -3;*stop=0x8c65f1ccu;r[0]=4294967295u;return 0; /* rts */
L8c65f1ce: *stop=0x8c65f1ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f28cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f1d0; /* mov.l 0x8c65f28c,r3 */
L8c65f1d0: *stop=0x8c65f1d0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f1d2; /* mov.l @r3,r0 */
L8c65f1d2: *stop=0x8c65f1d2u;if(!valid_ram(r[0],1u))return -3;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[0],3u);if(status)return status;{uint32_t value=rd(m,bus,&io_status,r[0],1);if(io_status)return io_status;wr(m,bus,&io_status,r[0],value|0x80u,1);if(io_status)return io_status;*sr=(*sr&~1u)|(value==0);}goto L8c65f1d4; /* tas.b @r0 */
L8c65f1d4: if((*sr&1u))goto L8c65f1e4;goto L8c65f1d6; /* bt 0x8c65f1e4 */
L8c65f1d6: *stop=0x8c65f1d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f294u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f1d8; /* mov.l 0x8c65f294,r1 */
L8c65f1d8: target=0x8c65f348u;*pr=0x8c65f1dcu;*stop=0x8c65f1dau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f1dc; /* bsr 0x8c65f348 */
L8c65f1dc: *stop=0x8c65f1dcu;r[15]+=4u;goto L8c65f1de; /* add #0x4,r15 */
L8c65f1de: *stop=0x8c65f1deu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f1e0; /* lds.l @r15+,PR */
L8c65f1e0: *stop=0x8c65f1e0u;if(*pr!=entry_pr)return -3;*stop=0x8c65f1e2u;r[0]=4294967294u;return 0; /* rts */
L8c65f1e4: *stop=0x8c65f1e4u;r[0]=0u;goto L8c65f1e6; /* mov #0x0,r0 */
L8c65f1e6: *stop=0x8c65f1e6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f1e8; /* lds.l @r15+,PR */
L8c65f1e8: *stop=0x8c65f1e8u;if(*pr!=entry_pr)return -3;*stop=0x8c65f1eau;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c65f202(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65f202;
L8c65f202: *stop=0x8c65f202u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65f204; /* mov.l r14,@-r15 */
L8c65f204: *stop=0x8c65f204u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65f206; /* mov.l r13,@-r15 */
L8c65f206: *stop=0x8c65f206u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65f208; /* mov.l r12,@-r15 */
L8c65f208: *stop=0x8c65f208u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65f20a; /* mov.l r11,@-r15 */
L8c65f20a: *stop=0x8c65f20au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65f20c; /* mov.l r10,@-r15 */
L8c65f20c: *stop=0x8c65f20cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65f20e; /* mov.l r9,@-r15 */
L8c65f20e: *stop=0x8c65f20eu;r[9]=6u;goto L8c65f210; /* mov #0x6,r9 */
L8c65f210: *stop=0x8c65f210u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65f212; /* mov.l r8,@-r15 */
L8c65f212: *stop=0x8c65f212u;r[8]=0u;goto L8c65f214; /* mov #0x0,r8 */
L8c65f214: *stop=0x8c65f214u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65f216; /* sts.l PR,@-r15 */
L8c65f216: *stop=0x8c65f216u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f290u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c65f218; /* mov.l 0x8c65f290,r10 */
L8c65f218: *stop=0x8c65f21au;r[14]=r[8];goto L8c65f26a; /* bra 0x8c65f26a */
L8c65f21c: *stop=0x8c65f21cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f298u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f21e; /* mov.l 0x8c65f298,r0 */
L8c65f21e: *stop=0x8c65f21eu;r[3]=r[14];goto L8c65f220; /* mov r14,r3 */
L8c65f220: *stop=0x8c65f220u;r[3]<<=2;goto L8c65f222; /* shll2 r3 */
L8c65f222: *stop=0x8c65f222u;r[1]=sx(m,bus,&io_status,0x8c65f284u,2);if(io_status)return io_status;goto L8c65f224; /* mov.w 0x8c65f284,r1 */
L8c65f224: *stop=0x8c65f224u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f226; /* mov.l @(r0,r3),r2 */
L8c65f226: *stop=0x8c65f226u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f228; /* mov.l @r10,r0 */
L8c65f228: *stop=0x8c65f228u;r[1]+=r[0];goto L8c65f22a; /* add r0,r1 */
L8c65f22a: *stop=0x8c65f22au;r[2]+=r[1];goto L8c65f22c; /* add r1,r2 */
L8c65f22c: *stop=0x8c65f22cu;if(!valid_access(r[2],1u))return -3;r[3]=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;goto L8c65f22e; /* mov.b @r2,r3 */
L8c65f22e: *stop=0x8c65f22eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65f230; /* tst r3,r3 */
L8c65f230: if((*sr&1u))goto L8c65f268;goto L8c65f232; /* bt 0x8c65f268 */
L8c65f232: *stop=0x8c65f232u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f29cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f234; /* mov.l 0x8c65f29c,r2 */
L8c65f234: target=r[2];*pr=0x8c65f238u;*stop=0x8c65f236u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f238; /* jsr @r2 */
L8c65f238: *stop=0x8c65f238u;r[3]=r[14];goto L8c65f23a; /* mov r14,r3 */
L8c65f23a: *stop=0x8c65f23au;r[4]=r[14];goto L8c65f23c; /* mov r14,r4 */
L8c65f23c: *stop=0x8c65f23cu;r[3]<<=2;goto L8c65f23e; /* shll2 r3 */
L8c65f23e: *stop=0x8c65f23eu;r[11]=r[8];goto L8c65f240; /* mov r8,r11 */
L8c65f240: *stop=0x8c65f240u;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c65f242; /* shll r4 */
L8c65f242: *stop=0x8c65f242u;r[4]+=r[3];goto L8c65f244; /* add r3,r4 */
L8c65f244: *stop=0x8c65f244u;r[12]=r[4];goto L8c65f246; /* mov r4,r12 */
L8c65f246: *stop=0x8c65f246u;r[13]=r[4];goto L8c65f248; /* mov r4,r13 */
L8c65f248: *stop=0x8c65f248u;r[3]=sx(m,bus,&io_status,0x8c65f284u,2);if(io_status)return io_status;goto L8c65f24a; /* mov.w 0x8c65f284,r3 */
L8c65f24a: *stop=0x8c65f24au;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f24c; /* mov.l @r10,r0 */
L8c65f24c: *stop=0x8c65f24cu;r[3]+=r[0];goto L8c65f24e; /* add r0,r3 */
L8c65f24e: *stop=0x8c65f24eu;r[3]+=r[12];goto L8c65f250; /* add r12,r3 */
L8c65f250: *stop=0x8c65f250u;if(!valid_access(r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;goto L8c65f252; /* mov.b @r3,r2 */
L8c65f252: *stop=0x8c65f252u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65f254; /* tst r2,r2 */
L8c65f254: if((*sr&1u))goto L8c65f25e;goto L8c65f256; /* bt 0x8c65f25e */
L8c65f256: *stop=0x8c65f256u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f2a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f258; /* mov.l 0x8c65f2a0,r3 */
L8c65f258: *stop=0x8c65f258u;r[5]=r[14];goto L8c65f25a; /* mov r14,r5 */
L8c65f25a: target=r[3];*pr=0x8c65f25eu;*stop=0x8c65f25cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f25e; /* jsr @r3 */
L8c65f25e: *stop=0x8c65f25eu;r[11]+=1u;goto L8c65f260; /* add #0x1,r11 */
L8c65f260: *stop=0x8c65f260u;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[9]);goto L8c65f262; /* cmp/ge r9,r11 */
L8c65f262: *stop=0x8c65f262u;r[13]+=1u;goto L8c65f264; /* add #0x1,r13 */
L8c65f264: take=!(*sr&1u);*stop=0x8c65f266u;r[12]+=1u;if(take)goto L8c65f248;goto L8c65f268; /* bf/s 0x8c65f248 */
L8c65f268: *stop=0x8c65f268u;r[14]+=1u;goto L8c65f26a; /* add #0x1,r14 */
L8c65f26a: *stop=0x8c65f26au;r[3]=4u;goto L8c65f26c; /* mov #0x4,r3 */
L8c65f26c: *stop=0x8c65f26cu;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[3]);goto L8c65f26e; /* cmp/ge r3,r14 */
L8c65f26e: if(!(*sr&1u))goto L8c65f21c;goto L8c65f270; /* bf 0x8c65f21c */
L8c65f270: *stop=0x8c65f270u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f272; /* lds.l @r15+,PR */
L8c65f272: *stop=0x8c65f272u;r[0]=0u;goto L8c65f274; /* mov #0x0,r0 */
L8c65f274: *stop=0x8c65f274u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65f276; /* mov.l @r15+,r8 */
L8c65f276: *stop=0x8c65f276u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65f278; /* mov.l @r15+,r9 */
L8c65f278: *stop=0x8c65f278u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65f27a; /* mov.l @r15+,r10 */
L8c65f27a: *stop=0x8c65f27au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65f27c; /* mov.l @r15+,r11 */
L8c65f27c: *stop=0x8c65f27cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65f27e; /* mov.l @r15+,r12 */
L8c65f27e: *stop=0x8c65f27eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65f280; /* mov.l @r15+,r13 */
L8c65f280: *stop=0x8c65f280u;if(*pr!=entry_pr)return -3;*stop=0x8c65f282u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c65f1ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f1ec;
L8c65f1ec: *stop=0x8c65f1ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f290u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f1ee; /* mov.l 0x8c65f290,r2 */
L8c65f1ee: *stop=0x8c65f1eeu;r[1]=1u;goto L8c65f1f0; /* mov #0x1,r1 */
L8c65f1f0: *stop=0x8c65f1f0u;r[0]=sx(m,bus,&io_status,0x8c65f286u,2);if(io_status)return io_status;goto L8c65f1f2; /* mov.w 0x8c65f286,r0 */
L8c65f1f2: *stop=0x8c65f1f2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f1f4; /* mov.l @r2,r3 */
L8c65f1f4: *stop=0x8c65f1f4u;r[2]=0u;goto L8c65f1f6; /* mov #0x0,r2 */
L8c65f1f6: *stop=0x8c65f1f6u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[1],4);if(io_status)return io_status;goto L8c65f1f8; /* mov.l r1,@(r0,r3) */
L8c65f1f8: *stop=0x8c65f1f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f28cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65f1fa; /* mov.l 0x8c65f28c,r1 */
L8c65f1fa: *stop=0x8c65f1fau;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f1fc; /* mov.l @r1,r3 */
L8c65f1fc: *stop=0x8c65f1fcu;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[2],1);if(io_status)return io_status;goto L8c65f1fe; /* mov.b r2,@r3 */
L8c65f1fe: *stop=0x8c65f1feu;if(*pr!=entry_pr)return -3;*stop=0x8c65f200u;r[0]=r[2];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c64658a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64658a;
L8c64658a: *stop=0x8c64658au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64658c; /* mov.l r14,@-r15 */
L8c64658c: *stop=0x8c64658cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64658e; /* mov.l r13,@-r15 */
L8c64658e: *stop=0x8c64658eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c646590; /* mov.l r12,@-r15 */
L8c646590: *stop=0x8c646590u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c646592; /* mov.l r11,@-r15 */
L8c646592: *stop=0x8c646592u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c646594; /* mov.l r10,@-r15 */
L8c646594: *stop=0x8c646594u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c646596; /* mov.l r9,@-r15 */
L8c646596: *stop=0x8c646596u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c646598; /* mov.l r8,@-r15 */
L8c646598: *stop=0x8c646598u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64659a; /* sts.l PR,@-r15 */
L8c64659a: *stop=0x8c64659au;r[15]+=4294967288u;goto L8c64659c; /* add #-0x8,r15 */
L8c64659c: *stop=0x8c64659cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c646628u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64659e; /* mov.l 0x8c646628,r2 */
L8c64659e: *stop=0x8c64659eu;r[8]=4u;goto L8c6465a0; /* mov #0x4,r8 */
L8c6465a0: *stop=0x8c6465a0u;r[0]=sx(m,bus,&io_status,0x8c646616u,2);if(io_status)return io_status;goto L8c6465a2; /* mov.w 0x8c646616,r0 */
L8c6465a2: *stop=0x8c6465a2u;r[10]=r[8];goto L8c6465a4; /* mov r8,r10 */
L8c6465a4: *stop=0x8c6465a4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6465a6; /* mov.l @r2,r3 */
L8c6465a6: *stop=0x8c6465a6u;r[9]=1u;goto L8c6465a8; /* mov #0x1,r9 */
L8c6465a8: *stop=0x8c6465a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64662cu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6465aa; /* mov.l 0x8c64662c,r11 */
L8c6465aa: *stop=0x8c6465aau;r[10]+=124u;goto L8c6465ac; /* add #0x7c,r10 */
L8c6465ac: *stop=0x8c6465acu;if(!valid_access(r[0]+r[3],1u))return -3;r[1]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c6465ae; /* mov.b @(r0,r3),r1 */
L8c6465ae: *stop=0x8c6465aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c646630u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c6465b0; /* mov.l 0x8c646630,r12 */
L8c6465b0: *stop=0x8c6465b0u;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[1],1);if(io_status)return io_status;goto L8c6465b2; /* mov.b r1,@r15 */
L8c6465b2: *stop=0x8c6465b4u;r[14]=0u;goto L8c64663a; /* bra 0x8c64663a */
L8c6465b6: *stop=0x8c6465b6u;if(!valid_access(r[15],1u))return -3;r[3]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c6465b8; /* mov.b @r15,r3 */
L8c6465b8: *stop=0x8c6465b8u;r[2]=r[9];goto L8c6465ba; /* mov r9,r2 */
L8c6465ba: *stop=0x8c6465bau;{uint32_t shift=r[14];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c6465bc; /* shad r14,r2 */
L8c6465bc: *stop=0x8c6465bcu;r[3]=r[3]&255u;goto L8c6465be; /* extu.b r3,r3 */
L8c6465be: *stop=0x8c6465beu;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c6465c0; /* tst r3,r2 */
L8c6465c0: if((*sr&1u))goto L8c646638;goto L8c6465c2; /* bt 0x8c646638 */
L8c6465c2: *stop=0x8c6465c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646634u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6465c4; /* mov.l 0x8c646634,r4 */
L8c6465c4: *stop=0x8c6465c4u;r[4]+=r[14];goto L8c6465c6; /* add r14,r4 */
L8c6465c6: *stop=0x8c6465c6u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c6465c8; /* mov.l r4,@(0x4,r15) */
L8c6465c8: *stop=0x8c6465c8u;if(!valid_access(r[4],1u))return -3;r[4]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c6465ca; /* mov.b @r4,r4 */
L8c6465ca: target=0x8c646856u;*pr=0x8c6465ceu;*stop=0x8c6465ccu;r[4]=r[4]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6465ce; /* bsr 0x8c646856 */
L8c6465ce: *stop=0x8c6465ceu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6465d0; /* mov.l @(0x4,r15),r4 */
L8c6465d0: *stop=0x8c6465d0u;r[13]=r[0];goto L8c6465d2; /* mov r0,r13 */
L8c6465d2: *stop=0x8c6465d2u;if(!valid_access(r[4],1u))return -3;r[4]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c6465d4; /* mov.b @r4,r4 */
L8c6465d4: target=0x8c646868u;*pr=0x8c6465d8u;*stop=0x8c6465d6u;r[4]=r[4]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6465d8; /* bsr 0x8c646868 */
L8c6465d8: *stop=0x8c6465d8u;r[4]=r[0];goto L8c6465da; /* mov r0,r4 */
L8c6465da: *stop=0x8c6465dau;r[0]=r[14];goto L8c6465dc; /* mov r14,r0 */
L8c6465dc: *stop=0x8c6465dcu;if(!valid_access(r[0]+r[11],1u))return -3;r[3]=sx(m,bus,&io_status,r[11]+r[0],1);if(io_status)return io_status;goto L8c6465de; /* mov.b @(r0,r11),r3 */
L8c6465de: *stop=0x8c6465deu;if(!valid_access(r[0]+r[12],1u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[3],1);if(io_status)return io_status;goto L8c6465e0; /* mov.b r3,@(r0,r12) */
L8c6465e0: *stop=0x8c6465e0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6465e2; /* mov.l @r4,r2 */
L8c6465e2: *stop=0x8c6465e2u;*sr=(*sr&~1u)|!(r[10]&r[2]);goto L8c6465e4; /* tst r10,r2 */
L8c6465e4: if((*sr&1u))goto L8c646638;goto L8c6465e6; /* bt 0x8c646638 */
L8c6465e6: *stop=0x8c6465e6u;if(!valid_access(r[13]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x2u,1);if(io_status)return io_status;goto L8c6465e8; /* mov.b @(0x2,r13),r0 */
L8c6465e8: *stop=0x8c6465e8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6465ea; /* tst r0,r0 */
L8c6465ea: if(!(*sr&1u))goto L8c646638;goto L8c6465ec; /* bf 0x8c646638 */
L8c6465ec: *stop=0x8c6465ecu;if(!valid_access(r[13]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,1);if(io_status)return io_status;goto L8c6465ee; /* mov.b @(0x4,r13),r0 */
L8c6465ee: *stop=0x8c6465eeu;r[0]=r[0]&255u;goto L8c6465f0; /* extu.b r0,r0 */
L8c6465f0: *stop=0x8c6465f0u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c6465f2; /* cmp/eq #0x8,r0 */
L8c6465f2: if(!(*sr&1u))goto L8c646638;goto L8c6465f4; /* bf 0x8c646638 */
L8c6465f4: *stop=0x8c6465f4u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6465f6; /* mov.l @(0x8,r13),r4 */
L8c6465f6: *stop=0x8c6465f6u;if(!valid_access(r[4]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x8u,1);if(io_status)return io_status;goto L8c6465f8; /* mov.b @(0x8,r4),r0 */
L8c6465f8: *stop=0x8c6465f8u;r[0]&=0x4u;goto L8c6465fa; /* and #0x4,r0 */
L8c6465fa: *stop=0x8c6465fau;r[4]=r[0];goto L8c6465fc; /* mov r0,r4 */
L8c6465fc: *stop=0x8c6465fcu;r[0]=r[14];goto L8c6465fe; /* mov r14,r0 */
L8c6465fe: *stop=0x8c6465feu;if(!valid_access(r[0]+r[11],1u))return -3;wr(m,bus,&io_status,r[11]+r[0],r[4],1);if(io_status)return io_status;goto L8c646600; /* mov.b r4,@(r0,r11) */
L8c646600: *stop=0x8c646600u;if(!valid_access(r[0]+r[12],1u))return -3;r[5]=sx(m,bus,&io_status,r[12]+r[0],1);if(io_status)return io_status;goto L8c646602; /* mov.b @(r0,r12),r5 */
L8c646602: *stop=0x8c646602u;r[5]^=r[4];goto L8c646604; /* xor r4,r5 */
L8c646604: *stop=0x8c646604u;r[4]=r[4]&255u;goto L8c646606; /* extu.b r4,r4 */
L8c646606: *stop=0x8c646606u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c646608; /* tst r4,r4 */
L8c646608: if(!(*sr&1u))goto L8c646638;goto L8c64660a; /* bf 0x8c646638 */
L8c64660a: *stop=0x8c64660au;r[5]=r[5]&255u;goto L8c64660c; /* extu.b r5,r5 */
L8c64660c: *stop=0x8c64660cu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c64660e; /* tst r5,r5 */
L8c64660e: if((*sr&1u))goto L8c646638;goto L8c646610; /* bt 0x8c646638 */
L8c646610: *stop=0x8c646610u;r[0]=r[14];goto L8c646612; /* mov r14,r0 */
L8c646612: *stop=0x8c646614u;r[0]+=1u;goto L8c646640; /* bra 0x8c646640 */
L8c646638: *stop=0x8c646638u;r[14]+=1u;goto L8c64663a; /* add #0x1,r14 */
L8c64663a: *stop=0x8c64663au;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[8]);goto L8c64663c; /* cmp/ge r8,r14 */
L8c64663c: if(!(*sr&1u))goto L8c6465b6;goto L8c64663e; /* bf 0x8c6465b6 */
L8c64663e: *stop=0x8c64663eu;r[0]=0u;goto L8c646640; /* mov #0x0,r0 */
L8c646640: *stop=0x8c646640u;r[15]+=8u;goto L8c646642; /* add #0x8,r15 */
L8c646642: *stop=0x8c646642u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646644; /* lds.l @r15+,PR */
L8c646644: *stop=0x8c646644u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c646646; /* mov.l @r15+,r8 */
L8c646646: *stop=0x8c646646u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c646648; /* mov.l @r15+,r9 */
L8c646648: *stop=0x8c646648u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64664a; /* mov.l @r15+,r10 */
L8c64664a: *stop=0x8c64664au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64664c; /* mov.l @r15+,r11 */
L8c64664c: *stop=0x8c64664cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64664e; /* mov.l @r15+,r12 */
L8c64664e: *stop=0x8c64664eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c646650; /* mov.l @r15+,r13 */
L8c646650: *stop=0x8c646650u;if(*pr!=entry_pr)return -3;*stop=0x8c646652u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c65fd4e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65fd4e;
L8c65fd4e: *stop=0x8c65fd4eu;r[15]+=4294967292u;goto L8c65fd50; /* add #-0x4,r15 */
L8c65fd50: *stop=0x8c65fd50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65fe0cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65fd52; /* mov.l 0x8c65fe0c,r6 */
L8c65fd52: *stop=0x8c65fd52u;r[3]=r[4];goto L8c65fd54; /* mov r4,r3 */
L8c65fd54: *stop=0x8c65fd54u;r[3]<<=2;goto L8c65fd56; /* shll2 r3 */
L8c65fd56: *stop=0x8c65fd56u;r[2]=sx(m,bus,&io_status,0x8c65fe02u,2);if(io_status)return io_status;goto L8c65fd58; /* mov.w 0x8c65fe02,r2 */
L8c65fd58: *stop=0x8c65fd58u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65fd5a; /* mov.l @r6,r5 */
L8c65fd5a: *stop=0x8c65fd5au;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c65fd5c; /* shll r3 */
L8c65fd5c: *stop=0x8c65fd5cu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fd5e; /* mov.l @r6,r0 */
L8c65fd5e: *stop=0x8c65fd5eu;r[4]<<=2;goto L8c65fd60; /* shll2 r4 */
L8c65fd60: *stop=0x8c65fd60u;r[5]+=4u;goto L8c65fd62; /* add #0x4,r5 */
L8c65fd62: *stop=0x8c65fd62u;r[1]=sx(m,bus,&io_status,0x8c65fe04u,2);if(io_status)return io_status;goto L8c65fd64; /* mov.w 0x8c65fe04,r1 */
L8c65fd64: *stop=0x8c65fd64u;r[5]+=r[3];goto L8c65fd66; /* add r3,r5 */
L8c65fd66: *stop=0x8c65fd66u;r[3]=r[0];goto L8c65fd68; /* mov r0,r3 */
L8c65fd68: *stop=0x8c65fd68u;r[2]+=r[3];goto L8c65fd6a; /* add r3,r2 */
L8c65fd6a: *stop=0x8c65fd6au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65fd6c; /* mov.l @r2,r2 */
L8c65fd6c: *stop=0x8c65fd6cu;r[0]+=r[1];goto L8c65fd6e; /* add r1,r0 */
L8c65fd6e: *stop=0x8c65fd6eu;r[2]=0u-r[2];goto L8c65fd70; /* neg r2,r2 */
L8c65fd70: *stop=0x8c65fd70u;r[2]+=1u;goto L8c65fd72; /* add #0x1,r2 */
L8c65fd72: *stop=0x8c65fd72u;r[3]=r[2];goto L8c65fd74; /* mov r2,r3 */
L8c65fd74: *stop=0x8c65fd74u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c65fd76; /* shll r2 */
L8c65fd76: *stop=0x8c65fd76u;r[2]+=r[3];goto L8c65fd78; /* add r3,r2 */
L8c65fd78: *stop=0x8c65fd78u;r[2]<<=2;goto L8c65fd7a; /* shll2 r2 */
L8c65fd7a: *stop=0x8c65fd7au;r[2]<<=2;goto L8c65fd7c; /* shll2 r2 */
L8c65fd7c: *stop=0x8c65fd7cu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c65fd7e; /* shll r2 */
L8c65fd7e: *stop=0x8c65fd7eu;r[0]+=r[2];goto L8c65fd80; /* add r2,r0 */
L8c65fd80: *stop=0x8c65fd80u;r[2]=r[0];goto L8c65fd82; /* mov r0,r2 */
L8c65fd82: *stop=0x8c65fd82u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65fe10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fd84; /* mov.l 0x8c65fe10,r0 */
L8c65fd84: *stop=0x8c65fd84u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65fd86; /* mov.l @(r0,r4),r3 */
L8c65fd86: *stop=0x8c65fd86u;r[3]<<=2;goto L8c65fd88; /* shll2 r3 */
L8c65fd88: *stop=0x8c65fd88u;r[2]+=r[3];goto L8c65fd8a; /* add r3,r2 */
L8c65fd8a: *stop=0x8c65fd8au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65fd8c; /* mov.l @r2,r4 */
L8c65fd8c: *stop=0x8c65fd8cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65fd8e; /* mov.l @r4,r3 */
L8c65fd8e: *stop=0x8c65fd8eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65fd90; /* mov.l r3,@r15 */
L8c65fd90: *stop=0x8c65fd90u;r[0]=r[3];goto L8c65fd92; /* mov r3,r0 */
L8c65fd92: *stop=0x8c65fd92u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c65fd94; /* cmp/eq #-0x1,r0 */
L8c65fd94: take=(*sr&1u);*stop=0x8c65fd96u;r[7]=0u;if(take)goto L8c65fda2;goto L8c65fd98; /* bt/s 0x8c65fda2 */
L8c65fd98: *stop=0x8c65fd98u;r[1]=sx(m,bus,&io_status,0x8c65fe06u,2);if(io_status)return io_status;goto L8c65fd9a; /* mov.w 0x8c65fe06,r1 */
L8c65fd9a: *stop=0x8c65fd9au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c65fd9c; /* cmp/eq r1,r0 */
L8c65fd9c: if((*sr&1u))goto L8c65fda2;goto L8c65fd9e; /* bt 0x8c65fda2 */
L8c65fd9e: *stop=0x8c65fda0u;goto L8c65fdc0; /* bra 0x8c65fdc0 */
L8c65fda2: *stop=0x8c65fda2u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65fda4; /* mov.l @(0x4,r5),r3 */
L8c65fda4: *stop=0x8c65fda4u;r[0]=sx(m,bus,&io_status,0x8c65fe08u,2);if(io_status)return io_status;goto L8c65fda6; /* mov.w 0x8c65fe08,r0 */
L8c65fda6: *stop=0x8c65fda6u;r[3]+=1u;goto L8c65fda8; /* add #0x1,r3 */
L8c65fda8: *stop=0x8c65fda8u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[3],4);if(io_status)return io_status;goto L8c65fdaa; /* mov.l r3,@(0x4,r5) */
L8c65fdaa: *stop=0x8c65fdaau;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65fdac; /* mov.l @r6,r2 */
L8c65fdac: *stop=0x8c65fdacu;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65fdae; /* mov.l @(0x4,r5),r1 */
L8c65fdae: *stop=0x8c65fdaeu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65fdb0; /* mov.l @(r0,r2),r3 */
L8c65fdb0: *stop=0x8c65fdb0u;*sr=(*sr&~1u)|(r[1]>r[3]);goto L8c65fdb2; /* cmp/hi r3,r1 */
L8c65fdb2: if(!(*sr&1u))goto L8c65fdba;goto L8c65fdb4; /* bf 0x8c65fdba */
L8c65fdb4: *stop=0x8c65fdb4u;r[6]=r[7];goto L8c65fdb6; /* mov r7,r6 */
L8c65fdb6: *stop=0x8c65fdb8u;r[4]=4294967295u;goto L8c65fdda; /* bra 0x8c65fdda */
L8c65fdba: *stop=0x8c65fdbau;r[0]=0u;goto L8c65fdbc; /* mov #0x0,r0 */
L8c65fdbc: *stop=0x8c65fdbcu;if(*pr!=entry_pr)return -3;*stop=0x8c65fdbeu;r[15]+=4u;return 0; /* rts */
L8c65fdc0: *stop=0x8c65fdc0u;r[1]=sx(m,bus,&io_status,0x8c65fe0au,2);if(io_status)return io_status;goto L8c65fdc2; /* mov.w 0x8c65fe0a,r1 */
L8c65fdc2: *stop=0x8c65fdc2u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fdc4; /* mov.l @r6,r0 */
L8c65fdc4: *stop=0x8c65fdc4u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fdc6; /* mov.l @(r0,r1),r0 */
L8c65fdc6: *stop=0x8c65fdc6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65fdc8; /* cmp/eq #0x1,r0 */
L8c65fdc8: if(!(*sr&1u))goto L8c65fdd0;goto L8c65fdca; /* bf 0x8c65fdd0 */
L8c65fdca: *stop=0x8c65fdcau;r[4]=r[15];goto L8c65fdcc; /* mov r15,r4 */
L8c65fdcc: *stop=0x8c65fdceu;r[4]+=1u;goto L8c65fdd4; /* bra 0x8c65fdd4 */
L8c65fdd0: *stop=0x8c65fdd0u;r[4]=r[15];goto L8c65fdd2; /* mov r15,r4 */
L8c65fdd2: *stop=0x8c65fdd2u;r[4]+=2u;goto L8c65fdd4; /* add #0x2,r4 */
L8c65fdd4: *stop=0x8c65fdd4u;if(!valid_access(r[4],1u))return -3;r[6]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c65fdd6; /* mov.b @r4,r6 */
L8c65fdd6: *stop=0x8c65fdd6u;r[4]=r[7];goto L8c65fdd8; /* mov r7,r4 */
L8c65fdd8: *stop=0x8c65fdd8u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[7],4);if(io_status)return io_status;goto L8c65fdda; /* mov.l r7,@(0x4,r5) */
L8c65fdda: *stop=0x8c65fddau;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c65fddc; /* mov.b @r5,r0 */
L8c65fddc: *stop=0x8c65fddcu;if(!valid_access(r[5]+1u,1u))return -3;wr(m,bus,&io_status,r[5]+0x1u,r[0],1);if(io_status)return io_status;goto L8c65fdde; /* mov.b r0,@(0x1,r5) */
L8c65fdde: *stop=0x8c65fddeu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[6],1);if(io_status)return io_status;goto L8c65fde0; /* mov.b r6,@r5 */
L8c65fde0: *stop=0x8c65fde0u;if(!valid_access(r[5]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x1u,1);if(io_status)return io_status;goto L8c65fde2; /* mov.b @(0x1,r5),r0 */
L8c65fde2: *stop=0x8c65fde2u;if(!valid_access(r[5],1u))return -3;r[6]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c65fde4; /* mov.b @r5,r6 */
L8c65fde4: *stop=0x8c65fde4u;r[0]^=r[6];goto L8c65fde6; /* xor r6,r0 */
L8c65fde6: *stop=0x8c65fde6u;r[0]&=r[6];goto L8c65fde8; /* and r6,r0 */
L8c65fde8: *stop=0x8c65fde8u;if(!valid_access(r[5]+2u,1u))return -3;wr(m,bus,&io_status,r[5]+0x2u,r[0],1);if(io_status)return io_status;goto L8c65fdea; /* mov.b r0,@(0x2,r5) */
L8c65fdea: *stop=0x8c65fdeau;if(!valid_access(r[5],1u))return -3;r[6]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c65fdec; /* mov.b @r5,r6 */
L8c65fdec: *stop=0x8c65fdecu;r[0]=~r[6];goto L8c65fdee; /* not r6,r0 */
L8c65fdee: *stop=0x8c65fdeeu;r[3]=r[0];goto L8c65fdf0; /* mov r0,r3 */
L8c65fdf0: *stop=0x8c65fdf0u;if(!valid_access(r[5]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x1u,1);if(io_status)return io_status;goto L8c65fdf2; /* mov.b @(0x1,r5),r0 */
L8c65fdf2: *stop=0x8c65fdf2u;r[0]^=r[6];goto L8c65fdf4; /* xor r6,r0 */
L8c65fdf4: *stop=0x8c65fdf4u;r[0]&=r[3];goto L8c65fdf6; /* and r3,r0 */
L8c65fdf6: *stop=0x8c65fdf6u;if(!valid_access(r[5]+3u,1u))return -3;wr(m,bus,&io_status,r[5]+0x3u,r[0],1);if(io_status)return io_status;goto L8c65fdf8; /* mov.b r0,@(0x3,r5) */
L8c65fdf8: *stop=0x8c65fdf8u;r[0]=r[4];goto L8c65fdfa; /* mov r4,r0 */
L8c65fdfa: *stop=0x8c65fdfau;if(*pr!=entry_pr)return -3;*stop=0x8c65fdfcu;r[15]+=4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c65fe14(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65fe14;
L8c65fe14: *stop=0x8c65fe14u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65fe16; /* mov.l r14,@-r15 */
L8c65fe16: *stop=0x8c65fe16u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65fe18; /* mov.l r13,@-r15 */
L8c65fe18: *stop=0x8c65fe18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65fe1a; /* mov.l r12,@-r15 */
L8c65fe1a: *stop=0x8c65fe1au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65fe1c; /* mov.l r11,@-r15 */
L8c65fe1c: *stop=0x8c65fe1cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65fe1e; /* mov.l r10,@-r15 */
L8c65fe1e: *stop=0x8c65fe1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65fe20; /* sts.l PR,@-r15 */
L8c65fe20: *stop=0x8c65fe20u;r[15]+=4294967284u;goto L8c65fe22; /* add #-0xc,r15 */
L8c65fe22: *stop=0x8c65fe22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ff4cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c65fe24; /* mov.l 0x8c65ff4c,r13 */
L8c65fe24: *stop=0x8c65fe24u;r[2]=r[5];goto L8c65fe26; /* mov r5,r2 */
L8c65fe26: *stop=0x8c65fe26u;r[2]<<=2;goto L8c65fe28; /* shll2 r2 */
L8c65fe28: *stop=0x8c65fe28u;r[1]=sx(m,bus,&io_status,0x8c65ff46u,2);if(io_status)return io_status;goto L8c65fe2a; /* mov.w 0x8c65ff46,r1 */
L8c65fe2a: *stop=0x8c65fe2au;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c65fe2c; /* shll r2 */
L8c65fe2c: *stop=0x8c65fe2cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c65fe2e; /* mov.l r5,@r15 */
L8c65fe2e: *stop=0x8c65fe2eu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65fe30; /* mov.l @r13,r3 */
L8c65fe30: *stop=0x8c65fe30u;r[3]+=4u;goto L8c65fe32; /* add #0x4,r3 */
L8c65fe32: *stop=0x8c65fe32u;r[2]+=r[3];goto L8c65fe34; /* add r3,r2 */
L8c65fe34: *stop=0x8c65fe34u;r[3]=44u;goto L8c65fe36; /* mov #0x2c,r3 */
L8c65fe36: *stop=0x8c65fe36u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c65fe38; /* mov.l r2,@(0x4,r15) */
L8c65fe38: *stop=0x8c65fe38u;*macl=r[3]*r[4];goto L8c65fe3a; /* mul.l r3,r4 */
L8c65fe3a: *stop=0x8c65fe3au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[14]=loaded;}goto L8c65fe3c; /* mov.l @r13,r14 */
L8c65fe3c: *stop=0x8c65fe3cu;r[2]=sx(m,bus,&io_status,0x8c65ff44u,2);if(io_status)return io_status;goto L8c65fe3e; /* mov.w 0x8c65ff44,r2 */
L8c65fe3e: *stop=0x8c65fe3eu;r[14]+=40u;goto L8c65fe40; /* add #0x28,r14 */
L8c65fe40: *stop=0x8c65fe40u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[10]=loaded;}goto L8c65fe42; /* mov.l @r13,r10 */
L8c65fe42: *stop=0x8c65fe42u;r[3]=*macl;goto L8c65fe44; /* sts MACL,r3 */
L8c65fe44: *stop=0x8c65fe44u;r[10]+=r[2];goto L8c65fe46; /* add r2,r10 */
L8c65fe46: *stop=0x8c65fe46u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fe48; /* mov.l @r13,r0 */
L8c65fe48: *stop=0x8c65fe48u;r[14]+=r[3];goto L8c65fe4a; /* add r3,r14 */
L8c65fe4a: *stop=0x8c65fe4au;r[3]=120u;goto L8c65fe4c; /* mov #0x78,r3 */
L8c65fe4c: *stop=0x8c65fe4cu;*macl=r[3]*r[4];goto L8c65fe4e; /* mul.l r3,r4 */
L8c65fe4e: *stop=0x8c65fe4eu;r[3]=*macl;goto L8c65fe50; /* sts MACL,r3 */
L8c65fe50: *stop=0x8c65fe50u;r[10]+=r[3];goto L8c65fe52; /* add r3,r10 */
L8c65fe52: *stop=0x8c65fe52u;r[3]=r[0];goto L8c65fe54; /* mov r0,r3 */
L8c65fe54: *stop=0x8c65fe54u;r[1]+=r[3];goto L8c65fe56; /* add r3,r1 */
L8c65fe56: *stop=0x8c65fe56u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65fe58; /* mov.l @r1,r1 */
L8c65fe58: *stop=0x8c65fe58u;r[1]=0u-r[1];goto L8c65fe5a; /* neg r1,r1 */
L8c65fe5a: *stop=0x8c65fe5au;r[1]+=1u;goto L8c65fe5c; /* add #0x1,r1 */
L8c65fe5c: *stop=0x8c65fe5cu;r[3]=r[1];goto L8c65fe5e; /* mov r1,r3 */
L8c65fe5e: *stop=0x8c65fe5eu;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c65fe60; /* shll r1 */
L8c65fe60: *stop=0x8c65fe60u;r[1]+=r[3];goto L8c65fe62; /* add r3,r1 */
L8c65fe62: *stop=0x8c65fe62u;r[3]=sx(m,bus,&io_status,0x8c65ff48u,2);if(io_status)return io_status;goto L8c65fe64; /* mov.w 0x8c65ff48,r3 */
L8c65fe64: *stop=0x8c65fe64u;r[1]<<=2;goto L8c65fe66; /* shll2 r1 */
L8c65fe66: *stop=0x8c65fe66u;r[11]=r[4];goto L8c65fe68; /* mov r4,r11 */
L8c65fe68: *stop=0x8c65fe68u;r[1]<<=2;goto L8c65fe6a; /* shll2 r1 */
L8c65fe6a: *stop=0x8c65fe6au;r[11]<<=2;goto L8c65fe6c; /* shll2 r11 */
L8c65fe6c: *stop=0x8c65fe6cu;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c65fe6e; /* shll r1 */
L8c65fe6e: *stop=0x8c65fe6eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[11],4);if(io_status)return io_status;goto L8c65fe70; /* mov.l r11,@(0x8,r15) */
L8c65fe70: *stop=0x8c65fe70u;r[0]+=r[3];goto L8c65fe72; /* add r3,r0 */
L8c65fe72: *stop=0x8c65fe72u;r[0]+=r[1];goto L8c65fe74; /* add r1,r0 */
L8c65fe74: *stop=0x8c65fe74u;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[11]=loaded;}goto L8c65fe76; /* mov.l @(r0,r11),r11 */
L8c65fe76: *stop=0x8c65fe76u;if(!valid_access(r[14],1u))return -3;r[0]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c65fe78; /* mov.b @r14,r0 */
L8c65fe78: *stop=0x8c65fe78u;if(!valid_access(r[14]+1u,1u))return -3;wr(m,bus,&io_status,r[14]+0x1u,r[0],1);if(io_status)return io_status;goto L8c65fe7a; /* mov.b r0,@(0x1,r14) */
L8c65fe7a: *stop=0x8c65fe7au;if(!valid_access(r[14]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,1);if(io_status)return io_status;goto L8c65fe7c; /* mov.b @(0x4,r14),r0 */
L8c65fe7c: *stop=0x8c65fe7cu;if(!valid_access(r[14]+5u,1u))return -3;wr(m,bus,&io_status,r[14]+0x5u,r[0],1);if(io_status)return io_status;goto L8c65fe7e; /* mov.b r0,@(0x5,r14) */
L8c65fe7e: *stop=0x8c65fe7eu;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65fe80; /* mov.l @(0x1c,r14),r1 */
L8c65fe80: *stop=0x8c65fe80u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c65fe82; /* tst r1,r1 */
L8c65fe82: take=(*sr&1u);*stop=0x8c65fe84u;r[12]=0u;if(take)goto L8c65fe8c;goto L8c65fe86; /* bt/s 0x8c65fe8c */
L8c65fe86: *stop=0x8c65fe86u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65fe88; /* mov.l @(0x1c,r14),r1 */
L8c65fe88: *stop=0x8c65fe88u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[1],4);if(io_status)return io_status;goto L8c65fe8a; /* mov.l r1,@(0x20,r14) */
L8c65fe8a: *stop=0x8c65fe8au;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[12],4);if(io_status)return io_status;goto L8c65fe8c; /* mov.l r12,@(0x1c,r14) */
L8c65fe8c: *stop=0x8c65fe8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ff50u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fe8e; /* mov.l 0x8c65ff50,r0 */
L8c65fe8e: *stop=0x8c65fe8eu;r[2]=31u;goto L8c65fe90; /* mov #0x1f,r2 */
L8c65fe90: *stop=0x8c65fe90u;if(!valid_access(r[0]+r[4],1u))return -3;r[3]=sx(m,bus,&io_status,r[4]+r[0],1);if(io_status)return io_status;goto L8c65fe92; /* mov.b @(r0,r4),r3 */
L8c65fe92: *stop=0x8c65fe92u;r[3]&=r[2];goto L8c65fe94; /* and r2,r3 */
L8c65fe94: *stop=0x8c65fe94u;r[4]=r[3];goto L8c65fe96; /* mov r3,r4 */
L8c65fe96: *stop=0x8c65fe96u;r[1]=r[4]&255u;goto L8c65fe98; /* extu.b r4,r1 */
L8c65fe98: *stop=0x8c65fe98u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c65fe9a; /* tst r1,r1 */
L8c65fe9a: if((*sr&1u))goto L8c65feaa;goto L8c65fe9c; /* bt 0x8c65feaa */
L8c65fe9c: *stop=0x8c65fe9cu;r[6]=r[10];goto L8c65fe9e; /* mov r10,r6 */
L8c65fe9e: *stop=0x8c65fe9eu;r[7]=r[14];goto L8c65fea0; /* mov r14,r7 */
L8c65fea0: *stop=0x8c65fea0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65fea2; /* mov.l r11,@-r15 */
L8c65fea2: target=0x8c66005cu;*pr=0x8c65fea6u;*stop=0x8c65fea4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65fea6; /* bsr 0x8c66005c */
L8c65fea6: *stop=0x8c65fea8u;r[15]+=4u;goto L8c65feb4; /* bra 0x8c65feb4 */
L8c65feaa: *stop=0x8c65feaau;r[5]=r[10];goto L8c65feac; /* mov r10,r5 */
L8c65feac: *stop=0x8c65feacu;r[6]=r[14];goto L8c65feae; /* mov r14,r6 */
L8c65feae: *stop=0x8c65feaeu;r[7]=r[11];goto L8c65feb0; /* mov r11,r7 */
L8c65feb0: target=0x8c65ffb8u;*pr=0x8c65feb4u;*stop=0x8c65feb2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65feb4; /* bsr 0x8c65ffb8 */
L8c65feb4: *stop=0x8c65feb4u;r[5]=r[0];goto L8c65feb6; /* mov r0,r5 */
L8c65feb6: *stop=0x8c65feb6u;if(!valid_access(r[14]+7u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x7u,1);if(io_status)return io_status;goto L8c65feb8; /* mov.b @(0x7,r14),r0 */
L8c65feb8: *stop=0x8c65feb8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c65feba; /* tst r0,r0 */
L8c65feba: if((*sr&1u))goto L8c65fee0;goto L8c65febc; /* bt 0x8c65fee0 */
L8c65febc: *stop=0x8c65febcu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65febe; /* mov.l @r13,r2 */
L8c65febe: *stop=0x8c65febeu;r[0]=sx(m,bus,&io_status,0x8c65ff46u,2);if(io_status)return io_status;goto L8c65fec0; /* mov.w 0x8c65ff46,r0 */
L8c65fec0: *stop=0x8c65fec0u;r[3]=r[2];goto L8c65fec2; /* mov r2,r3 */
L8c65fec2: *stop=0x8c65fec2u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65fec4; /* mov.l @(r0,r3),r1 */
L8c65fec4: *stop=0x8c65fec4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fec6; /* mov.l @(0x8,r15),r0 */
L8c65fec6: *stop=0x8c65fec6u;r[1]=0u-r[1];goto L8c65fec8; /* neg r1,r1 */
L8c65fec8: *stop=0x8c65fec8u;r[1]+=1u;goto L8c65feca; /* add #0x1,r1 */
L8c65feca: *stop=0x8c65fecau;r[3]=r[1];goto L8c65fecc; /* mov r1,r3 */
L8c65fecc: *stop=0x8c65feccu;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c65fece; /* shll r1 */
L8c65fece: *stop=0x8c65feceu;r[1]+=r[3];goto L8c65fed0; /* add r3,r1 */
L8c65fed0: *stop=0x8c65fed0u;r[3]=sx(m,bus,&io_status,0x8c65ff48u,2);if(io_status)return io_status;goto L8c65fed2; /* mov.w 0x8c65ff48,r3 */
L8c65fed2: *stop=0x8c65fed2u;r[1]<<=2;goto L8c65fed4; /* shll2 r1 */
L8c65fed4: *stop=0x8c65fed4u;r[1]<<=2;goto L8c65fed6; /* shll2 r1 */
L8c65fed6: *stop=0x8c65fed6u;r[2]+=r[3];goto L8c65fed8; /* add r3,r2 */
L8c65fed8: *stop=0x8c65fed8u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c65feda; /* shll r1 */
L8c65feda: *stop=0x8c65fedau;r[1]+=r[2];goto L8c65fedc; /* add r2,r1 */
L8c65fedc: *stop=0x8c65fedcu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65fede; /* mov.l @(r0,r1),r2 */
L8c65fede: *stop=0x8c65fedeu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c65fee0; /* mov.l r2,@(0x10,r14) */
L8c65fee0: *stop=0x8c65fee0u;if(!valid_access(r[14]+13u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0xdu,1);if(io_status)return io_status;goto L8c65fee2; /* mov.b @(0xd,r14),r0 */
L8c65fee2: *stop=0x8c65fee2u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c65fee4; /* tst r0,r0 */
L8c65fee4: if((*sr&1u))goto L8c65feec;goto L8c65fee6; /* bt 0x8c65feec */
L8c65fee6: *stop=0x8c65fee6u;r[0]=r[12];goto L8c65fee8; /* mov r12,r0 */
L8c65fee8: *stop=0x8c65fee8u;if(!valid_access(r[14]+12u,1u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],1);if(io_status)return io_status;goto L8c65feea; /* mov.b r0,@(0xc,r14) */
L8c65feea: *stop=0x8c65feeau;if(!valid_access(r[14]+13u,1u))return -3;wr(m,bus,&io_status,r[14]+0xdu,r[0],1);if(io_status)return io_status;goto L8c65feec; /* mov.b r0,@(0xd,r14) */
L8c65feec: *stop=0x8c65feecu;if(!valid_access(r[14]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,1);if(io_status)return io_status;goto L8c65feee; /* mov.b @(0x2,r14),r0 */
L8c65feee: *stop=0x8c65feeeu;r[3]=r[0];goto L8c65fef0; /* mov r0,r3 */
L8c65fef0: *stop=0x8c65fef0u;if(!valid_access(r[14]+12u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0xcu,1);if(io_status)return io_status;goto L8c65fef2; /* mov.b @(0xc,r14),r0 */
L8c65fef2: *stop=0x8c65fef2u;r[0]|=r[3];goto L8c65fef4; /* or r3,r0 */
L8c65fef4: *stop=0x8c65fef4u;if(!valid_access(r[14]+12u,1u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],1);if(io_status)return io_status;goto L8c65fef6; /* mov.b r0,@(0xc,r14) */
L8c65fef6: *stop=0x8c65fef6u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c65fef8; /* mov.b @(0x3,r14),r0 */
L8c65fef8: *stop=0x8c65fef8u;r[3]=r[0];goto L8c65fefa; /* mov r0,r3 */
L8c65fefa: *stop=0x8c65fefau;if(!valid_access(r[14]+12u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0xcu,1);if(io_status)return io_status;goto L8c65fefc; /* mov.b @(0xc,r14),r0 */
L8c65fefc: *stop=0x8c65fefcu;r[0]|=r[3];goto L8c65fefe; /* or r3,r0 */
L8c65fefe: *stop=0x8c65fefeu;if(!valid_access(r[14]+12u,1u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],1);if(io_status)return io_status;goto L8c65ff00; /* mov.b r0,@(0xc,r14) */
L8c65ff00: *stop=0x8c65ff00u;r[0]=r[5];goto L8c65ff02; /* mov r5,r0 */
L8c65ff02: *stop=0x8c65ff02u;r[15]+=12u;goto L8c65ff04; /* add #0xc,r15 */
L8c65ff04: *stop=0x8c65ff04u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65ff06; /* lds.l @r15+,PR */
L8c65ff06: *stop=0x8c65ff06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65ff08; /* mov.l @r15+,r10 */
L8c65ff08: *stop=0x8c65ff08u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65ff0a; /* mov.l @r15+,r11 */
L8c65ff0a: *stop=0x8c65ff0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65ff0c; /* mov.l @r15+,r12 */
L8c65ff0c: *stop=0x8c65ff0cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65ff0e; /* mov.l @r15+,r13 */
L8c65ff0e: *stop=0x8c65ff0eu;if(*pr!=entry_pr)return -3;*stop=0x8c65ff10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c65ff12(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65ff12;
L8c65ff12: *stop=0x8c65ff12u;if(!valid_access(r[4]+7u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x7u,1);if(io_status)return io_status;goto L8c65ff14; /* mov.b @(0x7,r4),r0 */
L8c65ff14: *stop=0x8c65ff14u;r[6]=1u;goto L8c65ff16; /* mov #0x1,r6 */
L8c65ff16: *stop=0x8c65ff16u;r[0]=r[0]&255u;goto L8c65ff18; /* extu.b r0,r0 */
L8c65ff18: *stop=0x8c65ff18u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65ff1a; /* cmp/eq #0x1,r0 */
L8c65ff1a: take=!(*sr&1u);*stop=0x8c65ff1cu;r[5]=0u;if(take)goto L8c65ff2e;goto L8c65ff1e; /* bf/s 0x8c65ff2e */
L8c65ff1e: *stop=0x8c65ff1eu;if(!valid_access(r[4]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x8u,1);if(io_status)return io_status;goto L8c65ff20; /* mov.b @(0x8,r4),r0 */
L8c65ff20: *stop=0x8c65ff20u;r[0]=r[0]&255u;goto L8c65ff22; /* extu.b r0,r0 */
L8c65ff22: *stop=0x8c65ff22u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c65ff24; /* cmp/eq #0x5,r0 */
L8c65ff24: if(!(*sr&1u))goto L8c65ff2a;goto L8c65ff26; /* bf 0x8c65ff2a */
L8c65ff26: *stop=0x8c65ff28u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[6],1);if(io_status)return io_status;goto L8c65ff56; /* bra 0x8c65ff56 */
L8c65ff2a: *stop=0x8c65ff2cu;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[5],1);if(io_status)return io_status;goto L8c65ff56; /* bra 0x8c65ff56 */
L8c65ff2e: *stop=0x8c65ff2eu;if(!valid_access(r[4]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x8u,1);if(io_status)return io_status;goto L8c65ff30; /* mov.b @(0x8,r4),r0 */
L8c65ff30: *stop=0x8c65ff30u;r[1]=sx(m,bus,&io_status,0x8c65ff4au,2);if(io_status)return io_status;goto L8c65ff32; /* mov.w 0x8c65ff4a,r1 */
L8c65ff32: *stop=0x8c65ff32u;r[0]=r[0]&255u;goto L8c65ff34; /* extu.b r0,r0 */
L8c65ff34: *stop=0x8c65ff34u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c65ff36; /* cmp/eq r1,r0 */
L8c65ff36: if((*sr&1u))goto L8c65ff40;goto L8c65ff38; /* bt 0x8c65ff40 */
L8c65ff38: *stop=0x8c65ff38u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c65ff3a; /* cmp/eq #0x0,r0 */
L8c65ff3a: if((*sr&1u))goto L8c65ff40;goto L8c65ff3c; /* bt 0x8c65ff40 */
L8c65ff3c: *stop=0x8c65ff3eu;goto L8c65ff54; /* bra 0x8c65ff54 */
L8c65ff40: *stop=0x8c65ff42u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[5],1);if(io_status)return io_status;goto L8c65ff56; /* bra 0x8c65ff56 */
L8c65ff54: *stop=0x8c65ff54u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[6],1);if(io_status)return io_status;goto L8c65ff56; /* mov.b r6,@r4 */
L8c65ff56: *stop=0x8c65ff56u;if(*pr!=entry_pr)return -3;*stop=0x8c65ff58u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c65ff5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65ff5a;
L8c65ff5a: *stop=0x8c65ff5au;r[15]+=4294967292u;goto L8c65ff5c; /* add #-0x4,r15 */
L8c65ff5c: *stop=0x8c65ff5cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c660054u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ff5e; /* mov.l 0x8c660054,r2 */
L8c65ff5e: *stop=0x8c65ff5eu;r[1]=sx(m,bus,&io_status,0x8c66004cu,2);if(io_status)return io_status;goto L8c65ff60; /* mov.w 0x8c66004c,r1 */
L8c65ff60: *stop=0x8c65ff60u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}r[5]+=4;goto L8c65ff62; /* mov.l @r5+,r3 */
L8c65ff62: *stop=0x8c65ff62u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65ff64; /* mov.l r3,@r15 */
L8c65ff64: *stop=0x8c65ff64u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65ff66; /* mov.l @r2,r0 */
L8c65ff66: *stop=0x8c65ff66u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65ff68; /* mov.l @(r0,r1),r0 */
L8c65ff68: *stop=0x8c65ff68u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65ff6a; /* cmp/eq #0x1,r0 */
L8c65ff6a: if(!(*sr&1u))goto L8c65ff80;goto L8c65ff6c; /* bf 0x8c65ff80 */
L8c65ff6c: *stop=0x8c65ff6cu;r[6]=r[15];goto L8c65ff6e; /* mov r15,r6 */
L8c65ff6e: *stop=0x8c65ff6eu;if(!valid_access(r[6],1u))return -3;r[0]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[6]+=1;goto L8c65ff70; /* mov.b @r6+,r0 */
L8c65ff70: *stop=0x8c65ff70u;if(!valid_access(r[4]+11u,1u))return -3;wr(m,bus,&io_status,r[4]+0xbu,r[0],1);if(io_status)return io_status;goto L8c65ff72; /* mov.b r0,@(0xb,r4) */
L8c65ff72: *stop=0x8c65ff72u;if(!valid_access(r[6],1u))return -3;r[0]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[6]+=1;goto L8c65ff74; /* mov.b @r6+,r0 */
L8c65ff74: *stop=0x8c65ff74u;if(!valid_access(r[4]+10u,1u))return -3;wr(m,bus,&io_status,r[4]+0xau,r[0],1);if(io_status)return io_status;goto L8c65ff76; /* mov.b r0,@(0xa,r4) */
L8c65ff76: *stop=0x8c65ff76u;if(!valid_access(r[6],1u))return -3;r[0]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[6]+=1;goto L8c65ff78; /* mov.b @r6+,r0 */
L8c65ff78: *stop=0x8c65ff78u;if(!valid_access(r[4]+9u,1u))return -3;wr(m,bus,&io_status,r[4]+0x9u,r[0],1);if(io_status)return io_status;goto L8c65ff7a; /* mov.b r0,@(0x9,r4) */
L8c65ff7a: *stop=0x8c65ff7au;if(!valid_access(r[6],1u))return -3;r[0]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;goto L8c65ff7c; /* mov.b @r6,r0 */
L8c65ff7c: *stop=0x8c65ff7eu;if(!valid_access(r[4]+8u,1u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[0],1);if(io_status)return io_status;goto L8c65ff92; /* bra 0x8c65ff92 */
L8c65ff80: *stop=0x8c65ff80u;r[6]=r[15];goto L8c65ff82; /* mov r15,r6 */
L8c65ff82: *stop=0x8c65ff82u;if(!valid_access(r[6],1u))return -3;r[0]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[6]+=1;goto L8c65ff84; /* mov.b @r6+,r0 */
L8c65ff84: *stop=0x8c65ff84u;if(!valid_access(r[4]+8u,1u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[0],1);if(io_status)return io_status;goto L8c65ff86; /* mov.b r0,@(0x8,r4) */
L8c65ff86: *stop=0x8c65ff86u;if(!valid_access(r[6],1u))return -3;r[0]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[6]+=1;goto L8c65ff88; /* mov.b @r6+,r0 */
L8c65ff88: *stop=0x8c65ff88u;if(!valid_access(r[4]+9u,1u))return -3;wr(m,bus,&io_status,r[4]+0x9u,r[0],1);if(io_status)return io_status;goto L8c65ff8a; /* mov.b r0,@(0x9,r4) */
L8c65ff8a: *stop=0x8c65ff8au;if(!valid_access(r[6],1u))return -3;r[0]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[6]+=1;goto L8c65ff8c; /* mov.b @r6+,r0 */
L8c65ff8c: *stop=0x8c65ff8cu;if(!valid_access(r[4]+10u,1u))return -3;wr(m,bus,&io_status,r[4]+0xau,r[0],1);if(io_status)return io_status;goto L8c65ff8e; /* mov.b r0,@(0xa,r4) */
L8c65ff8e: *stop=0x8c65ff8eu;if(!valid_access(r[6],1u))return -3;r[0]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;goto L8c65ff90; /* mov.b @r6,r0 */
L8c65ff90: *stop=0x8c65ff90u;if(!valid_access(r[4]+11u,1u))return -3;wr(m,bus,&io_status,r[4]+0xbu,r[0],1);if(io_status)return io_status;goto L8c65ff92; /* mov.b r0,@(0xb,r4) */
L8c65ff92: *stop=0x8c65ff92u;if(!valid_access(r[4]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x8u,1);if(io_status)return io_status;goto L8c65ff94; /* mov.b @(0x8,r4),r0 */
L8c65ff94: *stop=0x8c65ff94u;r[3]=sx(m,bus,&io_status,0x8c66004eu,2);if(io_status)return io_status;goto L8c65ff96; /* mov.w 0x8c66004e,r3 */
L8c65ff96: *stop=0x8c65ff96u;r[0]=r[0]&255u;goto L8c65ff98; /* extu.b r0,r0 */
L8c65ff98: *stop=0x8c65ff98u;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c65ff9a; /* cmp/eq r3,r0 */
L8c65ff9a: if(!(*sr&1u))goto L8c65ffa2;goto L8c65ff9c; /* bf 0x8c65ffa2 */
L8c65ff9c: *stop=0x8c65ff9cu;r[1]=4294967295u;goto L8c65ff9e; /* mov #-0x1,r1 */
L8c65ff9e: *stop=0x8c65ffa0u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[1],4);if(io_status)return io_status;goto L8c65ffb2; /* bra 0x8c65ffb2 */
L8c65ffa2: *stop=0x8c65ffa2u;r[2]=sx(m,bus,&io_status,0x8c660050u,2);if(io_status)return io_status;goto L8c65ffa4; /* mov.w 0x8c660050,r2 */
L8c65ffa4: *stop=0x8c65ffa4u;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[2]);goto L8c65ffa6; /* cmp/ge r2,r0 */
L8c65ffa6: if((*sr&1u))goto L8c65ffae;goto L8c65ffa8; /* bt 0x8c65ffae */
L8c65ffa8: *stop=0x8c65ffa8u;r[0]=0u;goto L8c65ffaa; /* mov #0x0,r0 */
L8c65ffaa: *stop=0x8c65ffacu;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[0],4);if(io_status)return io_status;goto L8c65ffb2; /* bra 0x8c65ffb2 */
L8c65ffae: *stop=0x8c65ffaeu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ffb0; /* mov.l @r5,r1 */
L8c65ffb0: *stop=0x8c65ffb0u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[1],4);if(io_status)return io_status;goto L8c65ffb2; /* mov.l r1,@(0x14,r4) */
L8c65ffb2: *stop=0x8c65ffb2u;r[0]=0u;goto L8c65ffb4; /* mov #0x0,r0 */
L8c65ffb4: *stop=0x8c65ffb4u;if(*pr!=entry_pr)return -3;*stop=0x8c65ffb6u;r[15]+=4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c65ffb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65ffb8;
L8c65ffb8: *stop=0x8c65ffb8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65ffba; /* mov.l r14,@-r15 */
L8c65ffba: *stop=0x8c65ffbau;r[14]=r[6];goto L8c65ffbc; /* mov r6,r14 */
L8c65ffbc: *stop=0x8c65ffbcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65ffbe; /* mov.l r13,@-r15 */
L8c65ffbe: *stop=0x8c65ffbeu;r[13]=r[7];goto L8c65ffc0; /* mov r7,r13 */
L8c65ffc0: *stop=0x8c65ffc0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65ffc2; /* mov.l r12,@-r15 */
L8c65ffc2: *stop=0x8c65ffc2u;r[12]=r[5];goto L8c65ffc4; /* mov r5,r12 */
L8c65ffc4: *stop=0x8c65ffc4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65ffc6; /* sts.l PR,@-r15 */
L8c65ffc6: *stop=0x8c65ffc6u;r[5]=r[7];goto L8c65ffc8; /* mov r7,r5 */
L8c65ffc8: target=0x8c65ff5au;*pr=0x8c65ffccu;*stop=0x8c65ffcau;r[4]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ffcc; /* bsr 0x8c65ff5a */
L8c65ffcc: *stop=0x8c65ffccu;r[5]=r[13];goto L8c65ffce; /* mov r13,r5 */
L8c65ffce: target=0x8c65ff12u;*pr=0x8c65ffd2u;*stop=0x8c65ffd0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ffd2; /* bsr 0x8c65ff12 */
L8c65ffd2: *stop=0x8c65ffd2u;if(!valid_access(r[14]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x1u,1);if(io_status)return io_status;goto L8c65ffd4; /* mov.b @(0x1,r14),r0 */
L8c65ffd4: *stop=0x8c65ffd4u;if(!valid_access(r[14],1u))return -3;r[4]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c65ffd6; /* mov.b @r14,r4 */
L8c65ffd6: *stop=0x8c65ffd6u;r[1]=sx(m,bus,&io_status,0x8c66004cu,2);if(io_status)return io_status;goto L8c65ffd8; /* mov.w 0x8c66004c,r1 */
L8c65ffd8: *stop=0x8c65ffd8u;r[0]^=r[4];goto L8c65ffda; /* xor r4,r0 */
L8c65ffda: *stop=0x8c65ffdau;r[0]&=r[4];goto L8c65ffdc; /* and r4,r0 */
L8c65ffdc: *stop=0x8c65ffdcu;if(!valid_access(r[14]+2u,1u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],1);if(io_status)return io_status;goto L8c65ffde; /* mov.b r0,@(0x2,r14) */
L8c65ffde: *stop=0x8c65ffdeu;if(!valid_access(r[14],1u))return -3;r[4]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c65ffe0; /* mov.b @r14,r4 */
L8c65ffe0: *stop=0x8c65ffe0u;r[0]=~r[4];goto L8c65ffe2; /* not r4,r0 */
L8c65ffe2: *stop=0x8c65ffe2u;r[3]=r[0];goto L8c65ffe4; /* mov r0,r3 */
L8c65ffe4: *stop=0x8c65ffe4u;if(!valid_access(r[14]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x1u,1);if(io_status)return io_status;goto L8c65ffe6; /* mov.b @(0x1,r14),r0 */
L8c65ffe6: *stop=0x8c65ffe6u;r[0]^=r[4];goto L8c65ffe8; /* xor r4,r0 */
L8c65ffe8: *stop=0x8c65ffe8u;r[0]&=r[3];goto L8c65ffea; /* and r3,r0 */
L8c65ffea: *stop=0x8c65ffeau;{uint32_t loaded=rd(m,bus,&io_status,0x8c660054u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ffec; /* mov.l 0x8c660054,r3 */
L8c65ffec: *stop=0x8c65ffecu;if(!valid_access(r[14]+3u,1u))return -3;wr(m,bus,&io_status,r[14]+0x3u,r[0],1);if(io_status)return io_status;goto L8c65ffee; /* mov.b r0,@(0x3,r14) */
L8c65ffee: *stop=0x8c65ffeeu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fff0; /* mov.l @r3,r0 */
L8c65fff0: *stop=0x8c65fff0u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65fff2; /* mov.l @(r0,r1),r0 */
L8c65fff2: *stop=0x8c65fff2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65fff4; /* cmp/eq #0x1,r0 */
L8c65fff4: if(!(*sr&1u))goto L8c660010;goto L8c65fff6; /* bf 0x8c660010 */
L8c65fff6: *stop=0x8c65fff6u;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c65fff8; /* mov.b @(0x8,r14),r0 */
L8c65fff8: *stop=0x8c65fff8u;r[2]=sx(m,bus,&io_status,0x8c66004eu,2);if(io_status)return io_status;goto L8c65fffa; /* mov.w 0x8c66004e,r2 */
L8c65fffa: *stop=0x8c65fffau;r[4]=r[0]&255u;goto L8c65fffc; /* extu.b r0,r4 */
L8c65fffc: *stop=0x8c65fffcu;*sr=(*sr&~1u)|(r[2]==r[4]);goto L8c65fffe; /* cmp/eq r2,r4 */
L8c65fffe: if((*sr&1u))goto L8c660010;goto L8c660000; /* bt 0x8c660010 */
L8c660000: *stop=0x8c660000u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c660002; /* tst r4,r4 */
L8c660002: if((*sr&1u))goto L8c660010;goto L8c660004; /* bt 0x8c660010 */
L8c660004: *stop=0x8c660004u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[5]=loaded;}goto L8c660006; /* mov.l @r13,r5 */
L8c660006: *stop=0x8c660006u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660058u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660008; /* mov.l 0x8c660058,r3 */
L8c660008: *stop=0x8c660008u;r[5]=r[5]&255u;goto L8c66000a; /* extu.b r5,r5 */
L8c66000a: *stop=0x8c66000au;r[5]+=1u;goto L8c66000c; /* add #0x1,r5 */
L8c66000c: target=r[3];*pr=0x8c660010u;*stop=0x8c66000eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660010; /* jsr @r3 */
L8c660010: *stop=0x8c660010u;if(!valid_access(r[14],1u))return -3;r[2]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c660012; /* mov.b @r14,r2 */
L8c660012: *stop=0x8c660012u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c660014; /* tst r2,r2 */
L8c660014: take=(*sr&1u);*stop=0x8c660016u;r[4]=1u;if(take)goto L8c66001c;goto L8c660018; /* bt/s 0x8c66001c */
L8c660018: *stop=0x8c66001au;r[0]=9u;goto L8c66001e; /* bra 0x8c66001e */
L8c66001c: *stop=0x8c66001cu;r[0]=r[4];goto L8c66001e; /* mov r4,r0 */
L8c66001e: *stop=0x8c66001eu;if(!valid_access(r[14]+4u,1u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],1);if(io_status)return io_status;goto L8c660020; /* mov.b r0,@(0x4,r14) */
L8c660020: *stop=0x8c660020u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c660022; /* mov.b @(0x3,r14),r0 */
L8c660022: *stop=0x8c660022u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660024; /* tst r0,r0 */
L8c660024: *stop=0x8c660024u;if(!valid_access(r[14]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,1);if(io_status)return io_status;goto L8c660026; /* mov.b @(0x2,r14),r0 */
L8c660026: *stop=0x8c660026u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660028; /* tst r0,r0 */
L8c660028: if((*sr&1u))goto L8c660036;goto L8c66002a; /* bt 0x8c660036 */
L8c66002a: *stop=0x8c66002au;r[0]=16u;goto L8c66002c; /* mov #0x10,r0 */
L8c66002c: *stop=0x8c66002cu;r[5]=r[14];goto L8c66002e; /* mov r14,r5 */
L8c66002e: *stop=0x8c66002eu;r[6]=r[13];goto L8c660030; /* mov r13,r6 */
L8c660030: *stop=0x8c660030u;if(!valid_access(r[0]+r[12],1u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[4],1);if(io_status)return io_status;goto L8c660032; /* mov.b r4,@(r0,r12) */
L8c660032: target=0x8c66018eu;*pr=0x8c660036u;*stop=0x8c660034u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660036; /* bsr 0x8c66018e */
L8c660036: *stop=0x8c660036u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c660038; /* mov.b @(0x3,r14),r0 */
L8c660038: *stop=0x8c660038u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c66003a; /* tst r0,r0 */
L8c66003a: if((*sr&1u))goto L8c660040;goto L8c66003c; /* bt 0x8c660040 */
L8c66003c: target=0x8c6602d2u;*pr=0x8c660040u;*stop=0x8c66003eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660040; /* bsr 0x8c6602d2 */
L8c660040: *stop=0x8c660040u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660042; /* lds.l @r15+,PR */
L8c660042: *stop=0x8c660042u;r[0]=0u;goto L8c660044; /* mov #0x0,r0 */
L8c660044: *stop=0x8c660044u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c660046; /* mov.l @r15+,r12 */
L8c660046: *stop=0x8c660046u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c660048; /* mov.l @r15+,r13 */
L8c660048: *stop=0x8c660048u;if(*pr!=entry_pr)return -3;*stop=0x8c66004au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c66005c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c66005c;
L8c66005c: *stop=0x8c66005cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c66005e; /* mov.l r14,@-r15 */
L8c66005e: *stop=0x8c66005eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c660060; /* mov.l r13,@-r15 */
L8c660060: *stop=0x8c660060u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c660062; /* mov.l r12,@-r15 */
L8c660062: *stop=0x8c660062u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c660064; /* mov.l r11,@-r15 */
L8c660064: *stop=0x8c660064u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c660066; /* mov.l r10,@-r15 */
L8c660066: *stop=0x8c660066u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c660068; /* sts.l PR,@-r15 */
L8c660068: *stop=0x8c660068u;r[15]+=4294967292u;goto L8c66006a; /* add #-0x4,r15 */
L8c66006a: *stop=0x8c66006au;r[10]=r[5];goto L8c66006c; /* mov r5,r10 */
L8c66006c: *stop=0x8c66006cu;r[14]=r[7];goto L8c66006e; /* mov r7,r14 */
L8c66006e: *stop=0x8c66006eu;r[13]=0u;goto L8c660070; /* mov #0x0,r13 */
L8c660070: *stop=0x8c660070u;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[4],1);if(io_status)return io_status;goto L8c660072; /* mov.b r4,@r15 */
L8c660072: *stop=0x8c660072u;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c660074; /* mov.l @(0x1c,r15),r12 */
L8c660074: *stop=0x8c660074u;r[11]=r[6];goto L8c660076; /* mov r6,r11 */
L8c660076: *stop=0x8c660076u;r[5]=r[12];goto L8c660078; /* mov r12,r5 */
L8c660078: target=0x8c65ff5au;*pr=0x8c66007cu;*stop=0x8c66007au;r[4]=r[7];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66007c; /* bsr 0x8c65ff5a */
L8c66007c: *stop=0x8c66007cu;r[0]=r[13];goto L8c66007e; /* mov r13,r0 */
L8c66007e: *stop=0x8c66007eu;if(!valid_access(r[14]+2u,1u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],1);if(io_status)return io_status;goto L8c660080; /* mov.b r0,@(0x2,r14) */
L8c660080: *stop=0x8c660080u;if(!valid_access(r[14]+3u,1u))return -3;wr(m,bus,&io_status,r[14]+0x3u,r[0],1);if(io_status)return io_status;goto L8c660082; /* mov.b r0,@(0x3,r14) */
L8c660082: *stop=0x8c660082u;if(!valid_access(r[10]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[10]+0x2u,1);if(io_status)return io_status;goto L8c660084; /* mov.b @(0x2,r10),r0 */
L8c660084: *stop=0x8c660084u;if(!valid_access(r[15],1u))return -3;r[5]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c660086; /* mov.b @r15,r5 */
L8c660086: *stop=0x8c660086u;r[0]=r[0]&255u;goto L8c660088; /* extu.b r0,r0 */
L8c660088: *stop=0x8c660088u;r[5]=r[5]&255u;goto L8c66008a; /* extu.b r5,r5 */
L8c66008a: *stop=0x8c66008au;*sr=(*sr&~1u)|!(r[5]&r[0]);goto L8c66008c; /* tst r5,r0 */
L8c66008c: take=(*sr&1u);*stop=0x8c66008eu;r[4]=1u;if(take)goto L8c66009c;goto L8c660090; /* bt/s 0x8c66009c */
L8c660090: *stop=0x8c660090u;r[0]=r[4];goto L8c660092; /* mov r4,r0 */
L8c660092: *stop=0x8c660092u;if(!valid_access(r[14]+4u,1u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],1);if(io_status)return io_status;goto L8c660094; /* mov.b r0,@(0x4,r14) */
L8c660094: *stop=0x8c660094u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[13],1);if(io_status)return io_status;goto L8c660096; /* mov.b r13,@r14 */
L8c660096: *stop=0x8c660096u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660098; /* mov.l @(0x24,r14),r3 */
L8c660098: *stop=0x8c660098u;r[3]+=1u;goto L8c66009a; /* add #0x1,r3 */
L8c66009a: *stop=0x8c66009au;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[3],4);if(io_status)return io_status;goto L8c66009c; /* mov.l r3,@(0x24,r14) */
L8c66009c: *stop=0x8c66009cu;if(!valid_access(r[10]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[10]+0x3u,1);if(io_status)return io_status;goto L8c66009e; /* mov.b @(0x3,r10),r0 */
L8c66009e: *stop=0x8c66009eu;r[0]=r[0]&255u;goto L8c6600a0; /* extu.b r0,r0 */
L8c6600a0: *stop=0x8c6600a0u;*sr=(*sr&~1u)|!(r[5]&r[0]);goto L8c6600a2; /* tst r5,r0 */
L8c6600a2: if((*sr&1u))goto L8c6600b0;goto L8c6600a4; /* bt 0x8c6600b0 */
L8c6600a4: *stop=0x8c6600a4u;r[0]=4u;goto L8c6600a6; /* mov #0x4,r0 */
L8c6600a6: *stop=0x8c6600a6u;if(!valid_access(r[14]+4u,1u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],1);if(io_status)return io_status;goto L8c6600a8; /* mov.b r0,@(0x4,r14) */
L8c6600a8: *stop=0x8c6600a8u;r[0]=r[4];goto L8c6600aa; /* mov r4,r0 */
L8c6600aa: *stop=0x8c6600aau;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[13],1);if(io_status)return io_status;goto L8c6600ac; /* mov.b r13,@r14 */
L8c6600ac: *stop=0x8c6600acu;if(!valid_access(r[14]+3u,1u))return -3;wr(m,bus,&io_status,r[14]+0x3u,r[0],1);if(io_status)return io_status;goto L8c6600ae; /* mov.b r0,@(0x3,r14) */
L8c6600ae: *stop=0x8c6600aeu;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[13],4);if(io_status)return io_status;goto L8c6600b0; /* mov.l r13,@(0x24,r14) */
L8c6600b0: *stop=0x8c6600b0u;if(!valid_access(r[10],1u))return -3;r[2]=sx(m,bus,&io_status,r[10],1);if(io_status)return io_status;goto L8c6600b2; /* mov.b @r10,r2 */
L8c6600b2: *stop=0x8c6600b2u;r[6]=sx(m,bus,&io_status,0x8c66011cu,2);if(io_status)return io_status;goto L8c6600b4; /* mov.w 0x8c66011c,r6 */
L8c6600b4: *stop=0x8c6600b4u;r[2]=r[2]&255u;goto L8c6600b6; /* extu.b r2,r2 */
L8c6600b6: *stop=0x8c6600b6u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c6600b8; /* tst r5,r2 */
L8c6600b8: if((*sr&1u))goto L8c660124;goto L8c6600ba; /* bt 0x8c660124 */
L8c6600ba: *stop=0x8c6600bau;if(!valid_access(r[14]+7u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x7u,1);if(io_status)return io_status;goto L8c6600bc; /* mov.b @(0x7,r14),r0 */
L8c6600bc: *stop=0x8c6600bcu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6600be; /* tst r0,r0 */
L8c6600be: if(!(*sr&1u))goto L8c6600ca;goto L8c6600c0; /* bf 0x8c6600ca */
L8c6600c0: *stop=0x8c6600c0u;if(!valid_access(r[14],1u))return -3;r[2]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c6600c2; /* mov.b @r14,r2 */
L8c6600c2: *stop=0x8c6600c2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6600c4; /* tst r2,r2 */
L8c6600c4: if(!(*sr&1u))goto L8c660116;goto L8c6600c6; /* bf 0x8c660116 */
L8c6600c6: *stop=0x8c6600c8u;goto L8c66012a; /* bra 0x8c66012a */
L8c6600ca: *stop=0x8c6600cau;r[0]=r[0]&255u;goto L8c6600cc; /* extu.b r0,r0 */
L8c6600cc: *stop=0x8c6600ccu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6600ce; /* cmp/eq #0x1,r0 */
L8c6600ce: if(!(*sr&1u))goto L8c6600fa;goto L8c6600d0; /* bf 0x8c6600fa */
L8c6600d0: *stop=0x8c6600d0u;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c6600d2; /* mov.b @(0x8,r14),r0 */
L8c6600d2: *stop=0x8c6600d2u;r[0]=r[0]&255u;goto L8c6600d4; /* extu.b r0,r0 */
L8c6600d4: *stop=0x8c6600d4u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c6600d6; /* cmp/eq #0x5,r0 */
L8c6600d6: if(!(*sr&1u))goto L8c6600e0;goto L8c6600d8; /* bf 0x8c6600e0 */
L8c6600d8: *stop=0x8c6600d8u;r[0]=r[4];goto L8c6600da; /* mov r4,r0 */
L8c6600da: *stop=0x8c6600dau;if(!valid_access(r[14]+2u,1u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],1);if(io_status)return io_status;goto L8c6600dc; /* mov.b r0,@(0x2,r14) */
L8c6600dc: *stop=0x8c6600deu;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c660116; /* bra 0x8c660116 */
L8c6600e0: *stop=0x8c6600e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660120u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6600e2; /* mov.l 0x8c660120,r1 */
L8c6600e2: *stop=0x8c6600e2u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6600e4; /* mov.l @(0x24,r14),r3 */
L8c6600e4: *stop=0x8c6600e4u;r[0]=sx(m,bus,&io_status,0x8c66011eu,2);if(io_status)return io_status;goto L8c6600e6; /* mov.w 0x8c66011e,r0 */
L8c6600e6: *stop=0x8c6600e6u;r[3]+=1u;goto L8c6600e8; /* add #0x1,r3 */
L8c6600e8: *stop=0x8c6600e8u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[3],4);if(io_status)return io_status;goto L8c6600ea; /* mov.l r3,@(0x24,r14) */
L8c6600ea: *stop=0x8c6600eau;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6600ec; /* mov.l @r1,r2 */
L8c6600ec: *stop=0x8c6600ecu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6600ee; /* mov.l @(r0,r2),r3 */
L8c6600ee: *stop=0x8c6600eeu;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6600f0; /* mov.l @(0x24,r14),r0 */
L8c6600f0: *stop=0x8c6600f0u;*sr=(*sr&~1u)|(r[0]>r[3]);goto L8c6600f2; /* cmp/hi r3,r0 */
L8c6600f2: if((*sr&1u))goto L8c660106;goto L8c6600f4; /* bt 0x8c660106 */
L8c6600f4: *stop=0x8c6600f4u;r[0]=r[4];goto L8c6600f6; /* mov r4,r0 */
L8c6600f6: *stop=0x8c6600f8u;if(!valid_access(r[14]+4u,1u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],1);if(io_status)return io_status;goto L8c66012a; /* bra 0x8c66012a */
L8c6600fa: *stop=0x8c6600fau;if(!valid_access(r[14]+7u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x7u,1);if(io_status)return io_status;goto L8c6600fc; /* mov.b @(0x7,r14),r0 */
L8c6600fc: *stop=0x8c6600fcu;r[0]=r[0]&255u;goto L8c6600fe; /* extu.b r0,r0 */
L8c6600fe: *stop=0x8c6600feu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c660100; /* cmp/eq #0x4,r0 */
L8c660100: if(!(*sr&1u))goto L8c660110;goto L8c660102; /* bf 0x8c660110 */
L8c660102: *stop=0x8c660102u;r[0]=r[4];goto L8c660104; /* mov r4,r0 */
L8c660104: *stop=0x8c660104u;if(!valid_access(r[14]+3u,1u))return -3;wr(m,bus,&io_status,r[14]+0x3u,r[0],1);if(io_status)return io_status;goto L8c660106; /* mov.b r0,@(0x3,r14) */
L8c660106: *stop=0x8c660106u;r[0]=r[6];goto L8c660108; /* mov r6,r0 */
L8c660108: *stop=0x8c660108u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[13],1);if(io_status)return io_status;goto L8c66010a; /* mov.b r13,@r14 */
L8c66010a: *stop=0x8c66010au;if(!valid_access(r[14]+4u,1u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],1);if(io_status)return io_status;goto L8c66010c; /* mov.b r0,@(0x4,r14) */
L8c66010c: *stop=0x8c66010eu;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[13],4);if(io_status)return io_status;goto L8c66012a; /* bra 0x8c66012a */
L8c660110: *stop=0x8c660110u;if(!valid_access(r[14]+7u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x7u,1);if(io_status)return io_status;goto L8c660112; /* mov.b @(0x7,r14),r0 */
L8c660112: *stop=0x8c660112u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660114; /* tst r0,r0 */
L8c660114: if((*sr&1u))goto L8c66012a;goto L8c660116; /* bt 0x8c66012a */
L8c660116: *stop=0x8c660116u;r[0]=r[13];goto L8c660118; /* mov r13,r0 */
L8c660118: *stop=0x8c66011au;if(!valid_access(r[14]+4u,1u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],1);if(io_status)return io_status;goto L8c66012a; /* bra 0x8c66012a */
L8c660124: *stop=0x8c660124u;r[0]=r[6];goto L8c660126; /* mov r6,r0 */
L8c660126: *stop=0x8c660126u;if(!valid_access(r[14]+4u,1u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],1);if(io_status)return io_status;goto L8c660128; /* mov.b r0,@(0x4,r14) */
L8c660128: *stop=0x8c660128u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[13],1);if(io_status)return io_status;goto L8c66012a; /* mov.b r13,@r14 */
L8c66012a: *stop=0x8c66012au;if(!valid_access(r[14],1u))return -3;r[3]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c66012c; /* mov.b @r14,r3 */
L8c66012c: *stop=0x8c66012cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c66012e; /* tst r3,r3 */
L8c66012e: if((*sr&1u))goto L8c66015c;goto L8c660130; /* bt 0x8c66015c */
L8c660130: *stop=0x8c660130u;if(!valid_access(r[14]+7u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x7u,1);if(io_status)return io_status;goto L8c660132; /* mov.b @(0x7,r14),r0 */
L8c660132: *stop=0x8c660132u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660134; /* tst r0,r0 */
L8c660134: if((*sr&1u))goto L8c66015c;goto L8c660136; /* bt 0x8c66015c */
L8c660136: *stop=0x8c660136u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66022cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660138; /* mov.l 0x8c66022c,r3 */
L8c660138: *stop=0x8c660138u;r[1]=sx(m,bus,&io_status,0x8c660228u,2);if(io_status)return io_status;goto L8c66013a; /* mov.w 0x8c660228,r1 */
L8c66013a: *stop=0x8c66013au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66013c; /* mov.l @r3,r0 */
L8c66013c: *stop=0x8c66013cu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66013e; /* mov.l @(r0,r1),r0 */
L8c66013e: *stop=0x8c66013eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c660140; /* cmp/eq #0x1,r0 */
L8c660140: if(!(*sr&1u))goto L8c66015c;goto L8c660142; /* bf 0x8c66015c */
L8c660142: *stop=0x8c660142u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[0]=loaded;}goto L8c660144; /* mov.l @r12,r0 */
L8c660144: *stop=0x8c660144u;r[4]=r[0];goto L8c660146; /* mov r0,r4 */
L8c660146: *stop=0x8c660146u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c660148; /* cmp/eq #-0x1,r0 */
L8c660148: if((*sr&1u))goto L8c66015c;goto L8c66014a; /* bt 0x8c66015c */
L8c66014a: *stop=0x8c66014au;r[2]=sx(m,bus,&io_status,0x8c66022au,2);if(io_status)return io_status;goto L8c66014c; /* mov.w 0x8c66022a,r2 */
L8c66014c: *stop=0x8c66014cu;*sr=(*sr&~1u)|(r[2]==r[4]);goto L8c66014e; /* cmp/eq r2,r4 */
L8c66014e: if((*sr&1u))goto L8c66015c;goto L8c660150; /* bt 0x8c66015c */
L8c660150: *stop=0x8c660150u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[5]=loaded;}goto L8c660152; /* mov.l @r12,r5 */
L8c660152: *stop=0x8c660152u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660230u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660154; /* mov.l 0x8c660230,r3 */
L8c660154: *stop=0x8c660154u;r[5]=r[5]&255u;goto L8c660156; /* extu.b r5,r5 */
L8c660156: *stop=0x8c660156u;r[5]+=1u;goto L8c660158; /* add #0x1,r5 */
L8c660158: target=r[3];*pr=0x8c66015cu;*stop=0x8c66015au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66015c; /* jsr @r3 */
L8c66015c: *stop=0x8c66015cu;if(!valid_access(r[14]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,1);if(io_status)return io_status;goto L8c66015e; /* mov.b @(0x2,r14),r0 */
L8c66015e: *stop=0x8c66015eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660160; /* tst r0,r0 */
L8c660160: if((*sr&1u))goto L8c66016e;goto L8c660162; /* bt 0x8c66016e */
L8c660162: *stop=0x8c660162u;r[0]=16u;goto L8c660164; /* mov #0x10,r0 */
L8c660164: *stop=0x8c660164u;r[5]=r[14];goto L8c660166; /* mov r14,r5 */
L8c660166: *stop=0x8c660166u;r[6]=r[12];goto L8c660168; /* mov r12,r6 */
L8c660168: *stop=0x8c660168u;if(!valid_access(r[0]+r[11],1u))return -3;wr(m,bus,&io_status,r[11]+r[0],r[13],1);if(io_status)return io_status;goto L8c66016a; /* mov.b r13,@(r0,r11) */
L8c66016a: target=0x8c66018eu;*pr=0x8c66016eu;*stop=0x8c66016cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66016e; /* bsr 0x8c66018e */
L8c66016e: *stop=0x8c66016eu;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c660170; /* mov.b @(0x3,r14),r0 */
L8c660170: *stop=0x8c660170u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660172; /* tst r0,r0 */
L8c660172: if((*sr&1u))goto L8c66017c;goto L8c660174; /* bt 0x8c66017c */
L8c660174: *stop=0x8c660174u;r[0]=16u;goto L8c660176; /* mov #0x10,r0 */
L8c660176: *stop=0x8c660176u;if(!valid_access(r[0]+r[11],1u))return -3;wr(m,bus,&io_status,r[11]+r[0],r[13],1);if(io_status)return io_status;goto L8c660178; /* mov.b r13,@(r0,r11) */
L8c660178: target=0x8c6602d2u;*pr=0x8c66017cu;*stop=0x8c66017au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66017c; /* bsr 0x8c6602d2 */
L8c66017c: *stop=0x8c66017cu;r[0]=0u;goto L8c66017e; /* mov #0x0,r0 */
L8c66017e: *stop=0x8c66017eu;r[15]+=4u;goto L8c660180; /* add #0x4,r15 */
L8c660180: *stop=0x8c660180u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660182; /* lds.l @r15+,PR */
L8c660182: *stop=0x8c660182u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c660184; /* mov.l @r15+,r10 */
L8c660184: *stop=0x8c660184u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c660186; /* mov.l @r15+,r11 */
L8c660186: *stop=0x8c660186u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c660188; /* mov.l @r15+,r12 */
L8c660188: *stop=0x8c660188u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c66018a; /* mov.l @r15+,r13 */
L8c66018a: *stop=0x8c66018au;if(*pr!=entry_pr)return -3;*stop=0x8c66018cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c66018e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c66018e;
L8c66018e: *stop=0x8c66018eu;r[6]+=4u;goto L8c660190; /* add #0x4,r6 */
L8c660190: *stop=0x8c660190u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660234u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660192; /* mov.l 0x8c660234,r3 */
L8c660192: *stop=0x8c660192u;r[5]=r[6];goto L8c660194; /* mov r6,r5 */
L8c660194: *stop=0x8c660194u;if(!valid_access(r[5]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x1u,1);if(io_status)return io_status;goto L8c660196; /* mov.b @(0x1,r5),r0 */
L8c660196: *stop=0x8c660196u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660238u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660198; /* mov.l 0x8c660238,r2 */
L8c660198: *stop=0x8c660198u;r[6]=r[4];goto L8c66019a; /* mov r4,r6 */
L8c66019a: *stop=0x8c66019au;r[0]=r[0]&255u;goto L8c66019c; /* extu.b r0,r0 */
L8c66019c: *stop=0x8c66019cu;r[0]<<=16;goto L8c66019e; /* shll16 r0 */
L8c66019e: *stop=0x8c66019eu;r[0]&=r[3];goto L8c6601a0; /* and r3,r0 */
L8c6601a0: *stop=0x8c6601a0u;r[1]=r[0];goto L8c6601a2; /* mov r0,r1 */
L8c6601a2: *stop=0x8c6601a2u;if(!valid_access(r[5]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x2u,1);if(io_status)return io_status;goto L8c6601a4; /* mov.b @(0x2,r5),r0 */
L8c6601a4: *stop=0x8c6601a4u;r[3]<<=8;goto L8c6601a6; /* shll8 r3 */
L8c6601a6: *stop=0x8c6601a6u;r[0]=r[0]&255u;goto L8c6601a8; /* extu.b r0,r0 */
L8c6601a8: *stop=0x8c6601a8u;r[0]<<=8;goto L8c6601aa; /* shll8 r0 */
L8c6601aa: *stop=0x8c6601aau;r[0]&=r[2];goto L8c6601ac; /* and r2,r0 */
L8c6601ac: *stop=0x8c6601acu;r[1]|=r[0];goto L8c6601ae; /* or r0,r1 */
L8c6601ae: *stop=0x8c6601aeu;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c6601b0; /* mov.b @r5,r0 */
L8c6601b0: *stop=0x8c6601b0u;r[7]=0u;goto L8c6601b2; /* mov #0x0,r7 */
L8c6601b2: *stop=0x8c6601b2u;r[0]=r[0]&255u;goto L8c6601b4; /* extu.b r0,r0 */
L8c6601b4: *stop=0x8c6601b4u;r[0]<<=16;goto L8c6601b6; /* shll16 r0 */
L8c6601b6: *stop=0x8c6601b6u;r[0]<<=8;goto L8c6601b8; /* shll8 r0 */
L8c6601b8: *stop=0x8c6601b8u;r[0]&=r[3];goto L8c6601ba; /* and r3,r0 */
L8c6601ba: *stop=0x8c6601bau;r[1]|=r[0];goto L8c6601bc; /* or r0,r1 */
L8c6601bc: *stop=0x8c6601bcu;if(!valid_access(r[5]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x3u,1);if(io_status)return io_status;goto L8c6601be; /* mov.b @(0x3,r5),r0 */
L8c6601be: *stop=0x8c6601beu;r[5]+=4u;goto L8c6601c0; /* add #0x4,r5 */
L8c6601c0: *stop=0x8c6601c0u;r[0]=r[0]&255u;goto L8c6601c2; /* extu.b r0,r0 */
L8c6601c2: *stop=0x8c6601c2u;r[1]|=r[0];goto L8c6601c4; /* or r0,r1 */
L8c6601c4: *stop=0x8c6601c4u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c6601c6; /* mov.l r1,@r4 */
L8c6601c6: *stop=0x8c6601c8u;r[6]+=4u;goto L8c6601d0; /* bra 0x8c6601d0 */
L8c6601ca: *stop=0x8c6601cau;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c6601cc; /* mov.b @r5+,r3 */
L8c6601cc: *stop=0x8c6601ccu;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[3],1);if(io_status)return io_status;goto L8c6601ce; /* mov.b r3,@r6 */
L8c6601ce: *stop=0x8c6601ceu;r[6]+=1u;goto L8c6601d0; /* add #0x1,r6 */
L8c6601d0: *stop=0x8c6601d0u;r[2]=r[4];goto L8c6601d2; /* mov r4,r2 */
L8c6601d2: *stop=0x8c6601d2u;r[2]+=8u;goto L8c6601d4; /* add #0x8,r2 */
L8c6601d4: *stop=0x8c6601d4u;*sr=(*sr&~1u)|(r[6]>=r[2]);goto L8c6601d6; /* cmp/hs r2,r6 */
L8c6601d6: if(!(*sr&1u))goto L8c6601ca;goto L8c6601d8; /* bf 0x8c6601ca */
L8c6601d8: *stop=0x8c6601d8u;r[6]=r[4];goto L8c6601da; /* mov r4,r6 */
L8c6601da: *stop=0x8c6601dcu;r[6]+=8u;goto L8c6601e4; /* bra 0x8c6601e4 */
L8c6601de: *stop=0x8c6601deu;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c6601e0; /* mov.b @r5+,r3 */
L8c6601e0: *stop=0x8c6601e0u;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[3],1);if(io_status)return io_status;goto L8c6601e2; /* mov.b r3,@r6 */
L8c6601e2: *stop=0x8c6601e2u;r[6]+=1u;goto L8c6601e4; /* add #0x1,r6 */
L8c6601e4: *stop=0x8c6601e4u;r[2]=r[4];goto L8c6601e6; /* mov r4,r2 */
L8c6601e6: *stop=0x8c6601e6u;r[2]+=12u;goto L8c6601e8; /* add #0xc,r2 */
L8c6601e8: *stop=0x8c6601e8u;*sr=(*sr&~1u)|(r[6]>=r[2]);goto L8c6601ea; /* cmp/hs r2,r6 */
L8c6601ea: if(!(*sr&1u))goto L8c6601de;goto L8c6601ec; /* bf 0x8c6601de */
L8c6601ec: *stop=0x8c6601ecu;r[6]=r[4];goto L8c6601ee; /* mov r4,r6 */
L8c6601ee: *stop=0x8c6601f0u;r[6]+=12u;goto L8c6601f8; /* bra 0x8c6601f8 */
L8c6601f2: *stop=0x8c6601f2u;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c6601f4; /* mov.b @r5+,r3 */
L8c6601f4: *stop=0x8c6601f4u;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[3],1);if(io_status)return io_status;goto L8c6601f6; /* mov.b r3,@r6 */
L8c6601f6: *stop=0x8c6601f6u;r[6]+=1u;goto L8c6601f8; /* add #0x1,r6 */
L8c6601f8: *stop=0x8c6601f8u;r[2]=r[4];goto L8c6601fa; /* mov r4,r2 */
L8c6601fa: *stop=0x8c6601fau;r[2]+=16u;goto L8c6601fc; /* add #0x10,r2 */
L8c6601fc: *stop=0x8c6601fcu;*sr=(*sr&~1u)|(r[6]>=r[2]);goto L8c6601fe; /* cmp/hs r2,r6 */
L8c6601fe: if(!(*sr&1u))goto L8c6601f2;goto L8c660200; /* bf 0x8c6601f2 */
L8c660200: *stop=0x8c660200u;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c660202; /* mov.b @r5+,r3 */
L8c660202: *stop=0x8c660202u;r[0]=17u;goto L8c660204; /* mov #0x11,r0 */
L8c660204: *stop=0x8c660204u;if(!valid_access(r[0]+r[4],1u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[3],1);if(io_status)return io_status;goto L8c660206; /* mov.b r3,@(r0,r4) */
L8c660206: *stop=0x8c660206u;r[0]=16u;goto L8c660208; /* mov #0x10,r0 */
L8c660208: *stop=0x8c660208u;if(!valid_access(r[0]+r[4],1u))return -3;r[2]=sx(m,bus,&io_status,r[4]+r[0],1);if(io_status)return io_status;goto L8c66020a; /* mov.b @(r0,r4),r2 */
L8c66020a: *stop=0x8c66020au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c66020c; /* tst r2,r2 */
L8c66020c: if((*sr&1u))goto L8c66023c;goto L8c66020e; /* bt 0x8c66023c */
L8c66020e: *stop=0x8c66020eu;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c660210; /* mov.b @r5,r0 */
L8c660210: *stop=0x8c660210u;r[2]=r[4];goto L8c660212; /* mov r4,r2 */
L8c660212: *stop=0x8c660212u;r[2]+=18u;goto L8c660214; /* add #0x12,r2 */
L8c660214: *stop=0x8c660214u;r[0]&=0x3u;goto L8c660216; /* and #0x3,r0 */
L8c660216: *stop=0x8c660216u;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[0],1);if(io_status)return io_status;goto L8c660218; /* mov.b r0,@r2 */
L8c660218: *stop=0x8c660218u;r[1]=19u;goto L8c66021a; /* mov #0x13,r1 */
L8c66021a: *stop=0x8c66021au;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c66021c; /* mov.b @r5,r0 */
L8c66021c: *stop=0x8c66021cu;r[1]+=r[4];goto L8c66021e; /* add r4,r1 */
L8c66021e: *stop=0x8c66021eu;r[0]=r[0]&255u;goto L8c660220; /* extu.b r0,r0 */
L8c660220: *stop=0x8c660220u;r[0]>>=2;goto L8c660222; /* shlr2 r0 */
L8c660222: *stop=0x8c660222u;r[0]&=0x3u;goto L8c660224; /* and #0x3,r0 */
L8c660224: *stop=0x8c660226u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[0],1);if(io_status)return io_status;goto L8c660250; /* bra 0x8c660250 */
L8c66023c: *stop=0x8c66023cu;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c66023e; /* mov.b @r5,r0 */
L8c66023e: *stop=0x8c66023eu;r[2]=r[4];goto L8c660240; /* mov r4,r2 */
L8c660240: *stop=0x8c660240u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660370u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c660242; /* mov.l 0x8c660370,r1 */
L8c660242: *stop=0x8c660242u;r[2]+=18u;goto L8c660244; /* add #0x12,r2 */
L8c660244: *stop=0x8c660244u;r[0]=r[0]&255u;goto L8c660246; /* extu.b r0,r0 */
L8c660246: *stop=0x8c660246u;r[0]&=0xfu;goto L8c660248; /* and #0xf,r0 */
L8c660248: *stop=0x8c660248u;if(!valid_access(r[0]+r[1],1u))return -3;r[3]=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;goto L8c66024a; /* mov.b @(r0,r1),r3 */
L8c66024a: *stop=0x8c66024au;r[0]=19u;goto L8c66024c; /* mov #0x13,r0 */
L8c66024c: *stop=0x8c66024cu;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[3],1);if(io_status)return io_status;goto L8c66024e; /* mov.b r3,@r2 */
L8c66024e: *stop=0x8c66024eu;if(!valid_access(r[0]+r[4],1u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[7],1);if(io_status)return io_status;goto L8c660250; /* mov.b r7,@(r0,r4) */
L8c660250: *stop=0x8c660250u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660374u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660252; /* mov.l 0x8c660374,r3 */
L8c660252: *stop=0x8c660252u;r[1]=sx(m,bus,&io_status,0x8c66036cu,2);if(io_status)return io_status;goto L8c660254; /* mov.w 0x8c66036c,r1 */
L8c660254: *stop=0x8c660254u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c660256; /* mov.l @r3,r0 */
L8c660256: *stop=0x8c660256u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c660258; /* mov.l @(r0,r1),r0 */
L8c660258: *stop=0x8c660258u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c66025a; /* cmp/eq #0x1,r0 */
L8c66025a: if(!(*sr&1u))goto L8c660274;goto L8c66025c; /* bf 0x8c660274 */
L8c66025c: *stop=0x8c66025cu;r[6]=r[4];goto L8c66025e; /* mov r4,r6 */
L8c66025e: *stop=0x8c660260u;r[6]+=20u;goto L8c660268; /* bra 0x8c660268 */
L8c660262: *stop=0x8c660262u;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c660264; /* mov.b @r5+,r3 */
L8c660264: *stop=0x8c660264u;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[3],1);if(io_status)return io_status;goto L8c660266; /* mov.b r3,@r6 */
L8c660266: *stop=0x8c660266u;r[6]+=1u;goto L8c660268; /* add #0x1,r6 */
L8c660268: *stop=0x8c660268u;r[2]=r[4];goto L8c66026a; /* mov r4,r2 */
L8c66026a: *stop=0x8c66026au;r[2]+=51u;goto L8c66026c; /* add #0x33,r2 */
L8c66026c: *stop=0x8c66026cu;*sr=(*sr&~1u)|(r[6]>=r[2]);goto L8c66026e; /* cmp/hs r2,r6 */
L8c66026e: if(!(*sr&1u))goto L8c660262;goto L8c660270; /* bf 0x8c660262 */
L8c660270: *stop=0x8c660272u;goto L8c66028e; /* bra 0x8c66028e */
L8c660274: *stop=0x8c660274u;r[6]=r[4];goto L8c660276; /* mov r4,r6 */
L8c660276: *stop=0x8c660276u;r[5]+=1u;goto L8c660278; /* add #0x1,r5 */
L8c660278: *stop=0x8c66027au;r[6]+=20u;goto L8c660282; /* bra 0x8c660282 */
L8c66027c: *stop=0x8c66027cu;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c66027e; /* mov.b @r5+,r3 */
L8c66027e: *stop=0x8c66027eu;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[3],1);if(io_status)return io_status;goto L8c660280; /* mov.b r3,@r6 */
L8c660280: *stop=0x8c660280u;r[6]+=1u;goto L8c660282; /* add #0x1,r6 */
L8c660282: *stop=0x8c660282u;r[2]=r[4];goto L8c660284; /* mov r4,r2 */
L8c660284: *stop=0x8c660284u;r[2]+=50u;goto L8c660286; /* add #0x32,r2 */
L8c660286: *stop=0x8c660286u;*sr=(*sr&~1u)|(r[6]>=r[2]);goto L8c660288; /* cmp/hs r2,r6 */
L8c660288: if(!(*sr&1u))goto L8c66027c;goto L8c66028a; /* bf 0x8c66027c */
L8c66028a: *stop=0x8c66028au;r[0]=50u;goto L8c66028c; /* mov #0x32,r0 */
L8c66028c: *stop=0x8c66028cu;if(!valid_access(r[0]+r[4],1u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[7],1);if(io_status)return io_status;goto L8c66028e; /* mov.b r7,@(r0,r4) */
L8c66028e: *stop=0x8c66028eu;r[0]=51u;goto L8c660290; /* mov #0x33,r0 */
L8c660290: *stop=0x8c660290u;r[6]=r[4];goto L8c660292; /* mov r4,r6 */
L8c660292: *stop=0x8c660292u;if(!valid_access(r[0]+r[4],1u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[7],1);if(io_status)return io_status;goto L8c660294; /* mov.b r7,@(r0,r4) */
L8c660294: *stop=0x8c660296u;r[6]+=52u;goto L8c66029e; /* bra 0x8c66029e */
L8c660298: *stop=0x8c660298u;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c66029a; /* mov.b @r5+,r3 */
L8c66029a: *stop=0x8c66029au;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[3],1);if(io_status)return io_status;goto L8c66029c; /* mov.b r3,@r6 */
L8c66029c: *stop=0x8c66029cu;r[6]+=1u;goto L8c66029e; /* add #0x1,r6 */
L8c66029e: *stop=0x8c66029eu;r[2]=r[4];goto L8c6602a0; /* mov r4,r2 */
L8c6602a0: *stop=0x8c6602a0u;r[2]+=112u;goto L8c6602a2; /* add #0x70,r2 */
L8c6602a2: *stop=0x8c6602a2u;*sr=(*sr&~1u)|(r[6]>=r[2]);goto L8c6602a4; /* cmp/hs r2,r6 */
L8c6602a4: if(!(*sr&1u))goto L8c660298;goto L8c6602a6; /* bf 0x8c660298 */
L8c6602a6: *stop=0x8c6602a6u;r[0]=112u;goto L8c6602a8; /* mov #0x70,r0 */
L8c6602a8: *stop=0x8c6602a8u;if(!valid_access(r[0]+r[4],1u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[7],1);if(io_status)return io_status;goto L8c6602aa; /* mov.b r7,@(r0,r4) */
L8c6602aa: *stop=0x8c6602aau;r[1]=116u;goto L8c6602ac; /* mov #0x74,r1 */
L8c6602ac: *stop=0x8c6602acu;if(!valid_access(r[5]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x1u,1);if(io_status)return io_status;goto L8c6602ae; /* mov.b @(0x1,r5),r0 */
L8c6602ae: *stop=0x8c6602aeu;r[1]+=r[4];goto L8c6602b0; /* add r4,r1 */
L8c6602b0: *stop=0x8c6602b0u;if(!valid_access(r[5],1u))return -3;r[3]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c6602b2; /* mov.b @r5,r3 */
L8c6602b2: *stop=0x8c6602b2u;r[0]=r[0]&255u;goto L8c6602b4; /* extu.b r0,r0 */
L8c6602b4: *stop=0x8c6602b4u;r[3]=r[3]&255u;goto L8c6602b6; /* extu.b r3,r3 */
L8c6602b6: *stop=0x8c6602b6u;r[0]<<=8;goto L8c6602b8; /* shll8 r0 */
L8c6602b8: *stop=0x8c6602b8u;r[0]|=r[3];goto L8c6602ba; /* or r3,r0 */
L8c6602ba: *stop=0x8c6602bau;if(!valid_access(r[1],2u))return -3;wr(m,bus,&io_status,r[1],r[0],2);if(io_status)return io_status;goto L8c6602bc; /* mov.w r0,@r1 */
L8c6602bc: *stop=0x8c6602bcu;if(!valid_access(r[5]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x3u,1);if(io_status)return io_status;goto L8c6602be; /* mov.b @(0x3,r5),r0 */
L8c6602be: *stop=0x8c6602beu;r[0]=r[0]&255u;goto L8c6602c0; /* extu.b r0,r0 */
L8c6602c0: *stop=0x8c6602c0u;r[0]<<=8;goto L8c6602c2; /* shll8 r0 */
L8c6602c2: *stop=0x8c6602c2u;r[3]=r[0];goto L8c6602c4; /* mov r0,r3 */
L8c6602c4: *stop=0x8c6602c4u;if(!valid_access(r[5]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x2u,1);if(io_status)return io_status;goto L8c6602c6; /* mov.b @(0x2,r5),r0 */
L8c6602c6: *stop=0x8c6602c6u;r[0]=r[0]&255u;goto L8c6602c8; /* extu.b r0,r0 */
L8c6602c8: *stop=0x8c6602c8u;r[3]|=r[0];goto L8c6602ca; /* or r0,r3 */
L8c6602ca: *stop=0x8c6602cau;r[0]=118u;goto L8c6602cc; /* mov #0x76,r0 */
L8c6602cc: *stop=0x8c6602ccu;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[3],2);if(io_status)return io_status;goto L8c6602ce; /* mov.w r3,@(r0,r4) */
L8c6602ce: *stop=0x8c6602ceu;if(*pr!=entry_pr)return -3;*stop=0x8c6602d0u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_input_pump_8c668b24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c668b24;
L8c668b24: *stop=0x8c668b24u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c668b26; /* mov.l @r4,r0 */
L8c668b26: *stop=0x8c668b26u;r[1]=(r[0]&0xffff0000u)|((r[0]&255u)<<8)|((r[0]>>8)&255u);goto L8c668b28; /* swap.b r0,r1 */
L8c668b28: *stop=0x8c668b28u;r[0]=(r[1]<<16)|(r[1]>>16);goto L8c668b2a; /* swap.w r1,r0 */
L8c668b2a: *stop=0x8c668b2au;r[1]=(r[0]&0xffff0000u)|((r[0]&255u)<<8)|((r[0]>>8)&255u);goto L8c668b2c; /* swap.b r0,r1 */
L8c668b2c: *stop=0x8c668b2cu;r[5]--;*sr=(*sr&~1u)|(!r[5]);goto L8c668b2e; /* dt r5 */
L8c668b2e: *stop=0x8c668b2eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c668b30; /* mov.l r1,@r4 */
L8c668b30: take=!(*sr&1u);*stop=0x8c668b32u;r[4]+=4u;if(take)goto L8c668b24;goto L8c668b34; /* bf/s 0x8c668b24 */
L8c668b34: *stop=0x8c668b34u;if(*pr!=entry_pr)return -3;*stop=0x8c668b36u;r[0]=0u;return 0; /* rts */
}
