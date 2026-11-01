/* Original complete movement routing body,including every stage-specific path.
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
static int sa_player_movement_route_run_8c0730d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c0730e6u)goto L8c0730e6;if(resume_pc==0x8c0730f8u)goto L8c0730f8;if(resume_pc==0x8c073214u)goto L8c073214;if(resume_pc==0x8c073226u)goto L8c073226;if(resume_pc==0x8c073238u)goto L8c073238;if(resume_pc==0x8c07324au)goto L8c07324a;if(resume_pc==0x8c073258u)goto L8c073258;if(resume_pc==0x8c073266u)goto L8c073266;if(resume_pc==0x8c073278u)goto L8c073278;if(resume_pc==0x8c073286u)goto L8c073286;if(resume_pc==0x8c0732eau)goto L8c0732ea;if(resume_pc==0x8c073366u)goto L8c073366;if(resume_pc==0x8c073382u)goto L8c073382;if(resume_pc==0x8c0733ecu)goto L8c0733ec;if(resume_pc==0x8c073482u)goto L8c073482;if(resume_pc==0x8c073494u)goto L8c073494;if(resume_pc==0x8c07366au)goto L8c07366a;if(resume_pc==0x8c07373cu)goto L8c07373c;if(resume_pc==0x8c073752u)goto L8c073752;if(resume_pc==0x8c073762u)goto L8c073762;if(resume_pc==0x8c073810u)goto L8c073810;if(resume_pc==0x8c07381eu)goto L8c07381e;if(resume_pc==0x8c073848u)goto L8c073848;if(resume_pc==0x8c073864u)goto L8c073864;if(resume_pc==0x8c073870u)goto L8c073870;if(resume_pc==0x8c07388eu)goto L8c07388e;if(resume_pc==0x8c0738a0u)goto L8c0738a0;if(resume_pc==0x8c0738aeu)goto L8c0738ae;if(resume_pc==0x8c0738e8u)goto L8c0738e8;if(resume_pc==0x8c0738f4u)goto L8c0738f4;if(resume_pc==0x8c073930u)goto L8c073930;if(resume_pc==0x8c07394eu)goto L8c07394e;if(resume_pc==0x8c073970u)goto L8c073970;if(resume_pc==0x8c0739a4u)goto L8c0739a4;if(resume_pc==0x8c0739b4u)goto L8c0739b4;if(resume_pc==0x8c073a54u)goto L8c073a54;if(resume_pc==0x8c073a9cu)goto L8c073a9c;if(resume_pc==0x8c073aaeu)goto L8c073aae;if(resume_pc==0x8c073ad8u)goto L8c073ad8;if(resume_pc==0x8c073ae4u)goto L8c073ae4;if(resume_pc==0x8c073af8u)goto L8c073af8;if(resume_pc==0x8c073b04u)goto L8c073b04;if(resume_pc==0x8c073b1eu)goto L8c073b1e;if(resume_pc==0x8c073b68u)goto L8c073b68;if(resume_pc==0x8c073b96u)goto L8c073b96;if(resume_pc==0x8c073ba2u)goto L8c073ba2;if(resume_pc==0x8c073bb0u)goto L8c073bb0;if(resume_pc==0x8c073bc0u)goto L8c073bc0;if(resume_pc==0x8c073becu)goto L8c073bec;if(resume_pc==0x8c073bfau)goto L8c073bfa;if(resume_pc==0x8c073c1eu)goto L8c073c1e;if(resume_pc==0x8c073c54u)goto L8c073c54;if(resume_pc==0x8c073c62u)goto L8c073c62;if(resume_pc==0x8c073cbau)goto L8c073cba;if(resume_pc==0x8c073cccu)goto L8c073ccc;if(resume_pc==0x8c073cdeu)goto L8c073cde;if(resume_pc==0x8c073cfau)goto L8c073cfa;if(resume_pc==0x8c073d4eu)goto L8c073d4e;if(resume_pc==0x8c073d98u)goto L8c073d98;if(resume_pc==0x8c073da6u)goto L8c073da6;if(resume_pc==0x8c073db6u)goto L8c073db6;if(resume_pc==0x8c073df8u)goto L8c073df8;if(resume_pc==0x8c073e0cu)goto L8c073e0c;if(resume_pc==0x8c073e14u)goto L8c073e14;if(resume_pc==0x8c073f24u)goto L8c073f24;if(resume_pc==0x8c073f78u)goto L8c073f78;if(resume_pc==0x8c073ff6u)goto L8c073ff6;if(resume_pc==0x8c074068u)goto L8c074068;if(resume_pc==0x8c0740a0u)goto L8c0740a0;if(resume_pc==0x8c0740bau)goto L8c0740ba;if(resume_pc==0x8c0740cau)goto L8c0740ca;if(resume_pc==0x8c0740deu)goto L8c0740de;if(resume_pc==0x8c074108u)goto L8c074108;if(resume_pc==0x8c074186u)goto L8c074186;if(resume_pc==0x8c0741c6u)goto L8c0741c6;if(resume_pc==0x8c07427cu)goto L8c07427c;goto L8c0730d8;
L8c0730d8: *stop=0x8c0730d8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0730da; /* sts.l PR,@-r15 */
L8c0730da: *stop=0x8c0730dau;r[15]+=4294967284u;goto L8c0730dc; /* add #-0xc,r15 */
L8c0730dc: *stop=0x8c0730dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073140u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0730de; /* mov.l 0x8c073140,r3 */
L8c0730de: *stop=0x8c0730deu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c0730e0; /* mov.l r4,@r15 */
L8c0730e0: *stop=0x8c0730e0u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c0730e2; /* mov.l r5,@(0x4,r15) */
L8c0730e2: target=r[3];*pr=0x8c0730e6u;*stop=0x8c0730e4u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0730e6; /* jsr @r3 */
L8c0730e6: *stop=0x8c0730e6u;r[2]=9u;goto L8c0730e8; /* mov #0x9,r2 */
L8c0730e8: *stop=0x8c0730e8u;r[2]<<=8;goto L8c0730ea; /* shll8 r2 */
L8c0730ea: *stop=0x8c0730eau;r[2]+=2u;goto L8c0730ec; /* add #0x2,r2 */
L8c0730ec: *stop=0x8c0730ecu;r[0]=r[0]&65535u;goto L8c0730ee; /* extu.w r0,r0 */
L8c0730ee: *stop=0x8c0730eeu;*sr=(*sr&~1u)|(r[2]==r[0]);goto L8c0730f0; /* cmp/eq r2,r0 */
L8c0730f0: if((*sr&1u))goto L8c073104;goto L8c0730f2; /* bt 0x8c073104 */
L8c0730f2: *stop=0x8c0730f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073140u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0730f4; /* mov.l 0x8c073140,r1 */
L8c0730f4: target=r[1];*pr=0x8c0730f8u;*stop=0x8c0730f6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0730f8; /* jsr @r1 */
L8c0730f8: *stop=0x8c0730f8u;r[3]=9u;goto L8c0730fa; /* mov #0x9,r3 */
L8c0730fa: *stop=0x8c0730fau;r[3]<<=8;goto L8c0730fc; /* shll8 r3 */
L8c0730fc: *stop=0x8c0730fcu;r[3]+=3u;goto L8c0730fe; /* add #0x3,r3 */
L8c0730fe: *stop=0x8c0730feu;r[0]=r[0]&65535u;goto L8c073100; /* extu.w r0,r0 */
L8c073100: *stop=0x8c073100u;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c073102; /* cmp/eq r3,r0 */
L8c073102: if(!(*sr&1u))goto L8c073112;goto L8c073104; /* bf 0x8c073112 */
L8c073104: *stop=0x8c073104u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c073106; /* mov.l @r15,r4 */
L8c073106: *stop=0x8c073106u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c073108; /* mov.l @(0x4,r15),r5 */
L8c073108: *stop=0x8c073108u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c07310a; /* mov.l @(0x8,r15),r6 */
L8c07310a: *stop=0x8c07310au;r[15]+=12u;goto L8c07310c; /* add #0xc,r15 */
L8c07310c: *stop=0x8c07310cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073144u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07310e; /* mov.l 0x8c073144,r3 */
L8c07310e: target=r[3];*stop=0x8c073110u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c073112: *stop=0x8c073112u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c073114; /* mov.l @r15,r4 */
L8c073114: *stop=0x8c073114u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c073116; /* mov.l @(0x4,r15),r5 */
L8c073116: *stop=0x8c073116u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c073118; /* mov.l @(0x8,r15),r6 */
L8c073118: *stop=0x8c073118u;r[15]+=12u;goto L8c07311a; /* add #0xc,r15 */
L8c07311a: *stop=0x8c07311cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c073160; /* bra 0x8c073160 */
L8c073160: *stop=0x8c073160u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c073162; /* mov.l r14,@-r15 */
L8c073162: *stop=0x8c073162u;r[0]=4294967294u;goto L8c073164; /* mov #-0x2,r0 */
L8c073164: *stop=0x8c073164u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c073166; /* mov.l r13,@-r15 */
L8c073166: *stop=0x8c073166u;r[0]<<=8;goto L8c073168; /* shll8 r0 */
L8c073168: *stop=0x8c073168u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c07316a; /* mov.l r12,@-r15 */
L8c07316a: *stop=0x8c07316au;r[0]+=4294967248u;goto L8c07316c; /* add #-0x30,r0 */
L8c07316c: *stop=0x8c07316cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c07316e; /* mov.l r11,@-r15 */
L8c07316e: *stop=0x8c07316eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c073170; /* mov.l r10,@-r15 */
L8c073170: *stop=0x8c073170u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c073172; /* mov.l r9,@-r15 */
L8c073172: *stop=0x8c073172u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c073174; /* mov.l r8,@-r15 */
L8c073174: *stop=0x8c073174u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c073176; /* fmov.s fr15,@-r15 */
L8c073176: *stop=0x8c073176u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c073178; /* fmov.s fr14,@-r15 */
L8c073178: *stop=0x8c073178u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c07317a; /* fmov.s fr13,@-r15 */
L8c07317a: *stop=0x8c07317au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c07317c; /* fmov.s fr12,@-r15 */
L8c07317c: *stop=0x8c07317cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c07317e; /* sts.l PR,@-r15 */
L8c07317e: *stop=0x8c07317eu;r[15]+=r[0];goto L8c073180; /* add r0,r15 */
L8c073180: *stop=0x8c073180u;r[0]=100u;goto L8c073182; /* mov #0x64,r0 */
L8c073182: *stop=0x8c073182u;r[12]=r[6];goto L8c073184; /* mov r6,r12 */
L8c073184: *stop=0x8c073184u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c073186; /* mov.l @(r0,r12),r14 */
L8c073186: *stop=0x8c073186u;r[13]=r[4];goto L8c073188; /* mov r4,r13 */
L8c073188: *stop=0x8c073188u;r[2]=4294967294u;goto L8c07318a; /* mov #-0x2,r2 */
L8c07318a: *stop=0x8c07318au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c07318c; /* fldi0 fr15 */
L8c07318c: *stop=0x8c07318cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][13]=0x3f800000u;goto L8c07318e; /* fldi1 fr13 */
L8c07318e: *stop=0x8c07318eu;r[9]=0u;goto L8c073190; /* mov #0x0,r9 */
L8c073190: *stop=0x8c073190u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073192; /* mov.l 0x8c0733a0,r3 */
L8c073192: *stop=0x8c073192u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[9],4);if(io_status)return io_status;goto L8c073194; /* mov.l r9,@(0x14,r15) */
L8c073194: *stop=0x8c073194u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[9],4);if(io_status)return io_status;goto L8c073196; /* mov.l r9,@r3 */
L8c073196: *stop=0x8c073196u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c073198; /* mov.w @(0x10,r12),r0 */
L8c073198: *stop=0x8c073198u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[0],4);if(io_status)return io_status;goto L8c07319a; /* mov.l r0,@(0x20,r15) */
L8c07319a: *stop=0x8c07319au;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c07319c; /* mov.w @(0x4,r13),r0 */
L8c07319c: *stop=0x8c07319cu;if(!valid_access(r[12]+16u,2u))return -3;wr(m,bus,&io_status,r[12]+0x10u,r[0],2);if(io_status)return io_status;goto L8c07319e; /* mov.w r0,@(0x10,r12) */
L8c07319e: *stop=0x8c07319eu;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c0731a0; /* mov.w @(0x4,r13),r0 */
L8c0731a0: *stop=0x8c0731a0u;r[0]&=r[2];goto L8c0731a2; /* and r2,r0 */
L8c0731a2: *stop=0x8c0731a2u;if(!valid_access(r[13]+4u,2u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[0],2);if(io_status)return io_status;goto L8c0731a4; /* mov.w r0,@(0x4,r13) */
L8c0731a4: *stop=0x8c0731a4u;r[0]=36u;goto L8c0731a6; /* mov #0x24,r0 */
L8c0731a6: *stop=0x8c0731a6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c0731a8; /* fmov fr15,@(r0,r14) */
L8c0731a8: *stop=0x8c0731a8u;r[0]=28u;goto L8c0731aa; /* mov #0x1c,r0 */
L8c0731aa: *stop=0x8c0731aau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c0731ac; /* fmov fr15,@(r0,r14) */
L8c0731ac: *stop=0x8c0731acu;r[0]=32u;goto L8c0731ae; /* mov #0x20,r0 */
L8c0731ae: *stop=0x8c0731aeu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,13u,r[0]+r[14]);if(io_status)return io_status;goto L8c0731b0; /* fmov fr13,@(r0,r14) */
L8c0731b0: *stop=0x8c0731b0u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0731b2; /* mov.l @(0x14,r13),r1 */
L8c0731b2: *stop=0x8c0731b2u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[1],4);if(io_status)return io_status;goto L8c0731b4; /* mov.l r1,@(0x34,r14) */
L8c0731b4: *stop=0x8c0731b4u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0731b6; /* mov.l @(0x1c,r13),r0 */
L8c0731b6: *stop=0x8c0731b6u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[0],4);if(io_status)return io_status;goto L8c0731b8; /* mov.l r0,@(0x38,r14) */
L8c0731b8: *stop=0x8c0731b8u;r[0]=2u;goto L8c0731ba; /* mov #0x2,r0 */
L8c0731ba: *stop=0x8c0731bau;r[0]<<=8;goto L8c0731bc; /* shll8 r0 */
L8c0731bc: *stop=0x8c0731bcu;r[0]+=44u;goto L8c0731be; /* add #0x2c,r0 */
L8c0731be: *stop=0x8c0731beu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0731c0; /* fmov fr15,@(r0,r15) */
L8c0731c0: *stop=0x8c0731c0u;r[0]=2u;goto L8c0731c2; /* mov #0x2,r0 */
L8c0731c2: *stop=0x8c0731c2u;r[0]<<=8;goto L8c0731c4; /* shll8 r0 */
L8c0731c4: *stop=0x8c0731c4u;r[0]+=36u;goto L8c0731c6; /* add #0x24,r0 */
L8c0731c6: *stop=0x8c0731c6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0731c8; /* fmov fr15,@(r0,r15) */
L8c0731c8: *stop=0x8c0731c8u;r[0]=0x8c0733a4u;goto L8c0731ca; /* mova 0x8c0733a4,r0 */
L8c0731ca: *stop=0x8c0731cau;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c0731cc; /* fmov.s @r0,fr12 */
L8c0731cc: *stop=0x8c0731ccu;r[0]=68u;goto L8c0731ce; /* mov #0x44,r0 */
L8c0731ce: *stop=0x8c0731ceu;r[0]<<=2;goto L8c0731d0; /* shll2 r0 */
L8c0731d0: *stop=0x8c0731d0u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c0731d2; /* fmov.s @(r0,r12),fr3 */
L8c0731d2: *stop=0x8c0731d2u;r[0]=2u;goto L8c0731d4; /* mov #0x2,r0 */
L8c0731d4: *stop=0x8c0731d4u;r[0]<<=8;goto L8c0731d6; /* shll8 r0 */
L8c0731d6: *stop=0x8c0731d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[12],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0731d8; /* fmul fr12,fr3 */
L8c0731d8: *stop=0x8c0731d8u;r[1]=2u;goto L8c0731da; /* mov #0x2,r1 */
L8c0731da: *stop=0x8c0731dau;r[0]+=40u;goto L8c0731dc; /* add #0x28,r0 */
L8c0731dc: *stop=0x8c0731dcu;r[1]<<=8;goto L8c0731de; /* shll8 r1 */
L8c0731de: *stop=0x8c0731deu;r[1]+=36u;goto L8c0731e0; /* add #0x24,r1 */
L8c0731e0: *stop=0x8c0731e0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0731e2; /* fmov fr3,@(r0,r15) */
L8c0731e2: *stop=0x8c0731e2u;r[0]=2u;goto L8c0731e4; /* mov #0x2,r0 */
L8c0731e4: *stop=0x8c0731e4u;r[0]<<=8;goto L8c0731e6; /* shll8 r0 */
L8c0731e6: *stop=0x8c0731e6u;r[1]+=r[15];goto L8c0731e8; /* add r15,r1 */
L8c0731e8: *stop=0x8c0731e8u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0731ea; /* mov.l @r1,r3 */
L8c0731ea: *stop=0x8c0731eau;r[0]+=24u;goto L8c0731ec; /* add #0x18,r0 */
L8c0731ec: *stop=0x8c0731ecu;r[0]+=r[15];goto L8c0731ee; /* add r15,r0 */
L8c0731ee: *stop=0x8c0731eeu;if(!valid_access(r[1]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0731f0; /* mov.l @(0x4,r1),r2 */
L8c0731f0: *stop=0x8c0731f0u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c0731f2; /* mov.l r3,@r0 */
L8c0731f2: *stop=0x8c0731f2u;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[2],4);if(io_status)return io_status;goto L8c0731f4; /* mov.l r2,@(0x4,r0) */
L8c0731f4: *stop=0x8c0731f4u;if(!valid_access(r[1]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0731f6; /* mov.l @(0x8,r1),r3 */
L8c0731f6: *stop=0x8c0731f6u;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[3],4);if(io_status)return io_status;goto L8c0731f8; /* mov.l r3,@(0x8,r0) */
L8c0731f8: *stop=0x8c0731f8u;r[0]=96u;goto L8c0731fa; /* mov #0x60,r0 */
L8c0731fa: *stop=0x8c0731fau;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0731fc; /* mov.l @(r0,r12),r1 */
L8c0731fc: *stop=0x8c0731fcu;r[3]=64u;goto L8c0731fe; /* mov #0x40,r3 */
L8c0731fe: *stop=0x8c0731feu;r[3]<<=8;goto L8c073200; /* shll8 r3 */
L8c073200: *stop=0x8c073200u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c073202; /* tst r3,r1 */
L8c073202: take=(*sr&1u);*stop=0x8c073204u;r[11]=r[5];if(take)goto L8c07324e;goto L8c073206; /* bt/s 0x8c07324e */
L8c073206: *stop=0x8c073206u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733a8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073208; /* mov.l 0x8c0733a8,r1 */
L8c073208: *stop=0x8c073208u;r[5]=r[14];goto L8c07320a; /* mov r14,r5 */
L8c07320a: *stop=0x8c07320au;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c07320c; /* mov.l @(0x18,r13),r6 */
L8c07320c: *stop=0x8c07320cu;r[4]=r[12];goto L8c07320e; /* mov r12,r4 */
L8c07320e: *stop=0x8c07320eu;r[5]+=28u;goto L8c073210; /* add #0x1c,r5 */
L8c073210: target=r[1];*pr=0x8c073214u;*stop=0x8c073212u;r[4]+=80u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073214; /* jsr @r1 */
L8c073214: *stop=0x8c073214u;r[5]=2u;goto L8c073216; /* mov #0x2,r5 */
L8c073216: *stop=0x8c073216u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073218; /* mov.l 0x8c0733a8,r3 */
L8c073218: *stop=0x8c073218u;r[5]<<=8;goto L8c07321a; /* shll8 r5 */
L8c07321a: *stop=0x8c07321au;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c07321c; /* mov.l @(0x18,r13),r6 */
L8c07321c: *stop=0x8c07321cu;r[5]+=36u;goto L8c07321e; /* add #0x24,r5 */
L8c07321e: *stop=0x8c07321eu;r[4]=r[12];goto L8c073220; /* mov r12,r4 */
L8c073220: *stop=0x8c073220u;r[5]+=r[15];goto L8c073222; /* add r15,r5 */
L8c073222: target=r[3];*pr=0x8c073226u;*stop=0x8c073224u;r[4]+=80u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073226; /* jsr @r3 */
L8c073226: *stop=0x8c073226u;r[7]=2u;goto L8c073228; /* mov #0x2,r7 */
L8c073228: *stop=0x8c073228u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07322a; /* mov.l 0x8c0733ac,r2 */
L8c07322a: *stop=0x8c07322au;r[7]<<=8;goto L8c07322c; /* shll8 r7 */
L8c07322c: *stop=0x8c07322cu;r[5]=r[12];goto L8c07322e; /* mov r12,r5 */
L8c07322e: *stop=0x8c07322eu;r[7]+=24u;goto L8c073230; /* add #0x18,r7 */
L8c073230: *stop=0x8c073230u;r[6]=r[14];goto L8c073232; /* mov r14,r6 */
L8c073232: *stop=0x8c073232u;r[7]+=r[15];goto L8c073234; /* add r15,r7 */
L8c073234: target=r[2];*pr=0x8c073238u;*stop=0x8c073236u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073238; /* jsr @r2 */
L8c073238: *stop=0x8c073238u;r[5]=2u;goto L8c07323a; /* mov #0x2,r5 */
L8c07323a: *stop=0x8c07323au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07323c; /* mov.l 0x8c0733a8,r3 */
L8c07323c: *stop=0x8c07323cu;r[5]<<=8;goto L8c07323e; /* shll8 r5 */
L8c07323e: *stop=0x8c07323eu;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c073240; /* mov.l @(0x18,r13),r6 */
L8c073240: *stop=0x8c073240u;r[5]+=24u;goto L8c073242; /* add #0x18,r5 */
L8c073242: *stop=0x8c073242u;r[4]=r[12];goto L8c073244; /* mov r12,r4 */
L8c073244: *stop=0x8c073244u;r[5]+=r[15];goto L8c073246; /* add r15,r5 */
L8c073246: target=r[3];*pr=0x8c07324au;*stop=0x8c073248u;r[4]+=80u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07324a; /* jsr @r3 */
L8c07324a: *stop=0x8c07324cu;goto L8c073286; /* bra 0x8c073286 */
L8c07324e: *stop=0x8c07324eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733b0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073250; /* mov.l 0x8c0733b0,r1 */
L8c073250: *stop=0x8c073250u;r[5]=r[14];goto L8c073252; /* mov r14,r5 */
L8c073252: *stop=0x8c073252u;r[5]+=28u;goto L8c073254; /* add #0x1c,r5 */
L8c073254: target=r[1];*pr=0x8c073258u;*stop=0x8c073256u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073258; /* jsr @r1 */
L8c073258: *stop=0x8c073258u;r[5]=2u;goto L8c07325a; /* mov #0x2,r5 */
L8c07325a: *stop=0x8c07325au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07325c; /* mov.l 0x8c0733b0,r3 */
L8c07325c: *stop=0x8c07325cu;r[5]<<=8;goto L8c07325e; /* shll8 r5 */
L8c07325e: *stop=0x8c07325eu;r[5]+=36u;goto L8c073260; /* add #0x24,r5 */
L8c073260: *stop=0x8c073260u;r[5]+=r[15];goto L8c073262; /* add r15,r5 */
L8c073262: target=r[3];*pr=0x8c073266u;*stop=0x8c073264u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073266; /* jsr @r3 */
L8c073266: *stop=0x8c073266u;r[7]=2u;goto L8c073268; /* mov #0x2,r7 */
L8c073268: *stop=0x8c073268u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07326a; /* mov.l 0x8c0733ac,r2 */
L8c07326a: *stop=0x8c07326au;r[7]<<=8;goto L8c07326c; /* shll8 r7 */
L8c07326c: *stop=0x8c07326cu;r[5]=r[12];goto L8c07326e; /* mov r12,r5 */
L8c07326e: *stop=0x8c07326eu;r[7]+=24u;goto L8c073270; /* add #0x18,r7 */
L8c073270: *stop=0x8c073270u;r[6]=r[14];goto L8c073272; /* mov r14,r6 */
L8c073272: *stop=0x8c073272u;r[7]+=r[15];goto L8c073274; /* add r15,r7 */
L8c073274: target=r[2];*pr=0x8c073278u;*stop=0x8c073276u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073278; /* jsr @r2 */
L8c073278: *stop=0x8c073278u;r[5]=2u;goto L8c07327a; /* mov #0x2,r5 */
L8c07327a: *stop=0x8c07327au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07327c; /* mov.l 0x8c0733b0,r3 */
L8c07327c: *stop=0x8c07327cu;r[5]<<=8;goto L8c07327e; /* shll8 r5 */
L8c07327e: *stop=0x8c07327eu;r[5]+=24u;goto L8c073280; /* add #0x18,r5 */
L8c073280: *stop=0x8c073280u;r[5]+=r[15];goto L8c073282; /* add r15,r5 */
L8c073282: target=r[3];*pr=0x8c073286u;*stop=0x8c073284u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073286; /* jsr @r3 */
L8c073286: *stop=0x8c073286u;r[0]=2u;goto L8c073288; /* mov #0x2,r0 */
L8c073288: *stop=0x8c073288u;r[0]<<=8;goto L8c07328a; /* shll8 r0 */
L8c07328a: *stop=0x8c07328au;r[0]+=24u;goto L8c07328c; /* add #0x18,r0 */
L8c07328c: *stop=0x8c07328cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07328e; /* fmov.s @(r0,r15),fr3 */
L8c07328e: *stop=0x8c07328eu;r[0]=32u;goto L8c073290; /* mov #0x20,r0 */
L8c073290: *stop=0x8c073290u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c073292; /* fmov.s @(r0,r13),fr2 */
L8c073292: *stop=0x8c073292u;r[0]=4u;goto L8c073294; /* mov #0x4,r0 */
L8c073294: *stop=0x8c073294u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073296; /* fadd fr3,fr2 */
L8c073296: *stop=0x8c073296u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073298; /* fmov fr2,@(r0,r14) */
L8c073298: *stop=0x8c073298u;r[0]=2u;goto L8c07329a; /* mov #0x2,r0 */
L8c07329a: *stop=0x8c07329au;r[0]<<=8;goto L8c07329c; /* shll8 r0 */
L8c07329c: *stop=0x8c07329cu;r[0]+=28u;goto L8c07329e; /* add #0x1c,r0 */
L8c07329e: *stop=0x8c07329eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0732a0; /* fmov.s @(r0,r15),fr3 */
L8c0732a0: *stop=0x8c0732a0u;r[0]=36u;goto L8c0732a2; /* mov #0x24,r0 */
L8c0732a2: *stop=0x8c0732a2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0732a4; /* fmov.s @(r0,r13),fr2 */
L8c0732a4: *stop=0x8c0732a4u;r[0]=8u;goto L8c0732a6; /* mov #0x8,r0 */
L8c0732a6: *stop=0x8c0732a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0732a8; /* fadd fr3,fr2 */
L8c0732a8: *stop=0x8c0732a8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0732aa; /* fmov fr2,@(r0,r14) */
L8c0732aa: *stop=0x8c0732aau;r[0]=2u;goto L8c0732ac; /* mov #0x2,r0 */
L8c0732ac: *stop=0x8c0732acu;r[0]<<=8;goto L8c0732ae; /* shll8 r0 */
L8c0732ae: *stop=0x8c0732aeu;r[0]+=32u;goto L8c0732b0; /* add #0x20,r0 */
L8c0732b0: *stop=0x8c0732b0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0732b2; /* fmov.s @(r0,r15),fr3 */
L8c0732b2: *stop=0x8c0732b2u;r[0]=40u;goto L8c0732b4; /* mov #0x28,r0 */
L8c0732b4: *stop=0x8c0732b4u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0732b6; /* fmov.s @(r0,r13),fr2 */
L8c0732b6: *stop=0x8c0732b6u;r[0]=12u;goto L8c0732b8; /* mov #0xc,r0 */
L8c0732b8: *stop=0x8c0732b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0732ba; /* fadd fr3,fr2 */
L8c0732ba: *stop=0x8c0732bau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0732bc; /* fmov fr2,@(r0,r14) */
L8c0732bc: *stop=0x8c0732bcu;r[0]=68u;goto L8c0732be; /* mov #0x44,r0 */
L8c0732be: *stop=0x8c0732beu;r[0]<<=2;goto L8c0732c0; /* shll2 r0 */
L8c0732c0: *stop=0x8c0732c0u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c0732c2; /* fmov.s @(r0,r12),fr3 */
L8c0732c2: *stop=0x8c0732c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[12],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0732c4; /* fmul fr12,fr3 */
L8c0732c4: *stop=0x8c0732c4u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c0732c6; /* fmov.s fr3,@r14 */
L8c0732c6: *stop=0x8c0732c6u;if(!valid_access(r[11]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0732c8; /* mov.l @(0x4,r11),r3 */
L8c0732c8: *stop=0x8c0732c8u;r[1]=2u;goto L8c0732ca; /* mov #0x2,r1 */
L8c0732ca: *stop=0x8c0732cau;r[1]<<=8;goto L8c0732cc; /* shll8 r1 */
L8c0732cc: *stop=0x8c0732ccu;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0732ce; /* mov.l @(0x8,r11),r2 */
L8c0732ce: *stop=0x8c0732ceu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c0732d0; /* mov.l r3,@(0x10,r14) */
L8c0732d0: *stop=0x8c0732d0u;r[1]+=r[15];goto L8c0732d2; /* add r15,r1 */
L8c0732d2: *stop=0x8c0732d2u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[2],4);if(io_status)return io_status;goto L8c0732d4; /* mov.l r2,@(0x14,r14) */
L8c0732d4: *stop=0x8c0732d4u;if(!valid_access(r[11]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0732d6; /* mov.l @(0xc,r11),r3 */
L8c0732d6: *stop=0x8c0732d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0732d8; /* mov.l 0x8c0733b4,r2 */
L8c0732d8: *stop=0x8c0732d8u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c0732da; /* mov.l r3,@(0x18,r14) */
L8c0732da: *stop=0x8c0732dau;if(!valid_access(r[13]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0732dc; /* mov.l @(0x20,r13),r0 */
L8c0732dc: *stop=0x8c0732dcu;if(!valid_access(r[13]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0732de; /* mov.l @(0x24,r13),r3 */
L8c0732de: *stop=0x8c0732deu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0732e0; /* mov.l r0,@r1 */
L8c0732e0: *stop=0x8c0732e0u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[3],4);if(io_status)return io_status;goto L8c0732e2; /* mov.l r3,@(0x4,r1) */
L8c0732e2: *stop=0x8c0732e2u;if(!valid_access(r[13]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0732e4; /* mov.l @(0x28,r13),r0 */
L8c0732e4: *stop=0x8c0732e4u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c0732e6; /* mov.l r0,@(0x8,r1) */
L8c0732e6: target=r[2];*pr=0x8c0732eau;*stop=0x8c0732e8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0732ea; /* jsr @r2 */
L8c0732ea: *stop=0x8c0732eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0732ec; /* mov.l 0x8c0733b8,r2 */
L8c0732ec: *stop=0x8c0732ecu;if(!valid_access(r[2],2u))return -3;r[3]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c0732ee; /* mov.w @r2,r3 */
L8c0732ee: *stop=0x8c0732eeu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0732f0; /* tst r3,r3 */
L8c0732f0: if(!(*sr&1u))goto L8c073318;goto L8c0732f2; /* bf 0x8c073318 */
L8c0732f2: *stop=0x8c0732f2u;r[1]=4u;goto L8c0732f4; /* mov #0x4,r1 */
L8c0732f4: *stop=0x8c0732f4u;r[1]+=r[11];goto L8c0732f6; /* add r11,r1 */
L8c0732f6: *stop=0x8c0732f6u;r[0]=32u;goto L8c0732f8; /* mov #0x20,r0 */
L8c0732f8: *stop=0x8c0732f8u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0732fa; /* fmov.s @r1,fr3 */
L8c0732fa: *stop=0x8c0732fau;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0732fc; /* fmov.s @(r0,r13),fr2 */
L8c0732fc: *stop=0x8c0732fcu;r[1]=8u;goto L8c0732fe; /* mov #0x8,r1 */
L8c0732fe: *stop=0x8c0732feu;r[1]+=r[11];goto L8c073300; /* add r11,r1 */
L8c073300: *stop=0x8c073300u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073302; /* fadd fr3,fr2 */
L8c073302: *stop=0x8c073302u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c073304; /* fmov fr2,@(r0,r13) */
L8c073304: *stop=0x8c073304u;r[0]=36u;goto L8c073306; /* mov #0x24,r0 */
L8c073306: *stop=0x8c073306u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c073308; /* fmov.s @(r0,r13),fr2 */
L8c073308: *stop=0x8c073308u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c07330a; /* fmov.s @r1,fr3 */
L8c07330a: *stop=0x8c07330au;r[1]=12u;goto L8c07330c; /* mov #0xc,r1 */
L8c07330c: *stop=0x8c07330cu;r[1]+=r[11];goto L8c07330e; /* add r11,r1 */
L8c07330e: *stop=0x8c07330eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073310; /* fadd fr3,fr2 */
L8c073310: *stop=0x8c073310u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c073312; /* fmov fr2,@(r0,r13) */
L8c073312: *stop=0x8c073312u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c073314; /* fmov.s @r1,fr3 */
L8c073314: *stop=0x8c073316u;r[0]=40u;goto L8c073470; /* bra 0x8c073470 */
L8c073318: *stop=0x8c073318u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07331a; /* mov.l 0x8c0733bc,r3 */
L8c07331a: *stop=0x8c07331au;r[0]=76u;goto L8c07331c; /* mov #0x4c,r0 */
L8c07331c: *stop=0x8c07331cu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[9],4);if(io_status)return io_status;goto L8c07331e; /* mov.l r9,@(r0,r15) */
L8c07331e: *stop=0x8c07331eu;r[10]=r[9];goto L8c073320; /* mov r9,r10 */
L8c073320: *stop=0x8c073320u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c073322; /* mov.l r3,@r15 */
L8c073322: *stop=0x8c073322u;if(!valid_access(r[15]+56u,4u))return -3;wr(m,bus,&io_status,r[15]+0x38u,r[9],4);if(io_status)return io_status;goto L8c073324; /* mov.l r9,@(0x38,r15) */
L8c073324: *stop=0x8c073324u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c073326; /* mov.l r9,@(0x4,r15) */
L8c073326: *stop=0x8c073326u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[9],4);if(io_status)return io_status;goto L8c073328; /* mov.l r9,@(0x8,r15) */
L8c073328: *stop=0x8c073328u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[9],4);if(io_status)return io_status;goto L8c07332a; /* mov.l r9,@(0x10,r15) */
L8c07332a: *stop=0x8c07332cu;r[8]=1u;goto L8c073438; /* bra 0x8c073438 */
L8c07332e: *stop=0x8c07332eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c073330; /* mov.l @r15,r2 */
L8c073330: *stop=0x8c073330u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073332; /* mov.l 0x8c0733c0,r3 */
L8c073332: *stop=0x8c073332u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c073334; /* mov.l @r2,r1 */
L8c073334: *stop=0x8c073334u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c073336; /* tst r3,r1 */
L8c073336: if((*sr&1u))goto L8c073344;goto L8c073338; /* bt 0x8c073344 */
L8c073338: *stop=0x8c073338u;if(!valid_access(r[13]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x9u,1);if(io_status)return io_status;goto L8c07333a; /* mov.b @(0x9,r13),r0 */
L8c07333a: *stop=0x8c07333au;r[0]=r[0]&255u;goto L8c07333c; /* extu.b r0,r0 */
L8c07333c: *stop=0x8c07333cu;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c07333e; /* cmp/eq #0x6,r0 */
L8c07333e: if((*sr&1u))goto L8c073344;goto L8c073340; /* bt 0x8c073344 */
L8c073340: *stop=0x8c073342u;goto L8c07342c; /* bra 0x8c07342c */
L8c073344: *stop=0x8c073344u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c073346; /* mov.l @r15,r4 */
L8c073346: *stop=0x8c073346u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c073348; /* mov.l @(0x8,r4),r4 */
L8c073348: *stop=0x8c073348u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c07334a; /* tst r4,r4 */
L8c07334a: if((*sr&1u))goto L8c073358;goto L8c07334c; /* bt 0x8c073358 */
L8c07334c: *stop=0x8c07334cu;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07334e; /* mov.l @(0x20,r4),r2 */
L8c07334e: *stop=0x8c07334eu;r[3]=1u;goto L8c073350; /* mov #0x1,r3 */
L8c073350: *stop=0x8c073350u;r[3]<<=8;goto L8c073352; /* shll8 r3 */
L8c073352: *stop=0x8c073352u;if(!valid_access(r[2]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x4u,2);if(io_status)return io_status;goto L8c073354; /* mov.w @(0x4,r2),r0 */
L8c073354: *stop=0x8c073354u;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c073356; /* tst r3,r0 */
L8c073356: if((*sr&1u))goto L8c07342c;goto L8c073358; /* bt 0x8c07342c */
L8c073358: *stop=0x8c073358u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c07335a; /* mov.l @r15,r5 */
L8c07335a: *stop=0x8c07335au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c07335c; /* mov.l @r15,r6 */
L8c07335c: *stop=0x8c07335cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07335e; /* mov.l 0x8c0733c4,r3 */
L8c07335e: *stop=0x8c07335eu;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c073360; /* mov.l @(0x4,r5),r5 */
L8c073360: *stop=0x8c073360u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[6]=loaded;}goto L8c073362; /* mov.l @r6,r6 */
L8c073362: target=r[3];*pr=0x8c073366u;*stop=0x8c073364u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073366; /* jsr @r3 */
L8c073366: *stop=0x8c073366u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c073368; /* tst r0,r0 */
L8c073368: if((*sr&1u))goto L8c0733f6;goto L8c07336a; /* bt 0x8c0733f6 */
L8c07336a: *stop=0x8c07336au;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c07336c; /* mov.w @(0x10,r12),r0 */
L8c07336c: *stop=0x8c07336cu;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c07336e; /* tst #0x3,r0 */
L8c07336e: if((*sr&1u))goto L8c0733e0;goto L8c073370; /* bt 0x8c0733e0 */
L8c073370: *stop=0x8c073370u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c073372; /* mov.l @r15,r2 */
L8c073372: *stop=0x8c073372u;r[5]=r[11];goto L8c073374; /* mov r11,r5 */
L8c073374: *stop=0x8c073374u;r[6]=r[12];goto L8c073376; /* mov r12,r6 */
L8c073376: *stop=0x8c073376u;r[7]=r[14];goto L8c073378; /* mov r14,r7 */
L8c073378: *stop=0x8c073378u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c07337a; /* mov.l @r2,r3 */
L8c07337a: *stop=0x8c07337au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0733c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07337c; /* mov.l 0x8c0733c8,r2 */
L8c07337c: *stop=0x8c07337cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c07337e; /* mov.l r3,@-r15 */
L8c07337e: target=r[2];*pr=0x8c073382u;*stop=0x8c073380u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073382; /* jsr @r2 */
L8c073382: *stop=0x8c073382u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c073384; /* cmp/eq #0x1,r0 */
L8c073384: take=!(*sr&1u);*stop=0x8c073386u;r[15]+=4u;if(take)goto L8c0733f6;goto L8c073388; /* bf/s 0x8c0733f6 */
L8c073388: *stop=0x8c073388u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07338a; /* mov.l @(0x8,r15),r2 */
L8c07338a: *stop=0x8c07338au;r[10]=r[8];goto L8c07338c; /* mov r8,r10 */
L8c07338c: *stop=0x8c07338eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c0733f6; /* bra 0x8c0733f6 */
L8c0733e0: *stop=0x8c0733e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073580u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0733e2; /* mov.l 0x8c073580,r2 */
L8c0733e2: *stop=0x8c0733e2u;r[5]=r[11];goto L8c0733e4; /* mov r11,r5 */
L8c0733e4: *stop=0x8c0733e4u;r[7]=r[14];goto L8c0733e6; /* mov r14,r7 */
L8c0733e6: *stop=0x8c0733e6u;r[6]=r[12];goto L8c0733e8; /* mov r12,r6 */
L8c0733e8: target=r[2];*pr=0x8c0733ecu;*stop=0x8c0733eau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0733ec; /* jsr @r2 */
L8c0733ec: *stop=0x8c0733ecu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0733ee; /* cmp/eq #0x1,r0 */
L8c0733ee: if(!(*sr&1u))goto L8c0733f6;goto L8c0733f0; /* bf 0x8c0733f6 */
L8c0733f0: *stop=0x8c0733f0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0733f2; /* mov.l @(0x8,r15),r2 */
L8c0733f2: *stop=0x8c0733f2u;r[10]=r[8];goto L8c0733f4; /* mov r8,r10 */
L8c0733f4: *stop=0x8c0733f4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c0733f6; /* mov.l r2,@(0x4,r15) */
L8c0733f6: *stop=0x8c0733f6u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0733f8; /* mov.l @(0x8,r15),r3 */
L8c0733f8: *stop=0x8c0733f8u;r[1]=80u;goto L8c0733fa; /* mov #0x50,r1 */
L8c0733fa: *stop=0x8c0733fau;r[1]<<=2;goto L8c0733fc; /* shll2 r1 */
L8c0733fc: *stop=0x8c0733fcu;r[2]=r[3];goto L8c0733fe; /* mov r3,r2 */
L8c0733fe: *stop=0x8c0733feu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c073400; /* shll r3 */
L8c073400: *stop=0x8c073400u;r[3]+=r[2];goto L8c073402; /* add r2,r3 */
L8c073402: *stop=0x8c073402u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c073404; /* mov.l @r15,r2 */
L8c073404: *stop=0x8c073404u;r[3]<<=2;goto L8c073406; /* shll2 r3 */
L8c073406: *stop=0x8c073406u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c073408; /* mov.l @r2,r0 */
L8c073408: *stop=0x8c073408u;r[1]+=r[15];goto L8c07340a; /* add r15,r1 */
L8c07340a: *stop=0x8c07340au;r[3]+=r[1];goto L8c07340c; /* add r1,r3 */
L8c07340c: *stop=0x8c07340cu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07340e; /* mov.l @(0x4,r2),r1 */
L8c07340e: *stop=0x8c07340eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c073410; /* mov.l r0,@r3 */
L8c073410: *stop=0x8c073410u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c073412; /* mov.l r1,@(0x4,r3) */
L8c073412: *stop=0x8c073412u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c073414; /* mov.l @(0x8,r2),r0 */
L8c073414: *stop=0x8c073414u;r[2]=64u;goto L8c073416; /* mov #0x40,r2 */
L8c073416: *stop=0x8c073416u;r[2]<<=8;goto L8c073418; /* shll8 r2 */
L8c073418: *stop=0x8c073418u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[0],4);if(io_status)return io_status;goto L8c07341a; /* mov.l r0,@(0x8,r3) */
L8c07341a: *stop=0x8c07341au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c07341c; /* mov.l @(0x8,r15),r0 */
L8c07341c: *stop=0x8c07341cu;r[0]+=1u;goto L8c07341e; /* add #0x1,r0 */
L8c07341e: *stop=0x8c07341eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c073420; /* mov.l r0,@(0x8,r15) */
L8c073420: *stop=0x8c073420u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c073422; /* mov.l @r15,r3 */
L8c073422: *stop=0x8c073422u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c073424; /* mov.l @r3,r1 */
L8c073424: *stop=0x8c073424u;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c073426; /* tst r2,r1 */
L8c073426: if((*sr&1u))goto L8c07342c;goto L8c073428; /* bt 0x8c07342c */
L8c073428: *stop=0x8c073428u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073584u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c07342a; /* mov.l 0x8c073584,r0 */
L8c07342a: *stop=0x8c07342au;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[8],4);if(io_status)return io_status;goto L8c07342c; /* mov.l r8,@r0 */
L8c07342c: *stop=0x8c07342cu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07342e; /* mov.l @(0x10,r15),r3 */
L8c07342e: *stop=0x8c07342eu;r[3]+=1u;goto L8c073430; /* add #0x1,r3 */
L8c073430: *stop=0x8c073430u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c073432; /* mov.l r3,@(0x10,r15) */
L8c073432: *stop=0x8c073432u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c073434; /* mov.l @r15,r2 */
L8c073434: *stop=0x8c073434u;r[2]+=12u;goto L8c073436; /* add #0xc,r2 */
L8c073436: *stop=0x8c073436u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c073438; /* mov.l r2,@r15 */
L8c073438: *stop=0x8c073438u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073588u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07343a; /* mov.l 0x8c073588,r1 */
L8c07343a: *stop=0x8c07343au;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07343c; /* mov.l @(0x10,r15),r2 */
L8c07343c: *stop=0x8c07343cu;if(!valid_access(r[1],2u))return -3;r[3]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;goto L8c07343e; /* mov.w @r1,r3 */
L8c07343e: *stop=0x8c07343eu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c073440; /* cmp/ge r3,r2 */
L8c073440: if((*sr&1u))goto L8c073446;goto L8c073442; /* bt 0x8c073446 */
L8c073442: *stop=0x8c073444u;goto L8c07332e; /* bra 0x8c07332e */
L8c073446: *stop=0x8c073446u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073448; /* mov.l @(0x8,r15),r1 */
L8c073448: *stop=0x8c073448u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c07344a; /* tst r1,r1 */
L8c07344a: if(!(*sr&1u))goto L8c07348e;goto L8c07344c; /* bf 0x8c07348e */
L8c07344c: *stop=0x8c07344cu;r[1]=4u;goto L8c07344e; /* mov #0x4,r1 */
L8c07344e: *stop=0x8c07344eu;r[1]+=r[11];goto L8c073450; /* add r11,r1 */
L8c073450: *stop=0x8c073450u;r[0]=32u;goto L8c073452; /* mov #0x20,r0 */
L8c073452: *stop=0x8c073452u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c073454; /* fmov.s @r1,fr3 */
L8c073454: *stop=0x8c073454u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c073456; /* fmov.s @(r0,r13),fr2 */
L8c073456: *stop=0x8c073456u;r[1]=8u;goto L8c073458; /* mov #0x8,r1 */
L8c073458: *stop=0x8c073458u;r[1]+=r[11];goto L8c07345a; /* add r11,r1 */
L8c07345a: *stop=0x8c07345au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07345c; /* fadd fr3,fr2 */
L8c07345c: *stop=0x8c07345cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07345e; /* fmov fr2,@(r0,r13) */
L8c07345e: *stop=0x8c07345eu;r[0]=36u;goto L8c073460; /* mov #0x24,r0 */
L8c073460: *stop=0x8c073460u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c073462; /* fmov.s @(r0,r13),fr2 */
L8c073462: *stop=0x8c073462u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c073464; /* fmov.s @r1,fr3 */
L8c073464: *stop=0x8c073464u;r[1]=12u;goto L8c073466; /* mov #0xc,r1 */
L8c073466: *stop=0x8c073466u;r[1]+=r[11];goto L8c073468; /* add r11,r1 */
L8c073468: *stop=0x8c073468u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07346a; /* fadd fr3,fr2 */
L8c07346a: *stop=0x8c07346au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07346c; /* fmov fr2,@(r0,r13) */
L8c07346c: *stop=0x8c07346cu;r[0]=40u;goto L8c07346e; /* mov #0x28,r0 */
L8c07346e: *stop=0x8c07346eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c073470; /* fmov.s @r1,fr3 */
L8c073470: *stop=0x8c073470u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c073472; /* fmov.s @(r0,r13),fr2 */
L8c073472: *stop=0x8c073472u;r[5]=r[11];goto L8c073474; /* mov r11,r5 */
L8c073474: *stop=0x8c073474u;{uint32_t loaded=rd(m,bus,&io_status,0x8c07358cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073476; /* mov.l 0x8c07358c,r3 */
L8c073476: *stop=0x8c073476u;r[6]=r[12];goto L8c073478; /* mov r12,r6 */
L8c073478: *stop=0x8c073478u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07347a; /* fadd fr3,fr2 */
L8c07347a: *stop=0x8c07347au;r[7]=0u;goto L8c07347c; /* mov #0x0,r7 */
L8c07347c: *stop=0x8c07347cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07347e; /* fmov fr2,@(r0,r13) */
L8c07347e: target=r[3];*pr=0x8c073482u;*stop=0x8c073480u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073482; /* jsr @r3 */
L8c073482: *stop=0x8c073482u;r[0]=108u;goto L8c073484; /* mov #0x6c,r0 */
L8c073484: *stop=0x8c073484u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[9],4);if(io_status)return io_status;goto L8c073486; /* mov.l r9,@(r0,r12) */
L8c073486: *stop=0x8c073486u;r[0]=116u;goto L8c073488; /* mov #0x74,r0 */
L8c073488: *stop=0x8c073488u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[9],4);if(io_status)return io_status;goto L8c07348a; /* mov.l r9,@(r0,r12) */
L8c07348a: *stop=0x8c07348cu;r[0]=0u;goto L8c0742b2; /* bra 0x8c0742b2 */
L8c07348e: *stop=0x8c07348eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073590u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073490; /* mov.l 0x8c073590,r3 */
L8c073490: target=r[3];*pr=0x8c073494u;*stop=0x8c073492u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073494; /* jsr @r3 */
L8c073494: *stop=0x8c073494u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c073496; /* tst r0,r0 */
L8c073496: take=(*sr&1u);*stop=0x8c073498u;r[4]=r[9];if(take)goto L8c07349e;goto L8c07349a; /* bt/s 0x8c07349e */
L8c07349a: *stop=0x8c07349au;{uint32_t loaded=rd(m,bus,&io_status,0x8c073584u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07349c; /* mov.l 0x8c073584,r3 */
L8c07349c: *stop=0x8c07349cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[9],4);if(io_status)return io_status;goto L8c07349e; /* mov.l r9,@r3 */
L8c07349e: *stop=0x8c07349eu;r[0]=r[10];goto L8c0734a0; /* mov r10,r0 */
L8c0734a0: *stop=0x8c0734a0u;r[5]=4294967168u;goto L8c0734a2; /* mov #-0x80,r5 */
L8c0734a2: *stop=0x8c0734a2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0734a4; /* cmp/eq #0x1,r0 */
L8c0734a4: *stop=0x8c0734a4u;r[5]=r[5]&255u;goto L8c0734a6; /* extu.b r5,r5 */
L8c0734a6: take=!(*sr&1u);*stop=0x8c0734a8u;r[5]+=r[15];if(take)goto L8c0734ca;goto L8c0734aa; /* bf/s 0x8c0734ca */
L8c0734aa: *stop=0x8c0734aau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0734ac; /* mov.l @(0x4,r15),r3 */
L8c0734ac: *stop=0x8c0734acu;r[1]=80u;goto L8c0734ae; /* mov #0x50,r1 */
L8c0734ae: *stop=0x8c0734aeu;r[1]<<=2;goto L8c0734b0; /* shll2 r1 */
L8c0734b0: *stop=0x8c0734b0u;r[2]=r[3];goto L8c0734b2; /* mov r3,r2 */
L8c0734b2: *stop=0x8c0734b2u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c0734b4; /* shll r3 */
L8c0734b4: *stop=0x8c0734b4u;r[3]+=r[2];goto L8c0734b6; /* add r2,r3 */
L8c0734b6: *stop=0x8c0734b6u;r[1]+=r[15];goto L8c0734b8; /* add r15,r1 */
L8c0734b8: *stop=0x8c0734b8u;r[3]<<=2;goto L8c0734ba; /* shll2 r3 */
L8c0734ba: *stop=0x8c0734bau;r[3]+=r[1];goto L8c0734bc; /* add r1,r3 */
L8c0734bc: *stop=0x8c0734bcu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0734be; /* mov.l @r3,r2 */
L8c0734be: *stop=0x8c0734beu;r[4]+=1u;goto L8c0734c0; /* add #0x1,r4 */
L8c0734c0: *stop=0x8c0734c0u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0734c2; /* mov.l @(0x4,r3),r0 */
L8c0734c2: *stop=0x8c0734c2u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[2],4);if(io_status)return io_status;goto L8c0734c4; /* mov.l r2,@r5 */
L8c0734c4: *stop=0x8c0734c4u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[0],4);if(io_status)return io_status;goto L8c0734c6; /* mov.l r0,@(0x4,r5) */
L8c0734c6: *stop=0x8c0734c6u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0734c8; /* mov.l @(0x8,r3),r2 */
L8c0734c8: *stop=0x8c0734c8u;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[2],4);if(io_status)return io_status;goto L8c0734ca; /* mov.l r2,@(0x8,r5) */
L8c0734ca: *stop=0x8c0734cau;r[6]=r[4];goto L8c0734cc; /* mov r4,r6 */
L8c0734cc: *stop=0x8c0734ccu;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c0734ce; /* shll r6 */
L8c0734ce: *stop=0x8c0734ceu;r[3]=r[4];goto L8c0734d0; /* mov r4,r3 */
L8c0734d0: *stop=0x8c0734d0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0734d2; /* mov.l @(0x8,r15),r2 */
L8c0734d2: *stop=0x8c0734d2u;r[0]=108u;goto L8c0734d4; /* mov #0x6c,r0 */
L8c0734d4: *stop=0x8c0734d4u;r[6]+=r[3];goto L8c0734d6; /* add r3,r6 */
L8c0734d6: *stop=0x8c0734d6u;r[2]+=4294967295u;goto L8c0734d8; /* add #-0x1,r2 */
L8c0734d8: *stop=0x8c0734d8u;r[6]<<=2;goto L8c0734da; /* shll2 r6 */
L8c0734da: *stop=0x8c0734dau;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c0734dc; /* mov.l r2,@(0x10,r15) */
L8c0734dc: *stop=0x8c0734dcu;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0734de; /* mov.l @(r0,r12),r3 */
L8c0734de: *stop=0x8c0734deu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0734e0; /* tst r3,r3 */
L8c0734e0: take=(*sr&1u);*stop=0x8c0734e2u;r[6]+=r[5];if(take)goto L8c073558;goto L8c0734e4; /* bt/s 0x8c073558 */
L8c0734e4: *stop=0x8c0734e4u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0734e6; /* mov.l @(0x10,r15),r3 */
L8c0734e6: *stop=0x8c0734e6u;r[4]=r[9];goto L8c0734e8; /* mov r9,r4 */
L8c0734e8: *stop=0x8c0734e8u;r[2]=r[3];goto L8c0734ea; /* mov r3,r2 */
L8c0734ea: *stop=0x8c0734eau;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c0734ec; /* shll r3 */
L8c0734ec: *stop=0x8c0734ecu;r[3]+=r[2];goto L8c0734ee; /* add r2,r3 */
L8c0734ee: *stop=0x8c0734eeu;r[3]<<=2;goto L8c0734f0; /* shll2 r3 */
L8c0734f0: *stop=0x8c0734f0u;r[3]+=r[5];goto L8c0734f2; /* add r5,r3 */
L8c0734f2: *stop=0x8c0734f2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0734f4; /* mov.l r3,@r15 */
L8c0734f4: *stop=0x8c0734f4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0734f6; /* mov.l @(0x8,r15),r2 */
L8c0734f6: *stop=0x8c0734f6u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c0734f8; /* cmp/pl r2 */
L8c0734f8: take=(*sr&1u);*stop=0x8c0734fau;r[7]=r[6];if(take)goto L8c073500;goto L8c0734fc; /* bt/s 0x8c073500 */
L8c0734fc: *stop=0x8c0734feu;goto L8c0735e2; /* bra 0x8c0735e2 */
L8c073500: *stop=0x8c073500u;r[0]=r[10];goto L8c073502; /* mov r10,r0 */
L8c073502: *stop=0x8c073502u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c073504; /* cmp/eq #0x1,r0 */
L8c073504: if(!(*sr&1u))goto L8c073510;goto L8c073506; /* bf 0x8c073510 */
L8c073506: *stop=0x8c073506u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073508; /* mov.l @(0x4,r15),r2 */
L8c073508: *stop=0x8c073508u;*sr=(*sr&~1u)|(r[2]==r[4]);goto L8c07350a; /* cmp/eq r2,r4 */
L8c07350a: if(!(*sr&1u))goto L8c073510;goto L8c07350c; /* bf 0x8c073510 */
L8c07350c: *stop=0x8c07350eu;r[10]=r[9];goto L8c07354c; /* bra 0x8c07354c */
L8c073510: *stop=0x8c073510u;r[6]=r[4];goto L8c073512; /* mov r4,r6 */
L8c073512: *stop=0x8c073512u;r[2]=80u;goto L8c073514; /* mov #0x50,r2 */
L8c073514: *stop=0x8c073514u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c073516; /* shll r6 */
L8c073516: *stop=0x8c073516u;r[3]=r[4];goto L8c073518; /* mov r4,r3 */
L8c073518: *stop=0x8c073518u;r[2]<<=2;goto L8c07351a; /* shll2 r2 */
L8c07351a: *stop=0x8c07351au;r[6]+=r[3];goto L8c07351c; /* add r3,r6 */
L8c07351c: *stop=0x8c07351cu;r[2]+=r[15];goto L8c07351e; /* add r15,r2 */
L8c07351e: *stop=0x8c07351eu;r[6]<<=2;goto L8c073520; /* shll2 r6 */
L8c073520: *stop=0x8c073520u;r[0]=116u;goto L8c073522; /* mov #0x74,r0 */
L8c073522: *stop=0x8c073522u;r[6]+=r[2];goto L8c073524; /* add r2,r6 */
L8c073524: *stop=0x8c073524u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c073526; /* mov.l @(r0,r12),r1 */
L8c073526: *stop=0x8c073526u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073528; /* mov.l @(0x4,r6),r3 */
L8c073528: *stop=0x8c073528u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c07352a; /* cmp/eq r3,r1 */
L8c07352a: if(!(*sr&1u))goto L8c07353e;goto L8c07352c; /* bf 0x8c07353e */
L8c07352c: *stop=0x8c07352cu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[1]=loaded;}goto L8c07352e; /* mov.l @r6,r1 */
L8c07352e: *stop=0x8c07352eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c073530; /* mov.l @r15,r3 */
L8c073530: *stop=0x8c073530u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c073532; /* mov.l @(0x4,r6),r0 */
L8c073532: *stop=0x8c073532u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c073534; /* mov.l r1,@r3 */
L8c073534: *stop=0x8c073534u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c073536; /* mov.l r0,@(0x4,r3) */
L8c073536: *stop=0x8c073536u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073538; /* mov.l @(0x8,r6),r1 */
L8c073538: *stop=0x8c073538u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c07353a; /* mov.l r1,@(0x8,r3) */
L8c07353a: *stop=0x8c07353cu;goto L8c07354c; /* bra 0x8c07354c */
L8c07353e: *stop=0x8c07353eu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c073540; /* mov.l @r6,r3 */
L8c073540: *stop=0x8c073540u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073542; /* mov.l @(0x4,r6),r1 */
L8c073542: *stop=0x8c073542u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[3],4);if(io_status)return io_status;goto L8c073544; /* mov.l r3,@r7 */
L8c073544: *stop=0x8c073544u;if(!valid_access(r[7]+4u,4u))return -3;wr(m,bus,&io_status,r[7]+0x4u,r[1],4);if(io_status)return io_status;goto L8c073546; /* mov.l r1,@(0x4,r7) */
L8c073546: *stop=0x8c073546u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073548; /* mov.l @(0x8,r6),r3 */
L8c073548: *stop=0x8c073548u;if(!valid_access(r[7]+8u,4u))return -3;wr(m,bus,&io_status,r[7]+0x8u,r[3],4);if(io_status)return io_status;goto L8c07354a; /* mov.l r3,@(0x8,r7) */
L8c07354a: *stop=0x8c07354au;r[7]+=12u;goto L8c07354c; /* add #0xc,r7 */
L8c07354c: *stop=0x8c07354cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07354e; /* mov.l @(0x8,r15),r3 */
L8c07354e: *stop=0x8c07354eu;r[4]+=1u;goto L8c073550; /* add #0x1,r4 */
L8c073550: *stop=0x8c073550u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c073552; /* cmp/ge r3,r4 */
L8c073552: if(!(*sr&1u))goto L8c073500;goto L8c073554; /* bf 0x8c073500 */
L8c073554: *stop=0x8c073556u;goto L8c0735e2; /* bra 0x8c0735e2 */
L8c073558: *stop=0x8c073558u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c07355a; /* mov.l @(0x10,r15),r4 */
L8c07355a: *stop=0x8c07355au;r[7]=r[6];goto L8c07355c; /* mov r6,r7 */
L8c07355c: *stop=0x8c07355cu;r[3]=r[4];goto L8c07355e; /* mov r4,r3 */
L8c07355e: *stop=0x8c07355eu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c073560; /* shll r3 */
L8c073560: *stop=0x8c073560u;r[2]=r[4];goto L8c073562; /* mov r4,r2 */
L8c073562: *stop=0x8c073562u;r[3]+=r[2];goto L8c073564; /* add r2,r3 */
L8c073564: *stop=0x8c073564u;r[3]<<=2;goto L8c073566; /* shll2 r3 */
L8c073566: *stop=0x8c073566u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c073568; /* cmp/pz r4 */
L8c073568: *stop=0x8c073568u;r[3]+=r[5];goto L8c07356a; /* add r5,r3 */
L8c07356a: take=!(*sr&1u);*stop=0x8c07356cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c0735e2;goto L8c07356e; /* bf/s 0x8c0735e2 */
L8c07356e: *stop=0x8c07356eu;r[0]=r[10];goto L8c073570; /* mov r10,r0 */
L8c073570: *stop=0x8c073570u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c073572; /* cmp/eq #0x1,r0 */
L8c073572: if(!(*sr&1u))goto L8c0735a0;goto L8c073574; /* bf 0x8c0735a0 */
L8c073574: *stop=0x8c073574u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073576; /* mov.l @(0x4,r15),r2 */
L8c073576: *stop=0x8c073576u;*sr=(*sr&~1u)|(r[2]==r[4]);goto L8c073578; /* cmp/eq r2,r4 */
L8c073578: if(!(*sr&1u))goto L8c0735a0;goto L8c07357a; /* bf 0x8c0735a0 */
L8c07357a: *stop=0x8c07357cu;r[10]=r[9];goto L8c0735dc; /* bra 0x8c0735dc */
L8c0735a0: *stop=0x8c0735a0u;r[6]=r[4];goto L8c0735a2; /* mov r4,r6 */
L8c0735a2: *stop=0x8c0735a2u;r[2]=80u;goto L8c0735a4; /* mov #0x50,r2 */
L8c0735a4: *stop=0x8c0735a4u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c0735a6; /* shll r6 */
L8c0735a6: *stop=0x8c0735a6u;r[3]=r[4];goto L8c0735a8; /* mov r4,r3 */
L8c0735a8: *stop=0x8c0735a8u;r[2]<<=2;goto L8c0735aa; /* shll2 r2 */
L8c0735aa: *stop=0x8c0735aau;r[6]+=r[3];goto L8c0735ac; /* add r3,r6 */
L8c0735ac: *stop=0x8c0735acu;r[2]+=r[15];goto L8c0735ae; /* add r15,r2 */
L8c0735ae: *stop=0x8c0735aeu;r[6]<<=2;goto L8c0735b0; /* shll2 r6 */
L8c0735b0: *stop=0x8c0735b0u;r[0]=116u;goto L8c0735b2; /* mov #0x74,r0 */
L8c0735b2: *stop=0x8c0735b2u;r[6]+=r[2];goto L8c0735b4; /* add r2,r6 */
L8c0735b4: *stop=0x8c0735b4u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0735b6; /* mov.l @(r0,r12),r1 */
L8c0735b6: *stop=0x8c0735b6u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0735b8; /* mov.l @(0x4,r6),r3 */
L8c0735b8: *stop=0x8c0735b8u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c0735ba; /* cmp/eq r3,r1 */
L8c0735ba: if(!(*sr&1u))goto L8c0735ce;goto L8c0735bc; /* bf 0x8c0735ce */
L8c0735bc: *stop=0x8c0735bcu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0735be; /* mov.l @r6,r1 */
L8c0735be: *stop=0x8c0735beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0735c0; /* mov.l @r15,r3 */
L8c0735c0: *stop=0x8c0735c0u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0735c2; /* mov.l @(0x4,r6),r0 */
L8c0735c2: *stop=0x8c0735c2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c0735c4; /* mov.l r1,@r3 */
L8c0735c4: *stop=0x8c0735c4u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c0735c6; /* mov.l r0,@(0x4,r3) */
L8c0735c6: *stop=0x8c0735c6u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0735c8; /* mov.l @(0x8,r6),r1 */
L8c0735c8: *stop=0x8c0735c8u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c0735ca; /* mov.l r1,@(0x8,r3) */
L8c0735ca: *stop=0x8c0735ccu;goto L8c0735dc; /* bra 0x8c0735dc */
L8c0735ce: *stop=0x8c0735ceu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0735d0; /* mov.l @r6,r3 */
L8c0735d0: *stop=0x8c0735d0u;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0735d2; /* mov.l @(0x4,r6),r1 */
L8c0735d2: *stop=0x8c0735d2u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[3],4);if(io_status)return io_status;goto L8c0735d4; /* mov.l r3,@r7 */
L8c0735d4: *stop=0x8c0735d4u;if(!valid_access(r[7]+4u,4u))return -3;wr(m,bus,&io_status,r[7]+0x4u,r[1],4);if(io_status)return io_status;goto L8c0735d6; /* mov.l r1,@(0x4,r7) */
L8c0735d6: *stop=0x8c0735d6u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0735d8; /* mov.l @(0x8,r6),r3 */
L8c0735d8: *stop=0x8c0735d8u;if(!valid_access(r[7]+8u,4u))return -3;wr(m,bus,&io_status,r[7]+0x8u,r[3],4);if(io_status)return io_status;goto L8c0735da; /* mov.l r3,@(0x8,r7) */
L8c0735da: *stop=0x8c0735dau;r[7]+=12u;goto L8c0735dc; /* add #0xc,r7 */
L8c0735dc: *stop=0x8c0735dcu;r[4]+=4294967295u;goto L8c0735de; /* add #-0x1,r4 */
L8c0735de: *stop=0x8c0735deu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c0735e0; /* cmp/pz r4 */
L8c0735e0: if((*sr&1u))goto L8c07356e;goto L8c0735e2; /* bt 0x8c07356e */
L8c0735e2: *stop=0x8c0735e2u;r[0]=2u;goto L8c0735e4; /* mov #0x2,r0 */
L8c0735e4: *stop=0x8c0735e4u;r[0]<<=8;goto L8c0735e6; /* shll8 r0 */
L8c0735e6: *stop=0x8c0735e6u;r[0]+=24u;goto L8c0735e8; /* add #0x18,r0 */
L8c0735e8: *stop=0x8c0735e8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0735ea; /* fmov.s @(r0,r15),fr3 */
L8c0735ea: *stop=0x8c0735eau;r[0]=32u;goto L8c0735ec; /* mov #0x20,r0 */
L8c0735ec: *stop=0x8c0735ecu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0735ee; /* fmov.s @(r0,r13),fr2 */
L8c0735ee: *stop=0x8c0735eeu;r[0]=4u;goto L8c0735f0; /* mov #0x4,r0 */
L8c0735f0: *stop=0x8c0735f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0735f2; /* fadd fr3,fr2 */
L8c0735f2: *stop=0x8c0735f2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0735f4; /* fmov fr2,@(r0,r14) */
L8c0735f4: *stop=0x8c0735f4u;r[0]=2u;goto L8c0735f6; /* mov #0x2,r0 */
L8c0735f6: *stop=0x8c0735f6u;r[0]<<=8;goto L8c0735f8; /* shll8 r0 */
L8c0735f8: *stop=0x8c0735f8u;r[0]+=28u;goto L8c0735fa; /* add #0x1c,r0 */
L8c0735fa: *stop=0x8c0735fau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0735fc; /* fmov.s @(r0,r15),fr3 */
L8c0735fc: *stop=0x8c0735fcu;r[0]=36u;goto L8c0735fe; /* mov #0x24,r0 */
L8c0735fe: *stop=0x8c0735feu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c073600; /* fmov.s @(r0,r13),fr2 */
L8c073600: *stop=0x8c073600u;r[0]=8u;goto L8c073602; /* mov #0x8,r0 */
L8c073602: *stop=0x8c073602u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073604; /* fadd fr3,fr2 */
L8c073604: *stop=0x8c073604u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073606; /* fmov fr2,@(r0,r14) */
L8c073606: *stop=0x8c073606u;r[0]=2u;goto L8c073608; /* mov #0x2,r0 */
L8c073608: *stop=0x8c073608u;r[0]<<=8;goto L8c07360a; /* shll8 r0 */
L8c07360a: *stop=0x8c07360au;r[0]+=32u;goto L8c07360c; /* add #0x20,r0 */
L8c07360c: *stop=0x8c07360cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07360e; /* fmov.s @(r0,r15),fr3 */
L8c07360e: *stop=0x8c07360eu;r[0]=40u;goto L8c073610; /* mov #0x28,r0 */
L8c073610: *stop=0x8c073610u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c073612; /* fmov.s @(r0,r13),fr2 */
L8c073612: *stop=0x8c073612u;r[0]=12u;goto L8c073614; /* mov #0xc,r0 */
L8c073614: *stop=0x8c073614u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073616; /* fadd fr3,fr2 */
L8c073616: *stop=0x8c073616u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073618; /* fmov fr2,@(r0,r14) */
L8c073618: *stop=0x8c073618u;if(!valid_access(r[11]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07361a; /* mov.l @(0x4,r11),r3 */
L8c07361a: *stop=0x8c07361au;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07361c; /* mov.l @(0x8,r11),r2 */
L8c07361c: *stop=0x8c07361cu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c07361e; /* mov.l r3,@(0x10,r14) */
L8c07361e: *stop=0x8c07361eu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[2],4);if(io_status)return io_status;goto L8c073620; /* mov.l r2,@(0x14,r14) */
L8c073620: *stop=0x8c073620u;if(!valid_access(r[11]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073622; /* mov.l @(0xc,r11),r3 */
L8c073622: *stop=0x8c073622u;r[0]=108u;goto L8c073624; /* mov #0x6c,r0 */
L8c073624: *stop=0x8c073624u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c073626; /* mov.l r3,@(0x18,r14) */
L8c073626: *stop=0x8c073626u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[9],4);if(io_status)return io_status;goto L8c073628; /* mov.l r9,@(r0,r12) */
L8c073628: *stop=0x8c073628u;r[1]=r[15];goto L8c07362a; /* mov r15,r1 */
L8c07362a: *stop=0x8c07362au;r[1]+=72u;goto L8c07362c; /* add #0x48,r1 */
L8c07362c: *stop=0x8c07362cu;if(!valid_access(r[15]+52u,4u))return -3;wr(m,bus,&io_status,r[15]+0x34u,r[5],4);if(io_status)return io_status;goto L8c07362e; /* mov.l r5,@(0x34,r15) */
L8c07362e: *stop=0x8c07362eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c073630; /* mov.l r9,@r15 */
L8c073630: *stop=0x8c073630u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c073632; /* mov.l @(0x20,r15),r0 */
L8c073632: *stop=0x8c073632u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c073634; /* tst #0x3,r0 */
L8c073634: *stop=0x8c073634u;r[0]=*sr&1u;goto L8c073636; /* movt r0 */
L8c073636: *stop=0x8c073636u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c073638; /* mov.l r0,@r1 */
L8c073638: *stop=0x8c073638u;r[0]=0x8c0737c0u;goto L8c07363a; /* mova 0x8c0737c0,r0 */
L8c07363a: *stop=0x8c07363au;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07363c; /* mov.l @(0x10,r15),r3 */
L8c07363c: *stop=0x8c07363cu;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07363e; /* mov.l @(0x34,r15),r1 */
L8c07363e: *stop=0x8c07363eu;r[2]=r[3];goto L8c073640; /* mov r3,r2 */
L8c073640: *stop=0x8c073640u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c073642; /* shll r3 */
L8c073642: *stop=0x8c073642u;r[3]+=r[2];goto L8c073644; /* add r2,r3 */
L8c073644: *stop=0x8c073644u;r[3]<<=2;goto L8c073646; /* shll2 r3 */
L8c073646: *stop=0x8c073646u;r[3]+=r[1];goto L8c073648; /* add r1,r3 */
L8c073648: *stop=0x8c073648u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[3],4);if(io_status)return io_status;goto L8c07364a; /* mov.l r3,@(0x20,r15) */
L8c07364a: *stop=0x8c07364cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c074190; /* bra 0x8c074190 */
L8c07364e: *stop=0x8c07364eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}goto L8c073650; /* mov.l @r15,r10 */
L8c073650: *stop=0x8c073650u;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073652; /* mov.l @(0x34,r15),r2 */
L8c073652: *stop=0x8c073652u;r[3]=r[10];goto L8c073654; /* mov r10,r3 */
L8c073654: *stop=0x8c073654u;*sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c073656; /* shll r10 */
L8c073656: *stop=0x8c073656u;r[10]+=r[3];goto L8c073658; /* add r3,r10 */
L8c073658: *stop=0x8c073658u;r[10]<<=2;goto L8c07365a; /* shll2 r10 */
L8c07365a: *stop=0x8c07365au;r[10]+=r[2];goto L8c07365c; /* add r2,r10 */
L8c07365c: *stop=0x8c07365cu;if(!valid_access(r[10]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07365e; /* mov.l @(0x8,r10),r3 */
L8c07365e: *stop=0x8c07365eu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c073660; /* mov.l r3,@(0x10,r15) */
L8c073660: *stop=0x8c073660u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0737c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073662; /* mov.l 0x8c0737c4,r3 */
L8c073662: *stop=0x8c073662u;if(!valid_access(r[10]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c073664; /* mov.l @(0x4,r10),r5 */
L8c073664: *stop=0x8c073664u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[6]=loaded;}goto L8c073666; /* mov.l @r10,r6 */
L8c073666: target=r[3];*pr=0x8c07366au;*stop=0x8c073668u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07366a; /* jsr @r3 */
L8c07366a: *stop=0x8c07366au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c07366c; /* tst r0,r0 */
L8c07366c: if(!(*sr&1u))goto L8c073672;goto L8c07366e; /* bf 0x8c073672 */
L8c07366e: *stop=0x8c073670u;goto L8c07418a; /* bra 0x8c07418a */
L8c073672: *stop=0x8c073672u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[8],4);if(io_status)return io_status;goto L8c073674; /* mov.l r8,@(0x4,r15) */
L8c073674: *stop=0x8c073674u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c073676; /* mov.w @(0x4,r13),r0 */
L8c073676: *stop=0x8c073676u;*sr=(*sr&~1u)|!(r[8]&r[0]);goto L8c073678; /* tst r8,r0 */
L8c073678: if(!(*sr&1u))goto L8c07367e;goto L8c07367a; /* bf 0x8c07367e */
L8c07367a: *stop=0x8c07367cu;goto L8c07377c; /* bra 0x8c07377c */
L8c07367e: *stop=0x8c07367eu;if(!valid_access(r[15]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073680; /* mov.l @(0x38,r15),r3 */
L8c073680: *stop=0x8c073680u;if(!valid_access(r[10]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073682; /* mov.l @(0x4,r10),r2 */
L8c073682: *stop=0x8c073682u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c073684; /* cmp/eq r2,r3 */
L8c073684: if((*sr&1u))goto L8c07377c;goto L8c073686; /* bt 0x8c07377c */
L8c073686: *stop=0x8c073686u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0737c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073688; /* mov.l 0x8c0737c8,r3 */
L8c073688: *stop=0x8c073688u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c07368a; /* mov.l @r3,r0 */
L8c07368a: *stop=0x8c07368au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c07368c; /* cmp/eq #0x1,r0 */
L8c07368c: if(!(*sr&1u))goto L8c0736ca;goto L8c07368e; /* bf 0x8c0736ca */
L8c07368e: *stop=0x8c07368eu;r[0]=108u;goto L8c073690; /* mov #0x6c,r0 */
L8c073690: *stop=0x8c073690u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073692; /* fmov.s @(r0,r14),fr3 */
L8c073692: *stop=0x8c073692u;r[0]=84u;goto L8c073694; /* mov #0x54,r0 */
L8c073694: *stop=0x8c073694u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073696; /* fmov.s @(r0,r14),fr2 */
L8c073696: *stop=0x8c073696u;r[0]=32u;goto L8c073698; /* mov #0x20,r0 */
L8c073698: *stop=0x8c073698u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07369a; /* fadd fr3,fr2 */
L8c07369a: *stop=0x8c07369au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c07369c; /* fmov.s @(r0,r13),fr3 */
L8c07369c: *stop=0x8c07369cu;r[0]=2u;goto L8c07369e; /* mov #0x2,r0 */
L8c07369e: *stop=0x8c07369eu;r[0]<<=8;goto L8c0736a0; /* shll8 r0 */
L8c0736a0: *stop=0x8c0736a0u;r[0]+=12u;goto L8c0736a2; /* add #0xc,r0 */
L8c0736a2: *stop=0x8c0736a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0736a4; /* fsub fr3,fr2 */
L8c0736a4: *stop=0x8c0736a4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0736a6; /* fmov fr2,@(r0,r15) */
L8c0736a6: *stop=0x8c0736a6u;r[0]=112u;goto L8c0736a8; /* mov #0x70,r0 */
L8c0736a8: *stop=0x8c0736a8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0736aa; /* fmov.s @(r0,r14),fr3 */
L8c0736aa: *stop=0x8c0736aau;r[0]=88u;goto L8c0736ac; /* mov #0x58,r0 */
L8c0736ac: *stop=0x8c0736acu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0736ae; /* fmov.s @(r0,r14),fr2 */
L8c0736ae: *stop=0x8c0736aeu;r[0]=36u;goto L8c0736b0; /* mov #0x24,r0 */
L8c0736b0: *stop=0x8c0736b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0736b2; /* fadd fr3,fr2 */
L8c0736b2: *stop=0x8c0736b2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0736b4; /* fmov.s @(r0,r13),fr3 */
L8c0736b4: *stop=0x8c0736b4u;r[0]=2u;goto L8c0736b6; /* mov #0x2,r0 */
L8c0736b6: *stop=0x8c0736b6u;r[0]<<=8;goto L8c0736b8; /* shll8 r0 */
L8c0736b8: *stop=0x8c0736b8u;r[0]+=16u;goto L8c0736ba; /* add #0x10,r0 */
L8c0736ba: *stop=0x8c0736bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0736bc; /* fsub fr3,fr2 */
L8c0736bc: *stop=0x8c0736bcu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0736be; /* fmov fr2,@(r0,r15) */
L8c0736be: *stop=0x8c0736beu;r[0]=116u;goto L8c0736c0; /* mov #0x74,r0 */
L8c0736c0: *stop=0x8c0736c0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0736c2; /* fmov.s @(r0,r14),fr3 */
L8c0736c2: *stop=0x8c0736c2u;r[0]=92u;goto L8c0736c4; /* mov #0x5c,r0 */
L8c0736c4: *stop=0x8c0736c4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0736c6; /* fmov.s @(r0,r14),fr2 */
L8c0736c6: *stop=0x8c0736c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073722; /* bra 0x8c073722 */
L8c0736ca: *stop=0x8c0736cau;r[0]=108u;goto L8c0736cc; /* mov #0x6c,r0 */
L8c0736cc: *stop=0x8c0736ccu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0736ce; /* fmov.s @(r0,r14),fr3 */
L8c0736ce: *stop=0x8c0736ceu;r[0]=96u;goto L8c0736d0; /* mov #0x60,r0 */
L8c0736d0: *stop=0x8c0736d0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0736d2; /* fmov.s @(r0,r14),fr2 */
L8c0736d2: *stop=0x8c0736d2u;r[0]=2u;goto L8c0736d4; /* mov #0x2,r0 */
L8c0736d4: *stop=0x8c0736d4u;r[0]<<=8;goto L8c0736d6; /* shll8 r0 */
L8c0736d6: *stop=0x8c0736d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0736d8; /* fadd fr3,fr2 */
L8c0736d8: *stop=0x8c0736d8u;r[0]+=36u;goto L8c0736da; /* add #0x24,r0 */
L8c0736da: *stop=0x8c0736dau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0736dc; /* fmov.s @(r0,r15),fr3 */
L8c0736dc: *stop=0x8c0736dcu;r[0]=32u;goto L8c0736de; /* mov #0x20,r0 */
L8c0736de: *stop=0x8c0736deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0736e0; /* fsub fr3,fr2 */
L8c0736e0: *stop=0x8c0736e0u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0736e2; /* fmov.s @(r0,r13),fr3 */
L8c0736e2: *stop=0x8c0736e2u;r[0]=2u;goto L8c0736e4; /* mov #0x2,r0 */
L8c0736e4: *stop=0x8c0736e4u;r[0]<<=8;goto L8c0736e6; /* shll8 r0 */
L8c0736e6: *stop=0x8c0736e6u;r[0]+=12u;goto L8c0736e8; /* add #0xc,r0 */
L8c0736e8: *stop=0x8c0736e8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0736ea; /* fsub fr3,fr2 */
L8c0736ea: *stop=0x8c0736eau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0736ec; /* fmov fr2,@(r0,r15) */
L8c0736ec: *stop=0x8c0736ecu;r[0]=112u;goto L8c0736ee; /* mov #0x70,r0 */
L8c0736ee: *stop=0x8c0736eeu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0736f0; /* fmov.s @(r0,r14),fr3 */
L8c0736f0: *stop=0x8c0736f0u;r[0]=100u;goto L8c0736f2; /* mov #0x64,r0 */
L8c0736f2: *stop=0x8c0736f2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0736f4; /* fmov.s @(r0,r14),fr2 */
L8c0736f4: *stop=0x8c0736f4u;r[0]=2u;goto L8c0736f6; /* mov #0x2,r0 */
L8c0736f6: *stop=0x8c0736f6u;r[0]<<=8;goto L8c0736f8; /* shll8 r0 */
L8c0736f8: *stop=0x8c0736f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0736fa; /* fadd fr3,fr2 */
L8c0736fa: *stop=0x8c0736fau;r[0]+=40u;goto L8c0736fc; /* add #0x28,r0 */
L8c0736fc: *stop=0x8c0736fcu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0736fe; /* fmov.s @(r0,r15),fr3 */
L8c0736fe: *stop=0x8c0736feu;r[0]=36u;goto L8c073700; /* mov #0x24,r0 */
L8c073700: *stop=0x8c073700u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c073702; /* fsub fr3,fr2 */
L8c073702: *stop=0x8c073702u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c073704; /* fmov.s @(r0,r13),fr3 */
L8c073704: *stop=0x8c073704u;r[0]=2u;goto L8c073706; /* mov #0x2,r0 */
L8c073706: *stop=0x8c073706u;r[0]<<=8;goto L8c073708; /* shll8 r0 */
L8c073708: *stop=0x8c073708u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07370a; /* fsub fr3,fr2 */
L8c07370a: *stop=0x8c07370au;r[0]+=16u;goto L8c07370c; /* add #0x10,r0 */
L8c07370c: *stop=0x8c07370cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07370e; /* fmov fr2,@(r0,r15) */
L8c07370e: *stop=0x8c07370eu;r[0]=116u;goto L8c073710; /* mov #0x74,r0 */
L8c073710: *stop=0x8c073710u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073712; /* fmov.s @(r0,r14),fr3 */
L8c073712: *stop=0x8c073712u;r[0]=104u;goto L8c073714; /* mov #0x68,r0 */
L8c073714: *stop=0x8c073714u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073716; /* fmov.s @(r0,r14),fr2 */
L8c073716: *stop=0x8c073716u;r[0]=2u;goto L8c073718; /* mov #0x2,r0 */
L8c073718: *stop=0x8c073718u;r[0]<<=8;goto L8c07371a; /* shll8 r0 */
L8c07371a: *stop=0x8c07371au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07371c; /* fadd fr3,fr2 */
L8c07371c: *stop=0x8c07371cu;r[0]+=44u;goto L8c07371e; /* add #0x2c,r0 */
L8c07371e: *stop=0x8c07371eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073720; /* fmov.s @(r0,r15),fr3 */
L8c073720: *stop=0x8c073720u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c073722; /* fsub fr3,fr2 */
L8c073722: *stop=0x8c073722u;r[0]=40u;goto L8c073724; /* mov #0x28,r0 */
L8c073724: *stop=0x8c073724u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0737ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073726; /* mov.l 0x8c0737cc,r3 */
L8c073726: *stop=0x8c073726u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c073728; /* fmov.s @(r0,r13),fr3 */
L8c073728: *stop=0x8c073728u;r[0]=2u;goto L8c07372a; /* mov #0x2,r0 */
L8c07372a: *stop=0x8c07372au;r[4]=2u;goto L8c07372c; /* mov #0x2,r4 */
L8c07372c: *stop=0x8c07372cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07372e; /* fsub fr3,fr2 */
L8c07372e: *stop=0x8c07372eu;r[0]<<=8;goto L8c073730; /* shll8 r0 */
L8c073730: *stop=0x8c073730u;r[4]<<=8;goto L8c073732; /* shll8 r4 */
L8c073732: *stop=0x8c073732u;r[0]+=20u;goto L8c073734; /* add #0x14,r0 */
L8c073734: *stop=0x8c073734u;r[4]+=12u;goto L8c073736; /* add #0xc,r4 */
L8c073736: *stop=0x8c073736u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073738; /* fmov fr2,@(r0,r15) */
L8c073738: target=r[3];*pr=0x8c07373cu;*stop=0x8c07373au;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07373c; /* jsr @r3 */
L8c07373c: *stop=0x8c07373cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c07373e; /* fldi0 fr3 */
L8c07373e: *stop=0x8c07373eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c073740; /* fcmp/eq fr3,fr0 */
L8c073740: *stop=0x8c073740u;r[0]=36u;goto L8c073742; /* mov #0x24,r0 */
L8c073742: take=(*sr&1u);*stop=0x8c073744u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[0]+r[15]);if(io_status)return io_status;if(take)goto L8c07377c;goto L8c073746; /* bt/s 0x8c07377c */
L8c073746: *stop=0x8c073746u;r[4]=2u;goto L8c073748; /* mov #0x2,r4 */
L8c073748: *stop=0x8c073748u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0737d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07374a; /* mov.l 0x8c0737d0,r2 */
L8c07374a: *stop=0x8c07374au;r[4]<<=8;goto L8c07374c; /* shll8 r4 */
L8c07374c: *stop=0x8c07374cu;r[4]+=12u;goto L8c07374e; /* add #0xc,r4 */
L8c07374e: target=r[2];*pr=0x8c073752u;*stop=0x8c073750u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073752; /* jsr @r2 */
L8c073752: *stop=0x8c073752u;r[4]=2u;goto L8c073754; /* mov #0x2,r4 */
L8c073754: *stop=0x8c073754u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0737d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073756; /* mov.l 0x8c0737d4,r3 */
L8c073756: *stop=0x8c073756u;r[4]<<=8;goto L8c073758; /* shll8 r4 */
L8c073758: *stop=0x8c073758u;r[5]=r[14];goto L8c07375a; /* mov r14,r5 */
L8c07375a: *stop=0x8c07375au;r[5]+=72u;goto L8c07375c; /* add #0x48,r5 */
L8c07375c: *stop=0x8c07375cu;r[4]+=12u;goto L8c07375e; /* add #0xc,r4 */
L8c07375e: target=r[3];*pr=0x8c073762u;*stop=0x8c073760u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073762; /* jsr @r3 */
L8c073762: *stop=0x8c073762u;r[0]=0x8c0737d8u;goto L8c073764; /* mova 0x8c0737d8,r0 */
L8c073764: *stop=0x8c073764u;fcopy(f,4u,0u);goto L8c073766; /* fmov fr0,fr4 */
L8c073766: *stop=0x8c073766u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c073768; /* fmov.s @r0,fr3 */
L8c073768: *stop=0x8c073768u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c07376a; /* fcmp/gt fr4,fr3 */
L8c07376a: if((*sr&1u))goto L8c07377a;goto L8c07376c; /* bt 0x8c07377a */
L8c07376c: *stop=0x8c07376cu;r[0]=68u;goto L8c07376e; /* mov #0x44,r0 */
L8c07376e: *stop=0x8c07376eu;r[0]<<=2;goto L8c073770; /* shll2 r0 */
L8c073770: *stop=0x8c073770u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c073772; /* fmov.s @(r0,r12),fr2 */
L8c073772: *stop=0x8c073772u;r[0]=36u;goto L8c073774; /* mov #0x24,r0 */
L8c073774: *stop=0x8c073774u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c073776; /* fmov.s @(r0,r15),fr1 */
L8c073776: *stop=0x8c073776u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c073778; /* fcmp/gt fr2,fr1 */
L8c073778: if(!(*sr&1u))goto L8c07377c;goto L8c07377a; /* bf 0x8c07377c */
L8c07377a: *stop=0x8c07377au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c07377c; /* mov.l r9,@(0x4,r15) */
L8c07377c: *stop=0x8c07377cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c07377e; /* mov.l @(0x4,r15),r0 */
L8c07377e: *stop=0x8c07377eu;r[3]=3u;goto L8c073780; /* mov #0x3,r3 */
L8c073780: *stop=0x8c073780u;r[0]|=0x4u;goto L8c073782; /* or #0x4,r0 */
L8c073782: *stop=0x8c073782u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c073784; /* mov.l r0,@(0x4,r15) */
L8c073784: *stop=0x8c073784u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c073786; /* mov.w @(0x4,r13),r0 */
L8c073786: *stop=0x8c073786u;r[4]=r[0];goto L8c073788; /* mov r0,r4 */
L8c073788: *stop=0x8c073788u;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c07378a; /* tst r4,r3 */
L8c07378a: if(!(*sr&1u))goto L8c073790;goto L8c07378c; /* bf 0x8c073790 */
L8c07378c: *stop=0x8c07378eu;goto L8c073880; /* bra 0x8c073880 */
L8c073790: *stop=0x8c073790u;r[1]=32u;goto L8c073792; /* mov #0x20,r1 */
L8c073792: *stop=0x8c073792u;r[1]<<=8;goto L8c073794; /* shll8 r1 */
L8c073794: *stop=0x8c073794u;*sr=(*sr&~1u)|!(r[4]&r[1]);goto L8c073796; /* tst r4,r1 */
L8c073796: if((*sr&1u))goto L8c07379c;goto L8c073798; /* bt 0x8c07379c */
L8c073798: *stop=0x8c07379au;goto L8c073880; /* bra 0x8c073880 */
L8c07379c: *stop=0x8c07379cu;r[0]=108u;goto L8c07379e; /* mov #0x6c,r0 */
L8c07379e: *stop=0x8c07379eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0737c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0737a0; /* mov.l 0x8c0737c8,r2 */
L8c0737a0: *stop=0x8c0737a0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0737a2; /* fmov.s @(r0,r14),fr3 */
L8c0737a2: *stop=0x8c0737a2u;r[0]=84u;goto L8c0737a4; /* mov #0x54,r0 */
L8c0737a4: *stop=0x8c0737a4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0737a6; /* fmov.s @(r0,r14),fr2 */
L8c0737a6: *stop=0x8c0737a6u;r[0]=4u;goto L8c0737a8; /* mov #0x4,r0 */
L8c0737a8: *stop=0x8c0737a8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0737aa; /* fadd fr3,fr2 */
L8c0737aa: *stop=0x8c0737aau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0737ac; /* fmov.s @(r0,r14),fr3 */
L8c0737ac: *stop=0x8c0737acu;r[0]=116u;goto L8c0737ae; /* mov #0x74,r0 */
L8c0737ae: *stop=0x8c0737aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0737b0; /* fsub fr3,fr2 */
L8c0737b0: *stop=0x8c0737b0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0737b2; /* fmov fr2,@(r0,r15) */
L8c0737b2: *stop=0x8c0737b2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0737b4; /* mov.l @r2,r3 */
L8c0737b4: *stop=0x8c0737b4u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0737b6; /* tst r3,r3 */
L8c0737b6: if((*sr&1u))goto L8c0737e0;goto L8c0737b8; /* bt 0x8c0737e0 */
L8c0737b8: *stop=0x8c0737b8u;r[0]=120u;goto L8c0737ba; /* mov #0x78,r0 */
L8c0737ba: *stop=0x8c0737bcu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0737f4; /* bra 0x8c0737f4 */
L8c0737e0: *stop=0x8c0737e0u;r[0]=112u;goto L8c0737e2; /* mov #0x70,r0 */
L8c0737e2: *stop=0x8c0737e2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0737e4; /* fmov.s @(r0,r14),fr3 */
L8c0737e4: *stop=0x8c0737e4u;r[0]=88u;goto L8c0737e6; /* mov #0x58,r0 */
L8c0737e6: *stop=0x8c0737e6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0737e8; /* fmov.s @(r0,r14),fr2 */
L8c0737e8: *stop=0x8c0737e8u;r[0]=8u;goto L8c0737ea; /* mov #0x8,r0 */
L8c0737ea: *stop=0x8c0737eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0737ec; /* fadd fr3,fr2 */
L8c0737ec: *stop=0x8c0737ecu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0737ee; /* fmov.s @(r0,r14),fr3 */
L8c0737ee: *stop=0x8c0737eeu;r[0]=120u;goto L8c0737f0; /* mov #0x78,r0 */
L8c0737f0: *stop=0x8c0737f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0737f2; /* fsub fr3,fr2 */
L8c0737f2: *stop=0x8c0737f2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0737f4; /* fmov fr2,@(r0,r15) */
L8c0737f4: *stop=0x8c0737f4u;r[0]=116u;goto L8c0737f6; /* mov #0x74,r0 */
L8c0737f6: *stop=0x8c0737f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0737f8; /* mov.l 0x8c0739e0,r3 */
L8c0737f8: *stop=0x8c0737f8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0737fa; /* fmov.s @(r0,r14),fr3 */
L8c0737fa: *stop=0x8c0737fau;r[0]=92u;goto L8c0737fc; /* mov #0x5c,r0 */
L8c0737fc: *stop=0x8c0737fcu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0737fe; /* fmov.s @(r0,r14),fr2 */
L8c0737fe: *stop=0x8c0737feu;r[0]=12u;goto L8c073800; /* mov #0xc,r0 */
L8c073800: *stop=0x8c073800u;r[4]=r[15];goto L8c073802; /* mov r15,r4 */
L8c073802: *stop=0x8c073802u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073804; /* fadd fr3,fr2 */
L8c073804: *stop=0x8c073804u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073806; /* fmov.s @(r0,r14),fr3 */
L8c073806: *stop=0x8c073806u;r[0]=124u;goto L8c073808; /* mov #0x7c,r0 */
L8c073808: *stop=0x8c073808u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07380a; /* fsub fr3,fr2 */
L8c07380a: *stop=0x8c07380au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07380c; /* fmov fr2,@(r0,r15) */
L8c07380c: target=r[3];*pr=0x8c073810u;*stop=0x8c07380eu;r[4]+=116u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073810; /* jsr @r3 */
L8c073810: *stop=0x8c073810u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c073812; /* fldi0 fr3 */
L8c073812: *stop=0x8c073812u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c073814; /* fcmp/eq fr3,fr0 */
L8c073814: if((*sr&1u))goto L8c073880;goto L8c073816; /* bt 0x8c073880 */
L8c073816: *stop=0x8c073816u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073818; /* mov.l 0x8c0739e4,r2 */
L8c073818: *stop=0x8c073818u;r[4]=r[15];goto L8c07381a; /* mov r15,r4 */
L8c07381a: target=r[2];*pr=0x8c07381eu;*stop=0x8c07381cu;r[4]+=116u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07381e; /* jsr @r2 */
L8c07381e: *stop=0x8c07381eu;r[0]=56u;goto L8c073820; /* mov #0x38,r0 */
L8c073820: *stop=0x8c073820u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c073822; /* fldi0 fr3 */
L8c073822: *stop=0x8c073822u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c073824; /* fmov.s @(r0,r12),fr2 */
L8c073824: *stop=0x8c073824u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c073826; /* fcmp/gt fr2,fr3 */
L8c073826: if((*sr&1u))goto L8c07382e;goto L8c073828; /* bt 0x8c07382e */
L8c073828: *stop=0x8c073828u;r[0]=104u;goto L8c07382a; /* mov #0x68,r0 */
L8c07382a: *stop=0x8c07382cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,13u,r[0]+r[15]);if(io_status)return io_status;goto L8c073836; /* bra 0x8c073836 */
L8c07382e: *stop=0x8c07382eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c073830; /* fldi1 fr2 */
L8c073830: *stop=0x8c073830u;r[0]=104u;goto L8c073832; /* mov #0x68,r0 */
L8c073832: *stop=0x8c073832u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c073834; /* fneg fr2 */
L8c073834: *stop=0x8c073834u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073836; /* fmov fr2,@(r0,r15) */
L8c073836: *stop=0x8c073836u;r[0]=108u;goto L8c073838; /* mov #0x6c,r0 */
L8c073838: *stop=0x8c073838u;r[5]=r[15];goto L8c07383a; /* mov r15,r5 */
L8c07383a: *stop=0x8c07383au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c07383c; /* fmov fr15,@(r0,r15) */
L8c07383c: *stop=0x8c07383cu;r[0]=112u;goto L8c07383e; /* mov #0x70,r0 */
L8c07383e: *stop=0x8c07383eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073840; /* mov.l 0x8c0739e8,r3 */
L8c073840: *stop=0x8c073840u;r[5]+=104u;goto L8c073842; /* add #0x68,r5 */
L8c073842: *stop=0x8c073842u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c073844; /* fmov fr15,@(r0,r15) */
L8c073844: target=r[3];*pr=0x8c073848u;*stop=0x8c073846u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073848; /* jsr @r3 */
L8c073848: *stop=0x8c073848u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07384a; /* mov.l 0x8c0739ec,r3 */
L8c07384a: *stop=0x8c07384au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c07384c; /* mov.l @r3,r2 */
L8c07384c: *stop=0x8c07384cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c07384e; /* tst r2,r2 */
L8c07384e: if((*sr&1u))goto L8c07385c;goto L8c073850; /* bt 0x8c07385c */
L8c073850: *stop=0x8c073850u;r[0]=108u;goto L8c073852; /* mov #0x6c,r0 */
L8c073852: *stop=0x8c073852u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c073854; /* fldi1 fr2 */
L8c073854: *stop=0x8c073854u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073856; /* fmov.s @(r0,r15),fr3 */
L8c073856: *stop=0x8c073856u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]&=0x7fffffffu;goto L8c073858; /* fabs fr3 */
L8c073858: *stop=0x8c073858u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,0);if(status)return status;goto L8c07385a; /* fcmp/eq fr2,fr3 */
L8c07385a: if((*sr&1u))goto L8c073880;goto L8c07385c; /* bt 0x8c073880 */
L8c07385c: *stop=0x8c07385cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07385e; /* mov.l 0x8c0739e4,r2 */
L8c07385e: *stop=0x8c07385eu;r[4]=r[15];goto L8c073860; /* mov r15,r4 */
L8c073860: target=r[2];*pr=0x8c073864u;*stop=0x8c073862u;r[4]+=104u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073864; /* jsr @r2 */
L8c073864: *stop=0x8c073864u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073866; /* mov.l 0x8c0739f0,r3 */
L8c073866: *stop=0x8c073866u;r[5]=r[15];goto L8c073868; /* mov r15,r5 */
L8c073868: *stop=0x8c073868u;r[4]=r[15];goto L8c07386a; /* mov r15,r4 */
L8c07386a: *stop=0x8c07386au;r[5]+=104u;goto L8c07386c; /* add #0x68,r5 */
L8c07386c: target=r[3];*pr=0x8c073870u;*stop=0x8c07386eu;r[4]+=116u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073870; /* jsr @r3 */
L8c073870: *stop=0x8c073870u;r[0]=0x8c0739f4u;goto L8c073872; /* mova 0x8c0739f4,r0 */
L8c073872: *stop=0x8c073872u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c073874; /* fmov.s @r0,fr3 */
L8c073874: *stop=0x8c073874u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c073876; /* fcmp/gt fr0,fr3 */
L8c073876: if(!(*sr&1u))goto L8c073880;goto L8c073878; /* bf 0x8c073880 */
L8c073878: *stop=0x8c073878u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07387a; /* mov.l @(0x4,r15),r2 */
L8c07387a: *stop=0x8c07387au;r[3]=4294967291u;goto L8c07387c; /* mov #-0x5,r3 */
L8c07387c: *stop=0x8c07387cu;r[2]&=r[3];goto L8c07387e; /* and r3,r2 */
L8c07387e: *stop=0x8c07387eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c073880; /* mov.l r2,@(0x4,r15) */
L8c073880: *stop=0x8c073880u;r[5]=r[14];goto L8c073882; /* mov r14,r5 */
L8c073882: *stop=0x8c073882u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073884; /* mov.l 0x8c0739f8,r3 */
L8c073884: *stop=0x8c073884u;r[4]=r[14];goto L8c073886; /* mov r14,r4 */
L8c073886: *stop=0x8c073886u;r[6]=0u;goto L8c073888; /* mov #0x0,r6 */
L8c073888: *stop=0x8c073888u;r[5]+=72u;goto L8c07388a; /* add #0x48,r5 */
L8c07388a: target=r[3];*pr=0x8c07388eu;*stop=0x8c07388cu;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07388e; /* jsr @r3 */
L8c07388e: *stop=0x8c07388eu;r[3]=44u;goto L8c073890; /* mov #0x2c,r3 */
L8c073890: *stop=0x8c073890u;r[3]<<=8;goto L8c073892; /* shll8 r3 */
L8c073892: *stop=0x8c073892u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[3]);goto L8c073894; /* cmp/gt r3,r0 */
L8c073894: take=!(*sr&1u);*stop=0x8c073896u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;if(take)goto L8c0738b6;goto L8c073898; /* bf/s 0x8c0738b6 */
L8c073898: *stop=0x8c073898u;r[5]=r[14];goto L8c07389a; /* mov r14,r5 */
L8c07389a: *stop=0x8c07389au;r[6]=r[10];goto L8c07389c; /* mov r10,r6 */
L8c07389c: target=0x8c0742d4u;*pr=0x8c0738a0u;*stop=0x8c07389eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0738a0; /* bsr 0x8c0742d4 */
L8c0738a0: *stop=0x8c0738a0u;r[5]=r[14];goto L8c0738a2; /* mov r14,r5 */
L8c0738a2: *stop=0x8c0738a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0738a4; /* mov.l 0x8c0739f8,r2 */
L8c0738a4: *stop=0x8c0738a4u;r[4]=r[14];goto L8c0738a6; /* mov r14,r4 */
L8c0738a6: *stop=0x8c0738a6u;r[6]=0u;goto L8c0738a8; /* mov #0x0,r6 */
L8c0738a8: *stop=0x8c0738a8u;r[5]+=72u;goto L8c0738aa; /* add #0x48,r5 */
L8c0738aa: target=r[2];*pr=0x8c0738aeu;*stop=0x8c0738acu;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0738ae; /* jsr @r2 */
L8c0738ae: *stop=0x8c0738aeu;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c0738b0; /* mov.l r0,@(0x18,r15) */
L8c0738b0: *stop=0x8c0738b0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0738b2; /* mov.l @(0x4,r15),r0 */
L8c0738b2: *stop=0x8c0738b2u;r[0]|=0x2u;goto L8c0738b4; /* or #0x2,r0 */
L8c0738b4: *stop=0x8c0738b4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c0738b6; /* mov.l r0,@(0x4,r15) */
L8c0738b6: *stop=0x8c0738b6u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c0738b8; /* mov.w @(0x4,r13),r0 */
L8c0738b8: *stop=0x8c0738b8u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c0738ba; /* tst #0x2,r0 */
L8c0738ba: if((*sr&1u))goto L8c0738c0;goto L8c0738bc; /* bt 0x8c0738c0 */
L8c0738bc: *stop=0x8c0738beu;goto L8c074160; /* bra 0x8c074160 */
L8c0738c0: *stop=0x8c0738c0u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0738c2; /* mov.l @r10,r2 */
L8c0738c2: *stop=0x8c0738c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739fcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0738c4; /* mov.l 0x8c0739fc,r3 */
L8c0738c4: *stop=0x8c0738c4u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c0738c6; /* tst r3,r2 */
L8c0738c6: if((*sr&1u))goto L8c0738cc;goto L8c0738c8; /* bt 0x8c0738cc */
L8c0738c8: *stop=0x8c0738cau;goto L8c074160; /* bra 0x8c074160 */
L8c0738cc: *stop=0x8c0738ccu;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0738ce; /* mov.l @(0x18,r15),r1 */
L8c0738ce: *stop=0x8c0738ceu;r[0]=44u;goto L8c0738d0; /* mov #0x2c,r0 */
L8c0738d0: *stop=0x8c0738d0u;r[0]<<=8;goto L8c0738d2; /* shll8 r0 */
L8c0738d2: *stop=0x8c0738d2u;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[0]);goto L8c0738d4; /* cmp/gt r0,r1 */
L8c0738d4: if((*sr&1u))goto L8c0738de;goto L8c0738d6; /* bt 0x8c0738de */
L8c0738d6: *stop=0x8c0738d6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0738d8; /* mov.l @(0x4,r15),r0 */
L8c0738d8: *stop=0x8c0738d8u;r[0]&=0x5u;goto L8c0738da; /* and #0x5,r0 */
L8c0738da: *stop=0x8c0738dau;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c0738dc; /* cmp/eq #0x5,r0 */
L8c0738dc: if((*sr&1u))goto L8c073914;goto L8c0738de; /* bt 0x8c073914 */
L8c0738de: *stop=0x8c0738deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0738e0; /* mov.l 0x8c0739f0,r3 */
L8c0738e0: *stop=0x8c0738e0u;r[5]=r[14];goto L8c0738e2; /* mov r14,r5 */
L8c0738e2: *stop=0x8c0738e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073a00u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0738e4; /* mov.l 0x8c073a00,r4 */
L8c0738e4: target=r[3];*pr=0x8c0738e8u;*stop=0x8c0738e6u;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0738e8; /* jsr @r3 */
L8c0738e8: *stop=0x8c0738e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073a00u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0738ea; /* mov.l 0x8c073a00,r4 */
L8c0738ea: *stop=0x8c0738eau;r[5]=r[14];goto L8c0738ec; /* mov r14,r5 */
L8c0738ec: *stop=0x8c0738ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0738ee; /* mov.l 0x8c0739f0,r2 */
L8c0738ee: *stop=0x8c0738eeu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c0738f0; /* fmov.s fr0,@-r15 */
L8c0738f0: target=r[2];*pr=0x8c0738f4u;*stop=0x8c0738f2u;r[5]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0738f4; /* jsr @r2 */
L8c0738f4: *stop=0x8c0738f4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0738f6; /* fmov.s @r15+,fr3 */
L8c0738f6: *stop=0x8c0738f6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0738f8; /* fcmp/gt fr0,fr3 */
L8c0738f8: if((*sr&1u))goto L8c073914;goto L8c0738fa; /* bt 0x8c073914 */
L8c0738fa: *stop=0x8c0738fau;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0738fc; /* mov.l @r10,r2 */
L8c0738fc: *stop=0x8c0738fcu;r[3]=64u;goto L8c0738fe; /* mov #0x40,r3 */
L8c0738fe: *stop=0x8c0738feu;r[3]<<=8;goto L8c073900; /* shll8 r3 */
L8c073900: *stop=0x8c073900u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c073902; /* tst r3,r2 */
L8c073902: if(!(*sr&1u))goto L8c073908;goto L8c073904; /* bf 0x8c073908 */
L8c073904: *stop=0x8c073906u;goto L8c074160; /* bra 0x8c074160 */
L8c073908: *stop=0x8c073908u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c07390a; /* mov.l @(0x4,r15),r0 */
L8c07390a: *stop=0x8c07390au;r[0]&=0x5u;goto L8c07390c; /* and #0x5,r0 */
L8c07390c: *stop=0x8c07390cu;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c07390e; /* cmp/eq #0x5,r0 */
L8c07390e: if((*sr&1u))goto L8c073914;goto L8c073910; /* bt 0x8c073914 */
L8c073910: *stop=0x8c073912u;goto L8c074160; /* bra 0x8c074160 */
L8c073914: *stop=0x8c073914u;r[0]=72u;goto L8c073916; /* mov #0x48,r0 */
L8c073916: *stop=0x8c073916u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c073918; /* mov.l @(r0,r15),r3 */
L8c073918: *stop=0x8c073918u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c07391a; /* tst r3,r3 */
L8c07391a: if(!(*sr&1u))goto L8c073926;goto L8c07391c; /* bf 0x8c073926 */
L8c07391c: *stop=0x8c07391cu;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c07391e; /* mov.w @(0x10,r12),r0 */
L8c07391e: *stop=0x8c07391eu;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c073920; /* tst #0x3,r0 */
L8c073920: if(!(*sr&1u))goto L8c073926;goto L8c073922; /* bf 0x8c073926 */
L8c073922: *stop=0x8c073924u;goto L8c074160; /* bra 0x8c074160 */
L8c073926: *stop=0x8c073926u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073928; /* mov.l 0x8c0739f0,r2 */
L8c073928: *stop=0x8c073928u;r[5]=r[14];goto L8c07392a; /* mov r14,r5 */
L8c07392a: *stop=0x8c07392au;{uint32_t loaded=rd(m,bus,&io_status,0x8c073a00u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c07392c; /* mov.l 0x8c073a00,r4 */
L8c07392c: target=r[2];*pr=0x8c073930u;*stop=0x8c07392eu;r[5]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073930; /* jsr @r2 */
L8c073930: *stop=0x8c073930u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c073932; /* fldi0 fr3 */
L8c073932: *stop=0x8c073932u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c073934; /* fcmp/gt fr3,fr0 */
L8c073934: if(!(*sr&1u))goto L8c073940;goto L8c073936; /* bf 0x8c073940 */
L8c073936: *stop=0x8c073936u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c073938; /* mov.w @(0x10,r12),r0 */
L8c073938: *stop=0x8c073938u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c07393a; /* tst #0x3,r0 */
L8c07393a: if(!(*sr&1u))goto L8c073940;goto L8c07393c; /* bf 0x8c073940 */
L8c07393c: *stop=0x8c07393eu;goto L8c074160; /* bra 0x8c074160 */
L8c073940: *stop=0x8c073940u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c073942; /* mov.l @(0x4,r15),r0 */
L8c073942: *stop=0x8c073942u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c073944; /* tst #0x2,r0 */
L8c073944: if(!(*sr&1u))goto L8c073956;goto L8c073946; /* bf 0x8c073956 */
L8c073946: *stop=0x8c073946u;r[5]=r[14];goto L8c073948; /* mov r14,r5 */
L8c073948: *stop=0x8c073948u;r[6]=r[10];goto L8c07394a; /* mov r10,r6 */
L8c07394a: target=0x8c0742d4u;*pr=0x8c07394eu;*stop=0x8c07394cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07394e; /* bsr 0x8c0742d4 */
L8c07394e: *stop=0x8c07394eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c073950; /* tst r0,r0 */
L8c073950: if(!(*sr&1u))goto L8c073956;goto L8c073952; /* bf 0x8c073956 */
L8c073952: *stop=0x8c073954u;goto L8c074160; /* bra 0x8c074160 */
L8c073956: *stop=0x8c073956u;if(!valid_access(r[13]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x9u,1);if(io_status)return io_status;goto L8c073958; /* mov.b @(0x9,r13),r0 */
L8c073958: *stop=0x8c073958u;r[0]=r[0]&255u;goto L8c07395a; /* extu.b r0,r0 */
L8c07395a: *stop=0x8c07395au;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c07395c; /* cmp/eq #0x7,r0 */
L8c07395c: if(!(*sr&1u))goto L8c073968;goto L8c07395e; /* bf 0x8c073968 */
L8c07395e: *stop=0x8c07395eu;if(!valid_access(r[13],1u))return -3;r[0]=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;goto L8c073960; /* mov.b @r13,r0 */
L8c073960: *stop=0x8c073960u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c073962; /* cmp/eq #0x30,r0 */
L8c073962: if(!(*sr&1u))goto L8c073968;goto L8c073964; /* bf 0x8c073968 */
L8c073964: *stop=0x8c073966u;goto L8c074160; /* bra 0x8c074160 */
L8c073968: *stop=0x8c073968u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739e0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07396a; /* mov.l 0x8c0739e0,r2 */
L8c07396a: *stop=0x8c07396au;r[4]=r[11];goto L8c07396c; /* mov r11,r4 */
L8c07396c: target=r[2];*pr=0x8c073970u;*stop=0x8c07396eu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073970; /* jsr @r2 */
L8c073970: *stop=0x8c073970u;r[0]=4294967236u;goto L8c073972; /* mov #-0x3c,r0 */
L8c073972: *stop=0x8c073972u;r[0]=r[0]&255u;goto L8c073974; /* extu.b r0,r0 */
L8c073974: *stop=0x8c073974u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c073976; /* fmov.s @(r0,r12),fr3 */
L8c073976: *stop=0x8c073976u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c073978; /* fcmp/gt fr0,fr3 */
L8c073978: if((*sr&1u))goto L8c073a20;goto L8c07397a; /* bt 0x8c073a20 */
L8c07397a: *stop=0x8c07397au;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c07397c; /* mov.l @r10,r2 */
L8c07397c: *stop=0x8c07397cu;r[3]=64u;goto L8c07397e; /* mov #0x40,r3 */
L8c07397e: *stop=0x8c07397eu;r[3]<<=8;goto L8c073980; /* shll8 r3 */
L8c073980: *stop=0x8c073980u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c073982; /* tst r3,r2 */
L8c073982: if(!(*sr&1u))goto L8c073a20;goto L8c073984; /* bf 0x8c073a20 */
L8c073984: *stop=0x8c073984u;r[0]=2u;goto L8c073986; /* mov #0x2,r0 */
L8c073986: *stop=0x8c073986u;if(!valid_access(r[11]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073988; /* mov.l @(0x4,r11),r1 */
L8c073988: *stop=0x8c073988u;r[0]<<=8;goto L8c07398a; /* shll8 r0 */
L8c07398a: *stop=0x8c07398au;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07398c; /* mov.l @(0x8,r11),r3 */
L8c07398c: *stop=0x8c07398cu;r[0]+=12u;goto L8c07398e; /* add #0xc,r0 */
L8c07398e: *stop=0x8c07398eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073990; /* mov.l 0x8c0739e4,r2 */
L8c073990: *stop=0x8c073990u;r[0]+=r[15];goto L8c073992; /* add r15,r0 */
L8c073992: *stop=0x8c073992u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c073994; /* mov.l r1,@r0 */
L8c073994: *stop=0x8c073994u;r[4]=2u;goto L8c073996; /* mov #0x2,r4 */
L8c073996: *stop=0x8c073996u;r[4]<<=8;goto L8c073998; /* shll8 r4 */
L8c073998: *stop=0x8c073998u;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[3],4);if(io_status)return io_status;goto L8c07399a; /* mov.l r3,@(0x4,r0) */
L8c07399a: *stop=0x8c07399au;if(!valid_access(r[11]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07399c; /* mov.l @(0xc,r11),r1 */
L8c07399c: *stop=0x8c07399cu;r[4]+=12u;goto L8c07399e; /* add #0xc,r4 */
L8c07399e: *stop=0x8c07399eu;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[1],4);if(io_status)return io_status;goto L8c0739a0; /* mov.l r1,@(0x8,r0) */
L8c0739a0: target=r[2];*pr=0x8c0739a4u;*stop=0x8c0739a2u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0739a4; /* jsr @r2 */
L8c0739a4: *stop=0x8c0739a4u;r[4]=2u;goto L8c0739a6; /* mov #0x2,r4 */
L8c0739a6: *stop=0x8c0739a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0739f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0739a8; /* mov.l 0x8c0739f0,r3 */
L8c0739a8: *stop=0x8c0739a8u;r[4]<<=8;goto L8c0739aa; /* shll8 r4 */
L8c0739aa: *stop=0x8c0739aau;r[5]=r[14];goto L8c0739ac; /* mov r14,r5 */
L8c0739ac: *stop=0x8c0739acu;r[5]+=72u;goto L8c0739ae; /* add #0x48,r5 */
L8c0739ae: *stop=0x8c0739aeu;r[4]+=12u;goto L8c0739b0; /* add #0xc,r4 */
L8c0739b0: target=r[3];*pr=0x8c0739b4u;*stop=0x8c0739b2u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0739b4; /* jsr @r3 */
L8c0739b4: *stop=0x8c0739b4u;r[0]=0x8c073a04u;goto L8c0739b6; /* mova 0x8c073a04,r0 */
L8c0739b6: *stop=0x8c0739b6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0739b8; /* fmov.s @r0,fr3 */
L8c0739b8: *stop=0x8c0739b8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0739ba; /* fcmp/gt fr3,fr0 */
L8c0739ba: if(!(*sr&1u))goto L8c073a20;goto L8c0739bc; /* bf 0x8c073a20 */
L8c0739bc: *stop=0x8c0739bcu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0739be; /* mov.l @r10,r2 */
L8c0739be: *stop=0x8c0739beu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c0739c0; /* mov.l r2,@-r15 */
L8c0739c0: *stop=0x8c0739c0u;r[2]=2u;goto L8c0739c2; /* mov #0x2,r2 */
L8c0739c2: *stop=0x8c0739c2u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0739c4; /* mov.l @(0x18,r15),r3 */
L8c0739c4: *stop=0x8c0739c4u;r[2]<<=8;goto L8c0739c6; /* shll8 r2 */
L8c0739c6: *stop=0x8c0739c6u;r[2]+=44u;goto L8c0739c8; /* add #0x2c,r2 */
L8c0739c8: *stop=0x8c0739c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0739ca; /* mov.l r3,@-r15 */
L8c0739ca: *stop=0x8c0739cau;r[3]=2u;goto L8c0739cc; /* mov #0x2,r3 */
L8c0739cc: *stop=0x8c0739ccu;r[3]<<=8;goto L8c0739ce; /* shll8 r3 */
L8c0739ce: *stop=0x8c0739ceu;r[2]+=r[15];goto L8c0739d0; /* add r15,r2 */
L8c0739d0: *stop=0x8c0739d0u;r[3]+=12u;goto L8c0739d2; /* add #0xc,r3 */
L8c0739d2: *stop=0x8c0739d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c0739d4; /* mov.l r2,@-r15 */
L8c0739d4: *stop=0x8c0739d4u;r[3]+=r[15];goto L8c0739d6; /* add r15,r3 */
L8c0739d6: *stop=0x8c0739d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0739d8; /* mov.l r3,@-r15 */
L8c0739d8: *stop=0x8c0739dau;goto L8c0740fe; /* bra 0x8c0740fe */
L8c073a20: *stop=0x8c073a20u;r[0]=60u;goto L8c073a22; /* mov #0x3c,r0 */
L8c073a22: *stop=0x8c073a22u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c073a24; /* fldi0 fr3 */
L8c073a24: *stop=0x8c073a24u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c073a26; /* fmov.s @(r0,r12),fr4 */
L8c073a26: *stop=0x8c073a26u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c073a28; /* fcmp/gt fr3,fr4 */
L8c073a28: if(!(*sr&1u))goto L8c073a3c;goto L8c073a2a; /* bf 0x8c073a3c */
L8c073a2a: *stop=0x8c073a2au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c073a2c; /* fcmp/gt fr3,fr4 */
L8c073a2c: if((*sr&1u))goto L8c073a32;goto L8c073a2e; /* bt 0x8c073a32 */
L8c073a2e: *stop=0x8c073a30u;goto L8c0740e2; /* bra 0x8c0740e2 */
L8c073a32: *stop=0x8c073a32u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c073a34; /* mov.w @(0x10,r12),r0 */
L8c073a34: *stop=0x8c073a34u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c073a36; /* tst #0x3,r0 */
L8c073a36: if((*sr&1u))goto L8c073a3c;goto L8c073a38; /* bt 0x8c073a3c */
L8c073a38: *stop=0x8c073a3au;goto L8c0740e2; /* bra 0x8c0740e2 */
L8c073a3c: *stop=0x8c073a3cu;r[0]=76u;goto L8c073a3e; /* mov #0x4c,r0 */
L8c073a3e: *stop=0x8c073a3eu;r[5]=r[11];goto L8c073a40; /* mov r11,r5 */
L8c073a40: *stop=0x8c073a40u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[8],4);if(io_status)return io_status;goto L8c073a42; /* mov.l r8,@(r0,r15) */
L8c073a42: *stop=0x8c073a42u;r[6]=r[12];goto L8c073a44; /* mov r12,r6 */
L8c073a44: *stop=0x8c073a44u;if(!valid_access(r[10]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073a46; /* mov.l @(0x4,r10),r3 */
L8c073a46: *stop=0x8c073a46u;r[7]=r[14];goto L8c073a48; /* mov r14,r7 */
L8c073a48: *stop=0x8c073a48u;if(!valid_access(r[15]+56u,4u))return -3;wr(m,bus,&io_status,r[15]+0x38u,r[3],4);if(io_status)return io_status;goto L8c073a4a; /* mov.l r3,@(0x38,r15) */
L8c073a4a: *stop=0x8c073a4au;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c073a4c; /* mov.l @r10,r2 */
L8c073a4c: *stop=0x8c073a4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073a4e; /* mov.l 0x8c073b40,r3 */
L8c073a4e: *stop=0x8c073a4eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c073a50; /* mov.l r2,@-r15 */
L8c073a50: target=r[3];*pr=0x8c073a54u;*stop=0x8c073a52u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073a54; /* jsr @r3 */
L8c073a54: *stop=0x8c073a54u;r[4]=r[0];goto L8c073a56; /* mov r0,r4 */
L8c073a56: *stop=0x8c073a56u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c073a58; /* tst r4,r4 */
L8c073a58: take=(*sr&1u);*stop=0x8c073a5au;r[15]+=4u;if(take)goto L8c073a6e;goto L8c073a5c; /* bt/s 0x8c073a6e */
L8c073a5c: *stop=0x8c073a5cu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073a5e; /* mov.l @(0x14,r15),r3 */
L8c073a5e: *stop=0x8c073a5eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c073a60; /* tst r3,r3 */
L8c073a60: if(!(*sr&1u))goto L8c073a66;goto L8c073a62; /* bf 0x8c073a66 */
L8c073a62: *stop=0x8c073a64u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[4],4);if(io_status)return io_status;goto L8c073a6e; /* bra 0x8c073a6e */
L8c073a66: *stop=0x8c073a66u;r[0]=r[4];goto L8c073a68; /* mov r4,r0 */
L8c073a68: *stop=0x8c073a68u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c073a6a; /* cmp/eq #0x2,r0 */
L8c073a6a: if(!(*sr&1u))goto L8c073a6e;goto L8c073a6c; /* bf 0x8c073a6e */
L8c073a6c: *stop=0x8c073a6cu;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[4],4);if(io_status)return io_status;goto L8c073a6e; /* mov.l r4,@(0x14,r15) */
L8c073a6e: *stop=0x8c073a6eu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c073a70; /* mov.l @r10,r2 */
L8c073a70: *stop=0x8c073a70u;r[3]=64u;goto L8c073a72; /* mov #0x40,r3 */
L8c073a72: *stop=0x8c073a72u;r[3]<<=8;goto L8c073a74; /* shll8 r3 */
L8c073a74: *stop=0x8c073a74u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c073a76; /* tst r3,r2 */
L8c073a76: if((*sr&1u))goto L8c073a7e;goto L8c073a78; /* bt 0x8c073a7e */
L8c073a78: *stop=0x8c073a78u;if(!valid_access(r[13]+28u,4u))return -3;wr(m,bus,&io_status,r[13]+0x1cu,r[9],4);if(io_status)return io_status;goto L8c073a7a; /* mov.l r9,@(0x1c,r13) */
L8c073a7a: *stop=0x8c073a7cu;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[9],4);if(io_status)return io_status;goto L8c073c1e; /* bra 0x8c073c1e */
L8c073a7e: *stop=0x8c073a7eu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[1]=loaded;}goto L8c073a80; /* mov.l @r10,r1 */
L8c073a80: *stop=0x8c073a80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b44u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073a82; /* mov.l 0x8c073b44,r2 */
L8c073a82: *stop=0x8c073a82u;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c073a84; /* tst r2,r1 */
L8c073a84: if(!(*sr&1u))goto L8c073a90;goto L8c073a86; /* bf 0x8c073a90 */
L8c073a86: *stop=0x8c073a86u;r[1]=32u;goto L8c073a88; /* mov #0x20,r1 */
L8c073a88: *stop=0x8c073a88u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c073a8a; /* mov.w @(0x10,r12),r0 */
L8c073a8a: *stop=0x8c073a8au;r[1]<<=8;goto L8c073a8c; /* shll8 r1 */
L8c073a8c: *stop=0x8c073a8cu;*sr=(*sr&~1u)|!(r[1]&r[0]);goto L8c073a8e; /* tst r1,r0 */
L8c073a8e: if((*sr&1u))goto L8c073afc;goto L8c073a90; /* bt 0x8c073afc */
L8c073a90: *stop=0x8c073a90u;r[0]=76u;goto L8c073a92; /* mov #0x4c,r0 */
L8c073a92: *stop=0x8c073a92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073a94; /* mov.l 0x8c073b48,r3 */
L8c073a94: *stop=0x8c073a94u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c073a96; /* fmov.s @(r0,r14),fr5 */
L8c073a96: *stop=0x8c073a96u;r[0]=72u;goto L8c073a98; /* mov #0x48,r0 */
L8c073a98: target=r[3];*pr=0x8c073a9cu;*stop=0x8c073a9au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073a9c; /* jsr @r3 */
L8c073a9c: *stop=0x8c073a9cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c073a9e; /* fmul fr14,fr0 */
L8c073a9e: *stop=0x8c073a9eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b4cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073aa0; /* mov.l 0x8c073b4c,r3 */
L8c073aa0: *stop=0x8c073aa0u;r[0]=80u;goto L8c073aa2; /* mov #0x50,r0 */
L8c073aa2: *stop=0x8c073aa2u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c073aa4; /* ftrc fr0,FPUL */
L8c073aa4: *stop=0x8c073aa4u;r[2]=*fpul;goto L8c073aa6; /* sts FPUL,r2 */
L8c073aa6: *stop=0x8c073aa6u;r[2]=0u-r[2];goto L8c073aa8; /* neg r2,r2 */
L8c073aa8: *stop=0x8c073aa8u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[2],4);if(io_status)return io_status;goto L8c073aaa; /* mov.l r2,@(0x38,r14) */
L8c073aaa: target=r[3];*pr=0x8c073aaeu;*stop=0x8c073aacu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073aae; /* jsr @r3 */
L8c073aae: *stop=0x8c073aaeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c073ab0; /* fmul fr14,fr0 */
L8c073ab0: *stop=0x8c073ab0u;r[0]=4u;goto L8c073ab2; /* mov #0x4,r0 */
L8c073ab2: *stop=0x8c073ab2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c073ab4; /* fldi0 fr3 */
L8c073ab4: *stop=0x8c073ab4u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c073ab6; /* ftrc fr0,FPUL */
L8c073ab6: *stop=0x8c073ab6u;r[2]=*fpul;goto L8c073ab8; /* sts FPUL,r2 */
L8c073ab8: *stop=0x8c073ab8u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[2],4);if(io_status)return io_status;goto L8c073aba; /* mov.l r2,@(0x34,r14) */
L8c073aba: *stop=0x8c073abau;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c073abc; /* fmov.s @(r0,r11),fr2 */
L8c073abc: *stop=0x8c073abcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c073abe; /* fcmp/eq fr3,fr2 */
L8c073abe: if(!(*sr&1u))goto L8c073aee;goto L8c073ac0; /* bf 0x8c073aee */
L8c073ac0: *stop=0x8c073ac0u;r[0]=12u;goto L8c073ac2; /* mov #0xc,r0 */
L8c073ac2: *stop=0x8c073ac2u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c073ac4; /* fmov.s @(r0,r11),fr2 */
L8c073ac4: *stop=0x8c073ac4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c073ac6; /* fcmp/eq fr3,fr2 */
L8c073ac6: if(!(*sr&1u))goto L8c073aee;goto L8c073ac8; /* bf 0x8c073aee */
L8c073ac8: *stop=0x8c073ac8u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c073aca; /* mov.w @(0x10,r12),r0 */
L8c073aca: *stop=0x8c073acau;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c073acc; /* tst #0x1,r0 */
L8c073acc: if((*sr&1u))goto L8c073aee;goto L8c073ace; /* bt 0x8c073aee */
L8c073ace: *stop=0x8c073aceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b54u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073ad0; /* mov.l 0x8c073b54,r2 */
L8c073ad0: *stop=0x8c073ad0u;r[5]=r[14];goto L8c073ad2; /* mov r14,r5 */
L8c073ad2: *stop=0x8c073ad2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b50u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c073ad4; /* mov.l 0x8c073b50,r4 */
L8c073ad4: target=r[2];*pr=0x8c073ad8u;*stop=0x8c073ad6u;r[5]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073ad8; /* jsr @r2 */
L8c073ad8: *stop=0x8c073ad8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b50u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c073ada; /* mov.l 0x8c073b50,r4 */
L8c073ada: *stop=0x8c073adau;r[5]=r[14];goto L8c073adc; /* mov r14,r5 */
L8c073adc: *stop=0x8c073adcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b54u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073ade; /* mov.l 0x8c073b54,r3 */
L8c073ade: *stop=0x8c073adeu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c073ae0; /* fmov.s fr0,@-r15 */
L8c073ae0: target=r[3];*pr=0x8c073ae4u;*stop=0x8c073ae2u;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073ae4; /* jsr @r3 */
L8c073ae4: *stop=0x8c073ae4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c073ae6; /* fmov.s @r15+,fr3 */
L8c073ae6: *stop=0x8c073ae6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c073ae8; /* fcmp/gt fr3,fr0 */
L8c073ae8: if((*sr&1u))goto L8c073aee;goto L8c073aea; /* bt 0x8c073aee */
L8c073aea: *stop=0x8c073aecu;goto L8c073c1e; /* bra 0x8c073c1e */
L8c073aee: *stop=0x8c073aeeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073af0; /* mov.l 0x8c073b58,r2 */
L8c073af0: *stop=0x8c073af0u;r[5]=r[12];goto L8c073af2; /* mov r12,r5 */
L8c073af2: *stop=0x8c073af2u;r[6]=r[14];goto L8c073af4; /* mov r14,r6 */
L8c073af4: target=r[2];*pr=0x8c073af8u;*stop=0x8c073af6u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073af8; /* jsr @r2 */
L8c073af8: *stop=0x8c073afau;goto L8c073c1e; /* bra 0x8c073c1e */
L8c073afc: *stop=0x8c073afcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b4cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073afe; /* mov.l 0x8c073b4c,r3 */
L8c073afe: *stop=0x8c073afeu;r[0]=80u;goto L8c073b00; /* mov #0x50,r0 */
L8c073b00: target=r[3];*pr=0x8c073b04u;*stop=0x8c073b02u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073b04; /* jsr @r3 */
L8c073b04: *stop=0x8c073b04u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c073b06; /* fmul fr14,fr0 */
L8c073b06: *stop=0x8c073b06u;r[0]=76u;goto L8c073b08; /* mov #0x4c,r0 */
L8c073b08: *stop=0x8c073b08u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c073b0a; /* fldi0 fr3 */
L8c073b0a: *stop=0x8c073b0au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c073b0c; /* ftrc fr0,FPUL */
L8c073b0c: *stop=0x8c073b0cu;r[2]=*fpul;goto L8c073b0e; /* sts FPUL,r2 */
L8c073b0e: *stop=0x8c073b0eu;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[2],4);if(io_status)return io_status;goto L8c073b10; /* mov.l r2,@(0x34,r14) */
L8c073b10: *stop=0x8c073b10u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073b12; /* fmov.s @(r0,r14),fr2 */
L8c073b12: *stop=0x8c073b12u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c073b14; /* fcmp/gt fr2,fr3 */
L8c073b14: if((*sr&1u))goto L8c073b60;goto L8c073b16; /* bt 0x8c073b60 */
L8c073b16: *stop=0x8c073b16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073b4cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073b18; /* mov.l 0x8c073b4c,r3 */
L8c073b18: *stop=0x8c073b18u;r[0]=72u;goto L8c073b1a; /* mov #0x48,r0 */
L8c073b1a: target=r[3];*pr=0x8c073b1eu;*stop=0x8c073b1cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073b1e; /* jsr @r3 */
L8c073b1e: *stop=0x8c073b1eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c073b20; /* fmul fr14,fr0 */
L8c073b20: *stop=0x8c073b20u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c073b22; /* ftrc fr0,FPUL */
L8c073b22: *stop=0x8c073b22u;r[2]=*fpul;goto L8c073b24; /* sts FPUL,r2 */
L8c073b24: *stop=0x8c073b26u;r[2]=0u-r[2];goto L8c073b72; /* bra 0x8c073b72 */
L8c073b60: *stop=0x8c073b60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d00u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073b62; /* mov.l 0x8c073d00,r3 */
L8c073b62: *stop=0x8c073b62u;r[0]=72u;goto L8c073b64; /* mov #0x48,r0 */
L8c073b64: target=r[3];*pr=0x8c073b68u;*stop=0x8c073b66u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073b68; /* jsr @r3 */
L8c073b68: *stop=0x8c073b68u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c073b6a; /* fmul fr14,fr0 */
L8c073b6a: *stop=0x8c073b6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073b6c; /* mov.l 0x8c073d04,r3 */
L8c073b6c: *stop=0x8c073b6cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c073b6e; /* ftrc fr0,FPUL */
L8c073b6e: *stop=0x8c073b6eu;r[2]=*fpul;goto L8c073b70; /* sts FPUL,r2 */
L8c073b70: *stop=0x8c073b70u;r[2]+=r[3];goto L8c073b72; /* add r3,r2 */
L8c073b72: *stop=0x8c073b72u;r[0]=4u;goto L8c073b74; /* mov #0x4,r0 */
L8c073b74: *stop=0x8c073b74u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c073b76; /* fldi0 fr3 */
L8c073b76: *stop=0x8c073b76u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[2],4);if(io_status)return io_status;goto L8c073b78; /* mov.l r2,@(0x38,r14) */
L8c073b78: *stop=0x8c073b78u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c073b7a; /* fmov.s @(r0,r11),fr2 */
L8c073b7a: *stop=0x8c073b7au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c073b7c; /* fcmp/eq fr3,fr2 */
L8c073b7c: if(!(*sr&1u))goto L8c073ba8;goto L8c073b7e; /* bf 0x8c073ba8 */
L8c073b7e: *stop=0x8c073b7eu;r[0]=12u;goto L8c073b80; /* mov #0xc,r0 */
L8c073b80: *stop=0x8c073b80u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c073b82; /* fmov.s @(r0,r11),fr2 */
L8c073b82: *stop=0x8c073b82u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c073b84; /* fcmp/eq fr3,fr2 */
L8c073b84: if(!(*sr&1u))goto L8c073ba8;goto L8c073b86; /* bf 0x8c073ba8 */
L8c073b86: *stop=0x8c073b86u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c073b88; /* mov.w @(0x10,r12),r0 */
L8c073b88: *stop=0x8c073b88u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c073b8a; /* tst #0x1,r0 */
L8c073b8a: if((*sr&1u))goto L8c073ba8;goto L8c073b8c; /* bt 0x8c073ba8 */
L8c073b8c: *stop=0x8c073b8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d0cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073b8e; /* mov.l 0x8c073d0c,r2 */
L8c073b8e: *stop=0x8c073b8eu;r[5]=r[14];goto L8c073b90; /* mov r14,r5 */
L8c073b90: *stop=0x8c073b90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d08u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c073b92; /* mov.l 0x8c073d08,r4 */
L8c073b92: target=r[2];*pr=0x8c073b96u;*stop=0x8c073b94u;r[5]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073b96; /* jsr @r2 */
L8c073b96: *stop=0x8c073b96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d08u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c073b98; /* mov.l 0x8c073d08,r4 */
L8c073b98: *stop=0x8c073b98u;r[5]=r[14];goto L8c073b9a; /* mov r14,r5 */
L8c073b9a: *stop=0x8c073b9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d0cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073b9c; /* mov.l 0x8c073d0c,r3 */
L8c073b9c: *stop=0x8c073b9cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c073b9e; /* fmov.s fr0,@-r15 */
L8c073b9e: target=r[3];*pr=0x8c073ba2u;*stop=0x8c073ba0u;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073ba2; /* jsr @r3 */
L8c073ba2: *stop=0x8c073ba2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c073ba4; /* fmov.s @r15+,fr3 */
L8c073ba4: *stop=0x8c073ba4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c073ba6; /* fcmp/gt fr3,fr0 */
L8c073ba6: if(!(*sr&1u))goto L8c073c1e;goto L8c073ba8; /* bf 0x8c073c1e */
L8c073ba8: *stop=0x8c073ba8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d00u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073baa; /* mov.l 0x8c073d00,r3 */
L8c073baa: *stop=0x8c073baau;r[0]=80u;goto L8c073bac; /* mov #0x50,r0 */
L8c073bac: target=r[3];*pr=0x8c073bb0u;*stop=0x8c073baeu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073bb0; /* jsr @r3 */
L8c073bb0: *stop=0x8c073bb0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c073bb2; /* fmul fr14,fr0 */
L8c073bb2: *stop=0x8c073bb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d00u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073bb4; /* mov.l 0x8c073d00,r3 */
L8c073bb4: *stop=0x8c073bb4u;r[0]=72u;goto L8c073bb6; /* mov #0x48,r0 */
L8c073bb6: *stop=0x8c073bb6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c073bb8; /* ftrc fr0,FPUL */
L8c073bb8: *stop=0x8c073bb8u;r[2]=*fpul;goto L8c073bba; /* sts FPUL,r2 */
L8c073bba: *stop=0x8c073bbau;if(!valid_access(r[15]+40u,4u))return -3;wr(m,bus,&io_status,r[15]+0x28u,r[2],4);if(io_status)return io_status;goto L8c073bbc; /* mov.l r2,@(0x28,r15) */
L8c073bbc: target=r[3];*pr=0x8c073bc0u;*stop=0x8c073bbeu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073bc0; /* jsr @r3 */
L8c073bc0: *stop=0x8c073bc0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c073bc2; /* fmul fr14,fr0 */
L8c073bc2: *stop=0x8c073bc2u;r[0]=76u;goto L8c073bc4; /* mov #0x4c,r0 */
L8c073bc4: *stop=0x8c073bc4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c073bc6; /* fldi0 fr3 */
L8c073bc6: *stop=0x8c073bc6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c073bc8; /* ftrc fr0,FPUL */
L8c073bc8: *stop=0x8c073bc8u;r[2]=*fpul;goto L8c073bca; /* sts FPUL,r2 */
L8c073bca: *stop=0x8c073bcau;r[2]=0u-r[2];goto L8c073bcc; /* neg r2,r2 */
L8c073bcc: *stop=0x8c073bccu;if(!valid_access(r[15]+44u,4u))return -3;wr(m,bus,&io_status,r[15]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c073bce; /* mov.l r2,@(0x2c,r15) */
L8c073bce: *stop=0x8c073bceu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073bd0; /* fmov.s @(r0,r14),fr2 */
L8c073bd0: *stop=0x8c073bd0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c073bd2; /* fcmp/gt fr2,fr3 */
L8c073bd2: if(!(*sr&1u))goto L8c073c14;goto L8c073bd4; /* bf 0x8c073c14 */
L8c073bd4: *stop=0x8c073bd4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d04u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073bd6; /* mov.l 0x8c073d04,r2 */
L8c073bd6: *stop=0x8c073bd6u;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073bd8; /* mov.l @(0x28,r15),r3 */
L8c073bd8: *stop=0x8c073bd8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d04u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c073bda; /* mov.l 0x8c073d04,r0 */
L8c073bda: *stop=0x8c073bdau;r[2]-=r[3];goto L8c073bdc; /* sub r3,r2 */
L8c073bdc: *stop=0x8c073bdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073bde; /* mov.l 0x8c073d10,r3 */
L8c073bde: *stop=0x8c073bdeu;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[2],4);if(io_status)return io_status;goto L8c073be0; /* mov.l r2,@(0x18,r15) */
L8c073be0: *stop=0x8c073be0u;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073be2; /* mov.l @(0x2c,r15),r1 */
L8c073be2: *stop=0x8c073be2u;r[0]-=r[1];goto L8c073be4; /* sub r1,r0 */
L8c073be4: *stop=0x8c073be4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c073be6; /* mov.l r0,@(0x4,r15) */
L8c073be6: *stop=0x8c073be6u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c073be8; /* mov.l @(0x18,r15),r5 */
L8c073be8: target=r[3];*pr=0x8c073becu;*stop=0x8c073beau;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073bec; /* jsr @r3 */
L8c073bec: *stop=0x8c073becu;r[1]=r[15];goto L8c073bee; /* mov r15,r1 */
L8c073bee: *stop=0x8c073beeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073bf0; /* mov.l 0x8c073d10,r3 */
L8c073bf0: *stop=0x8c073bf0u;r[1]+=68u;goto L8c073bf2; /* add #0x44,r1 */
L8c073bf2: *stop=0x8c073bf2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c073bf4; /* mov.l r0,@r1 */
L8c073bf4: *stop=0x8c073bf4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c073bf6; /* mov.l @(0x4,r15),r5 */
L8c073bf6: target=r[3];*pr=0x8c073bfau;*stop=0x8c073bf8u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073bfa; /* jsr @r3 */
L8c073bfa: *stop=0x8c073bfau;r[4]=r[0];goto L8c073bfc; /* mov r0,r4 */
L8c073bfc: *stop=0x8c073bfcu;r[0]=68u;goto L8c073bfe; /* mov #0x44,r0 */
L8c073bfe: *stop=0x8c073bfeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c073c00; /* mov.l @(r0,r15),r2 */
L8c073c00: *stop=0x8c073c00u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[4]);goto L8c073c02; /* cmp/ge r4,r2 */
L8c073c02: if((*sr&1u))goto L8c073c0c;goto L8c073c04; /* bt 0x8c073c0c */
L8c073c04: *stop=0x8c073c04u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073c06; /* mov.l @(0x18,r15),r1 */
L8c073c06: *stop=0x8c073c06u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[1],4);if(io_status)return io_status;goto L8c073c08; /* mov.l r1,@(0x34,r14) */
L8c073c08: *stop=0x8c073c0au;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073c12; /* bra 0x8c073c12 */
L8c073c0c: *stop=0x8c073c0cu;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073c0e; /* mov.l @(0x28,r15),r1 */
L8c073c0e: *stop=0x8c073c0eu;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[1],4);if(io_status)return io_status;goto L8c073c10; /* mov.l r1,@(0x34,r14) */
L8c073c10: *stop=0x8c073c10u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073c12; /* mov.l @(0x4,r15),r3 */
L8c073c12: *stop=0x8c073c12u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[3],4);if(io_status)return io_status;goto L8c073c14; /* mov.l r3,@(0x38,r14) */
L8c073c14: *stop=0x8c073c14u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073c16; /* mov.l 0x8c073d14,r3 */
L8c073c16: *stop=0x8c073c16u;r[5]=r[12];goto L8c073c18; /* mov r12,r5 */
L8c073c18: *stop=0x8c073c18u;r[6]=r[14];goto L8c073c1a; /* mov r14,r6 */
L8c073c1a: target=r[3];*pr=0x8c073c1eu;*stop=0x8c073c1cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073c1e; /* jsr @r3 */
L8c073c1e: *stop=0x8c073c1eu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073c20; /* mov.l @(0x14,r15),r2 */
L8c073c20: *stop=0x8c073c20u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c073c22; /* tst r2,r2 */
L8c073c22: if((*sr&1u))goto L8c073c28;goto L8c073c24; /* bt 0x8c073c28 */
L8c073c24: *stop=0x8c073c26u;goto L8c073d4e; /* bra 0x8c073d4e */
L8c073c28: *stop=0x8c073c28u;r[0]=36u;goto L8c073c2a; /* mov #0x24,r0 */
L8c073c2a: *stop=0x8c073c2au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c073c2c; /* fmov fr15,@(r0,r14) */
L8c073c2c: *stop=0x8c073c2cu;r[0]=28u;goto L8c073c2e; /* mov #0x1c,r0 */
L8c073c2e: *stop=0x8c073c2eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c073c30; /* fmov fr15,@(r0,r14) */
L8c073c30: *stop=0x8c073c30u;r[0]=32u;goto L8c073c32; /* mov #0x20,r0 */
L8c073c32: *stop=0x8c073c32u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,13u,r[0]+r[14]);if(io_status)return io_status;goto L8c073c34; /* fmov fr13,@(r0,r14) */
L8c073c34: *stop=0x8c073c34u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073c36; /* mov.l @(0x14,r13),r3 */
L8c073c36: *stop=0x8c073c36u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[3],4);if(io_status)return io_status;goto L8c073c38; /* mov.l r3,@(0x34,r14) */
L8c073c38: *stop=0x8c073c38u;r[3]=64u;goto L8c073c3a; /* mov #0x40,r3 */
L8c073c3a: *stop=0x8c073c3au;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073c3c; /* mov.l @(0x1c,r13),r2 */
L8c073c3c: *stop=0x8c073c3cu;r[3]<<=8;goto L8c073c3e; /* shll8 r3 */
L8c073c3e: *stop=0x8c073c3eu;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[2],4);if(io_status)return io_status;goto L8c073c40; /* mov.l r2,@(0x38,r14) */
L8c073c40: *stop=0x8c073c40u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[1]=loaded;}goto L8c073c42; /* mov.l @r10,r1 */
L8c073c42: *stop=0x8c073c42u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c073c44; /* tst r3,r1 */
L8c073c44: if((*sr&1u))goto L8c073c58;goto L8c073c46; /* bt 0x8c073c58 */
L8c073c46: *stop=0x8c073c46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073c48; /* mov.l 0x8c073d18,r1 */
L8c073c48: *stop=0x8c073c48u;r[5]=r[14];goto L8c073c4a; /* mov r14,r5 */
L8c073c4a: *stop=0x8c073c4au;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c073c4c; /* mov.l @(0x18,r13),r6 */
L8c073c4c: *stop=0x8c073c4cu;r[4]=r[14];goto L8c073c4e; /* mov r14,r4 */
L8c073c4e: *stop=0x8c073c4eu;r[5]+=28u;goto L8c073c50; /* add #0x1c,r5 */
L8c073c50: target=r[1];*pr=0x8c073c54u;*stop=0x8c073c52u;r[4]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073c54; /* jsr @r1 */
L8c073c54: *stop=0x8c073c56u;goto L8c073c62; /* bra 0x8c073c62 */
L8c073c58: *stop=0x8c073c58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073c5a; /* mov.l 0x8c073d1c,r2 */
L8c073c5a: *stop=0x8c073c5au;r[5]=r[14];goto L8c073c5c; /* mov r14,r5 */
L8c073c5c: *stop=0x8c073c5cu;r[5]+=28u;goto L8c073c5e; /* add #0x1c,r5 */
L8c073c5e: target=r[2];*pr=0x8c073c62u;*stop=0x8c073c60u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073c62; /* jsr @r2 */
L8c073c62: *stop=0x8c073c62u;r[0]=2u;goto L8c073c64; /* mov #0x2,r0 */
L8c073c64: *stop=0x8c073c64u;r[0]<<=8;goto L8c073c66; /* shll8 r0 */
L8c073c66: *stop=0x8c073c66u;r[0]+=44u;goto L8c073c68; /* add #0x2c,r0 */
L8c073c68: *stop=0x8c073c68u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c073c6a; /* fmov fr15,@(r0,r15) */
L8c073c6a: *stop=0x8c073c6au;r[0]=2u;goto L8c073c6c; /* mov #0x2,r0 */
L8c073c6c: *stop=0x8c073c6cu;r[0]<<=8;goto L8c073c6e; /* shll8 r0 */
L8c073c6e: *stop=0x8c073c6eu;r[0]+=36u;goto L8c073c70; /* add #0x24,r0 */
L8c073c70: *stop=0x8c073c70u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c073c72; /* fmov fr15,@(r0,r15) */
L8c073c72: *stop=0x8c073c72u;r[0]=68u;goto L8c073c74; /* mov #0x44,r0 */
L8c073c74: *stop=0x8c073c74u;r[0]<<=2;goto L8c073c76; /* shll2 r0 */
L8c073c76: *stop=0x8c073c76u;r[3]=2u;goto L8c073c78; /* mov #0x2,r3 */
L8c073c78: *stop=0x8c073c78u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c073c7a; /* fmov.s @(r0,r12),fr3 */
L8c073c7a: *stop=0x8c073c7au;r[3]<<=8;goto L8c073c7c; /* shll8 r3 */
L8c073c7c: *stop=0x8c073c7cu;r[3]+=36u;goto L8c073c7e; /* add #0x24,r3 */
L8c073c7e: *stop=0x8c073c7eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[12],&f->fpscr,&fr[3]);if(status)return status;}goto L8c073c80; /* fmul fr12,fr3 */
L8c073c80: *stop=0x8c073c80u;r[2]=2u;goto L8c073c82; /* mov #0x2,r2 */
L8c073c82: *stop=0x8c073c82u;r[3]+=r[15];goto L8c073c84; /* add r15,r3 */
L8c073c84: *stop=0x8c073c84u;r[2]<<=8;goto L8c073c86; /* shll8 r2 */
L8c073c86: *stop=0x8c073c86u;r[0]=2u;goto L8c073c88; /* mov #0x2,r0 */
L8c073c88: *stop=0x8c073c88u;r[2]+=24u;goto L8c073c8a; /* add #0x18,r2 */
L8c073c8a: *stop=0x8c073c8au;r[0]<<=8;goto L8c073c8c; /* shll8 r0 */
L8c073c8c: *stop=0x8c073c8cu;r[2]+=r[15];goto L8c073c8e; /* add r15,r2 */
L8c073c8e: *stop=0x8c073c8eu;r[0]+=40u;goto L8c073c90; /* add #0x28,r0 */
L8c073c90: *stop=0x8c073c90u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073c92; /* fmov fr3,@(r0,r15) */
L8c073c92: *stop=0x8c073c92u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c073c94; /* mov.l @r3,r1 */
L8c073c94: *stop=0x8c073c94u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c073c96; /* mov.l @(0x4,r3),r0 */
L8c073c96: *stop=0x8c073c96u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c073c98; /* mov.l r1,@r2 */
L8c073c98: *stop=0x8c073c98u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c073c9a; /* mov.l r0,@(0x4,r2) */
L8c073c9a: *stop=0x8c073c9au;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073c9c; /* mov.l @(0x8,r3),r1 */
L8c073c9c: *stop=0x8c073c9cu;r[3]=64u;goto L8c073c9e; /* mov #0x40,r3 */
L8c073c9e: *stop=0x8c073c9eu;r[3]<<=8;goto L8c073ca0; /* shll8 r3 */
L8c073ca0: *stop=0x8c073ca0u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c073ca2; /* mov.l r1,@(0x8,r2) */
L8c073ca2: *stop=0x8c073ca2u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c073ca4; /* mov.l @r10,r2 */
L8c073ca4: *stop=0x8c073ca4u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c073ca6; /* tst r3,r2 */
L8c073ca6: if((*sr&1u))goto L8c073cbe;goto L8c073ca8; /* bt 0x8c073cbe */
L8c073ca8: *stop=0x8c073ca8u;r[5]=2u;goto L8c073caa; /* mov #0x2,r5 */
L8c073caa: *stop=0x8c073caau;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073cac; /* mov.l 0x8c073d18,r2 */
L8c073cac: *stop=0x8c073cacu;r[5]<<=8;goto L8c073cae; /* shll8 r5 */
L8c073cae: *stop=0x8c073caeu;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c073cb0; /* mov.l @(0x18,r13),r6 */
L8c073cb0: *stop=0x8c073cb0u;r[5]+=36u;goto L8c073cb2; /* add #0x24,r5 */
L8c073cb2: *stop=0x8c073cb2u;r[4]=r[14];goto L8c073cb4; /* mov r14,r4 */
L8c073cb4: *stop=0x8c073cb4u;r[5]+=r[15];goto L8c073cb6; /* add r15,r5 */
L8c073cb6: target=r[2];*pr=0x8c073cbau;*stop=0x8c073cb8u;r[4]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073cba; /* jsr @r2 */
L8c073cba: *stop=0x8c073cbcu;goto L8c073ccc; /* bra 0x8c073ccc */
L8c073cbe: *stop=0x8c073cbeu;r[5]=2u;goto L8c073cc0; /* mov #0x2,r5 */
L8c073cc0: *stop=0x8c073cc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073cc2; /* mov.l 0x8c073d1c,r2 */
L8c073cc2: *stop=0x8c073cc2u;r[5]<<=8;goto L8c073cc4; /* shll8 r5 */
L8c073cc4: *stop=0x8c073cc4u;r[5]+=36u;goto L8c073cc6; /* add #0x24,r5 */
L8c073cc6: *stop=0x8c073cc6u;r[5]+=r[15];goto L8c073cc8; /* add r15,r5 */
L8c073cc8: target=r[2];*pr=0x8c073cccu;*stop=0x8c073ccau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073ccc; /* jsr @r2 */
L8c073ccc: *stop=0x8c073cccu;r[7]=2u;goto L8c073cce; /* mov #0x2,r7 */
L8c073cce: *stop=0x8c073cceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073cd0; /* mov.l 0x8c073d20,r3 */
L8c073cd0: *stop=0x8c073cd0u;r[7]<<=8;goto L8c073cd2; /* shll8 r7 */
L8c073cd2: *stop=0x8c073cd2u;r[5]=r[12];goto L8c073cd4; /* mov r12,r5 */
L8c073cd4: *stop=0x8c073cd4u;r[7]+=24u;goto L8c073cd6; /* add #0x18,r7 */
L8c073cd6: *stop=0x8c073cd6u;r[6]=r[14];goto L8c073cd8; /* mov r14,r6 */
L8c073cd8: *stop=0x8c073cd8u;r[7]+=r[15];goto L8c073cda; /* add r15,r7 */
L8c073cda: target=r[3];*pr=0x8c073cdeu;*stop=0x8c073cdcu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073cde; /* jsr @r3 */
L8c073cde: *stop=0x8c073cdeu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c073ce0; /* mov.l @r10,r2 */
L8c073ce0: *stop=0x8c073ce0u;r[3]=64u;goto L8c073ce2; /* mov #0x40,r3 */
L8c073ce2: *stop=0x8c073ce2u;r[3]<<=8;goto L8c073ce4; /* shll8 r3 */
L8c073ce4: *stop=0x8c073ce4u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c073ce6; /* tst r3,r2 */
L8c073ce6: if((*sr&1u))goto L8c073d40;goto L8c073ce8; /* bt 0x8c073d40 */
L8c073ce8: *stop=0x8c073ce8u;r[5]=2u;goto L8c073cea; /* mov #0x2,r5 */
L8c073cea: *stop=0x8c073ceau;{uint32_t loaded=rd(m,bus,&io_status,0x8c073d18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073cec; /* mov.l 0x8c073d18,r2 */
L8c073cec: *stop=0x8c073cecu;r[5]<<=8;goto L8c073cee; /* shll8 r5 */
L8c073cee: *stop=0x8c073ceeu;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c073cf0; /* mov.l @(0x18,r13),r6 */
L8c073cf0: *stop=0x8c073cf0u;r[5]+=24u;goto L8c073cf2; /* add #0x18,r5 */
L8c073cf2: *stop=0x8c073cf2u;r[4]=r[14];goto L8c073cf4; /* mov r14,r4 */
L8c073cf4: *stop=0x8c073cf4u;r[5]+=r[15];goto L8c073cf6; /* add r15,r5 */
L8c073cf6: target=r[2];*pr=0x8c073cfau;*stop=0x8c073cf8u;r[4]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073cfa; /* jsr @r2 */
L8c073cfa: *stop=0x8c073cfcu;goto L8c073d4e; /* bra 0x8c073d4e */
L8c073d40: *stop=0x8c073d40u;r[5]=2u;goto L8c073d42; /* mov #0x2,r5 */
L8c073d42: *stop=0x8c073d42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073f80u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073d44; /* mov.l 0x8c073f80,r2 */
L8c073d44: *stop=0x8c073d44u;r[5]<<=8;goto L8c073d46; /* shll8 r5 */
L8c073d46: *stop=0x8c073d46u;r[5]+=24u;goto L8c073d48; /* add #0x18,r5 */
L8c073d48: *stop=0x8c073d48u;r[5]+=r[15];goto L8c073d4a; /* add r15,r5 */
L8c073d4a: target=r[2];*pr=0x8c073d4eu;*stop=0x8c073d4cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073d4e; /* jsr @r2 */
L8c073d4e: *stop=0x8c073d4eu;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073d50; /* mov.l @(0x34,r14),r3 */
L8c073d50: *stop=0x8c073d50u;r[1]=r[12];goto L8c073d52; /* mov r12,r1 */
L8c073d52: *stop=0x8c073d52u;r[1]+=80u;goto L8c073d54; /* add #0x50,r1 */
L8c073d54: *stop=0x8c073d54u;if(!valid_access(r[11]+28u,4u))return -3;wr(m,bus,&io_status,r[11]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c073d56; /* mov.l r3,@(0x1c,r11) */
L8c073d56: *stop=0x8c073d56u;r[3]=r[14];goto L8c073d58; /* mov r14,r3 */
L8c073d58: *stop=0x8c073d58u;r[3]+=72u;goto L8c073d5a; /* add #0x48,r3 */
L8c073d5a: *stop=0x8c073d5au;if(!valid_access(r[14]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073d5c; /* mov.l @(0x38,r14),r2 */
L8c073d5c: *stop=0x8c073d5cu;if(!valid_access(r[11]+36u,4u))return -3;wr(m,bus,&io_status,r[11]+0x24u,r[2],4);if(io_status)return io_status;goto L8c073d5e; /* mov.l r2,@(0x24,r11) */
L8c073d5e: *stop=0x8c073d5eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c073d60; /* mov.l @r3,r0 */
L8c073d60: *stop=0x8c073d60u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073d62; /* mov.l @(0x4,r3),r2 */
L8c073d62: *stop=0x8c073d62u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c073d64; /* mov.l r0,@r1 */
L8c073d64: *stop=0x8c073d64u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[2],4);if(io_status)return io_status;goto L8c073d66; /* mov.l r2,@(0x4,r1) */
L8c073d66: *stop=0x8c073d66u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c073d68; /* mov.l @(0x8,r3),r0 */
L8c073d68: *stop=0x8c073d68u;r[3]=64u;goto L8c073d6a; /* mov #0x40,r3 */
L8c073d6a: *stop=0x8c073d6au;r[3]<<=8;goto L8c073d6c; /* shll8 r3 */
L8c073d6c: *stop=0x8c073d6cu;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c073d6e; /* mov.l r0,@(0x8,r1) */
L8c073d6e: *stop=0x8c073d6eu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[1]=loaded;}goto L8c073d70; /* mov.l @r10,r1 */
L8c073d70: *stop=0x8c073d70u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c073d72; /* tst r3,r1 */
L8c073d72: if((*sr&1u))goto L8c073dec;goto L8c073d74; /* bt 0x8c073dec */
L8c073d74: *stop=0x8c073d74u;r[2]=2u;goto L8c073d76; /* mov #0x2,r2 */
L8c073d76: *stop=0x8c073d76u;r[0]=r[14];goto L8c073d78; /* mov r14,r0 */
L8c073d78: *stop=0x8c073d78u;r[2]<<=8;goto L8c073d7a; /* shll8 r2 */
L8c073d7a: *stop=0x8c073d7au;r[0]+=108u;goto L8c073d7c; /* add #0x6c,r0 */
L8c073d7c: *stop=0x8c073d7cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c073d7e; /* mov.l @r0,r1 */
L8c073d7e: *stop=0x8c073d7eu;r[2]+=12u;goto L8c073d80; /* add #0xc,r2 */
L8c073d80: *stop=0x8c073d80u;r[2]+=r[15];goto L8c073d82; /* add r15,r2 */
L8c073d82: *stop=0x8c073d82u;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073d84; /* mov.l @(0x4,r0),r3 */
L8c073d84: *stop=0x8c073d84u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c073d86; /* mov.l r1,@r2 */
L8c073d86: *stop=0x8c073d86u;r[4]=2u;goto L8c073d88; /* mov #0x2,r4 */
L8c073d88: *stop=0x8c073d88u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c073d8a; /* mov.l r3,@(0x4,r2) */
L8c073d8a: *stop=0x8c073d8au;r[4]<<=8;goto L8c073d8c; /* shll8 r4 */
L8c073d8c: *stop=0x8c073d8cu;if(!valid_access(r[0]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073d8e; /* mov.l @(0x8,r0),r1 */
L8c073d8e: *stop=0x8c073d8eu;r[4]+=12u;goto L8c073d90; /* add #0xc,r4 */
L8c073d90: *stop=0x8c073d90u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c073d92; /* mov.l r1,@(0x8,r2) */
L8c073d92: *stop=0x8c073d92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073f84u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073d94; /* mov.l 0x8c073f84,r2 */
L8c073d94: target=r[2];*pr=0x8c073d98u;*stop=0x8c073d96u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073d98; /* jsr @r2 */
L8c073d98: *stop=0x8c073d98u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c073d9a; /* fldi0 fr3 */
L8c073d9a: *stop=0x8c073d9au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c073d9c; /* fcmp/eq fr3,fr0 */
L8c073d9c: if((*sr&1u))goto L8c073dec;goto L8c073d9e; /* bt 0x8c073dec */
L8c073d9e: *stop=0x8c073d9eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073f84u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073da0; /* mov.l 0x8c073f84,r2 */
L8c073da0: *stop=0x8c073da0u;r[4]=r[14];goto L8c073da2; /* mov r14,r4 */
L8c073da2: target=r[2];*pr=0x8c073da6u;*stop=0x8c073da4u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073da6; /* jsr @r2 */
L8c073da6: *stop=0x8c073da6u;r[4]=2u;goto L8c073da8; /* mov #0x2,r4 */
L8c073da8: *stop=0x8c073da8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073f88u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073daa; /* mov.l 0x8c073f88,r3 */
L8c073daa: *stop=0x8c073daau;r[4]<<=8;goto L8c073dac; /* shll8 r4 */
L8c073dac: *stop=0x8c073dacu;r[0]=28u;goto L8c073dae; /* mov #0x1c,r0 */
L8c073dae: *stop=0x8c073daeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[0]+r[15]);if(io_status)return io_status;goto L8c073db0; /* fmov fr0,@(r0,r15) */
L8c073db0: *stop=0x8c073db0u;r[4]+=12u;goto L8c073db2; /* add #0xc,r4 */
L8c073db2: target=r[3];*pr=0x8c073db6u;*stop=0x8c073db4u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073db6; /* jsr @r3 */
L8c073db6: *stop=0x8c073db6u;r[0]=28u;goto L8c073db8; /* mov #0x1c,r0 */
L8c073db8: *stop=0x8c073db8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073dba; /* fmov.s @(r0,r15),fr3 */
L8c073dba: *stop=0x8c073dbau;r[0]=2u;goto L8c073dbc; /* mov #0x2,r0 */
L8c073dbc: *stop=0x8c073dbcu;r[0]<<=8;goto L8c073dbe; /* shll8 r0 */
L8c073dbe: *stop=0x8c073dbeu;r[0]+=12u;goto L8c073dc0; /* add #0xc,r0 */
L8c073dc0: *stop=0x8c073dc0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073dc2; /* fmov.s @(r0,r15),fr2 */
L8c073dc2: *stop=0x8c073dc2u;r[0]=108u;goto L8c073dc4; /* mov #0x6c,r0 */
L8c073dc4: *stop=0x8c073dc4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c073dc6; /* fmul fr3,fr2 */
L8c073dc6: *stop=0x8c073dc6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073dc8; /* fmov fr2,@(r0,r14) */
L8c073dc8: *stop=0x8c073dc8u;r[0]=28u;goto L8c073dca; /* mov #0x1c,r0 */
L8c073dca: *stop=0x8c073dcau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073dcc; /* fmov.s @(r0,r15),fr3 */
L8c073dcc: *stop=0x8c073dccu;r[0]=2u;goto L8c073dce; /* mov #0x2,r0 */
L8c073dce: *stop=0x8c073dceu;r[0]<<=8;goto L8c073dd0; /* shll8 r0 */
L8c073dd0: *stop=0x8c073dd0u;r[0]+=16u;goto L8c073dd2; /* add #0x10,r0 */
L8c073dd2: *stop=0x8c073dd2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073dd4; /* fmov.s @(r0,r15),fr2 */
L8c073dd4: *stop=0x8c073dd4u;r[0]=112u;goto L8c073dd6; /* mov #0x70,r0 */
L8c073dd6: *stop=0x8c073dd6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c073dd8; /* fmul fr3,fr2 */
L8c073dd8: *stop=0x8c073dd8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073dda; /* fmov fr2,@(r0,r14) */
L8c073dda: *stop=0x8c073ddau;r[0]=28u;goto L8c073ddc; /* mov #0x1c,r0 */
L8c073ddc: *stop=0x8c073ddcu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073dde; /* fmov.s @(r0,r15),fr3 */
L8c073dde: *stop=0x8c073ddeu;r[0]=2u;goto L8c073de0; /* mov #0x2,r0 */
L8c073de0: *stop=0x8c073de0u;r[0]<<=8;goto L8c073de2; /* shll8 r0 */
L8c073de2: *stop=0x8c073de2u;r[0]+=20u;goto L8c073de4; /* add #0x14,r0 */
L8c073de4: *stop=0x8c073de4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073de6; /* fmov.s @(r0,r15),fr2 */
L8c073de6: *stop=0x8c073de6u;r[0]=116u;goto L8c073de8; /* mov #0x74,r0 */
L8c073de8: *stop=0x8c073de8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c073dea; /* fmul fr3,fr2 */
L8c073dea: *stop=0x8c073deau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073dec; /* fmov fr2,@(r0,r14) */
L8c073dec: *stop=0x8c073decu;*sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c073dee; /* tst r9,r9 */
L8c073dee: if((*sr&1u))goto L8c073ea0;goto L8c073df0; /* bt 0x8c073ea0 */
L8c073df0: *stop=0x8c073df0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073f84u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073df2; /* mov.l 0x8c073f84,r2 */
L8c073df2: *stop=0x8c073df2u;r[4]=r[14];goto L8c073df4; /* mov r14,r4 */
L8c073df4: target=r[2];*pr=0x8c073df8u;*stop=0x8c073df6u;r[4]+=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073df8; /* jsr @r2 */
L8c073df8: *stop=0x8c073df8u;r[0]=12u;goto L8c073dfa; /* mov #0xc,r0 */
L8c073dfa: *stop=0x8c073dfau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[0]+r[15]);if(io_status)return io_status;goto L8c073dfc; /* fmov fr0,@(r0,r15) */
L8c073dfc: *stop=0x8c073dfcu;r[0]=0x8c073f8cu;goto L8c073dfe; /* mova 0x8c073f8c,r0 */
L8c073dfe: *stop=0x8c073dfeu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c073e00; /* fmov.s @r0,fr3 */
L8c073e00: *stop=0x8c073e00u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c073e02; /* fcmp/gt fr3,fr0 */
L8c073e02: if(!(*sr&1u))goto L8c073ea0;goto L8c073e04; /* bf 0x8c073ea0 */
L8c073e04: *stop=0x8c073e04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073f88u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073e06; /* mov.l 0x8c073f88,r2 */
L8c073e06: *stop=0x8c073e06u;r[4]=r[14];goto L8c073e08; /* mov r14,r4 */
L8c073e08: target=r[2];*pr=0x8c073e0cu;*stop=0x8c073e0au;r[4]+=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073e0c; /* jsr @r2 */
L8c073e0c: *stop=0x8c073e0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073f84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073e0e; /* mov.l 0x8c073f84,r3 */
L8c073e0e: *stop=0x8c073e0eu;r[4]=r[14];goto L8c073e10; /* mov r14,r4 */
L8c073e10: target=r[3];*pr=0x8c073e14u;*stop=0x8c073e12u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073e14; /* jsr @r3 */
L8c073e14: *stop=0x8c073e14u;r[0]=12u;goto L8c073e16; /* mov #0xc,r0 */
L8c073e16: *stop=0x8c073e16u;fcopy(f,4u,0u);goto L8c073e18; /* fmov fr0,fr4 */
L8c073e18: *stop=0x8c073e18u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[0]+r[15]);if(io_status)return io_status;goto L8c073e1a; /* fmov fr0,@(r0,r15) */
L8c073e1a: *stop=0x8c073e1au;r[0]=0x8c073f90u;goto L8c073e1c; /* mova 0x8c073f90,r0 */
L8c073e1c: *stop=0x8c073e1cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c073e1e; /* fmov.s @r0,fr3 */
L8c073e1e: *stop=0x8c073e1eu;r[0]=108u;goto L8c073e20; /* mov #0x6c,r0 */
L8c073e20: *stop=0x8c073e20u;r[1]=r[15];goto L8c073e22; /* mov r15,r1 */
L8c073e22: *stop=0x8c073e22u;r[1]+=12u;goto L8c073e24; /* add #0xc,r1 */
L8c073e24: *stop=0x8c073e24u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c073e26; /* fadd fr3,fr4 */
L8c073e26: *stop=0x8c073e26u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e28; /* fmov.s @(r0,r14),fr3 */
L8c073e28: *stop=0x8c073e28u;r[0]=16u;goto L8c073e2a; /* mov #0x10,r0 */
L8c073e2a: *stop=0x8c073e2au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c073e2c; /* fmul fr4,fr3 */
L8c073e2c: *stop=0x8c073e2cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e2e; /* fmov fr3,@(r0,r14) */
L8c073e2e: *stop=0x8c073e2eu;r[0]=112u;goto L8c073e30; /* mov #0x70,r0 */
L8c073e30: *stop=0x8c073e30u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e32; /* fmov.s @(r0,r14),fr3 */
L8c073e32: *stop=0x8c073e32u;r[0]=20u;goto L8c073e34; /* mov #0x14,r0 */
L8c073e34: *stop=0x8c073e34u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c073e36; /* fmul fr4,fr3 */
L8c073e36: *stop=0x8c073e36u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e38; /* fmov fr3,@(r0,r14) */
L8c073e38: *stop=0x8c073e38u;r[0]=116u;goto L8c073e3a; /* mov #0x74,r0 */
L8c073e3a: *stop=0x8c073e3au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e3c; /* fmov.s @(r0,r14),fr3 */
L8c073e3c: *stop=0x8c073e3cu;r[0]=24u;goto L8c073e3e; /* mov #0x18,r0 */
L8c073e3e: *stop=0x8c073e3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c073e40; /* fmul fr4,fr3 */
L8c073e40: *stop=0x8c073e40u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e42; /* fmov fr3,@(r0,r14) */
L8c073e42: *stop=0x8c073e42u;r[0]=0x8c073f8cu;goto L8c073e44; /* mova 0x8c073f8c,r0 */
L8c073e44: *stop=0x8c073e44u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c073e46; /* fmov.s @r0,fr3 */
L8c073e46: *stop=0x8c073e46u;r[0]=12u;goto L8c073e48; /* mov #0xc,r0 */
L8c073e48: *stop=0x8c073e48u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073e4a; /* fmov fr3,@(r0,r15) */
L8c073e4a: *stop=0x8c073e4au;r[0]=108u;goto L8c073e4c; /* mov #0x6c,r0 */
L8c073e4c: *stop=0x8c073e4cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e4e; /* fmov.s @(r0,r14),fr2 */
L8c073e4e: *stop=0x8c073e4eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c073e50; /* fmul fr3,fr2 */
L8c073e50: *stop=0x8c073e50u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e52; /* fmov fr2,@(r0,r14) */
L8c073e52: *stop=0x8c073e52u;r[0]=112u;goto L8c073e54; /* mov #0x70,r0 */
L8c073e54: *stop=0x8c073e54u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e56; /* fmov.s @(r0,r14),fr2 */
L8c073e56: *stop=0x8c073e56u;r[2]=r[15];goto L8c073e58; /* mov r15,r2 */
L8c073e58: *stop=0x8c073e58u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c073e5a; /* fmov.s @r1,fr3 */
L8c073e5a: *stop=0x8c073e5au;r[1]=r[15];goto L8c073e5c; /* mov r15,r1 */
L8c073e5c: *stop=0x8c073e5cu;r[1]+=12u;goto L8c073e5e; /* add #0xc,r1 */
L8c073e5e: *stop=0x8c073e5eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c073e60; /* fmul fr3,fr2 */
L8c073e60: *stop=0x8c073e60u;r[2]+=80u;goto L8c073e62; /* add #0x50,r2 */
L8c073e62: *stop=0x8c073e62u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e64; /* fmov fr2,@(r0,r14) */
L8c073e64: *stop=0x8c073e64u;r[0]=116u;goto L8c073e66; /* mov #0x74,r0 */
L8c073e66: *stop=0x8c073e66u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e68; /* fmov.s @(r0,r14),fr2 */
L8c073e68: *stop=0x8c073e68u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c073e6a; /* fmov.s @r1,fr3 */
L8c073e6a: *stop=0x8c073e6au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c073e6c; /* fmul fr3,fr2 */
L8c073e6c: *stop=0x8c073e6cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073e6e; /* fmov fr2,@(r0,r14) */
L8c073e6e: *stop=0x8c073e6eu;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073e70; /* mov.l @(0x20,r15),r3 */
L8c073e70: *stop=0x8c073e70u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c073e72; /* mov.l @r3,r1 */
L8c073e72: *stop=0x8c073e72u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c073e74; /* mov.l @(0x4,r3),r0 */
L8c073e74: *stop=0x8c073e74u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c073e76; /* mov.l r1,@r2 */
L8c073e76: *stop=0x8c073e76u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c073e78; /* mov.l r0,@(0x4,r2) */
L8c073e78: *stop=0x8c073e78u;r[0]=r[15];goto L8c073e7a; /* mov r15,r0 */
L8c073e7a: *stop=0x8c073e7au;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073e7c; /* mov.l @(0x8,r3),r1 */
L8c073e7c: *stop=0x8c073e7cu;r[0]+=80u;goto L8c073e7e; /* add #0x50,r0 */
L8c073e7e: *stop=0x8c073e7eu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c073e80; /* mov.l r1,@(0x8,r2) */
L8c073e80: *stop=0x8c073e80u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c073e82; /* mov.l @r10,r3 */
L8c073e82: *stop=0x8c073e82u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073e84; /* mov.l @(0x20,r15),r2 */
L8c073e84: *stop=0x8c073e84u;if(!valid_access(r[10]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073e86; /* mov.l @(0x4,r10),r1 */
L8c073e86: *stop=0x8c073e86u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c073e88; /* mov.l r3,@r2 */
L8c073e88: *stop=0x8c073e88u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c073e8a; /* mov.l r1,@(0x4,r2) */
L8c073e8a: *stop=0x8c073e8au;if(!valid_access(r[10]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073e8c; /* mov.l @(0x8,r10),r3 */
L8c073e8c: *stop=0x8c073e8cu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c073e8e; /* mov.l r3,@(0x8,r2) */
L8c073e8e: *stop=0x8c073e8eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c073e90; /* mov.l @r0,r3 */
L8c073e90: *stop=0x8c073e90u;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073e92; /* mov.l @(0x4,r0),r2 */
L8c073e92: *stop=0x8c073e92u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c073e94; /* mov.l r3,@r10 */
L8c073e94: *stop=0x8c073e94u;r[1]=4294967295u;goto L8c073e96; /* mov #-0x1,r1 */
L8c073e96: *stop=0x8c073e96u;if(!valid_access(r[10]+4u,4u))return -3;wr(m,bus,&io_status,r[10]+0x4u,r[2],4);if(io_status)return io_status;goto L8c073e98; /* mov.l r2,@(0x4,r10) */
L8c073e98: *stop=0x8c073e98u;if(!valid_access(r[0]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073e9a; /* mov.l @(0x8,r0),r3 */
L8c073e9a: *stop=0x8c073e9au;if(!valid_access(r[10]+8u,4u))return -3;wr(m,bus,&io_status,r[10]+0x8u,r[3],4);if(io_status)return io_status;goto L8c073e9c; /* mov.l r3,@(0x8,r10) */
L8c073e9c: *stop=0x8c073e9eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c073eac; /* bra 0x8c073eac */
L8c073ea0: *stop=0x8c073ea0u;r[0]=24u;goto L8c073ea2; /* mov #0x18,r0 */
L8c073ea2: *stop=0x8c073ea2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c073ea4; /* fmov fr15,@(r0,r14) */
L8c073ea4: *stop=0x8c073ea4u;r[0]=20u;goto L8c073ea6; /* mov #0x14,r0 */
L8c073ea6: *stop=0x8c073ea6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c073ea8; /* fmov fr15,@(r0,r14) */
L8c073ea8: *stop=0x8c073ea8u;r[0]=16u;goto L8c073eaa; /* mov #0x10,r0 */
L8c073eaa: *stop=0x8c073eaau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c073eac; /* fmov fr15,@(r0,r14) */
L8c073eac: *stop=0x8c073eacu;r[0]=108u;goto L8c073eae; /* mov #0x6c,r0 */
L8c073eae: *stop=0x8c073eaeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073f94u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c073eb0; /* mov.l 0x8c073f94,r1 */
L8c073eb0: *stop=0x8c073eb0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073eb2; /* fmov.s @(r0,r14),fr3 */
L8c073eb2: *stop=0x8c073eb2u;r[0]=96u;goto L8c073eb4; /* mov #0x60,r0 */
L8c073eb4: *stop=0x8c073eb4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073eb6; /* fmov.s @(r0,r14),fr2 */
L8c073eb6: *stop=0x8c073eb6u;r[0]=4u;goto L8c073eb8; /* mov #0x4,r0 */
L8c073eb8: *stop=0x8c073eb8u;r[3]=88u;goto L8c073eba; /* mov #0x58,r3 */
L8c073eba: *stop=0x8c073ebau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073ebc; /* fadd fr3,fr2 */
L8c073ebc: *stop=0x8c073ebcu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073ebe; /* fmov fr2,@(r0,r14) */
L8c073ebe: *stop=0x8c073ebeu;r[0]=112u;goto L8c073ec0; /* mov #0x70,r0 */
L8c073ec0: *stop=0x8c073ec0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073ec2; /* fmov.s @(r0,r14),fr3 */
L8c073ec2: *stop=0x8c073ec2u;r[0]=100u;goto L8c073ec4; /* mov #0x64,r0 */
L8c073ec4: *stop=0x8c073ec4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073ec6; /* fmov.s @(r0,r14),fr2 */
L8c073ec6: *stop=0x8c073ec6u;r[0]=8u;goto L8c073ec8; /* mov #0x8,r0 */
L8c073ec8: *stop=0x8c073ec8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073eca; /* fadd fr3,fr2 */
L8c073eca: *stop=0x8c073ecau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073ecc; /* fmov fr2,@(r0,r14) */
L8c073ecc: *stop=0x8c073eccu;r[0]=116u;goto L8c073ece; /* mov #0x74,r0 */
L8c073ece: *stop=0x8c073eceu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073ed0; /* fmov.s @(r0,r14),fr3 */
L8c073ed0: *stop=0x8c073ed0u;r[0]=104u;goto L8c073ed2; /* mov #0x68,r0 */
L8c073ed2: *stop=0x8c073ed2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073ed4; /* fmov.s @(r0,r14),fr2 */
L8c073ed4: *stop=0x8c073ed4u;r[0]=12u;goto L8c073ed6; /* mov #0xc,r0 */
L8c073ed6: *stop=0x8c073ed6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073ed8; /* fadd fr3,fr2 */
L8c073ed8: *stop=0x8c073ed8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073eda; /* fmov fr2,@(r0,r14) */
L8c073eda: *stop=0x8c073edau;r[0]=42u;goto L8c073edc; /* mov #0x2a,r0 */
L8c073edc: *stop=0x8c073edcu;if(!valid_access(r[0]+r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c073ede; /* mov.w @(r0,r14),r2 */
L8c073ede: *stop=0x8c073edeu;r[0]=28u;goto L8c073ee0; /* mov #0x1c,r0 */
L8c073ee0: *stop=0x8c073ee0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073ee2; /* fmov.s @(r0,r14),fr3 */
L8c073ee2: *stop=0x8c073ee2u;r[0]=116u;goto L8c073ee4; /* mov #0x74,r0 */
L8c073ee4: *stop=0x8c073ee4u;*macl=(r[3]&65535u)*(r[2]&65535u);goto L8c073ee6; /* mulu.w r3,r2 */
L8c073ee6: *stop=0x8c073ee6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073ee8; /* fmov fr3,@(r0,r15) */
L8c073ee8: *stop=0x8c073ee8u;r[4]=*macl;goto L8c073eea; /* sts MACL,r4 */
L8c073eea: *stop=0x8c073eeau;r[4]+=r[1];goto L8c073eec; /* add r1,r4 */
L8c073eec: *stop=0x8c073eecu;r[0]=32u;goto L8c073eee; /* mov #0x20,r0 */
L8c073eee: *stop=0x8c073eeeu;r[6]=r[15];goto L8c073ef0; /* mov r15,r6 */
L8c073ef0: *stop=0x8c073ef0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073ef2; /* fmov.s @(r0,r14),fr3 */
L8c073ef2: *stop=0x8c073ef2u;r[0]=120u;goto L8c073ef4; /* mov #0x78,r0 */
L8c073ef4: *stop=0x8c073ef4u;r[5]=r[15];goto L8c073ef6; /* mov r15,r5 */
L8c073ef6: *stop=0x8c073ef6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c073f98u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073ef8; /* mov.l 0x8c073f98,r2 */
L8c073ef8: *stop=0x8c073ef8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073efa; /* fmov fr3,@(r0,r15) */
L8c073efa: *stop=0x8c073efau;r[0]=36u;goto L8c073efc; /* mov #0x24,r0 */
L8c073efc: *stop=0x8c073efcu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c073efe; /* fmov.s @(r0,r14),fr3 */
L8c073efe: *stop=0x8c073efeu;r[0]=124u;goto L8c073f00; /* mov #0x7c,r0 */
L8c073f00: *stop=0x8c073f00u;r[5]+=104u;goto L8c073f02; /* add #0x68,r5 */
L8c073f02: *stop=0x8c073f02u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073f04; /* fmov fr3,@(r0,r15) */
L8c073f04: *stop=0x8c073f04u;r[0]=32u;goto L8c073f06; /* mov #0x20,r0 */
L8c073f06: *stop=0x8c073f06u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c073f08; /* fmov.s @(r0,r4),fr3 */
L8c073f08: *stop=0x8c073f08u;r[0]=104u;goto L8c073f0a; /* mov #0x68,r0 */
L8c073f0a: *stop=0x8c073f0au;r[6]+=92u;goto L8c073f0c; /* add #0x5c,r6 */
L8c073f0c: *stop=0x8c073f0cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073f0e; /* fmov fr3,@(r0,r15) */
L8c073f0e: *stop=0x8c073f0eu;r[0]=36u;goto L8c073f10; /* mov #0x24,r0 */
L8c073f10: *stop=0x8c073f10u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c073f12; /* fmov.s @(r0,r4),fr3 */
L8c073f12: *stop=0x8c073f12u;r[0]=108u;goto L8c073f14; /* mov #0x6c,r0 */
L8c073f14: *stop=0x8c073f14u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073f16; /* fmov fr3,@(r0,r15) */
L8c073f16: *stop=0x8c073f16u;r[0]=40u;goto L8c073f18; /* mov #0x28,r0 */
L8c073f18: *stop=0x8c073f18u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c073f1a; /* fmov.s @(r0,r4),fr3 */
L8c073f1a: *stop=0x8c073f1au;r[0]=112u;goto L8c073f1c; /* mov #0x70,r0 */
L8c073f1c: *stop=0x8c073f1cu;r[4]=r[14];goto L8c073f1e; /* mov r14,r4 */
L8c073f1e: *stop=0x8c073f1eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073f20; /* fmov fr3,@(r0,r15) */
L8c073f20: target=r[2];*pr=0x8c073f24u;*stop=0x8c073f22u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073f24; /* jsr @r2 */
L8c073f24: *stop=0x8c073f24u;r[0]=92u;goto L8c073f26; /* mov #0x5c,r0 */
L8c073f26: *stop=0x8c073f26u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073f28; /* fmov.s @(r0,r15),fr3 */
L8c073f28: *stop=0x8c073f28u;r[0]=2u;goto L8c073f2a; /* mov #0x2,r0 */
L8c073f2a: *stop=0x8c073f2au;r[0]<<=8;goto L8c073f2c; /* shll8 r0 */
L8c073f2c: *stop=0x8c073f2cu;r[0]+=36u;goto L8c073f2e; /* add #0x24,r0 */
L8c073f2e: *stop=0x8c073f2eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073f30; /* fmov.s @(r0,r15),fr2 */
L8c073f30: *stop=0x8c073f30u;r[0]=4u;goto L8c073f32; /* mov #0x4,r0 */
L8c073f32: *stop=0x8c073f32u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073f34; /* fadd fr3,fr2 */
L8c073f34: *stop=0x8c073f34u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073f36; /* fmov fr2,@(r0,r14) */
L8c073f36: *stop=0x8c073f36u;r[0]=96u;goto L8c073f38; /* mov #0x60,r0 */
L8c073f38: *stop=0x8c073f38u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073f3a; /* fmov.s @(r0,r15),fr3 */
L8c073f3a: *stop=0x8c073f3au;r[0]=2u;goto L8c073f3c; /* mov #0x2,r0 */
L8c073f3c: *stop=0x8c073f3cu;r[0]<<=8;goto L8c073f3e; /* shll8 r0 */
L8c073f3e: *stop=0x8c073f3eu;r[0]+=40u;goto L8c073f40; /* add #0x28,r0 */
L8c073f40: *stop=0x8c073f40u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073f42; /* fmov.s @(r0,r15),fr2 */
L8c073f42: *stop=0x8c073f42u;r[0]=8u;goto L8c073f44; /* mov #0x8,r0 */
L8c073f44: *stop=0x8c073f44u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073f46; /* fadd fr3,fr2 */
L8c073f46: *stop=0x8c073f46u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073f48; /* fmov fr2,@(r0,r14) */
L8c073f48: *stop=0x8c073f48u;r[0]=100u;goto L8c073f4a; /* mov #0x64,r0 */
L8c073f4a: *stop=0x8c073f4au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073f4c; /* fmov.s @(r0,r15),fr3 */
L8c073f4c: *stop=0x8c073f4cu;r[0]=2u;goto L8c073f4e; /* mov #0x2,r0 */
L8c073f4e: *stop=0x8c073f4eu;r[0]<<=8;goto L8c073f50; /* shll8 r0 */
L8c073f50: *stop=0x8c073f50u;r[0]+=44u;goto L8c073f52; /* add #0x2c,r0 */
L8c073f52: *stop=0x8c073f52u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073f54; /* fmov.s @(r0,r15),fr2 */
L8c073f54: *stop=0x8c073f54u;r[0]=12u;goto L8c073f56; /* mov #0xc,r0 */
L8c073f56: *stop=0x8c073f56u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c073f58; /* fadd fr3,fr2 */
L8c073f58: *stop=0x8c073f58u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073f5a; /* fmov fr2,@(r0,r14) */
L8c073f5a: *stop=0x8c073f5au;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073f5c; /* mov.l @(0x10,r15),r3 */
L8c073f5c: *stop=0x8c073f5cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c073f5e; /* tst r3,r3 */
L8c073f5e: if((*sr&1u))goto L8c073fa0;goto L8c073f60; /* bt 0x8c073fa0 */
L8c073f60: *stop=0x8c073f60u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073f62; /* mov.l @(0x10,r15),r3 */
L8c073f62: *stop=0x8c073f62u;r[7]=2u;goto L8c073f64; /* mov #0x2,r7 */
L8c073f64: *stop=0x8c073f64u;r[0]=108u;goto L8c073f66; /* mov #0x6c,r0 */
L8c073f66: *stop=0x8c073f66u;r[5]=r[12];goto L8c073f68; /* mov r12,r5 */
L8c073f68: *stop=0x8c073f68u;r[7]<<=8;goto L8c073f6a; /* shll8 r7 */
L8c073f6a: *stop=0x8c073f6au;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[3],4);if(io_status)return io_status;goto L8c073f6c; /* mov.l r3,@(r0,r12) */
L8c073f6c: *stop=0x8c073f6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c073f9cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073f6e; /* mov.l 0x8c073f9c,r3 */
L8c073f6e: *stop=0x8c073f6eu;r[7]+=36u;goto L8c073f70; /* add #0x24,r7 */
L8c073f70: *stop=0x8c073f70u;r[7]+=r[15];goto L8c073f72; /* add r15,r7 */
L8c073f72: *stop=0x8c073f72u;r[6]=r[14];goto L8c073f74; /* mov r14,r6 */
L8c073f74: target=r[3];*pr=0x8c073f78u;*stop=0x8c073f76u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073f78; /* jsr @r3 */
L8c073f78: *stop=0x8c073f7au;goto L8c073fa4; /* bra 0x8c073fa4 */
L8c073fa0: *stop=0x8c073fa0u;r[0]=108u;goto L8c073fa2; /* mov #0x6c,r0 */
L8c073fa2: *stop=0x8c073fa2u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[9],4);if(io_status)return io_status;goto L8c073fa4; /* mov.l r9,@(r0,r12) */
L8c073fa4: *stop=0x8c073fa4u;r[0]=2u;goto L8c073fa6; /* mov #0x2,r0 */
L8c073fa6: *stop=0x8c073fa6u;r[0]<<=8;goto L8c073fa8; /* shll8 r0 */
L8c073fa8: *stop=0x8c073fa8u;r[0]+=36u;goto L8c073faa; /* add #0x24,r0 */
L8c073faa: *stop=0x8c073faau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073fac; /* fmov.s @(r0,r15),fr3 */
L8c073fac: *stop=0x8c073facu;r[0]=4u;goto L8c073fae; /* mov #0x4,r0 */
L8c073fae: *stop=0x8c073faeu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073fb0; /* fmov.s @(r0,r14),fr2 */
L8c073fb0: *stop=0x8c073fb0u;r[0]=2u;goto L8c073fb2; /* mov #0x2,r0 */
L8c073fb2: *stop=0x8c073fb2u;r[0]<<=8;goto L8c073fb4; /* shll8 r0 */
L8c073fb4: *stop=0x8c073fb4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c073fb6; /* fsub fr3,fr2 */
L8c073fb6: *stop=0x8c073fb6u;r[0]+=12u;goto L8c073fb8; /* add #0xc,r0 */
L8c073fb8: *stop=0x8c073fb8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073fba; /* fmov fr2,@(r0,r15) */
L8c073fba: *stop=0x8c073fbau;r[0]=2u;goto L8c073fbc; /* mov #0x2,r0 */
L8c073fbc: *stop=0x8c073fbcu;r[0]<<=8;goto L8c073fbe; /* shll8 r0 */
L8c073fbe: *stop=0x8c073fbeu;r[0]+=40u;goto L8c073fc0; /* add #0x28,r0 */
L8c073fc0: *stop=0x8c073fc0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073fc2; /* fmov.s @(r0,r15),fr3 */
L8c073fc2: *stop=0x8c073fc2u;r[0]=8u;goto L8c073fc4; /* mov #0x8,r0 */
L8c073fc4: *stop=0x8c073fc4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073fc6; /* fmov.s @(r0,r14),fr2 */
L8c073fc6: *stop=0x8c073fc6u;r[0]=2u;goto L8c073fc8; /* mov #0x2,r0 */
L8c073fc8: *stop=0x8c073fc8u;r[0]<<=8;goto L8c073fca; /* shll8 r0 */
L8c073fca: *stop=0x8c073fcau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c073fcc; /* fsub fr3,fr2 */
L8c073fcc: *stop=0x8c073fccu;r[0]+=16u;goto L8c073fce; /* add #0x10,r0 */
L8c073fce: *stop=0x8c073fceu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073fd0; /* fmov fr2,@(r0,r15) */
L8c073fd0: *stop=0x8c073fd0u;r[0]=2u;goto L8c073fd2; /* mov #0x2,r0 */
L8c073fd2: *stop=0x8c073fd2u;r[0]<<=8;goto L8c073fd4; /* shll8 r0 */
L8c073fd4: *stop=0x8c073fd4u;r[0]+=44u;goto L8c073fd6; /* add #0x2c,r0 */
L8c073fd6: *stop=0x8c073fd6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c073fd8; /* fmov.s @(r0,r15),fr3 */
L8c073fd8: *stop=0x8c073fd8u;r[0]=12u;goto L8c073fda; /* mov #0xc,r0 */
L8c073fda: *stop=0x8c073fdau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c073fdc; /* fmov.s @(r0,r14),fr2 */
L8c073fdc: *stop=0x8c073fdcu;r[0]=2u;goto L8c073fde; /* mov #0x2,r0 */
L8c073fde: *stop=0x8c073fdeu;r[0]<<=8;goto L8c073fe0; /* shll8 r0 */
L8c073fe0: *stop=0x8c073fe0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c073fe2; /* fsub fr3,fr2 */
L8c073fe2: *stop=0x8c073fe2u;r[0]+=20u;goto L8c073fe4; /* add #0x14,r0 */
L8c073fe4: *stop=0x8c073fe4u;r[5]=2u;goto L8c073fe6; /* mov #0x2,r5 */
L8c073fe6: *stop=0x8c073fe6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074120u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c073fe8; /* mov.l 0x8c074120,r3 */
L8c073fe8: *stop=0x8c073fe8u;r[5]<<=8;goto L8c073fea; /* shll8 r5 */
L8c073fea: *stop=0x8c073feau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c073fec; /* fmov fr2,@(r0,r15) */
L8c073fec: *stop=0x8c073fecu;r[5]+=12u;goto L8c073fee; /* add #0xc,r5 */
L8c073fee: *stop=0x8c073feeu;r[4]=r[13];goto L8c073ff0; /* mov r13,r4 */
L8c073ff0: *stop=0x8c073ff0u;r[5]+=r[15];goto L8c073ff2; /* add r15,r5 */
L8c073ff2: target=r[3];*pr=0x8c073ff6u;*stop=0x8c073ff4u;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c073ff6; /* jsr @r3 */
L8c073ff6: *stop=0x8c073ff6u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c073ff8; /* mov.l @(0x10,r15),r2 */
L8c073ff8: *stop=0x8c073ff8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c073ffa; /* tst r2,r2 */
L8c073ffa: take=!(*sr&1u);*stop=0x8c073ffcu;fcopy(f,4u,0u);if(take)goto L8c07401e;goto L8c073ffe; /* bf/s 0x8c07401e */
L8c073ffe: *stop=0x8c073ffeu;r[0]=56u;goto L8c074000; /* mov #0x38,r0 */
L8c074000: *stop=0x8c074000u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074002; /* fldi0 fr3 */
L8c074002: *stop=0x8c074002u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c074004; /* fmov.s @(r0,r12),fr2 */
L8c074004: *stop=0x8c074004u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c074006; /* fcmp/eq fr3,fr2 */
L8c074006: if(!(*sr&1u))goto L8c07401e;goto L8c074008; /* bf 0x8c07401e */
L8c074008: *stop=0x8c074008u;r[0]=64u;goto L8c07400a; /* mov #0x40,r0 */
L8c07400a: *stop=0x8c07400au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c07400c; /* fmov.s @(r0,r12),fr2 */
L8c07400c: *stop=0x8c07400cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c07400e; /* fcmp/eq fr3,fr2 */
L8c07400e: if(!(*sr&1u))goto L8c07401e;goto L8c074010; /* bf 0x8c07401e */
L8c074010: *stop=0x8c074010u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c074012; /* fldi1 fr3 */
L8c074012: *stop=0x8c074012u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074014; /* fcmp/gt fr3,fr4 */
L8c074014: if((*sr&1u))goto L8c07401e;goto L8c074016; /* bt 0x8c07401e */
L8c074016: *stop=0x8c074016u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074018; /* mov.l 0x8c074124,r3 */
L8c074018: *stop=0x8c074018u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c07401a; /* mov.l @r3,r2 */
L8c07401a: *stop=0x8c07401au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c07401c; /* tst r2,r2 */
L8c07401c: if((*sr&1u))goto L8c074042;goto L8c07401e; /* bt 0x8c074042 */
L8c07401e: *stop=0x8c07401eu;r[0]=0x8c074128u;goto L8c074020; /* mova 0x8c074128,r0 */
L8c074020: *stop=0x8c074020u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c074022; /* fmov.s @r0,fr3 */
L8c074022: *stop=0x8c074022u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074024; /* fcmp/gt fr3,fr4 */
L8c074024: if((*sr&1u))goto L8c07402e;goto L8c074026; /* bt 0x8c07402e */
L8c074026: *stop=0x8c074026u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074028; /* mov.l 0x8c074124,r3 */
L8c074028: *stop=0x8c074028u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c07402a; /* mov.l @r3,r2 */
L8c07402a: *stop=0x8c07402au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c07402c; /* tst r2,r2 */
L8c07402c: if((*sr&1u))goto L8c074042;goto L8c07402e; /* bt 0x8c074042 */
L8c07402e: *stop=0x8c07402eu;r[2]=2u;goto L8c074030; /* mov #0x2,r2 */
L8c074030: *stop=0x8c074030u;r[2]<<=8;goto L8c074032; /* shll8 r2 */
L8c074032: *stop=0x8c074032u;r[2]+=12u;goto L8c074034; /* add #0xc,r2 */
L8c074034: *stop=0x8c074034u;r[2]+=r[15];goto L8c074036; /* add r15,r2 */
L8c074036: *stop=0x8c074036u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c074038; /* mov.l @r2,r3 */
L8c074038: *stop=0x8c074038u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07403a; /* mov.l @(0x4,r2),r1 */
L8c07403a: *stop=0x8c07403au;if(!valid_access(r[13]+32u,4u))return -3;wr(m,bus,&io_status,r[13]+0x20u,r[3],4);if(io_status)return io_status;goto L8c07403c; /* mov.l r3,@(0x20,r13) */
L8c07403c: *stop=0x8c07403cu;if(!valid_access(r[13]+36u,4u))return -3;wr(m,bus,&io_status,r[13]+0x24u,r[1],4);if(io_status)return io_status;goto L8c07403e; /* mov.l r1,@(0x24,r13) */
L8c07403e: *stop=0x8c07403eu;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074040; /* mov.l @(0x8,r2),r3 */
L8c074040: *stop=0x8c074040u;if(!valid_access(r[13]+40u,4u))return -3;wr(m,bus,&io_status,r[13]+0x28u,r[3],4);if(io_status)return io_status;goto L8c074042; /* mov.l r3,@(0x28,r13) */
L8c074042: *stop=0x8c074042u;r[0]=56u;goto L8c074044; /* mov #0x38,r0 */
L8c074044: *stop=0x8c074044u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c074046; /* fmov.s @(r0,r12),fr3 */
L8c074046: *stop=0x8c074046u;r[0]=60u;goto L8c074048; /* mov #0x3c,r0 */
L8c074048: *stop=0x8c074048u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07404a; /* fmov fr3,@(r0,r15) */
L8c07404a: *stop=0x8c07404au;r[0]=0x8c07412cu;goto L8c07404c; /* mova 0x8c07412c,r0 */
L8c07404c: *stop=0x8c07404cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c07404e; /* fmov.s @r0,fr2 */
L8c07404e: *stop=0x8c07404eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074050; /* fcmp/gt fr3,fr2 */
L8c074050: if(!(*sr&1u))goto L8c0740a8;goto L8c074052; /* bf 0x8c0740a8 */
L8c074052: *stop=0x8c074052u;r[0]=0x8c074130u;goto L8c074054; /* mova 0x8c074130,r0 */
L8c074054: *stop=0x8c074054u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c074056; /* fmov.s @r0,fr3 */
L8c074056: *stop=0x8c074056u;r[0]=60u;goto L8c074058; /* mov #0x3c,r0 */
L8c074058: *stop=0x8c074058u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c07405a; /* fmov.s @(r0,r15),fr1 */
L8c07405a: *stop=0x8c07405au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c07405c; /* fcmp/gt fr3,fr1 */
L8c07405c: if(!(*sr&1u))goto L8c0740a8;goto L8c07405e; /* bf 0x8c0740a8 */
L8c07405e: *stop=0x8c07405eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074138u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074060; /* mov.l 0x8c074138,r2 */
L8c074060: *stop=0x8c074060u;r[5]=r[14];goto L8c074062; /* mov r14,r5 */
L8c074062: *stop=0x8c074062u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074134u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c074064; /* mov.l 0x8c074134,r4 */
L8c074064: target=r[2];*pr=0x8c074068u;*stop=0x8c074066u;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074068; /* jsr @r2 */
L8c074068: *stop=0x8c074068u;r[0]=0x8c07413cu;goto L8c07406a; /* mova 0x8c07413c,r0 */
L8c07406a: *stop=0x8c07406au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c07406c; /* fmov.s @r0,fr3 */
L8c07406c: *stop=0x8c07406cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c07406e; /* fcmp/gt fr0,fr3 */
L8c07406e: if((*sr&1u))goto L8c0740a8;goto L8c074070; /* bt 0x8c0740a8 */
L8c074070: *stop=0x8c074070u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c074072; /* mov.l @r10,r2 */
L8c074072: *stop=0x8c074072u;r[3]=64u;goto L8c074074; /* mov #0x40,r3 */
L8c074074: *stop=0x8c074074u;r[3]<<=8;goto L8c074076; /* shll8 r3 */
L8c074076: *stop=0x8c074076u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c074078; /* tst r3,r2 */
L8c074078: if(!(*sr&1u))goto L8c0740a8;goto L8c07407a; /* bf 0x8c0740a8 */
L8c07407a: *stop=0x8c07407au;r[0]=56u;goto L8c07407c; /* mov #0x38,r0 */
L8c07407c: *stop=0x8c07407cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c07407e; /* fmov.s @(r0,r12),fr3 */
L8c07407e: *stop=0x8c07407eu;r[0]=64u;goto L8c074080; /* mov #0x40,r0 */
L8c074080: *stop=0x8c074080u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074082; /* fmov fr3,@(r0,r15) */
L8c074082: *stop=0x8c074082u;r[0]=0x8c074140u;goto L8c074084; /* mova 0x8c074140,r0 */
L8c074084: *stop=0x8c074084u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c074086; /* fmov.s @r0,fr2 */
L8c074086: *stop=0x8c074086u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074088; /* fcmp/gt fr3,fr2 */
L8c074088: if(!(*sr&1u))goto L8c074096;goto L8c07408a; /* bf 0x8c074096 */
L8c07408a: *stop=0x8c07408au;r[0]=0x8c074144u;goto L8c07408c; /* mova 0x8c074144,r0 */
L8c07408c: *stop=0x8c07408cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c07408e; /* fmov.s @r0,fr3 */
L8c07408e: *stop=0x8c07408eu;r[0]=64u;goto L8c074090; /* mov #0x40,r0 */
L8c074090: *stop=0x8c074090u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c074092; /* fmov.s @(r0,r15),fr1 */
L8c074092: *stop=0x8c074092u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c074094; /* fcmp/gt fr3,fr1 */
L8c074094: if((*sr&1u))goto L8c0740be;goto L8c074096; /* bt 0x8c0740be */
L8c074096: *stop=0x8c074096u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074138u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074098; /* mov.l 0x8c074138,r2 */
L8c074098: *stop=0x8c074098u;r[5]=r[14];goto L8c07409a; /* mov r14,r5 */
L8c07409a: *stop=0x8c07409au;{uint32_t loaded=rd(m,bus,&io_status,0x8c074134u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c07409c; /* mov.l 0x8c074134,r4 */
L8c07409c: target=r[2];*pr=0x8c0740a0u;*stop=0x8c07409eu;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0740a0; /* jsr @r2 */
L8c0740a0: *stop=0x8c0740a0u;r[0]=0x8c07413cu;goto L8c0740a2; /* mova 0x8c07413c,r0 */
L8c0740a2: *stop=0x8c0740a2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0740a4; /* fmov.s @r0,fr3 */
L8c0740a4: *stop=0x8c0740a4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0740a6; /* fcmp/gt fr0,fr3 */
L8c0740a6: if(!(*sr&1u))goto L8c0740be;goto L8c0740a8; /* bf 0x8c0740be */
L8c0740a8: *stop=0x8c0740a8u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c0740aa; /* mov.w @(0x4,r13),r0 */
L8c0740aa: *stop=0x8c0740aau;r[5]=r[11];goto L8c0740ac; /* mov r11,r5 */
L8c0740ac: *stop=0x8c0740acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074148u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0740ae; /* mov.l 0x8c074148,r3 */
L8c0740ae: *stop=0x8c0740aeu;r[6]=r[12];goto L8c0740b0; /* mov r12,r6 */
L8c0740b0: *stop=0x8c0740b0u;r[0]|=0x1u;goto L8c0740b2; /* or #0x1,r0 */
L8c0740b2: *stop=0x8c0740b2u;if(!valid_access(r[13]+4u,2u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[0],2);if(io_status)return io_status;goto L8c0740b4; /* mov.w r0,@(0x4,r13) */
L8c0740b4: *stop=0x8c0740b4u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[7]=loaded;}goto L8c0740b6; /* mov.l @r10,r7 */
L8c0740b6: target=r[3];*pr=0x8c0740bau;*stop=0x8c0740b8u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0740ba; /* jsr @r3 */
L8c0740ba: *stop=0x8c0740bcu;goto L8c07418a; /* bra 0x8c07418a */
L8c0740be: *stop=0x8c0740beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074148u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0740c0; /* mov.l 0x8c074148,r3 */
L8c0740c0: *stop=0x8c0740c0u;r[5]=r[11];goto L8c0740c2; /* mov r11,r5 */
L8c0740c2: *stop=0x8c0740c2u;r[7]=0u;goto L8c0740c4; /* mov #0x0,r7 */
L8c0740c4: *stop=0x8c0740c4u;r[6]=r[12];goto L8c0740c6; /* mov r12,r6 */
L8c0740c6: target=r[3];*pr=0x8c0740cau;*stop=0x8c0740c8u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0740ca; /* jsr @r3 */
L8c0740ca: *stop=0x8c0740cau;r[0]=56u;goto L8c0740cc; /* mov #0x38,r0 */
L8c0740cc: *stop=0x8c0740ccu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0740ce; /* fldi0 fr3 */
L8c0740ce: *stop=0x8c0740ceu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c0740d0; /* fmov.s @(r0,r12),fr2 */
L8c0740d0: *stop=0x8c0740d0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0740d2; /* fcmp/gt fr3,fr2 */
L8c0740d2: if(!(*sr&1u))goto L8c07418a;goto L8c0740d4; /* bf 0x8c07418a */
L8c0740d4: *stop=0x8c0740d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c07414cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0740d6; /* mov.l 0x8c07414c,r2 */
L8c0740d6: *stop=0x8c0740d6u;r[5]=r[11];goto L8c0740d8; /* mov r11,r5 */
L8c0740d8: *stop=0x8c0740d8u;r[6]=r[14];goto L8c0740da; /* mov r14,r6 */
L8c0740da: target=r[2];*pr=0x8c0740deu;*stop=0x8c0740dcu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0740de; /* jsr @r2 */
L8c0740de: *stop=0x8c0740e0u;goto L8c07418a; /* bra 0x8c07418a */
L8c0740e2: *stop=0x8c0740e2u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0740e4; /* mov.l @r10,r2 */
L8c0740e4: *stop=0x8c0740e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c0740e6; /* mov.l r2,@-r15 */
L8c0740e6: *stop=0x8c0740e6u;r[2]=2u;goto L8c0740e8; /* mov #0x2,r2 */
L8c0740e8: *stop=0x8c0740e8u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0740ea; /* mov.l @(0x18,r15),r3 */
L8c0740ea: *stop=0x8c0740eau;r[2]<<=8;goto L8c0740ec; /* shll8 r2 */
L8c0740ec: *stop=0x8c0740ecu;r[2]+=44u;goto L8c0740ee; /* add #0x2c,r2 */
L8c0740ee: *stop=0x8c0740eeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0740f0; /* mov.l r3,@-r15 */
L8c0740f0: *stop=0x8c0740f0u;r[3]=2u;goto L8c0740f2; /* mov #0x2,r3 */
L8c0740f2: *stop=0x8c0740f2u;r[3]<<=8;goto L8c0740f4; /* shll8 r3 */
L8c0740f4: *stop=0x8c0740f4u;r[2]+=r[15];goto L8c0740f6; /* add r15,r2 */
L8c0740f6: *stop=0x8c0740f6u;r[3]+=12u;goto L8c0740f8; /* add #0xc,r3 */
L8c0740f8: *stop=0x8c0740f8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c0740fa; /* mov.l r2,@-r15 */
L8c0740fa: *stop=0x8c0740fau;r[3]+=r[15];goto L8c0740fc; /* add r15,r3 */
L8c0740fc: *stop=0x8c0740fcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0740fe; /* mov.l r3,@-r15 */
L8c0740fe: *stop=0x8c0740feu;r[5]=r[11];goto L8c074100; /* mov r11,r5 */
L8c074100: *stop=0x8c074100u;r[6]=r[12];goto L8c074102; /* mov r12,r6 */
L8c074102: *stop=0x8c074102u;r[7]=r[14];goto L8c074104; /* mov r14,r7 */
L8c074104: target=0x8c0747d2u;*pr=0x8c074108u;*stop=0x8c074106u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074108; /* bsr 0x8c0747d2 */
L8c074108: *stop=0x8c074108u;r[15]+=16u;goto L8c07410a; /* add #0x10,r15 */
L8c07410a: *stop=0x8c07410cu;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[0],4);if(io_status)return io_status;goto L8c07418a; /* bra 0x8c07418a */
L8c074160: *stop=0x8c074160u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c074162; /* mov.l @r10,r3 */
L8c074162: *stop=0x8c074162u;r[6]=r[12];goto L8c074164; /* mov r12,r6 */
L8c074164: *stop=0x8c074164u;r[5]=r[11];goto L8c074166; /* mov r11,r5 */
L8c074166: *stop=0x8c074166u;r[7]=r[14];goto L8c074168; /* mov r14,r7 */
L8c074168: *stop=0x8c074168u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c07416a; /* mov.l r3,@-r15 */
L8c07416a: *stop=0x8c07416au;r[3]=2u;goto L8c07416c; /* mov #0x2,r3 */
L8c07416c: *stop=0x8c07416cu;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07416e; /* mov.l @(0x18,r15),r2 */
L8c07416e: *stop=0x8c07416eu;r[3]<<=8;goto L8c074170; /* shll8 r3 */
L8c074170: *stop=0x8c074170u;r[3]+=44u;goto L8c074172; /* add #0x2c,r3 */
L8c074172: *stop=0x8c074172u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c074174; /* mov.l r2,@-r15 */
L8c074174: *stop=0x8c074174u;r[2]=2u;goto L8c074176; /* mov #0x2,r2 */
L8c074176: *stop=0x8c074176u;r[2]<<=8;goto L8c074178; /* shll8 r2 */
L8c074178: *stop=0x8c074178u;r[3]+=r[15];goto L8c07417a; /* add r15,r3 */
L8c07417a: *stop=0x8c07417au;r[2]+=12u;goto L8c07417c; /* add #0xc,r2 */
L8c07417c: *stop=0x8c07417cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c07417e; /* mov.l r3,@-r15 */
L8c07417e: *stop=0x8c07417eu;r[2]+=r[15];goto L8c074180; /* add r15,r2 */
L8c074180: *stop=0x8c074180u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c074182; /* mov.l r2,@-r15 */
L8c074182: target=0x8c0747d2u;*pr=0x8c074186u;*stop=0x8c074184u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074186; /* bsr 0x8c0747d2 */
L8c074186: *stop=0x8c074186u;r[15]+=16u;goto L8c074188; /* add #0x10,r15 */
L8c074188: *stop=0x8c074188u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[0],4);if(io_status)return io_status;goto L8c07418a; /* mov.l r0,@(0x14,r15) */
L8c07418a: *stop=0x8c07418au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c07418c; /* mov.l @r15,r3 */
L8c07418c: *stop=0x8c07418cu;r[3]+=1u;goto L8c07418e; /* add #0x1,r3 */
L8c07418e: *stop=0x8c07418eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c074190; /* mov.l r3,@r15 */
L8c074190: *stop=0x8c074190u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c074192; /* mov.l @r15,r1 */
L8c074192: *stop=0x8c074192u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074194; /* mov.l @(0x8,r15),r2 */
L8c074194: *stop=0x8c074194u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[2]);goto L8c074196; /* cmp/ge r2,r1 */
L8c074196: if((*sr&1u))goto L8c07419c;goto L8c074198; /* bt 0x8c07419c */
L8c074198: *stop=0x8c07419au;goto L8c07364e; /* bra 0x8c07364e */
L8c07419c: *stop=0x8c07419cu;r[0]=76u;goto L8c07419e; /* mov #0x4c,r0 */
L8c07419e: *stop=0x8c07419eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c0741a0; /* mov.l @(r0,r15),r0 */
L8c0741a0: *stop=0x8c0741a0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0741a2; /* cmp/eq #0x1,r0 */
L8c0741a2: if((*sr&1u))goto L8c074280;goto L8c0741a4; /* bt 0x8c074280 */
L8c0741a4: *stop=0x8c0741a4u;r[0]=48u;goto L8c0741a6; /* mov #0x30,r0 */
L8c0741a6: *stop=0x8c0741a6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0741a8; /* fmov fr15,@(r0,r15) */
L8c0741a8: *stop=0x8c0741a8u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c0741aa; /* mov.w @(0x10,r12),r0 */
L8c0741aa: *stop=0x8c0741aau;*sr=(*sr&~1u)|!(r[8]&r[0]);goto L8c0741ac; /* tst r8,r0 */
L8c0741ac: take=(*sr&1u);*stop=0x8c0741aeu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;if(take)goto L8c074246;goto L8c0741b0; /* bt/s 0x8c074246 */
L8c0741b0: *stop=0x8c0741b0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0741b2; /* mov.l @(0x8,r15),r2 */
L8c0741b2: *stop=0x8c0741b2u;r[3]=64u;goto L8c0741b4; /* mov #0x40,r3 */
L8c0741b4: *stop=0x8c0741b4u;r[3]<<=8;goto L8c0741b6; /* shll8 r3 */
L8c0741b6: *stop=0x8c0741b6u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c0741b8; /* tst r3,r2 */
L8c0741b8: if(!(*sr&1u))goto L8c074246;goto L8c0741ba; /* bf 0x8c074246 */
L8c0741ba: *stop=0x8c0741bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c074380u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0741bc; /* mov.l 0x8c074380,r2 */
L8c0741bc: *stop=0x8c0741bcu;r[6]=r[15];goto L8c0741be; /* mov r15,r6 */
L8c0741be: *stop=0x8c0741beu;r[5]=0u;goto L8c0741c0; /* mov #0x0,r5 */
L8c0741c0: *stop=0x8c0741c0u;r[6]+=48u;goto L8c0741c2; /* add #0x30,r6 */
L8c0741c2: target=r[2];*pr=0x8c0741c6u;*stop=0x8c0741c4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0741c6; /* jsr @r2 */
L8c0741c6: *stop=0x8c0741c6u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0741c8; /* tst r0,r0 */
L8c0741c8: if((*sr&1u))goto L8c0741dc;goto L8c0741ca; /* bt 0x8c0741dc */
L8c0741ca: *stop=0x8c0741cau;r[0]=0x8c074384u;goto L8c0741cc; /* mova 0x8c074384,r0 */
L8c0741cc: *stop=0x8c0741ccu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0741ce; /* fmov.s @r0,fr3 */
L8c0741ce: *stop=0x8c0741ceu;r[0]=48u;goto L8c0741d0; /* mov #0x30,r0 */
L8c0741d0: *stop=0x8c0741d0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0741d2; /* fmov.s @(r0,r15),fr2 */
L8c0741d2: *stop=0x8c0741d2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0741d4; /* fcmp/gt fr3,fr2 */
L8c0741d4: if((*sr&1u))goto L8c074246;goto L8c0741d6; /* bt 0x8c074246 */
L8c0741d6: *stop=0x8c0741d6u;if(!valid_access(r[12]+10u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0xau,2);if(io_status)return io_status;goto L8c0741d8; /* mov.w @(0xa,r12),r0 */
L8c0741d8: *stop=0x8c0741d8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0741da; /* tst r0,r0 */
L8c0741da: if(!(*sr&1u))goto L8c074246;goto L8c0741dc; /* bf 0x8c074246 */
L8c0741dc: *stop=0x8c0741dcu;r[0]=60u;goto L8c0741de; /* mov #0x3c,r0 */
L8c0741de: *stop=0x8c0741deu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0741e0; /* fldi0 fr3 */
L8c0741e0: *stop=0x8c0741e0u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c0741e2; /* fmov.s @(r0,r12),fr2 */
L8c0741e2: *stop=0x8c0741e2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0741e4; /* fcmp/gt fr3,fr2 */
L8c0741e4: if((*sr&1u))goto L8c074246;goto L8c0741e6; /* bt 0x8c074246 */
L8c0741e6: *stop=0x8c0741e6u;r[0]=4294967236u;goto L8c0741e8; /* mov #-0x3c,r0 */
L8c0741e8: *stop=0x8c0741e8u;r[0]=r[0]&255u;goto L8c0741ea; /* extu.b r0,r0 */
L8c0741ea: *stop=0x8c0741eau;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c0741ec; /* fmov.s @(r0,r12),fr2 */
L8c0741ec: *stop=0x8c0741ecu;r[0]=56u;goto L8c0741ee; /* mov #0x38,r0 */
L8c0741ee: *stop=0x8c0741eeu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c0741f0; /* fmov.s @(r0,r12),fr1 */
L8c0741f0: *stop=0x8c0741f0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c0741f2; /* fcmp/gt fr2,fr1 */
L8c0741f2: if((*sr&1u))goto L8c074246;goto L8c0741f4; /* bt 0x8c074246 */
L8c0741f4: *stop=0x8c0741f4u;r[0]=96u;goto L8c0741f6; /* mov #0x60,r0 */
L8c0741f6: *stop=0x8c0741f6u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c0741f8; /* mov.l @(r0,r12),r0 */
L8c0741f8: *stop=0x8c0741f8u;*sr=(*sr&~1u)|!(r[0]&0x1cu);goto L8c0741fa; /* tst #0x1c,r0 */
L8c0741fa: if(!(*sr&1u))goto L8c074246;goto L8c0741fc; /* bf 0x8c074246 */
L8c0741fc: *stop=0x8c0741fcu;r[0]=0x8c074388u;goto L8c0741fe; /* mova 0x8c074388,r0 */
L8c0741fe: *stop=0x8c0741feu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c074200; /* fmov.s @r0,fr2 */
L8c074200: *stop=0x8c074200u;r[0]=28u;goto L8c074202; /* mov #0x1c,r0 */
L8c074202: *stop=0x8c074202u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c074204; /* fmov.s @(r0,r12),fr1 */
L8c074204: *stop=0x8c074204u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c074206; /* fcmp/gt fr2,fr1 */
L8c074206: if(!(*sr&1u))goto L8c074246;goto L8c074208; /* bf 0x8c074246 */
L8c074208: *stop=0x8c074208u;r[1]=4u;goto L8c07420a; /* mov #0x4,r1 */
L8c07420a: *stop=0x8c07420au;r[1]+=r[11];goto L8c07420c; /* add r11,r1 */
L8c07420c: *stop=0x8c07420cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c07420e; /* fmov.s @r1,fr3 */
L8c07420e: *stop=0x8c07420eu;r[0]=32u;goto L8c074210; /* mov #0x20,r0 */
L8c074210: *stop=0x8c074210u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074212; /* fmov.s @(r0,r13),fr2 */
L8c074212: *stop=0x8c074212u;r[1]=8u;goto L8c074214; /* mov #0x8,r1 */
L8c074214: *stop=0x8c074214u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[12],&f->fpscr,&fr[3]);if(status)return status;}goto L8c074216; /* fmul fr12,fr3 */
L8c074216: *stop=0x8c074216u;r[1]+=r[11];goto L8c074218; /* add r11,r1 */
L8c074218: *stop=0x8c074218u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07421a; /* fsub fr3,fr2 */
L8c07421a: *stop=0x8c07421au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07421c; /* fmov fr2,@(r0,r13) */
L8c07421c: *stop=0x8c07421cu;r[0]=36u;goto L8c07421e; /* mov #0x24,r0 */
L8c07421e: *stop=0x8c07421eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c074220; /* fmov.s @r1,fr3 */
L8c074220: *stop=0x8c074220u;r[1]=12u;goto L8c074222; /* mov #0xc,r1 */
L8c074222: *stop=0x8c074222u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074224; /* fmov.s @(r0,r13),fr2 */
L8c074224: *stop=0x8c074224u;r[1]+=r[11];goto L8c074226; /* add r11,r1 */
L8c074226: *stop=0x8c074226u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[12],&f->fpscr,&fr[3]);if(status)return status;}goto L8c074228; /* fmul fr12,fr3 */
L8c074228: *stop=0x8c074228u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07422a; /* fsub fr3,fr2 */
L8c07422a: *stop=0x8c07422au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07422c; /* fmov fr2,@(r0,r13) */
L8c07422c: *stop=0x8c07422cu;r[0]=40u;goto L8c07422e; /* mov #0x28,r0 */
L8c07422e: *stop=0x8c07422eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c074230; /* fmov.s @r1,fr3 */
L8c074230: *stop=0x8c074230u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074232; /* fmov.s @(r0,r13),fr2 */
L8c074232: *stop=0x8c074232u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[12],&f->fpscr,&fr[3]);if(status)return status;}goto L8c074234; /* fmul fr12,fr3 */
L8c074234: *stop=0x8c074234u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074236; /* fsub fr3,fr2 */
L8c074236: *stop=0x8c074236u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074238; /* fmov fr2,@(r0,r13) */
L8c074238: *stop=0x8c074238u;r[0]=4u;goto L8c07423a; /* mov #0x4,r0 */
L8c07423a: *stop=0x8c07423au;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[11]);if(io_status)return io_status;goto L8c07423c; /* fmov fr15,@(r0,r11) */
L8c07423c: *stop=0x8c07423cu;r[0]=8u;goto L8c07423e; /* mov #0x8,r0 */
L8c07423e: *stop=0x8c07423eu;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[11]);if(io_status)return io_status;goto L8c074240; /* fmov fr15,@(r0,r11) */
L8c074240: *stop=0x8c074240u;r[0]=12u;goto L8c074242; /* mov #0xc,r0 */
L8c074242: *stop=0x8c074244u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[11]);if(io_status)return io_status;goto L8c074270; /* bra 0x8c074270 */
L8c074246: *stop=0x8c074246u;r[1]=16u;goto L8c074248; /* mov #0x10,r1 */
L8c074248: *stop=0x8c074248u;r[1]+=r[14];goto L8c07424a; /* add r14,r1 */
L8c07424a: *stop=0x8c07424au;r[0]=32u;goto L8c07424c; /* mov #0x20,r0 */
L8c07424c: *stop=0x8c07424cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c07424e; /* fmov.s @r1,fr3 */
L8c07424e: *stop=0x8c07424eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074250; /* fmov.s @(r0,r13),fr2 */
L8c074250: *stop=0x8c074250u;r[1]=20u;goto L8c074252; /* mov #0x14,r1 */
L8c074252: *stop=0x8c074252u;r[1]+=r[14];goto L8c074254; /* add r14,r1 */
L8c074254: *stop=0x8c074254u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c074256; /* fadd fr3,fr2 */
L8c074256: *stop=0x8c074256u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074258; /* fmov fr2,@(r0,r13) */
L8c074258: *stop=0x8c074258u;r[0]=36u;goto L8c07425a; /* mov #0x24,r0 */
L8c07425a: *stop=0x8c07425au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07425c; /* fmov.s @(r0,r13),fr2 */
L8c07425c: *stop=0x8c07425cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c07425e; /* fmov.s @r1,fr3 */
L8c07425e: *stop=0x8c07425eu;r[1]=24u;goto L8c074260; /* mov #0x18,r1 */
L8c074260: *stop=0x8c074260u;r[1]+=r[14];goto L8c074262; /* add r14,r1 */
L8c074262: *stop=0x8c074262u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c074264; /* fadd fr3,fr2 */
L8c074264: *stop=0x8c074264u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074266; /* fmov fr2,@(r0,r13) */
L8c074266: *stop=0x8c074266u;r[0]=40u;goto L8c074268; /* mov #0x28,r0 */
L8c074268: *stop=0x8c074268u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07426a; /* fmov.s @(r0,r13),fr2 */
L8c07426a: *stop=0x8c07426au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c07426c; /* fmov.s @r1,fr3 */
L8c07426c: *stop=0x8c07426cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c07426e; /* fadd fr3,fr2 */
L8c07426e: *stop=0x8c07426eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074270; /* fmov fr2,@(r0,r13) */
L8c074270: *stop=0x8c074270u;{uint32_t loaded=rd(m,bus,&io_status,0x8c07438cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074272; /* mov.l 0x8c07438c,r3 */
L8c074272: *stop=0x8c074272u;r[5]=r[11];goto L8c074274; /* mov r11,r5 */
L8c074274: *stop=0x8c074274u;r[7]=0u;goto L8c074276; /* mov #0x0,r7 */
L8c074276: *stop=0x8c074276u;r[6]=r[12];goto L8c074278; /* mov r12,r6 */
L8c074278: target=r[3];*pr=0x8c07427cu;*stop=0x8c07427au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07427c; /* jsr @r3 */
L8c07427c: *stop=0x8c07427eu;goto L8c0742b0; /* bra 0x8c0742b0 */
L8c074280: *stop=0x8c074280u;if(!valid_access(r[15]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074282; /* mov.l @(0x38,r15),r3 */
L8c074282: *stop=0x8c074282u;r[0]=116u;goto L8c074284; /* mov #0x74,r0 */
L8c074284: *stop=0x8c074284u;r[1]=16u;goto L8c074286; /* mov #0x10,r1 */
L8c074286: *stop=0x8c074286u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[3],4);if(io_status)return io_status;goto L8c074288; /* mov.l r3,@(r0,r12) */
L8c074288: *stop=0x8c074288u;r[1]+=r[14];goto L8c07428a; /* add r14,r1 */
L8c07428a: *stop=0x8c07428au;r[0]=32u;goto L8c07428c; /* mov #0x20,r0 */
L8c07428c: *stop=0x8c07428cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c07428e; /* fmov.s @r1,fr3 */
L8c07428e: *stop=0x8c07428eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074290; /* fmov.s @(r0,r13),fr2 */
L8c074290: *stop=0x8c074290u;r[1]=20u;goto L8c074292; /* mov #0x14,r1 */
L8c074292: *stop=0x8c074292u;r[1]+=r[14];goto L8c074294; /* add r14,r1 */
L8c074294: *stop=0x8c074294u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c074296; /* fadd fr3,fr2 */
L8c074296: *stop=0x8c074296u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074298; /* fmov fr2,@(r0,r13) */
L8c074298: *stop=0x8c074298u;r[0]=36u;goto L8c07429a; /* mov #0x24,r0 */
L8c07429a: *stop=0x8c07429au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07429c; /* fmov.s @(r0,r13),fr2 */
L8c07429c: *stop=0x8c07429cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c07429e; /* fmov.s @r1,fr3 */
L8c07429e: *stop=0x8c07429eu;r[1]=24u;goto L8c0742a0; /* mov #0x18,r1 */
L8c0742a0: *stop=0x8c0742a0u;r[1]+=r[14];goto L8c0742a2; /* add r14,r1 */
L8c0742a2: *stop=0x8c0742a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0742a4; /* fadd fr3,fr2 */
L8c0742a4: *stop=0x8c0742a4u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0742a6; /* fmov fr2,@(r0,r13) */
L8c0742a6: *stop=0x8c0742a6u;r[0]=40u;goto L8c0742a8; /* mov #0x28,r0 */
L8c0742a8: *stop=0x8c0742a8u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0742aa; /* fmov.s @(r0,r13),fr2 */
L8c0742aa: *stop=0x8c0742aau;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0742ac; /* fmov.s @r1,fr3 */
L8c0742ac: *stop=0x8c0742acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0742ae; /* fadd fr3,fr2 */
L8c0742ae: *stop=0x8c0742aeu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0742b0; /* fmov fr2,@(r0,r13) */
L8c0742b0: *stop=0x8c0742b0u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0742b2; /* mov.l @(0x14,r15),r0 */
L8c0742b2: *stop=0x8c0742b2u;r[1]=2u;goto L8c0742b4; /* mov #0x2,r1 */
L8c0742b4: *stop=0x8c0742b4u;r[1]<<=8;goto L8c0742b6; /* shll8 r1 */
L8c0742b6: *stop=0x8c0742b6u;r[1]+=48u;goto L8c0742b8; /* add #0x30,r1 */
L8c0742b8: *stop=0x8c0742b8u;r[15]+=r[1];goto L8c0742ba; /* add r1,r15 */
L8c0742ba: *stop=0x8c0742bau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0742bc; /* lds.l @r15+,PR */
L8c0742bc: *stop=0x8c0742bcu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0742be; /* fmov.s @r15+,fr12 */
L8c0742be: *stop=0x8c0742beu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0742c0; /* fmov.s @r15+,fr13 */
L8c0742c0: *stop=0x8c0742c0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0742c2; /* fmov.s @r15+,fr14 */
L8c0742c2: *stop=0x8c0742c2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0742c4; /* fmov.s @r15+,fr15 */
L8c0742c4: *stop=0x8c0742c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c0742c6; /* mov.l @r15+,r8 */
L8c0742c6: *stop=0x8c0742c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c0742c8; /* mov.l @r15+,r9 */
L8c0742c8: *stop=0x8c0742c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c0742ca; /* mov.l @r15+,r10 */
L8c0742ca: *stop=0x8c0742cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c0742cc; /* mov.l @r15+,r11 */
L8c0742cc: *stop=0x8c0742ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c0742ce; /* mov.l @r15+,r12 */
L8c0742ce: *stop=0x8c0742ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0742d0; /* mov.l @r15+,r13 */
L8c0742d0: *stop=0x8c0742d0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0742d2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_movement_route_8c0730d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0730e6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0730e6u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0730f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0730f8u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073214(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073214u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073226(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073226u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073238(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073238u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c07324a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07324au);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073258(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073258u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073266(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073266u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073278(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073278u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073286(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073286u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0732ea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0732eau);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073366(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073366u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073382(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073382u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0733ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0733ecu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073482(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073482u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073494(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073494u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c07366a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07366au);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c07373c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07373cu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073752(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073752u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073762(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073762u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073810(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073810u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c07381e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07381eu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073848(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073848u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073864(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073864u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073870(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073870u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c07388e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07388eu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0738a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0738a0u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0738ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0738aeu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0738e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0738e8u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0738f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0738f4u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073930(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073930u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c07394e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07394eu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073970(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073970u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0739a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0739a4u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0739b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0739b4u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073a54(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073a54u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073a9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073a9cu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073aae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073aaeu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073ad8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073ad8u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073ae4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073ae4u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073af8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073af8u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073b04(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073b04u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073b1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073b1eu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073b68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073b68u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073b96(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073b96u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073ba2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073ba2u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073bb0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073bb0u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073bc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073bc0u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073bec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073becu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073bfa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073bfau);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073c1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073c1eu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073c54(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073c54u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073c62(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073c62u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073cba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073cbau);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073ccc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073cccu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073cde(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073cdeu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073cfa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073cfau);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073d4e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073d4eu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073d98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073d98u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073da6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073da6u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073db6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073db6u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073df8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073df8u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073e0c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073e0cu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073e14(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073e14u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073f24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073f24u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073f78(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073f78u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c073ff6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c073ff6u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c074068(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074068u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0740a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0740a0u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0740ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0740bau);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0740ca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0740cau);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0740de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0740deu);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c074108(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074108u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c074186(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074186u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c0741c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0741c6u);}
SA_STAGE_EXPORT int sa_player_movement_route_resume_8c07427c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_route_run_8c0730d8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07427cu);}
