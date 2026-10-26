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
 if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_midi_packet_8c643c18(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c643c18;
L8c643c18: *stop=0x8c643c18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c643c1a; /* mov.l r14,@-r15 */
L8c643c1a: *stop=0x8c643c1au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c643c1c; /* mov.l r13,@-r15 */
L8c643c1c: *stop=0x8c643c1cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c643c1e; /* mov.l r12,@-r15 */
L8c643c1e: *stop=0x8c643c1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c643c20; /* mov.l r11,@-r15 */
L8c643c20: *stop=0x8c643c20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c643c22; /* sts.l PR,@-r15 */
L8c643c22: *stop=0x8c643c22u;r[15]+=4294967284u;goto L8c643c24; /* add #-0xc,r15 */
L8c643c24: *stop=0x8c643c24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643d18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643c26; /* mov.l 0x8c643d18,r2 */
L8c643c26: *stop=0x8c643c26u;r[14]=r[5];goto L8c643c28; /* mov r5,r14 */
L8c643c28: *stop=0x8c643c28u;r[5]=0u;goto L8c643c2a; /* mov #0x0,r5 */
L8c643c2a: *stop=0x8c643c2au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c643c2c; /* mov.l @r2,r3 */
L8c643c2c: *stop=0x8c643c2cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c643c2e; /* tst r3,r3 */
L8c643c2e: take=!(*sr&1u);*stop=0x8c643c30u;r[13]=r[5];if(take)goto L8c643c38;goto L8c643c32; /* bf/s 0x8c643c38 */
L8c643c32: *stop=0x8c643c32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643d1cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c643c34; /* mov.l 0x8c643d1c,r13 */
L8c643c34: *stop=0x8c643c36u;goto L8c643d5c; /* bra 0x8c643d5c */
L8c643c38: *stop=0x8c643c38u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c643c3a; /* tst r4,r4 */
L8c643c3a: if(!(*sr&1u))goto L8c643c42;goto L8c643c3c; /* bf 0x8c643c42 */
L8c643c3c: *stop=0x8c643c3cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643d20u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c643c3e; /* mov.l 0x8c643d20,r13 */
L8c643c3e: *stop=0x8c643c40u;goto L8c643d5c; /* bra 0x8c643d5c */
L8c643c42: *stop=0x8c643c42u;if(!valid_access(r[4]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c643c44; /* mov.b @(0x1,r4),r0 */
L8c643c44: *stop=0x8c643c44u;{uint32_t loaded=sx(m,bus,&io_status,0x8c643d10u,2);if(io_status)return io_status;r[12]=loaded;}goto L8c643c46; /* mov.w 0x8c643d10,r12 */
L8c643c46: *stop=0x8c643c46u;if(!valid_access(r[15]+8u,1u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],1);if(io_status)return io_status;goto L8c643c48; /* mov.b r0,@(0x8,r15) */
L8c643c48: *stop=0x8c643c48u;r[0]=32u;goto L8c643c4a; /* mov #0x20,r0 */
L8c643c4a: *stop=0x8c643c4au;if(!valid_access(r[0]+r[15],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+r[0],1);if(io_status)return io_status;r[4]=loaded;}goto L8c643c4c; /* mov.b @(r0,r15),r4 */
L8c643c4c: *stop=0x8c643c4cu;r[4]=r[4]&255u;goto L8c643c4e; /* extu.b r4,r4 */
L8c643c4e: *stop=0x8c643c4eu;r[3]=r[4];goto L8c643c50; /* mov r4,r3 */
L8c643c50: *stop=0x8c643c50u;r[3]&=r[12];goto L8c643c52; /* and r12,r3 */
L8c643c52: *stop=0x8c643c52u;*sr=(*sr&~1u)|(r[12]==r[3]);goto L8c643c54; /* cmp/eq r12,r3 */
L8c643c54: if((*sr&1u))goto L8c643d2c;goto L8c643c56; /* bt 0x8c643d2c */
L8c643c56: *stop=0x8c643c56u;r[11]=r[15];goto L8c643c58; /* mov r15,r11 */
L8c643c58: *stop=0x8c643c58u;r[11]+=32u;goto L8c643c5a; /* add #0x20,r11 */
L8c643c5a: *stop=0x8c643c5au;r[11]+=1u;goto L8c643c5c; /* add #0x1,r11 */
L8c643c5c: *stop=0x8c643c5cu;r[6]=3u;goto L8c643c5e; /* mov #0x3,r6 */
L8c643c5e: *stop=0x8c643c5eu;*sr=(*sr&~1u)|!(r[6]&r[11]);goto L8c643c60; /* tst r6,r11 */
L8c643c60: if((*sr&1u))goto L8c643c6a;goto L8c643c62; /* bt 0x8c643c6a */
L8c643c62: *stop=0x8c643c62u;r[11]=r[15];goto L8c643c64; /* mov r15,r11 */
L8c643c64: *stop=0x8c643c64u;r[11]+=32u;goto L8c643c66; /* add #0x20,r11 */
L8c643c66: *stop=0x8c643c68u;r[11]+=4u;goto L8c643c70; /* bra 0x8c643c70 */
L8c643c6a: *stop=0x8c643c6au;r[11]=r[15];goto L8c643c6c; /* mov r15,r11 */
L8c643c6c: *stop=0x8c643c6cu;r[11]+=32u;goto L8c643c6e; /* add #0x20,r11 */
L8c643c6e: *stop=0x8c643c6eu;r[11]+=1u;goto L8c643c70; /* add #0x1,r11 */
L8c643c70: *stop=0x8c643c70u;r[7]=r[11];goto L8c643c72; /* mov r11,r7 */
L8c643c72: *stop=0x8c643c72u;r[7]+=4u;goto L8c643c74; /* add #0x4,r7 */
L8c643c74: *stop=0x8c643c74u;*sr=(*sr&~1u)|!(r[6]&r[7]);goto L8c643c76; /* tst r6,r7 */
L8c643c76: take=(*sr&1u);*stop=0x8c643c78u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;if(take)goto L8c643c80;goto L8c643c7a; /* bt/s 0x8c643c80 */
L8c643c7a: *stop=0x8c643c7au;r[7]=r[11];goto L8c643c7c; /* mov r11,r7 */
L8c643c7c: *stop=0x8c643c7eu;r[7]+=8u;goto L8c643c82; /* bra 0x8c643c82 */
L8c643c80: *stop=0x8c643c80u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[7]=loaded;}goto L8c643c82; /* mov.l @r15,r7 */
L8c643c82: *stop=0x8c643c82u;r[3]=r[7];goto L8c643c84; /* mov r7,r3 */
L8c643c84: *stop=0x8c643c84u;r[3]+=4294967292u;goto L8c643c86; /* add #-0x4,r3 */
L8c643c86: *stop=0x8c643c86u;*sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c643c88; /* tst r6,r3 */
L8c643c88: take=(*sr&1u);*stop=0x8c643c8au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;if(take)goto L8c643c92;goto L8c643c8c; /* bt/s 0x8c643c92 */
L8c643c8c: *stop=0x8c643c8cu;r[2]=r[7];goto L8c643c8e; /* mov r7,r2 */
L8c643c8e: *stop=0x8c643c90u;r[2]+=4294967288u;goto L8c643c94; /* bra 0x8c643c94 */
L8c643c92: *stop=0x8c643c92u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643c94; /* mov.l @(0x4,r15),r2 */
L8c643c94: *stop=0x8c643c94u;{uint32_t loaded=sx(m,bus,&io_status,0x8c643d12u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c643c96; /* mov.w 0x8c643d12,r1 */
L8c643c96: *stop=0x8c643c96u;r[0]=r[4];goto L8c643c98; /* mov r4,r0 */
L8c643c98: *stop=0x8c643c98u;r[0]&=r[12];goto L8c643c9a; /* and r12,r0 */
L8c643c9a: *stop=0x8c643c9au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c643c9c; /* cmp/eq r1,r0 */
L8c643c9c: take=(*sr&1u);*stop=0x8c643c9eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[11]=loaded;}if(take)goto L8c643caa;goto L8c643ca0; /* bt/s 0x8c643caa */
L8c643ca0: *stop=0x8c643ca0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c643d14u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c643ca2; /* mov.w 0x8c643d14,r1 */
L8c643ca2: *stop=0x8c643ca2u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c643ca4; /* cmp/eq r1,r0 */
L8c643ca4: if((*sr&1u))goto L8c643caa;goto L8c643ca6; /* bt 0x8c643caa */
L8c643ca6: *stop=0x8c643ca8u;goto L8c643cae; /* bra 0x8c643cae */
L8c643caa: *stop=0x8c643cacu;r[12]=r[5];goto L8c643cd4; /* bra 0x8c643cd4 */
L8c643cae: *stop=0x8c643caeu;r[5]=r[7];goto L8c643cb0; /* mov r7,r5 */
L8c643cb0: *stop=0x8c643cb0u;r[5]+=4u;goto L8c643cb2; /* add #0x4,r5 */
L8c643cb2: *stop=0x8c643cb2u;*sr=(*sr&~1u)|!(r[6]&r[5]);goto L8c643cb4; /* tst r6,r5 */
L8c643cb4: take=(*sr&1u);*stop=0x8c643cb6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;if(take)goto L8c643cbe;goto L8c643cb8; /* bt/s 0x8c643cbe */
L8c643cb8: *stop=0x8c643cb8u;r[5]=r[7];goto L8c643cba; /* mov r7,r5 */
L8c643cba: *stop=0x8c643cbcu;r[5]+=8u;goto L8c643cc0; /* bra 0x8c643cc0 */
L8c643cbe: *stop=0x8c643cbeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c643cc0; /* mov.l @r15,r5 */
L8c643cc0: *stop=0x8c643cc0u;r[12]=r[5];goto L8c643cc2; /* mov r5,r12 */
L8c643cc2: *stop=0x8c643cc2u;r[12]+=4294967292u;goto L8c643cc4; /* add #-0x4,r12 */
L8c643cc4: *stop=0x8c643cc4u;*sr=(*sr&~1u)|!(r[6]&r[12]);goto L8c643cc6; /* tst r6,r12 */
L8c643cc6: take=(*sr&1u);*stop=0x8c643cc8u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[12],4);if(io_status)return io_status;if(take)goto L8c643cd0;goto L8c643cca; /* bt/s 0x8c643cd0 */
L8c643cca: *stop=0x8c643ccau;r[12]=r[5];goto L8c643ccc; /* mov r5,r12 */
L8c643ccc: *stop=0x8c643cceu;r[12]+=4294967288u;goto L8c643cd2; /* bra 0x8c643cd2 */
L8c643cd0: *stop=0x8c643cd0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c643cd2; /* mov.l @(0x4,r15),r12 */
L8c643cd2: *stop=0x8c643cd2u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[12]=loaded;}goto L8c643cd4; /* mov.l @r12,r12 */
L8c643cd4: *stop=0x8c643cd4u;r[2]=(uint32_t)(int32_t)(int8_t)r[12];goto L8c643cd6; /* exts.b r12,r2 */
L8c643cd6: *stop=0x8c643cd6u;r[3]=(uint32_t)(int32_t)(int8_t)r[11];goto L8c643cd8; /* exts.b r11,r3 */
L8c643cd8: *stop=0x8c643cd8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c643cda; /* mov.l r2,@-r15 */
L8c643cda: *stop=0x8c643cdau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c643cdc; /* mov.l r3,@-r15 */
L8c643cdc: *stop=0x8c643cdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643d28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643cde; /* mov.l 0x8c643d28,r3 */
L8c643cde: *stop=0x8c643cdeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643d24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643ce0; /* mov.l 0x8c643d24,r2 */
L8c643ce0: *stop=0x8c643ce0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c643ce2; /* mov.l r4,@-r15 */
L8c643ce2: target=r[3];*pr=0x8c643ce6u;*stop=0x8c643ce4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643ce6; /* jsr @r3 */
L8c643ce6: *stop=0x8c643ce6u;r[15]+=16u;goto L8c643ce8; /* add #0x10,r15 */
L8c643ce8: *stop=0x8c643ce8u;r[0]=r[11];goto L8c643cea; /* mov r11,r0 */
L8c643cea: *stop=0x8c643ceau;r[1]=r[15];goto L8c643cec; /* mov r15,r1 */
L8c643cec: *stop=0x8c643cecu;r[1]+=32u;goto L8c643cee; /* add #0x20,r1 */
L8c643cee: *stop=0x8c643ceeu;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[12],1);if(io_status)return io_status;goto L8c643cf0; /* mov.b r12,@r14 */
L8c643cf0: *stop=0x8c643cf0u;if(!valid_access(r[14]+1u,1u))return -3;wr(m,bus,&io_status,r[14]+0x1u,r[0],1);if(io_status)return io_status;goto L8c643cf2; /* mov.b r0,@(0x1,r14) */
L8c643cf2: *stop=0x8c643cf2u;if(!valid_access(r[15]+8u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x8u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c643cf4; /* mov.b @(0x8,r15),r0 */
L8c643cf4: *stop=0x8c643cf4u;r[0]<<=2;goto L8c643cf6; /* shll2 r0 */
L8c643cf6: *stop=0x8c643cf6u;r[0]<<=2;goto L8c643cf8; /* shll2 r0 */
L8c643cf8: *stop=0x8c643cf8u;r[3]=r[0];goto L8c643cfa; /* mov r0,r3 */
L8c643cfa: *stop=0x8c643cfau;if(!valid_access(r[1],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;r[0]=loaded;}goto L8c643cfc; /* mov.b @r1,r0 */
L8c643cfc: *stop=0x8c643cfcu;r[0]&=0xfu;goto L8c643cfe; /* and #0xf,r0 */
L8c643cfe: *stop=0x8c643cfeu;r[0]|=r[3];goto L8c643d00; /* or r3,r0 */
L8c643d00: *stop=0x8c643d00u;if(!valid_access(r[14]+2u,1u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],1);if(io_status)return io_status;goto L8c643d02; /* mov.b r0,@(0x2,r14) */
L8c643d02: *stop=0x8c643d02u;r[0]=32u;goto L8c643d04; /* mov #0x20,r0 */
L8c643d04: *stop=0x8c643d04u;if(!valid_access(r[0]+r[15],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+r[0],1);if(io_status)return io_status;r[0]=loaded;}goto L8c643d06; /* mov.b @(r0,r15),r0 */
L8c643d06: *stop=0x8c643d06u;r[0]=r[0]&255u;goto L8c643d08; /* extu.b r0,r0 */
L8c643d08: *stop=0x8c643d08u;r[0]>>=2;goto L8c643d0a; /* shlr2 r0 */
L8c643d0a: *stop=0x8c643d0au;r[0]>>=2;goto L8c643d0c; /* shlr2 r0 */
L8c643d0c: *stop=0x8c643d0eu;r[0]&=0x7u;goto L8c643d34; /* bra 0x8c643d34 */
L8c643d2c: *stop=0x8c643d2cu;r[0]=r[5];goto L8c643d2e; /* mov r5,r0 */
L8c643d2e: *stop=0x8c643d2eu;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[5],1);if(io_status)return io_status;goto L8c643d30; /* mov.b r5,@r14 */
L8c643d30: *stop=0x8c643d30u;if(!valid_access(r[14]+1u,1u))return -3;wr(m,bus,&io_status,r[14]+0x1u,r[0],1);if(io_status)return io_status;goto L8c643d32; /* mov.b r0,@(0x1,r14) */
L8c643d32: *stop=0x8c643d32u;if(!valid_access(r[14]+2u,1u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],1);if(io_status)return io_status;goto L8c643d34; /* mov.b r0,@(0x2,r14) */
L8c643d34: *stop=0x8c643d34u;r[12]=r[14];goto L8c643d36; /* mov r14,r12 */
L8c643d36: *stop=0x8c643d36u;if(!valid_access(r[14]+3u,1u))return -3;wr(m,bus,&io_status,r[14]+0x3u,r[0],1);if(io_status)return io_status;goto L8c643d38; /* mov.b r0,@(0x3,r14) */
L8c643d38: *stop=0x8c643d38u;if(!valid_access(r[12]+3u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12]+0x3u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c643d3a; /* mov.b @(0x3,r12),r0 */
L8c643d3a: *stop=0x8c643d3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c643d78u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643d3c; /* mov.l 0x8c643d78,r2 */
L8c643d3c: *stop=0x8c643d3cu;r[0]=r[0]&255u;goto L8c643d3e; /* extu.b r0,r0 */
L8c643d3e: *stop=0x8c643d3eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c643d40; /* mov.l r0,@-r15 */
L8c643d40: *stop=0x8c643d40u;if(!valid_access(r[12]+2u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12]+0x2u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c643d42; /* mov.b @(0x2,r12),r0 */
L8c643d42: *stop=0x8c643d42u;r[0]=r[0]&255u;goto L8c643d44; /* extu.b r0,r0 */
L8c643d44: *stop=0x8c643d44u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c643d46; /* mov.l r0,@-r15 */
L8c643d46: *stop=0x8c643d46u;if(!valid_access(r[12]+1u,1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12]+0x1u,1);if(io_status)return io_status;r[0]=loaded;}goto L8c643d48; /* mov.b @(0x1,r12),r0 */
L8c643d48: *stop=0x8c643d48u;r[0]=r[0]&255u;goto L8c643d4a; /* extu.b r0,r0 */
L8c643d4a: *stop=0x8c643d4au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c643d4c; /* mov.l r0,@-r15 */
L8c643d4c: *stop=0x8c643d4cu;if(!valid_access(r[12],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[3]=loaded;}goto L8c643d4e; /* mov.b @r12,r3 */
L8c643d4e: *stop=0x8c643d4eu;r[3]=r[3]&255u;goto L8c643d50; /* extu.b r3,r3 */
L8c643d50: *stop=0x8c643d50u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c643d52; /* mov.l r3,@-r15 */
L8c643d52: *stop=0x8c643d52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643d7cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643d54; /* mov.l 0x8c643d7c,r3 */
L8c643d54: *stop=0x8c643d54u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c643d56; /* mov.l r12,@-r15 */
L8c643d56: target=r[3];*pr=0x8c643d5au;*stop=0x8c643d58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643d5a; /* jsr @r3 */
L8c643d5a: *stop=0x8c643d5au;r[15]+=24u;goto L8c643d5c; /* add #0x18,r15 */
L8c643d5c: *stop=0x8c643d5cu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c643d5e; /* tst r13,r13 */
L8c643d5e: if((*sr&1u))goto L8c643d68;goto L8c643d60; /* bt 0x8c643d68 */
L8c643d60: *stop=0x8c643d60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643d84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643d62; /* mov.l 0x8c643d84,r3 */
L8c643d62: *stop=0x8c643d62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643d80u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643d64; /* mov.l 0x8c643d80,r4 */
L8c643d64: target=r[3];*pr=0x8c643d68u;*stop=0x8c643d66u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643d68; /* jsr @r3 */
L8c643d68: *stop=0x8c643d68u;r[0]=r[13];goto L8c643d6a; /* mov r13,r0 */
L8c643d6a: *stop=0x8c643d6au;r[15]+=12u;goto L8c643d6c; /* add #0xc,r15 */
L8c643d6c: *stop=0x8c643d6cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c643d6e; /* lds.l @r15+,PR */
L8c643d6e: *stop=0x8c643d6eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c643d70; /* mov.l @r15+,r11 */
L8c643d70: *stop=0x8c643d70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c643d72; /* mov.l @r15+,r12 */
L8c643d72: *stop=0x8c643d72u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c643d74; /* mov.l @r15+,r13 */
L8c643d74: *stop=0x8c643d74u;if(*pr!=entry_pr)return -3;*stop=0x8c643d76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
