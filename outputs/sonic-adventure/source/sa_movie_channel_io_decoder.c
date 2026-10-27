/* Original channel descriptor decode, validation and callback paths.
 * Static C from byte-checked SH-4 instructions; complete explicit state.
 * Missing callees remain reached boundaries; no callback result is substituted.
 * Context and interrupt entry remain explicit platform state.
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
int sa_movie_channel_io_return_allowed(void *,uint32_t);
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
static int sa_movie_channel_io_decoder_run_8c6482a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c648312u)goto L8c648312;if(resume_pc==0x8c64831cu)goto L8c64831c;if(resume_pc==0x8c648332u)goto L8c648332;goto L8c6482a0;
L8c6482a0: *stop=0x8c6482a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6482a2; /* mov.l r14,@-r15 */
L8c6482a2: *stop=0x8c6482a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6482a4; /* mov.l r13,@-r15 */
L8c6482a4: *stop=0x8c6482a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6482a6; /* mov.l r12,@-r15 */
L8c6482a6: *stop=0x8c6482a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6482a8; /* mov.l r11,@-r15 */
L8c6482a8: *stop=0x8c6482a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6482aa; /* sts.l PR,@-r15 */
L8c6482aa: *stop=0x8c6482aau;r[15]+=4294967292u;goto L8c6482ac; /* add #-0x4,r15 */
L8c6482ac: *stop=0x8c6482acu;r[14]=sx(m,bus,&io_status,0x8c648360u,2);if(io_status)return io_status;goto L8c6482ae; /* mov.w 0x8c648360,r14 */
L8c6482ae: *stop=0x8c6482aeu;r[13]=r[4];goto L8c6482b0; /* mov r4,r13 */
L8c6482b0: *stop=0x8c6482b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648370u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6482b2; /* mov.l 0x8c648370,r2 */
L8c6482b2: *stop=0x8c6482b2u;*macl=r[14]*r[13];goto L8c6482b4; /* mul.l r14,r13 */
L8c6482b4: *stop=0x8c6482b4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6482b6; /* mov.l @r2,r0 */
L8c6482b6: *stop=0x8c6482b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648368u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6482b8; /* mov.l 0x8c648368,r3 */
L8c6482b8: *stop=0x8c6482b8u;if(!valid_access(r[0]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6482ba; /* mov.l @(0x8,r0),r0 */
L8c6482ba: *stop=0x8c6482bau;r[14]=*macl;goto L8c6482bc; /* sts MACL,r14 */
L8c6482bc: *stop=0x8c6482bcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6482be; /* cmp/eq #0x1,r0 */
L8c6482be: take=!(*sr&1u);*stop=0x8c6482c0u;r[14]+=r[3];if(take)goto L8c6482d2;goto L8c6482c2; /* bf/s 0x8c6482d2 */
L8c6482c2: *stop=0x8c6482c2u;r[4]=r[13];goto L8c6482c4; /* mov r13,r4 */
L8c6482c4: *stop=0x8c6482c4u;r[15]+=4u;goto L8c6482c6; /* add #0x4,r15 */
L8c6482c6: *stop=0x8c6482c6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6482c8; /* lds.l @r15+,PR */
L8c6482c8: *stop=0x8c6482c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6482ca; /* mov.l @r15+,r11 */
L8c6482ca: *stop=0x8c6482cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6482cc; /* mov.l @r15+,r12 */
L8c6482cc: *stop=0x8c6482ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6482ce; /* mov.l @r15+,r13 */
L8c6482ce: *stop=0x8c6482d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,0x8c648470u,m,f,sr,r,pr); /* bra 0x8c648470 */
L8c6482d2: *stop=0x8c6482d2u;if(!valid_access(r[14]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,1);if(io_status)return io_status;goto L8c6482d4; /* mov.b @(0x2,r14),r0 */
L8c6482d4: *stop=0x8c6482d4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6482d6; /* tst r0,r0 */
L8c6482d6: if((*sr&1u))goto L8c648326;goto L8c6482d8; /* bt 0x8c648326 */
L8c6482d8: *stop=0x8c6482d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648374u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6482da; /* mov.l 0x8c648374,r0 */
L8c6482da: *stop=0x8c6482dau;r[11]=1u;goto L8c6482dc; /* mov #0x1,r11 */
L8c6482dc: *stop=0x8c6482dcu;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[11],4);if(io_status)return io_status;goto L8c6482de; /* mov.l r11,@(0x30,r14) */
L8c6482de: *stop=0x8c6482deu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6482e0; /* mov.l @(0x4,r14),r2 */
L8c6482e0: *stop=0x8c6482e0u;r[3]=r[2];goto L8c6482e2; /* mov r2,r3 */
L8c6482e2: *stop=0x8c6482e2u;r[2]<<=2;goto L8c6482e4; /* shll2 r2 */
L8c6482e4: *stop=0x8c6482e4u;r[2]+=r[3];goto L8c6482e6; /* add r3,r2 */
L8c6482e6: *stop=0x8c6482e6u;r[3]=r[13];goto L8c6482e8; /* mov r13,r3 */
L8c6482e8: *stop=0x8c6482e8u;r[2]<<=2;goto L8c6482ea; /* shll2 r2 */
L8c6482ea: *stop=0x8c6482eau;r[3]<<=2;goto L8c6482ec; /* shll2 r3 */
L8c6482ec: *stop=0x8c6482ecu;r[2]<<=2;goto L8c6482ee; /* shll2 r2 */
L8c6482ee: *stop=0x8c6482eeu;r[3]<<=2;goto L8c6482f0; /* shll2 r3 */
L8c6482f0: *stop=0x8c6482f0u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6482f2; /* mov.l @(r0,r2),r1 */
L8c6482f2: *stop=0x8c6482f2u;r[3]<<=2;goto L8c6482f4; /* shll2 r3 */
L8c6482f4: *stop=0x8c6482f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648378u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6482f6; /* mov.l 0x8c648378,r2 */
L8c6482f6: *stop=0x8c6482f6u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c6482f8; /* shll r3 */
L8c6482f8: *stop=0x8c6482f8u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[1],4);if(io_status)return io_status;goto L8c6482fa; /* mov.l r1,@(0x34,r14) */
L8c6482fa: *stop=0x8c6482fau;r[3]+=r[2];goto L8c6482fc; /* add r2,r3 */
L8c6482fc: *stop=0x8c6482fcu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6482fe; /* mov.l r3,@r15 */
L8c6482fe: *stop=0x8c6482feu;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c648300; /* mov.l @(0x30,r14),r6 */
L8c648300: *stop=0x8c648300u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64837cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648302; /* mov.l 0x8c64837c,r3 */
L8c648302: *stop=0x8c648302u;r[6]+=1u;goto L8c648304; /* add #0x1,r6 */
L8c648304: *stop=0x8c648304u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c648306; /* mov.l @(0x34,r14),r7 */
L8c648306: *stop=0x8c648306u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[6],4);if(io_status)return io_status;goto L8c648308; /* mov.l r6,@(0x30,r14) */
L8c648308: *stop=0x8c648308u;r[6]+=4294967295u;goto L8c64830a; /* add #-0x1,r6 */
L8c64830a: *stop=0x8c64830au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64830c; /* mov.l @r15,r5 */
L8c64830c: *stop=0x8c64830cu;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64830e; /* mov.l @(0x8,r5),r5 */
L8c64830e: target=r[3];*pr=0x8c648312u;*stop=0x8c648310u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648312; /* jsr @r3 */
L8c648312: *stop=0x8c648312u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648380u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648314; /* mov.l 0x8c648380,r2 */
L8c648314: *stop=0x8c648314u;r[12]=0u;goto L8c648316; /* mov #0x0,r12 */
L8c648316: *stop=0x8c648316u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c648318; /* mov.l @(0x1c,r14),r5 */
L8c648318: target=r[2];*pr=0x8c64831cu;*stop=0x8c64831au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64831c; /* jsr @r2 */
L8c64831c: *stop=0x8c64831cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64831e; /* cmp/pz r0 */
L8c64831e: if((*sr&1u))goto L8c64832c;goto L8c648320; /* bt 0x8c64832c */
L8c648320: *stop=0x8c648320u;r[0]=r[12];goto L8c648322; /* mov r12,r0 */
L8c648322: *stop=0x8c648322u;if(!valid_access(r[14]+9u,1u))return -3;wr(m,bus,&io_status,r[14]+0x9u,r[0],1);if(io_status)return io_status;goto L8c648324; /* mov.b r0,@(0x9,r14) */
L8c648324: *stop=0x8c648324u;if(!valid_access(r[14]+8u,1u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],1);if(io_status)return io_status;goto L8c648326; /* mov.b r0,@(0x8,r14) */
L8c648326: *stop=0x8c648326u;r[0]=sx(m,bus,&io_status,0x8c648362u,2);if(io_status)return io_status;goto L8c648328; /* mov.w 0x8c648362,r0 */
L8c648328: *stop=0x8c64832au;goto L8c64838c; /* bra 0x8c64838c */
L8c64832c: *stop=0x8c64832cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c648384u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64832e; /* mov.l 0x8c648384,r2 */
L8c64832e: target=r[2];*pr=0x8c648332u;*stop=0x8c648330u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648332; /* jsr @r2 */
L8c648332: *stop=0x8c648332u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648334; /* cmp/pz r0 */
L8c648334: if((*sr&1u))goto L8c64833a;goto L8c648336; /* bt 0x8c64833a */
L8c648336: *stop=0x8c648338u;r[0]=4294967171u;goto L8c64838c; /* bra 0x8c64838c */
L8c64833a: *stop=0x8c64833au;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64833c; /* mov.l @(0x1c,r14),r4 */
L8c64833c: *stop=0x8c64833cu;r[0]=r[11];goto L8c64833e; /* mov r11,r0 */
L8c64833e: *stop=0x8c64833eu;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648340; /* mov.l @(0x1c,r4),r2 */
L8c648340: *stop=0x8c648340u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[2],4);if(io_status)return io_status;goto L8c648342; /* mov.l r2,@(0x30,r4) */
L8c648342: *stop=0x8c648342u;r[4]=r[13];goto L8c648344; /* mov r13,r4 */
L8c648344: *stop=0x8c648344u;if(!valid_access(r[14]+9u,1u))return -3;wr(m,bus,&io_status,r[14]+0x9u,r[0],1);if(io_status)return io_status;goto L8c648346; /* mov.b r0,@(0x9,r14) */
L8c648346: *stop=0x8c648346u;if(!valid_access(r[14]+8u,1u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],1);if(io_status)return io_status;goto L8c648348; /* mov.b r0,@(0x8,r14) */
L8c648348: *stop=0x8c648348u;r[0]=r[12];goto L8c64834a; /* mov r12,r0 */
L8c64834a: *stop=0x8c64834au;if(!valid_access(r[14]+10u,1u))return -3;wr(m,bus,&io_status,r[14]+0xau,r[0],1);if(io_status)return io_status;goto L8c64834c; /* mov.b r0,@(0xa,r14) */
L8c64834c: *stop=0x8c64834cu;if(!valid_access(r[14]+11u,1u))return -3;wr(m,bus,&io_status,r[14]+0xbu,r[0],1);if(io_status)return io_status;goto L8c64834e; /* mov.b r0,@(0xb,r14) */
L8c64834e: *stop=0x8c64834eu;if(!valid_access(r[14]+12u,1u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],1);if(io_status)return io_status;goto L8c648350; /* mov.b r0,@(0xc,r14) */
L8c648350: *stop=0x8c648350u;r[15]+=4u;goto L8c648352; /* add #0x4,r15 */
L8c648352: *stop=0x8c648352u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c648354; /* lds.l @r15+,PR */
L8c648354: *stop=0x8c648354u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648388u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648356; /* mov.l 0x8c648388,r3 */
L8c648356: *stop=0x8c648356u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c648358; /* mov.l @r15+,r11 */
L8c648358: *stop=0x8c648358u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64835a; /* mov.l @r15+,r12 */
L8c64835a: *stop=0x8c64835au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64835c; /* mov.l @r15+,r13 */
L8c64835c: target=r[3];*stop=0x8c64835eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c64838c: *stop=0x8c64838cu;r[15]+=4u;goto L8c64838e; /* add #0x4,r15 */
L8c64838e: *stop=0x8c64838eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c648390; /* lds.l @r15+,PR */
L8c648390: *stop=0x8c648390u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c648392; /* mov.l @r15+,r11 */
L8c648392: *stop=0x8c648392u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c648394; /* mov.l @r15+,r12 */
L8c648394: *stop=0x8c648394u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c648396; /* mov.l @r15+,r13 */
L8c648396: *stop=0x8c648396u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c648398u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_decoder_8c6482a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c6482a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c648312(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c6482a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648312u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c64831c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c6482a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64831cu);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c648332(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c6482a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648332u);}


static int sa_movie_channel_io_decoder_run_8c64839a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c6483f6u)goto L8c6483f6;if(resume_pc==0x8c648400u)goto L8c648400;if(resume_pc==0x8c648412u)goto L8c648412;if(resume_pc==0x8c648426u)goto L8c648426;goto L8c64839a;
L8c64839a: *stop=0x8c64839au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64839c; /* mov.l r14,@-r15 */
L8c64839c: *stop=0x8c64839cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64839e; /* mov.l r13,@-r15 */
L8c64839e: *stop=0x8c64839eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6483a0; /* mov.l r12,@-r15 */
L8c6483a0: *stop=0x8c6483a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6483a2; /* sts.l PR,@-r15 */
L8c6483a2: *stop=0x8c6483a2u;r[15]+=4294967292u;goto L8c6483a4; /* add #-0x4,r15 */
L8c6483a4: *stop=0x8c6483a4u;r[14]=sx(m,bus,&io_status,0x8c648490u,2);if(io_status)return io_status;goto L8c6483a6; /* mov.w 0x8c648490,r14 */
L8c6483a6: *stop=0x8c6483a6u;r[13]=r[4];goto L8c6483a8; /* mov r4,r13 */
L8c6483a8: *stop=0x8c6483a8u;r[2]=76u;goto L8c6483aa; /* mov #0x4c,r2 */
L8c6483aa: *stop=0x8c6483aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c648498u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6483ac; /* mov.l 0x8c648498,r1 */
L8c6483ac: *stop=0x8c6483acu;*macl=r[14]*r[13];goto L8c6483ae; /* mul.l r14,r13 */
L8c6483ae: *stop=0x8c6483aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c648494u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6483b0; /* mov.l 0x8c648494,r3 */
L8c6483b0: *stop=0x8c6483b0u;r[0]=1u;goto L8c6483b2; /* mov #0x1,r0 */
L8c6483b2: *stop=0x8c6483b2u;r[14]=*macl;goto L8c6483b4; /* sts MACL,r14 */
L8c6483b4: *stop=0x8c6483b4u;*macl=r[2]*r[13];goto L8c6483b6; /* mul.l r2,r13 */
L8c6483b6: *stop=0x8c6483b6u;r[14]+=r[3];goto L8c6483b8; /* add r3,r14 */
L8c6483b8: *stop=0x8c6483b8u;r[3]=r[13];goto L8c6483ba; /* mov r13,r3 */
L8c6483ba: *stop=0x8c6483bau;r[3]<<=2;goto L8c6483bc; /* shll2 r3 */
L8c6483bc: *stop=0x8c6483bcu;r[2]=*macl;goto L8c6483be; /* sts MACL,r2 */
L8c6483be: *stop=0x8c6483beu;r[3]<<=2;goto L8c6483c0; /* shll2 r3 */
L8c6483c0: *stop=0x8c6483c0u;r[3]<<=2;goto L8c6483c2; /* shll2 r3 */
L8c6483c2: *stop=0x8c6483c2u;r[2]+=r[1];goto L8c6483c4; /* add r1,r2 */
L8c6483c4: *stop=0x8c6483c4u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c6483c6; /* mov.l r2,@(0x1c,r14) */
L8c6483c6: *stop=0x8c6483c6u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c6483c8; /* shll r3 */
L8c6483c8: *stop=0x8c6483c8u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[0],4);if(io_status)return io_status;goto L8c6483ca; /* mov.l r0,@(0x30,r14) */
L8c6483ca: *stop=0x8c6483cau;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6483cc; /* mov.l @(0x4,r14),r2 */
L8c6483cc: *stop=0x8c6483ccu;r[0]=r[2];goto L8c6483ce; /* mov r2,r0 */
L8c6483ce: *stop=0x8c6483ceu;r[2]<<=2;goto L8c6483d0; /* shll2 r2 */
L8c6483d0: *stop=0x8c6483d0u;r[2]+=r[0];goto L8c6483d2; /* add r0,r2 */
L8c6483d2: *stop=0x8c6483d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64849cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6483d4; /* mov.l 0x8c64849c,r0 */
L8c6483d4: *stop=0x8c6483d4u;r[2]<<=2;goto L8c6483d6; /* shll2 r2 */
L8c6483d6: *stop=0x8c6483d6u;r[2]<<=2;goto L8c6483d8; /* shll2 r2 */
L8c6483d8: *stop=0x8c6483d8u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6483da; /* mov.l @(r0,r2),r2 */
L8c6483da: *stop=0x8c6483dau;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[2],4);if(io_status)return io_status;goto L8c6483dc; /* mov.l r2,@(0x34,r14) */
L8c6483dc: *stop=0x8c6483dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6484a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6483de; /* mov.l 0x8c6484a0,r2 */
L8c6483de: *stop=0x8c6483deu;r[3]+=r[2];goto L8c6483e0; /* add r2,r3 */
L8c6483e0: *stop=0x8c6483e0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6483e2; /* mov.l r3,@r15 */
L8c6483e2: *stop=0x8c6483e2u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6483e4; /* mov.l @(0x34,r14),r7 */
L8c6483e4: *stop=0x8c6483e4u;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6483e6; /* mov.l @(0x30,r14),r6 */
L8c6483e6: *stop=0x8c6483e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6484a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6483e8; /* mov.l 0x8c6484a4,r3 */
L8c6483e8: *stop=0x8c6483e8u;r[6]+=1u;goto L8c6483ea; /* add #0x1,r6 */
L8c6483ea: *stop=0x8c6483eau;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[6],4);if(io_status)return io_status;goto L8c6483ec; /* mov.l r6,@(0x30,r14) */
L8c6483ec: *stop=0x8c6483ecu;r[6]+=4294967295u;goto L8c6483ee; /* add #-0x1,r6 */
L8c6483ee: *stop=0x8c6483eeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6483f0; /* mov.l @r15,r5 */
L8c6483f0: *stop=0x8c6483f0u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6483f2; /* mov.l @(0x8,r5),r5 */
L8c6483f2: target=r[3];*pr=0x8c6483f6u;*stop=0x8c6483f4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6483f6; /* jsr @r3 */
L8c6483f6: *stop=0x8c6483f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6484a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6483f8; /* mov.l 0x8c6484a8,r2 */
L8c6483f8: *stop=0x8c6483f8u;r[12]=0u;goto L8c6483fa; /* mov #0x0,r12 */
L8c6483fa: *stop=0x8c6483fau;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6483fc; /* mov.l @(0x1c,r14),r5 */
L8c6483fc: target=r[2];*pr=0x8c648400u;*stop=0x8c6483feu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648400; /* jsr @r2 */
L8c648400: *stop=0x8c648400u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648402; /* cmp/pz r0 */
L8c648402: if((*sr&1u))goto L8c64840c;goto L8c648404; /* bt 0x8c64840c */
L8c648404: *stop=0x8c648404u;r[0]=r[12];goto L8c648406; /* mov r12,r0 */
L8c648406: *stop=0x8c648406u;if(!valid_access(r[14]+9u,1u))return -3;wr(m,bus,&io_status,r[14]+0x9u,r[0],1);if(io_status)return io_status;goto L8c648408; /* mov.b r0,@(0x9,r14) */
L8c648408: *stop=0x8c64840au;if(!valid_access(r[14]+8u,1u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],1);if(io_status)return io_status;goto L8c64842a; /* bra 0x8c64842a */
L8c64840c: *stop=0x8c64840cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6484acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64840e; /* mov.l 0x8c6484ac,r3 */
L8c64840e: target=r[3];*pr=0x8c648412u;*stop=0x8c648410u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648412; /* jsr @r3 */
L8c648412: *stop=0x8c648412u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648414; /* cmp/pz r0 */
L8c648414: if(!(*sr&1u))goto L8c648462;goto L8c648416; /* bf 0x8c648462 */
L8c648416: *stop=0x8c648416u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c648418; /* mov.l @(0x1c,r14),r4 */
L8c648418: *stop=0x8c648418u;r[5]=0u;goto L8c64841a; /* mov #0x0,r5 */
L8c64841a: *stop=0x8c64841au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6484b0u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64841c; /* mov.l 0x8c6484b0,r6 */
L8c64841c: *stop=0x8c64841cu;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64841e; /* mov.l @(0x1c,r4),r2 */
L8c64841e: *stop=0x8c64841eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6484b4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648420; /* mov.l 0x8c6484b4,r3 */
L8c648420: *stop=0x8c648420u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[2],4);if(io_status)return io_status;goto L8c648422; /* mov.l r2,@(0x30,r4) */
L8c648422: target=r[3];*pr=0x8c648426u;*stop=0x8c648424u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648426; /* jsr @r3 */
L8c648426: *stop=0x8c648426u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648428; /* cmp/pz r0 */
L8c648428: if((*sr&1u))goto L8c648430;goto L8c64842a; /* bt 0x8c648430 */
L8c64842a: *stop=0x8c64842au;r[0]=sx(m,bus,&io_status,0x8c648492u,2);if(io_status)return io_status;goto L8c64842c; /* mov.w 0x8c648492,r0 */
L8c64842c: *stop=0x8c64842eu;goto L8c648464; /* bra 0x8c648464 */
L8c648430: *stop=0x8c648430u;r[0]=1u;goto L8c648432; /* mov #0x1,r0 */
L8c648432: *stop=0x8c648432u;if(!valid_access(r[14]+9u,1u))return -3;wr(m,bus,&io_status,r[14]+0x9u,r[0],1);if(io_status)return io_status;goto L8c648434; /* mov.b r0,@(0x9,r14) */
L8c648434: *stop=0x8c648434u;r[0]=r[12];goto L8c648436; /* mov r12,r0 */
L8c648436: *stop=0x8c648436u;if(!valid_access(r[14]+10u,1u))return -3;wr(m,bus,&io_status,r[14]+0xau,r[0],1);if(io_status)return io_status;goto L8c648438; /* mov.b r0,@(0xa,r14) */
L8c648438: *stop=0x8c648438u;if(!valid_access(r[14]+11u,1u))return -3;wr(m,bus,&io_status,r[14]+0xbu,r[0],1);if(io_status)return io_status;goto L8c64843a; /* mov.b r0,@(0xb,r14) */
L8c64843a: *stop=0x8c64843au;if(!valid_access(r[14]+12u,1u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],1);if(io_status)return io_status;goto L8c64843c; /* mov.b r0,@(0xc,r14) */
L8c64843c: *stop=0x8c64843cu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64843e; /* mov.l @(0x4,r14),r0 */
L8c64843e: *stop=0x8c64843eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c648440; /* cmp/eq #0x0,r0 */
L8c648440: if((*sr&1u))goto L8c648452;goto L8c648442; /* bt 0x8c648452 */
L8c648442: *stop=0x8c648442u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c648444; /* cmp/eq #0x1,r0 */
L8c648444: if((*sr&1u))goto L8c648456;goto L8c648446; /* bt 0x8c648456 */
L8c648446: *stop=0x8c648446u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c648448; /* cmp/eq #0x2,r0 */
L8c648448: if((*sr&1u))goto L8c64845a;goto L8c64844a; /* bt 0x8c64845a */
L8c64844a: *stop=0x8c64844au;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c64844c; /* cmp/eq #0x3,r0 */
L8c64844c: if((*sr&1u))goto L8c64845e;goto L8c64844e; /* bt 0x8c64845e */
L8c64844e: *stop=0x8c648450u;goto L8c648462; /* bra 0x8c648462 */
L8c648452: *stop=0x8c648454u;r[0]=15u;goto L8c648464; /* bra 0x8c648464 */
L8c648456: *stop=0x8c648458u;r[0]=33u;goto L8c648464; /* bra 0x8c648464 */
L8c64845a: *stop=0x8c64845cu;r[0]=65u;goto L8c648464; /* bra 0x8c648464 */
L8c64845e: *stop=0x8c648460u;r[0]=122u;goto L8c648464; /* bra 0x8c648464 */
L8c648462: *stop=0x8c648462u;r[0]=4294967171u;goto L8c648464; /* mov #-0x7d,r0 */
L8c648464: *stop=0x8c648464u;r[15]+=4u;goto L8c648466; /* add #0x4,r15 */
L8c648466: *stop=0x8c648466u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c648468; /* lds.l @r15+,PR */
L8c648468: *stop=0x8c648468u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64846a; /* mov.l @r15+,r12 */
L8c64846a: *stop=0x8c64846au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64846c; /* mov.l @r15+,r13 */
L8c64846c: *stop=0x8c64846cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64846eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_decoder_8c64839a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64839a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c6483f6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64839a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6483f6u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c648400(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64839a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648400u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c648412(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64839a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648412u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c648426(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64839a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648426u);}


SA_STAGE_EXPORT int sa_movie_channel_decoder_8c64850a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64850a;
L8c64850a: *stop=0x8c64850au;r[1]=sx(m,bus,&io_status,0x8c6485aeu,2);if(io_status)return io_status;goto L8c64850c; /* mov.w 0x8c6485ae,r1 */
L8c64850c: *stop=0x8c64850cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64850e; /* mov.l 0x8c6485c8,r3 */
L8c64850e: *stop=0x8c64850eu;*macl=r[1]*r[4];goto L8c648510; /* mul.l r1,r4 */
L8c648510: *stop=0x8c648510u;r[1]=*macl;goto L8c648512; /* sts MACL,r1 */
L8c648512: *stop=0x8c648512u;r[1]+=r[3];goto L8c648514; /* add r3,r1 */
L8c648514: *stop=0x8c648514u;if(!valid_access(r[1]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[1]+0x2u,1);if(io_status)return io_status;goto L8c648516; /* mov.b @(0x2,r1),r0 */
L8c648516: *stop=0x8c648516u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648518; /* tst r0,r0 */
L8c648518: if(!(*sr&1u))goto L8c648520;goto L8c64851a; /* bf 0x8c648520 */
L8c64851a: *stop=0x8c64851au;r[0]=sx(m,bus,&io_status,0x8c6485b0u,2);if(io_status)return io_status;goto L8c64851c; /* mov.w 0x8c6485b0,r0 */
L8c64851c: *stop=0x8c64851cu;if(*pr!=entry_pr)return -3;*stop=0x8c64851eu;return 0; /* rts */
L8c648520: *stop=0x8c648520u;r[4]=0u;goto L8c648522; /* mov #0x0,r4 */
L8c648522: *stop=0x8c648522u;r[0]=r[4];goto L8c648524; /* mov r4,r0 */
L8c648524: *stop=0x8c648524u;if(!valid_access(r[1]+9u,1u))return -3;wr(m,bus,&io_status,r[1]+0x9u,r[0],1);if(io_status)return io_status;goto L8c648526; /* mov.b r0,@(0x9,r1) */
L8c648526: *stop=0x8c648526u;if(!valid_access(r[1]+8u,1u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],1);if(io_status)return io_status;goto L8c648528; /* mov.b r0,@(0x8,r1) */
L8c648528: *stop=0x8c648528u;if(!valid_access(r[1]+10u,1u))return -3;wr(m,bus,&io_status,r[1]+0xau,r[0],1);if(io_status)return io_status;goto L8c64852a; /* mov.b r0,@(0xa,r1) */
L8c64852a: *stop=0x8c64852au;if(!valid_access(r[1]+11u,1u))return -3;wr(m,bus,&io_status,r[1]+0xbu,r[0],1);if(io_status)return io_status;goto L8c64852c; /* mov.b r0,@(0xb,r1) */
L8c64852c: *stop=0x8c64852cu;if(!valid_access(r[1]+12u,1u))return -3;wr(m,bus,&io_status,r[1]+0xcu,r[0],1);if(io_status)return io_status;goto L8c64852e; /* mov.b r0,@(0xc,r1) */
L8c64852e: *stop=0x8c64852eu;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c648530; /* mov.l r0,@(0x4,r1) */
L8c648530: *stop=0x8c648530u;if(*pr!=entry_pr)return -3;*stop=0x8c648532u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_channel_decoder_8c647bde(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647bde;
L8c647bde: *stop=0x8c647bdeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c647be0; /* mov.l r14,@-r15 */
L8c647be0: *stop=0x8c647be0u;r[14]=r[4];goto L8c647be2; /* mov r4,r14 */
L8c647be2: *stop=0x8c647be2u;r[14]<<=2;goto L8c647be4; /* shll2 r14 */
L8c647be4: *stop=0x8c647be4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c4cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647be6; /* mov.l 0x8c647c4c,r3 */
L8c647be6: *stop=0x8c647be6u;r[14]<<=2;goto L8c647be8; /* shll2 r14 */
L8c647be8: *stop=0x8c647be8u;r[14]<<=2;goto L8c647bea; /* shll2 r14 */
L8c647bea: *stop=0x8c647beau;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c647bec; /* shll r14 */
L8c647bec: *stop=0x8c647becu;r[0]=120u;goto L8c647bee; /* mov #0x78,r0 */
L8c647bee: *stop=0x8c647beeu;r[14]+=r[3];goto L8c647bf0; /* add r3,r14 */
L8c647bf0: *stop=0x8c647bf0u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c647bf2; /* mov.l @(r0,r14),r2 */
L8c647bf2: *stop=0x8c647bf2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c647bf4; /* tst r2,r2 */
L8c647bf4: if((*sr&1u))goto L8c647bfc;goto L8c647bf6; /* bt 0x8c647bfc */
L8c647bf6: *stop=0x8c647bf6u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c647bf8; /* mov.l @(r0,r14),r2 */
L8c647bf8: target=r[2];*stop=0x8c647bfau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c647bfc: *stop=0x8c647bfcu;if(*pr!=entry_pr)return -3;*stop=0x8c647bfeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

static int sa_movie_channel_io_decoder_run_8c64753c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64756eu)goto L8c64756e;if(resume_pc==0x8c6475bcu)goto L8c6475bc;goto L8c64753c;
L8c64753c: *stop=0x8c64753cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64753e; /* mov.l r14,@-r15 */
L8c64753e: *stop=0x8c64753eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c647540; /* mov.l r13,@-r15 */
L8c647540: *stop=0x8c647540u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c647542; /* mov.l r12,@-r15 */
L8c647542: *stop=0x8c647542u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c647544; /* mov.l r11,@-r15 */
L8c647544: *stop=0x8c647544u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c647546; /* mov.l r10,@-r15 */
L8c647546: *stop=0x8c647546u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c647548; /* mov.l r9,@-r15 */
L8c647548: *stop=0x8c647548u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64754a; /* mov.l r8,@-r15 */
L8c64754a: *stop=0x8c64754au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64754c; /* sts.l PR,@-r15 */
L8c64754c: *stop=0x8c64754cu;r[15]+=4294967288u;goto L8c64754e; /* add #-0x8,r15 */
L8c64754e: *stop=0x8c64754eu;r[12]=sx(m,bus,&io_status,0x8c6475ecu,2);if(io_status)return io_status;goto L8c647550; /* mov.w 0x8c6475ec,r12 */
L8c647550: *stop=0x8c647550u;r[13]=r[4];goto L8c647552; /* mov r4,r13 */
L8c647552: *stop=0x8c647552u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6475f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647554; /* mov.l 0x8c6475f4,r3 */
L8c647554: *stop=0x8c647554u;*macl=r[12]*r[13];goto L8c647556; /* mul.l r12,r13 */
L8c647556: *stop=0x8c647556u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6475f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647558; /* mov.l 0x8c6475f8,r2 */
L8c647558: *stop=0x8c647558u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647600u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64755a; /* mov.l 0x8c647600,r10 */
L8c64755a: *stop=0x8c64755au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6475fcu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c64755c; /* mov.l 0x8c6475fc,r8 */
L8c64755c: *stop=0x8c64755cu;r[12]=*macl;goto L8c64755e; /* sts MACL,r12 */
L8c64755e: *stop=0x8c64755eu;r[11]=sx(m,bus,&io_status,0x8c6475eeu,2);if(io_status)return io_status;goto L8c647560; /* mov.w 0x8c6475ee,r11 */
L8c647560: *stop=0x8c647560u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c647562; /* mov.l r5,@r15 */
L8c647562: *stop=0x8c647562u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[9]=loaded;}goto L8c647564; /* mov.l @r3,r9 */
L8c647564: *stop=0x8c647564u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[12],4);if(io_status)return io_status;goto L8c647566; /* mov.l r12,@(0x4,r15) */
L8c647566: *stop=0x8c647566u;r[12]+=r[2];goto L8c647568; /* add r2,r12 */
L8c647568: *stop=0x8c647568u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64756a; /* mov.l @r15,r5 */
L8c64756a: target=r[10];*pr=0x8c64756eu;*stop=0x8c64756cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64756e; /* jsr @r10 */
L8c64756e: *stop=0x8c64756eu;r[14]=r[0];goto L8c647570; /* mov r0,r14 */
L8c647570: *stop=0x8c647570u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c647572; /* tst r14,r14 */
L8c647572: if((*sr&1u))goto L8c647592;goto L8c647574; /* bt 0x8c647592 */
L8c647574: *stop=0x8c647574u;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[3]=loaded;}goto L8c647576; /* mov.l @r8,r3 */
L8c647576: *stop=0x8c647576u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c647578; /* tst r3,r3 */
L8c647578: if((*sr&1u))goto L8c64757e;goto L8c64757a; /* bt 0x8c64757e */
L8c64757a: *stop=0x8c64757au;*sr=(*sr&~1u)|(r[11]==r[14]);goto L8c64757c; /* cmp/eq r11,r14 */
L8c64757c: if((*sr&1u))goto L8c6475d0;goto L8c64757e; /* bt 0x8c6475d0 */
L8c64757e: *stop=0x8c64757eu;if(!valid_access(r[12]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647580; /* mov.l @(0x34,r12),r3 */
L8c647580: *stop=0x8c647580u;*sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c647582; /* tst r9,r9 */
L8c647582: *stop=0x8c647582u;r[3]+=1u;goto L8c647584; /* add #0x1,r3 */
L8c647584: *stop=0x8c647584u;if(!valid_access(r[12]+52u,4u))return -3;wr(m,bus,&io_status,r[12]+0x34u,r[3],4);if(io_status)return io_status;goto L8c647586; /* mov.l r3,@(0x34,r12) */
L8c647586: take=!(*sr&1u);*stop=0x8c647588u;r[9]+=4294967295u;if(take)goto L8c647568;goto L8c64758a; /* bf/s 0x8c647568 */
L8c64758a: *stop=0x8c64758au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6475f4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64758c; /* mov.l 0x8c6475f4,r1 */
L8c64758c: *stop=0x8c64758cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64758e; /* mov.l @r1,r0 */
L8c64758e: *stop=0x8c64758eu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c647590; /* cmp/eq #-0x1,r0 */
L8c647590: if((*sr&1u))goto L8c647568;goto L8c647592; /* bt 0x8c647568 */
L8c647592: *stop=0x8c647592u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c647594; /* mov.l @(0x4,r15),r12 */
L8c647594: *stop=0x8c647594u;r[2]=r[13];goto L8c647596; /* mov r13,r2 */
L8c647596: *stop=0x8c647596u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6475f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647598; /* mov.l 0x8c6475f8,r3 */
L8c647598: *stop=0x8c647598u;r[2]<<=2;goto L8c64759a; /* shll2 r2 */
L8c64759a: *stop=0x8c64759au;r[2]<<=2;goto L8c64759c; /* shll2 r2 */
L8c64759c: *stop=0x8c64759cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647604u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64759e; /* mov.l 0x8c647604,r1 */
L8c64759e: *stop=0x8c64759eu;r[12]+=r[3];goto L8c6475a0; /* add r3,r12 */
L8c6475a0: *stop=0x8c6475a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647608u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6475a2; /* mov.l 0x8c647608,r3 */
L8c6475a2: *stop=0x8c6475a2u;r[2]<<=2;goto L8c6475a4; /* shll2 r2 */
L8c6475a4: *stop=0x8c6475a4u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6475a6; /* shll r2 */
L8c6475a6: *stop=0x8c6475a6u;r[2]+=r[1];goto L8c6475a8; /* add r1,r2 */
L8c6475a8: *stop=0x8c6475a8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6475aa; /* mov.l r2,@r15 */
L8c6475aa: *stop=0x8c6475aau;if(!valid_access(r[12]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x30u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6475ac; /* mov.l @(0x30,r12),r6 */
L8c6475ac: *stop=0x8c6475acu;if(!valid_access(r[12]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x34u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6475ae; /* mov.l @(0x34,r12),r7 */
L8c6475ae: *stop=0x8c6475aeu;r[6]+=1u;goto L8c6475b0; /* add #0x1,r6 */
L8c6475b0: *stop=0x8c6475b0u;if(!valid_access(r[12]+48u,4u))return -3;wr(m,bus,&io_status,r[12]+0x30u,r[6],4);if(io_status)return io_status;goto L8c6475b2; /* mov.l r6,@(0x30,r12) */
L8c6475b2: *stop=0x8c6475b2u;r[6]+=4294967295u;goto L8c6475b4; /* add #-0x1,r6 */
L8c6475b4: *stop=0x8c6475b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6475b6; /* mov.l @r15,r5 */
L8c6475b6: *stop=0x8c6475b6u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6475b8; /* mov.l @(0x8,r5),r5 */
L8c6475b8: target=r[3];*pr=0x8c6475bcu;*stop=0x8c6475bau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6475bc; /* jsr @r3 */
L8c6475bc: *stop=0x8c6475bcu;r[0]=r[14];goto L8c6475be; /* mov r14,r0 */
L8c6475be: *stop=0x8c6475beu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c6475c0; /* cmp/eq #0x0,r0 */
L8c6475c0: if((*sr&1u))goto L8c6475cc;goto L8c6475c2; /* bt 0x8c6475cc */
L8c6475c2: *stop=0x8c6475c2u;r[1]=sx(m,bus,&io_status,0x8c6475f0u,2);if(io_status)return io_status;goto L8c6475c4; /* mov.w 0x8c6475f0,r1 */
L8c6475c4: *stop=0x8c6475c4u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c6475c6; /* cmp/eq r1,r0 */
L8c6475c6: if((*sr&1u))goto L8c6475d0;goto L8c6475c8; /* bt 0x8c6475d0 */
L8c6475c8: *stop=0x8c6475cau;goto L8c6475d6; /* bra 0x8c6475d6 */
L8c6475cc: *stop=0x8c6475ceu;r[0]=0u;goto L8c6475d8; /* bra 0x8c6475d8 */
L8c6475d0: *stop=0x8c6475d0u;r[0]=sx(m,bus,&io_status,0x8c6475f0u,2);if(io_status)return io_status;goto L8c6475d2; /* mov.w 0x8c6475f0,r0 */
L8c6475d2: *stop=0x8c6475d4u;goto L8c6475d8; /* bra 0x8c6475d8 */
L8c6475d6: *stop=0x8c6475d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64760cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6475d8; /* mov.l 0x8c64760c,r0 */
L8c6475d8: *stop=0x8c6475d8u;r[15]+=8u;goto L8c6475da; /* add #0x8,r15 */
L8c6475da: *stop=0x8c6475dau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6475dc; /* lds.l @r15+,PR */
L8c6475dc: *stop=0x8c6475dcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6475de; /* mov.l @r15+,r8 */
L8c6475de: *stop=0x8c6475deu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6475e0; /* mov.l @r15+,r9 */
L8c6475e0: *stop=0x8c6475e0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6475e2; /* mov.l @r15+,r10 */
L8c6475e2: *stop=0x8c6475e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6475e4; /* mov.l @r15+,r11 */
L8c6475e4: *stop=0x8c6475e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6475e6; /* mov.l @r15+,r12 */
L8c6475e6: *stop=0x8c6475e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6475e8; /* mov.l @r15+,r13 */
L8c6475e8: *stop=0x8c6475e8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6475eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_decoder_8c64753c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64753c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c64756e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64753c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64756eu);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c6475bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64753c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6475bcu);}


SA_STAGE_EXPORT int sa_movie_channel_decoder_8c64b4e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64b4e4;
L8c64b4e4: *stop=0x8c64b4e4u;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b4e6; /* mov.l @(0x1c,r4),r5 */
L8c64b4e6: *stop=0x8c64b4e6u;r[6]=1u;goto L8c64b4e8; /* mov #0x1,r6 */
L8c64b4e8: *stop=0x8c64b4e8u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[7]=loaded;}goto L8c64b4ea; /* mov.l @r5,r7 */
L8c64b4ea: *stop=0x8c64b4eau;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[6]);goto L8c64b4ec; /* cmp/ge r6,r7 */
L8c64b4ec: if(!(*sr&1u))goto L8c64b57a;goto L8c64b4ee; /* bf 0x8c64b57a */
L8c64b4ee: *stop=0x8c64b4eeu;r[2]=sx(m,bus,&io_status,0x8c64b5beu,2);if(io_status)return io_status;goto L8c64b4f0; /* mov.w 0x8c64b5be,r2 */
L8c64b4f0: *stop=0x8c64b4f0u;*sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[2]);goto L8c64b4f2; /* cmp/gt r2,r7 */
L8c64b4f2: if((*sr&1u))goto L8c64b57a;goto L8c64b4f4; /* bt 0x8c64b57a */
L8c64b4f4: *stop=0x8c64b4f4u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b4f6; /* mov.l @r5,r3 */
L8c64b4f6: *stop=0x8c64b4f6u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b4f8; /* mov.l @(0x8,r5),r2 */
L8c64b4f8: *stop=0x8c64b4f8u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c64b4fa; /* cmp/eq r2,r3 */
L8c64b4fa: if(!(*sr&1u))goto L8c64b57a;goto L8c64b4fc; /* bf 0x8c64b57a */
L8c64b4fc: *stop=0x8c64b4fcu;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64b4fe; /* mov.l @(0xc,r5),r7 */
L8c64b4fe: *stop=0x8c64b4feu;*sr=(*sr&~1u)|((int32_t)r[7]>=0);goto L8c64b500; /* cmp/pz r7 */
L8c64b500: if(!(*sr&1u))goto L8c64b57a;goto L8c64b502; /* bf 0x8c64b57a */
L8c64b502: *stop=0x8c64b502u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b504; /* mov.l @r5,r2 */
L8c64b504: *stop=0x8c64b504u;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[2]);goto L8c64b506; /* cmp/ge r2,r7 */
L8c64b506: if((*sr&1u))goto L8c64b57a;goto L8c64b508; /* bt 0x8c64b57a */
L8c64b508: *stop=0x8c64b508u;if(!valid_access(r[5]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x14u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64b50a; /* mov.l @(0x14,r5),r7 */
L8c64b50a: *stop=0x8c64b50au;*sr=(*sr&~1u)|((int32_t)r[7]>=0);goto L8c64b50c; /* cmp/pz r7 */
L8c64b50c: if(!(*sr&1u))goto L8c64b57a;goto L8c64b50e; /* bf 0x8c64b57a */
L8c64b50e: *stop=0x8c64b50eu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b510; /* mov.l @r5,r3 */
L8c64b510: *stop=0x8c64b510u;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[3]);goto L8c64b512; /* cmp/ge r3,r7 */
L8c64b512: if((*sr&1u))goto L8c64b57a;goto L8c64b514; /* bt 0x8c64b57a */
L8c64b514: *stop=0x8c64b514u;if(!valid_access(r[5]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x34u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64b516; /* mov.l @(0x34,r5),r7 */
L8c64b516: *stop=0x8c64b516u;*sr=(*sr&~1u)|((int32_t)r[7]>=0);goto L8c64b518; /* cmp/pz r7 */
L8c64b518: if(!(*sr&1u))goto L8c64b57a;goto L8c64b51a; /* bf 0x8c64b57a */
L8c64b51a: *stop=0x8c64b51au;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b51c; /* mov.l @r5,r2 */
L8c64b51c: *stop=0x8c64b51cu;*sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[2]);goto L8c64b51e; /* cmp/gt r2,r7 */
L8c64b51e: if((*sr&1u))goto L8c64b57a;goto L8c64b520; /* bt 0x8c64b57a */
L8c64b520: *stop=0x8c64b520u;if(!valid_access(r[5]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x38u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64b522; /* mov.l @(0x38,r5),r7 */
L8c64b522: *stop=0x8c64b522u;*sr=(*sr&~1u)|((int32_t)r[7]>=0);goto L8c64b524; /* cmp/pz r7 */
L8c64b524: if(!(*sr&1u))goto L8c64b57a;goto L8c64b526; /* bf 0x8c64b57a */
L8c64b526: *stop=0x8c64b526u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b528; /* mov.l @r5,r3 */
L8c64b528: *stop=0x8c64b528u;*sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[3]);goto L8c64b52a; /* cmp/gt r3,r7 */
L8c64b52a: if((*sr&1u))goto L8c64b57a;goto L8c64b52c; /* bt 0x8c64b57a */
L8c64b52c: *stop=0x8c64b52cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b5ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b52e; /* mov.l 0x8c64b5cc,r2 */
L8c64b52e: *stop=0x8c64b52eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b530; /* mov.l @r2,r3 */
L8c64b530: *stop=0x8c64b530u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b532; /* mov.l @(0x8,r3),r1 */
L8c64b532: *stop=0x8c64b532u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c64b534; /* tst r1,r1 */
L8c64b534: if(!(*sr&1u))goto L8c64b546;goto L8c64b536; /* bf 0x8c64b546 */
L8c64b536: *stop=0x8c64b536u;if(!valid_access(r[5]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b538; /* mov.l @(0x10,r5),r3 */
L8c64b538: *stop=0x8c64b538u;if(!valid_access(r[4]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x10u,2);if(io_status)return io_status;goto L8c64b53a; /* mov.w @(0x10,r4),r0 */
L8c64b53a: *stop=0x8c64b53au;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]);goto L8c64b53c; /* cmp/ge r3,r0 */
L8c64b53c: if(!(*sr&1u))goto L8c64b57a;goto L8c64b53e; /* bf 0x8c64b57a */
L8c64b53e: *stop=0x8c64b53eu;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b540; /* mov.l @(0x18,r5),r3 */
L8c64b540: *stop=0x8c64b540u;if(!valid_access(r[4]+18u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x12u,2);if(io_status)return io_status;goto L8c64b542; /* mov.w @(0x12,r4),r0 */
L8c64b542: *stop=0x8c64b542u;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]);goto L8c64b544; /* cmp/ge r3,r0 */
L8c64b544: if(!(*sr&1u))goto L8c64b57a;goto L8c64b546; /* bf 0x8c64b57a */
L8c64b546: *stop=0x8c64b546u;r[1]=sx(m,bus,&io_status,0x8c64b5c0u,2);if(io_status)return io_status;goto L8c64b548; /* mov.w 0x8c64b5c0,r1 */
L8c64b548: *stop=0x8c64b548u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b54a; /* mov.l @r5,r0 */
L8c64b54a: *stop=0x8c64b54au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64b54c; /* cmp/eq r1,r0 */
L8c64b54c: if((*sr&1u))goto L8c64b564;goto L8c64b54e; /* bt 0x8c64b564 */
L8c64b54e: *stop=0x8c64b54eu;r[1]=sx(m,bus,&io_status,0x8c64b5c2u,2);if(io_status)return io_status;goto L8c64b550; /* mov.w 0x8c64b5c2,r1 */
L8c64b550: *stop=0x8c64b550u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64b552; /* cmp/eq r1,r0 */
L8c64b552: if((*sr&1u))goto L8c64b56a;goto L8c64b554; /* bt 0x8c64b56a */
L8c64b554: *stop=0x8c64b554u;r[1]=sx(m,bus,&io_status,0x8c64b5c4u,2);if(io_status)return io_status;goto L8c64b556; /* mov.w 0x8c64b5c4,r1 */
L8c64b556: *stop=0x8c64b556u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64b558; /* cmp/eq r1,r0 */
L8c64b558: if((*sr&1u))goto L8c64b56e;goto L8c64b55a; /* bt 0x8c64b56e */
L8c64b55a: *stop=0x8c64b55au;r[1]=sx(m,bus,&io_status,0x8c64b5beu,2);if(io_status)return io_status;goto L8c64b55c; /* mov.w 0x8c64b5be,r1 */
L8c64b55c: *stop=0x8c64b55cu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c64b55e; /* cmp/eq r1,r0 */
L8c64b55e: if((*sr&1u))goto L8c64b574;goto L8c64b560; /* bt 0x8c64b574 */
L8c64b560: *stop=0x8c64b562u;goto L8c64b57a; /* bra 0x8c64b57a */
L8c64b564: *stop=0x8c64b564u;r[3]=0u;goto L8c64b566; /* mov #0x0,r3 */
L8c64b566: *stop=0x8c64b568u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[3],4);if(io_status)return io_status;goto L8c64b57e; /* bra 0x8c64b57e */
L8c64b56a: *stop=0x8c64b56cu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[6],4);if(io_status)return io_status;goto L8c64b57e; /* bra 0x8c64b57e */
L8c64b56e: *stop=0x8c64b56eu;r[2]=2u;goto L8c64b570; /* mov #0x2,r2 */
L8c64b570: *stop=0x8c64b572u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[2],4);if(io_status)return io_status;goto L8c64b57e; /* bra 0x8c64b57e */
L8c64b574: *stop=0x8c64b574u;r[1]=3u;goto L8c64b576; /* mov #0x3,r1 */
L8c64b576: *stop=0x8c64b578u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[1],4);if(io_status)return io_status;goto L8c64b57e; /* bra 0x8c64b57e */
L8c64b57a: *stop=0x8c64b57au;if(*pr!=entry_pr)return -3;*stop=0x8c64b57cu;r[0]=4294967295u;return 0; /* rts */
L8c64b57e: *stop=0x8c64b57eu;r[0]=0u;goto L8c64b580; /* mov #0x0,r0 */
L8c64b580: *stop=0x8c64b580u;if(*pr!=entry_pr)return -3;*stop=0x8c64b582u;return 0; /* rts */
}

static int sa_movie_channel_io_decoder_run_8c660a7e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c660aa6u)goto L8c660aa6;if(resume_pc==0x8c660aaeu)goto L8c660aae;if(resume_pc==0x8c660abeu)goto L8c660abe;if(resume_pc==0x8c660af8u)goto L8c660af8;if(resume_pc==0x8c660b0cu)goto L8c660b0c;if(resume_pc==0x8c660b18u)goto L8c660b18;if(resume_pc==0x8c660b26u)goto L8c660b26;if(resume_pc==0x8c660b2eu)goto L8c660b2e;if(resume_pc==0x8c660b38u)goto L8c660b38;if(resume_pc==0x8c660b3eu)goto L8c660b3e;if(resume_pc==0x8c660b7eu)goto L8c660b7e;goto L8c660a7e;
L8c660a7e: *stop=0x8c660a7eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c660a80; /* mov.l r14,@-r15 */
L8c660a80: *stop=0x8c660a80u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c660a82; /* mov.l r13,@-r15 */
L8c660a82: *stop=0x8c660a82u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c660a84; /* mov.l r12,@-r15 */
L8c660a84: *stop=0x8c660a84u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c660a86; /* mov.l r11,@-r15 */
L8c660a86: *stop=0x8c660a86u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c660a88; /* mov.l r10,@-r15 */
L8c660a88: *stop=0x8c660a88u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c660a8a; /* mov.l r9,@-r15 */
L8c660a8a: *stop=0x8c660a8au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c660a8c; /* mov.l r8,@-r15 */
L8c660a8c: *stop=0x8c660a8cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c660a8e; /* sts.l PR,@-r15 */
L8c660a8e: *stop=0x8c660a8eu;r[15]+=4294967284u;goto L8c660a90; /* add #-0xc,r15 */
L8c660a90: *stop=0x8c660a90u;r[11]=sx(m,bus,&io_status,0x8c660ad0u,2);if(io_status)return io_status;goto L8c660a92; /* mov.w 0x8c660ad0,r11 */
L8c660a92: *stop=0x8c660a92u;r[14]=r[4];goto L8c660a94; /* mov r4,r14 */
L8c660a94: *stop=0x8c660a94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660ae4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660a96; /* mov.l 0x8c660ae4,r3 */
L8c660a96: *stop=0x8c660a96u;*macl=r[11]*r[14];goto L8c660a98; /* mul.l r11,r14 */
L8c660a98: *stop=0x8c660a98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660ae8u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c660a9a; /* mov.l 0x8c660ae8,r13 */
L8c660a9a: *stop=0x8c660a9au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c660a9c; /* mov.l r5,@(0x8,r15) */
L8c660a9c: *stop=0x8c660a9cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c660a9e; /* mov.l r6,@r15 */
L8c660a9e: *stop=0x8c660a9eu;r[11]=*macl;goto L8c660aa0; /* sts MACL,r11 */
L8c660aa0: *stop=0x8c660aa0u;r[11]+=r[3];goto L8c660aa2; /* add r3,r11 */
L8c660aa2: target=0x8c660decu;*pr=0x8c660aa6u;*stop=0x8c660aa4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660aa6; /* bsr 0x8c660dec */
L8c660aa6: *stop=0x8c660aa6u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660aa8; /* tst r0,r0 */
L8c660aa8: if(!(*sr&1u))goto L8c660ab2;goto L8c660aaa; /* bf 0x8c660ab2 */
L8c660aaa: target=r[13];*pr=0x8c660aaeu;*stop=0x8c660aacu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660aae; /* jsr @r13 */
L8c660aae: *stop=0x8c660ab0u;goto L8c660b68; /* bra 0x8c660b68 */
L8c660ab2: *stop=0x8c660ab2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660aecu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660ab4; /* mov.l 0x8c660aec,r0 */
L8c660ab4: *stop=0x8c660ab4u;r[14]<<=2;goto L8c660ab6; /* shll2 r14 */
L8c660ab6: *stop=0x8c660ab6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660af0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660ab8; /* mov.l 0x8c660af0,r3 */
L8c660ab8: *stop=0x8c660ab8u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c660aba; /* mov.l @(r0,r14),r14 */
L8c660aba: target=r[3];*pr=0x8c660abeu;*stop=0x8c660abcu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660abe; /* jsr @r3 */
L8c660abe: *stop=0x8c660abeu;r[4]=0u;goto L8c660ac0; /* mov #0x0,r4 */
L8c660ac0: *stop=0x8c660ac0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}goto L8c660ac2; /* mov.l @r15,r8 */
L8c660ac2: *stop=0x8c660ac2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c660ac4; /* mov.l r4,@r15 */
L8c660ac4: *stop=0x8c660ac6u;r[10]=r[4];goto L8c660b8e; /* bra 0x8c660b8e */
L8c660af4: target=r[9];*pr=0x8c660af8u;*stop=0x8c660af6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660af8; /* jsr @r9 */
L8c660af8: *stop=0x8c660af8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660afa; /* tst r0,r0 */
L8c660afa: if(!(*sr&1u))goto L8c660b42;goto L8c660afc; /* bf 0x8c660b42 */
L8c660afc: *stop=0x8c660afcu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660afe; /* mov.l @(0x8,r15),r3 */
L8c660afe: *stop=0x8c660afeu;r[6]=0u;goto L8c660b00; /* mov #0x0,r6 */
L8c660b00: *stop=0x8c660b00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660bf4u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c660b02; /* mov.l 0x8c660bf4,r12 */
L8c660b02: *stop=0x8c660b02u;r[5]=2u;goto L8c660b04; /* mov #0x2,r5 */
L8c660b04: *stop=0x8c660b04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c660b06; /* mov.l r3,@-r15 */
L8c660b06: *stop=0x8c660b06u;r[7]=r[10];goto L8c660b08; /* mov r10,r7 */
L8c660b08: target=r[12];*pr=0x8c660b0cu;*stop=0x8c660b0au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660b0c; /* jsr @r12 */
L8c660b0c: *stop=0x8c660b0cu;r[12]=r[0];goto L8c660b0e; /* mov r0,r12 */
L8c660b0e: *stop=0x8c660b0eu;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c660b10; /* cmp/pz r12 */
L8c660b10: take=(*sr&1u);*stop=0x8c660b12u;r[15]+=4u;if(take)goto L8c660b1e;goto L8c660b14; /* bt/s 0x8c660b1e */
L8c660b14: target=r[13];*pr=0x8c660b18u;*stop=0x8c660b16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660b18; /* jsr @r13 */
L8c660b18: *stop=0x8c660b18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660bf8u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c660b1a; /* mov.l 0x8c660bf8,r9 */
L8c660b1a: *stop=0x8c660b1cu;goto L8c660af4; /* bra 0x8c660af4 */
L8c660b1e: *stop=0x8c660b1eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c660bfcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660b20; /* mov.l 0x8c660bfc,r2 */
L8c660b20: *stop=0x8c660b20u;r[5]=r[12];goto L8c660b22; /* mov r12,r5 */
L8c660b22: target=r[2];*pr=0x8c660b26u;*stop=0x8c660b24u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660b26; /* jsr @r2 */
L8c660b26: *stop=0x8c660b26u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660b28; /* tst r0,r0 */
L8c660b28: if(!(*sr&1u))goto L8c660b32;goto L8c660b2a; /* bf 0x8c660b32 */
L8c660b2a: target=r[13];*pr=0x8c660b2eu;*stop=0x8c660b2cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660b2e; /* jsr @r13 */
L8c660b2e: *stop=0x8c660b30u;goto L8c660b1e; /* bra 0x8c660b1e */
L8c660b32: *stop=0x8c660b32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660c00u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660b34; /* mov.l 0x8c660c00,r3 */
L8c660b34: target=r[3];*pr=0x8c660b38u;*stop=0x8c660b36u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660b38; /* jsr @r3 */
L8c660b38: *stop=0x8c660b38u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c660b3a; /* mov.l r0,@(0x4,r15) */
L8c660b3a: target=r[9];*pr=0x8c660b3eu;*stop=0x8c660b3cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660b3e; /* jsr @r9 */
L8c660b3e: *stop=0x8c660b3eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660b40; /* tst r0,r0 */
L8c660b40: if((*sr&1u))goto L8c660b48;goto L8c660b42; /* bt 0x8c660b48 */
L8c660b42: *stop=0x8c660b42u;r[0]=sx(m,bus,&io_status,0x8c660beau,2);if(io_status)return io_status;goto L8c660b44; /* mov.w 0x8c660bea,r0 */
L8c660b44: *stop=0x8c660b46u;goto L8c660b9a; /* bra 0x8c660b9a */
L8c660b48: *stop=0x8c660b48u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660b4a; /* mov.l @(0x4,r15),r0 */
L8c660b4a: *stop=0x8c660b4au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c660b4c; /* mov.l @(0x4,r15),r9 */
L8c660b4c: *stop=0x8c660b4cu;if(!valid_access(r[0]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x4u,1);if(io_status)return io_status;goto L8c660b4e; /* mov.b @(0x4,r0),r0 */
L8c660b4e: *stop=0x8c660b4eu;r[0]=r[0]&255u;goto L8c660b50; /* extu.b r0,r0 */
L8c660b50: *stop=0x8c660b50u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c660b52; /* cmp/eq #0x8,r0 */
L8c660b52: take=(*sr&1u);*stop=0x8c660b54u;if(!valid_access(r[9]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x8u,4);if(io_status)return io_status;r[9]=loaded;}if(take)goto L8c660b6e;goto L8c660b56; /* bt/s 0x8c660b6e */
L8c660b56: *stop=0x8c660b56u;r[1]=sx(m,bus,&io_status,0x8c660becu,2);if(io_status)return io_status;goto L8c660b58; /* mov.w 0x8c660bec,r1 */
L8c660b58: *stop=0x8c660b58u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660b5a; /* cmp/eq r1,r0 */
L8c660b5a: if((*sr&1u))goto L8c660b18;goto L8c660b5c; /* bt 0x8c660b18 */
L8c660b5c: *stop=0x8c660b5cu;r[1]=sx(m,bus,&io_status,0x8c660beeu,2);if(io_status)return io_status;goto L8c660b5e; /* mov.w 0x8c660bee,r1 */
L8c660b5e: *stop=0x8c660b5eu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660b60; /* cmp/eq r1,r0 */
L8c660b60: if(!(*sr&1u))goto L8c660b68;goto L8c660b62; /* bf 0x8c660b68 */
L8c660b62: *stop=0x8c660b62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660c04u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660b64; /* mov.l 0x8c660c04,r0 */
L8c660b64: *stop=0x8c660b66u;goto L8c660b9a; /* bra 0x8c660b9a */
L8c660b68: *stop=0x8c660b68u;r[0]=sx(m,bus,&io_status,0x8c660bf0u,2);if(io_status)return io_status;goto L8c660b6a; /* mov.w 0x8c660bf0,r0 */
L8c660b6a: *stop=0x8c660b6cu;goto L8c660b9a; /* bra 0x8c660b9a */
L8c660b6e: *stop=0x8c660b6eu;if(!valid_access(r[11]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c660b70; /* mov.l @(0x1c,r11),r6 */
L8c660b70: *stop=0x8c660b70u;r[0]=68u;goto L8c660b72; /* mov #0x44,r0 */
L8c660b72: *stop=0x8c660b72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660c08u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660b74; /* mov.l 0x8c660c08,r3 */
L8c660b74: *stop=0x8c660b74u;r[5]=r[9];goto L8c660b76; /* mov r9,r5 */
L8c660b76: *stop=0x8c660b76u;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c660b78; /* mov.l @(r0,r6),r6 */
L8c660b78: *stop=0x8c660b78u;r[5]+=12u;goto L8c660b7a; /* add #0xc,r5 */
L8c660b7a: target=r[3];*pr=0x8c660b7eu;*stop=0x8c660b7cu;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660b7e; /* jsr @r3 */
L8c660b7e: *stop=0x8c660b7eu;if(!valid_access(r[11]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660b80; /* mov.l @(0x1c,r11),r2 */
L8c660b80: *stop=0x8c660b80u;r[0]=68u;goto L8c660b82; /* mov #0x44,r0 */
L8c660b82: *stop=0x8c660b82u;r[10]+=1u;goto L8c660b84; /* add #0x1,r10 */
L8c660b84: *stop=0x8c660b84u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c660b86; /* mov.l @(r0,r2),r3 */
L8c660b86: *stop=0x8c660b86u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c660b88; /* mov.l @r15,r2 */
L8c660b88: *stop=0x8c660b88u;r[8]+=r[3];goto L8c660b8a; /* add r3,r8 */
L8c660b8a: *stop=0x8c660b8au;r[2]+=1u;goto L8c660b8c; /* add #0x1,r2 */
L8c660b8c: *stop=0x8c660b8cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c660b8e; /* mov.l r2,@r15 */
L8c660b8e: *stop=0x8c660b8eu;if(!valid_access(r[11]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660b90; /* mov.l @(0x1c,r11),r3 */
L8c660b90: *stop=0x8c660b90u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c660b92; /* mov.l @r15,r1 */
L8c660b92: *stop=0x8c660b92u;if(!valid_access(r[3]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660b94; /* mov.l @(0x3c,r3),r2 */
L8c660b94: *stop=0x8c660b94u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[2]);goto L8c660b96; /* cmp/ge r2,r1 */
L8c660b96: if(!(*sr&1u))goto L8c660b18;goto L8c660b98; /* bf 0x8c660b18 */
L8c660b98: *stop=0x8c660b98u;r[0]=0u;goto L8c660b9a; /* mov #0x0,r0 */
L8c660b9a: *stop=0x8c660b9au;r[15]+=12u;goto L8c660b9c; /* add #0xc,r15 */
L8c660b9c: *stop=0x8c660b9cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660b9e; /* lds.l @r15+,PR */
L8c660b9e: *stop=0x8c660b9eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c660ba0; /* mov.l @r15+,r8 */
L8c660ba0: *stop=0x8c660ba0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c660ba2; /* mov.l @r15+,r9 */
L8c660ba2: *stop=0x8c660ba2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c660ba4; /* mov.l @r15+,r10 */
L8c660ba4: *stop=0x8c660ba4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c660ba6; /* mov.l @r15+,r11 */
L8c660ba6: *stop=0x8c660ba6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c660ba8; /* mov.l @r15+,r12 */
L8c660ba8: *stop=0x8c660ba8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c660baa; /* mov.l @r15+,r13 */
L8c660baa: *stop=0x8c660baau;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c660bacu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_decoder_8c660a7e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660aa6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660aa6u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660aae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660aaeu);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660abe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660abeu);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660af8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660af8u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660b0c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660b0cu);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660b18(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660b18u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660b26(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660b26u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660b2e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660b2eu);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660b38(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660b38u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660b3e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660b3eu);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c660b7e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c660a7e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660b7eu);}


static int sa_movie_channel_io_decoder_run_8c648470(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c6484f2u)goto L8c6484f2;goto L8c648470;
L8c648470: *stop=0x8c648470u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c648472; /* mov.l r14,@-r15 */
L8c648472: *stop=0x8c648472u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c648474; /* mov.l r13,@-r15 */
L8c648474: *stop=0x8c648474u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c648476; /* sts.l PR,@-r15 */
L8c648476: *stop=0x8c648476u;r[15]+=4294967292u;goto L8c648478; /* add #-0x4,r15 */
L8c648478: *stop=0x8c648478u;r[14]=sx(m,bus,&io_status,0x8c648490u,2);if(io_status)return io_status;goto L8c64847a; /* mov.w 0x8c648490,r14 */
L8c64847a: *stop=0x8c64847au;r[13]=r[4];goto L8c64847c; /* mov r4,r13 */
L8c64847c: *stop=0x8c64847cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c648494u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64847e; /* mov.l 0x8c648494,r3 */
L8c64847e: *stop=0x8c64847eu;*macl=r[14]*r[13];goto L8c648480; /* mul.l r14,r13 */
L8c648480: *stop=0x8c648480u;r[14]=*macl;goto L8c648482; /* sts MACL,r14 */
L8c648482: *stop=0x8c648482u;r[14]+=r[3];goto L8c648484; /* add r3,r14 */
L8c648484: *stop=0x8c648484u;if(!valid_access(r[14]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,1);if(io_status)return io_status;goto L8c648486; /* mov.b @(0x2,r14),r0 */
L8c648486: *stop=0x8c648486u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648488; /* tst r0,r0 */
L8c648488: if(!(*sr&1u))goto L8c6484b8;goto L8c64848a; /* bf 0x8c6484b8 */
L8c64848a: *stop=0x8c64848au;r[0]=sx(m,bus,&io_status,0x8c648492u,2);if(io_status)return io_status;goto L8c64848c; /* mov.w 0x8c648492,r0 */
L8c64848c: *stop=0x8c64848eu;goto L8c648500; /* bra 0x8c648500 */
L8c6484b8: *stop=0x8c6484b8u;r[3]=1u;goto L8c6484ba; /* mov #0x1,r3 */
L8c6484ba: *stop=0x8c6484bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485b8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6484bc; /* mov.l 0x8c6485b8,r0 */
L8c6484bc: *stop=0x8c6484bcu;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[3],4);if(io_status)return io_status;goto L8c6484be; /* mov.l r3,@(0x30,r14) */
L8c6484be: *stop=0x8c6484beu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6484c0; /* mov.l @(0x4,r14),r2 */
L8c6484c0: *stop=0x8c6484c0u;r[3]=r[2];goto L8c6484c2; /* mov r2,r3 */
L8c6484c2: *stop=0x8c6484c2u;r[2]<<=2;goto L8c6484c4; /* shll2 r2 */
L8c6484c4: *stop=0x8c6484c4u;r[2]+=r[3];goto L8c6484c6; /* add r3,r2 */
L8c6484c6: *stop=0x8c6484c6u;r[3]=r[13];goto L8c6484c8; /* mov r13,r3 */
L8c6484c8: *stop=0x8c6484c8u;r[2]<<=2;goto L8c6484ca; /* shll2 r2 */
L8c6484ca: *stop=0x8c6484cau;r[3]<<=2;goto L8c6484cc; /* shll2 r3 */
L8c6484cc: *stop=0x8c6484ccu;r[2]<<=2;goto L8c6484ce; /* shll2 r2 */
L8c6484ce: *stop=0x8c6484ceu;r[3]<<=2;goto L8c6484d0; /* shll2 r3 */
L8c6484d0: *stop=0x8c6484d0u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6484d2; /* mov.l @(r0,r2),r1 */
L8c6484d2: *stop=0x8c6484d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6484d4; /* mov.l 0x8c6485bc,r2 */
L8c6484d4: *stop=0x8c6484d4u;r[3]<<=2;goto L8c6484d6; /* shll2 r3 */
L8c6484d6: *stop=0x8c6484d6u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c6484d8; /* shll r3 */
L8c6484d8: *stop=0x8c6484d8u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[1],4);if(io_status)return io_status;goto L8c6484da; /* mov.l r1,@(0x34,r14) */
L8c6484da: *stop=0x8c6484dau;r[3]+=r[2];goto L8c6484dc; /* add r2,r3 */
L8c6484dc: *stop=0x8c6484dcu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6484de; /* mov.l r3,@r15 */
L8c6484de: *stop=0x8c6484deu;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6484e0; /* mov.l @(0x30,r14),r6 */
L8c6484e0: *stop=0x8c6484e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6484e2; /* mov.l 0x8c6485c0,r3 */
L8c6484e2: *stop=0x8c6484e2u;r[6]+=1u;goto L8c6484e4; /* add #0x1,r6 */
L8c6484e4: *stop=0x8c6484e4u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6484e6; /* mov.l @(0x34,r14),r7 */
L8c6484e6: *stop=0x8c6484e6u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[6],4);if(io_status)return io_status;goto L8c6484e8; /* mov.l r6,@(0x30,r14) */
L8c6484e8: *stop=0x8c6484e8u;r[6]+=4294967295u;goto L8c6484ea; /* add #-0x1,r6 */
L8c6484ea: *stop=0x8c6484eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6484ec; /* mov.l @r15,r5 */
L8c6484ec: *stop=0x8c6484ecu;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6484ee; /* mov.l @(0x8,r5),r5 */
L8c6484ee: target=r[3];*pr=0x8c6484f2u;*stop=0x8c6484f0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6484f2; /* jsr @r3 */
L8c6484f2: *stop=0x8c6484f2u;r[4]=r[13];goto L8c6484f4; /* mov r13,r4 */
L8c6484f4: *stop=0x8c6484f4u;r[15]+=4u;goto L8c6484f6; /* add #0x4,r15 */
L8c6484f6: *stop=0x8c6484f6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6484f8; /* lds.l @r15+,PR */
L8c6484f8: *stop=0x8c6484f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6485c4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6484fa; /* mov.l 0x8c6485c4,r2 */
L8c6484fa: *stop=0x8c6484fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6484fc; /* mov.l @r15+,r13 */
L8c6484fc: target=r[2];*stop=0x8c6484feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c648500: *stop=0x8c648500u;r[15]+=4u;goto L8c648502; /* add #0x4,r15 */
L8c648502: *stop=0x8c648502u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c648504; /* lds.l @r15+,PR */
L8c648504: *stop=0x8c648504u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c648506; /* mov.l @r15+,r13 */
L8c648506: *stop=0x8c648506u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c648508u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_decoder_8c648470(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c648470(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c6484f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c648470(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6484f2u);}


static int sa_movie_channel_io_decoder_run_8c64b1f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64b21eu)goto L8c64b21e;if(resume_pc==0x8c64b228u)goto L8c64b228;if(resume_pc==0x8c64b236u)goto L8c64b236;if(resume_pc==0x8c64b246u)goto L8c64b246;goto L8c64b1f0;
L8c64b1f0: *stop=0x8c64b1f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b1f2; /* mov.l r14,@-r15 */
L8c64b1f2: *stop=0x8c64b1f2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b1f4; /* mov.l r13,@-r15 */
L8c64b1f4: *stop=0x8c64b1f4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b1f6; /* mov.l r12,@-r15 */
L8c64b1f6: *stop=0x8c64b1f6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b1f8; /* sts.l PR,@-r15 */
L8c64b1f8: *stop=0x8c64b1f8u;r[15]+=4294967292u;goto L8c64b1fa; /* add #-0x4,r15 */
L8c64b1fa: *stop=0x8c64b1fau;r[14]=sx(m,bus,&io_status,0x8c64b2e0u,2);if(io_status)return io_status;goto L8c64b1fc; /* mov.w 0x8c64b2e0,r14 */
L8c64b1fc: *stop=0x8c64b1fcu;r[13]=r[4];goto L8c64b1fe; /* mov r4,r13 */
L8c64b1fe: *stop=0x8c64b1feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b2e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b200; /* mov.l 0x8c64b2e4,r3 */
L8c64b200: *stop=0x8c64b200u;r[2]=r[4];goto L8c64b202; /* mov r4,r2 */
L8c64b202: *stop=0x8c64b202u;*macl=r[14]*r[13];goto L8c64b204; /* mul.l r14,r13 */
L8c64b204: *stop=0x8c64b204u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b2e8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b206; /* mov.l 0x8c64b2e8,r0 */
L8c64b206: *stop=0x8c64b206u;r[2]<<=2;goto L8c64b208; /* shll2 r2 */
L8c64b208: *stop=0x8c64b208u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64b20a; /* mov.l r2,@r15 */
L8c64b20a: *stop=0x8c64b20au;r[14]=*macl;goto L8c64b20c; /* sts MACL,r14 */
L8c64b20c: *stop=0x8c64b20cu;r[14]+=r[3];goto L8c64b20e; /* add r3,r14 */
L8c64b20e: *stop=0x8c64b20eu;r[3]=0u;goto L8c64b210; /* mov #0x0,r3 */
L8c64b210: *stop=0x8c64b210u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[3],4);if(io_status)return io_status;goto L8c64b212; /* mov.l r3,@(r0,r2) */
L8c64b212: *stop=0x8c64b212u;if(!valid_access(r[14]+10u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0xau,1);if(io_status)return io_status;goto L8c64b214; /* mov.b @(0xa,r14),r0 */
L8c64b214: *stop=0x8c64b214u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64b216; /* tst r0,r0 */
L8c64b216: take=!(*sr&1u);*stop=0x8c64b218u;r[12]=1u;if(take)goto L8c64b224;goto L8c64b21a; /* bf/s 0x8c64b224 */
L8c64b21a: target=0x8c64ae32u;*pr=0x8c64b21eu;*stop=0x8c64b21cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b21e; /* bsr 0x8c64ae32 */
L8c64b21e: *stop=0x8c64b21eu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64b220; /* cmp/pz r0 */
L8c64b220: take=!(*sr&1u);*stop=0x8c64b222u;r[4]=r[0];if(take)goto L8c64b25a;goto L8c64b224; /* bf/s 0x8c64b25a */
L8c64b224: target=0x8c64a4b2u;*pr=0x8c64b228u;*stop=0x8c64b226u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b228; /* bsr 0x8c64a4b2 */
L8c64b228: *stop=0x8c64b228u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64b22a; /* cmp/pz r0 */
L8c64b22a: if(!(*sr&1u))goto L8c64b266;goto L8c64b22c; /* bf 0x8c64b266 */
L8c64b22c: *stop=0x8c64b22cu;if(!valid_access(r[14]+11u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0xbu,1);if(io_status)return io_status;goto L8c64b22e; /* mov.b @(0xb,r14),r0 */
L8c64b22e: *stop=0x8c64b22eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64b230; /* tst r0,r0 */
L8c64b230: if(!(*sr&1u))goto L8c64b23c;goto L8c64b232; /* bf 0x8c64b23c */
L8c64b232: target=0x8c64ae62u;*pr=0x8c64b236u;*stop=0x8c64b234u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b236; /* bsr 0x8c64ae62 */
L8c64b236: *stop=0x8c64b236u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64b238; /* cmp/pz r0 */
L8c64b238: take=!(*sr&1u);*stop=0x8c64b23au;r[4]=r[0];if(take)goto L8c64b24c;goto L8c64b23c; /* bf/s 0x8c64b24c */
L8c64b23c: *stop=0x8c64b23cu;if(!valid_access(r[14]+12u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0xcu,1);if(io_status)return io_status;goto L8c64b23e; /* mov.b @(0xc,r14),r0 */
L8c64b23e: *stop=0x8c64b23eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64b240; /* tst r0,r0 */
L8c64b240: if(!(*sr&1u))goto L8c64b25e;goto L8c64b242; /* bf 0x8c64b25e */
L8c64b242: target=0x8c64a9a8u;*pr=0x8c64b246u;*stop=0x8c64b244u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b246; /* bsr 0x8c64a9a8 */
L8c64b246: *stop=0x8c64b246u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64b248; /* cmp/pz r0 */
L8c64b248: take=(*sr&1u);*stop=0x8c64b24au;r[4]=r[0];if(take)goto L8c64b25e;goto L8c64b24c; /* bt/s 0x8c64b25e */
L8c64b24c: *stop=0x8c64b24cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b2ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b24e; /* mov.l 0x8c64b2ec,r3 */
L8c64b24e: *stop=0x8c64b24eu;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c64b250; /* cmp/eq r3,r4 */
L8c64b250: if(!(*sr&1u))goto L8c64b25a;goto L8c64b252; /* bf 0x8c64b25a */
L8c64b252: *stop=0x8c64b252u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b2e8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b254; /* mov.l 0x8c64b2e8,r0 */
L8c64b254: *stop=0x8c64b254u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b256; /* mov.l @r15,r1 */
L8c64b256: *stop=0x8c64b258u;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[12],4);if(io_status)return io_status;goto L8c64b266; /* bra 0x8c64b266 */
L8c64b25a: *stop=0x8c64b25cu;r[0]=r[4];goto L8c64b268; /* bra 0x8c64b268 */
L8c64b25e: *stop=0x8c64b25eu;r[0]=r[12];goto L8c64b260; /* mov r12,r0 */
L8c64b260: *stop=0x8c64b260u;if(!valid_access(r[14]+10u,1u))return -3;wr(m,bus,&io_status,r[14]+0xau,r[0],1);if(io_status)return io_status;goto L8c64b262; /* mov.b r0,@(0xa,r14) */
L8c64b262: *stop=0x8c64b262u;if(!valid_access(r[14]+11u,1u))return -3;wr(m,bus,&io_status,r[14]+0xbu,r[0],1);if(io_status)return io_status;goto L8c64b264; /* mov.b r0,@(0xb,r14) */
L8c64b264: *stop=0x8c64b264u;if(!valid_access(r[14]+12u,1u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],1);if(io_status)return io_status;goto L8c64b266; /* mov.b r0,@(0xc,r14) */
L8c64b266: *stop=0x8c64b266u;r[0]=0u;goto L8c64b268; /* mov #0x0,r0 */
L8c64b268: *stop=0x8c64b268u;r[15]+=4u;goto L8c64b26a; /* add #0x4,r15 */
L8c64b26a: *stop=0x8c64b26au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64b26c; /* lds.l @r15+,PR */
L8c64b26c: *stop=0x8c64b26cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64b26e; /* mov.l @r15+,r12 */
L8c64b26e: *stop=0x8c64b26eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64b270; /* mov.l @r15+,r13 */
L8c64b270: *stop=0x8c64b270u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64b272u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_decoder_8c64b1f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64b1f0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c64b21e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64b1f0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b21eu);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c64b228(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64b1f0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b228u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c64b236(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64b1f0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b236u);}
SA_STAGE_EXPORT int sa_movie_channel_io_decoder_resume_8c64b246(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_decoder_run_8c64b1f0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b246u);}

