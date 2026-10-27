/* Original frame wait, queue records, release/submission and display sync.
 * Static C from byte-checked SH-4 instructions; complete explicit state.
 * Missing callees remain reached boundaries; frame wait is research-bounded
 * only under SA_BOUND_RETAIL_STALLS and is not a successful completion.
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
SA_STAGE_EXPORT int sa_movie_frame_wait_8c051790(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 unsigned delay_visits=0;(void)delay_visits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c051790;
L8c051790: *stop=0x8c051790u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c051792; /* sts.l PR,@-r15 */
L8c051792: *stop=0x8c051792u;{uint32_t loaded=rd(m,bus,&io_status,0x8c051884u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c051794; /* mov.l 0x8c051884,r4 */
L8c051794: *stop=0x8c051794u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c051796; /* mov.l @r4,r2 */
L8c051796: *stop=0x8c051796u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c051798; /* cmp/pl r2 */
L8c051798:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c051798u;if((*sr&1u) && ++delay_visits>=2u)return -4;
#endif
 if((*sr&1u))goto L8c051794;goto L8c05179a; /* bt 0x8c051794 */
L8c05179a: *stop=0x8c05179au;{uint32_t loaded=rd(m,bus,&io_status,0x8c05189cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c05179c; /* mov.l 0x8c05189c,r2 */
L8c05179c: *stop=0x8c05179cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c051880u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05179e; /* mov.l 0x8c051880,r3 */
L8c05179e: *stop=0x8c05179eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0517a0; /* mov.l @r3,r1 */
L8c0517a0: *stop=0x8c0517a0u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c0517a2; /* mov.l r1,@r4 */
L8c0517a2: *stop=0x8c0517a2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c0517a4; /* mov.l @r2,r0 */
L8c0517a4: *stop=0x8c0517a4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0517a6; /* cmp/eq #0x1,r0 */
L8c0517a6: if(!(*sr&1u))goto L8c0517b2;goto L8c0517a8; /* bf 0x8c0517b2 */
L8c0517a8: *stop=0x8c0517a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518a0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0517aa; /* mov.l 0x8c0518a0,r1 */
L8c0517aa: target=r[1];*pr=0x8c0517aeu;*stop=0x8c0517acu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0517ae; /* jsr @r1 */
L8c0517ae: *stop=0x8c0517b0u;goto L8c0517e2; /* bra 0x8c0517e2 */
L8c0517b2: *stop=0x8c0517b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0517b4; /* mov.l 0x8c0518a4,r1 */
L8c0517b4: target=r[1];*pr=0x8c0517b8u;*stop=0x8c0517b6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0517b8; /* jsr @r1 */
L8c0517b8: *stop=0x8c0517b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0517ba; /* mov.l 0x8c0518a8,r3 */
L8c0517ba: target=r[3];*pr=0x8c0517beu;*stop=0x8c0517bcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0517be; /* jsr @r3 */
L8c0517be: *stop=0x8c0517beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0517c0; /* mov.l 0x8c0518b0,r3 */
L8c0517c0: *stop=0x8c0517c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518acu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0517c2; /* mov.l 0x8c0518ac,r1 */
L8c0517c2: target=r[3];*pr=0x8c0517c6u;*stop=0x8c0517c4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0517c6; /* jsr @r3 */
L8c0517c6: *stop=0x8c0517c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0517c8; /* mov.l 0x8c0518b8,r3 */
L8c0517c8: *stop=0x8c0517c8u;r[4]=0u;goto L8c0517ca; /* mov #0x0,r4 */
L8c0517ca: *stop=0x8c0517cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518bcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0517cc; /* mov.l 0x8c0518bc,r1 */
L8c0517cc: *stop=0x8c0517ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0517ce; /* mov.l 0x8c0518b4,r2 */
L8c0517ce: *stop=0x8c0517ceu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c0517d0; /* mov.l r4,@r2 */
L8c0517d0: *stop=0x8c0517d0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c0517d2; /* mov.l r4,@r3 */
L8c0517d2: *stop=0x8c0517d2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c0517d4; /* mov.l r4,@r1 */
L8c0517d4: *stop=0x8c0517d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518c0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0517d6; /* mov.l 0x8c0518c0,r4 */
L8c0517d6: *stop=0x8c0517d6u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0517d8; /* mov.l @r4,r2 */
L8c0517d8: *stop=0x8c0517d8u;r[2]+=1u;goto L8c0517da; /* add #0x1,r2 */
L8c0517da: *stop=0x8c0517dau;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c0517dc; /* mov.l r2,@r4 */
L8c0517dc: *stop=0x8c0517dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518c4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0517de; /* mov.l 0x8c0518c4,r2 */
L8c0517de: target=r[2];*pr=0x8c0517e2u;*stop=0x8c0517e0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0517e2; /* jsr @r2 */
L8c0517e2: *stop=0x8c0517e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0517e4; /* mov.l 0x8c0518c8,r3 */
L8c0517e4: target=r[3];*pr=0x8c0517e8u;*stop=0x8c0517e6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0517e8; /* jsr @r3 */
L8c0517e8: *stop=0x8c0517e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518a8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0517ea; /* mov.l 0x8c0518a8,r1 */
L8c0517ea: target=r[1];*pr=0x8c0517eeu;*stop=0x8c0517ecu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0517ee; /* jsr @r1 */
L8c0517ee: *stop=0x8c0517eeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0517f0; /* lds.l @r15+,PR */
L8c0517f0: *stop=0x8c0517f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0517f2; /* mov.l 0x8c0518cc,r3 */
L8c0517f2: *stop=0x8c0517f2u;if(*pr!=entry_pr)return -3;*stop=0x8c0517f4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_frame_wait_8c6433e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6433e0;
L8c6433e0: *stop=0x8c6433e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6433e2; /* mov.l r14,@-r15 */
L8c6433e2: *stop=0x8c6433e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6433e4; /* mov.l r13,@-r15 */
L8c6433e4: *stop=0x8c6433e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6433e6; /* sts.l PR,@-r15 */
L8c6433e6: *stop=0x8c6433e6u;r[15]+=4294967292u;goto L8c6433e8; /* add #-0x4,r15 */
L8c6433e8: *stop=0x8c6433e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64348cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6433ea; /* mov.l 0x8c64348c,r3 */
L8c6433ea: *stop=0x8c6433eau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6433ec; /* mov.l @r3,r0 */
L8c6433ec: *stop=0x8c6433ecu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6433ee; /* cmp/eq #0x1,r0 */
L8c6433ee: if(!(*sr&1u))goto L8c643454;goto L8c6433f0; /* bf 0x8c643454 */
L8c6433f0: *stop=0x8c6433f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643490u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6433f2; /* mov.l 0x8c643490,r1 */
L8c6433f2: *stop=0x8c6433f2u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6433f4; /* mov.l @r1,r0 */
L8c6433f4: *stop=0x8c6433f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643494u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6433f6; /* mov.l 0x8c643494,r14 */
L8c6433f6: *stop=0x8c6433f6u;r[0]&=0x1u;goto L8c6433f8; /* and #0x1,r0 */
L8c6433f8: *stop=0x8c6433f8u;r[5]=1u;goto L8c6433fa; /* mov #0x1,r5 */
L8c6433fa: *stop=0x8c6433fau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c6433fc; /* mov.l r0,@r15 */
L8c6433fc: target=r[14];*pr=0x8c643400u;*stop=0x8c6433feu;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643400; /* jsr @r14 */
L8c643400: *stop=0x8c643400u;r[5]=0u;goto L8c643402; /* mov #0x0,r5 */
L8c643402: target=r[14];*pr=0x8c643406u;*stop=0x8c643404u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643406; /* jsr @r14 */
L8c643406: *stop=0x8c643406u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}goto L8c643408; /* mov.l @r15,r13 */
L8c643408: *stop=0x8c643408u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643498u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64340a; /* mov.l 0x8c643498,r0 */
L8c64340a: *stop=0x8c64340au;r[13]<<=2;goto L8c64340c; /* shll2 r13 */
L8c64340c: *stop=0x8c64340cu;r[4]=sx(m,bus,&io_status,0x8c643480u,2);if(io_status)return io_status;goto L8c64340e; /* mov.w 0x8c643480,r4 */
L8c64340e: *stop=0x8c64340eu;r[13]<<=2;goto L8c643410; /* shll2 r13 */
L8c643410: *stop=0x8c643410u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c643412; /* shll r13 */
L8c643412: target=r[14];*pr=0x8c643416u;*stop=0x8c643414u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643416; /* jsr @r14 */
L8c643416: *stop=0x8c643416u;r[4]=sx(m,bus,&io_status,0x8c643482u,2);if(io_status)return io_status;goto L8c643418; /* mov.w 0x8c643482,r4 */
L8c643418: *stop=0x8c643418u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64349cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64341a; /* mov.l 0x8c64349c,r0 */
L8c64341a: target=r[14];*pr=0x8c64341eu;*stop=0x8c64341cu;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64341e; /* jsr @r14 */
L8c64341e: *stop=0x8c64341eu;r[4]=sx(m,bus,&io_status,0x8c643484u,2);if(io_status)return io_status;goto L8c643420; /* mov.w 0x8c643484,r4 */
L8c643420: *stop=0x8c643420u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6434a0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c643422; /* mov.l 0x8c6434a0,r0 */
L8c643422: target=r[14];*pr=0x8c643426u;*stop=0x8c643424u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643426; /* jsr @r14 */
L8c643426: *stop=0x8c643426u;r[4]=sx(m,bus,&io_status,0x8c643486u,2);if(io_status)return io_status;goto L8c643428; /* mov.w 0x8c643486,r4 */
L8c643428: *stop=0x8c643428u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6434a4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64342a; /* mov.l 0x8c6434a4,r0 */
L8c64342a: target=r[14];*pr=0x8c64342eu;*stop=0x8c64342cu;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64342e; /* jsr @r14 */
L8c64342e: *stop=0x8c64342eu;r[4]=sx(m,bus,&io_status,0x8c643488u,2);if(io_status)return io_status;goto L8c643430; /* mov.w 0x8c643488,r4 */
L8c643430: *stop=0x8c643430u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6434a8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c643432; /* mov.l 0x8c6434a8,r0 */
L8c643432: target=r[14];*pr=0x8c643436u;*stop=0x8c643434u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643436; /* jsr @r14 */
L8c643436: *stop=0x8c643436u;r[4]=sx(m,bus,&io_status,0x8c64348au,2);if(io_status)return io_status;goto L8c643438; /* mov.w 0x8c64348a,r4 */
L8c643438: *stop=0x8c643438u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6434acu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64343a; /* mov.l 0x8c6434ac,r5 */
L8c64343a: target=r[14];*pr=0x8c64343eu;*stop=0x8c64343cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64343e; /* jsr @r14 */
L8c64343e: *stop=0x8c64343eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6434b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643440; /* mov.l 0x8c6434b0,r3 */
L8c643440: *stop=0x8c643440u;r[4]=sx(m,bus,&io_status,0x8c64348au,2);if(io_status)return io_status;goto L8c643442; /* mov.w 0x8c64348a,r4 */
L8c643442: target=r[3];*pr=0x8c643446u;*stop=0x8c643444u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643446; /* jsr @r3 */
L8c643446: *stop=0x8c643446u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6434b4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643448; /* mov.l 0x8c6434b4,r3 */
L8c643448: *stop=0x8c643448u;r[2]=0u;goto L8c64344a; /* mov #0x0,r2 */
L8c64344a: *stop=0x8c64344au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64344c; /* mov.l r2,@r3 */
L8c64344c: *stop=0x8c64344cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6434b8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64344e; /* mov.l 0x8c6434b8,r0 */
L8c64344e: *stop=0x8c64344eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c643450; /* mov.l @r0,r1 */
L8c643450: *stop=0x8c643450u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6434bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643452; /* mov.l 0x8c6434bc,r2 */
L8c643452: *stop=0x8c643452u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c643454; /* mov.l r1,@r2 */
L8c643454: *stop=0x8c643454u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6434c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643456; /* mov.l 0x8c6434c0,r3 */
L8c643456: *stop=0x8c643456u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c643458; /* mov.l @r3,r4 */
L8c643458: *stop=0x8c643458u;r[15]+=4u;goto L8c64345a; /* add #0x4,r15 */
L8c64345a: *stop=0x8c64345au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64345c; /* lds.l @r15+,PR */
L8c64345c: *stop=0x8c64345cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6434c4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64345e; /* mov.l 0x8c6434c4,r2 */
L8c64345e: *stop=0x8c64345eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c643460; /* mov.l @r15+,r13 */
L8c643460: target=r[2];*stop=0x8c643462u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_movie_frame_wait_8c055c4c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c055c4c;
L8c055c4c: *stop=0x8c055c4cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c055c4e; /* mov.l r14,@-r15 */
L8c055c4e: *stop=0x8c055c4eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c055c50; /* mov.l r13,@-r15 */
L8c055c50: *stop=0x8c055c50u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c055c52; /* mov.l r12,@-r15 */
L8c055c52: *stop=0x8c055c52u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c055c54; /* mov.l r11,@-r15 */
L8c055c54: *stop=0x8c055c54u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c055c56; /* mov.l r10,@-r15 */
L8c055c56: *stop=0x8c055c56u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c055c58; /* mov.l r9,@-r15 */
L8c055c58: *stop=0x8c055c58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c055c5a; /* mov.l r8,@-r15 */
L8c055c5a: *stop=0x8c055c5au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c055c5c; /* sts.l PR,@-r15 */
L8c055c5c: *stop=0x8c055c5cu;r[15]+=4294967252u;goto L8c055c5e; /* add #-0x2c,r15 */
L8c055c5e: *stop=0x8c055c5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c055d08u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c055c60; /* mov.l 0x8c055d08,r13 */
L8c055c60: *stop=0x8c055c60u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}goto L8c055c62; /* mov.l @r13,r4 */
L8c055c62: *stop=0x8c055c62u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c055c64; /* tst r4,r4 */
L8c055c64: if((*sr&1u))goto L8c055cca;goto L8c055c66; /* bt 0x8c055cca */
L8c055c66: *stop=0x8c055c66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c055d0cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c055c68; /* mov.l 0x8c055d0c,r14 */
L8c055c68: *stop=0x8c055c68u;r[9]=9u;goto L8c055c6a; /* mov #0x9,r9 */
L8c055c6a: *stop=0x8c055c6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c055d14u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c055c6c; /* mov.l 0x8c055d14,r10 */
L8c055c6c: *stop=0x8c055c6cu;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c055c6e; /* cmp/pl r4 */
L8c055c6e: *stop=0x8c055c6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c055d10u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c055c70; /* mov.l 0x8c055d10,r8 */
L8c055c70: *stop=0x8c055c70u;r[12]=0u;goto L8c055c72; /* mov #0x0,r12 */
L8c055c72: *stop=0x8c055c72u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[14]=loaded;}goto L8c055c74; /* mov.l @r14,r14 */
L8c055c74: *stop=0x8c055c74u;r[9]<<=8;goto L8c055c76; /* shll8 r9 */
L8c055c76: take=!(*sr&1u);*stop=0x8c055c78u;r[11]=1u;if(take)goto L8c055cc6;goto L8c055c7a; /* bf/s 0x8c055cc6 */
L8c055c7a: *stop=0x8c055c7au;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c055c7c; /* mov.l @(0x28,r14),r3 */
L8c055c7c: *stop=0x8c055c7cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c055c7e; /* tst r3,r3 */
L8c055c7e: if((*sr&1u))goto L8c055c88;goto L8c055c80; /* bt 0x8c055c88 */
L8c055c80: *stop=0x8c055c80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c055d04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c055c82; /* mov.l 0x8c055d04,r3 */
L8c055c82: *stop=0x8c055c82u;r[4]=r[14];goto L8c055c84; /* mov r14,r4 */
L8c055c84: target=r[3];*pr=0x8c055c88u;*stop=0x8c055c86u;r[4]+=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c055c88; /* jsr @r3 */
L8c055c88: *stop=0x8c055c88u;if(!valid_access(r[14]+12u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0xcu,2);if(io_status)return io_status;goto L8c055c8a; /* mov.w @(0xc,r14),r0 */
L8c055c8a: *stop=0x8c055c8au;r[6]=r[9];goto L8c055c8c; /* mov r9,r6 */
L8c055c8c: *stop=0x8c055c8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c055d18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c055c8e; /* mov.l 0x8c055d18,r3 */
L8c055c8e: *stop=0x8c055c8eu;r[0]=r[0]&65535u;goto L8c055c90; /* extu.w r0,r0 */
L8c055c90: *stop=0x8c055c90u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c055c92; /* mov.l r0,@-r15 */
L8c055c92: *stop=0x8c055c92u;if(!valid_access(r[14]+10u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0xau,2);if(io_status)return io_status;goto L8c055c94; /* mov.w @(0xa,r14),r0 */
L8c055c94: *stop=0x8c055c94u;r[4]=r[15];goto L8c055c96; /* mov r15,r4 */
L8c055c96: *stop=0x8c055c96u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c055c98; /* mov.l @(0x20,r14),r5 */
L8c055c98: *stop=0x8c055c98u;r[7]=r[0]&65535u;goto L8c055c9a; /* extu.w r0,r7 */
L8c055c9a: target=r[3];*pr=0x8c055c9eu;*stop=0x8c055c9cu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c055c9e; /* jsr @r3 */
L8c055c9e: *stop=0x8c055c9eu;r[15]+=4u;goto L8c055ca0; /* add #0x4,r15 */
L8c055ca0: *stop=0x8c055ca0u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c055ca2; /* mov.l @(0x24,r14),r6 */
L8c055ca2: *stop=0x8c055ca2u;r[4]=r[14];goto L8c055ca4; /* mov r14,r4 */
L8c055ca4: *stop=0x8c055ca4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c055d1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c055ca6; /* mov.l 0x8c055d1c,r2 */
L8c055ca6: *stop=0x8c055ca6u;r[7]=r[10];goto L8c055ca8; /* mov r10,r7 */
L8c055ca8: *stop=0x8c055ca8u;r[5]=r[15];goto L8c055caa; /* mov r15,r5 */
L8c055caa: target=r[2];*pr=0x8c055caeu;*stop=0x8c055cacu;r[4]+=48u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c055cae; /* jsr @r2 */
L8c055cae: *stop=0x8c055caeu;r[3]=r[14];goto L8c055cb0; /* mov r14,r3 */
L8c055cb0: *stop=0x8c055cb0u;r[3]+=48u;goto L8c055cb2; /* add #0x30,r3 */
L8c055cb2: *stop=0x8c055cb2u;r[4]=r[14];goto L8c055cb4; /* mov r14,r4 */
L8c055cb4: *stop=0x8c055cb4u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[3],4);if(io_status)return io_status;goto L8c055cb6; /* mov.l r3,@(0x28,r14) */
L8c055cb6: *stop=0x8c055cb6u;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[11],4);if(io_status)return io_status;goto L8c055cb8; /* mov.l r11,@(0x2c,r14) */
L8c055cb8: target=r[8];*pr=0x8c055cbcu;*stop=0x8c055cbau;r[4]+=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c055cbc; /* jsr @r8 */
L8c055cbc: *stop=0x8c055cbcu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c055cbe; /* mov.l @r13,r2 */
L8c055cbe: *stop=0x8c055cbeu;r[12]+=1u;goto L8c055cc0; /* add #0x1,r12 */
L8c055cc0: *stop=0x8c055cc0u;*sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[2]);goto L8c055cc2; /* cmp/ge r2,r12 */
L8c055cc2: take=!(*sr&1u);*stop=0x8c055cc4u;r[14]+=60u;if(take)goto L8c055c7a;goto L8c055cc6; /* bf/s 0x8c055c7a */
L8c055cc6: *stop=0x8c055cc6u;r[1]=0u;goto L8c055cc8; /* mov #0x0,r1 */
L8c055cc8: *stop=0x8c055cc8u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[1],4);if(io_status)return io_status;goto L8c055cca; /* mov.l r1,@r13 */
L8c055cca: *stop=0x8c055ccau;r[15]+=44u;goto L8c055ccc; /* add #0x2c,r15 */
L8c055ccc: *stop=0x8c055cccu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c055cce; /* lds.l @r15+,PR */
L8c055cce: *stop=0x8c055cceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c055cd0; /* mov.l @r15+,r8 */
L8c055cd0: *stop=0x8c055cd0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c055cd2; /* mov.l @r15+,r9 */
L8c055cd2: *stop=0x8c055cd2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c055cd4; /* mov.l @r15+,r10 */
L8c055cd4: *stop=0x8c055cd4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c055cd6; /* mov.l @r15+,r11 */
L8c055cd6: *stop=0x8c055cd6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c055cd8; /* mov.l @r15+,r12 */
L8c055cd8: *stop=0x8c055cd8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c055cda; /* mov.l @r15+,r13 */
L8c055cda: *stop=0x8c055cdau;if(*pr!=entry_pr)return -3;*stop=0x8c055cdcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_frame_wait_8c6088bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6088bc;
L8c6088bc: *stop=0x8c6088bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6088be; /* mov.l r14,@-r15 */
L8c6088be: *stop=0x8c6088beu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6088c0; /* mov.l r13,@-r15 */
L8c6088c0: *stop=0x8c6088c0u;r[13]=r[4];goto L8c6088c2; /* mov r4,r13 */
L8c6088c2: *stop=0x8c6088c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6088c4; /* mov.l r12,@-r15 */
L8c6088c4: *stop=0x8c6088c4u;r[12]=1u;goto L8c6088c6; /* mov #0x1,r12 */
L8c6088c6: *stop=0x8c6088c6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6088c8; /* mov.l r11,@-r15 */
L8c6088c8: *stop=0x8c6088c8u;r[11]=4294967295u;goto L8c6088ca; /* mov #-0x1,r11 */
L8c6088ca: *stop=0x8c6088cau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6088cc; /* mov.l r10,@-r15 */
L8c6088cc: *stop=0x8c6088ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6088ce; /* mov.l r9,@-r15 */
L8c6088ce: *stop=0x8c6088ceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6088d0; /* sts.l PR,@-r15 */
L8c6088d0: *stop=0x8c6088d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608910u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c6088d2; /* mov.l 0x8c608910,r10 */
L8c6088d2: *stop=0x8c6088d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60890cu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c6088d4; /* mov.l 0x8c60890c,r9 */
L8c6088d4: *stop=0x8c6088d6u;r[14]=0u;goto L8c6088f4; /* bra 0x8c6088f4 */
L8c6088d8: *stop=0x8c6088d8u;r[4]=r[14];goto L8c6088da; /* mov r14,r4 */
L8c6088da: *stop=0x8c6088dau;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6088dc; /* shll r4 */
L8c6088dc: *stop=0x8c6088dcu;r[3]=r[14];goto L8c6088de; /* mov r14,r3 */
L8c6088de: *stop=0x8c6088deu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6088e0; /* mov.l @r13,r2 */
L8c6088e0: *stop=0x8c6088e0u;r[4]+=r[3];goto L8c6088e2; /* add r3,r4 */
L8c6088e2: *stop=0x8c6088e2u;r[4]<<=2;goto L8c6088e4; /* shll2 r4 */
L8c6088e4: *stop=0x8c6088e4u;r[4]+=r[2];goto L8c6088e6; /* add r2,r4 */
L8c6088e6: target=r[10];*pr=0x8c6088eau;*stop=0x8c6088e8u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6088ea; /* jsr @r10 */
L8c6088ea: *stop=0x8c6088eau;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c6088ec; /* cmp/pz r0 */
L8c6088ec: if((*sr&1u))goto L8c6088f2;goto L8c6088ee; /* bt 0x8c6088f2 */
L8c6088ee: *stop=0x8c6088eeu;r[12]=r[11];goto L8c6088f0; /* mov r11,r12 */
L8c6088f0: *stop=0x8c6088f0u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[14],4);if(io_status)return io_status;goto L8c6088f2; /* mov.l r14,@r9 */
L8c6088f2: *stop=0x8c6088f2u;r[14]+=1u;goto L8c6088f4; /* add #0x1,r14 */
L8c6088f4: *stop=0x8c6088f4u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6088f6; /* mov.l @(0x4,r13),r2 */
L8c6088f6: *stop=0x8c6088f6u;*sr=(*sr&~1u)|(r[14]>=r[2]);goto L8c6088f8; /* cmp/hs r2,r14 */
L8c6088f8: if(!(*sr&1u))goto L8c6088d8;goto L8c6088fa; /* bf 0x8c6088d8 */
L8c6088fa: *stop=0x8c6088fau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6088fc; /* lds.l @r15+,PR */
L8c6088fc: *stop=0x8c6088fcu;r[0]=r[12];goto L8c6088fe; /* mov r12,r0 */
L8c6088fe: *stop=0x8c6088feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c608900; /* mov.l @r15+,r9 */
L8c608900: *stop=0x8c608900u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c608902; /* mov.l @r15+,r10 */
L8c608902: *stop=0x8c608902u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c608904; /* mov.l @r15+,r11 */
L8c608904: *stop=0x8c608904u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c608906; /* mov.l @r15+,r12 */
L8c608906: *stop=0x8c608906u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c608908; /* mov.l @r15+,r13 */
L8c608908: *stop=0x8c608908u;if(*pr!=entry_pr)return -3;*stop=0x8c60890au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_frame_wait_8c608914(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c608914;
L8c608914: *stop=0x8c608914u;r[6]<<=16;goto L8c608916; /* shll16 r6 */
L8c608916: *stop=0x8c608916u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c608918; /* mov.l r5,@r4 */
L8c608918: *stop=0x8c608918u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[7],4);if(io_status)return io_status;goto L8c60891a; /* mov.l r7,@(0x10,r4) */
L8c60891a: *stop=0x8c60891au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60891c; /* mov.l @r15,r3 */
L8c60891c: *stop=0x8c60891cu;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[3],4);if(io_status)return io_status;goto L8c60891e; /* mov.l r3,@(0x14,r4) */
L8c60891e: *stop=0x8c60891eu;if(*pr!=entry_pr)return -3;*stop=0x8c608920u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[6],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_frame_wait_8c608922(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c608922;
L8c608922: *stop=0x8c608922u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608930u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608924; /* mov.l 0x8c608930,r3 */
L8c608924: *stop=0x8c608924u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c608926; /* mov.l r5,@r4 */
L8c608926: *stop=0x8c608926u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[7],4);if(io_status)return io_status;goto L8c608928; /* mov.l r7,@(0x4,r4) */
L8c608928: *stop=0x8c608928u;*sr=(*sr&~1u)|!(r[7]&r[3]);goto L8c60892a; /* tst r7,r3 */
L8c60892a: *stop=0x8c60892au;if(*pr!=entry_pr)return -3;*stop=0x8c60892cu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[6],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_frame_wait_8c608c82(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c608c82;
L8c608c82: *stop=0x8c608c82u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c608c84; /* mov.l r14,@-r15 */
L8c608c84: *stop=0x8c608c84u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c608c86; /* mov.l r13,@-r15 */
L8c608c86: *stop=0x8c608c86u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c608c88; /* mov.l r12,@-r15 */
L8c608c88: *stop=0x8c608c88u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c608c8a; /* mov.l r11,@-r15 */
L8c608c8a: *stop=0x8c608c8au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c608c8c; /* sts.l PR,@-r15 */
L8c608c8c: target=0x8c608e24u;*pr=0x8c608c90u;*stop=0x8c608c8eu;r[13]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608c90; /* bsr 0x8c608e24 */
L8c608c90: *stop=0x8c608c90u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608c92; /* mov.l @(0x4,r13),r3 */
L8c608c92: *stop=0x8c608c92u;r[2]=0u;goto L8c608c94; /* mov #0x0,r2 */
L8c608c94: *stop=0x8c608c94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608d08u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c608c96; /* mov.l 0x8c608d08,r12 */
L8c608c96: *stop=0x8c608c96u;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c608c98; /* cmp/hi r2,r3 */
L8c608c98: if((*sr&1u))goto L8c608ca0;goto L8c608c9a; /* bt 0x8c608ca0 */
L8c608c9a: *stop=0x8c608c9au;r[0]=1u;goto L8c608c9c; /* mov #0x1,r0 */
L8c608c9c: *stop=0x8c608c9eu;if(!valid_access(r[12]+8u,4u))return -3;wr(m,bus,&io_status,r[12]+0x8u,r[0],4);if(io_status)return io_status;goto L8c608cb2; /* bra 0x8c608cb2 */
L8c608ca0: *stop=0x8c608ca2u;r[14]=0u;goto L8c608cb8; /* bra 0x8c608cb8 */
L8c608ca4: *stop=0x8c608ca4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608d0cu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c608ca6; /* mov.l 0x8c608d0c,r11 */
L8c608ca6: *stop=0x8c608ca6u;r[5]=r[14];goto L8c608ca8; /* mov r14,r5 */
L8c608ca8: target=r[11];*pr=0x8c608cacu;*stop=0x8c608caau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608cac; /* jsr @r11 */
L8c608cac: *stop=0x8c608cacu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c608cae; /* cmp/pz r0 */
L8c608cae: if((*sr&1u))goto L8c608cb6;goto L8c608cb0; /* bt 0x8c608cb6 */
L8c608cb0: *stop=0x8c608cb0u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[14],4);if(io_status)return io_status;goto L8c608cb2; /* mov.l r14,@r12 */
L8c608cb2: *stop=0x8c608cb4u;r[0]=4294967295u;goto L8c608cc0; /* bra 0x8c608cc0 */
L8c608cb6: *stop=0x8c608cb6u;r[14]+=1u;goto L8c608cb8; /* add #0x1,r14 */
L8c608cb8: *stop=0x8c608cb8u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608cba; /* mov.l @(0x4,r13),r2 */
L8c608cba: *stop=0x8c608cbau;*sr=(*sr&~1u)|(r[14]>=r[2]);goto L8c608cbc; /* cmp/hs r2,r14 */
L8c608cbc: if(!(*sr&1u))goto L8c608ca4;goto L8c608cbe; /* bf 0x8c608ca4 */
L8c608cbe: *stop=0x8c608cbeu;r[0]=1u;goto L8c608cc0; /* mov #0x1,r0 */
L8c608cc0: *stop=0x8c608cc0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c608cc2; /* lds.l @r15+,PR */
L8c608cc2: *stop=0x8c608cc2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c608cc4; /* mov.l @r15+,r11 */
L8c608cc4: *stop=0x8c608cc4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c608cc6; /* mov.l @r15+,r12 */
L8c608cc6: *stop=0x8c608cc6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c608cc8; /* mov.l @r15+,r13 */
L8c608cc8: *stop=0x8c608cc8u;if(*pr!=entry_pr)return -3;*stop=0x8c608ccau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_frame_wait_8c608e24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c608e24;
L8c608e24: *stop=0x8c608e24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608e70u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c608e26; /* mov.l 0x8c608e70,r4 */
L8c608e26: *stop=0x8c608e26u;r[5]=4294967295u;goto L8c608e28; /* mov #-0x1,r5 */
L8c608e28: *stop=0x8c608e28u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c608e2a; /* mov.l r5,@r4 */
L8c608e2a: *stop=0x8c608e2au;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c608e2c; /* mov.l r5,@(0x4,r4) */
L8c608e2c: *stop=0x8c608e2cu;r[5]=0u;goto L8c608e2e; /* mov #0x0,r5 */
L8c608e2e: *stop=0x8c608e2eu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;goto L8c608e30; /* mov.l r5,@(0x8,r4) */
L8c608e30: *stop=0x8c608e30u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[5],4);if(io_status)return io_status;goto L8c608e32; /* mov.l r5,@(0xc,r4) */
L8c608e32: *stop=0x8c608e32u;if(*pr!=entry_pr)return -3;*stop=0x8c608e34u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[5],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_frame_wait_8c64ddc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64ddc0;
L8c64ddc0: *stop=0x8c64ddc0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ddc2; /* mov.l r14,@-r15 */
L8c64ddc2: *stop=0x8c64ddc2u;r[14]=r[4];goto L8c64ddc4; /* mov r4,r14 */
L8c64ddc4: *stop=0x8c64ddc4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ddc6; /* mov.l r13,@-r15 */
L8c64ddc6: *stop=0x8c64ddc6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64ddc8; /* mov.l r12,@-r15 */
L8c64ddc8: *stop=0x8c64ddc8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ddca; /* sts.l PR,@-r15 */
L8c64ddca: *stop=0x8c64ddcau;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ddcc; /* mov.l @(0x10,r14),r0 */
L8c64ddcc: *stop=0x8c64ddccu;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c64ddce; /* tst #0x2,r0 */
L8c64ddce: take=(*sr&1u);*stop=0x8c64ddd0u;r[13]=1u;if(take)goto L8c64de8e;goto L8c64ddd2; /* bt/s 0x8c64de8e */
L8c64ddd2: *stop=0x8c64ddd2u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64ddd4; /* mov.l @(0x18,r14),r4 */
L8c64ddd4: *stop=0x8c64ddd4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64dee8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ddd6; /* mov.l 0x8c64dee8,r2 */
L8c64ddd6: *stop=0x8c64ddd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64deecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ddd8; /* mov.l 0x8c64deec,r3 */
L8c64ddd8: *stop=0x8c64ddd8u;r[4]>>=16;goto L8c64ddda; /* shlr16 r4 */
L8c64ddda: target=r[3];*pr=0x8c64dddeu;*stop=0x8c64dddcu;r[4]&=r[2];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ddde; /* jsr @r3 */
L8c64ddde: *stop=0x8c64dddeu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64dde0; /* cmp/eq #0x1,r0 */
L8c64dde0: *stop=0x8c64dde0u;r[4]=r[0];goto L8c64dde2; /* mov r0,r4 */
L8c64dde2: take=(*sr&1u);*stop=0x8c64dde4u;r[12]=4294967293u;if(take)goto L8c64de20;goto L8c64dde6; /* bt/s 0x8c64de20 */
L8c64dde6: *stop=0x8c64dde6u;r[0]=64u;goto L8c64dde8; /* mov #0x40,r0 */
L8c64dde8: *stop=0x8c64dde8u;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c64ddea; /* mov.w @(r0,r14),r3 */
L8c64ddea: *stop=0x8c64ddeau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64ddec; /* tst r3,r3 */
L8c64ddec: if((*sr&1u))goto L8c64ddf4;goto L8c64ddee; /* bt 0x8c64ddf4 */
L8c64ddee: *stop=0x8c64ddeeu;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c64ddf0; /* mov.w @(r0,r14),r3 */
L8c64ddf0: *stop=0x8c64ddf0u;r[3]+=4294967295u;goto L8c64ddf2; /* add #-0x1,r3 */
L8c64ddf2: *stop=0x8c64ddf2u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[3],2);if(io_status)return io_status;goto L8c64ddf4; /* mov.w r3,@(r0,r14) */
L8c64ddf4: *stop=0x8c64ddf4u;if(!valid_access(r[0]+r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c64ddf6; /* mov.w @(r0,r14),r2 */
L8c64ddf6: *stop=0x8c64ddf6u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64ddf8; /* tst r2,r2 */
L8c64ddf8: if(!(*sr&1u))goto L8c64deac;goto L8c64ddfa; /* bf 0x8c64deac */
L8c64ddfa: *stop=0x8c64ddfau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64def0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ddfc; /* mov.l 0x8c64def0,r2 */
L8c64ddfc: *stop=0x8c64ddfcu;r[4]=r[14];goto L8c64ddfe; /* mov r14,r4 */
L8c64ddfe: target=r[2];*pr=0x8c64de02u;*stop=0x8c64de00u;r[4]+=24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64de02; /* jsr @r2 */
L8c64de02: *stop=0x8c64de02u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64de04; /* mov.l @(0x10,r14),r3 */
L8c64de04: *stop=0x8c64de04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64def4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64de06; /* mov.l 0x8c64def4,r4 */
L8c64de06: *stop=0x8c64de06u;r[3]&=r[12];goto L8c64de08; /* and r12,r3 */
L8c64de08: *stop=0x8c64de08u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c64de0a; /* mov.l r3,@(0x10,r14) */
L8c64de0a: *stop=0x8c64de0au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64de0c; /* mov.l @r4,r3 */
L8c64de0c: *stop=0x8c64de0cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64de0e; /* mov.l @r14,r2 */
L8c64de0e: *stop=0x8c64de0eu;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c64de10; /* cmp/eq r2,r3 */
L8c64de10: if(!(*sr&1u))goto L8c64de16;goto L8c64de12; /* bf 0x8c64de16 */
L8c64de12: *stop=0x8c64de12u;r[0]=4294967295u;goto L8c64de14; /* mov #-0x1,r0 */
L8c64de14: *stop=0x8c64de14u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c64de16; /* mov.l r0,@r4 */
L8c64de16: *stop=0x8c64de16u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64de18; /* mov.l @(0x10,r14),r3 */
L8c64de18: *stop=0x8c64de18u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64de1a; /* tst r3,r3 */
L8c64de1a: if((*sr&1u))goto L8c64de9a;goto L8c64de1c; /* bt 0x8c64de9a */
L8c64de1c: *stop=0x8c64de1eu;goto L8c64deac; /* bra 0x8c64deac */
L8c64de20: *stop=0x8c64de20u;r[7]=0u;goto L8c64de22; /* mov #0x0,r7 */
L8c64de22: *stop=0x8c64de22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64def8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64de24; /* mov.l 0x8c64def8,r0 */
L8c64de24: *stop=0x8c64de24u;r[4]=r[7];goto L8c64de26; /* mov r7,r4 */
L8c64de26: *stop=0x8c64de26u;r[5]=r[7];goto L8c64de28; /* mov r7,r5 */
L8c64de28: *stop=0x8c64de28u;r[6]=r[7];goto L8c64de2a; /* mov r7,r6 */
L8c64de2a: *stop=0x8c64de2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64defcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64de2c; /* mov.l 0x8c64defc,r7 */
L8c64de2c: *stop=0x8c64de2eu;goto L8c64de42; /* bra 0x8c64de42 */
L8c64de30: *stop=0x8c64de30u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64de32; /* mov.l @r0,r2 */
L8c64de32: *stop=0x8c64de32u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64de34; /* mov.l @r14,r3 */
L8c64de34: *stop=0x8c64de34u;r[2]+=r[4];goto L8c64de36; /* add r4,r2 */
L8c64de36: *stop=0x8c64de36u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64de38; /* mov.l @r2,r1 */
L8c64de38: *stop=0x8c64de38u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c64de3a; /* cmp/eq r3,r1 */
L8c64de3a: take=!(*sr&1u);*stop=0x8c64de3cu;r[5]+=1u;if(take)goto L8c64de40;goto L8c64de3e; /* bf/s 0x8c64de40 */
L8c64de3e: *stop=0x8c64de3eu;r[6]+=1u;goto L8c64de40; /* add #0x1,r6 */
L8c64de40: *stop=0x8c64de40u;r[4]+=68u;goto L8c64de42; /* add #0x44,r4 */
L8c64de42: *stop=0x8c64de42u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64de44; /* mov.l @r7,r3 */
L8c64de44: *stop=0x8c64de44u;*sr=(*sr&~1u)|(r[5]>=r[3]);goto L8c64de46; /* cmp/hs r3,r5 */
L8c64de46: if(!(*sr&1u))goto L8c64de30;goto L8c64de48; /* bf 0x8c64de30 */
L8c64de48: *stop=0x8c64de48u;*sr=(*sr&~1u)|((int32_t)r[6]>(int32_t)r[13]);goto L8c64de4a; /* cmp/gt r13,r6 */
L8c64de4a: if(!(*sr&1u))goto L8c64de68;goto L8c64de4c; /* bf 0x8c64de68 */
L8c64de4c: *stop=0x8c64de4cu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64de4e; /* mov.l @(0x10,r14),r2 */
L8c64de4e: *stop=0x8c64de4eu;*sr=(*sr&~1u)|!(r[2]&r[13]);goto L8c64de50; /* tst r2,r13 */
L8c64de50: if(!(*sr&1u))goto L8c64deac;goto L8c64de52; /* bf 0x8c64deac */
L8c64de52: *stop=0x8c64de52u;r[0]=64u;goto L8c64de54; /* mov #0x40,r0 */
L8c64de54: *stop=0x8c64de54u;if(!valid_access(r[0]+r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c64de56; /* mov.w @(r0,r14),r2 */
L8c64de56: *stop=0x8c64de56u;r[2]+=4294967295u;goto L8c64de58; /* add #-0x1,r2 */
L8c64de58: *stop=0x8c64de58u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[2],2);if(io_status)return io_status;goto L8c64de5a; /* mov.w r2,@(r0,r14) */
L8c64de5a: *stop=0x8c64de5au;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c64de5c; /* mov.w @(r0,r14),r3 */
L8c64de5c: *stop=0x8c64de5cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64de5e; /* tst r3,r3 */
L8c64de5e: if(!(*sr&1u))goto L8c64deac;goto L8c64de60; /* bf 0x8c64deac */
L8c64de60: *stop=0x8c64de60u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64de62; /* mov.l @(0x10,r14),r1 */
L8c64de62: *stop=0x8c64de62u;r[1]&=r[12];goto L8c64de64; /* and r12,r1 */
L8c64de64: *stop=0x8c64de66u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[1],4);if(io_status)return io_status;goto L8c64de9a; /* bra 0x8c64de9a */
L8c64de68: *stop=0x8c64de68u;r[0]=64u;goto L8c64de6a; /* mov #0x40,r0 */
L8c64de6a: *stop=0x8c64de6au;if(!valid_access(r[0]+r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c64de6c; /* mov.w @(r0,r14),r2 */
L8c64de6c: *stop=0x8c64de6cu;r[2]+=4294967295u;goto L8c64de6e; /* add #-0x1,r2 */
L8c64de6e: *stop=0x8c64de6eu;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[2],2);if(io_status)return io_status;goto L8c64de70; /* mov.w r2,@(r0,r14) */
L8c64de70: *stop=0x8c64de70u;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c64de72; /* mov.w @(r0,r14),r3 */
L8c64de72: *stop=0x8c64de72u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64de74; /* tst r3,r3 */
L8c64de74: if(!(*sr&1u))goto L8c64deac;goto L8c64de76; /* bf 0x8c64deac */
L8c64de76: *stop=0x8c64de76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64def0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64de78; /* mov.l 0x8c64def0,r3 */
L8c64de78: *stop=0x8c64de78u;r[4]=r[14];goto L8c64de7a; /* mov r14,r4 */
L8c64de7a: target=r[3];*pr=0x8c64de7eu;*stop=0x8c64de7cu;r[4]+=24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64de7e; /* jsr @r3 */
L8c64de7e: *stop=0x8c64de7eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64de80; /* mov.l @(0x10,r14),r2 */
L8c64de80: *stop=0x8c64de80u;r[2]&=r[12];goto L8c64de82; /* and r12,r2 */
L8c64de82: *stop=0x8c64de82u;r[3]=r[2];goto L8c64de84; /* mov r2,r3 */
L8c64de84: *stop=0x8c64de84u;*sr=(*sr&~1u)|!(r[3]&r[13]);goto L8c64de86; /* tst r3,r13 */
L8c64de86: *stop=0x8c64de86u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c64de88; /* mov.l r2,@(0x10,r14) */
L8c64de88: if(!(*sr&1u))goto L8c64deac;goto L8c64de8a; /* bf 0x8c64deac */
L8c64de8a: *stop=0x8c64de8cu;goto L8c64de9a; /* bra 0x8c64de9a */
L8c64de8e: *stop=0x8c64de8eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64de90; /* mov.l @(0x10,r14),r1 */
L8c64de90: *stop=0x8c64de90u;*sr=(*sr&~1u)|!(r[13]&r[1]);goto L8c64de92; /* tst r13,r1 */
L8c64de92: if(!(*sr&1u))goto L8c64dea4;goto L8c64de94; /* bf 0x8c64dea4 */
L8c64de94: *stop=0x8c64de94u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64de96; /* mov.l @r14,r0 */
L8c64de96: *stop=0x8c64de96u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c64de98; /* cmp/eq #-0x1,r0 */
L8c64de98: if((*sr&1u))goto L8c64dea4;goto L8c64de9a; /* bt 0x8c64dea4 */
L8c64de9a: *stop=0x8c64de9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64df00u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64de9c; /* mov.l 0x8c64df00,r3 */
L8c64de9c: target=r[3];*pr=0x8c64dea0u;*stop=0x8c64de9eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64dea0; /* jsr @r3 */
L8c64dea0: *stop=0x8c64dea2u;goto L8c64deac; /* bra 0x8c64deac */
L8c64dea4: *stop=0x8c64dea4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64df04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64dea6; /* mov.l 0x8c64df04,r3 */
L8c64dea6: *stop=0x8c64dea6u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c64dea8; /* mov.l r13,@r3 */
L8c64dea8: *stop=0x8c64deaau;r[0]=4294967295u;goto L8c64deae; /* bra 0x8c64deae */
L8c64deac: *stop=0x8c64deacu;r[0]=1u;goto L8c64deae; /* mov #0x1,r0 */
L8c64deae: *stop=0x8c64deaeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64deb0; /* lds.l @r15+,PR */
L8c64deb0: *stop=0x8c64deb0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64deb2; /* mov.l @r15+,r12 */
L8c64deb2: *stop=0x8c64deb2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64deb4; /* mov.l @r15+,r13 */
L8c64deb4: *stop=0x8c64deb4u;if(*pr!=entry_pr)return -3;*stop=0x8c64deb6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_frame_wait_8c64c34c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64c34c;
L8c64c34c: *stop=0x8c64c34cu;r[3]=r[5];goto L8c64c34e; /* mov r5,r3 */
L8c64c34e: *stop=0x8c64c34eu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c64c350; /* shll r3 */
L8c64c350: *stop=0x8c64c350u;r[2]=r[5];goto L8c64c352; /* mov r5,r2 */
L8c64c352: *stop=0x8c64c352u;r[3]+=r[2];goto L8c64c354; /* add r2,r3 */
L8c64c354: *stop=0x8c64c354u;r[3]<<=2;goto L8c64c356; /* shll2 r3 */
L8c64c356: *stop=0x8c64c356u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c484u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64c358; /* mov.l 0x8c64c484,r2 */
L8c64c358: *stop=0x8c64c358u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64c35a; /* mov.l r14,@-r15 */
L8c64c35a: *stop=0x8c64c35au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c35c; /* mov.l @r4,r1 */
L8c64c35c: *stop=0x8c64c35cu;r[3]+=r[1];goto L8c64c35e; /* add r1,r3 */
L8c64c35e: *stop=0x8c64c35eu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c360; /* mov.l @(0x4,r3),r0 */
L8c64c360: *stop=0x8c64c360u;*sr=(*sr&~1u)|!(r[2]&r[0]);goto L8c64c362; /* tst r2,r0 */
L8c64c362: take=(*sr&1u);*stop=0x8c64c364u;r[14]=0u;if(take)goto L8c64c368;goto L8c64c366; /* bt/s 0x8c64c368 */
L8c64c366: *stop=0x8c64c366u;r[14]=1u;goto L8c64c368; /* mov #0x1,r14 */
L8c64c368: *stop=0x8c64c368u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64c488u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64c36a; /* mov.l 0x8c64c488,r1 */
L8c64c36a: *stop=0x8c64c36au;r[0]=r[14];goto L8c64c36c; /* mov r14,r0 */
L8c64c36c: *stop=0x8c64c36cu;r[0]<<=2;goto L8c64c36e; /* shll2 r0 */
L8c64c36e: *stop=0x8c64c36eu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64c370; /* mov.l @(r0,r1),r0 */
L8c64c370: target=r[0];*stop=0x8c64c372u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}
