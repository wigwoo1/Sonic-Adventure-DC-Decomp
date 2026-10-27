/* Original root-block availability and sixteen-byte signature validation.
 * Existing synchronous status, readiness and cleanup callees remain unchanged.
 * No supplied parser/callback result or task frame.
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
SA_STAGE_EXPORT int sa_movie_filesystem_root_8c64a4b2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64a4b2;
L8c64a4b2: *stop=0x8c64a4b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a4b4; /* mov.l r14,@-r15 */
L8c64a4b4: *stop=0x8c64a4b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64a4b6; /* mov.l r13,@-r15 */
L8c64a4b6: *stop=0x8c64a4b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64a4b8; /* sts.l PR,@-r15 */
L8c64a4b8: *stop=0x8c64a4b8u;r[15]+=4294967292u;goto L8c64a4ba; /* add #-0x4,r15 */
L8c64a4ba: *stop=0x8c64a4bau;r[14]=sx(m,bus,&io_status,0x8c64a53eu,2);if(io_status)return io_status;goto L8c64a4bc; /* mov.w 0x8c64a53e,r14 */
L8c64a4bc: *stop=0x8c64a4bcu;r[13]=r[4];goto L8c64a4be; /* mov r4,r13 */
L8c64a4be: *stop=0x8c64a4beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a548u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a4c0; /* mov.l 0x8c64a548,r3 */
L8c64a4c0: *stop=0x8c64a4c0u;*macl=r[14]*r[13];goto L8c64a4c2; /* mul.l r14,r13 */
L8c64a4c2: *stop=0x8c64a4c2u;r[14]=*macl;goto L8c64a4c4; /* sts MACL,r14 */
L8c64a4c4: *stop=0x8c64a4c4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64a4c6; /* mov.l r14,@r15 */
L8c64a4c6: *stop=0x8c64a4c6u;r[14]+=r[3];goto L8c64a4c8; /* add r3,r14 */
L8c64a4c8: *stop=0x8c64a4c8u;if(!valid_access(r[14]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,1);if(io_status)return io_status;goto L8c64a4ca; /* mov.b @(0x2,r14),r0 */
L8c64a4ca: *stop=0x8c64a4cau;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a4cc; /* tst r0,r0 */
L8c64a4cc: if((*sr&1u))goto L8c64a4f8;goto L8c64a4ce; /* bt 0x8c64a4f8 */
L8c64a4ce: *stop=0x8c64a4ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a54cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a4d0; /* mov.l 0x8c64a54c,r3 */
L8c64a4d0: target=r[3];*pr=0x8c64a4d4u;*stop=0x8c64a4d2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a4d4; /* jsr @r3 */
L8c64a4d4: *stop=0x8c64a4d4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a4d6; /* tst r0,r0 */
L8c64a4d6: if(!(*sr&1u))goto L8c64a4e2;goto L8c64a4d8; /* bf 0x8c64a4e2 */
L8c64a4d8: *stop=0x8c64a4d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a550u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a4da; /* mov.l 0x8c64a550,r3 */
L8c64a4da: target=r[3];*pr=0x8c64a4deu;*stop=0x8c64a4dcu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a4de; /* jsr @r3 */
L8c64a4de: *stop=0x8c64a4e0u;goto L8c64a4f8; /* bra 0x8c64a4f8 */
L8c64a4e2: *stop=0x8c64a4e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a554u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a4e4; /* mov.l 0x8c64a554,r2 */
L8c64a4e4: target=r[2];*pr=0x8c64a4e8u;*stop=0x8c64a4e6u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64a4e8; /* jsr @r2 */
L8c64a4e8: *stop=0x8c64a4e8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a4ea; /* tst r0,r0 */
L8c64a4ea: if((*sr&1u))goto L8c64a4f8;goto L8c64a4ec; /* bt 0x8c64a4f8 */
L8c64a4ec: *stop=0x8c64a4ecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64a4ee; /* mov.l @r15,r2 */
L8c64a4ee: *stop=0x8c64a4eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a548u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a4f0; /* mov.l 0x8c64a548,r3 */
L8c64a4f0: *stop=0x8c64a4f0u;r[2]+=r[3];goto L8c64a4f2; /* add r3,r2 */
L8c64a4f2: *stop=0x8c64a4f2u;if(!valid_access(r[2]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x8u,1);if(io_status)return io_status;goto L8c64a4f4; /* mov.b @(0x8,r2),r0 */
L8c64a4f4: *stop=0x8c64a4f4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64a4f6; /* tst r0,r0 */
L8c64a4f6: if(!(*sr&1u))goto L8c64a4fe;goto L8c64a4f8; /* bf 0x8c64a4fe */
L8c64a4f8: *stop=0x8c64a4f8u;r[0]=sx(m,bus,&io_status,0x8c64a540u,2);if(io_status)return io_status;goto L8c64a4fa; /* mov.w 0x8c64a540,r0 */
L8c64a4fa: *stop=0x8c64a4fcu;goto L8c64a530; /* bra 0x8c64a530 */
L8c64a4fe: *stop=0x8c64a4feu;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64a500; /* mov.l @(0x20,r14),r5 */
L8c64a500: *stop=0x8c64a500u;r[4]=0u;goto L8c64a502; /* mov #0x0,r4 */
L8c64a502: *stop=0x8c64a502u;r[7]=16u;goto L8c64a504; /* mov #0x10,r7 */
L8c64a504: *stop=0x8c64a504u;r[6]=r[4];goto L8c64a506; /* mov r4,r6 */
L8c64a506: *stop=0x8c64a506u;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c64a508; /* mov.b @r5,r0 */
L8c64a508: *stop=0x8c64a508u;r[0]=r[0]&255u;goto L8c64a50a; /* extu.b r0,r0 */
L8c64a50a: *stop=0x8c64a50au;*sr=(*sr&~1u)|(r[0]==0x00000055u);goto L8c64a50c; /* cmp/eq #0x55,r0 */
L8c64a50c: if((*sr&1u))goto L8c64a516;goto L8c64a50e; /* bt 0x8c64a516 */
L8c64a50e: *stop=0x8c64a50eu;r[0]=r[4];goto L8c64a510; /* mov r4,r0 */
L8c64a510: *stop=0x8c64a510u;if(!valid_access(r[14]+11u,1u))return -3;wr(m,bus,&io_status,r[14]+0xbu,r[0],1);if(io_status)return io_status;goto L8c64a512; /* mov.b r0,@(0xb,r14) */
L8c64a512: *stop=0x8c64a514u;if(!valid_access(r[14]+12u,1u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],1);if(io_status)return io_status;goto L8c64a528; /* bra 0x8c64a528 */
L8c64a516: *stop=0x8c64a516u;r[6]+=1u;goto L8c64a518; /* add #0x1,r6 */
L8c64a518: *stop=0x8c64a518u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c64a51a; /* cmp/ge r7,r6 */
L8c64a51a: take=!(*sr&1u);*stop=0x8c64a51cu;r[5]+=1u;if(take)goto L8c64a506;goto L8c64a51e; /* bf/s 0x8c64a506 */
L8c64a51e: *stop=0x8c64a51eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64a558u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64a520; /* mov.l 0x8c64a558,r0 */
L8c64a520: *stop=0x8c64a520u;r[13]<<=2;goto L8c64a522; /* shll2 r13 */
L8c64a522: *stop=0x8c64a522u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64a524; /* mov.l @(r0,r13),r3 */
L8c64a524: *stop=0x8c64a524u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64a526; /* tst r3,r3 */
L8c64a526: if((*sr&1u))goto L8c64a52e;goto L8c64a528; /* bt 0x8c64a52e */
L8c64a528: *stop=0x8c64a528u;r[0]=sx(m,bus,&io_status,0x8c64a53au,2);if(io_status)return io_status;goto L8c64a52a; /* mov.w 0x8c64a53a,r0 */
L8c64a52a: *stop=0x8c64a52cu;goto L8c64a530; /* bra 0x8c64a530 */
L8c64a52e: *stop=0x8c64a52eu;r[0]=0u;goto L8c64a530; /* mov #0x0,r0 */
L8c64a530: *stop=0x8c64a530u;r[15]+=4u;goto L8c64a532; /* add #0x4,r15 */
L8c64a532: *stop=0x8c64a532u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64a534; /* lds.l @r15+,PR */
L8c64a534: *stop=0x8c64a534u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64a536; /* mov.l @r15+,r13 */
L8c64a536: *stop=0x8c64a536u;if(*pr!=entry_pr)return -3;*stop=0x8c64a538u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
