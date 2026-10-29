/* Original conditional heading/value query with optional outputs and actual stage fallback.
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
static int sa_movement_heading_query_run_8c06cd22(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c06cdb8u)goto L8c06cdb8;if(resume_pc==0x8c06cdcau)goto L8c06cdca;goto L8c06cd22;
L8c06cd22: *stop=0x8c06cd22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c06cd24; /* mov.l r14,@-r15 */
L8c06cd24: *stop=0x8c06cd24u;r[14]=r[4];goto L8c06cd26; /* mov r4,r14 */
L8c06cd26: *stop=0x8c06cd26u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c06cd28; /* mov.l r13,@-r15 */
L8c06cd28: *stop=0x8c06cd28u;r[3]=64u;goto L8c06cd2a; /* mov #0x40,r3 */
L8c06cd2a: *stop=0x8c06cd2au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c06cd2c; /* mov.l r12,@-r15 */
L8c06cd2c: *stop=0x8c06cd2cu;r[3]<<=8;goto L8c06cd2e; /* shll8 r3 */
L8c06cd2e: *stop=0x8c06cd2eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c06cd30; /* sts.l PR,@-r15 */
L8c06cd30: *stop=0x8c06cd30u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06cd32; /* mov.w @(0x4,r14),r0 */
L8c06cd32: *stop=0x8c06cd32u;r[12]=r[5];goto L8c06cd34; /* mov r5,r12 */
L8c06cd34: *stop=0x8c06cd34u;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c06cd36; /* tst r3,r0 */
L8c06cd36: take=(*sr&1u);*stop=0x8c06cd38u;r[13]=r[6];if(take)goto L8c06cd48;goto L8c06cd3a; /* bt/s 0x8c06cd48 */
L8c06cd3a: *stop=0x8c06cd3au;if(!valid_access(r[14]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x3cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c06cd3c; /* mov.l @(0x3c,r14),r1 */
L8c06cd3c: *stop=0x8c06cd3cu;r[0]=36u;goto L8c06cd3e; /* mov #0x24,r0 */
L8c06cd3e: *stop=0x8c06cd3eu;if(!valid_access(r[0]+r[1],1u))return -3;r[2]=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;goto L8c06cd40; /* mov.b @(r0,r1),r2 */
L8c06cd40: *stop=0x8c06cd40u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c06cd42; /* tst r2,r2 */
L8c06cd42: if((*sr&1u))goto L8c06cdea;goto L8c06cd44; /* bt 0x8c06cdea */
L8c06cd44: *stop=0x8c06cd46u;goto L8c06cde6; /* bra 0x8c06cde6 */
L8c06cd48: *stop=0x8c06cd48u;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c06cd4a; /* mov.b @(0x8,r14),r0 */
L8c06cd4a: *stop=0x8c06cd4au;r[1]=4u;goto L8c06cd4c; /* mov #0x4,r1 */
L8c06cd4c: *stop=0x8c06cd4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c06ce18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c06cd4e; /* mov.l 0x8c06ce18,r5 */
L8c06cd4e: *stop=0x8c06cd4eu;r[0]=r[0]&255u;goto L8c06cd50; /* extu.b r0,r0 */
L8c06cd50: *stop=0x8c06cd50u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c06cd52; /* fldi0 fr3 */
L8c06cd52: *stop=0x8c06cd52u;r[0]<<=2;goto L8c06cd54; /* shll2 r0 */
L8c06cd54: *stop=0x8c06cd54u;r[4]=r[5];goto L8c06cd56; /* mov r5,r4 */
L8c06cd56: *stop=0x8c06cd56u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c06cd58; /* shll r0 */
L8c06cd58: *stop=0x8c06cd58u;r[0]+=r[4];goto L8c06cd5a; /* add r4,r0 */
L8c06cd5a: *stop=0x8c06cd5au;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c06cd5c; /* fmov.s @(r0,r1),fr2 */
L8c06cd5c: *stop=0x8c06cd5cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c06cd5e; /* fcmp/eq fr3,fr2 */
L8c06cd5e: if((*sr&1u))goto L8c06cda2;goto L8c06cd60; /* bt 0x8c06cda2 */
L8c06cd60: *stop=0x8c06cd60u;r[3]=32u;goto L8c06cd62; /* mov #0x20,r3 */
L8c06cd62: *stop=0x8c06cd62u;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06cd64; /* mov.w @(0x4,r14),r0 */
L8c06cd64: *stop=0x8c06cd64u;r[3]<<=8;goto L8c06cd66; /* shll8 r3 */
L8c06cd66: *stop=0x8c06cd66u;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c06cd68; /* tst r3,r0 */
L8c06cd68: if(!(*sr&1u))goto L8c06cd7a;goto L8c06cd6a; /* bf 0x8c06cd7a */
L8c06cd6a: *stop=0x8c06cd6au;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c06cd6c; /* mov.b @(0x8,r14),r0 */
L8c06cd6c: *stop=0x8c06cd6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c06ce1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c06cd6e; /* mov.l 0x8c06ce1c,r1 */
L8c06cd6e: *stop=0x8c06cd6eu;r[0]=r[0]&255u;goto L8c06cd70; /* extu.b r0,r0 */
L8c06cd70: *stop=0x8c06cd70u;r[0]<<=2;goto L8c06cd72; /* shll2 r0 */
L8c06cd72: *stop=0x8c06cd72u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c06cd74; /* mov.l @(r0,r1),r2 */
L8c06cd74: *stop=0x8c06cd74u;if(!valid_access(r[2]+10u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0xau,2);if(io_status)return io_status;goto L8c06cd76; /* mov.w @(0xa,r2),r0 */
L8c06cd76: *stop=0x8c06cd76u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06cd78; /* tst r0,r0 */
L8c06cd78: if(!(*sr&1u))goto L8c06cdea;goto L8c06cd7a; /* bf 0x8c06cdea */
L8c06cd7a: *stop=0x8c06cd7au;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c06cd7c; /* tst r12,r12 */
L8c06cd7c: if((*sr&1u))goto L8c06cd8c;goto L8c06cd7e; /* bt 0x8c06cd8c */
L8c06cd7e: *stop=0x8c06cd7eu;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c06cd80; /* mov.b @(0x8,r14),r0 */
L8c06cd80: *stop=0x8c06cd80u;r[0]=r[0]&255u;goto L8c06cd82; /* extu.b r0,r0 */
L8c06cd82: *stop=0x8c06cd82u;r[0]<<=2;goto L8c06cd84; /* shll2 r0 */
L8c06cd84: *stop=0x8c06cd84u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c06cd86; /* shll r0 */
L8c06cd86: *stop=0x8c06cd86u;r[4]+=r[0];goto L8c06cd88; /* add r0,r4 */
L8c06cd88: *stop=0x8c06cd88u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c06cd8a; /* mov.l @r4,r3 */
L8c06cd8a: *stop=0x8c06cd8au;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;goto L8c06cd8c; /* mov.l r3,@r12 */
L8c06cd8c: *stop=0x8c06cd8cu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c06cd8e; /* tst r13,r13 */
L8c06cd8e: if((*sr&1u))goto L8c06cde6;goto L8c06cd90; /* bt 0x8c06cde6 */
L8c06cd90: *stop=0x8c06cd90u;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c06cd92; /* mov.b @(0x8,r14),r0 */
L8c06cd92: *stop=0x8c06cd92u;r[0]=r[0]&255u;goto L8c06cd94; /* extu.b r0,r0 */
L8c06cd94: *stop=0x8c06cd94u;r[0]<<=2;goto L8c06cd96; /* shll2 r0 */
L8c06cd96: *stop=0x8c06cd96u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c06cd98; /* shll r0 */
L8c06cd98: *stop=0x8c06cd98u;r[5]+=r[0];goto L8c06cd9a; /* add r0,r5 */
L8c06cd9a: *stop=0x8c06cd9au;r[0]=4u;goto L8c06cd9c; /* mov #0x4,r0 */
L8c06cd9c: *stop=0x8c06cd9cu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c06cd9e; /* fmov.s @(r0,r5),fr3 */
L8c06cd9e: *stop=0x8c06cda0u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c06cde6; /* bra 0x8c06cde6 */
L8c06cda2: *stop=0x8c06cda2u;if(!valid_access(r[14]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x9u,1);if(io_status)return io_status;goto L8c06cda4; /* mov.b @(0x9,r14),r0 */
L8c06cda4: *stop=0x8c06cda4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c06cda6; /* tst r0,r0 */
L8c06cda6: if(!(*sr&1u))goto L8c06cdea;goto L8c06cda8; /* bf 0x8c06cdea */
L8c06cda8: *stop=0x8c06cda8u;r[3]=1u;goto L8c06cdaa; /* mov #0x1,r3 */
L8c06cdaa: *stop=0x8c06cdaau;if(!valid_access(r[14]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x4u,2);if(io_status)return io_status;goto L8c06cdac; /* mov.w @(0x4,r14),r0 */
L8c06cdac: *stop=0x8c06cdacu;r[3]<<=8;goto L8c06cdae; /* shll8 r3 */
L8c06cdae: *stop=0x8c06cdaeu;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c06cdb0; /* tst r3,r0 */
L8c06cdb0: if((*sr&1u))goto L8c06cdea;goto L8c06cdb2; /* bt 0x8c06cdea */
L8c06cdb2: *stop=0x8c06cdb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06ce20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c06cdb4; /* mov.l 0x8c06ce20,r1 */
L8c06cdb4: target=r[1];*pr=0x8c06cdb8u;*stop=0x8c06cdb6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06cdb8; /* jsr @r1 */
L8c06cdb8: *stop=0x8c06cdb8u;r[3]=9u;goto L8c06cdba; /* mov #0x9,r3 */
L8c06cdba: *stop=0x8c06cdbau;r[3]<<=8;goto L8c06cdbc; /* shll8 r3 */
L8c06cdbc: *stop=0x8c06cdbcu;r[3]+=2u;goto L8c06cdbe; /* add #0x2,r3 */
L8c06cdbe: *stop=0x8c06cdbeu;r[0]=r[0]&65535u;goto L8c06cdc0; /* extu.w r0,r0 */
L8c06cdc0: *stop=0x8c06cdc0u;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c06cdc2; /* cmp/eq r3,r0 */
L8c06cdc2: if((*sr&1u))goto L8c06cdd6;goto L8c06cdc4; /* bt 0x8c06cdd6 */
L8c06cdc4: *stop=0x8c06cdc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06ce20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c06cdc6; /* mov.l 0x8c06ce20,r1 */
L8c06cdc6: target=r[1];*pr=0x8c06cdcau;*stop=0x8c06cdc8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c06cdca; /* jsr @r1 */
L8c06cdca: *stop=0x8c06cdcau;r[3]=9u;goto L8c06cdcc; /* mov #0x9,r3 */
L8c06cdcc: *stop=0x8c06cdccu;r[3]<<=8;goto L8c06cdce; /* shll8 r3 */
L8c06cdce: *stop=0x8c06cdceu;r[3]+=3u;goto L8c06cdd0; /* add #0x3,r3 */
L8c06cdd0: *stop=0x8c06cdd0u;r[0]=r[0]&65535u;goto L8c06cdd2; /* extu.w r0,r0 */
L8c06cdd2: *stop=0x8c06cdd2u;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c06cdd4; /* cmp/eq r3,r0 */
L8c06cdd4: if(!(*sr&1u))goto L8c06cdea;goto L8c06cdd6; /* bf 0x8c06cdea */
L8c06cdd6: *stop=0x8c06cdd6u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c06cdd8; /* tst r12,r12 */
L8c06cdd8: if((*sr&1u))goto L8c06cdde;goto L8c06cdda; /* bt 0x8c06cdde */
L8c06cdda: *stop=0x8c06cddau;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c06cddc; /* mov.l @(0x18,r14),r2 */
L8c06cddc: *stop=0x8c06cddcu;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[2],4);if(io_status)return io_status;goto L8c06cdde; /* mov.l r2,@r12 */
L8c06cdde: *stop=0x8c06cddeu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c06cde0; /* tst r13,r13 */
L8c06cde0: if((*sr&1u))goto L8c06cde6;goto L8c06cde2; /* bt 0x8c06cde6 */
L8c06cde2: *stop=0x8c06cde2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c06cde4; /* fldi0 fr3 */
L8c06cde4: *stop=0x8c06cde4u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c06cde6; /* fmov.s fr3,@r13 */
L8c06cde6: *stop=0x8c06cde8u;r[0]=1u;goto L8c06cdec; /* bra 0x8c06cdec */
L8c06cdea: *stop=0x8c06cdeau;r[0]=0u;goto L8c06cdec; /* mov #0x0,r0 */
L8c06cdec: *stop=0x8c06cdecu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c06cdee; /* lds.l @r15+,PR */
L8c06cdee: *stop=0x8c06cdeeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c06cdf0; /* mov.l @r15+,r12 */
L8c06cdf0: *stop=0x8c06cdf0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c06cdf2; /* mov.l @r15+,r13 */
L8c06cdf2: *stop=0x8c06cdf2u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c06cdf4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_heading_query_8c06cd22(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_heading_query_run_8c06cd22(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_heading_query_resume_8c06cdb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_heading_query_run_8c06cd22(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06cdb8u);}
SA_STAGE_EXPORT int sa_movement_heading_query_resume_8c06cdca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_heading_query_run_8c06cd22(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c06cdcau);}
