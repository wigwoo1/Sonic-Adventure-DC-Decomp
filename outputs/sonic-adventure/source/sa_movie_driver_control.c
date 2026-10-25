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
SA_STAGE_EXPORT int sa_movie_driver_control_8c65a200(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a200;
L8c65a200: *stop=0x8c65a200u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a202; /* mov.l r14,@-r15 */
L8c65a202: *stop=0x8c65a202u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a204; /* sts.l PR,@-r15 */
L8c65a204: *stop=0x8c65a204u;r[15]+=4294967292u;goto L8c65a206; /* add #-0x4,r15 */
L8c65a206: *stop=0x8c65a206u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65a208; /* stc SR,r0 */
L8c65a208: *stop=0x8c65a208u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a268u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65a20a; /* mov.w 0x8c65a268,r3 */
L8c65a20a: *stop=0x8c65a20au;r[0]>>=2;goto L8c65a20c; /* shlr2 r0 */
L8c65a20c: *stop=0x8c65a20cu;r[0]>>=2;goto L8c65a20e; /* shlr2 r0 */
L8c65a20e: *stop=0x8c65a20eu;r[0]&=0xfu;goto L8c65a210; /* and #0xf,r0 */
L8c65a210: *stop=0x8c65a210u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c65a212; /* mov.l r0,@r15 */
L8c65a212: *stop=0x8c65a212u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65a214; /* stc SR,r0 */
L8c65a214: *stop=0x8c65a214u;r[0]&=r[3];goto L8c65a216; /* and r3,r0 */
L8c65a216: *stop=0x8c65a216u;r[0]|=0xf0u;goto L8c65a218; /* or #0xf0,r0 */
L8c65a218: *stop=0x8c65a218u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a21a; /* ldc r0,SR */
L8c65a21a: *stop=0x8c65a21au;r[14]=1u;goto L8c65a21c; /* mov #0x1,r14 */
L8c65a21c: *stop=0x8c65a21cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a270u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a21e; /* mov.l 0x8c65a270,r4 */
L8c65a21e: *stop=0x8c65a21eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a26cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a220; /* mov.l 0x8c65a26c,r5 */
L8c65a220: *stop=0x8c65a220u;r[7]=r[14];goto L8c65a222; /* mov r14,r7 */
L8c65a222: *stop=0x8c65a222u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a224; /* mov.l r14,@-r15 */
L8c65a224: *stop=0x8c65a224u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a274u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a226; /* mov.l 0x8c65a274,r2 */
L8c65a226: *stop=0x8c65a226u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a228; /* mov.l r14,@-r15 */
L8c65a228: target=r[2];*pr=0x8c65a22cu;*stop=0x8c65a22au;r[6]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a22c; /* jsr @r2 */
L8c65a22c: *stop=0x8c65a22cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a26cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a22e; /* mov.l 0x8c65a26c,r3 */
L8c65a22e: *stop=0x8c65a22eu;r[7]=r[14];goto L8c65a230; /* mov r14,r7 */
L8c65a230: *stop=0x8c65a230u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a270u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a232; /* mov.l 0x8c65a270,r5 */
L8c65a232: *stop=0x8c65a232u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a234; /* mov.l @r3,r2 */
L8c65a234: *stop=0x8c65a234u;r[4]=r[3];goto L8c65a236; /* mov r3,r4 */
L8c65a236: *stop=0x8c65a236u;r[2]|=r[14];goto L8c65a238; /* or r14,r2 */
L8c65a238: *stop=0x8c65a238u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c65a23a; /* mov.l r2,@r3 */
L8c65a23a: *stop=0x8c65a23au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a278u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a23c; /* mov.l 0x8c65a278,r3 */
L8c65a23c: *stop=0x8c65a23cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a23e; /* mov.l r14,@-r15 */
L8c65a23e: *stop=0x8c65a23eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a240; /* mov.l r14,@-r15 */
L8c65a240: target=r[3];*pr=0x8c65a244u;*stop=0x8c65a242u;r[6]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a244; /* jsr @r3 */
L8c65a244: *stop=0x8c65a244u;r[15]+=16u;goto L8c65a246; /* add #0x10,r15 */
L8c65a246: *stop=0x8c65a246u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a248; /* mov.l @r15,r0 */
L8c65a248: *stop=0x8c65a248u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c65a24a; /* stc SR,r2 */
L8c65a24a: *stop=0x8c65a24au;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a268u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65a24c; /* mov.w 0x8c65a268,r3 */
L8c65a24c: *stop=0x8c65a24cu;r[0]&=0xfu;goto L8c65a24e; /* and #0xf,r0 */
L8c65a24e: *stop=0x8c65a24eu;r[0]<<=2;goto L8c65a250; /* shll2 r0 */
L8c65a250: *stop=0x8c65a250u;r[2]&=r[3];goto L8c65a252; /* and r3,r2 */
L8c65a252: *stop=0x8c65a252u;r[0]<<=2;goto L8c65a254; /* shll2 r0 */
L8c65a254: *stop=0x8c65a254u;r[0]|=r[2];goto L8c65a256; /* or r2,r0 */
L8c65a256: *stop=0x8c65a256u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a258; /* ldc r0,SR */
L8c65a258: *stop=0x8c65a258u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a26au,2);if(io_status)return io_status;r[4]=loaded;}goto L8c65a25a; /* mov.w 0x8c65a26a,r4 */
L8c65a25a: *stop=0x8c65a25au;r[4]--;*sr=(*sr&~1u)|(!r[4]);goto L8c65a25c; /* dt r4 */
L8c65a25c: if(!(*sr&1u))goto L8c65a25a;goto L8c65a25e; /* bf 0x8c65a25a */
L8c65a25e: *stop=0x8c65a25eu;r[0]=0u;goto L8c65a260; /* mov #0x0,r0 */
L8c65a260: *stop=0x8c65a260u;r[15]+=4u;goto L8c65a262; /* add #0x4,r15 */
L8c65a262: *stop=0x8c65a262u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a264; /* lds.l @r15+,PR */
L8c65a264: *stop=0x8c65a264u;if(*pr!=entry_pr)return -3;*stop=0x8c65a266u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
