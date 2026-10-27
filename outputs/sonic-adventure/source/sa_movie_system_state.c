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
SA_STAGE_EXPORT int sa_movie_system_state_8c5ff39c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c5ff39c;
L8c5ff39c: *stop=0x8c5ff39cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c5ff39e; /* mov.l r14,@-r15 */
L8c5ff39e: *stop=0x8c5ff39eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c5ff3a0; /* sts.l PR,@-r15 */
L8c5ff3a0: *stop=0x8c5ff3a0u;r[15]+=4294967292u;goto L8c5ff3a2; /* add #-0x4,r15 */
L8c5ff3a2: *stop=0x8c5ff3a2u;r[14]=r[4];goto L8c5ff3a4; /* mov r4,r14 */
L8c5ff3a4: *stop=0x8c5ff3a4u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c5ff3a6; /* tst r14,r14 */
L8c5ff3a6: if((*sr&1u))goto L8c5ff3c0;goto L8c5ff3a8; /* bt 0x8c5ff3c0 */
L8c5ff3a8: *stop=0x8c5ff3a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ff4b4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ff3aa; /* mov.l 0x8c5ff4b4,r3 */
L8c5ff3aa: *stop=0x8c5ff3aau;r[2]=0u;goto L8c5ff3ac; /* mov #0x0,r2 */
L8c5ff3ac: *stop=0x8c5ff3acu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c5ff3ae; /* mov.l r2,@r14 */
L8c5ff3ae: *stop=0x8c5ff3aeu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c5ff3b0; /* mov.l @r3,r5 */
L8c5ff3b0: *stop=0x8c5ff3b0u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c5ff3b2; /* tst r5,r5 */
L8c5ff3b2: if((*sr&1u))goto L8c5ff3c0;goto L8c5ff3b4; /* bt 0x8c5ff3c0 */
L8c5ff3b4: *stop=0x8c5ff3b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ff4bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ff3b6; /* mov.l 0x8c5ff4bc,r3 */
L8c5ff3b6: target=r[3];*pr=0x8c5ff3bau;*stop=0x8c5ff3b8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ff3ba; /* jsr @r3 */
L8c5ff3ba: *stop=0x8c5ff3bau;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c5ff3bc; /* cmp/eq #-0x1,r0 */
L8c5ff3bc: take=!(*sr&1u);*stop=0x8c5ff3beu;r[4]=r[0];if(take)goto L8c5ff3ca;goto L8c5ff3c0; /* bf/s 0x8c5ff3ca */
L8c5ff3c0: *stop=0x8c5ff3c0u;r[0]=4294967295u;goto L8c5ff3c2; /* mov #-0x1,r0 */
L8c5ff3c2: *stop=0x8c5ff3c2u;r[15]+=4u;goto L8c5ff3c4; /* add #0x4,r15 */
L8c5ff3c4: *stop=0x8c5ff3c4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ff3c6; /* lds.l @r15+,PR */
L8c5ff3c6: *stop=0x8c5ff3c6u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff3c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c5ff3ca: *stop=0x8c5ff3cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ff4c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ff3cc; /* mov.l 0x8c5ff4c0,r3 */
L8c5ff3cc: *stop=0x8c5ff3ccu;if(!valid_access(r[3],1u))return -3;r[5]=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;goto L8c5ff3ce; /* mov.b @r3,r5 */
L8c5ff3ce: *stop=0x8c5ff3ceu;r[4]=(uint32_t)(int32_t)(int8_t)r[5];goto L8c5ff3d0; /* exts.b r5,r4 */
L8c5ff3d0: *stop=0x8c5ff3d0u;r[0]=r[4];goto L8c5ff3d2; /* mov r4,r0 */
L8c5ff3d2: *stop=0x8c5ff3d2u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c5ff3d4; /* cmp/eq #-0x1,r0 */
L8c5ff3d4: if(!(*sr&1u))goto L8c5ff3ee;goto L8c5ff3d6; /* bf 0x8c5ff3ee */
L8c5ff3d6: target=0x8c5ff3fau;*pr=0x8c5ff3dau;*stop=0x8c5ff3d8u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ff3da; /* bsr 0x8c5ff3fa */
L8c5ff3da: *stop=0x8c5ff3dau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c5ff3dc; /* mov.l @r15,r0 */
L8c5ff3dc: *stop=0x8c5ff3dcu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c5ff3de; /* cmp/eq #-0x1,r0 */
L8c5ff3de: if(!(*sr&1u))goto L8c5ff3e6;goto L8c5ff3e0; /* bf 0x8c5ff3e6 */
L8c5ff3e0: *stop=0x8c5ff3e0u;r[2]=1u;goto L8c5ff3e2; /* mov #0x1,r2 */
L8c5ff3e2: *stop=0x8c5ff3e4u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c5ff3f0; /* bra 0x8c5ff3f0 */
L8c5ff3e6: target=0x8c5ff396u;*pr=0x8c5ff3eau;*stop=0x8c5ff3e8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ff3ea; /* bsr 0x8c5ff396 */
L8c5ff3ea: *stop=0x8c5ff3ecu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c5ff3f0; /* bra 0x8c5ff3f0 */
L8c5ff3ee: *stop=0x8c5ff3eeu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c5ff3f0; /* mov.l r4,@r14 */
L8c5ff3f0: *stop=0x8c5ff3f0u;r[0]=0u;goto L8c5ff3f2; /* mov #0x0,r0 */
L8c5ff3f2: *stop=0x8c5ff3f2u;r[15]+=4u;goto L8c5ff3f4; /* add #0x4,r15 */
L8c5ff3f4: *stop=0x8c5ff3f4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ff3f6; /* lds.l @r15+,PR */
L8c5ff3f6: *stop=0x8c5ff3f6u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff3f8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_system_state_8c5ff3fa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ff3fa;
L8c5ff3fa: *stop=0x8c5ff3fau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c5ff3fc; /* mov.l r14,@-r15 */
L8c5ff3fc: *stop=0x8c5ff3fcu;r[14]=r[4];goto L8c5ff3fe; /* mov r4,r14 */
L8c5ff3fe: *stop=0x8c5ff3feu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c5ff400; /* sts.l PR,@-r15 */
L8c5ff400: *stop=0x8c5ff400u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c5ff402; /* tst r14,r14 */
L8c5ff402: if((*sr&1u))goto L8c5ff410;goto L8c5ff404; /* bt 0x8c5ff410 */
L8c5ff404: *stop=0x8c5ff404u;r[3]=0u;goto L8c5ff406; /* mov #0x0,r3 */
L8c5ff406: target=0x8c5ff494u;*pr=0x8c5ff40au;*stop=0x8c5ff408u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ff40a; /* bsr 0x8c5ff494 */
L8c5ff40a: *stop=0x8c5ff40au;r[4]=r[0];goto L8c5ff40c; /* mov r0,r4 */
L8c5ff40c: *stop=0x8c5ff40cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ff40e; /* tst r4,r4 */
L8c5ff40e: if(!(*sr&1u))goto L8c5ff418;goto L8c5ff410; /* bf 0x8c5ff418 */
L8c5ff410: *stop=0x8c5ff410u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ff412; /* lds.l @r15+,PR */
L8c5ff412: *stop=0x8c5ff412u;r[0]=4294967295u;goto L8c5ff414; /* mov #-0x1,r0 */
L8c5ff414: *stop=0x8c5ff414u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff416u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c5ff418: *stop=0x8c5ff418u;if(!valid_access(r[4]+11u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0xbu,1);if(io_status)return io_status;goto L8c5ff41a; /* mov.b @(0xb,r4),r0 */
L8c5ff41a: *stop=0x8c5ff41au;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c5ff41c; /* mov.l r0,@r14 */
L8c5ff41c: *stop=0x8c5ff41cu;r[0]=0u;goto L8c5ff41e; /* mov #0x0,r0 */
L8c5ff41e: *stop=0x8c5ff41eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ff420; /* lds.l @r15+,PR */
L8c5ff420: *stop=0x8c5ff420u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff422u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_system_state_8c5ff396(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ff396;
L8c5ff396: *stop=0x8c5ff396u;r[0]=r[4];goto L8c5ff398; /* mov r4,r0 */
L8c5ff398: *stop=0x8c5ff398u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff39au;r[0]+=4294967248u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_system_state_8c5ff494(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ff494;
L8c5ff494: *stop=0x8c5ff494u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c5ff496; /* sts.l PR,@-r15 */
L8c5ff496: *stop=0x8c5ff496u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ff4c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ff498; /* mov.l 0x8c5ff4c4,r3 */
L8c5ff498: target=r[3];*pr=0x8c5ff49cu;*stop=0x8c5ff49au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ff49c; /* jsr @r3 */
L8c5ff49c: *stop=0x8c5ff49cu;r[4]=r[0];goto L8c5ff49e; /* mov r0,r4 */
L8c5ff49e: *stop=0x8c5ff49eu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ff4a0; /* tst r4,r4 */
L8c5ff4a0: if((*sr&1u))goto L8c5ff4a8;goto L8c5ff4a2; /* bt 0x8c5ff4a8 */
L8c5ff4a2: *stop=0x8c5ff4a2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ff4a4; /* lds.l @r15+,PR */
L8c5ff4a4: *stop=0x8c5ff4a4u;if(*pr!=entry_pr)return -3;*stop=0x8c5ff4a6u;r[0]=0u;return 0; /* rts */
L8c5ff4a8: *stop=0x8c5ff4a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ff4c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ff4aa; /* mov.l 0x8c5ff4c8,r3 */
L8c5ff4aa: target=r[3];*stop=0x8c5ff4acu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_system_state_8c04fe40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c04fe40;
L8c04fe40: *stop=0x8c04fe40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04fe8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c04fe42; /* mov.l 0x8c04fe8c,r3 */
L8c04fe42: *stop=0x8c04fe42u;if(!valid_access(r[3],2u))return -3;r[0]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c04fe44; /* mov.w @r3,r0 */
L8c04fe44: *stop=0x8c04fe44u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c04fe46; /* cmp/eq #0x10,r0 */
L8c04fe46: *stop=0x8c04fe46u;if(*pr!=entry_pr)return -3;*stop=0x8c04fe48u;r[0]=*sr&1u;return 0; /* rts */
}
