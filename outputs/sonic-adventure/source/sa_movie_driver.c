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
SA_STAGE_EXPORT int sa_movie_driver_8c604702(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c604702;
L8c604702: *stop=0x8c604702u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c604704; /* mov.l r14,@-r15 */
L8c604704: *stop=0x8c604704u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c604706; /* mov.l r13,@-r15 */
L8c604706: *stop=0x8c604706u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c604708; /* mov.l r12,@-r15 */
L8c604708: *stop=0x8c604708u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c60470a; /* mov.l r11,@-r15 */
L8c60470a: *stop=0x8c60470au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c60470c; /* mov.l r10,@-r15 */
L8c60470c: *stop=0x8c60470cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c60470e; /* fmov.s fr15,@-r15 */
L8c60470e: *stop=0x8c60470eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c604710; /* fmov.s fr14,@-r15 */
L8c604710: *stop=0x8c604710u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c604712; /* fmov.s fr13,@-r15 */
L8c604712: *stop=0x8c604712u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c604714; /* sts.l PR,@-r15 */
L8c604714: *stop=0x8c604714u;r[15]+=4294967284u;goto L8c604716; /* add #-0xc,r15 */
L8c604716: *stop=0x8c604716u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604784u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604718; /* mov.l 0x8c604784,r3 */
L8c604718: *stop=0x8c604718u;r[13]=r[4];goto L8c60471a; /* mov r4,r13 */
L8c60471a: *stop=0x8c60471au;{uint32_t loaded=rd(m,bus,&io_status,0x8c604788u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c60471c; /* mov.l 0x8c604788,r1 */
L8c60471c: *stop=0x8c60471cu;r[11]=3u;goto L8c60471e; /* mov #0x3,r11 */
L8c60471e: *stop=0x8c60471eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604798u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c604720; /* mov.l 0x8c604798,r14 */
L8c604720: *stop=0x8c604720u;r[0]=0x8c604790u;goto L8c604722; /* mova 0x8c604790,r0 */
L8c604722: *stop=0x8c604722u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604794u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c604724; /* mov.l 0x8c604794,r10 */
L8c604724: *stop=0x8c604724u;r[11]&=r[13];goto L8c604726; /* and r13,r11 */
L8c604726: *stop=0x8c604726u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c604728; /* mov.l r5,@r15 */
L8c604728: *stop=0x8c604728u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c60472a; /* mov.l r13,@r3 */
L8c60472a: *stop=0x8c60472au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60472c; /* mov.l @r15,r2 */
L8c60472c: *stop=0x8c60472cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c604770u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c60472e; /* mov.w 0x8c604770,r3 */
L8c60472e: *stop=0x8c60472eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c604730; /* mov.l r2,@r1 */
L8c604730: *stop=0x8c604730u;r[1]=16u;goto L8c604732; /* mov #0x10,r1 */
L8c604732: *stop=0x8c604732u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60478cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604734; /* mov.l 0x8c60478c,r2 */
L8c604734: *stop=0x8c604734u;r[3]&=r[13];goto L8c604736; /* and r13,r3 */
L8c604736: *stop=0x8c604736u;*sr=(*sr&~1u)|!(r[13]&r[1]);goto L8c604738; /* tst r13,r1 */
L8c604738: *stop=0x8c604738u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[6],4);if(io_status)return io_status;goto L8c60473a; /* mov.l r6,@r2 */
L8c60473a: *stop=0x8c60473au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c60473c; /* mov.l r3,@(0x8,r15) */
L8c60473c: take=(*sr&1u);*stop=0x8c60473eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]);if(io_status)return io_status;if(take)goto L8c604748;goto L8c604740; /* bt/s 0x8c604748 */
L8c604740: *stop=0x8c604740u;{uint32_t loaded=sx(m,bus,&io_status,0x8c604772u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c604742; /* mov.w 0x8c604772,r3 */
L8c604742: *stop=0x8c604742u;r[0]=4u;goto L8c604744; /* mov #0x4,r0 */
L8c604744: *stop=0x8c604746u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,13u,r[0]+r[14]);if(io_status)return io_status;goto L8c604752; /* bra 0x8c604752 */
L8c604748: *stop=0x8c604748u;r[0]=0x8c60479cu;goto L8c60474a; /* mova 0x8c60479c,r0 */
L8c60474a: *stop=0x8c60474au;{uint32_t loaded=sx(m,bus,&io_status,0x8c604774u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c60474c; /* mov.w 0x8c604774,r3 */
L8c60474c: *stop=0x8c60474cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c60474e; /* fmov.s @r0,fr3 */
L8c60474e: *stop=0x8c60474eu;r[0]=4u;goto L8c604750; /* mov #0x4,r0 */
L8c604750: *stop=0x8c604750u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c604752; /* fmov fr3,@(r0,r14) */
L8c604752: *stop=0x8c604752u;r[0]=0x8c6047a0u;goto L8c604754; /* mova 0x8c6047a0,r0 */
L8c604754: *stop=0x8c604754u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c604756; /* mov.l r3,@r10 */
L8c604756: *stop=0x8c604756u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6047a8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c604758; /* mov.l 0x8c6047a8,r12 */
L8c604758: *stop=0x8c604758u;r[3]=32u;goto L8c60475a; /* mov #0x20,r3 */
L8c60475a: *stop=0x8c60475au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c60475c; /* fmov.s @r0,fr14 */
L8c60475c: *stop=0x8c60475cu;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c60475e; /* tst r13,r3 */
L8c60475e: *stop=0x8c60475eu;r[0]=0x8c6047a4u;goto L8c604760; /* mova 0x8c6047a4,r0 */
L8c604760: take=(*sr&1u);*stop=0x8c604762u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;if(take)goto L8c6047ac;goto L8c604764; /* bt/s 0x8c6047ac */
L8c604764: *stop=0x8c604764u;{uint32_t loaded=sx(m,bus,&io_status,0x8c604776u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c604766; /* mov.w 0x8c604776,r3 */
L8c604766: *stop=0x8c604766u;r[0]=8u;goto L8c604768; /* mov #0x8,r0 */
L8c604768: *stop=0x8c60476au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c6047b2; /* bra 0x8c6047b2 */
L8c6047ac: *stop=0x8c6047acu;{uint32_t loaded=sx(m,bus,&io_status,0x8c604858u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6047ae; /* mov.w 0x8c604858,r3 */
L8c6047ae: *stop=0x8c6047aeu;r[0]=8u;goto L8c6047b0; /* mov #0x8,r0 */
L8c6047b0: *stop=0x8c6047b0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c6047b2; /* fmov fr14,@(r0,r14) */
L8c6047b2: *stop=0x8c6047b2u;r[0]=r[11];goto L8c6047b4; /* mov r11,r0 */
L8c6047b4: *stop=0x8c6047b4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6047b6; /* cmp/eq #0x1,r0 */
L8c6047b6: take=!(*sr&1u);*stop=0x8c6047b8u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;if(take)goto L8c6047d4;goto L8c6047ba; /* bf/s 0x8c6047d4 */
L8c6047ba: target=0x8c604594u;*pr=0x8c6047beu;*stop=0x8c6047bcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6047be; /* bsr 0x8c604594 */
L8c6047be: target=0x8c6045b4u;*pr=0x8c6047c2u;*stop=0x8c6047c0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6047c2; /* bsr 0x8c6045b4 */
L8c6047c2: *stop=0x8c6047c2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c60485au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6047c4; /* mov.w 0x8c60485a,r3 */
L8c6047c4: *stop=0x8c6047c4u;r[0]=4u;goto L8c6047c6; /* mov #0x4,r0 */
L8c6047c6: *stop=0x8c6047c6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,13u,r[0]+r[14]);if(io_status)return io_status;goto L8c6047c8; /* fmov fr13,@(r0,r14) */
L8c6047c8: *stop=0x8c6047c8u;r[0]=8u;goto L8c6047ca; /* mov #0x8,r0 */
L8c6047ca: *stop=0x8c6047cau;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c6047cc; /* mov.l r3,@r10 */
L8c6047cc: *stop=0x8c6047ccu;{uint32_t loaded=sx(m,bus,&io_status,0x8c60485cu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6047ce; /* mov.w 0x8c60485c,r3 */
L8c6047ce: *stop=0x8c6047ceu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c6047d0; /* fmov fr15,@(r0,r14) */
L8c6047d0: *stop=0x8c6047d2u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;goto L8c604808; /* bra 0x8c604808 */
L8c6047d4: *stop=0x8c6047d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604864u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6047d6; /* mov.l 0x8c604864,r1 */
L8c6047d6: target=r[1];*pr=0x8c6047dau;*stop=0x8c6047d8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6047da; /* jsr @r1 */
L8c6047da: target=0x8c604594u;*pr=0x8c6047deu;*stop=0x8c6047dcu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6047de; /* bsr 0x8c604594 */
L8c6047de: *stop=0x8c6047deu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6047e0; /* mov.l @(0x4,r15),r0 */
L8c6047e0: *stop=0x8c6047e0u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c6047e2; /* cmp/eq #0x2,r0 */
L8c6047e2: if((*sr&1u))goto L8c6047f0;goto L8c6047e4; /* bt 0x8c6047f0 */
L8c6047e4: *stop=0x8c6047e4u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c6047e6; /* cmp/eq #0x3,r0 */
L8c6047e6: if((*sr&1u))goto L8c6047f6;goto L8c6047e8; /* bt 0x8c6047f6 */
L8c6047e8: *stop=0x8c6047e8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6047ea; /* cmp/eq #0x1,r0 */
L8c6047ea: if((*sr&1u))goto L8c6047fc;goto L8c6047ec; /* bt 0x8c6047fc */
L8c6047ec: *stop=0x8c6047eeu;goto L8c604802; /* bra 0x8c604802 */
L8c6047f0: *stop=0x8c6047f0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c60485eu,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6047f2; /* mov.w 0x8c60485e,r4 */
L8c6047f2: *stop=0x8c6047f4u;goto L8c604804; /* bra 0x8c604804 */
L8c6047f6: *stop=0x8c6047f6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c604860u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6047f8; /* mov.w 0x8c604860,r4 */
L8c6047f8: *stop=0x8c6047fau;goto L8c604804; /* bra 0x8c604804 */
L8c6047fc: *stop=0x8c6047fcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c604862u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6047fe; /* mov.w 0x8c604862,r4 */
L8c6047fe: *stop=0x8c604800u;goto L8c604804; /* bra 0x8c604804 */
L8c604802: *stop=0x8c604802u;{uint32_t loaded=sx(m,bus,&io_status,0x8c604860u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c604804; /* mov.w 0x8c604860,r4 */
L8c604804: target=0x8c6045b4u;*pr=0x8c604808u;*stop=0x8c604806u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604808; /* bsr 0x8c6045b4 */
L8c604808: *stop=0x8c604808u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604868u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60480a; /* mov.l 0x8c604868,r3 */
L8c60480a: *stop=0x8c60480au;r[10]=1u;goto L8c60480c; /* mov #0x1,r10 */
L8c60480c: *stop=0x8c60480cu;*sr=(*sr&~1u)|!(r[3]&r[13]);goto L8c60480e; /* tst r3,r13 */
L8c60480e: take=(*sr&1u);*stop=0x8c604810u;r[12]=0u;if(take)goto L8c60482c;goto L8c604812; /* bt/s 0x8c60482c */
L8c604812: *stop=0x8c604812u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c604814; /* fldi1 fr3 */
L8c604814: *stop=0x8c604814u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c604816; /* fadd fr3,fr3 */
L8c604816: *stop=0x8c604816u;r[0]=4u;goto L8c604818; /* mov #0x4,r0 */
L8c604818: *stop=0x8c604818u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c60481a; /* fmov.s @(r0,r14),fr2 */
L8c60481a: *stop=0x8c60481au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60481c; /* fmul fr3,fr2 */
L8c60481c: *stop=0x8c60481cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c60481e; /* fmov fr2,@(r0,r14) */
L8c60481e: *stop=0x8c60481eu;r[0]=8u;goto L8c604820; /* mov #0x8,r0 */
L8c604820: *stop=0x8c604820u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c604822; /* fmov.s @(r0,r14),fr2 */
L8c604822: *stop=0x8c604822u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,0);if(status)return status;goto L8c604824; /* fcmp/eq fr14,fr2 */
L8c604824: take=!(*sr&1u);*stop=0x8c604826u;r[13]=r[10];if(take)goto L8c60482e;goto L8c604828; /* bf/s 0x8c60482e */
L8c604828: *stop=0x8c60482au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c60482e; /* bra 0x8c60482e */
L8c60482c: *stop=0x8c60482cu;r[13]=r[12];goto L8c60482e; /* mov r12,r13 */
L8c60482e: *stop=0x8c60482eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c604830; /* mov.l @r15,r0 */
L8c604830: *stop=0x8c604830u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c604832; /* cmp/eq #0x0,r0 */
L8c604832: if((*sr&1u))goto L8c604848;goto L8c604834; /* bt 0x8c604848 */
L8c604834: *stop=0x8c604834u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c604836; /* cmp/eq #0x1,r0 */
L8c604836: if((*sr&1u))goto L8c60484c;goto L8c604838; /* bt 0x8c60484c */
L8c604838: *stop=0x8c604838u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c60483a; /* cmp/eq #0x3,r0 */
L8c60483a: if((*sr&1u))goto L8c604850;goto L8c60483c; /* bt 0x8c604850 */
L8c60483c: *stop=0x8c60483cu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c60483e; /* cmp/eq #0x4,r0 */
L8c60483e: if((*sr&1u))goto L8c604854;goto L8c604840; /* bt 0x8c604854 */
L8c604840: *stop=0x8c604840u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c604842; /* cmp/eq #0x5,r0 */
L8c604842: if((*sr&1u))goto L8c60486c;goto L8c604844; /* bt 0x8c60486c */
L8c604844: *stop=0x8c604846u;goto L8c60486e; /* bra 0x8c60486e */
L8c604848: *stop=0x8c60484au;r[14]=r[12];goto L8c60486e; /* bra 0x8c60486e */
L8c60484c: *stop=0x8c60484eu;r[14]=r[10];goto L8c60486e; /* bra 0x8c60486e */
L8c604850: *stop=0x8c604852u;r[14]=3u;goto L8c60486e; /* bra 0x8c60486e */
L8c604854: *stop=0x8c604856u;r[14]=4u;goto L8c60486e; /* bra 0x8c60486e */
L8c60486c: *stop=0x8c60486cu;r[14]=5u;goto L8c60486e; /* mov #0x5,r14 */
L8c60486e: *stop=0x8c60486eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604954u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c604870; /* mov.l 0x8c604954,r5 */
L8c604870: *stop=0x8c604870u;r[4]=8u;goto L8c604872; /* mov #0x8,r4 */
L8c604872: *stop=0x8c604872u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604874; /* mov.l @r5,r2 */
L8c604874: *stop=0x8c604874u;r[2]|=r[4];goto L8c604876; /* or r4,r2 */
L8c604876: *stop=0x8c604876u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[2],4);if(io_status)return io_status;goto L8c604878; /* mov.l r2,@r5 */
L8c604878: *stop=0x8c604878u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604958u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c60487a; /* mov.l 0x8c604958,r5 */
L8c60487a: *stop=0x8c60487au;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c60487c; /* mov.l r4,@r5 */
L8c60487c: *stop=0x8c60487cu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60487e; /* mov.l @r5,r2 */
L8c60487e: *stop=0x8c60487eu;*sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c604880; /* tst r4,r2 */
L8c604880: if((*sr&1u))goto L8c60487c;goto L8c604882; /* bt 0x8c60487c */
L8c604882: *stop=0x8c604882u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60495cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604884; /* mov.l 0x8c60495c,r2 */
L8c604884: target=r[2];*pr=0x8c604888u;*stop=0x8c604886u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604888; /* jsr @r2 */
L8c604888: *stop=0x8c604888u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604960u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60488a; /* mov.l 0x8c604960,r3 */
L8c60488a: *stop=0x8c60488au;r[5]=r[14];goto L8c60488c; /* mov r14,r5 */
L8c60488c: *stop=0x8c60488cu;r[7]=r[13];goto L8c60488e; /* mov r13,r7 */
L8c60488e: *stop=0x8c60488eu;r[6]=1u;goto L8c604890; /* mov #0x1,r6 */
L8c604890: target=r[3];*pr=0x8c604894u;*stop=0x8c604892u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604894; /* jsr @r3 */
L8c604894: *stop=0x8c604894u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604964u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604896; /* mov.l 0x8c604964,r4 */
L8c604896: *stop=0x8c604896u;r[2]=96u;goto L8c604898; /* mov #0x60,r2 */
L8c604898: *stop=0x8c604898u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604968u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c60489a; /* mov.l 0x8c604968,r5 */
L8c60489a: *stop=0x8c60489au;{uint32_t loaded=rd(m,bus,&io_status,0x8c60496cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60489c; /* mov.l 0x8c60496c,r3 */
L8c60489c: *stop=0x8c60489cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c60489e; /* mov.l r2,@r4 */
L8c60489e: *stop=0x8c60489eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604970u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6048a0; /* mov.l 0x8c604970,r2 */
L8c6048a0: *stop=0x8c6048a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604978u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6048a2; /* mov.l 0x8c604978,r0 */
L8c6048a2: *stop=0x8c6048a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604974u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6048a4; /* mov.l 0x8c604974,r1 */
L8c6048a4: *stop=0x8c6048a4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c6048a6; /* fldi0 fr4 */
L8c6048a6: *stop=0x8c6048a6u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[10],4);if(io_status)return io_status;goto L8c6048a8; /* mov.l r10,@(0x4,r4) */
L8c6048a8: *stop=0x8c6048a8u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6048aa; /* mov.l r3,@r5 */
L8c6048aa: *stop=0x8c6048aau;r[3]=0u;goto L8c6048ac; /* mov #0x0,r3 */
L8c6048ac: *stop=0x8c6048acu;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6048ae; /* mov.l r2,@(0x4,r5) */
L8c6048ae: *stop=0x8c6048aeu;r[2]=r[3];goto L8c6048b0; /* mov r3,r2 */
L8c6048b0: *stop=0x8c6048b0u;r[2]=0u;goto L8c6048b2; /* mov #0x0,r2 */
L8c6048b2: *stop=0x8c6048b2u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;goto L8c6048b4; /* mov.l r5,@(0x8,r4) */
L8c6048b4: *stop=0x8c6048b4u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6048b6; /* mov.l r3,@(0x10,r4) */
L8c6048b6: *stop=0x8c6048b6u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[3],4);if(io_status)return io_status;goto L8c6048b8; /* mov.l r3,@(0x14,r4) */
L8c6048b8: *stop=0x8c6048b8u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6048ba; /* mov.l @r1,r3 */
L8c6048ba: *stop=0x8c6048bau;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c6048bc; /* mov.l r3,@(0x1c,r4) */
L8c6048bc: *stop=0x8c6048bcu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c6048be; /* mov.l r2,@r0 */
L8c6048be: *stop=0x8c6048beu;r[0]=44u;goto L8c6048c0; /* mov #0x2c,r0 */
L8c6048c0: *stop=0x8c6048c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60497cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6048c2; /* mov.l 0x8c60497c,r3 */
L8c6048c2: *stop=0x8c6048c2u;if(!valid_access(r[4]+32u,4u))return -3;wr(m,bus,&io_status,r[4]+0x20u,r[3],4);if(io_status)return io_status;goto L8c6048c4; /* mov.l r3,@(0x20,r4) */
L8c6048c4: *stop=0x8c6048c4u;if(!valid_access(r[4]+36u,4u))return -3;wr(m,bus,&io_status,r[4]+0x24u,r[12],4);if(io_status)return io_status;goto L8c6048c6; /* mov.l r12,@(0x24,r4) */
L8c6048c6: *stop=0x8c6048c6u;if(!valid_access(r[4]+40u,4u))return -3;wr(m,bus,&io_status,r[4]+0x28u,r[2],4);if(io_status)return io_status;goto L8c6048c8; /* mov.l r2,@(0x28,r4) */
L8c6048c8: *stop=0x8c6048c8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c6048ca; /* fmov fr4,@(r0,r4) */
L8c6048ca: *stop=0x8c6048cau;r[0]=48u;goto L8c6048cc; /* mov #0x30,r0 */
L8c6048cc: *stop=0x8c6048ccu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c6048ce; /* fmov fr4,@(r0,r4) */
L8c6048ce: *stop=0x8c6048ceu;r[0]=52u;goto L8c6048d0; /* mov #0x34,r0 */
L8c6048d0: *stop=0x8c6048d0u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c6048d2; /* fmov fr4,@(r0,r4) */
L8c6048d2: *stop=0x8c6048d2u;r[0]=56u;goto L8c6048d4; /* mov #0x38,r0 */
L8c6048d4: *stop=0x8c6048d4u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c6048d6; /* fmov fr4,@(r0,r4) */
L8c6048d6: *stop=0x8c6048d6u;r[0]=60u;goto L8c6048d8; /* mov #0x3c,r0 */
L8c6048d8: *stop=0x8c6048d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604980u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6048da; /* mov.l 0x8c604980,r3 */
L8c6048da: *stop=0x8c6048dau;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c6048dc; /* fmov fr4,@(r0,r4) */
L8c6048dc: *stop=0x8c6048dcu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8c6048de; /* mov.l r12,@r3 */
L8c6048de: *stop=0x8c6048deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604984u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6048e0; /* mov.l 0x8c604984,r2 */
L8c6048e0: *stop=0x8c6048e0u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[12],4);if(io_status)return io_status;goto L8c6048e2; /* mov.l r12,@r2 */
L8c6048e2: *stop=0x8c6048e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604988u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6048e4; /* mov.l 0x8c604988,r0 */
L8c6048e4: *stop=0x8c6048e4u;r[1]=1u;goto L8c6048e6; /* mov #0x1,r1 */
L8c6048e6: *stop=0x8c6048e6u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c6048e8; /* mov.l r1,@r0 */
L8c6048e8: *stop=0x8c6048e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60498cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6048ea; /* mov.l 0x8c60498c,r3 */
L8c6048ea: *stop=0x8c6048eau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8c6048ec; /* mov.l r12,@r3 */
L8c6048ec: *stop=0x8c6048ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604994u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6048ee; /* mov.l 0x8c604994,r2 */
L8c6048ee: *stop=0x8c6048eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604990u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6048f0; /* mov.l 0x8c604990,r4 */
L8c6048f0: target=r[2];*pr=0x8c6048f4u;*stop=0x8c6048f2u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6048f4; /* jsr @r2 */
L8c6048f4: *stop=0x8c6048f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60499cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6048f6; /* mov.l 0x8c60499c,r3 */
L8c6048f6: *stop=0x8c6048f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604998u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6048f8; /* mov.l 0x8c604998,r4 */
L8c6048f8: target=r[3];*pr=0x8c6048fcu;*stop=0x8c6048fau;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6048fc; /* jsr @r3 */
L8c6048fc: *stop=0x8c6048fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6049a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6048fe; /* mov.l 0x8c6049a4,r2 */
L8c6048fe: *stop=0x8c6048feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6049a0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604900; /* mov.l 0x8c6049a0,r4 */
L8c604900: target=r[2];*pr=0x8c604904u;*stop=0x8c604902u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604904; /* jsr @r2 */
L8c604904: *stop=0x8c604904u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6049acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604906; /* mov.l 0x8c6049ac,r2 */
L8c604906: *stop=0x8c604906u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6049a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604908; /* mov.l 0x8c6049a8,r3 */
L8c604908: target=r[2];*pr=0x8c60490cu;*stop=0x8c60490au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60490c; /* jsr @r2 */
L8c60490c: target=0x8c604ba0u;*pr=0x8c604910u;*stop=0x8c60490eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604910; /* bsr 0x8c604ba0 */
L8c604910: *stop=0x8c604910u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6049b0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c604912; /* mov.l 0x8c6049b0,r1 */
L8c604912: *stop=0x8c604912u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604914; /* mov.l @r1,r2 */
L8c604914: *stop=0x8c604914u;r[3]=10u;goto L8c604916; /* mov #0xa,r3 */
L8c604916: *stop=0x8c604916u;r[2]=r[2]&255u;goto L8c604918; /* extu.b r2,r2 */
L8c604918: *stop=0x8c604918u;*sr=(*sr&~1u)|(r[2]>=r[3]);goto L8c60491a; /* cmp/hs r3,r2 */
L8c60491a: if((*sr&1u))goto L8c60493e;goto L8c60491c; /* bt 0x8c60493e */
L8c60491c: *stop=0x8c60491cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6049b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60491e; /* mov.l 0x8c6049b8,r2 */
L8c60491e: *stop=0x8c60491eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6049b4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604920; /* mov.l 0x8c6049b4,r4 */
L8c604920: target=r[2];*pr=0x8c604924u;*stop=0x8c604922u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604924; /* jsr @r2 */
L8c604924: *stop=0x8c604924u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6049bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c604926; /* mov.l 0x8c6049bc,r2 */
L8c604926: *stop=0x8c604926u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c604928; /* mov.l @r2,r3 */
L8c604928: *stop=0x8c604928u;r[15]+=12u;goto L8c60492a; /* add #0xc,r15 */
L8c60492a: *stop=0x8c60492au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c60492c; /* lds.l @r15+,PR */
L8c60492c: *stop=0x8c60492cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c60492e; /* fmov.s @r15+,fr13 */
L8c60492e: *stop=0x8c60492eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c604930; /* fmov.s @r15+,fr14 */
L8c604930: *stop=0x8c604930u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c604932; /* fmov.s @r15+,fr15 */
L8c604932: *stop=0x8c604932u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c604934; /* mov.l @r15+,r10 */
L8c604934: *stop=0x8c604934u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c604936; /* mov.l @r15+,r11 */
L8c604936: *stop=0x8c604936u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c604938; /* mov.l @r15+,r12 */
L8c604938: *stop=0x8c604938u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c60493a; /* mov.l @r15+,r13 */
L8c60493a: target=r[3];*stop=0x8c60493cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c60493e: *stop=0x8c60493eu;r[15]+=12u;goto L8c604940; /* add #0xc,r15 */
L8c604940: *stop=0x8c604940u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c604942; /* lds.l @r15+,PR */
L8c604942: *stop=0x8c604942u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c604944; /* fmov.s @r15+,fr13 */
L8c604944: *stop=0x8c604944u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c604946; /* fmov.s @r15+,fr14 */
L8c604946: *stop=0x8c604946u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c604948; /* fmov.s @r15+,fr15 */
L8c604948: *stop=0x8c604948u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c60494a; /* mov.l @r15+,r10 */
L8c60494a: *stop=0x8c60494au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c60494c; /* mov.l @r15+,r11 */
L8c60494c: *stop=0x8c60494cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c60494e; /* mov.l @r15+,r12 */
L8c60494e: *stop=0x8c60494eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c604950; /* mov.l @r15+,r13 */
L8c604950: *stop=0x8c604950u;if(*pr!=entry_pr)return -3;*stop=0x8c604952u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c6045b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6045b4;
L8c6045b4: *stop=0x8c6045b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6045b6; /* mov.l r14,@-r15 */
L8c6045b6: *stop=0x8c6045b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6045b8; /* mov.l r13,@-r15 */
L8c6045b8: *stop=0x8c6045b8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6045ba; /* mov.l r12,@-r15 */
L8c6045ba: *stop=0x8c6045bau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6045bc; /* sts.l PR,@-r15 */
L8c6045bc: *stop=0x8c6045bcu;r[15]+=4294967264u;goto L8c6045be; /* add #-0x20,r15 */
L8c6045be: *stop=0x8c6045beu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6045c0; /* stc SR,r0 */
L8c6045c0: *stop=0x8c6045c0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c60461au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6045c2; /* mov.w 0x8c60461a,r3 */
L8c6045c2: *stop=0x8c6045c2u;r[13]=0u;goto L8c6045c4; /* mov #0x0,r13 */
L8c6045c4: *stop=0x8c6045c4u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c6045c6; /* mov.l r4,@(0x8,r15) */
L8c6045c6: *stop=0x8c6045c6u;r[0]>>=2;goto L8c6045c8; /* shlr2 r0 */
L8c6045c8: *stop=0x8c6045c8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6045ca; /* mov.l r13,@r15 */
L8c6045ca: *stop=0x8c6045cau;r[0]>>=2;goto L8c6045cc; /* shlr2 r0 */
L8c6045cc: *stop=0x8c6045ccu;r[0]&=0xfu;goto L8c6045ce; /* and #0xf,r0 */
L8c6045ce: *stop=0x8c6045ceu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6045d0; /* mov.l r0,@(0xc,r15) */
L8c6045d0: *stop=0x8c6045d0u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6045d2; /* stc SR,r0 */
L8c6045d2: *stop=0x8c6045d2u;r[0]&=r[3];goto L8c6045d4; /* and r3,r0 */
L8c6045d4: *stop=0x8c6045d4u;r[0]|=0xf0u;goto L8c6045d6; /* or #0xf0,r0 */
L8c6045d6: *stop=0x8c6045d6u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c6045d8; /* ldc r0,SR */
L8c6045d8: *stop=0x8c6045d8u;r[7]=r[15];goto L8c6045da; /* mov r15,r7 */
L8c6045da: *stop=0x8c6045dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c604668u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c6045dc; /* mov.l 0x8c604668,r12 */
L8c6045dc: *stop=0x8c6045dcu;r[7]+=16u;goto L8c6045de; /* add #0x10,r7 */
L8c6045de: *stop=0x8c6045deu;r[5]=r[13];goto L8c6045e0; /* mov r13,r5 */
L8c6045e0: *stop=0x8c6045e0u;r[0]=4u;goto L8c6045e2; /* mov #0x4,r0 */
L8c6045e2: *stop=0x8c6045e2u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[7],4);if(io_status)return io_status;goto L8c6045e4; /* mov.l r7,@(0x4,r15) */
L8c6045e4: *stop=0x8c6045e4u;r[14]=1u;goto L8c6045e6; /* mov #0x1,r14 */
L8c6045e6: *stop=0x8c6045e6u;r[4]=r[7];goto L8c6045e8; /* mov r7,r4 */
L8c6045e8: *stop=0x8c6045e8u;r[6]=r[5];goto L8c6045ea; /* mov r5,r6 */
L8c6045ea: *stop=0x8c6045eau;r[3]=r[5];goto L8c6045ec; /* mov r5,r3 */
L8c6045ec: *stop=0x8c6045ecu;r[3]<<=2;goto L8c6045ee; /* shll2 r3 */
L8c6045ee: *stop=0x8c6045eeu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6045f0; /* mov.l @r12,r2 */
L8c6045f0: *stop=0x8c6045f0u;r[3]<<=2;goto L8c6045f2; /* shll2 r3 */
L8c6045f2: *stop=0x8c6045f2u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c6045f4; /* shll r3 */
L8c6045f4: *stop=0x8c6045f4u;r[3]+=r[2];goto L8c6045f6; /* add r2,r3 */
L8c6045f6: *stop=0x8c6045f6u;if(!valid_access(r[3]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6045f8; /* mov.l @(0x18,r3),r1 */
L8c6045f8: *stop=0x8c6045f8u;*sr=(*sr&~1u)|!(r[14]&r[1]);goto L8c6045fa; /* tst r14,r1 */
L8c6045fa: take=(*sr&1u);*stop=0x8c6045fcu;r[5]+=1u;if(take)goto L8c604608;goto L8c6045fe; /* bt/s 0x8c604608 */
L8c6045fe: *stop=0x8c6045feu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604600; /* mov.l @r12,r2 */
L8c604600: *stop=0x8c604600u;r[2]+=r[6];goto L8c604602; /* add r6,r2 */
L8c604602: *stop=0x8c604602u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[13],4);if(io_status)return io_status;goto L8c604604; /* mov.l r13,@(0x1c,r2) */
L8c604604: *stop=0x8c604606u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c60460a; /* bra 0x8c60460a */
L8c604608: *stop=0x8c604608u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[13],4);if(io_status)return io_status;goto L8c60460a; /* mov.l r13,@r7 */
L8c60460a: *stop=0x8c60460au;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[0]);goto L8c60460c; /* cmp/ge r0,r5 */
L8c60460c: *stop=0x8c60460cu;r[4]+=4u;goto L8c60460e; /* add #0x4,r4 */
L8c60460e: *stop=0x8c60460eu;r[7]+=4u;goto L8c604610; /* add #0x4,r7 */
L8c604610: take=!(*sr&1u);*stop=0x8c604612u;r[6]+=32u;if(take)goto L8c6045ea;goto L8c604614; /* bf/s 0x8c6045ea */
L8c604614: *stop=0x8c604616u;goto L8c604670; /* bra 0x8c604670 */
L8c60466c: target=0x8c6045a8u;*pr=0x8c604670u;*stop=0x8c60466eu;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604670; /* bsr 0x8c6045a8 */
L8c604670: *stop=0x8c604670u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c604672; /* mov.l @r15,r0 */
L8c604672: *stop=0x8c604672u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c604674; /* cmp/eq #0x1,r0 */
L8c604674: if(!(*sr&1u))goto L8c60466c;goto L8c604676; /* bf 0x8c60466c */
L8c604676: *stop=0x8c604676u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604778u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604678; /* mov.l 0x8c604778,r4 */
L8c604678: *stop=0x8c604678u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[5]=loaded;}goto L8c60467a; /* mov.l @r4,r5 */
L8c60467a: *stop=0x8c60467au;{uint32_t loaded=sx(m,bus,&io_status,0x8c60476cu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c60467c; /* mov.w 0x8c60476c,r2 */
L8c60467c: *stop=0x8c60467cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60467e; /* mov.l @(0x8,r15),r3 */
L8c60467e: *stop=0x8c60467eu;r[5]&=r[2];goto L8c604680; /* and r2,r5 */
L8c604680: *stop=0x8c604680u;r[5]|=r[3];goto L8c604682; /* or r3,r5 */
L8c604682: *stop=0x8c604682u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c604684; /* mov.l r5,@r4 */
L8c604684: *stop=0x8c604684u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604686; /* mov.l @(0x4,r15),r4 */
L8c604686: *stop=0x8c604686u;r[6]=r[15];goto L8c604688; /* mov r15,r6 */
L8c604688: *stop=0x8c604688u;r[5]=r[13];goto L8c60468a; /* mov r13,r5 */
L8c60468a: *stop=0x8c60468cu;r[6]+=32u;goto L8c60469e; /* bra 0x8c60469e */
L8c60468e: *stop=0x8c60468eu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c604690; /* mov.l @r4,r0 */
L8c604690: *stop=0x8c604690u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c604692; /* cmp/eq #0x1,r0 */
L8c604692: if(!(*sr&1u))goto L8c60469a;goto L8c604694; /* bf 0x8c60469a */
L8c604694: *stop=0x8c604694u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c604696; /* mov.l @r12,r3 */
L8c604696: *stop=0x8c604696u;r[3]+=r[5];goto L8c604698; /* add r5,r3 */
L8c604698: *stop=0x8c604698u;if(!valid_access(r[3]+28u,4u))return -3;wr(m,bus,&io_status,r[3]+0x1cu,r[14],4);if(io_status)return io_status;goto L8c60469a; /* mov.l r14,@(0x1c,r3) */
L8c60469a: *stop=0x8c60469au;r[4]+=4u;goto L8c60469c; /* add #0x4,r4 */
L8c60469c: *stop=0x8c60469cu;r[5]+=32u;goto L8c60469e; /* add #0x20,r5 */
L8c60469e: *stop=0x8c60469eu;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c6046a0; /* cmp/hs r6,r4 */
L8c6046a0: if(!(*sr&1u))goto L8c60468e;goto L8c6046a2; /* bf 0x8c60468e */
L8c6046a2: *stop=0x8c6046a2u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6046a4; /* mov.l @(0xc,r15),r0 */
L8c6046a4: *stop=0x8c6046a4u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c6046a6; /* stc SR,r3 */
L8c6046a6: *stop=0x8c6046a6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c60476eu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c6046a8; /* mov.w 0x8c60476e,r2 */
L8c6046a8: *stop=0x8c6046a8u;r[0]&=0xfu;goto L8c6046aa; /* and #0xf,r0 */
L8c6046aa: *stop=0x8c6046aau;r[0]<<=2;goto L8c6046ac; /* shll2 r0 */
L8c6046ac: *stop=0x8c6046acu;r[3]&=r[2];goto L8c6046ae; /* and r2,r3 */
L8c6046ae: *stop=0x8c6046aeu;r[0]<<=2;goto L8c6046b0; /* shll2 r0 */
L8c6046b0: *stop=0x8c6046b0u;r[0]|=r[3];goto L8c6046b2; /* or r3,r0 */
L8c6046b2: *stop=0x8c6046b2u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c6046b4; /* ldc r0,SR */
L8c6046b4: *stop=0x8c6046b4u;r[15]+=32u;goto L8c6046b6; /* add #0x20,r15 */
L8c6046b6: *stop=0x8c6046b6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6046b8; /* lds.l @r15+,PR */
L8c6046b8: *stop=0x8c6046b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6046ba; /* mov.l @r15+,r12 */
L8c6046ba: *stop=0x8c6046bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6046bc; /* mov.l @r15+,r13 */
L8c6046bc: *stop=0x8c6046bcu;if(*pr!=entry_pr)return -3;*stop=0x8c6046beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c65262c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65262c;
L8c65262c: *stop=0x8c65262cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65262e; /* mov.l r14,@-r15 */
L8c65262e: *stop=0x8c65262eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c652630; /* sts.l PR,@-r15 */
L8c652630: *stop=0x8c652630u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652694u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652632; /* mov.l 0x8c652694,r3 */
L8c652632: target=r[3];*pr=0x8c652636u;*stop=0x8c652634u;r[14]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652636; /* jsr @r3 */
L8c652636: *stop=0x8c652636u;r[2]=2u;goto L8c652638; /* mov #0x2,r2 */
L8c652638: *stop=0x8c652638u;*sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[2]);goto L8c65263a; /* cmp/gt r2,r14 */
L8c65263a: if((*sr&1u))goto L8c652640;goto L8c65263c; /* bt 0x8c652640 */
L8c65263c: *stop=0x8c65263cu;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c65263e; /* cmp/pz r14 */
L8c65263e: if((*sr&1u))goto L8c6526a0;goto L8c652640; /* bt 0x8c6526a0 */
L8c652640: *stop=0x8c652640u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c652642; /* lds.l @r15+,PR */
L8c652642: *stop=0x8c652642u;r[0]=2u;goto L8c652644; /* mov #0x2,r0 */
L8c652644: *stop=0x8c652644u;if(*pr!=entry_pr)return -3;*stop=0x8c652646u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c6526a0: *stop=0x8c6526a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652744u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6526a2; /* mov.l 0x8c652744,r4 */
L8c6526a2: *stop=0x8c6526a2u;r[6]=3u;goto L8c6526a4; /* mov #0x3,r6 */
L8c6526a4: *stop=0x8c6526a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652748u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6526a6; /* mov.l 0x8c652748,r3 */
L8c6526a6: *stop=0x8c6526a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652740u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6526a8; /* mov.l 0x8c652740,r5 */
L8c6526a8: target=r[3];*pr=0x8c6526acu;*stop=0x8c6526aau;r[6]&=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6526ac; /* jsr @r3 */
L8c6526ac: *stop=0x8c6526acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652750u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6526ae; /* mov.l 0x8c652750,r3 */
L8c6526ae: *stop=0x8c6526aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65274cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6526b0; /* mov.l 0x8c65274c,r2 */
L8c6526b0: target=r[3];*pr=0x8c6526b4u;*stop=0x8c6526b2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6526b4; /* jsr @r3 */
L8c6526b4: *stop=0x8c6526b4u;r[0]=0u;goto L8c6526b6; /* mov #0x0,r0 */
L8c6526b6: *stop=0x8c6526b6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6526b8; /* lds.l @r15+,PR */
L8c6526b8: *stop=0x8c6526b8u;if(*pr!=entry_pr)return -3;*stop=0x8c6526bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c6045a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6045a8;
L8c6045a8: *stop=0x8c6045a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604664u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6045aa; /* mov.l 0x8c604664,r1 */
L8c6045aa: *stop=0x8c6045aau;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6045ac; /* mov.l @r1,r0 */
L8c6045ac: *stop=0x8c6045acu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c6045ae; /* tst #0x20,r0 */
L8c6045ae: *stop=0x8c6045aeu;r[2]=*sr&1u;goto L8c6045b0; /* movt r2 */
L8c6045b0: *stop=0x8c6045b0u;if(*pr!=entry_pr)return -3;*stop=0x8c6045b2u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c6510a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6510a4;
L8c6510a4: *stop=0x8c6510a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6510a6; /* mov.l r14,@-r15 */
L8c6510a6: *stop=0x8c6510a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6510a8; /* mov.l r13,@-r15 */
L8c6510a8: *stop=0x8c6510a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6510aa; /* mov.l r12,@-r15 */
L8c6510aa: *stop=0x8c6510aau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6510ac; /* mov.l r11,@-r15 */
L8c6510ac: *stop=0x8c6510acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6510ae; /* mov.l r10,@-r15 */
L8c6510ae: *stop=0x8c6510aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6510b0; /* mov.l r9,@-r15 */
L8c6510b0: *stop=0x8c6510b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6510b2; /* mov.l r8,@-r15 */
L8c6510b2: *stop=0x8c6510b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6510b4; /* sts.l PR,@-r15 */
L8c6510b4: *stop=0x8c6510b4u;r[15]+=4294967284u;goto L8c6510b6; /* add #-0xc,r15 */
L8c6510b6: *stop=0x8c6510b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6511e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6510b8; /* mov.l 0x8c6511e4,r2 */
L8c6510b8: *stop=0x8c6510b8u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6510ba; /* mov.l @r2,r3 */
L8c6510ba: *stop=0x8c6510bau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6510bc; /* tst r3,r3 */
L8c6510bc: if(!(*sr&1u))goto L8c6510ce;goto L8c6510be; /* bf 0x8c6510ce */
L8c6510be: *stop=0x8c6510beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6511e4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6510c0; /* mov.l 0x8c6511e4,r1 */
L8c6510c0: *stop=0x8c6510c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6511e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6510c2; /* mov.l 0x8c6511e8,r3 */
L8c6510c2: *stop=0x8c6510c2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c6510c4; /* mov.l r3,@r1 */
L8c6510c4: *stop=0x8c6510c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6511ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6510c6; /* mov.l 0x8c6511ec,r3 */
L8c6510c6: *stop=0x8c6510c6u;r[5]=0u;goto L8c6510c8; /* mov #0x0,r5 */
L8c6510c8: *stop=0x8c6510c8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6511e0u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c6510ca; /* mov.w 0x8c6511e0,r6 */
L8c6510ca: target=r[3];*pr=0x8c6510ceu;*stop=0x8c6510ccu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6510ce; /* jsr @r3 */
L8c6510ce: *stop=0x8c6510ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6511f0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6510d0; /* mov.l 0x8c6511f0,r4 */
L8c6510d0: *stop=0x8c6510d0u;r[3]=3u;goto L8c6510d2; /* mov #0x3,r3 */
L8c6510d2: *stop=0x8c6510d2u;r[2]=6u;goto L8c6510d4; /* mov #0x6,r2 */
L8c6510d4: *stop=0x8c6510d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6511e8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6510d6; /* mov.l 0x8c6511e8,r1 */
L8c6510d6: *stop=0x8c6510d6u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6510d8; /* mov.l r3,@r4 */
L8c6510d8: *stop=0x8c6510d8u;r[5]=2u;goto L8c6510da; /* mov #0x2,r5 */
L8c6510da: *stop=0x8c6510dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6511f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6510dc; /* mov.l 0x8c6511f4,r3 */
L8c6510dc: *stop=0x8c6510dcu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6510de; /* mov.l r5,@(0x4,r4) */
L8c6510de: *stop=0x8c6510deu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6510e0; /* mov.l r2,@(0x8,r4) */
L8c6510e0: *stop=0x8c6510e0u;r[2]=r[4];goto L8c6510e2; /* mov r4,r2 */
L8c6510e2: *stop=0x8c6510e2u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[5],4);if(io_status)return io_status;goto L8c6510e4; /* mov.l r5,@(0xc,r4) */
L8c6510e4: *stop=0x8c6510e4u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6510e6; /* mov.l r3,@(0x10,r4) */
L8c6510e6: *stop=0x8c6510e6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c6510e8; /* mov.l r4,@r1 */
L8c6510e8: *stop=0x8c6510e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6511f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6510ea; /* mov.l 0x8c6511f8,r3 */
L8c6510ea: *stop=0x8c6510eau;r[14]=0u;goto L8c6510ec; /* mov #0x0,r14 */
L8c6510ec: *stop=0x8c6510ecu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6510ee; /* mov.l r14,@r3 */
L8c6510ee: *stop=0x8c6510eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6511fcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6510f0; /* mov.l 0x8c6511fc,r2 */
L8c6510f0: *stop=0x8c6510f0u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c6510f2; /* mov.l r14,@r2 */
L8c6510f2: *stop=0x8c6510f2u;r[1]+=60u;goto L8c6510f4; /* add #0x3c,r1 */
L8c6510f4: *stop=0x8c6510f4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c6510f6; /* mov.l r14,@r1 */
L8c6510f6: *stop=0x8c6510f6u;r[3]+=28u;goto L8c6510f8; /* add #0x1c,r3 */
L8c6510f8: *stop=0x8c6510f8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6510fa; /* mov.l r14,@r3 */
L8c6510fa: *stop=0x8c6510fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c651200u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6510fc; /* mov.l 0x8c651200,r1 */
L8c6510fc: *stop=0x8c6510fcu;r[2]+=28u;goto L8c6510fe; /* add #0x1c,r2 */
L8c6510fe: target=r[1];*pr=0x8c651102u;*stop=0x8c651100u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651102; /* jsr @r1 */
L8c651102: *stop=0x8c651102u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651204u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651104; /* mov.l 0x8c651204,r3 */
L8c651104: *stop=0x8c651104u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c651106; /* mov.l r14,@r3 */
L8c651106: *stop=0x8c651106u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651208u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651108; /* mov.l 0x8c651208,r2 */
L8c651108: *stop=0x8c651108u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c65110a; /* mov.l r14,@r2 */
L8c65110a: *stop=0x8c65110au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651214u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65110c; /* mov.l 0x8c651214,r5 */
L8c65110c: *stop=0x8c65110cu;r[6]=1u;goto L8c65110e; /* mov #0x1,r6 */
L8c65110e: *stop=0x8c65110eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651210u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c651110; /* mov.l 0x8c651210,r13 */
L8c651110: *stop=0x8c651110u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65120cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c651112; /* mov.l 0x8c65120c,r12 */
L8c651112: target=r[13];*pr=0x8c651116u;*stop=0x8c651114u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651116; /* jsr @r13 */
L8c651116: *stop=0x8c651116u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651218u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651118; /* mov.l 0x8c651218,r3 */
L8c651118: *stop=0x8c651118u;r[0]+=4294967295u;goto L8c65111a; /* add #-0x1,r0 */
L8c65111a: *stop=0x8c65111au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c65111c; /* mov.l r0,@r3 */
L8c65111c: *stop=0x8c65111cu;r[1]=r[3];goto L8c65111e; /* mov r3,r1 */
L8c65111e: *stop=0x8c65111eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651120; /* mov.l @r1,r2 */
L8c651120: *stop=0x8c651120u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65121cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651122; /* mov.l 0x8c65121c,r0 */
L8c651122: *stop=0x8c651122u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c651124; /* mov.l r2,@r0 */
L8c651124: *stop=0x8c651124u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651224u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651126; /* mov.l 0x8c651224,r2 */
L8c651126: *stop=0x8c651126u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651220u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651128; /* mov.l 0x8c651220,r3 */
L8c651128: *stop=0x8c651128u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c65112a; /* mov.l r3,@r2 */
L8c65112a: *stop=0x8c65112au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65122cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65112c; /* mov.l 0x8c65122c,r3 */
L8c65112c: *stop=0x8c65112cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651228u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65112e; /* mov.l 0x8c651228,r4 */
L8c65112e: *stop=0x8c65112eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c651130; /* mov.l r4,@r3 */
L8c651130: *stop=0x8c651130u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651230u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651132; /* mov.l 0x8c651230,r1 */
L8c651132: *stop=0x8c651132u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c651134; /* mov.l r4,@r1 */
L8c651134: *stop=0x8c651134u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651234u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651136; /* mov.l 0x8c651234,r2 */
L8c651136: *stop=0x8c651136u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c651138; /* mov.l r14,@r2 */
L8c651138: *stop=0x8c651138u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651238u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65113a; /* mov.l 0x8c651238,r3 */
L8c65113a: *stop=0x8c65113au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c65113c; /* mov.l r14,@r3 */
L8c65113c: *stop=0x8c65113cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65123cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65113e; /* mov.l 0x8c65123c,r1 */
L8c65113e: *stop=0x8c65113eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c651140; /* mov.l r14,@r1 */
L8c651140: *stop=0x8c651140u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651240u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651142; /* mov.l 0x8c651240,r0 */
L8c651142: *stop=0x8c651142u;r[2]=3u;goto L8c651144; /* mov #0x3,r2 */
L8c651144: *stop=0x8c651144u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c651146; /* mov.l r2,@r0 */
L8c651146: *stop=0x8c651146u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651248u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651148; /* mov.l 0x8c651248,r1 */
L8c651148: *stop=0x8c651148u;r[3]=1u;goto L8c65114a; /* mov #0x1,r3 */
L8c65114a: *stop=0x8c65114au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651244u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65114c; /* mov.l 0x8c651244,r2 */
L8c65114c: target=r[1];*pr=0x8c651150u;*stop=0x8c65114eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651150; /* jsr @r1 */
L8c651150: *stop=0x8c651150u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65124cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c651152; /* mov.l 0x8c65124c,r5 */
L8c651152: *stop=0x8c651152u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6511e2u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c651154; /* mov.w 0x8c6511e2,r6 */
L8c651154: target=r[13];*pr=0x8c651158u;*stop=0x8c651156u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651158; /* jsr @r13 */
L8c651158: *stop=0x8c651158u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651254u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65115a; /* mov.l 0x8c651254,r5 */
L8c65115a: *stop=0x8c65115au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651250u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65115c; /* mov.l 0x8c651250,r6 */
L8c65115c: *stop=0x8c65115cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c65115e; /* mov.l r0,@(0x8,r15) */
L8c65115e: target=r[13];*pr=0x8c651162u;*stop=0x8c651160u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651162; /* jsr @r13 */
L8c651162: *stop=0x8c651162u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65125cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c651164; /* mov.l 0x8c65125c,r5 */
L8c651164: *stop=0x8c651164u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651258u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c651166; /* mov.l 0x8c651258,r6 */
L8c651166: *stop=0x8c651166u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c651168; /* mov.l r0,@(0x4,r15) */
L8c651168: target=r[13];*pr=0x8c65116cu;*stop=0x8c65116au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65116c; /* jsr @r13 */
L8c65116c: *stop=0x8c65116cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651260u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65116e; /* mov.l 0x8c651260,r5 */
L8c65116e: *stop=0x8c65116eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651258u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c651170; /* mov.l 0x8c651258,r6 */
L8c651170: *stop=0x8c651170u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c651172; /* mov.l r0,@r15 */
L8c651172: target=r[13];*pr=0x8c651176u;*stop=0x8c651174u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651176; /* jsr @r13 */
L8c651176: *stop=0x8c651176u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c651178; /* mov.l @(0x4,r15),r6 */
L8c651178: *stop=0x8c651178u;r[5]=r[0];goto L8c65117a; /* mov r0,r5 */
L8c65117a: *stop=0x8c65117au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65117c; /* mov.l @r15,r4 */
L8c65117c: *stop=0x8c65117cu;r[5]>>=8;goto L8c65117e; /* shlr8 r5 */
L8c65117e: *stop=0x8c65117eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c651180; /* mov.l @(0x8,r15),r11 */
L8c651180: *stop=0x8c651180u;r[6]>>=8;goto L8c651182; /* shlr8 r6 */
L8c651182: *stop=0x8c651182u;r[4]>>=8;goto L8c651184; /* shlr8 r4 */
L8c651184: *stop=0x8c651184u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651264u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651186; /* mov.l 0x8c651264,r2 */
L8c651186: *stop=0x8c651186u;r[11]>>=8;goto L8c651188; /* shlr8 r11 */
L8c651188: *stop=0x8c651188u;r[6]>>=2;goto L8c65118a; /* shlr2 r6 */
L8c65118a: *stop=0x8c65118au;r[11]>>=2;goto L8c65118c; /* shlr2 r11 */
L8c65118c: *stop=0x8c65118cu;r[4]>>=2;goto L8c65118e; /* shlr2 r4 */
L8c65118e: *stop=0x8c65118eu;r[5]>>=2;goto L8c651190; /* shlr2 r5 */
L8c651190: *stop=0x8c651190u;*sr=(*sr&~1u)|(r[11]&1u);r[11]>>=1;goto L8c651192; /* shlr r11 */
L8c651192: *stop=0x8c651192u;r[11]<<=8;goto L8c651194; /* shll8 r11 */
L8c651194: *stop=0x8c651194u;r[11]<<=2;goto L8c651196; /* shll2 r11 */
L8c651196: *stop=0x8c651196u;*sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c651198; /* shll r11 */
L8c651198: *stop=0x8c651198u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c65119a; /* shlr r6 */
L8c65119a: *stop=0x8c65119au;r[6]<<=8;goto L8c65119c; /* shll8 r6 */
L8c65119c: *stop=0x8c65119cu;r[6]<<=2;goto L8c65119e; /* shll2 r6 */
L8c65119e: *stop=0x8c65119eu;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c6511a0; /* shll r6 */
L8c6511a0: *stop=0x8c6511a0u;*sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1;goto L8c6511a2; /* shlr r4 */
L8c6511a2: *stop=0x8c6511a2u;r[4]<<=8;goto L8c6511a4; /* shll8 r4 */
L8c6511a4: *stop=0x8c6511a4u;r[4]<<=2;goto L8c6511a6; /* shll2 r4 */
L8c6511a6: *stop=0x8c6511a6u;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6511a8; /* shll r4 */
L8c6511a8: *stop=0x8c6511a8u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c6511aa; /* shlr r5 */
L8c6511aa: *stop=0x8c6511aau;r[5]<<=8;goto L8c6511ac; /* shll8 r5 */
L8c6511ac: *stop=0x8c6511acu;r[5]<<=2;goto L8c6511ae; /* shll2 r5 */
L8c6511ae: *stop=0x8c6511aeu;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6511b0; /* shll r5 */
L8c6511b0: *stop=0x8c6511b0u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c6511b2; /* mov.l r4,@r2 */
L8c6511b2: *stop=0x8c6511b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651268u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6511b4; /* mov.l 0x8c651268,r3 */
L8c6511b4: *stop=0x8c6511b4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c6511b6; /* mov.l r5,@r3 */
L8c6511b6: *stop=0x8c6511b6u;r[7]=r[11];goto L8c6511b8; /* mov r11,r7 */
L8c6511b8: *stop=0x8c6511b8u;r[7]+=r[6];goto L8c6511ba; /* add r6,r7 */
L8c6511ba: *stop=0x8c6511bau;r[4]+=r[7];goto L8c6511bc; /* add r7,r4 */
L8c6511bc: *stop=0x8c6511bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65126cu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c6511be; /* mov.l 0x8c65126c,r8 */
L8c6511be: *stop=0x8c6511beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651270u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c6511c0; /* mov.l 0x8c651270,r10 */
L8c6511c0: *stop=0x8c6511c0u;r[6]=r[14];goto L8c6511c2; /* mov r14,r6 */
L8c6511c2: *stop=0x8c6511c2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c6511c4; /* mov.l r4,@r15 */
L8c6511c4: *stop=0x8c6511c4u;r[4]=r[14];goto L8c6511c6; /* mov r14,r4 */
L8c6511c6: *stop=0x8c6511c6u;r[5]=r[14];goto L8c6511c8; /* mov r14,r5 */
L8c6511c8: *stop=0x8c6511cau;r[9]=32u;goto L8c6512b0; /* bra 0x8c6512b0 */
L8c651280: *stop=0x8c651280u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651420u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651282; /* mov.l 0x8c651420,r0 */
L8c651282: *stop=0x8c651282u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c651284; /* mov.l r5,@(r0,r4) */
L8c651284: *stop=0x8c651284u;r[2]=r[5];goto L8c651286; /* mov r5,r2 */
L8c651286: *stop=0x8c651286u;r[3]=r[0];goto L8c651288; /* mov r0,r3 */
L8c651288: *stop=0x8c651288u;r[2]+=r[11];goto L8c65128a; /* add r11,r2 */
L8c65128a: *stop=0x8c65128au;r[3]+=r[4];goto L8c65128c; /* add r4,r3 */
L8c65128c: *stop=0x8c65128cu;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[2],4);if(io_status)return io_status;goto L8c65128e; /* mov.l r2,@(0x4,r3) */
L8c65128e: *stop=0x8c65128eu;r[2]=r[5];goto L8c651290; /* mov r5,r2 */
L8c651290: *stop=0x8c651290u;r[3]=r[0];goto L8c651292; /* mov r0,r3 */
L8c651292: *stop=0x8c651292u;r[2]+=r[7];goto L8c651294; /* add r7,r2 */
L8c651294: *stop=0x8c651294u;r[3]+=r[4];goto L8c651296; /* add r4,r3 */
L8c651296: *stop=0x8c651296u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[2],4);if(io_status)return io_status;goto L8c651298; /* mov.l r2,@(0x8,r3) */
L8c651298: *stop=0x8c651298u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65129a; /* mov.l @r15,r2 */
L8c65129a: *stop=0x8c65129au;r[3]=r[0];goto L8c65129c; /* mov r0,r3 */
L8c65129c: *stop=0x8c65129cu;r[3]+=r[4];goto L8c65129e; /* add r4,r3 */
L8c65129e: *stop=0x8c65129eu;r[2]+=r[5];goto L8c6512a0; /* add r5,r2 */
L8c6512a0: *stop=0x8c6512a0u;if(!valid_access(r[3]+12u,4u))return -3;wr(m,bus,&io_status,r[3]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6512a2; /* mov.l r2,@(0xc,r3) */
L8c6512a2: *stop=0x8c6512a2u;r[3]=r[8];goto L8c6512a4; /* mov r8,r3 */
L8c6512a4: *stop=0x8c6512a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651424u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6512a6; /* mov.l 0x8c651424,r0 */
L8c6512a6: *stop=0x8c6512a6u;r[3]-=r[5];goto L8c6512a8; /* sub r5,r3 */
L8c6512a8: *stop=0x8c6512a8u;if(!valid_access(r[0]+r[6],4u))return -3;wr(m,bus,&io_status,r[6]+r[0],r[3],4);if(io_status)return io_status;goto L8c6512aa; /* mov.l r3,@(r0,r6) */
L8c6512aa: *stop=0x8c6512aau;r[4]+=16u;goto L8c6512ac; /* add #0x10,r4 */
L8c6512ac: *stop=0x8c6512acu;r[5]+=r[10];goto L8c6512ae; /* add r10,r5 */
L8c6512ae: *stop=0x8c6512aeu;r[6]+=4u;goto L8c6512b0; /* add #0x4,r6 */
L8c6512b0: *stop=0x8c6512b0u;*sr=(*sr&~1u)|(r[4]>=r[9]);goto L8c6512b2; /* cmp/hs r9,r4 */
L8c6512b2: if(!(*sr&1u))goto L8c651280;goto L8c6512b4; /* bf 0x8c651280 */
L8c6512b4: *stop=0x8c6512b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651428u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6512b6; /* mov.l 0x8c651428,r3 */
L8c6512b6: *stop=0x8c6512b6u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6512b8; /* mov.l r14,@r3 */
L8c6512b8: *stop=0x8c6512b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65142cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6512ba; /* mov.l 0x8c65142c,r2 */
L8c6512ba: *stop=0x8c6512bau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c6512bc; /* mov.l r14,@r2 */
L8c6512bc: *stop=0x8c6512bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651430u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6512be; /* mov.l 0x8c651430,r1 */
L8c6512be: *stop=0x8c6512beu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c6512c0; /* mov.l r14,@r1 */
L8c6512c0: *stop=0x8c6512c0u;r[3]+=12u;goto L8c6512c2; /* add #0xc,r3 */
L8c6512c2: *stop=0x8c6512c2u;r[4]=4294967295u;goto L8c6512c4; /* mov #-0x1,r4 */
L8c6512c4: *stop=0x8c6512c4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c6512c6; /* mov.l r4,@r3 */
L8c6512c6: *stop=0x8c6512c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651434u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6512c8; /* mov.l 0x8c651434,r2 */
L8c6512c8: *stop=0x8c6512c8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c6512ca; /* mov.l r4,@r2 */
L8c6512ca: *stop=0x8c6512cau;r[1]+=12u;goto L8c6512cc; /* add #0xc,r1 */
L8c6512cc: *stop=0x8c6512ccu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c6512ce; /* mov.l r4,@r1 */
L8c6512ce: *stop=0x8c6512ceu;r[3]+=12u;goto L8c6512d0; /* add #0xc,r3 */
L8c6512d0: *stop=0x8c6512d0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6512d2; /* mov.l r14,@r3 */
L8c6512d2: *stop=0x8c6512d2u;r[2]+=12u;goto L8c6512d4; /* add #0xc,r2 */
L8c6512d4: *stop=0x8c6512d4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c6512d6; /* mov.l r14,@r2 */
L8c6512d6: *stop=0x8c6512d6u;r[1]+=12u;goto L8c6512d8; /* add #0xc,r1 */
L8c6512d8: *stop=0x8c6512d8u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c6512da; /* mov.l r14,@r1 */
L8c6512da: *stop=0x8c6512dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c651438u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6512dc; /* mov.l 0x8c651438,r3 */
L8c6512dc: *stop=0x8c6512dcu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6512de; /* mov.l r14,@r3 */
L8c6512de: *stop=0x8c6512deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65143cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6512e0; /* mov.l 0x8c65143c,r2 */
L8c6512e0: *stop=0x8c6512e0u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c6512e2; /* mov.l r14,@r2 */
L8c6512e2: *stop=0x8c6512e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651448u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6512e4; /* mov.l 0x8c651448,r5 */
L8c6512e4: *stop=0x8c6512e4u;r[1]=1u;goto L8c6512e6; /* mov #0x1,r1 */
L8c6512e6: *stop=0x8c6512e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65144cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6512e8; /* mov.l 0x8c65144c,r4 */
L8c6512e8: *stop=0x8c6512e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651444u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6512ea; /* mov.l 0x8c651444,r6 */
L8c6512ea: *stop=0x8c6512eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c651440u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6512ec; /* mov.l 0x8c651440,r0 */
L8c6512ec: target=r[13];*pr=0x8c6512f0u;*stop=0x8c6512eeu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6512f0; /* jsr @r13 */
L8c6512f0: *stop=0x8c6512f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651450u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6512f2; /* mov.l 0x8c651450,r3 */
L8c6512f2: *stop=0x8c6512f2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c6512f4; /* mov.l r0,@r3 */
L8c6512f4: *stop=0x8c6512f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65144cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6512f6; /* mov.l 0x8c65144c,r4 */
L8c6512f6: *stop=0x8c6512f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651454u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6512f8; /* mov.l 0x8c651454,r5 */
L8c6512f8: target=r[13];*pr=0x8c6512fcu;*stop=0x8c6512fau;r[6]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6512fc; /* jsr @r13 */
L8c6512fc: *stop=0x8c6512fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651458u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6512fe; /* mov.l 0x8c651458,r2 */
L8c6512fe: *stop=0x8c6512feu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c651300; /* mov.l r0,@r2 */
L8c651300: *stop=0x8c651300u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65145cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c651302; /* mov.l 0x8c65145c,r5 */
L8c651302: *stop=0x8c651302u;r[6]=0u;goto L8c651304; /* mov #0x0,r6 */
L8c651304: target=r[13];*pr=0x8c651308u;*stop=0x8c651306u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651308; /* jsr @r13 */
L8c651308: *stop=0x8c651308u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651460u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65130a; /* mov.l 0x8c651460,r3 */
L8c65130a: *stop=0x8c65130au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c65130c; /* mov.l r0,@r3 */
L8c65130c: *stop=0x8c65130cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651464u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65130e; /* mov.l 0x8c651464,r5 */
L8c65130e: *stop=0x8c65130eu;r[6]=10u;goto L8c651310; /* mov #0xa,r6 */
L8c651310: target=r[13];*pr=0x8c651314u;*stop=0x8c651312u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651314; /* jsr @r13 */
L8c651314: *stop=0x8c651314u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651468u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651316; /* mov.l 0x8c651468,r2 */
L8c651316: *stop=0x8c651316u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c651318; /* mov.l r0,@r2 */
L8c651318: *stop=0x8c651318u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65146cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65131a; /* mov.l 0x8c65146c,r5 */
L8c65131a: *stop=0x8c65131au;r[6]=10u;goto L8c65131c; /* mov #0xa,r6 */
L8c65131c: target=r[13];*pr=0x8c651320u;*stop=0x8c65131eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651320; /* jsr @r13 */
L8c651320: *stop=0x8c651320u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651470u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651322; /* mov.l 0x8c651470,r3 */
L8c651322: *stop=0x8c651322u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c651324; /* mov.l r0,@r3 */
L8c651324: *stop=0x8c651324u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651474u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c651326; /* mov.l 0x8c651474,r5 */
L8c651326: *stop=0x8c651326u;r[6]=10u;goto L8c651328; /* mov #0xa,r6 */
L8c651328: target=r[13];*pr=0x8c65132cu;*stop=0x8c65132au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65132c; /* jsr @r13 */
L8c65132c: *stop=0x8c65132cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651478u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65132e; /* mov.l 0x8c651478,r2 */
L8c65132e: *stop=0x8c65132eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c651330; /* mov.l r0,@r2 */
L8c651330: *stop=0x8c651330u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65147cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651332; /* mov.l 0x8c65147c,r3 */
L8c651332: *stop=0x8c651332u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c651334; /* mov.l r14,@r3 */
L8c651334: *stop=0x8c651334u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651480u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651336; /* mov.l 0x8c651480,r0 */
L8c651336: *stop=0x8c651336u;r[1]=1u;goto L8c651338; /* mov #0x1,r1 */
L8c651338: *stop=0x8c651338u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c65133a; /* mov.l r1,@r0 */
L8c65133a: *stop=0x8c65133au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651484u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65133c; /* mov.l 0x8c651484,r1 */
L8c65133c: *stop=0x8c65133cu;r[2]=15u;goto L8c65133e; /* mov #0xf,r2 */
L8c65133e: *stop=0x8c65133eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c651340; /* mov.l r2,@r1 */
L8c651340: *stop=0x8c651340u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651342; /* mov.l @r1,r3 */
L8c651342: *stop=0x8c651342u;r[3]|=r[14];goto L8c651344; /* or r14,r3 */
L8c651344: *stop=0x8c651344u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c651346; /* mov.l r3,@r1 */
L8c651346: *stop=0x8c651346u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651488u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651348; /* mov.l 0x8c651488,r2 */
L8c651348: *stop=0x8c651348u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c65134a; /* mov.l r14,@r2 */
L8c65134a: *stop=0x8c65134au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65148cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65134c; /* mov.l 0x8c65148c,r3 */
L8c65134c: *stop=0x8c65134cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c65134e; /* mov.l r14,@r3 */
L8c65134e: *stop=0x8c65134eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651490u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651350; /* mov.l 0x8c651490,r3 */
L8c651350: target=r[3];*pr=0x8c651354u;*stop=0x8c651352u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651354; /* jsr @r3 */
L8c651354: *stop=0x8c651354u;r[15]+=12u;goto L8c651356; /* add #0xc,r15 */
L8c651356: *stop=0x8c651356u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c651358; /* lds.l @r15+,PR */
L8c651358: *stop=0x8c651358u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65135a; /* mov.l @r15+,r8 */
L8c65135a: *stop=0x8c65135au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65135c; /* mov.l @r15+,r9 */
L8c65135c: *stop=0x8c65135cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65135e; /* mov.l @r15+,r10 */
L8c65135e: *stop=0x8c65135eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c651360; /* mov.l @r15+,r11 */
L8c651360: *stop=0x8c651360u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c651362; /* mov.l @r15+,r12 */
L8c651362: *stop=0x8c651362u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c651364; /* mov.l @r15+,r13 */
L8c651364: *stop=0x8c651364u;if(*pr!=entry_pr)return -3;*stop=0x8c651366u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c651384(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c651384;
L8c651384: *stop=0x8c651384u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c651386; /* mov.l r14,@-r15 */
L8c651386: *stop=0x8c651386u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c651388; /* sts.l PR,@-r15 */
L8c651388: *stop=0x8c651388u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65149cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65138a; /* mov.l 0x8c65149c,r1 */
L8c65138a: target=r[1];*pr=0x8c65138eu;*stop=0x8c65138cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65138e; /* jsr @r1 */
L8c65138e: *stop=0x8c65138eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651390; /* mov.l 0x8c6514a0,r3 */
L8c651390: target=r[3];*pr=0x8c651394u;*stop=0x8c651392u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651394; /* jsr @r3 */
L8c651394: *stop=0x8c651394u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651396; /* mov.l 0x8c6514a4,r1 */
L8c651396: target=r[1];*pr=0x8c65139au;*stop=0x8c651398u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65139a; /* jsr @r1 */
L8c65139a: *stop=0x8c65139au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65139c; /* mov.l 0x8c6514a8,r3 */
L8c65139c: target=r[3];*pr=0x8c6513a0u;*stop=0x8c65139eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6513a0; /* jsr @r3 */
L8c6513a0: *stop=0x8c6513a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514acu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6513a2; /* mov.l 0x8c6514ac,r1 */
L8c6513a2: target=r[1];*pr=0x8c6513a6u;*stop=0x8c6513a4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6513a6; /* jsr @r1 */
L8c6513a6: *stop=0x8c6513a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6513a8; /* mov.l 0x8c6514b0,r3 */
L8c6513a8: target=r[3];*pr=0x8c6513acu;*stop=0x8c6513aau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6513ac; /* jsr @r3 */
L8c6513ac: *stop=0x8c6513acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514b8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6513ae; /* mov.l 0x8c6514b8,r0 */
L8c6513ae: *stop=0x8c6513aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514b4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6513b0; /* mov.l 0x8c6514b4,r1 */
L8c6513b0: *stop=0x8c6513b0u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c6513b2; /* mov.l r1,@r0 */
L8c6513b2: *stop=0x8c6513b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6513b4; /* mov.l 0x8c6514bc,r3 */
L8c6513b4: *stop=0x8c6513b4u;r[14]=0u;goto L8c6513b6; /* mov #0x0,r14 */
L8c6513b6: *stop=0x8c6513b6u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6513b8; /* mov.l r14,@r3 */
L8c6513b8: *stop=0x8c6513b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514c0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6513ba; /* mov.l 0x8c6514c0,r2 */
L8c6513ba: *stop=0x8c6513bau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c6513bc; /* mov.l r14,@r2 */
L8c6513bc: *stop=0x8c6513bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514c4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6513be; /* mov.l 0x8c6514c4,r1 */
L8c6513be: *stop=0x8c6513beu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c6513c0; /* mov.l r14,@r1 */
L8c6513c0: *stop=0x8c6513c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651484u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6513c2; /* mov.l 0x8c651484,r0 */
L8c6513c2: *stop=0x8c6513c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6513c4; /* mov.l r14,@-r15 */
L8c6513c4: *stop=0x8c6513c4u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6513c6; /* mov.l @r0,r3 */
L8c6513c6: *stop=0x8c6513c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651420u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6513c8; /* mov.l 0x8c651420,r7 */
L8c6513c8: *stop=0x8c6513c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6513ca; /* mov.l r3,@-r15 */
L8c6513ca: *stop=0x8c6513cau;if(!valid_access(r[7]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6513cc; /* mov.l @(0xc,r7),r7 */
L8c6513cc: *stop=0x8c6513ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651420u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6513ce; /* mov.l 0x8c651420,r6 */
L8c6513ce: *stop=0x8c6513ceu;r[3]=4294967289u;goto L8c6513d0; /* mov #-0x7,r3 */
L8c6513d0: *stop=0x8c6513d0u;r[7]=(r[3]&0x80000000u)?((r[3]&31u)?r[7]>>((0u-r[3])&31u):0u):(r[7]<<(r[3]&31u));goto L8c6513d2; /* shld r3,r7 */
L8c6513d2: *stop=0x8c6513d2u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6513d4; /* mov.l @(0x8,r6),r6 */
L8c6513d4: *stop=0x8c6513d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651420u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6513d6; /* mov.l 0x8c651420,r5 */
L8c6513d6: *stop=0x8c6513d6u;r[6]=(r[3]&0x80000000u)?((r[3]&31u)?r[6]>>((0u-r[3])&31u):0u):(r[6]<<(r[3]&31u));goto L8c6513d8; /* shld r3,r6 */
L8c6513d8: *stop=0x8c6513d8u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6513da; /* mov.l @(0x4,r5),r5 */
L8c6513da: *stop=0x8c6513dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c651420u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6513dc; /* mov.l 0x8c651420,r4 */
L8c6513dc: *stop=0x8c6513dcu;r[5]=(r[3]&0x80000000u)?((r[3]&31u)?r[5]>>((0u-r[3])&31u):0u):(r[5]<<(r[3]&31u));goto L8c6513de; /* shld r3,r5 */
L8c6513de: *stop=0x8c6513deu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6513e0; /* mov.l @r4,r4 */
L8c6513e0: *stop=0x8c6513e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6514c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6513e2; /* mov.l 0x8c6514c8,r2 */
L8c6513e2: target=r[2];*pr=0x8c6513e6u;*stop=0x8c6513e4u;r[4]=(r[3]&0x80000000u)?((r[3]&31u)?r[4]>>((0u-r[3])&31u):0u):(r[4]<<(r[3]&31u));if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6513e6; /* jsr @r2 */
L8c6513e6: *stop=0x8c6513e6u;r[15]+=8u;goto L8c6513e8; /* add #0x8,r15 */
L8c6513e8: *stop=0x8c6513e8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6513ea; /* lds.l @r15+,PR */
L8c6513ea: *stop=0x8c6513eau;if(*pr!=entry_pr)return -3;*stop=0x8c6513ecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c641160(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c641160;
L8c641160: *stop=0x8c641160u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641200u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641162; /* mov.l 0x8c641200,r3 */
L8c641162: *stop=0x8c641162u;r[4]=0u;goto L8c641164; /* mov #0x0,r4 */
L8c641164: *stop=0x8c641164u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c641166; /* mov.l r4,@r3 */
L8c641166: *stop=0x8c641166u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641208u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641168; /* mov.l 0x8c641208,r1 */
L8c641168: *stop=0x8c641168u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641204u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64116a; /* mov.l 0x8c641204,r2 */
L8c64116a: *stop=0x8c64116au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c64116c; /* mov.l r1,@r2 */
L8c64116c: *stop=0x8c64116cu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[4],4);if(io_status)return io_status;goto L8c64116e; /* mov.l r4,@(0x4,r2) */
L8c64116e: *stop=0x8c64116eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64120cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641170; /* mov.l 0x8c64120c,r1 */
L8c641170: *stop=0x8c641170u;r[3]+=4294967188u;goto L8c641172; /* add #-0x6c,r3 */
L8c641172: *stop=0x8c641172u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c641174; /* mov.l r1,@r3 */
L8c641174: *stop=0x8c641174u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[4],4);if(io_status)return io_status;goto L8c641176; /* mov.l r4,@(0x4,r3) */
L8c641176: *stop=0x8c641176u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641210u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641178; /* mov.l 0x8c641210,r1 */
L8c641178: *stop=0x8c641178u;r[2]+=16u;goto L8c64117a; /* add #0x10,r2 */
L8c64117a: *stop=0x8c64117au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c64117c; /* mov.l r1,@r2 */
L8c64117c: *stop=0x8c64117cu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[4],4);if(io_status)return io_status;goto L8c64117e; /* mov.l r4,@(0x4,r2) */
L8c64117e: *stop=0x8c64117eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641214u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641180; /* mov.l 0x8c641214,r1 */
L8c641180: *stop=0x8c641180u;r[3]+=16u;goto L8c641182; /* add #0x10,r3 */
L8c641182: *stop=0x8c641182u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c641184; /* mov.l r1,@r3 */
L8c641184: *stop=0x8c641184u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[4],4);if(io_status)return io_status;goto L8c641186; /* mov.l r4,@(0x4,r3) */
L8c641186: *stop=0x8c641186u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641218u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641188; /* mov.l 0x8c641218,r1 */
L8c641188: *stop=0x8c641188u;r[2]+=16u;goto L8c64118a; /* add #0x10,r2 */
L8c64118a: *stop=0x8c64118au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c64118c; /* mov.l r1,@r2 */
L8c64118c: *stop=0x8c64118cu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[4],4);if(io_status)return io_status;goto L8c64118e; /* mov.l r4,@(0x4,r2) */
L8c64118e: *stop=0x8c64118eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64121cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641190; /* mov.l 0x8c64121c,r1 */
L8c641190: *stop=0x8c641190u;r[3]+=16u;goto L8c641192; /* add #0x10,r3 */
L8c641192: *stop=0x8c641192u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c641194; /* mov.l r1,@r3 */
L8c641194: *stop=0x8c641194u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[4],4);if(io_status)return io_status;goto L8c641196; /* mov.l r4,@(0x4,r3) */
L8c641196: *stop=0x8c641196u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641220u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641198; /* mov.l 0x8c641220,r1 */
L8c641198: *stop=0x8c641198u;r[2]+=16u;goto L8c64119a; /* add #0x10,r2 */
L8c64119a: *stop=0x8c64119au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c64119c; /* mov.l r1,@r2 */
L8c64119c: *stop=0x8c64119cu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[4],4);if(io_status)return io_status;goto L8c64119e; /* mov.l r4,@(0x4,r2) */
L8c64119e: *stop=0x8c64119eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641224u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6411a0; /* mov.l 0x8c641224,r1 */
L8c6411a0: *stop=0x8c6411a0u;r[3]+=16u;goto L8c6411a2; /* add #0x10,r3 */
L8c6411a2: *stop=0x8c6411a2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c6411a4; /* mov.l r1,@r3 */
L8c6411a4: *stop=0x8c6411a4u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[4],4);if(io_status)return io_status;goto L8c6411a6; /* mov.l r4,@(0x4,r3) */
L8c6411a6: *stop=0x8c6411a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641228u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6411a8; /* mov.l 0x8c641228,r1 */
L8c6411a8: *stop=0x8c6411a8u;r[2]+=16u;goto L8c6411aa; /* add #0x10,r2 */
L8c6411aa: *stop=0x8c6411aau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6411ac; /* mov.l r1,@r2 */
L8c6411ac: *stop=0x8c6411acu;if(*pr!=entry_pr)return -3;*stop=0x8c6411aeu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[4],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c66120c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c66120c;
L8c66120c: *stop=0x8c66120cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66120e; /* mov.l 0x8c6612a0,r2 */
L8c66120e: *stop=0x8c66120eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c661298u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c661210; /* mov.l 0x8c661298,r3 */
L8c661210: *stop=0x8c661210u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c661212; /* mov.l r2,@r3 */
L8c661212: *stop=0x8c661212u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c661214; /* mov.l 0x8c6612a8,r2 */
L8c661214: *stop=0x8c661214u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c661216; /* mov.l 0x8c6612a4,r1 */
L8c661216: *stop=0x8c661216u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c661218; /* mov.l r2,@r1 */
L8c661218: *stop=0x8c661218u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612acu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c66121a; /* mov.l 0x8c6612ac,r0 */
L8c66121a: *stop=0x8c66121au;r[3]=0u;goto L8c66121c; /* mov #0x0,r3 */
L8c66121c: *stop=0x8c66121cu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c66121e; /* mov.l r3,@r0 */
L8c66121e: *stop=0x8c66121eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c661220; /* mov.l 0x8c6612b4,r2 */
L8c661220: *stop=0x8c661220u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612b0u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c661222; /* mov.l 0x8c6612b0,r7 */
L8c661222: *stop=0x8c661222u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[7],4);if(io_status)return io_status;goto L8c661224; /* mov.l r7,@r2 */
L8c661224: *stop=0x8c661224u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c661226; /* mov.l 0x8c6612bc,r3 */
L8c661226: *stop=0x8c661226u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612b8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c661228; /* mov.l 0x8c6612b8,r6 */
L8c661228: *stop=0x8c661228u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c66122a; /* mov.l r6,@r3 */
L8c66122a: *stop=0x8c66122au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612c0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c66122c; /* mov.l 0x8c6612c0,r5 */
L8c66122c: *stop=0x8c66122cu;r[1]+=16u;goto L8c66122e; /* add #0x10,r1 */
L8c66122e: *stop=0x8c66122eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c661230; /* mov.l r5,@r1 */
L8c661230: *stop=0x8c661230u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612c4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c661232; /* mov.l 0x8c6612c4,r4 */
L8c661232: *stop=0x8c661232u;r[2]+=12u;goto L8c661234; /* add #0xc,r2 */
L8c661234: *stop=0x8c661234u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c661236; /* mov.l r4,@r2 */
L8c661236: *stop=0x8c661236u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612c8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c661238; /* mov.l 0x8c6612c8,r1 */
L8c661238: *stop=0x8c661238u;r[3]+=12u;goto L8c66123a; /* add #0xc,r3 */
L8c66123a: *stop=0x8c66123au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c66123c; /* mov.l r1,@r3 */
L8c66123c: *stop=0x8c66123cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612ccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c66123e; /* mov.l 0x8c6612cc,r0 */
L8c66123e: *stop=0x8c66123eu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[7],4);if(io_status)return io_status;goto L8c661240; /* mov.l r7,@r0 */
L8c661240: *stop=0x8c661240u;r[2]+=12u;goto L8c661242; /* add #0xc,r2 */
L8c661242: *stop=0x8c661242u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c661244; /* mov.l r5,@r2 */
L8c661244: *stop=0x8c661244u;r[3]+=12u;goto L8c661246; /* add #0xc,r3 */
L8c661246: *stop=0x8c661246u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c661248; /* mov.l r4,@r3 */
L8c661248: *stop=0x8c661248u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66124a; /* mov.l 0x8c6612d4,r2 */
L8c66124a: *stop=0x8c66124au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612d0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66124c; /* mov.l 0x8c6612d0,r1 */
L8c66124c: *stop=0x8c66124cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c66124e; /* mov.l r2,@r1 */
L8c66124e: *stop=0x8c66124eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612d8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c661250; /* mov.l 0x8c6612d8,r0 */
L8c661250: *stop=0x8c661250u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[6],4);if(io_status)return io_status;goto L8c661252; /* mov.l r6,@r0 */
L8c661252: *stop=0x8c661252u;r[3]+=12u;goto L8c661254; /* add #0xc,r3 */
L8c661254: *stop=0x8c661254u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c661256; /* mov.l r5,@r3 */
L8c661256: *stop=0x8c661256u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c661258; /* mov.l 0x8c6612dc,r2 */
L8c661258: *stop=0x8c661258u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c66125a; /* mov.l r4,@r2 */
L8c66125a: *stop=0x8c66125au;r[1]+=4294967272u;goto L8c66125c; /* add #-0x18,r1 */
L8c66125c: *stop=0x8c66125cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66125e; /* mov.l @r1,r3 */
L8c66125e: *stop=0x8c66125eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6612e0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c661260; /* mov.l 0x8c6612e0,r0 */
L8c661260: *stop=0x8c661260u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c661262; /* mov.l r3,@r0 */
L8c661262: *stop=0x8c661262u;{uint32_t loaded=rd(m,bus,&io_status,0x8c661280u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c661264; /* mov.l 0x8c661280,r3 */
L8c661264: *stop=0x8c661264u;{uint32_t loaded=rd(m,bus,&io_status,0x8c661290u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c661266; /* mov.l 0x8c661290,r2 */
L8c661266: *stop=0x8c661266u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c661268; /* mov.l r2,@r3 */
L8c661268: *stop=0x8c661268u;if(*pr!=entry_pr)return -3;*stop=0x8c66126au;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c64241a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64241a;
L8c64241a: *stop=0x8c64241au;if(*pr!=entry_pr)return -3;*stop=0x8c64241cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c653440(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c653440;
L8c653440: *stop=0x8c653440u;if(*pr!=entry_pr)return -3;*stop=0x8c653442u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c6656e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 unsigned driver_stall_visits=0,driver_clear_visits=0;(void)driver_stall_visits;(void)driver_clear_visits;
 int take;
 goto L8c6656e2;
L8c6656e2: *stop=0x8c6656e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6656e4; /* mov.l r14,@-r15 */
L8c6656e4: *stop=0x8c6656e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6656e6; /* mov.l r13,@-r15 */
L8c6656e6: *stop=0x8c6656e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6656e8; /* mov.l r12,@-r15 */
L8c6656e8: *stop=0x8c6656e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6656ea; /* mov.l r11,@-r15 */
L8c6656ea: *stop=0x8c6656eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6656ec; /* sts.l PR,@-r15 */
L8c6656ec: *stop=0x8c6656ecu;r[15]+=4294967288u;goto L8c6656ee; /* add #-0x8,r15 */
L8c6656ee: *stop=0x8c6656eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c665748u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6656f0; /* mov.l 0x8c665748,r3 */
L8c6656f0: *stop=0x8c6656f0u;r[12]=0u;goto L8c6656f2; /* mov #0x0,r12 */
L8c6656f2: *stop=0x8c6656f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665744u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6656f4; /* mov.l 0x8c665744,r11 */
L8c6656f4: *stop=0x8c6656f4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8c6656f6; /* mov.l r12,@r3 */
L8c6656f6: *stop=0x8c6656f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66574cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6656f8; /* mov.l 0x8c66574c,r2 */
L8c6656f8: target=r[2];*pr=0x8c6656fcu;*stop=0x8c6656fau;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6656fc; /* jsr @r2 */
L8c6656fc: *stop=0x8c6656fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66574cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6656fe; /* mov.l 0x8c66574c,r3 */
L8c6656fe: *stop=0x8c6656feu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c665700; /* mov.l r0,@(0x4,r15) */
L8c665700: target=r[3];*pr=0x8c665704u;*stop=0x8c665702u;r[4]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665704; /* jsr @r3 */
L8c665704: *stop=0x8c665704u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665750u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665706; /* mov.l 0x8c665750,r2 */
L8c665706: *stop=0x8c665706u;r[4]=5u;goto L8c665708; /* mov #0x5,r4 */
L8c665708: *stop=0x8c665708u;r[5]=r[0];goto L8c66570a; /* mov r0,r5 */
L8c66570a: *stop=0x8c66570au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c66570c; /* mov.l r4,@r2 */
L8c66570c: *stop=0x8c66570cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66570e; /* mov.l @(0x4,r15),r1 */
L8c66570e: *stop=0x8c66570eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c665754u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665710; /* mov.l 0x8c665754,r3 */
L8c665710: *stop=0x8c665710u;*sr=(*sr&~1u)|(r[3]==r[1]);goto L8c665712; /* cmp/eq r3,r1 */
L8c665712: if((*sr&1u))goto L8c66571a;goto L8c665714; /* bt 0x8c66571a */
L8c665714: *stop=0x8c665714u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665750u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665716; /* mov.l 0x8c665750,r1 */
L8c665716: *stop=0x8c665718u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c665774; /* bra 0x8c665774 */
L8c66571a: *stop=0x8c66571au;r[0]=r[5];goto L8c66571c; /* mov r5,r0 */
L8c66571c: *stop=0x8c66571cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c66571e; /* cmp/eq #0x1,r0 */
L8c66571e: if(!(*sr&1u))goto L8c665760;goto L8c665720; /* bf 0x8c665760 */
L8c665720: *stop=0x8c665720u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665750u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665722; /* mov.l 0x8c665750,r0 */
L8c665722: *stop=0x8c665724u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;goto L8c665774; /* bra 0x8c665774 */
L8c665760: *stop=0x8c665760u;r[1]=17u;goto L8c665762; /* mov #0x11,r1 */
L8c665762: *stop=0x8c665762u;*sr=(*sr&~1u)|(r[5]>=r[1]);goto L8c665764; /* cmp/hs r1,r5 */
L8c665764: if(!(*sr&1u))goto L8c665774;goto L8c665766; /* bf 0x8c665774 */
L8c665766: *stop=0x8c665766u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6657c4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665768; /* mov.l 0x8c6657c4,r2 */
L8c665768: *stop=0x8c665768u;r[0]=6u;goto L8c66576a; /* mov #0x6,r0 */
L8c66576a: *stop=0x8c66576au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c66576c; /* mov.l r0,@r2 */
L8c66576c: *stop=0x8c66576cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6657ccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c66576e; /* mov.l 0x8c6657cc,r0 */
L8c66576e: *stop=0x8c66576eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6657c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665770; /* mov.l 0x8c6657c8,r3 */
L8c665770: *stop=0x8c665770u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c665772; /* mov.l r3,@r0 */
L8c665772: *stop=0x8c665772u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6657d0u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c665774; /* mov.l 0x8c6657d0,r11 */
L8c665774: *stop=0x8c665774u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6657d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665776; /* mov.l 0x8c6657d4,r3 */
L8c665776: *stop=0x8c665776u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6657c0u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665778; /* mov.w 0x8c6657c0,r4 */
L8c665778: *stop=0x8c665778u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c66577a; /* mov.l @r3,r5 */
L8c66577a: *stop=0x8c66577au;r[0]=r[5];goto L8c66577c; /* mov r5,r0 */
L8c66577c: *stop=0x8c66577cu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c66577e; /* cmp/eq #0x0,r0 */
L8c66577e: take=(*sr&1u);*stop=0x8c665780u;r[6]=15u;if(take)goto L8c6657a2;goto L8c665782; /* bt/s 0x8c6657a2 */
L8c665782: *stop=0x8c665782u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c665784; /* cmp/eq #0x1,r0 */
L8c665784: if((*sr&1u))goto L8c6657a8;goto L8c665786; /* bt 0x8c6657a8 */
L8c665786: *stop=0x8c665786u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c665788; /* cmp/eq #0x2,r0 */
L8c665788: if((*sr&1u))goto L8c6657e0;goto L8c66578a; /* bt 0x8c6657e0 */
L8c66578a: *stop=0x8c66578au;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c66578c; /* cmp/eq #0x4,r0 */
L8c66578c: if((*sr&1u))goto L8c6657ea;goto L8c66578e; /* bt 0x8c6657ea */
L8c66578e: *stop=0x8c66578eu;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c665790; /* cmp/eq #0x5,r0 */
L8c665790: if((*sr&1u))goto L8c6657ea;goto L8c665792; /* bt 0x8c6657ea */
L8c665792: *stop=0x8c665792u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c665794; /* cmp/eq #0x6,r0 */
L8c665794: if((*sr&1u))goto L8c6657ea;goto L8c665796; /* bt 0x8c6657ea */
L8c665796: *stop=0x8c665796u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c665798; /* cmp/eq #0x8,r0 */
L8c665798: if((*sr&1u))goto L8c6657f6;goto L8c66579a; /* bt 0x8c6657f6 */
L8c66579a: *stop=0x8c66579au;*sr=(*sr&~1u)|(r[0]==0x00000009u);goto L8c66579c; /* cmp/eq #0x9,r0 */
L8c66579c: if((*sr&1u))goto L8c665800;goto L8c66579e; /* bt 0x8c665800 */
L8c66579e: *stop=0x8c6657a0u;goto L8c665840; /* bra 0x8c665840 */
L8c6657a2: *stop=0x8c6657a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6657d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6657a4; /* mov.l 0x8c6657d8,r2 */
L8c6657a4: *stop=0x8c6657a6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[12],4);if(io_status)return io_status;goto L8c6657ae; /* bra 0x8c6657ae */
L8c6657a8: *stop=0x8c6657a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6657d8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6657aa; /* mov.l 0x8c6657d8,r1 */
L8c6657aa: *stop=0x8c6657aau;r[2]=1u;goto L8c6657ac; /* mov #0x1,r2 */
L8c6657ac: *stop=0x8c6657acu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c6657ae; /* mov.l r2,@r1 */
L8c6657ae: *stop=0x8c6657b0u;r[13]=r[12];goto L8c665846; /* bra 0x8c665846 */
L8c6657e0: *stop=0x8c6657e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665820u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6657e2; /* mov.l 0x8c665820,r2 */
L8c6657e2: *stop=0x8c6657e2u;r[1]=3u;goto L8c6657e4; /* mov #0x3,r1 */
L8c6657e4: *stop=0x8c6657e4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6657e6; /* mov.l r1,@r2 */
L8c6657e6: *stop=0x8c6657e8u;goto L8c665844; /* bra 0x8c665844 */
L8c6657ea: *stop=0x8c6657eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c665820u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6657ec; /* mov.l 0x8c665820,r1 */
L8c6657ec: *stop=0x8c6657ecu;r[2]=127u;goto L8c6657ee; /* mov #0x7f,r2 */
L8c6657ee: *stop=0x8c6657eeu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c6657f0; /* mov.l r2,@r1 */
L8c6657f0: *stop=0x8c6657f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665824u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c6657f2; /* mov.l 0x8c665824,r13 */
L8c6657f2: *stop=0x8c6657f4u;goto L8c665846; /* bra 0x8c665846 */
L8c6657f6: *stop=0x8c6657f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665820u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6657f8; /* mov.l 0x8c665820,r2 */
L8c6657f8: *stop=0x8c6657f8u;r[1]=31u;goto L8c6657fa; /* mov #0x1f,r1 */
L8c6657fa: *stop=0x8c6657fau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6657fc; /* mov.l r1,@r2 */
L8c6657fc: *stop=0x8c6657feu;goto L8c665844; /* bra 0x8c665844 */
L8c665800: *stop=0x8c665800u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665820u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665802; /* mov.l 0x8c665820,r2 */
L8c665802: *stop=0x8c665804u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[6],4);if(io_status)return io_status;goto L8c665844; /* bra 0x8c665844 */
L8c665840: *stop=0x8c665840u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665842; /* mov.l 0x8c6658cc,r2 */
L8c665842: *stop=0x8c665842u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[6],4);if(io_status)return io_status;goto L8c665844; /* mov.l r6,@r2 */
L8c665844: *stop=0x8c665844u;r[13]=r[4];goto L8c665846; /* mov r4,r13 */
L8c665846: *stop=0x8c665846u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6658c0u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665848; /* mov.w 0x8c6658c0,r4 */
L8c665848: *stop=0x8c665848u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658d0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c66584a; /* mov.l 0x8c6658d0,r14 */
L8c66584a: target=r[14];*pr=0x8c66584eu;*stop=0x8c66584cu;r[5]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66584e; /* jsr @r14 */
L8c66584e: *stop=0x8c66584eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6658c2u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665850; /* mov.w 0x8c6658c2,r4 */
L8c665850: target=r[14];*pr=0x8c665854u;*stop=0x8c665852u;r[5]=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665854; /* jsr @r14 */
L8c665854: *stop=0x8c665854u;r[5]=0u;goto L8c665856; /* mov #0x0,r5 */
L8c665856: target=r[14];*pr=0x8c66585au;*stop=0x8c665858u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66585a; /* jsr @r14 */
L8c66585a: *stop=0x8c66585au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6658c4u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c66585c; /* mov.w 0x8c6658c4,r4 */
L8c66585c: target=r[14];*pr=0x8c665860u;*stop=0x8c66585eu;r[5]=3u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665860; /* jsr @r14 */
L8c665860: *stop=0x8c665860u;r[5]=9u;goto L8c665862; /* mov #0x9,r5 */
L8c665862: target=r[14];*pr=0x8c665866u;*stop=0x8c665864u;r[4]=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665866; /* jsr @r14 */
L8c665866: *stop=0x8c665866u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665868; /* mov.l 0x8c6658d4,r3 */
L8c665868: *stop=0x8c665868u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66586a; /* mov.l @r3,r0 */
L8c66586a: *stop=0x8c66586au;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c66586c; /* tst #0x1,r0 */
L8c66586c: if((*sr&1u))goto L8c66587a;goto L8c66586e; /* bt 0x8c66587a */
L8c66586e: *stop=0x8c66586eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658dcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665870; /* mov.l 0x8c6658dc,r1 */
L8c665870: *stop=0x8c665870u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658d8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665872; /* mov.l 0x8c6658d8,r5 */
L8c665872: target=r[1];*pr=0x8c665876u;*stop=0x8c665874u;r[4]=68u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665876; /* jsr @r1 */
L8c665876: *stop=0x8c665878u;goto L8c665882; /* bra 0x8c665882 */
L8c66587a: *stop=0x8c66587au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66587c; /* mov.l 0x8c6658dc,r2 */
L8c66587c: *stop=0x8c66587cu;r[5]=0u;goto L8c66587e; /* mov #0x0,r5 */
L8c66587e: target=r[2];*pr=0x8c665882u;*stop=0x8c665880u;r[4]=68u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665882; /* jsr @r2 */
L8c665882: *stop=0x8c665882u;r[5]=0u;goto L8c665884; /* mov #0x0,r5 */
L8c665884: target=r[14];*pr=0x8c665888u;*stop=0x8c665886u;r[4]=92u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665888; /* jsr @r14 */
L8c665888: *stop=0x8c665888u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658e0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c66588a; /* mov.l 0x8c6658e0,r5 */
L8c66588a: target=r[14];*pr=0x8c66588eu;*stop=0x8c66588cu;r[4]=104u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66588e; /* jsr @r14 */
L8c66588e: *stop=0x8c66588eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658e4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665890; /* mov.l 0x8c6658e4,r5 */
L8c665890: target=r[14];*pr=0x8c665894u;*stop=0x8c665892u;r[4]=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665894; /* jsr @r14 */
L8c665894: *stop=0x8c665894u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665896; /* mov.l 0x8c6658e8,r3 */
L8c665896: *stop=0x8c665896u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665898; /* mov.l @r3,r0 */
L8c665898: *stop=0x8c665898u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c66589a; /* cmp/eq #0x6,r0 */
L8c66589a: if(!(*sr&1u))goto L8c665900;goto L8c66589c; /* bf 0x8c665900 */
L8c66589c: *stop=0x8c66589cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6658c6u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c66589e; /* mov.w 0x8c6658c6,r4 */
L8c66589e: *stop=0x8c66589eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658ecu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6658a0; /* mov.l 0x8c6658ec,r5 */
L8c6658a0: target=r[14];*pr=0x8c6658a4u;*stop=0x8c6658a2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6658a4; /* jsr @r14 */
L8c6658a4: *stop=0x8c6658a4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6658c8u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c6658a6; /* mov.w 0x8c6658c8,r4 */
L8c6658a6: *stop=0x8c6658a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6658f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6658a8; /* mov.l 0x8c6658f0,r3 */
L8c6658a8: *stop=0x8c6658aau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c665904; /* bra 0x8c665904 */
L8c665900: *stop=0x8c665900u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6659c0u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665902; /* mov.w 0x8c6659c0,r4 */
L8c665902: *stop=0x8c665902u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659d4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665904; /* mov.l 0x8c6659d4,r5 */
L8c665904: target=r[14];*pr=0x8c665908u;*stop=0x8c665906u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665908; /* jsr @r14 */
L8c665908: *stop=0x8c665908u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6659c2u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c66590a; /* mov.w 0x8c6659c2,r5 */
L8c66590a: target=r[14];*pr=0x8c66590eu;*stop=0x8c66590cu;r[4]=76u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66590e; /* jsr @r14 */
L8c66590e: *stop=0x8c66590eu;r[5]=0u;goto L8c665910; /* mov #0x0,r5 */
L8c665910: target=r[14];*pr=0x8c665914u;*stop=0x8c665912u;r[4]=116u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665914; /* jsr @r14 */
L8c665914: *stop=0x8c665914u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659d8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665916; /* mov.l 0x8c6659d8,r5 */
L8c665916: target=r[14];*pr=0x8c66591au;*stop=0x8c665918u;r[4]=120u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66591a; /* jsr @r14 */
L8c66591a: *stop=0x8c66591au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6659c4u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c66591c; /* mov.w 0x8c6659c4,r4 */
L8c66591c: target=r[14];*pr=0x8c665920u;*stop=0x8c66591eu;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665920; /* jsr @r14 */
L8c665920: *stop=0x8c665920u;r[5]=r[13];goto L8c665922; /* mov r13,r5 */
L8c665922: target=r[14];*pr=0x8c665926u;*stop=0x8c665924u;r[4]=48u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665926; /* jsr @r14 */
L8c665926: *stop=0x8c665926u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6659c6u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665928; /* mov.w 0x8c6659c6,r4 */
L8c665928: *stop=0x8c665928u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659dcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c66592a; /* mov.l 0x8c6659dc,r5 */
L8c66592a: target=r[14];*pr=0x8c66592eu;*stop=0x8c66592cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66592e; /* jsr @r14 */
L8c66592e: *stop=0x8c66592eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6659c8u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665930; /* mov.w 0x8c6659c8,r4 */
L8c665930: *stop=0x8c665930u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659dcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665932; /* mov.l 0x8c6659dc,r5 */
L8c665932: target=r[14];*pr=0x8c665936u;*stop=0x8c665934u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665936; /* jsr @r14 */
L8c665936: *stop=0x8c665936u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6659cau,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665938; /* mov.w 0x8c6659ca,r4 */
L8c665938: target=r[14];*pr=0x8c66593cu;*stop=0x8c66593au;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66593c; /* jsr @r14 */
L8c66593c: *stop=0x8c66593cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6659ccu,2);if(io_status)return io_status;r[4]=loaded;}goto L8c66593e; /* mov.w 0x8c6659cc,r4 */
L8c66593e: target=r[14];*pr=0x8c665942u;*stop=0x8c665940u;r[5]=4294967295u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665942; /* jsr @r14 */
L8c665942: *stop=0x8c665942u;r[5]=7u;goto L8c665944; /* mov #0x7,r5 */
L8c665944: *stop=0x8c665944u;r[4]=r[5];goto L8c665946; /* mov r5,r4 */
L8c665946: target=r[14];*pr=0x8c66594au;*stop=0x8c665948u;r[4]+=121u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66594a; /* jsr @r14 */
L8c66594a: *stop=0x8c66594au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6659ceu,2);if(io_status)return io_status;r[4]=loaded;}goto L8c66594c; /* mov.w 0x8c6659ce,r4 */
L8c66594c: *stop=0x8c66594cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659e0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c66594e; /* mov.l 0x8c6659e0,r5 */
L8c66594e: target=r[14];*pr=0x8c665952u;*stop=0x8c665950u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665952; /* jsr @r14 */
L8c665952: *stop=0x8c665952u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6659d0u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665954; /* mov.w 0x8c6659d0,r4 */
L8c665954: *stop=0x8c665954u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659e4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665956; /* mov.l 0x8c6659e4,r5 */
L8c665956: target=r[14];*pr=0x8c66595au;*stop=0x8c665958u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66595a; /* jsr @r14 */
L8c66595a: *stop=0x8c66595au;r[5]=1u;goto L8c66595c; /* mov #0x1,r5 */
L8c66595c: target=r[14];*pr=0x8c665960u;*stop=0x8c66595eu;r[4]=116u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665960; /* jsr @r14 */
L8c665960: *stop=0x8c665960u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665962; /* mov.l 0x8c6659e8,r3 */
L8c665962: *stop=0x8c665962u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665964; /* mov.l @r3,r0 */
L8c665964: *stop=0x8c665964u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c665966; /* cmp/eq #0x6,r0 */
L8c665966: if((*sr&1u))goto L8c66596c;goto L8c665968; /* bt 0x8c66596c */
L8c665968:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c665968u;if(++driver_stall_visits>=2u)return -4;
#endif
 *stop=0x8c66596au;goto L8c665968; /* bra 0x8c665968 */
L8c66596c: *stop=0x8c66596cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66596e; /* mov.l 0x8c6659e8,r2 */
L8c66596e: *stop=0x8c66596eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665970; /* mov.l @r2,r0 */
L8c665970: *stop=0x8c665970u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c665972; /* cmp/eq #0x6,r0 */
L8c665972: take=!(*sr&1u);*stop=0x8c665974u;r[4]=124u;if(take)goto L8c66597c;goto L8c665976; /* bf/s 0x8c66597c */
L8c665976: *stop=0x8c665976u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659ecu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665978; /* mov.l 0x8c6659ec,r5 */
L8c665978: *stop=0x8c66597au;goto L8c66597e; /* bra 0x8c66597e */
L8c66597c: *stop=0x8c66597cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659f0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c66597e; /* mov.l 0x8c6659f0,r5 */
L8c66597e: target=r[14];*pr=0x8c665982u;*stop=0x8c665980u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665982; /* jsr @r14 */
L8c665982: *stop=0x8c665982u;r[13]=r[12];goto L8c665984; /* mov r12,r13 */
L8c665984: *stop=0x8c665984u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c665986; /* mov.l r12,@r15 */
L8c665986: *stop=0x8c665986u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659fcu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c665988; /* mov.l 0x8c6659fc,r14 */
L8c665988: *stop=0x8c665988u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659f8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c66598a; /* mov.l 0x8c6659f8,r12 */
L8c66598a: *stop=0x8c66598au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6659f4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c66598c; /* mov.l 0x8c6659f4,r11 */
L8c66598c: *stop=0x8c66598cu;r[5]=r[15];goto L8c66598e; /* mov r15,r5 */
L8c66598e: *stop=0x8c66598eu;r[6]=4u;goto L8c665990; /* mov #0x4,r6 */
L8c665990: target=r[11];*pr=0x8c665994u;*stop=0x8c665992u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665994; /* jsr @r11 */
L8c665994: *stop=0x8c665994u;r[13]+=1u;goto L8c665996; /* add #0x1,r13 */
L8c665996: *stop=0x8c665996u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[12]);goto L8c665998; /* cmp/ge r12,r13 */
L8c665998:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c665998u;if(!(*sr&1u) && ++driver_clear_visits>=2u)return -4;
#endif
 take=!(*sr&1u);*stop=0x8c66599au;r[14]+=4u;if(take)goto L8c66598c;goto L8c66599c; /* bf/s 0x8c66598c */
L8c66599c: *stop=0x8c66599cu;r[0]=1u;goto L8c66599e; /* mov #0x1,r0 */
L8c66599e: *stop=0x8c66599eu;r[15]+=8u;goto L8c6659a0; /* add #0x8,r15 */
L8c6659a0: *stop=0x8c6659a0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6659a2; /* lds.l @r15+,PR */
L8c6659a2: *stop=0x8c6659a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6659a4; /* mov.l @r15+,r11 */
L8c6659a4: *stop=0x8c6659a4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6659a6; /* mov.l @r15+,r12 */
L8c6659a6: *stop=0x8c6659a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6659a8; /* mov.l @r15+,r13 */
L8c6659a8: *stop=0x8c6659a8u;if(*pr!=entry_pr)return -3;*stop=0x8c6659aau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c6666c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6666c0;
L8c6666c0: *stop=0x8c6666c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6666c2; /* sts.l PR,@-r15 */
L8c6666c2: *stop=0x8c6666c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666880u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6666c4; /* mov.l 0x8c666880,r3 */
L8c6666c4: *stop=0x8c6666c4u;r[5]=1u;goto L8c6666c6; /* mov #0x1,r5 */
L8c6666c6: target=r[3];*pr=0x8c6666cau;*stop=0x8c6666c8u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6666ca; /* jsr @r3 */
L8c6666ca: *stop=0x8c6666cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c666880u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6666cc; /* mov.l 0x8c666880,r2 */
L8c6666cc: *stop=0x8c6666ccu;r[5]=0u;goto L8c6666ce; /* mov #0x0,r5 */
L8c6666ce: target=r[2];*pr=0x8c6666d2u;*stop=0x8c6666d0u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6666d2; /* jsr @r2 */
L8c6666d2: *stop=0x8c6666d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666884u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6666d4; /* mov.l 0x8c666884,r3 */
L8c6666d4: target=r[3];*pr=0x8c6666d8u;*stop=0x8c6666d6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6666d8; /* jsr @r3 */
L8c6666d8: *stop=0x8c6666d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66688cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6666da; /* mov.l 0x8c66688c,r3 */
L8c6666da: *stop=0x8c6666dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c666888u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6666dc; /* mov.l 0x8c666888,r2 */
L8c6666dc: *stop=0x8c6666dcu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6666de; /* mov.l r2,@r3 */
L8c6666de: *stop=0x8c6666deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666894u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6666e0; /* mov.l 0x8c666894,r0 */
L8c6666e0: *stop=0x8c6666e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666890u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6666e2; /* mov.l 0x8c666890,r1 */
L8c6666e2: *stop=0x8c6666e2u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c6666e4; /* mov.l r1,@r0 */
L8c6666e4: *stop=0x8c6666e4u;r[1]=r[0];goto L8c6666e6; /* mov r0,r1 */
L8c6666e6: *stop=0x8c6666e6u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6666e8; /* mov.l @r1,r2 */
L8c6666e8: *stop=0x8c6666e8u;r[3]+=12u;goto L8c6666ea; /* add #0xc,r3 */
L8c6666ea: *stop=0x8c6666eau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6666ec; /* mov.l r2,@r3 */
L8c6666ec: *stop=0x8c6666ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66688cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6666ee; /* mov.l 0x8c66688c,r2 */
L8c6666ee: *stop=0x8c6666eeu;r[0]+=28u;goto L8c6666f0; /* add #0x1c,r0 */
L8c6666f0: *stop=0x8c6666f0u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6666f2; /* mov.l @r2,r3 */
L8c6666f2: *stop=0x8c6666f2u;*sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c6666f4; /* shlr r3 */
L8c6666f4: *stop=0x8c6666f4u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c6666f6; /* mov.l r3,@r0 */
L8c6666f6: *stop=0x8c6666f6u;r[3]=0u;goto L8c6666f8; /* mov #0x0,r3 */
L8c6666f8: *stop=0x8c6666f8u;r[1]+=60u;goto L8c6666fa; /* add #0x3c,r1 */
L8c6666fa: *stop=0x8c6666fau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c6666fc; /* mov.l r3,@r1 */
L8c6666fc: *stop=0x8c6666fcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6666fe; /* lds.l @r15+,PR */
L8c6666fe: *stop=0x8c6666feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666898u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666700; /* mov.l 0x8c666898,r3 */
L8c666700: *stop=0x8c666700u;r[2]=1u;goto L8c666702; /* mov #0x1,r2 */
L8c666702: *stop=0x8c666702u;if(*pr!=entry_pr)return -3;*stop=0x8c666704u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c663140(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c663140;
L8c663140: *stop=0x8c663140u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c663142; /* mov.l r14,@-r15 */
L8c663142: *stop=0x8c663142u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c663144; /* mov.l r13,@-r15 */
L8c663144: *stop=0x8c663144u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c663146; /* mov.l r12,@-r15 */
L8c663146: *stop=0x8c663146u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c663148; /* mov.l r11,@-r15 */
L8c663148: *stop=0x8c663148u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c66314a; /* mov.l r10,@-r15 */
L8c66314a: *stop=0x8c66314au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6631fcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66314c; /* mov.l 0x8c6631fc,r3 */
L8c66314c: *stop=0x8c66314cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c66314e; /* mov.l r9,@-r15 */
L8c66314e: *stop=0x8c66314eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c663150; /* mov.l r8,@-r15 */
L8c663150: *stop=0x8c663150u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c663152; /* sts.l PR,@-r15 */
L8c663152: *stop=0x8c663152u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c663154; /* mov.l @r3,r0 */
L8c663154: *stop=0x8c663154u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6631f4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c663156; /* mov.l 0x8c6631f4,r13 */
L8c663156: *stop=0x8c663156u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6631f8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c663158; /* mov.l 0x8c6631f8,r14 */
L8c663158: *stop=0x8c663158u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c66315a; /* cmp/eq #0x1,r0 */
L8c66315a: *stop=0x8c66315au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6631e8u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c66315c; /* mov.l 0x8c6631e8,r9 */
L8c66315c: *stop=0x8c66315cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6631e4u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c66315e; /* mov.l 0x8c6631e4,r8 */
L8c66315e: *stop=0x8c66315eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6631f0u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c663160; /* mov.l 0x8c6631f0,r12 */
L8c663160: *stop=0x8c663160u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6631ecu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c663162; /* mov.l 0x8c6631ec,r10 */
L8c663162: take=!(*sr&1u);*stop=0x8c663164u;r[11]=0u;if(take)goto L8c663220;goto L8c663166; /* bf/s 0x8c663220 */
L8c663166: *stop=0x8c663166u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663208u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c663168; /* mov.l 0x8c663208,r1 */
L8c663168: *stop=0x8c663168u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663204u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c66316a; /* mov.l 0x8c663204,r4 */
L8c66316a: *stop=0x8c66316au;{uint32_t loaded=rd(m,bus,&io_status,0x8c663200u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c66316c; /* mov.l 0x8c663200,r5 */
L8c66316c: target=r[1];*pr=0x8c663170u;*stop=0x8c66316eu;r[6]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663170; /* jsr @r1 */
L8c663170: *stop=0x8c663170u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c663172; /* cmp/eq #0x1,r0 */
L8c663172: if(!(*sr&1u))goto L8c663194;goto L8c663174; /* bf 0x8c663194 */
L8c663174: *stop=0x8c663174u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663210u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663176; /* mov.l 0x8c663210,r3 */
L8c663176: *stop=0x8c663176u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66320cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663178; /* mov.l 0x8c66320c,r2 */
L8c663178: *stop=0x8c663178u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c66317a; /* mov.l r2,@r3 */
L8c66317a: *stop=0x8c66317au;{uint32_t loaded=rd(m,bus,&io_status,0x8c663214u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c66317c; /* mov.l 0x8c663214,r0 */
L8c66317c: *stop=0x8c66317cu;r[1]=r[2];goto L8c66317e; /* mov r2,r1 */
L8c66317e: *stop=0x8c66317eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6631e0u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c663180; /* mov.w 0x8c6631e0,r4 */
L8c663180: *stop=0x8c663180u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c663182; /* mov.l r2,@r0 */
L8c663182: *stop=0x8c663182u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[11],4);if(io_status)return io_status;goto L8c663184; /* mov.l r11,@r9 */
L8c663184: *stop=0x8c663184u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c663186; /* mov.l r2,@r13 */
L8c663186: *stop=0x8c663186u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663218u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663188; /* mov.l 0x8c663218,r2 */
L8c663188: *stop=0x8c663188u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c66318a; /* mov.l r1,@r8 */
L8c66318a: *stop=0x8c66318au;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[11],4);if(io_status)return io_status;goto L8c66318c; /* mov.l r11,@r10 */
L8c66318c: target=r[2];*pr=0x8c663190u;*stop=0x8c66318eu;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663190; /* jsr @r2 */
L8c663190: *stop=0x8c663192u;goto L8c6631ac; /* bra 0x8c6631ac */
L8c663194: *stop=0x8c663194u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663210u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663196; /* mov.l 0x8c663210,r2 */
L8c663196: *stop=0x8c663196u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c663198; /* mov.l r14,@r2 */
L8c663198: *stop=0x8c663198u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663214u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66319a; /* mov.l 0x8c663214,r3 */
L8c66319a: *stop=0x8c66319au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6631e0u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c66319c; /* mov.w 0x8c6631e0,r4 */
L8c66319c: *stop=0x8c66319cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663218u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66319e; /* mov.l 0x8c663218,r1 */
L8c66319e: *stop=0x8c66319eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6631a0; /* mov.l r14,@r3 */
L8c6631a0: *stop=0x8c6631a0u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[11],4);if(io_status)return io_status;goto L8c6631a2; /* mov.l r11,@r9 */
L8c6631a2: *stop=0x8c6631a2u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[14],4);if(io_status)return io_status;goto L8c6631a4; /* mov.l r14,@r13 */
L8c6631a4: *stop=0x8c6631a4u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[14],4);if(io_status)return io_status;goto L8c6631a6; /* mov.l r14,@r8 */
L8c6631a6: *stop=0x8c6631a6u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[11],4);if(io_status)return io_status;goto L8c6631a8; /* mov.l r11,@r10 */
L8c6631a8: target=r[1];*pr=0x8c6631acu;*stop=0x8c6631aau;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6631ac; /* jsr @r1 */
L8c6631ac: target=0x8c663080u;*pr=0x8c6631b0u;*stop=0x8c6631aeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6631b0; /* bsr 0x8c663080 */
L8c6631b0: *stop=0x8c6631b0u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6631b2; /* mov.l @r13,r2 */
L8c6631b2: *stop=0x8c6631b2u;if(!valid_access(r[12]+16u,4u))return -3;wr(m,bus,&io_status,r[12]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6631b4; /* mov.l r2,@(0x10,r12) */
L8c6631b4: *stop=0x8c6631b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66321cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6631b6; /* mov.l 0x8c66321c,r2 */
L8c6631b6: *stop=0x8c6631b6u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6631b8; /* mov.l @r10,r3 */
L8c6631b8: target=r[2];*pr=0x8c6631bcu;*stop=0x8c6631bau;if(!valid_access(r[12]+36u,4u))return -3;wr(m,bus,&io_status,r[12]+0x24u,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6631bc; /* jsr @r2 */
L8c6631bc: *stop=0x8c6631bcu;r[4]=r[0];goto L8c6631be; /* mov r0,r4 */
L8c6631be: *stop=0x8c6631beu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6631c0; /* tst r4,r4 */
L8c6631c0: if(!(*sr&1u))goto L8c6631c6;goto L8c6631c2; /* bf 0x8c6631c6 */
L8c6631c2: *stop=0x8c6631c4u;goto L8c66329c; /* bra 0x8c66329c */
L8c6631c6: *stop=0x8c6631c6u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6631c8; /* mov.l @r9,r2 */
L8c6631c8: *stop=0x8c6631c8u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6631ca; /* mov.l r2,@(0xc,r4) */
L8c6631ca: *stop=0x8c6631cau;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6631cc; /* mov.l @r13,r3 */
L8c6631cc: *stop=0x8c6631ccu;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6631ce; /* mov.l r3,@(0x10,r4) */
L8c6631ce: *stop=0x8c6631d0u;r[5]=r[12];goto L8c6632ac; /* bra 0x8c6632ac */
L8c663220: *stop=0x8c663220u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6632ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663222; /* mov.l 0x8c6632ec,r2 */
L8c663222: *stop=0x8c663222u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6632e8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c663224; /* mov.l 0x8c6632e8,r4 */
L8c663224: *stop=0x8c663224u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6632e4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c663226; /* mov.l 0x8c6632e4,r5 */
L8c663226: target=r[2];*pr=0x8c66322au;*stop=0x8c663228u;r[6]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66322a; /* jsr @r2 */
L8c66322a: *stop=0x8c66322au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c66322c; /* cmp/eq #0x1,r0 */
L8c66322c: if(!(*sr&1u))goto L8c66324c;goto L8c66322e; /* bf 0x8c66324c */
L8c66322e: *stop=0x8c66322eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6632f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663230; /* mov.l 0x8c6632f4,r3 */
L8c663230: *stop=0x8c663230u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6632f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663232; /* mov.l 0x8c6632f0,r2 */
L8c663232: *stop=0x8c663232u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c663234; /* mov.l r2,@r3 */
L8c663234: *stop=0x8c663234u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6632f8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c663236; /* mov.l 0x8c6632f8,r1 */
L8c663236: *stop=0x8c663236u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6632e0u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c663238; /* mov.w 0x8c6632e0,r4 */
L8c663238: *stop=0x8c663238u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6632fcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66323a; /* mov.l 0x8c6632fc,r2 */
L8c66323a: *stop=0x8c66323au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c66323c; /* mov.l r14,@r1 */
L8c66323c: *stop=0x8c66323cu;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[11],4);if(io_status)return io_status;goto L8c66323e; /* mov.l r11,@r9 */
L8c66323e: *stop=0x8c66323eu;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[14],4);if(io_status)return io_status;goto L8c663240; /* mov.l r14,@r13 */
L8c663240: *stop=0x8c663240u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[14],4);if(io_status)return io_status;goto L8c663242; /* mov.l r14,@r8 */
L8c663242: *stop=0x8c663242u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[14],4);if(io_status)return io_status;goto L8c663244; /* mov.l r14,@r10 */
L8c663244: target=r[2];*pr=0x8c663248u;*stop=0x8c663246u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663248; /* jsr @r2 */
L8c663248: *stop=0x8c66324au;goto L8c663266; /* bra 0x8c663266 */
L8c66324c: *stop=0x8c66324cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6632f4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66324e; /* mov.l 0x8c6632f4,r2 */
L8c66324e: *stop=0x8c66324eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c663250; /* mov.l r14,@r2 */
L8c663250: *stop=0x8c663250u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6632f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663252; /* mov.l 0x8c6632f8,r3 */
L8c663252: *stop=0x8c663252u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663300u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c663254; /* mov.l 0x8c663300,r4 */
L8c663254: *stop=0x8c663254u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6632fcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c663256; /* mov.l 0x8c6632fc,r1 */
L8c663256: *stop=0x8c663256u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c663258; /* mov.l r4,@r3 */
L8c663258: *stop=0x8c663258u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[11],4);if(io_status)return io_status;goto L8c66325a; /* mov.l r11,@r9 */
L8c66325a: *stop=0x8c66325au;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[4],4);if(io_status)return io_status;goto L8c66325c; /* mov.l r4,@r13 */
L8c66325c: *stop=0x8c66325cu;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c66325e; /* mov.l r4,@r8 */
L8c66325e: *stop=0x8c66325eu;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[4],4);if(io_status)return io_status;goto L8c663260; /* mov.l r4,@r10 */
L8c663260: *stop=0x8c663260u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6632e0u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c663262; /* mov.w 0x8c6632e0,r4 */
L8c663262: target=r[1];*pr=0x8c663266u;*stop=0x8c663264u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663266; /* jsr @r1 */
L8c663266: target=0x8c663080u;*pr=0x8c66326au;*stop=0x8c663268u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66326a; /* bsr 0x8c663080 */
L8c66326a: *stop=0x8c66326au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66326c; /* mov.l @r13,r2 */
L8c66326c: *stop=0x8c66326cu;if(!valid_access(r[12]+16u,4u))return -3;wr(m,bus,&io_status,r[12]+0x10u,r[2],4);if(io_status)return io_status;goto L8c66326e; /* mov.l r2,@(0x10,r12) */
L8c66326e: *stop=0x8c66326eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663304u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663270; /* mov.l 0x8c663304,r2 */
L8c663270: *stop=0x8c663270u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c663272; /* mov.l @r10,r3 */
L8c663272: target=r[2];*pr=0x8c663276u;*stop=0x8c663274u;if(!valid_access(r[12]+36u,4u))return -3;wr(m,bus,&io_status,r[12]+0x24u,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663276; /* jsr @r2 */
L8c663276: *stop=0x8c663276u;r[4]=r[0];goto L8c663278; /* mov r0,r4 */
L8c663278: *stop=0x8c663278u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c66327a; /* tst r4,r4 */
L8c66327a: if((*sr&1u))goto L8c66329c;goto L8c66327c; /* bt 0x8c66329c */
L8c66327c: *stop=0x8c66327cu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66327e; /* mov.l @r9,r2 */
L8c66327e: *stop=0x8c66327eu;r[5]=r[12];goto L8c663280; /* mov r12,r5 */
L8c663280: *stop=0x8c663280u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[2],4);if(io_status)return io_status;goto L8c663282; /* mov.l r2,@(0xc,r4) */
L8c663282: *stop=0x8c663282u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c663284; /* mov.l @r13,r3 */
L8c663284: *stop=0x8c663284u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[3],4);if(io_status)return io_status;goto L8c663286; /* mov.l r3,@(0x10,r4) */
L8c663286: *stop=0x8c663286u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663288; /* mov.l @(0x8,r5),r2 */
L8c663288: *stop=0x8c663288u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c66328a; /* mov.l r2,@(0x8,r4) */
L8c66328a: *stop=0x8c66328au;{uint32_t loaded=rd(m,bus,&io_status,0x8c663304u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66328c; /* mov.l 0x8c663304,r2 */
L8c66328c: *stop=0x8c66328cu;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[4],4);if(io_status)return io_status;goto L8c66328e; /* mov.l r4,@(0x8,r5) */
L8c66328e: *stop=0x8c66328eu;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663290; /* mov.l @(0xc,r5),r3 */
L8c663290: *stop=0x8c663290u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[3],4);if(io_status)return io_status;goto L8c663292; /* mov.l r3,@(0x4,r4) */
L8c663292: target=r[2];*pr=0x8c663296u;*stop=0x8c663294u;if(!valid_access(r[5]+12u,4u))return -3;wr(m,bus,&io_status,r[5]+0xcu,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663296; /* jsr @r2 */
L8c663296: *stop=0x8c663296u;r[4]=r[0];goto L8c663298; /* mov r0,r4 */
L8c663298: *stop=0x8c663298u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c66329a; /* tst r4,r4 */
L8c66329a: if(!(*sr&1u))goto L8c6632a0;goto L8c66329c; /* bf 0x8c6632a0 */
L8c66329c: *stop=0x8c66329eu;r[0]=3u;goto L8c6632ba; /* bra 0x8c6632ba */
L8c6632a0: *stop=0x8c6632a0u;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6632a2; /* mov.l @r8,r2 */
L8c6632a2: *stop=0x8c6632a2u;r[5]=r[12];goto L8c6632a4; /* mov r12,r5 */
L8c6632a4: *stop=0x8c6632a4u;r[5]+=20u;goto L8c6632a6; /* add #0x14,r5 */
L8c6632a6: *stop=0x8c6632a6u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6632a8; /* mov.l r2,@(0xc,r4) */
L8c6632a8: *stop=0x8c6632a8u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6632aa; /* mov.l @r10,r3 */
L8c6632aa: *stop=0x8c6632aau;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6632ac; /* mov.l r3,@(0x10,r4) */
L8c6632ac: *stop=0x8c6632acu;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6632ae; /* mov.l @(0x8,r5),r2 */
L8c6632ae: *stop=0x8c6632aeu;r[0]=0u;goto L8c6632b0; /* mov #0x0,r0 */
L8c6632b0: *stop=0x8c6632b0u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6632b2; /* mov.l r2,@(0x8,r4) */
L8c6632b2: *stop=0x8c6632b2u;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[4],4);if(io_status)return io_status;goto L8c6632b4; /* mov.l r4,@(0x8,r5) */
L8c6632b4: *stop=0x8c6632b4u;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6632b6; /* mov.l @(0xc,r5),r3 */
L8c6632b6: *stop=0x8c6632b6u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6632b8; /* mov.l r3,@(0x4,r4) */
L8c6632b8: *stop=0x8c6632b8u;if(!valid_access(r[5]+12u,4u))return -3;wr(m,bus,&io_status,r[5]+0xcu,r[4],4);if(io_status)return io_status;goto L8c6632ba; /* mov.l r4,@(0xc,r5) */
L8c6632ba: *stop=0x8c6632bau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6632bc; /* lds.l @r15+,PR */
L8c6632bc: *stop=0x8c6632bcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6632be; /* mov.l @r15+,r8 */
L8c6632be: *stop=0x8c6632beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6632c0; /* mov.l @r15+,r9 */
L8c6632c0: *stop=0x8c6632c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6632c2; /* mov.l @r15+,r10 */
L8c6632c2: *stop=0x8c6632c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6632c4; /* mov.l @r15+,r11 */
L8c6632c4: *stop=0x8c6632c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6632c6; /* mov.l @r15+,r12 */
L8c6632c6: *stop=0x8c6632c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6632c8; /* mov.l @r15+,r13 */
L8c6632c8: *stop=0x8c6632c8u;if(*pr!=entry_pr)return -3;*stop=0x8c6632cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c651520(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c651520;
L8c651520: *stop=0x8c651520u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c651522; /* sts.l PR,@-r15 */
L8c651522: *stop=0x8c651522u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651644u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651524; /* mov.l 0x8c651644,r3 */
L8c651524: *stop=0x8c651524u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651640u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c651526; /* mov.l 0x8c651640,r4 */
L8c651526: target=r[3];*pr=0x8c65152au;*stop=0x8c651528u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65152a; /* jsr @r3 */
L8c65152a: *stop=0x8c65152au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651648u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65152c; /* mov.l 0x8c651648,r2 */
L8c65152c: *stop=0x8c65152cu;r[4]=0u;goto L8c65152e; /* mov #0x0,r4 */
L8c65152e: *stop=0x8c65152eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c651530; /* mov.l r4,@r2 */
L8c651530: *stop=0x8c651530u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65164cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651532; /* mov.l 0x8c65164c,r3 */
L8c651532: *stop=0x8c651532u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c651534; /* mov.l r4,@r3 */
L8c651534: *stop=0x8c651534u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651650u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651536; /* mov.l 0x8c651650,r1 */
L8c651536: *stop=0x8c651536u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c651538; /* mov.l r4,@r1 */
L8c651538: *stop=0x8c651538u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651654u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65153a; /* mov.l 0x8c651654,r2 */
L8c65153a: *stop=0x8c65153au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c65153c; /* mov.l r4,@r2 */
L8c65153c: *stop=0x8c65153cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65165cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65153e; /* mov.l 0x8c65165c,r3 */
L8c65153e: *stop=0x8c65153eu;r[6]=r[4];goto L8c651540; /* mov r4,r6 */
L8c651540: *stop=0x8c651540u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651660u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651542; /* mov.l 0x8c651660,r1 */
L8c651542: *stop=0x8c651542u;r[7]=20u;goto L8c651544; /* mov #0x14,r7 */
L8c651544: *stop=0x8c651544u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651658u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c651546; /* mov.l 0x8c651658,r5 */
L8c651546: *stop=0x8c651546u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c651548; /* mov.l r4,@r5 */
L8c651548: *stop=0x8c651548u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[4],4);if(io_status)return io_status;goto L8c65154a; /* mov.l r4,@(0x4,r5) */
L8c65154a: *stop=0x8c65154au;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[4],4);if(io_status)return io_status;goto L8c65154c; /* mov.l r4,@(0x8,r5) */
L8c65154c: *stop=0x8c65154cu;if(!valid_access(r[5]+12u,4u))return -3;wr(m,bus,&io_status,r[5]+0xcu,r[4],4);if(io_status)return io_status;goto L8c65154e; /* mov.l r4,@(0xc,r5) */
L8c65154e: *stop=0x8c65154eu;if(!valid_access(r[5]+16u,4u))return -3;wr(m,bus,&io_status,r[5]+0x10u,r[4],4);if(io_status)return io_status;goto L8c651550; /* mov.l r4,@(0x10,r5) */
L8c651550: *stop=0x8c651550u;if(!valid_access(r[5]+20u,4u))return -3;wr(m,bus,&io_status,r[5]+0x14u,r[4],4);if(io_status)return io_status;goto L8c651552; /* mov.l r4,@(0x14,r5) */
L8c651552: *stop=0x8c651552u;if(!valid_access(r[5]+24u,4u))return -3;wr(m,bus,&io_status,r[5]+0x18u,r[4],4);if(io_status)return io_status;goto L8c651554; /* mov.l r4,@(0x18,r5) */
L8c651554: *stop=0x8c651554u;if(!valid_access(r[5]+28u,4u))return -3;wr(m,bus,&io_status,r[5]+0x1cu,r[4],4);if(io_status)return io_status;goto L8c651556; /* mov.l r4,@(0x1c,r5) */
L8c651556: *stop=0x8c651556u;if(!valid_access(r[5]+32u,4u))return -3;wr(m,bus,&io_status,r[5]+0x20u,r[4],4);if(io_status)return io_status;goto L8c651558; /* mov.l r4,@(0x20,r5) */
L8c651558: *stop=0x8c651558u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c65155a; /* mov.l r4,@r3 */
L8c65155a: *stop=0x8c65155au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c65155c; /* mov.l r4,@r1 */
L8c65155c: *stop=0x8c65155cu;r[3]=r[5];goto L8c65155e; /* mov r5,r3 */
L8c65155e: *stop=0x8c65155eu;r[3]+=44u;goto L8c651560; /* add #0x2c,r3 */
L8c651560: *stop=0x8c651560u;r[3]+=r[6];goto L8c651562; /* add r6,r3 */
L8c651562: *stop=0x8c651562u;r[6]+=4u;goto L8c651564; /* add #0x4,r6 */
L8c651564: *stop=0x8c651564u;*sr=(*sr&~1u)|(r[6]>=r[7]);goto L8c651566; /* cmp/hs r7,r6 */
L8c651566: take=!(*sr&1u);*stop=0x8c651568u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;if(take)goto L8c65155c;goto L8c65156a; /* bf/s 0x8c65155c */
L8c65156a: *stop=0x8c65156au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65156c; /* lds.l @r15+,PR */
L8c65156c: *stop=0x8c65156cu;if(*pr!=entry_pr)return -3;*stop=0x8c65156eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c665a00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c665a00;
L8c665a00: *stop=0x8c665a00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c665a02; /* mov.l r14,@-r15 */
L8c665a02: *stop=0x8c665a02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c665a04; /* mov.l r13,@-r15 */
L8c665a04: *stop=0x8c665a04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c665a06; /* mov.l r12,@-r15 */
L8c665a06: *stop=0x8c665a06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c665a08; /* mov.l r11,@-r15 */
L8c665a08: *stop=0x8c665a08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c665a0a; /* mov.l r10,@-r15 */
L8c665a0a: *stop=0x8c665a0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c665a0c; /* sts.l PR,@-r15 */
L8c665a0c: *stop=0x8c665a0cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c665a60u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665a0e; /* mov.w 0x8c665a60,r4 */
L8c665a0e: *stop=0x8c665a0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c665a70u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665a10; /* mov.l 0x8c665a70,r5 */
L8c665a10: *stop=0x8c665a10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665a6cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c665a12; /* mov.l 0x8c665a6c,r12 */
L8c665a12: target=r[12];*pr=0x8c665a16u;*stop=0x8c665a14u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665a16; /* jsr @r12 */
L8c665a16: *stop=0x8c665a16u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665a62u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665a18; /* mov.w 0x8c665a62,r4 */
L8c665a18: *stop=0x8c665a18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665a74u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665a1a; /* mov.l 0x8c665a74,r5 */
L8c665a1a: target=r[12];*pr=0x8c665a1eu;*stop=0x8c665a1cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665a1e; /* jsr @r12 */
L8c665a1e: *stop=0x8c665a1eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c665a64u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c665a20; /* mov.w 0x8c665a64,r4 */
L8c665a20: *stop=0x8c665a20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665a74u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665a22; /* mov.l 0x8c665a74,r5 */
L8c665a22: target=r[12];*pr=0x8c665a26u;*stop=0x8c665a24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665a26; /* jsr @r12 */
L8c665a26: *stop=0x8c665a26u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665a6au,2);if(io_status)return io_status;r[14]=loaded;}goto L8c665a28; /* mov.w 0x8c665a6a,r14 */
L8c665a28: *stop=0x8c665a28u;r[10]=127u;goto L8c665a2a; /* mov #0x7f,r10 */
L8c665a2a: *stop=0x8c665a2au;{uint32_t loaded=sx(m,bus,&io_status,0x8c665a68u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c665a2c; /* mov.w 0x8c665a68,r13 */
L8c665a2c: *stop=0x8c665a2cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c665a66u,2);if(io_status)return io_status;r[11]=loaded;}goto L8c665a2e; /* mov.w 0x8c665a66,r11 */
L8c665a2e: *stop=0x8c665a2eu;r[5]=r[14]&255u;goto L8c665a30; /* extu.b r14,r5 */
L8c665a30: *stop=0x8c665a30u;r[3]=r[13]&255u;goto L8c665a32; /* extu.b r13,r3 */
L8c665a32: *stop=0x8c665a32u;r[5]<<=8;goto L8c665a34; /* shll8 r5 */
L8c665a34: *stop=0x8c665a34u;r[5]|=r[3];goto L8c665a36; /* or r3,r5 */
L8c665a36: *stop=0x8c665a36u;r[5]=r[5]&65535u;goto L8c665a38; /* extu.w r5,r5 */
L8c665a38: target=r[12];*pr=0x8c665a3cu;*stop=0x8c665a3au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665a3c; /* jsr @r12 */
L8c665a3c: *stop=0x8c665a3cu;r[14]+=4294967295u;goto L8c665a3e; /* add #-0x1,r14 */
L8c665a3e: *stop=0x8c665a3eu;*sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[10]);goto L8c665a40; /* cmp/gt r10,r14 */
L8c665a40: *stop=0x8c665a40u;r[11]+=4u;goto L8c665a42; /* add #0x4,r11 */
L8c665a42: take=(*sr&1u);*stop=0x8c665a44u;r[13]+=4294967295u;if(take)goto L8c665a2e;goto L8c665a46; /* bt/s 0x8c665a2e */
L8c665a46: *stop=0x8c665a46u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c665a48; /* lds.l @r15+,PR */
L8c665a48: *stop=0x8c665a48u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c665a4a; /* mov.l @r15+,r10 */
L8c665a4a: *stop=0x8c665a4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c665a4c; /* mov.l @r15+,r11 */
L8c665a4c: *stop=0x8c665a4cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c665a4e; /* mov.l @r15+,r12 */
L8c665a4e: *stop=0x8c665a4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c665a50; /* mov.l @r15+,r13 */
L8c665a50: *stop=0x8c665a50u;if(*pr!=entry_pr)return -3;*stop=0x8c665a52u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c666706(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c666706;
L8c666706: *stop=0x8c666706u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c666708; /* mov.l r14,@-r15 */
L8c666708: *stop=0x8c666708u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c66670a; /* mov.l r13,@-r15 */
L8c66670a: *stop=0x8c66670au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c66670c; /* mov.l r12,@-r15 */
L8c66670c: *stop=0x8c66670cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c66670e; /* mov.l r11,@-r15 */
L8c66670e: *stop=0x8c66670eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c666710; /* mov.l r10,@-r15 */
L8c666710: *stop=0x8c666710u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c666712; /* mov.l r9,@-r15 */
L8c666712: *stop=0x8c666712u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c666714; /* mov.l r8,@-r15 */
L8c666714: *stop=0x8c666714u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c666716; /* sts.l PR,@-r15 */
L8c666716: *stop=0x8c666716u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66689cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666718; /* mov.l 0x8c66689c,r3 */
L8c666718: *stop=0x8c666718u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c66671a; /* mov.l @(0x20,r15),r4 */
L8c66671a: *stop=0x8c66671au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c66671c; /* mov.l r4,@r3 */
L8c66671c: *stop=0x8c66671cu;r[2]=r[4];goto L8c66671e; /* mov r4,r2 */
L8c66671e: *stop=0x8c66671eu;r[5]=1u;goto L8c666720; /* mov #0x1,r5 */
L8c666720: *stop=0x8c666720u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c666722; /* tst r5,r2 */
L8c666722: take=(*sr&1u);*stop=0x8c666724u;r[11]=0u;if(take)goto L8c66672a;goto L8c666726; /* bt/s 0x8c66672a */
L8c666726: *stop=0x8c666728u;r[9]=r[11];goto L8c66672c; /* bra 0x8c66672c */
L8c66672a: *stop=0x8c66672au;r[9]=r[5];goto L8c66672c; /* mov r5,r9 */
L8c66672c: *stop=0x8c66672cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66672e; /* mov.l 0x8c6668a0,r3 */
L8c66672e: *stop=0x8c66672eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c666898u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666730; /* mov.l 0x8c666898,r1 */
L8c666730: *stop=0x8c666730u;r[3]&=r[4];goto L8c666732; /* and r4,r3 */
L8c666732: *stop=0x8c666732u;r[3]>>=16;goto L8c666734; /* shlr16 r3 */
L8c666734: *stop=0x8c666734u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c666736; /* mov.l r3,@r1 */
L8c666736: *stop=0x8c666736u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666738; /* mov.l 0x8c6668a4,r2 */
L8c666738: *stop=0x8c666738u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668a8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c66673a; /* mov.l 0x8c6668a8,r0 */
L8c66673a: *stop=0x8c66673au;r[2]&=r[4];goto L8c66673c; /* and r4,r2 */
L8c66673c: *stop=0x8c66673cu;r[2]>>=16;goto L8c66673e; /* shlr16 r2 */
L8c66673e: *stop=0x8c66673eu;*sr=(*sr&~1u)|(r[2]&1u);r[2]>>=1;goto L8c666740; /* shlr r2 */
L8c666740: *stop=0x8c666740u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c666742; /* mov.l r2,@r0 */
L8c666742: *stop=0x8c666742u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666744; /* mov.l 0x8c6668ac,r3 */
L8c666744: *stop=0x8c666744u;r[1]+=8u;goto L8c666746; /* add #0x8,r1 */
L8c666746: *stop=0x8c666746u;r[3]&=r[4];goto L8c666748; /* and r4,r3 */
L8c666748: *stop=0x8c666748u;r[3]>>=16;goto L8c66674a; /* shlr16 r3 */
L8c66674a: *stop=0x8c66674au;r[3]>>=2;goto L8c66674c; /* shlr2 r3 */
L8c66674c: *stop=0x8c66674cu;*sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c66674e; /* shlr r3 */
L8c66674e: *stop=0x8c66674eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c666750; /* mov.l r3,@r1 */
L8c666750: *stop=0x8c666750u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666890u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666752; /* mov.l 0x8c666890,r2 */
L8c666752: *stop=0x8c666752u;r[3]=4294967275u;goto L8c666754; /* mov #-0x15,r3 */
L8c666754: *stop=0x8c666754u;r[0]+=8u;goto L8c666756; /* add #0x8,r0 */
L8c666756: *stop=0x8c666756u;r[2]&=r[4];goto L8c666758; /* and r4,r2 */
L8c666758: *stop=0x8c666758u;r[2]=(r[3]&0x80000000u)?((r[3]&31u)?r[2]>>((0u-r[3])&31u):0u):(r[2]<<(r[3]&31u));goto L8c66675a; /* shld r3,r2 */
L8c66675a: *stop=0x8c66675au;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c66675c; /* mov.l r2,@r0 */
L8c66675c: *stop=0x8c66675cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66675e; /* mov.l 0x8c6668b0,r3 */
L8c66675e: *stop=0x8c66675eu;r[2]=4294967273u;goto L8c666760; /* mov #-0x17,r2 */
L8c666760: *stop=0x8c666760u;r[1]+=8u;goto L8c666762; /* add #0x8,r1 */
L8c666762: *stop=0x8c666762u;r[3]&=r[4];goto L8c666764; /* and r4,r3 */
L8c666764: *stop=0x8c666764u;r[3]=(r[2]&0x80000000u)?((r[2]&31u)?r[3]>>((0u-r[2])&31u):0u):(r[3]<<(r[2]&31u));goto L8c666766; /* shld r2,r3 */
L8c666766: *stop=0x8c666766u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c666768; /* mov.l r3,@r1 */
L8c666768: *stop=0x8c666768u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66676a; /* mov.l 0x8c6668b4,r2 */
L8c66676a: *stop=0x8c66676au;r[0]+=8u;goto L8c66676c; /* add #0x8,r0 */
L8c66676c: *stop=0x8c66676cu;r[4]&=r[2];goto L8c66676e; /* and r2,r4 */
L8c66676e: *stop=0x8c66676eu;r[4]>>=16;goto L8c666770; /* shlr16 r4 */
L8c666770: *stop=0x8c666770u;r[4]>>=8;goto L8c666772; /* shlr8 r4 */
L8c666772: *stop=0x8c666772u;*sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1;goto L8c666774; /* shlr r4 */
L8c666774: *stop=0x8c666774u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;goto L8c666776; /* mov.l r4,@r0 */
L8c666776: *stop=0x8c666776u;r[1]+=4294967196u;goto L8c666778; /* add #-0x64,r1 */
L8c666778: *stop=0x8c666778u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66677a; /* mov.l @r1,r3 */
L8c66677a: *stop=0x8c66677au;r[3]>>=2;goto L8c66677c; /* shlr2 r3 */
L8c66677c: *stop=0x8c66677cu;r[2]=r[3];goto L8c66677e; /* mov r3,r2 */
L8c66677e: *stop=0x8c66677eu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c666780; /* shll r3 */
L8c666780: *stop=0x8c666780u;r[1]+=4294967284u;goto L8c666782; /* add #-0xc,r1 */
L8c666782: *stop=0x8c666782u;r[3]+=r[2];goto L8c666784; /* add r2,r3 */
L8c666784: *stop=0x8c666784u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c666786; /* mov.l r3,@r1 */
L8c666786: *stop=0x8c666786u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666788; /* mov.l 0x8c6668b8,r2 */
L8c666788: *stop=0x8c666788u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66678a; /* mov.l @r2,r3 */
L8c66678a: *stop=0x8c66678au;r[0]+=4294967196u;goto L8c66678c; /* add #-0x64,r0 */
L8c66678c: *stop=0x8c66678cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66678e; /* mov.l @r0,r2 */
L8c66678e: *stop=0x8c66678eu;*macl=r[3]*r[2];goto L8c666790; /* mul.l r3,r2 */
L8c666790: target=0x8c665e00u;*pr=0x8c666794u;*stop=0x8c666792u;r[4]=*macl;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666794; /* bsr 0x8c665e00 */
L8c666794: *stop=0x8c666794u;r[8]=2u;goto L8c666796; /* mov #0x2,r8 */
L8c666796: *stop=0x8c666796u;r[4]=r[11];goto L8c666798; /* mov r11,r4 */
L8c666798: *stop=0x8c666798u;r[7]=4294967168u;goto L8c66679a; /* mov #-0x80,r7 */
L8c66679a: *stop=0x8c66679au;r[10]=r[11];goto L8c66679c; /* mov r11,r10 */
L8c66679c: *stop=0x8c66679cu;r[13]=4294967264u;goto L8c66679e; /* mov #-0x20,r13 */
L8c66679e: *stop=0x8c66679eu;r[12]=r[0];goto L8c6667a0; /* mov r0,r12 */
L8c6667a0: *stop=0x8c6667a0u;r[5]=r[11];goto L8c6667a2; /* mov r11,r5 */
L8c6667a2: *stop=0x8c6667a2u;r[6]=r[11];goto L8c6667a4; /* mov r11,r6 */
L8c6667a4: *stop=0x8c6667a4u;r[14]=r[10];goto L8c6667a6; /* mov r10,r14 */
L8c6667a6: *stop=0x8c6667a6u;r[14]<<=2;goto L8c6667a8; /* shll2 r14 */
L8c6667a8: *stop=0x8c6667a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668bcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6667aa; /* mov.l 0x8c6668bc,r1 */
L8c6667aa: *stop=0x8c6667aau;r[14]<<=2;goto L8c6667ac; /* shll2 r14 */
L8c6667ac: *stop=0x8c6667acu;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6667ae; /* shll r14 */
L8c6667ae: *stop=0x8c6667aeu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6667b0; /* mov.l @r1,r3 */
L8c6667b0: *stop=0x8c6667b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668c0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6667b2; /* mov.l 0x8c6668c0,r0 */
L8c6667b2: *stop=0x8c6667b2u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6667b4; /* mov.l @(r0,r14),r2 */
L8c6667b4: *stop=0x8c6667b4u;r[0]+=4u;goto L8c6667b6; /* add #0x4,r0 */
L8c6667b6: *stop=0x8c6667b6u;r[2]+=r[3];goto L8c6667b8; /* add r3,r2 */
L8c6667b8: *stop=0x8c6667b8u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c6667ba; /* mov.l r2,@(r0,r14) */
L8c6667ba: *stop=0x8c6667bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6667bc; /* mov.l 0x8c6668c4,r3 */
L8c6667bc: *stop=0x8c6667bcu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6667be; /* mov.l @r3,r0 */
L8c6667be: *stop=0x8c6667beu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c6667c0; /* tst #0x20,r0 */
L8c6667c0: if((*sr&1u))goto L8c666814;goto L8c6667c2; /* bt 0x8c666814 */
L8c6667c2: *stop=0x8c6667c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668c8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6667c4; /* mov.l 0x8c6668c8,r0 */
L8c6667c4: *stop=0x8c6667c4u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6667c6; /* mov.l @r0,r2 */
L8c6667c6: *stop=0x8c6667c6u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6667c8; /* tst r2,r2 */
L8c6667c8: if((*sr&1u))goto L8c6667f2;goto L8c6667ca; /* bt 0x8c6667f2 */
L8c6667ca: *stop=0x8c6667cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668ccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6667cc; /* mov.l 0x8c6668cc,r0 */
L8c6667cc: *stop=0x8c6667ccu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6667ce; /* mov.l @(r0,r14),r2 */
L8c6667ce: *stop=0x8c6667ceu;r[2]+=4294967228u;goto L8c6667d0; /* add #-0x44,r2 */
L8c6667d0: *stop=0x8c6667d0u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c6667d2; /* mov.l r2,@(r0,r14) */
L8c6667d2: *stop=0x8c6667d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668b8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6667d4; /* mov.l 0x8c6668b8,r1 */
L8c6667d4: *stop=0x8c6667d4u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6667d6; /* mov.l @r1,r2 */
L8c6667d6: *stop=0x8c6667d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6667d8; /* mov.l 0x8c6668d0,r3 */
L8c6667d8: *stop=0x8c6667d8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6667da; /* mov.l @r3,r1 */
L8c6667da: *stop=0x8c6667dau;*macl=r[2]*r[1];goto L8c6667dc; /* mul.l r2,r1 */
L8c6667dc: *stop=0x8c6667dcu;r[2]=*macl;goto L8c6667de; /* sts MACL,r2 */
L8c6667de: *stop=0x8c6667deu;r[1]=r[2];goto L8c6667e0; /* mov r2,r1 */
L8c6667e0: *stop=0x8c6667e0u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6667e2; /* shll r2 */
L8c6667e2: *stop=0x8c6667e2u;r[2]+=r[1];goto L8c6667e4; /* add r1,r2 */
L8c6667e4: *stop=0x8c6667e4u;r[2]<<=2;goto L8c6667e6; /* shll2 r2 */
L8c6667e6: *stop=0x8c6667e6u;r[2]<<=2;goto L8c6667e8; /* shll2 r2 */
L8c6667e8: *stop=0x8c6667e8u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6667ea; /* shll r2 */
L8c6667ea: *stop=0x8c6667eau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6667ec; /* mov.l @(r0,r14),r1 */
L8c6667ec: *stop=0x8c6667ecu;r[1]-=r[2];goto L8c6667ee; /* sub r2,r1 */
L8c6667ee: *stop=0x8c6667f0u;goto L8c666810; /* bra 0x8c666810 */
L8c6667f2: *stop=0x8c6667f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668ccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6667f4; /* mov.l 0x8c6668cc,r0 */
L8c6667f4: *stop=0x8c6667f4u;r[1]=r[0];goto L8c6667f6; /* mov r0,r1 */
L8c6667f6: *stop=0x8c6667f6u;r[1]+=4294967280u;goto L8c6667f8; /* add #-0x10,r1 */
L8c6667f8: *stop=0x8c6667f8u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6667fa; /* mov.l @r1,r2 */
L8c6667fa: *stop=0x8c6667fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6667fc; /* mov.l 0x8c6668d0,r3 */
L8c6667fc: *stop=0x8c6667fcu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6667fe; /* mov.l @r3,r1 */
L8c6667fe: *stop=0x8c6667feu;*macl=r[2]*r[1];goto L8c666800; /* mul.l r2,r1 */
L8c666800: *stop=0x8c666800u;r[2]=*macl;goto L8c666802; /* sts MACL,r2 */
L8c666802: *stop=0x8c666802u;r[1]=r[2];goto L8c666804; /* mov r2,r1 */
L8c666804: *stop=0x8c666804u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c666806; /* shll r2 */
L8c666806: *stop=0x8c666806u;r[2]+=r[1];goto L8c666808; /* add r1,r2 */
L8c666808: *stop=0x8c666808u;r[2]<<=2;goto L8c66680a; /* shll2 r2 */
L8c66680a: *stop=0x8c66680au;r[2]<<=2;goto L8c66680c; /* shll2 r2 */
L8c66680c: *stop=0x8c66680cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c66680e; /* mov.l @(r0,r14),r1 */
L8c66680e: *stop=0x8c66680eu;r[1]-=r[2];goto L8c666810; /* sub r2,r1 */
L8c666810: *stop=0x8c666812u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[1],4);if(io_status)return io_status;goto L8c666834; /* bra 0x8c666834 */
L8c666814: *stop=0x8c666814u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668ccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c666816; /* mov.l 0x8c6668cc,r0 */
L8c666816: *stop=0x8c666816u;r[1]=r[0];goto L8c666818; /* mov r0,r1 */
L8c666818: *stop=0x8c666818u;r[1]+=4294967280u;goto L8c66681a; /* add #-0x10,r1 */
L8c66681a: *stop=0x8c66681au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66681c; /* mov.l @r1,r2 */
L8c66681c: *stop=0x8c66681cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66681e; /* mov.l 0x8c6668d0,r3 */
L8c66681e: *stop=0x8c66681eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666820; /* mov.l @r3,r1 */
L8c666820: *stop=0x8c666820u;*macl=r[2]*r[1];goto L8c666822; /* mul.l r2,r1 */
L8c666822: *stop=0x8c666822u;r[2]=*macl;goto L8c666824; /* sts MACL,r2 */
L8c666824: *stop=0x8c666824u;r[1]=r[2];goto L8c666826; /* mov r2,r1 */
L8c666826: *stop=0x8c666826u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c666828; /* shll r2 */
L8c666828: *stop=0x8c666828u;r[2]+=r[1];goto L8c66682a; /* add r1,r2 */
L8c66682a: *stop=0x8c66682au;r[2]<<=2;goto L8c66682c; /* shll2 r2 */
L8c66682c: *stop=0x8c66682cu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c66682e; /* shll r2 */
L8c66682e: *stop=0x8c66682eu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666830; /* mov.l @(r0,r14),r1 */
L8c666830: *stop=0x8c666830u;r[1]-=r[2];goto L8c666832; /* sub r2,r1 */
L8c666832: *stop=0x8c666832u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[1],4);if(io_status)return io_status;goto L8c666834; /* mov.l r1,@(r0,r14) */
L8c666834: *stop=0x8c666834u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666898u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666836; /* mov.l 0x8c666898,r2 */
L8c666836: *stop=0x8c666836u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666838; /* mov.l @r2,r3 */
L8c666838: *stop=0x8c666838u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c66683a; /* tst r3,r3 */
L8c66683a: if(!(*sr&1u))goto L8c6668e0;goto L8c66683c; /* bf 0x8c6668e0 */
L8c66683c: *stop=0x8c66683cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668d4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66683e; /* mov.l 0x8c6668d4,r1 */
L8c66683e: *stop=0x8c66683eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666840; /* mov.l @r1,r3 */
L8c666840: *stop=0x8c666840u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668c0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c666842; /* mov.l 0x8c6668c0,r0 */
L8c666842: *stop=0x8c666842u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666844; /* mov.l @(r0,r4),r0 */
L8c666844: *stop=0x8c666844u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666846; /* mov.l 0x8c6668d8,r2 */
L8c666846: *stop=0x8c666846u;r[0]+=r[3];goto L8c666848; /* add r3,r0 */
L8c666848: *stop=0x8c666848u;r[0]&=r[13];goto L8c66684a; /* and r13,r0 */
L8c66684a: *stop=0x8c66684au;r[2]+=r[4];goto L8c66684c; /* add r4,r2 */
L8c66684c: *stop=0x8c66684cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c66684e; /* mov.l r0,@r2 */
L8c66684e: *stop=0x8c66684eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6668ccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c666850; /* mov.l 0x8c6668cc,r0 */
L8c666850: *stop=0x8c666850u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666852; /* mov.l @(r0,r4),r3 */
L8c666852: *stop=0x8c666852u;r[3]-=r[12];goto L8c666854; /* sub r12,r3 */
L8c666854: *stop=0x8c666854u;r[0]+=12u;goto L8c666856; /* add #0xc,r0 */
L8c666856: *stop=0x8c666856u;r[3]&=r[7];goto L8c666858; /* and r7,r3 */
L8c666858: *stop=0x8c666858u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[3],4);if(io_status)return io_status;goto L8c66685a; /* mov.l r3,@(r0,r4) */
L8c66685a: *stop=0x8c66685au;r[0]+=4294967292u;goto L8c66685c; /* add #-0x4,r0 */
L8c66685c: *stop=0x8c66685cu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66685e; /* mov.l @(r0,r4),r2 */
L8c66685e: *stop=0x8c66685eu;r[0]+=8u;goto L8c666860; /* add #0x8,r0 */
L8c666860: *stop=0x8c666860u;r[2]+=r[12];goto L8c666862; /* add r12,r2 */
L8c666862: *stop=0x8c666864u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[2],4);if(io_status)return io_status;goto L8c666904; /* bra 0x8c666904 */
L8c6668e0: *stop=0x8c6668e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6669a4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6668e2; /* mov.l 0x8c6669a4,r0 */
L8c6668e2: *stop=0x8c6668e2u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6668e4; /* mov.l @(r0,r5),r1 */
L8c6668e4: *stop=0x8c6668e4u;r[1]-=r[12];goto L8c6668e6; /* sub r12,r1 */
L8c6668e6: *stop=0x8c6668e6u;r[0]+=8u;goto L8c6668e8; /* add #0x8,r0 */
L8c6668e8: *stop=0x8c6668e8u;r[1]&=r[7];goto L8c6668ea; /* and r7,r1 */
L8c6668ea: *stop=0x8c6668eau;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[1],4);if(io_status)return io_status;goto L8c6668ec; /* mov.l r1,@(r0,r5) */
L8c6668ec: *stop=0x8c6668ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6669a8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6668ee; /* mov.l 0x8c6669a8,r1 */
L8c6668ee: *stop=0x8c6668eeu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6668f0; /* mov.l @r1,r3 */
L8c6668f0: *stop=0x8c6668f0u;r[0]+=4294967284u;goto L8c6668f2; /* add #-0xc,r0 */
L8c6668f2: *stop=0x8c6668f2u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6668f4; /* mov.l @(r0,r5),r1 */
L8c6668f4: *stop=0x8c6668f4u;r[1]+=r[3];goto L8c6668f6; /* add r3,r1 */
L8c6668f6: *stop=0x8c6668f6u;r[0]+=16u;goto L8c6668f8; /* add #0x10,r0 */
L8c6668f8: *stop=0x8c6668f8u;r[1]&=r[13];goto L8c6668fa; /* and r13,r1 */
L8c6668fa: *stop=0x8c6668fau;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[1],4);if(io_status)return io_status;goto L8c6668fc; /* mov.l r1,@(r0,r5) */
L8c6668fc: *stop=0x8c6668fcu;r[0]+=4294967292u;goto L8c6668fe; /* add #-0x4,r0 */
L8c6668fe: *stop=0x8c6668feu;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666900; /* mov.l @(r0,r5),r3 */
L8c666900: *stop=0x8c666900u;r[0]+=8u;goto L8c666902; /* add #0x8,r0 */
L8c666902: *stop=0x8c666902u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[3],4);if(io_status)return io_status;goto L8c666904; /* mov.l r3,@(r0,r5) */
L8c666904: *stop=0x8c666904u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6669acu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c666906; /* mov.l 0x8c6669ac,r0 */
L8c666906: *stop=0x8c666906u;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666908; /* mov.l @(r0,r6),r2 */
L8c666908: *stop=0x8c666908u;r[0]+=24u;goto L8c66690a; /* add #0x18,r0 */
L8c66690a: *stop=0x8c66690au;if(!valid_access(r[0]+r[6],4u))return -3;wr(m,bus,&io_status,r[6]+r[0],r[2],4);if(io_status)return io_status;goto L8c66690c; /* mov.l r2,@(r0,r6) */
L8c66690c: *stop=0x8c66690cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6669a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66690e; /* mov.l 0x8c6669a8,r2 */
L8c66690e: *stop=0x8c66690eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666910; /* mov.l @r2,r3 */
L8c666910: *stop=0x8c666910u;r[0]+=4294967272u;goto L8c666912; /* add #-0x18,r0 */
L8c666912: *stop=0x8c666912u;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666914; /* mov.l @(r0,r6),r1 */
L8c666914: *stop=0x8c666914u;r[1]+=r[3];goto L8c666916; /* add r3,r1 */
L8c666916: *stop=0x8c666916u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6669a0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c666918; /* mov.w 0x8c6669a0,r3 */
L8c666918: *stop=0x8c666918u;r[0]+=28u;goto L8c66691a; /* add #0x1c,r0 */
L8c66691a: *stop=0x8c66691au;r[1]+=r[3];goto L8c66691c; /* add r3,r1 */
L8c66691c: *stop=0x8c66691cu;if(!valid_access(r[0]+r[6],4u))return -3;wr(m,bus,&io_status,r[6]+r[0],r[1],4);if(io_status)return io_status;goto L8c66691e; /* mov.l r1,@(r0,r6) */
L8c66691e: *stop=0x8c66691eu;r[10]+=1u;goto L8c666920; /* add #0x1,r10 */
L8c666920: *stop=0x8c666920u;*sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[8]);goto L8c666922; /* cmp/ge r8,r10 */
L8c666922: *stop=0x8c666922u;r[5]+=32u;goto L8c666924; /* add #0x20,r5 */
L8c666924: *stop=0x8c666924u;r[6]+=32u;goto L8c666926; /* add #0x20,r6 */
L8c666926: take=(*sr&1u);*stop=0x8c666928u;r[4]+=32u;if(take)goto L8c66692e;goto L8c66692a; /* bt/s 0x8c66692e */
L8c66692a: *stop=0x8c66692cu;goto L8c6667a4; /* bra 0x8c6667a4 */
L8c66692e: *stop=0x8c66692eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6669b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666930; /* mov.l 0x8c6669b0,r3 */
L8c666930: *stop=0x8c666930u;r[6]=0u;goto L8c666932; /* mov #0x0,r6 */
L8c666932: *stop=0x8c666932u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c666934; /* mov.l r9,@-r15 */
L8c666934: *stop=0x8c666934u;r[7]=1u;goto L8c666936; /* mov #0x1,r7 */
L8c666936: *stop=0x8c666936u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c666938; /* mov.l r11,@-r15 */
L8c666938: *stop=0x8c666938u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c66693a; /* mov.l @r3,r5 */
L8c66693a: *stop=0x8c66693au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6669b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66693c; /* mov.l 0x8c6669b4,r2 */
L8c66693c: target=0x8c665fa8u;*pr=0x8c666940u;*stop=0x8c66693eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666940; /* bsr 0x8c665fa8 */
L8c666940: *stop=0x8c666940u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6669b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666942; /* mov.l 0x8c6669b0,r2 */
L8c666942: *stop=0x8c666942u;r[3]=1u;goto L8c666944; /* mov #0x1,r3 */
L8c666944: *stop=0x8c666944u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c666946; /* mov.l r9,@-r15 */
L8c666946: *stop=0x8c666946u;r[6]=0u;goto L8c666948; /* mov #0x0,r6 */
L8c666948: *stop=0x8c666948u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c66694a; /* mov.l r3,@-r15 */
L8c66694a: *stop=0x8c66694au;r[7]=r[3];goto L8c66694c; /* mov r3,r7 */
L8c66694c: *stop=0x8c66694cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c66694e; /* mov.l @r2,r5 */
L8c66694e: *stop=0x8c66694eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6669b4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666950; /* mov.l 0x8c6669b4,r1 */
L8c666950: target=0x8c665fa8u;*pr=0x8c666954u;*stop=0x8c666952u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666954; /* bsr 0x8c665fa8 */
L8c666954: *stop=0x8c666954u;r[15]+=16u;goto L8c666956; /* add #0x10,r15 */
L8c666956: *stop=0x8c666956u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c666958; /* lds.l @r15+,PR */
L8c666958: *stop=0x8c666958u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c66695a; /* mov.l @r15+,r8 */
L8c66695a: *stop=0x8c66695au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c66695c; /* mov.l @r15+,r9 */
L8c66695c: *stop=0x8c66695cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c66695e; /* mov.l @r15+,r10 */
L8c66695e: *stop=0x8c66695eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c666960; /* mov.l @r15+,r11 */
L8c666960: *stop=0x8c666960u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c666962; /* mov.l @r15+,r12 */
L8c666962: *stop=0x8c666962u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c666964; /* mov.l @r15+,r13 */
L8c666964: *stop=0x8c666964u;if(*pr!=entry_pr)return -3;*stop=0x8c666966u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c653640(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c653640;
L8c653640: *stop=0x8c653640u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c653642; /* stc SR,r0 */
L8c653642: *stop=0x8c653642u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6536a0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c653644; /* mov.w 0x8c6536a0,r3 */
L8c653644: *stop=0x8c653644u;r[0]>>=2;goto L8c653646; /* shlr2 r0 */
L8c653646: *stop=0x8c653646u;r[0]>>=2;goto L8c653648; /* shlr2 r0 */
L8c653648: *stop=0x8c653648u;r[0]&=0xfu;goto L8c65364a; /* and #0xf,r0 */
L8c65364a: *stop=0x8c65364au;r[5]=r[0];goto L8c65364c; /* mov r0,r5 */
L8c65364c: *stop=0x8c65364cu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65364e; /* stc SR,r0 */
L8c65364e: *stop=0x8c65364eu;r[0]&=r[3];goto L8c653650; /* and r3,r0 */
L8c653650: *stop=0x8c653650u;r[0]|=0xf0u;goto L8c653652; /* or #0xf0,r0 */
L8c653652: *stop=0x8c653652u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c653654; /* ldc r0,SR */
L8c653654: *stop=0x8c653654u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[2],0u);if(status)return status;goto L8c653656; /* stc VBR,r2 */
L8c653656: *stop=0x8c653656u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6536a2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c653658; /* mov.w 0x8c6536a2,r0 */
L8c653658: *stop=0x8c653658u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6536a4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65365a; /* mov.l 0x8c6536a4,r6 */
L8c65365a: *stop=0x8c65365au;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[4],4);if(io_status)return io_status;goto L8c65365c; /* mov.l r4,@(r0,r2) */
L8c65365c: *stop=0x8c65365cu;r[4]=0u;goto L8c65365e; /* mov #0x0,r4 */
L8c65365e: *stop=0x8c65365eu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c653660; /* mov.l r4,@r6 */
L8c653660: *stop=0x8c653660u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6536a8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c653662; /* mov.l 0x8c6536a8,r7 */
L8c653662: *stop=0x8c653662u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[4],4);if(io_status)return io_status;goto L8c653664; /* mov.l r4,@r7 */
L8c653664: *stop=0x8c653664u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6536acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c653666; /* mov.l 0x8c6536ac,r3 */
L8c653666: *stop=0x8c653666u;r[4]=4294967295u;goto L8c653668; /* mov #-0x1,r4 */
L8c653668: *stop=0x8c653668u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c65366a; /* mov.l r4,@r3 */
L8c65366a: *stop=0x8c65366au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6536b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65366c; /* mov.l 0x8c6536b0,r2 */
L8c65366c: *stop=0x8c65366cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c65366e; /* mov.l r4,@r2 */
L8c65366e: *stop=0x8c65366eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6536b4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c653670; /* mov.l 0x8c6536b4,r1 */
L8c653670: *stop=0x8c653670u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[1],4);if(io_status)return io_status;goto L8c653672; /* mov.l r1,@r6 */
L8c653672: *stop=0x8c653672u;r[0]=14u;goto L8c653674; /* mov #0xe,r0 */
L8c653674: *stop=0x8c653674u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[0],4);if(io_status)return io_status;goto L8c653676; /* mov.l r0,@r7 */
L8c653676: *stop=0x8c653676u;status=sa_sh4_stc_sr(*sr,&r[1],0u);if(status)return status;goto L8c653678; /* stc SR,r1 */
L8c653678: *stop=0x8c653678u;r[0]=r[5];goto L8c65367a; /* mov r5,r0 */
L8c65367a: *stop=0x8c65367au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6536a0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65367c; /* mov.w 0x8c6536a0,r3 */
L8c65367c: *stop=0x8c65367cu;r[0]&=0xfu;goto L8c65367e; /* and #0xf,r0 */
L8c65367e: *stop=0x8c65367eu;r[0]<<=2;goto L8c653680; /* shll2 r0 */
L8c653680: *stop=0x8c653680u;r[1]&=r[3];goto L8c653682; /* and r3,r1 */
L8c653682: *stop=0x8c653682u;r[0]<<=2;goto L8c653684; /* shll2 r0 */
L8c653684: *stop=0x8c653684u;r[0]|=r[1];goto L8c653686; /* or r1,r0 */
L8c653686: *stop=0x8c653686u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c653688; /* ldc r0,SR */
L8c653688: *stop=0x8c653688u;if(*pr!=entry_pr)return -3;*stop=0x8c65368au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c663080(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c663080;
L8c663080: *stop=0x8c663080u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c663082; /* mov.l r14,@-r15 */
L8c663082: *stop=0x8c663082u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c663084; /* mov.l r13,@-r15 */
L8c663084: *stop=0x8c663084u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663120u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c663086; /* mov.l 0x8c663120,r13 */
L8c663086: *stop=0x8c663086u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c663088; /* sts.l PR,@-r15 */
L8c663088: *stop=0x8c663088u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c66308a; /* mov.l @(0x8,r13),r4 */
L8c66308a: *stop=0x8c66308au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c66308c; /* tst r4,r4 */
L8c66308c: if((*sr&1u))goto L8c66309a;goto L8c66308e; /* bt 0x8c66309a */
L8c66308e: *stop=0x8c66308eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663124u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663090; /* mov.l 0x8c663124,r2 */
L8c663090: target=r[2];*pr=0x8c663094u;*stop=0x8c663092u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663094; /* jsr @r2 */
L8c663094: *stop=0x8c663094u;r[4]=r[14];goto L8c663096; /* mov r14,r4 */
L8c663096: *stop=0x8c663096u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c663098; /* tst r4,r4 */
L8c663098: if(!(*sr&1u))goto L8c66308e;goto L8c66309a; /* bf 0x8c66308e */
L8c66309a: *stop=0x8c66309au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}goto L8c66309c; /* mov.l @r13,r4 */
L8c66309c: *stop=0x8c66309cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c66309e; /* tst r4,r4 */
L8c66309e: if((*sr&1u))goto L8c6630ac;goto L8c6630a0; /* bt 0x8c6630ac */
L8c6630a0: *stop=0x8c6630a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6630a2; /* mov.l 0x8c663124,r3 */
L8c6630a2: target=r[3];*pr=0x8c6630a6u;*stop=0x8c6630a4u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6630a6; /* jsr @r3 */
L8c6630a6: *stop=0x8c6630a6u;r[4]=r[14];goto L8c6630a8; /* mov r14,r4 */
L8c6630a8: *stop=0x8c6630a8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6630aa; /* tst r4,r4 */
L8c6630aa: if(!(*sr&1u))goto L8c6630a0;goto L8c6630ac; /* bf 0x8c6630a0 */
L8c6630ac: *stop=0x8c6630acu;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6630ae; /* mov.l @(0x1c,r13),r4 */
L8c6630ae: *stop=0x8c6630aeu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6630b0; /* tst r4,r4 */
L8c6630b0: if((*sr&1u))goto L8c6630be;goto L8c6630b2; /* bt 0x8c6630be */
L8c6630b2: *stop=0x8c6630b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663124u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6630b4; /* mov.l 0x8c663124,r2 */
L8c6630b4: target=r[2];*pr=0x8c6630b8u;*stop=0x8c6630b6u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6630b8; /* jsr @r2 */
L8c6630b8: *stop=0x8c6630b8u;r[4]=r[14];goto L8c6630ba; /* mov r14,r4 */
L8c6630ba: *stop=0x8c6630bau;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6630bc; /* tst r4,r4 */
L8c6630bc: if(!(*sr&1u))goto L8c6630b2;goto L8c6630be; /* bf 0x8c6630b2 */
L8c6630be: *stop=0x8c6630beu;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6630c0; /* mov.l @(0x14,r13),r4 */
L8c6630c0: *stop=0x8c6630c0u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6630c2; /* tst r4,r4 */
L8c6630c2: if((*sr&1u))goto L8c6630d0;goto L8c6630c4; /* bt 0x8c6630d0 */
L8c6630c4: *stop=0x8c6630c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663124u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6630c6; /* mov.l 0x8c663124,r3 */
L8c6630c6: target=r[3];*pr=0x8c6630cau;*stop=0x8c6630c8u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6630ca; /* jsr @r3 */
L8c6630ca: *stop=0x8c6630cau;r[4]=r[14];goto L8c6630cc; /* mov r14,r4 */
L8c6630cc: *stop=0x8c6630ccu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6630ce; /* tst r4,r4 */
L8c6630ce: if(!(*sr&1u))goto L8c6630c4;goto L8c6630d0; /* bf 0x8c6630c4 */
L8c6630d0: *stop=0x8c6630d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66312cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6630d2; /* mov.l 0x8c66312c,r5 */
L8c6630d2: *stop=0x8c6630d2u;r[14]=0u;goto L8c6630d4; /* mov #0x0,r14 */
L8c6630d4: *stop=0x8c6630d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663128u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6630d6; /* mov.l 0x8c663128,r6 */
L8c6630d6: *stop=0x8c6630d6u;r[5]+=r[6];goto L8c6630d8; /* add r6,r5 */
L8c6630d8: *stop=0x8c6630d8u;r[4]=r[6];goto L8c6630da; /* mov r6,r4 */
L8c6630da: *stop=0x8c6630dau;if(!valid_access(r[4],2u))return -3;wr(m,bus,&io_status,r[4],r[14],2);if(io_status)return io_status;goto L8c6630dc; /* mov.w r14,@r4 */
L8c6630dc: *stop=0x8c6630dcu;r[4]+=24u;goto L8c6630de; /* add #0x18,r4 */
L8c6630de: *stop=0x8c6630deu;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c6630e0; /* cmp/hs r5,r4 */
L8c6630e0: if(!(*sr&1u))goto L8c6630da;goto L8c6630e2; /* bf 0x8c6630da */
L8c6630e2: *stop=0x8c6630e2u;r[2]=0u;goto L8c6630e4; /* mov #0x0,r2 */
L8c6630e4: *stop=0x8c6630e4u;r[4]=r[13];goto L8c6630e6; /* mov r13,r4 */
L8c6630e6: *stop=0x8c6630e6u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c6630e8; /* mov.l r14,@r4 */
L8c6630e8: *stop=0x8c6630e8u;r[3]=r[2];goto L8c6630ea; /* mov r2,r3 */
L8c6630ea: *stop=0x8c6630eau;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[14],4);if(io_status)return io_status;goto L8c6630ec; /* mov.l r14,@(0x4,r4) */
L8c6630ec: *stop=0x8c6630ecu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[14],4);if(io_status)return io_status;goto L8c6630ee; /* mov.l r14,@(0x8,r4) */
L8c6630ee: *stop=0x8c6630eeu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[14],4);if(io_status)return io_status;goto L8c6630f0; /* mov.l r14,@(0xc,r4) */
L8c6630f0: *stop=0x8c6630f0u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6630f2; /* mov.l r2,@(0x10,r4) */
L8c6630f2: *stop=0x8c6630f2u;r[4]+=20u;goto L8c6630f4; /* add #0x14,r4 */
L8c6630f4: *stop=0x8c6630f4u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c6630f6; /* mov.l r14,@r4 */
L8c6630f6: *stop=0x8c6630f6u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[14],4);if(io_status)return io_status;goto L8c6630f8; /* mov.l r14,@(0x4,r4) */
L8c6630f8: *stop=0x8c6630f8u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[14],4);if(io_status)return io_status;goto L8c6630fa; /* mov.l r14,@(0x8,r4) */
L8c6630fa: *stop=0x8c6630fau;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[14],4);if(io_status)return io_status;goto L8c6630fc; /* mov.l r14,@(0xc,r4) */
L8c6630fc: *stop=0x8c6630fcu;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6630fe; /* mov.l r2,@(0x10,r4) */
L8c6630fe: *stop=0x8c6630feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663130u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663100; /* mov.l 0x8c663130,r2 */
L8c663100: *stop=0x8c663100u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto L8c663102; /* mov.l @r2,r4 */
L8c663102: *stop=0x8c663102u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c663104; /* tst r4,r4 */
L8c663104: if((*sr&1u))goto L8c663112;goto L8c663106; /* bt 0x8c663112 */
L8c663106: *stop=0x8c663106u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663134u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663108; /* mov.l 0x8c663134,r2 */
L8c663108: target=r[2];*pr=0x8c66310cu;*stop=0x8c66310au;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[13]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66310c; /* jsr @r2 */
L8c66310c: *stop=0x8c66310cu;r[4]=r[13];goto L8c66310e; /* mov r13,r4 */
L8c66310e: *stop=0x8c66310eu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c663110; /* tst r4,r4 */
L8c663110: if(!(*sr&1u))goto L8c663106;goto L8c663112; /* bf 0x8c663106 */
L8c663112: *stop=0x8c663112u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663130u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663114; /* mov.l 0x8c663130,r2 */
L8c663114: *stop=0x8c663114u;r[0]=0u;goto L8c663116; /* mov #0x0,r0 */
L8c663116: *stop=0x8c663116u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c663118; /* mov.l r14,@r2 */
L8c663118: *stop=0x8c663118u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66311a; /* lds.l @r15+,PR */
L8c66311a: *stop=0x8c66311au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c66311c; /* mov.l @r15+,r13 */
L8c66311c: *stop=0x8c66311cu;if(*pr!=entry_pr)return -3;*stop=0x8c66311eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c664ac0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c664ac0;
L8c664ac0: *stop=0x8c664ac0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664b84u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c664ac2; /* mov.l 0x8c664b84,r7 */
L8c664ac2: *stop=0x8c664ac2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664b80u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c664ac4; /* mov.l 0x8c664b80,r6 */
L8c664ac4: *stop=0x8c664ac4u;r[7]+=r[6];goto L8c664ac6; /* add r6,r7 */
L8c664ac6: *stop=0x8c664ac6u;r[4]=r[6];goto L8c664ac8; /* mov r6,r4 */
L8c664ac8: *stop=0x8c664ac8u;*sr=(*sr&~1u)|(r[4]>=r[7]);goto L8c664aca; /* cmp/hs r7,r4 */
L8c664aca: *stop=0x8c664acau;r[5]=r[6];goto L8c664acc; /* mov r6,r5 */
L8c664acc: take=(*sr&1u);*stop=0x8c664aceu;r[6]=1u;if(take)goto L8c664ae6;goto L8c664ad0; /* bt/s 0x8c664ae6 */
L8c664ad0: *stop=0x8c664ad0u;if(!valid_access(r[4],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[2]=loaded;}goto L8c664ad2; /* mov.w @r4,r2 */
L8c664ad2: *stop=0x8c664ad2u;r[2]=r[2]&65535u;goto L8c664ad4; /* extu.w r2,r2 */
L8c664ad4: *stop=0x8c664ad4u;*sr=(*sr&~1u)|!(r[6]&r[2]);goto L8c664ad6; /* tst r6,r2 */
L8c664ad6: if(!(*sr&1u))goto L8c664ade;goto L8c664ad8; /* bf 0x8c664ade */
L8c664ad8: *stop=0x8c664ad8u;if(!valid_access(r[5],2u))return -3;wr(m,bus,&io_status,r[5],r[6],2);if(io_status)return io_status;goto L8c664ada; /* mov.w r6,@r5 */
L8c664ada: *stop=0x8c664adau;if(*pr!=entry_pr)return -3;*stop=0x8c664adcu;r[0]=r[5];return 0; /* rts */
L8c664ade: *stop=0x8c664adeu;r[4]+=24u;goto L8c664ae0; /* add #0x18,r4 */
L8c664ae0: *stop=0x8c664ae0u;*sr=(*sr&~1u)|(r[4]>=r[7]);goto L8c664ae2; /* cmp/hs r7,r4 */
L8c664ae2: take=!(*sr&1u);*stop=0x8c664ae4u;r[5]+=24u;if(take)goto L8c664ad0;goto L8c664ae6; /* bf/s 0x8c664ad0 */
L8c664ae6: *stop=0x8c664ae6u;r[0]=0u;goto L8c664ae8; /* mov #0x0,r0 */
L8c664ae8: *stop=0x8c664ae8u;if(*pr!=entry_pr)return -3;*stop=0x8c664aeau;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c650aa0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c650aa0;
L8c650aa0: *stop=0x8c650aa0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c650aa2; /* mov.l r13,@-r15 */
L8c650aa2: *stop=0x8c650aa2u;r[15]+=4294967292u;goto L8c650aa4; /* add #-0x4,r15 */
L8c650aa4: *stop=0x8c650aa4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b24u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c650aa6; /* mov.l 0x8c650b24,r5 */
L8c650aa6: *stop=0x8c650aa6u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}goto L8c650aa8; /* mov.l @r5,r3 */
L8c650aa8: *stop=0x8c650aa8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650aaa; /* mov.l 0x8c650b28,r2 */
L8c650aaa: *stop=0x8c650aaau;r[4]=0u;goto L8c650aac; /* mov #0x0,r4 */
L8c650aac: *stop=0x8c650aacu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c650aae; /* mov.l r4,@r2 */
L8c650aae: *stop=0x8c650aaeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650ab0; /* mov.l 0x8c650b2c,r3 */
L8c650ab0: *stop=0x8c650ab0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c650ab2; /* mov.l r4,@r3 */
L8c650ab2: *stop=0x8c650ab2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b30u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650ab4; /* mov.l 0x8c650b30,r1 */
L8c650ab4: *stop=0x8c650ab4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c650ab6; /* mov.l r4,@r1 */
L8c650ab6: *stop=0x8c650ab6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c650b20u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c650ab8; /* mov.w 0x8c650b20,r2 */
L8c650ab8: *stop=0x8c650ab8u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[2],4);if(io_status)return io_status;goto L8c650aba; /* mov.l r2,@r5 */
L8c650aba: *stop=0x8c650abau;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650abc; /* mov.l 0x8c650b38,r2 */
L8c650abc: *stop=0x8c650abcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b34u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650abe; /* mov.l 0x8c650b34,r0 */
L8c650abe: *stop=0x8c650abeu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c650ac0; /* mov.l r0,@r2 */
L8c650ac0: *stop=0x8c650ac0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650ac2; /* mov.l 0x8c650b3c,r3 */
L8c650ac2: *stop=0x8c650ac2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c650ac4; /* mov.l r4,@r3 */
L8c650ac4: *stop=0x8c650ac4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b40u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650ac6; /* mov.l 0x8c650b40,r1 */
L8c650ac6: *stop=0x8c650ac6u;r[5]=16u;goto L8c650ac8; /* mov #0x10,r5 */
L8c650ac8: *stop=0x8c650ac8u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c650aca; /* mov.l r5,@r1 */
L8c650aca: *stop=0x8c650acau;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b44u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650acc; /* mov.l 0x8c650b44,r2 */
L8c650acc: *stop=0x8c650accu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c650ace; /* mov.l r5,@r2 */
L8c650ace: *stop=0x8c650aceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b54u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c650ad0; /* mov.l 0x8c650b54,r0 */
L8c650ad0: *stop=0x8c650ad0u;r[1]=7u;goto L8c650ad2; /* mov #0x7,r1 */
L8c650ad2: *stop=0x8c650ad2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b48u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c650ad4; /* mov.l 0x8c650b48,r5 */
L8c650ad4: *stop=0x8c650ad4u;r[2]=1u;goto L8c650ad6; /* mov #0x1,r2 */
L8c650ad6: *stop=0x8c650ad6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b5cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c650ad8; /* mov.l 0x8c650b5c,r13 */
L8c650ad8: *stop=0x8c650ad8u;r[7]=8u;goto L8c650ada; /* mov #0x8,r7 */
L8c650ada: *stop=0x8c650adau;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c650adc; /* mov.l r4,@r5 */
L8c650adc: *stop=0x8c650adcu;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[4],4);if(io_status)return io_status;goto L8c650ade; /* mov.l r4,@(0x4,r5) */
L8c650ade: *stop=0x8c650adeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b4cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c650ae0; /* mov.l 0x8c650b4c,r5 */
L8c650ae0: *stop=0x8c650ae0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c650b22u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c650ae2; /* mov.w 0x8c650b22,r6 */
L8c650ae2: *stop=0x8c650ae2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b50u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650ae4; /* mov.l 0x8c650b50,r3 */
L8c650ae4: *stop=0x8c650ae4u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c650ae6; /* mov.l r4,@r5 */
L8c650ae6: *stop=0x8c650ae6u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[4],4);if(io_status)return io_status;goto L8c650ae8; /* mov.l r4,@(0x4,r5) */
L8c650ae8: *stop=0x8c650ae8u;r[5]=r[4];goto L8c650aea; /* mov r4,r5 */
L8c650aea: *stop=0x8c650aeau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c650aec; /* mov.l r4,@r3 */
L8c650aec: *stop=0x8c650aecu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c650aee; /* mov.l r1,@r0 */
L8c650aee: *stop=0x8c650aeeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c650b58u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650af0; /* mov.l 0x8c650b58,r1 */
L8c650af0: *stop=0x8c650af0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c650af2; /* mov.l r2,@r1 */
L8c650af2: *stop=0x8c650af2u;r[1]=r[13];goto L8c650af4; /* mov r13,r1 */
L8c650af4: *stop=0x8c650af4u;r[1]+=r[6];goto L8c650af6; /* add r6,r1 */
L8c650af6: *stop=0x8c650af6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c650af8; /* mov.l r13,@r15 */
L8c650af8: *stop=0x8c650af8u;r[2]=r[1];goto L8c650afa; /* mov r1,r2 */
L8c650afa: *stop=0x8c650afau;r[3]=r[13];goto L8c650afc; /* mov r13,r3 */
L8c650afc: *stop=0x8c650afcu;r[2]+=r[5];goto L8c650afe; /* add r5,r2 */
L8c650afe: *stop=0x8c650afeu;r[3]+=r[5];goto L8c650b00; /* add r5,r3 */
L8c650b00: *stop=0x8c650b00u;r[5]+=32u;goto L8c650b02; /* add #0x20,r5 */
L8c650b02: *stop=0x8c650b02u;if(!valid_access(r[3]+24u,4u))return -3;wr(m,bus,&io_status,r[3]+0x18u,r[4],4);if(io_status)return io_status;goto L8c650b04; /* mov.l r4,@(0x18,r3) */
L8c650b04: *stop=0x8c650b04u;*sr=(*sr&~1u)|(r[5]>=r[6]);goto L8c650b06; /* cmp/hs r6,r5 */
L8c650b06: take=!(*sr&1u);*stop=0x8c650b08u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[7],4);if(io_status)return io_status;if(take)goto L8c650af8;goto L8c650b0a; /* bf/s 0x8c650af8 */
L8c650b0a: *stop=0x8c650b0au;r[15]+=4u;goto L8c650b0c; /* add #0x4,r15 */
L8c650b0c: *stop=0x8c650b0cu;if(*pr!=entry_pr)return -3;*stop=0x8c650b0eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c665e00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c665e00;
L8c665e00: *stop=0x8c665e00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ef0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665e02; /* mov.l 0x8c665ef0,r3 */
L8c665e02: *stop=0x8c665e02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665eecu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665e04; /* mov.l 0x8c665eec,r5 */
L8c665e04: *stop=0x8c665e04u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e06; /* mov.l @r3,r0 */
L8c665e06: *stop=0x8c665e06u;r[0]<<=2;goto L8c665e08; /* shll2 r0 */
L8c665e08: *stop=0x8c665e08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ef4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665e0a; /* mov.l 0x8c665ef4,r1 */
L8c665e0a: *stop=0x8c665e0au;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665e0c; /* mov.l @(r0,r5),r2 */
L8c665e0c: *stop=0x8c665e0cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e0e; /* mov.l @r1,r0 */
L8c665e0e: *stop=0x8c665e0eu;r[0]<<=2;goto L8c665e10; /* shll2 r0 */
L8c665e10: *stop=0x8c665e10u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e12; /* mov.l @(r0,r5),r0 */
L8c665e12: *stop=0x8c665e12u;r[3]+=8u;goto L8c665e14; /* add #0x8,r3 */
L8c665e14: *stop=0x8c665e14u;r[2]+=r[0];goto L8c665e16; /* add r0,r2 */
L8c665e16: *stop=0x8c665e16u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e18; /* mov.l @r3,r0 */
L8c665e18: *stop=0x8c665e18u;r[0]<<=2;goto L8c665e1a; /* shll2 r0 */
L8c665e1a: *stop=0x8c665e1au;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e1c; /* mov.l @(r0,r5),r0 */
L8c665e1c: *stop=0x8c665e1cu;r[3]+=4u;goto L8c665e1e; /* add #0x4,r3 */
L8c665e1e: *stop=0x8c665e1eu;r[2]+=r[0];goto L8c665e20; /* add r0,r2 */
L8c665e20: *stop=0x8c665e20u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e22; /* mov.l @r3,r0 */
L8c665e22: *stop=0x8c665e22u;r[0]<<=2;goto L8c665e24; /* shll2 r0 */
L8c665e24: *stop=0x8c665e24u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e26; /* mov.l @(r0,r5),r0 */
L8c665e26: *stop=0x8c665e26u;r[2]+=r[0];goto L8c665e28; /* add r0,r2 */
L8c665e28: *stop=0x8c665e28u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e2a; /* mov.l @(0x4,r3),r0 */
L8c665e2a: *stop=0x8c665e2au;r[0]<<=2;goto L8c665e2c; /* shll2 r0 */
L8c665e2c: *stop=0x8c665e2cu;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e2e; /* mov.l @(r0,r5),r0 */
L8c665e2e: *stop=0x8c665e2eu;r[2]+=r[0];goto L8c665e30; /* add r0,r2 */
L8c665e30: *stop=0x8c665e30u;*macl=r[4]*r[2];goto L8c665e32; /* mul.l r4,r2 */
L8c665e32: *stop=0x8c665e32u;r[2]=*macl;goto L8c665e34; /* sts MACL,r2 */
L8c665e34: *stop=0x8c665e34u;r[4]=r[2];goto L8c665e36; /* mov r2,r4 */
L8c665e36: *stop=0x8c665e36u;r[4]<<=2;goto L8c665e38; /* shll2 r4 */
L8c665e38: *stop=0x8c665e38u;if(*pr!=entry_pr)return -3;*stop=0x8c665e3au;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c664aec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c664aec;
L8c664aec: *stop=0x8c664aecu;r[15]+=4294967292u;goto L8c664aee; /* add #-0x4,r15 */
L8c664aee: *stop=0x8c664aeeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c664b80u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c664af0; /* mov.l 0x8c664b80,r5 */
L8c664af0: *stop=0x8c664af0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664b84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664af2; /* mov.l 0x8c664b84,r3 */
L8c664af2: *stop=0x8c664af2u;r[1]=r[5];goto L8c664af4; /* mov r5,r1 */
L8c664af4: *stop=0x8c664af4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664b88u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c664af6; /* mov.l 0x8c664b88,r7 */
L8c664af6: *stop=0x8c664af6u;r[1]+=r[3];goto L8c664af8; /* add r3,r1 */
L8c664af8: *stop=0x8c664af8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c664afa; /* mov.l r5,@r15 */
L8c664afa: *stop=0x8c664afau;r[6]=r[5];goto L8c664afc; /* mov r5,r6 */
L8c664afc: *stop=0x8c664afcu;*sr=(*sr&~1u)|(r[4]==r[5]);goto L8c664afe; /* cmp/eq r4,r5 */
L8c664afe: if(!(*sr&1u))goto L8c664b06;goto L8c664b00; /* bf 0x8c664b06 */
L8c664b00: *stop=0x8c664b00u;if(!valid_access(r[6],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[2]=loaded;}goto L8c664b02; /* mov.w @r6,r2 */
L8c664b02: *stop=0x8c664b02u;r[2]&=r[7];goto L8c664b04; /* and r7,r2 */
L8c664b04: *stop=0x8c664b04u;if(!valid_access(r[6],2u))return -3;wr(m,bus,&io_status,r[6],r[2],2);if(io_status)return io_status;goto L8c664b06; /* mov.w r2,@r6 */
L8c664b06: *stop=0x8c664b06u;r[5]+=24u;goto L8c664b08; /* add #0x18,r5 */
L8c664b08: *stop=0x8c664b08u;*sr=(*sr&~1u)|(r[5]>=r[1]);goto L8c664b0a; /* cmp/hs r1,r5 */
L8c664b0a: take=!(*sr&1u);*stop=0x8c664b0cu;r[6]+=24u;if(take)goto L8c664afc;goto L8c664b0e; /* bf/s 0x8c664afc */
L8c664b0e: *stop=0x8c664b0eu;if(*pr!=entry_pr)return -3;*stop=0x8c664b10u;r[15]+=4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_driver_8c665440(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c665440;
L8c665440: *stop=0x8c665440u;r[15]+=4294967292u;goto L8c665442; /* add #-0x4,r15 */
L8c665442: *stop=0x8c665442u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6654c0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665444; /* mov.l 0x8c6654c0,r5 */
L8c665444: *stop=0x8c665444u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6654c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665446; /* mov.l 0x8c6654c8,r3 */
L8c665446: *stop=0x8c665446u;r[1]=r[5];goto L8c665448; /* mov r5,r1 */
L8c665448: *stop=0x8c665448u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6654c4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c66544a; /* mov.l 0x8c6654c4,r7 */
L8c66544a: *stop=0x8c66544au;r[1]+=r[3];goto L8c66544c; /* add r3,r1 */
L8c66544c: *stop=0x8c66544cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c66544e; /* mov.l r5,@r15 */
L8c66544e: *stop=0x8c66544eu;r[6]=r[5];goto L8c665450; /* mov r5,r6 */
L8c665450: *stop=0x8c665450u;*sr=(*sr&~1u)|(r[4]==r[5]);goto L8c665452; /* cmp/eq r4,r5 */
L8c665452: if(!(*sr&1u))goto L8c66545a;goto L8c665454; /* bf 0x8c66545a */
L8c665454: *stop=0x8c665454u;if(!valid_access(r[6],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[2]=loaded;}goto L8c665456; /* mov.w @r6,r2 */
L8c665456: *stop=0x8c665456u;r[2]&=r[7];goto L8c665458; /* and r7,r2 */
L8c665458: *stop=0x8c665458u;if(!valid_access(r[6],2u))return -3;wr(m,bus,&io_status,r[6],r[2],2);if(io_status)return io_status;goto L8c66545a; /* mov.w r2,@r6 */
L8c66545a: *stop=0x8c66545au;r[5]+=76u;goto L8c66545c; /* add #0x4c,r5 */
L8c66545c: *stop=0x8c66545cu;*sr=(*sr&~1u)|(r[5]>=r[1]);goto L8c66545e; /* cmp/hs r1,r5 */
L8c66545e: take=!(*sr&1u);*stop=0x8c665460u;r[6]+=76u;if(take)goto L8c665450;goto L8c665462; /* bf/s 0x8c665450 */
L8c665462: *stop=0x8c665462u;if(*pr!=entry_pr)return -3;*stop=0x8c665464u;r[15]+=4u;return 0; /* rts */
}
