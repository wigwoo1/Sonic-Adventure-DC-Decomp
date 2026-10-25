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
SA_STAGE_EXPORT int sa_movie_initialize_8c0546c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0546c0;
L8c0546c0: *stop=0x8c0546c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0546c2; /* mov.l r14,@-r15 */
L8c0546c2: *stop=0x8c0546c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0546c4; /* mov.l r13,@-r15 */
L8c0546c4: *stop=0x8c0546c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0546c6; /* sts.l PR,@-r15 */
L8c0546c6: *stop=0x8c0546c6u;r[15]+=4294967292u;goto L8c0546c8; /* add #-0x4,r15 */
L8c0546c8: *stop=0x8c0546c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0547e0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0546ca; /* mov.l 0x8c0547e0,r2 */
L8c0546ca: *stop=0x8c0546cau;r[3]=0u;goto L8c0546cc; /* mov #0x0,r3 */
L8c0546cc: *stop=0x8c0546ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0547e8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0546ce; /* mov.l 0x8c0547e8,r4 */
L8c0546ce: *stop=0x8c0546ceu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c0546d0; /* mov.l r3,@r2 */
L8c0546d0: *stop=0x8c0546d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0547ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0546d2; /* mov.l 0x8c0547ec,r2 */
L8c0546d2: *stop=0x8c0546d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0547e4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0546d4; /* mov.l 0x8c0547e4,r1 */
L8c0546d4: target=r[2];*pr=0x8c0546d8u;*stop=0x8c0546d6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0546d8; /* jsr @r2 */
L8c0546d8: *stop=0x8c0546d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0547f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0546da; /* mov.l 0x8c0547f0,r3 */
L8c0546da: target=r[3];*pr=0x8c0546deu;*stop=0x8c0546dcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0546de; /* jsr @r3 */
L8c0546de: *stop=0x8c0546deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0547fcu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c0546e0; /* mov.l 0x8c0547fc,r14 */
L8c0546e0: *stop=0x8c0546e0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0546e2; /* cmp/eq #0x1,r0 */
L8c0546e2: *stop=0x8c0546e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0547f8u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c0546e4; /* mov.l 0x8c0547f8,r13 */
L8c0546e4: *stop=0x8c0546e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0547f4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0546e6; /* mov.l 0x8c0547f4,r2 */
L8c0546e6: take=!(*sr&1u);*stop=0x8c0546e8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;if(take)goto L8c0546f8;goto L8c0546ea; /* bf/s 0x8c0546f8 */
L8c0546ea: *stop=0x8c0546eau;r[5]=0u;goto L8c0546ec; /* mov #0x0,r5 */
L8c0546ec: *stop=0x8c0546ecu;r[6]=1u;goto L8c0546ee; /* mov #0x1,r6 */
L8c0546ee: target=r[14];*pr=0x8c0546f2u;*stop=0x8c0546f0u;r[4]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0546f2; /* jsr @r14 */
L8c0546f2: *stop=0x8c0546f2u;r[2]=2u;goto L8c0546f4; /* mov #0x2,r2 */
L8c0546f4: *stop=0x8c0546f6u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c05472e; /* bra 0x8c05472e */
L8c0546f8: *stop=0x8c0546f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054800u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0546fa; /* mov.l 0x8c054800,r1 */
L8c0546fa: target=r[1];*pr=0x8c0546feu;*stop=0x8c0546fcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0546fe; /* jsr @r1 */
L8c0546fe: *stop=0x8c0546feu;r[4]=r[0];goto L8c054700; /* mov r0,r4 */
L8c054700: *stop=0x8c054700u;r[4]+=4294967295u;goto L8c054702; /* add #-0x1,r4 */
L8c054702: *stop=0x8c054702u;r[5]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c054704; /* exts.w r4,r5 */
L8c054704: *stop=0x8c054704u;r[0]=r[5];goto L8c054706; /* mov r5,r0 */
L8c054706: *stop=0x8c054706u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c054708; /* cmp/eq #0x1,r0 */
L8c054708: if((*sr&1u))goto L8c054716;goto L8c05470a; /* bt 0x8c054716 */
L8c05470a: *stop=0x8c05470au;*sr=(*sr&~1u)|((int32_t)r[5]>=0);goto L8c05470c; /* cmp/pz r5 */
L8c05470c: if((*sr&1u))goto L8c054722;goto L8c05470e; /* bt 0x8c054722 */
L8c05470e: *stop=0x8c05470eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0547f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c054710; /* mov.l 0x8c0547f4,r3 */
L8c054710: *stop=0x8c054710u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c054712; /* mov.l @r3,r0 */
L8c054712: *stop=0x8c054712u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c054714; /* cmp/eq #0x2,r0 */
L8c054714: if(!(*sr&1u))goto L8c054722;goto L8c054716; /* bf 0x8c054722 */
L8c054716: *stop=0x8c054716u;r[5]=0u;goto L8c054718; /* mov #0x0,r5 */
L8c054718: *stop=0x8c054718u;r[6]=1u;goto L8c05471a; /* mov #0x1,r6 */
L8c05471a: target=r[14];*pr=0x8c05471eu;*stop=0x8c05471cu;r[4]=58u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05471e; /* jsr @r14 */
L8c05471e: *stop=0x8c054720u;r[3]=1u;goto L8c05472c; /* bra 0x8c05472c */
L8c054722: *stop=0x8c054722u;r[5]=0u;goto L8c054724; /* mov #0x0,r5 */
L8c054724: *stop=0x8c054724u;r[6]=1u;goto L8c054726; /* mov #0x1,r6 */
L8c054726: target=r[14];*pr=0x8c05472au;*stop=0x8c054728u;r[4]=56u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05472a; /* jsr @r14 */
L8c05472a: *stop=0x8c05472au;r[3]=0u;goto L8c05472c; /* mov #0x0,r3 */
L8c05472c: *stop=0x8c05472cu;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c05472e; /* mov.l r3,@r13 */
L8c05472e: *stop=0x8c05472eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c054804u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c054730; /* mov.l 0x8c054804,r2 */
L8c054730: target=r[2];*pr=0x8c054734u;*stop=0x8c054732u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054734; /* jsr @r2 */
L8c054734: *stop=0x8c054734u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054808u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c054736; /* mov.l 0x8c054808,r3 */
L8c054736: *stop=0x8c054736u;r[5]=2u;goto L8c054738; /* mov #0x2,r5 */
L8c054738: target=r[3];*pr=0x8c05473cu;*stop=0x8c05473au;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05473c; /* jsr @r3 */
L8c05473c: *stop=0x8c05473cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c05480cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c05473e; /* mov.l 0x8c05480c,r2 */
L8c05473e: target=r[2];*pr=0x8c054742u;*stop=0x8c054740u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054742; /* jsr @r2 */
L8c054742: *stop=0x8c054742u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054818u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c054744; /* mov.l 0x8c054818,r3 */
L8c054744: *stop=0x8c054744u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054814u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c054746; /* mov.l 0x8c054814,r4 */
L8c054746: *stop=0x8c054746u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054810u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c054748; /* mov.l 0x8c054810,r5 */
L8c054748: target=r[3];*pr=0x8c05474cu;*stop=0x8c05474au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05474c; /* jsr @r3 */
L8c05474c: *stop=0x8c05474cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c05481cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c05474e; /* mov.l 0x8c05481c,r2 */
L8c05474e: *stop=0x8c05474eu;r[7]=0u;goto L8c054750; /* mov #0x0,r7 */
L8c054750: *stop=0x8c054750u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054820u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c054752; /* mov.l 0x8c054820,r6 */
L8c054752: *stop=0x8c054752u;r[5]=r[7];goto L8c054754; /* mov r7,r5 */
L8c054754: *stop=0x8c054754u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054824u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c054756; /* mov.l 0x8c054824,r3 */
L8c054756: *stop=0x8c054756u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c054758; /* mov.l r2,@-r15 */
L8c054758: target=r[3];*pr=0x8c05475cu;*stop=0x8c05475au;r[4]=4294967295u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05475c; /* jsr @r3 */
L8c05475c: *stop=0x8c05475cu;r[15]+=4u;goto L8c05475e; /* add #0x4,r15 */
L8c05475e: *stop=0x8c05475eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c054828u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c054760; /* mov.l 0x8c054828,r2 */
L8c054760: *stop=0x8c054760u;r[6]=0u;goto L8c054762; /* mov #0x0,r6 */
L8c054762: *stop=0x8c054762u;r[5]=r[6];goto L8c054764; /* mov r6,r5 */
L8c054764: target=r[2];*pr=0x8c054768u;*stop=0x8c054766u;r[4]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054768; /* jsr @r2 */
L8c054768: *stop=0x8c054768u;{uint32_t loaded=rd(m,bus,&io_status,0x8c05482cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05476a; /* mov.l 0x8c05482c,r3 */
L8c05476a: *stop=0x8c05476au;r[6]=0u;goto L8c05476c; /* mov #0x0,r6 */
L8c05476c: *stop=0x8c05476cu;r[5]=r[6];goto L8c05476e; /* mov r6,r5 */
L8c05476e: target=r[3];*pr=0x8c054772u;*stop=0x8c054770u;r[4]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054772; /* jsr @r3 */
L8c054772: *stop=0x8c054772u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054814u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c054774; /* mov.l 0x8c054814,r4 */
L8c054774: *stop=0x8c054774u;r[5]=16u;goto L8c054776; /* mov #0x10,r5 */
L8c054776: *stop=0x8c054776u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054830u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c054778; /* mov.l 0x8c054830,r2 */
L8c054778: *stop=0x8c054778u;r[5]<<=8;goto L8c05477a; /* shll8 r5 */
L8c05477a: target=r[2];*pr=0x8c05477eu;*stop=0x8c05477cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05477e; /* jsr @r2 */
L8c05477e: *stop=0x8c05477eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c054838u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c054780; /* mov.l 0x8c054838,r3 */
L8c054780: *stop=0x8c054780u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054834u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c054782; /* mov.l 0x8c054834,r4 */
L8c054782: target=r[3];*pr=0x8c054786u;*stop=0x8c054784u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054786; /* jsr @r3 */
L8c054786: *stop=0x8c054786u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054840u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c054788; /* mov.l 0x8c054840,r2 */
L8c054788: *stop=0x8c054788u;{uint32_t loaded=rd(m,bus,&io_status,0x8c05483cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c05478a; /* mov.l 0x8c05483c,r4 */
L8c05478a: target=r[2];*pr=0x8c05478eu;*stop=0x8c05478cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05478e; /* jsr @r2 */
L8c05478e: *stop=0x8c05478eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c054790; /* tst r0,r0 */
L8c054790: if((*sr&1u))goto L8c054796;goto L8c054792; /* bt 0x8c054796 */
L8c054792: *stop=0x8c054794u;goto L8c05486e; /* bra 0x8c05486e */
L8c054796: *stop=0x8c054796u;{uint32_t loaded=rd(m,bus,&io_status,0x8c054844u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c054798; /* mov.l 0x8c054844,r2 */
L8c054798: *stop=0x8c054798u;r[14]=1u;goto L8c05479a; /* mov #0x1,r14 */
L8c05479a: *stop=0x8c05479au;{uint32_t loaded=rd(m,bus,&io_status,0x8c054848u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05479c; /* mov.l 0x8c054848,r3 */
L8c05479c: *stop=0x8c05479cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c05479e; /* mov.l r14,@r2 */
L8c05479e: target=r[3];*pr=0x8c0547a2u;*stop=0x8c0547a0u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0547a2; /* jsr @r3 */
L8c0547a2: *stop=0x8c0547a2u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0547a4; /* tst r0,r0 */
L8c0547a4: if((*sr&1u))goto L8c0547aa;goto L8c0547a6; /* bt 0x8c0547aa */
L8c0547a6: *stop=0x8c0547a8u;goto L8c054868; /* bra 0x8c054868 */
L8c0547aa: *stop=0x8c0547aau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c0547ac; /* mov.l @r15,r0 */
L8c0547ac: *stop=0x8c0547acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c05484cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0547ae; /* mov.l 0x8c05484c,r4 */
L8c0547ae: *stop=0x8c0547aeu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c0547b0; /* cmp/eq #0x0,r0 */
L8c0547b0: if((*sr&1u))goto L8c0547ca;goto L8c0547b2; /* bt 0x8c0547ca */
L8c0547b2: *stop=0x8c0547b2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0547b4; /* cmp/eq #0x1,r0 */
L8c0547b4: if((*sr&1u))goto L8c054866;goto L8c0547b6; /* bt 0x8c054866 */
L8c0547b6: *stop=0x8c0547b6u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c0547b8; /* cmp/eq #0x2,r0 */
L8c0547b8: if((*sr&1u))goto L8c0547d4;goto L8c0547ba; /* bt 0x8c0547d4 */
L8c0547ba: *stop=0x8c0547bau;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c0547bc; /* cmp/eq #0x3,r0 */
L8c0547bc: if((*sr&1u))goto L8c0547d8;goto L8c0547be; /* bt 0x8c0547d8 */
L8c0547be: *stop=0x8c0547beu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c0547c0; /* cmp/eq #0x4,r0 */
L8c0547c0: if((*sr&1u))goto L8c054860;goto L8c0547c2; /* bt 0x8c054860 */
L8c0547c2: *stop=0x8c0547c2u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c0547c4; /* cmp/eq #0x5,r0 */
L8c0547c4: if((*sr&1u))goto L8c054866;goto L8c0547c6; /* bt 0x8c054866 */
L8c0547c6: *stop=0x8c0547c8u;goto L8c054866; /* bra 0x8c054866 */
L8c0547ca: *stop=0x8c0547cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c054844u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0547cc; /* mov.l 0x8c054844,r2 */
L8c0547cc: *stop=0x8c0547ccu;r[3]=0u;goto L8c0547ce; /* mov #0x0,r3 */
L8c0547ce: *stop=0x8c0547ceu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c0547d0; /* mov.l r3,@r2 */
L8c0547d0: *stop=0x8c0547d2u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c054868; /* bra 0x8c054868 */
L8c0547d4: *stop=0x8c0547d6u;r[2]=4u;goto L8c054862; /* bra 0x8c054862 */
L8c0547d8: *stop=0x8c0547d8u;r[1]=2u;goto L8c0547da; /* mov #0x2,r1 */
L8c0547da: *stop=0x8c0547dcu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c054868; /* bra 0x8c054868 */
L8c054860: *stop=0x8c054860u;r[2]=3u;goto L8c054862; /* mov #0x3,r2 */
L8c054862: *stop=0x8c054864u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c054868; /* bra 0x8c054868 */
L8c054866: *stop=0x8c054866u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c054868; /* mov.l r14,@r4 */
L8c054868: *stop=0x8c054868u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0548c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05486a; /* mov.l 0x8c0548c0,r3 */
L8c05486a: target=r[3];*pr=0x8c05486eu;*stop=0x8c05486cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05486e; /* jsr @r3 */
L8c05486e: target=0x8c0544e2u;*pr=0x8c054872u;*stop=0x8c054870u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054872; /* bsr 0x8c0544e2 */
L8c054872: *stop=0x8c054872u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0548c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c054874; /* mov.l 0x8c0548c4,r3 */
L8c054874: target=r[3];*pr=0x8c054878u;*stop=0x8c054876u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054878; /* jsr @r3 */
L8c054878: *stop=0x8c054878u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0548c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c05487a; /* mov.l 0x8c0548c8,r2 */
L8c05487a: target=r[2];*pr=0x8c05487eu;*stop=0x8c05487cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05487e; /* jsr @r2 */
L8c05487e: *stop=0x8c05487eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0548d0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c054880; /* mov.l 0x8c0548d0,r14 */
L8c054880: *stop=0x8c054880u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0548ccu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c054882; /* mov.l 0x8c0548cc,r13 */
L8c054882: target=r[14];*pr=0x8c054886u;*stop=0x8c054884u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054886; /* jsr @r14 */
L8c054886: *stop=0x8c054886u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[0],4);if(io_status)return io_status;goto L8c054888; /* mov.l r0,@r13 */
L8c054888: target=r[14];*pr=0x8c05488cu;*stop=0x8c05488au;r[4]=6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05488c; /* jsr @r14 */
L8c05488c: *stop=0x8c05488cu;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[0],4);if(io_status)return io_status;goto L8c05488e; /* mov.l r0,@(0x4,r13) */
L8c05488e: target=r[14];*pr=0x8c054892u;*stop=0x8c054890u;r[4]=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054892; /* jsr @r14 */
L8c054892: *stop=0x8c054892u;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[0],4);if(io_status)return io_status;goto L8c054894; /* mov.l r0,@(0x8,r13) */
L8c054894: target=r[14];*pr=0x8c054898u;*stop=0x8c054896u;r[4]=18u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c054898; /* jsr @r14 */
L8c054898: *stop=0x8c054898u;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[0],4);if(io_status)return io_status;goto L8c05489a; /* mov.l r0,@(0xc,r13) */
L8c05489a: *stop=0x8c05489au;r[15]+=4u;goto L8c05489c; /* add #0x4,r15 */
L8c05489c: *stop=0x8c05489cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c05489e; /* lds.l @r15+,PR */
L8c05489e: *stop=0x8c05489eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0548a0; /* mov.l @r15+,r13 */
L8c0548a0: *stop=0x8c0548a0u;if(*pr!=entry_pr)return -3;*stop=0x8c0548a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_initialize_8c604510(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c604510;
L8c604510: *stop=0x8c604510u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60461cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604512; /* mov.l 0x8c60461c,r3 */
L8c604512: *stop=0x8c604512u;if(*pr!=entry_pr)return -3;*stop=0x8c604514u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_initialize_8c10c07e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10c07e;
L8c10c07e: *stop=0x8c10c07eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10c080; /* sts.l PR,@-r15 */
L8c10c080: *stop=0x8c10c080u;r[15]+=4294967292u;goto L8c10c082; /* add #-0x4,r15 */
L8c10c082: *stop=0x8c10c082u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10c0ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10c084; /* mov.l 0x8c10c0cc,r3 */
L8c10c084: target=r[3];*pr=0x8c10c088u;*stop=0x8c10c086u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10c088; /* jsr @r3 */
L8c10c088: target=0x8c10c044u;*pr=0x8c10c08cu;*stop=0x8c10c08au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10c08c; /* bsr 0x8c10c044 */
L8c10c08c: *stop=0x8c10c08cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10c08e; /* mov.l @r15,r2 */
L8c10c08e: *stop=0x8c10c08eu;r[4]=r[0];goto L8c10c090; /* mov r0,r4 */
L8c10c090: *stop=0x8c10c090u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c10c092; /* tst r2,r2 */
L8c10c092: if(!(*sr&1u))goto L8c10c09c;goto L8c10c094; /* bf 0x8c10c09c */
L8c10c094: *stop=0x8c10c094u;r[15]+=4u;goto L8c10c096; /* add #0x4,r15 */
L8c10c096: *stop=0x8c10c096u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10c098; /* lds.l @r15+,PR */
L8c10c098: *stop=0x8c10c098u;if(*pr!=entry_pr)return -3;*stop=0x8c10c09au;r[0]=1u;return 0; /* rts */
L8c10c09c: *stop=0x8c10c09cu;r[0]=r[4];goto L8c10c09e; /* mov r4,r0 */
L8c10c09e: *stop=0x8c10c09eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10c0a0; /* cmp/eq #0x1,r0 */
L8c10c0a0: if((*sr&1u))goto L8c10c0b2;goto L8c10c0a2; /* bt 0x8c10c0b2 */
L8c10c0a2: *stop=0x8c10c0a2u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c10c0a4; /* cmp/eq #0x3,r0 */
L8c10c0a4: if((*sr&1u))goto L8c10c0b2;goto L8c10c0a6; /* bt 0x8c10c0b2 */
L8c10c0a6: *stop=0x8c10c0a6u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10c0a8; /* cmp/eq #0x2,r0 */
L8c10c0a8: if((*sr&1u))goto L8c10c0ba;goto L8c10c0aa; /* bt 0x8c10c0ba */
L8c10c0aa: *stop=0x8c10c0aau;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10c0ac; /* cmp/eq #0x0,r0 */
L8c10c0ac: if((*sr&1u))goto L8c10c0ba;goto L8c10c0ae; /* bt 0x8c10c0ba */
L8c10c0ae: *stop=0x8c10c0b0u;goto L8c10c0ba; /* bra 0x8c10c0ba */
L8c10c0b2: *stop=0x8c10c0b2u;r[15]+=4u;goto L8c10c0b4; /* add #0x4,r15 */
L8c10c0b4: *stop=0x8c10c0b4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10c0b6; /* lds.l @r15+,PR */
L8c10c0b6: *stop=0x8c10c0b6u;if(*pr!=entry_pr)return -3;*stop=0x8c10c0b8u;r[0]=2u;return 0; /* rts */
L8c10c0ba: *stop=0x8c10c0bau;r[0]=0u;goto L8c10c0bc; /* mov #0x0,r0 */
L8c10c0bc: *stop=0x8c10c0bcu;r[15]+=4u;goto L8c10c0be; /* add #0x4,r15 */
L8c10c0be: *stop=0x8c10c0beu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10c0c0; /* lds.l @r15+,PR */
L8c10c0c0: *stop=0x8c10c0c0u;if(*pr!=entry_pr)return -3;*stop=0x8c10c0c2u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_initialize_8c10ba1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ba1e;
L8c10ba1e: *stop=0x8c10ba1eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ba44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ba20; /* mov.l 0x8c10ba44,r3 */
L8c10ba20: *stop=0x8c10ba20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ba38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ba22; /* mov.l 0x8c10ba38,r2 */
L8c10ba22: *stop=0x8c10ba22u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10ba24; /* mov.l @r3,r0 */
L8c10ba24: *stop=0x8c10ba24u;*sr=(*sr&~1u)|(r[2]==r[0]);goto L8c10ba26; /* cmp/eq r2,r0 */
L8c10ba26: if((*sr&1u))goto L8c10ba2c;goto L8c10ba28; /* bt 0x8c10ba2c */
L8c10ba28: *stop=0x8c10ba28u;if(*pr!=entry_pr)return -3;*stop=0x8c10ba2au;r[0]=0u;return 0; /* rts */
L8c10ba2c: *stop=0x8c10ba2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ba40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ba2e; /* mov.l 0x8c10ba40,r3 */
L8c10ba2e: *stop=0x8c10ba2eu;if(!valid_access(r[3],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;r[0]=loaded;}goto L8c10ba30; /* mov.w @r3,r0 */
L8c10ba30: *stop=0x8c10ba30u;if(*pr!=entry_pr)return -3;*stop=0x8c10ba32u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_initialize_8c051760(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c051760;
L8c051760: *stop=0x8c051760u;{uint32_t loaded=rd(m,bus,&io_status,0x8c05188cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c051762; /* mov.l 0x8c05188c,r0 */
L8c051762: *stop=0x8c051762u;r[3]=r[4];goto L8c051764; /* mov r4,r3 */
L8c051764: *stop=0x8c051764u;{uint32_t loaded=rd(m,bus,&io_status,0x8c051884u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c051766; /* mov.l 0x8c051884,r2 */
L8c051766: *stop=0x8c051766u;r[3]=0u;goto L8c051768; /* mov #0x0,r3 */
L8c051768: *stop=0x8c051768u;{uint32_t loaded=rd(m,bus,&io_status,0x8c051880u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c05176a; /* mov.l 0x8c051880,r6 */
L8c05176a: *stop=0x8c05176au;{uint32_t loaded=rd(m,bus,&io_status,0x8c051888u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c05176c; /* mov.l 0x8c051888,r1 */
L8c05176c: *stop=0x8c05176cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c05176e; /* sts.l PR,@-r15 */
L8c05176e: *stop=0x8c05176eu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c051770; /* mov.l r4,@r6 */
L8c051770: *stop=0x8c051770u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c051772; /* mov.l r4,@r2 */
L8c051772: *stop=0x8c051772u;{uint32_t loaded=rd(m,bus,&io_status,0x8c051890u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c051774; /* mov.l 0x8c051890,r4 */
L8c051774: *stop=0x8c051774u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c051776; /* mov.l r5,@r1 */
L8c051776: *stop=0x8c051776u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c051778; /* mov.l r3,@r0 */
L8c051778: *stop=0x8c051778u;{uint32_t loaded=rd(m,bus,&io_status,0x8c051894u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05177a; /* mov.l 0x8c051894,r3 */
L8c05177a: target=r[3];*pr=0x8c05177eu;*stop=0x8c05177cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05177e; /* jsr @r3 */
L8c05177e: *stop=0x8c05177eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c051898u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c051780; /* mov.l 0x8c051898,r2 */
L8c051780: target=r[2];*stop=0x8c051782u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_movie_initialize_8c10c044(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10c044;
L8c10c044: *stop=0x8c10c044u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10c046; /* sts.l PR,@-r15 */
L8c10c046: *stop=0x8c10c046u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10c0c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10c048; /* mov.l 0x8c10c0c4,r3 */
L8c10c048: target=r[3];*pr=0x8c10c04cu;*stop=0x8c10c04au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10c04c; /* jsr @r3 */
L8c10c04c: *stop=0x8c10c04cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10c0c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10c04e; /* mov.l 0x8c10c0c8,r2 */
L8c10c04e: target=r[2];*pr=0x8c10c052u;*stop=0x8c10c050u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10c052; /* jsr @r2 */
L8c10c052: *stop=0x8c10c052u;r[4]=r[0];goto L8c10c054; /* mov r0,r4 */
L8c10c054: *stop=0x8c10c054u;if(!valid_access(r[4]+12u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0xcu,1);if(io_status)return io_status;r[0]=loaded;}goto L8c10c056; /* mov.b @(0xc,r4),r0 */
L8c10c056: *stop=0x8c10c056u;*sr=(*sr&~1u)|(r[0]==0x00000031u);goto L8c10c058; /* cmp/eq #0x31,r0 */
L8c10c058: if((*sr&1u))goto L8c10c06a;goto L8c10c05a; /* bt 0x8c10c06a */
L8c10c05a: *stop=0x8c10c05au;*sr=(*sr&~1u)|(r[0]==0x00000032u);goto L8c10c05c; /* cmp/eq #0x32,r0 */
L8c10c05c: if((*sr&1u))goto L8c10c06e;goto L8c10c05e; /* bt 0x8c10c06e */
L8c10c05e: *stop=0x8c10c05eu;*sr=(*sr&~1u)|(r[0]==0x00000033u);goto L8c10c060; /* cmp/eq #0x33,r0 */
L8c10c060: if((*sr&1u))goto L8c10c072;goto L8c10c062; /* bt 0x8c10c072 */
L8c10c062: *stop=0x8c10c062u;*sr=(*sr&~1u)|(r[0]==0x00000030u);goto L8c10c064; /* cmp/eq #0x30,r0 */
L8c10c064: if((*sr&1u))goto L8c10c076;goto L8c10c066; /* bt 0x8c10c076 */
L8c10c066: *stop=0x8c10c068u;goto L8c10c076; /* bra 0x8c10c076 */
L8c10c06a: *stop=0x8c10c06cu;r[4]=1u;goto L8c10c078; /* bra 0x8c10c078 */
L8c10c06e: *stop=0x8c10c070u;r[4]=2u;goto L8c10c078; /* bra 0x8c10c078 */
L8c10c072: *stop=0x8c10c074u;r[4]=3u;goto L8c10c078; /* bra 0x8c10c078 */
L8c10c076: *stop=0x8c10c076u;r[4]=0u;goto L8c10c078; /* mov #0x0,r4 */
L8c10c078: *stop=0x8c10c078u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10c07a; /* lds.l @r15+,PR */
L8c10c07a: *stop=0x8c10c07au;if(*pr!=entry_pr)return -3;*stop=0x8c10c07cu;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_initialize_8c055c44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c055c44;
L8c055c44: *stop=0x8c055c44u;{uint32_t loaded=rd(m,bus,&io_status,0x8c055d08u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c055c46; /* mov.l 0x8c055d08,r1 */
L8c055c46: *stop=0x8c055c46u;r[2]=0u;goto L8c055c48; /* mov #0x0,r2 */
L8c055c48: *stop=0x8c055c48u;if(*pr!=entry_pr)return -3;*stop=0x8c055c4au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_initialize_8c603b84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c603b84;
L8c603b84: *stop=0x8c603b84u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c603b86; /* mov.l r14,@-r15 */
L8c603b86: *stop=0x8c603b86u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c603b88; /* sts.l PR,@-r15 */
L8c603b88: *stop=0x8c603b88u;r[15]+=4294967288u;goto L8c603b8a; /* add #-0x8,r15 */
L8c603b8a: *stop=0x8c603b8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c603bd8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603b8c; /* mov.l 0x8c603bd8,r3 */
L8c603b8c: target=r[3];*pr=0x8c603b90u;*stop=0x8c603b8eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603b90; /* jsr @r3 */
L8c603b90: *stop=0x8c603b90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c603bdcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c603b92; /* mov.l 0x8c603bdc,r4 */
L8c603b92: *stop=0x8c603b92u;r[14]=r[15];goto L8c603b94; /* mov r15,r14 */
L8c603b94: *stop=0x8c603b94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c603be0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603b96; /* mov.l 0x8c603be0,r3 */
L8c603b96: *stop=0x8c603b96u;r[6]=2u;goto L8c603b98; /* mov #0x2,r6 */
L8c603b98: *stop=0x8c603b98u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c603b9a; /* mov.l r0,@r15 */
L8c603b9a: *stop=0x8c603b9au;r[14]+=4u;goto L8c603b9c; /* add #0x4,r14 */
L8c603b9c: target=r[3];*pr=0x8c603ba0u;*stop=0x8c603b9eu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603ba0; /* jsr @r3 */
L8c603ba0: *stop=0x8c603ba0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c603ba2; /* tst r0,r0 */
L8c603ba2: if(!(*sr&1u))goto L8c603bca;goto L8c603ba4; /* bf 0x8c603bca */
L8c603ba4: *stop=0x8c603ba4u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[3]=loaded;}goto L8c603ba6; /* mov.b @r14,r3 */
L8c603ba6: *stop=0x8c603ba6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c603bd4u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c603ba8; /* mov.w 0x8c603bd4,r4 */
L8c603ba8: *stop=0x8c603ba8u;r[3]=r[3]&255u;goto L8c603baa; /* extu.b r3,r3 */
L8c603baa: *stop=0x8c603baau;*sr=(*sr&~1u)|(r[4]==r[3]);goto L8c603bac; /* cmp/eq r4,r3 */
L8c603bac: if(!(*sr&1u))goto L8c603bb6;goto L8c603bae; /* bf 0x8c603bb6 */
L8c603bae: *stop=0x8c603baeu;if(!valid_access(r[14]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c603bb0; /* mov.b @(0x1,r14),r0 */
L8c603bb0: *stop=0x8c603bb0u;r[0]=r[0]&255u;goto L8c603bb2; /* extu.b r0,r0 */
L8c603bb2: *stop=0x8c603bb2u;*sr=(*sr&~1u)|(r[4]==r[0]);goto L8c603bb4; /* cmp/eq r4,r0 */
L8c603bb4: if((*sr&1u))goto L8c603bca;goto L8c603bb6; /* bt 0x8c603bca */
L8c603bb6: *stop=0x8c603bb6u;if(!valid_access(r[14],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[3]=loaded;}goto L8c603bb8; /* mov.b @r14,r3 */
L8c603bb8: *stop=0x8c603bb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c603be4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c603bba; /* mov.l 0x8c603be4,r4 */
L8c603bba: *stop=0x8c603bbau;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c603bbc; /* mov.b r3,@r4 */
L8c603bbc: *stop=0x8c603bbcu;r[4]+=1u;goto L8c603bbe; /* add #0x1,r4 */
L8c603bbe: *stop=0x8c603bbeu;if(!valid_access(r[14]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c603bc0; /* mov.b @(0x1,r14),r0 */
L8c603bc0: *stop=0x8c603bc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c603be8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603bc2; /* mov.l 0x8c603be8,r3 */
L8c603bc2: *stop=0x8c603bc2u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[0],1);if(io_status)return io_status;goto L8c603bc4; /* mov.b r0,@r4 */
L8c603bc4: *stop=0x8c603bc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c603be4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c603bc6; /* mov.l 0x8c603be4,r4 */
L8c603bc6: target=r[3];*pr=0x8c603bcau;*stop=0x8c603bc8u;r[5]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603bca; /* jsr @r3 */
L8c603bca: *stop=0x8c603bcau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c603bcc; /* mov.l @r15,r0 */
L8c603bcc: *stop=0x8c603bccu;r[15]+=8u;goto L8c603bce; /* add #0x8,r15 */
L8c603bce: *stop=0x8c603bceu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c603bd0; /* lds.l @r15+,PR */
L8c603bd0: *stop=0x8c603bd0u;if(*pr!=entry_pr)return -3;*stop=0x8c603bd2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_initialize_8c10c0f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10c0f8;
L8c10c0f8: *stop=0x8c10c0f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10c104u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10c0fa; /* mov.l 0x8c10c104,r7 */
L8c10c0fa: *stop=0x8c10c0fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10c108u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10c0fc; /* mov.l 0x8c10c108,r0 */
L8c10c0fc: *stop=0x8c10c0fcu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10c0fe; /* mov.l @r0,r0 */
L8c10c0fe: target=r[0];*stop=0x8c10c100u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_movie_initialize_8c10c0d0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10c0d0;
L8c10c0d0: *stop=0x8c10c0d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10c0dcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10c0d2; /* mov.l 0x8c10c0dc,r7 */
L8c10c0d2: *stop=0x8c10c0d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10c0e0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10c0d4; /* mov.l 0x8c10c0e0,r0 */
L8c10c0d4: *stop=0x8c10c0d4u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10c0d6; /* mov.l @r0,r0 */
L8c10c0d6: target=r[0];*stop=0x8c10c0d8u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_movie_initialize_8c10da50(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10da50;
L8c10da50: *stop=0x8c10da50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10da5cu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10da52; /* mov.l 0x8c10da5c,r7 */
L8c10da52: *stop=0x8c10da52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10da60u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10da54; /* mov.l 0x8c10da60,r0 */
L8c10da54: *stop=0x8c10da54u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10da56; /* mov.l @r0,r0 */
L8c10da56: target=r[0];*stop=0x8c10da58u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}
