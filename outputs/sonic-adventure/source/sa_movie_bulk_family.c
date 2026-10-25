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
SA_STAGE_EXPORT int sa_movie_bulk_family_8c65a8c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a8c8;
L8c65a8c8: *stop=0x8c65a8c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a8ca; /* sts.l PR,@-r15 */
L8c65a8ca: *stop=0x8c65a8cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a8ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a8cc; /* mov.l 0x8c65a8ec,r3 */
L8c65a8cc: target=r[3];*pr=0x8c65a8d0u;*stop=0x8c65a8ceu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a8d0; /* jsr @r3 */
L8c65a8d0: *stop=0x8c65a8d0u;r[2]=1u;goto L8c65a8d2; /* mov #0x1,r2 */
L8c65a8d2: *stop=0x8c65a8d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a8f0u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c65a8d4; /* mov.l 0x8c65a8f0,r7 */
L8c65a8d4: *stop=0x8c65a8d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a8f4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a8d6; /* mov.l 0x8c65a8f4,r5 */
L8c65a8d6: *stop=0x8c65a8d6u;r[3]=0u;goto L8c65a8d8; /* mov #0x0,r3 */
L8c65a8d8: *stop=0x8c65a8d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a8f8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a8da; /* mov.l 0x8c65a8f8,r4 */
L8c65a8da: *stop=0x8c65a8dau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c65a8dc; /* mov.l r2,@-r15 */
L8c65a8dc: *stop=0x8c65a8dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a8fcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65a8de; /* mov.l 0x8c65a8fc,r1 */
L8c65a8de: *stop=0x8c65a8deu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65a8e0; /* mov.l r3,@-r15 */
L8c65a8e0: target=r[1];*pr=0x8c65a8e4u;*stop=0x8c65a8e2u;r[6]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a8e4; /* jsr @r1 */
L8c65a8e4: *stop=0x8c65a8e4u;r[15]+=8u;goto L8c65a8e6; /* add #0x8,r15 */
L8c65a8e6: *stop=0x8c65a8e6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a8e8; /* lds.l @r15+,PR */
L8c65a8e8: *stop=0x8c65a8e8u;if(*pr!=entry_pr)return -3;*stop=0x8c65a8eau;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_bulk_family_8c65a27c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a27c;
L8c65a27c: *stop=0x8c65a27cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a27e; /* mov.l r14,@-r15 */
L8c65a27e: *stop=0x8c65a27eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a280; /* sts.l PR,@-r15 */
L8c65a280: *stop=0x8c65a280u;r[15]+=4294967292u;goto L8c65a282; /* add #-0x4,r15 */
L8c65a282: *stop=0x8c65a282u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65a284; /* stc SR,r0 */
L8c65a284: *stop=0x8c65a284u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a2e6u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65a286; /* mov.w 0x8c65a2e6,r3 */
L8c65a286: *stop=0x8c65a286u;r[0]>>=2;goto L8c65a288; /* shlr2 r0 */
L8c65a288: *stop=0x8c65a288u;r[0]>>=2;goto L8c65a28a; /* shlr2 r0 */
L8c65a28a: *stop=0x8c65a28au;r[0]&=0xfu;goto L8c65a28c; /* and #0xf,r0 */
L8c65a28c: *stop=0x8c65a28cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c65a28e; /* mov.l r0,@r15 */
L8c65a28e: *stop=0x8c65a28eu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65a290; /* stc SR,r0 */
L8c65a290: *stop=0x8c65a290u;r[0]&=r[3];goto L8c65a292; /* and r3,r0 */
L8c65a292: *stop=0x8c65a292u;r[0]|=0xf0u;goto L8c65a294; /* or #0xf0,r0 */
L8c65a294: *stop=0x8c65a294u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a296; /* ldc r0,SR */
L8c65a296: *stop=0x8c65a296u;r[14]=1u;goto L8c65a298; /* mov #0x1,r14 */
L8c65a298: *stop=0x8c65a298u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a2f0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a29a; /* mov.l 0x8c65a2f0,r4 */
L8c65a29a: *stop=0x8c65a29au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a2ecu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a29c; /* mov.l 0x8c65a2ec,r5 */
L8c65a29c: *stop=0x8c65a29cu;r[7]=r[14];goto L8c65a29e; /* mov r14,r7 */
L8c65a29e: *stop=0x8c65a29eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a2a0; /* mov.l r14,@-r15 */
L8c65a2a0: *stop=0x8c65a2a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a2f4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a2a2; /* mov.l 0x8c65a2f4,r2 */
L8c65a2a2: *stop=0x8c65a2a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a2a4; /* mov.l r14,@-r15 */
L8c65a2a4: target=r[2];*pr=0x8c65a2a8u;*stop=0x8c65a2a6u;r[6]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a2a8; /* jsr @r2 */
L8c65a2a8: *stop=0x8c65a2a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a2ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a2aa; /* mov.l 0x8c65a2ec,r3 */
L8c65a2aa: *stop=0x8c65a2aau;r[2]=4294967294u;goto L8c65a2ac; /* mov #-0x2,r2 */
L8c65a2ac: *stop=0x8c65a2acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a2f0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a2ae; /* mov.l 0x8c65a2f0,r5 */
L8c65a2ae: *stop=0x8c65a2aeu;r[7]=r[14];goto L8c65a2b0; /* mov r14,r7 */
L8c65a2b0: *stop=0x8c65a2b0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65a2b2; /* mov.l @r3,r1 */
L8c65a2b2: *stop=0x8c65a2b2u;r[4]=r[3];goto L8c65a2b4; /* mov r3,r4 */
L8c65a2b4: *stop=0x8c65a2b4u;r[1]&=r[2];goto L8c65a2b6; /* and r2,r1 */
L8c65a2b6: *stop=0x8c65a2b6u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c65a2b8; /* mov.l r1,@r3 */
L8c65a2b8: *stop=0x8c65a2b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a2f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a2ba; /* mov.l 0x8c65a2f8,r3 */
L8c65a2ba: *stop=0x8c65a2bau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a2bc; /* mov.l r14,@-r15 */
L8c65a2bc: *stop=0x8c65a2bcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a2be; /* mov.l r14,@-r15 */
L8c65a2be: target=r[3];*pr=0x8c65a2c2u;*stop=0x8c65a2c0u;r[6]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a2c2; /* jsr @r3 */
L8c65a2c2: *stop=0x8c65a2c2u;r[15]+=16u;goto L8c65a2c4; /* add #0x10,r15 */
L8c65a2c4: *stop=0x8c65a2c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a2c6; /* mov.l @r15,r0 */
L8c65a2c6: *stop=0x8c65a2c6u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c65a2c8; /* stc SR,r2 */
L8c65a2c8: *stop=0x8c65a2c8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a2e6u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65a2ca; /* mov.w 0x8c65a2e6,r3 */
L8c65a2ca: *stop=0x8c65a2cau;r[0]&=0xfu;goto L8c65a2cc; /* and #0xf,r0 */
L8c65a2cc: *stop=0x8c65a2ccu;r[0]<<=2;goto L8c65a2ce; /* shll2 r0 */
L8c65a2ce: *stop=0x8c65a2ceu;r[2]&=r[3];goto L8c65a2d0; /* and r3,r2 */
L8c65a2d0: *stop=0x8c65a2d0u;r[0]<<=2;goto L8c65a2d2; /* shll2 r0 */
L8c65a2d2: *stop=0x8c65a2d2u;r[0]|=r[2];goto L8c65a2d4; /* or r2,r0 */
L8c65a2d4: *stop=0x8c65a2d4u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a2d6; /* ldc r0,SR */
L8c65a2d6: *stop=0x8c65a2d6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a2e8u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65a2d8; /* mov.w 0x8c65a2e8,r4 */
L8c65a2d8: *stop=0x8c65a2d8u;r[4]--;*sr=(*sr&~1u)|(!r[4]);goto L8c65a2da; /* dt r4 */
L8c65a2da: if(!(*sr&1u))goto L8c65a2d8;goto L8c65a2dc; /* bf 0x8c65a2d8 */
L8c65a2dc: *stop=0x8c65a2dcu;r[0]=0u;goto L8c65a2de; /* mov #0x0,r0 */
L8c65a2de: *stop=0x8c65a2deu;r[15]+=4u;goto L8c65a2e0; /* add #0x4,r15 */
L8c65a2e0: *stop=0x8c65a2e0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a2e2; /* lds.l @r15+,PR */
L8c65a2e2: *stop=0x8c65a2e2u;if(*pr!=entry_pr)return -3;*stop=0x8c65a2e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_bulk_family_8c65a5dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65a5dc;
L8c65a5dc: *stop=0x8c65a5dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a5de; /* mov.l r14,@-r15 */
L8c65a5de: *stop=0x8c65a5deu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65a5e0; /* mov.l r13,@-r15 */
L8c65a5e0: *stop=0x8c65a5e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65a5e2; /* mov.l r12,@-r15 */
L8c65a5e2: *stop=0x8c65a5e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65a5e4; /* mov.l r11,@-r15 */
L8c65a5e4: *stop=0x8c65a5e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65a5e6; /* mov.l r10,@-r15 */
L8c65a5e6: *stop=0x8c65a5e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65a5e8; /* mov.l r9,@-r15 */
L8c65a5e8: *stop=0x8c65a5e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a5ea; /* sts.l PR,@-r15 */
L8c65a5ea: *stop=0x8c65a5eau;r[15]+=4294967280u;goto L8c65a5ec; /* add #-0x10,r15 */
L8c65a5ec: *stop=0x8c65a5ecu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65a5ee; /* stc SR,r0 */
L8c65a5ee: *stop=0x8c65a5eeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a6b2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65a5f0; /* mov.w 0x8c65a6b2,r3 */
L8c65a5f0: *stop=0x8c65a5f0u;r[9]=0u;goto L8c65a5f2; /* mov #0x0,r9 */
L8c65a5f2: *stop=0x8c65a5f2u;r[11]=r[4];goto L8c65a5f4; /* mov r4,r11 */
L8c65a5f4: *stop=0x8c65a5f4u;r[13]=r[11];goto L8c65a5f6; /* mov r11,r13 */
L8c65a5f6: *stop=0x8c65a5f6u;r[0]>>=2;goto L8c65a5f8; /* shlr2 r0 */
L8c65a5f8: *stop=0x8c65a5f8u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c65a5fa; /* mov.l r5,@(0xc,r15) */
L8c65a5fa: *stop=0x8c65a5fau;r[0]>>=2;goto L8c65a5fc; /* shlr2 r0 */
L8c65a5fc: *stop=0x8c65a5fcu;r[14]=r[9];goto L8c65a5fe; /* mov r9,r14 */
L8c65a5fe: *stop=0x8c65a5feu;r[0]&=0xfu;goto L8c65a600; /* and #0xf,r0 */
L8c65a600: *stop=0x8c65a600u;r[12]=r[0];goto L8c65a602; /* mov r0,r12 */
L8c65a602: *stop=0x8c65a602u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65a604; /* stc SR,r0 */
L8c65a604: *stop=0x8c65a604u;r[0]&=r[3];goto L8c65a606; /* and r3,r0 */
L8c65a606: *stop=0x8c65a606u;r[0]|=0xf0u;goto L8c65a608; /* or #0xf0,r0 */
L8c65a608: *stop=0x8c65a608u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a60a; /* ldc r0,SR */
L8c65a60a: *stop=0x8c65a60au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6b8u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c65a60c; /* mov.l 0x8c65a6b8,r10 */
L8c65a60c: *stop=0x8c65a60cu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a60e; /* mov.l @r10,r2 */
L8c65a60e: *stop=0x8c65a60eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65a610; /* tst r2,r2 */
L8c65a610: if(!(*sr&1u))goto L8c65a618;goto L8c65a612; /* bf 0x8c65a618 */
L8c65a612: *stop=0x8c65a612u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6bcu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a614; /* mov.l 0x8c65a6bc,r14 */
L8c65a614: *stop=0x8c65a616u;goto L8c65a6ee; /* bra 0x8c65a6ee */
L8c65a618: *stop=0x8c65a618u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c65a61a; /* tst r11,r11 */
L8c65a61a: if(!(*sr&1u))goto L8c65a622;goto L8c65a61c; /* bf 0x8c65a622 */
L8c65a61c: *stop=0x8c65a61cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6c0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a61e; /* mov.l 0x8c65a6c0,r14 */
L8c65a61e: *stop=0x8c65a620u;goto L8c65a6ee; /* bra 0x8c65a6ee */
L8c65a622: *stop=0x8c65a622u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a624; /* mov.l 0x8c65a6c4,r3 */
L8c65a624: *stop=0x8c65a624u;r[5]=r[15];goto L8c65a626; /* mov r15,r5 */
L8c65a626: *stop=0x8c65a626u;r[5]+=8u;goto L8c65a628; /* add #0x8,r5 */
L8c65a628: target=r[3];*pr=0x8c65a62cu;*stop=0x8c65a62au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a62c; /* jsr @r3 */
L8c65a62c: *stop=0x8c65a62cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a62e; /* mov.l @(0x8,r15),r0 */
L8c65a62e: *stop=0x8c65a62eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c65a630; /* cmp/eq #0x2,r0 */
L8c65a630: if(!(*sr&1u))goto L8c65a638;goto L8c65a632; /* bf 0x8c65a638 */
L8c65a632: *stop=0x8c65a632u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6c8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a634; /* mov.l 0x8c65a6c8,r14 */
L8c65a634: *stop=0x8c65a636u;goto L8c65a6ee; /* bra 0x8c65a6ee */
L8c65a638: *stop=0x8c65a638u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a6b4u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65a63a; /* mov.w 0x8c65a6b4,r1 */
L8c65a63a: *stop=0x8c65a63au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a63c; /* mov.l 0x8c65a6cc,r2 */
L8c65a63c: *stop=0x8c65a63cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[9],4);if(io_status)return io_status;goto L8c65a63e; /* mov.l r9,@r2 */
L8c65a63e: *stop=0x8c65a63eu;if(!valid_access(r[13]+24u,4u))return -3;wr(m,bus,&io_status,r[13]+0x18u,r[9],4);if(io_status)return io_status;goto L8c65a640; /* mov.l r9,@(0x18,r13) */
L8c65a640: *stop=0x8c65a640u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a642; /* mov.l @r10,r0 */
L8c65a642: *stop=0x8c65a642u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a644; /* mov.l @(r0,r1),r0 */
L8c65a644: *stop=0x8c65a644u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c65a646; /* cmp/eq #0x0,r0 */
L8c65a646: if((*sr&1u))goto L8c65a650;goto L8c65a648; /* bt 0x8c65a650 */
L8c65a648: *stop=0x8c65a648u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65a64a; /* cmp/eq #0x1,r0 */
L8c65a64a: if((*sr&1u))goto L8c65a662;goto L8c65a64c; /* bt 0x8c65a662 */
L8c65a64c: *stop=0x8c65a64eu;goto L8c65a6ee; /* bra 0x8c65a6ee */
L8c65a650: *stop=0x8c65a650u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a652; /* mov.l 0x8c65a6d0,r3 */
L8c65a652: *stop=0x8c65a652u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a654; /* mov.l @(0xc,r15),r5 */
L8c65a654: target=r[3];*pr=0x8c65a658u;*stop=0x8c65a656u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a658; /* jsr @r3 */
L8c65a658: *stop=0x8c65a658u;r[14]=r[0];goto L8c65a65a; /* mov r0,r14 */
L8c65a65a: *stop=0x8c65a65au;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a65c; /* tst r14,r14 */
L8c65a65c: if((*sr&1u))goto L8c65a6fa;goto L8c65a65e; /* bt 0x8c65a6fa */
L8c65a65e: *stop=0x8c65a660u;goto L8c65a6ee; /* bra 0x8c65a6ee */
L8c65a662: *stop=0x8c65a662u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a664; /* mov.l 0x8c65a6d4,r3 */
L8c65a664: *stop=0x8c65a664u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a666; /* mov.l @(0xc,r15),r5 */
L8c65a666: target=r[3];*pr=0x8c65a66au;*stop=0x8c65a668u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a66a; /* jsr @r3 */
L8c65a66a: *stop=0x8c65a66au;r[14]=r[0];goto L8c65a66c; /* mov r0,r14 */
L8c65a66c: *stop=0x8c65a66cu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a66e; /* tst r14,r14 */
L8c65a66e: if(!(*sr&1u))goto L8c65a6ee;goto L8c65a670; /* bf 0x8c65a6ee */
L8c65a670: *stop=0x8c65a670u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a6b6u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65a672; /* mov.w 0x8c65a6b6,r0 */
L8c65a672: *stop=0x8c65a672u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a674; /* mov.l @r10,r2 */
L8c65a674: *stop=0x8c65a674u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a676; /* mov.l @(r0,r2),r3 */
L8c65a676: *stop=0x8c65a676u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65a678; /* tst r3,r3 */
L8c65a678: if(!(*sr&1u))goto L8c65a6ee;goto L8c65a67a; /* bf 0x8c65a6ee */
L8c65a67a: *stop=0x8c65a67au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a67c; /* mov.l 0x8c65a6d8,r2 */
L8c65a67c: *stop=0x8c65a67cu;r[5]=r[15];goto L8c65a67e; /* mov r15,r5 */
L8c65a67e: *stop=0x8c65a67eu;r[5]+=4u;goto L8c65a680; /* add #0x4,r5 */
L8c65a680: target=r[2];*pr=0x8c65a684u;*stop=0x8c65a682u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a684; /* jsr @r2 */
L8c65a684: *stop=0x8c65a684u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a686; /* mov.l 0x8c65a6dc,r3 */
L8c65a686: *stop=0x8c65a686u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a688; /* mov.l @(0x4,r15),r5 */
L8c65a688: target=r[3];*pr=0x8c65a68cu;*stop=0x8c65a68au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a68c; /* jsr @r3 */
L8c65a68c: *stop=0x8c65a68cu;r[14]=r[0];goto L8c65a68e; /* mov r0,r14 */
L8c65a68e: *stop=0x8c65a68eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a690; /* tst r14,r14 */
L8c65a690: if(!(*sr&1u))goto L8c65a6ee;goto L8c65a692; /* bf 0x8c65a6ee */
L8c65a692: *stop=0x8c65a692u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c65a694; /* stc SR,r2 */
L8c65a694: *stop=0x8c65a694u;r[0]=r[12];goto L8c65a696; /* mov r12,r0 */
L8c65a696: *stop=0x8c65a696u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a6b2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65a698; /* mov.w 0x8c65a6b2,r3 */
L8c65a698: *stop=0x8c65a698u;r[0]&=0xfu;goto L8c65a69a; /* and #0xf,r0 */
L8c65a69a: *stop=0x8c65a69au;r[0]<<=2;goto L8c65a69c; /* shll2 r0 */
L8c65a69c: *stop=0x8c65a69cu;r[2]&=r[3];goto L8c65a69e; /* and r3,r2 */
L8c65a69e: *stop=0x8c65a69eu;r[0]<<=2;goto L8c65a6a0; /* shll2 r0 */
L8c65a6a0: *stop=0x8c65a6a0u;r[0]|=r[2];goto L8c65a6a2; /* or r2,r0 */
L8c65a6a2: *stop=0x8c65a6a2u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a6a4; /* ldc r0,SR */
L8c65a6a4: *stop=0x8c65a6a4u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a6a6; /* mov.l @(0x10,r13),r0 */
L8c65a6a6: *stop=0x8c65a6a6u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c65a6a8; /* cmp/eq #-0x1,r0 */
L8c65a6a8: take=!(*sr&1u);*stop=0x8c65a6aau;r[12]=4294967295u;if(take)goto L8c65a6ee;goto L8c65a6ac; /* bf/s 0x8c65a6ee */
L8c65a6ac: *stop=0x8c65a6acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a6e0u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c65a6ae; /* mov.l 0x8c65a6e0,r11 */
L8c65a6ae: *stop=0x8c65a6b0u;goto L8c65a6e8; /* bra 0x8c65a6e8 */
L8c65a6e4: target=r[11];*pr=0x8c65a6e8u;*stop=0x8c65a6e6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a6e8; /* jsr @r11 */
L8c65a6e8: *stop=0x8c65a6e8u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a6ea; /* mov.l @(0x18,r13),r0 */
L8c65a6ea: *stop=0x8c65a6eau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65a6ec; /* cmp/eq #0x1,r0 */
L8c65a6ec: if(!(*sr&1u))goto L8c65a6e4;goto L8c65a6ee; /* bf 0x8c65a6e4 */
L8c65a6ee: *stop=0x8c65a6eeu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a6f0; /* tst r14,r14 */
L8c65a6f0: if((*sr&1u))goto L8c65a6fa;goto L8c65a6f2; /* bt 0x8c65a6fa */
L8c65a6f2: *stop=0x8c65a6f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a72cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a6f4; /* mov.l 0x8c65a72c,r3 */
L8c65a6f4: *stop=0x8c65a6f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a728u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a6f6; /* mov.l 0x8c65a728,r4 */
L8c65a6f6: target=r[3];*pr=0x8c65a6fau;*stop=0x8c65a6f8u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a6fa; /* jsr @r3 */
L8c65a6fa: *stop=0x8c65a6fau;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c65a6fc; /* cmp/pz r12 */
L8c65a6fc: if(!(*sr&1u))goto L8c65a710;goto L8c65a6fe; /* bf 0x8c65a710 */
L8c65a6fe: *stop=0x8c65a6feu;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c65a700; /* stc SR,r3 */
L8c65a700: *stop=0x8c65a700u;r[0]=r[12];goto L8c65a702; /* mov r12,r0 */
L8c65a702: *stop=0x8c65a702u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a724u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c65a704; /* mov.w 0x8c65a724,r2 */
L8c65a704: *stop=0x8c65a704u;r[0]&=0xfu;goto L8c65a706; /* and #0xf,r0 */
L8c65a706: *stop=0x8c65a706u;r[0]<<=2;goto L8c65a708; /* shll2 r0 */
L8c65a708: *stop=0x8c65a708u;r[3]&=r[2];goto L8c65a70a; /* and r2,r3 */
L8c65a70a: *stop=0x8c65a70au;r[0]<<=2;goto L8c65a70c; /* shll2 r0 */
L8c65a70c: *stop=0x8c65a70cu;r[0]|=r[3];goto L8c65a70e; /* or r3,r0 */
L8c65a70e: *stop=0x8c65a70eu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a710; /* ldc r0,SR */
L8c65a710: *stop=0x8c65a710u;r[0]=r[14];goto L8c65a712; /* mov r14,r0 */
L8c65a712: *stop=0x8c65a712u;r[15]+=16u;goto L8c65a714; /* add #0x10,r15 */
L8c65a714: *stop=0x8c65a714u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a716; /* lds.l @r15+,PR */
L8c65a716: *stop=0x8c65a716u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65a718; /* mov.l @r15+,r9 */
L8c65a718: *stop=0x8c65a718u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65a71a; /* mov.l @r15+,r10 */
L8c65a71a: *stop=0x8c65a71au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65a71c; /* mov.l @r15+,r11 */
L8c65a71c: *stop=0x8c65a71cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65a71e; /* mov.l @r15+,r12 */
L8c65a71e: *stop=0x8c65a71eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65a720; /* mov.l @r15+,r13 */
L8c65a720: *stop=0x8c65a720u;if(*pr!=entry_pr)return -3;*stop=0x8c65a722u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_bulk_family_8c65aaf4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65aaf4;
L8c65aaf4: *stop=0x8c65aaf4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65aaf6; /* mov.l r14,@-r15 */
L8c65aaf6: *stop=0x8c65aaf6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65aaf8; /* mov.l r13,@-r15 */
L8c65aaf8: *stop=0x8c65aaf8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65aafa; /* mov.l r12,@-r15 */
L8c65aafa: *stop=0x8c65aafau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65aafc; /* mov.l r11,@-r15 */
L8c65aafc: *stop=0x8c65aafcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65aafe; /* mov.l r10,@-r15 */
L8c65aafe: *stop=0x8c65aafeu;r[10]=0u;goto L8c65ab00; /* mov #0x0,r10 */
L8c65ab00: *stop=0x8c65ab00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65ab02; /* mov.l r9,@-r15 */
L8c65ab02: *stop=0x8c65ab02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65ab04; /* sts.l PR,@-r15 */
L8c65ab04: *stop=0x8c65ab04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ab70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ab06; /* mov.l 0x8c65ab70,r3 */
L8c65ab06: *stop=0x8c65ab06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ab74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ab08; /* mov.l 0x8c65ab74,r2 */
L8c65ab08: *stop=0x8c65ab08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ab6cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c65ab0a; /* mov.l 0x8c65ab6c,r12 */
L8c65ab0a: *stop=0x8c65ab0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65ab0c; /* mov.l r3,@-r15 */
L8c65ab0c: target=r[12];*pr=0x8c65ab10u;*stop=0x8c65ab0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ab10; /* jsr @r12 */
L8c65ab10: *stop=0x8c65ab10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ab78u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65ab12; /* mov.l 0x8c65ab78,r14 */
L8c65ab12: *stop=0x8c65ab12u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c65ab14; /* mov.l @r14,r4 */
L8c65ab14: *stop=0x8c65ab14u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c65ab16; /* tst r4,r4 */
L8c65ab16: take=!(*sr&1u);*stop=0x8c65ab18u;r[15]+=8u;if(take)goto L8c65ab20;goto L8c65ab1a; /* bf/s 0x8c65ab20 */
L8c65ab1a: *stop=0x8c65ab1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ab7cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c65ab1c; /* mov.l 0x8c65ab7c,r10 */
L8c65ab1c: *stop=0x8c65ab1eu;goto L8c65ac46; /* bra 0x8c65ac46 */
L8c65ab20: *stop=0x8c65ab20u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ab62u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ab22; /* mov.w 0x8c65ab62,r0 */
L8c65ab22: *stop=0x8c65ab22u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ab64u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65ab24; /* mov.w 0x8c65ab64,r3 */
L8c65ab24: *stop=0x8c65ab24u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[13]=loaded;}goto L8c65ab26; /* mov.l @(r0,r4),r13 */
L8c65ab26: *stop=0x8c65ab26u;r[0]+=4294967268u;goto L8c65ab28; /* add #-0x1c,r0 */
L8c65ab28: *stop=0x8c65ab28u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ab80u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c65ab2a; /* mov.l 0x8c65ab80,r11 */
L8c65ab2a: *stop=0x8c65ab2au;r[13]+=32u;goto L8c65ab2c; /* add #0x20,r13 */
L8c65ab2c: *stop=0x8c65ab2cu;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[13],4);if(io_status)return io_status;goto L8c65ab2e; /* mov.l r13,@(r0,r4) */
L8c65ab2e: *stop=0x8c65ab2eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65ab30; /* mov.l @r14,r5 */
L8c65ab30: *stop=0x8c65ab30u;r[5]+=r[3];goto L8c65ab32; /* add r3,r5 */
L8c65ab32: target=r[11];*pr=0x8c65ab36u;*stop=0x8c65ab34u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ab36; /* jsr @r11 */
L8c65ab36: *stop=0x8c65ab36u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ab66u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65ab38; /* mov.w 0x8c65ab66,r3 */
L8c65ab38: *stop=0x8c65ab38u;r[4]=r[13];goto L8c65ab3a; /* mov r13,r4 */
L8c65ab3a: *stop=0x8c65ab3au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65ab3c; /* mov.l @r14,r5 */
L8c65ab3c: *stop=0x8c65ab3cu;r[5]+=r[3];goto L8c65ab3e; /* add r3,r5 */
L8c65ab3e: target=r[11];*pr=0x8c65ab42u;*stop=0x8c65ab40u;r[4]+=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ab42; /* jsr @r11 */
L8c65ab42: *stop=0x8c65ab42u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[9]=loaded;}goto L8c65ab44; /* mov.l @r14,r9 */
L8c65ab44: *stop=0x8c65ab44u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ab64u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65ab46; /* mov.w 0x8c65ab64,r1 */
L8c65ab46: *stop=0x8c65ab46u;r[0]=r[9];goto L8c65ab48; /* mov r9,r0 */
L8c65ab48: *stop=0x8c65ab48u;if(!valid_access(r[0]+r[1],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c65ab4a; /* mov.b @(r0,r1),r0 */
L8c65ab4a: *stop=0x8c65ab4au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65ab4c; /* cmp/eq #0x1,r0 */
L8c65ab4c: if(!(*sr&1u))goto L8c65ab84;goto L8c65ab4e; /* bf 0x8c65ab84 */
L8c65ab4e: *stop=0x8c65ab4eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ab66u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ab50; /* mov.w 0x8c65ab66,r0 */
L8c65ab50: *stop=0x8c65ab50u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ab52; /* mov.l @r14,r2 */
L8c65ab52: *stop=0x8c65ab52u;if(!valid_access(r[0]+r[2],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[2]+r[0],1);if(io_status)return io_status;r[3]=loaded;}goto L8c65ab54; /* mov.b @(r0,r2),r3 */
L8c65ab54: *stop=0x8c65ab54u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65ab56; /* tst r3,r3 */
L8c65ab56: if(!(*sr&1u))goto L8c65ab84;goto L8c65ab58; /* bf 0x8c65ab84 */
L8c65ab58: *stop=0x8c65ab58u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ab68u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c65ab5a; /* mov.w 0x8c65ab68,r5 */
L8c65ab5a: *stop=0x8c65ab5au;r[4]=r[13];goto L8c65ab5c; /* mov r13,r4 */
L8c65ab5c: *stop=0x8c65ab5cu;r[5]+=r[9];goto L8c65ab5e; /* add r9,r5 */
L8c65ab5e: *stop=0x8c65ab60u;r[4]+=3u;goto L8c65ab8c; /* bra 0x8c65ab8c */
L8c65ab84: *stop=0x8c65ab84u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac64u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c65ab86; /* mov.w 0x8c65ac64,r5 */
L8c65ab86: *stop=0x8c65ab86u;r[4]=r[13];goto L8c65ab88; /* mov r13,r4 */
L8c65ab88: *stop=0x8c65ab88u;r[4]+=2u;goto L8c65ab8a; /* add #0x2,r4 */
L8c65ab8a: *stop=0x8c65ab8au;r[5]+=r[9];goto L8c65ab8c; /* add r9,r5 */
L8c65ab8c: target=r[11];*pr=0x8c65ab90u;*stop=0x8c65ab8eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ab90; /* jsr @r11 */
L8c65ab90: *stop=0x8c65ab90u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac66u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ab92; /* mov.w 0x8c65ac66,r0 */
L8c65ab92: *stop=0x8c65ab92u;r[3]=0u;goto L8c65ab94; /* mov #0x0,r3 */
L8c65ab94: *stop=0x8c65ab94u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ab96; /* mov.l @r14,r2 */
L8c65ab96: *stop=0x8c65ab96u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac6au,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65ab98; /* mov.w 0x8c65ac6a,r4 */
L8c65ab98: *stop=0x8c65ab98u;if(!valid_access(r[0]+r[2],1u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[3],1);if(io_status)return io_status;goto L8c65ab9a; /* mov.b r3,@(r0,r2) */
L8c65ab9a: *stop=0x8c65ab9au;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac68u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65ab9c; /* mov.w 0x8c65ac68,r3 */
L8c65ab9c: *stop=0x8c65ab9cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65ab9e; /* mov.l @r14,r5 */
L8c65ab9e: *stop=0x8c65ab9eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ac84u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c65aba0; /* mov.l 0x8c65ac84,r11 */
L8c65aba0: *stop=0x8c65aba0u;r[5]+=r[3];goto L8c65aba2; /* add r3,r5 */
L8c65aba2: target=r[11];*pr=0x8c65aba6u;*stop=0x8c65aba4u;r[4]+=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65aba6; /* jsr @r11 */
L8c65aba6: *stop=0x8c65aba6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac6cu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65aba8; /* mov.w 0x8c65ac6c,r3 */
L8c65aba8: *stop=0x8c65aba8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65abaa; /* mov.l @r14,r5 */
L8c65abaa: *stop=0x8c65abaau;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac6eu,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65abac; /* mov.w 0x8c65ac6e,r4 */
L8c65abac: *stop=0x8c65abacu;r[5]+=r[3];goto L8c65abae; /* add r3,r5 */
L8c65abae: target=r[11];*pr=0x8c65abb2u;*stop=0x8c65abb0u;r[4]+=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65abb2; /* jsr @r11 */
L8c65abb2: *stop=0x8c65abb2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac70u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65abb4; /* mov.w 0x8c65ac70,r3 */
L8c65abb4: *stop=0x8c65abb4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65abb6; /* mov.l @r14,r5 */
L8c65abb6: *stop=0x8c65abb6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac72u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65abb8; /* mov.w 0x8c65ac72,r4 */
L8c65abb8: *stop=0x8c65abb8u;r[5]+=r[3];goto L8c65abba; /* add r3,r5 */
L8c65abba: target=r[11];*pr=0x8c65abbeu;*stop=0x8c65abbcu;r[4]+=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65abbe; /* jsr @r11 */
L8c65abbe: *stop=0x8c65abbeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac74u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65abc0; /* mov.w 0x8c65ac74,r3 */
L8c65abc0: *stop=0x8c65abc0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65abc2; /* mov.l @r14,r5 */
L8c65abc2: *stop=0x8c65abc2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac76u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65abc4; /* mov.w 0x8c65ac76,r4 */
L8c65abc4: *stop=0x8c65abc4u;r[5]+=r[3];goto L8c65abc6; /* add r3,r5 */
L8c65abc6: target=r[11];*pr=0x8c65abcau;*stop=0x8c65abc8u;r[4]+=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65abca; /* jsr @r11 */
L8c65abca: *stop=0x8c65abcau;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac78u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65abcc; /* mov.w 0x8c65ac78,r3 */
L8c65abcc: *stop=0x8c65abccu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65abce; /* mov.l @r14,r5 */
L8c65abce: *stop=0x8c65abceu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac7au,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65abd0; /* mov.w 0x8c65ac7a,r4 */
L8c65abd0: *stop=0x8c65abd0u;r[5]+=r[3];goto L8c65abd2; /* add r3,r5 */
L8c65abd2: target=r[11];*pr=0x8c65abd6u;*stop=0x8c65abd4u;r[4]+=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65abd6; /* jsr @r11 */
L8c65abd6: *stop=0x8c65abd6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac7cu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65abd8; /* mov.w 0x8c65ac7c,r3 */
L8c65abd8: *stop=0x8c65abd8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65abda; /* mov.l @r14,r5 */
L8c65abda: *stop=0x8c65abdau;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac7eu,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65abdc; /* mov.w 0x8c65ac7e,r4 */
L8c65abdc: *stop=0x8c65abdcu;r[5]+=r[3];goto L8c65abde; /* add r3,r5 */
L8c65abde: target=r[11];*pr=0x8c65abe2u;*stop=0x8c65abe0u;r[4]+=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65abe2; /* jsr @r11 */
L8c65abe2: *stop=0x8c65abe2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac80u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65abe4; /* mov.w 0x8c65ac80,r0 */
L8c65abe4: *stop=0x8c65abe4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65abe6; /* mov.l @r14,r2 */
L8c65abe6: *stop=0x8c65abe6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ac88u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65abe8; /* mov.l 0x8c65ac88,r1 */
L8c65abe8: *stop=0x8c65abe8u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65abea; /* mov.l @(r0,r2),r3 */
L8c65abea: *stop=0x8c65abeau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65abec; /* mov.l r3,@-r15 */
L8c65abec: target=r[12];*pr=0x8c65abf0u;*stop=0x8c65abeeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65abf0; /* jsr @r12 */
L8c65abf0: *stop=0x8c65abf0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac68u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65abf2; /* mov.w 0x8c65ac68,r0 */
L8c65abf2: *stop=0x8c65abf2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65abf4; /* mov.l @r14,r3 */
L8c65abf4: *stop=0x8c65abf4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ac8cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65abf6; /* mov.l 0x8c65ac8c,r1 */
L8c65abf6: *stop=0x8c65abf6u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65abf8; /* mov.l @(r0,r3),r2 */
L8c65abf8: *stop=0x8c65abf8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c65abfa; /* mov.l r2,@-r15 */
L8c65abfa: target=r[12];*pr=0x8c65abfeu;*stop=0x8c65abfcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65abfe; /* jsr @r12 */
L8c65abfe: *stop=0x8c65abfeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac6cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ac00; /* mov.w 0x8c65ac6c,r0 */
L8c65ac00: *stop=0x8c65ac00u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ac02; /* mov.l @r14,r3 */
L8c65ac02: *stop=0x8c65ac02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ac90u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ac04; /* mov.l 0x8c65ac90,r1 */
L8c65ac04: *stop=0x8c65ac04u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ac06; /* mov.l @(r0,r3),r2 */
L8c65ac06: *stop=0x8c65ac06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c65ac08; /* mov.l r2,@-r15 */
L8c65ac08: target=r[12];*pr=0x8c65ac0cu;*stop=0x8c65ac0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ac0c; /* jsr @r12 */
L8c65ac0c: *stop=0x8c65ac0cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac70u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ac0e; /* mov.w 0x8c65ac70,r0 */
L8c65ac0e: *stop=0x8c65ac0eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ac10; /* mov.l @r14,r3 */
L8c65ac10: *stop=0x8c65ac10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ac94u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ac12; /* mov.l 0x8c65ac94,r1 */
L8c65ac12: *stop=0x8c65ac12u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ac14; /* mov.l @(r0,r3),r2 */
L8c65ac14: *stop=0x8c65ac14u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c65ac16; /* mov.l r2,@-r15 */
L8c65ac16: target=r[12];*pr=0x8c65ac1au;*stop=0x8c65ac18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ac1a; /* jsr @r12 */
L8c65ac1a: *stop=0x8c65ac1au;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac74u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ac1c; /* mov.w 0x8c65ac74,r0 */
L8c65ac1c: *stop=0x8c65ac1cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ac1e; /* mov.l @r14,r3 */
L8c65ac1e: *stop=0x8c65ac1eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ac98u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ac20; /* mov.l 0x8c65ac98,r1 */
L8c65ac20: *stop=0x8c65ac20u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ac22; /* mov.l @(r0,r3),r2 */
L8c65ac22: *stop=0x8c65ac22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c65ac24; /* mov.l r2,@-r15 */
L8c65ac24: target=r[12];*pr=0x8c65ac28u;*stop=0x8c65ac26u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ac28; /* jsr @r12 */
L8c65ac28: *stop=0x8c65ac28u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac78u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ac2a; /* mov.w 0x8c65ac78,r0 */
L8c65ac2a: *stop=0x8c65ac2au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ac2c; /* mov.l @r14,r3 */
L8c65ac2c: *stop=0x8c65ac2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65ac9cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ac2e; /* mov.l 0x8c65ac9c,r1 */
L8c65ac2e: *stop=0x8c65ac2eu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ac30; /* mov.l @(r0,r3),r2 */
L8c65ac30: *stop=0x8c65ac30u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c65ac32; /* mov.l r2,@-r15 */
L8c65ac32: target=r[12];*pr=0x8c65ac36u;*stop=0x8c65ac34u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ac36; /* jsr @r12 */
L8c65ac36: *stop=0x8c65ac36u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65ac7cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65ac38; /* mov.w 0x8c65ac7c,r0 */
L8c65ac38: *stop=0x8c65ac38u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ac3a; /* mov.l @r14,r3 */
L8c65ac3a: *stop=0x8c65ac3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65aca0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65ac3c; /* mov.l 0x8c65aca0,r1 */
L8c65ac3c: *stop=0x8c65ac3cu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ac3e; /* mov.l @(r0,r3),r2 */
L8c65ac3e: *stop=0x8c65ac3eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c65ac40; /* mov.l r2,@-r15 */
L8c65ac40: target=r[12];*pr=0x8c65ac44u;*stop=0x8c65ac42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ac44; /* jsr @r12 */
L8c65ac44: *stop=0x8c65ac44u;r[15]+=56u;goto L8c65ac46; /* add #0x38,r15 */
L8c65ac46: *stop=0x8c65ac46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65aca4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65ac48; /* mov.l 0x8c65aca4,r3 */
L8c65ac48: *stop=0x8c65ac48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65aca8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65ac4a; /* mov.l 0x8c65aca8,r2 */
L8c65ac4a: *stop=0x8c65ac4au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65ac4c; /* mov.l r3,@-r15 */
L8c65ac4c: target=r[12];*pr=0x8c65ac50u;*stop=0x8c65ac4eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65ac50; /* jsr @r12 */
L8c65ac50: *stop=0x8c65ac50u;r[0]=r[10];goto L8c65ac52; /* mov r10,r0 */
L8c65ac52: *stop=0x8c65ac52u;r[15]+=8u;goto L8c65ac54; /* add #0x8,r15 */
L8c65ac54: *stop=0x8c65ac54u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65ac56; /* lds.l @r15+,PR */
L8c65ac56: *stop=0x8c65ac56u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65ac58; /* mov.l @r15+,r9 */
L8c65ac58: *stop=0x8c65ac58u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65ac5a; /* mov.l @r15+,r10 */
L8c65ac5a: *stop=0x8c65ac5au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65ac5c; /* mov.l @r15+,r11 */
L8c65ac5c: *stop=0x8c65ac5cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65ac5e; /* mov.l @r15+,r12 */
L8c65ac5e: *stop=0x8c65ac5eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65ac60; /* mov.l @r15+,r13 */
L8c65ac60: *stop=0x8c65ac60u;if(*pr!=entry_pr)return -3;*stop=0x8c65ac62u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_bulk_family_8c65a518(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65a518;
L8c65a518: *stop=0x8c65a518u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a51a; /* mov.l r14,@-r15 */
L8c65a51a: *stop=0x8c65a51au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c65a51c; /* tst r4,r4 */
L8c65a51c: *stop=0x8c65a51cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65a51e; /* mov.l r13,@-r15 */
L8c65a51e: *stop=0x8c65a51eu;r[13]=0u;goto L8c65a520; /* mov #0x0,r13 */
L8c65a520: *stop=0x8c65a520u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a522; /* sts.l PR,@-r15 */
L8c65a522: take=!(*sr&1u);*stop=0x8c65a524u;r[14]=r[4];if(take)goto L8c65a52c;goto L8c65a526; /* bf/s 0x8c65a52c */
L8c65a526: *stop=0x8c65a526u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a55cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c65a528; /* mov.l 0x8c65a55c,r13 */
L8c65a528: *stop=0x8c65a52au;goto L8c65a544; /* bra 0x8c65a544 */
L8c65a52c: *stop=0x8c65a52cu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a52e; /* mov.l @(0x10,r14),r3 */
L8c65a52e: *stop=0x8c65a52eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65a530; /* tst r3,r3 */
L8c65a530: if((*sr&1u))goto L8c65a544;goto L8c65a532; /* bt 0x8c65a544 */
L8c65a532: *stop=0x8c65a532u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a534; /* mov.l @(0x10,r14),r0 */
L8c65a534: *stop=0x8c65a534u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c65a536; /* cmp/eq #-0x1,r0 */
L8c65a536: if(!(*sr&1u))goto L8c65a53e;goto L8c65a538; /* bf 0x8c65a53e */
L8c65a538: *stop=0x8c65a538u;r[2]=1u;goto L8c65a53a; /* mov #0x1,r2 */
L8c65a53a: *stop=0x8c65a53cu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[2],4);if(io_status)return io_status;goto L8c65a544; /* bra 0x8c65a544 */
L8c65a53e: *stop=0x8c65a53eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65a540; /* mov.l @(0x10,r14),r1 */
L8c65a540: target=r[1];*pr=0x8c65a544u;*stop=0x8c65a542u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a544; /* jsr @r1 */
L8c65a544: *stop=0x8c65a544u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c65a546; /* tst r13,r13 */
L8c65a546: if((*sr&1u))goto L8c65a550;goto L8c65a548; /* bt 0x8c65a550 */
L8c65a548: *stop=0x8c65a548u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a564u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a54a; /* mov.l 0x8c65a564,r2 */
L8c65a54a: *stop=0x8c65a54au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a560u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a54c; /* mov.l 0x8c65a560,r4 */
L8c65a54c: target=r[2];*pr=0x8c65a550u;*stop=0x8c65a54eu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a550; /* jsr @r2 */
L8c65a550: *stop=0x8c65a550u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a552; /* lds.l @r15+,PR */
L8c65a552: *stop=0x8c65a552u;r[0]=r[13];goto L8c65a554; /* mov r13,r0 */
L8c65a554: *stop=0x8c65a554u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65a556; /* mov.l @r15+,r13 */
L8c65a556: *stop=0x8c65a556u;if(*pr!=entry_pr)return -3;*stop=0x8c65a558u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
