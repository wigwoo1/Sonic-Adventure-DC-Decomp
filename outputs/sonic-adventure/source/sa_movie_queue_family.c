/* Original driver-control word update; explicit machine/platform state.
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
SA_STAGE_EXPORT int sa_movie_queue_family_8c667080(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c667080;
L8c667080: *stop=0x8c667080u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c667082; /* mov.l r14,@-r15 */
L8c667082: *stop=0x8c667082u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c667084; /* sts.l PR,@-r15 */
L8c667084: *stop=0x8c667084u;r[15]+=4294967292u;goto L8c667086; /* add #-0x4,r15 */
L8c667086: *stop=0x8c667086u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c667088; /* stc SR,r0 */
L8c667088: *stop=0x8c667088u;{uint32_t loaded=sx(m,bus,&io_status,0x8c667134u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c66708a; /* mov.w 0x8c667134,r3 */
L8c66708a: *stop=0x8c66708au;r[14]=0u;goto L8c66708c; /* mov #0x0,r14 */
L8c66708c: *stop=0x8c66708cu;r[0]>>=2;goto L8c66708e; /* shlr2 r0 */
L8c66708e: *stop=0x8c66708eu;r[0]>>=2;goto L8c667090; /* shlr2 r0 */
L8c667090: *stop=0x8c667090u;r[0]&=0xfu;goto L8c667092; /* and #0xf,r0 */
L8c667092: *stop=0x8c667092u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c667094; /* mov.l r0,@r15 */
L8c667094: *stop=0x8c667094u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c667096; /* stc SR,r0 */
L8c667096: *stop=0x8c667096u;r[0]&=r[3];goto L8c667098; /* and r3,r0 */
L8c667098: *stop=0x8c667098u;r[0]|=0xf0u;goto L8c66709a; /* or #0xf0,r0 */
L8c66709a: *stop=0x8c66709au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c66709c; /* ldc r0,SR */
L8c66709c: *stop=0x8c66709cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66713cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c66709e; /* mov.l 0x8c66713c,r6 */
L8c66709e: *stop=0x8c66709eu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[7]=loaded;}goto L8c6670a0; /* mov.l @r6,r7 */
L8c6670a0: *stop=0x8c6670a0u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c6670a2; /* tst r7,r7 */
L8c6670a2: if(!(*sr&1u))goto L8c6670aa;goto L8c6670a4; /* bf 0x8c6670aa */
L8c6670a4: *stop=0x8c6670a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c667140u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6670a6; /* mov.l 0x8c667140,r14 */
L8c6670a6: *stop=0x8c6670a8u;goto L8c66710c; /* bra 0x8c66710c */
L8c6670aa: *stop=0x8c6670aau;{uint32_t loaded=sx(m,bus,&io_status,0x8c667136u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6670ac; /* mov.w 0x8c667136,r0 */
L8c6670ac: *stop=0x8c6670acu;if(!valid_access(r[0]+r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6670ae; /* mov.l @(r0,r7),r3 */
L8c6670ae: *stop=0x8c6670aeu;r[0]+=4294967284u;goto L8c6670b0; /* add #-0xc,r0 */
L8c6670b0: *stop=0x8c6670b0u;if(!valid_access(r[0]+r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6670b2; /* mov.l @(r0,r7),r2 */
L8c6670b2: *stop=0x8c6670b2u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]);goto L8c6670b4; /* cmp/gt r3,r2 */
L8c6670b4: if((*sr&1u))goto L8c6670bc;goto L8c6670b6; /* bt 0x8c6670bc */
L8c6670b6: *stop=0x8c6670b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c667144u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6670b8; /* mov.l 0x8c667144,r14 */
L8c6670b8: *stop=0x8c6670bau;goto L8c66710c; /* bra 0x8c66710c */
L8c6670bc: *stop=0x8c6670bcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c667138u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6670be; /* mov.w 0x8c667138,r0 */
L8c6670be: *stop=0x8c6670beu;if(!valid_access(r[0]+r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6670c0; /* mov.l @(r0,r7),r2 */
L8c6670c0: *stop=0x8c6670c0u;r[0]+=4294967288u;goto L8c6670c2; /* add #-0x8,r0 */
L8c6670c2: *stop=0x8c6670c2u;if(!valid_access(r[0]+r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6670c4; /* mov.l @(r0,r7),r3 */
L8c6670c4: *stop=0x8c6670c4u;r[0]+=8u;goto L8c6670c6; /* add #0x8,r0 */
L8c6670c6: *stop=0x8c6670c6u;r[2]<<=2;goto L8c6670c8; /* shll2 r2 */
L8c6670c8: *stop=0x8c6670c8u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6670ca; /* shll r2 */
L8c6670ca: *stop=0x8c6670cau;r[2]+=r[3];goto L8c6670cc; /* add r3,r2 */
L8c6670cc: *stop=0x8c6670ccu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c6670ce; /* mov.l r4,@r2 */
L8c6670ce: *stop=0x8c6670ceu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6670d0; /* mov.l @r6,r3 */
L8c6670d0: *stop=0x8c6670d0u;r[2]=r[3];goto L8c6670d2; /* mov r3,r2 */
L8c6670d2: *stop=0x8c6670d2u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6670d4; /* mov.l @(r0,r2),r1 */
L8c6670d4: *stop=0x8c6670d4u;r[0]+=4294967288u;goto L8c6670d6; /* add #-0x8,r0 */
L8c6670d6: *stop=0x8c6670d6u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6670d8; /* mov.l @(r0,r3),r3 */
L8c6670d8: *stop=0x8c6670d8u;r[1]<<=2;goto L8c6670da; /* shll2 r1 */
L8c6670da: *stop=0x8c6670dau;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c6670dc; /* shll r1 */
L8c6670dc: *stop=0x8c6670dcu;r[1]+=r[3];goto L8c6670de; /* add r3,r1 */
L8c6670de: *stop=0x8c6670deu;r[0]+=8u;goto L8c6670e0; /* add #0x8,r0 */
L8c6670e0: *stop=0x8c6670e0u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6670e2; /* mov.l r5,@(0x4,r1) */
L8c6670e2: *stop=0x8c6670e2u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6670e4; /* mov.l @r6,r3 */
L8c6670e4: *stop=0x8c6670e4u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6670e6; /* mov.l @(r0,r3),r2 */
L8c6670e6: *stop=0x8c6670e6u;r[2]+=1u;goto L8c6670e8; /* add #0x1,r2 */
L8c6670e8: *stop=0x8c6670e8u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c6670ea; /* mov.l r2,@(r0,r3) */
L8c6670ea: *stop=0x8c6670eau;r[0]+=4294967292u;goto L8c6670ec; /* add #-0x4,r0 */
L8c6670ec: *stop=0x8c6670ecu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6670ee; /* mov.l @r6,r3 */
L8c6670ee: *stop=0x8c6670eeu;r[2]=r[3];goto L8c6670f0; /* mov r3,r2 */
L8c6670f0: *stop=0x8c6670f0u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6670f2; /* mov.l @(r0,r2),r1 */
L8c6670f2: *stop=0x8c6670f2u;r[0]+=4u;goto L8c6670f4; /* add #0x4,r0 */
L8c6670f4: *stop=0x8c6670f4u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6670f6; /* mov.l @(r0,r3),r3 */
L8c6670f6: *stop=0x8c6670f6u;*sr=(*sr&~1u)|(r[1]==r[3]);goto L8c6670f8; /* cmp/eq r1,r3 */
L8c6670f8: if(!(*sr&1u))goto L8c667102;goto L8c6670fa; /* bf 0x8c667102 */
L8c6670fa: *stop=0x8c6670fau;{uint32_t loaded=sx(m,bus,&io_status,0x8c667138u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6670fc; /* mov.w 0x8c667138,r0 */
L8c6670fc: *stop=0x8c6670fcu;r[2]=0u;goto L8c6670fe; /* mov #0x0,r2 */
L8c6670fe: *stop=0x8c6670feu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c667100; /* mov.l @r6,r3 */
L8c667100: *stop=0x8c667100u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c667102; /* mov.l r2,@(r0,r3) */
L8c667102: *stop=0x8c667102u;{uint32_t loaded=sx(m,bus,&io_status,0x8c667136u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c667104; /* mov.w 0x8c667136,r0 */
L8c667104: *stop=0x8c667104u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c667106; /* mov.l @r6,r3 */
L8c667106: *stop=0x8c667106u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c667108; /* mov.l @(r0,r3),r2 */
L8c667108: *stop=0x8c667108u;r[2]+=1u;goto L8c66710a; /* add #0x1,r2 */
L8c66710a: *stop=0x8c66710au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c66710c; /* mov.l r2,@(r0,r3) */
L8c66710c: *stop=0x8c66710cu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c66710e; /* tst r14,r14 */
L8c66710e: if((*sr&1u))goto L8c667118;goto L8c667110; /* bt 0x8c667118 */
L8c667110: *stop=0x8c667110u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66714cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c667112; /* mov.l 0x8c66714c,r2 */
L8c667112: *stop=0x8c667112u;{uint32_t loaded=rd(m,bus,&io_status,0x8c667148u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c667114; /* mov.l 0x8c667148,r4 */
L8c667114: target=r[2];*pr=0x8c667118u;*stop=0x8c667116u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c667118; /* jsr @r2 */
L8c667118: *stop=0x8c667118u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66711a; /* mov.l @r15,r0 */
L8c66711a: *stop=0x8c66711au;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c66711c; /* stc SR,r3 */
L8c66711c: *stop=0x8c66711cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c667134u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c66711e; /* mov.w 0x8c667134,r2 */
L8c66711e: *stop=0x8c66711eu;r[0]&=0xfu;goto L8c667120; /* and #0xf,r0 */
L8c667120: *stop=0x8c667120u;r[0]<<=2;goto L8c667122; /* shll2 r0 */
L8c667122: *stop=0x8c667122u;r[3]&=r[2];goto L8c667124; /* and r2,r3 */
L8c667124: *stop=0x8c667124u;r[0]<<=2;goto L8c667126; /* shll2 r0 */
L8c667126: *stop=0x8c667126u;r[0]|=r[3];goto L8c667128; /* or r3,r0 */
L8c667128: *stop=0x8c667128u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c66712a; /* ldc r0,SR */
L8c66712a: *stop=0x8c66712au;r[0]=r[14];goto L8c66712c; /* mov r14,r0 */
L8c66712c: *stop=0x8c66712cu;r[15]+=4u;goto L8c66712e; /* add #0x4,r15 */
L8c66712e: *stop=0x8c66712eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c667130; /* lds.l @r15+,PR */
L8c667130: *stop=0x8c667130u;if(*pr!=entry_pr)return -3;*stop=0x8c667132u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_queue_family_8c666f98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c666f98;
L8c666f98: *stop=0x8c666f98u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c666f9a; /* mov.l r14,@-r15 */
L8c666f9a: *stop=0x8c666f9au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c666f9c; /* mov.l r13,@-r15 */
L8c666f9c: *stop=0x8c666f9cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c666f9e; /* sts.l PR,@-r15 */
L8c666f9e: *stop=0x8c666f9eu;r[15]+=4294967292u;goto L8c666fa0; /* add #-0x4,r15 */
L8c666fa0: *stop=0x8c666fa0u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c666fa2; /* stc SR,r0 */
L8c666fa2: *stop=0x8c666fa2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c66706au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c666fa4; /* mov.w 0x8c66706a,r3 */
L8c666fa4: *stop=0x8c666fa4u;r[13]=0u;goto L8c666fa6; /* mov #0x0,r13 */
L8c666fa6: *stop=0x8c666fa6u;r[14]=r[13];goto L8c666fa8; /* mov r13,r14 */
L8c666fa8: *stop=0x8c666fa8u;r[0]>>=2;goto L8c666faa; /* shlr2 r0 */
L8c666faa: *stop=0x8c666faau;r[0]>>=2;goto L8c666fac; /* shlr2 r0 */
L8c666fac: *stop=0x8c666facu;r[0]&=0xfu;goto L8c666fae; /* and #0xf,r0 */
L8c666fae: *stop=0x8c666faeu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c666fb0; /* mov.l r0,@r15 */
L8c666fb0: *stop=0x8c666fb0u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c666fb2; /* stc SR,r0 */
L8c666fb2: *stop=0x8c666fb2u;r[0]&=r[3];goto L8c666fb4; /* and r3,r0 */
L8c666fb4: *stop=0x8c666fb4u;r[0]|=0xf0u;goto L8c666fb6; /* or #0xf0,r0 */
L8c666fb6: *stop=0x8c666fb6u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c666fb8; /* ldc r0,SR */
L8c666fb8: *stop=0x8c666fb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c667070u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c666fba; /* mov.l 0x8c667070,r6 */
L8c666fba: *stop=0x8c666fbau;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[7]=loaded;}goto L8c666fbc; /* mov.l @r6,r7 */
L8c666fbc: *stop=0x8c666fbcu;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c666fbe; /* tst r7,r7 */
L8c666fbe: if(!(*sr&1u))goto L8c666fc6;goto L8c666fc0; /* bf 0x8c666fc6 */
L8c666fc0: *stop=0x8c666fc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c667074u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c666fc2; /* mov.l 0x8c667074,r14 */
L8c666fc2: *stop=0x8c666fc4u;goto L8c667040; /* bra 0x8c667040 */
L8c666fc6: *stop=0x8c666fc6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c66706cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666fc8; /* mov.w 0x8c66706c,r0 */
L8c666fc8: *stop=0x8c666fc8u;if(!valid_access(r[0]+r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666fca; /* mov.l @(r0,r7),r2 */
L8c666fca: *stop=0x8c666fcau;r[0]+=4294967284u;goto L8c666fcc; /* add #-0xc,r0 */
L8c666fcc: *stop=0x8c666fccu;if(!valid_access(r[0]+r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666fce; /* mov.l @(r0,r7),r3 */
L8c666fce: *stop=0x8c666fceu;r[0]+=12u;goto L8c666fd0; /* add #0xc,r0 */
L8c666fd0: *stop=0x8c666fd0u;r[2]<<=2;goto L8c666fd2; /* shll2 r2 */
L8c666fd2: *stop=0x8c666fd2u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c666fd4; /* shll r2 */
L8c666fd4: *stop=0x8c666fd4u;r[2]+=r[3];goto L8c666fd6; /* add r3,r2 */
L8c666fd6: *stop=0x8c666fd6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666fd8; /* mov.l @r2,r1 */
L8c666fd8: *stop=0x8c666fd8u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c666fda; /* mov.l r1,@r4 */
L8c666fda: *stop=0x8c666fdau;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666fdc; /* mov.l @r6,r3 */
L8c666fdc: *stop=0x8c666fdcu;r[2]=r[3];goto L8c666fde; /* mov r3,r2 */
L8c666fde: *stop=0x8c666fdeu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666fe0; /* mov.l @(r0,r2),r1 */
L8c666fe0: *stop=0x8c666fe0u;r[0]+=4294967284u;goto L8c666fe2; /* add #-0xc,r0 */
L8c666fe2: *stop=0x8c666fe2u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666fe4; /* mov.l @(r0,r3),r3 */
L8c666fe4: *stop=0x8c666fe4u;r[1]<<=2;goto L8c666fe6; /* shll2 r1 */
L8c666fe6: *stop=0x8c666fe6u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c666fe8; /* shll r1 */
L8c666fe8: *stop=0x8c666fe8u;r[1]+=r[3];goto L8c666fea; /* add r3,r1 */
L8c666fea: *stop=0x8c666feau;if(!valid_access(r[1]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666fec; /* mov.l @(0x4,r1),r2 */
L8c666fec: *stop=0x8c666fecu;r[0]+=12u;goto L8c666fee; /* add #0xc,r0 */
L8c666fee: *stop=0x8c666feeu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[2],4);if(io_status)return io_status;goto L8c666ff0; /* mov.l r2,@r5 */
L8c666ff0: *stop=0x8c666ff0u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666ff2; /* mov.l @r6,r3 */
L8c666ff2: *stop=0x8c666ff2u;r[2]=r[3];goto L8c666ff4; /* mov r3,r2 */
L8c666ff4: *stop=0x8c666ff4u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666ff6; /* mov.l @(r0,r2),r1 */
L8c666ff6: *stop=0x8c666ff6u;r[0]+=4294967284u;goto L8c666ff8; /* add #-0xc,r0 */
L8c666ff8: *stop=0x8c666ff8u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666ffa; /* mov.l @(r0,r3),r3 */
L8c666ffa: *stop=0x8c666ffau;r[1]<<=2;goto L8c666ffc; /* shll2 r1 */
L8c666ffc: *stop=0x8c666ffcu;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c666ffe; /* shll r1 */
L8c666ffe: *stop=0x8c666ffeu;r[1]+=r[3];goto L8c667000; /* add r3,r1 */
L8c667000: *stop=0x8c667000u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c667002; /* mov.l r13,@r1 */
L8c667002: *stop=0x8c667002u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c667004; /* mov.l @r6,r3 */
L8c667004: *stop=0x8c667004u;r[2]=r[3];goto L8c667006; /* mov r3,r2 */
L8c667006: *stop=0x8c667006u;r[0]+=12u;goto L8c667008; /* add #0xc,r0 */
L8c667008: *stop=0x8c667008u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c66700a; /* mov.l @(r0,r2),r1 */
L8c66700a: *stop=0x8c66700au;r[0]+=4294967284u;goto L8c66700c; /* add #-0xc,r0 */
L8c66700c: *stop=0x8c66700cu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66700e; /* mov.l @(r0,r3),r3 */
L8c66700e: *stop=0x8c66700eu;r[0]+=12u;goto L8c667010; /* add #0xc,r0 */
L8c667010: *stop=0x8c667010u;r[1]<<=2;goto L8c667012; /* shll2 r1 */
L8c667012: *stop=0x8c667012u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c667014; /* shll r1 */
L8c667014: *stop=0x8c667014u;r[1]+=r[3];goto L8c667016; /* add r3,r1 */
L8c667016: *stop=0x8c667016u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[13],4);if(io_status)return io_status;goto L8c667018; /* mov.l r13,@(0x4,r1) */
L8c667018: *stop=0x8c667018u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66701a; /* mov.l @r6,r3 */
L8c66701a: *stop=0x8c66701au;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66701c; /* mov.l @(r0,r3),r2 */
L8c66701c: *stop=0x8c66701cu;r[2]+=1u;goto L8c66701e; /* add #0x1,r2 */
L8c66701e: *stop=0x8c66701eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c667020; /* mov.l r2,@(r0,r3) */
L8c667020: *stop=0x8c667020u;r[0]+=4294967288u;goto L8c667022; /* add #-0x8,r0 */
L8c667022: *stop=0x8c667022u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c667024; /* mov.l @r6,r3 */
L8c667024: *stop=0x8c667024u;r[2]=r[3];goto L8c667026; /* mov r3,r2 */
L8c667026: *stop=0x8c667026u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c667028; /* mov.l @(r0,r2),r1 */
L8c667028: *stop=0x8c667028u;r[0]+=8u;goto L8c66702a; /* add #0x8,r0 */
L8c66702a: *stop=0x8c66702au;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66702c; /* mov.l @(r0,r3),r3 */
L8c66702c: *stop=0x8c66702cu;*sr=(*sr&~1u)|(r[1]==r[3]);goto L8c66702e; /* cmp/eq r1,r3 */
L8c66702e: if(!(*sr&1u))goto L8c667036;goto L8c667030; /* bf 0x8c667036 */
L8c667030: *stop=0x8c667030u;{uint32_t loaded=sx(m,bus,&io_status,0x8c66706cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c667032; /* mov.w 0x8c66706c,r0 */
L8c667032: *stop=0x8c667032u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c667034; /* mov.l @r6,r3 */
L8c667034: *stop=0x8c667034u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c667036; /* mov.l r13,@(r0,r3) */
L8c667036: *stop=0x8c667036u;{uint32_t loaded=sx(m,bus,&io_status,0x8c66706eu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c667038; /* mov.w 0x8c66706e,r0 */
L8c667038: *stop=0x8c667038u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66703a; /* mov.l @r6,r3 */
L8c66703a: *stop=0x8c66703au;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66703c; /* mov.l @(r0,r3),r2 */
L8c66703c: *stop=0x8c66703cu;r[2]+=4294967295u;goto L8c66703e; /* add #-0x1,r2 */
L8c66703e: *stop=0x8c66703eu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c667040; /* mov.l r2,@(r0,r3) */
L8c667040: *stop=0x8c667040u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c667042; /* tst r14,r14 */
L8c667042: if((*sr&1u))goto L8c66704c;goto L8c667044; /* bt 0x8c66704c */
L8c667044: *stop=0x8c667044u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66707cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c667046; /* mov.l 0x8c66707c,r2 */
L8c667046: *stop=0x8c667046u;{uint32_t loaded=rd(m,bus,&io_status,0x8c667078u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c667048; /* mov.l 0x8c667078,r4 */
L8c667048: target=r[2];*pr=0x8c66704cu;*stop=0x8c66704au;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66704c; /* jsr @r2 */
L8c66704c: *stop=0x8c66704cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66704e; /* mov.l @r15,r0 */
L8c66704e: *stop=0x8c66704eu;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c667050; /* stc SR,r3 */
L8c667050: *stop=0x8c667050u;{uint32_t loaded=sx(m,bus,&io_status,0x8c66706au,2);if(io_status)return io_status;r[2]=loaded;}goto L8c667052; /* mov.w 0x8c66706a,r2 */
L8c667052: *stop=0x8c667052u;r[0]&=0xfu;goto L8c667054; /* and #0xf,r0 */
L8c667054: *stop=0x8c667054u;r[0]<<=2;goto L8c667056; /* shll2 r0 */
L8c667056: *stop=0x8c667056u;r[3]&=r[2];goto L8c667058; /* and r2,r3 */
L8c667058: *stop=0x8c667058u;r[0]<<=2;goto L8c66705a; /* shll2 r0 */
L8c66705a: *stop=0x8c66705au;r[0]|=r[3];goto L8c66705c; /* or r3,r0 */
L8c66705c: *stop=0x8c66705cu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c66705e; /* ldc r0,SR */
L8c66705e: *stop=0x8c66705eu;r[0]=r[14];goto L8c667060; /* mov r14,r0 */
L8c667060: *stop=0x8c667060u;r[15]+=4u;goto L8c667062; /* add #0x4,r15 */
L8c667062: *stop=0x8c667062u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c667064; /* lds.l @r15+,PR */
L8c667064: *stop=0x8c667064u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c667066; /* mov.l @r15+,r13 */
L8c667066: *stop=0x8c667066u;if(*pr!=entry_pr)return -3;*stop=0x8c667068u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_queue_family_8c666d10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c666d10;
L8c666d10: *stop=0x8c666d10u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c666d12; /* mov.l r14,@-r15 */
L8c666d12: *stop=0x8c666d12u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c666d14; /* mov.l r13,@-r15 */
L8c666d14: *stop=0x8c666d14u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c666d16; /* mov.l r12,@-r15 */
L8c666d16: *stop=0x8c666d16u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c666d18; /* mov.l r11,@-r15 */
L8c666d18: *stop=0x8c666d18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c666d1a; /* mov.l r10,@-r15 */
L8c666d1a: *stop=0x8c666d1au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c666d1c; /* sts.l PR,@-r15 */
L8c666d1c: *stop=0x8c666d1cu;r[15]+=4294967284u;goto L8c666d1e; /* add #-0xc,r15 */
L8c666d1e: *stop=0x8c666d1eu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c666d20; /* stc SR,r0 */
L8c666d20: *stop=0x8c666d20u;{uint32_t loaded=sx(m,bus,&io_status,0x8c666e10u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c666d22; /* mov.w 0x8c666e10,r3 */
L8c666d22: *stop=0x8c666d22u;r[13]=r[4];goto L8c666d24; /* mov r4,r13 */
L8c666d24: *stop=0x8c666d24u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c666d26; /* mov.l r5,@(0x8,r15) */
L8c666d26: *stop=0x8c666d26u;r[0]>>=2;goto L8c666d28; /* shlr2 r0 */
L8c666d28: *stop=0x8c666d28u;r[0]>>=2;goto L8c666d2a; /* shlr2 r0 */
L8c666d2a: *stop=0x8c666d2au;r[0]&=0xfu;goto L8c666d2c; /* and #0xf,r0 */
L8c666d2c: *stop=0x8c666d2cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c666d2e; /* mov.l r0,@(0x4,r15) */
L8c666d2e: *stop=0x8c666d2eu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c666d30; /* stc SR,r0 */
L8c666d30: *stop=0x8c666d30u;r[0]&=r[3];goto L8c666d32; /* and r3,r0 */
L8c666d32: *stop=0x8c666d32u;r[0]|=0xf0u;goto L8c666d34; /* or #0xf0,r0 */
L8c666d34: *stop=0x8c666d34u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c666d36; /* ldc r0,SR */
L8c666d36: *stop=0x8c666d36u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e18u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c666d38; /* mov.l 0x8c666e18,r12 */
L8c666d38: *stop=0x8c666d38u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666d3a; /* mov.l @r12,r2 */
L8c666d3a: *stop=0x8c666d3au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c666d3c; /* tst r2,r2 */
L8c666d3c: if(!(*sr&1u))goto L8c666d44;goto L8c666d3e; /* bf 0x8c666d44 */
L8c666d3e: *stop=0x8c666d3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e1cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c666d40; /* mov.l 0x8c666e1c,r14 */
L8c666d40: *stop=0x8c666d42u;goto L8c666dd2; /* bra 0x8c666dd2 */
L8c666d44: *stop=0x8c666d44u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c666d46; /* tst r13,r13 */
L8c666d46: if(!(*sr&1u))goto L8c666d4e;goto L8c666d48; /* bf 0x8c666d4e */
L8c666d48: *stop=0x8c666d48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e20u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c666d4a; /* mov.l 0x8c666e20,r14 */
L8c666d4a: *stop=0x8c666d4cu;goto L8c666dd2; /* bra 0x8c666dd2 */
L8c666d4e: *stop=0x8c666d4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666d50; /* mov.l 0x8c666e24,r3 */
L8c666d50: *stop=0x8c666d50u;r[5]=r[15];goto L8c666d52; /* mov r15,r5 */
L8c666d52: target=r[3];*pr=0x8c666d56u;*stop=0x8c666d54u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666d56; /* jsr @r3 */
L8c666d56: *stop=0x8c666d56u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666d58; /* mov.l @r15,r0 */
L8c666d58: *stop=0x8c666d58u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c666d5a; /* cmp/eq #0x2,r0 */
L8c666d5a: if(!(*sr&1u))goto L8c666d62;goto L8c666d5c; /* bf 0x8c666d62 */
L8c666d5c: *stop=0x8c666d5cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e28u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c666d5e; /* mov.l 0x8c666e28,r14 */
L8c666d5e: *stop=0x8c666d60u;goto L8c666dd2; /* bra 0x8c666dd2 */
L8c666d62: *stop=0x8c666d62u;r[14]=r[13];goto L8c666d64; /* mov r13,r14 */
L8c666d64: *stop=0x8c666d64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666d66; /* mov.l 0x8c666e30,r3 */
L8c666d66: *stop=0x8c666d66u;r[2]=1u;goto L8c666d68; /* mov #0x1,r2 */
L8c666d68: *stop=0x8c666d68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e34u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666d6a; /* mov.l 0x8c666e34,r1 */
L8c666d6a: *stop=0x8c666d6au;r[10]=0u;goto L8c666d6c; /* mov #0x0,r10 */
L8c666d6c: *stop=0x8c666d6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e2cu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c666d6e; /* mov.l 0x8c666e2c,r11 */
L8c666d6e: *stop=0x8c666d6eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c666e12u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666d70; /* mov.w 0x8c666e12,r0 */
L8c666d70: *stop=0x8c666d70u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[10],4);if(io_status)return io_status;goto L8c666d72; /* mov.l r10,@r11 */
L8c666d72: *stop=0x8c666d72u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[10],4);if(io_status)return io_status;goto L8c666d74; /* mov.l r10,@(0x18,r14) */
L8c666d74: *stop=0x8c666d74u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c666d76; /* mov.l r14,@-r15 */
L8c666d76: *stop=0x8c666d76u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c666d78; /* mov.l r3,@-r15 */
L8c666d78: *stop=0x8c666d78u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c666d7a; /* mov.l r2,@-r15 */
L8c666d7a: *stop=0x8c666d7au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c666d7c; /* mov.l r2,@-r15 */
L8c666d7c: *stop=0x8c666d7cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c666d7e; /* mov.l r10,@-r15 */
L8c666d7e: *stop=0x8c666d7eu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c666d80; /* mov.l @(0x1c,r15),r5 */
L8c666d80: *stop=0x8c666d80u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}goto L8c666d82; /* mov.l @r12,r4 */
L8c666d82: *stop=0x8c666d82u;r[5]+=r[1];goto L8c666d84; /* add r1,r5 */
L8c666d84: *stop=0x8c666d84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e38u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666d86; /* mov.l 0x8c666e38,r1 */
L8c666d86: *stop=0x8c666d86u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c666d88; /* mov.l @(0xc,r14),r7 */
L8c666d88: *stop=0x8c666d88u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c666d8a; /* mov.l @(0x8,r14),r6 */
L8c666d8a: target=r[1];*pr=0x8c666d8eu;*stop=0x8c666d8cu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666d8e; /* jsr @r1 */
L8c666d8e: *stop=0x8c666d8eu;r[15]+=20u;goto L8c666d90; /* add #0x14,r15 */
L8c666d90: *stop=0x8c666d90u;r[3]=r[0];goto L8c666d92; /* mov r0,r3 */
L8c666d92: *stop=0x8c666d92u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c666d94; /* tst r3,r3 */
L8c666d94: take=(*sr&1u);*stop=0x8c666d96u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[0],4);if(io_status)return io_status;if(take)goto L8c666d9c;goto L8c666d98; /* bt/s 0x8c666d9c */
L8c666d98: target=0x8c666e06u;*pr=0x8c666d9cu;*stop=0x8c666d9au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666d9c; /* bsr 0x8c666e06 */
L8c666d9c: *stop=0x8c666d9cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666d9e; /* mov.l 0x8c666e3c,r2 */
L8c666d9e: *stop=0x8c666d9eu;r[5]=2u;goto L8c666da0; /* mov #0x2,r5 */
L8c666da0: target=r[2];*pr=0x8c666da4u;*stop=0x8c666da2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666da4; /* jsr @r2 */
L8c666da4: *stop=0x8c666da4u;r[14]=r[0];goto L8c666da6; /* mov r0,r14 */
L8c666da6: *stop=0x8c666da6u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c666da8; /* tst r14,r14 */
L8c666da8: if(!(*sr&1u))goto L8c666dd2;goto L8c666daa; /* bf 0x8c666dd2 */
L8c666daa: *stop=0x8c666daau;{uint32_t loaded=sx(m,bus,&io_status,0x8c666e14u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666dac; /* mov.w 0x8c666e14,r0 */
L8c666dac: *stop=0x8c666dacu;r[5]=0u;goto L8c666dae; /* mov #0x0,r5 */
L8c666dae: *stop=0x8c666daeu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666db0; /* mov.l @r12,r3 */
L8c666db0: *stop=0x8c666db0u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[13],4);if(io_status)return io_status;goto L8c666db2; /* mov.l r13,@(r0,r3) */
L8c666db2: *stop=0x8c666db2u;r[0]+=4294967288u;goto L8c666db4; /* add #-0x8,r0 */
L8c666db4: *stop=0x8c666db4u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666db6; /* mov.l @r12,r3 */
L8c666db6: *stop=0x8c666db6u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666db8; /* mov.l @(r0,r3),r2 */
L8c666db8: *stop=0x8c666db8u;r[2]+=1u;goto L8c666dba; /* add #0x1,r2 */
L8c666dba: *stop=0x8c666dbau;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c666dbc; /* mov.l r2,@(r0,r3) */
L8c666dbc: *stop=0x8c666dbcu;r[0]+=36u;goto L8c666dbe; /* add #0x24,r0 */
L8c666dbe: *stop=0x8c666dbeu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}goto L8c666dc0; /* mov.l @r12,r4 */
L8c666dc0: *stop=0x8c666dc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666dc2; /* mov.l 0x8c666e40,r3 */
L8c666dc2: target=r[3];*pr=0x8c666dc6u;*stop=0x8c666dc4u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666dc6; /* jsr @r3 */
L8c666dc6: *stop=0x8c666dc6u;r[2]=r[0];goto L8c666dc8; /* mov r0,r2 */
L8c666dc8: *stop=0x8c666dc8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c666dca; /* tst r2,r2 */
L8c666dca: take=(*sr&1u);*stop=0x8c666dccu;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[0],4);if(io_status)return io_status;if(take)goto L8c666dd2;goto L8c666dce; /* bt/s 0x8c666dd2 */
L8c666dce: target=0x8c666e06u;*pr=0x8c666dd2u;*stop=0x8c666dd0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666dd2; /* bsr 0x8c666e06 */
L8c666dd2: *stop=0x8c666dd2u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c666dd4; /* tst r14,r14 */
L8c666dd4: if((*sr&1u))goto L8c666dde;goto L8c666dd6; /* bt 0x8c666dde */
L8c666dd6: *stop=0x8c666dd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666dd8; /* mov.l 0x8c666e48,r3 */
L8c666dd8: *stop=0x8c666dd8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e44u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c666dda; /* mov.l 0x8c666e44,r4 */
L8c666dda: target=r[3];*pr=0x8c666ddeu;*stop=0x8c666ddcu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666dde; /* jsr @r3 */
L8c666dde: *stop=0x8c666ddeu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c666de0; /* mov.l @(0x4,r15),r0 */
L8c666de0: *stop=0x8c666de0u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c666de2; /* stc SR,r2 */
L8c666de2: *stop=0x8c666de2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c666e10u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c666de4; /* mov.w 0x8c666e10,r3 */
L8c666de4: *stop=0x8c666de4u;r[0]&=0xfu;goto L8c666de6; /* and #0xf,r0 */
L8c666de6: *stop=0x8c666de6u;r[0]<<=2;goto L8c666de8; /* shll2 r0 */
L8c666de8: *stop=0x8c666de8u;r[2]&=r[3];goto L8c666dea; /* and r3,r2 */
L8c666dea: *stop=0x8c666deau;r[0]<<=2;goto L8c666dec; /* shll2 r0 */
L8c666dec: *stop=0x8c666decu;r[0]|=r[2];goto L8c666dee; /* or r2,r0 */
L8c666dee: *stop=0x8c666deeu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c666df0; /* ldc r0,SR */
L8c666df0: *stop=0x8c666df0u;r[0]=r[14];goto L8c666df2; /* mov r14,r0 */
L8c666df2: *stop=0x8c666df2u;r[15]+=12u;goto L8c666df4; /* add #0xc,r15 */
L8c666df4: *stop=0x8c666df4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c666df6; /* lds.l @r15+,PR */
L8c666df6: *stop=0x8c666df6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c666df8; /* mov.l @r15+,r10 */
L8c666df8: *stop=0x8c666df8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c666dfa; /* mov.l @r15+,r11 */
L8c666dfa: *stop=0x8c666dfau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c666dfc; /* mov.l @r15+,r12 */
L8c666dfc: *stop=0x8c666dfcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c666dfe; /* mov.l @r15+,r13 */
L8c666dfe: *stop=0x8c666dfeu;if(*pr!=entry_pr)return -3;*stop=0x8c666e00u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_queue_family_8c666e02(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c666e02;
L8c666e02: *stop=0x8c666e02u;if(*pr!=entry_pr)return -3;*stop=0x8c666e04u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_queue_family_8c666c2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c666c2c;
L8c666c2c: *stop=0x8c666c2cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c666c2e; /* mov.l r14,@-r15 */
L8c666c2e: *stop=0x8c666c2eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c666c30; /* mov.l r13,@-r15 */
L8c666c30: *stop=0x8c666c30u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c666c32; /* mov.l r12,@-r15 */
L8c666c32: *stop=0x8c666c32u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c666c34; /* mov.l r11,@-r15 */
L8c666c34: *stop=0x8c666c34u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c666c36; /* sts.l PR,@-r15 */
L8c666c36: *stop=0x8c666c36u;r[15]+=4294967284u;goto L8c666c38; /* add #-0xc,r15 */
L8c666c38: *stop=0x8c666c38u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c666c3a; /* stc SR,r0 */
L8c666c3a: *stop=0x8c666c3au;{uint32_t loaded=sx(m,bus,&io_status,0x8c666ce2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c666c3c; /* mov.w 0x8c666ce2,r3 */
L8c666c3c: *stop=0x8c666c3cu;r[13]=r[4];goto L8c666c3e; /* mov r4,r13 */
L8c666c3e: *stop=0x8c666c3eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c666c40; /* mov.l r5,@(0x4,r15) */
L8c666c40: *stop=0x8c666c40u;r[0]>>=2;goto L8c666c42; /* shlr2 r0 */
L8c666c42: *stop=0x8c666c42u;r[0]>>=2;goto L8c666c44; /* shlr2 r0 */
L8c666c44: *stop=0x8c666c44u;r[0]&=0xfu;goto L8c666c46; /* and #0xf,r0 */
L8c666c46: *stop=0x8c666c46u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c666c48; /* mov.l r0,@(0x8,r15) */
L8c666c48: *stop=0x8c666c48u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c666c4a; /* stc SR,r0 */
L8c666c4a: *stop=0x8c666c4au;r[0]&=r[3];goto L8c666c4c; /* and r3,r0 */
L8c666c4c: *stop=0x8c666c4cu;r[0]|=0xf0u;goto L8c666c4e; /* or #0xf0,r0 */
L8c666c4e: *stop=0x8c666c4eu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c666c50; /* ldc r0,SR */
L8c666c50: *stop=0x8c666c50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666ce8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c666c52; /* mov.l 0x8c666ce8,r11 */
L8c666c52: *stop=0x8c666c52u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666c54; /* mov.l @r11,r2 */
L8c666c54: *stop=0x8c666c54u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c666c56; /* tst r2,r2 */
L8c666c56: if(!(*sr&1u))goto L8c666c5e;goto L8c666c58; /* bf 0x8c666c5e */
L8c666c58: *stop=0x8c666c58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666cecu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c666c5a; /* mov.l 0x8c666cec,r14 */
L8c666c5a: *stop=0x8c666c5cu;goto L8c666cb4; /* bra 0x8c666cb4 */
L8c666c5e: *stop=0x8c666c5eu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c666c60; /* tst r13,r13 */
L8c666c60: if(!(*sr&1u))goto L8c666c68;goto L8c666c62; /* bf 0x8c666c68 */
L8c666c62: *stop=0x8c666c62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666cf0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c666c64; /* mov.l 0x8c666cf0,r14 */
L8c666c64: *stop=0x8c666c66u;goto L8c666cb4; /* bra 0x8c666cb4 */
L8c666c68: *stop=0x8c666c68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666cf4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666c6a; /* mov.l 0x8c666cf4,r3 */
L8c666c6a: *stop=0x8c666c6au;r[5]=r[15];goto L8c666c6c; /* mov r15,r5 */
L8c666c6c: target=r[3];*pr=0x8c666c70u;*stop=0x8c666c6eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666c70; /* jsr @r3 */
L8c666c70: *stop=0x8c666c70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666c72; /* mov.l @r15,r0 */
L8c666c72: *stop=0x8c666c72u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c666c74; /* cmp/eq #0x2,r0 */
L8c666c74: if(!(*sr&1u))goto L8c666c7c;goto L8c666c76; /* bf 0x8c666c7c */
L8c666c76: *stop=0x8c666c76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666cf8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c666c78; /* mov.l 0x8c666cf8,r14 */
L8c666c78: *stop=0x8c666c7au;goto L8c666cb4; /* bra 0x8c666cb4 */
L8c666c7c: *stop=0x8c666c7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666cfcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666c7e; /* mov.l 0x8c666cfc,r3 */
L8c666c7e: *stop=0x8c666c7eu;r[2]=0u;goto L8c666c80; /* mov #0x0,r2 */
L8c666c80: *stop=0x8c666c80u;r[12]=r[13];goto L8c666c82; /* mov r13,r12 */
L8c666c82: *stop=0x8c666c82u;r[5]=2u;goto L8c666c84; /* mov #0x2,r5 */
L8c666c84: *stop=0x8c666c84u;if(!valid_access(r[12]+24u,4u))return -3;wr(m,bus,&io_status,r[12]+0x18u,r[2],4);if(io_status)return io_status;goto L8c666c86; /* mov.l r2,@(0x18,r12) */
L8c666c86: target=r[3];*pr=0x8c666c8au;*stop=0x8c666c88u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666c8a; /* jsr @r3 */
L8c666c8a: *stop=0x8c666c8au;r[14]=r[0];goto L8c666c8c; /* mov r0,r14 */
L8c666c8c: *stop=0x8c666c8cu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c666c8e; /* tst r14,r14 */
L8c666c8e: if(!(*sr&1u))goto L8c666cb4;goto L8c666c90; /* bf 0x8c666cb4 */
L8c666c90: *stop=0x8c666c90u;{uint32_t loaded=sx(m,bus,&io_status,0x8c666ce4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c666c92; /* mov.w 0x8c666ce4,r0 */
L8c666c92: *stop=0x8c666c92u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666c94; /* mov.l @r11,r2 */
L8c666c94: *stop=0x8c666c94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666d00u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c666c96; /* mov.l 0x8c666d00,r7 */
L8c666c96: *stop=0x8c666c96u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[13],4);if(io_status)return io_status;goto L8c666c98; /* mov.l r13,@(r0,r2) */
L8c666c98: *stop=0x8c666c98u;r[0]+=4294967288u;goto L8c666c9a; /* add #-0x8,r0 */
L8c666c9a: *stop=0x8c666c9au;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666c9c; /* mov.l @r11,r3 */
L8c666c9c: *stop=0x8c666c9cu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666c9e; /* mov.l @(r0,r3),r2 */
L8c666c9e: *stop=0x8c666c9eu;r[2]+=1u;goto L8c666ca0; /* add #0x1,r2 */
L8c666ca0: *stop=0x8c666ca0u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c666ca2; /* mov.l r2,@(r0,r3) */
L8c666ca2: *stop=0x8c666ca2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c666ca4; /* mov.l r12,@-r15 */
L8c666ca4: *stop=0x8c666ca4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c666ca6; /* mov.l @(0x8,r15),r5 */
L8c666ca6: *stop=0x8c666ca6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666d04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666ca8; /* mov.l 0x8c666d04,r3 */
L8c666ca8: *stop=0x8c666ca8u;if(!valid_access(r[12]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c666caa; /* mov.l @(0xc,r12),r6 */
L8c666caa: target=r[3];*pr=0x8c666caeu;*stop=0x8c666cacu;if(!valid_access(r[12]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666cae; /* jsr @r3 */
L8c666cae: *stop=0x8c666caeu;r[15]+=4u;goto L8c666cb0; /* add #0x4,r15 */
L8c666cb0: *stop=0x8c666cb0u;r[14]=r[0];goto L8c666cb2; /* mov r0,r14 */
L8c666cb2: *stop=0x8c666cb2u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c666cb4; /* tst r14,r14 */
L8c666cb4: *stop=0x8c666cb4u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c666cb6; /* tst r14,r14 */
L8c666cb6: if((*sr&1u))goto L8c666cc0;goto L8c666cb8; /* bt 0x8c666cc0 */
L8c666cb8: *stop=0x8c666cb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666d0cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666cba; /* mov.l 0x8c666d0c,r3 */
L8c666cba: *stop=0x8c666cbau;{uint32_t loaded=rd(m,bus,&io_status,0x8c666d08u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c666cbc; /* mov.l 0x8c666d08,r4 */
L8c666cbc: target=r[3];*pr=0x8c666cc0u;*stop=0x8c666cbeu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666cc0; /* jsr @r3 */
L8c666cc0: *stop=0x8c666cc0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c666cc2; /* mov.l @(0x8,r15),r0 */
L8c666cc2: *stop=0x8c666cc2u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c666cc4; /* stc SR,r2 */
L8c666cc4: *stop=0x8c666cc4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c666ce2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c666cc6; /* mov.w 0x8c666ce2,r3 */
L8c666cc6: *stop=0x8c666cc6u;r[0]&=0xfu;goto L8c666cc8; /* and #0xf,r0 */
L8c666cc8: *stop=0x8c666cc8u;r[0]<<=2;goto L8c666cca; /* shll2 r0 */
L8c666cca: *stop=0x8c666ccau;r[2]&=r[3];goto L8c666ccc; /* and r3,r2 */
L8c666ccc: *stop=0x8c666cccu;r[0]<<=2;goto L8c666cce; /* shll2 r0 */
L8c666cce: *stop=0x8c666cceu;r[0]|=r[2];goto L8c666cd0; /* or r2,r0 */
L8c666cd0: *stop=0x8c666cd0u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c666cd2; /* ldc r0,SR */
L8c666cd2: *stop=0x8c666cd2u;r[0]=r[14];goto L8c666cd4; /* mov r14,r0 */
L8c666cd4: *stop=0x8c666cd4u;r[15]+=12u;goto L8c666cd6; /* add #0xc,r15 */
L8c666cd6: *stop=0x8c666cd6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c666cd8; /* lds.l @r15+,PR */
L8c666cd8: *stop=0x8c666cd8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c666cda; /* mov.l @r15+,r11 */
L8c666cda: *stop=0x8c666cdau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c666cdc; /* mov.l @r15+,r12 */
L8c666cdc: *stop=0x8c666cdcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c666cde; /* mov.l @r15+,r13 */
L8c666cde: *stop=0x8c666cdeu;if(*pr!=entry_pr)return -3;*stop=0x8c666ce0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_queue_family_8c10f9f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10f9f4;
L8c10f9f4: *stop=0x8c10f9f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fa54u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f9f6; /* mov.l 0x8c10fa54,r3 */
L8c10f9f6: *stop=0x8c10f9f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fa58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f9f8; /* mov.l 0x8c10fa58,r2 */
L8c10f9f8: *stop=0x8c10f9f8u;r[6]&=r[3];goto L8c10f9fa; /* and r3,r6 */
L8c10f9fa: *stop=0x8c10f9fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fa5cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f9fc; /* mov.l 0x8c10fa5c,r1 */
L8c10f9fc: *stop=0x8c10f9fcu;r[5]&=r[3];goto L8c10f9fe; /* and r3,r5 */
L8c10f9fe: *stop=0x8c10f9feu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c10fa00; /* mov.l r5,@(0x4,r4) */
L8c10fa00: *stop=0x8c10fa00u;r[5]=1u;goto L8c10fa02; /* mov #0x1,r5 */
L8c10fa02: *stop=0x8c10fa02u;r[7]&=r[2];goto L8c10fa04; /* and r2,r7 */
L8c10fa04: *stop=0x8c10fa04u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[6],4);if(io_status)return io_status;goto L8c10fa06; /* mov.l r6,@(0x8,r4) */
L8c10fa06: *stop=0x8c10fa06u;r[7]|=r[1];goto L8c10fa08; /* or r1,r7 */
L8c10fa08: *stop=0x8c10fa08u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[7],4);if(io_status)return io_status;goto L8c10fa0a; /* mov.l r7,@(0xc,r4) */
L8c10fa0a: *stop=0x8c10fa0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fa0c; /* mov.l @r15,r3 */
L8c10fa0c: *stop=0x8c10fa0cu;r[3]&=r[5];goto L8c10fa0e; /* and r5,r3 */
L8c10fa0e: *stop=0x8c10fa0eu;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[3],4);if(io_status)return io_status;goto L8c10fa10; /* mov.l r3,@(0x10,r4) */
L8c10fa10: *stop=0x8c10fa10u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10fa12; /* mov.l @(0x4,r15),r0 */
L8c10fa12: *stop=0x8c10fa12u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10fa14; /* cmp/eq #0x1,r0 */
L8c10fa14: if(!(*sr&1u))goto L8c10fa1e;goto L8c10fa16; /* bf 0x8c10fa1e */
L8c10fa16: *stop=0x8c10fa16u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fa18; /* mov.l @(0x14,r4),r3 */
L8c10fa18: *stop=0x8c10fa18u;r[3]|=r[5];goto L8c10fa1a; /* or r5,r3 */
L8c10fa1a: *stop=0x8c10fa1cu;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[3],4);if(io_status)return io_status;goto L8c10fa26; /* bra 0x8c10fa26 */
L8c10fa1e: *stop=0x8c10fa1eu;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10fa20; /* mov.l @(0x14,r4),r0 */
L8c10fa20: *stop=0x8c10fa20u;r[3]=4294967293u;goto L8c10fa22; /* mov #-0x3,r3 */
L8c10fa22: *stop=0x8c10fa22u;r[0]&=r[3];goto L8c10fa24; /* and r3,r0 */
L8c10fa24: *stop=0x8c10fa24u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[0],4);if(io_status)return io_status;goto L8c10fa26; /* mov.l r0,@(0x14,r4) */
L8c10fa26: *stop=0x8c10fa26u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10fa28; /* mov.l @(0x8,r15),r0 */
L8c10fa28: *stop=0x8c10fa28u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10fa2a; /* cmp/eq #0x1,r0 */
L8c10fa2a: if(!(*sr&1u))goto L8c10fa34;goto L8c10fa2c; /* bf 0x8c10fa34 */
L8c10fa2c: *stop=0x8c10fa2cu;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10fa2e; /* mov.l @(0x14,r4),r0 */
L8c10fa2e: *stop=0x8c10fa2eu;r[0]|=0x4u;goto L8c10fa30; /* or #0x4,r0 */
L8c10fa30: *stop=0x8c10fa32u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[0],4);if(io_status)return io_status;goto L8c10fa3c; /* bra 0x8c10fa3c */
L8c10fa34: *stop=0x8c10fa34u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fa36; /* mov.l @(0x14,r4),r2 */
L8c10fa36: *stop=0x8c10fa36u;r[3]=4294967294u;goto L8c10fa38; /* mov #-0x2,r3 */
L8c10fa38: *stop=0x8c10fa38u;r[2]&=r[3];goto L8c10fa3a; /* and r3,r2 */
L8c10fa3a: *stop=0x8c10fa3au;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[2],4);if(io_status)return io_status;goto L8c10fa3c; /* mov.l r2,@(0x14,r4) */
L8c10fa3c: *stop=0x8c10fa3cu;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10fa3e; /* mov.l @(0x14,r4),r0 */
L8c10fa3e: *stop=0x8c10fa3eu;r[0]&=0x7u;goto L8c10fa40; /* and #0x7,r0 */
L8c10fa40: *stop=0x8c10fa40u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[0],4);if(io_status)return io_status;goto L8c10fa42; /* mov.l r0,@(0x14,r4) */
L8c10fa42: *stop=0x8c10fa42u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fa44; /* mov.l @(0xc,r15),r3 */
L8c10fa44: *stop=0x8c10fa44u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c10fa46; /* mov.l r3,@(0x1c,r4) */
L8c10fa46: *stop=0x8c10fa46u;r[3]=0u;goto L8c10fa48; /* mov #0x0,r3 */
L8c10fa48: *stop=0x8c10fa48u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fa4a; /* mov.l @(0x10,r15),r2 */
L8c10fa4a: *stop=0x8c10fa4au;if(!valid_access(r[4]+32u,4u))return -3;wr(m,bus,&io_status,r[4]+0x20u,r[2],4);if(io_status)return io_status;goto L8c10fa4c; /* mov.l r2,@(0x20,r4) */
L8c10fa4c: *stop=0x8c10fa4cu;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[3],4);if(io_status)return io_status;goto L8c10fa4e; /* mov.l r3,@(0x30,r4) */
L8c10fa4e: *stop=0x8c10fa4eu;if(*pr!=entry_pr)return -3;*stop=0x8c10fa50u;r[0]=r[3];return 0; /* rts */
}
