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
SA_STAGE_EXPORT int sa_movie_packet_startup_8c64bddc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 unsigned read_polls=0,write_polls=0;(void)read_polls;(void)write_polls;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64bddc;
L8c64bddc: *stop=0x8c64bddcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64bdde; /* mov.l r14,@-r15 */
L8c64bdde: *stop=0x8c64bddeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64bde0; /* mov.l r13,@-r15 */
L8c64bde0: *stop=0x8c64bde0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64bde2; /* mov.l r12,@-r15 */
L8c64bde2: *stop=0x8c64bde2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64bde4; /* mov.l r11,@-r15 */
L8c64bde4: *stop=0x8c64bde4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64bde6; /* sts.l PR,@-r15 */
L8c64bde6: *stop=0x8c64bde6u;r[15]+=4294967236u;goto L8c64bde8; /* add #-0x3c,r15 */
L8c64bde8: *stop=0x8c64bde8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bec0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bdea; /* mov.l 0x8c64bec0,r3 */
L8c64bdea: *stop=0x8c64bdeau;r[14]=r[4];goto L8c64bdec; /* mov r4,r14 */
L8c64bdec: target=r[3];*pr=0x8c64bdf0u;*stop=0x8c64bdeeu;r[4]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bdf0; /* jsr @r3 */
L8c64bdf0: *stop=0x8c64bdf0u;*sr=(*sr&~1u)|(r[0]==0xfffffff8u);goto L8c64bdf2; /* cmp/eq #-0x8,r0 */
L8c64bdf2: take=!(*sr&1u);*stop=0x8c64bdf4u;r[4]=r[0];if(take)goto L8c64be06;goto L8c64bdf6; /* bf/s 0x8c64be06 */
L8c64bdf6: *stop=0x8c64bdf6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bec4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bdf8; /* mov.l 0x8c64bec4,r3 */
L8c64bdf8: target=r[3];*pr=0x8c64bdfcu;*stop=0x8c64bdfau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bdfc; /* jsr @r3 */
L8c64bdfc: *stop=0x8c64bdfcu;r[4]=r[0];goto L8c64bdfe; /* mov r0,r4 */
L8c64bdfe: *stop=0x8c64bdfeu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64be00; /* tst r4,r4 */
L8c64be00: if(!(*sr&1u))goto L8c64beaa;goto L8c64be02; /* bf 0x8c64beaa */
L8c64be02: *stop=0x8c64be04u;goto L8c64be0a; /* bra 0x8c64be0a */
L8c64be06: *stop=0x8c64be06u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64be08; /* tst r4,r4 */
L8c64be08: if(!(*sr&1u))goto L8c64beaa;goto L8c64be0a; /* bf 0x8c64beaa */
L8c64be0a: *stop=0x8c64be0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bec8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64be0c; /* mov.l 0x8c64bec8,r2 */
L8c64be0c: *stop=0x8c64be0cu;r[12]=r[15];goto L8c64be0e; /* mov r15,r12 */
L8c64be0e: *stop=0x8c64be0eu;r[5]=r[12];goto L8c64be10; /* mov r12,r5 */
L8c64be10: target=r[2];*pr=0x8c64be14u;*stop=0x8c64be12u;r[4]=5u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64be14; /* jsr @r2 */
L8c64be14: *stop=0x8c64be14u;r[4]=r[0];goto L8c64be16; /* mov r0,r4 */
L8c64be16: *stop=0x8c64be16u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64be18; /* tst r4,r4 */
L8c64be18: if(!(*sr&1u))goto L8c64beaa;goto L8c64be1a; /* bf 0x8c64beaa */
L8c64be1a: *stop=0x8c64be1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c64beccu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64be1c; /* mov.l 0x8c64becc,r13 */
L8c64be1c: target=r[13];*pr=0x8c64be20u;*stop=0x8c64be1eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64be20; /* jsr @r13 */
L8c64be20: *stop=0x8c64be20u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c64be22; /* cmp/eq #0x2,r0 */
L8c64be22:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c64be22u;if((*sr&1u) && ++read_polls>=2u)return -4;
#endif
 take=(*sr&1u);*stop=0x8c64be24u;r[4]=r[0];if(take)goto L8c64be1c;goto L8c64be26; /* bt/s 0x8c64be1c */
L8c64be26: *stop=0x8c64be26u;r[0]=r[4];goto L8c64be28; /* mov r4,r0 */
L8c64be28: *stop=0x8c64be28u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64be2a; /* cmp/eq #0x1,r0 */
L8c64be2a: if(!(*sr&1u))goto L8c64beaa;goto L8c64be2c; /* bf 0x8c64beaa */
L8c64be2c: *stop=0x8c64be2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bed0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64be2e; /* mov.l 0x8c64bed0,r3 */
L8c64be2e: target=r[3];*pr=0x8c64be32u;*stop=0x8c64be30u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64be32; /* jsr @r3 */
L8c64be32: *stop=0x8c64be32u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c64be34; /* tst r14,r14 */
L8c64be34: *stop=0x8c64be34u;r[4]=r[0];goto L8c64be36; /* mov r0,r4 */
L8c64be36: take=!(*sr&1u);*stop=0x8c64be38u;r[4]+=16u;if(take)goto L8c64be50;goto L8c64be3a; /* bf/s 0x8c64be50 */
L8c64be3a: *stop=0x8c64be3au;r[6]=r[12];goto L8c64be3c; /* mov r12,r6 */
L8c64be3c: *stop=0x8c64be3eu;r[5]=8u;goto L8c64be48; /* bra 0x8c64be48 */
L8c64be40: *stop=0x8c64be40u;if(!valid_access(r[6],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[3]=loaded;}r[6]+=1;goto L8c64be42; /* mov.b @r6+,r3 */
L8c64be42: *stop=0x8c64be42u;r[5]+=4294967295u;goto L8c64be44; /* add #-0x1,r5 */
L8c64be44: *stop=0x8c64be44u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c64be46; /* mov.b r3,@r4 */
L8c64be46: *stop=0x8c64be46u;r[4]+=1u;goto L8c64be48; /* add #0x1,r4 */
L8c64be48: *stop=0x8c64be48u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c64be4a; /* tst r5,r5 */
L8c64be4a: if(!(*sr&1u))goto L8c64be40;goto L8c64be4c; /* bf 0x8c64be40 */
L8c64be4c: *stop=0x8c64be4eu;goto L8c64beae; /* bra 0x8c64beae */
L8c64be50: *stop=0x8c64be50u;r[5]=r[12];goto L8c64be52; /* mov r12,r5 */
L8c64be52: *stop=0x8c64be52u;r[6]=0u;goto L8c64be54; /* mov #0x0,r6 */
L8c64be54: *stop=0x8c64be54u;r[11]=5u;goto L8c64be56; /* mov #0x5,r11 */
L8c64be56: *stop=0x8c64be56u;r[5]+=7u;goto L8c64be58; /* add #0x7,r5 */
L8c64be58: *stop=0x8c64be58u;r[4]+=7u;goto L8c64be5a; /* add #0x7,r4 */
L8c64be5a: *stop=0x8c64be5cu;r[7]=1u;goto L8c64be88; /* bra 0x8c64be88 */
L8c64be5e: *stop=0x8c64be5eu;r[3]=r[14];goto L8c64be60; /* mov r14,r3 */
L8c64be60: *stop=0x8c64be60u;*sr=(*sr&~1u)|!(r[7]&r[3]);goto L8c64be62; /* tst r7,r3 */
L8c64be62: if((*sr&1u))goto L8c64be80;goto L8c64be64; /* bt 0x8c64be80 */
L8c64be64: *stop=0x8c64be64u;r[0]=r[6];goto L8c64be66; /* mov r6,r0 */
L8c64be66: *stop=0x8c64be66u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[1]=loaded;}goto L8c64be68; /* mov.b @r4,r1 */
L8c64be68: *stop=0x8c64be68u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c64be6a; /* cmp/eq #0x4,r0 */
L8c64be6a: take=!(*sr&1u);*stop=0x8c64be6cu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[1],1);if(io_status)return io_status;if(take)goto L8c64be80;goto L8c64be6e; /* bf/s 0x8c64be80 */
L8c64be6e: *stop=0x8c64be6eu;r[4]+=4294967295u;goto L8c64be70; /* add #-0x1,r4 */
L8c64be70: *stop=0x8c64be70u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}goto L8c64be72; /* mov.b @r4,r3 */
L8c64be72: *stop=0x8c64be72u;r[4]+=4294967295u;goto L8c64be74; /* add #-0x1,r4 */
L8c64be74: *stop=0x8c64be74u;if(!valid_access(r[5]-1u,1u))return -3;r[5]-=1;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c64be76; /* mov.b r3,@-r5 */
L8c64be76: *stop=0x8c64be76u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}goto L8c64be78; /* mov.b @r4,r3 */
L8c64be78: *stop=0x8c64be78u;r[4]+=4294967295u;goto L8c64be7a; /* add #-0x1,r4 */
L8c64be7a: *stop=0x8c64be7au;if(!valid_access(r[5]-1u,1u))return -3;r[5]-=1;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c64be7c; /* mov.b r3,@-r5 */
L8c64be7c: *stop=0x8c64be7cu;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}goto L8c64be7e; /* mov.b @r4,r3 */
L8c64be7e: *stop=0x8c64be7eu;if(!valid_access(r[5]-1u,1u))return -3;r[5]-=1;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c64be80; /* mov.b r3,@-r5 */
L8c64be80: *stop=0x8c64be80u;r[4]+=4294967295u;goto L8c64be82; /* add #-0x1,r4 */
L8c64be82: *stop=0x8c64be82u;r[6]+=1u;goto L8c64be84; /* add #0x1,r6 */
L8c64be84: *stop=0x8c64be84u;*sr=(*sr&~1u)|(r[14]&1u);r[14]=(r[14]>>1)|(r[14]&0x80000000u);goto L8c64be86; /* shar r14 */
L8c64be86: *stop=0x8c64be86u;r[5]+=4294967295u;goto L8c64be88; /* add #-0x1,r5 */
L8c64be88: *stop=0x8c64be88u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[11]);goto L8c64be8a; /* cmp/ge r11,r6 */
L8c64be8a: if(!(*sr&1u))goto L8c64be5e;goto L8c64be8c; /* bf 0x8c64be5e */
L8c64be8c: *stop=0x8c64be8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bed4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64be8e; /* mov.l 0x8c64bed4,r2 */
L8c64be8e: *stop=0x8c64be8eu;r[5]=r[12];goto L8c64be90; /* mov r12,r5 */
L8c64be90: target=r[2];*pr=0x8c64be94u;*stop=0x8c64be92u;r[4]=5u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64be94; /* jsr @r2 */
L8c64be94: *stop=0x8c64be94u;r[4]=r[0];goto L8c64be96; /* mov r0,r4 */
L8c64be96: *stop=0x8c64be96u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64be98; /* tst r4,r4 */
L8c64be98: if(!(*sr&1u))goto L8c64beaa;goto L8c64be9a; /* bf 0x8c64beaa */
L8c64be9a: target=r[13];*pr=0x8c64be9eu;*stop=0x8c64be9cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64be9e; /* jsr @r13 */
L8c64be9e: *stop=0x8c64be9eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c64bea0; /* cmp/eq #0x2,r0 */
L8c64bea0:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c64bea0u;if((*sr&1u) && ++write_polls>=3u)return -4;
#endif
 take=(*sr&1u);*stop=0x8c64bea2u;r[4]=r[0];if(take)goto L8c64be9a;goto L8c64bea4; /* bt/s 0x8c64be9a */
L8c64bea4: *stop=0x8c64bea4u;r[0]=r[4];goto L8c64bea6; /* mov r4,r0 */
L8c64bea6: *stop=0x8c64bea6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64bea8; /* cmp/eq #0x1,r0 */
L8c64bea8: if((*sr&1u))goto L8c64beae;goto L8c64beaa; /* bt 0x8c64beae */
L8c64beaa: *stop=0x8c64beacu;r[0]=4294967295u;goto L8c64beb0; /* bra 0x8c64beb0 */
L8c64beae: *stop=0x8c64beaeu;r[0]=0u;goto L8c64beb0; /* mov #0x0,r0 */
L8c64beb0: *stop=0x8c64beb0u;r[15]+=60u;goto L8c64beb2; /* add #0x3c,r15 */
L8c64beb2: *stop=0x8c64beb2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64beb4; /* lds.l @r15+,PR */
L8c64beb4: *stop=0x8c64beb4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64beb6; /* mov.l @r15+,r11 */
L8c64beb6: *stop=0x8c64beb6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64beb8; /* mov.l @r15+,r12 */
L8c64beb8: *stop=0x8c64beb8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64beba; /* mov.l @r15+,r13 */
L8c64beba: *stop=0x8c64bebau;if(*pr!=entry_pr)return -3;*stop=0x8c64bebcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c604044(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c604044;
L8c604044: *stop=0x8c604044u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c604046; /* mov.l r14,@-r15 */
L8c604046: *stop=0x8c604046u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c604048; /* mov.l r13,@-r15 */
L8c604048: *stop=0x8c604048u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c60404a; /* sts.l PR,@-r15 */
L8c60404a: *stop=0x8c60404au;r[15]+=4294967284u;goto L8c60404c; /* add #-0xc,r15 */
L8c60404c: *stop=0x8c60404cu;r[3]=1u;goto L8c60404e; /* mov #0x1,r3 */
L8c60404e: *stop=0x8c60404eu;r[13]=r[4];goto L8c604050; /* mov r4,r13 */
L8c604050: *stop=0x8c604050u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[3]);goto L8c604052; /* cmp/ge r3,r13 */
L8c604052: take=!(*sr&1u);*stop=0x8c604054u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;if(take)goto L8c60405c;goto L8c604056; /* bf/s 0x8c60405c */
L8c604056: *stop=0x8c604056u;r[1]=5u;goto L8c604058; /* mov #0x5,r1 */
L8c604058: *stop=0x8c604058u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[1]);goto L8c60405a; /* cmp/ge r1,r13 */
L8c60405a: if(!(*sr&1u))goto L8c604060;goto L8c60405c; /* bf 0x8c604060 */
L8c60405c: *stop=0x8c60405eu;r[0]=4294967289u;goto L8c60413e; /* bra 0x8c60413e */
L8c604060: *stop=0x8c604060u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604114u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604062; /* mov.l 0x8c604114,r3 */
L8c604062: *stop=0x8c604062u;r[5]=r[15];goto L8c604064; /* mov r15,r5 */
L8c604064: *stop=0x8c604064u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604110u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c604066; /* mov.l 0x8c604110,r14 */
L8c604066: *stop=0x8c604066u;r[5]+=4u;goto L8c604068; /* add #0x4,r5 */
L8c604068: *stop=0x8c604068u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604118u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60406a; /* mov.l 0x8c604118,r2 */
L8c60406a: *stop=0x8c60406au;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c60406c; /* mov.l r3,@r14 */
L8c60406c: target=r[2];*pr=0x8c604070u;*stop=0x8c60406eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604070; /* jsr @r2 */
L8c604070: *stop=0x8c604070u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c604072; /* mov.l @r14,r3 */
L8c604072: *stop=0x8c604072u;r[4]=0u;goto L8c604074; /* mov #0x0,r4 */
L8c604074: *stop=0x8c604074u;{uint32_t loaded=sx(m,bus,&io_status,0x8c604108u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c604076; /* mov.w 0x8c604108,r1 */
L8c604076: *stop=0x8c604076u;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[13],2);if(io_status)return io_status;goto L8c604078; /* mov.w r13,@r3 */
L8c604078: *stop=0x8c604078u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60407a; /* mov.l @(0x4,r15),r3 */
L8c60407a: *stop=0x8c60407au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60407c; /* mov.l @r14,r2 */
L8c60407c: *stop=0x8c60407cu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c60407e; /* mov.l r3,@(0x4,r2) */
L8c60407e: *stop=0x8c60407eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c604080; /* mov.l @(0x8,r15),r3 */
L8c604080: *stop=0x8c604080u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604082; /* mov.l @r14,r2 */
L8c604082: *stop=0x8c604082u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c604084; /* mov.l r3,@(0x8,r2) */
L8c604084: *stop=0x8c604084u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604086; /* mov.l @r14,r2 */
L8c604086: *stop=0x8c604086u;r[3]=r[2];goto L8c604088; /* mov r2,r3 */
L8c604088: *stop=0x8c604088u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c60408a; /* mov.l @(0x8,r3),r0 */
L8c60408a: *stop=0x8c60408au;r[3]=4294967281u;goto L8c60408c; /* mov #-0xf,r3 */
L8c60408c: *stop=0x8c60408cu;r[0]+=r[1];goto L8c60408e; /* add r1,r0 */
L8c60408e: *stop=0x8c60408eu;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c604090; /* shad r3,r0 */
L8c604090: *stop=0x8c604090u;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[0],4);if(io_status)return io_status;goto L8c604092; /* mov.l r0,@(0x18,r2) */
L8c604092: *stop=0x8c604092u;r[0]=4294967290u;goto L8c604094; /* mov #-0x6,r0 */
L8c604094: *stop=0x8c604094u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604096; /* mov.l @r14,r2 */
L8c604096: *stop=0x8c604096u;r[3]=r[2];goto L8c604098; /* mov r2,r3 */
L8c604098: *stop=0x8c604098u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60409a; /* mov.l @(0x8,r3),r3 */
L8c60409a: *stop=0x8c60409au;{uint32_t shift=r[0];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}}goto L8c60409c; /* shad r0,r3 */
L8c60409c: *stop=0x8c60409cu;r[0]=r[2];goto L8c60409e; /* mov r2,r0 */
L8c60409e: *stop=0x8c60409eu;if(!valid_access(r[0]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6040a0; /* mov.l @(0x18,r0),r0 */
L8c6040a0: *stop=0x8c6040a0u;r[3]-=r[0];goto L8c6040a2; /* sub r0,r3 */
L8c6040a2: *stop=0x8c6040a2u;r[3]+=4294967295u;goto L8c6040a4; /* add #-0x1,r3 */
L8c6040a4: *stop=0x8c6040a4u;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[3],4);if(io_status)return io_status;goto L8c6040a6; /* mov.l r3,@(0x14,r2) */
L8c6040a6: *stop=0x8c6040a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6040a8; /* mov.l @r15,r3 */
L8c6040a8: *stop=0x8c6040a8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6040aa; /* mov.l @r14,r2 */
L8c6040aa: *stop=0x8c6040aau;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6040ac; /* mov.l r3,@(0xc,r2) */
L8c6040ac: *stop=0x8c6040acu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6040ae; /* mov.l @r14,r3 */
L8c6040ae: *stop=0x8c6040aeu;if(!valid_access(r[3]+28u,4u))return -3;wr(m,bus,&io_status,r[3]+0x1cu,r[4],4);if(io_status)return io_status;goto L8c6040b0; /* mov.l r4,@(0x1c,r3) */
L8c6040b0: *stop=0x8c6040b0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6040b2; /* mov.l @r14,r2 */
L8c6040b2: *stop=0x8c6040b2u;r[6]=64u;goto L8c6040b4; /* mov #0x40,r6 */
L8c6040b4: *stop=0x8c6040b4u;if(!valid_access(r[2]+32u,4u))return -3;wr(m,bus,&io_status,r[2]+0x20u,r[4],4);if(io_status)return io_status;goto L8c6040b6; /* mov.l r4,@(0x20,r2) */
L8c6040b6: *stop=0x8c6040b6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6040b8; /* mov.l @r14,r3 */
L8c6040b8: *stop=0x8c6040b8u;if(!valid_access(r[3]+44u,4u))return -3;wr(m,bus,&io_status,r[3]+0x2cu,r[4],4);if(io_status)return io_status;goto L8c6040ba; /* mov.l r4,@(0x2c,r3) */
L8c6040ba: *stop=0x8c6040bau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6040bc; /* mov.l @r14,r5 */
L8c6040bc: *stop=0x8c6040bcu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6040be; /* mov.l @r14,r4 */
L8c6040be: *stop=0x8c6040beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60411cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6040c0; /* mov.l 0x8c60411c,r3 */
L8c6040c0: *stop=0x8c6040c0u;r[5]+=52u;goto L8c6040c2; /* add #0x34,r5 */
L8c6040c2: target=r[3];*pr=0x8c6040c6u;*stop=0x8c6040c4u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6040c6; /* jsr @r3 */
L8c6040c6: *stop=0x8c6040c6u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c6040c8; /* cmp/eq #-0x1,r0 */
L8c6040c8: take=!(*sr&1u);*stop=0x8c6040cau;r[4]=r[0];if(take)goto L8c6040d0;goto L8c6040cc; /* bf/s 0x8c6040d0 */
L8c6040cc: *stop=0x8c6040ceu;r[0]=4294967295u;goto L8c60413e; /* bra 0x8c60413e */
L8c6040d0: *stop=0x8c6040d0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[13]=loaded;}goto L8c6040d2; /* mov.l @r14,r13 */
L8c6040d2: *stop=0x8c6040d2u;r[6]=12u;goto L8c6040d4; /* mov #0xc,r6 */
L8c6040d4: *stop=0x8c6040d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604120u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6040d6; /* mov.l 0x8c604120,r5 */
L8c6040d6: *stop=0x8c6040d6u;r[13]+=52u;goto L8c6040d8; /* add #0x34,r13 */
L8c6040d8: target=0x8c603becu;*pr=0x8c6040dcu;*stop=0x8c6040dau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6040dc; /* bsr 0x8c603bec */
L8c6040dc: *stop=0x8c6040dcu;r[4]=r[0];goto L8c6040de; /* mov r0,r4 */
L8c6040de: *stop=0x8c6040deu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6040e0; /* tst r4,r4 */
L8c6040e0: if((*sr&1u))goto L8c604104;goto L8c6040e2; /* bt 0x8c604104 */
L8c6040e2: *stop=0x8c6040e2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6040e4; /* mov.l @r14,r4 */
L8c6040e4: *stop=0x8c6040e4u;r[0]=16u;goto L8c6040e6; /* mov #0x10,r0 */
L8c6040e6: *stop=0x8c6040e6u;if(!valid_access(r[0]+r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13]+r[0],1);if(io_status)return io_status;r[2]=loaded;}goto L8c6040e8; /* mov.b @(r0,r13),r2 */
L8c6040e8: *stop=0x8c6040e8u;if(!valid_access(r[4],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[3]=loaded;}goto L8c6040ea; /* mov.w @r4,r3 */
L8c6040ea: *stop=0x8c6040eau;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c6040ec; /* cmp/eq r2,r3 */
L8c6040ec: if(!(*sr&1u))goto L8c604104;goto L8c6040ee; /* bf 0x8c604104 */
L8c6040ee: *stop=0x8c6040eeu;r[0]=17u;goto L8c6040f0; /* mov #0x11,r0 */
L8c6040f0: *stop=0x8c6040f0u;if(!valid_access(r[0]+r[13],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c6040f2; /* mov.b @(r0,r13),r0 */
L8c6040f2: *stop=0x8c6040f2u;if(!valid_access(r[4]+2u,2u))return -3;wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6040f4; /* mov.w r0,@(0x2,r4) */
L8c6040f4: *stop=0x8c6040f4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6040f6; /* mov.l @r14,r4 */
L8c6040f6: *stop=0x8c6040f6u;if(!valid_access(r[4]+2u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6040f8; /* mov.w @(0x2,r4),r0 */
L8c6040f8: *stop=0x8c6040f8u;r[4]=r[0];goto L8c6040fa; /* mov r0,r4 */
L8c6040fa: *stop=0x8c6040fau;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6040fc; /* tst r4,r4 */
L8c6040fc: if((*sr&1u))goto L8c604124;goto L8c6040fe; /* bt 0x8c604124 */
L8c6040fe: *stop=0x8c6040feu;r[0]=r[4];goto L8c604100; /* mov r4,r0 */
L8c604100: *stop=0x8c604100u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c604102; /* cmp/eq #0x1,r0 */
L8c604102: if((*sr&1u))goto L8c604124;goto L8c604104; /* bt 0x8c604124 */
L8c604104: *stop=0x8c604106u;r[0]=4294967288u;goto L8c60413e; /* bra 0x8c60413e */
L8c604124: *stop=0x8c604124u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c604126; /* mov.l @r14,r5 */
L8c604126: *stop=0x8c604126u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c604128; /* mov.l @r14,r4 */
L8c604128: *stop=0x8c604128u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c60412a; /* mov.l @(0x8,r5),r5 */
L8c60412a: target=0x8c603c38u;*pr=0x8c60412eu;*stop=0x8c60412cu;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60412e; /* bsr 0x8c603c38 */
L8c60412e: target=0x8c603d22u;*pr=0x8c604132u;*stop=0x8c604130u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604132; /* bsr 0x8c603d22 */
L8c604132: *stop=0x8c604132u;r[4]=r[0];goto L8c604134; /* mov r0,r4 */
L8c604134: *stop=0x8c604134u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c604136; /* tst r4,r4 */
L8c604136: if((*sr&1u))goto L8c60413c;goto L8c604138; /* bt 0x8c60413c */
L8c604138: *stop=0x8c60413au;r[0]=r[4];goto L8c60413e; /* bra 0x8c60413e */
L8c60413c: *stop=0x8c60413cu;r[0]=0u;goto L8c60413e; /* mov #0x0,r0 */
L8c60413e: *stop=0x8c60413eu;r[15]+=12u;goto L8c604140; /* add #0xc,r15 */
L8c604140: *stop=0x8c604140u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c604142; /* lds.l @r15+,PR */
L8c604142: *stop=0x8c604142u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c604144; /* mov.l @r15+,r13 */
L8c604144: *stop=0x8c604144u;if(*pr!=entry_pr)return -3;*stop=0x8c604146u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c604148(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c604148;
L8c604148: *stop=0x8c604148u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c60414a; /* mov.l r14,@-r15 */
L8c60414a: *stop=0x8c60414au;{uint32_t loaded=rd(m,bus,&io_status,0x8c604320u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c60414c; /* mov.l 0x8c604320,r14 */
L8c60414c: *stop=0x8c60414cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c60414e; /* mov.l r13,@-r15 */
L8c60414e: *stop=0x8c60414eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c604150; /* mov.l r12,@-r15 */
L8c604150: *stop=0x8c604150u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c604152; /* sts.l PR,@-r15 */
L8c604152: *stop=0x8c604152u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[12]=loaded;}goto L8c604154; /* mov.l @r14,r12 */
L8c604154: *stop=0x8c604154u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c604156; /* tst r12,r12 */
L8c604156: take=!(*sr&1u);*stop=0x8c604158u;r[13]=r[4];if(take)goto L8c60415e;goto L8c60415a; /* bf/s 0x8c60415e */
L8c60415a: *stop=0x8c60415cu;r[0]=4294967285u;goto L8c6041ca; /* bra 0x8c6041ca */
L8c60415e: *stop=0x8c60415eu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c604160; /* tst r13,r13 */
L8c604160: if((*sr&1u))goto L8c60416c;goto L8c604162; /* bt 0x8c60416c */
L8c604162: *stop=0x8c604162u;r[0]=r[13];goto L8c604164; /* mov r13,r0 */
L8c604164: *stop=0x8c604164u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c604166; /* cmp/eq #0x1,r0 */
L8c604166: if((*sr&1u))goto L8c60416c;goto L8c604168; /* bt 0x8c60416c */
L8c604168: *stop=0x8c60416au;r[0]=4294967290u;goto L8c6041ca; /* bra 0x8c6041ca */
L8c60416c: *stop=0x8c60416cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604324u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60416e; /* mov.l 0x8c604324,r2 */
L8c60416e: target=r[2];*pr=0x8c604172u;*stop=0x8c604170u;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604172; /* jsr @r2 */
L8c604172: *stop=0x8c604172u;r[4]=r[0];goto L8c604174; /* mov r0,r4 */
L8c604174: *stop=0x8c604174u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c604176; /* tst r4,r4 */
L8c604176: if((*sr&1u))goto L8c60417c;goto L8c604178; /* bt 0x8c60417c */
L8c604178: *stop=0x8c60417au;r[0]=4294967293u;goto L8c6041ca; /* bra 0x8c6041ca */
L8c60417c: *stop=0x8c60417cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[12]=loaded;}goto L8c60417e; /* mov.l @r14,r12 */
L8c60417e: *stop=0x8c60417eu;r[6]=16u;goto L8c604180; /* mov #0x10,r6 */
L8c604180: *stop=0x8c604180u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604328u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c604182; /* mov.l 0x8c604328,r4 */
L8c604182: *stop=0x8c604182u;r[12]+=52u;goto L8c604184; /* add #0x34,r12 */
L8c604184: target=0x8c603c24u;*pr=0x8c604188u;*stop=0x8c604186u;r[5]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604188; /* bsr 0x8c603c24 */
L8c604188: *stop=0x8c604188u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60418a; /* mov.l @r14,r2 */
L8c60418a: *stop=0x8c60418au;r[0]=r[13];goto L8c60418c; /* mov r13,r0 */
L8c60418c: *stop=0x8c60418cu;r[5]=r[12];goto L8c60418e; /* mov r12,r5 */
L8c60418e: *stop=0x8c60418eu;r[6]=18u;goto L8c604190; /* mov #0x12,r6 */
L8c604190: *stop=0x8c604190u;if(!valid_access(r[2]+2u,2u))return -3;wr(m,bus,&io_status,r[2]+0x2u,r[0],2);if(io_status)return io_status;goto L8c604192; /* mov.w r0,@(0x2,r2) */
L8c604192: *stop=0x8c604192u;r[0]=17u;goto L8c604194; /* mov #0x11,r0 */
L8c604194: *stop=0x8c604194u;if(!valid_access(r[0]+r[12],1u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[13],1);if(io_status)return io_status;goto L8c604196; /* mov.b r13,@(r0,r12) */
L8c604196: *stop=0x8c604196u;r[0]=16u;goto L8c604198; /* mov #0x10,r0 */
L8c604198: *stop=0x8c604198u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60419a; /* mov.l @r14,r3 */
L8c60419a: *stop=0x8c60419au;if(!valid_access(r[3],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;r[2]=loaded;}goto L8c60419c; /* mov.b @r3,r2 */
L8c60419c: *stop=0x8c60419cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60432cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60419e; /* mov.l 0x8c60432c,r3 */
L8c60419e: *stop=0x8c60419eu;if(!valid_access(r[0]+r[12],1u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[2],1);if(io_status)return io_status;goto L8c6041a0; /* mov.b r2,@(r0,r12) */
L8c6041a0: *stop=0x8c6041a0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6041a2; /* mov.l @r14,r4 */
L8c6041a2: target=r[3];*pr=0x8c6041a6u;*stop=0x8c6041a4u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6041a6; /* jsr @r3 */
L8c6041a6: *stop=0x8c6041a6u;*sr=(*sr&~1u)|(r[0]==0x00000012u);goto L8c6041a8; /* cmp/eq #0x12,r0 */
L8c6041a8: take=(*sr&1u);*stop=0x8c6041aau;r[4]=r[0];if(take)goto L8c6041b0;goto L8c6041ac; /* bt/s 0x8c6041b0 */
L8c6041ac: *stop=0x8c6041aeu;r[0]=4294967294u;goto L8c6041ca; /* bra 0x8c6041ca */
L8c6041b0: *stop=0x8c6041b0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6041b2; /* mov.l @r14,r5 */
L8c6041b2: *stop=0x8c6041b2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6041b4; /* mov.l @r14,r4 */
L8c6041b4: *stop=0x8c6041b4u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6041b6; /* mov.l @(0x8,r5),r5 */
L8c6041b6: target=0x8c603c38u;*pr=0x8c6041bau;*stop=0x8c6041b8u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6041ba; /* bsr 0x8c603c38 */
L8c6041ba: target=0x8c603d22u;*pr=0x8c6041beu;*stop=0x8c6041bcu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6041be; /* bsr 0x8c603d22 */
L8c6041be: *stop=0x8c6041beu;r[4]=r[0];goto L8c6041c0; /* mov r0,r4 */
L8c6041c0: *stop=0x8c6041c0u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6041c2; /* tst r4,r4 */
L8c6041c2: if((*sr&1u))goto L8c6041c8;goto L8c6041c4; /* bt 0x8c6041c8 */
L8c6041c4: *stop=0x8c6041c6u;r[0]=r[4];goto L8c6041ca; /* bra 0x8c6041ca */
L8c6041c8: *stop=0x8c6041c8u;r[0]=0u;goto L8c6041ca; /* mov #0x0,r0 */
L8c6041ca: *stop=0x8c6041cau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6041cc; /* lds.l @r15+,PR */
L8c6041cc: *stop=0x8c6041ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6041ce; /* mov.l @r15+,r12 */
L8c6041ce: *stop=0x8c6041ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6041d0; /* mov.l @r15+,r13 */
L8c6041d0: *stop=0x8c6041d0u;if(*pr!=entry_pr)return -3;*stop=0x8c6041d2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c6041d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6041d4;
L8c6041d4: *stop=0x8c6041d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604320u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6041d6; /* mov.l 0x8c604320,r7 */
L8c6041d6: *stop=0x8c6041d6u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6041d8; /* mov.l @r7,r6 */
L8c6041d8: *stop=0x8c6041d8u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c6041da; /* tst r6,r6 */
L8c6041da: if(!(*sr&1u))goto L8c6041e0;goto L8c6041dc; /* bf 0x8c6041e0 */
L8c6041dc: *stop=0x8c6041dcu;if(*pr!=entry_pr)return -3;*stop=0x8c6041deu;r[0]=4294967285u;return 0; /* rts */
L8c6041e0: *stop=0x8c6041e0u;if(!valid_access(r[6]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6041e2; /* mov.l @(0x1c,r6),r2 */
L8c6041e2: *stop=0x8c6041e2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6041e4; /* tst r2,r2 */
L8c6041e4: if((*sr&1u))goto L8c6041ea;goto L8c6041e6; /* bt 0x8c6041ea */
L8c6041e6: *stop=0x8c6041e6u;if(*pr!=entry_pr)return -3;*stop=0x8c6041e8u;r[0]=4294967287u;return 0; /* rts */
L8c6041ea: *stop=0x8c6041eau;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c6041ec; /* cmp/pz r4 */
L8c6041ec: if(!(*sr&1u))goto L8c6041f4;goto L8c6041ee; /* bf 0x8c6041f4 */
L8c6041ee: *stop=0x8c6041eeu;if(!valid_access(r[6]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6041f0; /* mov.l @(0x14,r6),r2 */
L8c6041f0: *stop=0x8c6041f0u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]);goto L8c6041f2; /* cmp/ge r2,r4 */
L8c6041f2: if(!(*sr&1u))goto L8c6041f8;goto L8c6041f4; /* bf 0x8c6041f8 */
L8c6041f4: *stop=0x8c6041f4u;if(*pr!=entry_pr)return -3;*stop=0x8c6041f6u;r[0]=4294967291u;return 0; /* rts */
L8c6041f8: *stop=0x8c6041f8u;r[2]=1u;goto L8c6041fa; /* mov #0x1,r2 */
L8c6041fa: *stop=0x8c6041fau;if(!valid_access(r[6]+28u,4u))return -3;wr(m,bus,&io_status,r[6]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c6041fc; /* mov.l r2,@(0x1c,r6) */
L8c6041fc: *stop=0x8c6041fcu;r[2]=2u;goto L8c6041fe; /* mov #0x2,r2 */
L8c6041fe: *stop=0x8c6041feu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c604200; /* mov.l @r7,r3 */
L8c604200: *stop=0x8c604200u;r[0]=0u;goto L8c604202; /* mov #0x0,r0 */
L8c604202: *stop=0x8c604202u;if(!valid_access(r[3]+32u,4u))return -3;wr(m,bus,&io_status,r[3]+0x20u,r[2],4);if(io_status)return io_status;goto L8c604204; /* mov.l r2,@(0x20,r3) */
L8c604204: *stop=0x8c604204u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c604206; /* mov.l @r7,r3 */
L8c604206: *stop=0x8c604206u;if(!valid_access(r[3]+40u,4u))return -3;wr(m,bus,&io_status,r[3]+0x28u,r[4],4);if(io_status)return io_status;goto L8c604208; /* mov.l r4,@(0x28,r3) */
L8c604208: *stop=0x8c604208u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60420a; /* mov.l @r7,r2 */
L8c60420a: *stop=0x8c60420au;if(!valid_access(r[2]+48u,4u))return -3;wr(m,bus,&io_status,r[2]+0x30u,r[5],4);if(io_status)return io_status;goto L8c60420c; /* mov.l r5,@(0x30,r2) */
L8c60420c: *stop=0x8c60420cu;if(*pr!=entry_pr)return -3;*stop=0x8c60420eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c604210(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c604210;
L8c604210: *stop=0x8c604210u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604320u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c604212; /* mov.l 0x8c604320,r7 */
L8c604212: *stop=0x8c604212u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[6]=loaded;}goto L8c604214; /* mov.l @r7,r6 */
L8c604214: *stop=0x8c604214u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c604216; /* tst r6,r6 */
L8c604216: if(!(*sr&1u))goto L8c60421c;goto L8c604218; /* bf 0x8c60421c */
L8c604218: *stop=0x8c604218u;if(*pr!=entry_pr)return -3;*stop=0x8c60421au;r[0]=4294967285u;return 0; /* rts */
L8c60421c: *stop=0x8c60421cu;if(!valid_access(r[6]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60421e; /* mov.l @(0x1c,r6),r2 */
L8c60421e: *stop=0x8c60421eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c604220; /* tst r2,r2 */
L8c604220: if((*sr&1u))goto L8c604226;goto L8c604222; /* bt 0x8c604226 */
L8c604222: *stop=0x8c604222u;if(*pr!=entry_pr)return -3;*stop=0x8c604224u;r[0]=4294967287u;return 0; /* rts */
L8c604226: *stop=0x8c604226u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c604228; /* cmp/pz r4 */
L8c604228: if(!(*sr&1u))goto L8c604230;goto L8c60422a; /* bf 0x8c604230 */
L8c60422a: *stop=0x8c60422au;if(!valid_access(r[6]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60422c; /* mov.l @(0x14,r6),r2 */
L8c60422c: *stop=0x8c60422cu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]);goto L8c60422e; /* cmp/ge r2,r4 */
L8c60422e: if(!(*sr&1u))goto L8c604234;goto L8c604230; /* bf 0x8c604234 */
L8c604230: *stop=0x8c604230u;if(*pr!=entry_pr)return -3;*stop=0x8c604232u;r[0]=4294967291u;return 0; /* rts */
L8c604234: *stop=0x8c604234u;r[2]=2u;goto L8c604236; /* mov #0x2,r2 */
L8c604236: *stop=0x8c604236u;if(!valid_access(r[6]+28u,4u))return -3;wr(m,bus,&io_status,r[6]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c604238; /* mov.l r2,@(0x1c,r6) */
L8c604238: *stop=0x8c604238u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60423a; /* mov.l @r7,r3 */
L8c60423a: *stop=0x8c60423au;if(!valid_access(r[3]+32u,4u))return -3;wr(m,bus,&io_status,r[3]+0x20u,r[2],4);if(io_status)return io_status;goto L8c60423c; /* mov.l r2,@(0x20,r3) */
L8c60423c: *stop=0x8c60423cu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60423e; /* mov.l @r7,r3 */
L8c60423e: *stop=0x8c60423eu;if(!valid_access(r[3]+40u,4u))return -3;wr(m,bus,&io_status,r[3]+0x28u,r[4],4);if(io_status)return io_status;goto L8c604240; /* mov.l r4,@(0x28,r3) */
L8c604240: *stop=0x8c604240u;r[4]=0u;goto L8c604242; /* mov #0x0,r4 */
L8c604242: *stop=0x8c604242u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604244; /* mov.l @r7,r2 */
L8c604244: *stop=0x8c604244u;r[0]=r[4];goto L8c604246; /* mov r4,r0 */
L8c604246: *stop=0x8c604246u;if(!valid_access(r[2]+48u,4u))return -3;wr(m,bus,&io_status,r[2]+0x30u,r[5],4);if(io_status)return io_status;goto L8c604248; /* mov.l r5,@(0x30,r2) */
L8c604248: *stop=0x8c604248u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60424a; /* mov.l @r7,r3 */
L8c60424a: *stop=0x8c60424au;if(!valid_access(r[3]+36u,4u))return -3;wr(m,bus,&io_status,r[3]+0x24u,r[4],4);if(io_status)return io_status;goto L8c60424c; /* mov.l r4,@(0x24,r3) */
L8c60424c: *stop=0x8c60424cu;if(*pr!=entry_pr)return -3;*stop=0x8c60424eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c604250(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c604250;
L8c604250: *stop=0x8c604250u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c604252; /* mov.l r14,@-r15 */
L8c604252: *stop=0x8c604252u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604320u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c604254; /* mov.l 0x8c604320,r14 */
L8c604254: *stop=0x8c604254u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c604256; /* mov.l r13,@-r15 */
L8c604256: *stop=0x8c604256u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c604258; /* sts.l PR,@-r15 */
L8c604258: *stop=0x8c604258u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c60425a; /* mov.l @r14,r4 */
L8c60425a: *stop=0x8c60425au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c60425c; /* tst r4,r4 */
L8c60425c: if(!(*sr&1u))goto L8c604262;goto L8c60425e; /* bf 0x8c604262 */
L8c60425e: *stop=0x8c604260u;r[0]=0u;goto L8c6042ca; /* bra 0x8c6042ca */
L8c604262: *stop=0x8c604262u;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c604264; /* mov.l @(0x1c,r4),r0 */
L8c604264: *stop=0x8c604264u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c604266; /* cmp/eq #0x0,r0 */
L8c604266: take=(*sr&1u);*stop=0x8c604268u;r[13]=0u;if(take)goto L8c6042c6;goto L8c60426a; /* bt/s 0x8c6042c6 */
L8c60426a: *stop=0x8c60426au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c60426c; /* cmp/eq #0x1,r0 */
L8c60426c: if((*sr&1u))goto L8c604276;goto L8c60426e; /* bt 0x8c604276 */
L8c60426e: *stop=0x8c60426eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c604270; /* cmp/eq #0x2,r0 */
L8c604270: if((*sr&1u))goto L8c604298;goto L8c604272; /* bt 0x8c604298 */
L8c604272: *stop=0x8c604274u;goto L8c6042c0; /* bra 0x8c6042c0 */
L8c604276: target=0x8c603f8cu;*pr=0x8c60427au;*stop=0x8c604278u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60427a; /* bsr 0x8c603f8c */
L8c60427a: *stop=0x8c60427au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60427c; /* mov.l @r14,r2 */
L8c60427c: *stop=0x8c60427cu;r[4]=r[0];goto L8c60427e; /* mov r0,r4 */
L8c60427e: *stop=0x8c60427eu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c604280; /* tst r4,r4 */
L8c604280: take=(*sr&1u);*stop=0x8c604282u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[13],4);if(io_status)return io_status;if(take)goto L8c60428e;goto L8c604284; /* bt/s 0x8c60428e */
L8c604284: *stop=0x8c604284u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c604286; /* mov.l @r14,r2 */
L8c604286: *stop=0x8c604286u;r[3]=3u;goto L8c604288; /* mov #0x3,r3 */
L8c604288: *stop=0x8c604288u;if(!valid_access(r[2]+32u,4u))return -3;wr(m,bus,&io_status,r[2]+0x20u,r[3],4);if(io_status)return io_status;goto L8c60428a; /* mov.l r3,@(0x20,r2) */
L8c60428a: *stop=0x8c60428cu;goto L8c6042ba; /* bra 0x8c6042ba */
L8c60428e: *stop=0x8c60428eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c604290; /* mov.l @r14,r1 */
L8c604290: *stop=0x8c604290u;r[3]=1u;goto L8c604292; /* mov #0x1,r3 */
L8c604292: *stop=0x8c604292u;if(!valid_access(r[1]+32u,4u))return -3;wr(m,bus,&io_status,r[1]+0x20u,r[3],4);if(io_status)return io_status;goto L8c604294; /* mov.l r3,@(0x20,r1) */
L8c604294: *stop=0x8c604296u;goto L8c6042ba; /* bra 0x8c6042ba */
L8c604298: target=0x8c603fe4u;*pr=0x8c60429cu;*stop=0x8c60429au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60429c; /* bsr 0x8c603fe4 */
L8c60429c: *stop=0x8c60429cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c60429e; /* cmp/eq #0x1,r0 */
L8c60429e: take=(*sr&1u);*stop=0x8c6042a0u;r[4]=r[0];if(take)goto L8c6042c6;goto L8c6042a2; /* bt/s 0x8c6042c6 */
L8c6042a2: *stop=0x8c6042a2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6042a4; /* mov.l @r14,r3 */
L8c6042a4: *stop=0x8c6042a4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6042a6; /* tst r4,r4 */
L8c6042a6: take=(*sr&1u);*stop=0x8c6042a8u;if(!valid_access(r[3]+28u,4u))return -3;wr(m,bus,&io_status,r[3]+0x1cu,r[13],4);if(io_status)return io_status;if(take)goto L8c6042b4;goto L8c6042aa; /* bt/s 0x8c6042b4 */
L8c6042aa: *stop=0x8c6042aau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6042ac; /* mov.l @r14,r3 */
L8c6042ac: *stop=0x8c6042acu;r[2]=3u;goto L8c6042ae; /* mov #0x3,r2 */
L8c6042ae: *stop=0x8c6042aeu;if(!valid_access(r[3]+32u,4u))return -3;wr(m,bus,&io_status,r[3]+0x20u,r[2],4);if(io_status)return io_status;goto L8c6042b0; /* mov.l r2,@(0x20,r3) */
L8c6042b0: *stop=0x8c6042b2u;goto L8c6042ba; /* bra 0x8c6042ba */
L8c6042b4: *stop=0x8c6042b4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6042b6; /* mov.l @r14,r1 */
L8c6042b6: *stop=0x8c6042b6u;r[3]=1u;goto L8c6042b8; /* mov #0x1,r3 */
L8c6042b8: *stop=0x8c6042b8u;if(!valid_access(r[1]+32u,4u))return -3;wr(m,bus,&io_status,r[1]+0x20u,r[3],4);if(io_status)return io_status;goto L8c6042ba; /* mov.l r3,@(0x20,r1) */
L8c6042ba: *stop=0x8c6042bau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6042bc; /* mov.l @r14,r2 */
L8c6042bc: *stop=0x8c6042beu;if(!valid_access(r[2]+44u,4u))return -3;wr(m,bus,&io_status,r[2]+0x2cu,r[4],4);if(io_status)return io_status;goto L8c6042c6; /* bra 0x8c6042c6 */
L8c6042c0: *stop=0x8c6042c0u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[13],4);if(io_status)return io_status;goto L8c6042c2; /* mov.l r13,@(0x1c,r4) */
L8c6042c2: *stop=0x8c6042c2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6042c4; /* mov.l @r14,r2 */
L8c6042c4: *stop=0x8c6042c4u;if(!valid_access(r[2]+32u,4u))return -3;wr(m,bus,&io_status,r[2]+0x20u,r[13],4);if(io_status)return io_status;goto L8c6042c6; /* mov.l r13,@(0x20,r2) */
L8c6042c6: *stop=0x8c6042c6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6042c8; /* mov.l @r14,r0 */
L8c6042c8: *stop=0x8c6042c8u;if(!valid_access(r[0]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6042ca; /* mov.l @(0x20,r0),r0 */
L8c6042ca: *stop=0x8c6042cau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6042cc; /* lds.l @r15+,PR */
L8c6042cc: *stop=0x8c6042ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6042ce; /* mov.l @r15+,r13 */
L8c6042ce: *stop=0x8c6042ceu;if(*pr!=entry_pr)return -3;*stop=0x8c6042d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603bec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c603bec;
L8c603bec: *stop=0x8c603beeu;if(!call)return -2;return call(ctx,0x8c603bfcu,m,f,sr,r,pr); /* bra 0x8c603bfc */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603c24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c603c24;
L8c603c24: *stop=0x8c603c26u;if(!call)return -2;return call(ctx,0x8c603c2eu,m,f,sr,r,pr); /* bra 0x8c603c2e */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603c38(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c603c38;
L8c603c38: *stop=0x8c603c38u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c603c3a; /* cmp/pl r5 */
L8c603c3a: take=!(*sr&1u);*stop=0x8c603c3cu;r[6]=4294967295u;if(take)goto L8c603c48;goto L8c603c3e; /* bf/s 0x8c603c48 */
L8c603c3e: *stop=0x8c603c3eu;r[5]+=4294967295u;goto L8c603c40; /* add #-0x1,r5 */
L8c603c40: *stop=0x8c603c40u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[6],1);if(io_status)return io_status;goto L8c603c42; /* mov.b r6,@r4 */
L8c603c42: *stop=0x8c603c42u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c603c44; /* cmp/pl r5 */
L8c603c44: take=(*sr&1u);*stop=0x8c603c46u;r[4]+=1u;if(take)goto L8c603c3e;goto L8c603c48; /* bt/s 0x8c603c3e */
L8c603c48: *stop=0x8c603c48u;if(*pr!=entry_pr)return -3;*stop=0x8c603c4au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603d22(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c603d22;
L8c603d22: *stop=0x8c603d22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c603d24; /* mov.l r14,@-r15 */
L8c603d24: *stop=0x8c603d24u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c603d26; /* mov.l r13,@-r15 */
L8c603d26: *stop=0x8c603d26u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c603d28; /* mov.l r12,@-r15 */
L8c603d28: *stop=0x8c603d28u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c603d2a; /* mov.l r11,@-r15 */
L8c603d2a: *stop=0x8c603d2au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c603d2c; /* mov.l r10,@-r15 */
L8c603d2c: *stop=0x8c603d2cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c603d2e; /* mov.l r9,@-r15 */
L8c603d2e: *stop=0x8c603d2eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c603d30; /* mov.l r8,@-r15 */
L8c603d30: *stop=0x8c603d30u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c603d32; /* sts.l PR,@-r15 */
L8c603d32: *stop=0x8c603d32u;r[15]+=4294967292u;goto L8c603d34; /* add #-0x4,r15 */
L8c603d34: *stop=0x8c603d34u;r[13]=r[4];goto L8c603d36; /* mov r4,r13 */
L8c603d36: *stop=0x8c603d36u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c603d38; /* mov.l @(0xc,r13),r10 */
L8c603d38: *stop=0x8c603d38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c603da4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c603d3a; /* mov.l 0x8c603da4,r11 */
L8c603d3a: *stop=0x8c603d3au;r[6]=64u;goto L8c603d3c; /* mov #0x40,r6 */
L8c603d3c: *stop=0x8c603d3cu;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c603d3e; /* mov.l @(0x4,r13),r14 */
L8c603d3e: *stop=0x8c603d3eu;r[5]=r[10];goto L8c603d40; /* mov r10,r5 */
L8c603d40: target=r[11];*pr=0x8c603d44u;*stop=0x8c603d42u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603d44; /* jsr @r11 */
L8c603d44: *stop=0x8c603d44u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c603d46; /* cmp/eq #-0x1,r0 */
L8c603d46: take=(*sr&1u);*stop=0x8c603d48u;r[4]=r[0];if(take)goto L8c603d98;goto L8c603d4a; /* bt/s 0x8c603d98 */
L8c603d4a: *stop=0x8c603d4au;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c603d4c; /* mov.l @(0x14,r13),r12 */
L8c603d4c: *stop=0x8c603d4cu;r[4]=r[14];goto L8c603d4e; /* mov r14,r4 */
L8c603d4e: *stop=0x8c603d4eu;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c603d50; /* mov.l @(0x18,r13),r6 */
L8c603d50: *stop=0x8c603d50u;r[12]+=1u;goto L8c603d52; /* add #0x1,r12 */
L8c603d52: *stop=0x8c603d52u;r[12]<<=2;goto L8c603d54; /* shll2 r12 */
L8c603d54: *stop=0x8c603d54u;r[12]<<=2;goto L8c603d56; /* shll2 r12 */
L8c603d56: *stop=0x8c603d56u;r[12]<<=2;goto L8c603d58; /* shll2 r12 */
L8c603d58: *stop=0x8c603d58u;r[5]=r[12];goto L8c603d5a; /* mov r12,r5 */
L8c603d5a: *stop=0x8c603d5au;r[6]<<=2;goto L8c603d5c; /* shll2 r6 */
L8c603d5c: *stop=0x8c603d5cu;r[5]+=r[10];goto L8c603d5e; /* add r10,r5 */
L8c603d5e: *stop=0x8c603d5eu;r[6]<<=2;goto L8c603d60; /* shll2 r6 */
L8c603d60: *stop=0x8c603d60u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c603d62; /* mov.l r5,@r15 */
L8c603d62: *stop=0x8c603d62u;r[6]<<=2;goto L8c603d64; /* shll2 r6 */
L8c603d64: target=r[11];*pr=0x8c603d68u;*stop=0x8c603d66u;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603d68; /* jsr @r11 */
L8c603d68: *stop=0x8c603d68u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c603d6a; /* cmp/eq #-0x1,r0 */
L8c603d6a: take=(*sr&1u);*stop=0x8c603d6cu;r[4]=r[0];if(take)goto L8c603d98;goto L8c603d6e; /* bt/s 0x8c603d98 */
L8c603d6e: *stop=0x8c603d6eu;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c603d70; /* mov.l @(0x18,r13),r5 */
L8c603d70: target=0x8c603c4cu;*pr=0x8c603d74u;*stop=0x8c603d72u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603d74; /* bsr 0x8c603c4c */
L8c603d74: *stop=0x8c603d74u;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[0],4);if(io_status)return io_status;goto L8c603d76; /* mov.l r0,@(0x10,r13) */
L8c603d76: *stop=0x8c603d76u;r[12]=r[0];goto L8c603d78; /* mov r0,r12 */
L8c603d78: *stop=0x8c603d78u;if(!valid_access(r[13]+2u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c603d7a; /* mov.w @(0x2,r13),r0 */
L8c603d7a: *stop=0x8c603d7au;r[9]=r[14];goto L8c603d7c; /* mov r14,r9 */
L8c603d7c: *stop=0x8c603d7cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c603d7e; /* tst r0,r0 */
L8c603d7e: take=!(*sr&1u);*stop=0x8c603d80u;r[9]+=64u;if(take)goto L8c603df2;goto L8c603d82; /* bf/s 0x8c603df2 */
L8c603d82: *stop=0x8c603d82u;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c603d84; /* cmp/pl r12 */
L8c603d84: *stop=0x8c603d84u;r[14]=r[13];goto L8c603d86; /* mov r13,r14 */
L8c603d86: take=!(*sr&1u);*stop=0x8c603d88u;r[14]+=52u;if(take)goto L8c603dee;goto L8c603d8a; /* bf/s 0x8c603dee */
L8c603d8a: *stop=0x8c603d8au;r[5]=r[14];goto L8c603d8c; /* mov r14,r5 */
L8c603d8c: *stop=0x8c603d8cu;r[6]=64u;goto L8c603d8e; /* mov #0x40,r6 */
L8c603d8e: target=r[11];*pr=0x8c603d92u;*stop=0x8c603d90u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603d92; /* jsr @r11 */
L8c603d92: *stop=0x8c603d92u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c603d94; /* cmp/eq #-0x1,r0 */
L8c603d94: take=!(*sr&1u);*stop=0x8c603d96u;r[4]=r[0];if(take)goto L8c603da8;goto L8c603d98; /* bf/s 0x8c603da8 */
L8c603d98: *stop=0x8c603d9au;r[0]=4294967295u;goto L8c603df4; /* bra 0x8c603df4 */
L8c603da8: *stop=0x8c603da8u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[2]=loaded;}goto L8c603daa; /* mov.w @r14,r2 */
L8c603daa: *stop=0x8c603daau;{uint32_t loaded=rd(m,bus,&io_status,0x8c603ffcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c603dac; /* mov.l 0x8c603ffc,r4 */
L8c603dac: *stop=0x8c603dacu;r[2]=r[2]&65535u;goto L8c603dae; /* extu.w r2,r2 */
L8c603dae: *stop=0x8c603daeu;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c603db0; /* cmp/eq r4,r2 */
L8c603db0: if((*sr&1u))goto L8c603dee;goto L8c603db2; /* bt 0x8c603dee */
L8c603db2: *stop=0x8c603db2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604000u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c603db4; /* mov.l 0x8c604000,r8 */
L8c603db4: *stop=0x8c603db4u;r[5]=r[14];goto L8c603db6; /* mov r14,r5 */
L8c603db6: target=r[8];*pr=0x8c603dbau;*stop=0x8c603db8u;r[4]=62u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603dba; /* jsr @r8 */
L8c603dba: *stop=0x8c603dbau;r[4]=r[0];goto L8c603dbc; /* mov r0,r4 */
L8c603dbc: *stop=0x8c603dbcu;r[0]=62u;goto L8c603dbe; /* mov #0x3e,r0 */
L8c603dbe: *stop=0x8c603dbeu;if(!valid_access(r[0]+r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;r[3]=loaded;}goto L8c603dc0; /* mov.w @(r0,r14),r3 */
L8c603dc0: *stop=0x8c603dc0u;r[4]=r[4]&65535u;goto L8c603dc2; /* extu.w r4,r4 */
L8c603dc2: *stop=0x8c603dc2u;r[3]=r[3]&65535u;goto L8c603dc4; /* extu.w r3,r3 */
L8c603dc4: *stop=0x8c603dc4u;*sr=(*sr&~1u)|(r[4]==r[3]);goto L8c603dc6; /* cmp/eq r4,r3 */
L8c603dc6: if(!(*sr&1u))goto L8c603de6;goto L8c603dc8; /* bf 0x8c603de6 */
L8c603dc8: *stop=0x8c603dc8u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[2]=loaded;}goto L8c603dca; /* mov.w @r14,r2 */
L8c603dca: *stop=0x8c603dcau;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603dcc; /* mov.l @(0x14,r13),r3 */
L8c603dcc: *stop=0x8c603dccu;r[2]=r[2]&65535u;goto L8c603dce; /* extu.w r2,r2 */
L8c603dce: *stop=0x8c603dceu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c603dd0; /* cmp/ge r3,r2 */
L8c603dd0: if((*sr&1u))goto L8c603de6;goto L8c603dd2; /* bt 0x8c603de6 */
L8c603dd2: *stop=0x8c603dd2u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[5]=loaded;}goto L8c603dd4; /* mov.w @r14,r5 */
L8c603dd4: *stop=0x8c603dd4u;r[6]=64u;goto L8c603dd6; /* mov #0x40,r6 */
L8c603dd6: *stop=0x8c603dd6u;r[5]=r[5]&65535u;goto L8c603dd8; /* extu.w r5,r5 */
L8c603dd8: *stop=0x8c603dd8u;r[5]+=1u;goto L8c603dda; /* add #0x1,r5 */
L8c603dda: *stop=0x8c603ddau;r[5]<<=2;goto L8c603ddc; /* shll2 r5 */
L8c603ddc: *stop=0x8c603ddcu;r[5]<<=2;goto L8c603dde; /* shll2 r5 */
L8c603dde: *stop=0x8c603ddeu;r[5]<<=2;goto L8c603de0; /* shll2 r5 */
L8c603de0: *stop=0x8c603de0u;r[5]+=r[10];goto L8c603de2; /* add r10,r5 */
L8c603de2: target=0x8c603c08u;*pr=0x8c603de6u;*stop=0x8c603de4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603de6; /* bsr 0x8c603c08 */
L8c603de6: *stop=0x8c603de6u;r[12]+=4294967295u;goto L8c603de8; /* add #-0x1,r12 */
L8c603de8: *stop=0x8c603de8u;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c603dea; /* cmp/pl r12 */
L8c603dea: take=(*sr&1u);*stop=0x8c603decu;r[9]+=64u;if(take)goto L8c603d8a;goto L8c603dee; /* bt/s 0x8c603d8a */
L8c603dee: *stop=0x8c603df0u;r[0]=0u;goto L8c603df4; /* bra 0x8c603df4 */
L8c603df2: *stop=0x8c603df2u;r[0]=4294967290u;goto L8c603df4; /* mov #-0x6,r0 */
L8c603df4: *stop=0x8c603df4u;r[15]+=4u;goto L8c603df6; /* add #0x4,r15 */
L8c603df6: *stop=0x8c603df6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c603df8; /* lds.l @r15+,PR */
L8c603df8: *stop=0x8c603df8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c603dfa; /* mov.l @r15+,r8 */
L8c603dfa: *stop=0x8c603dfau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c603dfc; /* mov.l @r15+,r9 */
L8c603dfc: *stop=0x8c603dfcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c603dfe; /* mov.l @r15+,r10 */
L8c603dfe: *stop=0x8c603dfeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c603e00; /* mov.l @r15+,r11 */
L8c603e00: *stop=0x8c603e00u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c603e02; /* mov.l @r15+,r12 */
L8c603e02: *stop=0x8c603e02u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c603e04; /* mov.l @r15+,r13 */
L8c603e04: *stop=0x8c603e04u;if(*pr!=entry_pr)return -3;*stop=0x8c603e06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603f8c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c603f8c;
L8c603f8c: *stop=0x8c603f8cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c603f8e; /* mov.l r14,@-r15 */
L8c603f8e: *stop=0x8c603f8eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c603f90; /* mov.l r13,@-r15 */
L8c603f90: *stop=0x8c603f90u;r[13]=r[4];goto L8c603f92; /* mov r4,r13 */
L8c603f92: *stop=0x8c603f92u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c603f94; /* sts.l PR,@-r15 */
L8c603f94: *stop=0x8c603f94u;if(!valid_access(r[13]+2u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c603f96; /* mov.w @(0x2,r13),r0 */
L8c603f96: *stop=0x8c603f96u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c603f98; /* tst r0,r0 */
L8c603f98: if(!(*sr&1u))goto L8c603fda;goto L8c603f9a; /* bf 0x8c603fda */
L8c603f9a: *stop=0x8c603f9au;if(!valid_access(r[13]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x28u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c603f9c; /* mov.l @(0x28,r13),r14 */
L8c603f9c: *stop=0x8c603f9cu;r[6]=60u;goto L8c603f9e; /* mov #0x3c,r6 */
L8c603f9e: *stop=0x8c603f9eu;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603fa0; /* mov.l @(0xc,r13),r3 */
L8c603fa0: *stop=0x8c603fa0u;r[14]+=1u;goto L8c603fa2; /* add #0x1,r14 */
L8c603fa2: *stop=0x8c603fa2u;if(!valid_access(r[13]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x30u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c603fa4; /* mov.l @(0x30,r13),r5 */
L8c603fa4: *stop=0x8c603fa4u;r[14]<<=2;goto L8c603fa6; /* shll2 r14 */
L8c603fa6: *stop=0x8c603fa6u;r[14]<<=2;goto L8c603fa8; /* shll2 r14 */
L8c603fa8: *stop=0x8c603fa8u;r[14]<<=2;goto L8c603faa; /* shll2 r14 */
L8c603faa: *stop=0x8c603faau;r[14]+=r[3];goto L8c603fac; /* add r3,r14 */
L8c603fac: *stop=0x8c603facu;r[4]=r[14];goto L8c603fae; /* mov r14,r4 */
L8c603fae: target=0x8c603c24u;*pr=0x8c603fb2u;*stop=0x8c603fb0u;r[4]+=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603fb2; /* bsr 0x8c603c24 */
L8c603fb2: *stop=0x8c603fb2u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[3]=loaded;}goto L8c603fb4; /* mov.w @r14,r3 */
L8c603fb4: *stop=0x8c603fb4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c603ffcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c603fb6; /* mov.l 0x8c603ffc,r2 */
L8c603fb6: *stop=0x8c603fb6u;r[3]=r[3]&65535u;goto L8c603fb8; /* extu.w r3,r3 */
L8c603fb8: *stop=0x8c603fb8u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c603fba; /* cmp/eq r2,r3 */
L8c603fba: if((*sr&1u))goto L8c603fd6;goto L8c603fbc; /* bt 0x8c603fd6 */
L8c603fbc: *stop=0x8c603fbcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604000u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c603fbe; /* mov.l 0x8c604000,r1 */
L8c603fbe: *stop=0x8c603fbeu;r[5]=r[14];goto L8c603fc0; /* mov r14,r5 */
L8c603fc0: target=r[1];*pr=0x8c603fc4u;*stop=0x8c603fc2u;r[4]=62u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603fc4; /* jsr @r1 */
L8c603fc4: *stop=0x8c603fc4u;r[4]=r[0];goto L8c603fc6; /* mov r0,r4 */
L8c603fc6: *stop=0x8c603fc6u;r[0]=62u;goto L8c603fc8; /* mov #0x3e,r0 */
L8c603fc8: *stop=0x8c603fc8u;if(!valid_access(r[0]+r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;r[3]=loaded;}goto L8c603fca; /* mov.w @(r0,r14),r3 */
L8c603fca: *stop=0x8c603fcau;r[4]=r[4]&65535u;goto L8c603fcc; /* extu.w r4,r4 */
L8c603fcc: *stop=0x8c603fccu;r[3]=r[3]&65535u;goto L8c603fce; /* extu.w r3,r3 */
L8c603fce: *stop=0x8c603fceu;*sr=(*sr&~1u)|(r[4]==r[3]);goto L8c603fd0; /* cmp/eq r4,r3 */
L8c603fd0: if((*sr&1u))goto L8c603fd6;goto L8c603fd2; /* bt 0x8c603fd6 */
L8c603fd2: *stop=0x8c603fd4u;r[0]=4294967292u;goto L8c603fdc; /* bra 0x8c603fdc */
L8c603fd6: *stop=0x8c603fd8u;r[0]=0u;goto L8c603fdc; /* bra 0x8c603fdc */
L8c603fda: *stop=0x8c603fdau;r[0]=4294967290u;goto L8c603fdc; /* mov #-0x6,r0 */
L8c603fdc: *stop=0x8c603fdcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c603fde; /* lds.l @r15+,PR */
L8c603fde: *stop=0x8c603fdeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c603fe0; /* mov.l @r15+,r13 */
L8c603fe0: *stop=0x8c603fe0u;if(*pr!=entry_pr)return -3;*stop=0x8c603fe2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603fe4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c603fe4;
L8c603fe4: *stop=0x8c603fe4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c603fe6; /* mov.l r14,@-r15 */
L8c603fe6: *stop=0x8c603fe6u;r[14]=r[4];goto L8c603fe8; /* mov r4,r14 */
L8c603fe8: *stop=0x8c603fe8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c603fea; /* sts.l PR,@-r15 */
L8c603fea: *stop=0x8c603feau;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c603fec; /* mov.l @(0x24,r14),r0 */
L8c603fec: *stop=0x8c603fecu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c603fee; /* cmp/eq #0x0,r0 */
L8c603fee: if((*sr&1u))goto L8c604008;goto L8c603ff0; /* bt 0x8c604008 */
L8c603ff0: *stop=0x8c603ff0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c603ff2; /* cmp/eq #0x1,r0 */
L8c603ff2: if((*sr&1u))goto L8c60401a;goto L8c603ff4; /* bt 0x8c60401a */
L8c603ff4: *stop=0x8c603ff4u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c603ff6; /* cmp/eq #0x2,r0 */
L8c603ff6: if((*sr&1u))goto L8c604032;goto L8c603ff8; /* bt 0x8c604032 */
L8c603ff8: *stop=0x8c603ffau;goto L8c60403a; /* bra 0x8c60403a */
L8c604008: target=0x8c603ec4u;*pr=0x8c60400cu;*stop=0x8c60400au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60400c; /* bsr 0x8c603ec4 */
L8c60400c: *stop=0x8c60400cu;*sr=(*sr&~1u)|(r[0]==0xfffffff6u);goto L8c60400e; /* cmp/eq #-0xa,r0 */
L8c60400e: take=!(*sr&1u);*stop=0x8c604010u;r[5]=r[0];if(take)goto L8c604016;goto L8c604012; /* bf/s 0x8c604016 */
L8c604012: *stop=0x8c604014u;r[3]=1u;goto L8c60402c; /* bra 0x8c60402c */
L8c604016: *stop=0x8c604018u;r[4]=r[5];goto L8c60403c; /* bra 0x8c60403c */
L8c60401a: *stop=0x8c60401au;{uint32_t loaded=rd(m,bus,&io_status,0x8c60410cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60401c; /* mov.l 0x8c60410c,r2 */
L8c60401c: target=r[2];*pr=0x8c604020u;*stop=0x8c60401eu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604020; /* jsr @r2 */
L8c604020: *stop=0x8c604020u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c604022; /* cmp/eq #-0x1,r0 */
L8c604022: take=!(*sr&1u);*stop=0x8c604024u;r[4]=r[0];if(take)goto L8c60402a;goto L8c604026; /* bf/s 0x8c60402a */
L8c604026: *stop=0x8c604028u;r[4]=4294967293u;goto L8c60403c; /* bra 0x8c60403c */
L8c60402a: *stop=0x8c60402au;r[3]=2u;goto L8c60402c; /* mov #0x2,r3 */
L8c60402c: *stop=0x8c60402cu;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[3],4);if(io_status)return io_status;goto L8c60402e; /* mov.l r3,@(0x24,r14) */
L8c60402e: *stop=0x8c604030u;r[4]=1u;goto L8c60403c; /* bra 0x8c60403c */
L8c604032: target=0x8c603e08u;*pr=0x8c604036u;*stop=0x8c604034u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c604036; /* bsr 0x8c603e08 */
L8c604036: *stop=0x8c604038u;r[4]=r[0];goto L8c60403c; /* bra 0x8c60403c */
L8c60403a: *stop=0x8c60403au;r[4]=4294967294u;goto L8c60403c; /* mov #-0x2,r4 */
L8c60403c: *stop=0x8c60403cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c60403e; /* lds.l @r15+,PR */
L8c60403e: *stop=0x8c60403eu;r[0]=r[4];goto L8c604040; /* mov r4,r0 */
L8c604040: *stop=0x8c604040u;if(*pr!=entry_pr)return -3;*stop=0x8c604042u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c10da3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10da3c;
L8c10da3c: *stop=0x8c10da3cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10da48u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10da3e; /* mov.l 0x8c10da48,r7 */
L8c10da3e: *stop=0x8c10da3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10da4cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10da40; /* mov.l 0x8c10da4c,r0 */
L8c10da40: *stop=0x8c10da40u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10da42; /* mov.l @r0,r0 */
L8c10da42: target=r[0];*stop=0x8c10da44u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c10da64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10da64;
L8c10da64: *stop=0x8c10da64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10da70u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10da66; /* mov.l 0x8c10da70,r7 */
L8c10da66: *stop=0x8c10da66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10da74u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10da68; /* mov.l 0x8c10da74,r0 */
L8c10da68: *stop=0x8c10da68u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10da6a; /* mov.l @r0,r0 */
L8c10da6a: target=r[0];*stop=0x8c10da6cu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c10da78(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10da78;
L8c10da78: *stop=0x8c10da78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10da84u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10da7a; /* mov.l 0x8c10da84,r7 */
L8c10da7a: *stop=0x8c10da7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10da88u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10da7c; /* mov.l 0x8c10da88,r0 */
L8c10da7c: *stop=0x8c10da7cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10da7e; /* mov.l @r0,r0 */
L8c10da7e: target=r[0];*stop=0x8c10da80u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603bfc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c603bfc;
L8c603bf0: *stop=0x8c603bf0u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[2]=loaded;}r[5]+=1;goto L8c603bf2; /* mov.b @r5+,r2 */
L8c603bf2: *stop=0x8c603bf2u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}r[4]+=1;goto L8c603bf4; /* mov.b @r4+,r3 */
L8c603bf4: *stop=0x8c603bf4u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c603bf6; /* cmp/eq r2,r3 */
L8c603bf6: if((*sr&1u))goto L8c603bfc;goto L8c603bf8; /* bt 0x8c603bfc */
L8c603bf8: *stop=0x8c603bf8u;if(*pr!=entry_pr)return -3;*stop=0x8c603bfau;r[0]=0u;return 0; /* rts */
L8c603bfc: *stop=0x8c603bfcu;*sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c603bfe; /* cmp/pl r6 */
L8c603bfe: take=(*sr&1u);*stop=0x8c603c00u;r[6]+=4294967295u;if(take)goto L8c603bf0;goto L8c603c02; /* bt/s 0x8c603bf0 */
L8c603c02: *stop=0x8c603c02u;r[0]=1u;goto L8c603c04; /* mov #0x1,r0 */
L8c603c04: *stop=0x8c603c04u;if(*pr!=entry_pr)return -3;*stop=0x8c603c06u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603c2e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c603c2e;
L8c603c28: *stop=0x8c603c28u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[3]=loaded;}r[4]+=1;goto L8c603c2a; /* mov.b @r4+,r3 */
L8c603c2a: *stop=0x8c603c2au;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c603c2c; /* mov.b r3,@r5 */
L8c603c2c: *stop=0x8c603c2cu;r[5]+=1u;goto L8c603c2e; /* add #0x1,r5 */
L8c603c2e: *stop=0x8c603c2eu;*sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c603c30; /* cmp/pl r6 */
L8c603c30: take=(*sr&1u);*stop=0x8c603c32u;r[6]+=4294967295u;if(take)goto L8c603c28;goto L8c603c34; /* bt/s 0x8c603c28 */
L8c603c34: *stop=0x8c603c34u;if(*pr!=entry_pr)return -3;*stop=0x8c603c36u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603c4c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c603c4c;
L8c603c4c: *stop=0x8c603c4cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c603c4e; /* mov.l r14,@-r15 */
L8c603c4e: *stop=0x8c603c4eu;r[14]=r[5];goto L8c603c50; /* mov r5,r14 */
L8c603c50: *stop=0x8c603c50u;r[14]<<=2;goto L8c603c52; /* shll2 r14 */
L8c603c52: *stop=0x8c603c52u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c603c54; /* mov.l r12,@-r15 */
L8c603c54: *stop=0x8c603c54u;r[12]=8u;goto L8c603c56; /* mov #0x8,r12 */
L8c603c56: *stop=0x8c603c56u;r[14]<<=2;goto L8c603c58; /* shll2 r14 */
L8c603c58: *stop=0x8c603c58u;r[0]=r[12];goto L8c603c5a; /* mov r12,r0 */
L8c603c5a: *stop=0x8c603c5au;r[7]=0u;goto L8c603c5c; /* mov #0x0,r7 */
L8c603c5c: *stop=0x8c603c5cu;r[14]<<=2;goto L8c603c5e; /* shll2 r14 */
L8c603c5e: *stop=0x8c603c60u;r[0]+=120u;goto L8c603c78; /* bra 0x8c603c78 */
L8c603c62: *stop=0x8c603c62u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[6]=loaded;}r[4]+=1;goto L8c603c64; /* mov.b @r4+,r6 */
L8c603c64: *stop=0x8c603c64u;r[5]=r[12];goto L8c603c66; /* mov r12,r5 */
L8c603c66: *stop=0x8c603c66u;r[3]=(uint32_t)(int32_t)(int8_t)r[6];goto L8c603c68; /* exts.b r6,r3 */
L8c603c68: *stop=0x8c603c68u;*sr=(*sr&~1u)|!(r[0]&r[3]);goto L8c603c6a; /* tst r0,r3 */
L8c603c6a: if(!(*sr&1u))goto L8c603c7c;goto L8c603c6c; /* bf 0x8c603c7c */
L8c603c6c: *stop=0x8c603c6cu;r[5]+=4294967295u;goto L8c603c6e; /* add #-0x1,r5 */
L8c603c6e: *stop=0x8c603c6eu;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c603c70; /* shll r6 */
L8c603c70: *stop=0x8c603c70u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c603c72; /* cmp/pl r5 */
L8c603c72: take=(*sr&1u);*stop=0x8c603c74u;r[7]+=1u;if(take)goto L8c603c66;goto L8c603c76; /* bt/s 0x8c603c66 */
L8c603c76: *stop=0x8c603c76u;r[14]+=4294967295u;goto L8c603c78; /* add #-0x1,r14 */
L8c603c78: *stop=0x8c603c78u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c603c7a; /* cmp/pl r14 */
L8c603c7a: if((*sr&1u))goto L8c603c62;goto L8c603c7c; /* bt 0x8c603c62 */
L8c603c7c: *stop=0x8c603c7cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c603c7e; /* mov.l @r15+,r12 */
L8c603c7e: *stop=0x8c603c7eu;r[0]=r[7];goto L8c603c80; /* mov r7,r0 */
L8c603c80: *stop=0x8c603c80u;if(*pr!=entry_pr)return -3;*stop=0x8c603c82u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603c08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c603c08;
L8c603c08: *stop=0x8c603c08u;*sr=(*sr&~1u)|((int32_t)r[6]>=0);goto L8c603c0a; /* cmp/pz r6 */
L8c603c0a: if((*sr&1u))goto L8c603c0e;goto L8c603c0c; /* bt 0x8c603c0e */
L8c603c0c: *stop=0x8c603c0cu;r[6]+=3u;goto L8c603c0e; /* add #0x3,r6 */
L8c603c0e: *stop=0x8c603c0eu;*sr=(*sr&~1u)|(r[6]&1u);r[6]=(r[6]>>1)|(r[6]&0x80000000u);goto L8c603c10; /* shar r6 */
L8c603c10: *stop=0x8c603c12u;*sr=(*sr&~1u)|(r[6]&1u);r[6]=(r[6]>>1)|(r[6]&0x80000000u);goto L8c603c1a; /* bra 0x8c603c1a */
L8c603c14: *stop=0x8c603c14u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}r[4]+=4;goto L8c603c16; /* mov.l @r4+,r3 */
L8c603c16: *stop=0x8c603c16u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c603c18; /* mov.l r3,@r5 */
L8c603c18: *stop=0x8c603c18u;r[5]+=4u;goto L8c603c1a; /* add #0x4,r5 */
L8c603c1a: *stop=0x8c603c1au;*sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c603c1c; /* cmp/pl r6 */
L8c603c1c: take=(*sr&1u);*stop=0x8c603c1eu;r[6]+=4294967295u;if(take)goto L8c603c14;goto L8c603c20; /* bt/s 0x8c603c14 */
L8c603c20: *stop=0x8c603c20u;if(*pr!=entry_pr)return -3;*stop=0x8c603c22u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603ec4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c603ec4;
L8c603ec4: *stop=0x8c603ec4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c603ec6; /* mov.l r14,@-r15 */
L8c603ec6: *stop=0x8c603ec6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c603ec8; /* mov.l r13,@-r15 */
L8c603ec8: *stop=0x8c603ec8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c603eca; /* mov.l r12,@-r15 */
L8c603eca: *stop=0x8c603ecau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c603ecc; /* sts.l PR,@-r15 */
L8c603ecc: *stop=0x8c603eccu;r[15]+=4294967292u;goto L8c603ece; /* add #-0x4,r15 */
L8c603ece: *stop=0x8c603eceu;r[14]=r[4];goto L8c603ed0; /* mov r4,r14 */
L8c603ed0: *stop=0x8c603ed0u;if(!valid_access(r[14]+2u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c603ed2; /* mov.w @(0x2,r14),r0 */
L8c603ed2: *stop=0x8c603ed2u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c603ed4; /* tst r0,r0 */
L8c603ed4: if(!(*sr&1u))goto L8c603f7e;goto L8c603ed6; /* bf 0x8c603f7e */
L8c603ed6: *stop=0x8c603ed6u;r[13]=r[14];goto L8c603ed8; /* mov r14,r13 */
L8c603ed8: *stop=0x8c603ed8u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c603eda; /* mov.l @(0x28,r14),r2 */
L8c603eda: *stop=0x8c603edau;r[13]+=52u;goto L8c603edc; /* add #0x34,r13 */
L8c603edc: *stop=0x8c603edcu;r[5]=r[13];goto L8c603ede; /* mov r13,r5 */
L8c603ede: *stop=0x8c603edeu;r[5]+=2u;goto L8c603ee0; /* add #0x2,r5 */
L8c603ee0: *stop=0x8c603ee0u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[2],2);if(io_status)return io_status;goto L8c603ee2; /* mov.w r2,@r13 */
L8c603ee2: *stop=0x8c603ee2u;r[6]=60u;goto L8c603ee4; /* mov #0x3c,r6 */
L8c603ee4: target=0x8c603c24u;*pr=0x8c603ee8u;*stop=0x8c603ee6u;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603ee8; /* bsr 0x8c603c24 */
L8c603ee8: *stop=0x8c603ee8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604000u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c603eea; /* mov.l 0x8c604000,r2 */
L8c603eea: *stop=0x8c603eeau;r[5]=r[13];goto L8c603eec; /* mov r13,r5 */
L8c603eec: target=r[2];*pr=0x8c603ef0u;*stop=0x8c603eeeu;r[4]=62u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603ef0; /* jsr @r2 */
L8c603ef0: *stop=0x8c603ef0u;r[4]=r[0];goto L8c603ef2; /* mov r0,r4 */
L8c603ef2: *stop=0x8c603ef2u;r[0]=62u;goto L8c603ef4; /* mov #0x3e,r0 */
L8c603ef4: *stop=0x8c603ef4u;if(!valid_access(r[0]+r[13],2u))return -3;wr(m,bus,&io_status,r[0]+r[13],r[4],2);if(io_status)return io_status;goto L8c603ef6; /* mov.w r4,@(r0,r13) */
L8c603ef6: *stop=0x8c603ef6u;r[6]=64u;goto L8c603ef8; /* mov #0x40,r6 */
L8c603ef8: *stop=0x8c603ef8u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603efa; /* mov.l @(0x28,r14),r3 */
L8c603efa: *stop=0x8c603efau;r[3]+=1u;goto L8c603efc; /* add #0x1,r3 */
L8c603efc: *stop=0x8c603efcu;r[3]<<=2;goto L8c603efe; /* shll2 r3 */
L8c603efe: *stop=0x8c603efeu;r[3]<<=2;goto L8c603f00; /* shll2 r3 */
L8c603f00: *stop=0x8c603f00u;r[3]<<=2;goto L8c603f02; /* shll2 r3 */
L8c603f02: *stop=0x8c603f02u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c603f04; /* mov.l r3,@r15 */
L8c603f04: *stop=0x8c603f04u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c603f06; /* mov.l @(0xc,r14),r5 */
L8c603f06: *stop=0x8c603f06u;r[5]+=r[3];goto L8c603f08; /* add r3,r5 */
L8c603f08: target=0x8c603c08u;*pr=0x8c603f0cu;*stop=0x8c603f0au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603f0c; /* bsr 0x8c603c08 */
L8c603f0c: *stop=0x8c603f0cu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603f0e; /* mov.l @(0x10,r14),r3 */
L8c603f0e: *stop=0x8c603f0eu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c603f10; /* mov.l @(0x14,r14),r2 */
L8c603f10: *stop=0x8c603f10u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c603f12; /* cmp/ge r2,r3 */
L8c603f12: if(!(*sr&1u))goto L8c603f18;goto L8c603f14; /* bf 0x8c603f18 */
L8c603f14: *stop=0x8c603f16u;r[0]=4294967286u;goto L8c603f80; /* bra 0x8c603f80 */
L8c603f18: *stop=0x8c603f18u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c603f1a; /* mov.l @(0x14,r14),r12 */
L8c603f1a: *stop=0x8c603f1au;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c603f1c; /* mov.l @(0xc,r14),r5 */
L8c603f1c: *stop=0x8c603f1cu;r[12]+=1u;goto L8c603f1e; /* add #0x1,r12 */
L8c603f1e: *stop=0x8c603f1eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c603f20; /* mov.l @(0x10,r14),r4 */
L8c603f20: *stop=0x8c603f20u;r[12]<<=2;goto L8c603f22; /* shll2 r12 */
L8c603f22: *stop=0x8c603f22u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c603f24; /* mov.l @(0x18,r14),r6 */
L8c603f24: *stop=0x8c603f24u;r[12]<<=2;goto L8c603f26; /* shll2 r12 */
L8c603f26: *stop=0x8c603f26u;r[12]<<=2;goto L8c603f28; /* shll2 r12 */
L8c603f28: *stop=0x8c603f28u;r[5]+=r[12];goto L8c603f2a; /* add r12,r5 */
L8c603f2a: target=0x8c603c84u;*pr=0x8c603f2eu;*stop=0x8c603f2cu;r[4]+=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603f2e; /* bsr 0x8c603c84 */
L8c603f2e: *stop=0x8c603f2eu;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c603f30; /* mov.l @(0x18,r14),r6 */
L8c603f30: *stop=0x8c603f30u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c603f32; /* mov.l @(0xc,r14),r5 */
L8c603f32: *stop=0x8c603f32u;r[6]<<=2;goto L8c603f34; /* shll2 r6 */
L8c603f34: *stop=0x8c603f34u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c603f36; /* mov.l @(0x4,r14),r4 */
L8c603f36: *stop=0x8c603f36u;r[6]<<=2;goto L8c603f38; /* shll2 r6 */
L8c603f38: *stop=0x8c603f38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604004u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c603f3a; /* mov.l 0x8c604004,r2 */
L8c603f3a: *stop=0x8c603f3au;r[6]<<=2;goto L8c603f3c; /* shll2 r6 */
L8c603f3c: *stop=0x8c603f3cu;r[5]+=r[12];goto L8c603f3e; /* add r12,r5 */
L8c603f3e: target=r[2];*pr=0x8c603f42u;*stop=0x8c603f40u;r[4]+=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603f42; /* jsr @r2 */
L8c603f42: *stop=0x8c603f42u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603f44; /* mov.l @(0x18,r14),r3 */
L8c603f44: *stop=0x8c603f44u;r[4]=r[0];goto L8c603f46; /* mov r0,r4 */
L8c603f46: *stop=0x8c603f46u;r[3]<<=2;goto L8c603f48; /* shll2 r3 */
L8c603f48: *stop=0x8c603f48u;r[3]<<=2;goto L8c603f4a; /* shll2 r3 */
L8c603f4a: *stop=0x8c603f4au;r[3]<<=2;goto L8c603f4c; /* shll2 r3 */
L8c603f4c: *stop=0x8c603f4cu;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c603f4e; /* cmp/eq r3,r4 */
L8c603f4e: if(!(*sr&1u))goto L8c603f76;goto L8c603f50; /* bf 0x8c603f76 */
L8c603f50: *stop=0x8c603f50u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c603f52; /* mov.l @(0x10,r14),r2 */
L8c603f52: *stop=0x8c603f52u;r[6]=64u;goto L8c603f54; /* mov #0x40,r6 */
L8c603f54: *stop=0x8c603f54u;r[5]=r[13];goto L8c603f56; /* mov r13,r5 */
L8c603f56: *stop=0x8c603f56u;r[2]+=1u;goto L8c603f58; /* add #0x1,r2 */
L8c603f58: *stop=0x8c603f58u;r[2]<<=2;goto L8c603f5a; /* shll2 r2 */
L8c603f5a: *stop=0x8c603f5au;r[2]<<=2;goto L8c603f5c; /* shll2 r2 */
L8c603f5c: *stop=0x8c603f5cu;r[2]<<=2;goto L8c603f5e; /* shll2 r2 */
L8c603f5e: *stop=0x8c603f5eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c603f60; /* mov.l r2,@r15 */
L8c603f60: *stop=0x8c603f60u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603f62; /* mov.l @(0x10,r14),r3 */
L8c603f62: *stop=0x8c603f62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604004u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c603f64; /* mov.l 0x8c604004,r2 */
L8c603f64: *stop=0x8c603f64u;r[3]+=1u;goto L8c603f66; /* add #0x1,r3 */
L8c603f66: *stop=0x8c603f66u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c603f68; /* mov.l r3,@(0x10,r14) */
L8c603f68: *stop=0x8c603f68u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c603f6a; /* mov.l @r15,r3 */
L8c603f6a: *stop=0x8c603f6au;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c603f6c; /* mov.l @(0x4,r14),r4 */
L8c603f6c: target=r[2];*pr=0x8c603f70u;*stop=0x8c603f6eu;r[4]+=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603f70; /* jsr @r2 */
L8c603f70: *stop=0x8c603f70u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c603f72; /* cmp/eq #0x40,r0 */
L8c603f72: take=(*sr&1u);*stop=0x8c603f74u;r[4]=r[0];if(take)goto L8c603f7a;goto L8c603f76; /* bt/s 0x8c603f7a */
L8c603f76: *stop=0x8c603f78u;r[0]=4294967294u;goto L8c603f80; /* bra 0x8c603f80 */
L8c603f7a: *stop=0x8c603f7cu;r[0]=0u;goto L8c603f80; /* bra 0x8c603f80 */
L8c603f7e: *stop=0x8c603f7eu;r[0]=4294967290u;goto L8c603f80; /* mov #-0x6,r0 */
L8c603f80: *stop=0x8c603f80u;r[15]+=4u;goto L8c603f82; /* add #0x4,r15 */
L8c603f82: *stop=0x8c603f82u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c603f84; /* lds.l @r15+,PR */
L8c603f84: *stop=0x8c603f84u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c603f86; /* mov.l @r15+,r12 */
L8c603f86: *stop=0x8c603f86u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c603f88; /* mov.l @r15+,r13 */
L8c603f88: *stop=0x8c603f88u;if(*pr!=entry_pr)return -3;*stop=0x8c603f8au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603e08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c603e08;
L8c603e08: *stop=0x8c603e08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c603e0a; /* mov.l r14,@-r15 */
L8c603e0a: *stop=0x8c603e0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c603e0c; /* mov.l r13,@-r15 */
L8c603e0c: *stop=0x8c603e0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c603e0e; /* mov.l r12,@-r15 */
L8c603e0e: *stop=0x8c603e0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c603e10; /* mov.l r11,@-r15 */
L8c603e10: *stop=0x8c603e10u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c603e12; /* mov.l r10,@-r15 */
L8c603e12: *stop=0x8c603e12u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c603e14; /* mov.l r9,@-r15 */
L8c603e14: *stop=0x8c603e14u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c603e16; /* mov.l r8,@-r15 */
L8c603e16: *stop=0x8c603e16u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c603e18; /* sts.l PR,@-r15 */
L8c603e18: *stop=0x8c603e18u;r[15]+=4294967292u;goto L8c603e1a; /* add #-0x4,r15 */
L8c603e1a: *stop=0x8c603e1au;r[13]=r[4];goto L8c603e1c; /* mov r4,r13 */
L8c603e1c: *stop=0x8c603e1cu;if(!valid_access(r[13]+2u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c603e1e; /* mov.w @(0x2,r13),r0 */
L8c603e1e: *stop=0x8c603e1eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c603e20; /* tst r0,r0 */
L8c603e20: take=!(*sr&1u);*stop=0x8c603e22u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[8]=loaded;}if(take)goto L8c603eae;goto L8c603e24; /* bf/s 0x8c603eae */
L8c603e24: *stop=0x8c603e24u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c603e26; /* mov.l @(0xc,r13),r14 */
L8c603e26: *stop=0x8c603e26u;r[12]=r[8];goto L8c603e28; /* mov r8,r12 */
L8c603e28: *stop=0x8c603e28u;{uint32_t loaded=rd(m,bus,&io_status,0x8c604004u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c603e2a; /* mov.l 0x8c604004,r10 */
L8c603e2a: *stop=0x8c603e2au;r[6]=64u;goto L8c603e2c; /* mov #0x40,r6 */
L8c603e2c: *stop=0x8c603e2cu;r[5]=r[14];goto L8c603e2e; /* mov r14,r5 */
L8c603e2e: *stop=0x8c603e2eu;r[14]+=64u;goto L8c603e30; /* add #0x40,r14 */
L8c603e30: target=r[10];*pr=0x8c603e34u;*stop=0x8c603e32u;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603e34; /* jsr @r10 */
L8c603e34: *stop=0x8c603e34u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c603e36; /* cmp/eq #0x40,r0 */
L8c603e36: take=!(*sr&1u);*stop=0x8c603e38u;r[4]=r[0];if(take)goto L8c603ea4;goto L8c603e3a; /* bf/s 0x8c603ea4 */
L8c603e3a: *stop=0x8c603e3au;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c603e3c; /* mov.l @(0x14,r13),r9 */
L8c603e3c: *stop=0x8c603e3cu;r[12]+=64u;goto L8c603e3e; /* add #0x40,r12 */
L8c603e3e: *stop=0x8c603e3eu;*sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c603e40; /* cmp/pl r9 */
L8c603e40: take=!(*sr&1u);*stop=0x8c603e42u;r[11]=0u;if(take)goto L8c603e72;goto L8c603e44; /* bf/s 0x8c603e72 */
L8c603e44: *stop=0x8c603e44u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[2]=loaded;}goto L8c603e46; /* mov.w @r14,r2 */
L8c603e46: *stop=0x8c603e46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c603ffcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c603e48; /* mov.l 0x8c603ffc,r4 */
L8c603e48: *stop=0x8c603e48u;r[2]=r[2]&65535u;goto L8c603e4a; /* extu.w r2,r2 */
L8c603e4a: *stop=0x8c603e4au;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c603e4c; /* cmp/eq r4,r2 */
L8c603e4c: if((*sr&1u))goto L8c603e6a;goto L8c603e4e; /* bt 0x8c603e6a */
L8c603e4e: *stop=0x8c603e4eu;r[5]=r[14];goto L8c603e50; /* mov r14,r5 */
L8c603e50: target=0x8c603cd2u;*pr=0x8c603e54u;*stop=0x8c603e52u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603e54; /* bsr 0x8c603cd2 */
L8c603e54: *stop=0x8c603e54u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c603e56; /* tst r0,r0 */
L8c603e56: if(!(*sr&1u))goto L8c603e6a;goto L8c603e58; /* bf 0x8c603e6a */
L8c603e58: *stop=0x8c603e58u;r[5]=r[14];goto L8c603e5a; /* mov r14,r5 */
L8c603e5a: *stop=0x8c603e5au;r[6]=64u;goto L8c603e5c; /* mov #0x40,r6 */
L8c603e5c: target=r[10];*pr=0x8c603e60u;*stop=0x8c603e5eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603e60; /* jsr @r10 */
L8c603e60: *stop=0x8c603e60u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c603e62; /* cmp/eq #0x40,r0 */
L8c603e62: take=!(*sr&1u);*stop=0x8c603e64u;r[4]=r[0];if(take)goto L8c603ea4;goto L8c603e66; /* bf/s 0x8c603ea4 */
L8c603e66: *stop=0x8c603e66u;r[11]+=1u;goto L8c603e68; /* add #0x1,r11 */
L8c603e68: *stop=0x8c603e68u;r[12]+=64u;goto L8c603e6a; /* add #0x40,r12 */
L8c603e6a: *stop=0x8c603e6au;r[9]+=4294967295u;goto L8c603e6c; /* add #-0x1,r9 */
L8c603e6c: *stop=0x8c603e6cu;*sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c603e6e; /* cmp/pl r9 */
L8c603e6e: take=(*sr&1u);*stop=0x8c603e70u;r[14]+=64u;if(take)goto L8c603e44;goto L8c603e72; /* bt/s 0x8c603e44 */
L8c603e72: *stop=0x8c603e72u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603e74; /* mov.l @(0x14,r13),r3 */
L8c603e74: *stop=0x8c603e74u;r[5]=r[14];goto L8c603e76; /* mov r14,r5 */
L8c603e76: *stop=0x8c603e76u;r[3]+=1u;goto L8c603e78; /* add #0x1,r3 */
L8c603e78: *stop=0x8c603e78u;r[3]<<=2;goto L8c603e7a; /* shll2 r3 */
L8c603e7a: *stop=0x8c603e7au;r[3]<<=2;goto L8c603e7c; /* shll2 r3 */
L8c603e7c: *stop=0x8c603e7cu;r[3]<<=2;goto L8c603e7e; /* shll2 r3 */
L8c603e7e: *stop=0x8c603e7eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c603e80; /* mov.l r3,@r15 */
L8c603e80: *stop=0x8c603e80u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c603e82; /* mov.l @(0x18,r13),r6 */
L8c603e82: target=0x8c603c84u;*pr=0x8c603e86u;*stop=0x8c603e84u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603e86; /* bsr 0x8c603c84 */
L8c603e86: *stop=0x8c603e86u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c603e88; /* mov.l @(0x18,r13),r6 */
L8c603e88: *stop=0x8c603e88u;r[5]=r[14];goto L8c603e8a; /* mov r14,r5 */
L8c603e8a: *stop=0x8c603e8au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c603e8c; /* mov.l @r15,r4 */
L8c603e8c: *stop=0x8c603e8cu;r[6]<<=2;goto L8c603e8e; /* shll2 r6 */
L8c603e8e: *stop=0x8c603e8eu;r[6]<<=2;goto L8c603e90; /* shll2 r6 */
L8c603e90: *stop=0x8c603e90u;r[6]<<=2;goto L8c603e92; /* shll2 r6 */
L8c603e92: target=r[10];*pr=0x8c603e96u;*stop=0x8c603e94u;r[4]+=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603e96; /* jsr @r10 */
L8c603e96: *stop=0x8c603e96u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603e98; /* mov.l @(0x18,r13),r3 */
L8c603e98: *stop=0x8c603e98u;r[4]=r[0];goto L8c603e9a; /* mov r0,r4 */
L8c603e9a: *stop=0x8c603e9au;r[3]<<=2;goto L8c603e9c; /* shll2 r3 */
L8c603e9c: *stop=0x8c603e9cu;r[3]<<=2;goto L8c603e9e; /* shll2 r3 */
L8c603e9e: *stop=0x8c603e9eu;r[3]<<=2;goto L8c603ea0; /* shll2 r3 */
L8c603ea0: *stop=0x8c603ea0u;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c603ea2; /* cmp/eq r3,r4 */
L8c603ea2: if((*sr&1u))goto L8c603ea8;goto L8c603ea4; /* bt 0x8c603ea8 */
L8c603ea4: *stop=0x8c603ea6u;r[0]=4294967294u;goto L8c603eb0; /* bra 0x8c603eb0 */
L8c603ea8: *stop=0x8c603ea8u;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[11],4);if(io_status)return io_status;goto L8c603eaa; /* mov.l r11,@(0x10,r13) */
L8c603eaa: *stop=0x8c603eacu;r[0]=0u;goto L8c603eb0; /* bra 0x8c603eb0 */
L8c603eae: *stop=0x8c603eaeu;r[0]=4294967290u;goto L8c603eb0; /* mov #-0x6,r0 */
L8c603eb0: *stop=0x8c603eb0u;r[15]+=4u;goto L8c603eb2; /* add #0x4,r15 */
L8c603eb2: *stop=0x8c603eb2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c603eb4; /* lds.l @r15+,PR */
L8c603eb4: *stop=0x8c603eb4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c603eb6; /* mov.l @r15+,r8 */
L8c603eb6: *stop=0x8c603eb6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c603eb8; /* mov.l @r15+,r9 */
L8c603eb8: *stop=0x8c603eb8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c603eba; /* mov.l @r15+,r10 */
L8c603eba: *stop=0x8c603ebau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c603ebc; /* mov.l @r15+,r11 */
L8c603ebc: *stop=0x8c603ebcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c603ebe; /* mov.l @r15+,r12 */
L8c603ebe: *stop=0x8c603ebeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c603ec0; /* mov.l @r15+,r13 */
L8c603ec0: *stop=0x8c603ec0u;if(*pr!=entry_pr)return -3;*stop=0x8c603ec2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c64bda0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64bda0;
L8c64bda0: *stop=0x8c64bda0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64bda2; /* mov.l r14,@-r15 */
L8c64bda2: *stop=0x8c64bda2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bdd8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64bda4; /* mov.l 0x8c64bdd8,r14 */
L8c64bda4: *stop=0x8c64bda4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bdd4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64bda6; /* mov.l 0x8c64bdd4,r6 */
L8c64bda6: *stop=0x8c64bda8u;goto L8c64bdc4; /* bra 0x8c64bdc4 */
L8c64bdaa: *stop=0x8c64bdaau;r[7]=r[6]&65535u;goto L8c64bdac; /* extu.w r6,r7 */
L8c64bdac: *stop=0x8c64bdacu;r[0]=r[7];goto L8c64bdae; /* mov r7,r0 */
L8c64bdae: *stop=0x8c64bdaeu;r[3]=4294967288u;goto L8c64bdb0; /* mov #-0x8,r3 */
L8c64bdb0: *stop=0x8c64bdb0u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c64bdb2; /* shad r3,r0 */
L8c64bdb2: *stop=0x8c64bdb2u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[3]=loaded;}r[5]+=1;goto L8c64bdb4; /* mov.b @r5+,r3 */
L8c64bdb4: *stop=0x8c64bdb4u;r[2]=r[7];goto L8c64bdb6; /* mov r7,r2 */
L8c64bdb6: *stop=0x8c64bdb6u;r[2]<<=8;goto L8c64bdb8; /* shll8 r2 */
L8c64bdb8: *stop=0x8c64bdb8u;r[3]=r[3]&255u;goto L8c64bdba; /* extu.b r3,r3 */
L8c64bdba: *stop=0x8c64bdbau;r[6]=r[2];goto L8c64bdbc; /* mov r2,r6 */
L8c64bdbc: *stop=0x8c64bdbcu;r[0]^=r[3];goto L8c64bdbe; /* xor r3,r0 */
L8c64bdbe: *stop=0x8c64bdbeu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c64bdc0; /* shll r0 */
L8c64bdc0: *stop=0x8c64bdc0u;if(!valid_access(r[0]+r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;r[1]=loaded;}goto L8c64bdc2; /* mov.w @(r0,r14),r1 */
L8c64bdc2: *stop=0x8c64bdc2u;r[6]^=r[1];goto L8c64bdc4; /* xor r1,r6 */
L8c64bdc4: *stop=0x8c64bdc4u;r[4]+=4294967295u;goto L8c64bdc6; /* add #-0x1,r4 */
L8c64bdc6: *stop=0x8c64bdc6u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64bdc8; /* cmp/pz r4 */
L8c64bdc8: if((*sr&1u))goto L8c64bdaa;goto L8c64bdca; /* bt 0x8c64bdaa */
L8c64bdca: *stop=0x8c64bdcau;r[0]=r[6]&65535u;goto L8c64bdcc; /* extu.w r6,r0 */
L8c64bdcc: *stop=0x8c64bdccu;r[0]=~r[0];goto L8c64bdce; /* not r0,r0 */
L8c64bdce: *stop=0x8c64bdceu;r[0]=r[0]&65535u;goto L8c64bdd0; /* extu.w r0,r0 */
L8c64bdd0: *stop=0x8c64bdd0u;if(*pr!=entry_pr)return -3;*stop=0x8c64bdd2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603c84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c603c84;
L8c603c84: *stop=0x8c603c84u;r[1]=r[6];goto L8c603c86; /* mov r6,r1 */
L8c603c86: *stop=0x8c603c86u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c603c88; /* mov.l r12,@-r15 */
L8c603c88: *stop=0x8c603c88u;r[1]<<=2;goto L8c603c8a; /* shll2 r1 */
L8c603c8a: *stop=0x8c603c8au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c603c8c; /* mov.l r11,@-r15 */
L8c603c8c: *stop=0x8c603c8cu;r[1]<<=2;goto L8c603c8e; /* shll2 r1 */
L8c603c8e: *stop=0x8c603c8eu;r[11]=0u;goto L8c603c90; /* mov #0x0,r11 */
L8c603c90: *stop=0x8c603c90u;r[12]=4294967295u;goto L8c603c92; /* mov #-0x1,r12 */
L8c603c92: *stop=0x8c603c92u;r[7]=8u;goto L8c603c94; /* mov #0x8,r7 */
L8c603c94: *stop=0x8c603c96u;r[1]<<=2;goto L8c603cc6; /* bra 0x8c603cc6 */
L8c603c98: *stop=0x8c603c98u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[7]);goto L8c603c9a; /* cmp/ge r7,r4 */
L8c603c9a: if(!(*sr&1u))goto L8c603ca4;goto L8c603c9c; /* bf 0x8c603ca4 */
L8c603c9c: *stop=0x8c603c9cu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[11],1);if(io_status)return io_status;goto L8c603c9e; /* mov.b r11,@r5 */
L8c603c9e: *stop=0x8c603c9eu;r[5]+=1u;goto L8c603ca0; /* add #0x1,r5 */
L8c603ca0: *stop=0x8c603ca2u;r[4]+=4294967288u;goto L8c603cc4; /* bra 0x8c603cc4 */
L8c603ca4: *stop=0x8c603ca4u;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c603ca6; /* cmp/pl r4 */
L8c603ca6: if(!(*sr&1u))goto L8c603cc0;goto L8c603ca8; /* bf 0x8c603cc0 */
L8c603ca8: *stop=0x8c603ca8u;r[0]=r[7];goto L8c603caa; /* mov r7,r0 */
L8c603caa: *stop=0x8c603caau;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c603cac; /* shll r6 */
L8c603cac: *stop=0x8c603cacu;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c603cae; /* cmp/pl r4 */
L8c603cae: if(!(*sr&1u))goto L8c603cb4;goto L8c603cb0; /* bf 0x8c603cb4 */
L8c603cb0: *stop=0x8c603cb2u;r[4]+=4294967295u;goto L8c603cb8; /* bra 0x8c603cb8 */
L8c603cb4: *stop=0x8c603cb4u;r[2]=1u;goto L8c603cb6; /* mov #0x1,r2 */
L8c603cb6: *stop=0x8c603cb6u;r[6]|=r[2];goto L8c603cb8; /* or r2,r6 */
L8c603cb8: *stop=0x8c603cb8u;r[0]--;*sr=(*sr&~1u)|(!r[0]);goto L8c603cba; /* dt r0 */
L8c603cba: if(!(*sr&1u))goto L8c603caa;goto L8c603cbc; /* bf 0x8c603caa */
L8c603cbc: *stop=0x8c603cbeu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[6],1);if(io_status)return io_status;goto L8c603cc2; /* bra 0x8c603cc2 */
L8c603cc0: *stop=0x8c603cc0u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[12],1);if(io_status)return io_status;goto L8c603cc2; /* mov.b r12,@r5 */
L8c603cc2: *stop=0x8c603cc2u;r[5]+=1u;goto L8c603cc4; /* add #0x1,r5 */
L8c603cc4: *stop=0x8c603cc4u;r[1]+=4294967295u;goto L8c603cc6; /* add #-0x1,r1 */
L8c603cc6: *stop=0x8c603cc6u;*sr=(*sr&~1u)|((int32_t)r[1]>0);goto L8c603cc8; /* cmp/pl r1 */
L8c603cc8: if((*sr&1u))goto L8c603c98;goto L8c603cca; /* bt 0x8c603c98 */
L8c603cca: *stop=0x8c603ccau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c603ccc; /* mov.l @r15+,r11 */
L8c603ccc: *stop=0x8c603cccu;r[0]=0u;goto L8c603cce; /* mov #0x0,r0 */
L8c603cce: *stop=0x8c603cceu;if(*pr!=entry_pr)return -3;*stop=0x8c603cd0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_packet_startup_8c603cd2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c603cd2;
L8c603cd2: *stop=0x8c603cd2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c603cd4; /* mov.l r14,@-r15 */
L8c603cd4: *stop=0x8c603cd4u;r[4]=60u;goto L8c603cd6; /* mov #0x3c,r4 */
L8c603cd6: *stop=0x8c603cd6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c603cd8; /* mov.l r13,@-r15 */
L8c603cd8: *stop=0x8c603cd8u;r[13]=r[5];goto L8c603cda; /* mov r5,r13 */
L8c603cda: *stop=0x8c603cdau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c603cdc; /* sts.l PR,@-r15 */
L8c603cdc: *stop=0x8c603cdcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c603d9cu,2);if(io_status)return io_status;r[14]=loaded;}goto L8c603cde; /* mov.w 0x8c603d9c,r14 */
L8c603cde: *stop=0x8c603cdeu;r[5]+=2u;goto L8c603ce0; /* add #0x2,r5 */
L8c603ce0: *stop=0x8c603ce0u;if(!valid_access(r[5],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[3]=loaded;}r[5]+=1;goto L8c603ce2; /* mov.b @r5+,r3 */
L8c603ce2: *stop=0x8c603ce2u;r[3]=r[3]&255u;goto L8c603ce4; /* extu.b r3,r3 */
L8c603ce4: *stop=0x8c603ce4u;*sr=(*sr&~1u)|(r[14]==r[3]);goto L8c603ce6; /* cmp/eq r14,r3 */
L8c603ce6: if(!(*sr&1u))goto L8c603d04;goto L8c603ce8; /* bf 0x8c603d04 */
L8c603ce8: *stop=0x8c603ce8u;r[4]+=4294967295u;goto L8c603cea; /* add #-0x1,r4 */
L8c603cea: *stop=0x8c603ceau;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c603cec; /* cmp/pl r4 */
L8c603cec: if((*sr&1u))goto L8c603ce0;goto L8c603cee; /* bt 0x8c603ce0 */
L8c603cee: *stop=0x8c603ceeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c603da0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c603cf0; /* mov.l 0x8c603da0,r3 */
L8c603cf0: *stop=0x8c603cf0u;r[5]=r[13];goto L8c603cf2; /* mov r13,r5 */
L8c603cf2: target=r[3];*pr=0x8c603cf6u;*stop=0x8c603cf4u;r[4]=62u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c603cf6; /* jsr @r3 */
L8c603cf6: *stop=0x8c603cf6u;r[4]=r[0];goto L8c603cf8; /* mov r0,r4 */
L8c603cf8: *stop=0x8c603cf8u;r[0]=62u;goto L8c603cfa; /* mov #0x3e,r0 */
L8c603cfa: *stop=0x8c603cfau;if(!valid_access(r[0]+r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[0]+r[13],2);if(io_status)return io_status;r[2]=loaded;}goto L8c603cfc; /* mov.w @(r0,r13),r2 */
L8c603cfc: *stop=0x8c603cfcu;r[4]=r[4]&65535u;goto L8c603cfe; /* extu.w r4,r4 */
L8c603cfe: *stop=0x8c603cfeu;r[2]=r[2]&65535u;goto L8c603d00; /* extu.w r2,r2 */
L8c603d00: *stop=0x8c603d00u;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c603d02; /* cmp/eq r4,r2 */
L8c603d02: if((*sr&1u))goto L8c603d08;goto L8c603d04; /* bt 0x8c603d08 */
L8c603d04: *stop=0x8c603d06u;r[0]=0u;goto L8c603d1a; /* bra 0x8c603d1a */
L8c603d08: *stop=0x8c603d08u;r[5]=r[13];goto L8c603d0a; /* mov r13,r5 */
L8c603d0a: *stop=0x8c603d0cu;r[4]=64u;goto L8c603d14; /* bra 0x8c603d14 */
L8c603d0e: *stop=0x8c603d0eu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[14],1);if(io_status)return io_status;goto L8c603d10; /* mov.b r14,@r5 */
L8c603d10: *stop=0x8c603d10u;r[4]+=4294967295u;goto L8c603d12; /* add #-0x1,r4 */
L8c603d12: *stop=0x8c603d12u;r[5]+=1u;goto L8c603d14; /* add #0x1,r5 */
L8c603d14: *stop=0x8c603d14u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c603d16; /* tst r4,r4 */
L8c603d16: if(!(*sr&1u))goto L8c603d0e;goto L8c603d18; /* bf 0x8c603d0e */
L8c603d18: *stop=0x8c603d18u;r[0]=1u;goto L8c603d1a; /* mov #0x1,r0 */
L8c603d1a: *stop=0x8c603d1au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c603d1c; /* lds.l @r15+,PR */
L8c603d1c: *stop=0x8c603d1cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c603d1e; /* mov.l @r15+,r13 */
L8c603d1e: *stop=0x8c603d1eu;if(*pr!=entry_pr)return -3;*stop=0x8c603d20u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
