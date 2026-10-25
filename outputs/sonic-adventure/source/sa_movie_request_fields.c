/* Original request-record field providers; explicit machine/platform state.
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
SA_STAGE_EXPORT int sa_movie_request_fields_8c65a568(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65a568;
L8c65a568: *stop=0x8c65a568u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a56a; /* mov.l r14,@-r15 */
L8c65a56a: *stop=0x8c65a56au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c65a56c; /* tst r4,r4 */
L8c65a56c: *stop=0x8c65a56cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a56e; /* sts.l PR,@-r15 */
L8c65a56e: take=!(*sr&1u);*stop=0x8c65a570u;r[14]=0u;if(take)goto L8c65a578;goto L8c65a572; /* bf/s 0x8c65a578 */
L8c65a572: *stop=0x8c65a572u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a59cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a574; /* mov.l 0x8c65a59c,r14 */
L8c65a574: *stop=0x8c65a576u;goto L8c65a586; /* bra 0x8c65a586 */
L8c65a578: *stop=0x8c65a578u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c65a57a; /* tst r5,r5 */
L8c65a57a: if(!(*sr&1u))goto L8c65a582;goto L8c65a57c; /* bf 0x8c65a582 */
L8c65a57c: *stop=0x8c65a57cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a5a0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a57e; /* mov.l 0x8c65a5a0,r14 */
L8c65a57e: *stop=0x8c65a580u;goto L8c65a586; /* bra 0x8c65a586 */
L8c65a582: *stop=0x8c65a582u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a584; /* mov.l @(0x4,r4),r2 */
L8c65a584: *stop=0x8c65a584u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[2],4);if(io_status)return io_status;goto L8c65a586; /* mov.l r2,@r5 */
L8c65a586: *stop=0x8c65a586u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a588; /* tst r14,r14 */
L8c65a588: if((*sr&1u))goto L8c65a592;goto L8c65a58a; /* bt 0x8c65a592 */
L8c65a58a: *stop=0x8c65a58au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a5a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a58c; /* mov.l 0x8c65a5a8,r2 */
L8c65a58c: *stop=0x8c65a58cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a5a4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a58e; /* mov.l 0x8c65a5a4,r4 */
L8c65a58e: target=r[2];*pr=0x8c65a592u;*stop=0x8c65a590u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a592; /* jsr @r2 */
L8c65a592: *stop=0x8c65a592u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a594; /* lds.l @r15+,PR */
L8c65a594: *stop=0x8c65a594u;r[0]=r[14];goto L8c65a596; /* mov r14,r0 */
L8c65a596: *stop=0x8c65a596u;if(*pr!=entry_pr)return -3;*stop=0x8c65a598u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_request_fields_8c65a5ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a5ac;
L8c65a5ac: *stop=0x8c65a5acu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;goto L8c65a5ae; /* mov.l r5,@(0x8,r4) */
L8c65a5ae: *stop=0x8c65a5aeu;if(*pr!=entry_pr)return -3;*stop=0x8c65a5b0u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_request_fields_8c65a5cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a5cc;
L8c65a5cc: *stop=0x8c65a5ccu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c65a5ce; /* mov.l r5,@(0x4,r4) */
L8c65a5ce: *stop=0x8c65a5ceu;if(*pr!=entry_pr)return -3;*stop=0x8c65a5d0u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_request_fields_8c65a5d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a5d4;
L8c65a5d4: *stop=0x8c65a5d4u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[5],4);if(io_status)return io_status;goto L8c65a5d6; /* mov.l r5,@(0xc,r4) */
L8c65a5d6: *stop=0x8c65a5d6u;if(*pr!=entry_pr)return -3;*stop=0x8c65a5d8u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_request_fields_8c65a5b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65a5b4;
L8c65a5b4: *stop=0x8c65a5b4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c65a5b6; /* tst r4,r4 */
L8c65a5b6: take=!(*sr&1u);*stop=0x8c65a5b8u;r[7]=0u;if(take)goto L8c65a5c0;goto L8c65a5ba; /* bf/s 0x8c65a5c0 */
L8c65a5ba: *stop=0x8c65a5bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a5c8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c65a5bc; /* mov.l 0x8c65a5c8,r7 */
L8c65a5bc: *stop=0x8c65a5beu;goto L8c65a5c4; /* bra 0x8c65a5c4 */
L8c65a5c0: *stop=0x8c65a5c0u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[5],4);if(io_status)return io_status;goto L8c65a5c2; /* mov.l r5,@(0x10,r4) */
L8c65a5c2: *stop=0x8c65a5c2u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[6],4);if(io_status)return io_status;goto L8c65a5c4; /* mov.l r6,@(0x14,r4) */
L8c65a5c4: *stop=0x8c65a5c4u;if(*pr!=entry_pr)return -3;*stop=0x8c65a5c6u;r[0]=r[7];return 0; /* rts */
}
