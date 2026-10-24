/* Geometry projection, sampling and view proposals; explicit GBR/queue state.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
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
 if(a==0xffd8000cu && n==4u)return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_boot_wait_8c6050b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6050b0;
L8c6050b0: *stop=0x8c6050b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6050b2; /* mov.l r14,@-r15 */
L8c6050b2: *stop=0x8c6050b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6050b4; /* mov.l r13,@-r15 */
L8c6050b4: *stop=0x8c6050b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6050b6; /* mov.l r12,@-r15 */
L8c6050b6: *stop=0x8c6050b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6050b8; /* mov.l r11,@-r15 */
L8c6050b8: *stop=0x8c6050b8u;r[11]=0u;goto L8c6050ba; /* mov #0x0,r11 */
L8c6050ba: *stop=0x8c6050bau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6050bc; /* mov.l r10,@-r15 */
L8c6050bc: *stop=0x8c6050bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6050be; /* mov.l r9,@-r15 */
L8c6050be: *stop=0x8c6050beu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6050c0; /* sts.l PR,@-r15 */
L8c6050c0: *stop=0x8c6050c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051e0u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c6050c2; /* mov.l 0x8c6051e0,r12 */
L8c6050c2: *stop=0x8c6050c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051e4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c6050c4; /* mov.l 0x8c6051e4,r13 */
L8c6050c4: target=r[13];*pr=0x8c6050c8u;*stop=0x8c6050c6u;r[14]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6050c8; /* jsr @r13 */
L8c6050c8: *stop=0x8c6050c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6050ca; /* mov.l 0x8c6051e8,r2 */
L8c6050ca: *stop=0x8c6050cau;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[0],4);if(io_status)return io_status;goto L8c6050cc; /* mov.l r0,@r12 */
L8c6050cc: *stop=0x8c6050ccu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6050ce; /* mov.l @r2,r3 */
L8c6050ce: *stop=0x8c6050ceu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6050d0; /* tst r3,r3 */
L8c6050d0: if((*sr&1u))goto L8c605102;goto L8c6050d2; /* bt 0x8c605102 */
L8c6050d2: *stop=0x8c6050d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051f0u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c6050d4; /* mov.l 0x8c6051f0,r10 */
L8c6050d4: *stop=0x8c6050d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051ecu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c6050d6; /* mov.l 0x8c6051ec,r9 */
L8c6050d6: *stop=0x8c6050d8u;goto L8c6050ea; /* bra 0x8c6050ea */
L8c6050da: target=r[13];*pr=0x8c6050deu;*stop=0x8c6050dcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6050de; /* jsr @r13 */
L8c6050de: *stop=0x8c6050deu;r[5]=r[0];goto L8c6050e0; /* mov r0,r5 */
L8c6050e0: target=r[10];*pr=0x8c6050e4u;*stop=0x8c6050e2u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6050e4; /* jsr @r10 */
L8c6050e4: target=r[9];*pr=0x8c6050e8u;*stop=0x8c6050e6u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6050e8; /* jsr @r9 */
L8c6050e8: *stop=0x8c6050e8u;r[14]=r[0];goto L8c6050ea; /* mov r0,r14 */
L8c6050ea: *stop=0x8c6050eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6050ec; /* mov.l 0x8c6051f4,r3 */
L8c6050ec: *stop=0x8c6050ecu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6050ee; /* mov.l @r3,r2 */
L8c6050ee: *stop=0x8c6050eeu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6050f0; /* tst r2,r2 */
L8c6050f0: if((*sr&1u))goto L8c6050f8;goto L8c6050f2; /* bt 0x8c6050f8 */
L8c6050f2: *stop=0x8c6050f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6050f4; /* mov.l 0x8c6051f8,r2 */
L8c6050f4: *stop=0x8c6050f4u;*sr=(*sr&~1u)|(r[14]>=r[2]);goto L8c6050f6; /* cmp/hs r2,r14 */
L8c6050f6: if(!(*sr&1u))goto L8c6050da;goto L8c6050f8; /* bf 0x8c6050da */
L8c6050f8: *stop=0x8c6050f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6050fa; /* mov.l 0x8c6051f8,r3 */
L8c6050fa: *stop=0x8c6050fau;*sr=(*sr&~1u)|(r[14]>=r[3]);goto L8c6050fc; /* cmp/hs r3,r14 */
L8c6050fc: if((*sr&1u))goto L8c605102;goto L8c6050fe; /* bt 0x8c605102 */
L8c6050fe: target=0x8c60509au;*pr=0x8c605102u;*stop=0x8c605100u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c605102; /* bsr 0x8c60509a */
L8c605102: *stop=0x8c605102u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051fcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c605104; /* mov.l 0x8c6051fc,r3 */
L8c605104: *stop=0x8c605104u;r[2]=1u;goto L8c605106; /* mov #0x1,r2 */
L8c605106: *stop=0x8c605106u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051f4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c605108; /* mov.l 0x8c6051f4,r1 */
L8c605108: target=r[3];*pr=0x8c60510cu;*stop=0x8c60510au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60510c; /* jsr @r3 */
L8c60510c: *stop=0x8c60510cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60520cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c60510e; /* mov.l 0x8c60520c,r4 */
L8c60510e: *stop=0x8c60510eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c605208u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c605110; /* mov.l 0x8c605208,r2 */
L8c605110: *stop=0x8c605110u;{uint32_t loaded=rd(m,bus,&io_status,0x8c605200u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c605112; /* mov.l 0x8c605200,r0 */
L8c605112: *stop=0x8c605112u;{uint32_t loaded=rd(m,bus,&io_status,0x8c605204u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c605114; /* mov.l 0x8c605204,r3 */
L8c605114: *stop=0x8c605114u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[11],4);if(io_status)return io_status;goto L8c605116; /* mov.l r11,@r0 */
L8c605116: *stop=0x8c605116u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[11],4);if(io_status)return io_status;goto L8c605118; /* mov.l r11,@r3 */
L8c605118: *stop=0x8c605118u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[11],4);if(io_status)return io_status;goto L8c60511a; /* mov.l r11,@r2 */
L8c60511a: *stop=0x8c60511au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c60511c; /* mov.l @r4,r1 */
L8c60511c: *stop=0x8c60511cu;r[1]+=1u;goto L8c60511e; /* add #0x1,r1 */
L8c60511e: *stop=0x8c60511eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c605120; /* mov.l r1,@r4 */
L8c605120: *stop=0x8c605120u;{uint32_t loaded=rd(m,bus,&io_status,0x8c605210u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c605122; /* mov.l 0x8c605210,r1 */
L8c605122: target=r[1];*pr=0x8c605126u;*stop=0x8c605124u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c605126; /* jsr @r1 */
L8c605126: *stop=0x8c605126u;{uint32_t loaded=rd(m,bus,&io_status,0x8c605214u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c605128; /* mov.l 0x8c605214,r3 */
L8c605128: target=r[3];*pr=0x8c60512cu;*stop=0x8c60512au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60512c; /* jsr @r3 */
L8c60512c: target=r[13];*pr=0x8c605130u;*stop=0x8c60512eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c605130; /* jsr @r13 */
L8c605130: *stop=0x8c605130u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60521cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c605132; /* mov.l 0x8c60521c,r3 */
L8c605132: *stop=0x8c605132u;{uint32_t loaded=rd(m,bus,&io_status,0x8c605218u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c605134; /* mov.l 0x8c605218,r2 */
L8c605134: *stop=0x8c605134u;{uint32_t loaded=rd(m,bus,&io_status,0x8c605220u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c605136; /* mov.l 0x8c605220,r1 */
L8c605136: *stop=0x8c605136u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c605138; /* mov.l r0,@r2 */
L8c605138: *stop=0x8c605138u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c60513a; /* mov.l @r3,r4 */
L8c60513a: *stop=0x8c60513au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60513c; /* mov.l @r4,r2 */
L8c60513c: *stop=0x8c60513cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c60513e; /* mov.l r2,@r1 */
L8c60513e: *stop=0x8c60513eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c605224u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c605140; /* mov.l 0x8c605224,r2 */
L8c605140: *stop=0x8c605140u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c605142; /* mov.l @(0x4,r4),r0 */
L8c605142: *stop=0x8c605142u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c605144; /* mov.l r0,@r2 */
L8c605144: *stop=0x8c605144u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c605146; /* mov.l @(0x8,r4),r3 */
L8c605146: *stop=0x8c605146u;{uint32_t loaded=rd(m,bus,&io_status,0x8c605228u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c605148; /* mov.l 0x8c605228,r0 */
L8c605148: *stop=0x8c605148u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c60514a; /* mov.l r3,@r0 */
L8c60514a: *stop=0x8c60514au;{uint32_t loaded=rd(m,bus,&io_status,0x8c60522cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60514c; /* mov.l 0x8c60522c,r3 */
L8c60514c: *stop=0x8c60514cu;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c60514e; /* mov.l @(0xc,r4),r1 */
L8c60514e: *stop=0x8c60514eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c605150; /* mov.l r1,@r3 */
L8c605150: *stop=0x8c605150u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c605152; /* mov.l @(0x10,r4),r2 */
L8c605152: *stop=0x8c605152u;{uint32_t loaded=rd(m,bus,&io_status,0x8c605230u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c605154; /* mov.l 0x8c605230,r1 */
L8c605154: *stop=0x8c605154u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c605156; /* mov.l r2,@r1 */
L8c605156: *stop=0x8c605156u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c605158; /* lds.l @r15+,PR */
L8c605158: *stop=0x8c605158u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c60515a; /* mov.l @r15+,r9 */
L8c60515a: *stop=0x8c60515au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c60515c; /* mov.l @r15+,r10 */
L8c60515c: *stop=0x8c60515cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c60515e; /* mov.l @r15+,r11 */
L8c60515e: *stop=0x8c60515eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c605160; /* mov.l @r15+,r12 */
L8c605160: *stop=0x8c605160u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c605162; /* mov.l @r15+,r13 */
L8c605162: *stop=0x8c605162u;if(*pr!=entry_pr)return -3;*stop=0x8c605164u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_wait_8c60260e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60260e;
L8c60260e: *stop=0x8c60260eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60271cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602610; /* mov.l 0x8c60271c,r2 */
L8c602610: *stop=0x8c602610u;r[0]=4294967295u;goto L8c602612; /* mov #-0x1,r0 */
L8c602612: *stop=0x8c602612u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602614; /* mov.l @r2,r3 */
L8c602614: *stop=0x8c602614u;if(*pr!=entry_pr)return -3;*stop=0x8c602616u;r[0]-=r[3];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_wait_8c602618(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602618;
L8c602618: *stop=0x8c602618u;r[0]=r[5];goto L8c60261a; /* mov r5,r0 */
L8c60261a: *stop=0x8c60261au;if(*pr!=entry_pr)return -3;*stop=0x8c60261cu;r[0]-=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_wait_8c602652(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602652;
L8c602652: *stop=0x8c602652u;r[0]=127u;goto L8c602654; /* mov #0x7f,r0 */
L8c602654: *stop=0x8c602654u;r[2]=r[4];goto L8c602656; /* mov r4,r2 */
L8c602656: *stop=0x8c602656u;r[0]&=r[4];goto L8c602658; /* and r4,r0 */
L8c602658: *stop=0x8c602658u;r[5]=100u;goto L8c60265a; /* mov #0x64,r5 */
L8c60265a: *stop=0x8c60265au;*macl=r[5]*r[0];goto L8c60265c; /* mul.l r5,r0 */
L8c60265c: *stop=0x8c60265cu;r[3]=4294967289u;goto L8c60265e; /* mov #-0x7,r3 */
L8c60265e: *stop=0x8c60265eu;r[2]=(r[3]&0x80000000u)?((r[3]&31u)?r[2]>>((0u-r[3])&31u):0u):(r[2]<<(r[3]&31u));goto L8c602660; /* shld r3,r2 */
L8c602660: *stop=0x8c602660u;r[0]=*macl;goto L8c602662; /* sts MACL,r0 */
L8c602662: *stop=0x8c602662u;*macl=r[5]*r[2];goto L8c602664; /* mul.l r5,r2 */
L8c602664: *stop=0x8c602664u;r[0]=(r[3]&0x80000000u)?((r[3]&31u)?r[0]>>((0u-r[3])&31u):0u):(r[0]<<(r[3]&31u));goto L8c602666; /* shld r3,r0 */
L8c602666: *stop=0x8c602666u;r[2]=*macl;goto L8c602668; /* sts MACL,r2 */
L8c602668: *stop=0x8c602668u;if(*pr!=entry_pr)return -3;*stop=0x8c60266au;r[0]+=r[2];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_wait_8c60509a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60509a;
L8c60509a: *stop=0x8c60509au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6051d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60509c; /* mov.l 0x8c6051d8,r3 */
L8c60509c: *stop=0x8c60509cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c60509e; /* mov.l r14,@-r15 */
L8c60509e: *stop=0x8c60509eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6050a0; /* sts.l PR,@-r15 */
L8c6050a0: *stop=0x8c6050a0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[14]=loaded;}goto L8c6050a2; /* mov.l @r3,r14 */
L8c6050a2: *stop=0x8c6050a2u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6050a4; /* tst r14,r14 */
L8c6050a4: if((*sr&1u))goto L8c6050aa;goto L8c6050a6; /* bt 0x8c6050aa */
L8c6050a6: target=r[14];*pr=0x8c6050aau;*stop=0x8c6050a8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6050aa; /* jsr @r14 */
L8c6050aa: *stop=0x8c6050aau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6050ac; /* lds.l @r15+,PR */
L8c6050ac: *stop=0x8c6050acu;if(*pr!=entry_pr)return -3;*stop=0x8c6050aeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
