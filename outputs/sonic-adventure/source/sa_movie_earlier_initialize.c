/* Original earlier initialization, memory-manager setup and record status.
 * Static C from byte-checked SH-4 instructions; complete explicit state.
 * Missing callees remain reached boundaries; caller retry is research-bounded
 * only under SA_BOUND_RETAIL_STALLS and is not a successful completion.
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
SA_STAGE_EXPORT int sa_movie_earlier_initialize_8c053ac0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 unsigned delay_visits=0;(void)delay_visits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c053ac0;
L8c053ac0: *stop=0x8c053ac0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c053ac2; /* mov.l r14,@-r15 */
L8c053ac2: *stop=0x8c053ac2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c053ac4; /* mov.l r13,@-r15 */
L8c053ac4: *stop=0x8c053ac4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c053ac6; /* sts.l PR,@-r15 */
L8c053ac6: *stop=0x8c053ac6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c053ac8; /* mov.l 0x8c053b20,r3 */
L8c053ac8: target=r[3];*pr=0x8c053accu;*stop=0x8c053acau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c053acc; /* jsr @r3 */
L8c053acc: target=0x8c0546c0u;*pr=0x8c053ad0u;*stop=0x8c053aceu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c053ad0; /* bsr 0x8c0546c0 */
L8c053ad0: *stop=0x8c053ad0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b28u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c053ad2; /* mov.l 0x8c053b28,r14 */
L8c053ad2: *stop=0x8c053ad2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b24u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c053ad4; /* mov.l 0x8c053b24,r13 */
L8c053ad4: target=r[14];*pr=0x8c053ad8u;*stop=0x8c053ad6u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c053ad8; /* jsr @r14 */
L8c053ad8: *stop=0x8c053ad8u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c053ada; /* cmp/eq #-0x1,r0 */
L8c053ada:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c053adau;if((*sr&1u) && ++delay_visits>=2u)return -4;
#endif
 if((*sr&1u))goto L8c053ad4;goto L8c053adc; /* bt 0x8c053ad4 */
L8c053adc: *stop=0x8c053adcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c053ade; /* mov.l 0x8c053b2c,r3 */
L8c053ade: target=r[3];*pr=0x8c053ae2u;*stop=0x8c053ae0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c053ae2; /* jsr @r3 */
L8c053ae2: *stop=0x8c053ae2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b2cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c053ae4; /* mov.l 0x8c053b2c,r2 */
L8c053ae4: target=r[2];*pr=0x8c053ae8u;*stop=0x8c053ae6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c053ae8; /* jsr @r2 */
L8c053ae8: *stop=0x8c053ae8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c053aea; /* mov.l 0x8c053b30,r2 */
L8c053aea: *stop=0x8c053aeau;r[3]=0u;goto L8c053aec; /* mov #0x0,r3 */
L8c053aec: *stop=0x8c053aecu;r[4]=r[3];goto L8c053aee; /* mov r3,r4 */
L8c053aee: *stop=0x8c053aeeu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c053af0; /* mov.l r3,@r2 */
L8c053af0: *stop=0x8c053af0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c053af2; /* mov.l 0x8c053b34,r3 */
L8c053af2: target=r[3];*pr=0x8c053af6u;*stop=0x8c053af4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c053af6; /* jsr @r3 */
L8c053af6: *stop=0x8c053af6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c053af8; /* mov.l 0x8c053b38,r2 */
L8c053af8: target=r[2];*pr=0x8c053afcu;*stop=0x8c053afau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c053afc; /* jsr @r2 */
L8c053afc: *stop=0x8c053afcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c053afe; /* mov.l 0x8c053b3c,r3 */
L8c053afe: target=r[3];*pr=0x8c053b02u;*stop=0x8c053b00u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c053b02; /* jsr @r3 */
L8c053b02: *stop=0x8c053b02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c053b04; /* mov.l 0x8c053b40,r3 */
L8c053b04: *stop=0x8c053b04u;r[2]=0u;goto L8c053b06; /* mov #0x0,r2 */
L8c053b06: *stop=0x8c053b06u;r[5]=r[2];goto L8c053b08; /* mov r2,r5 */
L8c053b08: *stop=0x8c053b08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b44u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c053b0a; /* mov.l 0x8c053b44,r4 */
L8c053b0a: *stop=0x8c053b0au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c053b0c; /* mov.l r2,@r3 */
L8c053b0c: *stop=0x8c053b0cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c053b0e; /* lds.l @r15+,PR */
L8c053b0e: *stop=0x8c053b0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c053b48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c053b10; /* mov.l 0x8c053b48,r2 */
L8c053b10: *stop=0x8c053b10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c053b12; /* mov.l @r15+,r13 */
L8c053b12: target=r[2];*stop=0x8c053b14u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_movie_earlier_initialize_8c644d84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c644d84;
L8c644d84: *stop=0x8c644d84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c644d94u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c644d86; /* mov.l 0x8c644d94,r3 */
L8c644d86: *stop=0x8c644d86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c644d90u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c644d88; /* mov.l 0x8c644d90,r4 */
L8c644d88: *stop=0x8c644d88u;r[5]=sx(m,bus,&io_status,0x8c644d8eu,2);if(io_status)return io_status;goto L8c644d8a; /* mov.w 0x8c644d8e,r5 */
L8c644d8a: target=r[3];*stop=0x8c644d8cu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_earlier_initialize_8c6033e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6033e4;
L8c6033e4: *stop=0x8c6033e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6033e6; /* sts.l PR,@-r15 */
L8c6033e6: *stop=0x8c6033e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6034e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6033e8; /* mov.l 0x8c6034e8,r3 */
L8c6033e8: target=r[3];*pr=0x8c6033ecu;*stop=0x8c6033eau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6033ec; /* jsr @r3 */
L8c6033ec: *stop=0x8c6033ecu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6033ee; /* tst r0,r0 */
L8c6033ee: if((*sr&1u))goto L8c6033f6;goto L8c6033f0; /* bt 0x8c6033f6 */
L8c6033f0: *stop=0x8c6033f0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6033f2; /* lds.l @r15+,PR */
L8c6033f2: *stop=0x8c6033f2u;if(*pr!=entry_pr)return -3;*stop=0x8c6033f4u;r[0]=4294967295u;return 0; /* rts */
L8c6033f6: *stop=0x8c6033f6u;r[0]=0u;goto L8c6033f8; /* mov #0x0,r0 */
L8c6033f8: *stop=0x8c6033f8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6033fa; /* lds.l @r15+,PR */
L8c6033fa: *stop=0x8c6033fau;if(*pr!=entry_pr)return -3;*stop=0x8c6033fcu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_earlier_initialize_8c5fbb60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5fbb60;
L8c5fbb60: *stop=0x8c5fbb60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c5fbb62; /* sts.l PR,@-r15 */
L8c5fbb62: *stop=0x8c5fbb62u;r[15]+=4294967292u;goto L8c5fbb64; /* add #-0x4,r15 */
L8c5fbb64: *stop=0x8c5fbb64u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5fbb66; /* tst r4,r4 */
L8c5fbb66: if(!(*sr&1u))goto L8c5fbb70;goto L8c5fbb68; /* bf 0x8c5fbb70 */
L8c5fbb68: *stop=0x8c5fbb68u;r[15]+=4u;goto L8c5fbb6a; /* add #0x4,r15 */
L8c5fbb6a: *stop=0x8c5fbb6au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5fbb6c; /* lds.l @r15+,PR */
L8c5fbb6c: *stop=0x8c5fbb6cu;if(*pr!=entry_pr)return -3;*stop=0x8c5fbb6eu;r[0]=4294967295u;return 0; /* rts */
L8c5fbb70: *stop=0x8c5fbb70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fbd0cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fbb72; /* mov.l 0x8c5fbd0c,r3 */
L8c5fbb72: *stop=0x8c5fbb72u;r[5]<<=2;goto L8c5fbb74; /* shll2 r5 */
L8c5fbb74: *stop=0x8c5fbb74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fbd08u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fbb76; /* mov.l 0x8c5fbd08,r2 */
L8c5fbb76: *stop=0x8c5fbb76u;r[5]+=r[4];goto L8c5fbb78; /* add r4,r5 */
L8c5fbb78: *stop=0x8c5fbb78u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c5fbb7a; /* mov.l r2,@r3 */
L8c5fbb7a: *stop=0x8c5fbb7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fbd10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fbb7c; /* mov.l 0x8c5fbd10,r2 */
L8c5fbb7c: *stop=0x8c5fbb7cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c5fbb7e; /* mov.l r5,@r15 */
L8c5fbb7e: *stop=0x8c5fbb7eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c5fbb80; /* mov.l r5,@r2 */
L8c5fbb80: target=0x8c5fbcb6u;*pr=0x8c5fbb84u;*stop=0x8c5fbb82u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fbb84; /* bsr 0x8c5fbcb6 */
L8c5fbb84: *stop=0x8c5fbb84u;r[0]=0u;goto L8c5fbb86; /* mov #0x0,r0 */
L8c5fbb86: *stop=0x8c5fbb86u;r[15]+=4u;goto L8c5fbb88; /* add #0x4,r15 */
L8c5fbb88: *stop=0x8c5fbb88u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5fbb8a; /* lds.l @r15+,PR */
L8c5fbb8a: *stop=0x8c5fbb8au;if(*pr!=entry_pr)return -3;*stop=0x8c5fbb8cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_earlier_initialize_8c647ca4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647ca4;
L8c647ca4: *stop=0x8c647ca4u;r[4]<<=2;goto L8c647ca6; /* shll2 r4 */
L8c647ca6: *stop=0x8c647ca6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647d10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647ca8; /* mov.l 0x8c647d10,r3 */
L8c647ca8: *stop=0x8c647ca8u;r[4]<<=2;goto L8c647caa; /* shll2 r4 */
L8c647caa: *stop=0x8c647caau;r[4]<<=2;goto L8c647cac; /* shll2 r4 */
L8c647cac: *stop=0x8c647cacu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c647cae; /* shll r4 */
L8c647cae: *stop=0x8c647caeu;r[4]+=r[3];goto L8c647cb0; /* add r3,r4 */
L8c647cb0: *stop=0x8c647cb0u;if(*pr!=entry_pr)return -3;*stop=0x8c647cb2u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_earlier_initialize_8c5fbcb6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5fbcb6;
L8c5fbcb6: *stop=0x8c5fbcb6u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5fbcb8; /* tst r4,r4 */
L8c5fbcb8: if(!(*sr&1u))goto L8c5fbcbe;goto L8c5fbcba; /* bf 0x8c5fbcbe */
L8c5fbcba: *stop=0x8c5fbcbau;if(*pr!=entry_pr)return -3;*stop=0x8c5fbcbcu;r[0]=4294967295u;return 0; /* rts */
L8c5fbcbe: *stop=0x8c5fbcbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fbd18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fbcc0; /* mov.l 0x8c5fbd18,r2 */
L8c5fbcc0: *stop=0x8c5fbcc0u;r[0]=0u;goto L8c5fbcc2; /* mov #0x0,r0 */
L8c5fbcc2: *stop=0x8c5fbcc2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c5fbcc4; /* mov.l r4,@r2 */
L8c5fbcc4: *stop=0x8c5fbcc4u;if(*pr!=entry_pr)return -3;*stop=0x8c5fbcc6u;return 0; /* rts */
}
