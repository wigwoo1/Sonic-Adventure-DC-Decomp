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
 if(n==4u && !(a&3u) && (a&0x1fffffffu)==0x005f6908u)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_renderer_input_tick_8c646024(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646024;
L8c646024: *stop=0x8c646024u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c646026; /* mov.l r14,@-r15 */
L8c646026: *stop=0x8c646026u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c646028; /* mov.l r13,@-r15 */
L8c646028: *stop=0x8c646028u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646148u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64602a; /* mov.l 0x8c646148,r13 */
L8c64602a: *stop=0x8c64602au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64614cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64602c; /* mov.l 0x8c64614c,r1 */
L8c64602c: *stop=0x8c64602cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64602e; /* sts.l PR,@-r15 */
L8c64602e: *stop=0x8c64602eu;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646030; /* mov.l @(0x8,r13),r3 */
L8c646030: *stop=0x8c646030u;r[3]+=1u;goto L8c646032; /* add #0x1,r3 */
L8c646032: *stop=0x8c646032u;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[3],4);if(io_status)return io_status;goto L8c646034; /* mov.l r3,@(0x8,r13) */
L8c646034: *stop=0x8c646034u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646036; /* mov.l @r1,r2 */
L8c646036: *stop=0x8c646036u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c646038; /* tst r2,r2 */
L8c646038: if((*sr&1u))goto L8c646056;goto L8c64603a; /* bt 0x8c646056 */
L8c64603a: *stop=0x8c64603au;{uint32_t loaded=rd(m,bus,&io_status,0x8c646150u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64603c; /* mov.l 0x8c646150,r3 */
L8c64603c: *stop=0x8c64603cu;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64603e; /* mov.l @(0x14,r13),r2 */
L8c64603e: *stop=0x8c64603eu;r[2]+=1u;goto L8c646040; /* add #0x1,r2 */
L8c646040: *stop=0x8c646040u;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[2],4);if(io_status)return io_status;goto L8c646042; /* mov.l r2,@(0x14,r13) */
L8c646042: *stop=0x8c646042u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[14]=loaded;}goto L8c646044; /* mov.l @r3,r14 */
L8c646044: *stop=0x8c646044u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c646046; /* tst r14,r14 */
L8c646046: if((*sr&1u))goto L8c64605e;goto L8c646048; /* bt 0x8c64605e */
L8c646048: *stop=0x8c646048u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64604a; /* mov.l @(0x8,r13),r6 */
L8c64604a: *stop=0x8c64604au;r[5]=4294967295u;goto L8c64604c; /* mov #-0x1,r5 */
L8c64604c: *stop=0x8c64604cu;r[7]=0u;goto L8c64604e; /* mov #0x0,r7 */
L8c64604e: target=r[14];*pr=0x8c646052u;*stop=0x8c646050u;r[4]=r[7];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646052; /* jsr @r14 */
L8c646052: *stop=0x8c646054u;goto L8c64605e; /* bra 0x8c64605e */
L8c646056: *stop=0x8c646056u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646058; /* lds.l @r15+,PR */
L8c646058: *stop=0x8c646058u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64605a; /* mov.l @r15+,r13 */
L8c64605a: *stop=0x8c64605cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,0x8c6463acu,m,f,sr,r,pr); /* bra 0x8c6463ac */
L8c64605e: *stop=0x8c64605eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646060; /* lds.l @r15+,PR */
L8c646060: *stop=0x8c646060u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c646062; /* mov.l @r15+,r13 */
L8c646062: *stop=0x8c646062u;if(*pr!=entry_pr)return -3;*stop=0x8c646064u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_renderer_input_tick_8c6463ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6463ac;
L8c6463ac: *stop=0x8c6463acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6463ae; /* mov.l r14,@-r15 */
L8c6463ae: *stop=0x8c6463aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6463b0; /* mov.l r13,@-r15 */
L8c6463b0: *stop=0x8c6463b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6463b2; /* mov.l r12,@-r15 */
L8c6463b2: *stop=0x8c6463b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6463d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6463b4; /* mov.l 0x8c6463d0,r2 */
L8c6463b4: *stop=0x8c6463b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6463b6; /* mov.l r11,@-r15 */
L8c6463b6: *stop=0x8c6463b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6463b8; /* mov.l r10,@-r15 */
L8c6463b8: *stop=0x8c6463b8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6463ba; /* sts.l PR,@-r15 */
L8c6463ba: *stop=0x8c6463bau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6463bc; /* mov.l @r2,r3 */
L8c6463bc: *stop=0x8c6463bcu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6463be; /* tst r3,r3 */
L8c6463be: if(!(*sr&1u))goto L8c6463d8;goto L8c6463c0; /* bf 0x8c6463d8 */
L8c6463c0: *stop=0x8c6463c2u;r[0]=0u;goto L8c64657c; /* bra 0x8c64657c */
L8c6463d8: *stop=0x8c6463d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6464d8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6463da; /* mov.l 0x8c6464d8,r4 */
L8c6463da: *stop=0x8c6463dau;r[11]=0u;goto L8c6463dc; /* mov #0x0,r11 */
L8c6463dc: *stop=0x8c6463dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6464dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6463de; /* mov.l 0x8c6464dc,r2 */
L8c6463de: *stop=0x8c6463deu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6463e0; /* mov.l @r4,r3 */
L8c6463e0: *stop=0x8c6463e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6464e0u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6463e2; /* mov.l 0x8c6464e0,r6 */
L8c6463e2: *stop=0x8c6463e2u;r[3]+=4294967295u;goto L8c6463e4; /* add #-0x1,r3 */
L8c6463e4: *stop=0x8c6463e4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6464cau,2);if(io_status)return io_status;r[10]=loaded;}goto L8c6463e6; /* mov.w 0x8c6464ca,r10 */
L8c6463e6: *stop=0x8c6463e6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6463e8; /* mov.l r3,@r2 */
L8c6463e8: *stop=0x8c6463e8u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[11],4);if(io_status)return io_status;goto L8c6463ea; /* mov.l r11,@r4 */
L8c6463ea: *stop=0x8c6463eau;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6463ec; /* mov.l @r6,r3 */
L8c6463ec: *stop=0x8c6463ecu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6463ee; /* tst r3,r3 */
L8c6463ee: if((*sr&1u))goto L8c646404;goto L8c6463f0; /* bt 0x8c646404 */
L8c6463f0: *stop=0x8c6463f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6464e8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6463f2; /* mov.l 0x8c6464e8,r4 */
L8c6463f2: *stop=0x8c6463f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6464e4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6463f4; /* mov.l 0x8c6464e4,r5 */
L8c6463f4: *stop=0x8c6463f4u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6463f6; /* mov.l @r5,r3 */
L8c6463f6: *stop=0x8c6463f6u;r[3]&=r[10];goto L8c6463f8; /* and r10,r3 */
L8c6463f8: *stop=0x8c6463f8u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6463fa; /* mov.l r3,@r4 */
L8c6463fa: *stop=0x8c6463fau;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6463fc; /* mov.l @r5,r1 */
L8c6463fc: *stop=0x8c6463fcu;r[1]>>=16;goto L8c6463fe; /* shlr16 r1 */
L8c6463fe: *stop=0x8c6463feu;r[1]=(uint32_t)(int32_t)(int16_t)r[1];goto L8c646400; /* exts.w r1,r1 */
L8c646400: *stop=0x8c646400u;r[1]&=r[10];goto L8c646402; /* and r10,r1 */
L8c646402: *stop=0x8c646402u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[1],4);if(io_status)return io_status;goto L8c646404; /* mov.l r1,@(0x4,r4) */
L8c646404: *stop=0x8c646404u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6464ecu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c646406; /* mov.l 0x8c6464ec,r4 */
L8c646406: *stop=0x8c646406u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[11],4);if(io_status)return io_status;goto L8c646408; /* mov.l r11,@r6 */
L8c646408: *stop=0x8c646408u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64640a; /* mov.l @r4,r2 */
L8c64640a: *stop=0x8c64640au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64640c; /* tst r2,r2 */
L8c64640c: if((*sr&1u))goto L8c646422;goto L8c64640e; /* bt 0x8c646422 */
L8c64640e: *stop=0x8c64640eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6464f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646410; /* mov.l 0x8c6464f0,r2 */
L8c646410: *stop=0x8c646410u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6464f4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c646412; /* mov.l 0x8c6464f4,r1 */
L8c646412: *stop=0x8c646412u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c646414; /* mov.l @r2,r3 */
L8c646414: *stop=0x8c646414u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c646416; /* mov.l r3,@r1 */
L8c646416: *stop=0x8c646416u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6464f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646418; /* mov.l 0x8c6464f8,r3 */
L8c646418: *stop=0x8c646418u;r[0]=4294967287u;goto L8c64641a; /* mov #-0x9,r0 */
L8c64641a: *stop=0x8c64641au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64641c; /* mov.l @r3,r2 */
L8c64641c: *stop=0x8c64641cu;r[2]&=r[0];goto L8c64641e; /* and r0,r2 */
L8c64641e: *stop=0x8c64641eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c646420; /* mov.l r2,@r3 */
L8c646420: *stop=0x8c646420u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[11],4);if(io_status)return io_status;goto L8c646422; /* mov.l r11,@r4 */
L8c646422: *stop=0x8c646422u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646504u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c646424; /* mov.l 0x8c646504,r14 */
L8c646424: *stop=0x8c646424u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6464ccu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c646426; /* mov.w 0x8c6464cc,r3 */
L8c646426: *stop=0x8c646426u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6464fcu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c646428; /* mov.l 0x8c6464fc,r12 */
L8c646428: *stop=0x8c646428u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646500u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64642a; /* mov.l 0x8c646500,r13 */
L8c64642a: *stop=0x8c64642au;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64642c; /* mov.l @r12,r2 */
L8c64642c: *stop=0x8c64642cu;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c64642e; /* tst r3,r2 */
L8c64642e: if((*sr&1u))goto L8c646522;goto L8c646430; /* bt 0x8c646522 */
L8c646430: *stop=0x8c646430u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6464ceu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c646432; /* mov.w 0x8c6464ce,r2 */
L8c646432: *stop=0x8c646432u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[1]=loaded;}goto L8c646434; /* mov.l @r12,r1 */
L8c646434: *stop=0x8c646434u;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c646436; /* tst r2,r1 */
L8c646436: if((*sr&1u))goto L8c646450;goto L8c646438; /* bt 0x8c646450 */
L8c646438: *stop=0x8c646438u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64643a; /* mov.l @(0x1c,r14),r1 */
L8c64643a: *stop=0x8c64643au;r[1]+=1u;goto L8c64643c; /* add #0x1,r1 */
L8c64643c: *stop=0x8c64643cu;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[1],4);if(io_status)return io_status;goto L8c64643e; /* mov.l r1,@(0x1c,r14) */
L8c64643e: *stop=0x8c64643eu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[0]=loaded;}goto L8c646440; /* mov.l @r13,r0 */
L8c646440: *stop=0x8c646440u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c646442; /* tst r0,r0 */
L8c646442: if((*sr&1u))goto L8c646450;goto L8c646444; /* bt 0x8c646450 */
L8c646444: *stop=0x8c646444u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[0]=loaded;}goto L8c646446; /* mov.l @r13,r0 */
L8c646446: *stop=0x8c646446u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[7]=loaded;}goto L8c646448; /* mov.l @r12,r7 */
L8c646448: *stop=0x8c646448u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64644a; /* mov.l @(0x8,r14),r6 */
L8c64644a: *stop=0x8c64644au;r[5]=4294967294u;goto L8c64644c; /* mov #-0x2,r5 */
L8c64644c: target=r[0];*pr=0x8c646450u;*stop=0x8c64644eu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646450; /* jsr @r0 */
L8c646450: *stop=0x8c646450u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6464d0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c646452; /* mov.w 0x8c6464d0,r3 */
L8c646452: *stop=0x8c646452u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646454; /* mov.l @r12,r2 */
L8c646454: *stop=0x8c646454u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c646456; /* tst r3,r2 */
L8c646456: if((*sr&1u))goto L8c646470;goto L8c646458; /* bt 0x8c646470 */
L8c646458: *stop=0x8c646458u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64645a; /* mov.l @(0x20,r14),r0 */
L8c64645a: *stop=0x8c64645au;r[0]+=1u;goto L8c64645c; /* add #0x1,r0 */
L8c64645c: *stop=0x8c64645cu;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[0],4);if(io_status)return io_status;goto L8c64645e; /* mov.l r0,@(0x20,r14) */
L8c64645e: *stop=0x8c64645eu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646460; /* mov.l @r13,r2 */
L8c646460: *stop=0x8c646460u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c646462; /* tst r2,r2 */
L8c646462: if((*sr&1u))goto L8c646470;goto L8c646464; /* bt 0x8c646470 */
L8c646464: *stop=0x8c646464u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646466; /* mov.l @r13,r2 */
L8c646466: *stop=0x8c646466u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[7]=loaded;}goto L8c646468; /* mov.l @r12,r7 */
L8c646468: *stop=0x8c646468u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64646a; /* mov.l @(0x8,r14),r6 */
L8c64646a: *stop=0x8c64646au;r[5]=4294967293u;goto L8c64646c; /* mov #-0x3,r5 */
L8c64646c: target=r[2];*pr=0x8c646470u;*stop=0x8c64646eu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646470; /* jsr @r2 */
L8c646470: *stop=0x8c646470u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6464d2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c646472; /* mov.w 0x8c6464d2,r3 */
L8c646472: *stop=0x8c646472u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646474; /* mov.l @r12,r2 */
L8c646474: *stop=0x8c646474u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c646476; /* tst r3,r2 */
L8c646476: if((*sr&1u))goto L8c646490;goto L8c646478; /* bt 0x8c646490 */
L8c646478: *stop=0x8c646478u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64647a; /* mov.l @(0x24,r14),r0 */
L8c64647a: *stop=0x8c64647au;r[0]+=1u;goto L8c64647c; /* add #0x1,r0 */
L8c64647c: *stop=0x8c64647cu;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[0],4);if(io_status)return io_status;goto L8c64647e; /* mov.l r0,@(0x24,r14) */
L8c64647e: *stop=0x8c64647eu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646480; /* mov.l @r13,r2 */
L8c646480: *stop=0x8c646480u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c646482; /* tst r2,r2 */
L8c646482: if((*sr&1u))goto L8c646490;goto L8c646484; /* bt 0x8c646490 */
L8c646484: *stop=0x8c646484u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646486; /* mov.l @r13,r2 */
L8c646486: *stop=0x8c646486u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[7]=loaded;}goto L8c646488; /* mov.l @r12,r7 */
L8c646488: *stop=0x8c646488u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64648a; /* mov.l @(0x8,r14),r6 */
L8c64648a: *stop=0x8c64648au;r[5]=4294967292u;goto L8c64648c; /* mov #-0x4,r5 */
L8c64648c: target=r[2];*pr=0x8c646490u;*stop=0x8c64648eu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646490; /* jsr @r2 */
L8c646490: *stop=0x8c646490u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6464d4u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c646492; /* mov.w 0x8c6464d4,r3 */
L8c646492: *stop=0x8c646492u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646494; /* mov.l @r12,r2 */
L8c646494: *stop=0x8c646494u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c646496; /* tst r3,r2 */
L8c646496: if((*sr&1u))goto L8c6464b0;goto L8c646498; /* bt 0x8c6464b0 */
L8c646498: *stop=0x8c646498u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64649a; /* mov.l @(0x28,r14),r0 */
L8c64649a: *stop=0x8c64649au;r[0]+=1u;goto L8c64649c; /* add #0x1,r0 */
L8c64649c: *stop=0x8c64649cu;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[0],4);if(io_status)return io_status;goto L8c64649e; /* mov.l r0,@(0x28,r14) */
L8c64649e: *stop=0x8c64649eu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6464a0; /* mov.l @r13,r2 */
L8c6464a0: *stop=0x8c6464a0u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6464a2; /* tst r2,r2 */
L8c6464a2: if((*sr&1u))goto L8c6464b0;goto L8c6464a4; /* bt 0x8c6464b0 */
L8c6464a4: *stop=0x8c6464a4u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6464a6; /* mov.l @r13,r2 */
L8c6464a6: *stop=0x8c6464a6u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[7]=loaded;}goto L8c6464a8; /* mov.l @r12,r7 */
L8c6464a8: *stop=0x8c6464a8u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6464aa; /* mov.l @(0x8,r14),r6 */
L8c6464aa: *stop=0x8c6464aau;r[5]=4294967291u;goto L8c6464ac; /* mov #-0x5,r5 */
L8c6464ac: target=r[2];*pr=0x8c6464b0u;*stop=0x8c6464aeu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6464b0; /* jsr @r2 */
L8c6464b0: *stop=0x8c6464b0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6464ccu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6464b2; /* mov.w 0x8c6464cc,r3 */
L8c6464b2: target=0x8c646654u;*pr=0x8c6464b6u;*stop=0x8c6464b4u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6464b6; /* bsr 0x8c646654 */
L8c6464b6: *stop=0x8c6464b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646508u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6464b8; /* mov.l 0x8c646508,r1 */
L8c6464b8: target=r[1];*pr=0x8c6464bcu;*stop=0x8c6464bau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6464bc; /* jsr @r1 */
L8c6464bc: *stop=0x8c6464bcu;r[12]=r[0];goto L8c6464be; /* mov r0,r12 */
L8c6464be: *stop=0x8c6464beu;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c6464c0; /* cmp/pz r12 */
L8c6464c0: if(!(*sr&1u))goto L8c64650c;goto L8c6464c2; /* bf 0x8c64650c */
L8c6464c2: target=0x8c6461aeu;*pr=0x8c6464c6u;*stop=0x8c6464c4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6464c6; /* bsr 0x8c6461ae */
L8c6464c6: *stop=0x8c6464c8u;goto L8c64657a; /* bra 0x8c64657a */
L8c64650c: *stop=0x8c64650cu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64650e; /* mov.l @r13,r2 */
L8c64650e: *stop=0x8c64650eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c646510; /* tst r2,r2 */
L8c646510: if((*sr&1u))goto L8c64657a;goto L8c646512; /* bt 0x8c64657a */
L8c646512: *stop=0x8c646512u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c646514; /* mov.l @(0x8,r14),r6 */
L8c646514: *stop=0x8c646514u;r[5]=4294967290u;goto L8c646516; /* mov #-0x6,r5 */
L8c646516: *stop=0x8c646516u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c646518; /* mov.l @r13,r3 */
L8c646518: *stop=0x8c646518u;r[7]=r[12];goto L8c64651a; /* mov r12,r7 */
L8c64651a: target=r[3];*pr=0x8c64651eu;*stop=0x8c64651cu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64651e; /* jsr @r3 */
L8c64651e: *stop=0x8c646520u;goto L8c64657a; /* bra 0x8c64657a */
L8c646522: *stop=0x8c646522u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646618u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c646524; /* mov.l 0x8c646618,r1 */
L8c646524: target=r[1];*pr=0x8c646528u;*stop=0x8c646526u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646528; /* jsr @r1 */
L8c646528: *stop=0x8c646528u;r[12]=r[0];goto L8c64652a; /* mov r0,r12 */
L8c64652a: *stop=0x8c64652au;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c64652c; /* cmp/pz r12 */
L8c64652c: if(!(*sr&1u))goto L8c64655e;goto L8c64652e; /* bf 0x8c64655e */
L8c64652e: *stop=0x8c64652eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c646620u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c646530; /* mov.l 0x8c646620,r4 */
L8c646530: *stop=0x8c646530u;r[1]=r[14];goto L8c646532; /* mov r14,r1 */
L8c646532: *stop=0x8c646532u;r[1]+=52u;goto L8c646534; /* add #0x34,r1 */
L8c646534: *stop=0x8c646534u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64661cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646536; /* mov.l 0x8c64661c,r2 */
L8c646536: *stop=0x8c646536u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[11],4);if(io_status)return io_status;goto L8c646538; /* mov.l r11,@r2 */
L8c646538: *stop=0x8c646538u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64653a; /* mov.l @r4,r3 */
L8c64653a: *stop=0x8c64653au;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c64653c; /* shll r3 */
L8c64653c: *stop=0x8c64653cu;r[3]+=r[1];goto L8c64653e; /* add r1,r3 */
L8c64653e: *stop=0x8c64653eu;r[1]=r[14];goto L8c646540; /* mov r14,r1 */
L8c646540: *stop=0x8c646540u;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[11],2);if(io_status)return io_status;goto L8c646542; /* mov.w r11,@r3 */
L8c646542: *stop=0x8c646542u;r[1]+=44u;goto L8c646544; /* add #0x2c,r1 */
L8c646544: *stop=0x8c646544u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c646546; /* mov.l @r4,r3 */
L8c646546: *stop=0x8c646546u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c646548; /* shll r3 */
L8c646548: *stop=0x8c646548u;r[3]+=r[1];goto L8c64654a; /* add r1,r3 */
L8c64654a: *stop=0x8c64654au;{uint32_t loaded=rd(m,bus,&io_status,0x8c646624u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64654c; /* mov.l 0x8c646624,r1 */
L8c64654c: *stop=0x8c64654cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64654e; /* mov.l @r1,r0 */
L8c64654e: *stop=0x8c64654eu;r[0]&=r[10];goto L8c646550; /* and r10,r0 */
L8c646550: *stop=0x8c646550u;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c646552; /* mov.w r0,@r3 */
L8c646552: target=0x8c6461aeu;*pr=0x8c646556u;*stop=0x8c646554u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646556; /* bsr 0x8c6461ae */
L8c646556: *stop=0x8c646556u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646558; /* mov.l @r14,r2 */
L8c646558: *stop=0x8c646558u;r[2]+=1u;goto L8c64655a; /* add #0x1,r2 */
L8c64655a: *stop=0x8c64655cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c64657a; /* bra 0x8c64657a */
L8c64655e: target=0x8c646654u;*pr=0x8c646562u;*stop=0x8c646560u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646562; /* bsr 0x8c646654 */
L8c646562: *stop=0x8c646562u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646564; /* mov.l @(0x10,r14),r3 */
L8c646564: *stop=0x8c646564u;r[3]+=1u;goto L8c646566; /* add #0x1,r3 */
L8c646566: *stop=0x8c646566u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c646568; /* mov.l r3,@(0x10,r14) */
L8c646568: *stop=0x8c646568u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64656a; /* mov.l @r13,r2 */
L8c64656a: *stop=0x8c64656au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64656c; /* tst r2,r2 */
L8c64656c: if((*sr&1u))goto L8c64657a;goto L8c64656e; /* bt 0x8c64657a */
L8c64656e: *stop=0x8c64656eu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c646570; /* mov.l @(0x8,r14),r6 */
L8c646570: *stop=0x8c646570u;r[5]=4294967289u;goto L8c646572; /* mov #-0x7,r5 */
L8c646572: *stop=0x8c646572u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c646574; /* mov.l @r13,r2 */
L8c646574: *stop=0x8c646574u;r[7]=r[12];goto L8c646576; /* mov r12,r7 */
L8c646576: target=r[2];*pr=0x8c64657au;*stop=0x8c646578u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64657a; /* jsr @r2 */
L8c64657a: *stop=0x8c64657au;r[0]=r[12];goto L8c64657c; /* mov r12,r0 */
L8c64657c: *stop=0x8c64657cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64657e; /* lds.l @r15+,PR */
L8c64657e: *stop=0x8c64657eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c646580; /* mov.l @r15+,r10 */
L8c646580: *stop=0x8c646580u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c646582; /* mov.l @r15+,r11 */
L8c646582: *stop=0x8c646582u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c646584; /* mov.l @r15+,r12 */
L8c646584: *stop=0x8c646584u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c646586; /* mov.l @r15+,r13 */
L8c646586: *stop=0x8c646586u;if(*pr!=entry_pr)return -3;*stop=0x8c646588u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
