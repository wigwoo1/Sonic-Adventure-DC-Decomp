/* Original movement record callback/offset adjustment and impulse accumulation.
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
static int sa_movement_record_adjustment_run_8c07506e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c07509eu)goto L8c07509e;if(resume_pc==0x8c0750fcu)goto L8c0750fc;goto L8c07506e;
L8c07506e: *stop=0x8c07506eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c075070; /* mov.l r14,@-r15 */
L8c075070: *stop=0x8c075070u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c075072; /* mov.l r13,@-r15 */
L8c075072: *stop=0x8c075072u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c075074; /* mov.l r12,@-r15 */
L8c075074: *stop=0x8c075074u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c075076; /* mov.l r11,@-r15 */
L8c075076: *stop=0x8c075076u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c075078; /* sts.l PR,@-r15 */
L8c075078: *stop=0x8c075078u;r[15]+=4294967280u;goto L8c07507a; /* add #-0x10,r15 */
L8c07507a: *stop=0x8c07507au;r[13]=r[5];goto L8c07507c; /* mov r5,r13 */
L8c07507c: *stop=0x8c07507cu;r[0]=108u;goto L8c07507e; /* mov #0x6c,r0 */
L8c07507e: *stop=0x8c07507eu;r[12]=r[6];goto L8c075080; /* mov r6,r12 */
L8c075080: *stop=0x8c075080u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c075082; /* mov.l r5,@r15 */
L8c075082: *stop=0x8c075082u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[13]=loaded;}goto L8c075084; /* mov.l @(r0,r13),r13 */
L8c075084: *stop=0x8c075084u;r[14]=r[4];goto L8c075086; /* mov r4,r14 */
L8c075086: *stop=0x8c075086u;if(!valid_access(r[13]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x28u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c075088; /* mov.l @(0x28,r13),r13 */
L8c075088: *stop=0x8c075088u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c07508a; /* tst r13,r13 */
L8c07508a: take=(*sr&1u);*stop=0x8c07508cu;r[11]=r[7];if(take)goto L8c075154;goto L8c07508e; /* bt/s 0x8c075154 */
L8c07508e: *stop=0x8c07508eu;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c075090; /* mov.b @(0x8,r14),r0 */
L8c075090: *stop=0x8c075090u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c075092; /* tst r0,r0 */
L8c075092: if((*sr&1u))goto L8c075096;goto L8c075094; /* bt 0x8c075096 */
L8c075094: *stop=0x8c075094u;r[13]+=28u;goto L8c075096; /* add #0x1c,r13 */
L8c075096: *stop=0x8c075096u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0751a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075098; /* mov.l 0x8c0751a0,r3 */
L8c075098: *stop=0x8c075098u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c07509a; /* mov.l @r15,r4 */
L8c07509a: target=r[3];*pr=0x8c07509eu;*stop=0x8c07509cu;r[4]+=56u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07509e; /* jsr @r3 */
L8c07509e: *stop=0x8c07509eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0750a0; /* fldi0 fr3 */
L8c0750a0: *stop=0x8c0750a0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c0750a2; /* fcmp/eq fr3,fr0 */
L8c0750a2: if(!(*sr&1u))goto L8c0750c8;goto L8c0750a4; /* bf 0x8c0750c8 */
L8c0750a4: *stop=0x8c0750a4u;r[0]=32u;goto L8c0750a6; /* mov #0x20,r0 */
L8c0750a6: *stop=0x8c0750a6u;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c0750a8; /* fmov.s @r11,fr2 */
L8c0750a8: *stop=0x8c0750a8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c0750aa; /* fmov.s @(r0,r14),fr1 */
L8c0750aa: *stop=0x8c0750aau;r[0]=4u;goto L8c0750ac; /* mov #0x4,r0 */
L8c0750ac: *stop=0x8c0750acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c0750ae; /* fadd fr2,fr1 */
L8c0750ae: *stop=0x8c0750aeu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c0750b0; /* fmov fr1,@(r0,r12) */
L8c0750b0: *stop=0x8c0750b0u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c0750b2; /* fmov.s @(r0,r11),fr2 */
L8c0750b2: *stop=0x8c0750b2u;r[0]=36u;goto L8c0750b4; /* mov #0x24,r0 */
L8c0750b4: *stop=0x8c0750b4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c0750b6; /* fmov.s @(r0,r14),fr1 */
L8c0750b6: *stop=0x8c0750b6u;r[0]=8u;goto L8c0750b8; /* mov #0x8,r0 */
L8c0750b8: *stop=0x8c0750b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c0750ba; /* fadd fr2,fr1 */
L8c0750ba: *stop=0x8c0750bau;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c0750bc; /* fmov fr1,@(r0,r12) */
L8c0750bc: *stop=0x8c0750bcu;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c0750be; /* fmov.s @(r0,r11),fr2 */
L8c0750be: *stop=0x8c0750beu;r[0]=40u;goto L8c0750c0; /* mov #0x28,r0 */
L8c0750c0: *stop=0x8c0750c0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c0750c2; /* fmov.s @(r0,r14),fr1 */
L8c0750c2: *stop=0x8c0750c2u;r[0]=12u;goto L8c0750c4; /* mov #0xc,r0 */
L8c0750c4: *stop=0x8c0750c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c0750c6; /* fadd fr2,fr1 */
L8c0750c6: *stop=0x8c0750c6u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c0750c8; /* fmov fr1,@(r0,r12) */
L8c0750c8: *stop=0x8c0750c8u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0750ca; /* mov.l @r13,r3 */
L8c0750ca: *stop=0x8c0750cau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0750cc; /* tst r3,r3 */
L8c0750cc: if((*sr&1u))goto L8c07510c;goto L8c0750ce; /* bt 0x8c07510c */
L8c0750ce: *stop=0x8c0750ceu;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0750d0; /* mov.l @(0x20,r14),r2 */
L8c0750d0: *stop=0x8c0750d0u;r[3]=r[15];goto L8c0750d2; /* mov r15,r3 */
L8c0750d2: *stop=0x8c0750d2u;r[3]+=4u;goto L8c0750d4; /* add #0x4,r3 */
L8c0750d4: *stop=0x8c0750d4u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0750d6; /* mov.l @(0x24,r14),r1 */
L8c0750d6: *stop=0x8c0750d6u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c0750d8; /* mov.l r2,@r3 */
L8c0750d8: *stop=0x8c0750d8u;r[6]=r[13];goto L8c0750da; /* mov r13,r6 */
L8c0750da: *stop=0x8c0750dau;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c0750dc; /* mov.l r1,@(0x4,r3) */
L8c0750dc: *stop=0x8c0750dcu;r[5]=r[14];goto L8c0750de; /* mov r14,r5 */
L8c0750de: *stop=0x8c0750deu;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0750e0; /* mov.l @(0x28,r14),r2 */
L8c0750e0: *stop=0x8c0750e0u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[2],4);if(io_status)return io_status;goto L8c0750e2; /* mov.l r2,@(0x8,r3) */
L8c0750e2: *stop=0x8c0750e2u;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0750e4; /* mov.l @(0x4,r12),r0 */
L8c0750e4: *stop=0x8c0750e4u;if(!valid_access(r[12]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0750e6; /* mov.l @(0x8,r12),r3 */
L8c0750e6: *stop=0x8c0750e6u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[0],4);if(io_status)return io_status;goto L8c0750e8; /* mov.l r0,@(0x20,r14) */
L8c0750e8: *stop=0x8c0750e8u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[3],4);if(io_status)return io_status;goto L8c0750ea; /* mov.l r3,@(0x24,r14) */
L8c0750ea: *stop=0x8c0750eau;if(!valid_access(r[12]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0750ec; /* mov.l @(0xc,r12),r0 */
L8c0750ec: *stop=0x8c0750ecu;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[0],4);if(io_status)return io_status;goto L8c0750ee; /* mov.l r0,@(0x28,r14) */
L8c0750ee: *stop=0x8c0750eeu;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c0750f0; /* mov.b @(0x8,r14),r0 */
L8c0750f0: *stop=0x8c0750f0u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0750f2; /* mov.l @r13,r2 */
L8c0750f2: *stop=0x8c0750f2u;r[4]=r[0]&255u;goto L8c0750f4; /* extu.b r0,r4 */
L8c0750f4: *stop=0x8c0750f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0751a4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0750f6; /* mov.l 0x8c0751a4,r0 */
L8c0750f6: *stop=0x8c0750f6u;r[4]<<=2;goto L8c0750f8; /* shll2 r4 */
L8c0750f8: target=r[2];*pr=0x8c0750fcu;*stop=0x8c0750fau;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0750fc; /* jsr @r2 */
L8c0750fc: *stop=0x8c0750fcu;r[3]=r[15];goto L8c0750fe; /* mov r15,r3 */
L8c0750fe: *stop=0x8c0750feu;r[3]+=4u;goto L8c075100; /* add #0x4,r3 */
L8c075100: *stop=0x8c075100u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c075102; /* mov.l @r3,r2 */
L8c075102: *stop=0x8c075102u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075104; /* mov.l @(0x4,r3),r1 */
L8c075104: *stop=0x8c075104u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[2],4);if(io_status)return io_status;goto L8c075106; /* mov.l r2,@(0x20,r14) */
L8c075106: *stop=0x8c075106u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[1],4);if(io_status)return io_status;goto L8c075108; /* mov.l r1,@(0x24,r14) */
L8c075108: *stop=0x8c075108u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07510a; /* mov.l @(0x8,r3),r2 */
L8c07510a: *stop=0x8c07510au;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[2],4);if(io_status)return io_status;goto L8c07510c; /* mov.l r2,@(0x28,r14) */
L8c07510c: *stop=0x8c07510cu;r[1]=16u;goto L8c07510e; /* mov #0x10,r1 */
L8c07510e: *stop=0x8c07510eu;r[1]+=r[13];goto L8c075110; /* add r13,r1 */
L8c075110: *stop=0x8c075110u;r[0]=4u;goto L8c075112; /* mov #0x4,r0 */
L8c075112: *stop=0x8c075112u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c075114; /* fmov.s @r1,fr3 */
L8c075114: *stop=0x8c075114u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c075116; /* fmov.s @(r0,r12),fr2 */
L8c075116: *stop=0x8c075116u;r[1]=20u;goto L8c075118; /* mov #0x14,r1 */
L8c075118: *stop=0x8c075118u;r[1]+=r[13];goto L8c07511a; /* add r13,r1 */
L8c07511a: *stop=0x8c07511au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07511c; /* fadd fr3,fr2 */
L8c07511c: *stop=0x8c07511cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c07511e; /* fmov fr2,@(r0,r12) */
L8c07511e: *stop=0x8c07511eu;r[0]=8u;goto L8c075120; /* mov #0x8,r0 */
L8c075120: *stop=0x8c075120u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c075122; /* fmov.s @(r0,r12),fr2 */
L8c075122: *stop=0x8c075122u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c075124; /* fmov.s @r1,fr3 */
L8c075124: *stop=0x8c075124u;r[1]=24u;goto L8c075126; /* mov #0x18,r1 */
L8c075126: *stop=0x8c075126u;r[1]+=r[13];goto L8c075128; /* add r13,r1 */
L8c075128: *stop=0x8c075128u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07512a; /* fadd fr3,fr2 */
L8c07512a: *stop=0x8c07512au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c07512c; /* fmov fr2,@(r0,r12) */
L8c07512c: *stop=0x8c07512cu;r[0]=12u;goto L8c07512e; /* mov #0xc,r0 */
L8c07512e: *stop=0x8c07512eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c075130; /* fmov.s @r1,fr3 */
L8c075130: *stop=0x8c075130u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c075132; /* fmov.s @(r0,r12),fr2 */
L8c075132: *stop=0x8c075132u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c075134; /* fadd fr3,fr2 */
L8c075134: *stop=0x8c075134u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c075136; /* fmov fr2,@(r0,r12) */
L8c075136: *stop=0x8c075136u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075138; /* mov.l @(0x14,r14),r3 */
L8c075138: *stop=0x8c075138u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07513a; /* mov.l @(0x4,r13),r2 */
L8c07513a: *stop=0x8c07513au;r[3]+=r[2];goto L8c07513c; /* add r2,r3 */
L8c07513c: *stop=0x8c07513cu;r[3]=r[3]&65535u;goto L8c07513e; /* extu.w r3,r3 */
L8c07513e: *stop=0x8c07513eu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[3],4);if(io_status)return io_status;goto L8c075140; /* mov.l r3,@(0x14,r14) */
L8c075140: *stop=0x8c075140u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075142; /* mov.l @(0x18,r14),r1 */
L8c075142: *stop=0x8c075142u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075144; /* mov.l @(0x8,r13),r3 */
L8c075144: *stop=0x8c075144u;r[1]+=r[3];goto L8c075146; /* add r3,r1 */
L8c075146: *stop=0x8c075146u;r[1]=r[1]&65535u;goto L8c075148; /* extu.w r1,r1 */
L8c075148: *stop=0x8c075148u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[1],4);if(io_status)return io_status;goto L8c07514a; /* mov.l r1,@(0x18,r14) */
L8c07514a: *stop=0x8c07514au;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07514c; /* mov.l @(0x1c,r14),r2 */
L8c07514c: *stop=0x8c07514cu;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07514e; /* mov.l @(0xc,r13),r3 */
L8c07514e: *stop=0x8c07514eu;r[2]+=r[3];goto L8c075150; /* add r3,r2 */
L8c075150: *stop=0x8c075150u;r[2]=r[2]&65535u;goto L8c075152; /* extu.w r2,r2 */
L8c075152: *stop=0x8c075152u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c075154; /* mov.l r2,@(0x1c,r14) */
L8c075154: *stop=0x8c075154u;r[15]+=16u;goto L8c075156; /* add #0x10,r15 */
L8c075156: *stop=0x8c075156u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c075158; /* lds.l @r15+,PR */
L8c075158: *stop=0x8c075158u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c07515a; /* mov.l @r15+,r11 */
L8c07515a: *stop=0x8c07515au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c07515c; /* mov.l @r15+,r12 */
L8c07515c: *stop=0x8c07515cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c07515e; /* mov.l @r15+,r13 */
L8c07515e: *stop=0x8c07515eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c075160u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_record_adjustment_8c07506e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_record_adjustment_run_8c07506e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_record_adjustment_resume_8c07509e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_record_adjustment_run_8c07506e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07509eu);}
SA_STAGE_EXPORT int sa_movement_record_adjustment_resume_8c0750fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_record_adjustment_run_8c07506e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0750fcu);}

SA_STAGE_EXPORT int sa_movement_record_adjustment_8c0751c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0751c0;
L8c0751c0: *stop=0x8c0751c0u;r[0]=0x8c0753a0u;goto L8c0751c2; /* mova 0x8c0753a0,r0 */
L8c0751c2: *stop=0x8c0751c2u;r[1]=60u;goto L8c0751c4; /* mov #0x3c,r1 */
L8c0751c4: *stop=0x8c0751c4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c0751c6; /* fmov.s @r0,fr4 */
L8c0751c6: *stop=0x8c0751c6u;r[1]+=r[6];goto L8c0751c8; /* add r6,r1 */
L8c0751c8: *stop=0x8c0751c8u;r[0]=32u;goto L8c0751ca; /* mov #0x20,r0 */
L8c0751ca: *stop=0x8c0751cau;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0751cc; /* fmov.s @r1,fr3 */
L8c0751cc: *stop=0x8c0751ccu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c0751ce; /* fmov.s @(r0,r4),fr2 */
L8c0751ce: *stop=0x8c0751ceu;r[1]=64u;goto L8c0751d0; /* mov #0x40,r1 */
L8c0751d0: *stop=0x8c0751d0u;fcopy(f,0u,4u);goto L8c0751d2; /* fmov fr4,fr0 */
L8c0751d2: *stop=0x8c0751d2u;r[1]+=r[6];goto L8c0751d4; /* add r6,r1 */
L8c0751d4: *stop=0x8c0751d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0751d6; /* fmac fr0,fr3,fr2 */
L8c0751d6: *stop=0x8c0751d6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c0751d8; /* fldi1 fr4 */
L8c0751d8: *stop=0x8c0751d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[4],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0751da; /* fadd fr4,fr4 */
L8c0751da: *stop=0x8c0751dau;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c0751dc; /* fmov fr2,@(r0,r4) */
L8c0751dc: *stop=0x8c0751dcu;r[0]=36u;goto L8c0751de; /* mov #0x24,r0 */
L8c0751de: *stop=0x8c0751deu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c0751e0; /* fmov.s @(r0,r4),fr2 */
L8c0751e0: *stop=0x8c0751e0u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0751e2; /* fmov.s @r1,fr3 */
L8c0751e2: *stop=0x8c0751e2u;r[1]=68u;goto L8c0751e4; /* mov #0x44,r1 */
L8c0751e4: *stop=0x8c0751e4u;r[1]+=r[6];goto L8c0751e6; /* add r6,r1 */
L8c0751e6: *stop=0x8c0751e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0751e8; /* fmac fr0,fr3,fr2 */
L8c0751e8: *stop=0x8c0751e8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c0751ea; /* fmov fr2,@(r0,r4) */
L8c0751ea: *stop=0x8c0751eau;r[0]=40u;goto L8c0751ec; /* mov #0x28,r0 */
L8c0751ec: *stop=0x8c0751ecu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c0751ee; /* fmov.s @(r0,r4),fr2 */
L8c0751ee: *stop=0x8c0751eeu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0751f0; /* fmov.s @r1,fr3 */
L8c0751f0: *stop=0x8c0751f0u;r[1]=60u;goto L8c0751f2; /* mov #0x3c,r1 */
L8c0751f2: *stop=0x8c0751f2u;r[1]+=r[6];goto L8c0751f4; /* add r6,r1 */
L8c0751f4: *stop=0x8c0751f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0751f6; /* fmac fr0,fr3,fr2 */
L8c0751f6: *stop=0x8c0751f6u;fcopy(f,0u,4u);goto L8c0751f8; /* fmov fr4,fr0 */
L8c0751f8: *stop=0x8c0751f8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c0751fa; /* fmov fr2,@(r0,r4) */
L8c0751fa: *stop=0x8c0751fau;r[0]=4u;goto L8c0751fc; /* mov #0x4,r0 */
L8c0751fc: *stop=0x8c0751fcu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c0751fe; /* fmov.s @(r0,r5),fr2 */
L8c0751fe: *stop=0x8c0751feu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c075200; /* fmov.s @r1,fr3 */
L8c075200: *stop=0x8c075200u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c075202; /* fmac fr0,fr3,fr2 */
L8c075202: *stop=0x8c075202u;r[1]=68u;goto L8c075204; /* mov #0x44,r1 */
L8c075204: *stop=0x8c075204u;r[1]+=r[6];goto L8c075206; /* add r6,r1 */
L8c075206: *stop=0x8c075206u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c075208; /* fmov fr2,@(r0,r5) */
L8c075208: *stop=0x8c075208u;r[0]=12u;goto L8c07520a; /* mov #0xc,r0 */
L8c07520a: *stop=0x8c07520au;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c07520c; /* fmov.s @(r0,r5),fr2 */
L8c07520c: *stop=0x8c07520cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c07520e; /* fmov.s @r1,fr3 */
L8c07520e: *stop=0x8c07520eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c075210; /* fmac fr0,fr3,fr2 */
L8c075210: *stop=0x8c075210u;if(*pr!=entry_pr)return -3;*stop=0x8c075212u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;return 0; /* rts */
}
