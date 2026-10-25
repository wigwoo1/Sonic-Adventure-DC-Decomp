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
SA_STAGE_EXPORT int sa_movie_transfer_start_8c10faa4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10faa4;
L8c10faa4: *stop=0x8c10faa4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10faa6; /* mov.l r14,@-r15 */
L8c10faa6: *stop=0x8c10faa6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10faa8; /* mov.l r13,@-r15 */
L8c10faa8: *stop=0x8c10faa8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10faaa; /* mov.l r12,@-r15 */
L8c10faaa: *stop=0x8c10faaau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10faac; /* sts.l PR,@-r15 */
L8c10faac: *stop=0x8c10faacu;r[15]+=4294967288u;goto L8c10faae; /* add #-0x8,r15 */
L8c10faae: *stop=0x8c10faaeu;r[14]=r[4];goto L8c10fab0; /* mov r4,r14 */
L8c10fab0: *stop=0x8c10fab0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fb34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fab2; /* mov.l 0x8c10fb34,r3 */
L8c10fab2: *stop=0x8c10fab2u;r[12]=0u;goto L8c10fab4; /* mov #0x0,r12 */
L8c10fab4: *stop=0x8c10fab4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c10fab6; /* mov.l r5,@(0x4,r15) */
L8c10fab6: *stop=0x8c10fab6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fab8; /* mov.l @r14,r2 */
L8c10fab8: *stop=0x8c10fab8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[13]=loaded;}goto L8c10faba; /* mov.l @r3,r13 */
L8c10faba: *stop=0x8c10fabau;r[2]<<=2;goto L8c10fabc; /* shll2 r2 */
L8c10fabc: *stop=0x8c10fabcu;r[2]<<=2;goto L8c10fabe; /* shll2 r2 */
L8c10fabe: *stop=0x8c10fabeu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[13]=loaded;}goto L8c10fac0; /* mov.l @r13,r13 */
L8c10fac0: *stop=0x8c10fac0u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c10fac2; /* shll r2 */
L8c10fac2: *stop=0x8c10fac2u;r[13]+=r[2];goto L8c10fac4; /* add r2,r13 */
L8c10fac4: *stop=0x8c10fac4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fb38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fac6; /* mov.l 0x8c10fb38,r2 */
L8c10fac6: target=r[2];*pr=0x8c10facau;*stop=0x8c10fac8u;r[5]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10faca; /* jsr @r2 */
L8c10faca: *stop=0x8c10facau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10facc; /* mov.l @r15,r0 */
L8c10facc: *stop=0x8c10faccu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10face; /* cmp/eq #0x1,r0 */
L8c10face: if(!(*sr&1u))goto L8c10fad6;goto L8c10fad0; /* bf 0x8c10fad6 */
L8c10fad0: *stop=0x8c10fad0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fb3cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c10fad2; /* mov.l 0x8c10fb3c,r12 */
L8c10fad2: *stop=0x8c10fad4u;goto L8c10fb24; /* bra 0x8c10fb24 */
L8c10fad6: *stop=0x8c10fad6u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c10fad8; /* mov.l @(0xc,r14),r5 */
L8c10fad8: *stop=0x8c10fad8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fb40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fada; /* mov.l 0x8c10fb40,r3 */
L8c10fada: *stop=0x8c10fadau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fb44u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fadc; /* mov.l 0x8c10fb44,r2 */
L8c10fadc: *stop=0x8c10fadcu;r[5]&=r[3];goto L8c10fade; /* and r3,r5 */
L8c10fade: target=r[2];*pr=0x8c10fae2u;*stop=0x8c10fae0u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10fae2; /* jsr @r2 */
L8c10fae2: *stop=0x8c10fae2u;r[3]=0u;goto L8c10fae4; /* mov #0x0,r3 */
L8c10fae4: *stop=0x8c10fae4u;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[3],4);if(io_status)return io_status;goto L8c10fae6; /* mov.l r3,@(0x14,r13) */
L8c10fae6: *stop=0x8c10fae6u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fae8; /* mov.l @(0x4,r14),r2 */
L8c10fae8: *stop=0x8c10fae8u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c10faea; /* mov.l r2,@r13 */
L8c10faea: *stop=0x8c10faeau;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10faec; /* mov.l @(0x8,r14),r3 */
L8c10faec: *stop=0x8c10faecu;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[3],4);if(io_status)return io_status;goto L8c10faee; /* mov.l r3,@(0x4,r13) */
L8c10faee: *stop=0x8c10faeeu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10faf0; /* mov.l @(0xc,r14),r2 */
L8c10faf0: *stop=0x8c10faf0u;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[2],4);if(io_status)return io_status;goto L8c10faf2; /* mov.l r2,@(0x8,r13) */
L8c10faf2: *stop=0x8c10faf2u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10faf4; /* mov.l @(0x10,r14),r3 */
L8c10faf4: *stop=0x8c10faf4u;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[3],4);if(io_status)return io_status;goto L8c10faf6; /* mov.l r3,@(0xc,r13) */
L8c10faf6: *stop=0x8c10faf6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10faf8; /* mov.l @(0x4,r15),r1 */
L8c10faf8: *stop=0x8c10faf8u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c10fafa; /* tst r1,r1 */
L8c10fafa: if(!(*sr&1u))goto L8c10fb00;goto L8c10fafc; /* bf 0x8c10fb00 */
L8c10fafc: *stop=0x8c10fafeu;r[2]=0u;goto L8c10fb02; /* bra 0x8c10fb02 */
L8c10fb00: *stop=0x8c10fb00u;r[2]=2u;goto L8c10fb02; /* mov #0x2,r2 */
L8c10fb02: *stop=0x8c10fb02u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10fb04; /* mov.l @(0x14,r14),r1 */
L8c10fb04: *stop=0x8c10fb04u;r[4]=1u;goto L8c10fb06; /* mov #0x1,r4 */
L8c10fb06: *stop=0x8c10fb06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10fb34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10fb08; /* mov.l 0x8c10fb34,r3 */
L8c10fb08: *stop=0x8c10fb08u;r[2]|=r[1];goto L8c10fb0a; /* or r1,r2 */
L8c10fb0a: *stop=0x8c10fb0au;{uint32_t loaded=sx(m,bus,&io_status,0x8c10fb32u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10fb0c; /* mov.w 0x8c10fb32,r1 */
L8c10fb0c: *stop=0x8c10fb0cu;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[2],4);if(io_status)return io_status;goto L8c10fb0e; /* mov.l r2,@(0x10,r13) */
L8c10fb0e: *stop=0x8c10fb0eu;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[4],4);if(io_status)return io_status;goto L8c10fb10; /* mov.l r4,@(0x14,r13) */
L8c10fb10: *stop=0x8c10fb10u;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[4],4);if(io_status)return io_status;goto L8c10fb12; /* mov.l r4,@(0x2c,r14) */
L8c10fb12: *stop=0x8c10fb12u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10fb14; /* mov.l @r3,r0 */
L8c10fb14: *stop=0x8c10fb14u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10fb16; /* mov.l @r14,r2 */
L8c10fb16: *stop=0x8c10fb16u;r[1]+=r[0];goto L8c10fb18; /* add r0,r1 */
L8c10fb18: *stop=0x8c10fb18u;r[2]<<=2;goto L8c10fb1a; /* shll2 r2 */
L8c10fb1a: *stop=0x8c10fb1au;r[2]+=r[1];goto L8c10fb1c; /* add r1,r2 */
L8c10fb1c: *stop=0x8c10fb1cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10fb1e; /* mov.l @r2,r0 */
L8c10fb1e: *stop=0x8c10fb1eu;r[0]+=1u;goto L8c10fb20; /* add #0x1,r0 */
L8c10fb20: *stop=0x8c10fb20u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c10fb22; /* mov.l r0,@r2 */
L8c10fb22: *stop=0x8c10fb22u;if(!valid_access(r[13]+24u,4u))return -3;wr(m,bus,&io_status,r[13]+0x18u,r[4],4);if(io_status)return io_status;goto L8c10fb24; /* mov.l r4,@(0x18,r13) */
L8c10fb24: *stop=0x8c10fb24u;r[0]=r[12];goto L8c10fb26; /* mov r12,r0 */
L8c10fb26: *stop=0x8c10fb26u;r[15]+=8u;goto L8c10fb28; /* add #0x8,r15 */
L8c10fb28: *stop=0x8c10fb28u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10fb2a; /* lds.l @r15+,PR */
L8c10fb2a: *stop=0x8c10fb2au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10fb2c; /* mov.l @r15+,r12 */
L8c10fb2c: *stop=0x8c10fb2cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10fb2e; /* mov.l @r15+,r13 */
L8c10fb2e: *stop=0x8c10fb2eu;if(*pr!=entry_pr)return -3;*stop=0x8c10fb30u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_transfer_start_8c66abfc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c66abfc;
L8c66abfc: *stop=0x8c66abfcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66ac2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66abfe; /* mov.l 0x8c66ac2c,r3 */
L8c66abfe: *stop=0x8c66abfeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c66ac00; /* mov.l r14,@-r15 */
L8c66ac00: *stop=0x8c66ac00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66ac02; /* sts.l PR,@-r15 */
L8c66ac02: *stop=0x8c66ac02u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c66ac04; /* mov.l @r3,r5 */
L8c66ac04: *stop=0x8c66ac04u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c66ac06; /* tst r5,r5 */
L8c66ac06: take=!(*sr&1u);*stop=0x8c66ac08u;r[14]=0u;if(take)goto L8c66ac10;goto L8c66ac0a; /* bf/s 0x8c66ac10 */
L8c66ac0a: *stop=0x8c66ac0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c66ac30u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c66ac0c; /* mov.l 0x8c66ac30,r14 */
L8c66ac0c: *stop=0x8c66ac0eu;goto L8c66ac16; /* bra 0x8c66ac16 */
L8c66ac10: *stop=0x8c66ac10u;{uint32_t loaded=sx(m,bus,&io_status,0x8c66ac2au,2);if(io_status)return io_status;r[0]=loaded;}goto L8c66ac12; /* mov.w 0x8c66ac2a,r0 */
L8c66ac12: *stop=0x8c66ac12u;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66ac14; /* mov.l @(r0,r5),r3 */
L8c66ac14: *stop=0x8c66ac14u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c66ac16; /* mov.l r3,@r4 */
L8c66ac16: *stop=0x8c66ac16u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c66ac18; /* tst r14,r14 */
L8c66ac18: if((*sr&1u))goto L8c66ac22;goto L8c66ac1a; /* bt 0x8c66ac22 */
L8c66ac1a: *stop=0x8c66ac1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c66ac38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66ac1c; /* mov.l 0x8c66ac38,r3 */
L8c66ac1c: *stop=0x8c66ac1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66ac34u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c66ac1e; /* mov.l 0x8c66ac34,r4 */
L8c66ac1e: target=r[3];*pr=0x8c66ac22u;*stop=0x8c66ac20u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66ac22; /* jsr @r3 */
L8c66ac22: *stop=0x8c66ac22u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66ac24; /* lds.l @r15+,PR */
L8c66ac24: *stop=0x8c66ac24u;r[0]=r[14];goto L8c66ac26; /* mov r14,r0 */
L8c66ac26: *stop=0x8c66ac26u;if(*pr!=entry_pr)return -3;*stop=0x8c66ac28u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_transfer_start_8c668f40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c668f40;
L8c668f40: *stop=0x8c668f40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c668f42; /* mov.l r14,@-r15 */
L8c668f42: *stop=0x8c668f42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c668f44; /* sts.l PR,@-r15 */
L8c668f44: *stop=0x8c668f44u;r[15]+=4294967280u;goto L8c668f46; /* add #-0x10,r15 */
L8c668f46: *stop=0x8c668f46u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c668f48; /* stc SR,r0 */
L8c668f48: *stop=0x8c668f48u;{uint32_t loaded=sx(m,bus,&io_status,0x8c668fdau,2);if(io_status)return io_status;r[3]=loaded;}goto L8c668f4a; /* mov.w 0x8c668fda,r3 */
L8c668f4a: *stop=0x8c668f4au;r[0]>>=2;goto L8c668f4c; /* shlr2 r0 */
L8c668f4c: *stop=0x8c668f4cu;r[0]>>=2;goto L8c668f4e; /* shlr2 r0 */
L8c668f4e: *stop=0x8c668f4eu;r[0]&=0xfu;goto L8c668f50; /* and #0xf,r0 */
L8c668f50: *stop=0x8c668f50u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c668f52; /* mov.l r0,@(0xc,r15) */
L8c668f52: *stop=0x8c668f52u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c668f54; /* stc SR,r0 */
L8c668f54: *stop=0x8c668f54u;r[0]&=r[3];goto L8c668f56; /* and r3,r0 */
L8c668f56: *stop=0x8c668f56u;r[0]|=0xf0u;goto L8c668f58; /* or #0xf0,r0 */
L8c668f58: *stop=0x8c668f58u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c668f5a; /* ldc r0,SR */
L8c668f5a: *stop=0x8c668f5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c668fe4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c668f5c; /* mov.l 0x8c668fe4,r14 */
L8c668f5c: *stop=0x8c668f5cu;r[2]=1u;goto L8c668f5e; /* mov #0x1,r2 */
L8c668f5e: *stop=0x8c668f5eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c668fdcu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c668f60; /* mov.w 0x8c668fdc,r0 */
L8c668f60: *stop=0x8c668f60u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[2],4);if(io_status)return io_status;goto L8c668f62; /* mov.l r2,@(0x4,r4) */
L8c668f62: *stop=0x8c668f62u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668f64; /* mov.l @r14,r3 */
L8c668f64: *stop=0x8c668f64u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668f66; /* mov.l @(r0,r3),r2 */
L8c668f66: *stop=0x8c668f66u;r[2]+=1u;goto L8c668f68; /* add #0x1,r2 */
L8c668f68: *stop=0x8c668f68u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c668f6a; /* mov.l r2,@(r0,r3) */
L8c668f6a: *stop=0x8c668f6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c668fe8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668f6c; /* mov.l 0x8c668fe8,r3 */
L8c668f6c: target=r[3];*pr=0x8c668f70u;*stop=0x8c668f6eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668f70; /* jsr @r3 */
L8c668f70: *stop=0x8c668f70u;{uint32_t loaded=sx(m,bus,&io_status,0x8c668fdeu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c668f72; /* mov.w 0x8c668fde,r0 */
L8c668f72: *stop=0x8c668f72u;r[3]=0u;goto L8c668f74; /* mov #0x0,r3 */
L8c668f74: *stop=0x8c668f74u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668f76; /* mov.l @r14,r2 */
L8c668f76: *stop=0x8c668f76u;r[4]=r[15];goto L8c668f78; /* mov r15,r4 */
L8c668f78: *stop=0x8c668f78u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[3],4);if(io_status)return io_status;goto L8c668f7a; /* mov.l r3,@(r0,r2) */
L8c668f7a: *stop=0x8c668f7au;r[0]+=4294967292u;goto L8c668f7c; /* add #-0x4,r0 */
L8c668f7c: *stop=0x8c668f7cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668f7e; /* mov.l @r14,r2 */
L8c668f7e: *stop=0x8c668f7eu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668f80; /* mov.l @(r0,r2),r3 */
L8c668f80: *stop=0x8c668f80u;r[3]+=1u;goto L8c668f82; /* add #0x1,r3 */
L8c668f82: *stop=0x8c668f82u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[3],4);if(io_status)return io_status;goto L8c668f84; /* mov.l r3,@(r0,r2) */
L8c668f84: *stop=0x8c668f84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668fecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c668f86; /* mov.l 0x8c668fec,r2 */
L8c668f86: target=r[2];*pr=0x8c668f8au;*stop=0x8c668f88u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668f8a; /* jsr @r2 */
L8c668f8a: *stop=0x8c668f8au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668f8c; /* mov.l @(0x8,r15),r3 */
L8c668f8c: *stop=0x8c668f8cu;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c668f8e; /* cmp/pl r3 */
L8c668f8e: if(!(*sr&1u))goto L8c668fc0;goto L8c668f90; /* bf 0x8c668fc0 */
L8c668f90: *stop=0x8c668f90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668ff0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668f92; /* mov.l 0x8c668ff0,r3 */
L8c668f92: *stop=0x8c668f92u;r[5]=r[15];goto L8c668f94; /* mov r15,r5 */
L8c668f94: *stop=0x8c668f94u;r[5]+=4u;goto L8c668f96; /* add #0x4,r5 */
L8c668f96: target=r[3];*pr=0x8c668f9au;*stop=0x8c668f98u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668f9a; /* jsr @r3 */
L8c668f9a: *stop=0x8c668f9au;{uint32_t loaded=sx(m,bus,&io_status,0x8c668fe0u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c668f9c; /* mov.w 0x8c668fe0,r1 */
L8c668f9c: *stop=0x8c668f9cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c668f9e; /* mov.l @r14,r0 */
L8c668f9e: *stop=0x8c668f9eu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c668fa0; /* mov.l @(r0,r1),r0 */
L8c668fa0: *stop=0x8c668fa0u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c668fa2; /* cmp/eq #0x0,r0 */
L8c668fa2: if((*sr&1u))goto L8c668fac;goto L8c668fa4; /* bt 0x8c668fac */
L8c668fa4: *stop=0x8c668fa4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c668fa6; /* cmp/eq #0x1,r0 */
L8c668fa6: if((*sr&1u))goto L8c668fb8;goto L8c668fa8; /* bt 0x8c668fb8 */
L8c668fa8: *stop=0x8c668faau;goto L8c668fc0; /* bra 0x8c668fc0 */
L8c668fac: *stop=0x8c668facu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668ff4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668fae; /* mov.l 0x8c668ff4,r3 */
L8c668fae: *stop=0x8c668faeu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c668fb0; /* mov.l @(0x4,r15),r5 */
L8c668fb0: target=r[3];*pr=0x8c668fb4u;*stop=0x8c668fb2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668fb4; /* jsr @r3 */
L8c668fb4: *stop=0x8c668fb6u;goto L8c668fc0; /* bra 0x8c668fc0 */
L8c668fb8: *stop=0x8c668fb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668ff8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668fba; /* mov.l 0x8c668ff8,r3 */
L8c668fba: *stop=0x8c668fbau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c668fbc; /* mov.l @(0x4,r15),r5 */
L8c668fbc: target=r[3];*pr=0x8c668fc0u;*stop=0x8c668fbeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668fc0; /* jsr @r3 */
L8c668fc0: *stop=0x8c668fc0u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c668fc2; /* mov.l @(0xc,r15),r0 */
L8c668fc2: *stop=0x8c668fc2u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c668fc4; /* stc SR,r3 */
L8c668fc4: *stop=0x8c668fc4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c668fdau,2);if(io_status)return io_status;r[2]=loaded;}goto L8c668fc6; /* mov.w 0x8c668fda,r2 */
L8c668fc6: *stop=0x8c668fc6u;r[0]&=0xfu;goto L8c668fc8; /* and #0xf,r0 */
L8c668fc8: *stop=0x8c668fc8u;r[0]<<=2;goto L8c668fca; /* shll2 r0 */
L8c668fca: *stop=0x8c668fcau;r[3]&=r[2];goto L8c668fcc; /* and r2,r3 */
L8c668fcc: *stop=0x8c668fccu;r[0]<<=2;goto L8c668fce; /* shll2 r0 */
L8c668fce: *stop=0x8c668fceu;r[0]|=r[3];goto L8c668fd0; /* or r3,r0 */
L8c668fd0: *stop=0x8c668fd0u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c668fd2; /* ldc r0,SR */
L8c668fd2: *stop=0x8c668fd2u;r[15]+=16u;goto L8c668fd4; /* add #0x10,r15 */
L8c668fd4: *stop=0x8c668fd4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c668fd6; /* lds.l @r15+,PR */
L8c668fd6: *stop=0x8c668fd6u;if(*pr!=entry_pr)return -3;*stop=0x8c668fd8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_transfer_start_8c5fc58a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c5fc58a;
L8c5fc58a: *stop=0x8c5fc58au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c5fc58c; /* mov.l r14,@-r15 */
L8c5fc58c: *stop=0x8c5fc58cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c5fc58e; /* mov.l r13,@-r15 */
L8c5fc58e: *stop=0x8c5fc58eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c5fc590; /* mov.l r12,@-r15 */
L8c5fc590: *stop=0x8c5fc590u;r[15]+=4294967288u;goto L8c5fc592; /* add #-0x8,r15 */
L8c5fc592: *stop=0x8c5fc592u;r[13]=r[5];goto L8c5fc594; /* mov r5,r13 */
L8c5fc594: *stop=0x8c5fc594u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c5fc596; /* tst r13,r13 */
L8c5fc596: take=!(*sr&1u);*stop=0x8c5fc598u;r[12]=r[4];if(take)goto L8c5fc5a0;goto L8c5fc59a; /* bf/s 0x8c5fc5a0 */
L8c5fc59a: *stop=0x8c5fc59au;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc5c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc59c; /* mov.l 0x8c5fc5c0,r3 */
L8c5fc59c: target=r[3];*stop=0x8c5fc59eu;if(target==0x8c5fc7d4u)goto L8c5fc7d4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c5fc5a0: *stop=0x8c5fc5a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc5c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc5a2; /* mov.l 0x8c5fc5c8,r2 */
L8c5fc5a2: *stop=0x8c5fc5a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc5c4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c5fc5a4; /* mov.l 0x8c5fc5c4,r14 */
L8c5fc5a4: *stop=0x8c5fc5a4u;*sr=(*sr&~1u)|(r[13]>=r[2]);goto L8c5fc5a6; /* cmp/hs r2,r13 */
L8c5fc5a6: take=!(*sr&1u);*stop=0x8c5fc5a8u;r[14]&=r[12];if(take)goto L8c5fc5b0;goto L8c5fc5aa; /* bf/s 0x8c5fc5b0 */
L8c5fc5aa: *stop=0x8c5fc5aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc5ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc5ac; /* mov.l 0x8c5fc5cc,r3 */
L8c5fc5ac: target=r[3];*stop=0x8c5fc5aeu;if(target==0x8c5fc5e6u)goto L8c5fc5e6;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c5fc5b0: *stop=0x8c5fc5b0u;r[5]=31u;goto L8c5fc5b2; /* mov #0x1f,r5 */
L8c5fc5b2: *stop=0x8c5fc5b2u;r[4]=r[14];goto L8c5fc5b4; /* mov r14,r4 */
L8c5fc5b4: *stop=0x8c5fc5b4u;r[5]&=r[12];goto L8c5fc5b6; /* and r12,r5 */
L8c5fc5b6: *stop=0x8c5fc5b6u;r[5]+=r[13];goto L8c5fc5b8; /* add r13,r5 */
L8c5fc5b8: *stop=0x8c5fc5bau;goto L8c5fc5d0; /* bra 0x8c5fc5d0 */
L8c5fc5d0: *stop=0x8c5fc5d0u;r[5]+=31u;goto L8c5fc5d2; /* add #0x1f,r5 */
L8c5fc5d2: *stop=0x8c5fc5d2u;r[5]>>=2;goto L8c5fc5d4; /* shlr2 r5 */
L8c5fc5d4: *stop=0x8c5fc5d4u;r[5]>>=2;goto L8c5fc5d6; /* shlr2 r5 */
L8c5fc5d6: *stop=0x8c5fc5d6u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c5fc5d8; /* shlr r5 */
L8c5fc5d8: *stop=0x8c5fc5d8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c5fc5da; /* ocbwb @r4 */
L8c5fc5da: *stop=0x8c5fc5dau;r[5]--;*sr=(*sr&~1u)|(!r[5]);goto L8c5fc5dc; /* dt r5 */
L8c5fc5dc: take=!(*sr&1u);*stop=0x8c5fc5deu;r[4]+=32u;if(take)goto L8c5fc5d8;goto L8c5fc5e0; /* bf/s 0x8c5fc5d8 */
L8c5fc5e0: *stop=0x8c5fc5e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc600u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc5e2; /* mov.l 0x8c5fc600,r2 */
L8c5fc5e2: target=r[2];*stop=0x8c5fc5e4u;if(target==0x8c5fc7d4u)goto L8c5fc7d4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c5fc5e6: *stop=0x8c5fc5e6u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5fc5e8; /* stc SR,r0 */
L8c5fc5e8: *stop=0x8c5fc5e8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc5feu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc5ea; /* mov.w 0x8c5fc5fe,r3 */
L8c5fc5ea: *stop=0x8c5fc5eau;r[0]>>=2;goto L8c5fc5ec; /* shlr2 r0 */
L8c5fc5ec: *stop=0x8c5fc5ecu;r[0]>>=2;goto L8c5fc5ee; /* shlr2 r0 */
L8c5fc5ee: *stop=0x8c5fc5eeu;r[0]&=0xfu;goto L8c5fc5f0; /* and #0xf,r0 */
L8c5fc5f0: *stop=0x8c5fc5f0u;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c5fc5f2; /* mov.w r0,@r15 */
L8c5fc5f2: *stop=0x8c5fc5f2u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5fc5f4; /* stc SR,r0 */
L8c5fc5f4: *stop=0x8c5fc5f4u;r[0]&=r[3];goto L8c5fc5f6; /* and r3,r0 */
L8c5fc5f6: *stop=0x8c5fc5f6u;r[0]|=0xf0u;goto L8c5fc5f8; /* or #0xf0,r0 */
L8c5fc5f8: *stop=0x8c5fc5f8u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c5fc5fa; /* ldc r0,SR */
L8c5fc5fa: *stop=0x8c5fc5fcu;goto L8c5fc604; /* bra 0x8c5fc604 */
L8c5fc604: *stop=0x8c5fc604u;r[0]=0x8c5fc614u;goto L8c5fc606; /* mova 0x8c5fc614,r0 */
L8c5fc606: *stop=0x8c5fc606u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc610u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c5fc608; /* mov.l 0x8c5fc610,r1 */
L8c5fc608: *stop=0x8c5fc608u;r[1]|=r[0];goto L8c5fc60a; /* or r0,r1 */
L8c5fc60a: target=r[1];*stop=0x8c5fc60cu;if(target==0xac5fc614u)goto Lac5fc614;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r1 */
L8c5fc746: *stop=0x8c5fc746u;r[4]=r[14];goto L8c5fc748; /* mov r14,r4 */
L8c5fc748: *stop=0x8c5fc748u;r[5]=r[12];goto L8c5fc74a; /* mov r12,r5 */
L8c5fc74a: *stop=0x8c5fc74au;r[6]=r[13];goto L8c5fc74c; /* mov r13,r6 */
L8c5fc74c: *stop=0x8c5fc74eu;goto L8c5fc754; /* bra 0x8c5fc754 */
L8c5fc754: *stop=0x8c5fc754u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c5fc756; /* mov.l r11,@-r15 */
L8c5fc756: *stop=0x8c5fc756u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c5fc758; /* mov.l r10,@-r15 */
L8c5fc758: *stop=0x8c5fc758u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c5fc75a; /* mov.l r9,@-r15 */
L8c5fc75a: *stop=0x8c5fc75au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c5fc75c; /* mov.l r8,@-r15 */
L8c5fc75c: *stop=0x8c5fc75cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc778u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c5fc75e; /* mov.l 0x8c5fc778,r0 */
L8c5fc75e: *stop=0x8c5fc75eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc77cu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c5fc760; /* mov.l 0x8c5fc77c,r8 */
L8c5fc760: *stop=0x8c5fc760u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc780u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c5fc762; /* mov.l 0x8c5fc780,r9 */
L8c5fc762: *stop=0x8c5fc762u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc784u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c5fc764; /* mov.l 0x8c5fc784,r10 */
L8c5fc764: *stop=0x8c5fc764u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc788u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc766; /* mov.l 0x8c5fc788,r2 */
L8c5fc766: *stop=0x8c5fc766u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc78cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc768; /* mov.l 0x8c5fc78c,r3 */
L8c5fc768: *stop=0x8c5fc768u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc790u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c5fc76a; /* mov.l 0x8c5fc790,r7 */
L8c5fc76a: *stop=0x8c5fc76au;r[7]&=r[4];goto L8c5fc76c; /* and r4,r7 */
L8c5fc76c: *stop=0x8c5fc76cu;r[7]=~r[7];goto L8c5fc76e; /* not r7,r7 */
L8c5fc76e: *stop=0x8c5fc76eu;r[4]&=r[7];goto L8c5fc770; /* and r7,r4 */
L8c5fc770: *stop=0x8c5fc770u;r[7]=~r[7];goto L8c5fc772; /* not r7,r7 */
L8c5fc772: *stop=0x8c5fc774u;goto L8c5fc7b4; /* bra 0x8c5fc7b4 */
L8c5fc794: *stop=0x8c5fc794u;r[1]=r[0];goto L8c5fc796; /* mov r0,r1 */
L8c5fc796: *stop=0x8c5fc796u;r[1]|=r[8];goto L8c5fc798; /* or r8,r1 */
L8c5fc798: *stop=0x8c5fc798u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto L8c5fc79a; /* mov.l @r1,r1 */
L8c5fc79a: *stop=0x8c5fc79au;r[1]&=r[9];goto L8c5fc79c; /* and r9,r1 */
L8c5fc79c: *stop=0x8c5fc79cu;r[1]|=r[0];goto L8c5fc79e; /* or r0,r1 */
L8c5fc79e: *stop=0x8c5fc79eu;*sr=(*sr&~1u)|(r[4]>r[1]);goto L8c5fc7a0; /* cmp/hi r1,r4 */
L8c5fc7a0: if((*sr&1u))goto L8c5fc7aa;goto L8c5fc7a2; /* bt 0x8c5fc7aa */
L8c5fc7a2: *stop=0x8c5fc7a2u;*sr=(*sr&~1u)|(r[1]>r[5]);goto L8c5fc7a4; /* cmp/hi r5,r1 */
L8c5fc7a4: if((*sr&1u))goto L8c5fc7aa;goto L8c5fc7a6; /* bt 0x8c5fc7aa */
L8c5fc7a6: *stop=0x8c5fc7a6u;r[1]|=r[7];goto L8c5fc7a8; /* or r7,r1 */
L8c5fc7a8: *stop=0x8c5fc7a8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[1],2u);if(status)return status;goto L8c5fc7aa; /* ocbwb @r1 */
L8c5fc7aa: *stop=0x8c5fc7aau;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c5fc7ac; /* tst r6,r6 */
L8c5fc7ac: if((*sr&1u))goto L8c5fc7b2;goto L8c5fc7ae; /* bt 0x8c5fc7b2 */
L8c5fc7ae: *stop=0x8c5fc7aeu;*sr=(*sr&~1u)|(r[0]==r[3]);goto L8c5fc7b0; /* cmp/eq r0,r3 */
L8c5fc7b0: if((*sr&1u))goto L8c5fc7b8;goto L8c5fc7b2; /* bt 0x8c5fc7b8 */
L8c5fc7b2: *stop=0x8c5fc7b2u;r[0]+=32u;goto L8c5fc7b4; /* add #0x20,r0 */
L8c5fc7b4: *stop=0x8c5fc7b4u;*sr=(*sr&~1u)|(r[2]>r[0]);goto L8c5fc7b6; /* cmp/hi r0,r2 */
L8c5fc7b6: if((*sr&1u))goto L8c5fc794;goto L8c5fc7b8; /* bt 0x8c5fc794 */
L8c5fc7b8: *stop=0x8c5fc7b8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c5fc7ba; /* mov.l @r15+,r8 */
L8c5fc7ba: *stop=0x8c5fc7bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c5fc7bc; /* mov.l @r15+,r9 */
L8c5fc7bc: *stop=0x8c5fc7bcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c5fc7be; /* mov.l @r15+,r10 */
L8c5fc7be: *stop=0x8c5fc7beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c5fc7c0; /* mov.l @r15+,r11 */
L8c5fc7c0: *stop=0x8c5fc7c0u;if(!valid_access(r[15],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;r[0]=loaded;}goto L8c5fc7c2; /* mov.w @r15,r0 */
L8c5fc7c2: *stop=0x8c5fc7c2u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c5fc7c4; /* stc SR,r3 */
L8c5fc7c4: *stop=0x8c5fc7c4u;r[0]=r[0]&65535u;goto L8c5fc7c6; /* extu.w r0,r0 */
L8c5fc7c6: *stop=0x8c5fc7c6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc7f4u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc7c8; /* mov.w 0x8c5fc7f4,r2 */
L8c5fc7c8: *stop=0x8c5fc7c8u;r[0]&=0xfu;goto L8c5fc7ca; /* and #0xf,r0 */
L8c5fc7ca: *stop=0x8c5fc7cau;r[0]<<=2;goto L8c5fc7cc; /* shll2 r0 */
L8c5fc7cc: *stop=0x8c5fc7ccu;r[3]&=r[2];goto L8c5fc7ce; /* and r2,r3 */
L8c5fc7ce: *stop=0x8c5fc7ceu;r[0]<<=2;goto L8c5fc7d0; /* shll2 r0 */
L8c5fc7d0: *stop=0x8c5fc7d0u;r[0]|=r[3];goto L8c5fc7d2; /* or r3,r0 */
L8c5fc7d2: *stop=0x8c5fc7d2u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c5fc7d4; /* ldc r0,SR */
L8c5fc7d4: *stop=0x8c5fc7d4u;r[15]+=8u;goto L8c5fc7d6; /* add #0x8,r15 */
L8c5fc7d6: *stop=0x8c5fc7d6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c5fc7d8; /* mov.l @r15+,r12 */
L8c5fc7d8: *stop=0x8c5fc7d8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c5fc7da; /* mov.l @r15+,r13 */
L8c5fc7da: *stop=0x8c5fc7dau;if(*pr!=entry_pr)return -3;*stop=0x8c5fc7dcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
Lac5fc614: *stop=0xac5fc614u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc644u,4);if(io_status)return io_status;r[2]=loaded;}goto Lac5fc616; /* mov.l 0xac5fc644,r2 */
Lac5fc616: *stop=0xac5fc616u;r[12]+=r[13];goto Lac5fc618; /* add r13,r12 */
Lac5fc618: *stop=0xac5fc618u;r[1]=4294967289u;goto Lac5fc61a; /* mov #-0x7,r1 */
Lac5fc61a: *stop=0xac5fc61au;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc640u,4);if(io_status)return io_status;r[3]=loaded;}goto Lac5fc61c; /* mov.l 0xac5fc640,r3 */
Lac5fc61c: *stop=0xac5fc61cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto Lac5fc61e; /* mov.l @r2,r4 */
Lac5fc61e: *stop=0xac5fc61eu;r[5]=1u;goto Lac5fc620; /* mov #0x1,r5 */
Lac5fc620: *stop=0xac5fc620u;r[12]+=4294967295u;goto Lac5fc622; /* add #-0x1,r12 */
Lac5fc622: *stop=0xac5fc622u;r[13]=r[4];goto Lac5fc624; /* mov r4,r13 */
Lac5fc624: *stop=0xac5fc624u;r[13]>>=2;goto Lac5fc626; /* shlr2 r13 */
Lac5fc626: *stop=0xac5fc626u;r[4]=(r[1]&0x80000000u)?((r[1]&31u)?r[4]>>((0u-r[1])&31u):0u):(r[4]<<(r[1]&31u));goto Lac5fc628; /* shld r1,r4 */
Lac5fc628: *stop=0xac5fc628u;r[13]>>=2;goto Lac5fc62a; /* shlr2 r13 */
Lac5fc62a: *stop=0xac5fc62au;r[4]&=r[5];goto Lac5fc62c; /* and r5,r4 */
Lac5fc62c: *stop=0xac5fc62cu;*sr=(*sr&~1u)|(r[13]&1u);r[13]>>=1;goto Lac5fc62e; /* shlr r13 */
Lac5fc62e: *stop=0xac5fc62eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto Lac5fc630; /* mov.l r4,@(0x4,r15) */
Lac5fc630: *stop=0xac5fc630u;r[12]&=r[3];goto Lac5fc632; /* and r3,r12 */
Lac5fc632: *stop=0xac5fc632u;r[4]=r[14];goto Lac5fc634; /* mov r14,r4 */
Lac5fc634: *stop=0xac5fc634u;r[13]&=r[5];goto Lac5fc636; /* and r5,r13 */
Lac5fc636: *stop=0xac5fc636u;r[5]=r[12];goto Lac5fc638; /* mov r12,r5 */
Lac5fc638: *stop=0xac5fc638u;r[6]=r[13];goto Lac5fc63a; /* mov r13,r6 */
Lac5fc63a: *stop=0xac5fc63cu;goto Lac5fc648; /* bra 0xac5fc648 */
Lac5fc648: *stop=0xac5fc648u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto Lac5fc64a; /* mov.l r10,@-r15 */
Lac5fc64a: *stop=0xac5fc64au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto Lac5fc64c; /* mov.l r9,@-r15 */
Lac5fc64c: *stop=0xac5fc64cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto Lac5fc64e; /* mov.l r8,@-r15 */
Lac5fc64e: *stop=0xac5fc64eu;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc668u,4);if(io_status)return io_status;r[0]=loaded;}goto Lac5fc650; /* mov.l 0xac5fc668,r0 */
Lac5fc650: *stop=0xac5fc650u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc66cu,4);if(io_status)return io_status;r[8]=loaded;}goto Lac5fc652; /* mov.l 0xac5fc66c,r8 */
Lac5fc652: *stop=0xac5fc652u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc670u,4);if(io_status)return io_status;r[9]=loaded;}goto Lac5fc654; /* mov.l 0xac5fc670,r9 */
Lac5fc654: *stop=0xac5fc654u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc674u,4);if(io_status)return io_status;r[10]=loaded;}goto Lac5fc656; /* mov.l 0xac5fc674,r10 */
Lac5fc656: *stop=0xac5fc656u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc678u,4);if(io_status)return io_status;r[2]=loaded;}goto Lac5fc658; /* mov.l 0xac5fc678,r2 */
Lac5fc658: *stop=0xac5fc658u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc67cu,4);if(io_status)return io_status;r[3]=loaded;}goto Lac5fc65a; /* mov.l 0xac5fc67c,r3 */
Lac5fc65a: *stop=0xac5fc65au;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc680u,4);if(io_status)return io_status;r[7]=loaded;}goto Lac5fc65c; /* mov.l 0xac5fc680,r7 */
Lac5fc65c: *stop=0xac5fc65cu;r[7]&=r[4];goto Lac5fc65e; /* and r4,r7 */
Lac5fc65e: *stop=0xac5fc65eu;r[7]=~r[7];goto Lac5fc660; /* not r7,r7 */
Lac5fc660: *stop=0xac5fc660u;r[4]&=r[7];goto Lac5fc662; /* and r7,r4 */
Lac5fc662: *stop=0xac5fc662u;r[7]=~r[7];goto Lac5fc664; /* not r7,r7 */
Lac5fc664: *stop=0xac5fc666u;goto Lac5fc6a4; /* bra 0xac5fc6a4 */
Lac5fc684: *stop=0xac5fc684u;r[1]=r[0];goto Lac5fc686; /* mov r0,r1 */
Lac5fc686: *stop=0xac5fc686u;r[1]|=r[8];goto Lac5fc688; /* or r8,r1 */
Lac5fc688: *stop=0xac5fc688u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto Lac5fc68a; /* mov.l @r1,r1 */
Lac5fc68a: *stop=0xac5fc68au;r[1]&=r[9];goto Lac5fc68c; /* and r9,r1 */
Lac5fc68c: *stop=0xac5fc68cu;r[1]|=r[0];goto Lac5fc68e; /* or r0,r1 */
Lac5fc68e: *stop=0xac5fc68eu;*sr=(*sr&~1u)|(r[4]>r[1]);goto Lac5fc690; /* cmp/hi r1,r4 */
Lac5fc690: if((*sr&1u))goto Lac5fc69a;goto Lac5fc692; /* bt 0xac5fc69a */
Lac5fc692: *stop=0xac5fc692u;*sr=(*sr&~1u)|(r[1]>r[5]);goto Lac5fc694; /* cmp/hi r5,r1 */
Lac5fc694: if((*sr&1u))goto Lac5fc69a;goto Lac5fc696; /* bt 0xac5fc69a */
Lac5fc696: *stop=0xac5fc696u;r[1]|=r[7];goto Lac5fc698; /* or r7,r1 */
Lac5fc698: *stop=0xac5fc698u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[1],2u);if(status)return status;goto Lac5fc69a; /* ocbwb @r1 */
Lac5fc69a: *stop=0xac5fc69au;*sr=(*sr&~1u)|!(r[6]&r[6]);goto Lac5fc69c; /* tst r6,r6 */
Lac5fc69c: if((*sr&1u))goto Lac5fc6a2;goto Lac5fc69e; /* bt 0xac5fc6a2 */
Lac5fc69e: *stop=0xac5fc69eu;*sr=(*sr&~1u)|(r[0]==r[3]);goto Lac5fc6a0; /* cmp/eq r0,r3 */
Lac5fc6a0: if((*sr&1u))goto Lac5fc6a8;goto Lac5fc6a2; /* bt 0xac5fc6a8 */
Lac5fc6a2: *stop=0xac5fc6a2u;r[0]+=32u;goto Lac5fc6a4; /* add #0x20,r0 */
Lac5fc6a4: *stop=0xac5fc6a4u;*sr=(*sr&~1u)|(r[2]>r[0]);goto Lac5fc6a6; /* cmp/hi r0,r2 */
Lac5fc6a6: if((*sr&1u))goto Lac5fc684;goto Lac5fc6a8; /* bt 0xac5fc684 */
Lac5fc6a8: *stop=0xac5fc6a8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto Lac5fc6aa; /* mov.l @r15+,r8 */
Lac5fc6aa: *stop=0xac5fc6aau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto Lac5fc6ac; /* mov.l @r15+,r9 */
Lac5fc6ac: *stop=0xac5fc6acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto Lac5fc6ae; /* mov.l @r15+,r10 */
Lac5fc6ae: *stop=0xac5fc6aeu;goto Lac5fc6b0; /* nop */
Lac5fc6b0: *stop=0xac5fc6b0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto Lac5fc6b2; /* mov.l @(0x4,r15),r3 */
Lac5fc6b2: *stop=0xac5fc6b2u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto Lac5fc6b4; /* tst r3,r3 */
Lac5fc6b4: if(!(*sr&1u))goto Lac5fc6bc;goto Lac5fc6b6; /* bf 0xac5fc6bc */
Lac5fc6b6: *stop=0xac5fc6b6u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc6c8u,4);if(io_status)return io_status;r[3]=loaded;}goto Lac5fc6b8; /* mov.l 0xac5fc6c8,r3 */
Lac5fc6b8: target=r[3];*stop=0xac5fc6bau;if(target==0x8c5fc746u)goto L8c5fc746;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
Lac5fc6bc: *stop=0xac5fc6bcu;r[4]=r[14];goto Lac5fc6be; /* mov r14,r4 */
Lac5fc6be: *stop=0xac5fc6beu;r[5]=r[12];goto Lac5fc6c0; /* mov r12,r5 */
Lac5fc6c0: *stop=0xac5fc6c0u;r[6]=r[13];goto Lac5fc6c2; /* mov r13,r6 */
Lac5fc6c2: *stop=0xac5fc6c4u;goto Lac5fc6cc; /* bra 0xac5fc6cc */
Lac5fc6cc: *stop=0xac5fc6ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto Lac5fc6ce; /* mov.l r11,@-r15 */
Lac5fc6ce: *stop=0xac5fc6ceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto Lac5fc6d0; /* mov.l r10,@-r15 */
Lac5fc6d0: *stop=0xac5fc6d0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto Lac5fc6d2; /* mov.l r9,@-r15 */
Lac5fc6d2: *stop=0xac5fc6d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto Lac5fc6d4; /* mov.l r8,@-r15 */
Lac5fc6d4: *stop=0xac5fc6d4u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc6f0u,4);if(io_status)return io_status;r[0]=loaded;}goto Lac5fc6d6; /* mov.l 0xac5fc6f0,r0 */
Lac5fc6d6: *stop=0xac5fc6d6u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc6f4u,4);if(io_status)return io_status;r[8]=loaded;}goto Lac5fc6d8; /* mov.l 0xac5fc6f4,r8 */
Lac5fc6d8: *stop=0xac5fc6d8u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc6f8u,4);if(io_status)return io_status;r[9]=loaded;}goto Lac5fc6da; /* mov.l 0xac5fc6f8,r9 */
Lac5fc6da: *stop=0xac5fc6dau;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc6fcu,4);if(io_status)return io_status;r[10]=loaded;}goto Lac5fc6dc; /* mov.l 0xac5fc6fc,r10 */
Lac5fc6dc: *stop=0xac5fc6dcu;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc700u,4);if(io_status)return io_status;r[2]=loaded;}goto Lac5fc6de; /* mov.l 0xac5fc700,r2 */
Lac5fc6de: *stop=0xac5fc6deu;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc704u,4);if(io_status)return io_status;r[3]=loaded;}goto Lac5fc6e0; /* mov.l 0xac5fc704,r3 */
Lac5fc6e0: *stop=0xac5fc6e0u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc708u,4);if(io_status)return io_status;r[7]=loaded;}goto Lac5fc6e2; /* mov.l 0xac5fc708,r7 */
Lac5fc6e2: *stop=0xac5fc6e2u;r[7]&=r[4];goto Lac5fc6e4; /* and r4,r7 */
Lac5fc6e4: *stop=0xac5fc6e4u;r[7]=~r[7];goto Lac5fc6e6; /* not r7,r7 */
Lac5fc6e6: *stop=0xac5fc6e6u;r[4]&=r[7];goto Lac5fc6e8; /* and r7,r4 */
Lac5fc6e8: *stop=0xac5fc6e8u;r[7]=~r[7];goto Lac5fc6ea; /* not r7,r7 */
Lac5fc6ea: *stop=0xac5fc6ecu;goto Lac5fc734; /* bra 0xac5fc734 */
Lac5fc70c: *stop=0xac5fc70cu;r[1]=r[0];goto Lac5fc70e; /* mov r0,r1 */
Lac5fc70e: *stop=0xac5fc70eu;r[1]|=r[8];goto Lac5fc710; /* or r8,r1 */
Lac5fc710: *stop=0xac5fc710u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]=loaded;}goto Lac5fc712; /* mov.l @r1,r1 */
Lac5fc712: *stop=0xac5fc712u;r[1]&=r[9];goto Lac5fc714; /* and r9,r1 */
Lac5fc714: *stop=0xac5fc714u;r[1]|=r[0];goto Lac5fc716; /* or r0,r1 */
Lac5fc716: *stop=0xac5fc716u;r[11]=r[0];goto Lac5fc718; /* mov r0,r11 */
Lac5fc718: *stop=0xac5fc718u;r[11]&=r[10];goto Lac5fc71a; /* and r10,r11 */
Lac5fc71a: *stop=0xac5fc71au;r[1]|=r[11];goto Lac5fc71c; /* or r11,r1 */
Lac5fc71c: *stop=0xac5fc71cu;r[1]|=r[12];goto Lac5fc71e; /* or r12,r1 */
Lac5fc71e: *stop=0xac5fc71eu;*sr=(*sr&~1u)|(r[4]>r[1]);goto Lac5fc720; /* cmp/hi r1,r4 */
Lac5fc720: if((*sr&1u))goto Lac5fc72a;goto Lac5fc722; /* bt 0xac5fc72a */
Lac5fc722: *stop=0xac5fc722u;*sr=(*sr&~1u)|(r[1]>r[5]);goto Lac5fc724; /* cmp/hi r5,r1 */
Lac5fc724: if((*sr&1u))goto Lac5fc72a;goto Lac5fc726; /* bt 0xac5fc72a */
Lac5fc726: *stop=0xac5fc726u;r[1]|=r[7];goto Lac5fc728; /* or r7,r1 */
Lac5fc728: *stop=0xac5fc728u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[1],2u);if(status)return status;goto Lac5fc72a; /* ocbwb @r1 */
Lac5fc72a: *stop=0xac5fc72au;*sr=(*sr&~1u)|!(r[6]&r[6]);goto Lac5fc72c; /* tst r6,r6 */
Lac5fc72c: if((*sr&1u))goto Lac5fc732;goto Lac5fc72e; /* bt 0xac5fc732 */
Lac5fc72e: *stop=0xac5fc72eu;*sr=(*sr&~1u)|(r[0]==r[3]);goto Lac5fc730; /* cmp/eq r0,r3 */
Lac5fc730: if((*sr&1u))goto Lac5fc738;goto Lac5fc732; /* bt 0xac5fc738 */
Lac5fc732: *stop=0xac5fc732u;r[0]+=32u;goto Lac5fc734; /* add #0x20,r0 */
Lac5fc734: *stop=0xac5fc734u;*sr=(*sr&~1u)|(r[2]>r[0]);goto Lac5fc736; /* cmp/hi r0,r2 */
Lac5fc736: if((*sr&1u))goto Lac5fc70c;goto Lac5fc738; /* bt 0xac5fc70c */
Lac5fc738: *stop=0xac5fc738u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto Lac5fc73a; /* mov.l @r15+,r8 */
Lac5fc73a: *stop=0xac5fc73au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto Lac5fc73c; /* mov.l @r15+,r9 */
Lac5fc73c: *stop=0xac5fc73cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto Lac5fc73e; /* mov.l @r15+,r10 */
Lac5fc73e: *stop=0xac5fc73eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto Lac5fc740; /* mov.l @r15+,r11 */
Lac5fc740: *stop=0xac5fc740u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc750u,4);if(io_status)return io_status;r[2]=loaded;}goto Lac5fc742; /* mov.l 0xac5fc750,r2 */
Lac5fc742: target=r[2];*stop=0xac5fc744u;if(target==0x8c5fc7c0u)goto L8c5fc7c0;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}
