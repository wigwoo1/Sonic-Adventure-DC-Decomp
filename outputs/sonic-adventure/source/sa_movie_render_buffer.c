/* Original render-buffer descriptor, sizing and allocation; explicit complete state.
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
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x05000000u && (a&0x1fffffffu)<0x05800000u)return 1;
 if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_render_buffer_8c6536ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6536ec;
L8c6536ec: *stop=0x8c6536ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6536ee; /* mov.l r14,@-r15 */
L8c6536ee: *stop=0x8c6536eeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6536f0; /* mov.l r13,@-r15 */
L8c6536f0: *stop=0x8c6536f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6536f2; /* sts.l PR,@-r15 */
L8c6536f2: target=0x8c653f08u;*pr=0x8c6536f6u;*stop=0x8c6536f4u;r[14]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6536f6; /* bsr 0x8c653f08 */
L8c6536f6: *stop=0x8c6536f6u;r[4]=r[0];goto L8c6536f8; /* mov r0,r4 */
L8c6536f8: *stop=0x8c6536f8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6536fa; /* tst r4,r4 */
L8c6536fa: if((*sr&1u))goto L8c653700;goto L8c6536fc; /* bt 0x8c653700 */
L8c6536fc: *stop=0x8c6536feu;r[0]=r[4];goto L8c653728; /* bra 0x8c653728 */
L8c653700: *stop=0x8c653700u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c653702; /* mov.l @r14,r0 */
L8c653702: *stop=0x8c653702u;r[0]=r[0]&65535u;goto L8c653704; /* extu.w r0,r0 */
L8c653704: *stop=0x8c653704u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c653706; /* cmp/eq #0x3,r0 */
L8c653706: if(!(*sr&1u))goto L8c653712;goto L8c653708; /* bf 0x8c653712 */
L8c653708: *stop=0x8c653708u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65374cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65370a; /* mov.l 0x8c65374c,r2 */
L8c65370a: target=r[2];*pr=0x8c65370eu;*stop=0x8c65370cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65370e; /* jsr @r2 */
L8c65370e: *stop=0x8c653710u;goto L8c65371a; /* bra 0x8c65371a */
L8c653712: *stop=0x8c653712u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653750u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c653714; /* mov.l 0x8c653750,r2 */
L8c653714: *stop=0x8c653714u;r[5]=r[14];goto L8c653716; /* mov r14,r5 */
L8c653716: target=r[2];*pr=0x8c65371au;*stop=0x8c653718u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65371a; /* jsr @r2 */
L8c65371a: *stop=0x8c65371au;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c65371c; /* cmp/eq #0x3,r0 */
L8c65371c: take=!(*sr&1u);*stop=0x8c65371eu;r[13]=r[0];if(take)goto L8c653726;goto L8c653720; /* bf/s 0x8c653726 */
L8c653720: *stop=0x8c653720u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653754u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c653722; /* mov.l 0x8c653754,r2 */
L8c653722: target=r[2];*pr=0x8c653726u;*stop=0x8c653724u;r[4]=3u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653726; /* jsr @r2 */
L8c653726: *stop=0x8c653726u;r[0]=r[13];goto L8c653728; /* mov r13,r0 */
L8c653728: *stop=0x8c653728u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65372a; /* lds.l @r15+,PR */
L8c65372a: *stop=0x8c65372au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65372c; /* mov.l @r15+,r13 */
L8c65372c: *stop=0x8c65372cu;if(*pr!=entry_pr)return -3;*stop=0x8c65372eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c653f08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c653f08;
L8c653f08: *stop=0x8c653f08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c653f0a; /* mov.l r14,@-r15 */
L8c653f0a: *stop=0x8c653f0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c653f0c; /* mov.l r13,@-r15 */
L8c653f0c: *stop=0x8c653f0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c653f0e; /* mov.l r12,@-r15 */
L8c653f0e: *stop=0x8c653f0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c653f10; /* mov.l r11,@-r15 */
L8c653f10: *stop=0x8c653f10u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c653f12; /* mov.l r10,@-r15 */
L8c653f12: *stop=0x8c653f12u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c653f14; /* sts.l PR,@-r15 */
L8c653f14: *stop=0x8c653f14u;r[15]+=4294967292u;goto L8c653f16; /* add #-0x4,r15 */
L8c653f16: *stop=0x8c653f16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653f50u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c653f18; /* mov.l 0x8c653f50,r14 */
L8c653f18: *stop=0x8c653f18u;r[11]=0u;goto L8c653f1a; /* mov #0x0,r11 */
L8c653f1a: *stop=0x8c653f1au;{uint32_t loaded=sx(m,bus,&io_status,0x8c653f40u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c653f1c; /* mov.w 0x8c653f40,r3 */
L8c653f1c: *stop=0x8c653f1cu;r[13]=r[11];goto L8c653f1e; /* mov r11,r13 */
L8c653f1e: *stop=0x8c653f1eu;r[14]&=r[7];goto L8c653f20; /* and r7,r14 */
L8c653f20: *stop=0x8c653f20u;r[7]=r[7]&255u;goto L8c653f22; /* extu.b r7,r7 */
L8c653f22: *stop=0x8c653f22u;*sr=(*sr&~1u)|(r[3]==r[14]);goto L8c653f24; /* cmp/eq r3,r14 */
L8c653f24: *stop=0x8c653f24u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c653f26; /* mov.l r7,@r15 */
L8c653f26: take=(*sr&1u);*stop=0x8c653f28u;r[10]=3u;if(take)goto L8c653f30;goto L8c653f2a; /* bt/s 0x8c653f30 */
L8c653f2a: *stop=0x8c653f2au;{uint32_t loaded=sx(m,bus,&io_status,0x8c653f42u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c653f2c; /* mov.w 0x8c653f42,r1 */
L8c653f2c: *stop=0x8c653f2cu;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c653f2e; /* cmp/eq r1,r14 */
L8c653f2e: if(!(*sr&1u))goto L8c653f60;goto L8c653f30; /* bf 0x8c653f60 */
L8c653f30: *stop=0x8c653f32u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[10],4);if(io_status)return io_status;goto L8c653f64; /* bra 0x8c653f64 */
L8c653f60: *stop=0x8c653f60u;r[2]=2u;goto L8c653f62; /* mov #0x2,r2 */
L8c653f62: *stop=0x8c653f62u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c653f64; /* mov.l r2,@r4 */
L8c653f64: *stop=0x8c653f64u;{uint32_t loaded=sx(m,bus,&io_status,0x8c653fc0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c653f66; /* mov.w 0x8c653fc0,r3 */
L8c653f66: *stop=0x8c653f66u;r[12]=1u;goto L8c653f68; /* mov #0x1,r12 */
L8c653f68: *stop=0x8c653f68u;*sr=(*sr&~1u)|(r[3]==r[14]);goto L8c653f6a; /* cmp/eq r3,r14 */
L8c653f6a: take=(*sr&1u);*stop=0x8c653f6cu;r[7]=4u;if(take)goto L8c653f74;goto L8c653f6e; /* bt/s 0x8c653f74 */
L8c653f6e: *stop=0x8c653f6eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c653fc2u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c653f70; /* mov.w 0x8c653fc2,r1 */
L8c653f70: *stop=0x8c653f70u;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c653f72; /* cmp/eq r1,r14 */
L8c653f72: if(!(*sr&1u))goto L8c653f8c;goto L8c653f74; /* bf 0x8c653f8c */
L8c653f74: *stop=0x8c653f74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653fccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c653f76; /* mov.l 0x8c653fcc,r3 */
L8c653f76: *stop=0x8c653f76u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c653f78; /* mov.l @r3,r0 */
L8c653f78: *stop=0x8c653f78u;{uint32_t loaded=sx(m,bus,&io_status,0x8c653fc4u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c653f7a; /* mov.w 0x8c653fc4,r1 */
L8c653f7a: *stop=0x8c653f7au;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c653f7c; /* mov.l @(r0,r1),r0 */
L8c653f7c: *stop=0x8c653f7cu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c653f7e; /* cmp/eq #0x3,r0 */
L8c653f7e: if(!(*sr&1u))goto L8c653f84;goto L8c653f80; /* bf 0x8c653f84 */
L8c653f80: *stop=0x8c653f82u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[10],4);if(io_status)return io_status;goto L8c653f86; /* bra 0x8c653f86 */
L8c653f84: *stop=0x8c653f84u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[12],4);if(io_status)return io_status;goto L8c653f86; /* mov.l r12,@(0x4,r4) */
L8c653f86: *stop=0x8c653f86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653fd0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c653f88; /* mov.l 0x8c653fd0,r3 */
L8c653f88: *stop=0x8c653f8au;goto L8c653fac; /* bra 0x8c653fac */
L8c653f8c: *stop=0x8c653f8cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c653fc6u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c653f8e; /* mov.w 0x8c653fc6,r1 */
L8c653f8e: *stop=0x8c653f8eu;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c653f90; /* cmp/eq r1,r14 */
L8c653f90: if((*sr&1u))goto L8c653f98;goto L8c653f92; /* bt 0x8c653f98 */
L8c653f92: *stop=0x8c653f92u;{uint32_t loaded=sx(m,bus,&io_status,0x8c653fc8u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c653f94; /* mov.w 0x8c653fc8,r2 */
L8c653f94: *stop=0x8c653f94u;*sr=(*sr&~1u)|(r[2]==r[14]);goto L8c653f96; /* cmp/eq r2,r14 */
L8c653f96: if(!(*sr&1u))goto L8c653fe0;goto L8c653f98; /* bf 0x8c653fe0 */
L8c653f98: *stop=0x8c653f98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653fccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c653f9a; /* mov.l 0x8c653fcc,r3 */
L8c653f9a: *stop=0x8c653f9au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c653f9c; /* mov.l @r3,r0 */
L8c653f9c: *stop=0x8c653f9cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c653fc4u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c653f9e; /* mov.w 0x8c653fc4,r1 */
L8c653f9e: *stop=0x8c653f9eu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c653fa0; /* mov.l @(r0,r1),r0 */
L8c653fa0: *stop=0x8c653fa0u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c653fa2; /* cmp/eq #0x3,r0 */
L8c653fa2: if(!(*sr&1u))goto L8c653fa8;goto L8c653fa4; /* bf 0x8c653fa8 */
L8c653fa4: *stop=0x8c653fa6u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[10],4);if(io_status)return io_status;goto L8c653faa; /* bra 0x8c653faa */
L8c653fa8: *stop=0x8c653fa8u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[12],4);if(io_status)return io_status;goto L8c653faa; /* mov.l r12,@(0x4,r4) */
L8c653faa: *stop=0x8c653faau;{uint32_t loaded=rd(m,bus,&io_status,0x8c653fd4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c653fac; /* mov.l 0x8c653fd4,r3 */
L8c653fac: *stop=0x8c653faeu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[3],4);if(io_status)return io_status;goto L8c654042; /* bra 0x8c654042 */
L8c653fe0: *stop=0x8c653fe0u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[12],4);if(io_status)return io_status;goto L8c653fe2; /* mov.l r12,@(0x4,r4) */
L8c653fe2: *stop=0x8c653fe2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c653fe4; /* mov.l @r15,r0 */
L8c653fe4: *stop=0x8c653fe4u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c653fe6; /* cmp/eq #0x0,r0 */
L8c653fe6: if((*sr&1u))goto L8c653ffc;goto L8c653fe8; /* bt 0x8c653ffc */
L8c653fe8: *stop=0x8c653fe8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c653fea; /* cmp/eq #0x1,r0 */
L8c653fea: if((*sr&1u))goto L8c654000;goto L8c653fec; /* bt 0x8c654000 */
L8c653fec: *stop=0x8c653fecu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c653fee; /* cmp/eq #0x2,r0 */
L8c653fee: if((*sr&1u))goto L8c654006;goto L8c653ff0; /* bt 0x8c654006 */
L8c653ff0: *stop=0x8c653ff0u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c653ff2; /* cmp/eq #0x3,r0 */
L8c653ff2: if((*sr&1u))goto L8c65400c;goto L8c653ff4; /* bt 0x8c65400c */
L8c653ff4: *stop=0x8c653ff4u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c653ff6; /* cmp/eq #0x4,r0 */
L8c653ff6: if((*sr&1u))goto L8c654012;goto L8c653ff8; /* bt 0x8c654012 */
L8c653ff8: *stop=0x8c653ffau;goto L8c654040; /* bra 0x8c654040 */
L8c653ffc: *stop=0x8c653ffeu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[11],4);if(io_status)return io_status;goto L8c654042; /* bra 0x8c654042 */
L8c654000: *stop=0x8c654000u;{uint32_t loaded=rd(m,bus,&io_status,0x8c654020u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c654002; /* mov.l 0x8c654020,r2 */
L8c654002: *stop=0x8c654004u;goto L8c65400e; /* bra 0x8c65400e */
L8c654006: *stop=0x8c654006u;{uint32_t loaded=rd(m,bus,&io_status,0x8c654024u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c654008; /* mov.l 0x8c654024,r1 */
L8c654008: *stop=0x8c65400au;goto L8c654014; /* bra 0x8c654014 */
L8c65400c: *stop=0x8c65400cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c654028u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65400e; /* mov.l 0x8c654028,r2 */
L8c65400e: *stop=0x8c654010u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c654042; /* bra 0x8c654042 */
L8c654012: *stop=0x8c654012u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65402cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c654014; /* mov.l 0x8c65402c,r1 */
L8c654014: *stop=0x8c654016u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[1],4);if(io_status)return io_status;goto L8c654042; /* bra 0x8c654042 */
L8c654040: *stop=0x8c654040u;r[13]=r[7];goto L8c654042; /* mov r7,r13 */
L8c654042: *stop=0x8c654042u;r[0]=r[5];goto L8c654044; /* mov r5,r0 */
L8c654044: *stop=0x8c654044u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c654046; /* cmp/eq #0x8,r0 */
L8c654046: *stop=0x8c654046u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[5],4);if(io_status)return io_status;goto L8c654048; /* mov.l r5,@(0xc,r4) */
L8c654048: take=(*sr&1u);*stop=0x8c65404au;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[6],4);if(io_status)return io_status;if(take)goto L8c654072;goto L8c65404c; /* bt/s 0x8c654072 */
L8c65404c: *stop=0x8c65404cu;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c65404e; /* cmp/eq #0x10,r0 */
L8c65404e: if((*sr&1u))goto L8c654072;goto L8c654050; /* bt 0x8c654072 */
L8c654050: *stop=0x8c654050u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c654052; /* cmp/eq #0x20,r0 */
L8c654052: if((*sr&1u))goto L8c654072;goto L8c654054; /* bt 0x8c654072 */
L8c654054: *stop=0x8c654054u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c654056; /* cmp/eq #0x40,r0 */
L8c654056: if((*sr&1u))goto L8c654072;goto L8c654058; /* bt 0x8c654072 */
L8c654058: *stop=0x8c654058u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654100u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65405a; /* mov.w 0x8c654100,r1 */
L8c65405a: *stop=0x8c65405au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c65405c; /* cmp/eq r1,r0 */
L8c65405c: if((*sr&1u))goto L8c654072;goto L8c65405e; /* bt 0x8c654072 */
L8c65405e: *stop=0x8c65405eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c654102u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654060; /* mov.w 0x8c654102,r1 */
L8c654060: *stop=0x8c654060u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c654062; /* cmp/eq r1,r0 */
L8c654062: if((*sr&1u))goto L8c654072;goto L8c654064; /* bt 0x8c654072 */
L8c654064: *stop=0x8c654064u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654104u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654066; /* mov.w 0x8c654104,r1 */
L8c654066: *stop=0x8c654066u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c654068; /* cmp/eq r1,r0 */
L8c654068: if((*sr&1u))goto L8c654072;goto L8c65406a; /* bt 0x8c654072 */
L8c65406a: *stop=0x8c65406au;{uint32_t loaded=sx(m,bus,&io_status,0x8c654106u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65406c; /* mov.w 0x8c654106,r1 */
L8c65406c: *stop=0x8c65406cu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c65406e; /* cmp/eq r1,r0 */
L8c65406e: if((*sr&1u))goto L8c654072;goto L8c654070; /* bt 0x8c654072 */
L8c654070: *stop=0x8c654070u;r[13]=r[7];goto L8c654072; /* mov r7,r13 */
L8c654072: *stop=0x8c654072u;r[0]=r[6];goto L8c654074; /* mov r6,r0 */
L8c654074: *stop=0x8c654074u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c654076; /* cmp/eq #0x8,r0 */
L8c654076: if((*sr&1u))goto L8c65409e;goto L8c654078; /* bt 0x8c65409e */
L8c654078: *stop=0x8c654078u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c65407a; /* cmp/eq #0x10,r0 */
L8c65407a: if((*sr&1u))goto L8c65409e;goto L8c65407c; /* bt 0x8c65409e */
L8c65407c: *stop=0x8c65407cu;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c65407e; /* cmp/eq #0x20,r0 */
L8c65407e: if((*sr&1u))goto L8c65409e;goto L8c654080; /* bt 0x8c65409e */
L8c654080: *stop=0x8c654080u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c654082; /* cmp/eq #0x40,r0 */
L8c654082: if((*sr&1u))goto L8c65409e;goto L8c654084; /* bt 0x8c65409e */
L8c654084: *stop=0x8c654084u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654100u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654086; /* mov.w 0x8c654100,r1 */
L8c654086: *stop=0x8c654086u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c654088; /* cmp/eq r1,r0 */
L8c654088: if((*sr&1u))goto L8c65409e;goto L8c65408a; /* bt 0x8c65409e */
L8c65408a: *stop=0x8c65408au;{uint32_t loaded=sx(m,bus,&io_status,0x8c654102u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65408c; /* mov.w 0x8c654102,r1 */
L8c65408c: *stop=0x8c65408cu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c65408e; /* cmp/eq r1,r0 */
L8c65408e: if((*sr&1u))goto L8c65409e;goto L8c654090; /* bt 0x8c65409e */
L8c654090: *stop=0x8c654090u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654104u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654092; /* mov.w 0x8c654104,r1 */
L8c654092: *stop=0x8c654092u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c654094; /* cmp/eq r1,r0 */
L8c654094: if((*sr&1u))goto L8c65409e;goto L8c654096; /* bt 0x8c65409e */
L8c654096: *stop=0x8c654096u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654106u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654098; /* mov.w 0x8c654106,r1 */
L8c654098: *stop=0x8c654098u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c65409a; /* cmp/eq r1,r0 */
L8c65409a: if((*sr&1u))goto L8c65409e;goto L8c65409c; /* bt 0x8c65409e */
L8c65409c: *stop=0x8c65409cu;r[13]=r[7];goto L8c65409e; /* mov r7,r13 */
L8c65409e: *stop=0x8c65409eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c654108u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6540a0; /* mov.w 0x8c654108,r3 */
L8c6540a0: *stop=0x8c6540a0u;*sr=(*sr&~1u)|(r[3]==r[14]);goto L8c6540a2; /* cmp/eq r3,r14 */
L8c6540a2: if((*sr&1u))goto L8c6540c2;goto L8c6540a4; /* bt 0x8c6540c2 */
L8c6540a4: *stop=0x8c6540a4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65410au,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6540a6; /* mov.w 0x8c65410a,r1 */
L8c6540a6: *stop=0x8c6540a6u;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c6540a8; /* cmp/eq r1,r14 */
L8c6540a8: if((*sr&1u))goto L8c6540c2;goto L8c6540aa; /* bt 0x8c6540c2 */
L8c6540aa: *stop=0x8c6540aau;{uint32_t loaded=sx(m,bus,&io_status,0x8c65410cu,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6540ac; /* mov.w 0x8c65410c,r0 */
L8c6540ac: *stop=0x8c6540acu;*sr=(*sr&~1u)|(r[0]==r[14]);goto L8c6540ae; /* cmp/eq r0,r14 */
L8c6540ae: if((*sr&1u))goto L8c6540c2;goto L8c6540b0; /* bt 0x8c6540c2 */
L8c6540b0: *stop=0x8c6540b0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65410eu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c6540b2; /* mov.w 0x8c65410e,r2 */
L8c6540b2: *stop=0x8c6540b2u;*sr=(*sr&~1u)|(r[2]==r[14]);goto L8c6540b4; /* cmp/eq r2,r14 */
L8c6540b4: if((*sr&1u))goto L8c6540c2;goto L8c6540b6; /* bt 0x8c6540c2 */
L8c6540b6: *stop=0x8c6540b6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654110u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6540b8; /* mov.w 0x8c654110,r1 */
L8c6540b8: *stop=0x8c6540b8u;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c6540ba; /* cmp/eq r1,r14 */
L8c6540ba: if((*sr&1u))goto L8c6540c2;goto L8c6540bc; /* bt 0x8c6540c2 */
L8c6540bc: *stop=0x8c6540bcu;*sr=(*sr&~1u)|(r[6]==r[5]);goto L8c6540be; /* cmp/eq r6,r5 */
L8c6540be: if((*sr&1u))goto L8c6540c2;goto L8c6540c0; /* bt 0x8c6540c2 */
L8c6540c0: *stop=0x8c6540c0u;r[13]=r[7];goto L8c6540c2; /* mov r7,r13 */
L8c6540c2: *stop=0x8c6540c2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654112u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6540c4; /* mov.w 0x8c654112,r3 */
L8c6540c4: *stop=0x8c6540c4u;*sr=(*sr&~1u)|(r[3]==r[14]);goto L8c6540c6; /* cmp/eq r3,r14 */
L8c6540c6: take=(*sr&1u);*stop=0x8c6540c8u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[11],4);if(io_status)return io_status;if(take)goto L8c6540e8;goto L8c6540ca; /* bt/s 0x8c6540e8 */
L8c6540ca: *stop=0x8c6540cau;{uint32_t loaded=sx(m,bus,&io_status,0x8c654104u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6540cc; /* mov.w 0x8c654104,r1 */
L8c6540cc: *stop=0x8c6540ccu;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c6540ce; /* cmp/eq r1,r14 */
L8c6540ce: if((*sr&1u))goto L8c6540e8;goto L8c6540d0; /* bt 0x8c6540e8 */
L8c6540d0: *stop=0x8c6540d0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654106u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6540d2; /* mov.w 0x8c654106,r0 */
L8c6540d2: *stop=0x8c6540d2u;*sr=(*sr&~1u)|(r[0]==r[14]);goto L8c6540d4; /* cmp/eq r0,r14 */
L8c6540d4: if((*sr&1u))goto L8c6540e8;goto L8c6540d6; /* bt 0x8c6540e8 */
L8c6540d6: *stop=0x8c6540d6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654114u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c6540d8; /* mov.w 0x8c654114,r2 */
L8c6540d8: *stop=0x8c6540d8u;*sr=(*sr&~1u)|(r[2]==r[14]);goto L8c6540da; /* cmp/eq r2,r14 */
L8c6540da: if((*sr&1u))goto L8c6540e8;goto L8c6540dc; /* bt 0x8c6540e8 */
L8c6540dc: *stop=0x8c6540dcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c654116u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6540de; /* mov.w 0x8c654116,r1 */
L8c6540de: *stop=0x8c6540deu;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c6540e0; /* cmp/eq r1,r14 */
L8c6540e0: if((*sr&1u))goto L8c6540e8;goto L8c6540e2; /* bt 0x8c6540e8 */
L8c6540e2: *stop=0x8c6540e2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654118u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c6540e4; /* mov.w 0x8c654118,r2 */
L8c6540e4: *stop=0x8c6540e4u;*sr=(*sr&~1u)|(r[2]==r[14]);goto L8c6540e6; /* cmp/eq r2,r14 */
L8c6540e6: if(!(*sr&1u))goto L8c6540ee;goto L8c6540e8; /* bf 0x8c6540ee */
L8c6540e8: *stop=0x8c6540e8u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6540ea; /* mov.l @(0x18,r4),r1 */
L8c6540ea: *stop=0x8c6540eau;r[1]|=r[12];goto L8c6540ec; /* or r12,r1 */
L8c6540ec: *stop=0x8c6540ecu;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[1],4);if(io_status)return io_status;goto L8c6540ee; /* mov.l r1,@(0x18,r4) */
L8c6540ee: *stop=0x8c6540eeu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65411au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6540f0; /* mov.w 0x8c65411a,r3 */
L8c6540f0: *stop=0x8c6540f0u;*sr=(*sr&~1u)|(r[3]==r[14]);goto L8c6540f2; /* cmp/eq r3,r14 */
L8c6540f2: if((*sr&1u))goto L8c654136;goto L8c6540f4; /* bt 0x8c654136 */
L8c6540f4: *stop=0x8c6540f4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654112u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6540f6; /* mov.w 0x8c654112,r1 */
L8c6540f6: *stop=0x8c6540f8u;goto L8c654120; /* bra 0x8c654120 */
L8c654120: *stop=0x8c654120u;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c654122; /* cmp/eq r1,r14 */
L8c654122: if((*sr&1u))goto L8c654136;goto L8c654124; /* bt 0x8c654136 */
L8c654124: *stop=0x8c654124u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541c0u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c654126; /* mov.w 0x8c6541c0,r0 */
L8c654126: *stop=0x8c654126u;*sr=(*sr&~1u)|(r[0]==r[14]);goto L8c654128; /* cmp/eq r0,r14 */
L8c654128: if((*sr&1u))goto L8c654136;goto L8c65412a; /* bt 0x8c654136 */
L8c65412a: *stop=0x8c65412au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541c2u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c65412c; /* mov.w 0x8c6541c2,r2 */
L8c65412c: *stop=0x8c65412cu;*sr=(*sr&~1u)|(r[2]==r[14]);goto L8c65412e; /* cmp/eq r2,r14 */
L8c65412e: if((*sr&1u))goto L8c654136;goto L8c654130; /* bt 0x8c654136 */
L8c654130: *stop=0x8c654130u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541c4u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654132; /* mov.w 0x8c6541c4,r1 */
L8c654132: *stop=0x8c654132u;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c654134; /* cmp/eq r1,r14 */
L8c654134: if(!(*sr&1u))goto L8c65413c;goto L8c654136; /* bf 0x8c65413c */
L8c654136: *stop=0x8c654136u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c654138; /* mov.l @(0x18,r4),r2 */
L8c654138: *stop=0x8c654138u;r[2]|=r[7];goto L8c65413a; /* or r7,r2 */
L8c65413a: *stop=0x8c65413au;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[2],4);if(io_status)return io_status;goto L8c65413c; /* mov.l r2,@(0x18,r4) */
L8c65413c: *stop=0x8c65413cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541c6u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65413e; /* mov.w 0x8c6541c6,r3 */
L8c65413e: *stop=0x8c65413eu;*sr=(*sr&~1u)|(r[3]==r[14]);goto L8c654140; /* cmp/eq r3,r14 */
L8c654140: if((*sr&1u))goto L8c654154;goto L8c654142; /* bt 0x8c654154 */
L8c654142: *stop=0x8c654142u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541c8u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654144; /* mov.w 0x8c6541c8,r1 */
L8c654144: *stop=0x8c654144u;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c654146; /* cmp/eq r1,r14 */
L8c654146: if((*sr&1u))goto L8c654154;goto L8c654148; /* bt 0x8c654154 */
L8c654148: *stop=0x8c654148u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541cau,2);if(io_status)return io_status;r[0]=loaded;}goto L8c65414a; /* mov.w 0x8c6541ca,r0 */
L8c65414a: *stop=0x8c65414au;*sr=(*sr&~1u)|(r[0]==r[14]);goto L8c65414c; /* cmp/eq r0,r14 */
L8c65414c: if((*sr&1u))goto L8c654154;goto L8c65414e; /* bt 0x8c654154 */
L8c65414e: *stop=0x8c65414eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541ccu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c654150; /* mov.w 0x8c6541cc,r2 */
L8c654150: *stop=0x8c654150u;*sr=(*sr&~1u)|(r[2]==r[14]);goto L8c654152; /* cmp/eq r2,r14 */
L8c654152: if(!(*sr&1u))goto L8c65415e;goto L8c654154; /* bf 0x8c65415e */
L8c654154: *stop=0x8c654154u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c654156; /* mov.l @(0x18,r4),r0 */
L8c654156: *stop=0x8c654156u;r[0]|=0x8u;goto L8c654158; /* or #0x8,r0 */
L8c654158: *stop=0x8c654158u;r[2]=r[0];goto L8c65415a; /* mov r0,r2 */
L8c65415a: *stop=0x8c65415au;r[2]|=r[7];goto L8c65415c; /* or r7,r2 */
L8c65415c: *stop=0x8c65415cu;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[2],4);if(io_status)return io_status;goto L8c65415e; /* mov.l r2,@(0x18,r4) */
L8c65415e: *stop=0x8c65415eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541ceu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c654160; /* mov.w 0x8c6541ce,r3 */
L8c654160: *stop=0x8c654160u;*sr=(*sr&~1u)|(r[3]==r[14]);goto L8c654162; /* cmp/eq r3,r14 */
L8c654162: if(!(*sr&1u))goto L8c65416a;goto L8c654164; /* bf 0x8c65416a */
L8c654164: *stop=0x8c654164u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c654166; /* mov.l @(0x18,r4),r0 */
L8c654166: *stop=0x8c654166u;r[0]|=0x10u;goto L8c654168; /* or #0x10,r0 */
L8c654168: *stop=0x8c654168u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[0],4);if(io_status)return io_status;goto L8c65416a; /* mov.l r0,@(0x18,r4) */
L8c65416a: *stop=0x8c65416au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541d0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65416c; /* mov.w 0x8c6541d0,r3 */
L8c65416c: *stop=0x8c65416cu;*sr=(*sr&~1u)|(r[3]==r[14]);goto L8c65416e; /* cmp/eq r3,r14 */
L8c65416e: if((*sr&1u))goto L8c654182;goto L8c654170; /* bt 0x8c654182 */
L8c654170: *stop=0x8c654170u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541d2u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654172; /* mov.w 0x8c6541d2,r1 */
L8c654172: *stop=0x8c654172u;*sr=(*sr&~1u)|(r[1]==r[14]);goto L8c654174; /* cmp/eq r1,r14 */
L8c654174: if((*sr&1u))goto L8c654182;goto L8c654176; /* bt 0x8c654182 */
L8c654176: *stop=0x8c654176u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541d4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c654178; /* mov.w 0x8c6541d4,r0 */
L8c654178: *stop=0x8c654178u;*sr=(*sr&~1u)|(r[0]==r[14]);goto L8c65417a; /* cmp/eq r0,r14 */
L8c65417a: if((*sr&1u))goto L8c654182;goto L8c65417c; /* bt 0x8c654182 */
L8c65417c: *stop=0x8c65417cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6541d6u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c65417e; /* mov.w 0x8c6541d6,r2 */
L8c65417e: *stop=0x8c65417eu;*sr=(*sr&~1u)|(r[2]==r[14]);goto L8c654180; /* cmp/eq r2,r14 */
L8c654180: if(!(*sr&1u))goto L8c65418c;goto L8c654182; /* bf 0x8c65418c */
L8c654182: *stop=0x8c654182u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c654184; /* mov.l @(0x18,r4),r0 */
L8c654184: *stop=0x8c654184u;r[0]|=0x20u;goto L8c654186; /* or #0x20,r0 */
L8c654186: *stop=0x8c654186u;r[2]=r[0];goto L8c654188; /* mov r0,r2 */
L8c654188: *stop=0x8c654188u;r[2]|=r[7];goto L8c65418a; /* or r7,r2 */
L8c65418a: *stop=0x8c65418au;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[2],4);if(io_status)return io_status;goto L8c65418c; /* mov.l r2,@(0x18,r4) */
L8c65418c: target=0x8c654286u;*pr=0x8c654190u;*stop=0x8c65418eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c654190; /* bsr 0x8c654286 */
L8c654190: *stop=0x8c654190u;r[0]=r[13];goto L8c654192; /* mov r13,r0 */
L8c654192: *stop=0x8c654192u;r[15]+=4u;goto L8c654194; /* add #0x4,r15 */
L8c654194: *stop=0x8c654194u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c654196; /* lds.l @r15+,PR */
L8c654196: *stop=0x8c654196u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c654198; /* mov.l @r15+,r10 */
L8c654198: *stop=0x8c654198u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65419a; /* mov.l @r15+,r11 */
L8c65419a: *stop=0x8c65419au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65419c; /* mov.l @r15+,r12 */
L8c65419c: *stop=0x8c65419cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65419e; /* mov.l @r15+,r13 */
L8c65419e: *stop=0x8c65419eu;if(*pr!=entry_pr)return -3;*stop=0x8c6541a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c663712(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c663712;
L8c663712: *stop=0x8c663712u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c663714; /* mov.l r14,@-r15 */
L8c663714: *stop=0x8c663714u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c663716; /* mov.l r13,@-r15 */
L8c663716: *stop=0x8c663716u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c663718; /* mov.l r12,@-r15 */
L8c663718: *stop=0x8c663718u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c66371a; /* mov.l r11,@-r15 */
L8c66371a: *stop=0x8c66371au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66371c; /* sts.l PR,@-r15 */
L8c66371c: *stop=0x8c66371cu;r[15]+=4294967268u;goto L8c66371e; /* add #-0x1c,r15 */
L8c66371e: *stop=0x8c66371eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6637e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663720; /* mov.l 0x8c6637e8,r3 */
L8c663720: *stop=0x8c663720u;r[5]=r[4];goto L8c663722; /* mov r4,r5 */
L8c663722: *stop=0x8c663722u;r[11]=r[4];goto L8c663724; /* mov r4,r11 */
L8c663724: *stop=0x8c663724u;r[4]=r[15];goto L8c663726; /* mov r15,r4 */
L8c663726: target=r[3];*pr=0x8c66372au;*stop=0x8c663728u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66372a; /* jsr @r3 */
L8c66372a: *stop=0x8c66372au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6637ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66372c; /* mov.l 0x8c6637ec,r2 */
L8c66372c: *stop=0x8c66372cu;r[13]=0u;goto L8c66372e; /* mov #0x0,r13 */
L8c66372e: *stop=0x8c66372eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto L8c663730; /* mov.l @r2,r4 */
L8c663730: *stop=0x8c663730u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c663732; /* tst r4,r4 */
L8c663732: take=(*sr&1u);*stop=0x8c663734u;r[14]=r[13];if(take)goto L8c663776;goto L8c663736; /* bt/s 0x8c663776 */
L8c663736: *stop=0x8c663736u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6637f0u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c663738; /* mov.l 0x8c6637f0,r7 */
L8c663738: *stop=0x8c663738u;if(!valid_access(r[4],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[2]=loaded;}goto L8c66373a; /* mov.w @r4,r2 */
L8c66373a: *stop=0x8c66373au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66373c; /* mov.l @(0x8,r15),r3 */
L8c66373c: *stop=0x8c66373cu;r[2]=r[2]&65535u;goto L8c66373e; /* extu.w r2,r2 */
L8c66373e: *stop=0x8c66373eu;r[2]&=r[7];goto L8c663740; /* and r7,r2 */
L8c663740: *stop=0x8c663740u;*sr=(*sr&~1u)|(r[3]==r[2]);goto L8c663742; /* cmp/eq r3,r2 */
L8c663742: if(!(*sr&1u))goto L8c66376c;goto L8c663744; /* bf 0x8c66376c */
L8c663744: *stop=0x8c663744u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c663746; /* mov.l @(0x18,r15),r6 */
L8c663746: *stop=0x8c663746u;r[3]=0u;goto L8c663748; /* mov #0x0,r3 */
L8c663748: *stop=0x8c663748u;*sr=(*sr&~1u)|(r[3]>=r[6]);goto L8c66374a; /* cmp/hs r6,r3 */
L8c66374a: take=(*sr&1u);*stop=0x8c66374cu;r[5]=r[13];if(take)goto L8c66376c;goto L8c66374e; /* bt/s 0x8c66376c */
L8c66374e: *stop=0x8c66374eu;r[3]=r[4];goto L8c663750; /* mov r4,r3 */
L8c663750: *stop=0x8c663750u;r[1]=r[5];goto L8c663752; /* mov r5,r1 */
L8c663752: *stop=0x8c663752u;r[3]+=12u;goto L8c663754; /* add #0xc,r3 */
L8c663754: *stop=0x8c663754u;r[1]<<=2;goto L8c663756; /* shll2 r1 */
L8c663756: *stop=0x8c663756u;r[1]+=r[3];goto L8c663758; /* add r3,r1 */
L8c663758: *stop=0x8c663758u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66375a; /* mov.l @r1,r2 */
L8c66375a: *stop=0x8c66375au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c66375c; /* tst r2,r2 */
L8c66375c: if(!(*sr&1u))goto L8c663766;goto L8c66375e; /* bf 0x8c663766 */
L8c66375e: *stop=0x8c66375eu;r[12]=r[5];goto L8c663760; /* mov r5,r12 */
L8c663760: *stop=0x8c663760u;r[14]=r[4];goto L8c663762; /* mov r4,r14 */
L8c663762: *stop=0x8c663762u;r[4]=r[13];goto L8c663764; /* mov r13,r4 */
L8c663764: *stop=0x8c663764u;r[5]=r[6];goto L8c663766; /* mov r6,r5 */
L8c663766: *stop=0x8c663766u;r[5]+=1u;goto L8c663768; /* add #0x1,r5 */
L8c663768: *stop=0x8c663768u;*sr=(*sr&~1u)|(r[5]>=r[6]);goto L8c66376a; /* cmp/hs r6,r5 */
L8c66376a: if(!(*sr&1u))goto L8c66374e;goto L8c66376c; /* bf 0x8c66374e */
L8c66376c: *stop=0x8c66376cu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c66376e; /* tst r14,r14 */
L8c66376e: if(!(*sr&1u))goto L8c663772;goto L8c663770; /* bf 0x8c663772 */
L8c663770: *stop=0x8c663770u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c663772; /* mov.l @(0x4,r4),r4 */
L8c663772: *stop=0x8c663772u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c663774; /* tst r4,r4 */
L8c663774: if(!(*sr&1u))goto L8c663738;goto L8c663776; /* bf 0x8c663738 */
L8c663776: *stop=0x8c663776u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c663778; /* tst r14,r14 */
L8c663778: if(!(*sr&1u))goto L8c66382a;goto L8c66377a; /* bf 0x8c66382a */
L8c66377a: *stop=0x8c66377au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6637f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66377c; /* mov.l 0x8c6637f4,r3 */
L8c66377c: target=r[3];*pr=0x8c663780u;*stop=0x8c66377eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663780; /* jsr @r3 */
L8c663780: *stop=0x8c663780u;r[14]=r[0];goto L8c663782; /* mov r0,r14 */
L8c663782: *stop=0x8c663782u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c663784; /* tst r14,r14 */
L8c663784: if((*sr&1u))goto L8c663826;goto L8c663786; /* bt 0x8c663826 */
L8c663786: *stop=0x8c663786u;r[4]=r[14];goto L8c663788; /* mov r14,r4 */
L8c663788: *stop=0x8c663788u;r[4]+=12u;goto L8c66378a; /* add #0xc,r4 */
L8c66378a: *stop=0x8c66378au;r[3]=r[14];goto L8c66378c; /* mov r14,r3 */
L8c66378c: *stop=0x8c66378cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[13],4);if(io_status)return io_status;goto L8c66378e; /* mov.l r13,@r4 */
L8c66378e: *stop=0x8c66378eu;r[3]+=76u;goto L8c663790; /* add #0x4c,r3 */
L8c663790: *stop=0x8c663790u;r[4]+=4u;goto L8c663792; /* add #0x4,r4 */
L8c663792: *stop=0x8c663792u;*sr=(*sr&~1u)|(r[4]>=r[3]);goto L8c663794; /* cmp/hs r3,r4 */
L8c663794: if(!(*sr&1u))goto L8c66378a;goto L8c663796; /* bf 0x8c66378a */
L8c663796: *stop=0x8c663796u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6637f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663798; /* mov.l 0x8c6637f8,r3 */
L8c663798: *stop=0x8c663798u;r[12]=r[13];goto L8c66379a; /* mov r13,r12 */
L8c66379a: *stop=0x8c66379au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6637ecu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c66379c; /* mov.l 0x8c6637ec,r4 */
L8c66379c: target=r[3];*pr=0x8c6637a0u;*stop=0x8c66379eu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6637a0; /* jsr @r3 */
L8c6637a0: *stop=0x8c6637a0u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[2]=loaded;}goto L8c6637a2; /* mov.w @r14,r2 */
L8c6637a2: *stop=0x8c6637a2u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6637a4; /* mov.l @(0x8,r15),r3 */
L8c6637a4: *stop=0x8c6637a4u;r[2]|=r[3];goto L8c6637a6; /* or r3,r2 */
L8c6637a6: *stop=0x8c6637a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6637fcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6637a8; /* mov.l 0x8c6637fc,r3 */
L8c6637a8: *stop=0x8c6637a8u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c6637aa; /* mov.w r2,@r14 */
L8c6637aa: *stop=0x8c6637aau;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c6637ac; /* mov.l @(0x14,r15),r13 */
L8c6637ac: *stop=0x8c6637acu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6637ae; /* mov.l @r3,r0 */
L8c6637ae: *stop=0x8c6637aeu;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c6637b0; /* tst #0x4,r0 */
L8c6637b0: if((*sr&1u))goto L8c663810;goto L8c6637b2; /* bt 0x8c663810 */
L8c6637b2: *stop=0x8c6637b2u;r[5]=r[15];goto L8c6637b4; /* mov r15,r5 */
L8c6637b4: *stop=0x8c6637b4u;r[5]+=4u;goto L8c6637b6; /* add #0x4,r5 */
L8c6637b6: target=0x8c663c86u;*pr=0x8c6637bau;*stop=0x8c6637b8u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6637ba; /* bsr 0x8c663c86 */
L8c6637ba: *stop=0x8c6637bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6637bc; /* mov.l @r15,r2 */
L8c6637bc: *stop=0x8c6637bcu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6637be; /* mov.l @(0x4,r15),r3 */
L8c6637be: *stop=0x8c6637beu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c6637c0; /* cmp/ge r3,r2 */
L8c6637c0: if((*sr&1u))goto L8c663800;goto L8c6637c2; /* bt 0x8c663800 */
L8c6637c2: *stop=0x8c6637c2u;r[5]=r[13];goto L8c6637c4; /* mov r13,r5 */
L8c6637c4: *stop=0x8c6637c4u;r[6]=r[14];goto L8c6637c6; /* mov r14,r6 */
L8c6637c6: *stop=0x8c6637c8u;r[4]=1u;goto L8c663806; /* bra 0x8c663806 */
L8c663800: *stop=0x8c663800u;r[4]=0u;goto L8c663802; /* mov #0x0,r4 */
L8c663802: *stop=0x8c663802u;r[5]=r[13];goto L8c663804; /* mov r13,r5 */
L8c663804: *stop=0x8c663804u;r[6]=r[14];goto L8c663806; /* mov r14,r6 */
L8c663806: *stop=0x8c663806u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663900u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663808; /* mov.l 0x8c663900,r2 */
L8c663808: target=r[2];*pr=0x8c66380cu;*stop=0x8c66380au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66380c; /* jsr @r2 */
L8c66380c: *stop=0x8c66380eu;r[4]=r[0];goto L8c66381c; /* bra 0x8c66381c */
L8c663810: *stop=0x8c663810u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663900u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663812; /* mov.l 0x8c663900,r2 */
L8c663812: *stop=0x8c663812u;r[5]=r[13];goto L8c663814; /* mov r13,r5 */
L8c663814: *stop=0x8c663814u;r[6]=r[14];goto L8c663816; /* mov r14,r6 */
L8c663816: target=r[2];*pr=0x8c66381au;*stop=0x8c663818u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66381a; /* jsr @r2 */
L8c66381a: *stop=0x8c66381au;r[4]=r[0];goto L8c66381c; /* mov r0,r4 */
L8c66381c: *stop=0x8c66381cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c66381e; /* tst r4,r4 */
L8c66381e: if((*sr&1u))goto L8c66382a;goto L8c663820; /* bt 0x8c66382a */
L8c663820: *stop=0x8c663820u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663904u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663822; /* mov.l 0x8c663904,r2 */
L8c663822: target=r[2];*pr=0x8c663826u;*stop=0x8c663824u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663826; /* jsr @r2 */
L8c663826: *stop=0x8c663828u;r[0]=3u;goto L8c66384c; /* bra 0x8c66384c */
L8c66382a: *stop=0x8c66382au;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66382c; /* mov.l @(0xc,r15),r3 */
L8c66382c: *stop=0x8c66382cu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66382e; /* mov.l @(0x8,r14),r2 */
L8c66382e: *stop=0x8c66382eu;r[3]>>=2;goto L8c663830; /* shlr2 r3 */
L8c663830: *stop=0x8c663830u;*macl=r[3]*r[12];goto L8c663832; /* mul.l r3,r12 */
L8c663832: *stop=0x8c663832u;r[12]<<=2;goto L8c663834; /* shll2 r12 */
L8c663834: *stop=0x8c663834u;r[3]=*macl;goto L8c663836; /* sts MACL,r3 */
L8c663836: *stop=0x8c663836u;r[3]<<=2;goto L8c663838; /* shll2 r3 */
L8c663838: *stop=0x8c663838u;r[3]+=r[2];goto L8c66383a; /* add r2,r3 */
L8c66383a: *stop=0x8c66383au;if(!valid_access(r[11]+28u,4u))return -3;wr(m,bus,&io_status,r[11]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c66383c; /* mov.l r3,@(0x1c,r11) */
L8c66383c: *stop=0x8c66383cu;r[3]=r[14];goto L8c66383e; /* mov r14,r3 */
L8c66383e: *stop=0x8c66383eu;r[3]+=12u;goto L8c663840; /* add #0xc,r3 */
L8c663840: *stop=0x8c663840u;r[12]+=r[3];goto L8c663842; /* add r3,r12 */
L8c663842: *stop=0x8c663842u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[11],4);if(io_status)return io_status;goto L8c663844; /* mov.l r11,@r12 */
L8c663844: *stop=0x8c663844u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[0]=loaded;}goto L8c663846; /* mov.w @r14,r0 */
L8c663846: *stop=0x8c663846u;r[0]|=0x2u;goto L8c663848; /* or #0x2,r0 */
L8c663848: *stop=0x8c663848u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[0],2);if(io_status)return io_status;goto L8c66384a; /* mov.w r0,@r14 */
L8c66384a: *stop=0x8c66384au;r[0]=0u;goto L8c66384c; /* mov #0x0,r0 */
L8c66384c: *stop=0x8c66384cu;r[15]+=28u;goto L8c66384e; /* add #0x1c,r15 */
L8c66384e: *stop=0x8c66384eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c663850; /* lds.l @r15+,PR */
L8c663850: *stop=0x8c663850u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c663852; /* mov.l @r15+,r11 */
L8c663852: *stop=0x8c663852u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c663854; /* mov.l @r15+,r12 */
L8c663854: *stop=0x8c663854u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c663856; /* mov.l @r15+,r13 */
L8c663856: *stop=0x8c663856u;if(*pr!=entry_pr)return -3;*stop=0x8c663858u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c663540(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c663540;
L8c663540: *stop=0x8c663540u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c663542; /* mov.l r14,@-r15 */
L8c663542: *stop=0x8c663542u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c663544; /* mov.l r13,@-r15 */
L8c663544: *stop=0x8c663544u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c663546; /* sts.l PR,@-r15 */
L8c663546: *stop=0x8c663546u;r[15]+=4294967288u;goto L8c663548; /* add #-0x8,r15 */
L8c663548: *stop=0x8c663548u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663640u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66354a; /* mov.l 0x8c663640,r3 */
L8c66354a: *stop=0x8c66354au;r[13]=r[4];goto L8c66354c; /* mov r4,r13 */
L8c66354c: *stop=0x8c66354cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66354e; /* mov.l @r3,r0 */
L8c66354e: *stop=0x8c66354eu;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c663550; /* tst #0x4,r0 */
L8c663550: take=(*sr&1u);*stop=0x8c663552u;r[14]=r[5];if(take)goto L8c66357c;goto L8c663554; /* bt/s 0x8c66357c */
L8c663554: *stop=0x8c663554u;r[5]=r[15];goto L8c663556; /* mov r15,r5 */
L8c663556: *stop=0x8c663556u;r[5]+=4u;goto L8c663558; /* add #0x4,r5 */
L8c663558: target=0x8c663c86u;*pr=0x8c66355cu;*stop=0x8c66355au;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66355c; /* bsr 0x8c663c86 */
L8c66355c: *stop=0x8c66355cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66355e; /* mov.l @r15,r2 */
L8c66355e: *stop=0x8c66355eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663560; /* mov.l @(0x4,r15),r3 */
L8c663560: *stop=0x8c663560u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c663562; /* cmp/ge r3,r2 */
L8c663562: if((*sr&1u))goto L8c66356c;goto L8c663564; /* bt 0x8c66356c */
L8c663564: *stop=0x8c663564u;r[5]=r[13];goto L8c663566; /* mov r13,r5 */
L8c663566: *stop=0x8c663566u;r[6]=r[14];goto L8c663568; /* mov r14,r6 */
L8c663568: *stop=0x8c66356au;r[4]=1u;goto L8c663572; /* bra 0x8c663572 */
L8c66356c: *stop=0x8c66356cu;r[4]=0u;goto L8c66356e; /* mov #0x0,r4 */
L8c66356e: *stop=0x8c66356eu;r[5]=r[13];goto L8c663570; /* mov r13,r5 */
L8c663570: *stop=0x8c663570u;r[6]=r[14];goto L8c663572; /* mov r14,r6 */
L8c663572: *stop=0x8c663572u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663644u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663574; /* mov.l 0x8c663644,r2 */
L8c663574: target=r[2];*pr=0x8c663578u;*stop=0x8c663576u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663578; /* jsr @r2 */
L8c663578: *stop=0x8c66357au;r[4]=r[0];goto L8c663588; /* bra 0x8c663588 */
L8c66357c: *stop=0x8c66357cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663644u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66357e; /* mov.l 0x8c663644,r2 */
L8c66357e: *stop=0x8c66357eu;r[5]=r[13];goto L8c663580; /* mov r13,r5 */
L8c663580: *stop=0x8c663580u;r[6]=r[14];goto L8c663582; /* mov r14,r6 */
L8c663582: target=r[2];*pr=0x8c663586u;*stop=0x8c663584u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c663586; /* jsr @r2 */
L8c663586: *stop=0x8c663586u;r[4]=r[0];goto L8c663588; /* mov r0,r4 */
L8c663588: *stop=0x8c663588u;r[0]=r[4];goto L8c66358a; /* mov r4,r0 */
L8c66358a: *stop=0x8c66358au;r[15]+=8u;goto L8c66358c; /* add #0x8,r15 */
L8c66358c: *stop=0x8c66358cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66358e; /* lds.l @r15+,PR */
L8c66358e: *stop=0x8c66358eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c663590; /* mov.l @r15+,r13 */
L8c663590: *stop=0x8c663590u;if(*pr!=entry_pr)return -3;*stop=0x8c663592u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c6524d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6524d2;
L8c6524d2: *stop=0x8c6524d2u;r[15]+=4294967288u;goto L8c6524d4; /* add #-0x8,r15 */
L8c6524d4: *stop=0x8c6524d4u;r[3]=r[4];goto L8c6524d6; /* mov r4,r3 */
L8c6524d6: *stop=0x8c6524d6u;r[3]<<=2;goto L8c6524d8; /* shll2 r3 */
L8c6524d8: *stop=0x8c6524d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65250cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6524da; /* mov.l 0x8c65250c,r0 */
L8c6524da: *stop=0x8c6524dau;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c6524dc; /* shll r3 */
L8c6524dc: *stop=0x8c6524dcu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c6524de; /* mov.l r4,@r15 */
L8c6524de: *stop=0x8c6524deu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6524e0; /* mov.l r3,@(0x4,r15) */
L8c6524e0: *stop=0x8c6524e0u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6524e2; /* mov.l @(r0,r3),r2 */
L8c6524e2: *stop=0x8c6524e2u;r[4]=r[0];goto L8c6524e4; /* mov r0,r4 */
L8c6524e4: *stop=0x8c6524e4u;r[4]+=r[3];goto L8c6524e6; /* add r3,r4 */
L8c6524e6: *stop=0x8c6524e6u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6524e8; /* mov.l @(0x4,r4),r4 */
L8c6524e8: target=r[2];*stop=0x8c6524eau;r[15]+=8u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c654286(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c654286;
L8c654286: *stop=0x8c654286u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c654288; /* mov.l r14,@-r15 */
L8c654288: *stop=0x8c654288u;r[14]=r[4];goto L8c65428a; /* mov r4,r14 */
L8c65428a: *stop=0x8c65428au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65428c; /* sts.l PR,@-r15 */
L8c65428c: *stop=0x8c65428cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65428e; /* mov.l @r14,r0 */
L8c65428e: *stop=0x8c65428eu;r[0]=r[0]&65535u;goto L8c654290; /* extu.w r0,r0 */
L8c654290: *stop=0x8c654290u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c654292; /* cmp/eq #0x3,r0 */
L8c654292: if(!(*sr&1u))goto L8c65429c;goto L8c654294; /* bf 0x8c65429c */
L8c654294: target=0x8c65441cu;*pr=0x8c654298u;*stop=0x8c654296u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c654298; /* bsr 0x8c65441c */
L8c654298: *stop=0x8c65429au;goto L8c6542a6; /* bra 0x8c6542a6 */
L8c65429c: *stop=0x8c65429cu;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65429e; /* mov.l @(0x18,r14),r0 */
L8c65429e: *stop=0x8c65429eu;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8c6542a0; /* tst #0x8,r0 */
L8c6542a0: if((*sr&1u))goto L8c6542aa;goto L8c6542a2; /* bt 0x8c6542aa */
L8c6542a2: target=0x8c654334u;*pr=0x8c6542a6u;*stop=0x8c6542a4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6542a6; /* bsr 0x8c654334 */
L8c6542a6: *stop=0x8c6542a8u;r[4]=r[0];goto L8c6542bc; /* bra 0x8c6542bc */
L8c6542aa: *stop=0x8c6542aau;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c6542ac; /* tst #0x20,r0 */
L8c6542ac: if((*sr&1u))goto L8c6542b6;goto L8c6542ae; /* bt 0x8c6542b6 */
L8c6542ae: target=0x8c654392u;*pr=0x8c6542b2u;*stop=0x8c6542b0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6542b2; /* bsr 0x8c654392 */
L8c6542b2: *stop=0x8c6542b4u;goto L8c6542ba; /* bra 0x8c6542ba */
L8c6542b6: target=0x8c6542e0u;*pr=0x8c6542bau;*stop=0x8c6542b8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6542ba; /* bsr 0x8c6542e0 */
L8c6542ba: *stop=0x8c6542bau;r[4]=r[0];goto L8c6542bc; /* mov r0,r4 */
L8c6542bc: *stop=0x8c6542bcu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[4],4);if(io_status)return io_status;goto L8c6542be; /* mov.l r4,@(0x14,r14) */
L8c6542be: *stop=0x8c6542beu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6542c0; /* lds.l @r15+,PR */
L8c6542c0: *stop=0x8c6542c0u;if(*pr!=entry_pr)return -3;*stop=0x8c6542c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c663c86(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c663c86;
L8c663c86: *stop=0x8c663c86u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c663c88; /* mov.l r14,@-r15 */
L8c663c88: *stop=0x8c663c88u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c663c8a; /* mov.l r13,@-r15 */
L8c663c8a: *stop=0x8c663c8au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c663c8c; /* mov.l r12,@-r15 */
L8c663c8c: *stop=0x8c663c8cu;r[15]+=4294967292u;goto L8c663c8e; /* add #-0x4,r15 */
L8c663c8e: *stop=0x8c663c8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663da0u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c663c90; /* mov.l 0x8c663da0,r12 */
L8c663c90: *stop=0x8c663c90u;r[14]=0u;goto L8c663c92; /* mov #0x0,r14 */
L8c663c92: *stop=0x8c663c92u;if(!valid_access(r[12]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c663c94; /* mov.l @(0x8,r12),r6 */
L8c663c94: *stop=0x8c663c94u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c663c96; /* tst r6,r6 */
L8c663c96: take=(*sr&1u);*stop=0x8c663c98u;r[7]=r[14];if(take)goto L8c663ca8;goto L8c663c9a; /* bt/s 0x8c663ca8 */
L8c663c9a: *stop=0x8c663c9au;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663c9c; /* mov.l @(0x10,r6),r2 */
L8c663c9c: *stop=0x8c663c9cu;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[7]);goto L8c663c9e; /* cmp/gt r7,r2 */
L8c663c9e: if(!(*sr&1u))goto L8c663ca2;goto L8c663ca0; /* bf 0x8c663ca2 */
L8c663ca0: *stop=0x8c663ca0u;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c663ca2; /* mov.l @(0x10,r6),r7 */
L8c663ca2: *stop=0x8c663ca2u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c663ca4; /* mov.l @(0x8,r6),r6 */
L8c663ca4: *stop=0x8c663ca4u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c663ca6; /* tst r6,r6 */
L8c663ca6: if(!(*sr&1u))goto L8c663c9a;goto L8c663ca8; /* bf 0x8c663c9a */
L8c663ca8: *stop=0x8c663ca8u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[6]=loaded;}goto L8c663caa; /* mov.l @r12,r6 */
L8c663caa: *stop=0x8c663caau;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c663cac; /* tst r6,r6 */
L8c663cac: take=(*sr&1u);*stop=0x8c663caeu;r[13]=4u;if(take)goto L8c663cc6;goto L8c663cb0; /* bt/s 0x8c663cc6 */
L8c663cb0: *stop=0x8c663cb0u;if(!valid_access(r[6],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[2]=loaded;}goto L8c663cb2; /* mov.w @r6,r2 */
L8c663cb2: *stop=0x8c663cb2u;r[2]=r[2]&65535u;goto L8c663cb4; /* extu.w r2,r2 */
L8c663cb4: *stop=0x8c663cb4u;*sr=(*sr&~1u)|!(r[13]&r[2]);goto L8c663cb6; /* tst r13,r2 */
L8c663cb6: if(!(*sr&1u))goto L8c663cc0;goto L8c663cb8; /* bf 0x8c663cc0 */
L8c663cb8: *stop=0x8c663cb8u;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c663cba; /* mov.l @(0x10,r6),r1 */
L8c663cba: *stop=0x8c663cbau;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[7]);goto L8c663cbc; /* cmp/gt r7,r1 */
L8c663cbc: if(!(*sr&1u))goto L8c663cc0;goto L8c663cbe; /* bf 0x8c663cc0 */
L8c663cbe: *stop=0x8c663cbeu;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c663cc0; /* mov.l @(0x10,r6),r7 */
L8c663cc0: *stop=0x8c663cc0u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c663cc2; /* mov.l @(0x8,r6),r6 */
L8c663cc2: *stop=0x8c663cc2u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c663cc4; /* tst r6,r6 */
L8c663cc4: if(!(*sr&1u))goto L8c663cb0;goto L8c663cc6; /* bf 0x8c663cb0 */
L8c663cc6: *stop=0x8c663cc6u;r[6]=r[12];goto L8c663cc8; /* mov r12,r6 */
L8c663cc8: *stop=0x8c663cc8u;r[6]+=20u;goto L8c663cca; /* add #0x14,r6 */
L8c663cca: *stop=0x8c663ccau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c663ccc; /* mov.l r6,@r15 */
L8c663ccc: *stop=0x8c663cccu;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c663cce; /* mov.l @(0x8,r6),r6 */
L8c663cce: *stop=0x8c663cceu;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c663cd0; /* tst r6,r6 */
L8c663cd0: if((*sr&1u))goto L8c663ce0;goto L8c663cd2; /* bt 0x8c663ce0 */
L8c663cd2: *stop=0x8c663cd2u;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663cd4; /* mov.l @(0x10,r6),r2 */
L8c663cd4: *stop=0x8c663cd4u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[14]);goto L8c663cd6; /* cmp/gt r14,r2 */
L8c663cd6: if(!(*sr&1u))goto L8c663cda;goto L8c663cd8; /* bf 0x8c663cda */
L8c663cd8: *stop=0x8c663cd8u;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c663cda; /* mov.l @(0x10,r6),r14 */
L8c663cda: *stop=0x8c663cdau;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c663cdc; /* mov.l @(0x8,r6),r6 */
L8c663cdc: *stop=0x8c663cdcu;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c663cde; /* tst r6,r6 */
L8c663cde: if(!(*sr&1u))goto L8c663cd2;goto L8c663ce0; /* bf 0x8c663cd2 */
L8c663ce0: *stop=0x8c663ce0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c663ce2; /* mov.l @r15,r6 */
L8c663ce2: *stop=0x8c663ce2u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[6]=loaded;}goto L8c663ce4; /* mov.l @r6,r6 */
L8c663ce4: *stop=0x8c663ce4u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c663ce6; /* tst r6,r6 */
L8c663ce6: if((*sr&1u))goto L8c663cfe;goto L8c663ce8; /* bt 0x8c663cfe */
L8c663ce8: *stop=0x8c663ce8u;if(!valid_access(r[6],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[2]=loaded;}goto L8c663cea; /* mov.w @r6,r2 */
L8c663cea: *stop=0x8c663ceau;r[2]=r[2]&65535u;goto L8c663cec; /* extu.w r2,r2 */
L8c663cec: *stop=0x8c663cecu;*sr=(*sr&~1u)|!(r[13]&r[2]);goto L8c663cee; /* tst r13,r2 */
L8c663cee: if(!(*sr&1u))goto L8c663cf8;goto L8c663cf0; /* bf 0x8c663cf8 */
L8c663cf0: *stop=0x8c663cf0u;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c663cf2; /* mov.l @(0x10,r6),r1 */
L8c663cf2: *stop=0x8c663cf2u;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[14]);goto L8c663cf4; /* cmp/gt r14,r1 */
L8c663cf4: if(!(*sr&1u))goto L8c663cf8;goto L8c663cf6; /* bf 0x8c663cf8 */
L8c663cf6: *stop=0x8c663cf6u;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c663cf8; /* mov.l @(0x10,r6),r14 */
L8c663cf8: *stop=0x8c663cf8u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c663cfa; /* mov.l @(0x8,r6),r6 */
L8c663cfa: *stop=0x8c663cfau;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c663cfc; /* tst r6,r6 */
L8c663cfc: if(!(*sr&1u))goto L8c663ce8;goto L8c663cfe; /* bf 0x8c663ce8 */
L8c663cfe: *stop=0x8c663cfeu;r[0]=0u;goto L8c663d00; /* mov #0x0,r0 */
L8c663d00: *stop=0x8c663d00u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[7],4);if(io_status)return io_status;goto L8c663d02; /* mov.l r7,@r4 */
L8c663d02: *stop=0x8c663d02u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[14],4);if(io_status)return io_status;goto L8c663d04; /* mov.l r14,@r5 */
L8c663d04: *stop=0x8c663d04u;r[15]+=4u;goto L8c663d06; /* add #0x4,r15 */
L8c663d06: *stop=0x8c663d06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c663d08; /* mov.l @r15+,r12 */
L8c663d08: *stop=0x8c663d08u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c663d0a; /* mov.l @r15+,r13 */
L8c663d0a: *stop=0x8c663d0au;if(*pr!=entry_pr)return -3;*stop=0x8c663d0cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c66452a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c66452a;
L8c66452a: *stop=0x8c66452au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c66452c; /* mov.l r14,@-r15 */
L8c66452c: *stop=0x8c66452cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c66452e; /* mov.l r13,@-r15 */
L8c66452e: *stop=0x8c66452eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c664530; /* mov.l r12,@-r15 */
L8c664530: *stop=0x8c664530u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c664532; /* mov.l r11,@-r15 */
L8c664532: *stop=0x8c664532u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c664534; /* mov.l r10,@-r15 */
L8c664534: *stop=0x8c664534u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c664536; /* mov.l r9,@-r15 */
L8c664536: *stop=0x8c664536u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c664538; /* mov.l r8,@-r15 */
L8c664538: *stop=0x8c664538u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66453a; /* sts.l PR,@-r15 */
L8c66453a: *stop=0x8c66453au;r[15]+=4294967292u;goto L8c66453c; /* add #-0x4,r15 */
L8c66453c: *stop=0x8c66453cu;r[0]=r[5];goto L8c66453e; /* mov r5,r0 */
L8c66453e: *stop=0x8c66453eu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c664540; /* cmp/pz r0 */
L8c664540: *stop=0x8c664540u;r[8]=r[4];goto L8c664542; /* mov r4,r8 */
L8c664542: *stop=0x8c664542u;r[12]=r[5];goto L8c664544; /* mov r5,r12 */
L8c664544: take=!(*sr&1u);*stop=0x8c664546u;r[9]=r[6];if(take)goto L8c66454c;goto L8c664548; /* bf/s 0x8c66454c */
L8c664548: *stop=0x8c66454au;r[0]&=0x1fu;goto L8c664556; /* bra 0x8c664556 */
L8c66454c: *stop=0x8c66454cu;r[0]=~r[0];goto L8c66454e; /* not r0,r0 */
L8c66454e: *stop=0x8c66454eu;r[0]+=1u;goto L8c664550; /* add #0x1,r0 */
L8c664550: *stop=0x8c664550u;r[0]&=0x1fu;goto L8c664552; /* and #0x1f,r0 */
L8c664552: *stop=0x8c664552u;r[0]=~r[0];goto L8c664554; /* not r0,r0 */
L8c664554: *stop=0x8c664554u;r[0]+=1u;goto L8c664556; /* add #0x1,r0 */
L8c664556: *stop=0x8c664556u;r[4]=r[0];goto L8c664558; /* mov r0,r4 */
L8c664558: *stop=0x8c664558u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c66455a; /* tst r4,r4 */
L8c66455a: take=(*sr&1u);*stop=0x8c66455cu;r[5]=0u;if(take)goto L8c664564;goto L8c66455e; /* bt/s 0x8c664564 */
L8c66455e: *stop=0x8c66455eu;r[3]=32u;goto L8c664560; /* mov #0x20,r3 */
L8c664560: *stop=0x8c664560u;r[3]-=r[4];goto L8c664562; /* sub r4,r3 */
L8c664562: *stop=0x8c664562u;r[12]+=r[3];goto L8c664564; /* add r3,r12 */
L8c664564: *stop=0x8c664564u;r[10]=(uint32_t)(int32_t)(int16_t)r[8];goto L8c664566; /* exts.w r8,r10 */
L8c664566: *stop=0x8c664566u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664620u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c664568; /* mov.l 0x8c664620,r11 */
L8c664568: *stop=0x8c664568u;r[3]=r[10];goto L8c66456a; /* mov r10,r3 */
L8c66456a: *stop=0x8c66456au;r[10]<<=2;goto L8c66456c; /* shll2 r10 */
L8c66456c: *stop=0x8c66456cu;r[10]+=r[3];goto L8c66456e; /* add r3,r10 */
L8c66456e: *stop=0x8c66456eu;r[10]<<=2;goto L8c664570; /* shll2 r10 */
L8c664570: *stop=0x8c664570u;r[4]=r[10];goto L8c664572; /* mov r10,r4 */
L8c664572: *stop=0x8c664572u;r[4]+=r[11];goto L8c664574; /* add r11,r4 */
L8c664574: *stop=0x8c664574u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c664576; /* mov.l @(0x8,r4),r4 */
L8c664576: *stop=0x8c664576u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c664578; /* tst r4,r4 */
L8c664578: take=(*sr&1u);*stop=0x8c66457au;r[14]=r[5];if(take)goto L8c66458e;goto L8c66457c; /* bt/s 0x8c66458e */
L8c66457c: *stop=0x8c66457cu;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66457e; /* mov.l @(0x10,r4),r2 */
L8c66457e: *stop=0x8c66457eu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[12]);goto L8c664580; /* cmp/ge r12,r2 */
L8c664580: if(!(*sr&1u))goto L8c664588;goto L8c664582; /* bf 0x8c664588 */
L8c664582: *stop=0x8c664582u;r[14]=r[4];goto L8c664584; /* mov r4,r14 */
L8c664584: *stop=0x8c664586u;r[4]=r[5];goto L8c66458a; /* bra 0x8c66458a */
L8c664588: *stop=0x8c664588u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c66458a; /* mov.l @(0x8,r4),r4 */
L8c66458a: *stop=0x8c66458au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c66458c; /* tst r4,r4 */
L8c66458c: if(!(*sr&1u))goto L8c66457c;goto L8c66458e; /* bf 0x8c66457c */
L8c66458e: *stop=0x8c66458eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c664590; /* tst r14,r14 */
L8c664590: if((*sr&1u))goto L8c6645e4;goto L8c664592; /* bt 0x8c6645e4 */
L8c664592: *stop=0x8c664592u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664594; /* mov.l @(0x10,r14),r3 */
L8c664594: *stop=0x8c664594u;*sr=(*sr&~1u)|(r[12]==r[3]);goto L8c664596; /* cmp/eq r12,r3 */
L8c664596: if(!(*sr&1u))goto L8c6645da;goto L8c664598; /* bf 0x8c6645da */
L8c664598: *stop=0x8c664598u;r[13]=r[10];goto L8c66459a; /* mov r10,r13 */
L8c66459a: *stop=0x8c66459au;r[13]+=r[11];goto L8c66459c; /* add r11,r13 */
L8c66459c: *stop=0x8c66459cu;r[5]=r[13];goto L8c66459e; /* mov r13,r5 */
L8c66459e: *stop=0x8c66459eu;r[4]=r[13];goto L8c6645a0; /* mov r13,r4 */
L8c6645a0: *stop=0x8c6645a0u;r[5]+=12u;goto L8c6645a2; /* add #0xc,r5 */
L8c6645a2: *stop=0x8c6645a2u;r[6]=r[14];goto L8c6645a4; /* mov r14,r6 */
L8c6645a4: target=0x8c664b52u;*pr=0x8c6645a8u;*stop=0x8c6645a6u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6645a8; /* bsr 0x8c664b52 */
L8c6645a8: *stop=0x8c6645a8u;r[5]=r[13];goto L8c6645aa; /* mov r13,r5 */
L8c6645aa: *stop=0x8c6645aau;r[5]+=4u;goto L8c6645ac; /* add #0x4,r5 */
L8c6645ac: *stop=0x8c6645acu;r[6]=r[14];goto L8c6645ae; /* mov r14,r6 */
L8c6645ae: target=0x8c664b12u;*pr=0x8c6645b2u;*stop=0x8c6645b0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6645b2; /* bsr 0x8c664b12 */
L8c6645b2: *stop=0x8c6645b2u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6645b4; /* mov.l @(0xc,r14),r3 */
L8c6645b4: *stop=0x8c6645b4u;r[8]=(uint32_t)(int32_t)(int16_t)r[8];goto L8c6645b6; /* exts.w r8,r8 */
L8c6645b6: *stop=0x8c6645b6u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c6645b8; /* tst r8,r8 */
L8c6645b8: *stop=0x8c6645b8u;r[2]=1u;goto L8c6645ba; /* mov #0x1,r2 */
L8c6645ba: *stop=0x8c6645bau;if(!valid_access(r[9]+28u,4u))return -3;wr(m,bus,&io_status,r[9]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c6645bc; /* mov.l r3,@(0x1c,r9) */
L8c6645bc: *stop=0x8c6645bcu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[9],4);if(io_status)return io_status;goto L8c6645be; /* mov.l r9,@(0x14,r14) */
L8c6645be: take=!(*sr&1u);*stop=0x8c6645c0u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;if(take)goto L8c6645c8;goto L8c6645c2; /* bf/s 0x8c6645c8 */
L8c6645c2: *stop=0x8c6645c2u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[0]=loaded;}goto L8c6645c4; /* mov.w @r14,r0 */
L8c6645c4: *stop=0x8c6645c6u;r[0]|=0x20u;goto L8c6645cc; /* bra 0x8c6645cc */
L8c6645c8: *stop=0x8c6645c8u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[0]=loaded;}goto L8c6645ca; /* mov.w @r14,r0 */
L8c6645ca: *stop=0x8c6645cau;r[0]|=0x40u;goto L8c6645cc; /* or #0x40,r0 */
L8c6645cc: *stop=0x8c6645ccu;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[0],2);if(io_status)return io_status;goto L8c6645ce; /* mov.w r0,@r14 */
L8c6645ce: *stop=0x8c6645ceu;r[10]+=r[11];goto L8c6645d0; /* add r11,r10 */
L8c6645d0: *stop=0x8c6645d0u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[0]=loaded;}goto L8c6645d2; /* mov.w @r14,r0 */
L8c6645d2: *stop=0x8c6645d2u;r[0]|=0x4u;goto L8c6645d4; /* or #0x4,r0 */
L8c6645d4: *stop=0x8c6645d4u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[0],2);if(io_status)return io_status;goto L8c6645d6; /* mov.w r0,@r14 */
L8c6645d6: *stop=0x8c6645d8u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664650; /* bra 0x8c664650 */
L8c6645da: target=0x8c664ac0u;*pr=0x8c6645deu;*stop=0x8c6645dcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6645de; /* bsr 0x8c664ac0 */
L8c6645de: *stop=0x8c6645deu;r[13]=r[0];goto L8c6645e0; /* mov r0,r13 */
L8c6645e0: *stop=0x8c6645e0u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c6645e2; /* tst r13,r13 */
L8c6645e2: if(!(*sr&1u))goto L8c6645e8;goto L8c6645e4; /* bf 0x8c6645e8 */
L8c6645e4: *stop=0x8c6645e6u;r[0]=3u;goto L8c664658; /* bra 0x8c664658 */
L8c6645e8: *stop=0x8c6645e8u;r[5]=r[10];goto L8c6645ea; /* mov r10,r5 */
L8c6645ea: *stop=0x8c6645eau;r[5]+=r[11];goto L8c6645ec; /* add r11,r5 */
L8c6645ec: *stop=0x8c6645ecu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6645ee; /* mov.l @(0xc,r14),r3 */
L8c6645ee: *stop=0x8c6645eeu;r[6]=r[13];goto L8c6645f0; /* mov r13,r6 */
L8c6645f0: *stop=0x8c6645f0u;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6645f2; /* mov.l r3,@(0xc,r13) */
L8c6645f2: *stop=0x8c6645f2u;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[12],4);if(io_status)return io_status;goto L8c6645f4; /* mov.l r12,@(0x10,r13) */
L8c6645f4: *stop=0x8c6645f4u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6645f6; /* mov.l @(0x10,r14),r4 */
L8c6645f6: *stop=0x8c6645f6u;r[4]-=r[12];goto L8c6645f8; /* sub r12,r4 */
L8c6645f8: *stop=0x8c6645f8u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[4],4);if(io_status)return io_status;goto L8c6645fa; /* mov.l r4,@(0x10,r14) */
L8c6645fa: *stop=0x8c6645fau;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6645fc; /* mov.l @(0xc,r14),r2 */
L8c6645fc: *stop=0x8c6645fcu;r[2]+=r[12];goto L8c6645fe; /* add r12,r2 */
L8c6645fe: *stop=0x8c6645feu;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c664600; /* mov.l r2,@(0xc,r14) */
L8c664600: *stop=0x8c664600u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c664602; /* mov.l r5,@r15 */
L8c664602: *stop=0x8c664602u;r[5]+=4u;goto L8c664604; /* add #0x4,r5 */
L8c664604: target=0x8c664b12u;*pr=0x8c664608u;*stop=0x8c664606u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c664608; /* bsr 0x8c664b12 */
L8c664608: *stop=0x8c664608u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66460a; /* mov.l @(0xc,r13),r2 */
L8c66460a: *stop=0x8c66460au;r[8]=(uint32_t)(int32_t)(int16_t)r[8];goto L8c66460c; /* exts.w r8,r8 */
L8c66460c: *stop=0x8c66460cu;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c66460e; /* tst r8,r8 */
L8c66460e: *stop=0x8c66460eu;if(!valid_access(r[9]+28u,4u))return -3;wr(m,bus,&io_status,r[9]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c664610; /* mov.l r2,@(0x1c,r9) */
L8c664610: take=!(*sr&1u);*stop=0x8c664612u;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[9],4);if(io_status)return io_status;if(take)goto L8c664640;goto L8c664614; /* bf/s 0x8c664640 */
L8c664614: *stop=0x8c664614u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c664616; /* mov.w @r13,r0 */
L8c664616: *stop=0x8c664618u;r[0]|=0x20u;goto L8c664644; /* bra 0x8c664644 */
L8c664640: *stop=0x8c664640u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c664642; /* mov.w @r13,r0 */
L8c664642: *stop=0x8c664642u;r[0]|=0x40u;goto L8c664644; /* or #0x40,r0 */
L8c664644: *stop=0x8c664644u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c664646; /* mov.w r0,@r13 */
L8c664646: *stop=0x8c664646u;r[10]+=r[11];goto L8c664648; /* add r11,r10 */
L8c664648: *stop=0x8c664648u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c66464a; /* mov.w @r13,r0 */
L8c66464a: *stop=0x8c66464au;r[0]|=0x4u;goto L8c66464c; /* or #0x4,r0 */
L8c66464c: *stop=0x8c66464cu;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c66464e; /* mov.w r0,@r13 */
L8c66464e: *stop=0x8c66464eu;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664650; /* mov.l @(0x10,r13),r3 */
L8c664650: *stop=0x8c664650u;if(!valid_access(r[10]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664652; /* mov.l @(0x10,r10),r2 */
L8c664652: *stop=0x8c664652u;r[0]=0u;goto L8c664654; /* mov #0x0,r0 */
L8c664654: *stop=0x8c664654u;r[2]-=r[3];goto L8c664656; /* sub r3,r2 */
L8c664656: *stop=0x8c664656u;if(!valid_access(r[10]+16u,4u))return -3;wr(m,bus,&io_status,r[10]+0x10u,r[2],4);if(io_status)return io_status;goto L8c664658; /* mov.l r2,@(0x10,r10) */
L8c664658: *stop=0x8c664658u;r[15]+=4u;goto L8c66465a; /* add #0x4,r15 */
L8c66465a: *stop=0x8c66465au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66465c; /* lds.l @r15+,PR */
L8c66465c: *stop=0x8c66465cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c66465e; /* mov.l @r15+,r8 */
L8c66465e: *stop=0x8c66465eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c664660; /* mov.l @r15+,r9 */
L8c664660: *stop=0x8c664660u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c664662; /* mov.l @r15+,r10 */
L8c664662: *stop=0x8c664662u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c664664; /* mov.l @r15+,r11 */
L8c664664: *stop=0x8c664664u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c664666; /* mov.l @r15+,r12 */
L8c664666: *stop=0x8c664666u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c664668; /* mov.l @r15+,r13 */
L8c664668: *stop=0x8c664668u;if(*pr!=entry_pr)return -3;*stop=0x8c66466au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c66466c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c66466c;
L8c66466c: *stop=0x8c66466cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c66466e; /* mov.l r14,@-r15 */
L8c66466e: *stop=0x8c66466eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c664670; /* mov.l r13,@-r15 */
L8c664670: *stop=0x8c664670u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c664672; /* mov.l r12,@-r15 */
L8c664672: *stop=0x8c664672u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c664674; /* mov.l r11,@-r15 */
L8c664674: *stop=0x8c664674u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c664676; /* mov.l r10,@-r15 */
L8c664676: *stop=0x8c664676u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c664678; /* mov.l r9,@-r15 */
L8c664678: *stop=0x8c664678u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c66467a; /* mov.l r8,@-r15 */
L8c66467a: *stop=0x8c66467au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66467c; /* sts.l PR,@-r15 */
L8c66467c: *stop=0x8c66467cu;r[15]+=4294967284u;goto L8c66467e; /* add #-0xc,r15 */
L8c66467e: *stop=0x8c66467eu;r[0]=r[4];goto L8c664680; /* mov r4,r0 */
L8c664680: *stop=0x8c664680u;r[11]=r[5];goto L8c664682; /* mov r5,r11 */
L8c664682: *stop=0x8c664682u;if(!valid_access(r[15]+8u,2u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],2);if(io_status)return io_status;goto L8c664684; /* mov.w r0,@(0x8,r15) */
L8c664684: *stop=0x8c664684u;r[0]=r[11];goto L8c664686; /* mov r11,r0 */
L8c664686: *stop=0x8c664686u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c664688; /* cmp/pz r0 */
L8c664688: *stop=0x8c664688u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c66468a; /* mov.l r6,@(0x4,r15) */
L8c66468a: *stop=0x8c66468au;r[13]=0u;goto L8c66468c; /* mov #0x0,r13 */
L8c66468c: take=!(*sr&1u);*stop=0x8c66468eu;r[6]=r[13];if(take)goto L8c664694;goto L8c664690; /* bf/s 0x8c664694 */
L8c664690: *stop=0x8c664692u;r[0]&=0x1fu;goto L8c66469e; /* bra 0x8c66469e */
L8c664694: *stop=0x8c664694u;r[0]=~r[0];goto L8c664696; /* not r0,r0 */
L8c664696: *stop=0x8c664696u;r[0]+=1u;goto L8c664698; /* add #0x1,r0 */
L8c664698: *stop=0x8c664698u;r[0]&=0x1fu;goto L8c66469a; /* and #0x1f,r0 */
L8c66469a: *stop=0x8c66469au;r[0]=~r[0];goto L8c66469c; /* not r0,r0 */
L8c66469c: *stop=0x8c66469cu;r[0]+=1u;goto L8c66469e; /* add #0x1,r0 */
L8c66469e: *stop=0x8c66469eu;r[4]=r[0];goto L8c6646a0; /* mov r0,r4 */
L8c6646a0: *stop=0x8c6646a0u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6646a2; /* tst r4,r4 */
L8c6646a2: take=(*sr&1u);*stop=0x8c6646a4u;r[8]=r[13];if(take)goto L8c6646ac;goto L8c6646a6; /* bt/s 0x8c6646ac */
L8c6646a6: *stop=0x8c6646a6u;r[3]=32u;goto L8c6646a8; /* mov #0x20,r3 */
L8c6646a8: *stop=0x8c6646a8u;r[3]-=r[4];goto L8c6646aa; /* sub r4,r3 */
L8c6646aa: *stop=0x8c6646aau;r[11]+=r[3];goto L8c6646ac; /* add r3,r11 */
L8c6646ac: *stop=0x8c6646acu;if(!valid_access(r[15]+8u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6646ae; /* mov.w @(0x8,r15),r0 */
L8c6646ae: *stop=0x8c6646aeu;r[14]=r[13];goto L8c6646b0; /* mov r13,r14 */
L8c6646b0: *stop=0x8c6646b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664724u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c6646b2; /* mov.l 0x8c664724,r10 */
L8c6646b2: *stop=0x8c6646b2u;r[3]=r[0];goto L8c6646b4; /* mov r0,r3 */
L8c6646b4: *stop=0x8c6646b4u;r[0]<<=2;goto L8c6646b6; /* shll2 r0 */
L8c6646b6: *stop=0x8c6646b6u;r[0]+=r[3];goto L8c6646b8; /* add r3,r0 */
L8c6646b8: *stop=0x8c6646b8u;r[0]<<=2;goto L8c6646ba; /* shll2 r0 */
L8c6646ba: *stop=0x8c6646bau;r[4]=r[0];goto L8c6646bc; /* mov r0,r4 */
L8c6646bc: *stop=0x8c6646bcu;r[4]+=r[10];goto L8c6646be; /* add r10,r4 */
L8c6646be: *stop=0x8c6646beu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6646c0; /* mov.l @(0x8,r4),r4 */
L8c6646c0: *stop=0x8c6646c0u;r[9]=r[0];goto L8c6646c2; /* mov r0,r9 */
L8c6646c2: *stop=0x8c6646c2u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6646c4; /* tst r4,r4 */
L8c6646c4: take=(*sr&1u);*stop=0x8c6646c6u;r[12]=1u;if(take)goto L8c664746;goto L8c6646c8; /* bt/s 0x8c664746 */
L8c6646c8: *stop=0x8c6646c8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c664720u,2);if(io_status)return io_status;r[7]=loaded;}goto L8c6646ca; /* mov.w 0x8c664720,r7 */
L8c6646ca: *stop=0x8c6646cau;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6646cc; /* mov.l @(0x10,r4),r2 */
L8c6646cc: *stop=0x8c6646ccu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[11]);goto L8c6646ce; /* cmp/ge r11,r2 */
L8c6646ce: if(!(*sr&1u))goto L8c664740;goto L8c6646d0; /* bf 0x8c664740 */
L8c6646d0: *stop=0x8c6646d0u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c6646d2; /* tst r8,r8 */
L8c6646d2: if(!(*sr&1u))goto L8c6646d6;goto L8c6646d4; /* bf 0x8c6646d6 */
L8c6646d4: *stop=0x8c6646d4u;r[8]=r[4];goto L8c6646d6; /* mov r4,r8 */
L8c6646d6: *stop=0x8c6646d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c664728u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6646d8; /* mov.l 0x8c664728,r3 */
L8c6646d8: *stop=0x8c6646d8u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6646da; /* mov.l @(0xc,r4),r1 */
L8c6646da: target=r[3];*pr=0x8c6646deu;*stop=0x8c6646dcu;r[0]=r[7];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6646de; /* jsr @r3 */
L8c6646de: *stop=0x8c6646deu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6646e0; /* tst r0,r0 */
L8c6646e0: if((*sr&1u))goto L8c664712;goto L8c6646e2; /* bt 0x8c664712 */
L8c6646e2: *stop=0x8c6646e2u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6646e4; /* mov.l @(0xc,r4),r5 */
L8c6646e4: *stop=0x8c6646e4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c664722u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6646e6; /* mov.w 0x8c664722,r3 */
L8c6646e6: *stop=0x8c6646e6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c6646e8; /* mov.l r5,@r15 */
L8c6646e8: *stop=0x8c6646e8u;r[5]+=r[7];goto L8c6646ea; /* add r7,r5 */
L8c6646ea: *stop=0x8c6646eau;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6646ec; /* mov.l @(0x10,r4),r2 */
L8c6646ec: *stop=0x8c6646ecu;r[5]&=r[3];goto L8c6646ee; /* and r3,r5 */
L8c6646ee: *stop=0x8c6646eeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6646f0; /* mov.l @r15,r1 */
L8c6646f0: *stop=0x8c6646f0u;r[1]+=r[2];goto L8c6646f2; /* add r2,r1 */
L8c6646f2: *stop=0x8c6646f2u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[1]);goto L8c6646f4; /* cmp/ge r1,r5 */
L8c6646f4: take=(*sr&1u);*stop=0x8c6646f6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(take)goto L8c664714;goto L8c6646f8; /* bt/s 0x8c664714 */
L8c6646f8: *stop=0x8c6646f8u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6646fa; /* mov.l @(0xc,r4),r2 */
L8c6646fa: *stop=0x8c6646fau;r[1]=r[5];goto L8c6646fc; /* mov r5,r1 */
L8c6646fc: *stop=0x8c6646fcu;r[1]-=r[2];goto L8c6646fe; /* sub r2,r1 */
L8c6646fe: *stop=0x8c6646feu;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[11]);goto L8c664700; /* cmp/ge r11,r1 */
L8c664700: if((*sr&1u))goto L8c664714;goto L8c664702; /* bt 0x8c664714 */
L8c664702: *stop=0x8c664702u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c664704; /* mov.l @r15,r2 */
L8c664704: *stop=0x8c664704u;r[2]-=r[5];goto L8c664706; /* sub r5,r2 */
L8c664706: *stop=0x8c664706u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[11]);goto L8c664708; /* cmp/ge r11,r2 */
L8c664708: if(!(*sr&1u))goto L8c66470e;goto L8c66470a; /* bf 0x8c66470e */
L8c66470a: *stop=0x8c66470cu;r[6]=2u;goto L8c664716; /* bra 0x8c664716 */
L8c66470e: *stop=0x8c664710u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c664742; /* bra 0x8c664742 */
L8c664712: *stop=0x8c664712u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c664714; /* mov.l @(0xc,r4),r5 */
L8c664714: *stop=0x8c664714u;r[6]=r[12];goto L8c664716; /* mov r12,r6 */
L8c664716: *stop=0x8c664716u;r[14]=r[4];goto L8c664718; /* mov r4,r14 */
L8c664718: *stop=0x8c66471au;r[4]=r[13];goto L8c664742; /* bra 0x8c664742 */
L8c664740: *stop=0x8c664740u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c664742; /* mov.l @(0x8,r4),r4 */
L8c664742: *stop=0x8c664742u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c664744; /* tst r4,r4 */
L8c664744: if(!(*sr&1u))goto L8c6646ca;goto L8c664746; /* bf 0x8c6646ca */
L8c664746: *stop=0x8c664746u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c664748; /* tst r6,r6 */
L8c664748: if(!(*sr&1u))goto L8c664752;goto L8c66474a; /* bf 0x8c664752 */
L8c66474a: *stop=0x8c66474au;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c66474c; /* tst r8,r8 */
L8c66474c: if((*sr&1u))goto L8c664752;goto L8c66474e; /* bt 0x8c664752 */
L8c66474e: *stop=0x8c66474eu;r[14]=r[8];goto L8c664750; /* mov r8,r14 */
L8c664750: *stop=0x8c664750u;r[6]=r[12];goto L8c664752; /* mov r12,r6 */
L8c664752: *stop=0x8c664752u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c664754; /* tst r14,r14 */
L8c664754: if((*sr&1u))goto L8c6647ea;goto L8c664756; /* bt 0x8c6647ea */
L8c664756: *stop=0x8c664756u;r[0]=r[6];goto L8c664758; /* mov r6,r0 */
L8c664758: *stop=0x8c664758u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c66475a; /* cmp/eq #0x2,r0 */
L8c66475a: if(!(*sr&1u))goto L8c66476e;goto L8c66475c; /* bf 0x8c66476e */
L8c66475c: *stop=0x8c66475cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c66475e; /* mov.l r12,@-r15 */
L8c66475e: *stop=0x8c66475eu;r[6]=r[11];goto L8c664760; /* mov r11,r6 */
L8c664760: *stop=0x8c664760u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c664762; /* mov.l r13,@-r15 */
L8c664762: *stop=0x8c664762u;if(!valid_access(r[15]+16u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x10u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c664764; /* mov.w @(0x10,r15),r0 */
L8c664764: *stop=0x8c664764u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c664766; /* mov.l @(0xc,r15),r7 */
L8c664766: target=0x8c664f72u;*pr=0x8c66476au;*stop=0x8c664768u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66476a; /* bsr 0x8c664f72 */
L8c66476a: *stop=0x8c66476cu;r[15]+=8u;goto L8c664842; /* bra 0x8c664842 */
L8c66476e: *stop=0x8c66476eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664770; /* mov.l @(0x10,r14),r2 */
L8c664770: *stop=0x8c664770u;*sr=(*sr&~1u)|(r[11]==r[2]);goto L8c664772; /* cmp/eq r11,r2 */
L8c664772: if(!(*sr&1u))goto L8c6647e0;goto L8c664774; /* bf 0x8c6647e0 */
L8c664774: *stop=0x8c664774u;r[13]=r[9];goto L8c664776; /* mov r9,r13 */
L8c664776: *stop=0x8c664776u;r[13]+=r[10];goto L8c664778; /* add r10,r13 */
L8c664778: *stop=0x8c664778u;r[5]=r[13];goto L8c66477a; /* mov r13,r5 */
L8c66477a: *stop=0x8c66477au;r[4]=r[13];goto L8c66477c; /* mov r13,r4 */
L8c66477c: *stop=0x8c66477cu;r[5]+=12u;goto L8c66477e; /* add #0xc,r5 */
L8c66477e: *stop=0x8c66477eu;r[6]=r[14];goto L8c664780; /* mov r14,r6 */
L8c664780: target=0x8c664b52u;*pr=0x8c664784u;*stop=0x8c664782u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c664784; /* bsr 0x8c664b52 */
L8c664784: *stop=0x8c664784u;r[5]=r[13];goto L8c664786; /* mov r13,r5 */
L8c664786: *stop=0x8c664786u;r[5]+=4u;goto L8c664788; /* add #0x4,r5 */
L8c664788: *stop=0x8c664788u;r[6]=r[14];goto L8c66478a; /* mov r14,r6 */
L8c66478a: target=0x8c664b12u;*pr=0x8c66478eu;*stop=0x8c66478cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66478e; /* bsr 0x8c664b12 */
L8c66478e: *stop=0x8c66478eu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664790; /* mov.l @(0xc,r14),r3 */
L8c664790: *stop=0x8c664790u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664792; /* mov.l @(0x4,r15),r2 */
L8c664792: *stop=0x8c664792u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c664794; /* mov.l r3,@(0x8,r2) */
L8c664794: *stop=0x8c664794u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664796; /* mov.l @(0x4,r15),r2 */
L8c664796: *stop=0x8c664796u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[2],4);if(io_status)return io_status;goto L8c664798; /* mov.l r2,@(0x14,r14) */
L8c664798: *stop=0x8c664798u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[12],2);if(io_status)return io_status;goto L8c66479a; /* mov.w r12,@r14 */
L8c66479a: *stop=0x8c66479au;if(!valid_access(r[15]+8u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c66479c; /* mov.w @(0x8,r15),r0 */
L8c66479c: *stop=0x8c66479cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c66479e; /* tst r0,r0 */
L8c66479e: if(!(*sr&1u))goto L8c6647aa;goto L8c6647a0; /* bf 0x8c6647aa */
L8c6647a0: *stop=0x8c6647a0u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[2]=loaded;}goto L8c6647a2; /* mov.w @r14,r2 */
L8c6647a2: *stop=0x8c6647a2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6647c0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6647a4; /* mov.w 0x8c6647c0,r3 */
L8c6647a4: *stop=0x8c6647a4u;r[2]|=r[3];goto L8c6647a6; /* or r3,r2 */
L8c6647a6: *stop=0x8c6647a8u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c6647b2; /* bra 0x8c6647b2 */
L8c6647aa: *stop=0x8c6647aau;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[1]=loaded;}goto L8c6647ac; /* mov.w @r14,r1 */
L8c6647ac: *stop=0x8c6647acu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6647c2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6647ae; /* mov.w 0x8c6647c2,r3 */
L8c6647ae: *stop=0x8c6647aeu;r[1]|=r[3];goto L8c6647b0; /* or r3,r1 */
L8c6647b0: *stop=0x8c6647b0u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[1],2);if(io_status)return io_status;goto L8c6647b2; /* mov.w r1,@r14 */
L8c6647b2: *stop=0x8c6647b2u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[0]=loaded;}goto L8c6647b4; /* mov.w @r14,r0 */
L8c6647b4: *stop=0x8c6647b4u;r[9]+=r[10];goto L8c6647b6; /* add r10,r9 */
L8c6647b6: *stop=0x8c6647b6u;r[0]|=0x4u;goto L8c6647b8; /* or #0x4,r0 */
L8c6647b8: *stop=0x8c6647b8u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[0],2);if(io_status)return io_status;goto L8c6647ba; /* mov.w r0,@r14 */
L8c6647ba: *stop=0x8c6647bcu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66483a; /* bra 0x8c66483a */
L8c6647e0: target=0x8c664ac0u;*pr=0x8c6647e4u;*stop=0x8c6647e2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6647e4; /* bsr 0x8c664ac0 */
L8c6647e4: *stop=0x8c6647e4u;r[13]=r[0];goto L8c6647e6; /* mov r0,r13 */
L8c6647e6: *stop=0x8c6647e6u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c6647e8; /* tst r13,r13 */
L8c6647e8: if(!(*sr&1u))goto L8c6647ee;goto L8c6647ea; /* bf 0x8c6647ee */
L8c6647ea: *stop=0x8c6647ecu;r[0]=3u;goto L8c664842; /* bra 0x8c664842 */
L8c6647ee: *stop=0x8c6647eeu;r[5]=r[9];goto L8c6647f0; /* mov r9,r5 */
L8c6647f0: *stop=0x8c6647f0u;r[5]+=r[10];goto L8c6647f2; /* add r10,r5 */
L8c6647f2: *stop=0x8c6647f2u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6647f4; /* mov.l @(0xc,r14),r3 */
L8c6647f4: *stop=0x8c6647f4u;r[6]=r[13];goto L8c6647f6; /* mov r13,r6 */
L8c6647f6: *stop=0x8c6647f6u;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6647f8; /* mov.l r3,@(0xc,r13) */
L8c6647f8: *stop=0x8c6647f8u;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[11],4);if(io_status)return io_status;goto L8c6647fa; /* mov.l r11,@(0x10,r13) */
L8c6647fa: *stop=0x8c6647fau;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6647fc; /* mov.l @(0x10,r14),r4 */
L8c6647fc: *stop=0x8c6647fcu;r[4]-=r[11];goto L8c6647fe; /* sub r11,r4 */
L8c6647fe: *stop=0x8c6647feu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[4],4);if(io_status)return io_status;goto L8c664800; /* mov.l r4,@(0x10,r14) */
L8c664800: *stop=0x8c664800u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664802; /* mov.l @(0xc,r14),r2 */
L8c664802: *stop=0x8c664802u;r[2]+=r[11];goto L8c664804; /* add r11,r2 */
L8c664804: *stop=0x8c664804u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c664806; /* mov.l r2,@(0xc,r14) */
L8c664806: *stop=0x8c664806u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c664808; /* mov.l r5,@r15 */
L8c664808: *stop=0x8c664808u;r[5]+=4u;goto L8c66480a; /* add #0x4,r5 */
L8c66480a: target=0x8c664b12u;*pr=0x8c66480eu;*stop=0x8c66480cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66480e; /* bsr 0x8c664b12 */
L8c66480e: *stop=0x8c66480eu;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664810; /* mov.l @(0xc,r13),r3 */
L8c664810: *stop=0x8c664810u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664812; /* mov.l @(0x4,r15),r2 */
L8c664812: *stop=0x8c664812u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c664814; /* mov.l r3,@(0x8,r2) */
L8c664814: *stop=0x8c664814u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664816; /* mov.l @(0x4,r15),r2 */
L8c664816: *stop=0x8c664816u;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[2],4);if(io_status)return io_status;goto L8c664818; /* mov.l r2,@(0x14,r13) */
L8c664818: *stop=0x8c664818u;if(!valid_access(r[15]+8u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x8u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c66481a; /* mov.w @(0x8,r15),r0 */
L8c66481a: *stop=0x8c66481au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c66481c; /* tst r0,r0 */
L8c66481c: if(!(*sr&1u))goto L8c664828;goto L8c66481e; /* bf 0x8c664828 */
L8c66481e: *stop=0x8c66481eu;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[2]=loaded;}goto L8c664820; /* mov.w @r13,r2 */
L8c664820: *stop=0x8c664820u;{uint32_t loaded=sx(m,bus,&io_status,0x8c664860u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c664822; /* mov.w 0x8c664860,r3 */
L8c664822: *stop=0x8c664822u;r[2]|=r[3];goto L8c664824; /* or r3,r2 */
L8c664824: *stop=0x8c664826u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[2],2);if(io_status)return io_status;goto L8c664830; /* bra 0x8c664830 */
L8c664828: *stop=0x8c664828u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[1]=loaded;}goto L8c66482a; /* mov.w @r13,r1 */
L8c66482a: *stop=0x8c66482au;{uint32_t loaded=sx(m,bus,&io_status,0x8c664862u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c66482c; /* mov.w 0x8c664862,r3 */
L8c66482c: *stop=0x8c66482cu;r[1]|=r[3];goto L8c66482e; /* or r3,r1 */
L8c66482e: *stop=0x8c66482eu;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[1],2);if(io_status)return io_status;goto L8c664830; /* mov.w r1,@r13 */
L8c664830: *stop=0x8c664830u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c664832; /* mov.w @r13,r0 */
L8c664832: *stop=0x8c664832u;r[9]+=r[10];goto L8c664834; /* add r10,r9 */
L8c664834: *stop=0x8c664834u;r[0]|=0x4u;goto L8c664836; /* or #0x4,r0 */
L8c664836: *stop=0x8c664836u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c664838; /* mov.w r0,@r13 */
L8c664838: *stop=0x8c664838u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66483a; /* mov.l @(0x10,r13),r3 */
L8c66483a: *stop=0x8c66483au;if(!valid_access(r[9]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66483c; /* mov.l @(0x10,r9),r2 */
L8c66483c: *stop=0x8c66483cu;r[0]=0u;goto L8c66483e; /* mov #0x0,r0 */
L8c66483e: *stop=0x8c66483eu;r[2]-=r[3];goto L8c664840; /* sub r3,r2 */
L8c664840: *stop=0x8c664840u;if(!valid_access(r[9]+16u,4u))return -3;wr(m,bus,&io_status,r[9]+0x10u,r[2],4);if(io_status)return io_status;goto L8c664842; /* mov.l r2,@(0x10,r9) */
L8c664842: *stop=0x8c664842u;r[15]+=12u;goto L8c664844; /* add #0xc,r15 */
L8c664844: *stop=0x8c664844u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c664846; /* lds.l @r15+,PR */
L8c664846: *stop=0x8c664846u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c664848; /* mov.l @r15+,r8 */
L8c664848: *stop=0x8c664848u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c66484a; /* mov.l @r15+,r9 */
L8c66484a: *stop=0x8c66484au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c66484c; /* mov.l @r15+,r10 */
L8c66484c: *stop=0x8c66484cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c66484e; /* mov.l @r15+,r11 */
L8c66484e: *stop=0x8c66484eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c664850; /* mov.l @r15+,r12 */
L8c664850: *stop=0x8c664850u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c664852; /* mov.l @r15+,r13 */
L8c664852: *stop=0x8c664852u;if(*pr!=entry_pr)return -3;*stop=0x8c664854u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c6652c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6652c0;
L8c6652c0: *stop=0x8c6652c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6652c2; /* mov.l r14,@-r15 */
L8c6652c2: *stop=0x8c6652c2u;r[14]=1u;goto L8c6652c4; /* mov #0x1,r14 */
L8c6652c4: *stop=0x8c6652c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6652c6; /* sts.l PR,@-r15 */
L8c6652c6: *stop=0x8c6652c6u;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6652c8; /* mov.l @(0xc,r5),r0 */
L8c6652c8: *stop=0x8c6652c8u;r[7]=r[14];goto L8c6652ca; /* mov r14,r7 */
L8c6652ca: *stop=0x8c6652cau;{uint32_t loaded=sx(m,bus,&io_status,0x8c665320u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c6652cc; /* mov.w 0x8c665320,r6 */
L8c6652cc: *stop=0x8c6652ccu;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c6652ce; /* cmp/eq #0x8,r0 */
L8c6652ce: take=(*sr&1u);*stop=0x8c6652d0u;r[7]+=127u;if(take)goto L8c6652e2;goto L8c6652d2; /* bt/s 0x8c6652e2 */
L8c6652d2: *stop=0x8c6652d2u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c6652d4; /* cmp/eq #0x10,r0 */
L8c6652d4: if((*sr&1u))goto L8c665306;goto L8c6652d6; /* bt 0x8c665306 */
L8c6652d6: *stop=0x8c6652d6u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c6652d8; /* cmp/eq #0x20,r0 */
L8c6652d8: if((*sr&1u))goto L8c665350;goto L8c6652da; /* bt 0x8c665350 */
L8c6652da: *stop=0x8c6652dau;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c6652dc; /* cmp/eq #0x40,r0 */
L8c6652dc: if((*sr&1u))goto L8c665376;goto L8c6652de; /* bt 0x8c665376 */
L8c6652de: *stop=0x8c6652e0u;goto L8c6653ce; /* bra 0x8c6653ce */
L8c6652e2: *stop=0x8c6652e2u;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6652e4; /* mov.l @(0x18,r5),r3 */
L8c6652e4: *stop=0x8c6652e4u;*sr=(*sr&~1u)|!(r[3]&r[14]);goto L8c6652e6; /* tst r3,r14 */
L8c6652e6: if((*sr&1u))goto L8c6652f8;goto L8c6652e8; /* bt 0x8c6652f8 */
L8c6652e8: *stop=0x8c6652e8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665322u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6652ea; /* mov.w 0x8c665322,r1 */
L8c6652ea: *stop=0x8c6652eau;r[3]=32u;goto L8c6652ec; /* mov #0x20,r3 */
L8c6652ec: *stop=0x8c6652ecu;{uint32_t loaded=sx(m,bus,&io_status,0x8c665324u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c6652ee; /* mov.w 0x8c665324,r2 */
L8c6652ee: *stop=0x8c6652eeu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c6652f0; /* mov.l r1,@r4 */
L8c6652f0: *stop=0x8c6652f0u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[7],4);if(io_status)return io_status;goto L8c6652f2; /* mov.l r7,@(0x4,r4) */
L8c6652f2: *stop=0x8c6652f2u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6652f4; /* mov.l r3,@(0x8,r4) */
L8c6652f4: *stop=0x8c6652f6u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6653d6; /* bra 0x8c6653d6 */
L8c6652f8: *stop=0x8c6652f8u;r[2]=32u;goto L8c6652fa; /* mov #0x20,r2 */
L8c6652fa: *stop=0x8c6652fau;{uint32_t loaded=sx(m,bus,&io_status,0x8c665326u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6652fc; /* mov.w 0x8c665326,r3 */
L8c6652fc: *stop=0x8c6652fcu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[6],4);if(io_status)return io_status;goto L8c6652fe; /* mov.l r6,@r4 */
L8c6652fe: *stop=0x8c6652feu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[7],4);if(io_status)return io_status;goto L8c665300; /* mov.l r7,@(0x4,r4) */
L8c665300: *stop=0x8c665300u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c665302; /* mov.l r2,@(0x8,r4) */
L8c665302: *stop=0x8c665304u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6653d6; /* bra 0x8c6653d6 */
L8c665306: *stop=0x8c665306u;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665308; /* mov.l @(0x18,r5),r1 */
L8c665308: *stop=0x8c665308u;*sr=(*sr&~1u)|!(r[1]&r[14]);goto L8c66530a; /* tst r1,r14 */
L8c66530a: if((*sr&1u))goto L8c665340;goto L8c66530c; /* bt 0x8c665340 */
L8c66530c: *stop=0x8c66530cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c665328u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c66530e; /* mov.w 0x8c665328,r2 */
L8c66530e: *stop=0x8c66530eu;r[3]=96u;goto L8c665310; /* mov #0x60,r3 */
L8c665310: *stop=0x8c665310u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c665312; /* mov.l r2,@r4 */
L8c665312: *stop=0x8c665312u;{uint32_t loaded=sx(m,bus,&io_status,0x8c66532au,2);if(io_status)return io_status;r[2]=loaded;}goto L8c665314; /* mov.w 0x8c66532a,r2 */
L8c665314: *stop=0x8c665314u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[7],4);if(io_status)return io_status;goto L8c665316; /* mov.l r7,@(0x4,r4) */
L8c665316: *stop=0x8c665316u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[3],4);if(io_status)return io_status;goto L8c665318; /* mov.l r3,@(0x8,r4) */
L8c665318: *stop=0x8c66531au;goto L8c66534c; /* bra 0x8c66534c */
L8c665340: *stop=0x8c665340u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653a0u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c665342; /* mov.w 0x8c6653a0,r1 */
L8c665342: *stop=0x8c665342u;r[3]=64u;goto L8c665344; /* mov #0x40,r3 */
L8c665344: *stop=0x8c665344u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653a2u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c665346; /* mov.w 0x8c6653a2,r2 */
L8c665346: *stop=0x8c665346u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c665348; /* mov.l r1,@r4 */
L8c665348: *stop=0x8c665348u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[7],4);if(io_status)return io_status;goto L8c66534a; /* mov.l r7,@(0x4,r4) */
L8c66534a: *stop=0x8c66534au;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[3],4);if(io_status)return io_status;goto L8c66534c; /* mov.l r3,@(0x8,r4) */
L8c66534c: *stop=0x8c66534eu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6653d6; /* bra 0x8c6653d6 */
L8c665350: *stop=0x8c665350u;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665352; /* mov.l @(0x18,r5),r1 */
L8c665352: *stop=0x8c665352u;*sr=(*sr&~1u)|!(r[1]&r[14]);goto L8c665354; /* tst r1,r14 */
L8c665354: if((*sr&1u))goto L8c665368;goto L8c665356; /* bt 0x8c665368 */
L8c665356: *stop=0x8c665356u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653a4u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c665358; /* mov.w 0x8c6653a4,r2 */
L8c665358: *stop=0x8c665358u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653a6u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c66535a; /* mov.w 0x8c6653a6,r3 */
L8c66535a: *stop=0x8c66535au;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c66535c; /* mov.l r2,@r4 */
L8c66535c: *stop=0x8c66535cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653a8u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c66535e; /* mov.w 0x8c6653a8,r2 */
L8c66535e: *stop=0x8c66535eu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[3],4);if(io_status)return io_status;goto L8c665360; /* mov.l r3,@(0x4,r4) */
L8c665360: *stop=0x8c665360u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653aau,2);if(io_status)return io_status;r[3]=loaded;}goto L8c665362; /* mov.w 0x8c6653aa,r3 */
L8c665362: *stop=0x8c665362u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c665364; /* mov.l r2,@(0x8,r4) */
L8c665364: *stop=0x8c665366u;goto L8c66538a; /* bra 0x8c66538a */
L8c665368: *stop=0x8c665368u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653aeu,2);if(io_status)return io_status;r[5]=loaded;}goto L8c66536a; /* mov.w 0x8c6653ae,r5 */
L8c66536a: *stop=0x8c66536au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653acu,2);if(io_status)return io_status;r[1]=loaded;}goto L8c66536c; /* mov.w 0x8c6653ac,r1 */
L8c66536c: *stop=0x8c66536cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c66536e; /* mov.l r1,@r4 */
L8c66536e: *stop=0x8c66536eu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c665370; /* mov.l r5,@(0x4,r4) */
L8c665370: *stop=0x8c665370u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;goto L8c665372; /* mov.l r5,@(0x8,r4) */
L8c665372: *stop=0x8c665374u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[6],4);if(io_status)return io_status;goto L8c6653d6; /* bra 0x8c6653d6 */
L8c665376: *stop=0x8c665376u;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665378; /* mov.l @(0x18,r5),r2 */
L8c665378: *stop=0x8c665378u;*sr=(*sr&~1u)|!(r[2]&r[14]);goto L8c66537a; /* tst r2,r14 */
L8c66537a: if((*sr&1u))goto L8c6653c0;goto L8c66537c; /* bt 0x8c6653c0 */
L8c66537c: *stop=0x8c66537cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653b0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c66537e; /* mov.w 0x8c6653b0,r3 */
L8c66537e: *stop=0x8c66537eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653b2u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c665380; /* mov.w 0x8c6653b2,r2 */
L8c665380: *stop=0x8c665380u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6653b8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665382; /* mov.l 0x8c6653b8,r1 */
L8c665382: *stop=0x8c665382u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c665384; /* mov.l r1,@r4 */
L8c665384: *stop=0x8c665384u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[3],4);if(io_status)return io_status;goto L8c665386; /* mov.l r3,@(0x4,r4) */
L8c665386: *stop=0x8c665386u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6653b4u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c665388; /* mov.w 0x8c6653b4,r3 */
L8c665388: *stop=0x8c665388u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c66538a; /* mov.l r2,@(0x8,r4) */
L8c66538a: *stop=0x8c66538cu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6653d6; /* bra 0x8c6653d6 */
L8c6653c0: *stop=0x8c6653c0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665422u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c6653c2; /* mov.w 0x8c665422,r5 */
L8c6653c2: *stop=0x8c6653c2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665420u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6653c4; /* mov.w 0x8c665420,r1 */
L8c6653c4: *stop=0x8c6653c4u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c6653c6; /* mov.l r1,@r4 */
L8c6653c6: *stop=0x8c6653c6u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6653c8; /* mov.l r5,@(0x4,r4) */
L8c6653c8: *stop=0x8c6653c8u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;goto L8c6653ca; /* mov.l r5,@(0x8,r4) */
L8c6653ca: *stop=0x8c6653ccu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[6],4);if(io_status)return io_status;goto L8c6653d6; /* bra 0x8c6653d6 */
L8c6653ce: *stop=0x8c6653ceu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6653d0; /* lds.l @r15+,PR */
L8c6653d0: *stop=0x8c6653d0u;r[0]=0u;goto L8c6653d2; /* mov #0x0,r0 */
L8c6653d2: *stop=0x8c6653d2u;if(*pr!=entry_pr)return -3;*stop=0x8c6653d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c6653d6: *stop=0x8c6653d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665428u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6653d8; /* mov.l 0x8c665428,r3 */
L8c6653d8: *stop=0x8c6653d8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665424u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c6653da; /* mov.w 0x8c665424,r1 */
L8c6653da: target=r[3];*pr=0x8c6653deu;*stop=0x8c6653dcu;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6653de; /* jsr @r3 */
L8c6653de: *stop=0x8c6653deu;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[0],4);if(io_status)return io_status;goto L8c6653e0; /* mov.l r0,@(0x10,r4) */
L8c6653e0: *stop=0x8c6653e0u;r[0]=1u;goto L8c6653e2; /* mov #0x1,r0 */
L8c6653e2: *stop=0x8c6653e2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6653e4; /* lds.l @r15+,PR */
L8c6653e4: *stop=0x8c6653e4u;if(*pr!=entry_pr)return -3;*stop=0x8c6653e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c6653e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6653e8;
L8c6653e8: *stop=0x8c6653e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665430u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6653ea; /* mov.l 0x8c665430,r7 */
L8c6653ea: *stop=0x8c6653eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c66542cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6653ec; /* mov.l 0x8c66542c,r6 */
L8c6653ec: *stop=0x8c6653ecu;r[7]+=r[6];goto L8c6653ee; /* add r6,r7 */
L8c6653ee: *stop=0x8c6653eeu;r[4]=r[6];goto L8c6653f0; /* mov r6,r4 */
L8c6653f0: *stop=0x8c6653f0u;*sr=(*sr&~1u)|(r[4]>=r[7]);goto L8c6653f2; /* cmp/hs r7,r4 */
L8c6653f2: *stop=0x8c6653f2u;r[5]=r[6];goto L8c6653f4; /* mov r6,r5 */
L8c6653f4: take=(*sr&1u);*stop=0x8c6653f6u;r[6]=1u;if(take)goto L8c66540e;goto L8c6653f8; /* bt/s 0x8c66540e */
L8c6653f8: *stop=0x8c6653f8u;if(!valid_access(r[4],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[2]=loaded;}goto L8c6653fa; /* mov.w @r4,r2 */
L8c6653fa: *stop=0x8c6653fau;r[2]=r[2]&65535u;goto L8c6653fc; /* extu.w r2,r2 */
L8c6653fc: *stop=0x8c6653fcu;*sr=(*sr&~1u)|!(r[6]&r[2]);goto L8c6653fe; /* tst r6,r2 */
L8c6653fe: if(!(*sr&1u))goto L8c665406;goto L8c665400; /* bf 0x8c665406 */
L8c665400: *stop=0x8c665400u;if(!valid_access(r[5],2u))return -3;wr(m,bus,&io_status,r[5],r[6],2);if(io_status)return io_status;goto L8c665402; /* mov.w r6,@r5 */
L8c665402: *stop=0x8c665402u;if(*pr!=entry_pr)return -3;*stop=0x8c665404u;r[0]=r[5];return 0; /* rts */
L8c665406: *stop=0x8c665406u;r[4]+=76u;goto L8c665408; /* add #0x4c,r4 */
L8c665408: *stop=0x8c665408u;*sr=(*sr&~1u)|(r[4]>=r[7]);goto L8c66540a; /* cmp/hs r7,r4 */
L8c66540a: take=!(*sr&1u);*stop=0x8c66540cu;r[5]+=76u;if(take)goto L8c6653f8;goto L8c66540e; /* bf/s 0x8c6653f8 */
L8c66540e: *stop=0x8c66540eu;r[0]=0u;goto L8c665410; /* mov #0x0,r0 */
L8c665410: *stop=0x8c665410u;if(*pr!=entry_pr)return -3;*stop=0x8c665412u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c665466(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c665466;
L8c665466: *stop=0x8c665466u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c665468; /* mov.l @r4,r3 */
L8c665468: *stop=0x8c665468u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c66546a; /* tst r3,r3 */
L8c66546a: if(!(*sr&1u))goto L8c665470;goto L8c66546c; /* bf 0x8c665470 */
L8c66546c: *stop=0x8c66546eu;r[3]=0u;goto L8c665472; /* bra 0x8c665472 */
L8c665470: *stop=0x8c665470u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c665472; /* mov.l @r4,r3 */
L8c665472: *stop=0x8c665472u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[3],4);if(io_status)return io_status;goto L8c665474; /* mov.l r3,@(0x4,r5) */
L8c665474: *stop=0x8c665474u;if(*pr!=entry_pr)return -3;*stop=0x8c665476u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c65441c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65441c;
L8c65441c: *stop=0x8c65441cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65441e; /* mov.l r14,@-r15 */
L8c65441e: *stop=0x8c65441eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c654420; /* mov.l r13,@-r15 */
L8c654420: *stop=0x8c654420u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c654422; /* mov.l r12,@-r15 */
L8c654422: *stop=0x8c654422u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c654424; /* mov.l r11,@-r15 */
L8c654424: *stop=0x8c654424u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c654426; /* sts.l PR,@-r15 */
L8c654426: *stop=0x8c654426u;r[15]+=4294967288u;goto L8c654428; /* add #-0x8,r15 */
L8c654428: *stop=0x8c654428u;r[11]=r[4];goto L8c65442a; /* mov r4,r11 */
L8c65442a: *stop=0x8c65442au;if(!valid_access(r[11]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c65442c; /* mov.l @(0xc,r11),r13 */
L8c65442c: *stop=0x8c65442cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[13],4);if(io_status)return io_status;goto L8c65442e; /* mov.l r13,@(0x4,r15) */
L8c65442e: *stop=0x8c65442eu;*sr=(*sr&~1u)|(r[13]&1u);r[13]>>=1;goto L8c654430; /* shlr r13 */
L8c654430: *stop=0x8c654430u;if(!valid_access(r[11]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x10u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c654432; /* mov.l @(0x10,r11),r14 */
L8c654432: *stop=0x8c654432u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c654434; /* mov.l r14,@r15 */
L8c654434: *stop=0x8c654434u;*sr=(*sr&~1u)|(r[14]&1u);r[14]>>=1;goto L8c654436; /* shlr r14 */
L8c654436: *stop=0x8c654436u;*macl=r[14]*r[13];goto L8c654438; /* mul.l r14,r13 */
L8c654438: *stop=0x8c654438u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65443a; /* mov.l @r15,r5 */
L8c65443a: *stop=0x8c65443au;r[12]=*macl;goto L8c65443c; /* sts MACL,r12 */
L8c65443c: target=0x8c6541e0u;*pr=0x8c654440u;*stop=0x8c65443eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c654440; /* bsr 0x8c6541e0 */
L8c654440: *stop=0x8c654440u;if(!valid_access(r[11]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c654442; /* mov.l @(0x18,r11),r2 */
L8c654442: *stop=0x8c654442u;r[4]=1u;goto L8c654444; /* mov #0x1,r4 */
L8c654444: *stop=0x8c654444u;r[5]=r[0];goto L8c654446; /* mov r0,r5 */
L8c654446: *stop=0x8c654446u;r[6]=r[0];goto L8c654448; /* mov r0,r6 */
L8c654448: *stop=0x8c654448u;*sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c65444a; /* tst r4,r2 */
L8c65444a: take=(*sr&1u);*stop=0x8c65444cu;r[6]<<=2;if(take)goto L8c65448a;goto L8c65444e; /* bt/s 0x8c65448a */
L8c65444e: *stop=0x8c65444eu;*sr=(*sr&~1u)|(r[13]&1u);r[13]>>=1;goto L8c654450; /* shlr r13 */
L8c654450: *stop=0x8c654450u;*sr=(*sr&~1u)|(r[14]&1u);r[14]>>=1;goto L8c654452; /* shlr r14 */
L8c654452: *stop=0x8c654452u;*sr=(*sr&~1u)|(r[13]>=r[14]);goto L8c654454; /* cmp/hs r14,r13 */
L8c654454: if((*sr&1u))goto L8c65445a;goto L8c654456; /* bt 0x8c65445a */
L8c654456: *stop=0x8c654458u;r[5]=r[13];goto L8c65445c; /* bra 0x8c65445c */
L8c65445a: *stop=0x8c65445au;r[5]=r[14];goto L8c65445c; /* mov r14,r5 */
L8c65445c: *stop=0x8c65445cu;*sr=(*sr&~1u)|(r[5]>r[4]);goto L8c65445e; /* cmp/hi r4,r5 */
L8c65445e: if(!(*sr&1u))goto L8c654470;goto L8c654460; /* bf 0x8c654470 */
L8c654460: *stop=0x8c654460u;*macl=r[14]*r[13];goto L8c654462; /* mul.l r14,r13 */
L8c654462: *stop=0x8c654462u;*sr=(*sr&~1u)|(r[13]&1u);r[13]>>=1;goto L8c654464; /* shlr r13 */
L8c654464: *stop=0x8c654464u;*sr=(*sr&~1u)|(r[14]&1u);r[14]>>=1;goto L8c654466; /* shlr r14 */
L8c654466: *stop=0x8c654466u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c654468; /* shlr r5 */
L8c654468: *stop=0x8c654468u;r[3]=*macl;goto L8c65446a; /* sts MACL,r3 */
L8c65446a: *stop=0x8c65446au;*sr=(*sr&~1u)|(r[5]>r[4]);goto L8c65446c; /* cmp/hi r4,r5 */
L8c65446c: *stop=0x8c65446cu;r[12]+=r[3];goto L8c65446e; /* add r3,r12 */
L8c65446e: if((*sr&1u))goto L8c654460;goto L8c654470; /* bt 0x8c654460 */
L8c654470: *stop=0x8c654470u;*sr=(*sr&~1u)|(r[14]==r[13]);goto L8c654472; /* cmp/eq r14,r13 */
L8c654472: if(!(*sr&1u))goto L8c654478;goto L8c654474; /* bf 0x8c654478 */
L8c654474: *stop=0x8c654476u;r[12]+=2u;goto L8c65447e; /* bra 0x8c65447e */
L8c654478: *stop=0x8c654478u;*macl=r[14]*r[13];goto L8c65447a; /* mul.l r14,r13 */
L8c65447a: *stop=0x8c65447au;r[13]=*macl;goto L8c65447c; /* sts MACL,r13 */
L8c65447c: *stop=0x8c65447cu;r[12]+=r[13];goto L8c65447e; /* add r13,r12 */
L8c65447e: *stop=0x8c65447eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c654504u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c654480; /* mov.l 0x8c654504,r0 */
L8c654480: *stop=0x8c654480u;r[12]+=10u;goto L8c654482; /* add #0xa,r12 */
L8c654482: *stop=0x8c654482u;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c654484; /* mov.l @(r0,r6),r3 */
L8c654484: *stop=0x8c654484u;r[12]+=r[3];goto L8c654486; /* add r3,r12 */
L8c654486: *stop=0x8c654488u;goto L8c654490; /* bra 0x8c654490 */
L8c65448a: *stop=0x8c65448au;{uint32_t loaded=rd(m,bus,&io_status,0x8c654508u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65448c; /* mov.l 0x8c654508,r0 */
L8c65448c: *stop=0x8c65448cu;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65448e; /* mov.l @(r0,r6),r3 */
L8c65448e: *stop=0x8c65448eu;r[12]+=r[3];goto L8c654490; /* add r3,r12 */
L8c654490: *stop=0x8c654490u;r[0]=r[12];goto L8c654492; /* mov r12,r0 */
L8c654492: *stop=0x8c654492u;r[15]+=8u;goto L8c654494; /* add #0x8,r15 */
L8c654494: *stop=0x8c654494u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c654496; /* lds.l @r15+,PR */
L8c654496: *stop=0x8c654496u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c654498; /* mov.l @r15+,r11 */
L8c654498: *stop=0x8c654498u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65449a; /* mov.l @r15+,r12 */
L8c65449a: *stop=0x8c65449au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65449c; /* mov.l @r15+,r13 */
L8c65449c: *stop=0x8c65449cu;if(*pr!=entry_pr)return -3;*stop=0x8c65449eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c6542e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6542e0;
L8c6542e0: *stop=0x8c6542e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6542e2; /* mov.l r14,@-r15 */
L8c6542e2: *stop=0x8c6542e2u;r[14]=1u;goto L8c6542e4; /* mov #0x1,r14 */
L8c6542e4: *stop=0x8c6542e4u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6542e6; /* mov.l @(0x10,r4),r5 */
L8c6542e6: *stop=0x8c6542e6u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6542e8; /* mov.l @(0xc,r4),r6 */
L8c6542e8: *stop=0x8c6542e8u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6542ea; /* mov.l @(0x18,r4),r3 */
L8c6542ea: *stop=0x8c6542eau;*macl=r[5]*r[6];goto L8c6542ec; /* mul.l r5,r6 */
L8c6542ec: *stop=0x8c6542ecu;*sr=(*sr&~1u)|!(r[14]&r[3]);goto L8c6542ee; /* tst r14,r3 */
L8c6542ee: take=(*sr&1u);*stop=0x8c6542f0u;r[7]=*macl;if(take)goto L8c654322;goto L8c6542f2; /* bt/s 0x8c654322 */
L8c6542f2: *stop=0x8c6542f2u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c6542f4; /* shlr r6 */
L8c6542f4: *stop=0x8c6542f4u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c6542f6; /* shlr r5 */
L8c6542f6: *stop=0x8c6542f6u;*sr=(*sr&~1u)|(r[6]>=r[5]);goto L8c6542f8; /* cmp/hs r5,r6 */
L8c6542f8: if((*sr&1u))goto L8c6542fe;goto L8c6542fa; /* bt 0x8c6542fe */
L8c6542fa: *stop=0x8c6542fcu;r[0]=r[6];goto L8c654300; /* bra 0x8c654300 */
L8c6542fe: *stop=0x8c6542feu;r[0]=r[5];goto L8c654300; /* mov r5,r0 */
L8c654300: *stop=0x8c654300u;*sr=(*sr&~1u)|(r[0]>r[14]);goto L8c654302; /* cmp/hi r14,r0 */
L8c654302: if(!(*sr&1u))goto L8c654314;goto L8c654304; /* bf 0x8c654314 */
L8c654304: *stop=0x8c654304u;*macl=r[5]*r[6];goto L8c654306; /* mul.l r5,r6 */
L8c654306: *stop=0x8c654306u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c654308; /* shlr r6 */
L8c654308: *stop=0x8c654308u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c65430a; /* shlr r5 */
L8c65430a: *stop=0x8c65430au;*sr=(*sr&~1u)|(r[0]&1u);r[0]>>=1;goto L8c65430c; /* shlr r0 */
L8c65430c: *stop=0x8c65430cu;r[3]=*macl;goto L8c65430e; /* sts MACL,r3 */
L8c65430e: *stop=0x8c65430eu;*sr=(*sr&~1u)|(r[0]>r[14]);goto L8c654310; /* cmp/hi r14,r0 */
L8c654310: *stop=0x8c654310u;r[7]+=r[3];goto L8c654312; /* add r3,r7 */
L8c654312: if((*sr&1u))goto L8c654304;goto L8c654314; /* bt 0x8c654304 */
L8c654314: *stop=0x8c654314u;*sr=(*sr&~1u)|(r[5]==r[6]);goto L8c654316; /* cmp/eq r5,r6 */
L8c654316: if(!(*sr&1u))goto L8c65431c;goto L8c654318; /* bf 0x8c65431c */
L8c654318: *stop=0x8c65431au;r[7]+=4u;goto L8c654322; /* bra 0x8c654322 */
L8c65431c: *stop=0x8c65431cu;*macl=r[5]*r[6];goto L8c65431e; /* mul.l r5,r6 */
L8c65431e: *stop=0x8c65431eu;r[6]=*macl;goto L8c654320; /* sts MACL,r6 */
L8c654320: *stop=0x8c654320u;r[7]+=r[6];goto L8c654322; /* add r6,r7 */
L8c654322: *stop=0x8c654322u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c654324; /* mov.l @(0x4,r4),r0 */
L8c654324: *stop=0x8c654324u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c654326; /* cmp/eq #0x1,r0 */
L8c654326: if(!(*sr&1u))goto L8c65432c;goto L8c654328; /* bf 0x8c65432c */
L8c654328: *stop=0x8c65432au;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c65432e; /* bra 0x8c65432e */
L8c65432c: *stop=0x8c65432cu;r[7]<<=2;goto L8c65432e; /* shll2 r7 */
L8c65432e: *stop=0x8c65432eu;r[0]=r[7];goto L8c654330; /* mov r7,r0 */
L8c654330: *stop=0x8c654330u;if(*pr!=entry_pr)return -3;*stop=0x8c654332u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c654392(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c654392;
L8c654392: *stop=0x8c654392u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c654394; /* mov.l r14,@-r15 */
L8c654394: *stop=0x8c654394u;r[14]=1u;goto L8c654396; /* mov #0x1,r14 */
L8c654396: *stop=0x8c654396u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c654398; /* mov.l @(0x10,r4),r7 */
L8c654398: *stop=0x8c654398u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65439a; /* mov.l @(0xc,r4),r6 */
L8c65439a: *stop=0x8c65439au;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65439c; /* mov.l @(0x18,r4),r3 */
L8c65439c: *stop=0x8c65439cu;*macl=r[7]*r[6];goto L8c65439e; /* mul.l r7,r6 */
L8c65439e: *stop=0x8c65439eu;*sr=(*sr&~1u)|!(r[14]&r[3]);goto L8c6543a0; /* tst r14,r3 */
L8c6543a0: take=(*sr&1u);*stop=0x8c6543a2u;r[5]=*macl;if(take)goto L8c654402;goto L8c6543a4; /* bt/s 0x8c654402 */
L8c6543a4: *stop=0x8c6543a4u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c6543a6; /* shlr r6 */
L8c6543a6: *stop=0x8c6543a6u;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c6543a8; /* shlr r7 */
L8c6543a8: *stop=0x8c6543a8u;*sr=(*sr&~1u)|(r[6]>=r[7]);goto L8c6543aa; /* cmp/hs r7,r6 */
L8c6543aa: if((*sr&1u))goto L8c6543b0;goto L8c6543ac; /* bt 0x8c6543b0 */
L8c6543ac: *stop=0x8c6543aeu;r[0]=r[6];goto L8c6543b2; /* bra 0x8c6543b2 */
L8c6543b0: *stop=0x8c6543b0u;r[0]=r[7];goto L8c6543b2; /* mov r7,r0 */
L8c6543b2: *stop=0x8c6543b2u;*sr=(*sr&~1u)|(r[0]>r[14]);goto L8c6543b4; /* cmp/hi r14,r0 */
L8c6543b4: if(!(*sr&1u))goto L8c6543c6;goto L8c6543b6; /* bf 0x8c6543c6 */
L8c6543b6: *stop=0x8c6543b6u;*macl=r[7]*r[6];goto L8c6543b8; /* mul.l r7,r6 */
L8c6543b8: *stop=0x8c6543b8u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c6543ba; /* shlr r6 */
L8c6543ba: *stop=0x8c6543bau;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c6543bc; /* shlr r7 */
L8c6543bc: *stop=0x8c6543bcu;*sr=(*sr&~1u)|(r[0]&1u);r[0]>>=1;goto L8c6543be; /* shlr r0 */
L8c6543be: *stop=0x8c6543beu;r[3]=*macl;goto L8c6543c0; /* sts MACL,r3 */
L8c6543c0: *stop=0x8c6543c0u;*sr=(*sr&~1u)|(r[0]>r[14]);goto L8c6543c2; /* cmp/hi r14,r0 */
L8c6543c2: *stop=0x8c6543c2u;r[5]+=r[3];goto L8c6543c4; /* add r3,r5 */
L8c6543c4: if((*sr&1u))goto L8c6543b6;goto L8c6543c6; /* bt 0x8c6543b6 */
L8c6543c6: *stop=0x8c6543c6u;*sr=(*sr&~1u)|(r[7]==r[6]);goto L8c6543c8; /* cmp/eq r7,r6 */
L8c6543c8: if(!(*sr&1u))goto L8c6543ce;goto L8c6543ca; /* bf 0x8c6543ce */
L8c6543ca: *stop=0x8c6543ccu;r[5]+=1u;goto L8c6543d4; /* bra 0x8c6543d4 */
L8c6543ce: *stop=0x8c6543ceu;*macl=r[7]*r[6];goto L8c6543d0; /* mul.l r7,r6 */
L8c6543d0: *stop=0x8c6543d0u;r[6]=*macl;goto L8c6543d2; /* sts MACL,r6 */
L8c6543d2: *stop=0x8c6543d2u;r[5]+=r[6];goto L8c6543d4; /* add r6,r5 */
L8c6543d4: *stop=0x8c6543d4u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6543d6; /* mov.l @(0x8,r4),r3 */
L8c6543d6: *stop=0x8c6543d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6543e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6543d8; /* mov.l 0x8c6543e4,r2 */
L8c6543d8: *stop=0x8c6543d8u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c6543da; /* cmp/eq r2,r3 */
L8c6543da: if(!(*sr&1u))goto L8c654400;goto L8c6543dc; /* bf 0x8c654400 */
L8c6543dc: *stop=0x8c6543deu;r[5]+=43u;goto L8c654402; /* bra 0x8c654402 */
L8c654400: *stop=0x8c654400u;r[5]+=11u;goto L8c654402; /* add #0xb,r5 */
L8c654402: *stop=0x8c654402u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c654404; /* mov.l @(0x8,r4),r3 */
L8c654404: *stop=0x8c654404u;{uint32_t loaded=rd(m,bus,&io_status,0x8c654500u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c654406; /* mov.l 0x8c654500,r2 */
L8c654406: *stop=0x8c654406u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c654408; /* cmp/eq r2,r3 */
L8c654408: if(!(*sr&1u))goto L8c654416;goto L8c65440a; /* bf 0x8c654416 */
L8c65440a: *stop=0x8c65440au;r[0]=r[5];goto L8c65440c; /* mov r5,r0 */
L8c65440c: *stop=0x8c65440cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65440e; /* cmp/eq #0x1,r0 */
L8c65440e: if(!(*sr&1u))goto L8c654414;goto L8c654410; /* bf 0x8c654414 */
L8c654410: *stop=0x8c654412u;r[5]=r[14];goto L8c654416; /* bra 0x8c654416 */
L8c654414: *stop=0x8c654414u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c654416; /* shlr r5 */
L8c654416: *stop=0x8c654416u;r[0]=r[5];goto L8c654418; /* mov r5,r0 */
L8c654418: *stop=0x8c654418u;if(*pr!=entry_pr)return -3;*stop=0x8c65441au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c654334(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c654334;
L8c654334: *stop=0x8c654334u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c654336; /* mov.l @(0x10,r4),r5 */
L8c654336: *stop=0x8c654336u;r[0]=1u;goto L8c654338; /* mov #0x1,r0 */
L8c654338: *stop=0x8c654338u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65433a; /* mov.l @(0xc,r4),r6 */
L8c65433a: *stop=0x8c65433au;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65433c; /* mov.l @(0x18,r4),r2 */
L8c65433c: *stop=0x8c65433cu;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c65433e; /* shlr r6 */
L8c65433e: *stop=0x8c65433eu;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c654340; /* shlr r5 */
L8c654340: *stop=0x8c654340u;*macl=r[5]*r[6];goto L8c654342; /* mul.l r5,r6 */
L8c654342: *stop=0x8c654342u;*sr=(*sr&~1u)|!(r[0]&r[2]);goto L8c654344; /* tst r0,r2 */
L8c654344: take=(*sr&1u);*stop=0x8c654346u;r[7]=*macl;if(take)goto L8c65437c;goto L8c654348; /* bt/s 0x8c65437c */
L8c654348: *stop=0x8c654348u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c65434a; /* shlr r6 */
L8c65434a: *stop=0x8c65434au;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c65434c; /* shlr r5 */
L8c65434c: *stop=0x8c65434cu;*sr=(*sr&~1u)|(r[6]>=r[5]);goto L8c65434e; /* cmp/hs r5,r6 */
L8c65434e: if((*sr&1u))goto L8c654354;goto L8c654350; /* bt 0x8c654354 */
L8c654350: *stop=0x8c654352u;r[4]=r[6];goto L8c654356; /* bra 0x8c654356 */
L8c654354: *stop=0x8c654354u;r[4]=r[5];goto L8c654356; /* mov r5,r4 */
L8c654356: *stop=0x8c654356u;*sr=(*sr&~1u)|(r[4]>r[0]);goto L8c654358; /* cmp/hi r0,r4 */
L8c654358: if(!(*sr&1u))goto L8c65436a;goto L8c65435a; /* bf 0x8c65436a */
L8c65435a: *stop=0x8c65435au;*macl=r[5]*r[6];goto L8c65435c; /* mul.l r5,r6 */
L8c65435c: *stop=0x8c65435cu;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c65435e; /* shlr r6 */
L8c65435e: *stop=0x8c65435eu;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c654360; /* shlr r5 */
L8c654360: *stop=0x8c654360u;*sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1;goto L8c654362; /* shlr r4 */
L8c654362: *stop=0x8c654362u;r[3]=*macl;goto L8c654364; /* sts MACL,r3 */
L8c654364: *stop=0x8c654364u;*sr=(*sr&~1u)|(r[4]>r[0]);goto L8c654366; /* cmp/hi r0,r4 */
L8c654366: *stop=0x8c654366u;r[7]+=r[3];goto L8c654368; /* add r3,r7 */
L8c654368: if((*sr&1u))goto L8c65435a;goto L8c65436a; /* bt 0x8c65435a */
L8c65436a: *stop=0x8c65436au;*sr=(*sr&~1u)|(r[5]==r[6]);goto L8c65436c; /* cmp/eq r5,r6 */
L8c65436c: if(!(*sr&1u))goto L8c654372;goto L8c65436e; /* bf 0x8c654372 */
L8c65436e: *stop=0x8c654370u;r[7]+=2u;goto L8c654378; /* bra 0x8c654378 */
L8c654372: *stop=0x8c654372u;*macl=r[5]*r[6];goto L8c654374; /* mul.l r5,r6 */
L8c654374: *stop=0x8c654374u;r[6]=*macl;goto L8c654376; /* sts MACL,r6 */
L8c654376: *stop=0x8c654376u;r[7]+=r[6];goto L8c654378; /* add r6,r7 */
L8c654378: *stop=0x8c65437au;r[7]+=10u;goto L8c65438a; /* bra 0x8c65438a */
L8c65437c: *stop=0x8c65437cu;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65437e; /* mov.l @(0xc,r4),r0 */
L8c65437e: *stop=0x8c65437eu;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c654380; /* cmp/eq #0x8,r0 */
L8c654380: if(!(*sr&1u))goto L8c65438a;goto L8c654382; /* bf 0x8c65438a */
L8c654382: *stop=0x8c654382u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c654384; /* mov.l @(0x10,r4),r0 */
L8c654384: *stop=0x8c654384u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c654386; /* cmp/eq #0x8,r0 */
L8c654386: if(!(*sr&1u))goto L8c65438a;goto L8c654388; /* bf 0x8c65438a */
L8c654388: *stop=0x8c654388u;r[7]+=16u;goto L8c65438a; /* add #0x10,r7 */
L8c65438a: *stop=0x8c65438au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6543e0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65438c; /* mov.w 0x8c6543e0,r3 */
L8c65438c: *stop=0x8c65438cu;r[7]+=r[3];goto L8c65438e; /* add r3,r7 */
L8c65438e: *stop=0x8c65438eu;if(*pr!=entry_pr)return -3;*stop=0x8c654390u;r[0]=r[7];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c664f72(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c664f72;
L8c664f72: *stop=0x8c664f72u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c664f74; /* mov.l r14,@-r15 */
L8c664f74: *stop=0x8c664f74u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c664f76; /* mov.l r13,@-r15 */
L8c664f76: *stop=0x8c664f76u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c664f78; /* mov.l r12,@-r15 */
L8c664f78: *stop=0x8c664f78u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c664f7a; /* mov.l r11,@-r15 */
L8c664f7a: *stop=0x8c664f7au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c664f7c; /* mov.l r10,@-r15 */
L8c664f7c: *stop=0x8c664f7cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c664f7e; /* mov.l r9,@-r15 */
L8c664f7e: *stop=0x8c664f7eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c664f80; /* mov.l r8,@-r15 */
L8c664f80: *stop=0x8c664f80u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c664f82; /* sts.l PR,@-r15 */
L8c664f82: *stop=0x8c664f82u;r[15]+=4294967280u;goto L8c664f84; /* add #-0x10,r15 */
L8c664f84: *stop=0x8c664f84u;r[0]=r[4];goto L8c664f86; /* mov r4,r0 */
L8c664f86: *stop=0x8c664f86u;r[12]=r[6];goto L8c664f88; /* mov r6,r12 */
L8c664f88: *stop=0x8c664f88u;if(!valid_access(r[15]+4u,2u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],2);if(io_status)return io_status;goto L8c664f8a; /* mov.w r0,@(0x4,r15) */
L8c664f8a: *stop=0x8c664f8au;r[0]=r[12];goto L8c664f8c; /* mov r12,r0 */
L8c664f8c: *stop=0x8c664f8cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c664f8e; /* cmp/pz r0 */
L8c664f8e: *stop=0x8c664f8eu;r[6]=r[5];goto L8c664f90; /* mov r5,r6 */
L8c664f90: *stop=0x8c664f90u;r[11]=r[5];goto L8c664f92; /* mov r5,r11 */
L8c664f92: *stop=0x8c664f92u;r[8]=r[7];goto L8c664f94; /* mov r7,r8 */
L8c664f94: take=!(*sr&1u);*stop=0x8c664f96u;r[6]+=r[12];if(take)goto L8c664f9c;goto L8c664f98; /* bf/s 0x8c664f9c */
L8c664f98: *stop=0x8c664f9au;r[0]&=0x1fu;goto L8c664fa6; /* bra 0x8c664fa6 */
L8c664f9c: *stop=0x8c664f9cu;r[0]=~r[0];goto L8c664f9e; /* not r0,r0 */
L8c664f9e: *stop=0x8c664f9eu;r[0]+=1u;goto L8c664fa0; /* add #0x1,r0 */
L8c664fa0: *stop=0x8c664fa0u;r[0]&=0x1fu;goto L8c664fa2; /* and #0x1f,r0 */
L8c664fa2: *stop=0x8c664fa2u;r[0]=~r[0];goto L8c664fa4; /* not r0,r0 */
L8c664fa4: *stop=0x8c664fa4u;r[0]+=1u;goto L8c664fa6; /* add #0x1,r0 */
L8c664fa6: *stop=0x8c664fa6u;r[4]=r[0];goto L8c664fa8; /* mov r0,r4 */
L8c664fa8: *stop=0x8c664fa8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c664faa; /* tst r4,r4 */
L8c664faa: take=(*sr&1u);*stop=0x8c664facu;r[7]=0u;if(take)goto L8c664fb4;goto L8c664fae; /* bt/s 0x8c664fb4 */
L8c664fae: *stop=0x8c664faeu;r[3]=32u;goto L8c664fb0; /* mov #0x20,r3 */
L8c664fb0: *stop=0x8c664fb0u;r[3]-=r[4];goto L8c664fb2; /* sub r4,r3 */
L8c664fb2: *stop=0x8c664fb2u;r[12]+=r[3];goto L8c664fb4; /* add r3,r12 */
L8c664fb4: *stop=0x8c664fb4u;if(!valid_access(r[15]+4u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c664fb6; /* mov.w @(0x4,r15),r0 */
L8c664fb6: *stop=0x8c664fb6u;r[14]=r[7];goto L8c664fb8; /* mov r7,r14 */
L8c664fb8: *stop=0x8c664fb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665000u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c664fba; /* mov.l 0x8c665000,r10 */
L8c664fba: *stop=0x8c664fbau;r[3]=r[0];goto L8c664fbc; /* mov r0,r3 */
L8c664fbc: *stop=0x8c664fbcu;r[0]<<=2;goto L8c664fbe; /* shll2 r0 */
L8c664fbe: *stop=0x8c664fbeu;r[0]+=r[3];goto L8c664fc0; /* add r3,r0 */
L8c664fc0: *stop=0x8c664fc0u;r[0]<<=2;goto L8c664fc2; /* shll2 r0 */
L8c664fc2: *stop=0x8c664fc2u;r[4]=r[0];goto L8c664fc4; /* mov r0,r4 */
L8c664fc4: *stop=0x8c664fc4u;r[4]+=r[10];goto L8c664fc6; /* add r10,r4 */
L8c664fc6: *stop=0x8c664fc6u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c664fc8; /* mov.l @(0x8,r4),r4 */
L8c664fc8: *stop=0x8c664fc8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c664fca; /* tst r4,r4 */
L8c664fca: take=(*sr&1u);*stop=0x8c664fccu;r[9]=r[0];if(take)goto L8c665026;goto L8c664fce; /* bt/s 0x8c665026 */
L8c664fce: *stop=0x8c664fceu;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c664fd0; /* mov.l @(0xc,r4),r5 */
L8c664fd0: *stop=0x8c664fd0u;*sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[11]);goto L8c664fd2; /* cmp/gt r11,r5 */
L8c664fd2: if((*sr&1u))goto L8c665020;goto L8c664fd4; /* bt 0x8c665020 */
L8c664fd4: *stop=0x8c664fd4u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664fd6; /* mov.l @(0x10,r4),r3 */
L8c664fd6: *stop=0x8c664fd6u;r[3]+=r[5];goto L8c664fd8; /* add r5,r3 */
L8c664fd8: *stop=0x8c664fd8u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[11]);goto L8c664fda; /* cmp/gt r11,r3 */
L8c664fda: if(!(*sr&1u))goto L8c665020;goto L8c664fdc; /* bf 0x8c665020 */
L8c664fdc: *stop=0x8c664fdcu;*sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[6]);goto L8c664fde; /* cmp/gt r6,r5 */
L8c664fde: if((*sr&1u))goto L8c665020;goto L8c664fe0; /* bt 0x8c665020 */
L8c664fe0: *stop=0x8c664fe0u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c664fe2; /* mov.l @(0x10,r4),r2 */
L8c664fe2: *stop=0x8c664fe2u;r[2]+=r[5];goto L8c664fe4; /* add r5,r2 */
L8c664fe4: *stop=0x8c664fe4u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[6]);goto L8c664fe6; /* cmp/ge r6,r2 */
L8c664fe6: if(!(*sr&1u))goto L8c665020;goto L8c664fe8; /* bf 0x8c665020 */
L8c664fe8: *stop=0x8c664fe8u;r[14]=r[4];goto L8c664fea; /* mov r4,r14 */
L8c664fea: *stop=0x8c664fecu;r[4]=r[7];goto L8c665022; /* bra 0x8c665022 */
L8c665020: *stop=0x8c665020u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c665022; /* mov.l @(0x8,r4),r4 */
L8c665022: *stop=0x8c665022u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c665024; /* tst r4,r4 */
L8c665024: if(!(*sr&1u))goto L8c664fce;goto L8c665026; /* bf 0x8c664fce */
L8c665026: *stop=0x8c665026u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c665028; /* tst r14,r14 */
L8c665028: if(!(*sr&1u))goto L8c66502e;goto L8c66502a; /* bf 0x8c66502e */
L8c66502a: *stop=0x8c66502cu;goto L8c6651dc; /* bra 0x8c6651dc */
L8c66502e: *stop=0x8c66502eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665030; /* mov.l @(0x10,r14),r3 */
L8c665030: *stop=0x8c665030u;*sr=(*sr&~1u)|(r[12]==r[3]);goto L8c665032; /* cmp/eq r12,r3 */
L8c665032: if(!(*sr&1u))goto L8c6650c0;goto L8c665034; /* bf 0x8c6650c0 */
L8c665034: *stop=0x8c665034u;r[13]=r[9];goto L8c665036; /* mov r9,r13 */
L8c665036: *stop=0x8c665036u;r[13]+=r[10];goto L8c665038; /* add r10,r13 */
L8c665038: *stop=0x8c665038u;r[5]=r[13];goto L8c66503a; /* mov r13,r5 */
L8c66503a: *stop=0x8c66503au;r[4]=r[13];goto L8c66503c; /* mov r13,r4 */
L8c66503c: *stop=0x8c66503cu;r[5]+=12u;goto L8c66503e; /* add #0xc,r5 */
L8c66503e: *stop=0x8c66503eu;r[6]=r[14];goto L8c665040; /* mov r14,r6 */
L8c665040: target=0x8c664b52u;*pr=0x8c665044u;*stop=0x8c665042u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665044; /* bsr 0x8c664b52 */
L8c665044: *stop=0x8c665044u;r[5]=r[13];goto L8c665046; /* mov r13,r5 */
L8c665046: *stop=0x8c665046u;r[5]+=4u;goto L8c665048; /* add #0x4,r5 */
L8c665048: *stop=0x8c665048u;r[6]=r[14];goto L8c66504a; /* mov r14,r6 */
L8c66504a: target=0x8c664b12u;*pr=0x8c66504eu;*stop=0x8c66504cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66504e; /* bsr 0x8c664b12 */
L8c66504e: *stop=0x8c66504eu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665050; /* mov.l @(0xc,r14),r3 */
L8c665050: *stop=0x8c665050u;r[2]=1u;goto L8c665052; /* mov #0x1,r2 */
L8c665052: *stop=0x8c665052u;if(!valid_access(r[8]+8u,4u))return -3;wr(m,bus,&io_status,r[8]+0x8u,r[3],4);if(io_status)return io_status;goto L8c665054; /* mov.l r3,@(0x8,r8) */
L8c665054: *stop=0x8c665054u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[8],4);if(io_status)return io_status;goto L8c665056; /* mov.l r8,@(0x14,r14) */
L8c665056: *stop=0x8c665056u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c665058; /* mov.w r2,@r14 */
L8c665058: *stop=0x8c665058u;if(!valid_access(r[15]+4u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c66505a; /* mov.w @(0x4,r15),r0 */
L8c66505a: *stop=0x8c66505au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c66505c; /* tst r0,r0 */
L8c66505c: if(!(*sr&1u))goto L8c665068;goto L8c66505e; /* bf 0x8c665068 */
L8c66505e: *stop=0x8c66505eu;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[2]=loaded;}goto L8c665060; /* mov.w @r14,r2 */
L8c665060: *stop=0x8c665060u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6650a0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c665062; /* mov.w 0x8c6650a0,r3 */
L8c665062: *stop=0x8c665062u;r[2]|=r[3];goto L8c665064; /* or r3,r2 */
L8c665064: *stop=0x8c665066u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c665070; /* bra 0x8c665070 */
L8c665068: *stop=0x8c665068u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[1]=loaded;}goto L8c66506a; /* mov.w @r14,r1 */
L8c66506a: *stop=0x8c66506au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6650a2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c66506c; /* mov.w 0x8c6650a2,r3 */
L8c66506c: *stop=0x8c66506cu;r[1]|=r[3];goto L8c66506e; /* or r3,r1 */
L8c66506e: *stop=0x8c66506eu;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[1],2);if(io_status)return io_status;goto L8c665070; /* mov.w r1,@r14 */
L8c665070: *stop=0x8c665070u;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665072; /* mov.l @(0x30,r15),r2 */
L8c665072: *stop=0x8c665072u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c665074; /* tst r2,r2 */
L8c665074: if((*sr&1u))goto L8c66507c;goto L8c665076; /* bt 0x8c66507c */
L8c665076: *stop=0x8c665076u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[0]=loaded;}goto L8c665078; /* mov.w @r14,r0 */
L8c665078: *stop=0x8c665078u;r[0]|=0x2u;goto L8c66507a; /* or #0x2,r0 */
L8c66507a: *stop=0x8c66507au;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[0],2);if(io_status)return io_status;goto L8c66507c; /* mov.w r0,@r14 */
L8c66507c: *stop=0x8c66507cu;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66507e; /* mov.l @(0x34,r15),r3 */
L8c66507e: *stop=0x8c66507eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c665080; /* tst r3,r3 */
L8c665080: take=(*sr&1u);*stop=0x8c665082u;r[9]+=r[10];if(take)goto L8c66508a;goto L8c665084; /* bt/s 0x8c66508a */
L8c665084: *stop=0x8c665084u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[0]=loaded;}goto L8c665086; /* mov.w @r14,r0 */
L8c665086: *stop=0x8c665086u;r[0]|=0x4u;goto L8c665088; /* or #0x4,r0 */
L8c665088: *stop=0x8c665088u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[0],2);if(io_status)return io_status;goto L8c66508a; /* mov.w r0,@r14 */
L8c66508a: *stop=0x8c66508cu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66512e; /* bra 0x8c66512e */
L8c6650c0: *stop=0x8c6650c0u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6650c2; /* mov.l @(0xc,r14),r0 */
L8c6650c2: *stop=0x8c6650c2u;*sr=(*sr&~1u)|(r[11]==r[0]);goto L8c6650c4; /* cmp/eq r11,r0 */
L8c6650c4: if(!(*sr&1u))goto L8c665160;goto L8c6650c6; /* bf 0x8c665160 */
L8c6650c6: target=0x8c664ac0u;*pr=0x8c6650cau;*stop=0x8c6650c8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6650ca; /* bsr 0x8c664ac0 */
L8c6650ca: *stop=0x8c6650cau;r[13]=r[0];goto L8c6650cc; /* mov r0,r13 */
L8c6650cc: *stop=0x8c6650ccu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c6650ce; /* tst r13,r13 */
L8c6650ce: if(!(*sr&1u))goto L8c6650d4;goto L8c6650d0; /* bf 0x8c6650d4 */
L8c6650d0: *stop=0x8c6650d2u;goto L8c6651dc; /* bra 0x8c6651dc */
L8c6650d4: *stop=0x8c6650d4u;r[5]=r[9];goto L8c6650d6; /* mov r9,r5 */
L8c6650d6: *stop=0x8c6650d6u;r[5]+=r[10];goto L8c6650d8; /* add r10,r5 */
L8c6650d8: *stop=0x8c6650d8u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6650da; /* mov.l @(0xc,r14),r2 */
L8c6650da: *stop=0x8c6650dau;r[6]=r[13];goto L8c6650dc; /* mov r13,r6 */
L8c6650dc: *stop=0x8c6650dcu;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6650de; /* mov.l r2,@(0xc,r13) */
L8c6650de: *stop=0x8c6650deu;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[12],4);if(io_status)return io_status;goto L8c6650e0; /* mov.l r12,@(0x10,r13) */
L8c6650e0: *stop=0x8c6650e0u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6650e2; /* mov.l @(0x10,r14),r4 */
L8c6650e2: *stop=0x8c6650e2u;r[4]-=r[12];goto L8c6650e4; /* sub r12,r4 */
L8c6650e4: *stop=0x8c6650e4u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[4],4);if(io_status)return io_status;goto L8c6650e6; /* mov.l r4,@(0x10,r14) */
L8c6650e6: *stop=0x8c6650e6u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6650e8; /* mov.l @(0xc,r14),r3 */
L8c6650e8: *stop=0x8c6650e8u;r[3]+=r[12];goto L8c6650ea; /* add r12,r3 */
L8c6650ea: *stop=0x8c6650eau;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6650ec; /* mov.l r3,@(0xc,r14) */
L8c6650ec: *stop=0x8c6650ecu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c6650ee; /* mov.l r5,@r15 */
L8c6650ee: *stop=0x8c6650eeu;r[5]+=4u;goto L8c6650f0; /* add #0x4,r5 */
L8c6650f0: target=0x8c664b12u;*pr=0x8c6650f4u;*stop=0x8c6650f2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6650f4; /* bsr 0x8c664b12 */
L8c6650f4: *stop=0x8c6650f4u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6650f6; /* mov.l @(0xc,r13),r2 */
L8c6650f6: *stop=0x8c6650f6u;if(!valid_access(r[8]+8u,4u))return -3;wr(m,bus,&io_status,r[8]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6650f8; /* mov.l r2,@(0x8,r8) */
L8c6650f8: *stop=0x8c6650f8u;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[8],4);if(io_status)return io_status;goto L8c6650fa; /* mov.l r8,@(0x14,r13) */
L8c6650fa: *stop=0x8c6650fau;if(!valid_access(r[15]+4u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c6650fc; /* mov.w @(0x4,r15),r0 */
L8c6650fc: *stop=0x8c6650fcu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6650fe; /* tst r0,r0 */
L8c6650fe: if(!(*sr&1u))goto L8c66510a;goto L8c665100; /* bf 0x8c66510a */
L8c665100: *stop=0x8c665100u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[2]=loaded;}goto L8c665102; /* mov.w @r13,r2 */
L8c665102: *stop=0x8c665102u;{uint32_t loaded=sx(m,bus,&io_status,0x8c665140u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c665104; /* mov.w 0x8c665140,r3 */
L8c665104: *stop=0x8c665104u;r[2]|=r[3];goto L8c665106; /* or r3,r2 */
L8c665106: *stop=0x8c665108u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[2],2);if(io_status)return io_status;goto L8c665112; /* bra 0x8c665112 */
L8c66510a: *stop=0x8c66510au;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[1]=loaded;}goto L8c66510c; /* mov.w @r13,r1 */
L8c66510c: *stop=0x8c66510cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c665142u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c66510e; /* mov.w 0x8c665142,r3 */
L8c66510e: *stop=0x8c66510eu;r[1]|=r[3];goto L8c665110; /* or r3,r1 */
L8c665110: *stop=0x8c665110u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[1],2);if(io_status)return io_status;goto L8c665112; /* mov.w r1,@r13 */
L8c665112: *stop=0x8c665112u;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665114; /* mov.l @(0x30,r15),r2 */
L8c665114: *stop=0x8c665114u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c665116; /* tst r2,r2 */
L8c665116: if((*sr&1u))goto L8c66511e;goto L8c665118; /* bt 0x8c66511e */
L8c665118: *stop=0x8c665118u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c66511a; /* mov.w @r13,r0 */
L8c66511a: *stop=0x8c66511au;r[0]|=0x2u;goto L8c66511c; /* or #0x2,r0 */
L8c66511c: *stop=0x8c66511cu;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c66511e; /* mov.w r0,@r13 */
L8c66511e: *stop=0x8c66511eu;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665120; /* mov.l @(0x34,r15),r3 */
L8c665120: *stop=0x8c665120u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c665122; /* tst r3,r3 */
L8c665122: take=(*sr&1u);*stop=0x8c665124u;r[9]+=r[10];if(take)goto L8c66512c;goto L8c665126; /* bt/s 0x8c66512c */
L8c665126: *stop=0x8c665126u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c665128; /* mov.w @r13,r0 */
L8c665128: *stop=0x8c665128u;r[0]|=0x4u;goto L8c66512a; /* or #0x4,r0 */
L8c66512a: *stop=0x8c66512au;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c66512c; /* mov.w r0,@r13 */
L8c66512c: *stop=0x8c66512cu;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66512e; /* mov.l @(0x10,r13),r3 */
L8c66512e: *stop=0x8c66512eu;if(!valid_access(r[9]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665130; /* mov.l @(0x10,r9),r2 */
L8c665130: *stop=0x8c665130u;r[2]-=r[3];goto L8c665132; /* sub r3,r2 */
L8c665132: *stop=0x8c665134u;if(!valid_access(r[9]+16u,4u))return -3;wr(m,bus,&io_status,r[9]+0x10u,r[2],4);if(io_status)return io_status;goto L8c66527e; /* bra 0x8c66527e */
L8c665160: *stop=0x8c665160u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665162; /* mov.l @(0xc,r14),r1 */
L8c665162: *stop=0x8c665162u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665164; /* mov.l @(0x10,r14),r3 */
L8c665164: *stop=0x8c665164u;r[1]+=r[3];goto L8c665166; /* add r3,r1 */
L8c665166: *stop=0x8c665166u;*sr=(*sr&~1u)|(r[6]==r[1]);goto L8c665168; /* cmp/eq r6,r1 */
L8c665168: if(!(*sr&1u))goto L8c6651c4;goto L8c66516a; /* bf 0x8c6651c4 */
L8c66516a: target=0x8c664ac0u;*pr=0x8c66516eu;*stop=0x8c66516cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66516e; /* bsr 0x8c664ac0 */
L8c66516e: *stop=0x8c66516eu;r[13]=r[0];goto L8c665170; /* mov r0,r13 */
L8c665170: *stop=0x8c665170u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c665172; /* tst r13,r13 */
L8c665172: if((*sr&1u))goto L8c6651dc;goto L8c665174; /* bt 0x8c6651dc */
L8c665174: *stop=0x8c665174u;r[5]=r[9];goto L8c665176; /* mov r9,r5 */
L8c665176: *stop=0x8c665176u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c665178; /* mov.l @(0x10,r14),r4 */
L8c665178: *stop=0x8c665178u;r[5]+=r[10];goto L8c66517a; /* add r10,r5 */
L8c66517a: *stop=0x8c66517au;r[6]=r[13];goto L8c66517c; /* mov r13,r6 */
L8c66517c: *stop=0x8c66517cu;r[4]-=r[12];goto L8c66517e; /* sub r12,r4 */
L8c66517e: *stop=0x8c66517eu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[4],4);if(io_status)return io_status;goto L8c665180; /* mov.l r4,@(0x10,r14) */
L8c665180: *stop=0x8c665180u;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[11],4);if(io_status)return io_status;goto L8c665182; /* mov.l r11,@(0xc,r13) */
L8c665182: *stop=0x8c665182u;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[12],4);if(io_status)return io_status;goto L8c665184; /* mov.l r12,@(0x10,r13) */
L8c665184: *stop=0x8c665184u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c665186; /* mov.l r5,@r15 */
L8c665186: *stop=0x8c665186u;r[5]+=4u;goto L8c665188; /* add #0x4,r5 */
L8c665188: target=0x8c664b12u;*pr=0x8c66518cu;*stop=0x8c66518au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66518c; /* bsr 0x8c664b12 */
L8c66518c: *stop=0x8c66518cu;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66518e; /* mov.l @(0xc,r13),r2 */
L8c66518e: *stop=0x8c66518eu;if(!valid_access(r[8]+8u,4u))return -3;wr(m,bus,&io_status,r[8]+0x8u,r[2],4);if(io_status)return io_status;goto L8c665190; /* mov.l r2,@(0x8,r8) */
L8c665190: *stop=0x8c665190u;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[8],4);if(io_status)return io_status;goto L8c665192; /* mov.l r8,@(0x14,r13) */
L8c665192: *stop=0x8c665192u;if(!valid_access(r[15]+4u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c665194; /* mov.w @(0x4,r15),r0 */
L8c665194: *stop=0x8c665194u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c665196; /* tst r0,r0 */
L8c665196: if(!(*sr&1u))goto L8c6651a2;goto L8c665198; /* bf 0x8c6651a2 */
L8c665198: *stop=0x8c665198u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[2]=loaded;}goto L8c66519a; /* mov.w @r13,r2 */
L8c66519a: *stop=0x8c66519au;{uint32_t loaded=sx(m,bus,&io_status,0x8c6651e0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c66519c; /* mov.w 0x8c6651e0,r3 */
L8c66519c: *stop=0x8c66519cu;r[2]|=r[3];goto L8c66519e; /* or r3,r2 */
L8c66519e: *stop=0x8c6651a0u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[2],2);if(io_status)return io_status;goto L8c6651aa; /* bra 0x8c6651aa */
L8c6651a2: *stop=0x8c6651a2u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[1]=loaded;}goto L8c6651a4; /* mov.w @r13,r1 */
L8c6651a4: *stop=0x8c6651a4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6651e2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c6651a6; /* mov.w 0x8c6651e2,r3 */
L8c6651a6: *stop=0x8c6651a6u;r[1]|=r[3];goto L8c6651a8; /* or r3,r1 */
L8c6651a8: *stop=0x8c6651a8u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[1],2);if(io_status)return io_status;goto L8c6651aa; /* mov.w r1,@r13 */
L8c6651aa: *stop=0x8c6651aau;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6651ac; /* mov.l @(0x30,r15),r2 */
L8c6651ac: *stop=0x8c6651acu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6651ae; /* tst r2,r2 */
L8c6651ae: if((*sr&1u))goto L8c6651b6;goto L8c6651b0; /* bt 0x8c6651b6 */
L8c6651b0: *stop=0x8c6651b0u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c6651b2; /* mov.w @r13,r0 */
L8c6651b2: *stop=0x8c6651b2u;r[0]|=0x2u;goto L8c6651b4; /* or #0x2,r0 */
L8c6651b4: *stop=0x8c6651b4u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c6651b6; /* mov.w r0,@r13 */
L8c6651b6: *stop=0x8c6651b6u;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6651b8; /* mov.l @(0x34,r15),r3 */
L8c6651b8: *stop=0x8c6651b8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6651ba; /* tst r3,r3 */
L8c6651ba: if((*sr&1u))goto L8c665274;goto L8c6651bc; /* bt 0x8c665274 */
L8c6651bc: *stop=0x8c6651bcu;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c6651be; /* mov.w @r13,r0 */
L8c6651be: *stop=0x8c6651beu;r[0]|=0x4u;goto L8c6651c0; /* or #0x4,r0 */
L8c6651c0: *stop=0x8c6651c2u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c665274; /* bra 0x8c665274 */
L8c6651c4: target=0x8c664ac0u;*pr=0x8c6651c8u;*stop=0x8c6651c6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6651c8; /* bsr 0x8c664ac0 */
L8c6651c8: *stop=0x8c6651c8u;r[13]=r[0];goto L8c6651ca; /* mov r0,r13 */
L8c6651ca: *stop=0x8c6651cau;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c6651cc; /* tst r13,r13 */
L8c6651cc: if((*sr&1u))goto L8c6651dc;goto L8c6651ce; /* bt 0x8c6651dc */
L8c6651ce: target=0x8c664ac0u;*pr=0x8c6651d2u;*stop=0x8c6651d0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6651d2; /* bsr 0x8c664ac0 */
L8c6651d2: *stop=0x8c6651d2u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c6651d4; /* tst r0,r0 */
L8c6651d4: take=!(*sr&1u);*stop=0x8c6651d6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;if(take)goto L8c665200;goto L8c6651d8; /* bf/s 0x8c665200 */
L8c6651d8: target=0x8c664aecu;*pr=0x8c6651dcu;*stop=0x8c6651dau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6651dc; /* bsr 0x8c664aec */
L8c6651dc: *stop=0x8c6651deu;r[0]=3u;goto L8c665280; /* bra 0x8c665280 */
L8c665200: *stop=0x8c665200u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665202; /* mov.l @(0x10,r14),r3 */
L8c665202: *stop=0x8c665202u;r[2]=r[11];goto L8c665204; /* mov r11,r2 */
L8c665204: *stop=0x8c665204u;r[6]=r[13];goto L8c665206; /* mov r13,r6 */
L8c665206: *stop=0x8c665206u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c665208; /* mov.l r3,@(0xc,r15) */
L8c665208: *stop=0x8c665208u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66520a; /* mov.l @(0xc,r14),r3 */
L8c66520a: *stop=0x8c66520au;r[2]-=r[3];goto L8c66520c; /* sub r3,r2 */
L8c66520c: *stop=0x8c66520cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c66520e; /* mov.l r2,@(0x8,r15) */
L8c66520e: *stop=0x8c66520eu;r[3]=r[2];goto L8c665210; /* mov r2,r3 */
L8c665210: *stop=0x8c665210u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c665212; /* mov.l r2,@(0x10,r14) */
L8c665212: *stop=0x8c665212u;r[14]=r[9];goto L8c665214; /* mov r9,r14 */
L8c665214: *stop=0x8c665214u;r[14]+=r[10];goto L8c665216; /* add r10,r14 */
L8c665216: *stop=0x8c665216u;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[11],4);if(io_status)return io_status;goto L8c665218; /* mov.l r11,@(0xc,r13) */
L8c665218: *stop=0x8c665218u;r[5]=r[14];goto L8c66521a; /* mov r14,r5 */
L8c66521a: *stop=0x8c66521au;r[5]+=4u;goto L8c66521c; /* add #0x4,r5 */
L8c66521c: *stop=0x8c66521cu;if(!valid_access(r[13]+16u,4u))return -3;wr(m,bus,&io_status,r[13]+0x10u,r[12],4);if(io_status)return io_status;goto L8c66521e; /* mov.l r12,@(0x10,r13) */
L8c66521e: target=0x8c664b12u;*pr=0x8c665222u;*stop=0x8c665220u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665222; /* bsr 0x8c664b12 */
L8c665222: *stop=0x8c665222u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665224; /* mov.l @r15,r2 */
L8c665224: *stop=0x8c665224u;r[11]+=r[12];goto L8c665226; /* add r12,r11 */
L8c665226: *stop=0x8c665226u;r[4]=r[14];goto L8c665228; /* mov r14,r4 */
L8c665228: *stop=0x8c665228u;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[11],4);if(io_status)return io_status;goto L8c66522a; /* mov.l r11,@(0xc,r2) */
L8c66522a: *stop=0x8c66522au;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66522c; /* mov.l @(0xc,r15),r2 */
L8c66522c: *stop=0x8c66522cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66522e; /* mov.l @(0x8,r15),r1 */
L8c66522e: *stop=0x8c66522eu;r[2]-=r[12];goto L8c665230; /* sub r12,r2 */
L8c665230: *stop=0x8c665230u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c665232; /* mov.l @r15,r3 */
L8c665232: *stop=0x8c665232u;r[2]-=r[1];goto L8c665234; /* sub r1,r2 */
L8c665234: *stop=0x8c665234u;if(!valid_access(r[3]+16u,4u))return -3;wr(m,bus,&io_status,r[3]+0x10u,r[2],4);if(io_status)return io_status;goto L8c665236; /* mov.l r2,@(0x10,r3) */
L8c665236: *stop=0x8c665236u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c665238; /* mov.l @(0xc,r14),r6 */
L8c665238: *stop=0x8c665238u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c66523a; /* mov.l @r15,r5 */
L8c66523a: target=0x8c664b36u;*pr=0x8c66523eu;*stop=0x8c66523cu;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66523e; /* bsr 0x8c664b36 */
L8c66523e: *stop=0x8c66523eu;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665240; /* mov.l @(0xc,r13),r2 */
L8c665240: *stop=0x8c665240u;if(!valid_access(r[8]+8u,4u))return -3;wr(m,bus,&io_status,r[8]+0x8u,r[2],4);if(io_status)return io_status;goto L8c665242; /* mov.l r2,@(0x8,r8) */
L8c665242: *stop=0x8c665242u;if(!valid_access(r[13]+20u,4u))return -3;wr(m,bus,&io_status,r[13]+0x14u,r[8],4);if(io_status)return io_status;goto L8c665244; /* mov.l r8,@(0x14,r13) */
L8c665244: *stop=0x8c665244u;if(!valid_access(r[15]+4u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c665246; /* mov.w @(0x4,r15),r0 */
L8c665246: *stop=0x8c665246u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c665248; /* tst r0,r0 */
L8c665248: if(!(*sr&1u))goto L8c665254;goto L8c66524a; /* bf 0x8c665254 */
L8c66524a: *stop=0x8c66524au;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[2]=loaded;}goto L8c66524c; /* mov.w @r13,r2 */
L8c66524c: *stop=0x8c66524cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6652a0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c66524e; /* mov.w 0x8c6652a0,r3 */
L8c66524e: *stop=0x8c66524eu;r[2]|=r[3];goto L8c665250; /* or r3,r2 */
L8c665250: *stop=0x8c665252u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[2],2);if(io_status)return io_status;goto L8c66525c; /* bra 0x8c66525c */
L8c665254: *stop=0x8c665254u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[1]=loaded;}goto L8c665256; /* mov.w @r13,r1 */
L8c665256: *stop=0x8c665256u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6652a2u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c665258; /* mov.w 0x8c6652a2,r3 */
L8c665258: *stop=0x8c665258u;r[1]|=r[3];goto L8c66525a; /* or r3,r1 */
L8c66525a: *stop=0x8c66525au;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[1],2);if(io_status)return io_status;goto L8c66525c; /* mov.w r1,@r13 */
L8c66525c: *stop=0x8c66525cu;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66525e; /* mov.l @(0x30,r15),r2 */
L8c66525e: *stop=0x8c66525eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c665260; /* tst r2,r2 */
L8c665260: if((*sr&1u))goto L8c665268;goto L8c665262; /* bt 0x8c665268 */
L8c665262: *stop=0x8c665262u;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c665264; /* mov.w @r13,r0 */
L8c665264: *stop=0x8c665264u;r[0]|=0x2u;goto L8c665266; /* or #0x2,r0 */
L8c665266: *stop=0x8c665266u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c665268; /* mov.w r0,@r13 */
L8c665268: *stop=0x8c665268u;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66526a; /* mov.l @(0x34,r15),r3 */
L8c66526a: *stop=0x8c66526au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c66526c; /* tst r3,r3 */
L8c66526c: if((*sr&1u))goto L8c665274;goto L8c66526e; /* bt 0x8c665274 */
L8c66526e: *stop=0x8c66526eu;if(!valid_access(r[13],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[0]=loaded;}goto L8c665270; /* mov.w @r13,r0 */
L8c665270: *stop=0x8c665270u;r[0]|=0x4u;goto L8c665272; /* or #0x4,r0 */
L8c665272: *stop=0x8c665272u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[0],2);if(io_status)return io_status;goto L8c665274; /* mov.w r0,@r13 */
L8c665274: *stop=0x8c665274u;r[9]+=r[10];goto L8c665276; /* add r10,r9 */
L8c665276: *stop=0x8c665276u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665278; /* mov.l @(0x10,r13),r3 */
L8c665278: *stop=0x8c665278u;if(!valid_access(r[9]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66527a; /* mov.l @(0x10,r9),r2 */
L8c66527a: *stop=0x8c66527au;r[2]-=r[3];goto L8c66527c; /* sub r3,r2 */
L8c66527c: *stop=0x8c66527cu;if(!valid_access(r[9]+16u,4u))return -3;wr(m,bus,&io_status,r[9]+0x10u,r[2],4);if(io_status)return io_status;goto L8c66527e; /* mov.l r2,@(0x10,r9) */
L8c66527e: *stop=0x8c66527eu;r[0]=0u;goto L8c665280; /* mov #0x0,r0 */
L8c665280: *stop=0x8c665280u;r[15]+=16u;goto L8c665282; /* add #0x10,r15 */
L8c665282: *stop=0x8c665282u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c665284; /* lds.l @r15+,PR */
L8c665284: *stop=0x8c665284u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c665286; /* mov.l @r15+,r8 */
L8c665286: *stop=0x8c665286u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c665288; /* mov.l @r15+,r9 */
L8c665288: *stop=0x8c665288u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c66528a; /* mov.l @r15+,r10 */
L8c66528a: *stop=0x8c66528au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c66528c; /* mov.l @r15+,r11 */
L8c66528c: *stop=0x8c66528cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c66528e; /* mov.l @r15+,r12 */
L8c66528e: *stop=0x8c66528eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c665290; /* mov.l @r15+,r13 */
L8c665290: *stop=0x8c665290u;if(*pr!=entry_pr)return -3;*stop=0x8c665292u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c6541e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6541e0;
L8c6541e0: *stop=0x8c6541e0u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]);goto L8c6541e2; /* cmp/ge r5,r4 */
L8c6541e2: if(!(*sr&1u))goto L8c6541e6;goto L8c6541e4; /* bf 0x8c6541e6 */
L8c6541e4: *stop=0x8c6541e4u;r[4]=r[5];goto L8c6541e6; /* mov r5,r4 */
L8c6541e6: *stop=0x8c6541e6u;r[0]=r[4];goto L8c6541e8; /* mov r4,r0 */
L8c6541e8: *stop=0x8c6541e8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6541ea; /* cmp/eq #0x1,r0 */
L8c6541ea: if((*sr&1u))goto L8c654220;goto L8c6541ec; /* bt 0x8c654220 */
L8c6541ec: *stop=0x8c6541ecu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c6541ee; /* cmp/eq #0x2,r0 */
L8c6541ee: if((*sr&1u))goto L8c654224;goto L8c6541f0; /* bt 0x8c654224 */
L8c6541f0: *stop=0x8c6541f0u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c6541f2; /* cmp/eq #0x4,r0 */
L8c6541f2: if((*sr&1u))goto L8c654260;goto L8c6541f4; /* bt 0x8c654260 */
L8c6541f4: *stop=0x8c6541f4u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c6541f6; /* cmp/eq #0x8,r0 */
L8c6541f6: if((*sr&1u))goto L8c654280;goto L8c6541f8; /* bt 0x8c654280 */
L8c6541f8: *stop=0x8c6541f8u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c6541fa; /* cmp/eq #0x10,r0 */
L8c6541fa: if((*sr&1u))goto L8c654264;goto L8c6541fc; /* bt 0x8c654264 */
L8c6541fc: *stop=0x8c6541fcu;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c6541fe; /* cmp/eq #0x20,r0 */
L8c6541fe: if((*sr&1u))goto L8c654268;goto L8c654200; /* bt 0x8c654268 */
L8c654200: *stop=0x8c654200u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c654202; /* cmp/eq #0x40,r0 */
L8c654202: if((*sr&1u))goto L8c65426c;goto L8c654204; /* bt 0x8c65426c */
L8c654204: *stop=0x8c654204u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654240u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654206; /* mov.w 0x8c654240,r1 */
L8c654206: *stop=0x8c654206u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c654208; /* cmp/eq r1,r0 */
L8c654208: if((*sr&1u))goto L8c654270;goto L8c65420a; /* bt 0x8c654270 */
L8c65420a: *stop=0x8c65420au;{uint32_t loaded=sx(m,bus,&io_status,0x8c654242u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65420c; /* mov.w 0x8c654242,r1 */
L8c65420c: *stop=0x8c65420cu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c65420e; /* cmp/eq r1,r0 */
L8c65420e: if((*sr&1u))goto L8c654274;goto L8c654210; /* bt 0x8c654274 */
L8c654210: *stop=0x8c654210u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654244u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654212; /* mov.w 0x8c654244,r1 */
L8c654212: *stop=0x8c654212u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c654214; /* cmp/eq r1,r0 */
L8c654214: if((*sr&1u))goto L8c654278;goto L8c654216; /* bt 0x8c654278 */
L8c654216: *stop=0x8c654216u;{uint32_t loaded=sx(m,bus,&io_status,0x8c654246u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c654218; /* mov.w 0x8c654246,r1 */
L8c654218: *stop=0x8c654218u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c65421a; /* cmp/eq r1,r0 */
L8c65421a: if((*sr&1u))goto L8c65427c;goto L8c65421c; /* bt 0x8c65427c */
L8c65421c: *stop=0x8c65421eu;goto L8c654280; /* bra 0x8c654280 */
L8c654220: *stop=0x8c654220u;if(*pr!=entry_pr)return -3;*stop=0x8c654222u;r[0]=0u;return 0; /* rts */
L8c654224: *stop=0x8c654224u;if(*pr!=entry_pr)return -3;*stop=0x8c654226u;r[0]=1u;return 0; /* rts */
L8c654260: *stop=0x8c654260u;if(*pr!=entry_pr)return -3;*stop=0x8c654262u;r[0]=2u;return 0; /* rts */
L8c654264: *stop=0x8c654264u;if(*pr!=entry_pr)return -3;*stop=0x8c654266u;r[0]=4u;return 0; /* rts */
L8c654268: *stop=0x8c654268u;if(*pr!=entry_pr)return -3;*stop=0x8c65426au;r[0]=5u;return 0; /* rts */
L8c65426c: *stop=0x8c65426cu;if(*pr!=entry_pr)return -3;*stop=0x8c65426eu;r[0]=6u;return 0; /* rts */
L8c654270: *stop=0x8c654270u;if(*pr!=entry_pr)return -3;*stop=0x8c654272u;r[0]=7u;return 0; /* rts */
L8c654274: *stop=0x8c654274u;if(*pr!=entry_pr)return -3;*stop=0x8c654276u;r[0]=8u;return 0; /* rts */
L8c654278: *stop=0x8c654278u;if(*pr!=entry_pr)return -3;*stop=0x8c65427au;r[0]=9u;return 0; /* rts */
L8c65427c: *stop=0x8c65427cu;if(*pr!=entry_pr)return -3;*stop=0x8c65427eu;r[0]=10u;return 0; /* rts */
L8c654280: *stop=0x8c654280u;r[0]=3u;goto L8c654282; /* mov #0x3,r0 */
L8c654282: *stop=0x8c654282u;if(*pr!=entry_pr)return -3;*stop=0x8c654284u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_render_buffer_8c664b36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c664b36;
L8c664b36: *stop=0x8c664b36u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664b38; /* mov.l @(0x8,r6),r3 */
L8c664b38: *stop=0x8c664b38u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c664b3a; /* tst r3,r3 */
L8c664b3a: if(!(*sr&1u))goto L8c664b44;goto L8c664b3c; /* bf 0x8c664b44 */
L8c664b3c: *stop=0x8c664b3cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c664b3e; /* mov.l r5,@r4 */
L8c664b3e: *stop=0x8c664b3eu;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[6],4);if(io_status)return io_status;goto L8c664b40; /* mov.l r6,@(0x4,r5) */
L8c664b40: *stop=0x8c664b42u;r[3]=0u;goto L8c664b4c; /* bra 0x8c664b4c */
L8c664b44: *stop=0x8c664b44u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c664b46; /* mov.l @(0x8,r6),r4 */
L8c664b46: *stop=0x8c664b46u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c664b48; /* mov.l r5,@(0x4,r4) */
L8c664b48: *stop=0x8c664b48u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[6],4);if(io_status)return io_status;goto L8c664b4a; /* mov.l r6,@(0x4,r5) */
L8c664b4a: *stop=0x8c664b4au;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c664b4c; /* mov.l @(0x8,r6),r3 */
L8c664b4c: *stop=0x8c664b4cu;if(!valid_access(r[5]+8u,4u))return -3;wr(m,bus,&io_status,r[5]+0x8u,r[3],4);if(io_status)return io_status;goto L8c664b4e; /* mov.l r3,@(0x8,r5) */
L8c664b4e: *stop=0x8c664b4eu;if(*pr!=entry_pr)return -3;*stop=0x8c664b50u;if(!valid_access(r[6]+8u,4u))return -3;wr(m,bus,&io_status,r[6]+0x8u,r[5],4);if(io_status)return io_status;return 0; /* rts */
}
