/* Original sound byte/word read adapters; explicit machine/platform state.
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
 if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_sound_read_8c667150(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c667150;
L8c667150: *stop=0x8c667150u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c667152; /* sts.l PR,@-r15 */
L8c667152: *stop=0x8c667152u;r[15]+=4294967292u;goto L8c667154; /* add #-0x4,r15 */
L8c667154: *stop=0x8c667154u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66717cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c667156; /* mov.l 0x8c66717c,r3 */
L8c667156: *stop=0x8c667156u;r[2]=r[4];goto L8c667158; /* mov r4,r2 */
L8c667158: *stop=0x8c667158u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c66715a; /* cmp/ge r3,r2 */
L8c66715a: take=(*sr&1u);*stop=0x8c66715cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;if(take)goto L8c667174;goto L8c66715e; /* bt/s 0x8c667174 */
L8c66715e: *stop=0x8c66715eu;r[2]=0u;goto L8c667160; /* mov #0x0,r2 */
L8c667160: *stop=0x8c667160u;{uint32_t loaded=rd(m,bus,&io_status,0x8c667180u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c667162; /* mov.l 0x8c667180,r1 */
L8c667162: *stop=0x8c667162u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c667164; /* mov.l r2,@-r15 */
L8c667164: *stop=0x8c667164u;r[7]=1u;goto L8c667166; /* mov #0x1,r7 */
L8c667166: *stop=0x8c667166u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c667168; /* mov.l r2,@-r15 */
L8c667168: *stop=0x8c667168u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c66716a; /* mov.l @(0x8,r15),r4 */
L8c66716a: *stop=0x8c66716au;r[4]+=r[1];goto L8c66716c; /* add r1,r4 */
L8c66716c: *stop=0x8c66716cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c667184u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66716e; /* mov.l 0x8c667184,r1 */
L8c66716e: target=r[1];*pr=0x8c667172u;*stop=0x8c667170u;r[6]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c667172; /* jsr @r1 */
L8c667172: *stop=0x8c667172u;r[15]+=8u;goto L8c667174; /* add #0x8,r15 */
L8c667174: *stop=0x8c667174u;r[15]+=4u;goto L8c667176; /* add #0x4,r15 */
L8c667176: *stop=0x8c667176u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c667178; /* lds.l @r15+,PR */
L8c667178: *stop=0x8c667178u;if(*pr!=entry_pr)return -3;*stop=0x8c66717au;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_sound_read_8c667188(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c667188;
L8c667188: *stop=0x8c667188u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66718a; /* sts.l PR,@-r15 */
L8c66718a: *stop=0x8c66718au;r[15]+=4294967284u;goto L8c66718c; /* add #-0xc,r15 */
L8c66718c: *stop=0x8c66718cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6671bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66718e; /* mov.l 0x8c6671bc,r3 */
L8c66718e: *stop=0x8c66718eu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c667190; /* cmp/ge r3,r4 */
L8c667190: take=(*sr&1u);*stop=0x8c667192u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;if(take)goto L8c6671b4;goto L8c667194; /* bt/s 0x8c6671b4 */
L8c667194: *stop=0x8c667194u;r[1]=3u;goto L8c667196; /* mov #0x3,r1 */
L8c667196: *stop=0x8c667196u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6671c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c667198; /* mov.l 0x8c6671c0,r3 */
L8c667198: *stop=0x8c667198u;r[2]=4294967292u;goto L8c66719a; /* mov #-0x4,r2 */
L8c66719a: *stop=0x8c66719au;r[5]=r[15];goto L8c66719c; /* mov r15,r5 */
L8c66719c: *stop=0x8c66719cu;r[1]&=r[4];goto L8c66719e; /* and r4,r1 */
L8c66719e: *stop=0x8c66719eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c6671a0; /* mov.l r1,@(0x4,r15) */
L8c6671a0: *stop=0x8c6671a0u;r[4]&=r[2];goto L8c6671a2; /* and r2,r4 */
L8c6671a2: target=r[3];*pr=0x8c6671a6u;*stop=0x8c6671a4u;r[5]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6671a6; /* jsr @r3 */
L8c6671a6: *stop=0x8c6671a6u;r[1]=r[15];goto L8c6671a8; /* mov r15,r1 */
L8c6671a8: *stop=0x8c6671a8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6671aa; /* mov.l @(0x4,r15),r3 */
L8c6671aa: *stop=0x8c6671aau;r[1]+=8u;goto L8c6671ac; /* add #0x8,r1 */
L8c6671ac: *stop=0x8c6671acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6671ae; /* mov.l @r15,r2 */
L8c6671ae: *stop=0x8c6671aeu;r[1]+=r[3];goto L8c6671b0; /* add r3,r1 */
L8c6671b0: *stop=0x8c6671b0u;if(!valid_access(r[1],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[1]=loaded;}goto L8c6671b2; /* mov.b @r1,r1 */
L8c6671b2: *stop=0x8c6671b2u;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[1],1);if(io_status)return io_status;goto L8c6671b4; /* mov.b r1,@r2 */
L8c6671b4: *stop=0x8c6671b4u;r[15]+=12u;goto L8c6671b6; /* add #0xc,r15 */
L8c6671b6: *stop=0x8c6671b6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6671b8; /* lds.l @r15+,PR */
L8c6671b8: *stop=0x8c6671b8u;if(*pr!=entry_pr)return -3;*stop=0x8c6671bau;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_sound_read_8c66910c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c66910c;
L8c66910c: *stop=0x8c66910cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66910e; /* sts.l PR,@-r15 */
L8c66910e: *stop=0x8c66910eu;r[15]+=4294967292u;goto L8c669110; /* add #-0x4,r15 */
L8c669110: *stop=0x8c669110u;{uint32_t loaded=rd(m,bus,&io_status,0x8c669138u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c669112; /* mov.l 0x8c669138,r3 */
L8c669112: *stop=0x8c669112u;r[2]=r[4];goto L8c669114; /* mov r4,r2 */
L8c669114: *stop=0x8c669114u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c669116; /* cmp/ge r3,r2 */
L8c669116: take=(*sr&1u);*stop=0x8c669118u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;if(take)goto L8c669130;goto L8c66911a; /* bt/s 0x8c669130 */
L8c66911a: *stop=0x8c66911au;r[2]=0u;goto L8c66911c; /* mov #0x0,r2 */
L8c66911c: *stop=0x8c66911cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66913cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66911e; /* mov.l 0x8c66913c,r1 */
L8c66911e: *stop=0x8c66911eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c669120; /* mov.l r2,@-r15 */
L8c669120: *stop=0x8c669120u;r[7]=1u;goto L8c669122; /* mov #0x1,r7 */
L8c669122: *stop=0x8c669122u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c669124; /* mov.l r2,@-r15 */
L8c669124: *stop=0x8c669124u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c669126; /* mov.l @(0x8,r15),r4 */
L8c669126: *stop=0x8c669126u;r[4]+=r[1];goto L8c669128; /* add r1,r4 */
L8c669128: *stop=0x8c669128u;{uint32_t loaded=rd(m,bus,&io_status,0x8c669140u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66912a; /* mov.l 0x8c669140,r1 */
L8c66912a: target=r[1];*pr=0x8c66912eu;*stop=0x8c66912cu;r[6]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66912e; /* jsr @r1 */
L8c66912e: *stop=0x8c66912eu;r[15]+=8u;goto L8c669130; /* add #0x8,r15 */
L8c669130: *stop=0x8c669130u;r[15]+=4u;goto L8c669132; /* add #0x4,r15 */
L8c669132: *stop=0x8c669132u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c669134; /* lds.l @r15+,PR */
L8c669134: *stop=0x8c669134u;if(*pr!=entry_pr)return -3;*stop=0x8c669136u;r[0]=0u;return 0; /* rts */
}
