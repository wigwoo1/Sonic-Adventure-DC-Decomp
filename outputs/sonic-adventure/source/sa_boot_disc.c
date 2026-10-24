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
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_boot_disc_8c6450ea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6450ea;
L8c6450ea: *stop=0x8c6450eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6450ec; /* mov.l r14,@-r15 */
L8c6450ec: *stop=0x8c6450ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6450ee; /* sts.l PR,@-r15 */
L8c6450ee: *stop=0x8c6450eeu;r[15]+=4294967280u;goto L8c6450f0; /* add #-0x10,r15 */
L8c6450f0: *stop=0x8c6450f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6451f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6450f2; /* mov.l 0x8c6451f4,r3 */
L8c6450f2: *stop=0x8c6450f2u;r[14]=r[5];goto L8c6450f4; /* mov r5,r14 */
L8c6450f4: target=r[3];*pr=0x8c6450f8u;*stop=0x8c6450f6u;r[5]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6450f8; /* jsr @r3 */
L8c6450f8: *stop=0x8c6450f8u;r[4]=r[0];goto L8c6450fa; /* mov r0,r4 */
L8c6450fa: *stop=0x8c6450fau;r[0]=r[4];goto L8c6450fc; /* mov r4,r0 */
L8c6450fc: *stop=0x8c6450fcu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6450fe; /* mov.l @(0x8,r15),r5 */
L8c6450fe: *stop=0x8c6450feu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c645100; /* cmp/eq #0x0,r0 */
L8c645100: if((*sr&1u))goto L8c64511a;goto L8c645102; /* bt 0x8c64511a */
L8c645102: *stop=0x8c645102u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c645104; /* cmp/eq #0x1,r0 */
L8c645104: if((*sr&1u))goto L8c64511e;goto L8c645106; /* bt 0x8c64511e */
L8c645106: *stop=0x8c645106u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c645108; /* cmp/eq #0x2,r0 */
L8c645108: if((*sr&1u))goto L8c645128;goto L8c64510a; /* bt 0x8c645128 */
L8c64510a: *stop=0x8c64510au;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c64510c; /* cmp/eq #0x3,r0 */
L8c64510c: if((*sr&1u))goto L8c64512e;goto L8c64510e; /* bt 0x8c64512e */
L8c64510e: *stop=0x8c64510eu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c645110; /* cmp/eq #0x4,r0 */
L8c645110: if((*sr&1u))goto L8c645132;goto L8c645112; /* bt 0x8c645132 */
L8c645112: *stop=0x8c645112u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c645114; /* cmp/eq #-0x1,r0 */
L8c645114: if((*sr&1u))goto L8c645136;goto L8c645116; /* bt 0x8c645136 */
L8c645116: *stop=0x8c645118u;goto L8c64519a; /* bra 0x8c64519a */
L8c64511a: *stop=0x8c64511cu;r[4]=0u;goto L8c64519a; /* bra 0x8c64519a */
L8c64511e: *stop=0x8c64511eu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c645120; /* mov.l r5,@r14 */
L8c645120: *stop=0x8c645120u;r[4]=4u;goto L8c645122; /* mov #0x4,r4 */
L8c645122: *stop=0x8c645122u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645124; /* mov.l @(0xc,r15),r2 */
L8c645124: *stop=0x8c645126u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c64519a; /* bra 0x8c64519a */
L8c645128: *stop=0x8c645128u;r[4]=1u;goto L8c64512a; /* mov #0x1,r4 */
L8c64512a: *stop=0x8c64512cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c64519a; /* bra 0x8c64519a */
L8c64512e: *stop=0x8c645130u;r[4]=65u;goto L8c64519a; /* bra 0x8c64519a */
L8c645132: *stop=0x8c645134u;r[4]=67u;goto L8c64519a; /* bra 0x8c64519a */
L8c645136: *stop=0x8c645136u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645138; /* mov.l @(0x4,r15),r3 */
L8c645138: *stop=0x8c645138u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c64513a; /* mov.l r3,@(0x4,r14) */
L8c64513a: *stop=0x8c64513au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64513c; /* mov.l @r15,r0 */
L8c64513c: *stop=0x8c64513cu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c64513e; /* cmp/eq #0x0,r0 */
L8c64513e: take=(*sr&1u);*stop=0x8c645140u;r[4]=5u;if(take)goto L8c64516a;goto L8c645142; /* bt/s 0x8c64516a */
L8c645142: *stop=0x8c645142u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c645144; /* cmp/eq #0x1,r0 */
L8c645144: if((*sr&1u))goto L8c645170;goto L8c645146; /* bt 0x8c645170 */
L8c645146: *stop=0x8c645146u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c645148; /* cmp/eq #0x2,r0 */
L8c645148: if((*sr&1u))goto L8c645174;goto L8c64514a; /* bt 0x8c645174 */
L8c64514a: *stop=0x8c64514au;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c64514c; /* cmp/eq #0x3,r0 */
L8c64514c: if((*sr&1u))goto L8c645178;goto L8c64514e; /* bt 0x8c645178 */
L8c64514e: *stop=0x8c64514eu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c645150; /* cmp/eq #0x4,r0 */
L8c645150: if((*sr&1u))goto L8c64517c;goto L8c645152; /* bt 0x8c64517c */
L8c645152: *stop=0x8c645152u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c645154; /* cmp/eq #0x5,r0 */
L8c645154: if((*sr&1u))goto L8c645180;goto L8c645156; /* bt 0x8c645180 */
L8c645156: *stop=0x8c645156u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c645158; /* cmp/eq #0x6,r0 */
L8c645158: if((*sr&1u))goto L8c645184;goto L8c64515a; /* bt 0x8c645184 */
L8c64515a: *stop=0x8c64515au;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c64515c; /* cmp/eq #0x7,r0 */
L8c64515c: if((*sr&1u))goto L8c645188;goto L8c64515e; /* bt 0x8c645188 */
L8c64515e: *stop=0x8c64515eu;*sr=(*sr&~1u)|(r[0]==0x0000000bu);goto L8c645160; /* cmp/eq #0xb,r0 */
L8c645160: if((*sr&1u))goto L8c64518c;goto L8c645162; /* bt 0x8c64518c */
L8c645162: *stop=0x8c645162u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c645164; /* cmp/eq #0x10,r0 */
L8c645164: if((*sr&1u))goto L8c645190;goto L8c645166; /* bt 0x8c645190 */
L8c645166: *stop=0x8c645168u;goto L8c645196; /* bra 0x8c645196 */
L8c64516a: *stop=0x8c64516au;r[3]=4294967269u;goto L8c64516c; /* mov #-0x1b,r3 */
L8c64516c: *stop=0x8c64516eu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c64519a; /* bra 0x8c64519a */
L8c645170: *stop=0x8c645172u;r[1]=4294967268u;goto L8c645192; /* bra 0x8c645192 */
L8c645174: *stop=0x8c645176u;r[2]=4294967267u;goto L8c645198; /* bra 0x8c645198 */
L8c645178: *stop=0x8c64517au;r[1]=4294967266u;goto L8c645192; /* bra 0x8c645192 */
L8c64517c: *stop=0x8c64517eu;r[2]=4294967265u;goto L8c645198; /* bra 0x8c645198 */
L8c645180: *stop=0x8c645182u;r[1]=4294967264u;goto L8c645192; /* bra 0x8c645192 */
L8c645184: *stop=0x8c645186u;r[2]=4294967263u;goto L8c645198; /* bra 0x8c645198 */
L8c645188: *stop=0x8c64518au;r[1]=4294967262u;goto L8c645192; /* bra 0x8c645192 */
L8c64518c: *stop=0x8c64518eu;r[2]=4294967261u;goto L8c645198; /* bra 0x8c645198 */
L8c645190: *stop=0x8c645190u;r[1]=4294967260u;goto L8c645192; /* mov #-0x24,r1 */
L8c645192: *stop=0x8c645194u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c64519a; /* bra 0x8c64519a */
L8c645196: *stop=0x8c645196u;r[2]=4294967270u;goto L8c645198; /* mov #-0x1a,r2 */
L8c645198: *stop=0x8c645198u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c64519a; /* mov.l r2,@r14 */
L8c64519a: *stop=0x8c64519au;r[0]=r[4];goto L8c64519c; /* mov r4,r0 */
L8c64519c: *stop=0x8c64519cu;r[15]+=16u;goto L8c64519e; /* add #0x10,r15 */
L8c64519e: *stop=0x8c64519eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6451a0; /* lds.l @r15+,PR */
L8c6451a0: *stop=0x8c6451a0u;if(*pr!=entry_pr)return -3;*stop=0x8c6451a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_disc_8c10d94c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d94c;
L8c10d94c: *stop=0x8c10d94cu;r[6]=0u;goto L8c10d94e; /* mov #0x0,r6 */
L8c10d94e: *stop=0x8c10d94eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d958u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d950; /* mov.l 0x8c10d958,r7 */
L8c10d950: *stop=0x8c10d950u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d95cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d952; /* mov.l 0x8c10d95c,r0 */
L8c10d952: *stop=0x8c10d952u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d954; /* mov.l @r0,r0 */
L8c10d954: target=r[0];*stop=0x8c10d956u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_boot_disc_8c5fc0d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c5fc0d8;
L8c5fc0d8: *stop=0x8c5fc0d8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c5fc0da; /* mov.l r14,@-r15 */
L8c5fc0da: *stop=0x8c5fc0dau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c5fc0dc; /* mov.l r13,@-r15 */
L8c5fc0dc: *stop=0x8c5fc0dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c5fc0de; /* mov.l r12,@-r15 */
L8c5fc0de: *stop=0x8c5fc0deu;r[15]+=4294967288u;goto L8c5fc0e0; /* add #-0x8,r15 */
L8c5fc0e0: *stop=0x8c5fc0e0u;r[13]=r[5];goto L8c5fc0e2; /* mov r5,r13 */
L8c5fc0e2: *stop=0x8c5fc0e2u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c5fc0e4; /* tst r13,r13 */
L8c5fc0e4: take=!(*sr&1u);*stop=0x8c5fc0e6u;r[12]=r[4];if(take)goto L8c5fc0ee;goto L8c5fc0e8; /* bf/s 0x8c5fc0ee */
L8c5fc0e8: *stop=0x8c5fc0e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc118u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc0ea; /* mov.l 0x8c5fc118,r3 */
L8c5fc0ea: target=r[3];*stop=0x8c5fc0ecu;if(target==0x8c5fc32cu)goto L8c5fc32c;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c5fc0ee: *stop=0x8c5fc0eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc120u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc0f0; /* mov.l 0x8c5fc120,r2 */
L8c5fc0f0: *stop=0x8c5fc0f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc11cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c5fc0f2; /* mov.l 0x8c5fc11c,r14 */
L8c5fc0f2: *stop=0x8c5fc0f2u;*sr=(*sr&~1u)|(r[13]>=r[2]);goto L8c5fc0f4; /* cmp/hs r2,r13 */
L8c5fc0f4: take=!(*sr&1u);*stop=0x8c5fc0f6u;r[14]&=r[12];if(take)goto L8c5fc0fe;goto L8c5fc0f8; /* bf/s 0x8c5fc0fe */
L8c5fc0f8: *stop=0x8c5fc0f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc0fa; /* mov.l 0x8c5fc124,r3 */
L8c5fc0fa: target=r[3];*stop=0x8c5fc0fcu;if(target==0x8c5fc13eu)goto L8c5fc13e;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c5fc0fe: *stop=0x8c5fc0feu;r[5]=31u;goto L8c5fc100; /* mov #0x1f,r5 */
L8c5fc100: *stop=0x8c5fc100u;r[4]=r[14];goto L8c5fc102; /* mov r14,r4 */
L8c5fc102: *stop=0x8c5fc102u;r[5]&=r[12];goto L8c5fc104; /* and r12,r5 */
L8c5fc104: *stop=0x8c5fc104u;r[5]+=r[13];goto L8c5fc106; /* add r13,r5 */
L8c5fc106: *stop=0x8c5fc108u;goto L8c5fc128; /* bra 0x8c5fc128 */
L8c5fc128: *stop=0x8c5fc128u;r[5]+=31u;goto L8c5fc12a; /* add #0x1f,r5 */
L8c5fc12a: *stop=0x8c5fc12au;r[5]>>=2;goto L8c5fc12c; /* shlr2 r5 */
L8c5fc12c: *stop=0x8c5fc12cu;r[5]>>=2;goto L8c5fc12e; /* shlr2 r5 */
L8c5fc12e: *stop=0x8c5fc12eu;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c5fc130; /* shlr r5 */
L8c5fc130: *stop=0x8c5fc130u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],0u);if(status)return status;goto L8c5fc132; /* ocbi @r4 */
L8c5fc132: *stop=0x8c5fc132u;r[5]--;*sr=(*sr&~1u)|(!r[5]);goto L8c5fc134; /* dt r5 */
L8c5fc134: take=!(*sr&1u);*stop=0x8c5fc136u;r[4]+=32u;if(take)goto L8c5fc130;goto L8c5fc138; /* bf/s 0x8c5fc130 */
L8c5fc138: *stop=0x8c5fc138u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc158u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc13a; /* mov.l 0x8c5fc158,r2 */
L8c5fc13a: target=r[2];*stop=0x8c5fc13cu;if(target==0x8c5fc32cu)goto L8c5fc32c;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c5fc13e: *stop=0x8c5fc13eu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5fc140; /* stc SR,r0 */
L8c5fc140: *stop=0x8c5fc140u;r[3]=sx(m,bus,&io_status,0x8c5fc156u,2);if(io_status)return io_status;goto L8c5fc142; /* mov.w 0x8c5fc156,r3 */
L8c5fc142: *stop=0x8c5fc142u;r[0]>>=2;goto L8c5fc144; /* shlr2 r0 */
L8c5fc144: *stop=0x8c5fc144u;r[0]>>=2;goto L8c5fc146; /* shlr2 r0 */
L8c5fc146: *stop=0x8c5fc146u;r[0]&=0xfu;goto L8c5fc148; /* and #0xf,r0 */
L8c5fc148: *stop=0x8c5fc148u;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c5fc14a; /* mov.w r0,@r15 */
L8c5fc14a: *stop=0x8c5fc14au;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5fc14c; /* stc SR,r0 */
L8c5fc14c: *stop=0x8c5fc14cu;r[0]&=r[3];goto L8c5fc14e; /* and r3,r0 */
L8c5fc14e: *stop=0x8c5fc14eu;r[0]|=0xf0u;goto L8c5fc150; /* or #0xf0,r0 */
L8c5fc150: *stop=0x8c5fc150u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c5fc152; /* ldc r0,SR */
L8c5fc152: *stop=0x8c5fc154u;goto L8c5fc15c; /* bra 0x8c5fc15c */
L8c5fc15c: *stop=0x8c5fc15cu;r[0]=0x8c5fc16cu;goto L8c5fc15e; /* mova 0x8c5fc16c,r0 */
L8c5fc15e: *stop=0x8c5fc15eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc168u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c5fc160; /* mov.l 0x8c5fc168,r1 */
L8c5fc160: *stop=0x8c5fc160u;r[1]|=r[0];goto L8c5fc162; /* or r0,r1 */
L8c5fc162: target=r[1];*stop=0x8c5fc164u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r1 */
L8c5fc32c: *stop=0x8c5fc32cu;r[15]+=8u;goto L8c5fc32e; /* add #0x8,r15 */
L8c5fc32e: *stop=0x8c5fc32eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c5fc330; /* mov.l @r15+,r12 */
L8c5fc330: *stop=0x8c5fc330u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c5fc332; /* mov.l @r15+,r13 */
L8c5fc332: *stop=0x8c5fc332u;if(*pr!=entry_pr)return -3;*stop=0x8c5fc334u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_disc_8c5fc336(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c5fc336;
L8c5fc336: *stop=0x8c5fc336u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c5fc338; /* mov.l r14,@-r15 */
L8c5fc338: *stop=0x8c5fc338u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c5fc33a; /* mov.l r13,@-r15 */
L8c5fc33a: *stop=0x8c5fc33au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c5fc33c; /* mov.l r12,@-r15 */
L8c5fc33c: *stop=0x8c5fc33cu;r[15]+=4294967288u;goto L8c5fc33e; /* add #-0x8,r15 */
L8c5fc33e: *stop=0x8c5fc33eu;r[13]=r[5];goto L8c5fc340; /* mov r5,r13 */
L8c5fc340: *stop=0x8c5fc340u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c5fc342; /* tst r13,r13 */
L8c5fc342: take=!(*sr&1u);*stop=0x8c5fc344u;r[12]=r[4];if(take)goto L8c5fc34c;goto L8c5fc346; /* bf/s 0x8c5fc34c */
L8c5fc346: *stop=0x8c5fc346u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc36cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc348; /* mov.l 0x8c5fc36c,r3 */
L8c5fc348: target=r[3];*stop=0x8c5fc34au;if(target==0x8c5fc580u)goto L8c5fc580;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c5fc34c: *stop=0x8c5fc34cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc374u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc34e; /* mov.l 0x8c5fc374,r2 */
L8c5fc34e: *stop=0x8c5fc34eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc370u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c5fc350; /* mov.l 0x8c5fc370,r14 */
L8c5fc350: *stop=0x8c5fc350u;*sr=(*sr&~1u)|(r[13]>=r[2]);goto L8c5fc352; /* cmp/hs r2,r13 */
L8c5fc352: take=!(*sr&1u);*stop=0x8c5fc354u;r[14]&=r[12];if(take)goto L8c5fc35c;goto L8c5fc356; /* bf/s 0x8c5fc35c */
L8c5fc356: *stop=0x8c5fc356u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc378u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc358; /* mov.l 0x8c5fc378,r3 */
L8c5fc358: target=r[3];*stop=0x8c5fc35au;if(target==0x8c5fc392u)goto L8c5fc392;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c5fc35c: *stop=0x8c5fc35cu;r[5]=31u;goto L8c5fc35e; /* mov #0x1f,r5 */
L8c5fc35e: *stop=0x8c5fc35eu;r[4]=r[14];goto L8c5fc360; /* mov r14,r4 */
L8c5fc360: *stop=0x8c5fc360u;r[5]&=r[12];goto L8c5fc362; /* and r12,r5 */
L8c5fc362: *stop=0x8c5fc362u;r[5]+=r[13];goto L8c5fc364; /* add r13,r5 */
L8c5fc364: *stop=0x8c5fc366u;goto L8c5fc37c; /* bra 0x8c5fc37c */
L8c5fc37c: *stop=0x8c5fc37cu;r[5]+=31u;goto L8c5fc37e; /* add #0x1f,r5 */
L8c5fc37e: *stop=0x8c5fc37eu;r[5]>>=2;goto L8c5fc380; /* shlr2 r5 */
L8c5fc380: *stop=0x8c5fc380u;r[5]>>=2;goto L8c5fc382; /* shlr2 r5 */
L8c5fc382: *stop=0x8c5fc382u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c5fc384; /* shlr r5 */
L8c5fc384: *stop=0x8c5fc384u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],3u);if(status)return status;goto L8c5fc386; /* ocbp @r4 */
L8c5fc386: *stop=0x8c5fc386u;r[5]--;*sr=(*sr&~1u)|(!r[5]);goto L8c5fc388; /* dt r5 */
L8c5fc388: take=!(*sr&1u);*stop=0x8c5fc38au;r[4]+=32u;if(take)goto L8c5fc384;goto L8c5fc38c; /* bf/s 0x8c5fc384 */
L8c5fc38c: *stop=0x8c5fc38cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc3acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc38e; /* mov.l 0x8c5fc3ac,r2 */
L8c5fc38e: target=r[2];*stop=0x8c5fc390u;if(target==0x8c5fc580u)goto L8c5fc580;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c5fc392: *stop=0x8c5fc392u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5fc394; /* stc SR,r0 */
L8c5fc394: *stop=0x8c5fc394u;r[3]=sx(m,bus,&io_status,0x8c5fc3aau,2);if(io_status)return io_status;goto L8c5fc396; /* mov.w 0x8c5fc3aa,r3 */
L8c5fc396: *stop=0x8c5fc396u;r[0]>>=2;goto L8c5fc398; /* shlr2 r0 */
L8c5fc398: *stop=0x8c5fc398u;r[0]>>=2;goto L8c5fc39a; /* shlr2 r0 */
L8c5fc39a: *stop=0x8c5fc39au;r[0]&=0xfu;goto L8c5fc39c; /* and #0xf,r0 */
L8c5fc39c: *stop=0x8c5fc39cu;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c5fc39e; /* mov.w r0,@r15 */
L8c5fc39e: *stop=0x8c5fc39eu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5fc3a0; /* stc SR,r0 */
L8c5fc3a0: *stop=0x8c5fc3a0u;r[0]&=r[3];goto L8c5fc3a2; /* and r3,r0 */
L8c5fc3a2: *stop=0x8c5fc3a2u;r[0]|=0xf0u;goto L8c5fc3a4; /* or #0xf0,r0 */
L8c5fc3a4: *stop=0x8c5fc3a4u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c5fc3a6; /* ldc r0,SR */
L8c5fc3a6: *stop=0x8c5fc3a8u;goto L8c5fc3b0; /* bra 0x8c5fc3b0 */
L8c5fc3b0: *stop=0x8c5fc3b0u;r[0]=0x8c5fc3c0u;goto L8c5fc3b2; /* mova 0x8c5fc3c0,r0 */
L8c5fc3b2: *stop=0x8c5fc3b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc3bcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c5fc3b4; /* mov.l 0x8c5fc3bc,r1 */
L8c5fc3b4: *stop=0x8c5fc3b4u;r[1]|=r[0];goto L8c5fc3b6; /* or r0,r1 */
L8c5fc3b6: target=r[1];*stop=0x8c5fc3b8u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r1 */
L8c5fc580: *stop=0x8c5fc580u;r[15]+=8u;goto L8c5fc582; /* add #0x8,r15 */
L8c5fc582: *stop=0x8c5fc582u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c5fc584; /* mov.l @r15+,r12 */
L8c5fc584: *stop=0x8c5fc584u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c5fc586; /* mov.l @r15+,r13 */
L8c5fc586: *stop=0x8c5fc586u;if(*pr!=entry_pr)return -3;*stop=0x8c5fc588u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
