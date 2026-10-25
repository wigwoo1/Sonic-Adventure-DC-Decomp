/* Original synchronous transfer/callback and error-counter bodies; explicit machine/platform state.
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
SA_STAGE_EXPORT int sa_movie_cpu_transfer_8c669088(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c669088;
L8c669088: *stop=0x8c669088u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c66908a; /* mov.l r14,@-r15 */
L8c66908a: *stop=0x8c66908au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c66908c; /* mov.l r13,@-r15 */
L8c66908c: *stop=0x8c66908cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66908e; /* sts.l PR,@-r15 */
L8c66908e: *stop=0x8c66908eu;r[15]+=4294967288u;goto L8c669090; /* add #-0x8,r15 */
L8c669090: *stop=0x8c669090u;r[3]=3u;goto L8c669092; /* mov #0x3,r3 */
L8c669092: *stop=0x8c669092u;r[13]=r[7];goto L8c669094; /* mov r7,r13 */
L8c669094: *stop=0x8c669094u;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c669096; /* tst r4,r3 */
L8c669096: *stop=0x8c669096u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c669098; /* mov.l r5,@(0x4,r15) */
L8c669098: *stop=0x8c669098u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c66909a; /* mov.l r6,@r15 */
L8c66909a: take=!(*sr&1u);*stop=0x8c66909cu;r[14]=0u;if(take)goto L8c6690a4;goto L8c66909e; /* bf/s 0x8c6690a4 */
L8c66909e: *stop=0x8c66909eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6690a0; /* mov.l @r15,r0 */
L8c6690a0: *stop=0x8c6690a0u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c6690a2; /* tst #0x3,r0 */
L8c6690a2: if((*sr&1u))goto L8c6690aa;goto L8c6690a4; /* bt 0x8c6690aa */
L8c6690a4: *stop=0x8c6690a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6690f4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6690a6; /* mov.l 0x8c6690f4,r14 */
L8c6690a6: *stop=0x8c6690a8u;goto L8c6690dc; /* bra 0x8c6690dc */
L8c6690aa: *stop=0x8c6690aau;r[3]=1u;goto L8c6690ac; /* mov #0x1,r3 */
L8c6690ac: *stop=0x8c6690acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6690ae; /* mov.l r3,@-r15 */
L8c6690ae: *stop=0x8c6690aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6690b0; /* mov.l r3,@-r15 */
L8c6690b0: *stop=0x8c6690b0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6690b2; /* mov.l @(0x8,r15),r7 */
L8c6690b2: *stop=0x8c6690b2u;*sr=(*sr&~1u)|((int32_t)r[7]>=0);goto L8c6690b4; /* cmp/pz r7 */
L8c6690b4: if((*sr&1u))goto L8c6690b8;goto L8c6690b6; /* bt 0x8c6690b8 */
L8c6690b6: *stop=0x8c6690b6u;r[7]+=3u;goto L8c6690b8; /* add #0x3,r7 */
L8c6690b8: *stop=0x8c6690b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6690f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6690ba; /* mov.l 0x8c6690f8,r2 */
L8c6690ba: *stop=0x8c6690bau;*sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u);goto L8c6690bc; /* shar r7 */
L8c6690bc: *stop=0x8c6690bcu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6690be; /* mov.l @(0xc,r15),r5 */
L8c6690be: *stop=0x8c6690beu;*sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u);goto L8c6690c0; /* shar r7 */
L8c6690c0: *stop=0x8c6690c0u;r[5]+=r[2];goto L8c6690c2; /* add r2,r5 */
L8c6690c2: *stop=0x8c6690c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6690fcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6690c4; /* mov.l 0x8c6690fc,r2 */
L8c6690c4: target=r[2];*pr=0x8c6690c8u;*stop=0x8c6690c6u;r[6]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6690c8; /* jsr @r2 */
L8c6690c8: *stop=0x8c6690c8u;r[15]+=8u;goto L8c6690ca; /* add #0x8,r15 */
L8c6690ca: *stop=0x8c6690cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c669100u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6690cc; /* mov.l 0x8c669100,r3 */
L8c6690cc: *stop=0x8c6690ccu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c6690ce; /* tst r13,r13 */
L8c6690ce: take=(*sr&1u);*stop=0x8c6690d0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;if(take)goto L8c6690dc;goto L8c6690d2; /* bt/s 0x8c6690dc */
L8c6690d2: *stop=0x8c6690d2u;r[0]=r[13];goto L8c6690d4; /* mov r13,r0 */
L8c6690d4: *stop=0x8c6690d4u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c6690d6; /* cmp/eq #-0x1,r0 */
L8c6690d6: if((*sr&1u))goto L8c6690dc;goto L8c6690d8; /* bt 0x8c6690dc */
L8c6690d8: target=r[13];*pr=0x8c6690dcu;*stop=0x8c6690dau;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6690dc; /* jsr @r13 */
L8c6690dc: *stop=0x8c6690dcu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6690de; /* tst r14,r14 */
L8c6690de: if((*sr&1u))goto L8c6690e8;goto L8c6690e0; /* bt 0x8c6690e8 */
L8c6690e0: *stop=0x8c6690e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c669108u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6690e2; /* mov.l 0x8c669108,r2 */
L8c6690e2: *stop=0x8c6690e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c669104u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6690e4; /* mov.l 0x8c669104,r4 */
L8c6690e4: target=r[2];*pr=0x8c6690e8u;*stop=0x8c6690e6u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6690e8; /* jsr @r2 */
L8c6690e8: *stop=0x8c6690e8u;r[0]=r[14];goto L8c6690ea; /* mov r14,r0 */
L8c6690ea: *stop=0x8c6690eau;r[15]+=8u;goto L8c6690ec; /* add #0x8,r15 */
L8c6690ec: *stop=0x8c6690ecu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6690ee; /* lds.l @r15+,PR */
L8c6690ee: *stop=0x8c6690eeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6690f0; /* mov.l @r15+,r13 */
L8c6690f0: *stop=0x8c6690f0u;if(*pr!=entry_pr)return -3;*stop=0x8c6690f2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_cpu_transfer_8c666e06(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c666e06;
L8c666e06: *stop=0x8c666e06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666e4cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c666e08; /* mov.l 0x8c666e4c,r4 */
L8c666e08: *stop=0x8c666e08u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666e0a; /* mov.l @r4,r3 */
L8c666e0a: *stop=0x8c666e0au;r[3]+=1u;goto L8c666e0c; /* add #0x1,r3 */
L8c666e0c: *stop=0x8c666e0cu;if(*pr!=entry_pr)return -3;*stop=0x8c666e0eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;return 0; /* rts */
}
