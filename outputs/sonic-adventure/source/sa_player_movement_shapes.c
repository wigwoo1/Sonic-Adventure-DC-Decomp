/* Original complete collision point and direction shape helpers.
 * Exact original instruction order and existing numerical/bus ABI.
 * No upstream implementation code imported.
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
int sa_sh4_sqrt(uint32_t,uint32_t*,uint32_t*);
int sa_movie_channel_io_return_allowed(void *,uint32_t);
static int sa_player_movement_shapes_run_8c02fc12(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c02fc72u)goto L8c02fc72;if(resume_pc==0x8c02fc7cu)goto L8c02fc7c;if(resume_pc==0x8c02fc92u)goto L8c02fc92;if(resume_pc==0x8c02fca2u)goto L8c02fca2;if(resume_pc==0x8c02fcb2u)goto L8c02fcb2;if(resume_pc==0x8c02fcc6u)goto L8c02fcc6;if(resume_pc==0x8c02fcd6u)goto L8c02fcd6;if(resume_pc==0x8c02fce6u)goto L8c02fce6;if(resume_pc==0x8c02fcf2u)goto L8c02fcf2;goto L8c02fc12;
L8c02fc12: *stop=0x8c02fc12u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c02fc14; /* mov.l r14,@-r15 */
L8c02fc14: *stop=0x8c02fc14u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c02fc16; /* mov.l r13,@-r15 */
L8c02fc16: *stop=0x8c02fc16u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c02fc18; /* mov.l r12,@-r15 */
L8c02fc18: *stop=0x8c02fc18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c02fc1a; /* sts.l PR,@-r15 */
L8c02fc1a: *stop=0x8c02fc1au;r[15]+=4294967292u;goto L8c02fc1c; /* add #-0x4,r15 */
L8c02fc1c: *stop=0x8c02fc1cu;r[14]=r[5];goto L8c02fc1e; /* mov r5,r14 */
L8c02fc1e: *stop=0x8c02fc1eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02fc20; /* mov.l @r14,r0 */
L8c02fc20: *stop=0x8c02fc20u;r[13]=r[4];goto L8c02fc22; /* mov r4,r13 */
L8c02fc22: *stop=0x8c02fc22u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c02fc24; /* tst #0x20,r0 */
L8c02fc24: take=(*sr&1u);*stop=0x8c02fc26u;r[12]=r[6];if(take)goto L8c02fc60;goto L8c02fc28; /* bt/s 0x8c02fc60 */
L8c02fc28: *stop=0x8c02fc28u;r[0]=8u;goto L8c02fc2a; /* mov #0x8,r0 */
L8c02fc2a: *stop=0x8c02fc2au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02fc2c; /* fmov.s @(r0,r14),fr3 */
L8c02fc2c: *stop=0x8c02fc2cu;r[0]=12u;goto L8c02fc2e; /* mov #0xc,r0 */
L8c02fc2e: *stop=0x8c02fc2eu;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c02fc30; /* fmov.s fr3,@r12 */
L8c02fc30: *stop=0x8c02fc30u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02fc32; /* fmov.s @(r0,r14),fr3 */
L8c02fc32: *stop=0x8c02fc32u;r[0]=4u;goto L8c02fc34; /* mov #0x4,r0 */
L8c02fc34: *stop=0x8c02fc34u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c02fc36; /* fmov fr3,@(r0,r12) */
L8c02fc36: *stop=0x8c02fc36u;r[0]=16u;goto L8c02fc38; /* mov #0x10,r0 */
L8c02fc38: *stop=0x8c02fc38u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02fc3a; /* fmov.s @(r0,r14),fr3 */
L8c02fc3a: *stop=0x8c02fc3au;r[0]=8u;goto L8c02fc3c; /* mov #0x8,r0 */
L8c02fc3c: *stop=0x8c02fc3eu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c02fd26; /* bra 0x8c02fd26 */
L8c02fc60: *stop=0x8c02fc60u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fc62; /* mov.l @r14,r2 */
L8c02fc62: *stop=0x8c02fc62u;r[3]=32u;goto L8c02fc64; /* mov #0x20,r3 */
L8c02fc64: *stop=0x8c02fc64u;r[3]<<=8;goto L8c02fc66; /* shll8 r3 */
L8c02fc66: *stop=0x8c02fc66u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c02fc68; /* tst r3,r2 */
L8c02fc68: if(!(*sr&1u))goto L8c02fd02;goto L8c02fc6a; /* bf 0x8c02fd02 */
L8c02fc6a: *stop=0x8c02fc6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdc4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fc6c; /* mov.l 0x8c02fdc4,r2 */
L8c02fc6c: *stop=0x8c02fc6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdc0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c02fc6e; /* mov.l 0x8c02fdc0,r4 */
L8c02fc6e: target=r[2];*pr=0x8c02fc72u;*stop=0x8c02fc70u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fc72; /* jsr @r2 */
L8c02fc72: *stop=0x8c02fc72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdc8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fc74; /* mov.l 0x8c02fdc8,r3 */
L8c02fc74: *stop=0x8c02fc74u;r[5]=r[13];goto L8c02fc76; /* mov r13,r5 */
L8c02fc76: *stop=0x8c02fc76u;r[5]+=32u;goto L8c02fc78; /* add #0x20,r5 */
L8c02fc78: target=r[3];*pr=0x8c02fc7cu;*stop=0x8c02fc7au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fc7c; /* jsr @r3 */
L8c02fc7c: *stop=0x8c02fc7cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02fc7e; /* mov.l @r14,r0 */
L8c02fc7e: *stop=0x8c02fc7eu;*sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c02fc80; /* tst #0x40,r0 */
L8c02fc80: if((*sr&1u))goto L8c02fcb6;goto L8c02fc82; /* bt 0x8c02fcb6 */
L8c02fc82: *stop=0x8c02fc82u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fc84; /* mov.l @(0x18,r13),r3 */
L8c02fc84: *stop=0x8c02fc84u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fc86; /* tst r3,r3 */
L8c02fc86: take=(*sr&1u);*stop=0x8c02fc88u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02fc92;goto L8c02fc8a; /* bt/s 0x8c02fc92 */
L8c02fc8a: *stop=0x8c02fc8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fc8c; /* mov.l 0x8c02fdcc,r3 */
L8c02fc8c: *stop=0x8c02fc8cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fc8e; /* mov.l @r15,r5 */
L8c02fc8e: target=r[3];*pr=0x8c02fc92u;*stop=0x8c02fc90u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fc92; /* jsr @r3 */
L8c02fc92: *stop=0x8c02fc92u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fc94; /* mov.l @(0x1c,r13),r3 */
L8c02fc94: *stop=0x8c02fc94u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fc96; /* tst r3,r3 */
L8c02fc96: take=(*sr&1u);*stop=0x8c02fc98u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02fca2;goto L8c02fc9a; /* bt/s 0x8c02fca2 */
L8c02fc9a: *stop=0x8c02fc9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdd0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fc9c; /* mov.l 0x8c02fdd0,r3 */
L8c02fc9c: *stop=0x8c02fc9cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fc9e; /* mov.l @r15,r5 */
L8c02fc9e: target=r[3];*pr=0x8c02fca2u;*stop=0x8c02fca0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fca2; /* jsr @r3 */
L8c02fca2: *stop=0x8c02fca2u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fca4; /* mov.l @(0x14,r13),r3 */
L8c02fca4: *stop=0x8c02fca4u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fca6; /* tst r3,r3 */
L8c02fca6: take=(*sr&1u);*stop=0x8c02fca8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02fce6;goto L8c02fcaa; /* bt/s 0x8c02fce6 */
L8c02fcaa: *stop=0x8c02fcaau;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdd4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fcac; /* mov.l 0x8c02fdd4,r3 */
L8c02fcac: *stop=0x8c02fcacu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fcae; /* mov.l @r15,r5 */
L8c02fcae: target=r[3];*pr=0x8c02fcb2u;*stop=0x8c02fcb0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fcb2; /* jsr @r3 */
L8c02fcb2: *stop=0x8c02fcb4u;goto L8c02fce6; /* bra 0x8c02fce6 */
L8c02fcb6: *stop=0x8c02fcb6u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fcb8; /* mov.l @(0x1c,r13),r2 */
L8c02fcb8: *stop=0x8c02fcb8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c02fcba; /* tst r2,r2 */
L8c02fcba: take=(*sr&1u);*stop=0x8c02fcbcu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c02fcc6;goto L8c02fcbe; /* bt/s 0x8c02fcc6 */
L8c02fcbe: *stop=0x8c02fcbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdd0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fcc0; /* mov.l 0x8c02fdd0,r3 */
L8c02fcc0: *stop=0x8c02fcc0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fcc2; /* mov.l @r15,r5 */
L8c02fcc2: target=r[3];*pr=0x8c02fcc6u;*stop=0x8c02fcc4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fcc6; /* jsr @r3 */
L8c02fcc6: *stop=0x8c02fcc6u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fcc8; /* mov.l @(0x14,r13),r3 */
L8c02fcc8: *stop=0x8c02fcc8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fcca; /* tst r3,r3 */
L8c02fcca: take=(*sr&1u);*stop=0x8c02fcccu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02fcd6;goto L8c02fcce; /* bt/s 0x8c02fcd6 */
L8c02fcce: *stop=0x8c02fcceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdd4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fcd0; /* mov.l 0x8c02fdd4,r3 */
L8c02fcd0: *stop=0x8c02fcd0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fcd2; /* mov.l @r15,r5 */
L8c02fcd2: target=r[3];*pr=0x8c02fcd6u;*stop=0x8c02fcd4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fcd6; /* jsr @r3 */
L8c02fcd6: *stop=0x8c02fcd6u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fcd8; /* mov.l @(0x18,r13),r3 */
L8c02fcd8: *stop=0x8c02fcd8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fcda; /* tst r3,r3 */
L8c02fcda: take=(*sr&1u);*stop=0x8c02fcdcu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02fce6;goto L8c02fcde; /* bt/s 0x8c02fce6 */
L8c02fcde: *stop=0x8c02fcdeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fce0; /* mov.l 0x8c02fdcc,r3 */
L8c02fce0: *stop=0x8c02fce0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fce2; /* mov.l @r15,r5 */
L8c02fce2: target=r[3];*pr=0x8c02fce6u;*stop=0x8c02fce4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fce6; /* jsr @r3 */
L8c02fce6: *stop=0x8c02fce6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdd8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fce8; /* mov.l 0x8c02fdd8,r3 */
L8c02fce8: *stop=0x8c02fce8u;r[5]=r[14];goto L8c02fcea; /* mov r14,r5 */
L8c02fcea: *stop=0x8c02fceau;r[6]=r[12];goto L8c02fcec; /* mov r12,r6 */
L8c02fcec: *stop=0x8c02fcecu;r[5]+=8u;goto L8c02fcee; /* add #0x8,r5 */
L8c02fcee: target=r[3];*pr=0x8c02fcf2u;*stop=0x8c02fcf0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fcf2; /* jsr @r3 */
L8c02fcf2: *stop=0x8c02fcf2u;r[4]=1u;goto L8c02fcf4; /* mov #0x1,r4 */
L8c02fcf4: *stop=0x8c02fcf4u;r[15]+=4u;goto L8c02fcf6; /* add #0x4,r15 */
L8c02fcf6: *stop=0x8c02fcf6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c02fcf8; /* lds.l @r15+,PR */
L8c02fcf8: *stop=0x8c02fcf8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fddcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fcfa; /* mov.l 0x8c02fddc,r2 */
L8c02fcfa: *stop=0x8c02fcfau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c02fcfc; /* mov.l @r15+,r12 */
L8c02fcfc: *stop=0x8c02fcfcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c02fcfe; /* mov.l @r15+,r13 */
L8c02fcfe: target=r[2];*stop=0x8c02fd00u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c02fd02: *stop=0x8c02fd02u;r[4]=r[13];goto L8c02fd04; /* mov r13,r4 */
L8c02fd04: *stop=0x8c02fd04u;r[4]+=32u;goto L8c02fd06; /* add #0x20,r4 */
L8c02fd06: *stop=0x8c02fd06u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02fd08; /* fmov.s @r4+,fr5 */
L8c02fd08: *stop=0x8c02fd08u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02fd0a; /* fmov.s @r4+,fr6 */
L8c02fd0a: *stop=0x8c02fd0au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c02fd0c; /* fmov.s @r4,fr4 */
L8c02fd0c: *stop=0x8c02fd0cu;r[4]=r[14];goto L8c02fd0e; /* mov r14,r4 */
L8c02fd0e: *stop=0x8c02fd0eu;r[4]+=8u;goto L8c02fd10; /* add #0x8,r4 */
L8c02fd10: *stop=0x8c02fd10u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02fd12; /* fmov.s @r4+,fr7 */
L8c02fd12: *stop=0x8c02fd12u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02fd14; /* fmov.s @r4+,fr8 */
L8c02fd14: *stop=0x8c02fd14u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[7],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c02fd16; /* fadd fr7,fr5 */
L8c02fd16: *stop=0x8c02fd16u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c02fd18; /* fmov.s @r4,fr9 */
L8c02fd18: *stop=0x8c02fd18u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[8],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c02fd1a; /* fadd fr8,fr6 */
L8c02fd1a: *stop=0x8c02fd1au;r[4]=r[12];goto L8c02fd1c; /* mov r12,r4 */
L8c02fd1c: *stop=0x8c02fd1cu;r[4]+=12u;goto L8c02fd1e; /* add #0xc,r4 */
L8c02fd1e: *stop=0x8c02fd1eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[9],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c02fd20; /* fadd fr9,fr4 */
L8c02fd20: *stop=0x8c02fd20u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c02fd22; /* fmov.s fr4,@-r4 */
L8c02fd22: *stop=0x8c02fd22u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c02fd24; /* fmov.s fr6,@-r4 */
L8c02fd24: *stop=0x8c02fd24u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c02fd26; /* fmov.s fr5,@-r4 */
L8c02fd26: *stop=0x8c02fd26u;r[15]+=4u;goto L8c02fd28; /* add #0x4,r15 */
L8c02fd28: *stop=0x8c02fd28u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c02fd2a; /* lds.l @r15+,PR */
L8c02fd2a: *stop=0x8c02fd2au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c02fd2c; /* mov.l @r15+,r12 */
L8c02fd2c: *stop=0x8c02fd2cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c02fd2e; /* mov.l @r15+,r13 */
L8c02fd2e: *stop=0x8c02fd2eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c02fd30u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_movement_shapes_8c02fc12(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fc12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fc72(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fc12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fc72u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fc7c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fc12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fc7cu);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fc92(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fc12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fc92u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fca2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fc12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fca2u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fcb2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fc12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fcb2u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fcc6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fc12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fcc6u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fcd6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fc12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fcd6u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fce6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fc12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fce6u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fcf2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fc12(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fcf2u);}

static int sa_player_movement_shapes_run_8c02fd32(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c02fd76u)goto L8c02fd76;if(resume_pc==0x8c02fd80u)goto L8c02fd80;if(resume_pc==0x8c02fd96u)goto L8c02fd96;if(resume_pc==0x8c02fda6u)goto L8c02fda6;if(resume_pc==0x8c02fdb6u)goto L8c02fdb6;if(resume_pc==0x8c02fdf0u)goto L8c02fdf0;if(resume_pc==0x8c02fe00u)goto L8c02fe00;if(resume_pc==0x8c02fe10u)goto L8c02fe10;if(resume_pc==0x8c02fe1cu)goto L8c02fe1c;goto L8c02fd32;
L8c02fd32: *stop=0x8c02fd32u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c02fd34; /* mov.l r14,@-r15 */
L8c02fd34: *stop=0x8c02fd34u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c02fd36; /* mov.l r13,@-r15 */
L8c02fd36: *stop=0x8c02fd36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c02fd38; /* mov.l r12,@-r15 */
L8c02fd38: *stop=0x8c02fd38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c02fd3a; /* sts.l PR,@-r15 */
L8c02fd3a: *stop=0x8c02fd3au;r[15]+=4294967292u;goto L8c02fd3c; /* add #-0x4,r15 */
L8c02fd3c: *stop=0x8c02fd3cu;r[14]=r[5];goto L8c02fd3e; /* mov r5,r14 */
L8c02fd3e: *stop=0x8c02fd3eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02fd40; /* mov.l @r14,r0 */
L8c02fd40: *stop=0x8c02fd40u;r[13]=r[4];goto L8c02fd42; /* mov r4,r13 */
L8c02fd42: *stop=0x8c02fd42u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c02fd44; /* tst #0x20,r0 */
L8c02fd44: take=(*sr&1u);*stop=0x8c02fd46u;r[12]=r[6];if(take)goto L8c02fd60;goto L8c02fd48; /* bt/s 0x8c02fd60 */
L8c02fd48: *stop=0x8c02fd48u;r[0]=8u;goto L8c02fd4a; /* mov #0x8,r0 */
L8c02fd4a: *stop=0x8c02fd4au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02fd4c; /* fmov.s @(r0,r14),fr3 */
L8c02fd4c: *stop=0x8c02fd4cu;r[0]=12u;goto L8c02fd4e; /* mov #0xc,r0 */
L8c02fd4e: *stop=0x8c02fd4eu;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c02fd50; /* fmov.s fr3,@r12 */
L8c02fd50: *stop=0x8c02fd50u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02fd52; /* fmov.s @(r0,r14),fr3 */
L8c02fd52: *stop=0x8c02fd52u;r[0]=4u;goto L8c02fd54; /* mov #0x4,r0 */
L8c02fd54: *stop=0x8c02fd54u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c02fd56; /* fmov fr3,@(r0,r12) */
L8c02fd56: *stop=0x8c02fd56u;r[0]=16u;goto L8c02fd58; /* mov #0x10,r0 */
L8c02fd58: *stop=0x8c02fd58u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02fd5a; /* fmov.s @(r0,r14),fr3 */
L8c02fd5a: *stop=0x8c02fd5au;r[0]=8u;goto L8c02fd5c; /* mov #0x8,r0 */
L8c02fd5c: *stop=0x8c02fd5eu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c02fe50; /* bra 0x8c02fe50 */
L8c02fd60: *stop=0x8c02fd60u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fd62; /* mov.l @r14,r2 */
L8c02fd62: *stop=0x8c02fd62u;r[3]=32u;goto L8c02fd64; /* mov #0x20,r3 */
L8c02fd64: *stop=0x8c02fd64u;r[3]<<=8;goto L8c02fd66; /* shll8 r3 */
L8c02fd66: *stop=0x8c02fd66u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c02fd68; /* tst r3,r2 */
L8c02fd68: if((*sr&1u))goto L8c02fd6e;goto L8c02fd6a; /* bt 0x8c02fd6e */
L8c02fd6a: *stop=0x8c02fd6cu;goto L8c02fe2c; /* bra 0x8c02fe2c */
L8c02fd6e: *stop=0x8c02fd6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdc4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fd70; /* mov.l 0x8c02fdc4,r2 */
L8c02fd70: *stop=0x8c02fd70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdc0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c02fd72; /* mov.l 0x8c02fdc0,r4 */
L8c02fd72: target=r[2];*pr=0x8c02fd76u;*stop=0x8c02fd74u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fd76; /* jsr @r2 */
L8c02fd76: *stop=0x8c02fd76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdc8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fd78; /* mov.l 0x8c02fdc8,r3 */
L8c02fd78: *stop=0x8c02fd78u;r[5]=r[13];goto L8c02fd7a; /* mov r13,r5 */
L8c02fd7a: *stop=0x8c02fd7au;r[5]+=32u;goto L8c02fd7c; /* add #0x20,r5 */
L8c02fd7c: target=r[3];*pr=0x8c02fd80u;*stop=0x8c02fd7eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fd80; /* jsr @r3 */
L8c02fd80: *stop=0x8c02fd80u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02fd82; /* mov.l @r14,r0 */
L8c02fd82: *stop=0x8c02fd82u;*sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c02fd84; /* tst #0x40,r0 */
L8c02fd84: if((*sr&1u))goto L8c02fde0;goto L8c02fd86; /* bt 0x8c02fde0 */
L8c02fd86: *stop=0x8c02fd86u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fd88; /* mov.l @(0x18,r13),r3 */
L8c02fd88: *stop=0x8c02fd88u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fd8a; /* tst r3,r3 */
L8c02fd8a: take=(*sr&1u);*stop=0x8c02fd8cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02fd96;goto L8c02fd8e; /* bt/s 0x8c02fd96 */
L8c02fd8e: *stop=0x8c02fd8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fd90; /* mov.l 0x8c02fdcc,r3 */
L8c02fd90: *stop=0x8c02fd90u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fd92; /* mov.l @r15,r5 */
L8c02fd92: target=r[3];*pr=0x8c02fd96u;*stop=0x8c02fd94u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fd96; /* jsr @r3 */
L8c02fd96: *stop=0x8c02fd96u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fd98; /* mov.l @(0x1c,r13),r3 */
L8c02fd98: *stop=0x8c02fd98u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fd9a; /* tst r3,r3 */
L8c02fd9a: take=(*sr&1u);*stop=0x8c02fd9cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02fda6;goto L8c02fd9e; /* bt/s 0x8c02fda6 */
L8c02fd9e: *stop=0x8c02fd9eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdd0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fda0; /* mov.l 0x8c02fdd0,r3 */
L8c02fda0: *stop=0x8c02fda0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fda2; /* mov.l @r15,r5 */
L8c02fda2: target=r[3];*pr=0x8c02fda6u;*stop=0x8c02fda4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fda6; /* jsr @r3 */
L8c02fda6: *stop=0x8c02fda6u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fda8; /* mov.l @(0x14,r13),r3 */
L8c02fda8: *stop=0x8c02fda8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fdaa; /* tst r3,r3 */
L8c02fdaa: take=(*sr&1u);*stop=0x8c02fdacu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02fe10;goto L8c02fdae; /* bt/s 0x8c02fe10 */
L8c02fdae: *stop=0x8c02fdaeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02fdd4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fdb0; /* mov.l 0x8c02fdd4,r3 */
L8c02fdb0: *stop=0x8c02fdb0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fdb2; /* mov.l @r15,r5 */
L8c02fdb2: target=r[3];*pr=0x8c02fdb6u;*stop=0x8c02fdb4u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fdb6; /* jsr @r3 */
L8c02fdb6: *stop=0x8c02fdb8u;goto L8c02fe10; /* bra 0x8c02fe10 */
L8c02fde0: *stop=0x8c02fde0u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fde2; /* mov.l @(0x1c,r13),r2 */
L8c02fde2: *stop=0x8c02fde2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c02fde4; /* tst r2,r2 */
L8c02fde4: take=(*sr&1u);*stop=0x8c02fde6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c02fdf0;goto L8c02fde8; /* bt/s 0x8c02fdf0 */
L8c02fde8: *stop=0x8c02fde8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fdea; /* mov.l 0x8c02ff40,r3 */
L8c02fdea: *stop=0x8c02fdeau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fdec; /* mov.l @r15,r5 */
L8c02fdec: target=r[3];*pr=0x8c02fdf0u;*stop=0x8c02fdeeu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fdf0; /* jsr @r3 */
L8c02fdf0: *stop=0x8c02fdf0u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fdf2; /* mov.l @(0x14,r13),r3 */
L8c02fdf2: *stop=0x8c02fdf2u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fdf4; /* tst r3,r3 */
L8c02fdf4: take=(*sr&1u);*stop=0x8c02fdf6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02fe00;goto L8c02fdf8; /* bt/s 0x8c02fe00 */
L8c02fdf8: *stop=0x8c02fdf8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fdfa; /* mov.l 0x8c02ff44,r3 */
L8c02fdfa: *stop=0x8c02fdfau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fdfc; /* mov.l @r15,r5 */
L8c02fdfc: target=r[3];*pr=0x8c02fe00u;*stop=0x8c02fdfeu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fe00; /* jsr @r3 */
L8c02fe00: *stop=0x8c02fe00u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fe02; /* mov.l @(0x18,r13),r3 */
L8c02fe02: *stop=0x8c02fe02u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fe04; /* tst r3,r3 */
L8c02fe04: take=(*sr&1u);*stop=0x8c02fe06u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02fe10;goto L8c02fe08; /* bt/s 0x8c02fe10 */
L8c02fe08: *stop=0x8c02fe08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fe0a; /* mov.l 0x8c02ff48,r3 */
L8c02fe0a: *stop=0x8c02fe0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fe0c; /* mov.l @r15,r5 */
L8c02fe0c: target=r[3];*pr=0x8c02fe10u;*stop=0x8c02fe0eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fe10; /* jsr @r3 */
L8c02fe10: *stop=0x8c02fe10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff4cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fe12; /* mov.l 0x8c02ff4c,r3 */
L8c02fe12: *stop=0x8c02fe12u;r[5]=r[14];goto L8c02fe14; /* mov r14,r5 */
L8c02fe14: *stop=0x8c02fe14u;r[6]=r[12];goto L8c02fe16; /* mov r12,r6 */
L8c02fe16: *stop=0x8c02fe16u;r[5]+=8u;goto L8c02fe18; /* add #0x8,r5 */
L8c02fe18: target=r[3];*pr=0x8c02fe1cu;*stop=0x8c02fe1au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fe1c; /* jsr @r3 */
L8c02fe1c: *stop=0x8c02fe1cu;r[4]=1u;goto L8c02fe1e; /* mov #0x1,r4 */
L8c02fe1e: *stop=0x8c02fe1eu;r[15]+=4u;goto L8c02fe20; /* add #0x4,r15 */
L8c02fe20: *stop=0x8c02fe20u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c02fe22; /* lds.l @r15+,PR */
L8c02fe22: *stop=0x8c02fe22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff50u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fe24; /* mov.l 0x8c02ff50,r2 */
L8c02fe24: *stop=0x8c02fe24u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c02fe26; /* mov.l @r15+,r12 */
L8c02fe26: *stop=0x8c02fe26u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c02fe28; /* mov.l @r15+,r13 */
L8c02fe28: target=r[2];*stop=0x8c02fe2au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c02fe2c: *stop=0x8c02fe2cu;r[4]=r[13];goto L8c02fe2e; /* mov r13,r4 */
L8c02fe2e: *stop=0x8c02fe2eu;r[4]+=32u;goto L8c02fe30; /* add #0x20,r4 */
L8c02fe30: *stop=0x8c02fe30u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02fe32; /* fmov.s @r4+,fr5 */
L8c02fe32: *stop=0x8c02fe32u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02fe34; /* fmov.s @r4+,fr6 */
L8c02fe34: *stop=0x8c02fe34u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c02fe36; /* fmov.s @r4,fr4 */
L8c02fe36: *stop=0x8c02fe36u;r[4]=r[14];goto L8c02fe38; /* mov r14,r4 */
L8c02fe38: *stop=0x8c02fe38u;r[4]+=8u;goto L8c02fe3a; /* add #0x8,r4 */
L8c02fe3a: *stop=0x8c02fe3au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02fe3c; /* fmov.s @r4+,fr7 */
L8c02fe3c: *stop=0x8c02fe3cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02fe3e; /* fmov.s @r4+,fr8 */
L8c02fe3e: *stop=0x8c02fe3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[7],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c02fe40; /* fadd fr7,fr5 */
L8c02fe40: *stop=0x8c02fe40u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c02fe42; /* fmov.s @r4,fr9 */
L8c02fe42: *stop=0x8c02fe42u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[8],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c02fe44; /* fadd fr8,fr6 */
L8c02fe44: *stop=0x8c02fe44u;r[4]=r[12];goto L8c02fe46; /* mov r12,r4 */
L8c02fe46: *stop=0x8c02fe46u;r[4]+=12u;goto L8c02fe48; /* add #0xc,r4 */
L8c02fe48: *stop=0x8c02fe48u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[9],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c02fe4a; /* fadd fr9,fr4 */
L8c02fe4a: *stop=0x8c02fe4au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c02fe4c; /* fmov.s fr4,@-r4 */
L8c02fe4c: *stop=0x8c02fe4cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c02fe4e; /* fmov.s fr6,@-r4 */
L8c02fe4e: *stop=0x8c02fe4eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c02fe50; /* fmov.s fr5,@-r4 */
L8c02fe50: *stop=0x8c02fe50u;r[15]+=4u;goto L8c02fe52; /* add #0x4,r15 */
L8c02fe52: *stop=0x8c02fe52u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c02fe54; /* lds.l @r15+,PR */
L8c02fe54: *stop=0x8c02fe54u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c02fe56; /* mov.l @r15+,r12 */
L8c02fe56: *stop=0x8c02fe56u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c02fe58; /* mov.l @r15+,r13 */
L8c02fe58: *stop=0x8c02fe58u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c02fe5au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_movement_shapes_8c02fd32(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fd32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fd76(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fd32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fd76u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fd80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fd32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fd80u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fd96(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fd32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fd96u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fda6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fd32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fda6u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fdb6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fd32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fdb6u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fdf0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fd32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fdf0u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fe00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fd32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fe00u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fe10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fd32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fe10u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fe1c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fd32(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fe1cu);}

static int sa_player_movement_shapes_run_8c02fe5c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c02fe92u)goto L8c02fe92;if(resume_pc==0x8c02fe9cu)goto L8c02fe9c;if(resume_pc==0x8c02febcu)goto L8c02febc;if(resume_pc==0x8c02feccu)goto L8c02fecc;if(resume_pc==0x8c02fedcu)goto L8c02fedc;if(resume_pc==0x8c02fef0u)goto L8c02fef0;if(resume_pc==0x8c02ff00u)goto L8c02ff00;if(resume_pc==0x8c02ff10u)goto L8c02ff10;if(resume_pc==0x8c02ff1cu)goto L8c02ff1c;goto L8c02fe5c;
L8c02fe5c: *stop=0x8c02fe5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c02fe5e; /* mov.l r14,@-r15 */
L8c02fe5e: *stop=0x8c02fe5eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c02fe60; /* mov.l r13,@-r15 */
L8c02fe60: *stop=0x8c02fe60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c02fe62; /* mov.l r12,@-r15 */
L8c02fe62: *stop=0x8c02fe62u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c02fe64; /* sts.l PR,@-r15 */
L8c02fe64: *stop=0x8c02fe64u;r[15]+=4294967292u;goto L8c02fe66; /* add #-0x4,r15 */
L8c02fe66: *stop=0x8c02fe66u;r[14]=r[5];goto L8c02fe68; /* mov r5,r14 */
L8c02fe68: *stop=0x8c02fe68u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02fe6a; /* mov.l @r14,r0 */
L8c02fe6a: *stop=0x8c02fe6au;r[13]=r[4];goto L8c02fe6c; /* mov r4,r13 */
L8c02fe6c: *stop=0x8c02fe6cu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c02fe6e; /* tst #0x20,r0 */
L8c02fe6e: take=(*sr&1u);*stop=0x8c02fe70u;r[12]=r[6];if(take)goto L8c02fe8a;goto L8c02fe72; /* bt/s 0x8c02fe8a */
L8c02fe72: *stop=0x8c02fe72u;r[0]=8u;goto L8c02fe74; /* mov #0x8,r0 */
L8c02fe74: *stop=0x8c02fe74u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02fe76; /* fmov.s @(r0,r14),fr3 */
L8c02fe76: *stop=0x8c02fe76u;r[0]=12u;goto L8c02fe78; /* mov #0xc,r0 */
L8c02fe78: *stop=0x8c02fe78u;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c02fe7a; /* fmov.s fr3,@r12 */
L8c02fe7a: *stop=0x8c02fe7au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02fe7c; /* fmov.s @(r0,r14),fr3 */
L8c02fe7c: *stop=0x8c02fe7cu;r[0]=4u;goto L8c02fe7e; /* mov #0x4,r0 */
L8c02fe7e: *stop=0x8c02fe7eu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c02fe80; /* fmov fr3,@(r0,r12) */
L8c02fe80: *stop=0x8c02fe80u;r[0]=16u;goto L8c02fe82; /* mov #0x10,r0 */
L8c02fe82: *stop=0x8c02fe82u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02fe84; /* fmov.s @(r0,r14),fr3 */
L8c02fe84: *stop=0x8c02fe84u;r[0]=8u;goto L8c02fe86; /* mov #0x8,r0 */
L8c02fe86: *stop=0x8c02fe88u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c02ff2c; /* bra 0x8c02ff2c */
L8c02fe8a: *stop=0x8c02fe8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fe8c; /* mov.l 0x8c02ff58,r2 */
L8c02fe8c: *stop=0x8c02fe8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff54u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c02fe8e; /* mov.l 0x8c02ff54,r4 */
L8c02fe8e: target=r[2];*pr=0x8c02fe92u;*stop=0x8c02fe90u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fe92; /* jsr @r2 */
L8c02fe92: *stop=0x8c02fe92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff5cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fe94; /* mov.l 0x8c02ff5c,r3 */
L8c02fe94: *stop=0x8c02fe94u;r[5]=r[13];goto L8c02fe96; /* mov r13,r5 */
L8c02fe96: *stop=0x8c02fe96u;r[5]+=32u;goto L8c02fe98; /* add #0x20,r5 */
L8c02fe98: target=r[3];*pr=0x8c02fe9cu;*stop=0x8c02fe9au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fe9c; /* jsr @r3 */
L8c02fe9c: *stop=0x8c02fe9cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fe9e; /* mov.l @r14,r2 */
L8c02fe9e: *stop=0x8c02fe9eu;r[3]=32u;goto L8c02fea0; /* mov #0x20,r3 */
L8c02fea0: *stop=0x8c02fea0u;r[3]<<=8;goto L8c02fea2; /* shll8 r3 */
L8c02fea2: *stop=0x8c02fea2u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c02fea4; /* tst r3,r2 */
L8c02fea4: if(!(*sr&1u))goto L8c02ff10;goto L8c02fea6; /* bf 0x8c02ff10 */
L8c02fea6: *stop=0x8c02fea6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02fea8; /* mov.l @r14,r0 */
L8c02fea8: *stop=0x8c02fea8u;*sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c02feaa; /* tst #0x40,r0 */
L8c02feaa: if((*sr&1u))goto L8c02fee0;goto L8c02feac; /* bt 0x8c02fee0 */
L8c02feac: *stop=0x8c02feacu;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02feae; /* mov.l @(0x18,r13),r1 */
L8c02feae: *stop=0x8c02feaeu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c02feb0; /* tst r1,r1 */
L8c02feb0: take=(*sr&1u);*stop=0x8c02feb2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;if(take)goto L8c02febc;goto L8c02feb4; /* bt/s 0x8c02febc */
L8c02feb4: *stop=0x8c02feb4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02feb6; /* mov.l 0x8c02ff48,r2 */
L8c02feb6: *stop=0x8c02feb6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02feb8; /* mov.l @r15,r5 */
L8c02feb8: target=r[2];*pr=0x8c02febcu;*stop=0x8c02febau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02febc; /* jsr @r2 */
L8c02febc: *stop=0x8c02febcu;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02febe; /* mov.l @(0x1c,r13),r2 */
L8c02febe: *stop=0x8c02febeu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c02fec0; /* tst r2,r2 */
L8c02fec0: take=(*sr&1u);*stop=0x8c02fec2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c02fecc;goto L8c02fec4; /* bt/s 0x8c02fecc */
L8c02fec4: *stop=0x8c02fec4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fec6; /* mov.l 0x8c02ff40,r3 */
L8c02fec6: *stop=0x8c02fec6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fec8; /* mov.l @r15,r5 */
L8c02fec8: target=r[3];*pr=0x8c02feccu;*stop=0x8c02fecau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fecc; /* jsr @r3 */
L8c02fecc: *stop=0x8c02feccu;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fece; /* mov.l @(0x14,r13),r3 */
L8c02fece: *stop=0x8c02feceu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02fed0; /* tst r3,r3 */
L8c02fed0: take=(*sr&1u);*stop=0x8c02fed2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02ff10;goto L8c02fed4; /* bt/s 0x8c02ff10 */
L8c02fed4: *stop=0x8c02fed4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fed6; /* mov.l 0x8c02ff44,r3 */
L8c02fed6: *stop=0x8c02fed6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fed8; /* mov.l @r15,r5 */
L8c02fed8: target=r[3];*pr=0x8c02fedcu;*stop=0x8c02fedau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fedc; /* jsr @r3 */
L8c02fedc: *stop=0x8c02fedeu;goto L8c02ff10; /* bra 0x8c02ff10 */
L8c02fee0: *stop=0x8c02fee0u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fee2; /* mov.l @(0x1c,r13),r2 */
L8c02fee2: *stop=0x8c02fee2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c02fee4; /* tst r2,r2 */
L8c02fee4: take=(*sr&1u);*stop=0x8c02fee6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c02fef0;goto L8c02fee8; /* bt/s 0x8c02fef0 */
L8c02fee8: *stop=0x8c02fee8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff40u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02feea; /* mov.l 0x8c02ff40,r2 */
L8c02feea: *stop=0x8c02feeau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02feec; /* mov.l @r15,r5 */
L8c02feec: target=r[2];*pr=0x8c02fef0u;*stop=0x8c02feeeu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02fef0; /* jsr @r2 */
L8c02fef0: *stop=0x8c02fef0u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02fef2; /* mov.l @(0x14,r13),r2 */
L8c02fef2: *stop=0x8c02fef2u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c02fef4; /* tst r2,r2 */
L8c02fef4: take=(*sr&1u);*stop=0x8c02fef6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c02ff00;goto L8c02fef8; /* bt/s 0x8c02ff00 */
L8c02fef8: *stop=0x8c02fef8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02fefa; /* mov.l 0x8c02ff44,r3 */
L8c02fefa: *stop=0x8c02fefau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02fefc; /* mov.l @r15,r5 */
L8c02fefc: target=r[3];*pr=0x8c02ff00u;*stop=0x8c02fefeu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02ff00; /* jsr @r3 */
L8c02ff00: *stop=0x8c02ff00u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02ff02; /* mov.l @(0x18,r13),r3 */
L8c02ff02: *stop=0x8c02ff02u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02ff04; /* tst r3,r3 */
L8c02ff04: take=(*sr&1u);*stop=0x8c02ff06u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c02ff10;goto L8c02ff08; /* bt/s 0x8c02ff10 */
L8c02ff08: *stop=0x8c02ff08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02ff0a; /* mov.l 0x8c02ff48,r3 */
L8c02ff0a: *stop=0x8c02ff0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02ff0c; /* mov.l @r15,r5 */
L8c02ff0c: target=r[3];*pr=0x8c02ff10u;*stop=0x8c02ff0eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02ff10; /* jsr @r3 */
L8c02ff10: *stop=0x8c02ff10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff4cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02ff12; /* mov.l 0x8c02ff4c,r3 */
L8c02ff12: *stop=0x8c02ff12u;r[5]=r[14];goto L8c02ff14; /* mov r14,r5 */
L8c02ff14: *stop=0x8c02ff14u;r[6]=r[12];goto L8c02ff16; /* mov r12,r6 */
L8c02ff16: *stop=0x8c02ff16u;r[5]+=8u;goto L8c02ff18; /* add #0x8,r5 */
L8c02ff18: target=r[3];*pr=0x8c02ff1cu;*stop=0x8c02ff1au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02ff1c; /* jsr @r3 */
L8c02ff1c: *stop=0x8c02ff1cu;r[4]=1u;goto L8c02ff1e; /* mov #0x1,r4 */
L8c02ff1e: *stop=0x8c02ff1eu;r[15]+=4u;goto L8c02ff20; /* add #0x4,r15 */
L8c02ff20: *stop=0x8c02ff20u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c02ff22; /* lds.l @r15+,PR */
L8c02ff22: *stop=0x8c02ff22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02ff50u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02ff24; /* mov.l 0x8c02ff50,r2 */
L8c02ff24: *stop=0x8c02ff24u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c02ff26; /* mov.l @r15+,r12 */
L8c02ff26: *stop=0x8c02ff26u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c02ff28; /* mov.l @r15+,r13 */
L8c02ff28: target=r[2];*stop=0x8c02ff2au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c02ff2c: *stop=0x8c02ff2cu;r[15]+=4u;goto L8c02ff2e; /* add #0x4,r15 */
L8c02ff2e: *stop=0x8c02ff2eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c02ff30; /* lds.l @r15+,PR */
L8c02ff30: *stop=0x8c02ff30u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c02ff32; /* mov.l @r15+,r12 */
L8c02ff32: *stop=0x8c02ff32u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c02ff34; /* mov.l @r15+,r13 */
L8c02ff34: *stop=0x8c02ff34u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c02ff36u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_movement_shapes_8c02fe5c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fe5c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fe92(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fe5c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fe92u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fe9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fe5c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fe9cu);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02febc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fe5c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02febcu);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fecc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fe5c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02feccu);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fedc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fe5c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fedcu);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02fef0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fe5c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02fef0u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02ff00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fe5c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02ff00u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02ff10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fe5c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02ff10u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c02ff1c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c02fe5c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02ff1cu);}

static int sa_player_movement_shapes_run_8c031a84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c031ab6u)goto L8c031ab6;if(resume_pc==0x8c031ac4u)goto L8c031ac4;if(resume_pc==0x8c031ad2u)goto L8c031ad2;if(resume_pc==0x8c031ad8u)goto L8c031ad8;goto L8c031a84;
L8c031a84: *stop=0x8c031a84u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c031a86; /* mov.l r14,@-r15 */
L8c031a86: *stop=0x8c031a86u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c031a88; /* sts.l PR,@-r15 */
L8c031a88: *stop=0x8c031a88u;r[15]+=4294967268u;goto L8c031a8a; /* add #-0x1c,r15 */
L8c031a8a: *stop=0x8c031a8au;r[0]=16u;goto L8c031a8c; /* mov #0x10,r0 */
L8c031a8c: *stop=0x8c031a8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c031b04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c031a8e; /* mov.l 0x8c031b04,r3 */
L8c031a8e: *stop=0x8c031a8eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c031a90; /* mov.l r6,@r15 */
L8c031a90: *stop=0x8c031a90u;r[14]=r[7];goto L8c031a92; /* mov r7,r14 */
L8c031a92: *stop=0x8c031a92u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c031a94; /* fmov.s @r5,fr3 */
L8c031a94: *stop=0x8c031a94u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c031a96; /* fmov.s @r4,fr2 */
L8c031a96: *stop=0x8c031a96u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c031a98; /* fsub fr3,fr2 */
L8c031a98: *stop=0x8c031a98u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c031a9a; /* fmov fr2,@(r0,r15) */
L8c031a9a: *stop=0x8c031a9au;r[0]=4u;goto L8c031a9c; /* mov #0x4,r0 */
L8c031a9c: *stop=0x8c031a9cu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c031a9e; /* fmov.s @(r0,r5),fr3 */
L8c031a9e: *stop=0x8c031a9eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c031aa0; /* fmov.s @(r0,r4),fr2 */
L8c031aa0: *stop=0x8c031aa0u;r[0]=20u;goto L8c031aa2; /* mov #0x14,r0 */
L8c031aa2: *stop=0x8c031aa2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c031aa4; /* fsub fr3,fr2 */
L8c031aa4: *stop=0x8c031aa4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c031aa6; /* fmov fr2,@(r0,r15) */
L8c031aa6: *stop=0x8c031aa6u;r[0]=8u;goto L8c031aa8; /* mov #0x8,r0 */
L8c031aa8: *stop=0x8c031aa8u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c031aaa; /* fmov.s @(r0,r5),fr3 */
L8c031aaa: *stop=0x8c031aaau;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c031aac; /* fmov.s @(r0,r4),fr2 */
L8c031aac: *stop=0x8c031aacu;r[0]=24u;goto L8c031aae; /* mov #0x18,r0 */
L8c031aae: *stop=0x8c031aaeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c031b00u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c031ab0; /* mov.l 0x8c031b00,r4 */
L8c031ab0: *stop=0x8c031ab0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c031ab2; /* fsub fr3,fr2 */
L8c031ab2: target=r[3];*pr=0x8c031ab6u;*stop=0x8c031ab4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c031ab6; /* jsr @r3 */
L8c031ab6: *stop=0x8c031ab6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c031ab8; /* mov.l @r15,r2 */
L8c031ab8: *stop=0x8c031ab8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c031aba; /* tst r2,r2 */
L8c031aba: if((*sr&1u))goto L8c031ac4;goto L8c031abc; /* bt 0x8c031ac4 */
L8c031abc: *stop=0x8c031abcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c031b08u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c031abe; /* mov.l 0x8c031b08,r3 */
L8c031abe: *stop=0x8c031abeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c031ac0; /* mov.l @r15,r5 */
L8c031ac0: target=r[3];*pr=0x8c031ac4u;*stop=0x8c031ac2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c031ac4; /* jsr @r3 */
L8c031ac4: *stop=0x8c031ac4u;r[5]=r[15];goto L8c031ac6; /* mov r15,r5 */
L8c031ac6: *stop=0x8c031ac6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c031b18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c031ac8; /* mov.l 0x8c031b18,r3 */
L8c031ac8: *stop=0x8c031ac8u;r[6]=r[15];goto L8c031aca; /* mov r15,r6 */
L8c031aca: *stop=0x8c031acau;r[5]+=16u;goto L8c031acc; /* add #0x10,r5 */
L8c031acc: *stop=0x8c031accu;r[6]+=4u;goto L8c031ace; /* add #0x4,r6 */
L8c031ace: target=r[3];*pr=0x8c031ad2u;*stop=0x8c031ad0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c031ad2; /* jsr @r3 */
L8c031ad2: *stop=0x8c031ad2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c031b10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c031ad4; /* mov.l 0x8c031b10,r2 */
L8c031ad4: target=r[2];*pr=0x8c031ad8u;*stop=0x8c031ad6u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c031ad8; /* jsr @r2 */
L8c031ad8: *stop=0x8c031ad8u;r[0]=4u;goto L8c031ada; /* mov #0x4,r0 */
L8c031ada: *stop=0x8c031adau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c031adc; /* fmov.s @(r0,r15),fr3 */
L8c031adc: *stop=0x8c031adcu;r[0]=8u;goto L8c031ade; /* mov #0x8,r0 */
L8c031ade: *stop=0x8c031adeu;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c031ae0; /* fmov.s fr3,@r14 */
L8c031ae0: *stop=0x8c031ae0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c031ae2; /* fmov.s @(r0,r15),fr3 */
L8c031ae2: *stop=0x8c031ae2u;r[0]=4u;goto L8c031ae4; /* mov #0x4,r0 */
L8c031ae4: *stop=0x8c031ae4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c031ae6; /* fmov fr3,@(r0,r14) */
L8c031ae6: *stop=0x8c031ae6u;r[0]=12u;goto L8c031ae8; /* mov #0xc,r0 */
L8c031ae8: *stop=0x8c031ae8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c031aea; /* fmov.s @(r0,r15),fr3 */
L8c031aea: *stop=0x8c031aeau;r[0]=8u;goto L8c031aec; /* mov #0x8,r0 */
L8c031aec: *stop=0x8c031aecu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c031aee; /* fmov fr3,@(r0,r14) */
L8c031aee: *stop=0x8c031aeeu;r[15]+=28u;goto L8c031af0; /* add #0x1c,r15 */
L8c031af0: *stop=0x8c031af0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c031af2; /* lds.l @r15+,PR */
L8c031af2: *stop=0x8c031af2u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c031af4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_movement_shapes_8c031a84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c031a84(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c031ab6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c031a84(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c031ab6u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c031ac4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c031a84(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c031ac4u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c031ad2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c031a84(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c031ad2u);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c031ad8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c031a84(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c031ad8u);}

static int sa_player_movement_shapes_run_8c043a40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c043a9cu)goto L8c043a9c;goto L8c043a40;
L8c043a40: *stop=0x8c043a40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c043a42; /* sts.l PR,@-r15 */
L8c043a42: *stop=0x8c043a42u;r[15]+=4294967248u;goto L8c043a44; /* add #-0x30,r15 */
L8c043a44: *stop=0x8c043a44u;r[6]=r[15];goto L8c043a46; /* mov r15,r6 */
L8c043a46: *stop=0x8c043a46u;r[6]+=8u;goto L8c043a48; /* add #0x8,r6 */
L8c043a48: *stop=0x8c043a48u;r[0]=4u;goto L8c043a4a; /* mov #0x4,r0 */
L8c043a4a: *stop=0x8c043a4au;r[4]=r[6];goto L8c043a4c; /* mov r6,r4 */
L8c043a4c: *stop=0x8c043a4cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c043a4e; /* fmov.s fr4,@r4 */
L8c043a4e: *stop=0x8c043a4eu;r[7]=r[15];goto L8c043a50; /* mov r15,r7 */
L8c043a50: *stop=0x8c043a50u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c043a52; /* fmov fr5,@(r0,r4) */
L8c043a52: *stop=0x8c043a52u;r[0]=8u;goto L8c043a54; /* mov #0x8,r0 */
L8c043a54: *stop=0x8c043a54u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,6u,r[0]+r[4]);if(io_status)return io_status;goto L8c043a56; /* fmov fr6,@(r0,r4) */
L8c043a56: *stop=0x8c043a56u;r[0]=4u;goto L8c043a58; /* mov #0x4,r0 */
L8c043a58: *stop=0x8c043a58u;r[4]+=12u;goto L8c043a5a; /* add #0xc,r4 */
L8c043a5a: *stop=0x8c043a5au;r[5]=r[7];goto L8c043a5c; /* mov r7,r5 */
L8c043a5c: *stop=0x8c043a5cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c043a5e; /* fmov.s fr7,@r4 */
L8c043a5e: *stop=0x8c043a5eu;r[3]=4294967227u;goto L8c043a60; /* mov #-0x45,r3 */
L8c043a60: *stop=0x8c043a60u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,8u,r[0]+r[4]);if(io_status)return io_status;goto L8c043a62; /* fmov fr8,@(r0,r4) */
L8c043a62: *stop=0x8c043a62u;r[0]=8u;goto L8c043a64; /* mov #0x8,r0 */
L8c043a64: *stop=0x8c043a64u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,9u,r[0]+r[4]);if(io_status)return io_status;goto L8c043a66; /* fmov fr9,@(r0,r4) */
L8c043a66: *stop=0x8c043a66u;r[4]=4294967295u;goto L8c043a68; /* mov #-0x1,r4 */
L8c043a68: *stop=0x8c043a68u;r[4]=r[4]&255u;goto L8c043a6a; /* extu.b r4,r4 */
L8c043a6a: *stop=0x8c043a6au;r[0]=r[4];goto L8c043a6c; /* mov r4,r0 */
L8c043a6c: *stop=0x8c043a6cu;if(!valid_access(r[5]+3u,1u))return -3;wr(m,bus,&io_status,r[5]+0x3u,r[0],1);if(io_status)return io_status;goto L8c043a6e; /* mov.b r0,@(0x3,r5) */
L8c043a6e: *stop=0x8c043a6eu;if(!valid_access(r[5]+2u,1u))return -3;wr(m,bus,&io_status,r[5]+0x2u,r[0],1);if(io_status)return io_status;goto L8c043a70; /* mov.b r0,@(0x2,r5) */
L8c043a70: *stop=0x8c043a70u;r[4]+=4294967177u;goto L8c043a72; /* add #-0x77,r4 */
L8c043a72: *stop=0x8c043a72u;if(!valid_access(r[5]+1u,1u))return -3;wr(m,bus,&io_status,r[5]+0x1u,r[0],1);if(io_status)return io_status;goto L8c043a74; /* mov.b r0,@(0x1,r5) */
L8c043a74: *stop=0x8c043a74u;r[3]=r[3]&255u;goto L8c043a76; /* extu.b r3,r3 */
L8c043a76: *stop=0x8c043a76u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[0],1);if(io_status)return io_status;goto L8c043a78; /* mov.b r0,@r5 */
L8c043a78: *stop=0x8c043a78u;r[5]+=4u;goto L8c043a7a; /* add #0x4,r5 */
L8c043a7a: *stop=0x8c043a7au;if(!valid_access(r[5]+3u,1u))return -3;wr(m,bus,&io_status,r[5]+0x3u,r[0],1);if(io_status)return io_status;goto L8c043a7c; /* mov.b r0,@(0x3,r5) */
L8c043a7c: *stop=0x8c043a7cu;r[0]=r[4];goto L8c043a7e; /* mov r4,r0 */
L8c043a7e: *stop=0x8c043a7eu;if(!valid_access(r[5]+2u,1u))return -3;wr(m,bus,&io_status,r[5]+0x2u,r[0],1);if(io_status)return io_status;goto L8c043a80; /* mov.b r0,@(0x2,r5) */
L8c043a80: *stop=0x8c043a80u;if(!valid_access(r[5]+1u,1u))return -3;wr(m,bus,&io_status,r[5]+0x1u,r[0],1);if(io_status)return io_status;goto L8c043a82; /* mov.b r0,@(0x1,r5) */
L8c043a82: *stop=0x8c043a82u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[3],1);if(io_status)return io_status;goto L8c043a84; /* mov.b r3,@r5 */
L8c043a84: *stop=0x8c043a84u;r[3]=0u;goto L8c043a86; /* mov #0x0,r3 */
L8c043a86: *stop=0x8c043a86u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[6],4);if(io_status)return io_status;goto L8c043a88; /* mov.l r6,@(0x20,r15) */
L8c043a88: *stop=0x8c043a88u;r[6]=r[3];goto L8c043a8a; /* mov r3,r6 */
L8c043a8a: *stop=0x8c043a8au;r[2]=2u;goto L8c043a8c; /* mov #0x2,r2 */
L8c043a8c: *stop=0x8c043a8cu;r[4]=r[15];goto L8c043a8e; /* mov r15,r4 */
L8c043a8e: *stop=0x8c043a8eu;if(!valid_access(r[15]+36u,4u))return -3;wr(m,bus,&io_status,r[15]+0x24u,r[7],4);if(io_status)return io_status;goto L8c043a90; /* mov.l r7,@(0x24,r15) */
L8c043a90: *stop=0x8c043a90u;if(!valid_access(r[15]+40u,4u))return -3;wr(m,bus,&io_status,r[15]+0x28u,r[3],4);if(io_status)return io_status;goto L8c043a92; /* mov.l r3,@(0x28,r15) */
L8c043a92: *stop=0x8c043a92u;r[5]=1u;goto L8c043a94; /* mov #0x1,r5 */
L8c043a94: *stop=0x8c043a94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c043be4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c043a96; /* mov.l 0x8c043be4,r3 */
L8c043a96: *stop=0x8c043a96u;if(!valid_access(r[15]+44u,4u))return -3;wr(m,bus,&io_status,r[15]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c043a98; /* mov.l r2,@(0x2c,r15) */
L8c043a98: target=r[3];*pr=0x8c043a9cu;*stop=0x8c043a9au;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c043a9c; /* jsr @r3 */
L8c043a9c: *stop=0x8c043a9cu;r[15]+=48u;goto L8c043a9e; /* add #0x30,r15 */
L8c043a9e: *stop=0x8c043a9eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c043aa0; /* lds.l @r15+,PR */
L8c043aa0: *stop=0x8c043aa0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c043aa2u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_movement_shapes_8c043a40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c043a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_movement_shapes_resume_8c043a9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_shapes_run_8c043a40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c043a9cu);}
