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
 if(n==4u && ((a&0x1fffffffu)==0x00710000u || (a&0x1fffffffu)==0x00710004u))return 1;
 if((n==1u||n==2u||n==4u) && (a&0x1fffffffu)>=0x00702c00u && (a&0x1fffffffu)+n<=0x00702c04u)return 1;
 if(n==4u && (a==0xff00001cu||a==0xffa00040u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f6800u && (a&0x1fffffffu)<0x005f7d00u)return 1;
 if(n==4u && !(a&31u) && a>=0xf4000000u && a<0xf4004000u)return 1;
 if(n==2u && (a==0xffd00000u||a==0xffd00004u||a==0xffd00008u||a==0xffd0000cu||a==0xffd80010u))return 1;
 if(n==1u && (a==0xffd80000u||a==0xffd80004u))return 1;
 if(n==4u && (a==0xffd80008u||a==0xffd8000cu))return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_disc_paths_8c64563e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64563e;
L8c64563e: *stop=0x8c64563eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645640; /* mov.l r14,@-r15 */
L8c645640: *stop=0x8c645640u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c645642; /* mov.l r13,@-r15 */
L8c645642: *stop=0x8c645642u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c645644; /* mov.l r12,@-r15 */
L8c645644: *stop=0x8c645644u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c645646; /* mov.l r11,@-r15 */
L8c645646: *stop=0x8c645646u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c645648; /* mov.l r10,@-r15 */
L8c645648: *stop=0x8c645648u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64564a; /* mov.l r9,@-r15 */
L8c64564a: *stop=0x8c64564au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64564c; /* mov.l r8,@-r15 */
L8c64564c: *stop=0x8c64564cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64564e; /* sts.l PR,@-r15 */
L8c64564e: *stop=0x8c64564eu;r[15]+=4294967272u;goto L8c645650; /* add #-0x18,r15 */
L8c645650: *stop=0x8c645650u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645766u,2);if(io_status)return io_status;r[14]=loaded;}goto L8c645652; /* mov.w 0x8c645766,r14 */
L8c645652: *stop=0x8c645652u;r[9]=r[4];goto L8c645654; /* mov r4,r9 */
L8c645654: *stop=0x8c645654u;r[4]=0u;goto L8c645656; /* mov #0x0,r4 */
L8c645656: *stop=0x8c645656u;r[12]=r[7];goto L8c645658; /* mov r7,r12 */
L8c645658: *stop=0x8c645658u;r[14]+=r[9];goto L8c64565a; /* add r9,r14 */
L8c64565a: *stop=0x8c64565au;r[10]=r[4];goto L8c64565c; /* mov r4,r10 */
L8c64565c: *stop=0x8c64565cu;r[8]=1u;goto L8c64565e; /* mov #0x1,r8 */
L8c64565e: *stop=0x8c64565eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c645660; /* mov.l r5,@(0x8,r15) */
L8c645660: *stop=0x8c645660u;r[12]+=1u;goto L8c645662; /* add #0x1,r12 */
L8c645662: *stop=0x8c645662u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c645664; /* mov.l r6,@(0x10,r15) */
L8c645664: *stop=0x8c645664u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[8],4);if(io_status)return io_status;goto L8c645666; /* mov.l r8,@(0x14,r15) */
L8c645666: *stop=0x8c645666u;r[11]=r[8];goto L8c645668; /* mov r8,r11 */
L8c645668: *stop=0x8c645668u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;goto L8c64566a; /* mov.l r4,@(0xc,r15) */
L8c64566a: *stop=0x8c64566au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64566c; /* mov.l r14,@r15 */
L8c64566c: *stop=0x8c64566cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64566e; /* mov.l @r15,r2 */
L8c64566e: *stop=0x8c64566eu;*sr=(*sr&~1u)|(r[14]>=r[2]);goto L8c645670; /* cmp/hs r2,r14 */
L8c645670: take=(*sr&1u);*stop=0x8c645672u;r[13]=r[14];if(take)goto L8c64568a;goto L8c645674; /* bt/s 0x8c64568a */
L8c645674: *stop=0x8c645674u;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[0]=loaded;}goto L8c645676; /* mov.b @r13,r0 */
L8c645676: *stop=0x8c645676u;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[1]=loaded;}goto L8c645678; /* mov.b @r13,r1 */
L8c645678: *stop=0x8c645678u;r[0]=r[0]&255u;goto L8c64567a; /* extu.b r0,r0 */
L8c64567a: *stop=0x8c64567au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64567c; /* mov.l @r15,r3 */
L8c64567c: *stop=0x8c64567cu;r[0]&=0x1u;goto L8c64567e; /* and #0x1,r0 */
L8c64567e: *stop=0x8c64567eu;r[1]=r[1]&255u;goto L8c645680; /* extu.b r1,r1 */
L8c645680: *stop=0x8c645680u;r[1]+=r[0];goto L8c645682; /* add r0,r1 */
L8c645682: *stop=0x8c645682u;r[1]+=8u;goto L8c645684; /* add #0x8,r1 */
L8c645684: *stop=0x8c645684u;r[1]+=r[14];goto L8c645686; /* add r14,r1 */
L8c645686: *stop=0x8c645686u;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c645688; /* cmp/hs r3,r1 */
L8c645688: if(!(*sr&1u))goto L8c6456b4;goto L8c64568a; /* bf 0x8c6456b4 */
L8c64568a: *stop=0x8c64568au;{uint32_t loaded=sx(m,bus,&io_status,0x8c645766u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c64568c; /* mov.w 0x8c645766,r3 */
L8c64568c: *stop=0x8c64568cu;r[6]=r[9];goto L8c64568e; /* mov r9,r6 */
L8c64568e: *stop=0x8c64568eu;r[6]-=r[14];goto L8c645690; /* sub r14,r6 */
L8c645690: *stop=0x8c645690u;r[13]=r[9];goto L8c645692; /* mov r9,r13 */
L8c645692: *stop=0x8c645692u;r[6]+=r[3];goto L8c645694; /* add r3,r6 */
L8c645694: *stop=0x8c645694u;r[13]-=r[6];goto L8c645696; /* sub r6,r13 */
L8c645696: *stop=0x8c645696u;r[5]=r[13];goto L8c645698; /* mov r13,r5 */
L8c645698: target=0x8c64535cu;*pr=0x8c64569cu;*stop=0x8c64569au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64569c; /* bsr 0x8c64535c */
L8c64569c: *stop=0x8c64569cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645768u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64569e; /* mov.l 0x8c645768,r3 */
L8c64569e: *stop=0x8c64569eu;r[5]=1u;goto L8c6456a0; /* mov #0x1,r5 */
L8c6456a0: *stop=0x8c6456a0u;r[6]=r[9];goto L8c6456a2; /* mov r9,r6 */
L8c6456a2: target=r[3];*pr=0x8c6456a6u;*stop=0x8c6456a4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6456a6; /* jsr @r3 */
L8c6456a6: *stop=0x8c6456a6u;r[4]=r[0];goto L8c6456a8; /* mov r0,r4 */
L8c6456a8: *stop=0x8c6456a8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6456aa; /* tst r4,r4 */
L8c6456aa: if(!(*sr&1u))goto L8c645728;goto L8c6456ac; /* bf 0x8c645728 */
L8c6456ac: *stop=0x8c6456acu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6456ae; /* mov.l @(0x8,r15),r2 */
L8c6456ae: *stop=0x8c6456aeu;r[14]=r[13];goto L8c6456b0; /* mov r13,r14 */
L8c6456b0: *stop=0x8c6456b0u;r[2]+=1u;goto L8c6456b2; /* add #0x1,r2 */
L8c6456b2: *stop=0x8c6456b2u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6456b4; /* mov.l r2,@(0x8,r15) */
L8c6456b4: *stop=0x8c6456b4u;r[5]=r[15];goto L8c6456b6; /* mov r15,r5 */
L8c6456b6: *stop=0x8c6456b6u;r[4]=r[13];goto L8c6456b8; /* mov r13,r4 */
L8c6456b8: *stop=0x8c6456b8u;r[5]+=4u;goto L8c6456ba; /* add #0x4,r5 */
L8c6456ba: *stop=0x8c6456bau;r[6]=2u;goto L8c6456bc; /* mov #0x2,r6 */
L8c6456bc: target=0x8c64535cu;*pr=0x8c6456c0u;*stop=0x8c6456beu;r[4]+=6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6456c0; /* bsr 0x8c64535c */
L8c6456c0: *stop=0x8c6456c0u;if(!valid_access(r[15]+4u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6456c2; /* mov.w @(0x4,r15),r0 */
L8c6456c2: *stop=0x8c6456c2u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6456c4; /* mov.l @(0x14,r15),r3 */
L8c6456c4: *stop=0x8c6456c4u;r[0]=r[0]&65535u;goto L8c6456c6; /* extu.w r0,r0 */
L8c6456c6: *stop=0x8c6456c6u;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c6456c8; /* cmp/eq r3,r0 */
L8c6456c8: if(!(*sr&1u))goto L8c64570c;goto L8c6456ca; /* bf 0x8c64570c */
L8c6456ca: *stop=0x8c6456cau;r[0]=r[11];goto L8c6456cc; /* mov r11,r0 */
L8c6456cc: *stop=0x8c6456ccu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6456ce; /* cmp/eq #0x1,r0 */
L8c6456ce: if((*sr&1u))goto L8c6456e6;goto L8c6456d0; /* bt 0x8c6456e6 */
L8c6456d0: *stop=0x8c6456d0u;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[2]=loaded;}goto L8c6456d2; /* mov.b @r13,r2 */
L8c6456d2: *stop=0x8c6456d2u;r[2]=r[2]&255u;goto L8c6456d4; /* extu.b r2,r2 */
L8c6456d4: *stop=0x8c6456d4u;*sr=(*sr&~1u)|(r[10]==r[2]);goto L8c6456d6; /* cmp/eq r10,r2 */
L8c6456d6: if(!(*sr&1u))goto L8c64570c;goto L8c6456d8; /* bf 0x8c64570c */
L8c6456d8: *stop=0x8c6456d8u;r[4]=r[13];goto L8c6456da; /* mov r13,r4 */
L8c6456da: *stop=0x8c6456dau;r[5]=r[12];goto L8c6456dc; /* mov r12,r5 */
L8c6456dc: *stop=0x8c6456dcu;r[6]=r[10];goto L8c6456de; /* mov r10,r6 */
L8c6456de: target=0x8c645374u;*pr=0x8c6456e2u;*stop=0x8c6456e0u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6456e2; /* bsr 0x8c645374 */
L8c6456e2: *stop=0x8c6456e2u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6456e4; /* tst r0,r0 */
L8c6456e4: if((*sr&1u))goto L8c64570c;goto L8c6456e6; /* bt 0x8c64570c */
L8c6456e6: *stop=0x8c6456e6u;r[12]+=r[10];goto L8c6456e8; /* add r10,r12 */
L8c6456e8: *stop=0x8c6456e8u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[11],4);if(io_status)return io_status;goto L8c6456ea; /* mov.l r11,@(0x14,r15) */
L8c6456ea: *stop=0x8c6456eau;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[3]=loaded;}goto L8c6456ec; /* mov.b @r12,r3 */
L8c6456ec: *stop=0x8c6456ecu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6456ee; /* tst r3,r3 */
L8c6456ee: if(!(*sr&1u))goto L8c6456fc;goto L8c6456f0; /* bf 0x8c6456fc */
L8c6456f0: *stop=0x8c6456f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64576cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6456f2; /* mov.l 0x8c64576c,r3 */
L8c6456f2: *stop=0x8c6456f2u;r[4]=r[13];goto L8c6456f4; /* mov r13,r4 */
L8c6456f4: target=r[3];*pr=0x8c6456f8u;*stop=0x8c6456f6u;r[4]+=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6456f8; /* jsr @r3 */
L8c6456f8: *stop=0x8c6456fau;goto L8c64572a; /* bra 0x8c64572a */
L8c6456fc: *stop=0x8c6456fcu;r[0]=r[11];goto L8c6456fe; /* mov r11,r0 */
L8c6456fe: *stop=0x8c6456feu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c645700; /* cmp/eq #0x1,r0 */
L8c645700: if((*sr&1u))goto L8c645704;goto L8c645702; /* bt 0x8c645704 */
L8c645702: *stop=0x8c645702u;r[12]+=1u;goto L8c645704; /* add #0x1,r12 */
L8c645704: *stop=0x8c645704u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645770u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c645706; /* mov.l 0x8c645770,r5 */
L8c645706: target=0x8c64541cu;*pr=0x8c64570au;*stop=0x8c645708u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64570a; /* bsr 0x8c64541c */
L8c64570a: *stop=0x8c64570au;r[10]=r[0];goto L8c64570c; /* mov r0,r10 */
L8c64570c: *stop=0x8c64570cu;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[4]=loaded;}goto L8c64570e; /* mov.b @r13,r4 */
L8c64570e: *stop=0x8c64570eu;r[11]+=1u;goto L8c645710; /* add #0x1,r11 */
L8c645710: *stop=0x8c645710u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645712; /* mov.l @(0xc,r15),r3 */
L8c645712: *stop=0x8c645712u;r[4]=r[4]&255u;goto L8c645714; /* extu.b r4,r4 */
L8c645714: *stop=0x8c645714u;r[5]=r[4];goto L8c645716; /* mov r4,r5 */
L8c645716: *stop=0x8c645716u;r[5]&=r[8];goto L8c645718; /* and r8,r5 */
L8c645718: *stop=0x8c645718u;r[5]+=r[4];goto L8c64571a; /* add r4,r5 */
L8c64571a: *stop=0x8c64571au;r[5]+=8u;goto L8c64571c; /* add #0x8,r5 */
L8c64571c: *stop=0x8c64571cu;r[3]+=r[5];goto L8c64571e; /* add r5,r3 */
L8c64571e: *stop=0x8c64571eu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c645720; /* mov.l r3,@(0xc,r15) */
L8c645720: *stop=0x8c645720u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645722; /* mov.l @(0x10,r15),r2 */
L8c645722: *stop=0x8c645722u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c645724; /* cmp/ge r2,r3 */
L8c645724: take=!(*sr&1u);*stop=0x8c645726u;r[14]+=r[5];if(take)goto L8c64566c;goto L8c645728; /* bf/s 0x8c64566c */
L8c645728: *stop=0x8c645728u;r[0]=0u;goto L8c64572a; /* mov #0x0,r0 */
L8c64572a: *stop=0x8c64572au;r[15]+=24u;goto L8c64572c; /* add #0x18,r15 */
L8c64572c: *stop=0x8c64572cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64572e; /* lds.l @r15+,PR */
L8c64572e: *stop=0x8c64572eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c645730; /* mov.l @r15+,r8 */
L8c645730: *stop=0x8c645730u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c645732; /* mov.l @r15+,r9 */
L8c645732: *stop=0x8c645732u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c645734; /* mov.l @r15+,r10 */
L8c645734: *stop=0x8c645734u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c645736; /* mov.l @r15+,r11 */
L8c645736: *stop=0x8c645736u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c645738; /* mov.l @r15+,r12 */
L8c645738: *stop=0x8c645738u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64573a; /* mov.l @r15+,r13 */
L8c64573a: *stop=0x8c64573au;if(*pr!=entry_pr)return -3;*stop=0x8c64573cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_paths_8c64573e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64573e;
L8c64573e: *stop=0x8c64573eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645740; /* mov.l r14,@-r15 */
L8c645740: *stop=0x8c645740u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c645742; /* mov.l r13,@-r15 */
L8c645742: *stop=0x8c645742u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c645744; /* mov.l r12,@-r15 */
L8c645744: *stop=0x8c645744u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c645746; /* mov.l r11,@-r15 */
L8c645746: *stop=0x8c645746u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c645748; /* mov.l r10,@-r15 */
L8c645748: *stop=0x8c645748u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64574a; /* mov.l r9,@-r15 */
L8c64574a: *stop=0x8c64574au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64574c; /* mov.l r8,@-r15 */
L8c64574c: *stop=0x8c64574cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64574e; /* sts.l PR,@-r15 */
L8c64574e: *stop=0x8c64574eu;r[15]+=4294967256u;goto L8c645750; /* add #-0x28,r15 */
L8c645750: *stop=0x8c645750u;r[9]=0u;goto L8c645752; /* mov #0x0,r9 */
L8c645752: *stop=0x8c645752u;r[12]=r[7];goto L8c645754; /* mov r7,r12 */
L8c645754: *stop=0x8c645754u;r[8]=1u;goto L8c645756; /* mov #0x1,r8 */
L8c645756: *stop=0x8c645756u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c645758; /* mov.l r4,@r15 */
L8c645758: *stop=0x8c645758u;r[10]=r[8];goto L8c64575a; /* mov r8,r10 */
L8c64575a: *stop=0x8c64575au;r[0]=72u;goto L8c64575c; /* mov #0x48,r0 */
L8c64575c: *stop=0x8c64575cu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[5],4);if(io_status)return io_status;goto L8c64575e; /* mov.l r5,@(0x10,r15) */
L8c64575e: *stop=0x8c64575eu;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[6],4);if(io_status)return io_status;goto L8c645760; /* mov.l r6,@(0x20,r15) */
L8c645760: *stop=0x8c645760u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c645762; /* mov.l r9,@(0x4,r15) */
L8c645762: *stop=0x8c645764u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64577a; /* bra 0x8c64577a */
L8c645774: *stop=0x8c645774u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645776; /* mov.l @(0x4,r15),r2 */
L8c645776: *stop=0x8c645776u;r[2]+=1u;goto L8c645778; /* add #0x1,r2 */
L8c645778: *stop=0x8c645778u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c64577a; /* mov.l r2,@(0x4,r15) */
L8c64577a: *stop=0x8c64577au;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}r[4]+=1;goto L8c64577c; /* mov.b @r4+,r3 */
L8c64577c: *stop=0x8c64577cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64577e; /* tst r3,r3 */
L8c64577e: if(!(*sr&1u))goto L8c645774;goto L8c645780; /* bf 0x8c645774 */
L8c645780: *stop=0x8c645780u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645846u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645782; /* mov.w 0x8c645846,r3 */
L8c645782: *stop=0x8c645782u;r[12]+=1u;goto L8c645784; /* add #0x1,r12 */
L8c645784: *stop=0x8c645784u;r[11]=r[8];goto L8c645786; /* mov r8,r11 */
L8c645786: *stop=0x8c645786u;if(!valid_access(r[15]+36u,4u))return -3;wr(m,bus,&io_status,r[15]+0x24u,r[8],4);if(io_status)return io_status;goto L8c645788; /* mov.l r8,@(0x24,r15) */
L8c645788: *stop=0x8c645788u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[9],4);if(io_status)return io_status;goto L8c64578a; /* mov.l r9,@(0x14,r15) */
L8c64578a: *stop=0x8c64578au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}goto L8c64578c; /* mov.l @r15,r14 */
L8c64578c: *stop=0x8c64578cu;r[14]+=r[3];goto L8c64578e; /* add r3,r14 */
L8c64578e: *stop=0x8c64578eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[14],4);if(io_status)return io_status;goto L8c645790; /* mov.l r14,@(0x8,r15) */
L8c645790: *stop=0x8c645790u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[9],4);if(io_status)return io_status;goto L8c645792; /* mov.l r9,@(0x18,r15) */
L8c645792: *stop=0x8c645792u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645794; /* mov.l @(0x8,r15),r3 */
L8c645794: *stop=0x8c645794u;*sr=(*sr&~1u)|(r[14]>=r[3]);goto L8c645796; /* cmp/hs r3,r14 */
L8c645796: take=(*sr&1u);*stop=0x8c645798u;r[13]=r[14];if(take)goto L8c6457b0;goto L8c64579a; /* bt/s 0x8c6457b0 */
L8c64579a: *stop=0x8c64579au;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[0]=loaded;}goto L8c64579c; /* mov.b @r13,r0 */
L8c64579c: *stop=0x8c64579cu;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[1]=loaded;}goto L8c64579e; /* mov.b @r13,r1 */
L8c64579e: *stop=0x8c64579eu;r[0]=r[0]&255u;goto L8c6457a0; /* extu.b r0,r0 */
L8c6457a0: *stop=0x8c6457a0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6457a2; /* mov.l @(0x8,r15),r3 */
L8c6457a2: *stop=0x8c6457a2u;r[0]&=0x1u;goto L8c6457a4; /* and #0x1,r0 */
L8c6457a4: *stop=0x8c6457a4u;r[1]=r[1]&255u;goto L8c6457a6; /* extu.b r1,r1 */
L8c6457a6: *stop=0x8c6457a6u;r[1]+=r[0];goto L8c6457a8; /* add r0,r1 */
L8c6457a8: *stop=0x8c6457a8u;r[1]+=8u;goto L8c6457aa; /* add #0x8,r1 */
L8c6457aa: *stop=0x8c6457aau;r[1]+=r[14];goto L8c6457ac; /* add r14,r1 */
L8c6457ac: *stop=0x8c6457acu;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c6457ae; /* cmp/hs r3,r1 */
L8c6457ae: if(!(*sr&1u))goto L8c6457da;goto L8c6457b0; /* bf 0x8c6457da */
L8c6457b0: *stop=0x8c6457b0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6457b2; /* mov.l @r15,r6 */
L8c6457b2: *stop=0x8c6457b2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645846u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6457b4; /* mov.w 0x8c645846,r3 */
L8c6457b4: *stop=0x8c6457b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}goto L8c6457b6; /* mov.l @r15,r13 */
L8c6457b6: *stop=0x8c6457b6u;r[6]-=r[14];goto L8c6457b8; /* sub r14,r6 */
L8c6457b8: *stop=0x8c6457b8u;r[6]+=r[3];goto L8c6457ba; /* add r3,r6 */
L8c6457ba: *stop=0x8c6457bau;r[13]-=r[6];goto L8c6457bc; /* sub r6,r13 */
L8c6457bc: *stop=0x8c6457bcu;r[5]=r[13];goto L8c6457be; /* mov r13,r5 */
L8c6457be: target=0x8c64535cu;*pr=0x8c6457c2u;*stop=0x8c6457c0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6457c2; /* bsr 0x8c64535c */
L8c6457c2: *stop=0x8c6457c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6457c4; /* mov.l @r15,r6 */
L8c6457c4: *stop=0x8c6457c4u;r[5]=1u;goto L8c6457c6; /* mov #0x1,r5 */
L8c6457c6: *stop=0x8c6457c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645848u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6457c8; /* mov.l 0x8c645848,r14 */
L8c6457c8: target=r[14];*pr=0x8c6457ccu;*stop=0x8c6457cau;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6457cc; /* jsr @r14 */
L8c6457cc: *stop=0x8c6457ccu;r[4]=r[0];goto L8c6457ce; /* mov r0,r4 */
L8c6457ce: *stop=0x8c6457ceu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6457d0; /* tst r4,r4 */
L8c6457d0: if(!(*sr&1u))goto L8c64587e;goto L8c6457d2; /* bf 0x8c64587e */
L8c6457d2: *stop=0x8c6457d2u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6457d4; /* mov.l @(0x10,r15),r3 */
L8c6457d4: *stop=0x8c6457d4u;r[14]=r[13];goto L8c6457d6; /* mov r13,r14 */
L8c6457d6: *stop=0x8c6457d6u;r[3]+=1u;goto L8c6457d8; /* add #0x1,r3 */
L8c6457d8: *stop=0x8c6457d8u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6457da; /* mov.l r3,@(0x10,r15) */
L8c6457da: *stop=0x8c6457dau;r[5]=r[15];goto L8c6457dc; /* mov r15,r5 */
L8c6457dc: *stop=0x8c6457dcu;r[4]=r[13];goto L8c6457de; /* mov r13,r4 */
L8c6457de: *stop=0x8c6457deu;r[5]+=12u;goto L8c6457e0; /* add #0xc,r5 */
L8c6457e0: *stop=0x8c6457e0u;r[6]=2u;goto L8c6457e2; /* mov #0x2,r6 */
L8c6457e2: target=0x8c64535cu;*pr=0x8c6457e6u;*stop=0x8c6457e4u;r[4]+=6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6457e6; /* bsr 0x8c64535c */
L8c6457e6: *stop=0x8c6457e6u;if(!valid_access(r[15]+12u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0xcu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6457e8; /* mov.w @(0xc,r15),r0 */
L8c6457e8: *stop=0x8c6457e8u;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6457ea; /* mov.l @(0x24,r15),r3 */
L8c6457ea: *stop=0x8c6457eau;r[0]=r[0]&65535u;goto L8c6457ec; /* extu.w r0,r0 */
L8c6457ec: *stop=0x8c6457ecu;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c6457ee; /* cmp/eq r3,r0 */
L8c6457ee: if(!(*sr&1u))goto L8c645862;goto L8c6457f0; /* bf 0x8c645862 */
L8c6457f0: *stop=0x8c6457f0u;r[0]=r[11];goto L8c6457f2; /* mov r11,r0 */
L8c6457f2: *stop=0x8c6457f2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6457f4; /* cmp/eq #0x1,r0 */
L8c6457f4: if((*sr&1u))goto L8c645860;goto L8c6457f6; /* bt 0x8c645860 */
L8c6457f6: *stop=0x8c6457f6u;r[0]=r[10];goto L8c6457f8; /* mov r10,r0 */
L8c6457f8: *stop=0x8c6457f8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6457fa; /* cmp/eq #0x1,r0 */
L8c6457fa: if(!(*sr&1u))goto L8c645806;goto L8c6457fc; /* bf 0x8c645806 */
L8c6457fc: *stop=0x8c6457fcu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6457fe; /* mov.l @(0x4,r15),r3 */
L8c6457fe: *stop=0x8c6457feu;r[0]=72u;goto L8c645800; /* mov #0x48,r0 */
L8c645800: *stop=0x8c645800u;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c645802; /* mov.l r3,@(0x1c,r15) */
L8c645802: *stop=0x8c645804u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64580c; /* bra 0x8c64580c */
L8c645806: *stop=0x8c645806u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645808; /* mov.l @(0x18,r15),r2 */
L8c645808: *stop=0x8c645808u;r[5]=r[12];goto L8c64580a; /* mov r12,r5 */
L8c64580a: *stop=0x8c64580au;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c64580c; /* mov.l r2,@(0x1c,r15) */
L8c64580c: *stop=0x8c64580cu;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[3]=loaded;}goto L8c64580e; /* mov.b @r13,r3 */
L8c64580e: *stop=0x8c64580eu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645810; /* mov.l @(0x1c,r15),r2 */
L8c645810: *stop=0x8c645810u;r[3]=r[3]&255u;goto L8c645812; /* extu.b r3,r3 */
L8c645812: *stop=0x8c645812u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c645814; /* cmp/eq r2,r3 */
L8c645814: if(!(*sr&1u))goto L8c645862;goto L8c645816; /* bf 0x8c645862 */
L8c645816: *stop=0x8c645816u;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c645818; /* mov.l @(0x1c,r15),r6 */
L8c645818: *stop=0x8c645818u;r[4]=r[13];goto L8c64581a; /* mov r13,r4 */
L8c64581a: target=0x8c645374u;*pr=0x8c64581eu;*stop=0x8c64581cu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64581e; /* bsr 0x8c645374 */
L8c64581e: *stop=0x8c64581eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c645820; /* tst r0,r0 */
L8c645820: if((*sr&1u))goto L8c645862;goto L8c645822; /* bt 0x8c645862 */
L8c645822: *stop=0x8c645822u;*sr=(*sr&~1u)|!(r[10]&r[10]);goto L8c645824; /* tst r10,r10 */
L8c645824: if(!(*sr&1u))goto L8c64582a;goto L8c645826; /* bf 0x8c64582a */
L8c645826: *stop=0x8c645826u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645828; /* mov.l @(0x18,r15),r2 */
L8c645828: *stop=0x8c645828u;r[12]+=r[2];goto L8c64582a; /* add r2,r12 */
L8c64582a: *stop=0x8c64582au;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[3]=loaded;}goto L8c64582c; /* mov.b @r12,r3 */
L8c64582c: *stop=0x8c64582cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64582e; /* tst r3,r3 */
L8c64582e: if(!(*sr&1u))goto L8c64583c;goto L8c645830; /* bf 0x8c64583c */
L8c645830: *stop=0x8c645830u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64584cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c645832; /* mov.l 0x8c64584c,r14 */
L8c645832: *stop=0x8c645832u;r[4]=r[13];goto L8c645834; /* mov r13,r4 */
L8c645834: target=r[14];*pr=0x8c645838u;*stop=0x8c645836u;r[4]+=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645838; /* jsr @r14 */
L8c645838: *stop=0x8c64583au;goto L8c645880; /* bra 0x8c645880 */
L8c64583c: *stop=0x8c64583cu;r[0]=r[10];goto L8c64583e; /* mov r10,r0 */
L8c64583e: *stop=0x8c64583eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c645840; /* cmp/eq #0x1,r0 */
L8c645840: if(!(*sr&1u))goto L8c645850;goto L8c645842; /* bf 0x8c645850 */
L8c645842: *stop=0x8c645844u;r[10]=r[9];goto L8c645858; /* bra 0x8c645858 */
L8c645850: *stop=0x8c645850u;r[0]=r[11];goto L8c645852; /* mov r11,r0 */
L8c645852: *stop=0x8c645852u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c645854; /* cmp/eq #0x1,r0 */
L8c645854: if((*sr&1u))goto L8c645858;goto L8c645856; /* bt 0x8c645858 */
L8c645856: *stop=0x8c645856u;r[12]+=1u;goto L8c645858; /* add #0x1,r12 */
L8c645858: *stop=0x8c645858u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645894u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64585a; /* mov.l 0x8c645894,r5 */
L8c64585a: target=0x8c64541cu;*pr=0x8c64585eu;*stop=0x8c64585cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64585e; /* bsr 0x8c64541c */
L8c64585e: *stop=0x8c64585eu;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c645860; /* mov.l r0,@(0x18,r15) */
L8c645860: *stop=0x8c645860u;if(!valid_access(r[15]+36u,4u))return -3;wr(m,bus,&io_status,r[15]+0x24u,r[11],4);if(io_status)return io_status;goto L8c645862; /* mov.l r11,@(0x24,r15) */
L8c645862: *stop=0x8c645862u;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[5]=loaded;}goto L8c645864; /* mov.b @r13,r5 */
L8c645864: *stop=0x8c645864u;r[11]+=1u;goto L8c645866; /* add #0x1,r11 */
L8c645866: *stop=0x8c645866u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645868; /* mov.l @(0x14,r15),r3 */
L8c645868: *stop=0x8c645868u;r[5]=r[5]&255u;goto L8c64586a; /* extu.b r5,r5 */
L8c64586a: *stop=0x8c64586au;r[4]=r[5];goto L8c64586c; /* mov r5,r4 */
L8c64586c: *stop=0x8c64586cu;r[4]&=r[8];goto L8c64586e; /* and r8,r4 */
L8c64586e: *stop=0x8c64586eu;r[4]+=r[5];goto L8c645870; /* add r5,r4 */
L8c645870: *stop=0x8c645870u;r[4]+=8u;goto L8c645872; /* add #0x8,r4 */
L8c645872: *stop=0x8c645872u;r[3]+=r[4];goto L8c645874; /* add r4,r3 */
L8c645874: *stop=0x8c645874u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c645876; /* mov.l r3,@(0x14,r15) */
L8c645876: *stop=0x8c645876u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645878; /* mov.l @(0x20,r15),r2 */
L8c645878: *stop=0x8c645878u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c64587a; /* cmp/ge r2,r3 */
L8c64587a: take=!(*sr&1u);*stop=0x8c64587cu;r[14]+=r[4];if(take)goto L8c645792;goto L8c64587e; /* bf/s 0x8c645792 */
L8c64587e: *stop=0x8c64587eu;r[0]=0u;goto L8c645880; /* mov #0x0,r0 */
L8c645880: *stop=0x8c645880u;r[15]+=40u;goto L8c645882; /* add #0x28,r15 */
L8c645882: *stop=0x8c645882u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645884; /* lds.l @r15+,PR */
L8c645884: *stop=0x8c645884u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c645886; /* mov.l @r15+,r8 */
L8c645886: *stop=0x8c645886u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c645888; /* mov.l @r15+,r9 */
L8c645888: *stop=0x8c645888u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64588a; /* mov.l @r15+,r10 */
L8c64588a: *stop=0x8c64588au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64588c; /* mov.l @r15+,r11 */
L8c64588c: *stop=0x8c64588cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64588e; /* mov.l @r15+,r12 */
L8c64588e: *stop=0x8c64588eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c645890; /* mov.l @r15+,r13 */
L8c645890: *stop=0x8c645890u;if(*pr!=entry_pr)return -3;*stop=0x8c645892u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_paths_8c645374(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c645374;
L8c645374: *stop=0x8c645374u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645376; /* mov.l r14,@-r15 */
L8c645376: *stop=0x8c645376u;r[14]=r[6];goto L8c645378; /* mov r6,r14 */
L8c645378: *stop=0x8c645378u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64537a; /* mov.l r13,@-r15 */
L8c64537a: *stop=0x8c64537au;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c64537c; /* cmp/pl r14 */
L8c64537c: *stop=0x8c64537cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64537e; /* mov.l r12,@-r15 */
L8c64537e: *stop=0x8c64537eu;r[13]=r[4];goto L8c645380; /* mov r4,r13 */
L8c645380: *stop=0x8c645380u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c645382; /* mov.l r8,@-r15 */
L8c645382: *stop=0x8c645382u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c645384; /* sts.l PR,@-r15 */
L8c645384: take=!(*sr&1u);*stop=0x8c645386u;r[12]=r[5];if(take)goto L8c6453b2;goto L8c645388; /* bf/s 0x8c6453b2 */
L8c645388: *stop=0x8c645388u;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[2]=loaded;}goto L8c64538a; /* mov.b @r13,r2 */
L8c64538a: *stop=0x8c64538au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64538c; /* tst r2,r2 */
L8c64538c: if((*sr&1u))goto L8c6453a2;goto L8c64538e; /* bt 0x8c6453a2 */
L8c64538e: target=0x8c645348u;*pr=0x8c645392u;*stop=0x8c645390u;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[4]=loaded;}r[13]+=1;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645392; /* bsr 0x8c645348 */
L8c645392: *stop=0x8c645392u;r[8]=(uint32_t)(int32_t)(int8_t)r[0];goto L8c645394; /* exts.b r0,r8 */
L8c645394: target=0x8c645348u;*pr=0x8c645398u;*stop=0x8c645396u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[4]=loaded;}r[12]+=1;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645398; /* bsr 0x8c645348 */
L8c645398: *stop=0x8c645398u;r[0]=(uint32_t)(int32_t)(int8_t)r[0];goto L8c64539a; /* exts.b r0,r0 */
L8c64539a: *stop=0x8c64539au;*sr=(*sr&~1u)|(r[0]==r[8]);goto L8c64539c; /* cmp/eq r0,r8 */
L8c64539c: if(!(*sr&1u))goto L8c6453a8;goto L8c64539e; /* bf 0x8c6453a8 */
L8c64539e: *stop=0x8c6453a0u;goto L8c6453ac; /* bra 0x8c6453ac */
L8c6453a2: *stop=0x8c6453a2u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[2]=loaded;}goto L8c6453a4; /* mov.b @r12,r2 */
L8c6453a4: *stop=0x8c6453a4u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6453a6; /* tst r2,r2 */
L8c6453a6: if((*sr&1u))goto L8c6453b2;goto L8c6453a8; /* bt 0x8c6453b2 */
L8c6453a8: *stop=0x8c6453aau;r[0]=0u;goto L8c6453b4; /* bra 0x8c6453b4 */
L8c6453ac: *stop=0x8c6453acu;r[14]+=4294967295u;goto L8c6453ae; /* add #-0x1,r14 */
L8c6453ae: *stop=0x8c6453aeu;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c6453b0; /* cmp/pl r14 */
L8c6453b0: if((*sr&1u))goto L8c645388;goto L8c6453b2; /* bt 0x8c645388 */
L8c6453b2: *stop=0x8c6453b2u;r[0]=1u;goto L8c6453b4; /* mov #0x1,r0 */
L8c6453b4: *stop=0x8c6453b4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6453b6; /* lds.l @r15+,PR */
L8c6453b6: *stop=0x8c6453b6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6453b8; /* mov.l @r15+,r8 */
L8c6453b8: *stop=0x8c6453b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6453ba; /* mov.l @r15+,r12 */
L8c6453ba: *stop=0x8c6453bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6453bc; /* mov.l @r15+,r13 */
L8c6453bc: *stop=0x8c6453bcu;if(*pr!=entry_pr)return -3;*stop=0x8c6453beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_disc_paths_8c64541c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64541c;
L8c64541c: *stop=0x8c64541eu;r[7]=0u;goto L8c645438; /* bra 0x8c645438 */
L8c645420: *stop=0x8c645422u;r[6]=r[5];goto L8c64542e; /* bra 0x8c64542e */
L8c645424: *stop=0x8c645424u;if(!valid_access(r[6],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[2]=loaded;}goto L8c645426; /* mov.b @r6,r2 */
L8c645426: *stop=0x8c645426u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}goto L8c645428; /* mov.b @r4,r3 */
L8c645428: *stop=0x8c645428u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c64542a; /* cmp/eq r2,r3 */
L8c64542a: if((*sr&1u))goto L8c64543e;goto L8c64542c; /* bt 0x8c64543e */
L8c64542c: *stop=0x8c64542cu;r[6]+=1u;goto L8c64542e; /* add #0x1,r6 */
L8c64542e: *stop=0x8c64542eu;if(!valid_access(r[6],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[3]=loaded;}goto L8c645430; /* mov.b @r6,r3 */
L8c645430: *stop=0x8c645430u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c645432; /* tst r3,r3 */
L8c645432: if(!(*sr&1u))goto L8c645424;goto L8c645434; /* bf 0x8c645424 */
L8c645434: *stop=0x8c645434u;r[7]+=1u;goto L8c645436; /* add #0x1,r7 */
L8c645436: *stop=0x8c645436u;r[4]+=1u;goto L8c645438; /* add #0x1,r4 */
L8c645438: *stop=0x8c645438u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}goto L8c64543a; /* mov.b @r4,r3 */
L8c64543a: *stop=0x8c64543au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64543c; /* tst r3,r3 */
L8c64543c: if(!(*sr&1u))goto L8c645420;goto L8c64543e; /* bf 0x8c645420 */
L8c64543e: *stop=0x8c64543eu;if(*pr!=entry_pr)return -3;*stop=0x8c645440u;r[0]=r[7];return 0; /* rts */
}
