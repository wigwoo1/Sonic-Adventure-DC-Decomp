/* Original countdown, signed variant, input and audio work.
 * Static C from byte-checked SH-4 instructions; complete explicit state.
 * Missing callees remain reached boundaries; no callback result is substituted.
 * Context and interrupt entry remain explicit platform state.
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
SA_STAGE_EXPORT int sa_movie_countdown_8c051810(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c051810;
L8c051810: *stop=0x8c051810u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c051812; /* sts.l PR,@-r15 */
L8c051812: *stop=0x8c051812u;r[15]+=4294967288u;goto L8c051814; /* add #-0x8,r15 */
L8c051814: *stop=0x8c051814u;{uint32_t loaded=rd(m,bus,&io_status,0x8c051884u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c051816; /* mov.l 0x8c051884,r4 */
L8c051816: *stop=0x8c051816u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c051818; /* mov.l @r4,r3 */
L8c051818: *stop=0x8c051818u;r[3]+=4294967295u;goto L8c05181a; /* add #-0x1,r3 */
L8c05181a: *stop=0x8c05181au;r[2]=r[3];goto L8c05181c; /* mov r3,r2 */
L8c05181c: *stop=0x8c05181cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c05181e; /* tst r2,r2 */
L8c05181e: take=!(*sr&1u);*stop=0x8c051820u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;if(take)goto L8c051870;goto L8c051822; /* bf/s 0x8c051870 */
L8c051822: *stop=0x8c051822u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c051824; /* mov.l 0x8c0518d4,r2 */
L8c051824: target=r[2];*pr=0x8c051828u;*stop=0x8c051826u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051828; /* jsr @r2 */
L8c051828: *stop=0x8c051828u;r[4]=r[0];goto L8c05182a; /* mov r0,r4 */
L8c05182a: *stop=0x8c05182au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05182c; /* mov.l 0x8c0518d8,r3 */
L8c05182c: *stop=0x8c05182cu;r[0]=r[4]&255u;goto L8c05182e; /* extu.b r4,r0 */
L8c05182e: *stop=0x8c05182eu;r[4]=r[4]&65535u;goto L8c051830; /* extu.w r4,r4 */
L8c051830: *stop=0x8c051830u;if(!valid_access(r[15]+4u,2u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],2);if(io_status)return io_status;goto L8c051832; /* mov.w r0,@(0x4,r15) */
L8c051832: *stop=0x8c051832u;r[2]=4294967288u;goto L8c051834; /* mov #-0x8,r2 */
L8c051834: *stop=0x8c051834u;r[4]&=r[3];goto L8c051836; /* and r3,r4 */
L8c051836: *stop=0x8c051836u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[4]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[4]=n?(uint32_t)((int32_t)r[4]>>n):((r[4]&0x80000000u)?0xffffffffu:0u);}}goto L8c051838; /* shad r2,r4 */
L8c051838: *stop=0x8c051838u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c05183a; /* mov.l 0x8c0518dc,r2 */
L8c05183a: *stop=0x8c05183au;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[4],2);if(io_status)return io_status;goto L8c05183c; /* mov.w r4,@r15 */
L8c05183c: *stop=0x8c05183cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c05183e; /* mov.l @r2,r0 */
L8c05183e: *stop=0x8c05183eu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c051840; /* cmp/eq #0x4,r0 */
L8c051840: take=(*sr&1u);*stop=0x8c051842u;r[4]=r[0];if(take)goto L8c051850;goto L8c051844; /* bt/s 0x8c051850 */
L8c051844: *stop=0x8c051844u;r[0]=r[4];goto L8c051846; /* mov r4,r0 */
L8c051846: *stop=0x8c051846u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c051848; /* cmp/eq #0x5,r0 */
L8c051848: if((*sr&1u))goto L8c051850;goto L8c05184a; /* bt 0x8c051850 */
L8c05184a: *stop=0x8c05184au;r[0]=r[4];goto L8c05184c; /* mov r4,r0 */
L8c05184c: *stop=0x8c05184cu;*sr=(*sr&~1u)|(r[0]==0x00000009u);goto L8c05184e; /* cmp/eq #0x9,r0 */
L8c05184e: if(!(*sr&1u))goto L8c05186a;goto L8c051850; /* bf 0x8c05186a */
L8c051850: *stop=0x8c051850u;if(!valid_access(r[15],2u))return -3;r[0]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c051852; /* mov.w @r15,r0 */
L8c051852: *stop=0x8c051852u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051854; /* mov.l 0x8c0518e0,r3 */
L8c051854: *stop=0x8c051854u;r[0]=r[0]&65535u;goto L8c051856; /* extu.w r0,r0 */
L8c051856: *stop=0x8c051856u;r[0]<<=2;goto L8c051858; /* shll2 r0 */
L8c051858: *stop=0x8c051858u;r[0]<<=2;goto L8c05185a; /* shll2 r0 */
L8c05185a: *stop=0x8c05185au;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c05185c; /* shll r0 */
L8c05185c: *stop=0x8c05185cu;r[3]+=r[0];goto L8c05185e; /* add r0,r3 */
L8c05185e: *stop=0x8c05185eu;if(!valid_access(r[15]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c051860; /* mov.w @(0x4,r15),r0 */
L8c051860: *stop=0x8c051860u;r[0]=r[0]&65535u;goto L8c051862; /* extu.w r0,r0 */
L8c051862: *stop=0x8c051862u;r[0]<<=2;goto L8c051864; /* shll2 r0 */
L8c051864: *stop=0x8c051864u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c051866; /* mov.l @(r0,r3),r2 */
L8c051866: target=r[2];*pr=0x8c05186au;*stop=0x8c051868u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05186a; /* jsr @r2 */
L8c05186a: *stop=0x8c05186au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518e4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c05186c; /* mov.l 0x8c0518e4,r1 */
L8c05186c: target=r[1];*pr=0x8c051870u;*stop=0x8c05186eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051870; /* jsr @r1 */
L8c051870: *stop=0x8c051870u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c051872; /* mov.l 0x8c0518e8,r3 */
L8c051872: target=r[3];*pr=0x8c051876u;*stop=0x8c051874u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051876; /* jsr @r3 */
L8c051876: *stop=0x8c051876u;r[15]+=8u;goto L8c051878; /* add #0x8,r15 */
L8c051878: *stop=0x8c051878u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0518ecu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c05187a; /* mov.l 0x8c0518ec,r1 */
L8c05187a: target=r[1];*stop=0x8c05187cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r1 */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c04f7e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c04f7e0;
L8c04f7e0: *stop=0x8c04f7e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04f81cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c04f7e2; /* mov.l 0x8c04f81c,r2 */
L8c04f7e2: *stop=0x8c04f7e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c04f820u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c04f7e4; /* mov.l 0x8c04f820,r1 */
L8c04f7e4: *stop=0x8c04f7e4u;if(!valid_access(r[2],2u))return -3;r[0]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c04f7e6; /* mov.w @r2,r0 */
L8c04f7e6: *stop=0x8c04f7e6u;if(!valid_access(r[1],2u))return -3;r[3]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;goto L8c04f7e8; /* mov.w @r1,r3 */
L8c04f7e8: *stop=0x8c04f7e8u;r[0]<<=8;goto L8c04f7ea; /* shll8 r0 */
L8c04f7ea: *stop=0x8c04f7eau;if(*pr!=entry_pr)return -3;*stop=0x8c04f7ecu;r[0]|=r[3];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c643f98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c643f98;
L8c643f98: *stop=0x8c643f98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643ff4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643f9a; /* mov.l 0x8c643ff4,r3 */
L8c643f9a: *stop=0x8c643f9au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c643f9c; /* sts.l PR,@-r15 */
L8c643f9c: *stop=0x8c643f9cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c643f9e; /* mov.l @r3,r4 */
L8c643f9e: *stop=0x8c643f9eu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c643fa0; /* tst r4,r4 */
L8c643fa0: if((*sr&1u))goto L8c643fea;goto L8c643fa2; /* bt 0x8c643fea */
L8c643fa2: *stop=0x8c643fa2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643ff8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643fa4; /* mov.l 0x8c643ff8,r1 */
L8c643fa4: *stop=0x8c643fa4u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c643fa6; /* mov.l @r1,r0 */
L8c643fa6: *stop=0x8c643fa6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c643fa8; /* cmp/eq #0x1,r0 */
L8c643fa8: if(!(*sr&1u))goto L8c643fea;goto L8c643faa; /* bf 0x8c643fea */
L8c643faa: *stop=0x8c643faau;r[0]=sx(m,bus,&io_status,0x8c643ff0u,2);if(io_status)return io_status;goto L8c643fac; /* mov.w 0x8c643ff0,r0 */
L8c643fac: *stop=0x8c643facu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c643fae; /* mov.l @(r0,r4),r3 */
L8c643fae: *stop=0x8c643faeu;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c643fb0; /* cmp/pl r3 */
L8c643fb0: if(!(*sr&1u))goto L8c643fb8;goto L8c643fb2; /* bf 0x8c643fb8 */
L8c643fb2: *stop=0x8c643fb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643ffcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643fb4; /* mov.l 0x8c643ffc,r1 */
L8c643fb4: target=r[1];*pr=0x8c643fb8u;*stop=0x8c643fb6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643fb8; /* jsr @r1 */
L8c643fb8: *stop=0x8c643fb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643ff4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643fba; /* mov.l 0x8c643ff4,r1 */
L8c643fba: *stop=0x8c643fbau;r[0]=sx(m,bus,&io_status,0x8c643ff2u,2);if(io_status)return io_status;goto L8c643fbc; /* mov.w 0x8c643ff2,r0 */
L8c643fbc: *stop=0x8c643fbcu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c643fbe; /* mov.l @r1,r2 */
L8c643fbe: *stop=0x8c643fbeu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c643fc0; /* mov.l @(r0,r2),r3 */
L8c643fc0: *stop=0x8c643fc0u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c643fc2; /* tst r3,r3 */
L8c643fc2: if((*sr&1u))goto L8c643fea;goto L8c643fc4; /* bt 0x8c643fea */
L8c643fc4: *stop=0x8c643fc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c644004u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643fc6; /* mov.l 0x8c644004,r3 */
L8c643fc6: *stop=0x8c643fc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c644000u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c643fc8; /* mov.l 0x8c644000,r5 */
L8c643fc8: target=r[3];*pr=0x8c643fccu;*stop=0x8c643fcau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643fcc; /* jsr @r3 */
L8c643fcc: *stop=0x8c643fccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64400cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643fce; /* mov.l 0x8c64400c,r2 */
L8c643fce: *stop=0x8c643fceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c644008u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c643fd0; /* mov.l 0x8c644008,r5 */
L8c643fd0: target=r[2];*pr=0x8c643fd4u;*stop=0x8c643fd2u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643fd4; /* jsr @r2 */
L8c643fd4: *stop=0x8c643fd4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c644008u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c643fd6; /* mov.l 0x8c644008,r0 */
L8c643fd6: *stop=0x8c643fd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c644000u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643fd8; /* mov.l 0x8c644000,r1 */
L8c643fd8: *stop=0x8c643fd8u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c643fda; /* mov.l @r0,r2 */
L8c643fda: *stop=0x8c643fdau;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c643fdc; /* mov.l @r1,r3 */
L8c643fdc: *stop=0x8c643fdcu;*sr=(*sr&~1u)|(r[2]>=r[3]);goto L8c643fde; /* cmp/hs r3,r2 */
L8c643fde: if((*sr&1u))goto L8c643fea;goto L8c643fe0; /* bt 0x8c643fea */
L8c643fe0: *stop=0x8c643fe0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c644018u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643fe2; /* mov.l 0x8c644018,r2 */
L8c643fe2: *stop=0x8c643fe2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c644014u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643fe4; /* mov.l 0x8c644014,r4 */
L8c643fe4: *stop=0x8c643fe4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c644010u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c643fe6; /* mov.l 0x8c644010,r5 */
L8c643fe6: target=r[2];*pr=0x8c643feau;*stop=0x8c643fe8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643fea; /* jsr @r2 */
L8c643fea: *stop=0x8c643feau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c643fec; /* lds.l @r15+,PR */
L8c643fec: *stop=0x8c643fecu;if(*pr!=entry_pr)return -3;*stop=0x8c643feeu;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c602cb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c602cb8;
L8c602cb8: *stop=0x8c602cb8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c602cba; /* mov.l r14,@-r15 */
L8c602cba: *stop=0x8c602cbau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c602cbc; /* mov.l r13,@-r15 */
L8c602cbc: *stop=0x8c602cbcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c602cbe; /* mov.l r12,@-r15 */
L8c602cbe: *stop=0x8c602cbeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c602cc0; /* mov.l r11,@-r15 */
L8c602cc0: *stop=0x8c602cc0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c602cc2; /* mov.l r10,@-r15 */
L8c602cc2: *stop=0x8c602cc2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c602cc4; /* mov.l r9,@-r15 */
L8c602cc4: *stop=0x8c602cc4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c602cc6; /* mov.l r8,@-r15 */
L8c602cc6: *stop=0x8c602cc6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c602cc8; /* sts.l PR,@-r15 */
L8c602cc8: *stop=0x8c602cc8u;r[15]+=4294967292u;goto L8c602cca; /* add #-0x4,r15 */
L8c602cca: *stop=0x8c602ccau;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f00u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602ccc; /* mov.l 0x8c602f00,r2 */
L8c602ccc: *stop=0x8c602cccu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602cce; /* mov.l @r2,r3 */
L8c602cce: *stop=0x8c602cceu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c602cd0; /* tst r3,r3 */
L8c602cd0: if((*sr&1u))goto L8c602dc6;goto L8c602cd2; /* bt 0x8c602dc6 */
L8c602cd2: *stop=0x8c602cd2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f04u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602cd4; /* mov.l 0x8c602f04,r2 */
L8c602cd4: *stop=0x8c602cd4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602cd6; /* mov.l @r2,r3 */
L8c602cd6: *stop=0x8c602cd6u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c602cd8; /* tst r3,r3 */
L8c602cd8: if((*sr&1u))goto L8c602ce0;goto L8c602cda; /* bt 0x8c602ce0 */
L8c602cda: *stop=0x8c602cdau;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f08u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c602cdc; /* mov.l 0x8c602f08,r9 */
L8c602cdc: *stop=0x8c602cdeu;goto L8c602ce2; /* bra 0x8c602ce2 */
L8c602ce0: *stop=0x8c602ce0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f0cu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c602ce2; /* mov.l 0x8c602f0c,r9 */
L8c602ce2: *stop=0x8c602ce2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602ce4; /* mov.l 0x8c602f10,r2 */
L8c602ce4: target=r[2];*pr=0x8c602ce8u;*stop=0x8c602ce6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602ce8; /* jsr @r2 */
L8c602ce8: *stop=0x8c602ce8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f14u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c602cea; /* mov.l 0x8c602f14,r8 */
L8c602cea: *stop=0x8c602ceau;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c602cec; /* mov.l 0x8c602f1c,r1 */
L8c602cec: *stop=0x8c602cecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602cee; /* mov.l 0x8c602f18,r2 */
L8c602cee: *stop=0x8c602ceeu;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602cf0; /* mov.l @r8,r3 */
L8c602cf0: target=r[1];*pr=0x8c602cf4u;*stop=0x8c602cf2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602cf4; /* jsr @r1 */
L8c602cf4: *stop=0x8c602cf4u;r[10]=0u;goto L8c602cf6; /* mov #0x0,r10 */
L8c602cf6: *stop=0x8c602cf6u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c602cf8; /* mov.l r0,@r8 */
L8c602cf8: *stop=0x8c602cf8u;r[14]=r[10];goto L8c602cfa; /* mov r10,r14 */
L8c602cfa: *stop=0x8c602cfau;r[13]=1u;goto L8c602cfc; /* mov #0x1,r13 */
L8c602cfc: *stop=0x8c602cfcu;r[12]=8u;goto L8c602cfe; /* mov #0x8,r12 */
L8c602cfe: target=0x8c6033e4u;*pr=0x8c602d02u;*stop=0x8c602d00u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602d02; /* bsr 0x8c6033e4 */
L8c602d02: *stop=0x8c602d02u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c602d04; /* tst r0,r0 */
L8c602d04: if(!(*sr&1u))goto L8c602d0a;goto L8c602d06; /* bf 0x8c602d0a */
L8c602d06: *stop=0x8c602d08u;r[3]=r[13];goto L8c602d0c; /* bra 0x8c602d0c */
L8c602d0a: *stop=0x8c602d0au;r[3]=0u;goto L8c602d0c; /* mov #0x0,r3 */
L8c602d0c: *stop=0x8c602d0cu;r[14]+=1u;goto L8c602d0e; /* add #0x1,r14 */
L8c602d0e: *stop=0x8c602d0eu;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c602d10; /* shll r13 */
L8c602d10: *stop=0x8c602d10u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[12]);goto L8c602d12; /* cmp/ge r12,r14 */
L8c602d12: *stop=0x8c602d12u;r[10]|=r[3];goto L8c602d14; /* or r3,r10 */
L8c602d14: if(!(*sr&1u))goto L8c602cfe;goto L8c602d16; /* bf 0x8c602cfe */
L8c602d16: *stop=0x8c602d16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c602d18; /* mov.l 0x8c602f18,r1 */
L8c602d18: *stop=0x8c602d18u;r[13]=1u;goto L8c602d1a; /* mov #0x1,r13 */
L8c602d1a: *stop=0x8c602d1au;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602d1c; /* mov.l @r8,r3 */
L8c602d1c: *stop=0x8c602d1cu;r[11]=0u;goto L8c602d1e; /* mov #0x0,r11 */
L8c602d1e: *stop=0x8c602d1eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c602d20; /* mov.l @r1,r2 */
L8c602d20: *stop=0x8c602d20u;r[0]=r[3];goto L8c602d22; /* mov r3,r0 */
L8c602d22: *stop=0x8c602d22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f20u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c602d24; /* mov.l 0x8c602f20,r14 */
L8c602d24: *stop=0x8c602d24u;r[2]^=r[3];goto L8c602d26; /* xor r3,r2 */
L8c602d26: *stop=0x8c602d26u;r[2]&=r[0];goto L8c602d28; /* and r0,r2 */
L8c602d28: *stop=0x8c602d28u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c602d2a; /* mov.l r2,@r15 */
L8c602d2a: *stop=0x8c602d2au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602d2c; /* mov.l @r15,r3 */
L8c602d2c: *stop=0x8c602d2cu;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c602d2e; /* tst r13,r3 */
L8c602d2e: if((*sr&1u))goto L8c602d4a;goto L8c602d30; /* bt 0x8c602d4a */
L8c602d30: *stop=0x8c602d30u;r[1]=r[13];goto L8c602d32; /* mov r13,r1 */
L8c602d32: *stop=0x8c602d32u;*sr=(*sr&~1u)|!(r[10]&r[1]);goto L8c602d34; /* tst r10,r1 */
L8c602d34: if((*sr&1u))goto L8c602d44;goto L8c602d36; /* bt 0x8c602d44 */
L8c602d36: target=r[9];*pr=0x8c602d3au;*stop=0x8c602d38u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602d3a; /* jsr @r9 */
L8c602d3a: *stop=0x8c602d3au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602d3c; /* mov.l @r14,r3 */
L8c602d3c: *stop=0x8c602d3cu;r[2]=~r[13];goto L8c602d3e; /* not r13,r2 */
L8c602d3e: *stop=0x8c602d3eu;r[3]&=r[2];goto L8c602d40; /* and r2,r3 */
L8c602d40: *stop=0x8c602d42u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c602d4a; /* bra 0x8c602d4a */
L8c602d44: *stop=0x8c602d44u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c602d46; /* mov.l @r14,r1 */
L8c602d46: *stop=0x8c602d46u;r[1]|=r[13];goto L8c602d48; /* or r13,r1 */
L8c602d48: *stop=0x8c602d48u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c602d4a; /* mov.l r1,@r14 */
L8c602d4a: *stop=0x8c602d4au;r[3]=r[13];goto L8c602d4c; /* mov r13,r3 */
L8c602d4c: *stop=0x8c602d4cu;*sr=(*sr&~1u)|!(r[10]&r[3]);goto L8c602d4e; /* tst r10,r3 */
L8c602d4e: if((*sr&1u))goto L8c602d62;goto L8c602d50; /* bt 0x8c602d62 */
L8c602d50: *stop=0x8c602d50u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c602d52; /* mov.l @r14,r1 */
L8c602d52: *stop=0x8c602d52u;*sr=(*sr&~1u)|!(r[13]&r[1]);goto L8c602d54; /* tst r13,r1 */
L8c602d54: if((*sr&1u))goto L8c602d62;goto L8c602d56; /* bt 0x8c602d62 */
L8c602d56: target=r[9];*pr=0x8c602d5au;*stop=0x8c602d58u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602d5a; /* jsr @r9 */
L8c602d5a: *stop=0x8c602d5au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602d5c; /* mov.l @r14,r3 */
L8c602d5c: *stop=0x8c602d5cu;r[2]=~r[13];goto L8c602d5e; /* not r13,r2 */
L8c602d5e: *stop=0x8c602d5eu;r[3]&=r[2];goto L8c602d60; /* and r2,r3 */
L8c602d60: *stop=0x8c602d60u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c602d62; /* mov.l r3,@r14 */
L8c602d62: *stop=0x8c602d62u;r[11]+=1u;goto L8c602d64; /* add #0x1,r11 */
L8c602d64: *stop=0x8c602d64u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c602d66; /* shll r13 */
L8c602d66: *stop=0x8c602d66u;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[12]);goto L8c602d68; /* cmp/ge r12,r11 */
L8c602d68: if(!(*sr&1u))goto L8c602d2a;goto L8c602d6a; /* bf 0x8c602d2a */
L8c602d6a: *stop=0x8c602d6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602d6c; /* mov.l 0x8c602f18,r3 */
L8c602d6c: *stop=0x8c602d6cu;r[13]=1u;goto L8c602d6e; /* mov #0x1,r13 */
L8c602d6e: *stop=0x8c602d6eu;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[2]=loaded;}goto L8c602d70; /* mov.l @r8,r2 */
L8c602d70: *stop=0x8c602d70u;r[11]=0u;goto L8c602d72; /* mov #0x0,r11 */
L8c602d72: *stop=0x8c602d72u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c602d74; /* mov.l @r3,r4 */
L8c602d74: *stop=0x8c602d74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f24u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c602d76; /* mov.l 0x8c602f24,r9 */
L8c602d76: *stop=0x8c602d76u;r[2]^=r[4];goto L8c602d78; /* xor r4,r2 */
L8c602d78: *stop=0x8c602d78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f28u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c602d7a; /* mov.l 0x8c602f28,r14 */
L8c602d7a: *stop=0x8c602d7au;r[2]&=r[4];goto L8c602d7c; /* and r4,r2 */
L8c602d7c: *stop=0x8c602d7cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c602d7e; /* mov.l r2,@r15 */
L8c602d7e: *stop=0x8c602d7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602d80; /* mov.l @r15,r3 */
L8c602d80: *stop=0x8c602d80u;r[8]=r[13];goto L8c602d82; /* mov r13,r8 */
L8c602d82: *stop=0x8c602d82u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c602d84; /* tst r13,r3 */
L8c602d84: take=(*sr&1u);*stop=0x8c602d86u;r[8]&=r[10];if(take)goto L8c602da4;goto L8c602d88; /* bt/s 0x8c602da4 */
L8c602d88: *stop=0x8c602d88u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c602d8a; /* tst r8,r8 */
L8c602d8a: if((*sr&1u))goto L8c602d9e;goto L8c602d8c; /* bt 0x8c602d9e */
L8c602d8c: *stop=0x8c602d8cu;r[5]=2u;goto L8c602d8e; /* mov #0x2,r5 */
L8c602d8e: *stop=0x8c602d8eu;r[6]=0u;goto L8c602d90; /* mov #0x0,r6 */
L8c602d90: target=r[9];*pr=0x8c602d94u;*stop=0x8c602d92u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602d94; /* jsr @r9 */
L8c602d94: *stop=0x8c602d94u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c602d96; /* mov.l @r14,r2 */
L8c602d96: *stop=0x8c602d96u;r[3]=~r[13];goto L8c602d98; /* not r13,r3 */
L8c602d98: *stop=0x8c602d98u;r[2]&=r[3];goto L8c602d9a; /* and r3,r2 */
L8c602d9a: *stop=0x8c602d9cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c602da4; /* bra 0x8c602da4 */
L8c602d9e: *stop=0x8c602d9eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c602da0; /* mov.l @r14,r1 */
L8c602da0: *stop=0x8c602da0u;r[1]|=r[13];goto L8c602da2; /* or r13,r1 */
L8c602da2: *stop=0x8c602da2u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8c602da4; /* mov.l r1,@r14 */
L8c602da4: *stop=0x8c602da4u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c602da6; /* tst r8,r8 */
L8c602da6: if((*sr&1u))goto L8c602dbe;goto L8c602da8; /* bt 0x8c602dbe */
L8c602da8: *stop=0x8c602da8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c602daa; /* mov.l @r14,r2 */
L8c602daa: *stop=0x8c602daau;*sr=(*sr&~1u)|!(r[13]&r[2]);goto L8c602dac; /* tst r13,r2 */
L8c602dac: if((*sr&1u))goto L8c602dbe;goto L8c602dae; /* bt 0x8c602dbe */
L8c602dae: *stop=0x8c602daeu;r[5]=2u;goto L8c602db0; /* mov #0x2,r5 */
L8c602db0: *stop=0x8c602db0u;r[6]=0u;goto L8c602db2; /* mov #0x0,r6 */
L8c602db2: target=r[9];*pr=0x8c602db6u;*stop=0x8c602db4u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602db6; /* jsr @r9 */
L8c602db6: *stop=0x8c602db6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602db8; /* mov.l @r14,r3 */
L8c602db8: *stop=0x8c602db8u;r[2]=~r[13];goto L8c602dba; /* not r13,r2 */
L8c602dba: *stop=0x8c602dbau;r[3]&=r[2];goto L8c602dbc; /* and r2,r3 */
L8c602dbc: *stop=0x8c602dbcu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c602dbe; /* mov.l r3,@r14 */
L8c602dbe: *stop=0x8c602dbeu;r[11]+=1u;goto L8c602dc0; /* add #0x1,r11 */
L8c602dc0: *stop=0x8c602dc0u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c602dc2; /* shll r13 */
L8c602dc2: *stop=0x8c602dc2u;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[12]);goto L8c602dc4; /* cmp/ge r12,r11 */
L8c602dc4: if(!(*sr&1u))goto L8c602d7e;goto L8c602dc6; /* bf 0x8c602d7e */
L8c602dc6: *stop=0x8c602dc6u;r[15]+=4u;goto L8c602dc8; /* add #0x4,r15 */
L8c602dc8: *stop=0x8c602dc8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c602dca; /* lds.l @r15+,PR */
L8c602dca: *stop=0x8c602dcau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c602dcc; /* mov.l @r15+,r8 */
L8c602dcc: *stop=0x8c602dccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c602dce; /* mov.l @r15+,r9 */
L8c602dce: *stop=0x8c602dceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c602dd0; /* mov.l @r15+,r10 */
L8c602dd0: *stop=0x8c602dd0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c602dd2; /* mov.l @r15+,r11 */
L8c602dd2: *stop=0x8c602dd2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c602dd4; /* mov.l @r15+,r12 */
L8c602dd4: *stop=0x8c602dd4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c602dd6; /* mov.l @r15+,r13 */
L8c602dd6: *stop=0x8c602dd6u;if(*pr!=entry_pr)return -3;*stop=0x8c602dd8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c65a378(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65a378;
L8c65a378: *stop=0x8c65a378u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a37a; /* mov.l r14,@-r15 */
L8c65a37a: *stop=0x8c65a37au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65a37c; /* mov.l r13,@-r15 */
L8c65a37c: *stop=0x8c65a37cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65a37e; /* mov.l r12,@-r15 */
L8c65a37e: *stop=0x8c65a37eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65a380; /* mov.l r11,@-r15 */
L8c65a380: *stop=0x8c65a380u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65a382; /* mov.l r10,@-r15 */
L8c65a382: *stop=0x8c65a382u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c65a384; /* mov.l r9,@-r15 */
L8c65a384: *stop=0x8c65a384u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c65a386; /* mov.l r8,@-r15 */
L8c65a386: *stop=0x8c65a386u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a388; /* sts.l PR,@-r15 */
L8c65a388: *stop=0x8c65a388u;r[15]+=4294967292u;goto L8c65a38a; /* add #-0x4,r15 */
L8c65a38a: *stop=0x8c65a38au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a444u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a38c; /* mov.l 0x8c65a444,r3 */
L8c65a38c: *stop=0x8c65a38cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a38e; /* mov.l @r3,r14 */
L8c65a38e: *stop=0x8c65a38eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a390; /* tst r14,r14 */
L8c65a390: if(!(*sr&1u))goto L8c65a398;goto L8c65a392; /* bf 0x8c65a398 */
L8c65a392: *stop=0x8c65a392u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a448u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a394; /* mov.l 0x8c65a448,r14 */
L8c65a394: *stop=0x8c65a396u;goto L8c65a418; /* bra 0x8c65a418 */
L8c65a398: *stop=0x8c65a398u;r[13]=sx(m,bus,&io_status,0x8c65a43au,2);if(io_status)return io_status;goto L8c65a39a; /* mov.w 0x8c65a43a,r13 */
L8c65a39a: *stop=0x8c65a39au;r[13]+=r[14];goto L8c65a39c; /* add r14,r13 */
L8c65a39c: *stop=0x8c65a39cu;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a39e; /* mov.l @(0x8,r13),r0 */
L8c65a39e: *stop=0x8c65a39eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65a3a0; /* cmp/eq #0x1,r0 */
L8c65a3a0: if((*sr&1u))goto L8c65a3b2;goto L8c65a3a2; /* bt 0x8c65a3b2 */
L8c65a3a2: *stop=0x8c65a3a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a44cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a3a4; /* mov.l 0x8c65a44c,r3 */
L8c65a3a4: *stop=0x8c65a3a4u;r[5]=r[15];goto L8c65a3a6; /* mov r15,r5 */
L8c65a3a6: *stop=0x8c65a3a6u;r[0]=sx(m,bus,&io_status,0x8c65a43cu,2);if(io_status)return io_status;goto L8c65a3a8; /* mov.w 0x8c65a43c,r0 */
L8c65a3a8: target=r[3];*pr=0x8c65a3acu;*stop=0x8c65a3aau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a3ac; /* jsr @r3 */
L8c65a3ac: *stop=0x8c65a3acu;if(!valid_access(r[15],1u))return -3;r[2]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c65a3ae; /* mov.b @r15,r2 */
L8c65a3ae: *stop=0x8c65a3aeu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65a3b0; /* tst r2,r2 */
L8c65a3b0: if((*sr&1u))goto L8c65a3b8;goto L8c65a3b2; /* bt 0x8c65a3b8 */
L8c65a3b2: *stop=0x8c65a3b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a450u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a3b4; /* mov.l 0x8c65a450,r14 */
L8c65a3b4: *stop=0x8c65a3b6u;goto L8c65a418; /* bra 0x8c65a418 */
L8c65a3b8: *stop=0x8c65a3b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a444u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a3ba; /* mov.l 0x8c65a444,r3 */
L8c65a3ba: *stop=0x8c65a3bau;r[12]=0u;goto L8c65a3bc; /* mov #0x0,r12 */
L8c65a3bc: *stop=0x8c65a3bcu;r[0]=sx(m,bus,&io_status,0x8c65a43eu,2);if(io_status)return io_status;goto L8c65a3be; /* mov.w 0x8c65a43e,r0 */
L8c65a3be: *stop=0x8c65a3beu;r[10]=r[12];goto L8c65a3c0; /* mov r12,r10 */
L8c65a3c0: *stop=0x8c65a3c0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[11]=loaded;}goto L8c65a3c2; /* mov.l @r3,r11 */
L8c65a3c2: *stop=0x8c65a3c2u;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[11]=loaded;}goto L8c65a3c4; /* mov.l @(r0,r11),r11 */
L8c65a3c4: *stop=0x8c65a3c4u;*sr=(*sr&~1u)|((int32_t)r[11]>0);goto L8c65a3c6; /* cmp/pl r11 */
L8c65a3c6: take=!(*sr&1u);*stop=0x8c65a3c8u;r[9]=r[12];if(take)goto L8c65a40e;goto L8c65a3ca; /* bf/s 0x8c65a40e */
L8c65a3ca: *stop=0x8c65a3cau;r[8]=28u;goto L8c65a3cc; /* mov #0x1c,r8 */
L8c65a3cc: *stop=0x8c65a3ccu;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a3ce; /* mov.l @(0x1c,r13),r4 */
L8c65a3ce: *stop=0x8c65a3ceu;*macl=r[8]*r[4];goto L8c65a3d0; /* mul.l r8,r4 */
L8c65a3d0: *stop=0x8c65a3d0u;r[4]+=1u;goto L8c65a3d2; /* add #0x1,r4 */
L8c65a3d2: *stop=0x8c65a3d2u;if(!valid_access(r[13]+28u,4u))return -3;wr(m,bus,&io_status,r[13]+0x1cu,r[4],4);if(io_status)return io_status;goto L8c65a3d4; /* mov.l r4,@(0x1c,r13) */
L8c65a3d4: *stop=0x8c65a3d4u;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a3d6; /* mov.l @(0x1c,r13),r2 */
L8c65a3d6: *stop=0x8c65a3d6u;r[4]=*macl;goto L8c65a3d8; /* sts MACL,r4 */
L8c65a3d8: *stop=0x8c65a3d8u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a3da; /* mov.l @(0xc,r13),r3 */
L8c65a3da: *stop=0x8c65a3dau;*sr=(*sr&~1u)|(r[11]==r[2]);goto L8c65a3dc; /* cmp/eq r11,r2 */
L8c65a3dc: *stop=0x8c65a3dcu;r[4]+=r[3];goto L8c65a3de; /* add r3,r4 */
L8c65a3de: if(!(*sr&1u))goto L8c65a3e2;goto L8c65a3e0; /* bf 0x8c65a3e2 */
L8c65a3e0: *stop=0x8c65a3e0u;if(!valid_access(r[13]+28u,4u))return -3;wr(m,bus,&io_status,r[13]+0x1cu,r[12],4);if(io_status)return io_status;goto L8c65a3e2; /* mov.l r12,@(0x1c,r13) */
L8c65a3e2: *stop=0x8c65a3e2u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a3e4; /* mov.l @(0x14,r4),r0 */
L8c65a3e4: *stop=0x8c65a3e4u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65a3e6; /* cmp/eq #0x1,r0 */
L8c65a3e6: if(!(*sr&1u))goto L8c65a408;goto L8c65a3e8; /* bf 0x8c65a408 */
L8c65a3e8: *stop=0x8c65a3e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a454u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a3ea; /* mov.l 0x8c65a454,r3 */
L8c65a3ea: target=r[3];*pr=0x8c65a3eeu;*stop=0x8c65a3ecu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a3ee; /* jsr @r3 */
L8c65a3ee: *stop=0x8c65a3eeu;r[14]=r[0];goto L8c65a3f0; /* mov r0,r14 */
L8c65a3f0: *stop=0x8c65a3f0u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a3f2; /* tst r14,r14 */
L8c65a3f2: if(!(*sr&1u))goto L8c65a418;goto L8c65a3f4; /* bf 0x8c65a418 */
L8c65a3f4: *stop=0x8c65a3f4u;r[10]+=1u;goto L8c65a3f6; /* add #0x1,r10 */
L8c65a3f6: *stop=0x8c65a3f6u;r[0]=r[10];goto L8c65a3f8; /* mov r10,r0 */
L8c65a3f8: *stop=0x8c65a3f8u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c65a3fa; /* cmp/eq #0x20,r0 */
L8c65a3fa: if((*sr&1u))goto L8c65a40e;goto L8c65a3fc; /* bt 0x8c65a40e */
L8c65a3fc: *stop=0x8c65a3fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a444u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a3fe; /* mov.l 0x8c65a444,r2 */
L8c65a3fe: *stop=0x8c65a3feu;r[0]=sx(m,bus,&io_status,0x8c65a440u,2);if(io_status)return io_status;goto L8c65a400; /* mov.w 0x8c65a440,r0 */
L8c65a400: *stop=0x8c65a400u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a402; /* mov.l @r2,r3 */
L8c65a402: *stop=0x8c65a402u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65a404; /* mov.l @(r0,r3),r1 */
L8c65a404: *stop=0x8c65a404u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c65a406; /* tst r1,r1 */
L8c65a406: if((*sr&1u))goto L8c65a40e;goto L8c65a408; /* bt 0x8c65a40e */
L8c65a408: *stop=0x8c65a408u;r[9]+=1u;goto L8c65a40a; /* add #0x1,r9 */
L8c65a40a: *stop=0x8c65a40au;*sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[11]);goto L8c65a40c; /* cmp/ge r11,r9 */
L8c65a40c: if(!(*sr&1u))goto L8c65a3cc;goto L8c65a40e; /* bf 0x8c65a3cc */
L8c65a40e: *stop=0x8c65a40eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a458u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a410; /* mov.l 0x8c65a458,r2 */
L8c65a410: target=r[2];*pr=0x8c65a414u;*stop=0x8c65a412u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a414; /* jsr @r2 */
L8c65a414: *stop=0x8c65a414u;r[14]=r[0];goto L8c65a416; /* mov r0,r14 */
L8c65a416: *stop=0x8c65a416u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a418; /* tst r14,r14 */
L8c65a418: *stop=0x8c65a418u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a41a; /* tst r14,r14 */
L8c65a41a: if((*sr&1u))goto L8c65a424;goto L8c65a41c; /* bt 0x8c65a424 */
L8c65a41c: *stop=0x8c65a41cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a460u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a41e; /* mov.l 0x8c65a460,r2 */
L8c65a41e: *stop=0x8c65a41eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a45cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a420; /* mov.l 0x8c65a45c,r4 */
L8c65a420: target=r[2];*pr=0x8c65a424u;*stop=0x8c65a422u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a424; /* jsr @r2 */
L8c65a424: *stop=0x8c65a424u;r[0]=r[14];goto L8c65a426; /* mov r14,r0 */
L8c65a426: *stop=0x8c65a426u;r[15]+=4u;goto L8c65a428; /* add #0x4,r15 */
L8c65a428: *stop=0x8c65a428u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a42a; /* lds.l @r15+,PR */
L8c65a42a: *stop=0x8c65a42au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65a42c; /* mov.l @r15+,r8 */
L8c65a42c: *stop=0x8c65a42cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65a42e; /* mov.l @r15+,r9 */
L8c65a42e: *stop=0x8c65a42eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65a430; /* mov.l @r15+,r10 */
L8c65a430: *stop=0x8c65a430u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65a432; /* mov.l @r15+,r11 */
L8c65a432: *stop=0x8c65a432u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65a434; /* mov.l @r15+,r12 */
L8c65a434: *stop=0x8c65a434u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c65a436; /* mov.l @r15+,r13 */
L8c65a436: *stop=0x8c65a436u;if(*pr!=entry_pr)return -3;*stop=0x8c65a438u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c10d428(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d428;
L8c10d428: *stop=0x8c10d428u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d440u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d42a; /* mov.l 0x8c10d440,r3 */
L8c10d42a: *stop=0x8c10d42au;r[4]<<=2;goto L8c10d42c; /* shll2 r4 */
L8c10d42c: *stop=0x8c10d42cu;r[2]=sx(m,bus,&io_status,0x8c10d43cu,2);if(io_status)return io_status;goto L8c10d42e; /* mov.w 0x8c10d43c,r2 */
L8c10d42e: *stop=0x8c10d42eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d430; /* mov.l @r3,r0 */
L8c10d430: *stop=0x8c10d430u;r[2]+=r[0];goto L8c10d432; /* add r0,r2 */
L8c10d432: *stop=0x8c10d432u;r[4]+=r[2];goto L8c10d434; /* add r2,r4 */
L8c10d434: *stop=0x8c10d434u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d436; /* mov.l @r4,r1 */
L8c10d436: *stop=0x8c10d436u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c10d438; /* mov.l r1,@r5 */
L8c10d438: *stop=0x8c10d438u;if(*pr!=entry_pr)return -3;*stop=0x8c10d43au;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c10d444(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d444;
L8c10d444: *stop=0x8c10d444u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d45cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d446; /* mov.l 0x8c10d45c,r3 */
L8c10d446: *stop=0x8c10d446u;r[4]<<=2;goto L8c10d448; /* shll2 r4 */
L8c10d448: *stop=0x8c10d448u;r[2]=sx(m,bus,&io_status,0x8c10d458u,2);if(io_status)return io_status;goto L8c10d44a; /* mov.w 0x8c10d458,r2 */
L8c10d44a: *stop=0x8c10d44au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d44c; /* mov.l @r3,r0 */
L8c10d44c: *stop=0x8c10d44cu;r[2]+=r[0];goto L8c10d44e; /* add r0,r2 */
L8c10d44e: *stop=0x8c10d44eu;r[4]+=r[2];goto L8c10d450; /* add r2,r4 */
L8c10d450: *stop=0x8c10d450u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d452; /* mov.l @r4,r1 */
L8c10d452: *stop=0x8c10d452u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c10d454; /* mov.l r1,@r5 */
L8c10d454: *stop=0x8c10d454u;if(*pr!=entry_pr)return -3;*stop=0x8c10d456u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c647908(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647908;
L8c647908: *stop=0x8c647908u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64797cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64790a; /* mov.l 0x8c64797c,r3 */
L8c64790a: target=r[3];*stop=0x8c64790cu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c648234(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c648234;
L8c648234: *stop=0x8c648234u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c648236; /* mov.l r14,@-r15 */
L8c648236: *stop=0x8c648236u;r[4]=0u;goto L8c648238; /* mov #0x0,r4 */
L8c648238: *stop=0x8c648238u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64823a; /* mov.l r13,@-r15 */
L8c64823a: *stop=0x8c64823au;r[14]=r[4];goto L8c64823c; /* mov r4,r14 */
L8c64823c: *stop=0x8c64823cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64823e; /* mov.l r12,@-r15 */
L8c64823e: *stop=0x8c64823eu;r[13]=1u;goto L8c648240; /* mov #0x1,r13 */
L8c648240: *stop=0x8c648240u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c648242; /* mov.l r11,@-r15 */
L8c648242: *stop=0x8c648242u;r[12]=r[4];goto L8c648244; /* mov r4,r12 */
L8c648244: *stop=0x8c648244u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c648246; /* mov.l r10,@-r15 */
L8c648246: *stop=0x8c648246u;r[11]=8u;goto L8c648248; /* mov #0x8,r11 */
L8c648248: *stop=0x8c648248u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64824a; /* sts.l PR,@-r15 */
L8c64824a: *stop=0x8c64824au;{uint32_t loaded=rd(m,bus,&io_status,0x8c648364u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64824c; /* mov.l 0x8c648364,r10 */
L8c64824c: target=r[10];*pr=0x8c648250u;*stop=0x8c64824eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648250; /* jsr @r10 */
L8c648250: *stop=0x8c648250u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648252; /* tst r0,r0 */
L8c648252: take=(*sr&1u);*stop=0x8c648254u;r[14]+=1u;if(take)goto L8c648258;goto L8c648256; /* bt/s 0x8c648258 */
L8c648256: *stop=0x8c648256u;r[12]|=r[13];goto L8c648258; /* or r13,r12 */
L8c648258: *stop=0x8c648258u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c64825a; /* shll r13 */
L8c64825a: *stop=0x8c64825au;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[11]);goto L8c64825c; /* cmp/ge r11,r14 */
L8c64825c: if(!(*sr&1u))goto L8c64824c;goto L8c64825e; /* bf 0x8c64824c */
L8c64825e: *stop=0x8c64825eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c648260; /* lds.l @r15+,PR */
L8c648260: *stop=0x8c648260u;r[0]=r[12];goto L8c648262; /* mov r12,r0 */
L8c648262: *stop=0x8c648262u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c648264; /* mov.l @r15+,r10 */
L8c648264: *stop=0x8c648264u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c648266; /* mov.l @r15+,r11 */
L8c648266: *stop=0x8c648266u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c648268; /* mov.l @r15+,r12 */
L8c648268: *stop=0x8c648268u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64826a; /* mov.l @r15+,r13 */
L8c64826a: *stop=0x8c64826au;if(*pr!=entry_pr)return -3;*stop=0x8c64826cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c602df8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602df8;
L8c602df8: *stop=0x8c602df8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c602dfa; /* mov.l r14,@-r15 */
L8c602dfa: *stop=0x8c602dfau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c602dfc; /* mov.l r13,@-r15 */
L8c602dfc: *stop=0x8c602dfcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c602dfe; /* sts.l PR,@-r15 */
L8c602dfe: *stop=0x8c602dfeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602e00; /* mov.l 0x8c602f40,r3 */
L8c602e00: target=r[3];*pr=0x8c602e04u;*stop=0x8c602e02u;r[14]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602e04; /* jsr @r3 */
L8c602e04: *stop=0x8c602e04u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c602e06; /* tst r0,r0 */
L8c602e06: if((*sr&1u))goto L8c602e0c;goto L8c602e08; /* bt 0x8c602e0c */
L8c602e08: *stop=0x8c602e0au;r[0]=4294967295u;goto L8c602e2c; /* bra 0x8c602e2c */
L8c602e0c: *stop=0x8c602e0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602e0e; /* mov.l 0x8c602f44,r3 */
L8c602e0e: *stop=0x8c602e0eu;r[5]=1u;goto L8c602e10; /* mov #0x1,r5 */
L8c602e10: target=r[3];*pr=0x8c602e14u;*stop=0x8c602e12u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602e14; /* jsr @r3 */
L8c602e14: *stop=0x8c602e14u;r[13]=0u;goto L8c602e16; /* mov #0x0,r13 */
L8c602e16: *stop=0x8c602e16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602e18; /* mov.l 0x8c602f48,r2 */
L8c602e18: *stop=0x8c602e18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c602e1a; /* mov.l r13,@-r15 */
L8c602e1a: *stop=0x8c602e1au;r[5]=r[14];goto L8c602e1c; /* mov r14,r5 */
L8c602e1c: *stop=0x8c602e1cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c602e1e; /* mov.l r13,@-r15 */
L8c602e1e: *stop=0x8c602e1eu;r[6]=r[13];goto L8c602e20; /* mov r13,r6 */
L8c602e20: *stop=0x8c602e20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c602e22; /* mov.l r13,@-r15 */
L8c602e22: *stop=0x8c602e22u;r[7]=r[13];goto L8c602e24; /* mov r13,r7 */
L8c602e24: target=r[2];*pr=0x8c602e28u;*stop=0x8c602e26u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602e28; /* jsr @r2 */
L8c602e28: *stop=0x8c602e28u;r[15]+=12u;goto L8c602e2a; /* add #0xc,r15 */
L8c602e2a: *stop=0x8c602e2au;r[0]=r[13];goto L8c602e2c; /* mov r13,r0 */
L8c602e2c: *stop=0x8c602e2cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c602e2e; /* lds.l @r15+,PR */
L8c602e2e: *stop=0x8c602e2eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c602e30; /* mov.l @r15+,r13 */
L8c602e30: *stop=0x8c602e30u;if(*pr!=entry_pr)return -3;*stop=0x8c602e32u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c602e34(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602e34;
L8c602e34: *stop=0x8c602e34u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c602e36; /* mov.l r14,@-r15 */
L8c602e36: *stop=0x8c602e36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c602e38; /* mov.l r13,@-r15 */
L8c602e38: *stop=0x8c602e38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c602e3a; /* sts.l PR,@-r15 */
L8c602e3a: *stop=0x8c602e3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602e3c; /* mov.l 0x8c602f40,r3 */
L8c602e3c: target=r[3];*pr=0x8c602e40u;*stop=0x8c602e3eu;r[14]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602e40; /* jsr @r3 */
L8c602e40: *stop=0x8c602e40u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c602e42; /* tst r0,r0 */
L8c602e42: if((*sr&1u))goto L8c602e48;goto L8c602e44; /* bt 0x8c602e48 */
L8c602e44: *stop=0x8c602e46u;r[0]=4294967295u;goto L8c602e68; /* bra 0x8c602e68 */
L8c602e48: *stop=0x8c602e48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602e4a; /* mov.l 0x8c602f44,r3 */
L8c602e4a: *stop=0x8c602e4au;r[5]=18u;goto L8c602e4c; /* mov #0x12,r5 */
L8c602e4c: target=r[3];*pr=0x8c602e50u;*stop=0x8c602e4eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602e50; /* jsr @r3 */
L8c602e50: *stop=0x8c602e50u;r[13]=0u;goto L8c602e52; /* mov #0x0,r13 */
L8c602e52: *stop=0x8c602e52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602e54; /* mov.l 0x8c602f48,r2 */
L8c602e54: *stop=0x8c602e54u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c602e56; /* mov.l r13,@-r15 */
L8c602e56: *stop=0x8c602e56u;r[5]=r[14];goto L8c602e58; /* mov r14,r5 */
L8c602e58: *stop=0x8c602e58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c602e5a; /* mov.l r13,@-r15 */
L8c602e5a: *stop=0x8c602e5au;r[6]=r[13];goto L8c602e5c; /* mov r13,r6 */
L8c602e5c: *stop=0x8c602e5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c602e5e; /* mov.l r13,@-r15 */
L8c602e5e: *stop=0x8c602e5eu;r[7]=r[13];goto L8c602e60; /* mov r13,r7 */
L8c602e60: target=r[2];*pr=0x8c602e64u;*stop=0x8c602e62u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602e64; /* jsr @r2 */
L8c602e64: *stop=0x8c602e64u;r[15]+=12u;goto L8c602e66; /* add #0xc,r15 */
L8c602e66: *stop=0x8c602e66u;r[0]=r[13];goto L8c602e68; /* mov r13,r0 */
L8c602e68: *stop=0x8c602e68u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c602e6a; /* lds.l @r15+,PR */
L8c602e6a: *stop=0x8c602e6au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c602e6c; /* mov.l @r15+,r13 */
L8c602e6c: *stop=0x8c602e6cu;if(*pr!=entry_pr)return -3;*stop=0x8c602e6eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c647bb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647bb8;
L8c647bb8: *stop=0x8c647bb8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c647bba; /* mov.l r14,@-r15 */
L8c647bba: *stop=0x8c647bbau;r[14]=r[4];goto L8c647bbc; /* mov r4,r14 */
L8c647bbc: *stop=0x8c647bbcu;r[14]<<=2;goto L8c647bbe; /* shll2 r14 */
L8c647bbe: *stop=0x8c647bbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647c4cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647bc0; /* mov.l 0x8c647c4c,r3 */
L8c647bc0: *stop=0x8c647bc0u;r[14]<<=2;goto L8c647bc2; /* shll2 r14 */
L8c647bc2: *stop=0x8c647bc2u;r[14]<<=2;goto L8c647bc4; /* shll2 r14 */
L8c647bc4: *stop=0x8c647bc4u;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c647bc6; /* shll r14 */
L8c647bc6: *stop=0x8c647bc6u;r[0]=116u;goto L8c647bc8; /* mov #0x74,r0 */
L8c647bc8: *stop=0x8c647bc8u;r[14]+=r[3];goto L8c647bca; /* add r3,r14 */
L8c647bca: *stop=0x8c647bcau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c647bcc; /* mov.l @(r0,r14),r2 */
L8c647bcc: *stop=0x8c647bccu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c647bce; /* tst r2,r2 */
L8c647bce: if((*sr&1u))goto L8c647bda;goto L8c647bd0; /* bt 0x8c647bda */
L8c647bd0: *stop=0x8c647bd0u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c647bd2; /* mov.l @(r0,r14),r2 */
L8c647bd2: *stop=0x8c647bd2u;r[0]=124u;goto L8c647bd4; /* mov #0x7c,r0 */
L8c647bd4: *stop=0x8c647bd4u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[7]=loaded;}goto L8c647bd6; /* mov.l @(r0,r14),r7 */
L8c647bd6: target=r[2];*stop=0x8c647bd8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c647bda: *stop=0x8c647bdau;if(*pr!=entry_pr)return -3;*stop=0x8c647bdcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c05180c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c05180c;
L8c05180c: *stop=0x8c05180cu;if(*pr!=entry_pr)return -3;*stop=0x8c05180eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c66085a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c66085a;
L8c66085a: *stop=0x8c66085au;{uint32_t loaded=rd(m,bus,&io_status,0x8c660888u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66085c; /* mov.l 0x8c660888,r3 */
L8c66085c: *stop=0x8c66085cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c660880u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66085e; /* mov.l 0x8c660880,r2 */
L8c66085e: target=r[3];*stop=0x8c660860u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c660dac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c660dac;
L8c660dac: *stop=0x8c660dacu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c660dae; /* mov.l r14,@-r15 */
L8c660dae: *stop=0x8c660daeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c660db0; /* sts.l PR,@-r15 */
L8c660db0: *stop=0x8c660db0u;r[15]+=4294967292u;goto L8c660db2; /* add #-0x4,r15 */
L8c660db2: *stop=0x8c660db2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660e3cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660db4; /* mov.l 0x8c660e3c,r0 */
L8c660db4: *stop=0x8c660db4u;r[14]=r[4];goto L8c660db6; /* mov r4,r14 */
L8c660db6: *stop=0x8c660db6u;r[14]<<=2;goto L8c660db8; /* shll2 r14 */
L8c660db8: *stop=0x8c660db8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660e40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660dba; /* mov.l 0x8c660e40,r3 */
L8c660dba: *stop=0x8c660dbau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c660dbc; /* mov.l @(r0,r14),r14 */
L8c660dbc: target=r[3];*pr=0x8c660dc0u;*stop=0x8c660dbeu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660dc0; /* jsr @r3 */
L8c660dc0: *stop=0x8c660dc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660e44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660dc2; /* mov.l 0x8c660e44,r3 */
L8c660dc2: *stop=0x8c660dc2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c660dc4; /* mov.l r0,@r15 */
L8c660dc4: target=r[3];*pr=0x8c660dc8u;*stop=0x8c660dc6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660dc8; /* jsr @r3 */
L8c660dc8: *stop=0x8c660dc8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c660dca; /* mov.l @r15,r2 */
L8c660dca: *stop=0x8c660dcau;r[4]=r[0];goto L8c660dcc; /* mov r0,r4 */
L8c660dcc: *stop=0x8c660dccu;if(!valid_access(r[2],1u))return -3;r[3]=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;goto L8c660dce; /* mov.b @r2,r3 */
L8c660dce: *stop=0x8c660dceu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c660dd0; /* tst r3,r3 */
L8c660dd0: if((*sr&1u))goto L8c660dd8;goto L8c660dd2; /* bt 0x8c660dd8 */
L8c660dd2: *stop=0x8c660dd2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c660dd4; /* mov.l @r4,r0 */
L8c660dd4: *stop=0x8c660dd4u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c660dd6; /* tst #0x2,r0 */
L8c660dd6: if(!(*sr&1u))goto L8c660de2;goto L8c660dd8; /* bf 0x8c660de2 */
L8c660dd8: *stop=0x8c660dd8u;r[0]=0u;goto L8c660dda; /* mov #0x0,r0 */
L8c660dda: *stop=0x8c660ddau;r[15]+=4u;goto L8c660ddc; /* add #0x4,r15 */
L8c660ddc: *stop=0x8c660ddcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660dde; /* lds.l @r15+,PR */
L8c660dde: *stop=0x8c660ddeu;if(*pr!=entry_pr)return -3;*stop=0x8c660de0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c660de2: *stop=0x8c660de2u;r[0]=1u;goto L8c660de4; /* mov #0x1,r0 */
L8c660de4: *stop=0x8c660de4u;r[15]+=4u;goto L8c660de6; /* add #0x4,r15 */
L8c660de6: *stop=0x8c660de6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660de8; /* lds.l @r15+,PR */
L8c660de8: *stop=0x8c660de8u;if(*pr!=entry_pr)return -3;*stop=0x8c660deau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c647cb4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647cb4;
L8c647cb4: *stop=0x8c647cb4u;r[4]<<=2;goto L8c647cb6; /* shll2 r4 */
L8c647cb6: *stop=0x8c647cb6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647d10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647cb8; /* mov.l 0x8c647d10,r3 */
L8c647cb8: *stop=0x8c647cb8u;r[4]<<=2;goto L8c647cba; /* shll2 r4 */
L8c647cba: *stop=0x8c647cbau;r[4]<<=2;goto L8c647cbc; /* shll2 r4 */
L8c647cbc: *stop=0x8c647cbcu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c647cbe; /* shll r4 */
L8c647cbe: *stop=0x8c647cbeu;r[4]+=r[3];goto L8c647cc0; /* add r3,r4 */
L8c647cc0: *stop=0x8c647cc0u;if(*pr!=entry_pr)return -3;*stop=0x8c647cc2u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c647cc4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647cc4;
L8c647cc4: *stop=0x8c647cc4u;r[4]<<=2;goto L8c647cc6; /* shll2 r4 */
L8c647cc6: *stop=0x8c647cc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647d10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647cc8; /* mov.l 0x8c647d10,r3 */
L8c647cc8: *stop=0x8c647cc8u;r[4]<<=2;goto L8c647cca; /* shll2 r4 */
L8c647cca: *stop=0x8c647ccau;r[4]<<=2;goto L8c647ccc; /* shll2 r4 */
L8c647ccc: *stop=0x8c647cccu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c647cce; /* shll r4 */
L8c647cce: *stop=0x8c647cceu;r[4]+=r[3];goto L8c647cd0; /* add r3,r4 */
L8c647cd0: *stop=0x8c647cd0u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[5],4);if(io_status)return io_status;goto L8c647cd2; /* mov.l r5,@(0x14,r4) */
L8c647cd2: *stop=0x8c647cd2u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[6],4);if(io_status)return io_status;goto L8c647cd4; /* mov.l r6,@(0x18,r4) */
L8c647cd4: *stop=0x8c647cd4u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[7],4);if(io_status)return io_status;goto L8c647cd6; /* mov.l r7,@(0x1c,r4) */
L8c647cd6: *stop=0x8c647cd6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c647cd8; /* mov.l @r15,r2 */
L8c647cd8: *stop=0x8c647cd8u;if(!valid_access(r[4]+32u,4u))return -3;wr(m,bus,&io_status,r[4]+0x20u,r[2],4);if(io_status)return io_status;goto L8c647cda; /* mov.l r2,@(0x20,r4) */
L8c647cda: *stop=0x8c647cdau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c647cdc; /* mov.l @(0x4,r15),r1 */
L8c647cdc: *stop=0x8c647cdcu;if(!valid_access(r[4]+36u,4u))return -3;wr(m,bus,&io_status,r[4]+0x24u,r[1],4);if(io_status)return io_status;goto L8c647cde; /* mov.l r1,@(0x24,r4) */
L8c647cde: *stop=0x8c647cdeu;r[1]=1u;goto L8c647ce0; /* mov #0x1,r1 */
L8c647ce0: *stop=0x8c647ce0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647ce2; /* mov.l @(0x8,r15),r2 */
L8c647ce2: *stop=0x8c647ce2u;if(!valid_access(r[4]+40u,4u))return -3;wr(m,bus,&io_status,r[4]+0x28u,r[2],4);if(io_status)return io_status;goto L8c647ce4; /* mov.l r2,@(0x28,r4) */
L8c647ce4: *stop=0x8c647ce4u;if(*pr!=entry_pr)return -3;*stop=0x8c647ce6u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[1],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_countdown_8c652554(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];(void)fr;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c652554;
L8c652554: *stop=0x8c652554u;if(*sr&0x8000u)return -4;*stop=0x8c652554u;r[0]=f->fpscr;goto L8c652556; /* sts FPSCR,r0 */
L8c652556: *stop=0x8c652556u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c652558; /* mov.l r0,@-r15 */
L8c652558: *stop=0x8c652558u;r[1]=16u;goto L8c65255a; /* mov #0x10,r1 */
L8c65255a: *stop=0x8c65255au;r[1]<<=16;goto L8c65255c; /* shll16 r1 */
L8c65255c: *stop=0x8c65255cu;r[0]=~r[0];goto L8c65255e; /* not r0,r0 */
L8c65255e: *stop=0x8c65255eu;r[0]|=r[1];goto L8c652560; /* or r1,r0 */
L8c652560: *stop=0x8c652560u;r[0]=~r[0];goto L8c652562; /* not r0,r0 */
L8c652562: *stop=0x8c652562u;if(*sr&0x8000u)return -4;*stop=0x8c652562u;f->fpscr=r[0];fr=f->bank[(f->fpscr>>21)&1u];goto L8c652564; /* lds r0,fpscr */
L8c652564: *stop=0x8c652564u;if(*sr&0x8000u)return -4;*stop=0x8c652564u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c652566; /* fmov.s fr0,@-r15 */
L8c652566: *stop=0x8c652566u;if(*sr&0x8000u)return -4;*stop=0x8c652566u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c652568; /* fmov.s fr1,@-r15 */
L8c652568: *stop=0x8c652568u;if(*sr&0x8000u)return -4;*stop=0x8c652568u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c65256a; /* fmov.s fr2,@-r15 */
L8c65256a: *stop=0x8c65256au;if(*sr&0x8000u)return -4;*stop=0x8c65256au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c65256c; /* fmov.s fr3,@-r15 */
L8c65256c: *stop=0x8c65256cu;if(*sr&0x8000u)return -4;*stop=0x8c65256cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c65256e; /* fmov.s fr4,@-r15 */
L8c65256e: *stop=0x8c65256eu;if(*sr&0x8000u)return -4;*stop=0x8c65256eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c652570; /* fmov.s fr5,@-r15 */
L8c652570: *stop=0x8c652570u;if(*sr&0x8000u)return -4;*stop=0x8c652570u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c652572; /* fmov.s fr6,@-r15 */
L8c652572: *stop=0x8c652572u;if(*sr&0x8000u)return -4;*stop=0x8c652572u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c652574; /* fmov.s fr7,@-r15 */
L8c652574: *stop=0x8c652574u;if(*sr&0x8000u)return -4;*stop=0x8c652574u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c652576; /* fmov.s fr8,@-r15 */
L8c652576: *stop=0x8c652576u;if(*sr&0x8000u)return -4;*stop=0x8c652576u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c652578; /* fmov.s fr9,@-r15 */
L8c652578: *stop=0x8c652578u;if(*sr&0x8000u)return -4;*stop=0x8c652578u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c65257a; /* fmov.s fr10,@-r15 */
L8c65257a: *stop=0x8c65257au;if(*sr&0x8000u)return -4;*stop=0x8c65257au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c65257c; /* fmov.s fr11,@-r15 */
L8c65257c: *stop=0x8c65257cu;if(*sr&0x8000u)return -4;*stop=0x8c65257cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c65257e; /* fmov.s fr12,@-r15 */
L8c65257e: *stop=0x8c65257eu;if(*sr&0x8000u)return -4;*stop=0x8c65257eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c652580; /* fmov.s fr13,@-r15 */
L8c652580: *stop=0x8c652580u;if(*sr&0x8000u)return -4;*stop=0x8c652580u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c652582; /* fmov.s fr14,@-r15 */
L8c652582: *stop=0x8c652582u;if(*sr&0x8000u)return -4;*stop=0x8c652582u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c652584; /* fmov.s fr15,@-r15 */
L8c652584: *stop=0x8c652584u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c652586; /* sts.l PR,@-r15 */
L8c652586: *stop=0x8c652586u;r[3]=r[4];goto L8c652588; /* mov r4,r3 */
L8c652588: target=r[3];*pr=0x8c65258cu;*stop=0x8c65258au;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65258c; /* jsr @r3 */
L8c65258c: *stop=0x8c65258cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65258e; /* lds.l @r15+,PR */
L8c65258e: *stop=0x8c65258eu;if(*sr&0x8000u)return -4;*stop=0x8c65258eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c652590; /* fmov.s @r15+,fr15 */
L8c652590: *stop=0x8c652590u;if(*sr&0x8000u)return -4;*stop=0x8c652590u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c652592; /* fmov.s @r15+,fr14 */
L8c652592: *stop=0x8c652592u;if(*sr&0x8000u)return -4;*stop=0x8c652592u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c652594; /* fmov.s @r15+,fr13 */
L8c652594: *stop=0x8c652594u;if(*sr&0x8000u)return -4;*stop=0x8c652594u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c652596; /* fmov.s @r15+,fr12 */
L8c652596: *stop=0x8c652596u;if(*sr&0x8000u)return -4;*stop=0x8c652596u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c652598; /* fmov.s @r15+,fr11 */
L8c652598: *stop=0x8c652598u;if(*sr&0x8000u)return -4;*stop=0x8c652598u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65259a; /* fmov.s @r15+,fr10 */
L8c65259a: *stop=0x8c65259au;if(*sr&0x8000u)return -4;*stop=0x8c65259au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65259c; /* fmov.s @r15+,fr9 */
L8c65259c: *stop=0x8c65259cu;if(*sr&0x8000u)return -4;*stop=0x8c65259cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65259e; /* fmov.s @r15+,fr8 */
L8c65259e: *stop=0x8c65259eu;if(*sr&0x8000u)return -4;*stop=0x8c65259eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6525a0; /* fmov.s @r15+,fr7 */
L8c6525a0: *stop=0x8c6525a0u;if(*sr&0x8000u)return -4;*stop=0x8c6525a0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6525a2; /* fmov.s @r15+,fr6 */
L8c6525a2: *stop=0x8c6525a2u;if(*sr&0x8000u)return -4;*stop=0x8c6525a2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6525a4; /* fmov.s @r15+,fr5 */
L8c6525a4: *stop=0x8c6525a4u;if(*sr&0x8000u)return -4;*stop=0x8c6525a4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6525a6; /* fmov.s @r15+,fr4 */
L8c6525a6: *stop=0x8c6525a6u;if(*sr&0x8000u)return -4;*stop=0x8c6525a6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6525a8; /* fmov.s @r15+,fr3 */
L8c6525a8: *stop=0x8c6525a8u;if(*sr&0x8000u)return -4;*stop=0x8c6525a8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6525aa; /* fmov.s @r15+,fr2 */
L8c6525aa: *stop=0x8c6525aau;if(*sr&0x8000u)return -4;*stop=0x8c6525aau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6525ac; /* fmov.s @r15+,fr1 */
L8c6525ac: *stop=0x8c6525acu;if(*sr&0x8000u)return -4;*stop=0x8c6525acu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6525ae; /* fmov.s @r15+,fr0 */
L8c6525ae: *stop=0x8c6525aeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}r[15]+=4;goto L8c6525b0; /* mov.l @r15+,r0 */
L8c6525b0: *stop=0x8c6525b0u;if(*sr&0x8000u)return -4;*stop=0x8c6525b0u;f->fpscr=r[0];fr=f->bank[(f->fpscr>>21)&1u];goto L8c6525b2; /* lds r0,fpscr */
L8c6525b2: *stop=0x8c6525b2u;if(*pr!=entry_pr)return -3;*stop=0x8c6525b4u;return 0; /* rts */
}
