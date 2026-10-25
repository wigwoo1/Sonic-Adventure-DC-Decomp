/* Geometry projection, sampling and view proposals; explicit GBR/queue state.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_widen_compare.h"
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
SA_STAGE_EXPORT int sa_movie_diagnostics_8c65a2fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a2fc;
L8c65a2fc: *stop=0x8c65a2fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a368u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a2fe; /* mov.l 0x8c65a368,r2 */
L8c65a2fe: *stop=0x8c65a2feu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a300; /* sts.l PR,@-r15 */
L8c65a300: *stop=0x8c65a300u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a302; /* mov.l @r2,r3 */
L8c65a302: *stop=0x8c65a302u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65a304; /* tst r3,r3 */
L8c65a304: if((*sr&1u))goto L8c65a338;goto L8c65a306; /* bt 0x8c65a338 */
L8c65a306: *stop=0x8c65a306u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a36cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a308; /* mov.l 0x8c65a36c,r3 */
L8c65a308: *stop=0x8c65a308u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a30a; /* mov.l @r3,r0 */
L8c65a30a: *stop=0x8c65a30au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65a30c; /* cmp/eq #0x1,r0 */
L8c65a30c: if(!(*sr&1u))goto L8c65a338;goto L8c65a30e; /* bf 0x8c65a338 */
L8c65a30e: *stop=0x8c65a30eu;r[0]=r[15];goto L8c65a310; /* mov r15,r0 */
L8c65a310: *stop=0x8c65a310u;r[0]+=4u;goto L8c65a312; /* add #0x4,r0 */
L8c65a312: *stop=0x8c65a312u;r[5]=3u;goto L8c65a314; /* mov #0x3,r5 */
L8c65a314: *stop=0x8c65a314u;r[0]+=4u;goto L8c65a316; /* add #0x4,r0 */
L8c65a316: *stop=0x8c65a316u;*sr=(*sr&~1u)|!(r[0]&r[5]);goto L8c65a318; /* tst r0,r5 */
L8c65a318: if((*sr&1u))goto L8c65a322;goto L8c65a31a; /* bt 0x8c65a322 */
L8c65a31a: *stop=0x8c65a31au;r[5]=r[15];goto L8c65a31c; /* mov r15,r5 */
L8c65a31c: *stop=0x8c65a31cu;r[5]+=4u;goto L8c65a31e; /* add #0x4,r5 */
L8c65a31e: *stop=0x8c65a320u;r[5]+=8u;goto L8c65a328; /* bra 0x8c65a328 */
L8c65a322: *stop=0x8c65a322u;r[5]=r[15];goto L8c65a324; /* mov r15,r5 */
L8c65a324: *stop=0x8c65a324u;r[5]+=4u;goto L8c65a326; /* add #0x4,r5 */
L8c65a326: *stop=0x8c65a326u;r[5]+=4u;goto L8c65a328; /* add #0x4,r5 */
L8c65a328: *stop=0x8c65a328u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a374u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a32a; /* mov.l 0x8c65a374,r3 */
L8c65a32a: *stop=0x8c65a32au;r[6]=r[5];goto L8c65a32c; /* mov r5,r6 */
L8c65a32c: *stop=0x8c65a32cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a370u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a32e; /* mov.l 0x8c65a370,r4 */
L8c65a32e: target=r[3];*pr=0x8c65a332u;*stop=0x8c65a330u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a332; /* jsr @r3 */
L8c65a332: *stop=0x8c65a332u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a370u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a334; /* mov.l 0x8c65a370,r4 */
L8c65a334: target=0x8c65a33eu;*pr=0x8c65a338u;*stop=0x8c65a336u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a338; /* bsr 0x8c65a33e */
L8c65a338: *stop=0x8c65a338u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a33a; /* lds.l @r15+,PR */
L8c65a33a: *stop=0x8c65a33au;if(*pr!=entry_pr)return -3;*stop=0x8c65a33cu;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10d400(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d400;
L8c10d400: *stop=0x8c10d400u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d402; /* sts.l PR,@-r15 */
L8c10d402: *stop=0x8c10d402u;r[15]+=4294967284u;goto L8c10d404; /* add #-0xc,r15 */
L8c10d404: *stop=0x8c10d404u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c10d406; /* mov.l r4,@r15 */
L8c10d406: *stop=0x8c10d406u;r[3]=r[6];goto L8c10d408; /* mov r6,r3 */
L8c10d408: *stop=0x8c10d408u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c10d40a; /* mov.l r5,@(0x4,r15) */
L8c10d40a: *stop=0x8c10d40au;r[5]=0u;goto L8c10d40c; /* mov #0x0,r5 */
L8c10d40c: *stop=0x8c10d40cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c10d40e; /* mov.l r6,@(0x8,r15) */
L8c10d40e: *stop=0x8c10d40eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c10d410; /* mov.l r6,@-r15 */
L8c10d410: *stop=0x8c10d410u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d424u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d412; /* mov.l 0x8c10d424,r3 */
L8c10d412: *stop=0x8c10d412u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c10d414; /* mov.l @(0x4,r15),r6 */
L8c10d414: *stop=0x8c10d414u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d416; /* mov.l @(0x8,r15),r7 */
L8c10d416: target=r[3];*pr=0x8c10d41au;*stop=0x8c10d418u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d41a; /* jsr @r3 */
L8c10d41a: *stop=0x8c10d41au;r[15]+=16u;goto L8c10d41c; /* add #0x10,r15 */
L8c10d41c: *stop=0x8c10d41cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d41e; /* lds.l @r15+,PR */
L8c10d41e: *stop=0x8c10d41eu;if(*pr!=entry_pr)return -3;*stop=0x8c10d420u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c65a33e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a33e;
L8c65a33e: *stop=0x8c65a33eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a368u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a340; /* mov.l 0x8c65a368,r3 */
L8c65a340: *stop=0x8c65a340u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a342; /* mov.l r14,@-r15 */
L8c65a342: *stop=0x8c65a342u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a344; /* sts.l PR,@-r15 */
L8c65a344: *stop=0x8c65a344u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a346; /* mov.l @r3,r14 */
L8c65a346: *stop=0x8c65a346u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a348; /* tst r14,r14 */
L8c65a348: if((*sr&1u))goto L8c65a356;goto L8c65a34a; /* bt 0x8c65a356 */
L8c65a34a: *stop=0x8c65a34au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a36cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65a34c; /* mov.l 0x8c65a36c,r1 */
L8c65a34c: *stop=0x8c65a34cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a34e; /* mov.l @r1,r0 */
L8c65a34e: *stop=0x8c65a34eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65a350; /* cmp/eq #0x1,r0 */
L8c65a350: if(!(*sr&1u))goto L8c65a356;goto L8c65a352; /* bf 0x8c65a356 */
L8c65a352: target=r[14];*pr=0x8c65a356u;*stop=0x8c65a354u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a356; /* jsr @r14 */
L8c65a356: *stop=0x8c65a356u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a358; /* lds.l @r15+,PR */
L8c65a358: *stop=0x8c65a358u;r[0]=0u;goto L8c65a35a; /* mov #0x0,r0 */
L8c65a35a: *stop=0x8c65a35au;if(*pr!=entry_pr)return -3;*stop=0x8c65a35cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10dbc4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10dbc4;
L8c10dbc4: *stop=0x8c10dbc4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10dbc6; /* mov.l r14,@-r15 */
L8c10dbc6: *stop=0x8c10dbc6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10dbc8; /* mov.l r13,@-r15 */
L8c10dbc8: *stop=0x8c10dbc8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10dbca; /* mov.l r12,@-r15 */
L8c10dbca: *stop=0x8c10dbcau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10dbcc; /* mov.l r11,@-r15 */
L8c10dbcc: *stop=0x8c10dbccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10dbce; /* mov.l r10,@-r15 */
L8c10dbce: *stop=0x8c10dbceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10dbd0; /* mov.l r9,@-r15 */
L8c10dbd0: *stop=0x8c10dbd0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10dbd2; /* mov.l r8,@-r15 */
L8c10dbd2: *stop=0x8c10dbd2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10dbd4; /* sts.l PR,@-r15 */
L8c10dbd4: *stop=0x8c10dbd4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dca4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dbd6; /* mov.w 0x8c10dca4,r0 */
L8c10dbd6: *stop=0x8c10dbd6u;r[15]+=r[0];goto L8c10dbd8; /* add r0,r15 */
L8c10dbd8: *stop=0x8c10dbd8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dca8u,2);if(io_status)return io_status;r[14]=loaded;}goto L8c10dbda; /* mov.w 0x8c10dca8,r14 */
L8c10dbda: *stop=0x8c10dbdau;r[11]=0u;goto L8c10dbdc; /* mov #0x0,r11 */
L8c10dbdc: *stop=0x8c10dbdcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dca6u,2);if(io_status)return io_status;r[10]=loaded;}goto L8c10dbde; /* mov.w 0x8c10dca6,r10 */
L8c10dbde: *stop=0x8c10dbdeu;r[0]=r[11];goto L8c10dbe0; /* mov r11,r0 */
L8c10dbe0: *stop=0x8c10dbe0u;r[14]+=r[15];goto L8c10dbe2; /* add r15,r14 */
L8c10dbe2: *stop=0x8c10dbe2u;r[12]=r[7];goto L8c10dbe4; /* mov r7,r12 */
L8c10dbe4: *stop=0x8c10dbe4u;r[10]+=r[15];goto L8c10dbe6; /* add r15,r10 */
L8c10dbe6: *stop=0x8c10dbe6u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c10dbe8; /* tst r4,r4 */
L8c10dbe8: *stop=0x8c10dbe8u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[11],4);if(io_status)return io_status;goto L8c10dbea; /* mov.l r11,@r10 */
L8c10dbea: *stop=0x8c10dbeau;if(!valid_access(r[10]+4u,4u))return -3;wr(m,bus,&io_status,r[10]+0x4u,r[11],4);if(io_status)return io_status;goto L8c10dbec; /* mov.l r11,@(0x4,r10) */
L8c10dbec: *stop=0x8c10dbecu;if(!valid_access(r[10]+8u,4u))return -3;wr(m,bus,&io_status,r[10]+0x8u,r[11],4);if(io_status)return io_status;goto L8c10dbee; /* mov.l r11,@(0x8,r10) */
L8c10dbee: *stop=0x8c10dbeeu;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[11],1);if(io_status)return io_status;goto L8c10dbf0; /* mov.b r11,@r14 */
L8c10dbf0: *stop=0x8c10dbf0u;if(!valid_access(r[14]+1u,1u))return -3;wr(m,bus,&io_status,r[14]+0x1u,r[0],1);if(io_status)return io_status;goto L8c10dbf2; /* mov.b r0,@(0x1,r14) */
L8c10dbf2: *stop=0x8c10dbf2u;if(!valid_access(r[14]+2u,2u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c10dbf4; /* mov.w r0,@(0x2,r14) */
L8c10dbf4: *stop=0x8c10dbf4u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],4);if(io_status)return io_status;goto L8c10dbf6; /* mov.l r0,@(0x4,r14) */
L8c10dbf6: *stop=0x8c10dbf6u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],4);if(io_status)return io_status;goto L8c10dbf8; /* mov.l r0,@(0x8,r14) */
L8c10dbf8: *stop=0x8c10dbf8u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[0],4);if(io_status)return io_status;goto L8c10dbfa; /* mov.l r0,@(0x10,r14) */
L8c10dbfa: *stop=0x8c10dbfau;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[0],4);if(io_status)return io_status;goto L8c10dbfc; /* mov.l r0,@(0x14,r14) */
L8c10dbfc: *stop=0x8c10dbfcu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[0],4);if(io_status)return io_status;goto L8c10dbfe; /* mov.l r0,@(0x18,r14) */
L8c10dbfe: *stop=0x8c10dbfeu;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c10dc00; /* mov.l r0,@(0x1c,r14) */
L8c10dc00: *stop=0x8c10dc00u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[0],4);if(io_status)return io_status;goto L8c10dc02; /* mov.l r0,@(0x20,r14) */
L8c10dc02: *stop=0x8c10dc02u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[0],4);if(io_status)return io_status;goto L8c10dc04; /* mov.l r0,@(0x24,r14) */
L8c10dc04: take=!(*sr&1u);*stop=0x8c10dc06u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[0],4);if(io_status)return io_status;if(take)goto L8c10dc3e;goto L8c10dc08; /* bf/s 0x8c10dc3e */
L8c10dc08: *stop=0x8c10dc08u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c10dc0a; /* tst r6,r6 */
L8c10dc0a: if(!(*sr&1u))goto L8c10dc14;goto L8c10dc0c; /* bf 0x8c10dc14 */
L8c10dc0c: *stop=0x8c10dc0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10dcb4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10dc0e; /* mov.l 0x8c10dcb4,r2 */
L8c10dc0e: *stop=0x8c10dc0eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dcaau,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10dc10; /* mov.w 0x8c10dcaa,r3 */
L8c10dc10: *stop=0x8c10dc12u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c10dc52; /* bra 0x8c10dc52 */
L8c10dc14: *stop=0x8c10dc14u;r[0]=16u;goto L8c10dc16; /* mov #0x10,r0 */
L8c10dc16: *stop=0x8c10dc16u;if(!valid_access(r[0]+r[6],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dc18; /* mov.b @(r0,r6),r0 */
L8c10dc18: *stop=0x8c10dc18u;*sr=(*sr&~1u)|!(r[0]&0x7u);goto L8c10dc1a; /* tst #0x7,r0 */
L8c10dc1a: if(!(*sr&1u))goto L8c10dc24;goto L8c10dc1c; /* bf 0x8c10dc24 */
L8c10dc1c: *stop=0x8c10dc1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10dcb4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10dc1e; /* mov.l 0x8c10dcb4,r3 */
L8c10dc1e: *stop=0x8c10dc1eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dcacu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c10dc20; /* mov.w 0x8c10dcac,r2 */
L8c10dc20: *stop=0x8c10dc22u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c10dc52; /* bra 0x8c10dc52 */
L8c10dc24: *stop=0x8c10dc24u;r[0]=16u;goto L8c10dc26; /* mov #0x10,r0 */
L8c10dc26: *stop=0x8c10dc26u;if(!valid_access(r[0]+r[6],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6]+r[0],1);if(io_status)return io_status;r[7]=loaded;}goto L8c10dc28; /* mov.b @(r0,r6),r7 */
L8c10dc28: *stop=0x8c10dc28u;r[3]=4u;goto L8c10dc2a; /* mov #0x4,r3 */
L8c10dc2a: *stop=0x8c10dc2au;*sr=(*sr&~1u)|!(r[7]&r[3]);goto L8c10dc2c; /* tst r7,r3 */
L8c10dc2c: if(!(*sr&1u))goto L8c10dc3c;goto L8c10dc2e; /* bf 0x8c10dc3c */
L8c10dc2e: *stop=0x8c10dc2eu;r[1]=1u;goto L8c10dc30; /* mov #0x1,r1 */
L8c10dc30: *stop=0x8c10dc30u;*sr=(*sr&~1u)|!(r[7]&r[1]);goto L8c10dc32; /* tst r7,r1 */
L8c10dc32: if((*sr&1u))goto L8c10dc3c;goto L8c10dc34; /* bt 0x8c10dc3c */
L8c10dc34: *stop=0x8c10dc34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10dcb4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10dc36; /* mov.l 0x8c10dcb4,r3 */
L8c10dc36: *stop=0x8c10dc36u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dcaeu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c10dc38; /* mov.w 0x8c10dcae,r2 */
L8c10dc38: *stop=0x8c10dc3au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c10dc52; /* bra 0x8c10dc52 */
L8c10dc3c: *stop=0x8c10dc3cu;if(!valid_access(r[10]+12u,4u))return -3;wr(m,bus,&io_status,r[10]+0xcu,r[5],4);if(io_status)return io_status;goto L8c10dc3e; /* mov.l r5,@(0xc,r10) */
L8c10dc3e: *stop=0x8c10dc3eu;r[0]=48u;goto L8c10dc40; /* mov #0x30,r0 */
L8c10dc40: *stop=0x8c10dc40u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[4],4);if(io_status)return io_status;goto L8c10dc42; /* mov.l r4,@r10 */
L8c10dc42: *stop=0x8c10dc42u;if(!valid_access(r[10]+4u,4u))return -3;wr(m,bus,&io_status,r[10]+0x4u,r[6],4);if(io_status)return io_status;goto L8c10dc44; /* mov.l r6,@(0x4,r10) */
L8c10dc44: *stop=0x8c10dc44u;if(!valid_access(r[10]+8u,4u))return -3;wr(m,bus,&io_status,r[10]+0x8u,r[11],4);if(io_status)return io_status;goto L8c10dc46; /* mov.l r11,@(0x8,r10) */
L8c10dc46: *stop=0x8c10dc46u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[11],1);if(io_status)return io_status;goto L8c10dc48; /* mov.b r11,@r14 */
L8c10dc48: *stop=0x8c10dc48u;if(!valid_access(r[15]+8u,1u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],1);if(io_status)return io_status;goto L8c10dc4a; /* mov.b r0,@(0x8,r15) */
L8c10dc4a: target=0x8c10e576u;*pr=0x8c10dc4eu;*stop=0x8c10dc4cu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10dc4e; /* bsr 0x8c10e576 */
L8c10dc4e: *stop=0x8c10dc4eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10dc50; /* tst r0,r0 */
L8c10dc50: if((*sr&1u))goto L8c10dc56;goto L8c10dc52; /* bt 0x8c10dc56 */
L8c10dc52: *stop=0x8c10dc54u;r[0]=4294967295u;goto L8c10e2ec; /* bra 0x8c10e2ec */
L8c10dc56: *stop=0x8c10dc56u;r[9]=r[15];goto L8c10dc58; /* mov r15,r9 */
L8c10dc58: *stop=0x8c10dc58u;r[9]+=20u;goto L8c10dc5a; /* add #0x14,r9 */
L8c10dc5a: *stop=0x8c10dc5au;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dcb0u,2);if(io_status)return io_status;r[8]=loaded;}goto L8c10dc5c; /* mov.w 0x8c10dcb0,r8 */
L8c10dc5c: *stop=0x8c10dc5cu;r[3]=r[9];goto L8c10dc5e; /* mov r9,r3 */
L8c10dc5e: *stop=0x8c10dc5eu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c10dc60; /* mov.l r3,@(0x10,r15) */
L8c10dc60: *stop=0x8c10dc62u;r[13]=3u;goto L8c10e2ce; /* bra 0x8c10e2ce */
L8c10dc64: *stop=0x8c10dc64u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dc66; /* mov.b @r12,r0 */
L8c10dc66: *stop=0x8c10dc66u;*sr=(*sr&~1u)|(r[0]==0x00000025u);goto L8c10dc68; /* cmp/eq #0x25,r0 */
L8c10dc68: if((*sr&1u))goto L8c10dc6e;goto L8c10dc6a; /* bt 0x8c10dc6e */
L8c10dc6a: *stop=0x8c10dc6cu;goto L8c10e2aa; /* bra 0x8c10e2aa */
L8c10dc6e: *stop=0x8c10dc6eu;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[11],1);if(io_status)return io_status;goto L8c10dc70; /* mov.b r11,@r14 */
L8c10dc70: *stop=0x8c10dc70u;r[12]+=1u;goto L8c10dc72; /* add #0x1,r12 */
L8c10dc72: *stop=0x8c10dc72u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[9],4);if(io_status)return io_status;goto L8c10dc74; /* mov.l r9,@(0x10,r14) */
L8c10dc74: *stop=0x8c10dc74u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[9],4);if(io_status)return io_status;goto L8c10dc76; /* mov.l r9,@(0x14,r14) */
L8c10dc76: *stop=0x8c10dc76u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[11],4);if(io_status)return io_status;goto L8c10dc78; /* mov.l r11,@(0x18,r14) */
L8c10dc78: *stop=0x8c10dc78u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[11],4);if(io_status)return io_status;goto L8c10dc7a; /* mov.l r11,@(0x1c,r14) */
L8c10dc7a: *stop=0x8c10dc7au;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[11],4);if(io_status)return io_status;goto L8c10dc7c; /* mov.l r11,@(0x20,r14) */
L8c10dc7c: *stop=0x8c10dc7cu;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[11],4);if(io_status)return io_status;goto L8c10dc7e; /* mov.l r11,@(0x24,r14) */
L8c10dc7e: *stop=0x8c10dc80u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[11],4);if(io_status)return io_status;goto L8c10dcce; /* bra 0x8c10dcce */
L8c10dc82: *stop=0x8c10dc82u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dc84; /* mov.b @r12,r0 */
L8c10dc84: *stop=0x8c10dc84u;*sr=(*sr&~1u)|(r[0]==0x0000002du);goto L8c10dc86; /* cmp/eq #0x2d,r0 */
L8c10dc86: if((*sr&1u))goto L8c10dc98;goto L8c10dc88; /* bt 0x8c10dc98 */
L8c10dc88: *stop=0x8c10dc88u;*sr=(*sr&~1u)|(r[0]==0x0000002bu);goto L8c10dc8a; /* cmp/eq #0x2b,r0 */
L8c10dc8a: if((*sr&1u))goto L8c10dc9e;goto L8c10dc8c; /* bt 0x8c10dc9e */
L8c10dc8c: *stop=0x8c10dc8cu;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c10dc8e; /* cmp/eq #0x20,r0 */
L8c10dc8e: if((*sr&1u))goto L8c10dcb8;goto L8c10dc90; /* bt 0x8c10dcb8 */
L8c10dc90: *stop=0x8c10dc90u;*sr=(*sr&~1u)|(r[0]==0x00000023u);goto L8c10dc92; /* cmp/eq #0x23,r0 */
L8c10dc92: if((*sr&1u))goto L8c10dcc6;goto L8c10dc94; /* bt 0x8c10dcc6 */
L8c10dc94: *stop=0x8c10dc96u;goto L8c10dccc; /* bra 0x8c10dccc */
L8c10dc98: *stop=0x8c10dc98u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dc9a; /* mov.b @r14,r0 */
L8c10dc9a: *stop=0x8c10dc9cu;r[0]|=0x2u;goto L8c10dcca; /* bra 0x8c10dcca */
L8c10dc9e: *stop=0x8c10dc9eu;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dca0; /* mov.b @r14,r0 */
L8c10dca0: *stop=0x8c10dca2u;r[0]|=0x8u;goto L8c10dcca; /* bra 0x8c10dcca */
L8c10dcb8: *stop=0x8c10dcb8u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dcba; /* mov.b @r14,r0 */
L8c10dcba: *stop=0x8c10dcbau;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c10dcbc; /* tst #0x8,r0 */
L8c10dcbc: if(!(*sr&1u))goto L8c10dccc;goto L8c10dcbe; /* bf 0x8c10dccc */
L8c10dcbe: *stop=0x8c10dcbeu;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dcc0; /* mov.b @r14,r0 */
L8c10dcc0: *stop=0x8c10dcc0u;r[0]|=0x10u;goto L8c10dcc2; /* or #0x10,r0 */
L8c10dcc2: *stop=0x8c10dcc4u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c10dccc; /* bra 0x8c10dccc */
L8c10dcc6: *stop=0x8c10dcc6u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dcc8; /* mov.b @r14,r0 */
L8c10dcc8: *stop=0x8c10dcc8u;r[0]|=0x4u;goto L8c10dcca; /* or #0x4,r0 */
L8c10dcca: *stop=0x8c10dccau;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c10dccc; /* mov.b r0,@r14 */
L8c10dccc: *stop=0x8c10dcccu;r[12]+=1u;goto L8c10dcce; /* add #0x1,r12 */
L8c10dcce: *stop=0x8c10dcceu;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dcd0; /* mov.b @r12,r0 */
L8c10dcd0: *stop=0x8c10dcd0u;*sr=(*sr&~1u)|(r[0]==0x0000002du);goto L8c10dcd2; /* cmp/eq #0x2d,r0 */
L8c10dcd2: take=(*sr&1u);*stop=0x8c10dcd4u;r[4]=r[0];if(take)goto L8c10dc82;goto L8c10dcd6; /* bt/s 0x8c10dc82 */
L8c10dcd6: *stop=0x8c10dcd6u;r[0]=r[4];goto L8c10dcd8; /* mov r4,r0 */
L8c10dcd8: *stop=0x8c10dcd8u;*sr=(*sr&~1u)|(r[0]==0x0000002bu);goto L8c10dcda; /* cmp/eq #0x2b,r0 */
L8c10dcda: if((*sr&1u))goto L8c10dc82;goto L8c10dcdc; /* bt 0x8c10dc82 */
L8c10dcdc: *stop=0x8c10dcdcu;r[0]=r[4];goto L8c10dcde; /* mov r4,r0 */
L8c10dcde: *stop=0x8c10dcdeu;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c10dce0; /* cmp/eq #0x20,r0 */
L8c10dce0: if((*sr&1u))goto L8c10dc82;goto L8c10dce2; /* bt 0x8c10dc82 */
L8c10dce2: *stop=0x8c10dce2u;r[0]=r[4];goto L8c10dce4; /* mov r4,r0 */
L8c10dce4: *stop=0x8c10dce4u;*sr=(*sr&~1u)|(r[0]==0x00000023u);goto L8c10dce6; /* cmp/eq #0x23,r0 */
L8c10dce6: if((*sr&1u))goto L8c10dc82;goto L8c10dce8; /* bt 0x8c10dc82 */
L8c10dce8: *stop=0x8c10dce8u;r[0]=32u;goto L8c10dcea; /* mov #0x20,r0 */
L8c10dcea: *stop=0x8c10dceau;if(!valid_access(r[14]+1u,1u))return -3;wr(m,bus,&io_status,r[14]+0x1u,r[0],1);if(io_status)return io_status;goto L8c10dcec; /* mov.b r0,@(0x1,r14) */
L8c10dcec: *stop=0x8c10dcecu;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dcee; /* mov.b @r12,r0 */
L8c10dcee: *stop=0x8c10dceeu;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10dcf0; /* cmp/eq #0x30,r0 */
L8c10dcf0: if(!(*sr&1u))goto L8c10dcf8;goto L8c10dcf2; /* bf 0x8c10dcf8 */
L8c10dcf2: *stop=0x8c10dcf2u;r[0]=48u;goto L8c10dcf4; /* mov #0x30,r0 */
L8c10dcf4: *stop=0x8c10dcf4u;r[12]+=1u;goto L8c10dcf6; /* add #0x1,r12 */
L8c10dcf6: *stop=0x8c10dcf6u;if(!valid_access(r[14]+1u,1u))return -3;wr(m,bus,&io_status,r[14]+0x1u,r[0],1);if(io_status)return io_status;goto L8c10dcf8; /* mov.b r0,@(0x1,r14) */
L8c10dcf8: *stop=0x8c10dcf8u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[11],4);if(io_status)return io_status;goto L8c10dcfa; /* mov.l r11,@(0x4,r14) */
L8c10dcfa: *stop=0x8c10dcfau;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dcfc; /* mov.b @r12,r0 */
L8c10dcfc: *stop=0x8c10dcfcu;*sr=(*sr&~1u)|(r[0]==0x0000002au);goto L8c10dcfe; /* cmp/eq #0x2a,r0 */
L8c10dcfe: if(!(*sr&1u))goto L8c10dd56;goto L8c10dd00; /* bf 0x8c10dd56 */
L8c10dd00: *stop=0x8c10dd00u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dd8eu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dd02; /* mov.w 0x8c10dd8e,r0 */
L8c10dd02: *stop=0x8c10dd02u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10dd04; /* mov.l @(r0,r15),r4 */
L8c10dd04: *stop=0x8c10dd04u;r[4]+=4u;goto L8c10dd06; /* add #0x4,r4 */
L8c10dd06: *stop=0x8c10dd06u;r[3]=r[4];goto L8c10dd08; /* mov r4,r3 */
L8c10dd08: *stop=0x8c10dd08u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c10dd0a; /* tst r13,r3 */
L8c10dd0a: if((*sr&1u))goto L8c10dd14;goto L8c10dd0c; /* bt 0x8c10dd14 */
L8c10dd0c: *stop=0x8c10dd0cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dd8eu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dd0e; /* mov.w 0x8c10dd8e,r0 */
L8c10dd0e: *stop=0x8c10dd0eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10dd10; /* mov.l @(r0,r15),r2 */
L8c10dd10: *stop=0x8c10dd12u;r[2]+=8u;goto L8c10dd16; /* bra 0x8c10dd16 */
L8c10dd14: *stop=0x8c10dd14u;r[2]=r[4];goto L8c10dd16; /* mov r4,r2 */
L8c10dd16: *stop=0x8c10dd16u;r[5]=r[2];goto L8c10dd18; /* mov r2,r5 */
L8c10dd18: *stop=0x8c10dd18u;r[5]+=4294967292u;goto L8c10dd1a; /* add #-0x4,r5 */
L8c10dd1a: *stop=0x8c10dd1au;r[1]=r[5];goto L8c10dd1c; /* mov r5,r1 */
L8c10dd1c: *stop=0x8c10dd1cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dd8eu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dd1e; /* mov.w 0x8c10dd8e,r0 */
L8c10dd1e: *stop=0x8c10dd1eu;*sr=(*sr&~1u)|!(r[13]&r[1]);goto L8c10dd20; /* tst r13,r1 */
L8c10dd20: take=(*sr&1u);*stop=0x8c10dd22u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;if(take)goto L8c10dd2a;goto L8c10dd24; /* bt/s 0x8c10dd2a */
L8c10dd24: *stop=0x8c10dd24u;r[3]=r[2];goto L8c10dd26; /* mov r2,r3 */
L8c10dd26: *stop=0x8c10dd28u;r[3]+=4294967288u;goto L8c10dd2c; /* bra 0x8c10dd2c */
L8c10dd2a: *stop=0x8c10dd2au;r[3]=r[5];goto L8c10dd2c; /* mov r5,r3 */
L8c10dd2c: *stop=0x8c10dd2cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10dd2e; /* mov.l @r3,r1 */
L8c10dd2e: *stop=0x8c10dd2eu;r[3]=r[1];goto L8c10dd30; /* mov r1,r3 */
L8c10dd30: *stop=0x8c10dd30u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c10dd32; /* cmp/pz r3 */
L8c10dd32: take=(*sr&1u);*stop=0x8c10dd34u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[1],4);if(io_status)return io_status;if(take)goto L8c10dd42;goto L8c10dd36; /* bt/s 0x8c10dd42 */
L8c10dd36: *stop=0x8c10dd36u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dd38; /* mov.b @r14,r0 */
L8c10dd38: *stop=0x8c10dd38u;r[0]|=0x2u;goto L8c10dd3a; /* or #0x2,r0 */
L8c10dd3a: *stop=0x8c10dd3au;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c10dd3c; /* mov.b r0,@r14 */
L8c10dd3c: *stop=0x8c10dd3cu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10dd3e; /* mov.l @(0x4,r14),r3 */
L8c10dd3e: *stop=0x8c10dd3eu;r[3]=0u-r[3];goto L8c10dd40; /* neg r3,r3 */
L8c10dd40: *stop=0x8c10dd40u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c10dd42; /* mov.l r3,@(0x4,r14) */
L8c10dd42: *stop=0x8c10dd42u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10dd44; /* mov.l @(0x4,r14),r1 */
L8c10dd44: *stop=0x8c10dd44u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dd90u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c10dd46; /* mov.w 0x8c10dd90,r2 */
L8c10dd46: *stop=0x8c10dd46u;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[2]);goto L8c10dd48; /* cmp/gt r2,r1 */
L8c10dd48: take=!(*sr&1u);*stop=0x8c10dd4au;r[12]+=1u;if(take)goto L8c10dd56;goto L8c10dd4c; /* bf/s 0x8c10dd56 */
L8c10dd4c: *stop=0x8c10dd4cu;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dd4e; /* mov.b @r14,r0 */
L8c10dd4e: *stop=0x8c10dd4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10dd94u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10dd50; /* mov.l 0x8c10dd94,r1 */
L8c10dd50: *stop=0x8c10dd50u;r[0]|=0x1u;goto L8c10dd52; /* or #0x1,r0 */
L8c10dd52: *stop=0x8c10dd52u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c10dd54; /* mov.b r0,@r14 */
L8c10dd54: *stop=0x8c10dd54u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[8],4);if(io_status)return io_status;goto L8c10dd56; /* mov.l r8,@r1 */
L8c10dd56: *stop=0x8c10dd56u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dd58; /* mov.b @r12,r0 */
L8c10dd58: *stop=0x8c10dd58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10dd98u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10dd5a; /* mov.l 0x8c10dd98,r1 */
L8c10dd5a: *stop=0x8c10dd5au;r[0]=r[0]&255u;goto L8c10dd5c; /* extu.b r0,r0 */
L8c10dd5c: *stop=0x8c10dd5cu;if(!valid_access(r[0]+r[1],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dd5e; /* mov.b @(r0,r1),r0 */
L8c10dd5e: *stop=0x8c10dd5eu;r[0]=r[0]&255u;goto L8c10dd60; /* extu.b r0,r0 */
L8c10dd60: *stop=0x8c10dd60u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c10dd62; /* tst #0x4,r0 */
L8c10dd62: if((*sr&1u))goto L8c10ddb8;goto L8c10dd64; /* bt 0x8c10ddb8 */
L8c10dd64: *stop=0x8c10dd66u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[11],4);if(io_status)return io_status;goto L8c10ddaa; /* bra 0x8c10ddaa */
L8c10dd68: *stop=0x8c10dd68u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[4]=loaded;}goto L8c10dd6a; /* mov.b @r12,r4 */
L8c10dd6a: *stop=0x8c10dd6au;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dd90u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10dd6c; /* mov.w 0x8c10dd90,r1 */
L8c10dd6c: *stop=0x8c10dd6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10dd9cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10dd6e; /* mov.l 0x8c10dd9c,r3 */
L8c10dd6e: *stop=0x8c10dd6eu;r[4]+=4294967248u;goto L8c10dd70; /* add #-0x30,r4 */
L8c10dd70: *stop=0x8c10dd70u;r[1]-=r[4];goto L8c10dd72; /* sub r4,r1 */
L8c10dd72: target=r[3];*pr=0x8c10dd76u;*stop=0x8c10dd74u;r[0]=10u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10dd76; /* jsr @r3 */
L8c10dd76: *stop=0x8c10dd76u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10dd78; /* mov.l @(0x4,r14),r2 */
L8c10dd78: *stop=0x8c10dd78u;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[2]);goto L8c10dd7a; /* cmp/ge r2,r0 */
L8c10dd7a: take=!(*sr&1u);*stop=0x8c10dd7cu;r[12]+=1u;if(take)goto L8c10dda0;goto L8c10dd7e; /* bf/s 0x8c10dda0 */
L8c10dd7e: *stop=0x8c10dd7eu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10dd80; /* mov.l @(0x4,r14),r1 */
L8c10dd80: *stop=0x8c10dd80u;r[3]=r[1];goto L8c10dd82; /* mov r1,r3 */
L8c10dd82: *stop=0x8c10dd82u;r[1]<<=2;goto L8c10dd84; /* shll2 r1 */
L8c10dd84: *stop=0x8c10dd84u;r[1]+=r[3];goto L8c10dd86; /* add r3,r1 */
L8c10dd86: *stop=0x8c10dd86u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c10dd88; /* shll r1 */
L8c10dd88: *stop=0x8c10dd88u;r[1]+=r[4];goto L8c10dd8a; /* add r4,r1 */
L8c10dd8a: *stop=0x8c10dd8cu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[1],4);if(io_status)return io_status;goto L8c10ddaa; /* bra 0x8c10ddaa */
L8c10dda0: *stop=0x8c10dda0u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dda2; /* mov.b @r14,r0 */
L8c10dda2: *stop=0x8c10dda2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10de54u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10dda4; /* mov.l 0x8c10de54,r3 */
L8c10dda4: *stop=0x8c10dda4u;r[0]|=0x1u;goto L8c10dda6; /* or #0x1,r0 */
L8c10dda6: *stop=0x8c10dda6u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c10dda8; /* mov.b r0,@r14 */
L8c10dda8: *stop=0x8c10dda8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[8],4);if(io_status)return io_status;goto L8c10ddaa; /* mov.l r8,@r3 */
L8c10ddaa: *stop=0x8c10ddaau;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10ddac; /* mov.b @r12,r0 */
L8c10ddac: *stop=0x8c10ddacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10de58u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ddae; /* mov.l 0x8c10de58,r1 */
L8c10ddae: *stop=0x8c10ddaeu;r[0]=r[0]&255u;goto L8c10ddb0; /* extu.b r0,r0 */
L8c10ddb0: *stop=0x8c10ddb0u;if(!valid_access(r[0]+r[1],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10ddb2; /* mov.b @(r0,r1),r0 */
L8c10ddb2: *stop=0x8c10ddb2u;r[0]=r[0]&255u;goto L8c10ddb4; /* extu.b r0,r0 */
L8c10ddb4: *stop=0x8c10ddb4u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c10ddb6; /* tst #0x4,r0 */
L8c10ddb6: if(!(*sr&1u))goto L8c10dd68;goto L8c10ddb8; /* bf 0x8c10dd68 */
L8c10ddb8: *stop=0x8c10ddb8u;r[2]=4294967295u;goto L8c10ddba; /* mov #-0x1,r2 */
L8c10ddba: *stop=0x8c10ddbau;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c10ddbc; /* mov.l r2,@(0x8,r14) */
L8c10ddbc: *stop=0x8c10ddbcu;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10ddbe; /* mov.b @r12,r0 */
L8c10ddbe: *stop=0x8c10ddbeu;*sr=(*sr&~1u)|(r[0]==0x0000002eu);goto L8c10ddc0; /* cmp/eq #0x2e,r0 */
L8c10ddc0: if(!(*sr&1u))goto L8c10de78;goto L8c10ddc2; /* bf 0x8c10de78 */
L8c10ddc2: *stop=0x8c10ddc2u;r[12]+=1u;goto L8c10ddc4; /* add #0x1,r12 */
L8c10ddc4: *stop=0x8c10ddc4u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10ddc6; /* mov.b @r12,r0 */
L8c10ddc6: *stop=0x8c10ddc6u;*sr=(*sr&~1u)|(r[0]==0x0000002au);goto L8c10ddc8; /* cmp/eq #0x2a,r0 */
L8c10ddc8: if(!(*sr&1u))goto L8c10de18;goto L8c10ddca; /* bf 0x8c10de18 */
L8c10ddca: *stop=0x8c10ddcau;{uint32_t loaded=sx(m,bus,&io_status,0x8c10de50u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10ddcc; /* mov.w 0x8c10de50,r0 */
L8c10ddcc: *stop=0x8c10ddccu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10ddce; /* mov.l @(r0,r15),r4 */
L8c10ddce: *stop=0x8c10ddceu;r[4]+=4u;goto L8c10ddd0; /* add #0x4,r4 */
L8c10ddd0: *stop=0x8c10ddd0u;r[3]=r[4];goto L8c10ddd2; /* mov r4,r3 */
L8c10ddd2: *stop=0x8c10ddd2u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c10ddd4; /* tst r13,r3 */
L8c10ddd4: if((*sr&1u))goto L8c10ddde;goto L8c10ddd6; /* bt 0x8c10ddde */
L8c10ddd6: *stop=0x8c10ddd6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10de50u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10ddd8; /* mov.w 0x8c10de50,r0 */
L8c10ddd8: *stop=0x8c10ddd8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ddda; /* mov.l @(r0,r15),r2 */
L8c10ddda: *stop=0x8c10dddcu;r[2]+=8u;goto L8c10dde0; /* bra 0x8c10dde0 */
L8c10ddde: *stop=0x8c10dddeu;r[2]=r[4];goto L8c10dde0; /* mov r4,r2 */
L8c10dde0: *stop=0x8c10dde0u;r[5]=r[2];goto L8c10dde2; /* mov r2,r5 */
L8c10dde2: *stop=0x8c10dde2u;r[5]+=4294967292u;goto L8c10dde4; /* add #-0x4,r5 */
L8c10dde4: *stop=0x8c10dde4u;r[1]=r[5];goto L8c10dde6; /* mov r5,r1 */
L8c10dde6: *stop=0x8c10dde6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10de50u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dde8; /* mov.w 0x8c10de50,r0 */
L8c10dde8: *stop=0x8c10dde8u;*sr=(*sr&~1u)|!(r[13]&r[1]);goto L8c10ddea; /* tst r13,r1 */
L8c10ddea: take=(*sr&1u);*stop=0x8c10ddecu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;if(take)goto L8c10ddf4;goto L8c10ddee; /* bt/s 0x8c10ddf4 */
L8c10ddee: *stop=0x8c10ddeeu;r[3]=r[2];goto L8c10ddf0; /* mov r2,r3 */
L8c10ddf0: *stop=0x8c10ddf2u;r[3]+=4294967288u;goto L8c10ddf6; /* bra 0x8c10ddf6 */
L8c10ddf4: *stop=0x8c10ddf4u;r[3]=r[5];goto L8c10ddf6; /* mov r5,r3 */
L8c10ddf6: *stop=0x8c10ddf6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ddf8; /* mov.l @r3,r1 */
L8c10ddf8: *stop=0x8c10ddf8u;r[3]=r[1];goto L8c10ddfa; /* mov r1,r3 */
L8c10ddfa: *stop=0x8c10ddfau;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c10ddfc; /* cmp/pz r3 */
L8c10ddfc: take=(*sr&1u);*stop=0x8c10ddfeu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[1],4);if(io_status)return io_status;if(take)goto L8c10de04;goto L8c10de00; /* bt/s 0x8c10de04 */
L8c10de00: *stop=0x8c10de00u;r[1]=4294967295u;goto L8c10de02; /* mov #-0x1,r1 */
L8c10de02: *stop=0x8c10de02u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[1],4);if(io_status)return io_status;goto L8c10de04; /* mov.l r1,@(0x8,r14) */
L8c10de04: *stop=0x8c10de04u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10de06; /* mov.l @(0x8,r14),r2 */
L8c10de06: *stop=0x8c10de06u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10de52u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10de08; /* mov.w 0x8c10de52,r3 */
L8c10de08: *stop=0x8c10de08u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]);goto L8c10de0a; /* cmp/gt r3,r2 */
L8c10de0a: take=!(*sr&1u);*stop=0x8c10de0cu;r[12]+=1u;if(take)goto L8c10de18;goto L8c10de0e; /* bf/s 0x8c10de18 */
L8c10de0e: *stop=0x8c10de0eu;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10de10; /* mov.b @r14,r0 */
L8c10de10: *stop=0x8c10de10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10de54u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10de12; /* mov.l 0x8c10de54,r2 */
L8c10de12: *stop=0x8c10de12u;r[0]|=0x1u;goto L8c10de14; /* or #0x1,r0 */
L8c10de14: *stop=0x8c10de14u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c10de16; /* mov.b r0,@r14 */
L8c10de16: *stop=0x8c10de16u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[8],4);if(io_status)return io_status;goto L8c10de18; /* mov.l r8,@r2 */
L8c10de18: *stop=0x8c10de18u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10de1a; /* mov.b @r12,r0 */
L8c10de1a: *stop=0x8c10de1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10de58u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10de1c; /* mov.l 0x8c10de58,r1 */
L8c10de1c: *stop=0x8c10de1cu;r[0]=r[0]&255u;goto L8c10de1e; /* extu.b r0,r0 */
L8c10de1e: *stop=0x8c10de1eu;if(!valid_access(r[0]+r[1],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10de20; /* mov.b @(r0,r1),r0 */
L8c10de20: *stop=0x8c10de20u;r[0]=r[0]&255u;goto L8c10de22; /* extu.b r0,r0 */
L8c10de22: *stop=0x8c10de22u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c10de24; /* tst #0x4,r0 */
L8c10de24: if((*sr&1u))goto L8c10de78;goto L8c10de26; /* bt 0x8c10de78 */
L8c10de26: *stop=0x8c10de28u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[11],4);if(io_status)return io_status;goto L8c10de6a; /* bra 0x8c10de6a */
L8c10de2a: *stop=0x8c10de2au;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[4]=loaded;}goto L8c10de2c; /* mov.b @r12,r4 */
L8c10de2c: *stop=0x8c10de2cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10de52u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10de2e; /* mov.w 0x8c10de52,r1 */
L8c10de2e: *stop=0x8c10de2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10de5cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10de30; /* mov.l 0x8c10de5c,r3 */
L8c10de30: *stop=0x8c10de30u;r[4]+=4294967248u;goto L8c10de32; /* add #-0x30,r4 */
L8c10de32: *stop=0x8c10de32u;r[1]-=r[4];goto L8c10de34; /* sub r4,r1 */
L8c10de34: target=r[3];*pr=0x8c10de38u;*stop=0x8c10de36u;r[0]=10u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10de38; /* jsr @r3 */
L8c10de38: *stop=0x8c10de38u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10de3a; /* mov.l @(0x8,r14),r2 */
L8c10de3a: *stop=0x8c10de3au;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[2]);goto L8c10de3c; /* cmp/ge r2,r0 */
L8c10de3c: take=!(*sr&1u);*stop=0x8c10de3eu;r[12]+=1u;if(take)goto L8c10de60;goto L8c10de40; /* bf/s 0x8c10de60 */
L8c10de40: *stop=0x8c10de40u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10de42; /* mov.l @(0x8,r14),r1 */
L8c10de42: *stop=0x8c10de42u;r[3]=r[1];goto L8c10de44; /* mov r1,r3 */
L8c10de44: *stop=0x8c10de44u;r[1]<<=2;goto L8c10de46; /* shll2 r1 */
L8c10de46: *stop=0x8c10de46u;r[1]+=r[3];goto L8c10de48; /* add r3,r1 */
L8c10de48: *stop=0x8c10de48u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c10de4a; /* shll r1 */
L8c10de4a: *stop=0x8c10de4au;r[1]+=r[4];goto L8c10de4c; /* add r4,r1 */
L8c10de4c: *stop=0x8c10de4eu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[1],4);if(io_status)return io_status;goto L8c10de6a; /* bra 0x8c10de6a */
L8c10de60: *stop=0x8c10de60u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10de62; /* mov.b @r14,r0 */
L8c10de62: *stop=0x8c10de62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10df34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10de64; /* mov.l 0x8c10df34,r3 */
L8c10de64: *stop=0x8c10de64u;r[0]|=0x1u;goto L8c10de66; /* or #0x1,r0 */
L8c10de66: *stop=0x8c10de66u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c10de68; /* mov.b r0,@r14 */
L8c10de68: *stop=0x8c10de68u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[8],4);if(io_status)return io_status;goto L8c10de6a; /* mov.l r8,@r3 */
L8c10de6a: *stop=0x8c10de6au;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10de6c; /* mov.b @r12,r0 */
L8c10de6c: *stop=0x8c10de6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10df38u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10de6e; /* mov.l 0x8c10df38,r1 */
L8c10de6e: *stop=0x8c10de6eu;r[0]=r[0]&255u;goto L8c10de70; /* extu.b r0,r0 */
L8c10de70: *stop=0x8c10de70u;if(!valid_access(r[0]+r[1],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10de72; /* mov.b @(r0,r1),r0 */
L8c10de72: *stop=0x8c10de72u;r[0]=r[0]&255u;goto L8c10de74; /* extu.b r0,r0 */
L8c10de74: *stop=0x8c10de74u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c10de76; /* tst #0x4,r0 */
L8c10de76: if(!(*sr&1u))goto L8c10de2a;goto L8c10de78; /* bf 0x8c10de2a */
L8c10de78: *stop=0x8c10de78u;r[2]=32u;goto L8c10de7a; /* mov #0x20,r2 */
L8c10de7a: *stop=0x8c10de7au;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c10de7c; /* mov.l r2,@(0xc,r14) */
L8c10de7c: *stop=0x8c10de7cu;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10de7e; /* mov.b @r12,r0 */
L8c10de7e: *stop=0x8c10de7eu;*sr=(*sr&~1u)|(r[0]==0x00000068u);goto L8c10de80; /* cmp/eq #0x68,r0 */
L8c10de80: take=(*sr&1u);*stop=0x8c10de82u;r[4]=r[0];if(take)goto L8c10de90;goto L8c10de84; /* bt/s 0x8c10de90 */
L8c10de84: *stop=0x8c10de84u;r[0]=r[4];goto L8c10de86; /* mov r4,r0 */
L8c10de86: *stop=0x8c10de86u;*sr=(*sr&~1u)|(r[0]==0x0000006cu);goto L8c10de88; /* cmp/eq #0x6c,r0 */
L8c10de88: if((*sr&1u))goto L8c10de90;goto L8c10de8a; /* bt 0x8c10de90 */
L8c10de8a: *stop=0x8c10de8au;r[0]=r[4];goto L8c10de8c; /* mov r4,r0 */
L8c10de8c: *stop=0x8c10de8cu;*sr=(*sr&~1u)|(r[0]==0x0000004cu);goto L8c10de8e; /* cmp/eq #0x4c,r0 */
L8c10de8e: if(!(*sr&1u))goto L8c10de94;goto L8c10de90; /* bf 0x8c10de94 */
L8c10de90: *stop=0x8c10de90u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[2]=loaded;}r[12]+=1;goto L8c10de92; /* mov.b @r12+,r2 */
L8c10de92: *stop=0x8c10de92u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c10de94; /* mov.l r2,@(0xc,r14) */
L8c10de94: *stop=0x8c10de94u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10de96; /* mov.b @r12,r0 */
L8c10de96: *stop=0x8c10de96u;*sr=(*sr&~1u)|(r[0]==0x00000064u);goto L8c10de98; /* cmp/eq #0x64,r0 */
L8c10de98: if((*sr&1u))goto L8c10df02;goto L8c10de9a; /* bt 0x8c10df02 */
L8c10de9a: *stop=0x8c10de9au;*sr=(*sr&~1u)|(r[0]==0x00000069u);goto L8c10de9c; /* cmp/eq #0x69,r0 */
L8c10de9c: if((*sr&1u))goto L8c10df02;goto L8c10de9e; /* bt 0x8c10df02 */
L8c10de9e: *stop=0x8c10de9eu;*sr=(*sr&~1u)|(r[0]==0x0000006fu);goto L8c10dea0; /* cmp/eq #0x6f,r0 */
L8c10dea0: if((*sr&1u))goto L8c10df02;goto L8c10dea2; /* bt 0x8c10df02 */
L8c10dea2: *stop=0x8c10dea2u;*sr=(*sr&~1u)|(r[0]==0x00000075u);goto L8c10dea4; /* cmp/eq #0x75,r0 */
L8c10dea4: if((*sr&1u))goto L8c10df02;goto L8c10dea6; /* bt 0x8c10df02 */
L8c10dea6: *stop=0x8c10dea6u;*sr=(*sr&~1u)|(r[0]==0x00000078u);goto L8c10dea8; /* cmp/eq #0x78,r0 */
L8c10dea8: if((*sr&1u))goto L8c10df02;goto L8c10deaa; /* bt 0x8c10df02 */
L8c10deaa: *stop=0x8c10deaau;*sr=(*sr&~1u)|(r[0]==0x00000058u);goto L8c10deac; /* cmp/eq #0x58,r0 */
L8c10deac: if((*sr&1u))goto L8c10df02;goto L8c10deae; /* bt 0x8c10df02 */
L8c10deae: *stop=0x8c10deaeu;*sr=(*sr&~1u)|(r[0]==0x00000066u);goto L8c10deb0; /* cmp/eq #0x66,r0 */
L8c10deb0: if(!(*sr&1u))goto L8c10deb6;goto L8c10deb2; /* bf 0x8c10deb6 */
L8c10deb2: *stop=0x8c10deb4u;goto L8c10e040; /* bra 0x8c10e040 */
L8c10deb6: *stop=0x8c10deb6u;*sr=(*sr&~1u)|(r[0]==0x00000065u);goto L8c10deb8; /* cmp/eq #0x65,r0 */
L8c10deb8: if(!(*sr&1u))goto L8c10debe;goto L8c10deba; /* bf 0x8c10debe */
L8c10deba: *stop=0x8c10debcu;goto L8c10e040; /* bra 0x8c10e040 */
L8c10debe: *stop=0x8c10debeu;*sr=(*sr&~1u)|(r[0]==0x00000045u);goto L8c10dec0; /* cmp/eq #0x45,r0 */
L8c10dec0: if(!(*sr&1u))goto L8c10dec6;goto L8c10dec2; /* bf 0x8c10dec6 */
L8c10dec2: *stop=0x8c10dec4u;goto L8c10e040; /* bra 0x8c10e040 */
L8c10dec6: *stop=0x8c10dec6u;*sr=(*sr&~1u)|(r[0]==0x00000067u);goto L8c10dec8; /* cmp/eq #0x67,r0 */
L8c10dec8: if(!(*sr&1u))goto L8c10dece;goto L8c10deca; /* bf 0x8c10dece */
L8c10deca: *stop=0x8c10deccu;goto L8c10e040; /* bra 0x8c10e040 */
L8c10dece: *stop=0x8c10deceu;*sr=(*sr&~1u)|(r[0]==0x00000047u);goto L8c10ded0; /* cmp/eq #0x47,r0 */
L8c10ded0: if(!(*sr&1u))goto L8c10ded6;goto L8c10ded2; /* bf 0x8c10ded6 */
L8c10ded2: *stop=0x8c10ded4u;goto L8c10e040; /* bra 0x8c10e040 */
L8c10ded6: *stop=0x8c10ded6u;*sr=(*sr&~1u)|(r[0]==0x00000063u);goto L8c10ded8; /* cmp/eq #0x63,r0 */
L8c10ded8: if(!(*sr&1u))goto L8c10dede;goto L8c10deda; /* bf 0x8c10dede */
L8c10deda: *stop=0x8c10dedcu;goto L8c10e062; /* bra 0x8c10e062 */
L8c10dede: *stop=0x8c10dedeu;*sr=(*sr&~1u)|(r[0]==0x00000073u);goto L8c10dee0; /* cmp/eq #0x73,r0 */
L8c10dee0: if(!(*sr&1u))goto L8c10dee6;goto L8c10dee2; /* bf 0x8c10dee6 */
L8c10dee2: *stop=0x8c10dee4u;goto L8c10e094; /* bra 0x8c10e094 */
L8c10dee6: *stop=0x8c10dee6u;*sr=(*sr&~1u)|(r[0]==0x00000070u);goto L8c10dee8; /* cmp/eq #0x70,r0 */
L8c10dee8: if(!(*sr&1u))goto L8c10deee;goto L8c10deea; /* bf 0x8c10deee */
L8c10deea: *stop=0x8c10deecu;goto L8c10e0ec; /* bra 0x8c10e0ec */
L8c10deee: *stop=0x8c10deeeu;*sr=(*sr&~1u)|(r[0]==0x0000006eu);goto L8c10def0; /* cmp/eq #0x6e,r0 */
L8c10def0: if(!(*sr&1u))goto L8c10def6;goto L8c10def2; /* bf 0x8c10def6 */
L8c10def2: *stop=0x8c10def4u;goto L8c10e148; /* bra 0x8c10e148 */
L8c10def6: *stop=0x8c10def6u;*sr=(*sr&~1u)|(r[0]==0x00000025u);goto L8c10def8; /* cmp/eq #0x25,r0 */
L8c10def8: if(!(*sr&1u))goto L8c10defe;goto L8c10defa; /* bf 0x8c10defe */
L8c10defa: *stop=0x8c10defcu;goto L8c10e17c; /* bra 0x8c10e17c */
L8c10defe: *stop=0x8c10df00u;goto L8c10e194; /* bra 0x8c10e194 */
L8c10df02: *stop=0x8c10df02u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10df04; /* mov.l @(0xc,r14),r0 */
L8c10df04: *stop=0x8c10df04u;*sr=(*sr&~1u)|(r[0]==0x0000006cu);goto L8c10df06; /* cmp/eq #0x6c,r0 */
L8c10df06: if(!(*sr&1u))goto L8c10df3c;goto L8c10df08; /* bf 0x8c10df3c */
L8c10df08: *stop=0x8c10df08u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10df32u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df0a; /* mov.w 0x8c10df32,r0 */
L8c10df0a: *stop=0x8c10df0au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c10df0c; /* mov.l @(r0,r15),r6 */
L8c10df0c: *stop=0x8c10df0cu;r[6]+=4u;goto L8c10df0e; /* add #0x4,r6 */
L8c10df0e: *stop=0x8c10df0eu;r[4]=r[6];goto L8c10df10; /* mov r6,r4 */
L8c10df10: *stop=0x8c10df10u;*sr=(*sr&~1u)|!(r[13]&r[4]);goto L8c10df12; /* tst r13,r4 */
L8c10df12: if((*sr&1u))goto L8c10df1a;goto L8c10df14; /* bt 0x8c10df1a */
L8c10df14: *stop=0x8c10df14u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10df32u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df16; /* mov.w 0x8c10df32,r0 */
L8c10df16: *stop=0x8c10df16u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10df18; /* mov.l @(r0,r15),r4 */
L8c10df18: *stop=0x8c10df18u;r[4]+=8u;goto L8c10df1a; /* add #0x8,r4 */
L8c10df1a: *stop=0x8c10df1au;{uint32_t loaded=sx(m,bus,&io_status,0x8c10df32u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df1c; /* mov.w 0x8c10df32,r0 */
L8c10df1c: *stop=0x8c10df1cu;r[5]=r[4];goto L8c10df1e; /* mov r4,r5 */
L8c10df1e: *stop=0x8c10df1eu;r[5]+=4294967292u;goto L8c10df20; /* add #-0x4,r5 */
L8c10df20: *stop=0x8c10df20u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[4],4);if(io_status)return io_status;goto L8c10df22; /* mov.l r4,@(r0,r15) */
L8c10df22: *stop=0x8c10df22u;r[4]=r[5];goto L8c10df24; /* mov r5,r4 */
L8c10df24: *stop=0x8c10df24u;*sr=(*sr&~1u)|!(r[13]&r[4]);goto L8c10df26; /* tst r13,r4 */
L8c10df26: if((*sr&1u))goto L8c10df2e;goto L8c10df28; /* bt 0x8c10df2e */
L8c10df28: *stop=0x8c10df28u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10df32u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df2a; /* mov.w 0x8c10df32,r0 */
L8c10df2a: *stop=0x8c10df2au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10df2c; /* mov.l @(r0,r15),r4 */
L8c10df2c: *stop=0x8c10df2cu;r[4]+=4294967288u;goto L8c10df2e; /* add #-0x8,r4 */
L8c10df2e: *stop=0x8c10df30u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e022; /* bra 0x8c10e022 */
L8c10df3c: *stop=0x8c10df3cu;*sr=(*sr&~1u)|(r[0]==0x00000068u);goto L8c10df3e; /* cmp/eq #0x68,r0 */
L8c10df3e: if(!(*sr&1u))goto L8c10dfb2;goto L8c10df40; /* bf 0x8c10dfb2 */
L8c10df40: *stop=0x8c10df40u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10df42; /* mov.b @r12,r0 */
L8c10df42: *stop=0x8c10df42u;*sr=(*sr&~1u)|(r[0]==0x00000075u);goto L8c10df44; /* cmp/eq #0x75,r0 */
L8c10df44: take=(*sr&1u);*stop=0x8c10df46u;r[4]=r[0];if(take)goto L8c10df5a;goto L8c10df48; /* bt/s 0x8c10df5a */
L8c10df48: *stop=0x8c10df48u;r[0]=r[4];goto L8c10df4a; /* mov r4,r0 */
L8c10df4a: *stop=0x8c10df4au;*sr=(*sr&~1u)|(r[0]==0x00000058u);goto L8c10df4c; /* cmp/eq #0x58,r0 */
L8c10df4c: if((*sr&1u))goto L8c10df5a;goto L8c10df4e; /* bt 0x8c10df5a */
L8c10df4e: *stop=0x8c10df4eu;r[0]=r[4];goto L8c10df50; /* mov r4,r0 */
L8c10df50: *stop=0x8c10df50u;*sr=(*sr&~1u)|(r[0]==0x00000078u);goto L8c10df52; /* cmp/eq #0x78,r0 */
L8c10df52: if((*sr&1u))goto L8c10df5a;goto L8c10df54; /* bt 0x8c10df5a */
L8c10df54: *stop=0x8c10df54u;r[0]=r[4];goto L8c10df56; /* mov r4,r0 */
L8c10df56: *stop=0x8c10df56u;*sr=(*sr&~1u)|(r[0]==0x0000006fu);goto L8c10df58; /* cmp/eq #0x6f,r0 */
L8c10df58: if(!(*sr&1u))goto L8c10df86;goto L8c10df5a; /* bf 0x8c10df86 */
L8c10df5a: *stop=0x8c10df5au;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df5c; /* mov.w 0x8c10dff8,r0 */
L8c10df5c: *stop=0x8c10df5cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10df5e; /* mov.l @(r0,r15),r5 */
L8c10df5e: *stop=0x8c10df5eu;r[5]+=4u;goto L8c10df60; /* add #0x4,r5 */
L8c10df60: *stop=0x8c10df60u;r[4]=r[5];goto L8c10df62; /* mov r5,r4 */
L8c10df62: *stop=0x8c10df62u;*sr=(*sr&~1u)|!(r[13]&r[4]);goto L8c10df64; /* tst r13,r4 */
L8c10df64: if((*sr&1u))goto L8c10df6c;goto L8c10df66; /* bt 0x8c10df6c */
L8c10df66: *stop=0x8c10df66u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df68; /* mov.w 0x8c10dff8,r0 */
L8c10df68: *stop=0x8c10df68u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10df6a; /* mov.l @(r0,r15),r4 */
L8c10df6a: *stop=0x8c10df6au;r[4]+=8u;goto L8c10df6c; /* add #0x8,r4 */
L8c10df6c: *stop=0x8c10df6cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df6e; /* mov.w 0x8c10dff8,r0 */
L8c10df6e: *stop=0x8c10df6eu;r[6]=r[4];goto L8c10df70; /* mov r4,r6 */
L8c10df70: *stop=0x8c10df70u;r[6]+=4294967292u;goto L8c10df72; /* add #-0x4,r6 */
L8c10df72: *stop=0x8c10df72u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[4],4);if(io_status)return io_status;goto L8c10df74; /* mov.l r4,@(r0,r15) */
L8c10df74: *stop=0x8c10df74u;r[4]=r[6];goto L8c10df76; /* mov r6,r4 */
L8c10df76: *stop=0x8c10df76u;*sr=(*sr&~1u)|!(r[13]&r[4]);goto L8c10df78; /* tst r13,r4 */
L8c10df78: if((*sr&1u))goto L8c10df80;goto L8c10df7a; /* bt 0x8c10df80 */
L8c10df7a: *stop=0x8c10df7au;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df7c; /* mov.w 0x8c10dff8,r0 */
L8c10df7c: *stop=0x8c10df7cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10df7e; /* mov.l @(r0,r15),r4 */
L8c10df7e: *stop=0x8c10df7eu;r[4]+=4294967288u;goto L8c10df80; /* add #-0x8,r4 */
L8c10df80: *stop=0x8c10df80u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10df82; /* mov.l @r4,r4 */
L8c10df82: *stop=0x8c10df84u;r[4]=r[4]&65535u;goto L8c10e022; /* bra 0x8c10e022 */
L8c10df86: *stop=0x8c10df86u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df88; /* mov.w 0x8c10dff8,r0 */
L8c10df88: *stop=0x8c10df88u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10df8a; /* mov.l @(r0,r15),r5 */
L8c10df8a: *stop=0x8c10df8au;r[5]+=4u;goto L8c10df8c; /* add #0x4,r5 */
L8c10df8c: *stop=0x8c10df8cu;r[4]=r[5];goto L8c10df8e; /* mov r5,r4 */
L8c10df8e: *stop=0x8c10df8eu;*sr=(*sr&~1u)|!(r[13]&r[4]);goto L8c10df90; /* tst r13,r4 */
L8c10df90: if((*sr&1u))goto L8c10df98;goto L8c10df92; /* bt 0x8c10df98 */
L8c10df92: *stop=0x8c10df92u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df94; /* mov.w 0x8c10dff8,r0 */
L8c10df94: *stop=0x8c10df94u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10df96; /* mov.l @(r0,r15),r4 */
L8c10df96: *stop=0x8c10df96u;r[4]+=8u;goto L8c10df98; /* add #0x8,r4 */
L8c10df98: *stop=0x8c10df98u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10df9a; /* mov.w 0x8c10dff8,r0 */
L8c10df9a: *stop=0x8c10df9au;r[6]=r[4];goto L8c10df9c; /* mov r4,r6 */
L8c10df9c: *stop=0x8c10df9cu;r[6]+=4294967292u;goto L8c10df9e; /* add #-0x4,r6 */
L8c10df9e: *stop=0x8c10df9eu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[4],4);if(io_status)return io_status;goto L8c10dfa0; /* mov.l r4,@(r0,r15) */
L8c10dfa0: *stop=0x8c10dfa0u;r[4]=r[6];goto L8c10dfa2; /* mov r6,r4 */
L8c10dfa2: *stop=0x8c10dfa2u;*sr=(*sr&~1u)|!(r[13]&r[4]);goto L8c10dfa4; /* tst r13,r4 */
L8c10dfa4: if((*sr&1u))goto L8c10dfac;goto L8c10dfa6; /* bt 0x8c10dfac */
L8c10dfa6: *stop=0x8c10dfa6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dfa8; /* mov.w 0x8c10dff8,r0 */
L8c10dfa8: *stop=0x8c10dfa8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10dfaa; /* mov.l @(r0,r15),r4 */
L8c10dfaa: *stop=0x8c10dfaau;r[4]+=4294967288u;goto L8c10dfac; /* add #-0x8,r4 */
L8c10dfac: *stop=0x8c10dfacu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10dfae; /* mov.l @r4,r4 */
L8c10dfae: *stop=0x8c10dfb0u;r[4]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c10e022; /* bra 0x8c10e022 */
L8c10dfb2: *stop=0x8c10dfb2u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10dfb4; /* mov.b @r12,r0 */
L8c10dfb4: *stop=0x8c10dfb4u;*sr=(*sr&~1u)|(r[0]==0x00000075u);goto L8c10dfb6; /* cmp/eq #0x75,r0 */
L8c10dfb6: take=(*sr&1u);*stop=0x8c10dfb8u;r[4]=r[0];if(take)goto L8c10dfcc;goto L8c10dfba; /* bt/s 0x8c10dfcc */
L8c10dfba: *stop=0x8c10dfbau;r[0]=r[4];goto L8c10dfbc; /* mov r4,r0 */
L8c10dfbc: *stop=0x8c10dfbcu;*sr=(*sr&~1u)|(r[0]==0x00000058u);goto L8c10dfbe; /* cmp/eq #0x58,r0 */
L8c10dfbe: if((*sr&1u))goto L8c10dfcc;goto L8c10dfc0; /* bt 0x8c10dfcc */
L8c10dfc0: *stop=0x8c10dfc0u;r[0]=r[4];goto L8c10dfc2; /* mov r4,r0 */
L8c10dfc2: *stop=0x8c10dfc2u;*sr=(*sr&~1u)|(r[0]==0x00000078u);goto L8c10dfc4; /* cmp/eq #0x78,r0 */
L8c10dfc4: if((*sr&1u))goto L8c10dfcc;goto L8c10dfc6; /* bt 0x8c10dfcc */
L8c10dfc6: *stop=0x8c10dfc6u;r[0]=r[4];goto L8c10dfc8; /* mov r4,r0 */
L8c10dfc8: *stop=0x8c10dfc8u;*sr=(*sr&~1u)|(r[0]==0x0000006fu);goto L8c10dfca; /* cmp/eq #0x6f,r0 */
L8c10dfca: if(!(*sr&1u))goto L8c10dffa;goto L8c10dfcc; /* bf 0x8c10dffa */
L8c10dfcc: *stop=0x8c10dfccu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dfce; /* mov.w 0x8c10dff8,r0 */
L8c10dfce: *stop=0x8c10dfceu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10dfd0; /* mov.l @(r0,r15),r5 */
L8c10dfd0: *stop=0x8c10dfd0u;r[5]+=4u;goto L8c10dfd2; /* add #0x4,r5 */
L8c10dfd2: *stop=0x8c10dfd2u;r[4]=r[5];goto L8c10dfd4; /* mov r5,r4 */
L8c10dfd4: *stop=0x8c10dfd4u;*sr=(*sr&~1u)|!(r[13]&r[4]);goto L8c10dfd6; /* tst r13,r4 */
L8c10dfd6: if((*sr&1u))goto L8c10dfde;goto L8c10dfd8; /* bt 0x8c10dfde */
L8c10dfd8: *stop=0x8c10dfd8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dfda; /* mov.w 0x8c10dff8,r0 */
L8c10dfda: *stop=0x8c10dfdau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10dfdc; /* mov.l @(r0,r15),r4 */
L8c10dfdc: *stop=0x8c10dfdcu;r[4]+=8u;goto L8c10dfde; /* add #0x8,r4 */
L8c10dfde: *stop=0x8c10dfdeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dfe0; /* mov.w 0x8c10dff8,r0 */
L8c10dfe0: *stop=0x8c10dfe0u;r[6]=r[4];goto L8c10dfe2; /* mov r4,r6 */
L8c10dfe2: *stop=0x8c10dfe2u;r[6]+=4294967292u;goto L8c10dfe4; /* add #-0x4,r6 */
L8c10dfe4: *stop=0x8c10dfe4u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[4],4);if(io_status)return io_status;goto L8c10dfe6; /* mov.l r4,@(r0,r15) */
L8c10dfe6: *stop=0x8c10dfe6u;r[4]=r[6];goto L8c10dfe8; /* mov r6,r4 */
L8c10dfe8: *stop=0x8c10dfe8u;*sr=(*sr&~1u)|!(r[13]&r[4]);goto L8c10dfea; /* tst r13,r4 */
L8c10dfea: if((*sr&1u))goto L8c10e020;goto L8c10dfec; /* bt 0x8c10e020 */
L8c10dfec: *stop=0x8c10dfecu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10dff8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dfee; /* mov.w 0x8c10dff8,r0 */
L8c10dfee: *stop=0x8c10dfeeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10dff0; /* mov.l @(r0,r15),r4 */
L8c10dff0: *stop=0x8c10dff2u;r[4]+=4294967288u;goto L8c10e020; /* bra 0x8c10e020 */
L8c10dffa: *stop=0x8c10dffau;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10dffc; /* mov.w 0x8c10e100,r0 */
L8c10dffc: *stop=0x8c10dffcu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10dffe; /* mov.l @(r0,r15),r5 */
L8c10dffe: *stop=0x8c10dffeu;r[5]+=4u;goto L8c10e000; /* add #0x4,r5 */
L8c10e000: *stop=0x8c10e000u;r[4]=r[5];goto L8c10e002; /* mov r5,r4 */
L8c10e002: *stop=0x8c10e002u;*sr=(*sr&~1u)|!(r[13]&r[4]);goto L8c10e004; /* tst r13,r4 */
L8c10e004: if((*sr&1u))goto L8c10e00c;goto L8c10e006; /* bt 0x8c10e00c */
L8c10e006: *stop=0x8c10e006u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e008; /* mov.w 0x8c10e100,r0 */
L8c10e008: *stop=0x8c10e008u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e00a; /* mov.l @(r0,r15),r4 */
L8c10e00a: *stop=0x8c10e00au;r[4]+=8u;goto L8c10e00c; /* add #0x8,r4 */
L8c10e00c: *stop=0x8c10e00cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e00e; /* mov.w 0x8c10e100,r0 */
L8c10e00e: *stop=0x8c10e00eu;r[6]=r[4];goto L8c10e010; /* mov r4,r6 */
L8c10e010: *stop=0x8c10e010u;r[6]+=4294967292u;goto L8c10e012; /* add #-0x4,r6 */
L8c10e012: *stop=0x8c10e012u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[4],4);if(io_status)return io_status;goto L8c10e014; /* mov.l r4,@(r0,r15) */
L8c10e014: *stop=0x8c10e014u;r[4]=r[6];goto L8c10e016; /* mov r6,r4 */
L8c10e016: *stop=0x8c10e016u;*sr=(*sr&~1u)|!(r[13]&r[4]);goto L8c10e018; /* tst r13,r4 */
L8c10e018: if((*sr&1u))goto L8c10e020;goto L8c10e01a; /* bt 0x8c10e020 */
L8c10e01a: *stop=0x8c10e01au;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e01c; /* mov.w 0x8c10e100,r0 */
L8c10e01c: *stop=0x8c10e01cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e01e; /* mov.l @(r0,r15),r4 */
L8c10e01e: *stop=0x8c10e01eu;r[4]+=4294967288u;goto L8c10e020; /* add #-0x8,r4 */
L8c10e020: *stop=0x8c10e020u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e022; /* mov.l @r4,r4 */
L8c10e022: *stop=0x8c10e022u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10e024; /* mov.l @(0x8,r14),r0 */
L8c10e024: *stop=0x8c10e024u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10e026; /* cmp/eq #-0x1,r0 */
L8c10e026: take=!(*sr&1u);*stop=0x8c10e028u;r[7]=r[14];if(take)goto L8c10e02e;goto L8c10e02a; /* bf/s 0x8c10e02e */
L8c10e02a: *stop=0x8c10e02au;r[3]=1u;goto L8c10e02c; /* mov #0x1,r3 */
L8c10e02c: *stop=0x8c10e02cu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10e02e; /* mov.l r3,@(0x8,r14) */
L8c10e02e: *stop=0x8c10e02eu;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[6]=loaded;}goto L8c10e030; /* mov.b @r12,r6 */
L8c10e030: target=0x8c10e304u;*pr=0x8c10e034u;*stop=0x8c10e032u;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e034; /* bsr 0x8c10e304 */
L8c10e034: *stop=0x8c10e034u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e104u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e036; /* mov.l 0x8c10e104,r3 */
L8c10e036: *stop=0x8c10e036u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c10e038; /* mov.l r9,@(0x4,r15) */
L8c10e038: target=r[3];*pr=0x8c10e03cu;*stop=0x8c10e03au;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e03c; /* jsr @r3 */
L8c10e03c: *stop=0x8c10e03eu;goto L8c10e144; /* bra 0x8c10e144 */
L8c10e040: *stop=0x8c10e040u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c10e042; /* mov.w 0x8c10e100,r4 */
L8c10e042: *stop=0x8c10e042u;r[7]=r[14];goto L8c10e044; /* mov r14,r7 */
L8c10e044: *stop=0x8c10e044u;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[6]=loaded;}goto L8c10e046; /* mov.b @r12,r6 */
L8c10e046: *stop=0x8c10e046u;r[5]=r[9];goto L8c10e048; /* mov r9,r5 */
L8c10e048: *stop=0x8c10e048u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e108u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e04a; /* mov.l 0x8c10e108,r2 */
L8c10e04a: target=r[2];*pr=0x8c10e04eu;*stop=0x8c10e04cu;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e04e; /* jsr @r2 */
L8c10e04e: *stop=0x8c10e04eu;r[4]=f->fpscr;goto L8c10e050; /* sts FPSCR,r4 */
L8c10e050: *stop=0x8c10e050u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e10cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e052; /* mov.l 0x8c10e10c,r3 */
L8c10e052: *stop=0x8c10e052u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e104u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e054; /* mov.l 0x8c10e104,r2 */
L8c10e054: *stop=0x8c10e054u;r[4]&=r[3];goto L8c10e056; /* and r3,r4 */
L8c10e056: *stop=0x8c10e056u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c10e058; /* mov.l r9,@(0x4,r15) */
L8c10e058: *stop=0x8c10e058u;f->fpscr=r[4];goto L8c10e05a; /* lds r4,fpscr */
L8c10e05a: target=r[2];*pr=0x8c10e05eu;*stop=0x8c10e05cu;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e05e; /* jsr @r2 */
L8c10e05e: *stop=0x8c10e060u;goto L8c10e144; /* bra 0x8c10e144 */
L8c10e062: *stop=0x8c10e062u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e064; /* mov.w 0x8c10e100,r0 */
L8c10e064: *stop=0x8c10e064u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e066; /* mov.l @(0x10,r15),r2 */
L8c10e066: *stop=0x8c10e066u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e068; /* mov.l @(r0,r15),r4 */
L8c10e068: *stop=0x8c10e068u;r[4]+=4u;goto L8c10e06a; /* add #0x4,r4 */
L8c10e06a: *stop=0x8c10e06au;r[3]=r[4];goto L8c10e06c; /* mov r4,r3 */
L8c10e06c: *stop=0x8c10e06cu;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c10e06e; /* tst r13,r3 */
L8c10e06e: if((*sr&1u))goto L8c10e076;goto L8c10e070; /* bt 0x8c10e076 */
L8c10e070: *stop=0x8c10e070u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e072; /* mov.w 0x8c10e100,r0 */
L8c10e072: *stop=0x8c10e072u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e074; /* mov.l @(r0,r15),r3 */
L8c10e074: *stop=0x8c10e074u;r[3]+=8u;goto L8c10e076; /* add #0x8,r3 */
L8c10e076: *stop=0x8c10e076u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e078; /* mov.w 0x8c10e100,r0 */
L8c10e078: *stop=0x8c10e078u;r[5]=r[3];goto L8c10e07a; /* mov r3,r5 */
L8c10e07a: *stop=0x8c10e07au;r[5]+=4294967292u;goto L8c10e07c; /* add #-0x4,r5 */
L8c10e07c: *stop=0x8c10e07cu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c10e07e; /* mov.l r3,@(r0,r15) */
L8c10e07e: *stop=0x8c10e07eu;r[0]=r[5];goto L8c10e080; /* mov r5,r0 */
L8c10e080: *stop=0x8c10e080u;*sr=(*sr&~1u)|!(r[13]&r[0]);goto L8c10e082; /* tst r13,r0 */
L8c10e082: if((*sr&1u))goto L8c10e08a;goto L8c10e084; /* bt 0x8c10e08a */
L8c10e084: *stop=0x8c10e084u;r[1]=r[3];goto L8c10e086; /* mov r3,r1 */
L8c10e086: *stop=0x8c10e088u;r[1]+=4294967288u;goto L8c10e08c; /* bra 0x8c10e08c */
L8c10e08a: *stop=0x8c10e08au;r[1]=r[5];goto L8c10e08c; /* mov r5,r1 */
L8c10e08c: *stop=0x8c10e08cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10e08e; /* mov.l @r1,r0 */
L8c10e08e: *stop=0x8c10e08eu;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[0],1);if(io_status)return io_status;goto L8c10e090; /* mov.b r0,@r2 */
L8c10e090: *stop=0x8c10e092u;goto L8c10e182; /* bra 0x8c10e182 */
L8c10e094: *stop=0x8c10e094u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e096; /* mov.w 0x8c10e100,r0 */
L8c10e096: *stop=0x8c10e096u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e098; /* mov.l @(r0,r15),r4 */
L8c10e098: *stop=0x8c10e098u;r[4]+=4u;goto L8c10e09a; /* add #0x4,r4 */
L8c10e09a: *stop=0x8c10e09au;r[3]=r[4];goto L8c10e09c; /* mov r4,r3 */
L8c10e09c: *stop=0x8c10e09cu;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c10e09e; /* tst r13,r3 */
L8c10e09e: if((*sr&1u))goto L8c10e0a8;goto L8c10e0a0; /* bt 0x8c10e0a8 */
L8c10e0a0: *stop=0x8c10e0a0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e0a2; /* mov.w 0x8c10e100,r0 */
L8c10e0a2: *stop=0x8c10e0a2u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10e0a4; /* mov.l @(r0,r15),r1 */
L8c10e0a4: *stop=0x8c10e0a6u;r[1]+=8u;goto L8c10e0aa; /* bra 0x8c10e0aa */
L8c10e0a8: *stop=0x8c10e0a8u;r[1]=r[4];goto L8c10e0aa; /* mov r4,r1 */
L8c10e0aa: *stop=0x8c10e0aau;r[5]=r[1];goto L8c10e0ac; /* mov r1,r5 */
L8c10e0ac: *stop=0x8c10e0acu;r[5]+=4294967292u;goto L8c10e0ae; /* add #-0x4,r5 */
L8c10e0ae: *stop=0x8c10e0aeu;r[2]=r[5];goto L8c10e0b0; /* mov r5,r2 */
L8c10e0b0: *stop=0x8c10e0b0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e0b2; /* mov.w 0x8c10e100,r0 */
L8c10e0b2: *stop=0x8c10e0b2u;*sr=(*sr&~1u)|!(r[13]&r[2]);goto L8c10e0b4; /* tst r13,r2 */
L8c10e0b4: take=(*sr&1u);*stop=0x8c10e0b6u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;if(take)goto L8c10e0be;goto L8c10e0b8; /* bt/s 0x8c10e0be */
L8c10e0b8: *stop=0x8c10e0b8u;r[3]=r[1];goto L8c10e0ba; /* mov r1,r3 */
L8c10e0ba: *stop=0x8c10e0bcu;r[3]+=4294967288u;goto L8c10e0c0; /* bra 0x8c10e0c0 */
L8c10e0be: *stop=0x8c10e0beu;r[3]=r[5];goto L8c10e0c0; /* mov r5,r3 */
L8c10e0c0: *stop=0x8c10e0c0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10e0c2; /* mov.l @r3,r1 */
L8c10e0c2: *stop=0x8c10e0c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e104u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e0c4; /* mov.l 0x8c10e104,r3 */
L8c10e0c4: *stop=0x8c10e0c4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c10e0c6; /* mov.l r1,@(0x4,r15) */
L8c10e0c6: target=r[3];*pr=0x8c10e0cau;*stop=0x8c10e0c8u;r[4]=r[1];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e0ca; /* jsr @r3 */
L8c10e0ca: *stop=0x8c10e0cau;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c10e0cc; /* mov.l r0,@(0xc,r15) */
L8c10e0cc: *stop=0x8c10e0ccu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10e0ce; /* mov.l @(0x8,r14),r0 */
L8c10e0ce: *stop=0x8c10e0ceu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10e0d0; /* cmp/eq #-0x1,r0 */
L8c10e0d0: take=(*sr&1u);*stop=0x8c10e0d2u;r[4]=r[0];if(take)goto L8c10e0de;goto L8c10e0d4; /* bt/s 0x8c10e0de */
L8c10e0d4: *stop=0x8c10e0d4u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e0d6; /* mov.l @(0xc,r15),r2 */
L8c10e0d6: *stop=0x8c10e0d6u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]);goto L8c10e0d8; /* cmp/ge r2,r4 */
L8c10e0d8: if((*sr&1u))goto L8c10e0de;goto L8c10e0da; /* bt 0x8c10e0de */
L8c10e0da: *stop=0x8c10e0dau;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10e0dc; /* mov.l @(0x8,r14),r1 */
L8c10e0dc: *stop=0x8c10e0dcu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c10e0de; /* mov.l r1,@(0xc,r15) */
L8c10e0de: *stop=0x8c10e0deu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[9],4);if(io_status)return io_status;goto L8c10e0e0; /* mov.l r9,@(0x14,r14) */
L8c10e0e0: *stop=0x8c10e0e0u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[11],4);if(io_status)return io_status;goto L8c10e0e2; /* mov.l r11,@(0x20,r14) */
L8c10e0e2: *stop=0x8c10e0e2u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e0e4; /* mov.l @(0xc,r15),r3 */
L8c10e0e4: *stop=0x8c10e0e4u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e0e6; /* mov.l @(0x4,r14),r2 */
L8c10e0e6: *stop=0x8c10e0e6u;r[2]-=r[3];goto L8c10e0e8; /* sub r3,r2 */
L8c10e0e8: *stop=0x8c10e0eau;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[2],4);if(io_status)return io_status;goto L8c10e19e; /* bra 0x8c10e19e */
L8c10e0ec: *stop=0x8c10e0ecu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e0ee; /* mov.w 0x8c10e100,r0 */
L8c10e0ee: *stop=0x8c10e0eeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10e0f0; /* mov.l @(r0,r15),r5 */
L8c10e0f0: *stop=0x8c10e0f0u;r[5]+=4u;goto L8c10e0f2; /* add #0x4,r5 */
L8c10e0f2: *stop=0x8c10e0f2u;r[3]=r[5];goto L8c10e0f4; /* mov r5,r3 */
L8c10e0f4: *stop=0x8c10e0f4u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c10e0f6; /* tst r13,r3 */
L8c10e0f6: if((*sr&1u))goto L8c10e110;goto L8c10e0f8; /* bt 0x8c10e110 */
L8c10e0f8: *stop=0x8c10e0f8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e100u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e0fa; /* mov.w 0x8c10e100,r0 */
L8c10e0fa: *stop=0x8c10e0fau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10e0fc; /* mov.l @(r0,r15),r1 */
L8c10e0fc: *stop=0x8c10e0feu;r[1]+=8u;goto L8c10e112; /* bra 0x8c10e112 */
L8c10e110: *stop=0x8c10e110u;r[1]=r[5];goto L8c10e112; /* mov r5,r1 */
L8c10e112: *stop=0x8c10e112u;r[4]=r[1];goto L8c10e114; /* mov r1,r4 */
L8c10e114: *stop=0x8c10e114u;r[4]+=4294967292u;goto L8c10e116; /* add #-0x4,r4 */
L8c10e116: *stop=0x8c10e116u;r[2]=r[4];goto L8c10e118; /* mov r4,r2 */
L8c10e118: *stop=0x8c10e118u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e208u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e11a; /* mov.w 0x8c10e208,r0 */
L8c10e11a: *stop=0x8c10e11au;*sr=(*sr&~1u)|!(r[13]&r[2]);goto L8c10e11c; /* tst r13,r2 */
L8c10e11c: take=(*sr&1u);*stop=0x8c10e11eu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;if(take)goto L8c10e126;goto L8c10e120; /* bt/s 0x8c10e126 */
L8c10e120: *stop=0x8c10e120u;r[3]=r[1];goto L8c10e122; /* mov r1,r3 */
L8c10e122: *stop=0x8c10e124u;r[3]+=4294967288u;goto L8c10e128; /* bra 0x8c10e128 */
L8c10e126: *stop=0x8c10e126u;r[3]=r[4];goto L8c10e128; /* mov r4,r3 */
L8c10e128: *stop=0x8c10e128u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10e12a; /* mov.l @(0x8,r14),r0 */
L8c10e12a: *stop=0x8c10e12au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e12c; /* mov.l @r3,r4 */
L8c10e12c: *stop=0x8c10e12cu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10e12e; /* cmp/eq #-0x1,r0 */
L8c10e12e: take=!(*sr&1u);*stop=0x8c10e130u;r[7]=r[14];if(take)goto L8c10e136;goto L8c10e132; /* bf/s 0x8c10e136 */
L8c10e132: *stop=0x8c10e132u;r[2]=1u;goto L8c10e134; /* mov #0x1,r2 */
L8c10e134: *stop=0x8c10e134u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c10e136; /* mov.l r2,@(0x8,r14) */
L8c10e136: *stop=0x8c10e136u;r[6]=120u;goto L8c10e138; /* mov #0x78,r6 */
L8c10e138: target=0x8c10e304u;*pr=0x8c10e13cu;*stop=0x8c10e13au;r[5]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e13c; /* bsr 0x8c10e304 */
L8c10e13c: *stop=0x8c10e13cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e20cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e13e; /* mov.l 0x8c10e20c,r3 */
L8c10e13e: *stop=0x8c10e13eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c10e140; /* mov.l r9,@(0x4,r15) */
L8c10e140: target=r[3];*pr=0x8c10e144u;*stop=0x8c10e142u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e144; /* jsr @r3 */
L8c10e144: *stop=0x8c10e146u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c10e19e; /* bra 0x8c10e19e */
L8c10e148: *stop=0x8c10e148u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e208u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e14a; /* mov.w 0x8c10e208,r0 */
L8c10e14a: *stop=0x8c10e14au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e14c; /* mov.l @(r0,r15),r4 */
L8c10e14c: *stop=0x8c10e14cu;r[4]+=4u;goto L8c10e14e; /* add #0x4,r4 */
L8c10e14e: *stop=0x8c10e14eu;r[3]=r[4];goto L8c10e150; /* mov r4,r3 */
L8c10e150: *stop=0x8c10e150u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c10e152; /* tst r13,r3 */
L8c10e152: if((*sr&1u))goto L8c10e15c;goto L8c10e154; /* bt 0x8c10e15c */
L8c10e154: *stop=0x8c10e154u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e208u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e156; /* mov.w 0x8c10e208,r0 */
L8c10e156: *stop=0x8c10e156u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e158; /* mov.l @(r0,r15),r2 */
L8c10e158: *stop=0x8c10e15au;r[2]+=8u;goto L8c10e15e; /* bra 0x8c10e15e */
L8c10e15c: *stop=0x8c10e15cu;r[2]=r[4];goto L8c10e15e; /* mov r4,r2 */
L8c10e15e: *stop=0x8c10e15eu;r[5]=r[2];goto L8c10e160; /* mov r2,r5 */
L8c10e160: *stop=0x8c10e160u;r[5]+=4294967292u;goto L8c10e162; /* add #-0x4,r5 */
L8c10e162: *stop=0x8c10e162u;r[1]=r[5];goto L8c10e164; /* mov r5,r1 */
L8c10e164: *stop=0x8c10e164u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e208u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c10e166; /* mov.w 0x8c10e208,r0 */
L8c10e166: *stop=0x8c10e166u;*sr=(*sr&~1u)|!(r[13]&r[1]);goto L8c10e168; /* tst r13,r1 */
L8c10e168: take=(*sr&1u);*stop=0x8c10e16au;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;if(take)goto L8c10e172;goto L8c10e16c; /* bt/s 0x8c10e172 */
L8c10e16c: *stop=0x8c10e16cu;r[3]=r[2];goto L8c10e16e; /* mov r2,r3 */
L8c10e16e: *stop=0x8c10e170u;r[3]+=4294967288u;goto L8c10e174; /* bra 0x8c10e174 */
L8c10e172: *stop=0x8c10e172u;r[3]=r[5];goto L8c10e174; /* mov r5,r3 */
L8c10e174: *stop=0x8c10e174u;if(!valid_access(r[10]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10e176; /* mov.l @(0x8,r10),r1 */
L8c10e176: *stop=0x8c10e176u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e178; /* mov.l @r3,r4 */
L8c10e178: *stop=0x8c10e17au;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c10e19e; /* bra 0x8c10e19e */
L8c10e17c: *stop=0x8c10e17cu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e17e; /* mov.l @(0x10,r15),r2 */
L8c10e17e: *stop=0x8c10e17eu;r[3]=37u;goto L8c10e180; /* mov #0x25,r3 */
L8c10e180: *stop=0x8c10e180u;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[3],1);if(io_status)return io_status;goto L8c10e182; /* mov.b r3,@r2 */
L8c10e182: *stop=0x8c10e182u;r[3]=1u;goto L8c10e184; /* mov #0x1,r3 */
L8c10e184: *stop=0x8c10e184u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c10e186; /* mov.l r9,@(0x4,r15) */
L8c10e186: *stop=0x8c10e186u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c10e188; /* mov.l r3,@(0xc,r15) */
L8c10e188: *stop=0x8c10e188u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[9],4);if(io_status)return io_status;goto L8c10e18a; /* mov.l r9,@(0x14,r14) */
L8c10e18a: *stop=0x8c10e18au;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[11],4);if(io_status)return io_status;goto L8c10e18c; /* mov.l r11,@(0x20,r14) */
L8c10e18c: *stop=0x8c10e18cu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e18e; /* mov.l @(0x4,r14),r3 */
L8c10e18e: *stop=0x8c10e18eu;r[3]+=4294967295u;goto L8c10e190; /* add #-0x1,r3 */
L8c10e190: *stop=0x8c10e192u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[3],4);if(io_status)return io_status;goto L8c10e19e; /* bra 0x8c10e19e */
L8c10e194: *stop=0x8c10e194u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e210u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10e196; /* mov.l 0x8c10e210,r1 */
L8c10e196: *stop=0x8c10e196u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[8],4);if(io_status)return io_status;goto L8c10e198; /* mov.l r8,@r1 */
L8c10e198: *stop=0x8c10e198u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e19a; /* mov.b @r14,r0 */
L8c10e19a: *stop=0x8c10e19au;r[0]|=0x1u;goto L8c10e19c; /* or #0x1,r0 */
L8c10e19c: *stop=0x8c10e19cu;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c10e19e; /* mov.b r0,@r14 */
L8c10e19e: *stop=0x8c10e19eu;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e1a0; /* mov.b @r12,r0 */
L8c10e1a0: *stop=0x8c10e1a0u;*sr=(*sr&~1u)|(r[0]==0x0000006eu);goto L8c10e1a2; /* cmp/eq #0x6e,r0 */
L8c10e1a2: if(!(*sr&1u))goto L8c10e1a8;goto L8c10e1a4; /* bf 0x8c10e1a8 */
L8c10e1a4: *stop=0x8c10e1a6u;goto L8c10e2a6; /* bra 0x8c10e2a6 */
L8c10e1a8: *stop=0x8c10e1a8u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e1aa; /* mov.b @r14,r0 */
L8c10e1aa: *stop=0x8c10e1aau;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c10e1ac; /* tst #0x1,r0 */
L8c10e1ac: if(!(*sr&1u))goto L8c10e2a6;goto L8c10e1ae; /* bf 0x8c10e2a6 */
L8c10e1ae: *stop=0x8c10e1aeu;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e1b0; /* mov.b @r14,r0 */
L8c10e1b0: *stop=0x8c10e1b0u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c10e1b2; /* tst #0x2,r0 */
L8c10e1b2: if(!(*sr&1u))goto L8c10e1d4;goto L8c10e1b4; /* bf 0x8c10e1d4 */
L8c10e1b4: *stop=0x8c10e1b4u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e1b6; /* mov.l @(0x28,r14),r2 */
L8c10e1b6: *stop=0x8c10e1b6u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c10e1b8; /* cmp/pl r2 */
L8c10e1b8: if(!(*sr&1u))goto L8c10e1d4;goto L8c10e1ba; /* bf 0x8c10e1d4 */
L8c10e1ba: *stop=0x8c10e1bcu;goto L8c10e1ce; /* bra 0x8c10e1ce */
L8c10e1be: *stop=0x8c10e1beu;r[4]=r[14];goto L8c10e1c0; /* mov r14,r4 */
L8c10e1c0: *stop=0x8c10e1c0u;r[5]=1u;goto L8c10e1c2; /* mov #0x1,r5 */
L8c10e1c2: *stop=0x8c10e1c2u;r[6]=r[10];goto L8c10e1c4; /* mov r10,r6 */
L8c10e1c4: target=0x8c10e50eu;*pr=0x8c10e1c8u;*stop=0x8c10e1c6u;r[4]+=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e1c8; /* bsr 0x8c10e50e */
L8c10e1c8: *stop=0x8c10e1c8u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e1ca; /* mov.l @(0x28,r14),r2 */
L8c10e1ca: *stop=0x8c10e1cau;r[2]+=4294967295u;goto L8c10e1cc; /* add #-0x1,r2 */
L8c10e1cc: *stop=0x8c10e1ccu;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[2],4);if(io_status)return io_status;goto L8c10e1ce; /* mov.l r2,@(0x28,r14) */
L8c10e1ce: *stop=0x8c10e1ceu;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e1d0; /* mov.l @(0x28,r14),r3 */
L8c10e1d0: *stop=0x8c10e1d0u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c10e1d2; /* cmp/pl r3 */
L8c10e1d2: if((*sr&1u))goto L8c10e1be;goto L8c10e1d4; /* bt 0x8c10e1be */
L8c10e1d4: *stop=0x8c10e1d4u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10e1d6; /* mov.l @(0x1c,r14),r1 */
L8c10e1d6: *stop=0x8c10e1d6u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c10e1d8; /* tst r1,r1 */
L8c10e1d8: if(!(*sr&1u))goto L8c10e1f2;goto L8c10e1da; /* bf 0x8c10e1f2 */
L8c10e1da: *stop=0x8c10e1dau;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e1dc; /* mov.l @(0x20,r14),r2 */
L8c10e1dc: *stop=0x8c10e1dcu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c10e1de; /* tst r2,r2 */
L8c10e1de: if(!(*sr&1u))goto L8c10e1f2;goto L8c10e1e0; /* bf 0x8c10e1f2 */
L8c10e1e0: *stop=0x8c10e1e0u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e1e2; /* mov.l @(0x24,r14),r2 */
L8c10e1e2: *stop=0x8c10e1e2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c10e1e4; /* tst r2,r2 */
L8c10e1e4: if(!(*sr&1u))goto L8c10e1f2;goto L8c10e1e6; /* bf 0x8c10e1f2 */
L8c10e1e6: *stop=0x8c10e1e6u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10e1e8; /* mov.l @(0xc,r15),r5 */
L8c10e1e8: *stop=0x8c10e1e8u;r[6]=r[10];goto L8c10e1ea; /* mov r10,r6 */
L8c10e1ea: target=0x8c10e50eu;*pr=0x8c10e1eeu;*stop=0x8c10e1ecu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e1ee; /* bsr 0x8c10e50e */
L8c10e1ee: *stop=0x8c10e1f0u;goto L8c10e280; /* bra 0x8c10e280 */
L8c10e1f2: *stop=0x8c10e1f2u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10e1f4; /* mov.l @(0x10,r14),r5 */
L8c10e1f4: *stop=0x8c10e1f4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e1f6; /* mov.l @(0x4,r15),r3 */
L8c10e1f6: *stop=0x8c10e1f6u;r[5]-=r[3];goto L8c10e1f8; /* sub r3,r5 */
L8c10e1f8: *stop=0x8c10e1f8u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c10e1fa; /* cmp/pl r5 */
L8c10e1fa: take=!(*sr&1u);*stop=0x8c10e1fcu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;if(take)goto L8c10e224;goto L8c10e1fe; /* bf/s 0x8c10e224 */
L8c10e1fe: *stop=0x8c10e1feu;r[6]=r[10];goto L8c10e200; /* mov r10,r6 */
L8c10e200: target=0x8c10e50eu;*pr=0x8c10e204u;*stop=0x8c10e202u;r[4]=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e204; /* bsr 0x8c10e50e */
L8c10e204: *stop=0x8c10e206u;goto L8c10e224; /* bra 0x8c10e224 */
L8c10e214: *stop=0x8c10e214u;r[4]=r[15];goto L8c10e216; /* mov r15,r4 */
L8c10e216: *stop=0x8c10e216u;r[5]=1u;goto L8c10e218; /* mov #0x1,r5 */
L8c10e218: *stop=0x8c10e218u;r[6]=r[10];goto L8c10e21a; /* mov r10,r6 */
L8c10e21a: target=0x8c10e50eu;*pr=0x8c10e21eu;*stop=0x8c10e21cu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e21e; /* bsr 0x8c10e50e */
L8c10e21e: *stop=0x8c10e21eu;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e220; /* mov.l @(0x1c,r14),r2 */
L8c10e220: *stop=0x8c10e220u;r[2]+=4294967295u;goto L8c10e222; /* add #-0x1,r2 */
L8c10e222: *stop=0x8c10e222u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c10e224; /* mov.l r2,@(0x1c,r14) */
L8c10e224: *stop=0x8c10e224u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e226; /* mov.l @(0x1c,r14),r3 */
L8c10e226: *stop=0x8c10e226u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c10e228; /* cmp/pl r3 */
L8c10e228: if((*sr&1u))goto L8c10e214;goto L8c10e22a; /* bt 0x8c10e214 */
L8c10e22a: *stop=0x8c10e22au;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10e22c; /* mov.l @(0x14,r14),r5 */
L8c10e22c: *stop=0x8c10e22cu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e22e; /* mov.l @(0x10,r14),r3 */
L8c10e22e: *stop=0x8c10e22eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e230; /* mov.l @r15,r2 */
L8c10e230: *stop=0x8c10e230u;r[5]-=r[3];goto L8c10e232; /* sub r3,r5 */
L8c10e232: *stop=0x8c10e232u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c10e234; /* cmp/pl r5 */
L8c10e234: *stop=0x8c10e234u;r[2]+=r[5];goto L8c10e236; /* add r5,r2 */
L8c10e236: take=!(*sr&1u);*stop=0x8c10e238u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c10e254;goto L8c10e23a; /* bf/s 0x8c10e254 */
L8c10e23a: *stop=0x8c10e23au;r[6]=r[10];goto L8c10e23c; /* mov r10,r6 */
L8c10e23c: target=0x8c10e50eu;*pr=0x8c10e240u;*stop=0x8c10e23eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e240; /* bsr 0x8c10e50e */
L8c10e240: *stop=0x8c10e242u;goto L8c10e254; /* bra 0x8c10e254 */
L8c10e244: *stop=0x8c10e244u;r[4]=r[15];goto L8c10e246; /* mov r15,r4 */
L8c10e246: *stop=0x8c10e246u;r[5]=1u;goto L8c10e248; /* mov #0x1,r5 */
L8c10e248: *stop=0x8c10e248u;r[6]=r[10];goto L8c10e24a; /* mov r10,r6 */
L8c10e24a: target=0x8c10e50eu;*pr=0x8c10e24eu;*stop=0x8c10e24cu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e24e; /* bsr 0x8c10e50e */
L8c10e24e: *stop=0x8c10e24eu;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e250; /* mov.l @(0x20,r14),r3 */
L8c10e250: *stop=0x8c10e250u;r[3]+=4294967295u;goto L8c10e252; /* add #-0x1,r3 */
L8c10e252: *stop=0x8c10e252u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[3],4);if(io_status)return io_status;goto L8c10e254; /* mov.l r3,@(0x20,r14) */
L8c10e254: *stop=0x8c10e254u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e256; /* mov.l @(0x20,r14),r2 */
L8c10e256: *stop=0x8c10e256u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c10e258; /* cmp/pl r2 */
L8c10e258: if((*sr&1u))goto L8c10e244;goto L8c10e25a; /* bt 0x8c10e244 */
L8c10e25a: *stop=0x8c10e25au;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10e25c; /* mov.l @(0xc,r15),r5 */
L8c10e25c: *stop=0x8c10e25cu;r[6]=r[10];goto L8c10e25e; /* mov r10,r6 */
L8c10e25e: *stop=0x8c10e25eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e260; /* mov.l @r15,r3 */
L8c10e260: *stop=0x8c10e260u;r[5]-=r[3];goto L8c10e262; /* sub r3,r5 */
L8c10e262: target=0x8c10e50eu;*pr=0x8c10e266u;*stop=0x8c10e264u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e266; /* bsr 0x8c10e50e */
L8c10e266: *stop=0x8c10e268u;goto L8c10e27a; /* bra 0x8c10e27a */
L8c10e26a: *stop=0x8c10e26au;r[4]=r[15];goto L8c10e26c; /* mov r15,r4 */
L8c10e26c: *stop=0x8c10e26cu;r[5]=1u;goto L8c10e26e; /* mov #0x1,r5 */
L8c10e26e: *stop=0x8c10e26eu;r[6]=r[10];goto L8c10e270; /* mov r10,r6 */
L8c10e270: target=0x8c10e50eu;*pr=0x8c10e274u;*stop=0x8c10e272u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e274; /* bsr 0x8c10e50e */
L8c10e274: *stop=0x8c10e274u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e276; /* mov.l @(0x24,r14),r2 */
L8c10e276: *stop=0x8c10e276u;r[2]+=4294967295u;goto L8c10e278; /* add #-0x1,r2 */
L8c10e278: *stop=0x8c10e278u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[2],4);if(io_status)return io_status;goto L8c10e27a; /* mov.l r2,@(0x24,r14) */
L8c10e27a: *stop=0x8c10e27au;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e27c; /* mov.l @(0x24,r14),r3 */
L8c10e27c: *stop=0x8c10e27cu;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c10e27e; /* cmp/pl r3 */
L8c10e27e: if((*sr&1u))goto L8c10e26a;goto L8c10e280; /* bt 0x8c10e26a */
L8c10e280: *stop=0x8c10e280u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e282; /* mov.b @r14,r0 */
L8c10e282: *stop=0x8c10e282u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c10e284; /* tst #0x2,r0 */
L8c10e284: if((*sr&1u))goto L8c10e2a6;goto L8c10e286; /* bt 0x8c10e2a6 */
L8c10e286: *stop=0x8c10e286u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e288; /* mov.l @(0x28,r14),r2 */
L8c10e288: *stop=0x8c10e288u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c10e28a; /* cmp/pl r2 */
L8c10e28a: if(!(*sr&1u))goto L8c10e2a6;goto L8c10e28c; /* bf 0x8c10e2a6 */
L8c10e28c: *stop=0x8c10e28eu;goto L8c10e2a0; /* bra 0x8c10e2a0 */
L8c10e290: *stop=0x8c10e290u;r[4]=r[14];goto L8c10e292; /* mov r14,r4 */
L8c10e292: *stop=0x8c10e292u;r[5]=1u;goto L8c10e294; /* mov #0x1,r5 */
L8c10e294: *stop=0x8c10e294u;r[6]=r[10];goto L8c10e296; /* mov r10,r6 */
L8c10e296: target=0x8c10e50eu;*pr=0x8c10e29au;*stop=0x8c10e298u;r[4]+=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e29a; /* bsr 0x8c10e50e */
L8c10e29a: *stop=0x8c10e29au;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e29c; /* mov.l @(0x28,r14),r2 */
L8c10e29c: *stop=0x8c10e29cu;r[2]+=4294967295u;goto L8c10e29e; /* add #-0x1,r2 */
L8c10e29e: *stop=0x8c10e29eu;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[2],4);if(io_status)return io_status;goto L8c10e2a0; /* mov.l r2,@(0x28,r14) */
L8c10e2a0: *stop=0x8c10e2a0u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e2a2; /* mov.l @(0x28,r14),r3 */
L8c10e2a2: *stop=0x8c10e2a2u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c10e2a4; /* cmp/pl r3 */
L8c10e2a4: if((*sr&1u))goto L8c10e290;goto L8c10e2a6; /* bt 0x8c10e290 */
L8c10e2a6: *stop=0x8c10e2a8u;r[12]+=1u;goto L8c10e2ce; /* bra 0x8c10e2ce */
L8c10e2aa: *stop=0x8c10e2acu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10e2b4; /* bra 0x8c10e2b4 */
L8c10e2ae: *stop=0x8c10e2aeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e2b0; /* mov.l @r15,r2 */
L8c10e2b0: *stop=0x8c10e2b0u;r[2]+=1u;goto L8c10e2b2; /* add #0x1,r2 */
L8c10e2b2: *stop=0x8c10e2b2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10e2b4; /* mov.l r2,@r15 */
L8c10e2b4: *stop=0x8c10e2b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e2b6; /* mov.l @r15,r4 */
L8c10e2b6: *stop=0x8c10e2b6u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]=loaded;}goto L8c10e2b8; /* mov.b @r4,r4 */
L8c10e2b8: *stop=0x8c10e2b8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c10e2ba; /* tst r4,r4 */
L8c10e2ba: if((*sr&1u))goto L8c10e2c2;goto L8c10e2bc; /* bt 0x8c10e2c2 */
L8c10e2bc: *stop=0x8c10e2bcu;r[0]=r[4];goto L8c10e2be; /* mov r4,r0 */
L8c10e2be: *stop=0x8c10e2beu;*sr=(*sr&~1u)|(r[0]==0x00000025u);goto L8c10e2c0; /* cmp/eq #0x25,r0 */
L8c10e2c0: if(!(*sr&1u))goto L8c10e2ae;goto L8c10e2c2; /* bf 0x8c10e2ae */
L8c10e2c2: *stop=0x8c10e2c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10e2c4; /* mov.l @r15,r5 */
L8c10e2c4: *stop=0x8c10e2c4u;r[6]=r[10];goto L8c10e2c6; /* mov r10,r6 */
L8c10e2c6: *stop=0x8c10e2c6u;r[5]-=r[12];goto L8c10e2c8; /* sub r12,r5 */
L8c10e2c8: target=0x8c10e50eu;*pr=0x8c10e2ccu;*stop=0x8c10e2cau;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e2cc; /* bsr 0x8c10e50e */
L8c10e2cc: *stop=0x8c10e2ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}goto L8c10e2ce; /* mov.l @r15,r12 */
L8c10e2ce: *stop=0x8c10e2ceu;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[3]=loaded;}goto L8c10e2d0; /* mov.b @r12,r3 */
L8c10e2d0: *stop=0x8c10e2d0u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c10e2d2; /* tst r3,r3 */
L8c10e2d2: if((*sr&1u))goto L8c10e2e6;goto L8c10e2d4; /* bt 0x8c10e2e6 */
L8c10e2d4: target=0x8c10e576u;*pr=0x8c10e2d8u;*stop=0x8c10e2d6u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e2d8; /* bsr 0x8c10e576 */
L8c10e2d8: *stop=0x8c10e2d8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10e2da; /* tst r0,r0 */
L8c10e2da: if(!(*sr&1u))goto L8c10e2e6;goto L8c10e2dc; /* bf 0x8c10e2e6 */
L8c10e2dc: *stop=0x8c10e2dcu;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e2de; /* mov.b @r14,r0 */
L8c10e2de: *stop=0x8c10e2deu;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c10e2e0; /* tst #0x1,r0 */
L8c10e2e0: if(!(*sr&1u))goto L8c10e2e6;goto L8c10e2e2; /* bf 0x8c10e2e6 */
L8c10e2e2: *stop=0x8c10e2e4u;goto L8c10dc64; /* bra 0x8c10dc64 */
L8c10e2e6: target=0x8c10e566u;*pr=0x8c10e2eau;*stop=0x8c10e2e8u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e2ea; /* bsr 0x8c10e566 */
L8c10e2ea: *stop=0x8c10e2eau;if(!valid_access(r[10]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10e2ec; /* mov.l @(0x8,r10),r0 */
L8c10e2ec: *stop=0x8c10e2ecu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10e302u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10e2ee; /* mov.w 0x8c10e302,r1 */
L8c10e2ee: *stop=0x8c10e2eeu;r[15]+=r[1];goto L8c10e2f0; /* add r1,r15 */
L8c10e2f0: *stop=0x8c10e2f0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10e2f2; /* lds.l @r15+,PR */
L8c10e2f2: *stop=0x8c10e2f2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c10e2f4; /* mov.l @r15+,r8 */
L8c10e2f4: *stop=0x8c10e2f4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c10e2f6; /* mov.l @r15+,r9 */
L8c10e2f6: *stop=0x8c10e2f6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10e2f8; /* mov.l @r15+,r10 */
L8c10e2f8: *stop=0x8c10e2f8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10e2fa; /* mov.l @r15+,r11 */
L8c10e2fa: *stop=0x8c10e2fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10e2fc; /* mov.l @r15+,r12 */
L8c10e2fc: *stop=0x8c10e2fcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10e2fe; /* mov.l @r15+,r13 */
L8c10e2fe: *stop=0x8c10e2feu;if(*pr!=entry_pr)return -3;*stop=0x8c10e300u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10e576(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10e576;
L8c10e576: *stop=0x8c10e576u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10e578; /* mov.l @r4,r0 */
L8c10e578: *stop=0x8c10e578u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10e57a; /* cmp/eq #0x1,r0 */
L8c10e57a: if(!(*sr&1u))goto L8c10e580;goto L8c10e57c; /* bf 0x8c10e580 */
L8c10e57c: *stop=0x8c10e57cu;if(*pr!=entry_pr)return -3;*stop=0x8c10e57eu;r[0]=0u;return 0; /* rts */
L8c10e580: *stop=0x8c10e580u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10e582; /* mov.l @(0x4,r4),r0 */
L8c10e582: *stop=0x8c10e582u;r[1]=16u;goto L8c10e584; /* mov #0x10,r1 */
L8c10e584: *stop=0x8c10e584u;if(!valid_access(r[0]+r[1],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e586; /* mov.b @(r0,r1),r0 */
L8c10e586: *stop=0x8c10e586u;r[0]&=0x40u;goto L8c10e588; /* and #0x40,r0 */
L8c10e588: *stop=0x8c10e588u;if(*pr!=entry_pr)return -3;*stop=0x8c10e58au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10e304(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10e304;
L8c10e304: *stop=0x8c10e304u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10e306; /* mov.l r14,@-r15 */
L8c10e306: *stop=0x8c10e306u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10e308; /* mov.l r13,@-r15 */
L8c10e308: *stop=0x8c10e308u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10e30a; /* mov.l r12,@-r15 */
L8c10e30a: *stop=0x8c10e30au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10e30c; /* mov.l r11,@-r15 */
L8c10e30c: *stop=0x8c10e30cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10e30e; /* mov.l r10,@-r15 */
L8c10e30e: *stop=0x8c10e30eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10e310; /* mov.l r9,@-r15 */
L8c10e310: *stop=0x8c10e310u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10e312; /* mov.l r8,@-r15 */
L8c10e312: *stop=0x8c10e312u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10e314; /* sts.l PR,@-r15 */
L8c10e314: *stop=0x8c10e314u;r[15]+=4294967280u;goto L8c10e316; /* add #-0x10,r15 */
L8c10e316: *stop=0x8c10e316u;r[0]=r[6];goto L8c10e318; /* mov r6,r0 */
L8c10e318: *stop=0x8c10e318u;*sr=(*sr&~1u)|(r[0]==0x00000064u);goto L8c10e31a; /* cmp/eq #0x64,r0 */
L8c10e31a: *stop=0x8c10e31au;r[10]=r[5];goto L8c10e31c; /* mov r5,r10 */
L8c10e31c: *stop=0x8c10e31cu;r[9]=0u;goto L8c10e31e; /* mov #0x0,r9 */
L8c10e31e: take=(*sr&1u);*stop=0x8c10e320u;if(!valid_access(r[10],1u))return -3;wr(m,bus,&io_status,r[10],r[9],1);if(io_status)return io_status;if(take)goto L8c10e33a;goto L8c10e322; /* bt/s 0x8c10e33a */
L8c10e322: *stop=0x8c10e322u;*sr=(*sr&~1u)|(r[0]==0x00000069u);goto L8c10e324; /* cmp/eq #0x69,r0 */
L8c10e324: if((*sr&1u))goto L8c10e33a;goto L8c10e326; /* bt 0x8c10e33a */
L8c10e326: *stop=0x8c10e326u;*sr=(*sr&~1u)|(r[0]==0x00000075u);goto L8c10e328; /* cmp/eq #0x75,r0 */
L8c10e328: if((*sr&1u))goto L8c10e33a;goto L8c10e32a; /* bt 0x8c10e33a */
L8c10e32a: *stop=0x8c10e32au;*sr=(*sr&~1u)|(r[0]==0x0000006fu);goto L8c10e32c; /* cmp/eq #0x6f,r0 */
L8c10e32c: if((*sr&1u))goto L8c10e33e;goto L8c10e32e; /* bt 0x8c10e33e */
L8c10e32e: *stop=0x8c10e32eu;*sr=(*sr&~1u)|(r[0]==0x00000078u);goto L8c10e330; /* cmp/eq #0x78,r0 */
L8c10e330: if((*sr&1u))goto L8c10e342;goto L8c10e332; /* bt 0x8c10e342 */
L8c10e332: *stop=0x8c10e332u;*sr=(*sr&~1u)|(r[0]==0x00000058u);goto L8c10e334; /* cmp/eq #0x58,r0 */
L8c10e334: if((*sr&1u))goto L8c10e342;goto L8c10e336; /* bt 0x8c10e342 */
L8c10e336: *stop=0x8c10e338u;goto L8c10e344; /* bra 0x8c10e344 */
L8c10e33a: *stop=0x8c10e33cu;r[8]=10u;goto L8c10e344; /* bra 0x8c10e344 */
L8c10e33e: *stop=0x8c10e340u;r[8]=8u;goto L8c10e344; /* bra 0x8c10e344 */
L8c10e342: *stop=0x8c10e342u;r[8]=16u;goto L8c10e344; /* mov #0x10,r8 */
L8c10e344: *stop=0x8c10e344u;r[0]=r[6];goto L8c10e346; /* mov r6,r0 */
L8c10e346: *stop=0x8c10e346u;*sr=(*sr&~1u)|(r[0]==0x00000064u);goto L8c10e348; /* cmp/eq #0x64,r0 */
L8c10e348: if((*sr&1u))goto L8c10e350;goto L8c10e34a; /* bt 0x8c10e350 */
L8c10e34a: *stop=0x8c10e34au;r[0]=r[6];goto L8c10e34c; /* mov r6,r0 */
L8c10e34c: *stop=0x8c10e34cu;*sr=(*sr&~1u)|(r[0]==0x00000069u);goto L8c10e34e; /* cmp/eq #0x69,r0 */
L8c10e34e: if(!(*sr&1u))goto L8c10e358;goto L8c10e350; /* bf 0x8c10e358 */
L8c10e350: *stop=0x8c10e350u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c10e352; /* cmp/pz r4 */
L8c10e352: if((*sr&1u))goto L8c10e358;goto L8c10e354; /* bt 0x8c10e358 */
L8c10e354: *stop=0x8c10e356u;r[12]=0u-r[4];goto L8c10e35a; /* bra 0x8c10e35a */
L8c10e358: *stop=0x8c10e358u;r[12]=r[4];goto L8c10e35a; /* mov r4,r12 */
L8c10e35a: *stop=0x8c10e35au;r[14]=r[15];goto L8c10e35c; /* mov r15,r14 */
L8c10e35c: *stop=0x8c10e35cu;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c10e35e; /* tst r12,r12 */
L8c10e35e: *stop=0x8c10e35eu;r[13]=r[9];goto L8c10e360; /* mov r9,r13 */
L8c10e360: *stop=0x8c10e360u;r[11]=r[9];goto L8c10e362; /* mov r9,r11 */
L8c10e362: take=!(*sr&1u);*stop=0x8c10e364u;r[14]+=4u;if(take)goto L8c10e374;goto L8c10e366; /* bf/s 0x8c10e374 */
L8c10e366: *stop=0x8c10e366u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e368; /* mov.l @(0x8,r7),r3 */
L8c10e368: *stop=0x8c10e368u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c10e36a; /* tst r3,r3 */
L8c10e36a: if((*sr&1u))goto L8c10e374;goto L8c10e36c; /* bt 0x8c10e374 */
L8c10e36c: *stop=0x8c10e36cu;r[3]=48u;goto L8c10e36e; /* mov #0x30,r3 */
L8c10e36e: *stop=0x8c10e36eu;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[3],1);if(io_status)return io_status;goto L8c10e370; /* mov.b r3,@r14 */
L8c10e370: *stop=0x8c10e372u;r[13]=1u;goto L8c10e3ce; /* bra 0x8c10e3ce */
L8c10e374: *stop=0x8c10e374u;r[3]=0u;goto L8c10e376; /* mov #0x0,r3 */
L8c10e376: *stop=0x8c10e376u;*sr=(*sr&~1u)|(r[12]>r[3]);goto L8c10e378; /* cmp/hi r3,r12 */
L8c10e378: if(!(*sr&1u))goto L8c10e3ce;goto L8c10e37a; /* bf 0x8c10e3ce */
L8c10e37a: *stop=0x8c10e37au;r[1]=r[13];goto L8c10e37c; /* mov r13,r1 */
L8c10e37c: *stop=0x8c10e37cu;r[1]+=r[14];goto L8c10e37e; /* add r14,r1 */
L8c10e37e: *stop=0x8c10e37eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e498u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e380; /* mov.l 0x8c10e498,r3 */
L8c10e380: *stop=0x8c10e380u;r[2]=r[1];goto L8c10e382; /* mov r1,r2 */
L8c10e382: *stop=0x8c10e382u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c10e384; /* mov.l r1,@r15 */
L8c10e384: *stop=0x8c10e384u;r[1]=r[12];goto L8c10e386; /* mov r12,r1 */
L8c10e386: target=r[3];*pr=0x8c10e38au;*stop=0x8c10e388u;r[0]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e38a; /* jsr @r3 */
L8c10e38a: *stop=0x8c10e38au;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[0],1);if(io_status)return io_status;goto L8c10e38c; /* mov.b r0,@r2 */
L8c10e38c: *stop=0x8c10e38cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e38e; /* mov.l @r15,r3 */
L8c10e38e: *stop=0x8c10e38eu;if(!valid_access(r[3],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10e390; /* mov.b @r3,r2 */
L8c10e390: *stop=0x8c10e390u;r[3]=9u;goto L8c10e392; /* mov #0x9,r3 */
L8c10e392: *stop=0x8c10e392u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]);goto L8c10e394; /* cmp/gt r3,r2 */
L8c10e394: if((*sr&1u))goto L8c10e3a2;goto L8c10e396; /* bt 0x8c10e3a2 */
L8c10e396: *stop=0x8c10e396u;r[0]=r[13];goto L8c10e398; /* mov r13,r0 */
L8c10e398: *stop=0x8c10e398u;if(!valid_access(r[0]+r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+r[0],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10e39a; /* mov.b @(r0,r14),r2 */
L8c10e39a: *stop=0x8c10e39au;r[2]+=48u;goto L8c10e39c; /* add #0x30,r2 */
L8c10e39c: *stop=0x8c10e39cu;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],1);if(io_status)return io_status;goto L8c10e39e; /* mov.b r2,@(r0,r14) */
L8c10e39e: *stop=0x8c10e3a0u;goto L8c10e3bc; /* bra 0x8c10e3bc */
L8c10e3a2: *stop=0x8c10e3a2u;r[1]=r[13];goto L8c10e3a4; /* mov r13,r1 */
L8c10e3a4: *stop=0x8c10e3a4u;r[0]=r[6];goto L8c10e3a6; /* mov r6,r0 */
L8c10e3a6: *stop=0x8c10e3a6u;r[1]+=r[14];goto L8c10e3a8; /* add r14,r1 */
L8c10e3a8: *stop=0x8c10e3a8u;*sr=(*sr&~1u)|(r[0]==0x00000078u);goto L8c10e3aa; /* cmp/eq #0x78,r0 */
L8c10e3aa: *stop=0x8c10e3aau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c10e3ac; /* mov.l r1,@r15 */
L8c10e3ac: if(!(*sr&1u))goto L8c10e3b6;goto L8c10e3ae; /* bf 0x8c10e3b6 */
L8c10e3ae: *stop=0x8c10e3aeu;if(!valid_access(r[1],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e3b0; /* mov.b @r1,r0 */
L8c10e3b0: *stop=0x8c10e3b0u;r[0]+=87u;goto L8c10e3b2; /* add #0x57,r0 */
L8c10e3b2: *stop=0x8c10e3b4u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[0],1);if(io_status)return io_status;goto L8c10e3bc; /* bra 0x8c10e3bc */
L8c10e3b6: *stop=0x8c10e3b6u;if(!valid_access(r[1],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10e3b8; /* mov.b @r1,r2 */
L8c10e3b8: *stop=0x8c10e3b8u;r[2]+=55u;goto L8c10e3ba; /* add #0x37,r2 */
L8c10e3ba: *stop=0x8c10e3bau;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[2],1);if(io_status)return io_status;goto L8c10e3bc; /* mov.b r2,@r1 */
L8c10e3bc: *stop=0x8c10e3bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e49cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e3be; /* mov.l 0x8c10e49c,r3 */
L8c10e3be: *stop=0x8c10e3beu;r[1]=r[12];goto L8c10e3c0; /* mov r12,r1 */
L8c10e3c0: target=r[3];*pr=0x8c10e3c4u;*stop=0x8c10e3c2u;r[0]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e3c4; /* jsr @r3 */
L8c10e3c4: *stop=0x8c10e3c4u;r[2]=0u;goto L8c10e3c6; /* mov #0x0,r2 */
L8c10e3c6: *stop=0x8c10e3c6u;r[12]=r[0];goto L8c10e3c8; /* mov r0,r12 */
L8c10e3c8: *stop=0x8c10e3c8u;*sr=(*sr&~1u)|(r[12]>r[2]);goto L8c10e3ca; /* cmp/hi r2,r12 */
L8c10e3ca: take=(*sr&1u);*stop=0x8c10e3ccu;r[13]+=1u;if(take)goto L8c10e37a;goto L8c10e3ce; /* bt/s 0x8c10e37a */
L8c10e3ce: *stop=0x8c10e3ceu;if(!valid_access(r[7],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e3d0; /* mov.b @r7,r0 */
L8c10e3d0: *stop=0x8c10e3d0u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c10e3d2; /* tst #0x4,r0 */
L8c10e3d2: if((*sr&1u))goto L8c10e404;goto L8c10e3d4; /* bt 0x8c10e404 */
L8c10e3d4: *stop=0x8c10e3d4u;r[0]=r[6];goto L8c10e3d6; /* mov r6,r0 */
L8c10e3d6: *stop=0x8c10e3d6u;*sr=(*sr&~1u)|(r[0]==0x0000006fu);goto L8c10e3d8; /* cmp/eq #0x6f,r0 */
L8c10e3d8: if((*sr&1u))goto L8c10e3e6;goto L8c10e3da; /* bt 0x8c10e3e6 */
L8c10e3da: *stop=0x8c10e3dau;*sr=(*sr&~1u)|(r[0]==0x00000078u);goto L8c10e3dc; /* cmp/eq #0x78,r0 */
L8c10e3dc: if((*sr&1u))goto L8c10e3f4;goto L8c10e3de; /* bt 0x8c10e3f4 */
L8c10e3de: *stop=0x8c10e3deu;*sr=(*sr&~1u)|(r[0]==0x00000058u);goto L8c10e3e0; /* cmp/eq #0x58,r0 */
L8c10e3e0: if((*sr&1u))goto L8c10e3f4;goto L8c10e3e2; /* bt 0x8c10e3f4 */
L8c10e3e2: *stop=0x8c10e3e4u;goto L8c10e404; /* bra 0x8c10e404 */
L8c10e3e6: *stop=0x8c10e3e6u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c10e3e8; /* tst r4,r4 */
L8c10e3e8: if((*sr&1u))goto L8c10e404;goto L8c10e3ea; /* bt 0x8c10e404 */
L8c10e3ea: *stop=0x8c10e3eau;r[0]=r[13];goto L8c10e3ec; /* mov r13,r0 */
L8c10e3ec: *stop=0x8c10e3ecu;r[3]=48u;goto L8c10e3ee; /* mov #0x30,r3 */
L8c10e3ee: *stop=0x8c10e3eeu;r[13]+=1u;goto L8c10e3f0; /* add #0x1,r13 */
L8c10e3f0: *stop=0x8c10e3f2u;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],1);if(io_status)return io_status;goto L8c10e404; /* bra 0x8c10e404 */
L8c10e3f4: *stop=0x8c10e3f4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c10e3f6; /* tst r4,r4 */
L8c10e3f6: if((*sr&1u))goto L8c10e404;goto L8c10e3f8; /* bt 0x8c10e404 */
L8c10e3f8: *stop=0x8c10e3f8u;r[3]=48u;goto L8c10e3fa; /* mov #0x30,r3 */
L8c10e3fa: *stop=0x8c10e3fau;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c10e3fc; /* mov.b r3,@r5 */
L8c10e3fc: *stop=0x8c10e3fcu;r[5]+=1u;goto L8c10e3fe; /* add #0x1,r5 */
L8c10e3fe: *stop=0x8c10e3feu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[6],1);if(io_status)return io_status;goto L8c10e400; /* mov.b r6,@r5 */
L8c10e400: *stop=0x8c10e400u;r[11]=2u;goto L8c10e402; /* mov #0x2,r11 */
L8c10e402: *stop=0x8c10e402u;r[5]+=1u;goto L8c10e404; /* add #0x1,r5 */
L8c10e404: *stop=0x8c10e404u;r[0]=r[6];goto L8c10e406; /* mov r6,r0 */
L8c10e406: *stop=0x8c10e406u;*sr=(*sr&~1u)|(r[0]==0x00000064u);goto L8c10e408; /* cmp/eq #0x64,r0 */
L8c10e408: *stop=0x8c10e408u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10e40a; /* mov.l r13,@r15 */
L8c10e40a: take=(*sr&1u);*stop=0x8c10e40cu;r[11]+=r[13];if(take)goto L8c10e414;goto L8c10e40e; /* bt/s 0x8c10e414 */
L8c10e40e: *stop=0x8c10e40eu;r[0]=r[6];goto L8c10e410; /* mov r6,r0 */
L8c10e410: *stop=0x8c10e410u;*sr=(*sr&~1u)|(r[0]==0x00000069u);goto L8c10e412; /* cmp/eq #0x69,r0 */
L8c10e412: if(!(*sr&1u))goto L8c10e446;goto L8c10e414; /* bf 0x8c10e446 */
L8c10e414: *stop=0x8c10e414u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e416; /* mov.l @(0x8,r7),r3 */
L8c10e416: *stop=0x8c10e416u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c10e418; /* tst r3,r3 */
L8c10e418: if(!(*sr&1u))goto L8c10e41e;goto L8c10e41a; /* bf 0x8c10e41e */
L8c10e41a: *stop=0x8c10e41au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c10e41c; /* tst r4,r4 */
L8c10e41c: if((*sr&1u))goto L8c10e446;goto L8c10e41e; /* bt 0x8c10e446 */
L8c10e41e: *stop=0x8c10e41eu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c10e420; /* cmp/pz r4 */
L8c10e420: if(!(*sr&1u))goto L8c10e43e;goto L8c10e422; /* bf 0x8c10e43e */
L8c10e422: *stop=0x8c10e422u;if(!valid_access(r[7],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e424; /* mov.b @r7,r0 */
L8c10e424: *stop=0x8c10e424u;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c10e426; /* tst #0x8,r0 */
L8c10e426: if((*sr&1u))goto L8c10e430;goto L8c10e428; /* bt 0x8c10e430 */
L8c10e428: *stop=0x8c10e428u;r[3]=43u;goto L8c10e42a; /* mov #0x2b,r3 */
L8c10e42a: *stop=0x8c10e42au;r[11]+=1u;goto L8c10e42c; /* add #0x1,r11 */
L8c10e42c: *stop=0x8c10e42eu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c10e444; /* bra 0x8c10e444 */
L8c10e430: *stop=0x8c10e430u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c10e432; /* tst #0x10,r0 */
L8c10e432: if((*sr&1u))goto L8c10e446;goto L8c10e434; /* bt 0x8c10e446 */
L8c10e434: *stop=0x8c10e434u;r[3]=32u;goto L8c10e436; /* mov #0x20,r3 */
L8c10e436: *stop=0x8c10e436u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c10e438; /* mov.b r3,@r5 */
L8c10e438: *stop=0x8c10e438u;r[11]+=1u;goto L8c10e43a; /* add #0x1,r11 */
L8c10e43a: *stop=0x8c10e43cu;r[5]+=1u;goto L8c10e446; /* bra 0x8c10e446 */
L8c10e43e: *stop=0x8c10e43eu;r[2]=45u;goto L8c10e440; /* mov #0x2d,r2 */
L8c10e440: *stop=0x8c10e440u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[2],1);if(io_status)return io_status;goto L8c10e442; /* mov.b r2,@r5 */
L8c10e442: *stop=0x8c10e442u;r[11]+=1u;goto L8c10e444; /* add #0x1,r11 */
L8c10e444: *stop=0x8c10e444u;r[5]+=1u;goto L8c10e446; /* add #0x1,r5 */
L8c10e446: *stop=0x8c10e446u;if(!valid_access(r[10],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[10],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e448; /* mov.b @r10,r0 */
L8c10e448: *stop=0x8c10e448u;r[4]=r[10];goto L8c10e44a; /* mov r10,r4 */
L8c10e44a: *stop=0x8c10e44au;r[4]+=1u;goto L8c10e44c; /* add #0x1,r4 */
L8c10e44c: *stop=0x8c10e44cu;*sr=(*sr&~1u)|(r[0]==0x0000002bu);goto L8c10e44e; /* cmp/eq #0x2b,r0 */
L8c10e44e: take=(*sr&1u);*stop=0x8c10e450u;r[13]=r[0];if(take)goto L8c10e45e;goto L8c10e452; /* bt/s 0x8c10e45e */
L8c10e452: *stop=0x8c10e452u;r[0]=r[13];goto L8c10e454; /* mov r13,r0 */
L8c10e454: *stop=0x8c10e454u;*sr=(*sr&~1u)|(r[0]==0x0000002du);goto L8c10e456; /* cmp/eq #0x2d,r0 */
L8c10e456: if((*sr&1u))goto L8c10e45e;goto L8c10e458; /* bt 0x8c10e45e */
L8c10e458: *stop=0x8c10e458u;r[0]=r[13];goto L8c10e45a; /* mov r13,r0 */
L8c10e45a: *stop=0x8c10e45au;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c10e45c; /* cmp/eq #0x20,r0 */
L8c10e45c: if(!(*sr&1u))goto L8c10e462;goto L8c10e45e; /* bf 0x8c10e462 */
L8c10e45e: *stop=0x8c10e460u;if(!valid_access(r[7]+20u,4u))return -3;wr(m,bus,&io_status,r[7]+0x14u,r[4],4);if(io_status)return io_status;goto L8c10e488; /* bra 0x8c10e488 */
L8c10e462: *stop=0x8c10e462u;if(!valid_access(r[7],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e464; /* mov.b @r7,r0 */
L8c10e464: *stop=0x8c10e464u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c10e466; /* tst #0x4,r0 */
L8c10e466: if((*sr&1u))goto L8c10e486;goto L8c10e468; /* bt 0x8c10e486 */
L8c10e468: *stop=0x8c10e468u;r[0]=r[6];goto L8c10e46a; /* mov r6,r0 */
L8c10e46a: *stop=0x8c10e46au;*sr=(*sr&~1u)|(r[0]==0x00000058u);goto L8c10e46c; /* cmp/eq #0x58,r0 */
L8c10e46c: if((*sr&1u))goto L8c10e47a;goto L8c10e46e; /* bt 0x8c10e47a */
L8c10e46e: *stop=0x8c10e46eu;*sr=(*sr&~1u)|(r[0]==0x00000078u);goto L8c10e470; /* cmp/eq #0x78,r0 */
L8c10e470: if((*sr&1u))goto L8c10e47a;goto L8c10e472; /* bt 0x8c10e47a */
L8c10e472: *stop=0x8c10e472u;*sr=(*sr&~1u)|(r[0]==0x0000006fu);goto L8c10e474; /* cmp/eq #0x6f,r0 */
L8c10e474: if((*sr&1u))goto L8c10e482;goto L8c10e476; /* bt 0x8c10e482 */
L8c10e476: *stop=0x8c10e478u;goto L8c10e488; /* bra 0x8c10e488 */
L8c10e47a: *stop=0x8c10e47au;r[2]=r[10];goto L8c10e47c; /* mov r10,r2 */
L8c10e47c: *stop=0x8c10e47cu;r[2]+=2u;goto L8c10e47e; /* add #0x2,r2 */
L8c10e47e: *stop=0x8c10e480u;if(!valid_access(r[7]+20u,4u))return -3;wr(m,bus,&io_status,r[7]+0x14u,r[2],4);if(io_status)return io_status;goto L8c10e488; /* bra 0x8c10e488 */
L8c10e482: *stop=0x8c10e484u;if(!valid_access(r[7]+20u,4u))return -3;wr(m,bus,&io_status,r[7]+0x14u,r[4],4);if(io_status)return io_status;goto L8c10e488; /* bra 0x8c10e488 */
L8c10e486: *stop=0x8c10e486u;if(!valid_access(r[7]+20u,4u))return -3;wr(m,bus,&io_status,r[7]+0x14u,r[10],4);if(io_status)return io_status;goto L8c10e488; /* mov.l r10,@(0x14,r7) */
L8c10e488: *stop=0x8c10e488u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e48a; /* mov.l @(0x8,r7),r3 */
L8c10e48a: *stop=0x8c10e48au;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[11]);goto L8c10e48c; /* cmp/gt r11,r3 */
L8c10e48c: if(!(*sr&1u))goto L8c10e4a0;goto L8c10e48e; /* bf 0x8c10e4a0 */
L8c10e48e: *stop=0x8c10e48eu;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c10e490; /* mov.l @(0x8,r7),r6 */
L8c10e490: *stop=0x8c10e490u;r[4]=r[6];goto L8c10e492; /* mov r6,r4 */
L8c10e492: *stop=0x8c10e492u;r[6]-=r[11];goto L8c10e494; /* sub r11,r6 */
L8c10e494: *stop=0x8c10e496u;if(!valid_access(r[7]+32u,4u))return -3;wr(m,bus,&io_status,r[7]+0x20u,r[6],4);if(io_status)return io_status;goto L8c10e4a4; /* bra 0x8c10e4a4 */
L8c10e4a0: *stop=0x8c10e4a0u;if(!valid_access(r[7]+32u,4u))return -3;wr(m,bus,&io_status,r[7]+0x20u,r[9],4);if(io_status)return io_status;goto L8c10e4a2; /* mov.l r9,@(0x20,r7) */
L8c10e4a2: *stop=0x8c10e4a2u;r[4]=r[11];goto L8c10e4a4; /* mov r11,r4 */
L8c10e4a4: *stop=0x8c10e4a4u;if(!valid_access(r[10],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[10],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e4a6; /* mov.b @r10,r0 */
L8c10e4a6: *stop=0x8c10e4a6u;*sr=(*sr&~1u)|(r[0]==0x0000002bu);goto L8c10e4a8; /* cmp/eq #0x2b,r0 */
L8c10e4a8: take=(*sr&1u);*stop=0x8c10e4aau;r[6]=r[0];if(take)goto L8c10e4b8;goto L8c10e4ac; /* bt/s 0x8c10e4b8 */
L8c10e4ac: *stop=0x8c10e4acu;r[0]=r[6];goto L8c10e4ae; /* mov r6,r0 */
L8c10e4ae: *stop=0x8c10e4aeu;*sr=(*sr&~1u)|(r[0]==0x0000002du);goto L8c10e4b0; /* cmp/eq #0x2d,r0 */
L8c10e4b0: if((*sr&1u))goto L8c10e4b8;goto L8c10e4b2; /* bt 0x8c10e4b8 */
L8c10e4b2: *stop=0x8c10e4b2u;r[0]=r[6];goto L8c10e4b4; /* mov r6,r0 */
L8c10e4b4: *stop=0x8c10e4b4u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c10e4b6; /* cmp/eq #0x20,r0 */
L8c10e4b6: if(!(*sr&1u))goto L8c10e4d2;goto L8c10e4b8; /* bf 0x8c10e4d2 */
L8c10e4b8: *stop=0x8c10e4b8u;if(!valid_access(r[7]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e4ba; /* mov.l @(0x4,r7),r3 */
L8c10e4ba: *stop=0x8c10e4bau;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[4]);goto L8c10e4bc; /* cmp/gt r4,r3 */
L8c10e4bc: if(!(*sr&1u))goto L8c10e4e0;goto L8c10e4be; /* bf 0x8c10e4e0 */
L8c10e4be: *stop=0x8c10e4beu;if(!valid_access(r[7]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c10e4c0; /* mov.b @(0x1,r7),r0 */
L8c10e4c0: *stop=0x8c10e4c0u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10e4c2; /* cmp/eq #0x30,r0 */
L8c10e4c2: if(!(*sr&1u))goto L8c10e4d2;goto L8c10e4c4; /* bf 0x8c10e4d2 */
L8c10e4c4: *stop=0x8c10e4c4u;if(!valid_access(r[7]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e4c6; /* mov.l @(0x4,r7),r2 */
L8c10e4c6: *stop=0x8c10e4c6u;if(!valid_access(r[7]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e4c8; /* mov.l @(0x20,r7),r3 */
L8c10e4c8: *stop=0x8c10e4c8u;r[2]-=r[4];goto L8c10e4ca; /* sub r4,r2 */
L8c10e4ca: *stop=0x8c10e4cau;r[3]+=r[2];goto L8c10e4cc; /* add r2,r3 */
L8c10e4cc: *stop=0x8c10e4ccu;if(!valid_access(r[7]+32u,4u))return -3;wr(m,bus,&io_status,r[7]+0x20u,r[3],4);if(io_status)return io_status;goto L8c10e4ce; /* mov.l r3,@(0x20,r7) */
L8c10e4ce: *stop=0x8c10e4d0u;if(!valid_access(r[7]+40u,4u))return -3;wr(m,bus,&io_status,r[7]+0x28u,r[9],4);if(io_status)return io_status;goto L8c10e4e2; /* bra 0x8c10e4e2 */
L8c10e4d2: *stop=0x8c10e4d2u;if(!valid_access(r[7]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e4d4; /* mov.l @(0x4,r7),r3 */
L8c10e4d4: *stop=0x8c10e4d4u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[4]);goto L8c10e4d6; /* cmp/gt r4,r3 */
L8c10e4d6: if(!(*sr&1u))goto L8c10e4e0;goto L8c10e4d8; /* bf 0x8c10e4e0 */
L8c10e4d8: *stop=0x8c10e4d8u;if(!valid_access(r[7]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10e4da; /* mov.l @(0x4,r7),r1 */
L8c10e4da: *stop=0x8c10e4dau;r[1]-=r[4];goto L8c10e4dc; /* sub r4,r1 */
L8c10e4dc: *stop=0x8c10e4deu;if(!valid_access(r[7]+40u,4u))return -3;wr(m,bus,&io_status,r[7]+0x28u,r[1],4);if(io_status)return io_status;goto L8c10e4e2; /* bra 0x8c10e4e2 */
L8c10e4e0: *stop=0x8c10e4e0u;if(!valid_access(r[7]+40u,4u))return -3;wr(m,bus,&io_status,r[7]+0x28u,r[9],4);if(io_status)return io_status;goto L8c10e4e2; /* mov.l r9,@(0x28,r7) */
L8c10e4e2: *stop=0x8c10e4e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10e4e4; /* mov.l @r15,r4 */
L8c10e4e4: *stop=0x8c10e4e4u;r[4]+=4294967295u;goto L8c10e4e6; /* add #-0x1,r4 */
L8c10e4e6: *stop=0x8c10e4e6u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c10e4e8; /* cmp/pz r4 */
L8c10e4e8: if(!(*sr&1u))goto L8c10e4f8;goto L8c10e4ea; /* bf 0x8c10e4f8 */
L8c10e4ea: *stop=0x8c10e4eau;r[0]=r[4];goto L8c10e4ec; /* mov r4,r0 */
L8c10e4ec: *stop=0x8c10e4ecu;if(!valid_access(r[0]+r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+r[0],1);if(io_status)return io_status;r[3]=loaded;}goto L8c10e4ee; /* mov.b @(r0,r14),r3 */
L8c10e4ee: *stop=0x8c10e4eeu;r[4]+=4294967295u;goto L8c10e4f0; /* add #-0x1,r4 */
L8c10e4f0: *stop=0x8c10e4f0u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c10e4f2; /* cmp/pz r4 */
L8c10e4f2: *stop=0x8c10e4f2u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c10e4f4; /* mov.b r3,@r5 */
L8c10e4f4: take=(*sr&1u);*stop=0x8c10e4f6u;r[5]+=1u;if(take)goto L8c10e4ea;goto L8c10e4f8; /* bt/s 0x8c10e4ea */
L8c10e4f8: *stop=0x8c10e4f8u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[9],1);if(io_status)return io_status;goto L8c10e4fa; /* mov.b r9,@r5 */
L8c10e4fa: *stop=0x8c10e4fau;r[15]+=16u;goto L8c10e4fc; /* add #0x10,r15 */
L8c10e4fc: *stop=0x8c10e4fcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10e4fe; /* lds.l @r15+,PR */
L8c10e4fe: *stop=0x8c10e4feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c10e500; /* mov.l @r15+,r8 */
L8c10e500: *stop=0x8c10e500u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c10e502; /* mov.l @r15+,r9 */
L8c10e502: *stop=0x8c10e502u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10e504; /* mov.l @r15+,r10 */
L8c10e504: *stop=0x8c10e504u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10e506; /* mov.l @r15+,r11 */
L8c10e506: *stop=0x8c10e506u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10e508; /* mov.l @r15+,r12 */
L8c10e508: *stop=0x8c10e508u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10e50a; /* mov.l @r15+,r13 */
L8c10e50a: *stop=0x8c10e50au;if(*pr!=entry_pr)return -3;*stop=0x8c10e50cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10d3f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d3f0;
L8c10d3f0: *stop=0x8c10d3f2u;r[5]=0u;goto L8c10d3f6; /* bra 0x8c10d3f6 */
L8c10d3f4: *stop=0x8c10d3f4u;r[5]+=1u;goto L8c10d3f6; /* add #0x1,r5 */
L8c10d3f6: *stop=0x8c10d3f6u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}r[4]+=1;goto L8c10d3f8; /* mov.b @r4+,r3 */
L8c10d3f8: *stop=0x8c10d3f8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c10d3fa; /* tst r3,r3 */
L8c10d3fa: if(!(*sr&1u))goto L8c10d3f4;goto L8c10d3fc; /* bf 0x8c10d3f4 */
L8c10d3fc: *stop=0x8c10d3fcu;if(*pr!=entry_pr)return -3;*stop=0x8c10d3feu;r[0]=r[5];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10f1a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10f1a4;
L8c10f1a4: *stop=0x8c10f1a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10f1a6; /* mov.l r14,@-r15 */
L8c10f1a6: *stop=0x8c10f1a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10f1a8; /* mov.l r13,@-r15 */
L8c10f1a8: *stop=0x8c10f1a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10f1aa; /* mov.l r12,@-r15 */
L8c10f1aa: *stop=0x8c10f1aau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10f1ac; /* mov.l r11,@-r15 */
L8c10f1ac: *stop=0x8c10f1acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10f1ae; /* mov.l r10,@-r15 */
L8c10f1ae: *stop=0x8c10f1aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10f1b0; /* mov.l r9,@-r15 */
L8c10f1b0: *stop=0x8c10f1b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10f1b2; /* mov.l r8,@-r15 */
L8c10f1b2: *stop=0x8c10f1b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10f1b4; /* sts.l PR,@-r15 */
L8c10f1b4: *stop=0x8c10f1b4u;r[15]+=4294967232u;goto L8c10f1b6; /* add #-0x40,r15 */
L8c10f1b6: *stop=0x8c10f1b6u;r[14]=r[7];goto L8c10f1b8; /* mov r7,r14 */
L8c10f1b8: *stop=0x8c10f1b8u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c10f1ba; /* mov.l r6,@(0x8,r15) */
L8c10f1ba: *stop=0x8c10f1bau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[7]=loaded;}goto L8c10f1bc; /* mov.l @r4,r7 */
L8c10f1bc: *stop=0x8c10f1bcu;r[6]=3u;goto L8c10f1be; /* mov #0x3,r6 */
L8c10f1be: *stop=0x8c10f1beu;r[13]=r[5];goto L8c10f1c0; /* mov r5,r13 */
L8c10f1c0: *stop=0x8c10f1c0u;r[2]=r[7];goto L8c10f1c2; /* mov r7,r2 */
L8c10f1c2: *stop=0x8c10f1c2u;r[2]+=4u;goto L8c10f1c4; /* add #0x4,r2 */
L8c10f1c4: *stop=0x8c10f1c4u;*sr=(*sr&~1u)|!(r[6]&r[2]);goto L8c10f1c6; /* tst r6,r2 */
L8c10f1c6: take=(*sr&1u);*stop=0x8c10f1c8u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;if(take)goto L8c10f1d0;goto L8c10f1ca; /* bt/s 0x8c10f1d0 */
L8c10f1ca: *stop=0x8c10f1cau;r[3]=r[7];goto L8c10f1cc; /* mov r7,r3 */
L8c10f1cc: *stop=0x8c10f1ceu;r[3]+=8u;goto L8c10f1d2; /* bra 0x8c10f1d2 */
L8c10f1d0: *stop=0x8c10f1d0u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f1d2; /* mov.l @(0xc,r15),r3 */
L8c10f1d2: *stop=0x8c10f1d2u;r[5]=r[3];goto L8c10f1d4; /* mov r3,r5 */
L8c10f1d4: *stop=0x8c10f1d4u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c10f1d6; /* mov.l r3,@r4 */
L8c10f1d6: *stop=0x8c10f1d6u;r[3]+=4294967292u;goto L8c10f1d8; /* add #-0x4,r3 */
L8c10f1d8: *stop=0x8c10f1d8u;*sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c10f1da; /* tst r6,r3 */
L8c10f1da: take=(*sr&1u);*stop=0x8c10f1dcu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;if(take)goto L8c10f1e4;goto L8c10f1de; /* bt/s 0x8c10f1e4 */
L8c10f1de: *stop=0x8c10f1deu;r[1]=r[5];goto L8c10f1e0; /* mov r5,r1 */
L8c10f1e0: *stop=0x8c10f1e2u;r[1]+=4294967288u;goto L8c10f1e6; /* bra 0x8c10f1e6 */
L8c10f1e4: *stop=0x8c10f1e4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f1e6; /* mov.l @(0x4,r15),r1 */
L8c10f1e6: *stop=0x8c10f1e6u;r[3]=f->fpscr;goto L8c10f1e8; /* sts FPSCR,r3 */
L8c10f1e8: *stop=0x8c10f1e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f384u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f1ea; /* mov.l 0x8c10f384,r2 */
L8c10f1ea: *stop=0x8c10f1eau;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c10f1ec; /* fmov.s @r1,fr3 */
L8c10f1ec: *stop=0x8c10f1ecu;r[3]|=r[2];goto L8c10f1ee; /* or r2,r3 */
L8c10f1ee: *stop=0x8c10f1eeu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f1f0; /* mov.l @(0x8,r14),r0 */
L8c10f1f0: *stop=0x8c10f1f0u;*fpul=f->bank[(f->fpscr>>21)&1u][3];goto L8c10f1f2; /* flds fr3,FPUL */
L8c10f1f2: *stop=0x8c10f1f2u;f->fpscr=r[3];goto L8c10f1f4; /* lds r3,fpscr */
L8c10f1f4: *stop=0x8c10f1f4u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10f1f6; /* cmp/eq #-0x1,r0 */
L8c10f1f6: *stop=0x8c10f1f6u;status=sa_sh4_fcnvsd(f,*fpul,2u);if(status)return status;goto L8c10f1f8; /* fcnvsd FPUL,dr2 */
L8c10f1f8: *stop=0x8c10f1f8u;fcopy(f,4u,2u);goto L8c10f1fa; /* fmov fr2,fr4 */
L8c10f1fa: take=!(*sr&1u);*stop=0x8c10f1fcu;fcopy(f,5u,3u);if(take)goto L8c10f202;goto L8c10f1fe; /* bf/s 0x8c10f202 */
L8c10f1fe: *stop=0x8c10f1feu;r[2]=6u;goto L8c10f200; /* mov #0x6,r2 */
L8c10f200: *stop=0x8c10f200u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c10f202; /* mov.l r2,@(0x8,r14) */
L8c10f202: *stop=0x8c10f202u;r[12]=r[15];goto L8c10f204; /* mov r15,r12 */
L8c10f204: *stop=0x8c10f204u;r[12]+=24u;goto L8c10f206; /* add #0x18,r12 */
L8c10f206: *stop=0x8c10f206u;r[2]=48u;goto L8c10f208; /* mov #0x30,r2 */
L8c10f208: *stop=0x8c10f208u;r[3]=r[12];goto L8c10f20a; /* mov r12,r3 */
L8c10f20a: *stop=0x8c10f20au;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c10f20c; /* mov.l r3,@(0xc,r15) */
L8c10f20c: *stop=0x8c10f20cu;r[0]=0x8c10f388u;goto L8c10f20e; /* mova 0x8c10f388,r0 */
L8c10f20e: *stop=0x8c10f20eu;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[2],1);if(io_status)return io_status;goto L8c10f210; /* mov.b r2,@r3 */
L8c10f210: *stop=0x8c10f210u;r[9]=r[13];goto L8c10f212; /* mov r13,r9 */
L8c10f212: *stop=0x8c10f212u;r[3]=f->fpscr;goto L8c10f214; /* sts FPSCR,r3 */
L8c10f214: *stop=0x8c10f214u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f384u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f216; /* mov.l 0x8c10f384,r2 */
L8c10f216: *stop=0x8c10f216u;r[8]=1u;goto L8c10f218; /* mov #0x1,r8 */
L8c10f218: *stop=0x8c10f218u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c10f21a; /* fmov.s @r0,fr2 */
L8c10f21a: *stop=0x8c10f21au;r[3]|=r[2];goto L8c10f21c; /* or r2,r3 */
L8c10f21c: *stop=0x8c10f21cu;fcopy(f,3u,2u);goto L8c10f21e; /* fmov fr2,fr3 */
L8c10f21e: *stop=0x8c10f21eu;f->fpscr=r[3];goto L8c10f220; /* lds r3,fpscr */
L8c10f220: *stop=0x8c10f220u;status=sa_sh4_compare_pair(f,sr,4u,2u,0);if(status)return status;goto L8c10f222; /* fcmp/eq fr2,fr4 */
L8c10f222: take=(*sr&1u);*stop=0x8c10f224u;r[11]=0u;if(take)goto L8c10f28e;goto L8c10f226; /* bt/s 0x8c10f28e */
L8c10f226: *stop=0x8c10f226u;r[2]=f->fpscr;goto L8c10f228; /* sts FPSCR,r2 */
L8c10f228: *stop=0x8c10f228u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f38cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f22a; /* mov.l 0x8c10f38c,r3 */
L8c10f22a: *stop=0x8c10f22au;r[7]=r[15];goto L8c10f22c; /* mov r15,r7 */
L8c10f22c: *stop=0x8c10f22cu;r[5]=r[15];goto L8c10f22e; /* mov r15,r5 */
L8c10f22e: *stop=0x8c10f22eu;r[7]+=20u;goto L8c10f230; /* add #0x14,r7 */
L8c10f230: *stop=0x8c10f230u;r[2]&=r[3];goto L8c10f232; /* and r3,r2 */
L8c10f232: *stop=0x8c10f232u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f390u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f234; /* mov.l 0x8c10f390,r3 */
L8c10f234: *stop=0x8c10f234u;f->fpscr=r[2];goto L8c10f236; /* lds r2,fpscr */
L8c10f236: *stop=0x8c10f236u;r[6]=r[15];goto L8c10f238; /* mov r15,r6 */
L8c10f238: *stop=0x8c10f238u;r[5]+=56u;goto L8c10f23a; /* add #0x38,r5 */
L8c10f23a: target=r[3];*pr=0x8c10f23eu;*stop=0x8c10f23cu;r[4]=17u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f23e; /* jsr @r3 */
L8c10f23e: *stop=0x8c10f23eu;r[4]=r[0];goto L8c10f240; /* mov r0,r4 */
L8c10f240: *stop=0x8c10f240u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c10f242; /* tst r4,r4 */
L8c10f242: if((*sr&1u))goto L8c10f274;goto L8c10f244; /* bt 0x8c10f274 */
L8c10f244: *stop=0x8c10f244u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[11],1);if(io_status)return io_status;goto L8c10f246; /* mov.b r11,@r13 */
L8c10f246: *stop=0x8c10f246u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f248; /* mov.l @(0x8,r14),r2 */
L8c10f248: *stop=0x8c10f248u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f24a; /* mov.l @(0x4,r14),r3 */
L8c10f24a: *stop=0x8c10f24au;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]);goto L8c10f24c; /* cmp/gt r3,r2 */
L8c10f24c: if(!(*sr&1u))goto L8c10f254;goto L8c10f24e; /* bf 0x8c10f254 */
L8c10f24e: *stop=0x8c10f24eu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f250; /* mov.l @(0x8,r14),r0 */
L8c10f250: *stop=0x8c10f252u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[0],4);if(io_status)return io_status;goto L8c10f258; /* bra 0x8c10f258 */
L8c10f254: *stop=0x8c10f254u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f256; /* mov.l @(0x4,r14),r2 */
L8c10f256: *stop=0x8c10f256u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[2],4);if(io_status)return io_status;goto L8c10f258; /* mov.l r2,@(0x28,r14) */
L8c10f258: *stop=0x8c10f258u;r[0]=r[4];goto L8c10f25a; /* mov r4,r0 */
L8c10f25a: *stop=0x8c10f25au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10f25c; /* cmp/eq #0x1,r0 */
L8c10f25c: if((*sr&1u))goto L8c10f266;goto L8c10f25e; /* bt 0x8c10f266 */
L8c10f25e: *stop=0x8c10f25eu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10f260; /* cmp/eq #-0x1,r0 */
L8c10f260: if((*sr&1u))goto L8c10f26a;goto L8c10f262; /* bt 0x8c10f26a */
L8c10f262: *stop=0x8c10f264u;goto L8c10f26e; /* bra 0x8c10f26e */
L8c10f266: *stop=0x8c10f268u;r[0]=43u;goto L8c10f270; /* bra 0x8c10f270 */
L8c10f26a: *stop=0x8c10f26cu;r[0]=45u;goto L8c10f270; /* bra 0x8c10f270 */
L8c10f26e: *stop=0x8c10f26eu;r[0]=42u;goto L8c10f270; /* mov #0x2a,r0 */
L8c10f270: *stop=0x8c10f272u;if(!valid_access(r[14]+1u,1u))return -3;wr(m,bus,&io_status,r[14]+0x1u,r[0],1);if(io_status)return io_status;goto L8c10f88e; /* bra 0x8c10f88e */
L8c10f274: *stop=0x8c10f274u;r[3]=f->fpscr;goto L8c10f276; /* sts FPSCR,r3 */
L8c10f276: *stop=0x8c10f276u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f38cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f278; /* mov.l 0x8c10f38c,r2 */
L8c10f278: *stop=0x8c10f278u;r[5]=r[12];goto L8c10f27a; /* mov r12,r5 */
L8c10f27a: *stop=0x8c10f27au;r[5]+=1u;goto L8c10f27c; /* add #0x1,r5 */
L8c10f27c: *stop=0x8c10f27cu;r[4]=r[15];goto L8c10f27e; /* mov r15,r4 */
L8c10f27e: *stop=0x8c10f27eu;r[3]&=r[2];goto L8c10f280; /* and r2,r3 */
L8c10f280: *stop=0x8c10f280u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f394u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f282; /* mov.l 0x8c10f394,r2 */
L8c10f282: *stop=0x8c10f282u;f->fpscr=r[3];goto L8c10f284; /* lds r3,fpscr */
L8c10f284: *stop=0x8c10f284u;r[6]=17u;goto L8c10f286; /* mov #0x11,r6 */
L8c10f286: target=r[2];*pr=0x8c10f28au;*stop=0x8c10f288u;r[4]+=56u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f28a; /* jsr @r2 */
L8c10f28a: *stop=0x8c10f28cu;goto L8c10f296; /* bra 0x8c10f296 */
L8c10f28e: *stop=0x8c10f28eu;r[0]=20u;goto L8c10f290; /* mov #0x14,r0 */
L8c10f290: *stop=0x8c10f290u;if(!valid_access(r[0]+r[15],1u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[8],1);if(io_status)return io_status;goto L8c10f292; /* mov.b r8,@(r0,r15) */
L8c10f292: *stop=0x8c10f292u;r[0]=r[11];goto L8c10f294; /* mov r11,r0 */
L8c10f294: *stop=0x8c10f294u;if(!valid_access(r[12]+1u,1u))return -3;wr(m,bus,&io_status,r[12]+0x1u,r[0],1);if(io_status)return io_status;goto L8c10f296; /* mov.b r0,@(0x1,r12) */
L8c10f296: *stop=0x8c10f298u;r[10]=r[8];goto L8c10f29c; /* bra 0x8c10f29c */
L8c10f29a: *stop=0x8c10f29au;r[10]+=1u;goto L8c10f29c; /* add #0x1,r10 */
L8c10f29c: *stop=0x8c10f29cu;r[0]=r[10];goto L8c10f29e; /* mov r10,r0 */
L8c10f29e: *stop=0x8c10f29eu;if(!valid_access(r[0]+r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f2a0; /* mov.b @(r0,r12),r0 */
L8c10f2a0: *stop=0x8c10f2a0u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f2a2; /* cmp/eq #0x30,r0 */
L8c10f2a2: if((*sr&1u))goto L8c10f29a;goto L8c10f2a4; /* bt 0x8c10f29a */
L8c10f2a4: *stop=0x8c10f2a4u;*sr=(*sr&~1u)|((int32_t)r[10]>(int32_t)r[8]);goto L8c10f2a6; /* cmp/gt r8,r10 */
L8c10f2a6: if(!(*sr&1u))goto L8c10f2dc;goto L8c10f2a8; /* bf 0x8c10f2dc */
L8c10f2a8: *stop=0x8c10f2a8u;r[2]=f->fpscr;goto L8c10f2aa; /* sts FPSCR,r2 */
L8c10f2aa: *stop=0x8c10f2aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f38cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f2ac; /* mov.l 0x8c10f38c,r3 */
L8c10f2ac: *stop=0x8c10f2acu;r[4]=r[10];goto L8c10f2ae; /* mov r10,r4 */
L8c10f2ae: *stop=0x8c10f2aeu;r[2]&=r[3];goto L8c10f2b0; /* and r3,r2 */
L8c10f2b0: *stop=0x8c10f2b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f398u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f2b2; /* mov.l 0x8c10f398,r3 */
L8c10f2b2: *stop=0x8c10f2b2u;f->fpscr=r[2];goto L8c10f2b4; /* lds r2,fpscr */
L8c10f2b4: target=r[3];*pr=0x8c10f2b8u;*stop=0x8c10f2b6u;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f2b8; /* jsr @r3 */
L8c10f2b8: *stop=0x8c10f2b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f2ba; /* mov.l @r15,r3 */
L8c10f2ba: *stop=0x8c10f2bau;r[2]=r[10];goto L8c10f2bc; /* mov r10,r2 */
L8c10f2bc: *stop=0x8c10f2bcu;r[4]=r[8];goto L8c10f2be; /* mov r8,r4 */
L8c10f2be: *stop=0x8c10f2beu;r[5]=r[0];goto L8c10f2c0; /* mov r0,r5 */
L8c10f2c0: *stop=0x8c10f2c0u;r[2]+=4294967295u;goto L8c10f2c2; /* add #-0x1,r2 */
L8c10f2c2: *stop=0x8c10f2c2u;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[5]);goto L8c10f2c4; /* cmp/gt r5,r4 */
L8c10f2c4: *stop=0x8c10f2c4u;r[3]+=r[2];goto L8c10f2c6; /* add r2,r3 */
L8c10f2c6: take=(*sr&1u);*stop=0x8c10f2c8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c10f2d8;goto L8c10f2ca; /* bt/s 0x8c10f2d8 */
L8c10f2ca: *stop=0x8c10f2cau;r[0]=r[12];goto L8c10f2cc; /* mov r12,r0 */
L8c10f2cc: *stop=0x8c10f2ccu;if(!valid_access(r[0]+r[10],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[10]+r[0],1);if(io_status)return io_status;r[3]=loaded;}goto L8c10f2ce; /* mov.b @(r0,r10),r3 */
L8c10f2ce: *stop=0x8c10f2ceu;if(!valid_access(r[0]+r[4],1u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[3],1);if(io_status)return io_status;goto L8c10f2d0; /* mov.b r3,@(r0,r4) */
L8c10f2d0: *stop=0x8c10f2d0u;r[4]+=1u;goto L8c10f2d2; /* add #0x1,r4 */
L8c10f2d2: *stop=0x8c10f2d2u;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[5]);goto L8c10f2d4; /* cmp/gt r5,r4 */
L8c10f2d4: take=!(*sr&1u);*stop=0x8c10f2d6u;r[10]+=1u;if(take)goto L8c10f2ca;goto L8c10f2d8; /* bf/s 0x8c10f2ca */
L8c10f2d8: *stop=0x8c10f2d8u;r[0]=r[4];goto L8c10f2da; /* mov r4,r0 */
L8c10f2da: *stop=0x8c10f2dau;if(!valid_access(r[0]+r[12],1u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[11],1);if(io_status)return io_status;goto L8c10f2dc; /* mov.b r11,@(r0,r12) */
L8c10f2dc: *stop=0x8c10f2dcu;r[2]=f->fpscr;goto L8c10f2de; /* sts FPSCR,r2 */
L8c10f2de: *stop=0x8c10f2deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f38cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f2e0; /* mov.l 0x8c10f38c,r3 */
L8c10f2e0: *stop=0x8c10f2e0u;r[4]=r[12];goto L8c10f2e2; /* mov r12,r4 */
L8c10f2e2: *stop=0x8c10f2e2u;r[4]+=1u;goto L8c10f2e4; /* add #0x1,r4 */
L8c10f2e4: *stop=0x8c10f2e4u;r[2]&=r[3];goto L8c10f2e6; /* and r3,r2 */
L8c10f2e6: *stop=0x8c10f2e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f398u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f2e8; /* mov.l 0x8c10f398,r3 */
L8c10f2e8: *stop=0x8c10f2e8u;f->fpscr=r[2];goto L8c10f2ea; /* lds r2,fpscr */
L8c10f2ea: target=r[3];*pr=0x8c10f2eeu;*stop=0x8c10f2ecu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f2ee; /* jsr @r3 */
L8c10f2ee: *stop=0x8c10f2eeu;r[10]=r[0];goto L8c10f2f0; /* mov r0,r10 */
L8c10f2f0: *stop=0x8c10f2f0u;r[2]=0u;goto L8c10f2f2; /* mov #0x0,r2 */
L8c10f2f2: *stop=0x8c10f2f2u;r[3]=r[10];goto L8c10f2f4; /* mov r10,r3 */
L8c10f2f4: *stop=0x8c10f2f4u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]);goto L8c10f2f6; /* cmp/gt r3,r2 */
L8c10f2f6: *stop=0x8c10f2f6u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10f2f8; /* mov.l @(0x10,r15),r4 */
L8c10f2f8: *stop=0x8c10f2f8u;{uint64_t sum=(uint64_t)r[3]+r[2]+(*sr&1u);r[3]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);}goto L8c10f2fa; /* addc r2,r3 */
L8c10f2fa: *stop=0x8c10f2fau;*sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u);goto L8c10f2fc; /* shar r3 */
L8c10f2fc: *stop=0x8c10f2fcu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10f2fe; /* mov.l @(0x10,r15),r5 */
L8c10f2fe: *stop=0x8c10f2feu;r[4]+=r[10];goto L8c10f300; /* add r10,r4 */
L8c10f300: *stop=0x8c10f300u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c10f302; /* cmp/pl r3 */
L8c10f302: *stop=0x8c10f302u;r[6]=r[11];goto L8c10f304; /* mov r11,r6 */
L8c10f304: *stop=0x8c10f304u;r[4]+=4294967295u;goto L8c10f306; /* add #-0x1,r4 */
L8c10f306: take=!(*sr&1u);*stop=0x8c10f308u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;if(take)goto L8c10f31e;goto L8c10f30a; /* bf/s 0x8c10f31e */
L8c10f30a: *stop=0x8c10f30au;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[7]=loaded;}goto L8c10f30c; /* mov.b @r5,r7 */
L8c10f30c: *stop=0x8c10f30cu;r[6]+=1u;goto L8c10f30e; /* add #0x1,r6 */
L8c10f30e: *stop=0x8c10f30eu;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}goto L8c10f310; /* mov.b @r4,r3 */
L8c10f310: *stop=0x8c10f310u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c10f312; /* mov.b r3,@r5 */
L8c10f312: *stop=0x8c10f312u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[7],1);if(io_status)return io_status;goto L8c10f314; /* mov.b r7,@r4 */
L8c10f314: *stop=0x8c10f314u;r[4]+=4294967295u;goto L8c10f316; /* add #-0x1,r4 */
L8c10f316: *stop=0x8c10f316u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f318; /* mov.l @(0x4,r15),r2 */
L8c10f318: *stop=0x8c10f318u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[2]);goto L8c10f31a; /* cmp/ge r2,r6 */
L8c10f31a: take=!(*sr&1u);*stop=0x8c10f31cu;r[5]+=1u;if(take)goto L8c10f30a;goto L8c10f31e; /* bf/s 0x8c10f30a */
L8c10f31e: *stop=0x8c10f31eu;*sr=(*sr&~1u)|!(r[10]&r[10]);goto L8c10f320; /* tst r10,r10 */
L8c10f320: if(!(*sr&1u))goto L8c10f32c;goto L8c10f322; /* bf 0x8c10f32c */
L8c10f322: *stop=0x8c10f322u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f324; /* mov.l @(0x10,r15),r3 */
L8c10f324: *stop=0x8c10f324u;r[2]=48u;goto L8c10f326; /* mov #0x30,r2 */
L8c10f326: *stop=0x8c10f326u;r[10]=r[8];goto L8c10f328; /* mov r8,r10 */
L8c10f328: *stop=0x8c10f328u;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[2],1);if(io_status)return io_status;goto L8c10f32a; /* mov.b r2,@r3 */
L8c10f32a: *stop=0x8c10f32au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10f32c; /* mov.l r11,@r15 */
L8c10f32c: *stop=0x8c10f32cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f32e; /* mov.l @(0x8,r15),r0 */
L8c10f32e: *stop=0x8c10f32eu;*sr=(*sr&~1u)|(r[0]==0x00000067u);goto L8c10f330; /* cmp/eq #0x67,r0 */
L8c10f330: if((*sr&1u))goto L8c10f338;goto L8c10f332; /* bt 0x8c10f338 */
L8c10f332: *stop=0x8c10f332u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f334; /* mov.l @(0x8,r15),r0 */
L8c10f334: *stop=0x8c10f334u;*sr=(*sr&~1u)|(r[0]==0x00000047u);goto L8c10f336; /* cmp/eq #0x47,r0 */
L8c10f336: if(!(*sr&1u))goto L8c10f35c;goto L8c10f338; /* bf 0x8c10f35c */
L8c10f338: *stop=0x8c10f338u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f33a; /* mov.b @r14,r0 */
L8c10f33a: *stop=0x8c10f33au;r[3]=4294967292u;goto L8c10f33c; /* mov #-0x4,r3 */
L8c10f33c: *stop=0x8c10f33cu;r[0]|=0x20u;goto L8c10f33e; /* or #0x20,r0 */
L8c10f33e: *stop=0x8c10f33eu;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[0],1);if(io_status)return io_status;goto L8c10f340; /* mov.b r0,@r14 */
L8c10f340: *stop=0x8c10f340u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c10f342; /* mov.l @r15,r4 */
L8c10f342: *stop=0x8c10f342u;r[4]+=r[10];goto L8c10f344; /* add r10,r4 */
L8c10f344: *stop=0x8c10f344u;r[4]+=4294967295u;goto L8c10f346; /* add #-0x1,r4 */
L8c10f346: *stop=0x8c10f346u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c10f348; /* cmp/ge r3,r4 */
L8c10f348: if(!(*sr&1u))goto L8c10f350;goto L8c10f34a; /* bf 0x8c10f350 */
L8c10f34a: *stop=0x8c10f34au;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f34c; /* mov.l @(0x8,r14),r1 */
L8c10f34c: *stop=0x8c10f34cu;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]);goto L8c10f34e; /* cmp/gt r1,r4 */
L8c10f34e: if(!(*sr&1u))goto L8c10f358;goto L8c10f350; /* bf 0x8c10f358 */
L8c10f350: *stop=0x8c10f350u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f352; /* mov.l @(0x8,r15),r3 */
L8c10f352: *stop=0x8c10f352u;r[3]+=4294967294u;goto L8c10f354; /* add #-0x2,r3 */
L8c10f354: *stop=0x8c10f356u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10f35c; /* bra 0x8c10f35c */
L8c10f358: *stop=0x8c10f358u;r[1]=102u;goto L8c10f35a; /* mov #0x66,r1 */
L8c10f35a: *stop=0x8c10f35au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c10f35c; /* mov.l r1,@(0x8,r15) */
L8c10f35c: *stop=0x8c10f35cu;r[0]=20u;goto L8c10f35e; /* mov #0x14,r0 */
L8c10f35e: *stop=0x8c10f35eu;if(!valid_access(r[0]+r[15],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f360; /* mov.b @(r0,r15),r0 */
L8c10f360: *stop=0x8c10f360u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10f362; /* cmp/eq #0x1,r0 */
L8c10f362: if(!(*sr&1u))goto L8c10f39c;goto L8c10f364; /* bf 0x8c10f39c */
L8c10f364: *stop=0x8c10f364u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f366; /* mov.b @r14,r0 */
L8c10f366: *stop=0x8c10f366u;r[0]&=0x18u;goto L8c10f368; /* and #0x18,r0 */
L8c10f368: *stop=0x8c10f368u;r[4]=r[0];goto L8c10f36a; /* mov r0,r4 */
L8c10f36a: *stop=0x8c10f36au;r[0]=(uint32_t)(int32_t)(int8_t)r[4];goto L8c10f36c; /* exts.b r4,r0 */
L8c10f36c: *stop=0x8c10f36cu;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c10f36e; /* cmp/eq #0x8,r0 */
L8c10f36e: if((*sr&1u))goto L8c10f378;goto L8c10f370; /* bt 0x8c10f378 */
L8c10f370: *stop=0x8c10f370u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c10f372; /* cmp/eq #0x10,r0 */
L8c10f372: if((*sr&1u))goto L8c10f37c;goto L8c10f374; /* bt 0x8c10f37c */
L8c10f374: *stop=0x8c10f376u;goto L8c10f3a2; /* bra 0x8c10f3a2 */
L8c10f378: *stop=0x8c10f37au;r[3]=43u;goto L8c10f37e; /* bra 0x8c10f37e */
L8c10f37c: *stop=0x8c10f37cu;r[3]=32u;goto L8c10f37e; /* mov #0x20,r3 */
L8c10f37e: *stop=0x8c10f37eu;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[3],1);if(io_status)return io_status;goto L8c10f380; /* mov.b r3,@r13 */
L8c10f380: *stop=0x8c10f382u;r[13]+=1u;goto L8c10f3a2; /* bra 0x8c10f3a2 */
L8c10f39c: *stop=0x8c10f39cu;r[3]=45u;goto L8c10f39e; /* mov #0x2d,r3 */
L8c10f39e: *stop=0x8c10f39eu;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[3],1);if(io_status)return io_status;goto L8c10f3a0; /* mov.b r3,@r13 */
L8c10f3a0: *stop=0x8c10f3a0u;r[13]+=1u;goto L8c10f3a2; /* add #0x1,r13 */
L8c10f3a2: *stop=0x8c10f3a2u;r[2]=r[9];goto L8c10f3a4; /* mov r9,r2 */
L8c10f3a4: *stop=0x8c10f3a4u;r[2]+=1u;goto L8c10f3a6; /* add #0x1,r2 */
L8c10f3a6: *stop=0x8c10f3a6u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c10f3a8; /* mov.l r2,@(0x4,r15) */
L8c10f3a8: *stop=0x8c10f3a8u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f3aa; /* mov.l @(0x8,r15),r0 */
L8c10f3aa: *stop=0x8c10f3aau;*sr=(*sr&~1u)|(r[0]==0x00000066u);goto L8c10f3ac; /* cmp/eq #0x66,r0 */
L8c10f3ac: if((*sr&1u))goto L8c10f3b2;goto L8c10f3ae; /* bt 0x8c10f3b2 */
L8c10f3ae: *stop=0x8c10f3b0u;goto L8c10f6f8; /* bra 0x8c10f6f8 */
L8c10f3b2: *stop=0x8c10f3b2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f3b4; /* mov.l @r15,r1 */
L8c10f3b4: *stop=0x8c10f3b4u;*sr=(*sr&~1u)|((int32_t)r[1]>=0);goto L8c10f3b6; /* cmp/pz r1 */
L8c10f3b6: if(!(*sr&1u))goto L8c10f454;goto L8c10f3b8; /* bf 0x8c10f454 */
L8c10f3b8: *stop=0x8c10f3b8u;*sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c10f3ba; /* cmp/pl r10 */
L8c10f3ba: *stop=0x8c10f3bau;r[4]=r[11];goto L8c10f3bc; /* mov r11,r4 */
L8c10f3bc: take=!(*sr&1u);*stop=0x8c10f3beu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}if(take)goto L8c10f3cc;goto L8c10f3c0; /* bf/s 0x8c10f3cc */
L8c10f3c0: *stop=0x8c10f3c0u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[3]=loaded;}r[5]+=1;goto L8c10f3c2; /* mov.b @r5+,r3 */
L8c10f3c2: *stop=0x8c10f3c2u;r[4]+=1u;goto L8c10f3c4; /* add #0x1,r4 */
L8c10f3c4: *stop=0x8c10f3c4u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[10]);goto L8c10f3c6; /* cmp/ge r10,r4 */
L8c10f3c6: *stop=0x8c10f3c6u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[3],1);if(io_status)return io_status;goto L8c10f3c8; /* mov.b r3,@r13 */
L8c10f3c8: take=!(*sr&1u);*stop=0x8c10f3cau;r[13]+=1u;if(take)goto L8c10f3c0;goto L8c10f3cc; /* bf/s 0x8c10f3c0 */
L8c10f3cc: *stop=0x8c10f3ccu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[13],4);if(io_status)return io_status;goto L8c10f3ce; /* mov.l r13,@(0x14,r14) */
L8c10f3ce: *stop=0x8c10f3ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f3d0; /* mov.l @r15,r3 */
L8c10f3d0: *stop=0x8c10f3d0u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[3],4);if(io_status)return io_status;goto L8c10f3d2; /* mov.l r3,@(0x20,r14) */
L8c10f3d2: *stop=0x8c10f3d2u;r[3]=32u;goto L8c10f3d4; /* mov #0x20,r3 */
L8c10f3d4: *stop=0x8c10f3d4u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[5]=loaded;}goto L8c10f3d6; /* mov.b @r14,r5 */
L8c10f3d6: *stop=0x8c10f3d6u;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c10f3d8; /* tst r5,r3 */
L8c10f3d8: take=!(*sr&1u);*stop=0x8c10f3dau;r[4]=r[11];if(take)goto L8c10f3e2;goto L8c10f3dc; /* bf/s 0x8c10f3e2 */
L8c10f3dc: *stop=0x8c10f3dcu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f3de; /* mov.l @(0x8,r14),r1 */
L8c10f3de: *stop=0x8c10f3deu;*sr=(*sr&~1u)|((int32_t)r[1]>0);goto L8c10f3e0; /* cmp/pl r1 */
L8c10f3e0: if((*sr&1u))goto L8c10f3e8;goto L8c10f3e2; /* bt 0x8c10f3e8 */
L8c10f3e2: *stop=0x8c10f3e2u;r[2]=4u;goto L8c10f3e4; /* mov #0x4,r2 */
L8c10f3e4: *stop=0x8c10f3e4u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f3e6; /* tst r5,r2 */
L8c10f3e6: if((*sr&1u))goto L8c10f3f0;goto L8c10f3e8; /* bt 0x8c10f3f0 */
L8c10f3e8: *stop=0x8c10f3e8u;r[3]=46u;goto L8c10f3ea; /* mov #0x2e,r3 */
L8c10f3ea: *stop=0x8c10f3eau;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[3],1);if(io_status)return io_status;goto L8c10f3ec; /* mov.b r3,@r13 */
L8c10f3ec: *stop=0x8c10f3ecu;r[4]+=1u;goto L8c10f3ee; /* add #0x1,r4 */
L8c10f3ee: *stop=0x8c10f3eeu;r[13]+=1u;goto L8c10f3f0; /* add #0x1,r13 */
L8c10f3f0: *stop=0x8c10f3f0u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[5]=loaded;}goto L8c10f3f2; /* mov.b @r14,r5 */
L8c10f3f2: *stop=0x8c10f3f2u;r[2]=32u;goto L8c10f3f4; /* mov #0x20,r2 */
L8c10f3f4: *stop=0x8c10f3f4u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f3f6; /* tst r5,r2 */
L8c10f3f6: if((*sr&1u))goto L8c10f404;goto L8c10f3f8; /* bt 0x8c10f404 */
L8c10f3f8: *stop=0x8c10f3f8u;r[1]=32u;goto L8c10f3fa; /* mov #0x20,r1 */
L8c10f3fa: *stop=0x8c10f3fau;*sr=(*sr&~1u)|!(r[5]&r[1]);goto L8c10f3fc; /* tst r5,r1 */
L8c10f3fc: if((*sr&1u))goto L8c10f40e;goto L8c10f3fe; /* bt 0x8c10f40e */
L8c10f3fe: *stop=0x8c10f3feu;r[2]=4u;goto L8c10f400; /* mov #0x4,r2 */
L8c10f400: *stop=0x8c10f400u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f402; /* tst r5,r2 */
L8c10f402: if((*sr&1u))goto L8c10f40e;goto L8c10f404; /* bt 0x8c10f40e */
L8c10f404: *stop=0x8c10f404u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[13],4);if(io_status)return io_status;goto L8c10f406; /* mov.l r13,@(0x18,r14) */
L8c10f406: *stop=0x8c10f406u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f408; /* mov.l @(0x8,r14),r2 */
L8c10f408: *stop=0x8c10f408u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[2],4);if(io_status)return io_status;goto L8c10f40a; /* mov.l r2,@(0x24,r14) */
L8c10f40a: *stop=0x8c10f40au;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f40c; /* mov.l @(0x8,r14),r3 */
L8c10f40c: *stop=0x8c10f40cu;r[4]+=r[3];goto L8c10f40e; /* add r3,r4 */
L8c10f40e: *stop=0x8c10f40eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f410; /* mov.l @r15,r2 */
L8c10f410: *stop=0x8c10f410u;if(!valid_access(r[9],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f412; /* mov.b @r9,r0 */
L8c10f412: *stop=0x8c10f412u;r[4]+=r[2];goto L8c10f414; /* add r2,r4 */
L8c10f414: *stop=0x8c10f414u;*sr=(*sr&~1u)|(r[0]==0x0000002bu);goto L8c10f416; /* cmp/eq #0x2b,r0 */
L8c10f416: *stop=0x8c10f416u;r[4]+=r[10];goto L8c10f418; /* add r10,r4 */
L8c10f418: take=(*sr&1u);*stop=0x8c10f41au;r[5]=r[0];if(take)goto L8c10f428;goto L8c10f41c; /* bt/s 0x8c10f428 */
L8c10f41c: *stop=0x8c10f41cu;r[0]=r[5];goto L8c10f41e; /* mov r5,r0 */
L8c10f41e: *stop=0x8c10f41eu;*sr=(*sr&~1u)|(r[0]==0x0000002du);goto L8c10f420; /* cmp/eq #0x2d,r0 */
L8c10f420: if((*sr&1u))goto L8c10f428;goto L8c10f422; /* bt 0x8c10f428 */
L8c10f422: *stop=0x8c10f422u;r[0]=r[5];goto L8c10f424; /* mov r5,r0 */
L8c10f424: *stop=0x8c10f424u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c10f426; /* cmp/eq #0x20,r0 */
L8c10f426: if(!(*sr&1u))goto L8c10f442;goto L8c10f428; /* bf 0x8c10f442 */
L8c10f428: *stop=0x8c10f428u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f42a; /* mov.l @(0x4,r14),r2 */
L8c10f42a: *stop=0x8c10f42au;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[4]);goto L8c10f42c; /* cmp/gt r4,r2 */
L8c10f42c: if(!(*sr&1u))goto L8c10f442;goto L8c10f42e; /* bf 0x8c10f442 */
L8c10f42e: *stop=0x8c10f42eu;if(!valid_access(r[14]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f430; /* mov.b @(0x1,r14),r0 */
L8c10f430: *stop=0x8c10f430u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f432; /* cmp/eq #0x30,r0 */
L8c10f432: if(!(*sr&1u))goto L8c10f442;goto L8c10f434; /* bf 0x8c10f442 */
L8c10f434: *stop=0x8c10f434u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f436; /* mov.l @(0x4,r15),r2 */
L8c10f436: *stop=0x8c10f436u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c10f438; /* mov.l r2,@(0x10,r14) */
L8c10f438: *stop=0x8c10f438u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f43a; /* mov.l @(0x4,r14),r3 */
L8c10f43a: *stop=0x8c10f43au;r[3]-=r[4];goto L8c10f43c; /* sub r4,r3 */
L8c10f43c: *stop=0x8c10f43cu;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c10f43e; /* mov.l r3,@(0x1c,r14) */
L8c10f43e: *stop=0x8c10f440u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[11],4);if(io_status)return io_status;goto L8c10f88c; /* bra 0x8c10f88c */
L8c10f442: *stop=0x8c10f442u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f444; /* mov.l @(0x4,r14),r3 */
L8c10f444: *stop=0x8c10f444u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[4]);goto L8c10f446; /* cmp/gt r4,r3 */
L8c10f446: if(!(*sr&1u))goto L8c10f450;goto L8c10f448; /* bf 0x8c10f450 */
L8c10f448: *stop=0x8c10f448u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f44a; /* mov.l @(0x4,r14),r1 */
L8c10f44a: *stop=0x8c10f44au;r[1]-=r[4];goto L8c10f44c; /* sub r4,r1 */
L8c10f44c: *stop=0x8c10f44eu;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[1],4);if(io_status)return io_status;goto L8c10f88c; /* bra 0x8c10f88c */
L8c10f450: *stop=0x8c10f452u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[11],4);if(io_status)return io_status;goto L8c10f88c; /* bra 0x8c10f88c */
L8c10f454: *stop=0x8c10f454u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f456; /* mov.l @r15,r3 */
L8c10f456: *stop=0x8c10f456u;r[3]+=r[10];goto L8c10f458; /* add r10,r3 */
L8c10f458: *stop=0x8c10f458u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10f45a; /* mov.l r3,@r15 */
L8c10f45a: *stop=0x8c10f45au;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f45c; /* mov.l @(0x8,r14),r2 */
L8c10f45c: *stop=0x8c10f45cu;r[3]+=r[2];goto L8c10f45e; /* add r2,r3 */
L8c10f45e: *stop=0x8c10f45eu;r[2]=r[3];goto L8c10f460; /* mov r3,r2 */
L8c10f460: *stop=0x8c10f460u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[10]);goto L8c10f462; /* cmp/ge r10,r2 */
L8c10f462: *stop=0x8c10f462u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10f464; /* mov.l r3,@(0x8,r15) */
L8c10f464: if((*sr&1u))goto L8c10f47c;goto L8c10f466; /* bt 0x8c10f47c */
L8c10f466: *stop=0x8c10f466u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f468; /* mov.l @(0x8,r15),r1 */
L8c10f468: *stop=0x8c10f468u;*sr=(*sr&~1u)|((int32_t)r[1]>=0);goto L8c10f46a; /* cmp/pz r1 */
L8c10f46a: if(!(*sr&1u))goto L8c10f47c;goto L8c10f46c; /* bf 0x8c10f47c */
L8c10f46c: *stop=0x8c10f46cu;r[2]=f->fpscr;goto L8c10f46e; /* sts FPSCR,r2 */
L8c10f46e: *stop=0x8c10f46eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f58cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f470; /* mov.l 0x8c10f58c,r3 */
L8c10f470: *stop=0x8c10f470u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10f472; /* mov.l @(0x8,r15),r5 */
L8c10f472: *stop=0x8c10f472u;r[2]&=r[3];goto L8c10f474; /* and r3,r2 */
L8c10f474: *stop=0x8c10f474u;f->fpscr=r[2];goto L8c10f476; /* lds r2,fpscr */
L8c10f476: *stop=0x8c10f476u;r[5]+=1u;goto L8c10f478; /* add #0x1,r5 */
L8c10f478: target=0x8c10f8a2u;*pr=0x8c10f47cu;*stop=0x8c10f47au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f47c; /* bsr 0x8c10f8a2 */
L8c10f47c: *stop=0x8c10f47cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f47e; /* mov.l @r15,r2 */
L8c10f47e: *stop=0x8c10f47eu;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c10f480; /* cmp/pl r2 */
L8c10f480: if((*sr&1u))goto L8c10f486;goto L8c10f482; /* bt 0x8c10f486 */
L8c10f482: *stop=0x8c10f484u;goto L8c10f5a2; /* bra 0x8c10f5a2 */
L8c10f486: *stop=0x8c10f486u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f488; /* mov.l @(0xc,r15),r0 */
L8c10f488: *stop=0x8c10f488u;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f48a; /* mov.b @r0,r0 */
L8c10f48a: *stop=0x8c10f48au;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f48c; /* cmp/eq #0x30,r0 */
L8c10f48c: take=!(*sr&1u);*stop=0x8c10f48eu;r[4]=r[11];if(take)goto L8c10f496;goto L8c10f490; /* bf/s 0x8c10f496 */
L8c10f490: *stop=0x8c10f490u;r[5]=r[8];goto L8c10f492; /* mov r8,r5 */
L8c10f492: *stop=0x8c10f494u;r[6]=r[8];goto L8c10f4a2; /* bra 0x8c10f4a2 */
L8c10f496: *stop=0x8c10f496u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f498; /* mov.l @r15,r3 */
L8c10f498: *stop=0x8c10f498u;r[10]+=1u;goto L8c10f49a; /* add #0x1,r10 */
L8c10f49a: *stop=0x8c10f49au;r[5]=r[11];goto L8c10f49c; /* mov r11,r5 */
L8c10f49c: *stop=0x8c10f49cu;r[6]=r[11];goto L8c10f49e; /* mov r11,r6 */
L8c10f49e: *stop=0x8c10f49eu;r[3]+=1u;goto L8c10f4a0; /* add #0x1,r3 */
L8c10f4a0: *stop=0x8c10f4a0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10f4a2; /* mov.l r3,@r15 */
L8c10f4a2: *stop=0x8c10f4a4u;goto L8c10f4b8; /* bra 0x8c10f4b8 */
L8c10f4a6: *stop=0x8c10f4a6u;r[0]=r[5];goto L8c10f4a8; /* mov r5,r0 */
L8c10f4a8: *stop=0x8c10f4a8u;if(!valid_access(r[0]+r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12]+r[0],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10f4aa; /* mov.b @(r0,r12),r2 */
L8c10f4aa: *stop=0x8c10f4aau;r[4]+=1u;goto L8c10f4ac; /* add #0x1,r4 */
L8c10f4ac: *stop=0x8c10f4acu;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[2],1);if(io_status)return io_status;goto L8c10f4ae; /* mov.b r2,@r13 */
L8c10f4ae: *stop=0x8c10f4aeu;r[5]+=1u;goto L8c10f4b0; /* add #0x1,r5 */
L8c10f4b0: *stop=0x8c10f4b0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f4b2; /* mov.l @r15,r3 */
L8c10f4b2: *stop=0x8c10f4b2u;r[13]+=1u;goto L8c10f4b4; /* add #0x1,r13 */
L8c10f4b4: *stop=0x8c10f4b4u;r[3]+=4294967295u;goto L8c10f4b6; /* add #-0x1,r3 */
L8c10f4b6: *stop=0x8c10f4b6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10f4b8; /* mov.l r3,@r15 */
L8c10f4b8: *stop=0x8c10f4b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f4ba; /* mov.l @r15,r3 */
L8c10f4ba: *stop=0x8c10f4bau;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c10f4bc; /* cmp/pl r3 */
L8c10f4bc: if((*sr&1u))goto L8c10f4a6;goto L8c10f4be; /* bt 0x8c10f4a6 */
L8c10f4be: *stop=0x8c10f4beu;r[3]=46u;goto L8c10f4c0; /* mov #0x2e,r3 */
L8c10f4c0: *stop=0x8c10f4c0u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[3],1);if(io_status)return io_status;goto L8c10f4c2; /* mov.b r3,@r13 */
L8c10f4c2: *stop=0x8c10f4c4u;r[13]+=1u;goto L8c10f4d6; /* bra 0x8c10f4d6 */
L8c10f4c6: *stop=0x8c10f4c6u;r[0]=r[5];goto L8c10f4c8; /* mov r5,r0 */
L8c10f4c8: *stop=0x8c10f4c8u;if(!valid_access(r[0]+r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12]+r[0],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10f4ca; /* mov.b @(r0,r12),r2 */
L8c10f4ca: *stop=0x8c10f4cau;r[5]+=1u;goto L8c10f4cc; /* add #0x1,r5 */
L8c10f4cc: *stop=0x8c10f4ccu;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[2],1);if(io_status)return io_status;goto L8c10f4ce; /* mov.b r2,@r13 */
L8c10f4ce: *stop=0x8c10f4ceu;r[13]+=1u;goto L8c10f4d0; /* add #0x1,r13 */
L8c10f4d0: *stop=0x8c10f4d0u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f4d2; /* mov.l @(0x8,r14),r3 */
L8c10f4d2: *stop=0x8c10f4d2u;r[3]+=4294967295u;goto L8c10f4d4; /* add #-0x1,r3 */
L8c10f4d4: *stop=0x8c10f4d4u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10f4d6; /* mov.l r3,@(0x8,r14) */
L8c10f4d6: *stop=0x8c10f4d6u;r[2]=r[5];goto L8c10f4d8; /* mov r5,r2 */
L8c10f4d8: *stop=0x8c10f4d8u;r[2]-=r[6];goto L8c10f4da; /* sub r6,r2 */
L8c10f4da: *stop=0x8c10f4dau;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[10]);goto L8c10f4dc; /* cmp/ge r10,r2 */
L8c10f4dc: take=(*sr&1u);*stop=0x8c10f4deu;r[4]+=1u;if(take)goto L8c10f4e6;goto L8c10f4e0; /* bt/s 0x8c10f4e6 */
L8c10f4e0: *stop=0x8c10f4e0u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f4e2; /* mov.l @(0x8,r14),r2 */
L8c10f4e2: *stop=0x8c10f4e2u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c10f4e4; /* cmp/pl r2 */
L8c10f4e4: if((*sr&1u))goto L8c10f4c6;goto L8c10f4e6; /* bt 0x8c10f4c6 */
L8c10f4e6: *stop=0x8c10f4e6u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[5]=loaded;}goto L8c10f4e8; /* mov.b @r14,r5 */
L8c10f4e8: *stop=0x8c10f4e8u;r[2]=32u;goto L8c10f4ea; /* mov #0x20,r2 */
L8c10f4ea: *stop=0x8c10f4eau;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f4ec; /* tst r5,r2 */
L8c10f4ec: if((*sr&1u))goto L8c10f4fa;goto L8c10f4ee; /* bt 0x8c10f4fa */
L8c10f4ee: *stop=0x8c10f4eeu;r[1]=32u;goto L8c10f4f0; /* mov #0x20,r1 */
L8c10f4f0: *stop=0x8c10f4f0u;*sr=(*sr&~1u)|!(r[5]&r[1]);goto L8c10f4f2; /* tst r5,r1 */
L8c10f4f2: if((*sr&1u))goto L8c10f506;goto L8c10f4f4; /* bt 0x8c10f506 */
L8c10f4f4: *stop=0x8c10f4f4u;r[2]=4u;goto L8c10f4f6; /* mov #0x4,r2 */
L8c10f4f6: *stop=0x8c10f4f6u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f4f8; /* tst r5,r2 */
L8c10f4f8: if((*sr&1u))goto L8c10f506;goto L8c10f4fa; /* bt 0x8c10f506 */
L8c10f4fa: *stop=0x8c10f4fau;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f4fc; /* mov.l @(0x8,r14),r1 */
L8c10f4fc: *stop=0x8c10f4fcu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[13],4);if(io_status)return io_status;goto L8c10f4fe; /* mov.l r13,@(0x14,r14) */
L8c10f4fe: *stop=0x8c10f4feu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f500; /* mov.l @(0x8,r14),r3 */
L8c10f500: *stop=0x8c10f500u;r[4]+=r[1];goto L8c10f502; /* add r1,r4 */
L8c10f502: *stop=0x8c10f504u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[3],4);if(io_status)return io_status;goto L8c10f522; /* bra 0x8c10f522 */
L8c10f506: *stop=0x8c10f506u;r[0]=r[13];goto L8c10f508; /* mov r13,r0 */
L8c10f508: *stop=0x8c10f508u;r[0]+=4294967295u;goto L8c10f50a; /* add #-0x1,r0 */
L8c10f50a: *stop=0x8c10f50au;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f50c; /* mov.b @r0,r0 */
L8c10f50c: *stop=0x8c10f50cu;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f50e; /* cmp/eq #0x30,r0 */
L8c10f50e: if(!(*sr&1u))goto L8c10f522;goto L8c10f510; /* bf 0x8c10f522 */
L8c10f510: *stop=0x8c10f512u;goto L8c10f518; /* bra 0x8c10f518 */
L8c10f514: *stop=0x8c10f514u;r[4]+=4294967295u;goto L8c10f516; /* add #-0x1,r4 */
L8c10f516: *stop=0x8c10f516u;r[13]+=4294967295u;goto L8c10f518; /* add #-0x1,r13 */
L8c10f518: *stop=0x8c10f518u;r[0]=r[13];goto L8c10f51a; /* mov r13,r0 */
L8c10f51a: *stop=0x8c10f51au;r[0]+=4294967295u;goto L8c10f51c; /* add #-0x1,r0 */
L8c10f51c: *stop=0x8c10f51cu;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f51e; /* mov.b @r0,r0 */
L8c10f51e: *stop=0x8c10f51eu;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f520; /* cmp/eq #0x30,r0 */
L8c10f520: if((*sr&1u))goto L8c10f514;goto L8c10f522; /* bt 0x8c10f514 */
L8c10f522: *stop=0x8c10f522u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[5]=loaded;}goto L8c10f524; /* mov.b @r14,r5 */
L8c10f524: *stop=0x8c10f524u;r[2]=4u;goto L8c10f526; /* mov #0x4,r2 */
L8c10f526: *stop=0x8c10f526u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f528; /* tst r5,r2 */
L8c10f528: if(!(*sr&1u))goto L8c10f550;goto L8c10f52a; /* bf 0x8c10f550 */
L8c10f52a: *stop=0x8c10f52au;r[0]=r[13];goto L8c10f52c; /* mov r13,r0 */
L8c10f52c: *stop=0x8c10f52cu;r[0]+=4294967295u;goto L8c10f52e; /* add #-0x1,r0 */
L8c10f52e: *stop=0x8c10f52eu;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f530; /* mov.b @r0,r0 */
L8c10f530: *stop=0x8c10f530u;*sr=(*sr&~1u)|(r[0]==0x0000002eu);goto L8c10f532; /* cmp/eq #0x2e,r0 */
L8c10f532: if(!(*sr&1u))goto L8c10f550;goto L8c10f534; /* bf 0x8c10f550 */
L8c10f534: *stop=0x8c10f534u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f536; /* mov.l @(0x20,r14),r2 */
L8c10f536: *stop=0x8c10f536u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c10f538; /* tst r2,r2 */
L8c10f538: if((*sr&1u))goto L8c10f540;goto L8c10f53a; /* bt 0x8c10f540 */
L8c10f53a: *stop=0x8c10f53au;r[3]=32u;goto L8c10f53c; /* mov #0x20,r3 */
L8c10f53c: *stop=0x8c10f53cu;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c10f53e; /* tst r5,r3 */
L8c10f53e: if((*sr&1u))goto L8c10f550;goto L8c10f540; /* bt 0x8c10f550 */
L8c10f540: *stop=0x8c10f540u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f542; /* mov.l @(0x20,r14),r3 */
L8c10f542: *stop=0x8c10f542u;r[2]=r[4];goto L8c10f544; /* mov r4,r2 */
L8c10f544: *stop=0x8c10f544u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[11],4);if(io_status)return io_status;goto L8c10f546; /* mov.l r11,@(0x20,r14) */
L8c10f546: *stop=0x8c10f546u;r[13]+=4294967295u;goto L8c10f548; /* add #-0x1,r13 */
L8c10f548: *stop=0x8c10f548u;r[2]-=r[3];goto L8c10f54a; /* sub r3,r2 */
L8c10f54a: *stop=0x8c10f54au;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[9],4);if(io_status)return io_status;goto L8c10f54c; /* mov.l r9,@(0x14,r14) */
L8c10f54c: *stop=0x8c10f54cu;r[4]=r[2];goto L8c10f54e; /* mov r2,r4 */
L8c10f54e: *stop=0x8c10f54eu;r[4]+=4294967295u;goto L8c10f550; /* add #-0x1,r4 */
L8c10f550: *stop=0x8c10f550u;if(!valid_access(r[9],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f552; /* mov.b @r9,r0 */
L8c10f552: *stop=0x8c10f552u;*sr=(*sr&~1u)|(r[0]==0x0000002bu);goto L8c10f554; /* cmp/eq #0x2b,r0 */
L8c10f554: take=(*sr&1u);*stop=0x8c10f556u;r[5]=r[0];if(take)goto L8c10f564;goto L8c10f558; /* bt/s 0x8c10f564 */
L8c10f558: *stop=0x8c10f558u;r[0]=r[5];goto L8c10f55a; /* mov r5,r0 */
L8c10f55a: *stop=0x8c10f55au;*sr=(*sr&~1u)|(r[0]==0x0000002du);goto L8c10f55c; /* cmp/eq #0x2d,r0 */
L8c10f55c: if((*sr&1u))goto L8c10f564;goto L8c10f55e; /* bt 0x8c10f564 */
L8c10f55e: *stop=0x8c10f55eu;r[0]=r[5];goto L8c10f560; /* mov r5,r0 */
L8c10f560: *stop=0x8c10f560u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c10f562; /* cmp/eq #0x20,r0 */
L8c10f562: if(!(*sr&1u))goto L8c10f57e;goto L8c10f564; /* bf 0x8c10f57e */
L8c10f564: *stop=0x8c10f564u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f566; /* mov.l @(0x4,r14),r2 */
L8c10f566: *stop=0x8c10f566u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[4]);goto L8c10f568; /* cmp/gt r4,r2 */
L8c10f568: if(!(*sr&1u))goto L8c10f57e;goto L8c10f56a; /* bf 0x8c10f57e */
L8c10f56a: *stop=0x8c10f56au;if(!valid_access(r[14]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f56c; /* mov.b @(0x1,r14),r0 */
L8c10f56c: *stop=0x8c10f56cu;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f56e; /* cmp/eq #0x30,r0 */
L8c10f56e: if(!(*sr&1u))goto L8c10f57e;goto L8c10f570; /* bf 0x8c10f57e */
L8c10f570: *stop=0x8c10f570u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f572; /* mov.l @(0x4,r15),r2 */
L8c10f572: *stop=0x8c10f572u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c10f574; /* mov.l r2,@(0x10,r14) */
L8c10f574: *stop=0x8c10f574u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f576; /* mov.l @(0x4,r14),r3 */
L8c10f576: *stop=0x8c10f576u;r[3]-=r[4];goto L8c10f578; /* sub r4,r3 */
L8c10f578: *stop=0x8c10f578u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c10f57a; /* mov.l r3,@(0x1c,r14) */
L8c10f57a: *stop=0x8c10f57cu;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[11],4);if(io_status)return io_status;goto L8c10f592; /* bra 0x8c10f592 */
L8c10f57e: *stop=0x8c10f57eu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f580; /* mov.l @(0x4,r14),r3 */
L8c10f580: *stop=0x8c10f580u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[4]);goto L8c10f582; /* cmp/gt r4,r3 */
L8c10f582: if(!(*sr&1u))goto L8c10f590;goto L8c10f584; /* bf 0x8c10f590 */
L8c10f584: *stop=0x8c10f584u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f586; /* mov.l @(0x4,r14),r1 */
L8c10f586: *stop=0x8c10f586u;r[1]-=r[4];goto L8c10f588; /* sub r4,r1 */
L8c10f588: *stop=0x8c10f58au;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[1],4);if(io_status)return io_status;goto L8c10f592; /* bra 0x8c10f592 */
L8c10f590: *stop=0x8c10f590u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[11],4);if(io_status)return io_status;goto L8c10f592; /* mov.l r11,@(0x28,r14) */
L8c10f592: *stop=0x8c10f592u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f594; /* mov.l @(0x14,r14),r2 */
L8c10f594: *stop=0x8c10f594u;*sr=(*sr&~1u)|(r[9]==r[2]);goto L8c10f596; /* cmp/eq r9,r2 */
L8c10f596: if((*sr&1u))goto L8c10f59c;goto L8c10f598; /* bt 0x8c10f59c */
L8c10f598: *stop=0x8c10f59au;goto L8c10f88c; /* bra 0x8c10f88c */
L8c10f59c: *stop=0x8c10f59cu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f59e; /* mov.l @(0x10,r14),r1 */
L8c10f59e: *stop=0x8c10f5a0u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[1],4);if(io_status)return io_status;goto L8c10f88c; /* bra 0x8c10f88c */
L8c10f5a2: *stop=0x8c10f5a2u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f5a4; /* mov.l @(0xc,r15),r0 */
L8c10f5a4: *stop=0x8c10f5a4u;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f5a6; /* mov.b @r0,r0 */
L8c10f5a6: *stop=0x8c10f5a6u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f5a8; /* cmp/eq #0x30,r0 */
L8c10f5a8: if(!(*sr&1u))goto L8c10f5b0;goto L8c10f5aa; /* bf 0x8c10f5b0 */
L8c10f5aa: *stop=0x8c10f5aau;r[5]=r[8];goto L8c10f5ac; /* mov r8,r5 */
L8c10f5ac: *stop=0x8c10f5aeu;r[6]=r[8];goto L8c10f5bc; /* bra 0x8c10f5bc */
L8c10f5b0: *stop=0x8c10f5b0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f5b2; /* mov.l @r15,r3 */
L8c10f5b2: *stop=0x8c10f5b2u;r[10]+=1u;goto L8c10f5b4; /* add #0x1,r10 */
L8c10f5b4: *stop=0x8c10f5b4u;r[5]=r[11];goto L8c10f5b6; /* mov r11,r5 */
L8c10f5b6: *stop=0x8c10f5b6u;r[6]=r[11];goto L8c10f5b8; /* mov r11,r6 */
L8c10f5b8: *stop=0x8c10f5b8u;r[3]+=1u;goto L8c10f5ba; /* add #0x1,r3 */
L8c10f5ba: *stop=0x8c10f5bau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10f5bc; /* mov.l r3,@r15 */
L8c10f5bc: *stop=0x8c10f5bcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f5be; /* mov.l @r15,r3 */
L8c10f5be: *stop=0x8c10f5beu;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c10f5c0; /* cmp/pl r3 */
L8c10f5c0: if(!(*sr&1u))goto L8c10f5cc;goto L8c10f5c2; /* bf 0x8c10f5cc */
L8c10f5c2: *stop=0x8c10f5c2u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f5c4; /* mov.l @(0xc,r15),r2 */
L8c10f5c4: *stop=0x8c10f5c4u;r[5]=r[8];goto L8c10f5c6; /* mov r8,r5 */
L8c10f5c6: *stop=0x8c10f5c6u;if(!valid_access(r[2],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;r[1]=loaded;}goto L8c10f5c8; /* mov.b @r2,r1 */
L8c10f5c8: *stop=0x8c10f5cau;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[1],1);if(io_status)return io_status;goto L8c10f5d0; /* bra 0x8c10f5d0 */
L8c10f5cc: *stop=0x8c10f5ccu;r[3]=48u;goto L8c10f5ce; /* mov #0x30,r3 */
L8c10f5ce: *stop=0x8c10f5ceu;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[3],1);if(io_status)return io_status;goto L8c10f5d0; /* mov.b r3,@r13 */
L8c10f5d0: *stop=0x8c10f5d0u;r[3]=46u;goto L8c10f5d2; /* mov #0x2e,r3 */
L8c10f5d2: *stop=0x8c10f5d2u;r[4]=r[8];goto L8c10f5d4; /* mov r8,r4 */
L8c10f5d4: *stop=0x8c10f5d4u;r[13]+=1u;goto L8c10f5d6; /* add #0x1,r13 */
L8c10f5d6: *stop=0x8c10f5d6u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[3],1);if(io_status)return io_status;goto L8c10f5d8; /* mov.b r3,@r13 */
L8c10f5d8: *stop=0x8c10f5d8u;r[4]+=1u;goto L8c10f5da; /* add #0x1,r4 */
L8c10f5da: *stop=0x8c10f5dau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f5dc; /* mov.l @r15,r2 */
L8c10f5dc: *stop=0x8c10f5dcu;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c10f5de; /* cmp/pz r2 */
L8c10f5de: take=(*sr&1u);*stop=0x8c10f5e0u;r[13]+=1u;if(take)goto L8c10f61e;goto L8c10f5e2; /* bt/s 0x8c10f61e */
L8c10f5e2: *stop=0x8c10f5e2u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[13],4);if(io_status)return io_status;goto L8c10f5e4; /* mov.l r13,@(0x14,r14) */
L8c10f5e4: *stop=0x8c10f5e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f5e6; /* mov.l @r15,r2 */
L8c10f5e6: *stop=0x8c10f5e6u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f5e8; /* mov.l @(0x8,r14),r3 */
L8c10f5e8: *stop=0x8c10f5e8u;r[2]=0u-r[2];goto L8c10f5ea; /* neg r2,r2 */
L8c10f5ea: *stop=0x8c10f5eau;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[2]);goto L8c10f5ec; /* cmp/gt r2,r3 */
L8c10f5ec: if(!(*sr&1u))goto L8c10f5fa;goto L8c10f5ee; /* bf 0x8c10f5fa */
L8c10f5ee: *stop=0x8c10f5eeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f5f0; /* mov.l @r15,r1 */
L8c10f5f0: *stop=0x8c10f5f0u;r[1]=0u-r[1];goto L8c10f5f2; /* neg r1,r1 */
L8c10f5f2: *stop=0x8c10f5f2u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[1],4);if(io_status)return io_status;goto L8c10f5f4; /* mov.l r1,@(0x20,r14) */
L8c10f5f4: *stop=0x8c10f5f4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f5f6; /* mov.l @r15,r3 */
L8c10f5f6: *stop=0x8c10f5f8u;r[4]-=r[3];goto L8c10f602; /* bra 0x8c10f602 */
L8c10f5fa: *stop=0x8c10f5fau;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f5fc; /* mov.l @(0x8,r14),r1 */
L8c10f5fc: *stop=0x8c10f5fcu;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[1],4);if(io_status)return io_status;goto L8c10f5fe; /* mov.l r1,@(0x20,r14) */
L8c10f5fe: *stop=0x8c10f5feu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f600; /* mov.l @(0x8,r14),r3 */
L8c10f600: *stop=0x8c10f600u;r[4]+=r[3];goto L8c10f602; /* add r3,r4 */
L8c10f602: *stop=0x8c10f602u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f604; /* mov.l @(0x8,r14),r2 */
L8c10f604: *stop=0x8c10f604u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f606; /* mov.l @r15,r3 */
L8c10f606: *stop=0x8c10f606u;r[2]+=r[3];goto L8c10f608; /* add r3,r2 */
L8c10f608: *stop=0x8c10f60au;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c10f61e; /* bra 0x8c10f61e */
L8c10f60c: *stop=0x8c10f60cu;r[0]=r[5];goto L8c10f60e; /* mov r5,r0 */
L8c10f60e: *stop=0x8c10f60eu;if(!valid_access(r[0]+r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12]+r[0],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10f610; /* mov.b @(r0,r12),r2 */
L8c10f610: *stop=0x8c10f610u;r[4]+=1u;goto L8c10f612; /* add #0x1,r4 */
L8c10f612: *stop=0x8c10f612u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[2],1);if(io_status)return io_status;goto L8c10f614; /* mov.b r2,@r13 */
L8c10f614: *stop=0x8c10f614u;r[5]+=1u;goto L8c10f616; /* add #0x1,r5 */
L8c10f616: *stop=0x8c10f616u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f618; /* mov.l @(0x8,r14),r3 */
L8c10f618: *stop=0x8c10f618u;r[13]+=1u;goto L8c10f61a; /* add #0x1,r13 */
L8c10f61a: *stop=0x8c10f61au;r[3]+=4294967295u;goto L8c10f61c; /* add #-0x1,r3 */
L8c10f61c: *stop=0x8c10f61cu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10f61e; /* mov.l r3,@(0x8,r14) */
L8c10f61e: *stop=0x8c10f61eu;r[2]=r[5];goto L8c10f620; /* mov r5,r2 */
L8c10f620: *stop=0x8c10f620u;r[2]-=r[6];goto L8c10f622; /* sub r6,r2 */
L8c10f622: *stop=0x8c10f622u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[10]);goto L8c10f624; /* cmp/ge r10,r2 */
L8c10f624: if((*sr&1u))goto L8c10f62c;goto L8c10f626; /* bt 0x8c10f62c */
L8c10f626: *stop=0x8c10f626u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f628; /* mov.l @(0x8,r14),r2 */
L8c10f628: *stop=0x8c10f628u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c10f62a; /* cmp/pl r2 */
L8c10f62a: if((*sr&1u))goto L8c10f60c;goto L8c10f62c; /* bt 0x8c10f60c */
L8c10f62c: *stop=0x8c10f62cu;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[5]=loaded;}goto L8c10f62e; /* mov.b @r14,r5 */
L8c10f62e: *stop=0x8c10f62eu;r[2]=32u;goto L8c10f630; /* mov #0x20,r2 */
L8c10f630: *stop=0x8c10f630u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f632; /* tst r5,r2 */
L8c10f632: if((*sr&1u))goto L8c10f640;goto L8c10f634; /* bt 0x8c10f640 */
L8c10f634: *stop=0x8c10f634u;r[1]=32u;goto L8c10f636; /* mov #0x20,r1 */
L8c10f636: *stop=0x8c10f636u;*sr=(*sr&~1u)|!(r[5]&r[1]);goto L8c10f638; /* tst r5,r1 */
L8c10f638: if((*sr&1u))goto L8c10f652;goto L8c10f63a; /* bt 0x8c10f652 */
L8c10f63a: *stop=0x8c10f63au;r[2]=4u;goto L8c10f63c; /* mov #0x4,r2 */
L8c10f63c: *stop=0x8c10f63cu;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f63e; /* tst r5,r2 */
L8c10f63e: if((*sr&1u))goto L8c10f652;goto L8c10f640; /* bt 0x8c10f652 */
L8c10f640: *stop=0x8c10f640u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f642; /* mov.l @(0x8,r14),r1 */
L8c10f642: *stop=0x8c10f642u;*sr=(*sr&~1u)|((int32_t)r[1]>0);goto L8c10f644; /* cmp/pl r1 */
L8c10f644: if(!(*sr&1u))goto L8c10f66e;goto L8c10f646; /* bf 0x8c10f66e */
L8c10f646: *stop=0x8c10f646u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[13],4);if(io_status)return io_status;goto L8c10f648; /* mov.l r13,@(0x18,r14) */
L8c10f648: *stop=0x8c10f648u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f64a; /* mov.l @(0x8,r14),r2 */
L8c10f64a: *stop=0x8c10f64au;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[2],4);if(io_status)return io_status;goto L8c10f64c; /* mov.l r2,@(0x24,r14) */
L8c10f64c: *stop=0x8c10f64cu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f64e; /* mov.l @(0x8,r14),r3 */
L8c10f64e: *stop=0x8c10f650u;r[4]+=r[3];goto L8c10f66e; /* bra 0x8c10f66e */
L8c10f652: *stop=0x8c10f652u;r[0]=r[13];goto L8c10f654; /* mov r13,r0 */
L8c10f654: *stop=0x8c10f654u;r[0]+=4294967295u;goto L8c10f656; /* add #-0x1,r0 */
L8c10f656: *stop=0x8c10f656u;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f658; /* mov.b @r0,r0 */
L8c10f658: *stop=0x8c10f658u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f65a; /* cmp/eq #0x30,r0 */
L8c10f65a: if(!(*sr&1u))goto L8c10f66e;goto L8c10f65c; /* bf 0x8c10f66e */
L8c10f65c: *stop=0x8c10f65eu;goto L8c10f664; /* bra 0x8c10f664 */
L8c10f660: *stop=0x8c10f660u;r[4]+=4294967295u;goto L8c10f662; /* add #-0x1,r4 */
L8c10f662: *stop=0x8c10f662u;r[13]+=4294967295u;goto L8c10f664; /* add #-0x1,r13 */
L8c10f664: *stop=0x8c10f664u;r[0]=r[13];goto L8c10f666; /* mov r13,r0 */
L8c10f666: *stop=0x8c10f666u;r[0]+=4294967295u;goto L8c10f668; /* add #-0x1,r0 */
L8c10f668: *stop=0x8c10f668u;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f66a; /* mov.b @r0,r0 */
L8c10f66a: *stop=0x8c10f66au;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f66c; /* cmp/eq #0x30,r0 */
L8c10f66c: if((*sr&1u))goto L8c10f660;goto L8c10f66e; /* bt 0x8c10f660 */
L8c10f66e: *stop=0x8c10f66eu;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[5]=loaded;}goto L8c10f670; /* mov.b @r14,r5 */
L8c10f670: *stop=0x8c10f670u;r[2]=4u;goto L8c10f672; /* mov #0x4,r2 */
L8c10f672: *stop=0x8c10f672u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f674; /* tst r5,r2 */
L8c10f674: if(!(*sr&1u))goto L8c10f6aa;goto L8c10f676; /* bf 0x8c10f6aa */
L8c10f676: *stop=0x8c10f676u;r[0]=r[13];goto L8c10f678; /* mov r13,r0 */
L8c10f678: *stop=0x8c10f678u;r[0]+=4294967295u;goto L8c10f67a; /* add #-0x1,r0 */
L8c10f67a: *stop=0x8c10f67au;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f67c; /* mov.b @r0,r0 */
L8c10f67c: *stop=0x8c10f67cu;*sr=(*sr&~1u)|(r[0]==0x0000002eu);goto L8c10f67e; /* cmp/eq #0x2e,r0 */
L8c10f67e: if(!(*sr&1u))goto L8c10f6aa;goto L8c10f680; /* bf 0x8c10f6aa */
L8c10f680: *stop=0x8c10f680u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f682; /* mov.l @(0x20,r14),r2 */
L8c10f682: *stop=0x8c10f682u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c10f684; /* tst r2,r2 */
L8c10f684: if(!(*sr&1u))goto L8c10f68c;goto L8c10f686; /* bf 0x8c10f68c */
L8c10f686: *stop=0x8c10f686u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f688; /* mov.l @(0x24,r14),r3 */
L8c10f688: *stop=0x8c10f688u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c10f68a; /* tst r3,r3 */
L8c10f68a: if((*sr&1u))goto L8c10f692;goto L8c10f68c; /* bt 0x8c10f692 */
L8c10f68c: *stop=0x8c10f68cu;r[3]=32u;goto L8c10f68e; /* mov #0x20,r3 */
L8c10f68e: *stop=0x8c10f68eu;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c10f690; /* tst r5,r3 */
L8c10f690: if((*sr&1u))goto L8c10f6aa;goto L8c10f692; /* bt 0x8c10f6aa */
L8c10f692: *stop=0x8c10f692u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f694; /* mov.l @(0x20,r14),r3 */
L8c10f694: *stop=0x8c10f694u;r[2]=r[4];goto L8c10f696; /* mov r4,r2 */
L8c10f696: *stop=0x8c10f696u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f698; /* mov.l @(0x24,r14),r1 */
L8c10f698: *stop=0x8c10f698u;r[13]+=4294967295u;goto L8c10f69a; /* add #-0x1,r13 */
L8c10f69a: *stop=0x8c10f69au;r[2]-=r[3];goto L8c10f69c; /* sub r3,r2 */
L8c10f69c: *stop=0x8c10f69cu;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[11],4);if(io_status)return io_status;goto L8c10f69e; /* mov.l r11,@(0x24,r14) */
L8c10f69e: *stop=0x8c10f69eu;r[2]-=r[1];goto L8c10f6a0; /* sub r1,r2 */
L8c10f6a0: *stop=0x8c10f6a0u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[11],4);if(io_status)return io_status;goto L8c10f6a2; /* mov.l r11,@(0x20,r14) */
L8c10f6a2: *stop=0x8c10f6a2u;r[4]=r[2];goto L8c10f6a4; /* mov r2,r4 */
L8c10f6a4: *stop=0x8c10f6a4u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[9],4);if(io_status)return io_status;goto L8c10f6a6; /* mov.l r9,@(0x14,r14) */
L8c10f6a6: *stop=0x8c10f6a6u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[11],4);if(io_status)return io_status;goto L8c10f6a8; /* mov.l r11,@(0x18,r14) */
L8c10f6a8: *stop=0x8c10f6a8u;r[4]+=4294967295u;goto L8c10f6aa; /* add #-0x1,r4 */
L8c10f6aa: *stop=0x8c10f6aau;if(!valid_access(r[9],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f6ac; /* mov.b @r9,r0 */
L8c10f6ac: *stop=0x8c10f6acu;*sr=(*sr&~1u)|(r[0]==0x0000002bu);goto L8c10f6ae; /* cmp/eq #0x2b,r0 */
L8c10f6ae: take=(*sr&1u);*stop=0x8c10f6b0u;r[5]=r[0];if(take)goto L8c10f6be;goto L8c10f6b2; /* bt/s 0x8c10f6be */
L8c10f6b2: *stop=0x8c10f6b2u;r[0]=r[5];goto L8c10f6b4; /* mov r5,r0 */
L8c10f6b4: *stop=0x8c10f6b4u;*sr=(*sr&~1u)|(r[0]==0x0000002du);goto L8c10f6b6; /* cmp/eq #0x2d,r0 */
L8c10f6b6: if((*sr&1u))goto L8c10f6be;goto L8c10f6b8; /* bt 0x8c10f6be */
L8c10f6b8: *stop=0x8c10f6b8u;r[0]=r[5];goto L8c10f6ba; /* mov r5,r0 */
L8c10f6ba: *stop=0x8c10f6bau;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c10f6bc; /* cmp/eq #0x20,r0 */
L8c10f6bc: if(!(*sr&1u))goto L8c10f6d8;goto L8c10f6be; /* bf 0x8c10f6d8 */
L8c10f6be: *stop=0x8c10f6beu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f6c0; /* mov.l @(0x4,r14),r2 */
L8c10f6c0: *stop=0x8c10f6c0u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[4]);goto L8c10f6c2; /* cmp/gt r4,r2 */
L8c10f6c2: if(!(*sr&1u))goto L8c10f6d8;goto L8c10f6c4; /* bf 0x8c10f6d8 */
L8c10f6c4: *stop=0x8c10f6c4u;if(!valid_access(r[14]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f6c6; /* mov.b @(0x1,r14),r0 */
L8c10f6c6: *stop=0x8c10f6c6u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f6c8; /* cmp/eq #0x30,r0 */
L8c10f6c8: if(!(*sr&1u))goto L8c10f6d8;goto L8c10f6ca; /* bf 0x8c10f6d8 */
L8c10f6ca: *stop=0x8c10f6cau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f6cc; /* mov.l @(0x4,r15),r2 */
L8c10f6cc: *stop=0x8c10f6ccu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c10f6ce; /* mov.l r2,@(0x10,r14) */
L8c10f6ce: *stop=0x8c10f6ceu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f6d0; /* mov.l @(0x4,r14),r3 */
L8c10f6d0: *stop=0x8c10f6d0u;r[3]-=r[4];goto L8c10f6d2; /* sub r4,r3 */
L8c10f6d2: *stop=0x8c10f6d2u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c10f6d4; /* mov.l r3,@(0x1c,r14) */
L8c10f6d4: *stop=0x8c10f6d6u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[11],4);if(io_status)return io_status;goto L8c10f6e8; /* bra 0x8c10f6e8 */
L8c10f6d8: *stop=0x8c10f6d8u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f6da; /* mov.l @(0x4,r14),r3 */
L8c10f6da: *stop=0x8c10f6dau;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[4]);goto L8c10f6dc; /* cmp/gt r4,r3 */
L8c10f6dc: if(!(*sr&1u))goto L8c10f6e6;goto L8c10f6de; /* bf 0x8c10f6e6 */
L8c10f6de: *stop=0x8c10f6deu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f6e0; /* mov.l @(0x4,r14),r1 */
L8c10f6e0: *stop=0x8c10f6e0u;r[1]-=r[4];goto L8c10f6e2; /* sub r4,r1 */
L8c10f6e2: *stop=0x8c10f6e4u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[1],4);if(io_status)return io_status;goto L8c10f6e8; /* bra 0x8c10f6e8 */
L8c10f6e6: *stop=0x8c10f6e6u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[11],4);if(io_status)return io_status;goto L8c10f6e8; /* mov.l r11,@(0x28,r14) */
L8c10f6e8: *stop=0x8c10f6e8u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f6ea; /* mov.l @(0x14,r14),r2 */
L8c10f6ea: *stop=0x8c10f6eau;*sr=(*sr&~1u)|(r[9]==r[2]);goto L8c10f6ec; /* cmp/eq r9,r2 */
L8c10f6ec: if((*sr&1u))goto L8c10f6f2;goto L8c10f6ee; /* bt 0x8c10f6f2 */
L8c10f6ee: *stop=0x8c10f6f0u;goto L8c10f88c; /* bra 0x8c10f88c */
L8c10f6f2: *stop=0x8c10f6f2u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f6f4; /* mov.l @(0x10,r14),r1 */
L8c10f6f4: *stop=0x8c10f6f6u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[1],4);if(io_status)return io_status;goto L8c10f88c; /* bra 0x8c10f88c */
L8c10f6f8: *stop=0x8c10f6f8u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f6fa; /* mov.l @(0x8,r14),r2 */
L8c10f6fa: *stop=0x8c10f6fau;r[2]+=1u;goto L8c10f6fc; /* add #0x1,r2 */
L8c10f6fc: *stop=0x8c10f6fcu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[10]);goto L8c10f6fe; /* cmp/ge r10,r2 */
L8c10f6fe: if((*sr&1u))goto L8c10f710;goto L8c10f700; /* bt 0x8c10f710 */
L8c10f700: *stop=0x8c10f700u;r[3]=f->fpscr;goto L8c10f702; /* sts FPSCR,r3 */
L8c10f702: *stop=0x8c10f702u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f794u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f704; /* mov.l 0x8c10f794,r2 */
L8c10f704: *stop=0x8c10f704u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10f706; /* mov.l @(0x8,r14),r5 */
L8c10f706: *stop=0x8c10f706u;r[3]&=r[2];goto L8c10f708; /* and r2,r3 */
L8c10f708: *stop=0x8c10f708u;f->fpscr=r[3];goto L8c10f70a; /* lds r3,fpscr */
L8c10f70a: *stop=0x8c10f70au;r[5]+=2u;goto L8c10f70c; /* add #0x2,r5 */
L8c10f70c: target=0x8c10f8a2u;*pr=0x8c10f710u;*stop=0x8c10f70eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f710; /* bsr 0x8c10f8a2 */
L8c10f710: *stop=0x8c10f710u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f712; /* mov.l @(0xc,r15),r0 */
L8c10f712: *stop=0x8c10f712u;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f714; /* mov.b @r0,r0 */
L8c10f714: *stop=0x8c10f714u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f716; /* cmp/eq #0x30,r0 */
L8c10f716: if(!(*sr&1u))goto L8c10f71c;goto L8c10f718; /* bf 0x8c10f71c */
L8c10f718: *stop=0x8c10f71au;r[6]=r[8];goto L8c10f726; /* bra 0x8c10f726 */
L8c10f71c: *stop=0x8c10f71cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f71e; /* mov.l @r15,r3 */
L8c10f71e: *stop=0x8c10f71eu;r[10]+=1u;goto L8c10f720; /* add #0x1,r10 */
L8c10f720: *stop=0x8c10f720u;r[6]=r[11];goto L8c10f722; /* mov r11,r6 */
L8c10f722: *stop=0x8c10f722u;r[3]+=4294967295u;goto L8c10f724; /* add #-0x1,r3 */
L8c10f724: *stop=0x8c10f724u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10f726; /* mov.l r3,@r15 */
L8c10f726: *stop=0x8c10f726u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f728; /* mov.l @r15,r2 */
L8c10f728: *stop=0x8c10f728u;r[0]=r[6];goto L8c10f72a; /* mov r6,r0 */
L8c10f72a: *stop=0x8c10f72au;r[3]=r[10];goto L8c10f72c; /* mov r10,r3 */
L8c10f72c: *stop=0x8c10f72cu;r[3]-=r[6];goto L8c10f72e; /* sub r6,r3 */
L8c10f72e: *stop=0x8c10f72eu;r[2]+=r[3];goto L8c10f730; /* add r3,r2 */
L8c10f730: *stop=0x8c10f730u;r[5]=r[0];goto L8c10f732; /* mov r0,r5 */
L8c10f732: *stop=0x8c10f732u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10f734; /* mov.l r2,@r15 */
L8c10f734: *stop=0x8c10f734u;r[2]=46u;goto L8c10f736; /* mov #0x2e,r2 */
L8c10f736: *stop=0x8c10f736u;if(!valid_access(r[0]+r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12]+r[0],1);if(io_status)return io_status;r[1]=loaded;}goto L8c10f738; /* mov.b @(r0,r12),r1 */
L8c10f738: *stop=0x8c10f738u;r[4]=r[8];goto L8c10f73a; /* mov r8,r4 */
L8c10f73a: *stop=0x8c10f73au;r[5]+=1u;goto L8c10f73c; /* add #0x1,r5 */
L8c10f73c: *stop=0x8c10f73cu;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[1],1);if(io_status)return io_status;goto L8c10f73e; /* mov.b r1,@r13 */
L8c10f73e: *stop=0x8c10f73eu;r[13]+=1u;goto L8c10f740; /* add #0x1,r13 */
L8c10f740: *stop=0x8c10f740u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[2],1);if(io_status)return io_status;goto L8c10f742; /* mov.b r2,@r13 */
L8c10f742: *stop=0x8c10f742u;r[4]+=1u;goto L8c10f744; /* add #0x1,r4 */
L8c10f744: *stop=0x8c10f746u;r[13]+=1u;goto L8c10f75a; /* bra 0x8c10f75a */
L8c10f748: *stop=0x8c10f748u;r[0]=r[5];goto L8c10f74a; /* mov r5,r0 */
L8c10f74a: *stop=0x8c10f74au;if(!valid_access(r[0]+r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12]+r[0],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10f74c; /* mov.b @(r0,r12),r2 */
L8c10f74c: *stop=0x8c10f74cu;r[4]+=1u;goto L8c10f74e; /* add #0x1,r4 */
L8c10f74e: *stop=0x8c10f74eu;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[2],1);if(io_status)return io_status;goto L8c10f750; /* mov.b r2,@r13 */
L8c10f750: *stop=0x8c10f750u;r[5]+=1u;goto L8c10f752; /* add #0x1,r5 */
L8c10f752: *stop=0x8c10f752u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f754; /* mov.l @(0x8,r14),r3 */
L8c10f754: *stop=0x8c10f754u;r[13]+=1u;goto L8c10f756; /* add #0x1,r13 */
L8c10f756: *stop=0x8c10f756u;r[3]+=4294967295u;goto L8c10f758; /* add #-0x1,r3 */
L8c10f758: *stop=0x8c10f758u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10f75a; /* mov.l r3,@(0x8,r14) */
L8c10f75a: *stop=0x8c10f75au;*sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[10]);goto L8c10f75c; /* cmp/gt r10,r5 */
L8c10f75c: if((*sr&1u))goto L8c10f764;goto L8c10f75e; /* bt 0x8c10f764 */
L8c10f75e: *stop=0x8c10f75eu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f760; /* mov.l @(0x8,r14),r3 */
L8c10f760: *stop=0x8c10f760u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c10f762; /* cmp/pl r3 */
L8c10f762: if((*sr&1u))goto L8c10f748;goto L8c10f764; /* bt 0x8c10f748 */
L8c10f764: *stop=0x8c10f764u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[5]=loaded;}goto L8c10f766; /* mov.b @r14,r5 */
L8c10f766: *stop=0x8c10f766u;r[3]=32u;goto L8c10f768; /* mov #0x20,r3 */
L8c10f768: *stop=0x8c10f768u;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c10f76a; /* tst r5,r3 */
L8c10f76a: if((*sr&1u))goto L8c10f778;goto L8c10f76c; /* bt 0x8c10f778 */
L8c10f76c: *stop=0x8c10f76cu;r[1]=32u;goto L8c10f76e; /* mov #0x20,r1 */
L8c10f76e: *stop=0x8c10f76eu;*sr=(*sr&~1u)|!(r[5]&r[1]);goto L8c10f770; /* tst r5,r1 */
L8c10f770: if((*sr&1u))goto L8c10f784;goto L8c10f772; /* bt 0x8c10f784 */
L8c10f772: *stop=0x8c10f772u;r[2]=4u;goto L8c10f774; /* mov #0x4,r2 */
L8c10f774: *stop=0x8c10f774u;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f776; /* tst r5,r2 */
L8c10f776: if((*sr&1u))goto L8c10f784;goto L8c10f778; /* bt 0x8c10f784 */
L8c10f778: *stop=0x8c10f778u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[13],4);if(io_status)return io_status;goto L8c10f77a; /* mov.l r13,@(0x14,r14) */
L8c10f77a: *stop=0x8c10f77au;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f77c; /* mov.l @(0x8,r14),r2 */
L8c10f77c: *stop=0x8c10f77cu;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[2],4);if(io_status)return io_status;goto L8c10f77e; /* mov.l r2,@(0x20,r14) */
L8c10f77e: *stop=0x8c10f77eu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f780; /* mov.l @(0x8,r14),r3 */
L8c10f780: *stop=0x8c10f782u;r[4]+=r[3];goto L8c10f7a6; /* bra 0x8c10f7a6 */
L8c10f784: *stop=0x8c10f784u;r[0]=r[13];goto L8c10f786; /* mov r13,r0 */
L8c10f786: *stop=0x8c10f786u;r[0]+=4294967295u;goto L8c10f788; /* add #-0x1,r0 */
L8c10f788: *stop=0x8c10f788u;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f78a; /* mov.b @r0,r0 */
L8c10f78a: *stop=0x8c10f78au;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f78c; /* cmp/eq #0x30,r0 */
L8c10f78c: if(!(*sr&1u))goto L8c10f7a6;goto L8c10f78e; /* bf 0x8c10f7a6 */
L8c10f78e: *stop=0x8c10f790u;goto L8c10f79c; /* bra 0x8c10f79c */
L8c10f798: *stop=0x8c10f798u;r[4]+=4294967295u;goto L8c10f79a; /* add #-0x1,r4 */
L8c10f79a: *stop=0x8c10f79au;r[13]+=4294967295u;goto L8c10f79c; /* add #-0x1,r13 */
L8c10f79c: *stop=0x8c10f79cu;r[0]=r[13];goto L8c10f79e; /* mov r13,r0 */
L8c10f79e: *stop=0x8c10f79eu;r[0]+=4294967295u;goto L8c10f7a0; /* add #-0x1,r0 */
L8c10f7a0: *stop=0x8c10f7a0u;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f7a2; /* mov.b @r0,r0 */
L8c10f7a2: *stop=0x8c10f7a2u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f7a4; /* cmp/eq #0x30,r0 */
L8c10f7a4: if((*sr&1u))goto L8c10f798;goto L8c10f7a6; /* bt 0x8c10f798 */
L8c10f7a6: *stop=0x8c10f7a6u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[5]=loaded;}goto L8c10f7a8; /* mov.b @r14,r5 */
L8c10f7a8: *stop=0x8c10f7a8u;r[2]=4u;goto L8c10f7aa; /* mov #0x4,r2 */
L8c10f7aa: *stop=0x8c10f7aau;*sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c10f7ac; /* tst r5,r2 */
L8c10f7ac: if(!(*sr&1u))goto L8c10f7d4;goto L8c10f7ae; /* bf 0x8c10f7d4 */
L8c10f7ae: *stop=0x8c10f7aeu;r[0]=r[13];goto L8c10f7b0; /* mov r13,r0 */
L8c10f7b0: *stop=0x8c10f7b0u;r[0]+=4294967295u;goto L8c10f7b2; /* add #-0x1,r0 */
L8c10f7b2: *stop=0x8c10f7b2u;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f7b4; /* mov.b @r0,r0 */
L8c10f7b4: *stop=0x8c10f7b4u;*sr=(*sr&~1u)|(r[0]==0x0000002eu);goto L8c10f7b6; /* cmp/eq #0x2e,r0 */
L8c10f7b6: if(!(*sr&1u))goto L8c10f7d4;goto L8c10f7b8; /* bf 0x8c10f7d4 */
L8c10f7b8: *stop=0x8c10f7b8u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f7ba; /* mov.l @(0x20,r14),r2 */
L8c10f7ba: *stop=0x8c10f7bau;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c10f7bc; /* tst r2,r2 */
L8c10f7bc: if((*sr&1u))goto L8c10f7c4;goto L8c10f7be; /* bt 0x8c10f7c4 */
L8c10f7be: *stop=0x8c10f7beu;r[3]=32u;goto L8c10f7c0; /* mov #0x20,r3 */
L8c10f7c0: *stop=0x8c10f7c0u;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c10f7c2; /* tst r5,r3 */
L8c10f7c2: if((*sr&1u))goto L8c10f7d4;goto L8c10f7c4; /* bt 0x8c10f7d4 */
L8c10f7c4: *stop=0x8c10f7c4u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f7c6; /* mov.l @(0x20,r14),r3 */
L8c10f7c6: *stop=0x8c10f7c6u;r[2]=r[4];goto L8c10f7c8; /* mov r4,r2 */
L8c10f7c8: *stop=0x8c10f7c8u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[9],4);if(io_status)return io_status;goto L8c10f7ca; /* mov.l r9,@(0x14,r14) */
L8c10f7ca: *stop=0x8c10f7cau;r[13]+=4294967295u;goto L8c10f7cc; /* add #-0x1,r13 */
L8c10f7cc: *stop=0x8c10f7ccu;r[2]-=r[3];goto L8c10f7ce; /* sub r3,r2 */
L8c10f7ce: *stop=0x8c10f7ceu;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[11],4);if(io_status)return io_status;goto L8c10f7d0; /* mov.l r11,@(0x20,r14) */
L8c10f7d0: *stop=0x8c10f7d0u;r[4]=r[2];goto L8c10f7d2; /* mov r2,r4 */
L8c10f7d2: *stop=0x8c10f7d2u;r[4]+=4294967295u;goto L8c10f7d4; /* add #-0x1,r4 */
L8c10f7d4: *stop=0x8c10f7d4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10f7d6; /* mov.l @(0x8,r15),r0 */
L8c10f7d6: *stop=0x8c10f7d6u;*sr=(*sr&~1u)|(r[0]==0x00000065u);goto L8c10f7d8; /* cmp/eq #0x65,r0 */
L8c10f7d8: if(!(*sr&1u))goto L8c10f7de;goto L8c10f7da; /* bf 0x8c10f7de */
L8c10f7da: *stop=0x8c10f7dcu;r[3]=101u;goto L8c10f7e0; /* bra 0x8c10f7e0 */
L8c10f7de: *stop=0x8c10f7deu;r[3]=69u;goto L8c10f7e0; /* mov #0x45,r3 */
L8c10f7e0: *stop=0x8c10f7e0u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[3],1);if(io_status)return io_status;goto L8c10f7e2; /* mov.b r3,@r13 */
L8c10f7e2: *stop=0x8c10f7e2u;r[13]+=1u;goto L8c10f7e4; /* add #0x1,r13 */
L8c10f7e4: *stop=0x8c10f7e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f7e6; /* mov.l @r15,r2 */
L8c10f7e6: *stop=0x8c10f7e6u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c10f7e8; /* cmp/pz r2 */
L8c10f7e8: take=!(*sr&1u);*stop=0x8c10f7eau;r[4]+=1u;if(take)goto L8c10f7f4;goto L8c10f7ec; /* bf/s 0x8c10f7f4 */
L8c10f7ec: *stop=0x8c10f7ecu;r[3]=43u;goto L8c10f7ee; /* mov #0x2b,r3 */
L8c10f7ee: *stop=0x8c10f7eeu;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[3],1);if(io_status)return io_status;goto L8c10f7f0; /* mov.b r3,@r13 */
L8c10f7f0: *stop=0x8c10f7f2u;r[13]+=1u;goto L8c10f800; /* bra 0x8c10f800 */
L8c10f7f4: *stop=0x8c10f7f4u;r[3]=45u;goto L8c10f7f6; /* mov #0x2d,r3 */
L8c10f7f6: *stop=0x8c10f7f6u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[3],1);if(io_status)return io_status;goto L8c10f7f8; /* mov.b r3,@r13 */
L8c10f7f8: *stop=0x8c10f7f8u;r[13]+=1u;goto L8c10f7fa; /* add #0x1,r13 */
L8c10f7fa: *stop=0x8c10f7fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f7fc; /* mov.l @r15,r2 */
L8c10f7fc: *stop=0x8c10f7fcu;r[2]=0u-r[2];goto L8c10f7fe; /* neg r2,r2 */
L8c10f7fe: *stop=0x8c10f7feu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10f800; /* mov.l r2,@r15 */
L8c10f800: *stop=0x8c10f800u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f802; /* mov.l @r15,r2 */
L8c10f802: *stop=0x8c10f802u;r[3]=100u;goto L8c10f804; /* mov #0x64,r3 */
L8c10f804: *stop=0x8c10f804u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c10f806; /* cmp/ge r3,r2 */
L8c10f806: take=!(*sr&1u);*stop=0x8c10f808u;r[4]+=1u;if(take)goto L8c10f810;goto L8c10f80a; /* bf/s 0x8c10f810 */
L8c10f80a: *stop=0x8c10f80au;r[13]+=2u;goto L8c10f80c; /* add #0x2,r13 */
L8c10f80c: *stop=0x8c10f80eu;r[4]+=3u;goto L8c10f81c; /* bra 0x8c10f81c */
L8c10f810: *stop=0x8c10f810u;r[2]=48u;goto L8c10f812; /* mov #0x30,r2 */
L8c10f812: *stop=0x8c10f812u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[2],1);if(io_status)return io_status;goto L8c10f814; /* mov.b r2,@r13 */
L8c10f814: *stop=0x8c10f814u;r[1]=r[2];goto L8c10f816; /* mov r2,r1 */
L8c10f816: *stop=0x8c10f816u;r[13]+=1u;goto L8c10f818; /* add #0x1,r13 */
L8c10f818: *stop=0x8c10f818u;r[4]+=2u;goto L8c10f81a; /* add #0x2,r4 */
L8c10f81a: *stop=0x8c10f81au;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[1],1);if(io_status)return io_status;goto L8c10f81c; /* mov.b r1,@r13 */
L8c10f81c: *stop=0x8c10f81cu;r[6]=r[13];goto L8c10f81e; /* mov r13,r6 */
L8c10f81e: *stop=0x8c10f81eu;r[6]+=1u;goto L8c10f820; /* add #0x1,r6 */
L8c10f820: *stop=0x8c10f822u;r[5]=10u;goto L8c10f83c; /* bra 0x8c10f83c */
L8c10f824: *stop=0x8c10f824u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f904u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f826; /* mov.l 0x8c10f904,r3 */
L8c10f826: *stop=0x8c10f826u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f828; /* mov.l @r15,r1 */
L8c10f828: target=r[3];*pr=0x8c10f82cu;*stop=0x8c10f82au;r[0]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f82c; /* jsr @r3 */
L8c10f82c: *stop=0x8c10f82cu;r[0]+=48u;goto L8c10f82e; /* add #0x30,r0 */
L8c10f82e: *stop=0x8c10f82eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10f908u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f830; /* mov.l 0x8c10f908,r3 */
L8c10f830: *stop=0x8c10f830u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[0],1);if(io_status)return io_status;goto L8c10f832; /* mov.b r0,@r13 */
L8c10f832: *stop=0x8c10f832u;r[13]+=4294967295u;goto L8c10f834; /* add #-0x1,r13 */
L8c10f834: *stop=0x8c10f834u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f836; /* mov.l @r15,r1 */
L8c10f836: target=r[3];*pr=0x8c10f83au;*stop=0x8c10f838u;r[0]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f83a; /* jsr @r3 */
L8c10f83a: *stop=0x8c10f83au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c10f83c; /* mov.l r0,@r15 */
L8c10f83c: *stop=0x8c10f83cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f83e; /* mov.l @r15,r3 */
L8c10f83e: *stop=0x8c10f83eu;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c10f840; /* cmp/pl r3 */
L8c10f840: if((*sr&1u))goto L8c10f824;goto L8c10f842; /* bt 0x8c10f824 */
L8c10f842: *stop=0x8c10f842u;if(!valid_access(r[9],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f844; /* mov.b @r9,r0 */
L8c10f844: *stop=0x8c10f844u;r[13]=r[6];goto L8c10f846; /* mov r6,r13 */
L8c10f846: *stop=0x8c10f846u;*sr=(*sr&~1u)|(r[0]==0x0000002bu);goto L8c10f848; /* cmp/eq #0x2b,r0 */
L8c10f848: take=(*sr&1u);*stop=0x8c10f84au;r[5]=r[0];if(take)goto L8c10f858;goto L8c10f84c; /* bt/s 0x8c10f858 */
L8c10f84c: *stop=0x8c10f84cu;r[0]=r[5];goto L8c10f84e; /* mov r5,r0 */
L8c10f84e: *stop=0x8c10f84eu;*sr=(*sr&~1u)|(r[0]==0x0000002du);goto L8c10f850; /* cmp/eq #0x2d,r0 */
L8c10f850: if((*sr&1u))goto L8c10f858;goto L8c10f852; /* bt 0x8c10f858 */
L8c10f852: *stop=0x8c10f852u;r[0]=r[5];goto L8c10f854; /* mov r5,r0 */
L8c10f854: *stop=0x8c10f854u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c10f856; /* cmp/eq #0x20,r0 */
L8c10f856: if(!(*sr&1u))goto L8c10f872;goto L8c10f858; /* bf 0x8c10f872 */
L8c10f858: *stop=0x8c10f858u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f85a; /* mov.l @(0x4,r14),r2 */
L8c10f85a: *stop=0x8c10f85au;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[4]);goto L8c10f85c; /* cmp/gt r4,r2 */
L8c10f85c: if(!(*sr&1u))goto L8c10f872;goto L8c10f85e; /* bf 0x8c10f872 */
L8c10f85e: *stop=0x8c10f85eu;if(!valid_access(r[14]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c10f860; /* mov.b @(0x1,r14),r0 */
L8c10f860: *stop=0x8c10f860u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10f862; /* cmp/eq #0x30,r0 */
L8c10f862: if(!(*sr&1u))goto L8c10f872;goto L8c10f864; /* bf 0x8c10f872 */
L8c10f864: *stop=0x8c10f864u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f866; /* mov.l @(0x4,r15),r2 */
L8c10f866: *stop=0x8c10f866u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c10f868; /* mov.l r2,@(0x10,r14) */
L8c10f868: *stop=0x8c10f868u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f86a; /* mov.l @(0x4,r14),r3 */
L8c10f86a: *stop=0x8c10f86au;r[3]-=r[4];goto L8c10f86c; /* sub r4,r3 */
L8c10f86c: *stop=0x8c10f86cu;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c10f86e; /* mov.l r3,@(0x1c,r14) */
L8c10f86e: *stop=0x8c10f870u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[11],4);if(io_status)return io_status;goto L8c10f882; /* bra 0x8c10f882 */
L8c10f872: *stop=0x8c10f872u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10f874; /* mov.l @(0x4,r14),r3 */
L8c10f874: *stop=0x8c10f874u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[4]);goto L8c10f876; /* cmp/gt r4,r3 */
L8c10f876: if(!(*sr&1u))goto L8c10f880;goto L8c10f878; /* bf 0x8c10f880 */
L8c10f878: *stop=0x8c10f878u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f87a; /* mov.l @(0x4,r14),r1 */
L8c10f87a: *stop=0x8c10f87au;r[1]-=r[4];goto L8c10f87c; /* sub r4,r1 */
L8c10f87c: *stop=0x8c10f87eu;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[1],4);if(io_status)return io_status;goto L8c10f882; /* bra 0x8c10f882 */
L8c10f880: *stop=0x8c10f880u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[11],4);if(io_status)return io_status;goto L8c10f882; /* mov.l r11,@(0x28,r14) */
L8c10f882: *stop=0x8c10f882u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10f884; /* mov.l @(0x14,r14),r2 */
L8c10f884: *stop=0x8c10f884u;*sr=(*sr&~1u)|(r[9]==r[2]);goto L8c10f886; /* cmp/eq r9,r2 */
L8c10f886: if(!(*sr&1u))goto L8c10f88c;goto L8c10f888; /* bf 0x8c10f88c */
L8c10f888: *stop=0x8c10f888u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10f88a; /* mov.l @(0x10,r14),r1 */
L8c10f88a: *stop=0x8c10f88au;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[1],4);if(io_status)return io_status;goto L8c10f88c; /* mov.l r1,@(0x14,r14) */
L8c10f88c: *stop=0x8c10f88cu;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[11],1);if(io_status)return io_status;goto L8c10f88e; /* mov.b r11,@r13 */
L8c10f88e: *stop=0x8c10f88eu;r[15]+=64u;goto L8c10f890; /* add #0x40,r15 */
L8c10f890: *stop=0x8c10f890u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10f892; /* lds.l @r15+,PR */
L8c10f892: *stop=0x8c10f892u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c10f894; /* mov.l @r15+,r8 */
L8c10f894: *stop=0x8c10f894u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c10f896; /* mov.l @r15+,r9 */
L8c10f896: *stop=0x8c10f896u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10f898; /* mov.l @r15+,r10 */
L8c10f898: *stop=0x8c10f898u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10f89a; /* mov.l @r15+,r11 */
L8c10f89a: *stop=0x8c10f89au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10f89c; /* mov.l @r15+,r12 */
L8c10f89c: *stop=0x8c10f89cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10f89e; /* mov.l @r15+,r13 */
L8c10f89e: *stop=0x8c10f89eu;if(*pr!=entry_pr)return -3;*stop=0x8c10f8a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10e50e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10e50e;
L8c10e50e: *stop=0x8c10e50eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10e510; /* mov.l r14,@-r15 */
L8c10e510: *stop=0x8c10e510u;r[14]=r[6];goto L8c10e512; /* mov r6,r14 */
L8c10e512: *stop=0x8c10e512u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10e514; /* mov.l r13,@-r15 */
L8c10e514: *stop=0x8c10e514u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10e516; /* mov.l r12,@-r15 */
L8c10e516: *stop=0x8c10e516u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10e518; /* mov.l r11,@-r15 */
L8c10e518: *stop=0x8c10e518u;r[11]=r[4];goto L8c10e51a; /* mov r4,r11 */
L8c10e51a: *stop=0x8c10e51au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10e51c; /* sts.l PR,@-r15 */
L8c10e51c: *stop=0x8c10e51cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10e51e; /* mov.l @r14,r0 */
L8c10e51e: *stop=0x8c10e51eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10e520; /* cmp/eq #0x1,r0 */
L8c10e520: take=!(*sr&1u);*stop=0x8c10e522u;r[13]=r[5];if(take)goto L8c10e53c;goto L8c10e524; /* bf/s 0x8c10e53c */
L8c10e524: *stop=0x8c10e524u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10e58cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e526; /* mov.l 0x8c10e58c,r2 */
L8c10e526: *stop=0x8c10e526u;r[5]=r[11];goto L8c10e528; /* mov r11,r5 */
L8c10e528: *stop=0x8c10e528u;r[6]=r[13];goto L8c10e52a; /* mov r13,r6 */
L8c10e52a: target=r[2];*pr=0x8c10e52eu;*stop=0x8c10e52cu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e52e; /* jsr @r2 */
L8c10e52e: *stop=0x8c10e52eu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10e530; /* mov.l @(0x4,r14),r3 */
L8c10e530: *stop=0x8c10e530u;r[3]+=r[13];goto L8c10e532; /* add r13,r3 */
L8c10e532: *stop=0x8c10e532u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c10e534; /* mov.l r3,@(0x4,r14) */
L8c10e534: *stop=0x8c10e534u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e536; /* mov.l @(0x8,r14),r2 */
L8c10e536: *stop=0x8c10e536u;r[2]+=r[13];goto L8c10e538; /* add r13,r2 */
L8c10e538: *stop=0x8c10e53au;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c10e55a; /* bra 0x8c10e55a */
L8c10e53c: *stop=0x8c10e53cu;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c10e53e; /* cmp/pl r13 */
L8c10e53e: take=!(*sr&1u);*stop=0x8c10e540u;r[12]=0u;if(take)goto L8c10e55a;goto L8c10e542; /* bf/s 0x8c10e55a */
L8c10e542: *stop=0x8c10e542u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10e544; /* mov.l @(0x4,r14),r5 */
L8c10e544: *stop=0x8c10e544u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10e546; /* mov.l @(0xc,r14),r0 */
L8c10e546: target=r[0];*pr=0x8c10e54au;*stop=0x8c10e548u;if(!valid_access(r[11],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[11],1);if(io_status)return io_status;r[4]=loaded;}r[11]+=1;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e54a; /* jsr @r0 */
L8c10e54a: *stop=0x8c10e54au;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10e54c; /* cmp/eq #-0x1,r0 */
L8c10e54c: if((*sr&1u))goto L8c10e55a;goto L8c10e54e; /* bt 0x8c10e55a */
L8c10e54e: *stop=0x8c10e54eu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e550; /* mov.l @(0x8,r14),r2 */
L8c10e550: *stop=0x8c10e550u;r[12]+=1u;goto L8c10e552; /* add #0x1,r12 */
L8c10e552: *stop=0x8c10e552u;*sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[13]);goto L8c10e554; /* cmp/ge r13,r12 */
L8c10e554: *stop=0x8c10e554u;r[2]+=1u;goto L8c10e556; /* add #0x1,r2 */
L8c10e556: take=!(*sr&1u);*stop=0x8c10e558u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;if(take)goto L8c10e542;goto L8c10e55a; /* bf/s 0x8c10e542 */
L8c10e55a: *stop=0x8c10e55au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10e55c; /* lds.l @r15+,PR */
L8c10e55c: *stop=0x8c10e55cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10e55e; /* mov.l @r15+,r11 */
L8c10e55e: *stop=0x8c10e55eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10e560; /* mov.l @r15+,r12 */
L8c10e560: *stop=0x8c10e560u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10e562; /* mov.l @r15+,r13 */
L8c10e562: *stop=0x8c10e562u;if(*pr!=entry_pr)return -3;*stop=0x8c10e564u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10e566(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10e566;
L8c10e566: *stop=0x8c10e566u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10e568; /* mov.l @r4,r0 */
L8c10e568: *stop=0x8c10e568u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10e56a; /* cmp/eq #0x1,r0 */
L8c10e56a: if(!(*sr&1u))goto L8c10e572;goto L8c10e56c; /* bf 0x8c10e572 */
L8c10e56c: *stop=0x8c10e56cu;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10e56e; /* mov.l @(0x4,r4),r2 */
L8c10e56e: *stop=0x8c10e56eu;r[3]=0u;goto L8c10e570; /* mov #0x0,r3 */
L8c10e570: *stop=0x8c10e570u;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[3],1);if(io_status)return io_status;goto L8c10e572; /* mov.b r3,@r2 */
L8c10e572: *stop=0x8c10e572u;if(*pr!=entry_pr)return -3;*stop=0x8c10e574u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c600ec6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c600ec6;
L8c600ec6: *stop=0x8c600ec6u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c600ec8; /* tst r4,r4 */
L8c600ec8: if((*sr&1u))goto L8c600ed2;goto L8c600eca; /* bt 0x8c600ed2 */
L8c600eca: *stop=0x8c600ecau;r[0]=76u;goto L8c600ecc; /* mov #0x4c,r0 */
L8c600ecc: *stop=0x8c600eccu;if(!valid_access(r[0]+r[4],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0]+r[4],2);if(io_status)return io_status;r[3]=loaded;}goto L8c600ece; /* mov.w @(r0,r4),r3 */
L8c600ece: *stop=0x8c600eceu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c600ed0; /* tst r3,r3 */
L8c600ed0: if(!(*sr&1u))goto L8c600ed6;goto L8c600ed2; /* bf 0x8c600ed6 */
L8c600ed2: *stop=0x8c600ed2u;if(*pr!=entry_pr)return -3;*stop=0x8c600ed4u;r[0]=0u;return 0; /* rts */
L8c600ed6: *stop=0x8c600ed6u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c600ed8; /* mov.l @(0x14,r4),r0 */
L8c600ed8: *stop=0x8c600ed8u;if(*pr!=entry_pr)return -3;*stop=0x8c600edau;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10d25c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10d25c;
L8c10d25c: *stop=0x8c10d25cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10d25e; /* mov.l r13,@-r15 */
L8c10d25e: *stop=0x8c10d25eu;*sr=(*sr&~1u)|(r[5]==r[4]);goto L8c10d260; /* cmp/eq r5,r4 */
L8c10d260: *stop=0x8c10d260u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10d262; /* mov.l r12,@-r15 */
L8c10d262: *stop=0x8c10d262u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10d264; /* mov.l r11,@-r15 */
L8c10d264: if((*sr&1u))goto L8c10d310;goto L8c10d266; /* bt 0x8c10d310 */
L8c10d266: *stop=0x8c10d266u;r[2]=0u;goto L8c10d268; /* mov #0x0,r2 */
L8c10d268: *stop=0x8c10d268u;*sr=(*sr&~1u)|(r[6]>r[2]);goto L8c10d26a; /* cmp/hi r2,r6 */
L8c10d26a: if(!(*sr&1u))goto L8c10d310;goto L8c10d26c; /* bf 0x8c10d310 */
L8c10d26c: *stop=0x8c10d26cu;r[7]=r[4];goto L8c10d26e; /* mov r4,r7 */
L8c10d26e: *stop=0x8c10d26eu;r[11]=r[6]&65535u;goto L8c10d270; /* extu.w r6,r11 */
L8c10d270: *stop=0x8c10d270u;r[7]|=r[5];goto L8c10d272; /* or r5,r7 */
L8c10d272: *stop=0x8c10d272u;r[12]=r[6];goto L8c10d274; /* mov r6,r12 */
L8c10d274: *stop=0x8c10d274u;*sr=(*sr&~1u)|(r[11]&1u);r[11]>>=1;goto L8c10d276; /* shlr r11 */
L8c10d276: *stop=0x8c10d276u;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c10d278; /* cmp/hs r5,r4 */
L8c10d278: *stop=0x8c10d278u;r[7]|=r[6];goto L8c10d27a; /* or r6,r7 */
L8c10d27a: take=(*sr&1u);*stop=0x8c10d27cu;r[12]>>=2;if(take)goto L8c10d2c6;goto L8c10d27e; /* bt/s 0x8c10d2c6 */
L8c10d27e: *stop=0x8c10d27eu;r[3]=1u;goto L8c10d280; /* mov #0x1,r3 */
L8c10d280: *stop=0x8c10d280u;*sr=(*sr&~1u)|!(r[7]&r[3]);goto L8c10d282; /* tst r7,r3 */
L8c10d282: if(!(*sr&1u))goto L8c10d2b2;goto L8c10d284; /* bf 0x8c10d2b2 */
L8c10d284: *stop=0x8c10d284u;r[1]=3u;goto L8c10d286; /* mov #0x3,r1 */
L8c10d286: *stop=0x8c10d286u;*sr=(*sr&~1u)|!(r[1]&r[7]);goto L8c10d288; /* tst r1,r7 */
L8c10d288: if(!(*sr&1u))goto L8c10d29e;goto L8c10d28a; /* bf 0x8c10d29e */
L8c10d28a: *stop=0x8c10d28au;r[6]=r[12];goto L8c10d28c; /* mov r12,r6 */
L8c10d28c: *stop=0x8c10d28cu;r[7]=r[4];goto L8c10d28e; /* mov r4,r7 */
L8c10d28e: *stop=0x8c10d28eu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[3]=loaded;}r[5]+=4;goto L8c10d290; /* mov.l @r5+,r3 */
L8c10d290: *stop=0x8c10d290u;r[6]+=4294967295u;goto L8c10d292; /* add #-0x1,r6 */
L8c10d292: *stop=0x8c10d292u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c10d294; /* tst r6,r6 */
L8c10d294: *stop=0x8c10d294u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[3],4);if(io_status)return io_status;goto L8c10d296; /* mov.l r3,@r7 */
L8c10d296: take=!(*sr&1u);*stop=0x8c10d298u;r[7]+=4u;if(take)goto L8c10d28e;goto L8c10d29a; /* bf/s 0x8c10d28e */
L8c10d29a: *stop=0x8c10d29cu;goto L8c10d310; /* bra 0x8c10d310 */
L8c10d29e: *stop=0x8c10d29eu;r[6]=r[11];goto L8c10d2a0; /* mov r11,r6 */
L8c10d2a0: *stop=0x8c10d2a0u;r[7]=r[4];goto L8c10d2a2; /* mov r4,r7 */
L8c10d2a2: *stop=0x8c10d2a2u;if(!valid_access(r[5],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[3]=loaded;}r[5]+=2;goto L8c10d2a4; /* mov.w @r5+,r3 */
L8c10d2a4: *stop=0x8c10d2a4u;r[6]+=4294967295u;goto L8c10d2a6; /* add #-0x1,r6 */
L8c10d2a6: *stop=0x8c10d2a6u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c10d2a8; /* tst r6,r6 */
L8c10d2a8: *stop=0x8c10d2a8u;if(!valid_access(r[7],2u))return -3;wr(m,bus,&io_status,r[7],r[3],2);if(io_status)return io_status;goto L8c10d2aa; /* mov.w r3,@r7 */
L8c10d2aa: take=!(*sr&1u);*stop=0x8c10d2acu;r[7]+=2u;if(take)goto L8c10d2a2;goto L8c10d2ae; /* bf/s 0x8c10d2a2 */
L8c10d2ae: *stop=0x8c10d2b0u;goto L8c10d310; /* bra 0x8c10d310 */
L8c10d2b2: *stop=0x8c10d2b2u;r[0]=r[5];goto L8c10d2b4; /* mov r5,r0 */
L8c10d2b4: *stop=0x8c10d2b4u;r[7]=r[4];goto L8c10d2b6; /* mov r4,r7 */
L8c10d2b6: *stop=0x8c10d2b6u;if(!valid_access(r[0],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;r[3]=loaded;}r[0]+=1;goto L8c10d2b8; /* mov.b @r0+,r3 */
L8c10d2b8: *stop=0x8c10d2b8u;r[6]+=4294967295u;goto L8c10d2ba; /* add #-0x1,r6 */
L8c10d2ba: *stop=0x8c10d2bau;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c10d2bc; /* tst r6,r6 */
L8c10d2bc: *stop=0x8c10d2bcu;if(!valid_access(r[7],1u))return -3;wr(m,bus,&io_status,r[7],r[3],1);if(io_status)return io_status;goto L8c10d2be; /* mov.b r3,@r7 */
L8c10d2be: take=!(*sr&1u);*stop=0x8c10d2c0u;r[7]+=1u;if(take)goto L8c10d2b6;goto L8c10d2c2; /* bf/s 0x8c10d2b6 */
L8c10d2c2: *stop=0x8c10d2c4u;goto L8c10d310; /* bra 0x8c10d310 */
L8c10d2c6: *stop=0x8c10d2c6u;r[2]=1u;goto L8c10d2c8; /* mov #0x1,r2 */
L8c10d2c8: *stop=0x8c10d2c8u;r[0]=r[4];goto L8c10d2ca; /* mov r4,r0 */
L8c10d2ca: *stop=0x8c10d2cau;*sr=(*sr&~1u)|!(r[7]&r[2]);goto L8c10d2cc; /* tst r7,r2 */
L8c10d2cc: *stop=0x8c10d2ccu;r[13]=r[5];goto L8c10d2ce; /* mov r5,r13 */
L8c10d2ce: *stop=0x8c10d2ceu;r[0]+=r[6];goto L8c10d2d0; /* add r6,r0 */
L8c10d2d0: take=!(*sr&1u);*stop=0x8c10d2d2u;r[13]+=r[6];if(take)goto L8c10d302;goto L8c10d2d4; /* bf/s 0x8c10d302 */
L8c10d2d4: *stop=0x8c10d2d4u;r[1]=3u;goto L8c10d2d6; /* mov #0x3,r1 */
L8c10d2d6: *stop=0x8c10d2d6u;*sr=(*sr&~1u)|!(r[1]&r[7]);goto L8c10d2d8; /* tst r1,r7 */
L8c10d2d8: if(!(*sr&1u))goto L8c10d2ee;goto L8c10d2da; /* bf 0x8c10d2ee */
L8c10d2da: *stop=0x8c10d2dau;r[6]=r[12];goto L8c10d2dc; /* mov r12,r6 */
L8c10d2dc: *stop=0x8c10d2dcu;r[7]=r[13];goto L8c10d2de; /* mov r13,r7 */
L8c10d2de: *stop=0x8c10d2deu;r[5]=r[0];goto L8c10d2e0; /* mov r0,r5 */
L8c10d2e0: *stop=0x8c10d2e0u;r[7]+=4294967292u;goto L8c10d2e2; /* add #-0x4,r7 */
L8c10d2e2: *stop=0x8c10d2e2u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d2e4; /* mov.l @r7,r3 */
L8c10d2e4: *stop=0x8c10d2e4u;r[6]--;*sr=(*sr&~1u)|(!r[6]);goto L8c10d2e6; /* dt r6 */
L8c10d2e6: take=!(*sr&1u);*stop=0x8c10d2e8u;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;if(take)goto L8c10d2e0;goto L8c10d2ea; /* bf/s 0x8c10d2e0 */
L8c10d2ea: *stop=0x8c10d2ecu;goto L8c10d310; /* bra 0x8c10d310 */
L8c10d2ee: *stop=0x8c10d2eeu;r[6]=r[11];goto L8c10d2f0; /* mov r11,r6 */
L8c10d2f0: *stop=0x8c10d2f0u;r[5]=r[13];goto L8c10d2f2; /* mov r13,r5 */
L8c10d2f2: *stop=0x8c10d2f2u;r[7]=r[0];goto L8c10d2f4; /* mov r0,r7 */
L8c10d2f4: *stop=0x8c10d2f4u;r[5]+=4294967294u;goto L8c10d2f6; /* add #-0x2,r5 */
L8c10d2f6: *stop=0x8c10d2f6u;if(!valid_access(r[5],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d2f8; /* mov.w @r5,r3 */
L8c10d2f8: *stop=0x8c10d2f8u;r[6]--;*sr=(*sr&~1u)|(!r[6]);goto L8c10d2fa; /* dt r6 */
L8c10d2fa: take=!(*sr&1u);*stop=0x8c10d2fcu;if(!valid_access(r[7]-2u,2u))return -3;r[7]-=2;wr(m,bus,&io_status,r[7],r[3],2);if(io_status)return io_status;if(take)goto L8c10d2f4;goto L8c10d2fe; /* bf/s 0x8c10d2f4 */
L8c10d2fe: *stop=0x8c10d300u;goto L8c10d310; /* bra 0x8c10d310 */
L8c10d302: *stop=0x8c10d302u;r[7]=r[13];goto L8c10d304; /* mov r13,r7 */
L8c10d304: *stop=0x8c10d304u;r[5]=r[0];goto L8c10d306; /* mov r0,r5 */
L8c10d306: *stop=0x8c10d306u;r[7]+=4294967295u;goto L8c10d308; /* add #-0x1,r7 */
L8c10d308: *stop=0x8c10d308u;if(!valid_access(r[7],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7],1);if(io_status)return io_status;r[3]=loaded;}goto L8c10d30a; /* mov.b @r7,r3 */
L8c10d30a: *stop=0x8c10d30au;r[6]--;*sr=(*sr&~1u)|(!r[6]);goto L8c10d30c; /* dt r6 */
L8c10d30c: take=!(*sr&1u);*stop=0x8c10d30eu;if(!valid_access(r[5]-1u,1u))return -3;r[5]-=1;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;if(take)goto L8c10d306;goto L8c10d310; /* bf/s 0x8c10d306 */
L8c10d310: *stop=0x8c10d310u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10d312; /* mov.l @r15+,r11 */
L8c10d312: *stop=0x8c10d312u;r[0]=r[4];goto L8c10d314; /* mov r4,r0 */
L8c10d314: *stop=0x8c10d314u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10d316; /* mov.l @r15+,r12 */
L8c10d316: *stop=0x8c10d316u;if(*pr!=entry_pr)return -3;*stop=0x8c10d318u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10fedc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10fedc;
L8c10fedc: *stop=0x8c10fedcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10fede; /* mov.l r14,@-r15 */
L8c10fede: *stop=0x8c10fedeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10fee0; /* mov.l r13,@-r15 */
L8c10fee0: *stop=0x8c10fee0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10fee2; /* mov.l r12,@-r15 */
L8c10fee2: *stop=0x8c10fee2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10fee4; /* mov.l r11,@-r15 */
L8c10fee4: *stop=0x8c10fee4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10fee6; /* mov.l r10,@-r15 */
L8c10fee6: *stop=0x8c10fee6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10fee8; /* mov.l r9,@-r15 */
L8c10fee8: *stop=0x8c10fee8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10feea; /* mov.l r8,@-r15 */
L8c10feea: *stop=0x8c10feeau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10feec; /* sts.l PR,@-r15 */
L8c10feec: *stop=0x8c10feecu;r[15]+=4294967236u;goto L8c10feee; /* add #-0x3c,r15 */
L8c10feee: *stop=0x8c10feeeu;r[12]=r[15];goto L8c10fef0; /* mov r15,r12 */
L8c10fef0: *stop=0x8c10fef0u;r[14]=0u;goto L8c10fef2; /* mov #0x0,r14 */
L8c10fef2: *stop=0x8c10fef2u;r[12]+=52u;goto L8c10fef4; /* add #0x34,r12 */
L8c10fef4: *stop=0x8c10fef4u;r[0]=r[14];goto L8c10fef6; /* mov r14,r0 */
L8c10fef6: *stop=0x8c10fef6u;r[8]=r[12];goto L8c10fef8; /* mov r12,r8 */
L8c10fef8: *stop=0x8c10fef8u;r[8]+=7u;goto L8c10fefa; /* add #0x7,r8 */
L8c10fefa: *stop=0x8c10fefau;r[3]=r[15];goto L8c10fefc; /* mov r15,r3 */
L8c10fefc: *stop=0x8c10fefcu;r[3]+=52u;goto L8c10fefe; /* add #0x34,r3 */
L8c10fefe: *stop=0x8c10fefeu;r[2]=r[15];goto L8c10ff00; /* mov r15,r2 */
L8c10ff00: *stop=0x8c10ff00u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c10ff02; /* fmov.s fr5,@r3 */
L8c10ff02: *stop=0x8c10ff02u;r[2]+=56u;goto L8c10ff04; /* add #0x38,r2 */
L8c10ff04: *stop=0x8c10ff04u;r[9]=r[4];goto L8c10ff06; /* mov r4,r9 */
L8c10ff06: *stop=0x8c10ff06u;r[3]=8u;goto L8c10ff08; /* mov #0x8,r3 */
L8c10ff08: *stop=0x8c10ff08u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c10ff0a; /* fmov.s fr4,@r2 */
L8c10ff0a: *stop=0x8c10ff0au;r[11]=r[5];goto L8c10ff0c; /* mov r5,r11 */
L8c10ff0c: *stop=0x8c10ff0cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10ff0e; /* mov.l r3,@(0x8,r15) */
L8c10ff0e: *stop=0x8c10ff0eu;if(!valid_access(r[15]+16u,2u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[0],2);if(io_status)return io_status;goto L8c10ff10; /* mov.w r0,@(0x10,r15) */
L8c10ff10: *stop=0x8c10ff10u;r[10]=r[6];goto L8c10ff12; /* mov r6,r10 */
L8c10ff12: *stop=0x8c10ff12u;if(!valid_access(r[8],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[8],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10ff14; /* mov.b @r8,r0 */
L8c10ff14: *stop=0x8c10ff14u;r[0]=r[0]&255u;goto L8c10ff16; /* extu.b r0,r0 */
L8c10ff16: *stop=0x8c10ff16u;*sr=(*sr&~1u)|!(r[0]&0x7fu);goto L8c10ff18; /* tst #0x7f,r0 */
L8c10ff18: take=!(*sr&1u);*stop=0x8c10ff1au;r[13]=r[7];if(take)goto L8c10ff50;goto L8c10ff1c; /* bf/s 0x8c10ff50 */
L8c10ff1c: *stop=0x8c10ff1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ffc0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ff1e; /* mov.l 0x8c10ffc0,r2 */
L8c10ff1e: *stop=0x8c10ff1eu;r[5]=7u;goto L8c10ff20; /* mov #0x7,r5 */
L8c10ff20: target=r[2];*pr=0x8c10ff24u;*stop=0x8c10ff22u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10ff24; /* jsr @r2 */
L8c10ff24: *stop=0x8c10ff24u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10ff26; /* tst r0,r0 */
L8c10ff26: if((*sr&1u))goto L8c10ff50;goto L8c10ff28; /* bt 0x8c10ff50 */
L8c10ff28: *stop=0x8c10ff28u;r[5]=r[11];goto L8c10ff2a; /* mov r11,r5 */
L8c10ff2a: *stop=0x8c10ff2au;r[4]=r[11];goto L8c10ff2c; /* mov r11,r4 */
L8c10ff2c: *stop=0x8c10ff2eu;r[5]+=8u;goto L8c10ff34; /* bra 0x8c10ff34 */
L8c10ff30: *stop=0x8c10ff30u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[14],1);if(io_status)return io_status;goto L8c10ff32; /* mov.b r14,@r4 */
L8c10ff32: *stop=0x8c10ff32u;r[4]+=1u;goto L8c10ff34; /* add #0x1,r4 */
L8c10ff34: *stop=0x8c10ff34u;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c10ff36; /* cmp/hs r5,r4 */
L8c10ff36: if(!(*sr&1u))goto L8c10ff30;goto L8c10ff38; /* bf 0x8c10ff30 */
L8c10ff38: *stop=0x8c10ff38u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[14],4);if(io_status)return io_status;goto L8c10ff3a; /* mov.l r14,@r10 */
L8c10ff3a: *stop=0x8c10ff3au;if(!valid_access(r[8],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[8],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10ff3c; /* mov.b @r8,r0 */
L8c10ff3c: *stop=0x8c10ff3cu;r[0]=r[0]&255u;goto L8c10ff3e; /* extu.b r0,r0 */
L8c10ff3e: *stop=0x8c10ff3eu;*sr=(*sr&~1u)|!(r[0]&0x80u);goto L8c10ff40; /* tst #0x80,r0 */
L8c10ff40: if((*sr&1u))goto L8c10ff48;goto L8c10ff42; /* bt 0x8c10ff48 */
L8c10ff42: *stop=0x8c10ff42u;r[2]=4294967295u;goto L8c10ff44; /* mov #-0x1,r2 */
L8c10ff44: *stop=0x8c10ff46u;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[2],1);if(io_status)return io_status;goto L8c1101b8; /* bra 0x8c1101b8 */
L8c10ff48: *stop=0x8c10ff48u;r[1]=1u;goto L8c10ff4a; /* mov #0x1,r1 */
L8c10ff4a: *stop=0x8c10ff4au;if(!valid_access(r[13],1u))return -3;wr(m,bus,&io_status,r[13],r[1],1);if(io_status)return io_status;goto L8c10ff4c; /* mov.b r1,@r13 */
L8c10ff4c: *stop=0x8c10ff4eu;goto L8c1101b8; /* bra 0x8c1101b8 */
L8c10ff50: *stop=0x8c10ff50u;r[0]=52u;goto L8c10ff52; /* mov #0x34,r0 */
L8c10ff52: *stop=0x8c10ff52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ffc4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ff54; /* mov.l 0x8c10ffc4,r3 */
L8c10ff54: *stop=0x8c10ff54u;r[2]=f->fpscr;goto L8c10ff56; /* sts FPSCR,r2 */
L8c10ff56: *stop=0x8c10ff56u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c10ff58; /* fmov.s @(r0,r15),fr5 */
L8c10ff58: *stop=0x8c10ff58u;r[0]=56u;goto L8c10ff5a; /* mov #0x38,r0 */
L8c10ff5a: *stop=0x8c10ff5au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;goto L8c10ff5c; /* fmov.s @(r0,r15),fr4 */
L8c10ff5c: *stop=0x8c10ff5cu;r[8]=r[15];goto L8c10ff5e; /* mov r15,r8 */
L8c10ff5e: *stop=0x8c10ff5eu;r[2]&=r[3];goto L8c10ff60; /* and r3,r2 */
L8c10ff60: *stop=0x8c10ff60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ffc8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ff62; /* mov.l 0x8c10ffc8,r3 */
L8c10ff62: *stop=0x8c10ff62u;f->fpscr=r[2];goto L8c10ff64; /* lds r2,fpscr */
L8c10ff64: *stop=0x8c10ff64u;r[8]+=44u;goto L8c10ff66; /* add #0x2c,r8 */
L8c10ff66: *stop=0x8c10ff66u;r[5]=r[15];goto L8c10ff68; /* mov r15,r5 */
L8c10ff68: *stop=0x8c10ff68u;r[6]=r[8];goto L8c10ff6a; /* mov r8,r6 */
L8c10ff6a: target=r[3];*pr=0x8c10ff6eu;*stop=0x8c10ff6cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10ff6e; /* jsr @r3 */
L8c10ff6e: *stop=0x8c10ff6eu;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10ff70; /* mov.b @r13,r0 */
L8c10ff70: *stop=0x8c10ff70u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10ff72; /* cmp/eq #-0x1,r0 */
L8c10ff72: if(!(*sr&1u))goto L8c10ff7e;goto L8c10ff74; /* bf 0x8c10ff7e */
L8c10ff74: *stop=0x8c10ff74u;r[3]=r[15];goto L8c10ff76; /* mov r15,r3 */
L8c10ff76: *stop=0x8c10ff76u;r[3]+=52u;goto L8c10ff78; /* add #0x34,r3 */
L8c10ff78: *stop=0x8c10ff78u;if(!valid_access(r[3]+7u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3]+0x7u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c10ff7a; /* mov.b @(0x7,r3),r0 */
L8c10ff7a: *stop=0x8c10ff7au;r[0]&=0x7fu;goto L8c10ff7c; /* and #0x7f,r0 */
L8c10ff7c: *stop=0x8c10ff7cu;if(!valid_access(r[3]+7u,1u))return -3;wr(m,bus,&io_status,r[3]+0x7u,r[0],1);if(io_status)return io_status;goto L8c10ff7e; /* mov.b r0,@(0x7,r3) */
L8c10ff7e: *stop=0x8c10ff7eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10ffbau,2);if(io_status)return io_status;r[2]=loaded;}goto L8c10ff80; /* mov.w 0x8c10ffba,r2 */
L8c10ff80: *stop=0x8c10ff80u;if(!valid_access(r[15],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;r[3]=loaded;}goto L8c10ff82; /* mov.w @r15,r3 */
L8c10ff82: *stop=0x8c10ff82u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c10ff84; /* cmp/eq r2,r3 */
L8c10ff84: if(!(*sr&1u))goto L8c10ffcc;goto L8c10ff86; /* bf 0x8c10ffcc */
L8c10ff86: *stop=0x8c10ff86u;if(!valid_access(r[12]+6u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12]+0x6u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c10ff88; /* mov.b @(0x6,r12),r0 */
L8c10ff88: *stop=0x8c10ff88u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10ffbcu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10ff8a; /* mov.w 0x8c10ffbc,r3 */
L8c10ff8a: *stop=0x8c10ff8au;r[0]=r[0]&255u;goto L8c10ff8c; /* extu.b r0,r0 */
L8c10ff8c: *stop=0x8c10ff8cu;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c10ff8e; /* cmp/eq r3,r0 */
L8c10ff8e: if(!(*sr&1u))goto L8c10ffb6;goto L8c10ff90; /* bf 0x8c10ffb6 */
L8c10ff90: *stop=0x8c10ff90u;r[0]=f->fpscr;goto L8c10ff92; /* sts FPSCR,r0 */
L8c10ff92: *stop=0x8c10ff92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ffc4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ff94; /* mov.l 0x8c10ffc4,r1 */
L8c10ff94: *stop=0x8c10ff94u;r[5]=6u;goto L8c10ff96; /* mov #0x6,r5 */
L8c10ff96: *stop=0x8c10ff96u;r[0]&=r[1];goto L8c10ff98; /* and r1,r0 */
L8c10ff98: *stop=0x8c10ff98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ffc0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ff9a; /* mov.l 0x8c10ffc0,r1 */
L8c10ff9a: *stop=0x8c10ff9au;f->fpscr=r[0];goto L8c10ff9c; /* lds r0,fpscr */
L8c10ff9c: target=r[1];*pr=0x8c10ffa0u;*stop=0x8c10ff9eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10ffa0; /* jsr @r1 */
L8c10ffa0: *stop=0x8c10ffa0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10ffa2; /* tst r0,r0 */
L8c10ffa2: if((*sr&1u))goto L8c10ffb6;goto L8c10ffa4; /* bt 0x8c10ffb6 */
L8c10ffa4: *stop=0x8c10ffa4u;if(!valid_access(r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[0]=loaded;}goto L8c10ffa6; /* mov.b @r13,r0 */
L8c10ffa6: *stop=0x8c10ffa6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10ffa8; /* cmp/eq #0x1,r0 */
L8c10ffa8: if(!(*sr&1u))goto L8c10ffae;goto L8c10ffaa; /* bf 0x8c10ffae */
L8c10ffaa: *stop=0x8c10ffacu;r[0]=1u;goto L8c1101ba; /* bra 0x8c1101ba */
L8c10ffae: *stop=0x8c10ffaeu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10ffb0; /* cmp/eq #-0x1,r0 */
L8c10ffb0: if(!(*sr&1u))goto L8c10ffcc;goto L8c10ffb2; /* bf 0x8c10ffcc */
L8c10ffb2: *stop=0x8c10ffb4u;r[0]=4294967295u;goto L8c1101ba; /* bra 0x8c1101ba */
L8c10ffb6: *stop=0x8c10ffb8u;r[0]=2u;goto L8c1101ba; /* bra 0x8c1101ba */
L8c10ffcc: *stop=0x8c10ffccu;if(!valid_access(r[15],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;r[2]=loaded;}goto L8c10ffce; /* mov.w @r15,r2 */
L8c10ffce: *stop=0x8c10ffceu;{uint32_t loaded=sx(m,bus,&io_status,0x8c10fff4u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10ffd0; /* mov.w 0x8c10fff4,r3 */
L8c10ffd0: *stop=0x8c10ffd0u;r[2]-=r[3];goto L8c10ffd2; /* sub r3,r2 */
L8c10ffd2: *stop=0x8c10ffd2u;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[2],2);if(io_status)return io_status;goto L8c10ffd4; /* mov.w r2,@r15 */
L8c10ffd4: *stop=0x8c10ffd4u;r[2]=(uint32_t)(int32_t)(int16_t)r[2];goto L8c10ffd6; /* exts.w r2,r2 */
L8c10ffd6: *stop=0x8c10ffd6u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c10ffd8; /* cmp/pz r2 */
L8c10ffd8: if(!(*sr&1u))goto L8c110000;goto L8c10ffda; /* bf 0x8c110000 */
L8c10ffda: *stop=0x8c10ffdau;r[1]=f->fpscr;goto L8c10ffdc; /* sts FPSCR,r1 */
L8c10ffdc: *stop=0x8c10ffdcu;r[0]=52u;goto L8c10ffde; /* mov #0x34,r0 */
L8c10ffde: *stop=0x8c10ffdeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fff8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ffe0; /* mov.l 0x8c10fff8,r2 */
L8c10ffe0: *stop=0x8c10ffe0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c10ffe2; /* fmov.s @(r0,r15),fr5 */
L8c10ffe2: *stop=0x8c10ffe2u;r[0]=56u;goto L8c10ffe4; /* mov #0x38,r0 */
L8c10ffe4: *stop=0x8c10ffe4u;r[1]&=r[2];goto L8c10ffe6; /* and r2,r1 */
L8c10ffe6: *stop=0x8c10ffe6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;goto L8c10ffe8; /* fmov.s @(r0,r15),fr4 */
L8c10ffe8: *stop=0x8c10ffe8u;f->fpscr=r[1];goto L8c10ffea; /* lds r1,fpscr */
L8c10ffea: *stop=0x8c10ffeau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fffcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ffec; /* mov.l 0x8c10fffc,r2 */
L8c10ffec: target=r[2];*pr=0x8c10fff0u;*stop=0x8c10ffeeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10fff0; /* jsr @r2 */
L8c10fff0: *stop=0x8c10fff2u;r[4]=r[0];goto L8c11001a; /* bra 0x8c11001a */
L8c110000: *stop=0x8c110000u;r[1]=f->fpscr;goto L8c110002; /* sts FPSCR,r1 */
L8c110002: *stop=0x8c110002u;r[0]=52u;goto L8c110004; /* mov #0x34,r0 */
L8c110004: *stop=0x8c110004u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c110006; /* mov.l 0x8c1101d0,r2 */
L8c110006: *stop=0x8c110006u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c110008; /* fmov.s @(r0,r15),fr5 */
L8c110008: *stop=0x8c110008u;r[0]=56u;goto L8c11000a; /* mov #0x38,r0 */
L8c11000a: *stop=0x8c11000au;r[1]&=r[2];goto L8c11000c; /* and r2,r1 */
L8c11000c: *stop=0x8c11000cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;goto L8c11000e; /* fmov.s @(r0,r15),fr4 */
L8c11000e: *stop=0x8c11000eu;f->fpscr=r[1];goto L8c110010; /* lds r1,fpscr */
L8c110010: *stop=0x8c110010u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c110012; /* mov.l 0x8c1101d4,r2 */
L8c110012: target=r[2];*pr=0x8c110016u;*stop=0x8c110014u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c110016; /* jsr @r2 */
L8c110016: *stop=0x8c110016u;r[4]=r[0];goto L8c110018; /* mov r0,r4 */
L8c110018: *stop=0x8c110018u;r[4]+=4294967295u;goto L8c11001a; /* add #-0x1,r4 */
L8c11001a: *stop=0x8c11001au;r[2]=f->fpscr;goto L8c11001c; /* sts FPSCR,r2 */
L8c11001c: *stop=0x8c11001cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c11001e; /* mov.l 0x8c1101d0,r3 */
L8c11001e: *stop=0x8c11001eu;r[13]=r[15];goto L8c110020; /* mov r15,r13 */
L8c110020: *stop=0x8c110020u;r[4]-=r[9];goto L8c110022; /* sub r9,r4 */
L8c110022: *stop=0x8c110022u;r[0]=r[14];goto L8c110024; /* mov r14,r0 */
L8c110024: *stop=0x8c110024u;r[2]&=r[3];goto L8c110026; /* and r3,r2 */
L8c110026: *stop=0x8c110026u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c110028; /* mov.l 0x8c1101d8,r3 */
L8c110028: *stop=0x8c110028u;f->fpscr=r[2];goto L8c11002a; /* lds r2,fpscr */
L8c11002a: *stop=0x8c11002au;r[4]+=1u;goto L8c11002c; /* add #0x1,r4 */
L8c11002c: *stop=0x8c11002cu;if(!valid_access(r[8]+7u,1u))return -3;wr(m,bus,&io_status,r[8]+0x7u,r[0],1);if(io_status)return io_status;goto L8c11002e; /* mov.b r0,@(0x7,r8) */
L8c11002e: *stop=0x8c11002eu;r[13]+=36u;goto L8c110030; /* add #0x24,r13 */
L8c110030: *stop=0x8c110030u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[4],4);if(io_status)return io_status;goto L8c110032; /* mov.l r4,@r10 */
L8c110032: *stop=0x8c110032u;r[5]=r[13];goto L8c110034; /* mov r13,r5 */
L8c110034: target=r[3];*pr=0x8c110038u;*stop=0x8c110036u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c110038; /* jsr @r3 */
L8c110038: *stop=0x8c110038u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c11003a; /* mov.l 0x8c1101dc,r2 */
L8c11003a: *stop=0x8c11003au;r[1]=r[9];goto L8c11003c; /* mov r9,r1 */
L8c11003c: target=r[2];*pr=0x8c110040u;*stop=0x8c11003eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c110040; /* jsr @r2 */
L8c110040: *stop=0x8c110040u;r[5]=8u;goto L8c110042; /* mov #0x8,r5 */
L8c110042: *stop=0x8c110042u;r[12]=r[0];goto L8c110044; /* mov r0,r12 */
L8c110044: *stop=0x8c110044u;r[5]-=r[12];goto L8c110046; /* sub r12,r5 */
L8c110046: *stop=0x8c110046u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c110048; /* cmp/pl r5 */
L8c110048: take=!(*sr&1u);*stop=0x8c11004au;r[4]=r[14];if(take)goto L8c11005e;goto L8c11004c; /* bf/s 0x8c11005e */
L8c11004c: *stop=0x8c11004cu;r[3]=r[4];goto L8c11004e; /* mov r4,r3 */
L8c11004e: *stop=0x8c11004eu;r[3]+=r[12];goto L8c110050; /* add r12,r3 */
L8c110050: *stop=0x8c110050u;r[3]+=r[13];goto L8c110052; /* add r13,r3 */
L8c110052: *stop=0x8c110052u;r[0]=r[4];goto L8c110054; /* mov r4,r0 */
L8c110054: *stop=0x8c110054u;r[4]+=1u;goto L8c110056; /* add #0x1,r4 */
L8c110056: *stop=0x8c110056u;if(!valid_access(r[3],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;r[3]=loaded;}goto L8c110058; /* mov.b @r3,r3 */
L8c110058: *stop=0x8c110058u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]);goto L8c11005a; /* cmp/ge r5,r4 */
L8c11005a: take=!(*sr&1u);*stop=0x8c11005cu;if(!valid_access(r[0]+r[13],1u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[3],1);if(io_status)return io_status;if(take)goto L8c11004c;goto L8c11005e; /* bf/s 0x8c11004c */
L8c11005e: *stop=0x8c11005eu;r[5]=r[13];goto L8c110060; /* mov r13,r5 */
L8c110060: *stop=0x8c110060u;r[5]+=8u;goto L8c110062; /* add #0x8,r5 */
L8c110062: *stop=0x8c110062u;r[4]+=r[13];goto L8c110064; /* add r13,r4 */
L8c110064: *stop=0x8c110064u;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c110066; /* cmp/hs r5,r4 */
L8c110066: if((*sr&1u))goto L8c110070;goto L8c110068; /* bt 0x8c110070 */
L8c110068: *stop=0x8c110068u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[14],1);if(io_status)return io_status;goto L8c11006a; /* mov.b r14,@r4 */
L8c11006a: *stop=0x8c11006au;r[4]+=1u;goto L8c11006c; /* add #0x1,r4 */
L8c11006c: *stop=0x8c11006cu;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c11006e; /* cmp/hs r5,r4 */
L8c11006e: if(!(*sr&1u))goto L8c110068;goto L8c110070; /* bf 0x8c110068 */
L8c110070: *stop=0x8c110070u;r[2]=f->fpscr;goto L8c110072; /* sts FPSCR,r2 */
L8c110072: *stop=0x8c110072u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c110074; /* mov.l 0x8c1101d0,r3 */
L8c110074: *stop=0x8c110074u;r[12]<<=2;goto L8c110076; /* shll2 r12 */
L8c110076: *stop=0x8c110076u;*sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c110078; /* shll r12 */
L8c110078: *stop=0x8c110078u;r[6]=r[9];goto L8c11007a; /* mov r9,r6 */
L8c11007a: *stop=0x8c11007au;r[2]&=r[3];goto L8c11007c; /* and r3,r2 */
L8c11007c: *stop=0x8c11007cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c11007e; /* mov.l 0x8c1101e0,r3 */
L8c11007e: *stop=0x8c11007eu;f->fpscr=r[2];goto L8c110080; /* lds r2,fpscr */
L8c110080: *stop=0x8c110080u;r[6]-=r[12];goto L8c110082; /* sub r12,r6 */
L8c110082: *stop=0x8c110082u;r[5]=8u;goto L8c110084; /* mov #0x8,r5 */
L8c110084: target=r[3];*pr=0x8c110088u;*stop=0x8c110086u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c110088; /* jsr @r3 */
L8c110088: *stop=0x8c110088u;r[2]=f->fpscr;goto L8c11008a; /* sts FPSCR,r2 */
L8c11008a: *stop=0x8c11008au;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c11008c; /* mov.l 0x8c1101d0,r3 */
L8c11008c: *stop=0x8c11008cu;r[12]=r[15];goto L8c11008e; /* mov r15,r12 */
L8c11008e: *stop=0x8c11008eu;r[12]+=28u;goto L8c110090; /* add #0x1c,r12 */
L8c110090: *stop=0x8c110090u;r[4]=r[9];goto L8c110092; /* mov r9,r4 */
L8c110092: *stop=0x8c110092u;r[2]&=r[3];goto L8c110094; /* and r3,r2 */
L8c110094: *stop=0x8c110094u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c110096; /* mov.l 0x8c1101d8,r3 */
L8c110096: *stop=0x8c110096u;f->fpscr=r[2];goto L8c110098; /* lds r2,fpscr */
L8c110098: *stop=0x8c110098u;r[5]=r[12];goto L8c11009a; /* mov r12,r5 */
L8c11009a: *stop=0x8c11009au;r[4]+=4294967295u;goto L8c11009c; /* add #-0x1,r4 */
L8c11009c: target=r[3];*pr=0x8c1100a0u;*stop=0x8c11009eu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c1100a0; /* jsr @r3 */
L8c1100a0: *stop=0x8c1100a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c1100a2; /* mov.l 0x8c1101dc,r2 */
L8c1100a2: *stop=0x8c1100a2u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c1100a4; /* mov.l @(0xc,r15),r1 */
L8c1100a4: target=r[2];*pr=0x8c1100a8u;*stop=0x8c1100a6u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c1100a8; /* jsr @r2 */
L8c1100a8: *stop=0x8c1100a8u;r[5]=8u;goto L8c1100aa; /* mov #0x8,r5 */
L8c1100aa: *stop=0x8c1100aau;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c1100ac; /* mov.l r0,@(0x4,r15) */
L8c1100ac: *stop=0x8c1100acu;r[5]-=r[0];goto L8c1100ae; /* sub r0,r5 */
L8c1100ae: *stop=0x8c1100aeu;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c1100b0; /* cmp/pl r5 */
L8c1100b0: take=!(*sr&1u);*stop=0x8c1100b2u;r[4]=r[14];if(take)goto L8c1100c6;goto L8c1100b4; /* bf/s 0x8c1100c6 */
L8c1100b4: *stop=0x8c1100b4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c1100b6; /* mov.l @(0x4,r15),r3 */
L8c1100b6: *stop=0x8c1100b6u;r[0]=r[4];goto L8c1100b8; /* mov r4,r0 */
L8c1100b8: *stop=0x8c1100b8u;r[4]+=1u;goto L8c1100ba; /* add #0x1,r4 */
L8c1100ba: *stop=0x8c1100bau;r[3]+=r[0];goto L8c1100bc; /* add r0,r3 */
L8c1100bc: *stop=0x8c1100bcu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]);goto L8c1100be; /* cmp/ge r5,r4 */
L8c1100be: *stop=0x8c1100beu;r[3]+=r[12];goto L8c1100c0; /* add r12,r3 */
L8c1100c0: *stop=0x8c1100c0u;if(!valid_access(r[3],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;r[3]=loaded;}goto L8c1100c2; /* mov.b @r3,r3 */
L8c1100c2: take=!(*sr&1u);*stop=0x8c1100c4u;if(!valid_access(r[0]+r[12],1u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[3],1);if(io_status)return io_status;if(take)goto L8c1100b4;goto L8c1100c6; /* bf/s 0x8c1100b4 */
L8c1100c6: *stop=0x8c1100c6u;r[5]=r[12];goto L8c1100c8; /* mov r12,r5 */
L8c1100c8: *stop=0x8c1100c8u;r[5]+=8u;goto L8c1100ca; /* add #0x8,r5 */
L8c1100ca: *stop=0x8c1100cau;r[4]+=r[12];goto L8c1100cc; /* add r12,r4 */
L8c1100cc: *stop=0x8c1100ccu;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c1100ce; /* cmp/hs r5,r4 */
L8c1100ce: if((*sr&1u))goto L8c1100d8;goto L8c1100d0; /* bt 0x8c1100d8 */
L8c1100d0: *stop=0x8c1100d0u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[14],1);if(io_status)return io_status;goto L8c1100d2; /* mov.b r14,@r4 */
L8c1100d2: *stop=0x8c1100d2u;r[4]+=1u;goto L8c1100d4; /* add #0x1,r4 */
L8c1100d4: *stop=0x8c1100d4u;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c1100d6; /* cmp/hs r5,r4 */
L8c1100d6: if(!(*sr&1u))goto L8c1100d0;goto L8c1100d8; /* bf 0x8c1100d0 */
L8c1100d8: *stop=0x8c1100d8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c1100da; /* mov.l @(0x4,r15),r3 */
L8c1100da: *stop=0x8c1100dau;r[5]=8u;goto L8c1100dc; /* mov #0x8,r5 */
L8c1100dc: *stop=0x8c1100dcu;r[2]=f->fpscr;goto L8c1100de; /* sts FPSCR,r2 */
L8c1100de: *stop=0x8c1100deu;r[3]<<=2;goto L8c1100e0; /* shll2 r3 */
L8c1100e0: *stop=0x8c1100e0u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c1100e2; /* shll r3 */
L8c1100e2: *stop=0x8c1100e2u;r[9]-=r[3];goto L8c1100e4; /* sub r3,r9 */
L8c1100e4: *stop=0x8c1100e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c1100e6; /* mov.l 0x8c1101d0,r3 */
L8c1100e6: *stop=0x8c1100e6u;r[6]=r[9];goto L8c1100e8; /* mov r9,r6 */
L8c1100e8: *stop=0x8c1100e8u;r[6]+=4294967295u;goto L8c1100ea; /* add #-0x1,r6 */
L8c1100ea: *stop=0x8c1100eau;r[2]&=r[3];goto L8c1100ec; /* and r3,r2 */
L8c1100ec: *stop=0x8c1100ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c1100ee; /* mov.l 0x8c1101e0,r3 */
L8c1100ee: *stop=0x8c1100eeu;f->fpscr=r[2];goto L8c1100f0; /* lds r2,fpscr */
L8c1100f0: target=r[3];*pr=0x8c1100f4u;*stop=0x8c1100f2u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c1100f4; /* jsr @r3 */
L8c1100f4: *stop=0x8c1100f4u;r[2]=f->fpscr;goto L8c1100f6; /* sts FPSCR,r2 */
L8c1100f6: *stop=0x8c1100f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c1100f8; /* mov.l 0x8c1101d0,r3 */
L8c1100f8: *stop=0x8c1100f8u;r[5]=r[8];goto L8c1100fa; /* mov r8,r5 */
L8c1100fa: *stop=0x8c1100fau;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[6]=loaded;}goto L8c1100fc; /* mov.l @r10,r6 */
L8c1100fc: *stop=0x8c1100fcu;r[7]=r[11];goto L8c1100fe; /* mov r11,r7 */
L8c1100fe: *stop=0x8c1100feu;r[2]&=r[3];goto L8c110100; /* and r3,r2 */
L8c110100: *stop=0x8c110100u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c110102; /* mov.l 0x8c1101e4,r3 */
L8c110102: *stop=0x8c110102u;f->fpscr=r[2];goto L8c110104; /* lds r2,fpscr */
L8c110104: target=r[3];*pr=0x8c110108u;*stop=0x8c110106u;if(!valid_access(r[15],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c110108; /* jsr @r3 */
L8c110108: *stop=0x8c110108u;r[3]=f->fpscr;goto L8c11010a; /* sts FPSCR,r3 */
L8c11010a: *stop=0x8c11010au;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c11010c; /* mov.l 0x8c1101d0,r2 */
L8c11010c: *stop=0x8c11010cu;r[14]=r[15];goto L8c11010e; /* mov r15,r14 */
L8c11010e: *stop=0x8c11010eu;r[14]+=20u;goto L8c110110; /* add #0x14,r14 */
L8c110110: *stop=0x8c110110u;r[5]=r[11];goto L8c110112; /* mov r11,r5 */
L8c110112: *stop=0x8c110112u;r[3]&=r[2];goto L8c110114; /* and r2,r3 */
L8c110114: *stop=0x8c110114u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101e8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c110116; /* mov.l 0x8c1101e8,r2 */
L8c110116: *stop=0x8c110116u;f->fpscr=r[3];goto L8c110118; /* lds r3,fpscr */
L8c110118: *stop=0x8c110118u;r[6]=8u;goto L8c11011a; /* mov #0x8,r6 */
L8c11011a: target=r[2];*pr=0x8c11011eu;*stop=0x8c11011cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c11011e; /* jsr @r2 */
L8c11011e: *stop=0x8c11011eu;r[3]=f->fpscr;goto L8c110120; /* sts FPSCR,r3 */
L8c110120: *stop=0x8c110120u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c110122; /* mov.l 0x8c1101d0,r2 */
L8c110122: *stop=0x8c110122u;r[6]=8u;goto L8c110124; /* mov #0x8,r6 */
L8c110124: *stop=0x8c110124u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101ecu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c110126; /* mov.l 0x8c1101ec,r9 */
L8c110126: *stop=0x8c110126u;r[5]=r[13];goto L8c110128; /* mov r13,r5 */
L8c110128: *stop=0x8c110128u;r[3]&=r[2];goto L8c11012a; /* and r2,r3 */
L8c11012a: *stop=0x8c11012au;r[4]=r[14];goto L8c11012c; /* mov r14,r4 */
L8c11012c: target=r[9];*pr=0x8c110130u;*stop=0x8c11012eu;f->fpscr=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c110130; /* jsr @r9 */
L8c110130: *stop=0x8c110130u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c110132; /* cmp/pl r0 */
L8c110132: if(!(*sr&1u))goto L8c110140;goto L8c110134; /* bf 0x8c110140 */
L8c110134: *stop=0x8c110134u;r[0]=1u;goto L8c110136; /* mov #0x1,r0 */
L8c110136: *stop=0x8c110136u;if(!valid_access(r[15]+16u,2u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[0],2);if(io_status)return io_status;goto L8c110138; /* mov.w r0,@(0x10,r15) */
L8c110138: *stop=0x8c110138u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c11013a; /* mov.l @r10,r3 */
L8c11013a: *stop=0x8c11013au;r[3]+=1u;goto L8c11013c; /* add #0x1,r3 */
L8c11013c: *stop=0x8c11013eu;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c1100f4; /* bra 0x8c1100f4 */
L8c110140: *stop=0x8c110140u;r[2]=f->fpscr;goto L8c110142; /* sts FPSCR,r2 */
L8c110142: *stop=0x8c110142u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c110144; /* mov.l 0x8c1101d0,r3 */
L8c110144: *stop=0x8c110144u;r[6]=8u;goto L8c110146; /* mov #0x8,r6 */
L8c110146: *stop=0x8c110146u;r[5]=r[11];goto L8c110148; /* mov r11,r5 */
L8c110148: *stop=0x8c110148u;r[2]&=r[3];goto L8c11014a; /* and r3,r2 */
L8c11014a: *stop=0x8c11014au;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c11014c; /* mov.l 0x8c1101e8,r3 */
L8c11014c: *stop=0x8c11014cu;f->fpscr=r[2];goto L8c11014e; /* lds r2,fpscr */
L8c11014e: target=r[3];*pr=0x8c110152u;*stop=0x8c110150u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c110152; /* jsr @r3 */
L8c110152: *stop=0x8c110152u;r[2]=f->fpscr;goto L8c110154; /* sts FPSCR,r2 */
L8c110154: *stop=0x8c110154u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c110156; /* mov.l 0x8c1101d0,r3 */
L8c110156: *stop=0x8c110156u;r[6]=8u;goto L8c110158; /* mov #0x8,r6 */
L8c110158: *stop=0x8c110158u;r[5]=r[13];goto L8c11015a; /* mov r13,r5 */
L8c11015a: *stop=0x8c11015au;r[4]=r[14];goto L8c11015c; /* mov r14,r4 */
L8c11015c: *stop=0x8c11015cu;r[2]&=r[3];goto L8c11015e; /* and r3,r2 */
L8c11015e: target=r[9];*pr=0x8c110162u;*stop=0x8c110160u;f->fpscr=r[2];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c110162; /* jsr @r9 */
L8c110162: *stop=0x8c110162u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c110164; /* tst r0,r0 */
L8c110164: if(!(*sr&1u))goto L8c110184;goto L8c110166; /* bf 0x8c110184 */
L8c110166: *stop=0x8c110166u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c110168; /* mov.l @r10,r3 */
L8c110168: *stop=0x8c110168u;r[7]=r[11];goto L8c11016a; /* mov r11,r7 */
L8c11016a: *stop=0x8c11016au;r[2]=f->fpscr;goto L8c11016c; /* sts FPSCR,r2 */
L8c11016c: *stop=0x8c11016cu;r[3]+=1u;goto L8c11016e; /* add #0x1,r3 */
L8c11016e: *stop=0x8c11016eu;r[5]=r[8];goto L8c110170; /* mov r8,r5 */
L8c110170: *stop=0x8c110170u;r[6]=r[3];goto L8c110172; /* mov r3,r6 */
L8c110172: *stop=0x8c110172u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c110174; /* mov.l r3,@r10 */
L8c110174: *stop=0x8c110174u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c110176; /* mov.l 0x8c1101d0,r3 */
L8c110176: *stop=0x8c110176u;r[2]&=r[3];goto L8c110178; /* and r3,r2 */
L8c110178: *stop=0x8c110178u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c11017a; /* mov.l 0x8c1101e4,r3 */
L8c11017a: *stop=0x8c11017au;f->fpscr=r[2];goto L8c11017c; /* lds r2,fpscr */
L8c11017c: target=r[3];*pr=0x8c110180u;*stop=0x8c11017eu;if(!valid_access(r[15],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c110180; /* jsr @r3 */
L8c110180: *stop=0x8c110182u;goto L8c1101b8; /* bra 0x8c1101b8 */
L8c110184: *stop=0x8c110184u;r[2]=f->fpscr;goto L8c110186; /* sts FPSCR,r2 */
L8c110186: *stop=0x8c110186u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c110188; /* mov.l 0x8c1101d0,r3 */
L8c110188: *stop=0x8c110188u;r[6]=8u;goto L8c11018a; /* mov #0x8,r6 */
L8c11018a: *stop=0x8c11018au;r[5]=r[11];goto L8c11018c; /* mov r11,r5 */
L8c11018c: *stop=0x8c11018cu;r[2]&=r[3];goto L8c11018e; /* and r3,r2 */
L8c11018e: *stop=0x8c11018eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c110190; /* mov.l 0x8c1101e8,r3 */
L8c110190: *stop=0x8c110190u;f->fpscr=r[2];goto L8c110192; /* lds r2,fpscr */
L8c110192: target=r[3];*pr=0x8c110196u;*stop=0x8c110194u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c110196; /* jsr @r3 */
L8c110196: *stop=0x8c110196u;r[2]=f->fpscr;goto L8c110198; /* sts FPSCR,r2 */
L8c110198: *stop=0x8c110198u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1101d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c11019a; /* mov.l 0x8c1101d0,r3 */
L8c11019a: *stop=0x8c11019au;r[6]=8u;goto L8c11019c; /* mov #0x8,r6 */
L8c11019c: *stop=0x8c11019cu;r[5]=r[12];goto L8c11019e; /* mov r12,r5 */
L8c11019e: *stop=0x8c11019eu;r[4]=r[14];goto L8c1101a0; /* mov r14,r4 */
L8c1101a0: *stop=0x8c1101a0u;r[2]&=r[3];goto L8c1101a2; /* and r3,r2 */
L8c1101a2: target=r[9];*pr=0x8c1101a6u;*stop=0x8c1101a4u;f->fpscr=r[2];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c1101a6; /* jsr @r9 */
L8c1101a6: *stop=0x8c1101a6u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c1101a8; /* cmp/pz r0 */
L8c1101a8: if((*sr&1u))goto L8c1101b8;goto L8c1101aa; /* bt 0x8c1101b8 */
L8c1101aa: *stop=0x8c1101aau;if(!valid_access(r[15]+16u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x10u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c1101ac; /* mov.w @(0x10,r15),r0 */
L8c1101ac: *stop=0x8c1101acu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c1101ae; /* tst r0,r0 */
L8c1101ae: if(!(*sr&1u))goto L8c1101b8;goto L8c1101b0; /* bf 0x8c1101b8 */
L8c1101b0: *stop=0x8c1101b0u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c1101b2; /* mov.l @r10,r2 */
L8c1101b2: *stop=0x8c1101b2u;r[2]+=4294967295u;goto L8c1101b4; /* add #-0x1,r2 */
L8c1101b4: *stop=0x8c1101b6u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[2],4);if(io_status)return io_status;goto L8c1100f4; /* bra 0x8c1100f4 */
L8c1101b8: *stop=0x8c1101b8u;r[0]=0u;goto L8c1101ba; /* mov #0x0,r0 */
L8c1101ba: *stop=0x8c1101bau;r[15]+=60u;goto L8c1101bc; /* add #0x3c,r15 */
L8c1101bc: *stop=0x8c1101bcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c1101be; /* lds.l @r15+,PR */
L8c1101be: *stop=0x8c1101beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c1101c0; /* mov.l @r15+,r8 */
L8c1101c0: *stop=0x8c1101c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c1101c2; /* mov.l @r15+,r9 */
L8c1101c2: *stop=0x8c1101c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c1101c4; /* mov.l @r15+,r10 */
L8c1101c4: *stop=0x8c1101c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c1101c6; /* mov.l @r15+,r11 */
L8c1101c6: *stop=0x8c1101c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c1101c8; /* mov.l @r15+,r12 */
L8c1101c8: *stop=0x8c1101c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c1101ca; /* mov.l @r15+,r13 */
L8c1101ca: *stop=0x8c1101cau;if(*pr!=entry_pr)return -3;*stop=0x8c1101ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10fdac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10fdac;
L8c10fdac: *stop=0x8c10fdacu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10fdae; /* mov.l r14,@-r15 */
L8c10fdae: *stop=0x8c10fdaeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10fdb0; /* mov.l r13,@-r15 */
L8c10fdb0: *stop=0x8c10fdb0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10fdb2; /* mov.l r12,@-r15 */
L8c10fdb2: *stop=0x8c10fdb2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10fdb4; /* mov.l r11,@-r15 */
L8c10fdb4: *stop=0x8c10fdb4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10fdb6; /* mov.l r10,@-r15 */
L8c10fdb6: *stop=0x8c10fdb6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10fdb8; /* mov.l r9,@-r15 */
L8c10fdb8: *stop=0x8c10fdb8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10fdba; /* mov.l r8,@-r15 */
L8c10fdba: *stop=0x8c10fdbau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10fdbc; /* sts.l PR,@-r15 */
L8c10fdbc: *stop=0x8c10fdbcu;r[15]+=4294967252u;goto L8c10fdbe; /* add #-0x2c,r15 */
L8c10fdbe: *stop=0x8c10fdbeu;r[3]=1u;goto L8c10fdc0; /* mov #0x1,r3 */
L8c10fdc0: *stop=0x8c10fdc0u;r[8]=r[15];goto L8c10fdc2; /* mov r15,r8 */
L8c10fdc2: *stop=0x8c10fdc2u;r[2]=0u;goto L8c10fdc4; /* mov #0x0,r2 */
L8c10fdc4: *stop=0x8c10fdc4u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c10fdc6; /* mov.l r5,@(0xc,r15) */
L8c10fdc6: *stop=0x8c10fdc6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c10fdc8; /* mov.l r6,@r15 */
L8c10fdc8: *stop=0x8c10fdc8u;r[5]=r[4];goto L8c10fdca; /* mov r4,r5 */
L8c10fdca: *stop=0x8c10fdcau;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c10fdcc; /* mov.l r3,@(0x18,r15) */
L8c10fdcc: *stop=0x8c10fdccu;r[6]=8u;goto L8c10fdce; /* mov #0x8,r6 */
L8c10fdce: *stop=0x8c10fdceu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fdd0; /* mov.l @(0xc,r15),r3 */
L8c10fdd0: *stop=0x8c10fdd0u;r[8]+=36u;goto L8c10fdd2; /* add #0x24,r8 */
L8c10fdd2: *stop=0x8c10fdd2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}goto L8c10fdd4; /* mov.l @r15,r11 */
L8c10fdd4: *stop=0x8c10fdd4u;r[11]+=r[3];goto L8c10fdd6; /* add r3,r11 */
L8c10fdd6: *stop=0x8c10fdd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fec4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fdd8; /* mov.l 0x8c10fec4,r3 */
L8c10fdd8: *stop=0x8c10fdd8u;if(!valid_access(r[11],1u))return -3;wr(m,bus,&io_status,r[11],r[2],1);if(io_status)return io_status;goto L8c10fdda; /* mov.b r2,@r11 */
L8c10fdda: target=r[3];*pr=0x8c10fddeu;*stop=0x8c10fddcu;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10fdde; /* jsr @r3 */
L8c10fdde: *stop=0x8c10fddeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fde0; /* mov.l @r15,r2 */
L8c10fde0: *stop=0x8c10fde0u;r[12]=r[15];goto L8c10fde2; /* mov r15,r12 */
L8c10fde2: *stop=0x8c10fde2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fec8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fde4; /* mov.l 0x8c10fec8,r3 */
L8c10fde4: *stop=0x8c10fde4u;r[10]=8u;goto L8c10fde6; /* mov #0x8,r10 */
L8c10fde6: *stop=0x8c10fde6u;r[2]+=4294967295u;goto L8c10fde8; /* add #-0x1,r2 */
L8c10fde8: *stop=0x8c10fde8u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c10fdea; /* mov.l r2,@(0x4,r15) */
L8c10fdea: *stop=0x8c10fdeau;r[2]<<=2;goto L8c10fdec; /* shll2 r2 */
L8c10fdec: *stop=0x8c10fdecu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c10fdee; /* shll r2 */
L8c10fdee: *stop=0x8c10fdeeu;r[2]+=r[3];goto L8c10fdf0; /* add r3,r2 */
L8c10fdf0: *stop=0x8c10fdf0u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c10fdf2; /* mov.l r2,@(0x14,r15) */
L8c10fdf2: *stop=0x8c10fdf2u;r[12]+=28u;goto L8c10fdf4; /* add #0x1c,r12 */
L8c10fdf4: *stop=0x8c10fdf4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fdf6; /* mov.l @(0x4,r15),r3 */
L8c10fdf6: *stop=0x8c10fdf6u;r[3]+=1u;goto L8c10fdf8; /* add #0x1,r3 */
L8c10fdf8: *stop=0x8c10fdfau;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c10fea8; /* bra 0x8c10fea8 */
L8c10fdfc: *stop=0x8c10fdfcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10feccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fdfe; /* mov.l 0x8c10fecc,r3 */
L8c10fdfe: *stop=0x8c10fdfeu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10fe00; /* mov.l @(0x10,r15),r1 */
L8c10fe00: target=r[3];*pr=0x8c10fe04u;*stop=0x8c10fe02u;r[0]=5u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10fe04; /* jsr @r3 */
L8c10fe04: *stop=0x8c10fe04u;r[0]+=1u;goto L8c10fe06; /* add #0x1,r0 */
L8c10fe06: *stop=0x8c10fe06u;r[14]=r[10];goto L8c10fe08; /* mov r10,r14 */
L8c10fe08: *stop=0x8c10fe08u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c10fe0a; /* shll r0 */
L8c10fe0a: *stop=0x8c10fe0au;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10fe0c; /* mov.l @(0x14,r15),r5 */
L8c10fe0c: *stop=0x8c10fe0cu;r[13]=r[10];goto L8c10fe0e; /* mov r10,r13 */
L8c10fe0e: *stop=0x8c10fe0eu;r[14]-=r[0];goto L8c10fe10; /* sub r0,r14 */
L8c10fe10: *stop=0x8c10fe10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fec4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fe12; /* mov.l 0x8c10fec4,r3 */
L8c10fe12: *stop=0x8c10fe12u;r[13]-=r[14];goto L8c10fe14; /* sub r14,r13 */
L8c10fe14: *stop=0x8c10fe14u;r[4]=r[14];goto L8c10fe16; /* mov r14,r4 */
L8c10fe16: *stop=0x8c10fe16u;r[6]=r[13];goto L8c10fe18; /* mov r13,r6 */
L8c10fe18: *stop=0x8c10fe18u;r[5]+=r[14];goto L8c10fe1a; /* add r14,r5 */
L8c10fe1a: target=r[3];*pr=0x8c10fe1eu;*stop=0x8c10fe1cu;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10fe1e; /* jsr @r3 */
L8c10fe1e: *stop=0x8c10fe1eu;r[2]=0u;goto L8c10fe20; /* mov #0x0,r2 */
L8c10fe20: *stop=0x8c10fe20u;r[9]=r[10];goto L8c10fe22; /* mov r10,r9 */
L8c10fe22: *stop=0x8c10fe22u;r[3]=r[2];goto L8c10fe24; /* mov r2,r3 */
L8c10fe24: *stop=0x8c10fe24u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c10fe26; /* mov.l r2,@(0x8,r15) */
L8c10fe26: *stop=0x8c10fe26u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10fe28; /* mov.l r3,@r15 */
L8c10fe28: *stop=0x8c10fe28u;r[5]=r[14];goto L8c10fe2a; /* mov r14,r5 */
L8c10fe2a: *stop=0x8c10fe2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fed0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fe2c; /* mov.l 0x8c10fed0,r3 */
L8c10fe2c: *stop=0x8c10fe2cu;r[4]=r[14];goto L8c10fe2e; /* mov r14,r4 */
L8c10fe2e: *stop=0x8c10fe2eu;r[6]=r[13];goto L8c10fe30; /* mov r13,r6 */
L8c10fe30: *stop=0x8c10fe30u;r[5]+=r[12];goto L8c10fe32; /* add r12,r5 */
L8c10fe32: target=r[3];*pr=0x8c10fe36u;*stop=0x8c10fe34u;r[4]+=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10fe36; /* jsr @r3 */
L8c10fe36: *stop=0x8c10fe36u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c10fe38; /* cmp/pz r0 */
L8c10fe38: if(!(*sr&1u))goto L8c10fe52;goto L8c10fe3a; /* bf 0x8c10fe52 */
L8c10fe3a: *stop=0x8c10fe3au;r[5]=r[14];goto L8c10fe3c; /* mov r14,r5 */
L8c10fe3c: *stop=0x8c10fe3cu;r[6]=r[14];goto L8c10fe3e; /* mov r14,r6 */
L8c10fe3e: *stop=0x8c10fe3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fed4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fe40; /* mov.l 0x8c10fed4,r3 */
L8c10fe40: *stop=0x8c10fe40u;r[5]+=r[8];goto L8c10fe42; /* add r8,r5 */
L8c10fe42: *stop=0x8c10fe42u;r[7]=r[13];goto L8c10fe44; /* mov r13,r7 */
L8c10fe44: *stop=0x8c10fe44u;r[6]+=r[12];goto L8c10fe46; /* add r12,r6 */
L8c10fe46: target=r[3];*pr=0x8c10fe4au;*stop=0x8c10fe48u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10fe4a; /* jsr @r3 */
L8c10fe4a: *stop=0x8c10fe4au;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c10fe4c; /* mov.l r0,@(0x18,r15) */
L8c10fe4c: *stop=0x8c10fe4cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fe4e; /* mov.l @(0x8,r15),r3 */
L8c10fe4e: *stop=0x8c10fe4eu;r[3]+=r[9];goto L8c10fe50; /* add r9,r3 */
L8c10fe50: *stop=0x8c10fe50u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10fe52; /* mov.l r3,@(0x8,r15) */
L8c10fe52: *stop=0x8c10fe52u;r[3]=0u;goto L8c10fe54; /* mov #0x0,r3 */
L8c10fe54: *stop=0x8c10fe54u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[9]);goto L8c10fe56; /* cmp/gt r9,r3 */
L8c10fe56: *stop=0x8c10fe56u;{uint64_t sum=(uint64_t)r[9]+r[3]+(*sr&1u);r[9]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);}goto L8c10fe58; /* addc r3,r9 */
L8c10fe58: *stop=0x8c10fe58u;*sr=(*sr&~1u)|(r[9]&1u);r[9]=(r[9]>>1)|(r[9]&0x80000000u);goto L8c10fe5a; /* shar r9 */
L8c10fe5a: *stop=0x8c10fe5au;*sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c10fe5c; /* tst r9,r9 */
L8c10fe5c: if((*sr&1u))goto L8c10fe76;goto L8c10fe5e; /* bt 0x8c10fe76 */
L8c10fe5e: *stop=0x8c10fe5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fed8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fe60; /* mov.l 0x8c10fed8,r3 */
L8c10fe60: *stop=0x8c10fe60u;r[4]=r[14];goto L8c10fe62; /* mov r14,r4 */
L8c10fe62: *stop=0x8c10fe62u;r[6]=1u;goto L8c10fe64; /* mov #0x1,r6 */
L8c10fe64: *stop=0x8c10fe64u;r[5]=r[13];goto L8c10fe66; /* mov r13,r5 */
L8c10fe66: target=r[3];*pr=0x8c10fe6au;*stop=0x8c10fe68u;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10fe6a; /* jsr @r3 */
L8c10fe6a: *stop=0x8c10fe6au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fe6c; /* mov.l @r15,r2 */
L8c10fe6c: *stop=0x8c10fe6cu;r[3]=4u;goto L8c10fe6e; /* mov #0x4,r3 */
L8c10fe6e: *stop=0x8c10fe6eu;r[2]+=1u;goto L8c10fe70; /* add #0x1,r2 */
L8c10fe70: *stop=0x8c10fe70u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c10fe72; /* cmp/ge r3,r2 */
L8c10fe72: take=!(*sr&1u);*stop=0x8c10fe74u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c10fe28;goto L8c10fe76; /* bf/s 0x8c10fe28 */
L8c10fe76: *stop=0x8c10fe76u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fe78; /* mov.l @(0x18,r15),r3 */
L8c10fe78: *stop=0x8c10fe78u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10fe7a; /* mov.l @(0x8,r15),r4 */
L8c10fe7a: *stop=0x8c10fe7au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c10fe7c; /* tst r3,r3 */
L8c10fe7c: take=!(*sr&1u);*stop=0x8c10fe7eu;r[4]+=48u;if(take)goto L8c10fe94;goto L8c10fe80; /* bf/s 0x8c10fe94 */
L8c10fe80: *stop=0x8c10fe82u;if(!valid_access(r[11]-1u,1u))return -3;r[11]-=1;wr(m,bus,&io_status,r[11],r[4],1);if(io_status)return io_status;goto L8c10fe88; /* bra 0x8c10fe88 */
L8c10fe84: *stop=0x8c10fe84u;r[2]=48u;goto L8c10fe86; /* mov #0x30,r2 */
L8c10fe86: *stop=0x8c10fe86u;if(!valid_access(r[11],1u))return -3;wr(m,bus,&io_status,r[11],r[2],1);if(io_status)return io_status;goto L8c10fe88; /* mov.b r2,@r11 */
L8c10fe88: *stop=0x8c10fe88u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fe8a; /* mov.l @(0xc,r15),r3 */
L8c10fe8a: *stop=0x8c10fe8au;r[11]+=4294967295u;goto L8c10fe8c; /* add #-0x1,r11 */
L8c10fe8c: *stop=0x8c10fe8cu;*sr=(*sr&~1u)|(r[11]>=r[3]);goto L8c10fe8e; /* cmp/hs r3,r11 */
L8c10fe8e: if((*sr&1u))goto L8c10fe84;goto L8c10fe90; /* bt 0x8c10fe84 */
L8c10fe90: *stop=0x8c10fe92u;goto L8c10feae; /* bra 0x8c10feae */
L8c10fe94: *stop=0x8c10fe94u;if(!valid_access(r[11]-1u,1u))return -3;r[11]-=1;wr(m,bus,&io_status,r[11],r[4],1);if(io_status)return io_status;goto L8c10fe96; /* mov.b r4,@-r11 */
L8c10fe96: *stop=0x8c10fe96u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fe98; /* mov.l @(0x4,r15),r3 */
L8c10fe98: *stop=0x8c10fe98u;r[3]+=4294967295u;goto L8c10fe9a; /* add #-0x1,r3 */
L8c10fe9a: *stop=0x8c10fe9au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c10fe9c; /* mov.l r3,@(0x4,r15) */
L8c10fe9c: *stop=0x8c10fe9cu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fe9e; /* mov.l @(0x14,r15),r2 */
L8c10fe9e: *stop=0x8c10fe9eu;r[2]+=4294967288u;goto L8c10fea0; /* add #-0x8,r2 */
L8c10fea0: *stop=0x8c10fea0u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c10fea2; /* mov.l r2,@(0x14,r15) */
L8c10fea2: *stop=0x8c10fea2u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10fea4; /* mov.l @(0x10,r15),r1 */
L8c10fea4: *stop=0x8c10fea4u;r[1]+=4294967295u;goto L8c10fea6; /* add #-0x1,r1 */
L8c10fea6: *stop=0x8c10fea6u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c10fea8; /* mov.l r1,@(0x10,r15) */
L8c10fea8: *stop=0x8c10fea8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10feaa; /* mov.l @(0x4,r15),r3 */
L8c10feaa: *stop=0x8c10feaau;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c10feac; /* cmp/pz r3 */
L8c10feac: if((*sr&1u))goto L8c10fdfc;goto L8c10feae; /* bt 0x8c10fdfc */
L8c10feae: *stop=0x8c10feaeu;r[15]+=44u;goto L8c10feb0; /* add #0x2c,r15 */
L8c10feb0: *stop=0x8c10feb0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10feb2; /* lds.l @r15+,PR */
L8c10feb2: *stop=0x8c10feb2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c10feb4; /* mov.l @r15+,r8 */
L8c10feb4: *stop=0x8c10feb4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c10feb6; /* mov.l @r15+,r9 */
L8c10feb6: *stop=0x8c10feb6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10feb8; /* mov.l @r15+,r10 */
L8c10feb8: *stop=0x8c10feb8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10feba; /* mov.l @r15+,r11 */
L8c10feba: *stop=0x8c10febau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10febc; /* mov.l @r15+,r12 */
L8c10febc: *stop=0x8c10febcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10febe; /* mov.l @r15+,r13 */
L8c10febe: *stop=0x8c10febeu;if(*pr!=entry_pr)return -3;*stop=0x8c10fec0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_diagnostics_8c10f8a2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10f8a2;
L8c10f8a2: *stop=0x8c10f8a2u;r[15]+=4294967292u;goto L8c10f8a4; /* add #-0x4,r15 */
L8c10f8a4: *stop=0x8c10f8a4u;r[6]=r[5];goto L8c10f8a6; /* mov r5,r6 */
L8c10f8a6: *stop=0x8c10f8a6u;r[6]+=r[4];goto L8c10f8a8; /* add r4,r6 */
L8c10f8a8: *stop=0x8c10f8a8u;if(!valid_access(r[6],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[3]=loaded;}goto L8c10f8aa; /* mov.b @r6,r3 */
L8c10f8aa: *stop=0x8c10f8aau;r[2]=52u;goto L8c10f8ac; /* mov #0x34,r2 */
L8c10f8ac: *stop=0x8c10f8acu;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[2]);goto L8c10f8ae; /* cmp/gt r2,r3 */
L8c10f8ae: if(!(*sr&1u))goto L8c10f8fe;goto L8c10f8b0; /* bf 0x8c10f8fe */
L8c10f8b0: *stop=0x8c10f8b0u;r[5]+=4294967295u;goto L8c10f8b2; /* add #-0x1,r5 */
L8c10f8b2: *stop=0x8c10f8b2u;r[5]+=r[4];goto L8c10f8b4; /* add r4,r5 */
L8c10f8b4: *stop=0x8c10f8b4u;r[1]=48u;goto L8c10f8b6; /* mov #0x30,r1 */
L8c10f8b6: *stop=0x8c10f8b6u;if(!valid_access(r[6],1u))return -3;wr(m,bus,&io_status,r[6],r[1],1);if(io_status)return io_status;goto L8c10f8b8; /* mov.b r1,@r6 */
L8c10f8b8: *stop=0x8c10f8b8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c10f8ba; /* mov.l r5,@r15 */
L8c10f8ba: *stop=0x8c10f8bau;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[3]=loaded;}goto L8c10f8bc; /* mov.b @r5,r3 */
L8c10f8bc: *stop=0x8c10f8bcu;r[3]+=1u;goto L8c10f8be; /* add #0x1,r3 */
L8c10f8be: *stop=0x8c10f8beu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c10f8c0; /* mov.b r3,@r5 */
L8c10f8c0: *stop=0x8c10f8c0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10f8c2; /* mov.l @r15,r5 */
L8c10f8c2: *stop=0x8c10f8c2u;r[6]=r[5];goto L8c10f8c4; /* mov r5,r6 */
L8c10f8c4: *stop=0x8c10f8c4u;*sr=(*sr&~1u)|(r[6]>r[4]);goto L8c10f8c6; /* cmp/hi r4,r6 */
L8c10f8c6: take=!(*sr&1u);*stop=0x8c10f8c8u;r[7]=57u;if(take)goto L8c10f8e8;goto L8c10f8ca; /* bf/s 0x8c10f8e8 */
L8c10f8ca: *stop=0x8c10f8cau;if(!valid_access(r[6],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10f8cc; /* mov.b @r6,r2 */
L8c10f8cc: *stop=0x8c10f8ccu;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[7]);goto L8c10f8ce; /* cmp/gt r7,r2 */
L8c10f8ce: if(!(*sr&1u))goto L8c10f8e0;goto L8c10f8d0; /* bf 0x8c10f8e0 */
L8c10f8d0: *stop=0x8c10f8d0u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10f8d2; /* mov.b @r5,r2 */
L8c10f8d2: *stop=0x8c10f8d2u;r[3]=r[5];goto L8c10f8d4; /* mov r5,r3 */
L8c10f8d4: *stop=0x8c10f8d4u;r[3]+=4294967295u;goto L8c10f8d6; /* add #-0x1,r3 */
L8c10f8d6: *stop=0x8c10f8d6u;r[2]+=4294967286u;goto L8c10f8d8; /* add #-0xa,r2 */
L8c10f8d8: *stop=0x8c10f8d8u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[2],1);if(io_status)return io_status;goto L8c10f8da; /* mov.b r2,@r5 */
L8c10f8da: *stop=0x8c10f8dau;if(!valid_access(r[3],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10f8dc; /* mov.b @r3,r2 */
L8c10f8dc: *stop=0x8c10f8dcu;r[2]+=1u;goto L8c10f8de; /* add #0x1,r2 */
L8c10f8de: *stop=0x8c10f8deu;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[2],1);if(io_status)return io_status;goto L8c10f8e0; /* mov.b r2,@r3 */
L8c10f8e0: *stop=0x8c10f8e0u;r[6]+=4294967295u;goto L8c10f8e2; /* add #-0x1,r6 */
L8c10f8e2: *stop=0x8c10f8e2u;*sr=(*sr&~1u)|(r[6]>r[4]);goto L8c10f8e4; /* cmp/hi r4,r6 */
L8c10f8e4: take=(*sr&1u);*stop=0x8c10f8e6u;r[5]+=4294967295u;if(take)goto L8c10f8ca;goto L8c10f8e8; /* bt/s 0x8c10f8ca */
L8c10f8e8: *stop=0x8c10f8e8u;r[5]=r[4];goto L8c10f8ea; /* mov r4,r5 */
L8c10f8ea: *stop=0x8c10f8eau;r[5]+=1u;goto L8c10f8ec; /* add #0x1,r5 */
L8c10f8ec: *stop=0x8c10f8ecu;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10f8ee; /* mov.b @r5,r2 */
L8c10f8ee: *stop=0x8c10f8eeu;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[7]);goto L8c10f8f0; /* cmp/gt r7,r2 */
L8c10f8f0: if(!(*sr&1u))goto L8c10f8fe;goto L8c10f8f2; /* bf 0x8c10f8fe */
L8c10f8f2: *stop=0x8c10f8f2u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10f8f4; /* mov.b @r5,r2 */
L8c10f8f4: *stop=0x8c10f8f4u;r[2]+=4294967286u;goto L8c10f8f6; /* add #-0xa,r2 */
L8c10f8f6: *stop=0x8c10f8f6u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[2],1);if(io_status)return io_status;goto L8c10f8f8; /* mov.b r2,@r5 */
L8c10f8f8: *stop=0x8c10f8f8u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}goto L8c10f8fa; /* mov.b @r4,r3 */
L8c10f8fa: *stop=0x8c10f8fau;r[3]+=1u;goto L8c10f8fc; /* add #0x1,r3 */
L8c10f8fc: *stop=0x8c10f8fcu;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c10f8fe; /* mov.b r3,@r4 */
L8c10f8fe: *stop=0x8c10f8feu;if(*pr!=entry_pr)return -3;*stop=0x8c10f900u;r[15]+=4u;return 0; /* rts */
}
